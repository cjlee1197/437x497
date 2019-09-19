/*===========================================================================+
|  Class    : View install login machine                                     |
|  Task     : View install login machine                                     |
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
#ifndef D__VIEWQTEACH
#define D__VIEWQTEACH
#include	"../formview.h"
#include	"../database.h"
//#include	"../control.h"
#include	"../utils.h"
#include	"../main.h"
#include	"../commonaction.h"

typedef struct ACTION_P{     //定義
     int QTP; // QTeach Point
     int Step;
     int Type;
     int Num;
     int P1;
     int P2;
     int P3;
     int P4;
     int P5;
}ACTION_P;

enum
{
	Axis_X1 = 1,
	Axis_Y1 = 2,
	Axis_Z = 3,
	Axis_X2 = 4,
	Axis_Y2 = 5,
};

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);

void 		PromptMsg(char* pszInfo, BOOL nFlag);	//2013-6-5 

void	SaveMode();
void	Save();
void	Clean_QTeach();
void	Save2PG();
void	CleanPG();
void	GetPosNow(int GetAxisPos);
void	UpdateDataValue();
void	UpdateImg();
void	UpdateShow();
void	UpdateClamp();
void  UpdateSubArm(); // 更新 副臂選用 按鈕們
void	UpdateBtnDetect_Select(); // 更新 檢測選擇按紐
void	Update_ClampStr(); // 更新 被選擇治具 文字說明
void	UpdateBtnTeachStart();
void	UpdateBtnNextP();
void	UpdateTeach_Pos();
void	UpdateTeach_Speed();
void	UpdateHint();
void	UpdateCheck_Pile();
void	Update_SLVL();
void	Update_AxisMoveNow(int Axis); // 更新 現在移動軸	

void	SaveAction2Temp(int QTeach_PGNo,int j);
void	SaveAct_Axis2Temp(int QTeach_PGNo,int j);
void	SaveAct_Valve2Temp(int QTeach_PGNo,int j);
void	SaveAct_Detect2Temp(int QTeach_PGNo,int j);
void	SaveAct_Pile2Temp(int QTeach_PGNo,int j);
void	UnLockBtn(CtmWnd* pwnd);
void	LockBtn(CtmWnd* pwnd);
void	SetFromWaitP(int WhichAxis);
void	Update_PosHint(int Axis); // 更新 位置提示
void	Update_Check_Move(); // 更新 移動後下降 設定按鍵	
void	GetDownDis(); // 計算 下降距離
void	GetBackDis(); // 計算 後退距離
void	SaveSLVLData(); // 獲取當前 柔性 設定數值	存入DB	
void	ShowData(CtmWnd* pwnd);

#endif
