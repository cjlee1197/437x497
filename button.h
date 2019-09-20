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
|  Revision : 09/19/2003                                                     |
+===========================================================================*/

#ifndef	D_tmBUTTON
#define	D_tmBUTTON

#include	"wnd.h"
#include	"wndx2.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     tmBUTTON_TEXT         0x0001
#define     tmBUTTON_BMP          0x0002
#define     tmBUTTON_INVERSE      0x1000

#define     tmMAX_BUTTON_TEXT     32
#define     tmBUTTON_DOWN         0x0001
#define     tmBUTTON_UP           0x0002
#define     tmBUTTON_DOWNUP       0x0003

#define     tmDEFAULT_W_BUTTON    (2+8*8+2)
#define     tmDEFAULT_H_BUTTON    (4+16+4)

#define		tmBUTTON_CAPTION_NUM	10
#define		tmBUTTON_CAPTION_TOP	0x0008
#define		tmBUTTON_CAPTION_BOTTOM	0x0010


#define     MKBUTTON(button, t, pc)     button.wType=t; button.pContent=pc;
#define     MKBUTDEF1(butdef, x, y) 	butdef.nX=x; butdef.nY=y;

#define     HBT                 (WB*3+HF*1 +WB*2)           // Button height
#define     WBT_1               (WB*3+WF*1 +WB*2)           // Button width - 1  character
#define     WBT_2               (WB*3+WF*2 +WB*2)           // Button width - 2  characters
#define     WBT_3               (WB*3+WF*3 +WB*2)           // Button width - 3  characters
#define     WBT_4               (WB*3+WF*4 +WB*2)           // Button width - 4  characters
#define     WBT_5               (WB*3+WF*5 +WB*2)           // Button width - 5  characters
#define     WBT_6               (WB*3+WF*6 +WB*2)           // Button width - 6  characters
#define     WBT_7               (WB*3+WF*7 +WB*2)           // Button width - 7  characters
#define     WBT_8               (WB*3+WF*8 +WB*2)           // Button width - 8  characters
#define     WBT_9               (WB*3+WF*9 +WB*2)           // Button width - 9  characters
#define     WBT_10              (WB*3+WF*10+WB*2)           // Button width - 10 characters
#define     WBT_11              (WB*3+WF*11+WB*2)           // Button width - 11 characters
#define     WBT_12              (WB*3+WF*12+WB*2)           // Button width - 12 characters
#define     WBT_13              (WB*3+WF*13+WB*2)           // Button width - 13 characters
#define     WBT_14              (WB*3+WF*14+WB*2)           // Button width - 14 characters

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

typedef struct tagtmBUTTON
{
    WORD        wType;
    CtmBitMap	*pbmpUp;							//<<yang 2005/10/20 UpButton Bmp Pointer
    CtmBitMap	*pbmpDown;							//<<yang 2005/10/20 DownButton Bmp Pointer
    char*       pContent[tmBUTTON_CAPTION_NUM];     // Button content
}	tmBUTTON;

typedef struct tagtmBUTTON_DEF
{
    short       nX;                     // X of upper-left corner
    short       nY;                     // Y of upper-left corner
    tmHWND      hwndParent;
} 	tmBUTTON_DEF;

/*===========================================================================+
|           Class declaration - Button                                       |
+===========================================================================*/
class CtmButton : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmButton)
public:
    CtmButton();
    CtmButton(tmWND* pwnd, tmBUTTON* pbutton);
    ~CtmButton();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
protected:
	tmBUTTON* m_pbutton;
    tmBUTTON  m_button;	//	fans add
    int		  m_nSpace;
	
	WORD	m_wAction;		//	對應的動作類型
	char*	m_pszWndName;	//	對應的窗體，其具體的路徑位置
		
private:

    BOOL    m_bUp;
    BOOL    m_bActive;          //St122900 for disable button flag
    short   m_nX[tmBUTTON_CAPTION_NUM];
    short   m_nY[tmBUTTON_CAPTION_NUM];

   	FGC     m_bakFgc;
   	BGC     m_bakBgc;
    //CFONT   m_cf;               //St122900 for disable button font type    
    WORD    m_wStyle;           //St122900 for disable button border type   
    BOOL    m_bBmpStyle;
    
    ImageBackground	m_BackImage;		//	fans add
    BOOL        	m_bBackGround;
    
    //	fans add
    char*	m_pszCaptionID;	//	caption id
    char*	m_pszUpPath;	//	up bitmap
    char*	m_pszDownPath;	//	down bitmap
    
    char*	m_pszCaptionIDs[tmBUTTON_CAPTION_NUM];
	unsigned long	m_dwKeyColor;
	WORD	m_wCaptionType;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    BOOL    Create(tmWND* pwnd, tmBUTTON* pbutton);
/**/void    Set();
/**/void    Show();
/**/void    Update();
	void    Clear();

    void    SetFGColor(FGC fgc);
    void    SetBGColor(BGC bgc);
    void    Press(WORD wFlag);
    void    Disable();
    void    Enable();
    
    //	fans add
    BOOL	CreateA();	
    void 	SetPropertys();
    void 	SetPropertys_Nub();
    void	SetUpBmp(CtmBitMap* pbmp) {if (m_pszUpPath == NULL) m_button.pbmpUp = pbmp;};
    void	SetDownBmp(CtmBitMap* pbmp) {if (m_pszDownPath == NULL) m_button.pbmpDown = pbmp;};

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void    Up();
    void    Down();
    void    ShowContent();
	
	
};

/*===========================================================================+
|           Class declaration - ToolButton                                   |
+===========================================================================*/
//	fans add this class
class CtmToolButton: public CtmButton
{
	/*---------------------------------------------------------------------------+
	|           Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmToolButton)
	public:
		CtmToolButton();	
		~CtmToolButton();
	/*---------------------------------------------------------------------------+
	|           Attributes                                                       |
	+---------------------------------------------------------------------------*/
	private:
		WORD	m_wKey;			//	對應的是哪個按鍵
		//WORD	m_wAction;		//	對應的動作類型
		WORD	m_wGroup;		// Buttons of Group 0 has the down-up behavior
                         		// Buttons of Group 1 has the down-up behavior, others up
                         		// Buttons of the same Group number other than [0,1] will be one down, others up
		//char*	m_pszWndName;	//	對應的窗體，其具體的路徑位置
		char*	m_pszToolBarName;//	當點擊的時候，對應的Toolbar
		char*	m_pszSubToolBarName;
		BOOL	m_bOnFocus;
	/*---------------------------------------------------------------------------+
	|           Operations                                                       |
	+---------------------------------------------------------------------------*/
	public:
		void	SetPropertys();
		void	SetPropertys_Nub();
		void	OnLoseFocus();
		void	OnGetFocus();
		void	Update();
		WORD	OnKey(WORD	wKey);
		
	
};

/*===========================================================================+
|           Class declaration - ButtonOK                                     |
+===========================================================================*/
class CtmButtonOK : public CtmButton
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmButtonOK(tmBUTTON_DEF* pbutton);
    ~CtmButtonOK();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmBUTTON  m_button;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
};

/*===========================================================================+
|           Class declaration - ButtonCancel                                 |
+===========================================================================*/
class CtmButtonCancel : public CtmButton
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmButtonCancel(tmBUTTON_DEF* pbutton);
    ~CtmButtonCancel();
    
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmBUTTON  m_button;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
};

#endif
