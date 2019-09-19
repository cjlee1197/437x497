/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario write

#include	"ViewMsgBoxFile.h"
#include	"../utils.h"
#include 	"../tmFileExplorer.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

#define		MSGBOX_FILECONTROL				0
#define		PROGFILEPATH "./Prog"
#define		FILECONTROL_TYPE_COPYFILE			0
#define		FILECONTROL_TYPE_RENAMEFILE		1
#define		FILECONTROL_TYPE_CREATEFILE		2
/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
CtmWnd*		u_pwndFileExplorer			= NULL;
CtmWnd*		u_pwndBtnFileCopy				= NULL;
CtmWnd*		u_pwndBtnFileDelete			= NULL;
CtmWnd*		u_pwndBtnFileRename			= NULL;
CtmWnd*		u_pwndBtnFileCreate			= NULL;
CtmWnd*		u_pwndBtnFileOpen				= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;

char 			ButtonText[32]	=	"\0";
char			u_sNewFileName_FromMB[64]			= "\0";
char			u_sOperate_FileName[64]				= "\0";		//要進行處理的檔案路徑
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndFileExplorer 			= pwndSender->FindControlFromName("FileExplorer");
	u_pwndBtnFileCopy 			= pwndSender->FindControlFromName("BtnFileCopy");
	u_pwndBtnFileDelete 		= pwndSender->FindControlFromName("BtnFileDelete");
	u_pwndBtnFileRename 		= pwndSender->FindControlFromName("BtnFileRename");
	u_pwndBtnFileCreate 		= pwndSender->FindControlFromName("BtnFileCreate");
	u_pwndBtnFileOpen 			= pwndSender->FindControlFromName("FileOpen");
	u_pwndBtnCancel 	  		= pwndSender->FindControlFromName("BtnCancel");
	
	
	memset(g_Now_FileName,0,sizeof(g_Now_FileName));
		
	return TRUE;
}
/*
WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{	
		if(pwndTemp == u_pwndBtnCancel)
		{
			remove("./DialogHelp/buffer");
			Exit();
		}
		else if(pwndTemp == u_pwndBtnOK)
		{
			char temp[64]="\0";
			int temp2 = 0;
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			sprintf(temp,"GOTO %d;",temp2);
			AddCommand(temp);
			Exit();
		}
	}
	
	return wIDControl;
}
*/

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	

		if(pwndTemp == u_pwndBtnCancel)
		{
			memset(g_Now_FileName,0,sizeof(g_Now_FileName));
			Exit();
		}
		else if(pwndTemp == u_pwndBtnFileCopy)
		{
			CopyFile();
		}
		else if(pwndTemp == u_pwndBtnFileDelete)
		{
			if(g_TxCNCData.shmCNCData.CNC_auto_status==RUN_STATUS_STOP)
			{
				DelFile();
			}
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		}
		else if(pwndTemp == u_pwndBtnFileRename)
		{
			if(g_TxCNCData.shmCNCData.CNC_auto_status==RUN_STATUS_STOP)
			{
				RenameFile();
			}
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		}
		else if(pwndTemp == u_pwndBtnFileCreate)
		{
			CreateFile();
		}
		else if(pwndTemp == u_pwndBtnFileOpen)
		{
			memset(g_Now_FileName,0,sizeof(g_Now_FileName));
			((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFilePath(g_Now_FileName);
			//printf("g_Now_FileName is %s\n",g_Now_FileName);
			Exit();
		}
	
	return wIDControl;
}
/*
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//if(wKey == _ENTER)
	//	u_bEnterKey = TRUE;
	//else
	//	u_bEnterKey = FALSE;
	
	//((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;	
}
*/

void		OnUpdateA(CtmWnd* pwndSender)
{
	//printf("msgboxfile update\n");
	static BOOL FristOne = TRUE;
	if(FristOne)
	{
		FristOne = FALSE;
		UpdateFileList();
	}
	
}
/*========================================================+
|						Helper																				|
+========================================================*/

void		CopyFile()
{
	g_nFileProcessType = FILECONTROL_TYPE_COPYFILE;
	char szCopyFileCommand[64] = "\0"; 
	memset(szCopyFileCommand, 0,sizeof(szCopyFileCommand)); //FileProcess();
	MsgBoxSpecial(MSGBOX_FILECONTROL);
	GetNewFileName();
	if(g_sOperate_FileName == 0)		//使用者輸入空白檔名 或 取消動作
	{
		//printf("User doesn't Enter anything!\n");
		return;
	}
	else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
	{
		//檔案複製		
		memset(u_sOperate_FileName, 0, sizeof(u_sOperate_FileName));
		((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFilePath(u_sOperate_FileName);
		sprintf(szCopyFileCommand, "cp %s %s/%s", u_sOperate_FileName,PROGFILEPATH, u_sNewFileName_FromMB);
		system(szCopyFileCommand);
		UpdateFileList();
	}
	else
	{
		//視窗：已有重複的檔名！
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
		g_sOperate_FileName=0;
	}
	//清空提示訊息
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);  
 	
 	//20140912↑　改為複製貼上分開
 	
}

void		DelFile()
{
	if(u_pwndFileExplorer == NULL)	return;
	char	szMemPartFull[64] = "\0";
	char	szEditFile[64] = "\0";
	sprintf(szMemPartFull, "%s/%s", PROGFILEPATH, g_TxCNCData.shmCNCData.CNC_mem_part);
	
	memset(u_sOperate_FileName, 0, sizeof(u_sOperate_FileName));
	((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFilePath(u_sOperate_FileName);
	
	memset(szEditFile, 0, sizeof(szEditFile));
	GetDBString("CNC_EDIT_PATH",szEditFile, sizeof(szEditFile)); 
	
	if(strcmp(u_sOperate_FileName, szMemPartFull) == 0)		//判斷要刪除的檔案是否已開啟(執行中)
	{
		//視窗：檔案刪除失敗!
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_FILE_DEL_FAIL");
		return;
	}
	else if(strcmp(u_sOperate_FileName, szEditFile) == 0)		//判斷要刪除的檔案是否已開啟(編輯中)
	{
		//視窗：檔案刪除失敗!
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_FILE_DEL_FAIL");
		return;
	}
	else
	{
		char	szDeleteFileCommand[64] = "\0";
		//視窗：確定刪除檔案？
		MsgBoxCall("msgbox1.txt","ROBOT_MSG_FILE_DEL_SURE");
		if(g_bMsgBoxFlag)		//提示小視窗判斷 Yes or No
		{
			g_bMsgBoxFlag = FALSE;
			sprintf(szDeleteFileCommand, "rm %s", u_sOperate_FileName);
			system(szDeleteFileCommand);	
			UpdateFileList();	
		}
	}
	//清空提示訊息
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		CreateFile()
{
	g_nFileProcessType = FILECONTROL_TYPE_CREATEFILE;
	char szCreateFileCommand[64] = "\0";
	memset(szCreateFileCommand, 0, sizeof(szCreateFileCommand));
	
	MsgBoxSpecial(MSGBOX_FILECONTROL);
	GetNewFileName();
	
	if(g_sOperate_FileName==0)		//使用者輸入空白檔名 或 取消動作
	{
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
		sprintf(szCreateFileCommand, "mv %s %s", u_sNewFileName_FromMB, PROGFILEPATH);
		system(szCreateFileCommand);
		
		UpdateFileList(u_sNewFileName_FromMB);
		//printf("System Command(Move) :  %s\n", szCreateFileCommand);
	}
	else
	{
		//視窗：已有重複的檔名！
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
		g_sOperate_FileName=0;
	}
	//清空提示訊息
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		RenameFile()
{
	g_nFileProcessType = FILECONTROL_TYPE_RENAMEFILE;
	char	szMemPartFull[64] = "\0";
	sprintf(szMemPartFull, "%s/%s", PROGFILEPATH, g_TxCNCData.shmCNCData.CNC_mem_part);
	
	
	memset(u_sOperate_FileName, 0, sizeof(u_sOperate_FileName));
	((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFilePath(u_sOperate_FileName);
	if(strcmp(szMemPartFull,u_sOperate_FileName) == 0)	//禁止開啟中的檔案進行更名  by Mario
	{
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_FILE_RENAME_FAIL");
		return;
	}
	else
	{
		char szRenameFileCommand[64] = "\0";
		memset(szRenameFileCommand, 0, sizeof(szRenameFileCommand));
	
		MsgBoxSpecial(MSGBOX_FILECONTROL);
		GetNewFileName();
	
		if(g_sOperate_FileName==0)		//使用者輸入空白檔名 或 取消動作
		{
			//printf("User doesn't Enter anything!\n");
			return;
		}
		else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
		{
			//檔案更名
			sprintf(szRenameFileCommand, "mv %s %s/%s", u_sOperate_FileName,PROGFILEPATH,u_sNewFileName_FromMB);
			system(szRenameFileCommand);
			UpdateFileList(u_sNewFileName_FromMB);
		}
		else
		{
			//視窗：已有重複的檔名！
			
			MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
			g_sOperate_FileName=0;
		}
	}
	
}

void		GetNewFileName()		//取得使用者在提示小視窗裡輸入的檔名
{
	memset(u_sNewFileName_FromMB, 0, sizeof(u_sNewFileName_FromMB));
	sprintf(u_sNewFileName_FromMB,"O%04d",g_sOperate_FileName);
	//strcpy(u_sNewFileName_FromMB, g_sOperate_FileName);
	//printf("NewFileName edit by User: %s\n", u_sNewFileName_FromMB);
}

void		UpdateFileList()		//刷新右方的檔案列表
{
	if(u_pwndFileExplorer != NULL)
	{
		((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);
		((CtmFileExplorer*)u_pwndFileExplorer)->Opendir_File(PROGFILEPATH);
		((CtmFileExplorer*)u_pwndFileExplorer)->Update();
	}
}

void		UpdateFileList(char* FileStr)
{
	if(u_pwndFileExplorer != NULL)
	{
		int FileIndex = -1;
		((CtmFileExplorer*)u_pwndFileExplorer)->Opendir_File(PROGFILEPATH);
		FileIndex = ((CtmFileExplorer*)u_pwndFileExplorer)->GetFileIndex(FileStr);
		//printf("FileIndex %d\n",FileIndex);
		if(FileIndex >= 0 )
			((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(FileIndex);
		else
			((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);
			
		((CtmFileExplorer*)u_pwndFileExplorer)->Update();
	}
}