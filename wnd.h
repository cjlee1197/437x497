/*===========================================================================+
|  Class    : Windows                                                        |
|  Task     : Windows Manager header file                                    |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/10/1995                                                     |
|  Revision : 03/23/1996                                                     |
+===========================================================================*/

#ifndef     D_WND
#define     D_WND

#include	"common.h"
#include	"cbitmap.h"
#include	"cfont.h"
#include	"gui.h"
#include	"stable.h"
#include	"database.h"
#include	"_kb.h"
#include	"_gpio.h"
#include	"CObject.h"


#include	"strclass.h"
#include	"propertylist.h"
#include	"aioperation.h"



/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

#define     NULL_HWND           	0

#define     tmWS_BORDER_NONE      0x0000
#define     tmWS_BORDER_PLAIN     0x0001
#define     tmWS_BORDER_RAISE     0x0002
#define     tmWS_BORDER_INSET     0x0003
#define     tmWS_BORDER_FRAME     0x0004
#define     tmWS_BORDER_CLEARALL  0x0005
#define		tmWS_BORDER_ALLRAISE  0x000A
#define		tmWS_BORDER_SCROLL	  0x0001
#define     tmWS_BORDER			  0x0007
#define 	tmWS_UNDERLINE_BORDER 0x0009

#define     tmWS_BORDER_PLAIN2    0x0006
#define     tmWS_BORDER_ZERO      0x0007
#define		tmWS_FRAMEWND		  0x0800

#define		tmWS_CAPTION		  0x0008
#define     tmWS_TRANSPARENT      0x0010
#define     tmWS_CREATE           0x0020
#define     tmWS_SHOW             0x0020
#define     tmWS_MAXIMIZE         0x0040
#define     tmWS_MINIMIZE         0x0080
#define     tmWS_MOLDIMAGE        0x0100
#define     tmWS_RHOMB            0x0200
#define     tmWS_CLEAR            0x4000
#define     tmWS_REDRAW           0x8000
#define     tmWS_INVISIBLE        0x2000
#define		tmWS_ONBMP			  0x3000

#define     tmDEFAULT_W_BORDER    	4       // Default border's width
#define     tmDEFAULT_H_TITLE     	32

#define     tmMAX_TEXT_COL        	256

#define     tmPARA_ALIGN_LEFT     	0x0001 	//	left
#define     tmPARA_ALIGN_CENTER   	0x0002 	//	X.center
#define     tmPARA_ALIGN_RIGHT    	0x0004 	//	right
#define     tmPARA_ALIGN_TOP     	0x0008	//	top
#define     tmPARA_ALIGN_BOTTOM    	0x0010	//	bottom
#define     tmPARA_ALIGN_CENTERY   	0x0020	//	Y.Center


#define     DT_LEFT  				tmPARA_ALIGN_LEFT     	
#define     DT_CENTER				tmPARA_ALIGN_CENTER   	 
#define     DT_RIGHT 				tmPARA_ALIGN_RIGHT    	


#define     HF                  18                          // Default font height
#define     WF                  8                          // Default font width
#define     WP                  9                           // Default point width
#define     WB                  1                           // Default control border width

//=======
//		BMP
//=======
#define     tmHEIGHT_TOOL        	42 
#define     tmHEIGHT_STATUS       	45
#define		tmHEIGHT_PROMPT			26
#define		tmHEIGHT_VIEW			487
#define		tmTOOLBAR_BOTTON_WIDTH	75
#define		tmTOOLBAR_BOTTON_HEIGHT	22
#define		tmSTATE_WIDTH			387
#define		tmTITLE_HEIGHT			tmHEIGHT_STATUS
#define		tmTITLE_WIDTH			413
#define		tmLOGO_WIDTH			34
#define		tmLOGO_HEIGHT			32
#define		tmSTATEBAR_BMP_WIDTH	tmLOGO_WIDTH
#define		tmSTATEBAR_BMP_HEIGHT	tmLOGO_HEIGHT
#define		tmSTATE_START_POSITION	70
#define		tmSTATEBAR_CLAMP_WIDTH	tmTOOLBAR_BOTTON_WIDTH
#define		tmSTATEBAR_CLAMP_HEIGHT	tmSTATEBAR_BMP_HEIGHT
#define		tmNOZZLE_WIDTH			584
#define		tmNOZZLE_HEIGHT			83
#define		tmTEMPER_WIDTH			539
#define		tmTEMPER_HEIGHT			47
#define		tmMACHINE_WIDTH			509
#define		tmMACHINE_HEIGHT		159
#define		tmSPLIT_WIDTH			763
#define		tmSPLIT_HEIGHT			26


#define		PROMPT_BAR_DIR			"PromptBar.bmp"
#define		STATE_BAR_DIR			"StateBar.bmp"
#define		tmTOOLBAR_BACKGROUND_DIR "ToolBar.bmp"
#define		tmTOOLBAR_BUTTONUP		"ButtonUp.bmp"
#define		tmTOOLBAR_BUTTONDONW	"ButtonDown.bmp"
#define		tmTOOLBAR_BOTTONF1UP	"ButtonF1.bmp"
#define		tmTOOLBAR_BOTTONLARGER	"ButtonLarger.bmp"
#define		tmBMP_LEFT				"left.bmp"
#define		tmBMP_RIGHT				"right.bmp"
#define		tmBMP_BOTTOM			"bottom.bmp"
#define		tmBMP_TITLE				"Title.bmp"
#define		tmBMP_LOGO				"LOGO.bmp"
#define		tmBMP_HAND				"Hand.bmp"
#define		tmBMP_TIMEAUTO			"TimeAuto.bmp"
#define		tmBMP_SEMIAUTO			"SemiAuto.bmp"
#define		tmBMP_SENSORAUTO		"SensorAuto.bmp"
#define		tmBMP_TIME1				"Time1.bmp"
#define		tmBMP_TIME2				"Time2.bmp"
#define		tmBMP_TIME3				"Time3.bmp"
#define		tmBMP_LINK11			"Link1-1.bmp"
#define		tmBMP_LINK12			"Link1-2.bmp"
#define		tmBMP_LINK21			"Link2-1.bmp"
#define		tmBMP_LINK22			"Link2-2.bmp"
#define		tmBMP_OVERVIEW			"newbg.bmp"
#define		tmBMP_BACKGROUND		"Background.bmp"
#define		tmBMP_CLAMP1			"Clamp1.bmp"
#define		tmBMP_CLAMP2			"Clamp2.bmp"
#define		tmBMP_CLAMP3			"Clamp3.bmp"
#define		tmBMP_ALARM				"Alarm.bmp"
#define		tmBMP_TEMPER			"Temper.bmp"
#define		tmBMP_NOZZLE			"Nozzle.bmp"
#define		tmBMP_MACHINE			"Machine.bmp"
#define		tmBMP_ADJUST			"adjustmold.bmp"
#define		tmBMP_SPLIT				"Splite.bmp"
#define		tmBMP_HTLOGO			"HaitianLogo.bmp"
#define		tmBMP_TMLOGO			"TMLogo.bmp"
#define		tmBMP_SELECTED			"selected.bmp"
#define		tmBMP_UNSELECTED		"unselected.bmp"
#define		tmBMP_MACHINEBG			"MachineBg.bmp"
#define		tmBMP_JOG				"jog.bmp"
#define		tmBMP_HOME				"home.bmp"
#define		tmBMP_ACTUALBG			"ActualBg.bmp"
#define		tmBMP_ACTUALBGW			"ActualBgw.bmp"

//	fans add

#define		tmBMP_CLAMPOPEN			"moldopen.bmp"
#define		tmBMP_CLAMPCLOSE		"moldclose.bmp"
#define		tmBMP_AIRBLSTMOVE		"airblstmov.bmp"
#define		tmBMP_AIRBLSTSTN		"airblststn.bmp"
#define		tmBMP_COREINA			"corein-a.bmp"
#define		tmBMP_COREOUTA			"coreout-a.bmp"

#define		tmBMP_HORIZONAL			"Bhorizontal.bmp"
#define		tmBMP_VERTICAL			"Bvertical.bmp"
#define		tmBMP_POINT				"Bpoint.bmp"

#define     tmFONT_PLAIN          0x0000
#define     tmFONT_OPAQUE         0x8000
#define     tmFONT_BOLD           0x0001
#define     tmFONT_UNDERLINE      0x0002
#define     tmFONT_SHADOW         0x0004
#define     tmFONT_EMBOSS         0x0008
#define     tmFONT_INSET          0x0010

#define     tmFONT_C_UNDERLINE    '&'
#define     tmFONT_C_NEWLINE      '\n'

#define		MAX_HELP_NUM	32//64
//	fans add
const int MAX_CONTROLS = 300;//512;
const int MAX_NAME = 64;//256;
const int MAX_PROPERTY = 256;//164;//129;//MAX_CONTROLS;
const int MAX_PATH = 96;//256;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

enum PropType 
{
	tmBYTE, tmSHORT, tmINT, tmWORD, tmDWORD, tmLONG, tmFLOAT, tmINT64, tmDOUBLE,
	tmENUM, tmSTRUCT,
	tmSTR, tmUSTR, tmCHAR, tmBOOL,
	tmCLASS, tmPROCEDURE, 
	tmFontStream,
	tmUnknown
};

typedef struct tagPropInfo
{
	char 		pszName[20];
	int			nNub;
	PropType 	ptype;
	void* 		propAddress;
} tmPropInfo;

//typedef     WORD                HWND;   // Handle of window
typedef struct tagtmWINSTYLE
{
    WORD    border      : 3;
    WORD    caption     : 1;
    WORD    transparent : 1;
    WORD    create      : 1;

    WORD                : 7;    //wy022400
    WORD    invisible   : 1;    //wy022400
    WORD    clear       : 1;
    WORD    redraw      : 1;
} 	tmWINSTYLE;

typedef struct tagtmWND
{
    char*   	pszTitle;                   // Title of the window
    DWORD   	wStyle;                     // Style of the window
    RECT    	rect;                       // Window's position and size
    tmHWND  	hwndParent;                 // Handle of the parent window
    CtmFont**	pcFont;						// Handle of the font
} 	tmWND;

typedef struct tagtmWND_2
{
    WORD    wStyle;                     // Style of the window
    RECT    rect;                       // Window's position and size
} 	tmWND_2;

typedef struct  tagtmXYCOORD{
    short xcoord;
    short ycoord;
	} tm_xycoord;

typedef struct tagtmPARAGRAPH
{
    WORD    wAlign;
} tmPARAGRAPH;

typedef struct tagtmWNDATTR
{
//    COLOR       color[16];
    FGC         fgc;
    BGC         bgc;
    FGC         fgcBorder1;
    FGC         fgcBorder2;
    FGC         fgcBorder3;
    FGC         fgcBorder4;
    FGC         fgcShadow;           
    FGC         fgcTitle;
    BGC         bgcTitle;
    FGC         fgcEdit1;
    BGC         bgcEdit1;
    FGC         fgcEdit2;
    BGC         bgcEdit2;
    FGC         fgcOptionBox1;
    FGC         fgcOptionBox2;
    FGC         fgcOptionBox3;
    FGC         fgcOptionBox4;
    FGC         fgcCheckBox1;
    FGC         fgcCheckBox2;
    FGC         fgcCheckBox3;
    FGC         fgcCheckBox4;
    BGC         bgcMeter;
    BGC         bgcGauge1;
    BGC         bgcGauge2;
    BGC         bgcScrollBar;
    BGC         bgcListBox1;
    FGC         fgcListBox2;
    BGC         bgcListBox2;
    BGC         bgcChart;               //ST081099 UNMARK FOR Chart Background color 
    FGC         fgcChartScale1;
    FGC         fgcChartScale2;
    FGC         fgcChartScale3;
    FGC         fgcMenuBar;
    BGC         bgcMenuBar;
    FGC         fgcToolBar1;            // Foreground color of the tool bar itself
    BGC         bgcToolBar1;            // Background color of the tool bar itself
    FGC         fgcToolBar2;            // Foreground color of buttons on the toolbar
    BGC         bgcToolBar2;            // Background color of buttons on the toolbar
    FGC         fgcStatBar;
    BGC         bgcStatBar;
    FGC         fgcView;
    BGC         bgcView;
    
    FGC         fgcSelectEdit1;     //<LEO20060714 color for select edit
    BGC         bgcSelectEdit1;     //<LEO20060714 color for select edit
    FGC         fgcSelectEdit2;     //<LEO20060714 color for select edit
    BGC         bgcSelectEdit2;     //<LEO20060714 color for select edit
	WORD        wBorderWidthSelectEdit;//<LEO20060714 color for select edit
//    tmFONT       cf;
    tmPARAGRAPH   paragraph;    
    WORD        wBorderWidth;
    WORD        wBorderWidthStatic;
    WORD        wBorderWidthEdit;
    WORD        wBorderWidthOptionBox;
    WORD        wBorderWidthCheckBox;
    WORD        wBorderWidthMeter;
    WORD        wBorderWidthGauge;
    WORD        wBorderWidthGroupBox;
    WORD        wBorderWidthPanel;
    WORD        wBorderWidthButton;
    WORD        wBorderWidthScrollBar;
    WORD        wBorderWidthListBox;
    WORD        wBorderWidthCombo;
    WORD        wBorderWidthTable;
    WORD        wBorderWidthBitMap;
    WORD        wBorderWidthDrawing;
    WORD        wBorderWidthChart;
    WORD        wBorderWidthAnimator;
    WORD        wBorderWidthMsgBox;    
    WORD        wWidthFocus;
    WORD        wWidthCursor;	//PLOGFONT  	wfont;
    
    
}	tmWNDATTR;


typedef struct tagLISTCOORDINATE
{
	int 	nTop;
	int		nLeft;
	int		nRight;
	int 	nBottom;
}LISTCOORDINATE;
extern	LISTCOORDINATE	g_MemoCurCoordinate;
/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
/*
#define	Ellipse(Control, x1, y1, x2, y2)	_Ellipse(Control, x1, y1, x2, y2)
#define	Line(x1, y1, x2, y2)				_Line(x1, y1, x2, y2)
#define	LineTo(x, y)						_LineTo(x, y)
#define	Rectangle(Control, x1, y1, x2, y2)	_Rectangle(Control, x1, y1, x2, y2)
#define	SetColor(Color)						_SetColor(Color)
#define	SetViewPort(x1, y1, x2, y2)			_SetViewPort(x1, y1, x2, y2)
*/
#define     MKRECT(rect, l,t,r,b) \
            rect.left=l; rect.top=t; rect.right=r; rect.bottom=b;
#define     MKWND(wnd, title, style, l,t,r,b, hwnd, pfont) \
            wnd.pszTitle=title; wnd.wStyle=style; MKRECT(wnd.rect, l,t,r,b) \
            wnd.hwndParent=hwnd; wnd.pcFont=pfont;
#define     MKWND1(wnd, style, l,t,r,b, hwndmmi, pfont) \
			wnd.wStyle=style; MKRECT(wnd.rect, l,t,r,b) \
			wnd.hwndParent=hwndmmi; wnd.pcFont=pfont;
#define     MKWND2(wnd, style, l,t,r,b) \
            wnd.wStyle=style; MKRECT(wnd.rect, l,t,r,b)

#define     MKPARA(para, a)        para.wAlign=a;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - Wnd                                          |
+===========================================================================*/
class CtmWnd : public CObject
{
	/*---------------------------------------------------------------------------+
	|           Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/
	DECLARE_DYNAMIC(CtmWnd)
	public:
	    CtmWnd();
	    virtual ~CtmWnd();
	    
	/*---------------------------------------------------------------------------+
	|           Attributes                                                       |
	+---------------------------------------------------------------------------*/
	private:
	    static WORD         m_wWindowsCount;
	    static CtmWnd*      m_pwndList[tmMAX_WINDOWS+1];
	    long	m_lImageSize;														//<<yang 2005/11/23 
		char*	m_pImage;															//<<yang 2005/11/23 
	//	CtmBitMap*			m_pbmpTitle;
	//	static CtmBitMap*	m_pBmpBorder;
	
		
		//	fans add
		void*	m_plibHandle;		//	標記元件處理函數的句柄
		int		m_iTitleWidth;
		int		m_iTitleHeight;
		int		m_iStatusWidth;
		int		m_iMainWidth;
		int		m_iExTag;
	
	
	protected:
		//	fans add
		int			m_iMaxTab;
		int 		m_controlCount;
		char* 		m_typeName;
		char*		m_pszTitleID;		//	標題ID
		int			m_iFontType;		//	0:為字體16；1:為字體24；默認是0
		int			m_iFontUNType;
		int			m_iFontUNWidth;
		int			m_iFontUNHeight;
		
		CtmWnd*		m_parent;			//	fans add
		int			m_taborder;			//	fans add
		char*		m_pszLeftControl;	//	fans add	光標左移的時候,焦點需要到哪個元件
		char*		m_pszRightControl;  //	fans add	光標右移的時候,焦點需要到哪個元件
		char*		m_pszUpControl;     //	fans add	光標上移的時候,焦點需要到哪個元件
		char*		m_pszDownControl;   //	fans add    光標下移的時候,焦點需要到哪個元件
		int			m_iPrivilege;	    //	fans add	權限
		BOOL		m_bVisible;			//	fans add	可見
		BOOL		m_bEnabled;			//	fans add	可用
		BOOL		m_bDrawFocus;
	    static tmWNDATTR    m_wndattr;
	    static CtmWnd*      m_pwndCurrent;
		
	    static tmPARAGRAPH    m_paragraphLeft;
		
		short			m_nborder;
		short			m_nHeightTitle;
		RECT 			m_rectlBorder;
		RECT			m_rectrBorder;
		RECT			m_rectStatus;
		RECT			m_rectTitle;
		RECT			m_rectLogo;
	
	    tmWND           m_wnd;              // Window's basic parameter
	    tmHWND          m_hwndMe;           // Window's handle
	    RECT            m_rectWindow;       // Window's position and size in screen coordinate
	    RECT            m_rectClient;       // Client area position and size in screen coordinate
	    
	    WORD            m_wBorderWidth;     // The gap between m_rectWindow and m_rectClient
	    FGC             m_fgc;              // Foreground color
	    BGC             m_bgc;              // Background color
	    tmFONT         	m_Font;             // Font parameter
		CtmFont* 		m_pcFont;
	    tmPARAGRAPH     m_paragraph;        // Paragraph parameter
	
	    BOOL            m_bOnFocus;
	    BOOL            m_bChanged;         //St120100 move from CEdit for get Change status
		bool						m_bPropertyChanged; //	changed propertys
		int				m_nBgcColorControl;	
		int				m_nColorChange;	
		WORD			m_wDestColor;	
		
		BOOL			m_bArc;
		BOOL			m_bPreControl;
		
		WORD			m_wBorderColor;		//	fans add 2008/12/12 08:55上午
		
		//	fans add 2009-4-3 11:31:36
		//CtmAiOperation*	m_pAi;			
		//int				m_nAiType;	
		//char*			m_szAiWnd;
		//char*			m_szAiID;
		//long long		m_llAiValue;
		//long long		m_llAiValues[10];
		//long long 		m_llAidValue;
		//long long		m_llAivValue;
		//	fans add 2009-4-7 13:02:43
		int				m_nMsgID;
		int				m_nLParam;
		int				m_nWParam;
		
		char*			m_pszHelpStrID;
		char			m_pszHelpStrIDs[MAX_HELP_NUM][64];
		int				m_nHelpType;
	//	fans add
	protected:
		int 			PropCount;
		WORD 			wType;
		//CtmStrings		strings;
	//	fans add
	public:
		char 			Name[MAX_NAME];	//	name max length is 1024
		char 			TYPEName[MAX_NAME];				//	read only
		tmPropInfo 		Propertys[MAX_PROPERTY];		//	read only
		CtmWnd* 		Controls[MAX_CONTROLS];			//	read only
		
	/*---------------------------------------------------------------------------+
	|           Operations                                                       |
	+---------------------------------------------------------------------------*/
	//	fans add
	private:
		void 	InitProps();
	public:
	    BOOL    Create(tmWND* pwnd);
	
	    virtual void    Get()                       {};
	    virtual void    Set()                       {};
	    virtual void    SetAll()                    {Set();};
	    virtual void    Show();	//	fans add function content
	    virtual void    Update()                    {};
	    virtual	void	UpdateB(void* pData)		{};
	    virtual void    UpdateAll()                 {Update();};
	    virtual void    Redraw(RECT rectInvalid)    {};
	    virtual void    OnGetFocus()                {};
	    virtual void    OnLoseFocus()               {};
		virtual BOOL	MouseClick(short x_pos,short y_pos,int key_flag)  {return FALSE;};
		virtual BOOL	MouseMove(short x_pos,short y_pos) {return FALSE;};
		virtual BOOL	MouseMsg(WPARAM wParam,LPARAM lParam,int message) {return FALSE;};
	    virtual WORD    OnKey(WORD wKey)            {return FALSE;};
	    virtual WORD    OnCommand(WORD wCmd)        {return FALSE;};
	    virtual WORD    OnCommand(char* pszCmd)		{return FALSE;};	//	tong add
	    virtual WORD    OnChange(WORD wIDControl)   {return FALSE;};
	    virtual void    CalcRECT();
		//virtual void	WndProc(int message, WPARAM wParam, LPARAM lParam) {};
		virtual	void    DrawBorder();

		virtual	int		OnLose()					{OnLoseFocus(); return 0;};		//	fans add for the ok key 2010-1-15 15:46:54 
		virtual BOOL	OnFocus()					{ return m_bOnFocus; };
	    virtual	void    Clear();	// <FANS> 2006/11/28 04:13下午 modify
	    void    GetWindowAttribute(tmWNDATTR* pattr)  {*pattr = m_wndattr;};
	    void    SetWindowAttribute(tmWNDATTR* pattr)  {m_wndattr = *pattr;};
	    void    SetWindowFGC(FGC fgc)               {m_wndattr.fgc = fgc;};
	    void    SetWindowBGC(BGC bgc)               {m_wndattr.bgc = bgc;};
	    void    SetWindowFGCTitle(FGC fgc)          {m_wndattr.fgcTitle = fgc;};
	    void    SetWindowBGCTitle(BGC bgc)          {m_wndattr.bgcTitle = bgc;};
	    void    SetWindowParagraph(tmPARAGRAPH para)  {m_wndattr.paragraph = para;};
	
	    tmHWND  GetHandle()                         {return m_hwndMe;};
	    CtmWnd* GetWindow(tmHWND hwnd)              {return m_pwndList[hwnd];};
	    RECT    GetWindowRECT()                     {return m_rectWindow;};
	    RECT    GetClientRECT()                     {return m_rectClient;};
	    short   GetClientWidth()                    {return (m_rectClient.right-m_rectClient.left+1);};
	    short   GetClientHeight()                   {return (m_rectClient.bottom-m_rectClient.top+1);};
	    short   GetClientRight()                    {return (m_rectClient.right-m_rectClient.left);};
	    short   GetClientBottom()                   {return (m_rectClient.bottom-m_rectClient.top);};
		
		short 	GetmClientLeft()					{return m_rectClient.left;};
		short 	GetmClientTop()					 	{return m_rectClient.top;};
		short 	GetmClientRight()					{return m_rectClient.right;};
		short 	GetmClientBottom()					{return m_rectClient.bottom;};
	
	    FGC     	GetFGColor()                    {return m_fgc;};
	    BGC    		GetBGColor()                    {return m_bgc;};
	    BOOL    	GetChange()                     {return m_bChanged;};  
	    WORD    	GetStyle()                      {return m_wnd.wStyle;};    
		CtmFont*	GetFont()                       {return m_pcFont;};
		
	    void    SetFGColor(FGC fgc)                 {m_bPropertyChanged = TRUE;m_fgc = fgc;};
	    void    SetBGColor(BGC bgc)                 {m_bPropertyChanged = TRUE;m_bgc = bgc;};
		void    SetFont(CtmFont* pFont)				{m_pcFont = pFont;};
		void	SetFontFormat(WORD wFormat);
	    void    SetParagraph(tmPARAGRAPH paragraph) {m_paragraph = paragraph;};
	    void    SetBorderWidth(WORD wWidth)         {m_wBorderWidth = wWidth;};
	    void    SetStyle(WORD wStyle)               {m_wnd.wStyle   = wStyle;};
	    /*	fans mark
	    void    SetTitle();
	    void    SetTitle(char* pszTitle);
	    void	SetFormCaption();	//	fans add	    
		void    SetXPTitle(RECT rect);
		*/
		void	SetBorder();
	    void    ScreenToClient(RECT* rect);
	    void    SetClientRECT(short nLDelta, short nTDelta, short nRDelta, short nBDelta);
	   void     SetControlCountZero(CtmWnd* Parent);
	    
	    void    DrawGetFocus();
	    void    DrawLoseFocus();
	    void	DrawSplit(tm_xycoord xy);
		void	Show(char* pszString);
	    void    Show(short nX, short nY, char* pszString);
	    void    Show(short nY, char* pszString);
	    void    ShowFontStream(short nX, short nY, char* pszString, int nLen, int nType);		//rw.ying 2011/1/13 10:13上午
		short   ShowExtent(short nX, short nY, char* pszString, int nType = 3);
	    //add by J.Wong for unicode 2016/7/19 11:10:33
	    short	ShowExtent(short nX, short nY, unsigned int* pdwUnicode,int nLen);
	    void    ShowExtent(          short nY, char* pszString, int nType = 3);
	    void	ShowExtent_UN(short nY, char* pszString, int nType = 1);
	    void    Clear(RECT rect);
	
	    void    Move(short nX, short nY);
	    void    Size(short nW, short nH);
	
	    tmSIZE	GetStringExtent(char* pszString);
	    tmSIZE	GetUNStringExtent(char* pszString);
	    void    ClearBorderType(WORD Type)    {m_wnd.wStyle &= ~Type;};  
	    void    SetBorderType(WORD Type)      {m_wnd.wStyle |=  Type;};  
	    
	    BOOL	ptInRect(short x, short y, RECT *prect);
	    
	    
	    //	fans add
	    CtmWnd* GetParent();
		void 	SetParent(CtmWnd* Parent);
		int 	GetControlCount();
		
		CtmWnd*	GetControlA(int index);
		BOOL	GetVisible() {return m_bVisible;};	
		BOOL	GetEnabled() {return m_bEnabled;};
		
		virtual int 	GetPropCount();
		virtual void SetPropertys();		
		virtual void SetPropertys_Nub();
		virtual tmPropInfo* GetProperty(char* pszPropName);//	根據屬性名稱而獲取相關的屬性信息，可以對其進行修改，但更改名稱不可以變得比原有長
		virtual	tmPropInfo*	GetProperty(int nNub);
		virtual	void* GetPropValue(char* pszPropName);		//	根據屬性名稱而獲取相關的屬性值
		virtual	void* GetPropValue(int nNub);	
		virtual	bool SetProperty(char* pszPropName, char* pszValue);//	根據屬性名稱而對屬性進行相關的設置
		virtual	bool SetProperty(int nNub, char* pszValue);
		virtual	bool SetProperty(char* pszPropName, void* pValue);//	根據屬性名稱而對屬性進行相關的設置
		virtual	bool SetProperty(int nNub, void* pValue);
		
		virtual BOOL CreateA()	;	//	fans add function content
		
		virtual	bool SetPropValueT(char* pszPropName, double Value);
		virtual	bool SetPropValueT(int nNub, double Value);
		virtual bool SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType);		//rw.ying 2011/3/17 03:13下午
		//bool SetPropValueTD(char* pszPropName, double Value);
		virtual	bool SetPropValueT(char* pszPropName, char* pszValue);
		virtual	bool SetPropValueT(int nNub, char* pszValue);
		virtual	bool GetPropValueT(char* pszPropName, void* pValue, int nDataSize = -1);
		virtual	bool GetPropValueT(int nNub, void* pValue, int nDataSize = -1);
		
		
		CtmWnd*	FindControlFromTab(int iTabOrder);
		CtmWnd*	FindControlFromName(char* pszName);
		//	fans add:set/get taborder
	    int		GetTabOrder() 				{return m_taborder;};
	    void	SetTabOrder(int iTabOrder)  {m_taborder = iTabOrder;};
	    bool	Is(char* pszType);
	    void	SetLibHandle(void* pLib);				//	設置lib 的句柄
	    void*	GetLibHandle();							//	得到lib 的句柄
	    int		FindMaxTabOrder();
	    int		FindControl(CtmWnd* pControl);	//	確定給定的pControl是否在子控件列表中
	    void	CopyBg();														//<<yang 2005/11/23 
		void	ShowBg();														//<<yang 2005/11/23 
		void	FreeBg();														//<<yang 2005/11/23 
		BOOL	HasOnlineHelp();
		void	SetHelpStrID(char* pszHelpIDs, int nIndex);
		char*	GetHelpStrID(int nIndex);
		virtual void DrawOnlineHelp(BOOL bShow = TRUE);
		 
		//CtmAiOperation*	GetAi() {return m_pAi;};										//	fans add 2009-4-3 11:32:35
	
	/*---------------------------------------------------------------------------+
	|           Helpers                                                          |
	+---------------------------------------------------------------------------*/
	protected:
	    void    DrawFocus();
	    void 	ShowExtent1(short nY, char* pszString, int nType = 3);
		
	//	fans add
	protected:
		virtual	void 	SetPropInfo(char* pszName, PropType PType, void* PropAddress);
		virtual	void 	SetPropInfo(int nNub, PropType PType, void* PropAddress);
		virtual	int		FindProperty(char* pszPropertyName);
		virtual	int		FindProperty(int nNub);
		
		virtual void SetBaseInfo();	
		virtual void SetTypeName(char* pszName);
	    void	StrIDToStr(char* pszID, char** psz, BOOL bOnlineHelp = FALSE);
	 
};

#endif
