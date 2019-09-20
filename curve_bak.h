/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                      |
|  Version  : V1.00                                                          |
|  Creation : 22/08/2005                                                     |
|  Revision : netdigger v2.0  2006/4/17                                      |
+===========================================================================*/
#ifndef	D_DCURVE
#define	D_DCURVE

#include	"wnd.h"
#include	"wndx2.h"
#include	"gui_ex.h"
 
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
#define			CURVE_LINE_MAX			10		// 最多的曲線數目
#define			CURVE_BUFFERLINE_MAX	100
#define			CURVE_BEELINE			0x00000001		// 直線
#define			CURVE_CURVELINE			0x00000002		// 曲線

/*
#define			CURVE_SHOWXNAME			0x00000001		// 顯示X軸名稱
#define			CURVE_SHOWYNAME			0x00000002		// 顯示Y軸名稱
*/
#define			CURVE_XCONTRARY			0x00000001		// X軸從大到小

#define			CURVE_CUTLINE_RIGHT		0x00000010		// 在右邊顯示圖例
#define			CURVE_CUTLINE_TOP		0x00000020		// 在上邊顯示圖例
#define			CURVE_CUTLINE_LEFT		0x00000040		// 在左邊顯示圖例
#define			CURVE_CUTLINE_BOTTOM	0x00000080		// 在下邊顯示圖例

#define			CURVE_NOTDRAW			0x00000100		// 不畫曲線

//	
#define			CURVE_COOR_BROKENLINE	0x1000			//	the coordinate is real line
#define			CURVE_RULE_REALLINE		0x2000			//	the RULE is broken line

#define			CURVE_LINETYPE			0x000000FF		// 曲線標識

const	int		CURVE_XXPOS_TOP			= 0x00000010;	//	top
const	int		CURVE_XXPOS_BOTTOM		= 0x00000020;	//	bottom
const	int		CURVE_XXPOS_LEFT		= 0x00000040;	//	left
const	int		CURVE_XXPOS_RIGHT		= 0x00000080;	//	right

const	int		CURVE_VALUE_NOTHING	 	= 0x00000000;		//	show nothing
const	int		CURVE_VALUE_ALL	 		= 0x00000001;
const	int		CURVE_VALUE_LIMIT	 	= 0x00000002;		//	show the min  and max 
const	int		CURVE_VALUE_MINONLY 	= 0x00000003;		//	only show the min vlaue
const	int		CURVE_VALUE_MAXONLY 	= 0x00000004;		//	only show the max value
                                          
//const	int		CURVE_XAXES_POS_NOTHING = 0x00000000;		//	don't show the x axes
//const	int		CURVE_XAXES_POS_TOP	 	= 0x00000001;		//	show the X axes on the top of the curve
//const	int		CURVE_XAXES_POS_BOTTOM	= 0x00000002;		//	show the X axes on the bottom of the curve
//                                          
//const	int		CURVE_YAXES_POS_NOTHING	= 0x00000000;
//const	int		CURVE_YAXES_POS_LEFT	= 0x00000003;		//	show the y axes on the left of the curve
//const	int		CURVE_YAXES_POS_RIGHT	= 0x00000004;		//	show the y axes on the right of the curve

const	int		CURVE_ARROW_LNE			= 8;
const	int		CURVE_ARROW_INCLINE		= 2;


const	int 	MAXPOINT = 600;//一個線所能允許的最大點數	fans add
/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef	struct tagCURVEPOINT
{
	double	dX;
	double 	dY;
}	CURVEPOINT;

typedef	struct tagLINE
{
	DWORD	dwColor;		// 曲線的顏色
	int		nFlag;			// 曲線類型
	char	*pszName;		// 曲線名稱
	int		nRefernce;		//	2008/4/2 10:51上午	曲線的坐標系	fans add
	union
	{
		struct
		{
			CURVEPOINT	PTStart;		// 直線的起始點
			CURVEPOINT	PTEnd;			// 直線的終點
		};
		struct
		{
			int 		nCount;			// 曲線點的數目
			CURVEPOINT 	*pValue;		// 曲線點的數組的指針
		};
	};
}	LINE;

typedef struct tagLineBuffer
{
	DWORD	dwColor;
	int		nRefernce;		//	2008/4/2 10:51上午	曲線的坐標系	fans add
	struct
	{
		int 		nLineCount;			//	標記linebuffer中有多少條曲
		int			nIndex;				//	當需要的時候，標記著是對Buffer中的那條曲?進行了操作
	};
	struct
	{
		int			nCount[CURVE_BUFFERLINE_MAX];	//	每條曲?的有效點數,其總數是MAXPOINT是一個常數
		CURVEPOINT* pValue[CURVE_BUFFERLINE_MAX];	//	具體的坐標
	};
}	LINEBUFFER;

typedef	struct tagCURVE
{
	int			nFlag;					// 曲線標識
	int			nXDigits;				// X軸小數位數
	int			nYDigits;				// Y軸小數位數
	int			nCol;					// 顯示列數
	int			nRow;					// 顯示行數

	char		*pszName;				// 圖表名稱
	char		*pszXName;				// X軸名稱
	char		*pszYName;				// Y軸名稱
	
	LINE		*pLine[CURVE_LINE_MAX];
}	CURVE;

//	fans add
typedef struct tagtmPoint
{
	int ix;
	int iy;
}tmPoint;


typedef	struct	tagtmRectRelation
{
	RECT* 				pRect;
	tagtmRectRelation*	pPre;
	tagtmRectRelation*	pNext;
	//int					nPreRelationType;	//CURVE_XXPOS_TOP			= 0x00000010;	//	top     
	//int					nNextRelationType;  //CURVE_XXPOS_BOTTOM		= 0x00000020;	//	bottom  
	//                                        //CURVE_XXPOS_LEFT		= 0x00000040;	//	left    
	//                                        //CURVE_XXPOS_RIGHT		= 0x00000080;	//	right   
	
}	tmRectRelation;


/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
typedef	int	(* pFuncShowRuleStr)(double, char*, double*);
/*===========================================================================+
|           Class declaration - CtmCurve                                     |
+===========================================================================*/
class	CCurve	:	public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CCurve)
	public:
		CCurve(tmWND* pwnd, CURVE* pCurve);
		~CCurve();
		
		CCurve();	//	fans
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		double				m_dXMin, m_dXMax, m_dYMin, m_dYMax;	// X軸和Y軸顯示最大值最小值
		double				m_dXStart, m_dXEnd, m_dYStart, m_dYEnd;	// X軸起始坐標和終止做標
		double				m_dXUnit, m_dYUnit; 				// 每個點代表的值。
		RECT				m_rectCoordinate;					// 相對於元件rectClient的位置	
		CURVE				*m_pCurve;
		//LINE				**m_pLine;	fans mark
		LINE*				m_pLine[CURVE_LINE_MAX];//	fans add
		ImageBackground 	m_Image;
		//	fans add
		CURVE				m_Curve;
		char*				pszCurveNameID;
		char*				pszXNameID;
		char*				pxzYNameID;
		char*				m_pszLineNameID[CURVE_LINE_MAX];	
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		
		LINEBUFFER*			m_BufferLine[CURVE_LINE_MAX];	//	save point buffer
		int					m_nALineIndex[CURVE_LINE_MAX];		//	save current index in the point buffer
		int					m_nALineNum[CURVE_LINE_MAX];		//	save current num in the 
		
		//	目前看來是不需要考慮的，所以下面的無用的
		int					m_iLineCout;			//	曲線的數目
		char*				m_pszXPointID[CURVE_LINE_MAX][MAXPOINT];	//	每個點所對應的資料庫的ID（x軸上的）
		char*				m_pszYPointID[CURVE_LINE_MAX][MAXPOINT];	//	每個點所對應的資料庫的ID（Y軸上的）
		
		//	2009-2-10 10:01:31 Fans Add
		double				m_dXMinSet, m_dXMaxSet, m_dYMinSet, m_dYMaxSet;
		int					m_nSettingState;	//	0x0001 x min setting; 0x0002 x max setting; 0x0004 y min setting; 0x0008 y max setting
		
		
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		void	Show();
		void	Update();
		
		void 	SetPropertys();	//	fans add
		void 	SetPropertys_Nub();	
	    BOOL 	CreateA();		//	fans add
		void	SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount);	//	fans add
		void	SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount);	//	fans add
		
		//	set point buffer for the line that name is pszLineName
		void	SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount);	//	fans add 
		//	set point buffer for the line that index is iLineIndex
		void	SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount);	//	fans add
		
		BOOL	SetLineNum(char* pszLineName, int iNum);
		BOOL	SetLineNum(int	iLineIndex, int iNum);
		
		//	獲取第幾個點的坐標
		tmPoint	GetSpecialPoint(int iLineIndex, int iPointIndex);				//	fans add
		//	根據給定的X，Y值，來計算出來點的坐標
		tmPoint	GetSpecilaPonitA(double dX, double dY);							//	fans add
		
		short   ShowExtentD(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
	//	int		AddLine(LINE *pLine);
	//	int		DeleteLine(int nIndex);
	
	
		//	2009-2-10 11:44:40 fans add
		bool SetPropValueT(char* pszPropName, double Value);
	private:
		int		GetRange();			// 取X,Y軸最大值最小值
		int		DrawCutLine();		// 畫圖例
		int		DrawCoordinate();
		int		DrawLines();
		int		DrawBeeLine(LINE *pLine);
		int		DrawCurveLine(LINE *pLine);
		int		DrawBufferLine();
};


class	CtmCurveEx: public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmCurveEx)
	public:
		CtmCurveEx();
		~CtmCurveEx();
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		//===========
		//	the operations inherit from CtmWnd
		//===========
		BOOL 	CreateA();
		void 	SetPropertys();	
		void 	SetPropertys_Nub();	
		void	Show();
		void	Update();
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52上午
		//=======================================	
		void    OnGetFocus();			
	   	void    OnLoseFocus();			
		WORD    OnKey(WORD wKey);
		//===========
		//	the owner's operations
		//===========
		void	SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference = 0);	
		void	SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount, int nReference = 0);	
		void	SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount, int nReference = 0);	
		BOOL	SetLineNum(char* pszLineName, int iNum);
		BOOL	SetLineNum(int	iLineIndex, int iNum);
		int		GetIndexBaseOnX(CURVEPOINT 	*pValue, int iCount, double dX);
		void	SetShowFunc(pFuncShowRuleStr pFunc);
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
		void	DrawArrow();
		
		void	DrawName();	
		void	DrawXName();
		void	DrawYName();
			
		void	DrawSclae();
		void	DrawXScale();
		void	DrawYScale();
		void	DrawCoordinate();
		
		int		GetRangeXY();					// 取X,Y軸最大值最小值
		int		DrawCutLine();					// 畫圖例
		void	DrawLines();
		void	DrawCurveLine(LINE *pLine);
		
		void	PropareShow();
		void	ShowTitle();
		short   ShowExtentD(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		short	ShowExtentEx(short nX, short nY, char* pszString, int nType = tmFT_CODE_BIG5);
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52上午
		//=======================================	
		int		ComposeYRuleStrDef();
		int		ComposeXRuleStrDef();
		int		ShowYRuleStr();
		int		ShowXRuleStr();
		void	DrawRuleMark();
		void	DrawRuleLine();	
		void	GetXPosValue();	
	private:
        int		DrawBufferLine();
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
		
		

	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	protected:
		ImageBackground 	m_Image;
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		double	m_dXMin, m_dXMax, m_dYMin, m_dYMax;	// X軸和Y軸顯示最大值最小值
		double	m_dXStart, m_dXEnd, m_dYStart, m_dYEnd;	// X軸起始坐標和終止做標
		double	m_dXUnit, m_dYUnit; 				// 每個點代表的值。
		
		double	m_dYMin1, m_dYMax1, m_dYMin2, m_dYMax2;
		
		BOOL	m_bDoubleBuffer;				//	double buffer's on-off
		BOOL	m_bXName;						//	the on-off for shoowing XName
		BOOL	m_bYName;						//	the on-off for showing YName
		BOOL	m_bXArrow;						//	the on-off for the x arrow
		BOOL	m_bYArrow;						//	the on-off for the y arrow
		
		
		int		m_nShowXAxesType;				//	the type for XAxes
		int		m_nShowYAxesType;
		int		m_nShowXScaleType;				//	the type for x value
		int		m_nShowYScaleType;				//	the type for y value
		int		m_nShowTitleType;				//	the type for the curve's title
		int		m_nShowCutLineType;				//	the type for the cut curve
		
		char*	m_pszCurveNameID;				//	the name's id of the curve
		char*	m_pszXNameID;					//	the name's id of the x
		char*	m_pszYNameID;					//	the name's id of the y
		//char*	m_pszCurveName;					//	the name of the curve
		//char*	m_pszXName;						//	the name of the x
		//char*	m_pszYName;						//	the name of the y
		
		int		m_nXCoordinateColor;
		int		m_nYCoordinateColor;
		int		m_nXAxesColor;
		int		m_nYAxesColor;
		
		
		CURVE				m_Curve;
		LINE*				m_pLine[CURVE_LINE_MAX];		
		LINEBUFFER*			m_BufferLine[CURVE_LINE_MAX];		//	save point buffer
		int					m_nALineIndex[CURVE_LINE_MAX];		//	save current index in the point buffer
		int					m_nALineNum[CURVE_LINE_MAX];		//	save current num in the 
		char*				m_pszLineNameID[CURVE_LINE_MAX];	
		
		//=======================================	
		//	for the rule mark fans add 2009/9/8 11:52上午
		//=======================================	
		int					m_YRuleColor;			//	property
		int					m_XRuleColor;			//	property
		char				m_szYRule[256];			//	the show string of the y rule
		char				m_szXRule[256];			//	the show string of the x rule
		pFuncShowRuleStr	m_pFuncYStr;			//	the function pointer of the formlib, compose the show str
		pFuncShowRuleStr	m_pFuncXStr;			//	the function pointer of the formlib, compose the show str
		int					m_xPos;					//	the X pos of the y rule line, onle need it, beacuase the y can't changed
		int					m_yPos;					//	the y pos of the x rule line, onle need it, beacuase the x can't changed 
		double				m_dXPosValue;			//	
		int					m_xPerMove;				//	on pressing key(left & right), that move the piexs. it's a property
		int					m_yPerMove;				//	on pressing key(Up & Down), that move the piexs. it's a property
		int					m_yRuleStrXPos;			//	the x pos of the y rule show string
		int					m_yRuleStrYPos;			//	the y pos of the y rule show string
		int					m_xRuleStrXPos;			//	the x pos of the x rule show string
		int					m_xRuleStrYPos;			//	the y pos of the x rule show string
		double				m_dfPercent;			//	
		char*				m_pszPrompt;					
 		char*				m_pszPromptID;
	
	
	private:
		RECT	m_rectTitle;
		RECT	m_rectCutLine;
		RECT	m_rectXName;	
		RECT	m_rectXAxesScale;				//
		RECT	m_rectYName;		
		RECT	m_rectYAxesScale;				//	
		RECT	m_rectLine;
		RECT	m_rectCoordinate;					// 相對於元件rectClient的位置	
		
		tmRectRelation	m_relationTitle;        
		tmRectRelation	m_relationCutLine;      
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
		
		int	m_nReferenceFrame;
};
#endif
