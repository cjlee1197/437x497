/*============================================================================+
|  Class    : ViewMessageBoxTeachPoint library                               	|
|  Task     : ViewMessageBoxTeachPoint library action source file							|
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
#include	"ViewMsgBoxFileReName.h"
#include	"../utils.h"
#include	"../commonaction.h"

/*========================================================+
|						Constants																			|
+========================================================*/


/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey							= FALSE;

CtmWnd*		u_pwndStaticMessage			= NULL;
CtmWnd*		u_pwndEditFileName	    = NULL;
CtmWnd*		u_pwndBtnOK							= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;

//各元件taborder值
#define		INDEX_EDIT_FILENAME						0
#define		INDEX_BTN_OK									1
#define		INDEX_BTN_CANCEL							2

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{ 
	//取得各元件指針
	u_pwndStaticMessage 		= pwndSender->FindControlFromName("StaticMessage");
	u_pwndEditFileName	= pwndSender->FindControlFromName("EditFileName");		// 輸入新的檔案名稱
	u_pwndBtnOK							= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel					= pwndSender->FindControlFromName("btnCancel");
	
	u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_RENAME_FILENAME");
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	Exit();
	return wIDControl;
}



WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

		if(pwndTemp == u_pwndBtnOK)
		{
			//u_pwndEditFileName->GetPropValueT("text", g_sOperate_FileName, sizeof(g_sOperate_FileName));	
			u_pwndEditFileName->GetPropValueT("text", &g_Now_FileName, sizeof(g_Now_FileName));
			if(g_Now_FileName[0]=='\0')
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
			memset(g_Now_FileName, '\0', sizeof(g_Now_FileName));
			//g_Now_FileName='\0';
			Exit();		//關閉MsgBox
		}
	
	return wIDControl;
		
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	
	return wKey; 
}

/*========================================================+
|						Helper																				|
+========================================================*/

