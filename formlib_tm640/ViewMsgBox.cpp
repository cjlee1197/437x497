/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

#include	"ViewMsgBox.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/
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
CtmWnd*		u_pwndBtnCancel				= NULL;

WORD	u_wPickerOPSatus  = 0; // ����⪬�A
WORD	RunStopCmd=0xFFFF;	
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy��
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	
	g_bMsgBoxFlag = FALSE;
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnOK)
			g_bMsgBoxFlag = TRUE;
		else if(pwndTemp == u_pwndBtnCancel)
			g_bMsgBoxFlag = FALSE;
			
		Exit();
	}
	
	return wIDControl;
}
//20140624��
WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

		if(pwndTemp == u_pwndBtnOK)
			g_bMsgBoxFlag = TRUE;
		else if(pwndTemp == u_pwndBtnCancel)
			g_bMsgBoxFlag = FALSE;
			
		Exit();

	return wIDControl;
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//if (wKey == _ENTER) Exit();
	//else  ((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	//20140624��
	//Sunny<20080214><<<<< �W�[g_bMsgBoxFlag�O��Msgbox���X�ɬO�_key�U�T�{��
	//g_bMsgBoxFlag =FALSE;
	//if (wKey == _ENTER)
	//{
		//g_bMsgBoxFlag =TRUE;
		//Exit();
	//}
	//else if (wKey == 0x0E08/*_ESC/*_KEY_CLEAR*/)	//2012/7/17 �U�� 03:20:32  CAN == Backspace
	//{
		//g_bMsgBoxFlag =FALSE;
		//Exit();
	//}
	//else  ((CtmFormView*)pwndSender)->OnKey1(wKey);
	//20140624��
	
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