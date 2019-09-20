/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : ToolBar                                                        |
|  Task     : Tool Bar header file                                           |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 08/05/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TOOLBAR
#define     D_TOOLBAR

#include	"wnd.h"
#include	"button.h"


/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

#define     tmMAX_TOOLITEM        16
#define     tmMAX_TOOLNEST        8

#define		TOOLBAR_VIEW			0x0000
#define		TOOLBAR_NEXT			0x0100
#define		TOOLBAR_SELECT			0x0200
#define		TOOLBAR_RETURN			0x0300
#define		TOOLBAR_RETURNHOME		0x0400
#define		TOOLBAR_LOGIN			0x0500
#define		TOOLBAR_RESET			0x0600
#define		TOOLBAR_DUPL			0x0700
#define		TOOLBAR_SUBTOOLBAR		0x0800

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmTOOLITEM
{
    CtmButton*    pobj;
} 	tmTOOLITEM;
	
typedef struct tagtmTOOLBAR
{
    WORD    wGroup;      // Buttons of Group 0 has the down-up behavior
                         // Buttons of Group 1 has the down-up behavior, others up
                         // Buttons of the same Group number other than [0,1] will be one down, others up
    char	*pszWndName;
    WORD    wKey;
    char	*pszBtnCaption;
    WORD    wAction;
}	tmTOOLBAR;


typedef struct tagtmTOOLPARAM
{
    tmTOOLBAR*    	ptoolbar;
    short       	nItemCount;
    short       	nItemIndex;
    short       	nViewIndex;
    short       	nDuplIndex;
} 	tmTOOLPARAM;

typedef struct tagtmTOOLLOGIN
{
    WORD        wPrivilege;
    WORD        wCmd;
    WORD        wAction;
} 	tmTOOLLOGIN;

//	fans add
typedef struct tagtmTOOLSTACK
{
	char*		pszBarName;
	int			iViewIndex;
}	tmTOOLSTACK;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - ToolBar                                      |
+===========================================================================*/
class CtmToolBar : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmToolBar)
public:
	
    CtmToolBar();
    ~CtmToolBar();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
protected:
    WORD        	m_wSP;                 //the number of element in the stack
	BOOL			m_bBmpButton;			//<<yang 2005/10/21 
   
    CtmBitMap*		m_pbmpBackground;
    CtmBitMap      	m_pbmpUp;    // Must keep the button parameter for redraw
	CtmBitMap		m_pbmpDown;
	
	
	//	fans add
	char*			m_pszBmpBackPath;	//	背景圖片路徑
	char*			m_pszBmpUpPath;		
	char*			m_pszBmpDownPath;	
	char*			m_pszMainToolName;	//	就是第一個toolbar
	tmTOOLSTACK		m_ToolStack[tmMAX_TOOLNEST];
	char*			m_pszOnCreateName;
	char*			m_pszOnKey;	//	onkeyA
	char*			m_pszOnKeyEnd;	//
	char*			m_pszOnMessage;			//	fans 2008/6/16 11:30上午
	DWORD			m_dwKeyColor;


	BOOL	(*OnCreateA)			(CtmWnd* pwnd);
	WORD	(*OnToolBarKeyA)		(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex);
	WORD	(*OnToolBarKeyEnd)		(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex);
	void	(*OnMessage)			(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);	//	fans 2008/6/16 11:28上午
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	char*			m_pszCurrentToolName;//	存儲當前的toolbar的名稱
	char*			m_pszCurrentFormName;//	存儲當前的Form的名稱
	int				m_nToolIndex;
    void	CreateButtons(char* pszFileName, int iViewIndex = 0);
	
    void    Redraw(RECT rect);
    WORD    OnKey(WORD wKey);
	
	void    Clear();
	
	//	fans add
	BOOL	CreateA();
	void 	SetPropertys();
	void 	SetPropertys_Nub();
	void	DeleteAllButton();
	void	FreshDraw(CtmToolButton* pbtn);

	
	//	fans add 2008/8/29 01:08下午 for TouchScreen
	BOOL	MouseClick(short x_pos,short y_pos,int key_flag);//
	void	WndProc(int message, WPARAM wParam, LPARAM lParam);		//	fans add 2008/6/16 11:52上午
	
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
protected:
    //	fans add
    int		FindButton(WORD	wKey);	
    void	ShowAllButton();
    void	PushS(tmTOOLSTACK	toolstack);
    tmTOOLSTACK	PopS();
    
};


class CtmSubToolBar : public CtmToolBar
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmSubToolBar)
public:
	
    CtmSubToolBar();
    ~CtmSubToolBar();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    
    
public:
	BOOL	CreateA();
	void	CreateButtons(char* pszFileName, int iViewIndex = 0);
	BOOL	MouseClick(short x_pos,short y_pos,int key_flag);//
	
private:
	
    
};


#endif
