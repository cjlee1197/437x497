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
#include	"ViewMsgBoxTeachPoint.h"
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
CtmWnd*		u_pwndEditTeachPointNo	= NULL;
CtmWnd*		u_pwndBtnOK							= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{ 
	//取得各元件指針
	u_pwndStaticMessage 		= pwndSender->FindControlFromName("StaticMessage");
	u_pwndEditTeachPointNo	= pwndSender->FindControlFromName("EditTeachPointNo");		//Mario  代表輸入的密碼，懶得改名子
	u_pwndBtnOK							= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel					= pwndSender->FindControlFromName("btnCancel");
	
	u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_ENTER_PASSWORD");

	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnOK)
		{
			char GetPassword[20] = "\0";
			memset(GetPassword, 0, sizeof(GetPassword));
			u_pwndEditTeachPointNo->GetPropValueT("text", GetPassword, sizeof(GetPassword));
			if(!strcmp(GetPassword,"1111"))
			{
				g_Pass = TRUE;
				Exit();
			}
			else
			{
			 	u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_PASSWORD_ERROR");
			 	u_pwndStaticMessage->Update();
			}
		}
		if(pwndTemp == u_pwndBtnCancel)
		{
			Exit();
		}
	}
	return wIDControl;
}



WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{

	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
if(pwndTemp == u_pwndBtnOK)
		{
			char GetPassword[20] = "\0";
			memset(GetPassword, 0, sizeof(GetPassword));
			u_pwndEditTeachPointNo->GetPropValueT("text", GetPassword, sizeof(GetPassword));
			if(!strcmp(GetPassword,"1111"))
			{
				g_Pass = TRUE;
				Exit();
			}
			else
			{
			 	u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_PASSWORD_ERROR");
			 	u_pwndStaticMessage->Update();
			}
		}
		if(pwndTemp == u_pwndBtnCancel)
		{
			Exit();
		}
		
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey	= TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey; 
}

/*========================================================+
|						Helper																				|
+========================================================*/

