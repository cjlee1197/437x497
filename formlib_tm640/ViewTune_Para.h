/*===========================================================================+
|  Class    : ViewTune_Para library                                          |
|  Task     : ViewTune_Para library  header file                             |
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

#ifndef D__VIEWTUNEP
#define D__VIEWTUNEP
#include	"../formview.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);

void    UpdateTunePTable();
void    CodeChange(char *pDst, char *pSrc);
#endif
