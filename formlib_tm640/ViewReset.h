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
#ifndef D__VIEWRESET
#define D__VIEWRESET
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../main.h"
//#include	"../taskdsp.h"
#include	"../moldControl.h"


extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);    //James 2007-06-12 add
extern	"C" WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void		SetEnabled( CtmWnd* pwnd,BOOL  bEnabled);	
void		SetChecked( CtmWnd* pwnd,BOOL  bVisible);	
void		SaveShotCount();
void		SaveActTime();
/*void		SaveDefaultUser();*/
void	    SaveStringData(); /*ZHBA 2015/9/18 Replace SaveDefaultUser()*/ 
void		SaveEnergyData();
void		Save2PlatenPara();
void		SaveFuncConf();

void    	ResetMMI();
void    	ResetDSP();
void		SaveAutoRunTime();
void		SaveAS2PLATADJPOSI();
void		SavePushKeyOilData();
void		SaveTieBarDate();

#endif

