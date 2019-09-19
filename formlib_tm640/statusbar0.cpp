/*===========================================================================+
|  Class    : statusbar0 library                                             |
|  Task     : statusbar0 library action source file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                               		     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                  		   |
|  Revision :           			                                      			   |
+===========================================================================*/
#include	"statusbar0.h"
#include	"../commonaction.h"
#include	"../main.h"
#include	"../tmconfig.h"
#include    "../utils.h"
#include 	"../tmshmsg.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define 		CNT_INT_DEMO_TIMER 		2000;//1000; 50
#define 		GPIO_LED_DEFINE  		0x07;//1000; 50
#define 		GPIO_BIT_AUTO 0x01
#define 		GPIO_BIT_ALARM 0x02
#define 		GPIO_BIT_ENABLE 0x03

#define			GPIO_KEY_BIT	0xb
#define			GPIO_KEY_TEACH 0x8 //cjlee 
#define			GPIO_KEY_STOP 0x9 // 暫時修改模式 2018/12/14 下午 04:09:04
#define			GPIO_KEY_AUTO 0x2

#define			GPIO_SAFEKEY_BIT	0x14 //cjlee 
#define			GPIO_SAFEKEY_0 0x4
#define			GPIO_SAFEKEY_1 0x10
#define			GPIO_SAFEKEY_2 0x14
#define			MANUAL_CONTINUE_MODE	0
#define			MANUAL_JOG_MODE	1
#define			MANUAL_STOP_MODE	2

#define			NOT_READY	-1
#define			MANUAL	0
#define			AUTO	1
#define			REMOTE	2
#define			SINGLE_PAUSE 0x22 // 狀態 單步暫停 
#define			AUTO_PAUSE 0x24 // 狀態 自動暫停

#define 		CNT_INT_DEMO_TIMER 		2000;//1000; 50
#define 		GPIO_LED_DEFINE  		0x07;//1000; 50
#define 		GPIO_BIT_AUTO 0x01
#define 		GPIO_BIT_ALARM 0x02
#define 		GPIO_BIT_ENABLE 0x03

const			int	CNT_INT_UPDATE_DATE_TIMER = 1000;	//Delay time.

#define		CONST_REQ_COMMAND   6
		
#define		COMMAND_X_FWD				0xFF01
#define		COMMAND_X_BACK			0xFF02
#define		COMMAND_Y_FWD				0xFF03
#define		COMMAND_Y_BACK			0xFF04
#define		COMMAND_Z_FWD				0xFF05
#define		COMMAND_Z_BACK			0xFF06	
#define		COMMAND_C_FWD				0xFF07
#define		COMMAND_C_BACK			0xFF08
#define		COMMAND_X2_FWD			0xFF2E
#define		COMMAND_X2_BACK			0xFF2F	
#define		COMMAND_Y2_FWD			0xFF30
#define		COMMAND_Y2_BACK			0xFF31	
//#define		COMMAND_Y2_FWD			0xFF0D
//#define		COMMAND_Y2_BACK			0xFF0E
#define		COMMAND_STOP				0xFFFF
#define		COMMAND_X1SLOWDOWN	0xFF41
#define		COMMAND_Y1SLOWDOWN	0xFF42
#define		COMMAND_ZSLOWDOWN		0xFF43
#define		COMMAND_X2SLOWDOWN	0xFF44
#define		COMMAND_Y2SLOWDOWN	0xFF45

#define		MODE_NULL						0xF000
#define		MODE_HOMING					0xF100
#define		MODE_MOTORENABLE		0xF200
#define		MODE_MANUAL					0xF300
#define		MODE_SINGLESTEP			0xF400
#define		MODE_AUTO						0xF500

#define     STATE_SINGLESTEP        2
#define			STATE_HAND							3
#define     STATE_FULLAUTO          4 
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
char* pWhichAxisDB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_TYPE"; // 偵測哪一軸 DB

WORD			u_wError		= FALSE;					// control.cpp
//<< autodemo
extern	int     g_nDemo;									//	標記Demo，0為在自動Demo狀態，1為在非Demo狀態    
static  int     u_nKeyIndex		= 0;
BOOL			u_bPCLinkFlag	= FALSE;
BOOL			u_bRemote		= FALSE;
BOOL			u_biNet			= FALSE;
int				work_coord_no = -1;
int				tool_coord_no = -1;

long      lSpeedValueOld=0; //2019/5/9 下午 09:25:00
long      lStatusValueOld=7; //2019/5/11 下午 08:56:12

WORD			u_wGestureStatus = 0;
WORD			  u_wPickerOPSatus  = 0;

WORD        u_wKeys[] = {           //  PanelMap  //Sunny<20060918> add
//_F2_Offset_Work
    _F2,
    _F3,
    _F4,
    _F5,
    _F1,
    _F1,
//_F3_Offset_Tool
    _F3,
    _F3,
    _F4,
    _F5,
    _F1,
//_F4_System
    _F4, 
    _F3,
  	//_F4_F3_Para
    _F3,
    _F4,
    _F5,
    _F6,
    _F1,
    //_F4_F4_MLC
    _F4,
    _F3,
    _F4,
    _F5,
    _F6,
    _F7,
    _F8,
    _F9,
    _F10,
    _F1,
    _F5,
    _F6,
    _F7,
    _F1,
	//_F5_ProgManage
    _F5,
    _F3,
    _F4,
    _F1,
//_F6_Alarm
    _F6,
    _F3,
    _F4,
    _F5,
    _F1,
//_F7_Tune
    _F7,
    _F3,
    _F1,
//_F8
	_F8,
    _F3,
    _F1,
//_F9
	_F9,
//_F10
	_F10,
//Change Language
//	_F4,
//	_F7,
//	_KEY_RIGHTFIELD,
//	46,//_DOT
//	_KEY_DOWNFIELD,
//	46,//_DOT
//	_ENTER,

    _F1,
	_NULL_KEY    
};

WORD	KeyCommand=0xFFFF;	
WORD	RunStopCmd=0xFFFF;	

char*	u_pszRobotStatus[] =
{
	"ROBOT_STR_NOT_READY",	
	"ROBOT_STR_READY",			//edit
	"ROBOT_BTN_AUTOMATIC",	//auto
	"ROBOT_BTN_AUTOMATIC",	//mdi
	"ROBOT_BTN_MANUAL",			//JOG
	"ROBOT_BTN_MANUAL",			//吋動
	"ROBOT_BTN_MANUAL",			//MPG
	"ROBOT_BTN_BACKTO_HOME",//backhome
};

char*	u_pszAutoStatusImagePath[] =
{
	"res_tm640/StatusBarAutoStop.bmp",	
	"res_tm640/StatusBarAutoRun.bmp",
	"res_tm640/StatusBarAutoPause.bmp",
	"NULL"
};

char*	u_pszRunStatus[] =
{
	"ROBOT_STR_STATUS_STOPING",
	"ROBOT_STR_STATUS_RUNING",
	"ROBOT_STR_STATUS_PAUSEING"
};

char*	u_pszServoImagePath[] =
{
	"res_tm640/servo_off.png",	//servo_off
	"res_tm640/servo_on.png"	  //servo_on
};

char*	u_pszJawsImagePath[] =
{
	"res_tm640/ON.bmp",	//ON
	"res_tm640/OFF.bmp"	//OFF
};

char*	u_pszPickerImagePath[] =
{
	"taskcomm-off.bmp",
	"taskcomm-on.bmp",
};

char*	u_pszPickerEnableImagePath[] =
{
	"motoroff.bmp",
	"motoron.bmp"
};

char*	u_pszPickerModeImagePath[] =
{
	 NULL,										// 0 無模式
	"picker/BackPoint.bmp", 	// 1 回原點
	"picker/right.bmp", 			// 2 單步                  		
	"picker/Hand.bmp",				// 3 手動
	"picker/Auto.bmp",	      // 4 自動
	 NULL,                    // 5
	 NULL,                    // 6
	 NULL,                    // 7
	 NULL,                    // 8
	 NULL,                    // 9
	 NULL,                    // 10
	 NULL,                    // 11
	 NULL,                    // 12
	 NULL,                    // 13
	 NULL,                    // 14
	 NULL,                    // 15
	 NULL,                    // 16
	"picker/BackPoint.bmp",   // 17 回原點 準備
	"picker/right.bmp",       // 18 單步 準備
	 NULL,                    // 19
	"picker/Auto.bmp",        // 20 自動 準備
	 NULL,                    // 21
	 NULL,                    // 22
	 NULL,                    // 23
	 NULL,                    // 24
	 NULL,                    // 25
	 NULL,                    // 26
	 NULL,                    // 27
	 NULL,                    // 28
	 NULL,                    // 29
	 NULL,                    // 30
	 NULL,                    // 31
	 NULL,                    // 32
	 NULL,                    // 33
	"picker/RightPause.bmp",  // 34 單步 暫停
	 NULL,                    // 35
	"picker/AutoPause.bmp",   // 36 自動 暫停
};
char*	u_pszMoveModeImagePath[] = // 手動移動模式  禁止 吋動 連續動 
{
	 NULL,										// 0 禁止
	"picker/JogMove.bmp", 		// 1 吋動
	"picker/ContinueMove.bmp",// 2 連續動
};

char*	u_pszALARMRImagePath[] =
{
	NULL,
	"Alarm.bmp"
};

char* u_pszGestureImagePath[] =
{
	"picker/Vertical_2.bmp",
	"picker/Horizontical_2.bmp",
};
char*	u_pszClampImagePath[] =
{
	"picker/Clamp_close_22.bmp",
	"picker/Clamp_open_22.bmp",
};
char*	u_pszValveImagePath[] =
{
	"picker/ValveClose.bmp",
	"picker/ValveOpen.bmp",
};
long 	u_lTimer;
long    u_lPickerTimer;
long    u_lPickerError;
long 	u_lDateTimer;
long 	u_lAmtCounter ;
long 	u_lFuYaoCounter;

//enum KEYMODE {AUTO,MANUAL,REMOTE};
//CtmWnd*			pwndRobotStatusImage 	= NULL;
//CtmWnd*			pwndAutoStatusImage 	= NULL;
//CtmWnd*			pwndRobotStatus 	= NULL;
//CtmWnd*			pwndCoordTitle 		= NULL;
//CtmWnd*			pwndAutoStatus		= NULL;
//CtmWnd*			pwndErrorImage 		= NULL;
//CtmWnd*			pwndServoImage 		= NULL;
//CtmWnd*			pwndJaws1Image 		= NULL;
//CtmWnd*			pwndJaws2Static 		= NULL;
//CtmWnd*			pwndJaws2Image 		= NULL;
//CtmWnd*			pwndJaws2Mark			= NULL;

CtmWnd*			u_pwndBmpMoveMode = NULL; // 移動模式圖片 cjlee 2019/7/12 下午 05:45:41
CtmWnd*			u_pwndBmpMode            = NULL;//2017/8/9
CtmWnd*			pwndBmpPicker = NULL;
CtmWnd*			pwndBmpPickerEnable = NULL;
CtmWnd*			pwndBmpALARMR = NULL;
CtmWnd*			pwndBmpUSB = NULL;

CtmWnd*			pwndStaticPose = NULL;
CtmWnd*			pwndEditPostionX = NULL;
CtmWnd*			pwndEditPostionY = NULL;
CtmWnd*			pwndEditPostionZ = NULL;
CtmWnd*			pwndEditPostionX2 = NULL;
CtmWnd*			pwndEditPostionY2 = NULL;
CtmWnd*			pwndBmpState = NULL; // 機器手姿態 cjlee add 2019/5/11 下午 08:38:11
CtmWnd*			pwndPickerSpeed = NULL;// 機器手速度
CtmWnd*			pwndImg_Speed = NULL; // 機器手速度圖 cjlee add 2019/5/9 下午 09:21:24

CtmWnd*			pwndEditOPNCNT    = NULL;
CtmWnd*			pwndEditTOTALT    = NULL;
CtmWnd*			pwndEditACTT	  = NULL;
CtmWnd*			pwndDateTime = NULL; 

CtmWnd*			pwndScreenShot = NULL;

char		szPath[128];
#define 	TIMER_NULL		NULL
#define 	YES_FLASH		1		//flash flags,yes or no
#define 	NO_FLASH		0		//flash flags,yes or no
#define 	MAX_TIMER 		10		//max timer for all flash
WORD		u_wDisplayFlag[MAX_TIMER];	//Deside Display it or not
long 		u_lTimerValue[MAX_TIMER];	//time value for each timer
WORD		g_wMMCStatus =0;	// For statusbar0.cpp to check if usb/mmc was mounted done.<yuc080118>
WORD		g_wUSBStatus =0;   //ZHBA  2009-4-22 for 270
int			aa = 0;

char 			m_szRemoteStr[1024];
//long 			u_lDateTimer = CNT_INT_UPDATE_DATE_TIMER;
int 			RobotStatus = 0;
char 			old_filename[INT_FN_LEN] = "\0";
int 			KeyMode,Last_KeyMode;
int 			u_nMANUAL_TYPE,Last_u_nMANUAL_TYPE;
BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次
BOOL 				HomingEable 			= FALSE;

char 			KeyCodeNow;
/*===========================================================================+
|           Function		                                                 |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("OnCreateA\n");
	printf("CurrentMold=%s\n",CtmMoldSet::GetInstance()->GetCurrentMold());
    AddTimer(&u_lDateTimer);
    #ifdef      D_DEMO
    g_bKeyLockFlag = FALSE;
    #endif

	memset(m_szRemoteStr, 0, sizeof(m_szRemoteStr));
	u_bRemote = CtmConfig::GetInstance()->GetRemoteFlag();
	strncpy(szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
	//strncpy(szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
	//pwndRobotStatusImage	= pwndSender->FindControlFromName("HMIModeImage");
	//pwndAutoStatusImage	= pwndSender->FindControlFromName("HMIAutoSatausImage");
	//pwndRobotStatus		= pwndSender->FindControlFromName("HMIModeStatic");
	//pwndCoordTitle    = pwndSender->FindControlFromName("CoordianteStatic");
	//pwndAutoStatus		= pwndSender->FindControlFromName("AutoSataus");
	//pwndErrorImage		= pwndSender->FindControlFromName("AlarmImage");
	//pwndServoImage		= pwndSender->FindControlFromName("servoimage");
	//pwndJaws1Image		= pwndSender->FindControlFromName("Jaws1image");
	//pwndJaws2Static		= pwndSender->FindControlFromName("Jaws2Static");
	//pwndJaws2Image		= pwndSender->FindControlFromName("Jaws2image");
	//pwndJaws2Mark			= pwndSender->FindControlFromName("Jaws2Mark");
	
	u_pwndBmpMoveMode= pwndSender->FindControlFromName("BmpMoveMode");
	u_pwndBmpMode 	= pwndSender->FindControlFromName("BmpPickerMode");//2017/8/9 
	pwndBmpPicker    = pwndSender->FindControlFromName("BmpPICKER");
	pwndBmpPickerEnable  = pwndSender->FindControlFromName("BmpPickerEnable");
	pwndBmpALARMR 	= pwndSender->FindControlFromName("BmpALARM");
	pwndBmpUSB 		= pwndSender->FindControlFromName("BmpUSB");
	
	pwndStaticPose   = pwndSender->FindControlFromName("StaticPose");  
	pwndEditPostionX = pwndSender->FindControlFromName("EditXposition");
	pwndEditPostionY = pwndSender->FindControlFromName("EditYposition");
	pwndEditPostionZ = pwndSender->FindControlFromName("EditZposition");
	pwndEditPostionX2= pwndSender->FindControlFromName("EditX2position");
	pwndEditPostionY2= pwndSender->FindControlFromName("EditY2position");

	
	pwndEditOPNCNT  	= pwndSender->FindControlFromName("editOPENCOUNT");
  pwndEditTOTALT   	= pwndSender->FindControlFromName("editCYCLETIME");
  pwndEditACTT	    = pwndSender->FindControlFromName("editOPERATIONTIME");
  pwndDateTime 		= pwndSender->FindControlFromName("DateTime");

	pwndScreenShot   = pwndSender->FindControlFromName("btnScreenShot");
	pwndPickerSpeed	 = pwndSender->FindControlFromName("PickerSpeed");
	pwndImg_Speed 	 = pwndSender->FindControlFromName("Img_Speed");
	pwndBmpState		 = pwndSender->FindControlFromName("BmpState");
	
	//if(CtmConfig::GetInstance()->GetCncCommFlag() ==1)	SendMsg(MSG_CNC_PCTOOL_ONLINE, 0xffff, (long)0xffffffff, NULL);
	
	Init_Common_User();
	if(g_statusbar == NULL)
		g_statusbar = pwndSender;
	return TRUE;
}


void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	int 	nErrorStrIndex =-1;
	char	szErrorText[40];
 	szErrorText[0]='\0';
    long Color =63488;
    BOOL	ShowErrorNo=FALSE;
		int servo_temp = 0;
	//printf("message=0x%x\n",message);
    switch(message)
	{
		case MSG_USER_SH_ERROR_ADD:

			//進警報下始能
			//g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&servo_temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
			
			SetGpioLed(TRUE,GPIO_BIT_ALARM);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> SetPropValueT("fgc",Color);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> Update();
			switch(g_pRxCNCData->CNC_alarm_group)	//OPER,INTER,MOTION,MLC
			{
				case ID_OPER://CNC_ALARM_OP://1
					sprintf(szErrorText, "ROBOT_ALARM_OPER%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_INTER://CNC_ALARM_INTER://2
					sprintf(szErrorText, "ROBOT_ALARM_INTER%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_MOTION://CNC_ALARM_MOTION://3
					sprintf(szErrorText, "ROBOT_ALARM_MOT%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_MLC://CNC_ALARM_MLC://4
					sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_DRIVERX://CNC_ALARM_DRIVERX://5
					sprintf(szErrorText, "ROBOT_ALARM_AXIS1_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERY://CNC_ALARM_DRIVERY://6
					sprintf(szErrorText, "ROBOT_ALARM_AXIS2_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERZ://CNC_ALARM_DRIVERZ://7
					sprintf(szErrorText, "ROBOT_ALARM_AXIS3_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERA://CNC_ALARM_DRIVERA://8
					sprintf(szErrorText, "ROBOT_ALARM_AXIS4_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERB://CNC_ALARM_DRIVERB://9
					sprintf(szErrorText, "ROBOT_ALARM_AXIS5_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERC://CNC_ALARM_DRIVERC://10
					sprintf(szErrorText, "ROBOT_ALARM_AXIS6_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DSP://CNC_ALARM_DSP://11
					sprintf(szErrorText, "ROBOT_ALARM_DSP%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				default:
					sprintf(szErrorText, "_VW_DRIVER_MANUAL");
					break;	
			}
			nErrorStrIndex = g_MultiLanguage.GetStrKeyIndex(szErrorText);
			if(ShowErrorNo)
			{
				char temp[10],szTemp[20],ErrorStr[256];
				memset(temp, 0, sizeof(temp));
				memset(szTemp, 0, sizeof(szTemp));
				memset(ErrorStr, 0, sizeof(ErrorStr));
				sprintf(temp,"%d",g_pRxCNCData->CNC_alarm_id);
				CodeChange(szTemp,temp);
				Double_byte_String_Merger(ErrorStr,g_MultiLanguage[nErrorStrIndex],szTemp);
				Prompt(ErrorStr,1);
			}
			else
				Prompt(g_MultiLanguage[nErrorStrIndex],1);
			break;
		case MSG_USER_SH_ERROR_FIX:
			SetGpioLed(FALSE,GPIO_BIT_ALARM);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> SetPropValueT("fgc",(double)0x00);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> Update();
			Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY")],1);
			break; 
		case  MSG_SH_REMOTE_REQ_CONTROL:
			sprintf(m_szRemoteStr, "Connection Request!!\nName:%s\nPassWord:%s", 
									(char*)wParam, (char*)lParam);
			 
			MsgBox(g_MultiLanguage["VW_REMOTE_VIEW_RECV"], tmFT_CODE_TECH);
			SendMsg(MSG_SH_REMOTE_REV_CONTROL, g_bMsgBoxFlag, 0, NULL);
			break;
		//2012/6/20 上午 11:10:18 add for remote>>>>>		
		case	MSG_GPIO_READ:
		{
			HomingEable =FALSE;
			printf("MSG_GPIO_READ\n");
			// 旋轉鑰匙
			{
			char gpio_key;
			gpio_key = wParam&GPIO_KEY_BIT;
			printf("gpio_key = %d,wParam=%x\n",gpio_key,wParam);
			if(gpio_key==GPIO_KEY_AUTO) // 下
				{
					printf("KeyMode=AUTO\n");
					KeyMode=AUTO;
					//SendCommand(0xf500);
				}
			else if(gpio_key==GPIO_KEY_TEACH)
				{
					printf("KeyMode=REMOTE\n");
					KeyMode=REMOTE;
					//SendCommand(0xf300);
				}
			else if(gpio_key==GPIO_KEY_STOP) 
				{
					printf("KeyMode=MANUAL\n");
					KeyMode=MANUAL;
					//SendCommand(0xf400);
				}
			}
			// 背面安全按鈕
			{
			char gpio_safekey;
			gpio_safekey = wParam&GPIO_SAFEKEY_BIT;
			printf("gpio_safekey = %x\n",gpio_safekey);
			if(gpio_safekey==GPIO_SAFEKEY_0) // 放開 停止
				{
					//printf("ACT=STOP\n");
					u_nMANUAL_TYPE = MANUAL_STOP_MODE;
					{ // 停止軸動作
						if(KeyCodeNow == 0x83 || KeyCodeNow == 0x93)	//X1軸
						{
							KeyCommand = COMMAND_X1SLOWDOWN;
						}
						else if(KeyCodeNow == 0x84 || KeyCodeNow == 0x94)	//Y1軸
						{
							KeyCommand = COMMAND_Y1SLOWDOWN;
						}
						else if(KeyCodeNow == 0x82 || KeyCodeNow == 0x92)	//Z軸
						{
							KeyCommand = COMMAND_ZSLOWDOWN;
						}
						else if(KeyCodeNow == 0x87 || KeyCodeNow == 0x97)	//X2軸
						{
							KeyCommand = COMMAND_X2SLOWDOWN;
						}
						else if(KeyCodeNow == 0x88 || KeyCodeNow == 0x98)	//Y2軸
						{
							KeyCommand = COMMAND_Y2SLOWDOWN;
						}
						SendCommand(KeyCommand);
					}
				}
			else if(gpio_safekey==GPIO_SAFEKEY_1) // 1段 點動
				{
					//printf("ACT=JOG\n");
					u_nMANUAL_TYPE = MANUAL_JOG_MODE;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_2) // 2段 連續動
				{
					//printf("ACT=CONTINUE\n");
					u_nMANUAL_TYPE = MANUAL_CONTINUE_MODE;
				}
			}
		}
		break;
		case	MSG_PANEL_KEY: //hotkey
		{
			char keycode;
			keycode = wParam;
			KeyCodeNow = keycode;
			printf("panel key = %x\n",keycode);
			int N = (keycode & 0x10)/16;
			if(N==0) N=2;
			printf("N=%d\n",N);
			if((keycode >= 0x82 && keycode <= 0x88) || (keycode >= 0x92 && keycode <= 0x98) || (keycode >= 0x12 && keycode <= 0x18) || (keycode >= 0x02 && keycode <= 0x08))
			{/*-------------------------------軸動作----------------------------------*/
				printf("Move CMD\n");
				if(keycode == 0x83 || keycode == 0x93)	//X1軸
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 1);
					SetDBValue(pWhichAxisDB,1);
					KeyCommand = (N == 2) ? COMMAND_X_FWD : COMMAND_X_BACK;
					pwndEditPostionX->SetPropValueT("fgc",0xFF80); // 顯示目前移動軸 黃色
				}
				else if(keycode == 0x84 || keycode == 0x94)	//Y1軸
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 2);
					SetDBValue(pWhichAxisDB,2);
					KeyCommand = (N == 2) ? COMMAND_Y_FWD : COMMAND_Y_BACK;
					pwndEditPostionY->SetPropValueT("fgc",0xFF80); // 顯示目前移動軸 黃色
				}
				else if(keycode == 0x82 || keycode == 0x92)	//Z軸
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 4);
					SetDBValue(pWhichAxisDB,3);
					KeyCommand = (N == 2) ? COMMAND_Z_FWD : COMMAND_Z_BACK;
					pwndEditPostionZ->SetPropValueT("fgc",0xFF80); // 顯示目前移動軸 黃色
				}
				else if(keycode == 0x85 || keycode == 0x95)	//C軸 姿態
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 8);// cjlee 2019/4/6 下午 05:56:23
					KeyCommand = (N == 2) ? COMMAND_C_FWD : COMMAND_C_BACK;
				}
				else if(keycode == 0x87 || keycode == 0x97)	//X2軸
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44", 1);// cjlee 2019/4/6 下午 05:56:23
					SetDBValue(pWhichAxisDB,4);
					KeyCommand = (N == 2) ? COMMAND_X2_FWD : COMMAND_X2_BACK;
					pwndEditPostionX2->SetPropValueT("fgc",0xFF80); // 顯示目前移動軸 黃色
				}
				else if(keycode == 0x88 || keycode == 0x98)	//Y2軸
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44", 2);// cjlee 2019/4/6 下午 05:56:23
					SetDBValue(pWhichAxisDB,5);
					KeyCommand = (N == 2) ? COMMAND_Y2_FWD : COMMAND_Y2_BACK;
					pwndEditPostionY2->SetPropValueT("fgc",0xFF80); // 顯示目前移動軸 黃色
				}
				else if(keycode >= 0x12 || keycode <= 0x18 || keycode >= 0x02 || keycode <= 0x08)	//STOP
				{
					//KeyCommand = COMMAND_STOP; 
					if(keycode == 0x03 || keycode == 0x13)	//X1軸 放開
					{
						KeyCommand = COMMAND_X1SLOWDOWN;
						SetDBValue(pWhichAxisDB,6);
					}
					else if(keycode == 0x04 || keycode == 0x14)	//Y1軸 放開
					{
						KeyCommand = COMMAND_Y1SLOWDOWN;
						SetDBValue(pWhichAxisDB,7);
					}
					else if(keycode == 0x02 || keycode == 0x12)	//Z軸 放開
					{
						KeyCommand = COMMAND_ZSLOWDOWN;
						SetDBValue(pWhichAxisDB,8);
					}
					else if(keycode == 0x07 || keycode == 0x17)	//X2軸 放開
					{
						KeyCommand = COMMAND_X2SLOWDOWN;
						SetDBValue(pWhichAxisDB,9);
					}
					else if(keycode == 0x08 || keycode == 0x18)	//Y2軸 放開
					{
						KeyCommand = COMMAND_Y2SLOWDOWN;
						SetDBValue(pWhichAxisDB,10);
					}
					pwndEditPostionX->SetPropValueT("fgc",0xFFFF);
					pwndEditPostionY->SetPropValueT("fgc",0xFFFF);
					pwndEditPostionZ->SetPropValueT("fgc",0xFFFF);
					pwndEditPostionX2->SetPropValueT("fgc",0xFFFF);
					pwndEditPostionY2->SetPropValueT("fgc",0xFFFF);
				}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 0);
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44", 0);
				}
				/*-----------------------------------運動命令--------------------------------------*/
				if((KeyMode == MANUAL) && (g_ptaskpicker != NULL)) // 手動模式下
				{
					if(u_nMANUAL_TYPE != MANUAL_STOP_MODE) // 安全按鈕有按下
						SendCommand(KeyCommand);
					else // 請按安全開關
						 //MsgBox(g_MultiLanguage["PICKER_SAFEEYNOTPRESS"], tmFT_CODE_TECH);		
						 MsgBoxCall("msgboxConfirm.txt","PICKER_SAFEEYNOTPRESS");				
				}
				/*-----------------------------------運動命令--------------------------------------*/
			}/*-------------------------------軸動作----------------------------------*/
			
			if((keycode == 0xc0 && KeyMode != AUTO) || (keycode == 0x40 && KeyMode  != AUTO))
				{
					if(keycode == 0xc0)	//Homing down
					{
						RunStopCmd = MODE_HOMING;
						
					}
					else if(keycode == 0x40)	//Homing up
					{
						RunStopCmd = 0xFF50;
					}
					SendCommand(RunStopCmd);
					HomingEable =TRUE;
				}
				
			if((keycode == 0x91) || (keycode == 0x81) || (keycode == 0x11) || (keycode == 0x01))
			{ /*-------------------------------啟動、停止----------------------------------*/
				if(keycode == 0x91)	//Run down
				{
					RunStopCmd = 0xF700;
				}
				else if(keycode == 0x81)	//Stop down
				{
					RunStopCmd = 0xF701;
				}
				else if((keycode == 0x11)||(keycode == 0x01)) // Run/Stop up
				{
					RunStopCmd = 0xFF70;
				}

	  		if((g_ptaskpicker !=NULL) && (u_wPickerOPSatus != STATE_HAND))
				{
					SendCommand(RunStopCmd);
					//g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, (int *)&RunStopCmd, NULL);
					//printf("Send RunStopCmd=%x\n",RunStopCmd);
				}
				else if((g_ptaskpicker !=NULL) && (HomingEable == TRUE) )
				{
					SendCommand(RunStopCmd);
				}

			} /*-------------------------------啟動、停止----------------------------------*/
			
			if((keycode==0x99)||(keycode ==0x89)) //速度增減
			{
				if((KeyMode != AUTO) || ((KeyMode == AUTO) && (u_nMANUAL_TYPE != MANUAL_STOP_MODE))) // 自動模式下 // 安全按鈕有按下
					{				
						long lTempValue = GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue;
						if((lTempValue < 100)&&(keycode ==0x99))
							SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lTempValue+5);
						if((lTempValue > 0)&&(keycode ==0x89))
							SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lTempValue-5);
					}
				else if(KeyMode == AUTO && (u_nMANUAL_TYPE == MANUAL_STOP_MODE) ) // 請按安全開關
					{
						MsgBoxCall("msgboxConfirm.txt","PICKER_SAFEEYNOTPRESS");			
					}			
			}
		}
		break;
		default:
			break;	
	}	

}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("up  wIDControl:%d\n", wIDControl);
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;

 	char    szImagePath[256];
 	memset(szImagePath, 0, sizeof(szImagePath));
 	
 	if(pwnd == pwndScreenShot)
 	{
 		printf("SaveScreen\n");
		SaveScreen();
 	}
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	//WORD wGetTime =GetDBValue("MACHINE_INTERFACE_LASTMODETIME").lValue;
	//printf("wGetTime=%d\n",wGetTime);
	//printf("status OnUpdateA\n");
	WORD	wActionSatus = g_pRxCNCData->CNC_status;//Sunny<20101125>
	static char old_NowRunStatus = 9;
	static char Servo_status = 0;
	static char Auto_status = -1;
	static char Jaws1_status = -1;
	static char Jaws2_status = -1;
	char    sz[256];
	tmDATE        date;
	tmTIME        time;	
	
	/*if(Jaws1_status!=g_pRxCNCData->s_mlc_s_bit[101])
	{
		Jaws1_status=g_pRxCNCData->s_mlc_s_bit[101];
		pwndJaws1Image->SetPropValueT("imagepath",u_pszJawsImagePath[Jaws1_status]);
		pwndJaws1Image->Update();
		
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE)
		{
			int tmp = g_TxCNCData.shmCNCData.CNC_jaws_control;
			if(tmp%2>=1 && g_pRxCNCData->s_mlc_s_bit[101]==0)
	      tmp-=1;
			else if(tmp%2<1 && g_pRxCNCData->s_mlc_s_bit[101]==1)
	      tmp+=1;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		}
	}*/
	/*
	static int Last_Jaws_Count = -1;
	if(Last_Jaws_Count<2 && g_TxCNCData.shmCNCData.PARAM.maker_param[33]>=2)
	{
		Last_Jaws_Count = g_TxCNCData.shmCNCData.PARAM.maker_param[33];
		pwndJaws2Image->Update();
		pwndJaws2Static->Update();
	}
	else if (Last_Jaws_Count>=2 && g_TxCNCData.shmCNCData.PARAM.maker_param[33]<2)
	{
		Last_Jaws_Count = g_TxCNCData.shmCNCData.PARAM.maker_param[33];
		pwndJaws2Mark->Update();
	}
	
	if(Jaws2_status!=g_pRxCNCData->s_mlc_s_bit[102])
	{
		Jaws2_status=g_pRxCNCData->s_mlc_s_bit[102];
		pwndJaws2Image->SetPropValueT("imagepath",u_pszJawsImagePath[Jaws2_status]);
		if(g_TxCNCData.shmCNCData.PARAM.maker_param[33]>=2)
			pwndJaws2Image->Update();
		
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE)
		{
			int tmp = g_TxCNCData.shmCNCData.CNC_jaws_control;
			int tmp1 = g_TxCNCData.shmCNCData.CNC_jaws_control%4;
			if(tmp%4>=2 && g_pRxCNCData->s_mlc_s_bit[102]==0)
	      tmp-=2;
			else if(tmp%4<2 && g_pRxCNCData->s_mlc_s_bit[102]==1)
	      tmp+=2;
	      
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		}
	}
	*/
	
		if(g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP)
			SetGpioLed(TRUE,GPIO_BIT_AUTO);
		else
			SetGpioLed(FALSE,GPIO_BIT_AUTO);

		
	/*
	if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_STOP)
	{
		strcpy(old_filename,g_TxCNCData.shmCNCData.CNC_mem_part);
 		g_pMainFrame->SetTitle2(g_TxCNCData.shmCNCData.CNC_mem_part);
	}
	else if(g_pRxCNCData->CNC_curr_blk.modal.fn[0] != 0  && (strcmp(old_filename,g_pRxCNCData->CNC_curr_blk.modal.fn) != 0) && (g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_RUN || g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_PAUSE) )
	{
		strcpy(old_filename,g_pRxCNCData->CNC_curr_blk.modal.fn);
 		g_pMainFrame->SetTitle2(g_pRxCNCData->CNC_curr_blk.modal.fn);
	}
	*/
//	if(Servo_status != g_pRxCNCData->CNC_servo_status)
//	{
//		Servo_status = g_pRxCNCData->CNC_servo_status;
//		if(Servo_status==1)
//			SetGpioLed(TRUE,GPIO_BIT_ENABLE);
//		else
//			SetGpioLed(FALSE,GPIO_BIT_ENABLE);
//		/*pwndServoImage->SetPropValueT("imagepath",u_pszServoImagePath[(int)Servo_status]);
//		pwndServoImage->Update();*/
//	}

	// cjlee test 2019/3/22 上午 11:01:55
	/*{
		static int nLEDState = 0;
		static int nCount = 0;
		
		nCount ++;
		if(nCount >= 5)
		{
				nCount = 0;
			if(nLEDState == 0)
				nLEDState = 1;
			else
				nLEDState = 0;
		}
		if(nLEDState==1)
				SetGpioLed(TRUE,GPIO_BIT_ENABLE);
			else
				SetGpioLed(FALSE,GPIO_BIT_ENABLE);
	}*/
			
	/*
	if(old_NowRunStatus != g_TxCNCData.shmCNCData.CNC_auto_status)
	{
		old_NowRunStatus = g_TxCNCData.shmCNCData.CNC_auto_status;
		//pwndAutoStatus->SetPropValueT("textID",u_pszRunStatus[(int)g_TxCNCData.shmCNCData.CNC_auto_status]);
		//pwndAutoStatus->Update();
	}
	*/
	/* 
	if(Auto_status!=g_TxCNCData.shmCNCData.CNC_auto_status && pwndAutoStatusImage!=NULL)
	{
		Auto_status=g_TxCNCData.shmCNCData.CNC_auto_status;
		//pwndAutoStatusImage->SetPropValueT("imagepath",u_pszAutoStatusImagePath[Auto_status]);
		//pwndAutoStatusImage->Update();
	}
	*/
	//------------HMI模式狀態---------------//
	
/*
	if(KeyMode!=AUTO && (g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP))
	{
		char temp = RUN_STATUS_STOP;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_auto_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_auto_status));
	}
	else if(KeyMode != MANUAL && KeyMode != REMOTE)
	{
		char temp_CHAT = 0;
		int temp_INT = 0;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp_INT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_status,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_status));
	}
	if(g_pRxCNCData->CNC_move_end==1)
	{
		if(KeyMode==AUTO)
		//if((g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP) && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE)  	//執行時強制auto狀態
		{
			int mode_lgt  = OP_MEMORY_MODE;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_mode_lgt,&mode_lgt,sizeof(g_TxCNCData.shmCNCData.CNC_mode_lgt));
		}
		else if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_STOP && g_pRxCNCData->CNC_status==M_READY && g_TxCNCData.shmCNCData.CNC_mode_lgt != RobotStatus)								//停止時顯示目前狀態
		{
			int mode_lgt = RobotStatus;
			if(mode_lgt==OP_MEMORY_MODE) mode_lgt=OP_EDIT_MODE;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_mode_lgt,&mode_lgt,sizeof(g_TxCNCData.shmCNCData.CNC_mode_lgt));
		}
	}
*/

	if(RunOnlyOne)
{/*-----------------------------------機械手模式----------------------------------*/
			if(KeyMode != Last_KeyMode) // 旋轉鑰匙
				{
					printf("New KeyMode\n");
					if(KeyMode == MANUAL) // 手動
					{
						//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 3);
						SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
						SendCommand(MODE_MANUAL);
					}
					else if(KeyMode == AUTO) // 自動 
					{
						//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 4);
						SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
						SendCommand(MODE_AUTO);
					}
					else if(KeyMode == REMOTE) // 停止
					{
						//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 2);// 單步
						SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
						//SendCommand(0xf400);//單步
						SendCommand(MODE_NULL);//停止
					}
	
					Last_KeyMode = KeyMode;
				}
			if(u_nMANUAL_TYPE != Last_u_nMANUAL_TYPE) // 背面安全按鈕
				{
					printf("New SafeKey Mode\n");
					SetDBValue("MACHINE_CONFIGURATION_MANUAL_TYPE",u_nMANUAL_TYPE);
					printf("Safe Key =%d\n",u_nMANUAL_TYPE);
					Last_u_nMANUAL_TYPE = u_nMANUAL_TYPE;
				}	
}/*-----------------------------------機械手模式----------------------------------*/
	

	if(!RunOnlyOne) // 開機只運行一次
	{
		printf("RunOnlyOne\n");
	/*-----------------------------------機械手速度----------------------------------*/
		long lSpeed = GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue;
		if(lSpeed > 0)
		{
			SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lSpeed-5);
			SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lSpeed);
		}
		long lSpeedValue=GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue;
		if(pwndImg_Speed!=NULL)
		{
			char		szImagePath[256];
			sprintf(szImagePath, "res_tm640/pic/Speed_%d.bmp",lSpeedValue);
			//printf("Set Speed =%s\n",szImagePath);
  		pwndImg_Speed->SetPropValueT("imagepath",szImagePath);
  		pwndImg_Speed->Update();
		}
		lSpeedValueOld=lSpeedValue;
	/*-----------------------------------機械手速度----------------------------------*/
	
		//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 0);
		if(KeyMode == MANUAL) // 在手動才送 其他無模式
		{
			//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 3);
			SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
			WORD u_PickerState = GetDBValue("MACHINE_INTERFACE_CONTROL_RSV15").lValue;
			printf("STATE=%d\n",u_PickerState);
			SendCommand(MODE_MANUAL);
		}
		else if(KeyMode == AUTO) // 自動 
		{
			//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 4);
			SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
		}
		else if(KeyMode == REMOTE) 
		{
			//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 2);// 單步
			SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
		}
		Last_KeyMode = KeyMode;
		Last_u_nMANUAL_TYPE = u_nMANUAL_TYPE;
		RunOnlyOne=TRUE;
	}

	//----------現在時間---------------//
	if (u_lDateTimer <= 0)
	{	
			GetTime(&time);  		
   		GetDate(&date);

   		sprintf(sz, "%04u.%02u.%02u/%02u:%02u", date.year, date.month, date.day, time.hour, time.minute);
      g_pMainFrame->GetPromptBar()->GetControlA(0)->Clear();
      g_pMainFrame->GetPromptBar()->GetControlA(0)->ShowExtent(0, 1, sz, 0);
		
		u_lDateTimer= CNT_INT_UPDATE_DATE_TIMER;
	}

{/*-----------------------------------機械手座標----------------------------------*/
	SetEditValue(pwndEditPostionX);
	SetEditValue(pwndEditPostionY);
	SetEditValue(pwndEditPostionZ);
	SetEditValue(pwndEditPostionX2);
	SetEditValue(pwndEditPostionY2);	
}/*-----------------------------------機械手座標----------------------------------*/
	
{/*-----------------------------------機械手速度----------------------------------*/
	SetEditValue(pwndPickerSpeed);
	long lSpeedValue=GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue;
  	if(lSpeedValue!=lSpeedValueOld)
  	{
 			if(pwndImg_Speed!=NULL)
 			{
				char		szImagePath[256];
				sprintf(szImagePath, "res_tm640/pic/Speed_%d.bmp",lSpeedValue);
				//printf("Set Speed =%s\n",szImagePath);
	  		pwndImg_Speed->SetPropValueT("imagepath",szImagePath);
	  		pwndImg_Speed->Update();
			}
			lSpeedValueOld=lSpeedValue;
		}
}/*-----------------------------------機械手速度----------------------------------*/
	
{/*------------------------------------生產資訊-----------------------------------*/
	SetEditValue(pwndEditOPNCNT); 
	SetEditValue(pwndEditTOTALT);
	SetEditValue(pwndEditACTT);
	if(pwndEditTOTALT!=NULL)
	{
		pwndEditTOTALT->UpdateAll();
	}
}/*------------------------------------生產資訊-----------------------------------*/
	
{/*-----------------------------------機械手狀態----------------------------------*/
	long lpickerStatus = g_ptaskpicker->GetOnLineStatus();
	if(!lpickerStatus)  lpickerStatus = 1;
	else		 		lpickerStatus = 0;
	//printf("lpickerStatus=%d\n",lpickerStatus);	

	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
	//printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);	
	if(u_wPickerOPSatus < sizeof(u_pszPickerModeImagePath) / sizeof(char*))
	{
		sprintf(sz,"%s%s", szPath, u_pszPickerModeImagePath[u_wPickerOPSatus]);
		//printf("u_pszPickerModeImagePath=%s\n",sz);	
		ChangeImage(u_pwndBmpMode, sz, NO_FLASH, 0);
	}
	else
	{
		ChangeImage(u_pwndBmpMode, NULL,NO_FLASH, 0);
	}
}/*-----------------------------------機械手狀態----------------------------------*/

{/*----------------------------------手動移動模式---------------------------------*/
	if((u_nMANUAL_TYPE == MANUAL_JOG_MODE) && (u_wPickerOPSatus == STATE_HAND)) // 1段 點動
	{
		sprintf(sz,"%s%s", szPath, u_pszMoveModeImagePath[1]);
		//printf("u_pszMoveModeImagePath=%s\n",sz);	
		ChangeImage(u_pwndBmpMoveMode, sz, NO_FLASH, 0);
	}
	else if((u_nMANUAL_TYPE == MANUAL_CONTINUE_MODE) && (u_wPickerOPSatus == STATE_HAND)) // 2段 連續動
	{
		sprintf(sz,"%s%s", szPath, u_pszMoveModeImagePath[2]);
		//printf("u_pszMoveModeImagePath=%s\n",sz);	
		ChangeImage(u_pwndBmpMoveMode, sz, NO_FLASH, 0);
	}
	else
	{
		ChangeImage(u_pwndBmpMoveMode, NULL,NO_FLASH, 0);
	}
}/*-----------------------------------機械手狀態----------------------------------*/

//{/*-----------------------------------機械手暫停----------------------------------*/	
//	if((u_wPickerOPSatus == SINGLE_PAUSE) || (u_wPickerOPSatus==AUTO_PAUSE))
//	{
//		u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
//		printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);	
//		MsgBoxCall("msgboxPause.txt");
//	}
//}/*-----------------------------------機械手暫停----------------------------------*/

{/*-------------------------------------姿態----------------------------------------*/
	u_wGestureStatus = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE").lValue;	
//	if(pwndStaticPose!=NULL)
//	{
//		if(u_wGestureStatus & 0x0001) //0x0100
//			pwndStaticPose ->SetPropValueT("textID", "PICKER_VERTICAL");
//		if(u_wGestureStatus & 0x0002) //0x0200
//			pwndStaticPose ->SetPropValueT("textID", "PICKER_HORIZONTAL");
//		pwndStaticPose->Update();
//	}
	long lStatusValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE").lValue;
	if(lStatusValue != lStatusValueOld)
	{
		if(pwndBmpState!=NULL) // 姿態圖示
		{
			if(u_wGestureStatus & 0x0001) //0x0100
				pwndBmpState ->SetPropValueT("imagepath", "res_tm640/pic/Axis_C_V.bmp");
			if(u_wGestureStatus & 0x0002) //0x0200
				pwndBmpState ->SetPropValueT("imagepath", "res_tm640/pic/Axis_C_H.bmp");
			pwndBmpState->Update();
		}
		lStatusValueOld=lStatusValue;
	}

	
}/*-------------------------------------姿態----------------------------------------*/
	
{/*-------------------------------------使能----------------------------------------*/
	WORD wEnableStatus = GetDBValue("MACHINE_INTERFACE_MOTORENABLESTATE").lValue;
	//printf("wEnableStatus=%d\n",wEnableStatus);
	if(wEnableStatus < sizeof(u_pszPickerEnableImagePath)/sizeof(char *))
	{
		sprintf(sz,"%s%s",szPath,u_pszPickerEnableImagePath[wEnableStatus]);
		//printf("u_pszPickerEnableImagePath=%s\n",sz);
		ChangeImage(pwndBmpPickerEnable, sz, NO_FLASH, 0);
	}
}/*-------------------------------------使能----------------------------------------*/
	#ifdef	D_3354	
{/*-------------------------------------通訊----------------------------------------*/
	if(g_ptaskpicker != NULL)
	{
		int npickerStatus = g_ptaskpicker->GetOnLineStatus();
		if(!npickerStatus)  npickerStatus = 1;
		else		 		npickerStatus = 0;
		//printf("npickerStatus=%d\n",npickerStatus);
		sprintf(sz,"%s%s",szPath,u_pszPickerImagePath[npickerStatus]);
		//printf("u_pszPickerImagePath=%s\n",sz);
		ChangeImage(pwndBmpPicker, sz, NO_FLASH, 0);
	}
}/*-------------------------------------通訊----------------------------------------*/
	#endif
{/*-------------------------------------警報----------------------------------------*/
	if(g_wErrorStatus == 1 || g_wErrorStatus_Dsp28 == 1)						
	{
		if(u_lTimer <= 0)
		{
			u_wError ^= 1;
			u_lTimer = 200;
			//printf("u_wError = %d,u_lTimer = %d\n",u_wError,u_lTimer);
		}
		if(u_pszALARMRImagePath[u_wError] != NULL)
		{
			sprintf(sz,"%s%s",szPath,u_pszALARMRImagePath[u_wError]);
			ChangeImage(pwndBmpALARMR, sz, NO_FLASH,0);
		}
		else	ChangeImage(pwndBmpALARMR, NULL, NO_FLASH,0);
	}
	else
	{
		u_wError = 0;
		ChangeImage(pwndBmpALARMR, u_pszALARMRImagePath[u_wError], NO_FLASH,0);
	}
}/*-------------------------------------警報----------------------------------------*/
	
{/*-------------------------------------存儲----------------------------------------*/
	if((g_wMMCStatus	==	1)||(g_wUSBStatus ==1)) //ZHBA  2009-4-22 for 270 //1:Have mmc  0:Not have  yuc<080118>
	{
		sprintf(sz,"%s%s",szPath,"usb.bmp");
		ChangeImage(pwndBmpUSB, sz, NO_FLASH, 0);
	}
	else
		ChangeImage(pwndBmpUSB, NULL , NO_FLASH, 0);	
}/*-------------------------------------存儲----------------------------------------*/
	
	
	
	//-------------工作/工具座標顯示---------------//
	/*
	if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_RUN || g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_PAUSE)
	{
		if(work_coord_no != g_pRxCNCData->CNC_curr_blk.trans.cw_coord  || tool_coord_no != g_pRxCNCData->CNC_curr_blk.trans.ct_coord)
		{
			char	temp[15] 		= "\0";
			char	szTemp[15]		= "\0";
			memset(temp, 0, sizeof(temp));
			memset(szTemp, 0, sizeof(szTemp));
			work_coord_no = g_pRxCNCData->CNC_curr_blk.trans.cw_coord;
			tool_coord_no = g_pRxCNCData->CNC_curr_blk.trans.ct_coord;
			SetDBValue("CNC_WORK_COORD_NO", work_coord_no);	
			SetDBValue("CNC_TOOL_COORD_NO", tool_coord_no);
			if(work_coord_no == 0 && tool_coord_no == 0)
				sprintf(temp, "W:No T:No");
			else if(work_coord_no == 0)
				sprintf(temp, "W:No T:%d",tool_coord_no);
			else if(tool_coord_no == 0)
				sprintf(temp, "W:%d T:No", work_coord_no);
			else
				sprintf(temp, "W:%d T:%d", work_coord_no,tool_coord_no);
			
			CodeChange(szTemp, temp);
			pwndCoordTitle->SetPropValueT("text", szTemp);
			pwndCoordTitle->Update();
		}
	}
	else
	{
		//printf("status g_TxCNCData.shmCNCData.CNC_work_coord_no\n");
		if(g_TxCNCData.shmCNCData.CNC_work_coord_no==0)
			SetDBValue("CNC_WORK_COORD_NO", 1);	
		if(g_TxCNCData.shmCNCData.CNC_tool_coord_no==0)
			SetDBValue("CNC_TOOL_COORD_NO", 1);
		//printf("work_coord_no = %d , g_TxCNCData.shmCNCData.CNC_work_coord_no = %d\n",work_coord_no,g_TxCNCData.shmCNCData.CNC_work_coord_no);
		if(work_coord_no != g_TxCNCData.shmCNCData.CNC_work_coord_no  || tool_coord_no != g_TxCNCData.shmCNCData.CNC_tool_coord_no)
		{
			char	temp[15] 		= "\0";
			char	szTemp[15]		= "\0";
			memset(temp, 0, sizeof(temp));
			memset(szTemp, 0, sizeof(szTemp));
			work_coord_no = g_TxCNCData.shmCNCData.CNC_work_coord_no;
			tool_coord_no = g_TxCNCData.shmCNCData.CNC_tool_coord_no;
			if(work_coord_no == 0 && tool_coord_no == 0)
				sprintf(temp, "W:No T:No");
			else if(work_coord_no == 0)
				sprintf(temp, "W:No T:%d",tool_coord_no);
			else if(tool_coord_no == 0)
				sprintf(temp, "W:%d T:No", work_coord_no);
			else
				sprintf(temp, "W:%d T:%d", work_coord_no,tool_coord_no);
			
			CodeChange(szTemp, temp);
			pwndCoordTitle->SetPropValueT("text", szTemp);
			pwndCoordTitle->Update();
		}
	}
	*/
	if(g_TxCNCData.shmCNCData.CNC_update_wt_coord != 0 && g_pRxCNCData->CNC_update_wt_coord_OK != 0)
	{
		char temp=0;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_update_wt_coord,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
	}
	
	if(Ethernet_Break == 1 || Ethernet_Break == 2)
	{
		MsgBoxCall("msgboxEthernetBreak.txt");
	}
	
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	//DeleteTimer(&u_lDemoTime);			//	銷毀計時器
	//DeleteTimer(&u_lDateTimer);
	
}


void	Init_Common_User()
{
	umount("/mnt/usb");
}

void	Double_byte_String_Merger(char* out, char* str1 , char* str2) 
{
	if(str1 == NULL || str2 == NULL)  
	{
		//printf("NULL\n");
		return;
	}
	
	WORD *pszDst,*pszSrc;
	pszDst = (WORD*)out;
	
  pszSrc=(WORD*)str1;
	while(*pszSrc != 0) 
  {
  	*pszDst ++ = *pszSrc ++;
  }
  
  pszSrc=(WORD*)str2;
  while(*pszSrc != 0) 
  {
  	*pszDst ++ = *pszSrc ++;
  }
  out = (char*)pszDst;
}


void	SetGpioLed(BOOL status,int bit)
{
	static char ledstatus=0x07;	//gpio 
	if (bit < 1) return;
	char bit_0x = 0x01 << (bit-1);
	//printf("bit_0x = 0x%02x\n",bit_0x);
	if(status)
	{
		bit_0x = ~bit_0x;
		ledstatus &= bit_0x;
		_Gpio_OutputWrite(&ledstatus);
		//printf("true bit_0x = 0x%02x ledstatus = 0x%02x\n",bit_0x,ledstatus);
	}
	else
	{
		ledstatus |= bit_0x;
		_Gpio_OutputWrite(&ledstatus);
		//printf("false bit_0x = 0x%02x ledstatus = 0x%02x\n",bit_0x,ledstatus);
	}
}



/*---------------------------------------------------------------------------+
|  CommonFunction : ChangeImage			   	                          yuc<080117>
|
|		If no need flash ,will like this:
|	ChangeImage(pwndBmpMode, 	sz, 	NO_FLASH,	0);
|
|		common use:
|	ChangeImage(pwndBmpMode, 	sz, 	Yes_FLASH,	TIMER_STEP1);
+---------------------------------------------------------------------------*/

void	ChangeImage(CtmWnd* pimage, char* pszpath, WORD FLASH_FLAG, WORD TIMER_NUM)
{
	char pszTemp[256];

	if (pimage != NULL)
	{
		pimage->GetPropValueT("imagepath", pszTemp,sizeof(pszTemp));
//+++++++++++++++++++++++++++++++++++++++++++++++++ + + + + + + + + + + + + + + + + + + +
//yuc<080117>
		if(TIMER_NUM <=0 || TIMER_NUM >=MAX_TIMER)
			TIMER_NUM=0;		//for protect
 		if ((pszpath == NULL)) //no need flash
		{
			pimage->SetPropValueT("imagepath", pszpath);
			pimage->Update();
			return;
		}
		if ((FLASH_FLAG ==YES_FLASH))	//flash flag--:yes:flash the image,  no:not flash
		{
			if(u_lTimerValue[TIMER_NUM] <= 0)   	// check timer value
			{
				u_wDisplayFlag[TIMER_NUM] ^= 1;		//Key here
				u_lTimerValue[TIMER_NUM] = 200;		//Delay
			}
			if ( (u_wDisplayFlag[TIMER_NUM] ==0))
			{
				pszpath = NULL;
				pimage->SetPropValueT("imagepath", pszpath);
				pimage->Update();
				return;
			}
		}
		if (((FLASH_FLAG ==YES_FLASH) && (u_wDisplayFlag[TIMER_NUM] ==1)) || ((FLASH_FLAG ==NO_FLASH))) //Display image
		{
			//add program here
//+++++++++++++++++++++++++++++++++++++++++++++++++ + + + + + + + + + + + + + + + + + + +
			if ((pszTemp != NULL && pszTemp[0] != '\0'))
			{
				if (strcmp(pszTemp, pszpath) != 0)
				{
					pimage->SetPropValueT("imagepath", pszpath);
					pimage->Update();
				}
			}
			else if ((pszTemp == NULL || pszTemp[0] == '\0'))
			{
				pimage->SetPropValueT("imagepath", pszpath);
				pimage->Update();
			}
			else if ((pszTemp != NULL && pszTemp[0] != '\0'))
			{
				pimage->SetPropValueT("imagepath", pszpath);
				pimage->Update();
			}
		}
	}
}


void		SetEditValue(CtmWnd* pwnd)
{
	char	pDataID[256];
	long    temp=0;
	if(pwnd != NULL)
	{
		pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
		if(!(strcmp(pDataID,"SYSX_OTHERS_OTHERS_DWORD_RESERVED6")))
			temp = GetDBValue(pDataID).lValue * GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED2").lValue;
		else
			temp = GetDBValue(pDataID).lValue;
		if(pDataID != NULL && pDataID[0] != '\0')
		{
			pwnd->SetPropValueT("value",temp);
			pwnd->Update();
		}
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