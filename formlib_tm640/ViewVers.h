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


extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);
extern	"C" WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey);

void		SetDBRM(tmDB_DIAGRM* db_diagrm, CtmWnd* pwnd[], int nCount);

void       	CreateSet(CtmWnd* pwndSender);
void		ShowTransmitCom(CtmWnd* pwndSender);
void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType);
void   		ReadDingDanFrom();

char*		DW2Str_ASCII(DWORD dw_dbvalue); //cjlee 2019/5/12 下午 09:51:01

void	GetValueFrom28(); // 取得28設定值
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect); // 設定選擇DB數值

typedef struct tagParam
{
	int iMechType;  		// 機型選擇 機型&編碼器
	int iTransType; 		// 傳動方式
	int iDouble; 				// 倍數機構 
	int iPosInv; 				// 位置反向
	
}tmParam;

#endif
