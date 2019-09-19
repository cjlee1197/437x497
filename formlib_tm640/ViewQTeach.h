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

typedef struct ACTION_P{     //�w�q
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
void  UpdateSubArm(); // ��s ���u��� ���s��
void	UpdateBtnDetect_Select(); // ��s �˴���ܫ���
void	Update_ClampStr(); // ��s �Q��ܪv�� ��r����
void	UpdateBtnTeachStart();
void	UpdateBtnNextP();
void	UpdateTeach_Pos();
void	UpdateTeach_Speed();
void	UpdateHint();
void	UpdateCheck_Pile();
void	Update_SLVL();
void	Update_AxisMoveNow(int Axis); // ��s �{�b���ʶb	

void	SaveAction2Temp(int QTeach_PGNo,int j);
void	SaveAct_Axis2Temp(int QTeach_PGNo,int j);
void	SaveAct_Valve2Temp(int QTeach_PGNo,int j);
void	SaveAct_Detect2Temp(int QTeach_PGNo,int j);
void	SaveAct_Pile2Temp(int QTeach_PGNo,int j);
void	UnLockBtn(CtmWnd* pwnd);
void	LockBtn(CtmWnd* pwnd);
void	SetFromWaitP(int WhichAxis);
void	Update_PosHint(int Axis); // ��s ��m����
void	Update_Check_Move(); // ��s ���ʫ�U�� �]�w����	
void	GetDownDis(); // �p�� �U���Z��
void	GetBackDis(); // �p�� ��h�Z��
void	SaveSLVLData(); // �����e �X�� �]�w�ƭ�	�s�JDB	
void	ShowData(CtmWnd* pwnd);

#endif
