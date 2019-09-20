/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 
/*===========================================================================+
|  Class    : CtmProcgram		                                             |
|  Task     : CtmProcgram Source file                                         |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 4.3.2                                                  |
|  Link     :G++(GCC) 4.3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Wong                                                         	 |
|  Version  : V2.00                                                          |
|  Creation : 01/05/2015  													 |
|  Modification:01/22/2015                                                   |
|  Revision :                                                                |
+===========================================================================*/

#include	"procgram.h"
#include	"utils.h"
#include	"commonaction.h"
#include    "tmdebug.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

IMPLEMENT_DYNCREATE(CtmProcgram, CtmWnd)
/*===========================================================================+
|           Class implementation                                             |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
void CtmProcgram::InitData() 
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
	m_wRefCount					= 0;
}
CtmProcgram::CtmProcgram(): CtmWnd()
{
	strcpy(TYPEName, "CtmProcgram");	
	
	m_pDoubleBuffer = NULL;
	InitData();
	                                         
	SetBorderWidth(3);  
	
	m_bgc			= DEFAULT_BGC;
	m_fgc			= 0;
	for(int i=0; i<MAX_GRAPHCOLOR_CNT;++i)
	{
		
		m_dwOldGraphColor[i]=
		m_dwGraphColor[i] 	= DEFAULT_GRAPHCOLOR0+5535*i;
		m_dwRefDestColor[i]	=DEFAULT_GRAPHCOLOR1;
		m_pszRefNameID[i]	= NULL;
		m_pszRefName[i]		= NULL;
	}
	 
	m_wGraphType	= 0x22;
	m_nGraphWidth	= DEFAULT_GRAPHWIDTH;
	m_dXMax			= DEFAULT_SHOWXVALUE;
	m_bShowGrid		= TRUE;
	m_wGridRow		=
	m_wGridCol		= DEFAULT_GRIDCOL;
	
	m_dwGridColor	= 
	m_dwXAxesColor	= 
	m_dwYAxesColor	= DEFAULT_GRIDCOLOR;
	m_dwRefColorControl = 0;
	m_dwRefColorChange		= -1;
	
	m_dXMin 		= 0;
	m_dYMin 		= 0;
	m_dXMax 		= 0;
	m_dYMax 		= 0;
	m_dYUnit 		= 0;
	m_dXUnit 		= 0;
	m_wRefCount		= 0;
	
	m_pszGraphName = NULL;
	m_pszGraphNameID = NULL;
	m_pszXName		= NULL;							
	m_pszXNameID 	= NULL;	
	memset(m_pszYName,0,sizeof(m_pszYName));
	for(int i=0; i<MAX_ACTIONS_NUM;++i)
	{
		m_pszYName[i].pszYName		= NULL;	
		m_pszYName[i].pszYNameID	= NULL;				
	}			
	m_bXName 		= TRUE;
	m_bYName		= TRUE;
	                     
	m_wXDigits = 0;
   	for(int i=0; i<MAX_ACTIONS_NUM;i++)
   	{
			for(int j=0;j<MAX_ELEMENTS_CNT;j++)
			{
				memset(&m_tmStartValue[i][j],0,sizeof(tmProcValue));
				memset(&m_tmEndValue[i][j],0,sizeof(tmProcValue));
			}	
   	}
}     
CtmProcgram::~CtmProcgram()
{
	
//   for(int i=0; i<MAX_GRAPHCOLOR_CNT;++i)
//   {
//	   	if(m_pRefColor[i]!=NULL)
//	   	{
//	   		delete m_pRefColor[i];	
//	   		m_pRefColor[i]=NULL;
//	   	}
//	  	for(int j = 0; j < m_wGridCol; j++)
//   		{
//   			if(m_pRefStaticX2[i][j]!=NULL)
//   			{
//   				delete m_pRefStaticX2[i][j];
//   				m_pRefStaticX2[i][j]=NULL;
//   			}
//   		}
//   	if(m_pRefStaticX2[i]!=NULL)
//   	{
//   		delete []m_pRefStaticX2[i];
//   		m_pRefStaticX2[i]=NULL;
//   	}
//   		
//   }	
	if (m_pDoubleBuffer != NULL)
	{
		delete m_pDoubleBuffer;
		m_pDoubleBuffer = NULL;
	}
}

BOOL	CtmProcgram::CreateA()
{
	CtmWnd::CreateA();

	SetFGColor(m_fgc);
    SetBGColor(m_bgc);
    
   
//    for(int j=0; j<MAX_GRAPHCOLOR_CNT;j++)
//    {
//    	 m_pRefStaticX2[j] 	= new CtmStaticX2*[m_wGridCol];
//	    for(int i = 0; i < m_wGridCol; i++)
//	   	{
//	   		m_pRefStaticX2[j][i] = new CtmStaticX2;
//	   		m_pRefStaticX2[j][i]->SetPropertys();
//			m_pRefStaticX2[j][i]->SetPropValueT((char*)"bgc",	m_dwGraphColor[j]);
//			m_pRefStaticX2[j][i]->SetPropValueT((char*)"destcolor",m_dwRefDestColor[j]);
//   			m_pRefStaticX2[j][i]->SetPropValueT((char*)"colorcontrol",m_dwRefColorControl);
//   			m_pRefStaticX2[j][i]->SetPropValueT((char*)"colorchange",m_dwRefColorChange);
//			m_pRefStaticX2[j][i]->SetPropValueT((char*)"wtype",m_wGraphType);
//	   	}
//	   	m_pRefColor[j]	=	new CtmStaticX2;
//   		m_pRefColor[j]->SetPropertys();
//   		m_pRefColor[j]->SetPropValueT((char*)"bgc",	m_dwGraphColor[j]);
//   		m_pRefColor[j]->SetPropValueT((char*)"destcolor",m_dwRefDestColor[j]);
//   		m_pRefColor[j]->SetPropValueT((char*)"colorcontrol",m_dwRefColorControl);
//   		m_pRefColor[j]->SetPropValueT((char*)"colorchange",m_dwRefColorChange);
//   		m_pRefColor[j]->SetPropValueT((char*)"wtype",m_wGraphType);
//	}
   	
   
   	m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
   	if (m_wnd.wStyle & tmWS_TRANSPARENT)
   	{
   		if (GetParent() != NULL) 
 			m_bgc = GetParent()->GetBGColor();	
   	}
	m_pDoubleBuffer->InitBuffer(m_bgc);
	
	if(m_dXMax == 0) 	 m_dXMax= DEFAULT_SHOWXVALUE;
	if(m_wGridCol == 0)  m_wGridCol = DEFAULT_GRIDCOL;
	
	return TRUE;
}

void	CtmProcgram::SetPropertys()
{
	char szName[32];
	CtmWnd::SetPropertys();
	
	for(int i=0; i<MAX_GRAPHCOLOR_CNT;++i)
	{
		sprintf(szName,"destcolor%d",i);
		SetPropInfo(szName,		tmDWORD,	&m_dwRefDestColor[i]);
		sprintf(szName,"graphcolor%d",i);
		SetPropInfo(szName,		tmDWORD,	&m_dwGraphColor[i]);
		sprintf(szName,"refname%d",i);
		SetPropInfo(szName,		tmSTR,		&m_pszRefName[i]);
		sprintf(szName,"refnameID%d",i);
		SetPropInfo(szName,		tmSTR,		&m_pszRefNameID[i]);
			
	}
	SetPropInfo((char*)"colorchange",		tmDWORD,	&m_dwRefColorChange);
	SetPropInfo((char*)"colorcontrol", 	tmDWORD,	&m_dwRefColorControl);
	SetPropInfo((char*)"graphwidth",		tmINT,		&m_nGraphWidth);
	SetPropInfo((char*)"xmax",				tmDOUBLE, 	&m_dXMax);
	SetPropInfo((char*)"xdigits",			tmWORD,		&m_wXDigits);
	SetPropInfo((char*)"showgrid",			tmBOOL,		&m_bShowGrid);
	SetPropInfo((char*)"gridcols",			tmWORD,		&m_wGridCol);
	SetPropInfo((char*)"gridrows",			tmWORD,		&m_wGridRow);
	SetPropInfo((char*)"gridcolor",   		tmDWORD,	&m_dwGridColor);
	SetPropInfo((char*)"xaxescolor",  		tmDWORD,	&m_dwXAxesColor); 
	SetPropInfo((char*)"yaxescolor",   	tmDWORD,	&m_dwYAxesColor); 
	SetPropInfo((char*)"graphname",		tmSTR,		&m_pszGraphName);
	SetPropInfo((char*)"graphnameID",		tmSTR,		&m_pszGraphNameID);
	SetPropInfo((char*)"xname",			tmUSTR,		&m_pszXName);
	SetPropInfo((char*)"xnameID",			tmSTR,		&m_pszXNameID);   
    SetPropInfo((char*)"xnameshow",		tmBOOL	,	&m_bXName);
	SetPropInfo((char*)"ynameshow",		tmBOOL	,	&m_bYName);
	for(int i=0;i<MAX_ACTIONS_NUM;++i)
	{
		
		sprintf(szName,"yname%d",i);
		SetPropInfo(szName,			tmUSTR,		&m_pszYName[i].pszYName);
		sprintf(szName,"ynameID%d",i);
		SetPropInfo(szName,			tmSTR,		&m_pszYName[i].pszYNameID); 
			
	}
	
}



void	CtmProcgram::PropareShow()
{
    InitData();

	if (m_pszGraphNameID != NULL)
		StrIDToStr(m_pszGraphNameID, &m_pszGraphName);
	if (m_pszXNameID != NULL)
		StrIDToStr(m_pszXNameID, &m_pszXName);
	for(int i=0; i<MAX_GRAPHCOLOR_CNT;++i)
	{
		if(m_pszRefNameID[i]!=NULL)
		{
			m_wRefCount++;
			//printf("m_pszRefNameID[%d]=%s\n",i,m_pszRefNameID[i]);
			StrIDToStr(m_pszRefNameID[i], &m_pszRefName[i]);
		}
	}
		
	for(int i=0; i<m_wGridCol;++i)
	{
		if (m_pszYName[i].pszYNameID!= NULL)
		{
			StrIDToStr(m_pszYName[i].pszYNameID, &m_pszYName[i].pszYName);
		}
	}
	
	if (m_wnd.wStyle & tmWS_TRANSPARENT)
   	{
   		if (GetParent() != NULL) 
 			m_bgc = GetParent()->GetBGColor();	
 		
   	}	
	if (m_pDoubleBuffer != NULL)
		m_pDoubleBuffer->InitBuffer(m_bgc);
	else 
		Clear();
		
	m_rectCoordinate.left	= 0;
	m_rectCoordinate.right	= GetClientWidth() - 1;
	m_rectCoordinate.top	= 0;
	m_rectCoordinate.bottom	= GetClientHeight() -1;
	
}

int		CtmProcgram::GetRangeXY()
{
	int    nIndex = 0;
	double	dXMax=0;
	if(m_dXMax==0)	m_dXMax=DEFAULT_SHOWXVALUE;
	double	dResult=0;
	//printf("1:::m_dXMax=%lf\n",m_dXMax);
	for(nIndex = 0; nIndex < m_wGridCol; nIndex ++)
	{
		for(int j=0; j<MAX_GRAPHCOLOR_CNT;j++)
		{
			for(int i=0; i<MAX_ELEMENTS_CNT;i++)
			{
				dXMax=fmax(dXMax,m_tmEndValue[nIndex][i].dValue[j]);
				if(dResult <=dXMax )				
				{
					dResult	= dXMax;
					dResult += (m_dYMax/m_dXMax);
				}
			}
		}
	}
	
	//printf("2:::%d,%d,%d,%lf\n",m_wGridRow,(int)(m_wGridRow*PER_UNIT),
	//(int)dResult/(int)(m_wGridRow*PER_UNIT),
	//((int)dResult/(int)(m_wGridRow*PER_UNIT)+1)*(m_wGridRow*PER_UNIT));
	//printf("3:::m_dXMax=%lf,dResult=%lf\n",m_dXMax,dResult);	
	m_dXMax=((int)dResult/(int)(m_wGridRow*PER_UNIT)+1)*(m_wGridRow*PER_UNIT)/*dResult*/;
	
	return 0;
}

RECT	CtmProcgram::GetTitleRect()
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	
	m_bTitle = TRUE;
	if(m_wRefCount!=0)
		AddTopRelation(&m_relationTitle, m_pcFont->m_wHeight*(m_wRefCount+0.5));
	else
		AddTopRelation(&m_relationTitle, m_pcFont->m_wHeight);
	Result = *m_relationTitle.pRect;
	return Result;
}

RECT	CtmProcgram::GetCutRect()
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	
	return Result;
}

RECT	CtmProcgram::GetXNameRect()
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

RECT	CtmProcgram::GetYNameRect()
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

RECT	CtmProcgram::GetXAxesScaleRect()
{
	RECT Result;
	AddDownRelation(&m_relationXAxesScale, m_pcFont->m_wHeight);
	Result = *m_relationXAxesScale.pRect;
	return Result;
}

RECT	CtmProcgram::GetYAxesScaleRect()
{
	RECT Result;
	int	 nWidth=0;
	for(int i = 0; i < m_wGridCol; i ++)
	{
		if(m_pszYName[i].pszYName==NULL)	continue;
		if ((int)(strlen(m_pszYName[i].pszYName) * m_pcFont->m_wWidth) / 2 > nWidth)
			nWidth = strlen(m_pszYName[i].pszYName) * m_pcFont->m_wWidth / 2;
	}
	
	AddLeftRelation(&m_relationYAxesScale, nWidth);
	Result = *m_relationYAxesScale.pRect;
	
	return Result;
}

RECT	CtmProcgram::GetLineRect()
{
	RECT Result;
	int nXUnit, nYUnit;
	Result.left = GetLastLeft()+m_wXDigits*12;
	Result.right = GetLastRight()-m_wXDigits*12;
	Result.top = GetLastTop();
	Result.bottom = GetLastBottom();
	if(m_wGridRow==0)	m_wGridRow=	DEFAULT_GRIDCOL;
	if(m_wGridCol==0)	m_wGridCol=	DEFAULT_GRIDCOL;
	nXUnit	= (Result.right - Result.left) / m_wGridRow;
	nYUnit	= (Result.bottom - Result.top) / m_wGridCol;
	
	Result.right = Result.left + nXUnit * m_wGridRow;
	Result.bottom = Result.top + nYUnit * m_wGridCol;
	m_rectLine = Result;
	
	return Result;
}

void	CtmProcgram::SeperateArea()
{
	GetTitleRect();				
	GetXNameRect();	 			 
//	GetYNameRect();				   
	GetXAxesScaleRect();	 
	GetYAxesScaleRect();		  
	GetLineRect();				

}

short	CtmProcgram::ShowExtentEx(short nX, short nY, char* pszString, int nType)
{
	if (m_pDoubleBuffer != NULL)
		return ShowExtentD(nX, nY, pszString, nType);
	else return ShowExtent(nX, nY, pszString);
}

short   CtmProcgram::ShowExtentD(short nX, short nY, char* pszString, int nType)
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

void	CtmProcgram::ShowTitle()
{
	int	nPos = 0;
	if(m_pszGraphName != NULL)
	{
		
		nPos = m_rectTitle.left + 
		(m_rectTitle.right - m_rectTitle.left - strlen(m_pszGraphName) * m_pcFont->m_wWidth / 2) / 2;
		ShowExtentEx(nPos, m_rectTitle.top ,m_pszGraphName, m_pcFont->GetFontCode());					
	}
	for(int i=0; i<MAX_GRAPHCOLOR_CNT;++i)
	{
		if(m_pszRefName[i]!=NULL)
		{
			nPos =m_rectLine.left + 10;
			ShowExtentEx(nPos, m_rectTitle.top+m_pcFont->m_wHeight*(i+0.5) ,m_pszRefName[i], m_pcFont->GetFontCode());
		}
	}
}



void	CtmProcgram::DrawAxes()
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

void	CtmProcgram::DrawAxesGrid()
{
	int	nXUnit, nYUnit,nYDotLine;
	int	i = 0;
	if (m_wGridRow == 0) m_wGridRow = DEFAULT_GRIDCOL;
	if (m_wGridCol == 0) m_wGridCol = DEFAULT_GRIDCOL;
	
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	nXUnit	=(m_rectLine.right - m_rectLine.left)/m_wGridRow;
//	nXUnit	= (m_rectLine.right - m_rectLine.left - nXUnit/2) / m_wGridRow;
	nYDotLine=(m_rectLine.bottom - m_rectLine.top) / 4;
	if (m_pDoubleBuffer != NULL)
	{
		m_pDoubleBuffer->SetColorInBuffer(m_dwGridColor);
		for(i = 1; i <= m_wGridCol; i ++)
		{
			m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + 1, m_rectLine.bottom - nYUnit * i - 1);   
			m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom - nYUnit * i - 1); 
		}
		for(i = 1; i <= m_wGridRow; i ++)
		{
			for (int j = 0; j <= nYDotLine; j++)
			{
					m_pDoubleBuffer->MoveToInBuffer(m_rectLine.left + nXUnit * i,	m_rectLine.top + j * 4); 
					m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.top + j * 4 + 1);
			}
				
			m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.left + nXUnit * i, m_rectLine.bottom);  
		}
		for (int j = 0; j <= nYDotLine; j++)
		{
				m_pDoubleBuffer->MoveToInBuffer(m_rectLine.right,	m_rectLine.top + j * 4); 
				m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.top + j * 4 + 1);
		}
		m_pDoubleBuffer->DrawLineToInBuffer(m_rectLine.right, m_rectLine.bottom);
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
		for(int i=1; i<=m_wGridRow; i++)
		{
			for (int j = 0; j <= nYDotLine; j++)
			{
				_MoveTo(m_rectLine.left + nXUnit * i,	m_rectLine.top + j * 4); 
				_LineTo(m_rectLine.left + nXUnit * i, m_rectLine.top + j * 4 + 1);
			}
				
			_LineTo(m_rectLine.left + nXUnit * i, m_rectLine.bottom);  
		}
		for (int j = 0; j <= nYDotLine; j++)
		{
			_MoveTo(m_rectLine.right,	m_rectLine.top + j * 4); 
			_LineTo(m_rectLine.right, m_rectLine.top + j * 4 + 1);
		}
				
		_LineTo(m_rectLine.right, m_rectLine.bottom);  
	}
}
void	CtmProcgram::DrawSclae()
{
	DrawXScale();
	DrawYScale();
	
}

void	CtmProcgram::DrawXScale()
{
	int nStrXOffset = 0;
	int i = 0;
	int	nXUnit;
	char	sz[100], szXFormat[10];	
	double  dXValue = 0.0;
	if(m_wGridRow==0)	m_wGridRow=DEFAULT_GRIDCOL;
	
	memset(sz, 0, sizeof(sz));
	snprintf(szXFormat, 10, "%%.%df", m_wXDigits);
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_wGridRow;
//	nXUnit	= (m_rectLine.right - m_rectLine.left - nXUnit/2) / m_wGridRow;
	
	snprintf(sz, 100, szXFormat, 0);
	nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
	for(i = 0; i <= m_wGridRow; i ++)
	{
		memset(sz, 0, sizeof(sz));
		dXValue = m_dXMax*i/m_wGridRow;
		snprintf(sz, 100, szXFormat, dXValue);
		nStrXOffset	= strlen(sz) * m_pcFont->m_wWidth / 4;
		
		if (m_rectLine.left + nXUnit * i + nStrXOffset > m_rectLine.right)
		{
				nStrXOffset =m_rectLine.right -strlen(sz) * m_pcFont->m_wWidth / 4;	
		}
		if (i == m_wGridRow)
			ShowExtentEx(nStrXOffset,m_rectXAxesScale.top + 1, sz);	
		else	
			ShowExtentEx(m_rectLine.left + nXUnit * i - nStrXOffset,
						m_rectXAxesScale.top + 1, sz);		
		
	}
}


//	2008/4/2 11:06¤W¤È fans moidfy
void	CtmProcgram::DrawYScale()
{
	
	double	dYMin = m_dYMin;
	double	dYMax = m_dYMax;
	double	dYUnit= m_dYUnit;
	if(m_wGridCol==0)	m_wGridCol=DEFAULT_GRIDCOL;
	m_dYUnit = (m_dYMax - m_dYMin) / m_wGridCol;
	
    int nYUnit;
	int nStrXOffset, nStrYOffset;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	for(int i = 1; i <= m_wGridCol; i ++)
	{
		if(m_pszYName[i-1].pszYName==NULL)	continue;
		nStrXOffset	= strlen(m_pszYName[i-1].pszYName) * m_pcFont->m_wWidth / 2+ 1;
		nStrYOffset	= m_pcFont->m_wHeight / 2;
		ShowExtentEx(m_rectYAxesScale.left,	m_rectYAxesScale.bottom - nYUnit * i - nStrYOffset, m_pszYName[i-1].pszYName, m_pcFont->GetFontCode());	
	}
	m_dYMin = dYMin;
    m_dYMax = dYMax;
    m_dYUnit= dYUnit;
   
}

void	CtmProcgram::DrawName()
{
	if (m_bXName) DrawXName();
	//if (m_bYName) DrawYName();
}

void	CtmProcgram::DrawXName()
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

void	CtmProcgram::DrawYName()
{
	
	if(m_pszYName != NULL)
	{
		int nStrXOffset = 0;
		int nStrYOffset = 0;
		char	sz[100];	
		int nYStrLen  = 0;
		BYTE 	*pChar;
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
		
		while(*((WORD*)pChar) != 0)
		{		
			sz[2]	= 
			sz[1]	= 0;
			
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

void	CtmProcgram::DrawCoordinate()
{
	if (m_bShowGrid)
		DrawAxesGrid();
	DrawAxes();
	
	DrawSclae();
	
	DrawName();
}

void	CtmProcgram::Show()
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
		DrawTimergrambar();
		//	double buffer
		if (m_pDoubleBuffer != NULL)		
			m_pDoubleBuffer->DrawWithDoubleBuffer();
			
		//ShowGraph();
		CtmWnd::Show();	 
	}	

}


void	CtmProcgram::Update()
{
	Show();
	for(int i=0;i<MAX_GRAPHCOLOR_CNT;++i)
	if(m_dwOldGraphColor[i]!=m_dwGraphColor[i])
	{
		m_dwOldGraphColor[i]=m_dwGraphColor[i];
	}
}

BOOL	CtmProcgram::FindRelationEnd(tmRectRelation* pSource, tmRectRelation** pDest)
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

RECT	CtmProcgram::FindLast(tmRectRelation* pSource)
{
	RECT Result;
	memset(&Result, 0, sizeof(Result));
	tmRectRelation* TempRelation;
	
	if (FindRelationEnd(pSource, &TempRelation))
		Result = *TempRelation->pRect;
	return Result;
}

int		CtmProcgram::GetLastLeft()
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

int		CtmProcgram::GetLastRight()
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


int		CtmProcgram::GetLastTop()
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

int		CtmProcgram::GetLastBottom()
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

BOOL	CtmProcgram::AddTopRelation(tmRectRelation* pSource, int nHeight)
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
		
BOOL	CtmProcgram::AddDownRelation(tmRectRelation* pSource, int nHeight)
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
	}
	
	*pSource->pRect = TempRect;
	bResult= TRUE;
	return bResult;
}
		
BOOL	CtmProcgram::AddLeftRelation(tmRectRelation* pSource, int nWidth)
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

BOOL	CtmProcgram::AddRightRelation(tmRectRelation* pSource, int nWidth)
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

void	CtmProcgram::ShowGraph()
{
	int nYUnit,nXUnit;
	int	nCount=0;
	int nPos=0;
	int  nLeft, nRight, nTop, nBottom;
	if(m_wGridCol==0)	m_wGridCol=DEFAULT_GRIDCOL;
	if(m_wGridRow==0)	m_wGridRow=DEFAULT_GRIDCOL;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_wGridRow;
//	nXUnit	= (m_rectLine.right - m_rectLine.left - nXUnit/2) / m_wGridRow;
	
	for(int i=0;i<MAX_GRAPHCOLOR_CNT;++i)
	{
		if(m_pszRefName[i]!=NULL)
		{
			nPos =m_rectClient.left+m_rectLine.left + 10;
			nLeft=nPos+strlen(m_pszRefName[i]) * m_pcFont->m_wWidth/2+10;
			nTop=m_rectTitle.top+m_rectClient.top+m_pcFont->m_wHeight*(i+0.5);
			if(m_dwOldGraphColor[i]!=m_dwGraphColor[i])
			{
				m_pRefColor[i]->SetPropValueT((char*)"bgc",	m_dwGraphColor[i]);
				m_pRefColor[i]->SetPropValueT((char*)"destcolor",m_dwRefDestColor[i]);
   				m_pRefColor[i]->SetPropValueT((char*)"colorcontrol",m_dwRefColorControl);
   				m_pRefColor[i]->SetPropValueT((char*)"colorchange",m_dwRefColorChange);
			}
			m_pRefColor[i]->SetPropValueT((char*)"top",nTop);
			m_pRefColor[i]->SetPropValueT((char*)"bottom",nTop+m_pcFont->m_wHeight/2);
			m_pRefColor[i]->SetPropValueT((char*)"left",nLeft);
			m_pRefColor[i]->SetPropValueT((char*)"right",nLeft+20);
			m_pRefColor[i]->CreateA();
			m_pRefColor[i]->Update();
		}
	}
	
	
	for(int i = 0; i <m_wGridCol; ++i)
   	{
   		for(int j=0; j<m_wRefCount; ++j)
   		{	
   			for(int k=0; k<MAX_ELEMENTS_CNT;++k)
   			{
	   			if(m_tmEndValue[i][k].dValue[j]<=0.0||
	   			   m_tmEndValue[i][k].dValue[j]==m_tmStartValue[i][k].dValue[j])	
	   			 {
	   			 		//nCount++;
	   			 		continue;
	   			 }	
	   			nBottom = m_rectLine.bottom + m_rectClient.top-nCount*nYUnit/m_wRefCount-1;
	   			nTop=nBottom-nYUnit/m_wRefCount+1;	
	   			nLeft=m_rectLine.left+m_rectClient.left+(int)(m_tmStartValue[i][k].dValue[j]*nXUnit*m_wGridRow/m_dXMax);
	   			nRight=m_rectLine.left+m_rectClient.left+(int)(m_tmEndValue[i][k].dValue[j]*nXUnit*m_wGridRow/m_dXMax);
	   			if(m_dwOldGraphColor[j]!=m_dwGraphColor[j])
	   			{
	   				m_pRefStaticX2[j][i]->SetPropValueT((char*)"bgc",	m_dwGraphColor[j]);
	   				m_pRefStaticX2[j][i]->SetPropValueT((char*)"destcolor",m_dwRefDestColor[j]);
	   				m_pRefStaticX2[j][i]->SetPropValueT((char*)"colorcontrol",m_dwRefColorControl);
	   				m_pRefStaticX2[j][i]->SetPropValueT((char*)"colorchange",m_dwRefColorChange);
	   			}
	   			m_pRefStaticX2[j][i]->SetPropValueT((char*)"top", nTop);
	   			m_pRefStaticX2[j][i]->SetPropValueT((char*)"bottom", nBottom);
	   			m_pRefStaticX2[j][i]->SetPropValueT((char*)"left", nLeft+1);
	   			m_pRefStaticX2[j][i]->SetPropValueT((char*)"right", nRight);
	   			m_pRefStaticX2[j][i]->CreateA();
		   		m_pRefStaticX2[j][i]->Update();	
		   	}
		   	nCount++;
   		}
   	}
}


void	CtmProcgram::SetProcValue(int nType,tmProcValue* tmStartValue,tmProcValue* tmEndValue,int nElements)
{
	if(nType>=MAX_ACTIONS_NUM||nElements<=0)	return;
	for(int i=0;i<MAX_GRAPHCOLOR_CNT;++i)
	{
		for(int j=0; j<nElements;++j)
		{
			if(tmStartValue[j].dValue[i]<0.0)		tmStartValue[j].dValue[i]=0.0;
			if(tmEndValue[j].dValue[i]<0.0)		tmEndValue[j].dValue[i]=0.0;
			if(tmStartValue[j].dValue[i]>tmEndValue[j].dValue[i])
			{
				m_tmStartValue[nType][j].dValue[i]=tmEndValue[j].dValue[i];	
				m_tmEndValue[nType][j].dValue[i]=tmStartValue[j].dValue[i];
			}
			else
			{
				m_tmStartValue[nType][j].dValue[i]=tmStartValue[j].dValue[i];
				m_tmEndValue[nType][j].dValue[i]=tmEndValue[j].dValue[i];
			}
		}
	}	
}

void CtmProcgram::DrawTimergrambar()
{       
	
	int nYUnit,nXUnit;
	int	nCount=0;
	int nPos=0;
	int  nLeft, nRight, nTop, nBottom;
	if(m_wGridCol==0)	m_wGridCol=DEFAULT_GRIDCOL;
	if(m_wGridRow==0)	m_wGridRow=DEFAULT_GRIDCOL;
	nYUnit	= (m_rectLine.bottom - m_rectLine.top) / m_wGridCol;
	nXUnit	= (m_rectLine.right - m_rectLine.left) / m_wGridRow;
//	nXUnit	= (m_rectLine.right - m_rectLine.left - nXUnit/2) / m_wGridRow;
	
	for(int i=0;i<MAX_GRAPHCOLOR_CNT;++i)
	{
		if(m_pszRefName[i]!=NULL)
		{
			nPos =m_rectLine.left + 10;
			nLeft=nPos+strlen(m_pszRefName[i]) * m_pcFont->m_wWidth/2+10;
			nTop=m_rectTitle.top+m_pcFont->m_wHeight*(i+0.5);
			if (m_pDoubleBuffer != NULL)
				{
					//m_dwRefColorControl=0;
					if ((tmWS_FRAMEWND & m_wnd.wStyle) || m_dwRefColorControl== 0)
					{
						m_pDoubleBuffer->SetColorInBuffer( m_dwGraphColor[i]);
						//printf("m_dwRefColorControl=%d\n",m_dwRefColorControl);
						m_pDoubleBuffer->DrawRectangleInBuffer(m_dwGraphColor[i],nLeft,nTop,nLeft+20,nTop+5);
					}
					else
					{
						//printf("m_dwRefColorControl=%d,m_dwGraphColor[0]=%d,m_dwRefDestColor[0]=%d,m_dwRefColorChange=%d\n",
						//m_dwRefColorControl,m_dwGraphColor[i],m_dwRefDestColor[i],m_dwRefColorChange);
						m_pDoubleBuffer->FillRectInBuffer(m_dwRefColorControl, m_dwGraphColor[i], m_dwGraphColor[i], 
	    				nLeft,nTop,nLeft+20,nTop+5, m_dwRefDestColor[i],m_dwRefColorChange);	
					}	
				}	
		}
	}
	
	
	for(int i = 0; i <m_wGridCol; ++i)
   	{
   		for(int j=0; j<m_wRefCount; ++j)
   		{	
   			for(int k=0; k<MAX_ELEMENTS_CNT;++k)
   			{
	   			if(m_tmEndValue[i][k].dValue[j]<=0.0||
	   			   m_tmEndValue[i][k].dValue[j]==m_tmStartValue[i][k].dValue[j])	
	   			 {
	   			 		//nCount++;
	   			 		continue;
	   			 }	
	   			nBottom = m_rectLine.bottom -nCount*nYUnit/m_wRefCount-2;
	   			nTop=nBottom-nYUnit/m_wRefCount+2;	
	   			nLeft=m_rectLine.left+(int)(m_tmStartValue[i][k].dValue[j]*nXUnit*m_wGridRow/m_dXMax);
	   			nRight=m_rectLine.left+(int)(m_tmEndValue[i][k].dValue[j]*nXUnit*m_wGridRow/m_dXMax);
	   			//add method
	   			if (m_pDoubleBuffer != NULL)
				{
					//m_dwRefColorControl=0;
					if ((tmWS_FRAMEWND & m_wnd.wStyle) || m_dwRefColorControl== 0)
					{
						m_pDoubleBuffer->SetColorInBuffer( m_dwGraphColor[j]);
						//printf("m_dwRefColorControl=%d\n",m_dwRefColorControl);
						m_pDoubleBuffer->DrawRectangleInBuffer(m_dwGraphColor[j],nLeft+1,nTop,nRight,nBottom);
					}
					else
					{
						//printf("m_dwRefColorControl=%d,m_dwGraphColor[0]=%d,m_dwRefDestColor[0]=%d,m_dwRefColorChange=%d\n",
						//m_dwRefColorControl,m_dwGraphColor[j],m_dwRefDestColor[j],m_dwRefColorChange);
						m_pDoubleBuffer->FillRectInBuffer(m_dwRefColorControl, m_dwGraphColor[j], m_dwGraphColor[j], 
	    				nLeft+1,nTop,nRight,nBottom, m_dwRefDestColor[j],m_dwRefColorChange);	
					}	
				}	
		   	}
		   	nCount++;
   		}
   	}
   			                     
}