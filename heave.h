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
		void	PaintHLine3D(int x0, int x1, int y, int height = 9);					//	�����u3D
		void	PaintVLine3D(int y0, int y1, int x, int width = 11);					//	�����u3D
		
		void	PaintUpArc3D();
		void	PaintDownArc3D();
		
		
	
	private:
		int		m_nUpArcStart;					//	�W���꩷���}�l��m�A���ת���V�e�׬O8�ӹ���
		int		m_nDownArcStart;				//	�U���꩷�}�l����m�A���ת���V�e�׬O7�ӹ���
		
		DWORD	m_dwLineColor;					//	��?����R?�� 
		
		int		m_aryVLinePos[VLINENUM];
		int		m_aryHLinePos[HLINENUM];
		
		static	int	m_nHLineHeight;
		static	int	m_nVLineWidth;
};

#endif
