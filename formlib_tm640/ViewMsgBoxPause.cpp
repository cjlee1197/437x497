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

#include	"ViewMsgBoxPause.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../selectedit.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define     CONST_REQ_COMMAND       6

#define			HOMING 0 // �Ȱ���ʧ@ �^���I
#define			RESUME 1 // �Ȱ���ʧ@ �~��
/*========================================================+
|						Global Variable																|
+========================================================*/
CtmWnd*		u_pwndBtnResume						= NULL; // �~��
CtmWnd*		u_pwndBtnHoming						= NULL; // �^���I

CtmWnd*		u_pwndBtnOK								= NULL; // �T�{
CtmWnd*		u_pwndBtnCancle						= NULL; // ����

CtmWnd*		u_pwndStaticBox						= NULL; // ��r����
CtmWnd*		u_pwndMask								= NULL; // �B�n


BOOL			u_bAction									= FALSE; // �ʧ@ �~���٬O�^���I
BOOL			RunOnlyOne 								= FALSE; // �u����@��
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnResume 			= pwndSender->FindControlFromName("BtnResume");
	u_pwndBtnHoming				= pwndSender->FindControlFromName("BtnHoming");
	u_pwndBtnOK 					= pwndSender->FindControlFromName("BtnOK");
	u_pwndBtnCancle				= pwndSender->FindControlFromName("BtnCancle");
	u_pwndStaticBox 			= pwndSender->FindControlFromName("StaticBox");
	u_pwndMask 						= pwndSender->FindControlFromName("Mask");

	u_pwndBtnOK->SetPropValueT("taborder", -2);
	u_pwndBtnCancle->SetPropValueT("taborder", -2);
		
	return TRUE;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnResume)
	{
		printf("Resume\n");			
		u_bAction = RESUME;
		
		u_pwndBtnResume->SetPropValueT("taborder", -2);
		u_pwndBtnResume->Update();
		u_pwndBtnHoming->SetPropValueT("taborder", -2);
		u_pwndBtnHoming->Update();
		u_pwndMask->Update();
		u_pwndBtnOK->SetPropValueT("taborder", 1);
		u_pwndBtnOK->Update();
		u_pwndBtnCancle->SetPropValueT("taborder", 2);
		u_pwndBtnCancle->Update();
		
		u_pwndStaticBox->SetPropValueT("textID", "PICKER_PAUSE_CONFIRM_RESUME");
		u_pwndStaticBox->Update();
		SetDBValue("MACHINE_CONFIGURATION_IO_WOPTION", HOMING);
	}
	else if(pwndTemp == u_pwndBtnHoming)
	{
		printf("Homing\n");	
		u_bAction = HOMING;
		
		u_pwndBtnResume->SetPropValueT("taborder", -2);
		u_pwndBtnResume->Update();
		u_pwndBtnHoming->SetPropValueT("taborder", -2);
		u_pwndBtnHoming->Update();
		u_pwndMask->Update();
		u_pwndBtnOK->SetPropValueT("taborder", 1);
		u_pwndBtnOK->Update();
		u_pwndBtnCancle->SetPropValueT("taborder", 2);
		u_pwndBtnCancle->Update();
		
		u_pwndStaticBox->SetPropValueT("textID", "PICKER_PAUSE_CONFIRM_HOMING");
		u_pwndStaticBox->Update();
		SetDBValue("MACHINE_CONFIGURATION_IO_WOPTION", RESUME);
	}

	else if(pwndTemp == u_pwndBtnOK)
	{
		printf("OK\n");
//		if(u_bAction) // �~��
//			SetDBValue("MACHINE_CONFIGURATION_IO_WOPTION", HOMING);
//		else
//			SetDBValue("MACHINE_CONFIGURATION_IO_WOPTION", RESUME);
			
    //SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 3);
    SendCommand(0xFF48); // ACTKEY_ALLOWRESUME �e��28 ���\�~��R�O		
    Exit();
	}
	else if(pwndTemp == u_pwndBtnCancle)
	{
		u_pwndStaticBox->SetPropValueT("textID", "PICKER_PAUSE_CONFIRM");
		u_pwndStaticBox->Update();
		

		u_pwndBtnOK->SetPropValueT("taborder", -2);
		u_pwndBtnOK->Update();
		u_pwndBtnCancle->SetPropValueT("taborder", -2);
		u_pwndBtnCancle->Update();
		u_pwndMask->Update();
		u_pwndBtnResume->SetPropValueT("taborder", 3);
		u_pwndBtnResume->Update();
		u_pwndBtnHoming->SetPropValueT("taborder", 4);
		u_pwndBtnHoming->Update();
	}


	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;	
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
		{
			u_pwndMask->Update();
			u_pwndBtnResume->Update();
			u_pwndBtnHoming->Update();
			u_pwndStaticBox->SetPropValueT("textID", "PICKER_PAUSE_CONFIRM");
			u_pwndStaticBox->Update();
			RunOnlyOne=TRUE;
		}
}
/*========================================================+
|						Helper																				|
+========================================================*/

/*---------------------------------------------------------------------------+
|  Function : SendCommand()                    	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SendCommand(int	CommandID)
{
	if(g_ptaskpicker != NULL)
 	{			
		g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	//printf("Send Command = %x\n", CommandID);
	}
}