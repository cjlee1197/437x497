/* Copyright (C), 2005-2010, Techmation. Co., Ltd.*/ 
 
/*==========================================================================+
|  Class    : Windows Extension                                             |
|  Task     : Windows Extension Manager header file                         |
|---------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                   |
|  Link     : G++(GCC)3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                     |
|  Version  : V1.00                                                         |
|  Creation : 22/08/2005                                                    |
|  Revision : V2.00	netdigger 2006/4/17                                     |
+==========================================================================*/
#include	"curve.h"
#include	"utils.h"
#include	"commonaction.h"
#include    "tmdebug.h"
//#include	"memwatch.h"
    
/*==========================================================================+
|           Constants	                                                    |
+==========================================================================*/
#define		CURVE_CUTLINELEN			15		// �ϨҪ��u����

#define		CURVE_CUTLINE	(CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT  | CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM)

//	2009-2-10 11:46:20 fans add
#define		CURVE_X_MIN_SETTING_MODIFY	0x0001
#define		CURVE_X_MAX_SETTING_MODIFY	0x0002
#define		CURVE_Y_MIN_SETTING_MODIFY	0x0004
#define		CURVE_Y_MAX_SETTING_MODIFY	0x0008
/*==========================================================================+
|           External		                                                |
+==========================================================================*/
/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef struct tagCURVEDRAW
{
	int		nType;
	void    (CCurve::*npFunc)(); 
}	CURVEDRAW;



IMPLEMENT_DYNCREATE(CCurve, CtmWnd)
IMPLEMENT_DYNCREATE(CtmCurveEx, CtmWnd)
IMPLEMENT_DYNCREATE(CtmCurveEx2, CtmWnd)
/*==========================================================================+
|           Class implementation - CtmCurve	                                |
+==========================================================================*/


CCurve::CCurve(tmWND* pwnd, CURVE* pCurve)	:  CtmWnd()
{
	m_pCurve 			= pCurve;
	SetBorderWidth(3);
	
	//m_pLine	= m_pCurve->pLine;		fans mark
	
	//memcpy(m_pLine, m_pCurve->pLine);	//	fans add
	
	CtmWnd::Create(pwnd);
	m_Image.CopyImage(m_rectClient.left, m_rectClient.top, \
	m_rectClient.right, m_rectClient.bottom);
	
	return;
}

//	fans add
CCurve::CCurve(): CtmWnd()
{
	strcpy(TYPEName, "CCurve");	
	SetBorderWidth(3);
	
	
	pszCurveNameID 	= NULL;
	pszXNameID 		= NULL;
	pxzYNameID 		= NULL;
	
	m_Curve.nFlag 	= 0xFF;
	m_Curve.nXDigits = 0;
	m_Curve.nYDigits = 0;
	m_dXStart 		= 0;
	m_dYStart 		= 0;
	m_dXMin 		= 0;
	m_dYMin 		= 0;
	m_dXMax 		= 0;
	m_dYMax 		= 0;
	m_dYUnit 		= 0;
	m_dXUnit 		= 0;
	m_Curve.nCol 	= 10;
	m_Curve.nRow 	= 10;
	m_Curve.pszName = NULL;
	
	m_Curve.pszXName = NULL;
	m_Curve.pszYName = NULL;
	m_iLineCout 	= 0;
	
	m_bTableLine	= TRUE;
	m_nXRulerLineColor	= YELLOW;
	m_nYRulerLineColor	= YELLOW;
	m_nMaxPoint			= 600;
	//	fans add 2009-2-10 11:41:54
	m_nSettingState	= 0;
	m_dXMinSet = m_dXMaxSet = m_dYMinSet = m_dYMaxSet = 0;
	
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		//m_pLine[i] = new LINE;
		m_pLine[i].nFlag = CURVE_CURVELINE;
		m_pLine[i].pszName = NULL;
		m_pLine[i].nCount = 0;
		m_pLine[i].pValue = NULL;	
		m_pLine[i].dwColor= 0xFFFF;
		m_pszLineNameID[i] = NULL;		
		
		m_BufferLine[i] = new LINEBUFFER;		
		memset(m_BufferLine[i], 0, sizeof(LINEBUFFER));
	}
	
	m_pDoubleBuffer = NULL;
	
}

CCurve::~CCurve()
{
	
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		//delete m_pLine[i];
		for (int j = 0; j < m_BufferLine[i]->nLineCount; j++)
		{
			if (m_BufferLine[i]->pValue[j] != NULL) 
			{
				delete []m_BufferLine[i]->pValue[j];
			}
		}
		delete m_BufferLine[i];
	}
	
	//	free the class
	delete m_pDoubleBuffer;
	
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  Show()														|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CCurve::Show()
{
	//	2009-2-10 12:02:51 fans add
	if (!(m_nSettingState & CURVE_X_MIN_SETTING_MODIFY))	
		m_dXMin = m_dXMinSet; 
	if (!(m_nSettingState & CURVE_X_MAX_SETTING_MODIFY))		
		m_dXMax = m_dXMaxSet;
	if (!(m_nSettingState & CURVE_Y_MIN_SETTING_MODIFY))		
		m_dYMin = m_dYMinSet;
	if (!(m_nSettingState & CURVE_Y_MAX_SETTING_MODIFY))				
	 	m_dYMax = m_dYMaxSet;
	

	if (m_dYMax == 0) 
	{
		m_dYMax = m_dYMin + m_pCurve->nRow * m_dYUnit;
	}
	else 
	{
		m_dYUnit = (m_dYMax - m_dYMin) / m_pCurve->nRow;
	}
	
	if (m_dXMax == 0) 
	{
		m_dXMax = m_dXMin + m_pCurve->nCol * m_dXUnit;
	}
	else
	{
		m_dXUnit = (m_dXMax - m_dXMin) / m_pCurve->nCol;
	}

	StrIDToStr(pszCurveNameID, &m_Curve.pszName);
	StrIDToStr(pszXNameID, &m_Curve.pszXName);
	//printf( "m_Curve.pszYName 1 = %s \n", m_Curve.pszYName );
	//char* cTemp = new char[100];
	//CodeChange(cTemp, m_Curve.pszYName);
	StrIDToStr(pxzYNameID, &m_Curve.pszYName);
	for (int i = 0 ; i < CURVE_LINE_MAX; i++)
	{
		StrIDToStr(m_pszLineNameID[i], &m_pLine[i].pszName);
	}
	
	//Clear();
	m_pDoubleBuffer->InitBuffer(m_bgc);
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{
		//=======
		// �p��X���жb���S��
		//=======
		GetRange();
			
		//=======
		// ��ܹϪ���D
		//=======
		m_rectCoordinate.left	= 1;
		m_rectCoordinate.right	= GetClientWidth() - 2;
		m_rectCoordinate.top	= 2;
		m_rectCoordinate.bottom	= GetClientHeight() -2;
		
		if(m_pCurve->pszName != NULL)
		{
			ShowExtentD((m_rectCoordinate.right - m_rectCoordinate.left 
						- strlen(m_pCurve->pszName) * m_pcFont->m_wWidth / 2) / 2, 
						m_rectCoordinate.top ,m_pCurve->pszName, m_pcFont->GetFontCode());
			m_rectCoordinate.top	+= m_pcFont->m_wHeight;
		}	
		
		if(m_pCurve->nFlag & CURVE_NOTDRAW) 
		{
			//cout << "Exit Curve.show" << m_pCurve->nFlag << endl;
			return;
		}
		
		//=======
		// �e�Ϩ�
		//=======
		DrawCutLine();
		//========
		// �e���Шt
		//========
		DrawCoordinate();
		//=======
		// �e�U���u
		//=======
		DrawLines();
		
		m_pDoubleBuffer->DrawWithDoubleBuffer();
		//	fans add
		CtmWnd::Show();
	}

	
}



/*--------------------------------------------------------------------------+
|  Function :  Update()														|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CCurve::Update()
{
	Show();
}

//	fans add
BOOL	CCurve::CreateA()
{
	CtmWnd::CreateA();
		
	//	2009-2-10 11:59:30 fans add	
	m_dXMinSet = m_dXMin;
	m_dXMaxSet = m_dXMax;
	m_dYMinSet = m_dYMin;
	m_dYMaxSet = m_dYMax;

	m_pCurve = &m_Curve;
	m_Image.CopyImage(m_rectClient.left, m_rectClient.top, \
	m_rectClient.right, m_rectClient.bottom);
	
	if(m_pDoubleBuffer == NULL)
	{
		m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
		m_pDoubleBuffer->InitBuffer(m_bgc);
	}
	return TRUE;
}

//	2009-2-10 11:49:03 fans add 
bool CCurve::SetPropValueT(char* pszPropName, double Value)
{
	bool	Result = CtmWnd::SetPropValueT(pszPropName, Value);
	
	if (strcmp(pszPropName, "xmin") == 0)
		m_nSettingState |= CURVE_X_MIN_SETTING_MODIFY;
	
	if (strcmp(pszPropName, "xmax") == 0)
		m_nSettingState |= CURVE_X_MAX_SETTING_MODIFY;
		
	if (strcmp(pszPropName ,"ymin") == 0)
		m_nSettingState |= CURVE_Y_MIN_SETTING_MODIFY;
		
	if (strcmp(pszPropName, "ymax") == 0)
		m_nSettingState |= CURVE_Y_MAX_SETTING_MODIFY;
	return Result;

}

//	fans add
void	CCurve::SetPropertys()
{
	CtmWnd::SetPropertys();

	SetPropInfo((char *)"curvetype",	tmINT,		&m_Curve.nFlag);	
	SetPropInfo((char *)"xdigits",		tmINT,		&m_Curve.nXDigits);
	SetPropInfo((char *)"ydigits",		tmINT,		&m_Curve.nYDigits);
	SetPropInfo((char *)"cols",			tmINT,		&m_Curve.nCol);
	SetPropInfo((char *)"rows",			tmINT,		&m_Curve.nRow);
	SetPropInfo((char *)"curvename",	tmUSTR,		&m_Curve.pszName);
	SetPropInfo((char *)"curvenameID",	tmSTR,		&pszCurveNameID);
	SetPropInfo((char *)"xname",		tmUSTR,		&m_Curve.pszXName);
	SetPropInfo((char *)"yname",		tmUSTR,		&m_Curve.pszYName);
	SetPropInfo((char *)"xnameID",		tmSTR,		&pszXNameID);
	SetPropInfo((char *)"ynameID",		tmSTR,		&pxzYNameID);
	SetPropInfo((char *)"xmin",			tmDOUBLE,	&m_dXMin);
	SetPropInfo((char *)"xmax",			tmDOUBLE,	&m_dXMax);
	SetPropInfo((char *)"ymin",			tmDOUBLE,	&m_dYMin);
	SetPropInfo((char *)"ymax",			tmDOUBLE,	&m_dYMax);
	SetPropInfo((char *)"xstart",		tmDOUBLE,	&m_dXStart);
	SetPropInfo((char *)"xend",			tmDOUBLE,	&m_dXEnd);
	SetPropInfo((char *)"ystart",		tmDOUBLE,	&m_dYStart);
	SetPropInfo((char *)"yend",			tmDOUBLE,	&m_dYEnd);
	SetPropInfo((char *)"xunit",		tmDOUBLE,	&m_dXUnit);
	SetPropInfo((char *)"yunit",		tmDOUBLE,	&m_dYUnit);
	SetPropInfo((char *)"tableline",	tmBOOL,		&m_bTableLine);			//James add 2011/7/13 �U�� 05:19:16
	SetPropInfo((char *)"xRulercolor",	tmINT,		&m_nXRulerLineColor);	//James add 2011/7/13 �U�� 05:19:16
	SetPropInfo((char *)"yRulercolor",	tmINT,		&m_nYRulerLineColor);	//James add 2011/7/13 �U�� 05:19:16
	SetPropInfo((char *)"maxpoint",		tmINT,		&m_nMaxPoint);			//James add 2015/1/19 16:18:11
	
	char Name[20];
	char temp[3];
	for (int i = 0 ; i < CURVE_LINE_MAX; i++)
	{
		temp[0] = '\0';	
		sprintf(temp, "%d", i);
		
		strcpy(Name, "linecolor");		
		strcat(Name, temp);
		SetPropInfo(Name, 	tmDWORD,	&m_pLine[i].dwColor);
	
		strcpy(Name, "linename");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmUSTR,	&m_pLine[i].pszName);
		
		strcpy(Name, "linenameID");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszLineNameID[i]);

	
		strcpy(Name, "linetype");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmINT,	&m_pLine[i].nFlag);

	}


	
}


void	CCurve::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();

	SetPropInfo(CURVE_CURVETYPE,	tmINT,		&m_Curve.nFlag);	
	SetPropInfo(CURVE_XDIGITS,		tmINT,		&m_Curve.nXDigits);
	SetPropInfo(CURVE_YDIGITS,		tmINT,		&m_Curve.nYDigits);
	SetPropInfo(CURVE_COLS,			tmINT,		&m_Curve.nCol);
	SetPropInfo(CURVE_ROWS,			tmINT,		&m_Curve.nRow);
	SetPropInfo(CURVE_CURVENAME,	tmUSTR,		&m_Curve.pszName);
	SetPropInfo(CURVE_CURVENAMEID,	tmSTR,		&pszCurveNameID);
	SetPropInfo(CURVE_XNAME,		tmUSTR,		&m_Curve.pszXName);
	SetPropInfo(CURVE_YNAME,		tmUSTR,		&m_Curve.pszYName);
	SetPropInfo(CURVE_XNAMEID,		tmSTR,		&pszXNameID);
	SetPropInfo(CURVE_YNAMEID,		tmSTR,		&pxzYNameID);
	SetPropInfo(CURVE_XMIN,			tmDOUBLE,	&m_dXMin);
	SetPropInfo(CURVE_XMAX,			tmDOUBLE,	&m_dXMax);
	SetPropInfo(CURVE_YMIN,			tmDOUBLE,	&m_dYMin);
	SetPropInfo(CURVE_YMAX,			tmDOUBLE,	&m_dYMax);
	SetPropInfo(CURVE_XSTART,		tmDOUBLE,	&m_dXStart);
	SetPropInfo(CURVE_XEND,			tmDOUBLE,	&m_dXEnd);
	SetPropInfo(CURVE_YSTART,		tmDOUBLE,	&m_dYStart);
	SetPropInfo(CURVE_YEND,			tmDOUBLE,	&m_dYEnd);
	SetPropInfo(CURVE_XUNIT,		tmDOUBLE,	&m_dXUnit);
	SetPropInfo(CURVE_YUNIT,		tmDOUBLE,	&m_dYUnit);
	
	for (int i = 0 ; i < CURVE_LINE_MAX; i++)
	{
		SetPropInfo(CURVE_LINECOLOR0 + i * 4, 	tmDWORD,	&m_pLine[i].dwColor);
		SetPropInfo(CURVE_LINENAME0 + i * 4, 	tmUSTR,	&m_pLine[i].pszName);
		SetPropInfo(CURVE_LINENAMEID0 + i * 4, 	tmSTR,	&m_pszLineNameID[i]);
		SetPropInfo(CURVE_LINETYPE0 + i * 4, 	tmINT,	&m_pLine[i].nFlag);

	}
}


/*--------------------------------------------------------------------------+
|  Function :  DrawLine()													|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CCurve::DrawLines()
{
	int		i;
	LINE	*pLine;
	RECT	rectDraw;
	
	if((m_rectCoordinate.right - m_rectCoordinate.left) == 0 &&
		(m_rectCoordinate.bottom - m_rectCoordinate.top) == 0)
		return -1;
		
	rectDraw.left	= m_rectClient.left 	 + m_rectCoordinate.left;
	rectDraw.right	= m_rectCoordinate.right + m_rectClient.left;
	rectDraw.top	= m_rectClient.top 		 + m_rectCoordinate.top;
	rectDraw.bottom	= m_rectCoordinate.bottom+ m_rectClient.top;
	
	//_SetViewPort(rectDraw.left, rectDraw.top,
	//	rectDraw.right, rectDraw.bottom);
	
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//if(m_pLine[i] == NULL)	continue;	fans mark
		
		//	fans add
		if (m_pLine[i].pValue == NULL) continue;
		pLine	= &m_pLine[i];
		
		switch(pLine->nFlag & CURVE_LINETYPE)
		{
			case CURVE_BEELINE:
				DrawBeeLine(pLine);
				break;
			case CURVE_CURVELINE:
				DrawCurveLine(pLine);
				break;
		}
	}
	DrawBufferLine();
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  DrawBeeLine(LNE *pLine)										|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CCurve::DrawBeeLine(LINE *pLine)
{
	int		nBottom;
	if(pLine == NULL)						return -1;
	if((pLine->nFlag & CURVE_BEELINE) == 0)	return -1;
		
	nBottom	= m_rectCoordinate.bottom - m_rectCoordinate.top;	
	
	_SetColor(pLine->dwColor);
	_MoveTo((int)((pLine->PTStart.dX - m_dXStart) / m_dXUnit), 
			nBottom - (int)((pLine->PTStart.dY - m_dYMin) / m_dYUnit));
	_LineTo((int)((pLine->PTEnd.dX - m_dXStart) / m_dXUnit), 
			nBottom - (int)((pLine->PTEnd.dY - m_dYMin) / m_dYUnit));
	return 0;
	
}

/*--------------------------------------------------------------------------+
|  Function :  DrawCurveLine(LNE *pLine)									|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CCurve::DrawCurveLine(LINE *pLine)
{
	
	int		i, nBottom;
	int nXUnit, nYUnit;
	
	if(pLine == NULL)							return -1;
	if((pLine->nFlag & CURVE_CURVELINE) == 0)	return -1;
	if(pLine->pValue == NULL)					return -1;	
	
	
		
	nBottom	= m_rectCoordinate.bottom - m_rectCoordinate.top;
	nXUnit	= (m_rectCoordinate.right - m_rectCoordinate.left) / m_pCurve->nCol;
	nYUnit	= (m_rectCoordinate.bottom - m_rectCoordinate.top) / m_pCurve->nRow;
	//_SetColor(pLine->dwColor);
	m_pDoubleBuffer->SetColorInBuffer(pLine->dwColor);
	
	
	m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit), 
	nBottom - (int)((pLine->pValue[0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectCoordinate.top);
		
	//_MoveTo((int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit), 
	//	nBottom - (int)((pLine->pValue[0].dY - m_dYMin)/ m_dYUnit * nYUnit));
	
	
	for(i = 0; i< pLine->nCount; i ++)
	{
		m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit), 			
				nBottom - (int)((pLine->pValue[i].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectCoordinate.top);
		//_LineTo((int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit), 			
		//	nBottom - (int)((pLine->pValue[i].dY - m_dYMin)/ m_dYUnit * nYUnit));
	}
	
	return 0;
}

int		CCurve::DrawBufferLine()
{
	int result = 0;
	int nBottom;
	int nXUnit, nYUnit;
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		for (int j = 0; j < m_BufferLine[i]->nLineCount; j++)
		{
			if (m_BufferLine[i]->nCount[j] > 0)
			{
				nBottom	= m_rectCoordinate.bottom - m_rectCoordinate.top;
				nXUnit	= (m_rectCoordinate.right - m_rectCoordinate.left) / m_pCurve->nCol;
				nYUnit	= (m_rectCoordinate.bottom - m_rectCoordinate.top) / m_pCurve->nRow;
	
				m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
				m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left + (int)((m_BufferLine[i]->pValue[j][0].dX - m_dXMin) / m_dXUnit * nXUnit), 
												nBottom - (int)((m_BufferLine[i]->pValue[j][0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectCoordinate.top);
												
				for (int k = 0; k < m_BufferLine[i]->nCount[j]; k++)
				{
					m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.left + (int)((m_BufferLine[i]->pValue[j][k].dX - m_dXMin) / m_dXUnit * nXUnit), 			
														nBottom - (int)((m_BufferLine[i]->pValue[j][k].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectCoordinate.top);
				}
			}
		}
		
	}
	
	return result;
}


//	������w��?���I������,��^���O�۹�_��Ҧb���󪺬۹理��
//	fans add
tmPoint	CCurve::GetSpecialPoint(int iLineIndex, int iPointIndex)
{
	RECT	rectDraw;
	rectDraw.left	= m_rectClient.left 	 + m_rectCoordinate.left;
	rectDraw.right	= m_rectCoordinate.right + m_rectClient.left;
	rectDraw.top	= m_rectClient.top 		 + m_rectCoordinate.top;	//	11���ӬO�b�Y
	rectDraw.bottom	= m_rectCoordinate.bottom+ m_rectClient.top;
	
	tmPoint result;
	LINE* pLine = NULL;
	int	nBottom;
	int nXUnit, nYUnit;
	
	if (iLineIndex >= CURVE_LINE_MAX) 
	{
		result.ix = -1;
		result.iy = -1;
		return result;
	}
	
	
	pLine = &m_pLine[iLineIndex];
	
	if ((iPointIndex >= pLine->nCount) || (pLine->pValue == NULL))
	{
		result.ix = -1;
		result.iy = -1;
		return result;
	}
	
	nBottom	= m_rectCoordinate.bottom - m_rectCoordinate.top;
	nXUnit	= (m_rectCoordinate.right - m_rectCoordinate.left) / m_pCurve->nCol;
	nYUnit	= (m_rectCoordinate.bottom - m_rectCoordinate.top) / m_pCurve->nRow;
	
	result.ix = (int)((pLine->pValue[iPointIndex].dX - m_dXMin) / m_dXUnit * nXUnit) + rectDraw.left;
	result.iy = m_rectCoordinate.top + (nBottom - (int)((pLine->pValue[iPointIndex].dY - m_dYMin)/ m_dYUnit * nYUnit)) + m_wnd.rect.top;
	
	
	return result;
	
}

//	fans add
tmPoint	CCurve::GetSpecilaPonitA(double dX, double dY)
{
	RECT	rectDraw;
	rectDraw.left	= m_rectClient.left 	 + m_rectCoordinate.left;
	rectDraw.right	= m_rectCoordinate.right + m_rectClient.left;
	rectDraw.top	= m_rectClient.top 		 + m_rectCoordinate.top;	//	11���ӬO�b�Y
	rectDraw.bottom	= m_rectCoordinate.bottom+ m_rectClient.top;
	
	tmPoint result;
	
	int	nBottom;
	int nXUnit, nYUnit;
	
	
	
	nBottom	= m_rectCoordinate.bottom - m_rectCoordinate.top;
	nXUnit	= (m_rectCoordinate.right - m_rectCoordinate.left) / m_pCurve->nCol;
	nYUnit	= (m_rectCoordinate.bottom - m_rectCoordinate.top) / m_pCurve->nRow;
	
	result.ix = (int)((dX - m_dXMin) / m_dXUnit * nXUnit) + rectDraw.left;
	result.iy = m_rectCoordinate.top + (nBottom - (int)((dY - m_dYMin)/ m_dYUnit * nYUnit)) + m_wnd.rect.top;
	
	
	return result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetRange()													|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CCurve::GetRange()
{
	int i;
	CURVEPOINT	*pPoint;
	for (int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		//if(m_pLine[nIndex] == NULL)	continue;
		if(m_pLine[nIndex].pValue == NULL) continue;
			
		pPoint	= m_pLine[nIndex].pValue;
		
		if (m_dXMax < pPoint->dX)
			m_dXMax	= pPoint->dX; 
		if (m_dXMin > pPoint->dX)
			m_dXMin	= pPoint->dX; 
		if (m_dYMax < pPoint->dY)
			m_dYMax	= pPoint->dY; 
		if (m_dYMin > pPoint->dY)
			m_dYMin	= pPoint->dY; 
		break;
	}
	
	for(int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		//if(m_pLine[nIndex] == NULL)	continue;
		if(m_pLine[nIndex].pValue == NULL) continue;
		pPoint	= m_pLine[nIndex].pValue;
		for(i = 0; i < m_pLine[nIndex].nCount; i ++)
		{
			if(m_dXMax < pPoint->dX)			m_dXMax	= pPoint->dX;
			else if(m_dXMin > pPoint->dX)		m_dXMin	= pPoint->dX;
			if(m_dYMax < pPoint->dY)			m_dYMax	= pPoint->dY;
			else if(m_dYMin > pPoint->dY)		m_dYMin	= pPoint->dY;
			pPoint ++;
		}
	}
	
	for(int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		for (i = 0; i < CURVE_BUFFERLINE_MAX; i++)
		{
			if (m_BufferLine[nIndex]->pValue[i] == NULL) continue;	
			pPoint = m_BufferLine[nIndex]->pValue[i];
			for (int j = 0; j < m_BufferLine[nIndex]->nCount[i]; j++)
			{
				if(m_dXMax < pPoint->dX)			m_dXMax	= pPoint->dX;
				else if(m_dXMin > pPoint->dX)		m_dXMin	= pPoint->dX;
				if(m_dYMax < pPoint->dY)			m_dYMax	= pPoint->dY;
				else if(m_dYMin > pPoint->dY)		m_dYMin	= pPoint->dY;
				pPoint ++;
			}
		}
	}
	m_dYUnit = (m_dYMax - m_dYMin) / m_pCurve->nRow;
	m_dXUnit = (m_dXMax - m_dXMin) / m_pCurve->nCol;
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  DrawCutLine()												|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CCurve::DrawCutLine()
{
	int		i;
	int		nLineCount = 0;
	int		nCutLineWidth, nCutLineHeight;
	int		nCutLineLen, nCutLineStrMaxLen, nCutLineStrTotalLen;
	int		nStrLen;
	int		nStartLen, nMiddleLen, nEndLen;
	int		nVCutLine,		nHCutLine;	
	RECT	rectCutLine;
	int		nLeft, nTop;
	
	//========
	// �p��ϨҪ��j�p
	//========
	nCutLineStrMaxLen	= 
	nCutLineStrTotalLen	= 0;
	
	if((m_pCurve->nFlag & CURVE_CUTLINE) == 0) 
	{
		return -1;	
	}
	int nFontType = 0;
	WORD wTemp = 0;
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//if(m_pLine[i] == NULL)	continue;	fans mark
		
		//	fans add James modify 2008/11/19 9:53AM
		if (m_pLine[i].pValue  == NULL &&
            m_BufferLine[i]->nLineCount <= 0) continue;
		
		
		nLineCount ++;
		
		if(m_pLine[i].pszName == NULL) continue;
		
		//	fans modify 2007/9/15 09:07�W��
		//nStrLen	= strlen(m_pLine[i].pszName);
		nStrLen = StrLenTM(m_pLine[i].pszName);
		if (nFontType == 0)
		{
			for (int j = 0; j < nStrLen / 2; j++)
			{
				memcpy(&wTemp, &m_pLine[i].pszName[j * 2], sizeof(wTemp));
				if(wTemp > 0xFF) 
				{
					nFontType = 1;
					break;
				}
			}
		}
		
		nCutLineStrMaxLen	= (nStrLen > nCutLineStrMaxLen) ? 
								nStrLen :  nCutLineStrMaxLen;
		nCutLineStrTotalLen	   += nStrLen;		
	}
	if(nLineCount == 0)	return 0;
		
	if (nFontType == 0)
	{
		nCutLineStrMaxLen	= nCutLineStrMaxLen * m_pcFont->m_wWidth / 4;
		nCutLineStrTotalLen	= nCutLineStrTotalLen * m_pcFont->m_wWidth / 4;	
	}
	else
	{
		nCutLineStrMaxLen	= nCutLineStrMaxLen * m_pcFont->m_wWidth / 2;
		nCutLineStrTotalLen	= nCutLineStrTotalLen * m_pcFont->m_wWidth / 2;	
	}
	
	nCutLineLen	= 16;
	nStartLen	=
	nMiddleLen	=
	nEndLen		= 5;
	
	if(m_pCurve->nFlag & (CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT))
	{
		nCutLineWidth	= nCutLineStrMaxLen + nCutLineLen 
							+ nStartLen + nMiddleLen + nEndLen;
		nCutLineHeight	= (m_pcFont->m_wHeight + 2) * nLineCount;
		
		nVCutLine	= 1;
		nHCutLine	= 0;
	}
	else if(m_pCurve->nFlag & (CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM))
	{
		nCutLineWidth	= nCutLineStrTotalLen +	(nStartLen + nMiddleLen 
							+ nEndLen + nCutLineLen) * nLineCount;
		nCutLineHeight	= m_pcFont->m_wHeight + 2;
		nVCutLine	= 0;
		nHCutLine	= 1;
	}
	
	//======
	// �p��ϨҦ�m
	//======
	switch(m_pCurve->nFlag & CURVE_CUTLINE)
	{
		case CURVE_CUTLINE_LEFT:
			rectCutLine.left	= m_rectCoordinate.left + 1;
			rectCutLine.right	= rectCutLine.left + nCutLineWidth;
			rectCutLine.top	= (m_rectCoordinate.bottom 
						- m_rectCoordinate.top - nCutLineHeight) / 2;
			rectCutLine.bottom= rectCutLine.top + nCutLineHeight;
			
			m_rectCoordinate.left	= rectCutLine.right + 1;
			break;
		case CURVE_CUTLINE_RIGHT:
			rectCutLine.right	= m_rectCoordinate.right - 1;
			rectCutLine.left	= rectCutLine.right - nCutLineWidth;			
			rectCutLine.top	= (m_rectCoordinate.bottom 
						- m_rectCoordinate.top - nCutLineHeight) / 2;
			rectCutLine.bottom= rectCutLine.top + nCutLineHeight;
			
			m_rectCoordinate.right	= rectCutLine.left - 1;
			break;
		case CURVE_CUTLINE_TOP:
			rectCutLine.left	= (m_rectCoordinate.right 
						- m_rectCoordinate.left - nCutLineWidth) / 2;
			rectCutLine.right	= rectCutLine.left + nCutLineWidth;
			rectCutLine.top	= m_rectCoordinate.top + 2;
			rectCutLine.bottom= rectCutLine.top + nCutLineHeight;
			
			m_rectCoordinate.top= rectCutLine.bottom + 1;			
			break;
		case CURVE_CUTLINE_BOTTOM:
			rectCutLine.left	= (m_rectCoordinate.right 
						- m_rectCoordinate.left - nCutLineWidth) / 2;
			rectCutLine.right	= rectCutLine.left + nCutLineWidth;
			rectCutLine.bottom= m_rectCoordinate.bottom;
			rectCutLine.top	= rectCutLine.bottom - nCutLineHeight;
			
			m_rectCoordinate.bottom	= rectCutLine.top - 1;
			break;
		default	:;
	}
	
	//=======
	// �e�Ϩ�
	//=======
	m_pDoubleBuffer->SetColorInBuffer(BLACK);
	m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER, rectCutLine.left, 
											rectCutLine.top,
											rectCutLine.right,	
											rectCutLine.bottom);
	//_SetColor(BLACK);
	//_SetViewPort(m_rectClient.left, m_rectClient.top,
	//	m_rectClient.right, m_rectClient.bottom);
	//_Rectangle(_GBORDER, rectCutLine.left, rectCutLine.top,
	//	rectCutLine.right,	rectCutLine.bottom);
		
	nLeft	= rectCutLine.left;
	nTop	= rectCutLine.top;
	//_SetViewPort(m_rectClient.left, m_rectClient.top,
	//	m_rectClient.right, m_rectClient.bottom);	
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//if(m_pLine[i] == NULL)	continue;		fans mark
		
		//	fans add  James modify 2008/11/19 9:53AM
		if(m_pLine[i].pValue == NULL &&
            m_BufferLine[i]->nLineCount <= 0)	continue;		
		
		//_SetViewPort(m_rectClient.left, m_rectClient.top,
		//m_rectClient.right, m_rectClient.bottom);
			
		//_SetColor(m_pLine[i].dwColor);
		m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
		
		m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR, 
					nLeft + nStartLen, 
					nTop + m_pcFont->m_wHeight / 2,
					nLeft + nStartLen + nCutLineLen, 
					nTop + m_pcFont->m_wHeight / 2 + 2
				  );
		//_Rectangle(	_GFILLINTERIOR, 
		//			nLeft + nStartLen, 
		//			nTop + m_pcFont->m_wHeight / 2,
		//			nLeft + nStartLen + nCutLineLen, 
		//			nTop + m_pcFont->m_wHeight / 2 + 2
		//		  ); 

		if(m_pLine[i].pszName == NULL)
		{
			nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen) * nHCutLine;
			nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
			continue;
		}
		

		ShowExtentD(	nLeft + nStartLen + nMiddleLen + nCutLineLen,
					nTop  + 2, m_pLine[i].pszName, m_pcFont->GetFontCode());
		
					
		nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen 
					+ GetStringExtent(m_pLine[i].pszName).w) * nHCutLine;
		nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
	}

	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  DrawCoordinate()												|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CCurve::DrawCoordinate()
{
	int		i, nStrXOffset, nStrYOffset;
	int		nYStrLen;
	int		nXUnit, nYUnit;
	
	char	sz[100], szXFormat[10], szYFormat[10];	
	//char*	cTemp = new char[100];
	//CodeChange( cTemp,m_pCurve->pszYName);
	//========
	// �p�⧤�Шt��m
	//========	
	snprintf(szXFormat, 10, "%%.%df", m_pCurve->nXDigits);
	snprintf(szYFormat, 10, "%%.%df", m_pCurve->nYDigits);
	
	
	
	snprintf(sz, 100, szYFormat, m_dYMin);	
	i	= strlen(sz) * m_pcFont->m_wWidth / 2;
	snprintf(sz, 100, szYFormat, m_dYMax);
	nYStrLen	= strlen(sz) * m_pcFont->m_wWidth / 2;
	
	nYStrLen	= (i > nYStrLen) ? i : nYStrLen;
		
	m_rectCoordinate.left  += nYStrLen + 1;
	m_rectCoordinate.bottom-= m_pcFont->m_wHeight + 2;
	
	if(m_pCurve->pszXName != NULL)
		m_rectCoordinate.bottom	   -= m_pcFont->m_wHeight;
	
	if(m_pCurve->pszYName != NULL)
		m_rectCoordinate.left  += m_pcFont->m_wWidth + 1;
	
	//_SetViewPort(m_rectClient.left, m_rectClient.top,
	//	m_rectClient.right, m_rectClient.bottom);	
	
	//=======
	// �e���жb
	//=======	
//	printf("m_nXRulerLineColor=%x m_nYRulerLineColor=%x \n", m_nXRulerLineColor, m_nYRulerLineColor);
	m_pDoubleBuffer->SetColorInBuffer(m_nYRulerLineColor);			//�]�mY�Фت��C�� James add 2011/7/13 �U�� 05:33:37
	m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left, m_rectCoordinate.bottom);
	m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.left, m_rectCoordinate.top);            
	m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.left - 2, m_rectCoordinate.top + 8);    
	m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left, m_rectCoordinate.top);            
	m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.left + 2, m_rectCoordinate.top + 8);    
	m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left, m_rectCoordinate.bottom);        
	m_pDoubleBuffer->SetColorInBuffer(m_nXRulerLineColor); 			//�]�mX�Фت��C�� James add 2011/7/13 �U�� 05:33:37
	m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.right, m_rectCoordinate.bottom);        
	m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.right - 8, m_rectCoordinate.bottom + 2);
	m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.right, m_rectCoordinate.bottom);        
	m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.right - 8, m_rectCoordinate.bottom - 2);
	
	
	//=======
	// �e�I��
	//=======
	//_SetColor(RGB2PIXEL(0x88,0x88,0x88));
	m_pDoubleBuffer->SetColorInBuffer(RGB2PIXEL(0x88,0x88,0x88));
	m_rectCoordinate.right -= 8;
	m_rectCoordinate.top   += 8;
	
	
	nXUnit	= (m_rectCoordinate.right - m_rectCoordinate.left) / m_pCurve->nCol;
	nYUnit	= (m_rectCoordinate.bottom - m_rectCoordinate.top) / m_pCurve->nRow;
	
	
	m_rectCoordinate.right	= m_rectCoordinate.left + nXUnit * m_pCurve->nCol;
	m_rectCoordinate.top	= m_rectCoordinate.bottom - nYUnit * m_pCurve->nRow;
	
	if(m_bTableLine)				//�O�_�e���� jame add 2011/7/13 �U�� 05:32:30  
	{
		for(i = 1; i <= m_pCurve->nCol; i ++)
		{
			m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left + nXUnit * i,	m_rectCoordinate.bottom - 1); 
			m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.left + nXUnit * i, m_rectCoordinate.top);         
			//_MoveTo(m_rectCoordinate.left + nXUnit * i,	m_rectCoordinate.bottom - 1);
			//_LineTo(m_rectCoordinate.left + nXUnit * i, m_rectCoordinate.top);
		}
		
		for(i = 1; i <= m_pCurve->nRow; i ++)
		{
			m_pDoubleBuffer->MoveToInBuffer(m_rectCoordinate.left + 1, m_rectCoordinate.bottom - nYUnit * i);   
			m_pDoubleBuffer->DrawLineToInBuffer(m_rectCoordinate.right, m_rectCoordinate.bottom - nYUnit * i);      
			//_MoveTo(m_rectCoordinate.left + 1, m_rectCoordinate.bottom - nYUnit * i);
			//_LineTo(m_rectCoordinate.right, m_rectCoordinate.bottom - nYUnit * i);
		}
	}
	//=======
	// ��ܧ��жb���
	//=======	
	if(m_pCurve->nFlag & CURVE_XCONTRARY)
	{
		double temp = m_dXMin;
		m_dXMin = m_dXMax;
		m_dXMax = temp;
		m_dXUnit	= -m_dXUnit;
	}
	snprintf(sz, 100, szXFormat, m_dXMin);//m_dXStart);
	nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
	nStrYOffset	= 1;
	ShowExtentD(m_rectCoordinate.left - nStrXOffset,
		m_rectCoordinate.bottom + nStrYOffset, sz);

	if(!((m_dXMin==0)&&(m_dYMin==0)))	//Sunny<20080305>changed when XMin and YMin also zero,then show only XMin
	{
		snprintf(sz, 100, szYFormat, m_dYMin);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
		nStrYOffset	= m_pcFont->m_wHeight;
		ShowExtentD(m_rectCoordinate.left - nStrXOffset,
			m_rectCoordinate.bottom - nStrYOffset, sz);
	}
	/*
	m_dXUnit	= (m_dXMax - m_dXMin) 
					/ (m_rectCoordinate.right - m_rectCoordinate.left);
	m_dYUnit	= (m_dYMax - m_dYMin) 
					/ (m_rectCoordinate.bottom - m_rectCoordinate.top);
	*/
	
	/*
	if(m_pCurve->nFlag & CURVE_XCONTRARY)
	{
		m_dXUnit	= -m_dXUnit;
	}
	*/
	
	for(i = 1; i <= m_pCurve->nCol; i ++)
	{
		snprintf(sz, 100, szXFormat, m_dXUnit * i + m_dXMin);//m_dXStart);//m_dXUnit * nXUnit * i + m_dXStart);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
		if(nStrXOffset > GetClientWidth() - m_rectCoordinate.right)
			nStrXOffset	   += nStrXOffset - 
						(GetClientWidth() - m_rectCoordinate.right);
		nStrYOffset	= 1;
		ShowExtentD(m_rectCoordinate.left + nXUnit * i - nStrXOffset,
					m_rectCoordinate.bottom + nStrYOffset, sz);		
		
	}
	
	for(i = 1; i <= m_pCurve->nRow; i ++)
	{
		snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);//m_dYUnit * nYUnit * i + m_dYMin);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
		nStrYOffset	= m_pcFont->m_wHeight / 2;
		ShowExtentD(m_rectCoordinate.left - nStrXOffset,
					m_rectCoordinate.bottom - nYUnit * i - nStrYOffset, sz);
	}
	
	//=======
	// ��ܧ��жb�W��
	//=======
	if(m_pCurve->pszXName != NULL)
	{
		nStrXOffset	= ((m_rectCoordinate.right - m_rectCoordinate.left) 
						- strlen(m_pCurve->pszXName) * m_pcFont->m_wWidth / 2)
						/ 2;
		
		nStrYOffset	= m_pcFont->m_wHeight + 1;
		ShowExtentD(m_rectCoordinate.left + nStrXOffset,
					m_rectCoordinate.bottom + nStrYOffset, 
					m_pCurve->pszXName, m_pcFont->GetFontCode());
	}
	if(m_pCurve->pszYName != NULL)
	{
		BYTE 	*pChar;
		WORD	*pCode;
		int		nStrLen		= 0;
		
		pChar	= (BYTE *)m_pCurve->pszYName;
		while(*pChar != 0)
		{
			nStrLen	   += m_pcFont->m_wHeight;
			pChar +=2;
		}
		
		
		nStrYOffset	= ((m_rectCoordinate.bottom - m_rectCoordinate.top) 
						- nStrLen) / 2;
		//pChar	= (BYTE *)m_pCurve->pszYName;
		pChar = (BYTE *)m_Curve.pszYName;
		
		while(*pChar != 0)
		{	
			
			sz[2]	= 0;
			sz[1]	= 0;
			if(*pChar > 0xA0)
			{
				pCode	= (WORD *)pChar;				
				nStrXOffset	= m_pcFont->m_wWidth + nYStrLen;				
				sz[0]	= *pChar;
				//sz[1]   = ' ';
				sz[1]	= *(pChar + 1);
				pChar += 2;
			}
			else
			{
				nStrXOffset = m_pcFont->m_wWidth  + nYStrLen;
				sz[0]	= *pChar;	
				sz[1]	= *(pChar + 1);	
				//sz[1]	= ' ';		
				pChar +=2;
			}
			
			char cTemp[100];									// �B�z�h��y����ܪ��r�� James add 2007/9/5 12:39�U��
			
			CodeChange(cTemp,sz);
			
			if( sz[1] == 0 ) 									//�P�_�O���r�`�٬O��r�`.
			{
				ShowExtentD( m_rectCoordinate.left - nStrXOffset, 
						m_rectCoordinate.top + nStrYOffset,	cTemp, m_pcFont->GetFontCode());
			}
			else
			{		
				//CodeChange(&sz[2],&cTemp[2]);
				sz[2]	= 0;
				sz[3] 	= 0;
				ShowExtentD( m_rectCoordinate.left - nStrXOffset, 
								m_rectCoordinate.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
			}
			nStrYOffset += m_pcFont->m_wHeight;			
		}
	}
	
	return 0;
}



//	fans add
void	CCurve::SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount)	//	fans add
{
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
		{
			SetPoint(i, pValue, iCount);
			break;
		}
	}
}

//	fans add
void	CCurve::SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount)	//	fans add
{
	m_pLine[iLineIndex].nCount = iCount;
	if (pValue == NULL) m_pLine[iLineIndex].nCount = 0;
	
	if (iCount == 0) m_pLine[iLineIndex].pValue = NULL;
	else m_pLine[iLineIndex].pValue = pValue;
}

short   CCurve::ShowExtentD(short nX, short nY, char* pszString, int nType)
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

void	CCurve::SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount)	//	fans add 
{
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
		{
			SetPointBuffer(i, pValue, iCount);
			break;
		}
	}
}

void	CCurve::SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount)
{
	if (m_BufferLine[iLineIndex]->nLineCount > 0)
	{
		if (iCount > m_nMaxPoint) 	//James modify MAX_POINT_NUM-->m_nMaxPoint 2015/1/19 16:20:01
			iCount = m_nMaxPoint;
		if (pValue == NULL) iCount = 0;		
		m_BufferLine[iLineIndex]->nCount[m_BufferLine[iLineIndex]->nIndex] = iCount;		
		memcpy(m_BufferLine[iLineIndex]->pValue[m_BufferLine[iLineIndex]->nIndex++], 
			  pValue, sizeof(CURVEPOINT) * iCount);
		
		if (m_BufferLine[iLineIndex]->nIndex >= m_BufferLine[iLineIndex]->nLineCount) 
			m_BufferLine[iLineIndex]->nIndex = 0;
	}
}

BOOL	CCurve::SetLineNum(char* pszLineName, int iNum)
{
	BOOL	result = FALSE;
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
		{
			SetLineNum(i, iNum);
			break;
		}
	}
	return result;
}

BOOL	CCurve::SetLineNum(int	iLineIndex, int iNum)
{
	BOOL	result = FALSE;
	if ((iNum <= CURVE_BUFFERLINE_MAX) && (iLineIndex < CURVE_LINE_MAX))
	{
		if (iNum > 0)
		{
			for (int i = 0; i < iNum; i++)
			{
				if (m_BufferLine[iLineIndex]->pValue[i] == NULL)
				{
					
					m_BufferLine[iLineIndex]->pValue[i] = new CURVEPOINT[m_nMaxPoint];		//James modify MAX_POINT_NUM --> m_nMaxPoint 2015/1/19 16:20:26
					memset(m_BufferLine[iLineIndex]->pValue[i], 0, sizeof(CURVEPOINT) * m_nMaxPoint);
					
					m_BufferLine[iLineIndex]->nIndex = 0;
					m_BufferLine[iLineIndex]->nCount[i] = 0;	//	the num of the valid value
					//m_nALineIndex[iLineIndex] = 0;
				}
			}
			//m_nALineNum[iLineIndex] = iNum;
			m_BufferLine[iLineIndex]->nLineCount = iNum;
		}
		else
		{
			for (int i = 0; i < m_BufferLine[iLineIndex]->nLineCount; i++)
			{
				if (m_BufferLine[iLineIndex]->pValue[i] != NULL)
				{
					delete []m_BufferLine[iLineIndex]->pValue[i];
					m_BufferLine[iLineIndex]->pValue[i] = NULL;
				}	
				m_BufferLine[iLineIndex]->nIndex  = 0;			
			}
			m_BufferLine[iLineIndex]->nLineCount = 0;
		}
		result = TRUE;
	}
	
	return result;
}


/*==========================================================================+
|           Class implementation - CtmCurve	                                |
+==========================================================================*/

/**
 * 
 */
void CtmCurveEx::InitData() 
{
    memset(&m_relationLeft	, 			0, sizeof(m_relationLeft	));
	memset(&m_relationRight  , 			0, sizeof(m_relationRight   ));
	memset(&m_relationUp    , 			0, sizeof(m_relationUp      ));
	memset(&m_relationDown , 			0, sizeof(m_relationDown    ));
	
	memset(&m_relationTitle      , 		0, sizeof(m_relationTitle     ));    
	memset(&m_relationCutLine    , 		0, sizeof(m_relationCutLine   ));    
	memset(&m_relationXName      , 		0, sizeof(m_relationXName     ));    
	memset(&m_relationXAxesScale , 		0, sizeof(m_relationXAxesScale));	
	memset(&m_relationYName	     , 		0, sizeof(m_relationYName	    ));    
	memset(&m_relationYAxesScale , 		0, sizeof(m_relationYAxesScale));	
	memset(&m_relationLine       , 		0, sizeof(m_relationLine      ));     
	
	
	
	memset(&m_rectTitle      	, 		0, sizeof(m_rectTitle     ));    
	memset(&m_rectCutLine    	, 		0, sizeof(m_rectCutLine   ));    
	memset(&m_rectXName      	, 		0, sizeof(m_rectXName     ));    
	memset(&m_rectXAxesScale	, 		0, sizeof(m_rectXAxesScale));	
	memset(&m_rectYName	    	, 		0, sizeof(m_rectYName	    ));    
	memset(&m_rectYAxesScale	, 		0, sizeof(m_rectYAxesScale));	
	memset(&m_rectLine       	, 		0, sizeof(m_rectLine      )); 
     
	                      
	m_relationTitle.pRect 		= &m_rectTitle;
	m_relationCutLine.pRect 	= &m_rectCutLine;
	m_relationXName.pRect 		= &m_rectXName;
	m_relationXAxesScale.pRect 	= &m_rectXAxesScale;
	m_relationYName.pRect 		= &m_rectYName;
	m_relationYAxesScale.pRect 	= &m_rectYAxesScale;
	m_relationLine.pRect 		= &m_rectLine;
}
CtmCurveEx::CtmCurveEx(): CtmWnd()
{
	strcpy(TYPEName, "CtmCurveEx");	
	
	m_pDoubleBuffer = NULL;
	InitData();
	                                         
	SetBorderWidth(3);                       
	                                         
	                                         
	m_pszCurveNameID = NULL;                 
	m_pszXNameID = NULL;
	m_pszYNameID = NULL;
	 
	m_Curve.nFlag = 0xFF;
	m_Curve.nXDigits = 0;
	m_Curve.nYDigits = 0;
	m_dXStart = 0;
	m_dYStart = 0;
	m_dXMin = 0;
	m_dYMin = 0;
	m_dXMax = 0;
	m_dYMax = 0;
	m_dYMin1 = 0;
	m_dYMax1 = 0;
	m_dYMin2 = 0;
	m_dYMax2 = 10;
	m_dYUnit = 0;
	m_dXUnit = 0;
	m_nReferenceFrame = 0;
	m_Curve.nCol = 10;
	m_Curve.nRow = 10;
	
	m_CurPos.ix		= 0;
	m_CurPos.iy		= 0;
	m_nOpMode	= CURVE_OPERATION_NONE;
	m_bOut					= TRUE;
	m_nXPosDelta =
	m_nYPosDelta = 0;
	m_dXValueDelta =
	m_dYValueDelta = 0;
	
	m_Curve.pszName = NULL;
	
	m_Curve.pszXName = NULL;
	m_Curve.pszYName = NULL;
	
	m_bDoubleBuffer = TRUE;
	m_bXName = TRUE;
	m_bYName = TRUE;
	m_bXArrow = TRUE;
	m_bYArrow = TRUE;
	m_nShowTitleType = CURVE_XXPOS_TOP;
	m_nXAxesColor	= 
	m_nYAxesColor	= YELLOW;
	m_nShowYAxesType = CURVE_XXPOS_LEFT;
	m_nShowXAxesType = CURVE_XXPOS_BOTTOM;
	
	m_nShowXScaleType = 
	m_nShowYScaleType = CURVE_VALUE_ALL;
	
	m_nXCoordinateColor = 
	m_nYCoordinateColor = RGB2PIXEL(0x88,0x88,0x88);
	
	m_bShowGrid = TRUE;
	memset(&m_rectCutLine, 0, sizeof(m_rectCutLine));
	memset(&m_monitor_attr,0,sizeof(m_monitor_attr));
	m_monitor_attr.color=0x134F;
	m_monitor_attr.line_width=2;
	
	for (int i = 0; i < 4; i++)	m_pszDBID[i] = NULL;
	
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		//m_pLine[i] 				= new LINE;
		m_pLine[i].nFlag 		= CURVE_CURVELINE;
		m_pLine[i].nRefernce	= 0;
		m_pLine[i].pszName 	= NULL;
		m_pLine[i].nCount 		= 0;
		m_pLine[i].pValue 		= NULL;	
		m_pLine[i].dwColor		= 0xFFFF;
		
		m_pszLineNameID[i] 		= NULL;		
		
		m_BufferLine[i] = new LINEBUFFER;		
		memset(m_BufferLine[i], 0, sizeof(LINEBUFFER));
	}
	
	
	//	fans add 2009/9/8 01:07�U��
	m_XRuleColor	= 0xFFFF;
	m_YRuleColor	= 0xFFFF;
	memset(m_szYRule, 0, sizeof(m_szYRule));
	memset(m_szXRule, 0, sizeof(m_szYRule));
	m_pFuncYStr		= NULL;
	m_pFuncXStr		= NULL;
	m_xPos			= 0;
	m_yPos			= 0;
	m_dXPosValue	= 0;
	m_xPerMove		= 1;
	m_yPerMove		= 1;
	m_dfPercent		= 0;
	m_pszPromptID	= NULL;
	m_pszPrompt		= NULL;	
	
	m_bTouchSupport = TRUE;
	
	m_nHRulePos		= 0;
	m_nVRulePos		= 0;
	m_RuleType		= RULE_NONE;		//�q�{�������Ф�
	m_nPointIndex	= 0;
	m_nLineIndex	= 0;
	
	m_nXRange		= 0;
	m_nInterval		= 1;
}

CtmCurveEx::~CtmCurveEx()
{
	
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		//delete m_pLine[i];
		for (int j = 0; j < m_BufferLine[i]->nLineCount; j++)
		{
			if (m_BufferLine[i]->pValue[j] != NULL) 
			{
				delete []m_BufferLine[i]->pValue[j];
			}
		}
		delete m_BufferLine[i];
	}
	
	//	free the class
	if (m_pDoubleBuffer != NULL)
	{
		delete m_pDoubleBuffer;
		m_pDoubleBuffer = NULL;
	}
	
}

//	fans add
BOOL	CtmCurveEx::CreateA()
{
	CtmWnd::CreateA();

	m_dYMin = m_dYMin1;	
	m_dYMax = m_dYMax1;
	
	m_dOldXMin = m_dXMin;
	m_dOldXMax = m_dXMax;
	m_dOldYMin = m_dYMin1;
	m_dOldYMax = m_dYMax1;
	
	m_dOldYMin2 = m_dYMin2;
	m_dOldYMax2 = m_dYMax2;
	m_nMaxPoint	= 600;
	//m_pCurve = &m_Curve;
	m_Image.CopyImage(m_rectClient.left, m_rectClient.top, \
	m_rectClient.right, m_rectClient.bottom);
	if (m_bDoubleBuffer && m_pDoubleBuffer == NULL)
	{
		m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
		m_pDoubleBuffer->InitBuffer(m_bgc);
	}
	
	
	//StrIDToStr(m_pszPromptID, &m_pszPrompt);
	if(m_pszPromptID!=NULL)
	{
		if(m_pszPrompt!=NULL)
		{	
			free(m_pszPrompt);
		}
		char* pszTemp=g_MultiLanguage[m_pszPromptID];
		if(pszTemp!=NULL)
		{
			int len=0;
			WORD* pwCode  = (WORD*)pszTemp;
			while(pwCode[len ++] != 0);
			m_pszPrompt = (char*)malloc(len<<1);//new char[(len << 1)];
			memcpy(m_pszPrompt, pszTemp, len << 1);
		}
	}
	return TRUE;
}

void	CtmCurveEx::SetPropertys()
{
	CtmWnd::SetPropertys();

	SetPropInfo((char *)"doublebuffer",	tmBOOL	,	&m_bDoubleBuffer);
	
	SetPropInfo((char *)"curvetype",	tmINT,		&m_Curve.nFlag);	
	SetPropInfo((char *)"xdigits",		tmINT,		&m_Curve.nXDigits);
	SetPropInfo((char *)"ydigits",		tmINT,		&m_Curve.nYDigits);
	SetPropInfo((char *)"cols",			tmINT,		&m_Curve.nCol);
	SetPropInfo((char *)"rows",			tmINT,		&m_Curve.nRow);
	SetPropInfo((char *)"curvename",	tmSTR,		&m_Curve.pszName);
	SetPropInfo((char *)"curvenameID",	tmSTR,		&m_pszCurveNameID);
	SetPropInfo((char *)"xname",		tmUSTR,		&m_Curve.pszXName);
	SetPropInfo((char *)"yname",		tmUSTR,		&m_Curve.pszYName);
	SetPropInfo((char *)"xnameID",		tmSTR,		&m_pszXNameID);
	SetPropInfo((char *)"ynameID",		tmSTR,		&m_pszYNameID);
	SetPropInfo((char *)"xmin",			tmDOUBLE,	&m_dXMin);
	SetPropInfo((char *)"xmax",			tmDOUBLE,	&m_dXMax);
	SetPropInfo((char *)"ymin",			tmDOUBLE,	&m_dYMin1);
	SetPropInfo((char *)"ymax",			tmDOUBLE,	&m_dYMax1);
	SetPropInfo((char *)"ymin2",		tmDOUBLE,	&m_dYMin2);
	SetPropInfo((char *)"ymax2",		tmDOUBLE,	&m_dYMax2);
	            
	SetPropInfo((char *)"xstart",		tmDOUBLE,	&m_dXStart);
	SetPropInfo((char *)"xend",			tmDOUBLE,	&m_dXEnd);
	SetPropInfo((char *)"ystart",		tmDOUBLE,	&m_dYStart);
	SetPropInfo((char *)"yend",			tmDOUBLE,	&m_dYEnd);
	SetPropInfo((char *)"xunit",		tmDOUBLE,	&m_dXUnit);
	SetPropInfo((char *)"yunit",		tmDOUBLE,	&m_dYUnit);
	            
	            
	SetPropInfo((char *)"xnameshow",	tmBOOL	,	&m_bXName);
	SetPropInfo((char *)"ynameshow",	tmBOOL	,	&m_bYName);
	SetPropInfo((char *)"xarrowshow",	tmBOOL	,	&m_bXArrow);
	SetPropInfo((char *)"yarrowshow",	tmBOOL	,	&m_bYArrow);
    
    SetPropInfo((char *)"xaxestype",	tmINT	,	&m_nShowXAxesType);
    SetPropInfo((char *)"yaxestype",	tmINT	,	&m_nShowYAxesType);          
    SetPropInfo((char *)"xscaletype",	tmINT	,	&m_nShowXScaleType);		    
    SetPropInfo((char *)"yscaletype",   tmINT	,	&m_nShowYScaleType);		    
    SetPropInfo((char *)"titletype",    tmINT	,	&m_nShowTitleType);		    
    SetPropInfo((char *)"cutlinetype",  tmINT	,	&m_nShowCutLineType);		
    SetPropInfo((char *)"xgridcolor",	tmINT	,	&m_nXCoordinateColor);       
    SetPropInfo((char *)"ygridcolor",   tmINT	,	&m_nYCoordinateColor);       
    SetPropInfo((char *)"xaxescolor",   tmINT	,	&m_nXAxesColor);             
    SetPropInfo((char *)"yaxescolor",   tmINT	,	&m_nYAxesColor);    
    
    
    SetPropInfo((char *)"showgrid",		tmBOOL,		&m_bShowGrid);			//Gerry add 2011/9/17 �U�� 03:22:15
    //	2008/4/2 10:49�W�� �аO�ۥثe���u�O�H���ӧ��Шt���D,����OY�b 
    SetPropInfo((char *)"reference",   tmINT	,	&m_nReferenceFrame);  
            
    //	fans add  2009/9/9 03:33�U��
    SetPropInfo((char *)"yrulecolor",	tmINT	,	&m_YRuleColor);
    SetPropInfo((char *)"xrulecolor",	tmINT	,	&m_XRuleColor);    
	SetPropInfo((char *)"xmove",		tmINT	,	&m_xPerMove);    
    SetPropInfo((char *)"ymove",		tmINT	,	&m_yPerMove);    
    SetPropInfo((char *)"prompt",		tmUSTR,		&m_pszPrompt);
	SetPropInfo((char *)"promptID",		tmSTR,		&m_pszPromptID);
	
	SetPropInfo((char *)"ruletype",		tmINT	,	&m_RuleType);
	SetPropInfo((char *)"touch",		tmBOOL,		&m_bTouchSupport);
	SetPropInfo((char *)"xrange", 		tmINT, &m_nXRange);
	SetPropInfo((char *)"baseline", 	tmINT, &m_nLineIndex);
	SetPropInfo((char *)"interval", 	tmINT, &m_nInterval);
	SetPropInfo((char *)"maxpoint", 	tmINT, &m_nMaxPoint);
	
	
	SetPropInfo((char *)"monitor", 		tmBOOL,&m_monitor_attr.flag);
	SetPropInfo((char *)"monicolor", 	tmINT, &m_monitor_attr.color);
	SetPropInfo((char *)"moniref", 		tmINT, &m_monitor_attr.reference);
	SetPropInfo((char *)"monilinewidth",tmINT, &m_monitor_attr.line_width);
	SetPropInfo((char *)"x1", 			tmDOUBLE, &m_monitor_attr.point1.dX);
	SetPropInfo((char *)"y1", 			tmDOUBLE, &m_monitor_attr.point1.dY);
	SetPropInfo((char *)"x2", 			tmDOUBLE, &m_monitor_attr.point2.dX);
	SetPropInfo((char *)"y2", 			tmDOUBLE, &m_monitor_attr.point2.dY);
	
	char Name[20];
	char temp[3];
	for (int i = 0 ; i < CURVE_LINE_MAX; i++)
	{
		temp[0] = '\0';	
		sprintf(temp, "%d", i);
		
		strcpy(Name, "linecolor");		
		strcat(Name, temp);
		SetPropInfo(Name, 	tmDWORD,	&m_pLine[i].dwColor);
	
		strcpy(Name, "linename");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmUSTR,	&m_pLine[i].pszName);
		
		strcpy(Name, "linenameID");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszLineNameID[i]);

	
		strcpy(Name, "linetype");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmINT,	&m_pLine[i].nFlag);
	}
	char sz[10];
	for (int i = 0; i < 4; i++)
	{
		sprintf(sz, "%s%d", "dbid", i);
		SetPropInfo(sz,		tmSTR, 		&m_pszDBID[i]);	
	}
		
}



void	CtmCurveEx::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();

	SetPropInfo(CURVEEX_DOUBLEBUFFER,	tmBOOL	,	&m_bDoubleBuffer);
	
	SetPropInfo(CURVEEX_CURVETYPE,	tmINT,		&m_Curve.nFlag);	
	SetPropInfo(CURVEEX_XDIGITS,		tmINT,		&m_Curve.nXDigits);
	SetPropInfo(CURVEEX_YDIGITS,		tmINT,		&m_Curve.nYDigits);
	SetPropInfo(CURVEEX_COLS,			tmINT,		&m_Curve.nCol);
	SetPropInfo(CURVEEX_ROWS,			tmINT,		&m_Curve.nRow);
	SetPropInfo(CURVEEX_CURVENAME,	tmSTR,		&m_Curve.pszName);
	SetPropInfo(CURVEEX_CURVENAMEID,	tmSTR,		&m_pszCurveNameID);
	SetPropInfo(CURVEEX_XNAME,		tmUSTR,		&m_Curve.pszXName);
	SetPropInfo(CURVEEX_YNAME,		tmUSTR,		&m_Curve.pszYName);
	SetPropInfo(CURVEEX_XNAMEID,		tmSTR,		&m_pszXNameID);
	SetPropInfo(CURVEEX_YNAMEID,		tmSTR,		&m_pszYNameID);
	SetPropInfo(CURVEEX_XMIN,			tmDOUBLE,	&m_dXMin);
	SetPropInfo(CURVEEX_XMAX,			tmDOUBLE,	&m_dXMax);
	SetPropInfo(CURVEEX_YMIN,			tmDOUBLE,	&m_dYMin1);
	SetPropInfo(CURVEEX_YMAX,			tmDOUBLE,	&m_dYMax1);
	SetPropInfo(CURVEEX_YMIN2,		tmDOUBLE,	&m_dYMin2);
	SetPropInfo(CURVEEX_YMAX2,		tmDOUBLE,	&m_dYMax2);
	
	SetPropInfo(CURVEEX_XSTART,		tmDOUBLE,	&m_dXStart);
	SetPropInfo(CURVEEX_XEND,			tmDOUBLE,	&m_dXEnd);
	SetPropInfo(CURVEEX_YSTART,		tmDOUBLE,	&m_dYStart);
	SetPropInfo(CURVEEX_YEND,			tmDOUBLE,	&m_dYEnd);
	SetPropInfo(CURVEEX_XUNIT,		tmDOUBLE,	&m_dXUnit);
	SetPropInfo(CURVEEX_YUNIT,		tmDOUBLE,	&m_dYUnit);
	
	
	SetPropInfo(CURVEEX_XNAMESHOW,	tmBOOL	,	&m_bXName);
	SetPropInfo(CURVEEX_YNAMESHOW,	tmBOOL	,	&m_bYName);
	SetPropInfo(CURVEEX_XARROWSHOW,	tmBOOL	,	&m_bXArrow);
	SetPropInfo(CURVEEX_YARROWSHOW,	tmBOOL	,	&m_bYArrow);
    
    SetPropInfo(CURVEEX_XAXESTYPE,	tmINT	,	&m_nShowXAxesType);
    SetPropInfo(CURVEEX_YAXESTYPE,	tmINT	,	&m_nShowYAxesType);          
    SetPropInfo(CURVEEX_XSCALETYPE,	tmINT	,	&m_nShowXScaleType);		    
    SetPropInfo(CURVEEX_YSCALETYPE,   tmINT	,	&m_nShowYScaleType);		    
    SetPropInfo(CURVEEX_TITLETYPE,    tmINT	,	&m_nShowTitleType);		    
    SetPropInfo(CURVEEX_CUTLINETYPE,  tmINT	,	&m_nShowCutLineType);		
    SetPropInfo(CURVEEX_XGRIDCOLOR,	tmINT	,	&m_nXCoordinateColor);       
    SetPropInfo(CURVEEX_YGRIDCOLOR,   tmINT	,	&m_nYCoordinateColor);       
    SetPropInfo(CURVEEX_XAXESCOLOR,   tmINT	,	&m_nXAxesColor);             
    SetPropInfo(CURVEEX_YAXESCOLOR,   tmINT	,	&m_nYAxesColor);    
    
    //	2008/4/2 10:49�W�� �аO�ۥثe���u�O�H���ӧ��Шt���D,����OY�b 
    SetPropInfo(CURVEEX_REFERENCE,   tmINT	,	&m_nReferenceFrame);  
            
    //	fans add  2009/9/9 03:33�U��
    SetPropInfo(CURVEEX_YRULECOLOR,	tmINT	,	&m_YRuleColor);
    SetPropInfo(CURVEEX_XRULECOLOR,	tmINT	,	&m_XRuleColor);    
	SetPropInfo(CURVEEX_XMOVE,		tmINT	,	&m_xPerMove);    
    SetPropInfo(CURVEEX_YMOVE,		tmINT	,	&m_yPerMove);    
    SetPropInfo(CURVEEX_PROMPT,		tmUSTR,		&m_pszPrompt);
	SetPropInfo(CURVEEX_PROMPTID,		tmSTR,		&m_pszPromptID);
	
	for (int i = 0 ; i < CURVE_LINE_MAX; i++)
	{
		SetPropInfo(CURVEEX_LINECOLOR0 + i*4, 	tmDWORD,	&m_pLine[i].dwColor);
		SetPropInfo(CURVEEX_LINENAME0 + i*4, 	tmUSTR,	&m_pLine[i].pszName);
		SetPropInfo(CURVEEX_LINENAMEID0 + i*4, 	tmSTR,	&m_pszLineNameID[i]);
		SetPropInfo(CURVEEX_LINETYPE0 + i*4, 	tmINT,	&m_pLine[i].nFlag);
	}	
}

//	fans add
void	CtmCurveEx::SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount, int nReference)	//	fans add
{
	//	2008/4/2 10:56�W��	fans mark
	//m_nReferenceFrame = nReference;	
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
		{
			SetPoint(i, pValue, iCount, nReference);
			break;
		}
	}
}

//	fans add
void	CtmCurveEx::SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference)	//	fans add
{	
	m_bPropertyChanged=TRUE;
	m_pLine[iLineIndex].nCount 	= iCount;
	m_pLine[iLineIndex].nRefernce	= nReference;		//	2008/4/2 10:58�W�� fans add
	if (pValue == NULL) m_pLine[iLineIndex].nCount = 0;
	
	if (iCount == 0) m_pLine[iLineIndex].pValue = NULL;
	else m_pLine[iLineIndex].pValue = pValue;
		
	if (m_pLine[iLineIndex].pValue != NULL)
	{
		if(nReference==1)
		{
			for (int i = 0; i < m_pLine[iLineIndex].nCount; i++)
	      	{
	      		if (m_pLine[iLineIndex].pValue[i].dY > m_dYMax2) 	m_dYMax2 = m_pLine[iLineIndex].pValue[i].dY+5;
	      		if (m_pLine[iLineIndex].pValue[i].dY < m_dYMin2)	m_dYMin2 = m_pLine[iLineIndex].pValue[i].dY+5;
	      	}
	     }
	     else
	     {
	     		for (int i = 0; i < m_pLine[iLineIndex].nCount; i++)
	     		{
		     		if (m_pLine[iLineIndex].pValue[i].dY > m_dYMax1) 	m_dYMax1 = m_pLine[iLineIndex].pValue[i].dY+5;
		      		if (m_pLine[iLineIndex].pValue[i].dY < m_dYMin1)	m_dYMin1 = m_pLine[iLineIndex].pValue[i].dY+5;
		      	}
	     }
	}
}

void	CtmCurveEx::SetPointT(int iLineIndex, CURVEPOINT *pValue, int iCount, int nReference)
{
	int i;
	
	if (iCount == 0)
		return;
	m_bPropertyChanged=TRUE;	
	CURVEPOINT	*pPoint = NULL, *points = new CURVEPOINT[iCount * 2];
	
	memset(points, 0, sizeof(CURVEPOINT) * iCount * 2);
	pPoint = points;
	
	for (i = 0; i < iCount; i++)
	{
		pPoint->dX = pValue[i].dX;
		pPoint->dY = pValue[i].dY;
		
		if (i == iCount - 1)
			break;
			
		(pPoint + 1)->dX = pValue[i].dX;
		(pPoint + 1)->dY = pValue[i + 1].dY;
		
		pPoint += 2;
	}
	memcpy(pValue, points, sizeof(CURVEPOINT) * (iCount * 2 - 1));
	SetPoint(iLineIndex, pValue, iCount * 2 - 1, nReference);
	delete []points;
}

void	CtmCurveEx::SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount, int nReference)
{
	//m_nReferenceFrame = nReference;
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
		{
			SetPointBuffer(i, pValue, iCount, nReference);
			break;
		}
	}
}

void	CtmCurveEx::SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount, int nReference)
{
	//	fans mark 2008/4/2 11:00�W��
	//m_nReferenceFrame = nReference;
	//if (0 == nReference)
	//{
	//	m_dYMin = m_dYMin1;
	//	m_dYMax = m_dYMax1;
	//}
	//else
	//{
	//	m_dYMin = m_dYMin2;
	//	m_dYMax = m_dYMax2;
	//}
	m_bPropertyChanged=TRUE;
	
	m_BufferLine[iLineIndex]->nRefernce = nReference;	//	2008/4/2 11:00�W�� fans add
	
	if (m_BufferLine[iLineIndex]->nLineCount > 0)
	{
		//printf("iCount=%d m_nMaxPoint=%d \n", iCount, m_nMaxPoint);
		if (iCount > m_nMaxPoint)	//James modify MAX_POINT_NUM-->m_nMaxPoint 2015/1/19 16:20:48
			iCount = m_nMaxPoint;
		if (pValue == NULL) 	iCount = 0;		
		
		
			
		m_BufferLine[iLineIndex]->nCount[m_BufferLine[iLineIndex]->nIndex] = iCount;		
		memcpy(m_BufferLine[iLineIndex]->pValue[m_BufferLine[iLineIndex]->nIndex++], 
			  pValue, sizeof(CURVEPOINT) * iCount);
		
		if (m_BufferLine[iLineIndex]->nIndex >= m_BufferLine[iLineIndex]->nLineCount) 
			m_BufferLine[iLineIndex]->nIndex = 0;
		
		if (1 == m_BufferLine[iLineIndex]->nRefernce)
		{			
			for (int i = 0; i < iCount; i++)
	      	{
	      		if (pValue[i].dY > m_dYMax2) 	
	      			m_dYMax2 = pValue[i].dY;
	      		
	      		if (pValue[i].dY < m_dYMin2)	
	      			m_dYMin2 = pValue[i].dY;
	
	      	}
	    }
	}

	
}

BOOL	CtmCurveEx::SetLineNum(char* pszLineName, int iNum)
{
	BOOL	result = FALSE;
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
		{
			SetLineNum(i, iNum);
			break;
		}
	}
	return result;
}

BOOL	CtmCurveEx::SetLineNum(int	iLineIndex, int iNum)
{
	BOOL	result = FALSE;
	if ((iNum <= CURVE_BUFFERLINE_MAX) && (iLineIndex < CURVE_LINE_MAX))
	{
		if (iNum > 0)
		{
			for (int i = 0; i < iNum; i++)
			{
				if (m_BufferLine[iLineIndex]->pValue[i] == NULL)
				{
					
					m_BufferLine[iLineIndex]->pValue[i] = new CURVEPOINT[m_nMaxPoint];		//James modify MAX_POINT_NUM --> m_nMaxPoint 2015/1/19 16:21:11
					memset(m_BufferLine[iLineIndex]->pValue[i], 0, sizeof(CURVEPOINT) * m_nMaxPoint);
					
					m_BufferLine[iLineIndex]->nIndex = 0;
					m_BufferLine[iLineIndex]->nCount[i] = 0;	//	the num of the valid value
					//m_nALineIndex[iLineIndex] = 0;
				}
			}
			//m_nALineNum[iLineIndex] = iNum;
			m_BufferLine[iLineIndex]->nLineCount = iNum;
		}
		else
		{
			for (int i = 0; i < m_BufferLine[iLineIndex]->nLineCount; i++)
			{
				if (m_BufferLine[iLineIndex]->pValue[i] != NULL)
				{
					delete []m_BufferLine[iLineIndex]->pValue[i];
					m_BufferLine[iLineIndex]->pValue[i] = NULL;
				}	
				m_BufferLine[iLineIndex]->nIndex  = 0;			
			}
			m_BufferLine[iLineIndex]->nLineCount = 0;
		}
		result = TRUE;
	}
	
	return result;
}

void	CtmCurveEx::PropareShow()
{
    InitData();
	//	Calculate Y
	if (m_dYMax == 0) m_dYMax = m_dYMin + m_Curve.nRow * m_dYUnit;
	else m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
	
	//	Calculate X
	if (m_dXMax == 0 && !(m_Curve.nFlag & CURVE_XCONTRARY)) m_dXMax = m_dXMin + m_Curve.nCol * m_dXUnit;
	else m_dXUnit = (m_dXMax - m_dXMin) / m_Curve.nCol;
	
	if (m_pszCurveNameID != NULL)
		StrIDToStr(m_pszCurveNameID, &m_Curve.pszName);
	if (m_pszXNameID != NULL)
		StrIDToStr(m_pszXNameID, &m_Curve.pszXName);
	if (m_pszYNameID != NULL)
		StrIDToStr(m_pszYNameID, &m_Curve.pszYName);
	
	for (int i = 0 ; i < CURVE_LINE_MAX; i++)
		StrIDToStr(m_pszLineNameID[i], &m_pLine[i].pszName);
		
	if (m_pDoubleBuffer != NULL)
		m_pDoubleBuffer->InitBuffer(m_bgc);
	else 
		Clear();
		
	m_rectCoordinate.left	= 0;
	m_rectCoordinate.right	= GetClientWidth() - 1;
	m_rectCoordinate.top	= 0;
	m_rectCoordinate.bottom	= GetClientHeight() -1;
	
}

int		CtmCurveEx::GetRangeXY()
{
	int i;
	CURVEPOINT	*pPoint;
	double dXMin = 0;
	double dXMax = 0;
	int    nIndex = 0;
	
	dXMin = fmin(m_dXMin, m_dXMax);
	dXMax = fmax(m_dXMin, m_dXMax);
	
	for(nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		//if(m_pLine[nIndex] == NULL)	continue;
		if(m_pLine[nIndex].pValue == NULL) continue;
		pPoint	= m_pLine[nIndex].pValue;
		
		for(i = 0; i < m_pLine[nIndex].nCount; i ++)
		{
			if(dXMax < pPoint->dX)				
					dXMax	= pPoint->dX;
			if(dXMin > pPoint->dX)		
					dXMin	= pPoint->dX;
					
			if ((m_dYMax1 < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
				m_dYMax1 = pPoint->dY;
			else if ((m_dYMax2 < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
				m_dYMax2 = pPoint->dY;
			
			if ((m_dYMin1 > pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
				m_dYMin1 = pPoint->dY;
			else if ((m_dYMin2 > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
				m_dYMin2 = pPoint->dY;
			
			pPoint ++;
		}
	}

	//printf("m_nXRange=%d\n", m_nXRange);
	if (m_nXRange > 0 && m_nXRange <= CURVE_LINE_MAX && m_pLine[m_nXRange - 1].pValue != NULL)
	{
		nIndex = m_nXRange - 1;
		pPoint	= m_pLine[nIndex].pValue;
		
		if (m_pLine[nIndex].nCount > 0)
		{
			dXMin = dXMax = pPoint->dX;
		}
		
		for(i = 0; i < m_pLine[nIndex].nCount; i ++)
		{
			if(dXMax < pPoint->dX)				
					dXMax	= pPoint->dX;
			if(dXMin > pPoint->dX)		
					dXMin	= pPoint->dX;
					
			pPoint ++;
		}
		//printf("dXMin=%f, dXMax=%f\n", dXMin,dXMax);
	}
	
	for(int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		for (i = 0; i < CURVE_BUFFERLINE_MAX; i++)
		{
			if (m_BufferLine[nIndex]->pValue[i] == NULL) continue;	
			pPoint = m_BufferLine[nIndex]->pValue[i];
			for (int j = 0; j < m_BufferLine[nIndex]->nCount[i]; j++)
			{
				if(dXMax < pPoint->dX)				
					dXMax	= pPoint->dX;
				if(dXMin > pPoint->dX)		
					dXMin	= pPoint->dX;
				
				
				if		((m_dYMax1 < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
					m_dYMax1 = pPoint->dY;
				else if ((m_dYMax2 < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
					m_dYMax2 = pPoint->dY;
				
				if		((m_dYMin1 > pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
					m_dYMin1 = pPoint->dY;
				else if ((m_dYMin2 > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
					m_dYMin2 = pPoint->dY;
					
				pPoint ++;
			}
			
		}
	}
	
	if (1 == m_nReferenceFrame)
	{
		m_dYMin = m_dYMin2;
		m_dYMax = m_dYMax2;
	}
	else
	{
		m_dYMin = m_dYMin1;
		m_dYMax = m_dYMax1;	
	}
	
	if (m_Curve.nFlag & CURVE_XCONTRARY)
	{
		m_dXMax = dXMin;
		m_dXMin = dXMax;
	}
	else
	{
		m_dXMax = dXMax;
		m_dXMin = dXMin;
	}
	
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
	m_dXUnit = (m_dXMax - m_dXMin) / m_Curve.nCol;
	
	return 0;
}
/*
int		CtmCurveEx::GetRangeXY()
{
	int i;
	CURVEPOINT	*pPoint;
	
	//m_dYMin = m_dYMin1;
	//m_dYMax = m_dYMax1;
	//g_tmDebugInfo->PrintDebugInfo("GetRangeXY() Start = %f\n", m_dYMax);
	
	for (int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		if(m_pLine[nIndex] == NULL)	continue;
		if(m_pLine[nIndex].pValue == NULL) continue;
			
		
		pPoint	= m_pLine[nIndex].pValue;
		
		if (m_dXMax < pPoint->dX)
			m_dXMax	= pPoint->dX; 
		if (m_dXMin > pPoint->dX)
			m_dXMin	= pPoint->dX; 
		
		if		((m_dYMax1 < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
			m_dYMax1 = pPoint->dY;
		else if ((m_dYMax2 < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
			m_dYMax2 = pPoint->dY;
		
		if		((m_dYMin1 > pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
			m_dYMin1 = pPoint->dY;
		else if ((m_dYMin2 > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
			m_dYMin2 = pPoint->dY;
		
		break;
	}
	
	//g_tmDebugInfo->PrintDebugInfo("GetRangeXY() Start1 Max2= %f, Max =%f\n", m_dYMax2, m_dYMax);
	for(int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		if(m_pLine[nIndex] == NULL)	continue;
		if(m_pLine[nIndex].pValue == NULL) continue;
		pPoint	= m_pLine[nIndex].pValue;
		for(i = 0; i < m_pLine[nIndex].nCount; i ++)
		{
			if(m_dXMax < pPoint->dX)				
					m_dXMax	= pPoint->dX;
			if(m_dXMin > pPoint->dX)		
					m_dXMin	= pPoint->dX;
					
		
			if		((m_dYMax1 < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
				m_dYMax1 = pPoint->dY;
			else if ((m_dYMax2 < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
				m_dYMax2 = pPoint->dY;
			
			if		((m_dYMin1 > pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
				m_dYMin1 = pPoint->dY;
			else if ((m_dYMin2 > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
				m_dYMin2 = pPoint->dY;
			
			pPoint ++;
			
			
		}
	}
	
	//g_tmDebugInfo->PrintDebugInfo("GetRangeXY() Start2 = %f\n", m_dYMax2);
	for(int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
	{
		for (i = 0; i < CURVE_BUFFERLINE_MAX; i++)
		{
			if (m_BufferLine[nIndex]->pValue[i] == NULL) continue;	
			pPoint = m_BufferLine[nIndex]->pValue[i];
			for (int j = 0; j < m_BufferLine[nIndex]->nCount[i]; j++)
			{
				if(m_dXMax < pPoint->dX)				
					m_dXMax	= pPoint->dX;
				if(m_dXMin > pPoint->dX)		
					m_dXMin	= pPoint->dX;
				
				
				if		((m_dYMax1 < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
					m_dYMax1 = pPoint->dY;
				else if ((m_dYMax2 < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
					m_dYMax2 = pPoint->dY;
				
				if		((m_dYMin1 > pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
					m_dYMin1 = pPoint->dY;
				else if ((m_dYMin2 > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
					m_dYMin2 = pPoint->dY;
					
				pPoint ++;
				
			}
			
		}
	}
	
	if (1 == m_nReferenceFrame)
	{
		m_dYMin = m_dYMin2;
		m_dYMax = m_dYMax2;
		//printf("m_dYMin:%f, m_dYMax:%f\n", m_dYMin, m_dYMax);
	}
	else
	{
		m_dYMin = m_dYMin1;
		m_dYMax = m_dYMax1;	
	}
	
	//g_tmDebugInfo->PrintDebugInfo("GetRangeXY() Start3 = %f\n", m_dYMax2);
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
	m_dXUnit = (m_dXMax - m_dXMin) / m_Curve.nCol;
	//printf("m_dYMin:%f, m_dYMax:%f\n", m_dYMin, m_dYMax);

	

	//m_dYMin = m_dYMin1;
	//m_dYMax = m_dYMax1;
	//g_tmDebugInfo->PrintDebugInfo("GetRangeXY() end = %f\n", m_dYMax2);
	return 0;
}
*/

RECT	CtmCurveEx::GetTitleRect()
{
	RECT Result;
//	RECT TempRect; 
	memset(&Result, 0, sizeof(Result));
	
	if (m_nShowTitleType != CURVE_XXPOS_TOP && m_nShowTitleType != CURVE_XXPOS_BOTTOM)
	{
		m_bTitle = FALSE;
		//m_relationDown.pRect = NULL;
	}
	else if (CURVE_XXPOS_TOP == m_nShowTitleType)
	{
		
		m_bTitle = TRUE;
		AddTopRelation(&m_relationTitle, m_pcFont->m_wHeight);
		Result = *m_relationTitle.pRect;
	}
	else if (CURVE_XXPOS_BOTTOM == m_nShowTitleType)
	{
		m_bTitle = TRUE;
		AddDownRelation(&m_relationTitle, m_pcFont->m_wHeight);
		Result = *m_relationTitle.pRect;
	}
	return Result;
}

RECT	CtmCurveEx::GetCutRect()
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	m_bCutLine = FALSE;
	int		i;
	int		nLineCount = 0;
	int		nCutLineWidth, nCutLineHeight;
	int		nCutLineLen, nCutLineStrMaxLen, nCutLineStrTotalLen;
	int		nStrLen;
	int		nStartLen, nMiddleLen, nEndLen;
	int		nVCutLine,		nHCutLine;	
	//RECT	rectCutLine;
	//int		nLeft, nTop;
	
	nCutLineLen	= 80;
	nStartLen	=
	nMiddleLen	=
	nEndLen		= 5;
	
	nCutLineStrMaxLen	= 
	nCutLineStrTotalLen	= 0;
	
	if((m_Curve.nFlag & CURVE_CUTLINE) == 0) 
	{
		return Result;	
	}
	
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//if(m_pLine[i] == NULL)	continue;	fans mark
		
		//	fans add
		if (m_pLine[i].pValue  == NULL) continue;
		
		
		nLineCount ++;
		
		if(m_pLine[i].pszName == NULL) continue;
		
		nStrLen	= strlen(m_pLine[i].pszName);
		nCutLineStrMaxLen	= (nStrLen > nCutLineStrMaxLen) ? 
								nStrLen :  nCutLineStrMaxLen;
		nCutLineStrTotalLen	   += nStrLen;		
	}
	
	
	
	if(m_Curve.nFlag & (CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT))
	{
		nCutLineWidth	= nCutLineStrMaxLen + nCutLineLen 
							+ nStartLen + nMiddleLen + nEndLen;
		nCutLineHeight	= (m_pcFont->m_wHeight + 2) * nLineCount;
		
		nVCutLine	= 1;
		nHCutLine	= 0;
		m_bCutLine = TRUE;
	}
	else if(m_Curve.nFlag & (CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM))
	{
		nCutLineWidth	= nCutLineStrTotalLen +	(nStartLen + nMiddleLen 
							+ nEndLen + nCutLineLen) * nLineCount;
		nCutLineHeight	= m_pcFont->m_wHeight + 2;
		nVCutLine	= 0;
		nHCutLine	= 1;
		m_bCutLine = TRUE;
	}
	
	//======
	// �p��ϨҦ�m
	//======
	switch(m_Curve.nFlag & CURVE_CUTLINE)
	{
		case CURVE_CUTLINE_LEFT:
			AddLeftRelation(&m_relationCutLine, nCutLineWidth);
			Result = *m_relationCutLine.pRect;
			break;
		case CURVE_CUTLINE_RIGHT:
			AddRightRelation(&m_relationCutLine, nCutLineWidth);
			Result = *m_relationCutLine.pRect;
			break;
		case CURVE_CUTLINE_TOP:
			AddTopRelation(&m_relationCutLine, nCutLineHeight);
			m_relationCutLine.pRect->bottom += 1;
			Result = *m_relationCutLine.pRect;
			break;
		case CURVE_CUTLINE_BOTTOM:
			AddDownRelation(&m_relationCutLine, nCutLineHeight);
			Result = *m_relationCutLine.pRect;
			break;
		default	:
			break;
	}
	return Result;
}

RECT	CtmCurveEx::GetXNameRect()
{
	RECT Result;
	if (m_bXName)
	{
		switch (m_nShowXAxesType)
		{
			case CURVE_XXPOS_TOP:
				AddTopRelation(&m_relationXName, m_pcFont->m_wHeight);
				Result = *m_relationXName.pRect;
				break;
			case CURVE_XXPOS_BOTTOM:
				AddDownRelation(&m_relationXName, m_pcFont->m_wHeight);
				Result = *m_relationXName.pRect;
				break;
			default:
				m_bXName = FALSE;
				break;
		}
	}	
	return Result;
}

RECT	CtmCurveEx::GetYNameRect()
{
	RECT Result;
	if (m_bYName)
	{
		switch (m_nShowYAxesType)
		{
			case CURVE_XXPOS_LEFT:
				AddLeftRelation(&m_relationYName, m_pcFont->m_wWidth);
				Result = *m_relationYName.pRect;
				break;
			case CURVE_XXPOS_RIGHT:
				AddRightRelation(&m_relationYName, m_pcFont->m_wWidth);
				Result = *m_relationYName.pRect;
				break;
			default:
				m_bYName = FALSE;
				break;
		}
	}
	return Result;
}

RECT	CtmCurveEx::GetXAxesScaleRect()
{
	RECT Result;
	if (CURVE_VALUE_NOTHING != m_nShowXScaleType) 
	{
		switch (m_nShowXAxesType)
		{
			case CURVE_XXPOS_TOP:
				AddTopRelation(&m_relationXAxesScale, m_pcFont->m_wHeight);
				Result = *m_relationXAxesScale.pRect;
				break;
			case CURVE_XXPOS_BOTTOM:
				AddDownRelation(&m_relationXAxesScale, m_pcFont->m_wHeight);
				Result = *m_relationXAxesScale.pRect;
				break;
			default:
				break;
		}
	}
	return Result;
}

RECT	CtmCurveEx::GetYAxesScaleRect()
{
	RECT Result;
	
	if (CURVE_VALUE_NOTHING != m_nShowYScaleType) 
	{
		char sz[100], szYFormat[10];
		int	nWidth = 0;
		snprintf(szYFormat, 10, "%%.%df", m_Curve.nYDigits);
		for(int i = 0; i <= m_Curve.nRow; i ++)
		{
			snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);//m_dYUnit * nYUnit * i + m_dYMin);
			
			if ((int)(strlen(sz) * m_pcFont->m_wWidth) / 2 > nWidth)
				nWidth = strlen(sz) * m_pcFont->m_wWidth / 2;
		}
		
		//printf("width = %d\n", nWidth);
		
		switch (m_nShowYAxesType)
		{
			case CURVE_XXPOS_LEFT:
				AddLeftRelation(&m_relationYAxesScale, nWidth);
				Result = *m_relationYAxesScale.pRect;
				break;
			case CURVE_XXPOS_RIGHT:
				AddRightRelation(&m_relationYAxesScale, nWidth);
				Result = *m_relationYAxesScale.pRect;
				break;
			default:
				break;
		}
	}
	
	
	return Result;
}

RECT	CtmCurveEx::GetLineRect()
{
	RECT Result;
	int nXUnit, nYUnit;
	Result.left = GetLastLeft();
	Result.right = GetLastRight();
	Result.top = GetLastTop();
	Result.bottom = GetLastBottom();
	
	nXUnit	= (Result.right - Result.left) / m_Curve.nCol;
	nYUnit	= (Result.bottom - Result.top) / m_Curve.nRow;
	
	if (m_nShowYAxesType)
	{
		Result.right = Result.left + nXUnit * m_Curve.nCol;
		Result.bottom = Result.top + nYUnit * m_Curve.nRow;
	}
	m_rectLine = Result;
	
	return Result;
}

void	CtmCurveEx::SeperateArea()
{
	GetTitleRect();				//	Title���d�� 
	GetCutRect();				//	��o�ϨҪ��d��	
	GetXNameRect();	 			//	X�b�W�٪��d��    
	GetYNameRect();				//	Y�b�W�٪��d��   
	GetXAxesScaleRect();		//	���X�b���y�ЭȪ��d��   
	GetYAxesScaleRect();		//	���Y�b���y�ЭȪ��d��   
	GetLineRect();				//	����u�����e�Ͻd��	
	//printf("CurveName=%s\n", Name);
	//printf("Title Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectTitle.left, m_rectTitle.right, m_rectTitle.top, m_rectTitle.bottom);
	//printf("Cut Rect: left=%d, right=%d, top=%d, bottom=%d\n", 
	//		m_rectCutLine.left, m_rectCutLine.right, m_rectCutLine.top, m_rectCutLine.bottom);		
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

short	CtmCurveEx::ShowExtentEx(short nX, short nY, char* pszString, int nType)
{
	if (m_pDoubleBuffer != NULL)
		return ShowExtentD(nX, nY, pszString, nType);
	else return ShowExtent(nX, nY, pszString);
}

short   CtmCurveEx::ShowExtentD(short nX, short nY, char* pszString, int nType)
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

void	CtmCurveEx::ShowTitle()
{
	if(m_Curve.pszName != NULL)
	{
		int	nPos = 0;
		nPos = m_rectTitle.left + 
				(m_rectTitle.right - m_rectTitle.left - strlen(m_Curve.pszName) * m_pcFont->m_wWidth / 2) / 2;
		ShowExtentEx(nPos, m_rectTitle.top ,m_Curve.pszName, m_pcFont->GetFontCode());					
	}	
}

int		CtmCurveEx::DrawCutLine()
{
	int		i;
	int		nLineCount = 0;
	int		nCutLineWidth, nCutLineHeight;
	int		nCutLineLen, nCutLineStrMaxLen, nCutLineStrTotalLen;
	int		nStrLen;
	int		nStartLen, nMiddleLen, nEndLen;
	int		nVCutLine,		nHCutLine;
	RECT	rectCutLine;
	int		nLeft, nTop;
	
	//========
	// �p��ϨҪ��j�p
	//========
	nCutLineStrMaxLen	= 
	nCutLineStrTotalLen	= 0;
	
	if((m_Curve.nFlag & CURVE_CUTLINE) == 0) 
	{
		return -1;	
	}
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//	fans add James modify 2008/11/19 9:53AM
		if (m_pLine[i].pValue  == NULL &&
            m_BufferLine[i]->nLineCount <= 0) continue;
		
		nLineCount ++;

		if(m_pLine[i].pszName == NULL) continue;
		
		//Rw.Ying	2013/3/14 05:57�U��
		nStrLen = GetStringExtent(m_pLine[i].pszName).w;
		nCutLineStrMaxLen	= (nStrLen > nCutLineStrMaxLen) ? 
								nStrLen :  nCutLineStrMaxLen;
		nCutLineStrTotalLen	   += nStrLen;	
	}
	if(nLineCount == 0)	return 0;
	
	nCutLineLen	= 15;
	nStartLen	=
	nMiddleLen	=
	nEndLen		= 5;
	
	if(m_Curve.nFlag & (CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT))
	{
		nCutLineWidth	= nCutLineStrMaxLen + nCutLineLen 
							+ nStartLen + nMiddleLen + nEndLen;
		nCutLineHeight	= (m_pcFont->m_wHeight + 2) * nLineCount;
		
		nVCutLine	= 1;
		nHCutLine	= 0;
	}
	else if(m_Curve.nFlag & (CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM))
	{
		nCutLineWidth	= nCutLineStrTotalLen +	(nStartLen + nMiddleLen 
							+ nEndLen + nCutLineLen) * nLineCount;
		nCutLineHeight	= m_pcFont->m_wHeight + 2;
		nVCutLine	= 0;
		nHCutLine	= 1;
	}
	
	//======
	// �p��ϨҦ�m
	//======
	switch(m_Curve.nFlag & CURVE_CUTLINE)
	{
		case CURVE_CUTLINE_LEFT:
			rectCutLine.left	= m_rectCoordinate.left + 1;
			rectCutLine.right	= rectCutLine.left + nCutLineWidth;
			rectCutLine.top	= (m_rectCoordinate.bottom 
						- m_rectCoordinate.top - nCutLineHeight) / 2;
			rectCutLine.bottom= rectCutLine.top + nCutLineHeight;
			
			m_rectCoordinate.left	= rectCutLine.right + 1;
			break;
		case CURVE_CUTLINE_RIGHT:
			rectCutLine.right	= m_rectCoordinate.right - 1;
			rectCutLine.left	= rectCutLine.right - nCutLineWidth;	
			rectCutLine.top	= (m_rectCoordinate.bottom 
						- m_rectCoordinate.top - nCutLineHeight) / 2;
			rectCutLine.bottom= rectCutLine.top + nCutLineHeight + 2;
			
			m_rectLine.right	= rectCutLine.left - 5;
			break;
		case CURVE_CUTLINE_TOP:
			rectCutLine.left	= (m_rectCoordinate.right 
						- m_rectCoordinate.left - nCutLineWidth) / 2;
			rectCutLine.right	= rectCutLine.left + nCutLineWidth;
			rectCutLine.top	= m_rectCoordinate.top + 2;
			rectCutLine.bottom= rectCutLine.top + nCutLineHeight;
			
			m_rectCoordinate.top= rectCutLine.bottom + 1;			
			break;
		case CURVE_CUTLINE_BOTTOM:
			rectCutLine.left	= (m_rectCoordinate.right 
						- m_rectCoordinate.left - nCutLineWidth) / 2;
			rectCutLine.right	= rectCutLine.left + nCutLineWidth;
			rectCutLine.bottom= m_rectCoordinate.bottom;
			rectCutLine.top	= rectCutLine.bottom - nCutLineHeight;
			
			m_rectCoordinate.bottom	= rectCutLine.top - 1;
			break;
		default	:
			return 0;
	}
	
	//=======
	// �e�Ϩ�
	//=======
	m_pDoubleBuffer->SetColorInBuffer(BLACK);
	/*
	printf("cutline(%d, %d, %d, %d)\n", 
			rectCutLine.left, rectCutLine.top, rectCutLine.right, rectCutLine.bottom);
	printf("coordinate(%d, %d, %d, %d)\n", 
			m_rectCoordinate.left, m_rectCoordinate.top, m_rectCoordinate.right, m_rectCoordinate.bottom);
	printf("rectline(%d, %d, %d, %d)\n",
			m_rectLine.left, m_rectLine.top, m_rectLine.right, m_rectLine.bottom);
	*/
	m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER, rectCutLine.left, 
											rectCutLine.top,
											rectCutLine.right,	
											rectCutLine.bottom);
	//_SetColor(BLACK);
	//_SetViewPort(m_rectClient.left, m_rectClient.top,
	//	m_rectClient.right, m_rectClient.bottom);
	//_Rectangle(_GBORDER, rectCutLine.left, rectCutLine.top,
	//	rectCutLine.right,	rectCutLine.bottom);
		
	nLeft	= rectCutLine.left;
	nTop	= rectCutLine.top;
	//_SetViewPort(m_rectClient.left, m_rectClient.top,
	//	m_rectClient.right, m_rectClient.bottom);	
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//if(m_pLine[i] == NULL)	continue;		fans mark
		
		//	fans add  James modify 2008/11/19 9:53AM
		if(m_pLine[i].pValue == NULL &&
            m_BufferLine[i]->nLineCount <= 0)	continue;		
		
		//_SetViewPort(m_rectClient.left, m_rectClient.top,
		//m_rectClient.right, m_rectClient.bottom);
			
		//_SetColor(m_pLine[i].dwColor);
		m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
		
		m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR, 
					nLeft + nStartLen, 
					nTop + m_pcFont->m_wHeight / 2,
					nLeft + nStartLen + nCutLineLen, 
					nTop + m_pcFont->m_wHeight / 2 + 2
				  );
		//_Rectangle(	_GFILLINTERIOR, 
		//			nLeft + nStartLen, 
		//			nTop + m_pcFont->m_wHeight / 2,
		//			nLeft + nStartLen + nCutLineLen, 
		//			nTop + m_pcFont->m_wHeight / 2 + 2
		//		  ); 

		if(m_pLine[i].pszName == NULL)
		{
			nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen) * nHCutLine;
			nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
			continue;
		}
		

		ShowExtentD(	nLeft + nStartLen + nMiddleLen + nCutLineLen,
					nTop  + 2, m_pLine[i].pszName, m_pcFont->GetFontCode());
		
					
		nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen 
					+ GetStringExtent(m_pLine[i].pszName).w) * nHCutLine;
		nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
	}

	return 0;
}
/*
int		CtmCurveEx::DrawCutLine()
{
	int		i;
	int		nLineCount = 0;
	int		nCutLineWidth, nCutLineHeight;
	int		nCutLineLen, nCutLineStrMaxLen, nCutLineStrTotalLen;
	int		nStrLen;
	int		nStartLen, nMiddleLen, nEndLen;
	int		nVCutLine,		nHCutLine;	
	int		nLeft, nTop;
	BOOL	bCutLine = FALSE;
	//========
	// �p��ϨҪ��j�p
	//========
	nCutLineStrMaxLen	= 
	nCutLineStrTotalLen	= 0;
	
	if((m_Curve.nFlag & CURVE_CUTLINE) == 0) 
	{
		return -1;	
	}
	
	for(i = 0; i < CURVE_LINE_MAX; i ++)
	{
		//if(m_pLine[i] == NULL)	continue;	fans mark
		
		//	fans add
		if (m_pLine[i].pValue  == NULL &&
            m_BufferLine[i]->nLineCount <= 0) continue;
		
		//if (m_pLine[i].pValue  == NULL) continue;
		nLineCount ++;
		
		if(m_pLine[i].pszName == NULL) continue;
		
		//nStrLen	= strlen(m_pLine[i].pszName);
		nStrLen = StrLenTM(m_pLine[i].pszName);
		nCutLineStrMaxLen	= (nStrLen > nCutLineStrMaxLen) ? 
								nStrLen :  nCutLineStrMaxLen;
		nCutLineStrTotalLen	   += nStrLen;		
	}

    


	if(nLineCount == 0)	return 0;
		
	nCutLineStrMaxLen	= nCutLineStrMaxLen * m_pcFont->m_wWidth / 2;
	nCutLineStrTotalLen	= nCutLineStrTotalLen * m_pcFont->m_wWidth / 2;	
	
	nCutLineLen	= 16;
	nStartLen	=
	nMiddleLen	=
	nEndLen		= 5;
	
	if(m_Curve.nFlag & (CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT))
	{
		nCutLineWidth	= nCutLineStrMaxLen + nCutLineLen 
							+ nStartLen + nMiddleLen + nEndLen;
		nCutLineHeight	= (m_pcFont->m_wHeight + 2) * nLineCount;
		
		nVCutLine	= 1;
		nHCutLine	= 0;
		bCutLine = TRUE;
	}
	else if(m_Curve.nFlag & (CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM))
	{
		nCutLineWidth	= nCutLineStrTotalLen +	(nStartLen + nMiddleLen 
							+ nEndLen + nCutLineLen) * nLineCount;
		nCutLineHeight	= m_pcFont->m_wHeight + 2;
		nVCutLine	= 0;
		nHCutLine	= 1;
		bCutLine = TRUE;
	}
	
	//=======
	// �e�Ϩ�
	//=======
	if (bCutLine)
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(BLACK);
			m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER, 
												m_rectCutLine.left, 
												m_rectCutLine.top,
												m_rectCutLine.right,	
												m_rectCutLine.bottom - 2);
			nLeft	= m_rectCutLine.left;
			nTop	= m_rectCutLine.top;
			for(i = 0; i < CURVE_LINE_MAX; i ++)
			{
				if(m_pLine[i].pValue == NULL&&
            	m_BufferLine[i]->nLineCount <= 0)	continue;		
				
				m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
				
				m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR, 
							nLeft + nStartLen, 
							nTop + m_pcFont->m_wHeight / 2,
							nLeft + nStartLen + nCutLineLen, 
							nTop + m_pcFont->m_wHeight / 2 + 2
						  );
		
				if(m_pLine[i].pszName == NULL)
				{
					nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen) * nHCutLine;
					nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
					continue;
				}
				
				//printf("pszName=%s\n", m_pLine[i].pszName);
				ShowExtentEx(nLeft + nStartLen + nMiddleLen + nCutLineLen,
							nTop  + 2, m_pLine[i].pszName, m_pcFont->GetFontCode());
				
							
				nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen 
							+ GetStringExtent(m_pLine[i].pszName).w) * nHCutLine;
				nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
			}
		}
		else
		{
			_SetColor(BLACK);
			_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
				m_rectCoordinate.right, m_rectCoordinate.bottom);
			_Rectangle(_GBORDER, m_rectCutLine.left, m_rectCutLine.top,
				m_rectCutLine.right,	m_rectCutLine.bottom);
			
			nLeft	= m_rectCutLine.left;
			nTop	= m_rectCutLine.top;
				_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
					m_rectCoordinate.right, m_rectCoordinate.bottom);	
			for(i = 0; i < CURVE_LINE_MAX; i ++)
			{
				
				if(m_pLine[i].pValue == NULL&&
            	m_BufferLine[i]->nLineCount <= 0)	continue;		
				
				_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
				m_rectCoordinate.right, m_rectCoordinate.bottom);
					
				_SetColor(m_pLine[i].dwColor);
				
				_Rectangle(	_GFILLINTERIOR, 
							nLeft + nStartLen, 
							nTop + m_pcFont->m_wHeight / 2,
							nLeft + nStartLen + nCutLineLen, 
							nTop + m_pcFont->m_wHeight / 2 + 2
						  ); 
		
				if(m_pLine[i].pszName == NULL)
				{
					nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen) * nHCutLine;
					nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
					continue;
				}
				
		
				ShowExtent(	nLeft + nStartLen + nMiddleLen + nCutLineLen,
							nTop  + 2, m_pLine[i].pszName);
				
							
				nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen 
							+ strlen(m_pLine[i].pszName) 
							* m_pcFont->m_wWidth / 2) * nHCutLine;
				nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
			}
		}
	}
	return 0;
}
*/
void	CtmCurveEx::DrawArrow()
{
	//	draw y arrow
	//if (m_pDoubleBuffer != NULL)
	//{
	//	m_pDoubleBuffer->SetColorInBuffer(m_nYAxesColor);
	//	if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
	//	{
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left, m_rectLine.top);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left, m_rectLine.top + CURVE_ARROW_LNE);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left - CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left, m_rectLine.top + CURVE_ARROW_LNE);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//	}
	//	else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
	//	{
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.top);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top + CURVE_ARROW_LNE);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right - CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.top + CURVE_ARROW_LNE);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right + CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//	}
	//	else
	//	{
	//		;
	//	}
	//	//	draw x arrow
	//	
	//	m_pDoubleBuffer->SetColorInBuffer(m_nXAxesColor);
	//	if (CURVE_XXPOS_BOTTOM == m_nShowXAxesType)
	//	{
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.bottom);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.bottom);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - CURVE_ARROW_INCLINE);
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.bottom);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom + CURVE_ARROW_INCLINE);
	//	}
	//	else if (CURVE_XXPOS_TOP == m_nShowXAxesType)
	//	{
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.top);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.top);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top - CURVE_ARROW_INCLINE);
	//		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.top);
	//		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top + CURVE_ARROW_INCLINE);
	//	}
	//	else
	//	{
	//		;
	//	}
	//}
	//else
	//{
	//	_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
	//		m_rectCoordinate.right, m_rectCoordinate.bottom);
	//	_SetColor(m_nYAxesColor);
	//	if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
	//	{
	//		_MoveTo(m_rectLine.left, m_rectLine.top);
	//		_LineTo(m_rectLine.left, m_rectLine.top + CURVE_ARROW_LNE);
	//		_LineTo(m_rectLine.left - CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//		_MoveTo(m_rectLine.left, m_rectLine.top + CURVE_ARROW_LNE);
	//		_LineTo(m_rectLine.left + CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//	}
	//	else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
	//	{
	//		_MoveTo(m_rectLine.right, m_rectLine.top);
	//		_LineTo(m_rectLine.right, m_rectLine.top + CURVE_ARROW_LNE);
	//		_LineTo(m_rectLine.right - CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//		_MoveTo(m_rectLine.right, m_rectLine.top + CURVE_ARROW_LNE);
	//		_LineTo(m_rectLine.right + CURVE_ARROW_INCLINE, m_rectLine.top + CURVE_ARROW_LNE);
	//	}
	//	else
	//	{
	//		;
	//	}
	//	//	draw x arrow
	//	
	//	_SetColor(m_nXAxesColor);
	//	if (CURVE_XXPOS_BOTTOM == m_nShowXAxesType)
	//	{
	//		_MoveTo(m_rectLine.right, m_rectLine.bottom);
	//		_LineTo(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.bottom);
	//		_LineTo(m_rectLine.right, m_rectLine.bottom - CURVE_ARROW_INCLINE);
	//		_MoveTo(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.bottom);
	//		_LineTo(m_rectLine.right, m_rectLine.bottom + CURVE_ARROW_INCLINE);
	//	}
	//	else if (CURVE_XXPOS_TOP == m_nShowXAxesType)
	//	{
	//		_MoveTo(m_rectLine.right, m_rectLine.top);
	//		_LineTo(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.top);
	//		_LineTo(m_rectLine.right, m_rectLine.top - CURVE_ARROW_INCLINE);
	//		_MoveTo(m_rectLine.right + CURVE_ARROW_LNE, m_rectLine.top);
	//		_LineTo(m_rectLine.right, m_rectLine.top + CURVE_ARROW_INCLINE);
	//	}
	//	else
	//	{
	//		;
	//	}
	//}
	// 

}

void	CtmCurveEx::DrawAxes()
{
	
	if (m_pDoubleBuffer != NULL)
	{
		//	draw y	
		m_pDoubleBuffer->SetColorInBuffer(m_nYAxesColor);
		
		//printf("%s, line rect left=%d, right=%d, top=%d, bottom=%d\n", 
		//		Name, m_rectLine.left, m_rectLine.right, m_rectLine.top, m_rectLine.bottom);
		switch (m_nShowYAxesType)
		{
			case CURVE_XXPOS_RIGHT:
				m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.bottom);
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top);
				break;
			case CURVE_XXPOS_LEFT:
				m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left , m_rectLine.bottom);
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left, m_rectLine.top);
				break;
			default:
				break;
		}
		//	draw x
		m_pDoubleBuffer->SetColorInBuffer(m_nXAxesColor);
		m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left, m_rectLine.bottom);         
		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom);        
	}
	else
	{
		_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
			m_rectCoordinate.right, m_rectCoordinate.bottom);	
		//	draw y	
		_SetColor(m_nYAxesColor);
		_MoveTo(m_rectLine.left, m_rectLine.bottom);
		_LineTo(m_rectLine.left, m_rectLine.top); 
		
		//	draw x
		_SetColor(m_nXAxesColor);
		_MoveTo(m_rectLine.left, m_rectLine.bottom);         
		_LineTo(m_rectLine.right, m_rectLine.bottom);
	}
	DrawArrow();
	
}

void	CtmCurveEx::DrawAxesGrid()
{
	int	nXUnit, nYUnit;
	int	i = 0;
	if (m_Curve.nCol <= 0) m_Curve.nCol = 1;
	if (m_Curve.nRow <= 0) m_Curve.nRow = 1;
	//=============
	//	attention: m_rectLine.right - m_rectLine.left is divisibility,
	//			   m_rectLine.bottom - m_rectLine.top too
	//=============
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	
	if (m_Curve.nFlag & CURVE_COOR_BROKENLINE)
	{
		if (m_pDoubleBuffer != NULL)
		{
			//m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
			//for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
			//{
			//	m_pDoubleBuffer->MoveToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top + i * 4);	
			//	m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top + i * 4 + 1);
			//}
			//m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.bottom);	
			
			
			
			//	Draw vertical line in the grid
			m_pDoubleBuffer->SetColorInBuffer(m_nYCoordinateColor);
			for(i = 1; i <= m_Curve.nCol; i ++)
			{
				for (int j = 0; j <= (m_rectLine.bottom - m_rectLine.top) / 4; j++)
				{
					m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + nXUnit * i,	m_rectLine.top + j * 4); 
					m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.top + j * 4 + 1);
				}
				
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.bottom);  
			}
			
			//	draw horizontal line in the grid
			m_pDoubleBuffer->SetColorInBuffer(m_nXCoordinateColor);
			for(i = 1; i <= m_Curve.nRow; i ++)
			{
				for (int j = 0; j < (m_rectLine.right - m_rectLine.left) / 4; j++)
				{
					m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + j * 4, m_rectLine.bottom - nYUnit * i - 1);   
					m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + j * 4 + 1, m_rectLine.bottom - nYUnit * i - 1); 
				}
				
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1); 
			}
		}
		else
		{
			_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
						m_rectCoordinate.right, m_rectCoordinate.bottom);
						
			//	 vertical line in the grid			
			_SetColor(m_nYCoordinateColor);
			for(i = 1; i <= m_Curve.nCol; i ++)
			{				
				for (int j = 0; j <= (m_rectLine.bottom - m_rectLine.top) / 4; j++)
				{
					_MoveTo(m_rectLine.left + nXUnit * i,	m_rectLine.top + j * 4); 
					_LineTo(m_rectLine.left + nXUnit * i, m_rectLine.top + j * 4 + 1);
				}
				
				_LineTo(m_rectLine.left + nXUnit * i, m_rectLine.bottom);  
			}
			
			//	draw horizontal line in the grid
			_SetColor(m_nXCoordinateColor);
			for(i = 1; i <= m_Curve.nRow; i ++)
			{
				for (int j = 0; j < (m_rectLine.right - m_rectLine.left) / 4; j++)
				{
					_MoveTo(m_rectLine.left + j * 4, m_rectLine.bottom - nYUnit * i - 1);   
					_LineTo(m_rectLine.left + j * 4 + 1, m_rectLine.bottom - nYUnit * i - 1); 
				}
				
				_LineTo(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1);   
			}
		}
	}
	else
	{
		if (m_pDoubleBuffer != NULL)
		{
			//	Draw vertical line in the grid
			m_pDoubleBuffer->SetColorInBuffer(m_nYCoordinateColor);
			for(i = 1; i <= m_Curve.nCol; i ++)
			{
				m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + nXUnit * i,	m_rectLine.bottom - 1); 
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.top - 1);  
			}
			
			//	draw horizontal line in the grid
			m_pDoubleBuffer->SetColorInBuffer(m_nXCoordinateColor);
			for(i = 1; i <= m_Curve.nRow; i ++)
			{
				m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i - 1);   
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1); 
			}
		}
		else
		{
			_SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
						m_rectCoordinate.right, m_rectCoordinate.bottom);
						
			//	 vertical line in the grid			
			_SetColor(m_nYCoordinateColor);
			for(i = 1; i <= m_Curve.nCol; i ++)
			{
				_MoveTo(m_rectLine.left + nXUnit * i,	m_rectLine.bottom - 1); 
				_LineTo(m_rectLine.left + nXUnit * i, m_rectLine.top);        
			}
			
			//	draw horizontal line in the grid
			_SetColor(m_nXCoordinateColor);
			for(i = 1; i <= m_Curve.nRow; i ++)
			{
				_MoveTo(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i);   
				_LineTo(m_rectLine.right, m_rectLine.bottom - nYUnit * i);      
			}
		}
	}
}
void	CtmCurveEx::DrawSclae()
{
	if (CURVE_VALUE_NOTHING != m_nShowXScaleType) 
	{
		DrawXScale();
	}
	
	if (CURVE_VALUE_NOTHING != m_nShowYScaleType) 
	{
		DrawYScale();
	}
}

void	CtmCurveEx::DrawXScale()
{
	int nStrXOffset = 0;
	int i = 0;
	int	nXUnit, nStrOffset;
	char	sz[100], szXFormat[10];	
	
	memset(sz, 0, sizeof(sz));
	snprintf(szXFormat, 10, "%%.%df", m_Curve.nXDigits);
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	
	if((m_Curve.nFlag & CURVE_XCONTRARY) && m_dXMin < m_dXMax)
	{
		double temp = m_dXMin;
		m_dXMin = m_dXMax;
		m_dXMax = temp;
		m_dXUnit	= -m_dXUnit;
		//printf("%s scale contrary, xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);
	}
	
	snprintf(sz, 100, szXFormat, m_dXMin);//m_dXStart);
	nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;

	if (CURVE_VALUE_MAXONLY != m_nShowXScaleType)
	{
		ShowExtentEx(m_rectLine.left, m_rectXAxesScale.top + 1, sz);
	}
	
	
	if (CURVE_VALUE_ALL == m_nShowXScaleType)
	{
		for(i = 1; i <= m_Curve.nCol; i ++)
		{
			memset(sz, 0, sizeof(sz));
			snprintf(sz, 100, szXFormat, m_dXUnit * i + m_dXMin);
			nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
			
			
			
			if (m_rectLine.left + nXUnit * i + nStrXOffset > m_rectLine.right)
			{
				if (i == m_Curve.nCol)
					nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 2;				
				else
					nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 4;	
			}
			if (i == m_Curve.nCol)
				ShowExtentEx(m_rectLine.right - strlen(sz) * m_pcFont->m_wWidth / 2,
							m_rectXAxesScale.top + 1, sz);		
			else			
				ShowExtentEx(m_rectLine.left + nXUnit * i - nStrXOffset,
							m_rectXAxesScale.top + 1, sz);		
			//printf("xScale[%d]=%s, x=%d\n", i, sz, m_rectLine.left + nXUnit * i - nStrOffset);
		}
	}
	else if (CURVE_VALUE_LIMIT == m_nShowXScaleType || CURVE_VALUE_MAXONLY == m_nShowXScaleType)
	{
		memset(sz, 0, sizeof(sz));
		snprintf(sz, 100, szXFormat, m_dXUnit * m_Curve.nCol + m_dXMin);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
		if (m_rectLine.left + nXUnit * m_Curve.nCol - nStrXOffset > m_rectLine.right)
		{
			nStrOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 2;
		}
		ShowExtentEx(m_rectLine.left + nXUnit * m_Curve.nCol - nStrXOffset,
						m_rectXAxesScale.top + 1, sz);		
	}
}


//	2008/4/2 11:06�W�� fans moidfy
void	CtmCurveEx::DrawYScale()
{
	
	double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	if (1 == m_nReferenceFrame)
	{
		m_dYMin = m_dYMin2;
      	m_dYMax = m_dYMax2;
      	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
	}
	else
	{
		m_dYMin = m_dYMin1;
      	m_dYMax = m_dYMax1;
      	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
	}
	
	//g_tmDebugInfo->PrintDebugInfo("YMIN =%f, YMIN1 =%f, YMAX= %f, yMAX1 = %f\n",
	//		m_dYMin, m_dYMin1, m_dYMax,m_dYMax1);
	//if (0 == m_nReferenceFrame)
	{
	    int nYUnit;
		int nStrXOffset, nStrYOffset;
		char	sz[100], szYFormat[10];	
		
		memset(sz, 0, sizeof(sz));
		snprintf(szYFormat, 10, "%%.%df", m_Curve.nYDigits);	
		nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
		//g_tmDebugInfo->PrintDebugInfo("m_Curve.nRow = %d, nYNit=%d\n", m_Curve.nRow, nYUnit);
		snprintf(sz, 100, szYFormat, m_dYMin);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
		nStrYOffset	= m_pcFont->m_wHeight;
		if (CURVE_VALUE_MAXONLY != m_nShowYScaleType && CURVE_VALUE_NOTHING != m_nShowYScaleType)
		{
			if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
				ShowExtentEx(m_rectYAxesScale.left,
					m_rectYAxesScale.bottom - nStrYOffset, sz);
			else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
				ShowExtentEx(m_rectYAxesScale.right - strlen(sz) * m_pcFont->m_wWidth / 2 - 1,
								m_rectYAxesScale.bottom  - nStrYOffset, sz);
		}
			
		if (CURVE_VALUE_ALL == m_nShowYScaleType)
		{
			for(int i = 1; i <= m_Curve.nRow; i ++)
			{
				memset(sz, 0, sizeof(sz));
				snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);
				nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
				nStrYOffset	= m_pcFont->m_wHeight / 2;
				if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
					ShowExtentEx(m_rectYAxesScale.left,// - nStrXOffset,
								m_rectYAxesScale.bottom - nYUnit * i - nStrYOffset, sz);
				else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
					ShowExtentEx(m_rectYAxesScale.right - nStrXOffset,
								m_rectYAxesScale.bottom - nYUnit * i - nStrYOffset, sz);
				
			}
		}
		else if (CURVE_VALUE_LIMIT == m_nShowYScaleType || CURVE_VALUE_MAXONLY == m_nShowYScaleType)
		{
			memset(sz, 0, sizeof(sz));
			snprintf(sz, 100, szYFormat, m_dYUnit * m_Curve.nRow + m_dYMin);
			nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
			nStrYOffset	= m_pcFont->m_wHeight / 2;
			if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
				ShowExtentEx(m_rectYAxesScale.left,// - nStrXOffset,
								m_rectYAxesScale.bottom - nYUnit * m_Curve.nRow  - nStrYOffset, sz);
				else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
					ShowExtentEx(m_rectYAxesScale.right - nStrXOffset,
								m_rectYAxesScale.bottom - nYUnit * m_Curve.nRow  - nStrYOffset, sz);
		}
	}
	
	m_dYMin = dYMin;
    m_dYMax = dYMax;
    m_dYUnit= dYUnit;
   
}

void	CtmCurveEx::DrawName()
{
	if (m_bXName) DrawXName();
	if (m_bYName) DrawYName();
}

void	CtmCurveEx::DrawXName()
{
	int nStrXOffset = 0;
	int nStrYOffset = 0;
	if(m_Curve.pszXName != NULL)
	{
		nStrXOffset	= ((m_rectXName.right - m_rectXName.left) 
						- strlen(m_Curve.pszXName) * m_pcFont->m_wWidth / 2)
						/ 2;
		
		nStrYOffset	= m_pcFont->m_wHeight + 1;
		ShowExtentEx(m_rectXName.left + nStrXOffset,m_rectXName.top,
					m_Curve.pszXName, m_pcFont->GetFontCode());
	}
	
	
}

void	CtmCurveEx::DrawYName()
{
	
	if(m_Curve.pszYName != NULL)
	{
		int nStrXOffset = 0;
		int nStrYOffset = 0;
		char	sz[100];	
		int nYStrLen  = 0;
		BYTE 	*pChar;
		WORD	*pCode;
		int		nStrLen		= 0;
		
		pChar	= (BYTE *)m_Curve.pszYName;
		while(*pChar != 0)
		{
			nStrLen	   += m_pcFont->m_wHeight;
			pChar +=2;
		}
		
		
		nStrYOffset	= ((m_rectYName.bottom - m_rectYName.top) 
						- nStrLen) / 2;
		pChar	= (BYTE *)m_Curve.pszYName;
		
		BOOL	bDoubleChar = FALSE;
		while(*pChar != 0)
		{		
			sz[2]	= 
			sz[1]	= 0;
			if(*pChar > 0xA0)
			{
				pCode	= (WORD *)pChar;				
				nStrXOffset	= m_pcFont->m_wWidth + nYStrLen;				
				sz[0]	= *pChar;
				sz[1]	= *(pChar + 1);
				pChar += 2;
			}
			else
			{
				nStrXOffset = m_pcFont->m_wWidth  + nYStrLen;
				sz[0]	= *pChar;		
				sz[1]	= *(pChar + 1);		
				pChar +=2;
			}
			
			if (CURVE_XXPOS_LEFT== m_nShowYAxesType)
				ShowExtentEx( m_rectYName.left, 
							m_rectYName.top + nStrYOffset, sz, m_pcFont->GetFontCode());
			else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
				//ShowExtentEx( m_rectYName.right - m_pcFont->m_wWidth / 2, 
			{
				if (bDoubleChar)
					ShowExtentEx( m_rectYName.right - m_pcFont->m_iCharLen, 
								m_rectYName.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
				else
					ShowExtentEx( m_rectYName.right - m_pcFont->m_iCharLen / 2, 
								m_rectYName.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
			}
			
			nStrYOffset += m_pcFont->m_wHeight;			
		}
	}
}

void	CtmCurveEx::DrawCoordinate()
{
	//=======
	// �e�I��
	//=======
	if (m_bShowGrid)
		DrawAxesGrid();
	//=======
	// �e���жb
	//=======	
	DrawAxes();
	//=======
	// ��ܧ��жb���
	//=======	
	DrawSclae();
	//=======
	// ��ܧ��жb�W��
	//=======
	DrawName();
}

void	CtmCurveEx::DrawCurveLine(LINE *pLine)
{
	int		i, nBottom;
	int 	nXUnit, nYUnit;
	
	int nXPos = 0;
	int nYPos = 0;	
	
	//	fans add 2008/4/2 11:16�W��
	double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	if (1 == pLine->nRefernce)
	{
		m_dYMin = m_dYMin2;
      	m_dYMax = m_dYMax2;	
	}
	else
	{
		m_dYMin = m_dYMin1;
      	m_dYMax = m_dYMax1;    	
	}
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;  
	
	
	nBottom	= m_rectLine.bottom - m_rectLine.top;
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	
	if (m_pDoubleBuffer != NULL)
	{
		m_pDoubleBuffer->SetColorInBuffer(pLine->dwColor);
		
		//printf("%s xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);
		nXPos = m_rectLine.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit);
		nYPos = nBottom - (int)((pLine->pValue[0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
		m_pDoubleBuffer->MoveToInBuffer(nXPos, nYPos);	
		//printf("start(%d, %d), count:%d\n", nXPos, nYPos, pLine->nCount);
		for(i = 0; i< pLine->nCount; i ++)
		{
			nXPos = m_rectLine.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit);
			nYPos = nBottom - (int)((pLine->pValue[i].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
			m_pDoubleBuffer->DrawLineToInBuffer(nXPos, nYPos);
		}
	}
	else
	{
		_SetColor(pLine->dwColor);
		
		
		nXPos = m_rectLine.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit);
		nYPos = nBottom - (int)((pLine->pValue[0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
		_MoveTo(nXPos, nYPos);	
		for(i = 0; i< pLine->nCount; i ++)
		{
			nXPos = m_rectLine.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit);
			nYPos = nBottom - (int)((pLine->pValue[i].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
			_LineTo(nXPos, nYPos);
		}	
	}
	
	m_dYMin = dYMin;
	m_dYMax = dYMax;
	m_dYUnit= dYUnit;
}


void	CtmCurveEx::DrawCurveLine(LINE *pLine, int nLineIndex)
{
	int		i, nHeight;
	int 	nXUnit, nYUnit;
	tmPoint point1, point2;
	tmPoint *points[2];
	int		nOpMode;

	//	fans add 2008/4/2 11:16�W��
	double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	
	//printf("1 %s m_dXMin:%f, m_dXMax:%f\n", Name, m_dXMin, m_dXMax);
	
	points[0] = &point1;
	points[1] = &point2;
	
	if (1 == pLine->nRefernce)
	{
		m_dYMin = m_dYMin2;
      	m_dYMax = m_dYMax2;	
	}
	else
	{
		m_dYMin = m_dYMin1;
      	m_dYMax = m_dYMax1;    	
	}
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;  
	
	nHeight	= m_rectLine.bottom - m_rectLine.top;
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	
	if (m_pDoubleBuffer != NULL)
	{
		m_pDoubleBuffer->SetColorInBuffer(pLine->dwColor);
		
		//printf("pLine->nCount=%d \n", pLine->nCount);
		for (i = 0; i < pLine->nCount - 1; i++)
		{
			nOpMode = GenerateEndPoints(nLineIndex, &(pLine->pValue[i]), &(pLine->pValue[i + 1]), points, 2);
			if (nOpMode & DRAW_CURVE_IGNORE_POINT)
				continue;
			
			if (nOpMode & DRAW_CURVE_LINK_POINTS)
			{
				//printf("move to:%d (%d, %d)\n", i, point1.ix, point1.iy);
				m_pDoubleBuffer->MoveToInBuffer(point1.ix, point1.iy);
				//printf("draw to: %d (%d, %d)\n", i + 1, point2.ix, point2.iy);
				m_pDoubleBuffer->DrawLineToInBuffer(point2.ix, point2.iy);
			}
		}
	}
	
	m_dYMin = dYMin;
	m_dYMax = dYMax;
	m_dYUnit= dYUnit;
	//printf("2 %s m_dXMin:%f, m_dXMax:%f\n", Name, m_dXMin, m_dXMax);
}


void	CtmCurveEx::DrawLines()
{

	for(int i = 0; i < CURVE_LINE_MAX; i ++)
	{
		if (m_pLine[i].pValue == NULL) continue;
		
		if (!m_bTouchSupport)
			DrawCurveLine(&m_pLine[i]);
		else
			DrawCurveLine(&m_pLine[i], i);
	}
	
	DrawBufferLine();
}

void	CtmCurveEx::Show()
{
	PropareShow();
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{
		if(!(m_Curve.nFlag & CURVE_NOTDRAW))
		{
			//=======
			// �p��X���жb���S��
			//=======
			if (m_nOpMode == CURVE_OPERATION_NONE)
				GetRangeXY();
				
			SeperateArea();	
			
			//	show title
			ShowTitle();
			
			// draw cutline		
			DrawCutLine();
			
			//========
			// draw coordinate
			//========
			DrawCoordinate();
			
			//=======
			// lines
			//=======
			DrawLines();
			
			
			//=======
			// lines
			//=======
			DrawRuleMark();
			//DrawRuleMark(m_nLineIndex);
			if(m_monitor_attr.flag)
				DrawMonitorArea();
			//	double buffer
			if (m_pDoubleBuffer != NULL)		
				m_pDoubleBuffer->DrawWithDoubleBuffer();
		}

		CtmWnd::Show();
		
	}	

}



void	CtmCurveEx::Update()
{
	if(CheckMonitorCoordinateChange()||m_bPropertyChanged)
	{
    	Show();
    	m_bPropertyChanged=FALSE;
    }
}


BOOL	CtmCurveEx::FindRelationFirst(tmRectRelation* pSource, tmRectRelation** pDest)
{
	BOOL bResult = FALSE;
	/*
	if (pSource->pRect != NULL)
	{
		while (pSource->pPre != NULL)
		{
			(*pDest) = *(pSource->pPre);
		}
	}
	*/
	return bResult;
	
}

BOOL	CtmCurveEx::FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest)
{
	BOOL bResult = FALSE;
	//tmRectRelation  Temp;
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

RECT	CtmCurveEx::FindLast(tmRectRelation* pSource)
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	tmRectRelation* TempRelation;
	
	if (FindRelationEnd(pSource, &TempRelation))
		Result = *TempRelation->pRect;
	/*
	else
		Result = m_rectCoordinate;
	*/
	
	return Result;
}

int		CtmCurveEx::GetLastLeft()
{
	int nResult = 0;
	RECT Temp;
	Temp = FindLast(&m_relationLeft);
	if (Temp.bottom > 0)
	{
		nResult = Temp.right;
		//printf("%s find left=%d\n", Name, nResult);
	}
	else
	{
		nResult = m_rectCoordinate.left;
	}
	
	return nResult;
}

int		CtmCurveEx::GetLastRight()
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
	//printf("GetLastRight %s, %d\n", Name, nResult);
	return nResult;
}


int		CtmCurveEx::GetLastTop()
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

int		CtmCurveEx::GetLastBottom()
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

BOOL	CtmCurveEx::AddTopRelation(tmRectRelation* pSource, int nHeight)
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
		
BOOL	CtmCurveEx::AddDownRelation(tmRectRelation* pSource, int nHeight)
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
		
BOOL	CtmCurveEx::AddLeftRelation(tmRectRelation* pSource, int nWidth)
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

BOOL	CtmCurveEx::AddRightRelation(tmRectRelation* pSource, int nWidth)
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

int		CtmCurveEx::DrawBufferLine() 
{
    int result = 0;
	int	nBottom;
	int nXUnit, nYUnit;
	int	nXPos, nYPos;
	
	//	2008/4/2 11:30�W��	fans add
	double	dYMin = m_dYMin1;
	double	dYMax = m_dYMax1;
	double	dYUnit= m_dYUnit;
	
	
	
	if (m_pDoubleBuffer != NULL)
	{
		for (int i = 0; i < CURVE_LINE_MAX; i++)
		{
			for (int j = 0; j < m_BufferLine[i]->nLineCount; j++)
			{
				if (m_BufferLine[i]->nCount[j] > 0)
				{
					//DrawCurveLine(m_BufferLine[i]->pValue);
					//	fans add 2008/4/2 11:30�W��
					if (1 == m_BufferLine[i]->nRefernce)
					{
						m_dYMin = m_dYMin2;
				      	m_dYMax = m_dYMax2;
				      	
					}
					else if (0 == m_BufferLine[i]->nRefernce)
					{
						m_dYMin = dYMin;
				        m_dYMax = dYMax;
					}
					m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
							
					nBottom	= m_rectLine.bottom - m_rectLine.top;
					nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
					nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
				
					nXPos = m_rectLine.left + (int)((m_BufferLine[i]->pValue[j][0].dX - m_dXMin) / m_dXUnit * nXUnit);
					nYPos = nBottom - (int)((m_BufferLine[i]->pValue[j][0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
					
					m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
					m_pDoubleBuffer->MoveToInBuffer(nXPos, nYPos);
							
					for (int k = 0; k < m_BufferLine[i]->nCount[j]; k++)
					{
						nXPos = m_rectLine.left + (int)((m_BufferLine[i]->pValue[j][k].dX - m_dXMin) / m_dXUnit * nXUnit);
						
						nYPos = nBottom - (int)((m_BufferLine[i]->pValue[j][k].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
						m_pDoubleBuffer->DrawLineToInBuffer(nXPos, nYPos);
					}
				}
			}
			
		}
	}
	
	m_dYMin = dYMin;
    m_dYMax = dYMax;
    m_dYUnit= dYUnit;
    	
	return result;
}


void	CtmCurveEx::SetShowFunc(pFuncShowRuleStr pFunc)
{
	m_pFuncYStr = pFunc;
}

int		CtmCurveEx::GetIndexBaseOnX(CURVEPOINT 	*pValue, int iCount, double dX)
{
	m_dfPercent	= 0;
	if (iCount <= 0) return -1;
	else
	{
		int nLow 	= 0;
		int nHigh 	= iCount - 1;
		int nMid 	= 0;
		nMid = (nLow + nHigh) / 2;
		BOOL	bPercent = FALSE;
		if (pValue[nLow].dX < pValue[nHigh].dX)
		{
		
			if (dX < pValue[nLow].dX)	return -1;
			if (dX > pValue[nHigh].dX) 	return -1;
			while(nLow < nHigh)
			{
				if 		(pValue[nMid].dX == dX)  break;
				else if (pValue[nMid].dX <	dX)	 
				{
					nLow = nMid + 1;
					if (pValue[nLow].dX > dX) 
					{
						bPercent = TRUE;
						break;
					}
				}
				else if (pValue[nMid].dX > dX)
				{
					nHigh 	= nMid - 1;
					if (pValue[nHigh].dX < dX) 
					{
						nMid = nHigh;
						bPercent = TRUE;
						break;
					}
				}
				nMid = (nLow + nHigh) / 2;
			}
		}
		else
		{
			if (dX > pValue[nLow].dX)	return -1;
			if (dX < pValue[nHigh].dX) 	return -1;
			while(nLow < nHigh)
			{
				if 		(pValue[nMid].dX == dX)  break;
				
				else if (pValue[nMid].dX <	dX)	 
				{
					
					nHigh 	= nMid - 1;
					if (pValue[nHigh].dX > dX) 
					{
						nMid = nHigh;
						bPercent = TRUE;
						break;
					}
				}
				else if (pValue[nMid].dX > dX)
				{
					nLow = nMid + 1;
					if (pValue[nLow].dX < dX) 
					{
						bPercent = TRUE;
						break;
					}
				}
				nMid = (nLow + nHigh) / 2;
			}
		}
		
		
		if (bPercent && 
			(nMid < iCount - 1) && 								//	can't overflow
				ABS(pValue[nMid + 1].dX - pValue[nMid].dX) >= 0.0001)
			{
				m_dfPercent = (dX - pValue[nMid].dX) / (ABS(pValue[nMid + 1].dX - pValue[nMid].dX));
			}

		g_tmDebugInfo->PrintDebugInfo(" Mid = %d\n", nMid);
		return nMid;
	}
}



void	CtmCurveEx::GetXPosValue()
{
	int	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	m_dXPosValue	= (double)m_xPos / (double)nXUnit * m_dXUnit  + m_dXMin;
	m_bPropertyChanged=TRUE;
	g_tmDebugInfo->PrintDebugInfo("GetXPosValue() = %f;XUnit=%d, dxUnit=%f, Min=%f\n", 
									m_dXPosValue, nXUnit, m_dXUnit, m_dXMin);
}

void    CtmCurveEx::OnGetFocus()
{
	m_bOnFocus = TRUE;	
	GetXPosValue();
	Update();
	
	if (m_pszPrompt != NULL)	
		Prompt(m_pszPrompt);
}

void    CtmCurveEx::OnLoseFocus()
{
	m_bOnFocus = FALSE;
	//Update();
}

WORD    CtmCurveEx::OnKey(WORD wKey)
{
	WORD	wResult = _NULL_KEY;
	BOOL	bUpdate = TRUE;
	
	switch (wKey)
	{
		//case _LARROW :
		//case _KEY_LEFTFIELD:
		//	FindNextPoint(m_nPointIndex, -m_nInterval);
		//	break;
		//case _RARROW :		// right
		//case _KEY_RIGHTFIELD:
		//	FindNextPoint(m_nPointIndex, m_nInterval);
		//	break;
		case _PLUS:
			StretchCurve(-0.1);
			break;
		case _MINUS:
			StretchCurve(0.1);
			break;
		case _DOT:
			ResetCoordinate();
			break;
		//case _KEY_RIGHTFIELD:
		case '3':
		//case 27136:
		
			m_xPos += m_xPerMove;
			if (m_xPos > m_rectLine.right - m_rectLine.left)
				m_xPos =0;// m_rectLine.right - m_rectLine.left;
			
			GetXPosValue();
			Update();
			wResult = _NULL_KEY;
    		break;
    	//case _KEY_LEFTFIELD:
    	case '1':
    	//case 26880:
    		m_xPos -= m_xPerMove;
    		if (m_xPos < 0)
				m_xPos = m_rectLine.right - m_rectLine.left;
			GetXPosValue();
			Update();
    		wResult = _NULL_KEY;
    		break;
		default:
			wResult = wKey;
			bUpdate = FALSE;
			break;
			
	}
	if (bUpdate)
		Update();
		
	return wResult;
	
}


void	CtmCurveEx::DrawRuleMark()
{
	if (m_bOnFocus)
	{
		DrawRuleLine();
		
		if (m_pFuncYStr != NULL)
			m_pFuncYStr(m_dXPosValue, m_szYRule, &m_dfPercent);
//		else			
			ComposeYRuleStrDef();
			
//		ShowYRuleStr();
	}
}


void	CtmCurveEx::DrawRuleMark(int nLineIndex)
{
	int ret 	= 0;
	int i 		= 0;
	tmPoint	point;
	
	if (!m_bOnFocus || nLineIndex >= CURVE_LINE_MAX || m_pLine[nLineIndex].nCount == 0)
		return;
		
	if (GetRuleType() & RULE_HORIZONTAL)		//�����Ф�
	{	
		if (GetPointLocation(&point) >= 0)
			m_nHRulePos = point.iy;
			
		
		if (!(GetRuleType() & RULE_NO_SHOW))
			DrawHRuleLine();
	}
	else if (GetRuleType() & RULE_VERTICAL)		//�����Ф�
	{
		if (GetPointLocation(&point) >= 0)
			m_nVRulePos = point.ix;
		
		if (!(GetRuleType() & RULE_NO_SHOW))
			DrawVRuleLine();
	}
	else
		;
	
}


void	CtmCurveEx::DrawHRuleLine()
{	
	tmPoint point;
	
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::DrawHRuleLine()\n");
	
	if (m_Curve.nFlag & CURVE_RULE_REALLINE)
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(m_XRuleColor);
			GetPointLocation(&point);
			
			m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR, 
							m_rectLine.left + 1, 
							m_rectLine.top + point.iy - 1,
							m_rectLine.left + point.ix - 1, 
							m_rectLine.top + point.iy + 1
						  );
		}
		else
		{
			_SetColor(m_XRuleColor);
			_MoveTo(m_rectLine.left, m_rectLine.top + m_nHRulePos);
			_LineTo(m_rectLine.right, m_rectLine.top + m_nHRulePos);
		}
	}
	else
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(m_XRuleColor);
			for (int i = 0; i <= (m_rectLine.right - m_rectLine.left) / 4; i++)
			{
				m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + i * 4, m_rectLine.top + m_nHRulePos);	
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + i * 4 + 2, m_rectLine.top + m_nHRulePos);
			}
			//m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_yPos + m_rectLine.top);	
		}
		else
		{
			_SetColor(m_XRuleColor);
			
			for (int i = 0; i <= (m_rectLine.right - m_rectLine.left) / 4; i++)
			{
				_MoveTo(m_rectLine.left + i * 4, m_rectLine.top + m_nHRulePos);	
				_LineTo(m_rectLine.left + i * 4 + 2, m_rectLine.top + m_nHRulePos);
			}
			//_LineTo(m_rectLine.right, m_nRuleYPos);
		}
	}
}


void	CtmCurveEx::DrawVRuleLine()
{
	tmPoint point;
	
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::DrawYRuleLine()\n");

	if (m_Curve.nFlag & CURVE_RULE_REALLINE)
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
			GetPointLocation(&point);
			
			m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR, 
							m_rectLine.left + point.ix - 1, 
							m_rectLine.top + point.iy,
							m_rectLine.left + point.ix + 1, 
							m_rectLine.bottom
						  );
		}
		else
		{
			_SetColor(m_YRuleColor);
			_MoveTo(m_nVRulePos + m_rectLine.left, m_rectLine.top);
			_LineTo(m_nVRulePos + m_rectLine.left, m_rectLine.bottom);
		}
	}
	else
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
			for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
			{
				m_pDoubleBuffer->MoveToInBuffer(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4);	
				m_pDoubleBuffer->DrawLineToInBuffer(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4 + 2);
			}
			m_pDoubleBuffer->DrawLineToInBuffer(m_nVRulePos + m_rectLine.left, m_rectLine.bottom);	
		}
		else
		{
			_SetColor(m_YRuleColor);
			
			for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
			{
				_MoveTo(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4);	
				_LineTo(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4 + 2);
			}
			_LineTo(m_nVRulePos + m_rectLine.left, m_rectLine.bottom);
		}
	}
}

int		CtmCurveEx::GetPointLocation(tmPoint *point, int nLineIndex, int nPointIndex)
{
	int nXUnit, nYUnit, nHeight;
	int ix, iy;

	if (nLineIndex < 0)
		nLineIndex = m_nLineIndex;
	if (nPointIndex < 0)
		nPointIndex = m_nPointIndex;
	
	if (nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX || m_pLine[nLineIndex].pValue == NULL ||
		 nPointIndex < 0 || nPointIndex >= m_pLine[nLineIndex].nCount || point == NULL)
		return -1;
	
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	ix		= (int)((m_pLine[nLineIndex].pValue[nPointIndex].dX - m_dXMin) / m_dXUnit * nXUnit);
	
	if (1 == m_pLine[nLineIndex].nRefernce)
	{
		m_dYMin = m_dYMin2;
      	m_dYMax = m_dYMax2;	
	}
	else
	{
		m_dYMin = m_dYMin1;
      	m_dYMax = m_dYMax1;    	
	}
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow; 
	
	nHeight	 = m_rectLine.bottom - m_rectLine.top;
	nYUnit	 = (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	iy		 = nHeight - (int)((m_pLine[nLineIndex].pValue[nPointIndex].dY - m_dYMin)/ m_dYUnit * nYUnit);
	
	point->ix = ix;
	point->iy = iy;
	
	return 0;
}

int		CtmCurveEx::FindNextPoint(int nBaseIndex, int nOffset)
{
	if (m_pLine[m_nLineIndex].nCount <= 0)
		return -1;
	
	//if (m_Curve.nFlag & CURVE_XCONTRARY)
	nOffset = -nOffset;
	
	m_nPointIndex = (m_pLine[m_nLineIndex].nCount + nBaseIndex + nOffset)  //nOffset can be negative
						% m_pLine[m_nLineIndex].nCount;
	if (m_nPointIndex < 0)
		m_nPointIndex += m_pLine[m_nLineIndex].nCount;
	
	return m_nPointIndex;
}


void	CtmCurveEx::DrawRuleLine()
{
	//	draw the y line
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::DrawRuleLine()\n");
	
	
	if (m_Curve.nFlag & CURVE_RULE_REALLINE)
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
			m_pDoubleBuffer->MoveToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top);	
			m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.bottom);
		}
		else
		{
			_SetColor(m_YRuleColor);
			_MoveTo(m_xPos + m_rectLine.left, m_rectLine.top);
			_LineTo(m_xPos + m_rectLine.left, m_rectLine.bottom);
		}
	}
	else
	{
		if (m_pDoubleBuffer != NULL)
		{
			m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
			for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
			{
				m_pDoubleBuffer->MoveToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top + i * 4);	
				m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top + i * 4 + 1);
			}
			m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.bottom);	
		}
		else
		{
			_SetColor(m_YRuleColor);
			
			for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
			{
				_MoveTo(m_xPos + m_rectLine.left, m_rectLine.top + i * 4);	
				_LineTo(m_xPos + m_rectLine.left, m_rectLine.top + i * 4 + 1);
			}
			_LineTo(m_xPos + m_rectLine.left, m_rectLine.bottom);
		}
	}
}

int		CtmCurveEx::ComposeYRuleStrDef()
{	
	int nIndex = 0;
	char	szFormat[64];
	char	szTempY[256];
	sprintf(szFormat, "%%.%df",  m_Curve.nXDigits);
	sprintf(szTempY, szFormat, m_dXPosValue);
	memset(m_szYRule, 0, sizeof(m_szYRule));
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::ComposeYRuleStrDef() %s \n", szTempY);
	sprintf(szFormat, "%%s,%%.%df",  m_Curve.nYDigits);
	
	
	int		nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	BOOL	bGetYPos = FALSE;
	double	dY		= 0;
	
	for (int i = 0; i < CURVE_LINE_MAX; i++)
	{
		nIndex = GetIndexBaseOnX(m_pLine[i].pValue, m_pLine[i].nCount, m_dXPosValue);
		if (nIndex >= 0)
		{
			double	dTempMin, dTempMax, dTempUnit = 0;
			if (1 == m_pLine[i].nRefernce)
			{
				dTempMin = m_dYMin2;
		      		dTempMax = m_dYMax2;
			}
			else
			{
				dTempMin = m_dYMin1;
		      		dTempMax = m_dYMax1;
			}
			dTempUnit = (dTempMax - dTempMin) / m_Curve.nRow;
			
			dY = m_pLine[i].pValue[nIndex].dY;
			if (nIndex < m_pLine[i].nCount - 1)
				dY += (m_pLine[i].pValue[nIndex + 1].dY - m_pLine[i].pValue[nIndex].dY) * m_dfPercent;
						
			
			sprintf(szTempY, szFormat, szTempY, dY);
			if (!bGetYPos)
			{
				m_yPos = (int)((dY- dTempMin)/ dTempUnit * nYUnit);			
				bGetYPos = TRUE;
			}
			
		}	
	}    
	      
	sprintf(m_szYRule, "(%s)", szTempY);
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::ComposeYRuleStrDef2() %s \n", m_szYRule);
	return 0;
}
		
int		CtmCurveEx::ComposeXRuleStrDef()
{
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::ComposeXRuleStrDef()\n");
	return 0;
}
		
int		CtmCurveEx::ShowYRuleStr()
{
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::ShowYRuleStr()\n");
	if (m_szYRule[0] != '\0')
	{
		char	szTemp[256];
		memcpy(szTemp, m_szYRule, sizeof(m_szYRule));
		_tm_chartoword((WORD*)m_szYRule, szTemp);
		int nXPos, nYPos = 0;
		if ((int)(m_xPos + m_rectLine.left + strlen(szTemp) * m_pcFont->m_wWidth / 2) > m_rectLine.right)
			nXPos = m_xPos + m_rectLine.left - strlen(szTemp) * m_pcFont->m_wWidth / 2;
		else
			nXPos = m_xPos + m_rectLine.left;
			
		//if (m_yPos < m_pcFont->m_wHeight)
		nYPos =  m_rectLine.bottom - m_yPos - m_pcFont->m_wHeight;
		if (nYPos >= m_rectLine.bottom)
			nYPos -= m_pcFont->m_wHeight;
			
		if (nYPos <= m_rectLine.top)
			nYPos += m_pcFont->m_wHeight;
		ShowExtentD(nXPos, nYPos,m_szYRule, m_pcFont->GetFontCode());
	}
	return 0;
}
		
int		CtmCurveEx::ShowXRuleStr()
{
	g_tmDebugInfo->PrintDebugInfo("CtmCurveEx::ShowXRuleStr()\n");
	return 0;
}




int		CtmCurveEx::GetIndex(char *pszDBID, int nOffset)
{
	WORD	wPointStartValueID, wPointIDIndex;
	
	if (pszDBID == NULL || nOffset < 0)
		return -1;
	if (g_pDatabase != NULL)
	{
		wPointStartValueID 	= g_pDatabase->GetDataID(pszDBID);
		wPointIDIndex 		= g_pDatabase->DataIDToIndex(wPointStartValueID + nOffset);
	}
	return wPointIDIndex;
}


BOOL	CtmCurveEx::CheckValid(int nLineIndex, int nPointIndex)
{
	if (nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX 
		|| nPointIndex < 0 || nPointIndex >= m_nMaxPoint)  //James modify MAX_POINT_NUM-->m_nMaxPoint 2015/1/19 16:23:58
		return FALSE;
	else
		return TRUE;
}

/*
 * @pPoint: �ھڧ��Эȭp��o�쪺�����I�A��^���եΪ̡C
 * @nLineIndex: ���u���s��
 * @dX: ���Ъ�X��
 * @dY: ���Ъ�Y��
 * @Return: �p������쪺�����I
 *
 * �ھڧ��Эȭp����������I.
 */
tmPoint*	CtmCurveEx::GetPointPos(tmPoint *pPoint, int nLineIndex, double dX, double dY)
{
	int nXUnit, nYUnit, nHeight;
	int ix, iy;
	
	if (pPoint == NULL || nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX)
		return NULL;
	
	if (1 == m_pLine[nLineIndex].nRefernce)
	{
		m_dYMin = m_dYMin2;
      	m_dYMax = m_dYMax2;	
	}
	else
	{
		m_dYMin = m_dYMin1;
      	m_dYMax = m_dYMax1;
	}
	
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow; 
	
	nHeight	 = m_rectLine.bottom - m_rectLine.top;
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	 = (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	
	ix		= m_rectLine.left + (int)((dX - m_dXMin) / m_dXUnit * nXUnit);
	iy		 = nHeight - (int)((dY - m_dYMin) / m_dYUnit * nYUnit) + m_rectLine.top;
	
	pPoint->ix = ix;
	pPoint->iy = iy;
	
	
	return pPoint;
}

/*
 * �ھڦ��u�W�I��nPointIndex��������I�����Э�
 *
 */
tmPoint*	CtmCurveEx::GetPointPos(tmPoint *pPoint, int nLineIndex, int nPointIndex)
{
	//int nXUnit, nYUnit, nHeight;
	//int ix, iy;
	
	if (pPoint == NULL || !CheckValid(nLineIndex, nPointIndex))
		return NULL;
	
	return GetPointPos(pPoint, nLineIndex, 
						m_pLine[nLineIndex].pValue[nPointIndex].dX,
						m_pLine[nLineIndex].pValue[nPointIndex].dY);
}

/*
 * @pValue: �ھڧ����I�p��o�쪺���Э�,��^���եΪ�
 * @pPoint: �����I
 * �ھڧ���
 *
 */
CURVEPOINT	*CtmCurveEx::GetPointValue(CURVEPOINT *pValue, tmPoint *pPoint)
{
	int nXUnit, nYUnit;
	
	if (pPoint == NULL || pValue == NULL)
		return NULL;
	
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	
	pValue->dX = (double)pPoint->ix / (double)nXUnit * m_dXUnit  + m_dXMin;
	pValue->dY = m_dYMax - (double)pPoint->iy / (double)nYUnit * m_dYUnit;
	
	return pValue;
}

/*
 * �N�����ܤƶq�ഫ�����ЭȪ��ܤƶq�C
 *
 */
void	CtmCurveEx::CalcPointValueDelta()
{
	int nXUnit, nYUnit;
	
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
	
	m_dXValueDelta = (double)m_nXPosDelta / (double)nXUnit * m_dXUnit;
	m_dYValueDelta = (double)m_nYPosDelta / (double)nYUnit * m_dYUnit;
}

int CtmCurveEx::GenerateEndPoints(int nLineIndex, CURVEPOINT *p1, CURVEPOINT *p2, 
													tmPoint *pPoints[], int pNum)
{
	double dSlope, dIntercept, dCross[4];
	int nOpMode, nCrossNum;
	BOOL	bIn1, bIn2;
	bIn1 = bIn2 = FALSE;
	double dXMin, dXMax, dYMin, dYMax;
	
	if (nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX || p1 == NULL || p2 == NULL || pNum < 2)
		return DRAW_CURVE_IGNORE_POINT;
	
	dXMin = fmin(m_dXMin, m_dXMax);
	dXMax = fmax(m_dXMin, m_dXMax);
	dYMin = fmin(m_dYMin, m_dYMax);
	dYMax = fmax(m_dYMin, m_dYMax);
	
	nCrossNum = 0;
	nOpMode = DRAW_CURVE_LINK_POINTS;
	
	if (p1->dX >= dXMin && p1->dX <= dXMax 
		&& p1->dY >= dYMin && p1->dY <= dYMax)
		bIn1 = TRUE;
		
	if (p2->dX >= dXMin && p2->dX <= dXMax 
		&& p2->dY >= dYMin && p2->dY <= dYMax)
		bIn2 = TRUE;

	if (bIn1 && bIn2)
	{
		//printf("p1(%f, %f), p2(%f, %f)\n", p1->dX, p1->dY, p2->dX, p2->dY);
		GetPointPos(pPoints[0], nLineIndex, p1->dX, p1->dY);
		GetPointPos(pPoints[1], nLineIndex, p2->dX, p2->dY);
		return nOpMode;
	}
	
	if (p1->dX == p2->dX)			//vertical line
	{
		if (p1->dX < dXMin || p1->dX > dXMax) // out of the coordinate
			return DRAW_CURVE_IGNORE_POINT;
			
		if (dYMin >= min(p1->dY, p2->dY) && dYMin <= max(p1->dY, p2->dY)) // one point below the coordinate
		{
			dCross[nCrossNum] = dYMin;
			GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, dCross[nCrossNum]);
	
			//printf("p1[%d, %d], cross:%f\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy, dCross[nCrossNum]);
			nCrossNum++;
			
		}
		if (dYMax >= min(p1->dY, p2->dY) && dYMax <= max(p1->dY, p2->dY)) //one point above the coordinate 
		{
			dCross[nCrossNum] = dYMax;
			GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, dCross[nCrossNum]);
			
			//printf("p2[%d, %d]\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy);
			nCrossNum++;
		}
		
		if (nCrossNum == 0) // both points are out of coordinate
			nOpMode = DRAW_CURVE_IGNORE_POINT;
		else if (nCrossNum < pNum)	//only one point in the coordinate
		{
			if (p1->dY > dYMax || p1->dY < dYMin)
				dCross[nCrossNum] = p2->dY;
			else
				dCross[nCrossNum] = p1->dY;
				
			GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, dCross[nCrossNum]);
		}
		else
		{
			;
		}
			
		return nOpMode;
	}
	else if (p1->dY == p2->dY)
	{
		if ( p1->dY < dYMin || p1->dY > dYMax)
			return DRAW_CURVE_IGNORE_POINT;
			
		if (dXMin >= min(p1->dX, p2->dX) && dXMin <= max(p1->dX, p2->dX))
		{
			dCross[nCrossNum] = dXMin;
			GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], p1->dY);
	
			//printf("p1[%d, %d], cross:%f\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy, dCross[nCrossNum]);
			nCrossNum++;
			
		}
		if (dXMax >= min(p1->dX, p2->dX) && dXMax <= max(p1->dX, p2->dX))
		{
			dCross[nCrossNum] = dXMax;
			GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], p1->dY);
			
			//printf("p2[%d, %d]\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy);
			nCrossNum++;
			
		}
		
		if (nCrossNum == 0)
			nOpMode = DRAW_CURVE_IGNORE_POINT;
		else if (nCrossNum < pNum)
		{
			if (p1->dX > dXMax || p1->dX < dXMin)
				dCross[nCrossNum] = p2->dX;
			else
				dCross[nCrossNum] = p1->dX;
				
			GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], p1->dY);
		}
		else
		{
			;
		}
			
		return nOpMode;
	}
	else
		dSlope 		= (p2->dY - p1->dY) / (p2->dX - p1->dX);		//slope
	
	dIntercept 	= p2->dY - dSlope * p2->dX;						//intercept
	
	dCross[nCrossNum] = dSlope * dXMin + dIntercept;			//check left boundary
	if (dCross[nCrossNum] >= dYMin && dCross[nCrossNum] <= dYMax 
		&& dCross[nCrossNum] >= min(p1->dY, p2->dY) && dCross[nCrossNum] <= max(p1->dY, p2->dY))
	{
		if (nCrossNum < pNum)
			GetPointPos(pPoints[nCrossNum], nLineIndex, dXMin, dCross[nCrossNum]);
			
		nCrossNum++;
	}
	
	dCross[nCrossNum] = dSlope * dXMax + dIntercept;			//check right boundary
	if (dCross[nCrossNum] >= dYMin && dCross[nCrossNum] <= dYMax
		&& dCross[nCrossNum] >= min(p1->dY, p2->dY) && dCross[nCrossNum] <= max(p1->dY, p2->dY))
	{
		if (nCrossNum < pNum)
			GetPointPos(pPoints[nCrossNum], nLineIndex, dXMax, dCross[nCrossNum]);
			
		nCrossNum++;
	}
	
	dCross[nCrossNum] = (dYMin - dIntercept) / dSlope;		//check bottom boundary
	if (dCross[nCrossNum] >= dXMin && dCross[nCrossNum] <= dXMax
		&& dCross[nCrossNum] >= min(p1->dX, p2->dX) && dCross[nCrossNum] <= max(p1->dX, p2->dX))
	{
		if (nCrossNum < pNum)
			GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], dYMin);
			
		nCrossNum++;
	}	
	
	dCross[nCrossNum] = (dYMax - dIntercept) / dSlope;		//check top boundary
	if (dCross[nCrossNum] >= dXMin && dCross[nCrossNum] <= dXMax
		&& dCross[nCrossNum] >= min(p1->dX, p2->dX) && dCross[nCrossNum] <= max(p1->dX, p2->dX))
	{
		if (nCrossNum < pNum)
			GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], dYMax);
			
		nCrossNum++;
	}
	//printf("nCrossNum=%d\n", nCrossNum);
	if (nCrossNum == 0)
		nOpMode = DRAW_CURVE_IGNORE_POINT;
		
	if (nCrossNum == 1)
	{
		if (bIn1)
			GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, p1->dY);
		else if (bIn2)
			GetPointPos(pPoints[nCrossNum], nLineIndex, p2->dX, p2->dY);
		else
			nOpMode = DRAW_CURVE_IGNORE_POINT;
	}
	
	return nOpMode;
}

void	CtmCurveEx::SetOpMode(int nMode)
{
	m_nOpMode = nMode;
}

int		CtmCurveEx::GetOpMode()
{
	return m_nOpMode;
}

void	CtmCurveEx::StretchCurve(double dRatio)	//�Y��y�Шt
{
	m_bPropertyChanged=TRUE;
	double dXDelta, dYDelta1, dYDelta2;
	double dXMin, dXMax, dYMin, dYMax;
	
	dXDelta =  fabs((m_dXMax - m_dXMin)) * dRatio;
	dYDelta1 = fabs((m_dYMax1 - m_dYMin1)) * dRatio;
	dYDelta2 = fabs((m_dYMax2 - m_dYMin2)) * dRatio;
	
	m_nOpMode = CURVE_OPERATION_STRETCH;
	
	dXMin = fmin(m_dXMin, m_dXMax);
	dXMax = fmax(m_dXMin, m_dXMax);
		
	dXMax += dXDelta;
	dXMin -= dXDelta;
	if ((dXMax - dXMin) <= 0.001F)
		return;
	//printf("dXMin:%f, dXMax:%f\n", dXMin, dXMax);
	if (m_Curve.nFlag & CURVE_XCONTRARY)
	{
		m_dXMax = dXMin;
		m_dXMin = dXMax;
	}
	else
	{
		m_dXMax = dXMax;
		m_dXMin = dXMin;
	}
	
	dYMin = m_dYMin1;
	dYMax = m_dYMax1;
	
	dYMax += dYDelta1;
	dYMin -= dYDelta1;
	
	if ((dYMax - dYMin) <= 0.001F)
		return;
	
	m_dYMin1 = dYMin;
	m_dYMax1 = dYMax;
	
	dYMin = m_dYMin2;
	dYMax = m_dYMax2;
	
	dYMax += dYDelta2;
	dYMin -= dYDelta2;
	
	if ((dYMax - dYMin) <= 0.001F)
		return;
	
	m_dYMin2 = dYMin;
	m_dYMax2 = dYMax;
}

void	CtmCurveEx::MoveCoordinate(double dXDelta, double dYDelta)	//���ʮy�Шt����
{
	m_nOpMode = CURVE_OPERATION_MOVE_COORDINATE;
		
	m_dXMin  += dXDelta;
	m_dXMax  += dXDelta;
	
	m_dYMin1 += dYDelta;
	m_dYMax1 += dYDelta;
	m_dYMin2 += dYDelta;
	m_dYMax2 += dYDelta;
}

void	CtmCurveEx::ResetCoordinate()	//���m�y�Шt��m
{
	m_bPropertyChanged=TRUE;
	m_nOpMode = CURVE_OPERATION_NONE;
	
	m_dXMin = m_dOldXMin;
	m_dXMax = m_dOldXMax;
	m_dYMin1 = m_dOldYMin;
	m_dYMax1 = m_dOldYMax;
	m_dYMin2 = m_dOldYMin2;
	m_dYMax2 = m_dOldYMax2;
	
}

void	CtmCurveEx::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	BOOL	bNeedUpdate = FALSE;
		
	if (!m_bTouchSupport)
		return;
		
	switch(message)
    {
       case MSG_MOUSELUP:
       	//	printf("Mouse up(%d, %d)\n", wParam >> 16, wParam & 0x0000FFFF);
    		MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSELDOWN:
        //	printf("Mouse Down(%d, %d)\n", wParam >> 16, wParam & 0x0000FFFF);
        	MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	//bNeedUpdate = TRUE;
        	break;
        case MSG_MOUSEMOVE:
        //	printf("Mouse Move(%d, %d)\n", wParam >> 16, wParam & 0x0000FFFF);
        	MouseMove(wParam >> 16, wParam & 0x0000FFFF, 0);
        	//bNeedUpdate = TRUE;
        	break;
        default:
        	break;
    }
}

BOOL	CtmCurveEx::MouseUp(short x_pos,short y_pos,int key_flag)
{
	RECT rect    = GetWindowRECT();
	if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
		|| y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
		return FALSE;
	m_bPropertyChanged=TRUE;	
	ConvertPos(x_pos, y_pos);
	MoveCoordinate(-m_dXValueDelta, -m_dYValueDelta);
	Update();
	return TRUE;
}
BOOL	CtmCurveEx::MouseDown(short x_pos, short y_pos, int key_flag)
{
	RECT rect    = GetWindowRECT();
	if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
		|| y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
		return FALSE;
	m_bPropertyChanged=TRUE;	
	ConvertPos(x_pos, y_pos);
	
	return TRUE;
}
BOOL	CtmCurveEx::MouseMove(short x_pos, short y_pos, int key_flag)
{
	RECT rect    = GetWindowRECT();
	if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
		|| y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
		return FALSE;
	//ConvertPos(x_pos, y_pos);
	//MoveCoordinate(-m_dXValueDelta, -m_dYValueDelta);
	return TRUE;
}

void	CtmCurveEx::ConvertPos(short x_pos, short y_pos)
{
    RECT    	rect;
    tmPoint 	pointPos;
 //   CURVEPOINT 	pointValue;
    
    rect    = GetWindowRECT();
	if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
		|| y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
	{
		m_bOut = TRUE;		// out of the coordinate
		m_dXValueDelta = m_dYValueDelta = 0;
		return;
	}
	
	pointPos.ix = x_pos - (rect.left + m_rectLine.left);
	pointPos.iy = y_pos - (rect.top + m_rectLine.top);
	
	if (!m_bOut)
	{
		m_nXPosDelta = pointPos.ix - m_CurPos.ix;
		m_nYPosDelta = m_CurPos.iy - pointPos.iy;
	}
	else
	{
		m_bOut = !m_bOut;
		m_nXPosDelta = 0;
		m_nYPosDelta = 0; 
	}
	//printf("delta pos:(%d, %d)\n", m_nXPosDelta, m_nYPosDelta);
	m_CurPos.ix = pointPos.ix;
	m_CurPos.iy = pointPos.iy;
	
	CalcPointValueDelta();
}





void	CtmCurveEx::DrawMonitorArea()
{
	int		i, nBottom;
    int 	nXUnit, nYUnit;

    int nXPosLeft = 0;
    int nYPosTop  = 0;

	int nXPosRight = 0;
	int nYPosBottom =0;
    double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	if (1 ==m_monitor_attr.reference)
	{
		m_dYMin = m_dYMin2;
      	m_dYMax = m_dYMax2;	
	}
	else
	{
		m_dYMin = m_dYMin1;
      	m_dYMax = m_dYMax1;    	
	}
	
	m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;  
	
	
	nBottom	= m_rectLine.bottom - m_rectLine.top;
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
    
    
  	if (m_pDoubleBuffer != NULL)
  	{
  	    m_pDoubleBuffer->SetColorInBuffer(m_monitor_attr.color);
  	
  	    //printf("%s xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);
  	    nXPosLeft = m_rectLine.left + (int)((m_monitor_attr.point1.dX - m_dXMin) / m_dXUnit * nXUnit);
  	    nYPosTop = nBottom - (int)((m_monitor_attr.point1.dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
  	    
  	    nXPosRight = m_rectLine.left + (int)((m_monitor_attr.point2.dX - m_dXMin) / m_dXUnit * nXUnit);
  	    nYPosBottom = nBottom - (int)((m_monitor_attr.point2.dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
  	    for(int i=0; i<m_monitor_attr.line_width; i++)
  	    {
  	    	m_pDoubleBuffer->MoveToInBuffer(nXPosLeft+i, nYPosTop+i);
  	    	m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER,nXPosLeft+i,nYPosTop+i,nXPosRight-i,nYPosBottom-i);
  		}
  	}
  	else
  	{
  	    _SetColor(m_monitor_attr.color);
  	
  	
  	    nXPosLeft = m_rectLine.left + (int)((m_monitor_attr.point1.dX - m_dXMin) / m_dXUnit * nXUnit);
  	    nYPosTop = nBottom - (int)((m_monitor_attr.point1.dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
  	    
  	    nXPosRight = m_rectLine.left + (int)((m_monitor_attr.point2.dX  - m_dXMin) / m_dXUnit * nXUnit);
  	    nYPosBottom = nBottom - (int)((m_monitor_attr.point2.dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
  	    for(int i=0; i<m_monitor_attr.line_width; i++)
  	    {
  	    	_MoveTo(nXPosLeft+i, nYPosTop+i);
  	    	_Rectangle(_GBORDER,nXPosLeft+i,nYPosTop+i,nXPosRight-i,nYPosBottom-i);
  		}
  	}
  	m_dYMin = dYMin;
  	m_dYMax = dYMax;
  	m_dYUnit= dYUnit;
}

void	CtmCurveEx::SetMonitorAttr(CURVEPOINT 	*pValue1, CURVEPOINT *pValue2, int nLineWidth,int nColor,int nReference, BOOL bUsed)
{
	
	if(0==pValue1||0==pValue2)	return;
	m_monitor_attr.point1.dX=pValue1->dX;
	m_monitor_attr.point1.dY=pValue1->dY;
	m_monitor_attr.point2.dX=pValue2->dX;
	m_monitor_attr.point2.dY=pValue2->dY;
	m_monitor_attr.color=	nColor;
	m_monitor_attr.line_width	= nLineWidth;
	m_monitor_attr.reference=nReference;
	m_monitor_attr.flag=bUsed;
	double dMax=fmax(m_dXMin, m_dXMax);
	if(m_monitor_attr.point1.dX>=dMax)	m_monitor_attr.point1.dX=dMax;
	if(m_monitor_attr.point2.dX>=dMax) m_monitor_attr.point2.dX=dMax;
	if(m_monitor_attr.reference==0)
	{
		if(m_monitor_attr.point1.dY>=m_dYMax1)	m_monitor_attr.point1.dY=m_dYMax1;
		if(m_monitor_attr.point2.dY>=m_dYMax1) m_monitor_attr.point2.dY=m_dYMax1;
	}
	else
	{
		if(m_monitor_attr.point1.dY>=m_dYMax2)	m_monitor_attr.point1.dY=m_dYMax2;
		if(m_monitor_attr.point2.dY>=m_dYMax2) m_monitor_attr.point2.dY=m_dYMax2;
	}
	for(int i=0; i<4; i++)
	{
		if(!m_pszDBID[i])	return;	
	}
	long llValue=0;
	llValue=m_monitor_attr.point1.dX * Power(10, m_Curve.nXDigits);
	SetViewDBValue(m_pszDBID[0], llValue);
	llValue= m_monitor_attr.point2.dX * Power(10, m_Curve.nXDigits);
	SetViewDBValue(m_pszDBID[2], llValue);
	llValue = m_monitor_attr.point1.dY * Power(10, m_Curve.nYDigits);
	SetViewDBValue(m_pszDBID[1], llValue);
	llValue = m_monitor_attr.point2.dY * Power(10, m_Curve.nYDigits);
	SetViewDBValue(m_pszDBID[3], llValue);
}

BOOL	CtmCurveEx::CheckMonitorCoordinateChange()
{
	bool bFlag=TRUE;
	for(int i=0; i<4; i++)
	{ 
		if(!m_pszDBID[i])
		{
			bFlag=FALSE;
			break;	
		}	
	}
	double left,top,right,bottom;
	bool   changed=false;
	if(bFlag)
	{
		left=((double)GetDBValue(m_pszDBID[0]).lValue)/Power(10, m_Curve.nXDigits);
		right=((double)GetDBValue(m_pszDBID[2]).lValue)/Power(10, m_Curve.nXDigits);		 
		top=((double)GetDBValue(m_pszDBID[1]).lValue)/Power(10, m_Curve.nYDigits);	
		bottom=((double)GetDBValue(m_pszDBID[3]).lValue)/Power(10, m_Curve.nYDigits);	
		
		
		double dMax=fmax(m_dXMin, m_dXMax);
		if(left>=dMax)	left=dMax;
		if(right>=dMax) right=dMax;
		if(m_monitor_attr.reference==0)
		{
			if(top>=m_dYMax1)		top=m_dYMax1;
			if(bottom>=m_dYMax1) 	bottom=m_dYMax1;
		}
		else
		{
			if(top>=m_dYMax2)		top=m_dYMax2;
			if(bottom>=m_dYMax2) 	bottom=m_dYMax2;
		}
		
//		printf("1:%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",m_monitor_attr.point1.dX,
//		m_monitor_attr.point1.dY,
//		m_monitor_attr.point2.dX,
//		m_monitor_attr.point2.dY,
//		left,top,right,bottom,m_dYMax1);
		
		if(m_monitor_attr.point1.dX !=left||m_monitor_attr.point1.dY!=top||
			m_monitor_attr.point2.dX!=right||m_monitor_attr.point2.dY!=bottom)
		{
			m_monitor_attr.point1.dX=left;
			m_monitor_attr.point1.dY=top;
			m_monitor_attr.point2.dX=right;
			m_monitor_attr.point2.dY=bottom;
			changed=true;	
		}
	}
//	printf("%lf,%lf,%lf,%lf\n",m_monitor_attr.point1.dX,
//	m_monitor_attr.point1.dY,
//	m_monitor_attr.point2.dX,
//	m_monitor_attr.point2.dY);
	return changed;	
}






/*==========================================================================+
|           Class implementation - CtmCurveEx2                                |
+==========================================================================*/

/**
 *
 */
void CtmCurveEx2::InitData()
{
    memset(&m_relationLeft	, 			0, sizeof(m_relationLeft	));
    memset(&m_relationRight  , 			0, sizeof(m_relationRight   ));
    memset(&m_relationUp    , 			0, sizeof(m_relationUp      ));
    memset(&m_relationDown , 			0, sizeof(m_relationDown    ));

    memset(&m_relationTitle      , 		0, sizeof(m_relationTitle     ));
    memset(&m_relationCutLine    , 		0, sizeof(m_relationCutLine   ));
    memset(&m_relationXName      , 		0, sizeof(m_relationXName     ));
    memset(&m_relationXAxesScale , 		0, sizeof(m_relationXAxesScale));
    memset(&m_relationYName	     , 		0, sizeof(m_relationYName	    ));
    memset(&m_relationYNameEx	 , 		0, sizeof(m_relationYNameEx));
    memset(&m_relationYAxesScale , 		0, sizeof(m_relationYAxesScale));
    memset(&m_relationYAxesScaleEx , 	0, sizeof(m_relationYAxesScaleEx));
    memset(&m_relationLine       , 		0, sizeof(m_relationLine      ));



    memset(&m_rectTitle      	, 		0, sizeof(m_rectTitle     ));
    memset(&m_rectCutLine    	, 		0, sizeof(m_rectCutLine   ));
    memset(&m_rectXName      	, 		0, sizeof(m_rectXName     ));
    memset(&m_rectXAxesScale	, 		0, sizeof(m_rectXAxesScale));
    memset(&m_rectYName	    	, 		0, sizeof(m_rectYName	    ));
    memset(&m_rectYNameEx		,		0, sizeof(m_rectYNameEx));
    memset(&m_rectYAxesScale	, 		0, sizeof(m_rectYAxesScale));
    memset(&m_rectYAxesScaleEx	, 		0, sizeof(m_rectYAxesScaleEx));
    memset(&m_rectLine       	, 		0, sizeof(m_rectLine      ));


    m_relationTitle.pRect 		= &m_rectTitle;
    m_relationCutLine.pRect 	= &m_rectCutLine;
    m_relationXName.pRect 		= &m_rectXName;
    m_relationXAxesScale.pRect 	= &m_rectXAxesScale;
    m_relationYName.pRect 		= &m_rectYName;
    m_relationYNameEx.pRect		= &m_rectYNameEx;
    m_relationYAxesScale.pRect 	= &m_rectYAxesScale;
    m_relationYAxesScaleEx.pRect= &m_rectYAxesScaleEx;
    m_relationLine.pRect 		= &m_rectLine;
}
CtmCurveEx2::CtmCurveEx2(): CtmWnd()
{
    strcpy(TYPEName, "CtmCurveEx2");

    m_pDoubleBuffer = NULL;
    InitData();

    SetBorderWidth(3);


    m_pszCurveNameID = NULL;
    m_pszXNameID = NULL;
    m_pszYNameID = NULL;
    m_pszYNameExID =NULL;
	m_bRuleMark	   = 0;
    m_Curve.nFlag = 0xFF;
    m_Curve.nXDigits = 0;
    m_Curve.nYDigits = 0;
    m_Curve.nYExDigits = 0;
    m_dXStart = 0;
    m_dYStart = 0;
    m_dXMin = 0;
    m_dYMin = 0;
    m_dXMax = 0;
    m_dYMax = 0;
    m_dYExMin = 0;
    m_dYExMax = 0;
    m_dYUnit = 0;
    m_dYExUnit = 0;
    m_dXUnit = 0;
    m_Curve.nCol = 10;
    m_Curve.nRow = 10;

    m_CurPos.ix		= 0;
    m_CurPos.iy		= 0;
    m_nOpMode	= CURVE_OPERATION_NONE;
    m_bOut					= TRUE;
    m_nXPosDelta =
    m_nYPosDelta = 0;
    m_dXValueDelta =
    m_dYValueDelta = 0;
    //m_dYExValueDelta =0;

    m_Curve.pszName = NULL;

    m_Curve.pszXName = NULL;
    m_Curve.pszYName = NULL;
    m_Curve.pszYNameEx =NULL;

    m_bDoubleBuffer = TRUE;
    m_bXName = TRUE;
    m_bYName = TRUE;
    m_bYNameEx = FALSE;
    m_nShowTitleType = CURVE_XXPOS_TOP;
    m_nXAxesColor	=
    m_nYAxesColor	= YELLOW;
    m_nShowYAxesType = CURVE_XXPOS_LEFT;
    m_nShowXAxesType = CURVE_XXPOS_BOTTOM;

    m_nShowXScaleType =
    m_nShowYScaleType = CURVE_VALUE_ALL;

    m_nXCoordinateColor =
    m_nYCoordinateColor = RGB2PIXEL(0x88,0x88,0x88);
	
    m_bShowGrid = TRUE;
    memset(&m_rectCutLine, 0, sizeof(m_rectCutLine));
	memset(&m_monitor_attr,0,sizeof(m_monitor_attr));
	m_monitor_attr.color=0x134F;
	m_monitor_attr.line_width=2;
	
	for (int i = 0; i < 4; i++)	m_pszDBID[i] = NULL;
    for (int i = 0; i < CURVE_LINE_MAX; i++)
    {
        //m_pLine[i] 				= new LINE;
        m_pLine[i].nFlag 		= CURVE_CURVELINE;
        m_pLine[i].nRefernce	= 0;
        m_pLine[i].pszName 	= NULL;
        m_pLine[i].nCount 		= 0;
        m_pLine[i].pValue 		= NULL;
        m_pLine[i].dwColor		= 0xFFFF;

        m_pszLineNameID[i] 		= NULL;

        m_BufferLine[i] = new LINEBUFFER;
        memset(m_BufferLine[i], 0, sizeof(LINEBUFFER));
    }


    //	fans add 2009/9/8 01:07�U��
    m_XRuleColor	= 0xFFFF;
    m_YRuleColor	= 0xFFFF;
    memset(m_szYRule, 0, sizeof(m_szYRule));
    memset(m_szXRule, 0, sizeof(m_szYRule));
    m_pFuncYStr		= NULL;
    m_pFuncXStr		= NULL;
    m_xPos			= 0;
    m_yPos			= 0;
    m_dXPosValue	= 0;
    m_xPerMove		= 1;
    m_yPerMove		= 1;
    m_xMoveInterval	=5;
    m_yMoveInterval =5;
    m_dfPercent		= 0;
    m_pszPromptID	= NULL;
    m_pszPrompt		= NULL;

    m_bTouchSupport = TRUE;

    m_nHRulePos		= 0;
    m_nVRulePos		= 0;
    m_RuleType		= RULE_NONE;		//�q�{�������Ф�
    m_nPointIndex	= 0;
    m_nLineIndex	= 0;

    m_nXRange		= 0;
    m_nInterval		= 1;
    m_nShowCutLineType=0;
}

CtmCurveEx2::~CtmCurveEx2()
{

    for (int i = 0; i < CURVE_LINE_MAX; i++)
    {
        //delete m_pLine[i];
        for (int j = 0; j < m_BufferLine[i]->nLineCount; j++)
        {
            if (m_BufferLine[i]->pValue[j] != NULL)
            {
                delete []m_BufferLine[i]->pValue[j];
            }
        }
        delete m_BufferLine[i];
    }

    //	free the class
    if (m_pDoubleBuffer != NULL)
    {
        delete m_pDoubleBuffer;
        m_pDoubleBuffer = NULL;
    }

}

//	fans add
BOOL	CtmCurveEx2::CreateA()
{
    CtmWnd::CreateA();

    m_dOldXMin = m_dXMin;
    m_dOldXMax = m_dXMax;

    m_dOldYExMin = m_dYExMin;
    m_dOldYExMax = m_dYExMax;

    m_dOldYMin = m_dYMin;
    m_dOldYMax = m_dYMax;

    m_nMaxPoint	= 2000;
    m_Image.CopyImage(m_rectClient.left, m_rectClient.top, \
    m_rectClient.right, m_rectClient.bottom);
    if (m_bDoubleBuffer && m_pDoubleBuffer == NULL)
    {
        m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
        m_pDoubleBuffer->InitBuffer(m_bgc);
    }


    //StrIDToStr(m_pszPromptID, &m_pszPrompt);
    if(m_pszPromptID!=NULL)
	{
		if(m_pszPrompt!=NULL)
		{	
			free(m_pszPrompt);
		}
		char* pszTemp=g_MultiLanguage[m_pszPromptID];
		if(pszTemp!=NULL)
		{
			int len=0;
			WORD* pwCode  = (WORD*)pszTemp;
			while(pwCode[len ++] != 0);
			m_pszPrompt = (char*)malloc(len<<1);//new char[(len << 1)];
			memcpy(m_pszPrompt, pszTemp, len << 1);
		}
	}
    return TRUE;
}

void	CtmCurveEx2::SetPropertys()
{
    CtmWnd::SetPropertys();

    SetPropInfo((char *)"doublebuffer",	tmBOOL	,	&m_bDoubleBuffer);

    SetPropInfo((char *)"curvetype",	tmINT,		&m_Curve.nFlag);
    SetPropInfo((char *)"xdigits",		tmINT,		&m_Curve.nXDigits);
    SetPropInfo((char *)"ydigits",		tmINT,		&m_Curve.nYDigits);
    SetPropInfo((char *)"yexdigits",	tmINT, 		&m_Curve.nYExDigits);
    SetPropInfo((char *)"cols",			tmINT,		&m_Curve.nCol);
    SetPropInfo((char *)"rows",			tmINT,		&m_Curve.nRow);
    SetPropInfo((char *)"curvename",	tmSTR,		&m_Curve.pszName);
    SetPropInfo((char *)"curvenameID",	tmSTR,		&m_pszCurveNameID);
    SetPropInfo((char *)"xname",		tmUSTR,		&m_Curve.pszXName);
    SetPropInfo((char *)"yname",		tmUSTR,		&m_Curve.pszYName);
    SetPropInfo((char *)"ynameEx",		tmUSTR,		&m_Curve.pszYNameEx);
    SetPropInfo((char *)"xnameID",		tmSTR,		&m_pszXNameID);
    SetPropInfo((char *)"ynameID",		tmSTR,		&m_pszYNameID);
    SetPropInfo((char*) "ynameExID",	tmSTR, 		&m_pszYNameExID);
    SetPropInfo((char *)"xmin",			tmDOUBLE,	&m_dXMin);
    SetPropInfo((char *)"xmax",			tmDOUBLE,	&m_dXMax);
    SetPropInfo((char *)"ymin",			tmDOUBLE,	&m_dYMin);
    SetPropInfo((char *)"ymax",			tmDOUBLE,	&m_dYMax);
    SetPropInfo((char *)"yexmin",		tmDOUBLE,	&m_dYExMin);
    SetPropInfo((char *)"yexmax",		tmDOUBLE,	&m_dYExMax);
    SetPropInfo((char *)"xstart",		tmDOUBLE,	&m_dXStart);
    SetPropInfo((char *)"xend",			tmDOUBLE,	&m_dXEnd);
    SetPropInfo((char *)"ystart",		tmDOUBLE,	&m_dYStart);
    SetPropInfo((char *)"yend",			tmDOUBLE,	&m_dYEnd);
    SetPropInfo((char *)"xunit",		tmDOUBLE,	&m_dXUnit);
    SetPropInfo((char *)"yunit",		tmDOUBLE,	&m_dYUnit);
    SetPropInfo((char *)"yexunit",		tmDOUBLE, 	&m_dYExUnit);


    SetPropInfo((char *)"xnameshow",	tmBOOL	,	&m_bXName);
    SetPropInfo((char *)"ynameshow",	tmBOOL	,	&m_bYName);
    SetPropInfo((char *)"ynameexshow",	tmBOOL	,	&m_bYNameEx);

    SetPropInfo((char *)"xaxestype",	tmINT	,	&m_nShowXAxesType);
    SetPropInfo((char *)"yaxestype",	tmINT	,	&m_nShowYAxesType);
    SetPropInfo((char *)"xscaletype",	tmINT	,	&m_nShowXScaleType);
    SetPropInfo((char *)"yscaletype",   tmINT	,	&m_nShowYScaleType);
    SetPropInfo((char *)"titletype",    tmINT	,	&m_nShowTitleType);
    SetPropInfo((char *)"cutlinetype",  tmINT	,	&m_nShowCutLineType);
    SetPropInfo((char *)"xgridcolor",	tmINT	,	&m_nXCoordinateColor);
    SetPropInfo((char *)"ygridcolor",   tmINT	,	&m_nYCoordinateColor);
    SetPropInfo((char *)"xaxescolor",   tmINT	,	&m_nXAxesColor);
    SetPropInfo((char *)"yaxescolor",   tmINT	,	&m_nYAxesColor);
    SetPropInfo((char *)"ruletype",		tmINT	,	&m_RuleType);


    SetPropInfo((char *)"showgrid",		tmBOOL,		&m_bShowGrid);			//Gerry add 2011/9/17 �U�� 03:22:15

    //	fans add  2009/9/9 03:33�U��
    SetPropInfo((char *)"yrulecolor",	tmINT	,	&m_YRuleColor);
    SetPropInfo((char *)"xrulecolor",	tmINT	,	&m_XRuleColor);
    SetPropInfo((char *)"xmove",		tmINT	,	&m_xMoveInterval);
    SetPropInfo((char *)"ymove",		tmINT	,	&m_yMoveInterval);
    SetPropInfo((char *)"prompt",		tmUSTR,		&m_pszPrompt);
    SetPropInfo((char *)"promptID",		tmSTR,		&m_pszPromptID);

    SetPropInfo((char *)"touch",		tmBOOL,		&m_bTouchSupport);
    SetPropInfo((char *)"xrange", 		tmINT, &m_nXRange);
    SetPropInfo((char *)"baseline", 	tmINT, &m_nLineIndex);
    SetPropInfo((char *)"interval", 	tmINT, &m_nInterval);
    SetPropInfo((char *)"maxpoint", 	tmINT, &m_nMaxPoint);
    SetPropInfo((char *)"rulemark", 	tmBOOL,&m_bRuleMark);
    
	SetPropInfo((char *)"monitor", 		tmBOOL,&m_monitor_attr.flag);
	SetPropInfo((char *)"monicolor", 	tmINT, &m_monitor_attr.color);
	SetPropInfo((char *)"moniref", 		tmINT, &m_monitor_attr.reference);
	SetPropInfo((char *)"monilinewidth", 	tmINT, &m_monitor_attr.line_width);
	SetPropInfo((char *)"x1", 			tmDOUBLE, &m_monitor_attr.point1.dX);
	SetPropInfo((char *)"y1", 			tmDOUBLE, &m_monitor_attr.point1.dY);
	SetPropInfo((char *)"x2", 			tmDOUBLE, &m_monitor_attr.point2.dX);
	SetPropInfo((char *)"y2", 			tmDOUBLE, &m_monitor_attr.point2.dY);


    char Name[20];
    char temp[3];
    for (int i = 0 ; i < CURVE_LINE_MAX; i++)
    {
        temp[0] = '\0';
        sprintf(temp, "%d", i);

        strcpy(Name, "linecolor");
        strcat(Name, temp);
        SetPropInfo(Name, 	tmDWORD,	&m_pLine[i].dwColor);

        strcpy(Name, "linename");
        strcat(Name, temp);
        SetPropInfo(Name, 	tmUSTR,	&m_pLine[i].pszName);

        strcpy(Name, "linenameID");
        strcat(Name, temp);
        SetPropInfo(Name, 	tmSTR,	&m_pszLineNameID[i]);


        strcpy(Name, "linetype");
        strcat(Name, temp);
        SetPropInfo(Name, 	tmINT,	&m_pLine[i].nFlag);
    }
    char sz[10];
	for (int i = 0; i < 4; i++)
	{
		sprintf(sz, "%s%d", "dbid", i);
		SetPropInfo(sz,		tmSTR, 		&m_pszDBID[i]);	
	}
}



void	CtmCurveEx2::SetPropertys_Nub()
{

}

//	fans add
void	CtmCurveEx2::SetPoint(char* pszLineName, CURVEPOINT 	*pValue, int iCount, int nReference)	//	fans add
{
    //	2008/4/2 10:56�W��	fans mark
    //m_nReferenceFrame = nReference;
    for (int i = 0; i < CURVE_LINE_MAX; i++)
    {
        if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
        {
            SetPoint(i, pValue, iCount, nReference);
            break;
        }
    }
}

//	fans add
void	CtmCurveEx2::SetPoint(int   iLineIndex,  CURVEPOINT 	*pValue, int iCount, int nReference)	//	fans add
{
	m_bPropertyChanged=TRUE;
    m_pLine[iLineIndex].nCount 	= iCount;
    m_pLine[iLineIndex].nRefernce	= nReference;		//	2008/4/2 10:58�W�� fans add
    if (pValue == NULL) m_pLine[iLineIndex].nCount = 0;

    if (iCount == 0) m_pLine[iLineIndex].pValue = NULL;
    else m_pLine[iLineIndex].pValue = pValue;

    if (m_pLine[iLineIndex].pValue != NULL )
    {
    	if(nReference==1)
    	{
	        for (int i = 0; i < m_pLine[iLineIndex].nCount; i++)
	        {
	            if (m_pLine[iLineIndex].pValue[i].dY > m_dYExMax) 	m_dYExMax = m_pLine[iLineIndex].pValue[i].dY+5;
	
	            if (m_pLine[iLineIndex].pValue[i].dY < m_dYExMin)	m_dYExMin = m_pLine[iLineIndex].pValue[i].dY+5;
	
	        }
	     }
	     else
	     {
	     	for (int i = 0; i < m_pLine[iLineIndex].nCount; i++)
	        {
	            if (m_pLine[iLineIndex].pValue[i].dY > m_dYMax) 	m_dYMax = m_pLine[iLineIndex].pValue[i].dY+5;
	
	            if (m_pLine[iLineIndex].pValue[i].dY < m_dYMin)		m_dYMin = m_pLine[iLineIndex].pValue[i].dY+5;
	
	        }
	     }
    }
}

void	CtmCurveEx2::SetPointT(int iLineIndex, CURVEPOINT *pValue, int iCount, int nReference)
{
    int i;

    if (iCount == 0)
        return;

	m_bPropertyChanged=TRUE;
    CURVEPOINT	*pPoint = NULL, *points = new CURVEPOINT[iCount * 2];

    memset(points, 0, sizeof(CURVEPOINT) * iCount * 2);
    pPoint = points;

    for (i = 0; i < iCount; i++)
    {
        pPoint->dX = pValue[i].dX;
        pPoint->dY = pValue[i].dY;

        if (i == iCount - 1)
            break;

        (pPoint + 1)->dX = pValue[i].dX;
        (pPoint + 1)->dY = pValue[i + 1].dY;

        pPoint += 2;
    }
    memcpy(pValue, points, sizeof(CURVEPOINT) * (iCount * 2 - 1));
    SetPoint(iLineIndex, pValue, iCount * 2 - 1, nReference);
    delete []points;
}

void	CtmCurveEx2::SetPointBuffer(char* pszLineName, CURVEPOINT *pValue, int iCount, int nReference)
{
    //m_nReferenceFrame = nReference;
    for (int i = 0; i < CURVE_LINE_MAX; i++)
    {
        if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
        {
            SetPointBuffer(i, pValue, iCount, nReference);
            break;
        }
    }
}

void	CtmCurveEx2::SetPointBuffer(int	iLineIndex,  CURVEPOINT *pValue, int iCount, int nReference)
{

	m_bPropertyChanged=TRUE;
    m_BufferLine[iLineIndex]->nRefernce = nReference;	//	2008/4/2 11:00�W�� fans add

    if (m_BufferLine[iLineIndex]->nLineCount > 0)
    {
        //printf("iCount=%d m_nMaxPoint=%d \n", iCount, m_nMaxPoint);
        if (iCount > m_nMaxPoint)	//James modify MAX_POINT_NUM-->m_nMaxPoint 2015/1/19 16:20:48
            iCount = m_nMaxPoint;
        if (pValue == NULL) 	iCount = 0;



        m_BufferLine[iLineIndex]->nCount[m_BufferLine[iLineIndex]->nIndex] = iCount;
        memcpy(m_BufferLine[iLineIndex]->pValue[m_BufferLine[iLineIndex]->nIndex++],
              pValue, sizeof(CURVEPOINT) * iCount);

        if (m_BufferLine[iLineIndex]->nIndex >= m_BufferLine[iLineIndex]->nLineCount)
            m_BufferLine[iLineIndex]->nIndex = 0;

        if (1 == m_BufferLine[iLineIndex]->nRefernce)
        {
            for (int i = 0; i < iCount; i++)
            {
                if (pValue[i].dY > m_dYExMax)
                    m_dYExMax = pValue[i].dY;

                if (pValue[i].dY < m_dYExMin)
                    m_dYExMin= pValue[i].dY;

            }
        }
    }


}

BOOL	CtmCurveEx2::SetLineNum(char* pszLineName, int iNum)
{
    BOOL	result = FALSE;
    for (int i = 0; i < CURVE_LINE_MAX; i++)
    {
        if (strcmp(m_pLine[i].pszName, pszLineName) == 0)
        {
            SetLineNum(i, iNum);
            m_bPropertyChanged=TRUE;
            break;
        }
    }
    return result;
}

BOOL	CtmCurveEx2::SetLineNum(int	iLineIndex, int iNum)
{
    BOOL	result = FALSE;
    if ((iNum <= CURVE_BUFFERLINE_MAX) && (iLineIndex < CURVE_LINE_MAX))
    {
        if (iNum > 0)
        {
            for (int i = 0; i < iNum; i++)
            {
                if (m_BufferLine[iLineIndex]->pValue[i] == NULL)
                {

                    m_BufferLine[iLineIndex]->pValue[i] = new CURVEPOINT[m_nMaxPoint];		//James modify MAX_POINT_NUM --> m_nMaxPoint 2015/1/19 16:21:11
                    memset(m_BufferLine[iLineIndex]->pValue[i], 0, sizeof(CURVEPOINT) * m_nMaxPoint);

                    m_BufferLine[iLineIndex]->nIndex = 0;
                    m_BufferLine[iLineIndex]->nCount[i] = 0;	//	the num of the valid value
                    //m_nALineIndex[iLineIndex] = 0;
                }
            }
            //m_nALineNum[iLineIndex] = iNum;
            m_BufferLine[iLineIndex]->nLineCount = iNum;
        }
        else
        {
            for (int i = 0; i < m_BufferLine[iLineIndex]->nLineCount; i++)
            {
                if (m_BufferLine[iLineIndex]->pValue[i] != NULL)
                {
                    delete []m_BufferLine[iLineIndex]->pValue[i];
                    m_BufferLine[iLineIndex]->pValue[i] = NULL;
                }
                m_BufferLine[iLineIndex]->nIndex  = 0;
            }
            m_BufferLine[iLineIndex]->nLineCount = 0;
        }
        result = TRUE;
    }

    return result;
}

void	CtmCurveEx2::PropareShow()
{
    InitData();
    //	Calculate Y

    if (m_dYMax == 0) m_dYMax = m_dYMin + m_Curve.nRow * m_dYUnit;
    else m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;

    if(m_dYExMax==0) m_dYExMax =m_dYExMin+m_Curve.nRow*m_dYExUnit;
    else m_dYExUnit=(m_dYExMax-m_dYExMin)/ m_Curve.nRow;

    //	Calculate X
    if (m_dXMax == 0 && !(m_Curve.nFlag & CURVE_XCONTRARY)) m_dXMax = m_dXMin + m_Curve.nCol * m_dXUnit;
    else m_dXUnit = (m_dXMax - m_dXMin) / m_Curve.nCol;

    if (m_pszCurveNameID != NULL)
        StrIDToStr(m_pszCurveNameID, &m_Curve.pszName);
    if (m_pszXNameID != NULL)
        StrIDToStr(m_pszXNameID, &m_Curve.pszXName);
    if (m_pszYNameID != NULL)
        StrIDToStr(m_pszYNameID, &m_Curve.pszYName);
    if(m_pszYNameExID !=NULL)
        StrIDToStr(m_pszYNameExID,&m_Curve.pszYNameEx);

    for (int i = 0 ; i < CURVE_LINE_MAX; i++)
        StrIDToStr(m_pszLineNameID[i], &m_pLine[i].pszName);

    if (m_pDoubleBuffer != NULL)
        m_pDoubleBuffer->InitBuffer(m_bgc);
    else
        Clear();

    m_rectCoordinate.left	= 0;
    m_rectCoordinate.right	= GetClientWidth() - 1;
    m_rectCoordinate.top	= 0;
    m_rectCoordinate.bottom	= GetClientHeight() -1;

}

int		CtmCurveEx2::GetRangeXY()
{
    int i;
    CURVEPOINT	*pPoint;
    double dXMin = 0;
    double dXMax = 0;
    int    nIndex = 0;

    dXMin = fmin(m_dXMin, m_dXMax);
	dXMax = fmax(m_dXMin, m_dXMax);
    //printf("1:%lf,%lf<--->%lf,%lf\n",m_dYMin,m_dYMax,m_dYExMin,m_dYExMax);

    for(nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
    {
        if(m_pLine[nIndex].pValue == NULL) continue;
        pPoint	= m_pLine[nIndex].pValue;
        for(i = 0; i < m_pLine[nIndex].nCount; i ++)
        {
        	
        	//printf("nIndex=%d,m_pLine[nIndex].nCount=%d,pPoint->dX=%lf\n",
        	//nIndex,m_pLine[nIndex].nCount,pPoint->dX);
            if(dXMax < pPoint->dX)
                    dXMax	= pPoint->dX;
            if(dXMin > pPoint->dX)
                    dXMin	= pPoint->dX;

            if ((m_dYMax < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
                m_dYMax = pPoint->dY;
            else if ((m_dYExMax < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
                m_dYExMax = pPoint->dY;

            if ((m_dYMin > pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
                m_dYMin = pPoint->dY;
            else if ((m_dYExMin > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
                m_dYExMin = pPoint->dY;

            pPoint ++;
        }
    }

    if (m_nXRange > 0 && m_nXRange <= CURVE_LINE_MAX && m_pLine[m_nXRange - 1].pValue != NULL)
    {
        nIndex = m_nXRange - 1;
        pPoint	= m_pLine[nIndex].pValue;

        if (m_pLine[nIndex].nCount > 0)
        {
            dXMin = dXMax = pPoint->dX;
        }

        for(i = 0; i < m_pLine[nIndex].nCount; i ++)
        {
            if(dXMax < pPoint->dX)
                    dXMax	= pPoint->dX;
            if(dXMin > pPoint->dX)
                    dXMin	= pPoint->dX;

            pPoint ++;
        }
    }

    for(int nIndex = 0; nIndex < CURVE_LINE_MAX; nIndex ++)
    {
        for (i = 0; i < CURVE_BUFFERLINE_MAX; i++)
        {
            if (m_BufferLine[nIndex]->pValue[i] == NULL) continue;
            pPoint = m_BufferLine[nIndex]->pValue[i];
            for (int j = 0; j < m_BufferLine[nIndex]->nCount[i]; j++)
            {
                if(dXMax < pPoint->dX)
                    dXMax	= pPoint->dX;
                if(dXMin > pPoint->dX)
                    dXMin	= pPoint->dX;


                if	((m_dYMax < pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
                    m_dYMax = pPoint->dY;
                else if ((m_dYExMax < pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
                    m_dYExMax = pPoint->dY;

                if	((m_dYMin> pPoint->dY) && (0 == m_pLine[nIndex].nRefernce))
                    m_dYMin = pPoint->dY;
                else if ((m_dYExMin > pPoint->dY) && (1 == m_pLine[nIndex].nRefernce))
                    m_dYExMin = pPoint->dY;

                pPoint ++;
            }

        }
    }


    if (m_Curve.nFlag & CURVE_XCONTRARY)
    {
        m_dXMax = dXMin;
        m_dXMin = dXMax;
    }
    else
    {
        m_dXMax = dXMax;
        m_dXMin = dXMin;
    }

    m_dYUnit = (m_dYMax - m_dYMin) / m_Curve.nRow;
    m_dYExUnit =(m_dYExMax-m_dYExMin)/ m_Curve.nRow;
    m_dXUnit = (m_dXMax - m_dXMin) / m_Curve.nCol;

    //printf("2:%lf,%lf<--->%lf,%lf\n",m_dYMin,m_dYMax,m_dYExMin,m_dYExMax);
    return 0;
}

RECT	CtmCurveEx2::GetTitleRect()
{
    RECT Result;
//	RECT TempRect;
    memset(&Result, 0, sizeof(Result));

    if (m_nShowTitleType != CURVE_XXPOS_TOP && m_nShowTitleType != CURVE_XXPOS_BOTTOM)
    {
        m_bTitle = FALSE;
        //m_relationDown.pRect = NULL;
    }
    else if (CURVE_XXPOS_TOP == m_nShowTitleType)
    {

        m_bTitle = TRUE;
        AddTopRelation(&m_relationTitle, m_pcFont->m_wHeight);
        Result = *m_relationTitle.pRect;
    }
    else if (CURVE_XXPOS_BOTTOM == m_nShowTitleType)
    {
        m_bTitle = TRUE;
        AddDownRelation(&m_relationTitle, m_pcFont->m_wHeight);
        Result = *m_relationTitle.pRect;
    }
    return Result;
}

RECT	CtmCurveEx2::GetCutRect()
{
    RECT Result;
    memset(&Result, 0, sizeof(Result));
    m_bCutLine = FALSE;
    int		i;
    int		nLineCount = 0;
    int		nCutLineWidth, nCutLineHeight;
    int		nCutLineLen, nCutLineStrMaxLen, nCutLineStrTotalLen;
    int		nStrLen;
    int		nStartLen, nMiddleLen, nEndLen;
    int		nVCutLine,		nHCutLine;
    //RECT	rectCutLine;
    //int		nLeft, nTop;

    nCutLineLen	= 80;
    nStartLen	=
    nMiddleLen	=
    nEndLen		= 5;

    nCutLineStrMaxLen	=
    nCutLineStrTotalLen	= 0;

    if((m_Curve.nFlag & CURVE_CUTLINE) == 0)
    {
        return Result;
    }

    for(i = 0; i < CURVE_LINE_MAX; i ++)
    {
        //if(m_pLine[i] == NULL)	continue;	fans mark

        //	fans add
        if (m_pLine[i].pValue  == NULL) continue;


        nLineCount ++;

        if(m_pLine[i].pszName == NULL) continue;

        nStrLen	= strlen(m_pLine[i].pszName);
        nCutLineStrMaxLen	= (nStrLen > nCutLineStrMaxLen) ?
                                nStrLen :  nCutLineStrMaxLen;
        nCutLineStrTotalLen	   += nStrLen;
    }



    if(m_Curve.nFlag & (CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT))
    {
        nCutLineWidth	= nCutLineStrMaxLen + nCutLineLen
                            + nStartLen + nMiddleLen + nEndLen;
        nCutLineHeight	= (m_pcFont->m_wHeight + 2) * nLineCount;

        nVCutLine	= 1;
        nHCutLine	= 0;
        m_bCutLine = TRUE;
    }
    else if(m_Curve.nFlag & (CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM))
    {
        nCutLineWidth	= nCutLineStrTotalLen +	(nStartLen + nMiddleLen
                            + nEndLen + nCutLineLen) * nLineCount;
        nCutLineHeight	= m_pcFont->m_wHeight + 2;
        nVCutLine	= 0;
        nHCutLine	= 1;
        m_bCutLine = TRUE;
    }

    //======
    // �p��ϨҦ�m
    //======
    switch(m_Curve.nFlag & CURVE_CUTLINE)
    {
        case CURVE_CUTLINE_LEFT:
            AddLeftRelation(&m_relationCutLine, nCutLineWidth);
            Result = *m_relationCutLine.pRect;
            break;
        case CURVE_CUTLINE_RIGHT:
            AddRightRelation(&m_relationCutLine, nCutLineWidth);
            Result = *m_relationCutLine.pRect;
            break;
        case CURVE_CUTLINE_TOP:
            AddTopRelation(&m_relationCutLine, nCutLineHeight);
            m_relationCutLine.pRect->bottom += 1;
            Result = *m_relationCutLine.pRect;
            break;
        case CURVE_CUTLINE_BOTTOM:
            AddDownRelation(&m_relationCutLine, nCutLineHeight);
            Result = *m_relationCutLine.pRect;
            break;
        default	:
            break;
    }
    return Result;
}

RECT	CtmCurveEx2::GetXNameRect()
{
    RECT Result;
    if (m_bXName)
    {
        switch (m_nShowXAxesType)
        {
            case CURVE_XXPOS_TOP:
                AddTopRelation(&m_relationXName, m_pcFont->m_wHeight);
                Result = *m_relationXName.pRect;
                break;
            case CURVE_XXPOS_BOTTOM:
                AddDownRelation(&m_relationXName, m_pcFont->m_wHeight);
                Result = *m_relationXName.pRect;
                break;
            default:
                m_bXName = FALSE;
                break;
        }
    }
    return Result;
}

RECT	CtmCurveEx2::GetYNameRect()
{
    RECT Result;
    if (m_bYName)
    {
        switch (m_nShowYAxesType)
        {
            case CURVE_XXPOS_LEFT:
                AddLeftRelation(&m_relationYName, m_pcFont->m_wWidth);
                //Result = *m_relationYName.pRect;
                break;
            case CURVE_XXPOS_RIGHT:
                AddRightRelation(&m_relationYName, m_pcFont->m_wWidth);
                //Result = *m_relationYName.pRect;
                break;
            default:
                m_bYName = FALSE;
                break;
        }
    }
    if(m_bYNameEx)
    {
        switch (m_nShowYAxesType)
        {
            case CURVE_XXPOS_LEFT:
                AddRightRelation(&m_relationYNameEx, m_pcFont->m_wWidth);

                //Result = *m_relationYNameEx.pRect;
                break;
            case CURVE_XXPOS_RIGHT:
                AddLeftRelation(&m_relationYNameEx, m_pcFont->m_wWidth);
                //Result = *m_relationYNameEx.pRect;
                break;
            default:
                m_bYNameEx = FALSE;
                break;
        }
    }
    return Result;
}

RECT	CtmCurveEx2::GetXAxesScaleRect()
{
    RECT Result;
    if (CURVE_VALUE_NOTHING != m_nShowXScaleType)
    {
        switch (m_nShowXAxesType)
        {
            case CURVE_XXPOS_TOP:
                AddTopRelation(&m_relationXAxesScale, m_pcFont->m_wHeight);
                Result = *m_relationXAxesScale.pRect;
                break;
            case CURVE_XXPOS_BOTTOM:
                AddDownRelation(&m_relationXAxesScale, m_pcFont->m_wHeight);
                Result = *m_relationXAxesScale.pRect;
                break;
            default:
                break;
        }
    }
    return Result;
}

RECT	CtmCurveEx2::GetYAxesScaleRect()
{
    RECT Result;

    if (CURVE_VALUE_NOTHING != m_nShowYScaleType)
    {
        char sz[100], szYFormat[10];
        char szEx[100], szYExFormat[10];
        int	nWidth = 0,nExWidth=0;
        snprintf(szYFormat, 10, "%%.%df", m_Curve.nYDigits);
        snprintf(szYExFormat, 10, "%%.%df", m_Curve.nYExDigits);
        for(int i = 0; i <= m_Curve.nRow; i ++)
        {
            snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);//m_dYUnit * nYUnit * i + m_dYMin);
            snprintf(szEx, 100, szYExFormat, m_dYExUnit * i + m_dYExMin);
            if ((int)(strlen(sz) * m_pcFont->m_wWidth) / 2 > nWidth)
                nWidth = strlen(sz) * m_pcFont->m_wWidth / 2;
            if ((int)(strlen(szEx) * m_pcFont->m_wWidth) / 2 > nExWidth)
                nExWidth = strlen(szEx) * m_pcFont->m_wWidth / 2;
        }


        switch (m_nShowYAxesType)
        {
            case CURVE_XXPOS_LEFT:
                AddLeftRelation(&m_relationYAxesScale, nWidth);
                AddRightRelation(&m_relationYAxesScaleEx, nExWidth);
                //Result = *m_relationYAxesScale.pRect;
                break;
            case CURVE_XXPOS_RIGHT:
                AddRightRelation(&m_relationYAxesScale, nWidth);
                AddLeftRelation(&m_relationYAxesScaleEx, nExWidth);
                //Result = *m_relationYAxesScale.pRect;
                break;
            default:
                break;
        }
    }


    return Result;
}

RECT	CtmCurveEx2::GetLineRect()
{
    RECT Result;
    int nXUnit, nYUnit;
    Result.left = GetLastLeft();
    Result.right = GetLastRight();
    Result.top = GetLastTop();
    Result.bottom = GetLastBottom();

    nXUnit	= (Result.right - Result.left) / m_Curve.nCol;
    nYUnit	= (Result.bottom - Result.top) / m_Curve.nRow;

    if (m_nShowYAxesType)
    {
        Result.right = Result.left + nXUnit * m_Curve.nCol;
        Result.bottom = Result.top + nYUnit * m_Curve.nRow;
    }
    m_rectLine = Result;

    return Result;
}

void	CtmCurveEx2::SeperateArea()
{
    GetTitleRect();				//	Title���d��
    GetCutRect();				//	��o�ϨҪ��d��
    GetXNameRect();	 			//	X�b�W�٪��d��
    GetYNameRect();				//	Y�b�W�٪��d��
    GetXAxesScaleRect();		//	���X�b���y�ЭȪ��d��
    GetYAxesScaleRect();		//	���Y�b���y�ЭȪ��d��
    GetLineRect();				//	����u�����e�Ͻd��

}

short	CtmCurveEx2::ShowExtentEx(short nX, short nY, char* pszString, int nType)
{
    if (m_pDoubleBuffer != NULL)
        return ShowExtentD(nX, nY, pszString, nType);
    else return ShowExtent(nX, nY, pszString);
}

short   CtmCurveEx2::ShowExtentD(short nX, short nY, char* pszString, int nType)
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

void	CtmCurveEx2::ShowTitle()
{
    if(m_Curve.pszName != NULL)
    {
        int	nPos = 0;
        nPos = m_rectTitle.left +
                (m_rectTitle.right - m_rectTitle.left - strlen(m_Curve.pszName) * m_pcFont->m_wWidth / 2) / 2;
        ShowExtentEx(nPos, m_rectTitle.top ,m_Curve.pszName, m_pcFont->GetFontCode());
    }
}

int		CtmCurveEx2::DrawCutLine()
{
    int		i;
    int		nLineCount = 0;
    int		nCutLineWidth, nCutLineHeight;
    int		nCutLineLen, nCutLineStrMaxLen, nCutLineStrTotalLen;
    int		nStrLen;
    int		nStartLen, nMiddleLen, nEndLen;
    int		nVCutLine,		nHCutLine;
    RECT	rectCutLine;
    int		nLeft, nTop;

    //========
    // �p��ϨҪ��j�p
    //========
    nCutLineStrMaxLen	=
    nCutLineStrTotalLen	= 0;
    if((m_Curve.nFlag & CURVE_CUTLINE) == 0)
    {
        return -1;
    }
    for(i = 0; i < CURVE_LINE_MAX; i ++)
    {
        //	fans add James modify 2008/11/19 9:53AM
        if (m_pLine[i].pValue  == NULL &&
            m_BufferLine[i]->nLineCount <= 0) continue;

        nLineCount ++;

        if(m_pLine[i].pszName == NULL) continue;

        //Rw.Ying	2013/3/14 05:57�U��
        nStrLen = GetStringExtent(m_pLine[i].pszName).w;
        nCutLineStrMaxLen	= (nStrLen > nCutLineStrMaxLen) ?
                                nStrLen :  nCutLineStrMaxLen;
        nCutLineStrTotalLen	   += nStrLen;
    }
    if(nLineCount == 0)	return 0;

    nCutLineLen	= 15;
    nStartLen	=
    nMiddleLen	=
    nEndLen		= 5;

    if(m_Curve.nFlag & (CURVE_CUTLINE_LEFT | CURVE_CUTLINE_RIGHT))
    {
        nCutLineWidth	= nCutLineStrMaxLen + nCutLineLen
                            + nStartLen + nMiddleLen + nEndLen;
        nCutLineHeight	= (m_pcFont->m_wHeight + 2) * nLineCount;

        nVCutLine	= 1;
        nHCutLine	= 0;
    }
    else if(m_Curve.nFlag & (CURVE_CUTLINE_TOP | CURVE_CUTLINE_BOTTOM))
    {
        nCutLineWidth	= nCutLineStrTotalLen +	(nStartLen + nMiddleLen
                            + nEndLen + nCutLineLen) * nLineCount;
        nCutLineHeight	= m_pcFont->m_wHeight + 2;
        nVCutLine	= 0;
        nHCutLine	= 1;
    }

    //======
    // �p��ϨҦ�m
    //======
    switch(m_Curve.nFlag & CURVE_CUTLINE)
    {

        case CURVE_CUTLINE_LEFT:
            rectCutLine.left	= m_rectCoordinate.left + 1;
            rectCutLine.right	= rectCutLine.left + nCutLineWidth;
            rectCutLine.top	= (m_rectCoordinate.bottom
                        - m_rectCoordinate.top - nCutLineHeight) / 2;
            rectCutLine.bottom= rectCutLine.top + nCutLineHeight;

            m_rectCoordinate.left	= rectCutLine.right + 1;
            break;
        case CURVE_CUTLINE_RIGHT:
            rectCutLine.right	= m_rectCoordinate.right - 1;
            rectCutLine.left	= rectCutLine.right - nCutLineWidth;
            rectCutLine.top	= (m_rectCoordinate.bottom
                        - m_rectCoordinate.top - nCutLineHeight) / 2;
            rectCutLine.bottom= rectCutLine.top + nCutLineHeight + 2;

            m_rectLine.right	= rectCutLine.left - 5;
            break;
        case CURVE_CUTLINE_TOP:
            rectCutLine.left	= (m_rectCoordinate.right
                        - m_rectCoordinate.left - nCutLineWidth) / 2;
            rectCutLine.right	= rectCutLine.left + nCutLineWidth;
            rectCutLine.top	= m_rectCoordinate.top + 2;
            rectCutLine.bottom= rectCutLine.top + nCutLineHeight;

            m_rectCoordinate.top= rectCutLine.bottom + 1;
            break;
        case CURVE_CUTLINE_BOTTOM:
            rectCutLine.left	= (m_rectCoordinate.right
                        - m_rectCoordinate.left - nCutLineWidth) / 2;
            rectCutLine.right	= rectCutLine.left + nCutLineWidth;
            rectCutLine.bottom= m_rectCoordinate.bottom;
            rectCutLine.top	= rectCutLine.bottom - nCutLineHeight;

            m_rectCoordinate.bottom	= rectCutLine.top - 1;
            break;
        default	:
            return 0;
    }

    //=======
    // �e�Ϩ�
    //=======
    m_pDoubleBuffer->SetColorInBuffer(BLACK);

    m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER, rectCutLine.left,
                                            rectCutLine.top,
                                            rectCutLine.right,
                                            rectCutLine.bottom);


    nLeft	= rectCutLine.left;
    nTop	= rectCutLine.top;

    for(i = 0; i < CURVE_LINE_MAX; i ++)
    {

        if(m_pLine[i].pValue == NULL &&
            m_BufferLine[i]->nLineCount <= 0)	continue;


        m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);

        m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR,
                    nLeft + nStartLen,
                    nTop + m_pcFont->m_wHeight / 2,
                    nLeft + nStartLen + nCutLineLen,
                    nTop + m_pcFont->m_wHeight / 2 + 2
                  );


        if(m_pLine[i].pszName == NULL)
        {
            nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen) * nHCutLine;
            nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
            continue;
        }


        ShowExtentD(	nLeft + nStartLen + nMiddleLen + nCutLineLen,
                    nTop  + 2, m_pLine[i].pszName, m_pcFont->GetFontCode());


        nLeft  += (nStartLen + nMiddleLen + nEndLen + nCutLineLen
                    + GetStringExtent(m_pLine[i].pszName).w) * nHCutLine;
        nTop   += (m_pcFont->m_wHeight + 2) * nVCutLine;
    }

    return 0;
}

void	CtmCurveEx2::DrawArrow()
{

}

void	CtmCurveEx2::DrawAxes()
{

    if (m_pDoubleBuffer != NULL)
    {
        //	draw y
        m_pDoubleBuffer->SetColorInBuffer(m_nYAxesColor);

        switch (m_nShowYAxesType)
        {
            case CURVE_XXPOS_RIGHT:
                m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.bottom);
                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top);
                if(m_bYNameEx)
                {
                    m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left , m_rectLine.bottom);
                    m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left, m_rectLine.top);
                }
                break;
            case CURVE_XXPOS_LEFT:
                m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left , m_rectLine.bottom);
                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left, m_rectLine.top);
                if(m_bYNameEx)
                {
                    m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right, m_rectLine.bottom);
                    m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top);
                }
                break;
            default:
                break;
        }
        //	draw x
        m_pDoubleBuffer->SetColorInBuffer(m_nXAxesColor);
        m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left, m_rectLine.bottom);
        m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom);
    }
    else
    {
        _SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
            m_rectCoordinate.right, m_rectCoordinate.bottom);
        //	draw y
        _SetColor(m_nYAxesColor);
        _MoveTo(m_rectLine.left, m_rectLine.bottom);
        _LineTo(m_rectLine.left, m_rectLine.top);

        //	draw x
        _SetColor(m_nXAxesColor);
        _MoveTo(m_rectLine.left, m_rectLine.bottom);
        _LineTo(m_rectLine.right, m_rectLine.bottom);
    }

}

void	CtmCurveEx2::DrawAxesGrid()
{
    int	nXUnit, nYUnit;
    int	i = 0;
    if (m_Curve.nCol <= 0) m_Curve.nCol = 1;
    if (m_Curve.nRow <= 0) m_Curve.nRow = 1;
    //=============
    //	attention: m_rectLine.right - m_rectLine.left is divisibility,
    //			   m_rectLine.bottom - m_rectLine.top too
    //=============
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;

    if (m_Curve.nFlag & CURVE_COOR_BROKENLINE)
    {
        if (m_pDoubleBuffer != NULL)
        {

            //	Draw vertical line in the grid
            m_pDoubleBuffer->SetColorInBuffer(m_nYCoordinateColor);
            for(i = 1; i <= m_Curve.nCol; i ++)
            {
                for (int j = 0; j <= (m_rectLine.bottom - m_rectLine.top) / 4; j++)
                {
                    m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + nXUnit * i,	m_rectLine.top + j * 4);
                    m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.top + j * 4 + 1);
                }

                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.bottom);
            }

            //	draw horizontal line in the grid
            m_pDoubleBuffer->SetColorInBuffer(m_nXCoordinateColor);
            for(i = 1; i <= m_Curve.nRow; i ++)
            {
                for (int j = 0; j < (m_rectLine.right - m_rectLine.left) / 4; j++)
                {
                    m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + j * 4, m_rectLine.bottom - nYUnit * i - 1);
                    m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + j * 4 + 1, m_rectLine.bottom - nYUnit * i - 1);
                }

                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1);
            }
        }
        else
        {
            _SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
                        m_rectCoordinate.right, m_rectCoordinate.bottom);

            //	 vertical line in the grid
            _SetColor(m_nYCoordinateColor);
            for(i = 1; i <= m_Curve.nCol; i ++)
            {
                for (int j = 0; j <= (m_rectLine.bottom - m_rectLine.top) / 4; j++)
                {
                    _MoveTo(m_rectLine.left + nXUnit * i,	m_rectLine.top + j * 4);
                    _LineTo(m_rectLine.left + nXUnit * i, m_rectLine.top + j * 4 + 1);
                }

                _LineTo(m_rectLine.left + nXUnit * i, m_rectLine.bottom);
            }

            //	draw horizontal line in the grid
            _SetColor(m_nXCoordinateColor);
            for(i = 1; i <= m_Curve.nRow; i ++)
            {
                for (int j = 0; j < (m_rectLine.right - m_rectLine.left) / 4; j++)
                {
                    _MoveTo(m_rectLine.left + j * 4, m_rectLine.bottom - nYUnit * i - 1);
                    _LineTo(m_rectLine.left + j * 4 + 1, m_rectLine.bottom - nYUnit * i - 1);
                }

                _LineTo(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1);
            }
        }
    }
    else
    {
        if (m_pDoubleBuffer != NULL)
        {
            //	Draw vertical line in the grid
            m_pDoubleBuffer->SetColorInBuffer(m_nYCoordinateColor);
            for(i = 1; i <= m_Curve.nCol; i ++)
            {
                m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + nXUnit * i,	m_rectLine.bottom - 1);
                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.top - 1);
            }

            //	draw horizontal line in the grid
            m_pDoubleBuffer->SetColorInBuffer(m_nXCoordinateColor);
            for(i = 1; i <= m_Curve.nRow; i ++)
            {
                m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i - 1);
                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1);
            }
        }
        else
        {
            _SetViewPort(m_rectCoordinate.left, m_rectCoordinate.top,
                        m_rectCoordinate.right, m_rectCoordinate.bottom);

            //	 vertical line in the grid
            _SetColor(m_nYCoordinateColor);
            for(i = 1; i <= m_Curve.nCol; i ++)
            {
                _MoveTo(m_rectLine.left + nXUnit * i,	m_rectLine.bottom - 1);
                _LineTo(m_rectLine.left + nXUnit * i, m_rectLine.top);
            }

            //	draw horizontal line in the grid
            _SetColor(m_nXCoordinateColor);
            for(i = 1; i <= m_Curve.nRow; i ++)
            {
                _MoveTo(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i);
                _LineTo(m_rectLine.right, m_rectLine.bottom - nYUnit * i);
            }
        }
    }
}
void	CtmCurveEx2::DrawSclae()
{
    if (CURVE_VALUE_NOTHING != m_nShowXScaleType)
    {
    	//printf("m_dXMax=%d\n",m_dXMax);
        DrawXScale();
    }

    if (CURVE_VALUE_NOTHING != m_nShowYScaleType)
    {
        DrawYScale();
    }
}

void	CtmCurveEx2::DrawXScale()
{
    int nStrXOffset = 0;
    int i = 0;
    int	nXUnit, nStrOffset;
    char	sz[100], szXFormat[10];

    memset(sz, 0, sizeof(sz));
    snprintf(szXFormat, 10, "%%.%df", m_Curve.nXDigits);
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    //printf("%s scale contrary, xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);

    if((m_Curve.nFlag & CURVE_XCONTRARY) && m_dXMin < m_dXMax)
    {
        double temp = m_dXMin;
        m_dXMin = m_dXMax;
        m_dXMax = temp;
        m_dXUnit	= -m_dXUnit;
        //printf("%s scale contrary, xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);
    }

    snprintf(sz, 100, szXFormat, m_dXMin);//m_dXStart);
    nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
    if (CURVE_VALUE_MAXONLY != m_nShowXScaleType)
    {
        ShowExtentEx(m_rectLine.left, m_rectXAxesScale.top + 1, sz);
    }


    if (CURVE_VALUE_ALL == m_nShowXScaleType)
    {
        for(i = 1; i <= m_Curve.nCol; i ++)
        {
            memset(sz, 0, sizeof(sz));
            snprintf(sz, 100, szXFormat, fabs(m_dXUnit * i + m_dXMin));
            nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;


            if (m_rectLine.left + nXUnit * i + nStrXOffset > m_rectLine.right)
            {
                if (i == m_Curve.nCol)
                    nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 2;
                else
                    nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 4;
            }
            if (i == m_Curve.nCol)
                ShowExtentEx(m_rectLine.right - strlen(sz) * m_pcFont->m_wWidth / 2,
                            m_rectXAxesScale.top + 1, sz);
            else
                ShowExtentEx(m_rectLine.left + nXUnit * i - nStrXOffset,
                            m_rectXAxesScale.top + 1, sz);
            //printf("xScale[%d]=%s, x=%d\n", i, sz, m_rectLine.left + nXUnit * i - nStrOffset);
        }
    }
    else if (CURVE_VALUE_LIMIT == m_nShowXScaleType || CURVE_VALUE_MAXONLY == m_nShowXScaleType)
    {
        memset(sz, 0, sizeof(sz));
        snprintf(sz, 100, szXFormat, m_dXUnit * m_Curve.nCol + m_dXMin);
        nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
        if (m_rectLine.left + nXUnit * m_Curve.nCol - nStrXOffset > m_rectLine.right)
        {
            nStrOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 2;
        }
        ShowExtentEx(m_rectLine.left + nXUnit * m_Curve.nCol - nStrXOffset,
                        m_rectXAxesScale.top + 1, sz);
    }
}


//	2008/4/2 11:06�W�� fans moidfy
void	CtmCurveEx2::DrawYScale()
{

    double	dYMin = m_dYMin;
    double	dYMax = m_dYMax;
    double	dYExMin = m_dYExMin;
    double 	dYExMax = m_dYExMax;
    double	dYUnit= m_dYUnit;
    double 	dYExUnit= m_dYExUnit;

    //printf("YScale:: %lf,%lf<---->%lf,%lf\n",m_dYMin,m_dYMax,m_dYExMin,m_dYExMax);
    int nYUnit;
    int nStrXOffset, nStrYOffset;
    char	sz[100], szYFormat[10];

    memset(sz, 0, sizeof(sz));
    snprintf(szYFormat, 10, "%%.%df", m_Curve.nYDigits);
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
    snprintf(sz, 100, szYFormat, m_dYMin);
    nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
    nStrYOffset	= m_pcFont->m_wHeight;
    if (CURVE_VALUE_MAXONLY != m_nShowYScaleType && CURVE_VALUE_NOTHING != m_nShowYScaleType)
    {
        if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
            ShowExtentEx(m_rectYAxesScale.left,
                m_rectYAxesScale.bottom - nStrYOffset, sz);
        else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
            ShowExtentEx(m_rectYAxesScale.right - strlen(sz) * m_pcFont->m_wWidth / 2 - 1,
                            m_rectYAxesScale.bottom  - nStrYOffset, sz);
    }

    if (CURVE_VALUE_ALL == m_nShowYScaleType)
    {
        for(int i = 1; i <= m_Curve.nRow; i ++)
        {
            memset(sz, 0, sizeof(sz));
            snprintf(sz, 100, szYFormat, m_dYUnit * i + m_dYMin);
            nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
            nStrYOffset	= m_pcFont->m_wHeight / 2;
            if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScale.left,// - nStrXOffset,
                            m_rectYAxesScale.bottom - nYUnit * i - nStrYOffset, sz);
            else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScale.right - nStrXOffset,
                            m_rectYAxesScale.bottom - nYUnit * i - nStrYOffset, sz);

        }
    }
    else if (CURVE_VALUE_LIMIT == m_nShowYScaleType || CURVE_VALUE_MAXONLY == m_nShowYScaleType)
    {
        memset(sz, 0, sizeof(sz));
        snprintf(sz, 100, szYFormat, m_dYUnit * m_Curve.nRow + m_dYMin);
        nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
        nStrYOffset	= m_pcFont->m_wHeight / 2;
        if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
            ShowExtentEx(m_rectYAxesScale.left,// - nStrXOffset,
                            m_rectYAxesScale.bottom - nYUnit * m_Curve.nRow  - nStrYOffset, sz);
            else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScale.right - nStrXOffset,
                            m_rectYAxesScale.bottom - nYUnit * m_Curve.nRow  - nStrYOffset, sz);
     }

    if(m_bYNameEx)
    {
        int nYExUnit;
        int nStrXOffset, nStrYOffset;
        char	sz[100], szYFormat[10];

        memset(sz, 0, sizeof(sz));
        snprintf(szYFormat, 10, "%%.%df", m_Curve.nYExDigits);
        nYExUnit= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
        snprintf(sz, 100, szYFormat, m_dYExMin);
        nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
        nStrYOffset	= m_pcFont->m_wHeight;
        if (CURVE_VALUE_MAXONLY != m_nShowYScaleType && CURVE_VALUE_NOTHING != m_nShowYScaleType)
        {
            if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScaleEx.right - strlen(sz) * m_pcFont->m_wWidth / 2 - 1,
                                m_rectYAxesScaleEx.bottom  - nStrYOffset, sz);
            else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScaleEx.left,
                    m_rectYAxesScaleEx.bottom - nStrYOffset, sz);
        }

        if (CURVE_VALUE_ALL == m_nShowYScaleType)
        {
            for(int i = 1; i <= m_Curve.nRow; i ++)
            {
                memset(sz, 0, sizeof(sz));
                snprintf(sz, 100, szYFormat, m_dYExUnit * i + m_dYExMin);
                nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
                nStrYOffset	= m_pcFont->m_wHeight / 2;
                if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
                    ShowExtentEx(m_rectYAxesScaleEx.right - nStrXOffset,
                                m_rectYAxesScaleEx.bottom - nYExUnit * i - nStrYOffset, sz);
                else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                    ShowExtentEx(m_rectYAxesScaleEx.left,// - nStrXOffset,
                                m_rectYAxesScaleEx.bottom - nYExUnit * i - nStrYOffset, sz);

            }
        }
        else if (CURVE_VALUE_LIMIT == m_nShowYScaleType || CURVE_VALUE_MAXONLY == m_nShowYScaleType)
        {
            memset(sz, 0, sizeof(sz));
            snprintf(sz, 100, szYFormat, m_dYExUnit * m_Curve.nRow + m_dYExMin);
            nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 2 + 1;
            nStrYOffset	= m_pcFont->m_wHeight / 2;
            if (CURVE_XXPOS_LEFT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScaleEx.right - nStrXOffset,
                                m_rectYAxesScaleEx.bottom - nYExUnit * m_Curve.nRow  - nStrYOffset, sz);
            else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                ShowExtentEx(m_rectYAxesScaleEx.left,// - nStrXOffset,
                                m_rectYAxesScaleEx.bottom - nYExUnit * m_Curve.nRow  - nStrYOffset, sz);

         }
    }

}

void	CtmCurveEx2::DrawName()
{
    if (m_bXName) DrawXName();
    if (m_bYName) DrawYName();
}

void	CtmCurveEx2::DrawXName()
{
    int nStrXOffset = 0;
    int nStrYOffset = 0;
    if(m_Curve.pszXName != NULL)
    {
        nStrXOffset	= ((m_rectXName.right - m_rectXName.left)
                        - strlen(m_Curve.pszXName) * m_pcFont->m_wWidth / 2)
                        / 2;

        nStrYOffset	= m_pcFont->m_wHeight + 1;
        ShowExtentEx(m_rectXName.left + nStrXOffset,m_rectXName.top,
                    m_Curve.pszXName, m_pcFont->GetFontCode());
    }


}

void	CtmCurveEx2::DrawYName()
{

    if(m_Curve.pszYName != NULL)
    {
        int nStrXOffset = 0;
        int nStrYOffset = 0;
        char	sz[100];
        memset(sz,0,sizeof(sz));
        int nYStrLen  = 0;
        BYTE 	*pChar;
        WORD	*pCode;
        int		nStrLen		= 0;

        pChar	= (BYTE *)m_Curve.pszYName;
        while(*pChar != 0)
        {
            nStrLen	   += m_pcFont->m_wHeight;
            pChar +=2;
        }


        nStrYOffset	= ((m_rectYName.bottom - m_rectYName.top)
                        - nStrLen) / 2;
        pChar	= (BYTE *)m_Curve.pszYName;

        BOOL	bDoubleChar = FALSE;
        while(*pChar != 0)
        {
            sz[2]	=
            sz[1]	= 0;
            if(*pChar > 0xA0)
            {
                pCode	= (WORD *)pChar;
                nStrXOffset	= m_pcFont->m_wWidth + nYStrLen;
                sz[0]	= *pChar;
                sz[1]	= *(pChar + 1);
                pChar += 2;
            }
            else
            {
                nStrXOffset = m_pcFont->m_wWidth  + nYStrLen;
                sz[0]	= *pChar;
                sz[1]	= *(pChar + 1);
                pChar +=2;
            }

            if (CURVE_XXPOS_LEFT== m_nShowYAxesType)
                ShowExtentEx( m_rectYName.left,
                            m_rectYName.top + nStrYOffset, sz, m_pcFont->GetFontCode());
            else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                //ShowExtentEx( m_rectYName.right - m_pcFont->m_wWidth / 2,
            {
                if (bDoubleChar)
                    ShowExtentEx( m_rectYName.right - m_pcFont->m_iCharLen,
                                m_rectYName.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
                else
                    ShowExtentEx( m_rectYName.right - m_pcFont->m_iCharLen / 2,
                                m_rectYName.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
            }

            nStrYOffset += m_pcFont->m_wHeight;
        }
    }
    if(m_bYNameEx)
    {
        if(m_Curve.pszYNameEx !=NULL)
        {

            int nStrXOffset = 0;
            int nStrYOffset = 0;
            char	sz[100];
            int nYStrLen  = 0;
            BYTE 	*pChar;
            WORD	*pCode;
            int		nStrLen		= 0;

            pChar	= (BYTE *)m_Curve.pszYNameEx;
            while(*pChar != 0)
            {
                nStrLen	   += m_pcFont->m_wHeight;
                pChar +=2;
            }


            nStrYOffset	= ((m_rectYNameEx.bottom - m_rectYNameEx.top)
                            - nStrLen) / 2;
            pChar	= (BYTE *)m_Curve.pszYNameEx;

            BOOL	bDoubleChar = FALSE;
            while(*pChar != 0)
            {
                sz[2]	=
                sz[1]	= 0;
                if(*pChar > 0xA0)
                {
                    pCode	= (WORD *)pChar;
                    nStrXOffset	= m_pcFont->m_wWidth + nYStrLen;
                    sz[0]	= *pChar;
                    sz[1]	= *(pChar + 1);
                    pChar += 2;
                }
                else
                {
                    nStrXOffset = m_pcFont->m_wWidth  + nYStrLen;
                    sz[0]	= *pChar;
                    sz[1]	= *(pChar + 1);
                    pChar +=2;
                }

                if (CURVE_XXPOS_LEFT== m_nShowYAxesType)
                {
                    if (bDoubleChar)
                        ShowExtentEx( m_rectYNameEx.right - m_pcFont->m_iCharLen,
                                    m_rectYNameEx.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
                    else
                        ShowExtentEx( m_rectYNameEx.right - m_pcFont->m_iCharLen / 2,
                                    m_rectYNameEx.top + nStrYOffset,	sz, m_pcFont->GetFontCode());
                }
                else if (CURVE_XXPOS_RIGHT == m_nShowYAxesType)
                    //ShowExtentEx( m_rectYName.right - m_pcFont->m_wWidth / 2,
                {
                    ShowExtentEx( m_rectYNameEx.left,
                                m_rectYNameEx.top + nStrYOffset, sz, m_pcFont->GetFontCode());
                }

                nStrYOffset += m_pcFont->m_wHeight;
            }
        }
    }
}

void	CtmCurveEx2::DrawCoordinate()
{
    //=======
    // �e�I��
    //=======
    if (m_bShowGrid)
        DrawAxesGrid();
    //=======
    // �e���жb
    //=======
    DrawAxes();
    //=======
    // ��ܧ��жb���
    //=======
    DrawSclae();
    //=======
    // ��ܧ��жb�W��
    //=======
    DrawName();
}

void	CtmCurveEx2::DrawCurveLine(LINE *pLine)
{
    int		i, nBottom;
    int 	nXUnit, nYUnit;

    int nXPos = 0;
    int nYPos = 0;

    //printf("1:draw lines ...\n");
    nBottom	= m_rectLine.bottom - m_rectLine.top;
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;

    if (0 == pLine->nRefernce)
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(pLine->dwColor);

            //printf("%s xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);
            nXPos = m_rectLine.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPos = nBottom - (int)((pLine->pValue[0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
            m_pDoubleBuffer->MoveToInBuffer(nXPos, nYPos);
            //printf("start(%d, %d), count:%d\n", nXPos, nYPos, pLine->nCount);
            for(i = 0; i< pLine->nCount; i ++)
            {
                nXPos = m_rectLine.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit);
                nYPos = nBottom - (int)((pLine->pValue[i].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
                m_pDoubleBuffer->DrawLineToInBuffer(nXPos, nYPos);
            }
        }
        else
        {
            _SetColor(pLine->dwColor);


            nXPos = m_rectLine.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPos = nBottom - (int)((pLine->pValue[0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
            _MoveTo(nXPos, nYPos);
            for(i = 0; i< pLine->nCount; i ++)
            {
                nXPos = m_rectLine.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit);
                nYPos = nBottom - (int)((pLine->pValue[i].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
                _LineTo(nXPos, nYPos);
            }
        }
    }
    else
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(pLine->dwColor);

            //printf("%s xmin:%f, xmax:%f\n", Name, m_dXMin, m_dXMax);
            nXPos = m_rectLine.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPos = nBottom - (int)((pLine->pValue[0].dY - m_dYExMin)/ m_dYExUnit * nYUnit) + m_rectLine.top;
            m_pDoubleBuffer->MoveToInBuffer(nXPos, nYPos);
            //printf("start(%d, %d), count:%d\n", nXPos, nYPos, pLine->nCount);
            for(i = 0; i< pLine->nCount; i ++)
            {
                nXPos = m_rectLine.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit);
                nYPos = nBottom - (int)((pLine->pValue[i].dY - m_dYExMin)/ m_dYExUnit * nYUnit) + m_rectLine.top;
                m_pDoubleBuffer->DrawLineToInBuffer(nXPos, nYPos);
            }
        }
        else
        {
            _SetColor(pLine->dwColor);


            nXPos = m_rectLine.left + (int)((pLine->pValue[0].dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPos = nBottom - (int)((pLine->pValue[0].dY - m_dYExMin)/ m_dYExUnit * nYUnit) + m_rectLine.top;
            _MoveTo(nXPos, nYPos);
            for(i = 0; i< pLine->nCount; i ++)
            {
                nXPos = m_rectLine.left + (int)((pLine->pValue[i].dX - m_dXMin) / m_dXUnit * nXUnit);
                nYPos = nBottom - (int)((pLine->pValue[i].dY - m_dYExMin)/ m_dYExUnit * nYUnit) + m_rectLine.top;
                _LineTo(nXPos, nYPos);
            }
        }

    }
}


void	CtmCurveEx2::DrawCurveLine(LINE *pLine, int nLineIndex)
{
    int		i, nHeight;
    int 	nXUnit, nYUnit;
    tmPoint point1, point2;
    tmPoint *points[2];
    memset(&point1,0,sizeof(point1));
    memset(&point2,0,sizeof(point2));
    int		nOpMode;


    //printf("2:draw lines ...\n");
    //printf("1 %s m_dXMin:%f, m_dXMax:%f\n", Name, m_dXMin, m_dXMax);

    points[0] = &point1;
    points[1] = &point2;

    nHeight	= m_rectLine.bottom - m_rectLine.top;
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;


    if (m_pDoubleBuffer != NULL)
    {
        m_pDoubleBuffer->SetColorInBuffer(pLine->dwColor);

        //printf("pLine->nCount=%d \n", pLine->nCount);
        for (i = 0; i < pLine->nCount - 1; i++)
        {
            //printf("pLine->pValue[0].dX=%lf,pLine->pValue[0].dY=%lf\n",pLine->pValue[0].dX,pLine->pValue[0].dY);
            //printf("pLine->pValue[1].dX=%lf,pLine->pValue[1].dY=%lf\n",pLine->pValue[1].dX,pLine->pValue[1].dY);
            nOpMode = GenerateEndPoints(nLineIndex, &(pLine->pValue[i]), &(pLine->pValue[i + 1]), points, 2);
            if (nOpMode & DRAW_CURVE_IGNORE_POINT)
                continue;

            if (nOpMode & DRAW_CURVE_LINK_POINTS)
            {
                //printf("move to:%d (%d, %d)\n", i, point1.ix, point1.iy);
                m_pDoubleBuffer->MoveToInBuffer(point1.ix, point1.iy);
                //printf("draw to: %d (%d, %d)\n", i + 1, point2.ix, point2.iy);
                m_pDoubleBuffer->DrawLineToInBuffer(point2.ix, point2.iy);
            }
        }
    }


}


void	CtmCurveEx2::DrawLines()
{

    for(int i = 0; i < CURVE_LINE_MAX; i ++)
    {
        if (m_pLine[i].pValue == NULL) continue;

        if (!m_bTouchSupport)
            DrawCurveLine(&m_pLine[i]);
        else
            DrawCurveLine(&m_pLine[i], i);
    }

    DrawBufferLine();
}

void	CtmCurveEx2::Show()
{
    PropareShow();
    if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
    {
        if(!(m_Curve.nFlag & CURVE_NOTDRAW))
        {
            //=======
            // �p��X���жb���S��
            //=======
            if (m_nOpMode == CURVE_OPERATION_NONE)
                GetRangeXY();
            SeperateArea();
            //	show title
            ShowTitle();
            // draw cutline
           	if(m_nShowCutLineType)
            	DrawCutLine();
            //========
            // draw coordinate
            //========
            DrawCoordinate();
            //=======
            // lines
            //=======
            DrawLines();

            //=======
            // lines
            //=======
            DrawRuleMark();

            //DrawRuleMark(m_nLineIndex);
            if(m_monitor_attr.flag)
				DrawMonitorArea();
            //	double buffer
            if (m_pDoubleBuffer != NULL)
                m_pDoubleBuffer->DrawWithDoubleBuffer();
        }

        CtmWnd::Show();

    }

}



void	CtmCurveEx2::Update()
{
	if(CheckMonitorCoordinateChange()||m_bPropertyChanged)
	{
    	Show();
    	m_bPropertyChanged=FALSE;
    }
}


BOOL	CtmCurveEx2::FindRelationFirst(tmRectRelation* pSource, tmRectRelation** pDest)
{
    BOOL bResult = FALSE;

    return bResult;

}

BOOL	CtmCurveEx2::FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest)
{
    BOOL bResult = FALSE;
    //tmRectRelation  Temp;
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

RECT	CtmCurveEx2::FindLast(tmRectRelation* pSource)
{
    RECT Result;
    memset(&Result, 0, sizeof(Result));
    tmRectRelation* TempRelation;

    if (FindRelationEnd(pSource, &TempRelation))
        Result = *TempRelation->pRect;

    return Result;
}

int		CtmCurveEx2::GetLastLeft()
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

int		CtmCurveEx2::GetLastRight()
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


int		CtmCurveEx2::GetLastTop()
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

int		CtmCurveEx2::GetLastBottom()
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

BOOL	CtmCurveEx2::AddTopRelation(tmRectRelation* pSource, int nHeight)
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

BOOL	CtmCurveEx2::AddDownRelation(tmRectRelation* pSource, int nHeight)
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

BOOL	CtmCurveEx2::AddLeftRelation(tmRectRelation* pSource, int nWidth)
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

BOOL	CtmCurveEx2::AddRightRelation(tmRectRelation* pSource, int nWidth)
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

int		CtmCurveEx2::DrawBufferLine()
{
    int result = 0;
    int	nBottom;
    int nXUnit, nYUnit;
    int	nXPos, nYPos;

    nBottom	= m_rectLine.bottom - m_rectLine.top;
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;


    if (m_pDoubleBuffer != NULL)
    {
        for (int i = 0; i < CURVE_LINE_MAX; i++)
        {
            for (int j = 0; j < m_BufferLine[i]->nLineCount; j++)
            {
                if (m_BufferLine[i]->nCount[j] > 0)
                {
                    //DrawCurveLine(m_BufferLine[i]->pValue);
                    //	fans add 2008/4/2 11:30�W��

                    if (0 == m_BufferLine[i]->nRefernce)
                    {
                        nXPos = m_rectLine.left + (int)((m_BufferLine[i]->pValue[j][0].dX - m_dXMin) / m_dXUnit * nXUnit);
                        nYPos = nBottom - (int)((m_BufferLine[i]->pValue[j][0].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;

                        m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
                        m_pDoubleBuffer->MoveToInBuffer(nXPos, nYPos);

                        for (int k = 0; k < m_BufferLine[i]->nCount[j]; k++)
                        {
                            nXPos = m_rectLine.left + (int)((m_BufferLine[i]->pValue[j][k].dX - m_dXMin) / m_dXUnit * nXUnit);

                            nYPos = nBottom - (int)((m_BufferLine[i]->pValue[j][k].dY - m_dYMin)/ m_dYUnit * nYUnit) + m_rectLine.top;
                            m_pDoubleBuffer->DrawLineToInBuffer(nXPos, nYPos);
                        }
                    }
                    else if (1 == m_BufferLine[i]->nRefernce)
                    {
                        nXPos = m_rectLine.left + (int)((m_BufferLine[i]->pValue[j][0].dX - m_dXMin) / m_dXUnit * nXUnit);
                        nYPos = nBottom - (int)((m_BufferLine[i]->pValue[j][0].dY - m_dYExMin)/ m_dYExUnit * nYUnit) + m_rectLine.top;

                        m_pDoubleBuffer->SetColorInBuffer(m_pLine[i].dwColor);
                        m_pDoubleBuffer->MoveToInBuffer(nXPos, nYPos);

                        for (int k = 0; k < m_BufferLine[i]->nCount[j]; k++)
                        {
                            nXPos = m_rectLine.left + (int)((m_BufferLine[i]->pValue[j][k].dX - m_dXMin) / m_dXUnit * nXUnit);

                            nYPos = nBottom - (int)((m_BufferLine[i]->pValue[j][k].dY - m_dYExMin)/ m_dYExUnit * nYUnit) + m_rectLine.top;
                            m_pDoubleBuffer->DrawLineToInBuffer(nXPos, nYPos);
                        }
                    }

                }
            }
        }
    }


    return result;
}


void	CtmCurveEx2::SetShowFunc(pFuncShowRuleStr pFunc)
{
    m_pFuncYStr = pFunc;
}

int		CtmCurveEx2::GetIndexBaseOnX(CURVEPOINT 	*pValue, int iCount, double dX)
{
    m_dfPercent	= 0;
    if (iCount <= 0) return -1;
    else
    {
        int nLow 	= 0;
        int nHigh 	= iCount - 1;
        int nMid 	= 0;
        nMid = (nLow + nHigh) / 2;
        BOOL	bPercent = FALSE;
        if (pValue[nLow].dX < pValue[nHigh].dX)
        {

            if (dX < pValue[nLow].dX)	return -1;
            if (dX > pValue[nHigh].dX) 	return -1;
            while(nLow < nHigh)
            {
                if 		(pValue[nMid].dX == dX)  break;
                else if (pValue[nMid].dX <	dX)
                {
                    nLow = nMid + 1;
                    if (pValue[nLow].dX > dX)
                    {
                        bPercent = TRUE;
                        break;
                    }
                }
                else if (pValue[nMid].dX > dX)
                {
                    nHigh 	= nMid - 1;
                    if (pValue[nHigh].dX < dX)
                    {
                        nMid = nHigh;
                        bPercent = TRUE;
                        break;
                    }
                }
                nMid = (nLow + nHigh) / 2;
            }
        }
        else
        {
            if (dX > pValue[nLow].dX)	return -1;
            if (dX < pValue[nHigh].dX) 	return -1;
            while(nLow < nHigh)
            {
                if 		(pValue[nMid].dX == dX)  break;

                else if (pValue[nMid].dX <	dX)
                {

                    nHigh 	= nMid - 1;
                    if (pValue[nHigh].dX > dX)
                    {
                        nMid = nHigh;
                        bPercent = TRUE;
                        break;
                    }
                }
                else if (pValue[nMid].dX > dX)
                {
                    nLow = nMid + 1;
                    if (pValue[nLow].dX < dX)
                    {
                        bPercent = TRUE;
                        break;
                    }
                }
                nMid = (nLow + nHigh) / 2;
            }
        }


        if (bPercent &&
            (nMid < iCount - 1) && 								//	can't overflow
                ABS(pValue[nMid + 1].dX - pValue[nMid].dX) >= 0.0001)
            {
                m_dfPercent = (dX - pValue[nMid].dX) / (ABS(pValue[nMid + 1].dX - pValue[nMid].dX));
            }

        g_tmDebugInfo->PrintDebugInfo(" Mid = %d\n", nMid);
        return nMid;
    }
}



void	CtmCurveEx2::GetXPosValue()
{
    int	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    m_dXPosValue	= (double)m_xPos / (double)nXUnit * m_dXUnit  + m_dXMin;
	m_bPropertyChanged=TRUE;
    g_tmDebugInfo->PrintDebugInfo("GetXPosValue() = %f;XUnit=%d, dxUnit=%f, Min=%f\n",
                                    m_dXPosValue, nXUnit, m_dXUnit, m_dXMin);
}

void    CtmCurveEx2::OnGetFocus()
{
    m_bOnFocus = TRUE;
    GetXPosValue();
    m_xPerMove=1;
    Update();

    if (m_pszPrompt != NULL)
        Prompt(m_pszPrompt);
}

void    CtmCurveEx2::OnLoseFocus()
{
    m_bOnFocus = FALSE;
    //Update();
}

WORD    CtmCurveEx2::OnKey(WORD wKey)
{
    WORD	wResult = _NULL_KEY;
    BOOL	bUpdate = TRUE;
    switch (wKey)
    {

        case _PLUS:
            StretchCurve(-0.1);
            break;
        case _MINUS:
            StretchCurve(0.1);
            break;
        case _DOT:
            ResetCoordinate();
            break;
//        case '5':
//        	if(m_xPerMove<5*m_xMoveInterval)
//        	m_xPerMove+=m_xMoveInterval;
//        	break;
        case '3':
        //case 27136:

            m_xPos += m_xPerMove;
            if (m_xPos > m_rectLine.right - m_rectLine.left)
                m_xPos =0;// m_rectLine.right - m_rectLine.left;
	#if	!defined(D_X86)||defined(D_ATOM)
	if(m_xPerMove<5*m_xMoveInterval)
        	m_xPerMove+=m_xMoveInterval;
        #endif
            GetXPosValue();
            Update();
            wResult = _NULL_KEY;
            break;
//        case '0':
//        	if(m_xPerMove>m_xMoveInterval)
//        	m_xPerMove-=m_xMoveInterval;
//        	break;
        case '1':
        //case 26880:
            m_xPos -= m_xPerMove;
            if (m_xPos < 0)
                m_xPos = m_rectLine.right - m_rectLine.left;
            #if	!defined(D_X86)||defined(D_ATOM)
            if(m_xPerMove<5*m_xMoveInterval)
        	m_xPerMove+=m_xMoveInterval;
            #endif
            GetXPosValue();
            Update();
            wResult = _NULL_KEY;
            break;
        case 0x9492:
        case 0x9490:
           m_xPerMove=1;
           wResult=_NULL_KEY;
           break;
        default:
            wResult = wKey;
            bUpdate = FALSE;
            break;

    }
    if (bUpdate)
        Update();
    return wResult;

}


void	CtmCurveEx2::DrawRuleMark()
{
    if (m_bOnFocus)
    {
        DrawRuleLine();

        if (m_pFuncYStr != NULL)
        {
            m_pFuncYStr(m_dXPosValue, m_szYRule, &m_dfPercent);
//            ShowYRuleStr();
        }
        if(!m_bRuleMark)
        	ComposeYRuleStrDef();
        	
        else
            ShowYRuleStr();
       
    }
}


void	CtmCurveEx2::DrawRuleMark(int nLineIndex)
{
    int ret 	= 0;
    int i 		= 0;
    tmPoint	point;

    if (!m_bOnFocus || nLineIndex >= CURVE_LINE_MAX || m_pLine[nLineIndex].nCount == 0)
        return;

    if (GetRuleType() & RULE_HORIZONTAL)		//�����Ф�
    {
        if (GetPointLocation(&point) >= 0)
            m_nHRulePos = point.iy;


        if (!(GetRuleType() & RULE_NO_SHOW))
            DrawHRuleLine();
    }
    else if (GetRuleType() & RULE_VERTICAL)		//�����Ф�
    {
        if (GetPointLocation(&point) >= 0)
            m_nVRulePos = point.ix;

        if (!(GetRuleType() & RULE_NO_SHOW))
            DrawVRuleLine();
    }
    else
        ;

}


void	CtmCurveEx2::DrawHRuleLine()
{
    tmPoint point;

    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::DrawHRuleLine()\n");

    if (m_Curve.nFlag & CURVE_RULE_REALLINE)
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_XRuleColor);
            GetPointLocation(&point);

            m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR,
                            m_rectLine.left + 1,
                            m_rectLine.top + point.iy - 1,
                            m_rectLine.left + point.ix - 1,
                            m_rectLine.top + point.iy + 1
                          );
        }
        else
        {
            _SetColor(m_XRuleColor);
            _MoveTo(m_rectLine.left, m_rectLine.top + m_nHRulePos);
            _LineTo(m_rectLine.right, m_rectLine.top + m_nHRulePos);
        }
    }
    else
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_XRuleColor);
            for (int i = 0; i <= (m_rectLine.right - m_rectLine.left) / 4; i++)
            {
                m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + i * 4, m_rectLine.top + m_nHRulePos);
                m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + i * 4 + 2, m_rectLine.top + m_nHRulePos);
            }
            //m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_yPos + m_rectLine.top);
        }
        else
        {
            _SetColor(m_XRuleColor);

            for (int i = 0; i <= (m_rectLine.right - m_rectLine.left) / 4; i++)
            {
                _MoveTo(m_rectLine.left + i * 4, m_rectLine.top + m_nHRulePos);
                _LineTo(m_rectLine.left + i * 4 + 2, m_rectLine.top + m_nHRulePos);
            }
            //_LineTo(m_rectLine.right, m_nRuleYPos);
        }
    }
}


void	CtmCurveEx2::DrawVRuleLine()
{
    tmPoint point;

    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::DrawYRuleLine()\n");

    if (m_Curve.nFlag & CURVE_RULE_REALLINE)
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
            GetPointLocation(&point);

            m_pDoubleBuffer->DrawRectangleInBuffer(	_GFILLINTERIOR,
                            m_rectLine.left + point.ix - 1,
                            m_rectLine.top + point.iy,
                            m_rectLine.left + point.ix + 1,
                            m_rectLine.bottom
                          );
        }
        else
        {
            _SetColor(m_YRuleColor);
            _MoveTo(m_nVRulePos + m_rectLine.left, m_rectLine.top);
            _LineTo(m_nVRulePos + m_rectLine.left, m_rectLine.bottom);
        }
    }
    else
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
            for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
            {
                m_pDoubleBuffer->MoveToInBuffer(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4);
                m_pDoubleBuffer->DrawLineToInBuffer(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4 + 2);
            }
            m_pDoubleBuffer->DrawLineToInBuffer(m_nVRulePos + m_rectLine.left, m_rectLine.bottom);
        }
        else
        {
            _SetColor(m_YRuleColor);

            for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
            {
                _MoveTo(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4);
                _LineTo(m_nVRulePos + m_rectLine.left, m_rectLine.top + i * 4 + 2);
            }
            _LineTo(m_nVRulePos + m_rectLine.left, m_rectLine.bottom);
        }
    }
}

int		CtmCurveEx2::GetPointLocation(tmPoint *point, int nLineIndex, int nPointIndex)
{
    int nXUnit, nYUnit, nHeight;
    int ix, iy;

    if (nLineIndex < 0)
        nLineIndex = m_nLineIndex;
    if (nPointIndex < 0)
        nPointIndex = m_nPointIndex;

    if (nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX || m_pLine[nLineIndex].pValue == NULL ||
         nPointIndex < 0 || nPointIndex >= m_pLine[nLineIndex].nCount || point == NULL)
        return -1;

    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    ix		= (int)((m_pLine[nLineIndex].pValue[nPointIndex].dX - m_dXMin) / m_dXUnit * nXUnit);
    nHeight	 = m_rectLine.bottom - m_rectLine.top;
    nYUnit	 = (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;

    if (0 == m_pLine[nLineIndex].nRefernce)
    {

        iy		 = nHeight - (int)((m_pLine[nLineIndex].pValue[nPointIndex].dY - m_dYMin)/ m_dYUnit * nYUnit);
    }
    else
    {

        iy		 = nHeight - (int)((m_pLine[nLineIndex].pValue[nPointIndex].dY - m_dYExMin)/ m_dYExUnit * nYUnit);
    }




    point->ix = ix;
    point->iy = iy;

    return 0;
}

int		CtmCurveEx2::FindNextPoint(int nBaseIndex, int nOffset)
{
    if (m_pLine[m_nLineIndex].nCount <= 0)
        return -1;

    //if (m_Curve.nFlag & CURVE_XCONTRARY)
    nOffset = -nOffset;

    m_nPointIndex = (m_pLine[m_nLineIndex].nCount + nBaseIndex + nOffset)  //nOffset can be negative
                        % m_pLine[m_nLineIndex].nCount;
    if (m_nPointIndex < 0)
        m_nPointIndex += m_pLine[m_nLineIndex].nCount;

    return m_nPointIndex;
}


void	CtmCurveEx2::DrawRuleLine()
{
    //	draw the y line
    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::DrawRuleLine()\n");


    if (m_Curve.nFlag & CURVE_RULE_REALLINE)
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
            m_pDoubleBuffer->MoveToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top);
            m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.bottom);
        }
        else
        {
            _SetColor(m_YRuleColor);
            _MoveTo(m_xPos + m_rectLine.left, m_rectLine.top);
            _LineTo(m_xPos + m_rectLine.left, m_rectLine.bottom);
        }
    }
    else
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_YRuleColor);
            for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
            {
                m_pDoubleBuffer->MoveToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top + i * 4);
                m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.top + i * 4 + 1);
            }
            m_pDoubleBuffer->DrawLineToInBuffer(m_xPos + m_rectLine.left, m_rectLine.bottom);
        }
        else
        {
            _SetColor(m_YRuleColor);

            for (int i = 0; i < (m_rectLine.bottom - m_rectLine.top) / 4; i++)
            {
                _MoveTo(m_xPos + m_rectLine.left, m_rectLine.top + i * 4);
                _LineTo(m_xPos + m_rectLine.left, m_rectLine.top + i * 4 + 1);
            }
            _LineTo(m_xPos + m_rectLine.left, m_rectLine.bottom);
        }
    }
}

int		CtmCurveEx2::ComposeYRuleStrDef()
{
    int nIndex = 0;
    char	szFormat[64];
    char	szTempY[256];
    sprintf(szFormat, "%%.%df",  m_Curve.nXDigits);
    sprintf(szTempY, szFormat, m_dXPosValue);
    memset(m_szYRule, 0, sizeof(m_szYRule));
    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::ComposeYRuleStrDef() %s \n", szTempY);



    int		nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
 //   BOOL	bGetYPos = FALSE;
    double	dY		= 0;
    m_yPos=0;
    int nInterval	= (m_rectLine.bottom - m_rectLine.top)/CURVE_LINE_MAX;
    for (int i = 0; i < CURVE_LINE_MAX; i++)
    {
    	 sprintf(szFormat, "%%.%df",  m_Curve.nXDigits);
   	 sprintf(szTempY, szFormat, m_dXPosValue);
    	 memset(m_szYRule, 0, sizeof(m_szYRule));
        nIndex = GetIndexBaseOnX(m_pLine[i].pValue, m_pLine[i].nCount, m_dXPosValue);
        if (nIndex >= 0)
        {
            double	dTempMin, dTempMax, dTempUnit = 0;
            if (0 == m_pLine[i].nRefernce)
            {
                sprintf(szFormat, "%%s,%%.%df",  m_Curve.nYDigits);
                dTempMin = m_dYMin;
                dTempMax = m_dYMax;
            }
            else
            {
                sprintf(szFormat, "%%s,%%.%df",  m_Curve.nYExDigits);
                dTempMin = m_dYExMin;
                dTempMax = m_dYExMax;
            }
            dTempUnit = (dTempMax - dTempMin) / m_Curve.nRow;

            dY = m_pLine[i].pValue[nIndex].dY;
            if (nIndex < m_pLine[i].nCount - 1)
                dY += (m_pLine[i].pValue[nIndex + 1].dY - m_pLine[i].pValue[nIndex].dY) * m_dfPercent;


            sprintf(szTempY, szFormat, szTempY, dY);
 //           if (!bGetYPos)
            {
                m_yPos +=nInterval; /*(int)((dY- dTempMin)/ dTempUnit * nYUnit);*/
//                bGetYPos = TRUE;
            }
	  sprintf(m_szYRule, "<%s>", szTempY);
    	  int nOldColor=m_fgc;
    	  m_fgc=m_pLine[i].dwColor;
    	  ShowYRuleStr();
    	  m_fgc=nOldColor;
        }
          
    }
  
    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::ComposeYRuleStrDef2() %s \n", m_szYRule);
    return 0;
}

int		CtmCurveEx2::ComposeXRuleStrDef()
{
    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::ComposeXRuleStrDef()\n");
    return 0;
}

int		CtmCurveEx2::ShowYRuleStr()
{
    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::ShowYRuleStr()\n");
    if (m_szYRule[0] != '\0')
    {
        char	szTemp[256];
        memcpy(szTemp, m_szYRule, sizeof(m_szYRule));
        _tm_chartoword((WORD*)m_szYRule, szTemp);
        int nXPos, nYPos = 0;
        if ((int)(m_xPos + m_rectLine.left + strlen(szTemp) * m_pcFont->m_wWidth / 2) > m_rectLine.right)
            nXPos = m_xPos + m_rectLine.left - strlen(szTemp) * m_pcFont->m_wWidth / 2;
        else
            nXPos = m_xPos + m_rectLine.left;

        //if (m_yPos < m_pcFont->m_wHeight)
        nYPos =  m_rectLine.bottom - m_yPos - m_pcFont->m_wHeight;
        if (nYPos >= m_rectLine.bottom)
            nYPos -= m_pcFont->m_wHeight;

        if (nYPos <= m_rectLine.top)
            nYPos += m_pcFont->m_wHeight;
        ShowExtentD(nXPos, nYPos,m_szYRule, m_pcFont->GetFontCode());
    }
    return 0;
}

int		CtmCurveEx2::ShowXRuleStr()
{
    g_tmDebugInfo->PrintDebugInfo("CtmCurveEx2::ShowXRuleStr()\n");
    return 0;
}




int		CtmCurveEx2::GetIndex(char *pszDBID, int nOffset)
{
    WORD	wPointStartValueID, wPointIDIndex;

    if (pszDBID == NULL || nOffset < 0)
        return -1;
    if (g_pDatabase != NULL)
    {
        wPointStartValueID 	= g_pDatabase->GetDataID(pszDBID);
        wPointIDIndex 		= g_pDatabase->DataIDToIndex(wPointStartValueID + nOffset);
    }
    return wPointIDIndex;
}


BOOL	CtmCurveEx2::CheckValid(int nLineIndex, int nPointIndex)
{
    if (nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX
        || nPointIndex < 0 || nPointIndex >= m_nMaxPoint)  //James modify MAX_POINT_NUM-->m_nMaxPoint 2015/1/19 16:23:58
        return FALSE;
    else
        return TRUE;
}

/*
 * @pPoint: �ھڧ��Эȭp��o�쪺�����I�A��^���եΪ̡C
 * @nLineIndex: ���u���s��
 * @dX: ���Ъ�X��
 * @dY: ���Ъ�Y��
 * @Return: �p������쪺�����I
 *
 * �ھڧ��Эȭp����������I.
 */
tmPoint*	CtmCurveEx2::GetPointPos(tmPoint *pPoint, int nLineIndex, double dX, double dY)
{
    int nXUnit, nYUnit, nHeight;
    int ix, iy;


    nHeight	 = m_rectLine.bottom - m_rectLine.top;
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	 = (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;

    if (pPoint == NULL || nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX)
        return NULL;

    if (0 == m_pLine[nLineIndex].nRefernce)
    {
        ix		= m_rectLine.left + (int)((dX - m_dXMin) / m_dXUnit * nXUnit);
        iy		= nHeight - (int)((dY - m_dYMin) / m_dYUnit * nYUnit) + m_rectLine.top;
    }
    else
    {
        ix		= m_rectLine.left + (int)((dX - m_dXMin) / m_dXUnit * nXUnit);
        iy		= nHeight - (int)((dY - m_dYExMin) / m_dYExUnit * nYUnit) + m_rectLine.top;
    }





    pPoint->ix = ix;
    pPoint->iy = iy;

    return pPoint;
}

/*
 * �ھڦ��u�W�I��nPointIndex��������I�����Э�
 *
 */
tmPoint*	CtmCurveEx2::GetPointPos(tmPoint *pPoint, int nLineIndex, int nPointIndex)
{
    //int nXUnit, nYUnit, nHeight;
    //int ix, iy;

    if (pPoint == NULL || !CheckValid(nLineIndex, nPointIndex))
        return NULL;

    return GetPointPos(pPoint, nLineIndex,
                        m_pLine[nLineIndex].pValue[nPointIndex].dX,
                        m_pLine[nLineIndex].pValue[nPointIndex].dY);
}

/*
 * @pValue: �ھڧ����I�p��o�쪺���Э�,��^���եΪ�
 * @pPoint: �����I
 * �ھڧ���
 *
 */
CURVEPOINT	*CtmCurveEx2::GetPointValue(CURVEPOINT *pValue, tmPoint *pPoint, int nLineIndex)
{
    int nXUnit, nYUnit;

    if (pPoint == NULL || pValue == NULL)
        return NULL;

    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;

    pValue->dX = (double)pPoint->ix / (double)nXUnit * m_dXUnit  + m_dXMin;
    if(0 == m_pLine[nLineIndex].nRefernce)
        pValue->dY = m_dYMax - (double)pPoint->iy / (double)nYUnit * m_dYUnit;
    else
        pValue->dY = m_dYExMax - (double)pPoint->iy / (double)nYUnit * m_dYExUnit;

    return pValue;
}

/*
 * �N�����ܤƶq�ഫ�����ЭȪ��ܤƶq�C
 *
 */
void	CtmCurveEx2::CalcPointValueDelta()
{
    int nXUnit, nYUnit;

    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;

    m_dXValueDelta = (double)m_nXPosDelta / (double)nXUnit * m_dXUnit;
    //���Ե�һ��Y��Ϊ�ο���׼
    m_dYValueDelta = (double)m_nYPosDelta / (double)nYUnit * m_dYUnit;
}

int CtmCurveEx2::GenerateEndPoints(int nLineIndex, CURVEPOINT *p1, CURVEPOINT *p2,
                                                    tmPoint *pPoints[], int pNum)
{
    double dSlope, dIntercept, dCross[4];
    int nOpMode, nCrossNum;
    BOOL	bIn1, bIn2;
    bIn1 = bIn2 = FALSE;
    double dXMin, dXMax, dYMin, dYMax;

    if (nLineIndex < 0 || nLineIndex >= CURVE_LINE_MAX || p1 == NULL || p2 == NULL || pNum < 2)
        return DRAW_CURVE_IGNORE_POINT;

    dXMin = fmin(m_dXMin, m_dXMax);
    dXMax = fmax(m_dXMin, m_dXMax);
    if (0 == m_pLine[nLineIndex].nRefernce)
    {
        dYMin = fmin(m_dYMin, m_dYMax);
        dYMax = fmax(m_dYMin, m_dYMax);
    }
    else
    {
        dYMin = fmin(m_dYExMin, m_dYExMax);
        dYMax = fmax(m_dYExMin, m_dYExMax);
    }

    nCrossNum = 0;
    nOpMode = DRAW_CURVE_LINK_POINTS;

    if (p1->dX >= dXMin && p1->dX <= dXMax
        && p1->dY >= dYMin && p1->dY <= dYMax)
        bIn1 = TRUE;

    if (p2->dX >= dXMin && p2->dX <= dXMax
        && p2->dY >= dYMin && p2->dY <= dYMax)
        bIn2 = TRUE;

    if (bIn1 && bIn2)
    {
        //printf("p1(%f, %f), p2(%f, %f)\n", p1->dX, p1->dY, p2->dX, p2->dY);
        GetPointPos(pPoints[0], nLineIndex, p1->dX, p1->dY);
        GetPointPos(pPoints[1], nLineIndex, p2->dX, p2->dY);
        return nOpMode;
    }

    if (p1->dX == p2->dX)			//vertical line
    {
        if (p1->dX < dXMin || p1->dX > dXMax) // out of the coordinate
            return DRAW_CURVE_IGNORE_POINT;

        if (dYMin >= min(p1->dY, p2->dY) && dYMin <= max(p1->dY, p2->dY)) // one point below the coordinate
        {
            dCross[nCrossNum] = dYMin;
            GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, dCross[nCrossNum]);

            //printf("p1[%d, %d], cross:%f\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy, dCross[nCrossNum]);
            nCrossNum++;

        }
        if (dYMax >= min(p1->dY, p2->dY) && dYMax <= max(p1->dY, p2->dY)) //one point above the coordinate
        {
            dCross[nCrossNum] = dYMax;
            GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, dCross[nCrossNum]);

            //printf("p2[%d, %d]\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy);
            nCrossNum++;
        }

        if (nCrossNum == 0) // both points are out of coordinate
            nOpMode = DRAW_CURVE_IGNORE_POINT;
        else if (nCrossNum < pNum)	//only one point in the coordinate
        {
            if (p1->dY > dYMax || p1->dY < dYMin)
                dCross[nCrossNum] = p2->dY;
            else
                dCross[nCrossNum] = p1->dY;

            GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, dCross[nCrossNum]);
        }
        else
        {
            ;
        }

        return nOpMode;
    }
    else if (p1->dY == p2->dY)
    {
        if ( p1->dY < dYMin || p1->dY > dYMax)
            return DRAW_CURVE_IGNORE_POINT;

        if (dXMin >= min(p1->dX, p2->dX) && dXMin <= max(p1->dX, p2->dX))
        {
            dCross[nCrossNum] = dXMin;
            GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], p1->dY);

            //printf("p1[%d, %d], cross:%f\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy, dCross[nCrossNum]);
            nCrossNum++;

        }
        if (dXMax >= min(p1->dX, p2->dX) && dXMax <= max(p1->dX, p2->dX))
        {
            dCross[nCrossNum] = dXMax;
            GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], p1->dY);

            //printf("p2[%d, %d]\n", pPoints[nCrossNum]->ix, pPoints[nCrossNum]->iy);
            nCrossNum++;

        }

        if (nCrossNum == 0)
            nOpMode = DRAW_CURVE_IGNORE_POINT;
        else if (nCrossNum < pNum)
        {
            if (p1->dX > dXMax || p1->dX < dXMin)
                dCross[nCrossNum] = p2->dX;
            else
                dCross[nCrossNum] = p1->dX;

            GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], p1->dY);
        }
        else
        {
            ;
        }

        return nOpMode;
    }
    else
        dSlope 		= (p2->dY - p1->dY) / (p2->dX - p1->dX);		//slope

    dIntercept 	= p2->dY - dSlope * p2->dX;						//intercept

    dCross[nCrossNum] = dSlope * dXMin + dIntercept;			//check left boundary
    if (dCross[nCrossNum] >= dYMin && dCross[nCrossNum] <= dYMax
        && dCross[nCrossNum] >= min(p1->dY, p2->dY) && dCross[nCrossNum] <= max(p1->dY, p2->dY))
    {
        if (nCrossNum < pNum)
            GetPointPos(pPoints[nCrossNum], nLineIndex, dXMin, dCross[nCrossNum]);

        nCrossNum++;
    }

    dCross[nCrossNum] = dSlope * dXMax + dIntercept;			//check right boundary
    if (dCross[nCrossNum] >= dYMin && dCross[nCrossNum] <= dYMax
        && dCross[nCrossNum] >= min(p1->dY, p2->dY) && dCross[nCrossNum] <= max(p1->dY, p2->dY))
    {
        if (nCrossNum < pNum)
            GetPointPos(pPoints[nCrossNum], nLineIndex, dXMax, dCross[nCrossNum]);

        nCrossNum++;
    }

    dCross[nCrossNum] = (dYMin - dIntercept) / dSlope;		//check bottom boundary
    if (dCross[nCrossNum] >= dXMin && dCross[nCrossNum] <= dXMax
        && dCross[nCrossNum] >= min(p1->dX, p2->dX) && dCross[nCrossNum] <= max(p1->dX, p2->dX))
    {
        if (nCrossNum < pNum)
            GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], dYMin);

        nCrossNum++;
    }

    dCross[nCrossNum] = (dYMax - dIntercept) / dSlope;		//check top boundary
    if (dCross[nCrossNum] >= dXMin && dCross[nCrossNum] <= dXMax
        && dCross[nCrossNum] >= min(p1->dX, p2->dX) && dCross[nCrossNum] <= max(p1->dX, p2->dX))
    {
        if (nCrossNum < pNum)
            GetPointPos(pPoints[nCrossNum], nLineIndex, dCross[nCrossNum], dYMax);

        nCrossNum++;
    }
    //printf("nCrossNum=%d\n", nCrossNum);
    if (nCrossNum == 0)
        nOpMode = DRAW_CURVE_IGNORE_POINT;

    if (nCrossNum == 1)
    {
        if (bIn1)
            GetPointPos(pPoints[nCrossNum], nLineIndex, p1->dX, p1->dY);
        else if (bIn2)
            GetPointPos(pPoints[nCrossNum], nLineIndex, p2->dX, p2->dY);
        else
            nOpMode = DRAW_CURVE_IGNORE_POINT;
    }

    return nOpMode;
}

void	CtmCurveEx2::SetOpMode(int nMode)
{
    m_nOpMode = nMode;
}

int		CtmCurveEx2::GetOpMode()
{
    return m_nOpMode;
}

void	CtmCurveEx2::StretchCurve(double dRatio)	//�Y��y�Шt
{
	
	m_bPropertyChanged=TRUE;
    double dXDelta, dYDelta1, dYDelta2;
    double dXMin, dXMax, dYMin, dYMax;

    dXDelta =  fabs((m_dXMax - m_dXMin)) * dRatio;
    dYDelta1 = fabs((m_dYMax - m_dYMin)) * dRatio;
    dYDelta2 = fabs((m_dYExMax - m_dYExMin)) * dRatio;

    m_nOpMode = CURVE_OPERATION_STRETCH;

    dXMin = fmin(m_dXMin, m_dXMax);
    dXMax = fmax(m_dXMin, m_dXMax);

    dXMax += dXDelta;
    dXMin -= dXDelta;
    if ((dXMax - dXMin) <= 0.001F)
        return;
    //printf("dXMin:%f, dXMax:%f\n", dXMin, dXMax);
    if (m_Curve.nFlag & CURVE_XCONTRARY)
    {
        m_dXMax = dXMin;
        m_dXMin = dXMax;
    }
    else
    {
        m_dXMax = dXMax;
        m_dXMin = dXMin;
    }

    dYMin = m_dYMin;
    dYMax = m_dYMax;

    dYMax = dYDelta1;
    dYMin -= dYDelta1;

    if ((dYMax - dYMin) <= 0.001F)
        return;

    m_dYMin = dYMin;
    m_dYMax = dYMax;

    dYMin = m_dYExMin;
    dYMax = m_dYExMax;

    dYMax += dYDelta2;
    dYMin -= dYDelta2;

    if ((dYMax - dYMin) <= 0.001F)
        return;

    m_dYExMin = dYMin;
    m_dYExMax = dYMax;
}

void	CtmCurveEx2::MoveCoordinate(double dXDelta, double dYDelta)	//���ʮy�Шt����
{
    m_nOpMode = CURVE_OPERATION_MOVE_COORDINATE;

    m_dXMin  += dXDelta;
    m_dXMax  += dXDelta;

    m_dYMin += dYDelta;
    m_dYMax += dYDelta;
    m_dYExMin += dYDelta;
    m_dYExMax += dYDelta;
}

void	CtmCurveEx2::ResetCoordinate()	//���m�y�Шt��m
{
	m_bPropertyChanged=TRUE;
    m_nOpMode = CURVE_OPERATION_NONE;

    m_dXMin = m_dOldXMin;
    m_dXMax = m_dOldXMax;
    m_dYMin = m_dOldYMin;
    m_dYMax = m_dOldYMax;
    m_dYExMin = m_dOldYExMin;
    m_dYExMax = m_dOldYExMax;

}

void	CtmCurveEx2::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
    BOOL	bNeedUpdate = FALSE;

    if (!m_bTouchSupport)
        return;

    switch(message)
    {
       case MSG_MOUSELUP:
        //	printf("Mouse up(%d, %d)\n", wParam >> 16, wParam & 0x0000FFFF);
            MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
            break;
        case MSG_MOUSELDOWN:
        //	printf("Mouse Down(%d, %d)\n", wParam >> 16, wParam & 0x0000FFFF);
            MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
            //bNeedUpdate = TRUE;
            break;
        case MSG_MOUSEMOVE:
        //	printf("Mouse Move(%d, %d)\n", wParam >> 16, wParam & 0x0000FFFF);
            MouseMove(wParam >> 16, wParam & 0x0000FFFF, 0);
            break;
        default:
            break;
    }
}

BOOL	CtmCurveEx2::MouseUp(short x_pos,short y_pos,int key_flag)
{
    RECT rect    = GetWindowRECT();
    if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
        || y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
        return FALSE;

	m_bPropertyChanged=TRUE;
    ConvertPos(x_pos, y_pos);
    MoveCoordinate(-m_dXValueDelta, -m_dYValueDelta);
    Update();
    return TRUE;
}
BOOL	CtmCurveEx2::MouseDown(short x_pos, short y_pos, int key_flag)
{
    RECT rect    = GetWindowRECT();
    if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
        || y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
        return FALSE;
	m_bPropertyChanged=TRUE;
    ConvertPos(x_pos, y_pos);

    return TRUE;
}
BOOL	CtmCurveEx2::MouseMove(short x_pos, short y_pos, int key_flag)
{
    RECT rect    = GetWindowRECT();
    if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
        || y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
        return FALSE;
    return TRUE;
}

void	CtmCurveEx2::ConvertPos(short x_pos, short y_pos)
{
    RECT    	rect;
    tmPoint 	pointPos;
 //   CURVEPOINT 	pointValue;

    rect    = GetWindowRECT();
    if (x_pos < rect.left + m_rectLine.left || x_pos > rect.left + m_rectLine.right
        || y_pos < rect.top + m_rectLine.top || y_pos > rect.top + m_rectLine.bottom)
    {
        m_bOut = TRUE;		// out of the coordinate
        m_dXValueDelta = m_dYValueDelta = 0;
        return;
    }

    pointPos.ix = x_pos - (rect.left + m_rectLine.left);
    pointPos.iy = y_pos - (rect.top + m_rectLine.top);

    if (!m_bOut)
    {
        m_nXPosDelta = pointPos.ix - m_CurPos.ix;
        m_nYPosDelta = m_CurPos.iy - pointPos.iy;
    }
    else
    {
        m_bOut = !m_bOut;
        m_nXPosDelta = 0;
        m_nYPosDelta = 0;
    }
    //printf("delta pos:(%d, %d)\n", m_nXPosDelta, m_nYPosDelta);
    m_CurPos.ix = pointPos.ix;
    m_CurPos.iy = pointPos.iy;

    CalcPointValueDelta();
}


void	CtmCurveEx2::DrawMonitorArea()
{
	int		i, nBottom;
    int 	nXUnit, nYUnit;

    int nXPosLeft = 0;
    int nYPosTop  = 0;

	int nXPosRight = 0;
	int nYPosBottom =0;
	
	double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	if (1 ==m_monitor_attr.reference)
	{
		dYMin = m_dYExMin;
      	dYMax = m_dYExMax;	
	}
	
	dYUnit = (dYMax - dYMin) / m_Curve.nRow;  
	
	
    //printf("1:draw lines ...\n");
    nBottom	= m_rectLine.bottom - m_rectLine.top;
    nXUnit	= (m_rectLine.right - m_rectLine.left) / m_Curve.nCol;
    nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_Curve.nRow;
    
    
    if (0 == m_monitor_attr.reference)
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_monitor_attr.color);

            nXPosLeft = m_rectLine.left + (int)((m_monitor_attr.point1.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosTop = nBottom - (int)((m_monitor_attr.point1.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
            
            nXPosRight = m_rectLine.left + (int)((m_monitor_attr.point2.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosBottom = nBottom - (int)((m_monitor_attr.point2.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;

            for(int i=0; i<m_monitor_attr.line_width; i++)
            {
            	m_pDoubleBuffer->MoveToInBuffer(nXPosLeft+i, nYPosTop+i);
            	m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER,nXPosLeft+i,nYPosTop+i,nXPosRight-i,nYPosBottom-i);
        	}
        }
        else
        {
            _SetColor(m_monitor_attr.color);


            nXPosLeft = m_rectLine.left + (int)((m_monitor_attr.point1.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosTop = nBottom - (int)((m_monitor_attr.point1.dY - m_dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
            
            nXPosRight = m_rectLine.left + (int)((m_monitor_attr.point2.dX  - m_dXMin) / m_dXUnit * nXUnit);
            nYPosBottom = nBottom - (int)((m_monitor_attr.point2.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
            for(int i=0; i<m_monitor_attr.line_width; i++)
            {
            	_MoveTo(nXPosLeft+i, nYPosTop+i);
            	_Rectangle(_GBORDER,nXPosLeft+i,nYPosTop+i,nXPosRight-i,nYPosBottom-i);
        	}
        }
    }
    else
    {
        if (m_pDoubleBuffer != NULL)
        {
            m_pDoubleBuffer->SetColorInBuffer(m_monitor_attr.color);

           
            nXPosLeft = m_rectLine.left + (int)((m_monitor_attr.point1.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosTop = nBottom - (int)((m_monitor_attr.point1.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
            
            nXPosRight = m_rectLine.left + (int)((m_monitor_attr.point2.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosBottom = nBottom - (int)((m_monitor_attr.point2.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
          
           for(int i=0; i<m_monitor_attr.line_width; i++)
            {
            	m_pDoubleBuffer->MoveToInBuffer(nXPosLeft+i, nYPosTop+i);
            	m_pDoubleBuffer->DrawRectangleInBuffer(_GBORDER,nXPosLeft+i,nYPosTop+i,nXPosRight-i,nYPosBottom-i);
        	}
        }
        else
        {
            _SetColor(m_monitor_attr.color);

            nXPosLeft = m_rectLine.left + (int)((m_monitor_attr.point1.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosTop = nBottom - (int)((m_monitor_attr.point1.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
            
            nXPosRight = m_rectLine.left + (int)((m_monitor_attr.point2.dX - m_dXMin) / m_dXUnit * nXUnit);
            nYPosBottom = nBottom - (int)((m_monitor_attr.point2.dY - dYMin)/ dYUnit * nYUnit) + m_rectLine.top;
            for(int i=0; i<m_monitor_attr.line_width; i++)
            {
            	_MoveTo(nXPosLeft+i, nYPosTop+i);
            	_Rectangle(_GBORDER,nXPosLeft+i,nYPosTop+i,nXPosRight-i,nYPosBottom-i);
        	}
        }

    }	
}

void	CtmCurveEx2::SetMonitorAttr(CURVEPOINT 	*pValue1, CURVEPOINT *pValue2, int nLineWidth,int nColor,int nReference, BOOL bUsed)
{
	
	if(0==pValue1||0==pValue2)	return;
	m_monitor_attr.point1.dX=pValue1->dX;
	m_monitor_attr.point1.dY=pValue1->dY;
	m_monitor_attr.point2.dX=pValue2->dX;
	m_monitor_attr.point2.dY=pValue2->dY;
	m_monitor_attr.color=	nColor;
	m_monitor_attr.line_width	= nLineWidth;
	m_monitor_attr.reference=nReference;
	m_monitor_attr.flag=bUsed;
	double dMax=fmax(m_dXMin, m_dXMax);
	if(m_monitor_attr.point1.dX>=dMax)	m_monitor_attr.point1.dX=dMax;
	if(m_monitor_attr.point2.dX>=dMax) 	m_monitor_attr.point2.dX=dMax;
	if(m_monitor_attr.reference==0)
	{
		if(m_monitor_attr.point1.dY>=m_dYMax)	m_monitor_attr.point1.dY=m_dYMax;
		if(m_monitor_attr.point2.dY>=m_dYMax) m_monitor_attr.point2.dY=m_dYMax;
	}
	else
	{
		if(m_monitor_attr.point1.dY>=m_dYExMax)	m_monitor_attr.point1.dY=m_dYExMax;
		if(m_monitor_attr.point2.dY>=m_dYExMax) m_monitor_attr.point2.dY=m_dYExMax;
	}
	for(int i=0; i<4; i++)
	{
		if(!m_pszDBID[i])	return;	
	}
	long llValue=0;
	llValue=m_monitor_attr.point1.dX * Power(10, m_Curve.nXDigits);
	SetViewDBValue(m_pszDBID[0], llValue);
	llValue= m_monitor_attr.point2.dX * Power(10, m_Curve.nXDigits);
	SetViewDBValue(m_pszDBID[2], llValue);
	if(m_monitor_attr.reference==0)
	{
		llValue = m_monitor_attr.point1.dY * Power(10, m_Curve.nYDigits);
		SetViewDBValue(m_pszDBID[1], llValue);
		llValue = m_monitor_attr.point2.dY * Power(10, m_Curve.nYDigits);
		SetViewDBValue(m_pszDBID[3], llValue);
	}
	else
	{
		llValue = m_monitor_attr.point1.dY * Power(10, m_Curve.nYExDigits);
		SetViewDBValue(m_pszDBID[1], llValue);
		llValue = m_monitor_attr.point2.dY * Power(10, m_Curve.nYExDigits);
		SetViewDBValue(m_pszDBID[3], llValue);	
	}
}

BOOL	CtmCurveEx2::CheckMonitorCoordinateChange()
{
	bool bFlag=TRUE;
	for(int i=0; i<4; i++)
	{
		
		if(!m_pszDBID[i])
		{
			bFlag=FALSE;
			break;	
		}	
	}
	double left,top,right,bottom;
	bool   changed=false;
	
	if(bFlag)
	{
		left=((double)GetDBValue(m_pszDBID[0]).lValue)/Power(10, m_Curve.nXDigits);
		right=((double)GetDBValue(m_pszDBID[2]).lValue)/Power(10, m_Curve.nXDigits);		 
		if(m_monitor_attr.reference==0)
		{
			 top=((double)GetDBValue(m_pszDBID[1]).lValue)/Power(10, m_Curve.nYDigits);	
			 bottom=((double)GetDBValue(m_pszDBID[3]).lValue)/Power(10, m_Curve.nYDigits);	
		}
		else
		{
			top=((double)GetDBValue(m_pszDBID[1]).lValue)/Power(10, m_Curve.nYExDigits);	
			bottom =((double)GetDBValue(m_pszDBID[3]).lValue)/Power(10, m_Curve.nYExDigits);		
		}
		
		double dMax=fmax(m_dXMin, m_dXMax);
		if(left>=dMax)	left=dMax;
		if(right>=dMax) right=dMax;
		
		if(m_monitor_attr.reference==0)
		{
			if(top>=m_dYMax)		top=m_dYMax;
			if(bottom>=m_dYMax) 	bottom=m_dYMax;
		}
		else
		{
			if(top>=m_dYExMax)		top=m_dYExMax;
			if(bottom>=m_dYExMax) 	bottom=m_dYExMax;
		}
		if(m_monitor_attr.point1.dX !=left||m_monitor_attr.point1.dY!=top||
			m_monitor_attr.point2.dX!=right||m_monitor_attr.point2.dY!=bottom)
		{
			m_monitor_attr.point1.dX=left;
			m_monitor_attr.point1.dY=top;
			m_monitor_attr.point2.dX=right;
			m_monitor_attr.point2.dY=bottom;
			changed=true;	
		}
	}
	return changed;	
}


