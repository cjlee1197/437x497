/*===========================================================================+
|  Class    : maintoolbar                                                    |
|  Task     : Main Toolbar                                                   |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                           |
|  Version  : V1.00                                                          |
|  Creation : 12/23/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	"../formview.h"
#include	"../toolbar.h"
#include	"../utils.h"

//	按鍵開始的時候
extern	"C"	WORD	OnKeyA(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex);
extern	"C"	WORD	OnKeyAEnd(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex);

//void	ChangeImage(CtmWnd* pimage, char* pszpath,WORD FLASH_FLAG,WORD TIMER_NUM);
//void  DelaySec(int DelayNum);
//void		SaveProgram();		//編輯頁到自動頁時使用的儲存檔案  20140930 by Mario
//void		WorkBtnChangeCoord(CtmWnd* Btn1 ,CtmWnd* Btn2);		//位置互換 20141009
//void		WorkBtnReset(CtmWnd* BtnStop ,CtmWnd* BtnRun ,CtmWnd* BtnPause);		//位置還原20141009