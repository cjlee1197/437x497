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

//Mario rewrite
#ifndef		D__VIEWGLUECAD
#define		D__VIEWGLUECAD

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);

void FindCADFile();
void	SetSelectEditSTR(int no, char* str);
int CalPath();
char* GetCDFFileName();
int pathpointproduct(double distance_ratio,double theta,char* file);
void Cross_product( double a[],double b[],double c[]);
int CreateProg();
#endif
