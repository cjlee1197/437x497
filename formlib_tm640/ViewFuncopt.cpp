/*===========================================================================+
|  Class    : View initial login machine                                     |
|  Task     : View initial login machine                                     |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         	 |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#include	"ViewFuncopt.h"
#include	"../font.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		ON					1
#define		OFF					0
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*			pwndButton_Param       = NULL;  
CtmWnd*			pwndButton_SParam       = NULL;  
CtmWnd*			pwndButton_Back       = NULL;  

CtmWnd*			pwndBtn_MovePType[3] = {NULL};
int MovePTypeNo=0;
int	u_nMovePType = 0; // MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE

CtmWnd*			pwndBtn_MoldPType[3] = {NULL};
int MoldPTypeNo=0;
int	u_nMoldPType = 0; // MACHINE_FUNCTION_OPTIONS_MOLD_POSE

CtmWnd*			pwndBtn_MachineZero  = NULL; 
CtmWnd*			pwndBtn_MachineNext  = NULL; 
int	u_nMachineType = 0; // MACHINE_FUNCTION_OPTIONS_RSV01

long	Btn_bgc[] =
{
	0xD6BA,
	0xEF32,
};
long	Btn_fgc[] =
{
	//0x8CD1, //gray
	//0x4BAE,
	0x0000,
	0x0000,
};
		
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndButton_Param     = pwndSender->FindControlFromName("LinuxCtmToolButton1"); 
	pwndButton_SParam     = pwndSender->FindControlFromName("LinuxCtmToolButton2"); 
	pwndButton_Back     = pwndSender->FindControlFromName("Button_Para2");

	MovePTypeNo = GetSpecialControlNum(pwndSender, "MovePostType", "CtmToolButton", pwndBtn_MovePType);
	MoldPTypeNo = GetSpecialControlNum(pwndSender, "MoldPostType", "CtmToolButton", pwndBtn_MoldPType);
	pwndBtn_MachineZero = pwndSender->FindControlFromName("MachineZero");
	pwndBtn_MachineNext = pwndSender->FindControlFromName("MachineNext");
	
	// 刷新功能模式
	SetMovePType();
	SetMoldPType();
	SetMachineType();
		
	return TRUE;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{         
	printf("OnChangeA\n");
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
	if(pwnd ==pwndButton_SParam)
	{
		::PutCommand("SParam.txt");
	}
	if(pwnd ==pwndButton_Back)
	{
		//::PutCommand("Func.txt");
		::PutCommand("Index.txt"); // cjlee 2019/4/6 下午 04:00:27
	}
	
		for(int i = 0; i < MovePTypeNo; i++)
	{
		if(pwnd == pwndBtn_MovePType[i])
		{
			u_nMovePType = i;
			SetDBValue("MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE",u_nMovePType);
			SetMovePType();
		}
	}
		for(int i = 0; i < MoldPTypeNo; i++)
	{
		if(pwnd == pwndBtn_MoldPType[i])
		{
			u_nMoldPType = i;
			SetDBValue("MACHINE_FUNCTION_OPTIONS_MOLD_POSE",u_nMoldPType);
			SetMoldPType();
		}
	}
	
	if(pwnd == pwndBtn_MachineZero)
	{
		u_nMachineType = 0;
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV01",u_nMachineType);
		SetMachineType();
	}
	else if(pwnd == pwndBtn_MachineNext)
	{
		u_nMachineType = 1;
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV01",u_nMachineType);
		SetMachineType();
	}
	
  return wIDControl;	
}
/*---------------------------------------------------------------------------+
|  Function : OnMessage()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_MOUSELUP)
	{
		// 刷新功能模式
		SetMovePType();
		SetMoldPType();
		SetMachineType();
	}
}
void	OnUpdateA(CtmWnd* pwndSender)
{
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	SetMovePType()
{
	u_nMovePType = (int)GetDBValue("MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE").lValue;
	printf("MovePType=%d\n",u_nMovePType);
		for(int i = 0; i < MovePTypeNo; i++)
	{
		pwndBtn_MovePType[i]->SetPropValueT("bgc",Btn_bgc[OFF]); // 全部設為不選狀態
		pwndBtn_MovePType[i]->SetPropValueT("fgc",Btn_fgc[OFF]);
		pwndBtn_MovePType[i]->CreateA();
		pwndBtn_MovePType[i]->Update();
		if (i == u_nMovePType) // 顯示所選擇類型
		{
			pwndBtn_MovePType[i]->SetPropValueT("bgc",Btn_bgc[ON]);
			pwndBtn_MovePType[i]->SetPropValueT("fgc",Btn_fgc[ON]);
			pwndBtn_MovePType[i]->CreateA();
			pwndBtn_MovePType[i]->Update();
			((CtmButton*)pwndBtn_MovePType[i])->Press(tmBUTTON_DOWN);
		}
	}
}

void	SetMoldPType()
{
	u_nMoldPType = (int)GetDBValue("MACHINE_FUNCTION_OPTIONS_MOLD_POSE").lValue;
	printf("MoldPType=%d\n",u_nMoldPType);
			for(int i = 0; i < MoldPTypeNo; i++)
	{
		pwndBtn_MoldPType[i]->SetPropValueT("bgc",Btn_bgc[OFF]);
		pwndBtn_MoldPType[i]->SetPropValueT("fgc",Btn_fgc[OFF]);
		pwndBtn_MoldPType[i]->CreateA();
		pwndBtn_MoldPType[i]->Update();
		if (i == u_nMoldPType)
		{
			pwndBtn_MoldPType[i]->SetPropValueT("bgc",Btn_bgc[ON]);
			pwndBtn_MoldPType[i]->SetPropValueT("fgc",Btn_fgc[ON]);
			pwndBtn_MoldPType[i]->CreateA();
			pwndBtn_MoldPType[i]->Update();
			((CtmButton*)pwndBtn_MoldPType[i])->Press(tmBUTTON_DOWN);
		}
	}
}

void	SetMachineType()
{
	u_nMachineType = (int)GetDBValue("MACHINE_FUNCTION_OPTIONS_RSV01").lValue;
	printf("MachineType=%d\n",u_nMachineType);
	switch(u_nMachineType)
	{
		case 0:
			  if(pwndBtn_MachineZero!=NULL)
		  	{
					pwndBtn_MachineZero->SetPropValueT("bgc",Btn_bgc[ON]);
					pwndBtn_MachineZero->SetPropValueT("fgc",Btn_fgc[ON]);
				}
			  if(pwndBtn_MachineNext!=NULL)
		  	{
					pwndBtn_MachineNext->SetPropValueT("bgc",Btn_bgc[OFF]);
					pwndBtn_MachineNext->SetPropValueT("fgc",Btn_fgc[OFF]);
				}
			break;
		case 1:
			  if(pwndBtn_MachineZero!=NULL)
		  	{
	    		pwndBtn_MachineZero->SetPropValueT("bgc",Btn_bgc[OFF]);	
					pwndBtn_MachineZero->SetPropValueT("fgc",Btn_fgc[OFF]);
				}
			  if(pwndBtn_MachineNext!=NULL)
		  	{
					pwndBtn_MachineNext->SetPropValueT("bgc",Btn_bgc[ON]);
					pwndBtn_MachineNext->SetPropValueT("fgc",Btn_fgc[ON]);
				}
			break;
		default:
			break;
	}			
  if(pwndBtn_MachineZero!=NULL && pwndBtn_MachineNext!=NULL)
	{
		pwndBtn_MachineZero->CreateA();
		pwndBtn_MachineZero->Update();


		pwndBtn_MachineNext->CreateA();
		pwndBtn_MachineNext->Update();

		if(u_nMachineType == 0)
			((CtmButton*)pwndBtn_MachineZero)->Press(tmBUTTON_DOWN);
		else if(u_nMachineType == 1)
			((CtmButton*)pwndBtn_MachineNext)->Press(tmBUTTON_DOWN);
	}
}