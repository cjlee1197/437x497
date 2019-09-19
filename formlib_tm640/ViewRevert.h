/*===========================================================================+
|  Class    : ViewReset library                                              |
|  Task     : ViewReset library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#ifndef D__VIEWREVERT
#define D__VIEWREVERT
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../main.h"
#include	"../moldControl.h"


extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);    //James 2007-06-12 add
extern	"C" WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void  	SetPanelGroup(CtmWnd*	pwndTemp);
void    ResetMMI();
void    ResetDSP();
void		BackUpParam();
void		ReParam(char* ParamRevertPath);
void		BackUpDatabase();
BOOL 		file_exists (char * fileName);

#endif
