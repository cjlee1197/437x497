/*============================================================================+
|  Class    : ViewMessageBoxFileControl library                               |
|  Task     : ViewMessageBoxFileControl library action source file						|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxFileControl.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../_kb.h"
#include 	"../main.h"
#include 	"../wnd.h"
#include	"../tmdebug.h"
#include	"../lib/libiNetComm.h"

/*========================================================+
|						Constants																			|
+========================================================*/

//判斷要執行什麼功能處理
#define		FILECONTROL_TYPE_COPYFILE			0
#define		FILECONTROL_TYPE_RENAMEFILE		1
#define		FILECONTROL_TYPE_CREATEFILE		2

//各元件taborder值
#define		INDEX_EDIT_FILENAME						0
#define		INDEX_BTN_OK									1
#define		INDEX_BTN_CANCEL							2

/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

char			u_sNewFileName[64]		= "\0";

CtmWnd*		u_pwndStaticMessage		= NULL;
CtmWnd*		u_pwndEditFileName		= NULL;
CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;

BOOL			WorkOne								=TRUE;		//進入畫面時自動執行，僅一次
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//取得各元件指針
	u_pwndStaticMessage 	= pwndSender->FindControlFromName("StaticMessage");
	u_pwndEditFileName		= pwndSender->FindControlFromName("EditFileName");
	u_pwndBtnOK						= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel				= pwndSender->FindControlFromName("btnCancel");
	
	switch(g_nFileProcessType)
	{
		case	FILECONTROL_TYPE_COPYFILE:
			//請輸入複製後的新檔名：
			u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_COPY_FILENAME");
			break;
		case	FILECONTROL_TYPE_RENAMEFILE:
			//請輸入新的檔名：
			u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_RENAME_FILENAME");
			break;
		case	FILECONTROL_TYPE_CREATEFILE:
			//請輸入新建立的檔案名稱：
			u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_CREATE_FILENAME");
			break;
		default:
			break;
	}
	
	memset(u_sNewFileName, 0, sizeof(u_sNewFileName));
	
	return TRUE;
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

		if(pwndTemp == u_pwndBtnOK)
		{
			//u_pwndEditFileName->GetPropValueT("text", g_sOperate_FileName, sizeof(g_sOperate_FileName));	
			u_pwndEditFileName->GetPropValueT("value", &g_sOperate_FileName, sizeof(g_sOperate_FileName));
			if(g_sOperate_FileName==0)
			{
				//視窗：輸入的檔名不得為空！
				MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_EDIT_FILENAME_NULL");
				((CtmFormView*)pwndSender)->Goto(INDEX_EDIT_FILENAME);		//光標Taborder值依據移回指定的元件
			}
			else
				Exit();		//關閉MsgBox
		}
		else if(pwndTemp == u_pwndBtnCancel)
		{
			g_sOperate_FileName=0;
			Exit();		//關閉MsgBox
		}
	
	return wIDControl;
}


WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_pwndEditFileName)		//取得使用者輸入的資料
	{
		//u_pwndEditFileName->GetPropValueT("text", g_sOperate_FileName, sizeof(g_sOperate_FileName));	
		u_pwndEditFileName->GetPropValueT("value", &g_sOperate_FileName, sizeof(g_sOperate_FileName));
	}
	
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnOK)
		{
			if(g_sOperate_FileName==0)
			{
				//視窗：輸入的檔名不得為空！
				MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_EDIT_FILENAME_NULL");
				((CtmFormView*)pwndSender)->Goto(INDEX_EDIT_FILENAME);		//光標Taborder值依據移回指定的元件
			}
			else
				Exit();		//關閉MsgBox
		}
		else if(pwndTemp == u_pwndBtnCancel)
		{
			g_sOperate_FileName=0;
			Exit();		//關閉MsgBox
		}
	}
	
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)		//判斷是否有按下Enter鍵
		u_bEnterKey	= TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey; 
}

void		OnUpdateA(CtmWnd*	pwndSender)
{
	if(WorkOne)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
	}
}
/*========================================================+
|						Helper																				|
+========================================================*/

