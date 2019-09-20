/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 
/*===========================================================================+
|  Class    : Histogram			                                             |
|  Task     : Histogram header file                                          |
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
#include	"histogram.h"
#include	"utils.h"
#include	"commonaction.h"
#include    "tmdebug.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

IMPLEMENT_DYNCREATE(CtmHistogram, CtmWnd)
/*===========================================================================+
|           Class implementation                                             |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
void CtmHistogram::InitData() 
{
    memset(&m_relationLeft	, 			0, sizeof(m_relationLeft	));
	memset(&m_relationRight  , 			0, sizeof(m_relationRight   ));
	memset(&m_relationUp    , 			0, sizeof(m_relationUp      ));
	memset(&m_relationDown , 			0, sizeof(m_relationDown    ));
	
	memset(&m_relationTitle      , 		0, sizeof(m_relationTitle     ));    
	memset(&m_relationXName      , 		0, sizeof(m_relationXName     ));    
	memset(&m_relationXAxesScale , 		0, sizeof(m_relationXAxesScale));	
	memset(&m_relationYName	     , 		0, sizeof(m_relationYName	    ));    
	memset(&m_relationYAxesScale , 		0, sizeof(m_relationYAxesScale));	
	memset(&m_relationLine       , 		0, sizeof(m_relationLine      ));     
	
	
	
	memset(&m_rectTitle      	, 		0, sizeof(m_rectTitle     ));    
	memset(&m_rectXName      	, 		0, sizeof(m_rectXName     ));    
	memset(&m_rectXAxesScale	, 		0, sizeof(m_rectXAxesScale));	
	memset(&m_rectYName	    	, 		0, sizeof(m_rectYName	    ));    
	memset(&m_rectYAxesScale	, 		0, sizeof(m_rectYAxesScale));	
	memset(&m_rectLine       	, 		0, sizeof(m_rectLine      )); 
     
	                      
	m_relationTitle.pRect 		= &m_rectTitle;
	m_relationXName.pRect 		= &m_rectXName;
	m_relationXAxesScale.pRect 	= &m_rectXAxesScale;
	m_relationYName.pRect 		= &m_rectYName;
	m_relationYAxesScale.pRect 	= &m_rectYAxesScale;
	m_relationLine.pRect 		= &m_rectLine;
}
CtmHistogram::CtmHistogram(): CtmWnd()
{
	strcpy(TYPEName, "CtmHistogram");	
	
	m_pDoubleBuffer = NULL;
	InitData();
	                                         
	SetBorderWidth(3);  
	
	m_bgc			= DEFAULT_BGC;
	m_fgc			= 0;
	m_nHistogramType= 0;
	m_dwGraphColor 	= DEFAULT_GRAPHCOLOR;
	m_wGraphType	= 0;
	m_nGraphWidth	= DEFAULT_GRAPHWIDTH;
	m_wShowXCnt		= DEFAULT_SHOWXCNT;
	m_bShowValue	= TRUE;
	m_bShowGrid		= TRUE;
	m_wGridCol		= DEFAULT_GRIDCOL;
	
	m_dwGridColor	= 
	m_dwXAxesColor	= 
	m_dwYAxesColor	= DEFAULT_GRIDCOLOR;
	
	m_dXMin 		= 0;
	m_dYMin 		= 0;
	m_dXMax 		= 0;
	m_dYMax 		= 0;
	m_dYUnit 		= 0;
	m_dXUnit 		= 0;
	
	m_pszGraphName = NULL;
	m_pszGraphNameID = NULL;
	m_pszXName		= NULL;				
	m_pszYName		= NULL;				
	m_pszXNameID 	= NULL;				
	m_pszYNameID 	= NULL;				
	m_bXName 		= TRUE;
	m_bYName		= TRUE;
	                     
	m_wXDigits = 0;
	m_wYDigits = 0;
	
	for(int i = 0; i < MAX_GRAPH_NUM; i++)
   	{
   		m_GraphValue[i].wIndex = i + 1;
   		m_GraphValue[i].dValue = 0.0;
   	}
	m_bTouchSupport = TRUE;
	
	m_pGraphStaticX2 = NULL;
	m_pStrStaticX2 = NULL;
}

CtmHistogram::~CtmHistogram()
{
	for(int i = 0; i < m_wShowXCnt; i++)
   	{
   		delete m_pGraphStaticX2[i];
   		delete m_pStrStaticX2[i];
   	}
   	delete []m_pGraphStaticX2;
   	delete []m_pStrStaticX2;
   	
	if (m_pDoubleBuffer != NULL)
	{
		delete m_pDoubleBuffer;
		m_pDoubleBuffer = NULL;
	}
}

//	fans add
BOOL	CtmHistogram::CreateA()
{
	CtmWnd::CreateA();

	SetFGColor(m_fgc);
    SetBGColor(m_bgc);
    
    if(m_pGraphStaticX2 != NULL)
    {
    	for(int i = 0; i < m_wShowXCnt; i++)
	   	{
	   		delete m_pStrStaticX2[i];
	   	}
	   	delete []m_pStrStaticX2;	
    }
    
    if(m_pStrStaticX2 != NULL)
    {
    	for(int i = 0; i < m_wShowXCnt; i++)
	   	{
	   		delete m_pStrStaticX2[i];
	   	}
	   	delete []m_pStrStaticX2;	
    }
    
    m_pGraphStaticX2 	= new CtmStaticX2*[m_wShowXCnt];
    m_pStrStaticX2 		= new CtmStaticX2*[m_wShowXCnt];
    for(int i = 0; i < m_wShowXCnt; i++)
   	{
   		m_pGraphStaticX2[i] = new CtmStaticX2;
   		m_pGraphStaticX2[i]->SetPropertys();
		m_pGraphStaticX2[i]->SetPropValueT("bgc",	m_dwGraphColor);
		m_pGraphStaticX2[i]->SetPropValueT("wtype",m_wGraphType);
		
		
   		m_pStrStaticX2[i] 	= new CtmStaticX2;
   		m_pStrStaticX2[i]->SetPropertys();
		m_pStrStaticX2[i]->SetPropValueT("fgc",	m_fgc);
		m_pStrStaticX2[i]->SetPropValueT("bgc",m_bgc);
		m_pStrStaticX2[i]->SetPropValueT("wtype",0x27);
   	}
   	
   	
   	if(m_pDoubleBuffer == NULL)
   	{
   		m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
		m_pDoubleBuffer->InitBuffer(m_bgc);
	}
	
	if(m_wShowXCnt == 0) m_wShowXCnt= DEFAULT_SHOWXCNT;
	if(m_wGridCol == 0)  m_wGridCol = DEFAULT_GRIDCOL;
	
	return TRUE;
}

void	CtmHistogram::SetPropertys()
{
	CtmWnd::SetPropertys();
		
	SetPropInfo("histogramtype",	tmINT,		&m_nHistogramType);
	SetPropInfo("graphcolor",		tmDWORD,	&m_dwGraphColor);
	SetPropInfo("graphtype",		tmWORD, 	&m_wGraphType);
	SetPropInfo("graphwidth",		tmINT,		&m_nGraphWidth);
	SetPropInfo("showxcnt",			tmWORD, 	&m_wShowXCnt);
	SetPropInfo("showvalue",		tmBOOL,		&m_bShowValue);
	SetPropInfo("xdigits",			tmWORD,		&m_wXDigits);
	SetPropInfo("ydigits",			tmWORD,		&m_wYDigits);
	SetPropInfo("showgrid",			tmBOOL,		&m_bShowGrid);
	SetPropInfo("gridcols",			tmWORD,		&m_wGridCol);
	SetPropInfo("gridcolor",   		tmDWORD,	&m_dwGridColor);
	SetPropInfo("xaxescolor",  		tmDWORD,	&m_dwXAxesColor); 
	SetPropInfo("yaxescolor",   	tmDWORD,	&m_dwYAxesColor); 

	
	SetPropInfo("ymax",				tmDOUBLE,	&m_dYMax);
	SetPropInfo("graphname",		tmSTR,		&m_pszGraphName);
	SetPropInfo("graphnameID",		tmSTR,		&m_pszGraphNameID);
	SetPropInfo("xname",			tmUSTR,		&m_pszXName);
	SetPropInfo("yname",			tmUSTR,		&m_pszYName);
	SetPropInfo("xnameID",			tmSTR,		&m_pszXNameID);
	SetPropInfo("ynameID",			tmSTR,		&m_pszYNameID);     
    SetPropInfo("xnameshow",		tmBOOL	,	&m_bXName);
	SetPropInfo("ynameshow",		tmBOOL	,	&m_bYName);
	
}



void	CtmHistogram::PropareShow()
{
    InitData();
	
	if (m_pszGraphNameID != NULL)
		StrIDToStr(m_pszGraphNameID, &m_pszGraphName);
	if (m_pszXNameID != NULL)
		StrIDToStr(m_pszXNameID, &m_pszXName);
	if (m_pszYNameID != NULL)
		StrIDToStr(m_pszYNameID, &m_pszYName);
	
		
	if (m_pDoubleBuffer != NULL)
		m_pDoubleBuffer->InitBuffer(m_bgc);
	else 
		Clear();
		
	m_rectCoordinate.left	= 0;
	m_rectCoordinate.right	= GetClientWidth() - 1;
	m_rectCoordinate.top	= 0;
	m_rectCoordinate.bottom	= GetClientHeight() -1;
	
}

int		CtmHistogram::GetRangeXY()
{
	int i;
	CURVEPOINT	*pPoint;
	double dXMin = 0;
	double dXMax = 0;
	int    nIndex = 0;
	
	m_dXMax = m_wShowXCnt;
	dXMin = fmin(m_dXMin, m_dXMax);
	dXMax = fmax(m_dXMin, m_dXMax);
	for(nIndex = 0; nIndex < m_wShowXCnt; nIndex ++)
	{
		if(m_dYMax <= m_GraphValue[nIndex].dValue)				
		{
			m_dYMax	= m_GraphValue[nIndex].dValue;
			m_dYMax += (m_dYMax/m_wShowXCnt);
		}
	}
	m_dYUnit = (m_dYMax - m_dYMin) / m_wGridCol;
	m_dXUnit = (m_dXMax - m_dXMin) / m_wShowXCnt;
	
	return 0;
}

RECT	CtmHistogram::GetTitleRect()
{
	RECT Result;
//	RECT TempRect; 
	memset(&Result, 0, sizeof(Result));
	
	m_bTitle = TRUE;
	AddTopRelation(&m_relationTitle, m_pcFont->m_wHeight);
	Result = *m_relationTitle.pRect;
	return Result;
}

RECT	CtmHistogram::GetCutRect()
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	
	return Result;
}

RECT	CtmHistogram::GetXNameRect()
{
	RECT Result;    
	memset(&Result, 0, sizeof(Result));   
	if (m_bXName)
	{
		AddDownRelation(&m_relationXName, m_pcFont->m_wHeight);
		Result = *m_relationXName.pRect;
	}	
	return Result;
}

RECT	CtmHistogram::GetYNameRect()
{
	RECT Result;
	memset(&Result, 0, sizeof(Result)); 
	if (m_bYName)
	{
		AddLeftRelation(&m_relationYName, m_pcFont->m_wWidth);
		Result = *m_relationYName.pRect;		
	}
	return Result;
}

RECT	CtmHistogram::GetXAxesScaleRect()
{
	RECT Result;
	AddDownRelation(&m_relationXAxesScale, m_pcFont->m_wHeight);
	Result = *m_relationXAxesScale.pRect;
	return Result;
}

RECT	CtmHistogram::GetYAxesScaleRect()
{
	RECT Result;
	
	char sz[100], szYFormat[10];
	int	nWidth = 0;
	snprintf(szYFormat, 10, "%%.%df", m_wYDigits);
	for(int i = 0; i <= m_wGridCol; i ++)
	{
		snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);//m_dYUnit * nYUnit * i + m_dYMin);
		
		if ((int)(strlen(sz) * m_pcFont->m_wWidth) / 2 > nWidth)
			nWidth = strlen(sz) * m_pcFont->m_wWidth / 2;
	}
	
	//printf("width = %d\n", nWidth);
	
	AddLeftRelation(&m_relationYAxesScale, nWidth);
	Result = *m_relationYAxesScale.pRect;
	
	return Result;
}

RECT	CtmHistogram::GetLineRect()
{
	RECT Result;
	int nXUnit, nYUnit;
	Result.left = GetLastLeft();
	Result.right = GetLastRight();
	Result.top = GetLastTop();
	Result.bottom = GetLastBottom();
	
	nXUnit	= (Result.right - Result.left) / m_wShowXCnt;
	nYUnit	= (Result.bottom - Result.top) / m_wGridCol;
	
	Result.right = Result.left + nXUnit * m_wShowXCnt;
	Result.bottom = Result.top + nYUnit * m_wGridCol;
	m_rectLine = Result;
	
	for(int i = 0; i < m_wShowXCnt; i++)
   	{
   		m_pGraphStaticX2[i]->SetPropValueT("top", m_rectLine.bottom  + m_rectClient.top);
   		m_pGraphStaticX2[i]->SetPropValueT("bottom", m_rectLine.bottom + m_rectClient.top);
   	}
	return Result;
}

void	CtmHistogram::SeperateArea()
{
	GetTitleRect();				//	Title的範圍 
	GetXNameRect();	 			//	X軸名稱的範圍    
	GetYNameRect();				//	Y軸名稱的範圍   
	GetXAxesScaleRect();		//	獲取X軸的座標值的範圍   
	GetYAxesScaleRect();		//	獲取Y軸的座標值的範圍   
	GetLineRect();				//	獲取真正的畫圖範圍	
	//printf("CurveName=%s\n", Name);
	//printf("Title Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectTitle.left, m_rectTitle.right, m_rectTitle.top, m_rectTitle.bottom);
	//printf("XName Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectXName.left, m_rectXName.right, m_rectXName.top, m_rectXName.bottom);	
	//printf("XAxes Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectXAxesScale.left, m_rectXAxesScale.right, m_rectXAxesScale.top, m_rectXAxesScale.bottom);	
	//printf("YName Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectYName.left, m_rectYName.right, m_rectYName.top, m_rectYName.bottom);	
	//printf("YAXES Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectYAxesScale.left, m_rectYAxesScale.right, m_rectYAxesScale.top, m_rectYAxesScale.bottom);	
	//printf("line  Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectLine.left, m_rectLine.right, m_rectLine.top, m_rectLine.bottom);	

}

short	CtmHistogram::ShowExtentEx(short nX, short nY, char* pszString, int nType)
{
	if (m_pDoubleBuffer != NULL)
		return ShowExtentD(nX, nY, pszString, nType);
	else return ShowExtent(nX, nY, pszString);
}

short   CtmHistogram::ShowExtentD(short nX, short nY, char* pszString, int nType)
{

	short       i;
	short       nLength = strlen(pszString);
	WORD        wCode;
	BOOL        bUnderLine = FALSE;
	
	short       nXGap = 0;
	short       nXGapHalf = (short)m_pcFont->m_wWidth >> 1;   // Gap of Half font - English
	short       nXGapFull = (short)m_pcFont->m_wWidth;        // Gap of Full font - Chinese
	short       nYGap     = (short)m_pcFont->m_wWidth;
	short       nIndex;
	short       nStringWidth;
	short       nRight;
	
	short       nX0;
	short       nStringWidth0 = 0;

	//=======
	//      Check and adjust the position
	//=======
	if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
	{
	    _SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	    m_pcFont->SetColor(m_fgc);
	    return m_pcFont->ShowStringD(m_pDoubleBuffer->GetBuffer(), nX, 
	    							nY, pszString, nType);
	}
	
	if ((nY + nYGap) > GetClientHeight()) return 0;

	if (nX < 0)
    {
	    // nIndex = (abs(nX) + (nXgapHalf - 1)) / nXgapHalf
	    nIndex = (nXGapHalf-1-nX) / nXGapHalf;  
	    if ((BYTE)*(pszString+nIndex-1) >= tmCHINESE_CODE) nIndex++;
	    nX += nXGapHalf * nIndex;
    }
	else
    	nIndex = 0;

	//=======
	//      Show the string using the window's background and foreground color
	//=======
	m_pcFont->SetColor(m_fgc);
	nX0 = (nX += m_rectClient.left);
	nY += m_rectClient.top;
	nRight = m_rectClient.right + 1;
	_SetViewPort(0, 0, g_infoDrv.nX-1, g_infoDrv.nY-1);
	m_pDoubleBuffer->SetColorInBuffer(m_fgc);
	nStringWidth = 0;
	for (i=nIndex; i<nLength; i++)
    {
    	if ((wCode = (BYTE)*(pszString+i)) >= tmCHINESE_CODE)
        {
	        if ((nX += nXGapFull) > nRight) break;
	        wCode = B2W(pszString+i);
	        i++;
	        nXGap = nXGapFull;
        }
   		else
        {
        	if (wCode == tmFONT_C_UNDERLINE)
            {
            	if (!bUnderLine)
                {
	                bUnderLine = TRUE;
	                continue;
                }
            }
        	else if (wCode == tmFONT_C_NEWLINE)
            {
	            nY += nYGap;
	            if ((nY + nYGap) > (m_rectClient.bottom + 1)) break;
	            if (nStringWidth > nStringWidth0) nStringWidth0 = nStringWidth;
	            nX = nX0;
	            continue;
            }
        	if ((nX += nXGapHalf) > nRight) break;
        	nXGap = nXGapHalf;
        }
       
   		if (m_pcFont->m_wFormat & tmFONT_OPAQUE)
        {
	        m_pDoubleBuffer->SetColorInBuffer(m_bgc);
	        if (m_pcFont->m_wFormat & tmFONT_BOLD)
            {
            	m_pDoubleBuffer->DrawRectangleInBuffer(_GFILLINTERIOR, nX-nXGap, nY, nX, nY+nYGap-1);
            }
        	else if (m_pcFont->m_wFormat & tmFONT_SHADOW)
            {
            	m_pDoubleBuffer->DrawRectangleInBuffer(_GFILLINTERIOR, nX-nXGap, nY, nX, nY+nYGap);
            }
        	else if (m_pcFont->m_wFormat & tmFONT_EMBOSS)
            {
            	m_pDoubleBuffer->DrawRectangleInBuffer(_GFILLINTERIOR, nX-nXGap-1, nY, nX, nY+nYGap);
            }
        	else
            	m_pDoubleBuffer->DrawRectangleInBuffer(_GFILLINTERIOR, nX-nXGap, nY, nX-1, nY+nYGap-1);
        	m_pDoubleBuffer->SetColorInBuffer(m_fgc);
        }
    	if ((m_pcFont->m_wFormat & tmFONT_UNDERLINE) || (bUnderLine))
        {
	        m_pDoubleBuffer->MoveToInBuffer(nX-nXGap, nY+nYGap);
	        m_pDoubleBuffer->DrawLineToInBuffer(nX-1,     nY+nYGap);
	        bUnderLine = FALSE;
        }

		if (nXGap == nXGapHalf) nY++;

    	
	    m_pcFont->ShowD(m_pDoubleBuffer->GetBuffer(),nX-nXGap - m_rectClient.left, nY - m_rectClient.top, wCode);
    	nStringWidth += nXGap;
		if (nXGap == nXGapHalf) nY--;
    }
	if (nStringWidth0 > nStringWidth) nStringWidth = nStringWidth0;
	return nStringWidth;
}

void	CtmHistogram::ShowTitle()
{
	if(m_pszGraphName != NULL)
	{
		int	nPos = 0;
		nPos = m_rectTitle.left + 
				(m_rectTitle.right - m_rectTitle.left - strlen(m_pszGraphName) * m_pcFont->m_wWidth / 2) / 2;
		ShowExtentEx(nPos, m_rectTitle.top ,m_pszGraphName, m_pcFont->GetFontCode());					
	}	
}



void	CtmHistogram::DrawAxes()
{
	
	if (m_pDoubleBuffer != NULL)
	{
		//	draw y	
		m_pDoubleBuffer->SetColorInBuffer(m_dwYAxesColor);
		
		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left , m_rectLine.bottom);
		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left, m_rectLine.top);
				
		//	draw x
		m_pDoubleBuffer->SetColorInBuffer(m_dwXAxesColor);
		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left, m_rectLine.bottom);         
		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom);        
	}
	else
	{
		_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
			m_rectCoordinate.right, m_rectCoordinate.bottom);	
		//	draw y	
		_SetColor(m_dwYAxesColor);
		_MoveTo(m_rectLine.left, m_rectLine.bottom);
		_LineTo(m_rectLine.left, m_rectLine.top); 
		
		//	draw x
		_SetColor(m_dwXAxesColor);
		_MoveTo(m_rectLine.left, m_rectLine.bottom);         
		_LineTo(m_rectLine.right, m_rectLine.bottom);
	}
	
}

void	CtmHistogram::DrawAxesGrid()
{
	int	nXUnit, nYUnit;
	int	i = 0;
	if (m_wShowXCnt <= 0) m_wShowXCnt = 1;
	if (m_wGridCol <= 0) m_wGridCol = 1;
	
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	
	if (m_pDoubleBuffer != NULL)
	{
		m_pDoubleBuffer->SetColorInBuffer(m_dwGridColor);
		for(i = 1; i <= m_wGridCol; i ++)
		{
			m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i - 1);   
			m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1); 
		}
	}
	else
	{
		_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
					m_rectCoordinate.right, m_rectCoordinate.bottom);
					
		_SetColor(m_dwGridColor);
		for(i = 1; i <= m_wGridCol; i ++)
		{
			_MoveTo(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i);   
			_LineTo(m_rectLine.right, m_rectLine.bottom - nYUnit * i);      
		}
	}
}
void	CtmHistogram::DrawSclae()
{
	DrawXScale();
	DrawYScale();
	
}

void	CtmHistogram::DrawXScale()
{
	int nStrXOffset = 0;
	int i = 0;
	int	nXUnit, nStrOffset;
	char	sz[100], szXFormat[10];	
	double  dXValue = 0.0;
	int nLeft, nRight;
	
	memset(sz, 0, sizeof(sz));
	snprintf(szXFormat, 10, "%%.%df", m_wXDigits);
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_wShowXCnt;
	nXUnit	= (m_rectLine.right - m_rectLine.left - nXUnit/2) / m_wShowXCnt;
	
	snprintf(sz, 100, szXFormat, 0);
	nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;

	//ShowExtentEx(m_rectLine.left, m_rectXAxesScale.top + 1, sz);
	
	for(i = 1; i <= m_wShowXCnt; i ++)
	{
		memset(sz, 0, sizeof(sz));
		//snprintf(sz, 100, szXFormat, m_dXUnit * i + m_dXMin);
		dXValue = m_GraphValue[i-1].wIndex;
		snprintf(sz, 100, szXFormat, dXValue);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
		
		if (m_rectLine.left + nXUnit * i + nStrXOffset > m_rectLine.right)
		{
			if (i == m_wShowXCnt)
				nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 2;				
			else
				nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 4;	
		}
		
		ShowExtentEx(m_rectLine.left + nXUnit * i - nStrXOffset,
					m_rectXAxesScale.top + 1, sz);		
	
		nLeft  = m_rectLine.left + nXUnit * i - nStrXOffset - m_nGraphWidth/2 + m_rectClient.left + strlen(sz) * m_pcFont->m_wWidth / 4;
		nRight = m_rectLine.left + nXUnit * i - nStrXOffset + m_nGraphWidth/2 + m_rectClient.left + strlen(sz) * m_pcFont->m_wWidth / 4;
		m_pGraphStaticX2[i-1]->SetPropValueT("left", nLeft);
		m_pGraphStaticX2[i-1]->SetPropValueT("right", nRight);
		
	}
}


//	2008/4/2 11:06上午 fans moidfy
void	CtmHistogram::DrawYScale()
{
	
	double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	
	m_dYUnit = (m_dYMax - m_dYMin) / m_wGridCol;
	
    int nYUnit;
	int nStrXOffset, nStrYOffset;
	char	sz[100], szYFormat[10];	
	
	memset(sz, 0, sizeof(sz));
	snprintf(szYFormat, 10, "%%.%df", m_wYDigits);	
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	//g_tmDebugInfo->PrintDebugInfo("m_wGridCol = %d, nYNit=%d\n", m_wGridCol, nYUnit);
	snprintf(sz, 100, szYFormat, m_dYMin);
	nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
	nStrYOffset	= m_pcFont->m_wHeight;
	
	ShowExtentEx(m_rectYAxesScale.left,
		m_rectYAxesScale.bottom - nStrYOffset, sz);
		
		
	for(int i = 1; i <= m_wGridCol; i ++)
	{
		memset(sz, 0, sizeof(sz));
		snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
		nStrYOffset	= m_pcFont->m_wHeight / 2;
		ShowExtentEx(m_rectYAxesScale.left,	m_rectYAxesScale.bottom - nYUnit * i - nStrYOffset, sz);
		
	}
	m_dYMin = dYMin;
    m_dYMax = dYMax;
    m_dYUnit= dYUnit;
   
}

void	CtmHistogram::DrawName()
{
	if (m_bXName) DrawXName();
	if (m_bYName) DrawYName();
}

void	CtmHistogram::DrawXName()
{
	int nStrXOffset = 0;
	int nStrYOffset = 0;
	if(m_pszXName != NULL)
	{
		nStrXOffset	= ((m_rectXName.right - m_rectXName.left) 
						- strlen(m_pszXName) * m_pcFont->m_wWidth / 2)
						/ 2;
		
		nStrYOffset	= m_pcFont->m_wHeight + 1;
		ShowExtentEx(m_rectXName.left + nStrXOffset,m_rectXName.top,
					m_pszXName, m_pcFont->GetFontCode());
	}
	
	
}

void	CtmHistogram::DrawYName()
{
	
	if(m_pszYName != NULL)
	{
		int nStrXOffset = 0;
		int nStrYOffset = 0;
		char	sz[100];	
		int nYStrLen  = 0;
		BYTE 	*pChar;
		WORD	*pCode;
		int		nStrLen		= 0;
		
		pChar	= (BYTE *)m_pszYName;
		while(*((WORD*)pChar) != 0)
		{
			nStrLen	   += m_pcFont->m_wHeight;
			pChar +=2;
		}
		
		nStrYOffset	= ((m_rectYName.bottom - m_rectYName.top) 
						- nStrLen) / 2;
		pChar	= (BYTE *)m_pszYName;
		
		BOOL	bDoubleChar = FALSE;
		while(*((WORD*)pChar) != 0)
		{		
			//sz[2]	= 
			//sz[1]	= 0;
			memset(sz,0,sizeof(sz));
			nStrXOffset = m_pcFont->m_wWidth + nYStrLen;
			sz[0]	= *pChar;		
			sz[1]	= *(pChar + 1);		
			pChar +=2;
			
			ShowExtentEx( m_rectYName.left, 
							m_rectYName.top + nStrYOffset, sz, m_pcFont->GetFontCode());
			
			nStrYOffset += m_pcFont->m_wHeight;			
		}
	}
}

void	CtmHistogram::DrawCoordinate()
{
	int		i;
	int		nYStrLen;
	int		nXUnit, nYUnit;
	
	char	sz[100], szXFormat[10], szYFormat[10];	
	
	//========
	// 計算坐標系位置
	//========	
	snprintf(szXFormat, 10, "%%.%df", m_wXDigits);
	snprintf(szYFormat, 10, "%%.%df", m_wYDigits);
	
	
	
	snprintf(sz, 100, szYFormat, m_dYMin);	
	i	= strlen(sz) * m_pcFont->m_wWidth / 2;
	snprintf(sz, 100, szYFormat, m_dYMax);
	nYStrLen	= strlen(sz) * m_pcFont->m_wWidth / 2;
	
	nYStrLen	= (i > nYStrLen) ? i : nYStrLen;
		
	//=======
	// 畫网格
	//=======
	if (m_bShowGrid)
		DrawAxesGrid();
	//=======
	// 畫坐標軸
	//=======	
	DrawAxes();
	
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_wShowXCnt;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	//=======
	// 顯示坐標軸刻度
	//=======	
	DrawSclae();
	
	//=======
	// 顯示坐標軸名稱
	//=======
	DrawName();
	
	//=======
	// 顯示柱
	//=======
}

void	CtmHistogram::Show()
{
	PropareShow();
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{
		GetRangeXY();
			
		SeperateArea();	
		
		//	show title
		ShowTitle();
		
		//========
		// draw coordinate
		//========
		DrawCoordinate();
		
		//	double buffer
		if (m_pDoubleBuffer != NULL)		
			m_pDoubleBuffer->DrawWithDoubleBuffer();
		
		ShowGraph();
		CtmWnd::Show();
		
	}	

}


void	CtmHistogram::Update()
{
	Show();
}

BOOL	CtmHistogram::FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest)
{
	BOOL bResult = FALSE;
	if (pSource->pRect != NULL)
	{
		*pDest = pSource;
		while ((*pDest)->pNext != NULL)
		{
			*pDest = (*pDest)->pNext;
			
			
		}
		bResult = TRUE;
	}
	
	return bResult;
}

RECT	CtmHistogram::FindLast(tmRectRelation* pSource)
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	tmRectRelation* TempRelation;
	
	if (FindRelationEnd(pSource, &TempRelation))
		Result = *TempRelation->pRect;
	return Result;
}

int		CtmHistogram::GetLastLeft()
{
	int nResult = 0;
	RECT Temp;
	Temp = FindLast(&m_relationLeft);
	if (Temp.bottom > 0)
	{
		nResult = Temp.right;
	}
	else
	{
		nResult = m_rectCoordinate.left;
	}
	
	return nResult;
}

int		CtmHistogram::GetLastRight()
{
	int nResult = 0;
	RECT Temp;
	Temp = FindLast(&m_relationRight);
	if (Temp.bottom > 0)
	{
		nResult = Temp.left;
		
	}
	else
		nResult = m_rectCoordinate.right;	
	return nResult;
}


int		CtmHistogram::GetLastTop()
{
	int nResult = 0;
	RECT Temp;
	Temp = FindLast(&m_relationUp);
	
	if (Temp.bottom > 0)
		nResult = Temp.bottom;	
	else
		nResult = m_rectCoordinate.top;
	return nResult;
}

int		CtmHistogram::GetLastBottom()
{
	int nResult = 0;
	RECT Temp;
	Temp = FindLast(&m_relationDown);
	
	if (Temp.bottom > 0)
		nResult = Temp.top;
	else
		nResult = m_rectCoordinate.bottom;
	return nResult;
}

BOOL	CtmHistogram::AddTopRelation(tmRectRelation* pSource, int nHeight)
{
	BOOL bResult = FALSE;
	RECT TempRect;
	
	tmRectRelation* TempRelation;
	
	TempRect.left = GetLastLeft();
	TempRect.right = GetLastRight();
	TempRect.top = GetLastTop() + 1;	
	TempRect.bottom = TempRect.top + nHeight + 1;
	
	if (!FindRelationEnd(&m_relationUp, &TempRelation))
	{
		m_relationUp.pRect = pSource->pRect;
	}
	else
	{
		TempRelation->pNext = pSource;
	}
	
	*pSource->pRect = TempRect;
	bResult= TRUE;
	return bResult;
}
		
BOOL	CtmHistogram::AddDownRelation(tmRectRelation* pSource, int nHeight)
{
	BOOL bResult = FALSE;
	RECT TempRect;
	
	tmRectRelation* TempRelation;
	
	TempRect.left = GetLastLeft();
	TempRect.right = GetLastRight();
	TempRect.bottom = GetLastBottom() - 1;
	TempRect.top = TempRect.bottom - nHeight - 1;
	
	
	if (!FindRelationEnd(&m_relationDown, &TempRelation))
	{
		m_relationDown.pRect = pSource->pRect;
	}
	else
	{
		TempRelation->pNext = pSource;
		//TempRelation->pNext->pRect =  pSource->pRect;
	}
	
	*pSource->pRect = TempRect;
	bResult= TRUE;
	return bResult;
}
		
BOOL	CtmHistogram::AddLeftRelation(tmRectRelation* pSource, int nWidth)
{
	BOOL bResult = FALSE;
	RECT TempRect;
	
	tmRectRelation* TempRelation;
	
	TempRect.left = GetLastLeft() + 1;
	TempRect.right = TempRect.left + nWidth + 1;
	TempRect.bottom = GetLastBottom();
	TempRect.top = GetLastTop();
	
	
	
	if (!FindRelationEnd(&m_relationLeft, &TempRelation))
	{
		m_relationLeft.pRect = pSource->pRect;
	}
	else
	{
		TempRelation->pNext = pSource;
	}
	
	*pSource->pRect = TempRect;
	bResult= TRUE;
	return bResult;
}

BOOL	CtmHistogram::AddRightRelation(tmRectRelation* pSource, int nWidth)
{
	BOOL bResult = FALSE;
	RECT TempRect;
	
	tmRectRelation* TempRelation;
	
	TempRect.bottom = GetLastBottom();
	TempRect.top = GetLastTop();
	TempRect.right = GetLastRight() - 1;
	TempRect.left = TempRect.right - nWidth - 1;
	
	if (!FindRelationEnd(&m_relationRight, &TempRelation))
	{
		m_relationRight.pRect = pSource->pRect;
	}
	else
	{
		TempRelation->pNext = pSource;
	}
	
	*pSource->pRect = TempRect;
	bResult= TRUE;
	return bResult;
}


void	CtmHistogram::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	if (!m_bTouchSupport)
		return;
		
	switch(message)
    {
       case MSG_MOUSELUP:
       		MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSELDOWN:
       		MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSEMOVE:
        	MouseMove(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }
}

BOOL	CtmHistogram::MouseUp(short x_pos,short y_pos,int key_flag)
{
	return TRUE;
}
BOOL	CtmHistogram::MouseDown(short x_pos, short y_pos, int key_flag)
{
	return TRUE;
}
BOOL	CtmHistogram::MouseMove(short x_pos, short y_pos, int key_flag)
{
	return TRUE;
}
void	CtmHistogram::ShowGraph()
{
	int nYUnit;
	int  nLeft, nRight, nTop, nBottom, nTemp;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	
	int nStrLen;
	char	sz[100], szYFormat[10], szTm[200];	
	
	memset(sz, 0, sizeof(sz));
	memset(szTm, 0, sizeof(szTm));
	snprintf(szYFormat, 10, "%%.%df", m_wYDigits);	
	
	for(int i = 0; i < m_wShowXCnt; i++)
   	{
   		nTop = m_rectLine.bottom + m_rectClient.top - (m_GraphValue[i].dValue - m_dYMin)/ m_dYUnit * nYUnit;
   		m_pGraphStaticX2[i]->SetPropValueT("top", nTop);
   		m_pGraphStaticX2[i]->CreateA();
   		m_pGraphStaticX2[i]->Update();
   		
   		m_pGraphStaticX2[i]->GetPropValueT("left", &nLeft, sizeof(nLeft));
   		m_pGraphStaticX2[i]->GetPropValueT("right", &nRight, sizeof(nRight));
   		
   		if(!m_bShowValue) continue;
   		snprintf(sz, 100, szYFormat, m_GraphValue[i].dValue);
		nStrLen	= strlen(sz) * (m_pcFont->m_wWidth / 2);
		
		nTemp = (nStrLen - (nRight - nLeft))/2;
		
		if(nRight - nLeft < nStrLen)
		{
			nLeft  -= nTemp;
			nRight += nTemp;
		}
		nBottom = nTop - 1;
		nTop = nBottom -  m_pcFont->m_wHeight + 2; 
		
		CodeChange(szTm, sz);
		m_pStrStaticX2[i]->SetPropValueT("left", nLeft);
		m_pStrStaticX2[i]->SetPropValueT("right", nRight);
		m_pStrStaticX2[i]->SetPropValueT("top", nTop);
		m_pStrStaticX2[i]->SetPropValueT("bottom", nBottom);
		m_pStrStaticX2[i]->SetPropValueT("maxchar",	strlen(sz));
		
		m_pStrStaticX2[i]->CreateA();
		m_pStrStaticX2[i]->SetPropValueT("text", szTm);
		m_pStrStaticX2[i]->Update();
		
   	}
}

void	CtmHistogram::SetGraphValue(tmGraphValue* pGraphValue, int nCount)
{
	if(nCount > MAX_GRAPH_NUM) return;
	for(int i = 0; i < nCount; i++)
   	{
   		m_GraphValue[i].wIndex = pGraphValue[i].wIndex;
   		m_GraphValue[i].dValue = pGraphValue[i].dValue;
   	}
}
