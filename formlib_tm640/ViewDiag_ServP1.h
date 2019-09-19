/*===========================================================================+
|  Class    : ViewDiag_ServP library                                         |
|  Task     : ViewDiag_ServP library  header file                            |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny	          	                                             |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/

#ifndef D__VIEWSERVP
#define D__VIEWSERVP
#include	"../formview.h"

extern  "C"	BOOL	OnCreateServP(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreateTuneP(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreateMakerP(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreateUserP(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreateCommonP(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	BOOL	CreatePublic(CtmWnd* pwndSender);
extern	"C"	void	OnUpdateA(CtmWnd*	pwndSender);

void    UpdateServPTable(WORD	wStartNo);
void    CodeChange(char *pDst, char *pSrc);
#endif
