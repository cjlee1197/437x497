/*===========================================================================+
|  Class    : ViewClmpP2 library                                               |
|  Task     : ViewClmpP2 library  header file                                  |
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


#ifndef D__VIEWPARA1
#define D__VIEWPARA1
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../database.h"


extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void    OnUpdateA(CtmWnd* pwndSender);

// void	SendDSP28Data(CtmWnd * pwnd);  //ZHBA  2008-9-18
void	GetDWordValve(CtmWnd * pwnd);  //ZHBA  2009-4-24
void	SendDWordValve(CtmWnd * pwnd);  //ZHBA  2009-4-24
void	SetDisabledControl();
void	SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void	GetACTTime(CtmWnd* pwnd);
void	SendDSP28Data(CtmWnd * pwnd);
void  	SetSegment();
#endif
