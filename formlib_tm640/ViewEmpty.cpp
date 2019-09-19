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
#include	"ViewEmpty.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		BUTTONMAXNUM				6
#define		SPEEDMAXNUM				    6
#define		AXISMAXNUM				    3
#define		AXIS_X_SPEED				0
#define		AXIS_Y_SPEED				1
#define		AXIS_Z_SPEED				2
#define		STATE_HAND					3
#define		COMMAND_X_FWD				0xFF01
#define		COMMAND_X_BACK				0xFF02
#define		COMMAND_Y_FWD				0xFF03
#define		COMMAND_Y_BACK				0xFF04
#define		COMMAND_Z_FWD				0xFF05
#define		COMMAND_Z_BACK				0xFF06	
#define		COMMAND_STOP				0xFFFF
#define		CONST_REQ_COMMAND   6
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
		
CtmWnd*			pwndEditPostionX = NULL;
CtmWnd*			pwndEditPostionY = NULL;
CtmWnd*			pwndEditPostionZ = NULL;
CtmWnd*			pwndButtonPara1  = NULL;
CtmWnd*			pwndButtonPara2  = NULL;

CtmWnd*			u_pwndHandSpeed[SPEEDMAXNUM] = {NULL};
CtmWnd*			u_pwndButtonHand[BUTTONMAXNUM]  = {NULL};
long	        u_lAxisSpeed[AXISMAXNUM];
WORD	        u_wPickerOPSatus = 0;

int 	u_nCommand[] = { COMMAND_X_FWD, 		COMMAND_X_BACK,
                    	 COMMAND_Y_FWD, 		COMMAND_Y_BACK,
	                     COMMAND_Z_FWD, 		COMMAND_Z_BACK,
	                   };

char*	u_pszAxisSpeedID[] = 
{
	"AXIS1_PROFILE_MAXSPEED",
	"AXIS2_PROFILE_MAXSPEED",
	"AXIS3_PROFILE_MAXSPEED",
	 NULL
};
char*	u_pszControlName[] =
{				
	"EditSet_X_FWDSpeed",
	"EditSet_X_BackSpeed",
	"EditSet_Y_FWDSpeed",
	"EditSet_Y_BackSpeed",
	"EditSet_Z_FWDSpeed",
	"EditSet_Z_BackSpeed",
	 NULL
};

char*	u_pszButtonName[] =
{
	"Button_X_FWD",
	"Button_X_Back",
	"Button_Y_FWD",
	"Button_Y_Back",
	"Button_Z_FWD",
	"Button_Z_Back",
	 NULL 
};
char* u_pszRealSpeedID[] =
{
	"AXIS1_PROFILE_JOGFWD_FVELOCITY",
	"AXIS1_PROFILE_JOGBWD_FVELOCITY",
	"AXIS2_PROFILE_JOGFWD_FVELOCITY",
	"AXIS2_PROFILE_JOGBWD_FVELOCITY",
	"AXIS3_PROFILE_JOGFWD_FVELOCITY",
	"AXIS3_PROFILE_JOGBWD_FVELOCITY",
	 NULL
};
/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
  //g_iPrivilege=10;
  int	nTemp = -2;
  pwndEditPostionX = pwndSender->FindControlFromName("EditXposition");
  pwndEditPostionY = pwndSender->FindControlFromName("EditYposition");
  pwndEditPostionZ = pwndSender->FindControlFromName("EditZposition");
  pwndButtonPara1  = pwndSender->FindControlFromName("Button_Para1");
  pwndButtonPara2  = pwndSender->FindControlFromName("Button_Para2");
  
	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
	for(int i = 0; i < SPEEDMAXNUM; i++)
	{
		u_pwndHandSpeed[i] 	= pwndSender->FindControlFromName(u_pszControlName[i]);
	}	
	
	for(int i = 0; i < BUTTONMAXNUM; i++)
	{
 		u_pwndButtonHand[i] = pwndSender->FindControlFromName(u_pszButtonName[i]);
	}
 	for(int i = 0; i < AXISMAXNUM; i++)
	{
		u_lAxisSpeed[i] = GetDBValue(u_pszAxisSpeedID[i]).lValue;
	}
	
	if(u_wPickerOPSatus != STATE_HAND)
  	{
  		for(int i = 0; i < BUTTONMAXNUM; i++ )
  		{
  			if(u_pwndButtonHand[i] != NULL)
  			{
  				u_pwndButtonHand[i]->SetTabOrder(nTemp);
  		  	}
  		}
   	}
	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = NULL;	
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;

	SetSpeedToDB(pwnd);	
	
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	SetEditValue(pwndEditPostionX);
  	SetEditValue(pwndEditPostionY);
  	SetEditValue(pwndEditPostionZ);
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
WORD	SetSpeedToDB(CtmWnd* pwnd)
{
	WORD	wSpeedPercent = 0;
	long	lSpeedValue = 0;

	for(int i = 0; i < SPEEDMAXNUM; i++)
	{
		if(pwnd == u_pwndHandSpeed[i])
		{
			u_pwndHandSpeed[i]->GetPropValueT("value", &wSpeedPercent, sizeof(WORD));
			if(i < 2)
			{
			  lSpeedValue = u_lAxisSpeed[AXIS_X_SPEED]*wSpeedPercent/100;
		  	}
		  else if(i >= 2 && i <= 3)
		  {
		  	lSpeedValue = u_lAxisSpeed[AXIS_Y_SPEED]*wSpeedPercent/100;
		  	
		  }
		  else if(i > 3 && i <= 5)
		  {
		  	lSpeedValue = u_lAxisSpeed[AXIS_Z_SPEED]*wSpeedPercent/100;
		  }
		  SetDBValue(u_pszRealSpeedID[i],lSpeedValue,TRUE);
		}
	}
	return TRUE;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL) 		return wIDControl;

	for(int i = 0; i < BUTTONMAXNUM; i++)
	{
		if(pwnd == u_pwndButtonHand[i])
		{
			SendCommand(u_nCommand[i]);
			return wIDControl;
		}
	}
  return wIDControl;	
}
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == NULL)	return wIDControl;
 		
	for(int i = 0; i < BUTTONMAXNUM; i++)
	{
		if(pwnd == u_pwndButtonHand[i])
		{
			SendCommand(COMMAND_STOP);
			return wIDControl;
 	    }
    }
    if(pwnd == pwndButtonPara1)
    {
    	::PutCommand("Param.txt");
    }
    if(pwnd == pwndButtonPara2)
    {
    	Login("password_touch.txt");
    	if(g_WndLoginFlag)
    		::PutCommand("Param_1.txt");
    }
  return wIDControl;	
}
void	SendCommand(int	CommandID)
{
	
	 	if(g_ptaskpicker != NULL)
 		{			
			g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	  		printf("Send Command = %x\n", CommandID);
		}
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
//	printf("key = %d\n", wKey);
//	switch(wKey)
//	{
//		case 1:
//    		::PutCommand("Param_1.txt");
//			break;
//		case 2:
//    		::PutCommand("GuideSet.txt");
//			break;	
//		case 3:
//    		::PutCommand("Zero.txt");
//			break;	
//		case 97:
//    		::PutCommand("Param.txt");
//			break;	
//		case 98:
//    		::PutCommand("Funcopt.txt");
//			break;	
//		case 99:
//			::PutCommand("Func.txt");
//			break;
//		default:
//			break;
//	}
	if(wKey ==30)
	{
		MsgBox(g_MultiLanguage["PICKER_ZEROSET"], tmFT_CODE_TECH);
		if(g_bMsgBoxFlag)
		{
			SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV00", 12345);
		}
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV00", 0);
}
