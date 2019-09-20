       /* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|    
|  Author   :                           									 |    
|  Version  :                           									 |    
|  Creation :                           									 |    
|  Revision :                           									 |    
+===========================================================================*/    

#ifndef	D_HEAVE
#define	D_HEAVE

#include	"wnd.h"

const	int		HLINENUM = 10;
const	int		VLINENUM = HLINENUM;

class	CtmHeave: public CtmWnd
{
	DECLARE_DYNCREATE(CtmHeave)
	
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmHeave();
		~CtmHeave();
		
	public:
	    BOOL 	CreateA();
	    void 	SetPropertys();
	    void	Show();
	    void    DrawBorder();
	    
	protected:
		virtual	void	Paint();
		virtual	void	Paint3D();
		virtual	void	DrawVLine();
		virtual void	DrawHLine();
		virtual	void	PaintDemitint();	
		virtual	void	DrawVLineDemitint();
		virtual	void	DrawHLineDemitint();
	private:
		void	PaintLeftLine3D();
		void	PaintRightLine3D();
		void	PaintUpLine3D();
		void	PaintDownLine3D();
		void	PaintHLine3D(int x0, int x1, int y, int height = 9);					//	水平線3D
		void	PaintVLine3D(int y0, int y1, int x, int width = 11);					//	垂直線3D
		
		void	PaintUpArc3D();
		void	PaintDownArc3D();
		
		
	
	private:
		int		m_nUpArcStart;					//	上面圓弧的開始位置，弧度的橫向寬度是8個像素
		int		m_nDownArcStart;				//	下面圓弧開始的位置，弧度的橫向寬度是7個像素
		
		DWORD	m_dwLineColor;					//	邊?的填充?色 
		
		int		m_aryVLinePos[VLINENUM];
		int		m_aryHLinePos[HLINENUM];
		
		static	int	m_nHLineHeight;
		static	int	m_nVLineWidth;
};

#endif
