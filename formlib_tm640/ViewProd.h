/*===========================================================================+
|  Class    : ViewProd library                                               |
|  Task     : ViewProd library  header file                                  |
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


#ifndef D__VIEWPROD
#define D__VIEWPROD
#include	"../formview.h"
#include	"../commonaction.h"



extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void		SetEditValue(CtmWnd* pwnd);

#endif
