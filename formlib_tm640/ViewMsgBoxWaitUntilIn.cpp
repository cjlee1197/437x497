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

#include	"ViewMsgBoxWaitUntilIn.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndEditTable01				= NULL;
CtmWnd*		u_pwndEditTable02				= NULL;
/*========================================================+
|						Function																			|
+========================================================*/


BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditTable01	= pwndSender->FindControlFromName("EditTable01");
	u_pwndEditTable02	= pwndSender->FindControlFromName("EditTable02");
	
	return TRUE;
}

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
			int temp2=0;
			u_pwndEditTable01->GetPropValueT("value", &temp2, sizeof(temp2));
			sprintf(temp,"WAIT_UNTIL_IN(%d) =",temp2);
			u_pwndEditTable02->GetPropValueT("value", &temp2, sizeof(temp2));
			sprintf(temp,"%s%d;",temp,temp2);
			AddCommand(temp);
				Exit();
		}
	}
	
	return wIDControl;
}
//20140624��

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
			if(pwndTemp == u_pwndBtnCancel)
		{
			remove("./DialogHelp/buffer");
			Exit();
		}
		else if(pwndTemp == u_pwndBtnOK)
		{
			char temp[64]="\0";
			int temp2=0;
			u_pwndEditTable01->GetPropValueT("value", &temp2, sizeof(temp2));
			sprintf(temp,"WAIT_UNTIL_IN(%d) =",temp2);
			u_pwndEditTable02->GetPropValueT("value", &temp2, sizeof(temp2));
			sprintf(temp,"%s%d;",temp,temp2);
			AddCommand(temp);
				Exit();
		}
	
		((CtmFormView*)pwndSender)->Goto(wIDControl);
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


/*========================================================+
|						Helper																				|
+========================================================*/

void		AddCommand(char* pszCommand)		//���J���O
{
	//�ǥ��B�~���ͪ��ɮרӼȦs�n���J�����O
	FILE*		fCommandBuffer;
	fCommandBuffer = fopen("./DialogHelp/buffer", "w");
	fprintf(fCommandBuffer, "%s", pszCommand);
	fclose(fCommandBuffer);
}