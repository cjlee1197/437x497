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
#include	"ViewTeachOption.h"
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
			//::PutCommand("Param.txt");
			break;	
		case 2:
			::PutCommand("QTeach.txt");
			break;
		case 10:
			::PutCommand("PG_0.txt");
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

