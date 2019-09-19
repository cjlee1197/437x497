/*===========================================================================+
|  Class    : ViewMldS library                                               |
|  Task     : ViewMldS library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                         |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                         |
|  Revision :       		   					                             |
+===========================================================================*/
#ifndef D__VIEWMLDS
#define D__VIEWMLDS
#include	"../formview.h"
#include	"../moldControl.h"
#include	"../storage.h"

// Down 4 lines for sort only
#define		MAX_MODL_CNT		200
#define 	MAX_MODL_PER_PAGE 	20
#define 	SORT_BY_NAME 		1
#define 	SORT_BY_DATE 		0

extern  "C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void  	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void  	OnUpdateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C" void	OnUpdateB(CtmWnd* pwndSender, void *pData);//zholy070416
extern	"C" void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);

void  		CreateSet(CtmWnd* pwndSender);
void  		SetDestination(CtmWnd* pwndSender);
void  		UpdateDestination(CtmWnd* pwndSender);
void  		UpdateSourceBar(CtmWnd* pwndSender);
void  		UpdateDestinationBar(CtmWnd* pwndSender);
void  		SaveMold();
void		SaveCurrentMold();
void		SetServiceView();
STROAGETYPE		TOStorageType(int type);
void		OnUpdateB(CtmWnd* pwndSender, void *pData);
void		GetNetMoldList(int StartNum);

//Down 2 lines for sort only
WORD 		LoadData(STROAGETYPE m_nDestiMedia,WORD SORT_TYPE);
//	fans add 2009/5/14 10:39¤W¤È
void		GetMoldCount();

void PageUp(CtmWnd* pwndSender);
void PageDown(CtmWnd* pwndSender);

int		WriteMoldSetIni(char*	pszPath);
int		CheckFileExist(char*	pszName);
int		CopyFile(char*	pszSrc,	char*	pszDst);

void 	PageUp(CtmWnd* pwndSender);
void 	PageDown(CtmWnd* pwndSender);

void PageUp(CtmWnd* pwndSender);//2013-5-22,wangli
void PageDown(CtmWnd* pwndSender);
void	ReadMldCrlData();
#endif
