/*===========================================================================+
|  Class    : ViewCore library                                          	 |
|  Task     : ViewCore library action source file                       	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewUserLog.h"
//#include	"../control.h"
#include	"../tmdebug.h"
#include	"../tmcontrol.h"
#include 	"../oper_log.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL RunOnlyOne = FALSE;

WORD LogCount;
tmOper_log* Log;

CtmWnd*		pwndBtnTest	= NULL; // Btn
CtmWnd*		pwndTest_Mask	= NULL; // Mask
/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndBtnTest	= pwndSender->FindControlFromName("BtnTest");
	pwndTest_Mask	= pwndSender->FindControlFromName("Test_Mask");

	LogCount = g_ptmControlServer->Oper_Log()->GetOperLogCount();
	printf("LogCount=%d\n",LogCount);
	Log = g_ptmControlServer->Oper_Log()->GetOperLog(1);
	printf("wDBIndex=%d\n",Log->wDBIndex);
	printf("dNewValue=%d\n",Log->dNewValue);
	printf("dOldValue=%d\n",Log->dOldValue);



	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = NULL;	
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
		{
			if(pwndTest_Mask!=NULL)
				{
					pwndTest_Mask->Update();
					printf("pwndTest_Mask->Update();\n");
				}
			if(pwndBtnTest!=NULL)
				{
					pwndBtnTest->Update();
					pwndBtnTest->UpdateAll();
					printf("pwndTest_Mask->Update();\n");
				}
			RunOnlyOne = TRUE;
		}

}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{


	
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|  Function : OnMouseUp()                       	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("OnMouseUp\n");
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTest_Mask!=NULL)
		{
			pwndTest_Mask->Update();
			printf("pwndTest_Mask->Update();\n");
		}
	if(pwndBtnTest!=NULL)
		{
			pwndBtnTest->Update();
			pwndBtnTest->UpdateAll();
			printf("pwndTest_Mask->Update();\n");
		}

}

void	OnDestroyA(CtmWnd* pwndSender)
{
}

