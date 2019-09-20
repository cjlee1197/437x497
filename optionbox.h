/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef	D_OPTIONBOX
#define	D_OPTIONBOX

#include	"wnd.h"
#include	"wndx2.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     WOB                 24                          // Option button width
#define     WOI_1               (WOB+WF*1 +8)               // Option item width - 1  character
#define     WOI_2               (WOB+WF*2 +8)               // Option item width - 2  characters
#define     WOI_3               (WOB+WF*3 +8)               // Option item width - 3  characters
#define     WOI_4               (WOB+WF*4 +8)               // Option item width - 4  characters
#define     WOI_5               (WOB+WF*5 +8)               // Option item width - 5  characters
#define     WOI_6               (WOB+WF*6 +8)               // Option item width - 6  characters
#define     WOI_7               (WOB+WF*7 +8)               // Option item width - 7  characters
#define     WOI_8               (WOB+WF*8 +8)               // Option item width - 8  characters
#define     WOI_9               (WOB+WF*9 +8)               // Option item width - 9  characters
#define     WOI_10              (WOB+WF*10+8)               // Option item width - 10 characters
#define     WOI_11              (WOB+WF*11+8)               // Option item width - 11 characters
#define     WOI_12              (WOB+WF*12+8)               // Option item width - 12 characters


const	int	MAXOPTINSELECT = 12;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmOPTIONITEM
{
    short       nX;                     // X of upper-left corner
    short       nY;                     // Y of upper-left corner
    char*       psz;
} 	tmOPTIONITEM;

typedef struct tagtmOPTION
{
    short       	nIndex;                 // Current selection
    short      		nCount;                 // Number of option item
    tmOPTIONITEM 	pitem[MAXOPTINSELECT];
    char*       	pszPrompt;
} 	tmOPTION;

/*===========================================================================+
|           Class declaration - OptionBox                                    |
+===========================================================================*/
class CtmOptionBox : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmOptionBox)
public:
    CtmOptionBox(tmWND* pwnd, tmOPTION* poption);
    CtmOptionBox();
    ~CtmOptionBox();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmOPTION* m_poption;
    tmOPTION  m_option;	//	fans add
    int		m_nIndex;
    int		m_nOldIndex;//	fans add 2006/12/29 10:55上午
    BGC		m_bgcOld;	// <FANS> 2006/11/28 02:07下午


	//	fans add
	char*	m_psztSetlectID[MAXOPTINSELECT];
	char*	m_pszPromptID;
	char*	m_pszDBID[MAXOPTINSELECT];	//	fans add:對應的DataBase的ID
	int		m_iStartVaule;
	ImageBackground	m_BackImage;	//	fans add
	BOOL        	m_bBackGround;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Get();               //{m_poption->nIndex = m_nIndex;};
/**/void    Set()               {m_nIndex = m_poption->nIndex;};
/**/void    Show();
/**/void    Update();

/**/void    OnGetFocus();
/**/void    OnLoseFocus();
/**/WORD    OnKey(WORD wKey);

	void    Clear();
	//	fans add
	BOOL	CreateA();
	void	SetPropertys();
	
	int		OnLose();		//	fans add 2010-1-15 15:47:07
    void    WndProc(int message, WPARAM wParam, LPARAM lParam);
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void	Select(short nIndex);
    bool    TouchDownEvent(int x, int y);
    bool    TouchUpEvent(int x, int y);
    bool    m_inTouchArea;
};

#endif
