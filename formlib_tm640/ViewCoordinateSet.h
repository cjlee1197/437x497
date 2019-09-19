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

// write by Mario 20140917
#ifndef		D__VIEWCOORDINATESET
#define		D__VIEWCOORDINATESET

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"
#include "../selectedit.h"


extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreateA_Work(CtmWnd* pwndSender);
extern  "C"	BOOL	OnCreateA_Tool(CtmWnd* pwndSender);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);


void	SetBaseCoord_DB_ID();
void	SetTalbeCoord_DB_ID();
void	SetPage(CtmWnd* f_pwndSender);
void	SetTableStartNo();
void	SetGroupBGC(int f_GroupNum , BOOL SetOn);
void	SetSelectGroup(int	SelectNum,CtmWnd* f_pwndSender);
double	CnangeMMtoIN(double f_value);		//公轉英 且精度+1
double	CnangeINtoMM(double f_value);		//英轉公 精度-1
void	SetUnit();																						//設定單位							(單位型態(0 = 公制 , 1 = 英制))
void	SetEditEnabled(BOOL f_enabled);

#endif