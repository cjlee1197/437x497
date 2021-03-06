/*===========================================================================+
|  Class    : ViewProd library                                               |
|  Task     : ViewProd library  action source file                           |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include 	<math.h>
#include 	"ViewProd.h"
#include	"../tmcontrol.h"
#include 	"../main.h"
#include	"../utils.h"
#include	"../manufacture.h"
#include	"../selectedit.h"
#include	"../tmconfig.h"
#include	"../iotable.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		CONST_REQ_COMMAND   6

#define		UNWORD(hw,lw)       (((DWORD)(hw) << 16) | lw)/*Double Word*/
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/            
CtmWnd*			pwndButton_Back       = NULL; 

CtmWnd*			pwndTotalCount      			 = NULL;
CtmWnd*			pwndTotalGoodCount  			 = NULL;
CtmWnd*			pwndTotalbadCount   			 = NULL;
CtmWnd*	        pwndTotalSampleCount			 = NULL;
CtmWnd*	        pwndeditCOUNTPERUNIT			 = NULL;


/*---------------------------------------------------------------------------+
|           View Content -                                                   |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndButton_Back     = pwndSender->FindControlFromName("LinuxCtmToolButton5");
	
	pwndTotalCount      	  = pwndSender->FindControlFromName("TotalCount");
	pwndTotalGoodCount        = pwndSender->FindControlFromName("TotalGoodCount");
	pwndTotalbadCount         = pwndSender->FindControlFromName("TotalbadCount");
	pwndTotalSampleCount      = pwndSender->FindControlFromName("TotalSampleCount");
	pwndeditCOUNTPERUNIT      = pwndSender->FindControlFromName("editCOUNTPERUNIT");
	
	SetEditValue(pwndTotalCount);
	SetEditValue(pwndTotalGoodCount);
	SetEditValue(pwndTotalbadCount);
	SetEditValue(pwndTotalSampleCount);
	
	return TRUE;
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	   pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)  return wIDControl;
		
	if(pwnd == pwndeditCOUNTPERUNIT)
	{
		SetEditValue(pwndTotalCount);
		SetEditValue(pwndTotalGoodCount);
		SetEditValue(pwndTotalbadCount);
		SetEditValue(pwndTotalSampleCount);
	}
	
	return wIDControl;
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
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_Back)
	{
		//::PutCommand("Func.txt");
		::PutCommand("Index.txt"); // cjlee 2019/4/6 と 05:10:43
	}   

  return wIDControl;	
}


void	OnUpdateA(CtmWnd* pwndSender)
{
	
}
WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
//		case 99:
//			::PutCommand("Func.txt");
//			break;
		case 70: // Home
			::PutCommand("Over.txt"); // cjlee 2019/4/17 と 06:56:41
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
		case 0xFF4A: // 玻珇睲箂
			SendCommand(0xFF4A);
			break;
		default:
			break;
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType)
{
	if(pwnd != NULL)
	{
		if(wType)
			  pwnd->SetPropValueT("wtype",wType); 
		else
		{
		   	if(bVisible)
				pwnd->SetPropValueT("wtype",0x21);
			else
        		pwnd->SetPropValueT("wtype",0x30);
    }
			pwnd->Update();

			pwnd->SetPropValueT("visible",(double)bVisible);
			pwnd->Update();
	}		
}
void		SetEditValue(CtmWnd* pwnd)
{
	char	  pDataID[256];
	long long lTemp =0;
	if(pwnd != NULL)
	{
		WORD wMoldNum =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED2").lValue;
		pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));

		if( strcmp(pDataID,"MACHINE_INTERFACE_MONITORMODES")==0 ) // 羆玻秖
			{
				lTemp = GetDBValue(pDataID).lValue * wMoldNum; // 虫家玻珇计*秨家计
			}
		else if( strcmp(pDataID,"MACHINE_INTERFACE_MONITOR_RSV02")==0 ) // 羆ぃ▆珇
			{
				lTemp = GetDBValue(pDataID).lValue * wMoldNum; // 虫家玻珇计*秨家计
			} 
		else if( strcmp(pDataID,"MACHINE_INTERFACE_MONITOR_RSV09")==0 ) // 羆▆珇
			{
				lTemp = ( GetDBValue("MACHINE_INTERFACE_MONITORMODES").lValue - GetDBValue("MACHINE_INTERFACE_MONITOR_RSV02").lValue )* wMoldNum; // (羆玻秖-羆ぃ▆珇)*秨家计
			} 
		pwnd->SetPropValueT("value",lTemp);
		pwnd->UpdateAll();
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
			g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
		}
}
