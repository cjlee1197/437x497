/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
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
#ifndef		D_DIALOG
#define		D_DIALOG

#include	"button.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

//=======
//      Window's object
//=======
#define     tmOBJ_STATIC          0x0010
#define     tmOBJ_STATICX1        0x0011
#define     tmOBJ_STATICX2        0x0031
#define     tmOBJ_EDIT            0x0012
#define     tmOBJ_EDITX1          0x0013
#define     tmOBJ_EDITX2          0x0014
#define     tmOBJ_EDITX3          0x0030  //St0522 for HEX input numeric only
#define     tmOBJ_EDITX4          0x0032  //Steven 2004/3/5 for Time div 10
#define		tmOBJ_EDITUPDOWN	  0x1000
#define		tmOBJ_EDITSTRING	  0x1200
#define     tmOBJ_OPTION          0x0015
#define     tmOBJ_CHECK           0x0016
#define     tmOBJ_METER           0x0017
#define     tmOBJ_GAUGE           0x0018
#define     tmOBJ_GROUP           0x0019
#define     tmOBJ_PANEL           0x0020
#define     tmOBJ_BUTTON          0x0021
#define     tmOBJ_BUTTONOK        0x0022
#define     tmOBJ_BUTTONCANCEL    0x0023
#define     tmOBJ_BUTTONEXIT      0x0024
#define     tmOBJ_SCROLLBAR       0x0025
#define     tmOBJ_LIST            0x0026
#define     tmOBJ_COMBO           0x0027
#define		tmOBJ_BUTTONX1		  0x0028
#define     tmOBJ_SELECT          0x0029
#define     tmOBJ_BITMAP          0x0100
#define     tmOBJ_DRAW            0x0101
#define     tmOBJ_CHART           0x0102
#define     tmOBJ_POPUPMEMO       0x0104      //St103000 for popup message
#define		tmOBJ_DRAWBMP		  0x0105
#define		tmOBJ_PROGRESS		  0x0106
#define		tmOBJ_PLATO			  0x0107
#define		tmOBJ_BTGROUP		  0x0108
#define		tmOBJ_GRID			  0x0109
#define		tmOBJ_ANIMATOR		  0x0110

#define		tmOBJ_CARTOON			0x0111
#define		tmOBJ_IMAGEBOX			0x0112
#define		tmOBJ_CURVE				0x0113

//=======
//      Dialog
//=======
#define     tmMAX_DIALOGITEM       400//160
#define     tmDLG_FGC             0x0001
#define     tmDLG_BGC             0x0002
#define     tmDLG_FONT            0x0003
#define     tmDLG_PARAGRAPH       0x0004
#define     tmDLG_INVISIBLE       0x0005      //wy022400
#define     tmDLG_NULLEDIT        0x0006
#define     tmDLG_STATIC          tmOBJ_STATIC
#define     tmDLG_STATICX1        tmOBJ_STATICX1
#define     tmDLG_STATICX2        tmOBJ_STATICX2
#define     tmDLG_EDIT            tmOBJ_EDIT
#define     tmDLG_EDITX1          tmOBJ_EDITX1
#define     tmDLG_EDITX2          tmOBJ_EDITX2
#define     tmDLG_EDITX3          tmOBJ_EDITX3      //St0522 for HEX input numeric only
#define     tmDLG_EDITX4		  tmOBJ_EDITX4      //Steven 2004/3/5 for Time div 10
#define		tmDLG_EDITUPDOWN	  tmOBJ_EDITUPDOWN
#define	 	tmDLG_EDITSTRING	  tmOBJ_EDITSTRING
#define     tmDLG_OPTION          tmOBJ_OPTION
#define     tmDLG_SELECT          tmOBJ_SELECT 
#define     tmDLG_CHECK           tmOBJ_CHECK
#define     tmDLG_METER           tmOBJ_METER
#define     tmDLG_GAUGE           tmOBJ_GAUGE
#define     tmDLG_GROUP           tmOBJ_GROUP
#define     tmDLG_PANEL           tmOBJ_PANEL
#define     tmDLG_BUTTON          tmOBJ_BUTTON
#define     tmDLG_BUTTONOK        tmOBJ_BUTTONOK
#define     tmDLG_BUTTONCANCEL    tmOBJ_BUTTONCANCEL
#define     tmDLG_BUTTONEXIT      tmOBJ_BUTTONEXIT
#define		tmDLG_BUTTONX1		  tmOBJ_BUTTONX1
#define     tmDLG_SCROLLBAR       tmOBJ_SCROLLBAR
#define     tmDLG_LIST            tmOBJ_LIST
#define     tmDLG_COMBO           tmOBJ_COMBO
#define     tmDLG_BITMAP          tmOBJ_BITMAP
#define     tmDLG_DRAW            tmOBJ_DRAW
#define     tmDLG_CHART           tmOBJ_CHART
#define     tmDLG_POPUPMEMO       tmOBJ_POPUPMEMO    //St103000 for popup message
#define		tmDLG_DRAWBMP		  tmOBJ_DRAWBMP
#define		tmDLG_PROGRESS		  tmOBJ_PROGRESS
#define		tmDLG_GAUGE			  tmOBJ_GAUGE
#define		tmDLG_PLATO			  tmOBJ_PLATO
#define		tmDLG_BTGROUP		  tmOBJ_BTGROUP
#define		tmDLG_CHART			  tmOBJ_CHART
#define		tmDLG_GRID			  tmOBJ_GRID
#define		tmDLG_ANIMATOR		  tmOBJ_ANIMATOR

#define		tmDLG_CARTOON			tmOBJ_CARTOON
#define		tmDLG_IMAGEBOX			tmOBJ_IMAGEBOX
#define		tmDLG_CURVE				tmOBJ_CURVE

#define     tmDLG_END             0x7FFF
#define     tmDLG_TABSTOP         0x8000
#define     tmDLG_NOTABSTOP       0x0000
#define     tmDLG_ONCHANGE		  0x2000           //Chuntzu 2004/4/27 add for D_CMD not skip onchange

#define     MKDLG(dlg, type, style, l,t,r,b, obj) \
				dlg.wType=type; MKWND2(dlg.wnd, style, l,t,r,b) dlg.pobj=obj;
#define     MKDLG1(dlg, type, obj)    dlg.wType=type; dlg.pobj=obj;
#define     MKDLG2(dlg, type)         dlg.wType=type;



const int WAY_CURSORMOVE_DOWN 	= 0;		//	標記光標將向下移動
const int WAY_CURSORMOVE_UP 	= 2;		//	標記光標將向上移動
const int WAY_CURSORMOVE_LEFT	= 1;		//	標記光標將向左移動
const int WAY_CURSORMOVE_RIGHT	= 3;		//	標記光標將向右移動
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

typedef struct tagtmDIALOGITEM
{
    WORD        wType;
    CtmWnd*     pwnd;
} 	tmDIALOGITEM;

typedef struct tagtmDIALOG
{
    WORD        wType;
    tmWND       wnd;
    void*       pobj;
} 	tmDIALOG;

typedef struct tagtmDIALOG_1
{
    WORD        wType;
    tmWND_2       wnd;
} 	tmDIALOG_1;

/*===========================================================================+
|           Class declaration - Dialog                                       |
+===========================================================================*/
class CtmDialog : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmDialog)
public:                      
    CtmDialog();
    CtmDialog(tmWND* pwnd, tmDIALOG* pdlg);
    ~CtmDialog();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
protected:
    short           m_nItemCount;
    tmDIALOGITEM    m_aitem[tmMAX_DIALOGITEM];

    short           m_nTabStop;
    BOOL            m_bChangeTabStop;       //St120700 add for Goto() have goto next item   
    BOOL            m_bGetFocus;
    BOOL            m_bOK;
    BOOL            m_bPrompt;              //St052501 for message box not show prompt

    CtmButtonOK*      m_pbuttonOK;
    CtmButtonCancel*  m_pbuttonCancel;
    CtmButton*        m_pbuttonExit;
    
    //	fans add
    char*			m_pszTitle;
    char*			m_pszTitleID;
    char*			m_pszBmpBackPath;	//	fans add
    
    CtmWnd			*m_pCurrentFocus;
    BOOL			m_DrawHelpFlag;
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Make(tmDIALOG* pdlg, tmDIALOG_1* pdlg_1);
    BOOL    Create(tmWND* pwnd, tmDIALOG* pdlg);

	void    Get();
	void    Set();
	void    Show();
	void    Redraw(RECT rect);

	void    OnGetFocus();
	void    OnLoseFocus();
	WORD    OnKey(WORD wKey);

    BOOL    DoModal();
    void    Goto(WORD wIDControl);
    void    Update(WORD wIDControl);
    CtmWnd*   GetControl(WORD wIDControl)   {return m_aitem[wIDControl].pwnd;};
	short   Get_TabStop()                    {return m_nTabStop;};
	void    SetNoTabStop(WORD wIDControl)   {m_aitem[wIDControl].wType &= ~tmDLG_TABSTOP;};
	void    SetTabStop(WORD wIDControl)     {m_aitem[wIDControl].wType |=  tmDLG_TABSTOP;};
/**/void    SetOnChange(WORD wIDControl)    {m_aitem[wIDControl].wType |=  tmDLG_ONCHANGE;};//Chuntzu 2004/4/27 add for D_CMD not skip onchange



	//	fans add
	BOOL 	CreateA();
	void	SetPropertys();
	void	SetPropertys_Nub();
	
	//	fans add 2010-1-25 8:55:52
	int		OnLose();
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
protected:
    void    FindNextTabStop();
    void    FindLastTabStop();

    void    FindLeftTabStop();
    void    FindRightTabStop();
    void    FindUpTabStop();
    void    FindDownTabStop();
    
    void	FindTabStop(int iWay = 0);		//	iWay表示光標移動的方向，0：表示下，2：表示向上；1：表示左，2：表示右
	int		FindNearTabStop(char* pszAlign, char* pszWay, int iWay = 0);
	
	int		FindNearBig(char* pszAlign, char* pszWay, CtmWnd* pwndCurrent);
	int		FindNearSmall(char* pszAlign, char* pszWay, CtmWnd* pwndCurrent);
	
    void    OnGetFocus(short nTabStop);
    void    OnLoseFocus1(short nTabStop);
};


#endif
