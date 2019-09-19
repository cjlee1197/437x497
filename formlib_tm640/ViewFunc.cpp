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
#include	"ViewFunc.h"
#include	"../font.h"
#include	"../utils.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/


/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
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
	switch(wKey)
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
			printf("Press LOGIN\n");
			//Login();
			MsgBoxCall("msgboxUserLogin.txt");
				//if(g_Pass)
				//::PutCommand("Param_1.txt");
				::PutCommand("GuideSet.txt"); // cjlee 2019/2/22 ¤W¤È 09:53:48
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
		case 19:
			//::PutCommand("SUBTECH_0_1.txt");
			break;
		case 20:
			::PutCommand("SpeedSet.txt");
			break;	
		default:
			break;
	}
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
}
void	OnUpdateA(CtmWnd* pwndSender)
{
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

void	ChangeImage(CtmWnd* pimage, char* pszpath)
{
	char pszTemp[256];
	if (pimage != NULL)
	{
		pimage->GetPropValueT("imagepath", pszTemp,sizeof(pszTemp));
		if ((pszTemp != NULL && pszTemp[0] != '\0') && (pszpath != NULL))
		{
			if (strcmp(pszTemp, pszpath) != 0)
			{
				pimage->SetPropValueT("imagepath", pszpath);
				pimage->Update();
			}
		}
		else if ((pszTemp == NULL || pszTemp[0] == '\0') && (pszpath != NULL))
		{
			pimage->SetPropValueT("imagepath", pszpath);
			pimage->Update();
		}
		else if ((pszTemp != NULL && pszTemp[0] != '\0') && (pszpath == NULL))
		{
			pimage->SetPropValueT("imagepath", pszpath);
			pimage->Update();
		}
	}
}
