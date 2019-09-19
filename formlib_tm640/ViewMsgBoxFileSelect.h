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
#ifndef D__MESSAGEBOXFILESELECT
#define D__MESSAGEBOXFILESELECT

#include	"../formview.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void		OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);


void    CodeChange(char *pDst, char *pSrc);		//���U�Ψ禡�A��r�`��Ƭ����r�`�B�z

#endif
