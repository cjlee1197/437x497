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
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b
#define		EncWord 				0xFFFF0000 // High Word
#define 	MechWord 				0x0000FFFF // Low Word

#define 		CNT_INT_DEMO_TIMER 		2000;//1000; 50
#define 		GPIO_LED_DEFINE  		0x07;//1000; 50
#define 		GPIO_BIT_AUTO 0x01
#define 		GPIO_BIT_ALARM 0x02
#define 		GPIO_BIT_ENABLE 0x03

#define			GPIO_KEY_BIT	0xb
#define			GPIO_KEY_TEACH 0x8 //cjlee 
#define			GPIO_KEY_STOP 0x9 // �Ȯɭק�Ҧ� 2018/12/14 �U�� 04:09:04
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
#define			SINGLE_PAUSE 0x22 // ���A ��B�Ȱ� 
#define			AUTO_PAUSE 0x24 // ���A �۰ʼȰ�

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
#define			STATE_STOP							0
#define     STATE_FULLAUTO          4 

#define		REQ_READMOTOR    	 2   
#define		DB_TP						1 // ��ܥܱо��ѼƸ��
#define		DB_CON					2 // ��ܱ���ѼƸ��
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
DWORD dw_MechType = 0; //���� pMechTypeDB ���ƭ�
int u_PickerType = 0,u_PickerType_old=-1; // ������� 0-�T�b 1-���b
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b

char* pWhichAxisDB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_TYPE"; // �������@�b DB

WORD			u_wError		= FALSE;					// control.cpp
//<< autodemo
extern	int     g_nDemo;									//	�аODemo�A0���b�۰�Demo���A�A1���b�DDemo���A    
static  int     u_nKeyIndex		= 0;
BOOL			u_bPCLinkFlag	= FALSE;
BOOL			u_bRemote		= FALSE;
BOOL			u_biNet			= FALSE;
int				work_coord_no = -1;
int				tool_coord_no = -1;

long      lSpeedValueOld=0; //2019/5/9 �U�� 09:25:00
long      lStatusValueOld=7; //2019/5/11 �U�� 08:56:12

DWORD			u_dwGestureStatus = 0;
DWORD			u_wPickerOPSatus  = 0,u_wPickerOPSatus_Old;

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
	"ROBOT_BTN_MANUAL",			//�T��
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
	 NULL,										// 0 �L�Ҧ�
	"picker/BackPoint.bmp", 	// 1 �^���I
	"picker/right.bmp", 			// 2 ��B                  		
	"picker/Hand.bmp",				// 3 ���
	"picker/Auto.bmp",	      // 4 �۰�
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
	"picker/BackPoint.bmp",   // 17 �^���I �ǳ�
	"picker/right.bmp",       // 18 ��B �ǳ�
	 NULL,                    // 19
	"picker/Auto.bmp",        // 20 �۰� �ǳ�
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
	"picker/RightPause.bmp",  // 34 ��B �Ȱ�
	 NULL,                    // 35
	"picker/AutoPause.bmp",   // 36 �۰� �Ȱ�
};
char*	u_pszMoveModeImagePath[] = // ��ʲ��ʼҦ�  �T�� �T�� �s��� 
{
	 NULL,										// 0 �T��
	"picker/JogMove.bmp", 		// 1 �T��
	"picker/ContinueMove.bmp",// 2 �s���
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

/*�ѼƤ��*/
char* ParamStr[] = // �ѼƤ�r 
{
	/*���c�Ѽ�*/
	"MECH_MECHTYPE", //0 �������
	"TRANS_TYPE",		 //1 �ǰʤ覡
	"MECH_ENCTYPE",	 //2 �s�X�����
	"MOTOR_ENC_REZ", //3 �s�X���ѪR��
	"MOTOR_RATIO",	 //4 ��t��
	"MECH_GEAR_D",	 //5 �C��Z��
	"MECH_DOUB_MECH",//6 ���ƾ��c 
	"MECH_POS_INV",	 //7 ��m�ϦV
	/*�t�װѼ�*/
	"SPEED_MAX",	 // �̰���t
	"SPEED_ACC_T",	 // �[�t�ɶ�
	"SPEED_DCC_T",	 // ��t�ɶ�
	"SPEED_RATIO",	 // ���Ƥ��
	"SPEED_POS_KP",	 // ����T��
	"SPEED_SPEED_KP",	 // �l�H�T��
	/*�ϰ�]�w*/
	"ZONELIMIT_Zone",	 // ���ʳ̤j��m
	"ZONELIMIT_X1_MIN",	 // 1.���\�U���Ҥ��̤p��m
	"ZONELIMIT_X1_MAX",	 // 2.���\�U���Ҥ��̤j��m
	"ZONELIMIT_Y1_MIN",	 // 3.���\��X�̤j����
	"ZONELIMIT_Y1_MAX",	 // 4.���\���ҳ̤j�ݾ�����
	"ZONELIMIT_Z_MIN",	 // 5.���\�U���Ҥ��̤j��m
	"ZONELIMIT_Z_MAX",	 // 6.���\�m�����~�̤p��m
	"ZONELIMIT_X2_MIN",	 // 7.���\�U���Ҥ��̤p��m
	"ZONELIMIT_X2_MAX",	 // 8.���\�U���Ҥ��̤j��m
	"ZONELIMIT_Y2_MIN",	 // 9.���\��X�̤p����
	"ZONELIMIT_Y2_MAX",	 // 10.���\���ҳ̤j�ݾ�����
	"ARM_SAFE_DIS",	 // �D���u�w���Z��
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
};
char* ParamStr_Mech[] = // �ѼƤ�r  /*���c�Ѽ�*/
{
	"MOTOR_ENC_REZ", // �s�X���ѪR��
	"MOTOR_RATIO",	 // ��t��
	"MECH_GEAR_D",	 // �C��Z�� 
	"MECH_TOOTHM_NUM",// ����
	"MECH_TOOTHM_NUM"	 // �Ҽ�
};
char* ParamStr_Speed[] = // �ѼƤ�r  /*�t�װѼ�*/
{
	"SPEED_MAX", 			// �̰���t
	"SPEED_ACC_T", 		// �[�t�ɶ�
	"SPEED_DCC_T",		// ��t�ɶ�
	"SPEED_RATIO", 		// �[�t���Ƥ��
	"SPEED_RATIO",		// ��t���Ƥ��
	"SPEED_POS_KP",		// ����T�� ��mKp
	"SPEED_SPEED_KP",	// �l�H�T�� �t��Kp
};

int NumofMechType[] = {3,5}; // �T���b���������b��
char* dbid_MechType[] = 
{
	"MACHINE_CONFIGURATION_MACHINETYPE", // �������DB �T�b ���b
	"MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE", // �ǰʤ覡DB
};
/*---------------------------------------------------------------------------+
| u_pszMechPara �ǰʤ覡  																									 |
|							   _____________________________________        							 |	
|   						|	Bit |				0				|				1				| 										 |		
|								|	 0	|	�ϥθ`�ꪽ�|	|	�ϥξ��ƼҼ�  |       							 |
|								|	 1	|			���ϥ� 		|	�ϥέ��ƾ��c  |       							 |
|								|__2__|_____���ϥ�____|_�ϥΦ�m�ϦV__|       							 |
|													        																					 |
+---------------------------------------------------------------------------*/
char*	u_pszMechPara[] =
{
	"MACHINE_FUNCTION_OPTIONS_RSV07", // X1
	"MACHINE_FUNCTION_OPTIONS_RSV08", // Y1
	"MACHINE_FUNCTION_OPTIONS_RSV09", // Z
	"MACHINE_FUNCTION_OPTIONS_RSV10", // X2
	"MACHINE_FUNCTION_OPTIONS_RSV11"  // Y2
}; 
char*	u_pszEditDoubleDB[] = // �������ƾ��c�O�_�ϥ�DB�W��
{
	"",
	"PICKER_PARAMETER_DOUBLE_X1",
	"PICKER_PARAMETER_DOUBLE_Y1",
	"PICKER_PARAMETER_DOUBLE_Z",
	"PICKER_PARAMETER_DOUBLE_X2",
	"PICKER_PARAMETER_DOUBLE_Y2"
}; 
char*	u_pszEditPOSINVDB[] = // ������m�ϦV�O�_�ϥ�DB�W��
{
	"",
	"PICKER_PARAMETER_POS_INV_X1",
	"PICKER_PARAMETER_POS_INV_Y1",
	"PICKER_PARAMETER_POS_INV_Z",
	"PICKER_PARAMETER_POS_INV_X2",
	"PICKER_PARAMETER_POS_INV_Y2"
}; 

char* Mech_Data_String[] = // ���c�Ѽ� ���� �W��
{
	"edit_ENC_REZ", 				// �s�X���ѪR��
	"edit_Motor_Ratio", 	  // ��t��
	"edit_Motor_Diameter",	// �`�ꪽ�|
	"edit_Tooth_Num",				// ����
	"edit_M_Num",						// �Ҽ�
};
char* dbid_Mech[5][5] = // ���c�ѼƼƭ� db [�b][�Ѽ�]
{	//  �s�X���ѪR��									,��t��														,�`�ꪽ�|													,����														 ,�Ҽ�
	{"MACHINE_INTERFACE_CONTROL_RSV05","MACHINE_INTERFACE_CONTROL_RSV10","MACHINE_FUNCTION_OPTIONS_RSV22","MACHINE_FUNCTION_OPTIONS_RSV12","MACHINE_FUNCTION_OPTIONS_RSV17"},		// X1�b
	{"MACHINE_INTERFACE_CONTROL_RSV06","MACHINE_INTERFACE_CONTROL_RSV11","MACHINE_FUNCTION_OPTIONS_RSV23","MACHINE_FUNCTION_OPTIONS_RSV13","MACHINE_FUNCTION_OPTIONS_RSV18"},		// Y1�b
	{"MACHINE_INTERFACE_CONTROL_RSV07","MACHINE_INTERFACE_CONTROL_RSV12","MACHINE_FUNCTION_OPTIONS_RSV24","MACHINE_FUNCTION_OPTIONS_RSV14","MACHINE_FUNCTION_OPTIONS_RSV19"},		// Z�b
	{"MACHINE_INTERFACE_CONTROL_RSV08","MACHINE_INTERFACE_CONTROL_RSV13","MACHINE_FUNCTION_OPTIONS_RSV25","MACHINE_FUNCTION_OPTIONS_RSV15","MACHINE_FUNCTION_OPTIONS_RSV20"},		// X2�b
	{"MACHINE_INTERFACE_CONTROL_RSV09","MACHINE_INTERFACE_CONTROL_RSV14","MACHINE_FUNCTION_OPTIONS_RSV26","MACHINE_FUNCTION_OPTIONS_RSV16","MACHINE_FUNCTION_OPTIONS_RSV21"}			// Y2�b
};
char* Speed_Data_String[] = // �t�װѼ� ���� �W��
{
	"edit_MaxSpeed", 	// �̰���t
	"edit_ACC_T", 	  // �[�t�ɶ�
	"edit_DCC_T",			// ��t�ɶ�
	"edit_ACC_Ratio", // �[�t���Ƥ��
	"edit_DCC_Ratio",	// ��t���Ƥ��
	"edit_POS_KP",	// ����T�� ��mKp
	"edit_SPD_KP",			// �l�H�T�� �t��Kp
};
char* dbid_Speed[5][7] = // �t�װѼƼƭ� db [�b][�Ѽ�]
{	//
	{ "MACHINE_INTERFACE_MAXSPEED_X1",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_X1",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_X1", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV1", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV6", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_X1", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_X1"	// �l�H�T�� �t��KP,
		},		// X1�b                     
	{ "MACHINE_INTERFACE_MAXSPEED_Y1",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_Y1",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_Y1", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV2", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV7", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_Y1", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Y1"	// �l�H�T�� �t��KP,
		},		// Y1�b 
	{ "MACHINE_INTERFACE_MAXSPEED_Z",				// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_Z",			// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_Z", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV3", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV8", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_Z", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Z"	// �l�H�T�� �t��KP,
		},		// Z�b  
	{ "MACHINE_INTERFACE_MAXSPEED_X2",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_X2",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_X2", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV4", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV9", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_X2", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_X2"	// �l�H�T�� �t��KP,
		},		// X2�b                     
	{ "MACHINE_INTERFACE_MAXSPEED_Y2",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_Y2",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_Y2", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV5", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV10", 	// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_Y2", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Y2"	// �l�H�T�� �t��KP,
		},		// Y2�b 
};
char* ZoneLimit_String[6][3] = // �ϰ���ܤ�r [�b][�Ѽ�]
{
	{"VW_MAXPOSLIMIT","PICKER_SAFEDIST_BUFFER","POS_TOLERANCE"},	// �@�P
	{"ZONELIMIT_Zone","ZONELIMIT_X1_MIN","ZONELIMIT_X1_MAX"},			// X1�b
	{"ZONELIMIT_Zone","ZONELIMIT_Y1_MIN","ZONELIMIT_Y1_MAX"},			// Y1�b
	{"ZONELIMIT_Zone","ZONELIMIT_Z_MIN","ZONELIMIT_Z_MAX"},				// Z�b
	{"ZONELIMIT_Zone","ZONELIMIT_X2_MIN","ZONELIMIT_X2_MAX"},			// X2�b
	{"ZONELIMIT_Zone","ZONELIMIT_Y2_MIN","ZONELIMIT_Y2_MAX"}			// Y2�b
};
char* dbid_ZoneLimit[6][3] = // �ϰ�ѼƼƭ� db [�b][�Ѽ�]
{
	{"SYSX_OTHERS_OTHERS_DWORD_RESERVED4","MACHINE_LIMITS_AXIS1_CONTROLPRECISION","MACHINE_LIMITS_AXIS2_CONTROLPRECISION"},	// �@�P
	{"MACHINE_LIMITS_AXIS1_MAXPOSTION","MACHINE_LIMITS_AXIS_X1_MINPOSTION","MACHINE_LIMITS_AXIS_X1_MAXPOSTION"},						// X1�b
	{"MACHINE_LIMITS_AXIS2_MAXPOSTION","MACHINE_LIMITS_AXIS_Y1_MINPOSTION","MACHINE_LIMITS_AXIS_Y1_MAXPOSTION"},						// Y1�b
	{"MACHINE_LIMITS_AXIS3_MAXPOSTION","MACHINE_LIMITS_AXIS_Z_MINPOSTION","MACHINE_LIMITS_AXIS_Z_MAXPOSTION"},							// Z�b
	{"MACHINE_LIMITS_AXIS4_MAXPOSTION","MACHINE_LIMITS_AXIS_X2_MINPOSTION","MACHINE_LIMITS_AXIS_X2_MAXPOSTION"},						// X2�b
	{"MACHINE_LIMITS_AXIS5_MAXPOSTION","MACHINE_LIMITS_AXIS_Y2_MINPOSTION","MACHINE_LIMITS_AXIS_Y2_MAXPOSTION"}							// Y2�b
};
char* dbid_Mode[] = // �Ҧ���� db 
{
	"MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE",	// ��X���A
	"MACHINE_FUNCTION_OPTIONS_MOLD_POSE",		// �Ҥ����A
	"MACHINE_FUNCTION_OPTIONS_RSV01"				// �g�X���۰ʰ���
};
char* ParamStr_Mode[] = // �Ҧ���� ��ܤ�r
{
	"PICKER_POSE_RUN",				// ��X���A
	"PICKER_POSE_INMOLD",			// �Ҥ����A
	"PICKER_MACHINE_AUTOSTOP"	// �g�X���۰ʰ���
};
char* dbid_Detect[] = // �˴��]�w db 
{
	"MACHINE_INTERFACE_MONITOR_RSV16",											// �}���H�ʰ_�l��m
	"MACHINE_CONFIGURATION_STEPTIMEOUT",										// ��B����̤j�ɶ�
	"MACHINE_CONFIGURATION_VALVE_DETECT_TIME_VERTICAL",			// ����˴� ����
	"MACHINE_CONFIGURATION_VALVE_DETECT_TIME_HORIZONTAL",		// ����˴� ����
	"DBID_MACHINE_CONFIGURATION_VALVE_DETECT_TIME_B_PLUS",	// ����˴� B+
	"MACHINE_CONFIGURATION_VALVE_DETECT_TIME_B_MINUS",			// ����˴� B-
	
};
char* ParamStr_Detect[] = // �˴��]�w ��ܤ�r
{
	"VW_PICKERDN_POSN",			// �}���H�ʰ_�l��m
	"PICKER_STEPMAXTIME",		// ��B����̤j�ɶ�
	"PICKER_VERTICAL",			// ����˴� ����
	"PICKER_HORIZONTAL",		// ����˴� ����
	"PICKER_B_POS",					// ����˴� B+
	"PICKER_B_NEG",					// ����˴� B-
};
char* dbid_Prod[] = // �Ͳ��Ѽ� db 
{
	"MACHINE_CONFIGURATION_CAROUSELOPERATIONTIME",	// ��e�a�ɶ�
};
char* ParamStr_Prod[] = // �Ͳ��Ѽ� ��ܤ�r
{
	"VW_OTHER_COUNTPERUNIT",		// ��Ҳ��~�`��
	"PICKER_PROD_CONV_TIME",		// ��e�a�ɶ�
};

char*	dbid_Pile[4][8] =  // ���|�Ѽ� db
{/*����													,��V												,X�Ӽ�															,X���Z															,Y�Ӽ�														,Y���Z															,Z�Ӽ�														,Z���Z*/	
 {"MACHINE_FUNCTION_PILE1_ORDER","MACHINE_FUNCTION_PILE1_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT1","MACHINE_FUNCTION_XAXIS_PILE_DIS1","MACHINE_FUNCTION_YAXIS_PILE_CNT1","MACHINE_FUNCTION_YAXIS_PILE_DIS1","MACHINE_FUNCTION_ZAXIS_PILE_CNT1","MACHINE_FUNCTION_ZAXIS_PILE_DIS1"}, //�Ĥ@��
 {"MACHINE_FUNCTION_PILE2_ORDER","MACHINE_FUNCTION_PILE2_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT2","MACHINE_FUNCTION_XAXIS_PILE_DIS2","MACHINE_FUNCTION_YAXIS_PILE_CNT2","MACHINE_FUNCTION_YAXIS_PILE_DIS2","MACHINE_FUNCTION_ZAXIS_PILE_CNT2","MACHINE_FUNCTION_ZAXIS_PILE_DIS2"}, //�ĤG��
 {"MACHINE_FUNCTION_PILE3_ORDER","MACHINE_FUNCTION_PILE3_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT3","MACHINE_FUNCTION_XAXIS_PILE_DIS3","MACHINE_FUNCTION_YAXIS_PILE_CNT3","MACHINE_FUNCTION_YAXIS_PILE_DIS3","MACHINE_FUNCTION_ZAXIS_PILE_CNT3","MACHINE_FUNCTION_ZAXIS_PILE_DIS3"}, //�ĤT��
 {"MACHINE_FUNCTION_PILE4_ORDER","MACHINE_FUNCTION_PILE4_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT4","MACHINE_FUNCTION_XAXIS_PILE_DIS4","MACHINE_FUNCTION_YAXIS_PILE_CNT4","MACHINE_FUNCTION_YAXIS_PILE_DIS4","MACHINE_FUNCTION_ZAXIS_PILE_CNT4","MACHINE_FUNCTION_ZAXIS_PILE_DIS4"}, //�ĥ|��
};
char* ParamStr_Pile[] = // ���|�Ѽ� ��ܤ�r
{
	"VW_PID_GROUP1",		// �Ĥ@��
	"VW_PID_GROUP2",		// �ĤG��
	"VW_PID_GROUP3",		// �ĤT��
	"VW_PID_GROUP4",		// �ĥ|��
};

char* Manul_String[] = // ��ʰѼ� ���� �W��
{
	"PICKER_FORWARDSPEED", 	// �e�i�t��
	"PICKER_POINTDISTANCE", 	  // �I�ʶZ��
};
char* dbid_Manul[5][2] =  //��ʰѼ� db
{	/*�e�i�t��																									,�I�ʶZ��*/
	{"MACHINE_CONFIGURATION_MANUAL_AXIS1_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS1_MINPOSITION"}, // X1
	{"MACHINE_CONFIGURATION_MANUAL_AXIS2_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS2_MINPOSITION"}, // Y1
	{"MACHINE_CONFIGURATION_MANUAL_AXIS3_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS3_MINPOSITION"}, // Z
	{"MACHINE_CONFIGURATION_MANUAL_AXIS4_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS4_MINPOSITION"}, // X2
	{"MACHINE_CONFIGURATION_MANUAL_AXIS5_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS5_MINPOSITION"}, // Y2
};


int Precision[7][7] = // �b�Ѽ� �p���I��� [�������Ѽ�][�ĴX�ӰѼ�]
{
	{0,0,2,0,0,0,0},	// ���c�Ѽ�
	{0,0,0,0,0,3,3},	// �t�װѼ�
	{2,2,2,0,0,0,0},	// �ϰ�Ѽ�
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,2,0,0,0,0,0},	// ��ʰѼ�
};

//�ѼƼƭ� (��r)
char* ParamStr_TransType[] = // �ǰʤ覡 �ѼƼƭ� (��r)
{
	"MECH_GEAR_D", // �C��Z��
	"MECH_TOOTHM_NUM", // ���ơB�Ҽ�
};
int iTransType=0,iDouble[5]={0},iPosInv[5]={0}; // �ǰʤ覡,���ƾ��c,��m�ϦV

char* ParamStr_Double[] = // ���ƾ��c �ѼƼƭ� (��r)
{
	"MECH_UNUSE", // ���ϥ�
	"MECH_USE", // �ϥ�
};
char* ParamStr_PosInv[] = // ��m�ϦV �ѼƼƭ� (��r)
{
	"VW_ROTATELOC_ANGELREVERSED_OPT1", // ���ϥ�
	"VW_ROTATELOC_ANGELREVERSED_OPT2", // �ϥ�
};
char* ParamStr_PickerType[] = // ������� �ѼƼƭ� (��r)
{
	"MECH_MECHTYPE3", // �T�b
	"MECH_MECHTYPE5", // ���b
};
char* ParamStr_EncType[] = // �s�X����� �ѼƼƭ� (��r)
{
	"MECH_ENCTYPE_ABS", // ���ϥ�
	"MECH_ENCTYPE_RES", // �ϥ�
};
/*�ѼƤ��*/

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

CtmWnd*			u_pwndBmpMoveMode = NULL; // ���ʼҦ��Ϥ� cjlee 2019/7/12 �U�� 05:45:41
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
CtmWnd*			pwndBmpState = NULL; // �����⫺�A cjlee add 2019/5/11 �U�� 08:38:11
CtmWnd*			pwndPickerSpeed = NULL;// ������t��
CtmWnd*			pwndImg_Speed = NULL; // ������t�׹� cjlee add 2019/5/9 �U�� 09:21:24
CtmWnd*			pwndBmpAxis_X2 = NULL; // ������ X2�b cjlee add 2019/10/29 �W�� 11:41:25
CtmWnd*			pwndBmpAxis_Y2 = NULL; // ������ Y2�b cjlee add 2019/10/29 �W�� 11:41:25

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
BOOL				RunOnlyOne				=	FALSE;	//�Q��update�Ȱ���@��
BOOL 				HomingEnable 			= FALSE;

BOOL				CheckOnlyOne			=	FALSE;	//�Q��update�C�s�u�W�Ȱ���@�� 

char 			KeyCodeNow;
/*===========================================================================+
|           Function		                                                 |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("OnCreateA\n");
	printf("CurrentMold=%s\n",CtmMoldSet::GetInstance()->GetCurrentMold());
	
	BOOL	bScreenSaverLCDOFFOption=GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERLCDPOWEROFF").lValue;
	printf("bScreenSaverLCDOFFOption=%d\n",bScreenSaverLCDOFFOption);
	if(bScreenSaverLCDOFFOption)
	{
		system("./SetBL");
		printf("SetBL\n");
	}
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
	pwndBmpAxis_X2	 = pwndSender->FindControlFromName("BmpAxis_X2");
	pwndBmpAxis_Y2	 = pwndSender->FindControlFromName("BmpAxis_Y2");
	
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

			//�iĵ���U�l��
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
		//2012/6/20 �W�� 11:10:18 add for remote>>>>>		
		case	MSG_GPIO_READ:
		{
			HomingEnable =FALSE;
			printf("MSG_GPIO_READ\n");
			// �����_��
			{
			char gpio_key;
			gpio_key = wParam&GPIO_KEY_BIT;
			printf("gpio_key = %d,wParam=%x\n",gpio_key,wParam);
			if(gpio_key==GPIO_KEY_AUTO) // �U
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
			// �I���w�����s
			{
			char gpio_safekey;
			gpio_safekey = wParam&GPIO_SAFEKEY_BIT;
			printf("gpio_safekey = %x\n",gpio_safekey);
			if(gpio_safekey==GPIO_SAFEKEY_0) // ��} ����
				{
					//printf("ACT=STOP\n");
					u_nMANUAL_TYPE = MANUAL_STOP_MODE;
					{ // ����b�ʧ@
						if(KeyCodeNow == 0x83 || KeyCodeNow == 0x93)	//X1�b
						{
							KeyCommand = COMMAND_X1SLOWDOWN;
						}
						else if(KeyCodeNow == 0x84 || KeyCodeNow == 0x94)	//Y1�b
						{
							KeyCommand = COMMAND_Y1SLOWDOWN;
						}
						else if(KeyCodeNow == 0x82 || KeyCodeNow == 0x92)	//Z�b
						{
							KeyCommand = COMMAND_ZSLOWDOWN;
						}
						else if(KeyCodeNow == 0x87 || KeyCodeNow == 0x97)	//X2�b
						{
							KeyCommand = COMMAND_X2SLOWDOWN;
						}
						else if(KeyCodeNow == 0x88 || KeyCodeNow == 0x98)	//Y2�b
						{
							KeyCommand = COMMAND_Y2SLOWDOWN;
						}
						SendCommand(KeyCommand);
					}
				}
			else if(gpio_safekey==GPIO_SAFEKEY_1) // 1�q �I��
				{
					//printf("ACT=JOG\n");
					u_nMANUAL_TYPE = MANUAL_JOG_MODE;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_2) // 2�q �s���
				{
					//printf("ACT=CONTINUE\n");
					u_nMANUAL_TYPE = MANUAL_CONTINUE_MODE;
				}
			}
		}
		break;
		case	MSG_PANEL_KEY: //hotkey
		{
			if(!g_bMute)
				Beep();
			char keycode;
			keycode = wParam;
			KeyCodeNow = keycode;
			printf("panel key = %x\n",keycode);
			int N = (keycode & 0x10)/16;
			if(N==0) N=2;
			printf("N=%d\n",N);
			if((keycode >= 0x82 && keycode <= 0x88) || (keycode >= 0x92 && keycode <= 0x98) || (keycode >= 0x12 && keycode <= 0x18) || (keycode >= 0x02 && keycode <= 0x08))
			{/*-------------------------------�b�ʧ@----------------------------------*/
				printf("Move CMD\n");
				if(keycode == 0x83 || keycode == 0x93)	//X1�b
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 1);
					SetDBValue(pWhichAxisDB,1);
					KeyCommand = (N == 2) ? COMMAND_X_FWD : COMMAND_X_BACK;
					pwndEditPostionX->SetPropValueT("fgc",0xFF80); // ��ܥثe���ʶb ����
				}
				else if(keycode == 0x84 || keycode == 0x94)	//Y1�b
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 2);
					SetDBValue(pWhichAxisDB,2);
					KeyCommand = (N == 2) ? COMMAND_Y_FWD : COMMAND_Y_BACK;
					pwndEditPostionY->SetPropValueT("fgc",0xFF80); // ��ܥثe���ʶb ����
				}
				else if(keycode == 0x82 || keycode == 0x92)	//Z�b
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 4);
					SetDBValue(pWhichAxisDB,3);
					KeyCommand = (N == 2) ? COMMAND_Z_FWD : COMMAND_Z_BACK;
					pwndEditPostionZ->SetPropValueT("fgc",0xFF80); // ��ܥثe���ʶb ����
				}
				else if(keycode == 0x85 || keycode == 0x95)	//C�b ���A
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 8);// cjlee 2019/4/6 �U�� 05:56:23
					KeyCommand = (N == 2) ? COMMAND_C_FWD : COMMAND_C_BACK;
					if(u_nMANUAL_TYPE == MANUAL_STOP_MODE)
						KeyCommand = 0x1234;
				}
				else if(keycode == 0x87 || keycode == 0x97)	//X2�b
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44", 1);// cjlee 2019/4/6 �U�� 05:56:23
					SetDBValue(pWhichAxisDB,4);
					KeyCommand = (N == 2) ? COMMAND_X2_FWD : COMMAND_X2_BACK;
					pwndEditPostionX2->SetPropValueT("fgc",0xFF80); // ��ܥثe���ʶb ����
				}
				else if(keycode == 0x88 || keycode == 0x98)	//Y2�b
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44", 2);// cjlee 2019/4/6 �U�� 05:56:23
					SetDBValue(pWhichAxisDB,5);
					KeyCommand = (N == 2) ? COMMAND_Y2_FWD : COMMAND_Y2_BACK;
					pwndEditPostionY2->SetPropValueT("fgc",0xFF80); // ��ܥثe���ʶb ����
				}
				else if(keycode >= 0x12 || keycode <= 0x18 || keycode >= 0x02 || keycode <= 0x08)	//STOP
				{
					//KeyCommand = COMMAND_STOP; 
					if(keycode == 0x03 || keycode == 0x13)	//X1�b ��}
					{
						KeyCommand = COMMAND_X1SLOWDOWN;
						SetDBValue(pWhichAxisDB,6);
					}
					else if(keycode == 0x04 || keycode == 0x14)	//Y1�b ��}
					{
						KeyCommand = COMMAND_Y1SLOWDOWN;
						SetDBValue(pWhichAxisDB,7);
					}
					else if(keycode == 0x02 || keycode == 0x12)	//Z�b ��}
					{
						KeyCommand = COMMAND_ZSLOWDOWN;
						SetDBValue(pWhichAxisDB,8);
					}
					else if(keycode == 0x07 || keycode == 0x17)	//X2�b ��}
					{
						KeyCommand = COMMAND_X2SLOWDOWN;
						SetDBValue(pWhichAxisDB,9);
					}
					else if(keycode == 0x08 || keycode == 0x18)	//Y2�b ��}
					{
						KeyCommand = COMMAND_Y2SLOWDOWN;
						SetDBValue(pWhichAxisDB,10);
					}
					pwndEditPostionX->SetPropValueT("fgc",0xFFFF);
					pwndEditPostionY->SetPropValueT("fgc",0xFFFF);
					pwndEditPostionZ->SetPropValueT("fgc",0xFFFF);
					FGC FgcTemp = ( u_PickerType==MechType5 ? 0xFFFF : 63519 );
					pwndEditPostionX2->SetPropValueT("fgc",FgcTemp);
					pwndEditPostionY2->SetPropValueT("fgc",FgcTemp);
				}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43", 0);
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44", 0);
				}
				/*-----------------------------------�B�ʩR�O--------------------------------------*/
				if((KeyMode == MANUAL) && (g_ptaskpicker != NULL)) // ��ʼҦ��U
				{
					if(u_nMANUAL_TYPE != MANUAL_STOP_MODE) // �w�����s�����U
						SendCommand(KeyCommand);
					else // �Ы��w���}��
						{
						 //MsgBox(g_MultiLanguage["PICKER_SAFEEYNOTPRESS"], tmFT_CODE_TECH);
						 MsgBoxCall("msgboxConfirm.txt","PICKER_SAFEEYNOTPRESS");			
						 pwndEditPostionX->SetPropValueT("fgc",0xFFFF);
							pwndEditPostionY->SetPropValueT("fgc",0xFFFF);
							pwndEditPostionZ->SetPropValueT("fgc",0xFFFF);
							FGC FgcTemp = ( u_PickerType==MechType5 ? 0xFFFF : 63519 );
							pwndEditPostionX2->SetPropValueT("fgc",FgcTemp);
							pwndEditPostionY2->SetPropValueT("fgc",FgcTemp);
						}	
				}
				/*-----------------------------------�B�ʩR�O--------------------------------------*/
			}/*-------------------------------�b�ʧ@----------------------------------*/
			
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
					HomingEnable =TRUE;
				}
				
			if((keycode == 0x91) || (keycode == 0x81) || (keycode == 0x11) || (keycode == 0x01))
			{ /*-------------------------------�ҰʡB����----------------------------------*/
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
				SendCommand(RunStopCmd);
			} /*-------------------------------�ҰʡB����----------------------------------*/
			
			if((keycode==0x99)||(keycode ==0x89)) //�t�׼W��
			{
				if((KeyMode != AUTO) || ((KeyMode == AUTO) && (u_nMANUAL_TYPE != MANUAL_STOP_MODE))) // �۰ʼҦ��U // �w�����s�����U
					{				
						long lTempValue = GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue;
						if((lTempValue < 100)&&(keycode ==0x99))
							SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lTempValue+5);
						if((lTempValue > 0)&&(keycode ==0x89))
							SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lTempValue-5);
					}
				else if(KeyMode == AUTO && (u_nMANUAL_TYPE == MANUAL_STOP_MODE) ) // �Ы��w���}��
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

	// cjlee test 2019/3/22 �W�� 11:01:55
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
	//------------HMI�Ҧ����A---------------//
	
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
		//if((g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP) && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE)  	//����ɱj��auto���A
		{
			int mode_lgt  = OP_MEMORY_MODE;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_mode_lgt,&mode_lgt,sizeof(g_TxCNCData.shmCNCData.CNC_mode_lgt));
		}
		else if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_STOP && g_pRxCNCData->CNC_status==M_READY && g_TxCNCData.shmCNCData.CNC_mode_lgt != RobotStatus)								//�������ܥثe���A
		{
			int mode_lgt = RobotStatus;
			if(mode_lgt==OP_MEMORY_MODE) mode_lgt=OP_EDIT_MODE;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_mode_lgt,&mode_lgt,sizeof(g_TxCNCData.shmCNCData.CNC_mode_lgt));
		}
	}
*/

	if(RunOnlyOne)
{/*-----------------------------------�����Ҧ�----------------------------------*/
			if(KeyMode != Last_KeyMode) // �����_��
				{
					printf("New KeyMode\n");
					if(KeyMode == MANUAL) // ���
					{
						//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 3);
						SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
						SendCommand(MODE_MANUAL);
					}
					else if(KeyMode == AUTO) // �۰� 
					{
						//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 4);
						SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
						SendCommand(MODE_AUTO);
					}
					else if(KeyMode == REMOTE) // ����
					{
						//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 2);// ��B
						SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
						//SendCommand(0xf400);//��B
						SendCommand(MODE_NULL);//����
					}
	
					Last_KeyMode = KeyMode;
				}
			if(u_nMANUAL_TYPE != Last_u_nMANUAL_TYPE) // �I���w�����s
				{
					printf("New SafeKey Mode\n");
					SetDBValue("MACHINE_CONFIGURATION_MANUAL_TYPE",u_nMANUAL_TYPE);
					printf("Safe Key =%d\n",u_nMANUAL_TYPE);
					Last_u_nMANUAL_TYPE = u_nMANUAL_TYPE;
				}	
}/*-----------------------------------�����Ҧ�----------------------------------*/
	

	if(!RunOnlyOne) // �}���u�B��@��
	{
		printf("RunOnlyOne\n");
	/*-----------------------------------�����t��----------------------------------*/
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
	/*-----------------------------------�����t��----------------------------------*/
	
		//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 0);
		if(KeyMode == MANUAL) // �b��ʤ~�e ��L�L�Ҧ�
		{
			//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 3);
			SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
			WORD u_PickerState = GetDBValue("MACHINE_INTERFACE_CONTROL_RSV15").lValue;
			printf("STATE=%d\n",u_PickerState);
			SendCommand(MODE_MANUAL);
		}
		else if(KeyMode == AUTO) // �۰� 
		{
			//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 4);
			SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
		}
		else if(KeyMode == REMOTE) 
		{
			//SetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE", 2);// ��B
			SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
		}
		Last_KeyMode = KeyMode;
		Last_u_nMANUAL_TYPE = u_nMANUAL_TYPE;
		RunOnlyOne=TRUE;
	}

	//----------�{�b�ɶ�---------------//
	if (u_lDateTimer <= 0)
	{	
			GetTime(&time);  		
   		GetDate(&date);

   		sprintf(sz, "%04u.%02u.%02u/%02u:%02u", date.year, date.month, date.day, time.hour, time.minute);
      g_pMainFrame->GetPromptBar()->GetControlA(0)->Clear();
      g_pMainFrame->GetPromptBar()->GetControlA(0)->ShowExtent(0, 1, sz, 0);
		
		u_lDateTimer= CNT_INT_UPDATE_DATE_TIMER;
	}

{/*-----------------------------------�����y��----------------------------------*/
	SetEditValue(pwndEditPostionX);
	SetEditValue(pwndEditPostionY);
	SetEditValue(pwndEditPostionZ);
	SetEditValue(pwndEditPostionX2);
	SetEditValue(pwndEditPostionY2);	
}/*-----------------------------------�����y��----------------------------------*/
	
{/*-----------------------------------�����t��----------------------------------*/
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
}/*-----------------------------------�����t��----------------------------------*/
	
{/*------------------------------------�Ͳ���T-----------------------------------*/
	SetEditValue(pwndEditOPNCNT); 
	SetEditValue(pwndEditTOTALT);
	SetEditValue(pwndEditACTT);
	if(pwndEditTOTALT!=NULL)
	{
		pwndEditTOTALT->UpdateAll();
	}
}/*------------------------------------�Ͳ���T-----------------------------------*/
	
{/*-----------------------------------����⪬�A----------------------------------*/
	long lpickerStatus = g_ptaskpicker->GetOnLineStatus();
	if(!lpickerStatus)  lpickerStatus = 1;
	else		 		lpickerStatus = 0;
	//printf("lpickerStatus=%d\n",lpickerStatus);	
	
	if(g_BkScreenSaver==1) // �P�_�q�ù��O�@�{���^�� �_���ܴ�
		{
			printf("Key Change\n");
			printf("KeyMode=%d\n",KeyMode);
			int KeyState = GetDBValue("MACHINE_INTERFACE_CONTROL_RSV16").lValue;
			printf("KeyState=%d\n",KeyState);
			if(KeyState == GPIO_KEY_STOP) // ���
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
				SendCommand(MODE_MANUAL);
			}
			else if(KeyState == GPIO_KEY_AUTO) // �۰� 
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
				SendCommand(MODE_AUTO);
			}
			else if(KeyState == GPIO_KEY_TEACH) // ����
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
				SendCommand(MODE_NULL);//����
			}
			g_BkScreenSaver = 0; // Set Flag Back From Screen Saver
		}

	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
	//printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);	
	if(u_wPickerOPSatus!=u_wPickerOPSatus_Old) // ���A�o���ܤ�
	{
		printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);
		if(u_wPickerOPSatus==STATE_FULLAUTO)
		{
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
			strcpy(g_szCurrentFormName, "Over.txt");
			::PutCommand("Over.txt");
		}
		else if(u_wPickerOPSatus==STATE_STOP)
		{
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
			strcpy(g_szCurrentFormName, "Index.txt");
			::PutCommand("Index.txt");
		}
		else if(u_wPickerOPSatus==STATE_HAND)
		{
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
			strcpy(g_szCurrentFormName, "Hand.txt");
			::PutCommand("Hand.txt");
		}
		u_wPickerOPSatus_Old=u_wPickerOPSatus;
	}
	
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
}/*-----------------------------------����⪬�A----------------------------------*/

{/*-----------------------------------�ѼƤ��----------------------------------*/
	if(g_ptaskpicker != NULL)
	{
		int iPickerConnect = g_ptaskpicker->GetOnLineStatus();
		if(!iPickerConnect)  iPickerConnect = 1;
		else		 		iPickerConnect = 0;
		//printf("iPickerConnect=%d\n",iPickerConnect);
		if(iPickerConnect && CheckOnlyOne == FALSE) // �C���s���W �ˬd�@���Ѽ�
		{			
			printf("Connect\n");
			CheckOnlyOne=TRUE;
			GetValueFrom28(); // �ѼƤ��
			::PutCommand(g_szCurrentFormName);			
		}
		if(!iPickerConnect)
			CheckOnlyOne = FALSE;
	}
}/*-----------------------------------�ѼƤ��----------------------------------*/


{/*----------------------------------��ʲ��ʼҦ�---------------------------------*/
	if((u_nMANUAL_TYPE == MANUAL_JOG_MODE) && (u_wPickerOPSatus == STATE_HAND)) // 1�q �I��
	{
		sprintf(sz,"%s%s", szPath, u_pszMoveModeImagePath[1]);
		//printf("u_pszMoveModeImagePath=%s\n",sz);	
		ChangeImage(u_pwndBmpMoveMode, sz, NO_FLASH, 0);
	}
	else if((u_nMANUAL_TYPE == MANUAL_CONTINUE_MODE) && (u_wPickerOPSatus == STATE_HAND)) // 2�q �s���
	{
		sprintf(sz,"%s%s", szPath, u_pszMoveModeImagePath[2]);
		//printf("u_pszMoveModeImagePath=%s\n",sz);	
		ChangeImage(u_pwndBmpMoveMode, sz, NO_FLASH, 0);
	}
	else
	{
		ChangeImage(u_pwndBmpMoveMode, NULL,NO_FLASH, 0);
	}
}/*-----------------------------------����⪬�A----------------------------------*/

//{/*-----------------------------------�����Ȱ�----------------------------------*/	
//	if((u_wPickerOPSatus == SINGLE_PAUSE) || (u_wPickerOPSatus==AUTO_PAUSE))
//	{
//		u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
//		printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);	
//		MsgBoxCall("msgboxPause.txt");
//	}
//}/*-----------------------------------�����Ȱ�----------------------------------*/

{/*-------------------------------------���A----------------------------------------*/
	u_dwGestureStatus = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE").lValue;	
//	if(pwndStaticPose!=NULL)
//	{
//		if(u_wGestureStatus & 0x0001) //0x0100
//			pwndStaticPose ->SetPropValueT("textID", "PICKER_VERTICAL");
//		if(u_wGestureStatus & 0x0002) //0x0200
//			pwndStaticPose ->SetPropValueT("textID", "PICKER_HORIZONTAL");
//		pwndStaticPose->Update();
//	}
	long lStatusValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE").lValue;
	if(lStatusValue != lStatusValueOld)
	{
		if(pwndBmpState!=NULL) // ���A�ϥ�
		{
			if(u_dwGestureStatus & 0x0001) //0x0100
				pwndBmpState ->SetPropValueT("imagepath", "res_tm640/pic/Axis_C_V.bmp");
			if(u_dwGestureStatus & 0x0002) //0x0200
				pwndBmpState ->SetPropValueT("imagepath", "res_tm640/pic/Axis_C_H.bmp");
			pwndBmpState->Update();
		}
		lStatusValueOld=lStatusValue;
	}

	
}/*-------------------------------------���A----------------------------------------*/
	
{/*-------------------------------------�ϯ�----------------------------------------*/
	WORD wEnableStatus = GetDBValue("MACHINE_INTERFACE_MOTORENABLESTATE").lValue;
	//printf("wEnableStatus=%d\n",wEnableStatus);
	if(wEnableStatus < sizeof(u_pszPickerEnableImagePath)/sizeof(char *))
	{
		sprintf(sz,"%s%s",szPath,u_pszPickerEnableImagePath[wEnableStatus]);
		//printf("u_pszPickerEnableImagePath=%s\n",sz);
		ChangeImage(pwndBmpPickerEnable, sz, NO_FLASH, 0);
	}
}/*-------------------------------------�ϯ�----------------------------------------*/
	#ifdef	D_3354	
{/*-------------------------------------�q�T----------------------------------------*/
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
}/*-------------------------------------�q�T----------------------------------------*/
	#endif
{/*-------------------------------------ĵ��----------------------------------------*/
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
}/*-------------------------------------ĵ��----------------------------------------*/
	
{/*-------------------------------------�s�x----------------------------------------*/
	if((g_wMMCStatus	==	1)||(g_wUSBStatus ==1)) //ZHBA  2009-4-22 for 270 //1:Have mmc  0:Not have  yuc<080118>
	{
		sprintf(sz,"%s%s",szPath,"usb.bmp");
		ChangeImage(pwndBmpUSB, sz, NO_FLASH, 0);
	}
	else
		ChangeImage(pwndBmpUSB, NULL , NO_FLASH, 0);	
}/*-------------------------------------�s�x----------------------------------------*/
	
{/*-------------------------------------�b�y��----------------------------------------*/
	dw_MechType  = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	if(u_PickerType_old != u_PickerType)
	{
		if(u_PickerType==MechType5)
		{
			ChangeImage(pwndBmpAxis_X2, "res_tm640/pic/Axis_X2.bmp", NO_FLASH, 0);
			ChangeImage(pwndBmpAxis_Y2, "res_tm640/pic/Axis_Y2.bmp", NO_FLASH, 0);
			pwndEditPostionX2->SetPropValueT("fgc",0xFFFF);
			pwndEditPostionY2->SetPropValueT("fgc",0xFFFF);
		}
		else if(u_PickerType==MechType3)
		{
			ChangeImage(pwndBmpAxis_X2, NULL , NO_FLASH, 0);	
			ChangeImage(pwndBmpAxis_Y2, NULL , NO_FLASH, 0);
			pwndEditPostionX2->SetPropValueT("fgc",63519);
			pwndEditPostionY2->SetPropValueT("fgc",63519);
		}
		u_PickerType_old = u_PickerType;
	}
}/*-------------------------------------�b�y��----------------------------------------*/
	
	//-------------�u�@/�u��y�����---------------//
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
	//DeleteTimer(&u_lDemoTime);			//	�P���p�ɾ�
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


/*---------------------------------------------------------------------------+
|  Function : GetValueFrom28()                       					    		       |
|  Task     : ���o28�]�w�� ���Ѽ�                                          |
+----------------------------------------------------------------------------+
|  Parameter:   																	                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	GetValueFrom28() // ���o28�]�w�� ���Ѽ�
{
	if(g_ptaskpicker != NULL)
	{
		printf("Get 28 data value\n");
		int i_dbvalue_497[6][5] = {0};// �ѼƼƭ� db// �ܱо��Ȧs��
		int i_dbvalue_different = 0; // �ѼƼƭȮt���Ӽ�
		int iDBSelect=0; // �Ѽƨ̾ڿ��  1:�H�ܱо����D 2:�H������D
		
		WORD wNum = 0;
		int  itemp=0;
		tmParam Param497; // 497 �ܱо��W�Ѽƪ���
		tmParam Param28; // 28 ����W�Ѽƪ���
		int iPre=0;
		
		// �������
		// ����497�ƭ�
		Param497.iMechType = (int)(GetDBValue(dbid_MechType[0]).lValue);
		Param497.iPickerType = Param497.iMechType & MechWord; // ����
		Param497.iEncType		 = (Param497.iMechType & EncWord)>>16; // �s�X��
		
		// �V 28 �ШD���
		wNum = sizeof(dbid_MechType)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,dbid_MechType); // Update Data from 28
		printf("Get = %s\n",dbid_MechType[0]);
		Param28.iMechType = (int)(GetDBValue(dbid_MechType[0]).lValue); // �������
		Param28.iPickerType = Param28.iMechType & MechWord; // ����
		Param28.iEncType		 = (Param28.iMechType & EncWord)>>16; // �s�X��
		
		printf("%s=%d\n",dbid_MechType[0],Param28.iMechType); // 28����
		printf("497=%d\n",Param497.iMechType); // 497����

		if(Param497.iPickerType != Param28.iPickerType) // 497�M28�T���b��ܤ��P �������
		{
			SprintfDBValue(ParamStr_PickerType[Param497.iPickerType],ParamStr_PickerType[Param28.iPickerType]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
			MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_MECHTYPE]]);
			iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
			printf("Choose %d\n",iDBSelect);
			SetChosenDB(dbid_MechType[0],Param497.iMechType,Param28.iMechType,iDBSelect);
			dw_MechType  = (GetDBValue(pMechTypeDB).lValue);
			u_PickerType = dw_MechType & MechWord;
		}		
		if(Param497.iEncType != Param28.iEncType) // 497�M28 �s�X����� ���P
		{
			printf("Param497.iEncType=%d, Param28.iEncType=%d\n",Param497.iEncType,Param28.iEncType);
			SprintfDBValue(ParamStr_EncType[Param497.iEncType],ParamStr_EncType[Param28.iEncType]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
			MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_ENCTYPE]]);
			iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
			printf("Choose %d\n",iDBSelect);
			SetChosenDB(dbid_MechType[0],Param497.iMechType,Param28.iMechType,iDBSelect);
		}		
		
		printf("NumofMechType=%d\n",NumofMechType[u_PickerType]);
		for(int AxisNum=0; AxisNum<NumofMechType[u_PickerType]; AxisNum++) // �̾� NumofMechType[u_PickerType] 3�b��5�b
		{
			printf("AxisNum=%d\n",AxisNum);
			{/*�ǰʤ覡 + ���ƾ��c + ��m�ϦV*/
				// ����497�ƭ� �ǰʤ覡 + ���ƾ��c + ��m�ϦV
				Param497.iTransType = (int)(GetDBValue(u_pszMechPara[AxisNum]).lValue); // �ǰʤ覡 + ���ƾ��c + ��m�ϦV
				
				// �V 28 �ШD��� �ǰʤ覡 + ���ƾ��c + ��m�ϦV
				wNum = 1;
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,&u_pszMechPara[AxisNum]);
				printf("Get = %s\n",u_pszMechPara[AxisNum]);
				Param28.iTransType = (int)(GetDBValue(u_pszMechPara[AxisNum]).lValue);
				
				printf("%s=%d\n",u_pszMechPara[AxisNum],Param28.iTransType); // 28����
				printf("497=%d\n",Param497.iTransType); // 497����  
				
				// ���
				if(Param497.iTransType != Param28.iTransType) // 497�M28 bit 0 �ǰʤ覡 + bit 1 ���ƾ��c + bit 2 ��m�ϦV ���P
				{
					Param497.iDouble[AxisNum] 	= ((Param497.iTransType) & 0x002)>>1; // ���ƾ��c 497
					Param497.iPosInv[AxisNum] 	= ((Param497.iTransType) & 0x004)>>2; // ��m�ϦV 497
					Param28.iDouble[AxisNum] 	= ((Param28.iTransType) & 0x002)>>1; // ���ƾ��c 28
					Param28.iPosInv[AxisNum] 	= ((Param28.iTransType) & 0x004)>>2; // ��m�ϦV 28
					
					printf("TransType Double Inverse diff\n");
					// ����@�� �ǰʤ覡
					if(AxisNum<1) // 497�M28 �ǰʤ覡 ���P Param497.iTransType != Param28.iTransType
					{
						int iTransType497=0,iTransType28=0;
						iTransType497 = int(Param497.iTransType & 0x001); // �ǰʤ覡
						iTransType28 = int(Param28.iTransType & 0x001); // �ǰʤ覡
						if( iTransType497 != iTransType28 )
						{
							printf("TransType diff\n");
							SprintfDBValue(ParamStr_TransType[iTransType497],ParamStr_TransType[iTransType28]);  // �B�zDB�ƭ���ܤ�r
							MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[TRANS_TYPE]]);
							iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
							printf("Choose %d\n",iDBSelect);
							iTransType = (iDBSelect==DB_CON) ? (Param28.iTransType & 0x001):(Param497.iTransType & 0x001);
							printf("TransType=%x\n",iTransType);
						}
					}

					if(Param497.iDouble[AxisNum] != Param28.iDouble[AxisNum]) //  497�M28 ���ƾ��c ���P
						{
							printf("Double diff\n");
							SprintfDBValue(ParamStr_Double[Param497.iDouble[AxisNum]],ParamStr_Double[Param28.iDouble[AxisNum]]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
							MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_DOUB_MECH]]);
							iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
							printf("Choose %d\n",iDBSelect);
							iDouble[AxisNum] = (iDBSelect==DB_CON)?Param28.iDouble[AxisNum]:Param497.iDouble[AxisNum];
							printf("iDouble[%d]=%x\n",AxisNum,iDouble[AxisNum]);
						}
					else
						iDouble[AxisNum]=Param497.iDouble[AxisNum];
					if(Param497.iPosInv[AxisNum] != Param28.iPosInv[AxisNum]) //  497�M28 ��m�ϦV ���P
						{
							printf("PosInv diff\n");
							SprintfDBValue(ParamStr_PosInv[Param497.iPosInv[AxisNum]],ParamStr_PosInv[Param28.iPosInv[AxisNum]]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
							MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_POS_INV]]);
							iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
							printf("Choose %d\n",iDBSelect);
							iPosInv[AxisNum]=(iDBSelect==DB_CON)?Param28.iPosInv[AxisNum]:Param497.iPosInv[AxisNum];
							printf("iPosInv[%d]=%x\n",AxisNum,iPosInv[AxisNum]);
						}		
					else
						iPosInv[AxisNum]=Param497.iPosInv[AxisNum];
										
					int TempValue = iTransType + (iDouble[AxisNum]<<1) + (iPosInv[AxisNum]<<2);
					printf("iTransType=%d,iDouble=%d, iPosInv=%d\n",iTransType,iDouble[AxisNum],iPosInv[AxisNum]);
					printf("iTransType=%d,iDouble=%d, iPosInv=%d\n",iTransType,iDouble[AxisNum],iPosInv[AxisNum]);
					/*----------------------------------------+
					| �ǰʤ覡						  									|
					|	___________0_________________1__________|
					|	bit0 	�ϥΨC��Z��	|		�ϥξ��ƼҼ�		|
					| bit1 		���ϥ�			|		�ϥέ��ƾ��c		|
					| bit2 		���ϥ�			|		�ϥΤϦV��m		|
					+----------------------------------------*/
					printf("Set TransType = %x\n",TempValue);
					SetDBValue(u_pszMechPara[AxisNum],TempValue);
					printf("Get TransType=%d\n",int(GetDBValue(u_pszMechPara[AxisNum]).lValue));
					SetDBValue(u_pszEditDoubleDB[AxisNum+1],iDouble[AxisNum]); // ���ƾ��c DB
					SetDBValue(u_pszEditPOSINVDB[AxisNum+1],iPosInv[AxisNum]); // ��m�ϦV DB
					
					SetChosenDB(u_pszMechPara[AxisNum],Param497.iTransType,Param28.iTransType,iDBSelect);// �g�J (�ǰʤ覡 + ���ƾ��c + ��m�ϦV) �@��DB
				}
			}
			
			printf("\n\nAxis Check!!!!!!\n\n");
			{/*�b�Ѽ�*/
				// ����497�ƭ� �b�Ѽ�
				for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
				{	
					Param497.Axis[AxisNum][i] = (int)(GetDBValue(dbid_Mech[AxisNum][i]).lValue);
				}
				// �V 28 �ШD��� �b�Ѽ�
				wNum = sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Mech[AxisNum]);
				for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
				{	
					Param28.Axis[AxisNum][i] = (int)(GetDBValue(dbid_Mech[AxisNum][i]).lValue);
					printf("Get %s=%d\n",dbid_Mech[AxisNum][i],Param28.Axis[AxisNum][i]); // 28����
					printf("497=%d\n",Param497.Axis[AxisNum][i]); // 497����
					// ��� �b�Ѽ�
					if(Param497.Axis[AxisNum][i] != Param28.Axis[AxisNum][i]) // 497�M28 ���P
					{
						printf("%d is diff\n",i);
						g_DBVale_497=Param497.Axis[AxisNum][i];g_DBVale_28=Param28.Axis[AxisNum][i]; // �ܱо��α���ƭ�
						g_DBPrecision=Precision[PAxis][i]; // �Ѽƺ��
						
						SprintfDBValue(Param497.Axis[AxisNum][i],Param28.Axis[AxisNum][i]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�

						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Mech[i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_Mech[AxisNum][i],Param497.Axis[AxisNum][i],Param28.Axis[AxisNum][i],iDBSelect);
					}
				}
			}
			
			{/*�t�װѼ�*/
				// ����497�ƭ� �t�װѼ�
				for(int i = 0; i < sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]); i++ )
				{	 
					Param497.Speed[AxisNum][i] = (int)(GetDBValue(dbid_Speed[AxisNum][i]).lValue);
				}
				// �V 28 �ШD��� �t�װѼ�
				wNum = sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Speed[AxisNum]);
				for(int i = 0; i < sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]); i++ ) // �ھ� Speed_Data_String ���e�M�w�ѼƼƶq 
				{	
					Param28.Speed[AxisNum][i] = (int)(GetDBValue(dbid_Speed[AxisNum][i]).lValue);
					printf("Get %s=%d\n",dbid_Speed[AxisNum][i],Param28.Speed[AxisNum][i]); // 28����
					printf("497=%d\n",Param497.Speed[AxisNum][i]); // 497����
					// ��� �t�װѼ�
					if(Param497.Speed[AxisNum][i] != Param28.Speed[AxisNum][i]) // 497�M28 ���P
					{
						g_DBVale_497=Param497.Speed[AxisNum][i];g_DBVale_28=Param28.Speed[AxisNum][i]; // �ܱо��α���ƭ�
						g_DBPrecision=Precision[PSpeed][i]; // �Ѽƺ��
						SprintfDBValue(Param497.Speed[AxisNum][i],Param28.Speed[AxisNum][i]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
						
						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Speed[i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_Speed[AxisNum][i],Param497.Speed[AxisNum][i],Param28.Speed[AxisNum][i],iDBSelect);
					}
				}
			}
			
			{/*�ϰ�]�w*/
				printf("Compare ZoneLimit Param\n");
				// ����497�ƭ� �ϰ�]�w�Ѽ�
				for(int i = 0; i < sizeof(ZoneLimit_String[AxisNum+1])/sizeof(ZoneLimit_String[AxisNum+1][0]); i++ )
				{	 
					Param497.ZoneLimit[AxisNum+1][i] = (int)(GetDBValue(dbid_ZoneLimit[AxisNum+1][i]).lValue);
				}
				// �V 28 �ШD��� �ϰ�]�w�Ѽ�
				wNum = sizeof(ZoneLimit_String[AxisNum+1])/sizeof(ZoneLimit_String[AxisNum+1][0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_ZoneLimit[AxisNum+1]);
				for(int i = 0; i < sizeof(ZoneLimit_String[AxisNum+1])/sizeof(ZoneLimit_String[AxisNum+1][0]); i++ ) // �ھ� ZoneLimit_String[AxisNum+1] ���e�M�w�ѼƼƶq 
				{	
					Param28.ZoneLimit[AxisNum+1][i] = (int)(GetDBValue(dbid_ZoneLimit[AxisNum+1][i]).lValue);
					printf("Get %s=%d\n",dbid_ZoneLimit[AxisNum+1][i],Param28.ZoneLimit[AxisNum+1][i]); // 28����
					printf("497=%d\n",Param497.ZoneLimit[AxisNum+1][i]); // 497����
					// ��� �t�װѼ�
					if(Param497.ZoneLimit[AxisNum+1][i] != Param28.ZoneLimit[AxisNum+1][i]) // 497�M28 ���P
					{
						g_DBVale_497=Param497.ZoneLimit[AxisNum+1][i];g_DBVale_28=Param28.ZoneLimit[AxisNum+1][i]; // �ܱо��α���ƭ�
						g_DBPrecision=Precision[PZoneLimit][i]; // �Ѽƺ��
						SprintfDBValue(Param497.ZoneLimit[AxisNum+1][i],Param28.ZoneLimit[AxisNum+1][i]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
						
						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ZoneLimit_String[AxisNum+1][i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_ZoneLimit[AxisNum+1][i],Param497.ZoneLimit[AxisNum+1][i],Param28.ZoneLimit[AxisNum+1][i],iDBSelect);
					}
				}
				
				//�@�P
				// ����497�ƭ� �ϰ�]�w�Ѽ�
				Param497.ZoneLimit[0][1] = (int)(GetDBValue(dbid_ZoneLimit[0][1]).lValue);//�w���Z���w�İ�
				Param497.ZoneLimit[0][2] = (int)(GetDBValue(dbid_ZoneLimit[0][2]).lValue);//��촣�e�Ϋ�
				// �V 28 �ШD��� �ϰ�]�w�Ѽ�
				wNum = sizeof(ZoneLimit_String[0])/sizeof(ZoneLimit_String[0][0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_ZoneLimit[0]);
				
				Param28.ZoneLimit[0][1] = (int)(GetDBValue(dbid_ZoneLimit[0][1]).lValue);
				Param28.ZoneLimit[0][2] = (int)(GetDBValue(dbid_ZoneLimit[0][2]).lValue);
				
				printf("Get %s=%d\n",dbid_ZoneLimit[0][1],Param28.ZoneLimit[0][1]); // 28����
				printf("497=%d\n",Param497.ZoneLimit[0][1]); // 497����				
				if(Param497.ZoneLimit[0][1] != Param28.ZoneLimit[0][1]) // 497�M28 ���P
				{
					g_DBVale_497=Param497.ZoneLimit[0][1];g_DBVale_28=Param28.ZoneLimit[0][1]; // �ܱо��α���ƭ�
					g_DBPrecision=Precision[PZoneLimit][1]; // �Ѽƺ��
					SprintfDBValue(Param497.ZoneLimit[0][1],Param28.ZoneLimit[0][1]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
					
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ZoneLimit_String[0][1]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_ZoneLimit[0][1],Param497.ZoneLimit[0][1],Param28.ZoneLimit[0][1],iDBSelect);
				}
				
				printf("Get %s=%d\n",dbid_ZoneLimit[0][2],Param28.ZoneLimit[0][2]); // 28����
				printf("497=%d\n",Param497.ZoneLimit[0][2]); // 497����
				if(Param497.ZoneLimit[0][2] != Param28.ZoneLimit[0][2]) // 497�M28 ���P
				{
					g_DBVale_497=Param497.ZoneLimit[0][2];g_DBVale_28=Param28.ZoneLimit[0][2]; // �ܱо��α���ƭ�
					g_DBPrecision=Precision[PZoneLimit][2]; // �Ѽƺ��
					SprintfDBValue(Param497.ZoneLimit[0][2],Param28.ZoneLimit[0][2]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
					
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ZoneLimit_String[0][2]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_ZoneLimit[0][2],Param497.ZoneLimit[0][2],Param28.ZoneLimit[0][2],iDBSelect);
				}
			}
			
			{/*��ʰѼ�*/
				// ����497�ƭ� �t�װѼ�
				for(int i = 0; i < sizeof(Manul_String)/sizeof(Manul_String[0]); i++ )
				{	 
					Param497.Manual[AxisNum][i] = (int)(GetDBValue(dbid_Manul[AxisNum][i]).lValue);
				}
				// �V 28 �ШD��� �t�װѼ�
				wNum = sizeof(Manul_String)/sizeof(Manul_String[0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Manul[AxisNum]);
				for(int i = 0; i < sizeof(Manul_String)/sizeof(Manul_String[0]); i++ ) // �ھ� Manul_String ���e�M�w�ѼƼƶq 
				{	
					Param28.Manual[AxisNum][i] = (int)(GetDBValue(dbid_Manul[AxisNum][i]).lValue);
					printf("Get %s=%d\n",dbid_Manul[AxisNum][i],Param28.Manual[AxisNum][i]); // 28����
					printf("497=%d\n",Param497.Manual[AxisNum][i]); // 497����
					// ��� �t�װѼ�
					if(Param497.Manual[AxisNum][i] != Param28.Manual[AxisNum][i]) // 497�M28 ���P
					{
						g_DBVale_497=Param497.Manual[AxisNum][i];g_DBVale_28=Param28.Manual[AxisNum][i]; // �ܱо��α���ƭ�
						g_DBPrecision=Precision[PManual][i]; // �Ѽƺ��
						SprintfDBValue(Param497.Manual[AxisNum][i],Param28.Manual[AxisNum][i]); // �ѼƼƭ� �ഫ���r�� ��DBChoose �ϥ�
						printf("MsgBoxCall\n");
						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[Manul_String[i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_Manul[AxisNum][i],Param497.Manual[AxisNum][i],Param28.Manual[AxisNum][i],iDBSelect);
					}
				}
			}
			
		} // �̾� NumofMechType[u_PickerType] 3�b��5�b
		{/*�Ҧ����*/
			// ����497�ƭ� �Ҧ���� �Ѽ�
			for(int i = 0; i < sizeof(dbid_Mode)/sizeof(dbid_Mode[0]); i++ )
			{	
				Param497.Mode[i] = (int)(GetDBValue(dbid_Mode[i]).lValue);
			}
			// �V 28 �ШD��� �Ҧ���� �Ѽ�
			wNum = sizeof(dbid_Mode)/sizeof(dbid_Mode[0]);
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Mode);
			for(int i = 0; i < sizeof(dbid_Mode)/sizeof(dbid_Mode[0]); i++ )
			{	
				Param28.Mode[i] = (int)(GetDBValue(dbid_Mode[i]).lValue);
				printf("Get %s=%d\n",ParamStr_Mode[i],Param28.Mode[i]); // 28����
				printf("497=%d\n",Param497.Mode[i]); // 497����
				// ��� �b�Ѽ�
				if(Param497.Mode[i] != Param28.Mode[i]) // 497�M28 ���P
				{
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Mode[i]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_Mode[i],Param497.Mode[i],Param28.Mode[i],iDBSelect);
				}
			}
		}
		{/*�˴��]�w*/
			// ����497�ƭ� �˴��]�w �Ѽ�
			for(int i = 0; i < sizeof(dbid_Detect)/sizeof(dbid_Detect[0]); i++ )
			{	
				Param497.Detect[i] = (int)(GetDBValue(dbid_Detect[i]).lValue);
			}
			// �V 28 �ШD��� �˴��]�w �Ѽ�
			wNum = sizeof(dbid_Detect)/sizeof(dbid_Detect[0]);
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Detect);
			for(int i = 0; i < sizeof(dbid_Detect)/sizeof(dbid_Detect[0]); i++ )
			{	
				Param28.Detect[i] = (int)(GetDBValue(dbid_Detect[i]).lValue);
				printf("Get %s=%d\n",ParamStr_Detect[i],Param28.Detect[i]); // 28����
				printf("497=%d\n",Param497.Detect[i]); // 497����
				// ��� �b�Ѽ�
				if(Param497.Detect[i] != Param28.Detect[i]) // 497�M28 ���P
				{
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Detect[i]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_Detect[i],Param497.Detect[i],Param28.Detect[i],iDBSelect);
				}
			}
		}
		/*���ɳ]�w �|���T�w 2020/3/30 �U�� 04:37:45*/
//		{/*���ɳ]�w*/
//			// ����497�ƭ� ���ɳ]�w �Ѽ�
//			for(int i = 0; i < sizeof(dbid_Delay)/sizeof(dbid_Delay[0]); i++ )
//			{	
//				Param497.Delay[i] = (int)(GetDBValue(dbid_Delay[i]).lValue);
//			}
//			// �V 28 �ШD��� ���ɳ]�w �Ѽ�
//			wNum = sizeof(dbid_Delay)/sizeof(dbid_Delay[0]);
//			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Delay);
//			for(int i = 0; i < sizeof(dbid_Delay)/sizeof(dbid_Delay[0]); i++ )
//			{	
//				Param28.Delay[i] = (int)(GetDBValue(dbid_Delay[i]).lValue);
//				printf("Get %s=%d\n",ParamStr_Delay[i],Param28.Delay[i]); // 28����
//				printf("497=%d\n",Param497.Delay[i]); // 497����
//				// ��� �b�Ѽ�
//				if(Param497.Delay[i] != Param28.Delay[i]) // 497�M28 ���P
//				{
//					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Delay[i]]);
//					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
//					printf("Choose %d\n",iDBSelect);
//					SetChosenDB(dbid_Delay[i],Param497.Delay[i],Param28.Delay[i],iDBSelect);
//				}
//			}
//		}
		{/*���|�Ѽ�*/
			
			// ����497�ƭ� �˴��]�w �Ѽ�
			for(int iPileGroup = 0; iPileGroup < sizeof(dbid_Pile)/sizeof(dbid_Pile[0]); iPileGroup++ ) // �C�@�� ���|�Ѽ�
			{
				int PileDiff=0;
				for(int i = 0; i < sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]); i++ ) // ���| �ѼƼƶq
				{	
					Param497.Pile[iPileGroup][i] = (int)(GetDBValue(dbid_Pile[iPileGroup][i]).lValue);
				}
				// �V 28 �ШD��� �˴��]�w �Ѽ�
				wNum = sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Pile[iPileGroup]);
				
				for(int i = 0; i < sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]); i++ )
				{	
					Param28.Pile[iPileGroup][i] = (int)(GetDBValue(dbid_Pile[iPileGroup][i]).lValue);
					printf("Get %s=%d\n",ParamStr_Pile[iPileGroup],Param28.Pile[iPileGroup][i]); // 28����
					printf("497=%d\n",Param497.Pile[iPileGroup][i]); // 497����
					// ��� �b�Ѽ�
					if(Param497.Pile[iPileGroup][i] != Param28.Pile[iPileGroup][i]) // 497�M28 ���P
						{
							printf("\n\nPile%d %d diff\n\n\n",iPileGroup,i);
							PileDiff++;
						}
				}
				if(PileDiff>0)// 497�M28 ���P
				{
					
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Pile[iPileGroup]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					for(int i = 0; i < sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]); i++ ) // ���| �ѼƼƶq
					{	
						SetChosenDB(dbid_Pile[iPileGroup][i],Param497.Pile[iPileGroup][i],Param28.Pile[iPileGroup][i],iDBSelect);
					}
					PileDiff=0;
				}
			}
		}
		{/*�Ͳ��޲z*/
			// ����497�ƭ� �Ͳ��޲z �Ѽ�
			for(int i = 0; i < sizeof(dbid_Prod)/sizeof(dbid_Prod[0]); i++ )
			{	
				Param497.Prod[i] = (int)(GetDBValue(dbid_Prod[i]).lValue);
			}
			// �V 28 �ШD��� �Ͳ��޲z �Ѽ�
			wNum = sizeof(dbid_Prod)/sizeof(dbid_Prod[0]);
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Prod);
			for(int i = 0; i < sizeof(dbid_Prod)/sizeof(dbid_Prod[0]); i++ )
			{	
				Param28.Prod[i] = (int)(GetDBValue(dbid_Prod[i]).lValue);
				printf("Get %s=%d\n",ParamStr_Prod[i],Param28.Prod[i]); // 28����
				printf("497=%d\n",Param497.Prod[i]); // 497����
				// ��� �b�Ѽ�
				if(Param497.Prod[i] != Param28.Prod[i]) // 497�M28 ���P
				{
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Prod[i]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_Prod[i],Param497.Prod[i],Param28.Prod[i],iDBSelect);
				}
			}
		}
	}
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72", 0); // 1:�����H�ܱо����D 2:�����H������D
}

/*---------------------------------------------------------------------------+
|  Function : SetChosenDB()                       						    		       |
|  Task     : �]�w���DB�ƭ�				                                         |
+----------------------------------------------------------------------------+
|  Parameter: dbIDName:DB�W��, Param497:�ܱо��ƭ�, Param28:28�ƭ�           |
|							iDBSelect: ��ܨӷ�																						 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect)
{
	printf("SetChosenDB\n");
	if(iDBSelect == DB_TP) // 497���D
	{
		SetDBValue(dbIDName,Param497);
		printf("SetDBValue %s=%d\n",dbIDName,Param497);
	}
	else if(iDBSelect == DB_CON) // 28������D
	{
		SetDBValue(dbIDName,-99);
		SetDBValue(dbIDName,Param28);
		printf("SetDBValue %s=%d\n",dbIDName,Param28);
	}
	
}

/*---------------------------------------------------------------------------+
|  Function : SprintfDBValue(int iParam497, int iParam28)                    |
|  Task     : �B�zDB�ƭ���ܤ�r		                                         |
+----------------------------------------------------------------------------+
|  Parameter: iParam497:497�ѼƼƭ�	iParam28:28�ѼƼƭ�											 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void SprintfDBValue(int iParam497, int iParam28)
{
	printf("iParam497=%d,iParam28=%d\n",iParam497,iParam28);
	printf("SprintfDBValue g_DBPrecision=%d\n",g_DBPrecision);
	int iPre=0;

	memset(gstr_DBValue_497, 0, sizeof(gstr_DBValue_497));
	memset(gstr_DBValue_28, 0, sizeof(gstr_DBValue_28));
	
	if(g_DBPrecision>0)
	{
		iPre = pow(10,g_DBPrecision); // ���
		printf("iPre=%d\n",iPre);
		printf("28.=%d\n",(iParam28/iPre));
		printf(".28=%d\n",(iParam28%iPre));
		sprintf(gstr_DBValue_497,"%d"".""%d", (iParam497/iPre), (iParam497%iPre));
		sprintf(gstr_DBValue_28,"%d"".""%d", (iParam28/iPre), (iParam28%iPre));
	}
	else
	{
		sprintf(gstr_DBValue_497,"%d",iParam497);
		sprintf(gstr_DBValue_28,"%d",iParam28);
	}
}
/*---------------------------------------------------------------------------+
|  Function : SprintfDBValue(char* strParam497,char* strParam28)             |
|  Task     : �B�zDB�ƭ���ܤ�r		                                         |
+----------------------------------------------------------------------------+
|  Parameter: iParam497:497�Ѽƭ�	iParam28:28�Ѽƭ�													 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void SprintfDBValue(char* strParam497,char* strParam28)
{
printf("SprintfDBValue %s,%s\n",strParam497,strParam28);
	memset(gstr_DBValue_497, 0, sizeof(gstr_DBValue_497));
	memset(gstr_DBValue_28, 0, sizeof(gstr_DBValue_28));
	
	sprintf(gstr_DBValue_497,"%s",strParam497);
	sprintf(gstr_DBValue_28,"%s",strParam28);
}