/*===========================================================================+
|  Class    : View set Stop machine                                          |
|  Task     : View set Stop machine                                          |
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
#ifndef D__VIEWSETINST
#define D__VIEWSETINST
#include	"../formview.h"
#include	"../database.h"
#include	"../tmcontrol.h"
#include	"../utils.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void  	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void       	CreateSet(CtmWnd* pwndSender);
void		SetDisabledControl();
void		SetDisabledControlOther();

#endif
