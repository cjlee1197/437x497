/*===========================================================================+
|  Class    : ViewEjet library                                               |
|  Task     : ViewEjet library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		          	                                         |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/

#ifndef D__VIEWPOTHR
#define D__VIEWPOTHR
#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include 	"../utils.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);

void	UpdateDataValue();
void	GetPosNow(int GetAxisPos);

void	UpdateBtnNextP();
void	UpdateTeach_Pos();
void	Update_PosHint(int Axis); // 更新 位置提示

#endif
