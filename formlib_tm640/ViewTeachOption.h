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
#ifndef D__VIEWCOOLTIME
#define D__VIEWCOOLTIME
#include	"../formview.h"
#include	"../database.h"
//#include	"../control.h"
#include	"../utils.h"
#include	"../main.h"
#include	"../commonaction.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);

extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);

void 		PromptMsg(char* pszInfo, BOOL nFlag);	//2013-6-5 

#endif
