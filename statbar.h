/*===========================================================================+
|  Class    : StatBar                                                        |
|  Task     : Status Bar header file                                         |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 02/20/1995                                                     |
|  Revision : 04/10/1996                                                     |
+===========================================================================*/

#ifndef     D_STATBAR
#define     D_STATBAR

#include    "wnd.h"
//#include    "task.h"
#include	"alpha.h"
#include	"cartoon.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     FIELD_GAP           4
#define     FIELD_WIDTH         72      // 8/*Pixel*/*8/*Character*/+8/*Border*/
#define		tmMAX_STATFIELD		7
#define		tmMAX_PROMPTFIELD	1
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - StatusBar                                    |
+===========================================================================*/
class CtmStatusBar : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmStatusBar)
public:
    CtmStatusBar();
    ~CtmStatusBar();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
	tmWND   m_Field[tmMAX_STATFIELD];

    RECT    m_rect01;
    char    m_sz0[tmMAX_TEXT_COL+1];
    char    m_sz1[tmMAX_TEXT_COL+1];
  

    
    //	fans add
     void	(*OnUpdateA)	(CtmWnd* pwnd);
     BOOL	(*OnCreateA)	(CtmWnd* pwnd);	
     void	(*OnDestroyA)	(CtmWnd* pwnd);
     void	(*OnMessage)	(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);	//	fans 2008/6/16 11:28¤W¤È
     char*	m_pszUpdate;
     char*	m_pszCreate;
     char*	m_pszDestroy;
     char*	m_pszOnMessage;			//	fans 2008/6/16 11:30¤W¤È
     	
public:
	CtmBitMap*	m_pbmpBackground;
	
	//CtmBitMap	m_bmpLogo;
	
	
	//CtmCartoon*	m_pwndField[tmMAX_STATFIELD];	fans add

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    BOOL    Create(tmWND* pwnd);
    void    Redraw(RECT rect);
	void	Clear();
	void	Show();
	void	Update();
	
    void    Show(short nFieldNo, short Index);
    //	fans add
	BOOL 	CreateA();
	void	SetPropertys();
	void	SetPropertys_Nub();
	void	WndProc(int message, WPARAM wParam, LPARAM lParam);		//	fans add 2008/6/16 11:52¤W¤È
	
	

};

/*===========================================================================+
|           Class declaration - PromptBar                                    |
+===========================================================================*/
class CtmPromptBar : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmPromptBar)
public:
    CtmPromptBar();
    ~CtmPromptBar();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    char    m_sz0[tmMAX_TEXT_COL * 2];
	tmWND   m_Field[tmMAX_PROMPTFIELD];
	int		m_nErrorShowNum ;
	
	CtmGDIDoubleBuffer*	m_pDoubleBuffer;
public:    
	CtmBitMap*	m_pbmpBackground;
	CtmWnd		m_wndField[tmMAX_PROMPTFIELD];
	//CAlpha* m_pwndAlpha;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    BOOL    Create(tmWND* pwnd);
    void    Redraw(RECT rect);
	void	Clear();
    void    Show(short nFieldNo, char* pszMsg);
     //	fans add
	BOOL 	CreateA();
	void	ShowAlarm(BOOL bShow = FALSE);
	int		GetErrorCount() {return m_nErrorShowNum;};
    //void    ToggleAlpha();
    //CtmWnd*  GetAlpha()						{return (CtmWnd*)m_pwndAlpha;};
};

#endif
