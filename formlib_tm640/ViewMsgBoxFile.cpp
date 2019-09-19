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

//�P�_�O�_���UENTER��
CtmWnd*		u_pwndFileExplorer			= NULL;
CtmWnd*		u_pwndBtnFileCopy				= NULL;
CtmWnd*		u_pwndBtnFileDelete			= NULL;
CtmWnd*		u_pwndBtnFileRename			= NULL;
CtmWnd*		u_pwndBtnFileCreate			= NULL;
CtmWnd*		u_pwndBtnFileOpen				= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;

char 			ButtonText[32]	=	"\0";
char			u_sNewFileName_FromMB[64]			= "\0";
char			u_sOperate_FileName[64]				= "\0";		//�n�i��B�z���ɮ׸��|
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
	if(g_sOperate_FileName == 0)		//�ϥΪ̿�J�ť��ɦW �� �����ʧ@
	{
		//printf("User doesn't Enter anything!\n");
		return;
	}
	else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
	{
		//�ɮ׽ƻs		
		memset(u_sOperate_FileName, 0, sizeof(u_sOperate_FileName));
		((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFilePath(u_sOperate_FileName);
		sprintf(szCopyFileCommand, "cp %s %s/%s", u_sOperate_FileName,PROGFILEPATH, u_sNewFileName_FromMB);
		system(szCopyFileCommand);
		UpdateFileList();
	}
	else
	{
		//�����G�w�����ƪ��ɦW�I
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
		g_sOperate_FileName=0;
	}
	//�M�Ŵ��ܰT��
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);  
 	
 	//20140912���@�אּ�ƻs�K�W���}
 	
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
	
	if(strcmp(u_sOperate_FileName, szMemPartFull) == 0)		//�P�_�n�R�����ɮ׬O�_�w�}��(���椤)
	{
		//�����G�ɮקR������!
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_FILE_DEL_FAIL");
		return;
	}
	else if(strcmp(u_sOperate_FileName, szEditFile) == 0)		//�P�_�n�R�����ɮ׬O�_�w�}��(�s�褤)
	{
		//�����G�ɮקR������!
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_FILE_DEL_FAIL");
		return;
	}
	else
	{
		char	szDeleteFileCommand[64] = "\0";
		//�����G�T�w�R���ɮסH
		MsgBoxCall("msgbox1.txt","ROBOT_MSG_FILE_DEL_SURE");
		if(g_bMsgBoxFlag)		//���ܤp�����P�_ Yes or No
		{
			g_bMsgBoxFlag = FALSE;
			sprintf(szDeleteFileCommand, "rm %s", u_sOperate_FileName);
			system(szDeleteFileCommand);	
			UpdateFileList();	
		}
	}
	//�M�Ŵ��ܰT��
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		CreateFile()
{
	g_nFileProcessType = FILECONTROL_TYPE_CREATEFILE;
	char szCreateFileCommand[64] = "\0";
	memset(szCreateFileCommand, 0, sizeof(szCreateFileCommand));
	
	MsgBoxSpecial(MSGBOX_FILECONTROL);
	GetNewFileName();
	
	if(g_sOperate_FileName==0)		//�ϥΪ̿�J�ť��ɦW �� �����ʧ@
	{
		return;
	}
	else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
	{
		//�]���L�k�����إߦb���w���ؿ��U�A�G�ĥ��ثᲾ
		//�ɮ׫إ�
		sprintf(szCreateFileCommand, "touch %s", u_sNewFileName_FromMB);
		system(szCreateFileCommand);
		
		//���O���m
		memset(szCreateFileCommand, 0, sizeof(szCreateFileCommand));
		
		//���ܥثe�Ҧb���ؿ���
		sprintf(szCreateFileCommand, "mv %s %s", u_sNewFileName_FromMB, PROGFILEPATH);
		system(szCreateFileCommand);
		
		UpdateFileList(u_sNewFileName_FromMB);
		//printf("System Command(Move) :  %s\n", szCreateFileCommand);
	}
	else
	{
		//�����G�w�����ƪ��ɦW�I
		MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
		g_sOperate_FileName=0;
	}
	//�M�Ŵ��ܰT��
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		RenameFile()
{
	g_nFileProcessType = FILECONTROL_TYPE_RENAMEFILE;
	char	szMemPartFull[64] = "\0";
	sprintf(szMemPartFull, "%s/%s", PROGFILEPATH, g_TxCNCData.shmCNCData.CNC_mem_part);
	
	
	memset(u_sOperate_FileName, 0, sizeof(u_sOperate_FileName));
	((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFilePath(u_sOperate_FileName);
	if(strcmp(szMemPartFull,u_sOperate_FileName) == 0)	//�T��}�Ҥ����ɮ׶i���W  by Mario
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
	
		if(g_sOperate_FileName==0)		//�ϥΪ̿�J�ť��ɦW �� �����ʧ@
		{
			//printf("User doesn't Enter anything!\n");
			return;
		}
		else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
		{
			//�ɮק�W
			sprintf(szRenameFileCommand, "mv %s %s/%s", u_sOperate_FileName,PROGFILEPATH,u_sNewFileName_FromMB);
			system(szRenameFileCommand);
			UpdateFileList(u_sNewFileName_FromMB);
		}
		else
		{
			//�����G�w�����ƪ��ɦW�I
			
			MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_THESAME_FILENAME");
			g_sOperate_FileName=0;
		}
	}
	
}

void		GetNewFileName()		//���o�ϥΪ̦b���ܤp�����̿�J���ɦW
{
	memset(u_sNewFileName_FromMB, 0, sizeof(u_sNewFileName_FromMB));
	sprintf(u_sNewFileName_FromMB,"O%04d",g_sOperate_FileName);
	//strcpy(u_sNewFileName_FromMB, g_sOperate_FileName);
	//printf("NewFileName edit by User: %s\n", u_sNewFileName_FromMB);
}

void		UpdateFileList()		//��s�k�誺�ɮצC��
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