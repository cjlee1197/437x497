/*===========================================================================+
|  Class    : ViewHalfTest library                                           |
|  Task     : ViewHalfTest library  header file                              |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   :         	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                         			           |
|  Revision :           			                                       			   |
+===========================================================================*/

#ifndef D__VIEWGUIDESET
#define D__VIEWGUIDESET
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../main.h"
#include	"../utils.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
typedef struct tagSTEP
{
	WORD	wStep;
	WORD	wType;
	WORD	wNum;
	DWORD	dwParam1;
	DWORD	dwParam2;
}STEP;

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
void CreateRecipe(void);
void	SetEditValue(CtmWnd* pwnd);
void	SendCommand(int	CommandID);
void	LCD_Light_Test(int Bglight);
void	Show_LCD_Light_Result(int Bglight);
void	LCD_Color_Test(int Color);
void	ShowItem();
void	Show_LCD_Color_Result(int Color);
void TestDRAM();
#endif
