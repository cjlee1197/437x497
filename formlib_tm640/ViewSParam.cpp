/*===========================================================================+
|  Class    : View initial login machine                                     |
|  Task     : View initial login machine                                     |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#include	"ViewSParam.h"
#include	"../font.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*			pwndButton_Param       = NULL;  
CtmWnd*			pwndButton_Funcopt       = NULL;  
CtmWnd*			pwndButton_Back       = NULL;  

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndButton_Param     = pwndSender->FindControlFromName("LinuxCtmToolButton1"); 
	pwndButton_Funcopt     = pwndSender->FindControlFromName("LinuxCtmToolButton3"); 
	pwndButton_Back     = pwndSender->FindControlFromName("Button_Para2");
	
	return TRUE;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{              
	return wIDControl;
}

WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	/*switch(wKey)
	{
		case 1:
			::PutCommand("Internal_IO.txt");
			break;
		case 2:
			::PutCommand("Hand.txt");
			break;
		case 3:
			::PutCommand("MldS.txt");
			break;
		case 4:
			::PutCommand("OnlineProgram_0.txt");
			break;
		case 5:
			::PutCommand("OnlineProgram_X1.txt");
			break;
		case 6:
			::PutCommand("PG_0.txt");
			break;
		case 10:
			::PutCommand("Param.txt");
			break;	
		case 11:
			::PutCommand("Syst.txt");
			break;
		case 12:
			::PutCommand("Reset.txt");
			break;
		case 13:
			::PutCommand("GuideSet.txt");
			break;
		case 14:
			::PutCommand("Prod.txt");
			break;
		case 15:
			Login();
    		if(g_WndLoginFlag)
				::PutCommand("Param_1.txt");
			break;
		case 16:
			::PutCommand("Zero.txt");
			break;
		case 17:
			::PutCommand("Pile_1.txt");
			break;
		case 18:
			::PutCommand("OnlineProgram_X1.txt");
			break;
		default:
			break;
	}*/
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
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
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_Param)
	{
		::PutCommand("Param.txt");
	}
	if(pwnd ==pwndButton_Funcopt)
	{
		::PutCommand("Funcopt.txt");
	}
	if(pwnd ==pwndButton_Back)
	{
		::PutCommand("Func.txt");
	}
	
  return wIDControl;	
}


void	OnUpdateA(CtmWnd* pwndSender)
{
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
