/*===========================================================================+
|  Class    : View initial Stop machine                                      |
|  Task     : View initial Stop machine                                      |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Leo                                                            |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#ifndef D__VIEWINTSTOP
#define D__VIEWINTSTOP
#include	"../formview.h"
#include	"../database.h"
#include	"../utils.h"
#include	"../tmcontrol.h"

extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void  	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void       	CreateSet(CtmWnd* pwndSender);

#endif
