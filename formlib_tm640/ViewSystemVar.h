/*============================================================================+
|  Class    : ViewManualMode library                        	    	     			|
|  Task     : ViewManualMode library  header file           	  	         	 	|
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

//Mario rewrite	2014/09/18

#ifndef		D__VIEWSYSTEMVAR
#define		D__VIEWSYSTEMVAR

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);

void    CodeChange(char *pDst, char *pSrc);
void		UpdateMacroData(CtmWnd* pwndSender,int f_StartNO);
void		SetLocalMacroDB_NULL();
void		UpdateShareMemoryValue();
void		UpdateShareMemoryStatic();
int 		Pow_Mario(int a , int b);
void		SetButtonBgc(CtmWnd* f_pwndButton);

#endif