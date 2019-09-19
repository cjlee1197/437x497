/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : 	                                                     		|
|  Version  :                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

#include	"ViewMsgBoxFileSelect.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnYes					= NULL;
CtmWnd*		u_pwndBtnNo						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndFileNameStatic	= NULL;


BOOL		WorkOne									=TRUE;		//進入畫面時自動執行，僅一次

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnYes			= pwndSender->FindControlFromName("btnYes");
	u_pwndBtnNo 			= pwndSender->FindControlFromName("btnNo");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndFileNameStatic		= pwndSender->FindControlFromName("FileName");
	
	g_bMsgBoxFlag = FALSE;
	
	return TRUE;
}


WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	

		if(pwndTemp == u_pwndBtnYes)
		{
			g_bMsgBoxFlag = TRUE;
			char Command[64] ="\0";
			char EditFile[64] ="\0";
			char u_szMem_Part_File[64] ="\0";
			GetDBString("CNC_EDIT_PATH",EditFile, sizeof(EditFile));
			sprintf(u_szMem_Part_File, "%s/%s", g_szProgDirectory, g_TxCNCData.shmCNCData.CNC_mem_part);
			if(strcmp(EditFile,u_szMem_Part_File) == 0)
			{
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_FILE_RUN_NO_SAVE");
				return wIDControl;
			}
			sprintf(Command, "cp temp/temp %s",  EditFile);
			//printf("Command is %s\n",Command);
			system(Command);
			system("rm -r temp/temp");
			Exit();
		}
		else if(pwndTemp == u_pwndBtnNo)
		{
			g_bMsgBoxFlag = TRUE;
			system("rm -r temp/temp");
			Exit();
		}
		else if(pwndTemp == u_pwndBtnCancel)
		{
			g_bMsgBoxFlag = FALSE;
			Exit();
		}
	return wIDControl;	
}

//20140624↑

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;	
}


void		OnUpdateA(CtmWnd*	pwndSender)
{
	if(WorkOne)
	{
		char	szTemp[64]		= "\0";
		char	szTemp1[64]		= "\0";
		char 	EditFile[64] ="\0";
		GetDBString("CNC_EDIT_PATH",EditFile, sizeof(EditFile));
		sprintf(szTemp,"%s", EditFile);
		CodeChange(szTemp1,szTemp );
		u_pwndFileNameStatic->SetPropValueT("text", szTemp1);
		u_pwndFileNameStatic->Update();
		WorkOne =FALSE;
		((CtmFormView*)pwndSender)->OnLoseFocus();
	}
}

void    CodeChange(char *pDst, char *pSrc)		//輔助用函式，單字節轉化為雙字節處理
{
	//供StaticX2元件的text屬性使用(目前)
	WORD	*pwDst = (WORD *)pDst;
    
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}