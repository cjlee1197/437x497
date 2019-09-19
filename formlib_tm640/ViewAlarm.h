/*===========================================================================+
|  Class    : ViewAlarm library                                               |
|  Task     : ViewAlarm library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                           |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                                 |
|  Revision :       		   					                                         |
+===========================================================================*/

#ifndef D__VIEWALARM
#define D__VIEWALARM
#include	"../formview.h"
#include	"../tasksiem.h"
#include    "../qualitycontrol.h"
#include	"../curvedata.h"

extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void  OnUpdateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
//extern  "C"	void	OnDestroyA(CtmWnd* pwndSender);

void      CreateSet(CtmWnd* pwndSender);
void      CreateSetMul(CtmWnd* pwndSender);
//void	  AddChangeLine(CtmWnd* pwndSender,WORD wCol, char* pcStr);
//void	  DrawTableAlarm(CtmWnd* pwndSender); 

#endif
