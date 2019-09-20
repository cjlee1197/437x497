/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : MainFrm                                                        |
|  Task     : Main Frame header file                                         |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_MAINFRM
#define     D_MAINFRM

#include	"toolbar.h"
#include	"statbar.h"
#include	"view.h"
#include	"wnd.h"
#include	"wndx2.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/


/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - MainFrame                                    |
+===========================================================================*/
class CtmMainFrame : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmMainFrame)
public:
    CtmMainFrame();
    ~CtmMainFrame();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    CtmToolBar*   	m_pwndTool;
    CtmSubToolBar* 	m_pwndSubTool;
    CtmView*       	m_pwndView;		//	fans add
    CtmView*       	m_pwndView2;	//	James add 2015/11/19
    CtmStatusBar*	m_pwndStatus;	//	fans add
	CtmPromptBar*  	m_pwndPrompt;	//	fans add
	CtmWnd*			m_pwndBmpTitle;
	char*			m_pszToolBar;	//	fans add
	ImageBackground	m_BackImage;	//	fans add
	int				m_nTitlePos;	//	fans add: position for the title
	RECT			m_RectPriv;		
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    BOOL    Create(tmWND* pwnd);
	
    void    Update();
    void    Redraw(RECT rect);
    void    OnGetFocus();
    void    OnLoseFocus();
    WORD    OnKey(WORD wKey);
    WORD    OnCommand(WORD wCmd);
    WORD	OnCommand(char* pszCmd);	//	tong add
    WORD    OnChange(WORD wIDControl);
    void	UpdateMainFrom();
    RECT	GetViewRect();
    BOOL	MouseUp(short x_pos,short y_pos,int key_flag);			//James addd 2010/3/16 
	BOOL	MouseDown(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16 
    
     //	fans add
	BOOL 			CreateA();
	CtmView* 		GetCurrentView() {return m_pwndView;};
	CtmView* 		GetCurrentView2() {return m_pwndView2;};
	void 			SetPropertys();
	void 			SetPropertys_Nub();
	CtmStatusBar*	GetStatusBar()	{return m_pwndStatus;};
	CtmPromptBar*	GetPromptBar()	{return m_pwndPrompt;};
	CtmToolBar*		GetCurrentToolBar() {return m_pwndTool;};
	CtmSubToolBar*	GetSubToolBar()	{return m_pwndSubTool;};
	void			WndProc(int message, WPARAM wParam, LPARAM lParam);	//	fans add 2008/6/16 11:48¤W¤È

	//void    SetTitle();
	void    SetTitle(char* pszTitle);
	void    SetTitle2(char* pszTitle);
	void    SetUserID(char* pszTitle);
	
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	
};

/*===========================================================================+
|           External                                                         |
+===========================================================================*/


#endif
