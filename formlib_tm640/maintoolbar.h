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

//	����}�l���ɭ�
extern	"C"	WORD	OnKeyA(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex);
extern	"C"	WORD	OnKeyAEnd(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex);

//void	ChangeImage(CtmWnd* pimage, char* pszpath,WORD FLASH_FLAG,WORD TIMER_NUM);
//void  DelaySec(int DelayNum);
//void		SaveProgram();		//�s�譶��۰ʭ��ɨϥΪ��x�s�ɮ�  20140930 by Mario
//void		WorkBtnChangeCoord(CtmWnd* Btn1 ,CtmWnd* Btn2);		//��m���� 20141009
//void		WorkBtnReset(CtmWnd* BtnStop ,CtmWnd* BtnRun ,CtmWnd* BtnPause);		//��m�٭�20141009