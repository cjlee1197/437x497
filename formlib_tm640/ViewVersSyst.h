/*===========================================================================+
|  Class    : ViewVersSyst library                                       |
|  Task     : ViewVersSyst library  header file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : J.Wong		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/

#ifndef		D_VIEWVERSSYST_H
#define		D_VIEWVERSSYST_H

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../taskhost.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C" void  	OnDestroyA(CtmWnd* pwndSender);

void	UpdateState();
void  	SetAxisDate(CtmWnd* pwnd,int index);
char*  	ChangeDwordTOChar(DWORD dwValue);
BOOL    GetDeviceUsed(CtmWnd* pwnd);
void	SetAllVisible(CtmWnd** pwnd,int* NodeID,int nCount, BOOL bUpdate=FALSE);
void	SetVisible(CtmWnd* pwnd,BOOL  bVisible, WORD  wType);

#endif
