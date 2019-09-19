/*===========================================================================+
|  Class    : ViewHand library                                              |
|  Task     : ViewHand library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 	          	                                         |
|  Version  : V1.00                                                          |
|  Creation : 03/17/2014	    		                                     |
|  Revision :       		   					                             |
+===========================================================================*/
#ifndef D__VIEWHAND
#define D__VIEWHAND
#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../utils.h"
#include	"../main.h"


extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C" WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
WORD	SetSpeedToDB(CtmWnd* pWnd);
void	SendCommand(int	CommandID);
void	ChangeImage(CtmWnd* pwnd, char* pszPath);
void		SetEditValue(CtmWnd* pwnd);
void    CreateLEDSet();
void	CreateStringSet();
void	SetManualMode();
#endif
