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

#define		INDEX_FILE_EXPLORERD		1								//����FileExplorer����taborder��
#define		INDEX_FILE_EXPLORER			2								//�k��FileExplorer����taborder��

//�P�_�n���椰��\��B�z
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

char			u_sOperate_FileName[64]				= "\0";		//�n�i��B�z���ɮ׸��|
char			u_sNewFileName_FromMB[64]			= "\0";		//�ϥΪ̦b���ܤp�����̿�J���ɮצW��

char			u_sCopy_FileName[64]					=	"\0";		//�x�s�Q�ƻs�ɮת����| 20140914 by mario

int mode=0;
/*============================================================================+
|              Function	                                                      |
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)		//�N��qtoolbar1������toolbar11�]���|���s�Ы�
{
	//����U������w
	u_pwndFileExplorerD				= pwndSender->FindControlFromName("FileExplorerD");		//����ؿ��C��
	u_pwndFileExplorer				= pwndSender->FindControlFromName("FileExplorer");		//�k���ɮצC��
	
	u_pwndSelectEditFileDir		= pwndSender->FindControlFromName("SelectEditFileDir");		//�U�Ԧ����
	
	//�}�ҵ��ϥΪ̾ާ@���ؿ�
	((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File("./Prog");
	
	g_Now_FileName[0] = '\0';			//���m
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
			//�P�_����C�����ЩҦb��m�O�_������
			if(((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirName() != NULL)
			{
				ReLoadDirectoryAndFile();
				((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);		//�b�����ؿ����������k��
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
//20140923��		����i�J�s�譶�������]�w
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
		if(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORER)	//Get_TabStop()�^�Ƿ�e����taborder�ȡA�P�_�O�_�bFileExplorer�W
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
						//((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(0);		//Mario add �ץ����Ц�m���~
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
				if(wParam == _F9)		//��ؿ��U�S�����ɮסA�ߤ@�వ���N�O�Ыحӷs��
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
 	strcpy(u_sCopy_FileName , u_sOperate_FileName);				//�n�ƻs���ɮ�
}


void		DelFile()
{
	
	if(u_pwndFileExplorer == NULL)	return;
	
	char	szMemPartFull[64] = "\0";
	sprintf(szMemPartFull, "%s/%s", g_szProgDirectory, g_TxCNCData.shmCNCData.CNC_mem_part);
	
	if(strcmp(u_sOperate_FileName, szMemPartFull) == 0)		//�P�_�n�R�����ɮ׬O�_�w�}��(���椤)
	{
		//�����G�ɮקR������!
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_FILE_DEL_FAIL"], tmFT_CODE_TECH);
		return;
	}
	else
	{
		char	szDeleteFileCommand[64] = "\0";
		//�����G�T�w�R���ɮסH
		MsgBox(g_MultiLanguage["ROBOT_MSG_FILE_DEL_SURE"], tmFT_CODE_TECH);
		if(g_bMsgBoxFlag)		//���ܤp�����P�_ Yes or No
		{
			g_bMsgBoxFlag = FALSE;
			sprintf(szDeleteFileCommand, "rm %s", u_sOperate_FileName);
			system(szDeleteFileCommand);		//��Linux�׺ݾ�(��r����)�U���O
			UpdateFileList();
		}
	}
	//�M�Ŵ��ܰT��
 	Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMMY")], 0);
}

void		RenameFile()
{
	char 		Now_Open_File[64]		= "\0";
	sprintf(Now_Open_File, "%s/%s", g_szProgDirectory, g_TxCNCData.shmCNCData.CNC_mem_part);
	if(!strcmp(Now_Open_File,u_sOperate_FileName))	//�T��}�Ҥ����ɮ׶i���W  by Mario
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
	
		if(u_sNewFileName_FromMB[0] == '\0')		//�ϥΪ̿�J�ť��ɦW �� �����ʧ@
		{
			printf("User doesn't Enter anything!\n");
			return;
		}
		else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
		{
			//�ɮק�W
			sprintf(szRenameFileCommand, "mv %s %s/%s", u_sOperate_FileName, \
			g_szProgDirectory, u_sNewFileName_FromMB);
			system(szRenameFileCommand);
			UpdateFileList(u_sNewFileName_FromMB);
		}
		else
		{
		//�����G�w�����ƪ��ɦW�I
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_THESAME_FILENAME"], tmFT_CODE_TECH);
		g_sOperate_FileName=0;
		}
	//�M�Ŵ��ܰT��
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
	
	if(u_sNewFileName_FromMB[0] == '\0')		//�ϥΪ̿�J�ť��ɦW �� �����ʧ@
	{
		printf("User doesn't Enter anything!\n");
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
		sprintf(szCreateFileCommand, "mv %s %s", u_sNewFileName_FromMB, g_szProgDirectory);
		system(szCreateFileCommand);
		
		UpdateFileList(u_sNewFileName_FromMB);
	}
	else
	{
		//�����G�w�����ƪ��ɦW�I
		MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_THESAME_FILENAME"], tmFT_CODE_TECH);
		g_sOperate_FileName=0;
	}
	//�M�Ŵ��ܰT��
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


void		UpdateFileList()		//��s�k�誺�ɮצC��
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

void		ReLoadDirectoryAndFile()		//���s���J�ؿ��A�H�Ψ�s�ɮצC��
{
	char*	pszName = NULL;
	char	pszCurrentPath[256] = "\0";
	char	pszCurrentName[256] = "\0";
	
	//���o���ЩҦb��m�����ئW��
	pszName = ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirName();
	if(pszName != NULL)
	{
		memset(pszCurrentName, 0, sizeof(pszCurrentName));
		strcpy(pszCurrentName, pszName);
	}
	if(!DirectoryLock())
	{
		//�]�w�������
		if(((CtmFileExplorer*)u_pwndFileExplorerD)->Opendir_File(pszCurrentName, 1) == 0)
		{	
			((CtmFileExplorer*)u_pwndFileExplorerD)->SetFocusIndex(0);
			((CtmFileExplorer*)u_pwndFileExplorerD)->UpdateAll();
		}
		
		//���o��e�ؿ�
		memset(pszCurrentPath, 0, sizeof(pszCurrentPath));
		strcpy(pszCurrentPath, ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentPath());
		
		//�]�w�k�����
		if(u_pwndFileExplorer != NULL)
		{
			((CtmFileExplorer*)u_pwndFileExplorer)->Opendir_File(pszCurrentPath);
			((CtmFileExplorer*)u_pwndFileExplorer)->Update();
		}
		
		//�N��e�ؿ��ƻs��g_pFileExplPath(�����ܼơA�}���ɮ׻ݭn�Ψ�)
		strcpy(g_pFileExplPath, pszCurrentPath);
	}
}

BOOL		DirectoryLock()		//�ؿ���w
{
	char*	pszCurrentName = NULL;
	char 	pszCurrentPath[256] = "\0";
	
	pszCurrentName = ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirName();
	
	memset(pszCurrentPath, 0, sizeof(pszCurrentPath));
	strcpy(pszCurrentPath, ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentPath());
	//����ϥΪ̱�Ĳ./Prog�~���ؿ�
	if((strcmp(pszCurrentName, "..") == 0)&&(strcmp(pszCurrentPath, "./Prog") == 0) || (strcmp(pszCurrentName, "..") == 0)&&(strcmp(pszCurrentPath, "./usb") == 0))
	{
		return TRUE;			//Lock
	}
	else
		return FALSE;			//Unlock
}

void		GetNewFileName()		//���o�ϥΪ̦b���ܤp�����̿�J���ɦW
{
	memset(u_sNewFileName_FromMB, 0, sizeof(u_sNewFileName_FromMB));
	//strcpy(u_sNewFileName_FromMB, g_sOperate_FileName);
}

BOOL		FocusLock(CtmWnd* pwndSender)		//�P�_���U���ʥ��ЬO�_�|�b�������m(�ťճB)
{		
	//TRUE	-> ����
	//FALSE	-> ���F�ťճB�A������
	int nMaxIndex =0;
	
	if(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORERD)	//����C��
	{
		nMaxIndex = ((CtmFileExplorer*)u_pwndFileExplorerD)->GetCurrentDirCount()-1;
		//�P�_���Ц�m�O�_�W�X�ؿ��C�������`��
		if(((CtmFileExplorer*)u_pwndFileExplorerD)->GetFocusIndex() >= nMaxIndex)
		{
			((CtmFileExplorer*)u_pwndFileExplorerD)->SetFocusIndex(nMaxIndex);
			return TRUE;
		}
	}
	else if(((CtmFormView*)pwndSender)->Get_TabStop() == INDEX_FILE_EXPLORER)	//�k��C��
	{
		nMaxIndex = ((CtmFileExplorer*)u_pwndFileExplorer)->GetCurrentFileCount()-1;
		//�P�_���Ц�m�O�_�W�X�ɮצC�������`��
		if(((CtmFileExplorer*)u_pwndFileExplorer)->GetFocusIndex() >= nMaxIndex)
		{
			((CtmFileExplorer*)u_pwndFileExplorer)->SetFocusIndex(nMaxIndex);
			return TRUE;
		}
	}
	return FALSE;
}


//========================================================================
void		PasteFile()		//�K�W
{
	g_nFileProcessType = FILECONTROL_TYPE_COPY;
	char szCopyFileCommand[64] = "\0"; 
	memset(szCopyFileCommand, 0,sizeof(szCopyFileCommand));
	if (u_sCopy_FileName[0] != '\0')
	{
		MsgBoxSpecial(MSGBOX_FILECONTROL);
		GetNewFileName();
		if(u_sNewFileName_FromMB[0] == '\0')		//�ϥΪ̿�J�ť��ɦW �� �����ʧ@
		{
			return;
		}
		else if(((CtmFileExplorer*)u_pwndFileExplorer)->FileCheck(u_sNewFileName_FromMB))
		{
			//�ɮ׶K�W		
			sprintf(szCopyFileCommand, "cp %s %s/%s", u_sCopy_FileName,g_szProgDirectory, u_sNewFileName_FromMB);
			system(szCopyFileCommand);
			UpdateFileList(u_sNewFileName_FromMB);
		}
		else
		{
			//�����G�w�����ƪ��ɦW�I
			MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_THESAME_FILENAME"], tmFT_CODE_TECH);
			//memset(g_sOperate_FileName, 0, sizeof(g_sOperate_FileName));
		}
	}
	else
	{
		//printf("u_sCopy_FileName i %s\n",u_sCopy_FileName);
		//printf("�S���ƻs�ɮ�\n");
	}
}

