/*============================================================================+
|  Class    : ViewMessageBoxConfirm library                          			    |
|  Task     : ViewMessageBoxConfirm library action source file								|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxConfirm.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

//�U����taborder��
#define		INDEX_BTN_OK									0

#define		CONST_REQ_COMMAND   6
#define     STATE_SINGLESTEP        2
#define			STATE_MANUAL						3
#define     STATE_FULLAUTO          4 

#define			GPIO_KEY_BIT		0xb
#define			GPIO_KEY_TEACH	0x9	// �W
#define			GPIO_KEY_STOP 	0x8	// ��
#define			GPIO_KEY_AUTO 	0x2	// �U


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

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;

WORD	u_wPickerOPSatus  = 0; // ����⪬�A
WORD	RunStopCmd=0xFFFF;	
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{	
	u_pwndBtnOK = pwndSender->FindControlFromName("btnOK");
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if((pwndTemp == u_pwndBtnOK)&&(u_bEnterKey == TRUE))
		Exit();
	if(wIDControl==0) Exit();	
	return wIDControl; 
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnOK)
		Exit();

	return wIDControl;
}


WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	  
	((CtmFormView*)pwndSender)->OnKey1(wKey);		
	
	return wKey;
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
		{ /*-------------------------------����----------------------------------*/
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

		} /*-------------------------------����----------------------------------*/
	}
	if(message==MSG_GPIO_READ)
	{
		printf("Get MSG_GPIO_READ\n");
		// �����_��
		{
			char gpio_key;
			gpio_key = wParam&GPIO_KEY_BIT;
			printf("gpio_key = %d,wParam=%x\n",gpio_key,wParam);

			if(gpio_key==GPIO_KEY_TEACH) // �W
			{
				printf("KeyMode=MANUAL\n");
				KeyMode=MANUAL;
			}
			else if(gpio_key==GPIO_KEY_STOP) // ��
			{
				printf("KeyMode=STOP\n");
				KeyMode=STOP;
			}
			else if(gpio_key==GPIO_KEY_AUTO) // �U
			{
				printf("KeyMode=AUTO\n");
				KeyMode=AUTO;
			}
			
			if(KeyMode == MANUAL) // ���
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
				SendCommand(MODE_MANUAL);
			}
			else if(KeyMode == AUTO) // �۰� 
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
				SendCommand(MODE_AUTO);
			}
			else if(KeyMode == STOP) // ����
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
				SendCommand(MODE_NULL);//����
			}
		}
		Exit();
	}
	
	
//	if(message==MSG_GPIO_READ)
//	{
//		char temp;
//		if(wParam&0x4)
//			temp = 0;	
//		else return;
//		//printf("temp = %d\n",temp);
//		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
//	}
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL		RunFrist = TRUE;
	if(RunFrist)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		RunFrist = FALSE;	
	}
	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
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