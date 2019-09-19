/*===========================================================================+
|  Class    : ViewClamp library                                              |
|  Task     : ViewClamp library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                 		       |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                    				     |
|  Revision :       		   					                            						 |
+===========================================================================*/

#ifndef D__VIEWONLINEPROGRAM
#define D__VIEWONLINEPROGRAM
#include	"../formview.h"
#include	"../wnd.h"
#include	"../commonaction.h"
#include	"../database.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"		BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"		WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C" 	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"		void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"		void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void	PageDown(CtmWnd* pwndSender);
void	PageNext(CtmWnd* pwndSender);
void	PageUp(CtmWnd* pwndSender);
void	GetPickerDB(int SelectNo);
void	SetPickerDB(int SelectNo);
void	SetColour(CtmWnd* pwndSender);
void	Insert(int SelectNo);
void	Delete(int SelectNo);
void	Search(int SearchNo);
void	Sync(int SelectNo);
void	UnSync(int SelectNo);
void	UpdateNo();
void	Clear();
void	Clean();
void	Download();
void	GetPosTag();
void	UpdateText();
void	UpdatePara(int Type);
void	Single(CtmWnd* pwndSender);
void	SetVisible(CtmWnd* pwnd, BOOL  bVisible, WORD  wType);
void	SetEnabled(CtmWnd* pwnd, BOOL  bVisible,int taborder);
void	Update_Download_Hint();

#endif
