/*===========================================================================+
|  Class    : ViewGuideSet library                                           |
|  Task     : ViewGuideSet library action source file                        |
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
#include 	"ViewZeroP.h"
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
#define		NULL										0
#define		TYPE_MOTOR							1
#define		TYPE_WAIT						  	2
#define		TYPE_PERMIT						  3
#define		TYPE_VALVE							4
#define		TYPE_LABEL							5
#define		TYPE_JUMP						  	6		

#define		MOTOR_AXIS_X						1
#define		MOTOR_AXIS_Y						2
#define		MOTOR_AXIS_Z						3

#define		WAIT_MOULD_OPEN					1
#define		WAIT_GATE_CLOSE					2
#define		WAIT_THIMBLE_BACK				3
#define		WAIT_THIMBLE_FWD				4
#define		WAIT_MOULD_CLOSE				5
#define		WAIT_PRODUCT_BAD        6
#define		WAIT_PERMI_OPERATION    7

#define		PERMIT_MOULD_CLOSE			1
#define		PERMIT_MOULD_OPEN				2
#define		PERMIT_THIMBLE_BACK			3
#define		PERMIT_THIMBLE_FWD			4

#define		VALVE_CLAMP             1
#define		VALVE_GUEST_HORIZONTAL  2
#define		VALVE_GUEST_VERTICAL    3

#define		LABEL_1									1
#define		LABEL_2                 2
#define		LABEL_3									3
#define		LABEL_4									4


STEP	StepData[100] =
{
	{1,		TYPE_MOTOR,		MOTOR_AXIS_Z,						  0,	  300},
	{2,		TYPE_MOTOR,   MOTOR_AXIS_X,    				  0,    300},
	{3,		TYPE_MOTOR,		MOTOR_AXIS_Y,		 				  0,	  300},
	{4,		TYPE_PERMIT,  PERMIT_MOULD_CLOSE, 		  1,		  0},
	{5,		TYPE_WAIT,		WAIT_MOULD_CLOSE,				  0,		  0},
	{6,		TYPE_PERMIT,	PERMIT_MOULD_OPEN,				1,		  0},
	{7,   TYPE_LABEL,		LABEL_1,									0,		  0},
	{8,		TYPE_WAIT,    WAIT_MOULD_OPEN,				  0,			0},
	{9,		TYPE_PERMIT,	PERMIT_MOULD_CLOSE,				0,			0},
	{10,	TYPE_MOTOR,		MOTOR_AXIS_Y,					    0,	  900},
	{11,	TYPE_PERMIT,  PERMIT_THIMBLE_FWD,				1,			0},
	{12,	TYPE_WAIT,		WAIT_THIMBLE_FWD,					0,			0},
	{13,	TYPE_PERMIT,	PERMIT_THIMBLE_FWD,				0,			0},
	{14,	TYPE_MOTOR,		MOTOR_AXIS_X,					  	0,	 1000},
	{15,	TYPE_VALVE,		VALVE_CLAMP,							1,			0},
	{16,	TYPE_PERMIT,	PERMIT_THIMBLE_BACK,	  	1,			0},
	{17,	TYPE_WAIT,		WAIT_THIMBLE_BACK,				0,			0},
	{18,	TYPE_PERMIT,	PERMIT_THIMBLE_BACK,      0,			0},
	{19,	TYPE_MOTOR,		MOTOR_AXIS_X,						  0,	 1000},
  {20,	TYPE_MOTOR,		MOTOR_AXIS_Y,						  0,	  900},	
  {21,  TYPE_PERMIT,  PERMIT_MOULD_CLOSE,				1,			0},
 	{22,	TYPE_MOTOR,		MOTOR_AXIS_X,					    0,	 1000},	 
	{23,	TYPE_VALVE,		VALVE_GUEST_HORIZONTAL,		1,			0},
  {24,	TYPE_MOTOR,		MOTOR_AXIS_Z,				    	0,	 1000},
  {25,	TYPE_MOTOR,		MOTOR_AXIS_Y,					    0,    900},
	{26,	TYPE_VALVE,		VALVE_CLAMP,							0,			0},	
  {27,	TYPE_MOTOR,		MOTOR_AXIS_Y,				  	  0,    900},
	{28,	TYPE_MOTOR,		MOTOR_AXIS_Z,						  0,	 1000},	
	{29,	TYPE_VALVE,		VALVE_GUEST_VERTICAL,			1,			0},
  {30,	TYPE_MOTOR,		MOTOR_AXIS_X,						  0,	 1000},	
	{31,	TYPE_JUMP,		LABEL_1,									0,			0}
};
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*			pwndButton_Param_1       = NULL;  
CtmWnd*			pwndButton_GuideSet       = NULL;  
CtmWnd*			pwndButton_Back       = NULL;  

CtmWnd*			pwndEditPostionX = NULL;
CtmWnd*			pwndEditPostionY = NULL;
CtmWnd*			pwndEditPostionZ = NULL;

CtmWnd*	u_pwndEditWait[3] = {NULL};
CtmWnd* u_pwndEditGet[3]  = {NULL};
CtmWnd* u_pwndEditput[3]  = {NULL};
CtmWnd*   pwndEditNum[64] = {NULL};
int	 nEditNum 	= 0;

char*	u_pszEditWaitName[] =
{
	"EditWait_X",
	"EditWait_Y",
	"EditWait_Z"
}; 

char*	u_pszEditGetName[] = 
{
	"EditGetProduct_X",
	"EditGetProduct_Y",
	"EditGetProduct_Z"
};

char*	u_pszEditPutName[] =
{
	"EditPutProduct_X",
	"EditPutProduct_Y",
	"EditPutProduct_Z"
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
	pwndButton_Back     = pwndSender->FindControlFromName("LinuxCtmToolButton4");
	
	pwndEditPostionX = pwndSender->FindControlFromName("EditXposition");
 	pwndEditPostionY = pwndSender->FindControlFromName("EditYposition");
  	pwndEditPostionZ = pwndSender->FindControlFromName("EditZposition");
	nEditNum	= GetSpecialControlNum(pwndSender,	"_",	"CtmEditX1",	pwndEditNum);
	for(int i = 0; i < 3; i++)
	{
		u_pwndEditWait[i] = pwndSender->FindControlFromName(u_pszEditWaitName[i]);
		u_pwndEditGet[i] = pwndSender->FindControlFromName(u_pszEditGetName[i]);
		u_pwndEditput[i] = pwndSender->FindControlFromName(u_pszEditPutName[i]);
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
//	printf("key = %d\n", wKey);
//	switch(wKey)
//	{
//		case 1:
//    		::PutCommand("Param_1.txt");
//			break;
//		case 3:
//    		::PutCommand("Zero.txt");
//			break;
//		case 99:
//			::PutCommand("Func.txt");
//			break;
//		default:
//			break;
//	}
//	if(wKey == 0x0001)
//	{
//		MsgBox(g_MultiLanguage["PICKER_CONFIRMCREATEFLOW"], tmFT_CODE_TECH);
//	  if(g_bMsgBoxFlag ==TRUE)
//	  {
//	  	Prompt(g_MultiLanguage["PICKER_CREATEFLOWNOW"]);
//			CreateRecipe();
//			Prompt(g_MultiLanguage["PICKER_CREATEFLOWFINISH"]);
//		}		
//	}
//	if(wKey == 0x0030)
//	{
//		((CtmFormView *)pwndSender)->Goto(97);
//		SetDBValue("AXIS1_PROFILE_WAITPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_XAXISPOSITION").lValue);
//		SetDBValue("AXIS2_PROFILE_WAITPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_YAXISPOSITION").lValue);
//		SetDBValue("AXIS3_PROFILE_WAITANDGETPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_ZAXISPOSITION").lValue);
//		for(int i = 0; i < nEditNum; i++)
//		{
//			pwndEditNum[i]->Update();
//		}
//	}
//	if(wKey == 0x0040)
//	{
//		((CtmFormView *)pwndSender)->Goto(98);
//		SetDBValue("AXIS1_PROFILE_GETPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_XAXISPOSITION").lValue);
//		SetDBValue("AXIS2_PROFILE_GETPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_YAXISPOSITION").lValue);
//		SetDBValue("AXIS3_PROFILE_WAITANDGETPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_ZAXISPOSITION").lValue);
//		for(int i = 0; i < nEditNum; i++)
//		{
//			pwndEditNum[i]->Update();
//		}
//	}
//	if(wKey == 0x0050)
//	{
//		((CtmFormView *)pwndSender)->Goto(99);
//		SetDBValue("AXIS1_PROFILE_PUTPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_XAXISPOSITION").lValue);
//		SetDBValue("AXIS2_PROFILE_PUTPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_YAXISPOSITION").lValue);
//		SetDBValue("AXIS3_PROFILE_PUTPRODUCTPOSITION", GetDBValue("MACHINE_INTERFACE_CONTROL_ZAXISPOSITION").lValue);
//		for(int i = 0; i < nEditNum; i++)
//		{
//			pwndEditNum[i]->Update();
//		}
//	}
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
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_Param_1)
	{
		::PutCommand("Param_1.txt");
	}
	if(pwnd ==pwndButton_GuideSet)
	{
		::PutCommand("GuideSet.txt");
	}
	if(pwnd ==pwndButton_Back)
	{
		::PutCommand("Func.txt");
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
	SetEditValue(pwndEditPostionX);
  	SetEditValue(pwndEditPostionY);
  	SetEditValue(pwndEditPostionZ);
}

/*---------------------------------------------------------------------------+
|  Function : CreateRecipe()                        	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CreateRecipe()
{
	char	szTempStrID[256] = "\0";
	long	lTempValue = 0;
	
	for(int i = 0; i < 3; i++)
	{
		u_pwndEditWait[i]->GetPropValueT("dbid0", szTempStrID, sizeof(szTempStrID));
		lTempValue = GetDBValue(szTempStrID).lValue;
		switch(i)
		{
			case	0:
				StepData[1].dwParam1 = lTempValue;
				StepData[18].dwParam1 = lTempValue;
				StepData[29].dwParam1 = lTempValue;
				break;
			case	1:
				StepData[2].dwParam1 = lTempValue;
				StepData[19].dwParam1 = lTempValue;
				StepData[26].dwParam1 = lTempValue;
				break;
			case	2:
				StepData[0].dwParam1 = lTempValue;
				StepData[27].dwParam1 = lTempValue;
				break;
			default:
				break;	
		}
	}
	
	for(int i = 0; i < 3; i++)
	{
		u_pwndEditGet[i]->GetPropValueT("dbid0", szTempStrID, sizeof(szTempStrID));
		lTempValue = GetDBValue(szTempStrID).lValue;
		switch(i)
		{
			case	0:
				StepData[13].dwParam1 = lTempValue;
				break;
			case	1:
				StepData[9].dwParam1 = lTempValue;
				break;
			case	2:
				break;
			default:
				break;	
		}
	}
	
	for(int i = 0; i < 3; i++)
	{
		u_pwndEditput[i]->GetPropValueT("dbid0", szTempStrID, sizeof(szTempStrID));
		lTempValue = GetDBValue(szTempStrID).lValue;
		switch(i)
		{
			case	0:
				StepData[21].dwParam1 = lTempValue;
				break;
			case	1:
				StepData[24].dwParam1 = lTempValue;
				break;
			case	2:
				StepData[23].dwParam1 = lTempValue;
				break;
			default:
				break;	
		}
	}
		
	for(int i = 0; i < 31; i++)
	{
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", i+1);
		SetDBValue(szTempStrID,StepData[i].wStep, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i+1);
		SetDBValue(szTempStrID,StepData[i].wType, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", i+1);
		SetDBValue(szTempStrID,StepData[i].wNum, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i+1);
		SetDBValue(szTempStrID,StepData[i].dwParam1, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i+1);
		SetDBValue(szTempStrID,StepData[i].dwParam2, FALSE);
	}
	for(int i = 31; i < 100; i++)
	{
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", i+1);
		SetDBValue(szTempStrID, 0, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i+1);
		SetDBValue(szTempStrID, 0, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", i+1);
		SetDBValue(szTempStrID, 0, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i+1);
		SetDBValue(szTempStrID, 0, FALSE);
		sprintf(szTempStrID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i+1);
		SetDBValue(szTempStrID, 0, FALSE);
	}
}
void		SetEditValue(CtmWnd* pwnd)
{
	char	pDataID[256];
	long long lTemp;
	//printf("Name = %s\n",pwnd->Name);
	if(pwnd != NULL)
	{
		if(pwnd->Is("CtmEditX1"))
		{
			pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
      		lTemp = (long)GetDBValue(pDataID).lValue;
  		}
    	else
   		{
   			pwnd->GetPropValueT("dbid0", pDataID,sizeof(pDataID));
      		lTemp = GetDBValue(pDataID).lValue/100;
   			//printf("StrID = %s, Value = %d\n", pDataID, lTemp);
   		}
			//printf("lTemp = %ld\n",lTemp);
		if(pDataID != NULL && pDataID[0] != '\0')
		{
			pwnd->SetPropValueT("value",lTemp);
			pwnd->UpdateAll();
		}
	}	
}