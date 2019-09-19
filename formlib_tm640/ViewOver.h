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

void		UpdateCoordTable();			//��s�d��y��
void		SetCoordLimitFlag(int Coord,int Value);
void		UpdateJointInfo();			//��s6�b��T
void		SetJointLimitFlag(int Axis,int Value);
void		SetAndShowSpeed(int nType);		//�]�w�t��
void		SetRobotPosition(int nCoordinateType, int nBtnType, int nAxisNum);		//���u����+-���s����
void		UpdateMoveControlType(int nCoordinateType);	//�]�w�ϰ���s���Х� �d��<->���`
//add by mario 2015/3/2 �U�� 02:15:12
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
