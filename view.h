/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : View                                                           |
|  Task     : View header file                                               |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 05/02/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_VIEW
#define     D_VIEW

#include    "wnd.h"

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
|           Class declaration - View                                         |
+===========================================================================*/
class CtmView : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmView)
public:
    CtmView();
    virtual ~CtmView();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    BOOL            m_bGetFocus;
    BOOL            m_bOnChange;                        // St072601 for not save when not onchange
    CtmWnd*         m_pwndFocus;
	
	BOOL            m_bGetFocus2;
    BOOL            m_bOnChange2;                        // St072601 for not save when not onchange
    CtmWnd*         m_pwndFocus2;
	
    WORD            m_wCmd;
    char            m_szView[tmMAX_TEXT_COL+1];
    short			m_nTitleIndex;
    CtmBitMap*		m_pbmpBackGround;
    
    //	fans	add
    char*			m_pszOverViewName;
    char*			m_pszBmpBackPath;
    CtmBitMap		m_bmpBack;
    char			m_pszOldFormName[256];
    char			m_pszFormName[256];
    char			m_pszOldFormName2[256];
    char			m_pszFormName2[256];

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    BOOL    Create(tmWND* pwnd);

    void    Update();
    void    Redraw(RECT rect);
	void	OnResize();
    void    OnGetFocus();
    void    OnLoseFocus();
    void    OnGetFocus2();
    void    OnLoseFocus2();
    WORD    OnCommand(WORD wCmd);
    WORD	OnCommand(char* pszCmd);
    WORD    OnCommand2(WORD wCmd);
    WORD	OnCommand2(char* pszCmd);
    WORD    OnChange(WORD wIDControl);
    WORD    OnKey(WORD wKey);
    char*	GetCurrentFormName() { return m_pszFormName; };
    void	WndProc(int message, WPARAM wParam, LPARAM lParam);		//	fans add 2008/6/16 11:52¤W¤È
    
    //	fans add
    BOOL	CreateA();
    void 	SetPropertys();
    void 	SetPropertys_Nub();
    void    Show();
    void 	ClearOldName();
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
public:
    void      View_();
    void      Help();
    void	  SetBmpBack(char* pszFileName);	//fans add
private:
    void      UpdateFrameTitle();
    int		SearchParseView(char* pszCmd);
     void	AddParseView(CParseObject* ParseView);
     void	AddFormCmd(char* pszCmd);
    
};

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef     void    (CtmView::*PFUNC)();   // Pointer to member function

#endif
