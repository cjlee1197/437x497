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
#ifndef D__MESSAGEBOXEDITTEACHPOINT
#define D__MESSAGEBOXEDITTEACHPOINT

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void		OnUpdateA(CtmWnd* pwndSender);

void	GetTableValue(int Mode);
void	GetTeachPointValue();
void	SetUnit();
void	SetText();
void TeachPointUpdate(int f_OprMode ,int f_TeachPointNo ,LONG_AXIS * f_TeachPointVlaue ,int f_TeachPointPos);	//沒用到就填0或NULL
#endif
