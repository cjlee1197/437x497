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
	    int						m_nHistogramType;  		//0=��V, 1=��V
		                		
		DWORD					m_dwGraphColor;			//�W�C��
		WORD					m_wGraphType;			//�Wwtype
		int						m_nGraphWidth;			//�W�e��  
		WORD					m_wShowXCnt;			//��ܬW�Ӽ�
		WORD					m_wXStartIndex;			//X�b�_�lIndex
		                		
		BOOL					m_bShowValue;			//�O�_��ܼƦr	
		WORD					m_wXDigits;				//X�b��ܺ��
		WORD					m_wYDigits;				//Y�b��ܺ��
		BOOL					m_bShowGrid;
		WORD					m_wGridCol;				//��ܺ�����
		DWORD					m_dwGridColor;			//�����C��
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
		RECT	GetTitleRect();					//	Title���d��           
		RECT	GetCutRect();					//	��o�ϨҪ��d��        
		RECT	GetXNameRect();					//	X�b�W�٪��d��
		RECT	GetXAxesScaleRect();			//	���X�b���y�ЭȪ��d�� 
		RECT	GetYNameRect();					//	Y�b�W�٪��d��         
		RECT	GetYAxesScaleRect();			//	���Y�b���y�ЭȪ��d�� 
		RECT	GetLineRect();					//	����u�����e�Ͻd��    
	
		void	SeperateArea();					//	���εe�Ϫ��ϰ�        
		
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
		
		int		GetRangeXY();					// ��X,Y�b�̤j�ȳ̤p��
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
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);	// fans 2008/6/16 11:29�W��
		
		private:
		
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	protected:
		CtmGDIDoubleBuffer*	m_pDoubleBuffer;
		double	m_dXMin, m_dXMax, m_dYMin, m_dYMax;	// X�b�MY�b��̤ܳj�ȳ̤p��
		double	m_dXUnit, m_dYUnit; 				// �C���I�N���ȡC
		
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
		RECT	m_rectCoordinate;					// �۹�󤸥�rectClient����m	
		
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
