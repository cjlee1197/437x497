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
#include	"ViewParam.h"
#include	"../font.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*			pwndButton_SParam       = NULL;  
CtmWnd*			pwndButton_Funcopt       = NULL;  
CtmWnd*			pwndButton_Back       = NULL;  

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndButton_SParam     = pwndSender->FindControlFromName("LinuxCtmToolButton2"); 
	pwndButton_Funcopt     = pwndSender->FindControlFromName("LinuxCtmToolButton3"); 
	pwndButton_Back     = pwndSender->FindControlFromName("Button_Para2");
	
	long temp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
	printf("DB value =%d\n",temp);
	
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
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 1);
	long temp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
	printf("DB value =%d\n",temp);
	
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

	if(pwnd ==pwndButton_SParam)
	{
		::PutCommand("SParam.txt");
	}
	if(pwnd ==pwndButton_Funcopt)
	{
		::PutCommand("Funcopt.txt");
	}
	if(pwnd ==pwndButton_Back)
	{
		//::PutCommand("Func.txt");
		::PutCommand("Index.txt"); // cjlee 2019/4/6 ¤U¤È 04:00:27
	}
	
  return wIDControl;	
}


void	OnUpdateA(CtmWnd* pwndSender)
{
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
