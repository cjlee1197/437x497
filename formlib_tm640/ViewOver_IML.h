/*===========================================================================+
|  Class    : ViewOver library                                               |
|  Task     : ViewOver library  header file                                  |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   :         	                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                         			           |
|  Revision :           			                                       			   |
+===========================================================================*/

#ifndef D__VIEWOVER
#define D__VIEWOVER
#include	"../formview.h"
#include	"../commonaction.h"
#include	"../main.h"
#include	"../utils.h"

extern  "C"	WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	BOOL	OnCreateA(CtmWnd* pwndSender);
extern  "C"	void	OnShowA(CtmWnd* pwndSender);
extern  "C"	WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey);
extern  "C"	void	OnUpdateA(CtmWnd* pwndSender);
extern	"C"	void	OnDestroyA(CtmWnd* pwndSender);
extern  "C"	WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl);
extern  "C"	WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl);
extern	"C"	void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);

void	SendCommand(int	CommandID);
void	ControlButton(CtmWnd* pwnd, int	nValue);
void 	SetTempColor(CtmWnd* pwnd,FGC fgcTemp,BGC bgcTemp);
void	ControlBackPoint(void);
void	ControlFullAuto(void);
void	ControlHand(void);
void	ControlSingleStep(void);
void	ControlBackPointFinish(void);
void	ControlFullAutoReady(void);
void	ChangeImage(CtmWnd* pwnd, char* pszPath);
void	SetEditValue(CtmWnd* pwnd);
void  CreateLEDSet();
void  CreateDetectLEDSet();
void	CreateStringSet();
void	StepString();
void	ActionStep();
void	ActionNum(WORD wStepNo);
void	ActionName(WORD wStepNo);
void 	SetTempColor(CtmWnd* pwnd,FGC fgcTemp,BGC bgcTemp);
void    ActionAXIS(CtmWnd* pwnd);
void    ActionVALVE(CtmWnd* pwnd);
void    ActionPILE(CtmWnd* pwnd);
void    ActionWAIT(CtmWnd* pwnd);
void    ActionSIGN(CtmWnd* pwnd);
void    ActionASSIT(CtmWnd* pwnd);
void    ActionJUMP(CtmWnd* pwnd);
static      inline	void	TouchButtonEvent(CtmWnd* pwnd, int nPosX, int nPosY);

void    ShowValue(CtmWnd* pwnd); // 顯示元件 pwnd 輸入數值	

#endif
