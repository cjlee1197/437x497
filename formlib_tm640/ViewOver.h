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
#ifndef		D__VIEWOVER
#define		D__VIEWOVER

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);

void		UpdateCoordTable();			//更新卡氏座標
void		SetCoordLimitFlag(int Coord,int Value);
void		UpdateJointInfo();			//更新6軸資訊
void		SetJointLimitFlag(int Axis,int Value);
void		SetAndShowSpeed(int nType);		//設定速度
void		SetRobotPosition(int nCoordinateType, int nBtnType, int nAxisNum);		//手臂移動+-按鈕控制
void		UpdateMoveControlType(int nCoordinateType);	//設定區域按鈕的標示 卡式<->關節
//add by mario 2015/3/2 下午 02:15:12
void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum);
void		RestJogMoveDate();
void		ini(CtmWnd* pwndSender);
void 		SetBtn_JogMove(BOOL f_JogLock);
void		BackUpParam();
void		ReParam();
void		SetAndShowSpeed(int nType);
void		SetUnit();
void		UpdateRunAndCycleTime();
void		SetJogSelectEditMode();


#endif
