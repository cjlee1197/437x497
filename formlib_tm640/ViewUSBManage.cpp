/*============================================================================+
|  Class    : ViewOriginSet library             	   	                  			|
|  Task     : ViewOriginSet library  header file  	  	                 			|
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
#include	"ViewUSBManage.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include 	"../tmFileExplorer.h"
#include	"../tmshmsg.h"
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define USB_DRIVE_MAX_NUM 3
#define USB_MODE_MAX_NUM 3

#define FILE_COPY_TO_USB	1
#define FILE_COPY_TO_HMI	2

#define Mount_Path"./usb/"

#define AM437_USB_MOUNT_PATH	"/run/media/"
#define AM437_USB_PASS_PATH_1 "mmcblk0p1" 
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
char*	u_pszHMI_Date_Path[] =
{
	"./Prog/",
	"./Data/ScreenShot/",
	"./Data/DatabaseBackup/",
	"./Data/PLC/",
	"./Data/TP/",
	NULL
};

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

BOOL		u_bEnterKey						= FALSE;

CtmWnd*			u_pwndSelectEditHMIType		= NULL;
CtmWnd*			u_pwndBtnSwitchUSB				= NULL;
CtmWnd*			u_pwndBtnUmountUSB				= NULL;
CtmWnd*			u_pwndHMI_DateList					= NULL;
CtmWnd*			u_pwndUSB_DateList					= NULL;
CtmWnd*			u_pwndBtnMoveToUSB				= NULL;
CtmWnd*			u_pwndBtnMoveToHMI				= NULL;
CtmWnd*			u_pwndStaticHMIAlarm			= NULL;
CtmWnd*			u_pwndStaticUSBAlarm			= NULL;
 
CtmWnd*			u_pwndBtnLocalRaname			= NULL; 
CtmWnd*			u_pwndBtnLocalDelete			= NULL;                                             
CtmWnd*			u_pwndBtnUsbRaname				= NULL;                                             
CtmWnd*			u_pwndBtnUsbDelete				= NULL;                              
                                             
int			u_HMI_List_Mode = 0;                 
int			u_USB_Drive_Num = 0;
char		u_USB_Mount_Path[128]="";
BOOL		FristRun			= TRUE;
DIR *d; 
struct dirent *file;
		
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//抓取各元件的指針
	u_pwndSelectEditHMIType		= pwndSender->FindControlFromName("SelectEditHMIDateType");
	u_pwndBtnSwitchUSB				= pwndSender->FindControlFromName("BtnUSBMount");
	u_pwndBtnUmountUSB				= pwndSender->FindControlFromName("btnUmountDevice");
	u_pwndHMI_DateList				= pwndSender->FindControlFromName("HMIFileExplorer");
	u_pwndUSB_DateList				= pwndSender->FindControlFromName("USBFileExplorer");
	u_pwndBtnMoveToUSB				= pwndSender->FindControlFromName("BtnMoveFileToUSB");
	u_pwndBtnMoveToHMI				= pwndSender->FindControlFromName("BtnMoveFileToHMI");
	u_pwndStaticHMIAlarm			= pwndSender->FindControlFromName("StaticHMIAlarm");
	u_pwndStaticUSBAlarm			= pwndSender->FindControlFromName("StaticUSBAlarm");
	
	u_pwndBtnLocalRaname			= pwndSender->FindControlFromName("btnLocalRaname");
	u_pwndBtnLocalDelete			= pwndSender->FindControlFromName("btnLocalDelete");
	u_pwndBtnUsbRaname				= pwndSender->FindControlFromName("btnUsbRaname");
	u_pwndBtnUsbDelete				= pwndSender->FindControlFromName("btnUsbDelete");
	
	
	Update_HMI_List();
	USB_umount();
	//x86 use , am437x mount auto var = value;
	//USB_mount();
	RobotStatus = OP_EDIT_MODE;
	if(u_USB_Drive_Num > 0)
	{
		((CtmFileExplorer*)u_pwndUSB_DateList)->Opendir_File(Mount_Path);
		u_pwndUSB_DateList->Update();
	}
	return	TRUE;
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	
	if(pwndTemp == u_pwndBtnSwitchUSB)
	{
		USB_mount();
		//u_USB_Drive_Num = FindUsbPath(u_USB_Mount_Path);
		if(u_USB_Drive_Num > 0)
		{
			//printf("u_USB_Mount_Path = %s\n",u_USB_Mount_Path);
			((CtmFileExplorer*)u_pwndUSB_DateList)->Opendir_File(Mount_Path);
			u_pwndUSB_DateList->Update();
		}
	}
	else if(pwndTemp == u_pwndBtnLocalRaname)
	{
		//檢查自動模式
		if(CheckCNCMode(OP_MEMORY_MODE,"ROBOT_STR_ALRAM_AUTOING",TRUE))
			return wIDControl;
		//檢查光標位置
		int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
		CtmWnd*	pwndTemp1	= NULL;
		pwndTemp1	= pwndSender->FindControlFromTab(ViewTabStop);
		if(pwndTemp1 != u_pwndHMI_DateList) return wIDControl;
		
		//更改檔名 g_Now_FileName
		memset(g_Now_FileName,'\0',sizeof(g_Now_FileName));
		MsgBoxCall("msgboxFileReName.txt",0);
		//MsgBox(g_MultiLanguage["ROBOT_MSG_FILE_DEL_SURE"], tmFT_CODE_TECH);
		//printf("g_Now_FileName=%s\n",g_Now_FileName);
		if(g_Now_FileName[0]!='\0')
		{
			char FileName1[128];	//被更改檔名
			char FileName2[128];	//新檔名
			char cmd[256];				//命令
			memset(FileName1,0,sizeof(FileName1));
			memset(FileName2,0,sizeof(FileName2));
			memset(cmd,0,sizeof(cmd));
			
			sprintf(FileName2,"%s/%s",u_pszHMI_Date_Path[u_HMI_List_Mode],g_Now_FileName);
			//printf("FileName2=%s\n",FileName2);
			if(file_exists(FileName2))	//新檔名已存在
			{
				MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
				return wIDControl; 
			}
			//檢查檔案名稱格式
			//printf("%d = CheckFileName\n",CheckFileName(g_Now_FileName,u_HMI_List_Mode));
			if(CheckFileName(g_Now_FileName,u_HMI_List_Mode)!=1)
			{
				MsgBoxCall("msgboxFourStatic.txt",4,"ROBOT_STR_FILENAME_ERROR","ROBOT_STR_PROG_NAME_FORMAT"
									,"ROBOT_STR_PLC_NAME_FORMAT","ROBOT_STR_ACTION_CONTINUE");
			}
			else	g_bMsgBoxFlag=TRUE;
			memset(g_Now_FileName,'\0',sizeof(g_Now_FileName));
			if(g_bMsgBoxFlag)
			{
				g_bMsgBoxFlag=FALSE;
				((CtmFileExplorer*)u_pwndHMI_DateList)->GetCurrentFilePath(FileName1);
				sprintf(cmd,"mv %s %s",FileName1,FileName2);
				system(cmd);
				system("sync");
				((CtmFileExplorer*)u_pwndHMI_DateList)->Opendir_File(u_pszHMI_Date_Path[u_HMI_List_Mode]);
				u_pwndHMI_DateList->Update();
			}
		}
	}
	else if(pwndTemp == u_pwndBtnLocalDelete)
	{			
		//檢查光標位置
		int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
		CtmWnd*	pwndTemp1	= NULL;
		pwndTemp1	= pwndSender->FindControlFromTab(ViewTabStop);
		if(pwndTemp1 != u_pwndHMI_DateList) return wIDControl;
		
		//確定刪除 g_bMsgBoxFlag
		MsgBox(g_MultiLanguage["ROBOT_MSG_FILE_DEL_SURE"], tmFT_CODE_TECH);
		if(g_bMsgBoxFlag)
		{
			g_bMsgBoxFlag = FALSE;
			char FileName1[256]; //被刪除檔案路徑
			char cmd[256]; //命令
			memset(FileName1,0,sizeof(FileName1));
			memset(cmd,0,sizeof(cmd));
			((CtmFileExplorer*)u_pwndHMI_DateList)->GetCurrentFilePath(FileName1);
			sprintf(cmd,"rm %s",FileName1);
			system(cmd);
			system("sync");
			((CtmFileExplorer*)u_pwndHMI_DateList)->Opendir_File(u_pszHMI_Date_Path[u_HMI_List_Mode]);
			u_pwndHMI_DateList->Update();
		}
	}
	else if(pwndTemp == u_pwndBtnUmountUSB)
	{
		USB_umount();
		((CtmFileExplorer*)u_pwndUSB_DateList)->Opendir_File(Mount_Path);
		u_pwndUSB_DateList->Update();
	}
	else if(u_USB_Drive_Num > 0)
	{
		if(pwndTemp == u_pwndBtnMoveToUSB)
		{
			//檢查光標位置
			int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
			CtmWnd*	pwndTemp1	= NULL;
			pwndTemp1	= pwndSender->FindControlFromTab(ViewTabStop);
			if(pwndTemp1 == u_pwndHMI_DateList)
			{
				char FileName1[128]; //被複製檔案路徑
				char FileName2[128]; //新檔案路徑
				memset(FileName1,0,sizeof(FileName1));
				memset(FileName2,0,sizeof(FileName2));
				((CtmFileExplorer*)u_pwndHMI_DateList)->GetCurrentFilePath(FileName1);
				int nFocusIndex = ((CtmFileExplorer*)u_pwndUSB_DateList)->GetFocusIndex();
				sprintf(FileName2,"%s/%s",Mount_Path,((CtmFileExplorer*)u_pwndHMI_DateList)->GetCurrentFileName(nFocusIndex));
				if(file_exists(FileName2))
					MsgBox(g_MultiLanguage["ROBOT_STR_COPY_MESSAGE"], tmFT_CODE_TECH);
				else
					g_bMsgBoxFlag = TRUE;
				if(g_bMsgBoxFlag)
				{
					g_bMsgBoxFlag=FALSE;
					CopyFile(FileName1,Mount_Path);
					((CtmFileExplorer*)u_pwndUSB_DateList)->Opendir_File(Mount_Path);
					u_pwndUSB_DateList->Update();
				}
			}
		}
		if(pwndTemp == u_pwndBtnMoveToHMI)
		{
			if(CheckCNCMode(OP_MEMORY_MODE,"ROBOT_STR_ALRAM_AUTOING",TRUE))
				return wIDControl;
			int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
			CtmWnd*	pwndTemp1	= NULL;
			pwndTemp1	= pwndSender->FindControlFromTab(ViewTabStop);
			if(pwndTemp1 == u_pwndUSB_DateList)
			{
				char FileName1[128]; //被複製檔案路徑
				char FileName2[128]; //新檔案路徑
				memset(FileName1,0,sizeof(FileName1));
				memset(FileName2,0,sizeof(FileName2));
				((CtmFileExplorer*)u_pwndUSB_DateList)->GetCurrentFilePath(FileName1);
				int nFocusIndex = ((CtmFileExplorer*)u_pwndUSB_DateList)->GetFocusIndex();
				sprintf(FileName2,"%s%s",u_pszHMI_Date_Path[u_HMI_List_Mode],((CtmFileExplorer*)u_pwndUSB_DateList)->GetCurrentFileName(nFocusIndex));
				if(file_exists(FileName2))
					MsgBox(g_MultiLanguage["ROBOT_STR_COPY_MESSAGE"], tmFT_CODE_TECH);
				else
					g_bMsgBoxFlag = TRUE;
				if(g_bMsgBoxFlag)
				{
					CopyFile(FileName1,u_pszHMI_Date_Path[u_HMI_List_Mode]);
					((CtmFileExplorer*)u_pwndHMI_DateList)->Opendir_File(u_pszHMI_Date_Path[u_HMI_List_Mode]);
					u_pwndHMI_DateList->Update();
					g_bMsgBoxFlag = FALSE;
					if(u_HMI_List_Mode==0 || u_HMI_List_Mode==4)
						Etnernet_File(FileName2);
				}
			}
		}
		else if(pwndTemp == u_pwndBtnUsbRaname)
		{			
			//檢查光標位置
			int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
			CtmWnd*	pwndTemp1	= NULL;
			pwndTemp1	= pwndSender->FindControlFromTab(ViewTabStop);
			if(pwndTemp1 != u_pwndUSB_DateList) return wIDControl;
			
			//更改檔名 g_Now_FileName
			memset(g_Now_FileName,'\0',sizeof(g_Now_FileName));
			MsgBoxCall("msgboxFileReName.txt",0);
			if(g_Now_FileName[0]!='\0')
			{
				char FileName1[128];	//被更改檔名
				char FileName2[128];	//新檔名
				char cmd[256];				//命令
				memset(FileName1,0,sizeof(FileName1));
				memset(FileName2,0,sizeof(FileName2));
				memset(cmd,0,sizeof(cmd));
				
				sprintf(FileName2,"%s/%s",Mount_Path,g_Now_FileName);
				//printf("FileName2=%s\n",FileName2);
				if(file_exists(FileName2))	//新檔名已存在
				{
					MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
					return wIDControl; 
				}
				((CtmFileExplorer*)u_pwndUSB_DateList)->GetCurrentFilePath(FileName1);
				sprintf(cmd,"mv %s %s",FileName1,FileName2);
				system(cmd);
				system("sync");
				((CtmFileExplorer*)u_pwndUSB_DateList)->Opendir_File(Mount_Path);
				u_pwndHMI_DateList->Update();
			}
		}
		else if(pwndTemp == u_pwndBtnUsbDelete)
		{
			int	ViewTabStop = ((CtmFormView*)pwndSender)->Get_TabStop();
			CtmWnd*	pwndTemp1	= NULL;
			pwndTemp1	= pwndSender->FindControlFromTab(ViewTabStop);
			if(pwndTemp1 != u_pwndUSB_DateList) return wIDControl;
				
			MsgBox(g_MultiLanguage["ROBOT_MSG_FILE_DEL_SURE"], tmFT_CODE_TECH);
			if(g_bMsgBoxFlag)
			{
				g_bMsgBoxFlag = FALSE;
				char FileName1[256]; //被刪除的檔案
				char cmd[256]; 			//命令
				memset(FileName1,0,sizeof(FileName1));
				memset(cmd,0,sizeof(cmd));
				((CtmFileExplorer*)u_pwndUSB_DateList)->GetCurrentFilePath(FileName1);
				sprintf(cmd,"rm %s",FileName1);
				system(cmd);
				system("sync");
				((CtmFileExplorer*)u_pwndUSB_DateList)->Opendir_File(Mount_Path);
				u_pwndUSB_DateList->Update();
			}
		}
	}
	return	wIDControl;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	if(u_pwndSelectEditHMIType == pwndTemp)
	{
		if(((CtmSelectEdit*)u_pwndSelectEditHMIType)->GetIndex() != u_HMI_List_Mode)
			u_HMI_List_Mode = ((CtmSelectEdit*)u_pwndSelectEditHMIType)->GetIndex();
		Update_HMI_List();
	}
	return	wIDControl;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL flag = CheckCNCMode(OP_MEMORY_MODE,NULL,TRUE);
	static BOOL Frist = TRUE;
	if(CheckCNCMode(OP_MEMORY_MODE,NULL,TRUE) && !flag)
	{
		flag = !flag;
		u_pwndStaticHMIAlarm->SetPropValueT("visible",(double)TRUE);
		u_pwndStaticHMIAlarm->Update();
	}
	else if(!CheckCNCMode(OP_MEMORY_MODE,NULL,TRUE) && flag)
	{
		flag = !flag;
		u_pwndStaticHMIAlarm->SetPropValueT("visible",(double)FALSE);
		u_pwndStaticHMIAlarm->Update();
	}
	
	if(Frist)
	{
		if(flag)
			u_pwndStaticHMIAlarm->SetPropValueT("visible",(double)TRUE);
		else
			u_pwndStaticHMIAlarm->SetPropValueT("visible",(double)FALSE);
		u_pwndStaticHMIAlarm->Update();
		Frist = FALSE;
	}
		
}
/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void	Update_HMI_List()
{
	((CtmFileExplorer*)u_pwndHMI_DateList)->Opendir_File(u_pszHMI_Date_Path[u_HMI_List_Mode]);
	u_pwndHMI_DateList->Update();
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
		u_pwndStaticUSBAlarm->SetPropValueT("visible",(double)FALSE);
		u_pwndStaticUSBAlarm->Update();
	}
	else
	{
		u_USB_Drive_Num = 0;
		u_pwndStaticUSBAlarm->SetPropValueT("visible",(double)TRUE);
		u_pwndStaticUSBAlarm->Update();
	}
	//printf("u_USB_Drive_Num = %d\n",u_USB_Drive_Num);
}

void CopyFile(char * FilePath , char * ToPath)
{
	char command[128];
	sprintf(command,"cp %s %s",FilePath,ToPath);
	//printf("command %s\n",command);
	system(command);
	system("sync");
}

BOOL file_exists (char * fileName)
{
  struct stat buf;
	int i = stat ( fileName, &buf );
  /* File found */
  if ( i == 0 )
    return TRUE;
  return FALSE;
} 

/*int	FindUsbPath(char* return_path)
{
	u_USB_Drive_Num=0;
	d = opendir(AM437_USB_MOUNT_PATH);
	if(d==NULL) return;
	int filetype;
	while((file = readdir(d)) != NULL)
	{
		if(!strcmp(file->d_name,AM437_USB_PASS_PATH_1)) continue;
		filetype = CheckFile(file->d_name);
		printf("FileName = %s filetype=%d\n",file->d_name,filetype);
		if(filetype == -1)
		{
			u_USB_Drive_Num++;
			memset(return_path,0,sizeof(return_path));
			sprintf(return_path,"%s%s",AM437_USB_MOUNT_PATH,file->d_name);
		}
	}
	closedir(d);
	if(u_USB_Drive_Num>0)
	{
		u_pwndStaticUSBAlarm->SetPropValueT("visible",(double)FALSE);
		u_pwndStaticUSBAlarm->Update();
	}
	else
	{
		u_pwndStaticUSBAlarm->SetPropValueT("visible",(double)TRUE);
		u_pwndStaticUSBAlarm->Update();
	}
	
	return u_USB_Drive_Num;
}*/


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
	closedir(d);		u_USB_Drive_Num = 0;
	u_pwndStaticUSBAlarm->SetPropValueT("visible",(double)TRUE);
	u_pwndStaticUSBAlarm->Update();
}

BOOL	CheckCNCMode(long mode,char* msg,BOOL True)
{
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt == mode)
	{
		if(True && msg!=NULL)
			MsgBox(g_MultiLanguage[msg], tmFT_CODE_TECH);
		return TRUE;
	}
	else
	{
		if(!True && msg!=NULL)
			MsgBox(g_MultiLanguage[msg], tmFT_CODE_TECH);
		return FALSE;
	}
	return FALSE;
}

int CheckFileName(char* FileName, int Mode)
{
	if(Mode==0)
	{
		if(*FileName != 'O') return -1;
		FileName++;
		if(*(FileName) < '0' || *FileName > '9' ) return -2;
		FileName++;
		if(*(FileName) < '0' || *FileName > '9' ) return -3;
		FileName++;
		if(*(FileName) < '0' || *FileName > '9' ) return -4;
		FileName++;
		if(*(FileName) < '0' || *FileName > '9' ) return -5;
		FileName++;
		if(*(FileName) != '\0' ) return -6;
		return 1;
	}
	else if(Mode==3)
	{
		if(*FileName != 'P') return -301;
		FileName++;
		if(*(FileName) != 'L') return -302;
		FileName++;
		if(*(FileName) != 'C') return -303;
		FileName++;
		if(*(FileName) != '_') return -304;
		FileName++;
		if(*(FileName) != 'I' && *FileName != 'O' && *FileName != 'C' && *FileName != 'S' && *FileName != 'A') return -305;
		FileName++;
		if(*(FileName) != '.') return -306;
		FileName++;
		if(*(FileName) != 'c' && *FileName != 'C') return -307;
		FileName++;
		if(*(FileName) != 's' && *FileName != 'S') return -308;
		FileName++;
		if(*(FileName) != 'v' && *FileName != 'V') return -309;
		FileName++;
		if(*(FileName) != '\0' ) return -310;
		return 1;
	}
	else 
		return 1;
		
}