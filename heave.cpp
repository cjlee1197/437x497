/* Copyright (C), 2005-2010, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : Windows Extension                                             |
|  Task     : Windows Extension Manager header file                         |
|---------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                   |
|  Link     : G++(GCC)3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 			                                                    |
|  Version  :                                    							|
|  Creation :                                    							|
|  Revision :                                    							|
+==========================================================================*/
#include	"heave.h"

WORD	VCOLORS[]=
{
	525,
	655, 
	755,
	918,
	1049,
	5243,
	17724,
	21884,
	26077
};

WORD	HCOLORS[] =
{
	26077,	//RGB2PIXEL(61,	149,	255),//13564,	//	0		// 54,159, 225
	21884,	//RGB2PIXEL(61, 144, 	255),//11483,	//	1		// 47,155, 220	
	17724,	  //RGB2PIXEL( 3, 114, 	255),//9435,		//	2		// 39,152, 220
	5243,	  //RGB2PIXEL( 0, 88,		230),//7323,		//	4		// 12,139, 218
	3163,	  //RGB2PIXEL( 0, 88, 	230),//3163,		//	4		// 12,139, 218
	1083,	  //RGB2PIXEL( 0, 83, 	225),//1083,		//	5		//	6,135, 216
	1050,	  //RGB2PIXEL( 2,106, 	254),//1050,		//	16		//	5,131, 208
	918,		//RGB2PIXEL( 0,101, 	253),//984,		//	17		//	2,122, 198
	755,		//RGB2PIXEL( 0,	96, 	252),//918,		//	18		//	3,112, 177
	655,		//RGB2PIXEL( 0,	77, 	227),//820,		//	19		//	7,100, 167
	525		  //RGB2PIXEL( 0,	67, 	207),//722			//	20		//	4, 91, 145	
};

WORD	HCOLORS_EX[]=
{
	
	26077,//RGB2PIXEL(61, 149, 255),//13564,	//	0		// 54,159, 225
	21884,//RGB2PIXEL(61, 144, 255),//11483,	//	1		// 47,155, 220	
	17724,//RGB2PIXEL(3, 114, 255),	//9435,		//	2		// 39,152, 220
	5243,	//RGB2PIXEL(3, 101, 241),	//7323,		//	3		// 25,147, 220
	3163,	//RGB2PIXEL(0, 88, 230),	//3163,		//	4		// 12,139, 218
	1083,	//RGB2PIXEL(0, 83, 225),	//1083,		//	5		//	6,135, 216
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	6
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	7
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	8
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	9
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	10
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	11
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	12
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	13
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	14
	1115, //RGB2PIXEL(0,	85, 229),	//1115,   //	15
	1050,	//RGB2PIXEL(2,	106, 254),//1050,		//	16		//	5,131, 208
	918,	//RGB2PIXEL(0,	101, 253),//984,		//	17		//	2,122, 198
	755,	//RGB2PIXEL(0,	96, 252),	//918,		//	18		//	3,112, 177
	655,	//RGB2PIXEL(0,	77, 227),	//820,		//	19		//	7,100, 167
	525		//RGB2PIXEL(0,	67, 207),	//722			//	20		//	4, 91, 145		
};

IMPLEMENT_DYNCREATE(CtmHeave, CtmWnd)

int	CtmHeave::m_nHLineHeight = sizeof(HCOLORS_EX) / sizeof(HCOLORS_EX[0]);
int	CtmHeave::m_nVLineWidth =  sizeof(HCOLORS) / sizeof(HCOLORS[0]);


CtmHeave::CtmHeave():	CtmWnd()
{
	strcpy(TYPEName, "CtmHeave");
	for (int i = 0; i < VLINENUM; i++)
		m_aryVLinePos[i] = -1;
	for (int i = 0; i < HLINENUM; i++)
		m_aryHLinePos[i] = -1;
	m_dwLineColor 	= 1115;
	m_nUpArcStart 	= 0;
	m_nDownArcStart = 0;
	m_wnd.wStyle = 0x20;
}

CtmHeave::~CtmHeave()
{
	
}

BOOL	CtmHeave::CreateA()
{
	CtmWnd::CreateA();
		
	
	return TRUE;
	
}

void 	CtmHeave::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("linecolor",		tmDWORD,		&m_dwLineColor);
	SetPropInfo("uparcpos",			tmINT, 			&m_nUpArcStart);
	SetPropInfo("downarcpos",		tmINT,			&m_nDownArcStart);
	
	char sz[20];
	for (int i = 0; i < VLINENUM; i++)
	{
		sprintf(sz, "hlinepos%d", i);
		SetPropInfo(sz, 			tmINT,			&m_aryVLinePos[i]);
	}
	for (int i = 0; i < HLINENUM; i++)
	{
		sprintf(sz, "vlinepos%d", i);
		SetPropInfo(sz, 			tmINT,			&m_aryHLinePos[i]);
	}
}

void    CtmHeave::DrawBorder()
{
	_SetViewPort(0, 0, m_rectClient.right + 1,  m_rectClient.bottom + 1);
	
	
	//////////////////////////////////////
	//			draw inline				//
	//////////////////////////////////////
	_SetColor(0);
	//	draw left inline
	_MoveTo(m_rectClient.left + 11, m_rectClient.top + 19);
	_LineTo(m_rectClient.left + 11, m_rectClient.bottom - 19);
	//	draw Up inline
	_MoveTo(m_rectClient.left + 11, m_rectClient.top + 19);
	_LineTo(m_rectClient.right - 11, m_rectClient.top + 19);
	
	_SetColor(0xffff);
	//	draw down inline
	_MoveTo(m_rectClient.left + 11,  m_rectClient.bottom - 19);
	_LineTo(m_rectClient.right - 11, m_rectClient.bottom - 19);	
	//	draw down inline
	_MoveTo(m_rectClient.right - 11, m_rectClient.top + 19);
	_LineTo(m_rectClient.right - 11, m_rectClient.bottom - 19);
	
	
	
	//////////////////////////////////////
	//			draw outline			//
	//////////////////////////////////////
	_SetColor(0xFFFF);
	//	draw up border
	_MoveTo(m_rectClient.left,  m_rectClient.top - 1);
	_LineTo(m_rectClient.left + m_nUpArcStart,  m_rectClient.top - 1);
	
	_MoveTo(m_rectClient.left + m_nUpArcStart + 1,  m_rectClient.top);
	_LineTo(m_rectClient.left + m_nUpArcStart + 2,  m_rectClient.top);
	
	_MoveTo(m_rectClient.left + m_nUpArcStart + 2,  m_rectClient.top + 1);
	_LineTo(m_rectClient.left + m_nUpArcStart + 2,  m_rectClient.top + 1);
	
	for (int i = 1; i < 4; i++)
	{
		_MoveTo(m_rectClient.left + m_nUpArcStart + 3,  m_rectClient.top + i + 1);
		_LineTo(m_rectClient.left + m_nUpArcStart + 3,  m_rectClient.top + i + 1);
	}
	
	for (int i = 1; i < 3; i++)
	{
		_MoveTo(m_rectClient.left + m_nUpArcStart + 4,  m_rectClient.top + i + 4);
		_LineTo(m_rectClient.left + m_nUpArcStart + 4,  m_rectClient.top + i + 4);
	}
	
	_MoveTo(m_rectClient.left + m_nUpArcStart + 5,  m_rectClient.top + 7);
	_LineTo(m_rectClient.left + m_nUpArcStart + 5,  m_rectClient.top + 7);
	
	_MoveTo(m_rectClient.left + m_nUpArcStart + 6,  m_rectClient.top + 8);
	_LineTo(m_rectClient.left + m_nUpArcStart + 7,  m_rectClient.top + 8);
	
	_MoveTo(m_rectClient.left + m_nUpArcStart + 8,  m_rectClient.top + 9);
	_LineTo(m_rectClient.right - 2,  m_rectClient.top + 9);
	
	_MoveTo(m_rectClient.right - 1,  m_rectClient.top + 10);
	_LineTo(m_rectClient.right - 1,  m_rectClient.top + 10);
	
	_MoveTo(m_rectClient.right ,  m_rectClient.top + 11);
	_LineTo(m_rectClient.right,  m_rectClient.top + 11);
	
	_MoveTo(m_rectClient.right + 1,  m_rectClient.top + 12);
	_LineTo(m_rectClient.right + 1,  m_rectClient.top + 14);
	//	draw left border
	_MoveTo(m_rectClient.left - 1, m_rectClient.top - 1);
	_LineTo(m_rectClient.left - 1,  m_rectClient.bottom - 12);
	_MoveTo(m_rectClient.left,  m_rectClient.bottom - 11);
	_LineTo(m_rectClient.left,  m_rectClient.bottom - 11);	
	_MoveTo(m_rectClient.left + 1,  m_rectClient.bottom - 10);
	_LineTo(m_rectClient.left + 1,  m_rectClient.bottom - 10);
	
	
	
	
	_SetColor(0);
	//	draw right border
	_MoveTo(m_rectClient.right + 1,  m_rectClient.top + 12);
	_LineTo(m_rectClient.right + 1,  m_rectClient.bottom);	
	//	draw down border
	_MoveTo(m_rectClient.left + 2, m_rectClient.bottom - 9);
	_LineTo(m_rectClient.left + m_nDownArcStart - 8, m_rectClient.bottom - 9);
	_MoveTo(m_rectClient.left + m_nDownArcStart - 7, m_rectClient.bottom - 8);
	_LineTo(m_rectClient.left + m_nDownArcStart - 6, m_rectClient.bottom - 8);
	
	for (int i = 1; i < 4; i++)
	{
		_MoveTo(m_rectClient.left + m_nDownArcStart - 6 + i, m_rectClient.bottom - 8 + i);
		_LineTo(m_rectClient.left + m_nDownArcStart - 6 + i, m_rectClient.bottom - 8 + i);
	}
	
	_MoveTo(m_rectClient.left + m_nDownArcStart - 2,  m_rectClient.bottom - 4);
	_LineTo(m_rectClient.left + m_nDownArcStart - 2,  m_rectClient.bottom - 3);
	
	_MoveTo(m_rectClient.left + m_nDownArcStart - 1,  m_rectClient.bottom - 2);
	_LineTo(m_rectClient.left + m_nDownArcStart - 1,  m_rectClient.bottom - 0);
	_MoveTo(m_rectClient.left + m_nDownArcStart - 1,  m_rectClient.bottom + 1);
	_LineTo(m_rectClient.right + 1,  m_rectClient.bottom + 1);
}

void	CtmHeave::DrawVLine()
{
	_SetViewPort(0, 0, m_rectClient.right,  m_rectClient.bottom);
	
	for (int i = 0; i < HLINENUM; i++)
	{
		if (m_aryVLinePos[i] > 0)
		{
			_SetColor(0xFFFF);
			_MoveTo(m_rectClient.left + m_aryVLinePos[i] - 1, m_rectClient.top + 19);
			_LineTo(m_rectClient.left + m_aryVLinePos[i] - 1, m_rectClient.bottom - 19);
			_SetColor(0);
			_MoveTo(m_rectClient.left + m_aryVLinePos[i] + 9, m_rectClient.top + 19);
			_LineTo(m_rectClient.left + m_aryVLinePos[i] + 9, m_rectClient.bottom - 19);
			
			_SetColor(m_dwLineColor);		
			_Rectangle(_GFILLINTERIOR, m_rectClient.left + m_aryVLinePos[i], m_rectClient.top + 19,
									   m_rectClient.left + m_aryVLinePos[i] + 8, m_rectClient.bottom - 19);
			
		}
	}
}


void	CtmHeave::DrawVLineDemitint()
{
	_SetViewPort(0, 0, m_rectClient.right,  m_rectClient.bottom);
	for (int i = 0; i < VLINENUM; i++)
	{
		if (m_aryVLinePos[i] > 0)
		{
			for (int j = 0; j < m_nVLineWidth; j++)
			{
				_SetColor(HCOLORS[j]);
				_MoveTo(m_rectClient.left + m_aryVLinePos[i] + j, m_rectClient.top + m_nHLineHeight);
				_LineTo(m_rectClient.left + m_aryVLinePos[i] + j, m_rectClient.bottom - m_nHLineHeight);
			}
			
		}
	}
}


void	CtmHeave::DrawHLineDemitint()
{
	_SetViewPort(0, 0, m_rectClient.right,  m_rectClient.bottom);
	for (int i = 0; i < HLINENUM; i++)
	{
		if (m_aryHLinePos[i] > 0)
		{
			for (int j = 0; j < m_nVLineWidth; j++)
			{
				
//				printf("j= %d\n", j);
				_SetColor(HCOLORS[j]);
				
				_MoveTo(m_rectClient.left + m_nVLineWidth, m_rectClient.top + m_aryHLinePos[i] + j);
				_LineTo(m_rectClient.right - m_nVLineWidth, m_rectClient.top + m_aryHLinePos[i] + j);
				/*
				_MoveTo(m_rectClient.left + m_aryVLinePos[i] + j, m_rectClient.top + m_nHLineHeight);
				_LineTo(m_rectClient.left + m_aryVLinePos[i] + j, m_rectClient.bottom - m_nHLineHeight);
				*/
			}
			
		}
	}
}

void	CtmHeave::PaintHLine3D(int x0, int x1, int y, int height)
{	
	WORD*	pw = NULL;
	if (height <= sizeof(HCOLORS) / sizeof(WORD)) pw = HCOLORS;
	else pw = HCOLORS_EX;
	
	for (int i = 0; i < height; i++)
	{
		_SetColor(pw[i]);
		_MoveTo(x0, y + i);
		_LineTo(x1, y + i);
	}
}


void	CtmHeave::PaintVLine3D(int y0, int y1, int x, int width)
{
	for (int i = 0;i < width; i++)
	{
		_SetColor(VCOLORS[i]);
		_MoveTo(x + i, y0);
		_LineTo(x + i, y1);
	}
	
}



void	CtmHeave::Paint3D()
{
	_SetViewPort(0, 0, m_rectClient.right,  m_rectClient.bottom);
	
	//	left line
	PaintVLine3D(m_rectClient.top, m_rectClient.bottom, m_rectClient.left);
	//	right line
	PaintVLine3D(m_rectClient.top, m_rectClient.bottom, m_rectClient.right - 9);
	
	//	up line
	PaintHLine3D(m_rectClient.left, m_rectClient.right, m_rectClient.top, 19);
		
	//	down line
	PaintHLine3D(m_rectClient.left, m_rectClient.right, m_rectClient.bottom - 10, 11);
	
	
}

void	CtmHeave::DrawHLine()
{
}

void	CtmHeave::Paint()
{
	
	
	
	_SetViewPort(0, 0, m_rectClient.right,  m_rectClient.bottom);
	_SetColor(m_dwLineColor);
	
	
	//	Draw Up arc	
	_MoveTo(m_rectClient.left,  m_rectClient.top);
	_LineTo(m_rectClient.left + m_nUpArcStart, m_rectClient.top);
	//_SetColor(m_dwLineColor - 1); 
	_MoveTo(m_rectClient.left,  m_rectClient.top + 1);
	_LineTo(m_rectClient.left + m_nUpArcStart + 1, m_rectClient.top + 1);
	//_SetColor(m_dwLineColor - 2);
	_Rectangle(_GFILLINTERIOR, m_rectClient.left, m_rectClient.top + 2, m_rectClient.left + m_nUpArcStart + 2,  m_rectClient.top + 4);	
	_Rectangle(_GFILLINTERIOR, m_rectClient.left, m_rectClient.top + 5, m_rectClient.left + m_nUpArcStart + 3,  m_rectClient.top + 6);
	//_SetColor(m_dwLineColor - 3);
	_MoveTo(m_rectClient.left,  m_rectClient.top + 7);
	_LineTo(m_rectClient.left + m_nUpArcStart + 4, m_rectClient.top + 7);
	//_SetColor(m_dwLineColor - 4);
	_MoveTo(m_rectClient.left,  m_rectClient.top + 8);
	_LineTo(m_rectClient.left + m_nUpArcStart + 5, m_rectClient.top + 8);
	//_SetColor(m_dwLineColor - 5);
	_MoveTo(m_rectClient.left,  m_rectClient.top + 9);
	_LineTo(m_rectClient.left + m_nUpArcStart + 7, m_rectClient.top + 9);
	
	//	draw up line
	//_SetColor(m_dwLineColor - 6);
	_Rectangle(_GFILLINTERIOR, m_rectClient.left, m_rectClient.top + 10, m_rectClient.right - 2, m_rectClient.top + 14);
	_MoveTo( m_rectClient.right - 1, m_rectClient.top + 11);
	_LineTo( m_rectClient.right - 1, m_rectClient.top + 14);
	_MoveTo( m_rectClient.right, m_rectClient.top + 12);
	_LineTo( m_rectClient.right, m_rectClient.top + 14);
	_Rectangle(_GFILLINTERIOR, m_rectClient.left, m_rectClient.top + 15, m_rectClient.right, m_rectClient.top + 18);
	
	_SetColor(m_dwLineColor);
	//	draw left line, 
	_Rectangle(_GFILLINTERIOR, m_rectClient.left, m_rectClient.top + 19, m_rectClient.left + 10, m_rectClient.bottom - 12);
	_MoveTo(m_rectClient.left + 1, m_rectClient.bottom - 11);
	_LineTo(m_rectClient.left + 10, m_rectClient.bottom - 11);
	_MoveTo(m_rectClient.left + 2, m_rectClient.bottom - 10);
	_LineTo(m_rectClient.left + 10, m_rectClient.bottom - 10);
	
		
	//	draw right line
	_Rectangle(_GFILLINTERIOR, m_rectClient.right - 10, m_rectClient.top + 19, m_rectClient.right, m_rectClient.bottom - 10);
	
	
	//	draw down line
	_Rectangle(_GFILLINTERIOR, m_rectClient.left + 10, m_rectClient.bottom - 19, m_rectClient.right, m_rectClient.bottom - 10);
	
	//	draw down arc
	_Rectangle(_GFILLINTERIOR, m_rectClient.left + m_nDownArcStart,  m_rectClient.bottom - 2, 
			    m_rectClient.right, m_rectClient.bottom);
	_Rectangle(_GFILLINTERIOR, m_rectClient.left + m_nDownArcStart - 1,  m_rectClient.bottom - 4, 
			    m_rectClient.right, m_rectClient.bottom - 3);
			    
			    
	_MoveTo( m_rectClient.left + m_nDownArcStart - 2, m_rectClient.bottom - 5);
	_LineTo( m_rectClient.right, m_rectClient.bottom - 5);
	_MoveTo( m_rectClient.left + m_nDownArcStart - 3, m_rectClient.bottom - 6);
	_LineTo( m_rectClient.right, m_rectClient.bottom - 6);
	_MoveTo( m_rectClient.left + m_nDownArcStart - 4, m_rectClient.bottom - 7);
	_LineTo( m_rectClient.right, m_rectClient.bottom - 7);
	
	_MoveTo( m_rectClient.left + m_nDownArcStart - 5, m_rectClient.bottom - 8);
	_LineTo( m_rectClient.right, m_rectClient.bottom - 8);
	_MoveTo( m_rectClient.left + m_nDownArcStart - 7, m_rectClient.bottom - 9);
	_LineTo( m_rectClient.right, m_rectClient.bottom - 9);
	
	DrawBorder();
	DrawVLine();
	DrawHLine();
	
}

void	CtmHeave::PaintDemitint()
{
	_SetViewPort(0, 0, m_rectClient.right, m_rectClient.bottom);
	
	//	upline & arc
	int	nOffSet = 0;
	for (int i = 0; i < 6; i++)
	{
		nOffSet = i;
		_SetColor(HCOLORS_EX[nOffSet]);
		_MoveTo(m_rectClient.left + nOffSet, m_rectClient.top + nOffSet);
		
		if (i < 3) _LineTo(m_rectClient.left + m_nUpArcStart + i, m_rectClient.top + nOffSet);
		else if (i == 3 || i == 4)
		{
			_LineTo(m_rectClient.left + m_nUpArcStart + 2, m_rectClient.top + nOffSet);
		}
		else
		{
			_LineTo(m_rectClient.left +  m_nUpArcStart + i - 2, m_rectClient.top + nOffSet);
		}
	}
	
	for (int i = 6; i < 12; i++)
	{
		nOffSet = i;
		_SetColor(HCOLORS_EX[nOffSet]);
		
		_MoveTo(m_rectClient.left + 5, m_rectClient.top + nOffSet);
		
		if (i == 6)	_LineTo(m_rectClient.left + m_nUpArcStart + 3, m_rectClient.top + nOffSet);
		else if (i < 9) _LineTo(m_rectClient.left + m_nUpArcStart + 3 + i - 6, m_rectClient.top + nOffSet);
		else if (i == 9) _LineTo(m_rectClient.left + m_nUpArcStart + 4 + i - 6, m_rectClient.top + nOffSet);
		else _LineTo(m_rectClient.right - (12 - i), m_rectClient.top + nOffSet);		
	}
	
	
	for (int i = 12; i < m_nHLineHeight; i++)
	{
		nOffSet = i;
		_SetColor(HCOLORS_EX[nOffSet]);
		
		if (i < 16) _MoveTo(m_rectClient.left + 5, m_rectClient.top + nOffSet);
		else
		{
			_MoveTo(m_rectClient.left + 5 + i - 15, m_rectClient.top + nOffSet);
		}
		
		_LineTo(m_rectClient.right - (i - 9), m_rectClient.top + nOffSet);
		
		
	}
	
	//	draw left line
	nOffSet = 0;
	for (int i = 0; i < m_nVLineWidth; i++)
	{
		nOffSet = i;
		
		_SetColor(HCOLORS[nOffSet]);
		
		if (i < 6) _MoveTo(m_rectClient.left + nOffSet, m_rectClient.top + nOffSet);		
		else _MoveTo(m_rectClient.left + nOffSet, m_rectClient.top + 16 + nOffSet - 6);		
		
		
		if (i >= 2)
			_LineTo(m_rectClient.left + nOffSet, m_rectClient.bottom - 10 - nOffSet);
		else if (i == 0)
			_LineTo(m_rectClient.left + nOffSet, m_rectClient.bottom  - 10 - 2);
		else if (i == 1)
			_LineTo(m_rectClient.left + nOffSet, m_rectClient.bottom - 10 - 1);
			
	}
		
	//	draw down line
	nOffSet = 0;
	for (int i = 0; i < 6; i++)
	{
		nOffSet = m_nHLineHeight - i - 1;
		_SetColor(HCOLORS_EX[nOffSet]);
		nOffSet = i;
		_MoveTo(m_rectClient.right - nOffSet, m_rectClient.bottom - nOffSet);
		
		if (i < 3) _LineTo(m_rectClient.right - (m_nDownArcStart + i), m_rectClient.bottom - nOffSet);
		else if (i == 3 || i == 4) _LineTo(m_rectClient.right - (m_nDownArcStart + 2), m_rectClient.bottom - nOffSet);
		else _LineTo(m_rectClient.right - (m_nDownArcStart + i - 2),  m_rectClient.bottom - nOffSet);
		
		
	}
	
	for (int i = 6; i < 12; i++)
	{
		nOffSet = m_nHLineHeight - i - 1;
		_SetColor(HCOLORS_EX[nOffSet]);
		nOffSet = i;
		_MoveTo(m_rectClient.right - 5, m_rectClient.bottom - nOffSet);
		
		if (i == 6) _LineTo(m_rectClient.right - (m_nDownArcStart + 3), m_rectClient.bottom - nOffSet);
		else if (i < 9) _LineTo(m_rectClient.right - (m_nDownArcStart + 3 + i - 6), m_rectClient.bottom - nOffSet);
		else if (i == 9) _LineTo(m_rectClient.right - (m_nDownArcStart + 4 + i - 6), m_rectClient.bottom - nOffSet);
		else _LineTo(m_rectClient.left + (12 - i), m_rectClient.bottom - nOffSet);
	}
	
	for (int i = 12; i < m_nHLineHeight; i++)
	{
		nOffSet = m_nHLineHeight - i - 1;
		_SetColor(HCOLORS_EX[nOffSet]);
		nOffSet = i;
		if (i < 16) _MoveTo(m_rectClient.right - 5, m_rectClient.bottom - nOffSet);
		else
		{
			_MoveTo(m_rectClient.right - (5 + i - 15), m_rectClient.bottom - nOffSet);
			
		}
		_LineTo(m_rectClient.left + i - 9, m_rectClient.bottom - nOffSet);
	}
	
	//	draw right line
	nOffSet = 0;
	for (int i = 0; i < m_nVLineWidth; i++)
	{
		nOffSet = m_nVLineWidth - i - 1;
		_SetColor(HCOLORS[nOffSet]);
		nOffSet = i;
		if (i < 6) _MoveTo(m_rectClient.right - nOffSet, m_rectClient.bottom - nOffSet);
		else _MoveTo(m_rectClient.right - nOffSet, m_rectClient.bottom - (16 + nOffSet - 6));
		
		if (i >= 2)
			_LineTo(m_rectClient.right - nOffSet, m_rectClient.top + 10 + nOffSet);
		else if (i == 0)
			_LineTo(m_rectClient.right - nOffSet, m_rectClient.top + 10 + 2);
		else if (i == 1)
			_LineTo(m_rectClient.right - nOffSet, m_rectClient.top + 10 + 1);
	}
	
	DrawVLineDemitint();
	DrawHLineDemitint();
}

void	CtmHeave::Show()
{
	if (m_wnd.wStyle & tmWS_SHOW)
	{
		PaintDemitint();
		CtmWnd::Show();
	}	
}



