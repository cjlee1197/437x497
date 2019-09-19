/*===========================================================================+
|  Class    : ViewInternal_IO library                                        |
|  Task     : ViewInternal_IO library action source file                     |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   :                                   										         |
|  Version  : V1.00                                   			                 |
|  Creation : 			                                     		                 |
|  Revision :           			                             			             |
+===========================================================================*/

#ifndef D__VIEWINTERNAL_IO
#define D__VIEWINTERNAL_IO
#include	"../formview.h"
#include	"../tasksiem.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void  	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);

void    CreateLEDSet();
void		CreateStringSet();
void		CreatUpdat();
void		InitChangeV();
void		CheckChangeIO(WORD**	wTemp, int	nIndex);
void		CheckColor(int	nTemp, int	nIndex);
void		ReqIOState(void);
#endif
