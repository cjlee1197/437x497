/*============================================================================+
|  Class    : ViewMessageBoxConfirm library                   		            |
|  Task     : ViewMessageBoxConfirm library  header file											|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		          	                                           	|
|  Version  : V1.00                                                          	|
|  Creation : 			    		                                                 	|
|  Revision :       		   					                                         	|
+============================================================================*/

//Mario rewrite
#ifndef D__MESSAGEBOXMOVEPALLET
#define D__MESSAGEBOXMOVEPALLET

#include	"../formview.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C" WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void		OnUpdateA(CtmWnd* pwndSender);


void		AddCommand(char* pszCommand);
void		SetSpeedAndNearPlace(BOOL Mode ,BOOL Type);
void		SetMoveMode();
void		SetSpeedValue();
char*		ClearCoordZeor(char *Coord,int Num);
BOOL		RevertMoveCommand(char *Command);
void		UpdateSpeed();
#endif
