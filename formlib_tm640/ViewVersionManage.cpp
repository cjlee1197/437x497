/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	| 
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewVersionManage.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include	"../tmshmsg.h"
#include 	"../taskcnc.h"
#include 	"../gui.h"
#include 	"../tmFileExplorer.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

//JogLock
#define		COLOR_GRAY					50712	//灰色
#define		COLOR_BLUE					1046	//藍色蓋
#define		JOGLOCK_ON_EXTAG		20002	//上鎖
#define		JOGLOCK_OFF_EXTAG		0			//開鎖

#define	SEL_COLOR							0xFC00
#define	NO_SEL_COLOR						0x0416

#define	BACKUPPATH "/work/BackLogFile"
#define	UPDATEPATH "/work/UpdateFile"
#define	NOWPATH "./"

#define USB_DRIVE_MAX_NUM 3
#define USB_MODE_MAX_NUM 3
#define Mount_Path"./usb/"

#define AM437_USB_MOUNT_PATH	"/run/media/"
#define AM437_USB_PASS_PATH_1 "mmcblk0p1" 

#define D_PROGTYPE_HMI 0
#define D_PROGTYPE_MOTION 1
#define D_PROGTYPE_PLC 2

#define STATE_SINGLESTEP        2
#define	STATE_MANUAL						3
#define STATE_FULLAUTO          4 

char*	u_pszUSB_Drive_Path[] =
{
	"/dev/sda1",
	"/dev/sda2",
	"/dev/sda3",
	"/dev/sdb1",
	"/dev/sdc1",
	"/dev/sdd1",
	NULL
};

char*	u_pszUSB_Mode[] =
{
	"msdos",
	"vfat",
	"ntfs",
	NULL
};


/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/

CtmWnd*		u_SelectProg		= NULL;
CtmWnd*		u_ManageVersion		= NULL;
CtmWnd*		u_BtnOpenUsb		= NULL;
CtmWnd*		u_BtnOpenBk		= NULL;
CtmWnd*		u_BtnReadVersion		= NULL;
CtmWnd*		u_TextNowVersionInfo	= NULL;
CtmWnd*		u_TextSelVersionInfo	= NULL;
CtmWnd*		u_BtnReadyUpdate			= NULL;
CtmWnd*		u_BtnReStart					= NULL;
CtmWnd*		u_BtnBkProgToUSB			= NULL;
	
WORD        u_wPickerOPSatus    = 0; // 機械手狀態

char UpdatePath[1024];
BOOL SelUpdate=FALSE;
BOOL ReadyUpdate=FALSE;
int  ProgType=0; 
          
int			u_USB_Drive_Num = 0;
char		u_USB_Mount_Path[128]="";
DIR *d; 
struct dirent *file;
enum TabOrderNum{Tab_OPEN_BK=0,Tab_END};
enum BGCNum{BGC_OPEN_BK=0,BGC_END};
int taborder[Tab_END];
int bgc[BGC_END];
int	wait_tcp_req=0;;
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_SelectProg 		= pwndSender->FindControlFromName("SelectProg");
	u_ManageVersion 		= pwndSender->FindControlFromName("VersionManage");
	u_BtnOpenUsb 		= pwndSender->FindControlFromName("OpenUsb");
	u_BtnOpenBk 		= pwndSender->FindControlFromName("OpenBk");
	u_BtnReadVersion 		= pwndSender->FindControlFromName("ReadVer");
	u_TextNowVersionInfo 		= pwndSender->FindControlFromName("NowInfoText");
	u_TextSelVersionInfo 		= pwndSender->FindControlFromName("SelInfoText");
	u_BtnReadyUpdate 		= pwndSender->FindControlFromName("SetUpdateFile");
	u_BtnReStart 		= pwndSender->FindControlFromName("ReStart");
	u_BtnBkProgToUSB	 		= pwndSender->FindControlFromName("BkProgToUSB");
	//SetOldVerManage(BACKUPPATH,u_ManageOldVersion);
	ReadVersionInfo(NOWPATH,u_TextNowVersionInfo); // 
	return	TRUE;
}  

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_BtnOpenUsb) // 開啟USB設備
	{
		USB_mount();
		if(u_USB_Drive_Num > 0)
		{
			((CtmFileExplorer*)u_ManageVersion)->Opendir_File(Mount_Path);
			u_ManageVersion->Update();
		}
	}
	else if(pwndTemp == u_BtnOpenBk) // 開啟版本備份
	{
		((CtmFileExplorer*)u_ManageVersion)->Opendir_File(BACKUPPATH);
		u_ManageVersion->Update();
	}
	else	if(pwndTemp == u_BtnReadVersion) // 查看版本資訊
	{
		memset(UpdatePath,'\0',sizeof(UpdatePath));
		SelUpdate = GetPath(UpdatePath);
		ReadVersionInfo(UpdatePath,u_TextSelVersionInfo);
	}
	else	if(pwndTemp == u_BtnReadyUpdate) // 準備更新資料
	{
		printf("Press u_BtnReadyUpdate\n");
		//if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_JOG_MODE)
		if(u_wPickerOPSatus  != STATE_MANUAL) // 不在手動狀態
		{
			MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_SEITCH_MANUAL"); // 請切換至手動模式再進行操作
			return wIDControl;
		}
		//printf("pwndTemp == u_BtnReadyUpdate\n");
		if(SelUpdate) // 版本存在
		{
			MsgBox(g_MultiLanguage["UPDATE_STR_UPDATE_CONFIRM"], tmFT_CODE_TECH); // 確認更新所選版本
			if (!g_bMsgBoxFlag)	return wIDControl;
				
			u_BtnReadyUpdate->SetPropValueT("captionID","ROBOT_STR_READ_UPDATEING_FILE"); // 複製檔案
			u_BtnReadyUpdate->CreateA();
			u_BtnReadyUpdate->Update();
			
			ReadyUpdate = ReadyUpdateFile(UpdatePath);
			if(!ReadyUpdate) // 複製檔案失敗
			{
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_READY_UPDATE_ERROR");
				u_BtnReadyUpdate->SetPropValueT("captionID","ROBOT_STR_READ_UPDATE_FILE");
				u_BtnReadyUpdate->CreateA();
				u_BtnReadyUpdate->Update();
			}
			else // 更新檔案 複製至UpdateFile完成
			{
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_UPDATE_DONE");
				//MsgBoxCall("msgboxFourStaticNoChk.txt",3,"ROBOT_STR_READY_UPDATE_OK","ROBOT_STR_READY_UPDATE_OK1","ROBOT_STR_READY_UPDATE_OK2");
			}
		}
		else
			MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_NOT_SEL_VERSION");
	}
	else	if(pwndTemp == u_BtnReStart) // 重啟系統
	{
		//if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_JOG_MODE)
		if(u_wPickerOPSatus  != STATE_MANUAL) // 不在手動狀態
		{
			MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_SEITCH_MANUAL");
			return wIDControl;
		}
		if(ReadyUpdate)
			{
				printf("reboot!!!!\n");
				system("reboot");
			//MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_NO_SUP_REBOOT");
			}
		else
			MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_UPDATE_NOT_READY");
	}
	else	if(pwndTemp == u_BtnBkProgToUSB)
	{
		if(u_USB_Drive_Num<=0)
		{
			MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_PLEASE_MOUNT_USB");
			return wIDControl;
		}
		if(ReadyUpdate)
		{
			
			
		}
	}
	return wIDControl;
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_SelectProg)
	{
		USB_umount();
		ProgType = ((CtmSelectEdit*)u_SelectProg)->GetIndex();
		if(ProgType == D_PROGTYPE_HMI)
		{
			printf("D_PROGTYPE_HMI\n");
			u_BtnOpenBk->SetPropValueT("taborder",taborder[Tab_OPEN_BK]);
			u_BtnOpenBk->SetPropValueT("bgc",bgc[BGC_OPEN_BK]);
		}
		else if(ProgType == D_PROGTYPE_MOTION)
		{
			printf("D_PROGTYPE_MOTION\n");
			u_BtnOpenBk->SetPropValueT("taborder",-2);
			u_BtnOpenBk->SetPropValueT("bgc",COLOR_GRAY);
		}
		else if(ProgType == D_PROGTYPE_PLC)
		{
			printf("D_PROGTYPE_PLC\n");
			u_BtnOpenBk->SetPropValueT("taborder",-2);
			u_BtnOpenBk->SetPropValueT("bgc",COLOR_GRAY);
		}
		ReadVersionInfo(NOWPATH,u_TextNowVersionInfo);
		u_BtnOpenBk->Update();
		((CtmFileExplorer*)u_ManageVersion)->Opendir_File("CLEAR");
		u_ManageVersion->Update();
	}
	return wIDControl;
}

void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{	

}

void	OnDestroyA(CtmWnd* pwndSender)
{
	USB_umount();
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue; // 機械手狀態
	static BOOL StartRunOne = TRUE;
	if(StartRunOne)
	{
		u_BtnOpenBk->GetPropValueT("taborder",&(taborder[Tab_OPEN_BK]),sizeof(taborder[Tab_OPEN_BK]));
		u_BtnOpenBk->GetPropValueT("bgc",&(bgc[BGC_OPEN_BK]),sizeof(bgc[BGC_OPEN_BK]));
		StartRunOne=FALSE;
	}
	
//	if(TCP_Update_Rb_Plc==FALSE && wait_tcp_req==1)
//	{
//		wait_tcp_req=0;
//		u_BtnReadyUpdate->SetPropValueT("captionID","ROBOT_STR_READ_UPDATE_FILE");
//		u_BtnReadyUpdate->CreateA();
//		u_BtnReadyUpdate->Update();
//	}
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void	ReadVersionInfo(char* path , CtmWnd* ptr)
{
	char verfilepath[1024];
	memset(verfilepath,'\0',sizeof(verfilepath));
	if(ProgType == D_PROGTYPE_HMI)
		sprintf(verfilepath,"%s/hmiversion.ini\0",path);
	else if(ProgType == D_PROGTYPE_MOTION)
		sprintf(verfilepath,"%s/motionversion.ini\0",path);
	else if(ProgType == D_PROGTYPE_PLC)
		sprintf(verfilepath,"%s/plcversion.ini\0",path);
		
	printf("verfilepath  %s\n",verfilepath);
	if(file_exists(verfilepath))
	{
		((CtmMTextMemo*)ptr)->LoadFromFile(verfilepath, 0,TRUE);
		((CtmMTextMemo*)ptr)->GotoLine(0);
		((CtmMTextMemo*)ptr)->Update();
	}
	else
	{
		MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_READ_VERSION_ERROR"); // 讀取版本失敗
		SelUpdate = FALSE;
	}
}

void	USB_mount()
{
	int state=-1; 
	state = umount2("usb/",MNT_FORCE);
	int g_Drive_Num = u_USB_Drive_Num;
	int driveCnt = 0;
	state=-1;
	while(driveCnt < USB_DRIVE_MAX_NUM)
	{
		g_Drive_Num ++;
		if(g_Drive_Num > USB_DRIVE_MAX_NUM) g_Drive_Num = 1;
		int USBModeCnt = 0;
		//printf("u_pszUSB_Drive_Path[%d] = %s\n",g_Drive_Num-1,u_pszUSB_Drive_Path[g_Drive_Num-1]);
		while(state!=0 && USBModeCnt<USB_MODE_MAX_NUM)	
		{
			state = mount(u_pszUSB_Drive_Path[g_Drive_Num-1] , Mount_Path , u_pszUSB_Mode[USBModeCnt] , 0 , "shortname=winnt" );
			if(state == 0)	
				break;
			USBModeCnt++;
			//perror("mount");
		}
		if(state == 0)	
				break;
		driveCnt++;
	}
	
	//printf("u_pszUSB_Drive_Path[%d] = %s\n",g_Drive_Num-1,u_pszUSB_Drive_Path[g_Drive_Num-1]);
	if(state == 0)
	{
		u_USB_Drive_Num = g_Drive_Num;
		MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_FILE_USB_UMOUNT");
	}
	else
	{
		u_USB_Drive_Num = 0;
		MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_FILE_USB_ERROR");
	}
	//printf("u_USB_Drive_Num = %d\n",u_USB_Drive_Num);
}

void	USB_umount()
{
	umount2("usb/",MNT_FORCE);
	//memset(u_USB_Mount_Path,0,sizeof(u_USB_Mount_Path));
	char umount_path[128];
	u_USB_Drive_Num=0;
	d = opendir(AM437_USB_MOUNT_PATH);
	if(d==NULL) return;
	int filetype;
	while((file = readdir(d)) != NULL)
	{
		if(!strcmp(file->d_name,AM437_USB_PASS_PATH_1)) continue;
		filetype = CheckFile(file->d_name);
		if(filetype == -1)
		{
			memset(umount_path,0,sizeof(umount_path));
			sprintf(umount_path,"%s%s",AM437_USB_MOUNT_PATH,file->d_name);
			umount2(umount_path,MNT_FORCE);
		}
	}
	closedir(d);		
}

BOOL GetPath(char* path)
{
	char *path1;
	sprintf(path,"%s/%s",((CtmFileExplorer*)u_ManageVersion)->GetCurrentPath(),((CtmFileExplorer*)u_ManageVersion)->GetCurrentDirName());
	printf("path = %s\n",path);
	return TRUE;
}

BOOL ReadyUpdateFile(char* path) // 準備更新檔案
{
	printf("ReadyUpdateFile %s\n",path);
	char cmd[1024];
	memset(cmd,'\0',sizeof(cmd));
	
	if(ProgType == D_PROGTYPE_HMI) // 更新HMI
	{
		sprintf(cmd,"cp -rf %s %s/437xhmi",path,UPDATEPATH);
		printf("cmd: %s\n",cmd);
		system(cmd);
		system("sync");
		system("sync");
		system("sync");
	}
	else if(ProgType == D_PROGTYPE_MOTION) // 更新 motion
	{
		system("mkdir tmp_tar_dir");
		sprintf(cmd,"cp -r %s/* tmp_tar_dir/",path);
		system(cmd);
		memset(cmd,'\0',sizeof(cmd));
		sprintf(cmd,"tar -zcvf rb.tar.gz tmp_tar_dir",path);
		system(cmd);
		system("sync");
//		if(Etnernet_File ("rb.tar.gz",RB_PROG) > 0)
//		{ 
//			TCP_Update_Rb_Plc==TRUE;
//			wait_tcp_req=1;
//		}
		system("rm -f rb.tar.gz");
		system("rm -fr tmp_tar_dir");
		system("sync");
		
	}
	else if(ProgType == D_PROGTYPE_PLC) // 更新 PLC
	{
		system("mkdir tmp_tar_dir");
		sprintf(cmd,"cp -r %s/* tmp_tar_dir/",path);
		system(cmd);
		memset(cmd,'\0',sizeof(cmd));
		sprintf(cmd,"tar -zcvf plc.tar.gz tmp_tar_dir",path);
		system(cmd);
		system("sync");
//		if(Etnernet_File ("plc.tar.gz",RB_PROG) > 0)
//		{ 
//			TCP_Update_Rb_Plc==TRUE;
//			wait_tcp_req=1;
//		}
		system("rm -f plc.tar.gz");
		system("rm -fr tmp_tar_dir");
		system("sync");
	}
	
	return TRUE;
}


BOOL BackupProgToUSB(char* path)
{
	
	/*if(ProgType == D_PROGTYPE_HMI)
	{
		sprintf(cmd,"cp -rf %s %s/437xhmi",path,UPDATEPATH);
		system(cmd);
		system("sync");
	}
	else if(ProgType == D_PROGTYPE_MOTION)
	{
		system("mkdir tmp_tar_dir");
		sprintf(cmd,"cp -r %s/* tmp_tar_dir/",path);
		system(cmd);
		memset(cmd,'\0',sizeof(cmd));
		sprintf(cmd,"tar -zcvf rb.tar.gz tmp_tar_dir",path);
		system(cmd);
		system("sync");
		if(Etnernet_File ("rb.tar.gz",RB_PROG) > 0)
		{ 
			TCP_Update_Rb_Plc==TRUE;
			wait_tcp_req=1;
		}
		system("rm -f rb.tar.gz");
		system("rm -fr tmp_tar_dir");
		system("sync");
		
	}
	else if(ProgType == D_PROGTYPE_PLC)
	{
		system("mkdir tmp_tar_dir");
		sprintf(cmd,"cp -r %s/* tmp_tar_dir/",path);
		system(cmd);
		memset(cmd,'\0',sizeof(cmd));
		sprintf(cmd,"tar -zcvf plc.tar.gz tmp_tar_dir",path);
		system(cmd);
		system("sync");
		if(Etnernet_File ("plc.tar.gz",RB_PROG) > 0)
		{ 
			TCP_Update_Rb_Plc==TRUE;
			wait_tcp_req=1;
		}
		system("rm -f plc.tar.gz");
		system("rm -fr tmp_tar_dir");
		system("sync");
	}*/
}