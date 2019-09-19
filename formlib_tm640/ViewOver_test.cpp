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
#include 	"ViewOver_test.h"
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
	
	pwndEncoder_X1 = pwndSender->FindControlFromName("Encoder_X1");
	pwndEncoder_Y1 = pwndSender->FindControlFromName("Encoder_Y1");
	pwndEncoder_Z = pwndSender->FindControlFromName("Encoder_Z");
	pwndEncoder_X2 = pwndSender->FindControlFromName("Encoder_X2");
	pwndEncoder_Y2 = pwndSender->FindControlFromName("Encoder_Y2");

	pwndZeroPos_X1 = pwndSender->FindControlFromName("ZeroPos_X1");
	pwndZeroPos_Y1 = pwndSender->FindControlFromName("ZeroPos_Y1");
	pwndZeroPos_Z = pwndSender->FindControlFromName("ZeroPos_Z");
	
	pwndBtnZero_X1 = pwndSender->FindControlFromName("BtnZero_X1"); 
	pwndBtnZero_Y1 = pwndSender->FindControlFromName("BtnZero_Y1"); 
	pwndBtnZero_Z     = pwndSender->FindControlFromName("BtnZero_Z");
	pwndBtnZero_X2 = pwndSender->FindControlFromName("BtnZero_X2"); 
	pwndBtnZero_Y2 = pwndSender->FindControlFromName("BtnZero_Y2"); 
		
	pwndBtnON_OFF_CLEAR = pwndSender->FindControlFromName("ON_OFF_CLEAR");
	pwndBtnER_CLC = pwndSender->FindControlFromName("ER_CLC");
	
	pwndAxis_X = pwndSender->FindControlFromName("LinuxCtmStaticX23");
	pwndAxis_Y = pwndSender->FindControlFromName("LinuxCtmStaticX24");
	pwndAxis_Z = pwndSender->FindControlFromName("LinuxCtmStaticX25");
	 
	
	
	// ONOFF COUNT
{
	DWORD ONOFF_COUNT;
	ONOFF_COUNT = GetDBValue("MACHINE_INTERFACE_SPEED_DCCT_Y2").lValue;
	ONOFF_COUNT++;
	printf("ONOFF_COUNT=%d\n",ONOFF_COUNT);
	SetDBValue("MACHINE_INTERFACE_SPEED_DCCT_Y2",ONOFF_COUNT);
}
	
	
	// TEST Encoder
{
	DWORD POSLAST_X1;
	DWORD POSLAST_Y1;
	DWORD POSLAST_Z;
	
	DWORD l_ZeroPos_X1;
	DWORD l_ZeroPos_Y1;
	DWORD l_ZeroPos_Z;
	
	// GET DATA
		if(g_ptaskpicker != NULL)
	{
		printf("GET ENCODER\n");
		WORD		wNum = 0;
		wNum = sizeof(u_pszStrID)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID); // Update Data from 28
	}	
	
	
	// Get POS
	{
		// Get POS Last
		POSLAST_X1 = GetDBValue("MACHINE_FUNCTION_OPTIONS_RSV27").lValue;
		POSLAST_Y1 = GetDBValue("MACHINE_FUNCTION_OPTIONS_RSV28").lValue;
		POSLAST_Z = GetDBValue("MACHINE_FUNCTION_OPTIONS_RSV29").lValue;
		printf("POSLAST_X1=%d\nPOSLAST_Y1=%d\nPOSLAST_Z=%d\n",POSLAST_X1,POSLAST_Y1,POSLAST_Z);
		
		// Get POS Now
		l_ZeroPos_X1 = GetDBValue("MACHINE_INTERFACE_POSNOW_X1").lValue;
		l_ZeroPos_Y1 = GetDBValue("MACHINE_INTERFACE_POSNOW_Y1").lValue;
		l_ZeroPos_Z = GetDBValue("MACHINE_INTERFACE_POSNOW_Z").lValue;
		printf("l_ZeroPos_X1=%d\l_ZeroPos_Y1=%d\l_ZeroPos_Z=%d\n",l_ZeroPos_X1,l_ZeroPos_Y1,l_ZeroPos_Z);

		printf("DIV X=%d, Y=%d, Z=%d \n",(l_ZeroPos_X1-POSLAST_X1),(l_ZeroPos_Y1-POSLAST_Y1),(l_ZeroPos_Z-POSLAST_Z));
	}
	// Check POS
	{
		printf("Check POS\n");
		
		if( ( l_ZeroPos_X1 >= POSLAST_X1 ? (l_ZeroPos_X1-POSLAST_X1) : (POSLAST_X1-l_ZeroPos_X1) ) > 10000)
			{
				printf("X ERROR!!!!!\n");
				pwndAxis_X->SetPropValueT("bgc", 0xF800);
				pwndAxis_X->CreateA();
				pwndAxis_X->Update();	
				SendCommand(0xFF21);
			}
		if(  ( l_ZeroPos_Y1 >= POSLAST_Y1 ? (l_ZeroPos_Y1-POSLAST_Y1) : (POSLAST_X1-l_ZeroPos_Y1) ) > 10000)
			{
				printf("Y ERROR!!!!!\n");
				pwndAxis_Y->SetPropValueT("bgc", 0xF800);
				pwndAxis_Y->CreateA();
				pwndAxis_Y->Update();	
				SendCommand(0xFF21);
			}
		if( ( l_ZeroPos_Z >= POSLAST_Z ? (l_ZeroPos_Z-POSLAST_Z) : (POSLAST_Z-l_ZeroPos_Z) ) > 10000)
			{
				printf("Z ERROR!!!!!\n");
				pwndAxis_Z->SetPropValueT("bgc", 0xF800);
				pwndAxis_Z->CreateA();
				pwndAxis_Z->Update();	
				SendCommand(0xFF21);
			}
		else
			{
				printf("Check POS PASS\n");
					//Save POS NOW
				{
					printf("Save POS NOW\n");
					SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV27",l_ZeroPos_X1); // Save PosZero to HMI DB
					SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV28",l_ZeroPos_Y1); // Save PosZero to HMI DB
					SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV29",l_ZeroPos_Z); // Save PosZero to HMI DB
				}
					//Show POS
				{
					SetEditValue(pwndZeroPos_X1);
					SetEditValue(pwndZeroPos_Y1);
					SetEditValue(pwndZeroPos_Z);
				}
				
			}

	}
	
	



	
	
}
	
	
	
	
	
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
		
	if(pwnd == pwndBtnON_OFF_CLEAR)
		{
			printf("Clear ON_OFF_COUNT\n");
			SetDBValue("MACHINE_INTERFACE_SPEED_DCCT_Y2",0);
			SetEditValue(pwndBtnON_OFF_CLEAR);
		}
	if(pwnd == pwndBtnER_CLC)
		{
			printf("Error Clear\n");
			SendCommand(0xFF22);
			
				pwndAxis_X->SetPropValueT("bgc", 0x8DA0);
				pwndAxis_X->CreateA();
				pwndAxis_X->Update();	
				pwndAxis_Y->SetPropValueT("bgc", 0x8DA0);
				pwndAxis_Y->CreateA();
				pwndAxis_Y->Update();	
				pwndAxis_Z->SetPropValueT("bgc", 0x8DA0);
				pwndAxis_Z->CreateA();
				pwndAxis_Z->Update();	
				
			//Save POS NOW
					
				DWORD ZeroPos_X1;
				DWORD ZeroPos_Y1;
				DWORD ZeroPos_Z;
			{
				printf("Save POS NOW\n");
				ZeroPos_X1 = GetDBValue("MACHINE_INTERFACE_POSNOW_X1").lValue;
				ZeroPos_Y1 = GetDBValue("MACHINE_INTERFACE_POSNOW_Y1").lValue;
				ZeroPos_Z = GetDBValue("MACHINE_INTERFACE_POSNOW_Z").lValue;
				SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV27",ZeroPos_X1); // Save PosZero to HMI DB
				SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV28",ZeroPos_Y1); // Save PosZero to HMI DB
				SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV29",ZeroPos_Z); // Save PosZero to HMI DB
			}
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
