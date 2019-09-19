/*============================================================================+
|  Class    : ViewProg_FileManage library                	                 		|
|  Task     : ViewProg_FileManage library  header file                        |
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                      |
|  Link     :G++(GCC)3.2                                                      |
|  Call     :                                                                 |
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                        |
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewFileManage.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include 	"../tmFileExplorer.h"
#include	"../wnd.h"
#include	"../tmshmsg.h"

/*============================================================================+
|           Constant                                                         	|
+============================================================================*/

#define		INDEX_FILE_EXPLORERD		1								//左方FileExplorer元件的taborder值
#define		INDEX_FILE_EXPLORER			2								//右方FileExplorer元件的taborder值

//判斷要執行什麼功能處理
#define		FILECONTROL_TYPE_COPY			0
#define		FILECONTROL_TYPE_RENAME		1
#define		FILECONTROL_TYPE_CREATE		2

#define		MSGBOX_FILECONTROL				0

/*============================================================================+
|           Global variable                                                   |
+============================================================================*/

CtmWnd*		u_pwndFileExplorerD						= NULL;
CtmWnd*		u_pwndFileExplorer						= NULL;
CtmWnd*		u_pwndSelectEditFileDir				= NULL;

char			u_sOperate_FileName[64]				= "\0";		//要進行處理的檔案路徑
char			u_sNewFileName_FromMB[64]			= "\0";		//使用者在提示小視窗裡輸入的檔案名稱

char			u_sCopy_FileName[64]					=	"\0";		//儲存被複製檔案的路徑 20140914 by mario

int mode=0;
/*============================================================================+
|              Function	                                                      |
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)		//就算從toolbar1切換至toolbar11也不會重新創建
{
	//抓取各元件指針
	u_pwndFileExplorerD				= pwndSender->FindControlFromName("FileExplorerD");		//左方目錄列表
	u_pwndFileExplorer				= pwndSender->FindControlFromName("FileExplorer");		//右方檔案列表
	
	u_pwndSelectEditFileDir		= pwndSender->FindControlFromName("SelectEditFileDir");		//下拉式選單
	
	//開啟給使用者操作的目錄
	((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File("./Prog");
	
	g_Now_FileName[0] = '\0';			//重置
	ReLoadDirectoryAndFile();

	RobotStatus = OP_EDIT_MODE;
	return TRUE;
}

WORD		OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	if(wIDControl == INDEX_FILE_EXPLORER)
	{
		strcpy(g_szProgDirectory, g_pFileExplPath);
		int nFocusIndex = ((CtmFileExplorer*)u_pwndFileExplorer)->GetFocusIndex();
		sprintf(g_Now_FileName, "%s/%s", g_szProgDirectory,((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFileName(nFocusIndex));
	}
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
		if((wKey == _ENTER)&&\
		(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORERD))
		{
			//判斷左方列表的光標所在位置是否有項目
			if(((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirName() != NULL)
			{
				ReLoadDirectoryAndFile();
				((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);		//在切換目錄後讓光標歸位
			}
		}
		else if((wKey == _ENTER)&&\
			(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORER))
		{
			MsgBoxCall("msgboxFileOpen.txt");
		}
		else if(wKey == _KEY_DOWNFIELD)
		{
			if(!FocusLock(pwndSender))
				((CtmFormView*)pwndSender)->OnKey1(wKey);
		}
		else
			((CtmFormView*)pwndSender)->OnKey1(wKey);

}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
//20140923↓		控制進入編輯頁的相關設定
	if(wIDControl == 2)
	{
		memset(g_Now_FileName, 0, sizeof(g_Now_FileName));
	}
	
	CtmWnd*	pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_pwndSelectEditFileDir)
	{
		if(((CtmSelectEdit*)u_pwndSelectEditFileDir)->GetIndex() == 0 && ((CtmSelectEdit*)u_pwndSelectEditFileDir)->GetIndex() != mode)
		{
			mode = ((CtmSelectEdit*)u_pwndSelectEditFileDir)->GetIndex();
			((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File("./Prog");
			ReLoadDirectoryAndFile();
		}
		else if(((CtmSelectEdit*)u_pwndSelectEditFileDir)->GetIndex() == 1 && ((CtmSelectEdit*)u_pwndSelectEditFileDir)->GetIndex() != mode)
		{
			int state; 
			state = umount2("usb/",MNT_FORCE);
			//state = umount("/work/i86/usb");
			perror("errno");
			state = mount("/dev/sdb1" , "usb/" , "msdos" , 0 , NULL );
			if(state != 0)
				state = mount("/dev/sdc1" , "usb/" , "msdos" , 0 , NULL );
			perror("errno");
			if(state == 0)
			{
				((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File("./usb");
			}
			else	
			{
				((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File("./Prog");
				((CtmSelectEdit*)u_pwndSelectEditFileDir)->SetIndex(0);
				u_pwndSelectEditFileDir->Update();
			}
			mode = ((CtmSelectEdit*)u_pwndSelectEditFileDir)->GetIndex();
			ReLoadDirectoryAndFile();
		}
		
		((CtmFormView*)pwndSender)->Goto(wIDControl);
	}
	
		return wIDControl;
}


void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY )
	{
		if(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORER)	//Get_TabStop()回傳當前元件taborder值，判斷是否在FileExplorer上
		{
			strcpy(g_szProgDirectory, g_pFileExplPath);
			int nFocusIndex = ((CtmFileExplorer*)u_pwndFileExplorer)->GetFocusIndex();
			
			sprintf(u_sOperate_FileName, "%s/%s", g_szProgDirectory, \
			((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFileName(nFocusIndex));
				
			if(((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFileCount())
			{
				switch(wParam)
				{
					case	_F5:
						CopyFile();
						system("sleep 0.2");						//20140930 add by Mario
						Now_Push_Btn->CreateA();				//20140930 add by Mario
						Now_Push_Btn->UpdateAll();			//20140930 add by Mario
						Now_Push_Btn->Update();					//20140930 add by Mario
						//((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);		//Mario add 修正光標位置錯誤
						break;
					case	_F6:
						PasteFile();
						system("sleep 0.2");						//20140930 add by Mario
						Now_Push_Btn->CreateA();				//20140930 add by Mario
						Now_Push_Btn->UpdateAll();			//20140930 add by Mario
						Now_Push_Btn->Update();					//20140930 add by Mario
						break;
					case	_F7:
						if(g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
							DelFile();
						else
						{
							MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
							system("sleep 0.2");						//20140930 add by Mario
							Now_Push_Btn->CreateA();				//20140930 add by Mario
							Now_Push_Btn->UpdateAll();			//20140930 add by Mario
							Now_Push_Btn->Update();					//20140930 add by Mario
						}				
						system("sleep 0.2");						//20140930 add by Mario
						Now_Push_Btn->CreateA();				//20140930 add by Mario
						Now_Push_Btn->UpdateAll();			//20140930 add by Mario
						Now_Push_Btn->Update();					//20140930 add by Mario
						break;
					case	_F8:
						if(g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
							RenameFile();	
						else
						{
							MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
							system("sleep 0.2");						//20140930 add by Mario
							Now_Push_Btn->CreateA();				//20140930 add by Mario
							Now_Push_Btn->UpdateAll();			//20140930 add by Mario
							Now_Push_Btn->Update();					//20140930 add by Mario
						}
						system("sleep 0.2");						//20140930 add by Mario
						Now_Push_Btn->CreateA();				//20140930 add by Mario
						Now_Push_Btn->UpdateAll();			//20140930 add by Mario
						Now_Push_Btn->Update();					//20140930 add by Mario
						break;
					case	_F9:
						CreateFile();	
						system("sleep 0.2");						//20140930 add by Mario
						Now_Push_Btn->CreateA();				//20140930 add by Mario
						Now_Push_Btn->UpdateAll();			//20140930 add by Mario
						Now_Push_Btn->Update();					//20140930 add by Mario
						break;
					default:
						break;
				}
			}
			else
			{
				if(wParam == _F9)		//當目錄下沒任何檔案，唯一能做的就是創建個新的
				{
					CreateFile();
					system("sleep 0.2");						//20140930 add by Mario
					Now_Push_Btn->CreateA();				//20140930 add by Mario
					Now_Push_Btn->UpdateAll();			//20140930 add by Mario
					Now_Push_Btn->Update();					//20140930 add by Mario
				}
				else
					MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NO_FILE"], tmFT_CODE_TECH);
			}
		}
		else if(wParam == _F5 || wParam == _F6 || wParam == _F7 || wParam == _F8 || wParam == _F9)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NO_SELECT_FILE"], tmFT_CODE_TECH);
			system("sleep 0.2");			
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();		
		}
	}
	
}
/*============================================================================+
|				Helper																																|
+============================================================================*/																														
void		CopyFile()
{
 	strcpy(u_sCopy_FileName , u_sOperate_FileName);				//要複製的檔案
}


void		DelFile()
{
	
	if(u_pwndFileExplorer == NULL)	return;
	
	char	szMemPartFull[64] = "\0";
	sprintf(szMemPartFull, "%s/%s", g_szProgDirectory, g_TxCNCData.shmCNCData.CNC_mem_part);
	
	if(strcmp(u_sOperate_FileName, szMemPartFull) == 0)		//判斷要刪除的檔案是否已開啟(執行中)
	{
		//視窗：檔案刪除失敗!
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_FILE_DEL_FAIL"], tmFT_CODE_TECH);
		return;
	}
	else
	{
		char	szDeleteFileCommand[64] = "\0";
		//視窗：確定刪除檔案？
		MsgBox(g_MultiLanguage["ROBOT_MSG_FILE_DEL_SURE"], tmFT_CODE_TECH);
		if(g_bMsgBoxFlag)		//提示小視窗判斷 Yes or No
		{
			g_bMsgBoxFlag = FALSE;
			sprintf(szDeleteFileCommand, "rm %s", u_sOperate_FileName);
			system(szDeleteFileCommand);		//對Linux終端機(文字介面)下指令
			UpdateFileList();
		}
	}
	//清空提示訊息
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		RenameFile()
{
	char 		Now_Open_File[64]		= "\0";
	sprintf(Now_Open_File, "%s/%s", g_szProgDirectory, g_TxCNCData.shmCNCData.CNC_mem_part);
	if(!strcmp(Now_Open_File,u_sOperate_FileName))	//禁止開啟中的檔案進行更名  by Mario
	{
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_FILE_RENAME_FAIL"], tmFT_CODE_TECH);
		return;
	}
	else
	{
		g_nFileProcessType = FILECONTROL_TYPE_RENAME;
	
		char szRenameFileCommand[64] = "\0";
		memset(szRenameFileCommand, 0, sizeof(szRenameFileCommand));
	
		//FileProcess();
		MsgBoxSpecial(MSGBOX_FILECONTROL);
		GetNewFileName();
	
		if(u_sNewFileName_FromMB[0] == '\0')		//使用者輸入空白檔名 或 取消動作
		{
			printf("User doesn't Enter anything!\n");
			return;
		}
		else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
		{
			//檔案更名
			sprintf(szRenameFileCommand, "mv %s %s/%s", u_sOperate_FileName, \
			g_szProgDirectory, u_sNewFileName_FromMB);
			system(szRenameFileCommand);
			UpdateFileList(u_sNewFileName_FromMB);
		}
		else
		{
		//視窗：已有重複的檔名！
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_THESAME_FILENAME"], tmFT_CODE_TECH);
		g_sOperate_FileName=0;
		}
	//清空提示訊息
 		Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);  
	}
}

void		CreateFile()
{
	g_nFileProcessType = FILECONTROL_TYPE_CREATE;
	
	char szCreateFileCommand[64] = "\0";
	memset(szCreateFileCommand, 0, sizeof(szCreateFileCommand));
	
	//FileProcess();
	MsgBoxSpecial(MSGBOX_FILECONTROL);
	GetNewFileName();
	
	if(u_sNewFileName_FromMB[0] == '\0')		//使用者輸入空白檔名 或 取消動作
	{
		printf("User doesn't Enter anything!\n");
		return;
	}
	else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
	{
		//因為無法直接建立在指定的目錄下，故採先建後移
		//檔案建立
		sprintf(szCreateFileCommand, "touch %s", u_sNewFileName_FromMB);
		system(szCreateFileCommand);
		
		//指令重置
		memset(szCreateFileCommand, 0, sizeof(szCreateFileCommand));
		
		//移至目前所在的目錄內
		sprintf(szCreateFileCommand, "mv %s %s", u_sNewFileName_FromMB, g_szProgDirectory);
		system(szCreateFileCommand);
		
		UpdateFileList(u_sNewFileName_FromMB);
	}
	else
	{
		//視窗：已有重複的檔名！
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_THESAME_FILENAME"], tmFT_CODE_TECH);
		g_sOperate_FileName=0;
	}
	//清空提示訊息
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		UpdateFileList(char* FileStr)
{
	char	pszCurrentPath[256] = "\0";
	
	memset(pszCurrentPath, 0, sizeof(pszCurrentPath));
	strcpy(pszCurrentPath, ((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentPath());
	if(u_pwndFileExplorer != NULL)
	{
		int FileIndex = -1;
		((CtmFileExplorer*)u_pwndFileExplorer)->Opendir_File(pszCurrentPath);
		//((CtmFileExplorer*)u_pwndFileExplorer)->ShowFileList();
		FileIndex = ((CtmFileExplorer*)u_pwndFileExplorer)->GetFileIndex(FileStr);
		if(FileIndex >= 0 )
			((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(FileIndex);
		else
			((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);
			
		((CtmFileExplorer*)u_pwndFileExplorer)->Update();
	}
}


void		UpdateFileList()		//刷新右方的檔案列表
{
	char	pszCurrentPath[256] = "\0";
	
	memset(pszCurrentPath, 0, sizeof(pszCurrentPath));
	strcpy(pszCurrentPath, ((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentPath());
	
	if(u_pwndFileExplorer != NULL)
	{
		((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);
		((CtmFileExplorer*)u_pwndFileExplorer)->Opendir_File(pszCurrentPath);
		((CtmFileExplorer*)u_pwndFileExplorer)->Update();
	}
}

void		ReLoadDirectoryAndFile()		//重新載入目錄，以及刷新檔案列表
{
	char*	pszName = NULL;
	char	pszCurrentPath[256] = "\0";
	char	pszCurrentName[256] = "\0";
	
	//取得光標所在位置的項目名稱
	pszName = ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirName();
	if(pszName != NULL)
	{
		memset(pszCurrentName, 0, sizeof(pszCurrentName));
		strcpy(pszCurrentName, pszName);
	}
	if(!DirectoryLock())
	{
		//設定左方顯示
		if(((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File(pszCurrentName, 1) == 0)
		{	
			((CtmFileExplorer*)u_pwndFileExplorerD)->SetFocusIndex(0);
			((CtmFileExplorer*)u_pwndFileExplorerD)->UpdateAll();
		}
		
		//取得當前目錄
		memset(pszCurrentPath, 0, sizeof(pszCurrentPath));
		strcpy(pszCurrentPath, ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentPath());
		
		//設定右方顯示
		if(u_pwndFileExplorer != NULL)
		{
			((CtmFileExplorer*)u_pwndFileExplorer)->Opendir_File(pszCurrentPath);
			((CtmFileExplorer*)u_pwndFileExplorer)->Update();
		}
		
		//將當前目錄複製給g_pFileExplPath(全域變數，開啟檔案需要用到)
		strcpy(g_pFileExplPath, pszCurrentPath);
	}
}

BOOL		DirectoryLock()		//目錄鎖定
{
	char*	pszCurrentName = NULL;
	char 	pszCurrentPath[256] = "\0";
	
	pszCurrentName = ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirName();
	
	memset(pszCurrentPath, 0, sizeof(pszCurrentPath));
	strcpy(pszCurrentPath, ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentPath());
	//防止使用者接觸./Prog外的目錄
	if((strcmp(pszCurrentName, "..") == 0)&&(strcmp(pszCurrentPath, "./Prog") == 0) || (strcmp(pszCurrentName, "..") == 0)&&(strcmp(pszCurrentPath, "./usb") == 0))
	{
		return TRUE;			//Lock
	}
	else
		return FALSE;			//Unlock
}

void		GetNewFileName()		//取得使用者在提示小視窗裡輸入的檔名
{
	memset(u_sNewFileName_FromMB, 0, sizeof(u_sNewFileName_FromMB));
	//strcpy(u_sNewFileName_FromMB, g_sOperate_FileName);
}

BOOL		FocusLock(CtmWnd* pwndSender)		//判斷往下移動光標是否會在不恰當的位置(空白處)
{		
	//TRUE	-> 限制
	//FALSE	-> 未達空白處，不限制
	int nMaxIndex =0;
	
	if(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORERD)	//左邊列表
	{
		nMaxIndex = ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirCount()-1;
		//判斷光標位置是否超出目錄列表的項目總數
		if(((CtmFileExplorer*)u_pwndFileExplorerD)->GetFocusIndex() >= nMaxIndex)
		{
			((CtmFileExplorer*)u_pwndFileExplorerD)->SetFocusIndex(nMaxIndex);
			return TRUE;
		}
	}
	else if(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORER)	//右邊列表
	{
		nMaxIndex = ((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFileCount()-1;
		//判斷光標位置是否超出檔案列表的項目總數
		if(((CtmFileExplorer*)u_pwndFileExplorer)->GetFocusIndex() >= nMaxIndex)
		{
			((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(nMaxIndex);
			return TRUE;
		}
	}
	return FALSE;
}


//========================================================================
void		PasteFile()		//貼上
{
	g_nFileProcessType = FILECONTROL_TYPE_COPY;
	char szCopyFileCommand[64] = "\0"; 
	memset(szCopyFileCommand, 0,sizeof(szCopyFileCommand));
	if (u_sCopy_FileName[0] != '\0')
	{
		MsgBoxSpecial(MSGBOX_FILECONTROL);
		GetNewFileName();
		if(u_sNewFileName_FromMB[0] == '\0')		//使用者輸入空白檔名 或 取消動作
		{
			return;
		}
		else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
		{
			//檔案貼上		
			sprintf(szCopyFileCommand, "cp %s %s/%s", u_sCopy_FileName,g_szProgDirectory, u_sNewFileName_FromMB);
			system(szCopyFileCommand);
			UpdateFileList(u_sNewFileName_FromMB);
		}
		else
		{
			//視窗：已有重複的檔名！
			MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_THESAME_FILENAME"], tmFT_CODE_TECH);
			//memset(g_sOperate_FileName, 0, sizeof(g_sOperate_FileName));
		}
	}
	else
	{
		//printf("u_sCopy_FileName i %s\n",u_sCopy_FileName);
		//printf("沒有複製檔案\n");
	}
}

