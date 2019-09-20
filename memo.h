/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : ScrollBar                                                      |
|  Task     : scroll bar header file                                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                         |
|  Version  : V1.00                                                          |
|  Creation : 20/10/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef 	D_TMMEMO
#define		D_TMMEMO

#include 	"wnd.h"
#include	"scrollbar.h"
#include	"strlist.h"		//Mario debug
#include	"task.h"		//	include the taskCursor	//Mario debug
#include	"imagebox.h"


#define	MEMO_UNIT_HEIGHT		(m_pcFont->m_wHeight + 10)

#define	MEMO_TYPE_PLAIN_TEXT	1
#define	MEMO_TYPE_IMAGE			2
#define	MEMO_TYPE_TEXT_IMAGE	3
	
#define	MEMO_MOUSE_MOVE_RANGE	10		//越小感度越高，容易誤觸
#define	MEMO_MOUSE_MOVE_RANGE1	3		//小動作越順暢，但畫面更新更頻繁(LAG)
#define	MEMO_MOUSE_MOVE_RANGE2	5		//該行完全被遮蔽時，最少必須顯示多少字元		
#define AUTO_MODE_SELECT_LINE		10

const	int	CNT_INT_MEMO_MAX	= 20;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

typedef struct tag_memo_copy
{
	char* str ;
	int str_len;
	tag_memo_copy* next ;
}memo_copy;

/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- ScrollBar                                    |
+============================================================================*/
class CtmMemo :public CtmWnd
{
	DECLARE_DYNCREATE(CtmMemo)
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmMemo();
		~CtmMemo();
	
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:   
	    char*	m_pszShow[CNT_INT_MEMO_MAX];		//	fans add,
		char*	m_pszIDShow[CNT_INT_MEMO_MAX];		//	fans add,
		char*	m_pszStream[CNT_INT_MEMO_MAX];
		WORD	m_wszStreamLen[CNT_INT_MEMO_MAX];			// rw.ying 2011/3/17 03:36下午
		WORD	m_wStreamType[CNT_INT_MEMO_MAX];
		char*	m_pszImagePath[CNT_INT_MEMO_MAX];
		CtmImageBox *m_pImages[CNT_INT_MEMO_MAX];
		WORD	m_wMemType;
		DWORD	m_dwKeyColor;
		
		BOOL	m_bCopyBg;
		WORD	m_wStartStrIndex;					//	
		WORD	m_wFocusColor;
		WORD	m_wFocusIndex;						//	the memo's index, and is not Strings Index
		WORD	m_wOldFocusIndex;					//
		WORD	m_wFocusStrIndex;
		
		CtmScrollBar	m_scrollbar;
		BOOL	m_bBar;
		
		int		m_nEntWidth;
		int		m_nEntHeight;
		
		BOOL	m_bCalcSize;
		
		BOOL	MoveButtonDownFalg;	//Mario add 2015/1/20 上午 11:36:46
		WORD	Down_Y;							//Mario add 2015/1/20 上午 11:36:56
		
	protected:
		BOOL	m_bFocus;
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	private:
		void		ShowScrollbar();
		void		UpdateScrollbar();
		void		SetInitScrollbar();
		void		SetScrollbar(WORD wRange);
		void		ScrollUp();
		void		ScrollDown();
		
		CtmBitMap*	GetBitMap(char *imagePath);
		void		CalcSize();
	protected:
		void	GetShowText();
		void	ShowImages();
		void	DelImages();
		void	ShowStrs(WORD wOldFocusIndex, WORD wNewFocusIndex);
		void	SetFocusRow(WORD wOldFocusIndex, WORD wNewFocusIndex);
		WORD	GetMaxRows();
		WORD	GetStrsNum();
	public:
	/**/void    Show();
		void    Update();
		BOOL 	CreateA();	
		void 	SetPropertys();
		void 	SetPropertys_Nub();
		WORD    OnKey(WORD wKey);
		void    DrawBorder();
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		BOOL	MouseDown(short x_pos,short y_pos,int key_flag);
		BOOL	MouseUp(short x_pos,short y_pos,int key_flag);
		BOOL	MouseMove(short x_pos,short y_pos,int key_flag);
		void	SetFontLen(WORD wOffset, int nFontlen, int nType);		//rw.ying
		//↓Mario add 20150114
		WORD  GetStartStrIndex();
		
};

//	the new compoment for cnc
class	CtmCustomMemo: public CtmWnd
{
	DECLARE_DYNCREATE(CtmCustomMemo)
	
	public:
		CtmCustomMemo();
		~CtmCustomMemo();
	
	protected:
		RECT			m_MajorRect;
		CtmStringList	m_strlist;
		
	public:
		virtual	int		LoadFromFile(const char* pszFile,int nStartRow =0, BOOL bLoadOnceFlag = TRUE)	{return m_strlist.LoadFromFile(pszFile,nStartRow,bLoadOnceFlag);};
		virtual	int		SaveToFile(const char* pszFile)		{return m_strlist.SaveToFile(pszFile);};
};



const	int	CNT_INT_MEMO_MODE_DISABLE	= 0;			//	the memo cann't be modify, just see it
const	int	CNT_INT_MEMO_MODE_INSERT 	= 1;			//	the memo is in insert mode
const	int	CNT_INT_MEMO_MODE_COVER	 	= 2;			//	the memo is in cover mode
//	just text
class	CtmTextMemo: public CtmCustomMemo
{
	DECLARE_DYNCREATE(CtmTextMemo)
	
	public:
		CtmTextMemo();
		~CtmTextMemo();
		
	public:
		BOOL		CreateA();
		WORD		OnKey(WORD wKey);	
		void 		SetPropertys();
		
		void		Update();
		void		Show();
		void		OnGetFocus();
		void		OnLoseFocus();
			
		void		OnPaint();
		void		OnPaint2();
		void		PaintSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol, DWORD dwColor);
		void		DelSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol);	//Sunny<20110609> add
		void		PaintLine(int nIndex);
		void		PaintRow(int nRow);
		void		GotoLine(int nIndex);
		int			Find(char* psz);
		int			Find(int nStartIndex, int nEndIndex, char* psz);
		
		//	new method
		int			GetStrNum();
		int			GetCurrentCol();
		int			GetCurrentRow();
		int			GetCurentStrCol();
		int			GetCurrentStrRow();
		int			GetContainRow();
		int			GetContainCol();
		void		DelRow(int nIndex);//20110519
		
		const char*	GetCurrentString();				//	get the string that have the currsor
		const char*	GetVisibleString(int nRow);		//	get the string in the memo view
		const char*	GetString(int nIndex);			//	get the string in the list(not view)
		
		//New add by Roy↓
		void		AddNewStr(int nIndex, char* pszStr){m_strlist.Insert(nIndex, pszStr);};
		int			AppendStr(int nIndex, char* pszStr){m_strlist.AppendStr(nIndex, pszStr);};
		void		GetStr(int nIndex, char* pszStr);
		//void		Modify(int nIndex, char* pszStr){m_strlist.Modify(nIndex, pszStr);};
		//void		Del(int nIndex){m_strlist.Del(nIndex);};
		//string* at(int nIndex){return m_strlist.at(nIndex);};
		//string* 	assign(int nIndex){return m_strlist.assign(nIndex);};
		//20140530↑
		
		//Mario add 20150115
		
		
	protected:
		void		SetCurrsorPos(int nCol, int nRow);
		void		ShowCurrsor(BOOL bOn);
		void		EnableCurrsor();
		void		DisableCurrsor();
		
		void		TransPos(int nCol, int nRow, int* pOutX, int* pOutY);
		
		void		Insert(WORD wKey);
		WORD		Control(WORD wKey);
		
		
		void		AddNewStr(char* pszStr);
		char*		AppendChar(const char* pszStr, char sz);
		void		PaintStr(int nRow, int nHideCol, const char* psz);
		
		
		void		CorrectCurrsorCurrentPos();
		void		CalcReg();
		
		void		InsertNewLine();
		void		ClearLine(int nRow, int nStartCharIndex, DWORD Color, int nNum);
		
		
		void		MoveCurrsorR();
		void		MoveCurrsorL();
		void		MoveCurrsorU();
		void		MoveCurrsorD();
		void		MoveCurrsorEnd();
		void		MoveCurrsorHome();
		void		PageDown();
		void		PageUp();
		
		int			GetStrLen(int nIndex);
		
		void		DelFrontChar();
		void		DelBackChar();
		
		//void		PaintSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol, DWORD dwColor);
		bool		IsSelectState();
		void		CancelSelectState();
		
		
		void		FillLine(int nRow, int nStartCol, int nEndCol, DWORD dwColor);
		void		FillLine(int nRow, DWORD dwColor);

	private:	
		void		ShowScrollbar();
		void		UpdateScrollbar();
		void		SetInitScrollbar();
		void		SetScrollbar(WORD wRange);
		void    DrawBorder();	//20111010
		
	protected:
		int						m_nContainRow;				//	
		int						m_nContainCol;
		CTaskCursor*	m_ptaskCurrsor;
		int						m_nCurrentCol;
		int						m_nCurrentRow;
		
		int						m_nHideCol;
		int						m_nHideRow;
		
		int						m_nMode;
		
		int						m_nSelectStartRow;
		int						m_nSelectStartCol;
		int						m_nSelectEndRow;
		int						m_nSelectEndCol;
		DWORD					m_dwSelectColor;
		DWORD					m_dwLineColor;
		DWORD					m_dwLineFGC;
		
		int						m_nRowWithColor;
		int						m_nUserRowWithColor;
		
		RECT    			m_VisualRect;
		int						m_nInputMode;
		
	  CtmScrollBar	m_scrollbar;
	  DWORD					m_scrollbar_bgc;
		BOOL					m_bBar;
		WORD					m_wStartStrIndex;
		bool					m_bLoadFullFile;
		bool					m_bEditFlag;
		int						m_nCurrsorCorrect;
		
		bool					m_bPaintCurrentRowFlag;		//20140530 Roy add for CtmLTextMemo.PaintCurrentRow
		
		friend class CtmLTextMemo;
		friend class CtmMTextMemo;
		friend class CtmFileExplorer;
		friend class CtmCncDialog;
		
};

//Mario add 20150114
class	CtmMTextMemo: public CtmWnd		
{
	DECLARE_DYNCREATE(CtmMTextMemo)
	
	public:
		CtmMTextMemo();
		~CtmMTextMemo();
		
	public:
		BOOL 			CreateA();
		WORD			OnKey(WORD wKey);	
		void    	Show();
		void    	Update();
		void    	Update2();
		void 			SetPropertys();	
	
		void			OnGetFocus();
		void			OnLoseFocus();
	
		int				LoadFromFile(const char* pszFile, int nStartRow =0, BOOL bLoadOnceFlag = FALSE);
		int				SaveToFile(const char* pszFile);
		void			GotoLine(int nIndex);
		int				Find(char* psz);
		int				Find(int nStartIndex, int nEndIndex, char* psz);
		void			DelRow(int nIndex);//20110519
		void			PaintSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol, DWORD dwColor);
		
		void			DelSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol);
	
		void			MoveCurrsorR(){m_ContentMemo.MoveCurrsorR();};		//20110825 add
		void			MoveCurrsorL(){m_ContentMemo.MoveCurrsorL();};		//20110825 add
		int				GetStrNum(){return m_ContentMemo.GetStrNum();};		//20110927 add
		
		//New add by Roy↓
		int				GetCurrentCol(){m_ContentMemo.GetCurrentCol();};
		int				GetCurrentRow(){m_ContentMemo.GetCurrentRow();};
		
		//void			Modify(int nIndex, char* pszStr){m_ContentMemo.Modify(nIndex, pszStr);};
		//void			OnPaint(){m_ContentMemo.OnPaint();};
		//void			Del(int nIndex){m_ContentMemo.Del(nIndex);};
		//string* 	at(int nIndex){return m_ContentMemo.at(nIndex);};
		//string*		assign(int nIndex){return m_ContentMemo.assign(nIndex);};
		
		//ROY：將當前光標所在row的背景印色
		void			PaintCurrentRow(int nRow, DWORD dwColor);
		
		//void			PaintRow(int nRow){m_ContentMemo.PaintRow(nRow);};
		//void			FillLine(int nRow, DWORD dwColor){m_ContentMemo.FillLine(nRow, 0, m_ContentMemo.GetContainCol(), dwColor);};
		//20140529↑
	
	
		
		void			AddNewStr(char* pszStr);
		void			AddNewStr(int nIndex, char* pszStr,BOOL AddHide=TRUE);
		void			GetStr(int nIndex, char* pszStr,BOOL AddHide=TRUE);
		void			GetStr(char* pszStr);
		void			AddNewRow();
		int				AppendStr(int nIndex, char* pszStr){m_ContentMemo.AppendStr(nIndex, pszStr);};
		void   		SetSelectWithFileLine(int SelectFileLine ,int color ,int SelectOnMemoLine);
		//20140530↑
		
		void			DelRow_New(int start,int end);		//ROY：原先的DelRow()有刪除後直接存檔的功能，此為不存檔的版本
		//20140703↑
		//void			CancelSelectState(){m_ContentMemo.CancelSelectState();};
		//20140704↑
		
		//↓Mario add
		//20141230	
		void		WndProc(int message, WPARAM wParam, LPARAM lParam);
		BOOL		MouseDown(short x_pos,short y_pos,int key_flag);
		BOOL		MouseUp(short x_pos,short y_pos,int key_flag);
		//20150115
		BOOL		MouseMove(short x_pos,short y_pos,int key_flag);
		int	SetSelectStartLine(int start);
		int	SetSelectEndLine(int end);
		int	SetSelectRegion(int start , int end);
		void	SetSelectPtr(int* start , int* end);
		int* SetSelectStartPtr(){return &m_ContentMemo.m_nSelectStartRow;};
		int* SetSelectEndPtr(){return &m_ContentMemo.m_nSelectEndRow;};
		int	 CopyLine();
		int	 PatseLine();
		
		
	protected:
		void			InitContentMemo();
		void			InitLineNoMemo();
		void			LineNoMemoPaint();
	
	protected:
		CtmTextMemo		m_ContentMemo;
		CtmTextMemo		m_LineNoMemo;
		
		bool			m_bLineNoVisable;
		int				m_nLineNoLen;
		/*const*/ char* m_pszFile;//20110810 add
		int				m_nStartIndex;
		char*			m_pszPrompt;	//20120110 add
		
		//Mario add20150115
		BOOL					MoveButtonDownFalg;		//捲動元件是否被壓下
		WORD					Down_Y;								//按在捲動元件時的Y座標值
		//Mario add 20150116
		BOOL					MoveMemoStrFalg;		  //Memo元件是否被壓下
		WORD					Down_X;								//按在捲動元件時的Y座標值
		BOOL					StrMoveFlag;					//突破MEMO_MOUSE_MOVE_RANGE限制，開啟觸控功能
		int 					NowMaxCol;
		memo_copy*			copy_start;
		
		
};




#endif
