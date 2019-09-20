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

#ifndef  	D_EDIT
#define		D_EDIT

#include	"wnd.h"
#include	"task.h"
#include	"wndx2.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     MKEDIT(edit, type, max, echo, string, prompt) \
            edit##.wType=type; edit##.wMaxChar=max; edit##.wEcho=echo; edit##.psz=string; edit##.pszPrompt=prompt;
            
#define     MKEDITX1(edit, type, val, min, max, prompt) \
            edit.wType=type; edit.lValue=val; edit.lMin=min; edit.lMax=max; edit.pszPrompt=prompt;

#define     MKEDITX2(edit, type, val, min, max, prompt) \
            edit.wType=type; edit.wValue=val; edit.wMin=min; edit.wMax=max; edit.pszPrompt=prompt;
            
#define     tmEDIT_ALPHA          0x0001
#define     tmEDIT_NUMERIC        0x0002
#define     tmEDIT_ALPHANUMERIC   0x0004

#define     tmEDIT_BOOL           0x0008
#define     tmEDIT_FLOAT          0x0010
#define     tmEDIT_FLOAT1         0x0010
#define     tmEDIT_FLOAT2         0x0010
#define     tmEDIT_FLOAT3         0x0010
#define     tmEDIT_HEXADECIMAL    0x0020
#define     tmEDIT_HEXNOALPHA     0x0040   //St0522 for HEX input numeric only
#define		tmEDIT_PASSWORD		  0x0080	//	fans add
#define		tmEDIT_OVERPASSWORD	  0x0085	//James add
#define     tmEDIT_NOEMPTY        0x0100
#define     tmEDIT_NOTERMINATOR   0x0200
#define     tmEDIT_TIMEDIVTEN     0x0400   //Steven 2004/3/5 for Time div 10
#define     tmEDIT_ONCHANGE       0x0800   //Steven 2004/3/11 for D_CMD not skip Onchange
#define     tmEDIT_DISABLE        0x8000

                                                           // HF,the default font height; WF,the default font width
#define     HED                 (WB*1+HF*1+WB*1)            // Edit box height   //WB is the default border width
#define     WED_1               (WB*2+WF*1+WB*2)            // Edit box width - 1 character
#define     WED_2               (WB*2+WF*2+WB*2)            // Edit box width - 2 characters
#define     WED_3               (WB*2+WF*3+WB*2)            // Edit box width - 3 characters
#define     WED_4               (WB*2+WF*4+WB*2)            // Edit box width - 4 characters
#define     WED_5               (WB*2+WF*5+WB*2)            // Edit box width - 5 characters
#define     WED_6               (WB*2+WF*6+WB*2)            // Edit box width - 6 characters
#define     WED_7               (WB*2+WF*7+WB*2)            // Edit box width - 7 characters
#define     WED_8               (WB*2+WF*8+WB*2)            // Edit box width - 8 characters
#define     WED_9               (WB*2+WF*9 +WB*2)            // Edit box width - 9 characters
#define     WED_10              (WB*2+WF*10+WB*2)            // Edit box width - 10 characters



const	int MAXDBNUM = 10;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmEDIT
{
    WORD        wType;
    WORD        wMaxChar;               // Field width
    WORD        wEcho;                  // Echo character
    char*       psz;
    char*       pszPrompt;
} 	tmEDIT;

typedef struct tagtmEDITX1
{
	WORD        wType;                  // Precision: 小數位數
	long long   lValue;                 // Edit value
	long long	lMin;                   // Minimum scale
	long long	lMax;                   // Maximum scale
	char*       pszPrompt;              // 
} 	tmEDITX1;

typedef struct tagtmEDITUPDOWN
{
	WORD        wType;                  // Precision: 小數位數
	long        lValue;                 // Edit value
	long        lMin;                   // Minimum scale
	long        lMax;                   // Maximum scale
	char*       pszPrompt;              // 
} 	tmEDITUPDOWN;

typedef struct tagtmEDITX2
{
    WORD        wType;                  // Precision: 
    WORD        wValue;                 // Edit value
    WORD        wMin;                   // Minimum scale
    WORD        wMax;                   // Maximum scale
    char*       pszPrompt;
} 	tmEDITX2;

typedef struct tagtmEDITSTRING
{
    WORD        wType;
    WORD        wMaxChar;               // Field width
	WORD		wEcho;
    char*       psz;
    char*       pszPrompt;
} 	tmEDITSTRING;

/*===========================================================================+
|           Class declaration - Nulledit                                    |
+===========================================================================*/

class CtmNulledit : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmNulledit(tmWND* pwnd);
    ~CtmNulledit();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Show();
/**/void    Update()            {};
   void	   Redraw(RECT rect);
};

/*===========================================================================+
|           Class declaration - EditBox                                      |
+===========================================================================*/
class CtmEdit : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmEdit)
public:
    CtmEdit();
    ~CtmEdit();
    
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
//  short   m_nChar;					// zholy070803
    
    short   m_nXGapHalf;
    short   m_nYGapHalf;
    short   m_nClientWidthText;

    short   m_nRow;
	short   m_nCol;
//  short   m_nIndex;					// zholy070803
    char    m_cOldASC;                  // St1116'02 for ABC input
    short   m_nN2CIndex;
    short   m_nOldChar;
    short   m_nOldCol;
    short   m_nOldIndex;
    BOOL    m_bOldChanged;        
    short   m_nEchoCol;
    short   m_nEchoRow;
    char    m_szEcho[tmMAX_TEXT_COL+1];

    CTaskCursor*    m_ptaskCursor;
//  BOOL            m_bChanged;			//St120100 move to CtmWnd for get Change status
    ImageBackground	m_BackImage;		//	fans add
    BOOL        	m_bBackGround;
    
    
    short	m_nOffSet;					//	fans add 2009-3-24 13:58:47
 protected:
 	//	fans add
	char*	m_pszPrompt;					//	提示信息
 	char*	m_pszPromptLan;
 	char*	m_pszPromptID;				//	提示信息的ID
 	char    m_sz[tmMAX_TEXT_COL+1];		//	顯示出來的字串
 	char*	m_pszText;					//	這是與m_sz一致的,之所以需要,是因為屬性中需要
 	WORD    m_wType;					//	Edit的類型
    WORD    m_wMaxChar;					//	Edit所允許顯示的最長字串
    BOOL	m_bValueSymbol;				//  可輸入符號 TRUE 為可輸入- 
    WORD    m_wEcho;					//	?
    WORD	m_wBKSP;					//  Backspace  Flag Caozz add 2011/11/5 上午 11:23:59
	BGC 	m_bgcOld;
	
	short   m_nChar;					// zholy070803
	short   m_nIndex;					// zholy070803
	BOOL	m_bChanged;					// St120100 move to CtmWnd for get Change status zholy070803
	BOOL	m_bDelete;					// 判斷按C時密碼也不能看見  James add 2007/9/5 05:39下午
	BOOL	m_bOverPassword;			// PassWord  Edit
	BOOL	m_bFInputFlag;				// 輸入密碼第一次顯示也延遲200ms James add 2007/9/9 10:06下午
	WORD	m_wInsertPos;
	WORD	m_wOldInsertPos;
	bool	m_bHintFlag;
	bool	m_bLoseFocus;

	
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
   

/**/void    Get();
/**/void    Set();
/**/void    SetAll();
/**/void    Show();
/**/void    Update();
/**/void    UpdateAll();

/**/void    OnGetFocus();
/**/void    OnLoseFocus();
/**/WORD    OnKey(WORD wKey);

/**/void    CalcRECT();
	void    Clear();

    void    OnGetFocus1();
    void    OnLoseFocus1();
    
    void 	SetPropertys();	//	fans add
    void 	SetPropertys_Nub();
    BOOL 	CreateA();		//	fans add
    void	SetText(char* psz) ;	//	fans add
    char*	GetText() {return m_sz;};//	fans add
    
    void	ReSet();			//	fans add
    void    EnableCursor(BOOL bEnable);	// <FANS> 2006/11/28 04:21下午 move from private
    
protected:
	
    void    ResetPosition();
    void    DisplayCursor(BOOL bOn);  //St122700 move from private for popmemo
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void    ShowXY(short nCol, short nRow, char* psz);
    void    Clear(short nCol, short nRow, short nCount);
    
    void    SetCursorPosition(short nCol, short nRow);

    BOOL    GetWaitStatus();
    void    CursorWait(BOOL bWait);
    void    WriteEcho(short nX, short nY, char cEcho);
};

/*===========================================================================+
|           Class declaration - EditBox                                      |
+===========================================================================*/
class CtmEditX1 : public CtmEdit
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmEditX1)
public:
    
    CtmEditX1();	//	fans add
    ~CtmEditX1();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
protected:
	char*		m_pszMaxDBID;
	char*		m_pszMinDBID;
	char*		m_pszMaxEditName;
	char*		m_pszMinEditName;
    char*		m_pszDBID[MAXDBNUM];	//	fans add:對應的DataBase的ID,可以同時對應個
    char    	m_szPrompt0[tmMAX_TEXT_COL * 2];   // Prompt buffer
    int   		m_nPrecision;
    int			m_nPrecisionData;		  //	資料的精度
    long long	m_lMin;                   // Minimum scale
	long long	m_lMax;                   // Maximum scale
	long long   m_lValue;                 // Edit value
	long long   m_lOldValue;
	
	int			m_nExPrecision; 

	BOOL		m_bGetDB;
	BOOL		m_bDBMax;
	BOOL		m_bDBMin;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Get();
/**/void    Set();
/**/void    SetAll();
/**/void    Update();
/**/void    UpdateAll();
/**/WORD    OnKey(WORD wKey);

/**/void    OnGetFocus();	//	fans add
/**/void    OnLoseFocus();	//	fans add	
/**/void    Show();			//	fans add
/**/void 	SetPropertys();	//	fans add
    void 	SetPropertys_Nub();
/**/BOOL 	CreateA();		//	fans add

	void	ReSet();			//	fans add

	int		OnLose();		//	fans add 2010-1-15 15:47:07
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
    void	Prepare();
private:
	void	GetDBInfo(BOOL	bPrecisionZero);
};


/*===========================================================================+
|           Class declaration - EditBox                                      |
+===========================================================================*/
class CtmEditX2 : public CtmEdit
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmEditX2)
public:
    
    CtmEditX2();	//	fans add
    ~CtmEditX2();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:

	long long   m_lValue;                 // Edit value
	long long	m_lOldValue;
	char*		m_pszDBID[MAXDBNUM];	//	fans add:對應的DataBase的ID,可以同時對應個
	//Sunny<20070823> add	<<<<<<
	char    	m_szPrompt0[tmMAX_TEXT_COL * 2];   // Prompt buffer
	int   		m_nPrecision;
	long long	m_lMin;                   // Minimum scale
	long long	m_lMax;                   // Maximum scale
	//Sunny<20070823> add	>>>>>>
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Get();
/**/void    Set();
/**/WORD    OnKey(WORD wKey);
/**/void    Show();			//	fans add
/**/void 	SetPropertys();	//	fans add
    void 	SetPropertys_Nub();
/**/BOOL 	CreateA();		//	fans add
	void    Update();
/**/void    OnLoseFocus();	//	fans add	
/**/void    OnGetFocus();	//	fans add
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
	void	Prepare();		//Sunny<20070823> add
	
	int		OnLose();		//	fans add 2010-1-15 15:47:07
private:
};



#endif
