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
	int Axis[5][5];			// 軸參數
	int Speed[5][7];		// 速度參數
	int ZoneLimit[6][3];// 區域設定參數
	int Mode[3];				// 模式選擇
	int Detect[6];			// 檢測選擇
	int Delay[4];				// 延時選擇
	int Prod[2];				// 生產參數
	int	Pile[4][6];			// 堆疊參數
}tmParam;



#endif








