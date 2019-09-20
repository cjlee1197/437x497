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
	    int						m_nHistogramType;  		//0=��V, 1=��V
		                		
		DWORD					m_dwGraphColor[MAX_GRAPHCOLOR_CNT];			
		DWORD					m_dwOldGraphColor[MAX_GRAPHCOLOR_CNT];
		DWORD					m_dwRefDestColor[MAX_GRAPHCOLOR_CNT];
		DWORD					m_dwRefColorControl;
		DWORD					m_dwRefColorChange;
		WORD					m_wGraphType;			//�Wwtype
		int						m_nGraphWidth;			//�W�e��  
		WORD					m_wXStartIndex;			//X�b�_�lIndex
		                			
		WORD					m_wXDigits;				//X�b��ܺ��
		BOOL					m_bShowGrid;
		WORD					m_wGridCol;				//��ܺ�����
		WORD					m_wGridRow;				//������ָ����
		WORD					m_wRefCount;
		DWORD					m_dwGridColor;			//�����C��
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
		void	DrawTimergrambar();
		
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
		
		tmRectRelation	m_relationLeft;
		tmRectRelation	m_relationRight;
		tmRectRelation	m_relationUp;
		tmRectRelation 	m_relationDown;
};

#endif