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

//Mario write

#ifndef D__VIEWMSGBOXUSERLOGIN
#define D__VIEWMSGBOXUSERLOGIN

#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../utils.h"
#include	"../main.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void		OnUpdateA(CtmWnd* pwndSender);

void	Save();
void	SetVisible(CtmWnd* pwnd, BOOL  bVisible, WORD  wType);
void	SetEnabled(CtmWnd* pwnd, BOOL  bVisible);
void	UpdateText();

#endif
