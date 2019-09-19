/*===========================================================================+
|  Class    : ViewSyst library                                               |
|  Task     : ViewSyst library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		          	                                         |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/


#ifndef D__VIEWSYST
#define D__VIEWSYST
#include	"../formview.h"
#include	"../tmconfig.h"

extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void				GetIPAddr();
void        		CreateSet(CtmWnd* pwndSender,BOOL bEnable);
void				SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void				SetEnabled( CtmWnd* pwnd,BOOL  bEnabled);
int					GetRealIPAddr(BYTE *pRealIP, char* pszEth = NULL);
void		SetIPAddr();
#endif
