/*===========================================================================+
|  Class    : ViewMldD library                                               |
|  Task     : ViewMldD library  header file                                  |
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
#ifndef 	D__VIEWMLDD
#define 	D__VIEWMLDD
#include	"../formview.h"
#include	"../moldControl.h"
#include	"../storage.h"

extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);		//2009-4-15, add

void  	CreateSet(CtmWnd* pwndSender);
void  	SetSource(CtmWnd* pwndSender);
void  	UpdateSource(CtmWnd* pwndSender);
void  	UpdateSourceBar(CtmWnd* pwndSender);
void	DeleteMold(int nSourceMedia);
void	SetDeletNoMax();
void	GetMoldCount();
STROAGETYPE		TOStorageType(int type);

void PageUp(CtmWnd* pwndSender);
void PageDown(CtmWnd* pwndSender);

#endif
