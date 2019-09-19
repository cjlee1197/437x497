/*===========================================================================+
|  Class    : statusbar library                                              |
|  Task     : statusbar library  header file                                 |
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
#ifndef D__STATUSBAR0
#define D__STATUSBAR0

#include	"../formview.h"
#include	"../utils.h"
#include	"../database.h"

extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern	"C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);

void	ChangeImage(CtmWnd* pimage, char* pszpath,WORD FLASH_FLAG,WORD TIMER_NUM);
void	Init_Common_User();
void	Double_byte_String_Merger(char* out, char* str1 , char* str2);
void	SetGpioLed(BOOL status,int bit);
void	SetEditValue(CtmWnd* pwnd);
void	SendCommand(int	CommandID);

#endif
