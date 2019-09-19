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
#include 	"ViewZero.h"
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
#define		ZeroSet_X2	0xFF2C  
#define		ZeroSet_Y2	0xFF2D  
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
CtmWnd*			pwndZeroPos_X2 = NULL;
CtmWnd*			pwndZeroPos_Y2 = NULL;

CtmWnd*			pwndBtnZero_X1 = NULL;
CtmWnd*			pwndBtnZero_Y1 = NULL;
CtmWnd*			pwndBtnZero_Z = NULL;
CtmWnd*			pwndBtnZero_X2 = NULL;
CtmWnd*			pwndBtnZero_Y2 = NULL;

char* u_pszStrID[] = 
{
	"MACHINE_INTERFACE_POSNOW_X1",
	"MACHINE_INTERFACE_POSNOW_Y1",
	"MACHINE_INTERFACE_POSNOW_Z",
	"MACHINE_INTERFACE_POSNOW_X2",
	"MACHINE_INTERFACE_POSNOW_Y2",
	"MACHINE_FUNCTION_OPTIONS_RSV27",
	"MACHINE_FUNCTION_OPTIONS_RSV28",
	"MACHINE_FUNCTION_OPTIONS_RSV29",
	"MACHINE_FUNCTION_OPTIONS_RSV30",
	"MACHINE_FUNCTION_OPTIONS_RSV31",
	NULL,
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
	pwndButton_Param_1 = pwndSender->FindControlFromName("LinuxCtmToolButton1"); 
	pwndButton_GuideSet = pwndSender->FindControlFromName("LinuxCtmToolButton2"); 
	pwndButton_Mech     = pwndSender->FindControlFromName("LinuxCtmToolButton3");
	pwndButton_Back     = pwndSender->FindControlFromName("LinuxCtmToolButton5");
	
	pwndSETZERO123     = pwndSender->FindControlFromName("SETZERO123");
	pwndSETZERO123Show	= pwndSender->FindControlFromName("SETZERO123Show");
	
	pwndEncoder_X1 = pwndSender->FindControlFromName("Encoder_X1");
	pwndEncoder_Y1 = pwndSender->FindControlFromName("Encoder_Y1");
	pwndEncoder_Z = pwndSender->FindControlFromName("Encoder_Z");
	pwndEncoder_X2 = pwndSender->FindControlFromName("Encoder_X2");
	pwndEncoder_Y2 = pwndSender->FindControlFromName("Encoder_Y2");
	if(pwndEncoder_X1!=NULL)
		printf("pwndEncoder_X1\n");
	if(pwndEncoder_Y1!=NULL)
		printf("pwndEncoder_Y1\n");
	if(pwndEncoder_Z!=NULL)
		printf("pwndEncoder_Z\n");
	if(pwndEncoder_X2!=NULL)
		printf("pwndEncoder_X2\n");
	if(pwndEncoder_Y2!=NULL)
		printf("pwndEncoder_Y2\n");

	pwndZeroPos_X1 = pwndSender->FindControlFromName("ZeroPos_X1");
	pwndZeroPos_Y1 = pwndSender->FindControlFromName("ZeroPos_Y1");
	pwndZeroPos_Z = pwndSender->FindControlFromName("ZeroPos_Z");
	pwndZeroPos_X2 = pwndSender->FindControlFromName("ZeroPos_X2");
	pwndZeroPos_Y2 = pwndSender->FindControlFromName("ZeroPos_Y2");
	
	pwndBtnZero_X1 = pwndSender->FindControlFromName("BtnZero_X1"); 
	pwndBtnZero_Y1 = pwndSender->FindControlFromName("BtnZero_Y1"); 
	pwndBtnZero_Z     = pwndSender->FindControlFromName("BtnZero_Z");
	pwndBtnZero_X2 = pwndSender->FindControlFromName("BtnZero_X2"); 
	pwndBtnZero_Y2 = pwndSender->FindControlFromName("BtnZero_Y2"); 
	
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
	switch(wKey)
	{
		case 70: // Home
			::PutCommand("Over.txt");
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
		default:
			break;
	}
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

	if(pwnd ==pwndButton_Param_1)
	{
		::PutCommand("Param_1.txt");
	}
	if(pwnd ==pwndButton_GuideSet)
	{
		::PutCommand("GuideSet.txt");
	}
	if(pwnd ==pwndButton_Mech)
	{
		::PutCommand("Mech.txt");
	}
	if(pwnd ==pwndButton_Back)
	{
		//::PutCommand("Func.txt");
		::PutCommand("Index.txt"); // cjlee 2019/4/6 下午 04:00:03
	}   
	
	if(pwnd ==pwndSETZERO123)
		{
			printf("pwndSETZERO123\n");
			SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV27",2600000000);
			SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV28",2600000000);
			SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV29",2600000000);
			SetEditValue(pwndSETZERO123Show);
		}
		
	
	else if(pwnd ==pwndBtnZero_X1 && u_PickerState==3) // 3=手動
	{
		SendCommand(ZeroSet_X1); // Send CMD to 28 set PosNow as PosZero
		DWORD l_ZeroPos_X1;
		l_ZeroPos_X1 = GetDBValue("MACHINE_INTERFACE_POSNOW_X1").lValue;
		//printf("ZeroPos_X1=%ld\n",l_ZeroPos_X1);
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV27",l_ZeroPos_X1); // Save PosZero to HMI DB
		SetEditValue(pwndZeroPos_X1);
	}    
	else if(pwnd ==pwndBtnZero_Y1 && u_PickerState==3) // 3=手動
	{
		SendCommand(ZeroSet_Y1); // Send CMD to 28 set PosNow as PosZero
		DWORD l_ZeroPos_Y1;
		l_ZeroPos_Y1 = GetDBValue("MACHINE_INTERFACE_POSNOW_Y1").lValue;
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV28",l_ZeroPos_Y1); // Save PosZero to HMI DB
		SetEditValue(pwndZeroPos_Y1);
	}    
	else if(pwnd ==pwndBtnZero_Z && u_PickerState==3) // 3=手動
	{
		SendCommand(ZeroSet_Z); // Send CMD to 28 set PosNow as PosZero
		DWORD l_ZeroPos_Z;
		l_ZeroPos_Z = GetDBValue("MACHINE_INTERFACE_POSNOW_Z").lValue;
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV29",l_ZeroPos_Z); // Save PosZero to HMI DB
		SetEditValue(pwndZeroPos_Z);
	}  
	else if(pwnd ==pwndBtnZero_X2 && u_PickerState==3) // 3=手動
	{
		SendCommand(ZeroSet_X2); // Send CMD to 28 set PosNow as PosZero
		DWORD l_ZeroPos_X2;
		l_ZeroPos_X2 = GetDBValue("MACHINE_INTERFACE_POSNOW_X2").lValue;
		//printf("ZeroPos_X1=%ld\n",l_ZeroPos_X1);
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV30",l_ZeroPos_X2); // Save PosZero to HMI DB
		SetEditValue(pwndZeroPos_X2);
	}    
	else if(pwnd ==pwndBtnZero_Y2 && u_PickerState==3) // 3=手動
	{
		SendCommand(ZeroSet_Y2); // Send CMD to 28 set PosNow as PosZero
		DWORD l_ZeroPos_Y2;
		l_ZeroPos_Y2 = GetDBValue("MACHINE_INTERFACE_POSNOW_Y2").lValue;
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV31",l_ZeroPos_Y2); // Save PosZero to HMI DB
		SetEditValue(pwndZeroPos_Y2);
	}    
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
