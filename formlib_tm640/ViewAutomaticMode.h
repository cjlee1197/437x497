/*============================================================================+
|  Class    : ViewAutomaticMode library                                 			|
|  Task     : ViewAutomaticMode library  header file                       	 	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#ifndef		D__VIEWAUTOMATICMODE
#define		D__VIEWAUTOMATICMODE

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);


void		ShowProgram(int nProgramStartNo);
void		ShowCurrentCoordinate();
void		SetLimitFlag(int Axis,int Value);
void		SetAndShowSpeed(int nType);
void    CodeChange(char *pDst, char *pSrc);
void		ChangeFile();
void		ReturnSet(CtmWnd* pwndSender);
void		inidefine();			//設定回復預設
void		SetUnit();
void		UpdateRunAndCycleTime();
void  	SetProgressSpeed(int Mode,CtmWnd* pwndSender);
void		UpdateFileName(char* filename);
void		UpdateIOStatus();

void	  UpdateBtnSwitchStatus();

#endif
