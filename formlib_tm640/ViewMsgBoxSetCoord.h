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

//Mario write

#ifndef D__VIEWMSGBOXSETCOORD
#define D__VIEWMSGBOXSETCOORD

#include	"../formview.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl); 
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void		AddCommand(char* pszCommand);
void 		SetUnit();
void SetEditFgc();
BOOL	InputOk();
void		ClearCoordZeor(char *Coord,int Num);					//整理座標 去除小數點後多餘0
BOOL		RevertOldCommand(char *Command);
#endif
