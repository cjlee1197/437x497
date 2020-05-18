/*===========================================================================+
|  Class    : ViewVersion library                                         	 |
|  Task     : ViewVersion library  header file                            	 |
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

#ifndef D__VIEWVERSION
#define D__VIEWVERSION
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../moldControl.h"
#include	"../lib/libComm.h"


extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);
extern	"C" WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void		SetDBRM(tmDB_DIAGRM* db_diagrm, CtmWnd* pwnd[], int nCount);

void       	CreateSet(CtmWnd* pwndSender);
void		ShowTransmitCom(CtmWnd* pwndSender);
void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void   		ReadDingDanFrom();

char*		DW2Str_ASCII(DWORD dw_dbvalue); //cjlee 2019/5/12 下午 09:51:01

void	GetValueFrom28(); // 取得28設定值
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect); // 設定選擇DB數值
void	SprintfDBValue(int iParam497, int iParam28); // 處理DB數值 顯示文字
void	SprintfDBValue(char* strParam497,char* strParam28); // 處理DB數值 顯示文字

typedef struct tagAxis
{
	int iDouble; 				// 倍數機構 
	int iPosInv; 				// 位置反向
	int iENC_REZ;				// 編碼器解析度
	int iMotorRatio;		// 減速比
	int iMotorDiameter; // 節圓直徑
	int iToothNum;			// 齒數
	int iM_Num;					// 模數
	int iMaxSpeed;			// 最高轉速
	int iSpeedACCT;			// 加速時間
	int iSpeedDCCT;			// 減速時間
	int iACC_SmoothR;   // 加速平滑比例
	int iDCC_SmoothR;   // 減速平滑比例
	int iPosition_KP;		// 到位響應 位置KP
	int iSpeed_KP;			// 追隨響應 速度KP
}tmAxis;



typedef struct tagParam
{
	int iMechType;			// 機型選擇
	int iPickerType;  	// 機型選擇 機型
	int iEncType;				// 機型選擇 編碼器
	int iTransType; 		// 傳動方式
	int iDouble;				// 倍數機構
	int iPosInv;				// 位置反向
	int Axis[5][5];			// 軸參數
	int Speed[5][7];		// 速度參數
	int ZoneLimit[6][3];// 區域設定參數
	int Mode[3];				// 模式選擇
	int Detect[6];			// 檢測選擇
	int Delay[4];				// 延時選擇
	int Prod[2];				// 生產參數
	int	Pile[4][6];			// 堆疊參數
}tmParam;

enum PrecisionList		// 參數精度 小數點位置數
{
	PAxis,			// 軸參數
	PSpeed,			// 速度參數
	PZoneLimit,	// 區域參數
};

enum
{
	/*機構參數*/
	MECH_MECHTYPE, //0 機型選擇
	TRANS_TYPE,		 //1 傳動方式
	MECH_ENCTYPE,	 //2 編碼器選擇
	MOTOR_ENC_REZ, //3 編碼器解析度
	MOTOR_RATIO,	 //4 減速比
	MECH_GEAR_D,	 //5 每轉距離
	MECH_DOUB_MECH,//6 倍數機構 
	MECH_POS_INV,	 //7 位置反向
	/*速度參數*/
	SPEED_MAX,	 // 最高轉速
	SPEED_ACC_T,	 // 加速時間
	SPEED_DCC_T,	 // 減速時間
	SPEED_RATIO,	 // 平滑比例
	SPEED_POS_KP,	 // 到位響應
	SPEED_SPEED_KP,	 // 追隨響應
	/*區域設定*/
	ZONELIMIT_Zone,	 // 移動最大位置
	ZONELIMIT_X1_MIN,	 // 1.允許下降模內最小位置
	ZONELIMIT_X1_MAX,	 // 2.允許下降模內最大位置
	ZONELIMIT_Y1_MIN,	 // 3.允許橫出最大高度
	ZONELIMIT_Y1_MAX,	 // 4.允許關模最大待機高度
	ZONELIMIT_Z_MIN,	 // 5.允許下降模內最大位置
	ZONELIMIT_Z_MAX,	 // 6.允許置物型外最小位置
	ZONELIMIT_X2_MIN,	 // 7.允許下降模內最小位置
	ZONELIMIT_X2_MAX,	 // 8.允許下降模內最大位置
	ZONELIMIT_Y2_MIN,	 // 9.允許橫出最小高度
	ZONELIMIT_Y2_MAX,	 // 10.允許關模最大待機高度
	ARM_SAFE_DIS,	 // 主副臂安全距離
};

#endif








