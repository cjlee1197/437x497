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

#ifndef D__VIEWINPUTFORM
#define D__VIEWINPUTFORM
#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../utils.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnShowA(CtmWnd* pwnd);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);

void		ExitInputForm();
void	SendCommand(int	CommandID);

#endif
