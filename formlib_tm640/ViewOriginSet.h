/*============================================================================+
|  Class    : ViewOriginSet library                        	  	  	     			|
|  Task     : ViewOriginSet library  header file          	 	  	         	 	|
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
#ifndef		D__VIEWORIGINSET
#define		D__VIEWORIGINSET

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
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);

//void		UpdateBrakeStatus();				//更新剎車狀態
void		UpdateEncoderValue();						//更新編碼器數值

//add  by Mario 2015/3/3 上午 10:53:17
void		SetSendNCDate(int Type, int Joint);
//2015/5/12 下午 04:15:48
void		SetSendNCDate(int Type, int Joint);
void		SetAndShowSpeed(int nType);
void		UpdateMoveControlType(int nCoordinateType);
void		ini(CtmWnd* pwndSender);
void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum);
void		RestJogMoveDate();

#endif
