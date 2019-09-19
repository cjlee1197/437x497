/*===========================================================================+
|  Class    : statusbar library                                              |
|  Task     : statusbar library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                           |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                                 |
|  Revision :       		   					                                         |
+===========================================================================*/
#ifndef D__MESSAGEBOX
#define D__MESSAGEBOX

#include	"../formview.h"
extern  "C"	void	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C" WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);

void	GetChecked(BOOL bFalg);
void    SetNULLSelect();
void    SetMotorSelect();
void	SetWaitSelect();
void	SetPermitSelect();
void	SetValveSelect();
void	SetCheckSelect();
void	SetWriteFlagSelect();
void	SetReadFlagSelect();
void	SetLableSelect();
void	SetJumpSelect();
void  	SetDelaySelect();
void	DealControl();
void    AxisLimit();
void    ShowValue(WORD wInsert);
void	SetStepIndex(CtmWnd* pwnd,WORD wStep);
void	SetVisible(CtmWnd* pwnd, BOOL  bVisible, WORD  wType);
#endif
