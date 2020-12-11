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
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncWord 				0xFFFF0000 // High Word
#define 	MechWord 				0x0000FFFF // Low Word

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
#define		DB_TP						1 // 選擇示教器參數資料
#define		DB_CON					2 // 選擇控制器參數資料
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_PickerType = 0,u_PickerType_old=-1; // 機型選擇 0-三軸 1-五軸
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸

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

/*參數比對*/
char* ParamStr[] = // 參數文字 
{
	/*機構參數*/
	"MECH_MECHTYPE", //0 機型選擇
	"TRANS_TYPE",		 //1 傳動方式
	"MECH_ENCTYPE",	 //2 編碼器選擇
	"MOTOR_ENC_REZ", //3 編碼器解析度
	"MOTOR_RATIO",	 //4 減速比
	"MECH_GEAR_D",	 //5 每轉距離
	"MECH_DOUB_MECH",//6 倍數機構 
	"MECH_POS_INV",	 //7 位置反向
	/*速度參數*/
	"SPEED_MAX",	 // 最高轉速
	"SPEED_ACC_T",	 // 加速時間
	"SPEED_DCC_T",	 // 減速時間
	"SPEED_RATIO",	 // 平滑比例
	"SPEED_POS_KP",	 // 到位響應
	"SPEED_SPEED_KP",	 // 追隨響應
	/*區域設定*/
	"ZONELIMIT_Zone",	 // 移動最大位置
	"ZONELIMIT_X1_MIN",	 // 1.允許下降模內最小位置
	"ZONELIMIT_X1_MAX",	 // 2.允許下降模內最大位置
	"ZONELIMIT_Y1_MIN",	 // 3.允許橫出最大高度
	"ZONELIMIT_Y1_MAX",	 // 4.允許關模最大待機高度
	"ZONELIMIT_Z_MIN",	 // 5.允許下降模內最大位置
	"ZONELIMIT_Z_MAX",	 // 6.允許置物型外最小位置
	"ZONELIMIT_X2_MIN",	 // 7.允許下降模內最小位置
	"ZONELIMIT_X2_MAX",	 // 8.允許下降模內最大位置
	"ZONELIMIT_Y2_MIN",	 // 9.允許橫出最小高度
	"ZONELIMIT_Y2_MAX",	 // 10.允許關模最大待機高度
	"ARM_SAFE_DIS",	 // 主副臂安全距離
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
};
char* ParamStr_Mech[] = // 參數文字  /*機構參數*/
{
	"MOTOR_ENC_REZ", // 編碼器解析度
	"MOTOR_RATIO",	 // 減速比
	"MECH_GEAR_D",	 // 每轉距離 
	"MECH_TOOTHM_NUM",// 齒數
	"MECH_TOOTHM_NUM"	 // 模數
};
char* ParamStr_Speed[] = // 參數文字  /*速度參數*/
{
	"SPEED_MAX", 			// 最高轉速
	"SPEED_ACC_T", 		// 加速時間
	"SPEED_DCC_T",		// 減速時間
	"SPEED_RATIO", 		// 加速平滑比例
	"SPEED_RATIO",		// 減速平滑比例
	"SPEED_POS_KP",		// 到位響應 位置Kp
	"SPEED_SPEED_KP",	// 追隨響應 速度Kp
};

int NumofMechType[] = {3,5}; // 三五軸機型對應軸數
char* dbid_MechType[] = 
{
	"MACHINE_CONFIGURATION_MACHINETYPE", // 機型選擇DB 三軸 五軸
	"MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE", // 傳動方式DB
};
/*---------------------------------------------------------------------------+
| u_pszMechPara 傳動方式  																									 |
|							   _____________________________________        							 |	
|   						|	Bit |				0				|				1				| 										 |		
|								|	 0	|	使用節圓直徑	|	使用齒數模數  |       							 |
|								|	 1	|			不使用 		|	使用倍數機構  |       							 |
|								|__2__|_____不使用____|_使用位置反向__|       							 |
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
char*	u_pszEditDoubleDB[] = // 紀錄倍數機構是否使用DB名稱
{
	"",
	"PICKER_PARAMETER_DOUBLE_X1",
	"PICKER_PARAMETER_DOUBLE_Y1",
	"PICKER_PARAMETER_DOUBLE_Z",
	"PICKER_PARAMETER_DOUBLE_X2",
	"PICKER_PARAMETER_DOUBLE_Y2"
}; 
char*	u_pszEditPOSINVDB[] = // 紀錄位置反向是否使用DB名稱
{
	"",
	"PICKER_PARAMETER_POS_INV_X1",
	"PICKER_PARAMETER_POS_INV_Y1",
	"PICKER_PARAMETER_POS_INV_Z",
	"PICKER_PARAMETER_POS_INV_X2",
	"PICKER_PARAMETER_POS_INV_Y2"
}; 

char* Mech_Data_String[] = // 機構參數 元件 名稱
{
	"edit_ENC_REZ", 				// 編碼器解析度
	"edit_Motor_Ratio", 	  // 減速比
	"edit_Motor_Diameter",	// 節圓直徑
	"edit_Tooth_Num",				// 齒數
	"edit_M_Num",						// 模數
};
char* dbid_Mech[5][5] = // 機構參數數值 db [軸][參數]
{	//  編碼器解析度									,減速比														,節圓直徑													,齒數														 ,模數
	{"MACHINE_INTERFACE_CONTROL_RSV05","MACHINE_INTERFACE_CONTROL_RSV10","MACHINE_FUNCTION_OPTIONS_RSV22","MACHINE_FUNCTION_OPTIONS_RSV12","MACHINE_FUNCTION_OPTIONS_RSV17"},		// X1軸
	{"MACHINE_INTERFACE_CONTROL_RSV06","MACHINE_INTERFACE_CONTROL_RSV11","MACHINE_FUNCTION_OPTIONS_RSV23","MACHINE_FUNCTION_OPTIONS_RSV13","MACHINE_FUNCTION_OPTIONS_RSV18"},		// Y1軸
	{"MACHINE_INTERFACE_CONTROL_RSV07","MACHINE_INTERFACE_CONTROL_RSV12","MACHINE_FUNCTION_OPTIONS_RSV24","MACHINE_FUNCTION_OPTIONS_RSV14","MACHINE_FUNCTION_OPTIONS_RSV19"},		// Z軸
	{"MACHINE_INTERFACE_CONTROL_RSV08","MACHINE_INTERFACE_CONTROL_RSV13","MACHINE_FUNCTION_OPTIONS_RSV25","MACHINE_FUNCTION_OPTIONS_RSV15","MACHINE_FUNCTION_OPTIONS_RSV20"},		// X2軸
	{"MACHINE_INTERFACE_CONTROL_RSV09","MACHINE_INTERFACE_CONTROL_RSV14","MACHINE_FUNCTION_OPTIONS_RSV26","MACHINE_FUNCTION_OPTIONS_RSV16","MACHINE_FUNCTION_OPTIONS_RSV21"}			// Y2軸
};
char* Speed_Data_String[] = // 速度參數 元件 名稱
{
	"edit_MaxSpeed", 	// 最高轉速
	"edit_ACC_T", 	  // 加速時間
	"edit_DCC_T",			// 減速時間
	"edit_ACC_Ratio", // 加速平滑比例
	"edit_DCC_Ratio",	// 減速平滑比例
	"edit_POS_KP",	// 到位響應 位置Kp
	"edit_SPD_KP",			// 追隨響應 速度Kp
};
char* dbid_Speed[5][7] = // 速度參數數值 db [軸][參數]
{	//
	{ "MACHINE_INTERFACE_MAXSPEED_X1",			// 最高轉速
		"MACHINE_INTERFACE_SPEED_ACCT_X1",		// 加速時間
		"MACHINE_INTERFACE_SPEED_DCCT_X1", 		// 減速時間
		"MACHINE_INTERFACE_CONTROL_RESV1", 		// 加速平滑比例
		"MACHINE_INTERFACE_CONTROL_RESV6", 		// 減速平滑比例
		"MACHINE_INTERFACE_SPEED_POS_KP_X1", 	// 到位響應 位置KP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_X1"	// 追隨響應 速度KP,
		},		// X1軸                     
	{ "MACHINE_INTERFACE_MAXSPEED_Y1",			// 最高轉速
		"MACHINE_INTERFACE_SPEED_ACCT_Y1",		// 加速時間
		"MACHINE_INTERFACE_SPEED_DCCT_Y1", 		// 減速時間
		"MACHINE_INTERFACE_CONTROL_RESV2", 		// 加速平滑比例
		"MACHINE_INTERFACE_CONTROL_RESV7", 		// 減速平滑比例
		"MACHINE_INTERFACE_SPEED_POS_KP_Y1", 	// 到位響應 位置KP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Y1"	// 追隨響應 速度KP,
		},		// Y1軸 
	{ "MACHINE_INTERFACE_MAXSPEED_Z",				// 最高轉速
		"MACHINE_INTERFACE_SPEED_ACCT_Z",			// 加速時間
		"MACHINE_INTERFACE_SPEED_DCCT_Z", 		// 減速時間
		"MACHINE_INTERFACE_CONTROL_RESV3", 		// 加速平滑比例
		"MACHINE_INTERFACE_CONTROL_RESV8", 		// 減速平滑比例
		"MACHINE_INTERFACE_SPEED_POS_KP_Z", 	// 到位響應 位置KP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Z"	// 追隨響應 速度KP,
		},		// Z軸  
	{ "MACHINE_INTERFACE_MAXSPEED_X2",			// 最高轉速
		"MACHINE_INTERFACE_SPEED_ACCT_X2",		// 加速時間
		"MACHINE_INTERFACE_SPEED_DCCT_X2", 		// 減速時間
		"MACHINE_INTERFACE_CONTROL_RESV4", 		// 加速平滑比例
		"MACHINE_INTERFACE_CONTROL_RESV9", 		// 減速平滑比例
		"MACHINE_INTERFACE_SPEED_POS_KP_X2", 	// 到位響應 位置KP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_X2"	// 追隨響應 速度KP,
		},		// X2軸                     
	{ "MACHINE_INTERFACE_MAXSPEED_Y2",			// 最高轉速
		"MACHINE_INTERFACE_SPEED_ACCT_Y2",		// 加速時間
		"MACHINE_INTERFACE_SPEED_DCCT_Y2", 		// 減速時間
		"MACHINE_INTERFACE_CONTROL_RESV5", 		// 加速平滑比例
		"MACHINE_INTERFACE_CONTROL_RESV10", 	// 減速平滑比例
		"MACHINE_INTERFACE_SPEED_POS_KP_Y2", 	// 到位響應 位置KP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Y2"	// 追隨響應 速度KP,
		},		// Y2軸 
};
char* ZoneLimit_String[6][3] = // 區域顯示文字 [軸][參數]
{
	{"VW_MAXPOSLIMIT","PICKER_SAFEDIST_BUFFER","POS_TOLERANCE"},	// 共同
	{"ZONELIMIT_Zone","ZONELIMIT_X1_MIN","ZONELIMIT_X1_MAX"},			// X1軸
	{"ZONELIMIT_Zone","ZONELIMIT_Y1_MIN","ZONELIMIT_Y1_MAX"},			// Y1軸
	{"ZONELIMIT_Zone","ZONELIMIT_Z_MIN","ZONELIMIT_Z_MAX"},				// Z軸
	{"ZONELIMIT_Zone","ZONELIMIT_X2_MIN","ZONELIMIT_X2_MAX"},			// X2軸
	{"ZONELIMIT_Zone","ZONELIMIT_Y2_MIN","ZONELIMIT_Y2_MAX"}			// Y2軸
};
char* dbid_ZoneLimit[6][3] = // 區域參數數值 db [軸][參數]
{
	{"SYSX_OTHERS_OTHERS_DWORD_RESERVED4","MACHINE_LIMITS_AXIS1_CONTROLPRECISION","MACHINE_LIMITS_AXIS2_CONTROLPRECISION"},	// 共同
	{"MACHINE_LIMITS_AXIS1_MAXPOSTION","MACHINE_LIMITS_AXIS_X1_MINPOSTION","MACHINE_LIMITS_AXIS_X1_MAXPOSTION"},						// X1軸
	{"MACHINE_LIMITS_AXIS2_MAXPOSTION","MACHINE_LIMITS_AXIS_Y1_MINPOSTION","MACHINE_LIMITS_AXIS_Y1_MAXPOSTION"},						// Y1軸
	{"MACHINE_LIMITS_AXIS3_MAXPOSTION","MACHINE_LIMITS_AXIS_Z_MINPOSTION","MACHINE_LIMITS_AXIS_Z_MAXPOSTION"},							// Z軸
	{"MACHINE_LIMITS_AXIS4_MAXPOSTION","MACHINE_LIMITS_AXIS_X2_MINPOSTION","MACHINE_LIMITS_AXIS_X2_MAXPOSTION"},						// X2軸
	{"MACHINE_LIMITS_AXIS5_MAXPOSTION","MACHINE_LIMITS_AXIS_Y2_MINPOSTION","MACHINE_LIMITS_AXIS_Y2_MAXPOSTION"}							// Y2軸
};
char* dbid_Mode[] = // 模式選擇 db 
{
	"MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE",	// 橫出姿態
	"MACHINE_FUNCTION_OPTIONS_MOLD_POSE",		// 模內姿態
	"MACHINE_FUNCTION_OPTIONS_RSV01"				// 射出機自動停止
};
char* ParamStr_Mode[] = // 模式選擇 顯示文字
{
	"PICKER_POSE_RUN",				// 橫出姿態
	"PICKER_POSE_INMOLD",			// 模內姿態
	"PICKER_MACHINE_AUTOSTOP"	// 射出機自動停止
};
char* dbid_Detect[] = // 檢測設定 db 
{
	"MACHINE_INTERFACE_MONITOR_RSV16",											// 開模隨動起始位置
	"MACHINE_CONFIGURATION_STEPTIMEOUT",										// 單步執行最大時間
	"MACHINE_CONFIGURATION_VALVE_DETECT_TIME_VERTICAL",			// 氣閥檢測 垂直
	"MACHINE_CONFIGURATION_VALVE_DETECT_TIME_HORIZONTAL",		// 氣閥檢測 水平
	"DBID_MACHINE_CONFIGURATION_VALVE_DETECT_TIME_B_PLUS",	// 氣閥檢測 B+
	"MACHINE_CONFIGURATION_VALVE_DETECT_TIME_B_MINUS",			// 氣閥檢測 B-
	
};
char* ParamStr_Detect[] = // 檢測設定 顯示文字
{
	"VW_PICKERDN_POSN",			// 開模隨動起始位置
	"PICKER_STEPMAXTIME",		// 單步執行最大時間
	"PICKER_VERTICAL",			// 氣閥檢測 垂直
	"PICKER_HORIZONTAL",		// 氣閥檢測 水平
	"PICKER_B_POS",					// 氣閥檢測 B+
	"PICKER_B_NEG",					// 氣閥檢測 B-
};
char* dbid_Prod[] = // 生產參數 db 
{
	"MACHINE_CONFIGURATION_CAROUSELOPERATIONTIME",	// 輸送帶時間
};
char* ParamStr_Prod[] = // 生產參數 顯示文字
{
	"VW_OTHER_COUNTPERUNIT",		// 單模產品總數
	"PICKER_PROD_CONV_TIME",		// 輸送帶時間
};

char*	dbid_Pile[4][8] =  // 堆疊參數 db
{/*順序													,方向												,X個數															,X間距															,Y個數														,Y間距															,Z個數														,Z間距*/	
 {"MACHINE_FUNCTION_PILE1_ORDER","MACHINE_FUNCTION_PILE1_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT1","MACHINE_FUNCTION_XAXIS_PILE_DIS1","MACHINE_FUNCTION_YAXIS_PILE_CNT1","MACHINE_FUNCTION_YAXIS_PILE_DIS1","MACHINE_FUNCTION_ZAXIS_PILE_CNT1","MACHINE_FUNCTION_ZAXIS_PILE_DIS1"}, //第一組
 {"MACHINE_FUNCTION_PILE2_ORDER","MACHINE_FUNCTION_PILE2_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT2","MACHINE_FUNCTION_XAXIS_PILE_DIS2","MACHINE_FUNCTION_YAXIS_PILE_CNT2","MACHINE_FUNCTION_YAXIS_PILE_DIS2","MACHINE_FUNCTION_ZAXIS_PILE_CNT2","MACHINE_FUNCTION_ZAXIS_PILE_DIS2"}, //第二組
 {"MACHINE_FUNCTION_PILE3_ORDER","MACHINE_FUNCTION_PILE3_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT3","MACHINE_FUNCTION_XAXIS_PILE_DIS3","MACHINE_FUNCTION_YAXIS_PILE_CNT3","MACHINE_FUNCTION_YAXIS_PILE_DIS3","MACHINE_FUNCTION_ZAXIS_PILE_CNT3","MACHINE_FUNCTION_ZAXIS_PILE_DIS3"}, //第三組
 {"MACHINE_FUNCTION_PILE4_ORDER","MACHINE_FUNCTION_PILE4_DIR","MACHINE_FUNCTION_XAXIS_PILE_CNT4","MACHINE_FUNCTION_XAXIS_PILE_DIS4","MACHINE_FUNCTION_YAXIS_PILE_CNT4","MACHINE_FUNCTION_YAXIS_PILE_DIS4","MACHINE_FUNCTION_ZAXIS_PILE_CNT4","MACHINE_FUNCTION_ZAXIS_PILE_DIS4"}, //第四組
};
char* ParamStr_Pile[] = // 堆疊參數 顯示文字
{
	"VW_PID_GROUP1",		// 第一組
	"VW_PID_GROUP2",		// 第二組
	"VW_PID_GROUP3",		// 第三組
	"VW_PID_GROUP4",		// 第四組
};

char* Manul_String[] = // 手動參數 元件 名稱
{
	"PICKER_FORWARDSPEED", 	// 前進速度
	"PICKER_POINTDISTANCE", 	  // 點動距離
};
char* dbid_Manul[5][2] =  //手動參數 db
{	/*前進速度																									,點動距離*/
	{"MACHINE_CONFIGURATION_MANUAL_AXIS1_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS1_MINPOSITION"}, // X1
	{"MACHINE_CONFIGURATION_MANUAL_AXIS2_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS2_MINPOSITION"}, // Y1
	{"MACHINE_CONFIGURATION_MANUAL_AXIS3_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS3_MINPOSITION"}, // Z
	{"MACHINE_CONFIGURATION_MANUAL_AXIS4_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS4_MINPOSITION"}, // X2
	{"MACHINE_CONFIGURATION_MANUAL_AXIS5_FWD_PERCENTAGEOFSPEED","MACHINE_CONFIGURATION_MANUAL_AXIS5_MINPOSITION"}, // Y2
};


int Precision[7][7] = // 軸參數 小數點位數 [什麼類參數][第幾個參數]
{
	{0,0,2,0,0,0,0},	// 機構參數
	{0,0,0,0,0,3,3},	// 速度參數
	{2,2,2,0,0,0,0},	// 區域參數
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,2,0,0,0,0,0},	// 手動參數
};

//參數數值 (文字)
char* ParamStr_TransType[] = // 傳動方式 參數數值 (文字)
{
	"MECH_GEAR_D", // 每轉距離
	"MECH_TOOTHM_NUM", // 齒數、模數
};
int iTransType=0,iDouble[5]={0},iPosInv[5]={0}; // 傳動方式,倍數機構,位置反向

char* ParamStr_Double[] = // 倍數機構 參數數值 (文字)
{
	"MECH_UNUSE", // 不使用
	"MECH_USE", // 使用
};
char* ParamStr_PosInv[] = // 位置反向 參數數值 (文字)
{
	"VW_ROTATELOC_ANGELREVERSED_OPT1", // 不使用
	"VW_ROTATELOC_ANGELREVERSED_OPT2", // 使用
};
char* ParamStr_PickerType[] = // 機型選擇 參數數值 (文字)
{
	"MECH_MECHTYPE3", // 三軸
	"MECH_MECHTYPE5", // 五軸
};
char* ParamStr_EncType[] = // 編碼器選擇 參數數值 (文字)
{
	"MECH_ENCTYPE_ABS", // 不使用
	"MECH_ENCTYPE_RES", // 使用
};
/*參數比對*/

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
CtmWnd*			pwndBmpAxis_X2 = NULL; // 機器手 X2軸 cjlee add 2019/10/29 上午 11:41:25
CtmWnd*			pwndBmpAxis_Y2 = NULL; // 機器手 Y2軸 cjlee add 2019/10/29 上午 11:41:25

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
BOOL 				HomingEnable 			= FALSE;

BOOL				CheckOnlyOne			=	FALSE;	//利用update每連線上僅執行一次 

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
			HomingEnable =FALSE;
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
					if(u_nMANUAL_TYPE == MANUAL_STOP_MODE)
						KeyCommand = 0x1234;
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
					FGC FgcTemp = ( u_PickerType==MechType5 ? 0xFFFF : 63519 );
					pwndEditPostionX2->SetPropValueT("fgc",FgcTemp);
					pwndEditPostionY2->SetPropValueT("fgc",FgcTemp);
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
					HomingEnable =TRUE;
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
				SendCommand(RunStopCmd);
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
	
	if(g_BkScreenSaver==1) // 判斷從螢幕保護程式回來 鑰匙變換
		{
			printf("Key Change\n");
			printf("KeyMode=%d\n",KeyMode);
			int KeyState = GetDBValue("MACHINE_INTERFACE_CONTROL_RSV16").lValue;
			printf("KeyState=%d\n",KeyState);
			if(KeyState == GPIO_KEY_STOP) // 手動
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 3);
				SendCommand(MODE_MANUAL);
			}
			else if(KeyState == GPIO_KEY_AUTO) // 自動 
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 4);
				SendCommand(MODE_AUTO);
			}
			else if(KeyState == GPIO_KEY_TEACH) // 停止
			{
				SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15", 2);
				SendCommand(MODE_NULL);//停止
			}
			g_BkScreenSaver = 0; // Set Flag Back From Screen Saver
		}

	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
	//printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);	
	if(u_wPickerOPSatus!=u_wPickerOPSatus_Old) // 狀態發生變化
	{
		printf("u_wPickerOPSatus=%d\n",u_wPickerOPSatus);
		if(u_wPickerOPSatus==STATE_FULLAUTO)
		{
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
			strcpy(g_szCurrentFormName, "Over.txt");
			::PutCommand("Over.txt");
		}
		else if(u_wPickerOPSatus==STATE_STOP)
		{
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
			strcpy(g_szCurrentFormName, "Index.txt");
			::PutCommand("Index.txt");
		}
		else if(u_wPickerOPSatus==STATE_HAND)
		{
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
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
}/*-----------------------------------機械手狀態----------------------------------*/

{/*-----------------------------------參數比對----------------------------------*/
	if(g_ptaskpicker != NULL)
	{
		int iPickerConnect = g_ptaskpicker->GetOnLineStatus();
		if(!iPickerConnect)  iPickerConnect = 1;
		else		 		iPickerConnect = 0;
		//printf("iPickerConnect=%d\n",iPickerConnect);
		if(iPickerConnect && CheckOnlyOne == FALSE) // 每次連結上 檢查一次參數
		{			
			printf("Connect\n");
			CheckOnlyOne=TRUE;
			GetValueFrom28(); // 參數比較
			::PutCommand(g_szCurrentFormName);			
		}
		if(!iPickerConnect)
			CheckOnlyOne = FALSE;
	}
}/*-----------------------------------參數比對----------------------------------*/


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
		if(pwndBmpState!=NULL) // 姿態圖示
		{
			if(u_dwGestureStatus & 0x0001) //0x0100
				pwndBmpState ->SetPropValueT("imagepath", "res_tm640/pic/Axis_C_V.bmp");
			if(u_dwGestureStatus & 0x0002) //0x0200
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
	
{/*-------------------------------------軸座標----------------------------------------*/
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
}/*-------------------------------------軸座標----------------------------------------*/
	
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


/*---------------------------------------------------------------------------+
|  Function : GetValueFrom28()                       					    		       |
|  Task     : 取得28設定值 比對參數                                          |
+----------------------------------------------------------------------------+
|  Parameter:   																	                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	GetValueFrom28() // 取得28設定值 比對參數
{
	if(g_ptaskpicker != NULL)
	{
		printf("Get 28 data value\n");
		int i_dbvalue_497[6][5] = {0};// 參數數值 db// 示教器暫存值
		int i_dbvalue_different = 0; // 參數數值差異個數
		int iDBSelect=0; // 參數依據選擇  1:以示教器為主 2:以控制器為主
		
		WORD wNum = 0;
		int  itemp=0;
		tmParam Param497; // 497 示教器上參數的值
		tmParam Param28; // 28 控制器上參數的值
		int iPre=0;
		
		// 機型選擇
		// 紀錄497數值
		Param497.iMechType = (int)(GetDBValue(dbid_MechType[0]).lValue);
		Param497.iPickerType = Param497.iMechType & MechWord; // 機型
		Param497.iEncType		 = (Param497.iMechType & EncWord)>>16; // 編碼器
		
		// 向 28 請求資料
		wNum = sizeof(dbid_MechType)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,dbid_MechType); // Update Data from 28
		printf("Get = %s\n",dbid_MechType[0]);
		Param28.iMechType = (int)(GetDBValue(dbid_MechType[0]).lValue); // 機型選擇
		Param28.iPickerType = Param28.iMechType & MechWord; // 機型
		Param28.iEncType		 = (Param28.iMechType & EncWord)>>16; // 編碼器
		
		printf("%s=%d\n",dbid_MechType[0],Param28.iMechType); // 28的值
		printf("497=%d\n",Param497.iMechType); // 497的值

		if(Param497.iPickerType != Param28.iPickerType) // 497和28三五軸選擇不同 機型選擇
		{
			SprintfDBValue(ParamStr_PickerType[Param497.iPickerType],ParamStr_PickerType[Param28.iPickerType]); // 參數數值 轉換成字串 供DBChoose 使用
			MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_MECHTYPE]]);
			iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
			printf("Choose %d\n",iDBSelect);
			SetChosenDB(dbid_MechType[0],Param497.iMechType,Param28.iMechType,iDBSelect);
			dw_MechType  = (GetDBValue(pMechTypeDB).lValue);
			u_PickerType = dw_MechType & MechWord;
		}		
		if(Param497.iEncType != Param28.iEncType) // 497和28 編碼器選擇 不同
		{
			printf("Param497.iEncType=%d, Param28.iEncType=%d\n",Param497.iEncType,Param28.iEncType);
			SprintfDBValue(ParamStr_EncType[Param497.iEncType],ParamStr_EncType[Param28.iEncType]); // 參數數值 轉換成字串 供DBChoose 使用
			MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_ENCTYPE]]);
			iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
			printf("Choose %d\n",iDBSelect);
			SetChosenDB(dbid_MechType[0],Param497.iMechType,Param28.iMechType,iDBSelect);
		}		
		
		printf("NumofMechType=%d\n",NumofMechType[u_PickerType]);
		for(int AxisNum=0; AxisNum<NumofMechType[u_PickerType]; AxisNum++) // 依據 NumofMechType[u_PickerType] 3軸或5軸
		{
			printf("AxisNum=%d\n",AxisNum);
			{/*傳動方式 + 倍數機構 + 位置反向*/
				// 紀錄497數值 傳動方式 + 倍數機構 + 位置反向
				Param497.iTransType = (int)(GetDBValue(u_pszMechPara[AxisNum]).lValue); // 傳動方式 + 倍數機構 + 位置反向
				
				// 向 28 請求資料 傳動方式 + 倍數機構 + 位置反向
				wNum = 1;
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,&u_pszMechPara[AxisNum]);
				printf("Get = %s\n",u_pszMechPara[AxisNum]);
				Param28.iTransType = (int)(GetDBValue(u_pszMechPara[AxisNum]).lValue);
				
				printf("%s=%d\n",u_pszMechPara[AxisNum],Param28.iTransType); // 28的值
				printf("497=%d\n",Param497.iTransType); // 497的值  
				
				// 比較
				if(Param497.iTransType != Param28.iTransType) // 497和28 bit 0 傳動方式 + bit 1 倍數機構 + bit 2 位置反向 不同
				{
					Param497.iDouble[AxisNum] 	= ((Param497.iTransType) & 0x002)>>1; // 倍數機構 497
					Param497.iPosInv[AxisNum] 	= ((Param497.iTransType) & 0x004)>>2; // 位置反向 497
					Param28.iDouble[AxisNum] 	= ((Param28.iTransType) & 0x002)>>1; // 倍數機構 28
					Param28.iPosInv[AxisNum] 	= ((Param28.iTransType) & 0x004)>>2; // 位置反向 28
					
					printf("TransType Double Inverse diff\n");
					// 比較一次 傳動方式
					if(AxisNum<1) // 497和28 傳動方式 不同 Param497.iTransType != Param28.iTransType
					{
						int iTransType497=0,iTransType28=0;
						iTransType497 = int(Param497.iTransType & 0x001); // 傳動方式
						iTransType28 = int(Param28.iTransType & 0x001); // 傳動方式
						if( iTransType497 != iTransType28 )
						{
							printf("TransType diff\n");
							SprintfDBValue(ParamStr_TransType[iTransType497],ParamStr_TransType[iTransType28]);  // 處理DB數值顯示文字
							MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[TRANS_TYPE]]);
							iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
							printf("Choose %d\n",iDBSelect);
							iTransType = (iDBSelect==DB_CON) ? (Param28.iTransType & 0x001):(Param497.iTransType & 0x001);
							printf("TransType=%x\n",iTransType);
						}
					}

					if(Param497.iDouble[AxisNum] != Param28.iDouble[AxisNum]) //  497和28 倍數機構 不同
						{
							printf("Double diff\n");
							SprintfDBValue(ParamStr_Double[Param497.iDouble[AxisNum]],ParamStr_Double[Param28.iDouble[AxisNum]]); // 參數數值 轉換成字串 供DBChoose 使用
							MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[MECH_DOUB_MECH]]);
							iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
							printf("Choose %d\n",iDBSelect);
							iDouble[AxisNum] = (iDBSelect==DB_CON)?Param28.iDouble[AxisNum]:Param497.iDouble[AxisNum];
							printf("iDouble[%d]=%x\n",AxisNum,iDouble[AxisNum]);
						}
					else
						iDouble[AxisNum]=Param497.iDouble[AxisNum];
					if(Param497.iPosInv[AxisNum] != Param28.iPosInv[AxisNum]) //  497和28 位置反向 不同
						{
							printf("PosInv diff\n");
							SprintfDBValue(ParamStr_PosInv[Param497.iPosInv[AxisNum]],ParamStr_PosInv[Param28.iPosInv[AxisNum]]); // 參數數值 轉換成字串 供DBChoose 使用
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
					| 傳動方式						  									|
					|	___________0_________________1__________|
					|	bit0 	使用每轉距離	|		使用齒數模數		|
					| bit1 		不使用			|		使用倍數機構		|
					| bit2 		不使用			|		使用反向位置		|
					+----------------------------------------*/
					printf("Set TransType = %x\n",TempValue);
					SetDBValue(u_pszMechPara[AxisNum],TempValue);
					printf("Get TransType=%d\n",int(GetDBValue(u_pszMechPara[AxisNum]).lValue));
					SetDBValue(u_pszEditDoubleDB[AxisNum+1],iDouble[AxisNum]); // 倍數機構 DB
					SetDBValue(u_pszEditPOSINVDB[AxisNum+1],iPosInv[AxisNum]); // 位置反向 DB
					
					SetChosenDB(u_pszMechPara[AxisNum],Param497.iTransType,Param28.iTransType,iDBSelect);// 寫入 (傳動方式 + 倍數機構 + 位置反向) 共用DB
				}
			}
			
			printf("\n\nAxis Check!!!!!!\n\n");
			{/*軸參數*/
				// 紀錄497數值 軸參數
				for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
				{	
					Param497.Axis[AxisNum][i] = (int)(GetDBValue(dbid_Mech[AxisNum][i]).lValue);
				}
				// 向 28 請求資料 軸參數
				wNum = sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Mech[AxisNum]);
				for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
				{	
					Param28.Axis[AxisNum][i] = (int)(GetDBValue(dbid_Mech[AxisNum][i]).lValue);
					printf("Get %s=%d\n",dbid_Mech[AxisNum][i],Param28.Axis[AxisNum][i]); // 28的值
					printf("497=%d\n",Param497.Axis[AxisNum][i]); // 497的值
					// 比較 軸參數
					if(Param497.Axis[AxisNum][i] != Param28.Axis[AxisNum][i]) // 497和28 不同
					{
						printf("%d is diff\n",i);
						g_DBVale_497=Param497.Axis[AxisNum][i];g_DBVale_28=Param28.Axis[AxisNum][i]; // 示教器及控制器數值
						g_DBPrecision=Precision[PAxis][i]; // 參數精度
						
						SprintfDBValue(Param497.Axis[AxisNum][i],Param28.Axis[AxisNum][i]); // 參數數值 轉換成字串 供DBChoose 使用

						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Mech[i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_Mech[AxisNum][i],Param497.Axis[AxisNum][i],Param28.Axis[AxisNum][i],iDBSelect);
					}
				}
			}
			
			{/*速度參數*/
				// 紀錄497數值 速度參數
				for(int i = 0; i < sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]); i++ )
				{	 
					Param497.Speed[AxisNum][i] = (int)(GetDBValue(dbid_Speed[AxisNum][i]).lValue);
				}
				// 向 28 請求資料 速度參數
				wNum = sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Speed[AxisNum]);
				for(int i = 0; i < sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]); i++ ) // 根據 Speed_Data_String 內容決定參數數量 
				{	
					Param28.Speed[AxisNum][i] = (int)(GetDBValue(dbid_Speed[AxisNum][i]).lValue);
					printf("Get %s=%d\n",dbid_Speed[AxisNum][i],Param28.Speed[AxisNum][i]); // 28的值
					printf("497=%d\n",Param497.Speed[AxisNum][i]); // 497的值
					// 比較 速度參數
					if(Param497.Speed[AxisNum][i] != Param28.Speed[AxisNum][i]) // 497和28 不同
					{
						g_DBVale_497=Param497.Speed[AxisNum][i];g_DBVale_28=Param28.Speed[AxisNum][i]; // 示教器及控制器數值
						g_DBPrecision=Precision[PSpeed][i]; // 參數精度
						SprintfDBValue(Param497.Speed[AxisNum][i],Param28.Speed[AxisNum][i]); // 參數數值 轉換成字串 供DBChoose 使用
						
						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Speed[i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_Speed[AxisNum][i],Param497.Speed[AxisNum][i],Param28.Speed[AxisNum][i],iDBSelect);
					}
				}
			}
			
			{/*區域設定*/
				printf("Compare ZoneLimit Param\n");
				// 紀錄497數值 區域設定參數
				for(int i = 0; i < sizeof(ZoneLimit_String[AxisNum+1])/sizeof(ZoneLimit_String[AxisNum+1][0]); i++ )
				{	 
					Param497.ZoneLimit[AxisNum+1][i] = (int)(GetDBValue(dbid_ZoneLimit[AxisNum+1][i]).lValue);
				}
				// 向 28 請求資料 區域設定參數
				wNum = sizeof(ZoneLimit_String[AxisNum+1])/sizeof(ZoneLimit_String[AxisNum+1][0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_ZoneLimit[AxisNum+1]);
				for(int i = 0; i < sizeof(ZoneLimit_String[AxisNum+1])/sizeof(ZoneLimit_String[AxisNum+1][0]); i++ ) // 根據 ZoneLimit_String[AxisNum+1] 內容決定參數數量 
				{	
					Param28.ZoneLimit[AxisNum+1][i] = (int)(GetDBValue(dbid_ZoneLimit[AxisNum+1][i]).lValue);
					printf("Get %s=%d\n",dbid_ZoneLimit[AxisNum+1][i],Param28.ZoneLimit[AxisNum+1][i]); // 28的值
					printf("497=%d\n",Param497.ZoneLimit[AxisNum+1][i]); // 497的值
					// 比較 速度參數
					if(Param497.ZoneLimit[AxisNum+1][i] != Param28.ZoneLimit[AxisNum+1][i]) // 497和28 不同
					{
						g_DBVale_497=Param497.ZoneLimit[AxisNum+1][i];g_DBVale_28=Param28.ZoneLimit[AxisNum+1][i]; // 示教器及控制器數值
						g_DBPrecision=Precision[PZoneLimit][i]; // 參數精度
						SprintfDBValue(Param497.ZoneLimit[AxisNum+1][i],Param28.ZoneLimit[AxisNum+1][i]); // 參數數值 轉換成字串 供DBChoose 使用
						
						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ZoneLimit_String[AxisNum+1][i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_ZoneLimit[AxisNum+1][i],Param497.ZoneLimit[AxisNum+1][i],Param28.ZoneLimit[AxisNum+1][i],iDBSelect);
					}
				}
				
				//共同
				// 紀錄497數值 區域設定參數
				Param497.ZoneLimit[0][1] = (int)(GetDBValue(dbid_ZoneLimit[0][1]).lValue);//安全距離緩衝區
				Param497.ZoneLimit[0][2] = (int)(GetDBValue(dbid_ZoneLimit[0][2]).lValue);//到位提前裕度
				// 向 28 請求資料 區域設定參數
				wNum = sizeof(ZoneLimit_String[0])/sizeof(ZoneLimit_String[0][0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_ZoneLimit[0]);
				
				Param28.ZoneLimit[0][1] = (int)(GetDBValue(dbid_ZoneLimit[0][1]).lValue);
				Param28.ZoneLimit[0][2] = (int)(GetDBValue(dbid_ZoneLimit[0][2]).lValue);
				
				printf("Get %s=%d\n",dbid_ZoneLimit[0][1],Param28.ZoneLimit[0][1]); // 28的值
				printf("497=%d\n",Param497.ZoneLimit[0][1]); // 497的值				
				if(Param497.ZoneLimit[0][1] != Param28.ZoneLimit[0][1]) // 497和28 不同
				{
					g_DBVale_497=Param497.ZoneLimit[0][1];g_DBVale_28=Param28.ZoneLimit[0][1]; // 示教器及控制器數值
					g_DBPrecision=Precision[PZoneLimit][1]; // 參數精度
					SprintfDBValue(Param497.ZoneLimit[0][1],Param28.ZoneLimit[0][1]); // 參數數值 轉換成字串 供DBChoose 使用
					
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ZoneLimit_String[0][1]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_ZoneLimit[0][1],Param497.ZoneLimit[0][1],Param28.ZoneLimit[0][1],iDBSelect);
				}
				
				printf("Get %s=%d\n",dbid_ZoneLimit[0][2],Param28.ZoneLimit[0][2]); // 28的值
				printf("497=%d\n",Param497.ZoneLimit[0][2]); // 497的值
				if(Param497.ZoneLimit[0][2] != Param28.ZoneLimit[0][2]) // 497和28 不同
				{
					g_DBVale_497=Param497.ZoneLimit[0][2];g_DBVale_28=Param28.ZoneLimit[0][2]; // 示教器及控制器數值
					g_DBPrecision=Precision[PZoneLimit][2]; // 參數精度
					SprintfDBValue(Param497.ZoneLimit[0][2],Param28.ZoneLimit[0][2]); // 參數數值 轉換成字串 供DBChoose 使用
					
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ZoneLimit_String[0][2]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_ZoneLimit[0][2],Param497.ZoneLimit[0][2],Param28.ZoneLimit[0][2],iDBSelect);
				}
			}
			
			{/*手動參數*/
				// 紀錄497數值 速度參數
				for(int i = 0; i < sizeof(Manul_String)/sizeof(Manul_String[0]); i++ )
				{	 
					Param497.Manual[AxisNum][i] = (int)(GetDBValue(dbid_Manul[AxisNum][i]).lValue);
				}
				// 向 28 請求資料 速度參數
				wNum = sizeof(Manul_String)/sizeof(Manul_String[0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Manul[AxisNum]);
				for(int i = 0; i < sizeof(Manul_String)/sizeof(Manul_String[0]); i++ ) // 根據 Manul_String 內容決定參數數量 
				{	
					Param28.Manual[AxisNum][i] = (int)(GetDBValue(dbid_Manul[AxisNum][i]).lValue);
					printf("Get %s=%d\n",dbid_Manul[AxisNum][i],Param28.Manual[AxisNum][i]); // 28的值
					printf("497=%d\n",Param497.Manual[AxisNum][i]); // 497的值
					// 比較 速度參數
					if(Param497.Manual[AxisNum][i] != Param28.Manual[AxisNum][i]) // 497和28 不同
					{
						g_DBVale_497=Param497.Manual[AxisNum][i];g_DBVale_28=Param28.Manual[AxisNum][i]; // 示教器及控制器數值
						g_DBPrecision=Precision[PManual][i]; // 參數精度
						SprintfDBValue(Param497.Manual[AxisNum][i],Param28.Manual[AxisNum][i]); // 參數數值 轉換成字串 供DBChoose 使用
						printf("MsgBoxCall\n");
						MsgBoxCall("DB_Choose.txt",g_MultiLanguage[Manul_String[i]]);
						iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
						printf("Choose %d\n",iDBSelect);
						SetChosenDB(dbid_Manul[AxisNum][i],Param497.Manual[AxisNum][i],Param28.Manual[AxisNum][i],iDBSelect);
					}
				}
			}
			
		} // 依據 NumofMechType[u_PickerType] 3軸或5軸
		{/*模式選擇*/
			// 紀錄497數值 模式選擇 參數
			for(int i = 0; i < sizeof(dbid_Mode)/sizeof(dbid_Mode[0]); i++ )
			{	
				Param497.Mode[i] = (int)(GetDBValue(dbid_Mode[i]).lValue);
			}
			// 向 28 請求資料 模式選擇 參數
			wNum = sizeof(dbid_Mode)/sizeof(dbid_Mode[0]);
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Mode);
			for(int i = 0; i < sizeof(dbid_Mode)/sizeof(dbid_Mode[0]); i++ )
			{	
				Param28.Mode[i] = (int)(GetDBValue(dbid_Mode[i]).lValue);
				printf("Get %s=%d\n",ParamStr_Mode[i],Param28.Mode[i]); // 28的值
				printf("497=%d\n",Param497.Mode[i]); // 497的值
				// 比較 軸參數
				if(Param497.Mode[i] != Param28.Mode[i]) // 497和28 不同
				{
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Mode[i]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_Mode[i],Param497.Mode[i],Param28.Mode[i],iDBSelect);
				}
			}
		}
		{/*檢測設定*/
			// 紀錄497數值 檢測設定 參數
			for(int i = 0; i < sizeof(dbid_Detect)/sizeof(dbid_Detect[0]); i++ )
			{	
				Param497.Detect[i] = (int)(GetDBValue(dbid_Detect[i]).lValue);
			}
			// 向 28 請求資料 檢測設定 參數
			wNum = sizeof(dbid_Detect)/sizeof(dbid_Detect[0]);
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Detect);
			for(int i = 0; i < sizeof(dbid_Detect)/sizeof(dbid_Detect[0]); i++ )
			{	
				Param28.Detect[i] = (int)(GetDBValue(dbid_Detect[i]).lValue);
				printf("Get %s=%d\n",ParamStr_Detect[i],Param28.Detect[i]); // 28的值
				printf("497=%d\n",Param497.Detect[i]); // 497的值
				// 比較 軸參數
				if(Param497.Detect[i] != Param28.Detect[i]) // 497和28 不同
				{
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Detect[i]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_Detect[i],Param497.Detect[i],Param28.Detect[i],iDBSelect);
				}
			}
		}
		/*延時設定 尚不確定 2020/3/30 下午 04:37:45*/
//		{/*延時設定*/
//			// 紀錄497數值 延時設定 參數
//			for(int i = 0; i < sizeof(dbid_Delay)/sizeof(dbid_Delay[0]); i++ )
//			{	
//				Param497.Delay[i] = (int)(GetDBValue(dbid_Delay[i]).lValue);
//			}
//			// 向 28 請求資料 延時設定 參數
//			wNum = sizeof(dbid_Delay)/sizeof(dbid_Delay[0]);
//			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Delay);
//			for(int i = 0; i < sizeof(dbid_Delay)/sizeof(dbid_Delay[0]); i++ )
//			{	
//				Param28.Delay[i] = (int)(GetDBValue(dbid_Delay[i]).lValue);
//				printf("Get %s=%d\n",ParamStr_Delay[i],Param28.Delay[i]); // 28的值
//				printf("497=%d\n",Param497.Delay[i]); // 497的值
//				// 比較 軸參數
//				if(Param497.Delay[i] != Param28.Delay[i]) // 497和28 不同
//				{
//					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Delay[i]]);
//					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
//					printf("Choose %d\n",iDBSelect);
//					SetChosenDB(dbid_Delay[i],Param497.Delay[i],Param28.Delay[i],iDBSelect);
//				}
//			}
//		}
		{/*堆疊參數*/
			
			// 紀錄497數值 檢測設定 參數
			for(int iPileGroup = 0; iPileGroup < sizeof(dbid_Pile)/sizeof(dbid_Pile[0]); iPileGroup++ ) // 每一組 堆疊參數
			{
				int PileDiff=0;
				for(int i = 0; i < sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]); i++ ) // 堆疊 參數數量
				{	
					Param497.Pile[iPileGroup][i] = (int)(GetDBValue(dbid_Pile[iPileGroup][i]).lValue);
				}
				// 向 28 請求資料 檢測設定 參數
				wNum = sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]);
				g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Pile[iPileGroup]);
				
				for(int i = 0; i < sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]); i++ )
				{	
					Param28.Pile[iPileGroup][i] = (int)(GetDBValue(dbid_Pile[iPileGroup][i]).lValue);
					printf("Get %s=%d\n",ParamStr_Pile[iPileGroup],Param28.Pile[iPileGroup][i]); // 28的值
					printf("497=%d\n",Param497.Pile[iPileGroup][i]); // 497的值
					// 比較 軸參數
					if(Param497.Pile[iPileGroup][i] != Param28.Pile[iPileGroup][i]) // 497和28 不同
						{
							printf("\n\nPile%d %d diff\n\n\n",iPileGroup,i);
							PileDiff++;
						}
				}
				if(PileDiff>0)// 497和28 不同
				{
					
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Pile[iPileGroup]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					for(int i = 0; i < sizeof(dbid_Pile[iPileGroup])/sizeof(dbid_Pile[iPileGroup][0]); i++ ) // 堆疊 參數數量
					{	
						SetChosenDB(dbid_Pile[iPileGroup][i],Param497.Pile[iPileGroup][i],Param28.Pile[iPileGroup][i],iDBSelect);
					}
					PileDiff=0;
				}
			}
		}
		{/*生產管理*/
			// 紀錄497數值 生產管理 參數
			for(int i = 0; i < sizeof(dbid_Prod)/sizeof(dbid_Prod[0]); i++ )
			{	
				Param497.Prod[i] = (int)(GetDBValue(dbid_Prod[i]).lValue);
			}
			// 向 28 請求資料 生產管理 參數
			wNum = sizeof(dbid_Prod)/sizeof(dbid_Prod[0]);
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Prod);
			for(int i = 0; i < sizeof(dbid_Prod)/sizeof(dbid_Prod[0]); i++ )
			{	
				Param28.Prod[i] = (int)(GetDBValue(dbid_Prod[i]).lValue);
				printf("Get %s=%d\n",ParamStr_Prod[i],Param28.Prod[i]); // 28的值
				printf("497=%d\n",Param497.Prod[i]); // 497的值
				// 比較 軸參數
				if(Param497.Prod[i] != Param28.Prod[i]) // 497和28 不同
				{
					MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr_Prod[i]]);
					iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
					printf("Choose %d\n",iDBSelect);
					SetChosenDB(dbid_Prod[i],Param497.Prod[i],Param28.Prod[i],iDBSelect);
				}
			}
		}
	}
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72", 0); // 1:全部以示教器為主 2:全部以控制器為主
}

/*---------------------------------------------------------------------------+
|  Function : SetChosenDB()                       						    		       |
|  Task     : 設定選擇DB數值				                                         |
+----------------------------------------------------------------------------+
|  Parameter: dbIDName:DB名稱, Param497:示教器數值, Param28:28數值           |
|							iDBSelect: 選擇來源																						 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect)
{
	printf("SetChosenDB\n");
	if(iDBSelect == DB_TP) // 497為主
	{
		SetDBValue(dbIDName,Param497);
		printf("SetDBValue %s=%d\n",dbIDName,Param497);
	}
	else if(iDBSelect == DB_CON) // 28控制器為主
	{
		SetDBValue(dbIDName,-99);
		SetDBValue(dbIDName,Param28);
		printf("SetDBValue %s=%d\n",dbIDName,Param28);
	}
	
}

/*---------------------------------------------------------------------------+
|  Function : SprintfDBValue(int iParam497, int iParam28)                    |
|  Task     : 處理DB數值顯示文字		                                         |
+----------------------------------------------------------------------------+
|  Parameter: iParam497:497參數數值	iParam28:28參數數值											 |
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
		iPre = pow(10,g_DBPrecision); // 精度
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
|  Task     : 處理DB數值顯示文字		                                         |
+----------------------------------------------------------------------------+
|  Parameter: iParam497:497參數值	iParam28:28參數值													 |
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