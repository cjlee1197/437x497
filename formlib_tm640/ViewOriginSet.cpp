/*============================================================================+
|  Class    : ViewOriginSet library             	   	                  			|
|  Task     : ViewOriginSet library  header file  	  	                 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewOriginSet.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/
#define 		BTN_BRAKE  					0
#define 		BTN_COORECTION			1
#define 		BTN_ZERO   					2

#define 		COLOR_GRAY			   	0xC618
#define 		COLOR_BLUE			   	1046

#define		COORDINATETYPE_LOCK	  -1		//NO�y��
#define		COORDINATETYPE_J		0		//���`�y��
#define		SAVE_SWITCH_DELAY		30

#define		STATE_INITIAL				0		//��l�Ƴt��
#define		STATE_PLUS					1		//���UButton"+"
#define		STATE_MINUS					2		//���UButton"-"

#define	JOGMODE_N_LOCK				16
#define	JOGMODE_LOCK					63488

int color[] ={0x0023,0x0022,NULL};
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
BOOL		u_bEnterKey						= FALSE;



CtmWnd*			u_pwndBtnBKJoint[6]						= {NULL};		//���ӷ٨����s
int					u_nBtnBKJointCnt							=0;					//�b�����s�����`��
CtmWnd*			u_pwndBtnCorrection[6]				= {NULL};		//���ӷ٨����s
int					u_nBtnCorrectionCnt						=0;					//�b�����s�����`��
CtmWnd*			u_pwndBtnZero[6]							= {NULL};		//���ӷ٨����s
int					u_nBtnZeroCnt									=0;					//�b�����s�����`��
CtmWnd*			u_pwndEncoderValueTable[6]		={NULL};		//�s�X���ƭ�
int					u_nEncoderValueTableCnt				=0;					//�s�X���ƭȤ����`��
CtmWnd*			u_pwndStaticBrake[6]					= {NULL};		//�٨����A
int					u_nStaticBrakeCnt							=0;					//�b�����A�����`��


//�t�ױ����---------------------------------------
CtmWnd*		u_pwndStaticSpeedShow		= NULL;
CtmWnd*		u_pwndBtnPlus						= NULL;
CtmWnd*		u_pwndBtnMinus					= NULL;

//�k�貾�ʱ����-----------------------------------
CtmWnd*		u_pwndBtnCoordinateMM	= NULL;
CtmWnd*		u_pwndBtnPlusCTable[12]				= {NULL};		//�k�貾�ʱ���Ϫ�Button"+"
CtmWnd*		u_pwndBtnMinusCTable[12]			= {NULL};		//�k�貾�ʱ���Ϫ�Button"-"
CtmWnd*		u_pwndStaticCTable[12]				= {NULL};		//�k�貾�ʱ���Ϫ��y��

//��L����]�w��-----------------------------------
CtmWnd*		u_pwndBtnMoveMode			= NULL;
CtmWnd*		u_pwndSelectEditJog		= NULL;


int		u_nBtnPlusCTableCnt		=0;
int		u_nBtnMinusCTableCnt	=0;
int		u_nStaticCTableCnt		=0;


char				Two_BrakeStatus[6] = {'0','0','0','0','0','0'};
int					Ten_BrakeStatus = 0;
WORD				u_wDBvalueEncoder_StartNo		=0;		//����Ѽƽs���G�s�X���ƭ�J1

BOOL					BrakeLock					= FALSE;		//-1�N��L�ʰ��A0~5�N��1~6�b
BOOL					CorrectioLock			= FALSE;
BOOL					ZeroLock					= FALSE;

//CtmWnd*			u_pwndBtnPressBrake					=NULL;
//CtmWnd*			u_pwndBtnPressZero					=NULL;
//CtmWnd*			u_pwndBtnPressCorrection		=NULL;

BOOL		Press_Set_btn = FALSE;
BOOL		EditLock = FALSE; 
		
int u_nMoveControlType = COORDINATETYPE_J;
		
		
int u_Move_Mode = 0;
int			u_lSpeed							=0;
short		Press_Move_btn		=0;
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//����U���󪺫��w
	
	u_nBtnBKJointCnt					= GetSpecialControlNum(pwndSender, "BtnBKJ", "CtmToolButton", u_pwndBtnBKJoint);
	u_nBtnCorrectionCnt				= GetSpecialControlNum(pwndSender, "BtnCorrection", "CtmToolButton", u_pwndBtnCorrection);
	u_nBtnZeroCnt							= GetSpecialControlNum(pwndSender, "BtnZero", "CtmToolButton", u_pwndBtnZero);
	u_nEncoderValueTableCnt		= GetSpecialControlNum(pwndSender, "EditEncoderJ", "CtmEditX1", u_pwndEncoderValueTable);
	u_nStaticBrakeCnt		= GetSpecialControlNum(pwndSender, "StaticBrake", "CtmStaticX2", u_pwndStaticBrake);
		
	//JOG����
	
	u_pwndStaticSpeedShow	= pwndSender->FindControlFromName("StaticSpeedShow");
	u_pwndBtnPlus					= pwndSender->FindControlFromName("btnPlus");
	u_pwndBtnMinus				= pwndSender->FindControlFromName("btnMinus");	
	
	u_pwndBtnCoordinateMM	= pwndSender->FindControlFromName("BtnCoordinateMM");
	u_nBtnPlusCTableCnt		= GetSpecialControlNum(pwndSender, "btnPlusC", "CtmToolButton", u_pwndBtnPlusCTable);
	u_nBtnMinusCTableCnt	= GetSpecialControlNum(pwndSender, "btnMinusC", "CtmToolButton", u_pwndBtnMinusCTable);
	u_nStaticCTableCnt		= GetSpecialControlNum(pwndSender, "StaticC", "CtmStaticX2", u_pwndStaticCTable);
	
	u_pwndBtnMoveMode			= pwndSender->FindControlFromName("BtnMoveMode");
	u_pwndSelectEditJog		= pwndSender->FindControlFromName("SelectEditJog");

	
	if(g_TxCNCData.shmCNCData.CNC_jog_mode == 0)
		RobotStatus = OP_JOG_MODE;
	else if(g_TxCNCData.shmCNCData.CNC_jog_mode == 1)
		RobotStatus = OP_INCJOG_MODE;
	return	TRUE;
}

/*
WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	
	return	wIDControl;
}
*/

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		return	wIDControl;
	}
	for(int i = 0 ; i < cfg.jn ; i++)			//6�b
	{
		if(pwndTemp == u_pwndBtnBKJoint[i])
		{
			if(EditLock)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else if(BrakeLock)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_BRAKE"], tmFT_CODE_TECH);
			else
				SetSendNCDate(BTN_BRAKE,i);
			return	wIDControl;
		}		
		if(pwndTemp == u_pwndBtnCorrection[i])
		{
			if(EditLock)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else if(CorrectioLock)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_COORECTION"], tmFT_CODE_TECH);
			else
				SetSendNCDate(BTN_COORECTION,i);
			return	wIDControl;
		}		
		if(pwndTemp == u_pwndBtnZero[i])
		{
			if(EditLock)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else if(ZeroLock)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_ZERO"], tmFT_CODE_TECH);
			else
				SetSendNCDate(BTN_ZERO,i);
			return	wIDControl;
		}			
	}

	if(pwndTemp == u_pwndBtnPlus)		//Speed��
	{
		SetAndShowSpeed(STATE_PLUS);
	}
	else if(pwndTemp == u_pwndBtnMinus)		//Speed��
	{
		SetAndShowSpeed(STATE_MINUS);
	}
	else if(pwndTemp == u_pwndBtnCoordinateMM)
	{
		u_nMoveControlType+=1;
		if(u_nMoveControlType>=1) u_nMoveControlType=COORDINATETYPE_J; 
		UpdateMoveControlType(u_nMoveControlType);
		((CtmFormView*)pwndSender)->OnLoseFocus();
		
		if(u_nMoveControlType == COORDINATETYPE_J)
		{
			u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_JOINT");
			u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
			u_pwndBtnCoordinateMM->CreateA();
			u_pwndBtnCoordinateMM->Update();
		}
		else if (u_nMoveControlType == COORDINATETYPE_LOCK)
		{
			u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
			u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
			u_pwndBtnCoordinateMM->CreateA();
			u_pwndBtnCoordinateMM->Update();
		}
	}
	else if(pwndTemp == u_pwndBtnMoveMode)
	{
		u_Move_Mode =(!u_Move_Mode);		//0�P1�������ഫ
		char temp=u_Move_Mode;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_mode,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
		SetDBString("CNC_JOG_MODE",&temp,1);
		if(g_TxCNCData.shmCNCData.CNC_jog_mode == 0)
			RobotStatus = OP_JOG_MODE;
		else if(g_TxCNCData.shmCNCData.CNC_jog_mode == 1)
			RobotStatus = OP_INCJOG_MODE;
		if(u_Move_Mode == 0)
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_CONTINUE");
		}
		
		u_pwndSelectEditJog->SetPropValueT("wtype",color[u_Move_Mode]);
		u_pwndSelectEditJog->SetPropValueT("taborder", 34*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
		u_pwndSelectEditJog->UpdateAll();
		if(u_Move_Mode == 1)	
		{
			((CtmFormView*)pwndSender)->Goto(34);		//�T�ʶq�U�Կ��
			u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_JOG");
		}	
		u_pwndBtnMoveMode->CreateA();
		u_pwndBtnMoveMode->Update();
		
	}
	return	wIDControl;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	for(int i =0; i<u_nBtnPlusCTableCnt; i++)
	{
		if(pwndTemp == u_pwndBtnPlusCTable[i])		//"+"
		{
			if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_JOG_MODE || g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_INCJOG_MODE)
			{
				Press_Move_btn = i+1;
				SetJogMoveDate(u_nMoveControlType, STATE_PLUS, i);			//�ק�to CNC���
			}
			else if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);
			return wIDControl;
		}
	}
	for(int i =0; i<u_nBtnMinusCTableCnt; i++)
	{
		if(pwndTemp == u_pwndBtnMinusCTable[i])		//"-"
		{	
			if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_JOG_MODE || g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_INCJOG_MODE)
			{
				Press_Move_btn = i+1;
				SetJogMoveDate(u_nMoveControlType, STATE_MINUS, i);			//�ק�to CNC���
			}
			else if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);
			return wIDControl;
		}
	}
	return wIDControl;
}

void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_MOUSELUP)
	{
		//if(Press_Move_btn >0)
			//RestJogMoveDate();
	}
	else if(message==MSG_PANEL_KEY)
	{
		char keycode;
		keycode = wParam;
		//printf("panel key = %x\n",keycode);
		if(keycode>0x80 && Press_Move_btn ==0 && g_TxCNCData.shmCNCData.CNC_servo_on == 1)
		{
			keycode = keycode-0x80;
			if(keycode>= 0x40) return ;	//�׶}Start��
			keycode = keycode-1;	//�׶}Pause / Stop��
			int Joint = keycode&0xF;
			if(Joint==0) return; 
			int N = (keycode & 0x10)/16;
			if(N==0) N=2;
			if(Press_Move_btn!=0) return;
			Press_Move_btn = Joint;
			//printf("key down , joint is %d , N = %d\n",Joint,N);
			SetJogMoveDate(u_nMoveControlType, N, Joint-1);			//�ק�to CNC���
		}
		else if(keycode<0x80/* && Press_Move_btn*/)
		{
			keycode = keycode;
			if(keycode>= 0x40) return ;	//�׶}Start��
			keycode = keycode-1;	//�׶}Pause / Stop��
			int Joint = keycode&0xF;
			if(Joint==0) return; 
			int N = (keycode & 0x10)/16;
			if(N==0) N=2;
			if(Press_Move_btn!=Joint) return;
			Press_Move_btn = 0;
			//printf("key up , joint is %d , N = %d\n",Joint,N);
			RestJogMoveDate();
		}
	}
	else	if(message==MSG_GPIO_READ)
	{
		//printf("CtmGpio437Hmi ReadGpio=%x\n",wParam);
		char temp;
		if(wParam&0x4 || (g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_JOG_MODE && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_INCJOG_MODE && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_HOME_MODE))
		{
			RestJogMoveDate();
			Press_Move_btn = 0;
			temp = 0;
		}
		else if(!(wParam&0x4)/* && g_TxCNCData.shmCNCData.CNC_servo_on==0*/)
		{
			if(g_iPrivilege < 3)
			{
				Prompt(g_MultiLanguage["ROBOT_STR_PRIVILEGE_NO_ENOUGH"],1);
				return ;
			}
			temp = 1;
		}
		else return;
		//printf("temp = %d\n",temp);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
	}
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static int FristIne = 0;
	static char now_brake_status[6]={'0','0','0','0','0','0'}; 
	static int	old_CNC_barke_status = -1;
	if(FristIne==0)
	{
		FristIne++;
		old_CNC_barke_status = g_pRxCNCData->CNC_barke_status;
		int temp_barke_status = old_CNC_barke_status;
		for(int i = (cfg.jn-1) ; i >=0 ; i--)
		{
			int temp = pow(2,(double)i);
			if(temp_barke_status >= temp)
			{
				temp_barke_status-=temp;
				int temp_joint = i-(cfg.jn-1);
				if(temp_joint < 0) temp_joint = temp_joint*-1;
				now_brake_status[temp_joint] = 1;
				u_pwndStaticBrake[i]->SetPropValueT("textID","ROBOT_BTN_BRAKES_ON");
				u_pwndStaticBrake[i]->SetPropValueT("fgc",(double)0x0000);
				u_pwndStaticBrake[i]->Update();
			}
			else
			{
				u_pwndStaticBrake[i]->SetPropValueT("textID","ROBOT_BTN_BRAKES_OFF");
				u_pwndStaticBrake[i]->SetPropValueT("fgc",0xF800);
				u_pwndStaticBrake[i]->Update();
			}
		}
		ini(pwndSender);
		((CtmFormView*)pwndSender)->OnLoseFocus();

		for(int i=0 ; i<cfg.jn ; i++)
		{
			if(g_TxCNCData.shmCNCData.PARAM.maker_param[21+i]==0)
			{
				u_pwndStaticBrake[i]->SetPropValueT("textID","ROBOT_STR_NO_BRAKE");
				u_pwndStaticBrake[i]->SetPropValueT("fgc",(double)0x0000);
				u_pwndStaticBrake[i]->Update();
				u_pwndBtnBKJoint[i]->SetPropValueT("taborder",-2);
				u_pwndBtnBKJoint[i]->SetPropValueT("bgc",COLOR_GRAY);
				u_pwndBtnBKJoint[i]->Update();
			}
			if(g_TxCNCData.shmCNCData.PARAM.maker_param[27+i]==0)
			{
				u_pwndBtnZero[i]->SetPropValueT("taborder",-2);
				u_pwndBtnZero[i]->SetPropValueT("bgc",COLOR_GRAY);
				u_pwndBtnZero[i]->Update();
			}
		}
		
	}
	if(FristIne<5) FristIne++;
	if(FristIne==3)
	{
		FristIne++;
		MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ORIGIN_ALARM"], tmFT_CODE_TECH);
	}
	UpdateEncoderValue();
		
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE && !EditLock) 
		EditLock = TRUE;
	else if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE && EditLock)
		EditLock = FALSE;

	if(g_pRxCNCData->CNC_pos_return_calibrate == g_TxCNCData.shmCNCData.CNC_pos_calibrate && g_TxCNCData.shmCNCData.CNC_pos_calibrate!=0)
	{
		int joint = (int)g_pRxCNCData->CNC_pos_return_calibrate;
		joint-=1;
		if (joint >= 0 && joint <= (cfg.jn-1))
		{
			u_pwndBtnCorrection[joint]->SetPropValueT("taborder",joint+BTN_COORECTION*6);
			u_pwndBtnCorrection[joint]->SetPropValueT("bgc",COLOR_BLUE);
			u_pwndBtnCorrection[joint]->Update();
			char temp = 0;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_pos_calibrate,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_pos_calibrate));
		}
		CorrectioLock = FALSE;
	}
	if(g_TxCNCData.shmCNCData.CNC_jog_inc != ((CtmSelectEdit*)u_pwndSelectEditJog)->GetIndex()+1)
	{
		int temp = ((CtmSelectEdit*)u_pwndSelectEditJog)->GetIndex()+1;
		SetDBValue("CNC_JOG_INC", temp);
	}
	
	if(g_pRxCNCData->CNC_pos_return_zero == g_TxCNCData.shmCNCData.CNC_pos_zero && g_TxCNCData.shmCNCData.CNC_pos_zero!=0)
	{
		int joint = (int)g_pRxCNCData->CNC_pos_return_zero;
		joint-=1;
		if (joint >= 0 && joint <= (cfg.jn-1))
		{
			u_pwndBtnZero[joint]->SetPropValueT("taborder",joint+BTN_ZERO*6);
			u_pwndBtnZero[joint]->SetPropValueT("bgc",COLOR_BLUE);
			u_pwndBtnZero[joint]->Update();
			char temp = 0;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_pos_zero,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_pos_zero));
		}
		ZeroLock = FALSE;
	}
	
	if(g_pRxCNCData->CNC_barke_status !=  old_CNC_barke_status)
	{
		int 	temp_barke_status = g_pRxCNCData->CNC_barke_status;
		for(int i = (cfg.jn-1) ; i >=0 ; i--)
		{
			int temp = pow(2,(double)i);
			if(temp_barke_status >= temp)
			{
				temp_barke_status-=temp;
				int temp_joint = i-(cfg.jn-1);
				if(temp_joint < 0) temp_joint = temp_joint*-1;
				if(now_brake_status[temp_joint] == 0)
				{
					now_brake_status[temp_joint] = 1;
					u_pwndStaticBrake[i]->SetPropValueT("textID","ROBOT_BTN_BRAKES_ON");
					u_pwndStaticBrake[i]->SetPropValueT("fgc",(double)0x0000);
					u_pwndStaticBrake[i]->Update();
					u_pwndBtnBKJoint[i]->SetPropValueT("taborder",BTN_BRAKE*6+i);
					u_pwndBtnBKJoint[i]->SetPropValueT("bgc",COLOR_BLUE);
					u_pwndBtnBKJoint[i]->Update();
				}
			}
			else
			{
				int temp_joint = i-(cfg.jn-1);
				if(temp_joint < 0) temp_joint = temp_joint*-1;
				if(now_brake_status[temp_joint] == 1)
				{
					now_brake_status[temp_joint] = 0;
					u_pwndStaticBrake[i]->SetPropValueT("textID","ROBOT_BTN_BRAKES_OFF");
					u_pwndStaticBrake[i]->SetPropValueT("fgc",0xF800);
					u_pwndStaticBrake[i]->Update();
					u_pwndBtnBKJoint[i]->SetPropValueT("taborder",BTN_BRAKE*6+i);
					u_pwndBtnBKJoint[i]->SetPropValueT("bgc",COLOR_BLUE);
					u_pwndBtnBKJoint[i]->Update();
				}
			}
		}
		old_CNC_barke_status = g_pRxCNCData->CNC_barke_status;
	}
}
/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/

void		UpdateEncoderValue()						//��s�s�X���ƭ�
{
	for(int i=0 ; i < cfg.jn ; i++)
	{
		//printf("g_pRxCNCData->CNC_encoder_value[%d] = %d\n",i,g_pRxCNCData->CNC_encoder_value[i]);
			u_pwndEncoderValueTable[i]->SetPropValueT("value",g_pRxCNCData->CNC_encoder_value[i]);
			u_pwndEncoderValueTable[i]->Update();
	}
}


void		SetSendNCDate(int Type, int Joint)
{
	if(Type == BTN_BRAKE)
	{
		int temp_Joint = Joint-5;
		if(temp_Joint < 0) temp_Joint = temp_Joint*-1;
		if(Two_BrakeStatus[temp_Joint] == '0') Two_BrakeStatus[temp_Joint] = '1';
		else 	Two_BrakeStatus[temp_Joint] = '0';
		char * temp = NULL;
		Ten_BrakeStatus = strtol(Two_BrakeStatus,&temp,2);
		u_pwndBtnBKJoint[Joint]->SetPropValueT("taborder",-2);
		u_pwndBtnBKJoint[Joint]->SetPropValueT("bgc",COLOR_GRAY);
		u_pwndBtnBKJoint[Joint]->Update();
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_brake_release,&Ten_BrakeStatus,sizeof(g_TxCNCData.shmCNCData.CNC_brake_release));
	}
	else if(Type == BTN_COORECTION)
	{
		CorrectioLock = TRUE;
		u_pwndBtnCorrection[Joint]->SetPropValueT("taborder",-2);
		u_pwndBtnCorrection[Joint]->SetPropValueT("bgc",COLOR_GRAY);
		u_pwndBtnCorrection[Joint]->Update();
		int temp = Joint+1;
		char temp2 = temp;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_pos_calibrate,&temp2,sizeof(g_TxCNCData.shmCNCData.CNC_pos_calibrate));
		printf("g_TxCNCData.shmCNCData.CNC_pos_calibrate = %d\n",g_TxCNCData.shmCNCData.CNC_pos_calibrate);
	}
	else if(Type == BTN_ZERO)
	{
		ZeroLock = TRUE;
		u_pwndBtnZero[Joint]->SetPropValueT("taborder",-2);
		u_pwndBtnZero[Joint]->SetPropValueT("bgc",COLOR_GRAY);
		u_pwndBtnZero[Joint]->Update();
		
		int temp = Joint+1;
		char temp2;
		temp2 = temp;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_pos_zero,&temp2,sizeof(g_TxCNCData.shmCNCData.CNC_pos_zero));
	}
}



void		SetAndShowSpeed(int nType)		//�]�w������u�����ʳt��
{
	if(u_pwndStaticSpeedShow != NULL)
	{
		char	temp[15] 		= "\0";
		char	szTemp[15]		= "\0";
		memset(temp, 0, sizeof(temp));
		memset(szTemp, 0, sizeof(szTemp));
		
		//����ѼƦC������A"MAKER_PARAM1"->���ʳt�ת�����Ѽ�(������u)
		u_lSpeed = GetDBValue("CNC_JOG_OVER_CMD").lValue;
		
		//nType����SPEED_INITIAL��u_lSpeed�����B�z
		if(nType == STATE_PLUS)		//+
		{
			u_lSpeed+=10;
			if(u_lSpeed>100)		//MAX =100%
			{
				u_lSpeed = 100;
				return;
			}
		}
		else if(nType == STATE_MINUS)		//-
		{
			u_lSpeed-=10;
			if((u_lSpeed<0)||(u_lSpeed>100))		//MIN =0%
			{
				u_lSpeed =0;
				return;
			}
		}
		//����ѼƦC������A"MAKER_PARAM1"->���ʳt�ת�����Ѽ�(������u)
		SetDBValue("CNC_JOG_OVER_CMD", u_lSpeed);
		sprintf(temp, "%d%s", u_lSpeed, "%");
		CodeChange(szTemp, temp);
		u_pwndStaticSpeedShow->SetPropValueT("text", szTemp);
		u_pwndStaticSpeedShow->Update();
	}
}

void		UpdateMoveControlType(int nCoordinateType)		//�󴫥k�貾�ʱ���Ϫ��y�мҦ�
{
	if(nCoordinateType == COORDINATETYPE_J)		//���`�y��
	{
		for(int i =0; i<u_nStaticCTableCnt; i++)
		{
			if(u_pwndStaticCTable[i] != NULL)
			{
				u_pwndStaticCTable[i]->SetPropValueT("textID", cfg.j_sid[i]/*u_pszStrCoordinateJ[i]*/);
				u_pwndStaticCTable[i]->Update();
			}
		}
	}
	else if(nCoordinateType == -1)		//�L�y��
	{
		for(int i =0; i<u_nStaticCTableCnt; i++)
		{
			if(u_pwndStaticCTable[i] != NULL)
			{
				u_pwndStaticCTable[i]->SetPropValueT("textID", "ROBOT_STR_DUMMY");
				u_pwndStaticCTable[i]->Update();
			}
		}
	}
}


void		ini(CtmWnd* pwndSender)
{
/*---------------�t�ת�l��----------------*/
	SetAndShowSpeed(STATE_INITIAL);
/*---------------�s��/�T�� �P �T�ʶq-------------*/
	u_Move_Mode = (int)g_TxCNCData.shmCNCData.CNC_jog_mode;
	
	if(u_Move_Mode == 0)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_CONTINUE");
	}
	
	u_pwndSelectEditJog->SetPropValueT("wtype",color[u_Move_Mode]);
	u_pwndSelectEditJog->SetPropValueT("taborder", 34*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
	u_pwndSelectEditJog->UpdateAll();
	if(u_Move_Mode == 1)	
	{
		((CtmFormView*)pwndSender)->Goto(34);
		u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_JOG");
	}	
	u_pwndBtnMoveMode->CreateA();
	u_pwndBtnMoveMode->Update();
	
	int temp = GetDBValue("CNC_JOG_INC").lValue-1;
	((CtmSelectEdit*)u_pwndSelectEditJog)->SetIndex(temp);
	u_pwndSelectEditJog->Update();
		
/*-----------------������u�d��/���`----------------------*/
	if(u_nMoveControlType == 0)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_JOINT");
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
	}
	else if(u_nMoveControlType == -1)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
	}
	u_pwndBtnCoordinateMM->CreateA();
	u_pwndBtnCoordinateMM->Update();
	UpdateMoveControlType(u_nMoveControlType);
	
}


void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum)
{
	char temp = 9;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
	if(nBtnType == STATE_PLUS)
	{
		int temp = nAxisNum+1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
	}
	else if(nBtnType == STATE_MINUS)
	{
		int temp = -nAxisNum-1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
	}	
}

void		RestJogMoveDate()
{
	char temp_CHAT = 0;
	int temp_INT = 0;
	Press_Move_btn = 0;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp_INT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
	
}