/*===========================================================================+
|  Class    : ViewTouchScreenAdjust  library                                 |
|  Task     : ViewTouchScreenAdjust  library header file                     |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 01/25/2010                                                  	 |
|  Revision :                                                                |
+===========================================================================*/

#ifndef D__VIEWTOUChSCTEENADJUST
#define D__VIEWTOUChSCTEENADJUST

#include	"../formview.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);

#endif
