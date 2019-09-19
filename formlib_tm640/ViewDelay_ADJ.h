/*===========================================================================+
|  Class    : ViewVersion library                                         	 |
|  Task     : ViewVersion library  header file                            	 |
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

#ifndef D__VIEWVERSION
#define D__VIEWVERSION
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../moldControl.h"
#include	"../lib/libComm.h"

typedef struct DT_ADJ{     //©w¸q
     int Step;
     int Type;
     int Num;
     int P1;
     int P2;
     int P3;
     int P4;
     int P5;
}DT_ADJ;

enum
{
	Axis_X1 = 1,
	Axis_Y1 = 2,
	Axis_Z = 3,
	Axis_X2 = 4,
	Axis_Y2 = 5,
};

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);
extern	"C" WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void	Get_DT_DB();

#endif
