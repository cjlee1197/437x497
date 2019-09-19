/*===========================================================================+
|  Class    : ViewClamp library                                              |
|  Task     : ViewClamp library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                         |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/

#ifndef D__VIEWONLINEPROGRAM
#define D__VIEWONLINEPROGRAM
#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"		BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"		WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" 	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"		void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"		void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
void	UpdateText();

#endif
