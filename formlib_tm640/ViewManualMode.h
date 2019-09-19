/*============================================================================+
|  Class    : ViewManualMode library                        	    	     			|
|  Task     : ViewManualMode library  header file           	  	         	 	|
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
#ifndef		D__VIEWMANUALMODE
#define		D__VIEWMANUALMODE

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);  //�����!!!!!  �ΤFselectedit�|GG
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);

//=====��e������u����m=====
void		ShowCurrentCoordinate();
void		SetLimitFlag(int Axis,int Value);

//=====�ޱ�������u=====
void		SetAndShowSpeed(int nType);
void		SetRobotPosition(int nCoordinateType, int nBtnType, int nAxisNum);
void		UpdateMoveControlType(int nCoordinateType);

//=====�о��I�B�z=====
void		UpdateTeachPointTable();		//20140916�[�J���X�Ѽ�;
void		NonTeachPoint(int nStartNo);
void		SetUnit(int UnitType);			//20140915		by Mario
void 		SwitchJogMode();							

//=====�´���=====
void		InitialDBvalue();

//add by mario 2015/3/2 �U�� 02:15:12
void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum);
void		RestJogMoveDate();
void		ini(CtmWnd* pwndSender);
void 		SetBtn_JogMove(BOOL f_JogLock);

void 	TeachPointUpdate(int f_OprMode ,int f_TeachPointNo ,LONG_AXIS * f_TeachPointVlaue ,int f_TeachPointPos);
void	SetJogSelectEditMode();
void	SelectOper(int Oper);
void	SetTPAndHomeRun(BOOL	RunStatus);
void	SetCycleBtnImage();
void	SetAlarmStatic(int mode);
void	SetSelTPColor();
#endif
