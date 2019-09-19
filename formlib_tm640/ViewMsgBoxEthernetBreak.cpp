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
#include	"ViewMsgBoxEthernetBreak.h"
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

CtmWnd*		u_pwndStaticMessage						= NULL;
CtmWnd*		u_pwndStaticMessage1						= NULL;

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{	
	u_pwndStaticMessage = pwndSender->FindControlFromName("StaticMessage");
	u_pwndStaticMessage1 = pwndSender->FindControlFromName("StaticMessage1");
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	if(wIDControl==0 && Ethernet_Break==0) Exit();	
	return wIDControl; 
}


WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{

	((CtmFormView*)pwndSender)->OnKey1(wKey);		
	
	return wKey;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	//static BOOL		flag = 1;
	if(Ethernet_Break==2)
	{
		Ethernet_Break=0;
		u_pwndStaticMessage->SetPropValueT("textID","ROBOT_STR_ETHERNET_RESTART");
		u_pwndStaticMessage->Update();
		u_pwndStaticMessage1->SetPropValueT("textID","ROBOT_STR_TOUCH_EXIT");
		u_pwndStaticMessage1->Update();
		
	}
}
/*========================================================+
|						Helper																				|
+========================================================*/

