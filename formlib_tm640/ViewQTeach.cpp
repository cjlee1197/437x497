/*===========================================================================+
|  Class    : View Qteach								                                     |
|  Task     : View Qteach								                                     |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#include	"ViewQTeach.h"
#include	"../commonaction.h"
#include	"../font.h"
#include	"../utils.h"

/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncType_Abs			0x00000000  // 絕對
#define		EncType_Res		  0x00010000  // 增量
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
#define		GPIO_SAFEKEY_BIT	0x14 // 安全按鈕
#define		GPIO_SAFEKEY_0 0x4
#define		GPIO_SAFEKEY_1 0x10
#define		GPIO_SAFEKEY_2 0x14
#define		MANUAL_CONTINUE_MODE	0
#define		MANUAL_JOG_MODE	1
#define		MANUAL_STOP_MODE	2
// 流程 區域點
#define		Origin			1 // 原點
#define		WaitP				2 // 待機點
#define		DownWaitP		3 // 下待機點
#define		ReadyDownP	4	// 準備下降點
#define		PickDownP		5	// 取出側下降點
#define		PickP				6	// 取出側取件點
#define		MoveOutP		7	// 橫出點
#define		PlaceDownP	8	// 置物側下降點
#define		PlaceP			9	// 置物側置物點
#define		Homing		 10	// 回原點

#define     MaxDBNum	      100// 教導步驟數量上限
#define     PARA_NUM	      5	 // 參數數量
// 動作類型
#define		Action_Axis				1  // 軸動作
#define		Action_Wait				2  // 等待
#define		Action_Permit			3  // 允許

#define		Action_Valve			4	 // 閥門
#define		Action_Tag				5  // 標籤
#define		Action_Goto				6  // 跳轉
#define		Action_Detect			8  // 檢測
#define		Action_Pile				12 // 堆疊
#define		Action_Sub				13 // 副程式
// 軸數
#define		Axis_X1				1  // X1軸
#define		Axis_Y1				2  // Y1軸
#define		Axis_Z				3  // Z軸
#define		Axis_X2				4  // X2軸
#define		Axis_Y2				5  // Y2軸
#define		Axis_C				6  // C軸
//等待
#define		Wait_IMEStop		1  // 射出機急停
#define		Wait_MoldOpen		2  // 開模完成
#define		Wait_MoldClose	3  // 關模完成
#define		Wait_SDoorClose	4  // 安全門關
#define		Wait_IMAuto			5  // 射出機自動
#define		Wait_PushOut		6  // 頂出完成
#define		Wait_PullBack		7  // 頂退完成

// 允許
#define		Permit_MoldSafe				1  // 模區安全
#define		Permit_MoldClose			2  // 允許關模
#define		Permit_PushOut				3  // 允許頂出
#define		Permit_PickerESTOP		4  // 機械手急停
#define		Permit_MoldOpen				5  // 允許開模
#define		Permit_PullBack				6  // 允許頂退

// 閥門
#define		Valve_AxisC_V				1  // 主臂垂直
#define		Valve_AxisC_H				2  // 主臂水平
#define		Valve_Clamp1				3  // 夾1
#define		Valve_Clamp2				4  // 夾2
#define		Valve_Suck1					5  // 吸1
#define		Valve_Suck2					6  // 吸2
#define		Valve_Choose1				71 // 選擇1
#define		Valve_Choose2				72 // 選擇2	
#define		Valve_Choose3				73 // 選擇3
#define		Valve_Choose4				74 // 選擇4				

#define		ON					1  //
#define		OFF					0  //
#define		START				1  //
#define		END					1  //
#define		Selected		1  //
#define		NO					0  //
#define		OK					1  //

#define		Color_White		0xFFFF
#define		Color_Red			0xF800
#define		Color_Yellow 	0xFF80
#define		Color_LBlue		0xDFBF

#define		MIN				0  //
#define		MAX				1  //
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸

BOOL b_RunOnlyOne = FALSE; // 利用 Update() 來到這頁 只執行一次
BOOL b_SaveFlag = FALSE; // 是否儲存數值

int WhichAxis=0;
int WhichAxis_Old=-1;
char* pWhichAxisDB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_TYPE"; // 偵測哪一軸 DB

int 	Manual_Type=MANUAL_STOP_MODE; // 背後安全按鈕

int			u_QTeach_Type=0,u_QTeach_Pos=0;

CtmWnd*		pwndBtn_PosHV	= NULL; // 姿態選擇按鈕
CtmWnd*		pwndImg_NowMoveAxis	= NULL; // 現在移動軸 顯示 圖片
CtmWnd*		pwndStr_NowMoveAxis	= NULL; // 現在移動軸 顯示 文字

CtmWnd*		pwndBtn_GetPos[6]	= {NULL}; // 獲取位置按鈕
char* GetPos_String[] = // 取得位置Btn 名稱
{
	"",
	"Btn_GetPos_X1",
	"Btn_GetPos_Y1",
	"Btn_GetPos_Z",
	"Btn_GetPos_X2",
	"Btn_GetPos_Y2",
};

CtmWnd*		pwndBar_PosHint[6] = {NULL}; // 位置提示 bar
char* Bar_PosHint_String[] = // 位置提示 bar 名稱
{
	"",
	"PosHint_X1",
	"PosHint_Y1",
	"PosHint_Z",
	"PosHint_X2",
	"PosHint_Y2",
};

CtmWnd*		pwndImg_PosHint[6] = {NULL}; // 位置提示 圖片
char* Img_PosHint_String[] = // 位置提示 圖片 名稱
{
	"",
	"Img_PosHint_X1",
	"Img_PosHint_Y1",
	"Img_PosHint_Z",
	"Img_PosHint_X2",
	"Img_PosHint_Y2",
};
CtmWnd*		pwndMask_PosHint[6] = {NULL}; // 位置提示 遮罩
char* Mask_PosHint_String[] = // 位置提示 圖片 名稱
{
	"",
	"Mask_PosHint_X1",
	"Mask_PosHint_Y1",
	"Mask_PosHint_Z",
	"Mask_PosHint_X2",
	"Mask_PosHint_Y2",
};
long AxisPosNow_Old[6]={0};
long AxisPosNow[6]={0};

CtmWnd*		pwndBtnTeachStart	= NULL; // 開始教導按鈕
CtmWnd*		pwndBtnNextP	= NULL; // 下一位置按鈕
CtmWnd*		pwndStr_ClampSelect	= NULL; // 治具選用文字

CtmWnd*		pwndCheck_Move	= NULL; // 準備下降點 選用 移動後下降 勾勾
CtmWnd*		pwndBtn_Check_Move	= NULL; // 準備下降點 選用 移動後下降 Btn
CtmWnd*		pwndImg_QTeach_ReadyDownP	= NULL; // 準備下降點 顯示圖片

CtmWnd*		pwndCheck_SubUse	= NULL; // 副臂使用 勾勾
CtmWnd*		pwndCheck_SubSync	= NULL; // 副臂與主臂同動 勾勾
CtmWnd*		pwndBtn_SubUse		= NULL; // 副臂使用 Btn
CtmWnd*		pwndBtn_SubSync		= NULL; // 副臂與主臂同動 Btn
CtmWnd*		pwndMask_SubArm		= NULL; // 副臂 遮罩


CtmWnd*		pwndBtn_SLVL	= NULL; // 柔性使用 Btn
CtmWnd*		pwndCheck_SLVL	= NULL; // 柔性使用 勾勾
CtmWnd*		pwndMask_SLVL	= NULL; // 柔性使用 遮罩
CtmWnd*		pwndPick_SLVL			= NULL; // 取件柔性 數值設定
CtmWnd*		pwndPickOut_SLVL	= NULL; // 取出柔性 數值設定
CtmWnd*		pwndSLVL_Logo	= NULL; // 柔性使用 Logo 
CtmWnd*		pwndPick_Dis			= NULL; // 下降距離 數值顯示
CtmWnd*		pwndPickOut_Dis		= NULL; // 後退距離 數值顯示

CtmWnd*		pwndCheck_Ejector	= NULL; // 檢測 頂退完成(開始頂出) 勾勾

CtmWnd*		pwndBtn_PileUse	= NULL; // 堆疊選用 Btn
CtmWnd*		pwndCheck_Pile	= NULL; // 堆疊選用 勾勾
CtmWnd*		pwndMask_Pile		= NULL; // 堆疊組數 遮罩
CtmWnd*   pwndBtn_PileNum[10] = {NULL}; // 堆疊組數 Btn
CtmWnd*   pwndBtn_PileChoose[10] = {NULL}; // 堆疊組數 選擇鈕

char* PileNum_String[] = // 堆疊組數 名稱
{
	"",
	"Btn_PileUse1",
	"Btn_PileUse2",
	"Btn_PileUse3",
	"Btn_PileUse4",
};
char* PileChoose_String[] = // 堆疊組數 選擇鈕 名稱
{
	"",
	"Choose_Pile1",
	"Choose_Pile2",
	"Choose_Pile3",
	"Choose_Pile4",
};




CtmWnd*		pwndStr_Place_Z	= NULL; // 放置/堆疊 文字說明
CtmWnd*		pwndStr_Place_X	= NULL; // 放置/堆疊 文字說明

BOOL b_BtnNextP_OK = 0; // 完成教導 可至下一位置
BOOL b_PosSet_OK[10] = {0}; // 完成教導 設定位置
BOOL b_All_PosSet_OK = 0; //完成教導 全部設定位置 皆設定完成
char* pAll_PosSet_OK_DB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_NUM"; // 位置設置完成 DB
BOOL b_SpdSet_OK[10] = {0}; // 完成教導 設定位置
BOOL b_All_SpdSet_OK = 0; //完成教導 全部設定位置 皆設定完成
BOOL b_ClampSet_OK = OK; // 完成教導 夾具確認

int Teach_SetNum[2][10]= // 各點 所需設定數量 [3,5軸][哪一點]
{
	{
		0, // 0
		0, // 1
		3, // 2
		0, // 3
		1, // 4
		1, // 5
		1, // 6
		2, // 7
		2, // 8	
		1, // 9	
	},
	{
		0, // 0
		0, // 1
		5, // 2
		0, // 3
		2, // 4
		2, // 5
		2, // 6
		4, // 7
		2, // 8	
		1, // 9	
	},
};


char* PosHV_ImgPath[] = // 姿態顯示圖片
{
	"",
	"res_tm640/pic/picker/Clamp_Vertical.bmp", // 垂直 1
	"res_tm640/pic/picker/Clamp_Horizontal.bmp", // 水平 2
	"res_tm640/pic/picker/Clamp_Vertical_Disable.bmp",
	"res_tm640/pic/picker/Clamp_Horizontal_Disable.bmp",
};

char* Axis_ImgPath[] = // 軸 顯示圖片
{
	"res_tm640/pic/picker/Axis_NULL.bmp",
	"res_tm640/pic/picker/Axis_X1.bmp", // Axis_X1
	"res_tm640/pic/picker/Axis_Y1.bmp", // Axis_Y1
	"res_tm640/pic/picker/Axis_Z.bmp", // Axis_Z
	"res_tm640/pic/picker/Axis_X2.bmp", // Axis_X2
	"res_tm640/pic/picker/Axis_Y2.bmp", // Axis_Y2
};
char* QTeach_P_Page[2][20] = // 快速教導點 頁面
{
	{
		"",
		"QTeach_Mode.txt", 							// 1 模式選擇 
		"QTeach_WaitP_Axis3.txt", 			// 2 待機點 
		"", 														// 3 下待機點
		"QTeach_ReadyDownP_Axis3.txt", 	// 4 準備下降點 
		"QTeach_PickDownP_Axis3.txt", 	// 5 取出側下降點
		"QTeach_PickP_Axis3.txt", 			// 6 取出側取件點
		"QTeach_MoveOutP_Axis3.txt", 		// 7 橫出點
		"QTeach_PlaceDownP_Axis3.txt", 	// 8 置物側下降點
		"QTeach_PlaceP_Axis3.txt",			// 9 置物側置物點
	},
	{
		"",
		"QTeach_Mode.txt", // 模式選擇
		"QTeach_WaitP.txt", // 待機點
		"", // 下待機點
		"QTeach_ReadyDownP.txt", // 準備下降點 
		"QTeach_PickDownP.txt", // 取出側下降點
		"QTeach_PickP.txt", // 取出側取件點
		"QTeach_MoveOutP.txt", // 橫出點
		"QTeach_PlaceDownP.txt", // 置物側下降點
		"QTeach_PlaceP.txt", // 置物側置物點
	}
};

/*=================================治具設定=================================*/
int u_status=0;
int u_clamp_onoff[10]={0}; // 治具狀態
int u_detect_onoff[10]={0}; // 治具檢測
int ClampSelect; // 治具選擇 吸1 吸2 夾1 夾2
int ClampDetectSelect; // 治具檢測選擇 吸1 吸2 夾1 夾2

char* ClampSelect_DB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_STEP"; // 治具選擇 DB
char* ClampDetectSelect_DB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_NUM"; // 治具檢測選擇 DB

BOOL b_Select[10] = {FALSE}; // 治具選用與否 對照 u_pszClamp_SelectString[]
BOOL b_DetectSelect[10] = {FALSE}; // 治具檢測選用與否 對照 u_pszDetect_SelectString[]

CtmWnd*   pwndClamp_Select[10] ={NULL}; // 快速教導 治具選擇
CtmWnd*   pwndCheckBox[10] ={NULL}; // 快速教導 治具選擇 CheckBox

CtmWnd*   pwndDetect_Select[10] ={NULL}; // 快速教導 治具檢測選擇

CtmWnd*		pwndClamp_Mask	= NULL; // 治具 遮罩
CtmWnd*		pwndStr_Clamp	= NULL; // 治具 夾具 文字
CtmWnd*		pwndStr_Suck	= NULL; // 治具 吸盤 文字
CtmWnd*		pwndStr_Clamp_Detect1	= NULL; // 治具 檢測 文字1
CtmWnd*		pwndStr_Clamp_DT1	= NULL; // 治具 延時 文字1
CtmWnd*		pwndStr_Clamp_Detect2	= NULL; // 治具 檢測 文字2
CtmWnd*		pwndStr_Clamp_DT2 = NULL; // 治具 延時 文字2

CtmWnd*		pwndDetect_Mask1	= NULL; // 治具檢測選擇 遮罩1
CtmWnd*		pwndDetect_Mask2	= NULL; // 治具檢測選擇 遮罩2
CtmWnd*		pwndStr_Detect1	= NULL; // 治具檢測選擇 文字1
CtmWnd*		pwndStr_Detect2	= NULL; // 治具檢測選擇 文字2


char* u_pszClamp_SelectString[] = // 治具對應號碼
{
	"", // 0
	"",
	"",
	"Select_Clamp1", // 3
	"Select_Clamp2", // 4
	"Select_Suck1", // 5
	"Select_Suck2", // 6
	"",
	"",
	"",
	"",
};
char* u_pszCheckBoxString[] = // 確認 治具選擇 
{
	"",
	"",
	"",
	"Check_Clamp1",
	"Check_Clamp2",
	"Check_Suck1",
	"Check_Suck2",
	"",
	"",
};

char* u_pszDetect_SelectString[] = // 治具檢測對應號碼
{
	"", // 0
	"",
	"",
	"DetectSelect_Clamp1", // 3
	"DetectSelect_Clamp2", // 4
	"DetectSelect_Suck1", // 7
	"DetectSelect_Suck2", // 8
	"",
	"",
	"",
	"",
};

char* CheckBox_ImgPath[] = //  勾勾選擇 顯示圖片
{
	"res_tm640/pic/picker/Check_No.bmp",
	"res_tm640/pic/picker/Check_OK.bmp",
};
char* ChooseBox_ImgPath[] = //  選擇 顯示圖片
{
	"res_tm640/pic/picker/Choose_No.bmp",
	"res_tm640/pic/picker/Choose_OK.bmp",
};


char* Clamp_ImgPath[] = // 治具顯示圖片
{
	"",	// 0
	"",	// 1
	"",	// 2
	"res_tm640/pic/picker/Qclamp1_off.bmp", // 3
	"res_tm640/pic/picker/Qclamp2_off.bmp", //4
	"res_tm640/pic/picker/QCupulaClose1.bmp", // 5
	"res_tm640/pic/picker/QCupulaClose2.bmp",	// 6
	"",	// 7
	"",	// 8
	"", // 9
	"",	// 10
	"",	// 11
	"",	// 12
	"res_tm640/pic/picker/Qclamp1_on.bmp",	// 13
	"res_tm640/pic/picker/Qclamp2_on.bmp",	// 14
	"res_tm640/pic/picker/QCupulaOpen1.bmp", // 15
	"res_tm640/pic/picker/QCupulaOpen2.bmp",	// 16
	"",	// 15
	"",	// 16
	"", // 19
	"",	// 20
	"",	// 21
	"",	// 22
	"", // 23
	"", // 24
	"",	// 25
	"",	// 26
	"", // 27
	"",	// 28
	"", // 29
	"",	// 30
};
char* Clamp_Hint_ImgPath[] = // 治具提示顯示圖片
{
	"",	// 0
	"",	// 1
	"",	// 2
	"res_tm640/pic/picker/Hintclamp1_off.bmp", // 3
	"res_tm640/pic/picker/Hintclamp2_off.bmp", //4
	"res_tm640/pic/picker/HintCupulaClose1.bmp", // 5
	"res_tm640/pic/picker/HintCupulaClose2.bmp",	// 6
	"",	// 7
	"",	// 8
	"", // 9
	"",	// 10
	"",	// 11
	"",	// 12
	"res_tm640/pic/picker/Hintclamp1_on.bmp",	// 13
	"res_tm640/pic/picker/Hintclamp2_on.bmp",	// 14
	"res_tm640/pic/picker/HintCupulaOpen1.bmp", // 15
	"res_tm640/pic/picker/HintCupulaOpen2.bmp",	// 16
	"", // 17
	"",	// 18
	"", // 19
	"",	// 20
	"",	// 21
	"",	// 22
	"", // 23
	"", // 24
	"",	// 25
	"",	// 26
	"", // 27
	"",	// 28
	"", // 29
	"",	// 30
};

CtmWnd*   pwndBtn_Clamp[10] ={NULL}; // 快速教導 治具 Btn
char* u_pszBtn_Clamp_String[] =
{
	"",
	"",
	"",
	"Btn_Clamp1", // 3
	"Btn_Clamp2", // 4
	"Btn_Suck1", // 5
	"Btn_Suck2", // 6
	"",
	"",
};
char* P6_Clamp_onoff_DBString[] = // P6取件點 治具狀態 DB名稱
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_TYPE", // 3 夾具1
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_NUM", // 4 夾具2
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_TYPE", // 5 吸盤1
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_NUM", // 6 吸盤2
	"",
	"",
};
char* P9_Clamp_onoff_DBString[] = // P9放置點 治具狀態 DB名稱
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_TYPE", // 3 夾具1
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_NUM", // 4 夾具2
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_TYPE", // 5 吸盤1
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_NUM", // 6 吸盤2
	"",
	"",
};


CtmWnd*   pwndDetectBox[10] ={NULL}; // 快速教導 治具檢測 CheckBox (單點撿測)
char* u_pszDetectBoxString[] = // 確認 治具檢測選擇 
{
	"",
	"",
	"",
	"Detect_Clamp1",
	"Detect_Clamp2",
	"Detect_Suck1",
	"Detect_Suck2",
	"",
	"",
};

char* P6_Clamp_detect_DBString[] = // P6取件點 治具檢測狀態 DB名稱
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_TYPE", // 3 夾具1
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_NUM", // 4 夾具2
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_TYPE", // 5 吸盤1
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_NUM", // 6 吸盤2
	"",
	"",
};
char* P9_Clamp_detect_DBString[] = // P9放置點 治具檢測狀態 DB名稱
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_TYPE", // 3 夾具1
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_NUM", // 4 夾具2
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_TYPE", // 5 吸盤1
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_NUM", // 6 吸盤2
	"",
	"",
};

CtmWnd*   pwndQTeach_Clamp_DT[10] ={NULL}; // 快速教導 治具延時 數值輸入
char* u_pszQTeach_Clamp_DTString[] =
{
	"",
	"",
	"",
	"QTeach_DT_Clamp1", // 3
	"QTeach_DT_Clamp2", // 4
	"QTeach_DT_Suck1", // 5
	"QTeach_DT_Suck2", // 6
	"",
	"",
};
long l_Clamp_Delaytime[10] = {0};
char* P6_Clamp_DT_DBString[] = // P6取件點 治具延時 DB名稱
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER1", // 3 夾具1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER2", // 4 夾具2
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER3", // 5 吸盤1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER4", // 6 吸盤2
	"",
	"",
};
char* P9_Clamp_DT_DBString[] = // P9取件點 治具延時 DB名稱
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER5", // 3 夾具1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER6", // 4 夾具2
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER7", // 5 吸盤1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER8", // 6 吸盤2
	"",
	"",
};

/*=================================副臂 設定=================================*/
BOOL b_SubArmUse 			 = FALSE; // 副臂 選用與否
BOOL b_SubArmSync			 = FALSE; // 副臂 副臂與主臂同動 與否
int u_SubArmUse_WaitP 		 = 0; // 副臂 增加步數 等待點
int u_SubArmUse_PickDownP  = 0; // 副臂 增加步數 下降點
int u_SubArmUse_PickP			 = 0; // 副臂 增加步數 取件點
int u_SubArmUse_MoveOutP	 = 0; // 副臂 增加步數 橫出點
int u_SubArmUse_SubPlaceP  = 0; // 副臂 增加步數 橫出點 (丟水口)
char* SubStartNo_DB = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_TYPE"; // 放置水口 開始序號 DB
char* Check_SubUse_DB  = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_NUM"; // 確認是否選用 副臂使用 DB
char* Check_SubSync_DB = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_STEP"; // 確認是否選用 副臂與主臂同動 DB

/*=================================移動後下降 設定=================================*/
BOOL b_Check_Move = FALSE; // 移動後下降 選用與否
int u_Check_Move =0; // 移動後下降 增加步數
char* Check_Move_DB = "MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_STEP"; // 確認是否選用 移動後下降 DB

/*==============================柔性使用(軸提前) 設定==============================*/
BOOL b_Check_SLVL = FALSE; // 柔性使用 選用與否
int	 u_Check_SLVL =0; // 柔性使用 數值 第u_Group個bit
char* Check_SLVL_DB = "MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_STEP"; // 確認是否選用 柔性使用 DB
char* P_2_SLVL_Set_DB = "MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_TYPE"; // 從哪一點跳至 柔性設置 DB
char* Pick_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER1"; // 下降取物 提前距離 DB
char* PickOut_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER2"; // 上拉出模 提前距離 DB


/*==========================頂退完成(開始頂出) 設定==============================*/
BOOL b_Check_Ejector = FALSE; // 開始頂出 選用與否
int u_Check_Ejector =0; // 開始頂出 增加步數
char* Check_Ejector_DB = "MACHINE_PROFILE_NUM4_EQUIPMENT2_ACTION_STEP"; // 確認是否選用 開始頂出 DB

/*=================================堆疊 設定=================================*/
int PileNum = 0; // 堆疊 組號碼
int u_PileNum =0; // 堆疊 使用 增加步數
char* Check_Pile_DB = "MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_NUM"; // 確認是否選用 堆疊組號 DB
char* PilePage[] =
{
	"",
	"Pile_1.txt", // 1
	"Pile_2.txt", // 2
	"Pile_3.txt", // 3
	"Pile_4.txt", // 4
};
/*=================================位置設定=================================*/
CtmWnd*   pwndQTeach_Pos[6] ={NULL}; // 快速教導 位置設定
char* u_pszQTeach_PosString[] =
{
	"",
	"QTeach_Pos_X1",
	"QTeach_Pos_Y1",
	"QTeach_Pos_Z",
	"QTeach_Pos_X2",
	"QTeach_Pos_Y2",
};
CtmWnd*   pwndQTeach_Pos_Max[6] ={NULL}; // 快速教導 位置 上下限 顯示文字
CtmWnd*   pwndQTeach_Pos_Min[6] ={NULL};
char* u_pszQTeach_Pos_MaxString[] =
{
	"",
	"Pos_Max_X1",
	"Pos_Max_Y1",
	"Pos_Max_Z",
	"Pos_Max_X2",
	"Pos_Max_Y2",
};
char* u_pszQTeach_Pos_MinString[] =
{
	"",
	"Pos_Min_X1",
	"Pos_Min_Y1",
	"Pos_Min_Z",
	"Pos_Min_X2",
	"Pos_Min_Y2",
};

long l_Position[6] = {0};
char* P2_POSSET_DBString[] = // P2 等待點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P4_POSSET_DBString[] = // P4 準備下降點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P5_POSSET_DBString[] = // P5 取出側下降點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P6_POSSET_DBString[] = // P6 取出測取件點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P7_POSSET_DBString[] = // P7 取出側橫出點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P8_POSSET_DBString[] = // P8 置物側下降點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P9_POSSET_DBString[] = // P9 置物側放置點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER5",
};

/*=================================速度設定=================================*/
CtmWnd*   pwndQTeach_Speed[6] ={NULL}; // 快速教導 速度設定
char* u_pszQTeach_SpeedString[] =
{
	"",
	"QTeach_Speed_X1",
	"QTeach_Speed_Y1",
	"QTeach_Speed_Z",
	"QTeach_Speed_X2",
	"QTeach_Speed_Y2",
};
long l_Speed[5] = {0};
char* P2_Speed_DBString[] = // P2 等待點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P4_Speed_DBString[] = // P4 準備下降點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P5_Speed_DBString[] = // P5 取出側下降點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P6_Speed_DBString[] = // P6 取出測取件點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P7_Speed_DBString[] = // P7 取出側橫出點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P8_Speed_DBString[] = // P8 置物側下降點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P9_Speed_DBString[] = // P9 置物側放置點 速度設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER10",
};

/*=================================姿態設定=================================*/
char*	PosStatus_DBString[]= // 姿態 DB名稱
{
	"",
	"", // P1
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_STEP", // P2 等待點的姿態
	"",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_STEP", // P4
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_STEP", // P5
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_STEP", // P6 取件點的姿態
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_STEP", // P7
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_STEP", // P8
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_STEP", // P9 放置點的姿態
};

/*=================================延時設定=================================*/
CtmWnd*   pwndQTeach_DT[6] ={NULL}; // 快速教導 延時設定 DT-DelatTime
char* u_pszQTeach_DTString[] =
{
	"",
	"QTeach_DT_X1",
	"QTeach_DT_Y1",
	"QTeach_DT_Z",
	"QTeach_DT_X2",
	"QTeach_DT_Y2",
};
long l_Delaytime[6] = {0};
char* P2_DT_DBString[] = // P2 等待點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P4_DT_DBString[] = // P4 準備下降點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P5_DT_DBString[] = // P5 取出側下降點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P6_DT_DBString[] = // P6 取出測取件點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P7_DT_DBString[] = // P7 取出側橫出點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P8_DT_DBString[] = // P8 置物側下降點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P9_DT_DBString[] = // P9 置物側放置點 延時設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER5",
};


int u_Group;
char*	GroupName[] = // 群組名稱 對照畫面腳本標題
{
	"",
	"TEACH_QTEACH_Origin", 			// 1
	"TEACH_QTEACH_WAITP", 			// 2
	"",
	"TEACH_QTEACH_READYDOWNP", 	// 4
	"TEACH_QTEACH_PICKDOWNP", 	// 5 
	"TEACH_QTEACH_PICKP", 			// 6
	"TEACH_QTEACH_MOVEOUTP", 		// 7
	"TEACH_QTEACH_PLACEDOWNP", 	// 8
	"TEACH_QTEACH_PLACEP", 			// 9
};

char* AxisPosNow_DBString[] = // 紀錄現在位置 DB名稱
{
	"",
	"MACHINE_INTERFACE_CONTROL_XAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_YAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_ZAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_X2AXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_Y2AXISPOSITION",
};

int StartNo_R; // 每一點 開始 讀 步驟序號
int StartNo_W; // 每一點 開始 寫 步驟序號
int Action_Step; // 動作的序號
	
int StepNum[20]; // 每一點 步驟數量[Group]

int u_SelectClamp=0; // 選用治具數量
int SelectClamp[10]={0}; // 選用的治具號碼

int u_SelectDetect=0; // 選用治具檢測數量 
int SelectDetect[10]={0}; // 選用的治具檢測號碼 影響 動作命令 檢測 選n 打開

int u_StepParamNum=8; // 每一步參數數量 3+5

/*=================================教導動作列表=================================*/
ACTION_P Action_P[]= // 教導動作列表
{	
// 			QTP,	Step  ACTION_TYPE,	ACTION_NUM, 			P1,	P2, P3, P4, P5,
/*=====================================P2等待點=====================================*/
/* 1*/	{	2,		0, 	Action_Axis,	Axis_Y1,					0,	50,	0,	2,	0, }, //	Y1軸
///* 1*/	{	2,		0,	Action_Axis,	Axis_Y2,					0,	50,	0,	2,	0, }, //	Y2軸 (副臂)
/* 2*/	{	2,		0,	Action_Permit,Permit_MoldClose,	ON,	0,	0,	2,	0, }, //	允許 開模 開
/* 3*/	{	2,		0, 	Action_Axis,	Axis_X1,					0,	50,	0,	2,	0, }, //	X1軸
///* 3*/	{	2,		0, 	Action_Axis,	Axis_X2,					0,	50,	0,	2,	0, }, //	X2軸 (副臂)
/* 4*/	{	2,		0, 	Action_Axis,	Axis_Z,						0,	50,	0,	6,	0, }, //	Z軸
/* 5*/	{	2,		0, 	Action_Valve,	Valve_AxisC_V,		ON,	ON,	0,	2,	0, }, //	閥門 主臂垂直 開 檢測
/* 6*/	{	2,		0, 	Action_Tag,		START,						0,	0,	0,	0,	0, }, //	開始
/* 7*/	{	2,		0, 	Action_Permit,Permit_PushOut,		OFF,0,	0,	2,	0, }, //	允許 頂出 關
/* 8*/	{	2,		0, 	Action_Permit,Permit_PullBack,	OFF,0,	0,	2,	0, }, //	允許 頂退 關
/* 9*/	{	2,		0, 	Action_Wait,	Wait_MoldOpen,		ON,	0,	0,	2,	0, }, //	等待 開模完成
/*=====================================P5下降點=====================================*/
/*10*/	{	5,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	6,	0, }, //	Y1軸 (下降)
///*10*/	{	5,		0, 	Action_Axis,	Axis_Y2,					0,	80,	0,	6,	0, }, //	Y2軸 (下降)
/*=====================================P6取件點=====================================*/
///**/	{	6,		0, 	Action_Valve,	Valve_Choose1,		ON,	0,	0,	6,	0, }, //	閥門 選1 開
///**/	{	6,		0, 	Action_Valve,	Valve_Choose2,		ON,	0,	0,	6,	0, }, //	閥門 選2 開
///**/	{	6,		0, 	Action_Valve,	Valve_Choose3,		ON,	0,	0,	6,	0, }, //	閥門 選3 開
///**/	{	6,		0, 	Action_Valve,	Valve_Choose4,		ON,	0,	0,	6,	0, }, //	閥門 選4 開
/*11*/	{	6,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	6,	0, }, //	X1軸 (前進)
///*11*/	{	6,		0, 	Action_Axis,	Axis_X2,					0,	80,	0,	6,	0, }, //	X2軸 (前進)
/*12*/	{	6,		0, 	Action_Permit,Permit_PushOut,		ON,	0,	0,	6,	0, }, //	允許 頂出 開
///*13*/	{	6,		0, 	Action_Wait,	Wait_PushOut,			ON,	0,	0,	6,	0, }, //	等待 頂出完成 開
/*14*/	{	6,		0, 	Action_Permit,Permit_PullBack,	ON,	0,	0,	6,	0, }, //	允許 頂退 開
/*=====================================P7橫出點=====================================*/
/*15*/	{	7,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	7,	0, }, //	X1軸 (後退)
///*15*/	{	7,		0, 	Action_Axis,	Axis_X2,					0,	80,	0,	7,	0, }, //	X2軸 (後退)
/*16*/	{	7,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	7,	0, }, //	Y1軸 (上拉)
///*16*/	{	7,		0, 	Action_Axis,	Axis_Y2,					0,	80,	0,	7,	0, }, //	Y2軸 (上拉)
///**/	{	7,		0, 	Action_Detect,Valve_Choose1,		ON,	0,	0,	7,	0, }, //	檢測 選1 打開
///**/	{	7,		0, 	Action_Detect,Valve_Choose2,		ON,	0,	0,	7,	0, }, //	檢測 選2 打開
///**/	{	7,		0, 	Action_Detect,Valve_Choose3,		ON,	0,	0,	7,	0, }, //	檢測 選3 打開
///**/	{	7,		0, 	Action_Detect,Valve_Choose4,		ON,	0,	0,	7,	0, }, //	檢測 選4 打開
/*17*/	{	7,		0, 	Action_Permit,Permit_MoldClose,	ON,	0,	0,	7,	0, }, //	允許 關模 開
/*18*/	{	7,		0, 	Action_Permit,Permit_PushOut,		OFF,0,	0,	7,	0, }, //	允許 頂出 關
/*19*/	{	7,		0, 	Action_Valve,	Valve_AxisC_H,		ON,	ON,	0,	7,	0, }, //	閥門 主臂水平 開 檢測
/*20*/	{	7,		0, 	Action_Permit,Permit_MoldOpen,	ON,	0,	0,	7,	0, }, //	允許 開模 開
///**/	{	0,		0, 	丟水口,				AxisZ,X2,Y2,			0,	0,	0,	0,	0, }, //  丟水口
/*=====================================P8下降點=====================================*/
///**/	{	8,		0, 	Action_Pile,	Pile1,						ON,	0,	0,	8,	0, }, //  堆疊 推疊1 開始 
/*21*/	{	8,		0, 	Action_Axis,	Axis_Z,						0,	80,	0,	9,	0, }, //	Z軸  (橫出)
/*22*/	{	8,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	9,	0, }, //	X1軸 (放置/堆疊點)
/*=====================================P9放置點=====================================*/
/*23*/	{	9,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	9,	0, }, //	Y1軸 (放置/堆疊點)
///**/	{	9,		0, 	Action_Pile,	Pile1,						OFF,	0,	0,	9,	0, }, //  堆疊 推疊1 結束
///**/	{	9,		0, 	Action_Valve,	Valve_Choose1,		OFF,0,	0,	9,	0, }, //	閥門 選1 關
///**/	{	9,		0, 	Action_Valve,	Valve_Choose2,		OFF,0,	0,	9,	0, }, //	閥門 選2 關
///**/	{	9,		0, 	Action_Valve,	Valve_Choose3,		OFF,0,	0,	9,	0, }, //	閥門 選3 關
///**/	{	9,		0, 	Action_Valve,	Valve_Choose4,		OFF,0,	0,	9,	0, }, //	閥門 選4 關
/*=====================================P10回等待點=====================================*/
/*24*/	{	10,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	0,	0, }, //	Y1軸
/*25*/	{	10,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	0,	0, }, //	X1軸
///*25*/	{	10,		0, 	Action_Axis,	Axis_X2,					0,	80,	0,	0,	0, }, //	X2軸 (副臂)
/*26*/	{	10,		0, 	Action_Axis,	Axis_Z,						0,	80,	0,	0,	0, }, //	Z軸
/*27*/	{	10,		0, 	Action_Valve,	Valve_AxisC_V,		ON,	ON,	0,	0,	0, }, //	閥門 主臂垂直 開 檢測
/*28*/	{	10,		0, 	Action_Goto,		END,						0,	0,	0,	0,	0, }, //	結束
};

ACTION_P Action_Axis_P[]= // 教導動作 軸動作
{
	{	0,	0,	Action_Axis,	0,								0,	0,	0,	0,	0, }, //	0軸
	{	0,	0, 	Action_Axis,	Axis_X1,					0,	0,	0,	0,	0, }, //	X1軸
	{	0,	0, 	Action_Axis,	Axis_Y1,					0,	0,	0,	0,	0, }, //	Y1軸
	{	0,	0, 	Action_Axis,	Axis_Z ,					0,	0,	0,	0,	0, }, //	Z軸
	{	0,	0, 	Action_Axis,	Axis_X2,					0,	0,	0,	0,	0, }, //	X2軸
	{	0,	0, 	Action_Axis,	Axis_Y2,					0,	0,	0,	0,	0, }, //	Y2軸
};

ACTION_P Action_Valve_P[]= // 教導動作 閥門
{
	{	0,	0,	Action_Valve,	Valve_Choose1,		ON,	0,	0,	0,	0, }, //	閥門 選1 開
	{	0,	0,	Action_Valve,	Valve_Choose2,		ON,	0,	0,	0,	0, }, //	閥門 選2 開
	{	0,	0,	Action_Valve,	Valve_Choose3,		ON,	0,	0,	0,	0, }, //	閥門 選3 開
	{	0,	0,	Action_Valve,	Valve_Choose4,		ON,	0,	0,	0,	0, }, //	閥門 選4 開
};
ACTION_P Action_Detect_P[]= // 教導動作 檢測
{
	{	0,	0,	Action_Detect,Valve_Choose1,		ON,	0,	0,	0,	0, }, //	檢測 選1 打開
	{	0,	0,	Action_Detect,Valve_Choose2,		ON,	0,	0,	0,	0, }, //	檢測 選2 打開
	{	0,	0,	Action_Detect,Valve_Choose3,		ON,	0,	0,	0,	0, }, //	檢測 選3 打開
	{	0,	0,	Action_Detect,Valve_Choose4,		ON,	0,	0,	0,	0, }, //	檢測 選4 打開
};

ACTION_P Action_Wait_P[]= // 教導動作 等待
{
	{	0,	0,	Action_Wait,	0,								ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_IMEStop,			ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_MoldOpen,		ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_MoldClose,		ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_SDoorClose,	ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_IMAuto,			ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_PushOut,			ON,	0,	0,	0,	0, }, //	等待
	{	0,	0,	Action_Wait,	Wait_PullBack,		ON,	0,	0,	0,	0, }, //	等待
};


ACTION_P Action_Pile_P[]= // 教導動作 堆疊
{
	{	0,	0,	Action_Pile,						0,		ON,	0,	0,	0,	0, }, //  堆疊 推疊0 開始
	{	0,	0,	Action_Pile,						1,		ON,	0,	0,	0,	0, }, //  堆疊 推疊1 開始
	{	0,	0,	Action_Pile,						2,		ON,	0,	0,	0,	0, }, //  堆疊 推疊2 開始
	{	0,	0,	Action_Pile,						3,		ON,	0,	0,	0,	0, }, //  堆疊 推疊3 開始
	{	0,	0,	Action_Pile,						4,		ON,	0,	0,	0,	0, }, //  堆疊 推疊4 開始
};


/*=================================教導動作列表=================================*/

int Action_P_ListNum = ( sizeof(Action_P) / sizeof(Action_P[0]) ); // 教導動作列表數量

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : OnCreateA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{	
	char	m_pszTitleID[256];	memset(m_pszTitleID, 0, sizeof(m_pszTitleID));
	pwndSender->GetPropValueT("titleID", m_pszTitleID, sizeof(m_pszTitleID));
	printf("Now at %s\n",m_pszTitleID);

	// 取得群組號碼 (位置代碼)
	for(int i=0; i<sizeof(GroupName)/sizeof(GroupName[0]); i++)
	{
		if(strcmp(m_pszTitleID, GroupName[i]) == 0)
			u_Group=i;
	}
	printf("Group=%d\n",u_Group);
	
	// 取得 機構設定 3 or 5軸
	//u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);// 讀取設定 機型選擇 三五軸
	u_PickerType = dw_MechType & MechWord;
	
	for(int i=0;i<7;i++)
	{
		printf("%s=%d\n",P6_Clamp_DT_DBString[i],(GetDBValue(P6_Clamp_DT_DBString[i]).lValue));
	}
	
	
	
	printf("u_PickerType=%d\n",u_PickerType);
	if(u_PickerType==MechType5)
		u_SubArmUse_WaitP 		 = 2; // 副臂 增加步數 等待點
	if(u_PickerType==MechType3)
		SetDBValue(Check_SubUse_DB,FALSE);	
	
	// 取得指標 模式設定 治具選擇
	for(int i = 0; i < sizeof(u_pszClamp_SelectString)/sizeof(u_pszClamp_SelectString[0]); i++ )
	{
		pwndClamp_Select[i] = pwndSender->FindControlFromName(u_pszClamp_SelectString[i]); // 治具選擇
		pwndCheckBox[i] = pwndSender->FindControlFromName(u_pszCheckBoxString[i]); // 治具選擇 勾勾
		pwndDetect_Select[i] = pwndSender->FindControlFromName(u_pszDetect_SelectString[i]); // 治具檢測選擇 (動作命令)
		pwndDetectBox[i] = pwndSender->FindControlFromName(u_pszDetectBoxString[i]);	// 治具撿測 勾勾 (單點)
		pwndQTeach_Clamp_DT[i] = pwndSender->FindControlFromName(u_pszQTeach_Clamp_DTString[i]);// 治具延時
	} 
	// 取得指標 模式設定 治具選擇 遮罩
	pwndClamp_Mask	= pwndSender->FindControlFromName("Clamp_Mask");
	
	pwndStr_Clamp	= pwndSender->FindControlFromName("Str_Clamp");
	pwndStr_Suck	= pwndSender->FindControlFromName("Str_Suck");
	pwndStr_Clamp_Detect1	= pwndSender->FindControlFromName("Str_Clamp_Detect1");
	pwndStr_Clamp_DT1	= pwndSender->FindControlFromName("Str_Clamp_DT1");
	pwndStr_Clamp_Detect2	= pwndSender->FindControlFromName("Str_Clamp_Detect2");
	pwndStr_Clamp_DT2	= pwndSender->FindControlFromName("Str_Clamp_DT2");

	
	// 取得指標 模式設定 治具檢測選擇 遮罩  
	pwndDetect_Mask1	= pwndSender->FindControlFromName("Detect_Mask1");
	pwndDetect_Mask2	= pwndSender->FindControlFromName("Detect_Mask2");
	// 取得指標 模式設定 治具檢測選擇 文字  
	pwndStr_Detect1	= pwndSender->FindControlFromName("Str_Detect1");
	pwndStr_Detect2	= pwndSender->FindControlFromName("Str_Detect2");
	


	// 取得指標 座標位置設定
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		pwndQTeach_Pos[i] = pwndSender->FindControlFromName(u_pszQTeach_PosString[i]);
	} 
	// 取得座標位置 上下限顯示文字
	for(int i = 0; i < sizeof(u_pszQTeach_Pos_MaxString)/sizeof(u_pszQTeach_Pos_MaxString[0]); i++ )
	{
		pwndQTeach_Pos_Max[i] = pwndSender->FindControlFromName(u_pszQTeach_Pos_MaxString[i]);
	} 	
	for(int i = 0; i < sizeof(u_pszQTeach_Pos_MinString)/sizeof(u_pszQTeach_Pos_MinString[0]); i++ )
	{
		pwndQTeach_Pos_Min[i] = pwndSender->FindControlFromName(u_pszQTeach_Pos_MinString[i]);
	} 	
	
	// 取得指標 速度設定
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		pwndQTeach_Speed[i] = pwndSender->FindControlFromName(u_pszQTeach_SpeedString[i]);
	}
	// 取得指標 延時設定
	for(int i = 0; i < sizeof(u_pszQTeach_DTString)/sizeof(u_pszQTeach_DTString[0]); i++ )
	{
		pwndQTeach_DT[i] = pwndSender->FindControlFromName(u_pszQTeach_DTString[i]);
	}	
		
	// 取得指標 姿態
	pwndBtn_PosHV = pwndSender->FindControlFromName("Btn_PosHV");
	
	// 取得指標 位置提示 圖片
	for(int i = 0; i < sizeof(Bar_PosHint_String)/sizeof(Bar_PosHint_String[0]); i++ )
	{
		pwndBar_PosHint[i] = pwndSender->FindControlFromName(Bar_PosHint_String[i]);
	}
	// 取得指標 位置提示 圖片
	for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
	{
		pwndImg_PosHint[i] = pwndSender->FindControlFromName(Img_PosHint_String[i]);
	}
	// 取得指標 位置提示 遮罩
	for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
	{
		pwndMask_PosHint[i] = pwndSender->FindControlFromName(Mask_PosHint_String[i]);
	}
	
	// 取得指標 治具 ON/OFF Btn
	for(int i = 0; i < sizeof(u_pszBtn_Clamp_String)/sizeof(u_pszBtn_Clamp_String[0]); i++ )
	{
		pwndBtn_Clamp[i] = pwndSender->FindControlFromName(u_pszBtn_Clamp_String[i]);
	} 

	// 取得指標 獲取位置按鈕
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ )
	{
		pwndBtn_GetPos[i] = pwndSender->FindControlFromName(GetPos_String[i]);
	} 

	
	// 取得指標 現在移動軸 顯示 圖片
	pwndImg_NowMoveAxis	= pwndSender->FindControlFromName("Img_NowMoveAxis");	
	// 取得指標 現在移動軸 顯示 文字
	pwndStr_NowMoveAxis	= pwndSender->FindControlFromName("Str_NowMoveAxis");	
	
	// 取得指標 開始教導按鈕
	pwndBtnTeachStart	= pwndSender->FindControlFromName("BtnTeachStart");	
	// 取得指標 下一位置按鈕
	pwndBtnNextP	= pwndSender->FindControlFromName("BtnNextP");	
	// 取得指標 治具選用文字
	pwndStr_ClampSelect	= pwndSender->FindControlFromName("Str_ClampSelect");	
	
	// 取得指標 移動後下降 勾勾
	pwndCheck_Move	= pwndSender->FindControlFromName("Check_Move");	
	// 取得指標 移動後下降 btn
	pwndBtn_Check_Move	= pwndSender->FindControlFromName("Btn_Check_Move");
	b_Check_Move = GetDBValue(Check_Move_DB).lValue;
	if(b_Check_Move)
		{
			if(u_PickerType==MechType5)
				u_Check_Move 		 = 2; // 多2步驟
			else if(u_PickerType==MechType3)
				u_Check_Move 		 = 1; // 多1步驟				
		}
	if(b_Check_Move==0 && u_Group==ReadyDownP)
		b_All_PosSet_OK=OK; // 位置設定完成
		
	// 取得指標 副臂使用 勾勾
	pwndCheck_SubUse	= pwndSender->FindControlFromName("Check_SubUse");	
	b_SubArmUse = GetDBValue(Check_SubUse_DB).lValue;	
	if(b_SubArmUse)
		{
			u_SubArmUse_PickDownP  = 1; // 副臂 增加步數 下降點
			u_SubArmUse_PickP			 = 1; // 副臂 增加步數 取件點
			u_SubArmUse_MoveOutP	 = 2; // 副臂 增加步數 橫出點
			u_SubArmUse_SubPlaceP  = 6; // 副臂 增加步數 橫出點 (丟水口)
		}
	// 取得指標 副臂與主臂同動 勾勾
	pwndCheck_SubSync	= pwndSender->FindControlFromName("Check_SubSync");	
	b_SubArmSync = GetDBValue(Check_SubSync_DB).lValue;	
	// 取得指標 副臂使用 Btn
	pwndBtn_SubUse	= pwndSender->FindControlFromName("Btn_SubUse");
	// 取得指標 副臂與主臂同動 Btn
	pwndBtn_SubSync	= pwndSender->FindControlFromName("Btn_SubSync");
	// 取得指標 副臂 遮罩
	pwndMask_SubArm	= pwndSender->FindControlFromName("Mask_SubArm");
	 
	// 取得指標 柔性使用 勾勾
	pwndCheck_SLVL	= pwndSender->FindControlFromName("Check_SLVL");	
	// 取得指標 柔性使用 btn
	pwndBtn_SLVL	= pwndSender->FindControlFromName("Btn_SLVL");	
	u_Check_SLVL = GetDBValue(Check_SLVL_DB).lValue; // 取得db數值
	b_Check_SLVL = (u_Check_SLVL>>u_Group) & 1; // 確認此u_Group bit 是否為1
	//printf("u_Check_SLVL=%x, b_Check_SLVL=%d\n",u_Check_SLVL,b_Check_SLVL);

	// 取得指標 取件柔性/取出柔性 數值設定 
	pwndPick_SLVL 	 = pwndSender->FindControlFromName("Pick_SLVL");
	pwndPickOut_SLVL = pwndSender->FindControlFromName("PickOut_SLVL");
	// 取得指標 下降距離/後退距離 數值顯示 
	pwndPick_Dis 	 = pwndSender->FindControlFromName("Pick_Dis");
	pwndPickOut_Dis = pwndSender->FindControlFromName("PickOut_Dis");
	// 取得指標 柔性使用 Logo
	pwndSLVL_Logo = pwndSender->FindControlFromName("SLVL_Logo");	
	// 取得指標 柔性設置 遮罩
	pwndMask_SLVL = pwndSender->FindControlFromName("Mask_SLVL");	
	
	
	// 取得指標 移動後下降 顯示圖片
	pwndImg_QTeach_ReadyDownP	= pwndSender->FindControlFromName("Img_QTeach_ReadyDownP");
	
	// 取得指標 檢測 頂退完成(開始頂出) 選擇勾勾
	pwndCheck_Ejector 	 = pwndSender->FindControlFromName("Check_Ejector");
	u_Check_Ejector = GetDBValue(Check_Ejector_DB).lValue; // 取得db數值
	b_Check_Ejector = u_Check_Ejector & 1; // 確認此u_Group bit 是否為1
	printf("u_Check_Ejector=%x, b_Check_Ejector=%d\n",u_Check_Ejector,b_Check_Ejector);
	
	// 取得指標 堆疊選用
	pwndCheck_Pile = pwndSender->FindControlFromName("Check_Pile"); // 堆疊選用
	pwndBtn_PileUse	= pwndSender->FindControlFromName("Btn_PileUse"); // 堆疊選用 Btn 
	// 取得指標 堆疊組數 選擇
	for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ )
	{
		pwndBtn_PileNum[i] = pwndSender->FindControlFromName(PileNum_String[i]); // 堆疊組數
	} 
	// 取得指標 堆疊組數 選擇鈕
	for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ )
	{
		pwndBtn_PileChoose[i] = pwndSender->FindControlFromName(PileChoose_String[i]); // 堆疊組數 選擇鈕
	} 
	// 取得指標 堆疊組數 遮罩
	pwndMask_Pile = pwndSender->FindControlFromName("Mask_Pile"); // 堆疊組數 遮罩
	
	PileNum  = GetDBValue(Check_Pile_DB).lValue;
	printf("PileNum=%d\n",PileNum);
	if(PileNum>0)
		u_PileNum=1; // 多一步驟
	
	// 放置/堆疊 文字說明
	pwndStr_Place_Z = pwndSender->FindControlFromName("Str_Place_Z"); // 位置Z
	pwndStr_Place_X = pwndSender->FindControlFromName("Str_Place_X"); // 位置X
	
	
	int All_PosSet_OK = GetDBValue(pAll_PosSet_OK_DB).lValue; // 返回前一頁 不需再次確認設定值
	if(All_PosSet_OK)
		b_All_PosSet_OK=OK;
		
	if(u_Group==PickDownP)
		{
			GetDownDis();	
			ShowData(pwndPick_SLVL); // 顯示 設定數值
		}
	if(u_Group==MoveOutP)
		{
			GetBackDis();
			ShowData(pwndPickOut_SLVL); // 顯示 設定數值
		}
		
	/*=====================================顯示init()==================================*/	
	// Update 顯示
	UpdateShow();
	
	// 計算各點步驟
	printf("Action_P[] = %d\n",Action_P_ListNum);
	for(int Group=0;Group<20;Group++)
	{
		StepNum[Group]=0;
		for(int i=0;i<Action_P_ListNum;i++)
		{
			if(Action_P[i].QTP == Group)
				{
					printf("Action_P[%d].QTP=%d\n",i,Action_P[i].QTP);
					StepNum[Group]++;
				}	
		}
		printf("StepNum[%d]=%d\n",Group,StepNum[Group]);
	}

	if(u_Group==2)
		{
			int getvalue = GetDBValue(P2_POSSET_DBString[Axis_Y2]).lValue;
			printf("OnCreateA %s=%d\n",P2_POSSET_DBString[Axis_Y2],getvalue);
		}
	
	// 設定速度預設值 80
	long ltemp=0;
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		if(pwndQTeach_Speed[i]!=NULL)
		{
			pwndQTeach_Speed[i]->GetPropValueT("value", &ltemp,sizeof(ltemp));
			if(ltemp==0)
			{
				pwndQTeach_Speed[i]->SetPropValueT("value",80); // 寫入數值 80
				pwndQTeach_Speed[i]->Update();
			}
		}
	}
	return TRUE;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{              
	printf("OnChange\n");
	//taborder 5-9 是設定位置
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl); // 獲取此taborder的元件
	if(pwnd!=NULL)
	{
		switch(wIDControl)
		{
			case 0:
				b_SpdSet_OK[Axis_Z]=OK;
				break;
			case 1:
				b_SpdSet_OK[Axis_X1]=OK;
				break;
			case 2:
				b_SpdSet_OK[Axis_Y1]=OK;
				break;
			case 3:
				b_SpdSet_OK[Axis_X2]=OK;
				break;
			case 4:
				b_SpdSet_OK[Axis_Y2]=OK;
				break;
			case 5:
				b_PosSet_OK[Axis_Z]=OK;
				break;
			case 6:
				b_PosSet_OK[Axis_X1]=OK;
				if(u_Group==MoveOutP)
					{
						int value=0;
						pwndQTeach_Pos[Axis_X1]->GetPropValueT("value", &value,sizeof(value));
						SetDBValue(P7_POSSET_DBString[Axis_X1], value); // 設定位置
						if(b_Check_SLVL)
							GetBackDis();
					}
				break;
			case 7:
				b_PosSet_OK[Axis_Y1]=OK;
				if(u_Group==PickDownP)
					{
						int value=0;
						pwndQTeach_Pos[Axis_Y1]->GetPropValueT("value", &value,sizeof(value));
						SetDBValue(P5_POSSET_DBString[Axis_Y1], value); // 設定位置
						if(b_Check_SLVL)
							GetDownDis();
					}
				break;
			case 8:
				b_PosSet_OK[Axis_X2]=OK;
				break;
			case 9:
				b_PosSet_OK[Axis_Y2]=OK;
				break;
			default:
				break;
		}
		UpdateHint();
	}
	return wIDControl;
}
/*---------------------------------------------------------------------------+
|  Function : OnKeyA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA=%d\n",wKey);
	if(u_Group < wKey) // 往下一位置 才會儲存
	{
		b_SaveFlag = TRUE;
		SetDBValue(pAll_PosSet_OK_DB, 0); // 才會提示 設定位置
	}
	else 
	{
		b_SaveFlag = FALSE;
		SetDBValue(pAll_PosSet_OK_DB, 1);
	}
	switch(wKey)
	{
		case 1:
			::PutCommand("QTeach_Mode.txt");
			break;
		/*=================================WaitP===================================*/
		case WaitP:
			::PutCommand(QTeach_P_Page[u_PickerType][WaitP]);
			break;
		/*===============================ReadyDownP===============================*/
		case ReadyDownP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // 往上一頁 或是 設定完成往下一頁
				::PutCommand(QTeach_P_Page[u_PickerType][ReadyDownP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			break;
		/*===============================PickDownP===============================*/
		case PickDownP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // 往上一頁 或是 設定完成往下一頁
				::PutCommand(QTeach_P_Page[u_PickerType][PickDownP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			break;
		/*===============================PickP===============================*/
		case PickP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // 往上一頁 或是 設定完成往下一頁
				::PutCommand(QTeach_P_Page[u_PickerType][PickP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			break;
		/*===============================MoveOutP===============================*/
		case MoveOutP:
			if( (u_Group>MoveOutP) && (b_SubArmUse==1) )
				::PutCommand("QTeach_SubPlaceP.txt");
			else
			{
				if( (u_Group > wKey) || b_BtnNextP_OK) // 往上一頁 或是 設定完成往下一頁
					::PutCommand(QTeach_P_Page[u_PickerType][MoveOutP]);
				else
					MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			}
			break;
		/*===============================PlaceDownP===============================*/
		case PlaceDownP:
			if( (u_Group<PlaceDownP) && (b_SubArmUse==1) )
				::PutCommand("QTeach_SubPlaceP.txt");
			else
			{
				if( (u_Group > wKey) || b_BtnNextP_OK) // 往上一頁 或是 設定完成往下一頁
					::PutCommand(QTeach_P_Page[u_PickerType][PlaceDownP]);
				else
					MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			}
			break;
		/*===============================PlaceP===============================*/
		case PlaceP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // 往上一頁 或是 設定完成往下一頁
				::PutCommand(QTeach_P_Page[u_PickerType][PlaceP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			break;
		case 10: // 完成
			if(b_BtnNextP_OK) // 設定完成往下一頁
			{
				if(PileNum>0) //若有選擇堆疊
					{
						g_Pile_Select = PileNum; // 堆疊 第n組
						::PutCommand(PilePage[PileNum]); // 跳轉至堆疊第n組 參數設定頁
					}
				else
					{
						::PutCommand("QTeach_Done.txt"); // 跳轉至 完成教導頁
					}
			}
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 提示返回
			break;
		case 98: // 確認模式
			Clean_QTeach();// 清除快速教導程式
			SaveMode();
			::PutCommand(QTeach_P_Page[u_PickerType][WaitP]);
			break;
		case 99: // 取消教導
			::PutCommand("Index.txt");
			break;
		case 7178: // 設定數字後的確認鍵
			break;
		default:
			break;
	}
	
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
 		
 	for(int i=0; i<10; i++) // 治具選擇 Btn按下彈起
 	{
	 	if((pwnd == pwndClamp_Select[i])||(pwnd == pwndCheckBox[i])) // 治具選擇
	 	{
			b_Select[i] = !b_Select[i];
			pwndCheckBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Select[i]]);
			pwndCheckBox[i]->CreateA();
			pwndCheckBox[i]->Update();
			UpdateBtnTeachStart();
			b_DetectSelect[i]=0;
			UpdateBtnDetect_Select(); // 更新 治具 檢測選擇
	 	}
	 	//printf("b_Select[%d]=%d\n",i,b_Select[i]);
	 	if(pwnd == pwndDetect_Select[i])
 		{		
 			b_DetectSelect[i] = !b_DetectSelect[i];
			pwndDetect_Select[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_DetectSelect[i]]);
			pwndDetect_Select[i]->CreateA();
			pwndDetect_Select[i]->Update();
 		}
 	} 
 	 	
	if(pwnd == pwndBtn_PosHV) // 姿態 Btn按下彈起
	{
		printf("Btn_PosHV\n");
		u_status++;
		if(u_status>2)
			u_status=1;
		printf("u_status=%d\n",u_status);
		switch(u_Group)
		{
			case WaitP: //等待點
				SetDBValue(PosStatus_DBString[WaitP],u_status); // 等待點的姿態
				SetDBValue(PosStatus_DBString[ReadyDownP],u_status+2); // 準備下降點的姿態
				SetDBValue(PosStatus_DBString[PickDownP],u_status+2); // 取件側下降點的姿態
				SetDBValue(PosStatus_DBString[PickP],u_status+2); // 取件點的姿態
				break;// 姿態
			case MoveOutP: // 橫出點
				SetDBValue(PosStatus_DBString[MoveOutP],u_status); // 橫出點的姿態
				SetDBValue(PosStatus_DBString[PlaceDownP],u_status); // 置物側下降點的姿態
				break;
			case PlaceP: //放置點
				SetDBValue(PosStatus_DBString[PlaceP],u_status); // 放置點的姿態
				break;
			default:
				break;
		}
		if(pwndBtn_PosHV != NULL)
		{
		 pwndBtn_PosHV->SetPropValueT("upbitmap",PosHV_ImgPath[u_status]);
		 pwndBtn_PosHV->CreateA();
		 pwndBtn_PosHV->Update();
		}
	}
	
	for(int i=0;i<10;i++) // 治具 Btn按下彈起
	{
		if(pwnd == pwndBtn_Clamp[i])
		{
			u_clamp_onoff[i]++;
			if(u_clamp_onoff[i]>1)
				u_clamp_onoff[i]=0;
			printf("%s=%d\n",u_pszBtn_Clamp_String[i],u_clamp_onoff[i]);
			switch(u_Group) // 紀錄 ON/OFF狀態
			{
				case 6: // 取件點
					SetDBValue(P6_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					printf("Set %s = %d\n",P6_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					break;
				case 9: // 放置點
					SetDBValue(P9_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					printf("Set %s = %d\n",P6_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					break;
				default:
					break;
			}
			if(pwndBtn_Clamp[i] != NULL) // 顯示 夾具1 ON/OFF狀態
			{
				pwndBtn_Clamp[i]->SetPropValueT("upbitmap",Clamp_ImgPath[(u_clamp_onoff[i]*10+i)]);
			 pwndBtn_Clamp[i]->CreateA();
			 pwndBtn_Clamp[i]->Update();
			}
			b_ClampSet_OK=OK;
			UpdateBtnNextP(); // 更新提示顯示
		}		
	}
	
 	for(int i=0; i<10; i++) // 治具檢測 Btn按下彈起
 	{
		if(pwnd == pwndDetectBox[i]) // 治具檢測選擇
		{
			u_detect_onoff[i]++;
			if(u_detect_onoff[i]>1)
				u_detect_onoff[i]=0;
			printf("%s=%d\n",u_pszDetectBoxString[i],u_detect_onoff[i]);
			switch(u_Group) // 紀錄 ON/OFF狀態
			{
				case 6: // 取件點
					SetDBValue(P6_Clamp_detect_DBString[i],u_detect_onoff[i]);
					printf("Set %s = %d\n",P6_Clamp_detect_DBString[i],u_detect_onoff[i]);
					break;
				case 9: // 放置點
					SetDBValue(P9_Clamp_detect_DBString[i],u_detect_onoff[i]);
					printf("Set %s = %d\n",P9_Clamp_detect_DBString[i],u_detect_onoff[i]);
					break;
				default:
					break;
			}
			if(pwndDetectBox[i] != NULL) // 顯示 夾具1 ON/OFF狀態
			{
			 pwndDetectBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[u_detect_onoff[i]]);
			 pwndDetectBox[i]->CreateA();
			 pwndDetectBox[i]->Update();
			}
		}	
	 	
 	}
	
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ ) // 獲取位置 Btn按下彈起
	{
		if(pwnd == pwndBtn_GetPos[i])
		{
			GetPosNow(i);
			b_PosSet_OK[i]=OK;
			UpdateHint();
			
			if(u_Group==PickDownP)
				{
					int value=0;
					pwndQTeach_Pos[Axis_Y1]->GetPropValueT("value", &value,sizeof(value));
					SetDBValue(P5_POSSET_DBString[Axis_Y1], value); // 設定位置
					if(b_Check_SLVL)
						GetDownDis();
				}
			if(u_Group==MoveOutP)
				{
					int value=0;
					pwndQTeach_Pos[Axis_X1]->GetPropValueT("value", &value,sizeof(value));
					SetDBValue(P7_POSSET_DBString[Axis_X1], value); // 設定位置
					if(b_Check_SLVL)
						GetBackDis();
				}
		}
	} 
	
	if( (pwnd == pwndCheck_Move) || (pwnd == pwndBtn_Check_Move) ) // 前進後下降 勾勾 || Btn 按下彈起
	{
		b_Check_Move = !b_Check_Move;
		SetDBValue(Check_Move_DB,b_Check_Move);
		pwndCheck_Move->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Move]);
		pwndCheck_Move->CreateA();
		pwndCheck_Move->Update();
//		if(b_Check_Move)
//		{
//			b_All_PosSet_OK=0; // 需設定位置
//			UnLockBtn(pwndQTeach_Pos[Axis_X1]);
//			UnLockBtn(pwndQTeach_Speed[Axis_X1]);
//			UnLockBtn(pwndQTeach_DT[Axis_X1]);
//			UnLockBtn(pwndBtn_GetPos[Axis_X1]);
//			pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_ReadyDownP.bmp");
//			pwndImg_QTeach_ReadyDownP->Show();
//		}
//		else
//		{
//			b_All_PosSet_OK=OK; // 位置設定完成
//			LockBtn(pwndQTeach_Pos[Axis_X1]);
//			LockBtn(pwndQTeach_Speed[Axis_X1]);
//			LockBtn(pwndQTeach_DT[Axis_X1]);
//			LockBtn(pwndBtn_GetPos[Axis_X1]);
//			pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_WaitP.bmp");
//			pwndImg_QTeach_ReadyDownP->Show();
//			SetFromWaitP(Axis_X1);// 取用 WaitP X1的設定
//		}
		UpdateHint();
		UpdateBtnNextP();
	}
	
	if( (pwnd == pwndCheck_SubUse) || (pwnd == pwndBtn_SubUse) ) // 副臂使用 Btn按下彈起
	{
		b_SubArmUse = !b_SubArmUse;
		SetDBValue(Check_SubUse_DB,b_SubArmUse);
		if(!b_SubArmUse) // 不選擇 副臂使用 時 副臂同動自動取消
			{
				b_SubArmSync=FALSE;
				SetDBValue(Check_SubSync_DB,b_SubArmSync);
			}
		UpdateSubArm();
	}
	if(pwnd == pwndCheck_SubSync || (pwnd == pwndBtn_SubSync) ) // 副臂與主臂同動 Btn按下彈起
	{
		b_SubArmSync = !b_SubArmSync;
		SetDBValue(Check_SubSync_DB,b_SubArmSync);
		UpdateSubArm();
	}
	
	if( (pwnd == pwndCheck_SLVL) || (pwnd == pwndBtn_SLVL) ) // 柔性使用 勾勾 || Btn 按下彈起
	{
		b_Check_SLVL = !b_Check_SLVL;
		
		u_Check_SLVL = u_Check_SLVL & ~(1<<u_Group); // 清空該bit
		if(b_Check_SLVL)
			{
				u_Check_SLVL = u_Check_SLVL | (1<<u_Group); // 設定 該 bit 為 b_Check_SLVL
			}
		//printf("u_Check_SLVL=%x\n",u_Check_SLVL);
		
		SetDBValue(Check_SLVL_DB,u_Check_SLVL);
		
		pwndCheck_SLVL->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_SLVL]);
		pwndCheck_SLVL->CreateA();
		pwndCheck_SLVL->Update();
		if(b_Check_SLVL)
		{
			if(u_Group==PickDownP)
			{
				GetDownDis();
				UnLockBtn(pwndPick_SLVL); // 解鎖 取件柔性 數值設定
			}
			if(u_Group==MoveOutP)
			{
				GetBackDis();
				UnLockBtn(pwndPickOut_SLVL); // 解鎖 取出柔性 數值設定
			}
		}
		else
		{
			LockBtn(pwndPick_SLVL); // 上鎖 取件柔性 數值設定
			LockBtn(pwndPickOut_SLVL); // 上鎖 取出柔性 數值設定
		}
		Update_SLVL();
	}
	
	if(pwnd ==pwndCheck_Ejector) // 勾勾 檢測 頂退完成(開始頂出)
	{
		b_Check_Ejector = !b_Check_Ejector;
		u_Check_Ejector = b_Check_Ejector; // 設定 b_Check_Ejector
		SetDBValue(Check_Ejector_DB,u_Check_Ejector);
		printf("u_Check_Ejector=%d\n",u_Check_Ejector);

		pwndCheck_Ejector->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Ejector]);
		pwndCheck_Ejector->CreateA();
		pwndCheck_Ejector->Update();
	}
		

	if( (pwnd == pwndCheck_Pile) || (pwnd == pwndBtn_PileUse) ) // 堆疊勾勾 || 堆疊選擇 Btn按下彈起
	{
		if(PileNum==0)
			PileNum=1;
		else
			PileNum=0;
		SetDBValue(Check_Pile_DB, PileNum); // 寫入 DB
		UpdateCheck_Pile();
	}
	for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ )
	{
		if( (pwnd == pwndBtn_PileNum[i]) || (pwnd ==pwndBtn_PileChoose[i])) // 堆疊組數 || 堆疊組數選擇鈕 被選擇
		{
			PileNum=i;
			SetDBValue(Check_Pile_DB, PileNum); // 寫入 DB
			UpdateCheck_Pile();
		}
	}
	

  return wIDControl;	
}
/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!b_RunOnlyOne)
		{
			UpdateSubArm();
			UpdateCheck_Pile();
			UpdateClamp();
			UpdateBtnDetect_Select();
			Update_ClampStr();
			Update_SLVL();
			for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
			{
				Update_PosHint(i);
			}
			
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43",0x0000);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44",0x0000);// 監測移動軸 清0
			
			WhichAxis_Old = WhichAxis;
			
			((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標
			
			b_RunOnlyOne = TRUE;// 完成 執行一次
		}
		
	WhichAxis = GetDBValue(pWhichAxisDB).lValue; // 偵測哪一軸被按下
	
	if(WhichAxis<6 && WhichAxis!=WhichAxis_Old && Manual_Type != MANUAL_STOP_MODE)
	{
		b_PosSet_OK[WhichAxis]=0; // 位置需重新確認
		UpdateTeach_Pos(); // 更新 位置設定
		Update_AxisMoveNow(WhichAxis); // 更新 現在移動軸 圖片
		//MsgBoxCall("MsgBox_QTeachAxisSet.txt"); // 顯示 移動軸視窗
		WhichAxis_Old =WhichAxis;
	}	
	else if(WhichAxis>=6 && WhichAxis!=WhichAxis_Old) // 按鈕放開
	{
		Update_AxisMoveNow(0); // 更新 現在移動軸 圖片 顯示無
		WhichAxis_Old =WhichAxis;
	}
	
	
	for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
	{
		AxisPosNow[i] = GetDBValue(AxisPosNow_DBString[i]).lValue; // 取得數值
		if(AxisPosNow_Old[i]!=AxisPosNow[i])
		{
			Update_PosHint(i);
			AxisPosNow_Old[i] = AxisPosNow[i];
		}
	}
	
}
/*---------------------------------------------------------------------------+
|  Function : OnMessage()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
	{
		case	MSG_GPIO_READ:
		{
			printf("MSG_GPIO_READ\n");
			// 背面安全按鈕
			{
			char gpio_safekey;
			gpio_safekey = wParam&GPIO_SAFEKEY_BIT;
			printf("gpio_safekey = %x\n",gpio_safekey);
			if(gpio_safekey==GPIO_SAFEKEY_0) // 放開 停止
				{
					//printf("ACT=STOP\n");
					Manual_Type = MANUAL_STOP_MODE;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_1) // 1段 點動
				{
					//printf("ACT=JOG\n");
					Manual_Type = MANUAL_JOG_MODE;
					WhichAxis_Old=WhichAxis;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_2) // 2段 連續動
				{
					//printf("ACT=CONTINUE\n");
					Manual_Type = MANUAL_CONTINUE_MODE;
					WhichAxis_Old=WhichAxis;
				}
			}
		}
		break;
		case	MSG_PANEL_KEY: //hotkey
		{
			
		}
		break;
		default:
			break;	
	}	
}
/*---------------------------------------------------------------------------+
|  Function : OnDestroyA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    OnDestroyA(CtmWnd* pwndSender)
{ 
	printf("OnDestroyA %d\n",u_Group);
	printf("b_SaveFlag= %d\n",b_SaveFlag);
	int StartNum=0;
	for(int i=0;i<u_Group;i++)
	{
		StartNum=StartNum+StepNum[i];
	}
	StartNo_R=(1+StartNum); // 從 Action_P[StartNo_R] 開始讀
	StartNo_W=(1+StartNum); // 從 MACHINE_PROFILE_NUM[StartNo_W ]_EQUIPMENT1_ACTION_, 開始寫
	switch(u_Group)
	{
		case 1:
			
			break;
		case WaitP: // 離開P2
			if(b_SaveFlag)
				Save();
			break;
		case ReadyDownP:
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP); // 增加前一步有增加的步驟數
			Action_Step = StartNo_W-1;
			if(b_SaveFlag)
				Save();
			break;
		case PickDownP:
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move); // +從頭到尾增加的 跟 前一步增加的 
			Action_Step = StartNo_W-1;
			if(b_SaveFlag)
				Save();
			break;
		case PickP: // 離開P6
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP); // +從頭到尾增加的 跟 前一步增加的 
			Action_Step = StartNo_W-1 - b_SubArmSync;
			if(b_SaveFlag)
				Save();
			break;
		case MoveOutP: // 離開P7
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP + u_SubArmUse_PickP
			+ u_SelectClamp + b_Check_Ejector + b_SubArmUse/*副臂夾具閥*/);  // +從頭到尾增加的 跟 前一步增加的 
			Action_Step = StartNo_W-1 - b_SubArmSync - b_SubArmSync;
			if(b_SaveFlag)
				Save();
			break;
		case PlaceDownP: // 離開P8
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP + u_SubArmUse_PickP + u_SubArmUse_MoveOutP + u_SubArmUse_SubPlaceP
			+ u_SelectClamp + b_Check_Ejector + u_SelectDetect + b_SubArmUse/*副臂夾具閥*/); // +從頭到尾增加的 跟 前一步增加的 
			Action_Step = StartNo_W-1 - b_SubArmSync - b_SubArmSync - b_SubArmSync*2;
			if(b_SaveFlag)
				Save();
			break;
		case PlaceP: // 離開P9
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP + u_SubArmUse_PickP + u_SubArmUse_MoveOutP + u_SubArmUse_SubPlaceP
			+ u_PileNum + u_SelectClamp + b_Check_Ejector + u_SelectDetect + b_SubArmUse/*副臂夾具閥*/); // +從頭到尾增加的 跟 前一步增加的 
			Action_Step = StartNo_W-1 - b_SubArmSync - b_SubArmSync - b_SubArmSync*2;
			if(b_SaveFlag)
				{
					Save();
					Save2PG();
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", 1); // 設定跳到程式第一步
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", 6);
					g_Hint_Download = 1; // 提醒下載
				}
			break;
		default:
			break;
	} 
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : SaveMode()                      	     	           				 		 |
|       		: 紀錄治具選擇																									 |
+---------------------------------------------------------------------------*/
void	SaveMode()
{
	ClampSelect=0;// 治具選用
	for(int i=0;i<10;i++)
	{
		ClampSelect=ClampSelect+(b_Select[i]<<i);
	}
	printf("Set ClampSelect=%d\n",ClampSelect);
	SetDBValue(ClampSelect_DB,ClampSelect);
	if(ClampSelect==0)
		SetDBValue(ClampDetectSelect_DB,0);
	
	ClampDetectSelect=0; // 治具檢測
	for(int i=0;i<10;i++)
	{
		if(b_Select[i]==1)
			ClampDetectSelect=ClampDetectSelect+(b_DetectSelect[i]<<i);
	}
	printf("Set ClampDetectSelect=%d\n",ClampDetectSelect);
	SetDBValue(ClampDetectSelect_DB,ClampDetectSelect);
}
/*---------------------------------------------------------------------------+
|  Function : Save()                      	     	           				         |
|       		: 將快速教導編譯的內容存入暫存g_QTeach_Action_P[]								 |
+---------------------------------------------------------------------------*/
void	Save()
{
	printf("====================Save====================\n");
	printf("StartNo_R=%d\n",StartNo_R); //
	printf("StartNo_W=%d\n",StartNo_W);
	printf("StepNum[%d]=%d\n",u_Group,StepNum[u_Group]);
	
	int QTeach_PGNo = StartNo_W-1; // 快速教導 的步驟號碼
	int Action_PNo  = StartNo_R-1; // 快速教導 動作列表號碼	
	int Read_No = Action_PNo;

	printf("Action_Step=%d\n",Action_Step);
	
	switch(u_Group)
	{
		case Origin:
			
			break;
		/*=====================================P2等待點=====================================*/
		case WaitP: // 等待點			
			for(int i =0;i<StepNum[u_Group];i++) // 從Action_P[]讀取這點需要的步驟
			{
				if(u_PickerType==MechType5) // 使用5軸
				{
					if(i==1) // 加在第2步 設定動作 軸動作 新增動作 
					{
						printf("Set AxisY2\n");
						QTeach_PGNo++;
						Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // 寫入 軸動作 Axis_Y2
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step-b_SubArmSync;// 設定Step數 若有同步(b_SubArmSync),Step數和上一步相同
						printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,Action_Step-b_SubArmSync);
						
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // 寫入 WaitP db
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // 寫入 WaitP db
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0為不用
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_Y2]*10; // 寫入參數數值 至列表Action_P[]
							SetDBValue(P2_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // 寫入 WaitP db
						}	
					}
					if(i==3) // 加在第4步 設定動作 軸動作 新增動作
					{
						printf("Set AxisX2\n");
						QTeach_PGNo++;
						Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						
						if(b_SubArmSync)
						{
							g_QTeach_Action_P[QTeach_PGNo-2].Step = 3;
							g_QTeach_Action_P[QTeach_PGNo-1].Step = 3;// 設定Step數 若有同步(b_SubArmSync),Step數和上一步相同
							printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,3);
						}
						else
						{
							g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數 若有同步(b_SubArmSync),Step數和上一步相同
							printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,Action_Step);
						}
						
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // 寫入 WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // 寫入 WaitP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0為不用
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X2]*10; // 寫入參數數值 至列表Action_P[]
							SetDBValue(P2_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // 寫入 WaitP db
						}	
					}
				}
				QTeach_PGNo++;
				Action_Step++;
				// 依照使用者設定寫入參數數值 至列表Action_P[]	
				Action_P[Action_PNo].Step = Action_Step;// 設定步驟數
					
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // 設定位置 P1為位置
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P2_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 WaitP db
						if(b_Check_Move) // 移動後下降勾選
						{
							if(WhichAxis!=Axis_X1) // 不儲存X1軸座標
								{
									SetDBValue(P4_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 ReadyDownP db
								}
						}
						else
						{
							SetDBValue(P4_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 ReadyDownP db
						}
						
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // 設定速度 P2為速度
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P2_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 WaitP db
						if(b_Check_Move) // 移動後下降勾選
						{
							if(WhichAxis!=Axis_X1) // 不儲存X1軸速度
								SetDBValue(P4_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 ReadyDownP db
						}
						else
						{
							SetDBValue(P4_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 ReadyDownP db
						}
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P2_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // 寫入 db
					}
				}
				if(Action_P[Action_PNo].Type == Action_Valve 
					&& Action_P[Action_PNo].Num == Valve_AxisC_V) // 閥門 姿態 Num為姿態 水平/垂直
				{
					Action_P[Action_PNo].Num = GetDBValue(PosStatus_DBString[WaitP]).lValue; // 寫入參數數值 至列表Action_P[]
					printf("P2 Get %s=%d\n",PosStatus_DBString[WaitP],GetDBValue(PosStatus_DBString[WaitP]).lValue);
				}
				
				// 從Action_P[]讀取這點需要的步驟
				printf("SaveAction2Temp %d\n",Action_PNo);
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				
				if(i==1 && b_SubArmSync && u_PickerType==MechType5)
					{
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step-b_SubArmSync;// 設定Step數 若有同步(b_SubArmSync),Step數和上一步相同
						printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,2);
					}
				
				
				Action_PNo++;
			}
			break;
		/*====================================P3準備下降====================================*/
		case ReadyDownP:
			// 將ReadyDownP 位置 速度 等設定 寫入 PickDownP	
			for(int i=0; i<6; i++)
			{
				if( (i!=Axis_Y1) && (i!=Axis_Y2) ) // Y1,Y2軸 不被設定
				{
					int value=0;
					value= GetDBValue(P4_POSSET_DBString[i]).lValue; // 取得 ReadyDownP DB值
					SetDBValue(P5_POSSET_DBString[i], value); // 寫入 PickDownP db
					value= GetDBValue(P4_Speed_DBString[i]).lValue; // 取得 ReadyDownP DB值
					SetDBValue(P5_Speed_DBString[i], value); // 寫入 PickDownP db
				}
			}			
			if(b_Check_Move) // 移動後下降 勾選
			{
				for(int j=0;j<u_Check_Move;j++) // 新增動作
				{
					printf("Set Axis%d\n",j);
					QTeach_PGNo++;
					Action_Step++;
					if(j==0)
					{
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X1); // 寫入 軸動作 Axis_X1
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
						if(pwndQTeach_Pos[Axis_X1]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_X1]->GetPropValueT("value", &l_Position[Axis_X1],sizeof(l_Position[Axis_X1]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X1]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_POSSET_DBString[Axis_X1], l_Position[Axis_X1]); // 寫入 ReadyDownP db
							SetDBValue(P5_POSSET_DBString[Axis_X1], l_Position[Axis_X1]); // 寫入 PickDownP db
						}
						if(pwndQTeach_Speed[Axis_X1]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_X1]->GetPropValueT("value", &l_Speed[Axis_X1],sizeof(l_Speed[Axis_X1]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X1]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_Speed_DBString[Axis_X1], l_Speed[Axis_X1]); // 寫入 ReadyDownP db
							SetDBValue(P5_Speed_DBString[Axis_X1], l_Speed[Axis_X1]); // 寫入 PickDownP db
						}
						if(pwndQTeach_DT[Axis_X1]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_X1]->GetPropValueT("value", &l_Delaytime[Axis_X1],sizeof(l_Delaytime[Axis_X1]));
							if(l_Delaytime[Axis_X1]!=0) // 0為不用
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X1]*10; // 寫入參數數值 至列表Action_P[]
							SetDBValue(P4_DT_DBString[Axis_X1], l_Delaytime[Axis_X1]); // 寫入 ReadyDownP db
						}
					}
					if(j==1)
					{
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // 寫入 ReadyDownP db
							SetDBValue(P5_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // 寫入 PickDownP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // 寫入 ReadyDownP db
							SetDBValue(P5_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // 寫入 PickDownP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0為不用
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X2]*10; // 寫入參數數值 至列表Action_P[]
							SetDBValue(P4_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // 寫入 ReadyDownP db
						}
					}
				}
				

			}
			break;
		/*=====================================P5下降點=====================================*/
		case PickDownP: // 下降點
			for(int i =0;i<StepNum[u_Group];i++) // 從Action_P[]讀取這點需要的步驟
			{
				QTeach_PGNo++;
				Action_Step++;
				// 依照使用者設定寫入參數數值 至列表Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// 設定Step數
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // 設定位置 P1為位置
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P5_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 PickDownP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // 設定速度 P2為速度
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P5_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 PickDownP db
					}
					if( (b_Check_SLVL) && (Action_P[Action_PNo].Num==Axis_Y1)) // 柔性使用 && Y1軸  設定提前 P3為提前 
					{
						long l_dis =0;
						l_dis = GetDBValue(Pick_SLVL_DB).lValue;
						Action_P[Action_PNo].P3 = l_dis; // 寫入參數數值 Action_P[Action_PNo]
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P5_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // 寫入 PickDownP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				Action_PNo++;
			}
			if(b_SubArmUse) // 副臂使用
			{
				printf("Set AxisY2\n");
				QTeach_PGNo++;
				if(!b_SubArmSync) // 若有同步 序號不++
					Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // 寫入 軸動作 Axis_Y2 新增動作
				
				// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				if(pwndQTeach_Pos[Axis_Y2]!=NULL) // 設定位置 P1為位置
				{
					pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
					g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
					SetDBValue(P5_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // 寫入 WaitP db
				}
				if(pwndQTeach_Speed[Axis_Y2]!=NULL) // 設定速度 P2為速度
				{
					pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
					g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
					SetDBValue(P5_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // 寫入 WaitP db
				}
				if(b_Check_SLVL) // 柔性使用 設定提前 P3為提前 
				{
					long l_dis =0;
					l_dis = GetDBValue(Pick_SLVL_DB).lValue;
					g_QTeach_Action_P[QTeach_PGNo-1].P3 = l_dis; // 寫入參數數值 g_QTeach_Action_P[QTeach_PGNo-1]
					printf("Setg_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,l_dis);
				}
				if(pwndQTeach_DT[Axis_Y2]!=NULL) // 設定延時 P5為延時
				{
					pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
					if(l_Delaytime[Axis_Y2]!=0) // 0為不用
						g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // 寫入參數數值 g_QTeach_Action_P[QTeach_PGNo-1]
					SetDBValue(P5_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // 寫入 WaitP db
				}	
			}
			SaveSLVLData();
			break;
		/*=====================================P6取件點=====================================*/
		case PickP: // 取件點
			
			for(int i =0;i<StepNum[u_Group];i++) // 從Action_P[]讀取這點需要的步驟
			{
				if(i==0 && u_SelectClamp>0) // 設定動作 閥門  新增動作 吸
				{
					for(int j=0;j<u_SelectClamp;j++) // 所選 治具數量
					{
						int choose=0;
						int on_off=0;
						int detect=0;
						int DelayTime=0;
						for(int k=0;k<10;k++) // 設定 (治具j+1) 
						{
							if(b_Select[k]==1) // 第k個治具 被選用 k對照 u_pszClamp_SelectString[]
							{
								choose++;
								if(choose == j+1 && k>4)
								{
									printf("Set Valve%d\n",j);
									QTeach_PGNo++;
									Action_Step++;
									SaveAct_Valve2Temp(QTeach_PGNo,j);
									// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]		
									g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
									g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // 類型k k對照u_pszClamp_SelectString[]
									
									on_off = GetDBValue(P6_Clamp_onoff_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P1 = on_off; // 開關 P1
									detect = GetDBValue(P6_Clamp_detect_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // 檢測 P2
									if(pwndQTeach_Clamp_DT[k]!=NULL) // 設定延時 P5為延時 
									{
										pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
										if(l_Clamp_Delaytime[k]!=0) // 0為不用
											g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // 寫入參數數值 g_QTeach_Action_P[]
										SetDBValue(P6_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // 寫入 PickP db
									}
								}
							}
						}		
					}
				}	
				
				
				if(b_SubArmUse) // 副臂使用
				{
					if(i==1) // 加在第2步 設定動作 軸動作 新增動作 
					{
						printf("Set AxisX2\n");
						QTeach_PGNo++;
						if(!b_SubArmSync) // 若有同步 序號不++
							Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P6_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // 寫入 WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P6_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // 寫入 WaitP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0為不用
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X2]*10; // 寫入參數數值 至列表Action_P[]
							SetDBValue(P6_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // 寫入 WaitP db
						}	
					}
				}
				QTeach_PGNo++;
				Action_Step++;
				
				// 依照使用者設定寫入參數數值 至列表Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// 設定Step數
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // 設定位置 P1為位置
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P6_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 PickP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // 設定速度 P2為速度
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P6_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 PickP db
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P6_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // 寫入 PickP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				
				if(i==1 && u_SelectClamp>0) // 設定動作 閥門  新增動作 夾
				{
					for(int j=0;j<u_SelectClamp;j++) // 所選 治具數量
					{
						int choose=0;
						int on_off=0;
						int detect=0;
						int DelayTime=0;
						for(int k=0;k<10;k++) // 設定 (治具j+1) 
						{
							if(b_Select[k]==1) // 第k個治具 被選用 k對照 u_pszClamp_SelectString[]
							{
								choose++;
								if(choose == j+1 && k<5)
								{
									printf("Set Valve%d\n",j);
									QTeach_PGNo++;
									Action_Step++;
									SaveAct_Valve2Temp(QTeach_PGNo,j);
									// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]		
									g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
												
									g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // 類型k k對照u_pszClamp_SelectString[]
									
									on_off = GetDBValue(P6_Clamp_onoff_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P1 = on_off; // 開關 P1
									detect = GetDBValue(P6_Clamp_detect_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // 檢測 P2
									if(pwndQTeach_Clamp_DT[k]!=NULL) // 設定延時 P5為延時 
									{
										pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
										if(l_Clamp_Delaytime[k]!=0) // 0為不用
											g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // 寫入參數數值 g_QTeach_Action_P[]
										SetDBValue(P6_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // 寫入 PickP db
									}
								}
							}
						}		
					}
				}
				if(i==1 && b_SubArmUse) // 設定動作 閥門  新增動作 副臂閥門
				{
					QTeach_PGNo++;
					Action_Step++;
					SaveAct_Valve2Temp(QTeach_PGNo,0);
		
					g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
					g_QTeach_Action_P[QTeach_PGNo-1].Num = 14; // 
								
					//on_off = GetDBValue(P6_Clamp_onoff_DBString[k]).lValue;
					g_QTeach_Action_P[QTeach_PGNo-1].P1 = ON; // 開關 P1
					//detect = GetDBValue(P6_Clamp_detect_DBString[k]).lValue;
					//g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // 檢測 P2
//								if(pwndQTeach_Clamp_DT[k]!=NULL) // 設定延時 P5為延時 
//								{
//									pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
//									if(l_Clamp_Delaytime[k]!=0) // 0為不用
//										g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // 寫入參數數值 g_QTeach_Action_P[]
//									SetDBValue(P6_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // 寫入 PickP db
//								}
				}		
				
				if(i==2 && b_Check_Ejector) // 設定動作 等待 
				{
					printf("Check Ejector!!!\n");
					QTeach_PGNo++;
					Action_Step++;
					SaveAct_Wait2Temp(QTeach_PGNo,Wait_PullBack);
					
					// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
					g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
					g_QTeach_Action_P[QTeach_PGNo-1].P1 = OFF; // 開關 P1
				}
				Action_PNo++;
			}
			break;
		/*=====================================P7橫出點=====================================*/
		case MoveOutP: // 橫出點
			for(int i =0;i<StepNum[u_Group];i++) // 從Action_P[]讀取這點需要的步驟
			{
				if(b_SubArmUse) // 副臂使用
				{
					if(i==1) // 加在第1步 設定動作 軸動作 新增動作 
					{
						QTeach_PGNo++;
						if(!b_SubArmSync) // 若有同步 序號不++
							Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // 寫入 WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // 寫入 WaitP db
						}
						if(b_Check_SLVL) // 柔性使用  設定提前 P3為提前 
						{
							long l_dis =0;
							l_dis = GetDBValue(PickOut_SLVL_DB).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P3 = l_dis; // 寫入參數數值 g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Setg_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,l_dis);
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0為不用
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // 寫入 WaitP db
						}	
					}
					if(i==2) // 加在第2步 設定動作 軸動作 新增動作
					{
						QTeach_PGNo++;
						if(!b_SubArmSync) // 若有同步 序號不++
							Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // 寫入 軸動作 Axis_Y2
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // 設定位置 P1為位置
						{
							pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // 寫入 WaitP db
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // 設定速度 P2為速度
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // 寫入 WaitP db
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // 設定延時 P5為延時
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0為不用
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // 寫入 WaitP db
						}	
					}
				}
				if( i==2 && u_SelectDetect>0) // 設定動作 檢測  新增動作
				{
					for(int j=0;j<u_SelectDetect;j++) // 所選 治具數量
					{
						printf("Set Detect%d\n",j);
						QTeach_PGNo++;
						Action_Step++;
						SaveAct_Detect2Temp(QTeach_PGNo,j);
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]	
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數	
						int choose=0;
						int on_off=0;
						for(int k=0;k<10;k++) // 設定 (治具j+1) 
						{
							if(b_DetectSelect[k]==1)
							{
								choose++;
								if(choose == j+1)
								{
									g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // 類型k k對照u_pszClamp_SelectString[]
								}
							}
						}	
					}
				}
				QTeach_PGNo++;
				Action_Step++;
				
				// 依照使用者設定寫入參數數值 至列表Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// 設定Step數
				printf("\n\nAction_P[Action_PNo].Type=%d Action_P[Action_PNo].Num=%d\n\n",Action_P[Action_PNo].Type,Action_P[Action_PNo].Num);
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // 設定位置 P1為位置
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P7_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 MoveOutP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // 設定速度 P2為速度
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P7_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 MoveOutP db
					}
					if( (b_Check_SLVL) && (Action_P[Action_PNo].Num==Axis_X1)) // 設定提前 P3為提前 X1軸
					{
						long l_dis =0;
						l_dis = GetDBValue(PickOut_SLVL_DB).lValue;
						Action_P[Action_PNo].P3 = l_dis; // 寫入參數數值 Action_P[Action_PNo]
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P7_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // 寫入 MoveOutP db
					}
				}
				if(Action_P[Action_PNo].Type == Action_Valve 
					&& Action_P[Action_PNo].Num == Valve_AxisC_H) // 閥門 姿態 Num為姿態 水平/垂直
				{
					Action_P[Action_PNo].Num = GetDBValue(PosStatus_DBString[MoveOutP]).lValue; // 寫入參數數值 至列表Action_P[]
					printf("\n\n!!!!!!!!!!!!!MoveOutP =%d !!!!!!!!!!!!!!\n\n",Action_P[Action_PNo].Num);
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				Action_PNo++;
			}
			
			if(b_SubArmUse) // 副臂使用 (丟水口)
			{
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Z); // 寫入 軸動作 Axis_Z (橫出)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2 (前進)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // 寫入 軸動作 Axis_Y2 (下降)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Valve2Temp(QTeach_PGNo,0); // 寫入 閥門 副臂閥門 OFF
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // 寫入 軸動作 Axis_Y2 (上拉) (回等待點)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2 (後退) (回等待點)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				
				SetDBValue(SubStartNo_DB, QTeach_PGNo-5); // 設定 水口資料寫入 開始序號
			}
			SaveSLVLData();
			break;
		/*=====================================P8下降點=====================================*/
		case PlaceDownP: // 下降點
			for(int i =0;i<StepNum[u_Group];i++) // 從Action_P[]讀取這點需要的步驟
			{
				if(i==0 && PileNum>0) // 有堆疊需求  新增動作
				{
					// 新增動作
					QTeach_PGNo++;
					Action_Step++;
					SaveAct_Pile2Temp(QTeach_PGNo,PileNum); // 寫入 堆疊(動作)
					
					// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
					g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
					g_QTeach_Action_P[QTeach_PGNo-1].Num = PileNum; // 堆疊 組
					g_QTeach_Action_P[QTeach_PGNo-1].P1	= ON; // 堆疊 開始
				}
				
				QTeach_PGNo++;
				Action_Step++;
				
				// 依照使用者設定寫入參數數值 至列表Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// 設定Step數
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // 設定位置 P1為位置
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P8_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 PlaceDownP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // 設定速度 P2為速度
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P8_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 PlaceDownP db
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P8_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // 寫入 PlaceDownP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				Action_PNo++;
			}
			break;
		/*=====================================P9放置點=====================================*/
		case PlaceP: // 放置點
			for(int i =0;i<StepNum[u_Group];i++) // 從Action_P[]讀取這點需要的步驟
			{				
				QTeach_PGNo++;
				Action_Step++;
				
				// 依照使用者設定寫入參數數值 至列表Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// 設定Step數
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // 設定位置 P1為位置
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P9_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // 寫入 PlaceP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // 設定速度 P2為速度
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P9_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // 寫入 PlaceP db
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
						SetDBValue(P9_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // 寫入 PlaceP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				Action_PNo++;
			}	
			if(PileNum>0) // 有堆疊需求  新增動作
			{
				// 新增動作
				QTeach_PGNo++;
				Action_Step++;
				SaveAct_Pile2Temp(QTeach_PGNo,PileNum); // 寫入 堆疊(動作)
				
				// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
				g_QTeach_Action_P[QTeach_PGNo-1].Num = PileNum; // 堆疊 組
				g_QTeach_Action_P[QTeach_PGNo-1].P1	= OFF; // 堆疊 結束
			}
			
			if(u_SelectClamp>0) // 設定動作 閥門  新增動作 夾
			{
				for(int j=0;j<u_SelectClamp;j++) // 所選 治具數量
				{
					int choose=0;
					int on_off=0;
					int detect=0;
					int DelayTime=0;
					for(int k=0;k<10;k++) // 設定 (治具j+1) 
					{
						if(b_Select[k]==1) // 第k個治具 被選用 k對照 u_pszClamp_SelectString[]
						{
							choose++;
							if(choose == j+1)
							{
								printf("Set Valve%d\n",j);
								QTeach_PGNo++;
								Action_Step++;
								SaveAct_Valve2Temp(QTeach_PGNo,j);
								// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]		
								g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
											
								g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // 類型k k對照u_pszClamp_SelectString[]
								
								on_off = GetDBValue(P9_Clamp_onoff_DBString[k]).lValue;
								g_QTeach_Action_P[QTeach_PGNo-1].P1 = on_off; // 開關 P1
								detect = GetDBValue(P9_Clamp_detect_DBString[k]).lValue;
								g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // 檢測 P2
								if(pwndQTeach_Clamp_DT[k]!=NULL) // 設定延時 P5為延時 
								{
									pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
									if(l_Clamp_Delaytime[k]!=0) // 0為不用
										g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // 寫入參數數值 g_QTeach_Action_P[]
									SetDBValue(P9_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // 寫入 PickP db
								}
							}
						}
					}		
				}
			}
		/*=====================================P10回等待點=====================================*/
			// 回原點(等待點)
			for(int i =0;i<StepNum[Homing];i++) // 從Action_P[]讀取這點需要的步驟
			{
				if(u_PickerType==MechType5) // 使用5軸
				{			
					if(i==2) // 加在第3步 設定動作 軸動作 新增動作
					{
						printf("Set AxisX\n");
						QTeach_PGNo++;
						//Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // 寫入 軸動作 Axis_X2
						
						// 依照使用者設定寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// 設定Step數
						
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 設定位置 P1為位置
						{
							l_Position[Axis_X2] = GetDBValue(P2_POSSET_DBString[Axis_X2]).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 寫入參數數值 至列表g_QTeach_Action_P[QTeach_PGNo-1]
						}
						// 設定速度 P2為速度
						{
							l_Speed[Axis_X2] = GetDBValue(P2_Speed_DBString[Axis_X2]).lValue; // 取得等待點速度
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 寫入參數數值 至列表Action_P[]
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 設定延時 P5為延時
						{
							l_Delaytime[Axis_X2] = GetDBValue(P2_DT_DBString[Axis_X2]).lValue; // 取得等待點延時
							if(l_Delaytime[Axis_X2]!=0) // 0為不用
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // 寫入參數數值 至列表Action_P[]
						}
					}
				}
				
				QTeach_PGNo++;
				Action_Step++;
				
				// 依照使用者設定寫入參數數值 至列表Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// 設定Step數
				if(Action_P[Action_PNo].Type == Action_Axis) //軸動作 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // 哪一軸
					// 設定位置 P1為位置
					{
						l_Position[WhichAxis] = GetDBValue(P2_POSSET_DBString[WhichAxis]).lValue; // 取得等待點座標
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // 寫入參數數值 至列表Action_P[]
					}
					// 設定速度 P2為速度
					{
						l_Speed[WhichAxis] = GetDBValue(P2_Speed_DBString[WhichAxis]).lValue; // 取得等待點速度
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // 寫入參數數值 至列表Action_P[]
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // 設定延時 P5為延時
					{
						l_Delaytime[WhichAxis] = GetDBValue(P2_DT_DBString[WhichAxis]).lValue; // 取得等待點延時
						if(l_Delaytime[WhichAxis]!=0) // 0為不用
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // 寫入參數數值 至列表Action_P[]
					}
				}
				
				if(Action_P[Action_PNo].Type == Action_Valve 
					&& Action_P[Action_PNo].Num == Valve_AxisC_V) // 閥門 姿態 Num為姿態 水平/垂直
				{
					Action_P[Action_PNo].Num = GetDBValue(PosStatus_DBString[WaitP]).lValue; // 寫入參數數值 至列表Action_P[]
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // 將快速教導動作存入暫存位置
				Action_PNo++;
			}	
			break;
		default:
			break;
	} 
	
	b_SaveFlag=FALSE;



}
/*---------------------------------------------------------------------------+
|  Function : Save2PG()                      	     	           				       |
|       		: 把暫存的內容 g_QTeach_Action_P[i]															 |
|							寫入教導程式 MACHINE_PROFILE_NUM%d_ACTION_STEP	, i+1					 |
+---------------------------------------------------------------------------*/
void	Save2PG()
{
	printf("===========================Save2PG===========================\n");
	CleanPG();
	char 	pDataID[256];
	int		value = 0;
	
	for(int i=0;i<MaxDBNum;i++) // 
	{
	/*======================================= 步驟序號 STEP =======================================*/
		value = g_QTeach_Action_P[i].Step;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
		SetDBValue(pDataID, value);
		printf("Set %s=%d\n",pDataID,value);
	/*======================================= 動作類型 TYPE =======================================*/
		value = g_QTeach_Action_P[i].Type;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		SetDBValue(pDataID, value);
	/*======================================= 動作形式 NUM =======================================*/
		value = g_QTeach_Action_P[i].Num;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		SetDBValue(pDataID, value);
	/*======================================= 參數1 =======================================*/
		value = g_QTeach_Action_P[i].P1;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1",i+1);
		SetDBValue(pDataID, value);
	/*======================================= 參數2 =======================================*/
		value = g_QTeach_Action_P[i].P2;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2",i+1);
		SetDBValue(pDataID, value);
	/*======================================= 參數3 =======================================*/
		value = g_QTeach_Action_P[i].P3;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3",i+1);
		SetDBValue(pDataID, value);
	/*======================================= 參數4 =======================================*/
		value = g_QTeach_Action_P[i].P4;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4",i+1);
		SetDBValue(pDataID, value);
	/*======================================= 參數5 =======================================*/
		value = g_QTeach_Action_P[i].P5;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5",i+1);
		SetDBValue(pDataID, value);
	}
}
/*---------------------------------------------------------------------------+
|  Function : CleanPG()                      	     	           				 			 |
|       		: 清除現有教導程式																							 |
+---------------------------------------------------------------------------*/
void	CleanPG()
{
	char 	pDataID[256];	
	for(int i=0;i<MaxDBNum;i++) //
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
		SetDBValue(pDataID, 0);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		SetDBValue(pDataID, 0);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		SetDBValue(pDataID, 0);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i+1,j+1);
			SetDBValue(pDataID, 0);
		}
	}
}
/*---------------------------------------------------------------------------+
|  Function : Clean_QTeach()                  						   				         |
|       		: 清除快速教導編譯的內容																				 |
+---------------------------------------------------------------------------*/
void	Clean_QTeach()
{
	printf("Clean_QTeach\n");
	char 	pDataID[256];	
	for(int i=0;i<MaxDBNum;i++) //
	{
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_STEP",i+1);
//		SetDBValue(pDataID, 0);
		g_QTeach_Action_P[i].Step = 0;
		
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_TYPE",i+1);
//		SetDBValue(pDataID, 0);
		g_QTeach_Action_P[i].Type = 0;
		
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_NUM",i+1);
//		SetDBValue(pDataID, 0);
		g_QTeach_Action_P[i].Num = 0;
		
		g_QTeach_Action_P[i].P1 = 0;
		g_QTeach_Action_P[i].P2 = 0;
		g_QTeach_Action_P[i].P3 = 0;
		g_QTeach_Action_P[i].P4 = 0;
		g_QTeach_Action_P[i].P5 = 0;
//		for(int j=0;j<PARA_NUM;j++)
//		{
//			memset(pDataID, 0 ,sizeof(pDataID));
//			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_PARAMETER%d",i+1,j+1);
//			SetDBValue(pDataID, 0);
//		}
	}
	
}
/*---------------------------------------------------------------------------+
|  Function : GetPosNow(int GetAxisPos)                  				   		       |
|       		: 獲取當前 GetAxisPos 軸位置																		 |
+---------------------------------------------------------------------------*/
void	GetPosNow(int GetAxisPos)
{
	printf("Get Pos %d\n",GetAxisPos);

	if((u_Group==ReadyDownP) && (GetAxisPos==Axis_Y1) // P4準備下降點 除X1,X2軸外不可取得
		|| ((u_Group==ReadyDownP) && (GetAxisPos==Axis_Z))
		|| ((u_Group==ReadyDownP) && (GetAxisPos==Axis_Y2)) )	
	{
	}
	else if( (u_Group==PickDownP) && (GetAxisPos==Axis_X1) // P5取出側下降點 點除Y1,Y2軸外不可取得
		|| ((u_Group==PickDownP) && (GetAxisPos==Axis_Z))
		|| ((u_Group==PickDownP) && (GetAxisPos==Axis_X2)) )	
	{
	}
	else if( (u_Group==PickP) && (GetAxisPos==Axis_Y1) // P6取件點 除X1,X2軸外不可取得
		|| ((u_Group==PickP) && (GetAxisPos==Axis_Z))
		|| ((u_Group==PickP) && (GetAxisPos==Axis_Y2)) )	
	{
	}
	else if( ((u_Group==MoveOutP) && (GetAxisPos==Axis_Z)) ) // P7橫出點 點除X1,X2,Y1,Y2軸外不可取得
	{
	}
	else if((u_Group==PlaceDownP) && (GetAxisPos==Axis_Y1)
		|| ((u_Group==PlaceDownP) && (GetAxisPos==Axis_X2))
		|| ((u_Group==PlaceDownP) && (GetAxisPos==Axis_Y2)) ) // P8置物側下降點 點除X,Z軸外不可取得
	{
	}
	else if( (u_Group==PlaceP) && (GetAxisPos!=Axis_Y1) ) // P9置物側置物點 點除Y軸外不可取得
	{
	}
	else
	{
		long AxisPosNow = GetDBValue(AxisPosNow_DBString[GetAxisPos]).lValue; // 取得數值
		printf("Get %s = %d\n",u_pszQTeach_PosString[GetAxisPos],AxisPosNow);
		if(pwndQTeach_Pos[GetAxisPos] != NULL)
		{
			long Max_value=0,Min_value=0;
			char	MAX_DBID[256];	memset(MAX_DBID, 0, sizeof(MAX_DBID));
			pwndQTeach_Pos[GetAxisPos]->GetPropValueT("maxid", MAX_DBID, sizeof(MAX_DBID));
			Max_value = GetDBValue(MAX_DBID).lValue;
			printf("Max:%d\n",Max_value);
			char	MIN_DBID[256];	memset(MIN_DBID, 0, sizeof(MIN_DBID));
			pwndQTeach_Pos[GetAxisPos]->GetPropValueT("minid", MIN_DBID, sizeof(MIN_DBID));
			Min_value = GetDBValue(MIN_DBID).lValue;
			printf("Min:%d\n",Min_value);
			if( (AxisPosNow-Max_value)>0 )
			{
				printf("AxisPosNow>MAX_DBID\n");
				MsgBox(g_MultiLanguage["TEACH_QTEACH_POSMAX_ERROR"], tmFT_CODE_TECH); // 超過最大值
			}
			else if((AxisPosNow-Min_value)<0)
			{
				printf("AxisPosNow<Min_value\n");
				MsgBox(g_MultiLanguage["TEACH_QTEACH_POSMIN_ERROR"], tmFT_CODE_TECH); // 小於最小值
			}
			else
			{
				printf("Set Axis%d = %d\n",GetAxisPos,AxisPosNow);
				pwndQTeach_Pos[GetAxisPos]->OnLoseFocus();
				pwndQTeach_Pos[GetAxisPos]->SetPropValueT("value",AxisPosNow); // 寫入數值
				UpdateTeach_Pos();
			}
		}
	}	
}

/*---------------------------------------------------------------------------+
|  Function : UpdateImg()                  						   				   		       |
|       		: 更新顯示圖片																									 |
+---------------------------------------------------------------------------*/
void	UpdateImg()
{
	printf("Update Img\n");
	char 	pDataID[256];
	switch(u_Group) // 取得狀態值
	{
		case WaitP: // 等待點
			if(pwndBtn_PosHV !=NULL) // 姿態選擇存在
			{
				if(GetDBValue("MACHINE_FUNCTION_OPTIONS_MOLD_POSE").lValue == 0)// 若模內姿態 沒定義
				{
					pwndBtn_PosHV->SetPropValueT("enabled",double(TRUE)); // 解鎖姿態選擇
					u_status = GetDBValue(PosStatus_DBString[WaitP]).lValue; // 讀取姿態
				}
				else // 模內姿態 已定義
				{
					u_status = GetDBValue("MACHINE_FUNCTION_OPTIONS_MOLD_POSE").lValue; // 讀取姿態
//					if(u_status==1) // 模內姿態水平
//						u_status=4;
//					else if(u_status==2) // 模內姿態垂直
//						u_status=3;
					SetDBValue(PosStatus_DBString[WaitP],u_status); // 等待點的姿態
					SetDBValue(PosStatus_DBString[ReadyDownP],u_status+2); // 準備下降點的姿態
					SetDBValue(PosStatus_DBString[PickDownP],u_status+2); // 取件側下降點的姿態
					SetDBValue(PosStatus_DBString[PickP],u_status+2); // 取件點的姿態
					pwndBtn_PosHV->SetPropValueT("enabled",double(FALSE)); // 鎖姿態選擇
				}
			}
			if(u_status == 0)// 若姿態無定義 則給定預設值
			{
				u_status=Valve_AxisC_V; // 垂直
				SetDBValue(PosStatus_DBString[WaitP],u_status); // 等待點的姿態
				SetDBValue(PosStatus_DBString[ReadyDownP],u_status+2); // 準備下降點的姿態
				SetDBValue(PosStatus_DBString[PickDownP],u_status+2); // 取件側下降點的姿態
				SetDBValue(PosStatus_DBString[PickP],u_status+2); // 取件點的姿態
			}
			break;
		case ReadyDownP: // 準備下降點
			u_status = GetDBValue(PosStatus_DBString[ReadyDownP]).lValue; // 姿態
			if(pwndCheck_Move!=NULL) // 前進後下降
			{
				pwndCheck_Move->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Move]);
				pwndCheck_Move->CreateA();
				pwndCheck_Move->Update();
				if(b_Check_Move)
				{
					UnLockBtn(pwndQTeach_Pos[Axis_X1]);
					UnLockBtn(pwndQTeach_Speed[Axis_X1]);
					UnLockBtn(pwndQTeach_DT[Axis_X1]);
					UnLockBtn(pwndBtn_GetPos[Axis_X1]);
					if(pwndImg_QTeach_ReadyDownP!=NULL)
					{
						pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_ReadyDownP.bmp");
						pwndImg_QTeach_ReadyDownP->Show();							
					}
				}
				else
				{
					LockBtn(pwndQTeach_Pos[Axis_X1]);
					LockBtn(pwndQTeach_Speed[Axis_X1]);
					LockBtn(pwndQTeach_DT[Axis_X1]);
					LockBtn(pwndBtn_GetPos[Axis_X1]);
					if(pwndImg_QTeach_ReadyDownP!=NULL)
					{
						pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_WaitP.bmp");
						pwndImg_QTeach_ReadyDownP->Show();							
					}
				}
			}
			break;
		case PickDownP: // 取出測下降點
			u_status = GetDBValue(PosStatus_DBString[PickDownP]).lValue; // 姿態
			if(pwndCheck_SLVL!=NULL) // 柔性使用
			{
				pwndCheck_SLVL->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_SLVL]);
				pwndCheck_SLVL->CreateA();
				pwndCheck_SLVL->Update();
				if(b_Check_SLVL)
				{
					UnLockBtn(pwndPick_SLVL);
				}
				else
				{
					LockBtn(pwndPick_SLVL);
				}
			}
			break;
		case PickP: // 取件點
			u_status = GetDBValue(PosStatus_DBString[PickP]).lValue; // 姿態
			for(int i=0;i<10;i++)
			{
				if(pwndQTeach_Clamp_DT[i]!=NULL) // 顯示 治具 延時 數值
					{
						l_Clamp_Delaytime[i] = GetDBValue(P6_Clamp_DT_DBString[i]).lValue; // 讀取DB數值
						pwndQTeach_Clamp_DT[i]->SetPropValueT("value",l_Clamp_Delaytime[i]); // 寫入數值
						pwndQTeach_Clamp_DT[i]->CreateA();
						pwndQTeach_Clamp_DT[i]->Update();
					}
				u_clamp_onoff[i] = GetDBValue(P6_Clamp_onoff_DBString[i]).lValue; // 治具 ON/OFF 狀態
				u_detect_onoff[i] = GetDBValue(P6_Clamp_detect_DBString[i]).lValue; // 治具 檢測 狀態 
			}
			
			if(pwndCheck_Ejector!=NULL) // 顯示 檢測頂退完成(頂出開始)
			{
				pwndCheck_Ejector->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Ejector]);
				pwndCheck_Ejector->CreateA();
				pwndCheck_Ejector->Update();
			}
			
			break;
		case MoveOutP: // 橫出點
			if(pwndBtn_PosHV !=NULL) // 姿態選擇存在
			{
				if(GetDBValue("MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE").lValue == 0)// 若橫出姿態 沒定義
				{
					pwndBtn_PosHV->SetPropValueT("enabled",double(TRUE)); // 解鎖姿態選擇
					u_status = GetDBValue(PosStatus_DBString[MoveOutP]).lValue; // 讀取姿態
				}
				else // 模內姿態 已定義
				{
					u_status = GetDBValue("MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE").lValue; // 讀取姿態
//					if(u_status==1) // 模內姿態水平
//						u_status=4;
//					else if(u_status==2) // 模內姿態垂直
//						u_status=3;
					SetDBValue(PosStatus_DBString[MoveOutP],u_status); // 取出側橫出點的姿態
					SetDBValue(PosStatus_DBString[PlaceDownP],u_status+2); // 置物側下降點的姿態
					pwndBtn_PosHV->SetPropValueT("enabled",double(FALSE)); // 鎖姿態選擇
				}
			}
			if(u_status == 0)// 若姿態無定義 則給定預設值
			{
				u_status=Valve_AxisC_H; // 水平
				SetDBValue(PosStatus_DBString[MoveOutP],u_status); // 橫出點的姿態
			}
			if(pwndCheck_SLVL!=NULL) // 柔性使用
			{
				pwndCheck_SLVL->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_SLVL]);
				pwndCheck_SLVL->CreateA();
				pwndCheck_SLVL->Update();
				if(b_Check_SLVL)
				{
					UnLockBtn(pwndPickOut_SLVL);
				}
				else
				{
					LockBtn(pwndPickOut_SLVL);
				}
			}
			
			break;
		case PlaceDownP: // 下降點
			u_status = GetDBValue(PosStatus_DBString[PlaceDownP]).lValue; // 姿態  
			break;
		case PlaceP: // 放置點
			u_status = GetDBValue(PosStatus_DBString[PlaceP]).lValue; // 姿態
			for(int i=0;i<10;i++)
			{
				if(pwndQTeach_Clamp_DT[i]!=NULL) // 顯示 治具 延時 數值
					{
						l_Clamp_Delaytime[i] = GetDBValue(P9_Clamp_DT_DBString[i]).lValue; // 讀取DB數值
						pwndQTeach_Clamp_DT[i]->SetPropValueT("value",l_Clamp_Delaytime[i]); // 寫入數值
						pwndQTeach_Clamp_DT[i]->CreateA();
						pwndQTeach_Clamp_DT[i]->Update();
					}
				u_clamp_onoff[i] = GetDBValue(P9_Clamp_onoff_DBString[i]).lValue; // 治具 ON/OFF 狀態
				u_detect_onoff[i] = GetDBValue(P9_Clamp_detect_DBString[i]).lValue; // 治具 檢測 狀態
			}
			break;
		default:
			break;
	}
	
	if(pwndBtn_PosHV != NULL) // 顯示姿態狀態
	{
	 printf("u_status=%d\n",u_status);
	 pwndBtn_PosHV->SetPropValueT("upbitmap",PosHV_ImgPath[u_status]);
	 pwndBtn_PosHV->CreateA();
	 pwndBtn_PosHV->Update();
	}
	
	for(int i=0;i<10;i++) // 顯示治具們狀態
	{
		if(pwndBtn_Clamp[i] != NULL) // 治具們狀態
		{
		 printf("%s=%d\n",u_pszBtn_Clamp_String[i],u_clamp_onoff[i]);
		 pwndBtn_Clamp[i]->SetPropValueT("upbitmap",Clamp_ImgPath[ (u_clamp_onoff[i]*10+i)*b_Select[i] ]);
		 pwndBtn_Clamp[i]->CreateA();
		 pwndBtn_Clamp[i]->Update();
		 if(u_SelectClamp>0)
		 	b_ClampSet_OK=0; // 若有治具按鈕 需確認
		}	
		if(pwndDetectBox[i] != NULL) // 顯示治具檢測ON/OFF狀態
		{
		 pwndDetectBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[u_detect_onoff[i]]);
		 pwndDetectBox[i]->CreateA();
		 pwndDetectBox[i]->Update();
		}
		Update_ClampStr(); // 顯示被選擇治具的文字說明
	}
	
	
	if(pwndStr_Place_Z!=NULL) // 顯示 放置/堆疊 文字說明
		{
			if(PileNum>0) //堆疊
				pwndStr_Place_Z->SetPropValueT("textID","TEACH_QTEACH_PILEZ");
			else
				pwndStr_Place_Z->SetPropValueT("textID","TEACH_QTEACH_PLACEZ");
			pwndStr_Place_Z->Update();
		}
	if(pwndStr_Place_X!=NULL) // 顯示 放置/堆疊 文字說明
		{
			if(PileNum>0) //堆疊
				pwndStr_Place_X->SetPropValueT("textID","TEACH_QTEACH_PILEX");
			else
				pwndStr_Place_X->SetPropValueT("textID","TEACH_QTEACH_PLACEX");
			pwndStr_Place_X->Update();
		}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateDataValue()                  						   				   		 |
|       		: 更新設定位置、設定速度數值																		 |
+---------------------------------------------------------------------------*/
void	UpdateDataValue()
{
	//printf("Update data\n");
	long AxisSetPos[6] = {0};
	long AxisSetSpeed[6] = {0};
	long AxisSetDT[6] = {0};
	
	for(int WhichAxis=0; WhichAxis<6; WhichAxis++)
	{
		if(pwndQTeach_Pos[WhichAxis] != NULL) // Set Pos
		{
			// Get data Pos value from db
			char	SetPos_DBID[256];	memset(SetPos_DBID, 0, sizeof(SetPos_DBID));
			pwndQTeach_Pos[WhichAxis]->GetPropValueT("dbid1", SetPos_DBID, sizeof(SetPos_DBID));
			AxisSetPos[WhichAxis] = GetDBValue(SetPos_DBID).lValue;
			//printf("Get %s = %d\n",SetPos_DBID,AxisSetPos[WhichAxis]);
			// Update show data
			pwndQTeach_Pos[WhichAxis]->SetPropValueT("value",AxisSetPos[WhichAxis]);
			pwndQTeach_Pos[WhichAxis]->Update();
			pwndQTeach_Pos[WhichAxis]->UpdateAll();
			//printf("Set Axis%d = %d\n",WhichAxis,AxisSetPos[WhichAxis]);
		}
		
		if(pwndQTeach_Speed[WhichAxis] != NULL) // Set Speed
		{
			// Get data Speed value from db
			char	SetSpeed_DBID[256];	memset(SetSpeed_DBID, 0, sizeof(SetSpeed_DBID));
			pwndQTeach_Speed[WhichAxis]->GetPropValueT("dbid1", SetSpeed_DBID, sizeof(SetSpeed_DBID));
			AxisSetSpeed[WhichAxis] = GetDBValue(SetSpeed_DBID).lValue;
			//printf("Get %s = %d\n",SetSpeed_DBID,AxisSetSpeed[WhichAxis]);
			// Update show data
			pwndQTeach_Speed[WhichAxis]->SetPropValueT("value",AxisSetSpeed[WhichAxis]);
			pwndQTeach_Speed[WhichAxis]->Update();
			pwndQTeach_Speed[WhichAxis]->UpdateAll();
			//printf("Set Axis%d Speed = %d\n",WhichAxis,AxisSetSpeed[WhichAxis]);
		}
		
		if(pwndQTeach_DT[WhichAxis] != NULL) // Set DelayTime
		{
			// Get data DelayTime value from db
			char	SetDT_DBID[256];	memset(SetDT_DBID, 0, sizeof(SetDT_DBID));
			pwndQTeach_DT[WhichAxis]->GetPropValueT("dbid1", SetDT_DBID, sizeof(SetDT_DBID));
			AxisSetDT[WhichAxis] = GetDBValue(SetDT_DBID).lValue;
			// Update show data
			pwndQTeach_DT[WhichAxis]->SetPropValueT("value",AxisSetDT[WhichAxis]);
			pwndQTeach_DT[WhichAxis]->Update();
			pwndQTeach_DT[WhichAxis]->UpdateAll();
		}
		
	}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateClamp()                  						   				   		     |
|       		: 治具選擇顯示																									 |
+---------------------------------------------------------------------------*/
void	UpdateClamp()
{
	printf("Update Clamp\n");
	ClampSelect = GetDBValue(ClampSelect_DB).lValue;
	ClampDetectSelect = GetDBValue(ClampDetectSelect_DB).lValue;
	
	//printf("ClampSelect=%d\n",ClampSelect);
	
	// 遮罩
	if(pwndClamp_Mask!=NULL)
		{
			pwndClamp_Mask->Update();
		}
	for(int i=0; i<10; i++)
 	{
 		//if(((ClampSelect>>i) & 1)==1)
 		//	printf("Select[%d] = %d\n",i,((ClampSelect>>i) & 1)); // 治具選擇確認
 		b_Select[i] = ((ClampSelect>>i) & 1);
	 	if((pwndClamp_Select[i]!=NULL)&&(pwndCheckBox[i]!=NULL))  //治具選擇
	 	{
			pwndCheckBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Select[i]]);
			pwndCheckBox[i]->CreateA();
			pwndCheckBox[i]->Update();
	 	}
 		b_DetectSelect[i] = ((ClampDetectSelect>>i) & 1);
	 	if(pwndDetect_Select[i]!=NULL)  //治具檢測選擇
	 	{
			pwndDetect_Select[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_DetectSelect[i]]);
			pwndDetect_Select[i]->CreateA();
			pwndDetect_Select[i]->Update();
	 	}
	 	
	 	if(pwndBtn_Clamp[i]!=NULL) // 治具 ON/OFF Btn
 		{
 			if(b_Select[i]==1)
			{
				pwndBtn_Clamp[i]->SetPropValueT("enabled",double(TRUE));
				pwndBtn_Clamp[i]->SetPropValueT("upbitmap",Clamp_ImgPath[(u_clamp_onoff[i]*10+i)]); //Clamp_Hint_ImgPath //Clamp_ImgPath
				pwndBtn_Clamp[i]->CreateA();
				pwndBtn_Clamp[i]->Update();
			}
			else
			{
				pwndBtn_Clamp[i]->SetPropValueT("enabled",double(FALSE));
			}
 		}
 		if(pwndQTeach_Clamp_DT[i]!=NULL) // 治具 延時 數值輸入
		{
 			if(b_Select[i]==1)
			{
				pwndQTeach_Clamp_DT[i]->SetPropValueT("enabled",double(TRUE));
				pwndQTeach_Clamp_DT[i]->CreateA();
				pwndQTeach_Clamp_DT[i]->Update();
			}
			else
			{
				pwndQTeach_Clamp_DT[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
 		if(pwndDetectBox[i]!=NULL) // 治具 檢測 勾勾
		{
 			if(b_Select[i]==1)
			{
				pwndDetectBox[i]->SetPropValueT("enabled",double(TRUE));
				pwndDetectBox[i]->CreateA();
				pwndDetectBox[i]->Update();
			}
			else
			{
				pwndDetectBox[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
 	}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateShow()                  						   				   		     |
|       		: 畫面初始顯示																									 |
+---------------------------------------------------------------------------*/
void	UpdateShow()
{
	UpdateDataValue();
	UpdateClamp();
	// 確認治具選用
	for(int i=0;i<10;i++)
	{
		if(b_Select[i]==Selected)
			{
				u_SelectClamp++;
				SelectClamp[u_SelectClamp]=i;
			}
	}
	printf("u_SelectClamp=%d\n",u_SelectClamp);
	
	// 確認治具檢測選用
	for(int i=0;i<10;i++)
	{
		if(b_DetectSelect[i]==Selected)
			{
				u_SelectDetect++;
				SelectDetect[u_SelectDetect]=i;
			}
	}
	printf("u_SelectDetect=%d\n",u_SelectDetect);
	UpdateBtnDetect_Select();
	UpdateImg();
	if((u_Group == 0)||(u_Group == ReadyDownP))
		Update_Check_Move();
	UpdateHint();
}
/*---------------------------------------------------------------------------+
|  Function : UpdateSubArm()                  													     |
|       		: 刷新提示 副臂選用 同步勾勾																		 |
+---------------------------------------------------------------------------*/
void UpdateSubArm() // 更新 副臂選用 按鈕們
{
	if(pwndMask_SubArm!=NULL) // 副臂 遮罩
	{
		pwndMask_SubArm->Update();
	}
	
	if( pwndCheck_SubUse!=NULL && pwndBtn_SubUse!=NULL) // 副臂使用 Btn
	{
		if(u_PickerType==MechType3) // 3 軸不顯示
		{
			pwndCheck_SubUse->SetPropValueT("enabled",(double)(FALSE));
			pwndBtn_SubUse->SetPropValueT("enabled",(double)(FALSE));
		}
		else if(u_PickerType==MechType5) // 五軸 才顯示
		{
			pwndCheck_SubUse->SetPropValueT("enabled",(double)(TRUE)); // 勾勾
			pwndCheck_SubUse->SetPropValueT("upbitmap",CheckBox_ImgPath[b_SubArmUse]);
			pwndCheck_SubUse->CreateA();
			pwndCheck_SubUse->Update();
			
			pwndBtn_SubUse->SetPropValueT("enabled",(double)(TRUE)); //Btn
			pwndBtn_SubUse->Update();
		}
	}
	if( pwndCheck_SubSync!=NULL && pwndBtn_SubSync!=NULL) // 副臂與主臂同動 Btn
	{
		if(b_SubArmUse==TRUE) // 副臂使用 副臂與主臂同動才可選
		{
			pwndCheck_SubSync->SetPropValueT("enabled",(double)(TRUE)); // 勾勾
			pwndCheck_SubSync->SetPropValueT("upbitmap",CheckBox_ImgPath[b_SubArmSync]);
			pwndCheck_SubSync->CreateA();
			pwndCheck_SubSync->Update();
			
			pwndBtn_SubSync->SetPropValueT("enabled",(double)(TRUE)); // Btn
			pwndBtn_SubSync->Update();
		}
		else
		{
			pwndCheck_SubSync->SetPropValueT("enabled",(double)(FALSE)); // 勾勾
			pwndBtn_SubSync->SetPropValueT("enabled",(double)(FALSE)); // Btn
		}
	}
	
	
}
/*---------------------------------------------------------------------------+
|  Function : UpdateBtnDetect_Select()                  								     |
|       		: 刷新提示 治具選擇 檢測勾勾																		 |
+---------------------------------------------------------------------------*/
void UpdateBtnDetect_Select() // 更新 檢測選擇按紐
{
	printf("UpdateBtnDetect_Select\n");
	if(pwndDetect_Mask1!=NULL) // 檢測遮罩1
	{
		pwndDetect_Mask1->Update();
	}
	if(pwndDetect_Mask2!=NULL) // 檢測遮罩2
	{
		pwndDetect_Mask2->Update();
	}
		
	for(int i=0;i<10;i++) // 檢測 Btn
	{
		if(pwndDetect_Select[i]!=NULL)
		{
			if(b_Select[i])
			{
				pwndDetect_Select[i]->SetPropValueT("enabled",(double)(TRUE));
				pwndDetect_Select[i]->CreateA();
				pwndDetect_Select[i]->Update();	
				if(i==Valve_Clamp1 || i==Valve_Clamp2)
					{
						if(pwndStr_Detect1!=NULL)
							pwndStr_Detect1->Update();
					}
				if(i==Valve_Suck1 || i==Valve_Suck2)
					{
						if(pwndStr_Detect2!=NULL)
							pwndStr_Detect2->Update();
					}
			}
			else
			{
				pwndDetect_Select[i]->SetPropValueT("enabled",(double)(FALSE));
			}
		}
	}
}
/*---------------------------------------------------------------------------+
|  Function : Update_ClampStr()                  								  				   |
|       		: 刷新提示 被選擇治具 文字說明																	 |
+---------------------------------------------------------------------------*/
void Update_ClampStr()
{
	printf("Update_ClampStr\n");
	for(int i=0;i<10;i++)
	{
		if(b_Select[i]) // 治具i被選擇
		{
			if(i<=Valve_Clamp2) // 夾具
				{
					if(pwndStr_Clamp!=NULL)
						pwndStr_Clamp->Update();
					if(pwndStr_Clamp_Detect1!=NULL)
						pwndStr_Clamp_Detect1->Update();
					if(pwndStr_Clamp_DT1!=NULL)
						pwndStr_Clamp_DT1->Update();
				}
			else if(i>=Valve_Suck1) // 吸盤
				{
					if(pwndStr_Suck!=NULL)
						pwndStr_Suck->Update();
					if(pwndStr_Clamp_Detect2!=NULL)
						pwndStr_Clamp_Detect2->Update();
					if(pwndStr_Clamp_DT2!=NULL)
						pwndStr_Clamp_DT2->Update();
				}
		}
	}
}		
/*---------------------------------------------------------------------------+
|  Function : Update_SLVL()                  										  				   |
|       		: 刷新提示 柔性使用 柔性設置																		 |
+---------------------------------------------------------------------------*/
void	Update_SLVL()
{
	printf("Update_SLVL\n");
	if(pwndMask_SLVL!=NULL) // 遮罩
	{
		pwndMask_SLVL->Update();
	}
	
	if(b_Check_SLVL) // 若 選擇使用
	{
		if(pwndPick_SLVL!=NULL)
		{
			pwndPick_SLVL->Show(); // 顯示
		}
		if(pwndPick_Dis!=NULL)
		{
			pwndPick_Dis->Show(); // 顯示
		}
		if(pwndPickOut_SLVL!=NULL)
		{
			pwndPickOut_SLVL->Show(); // 顯示
		}
		if(pwndPickOut_Dis!=NULL)
		{
			pwndPickOut_Dis->Show(); // 顯示
		}
		if(pwndSLVL_Logo!=NULL)
		{
			pwndSLVL_Logo->Show(); // 顯示
		}
	}
	else
	{
	}
}

/*---------------------------------------------------------------------------+
|  Function : UpdateBtnTeachStart()                  										     |
|       		: 刷新提示 開始教導																							 |
+---------------------------------------------------------------------------*/
void	UpdateBtnTeachStart()
{
	ClampSelect=0;
	for(int i=0;i<10;i++)
	{
		ClampSelect=ClampSelect+(b_Select[i]<<i);
	}

	if(pwndBtnTeachStart!=NULL) // 開始教導
	{
		if(ClampSelect>0)
			{
				pwndBtnTeachStart->SetPropValueT("upbitmap","res_tm640/pic/PGBtnSelect.bmp");
			}
		else
			{
				pwndBtnTeachStart->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
			}
		pwndBtnTeachStart->CreateA();
		pwndBtnTeachStart->Update();
	}
	if(pwndStr_ClampSelect!=NULL) // 治具選用
	{
		if(ClampSelect>0)
		{
			pwndStr_ClampSelect->SetPropValueT("bgc",0xF7BE); // 灰色
		}
		else
		{
			pwndStr_ClampSelect->SetPropValueT("bgc",0xFF80); // 黃色
		}
		pwndStr_ClampSelect->CreateA();
		pwndStr_ClampSelect->Update();
	}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateBtnNextP()                  										  		   |
|       		: 刷新提示 下一位置																							 |
+---------------------------------------------------------------------------*/
void	UpdateBtnNextP()
{
	printf("UpdateBtnNextP()\n");
	if(pwndBtnNextP!=NULL)
	{
		printf("b_All_PosSet_OK==%d,b_ClampSet_OK=%d\n",b_All_PosSet_OK,b_ClampSet_OK);
		if(b_All_PosSet_OK==OK) // 位置 && 治具 && 速度 設定完成 &&(b_All_SpdSet_OK==OK) &&(b_ClampSet_OK==OK)
			{
				SetDBValue(pAll_PosSet_OK_DB, 1); // 寫入 位置設定完成 db
				b_BtnNextP_OK = OK;
				pwndBtnNextP->SetPropValueT("upbitmap","res_tm640/pic/PGBtnSelect.bmp");
			}
		else
			{
				b_BtnNextP_OK = 0;
				pwndBtnNextP->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
			}
		pwndBtnNextP->CreateA();
		pwndBtnNextP->Update();
	}
}

/*---------------------------------------------------------------------------+
|  Function : UpdateTeach_Pos()     	             										  		 |
|       		: 刷新提示 位置設定值																						 |
+---------------------------------------------------------------------------*/
void	UpdateTeach_Pos()
{
	printf("UpdateTeach_Pos()\n");
	int u_All_PosSet_OK=0;
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		if(pwndQTeach_Pos[i]!=NULL)
		{
			if(b_All_PosSet_OK==1)
				b_PosSet_OK[i]=OK;
			if(b_PosSet_OK[i]==OK)
			{
				u_All_PosSet_OK++;
				pwndQTeach_Pos[i]->SetPropValueT("bgc",Color_LBlue); // 白色
			}
			else
			{
				pwndQTeach_Pos[i]->SetPropValueT("bgc",Color_Yellow); // 黃色
			}
			//pwndQTeach_Pos[i]->CreateA();
			pwndQTeach_Pos[i]->Show();
			pwndQTeach_Pos[i]->OnLoseFocus();
		}
	} 
	printf("Teach_SetNum=%d,u_All_PosSet_OK=%d\n",Teach_SetNum[u_PickerType][u_Group],u_All_PosSet_OK);
	if(u_All_PosSet_OK>=Teach_SetNum[u_PickerType][u_Group])
		b_All_PosSet_OK =OK; //全部位置皆設定完成
}
/*---------------------------------------------------------------------------+
|  Function : UpdateTeach_Speed()     	             										  	 |
|       		: 刷新提示 速度設定值																						 |
+---------------------------------------------------------------------------*/
void	UpdateTeach_Speed()
{
	printf("UpdateTeach_Speed()\n");
//	int u_All_SpdSet_OK=0;
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		if(pwndQTeach_Speed[i]!=NULL)
		{
//			if(b_SpdSet_OK[i]==OK)
//			{
//				u_All_SpdSet_OK++;
//				pwndQTeach_Speed[i]->SetPropValueT("bgc",Color_LBlue); // 白色
//			}
//			else
//			{
//				pwndQTeach_Speed[i]->SetPropValueT("bgc",Color_Yellow); // 黃色
//			}
			pwndQTeach_Speed[i]->Show();
			pwndQTeach_Speed[i]->OnLoseFocus();
		}
	} 
//	if(u_All_SpdSet_OK>=Teach_SetNum[u_PickerType][u_Group])
//		b_All_SpdSet_OK =OK; //全部位置皆設定完成
}


/*---------------------------------------------------------------------------+
|  Function : UpdateHint()                  												  		   |
|       		: 刷新提示 																											 |
+---------------------------------------------------------------------------*/
void	UpdateHint()
{
	UpdateBtnTeachStart();
	UpdateTeach_Pos(); // 更新 位置設定
	UpdateTeach_Speed(); // 更新 速度設定
	UpdateBtnNextP(); // 最後更新 下一位置 提示顯示
}
/*---------------------------------------------------------------------------+
|  Function : UpdateCheck_Pile()           																	 |
|       		: 刷新提示 堆疊選擇																							 |
+---------------------------------------------------------------------------*/
void	UpdateCheck_Pile()
{
	// 堆疊選擇
	if(PileNum>0)
	{
		if(pwndCheck_Pile!=NULL)
			pwndCheck_Pile->SetPropValueT("upbitmap",CheckBox_ImgPath[OK]);
	}
	else
	{
		if(pwndCheck_Pile!=NULL)
			pwndCheck_Pile->SetPropValueT("upbitmap",CheckBox_ImgPath[NO]);
	}
	if(pwndCheck_Pile!=NULL)
	{
		pwndCheck_Pile->CreateA();
		pwndCheck_Pile->Show();
	}
	
	// 堆疊組數 遮罩
	if(pwndMask_Pile!=NULL)
		pwndMask_Pile->Show();
	
	
	// 堆疊組數 選擇鈕
	if(PileNum>0)
	{
		// enable
		for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ ) // 堆疊組數 選擇鈕
		{
			if(pwndBtn_PileChoose[i]!=NULL)
			{
				pwndBtn_PileChoose[i]->SetPropValueT("enabled",double(TRUE));
				pwndBtn_PileChoose[i]->Show();
			}
		}
		for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ ) // 堆疊組數 Btn
		{
			if(pwndBtn_PileNum[i]!=NULL)
			{
				pwndBtn_PileNum[i]->SetPropValueT("enabled",double(TRUE));
				pwndBtn_PileNum[i]->Show();
			}
		} 
		// Update Img
		for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ )
		{
			if(pwndBtn_PileChoose[i]!=NULL)
			{
				if(i==PileNum)
					{
						pwndBtn_PileChoose[i]->SetPropValueT("upbitmap",ChooseBox_ImgPath[OK]);
					}
				else
					pwndBtn_PileChoose[i]->SetPropValueT("upbitmap",ChooseBox_ImgPath[NO]);
				pwndBtn_PileChoose[i]->CreateA();
				pwndBtn_PileChoose[i]->Show();
			}
		} 
	}
	else
	{
		// disable
		for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ ) // 堆疊組數 選擇鈕
		{
			if(pwndBtn_PileChoose[i]!=NULL)
			{
				pwndBtn_PileChoose[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
		for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ ) // 堆疊組數 Btn
		{
			if(pwndBtn_PileNum[i]!=NULL)
			{
				pwndBtn_PileNum[i]->SetPropValueT("enabled",double(FALSE));
			}
		} 
	}
	
	
}
/*---------------------------------------------------------------------------+
|  Function : Update_AxisMoveNow()           																 |
|       		: 更新 現在移動軸																								 |
+---------------------------------------------------------------------------*/
void	Update_AxisMoveNow(int Axis)
{
	if(pwndImg_NowMoveAxis!=NULL) // 圖片
	{
		pwndImg_NowMoveAxis->SetPropValueT("imagepath",Axis_ImgPath[Axis]);
		pwndImg_NowMoveAxis->Show();
	}
	if(pwndStr_NowMoveAxis !=NULL) // 文字
	{
		if(Axis==0)
			pwndStr_NowMoveAxis->SetPropValueT("textID","PICKER_NULL");
		else
			pwndStr_NowMoveAxis->SetPropValueT("textID","TEACH_QTEACH_NOW_MOVE");
		//pwndStr_NowMoveAxis->CreateA();
		pwndStr_NowMoveAxis->Show();
	}
	
}
			

/*---------------------------------------------------------------------------+
|  Function : SaveAction2Temp()                  												  	 |
|       		: 將快速教導動作存入暫存g_QTeach_Action_P												 |
+---------------------------------------------------------------------------*/
void	SaveAction2Temp(int QTeach_PGNo,int j)
{
	/*======================================= 步驟序號 STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_P[j].Step);
	/*======================================= 動作類型 TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_P[j].Type);
	/*======================================= 動作形式 NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_P[j].Num);
	/*======================================= 參數1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_P[j].P1);
	/*======================================= 參數2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_P[j].P2);
	/*======================================= 參數3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_P[j].P3);
	/*======================================= 參數4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_P[j].P4);
	/*======================================= 參數5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_P[j].P5);
}

/*---------------------------------------------------------------------------+
|  Function : SaveAct_Axis2Temp()                  													 |
|       		: 將快速教導動作 軸動作 存入暫存g_QTeach_Action_P								 |
+---------------------------------------------------------------------------*/
void	SaveAct_Axis2Temp(int QTeach_PGNo,int Axis)
{
	/*======================================= 步驟序號 STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[Axis].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].Step);
	/*======================================= 動作類型 TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Axis_P[Axis].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].Type);
	/*======================================= 動作形式 NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Axis_P[Axis].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].Num);
	/*======================================= 參數1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Axis_P[Axis].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P1);
	/*======================================= 參數2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Axis_P[Axis].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P2);
	/*======================================= 參數3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Axis_P[Axis].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P3);
	/*======================================= 參數4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Axis_P[Axis].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P4);
	/*======================================= 參數5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Axis_P[Axis].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P5);
}
/*---------------------------------------------------------------------------+
|  Function : SaveAct_Valve2Temp()                  												 |
|       		: 將快速教導動作 閥門 存入暫存g_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Valve2Temp(int QTeach_PGNo,int j)
{
	/*======================================= 步驟序號 STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[j].Step);
	/*======================================= 動作類型 TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Valve_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Valve_P[j].Type);
	/*======================================= 動作形式 NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Valve_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Valve_P[j].Num);
	/*======================================= 參數1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Valve_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P1);
	/*======================================= 參數2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Valve_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P2);
	/*======================================= 參數3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Valve_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P3);
	/*======================================= 參數4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Valve_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P4);
	/*======================================= 參數5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Valve_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P5);
}

/*---------------------------------------------------------------------------+
|  Function : SaveAct_Valve2Temp()                  												 |
|       		: 將快速教導動作 檢測 存入暫存g_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Detect2Temp(int QTeach_PGNo,int j)
{
	/*======================================= 步驟序號 STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[j].Step);
	/*======================================= 動作類型 TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Detect_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Detect_P[j].Type);
	/*======================================= 動作形式 NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Detect_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Detect_P[j].Num);
	/*======================================= 參數1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Detect_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P1);
	/*======================================= 參數2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Detect_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P2);
	/*======================================= 參數3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Detect_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P3);
	/*======================================= 參數4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Detect_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P4);
	/*======================================= 參數5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Detect_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P5);
}
/*---------------------------------------------------------------------------+
|  Function : SaveAct_Wait2Temp()                  												 |
|       		: 將快速教導動作 等待 存入暫存g_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Wait2Temp(int QTeach_PGNo,int j)
{
	printf("SaveAct_Wait2Temp(%d,%d)\n",QTeach_PGNo,j);
	/*======================================= 步驟序號 STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Wait_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Wait_P[j].Step);
	/*======================================= 動作類型 TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Wait_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Wait_P[j].Type);
	/*======================================= 動作形式 NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Wait_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Wait_P[j].Num);
	/*======================================= 參數1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Wait_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P1);
	/*======================================= 參數2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Wait_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P2);
	/*======================================= 參數3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Wait_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P3);
	/*======================================= 參數4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Wait_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P4);
	/*======================================= 參數5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Wait_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P5);
}


/*---------------------------------------------------------------------------+
|  Function : SaveAct_Pile2Temp()                  													 |
|       		: 將快速教導動作 堆疊 存入暫存g_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Pile2Temp(int QTeach_PGNo,int j)
{
	/*======================================= 步驟序號 STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[j].Step);
	/*======================================= 動作類型 TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Pile_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Pile_P[j].Type);
	/*======================================= 動作形式 NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Pile_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Pile_P[j].Num);
	/*======================================= 參數1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Pile_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P1);
	/*======================================= 參數2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Pile_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P2);
	/*======================================= 參數3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Pile_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P3);
	/*======================================= 參數4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Pile_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P4);
	/*======================================= 參數5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Pile_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P5);
}


/*---------------------------------------------------------------------------+
|  Function : UnLockBtn()                  																	 |
|       		: 將	pwnd 元件解鎖																							 |
+---------------------------------------------------------------------------*/
void	UnLockBtn(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		((CtmFormView*)pwnd)->OnLoseFocus();
		pwnd->SetPropValueT("enabled",double(TRUE));
		pwnd->CreateA();
		pwnd->Update();
	}
}
/*---------------------------------------------------------------------------+
|  Function : LockBtn()                  																		 |
|       		: 將	pwnd 元件上鎖																							 |
+---------------------------------------------------------------------------*/
void	LockBtn(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{		
		((CtmFormView*)pwnd)->OnLoseFocus();
		pwnd->SetPropValueT("enabled",double(FALSE));
		pwnd->CreateA();
		pwnd->Update();
	}
}
/*---------------------------------------------------------------------------+
|  Function : SetFromWaitP()                  															 |
|       		: 獲取WaitP點 的位置 速度 延時 參數															 |
+---------------------------------------------------------------------------*/
void	SetFromWaitP(int WhichAxis)
{
	int value =0;
	// 位置
	if(pwndQTeach_Pos[WhichAxis]!=NULL)
	{
		value= GetDBValue(P2_POSSET_DBString[WhichAxis]).lValue; // 取得 WaitP DB值
		SetDBValue(P4_POSSET_DBString[WhichAxis], value); // 寫入 ReadyDownP db
	}
	// 速度
	if(pwndQTeach_Speed[WhichAxis]!=NULL)
	{
		value= GetDBValue(P2_Speed_DBString[WhichAxis]).lValue; // 取得 WaitP DB值
		SetDBValue(P4_Speed_DBString[WhichAxis], value); // 寫入 ReadyDownP db
	}
	// 延時
	if(pwndQTeach_DT[WhichAxis]!=NULL)
	{
		value= GetDBValue(P2_DT_DBString[WhichAxis]).lValue; // 取得 WaitP DB值
		SetDBValue(P4_DT_DBString[WhichAxis], value); // 寫入 ReadyDownP db
	}
}
		
/*---------------------------------------------------------------------------+
|  Function : Update_PosHint(int Axis)               												 |
|       		: 更新 位置提示																									 |
+---------------------------------------------------------------------------*/
void	Update_PosHint(int Axis)
{
	printf("Update_PosHint(%d)\n",Axis);
	if(pwndImg_PosHint[Axis]!=NULL)
	{
		long Max_value=0,Min_value=0;
		char	MAX_DBID[256];	memset(MAX_DBID, 0, sizeof(MAX_DBID));
		if(pwndQTeach_Pos[Axis]!=NULL)
			pwndQTeach_Pos[Axis]->GetPropValueT("maxid", MAX_DBID, sizeof(MAX_DBID));
		Max_value = GetDBValue(MAX_DBID).lValue;
		gQTeach_PosLimt[Axis][MAX]=Max_value; // 紀錄位置設定的最大值上限
		char	MIN_DBID[256];	memset(MIN_DBID, 0, sizeof(MIN_DBID));
		pwndQTeach_Pos[Axis]->GetPropValueT("minid", MIN_DBID, sizeof(MIN_DBID));
		Min_value = GetDBValue(MIN_DBID).lValue;			
		gQTeach_PosLimt[Axis][MIN]=Min_value; // 紀錄位置設定的最小值上限
		
		pwndMask_PosHint[Axis]->Show(); // 遮罩
		if(AxisPosNow[Axis]>Max_value) // 現在位置超過最大值
		{
			pwndImg_PosHint[Axis]->SetPropValueT("imagepath","res_tm640/pic/picker/PosHint_NO.bmp");
			pwndImg_PosHint[Axis]->SetPropValueT("left",317+(80)); // 標示 顯示位置
			pwndImg_PosHint[Axis]->SetPropValueT("right",317+(80)+20); // 標示 顯示位置
		}
		else if(AxisPosNow[Axis]<Min_value) // 現在位置 小於最小值
		{
			pwndImg_PosHint[Axis]->SetPropValueT("imagepath","res_tm640/pic/picker/PosHint_NO.bmp");
			pwndImg_PosHint[Axis]->SetPropValueT("left",317); // 標示 顯示位置
			pwndImg_PosHint[Axis]->SetPropValueT("right",317+20); // 標示 顯示位置
		}
		else // 合理的位置
		{
			if((Max_value-Min_value)!=0)
			{
				pwndImg_PosHint[Axis]->SetPropValueT("imagepath","res_tm640/pic/picker/PosHint_OK.bmp");
				pwndImg_PosHint[Axis]->SetPropValueT("left",317+((AxisPosNow[Axis]-Min_value)*80/(Max_value-Min_value))); // 標示 顯示位置
				pwndImg_PosHint[Axis]->SetPropValueT("right",317+((AxisPosNow[Axis]-Min_value)*80/(Max_value-Min_value))+20); // 標示 顯示位置
			}
		}
		pwndImg_PosHint[Axis]->CreateA();
		pwndImg_PosHint[Axis]->Show();
		
		if(pwndQTeach_Pos_Max[Axis]!=NULL) // 位置上下限 顯示文字
		{
			char StrValue[10]="\0";
			memset(StrValue, 0, sizeof(StrValue));
			sprintf(StrValue,"%d"".""%d", (Max_value/100), (Max_value%100)); // 2位小數
			pwndQTeach_Pos_Max[Axis]->SetPropValueT("text",StrValue);
			pwndQTeach_Pos_Max[Axis]->CreateA();
			pwndQTeach_Pos_Max[Axis]->Show();
		}
		if(pwndQTeach_Pos_Min[Axis]!=NULL) // 位置上下限 顯示文字
		{
			char StrValue[10]="\0";
			memset(StrValue, 0, sizeof(StrValue));
			sprintf(StrValue,"%d"".""%d", (Min_value/100), (Min_value%100)); // 2位小數
			pwndQTeach_Pos_Min[Axis]->SetPropValueT("text",StrValue);
			pwndQTeach_Pos_Min[Axis]->CreateA();
			pwndQTeach_Pos_Min[Axis]->Show();
		}
	}
	

}
/*---------------------------------------------------------------------------+
|  Function : Update_Check_Move()               														 |
|       		: 更新 移動後下降 設定按鍵																			 |
+---------------------------------------------------------------------------*/
void	Update_Check_Move() // 更新 移動後下降 設定按鍵	
{
	// Axis_X1
	if( (pwndQTeach_Pos[Axis_X1]!=NULL) && (pwndQTeach_Speed[Axis_X1]!=NULL) && (pwndQTeach_DT[Axis_X1]!=NULL) && (pwndBtn_GetPos[Axis_X1]!=NULL) )
	{
		if(b_Check_Move)
		{
			//b_All_PosSet_OK=0; // 需設定位置
			UnLockBtn(pwndQTeach_Pos[Axis_X1]);
			UnLockBtn(pwndQTeach_Speed[Axis_X1]);
			UnLockBtn(pwndQTeach_DT[Axis_X1]);
			UnLockBtn(pwndBtn_GetPos[Axis_X1]);
		}
		else
		{
			//b_All_PosSet_OK=OK; // 位置設定完成
			LockBtn(pwndQTeach_Pos[Axis_X1]);
			LockBtn(pwndQTeach_Speed[Axis_X1]);
			LockBtn(pwndQTeach_DT[Axis_X1]);
			LockBtn(pwndBtn_GetPos[Axis_X1]);
			SetFromWaitP(Axis_X1);// 取用 WaitP X1的設定
		}
	}
	// Axis_X2
	if( (pwndQTeach_Pos[Axis_X2]!=NULL) && (pwndQTeach_Speed[Axis_X2]!=NULL) && (pwndQTeach_DT[Axis_X2]!=NULL) && (pwndBtn_GetPos[Axis_X2]!=NULL) )
	{
		if(b_Check_Move)
		{
			//b_All_PosSet_OK=0; // 需設定位置
			UnLockBtn(pwndQTeach_Pos[Axis_X2]);
			UnLockBtn(pwndQTeach_Speed[Axis_X2]);
			UnLockBtn(pwndQTeach_DT[Axis_X2]);
			UnLockBtn(pwndBtn_GetPos[Axis_X2]);
		}
		else
		{
			//b_All_PosSet_OK=OK; // 位置設定完成
			LockBtn(pwndQTeach_Pos[Axis_X2]);
			LockBtn(pwndQTeach_Speed[Axis_X2]);
			LockBtn(pwndQTeach_DT[Axis_X2]);
			LockBtn(pwndBtn_GetPos[Axis_X2]);
			SetFromWaitP(Axis_X2);// 取用 WaitP X2的設定
		}
	}
	if(pwndCheck_Move!=NULL)
	{
		pwndCheck_Move->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Move]);
		pwndCheck_Move->CreateA();
		pwndCheck_Move->Update();
	}
}

/*---------------------------------------------------------------------------+
|  Function : GetDownDis()             							  											 |
|       		: 計算 下降距離																									 |
+---------------------------------------------------------------------------*/
void	GetDownDis() // 計算 下降距離
{
	int DownDis=0; //下降距離
	int u_WaitPos_Y1 		 = 0; // 等待點 Y1 設定位置
	int u_PickDownPos_Y1 = 0; // 取出下降點 Y1 設定位置

	u_WaitPos_Y1 = GetDBValue(P2_POSSET_DBString[Axis_Y1]).lValue;
	printf("u_WaitPos_Y1=%d\n",u_WaitPos_Y1);
	u_PickDownPos_Y1 = GetDBValue(P5_POSSET_DBString[Axis_Y1]).lValue;
	printf("u_PickDownPos_Y1=%d\n",u_PickDownPos_Y1);
	DownDis = u_PickDownPos_Y1-u_WaitPos_Y1;
	printf("DownDis=%d\n",DownDis);
	if(pwndPick_SLVL!=NULL)
		pwndPick_SLVL->SetPropValueT("max",DownDis);
	if(pwndPick_Dis!=NULL)
		{
			pwndPick_Dis->SetPropValueT("value",DownDis);
			printf("Set pwndPick_Dis=%d\n",DownDis);
			pwndPick_Dis->CreateA();
			pwndPick_Dis->Show();
		}
}


/*---------------------------------------------------------------------------+
|  Function : GetBackDis()             							  											 |
|       		: 計算 後退距離																									 |
+---------------------------------------------------------------------------*/
void	GetBackDis() // 計算 後退距離
{
	// 計算 後退距離
	int u_BackDis				 = 0; // 後退距離
	int u_MoveOutPos_X1	 = 0; // 橫出點 X1 設定位置
	int u_PickPos_X1 		 = 0; // 取出取件點 X1 設定位置
	u_MoveOutPos_X1 = GetDBValue(P7_POSSET_DBString[Axis_X1]).lValue;

	u_PickPos_X1 = GetDBValue(P6_POSSET_DBString[Axis_X1]).lValue;

	u_BackDis = abs(u_MoveOutPos_X1-u_PickPos_X1);
	printf("u_BackDis=%d\n",u_BackDis);
	if(pwndPickOut_SLVL!=NULL)
		pwndPickOut_SLVL->SetPropValueT("max",u_BackDis);
	if(pwndPickOut_Dis!=NULL)
	{
		pwndPickOut_Dis->SetPropValueT("value",u_BackDis);
		pwndPickOut_Dis->CreateA();
		pwndPickOut_Dis->Show();
	}
}
/*---------------------------------------------------------------------------+
|  Function : SaveSLVLData()                  		   									       |
|       		: 獲取當前 柔性 設定數值	存入DB																 |
+---------------------------------------------------------------------------*/
void	SaveSLVLData() // 獲取當前 柔性 設定數值	存入DB	
{
	printf("SaveSLVLData!!!!!!!!!!!!!!!!!!!!!\n");
	long value = 0;
	// 取件柔性
	if(pwndPick_SLVL!=NULL)
	{
		pwndPick_SLVL->GetPropValueT("value", &value,sizeof(value));
		SetDBValue(Pick_SLVL_DB,value); // 數值 寫入db
		printf("Set %s=%d\n",Pick_SLVL_DB,value);
	}
	
	// 取出柔性
	if(pwndPickOut_SLVL!=NULL)
	{
		pwndPickOut_SLVL->GetPropValueT("value", &value,sizeof(value));
		SetDBValue(PickOut_SLVL_DB,value); // 數值 寫入db
		printf("Set %s=%d\n",PickOut_SLVL_DB,value);
	}
}
/*---------------------------------------------------------------------------+
|  Function : SaveData(CtmWnd* pwnd, char* DB_ID)	   									       |
|       		: pwnd 元件指標									 																 |
|       		: 獲取當前 設定數值	存入DB																			 |
+---------------------------------------------------------------------------*/
void	ShowData(CtmWnd* pwnd)
{
	char	DB_ID[256];	memset(DB_ID, 0, sizeof(DB_ID));
	pwnd->GetPropValueT("dbid1", DB_ID, sizeof(DB_ID)); // 取得資料庫ID
			
	long value = 0;
	if(pwnd != NULL)
	{
		value = GetDBValue(DB_ID).lValue; // 讀取DB數值
		pwnd->SetPropValueT("value",value); // 寫入數值
		printf("Show %s=%d\n",DB_ID,value);
		pwnd->CreateA();
		pwnd->Update();
	}
}