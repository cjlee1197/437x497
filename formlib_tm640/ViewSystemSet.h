/*===========================================================================+
|  Class    : ViewSyst library                                               |
|  Task     : ViewSyst library action source file                            |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   :			                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#ifndef D__VIEWSYSTEMSET
#define D__VIEWSYSTEMSET
#include	"../formview.h"
#include	"../tmconfig.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);

extern  "C"	void	GetTime();
#endif