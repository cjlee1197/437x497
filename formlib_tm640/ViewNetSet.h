/*===========================================================================+
|  Class    : ViewNetSet library                                             |
|  Task     : ViewNetSet library action source file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Caozz		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/


#ifndef D__VIEWNETSET
#define D__VIEWNETSET
#include	"../formview.h"
#include	"../tmconfig.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);

void        CreateSet(CtmWnd* pwndSender);
void        ClearTemp();
void		GetIPAddr();
void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);

void		SetIPInfo();

#endif
