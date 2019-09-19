/*============================================================================+
|  Class    : ViewCodeEdit library                                    	 			|
|  Task     : ViewCodeEdit library  header file                       	 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	| 
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#ifndef		D__VIEWCODEEDIT
#define		D__VIEWCODEEDIT

#include	"../formview.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../libCNCComm.h"
#include	"../_kb.h"
#include	"../utils.h"
#include	"../main.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);


void 		ShowProgram(int nProgramStartNo ,BOOL mode);	//20141007  Mario 加入mode來分成開啟目標以及開啟暫存檔案
void 		SaveProgram(BOOL mode);		//20141007	Mario 加入mode來分成儲存檔案以及暫存檔案
void		TeachPointCheck(WORD wStartNo);
void		UpdateTable(WORD wStartNo);
void    CodeChange(char *pDst, char *pSrc);
void		GetNewStartNo();
void		NoUseTablePart(WORD wPartNo);
void		AddNewCommand();
void		ModifyCommand();


void		ChangeFile(CtmWnd* pwndSender);		//如有從檔案頁直接開啟  則更新開啟檔案
//20140923↑
BOOL		FileCmy();													//該檔若與暫存檔相同則回傳TRUE
//BOOL file_exists (char * fileName);					//判斷檔案是否存在 1:存在 0:不存在
//20141007
void  SetCommandMode(int Mode);										//設定顯示的指令類型
void	CommandListShow	();									//指令列表
void	SetListUpDown();										//上下按鈕
//20141023
//char* GetCommandStr(int mode , int Num);
void	CallCommandWork(int mode , int temp);		//呼叫指令工作函式或指令


//keyborad相關控制
void	ChangeNum2Sign(CtmWnd* m_pwndSender, int m_KeyMode);
void	OnKeyBorad();
void	OffKeyBorad();


//2015/6/15 下午 03:02:11
int		CallModifyCommand(char* f_Command);
void 	SetFileInfo(char* Str);

#endif
