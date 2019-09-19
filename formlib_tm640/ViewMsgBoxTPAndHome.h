/*===========================================================================+
|  Class    : statusbar library                                              |
|  Task     : statusbar library  header file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		          	                                           |
|  Version  : V1.00                                                          |
|  Creation : 			    		                                                 |
|  Revision :       		   					                                         |
+===========================================================================*/
#ifndef D__MESSAGEBOXTPANDHOME
#define D__MESSAGEBOXTPANDHOME

#include	"../formview.h"

extern	"C"	BOOL		OnCreateA_GotoTP(CtmWnd* pwndSender);
extern	"C"	BOOL		OnCreateA_ReturnHmoe(CtmWnd* pwndSender);
extern	"C"	BOOL		OnCreateA(CtmWnd* pwndSender);
extern	"C"	WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern	"C"	WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void		OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void		OnDestroyA(CtmWnd* pwndSender);

void	SetViewStatus(int f_RunStatus);		//移動時要關閉某些功能
void	SetCycleBtnImage(int mode);				// 0stop 1run 2pause
void	ReadEditNum();
void		SetAndShowSpeed(int nType);		//設定機械手臂的移動速度
void	SetAlarmStatic(int mode);
#endif
