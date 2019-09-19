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
#ifndef D__VIEWINSTLOGIN
#define D__VIEWINSTLOGIN
#include	"../formview.h"
#include	"../database.h"
#include	"../tmcontrol.h"
#include	"../utils.h"
#include	"../main.h"
#include	"../commonaction.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
#endif
