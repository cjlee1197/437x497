/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 
/*===========================================================================+
|  Class    : Histogram			                                             |
|  Task     : Histogram header file                                           |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 4.3.2                                                  |
|  Link     :G++(GCC) 4.3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Caozz	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 06/28/2012                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_DHISTOGRAM
#define		D_DHISTOGRAM

#include	"wnd.h"
#include    "wndx2.h"
#include	"static.h"
#include	"gui_ex.h"
#include	"curve.h"
#include	"edit.h"
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

#define		MAX_GRAPH_NUM			128

#define		DEFAULT_BGC				0xFFFF
#define		DEFAULT_GRAPHCOLOR		38395
#define		DEFAULT_GRAPHWIDTH		10
#define		DEFAULT_SHOWXCNT		10
#define		DEFAULT_GRIDCOL			5
#define		DEFAULT_GRIDCOLOR		33840


typedef struct tagtmGraphValue
{
	WORD	wIndex;
	double	dValue;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmGraphValue;
#else 
#ifdef	D_X86
tmGraphValue;
#endif
#endif

/*===========================================================================+
|           Class declaration - CtmHistogram                                 |
+===========================================================================*/
class	CtmHistogram: public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmHistogram)
	public:
		CtmHistogram();
		~CtmHistogram();
	private:
	    int						m_nHistogramType;  		//0=堅向, 1=橫向
		                		
		DWORD					m_dwGraphColor;			//柱顏色
		WORD					m_wGraphType;			//柱wtype
		int						m_nGraphWidth;			//柱寬度  
		WORD					m_wShowXCnt;			//顯示柱個數
		WORD					m_wXStartIndex;			//X軸起始Index
		                		
		BOOL					m_bShowValue;			//是否顯示數字	
		WORD					m_wXDigits;				//X軸顯示精度
		WORD					m_wYDigits;				//Y軸顯示精度
		BOOL					m_bShowGrid;
		WORD					m_wGridCol;				//顯示網格行數
		DWORD					m_dwGridColor;			//網格顏色
		DWORD					m_dwXAxesColor;
		DWORD					m_dwYAxesColor;
			                		
		char*					m_pszGraphNameID;
		char*					m_pszGraphName;
		char*					m_pszXName;
		char*					m_pszYName;
		char*					m_pszXNameID;
		char*					m_pszYNameID;
		BOOL					m_bXName;
		BOOL					m_bYName;
		                		
		tmGraphValue			m_GraphValue[MAX_GRAPH_NUM];
		CtmStaticX2**			m_pGraphStaticX2;
		CtmStaticX2**			m_pStrStaticX2;
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		//===========
		//	the operations inherit from CtmWnd
		//===========
		BOOL 	CreateA();
		void 	SetPropertys();	
		void	Show();
		void	Update();
		
		void	SetGraphValue(tmGraphValue* pGraphValue, int nCount);
		
	protected:
		RECT	GetTitleRect();					//	Title的範圍           
		RECT	GetCutRect();					//	獲得圖例的範圍        
		RECT	GetXNameRect();					//	X軸名稱的範圍
		RECT	GetXAxesScaleRect();			//	獲取X軸的座標值的範圍 
		RECT	GetYNameRect();					//	Y軸名稱的範圍         
		RECT	GetYAxesScaleRect();			//	獲取Y軸的座標值的範圍 
		RECT	GetLineRect();					//	獲取真正的畫圖範圍    
	
		void	SeperateArea();					//	分割畫圖的區域        
		
		void	DrawTitle();
		void	DrawAxesGrid();
		void	DrawAxes();
		
		void	DrawName();	
		void	DrawXName();
		void	DrawYName();
			
		void	DrawSclae();
		void	DrawXScale();
		void	DrawYScale();
		void	DrawCoordinate();
		
		int		GetRangeXY();					// 取X,Y軸最大值最小值
		void	PropareShow();
		void	ShowTitle();
		short   ShowExtentD(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		short	ShowExtentEx(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		
		void	ShowGraph();
		
	private:
        void    InitData();
		BOOL	FindRelationFirst(tmRectRelation* pSource, tmRectRelation** pDest);
		BOOL	FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest);
	
		BOOL	AddTopRelation(tmRectRelation* pSource, int nHeight);
		BOOL	AddDownRelation(tmRectRelation* pSource, int nHeight);
		BOOL	AddLeftRelation(tmRectRelation* pSource, int nWidth);
		BOOL	AddRightRelation(tmRectRelation* pSource, int nWidth);
		RECT	FindLast(tmRectRelation* pSource);
		int		GetLastLeft();
		int		GetLastRight();
		int		GetLastTop();
		int		GetLastBottom();
		
		public:
		BOOL	MouseUp(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16 
		BOOL	MouseDown(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16
		BOOL	MouseMove(short x_pos,short y_pos,int key_flag);
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);	// fans 2008/6/16 11:29上午
		
		private:
		
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	protected:
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		double	m_dXMin, m_dXMax, m_dYMin, m_dYMax;	// X軸和Y軸顯示最大值最小值
		double	m_dXUnit, m_dYUnit; 				// 每個點代表的值。
		
		int		m_nXCoordinateColor;
		int		m_nYCoordinateColor;
		int		m_nXAxesColor;
		int		m_nYAxesColor;
		
		BOOL				m_bTouchSupport;
	private:
		RECT	m_rectTitle;
		RECT	m_rectXName;	
		RECT	m_rectXAxesScale;				//
		RECT	m_rectYName;		
		RECT	m_rectYAxesScale;				//	
		RECT	m_rectLine;
		RECT	m_rectCoordinate;					// 相對於元件rectClient的位置	
		
		tmRectRelation	m_relationTitle;        
		tmRectRelation	m_relationXName;	    
		tmRectRelation	m_relationXAxesScale;	
		tmRectRelation	m_relationYName;		
		tmRectRelation	m_relationYAxesScale;	
		tmRectRelation	m_relationLine;         
		
		
		
		
		
		BOOL	m_bTitle;						//	the on-off for the title
		BOOL	m_bCutLine;
		//RECT	m_rectCoordinate;				//	
		
		tmRectRelation	m_relationLeft;
		tmRectRelation	m_relationRight;
		tmRectRelation	m_relationUp;
		tmRectRelation 	m_relationDown;
};

#endif
