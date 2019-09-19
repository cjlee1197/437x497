/*============================================================================+
|  Class    : ViewMessageBoxConfirm library                          			    |
|  Task     : ViewMessageBoxConfirm library action source file								|
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
#include	"ViewMsgBoxSetWorkCoord.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

//各元件taborder值
#define		INDEX_BTN_OK									0

/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK								= NULL;
CtmWnd*		u_pwndBtnCancel						= NULL;
CtmWnd*		u_pwndStatic						= NULL;
BOOL			FristOne								=	TRUE;
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{	
	u_pwndBtnOK 		= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel	= pwndSender->FindControlFromName("btnCancel");
	u_pwndStatic		= pwndSender->FindControlFromName("StaticMassage");
	g_bMsgBoxFlag = FALSE;
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if((pwndTemp == u_pwndBtnOK)&&(u_bEnterKey == TRUE))
		Exit();
		
	return wIDControl;
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

		if(pwndTemp == u_pwndBtnOK)
			g_bMsgBoxFlag = TRUE;
		else if(pwndTemp == u_pwndBtnCancel)
			g_bMsgBoxFlag = FALSE;
			
		Exit();

	return wIDControl;

	return wIDControl;
}

WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	  
	((CtmFormView*)pwndSender)->OnKey1(wKey);		
	
	return wKey;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	if(FristOne)
	{
		FristOne = FALSE;
		((CtmFormView*)pwndSender)->OnLoseFocus();
	}
}
/*========================================================+
|						Helper																				|
+========================================================*/

