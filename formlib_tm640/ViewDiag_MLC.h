/*===========================================================================+
|  Class    : ViewDiag_MLC_I library                                         |
|  Task     : ViewDiag_MLC_I library  header file                            |
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

#ifndef D__VIEWMLCI
#define D__VIEWMLCI
#include	"../formview.h"

extern  "C"	BOOL	OnCreate_MLC_I(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_O(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_C(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_S(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_A(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_Timer(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_Cnt(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_Reg(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreate_MLC_Dreg(CtmWnd* pwndSender);

extern	"C"	WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);

BOOL	CreatePublic(CtmWnd* pwndSender);
void    UpdateSelectTable(WORD	wStartNo);
void    UpdateStaticTable(WORD	wStartNo);
void    CodeChange(char *pDst, char *pSrc);
#endif
