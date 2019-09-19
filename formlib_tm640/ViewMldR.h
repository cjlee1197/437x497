/*===========================================================================+
|  Class    : ViewMldR library                                               |
|  Task     : ViewMldR library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans	          	                                         	 |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/
#ifndef 	D__VIEWMLDR
#define 	D__VIEWMLDR
#include	"../formview.h"
#include	"../moldControl.h"
#include	"../storage.h"

extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void  	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C" void	OnUpdateB(CtmWnd* pwndSender, void *pData);//zholy070416
extern	"C" void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);

void  	CreateSet(CtmWnd* pwndSender);
void  	SetSource(CtmWnd* pwndSender);
void  	UpdateSource(CtmWnd* pwndSender);
void  	UpdateSourceBar(CtmWnd* pwndSender);
void	SetServiceView();
void	ReadMold(int nSourceMedia);
void	GetNetMoldList();
void	GetMoldCount();
void	SetReadNoMax();
STROAGETYPE		TOStorageType(int type);

void PageUp(CtmWnd* pwndSender);
void PageDown(CtmWnd* pwndSender);
void	SetTempValueFahr() ;
void	SetTempValueCels();
void	ReadMldCrlData();

#endif
