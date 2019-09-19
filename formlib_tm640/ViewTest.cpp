/*===========================================================================+
|  Class    : ViewZero library                                        		   |
|  Task     : ViewZero library action source file                  		       |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 										                                           |
|  Version  : V1.00      			                                               |
|  Creation : 			                                            		         |
|  Revision :   																														 |
+===========================================================================*/
#include 	"ViewTest.h"
#include	"../main.h"
#include	"../_kb.h"
#include 	"../common.h"
#include 	"../commonaction.h"
#include	"../timer.h"
#include	"../utils.h"
#include	"../manufacture.h"
#include	"../tmdebug.h"
#include	"../selvat_ancestor.h"
#include	"../tmconfig.h"
#include	"../lib/libiNetComm.h"
#include	"../tmshmsg.h"
#include	"../lib/libControl.h"
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		REQ_READMOTOR    				2   
#define		ZeroSet_X1	0xFF29  
#define		ZeroSet_Y1	0xFF2A  
#define		ZeroSet_Z		0xFF2B  
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*			pwndButton_Param_1       = NULL;  
CtmWnd*			pwndButton_GuideSet       = NULL;  
CtmWnd*			pwndButton_Mech       = NULL;
CtmWnd*			pwndButton_Back       = NULL;
  
CtmWnd*			pwndSETZERO123       = NULL;
CtmWnd*			pwndSETZERO123Show       = NULL;


CtmWnd*			pwndEncoder_X1 = NULL;
CtmWnd*			pwndEncoder_Y1 = NULL;
CtmWnd*			pwndEncoder_Z = NULL;
CtmWnd*			pwndEncoder_X2 = NULL;
CtmWnd*			pwndEncoder_Y2 = NULL;

CtmWnd*			pwndZeroPos_X1 = NULL;
CtmWnd*			pwndZeroPos_Y1 = NULL;
CtmWnd*			pwndZeroPos_Z = NULL;


CtmWnd*			pwndBtnZero_X1 = NULL;
CtmWnd*			pwndBtnZero_Y1 = NULL;
CtmWnd*			pwndBtnZero_Z = NULL;
CtmWnd*			pwndBtnZero_X2 = NULL;
CtmWnd*			pwndBtnZero_Y2 = NULL;


CtmWnd*			pwndBtnON_OFF_CLEAR = NULL;
CtmWnd*			pwndBtnER_CLC = NULL;

CtmWnd*			pwndAxis_X = NULL;
CtmWnd*			pwndAxis_Y = NULL;
CtmWnd*			pwndAxis_Z = NULL;


char* u_pszStrID[] = 
{
	"MACHINE_INTERFACE_POSNOW_X1",
	"MACHINE_INTERFACE_POSNOW_Y1",
	"MACHINE_INTERFACE_POSNOW_Z",
	"MACHINE_INTERFACE_POSNOW_Y1",
	"MACHINE_INTERFACE_POSNOW_X2",
	NULL,
//		"MACHINE_INTERFACE_CONTROL_RSV15",
//		"MACHINE_INTERFACE_CONTROL_RSV16",
//		"MACHINE_INTERFACE_CONTROL_RSV17",
//		"MACHINE_INTERFACE_CONTROL_RSV18",
//		NULL,
};
/*---------------------------------------------------------------------------+
|           View Content - GuideSet                                          |
+---------------------------------------------------------------------------*/ 

/*---------------------------------------------------------------------------+
|  Function : OnCreateA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	
	pwndSETZERO123     = pwndSender->FindControlFromName("SETZERO123");
	pwndSETZERO123Show	= pwndSender->FindControlFromName("SETZERO123Show");
		
	
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pWnd = NULL;
	DWORD		dwTempValue = 0;

	pWnd = pwndSender->FindControlFromTab(wIDControl);
	if(pWnd == NULL) return wIDControl;
		
	return wIDControl;
}

/*---------------------------------------------------------------------------+
|  Function : OnKeyA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
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
 	WORD u_PickerState = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
 	
 	if(pwnd == NULL)	return wIDControl;
		

  return wIDControl;	
}




/*---------------------------------------------------------------------------+
|  Function : OnDestroyA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnDestroyA(CtmWnd* pwndSender)
{
}

/*---------------------------------------------------------------------------+
|  Function : OnShowA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnShowA(CtmWnd* pwndSender)
{

}

/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateA(CtmWnd* pwndSender)
{

	
	if(g_ptaskpicker != NULL)
	{
		WORD		wNum = 0;
		wNum = sizeof(u_pszStrID)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID); // Update Data from 28
	}	
}


/*---------------------------------------------------------------------------+
|  Function : SendCommand()                      	     	                     |
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
			g_ptaskpicker->ReqValues(6, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
		}
}

void		SetEditValue(CtmWnd* pwnd)
{
	char	pDataID[256];
	long    temp=0;
	if(pwnd != NULL)
	{
		pwnd->GetPropValueT("dbid0", pDataID,sizeof(pDataID));
		temp = GetDBValue(pDataID).lValue;
		if(pDataID!=NULL && temp!=0)
			printf("Get %s=%d\n",pDataID,temp);
			
		if(pDataID != NULL && pDataID[0] != '\0')
		{
			pwnd->SetPropValueT("value",temp);
			pwnd->Update();
		}
	}
}
