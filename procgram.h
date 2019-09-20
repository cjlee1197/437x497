/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 
/*===========================================================================+
|  Class    : CtmProcgram		                                             |
|  Task     : CtmProcgram header file                                         |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 4.3.2                                                  |
|  Link     :G++(GCC) 4.3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Wong                                                         	 |
|  Version  : V1.00                                                          |
|  Creation : 01/05/2015                                                     |
|  Revision :                                                                |
+===========================================================================*/


#ifndef     D_DPROCGRAM
#define		D_DPROCGRAM


#include	"wnd.h"
#include    "wndx2.h"
#include	"static.h"
#include	"gui_ex.h"
#include	"curve.h"
#include	"edit.h"
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

#define		MAX_ACTIONS_NUM			20
#define		MAX_ELEMENTS_CNT		10
#define		MAX_GRAPHCOLOR_CNT		3			

#define		DEFAULT_BGC				0xFFFF
#define		DEFAULT_GRAPHCOLOR0		12345
#define		DEFAULT_GRAPHCOLOR1		0xFFFF
#define		DEFAULT_GRAPHWIDTH		10
#define		DEFAULT_SHOWXVALUE		10
#define		DEFAULT_GRIDCOL			5
#define		DEFAULT_GRIDCOLOR		33840
#define		PER_UNIT				0.5

typedef struct tagColumsAttr
{
	char* pszYName;
	char* pszYNameID;
}COLUMSATTR;

typedef struct tagtmProcValue
{
	double	dValue[MAX_GRAPHCOLOR_CNT];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmProcValue;
#else 
#ifdef	D_X86
tmProcValue;
#endif
#endif

/*===========================================================================+
|           Class declaration - CtmProcgram                                 |
+===========================================================================*/
class	CtmProcgram: public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmProcgram)
	public:
		CtmProcgram();
		~CtmProcgram();
	private:
	    int						m_nHistogramType;  		//0=堅向, 1=橫向
		                		
		DWORD					m_dwGraphColor[MAX_GRAPHCOLOR_CNT];			
		DWORD					m_dwOldGraphColor[MAX_GRAPHCOLOR_CNT];
		DWORD					m_dwRefDestColor[MAX_GRAPHCOLOR_CNT];
		DWORD					m_dwRefColorControl;
		DWORD					m_dwRefColorChange;
		WORD					m_wGraphType;			//柱wtype
		int						m_nGraphWidth;			//柱寬度  
		WORD					m_wXStartIndex;			//X軸起始Index
		                			
		WORD					m_wXDigits;				//X軸顯示精度
		BOOL					m_bShowGrid;
		WORD					m_wGridCol;				//顯示網格行數
		WORD					m_wGridRow;				//筵釴梓煦賃跺杅
		WORD					m_wRefCount;
		DWORD					m_dwGridColor;			//網格顏色
		DWORD					m_dwXAxesColor;
		DWORD					m_dwYAxesColor;
			                		
		char*					m_pszGraphNameID;
		char*					m_pszGraphName;
		char*					m_pszLastNameID;
		char*					m_pszRefNameID[MAX_GRAPHCOLOR_CNT];
		char*					m_pszLastName;
		char*					m_pszRefName[MAX_GRAPHCOLOR_CNT];
		char*					m_pszXName;
		char*					m_pszXNameID;
		COLUMSATTR				m_pszYName[MAX_ACTIONS_NUM];
		BOOL					m_bXName;
		BOOL					m_bYName;
		                		
		tmProcValue				m_tmStartValue[MAX_ACTIONS_NUM][MAX_ELEMENTS_CNT];
		tmProcValue				m_tmEndValue[MAX_ACTIONS_NUM][MAX_ELEMENTS_CNT];
		CtmStaticX2**			m_pRefStaticX2[MAX_GRAPHCOLOR_CNT];
		CtmStaticX2*			m_pRefColor[MAX_GRAPHCOLOR_CNT];
		
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
		
		void	SetProcValue(int nType,tmProcValue* tmStartValue,tmProcValue* tmEndValue,int nElements);
		
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
		void	DrawTimergrambar();
		
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
		
		tmRectRelation	m_relationLeft;
		tmRectRelation	m_relationRight;
		tmRectRelation	m_relationUp;
		tmRectRelation 	m_relationDown;
};

#endif