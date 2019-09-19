/*============================================================================+
|  Class    : ViewMessageBoxMoveCommand library                		            |
|  Task     : ViewMessageBoxMoveCommand library  header file									|
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

//Mario rewrite		20141021
#ifndef D__MESSAGEBOXMOVECOMMAND
#define D__MESSAGEBOXMOVECOMMAND

#include	"../formview.h"

extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void		OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl); 
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);


void		AddCommand(char* pszCommand);
void		SetSpeedAndNearPlace(BOOL Mode ,BOOL Type);
void		SetSpeedAndAccel(BOOL Mode);
void		ClearValue(int NO);
void		SetTable();
void		ClearEdit();
void		ShowValue(int f_DefineMode);
void		SaveInputValue();		//儲存Input元件內的值
void		SetCheckBox();
void		SetMoveMode(BOOL f_mode);
void 		Initialize();				//初始化
void 		SetCoordMode();
void		SetSpeedValue();			//設定速度的初始值
void		SortOutCommand();
void		ClearCoordZeor(char *Coord,int Num);			//整理座標 去除小數點後多餘0
BOOL		RevertMoveCommand(char *Command);
void		UpdateSpeed();

void		SetBtnType();

#endif
