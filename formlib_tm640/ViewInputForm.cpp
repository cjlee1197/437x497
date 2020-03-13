/*===========================================================================+
|  Class    : ViewClmp library                                          	 |
|  Task     : ViewClmp library action source file                       	 |
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
#include	"ViewInputForm.h"

/*========================================================+
|						Constants																			|
+========================================================*/

#define		KEY_BTNCLOSE		0xA600
//#defien		KEY_
#define		CONST_REQ_COMMAND   6
#define     STATE_SINGLESTEP        2
#define			STATE_MANUAL						3
#define     STATE_FULLAUTO          4 

#define			GPIO_KEY_BIT		0xb
#define			GPIO_KEY_TEACH	0x9	// 上
#define			GPIO_KEY_STOP 	0x8	// 中
#define			GPIO_KEY_AUTO 	0x2	// 下


#define			MANUAL	0
#define			AUTO	1
#define			STOP	2

#define		MODE_NULL						0xF000
#define		MODE_HOMING					0xF100
#define		MODE_MOTORENABLE		0xF200
#define		MODE_MANUAL					0xF300
#define		MODE_SINGLESTEP			0xF400
#define		MODE_AUTO						0xF500

/*========================================================+
|						Global Variable																|
+========================================================*/
CtmWnd*		u_pWndInput	= NULL;
CtmWnd*		u_pWndEnter	= NULL;
//CtmWnd*		u_pWndBackSpace = NULL;
CtmWnd*		u_pWndMax	= NULL;
CtmWnd*		u_pWndMin	= NULL;
CtmWnd*		u_pWndTitle = NULL;

BOOL			u_bFirstEnter = TRUE;
DWORD			u_dwBgc				= 0xFFFF;
long long Old_Value 		= 0;
char 	old_sz[32];

WORD	u_wPickerOPSatus  = 0; // 機械手狀態
WORD	RunStopCmd=0xFFFF;
/*========================================================+
|						Function																			|
+========================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	u_pWndInput 	= pwndSender->FindControlFromName("inputValue");
	u_pWndEnter		= pwndSender->FindControlFromName("btnEnter");
	//u_pWndBackSpace = pwndSender->FindControlFromName("btnBackSpace");
	u_pWndMax		= pwndSender->FindControlFromName("editMaxValue");
	u_pWndMin		= pwndSender->FindControlFromName("editMinValue");
	u_pWndTitle		= pwndSender->FindControlFromName("staticTitle");
	//if (u_pWndBackSpace != NULL)
	//	((CtmToolButton*)u_pWndBackSpace)->Press(tmBUTTON_UP);
	
	return TRUE;
}

void	OnShowA(CtmWnd* pwnd)
{
	if (g_pWndInput == NULL)
		return;
		
	char 	sz[32];
	long long	lValue = 0, lPrecision = 0, lMax = 0, lMin = 0;
	memset(sz, 0, sizeof(sz));
	//g_pWndInput->GetPropValueT("value", &lValue, sizeof(lValue));	
	//g_pWndInput->GetPropValueT("precision", &lPrecision, sizeof(lPrecision));
	g_pWndInput->GetPropValueT("max", &lMax, sizeof(lMax));
	g_pWndInput->GetPropValueT("min", &lMin, sizeof(lMin));
	g_pWndInput->GetPropValueT("bgc", 	&u_dwBgc, sizeof(u_dwBgc));
	g_pWndInput->GetPropValueT("promptID", sz, sizeof(sz));
	
	g_pWndInput->SetPropValueT("bgc", 52833); 
	g_pWndInput->Update();
	
	/*
	if (u_pWndInput != NULL)
	{
		u_pWndInput->SetPropValueT("precision", lPrecision);
		u_pWndInput->SetPropValueT("value", lValue);
	}
	*/
	
	if (u_pWndTitle != NULL)
	{
		u_pWndTitle->SetPropValueT("textID", sz);
		u_pWndTitle->Update();
	}
	if (g_pWndInput->Is("CtmEditX1") || g_pWndInput->Is("CtmEditX2"))
	{
		//g_pWndInput->GetPropValueT("value", &Old_Value, sizeof(Old_Value));	
		g_pWndInput->GetPropValueT("value", &lValue, sizeof(lValue));
		Old_Value=lValue;
		g_pWndInput->GetPropValueT("precision", &lPrecision, sizeof(lPrecision));
	}
	g_pWndInput->GetPropValueT("text", old_sz, sizeof(old_sz));
	g_pWndInput->GetPropValueT("text", sz, sizeof(sz));
	
	if(u_pWndMax != NULL)
	{
		u_pWndMax->SetPropValueT("precision", lPrecision);
		u_pWndMax->SetPropValueT("value", lMax);
		u_pWndMax->Update();
	}
	
	if(u_pWndMin != NULL)
	{
		u_pWndMin->SetPropValueT("precision", lPrecision);
		u_pWndMin->SetPropValueT("value", lMin);
		u_pWndMin->Update();
	}
	
	if (u_pWndInput == NULL)
		return;
	
	if (u_pWndInput->Is("CtmEditX1") || u_pWndInput->Is("CtmEditX2"))
	{
		u_pWndInput->SetPropValueT("value", lValue);
		u_pWndInput->SetPropValueT("precision", lPrecision);
	}
	else
	{
		u_pWndInput->SetPropValueT("text", sz);
	}
	
	u_pWndInput->Update();
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	return wIDControl;
}

void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_PANEL_KEY) // hotkey
	{
		printf("Get MSG_PANEL_KEY\n");
		char keycode;
		keycode = wParam;
		printf("keycode=%x\n",keycode);
		if((keycode == 0x81) || (keycode == 0x01))
		{ /*-------------------------------停止----------------------------------*/
			if(keycode == 0x81)	//Stop down
			{
				RunStopCmd = 0xF701;
			}
			else if(keycode == 0x01) // Stop up
			{
				RunStopCmd = 0xFF70;
			}
			printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);
  		if((g_ptaskpicker !=NULL) && (u_wPickerOPSatus != STATE_MANUAL))
			{
				SendCommand(RunStopCmd);
			}

		} /*-------------------------------停止----------------------------------*/
	}
	if(message==MSG_GPIO_READ)
	{
		printf("Get MSG_GPIO_READ\n");
		// 旋轉鑰匙
		{
			char gpio_key;
			gpio_key = wParam&GPIO_KEY_BIT;
			printf("gpio_key = %d,wParam=%x\n",gpio_key,wParam);

			if(gpio_key==GPIO_KEY_TEACH) // 上
			{
				printf("KeyMode=MANUAL\n");
				KeyMode=MANUAL;
			}
			else if(gpio_key==GPIO_KEY_STOP) // 中
			{
				printf("KeyMode=STOP\n");
				KeyMode=STOP;
			}
			else if(gpio_key==GPIO_KEY_AUTO) // 下
			{
				printf("KeyMode=AUTO\n");
				KeyMode=AUTO;
			}
			
			if(KeyMode == MANUAL) // 手動
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
				SendCommand(MODE_MANUAL);
			}
			else if(KeyMode == AUTO) // 自動 
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
				SendCommand(MODE_AUTO);
			}
			else if(KeyMode == STOP) // 停止
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
				SendCommand(MODE_NULL);//停止
			}
		if(gpio_key>0)
			ExitInputForm();
		}
	}
}


void	OnUpdateA(CtmWnd* pwndSender)
{

}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
	//printf("viewinputform get key %x\n", wKey);
	char pszValue[32];
	long lValue = 0;
	
	memset(pszValue, 0, sizeof(pszValue));
	
	//int	nTab = ((CtmFormView*)pwndSender)->Get_TabStop();
	//CtmWnd*	pwnd = pwndSender->FindControlFromTab(nTab);
	
	if (wKey == _ENTER)
	{
		ExitInputForm();
		_PutKeyTail(_ENTER);
	}
	
	if (wKey == _EXT_ALT_ENTER || wKey == _KEY_CLEAR)
	{
		char	sz[256];
		g_pWndInput->GetPropValueT("dbid0", sz, sizeof(sz));	
		if (sz[0] !='\0' || u_pWndInput->Is("CtmEditX1") || u_pWndInput->Is("CtmEditX2"))
			g_pWndInput->SetPropValueT("value", Old_Value);
		else if(u_pWndInput->Is("CtmEdit"))
			g_pWndInput->SetPropValueT("text", old_sz);
		g_pWndInput->SetPropValueT("bgc", u_dwBgc);
		g_pWndInput->Update();
		Exit();
	}
	
	if (g_pWndInput != NULL && u_pWndInput != NULL && wKey != _EXT_ALT_ENTER)
	{
		g_pWndInput->OnKey(wKey);
		g_pWndInput->Show();
			
		if (g_pWndInput->Is("CtmEditX1") || g_pWndInput->Is("CtmEditX2"))
		{
			g_pWndInput->GetPropValueT("value", &lValue, sizeof(lValue));
		}
		g_pWndInput->GetPropValueT("text", pszValue, sizeof(pszValue));
		
		
		if (u_pWndInput->Is("CtmEditX1") || u_pWndInput->Is("CtmEditX2"))
		{
			u_pWndInput->SetPropValueT("value", lValue);
		}
		else
		{
			//printf("set text: %s\n", pszValue);
			u_pWndInput->SetPropValueT("text", pszValue);
		}
		u_pWndInput->Update();
	}
	
	if (pwndSender->Is("CtmFormView"))
		return _NULL_KEY;//return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else
		return _NULL_KEY;
}


void		ExitInputForm()
{
	//printf("exit input form\n");
	if (g_pWndInput != NULL)
	{
		char	sz[256];
		memset(sz, 0, sizeof(sz));
		long long llValue = 0;
		
		g_pWndInput->GetPropValueT("dbid0", sz, sizeof(sz));	
		g_pWndInput->SetPropValueT("bgc", u_dwBgc);
		if (g_pWndInput->GetPropValueT("value", &llValue, sizeof(llValue)) && sz[0] !='\0')
		{
			int	spepre	= 0;
			
			
			long long llMin = 0;
			long long llMax = 0;
			if (g_pWndInput->GetPropValueT("min", &llMin, sizeof(llMin)) &&
				g_pWndInput->GetPropValueT("max", &llMax, sizeof(llMax)))
			{
				BOOL	bSave = FALSE;
				
				
				if (llMin == 0xFFFF)
				{
					if (llValue <= llMax) 
						bSave = TRUE;
				}
				else
				{
					if ((llValue >= llMin) && (llValue <= llMax))
						bSave = TRUE;
				}
		
				if (g_pWndInput->GetPropValueT("spepre", &spepre, sizeof(spepre)))
					llValue = llValue * Power(10, spepre);
				
				if (bSave)	SetViewDBValue(sz, llValue);
				
			}
			else	
			{
				if (g_pWndInput->GetPropValueT("spepre", &spepre, sizeof(spepre)))
					llValue = llValue * Power(10, spepre);
				
				SetViewDBValue(sz, llValue);
			}
		}
		else if(g_pWndInput->GetPropValueT("value", &llValue, sizeof(llValue)))
		{
			if (g_pWndInput->Is("CtmEditX1") || g_pWndInput->Is("CtmEditX2"))
			{
				long long llMin = 0;
				long long llMax = 0;
				if (g_pWndInput->GetPropValueT("min", &llMin, sizeof(llMin)) &&
					g_pWndInput->GetPropValueT("max", &llMax, sizeof(llMax)))
				{
					BOOL	bSave = FALSE;
					if (llMin == 0xFFFF)
					{
						if (llValue <= llMax) 
							bSave = TRUE;
					}
					else
					{
						if ((llValue >= llMin) && (llValue <= llMax))
							bSave = TRUE;
					}
					if (!bSave)	g_pWndInput->SetPropValueT("value",Old_Value);
				}
			}
		}
		g_pWndInput->Update();
	}
	Exit();
}


/*========================================================+
|						Helper																				|
+========================================================*/

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
