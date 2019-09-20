/*==========================================================================+
|  Class    : Line Table		                                            |
|  Task     : Line Table source file                        				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Leo                                                           |
|  Version  : V1.00                                                         |
|  Creation : 2006/7/12                                                    	|
|  Revision : 			                                                    |
+==========================================================================*/

#include    "linetable.h"
#include	"init.h"
#include    "utils.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
#define     FLAG_LT_VERTLINE               0x0001  // 橫線
#define     FLAG_LT_HORILINE               0x0002  // 豎線

#define     ID_COLOR_MAINLINE               1
#define     ID_COLOR_ACCESSORIALLINE1       2
#define     ID_COLOR_ACCESSORIALLINE2       3
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
/*==========================================================================+
|           Class implementation - CtmLineTable                             |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmLineTable, CtmWnd)
CtmLineTable::CtmLineTable(tmWND* pwnd,tmLINETABLE* plinetable) : CtmWnd()
{
    short   i;
		m_bBackGround 	= FALSE;
    CtmWnd::Create(pwnd);

    m_linetable   = *plinetable;
    m_plinetable  = &m_linetable;
    m_wType       = m_linetable.wType;
    for(i=0; i<MAX_LT_POINTS; i++)
	{
    	m_anVertpoints[i]       = -1;
        m_anHoripoints[i]       = -1;
    }
    i = 0;
    while((i<MAX_LT_POINTS) && (*(m_linetable.pnVertpoints+i) != -1))
    {
        m_anVertpoints[i] = *(m_linetable.pnVertpoints+i);
        i++;
    }
    i = 0;
    while((i<MAX_LT_POINTS) && (*(m_linetable.pnHoripoints+i) != -1))
    {
        m_anHoripoints[i] = *(m_linetable.pnHoripoints+i);
        i++;
    }
    Set();
    if (m_wnd.wStyle & tmWS_SHOW) Show();
}

CtmLineTable::CtmLineTable(): CtmWnd()
{
	strcpy(TYPEName, "CtmLineTable");

    m_wType      = 0;
	m_clMainLine = 0;
	m_clAccrLine1= 0;
	m_clAccrLine2= 0;
	m_plinetable = NULL;
	m_linetable.tmclMainLine.r  = 173;//82;
	m_linetable.tmclMainLine.g  = 182;//101;
	m_linetable.tmclMainLine.b  = 189;//115;
	m_linetable.tmclAccrLine1.r = 82; //173;
	m_linetable.tmclAccrLine1.g = 101;//182;
	m_linetable.tmclAccrLine1.b = 115;//189;
	m_linetable.tmclAccrLine2.r = 33;
	m_linetable.tmclAccrLine2.g = 73;
	m_linetable.tmclAccrLine2.b = 107;
	m_linetable.nMainLineWidth  = 1;
	m_linetable.wType           = 0;
	m_linetable.nVertRows       = 0;
	m_linetable.nHoriCols       = 0;
	m_bBackGround 							= FALSE;
	for(short i=0; i<MAX_LT_POINTS; i++)
	{
    	m_anVertpoints[i]       = -1;
        m_anHoripoints[i]       = -1;
    }
}

CtmLineTable::~CtmLineTable()
{
	m_BackImage.FreeImage();
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
BOOL 	CtmLineTable::CreateA()
{
    if(m_linetable.wType & STYLE_TRANSPARENT)  m_wnd.wStyle |= tmWS_TRANSPARENT;
    CtmWnd::CreateA();
    m_plinetable  = &m_linetable;
    Set();

    return  TRUE;
}

void 	CtmLineTable::SetPropertys()
{
    char    msz[50];
   
    CtmWnd::SetPropertys();

    SetPropInfo("mainlinewidth", 	tmSHORT,	&m_linetable.nMainLineWidth);
	SetPropInfo("stype", 	        tmWORD,		&m_linetable.wType);

	SetPropInfo("maincolor.r",	    tmBYTE,		&m_linetable.tmclMainLine.r);
	SetPropInfo("maincolor.g",	    tmBYTE,		&m_linetable.tmclMainLine.g);
	SetPropInfo("maincolor.b",	    tmBYTE,		&m_linetable.tmclMainLine.b);

	SetPropInfo("accrcolor1.r",	    tmBYTE,		&m_linetable.tmclAccrLine1.r);
	SetPropInfo("accrcolor1.g",	    tmBYTE,		&m_linetable.tmclAccrLine1.g);
	SetPropInfo("accrcolor1.b",	    tmBYTE,		&m_linetable.tmclAccrLine1.b);

	SetPropInfo("accrcolor2.r",	    tmBYTE,		&m_linetable.tmclAccrLine2.r);
	SetPropInfo("accrcolor2.g",	    tmBYTE,		&m_linetable.tmclAccrLine2.g);
	SetPropInfo("accrcolor2.b",	    tmBYTE,		&m_linetable.tmclAccrLine2.b);

	SetPropInfo("vertrows", 	    tmSHORT,	&m_linetable.nVertRows);
	SetPropInfo("horicols", 	    tmSHORT,	&m_linetable.nHoriCols);

    for(short i=0; i<MAX_LT_POINTS; i++)
	{
	    sprintf(msz, "vertpoints%d", i+1);
    	SetPropInfo(msz, 	    tmSHORT,	&m_anVertpoints[i]);
    	sprintf(msz, "horipoints%d", i+1);
	    SetPropInfo(msz, 	    tmSHORT,	&m_anHoripoints[i]);
    }
}

void 	CtmLineTable::SetPropertys_Nub()
{
    char    msz[50];
   
    CtmWnd::SetPropertys_Nub();

    SetPropInfo(LINETABLE_MAINLINEWIDTH, 	tmSHORT,	&m_linetable.nMainLineWidth);
	SetPropInfo(LINETABLE_STYPE, 	        tmWORD,		&m_linetable.wType);

	SetPropInfo(LINETABLE_MAINCOLOR_R,	    tmBYTE,		&m_linetable.tmclMainLine.r);
	SetPropInfo(LINETABLE_MAINCOLOR_G,	    tmBYTE,		&m_linetable.tmclMainLine.g);
	SetPropInfo(LINETABLE_MAINCOLOR_B,	    tmBYTE,		&m_linetable.tmclMainLine.b);

	SetPropInfo(LINETABLE_ACCRCOLOR1_R,	    tmBYTE,		&m_linetable.tmclAccrLine1.r);
	SetPropInfo(LINETABLE_ACCRCOLOR1_G,	    tmBYTE,		&m_linetable.tmclAccrLine1.g);
	SetPropInfo(LINETABLE_ACCRCOLOR1_B,	    tmBYTE,		&m_linetable.tmclAccrLine1.b);

	SetPropInfo(LINETABLE_ACCRCOLOR2_R,	    tmBYTE,		&m_linetable.tmclAccrLine2.r);
	SetPropInfo(LINETABLE_ACCRCOLOR2_G,	    tmBYTE,		&m_linetable.tmclAccrLine2.g);
	SetPropInfo(LINETABLE_ACCRCOLOR2_B,	    tmBYTE,		&m_linetable.tmclAccrLine2.b);

	SetPropInfo(LINETABLE_VERTROWS, 	    tmSHORT,	&m_linetable.nVertRows);
	SetPropInfo(LINETABLE_HORICOLS, 	    tmSHORT,	&m_linetable.nHoriCols);

    for(short i=0; i<MAX_LT_POINTS; i++)
	{
    	SetPropInfo(LINETABLE_VERTPOINTS0 + i*2, 	    tmSHORT,	&m_anVertpoints[i]);
    	SetPropInfo(LINETABLE_HORIPOINTS0 + i*2, 	    tmSHORT,	&m_anHoripoints[i]);
    }
}
/*---------------------------------------------------------------------------+
|  Function :                Get()                                           |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmLineTable::GetColor(tmCOLOR *pLineColor, WORD wIDLine)
{
switch(wIDLine)
    {
    case ID_COLOR_MAINLINE:
        pLineColor = &m_linetable.tmclMainLine;
        break;
    case ID_COLOR_ACCESSORIALLINE1:
        pLineColor = &m_linetable.tmclAccrLine1;
        break;
    case ID_COLOR_ACCESSORIALLINE2:
        pLineColor = &m_linetable.tmclAccrLine2;
        break;
    default:
        pLineColor = NULL;
        break;
    }
}


void CtmLineTable::SetColor(tmCOLOR *pLineColor, WORD wIDLine)
{
switch(wIDLine)
    {
    case ID_COLOR_MAINLINE:
        memcpy(&m_linetable.tmclMainLine , pLineColor, sizeof(tmCOLOR));
        break;
    case ID_COLOR_ACCESSORIALLINE1:
        memcpy(&m_linetable.tmclAccrLine1, pLineColor, sizeof(tmCOLOR));
        break;
    case ID_COLOR_ACCESSORIALLINE2:
        memcpy(&m_linetable.tmclAccrLine2, pLineColor, sizeof(tmCOLOR));
        break;
    default:
        break;
    }
}

void    CtmLineTable::Set()
{
    m_clMainLine  = RGB2PIXEL(m_linetable.tmclMainLine.r, m_linetable.tmclMainLine.g, m_linetable.tmclMainLine.b);
    m_clAccrLine1 = RGB2PIXEL(m_linetable.tmclAccrLine1.r, m_linetable.tmclAccrLine1.g, m_linetable.tmclAccrLine1.b);
    m_clAccrLine2 = RGB2PIXEL(m_linetable.tmclAccrLine2.r, m_linetable.tmclAccrLine2.g, m_linetable.tmclAccrLine2.b);
    m_clDot       = m_clAccrLine1;
}
/*---------------------------------------------------------------------------+
|  Function :                Show()                                          |
|  Task     : Create the items and show them                                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmLineTable::Show()				/* Virtual function */
{
  DWORD     dwclTemp;
	Clear();
  if(m_linetable.nMainLineWidth <= 0)
	{ 
		Clear();
	 	return;
	}
	if((m_linetable.nVertRows <= 1) && (m_linetable.nHoriCols <= 1))
	{
	    dwclTemp      = m_clAccrLine1;
	    m_clAccrLine1 = m_clMainLine;
	    m_clMainLine  = dwclTemp;
	    DrawSingle();
	}
	else    DealFrame();
}

void CtmLineTable::Update()			/* Virtual function */
{
    Set();
    Show();
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void    CtmLineTable::DealFrame()
{
    short       nHeight = m_rectWindow.bottom - m_rectWindow.top;
    short       nWidth  = m_rectWindow.right  - m_rectWindow.left;
    short       nXRemain = 0, nIntervalX = 0;
    short       nYRemain = 0, nIntervalY = 0;
    short       nXPoint  = 0, nYPoint    = 0;
    short       nLines;
    short       i, nSideLineWidth, nLineWidth = m_linetable.nMainLineWidth;
    short       *pStartPoint;

    if((nHeight <= (nLineWidth + 2)) || (nHeight <= (nLineWidth + 2)))  return;
    if(m_linetable.wType & STYLE_SINGLELINE) nSideLineWidth = nLineWidth;
    else                                     nSideLineWidth = nLineWidth + 1;
    // 設定劃線範圍
    _SetViewPort(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
    // draw top vert line and bottom vert line
    if(!(m_linetable.wType & STYLE_LT_NOVERTLINE))
    {
        DrawLine(0,         0, nWidth-1,         0, nSideLineWidth, FLAG_LT_VERTLINE);
        DrawLine(0, nHeight-1-(nSideLineWidth+1), nWidth-1, nHeight-1-(nSideLineWidth+1), nSideLineWidth, FLAG_LT_VERTLINE);
    }
    // draw left hori line and right hori line
    if(!(m_linetable.wType & STYLE_LT_NOHORILINE))
    {
        DrawLine(        0,         0,        0, nHeight-1, nSideLineWidth, FLAG_LT_HORILINE);
        DrawLine( nWidth-1-(nSideLineWidth+1), 0, nWidth-1-(nSideLineWidth+1), nHeight-1, nSideLineWidth, FLAG_LT_HORILINE);
    }
    // draw vert line of title
    if((m_linetable.wType & STYLE_LT_VERTTITLE) && (!(m_linetable.wType & STYLE_LT_NOVERTLINE)) && (m_linetable.nVertRows > 1))
    {
        if((nYPoint = m_anVertpoints[0]-(nSideLineWidth >> 1)) < 0) nYPoint = 0;
        DrawLine( 0, nYPoint, nWidth-1, nYPoint, nSideLineWidth, FLAG_LT_VERTLINE);
    }
    // draw hori line of title
    if((m_linetable.wType & STYLE_LT_HORITITLE) && (!(m_linetable.wType & STYLE_LT_NOHORILINE)) && (m_linetable.nHoriCols > 1))
    {
        if((nXPoint = m_anHoripoints[0]-(nSideLineWidth >> 1)) < 0) nXPoint = 0;
        DrawLine(nXPoint, 0, nXPoint, nHeight-1, nSideLineWidth, FLAG_LT_HORILINE);
    }

    if(!(m_linetable.wType & STYLE_LT_NOVERTLINE)  && (m_linetable.nVertRows > 1))
    {
        // uneven vert line
        if(m_linetable.wType & STYLE_LT_VERTUNEVEN)
        {
            if(m_linetable.wType & STYLE_LT_VERTTITLE)
            {
                pStartPoint = &m_anVertpoints[1];
                nLines      = m_linetable.nVertRows - 1;
            }
            else
            {
                pStartPoint = &m_anVertpoints[0];
                nLines      = m_linetable.nVertRows;
            }
            for(i=0; i<(nLines - 1); i++)
            {
                if(*(pStartPoint+i) != -1)
                {
                    nYPoint += *(pStartPoint+i);
                    DrawLine( 0, nYPoint-(nLineWidth>>1), nWidth-1, nYPoint-(nLineWidth>>1), nLineWidth, FLAG_LT_VERTLINE);
                }
            }
        }
        else
        {
            if(m_linetable.wType & STYLE_LT_VERTTITLE)
            {
                nIntervalY = (nHeight-nYPoint)/(m_linetable.nVertRows - 1);
                nYRemain   = (nHeight-nYPoint)%(m_linetable.nVertRows - 1);
                nLines     = m_linetable.nVertRows - 1;
            }
            else
            {
                nIntervalY = nHeight/(m_linetable.nVertRows);
                nYRemain   = nHeight%(m_linetable.nVertRows);
                nLines     = m_linetable.nVertRows;
            }
            for(i=0; i<(nLines - 1); i++)
            {
                nYPoint += nIntervalY;
                if(nYRemain > 0)
                {
                    nYRemain--;
                    nYPoint++;
                }
                DrawLine( 0, nYPoint-(nLineWidth>>1), nWidth-1, nYPoint-(nLineWidth>>1), nLineWidth, FLAG_LT_VERTLINE);
            }
        }
    }

    if(!(m_linetable.wType & STYLE_LT_NOHORILINE) && (m_linetable.nHoriCols > 1))
    {
        // uneven hori line
        if(m_linetable.wType & STYLE_LT_HORIUNEVEN)
        {
            if(m_linetable.wType & STYLE_LT_HORITITLE)
            {
                pStartPoint = &m_anHoripoints[1];
                nLines      = m_linetable.nHoriCols - 1;
            }
            else
            {
                pStartPoint = &m_anHoripoints[0];
                nLines      = m_linetable.nHoriCols;
            }
            for(i=0; i<(nLines - 1); i++)
            {
                if(*(pStartPoint+i) != -1)
                {
                    nXPoint += *(pStartPoint+i);
                    DrawLine(nXPoint-(nLineWidth>>1), 0, nXPoint-(nLineWidth>>1), nHeight-1, nLineWidth, FLAG_LT_HORILINE);
                }
            }
        }
        else
        {
            if(m_linetable.wType & STYLE_LT_HORITITLE)
            {
                nIntervalX = (nWidth - nXPoint)/(m_linetable.nHoriCols - 1);
                nXRemain   = (nWidth - nXPoint)%(m_linetable.nHoriCols - 1);
                nLines     = m_linetable.nHoriCols - 1;
            }
            else
            {
                nIntervalX = nWidth/(m_linetable.nHoriCols);
                nXRemain   = nWidth%(m_linetable.nHoriCols);
                nLines     = m_linetable.nHoriCols;
            }
            for(i=0; i<(nLines - 1); i++)
            {
                nXPoint += nIntervalX;
                if(nXRemain > 0)
                {
                    nXRemain--;
                    nXPoint++;
                }
                DrawLine(nXPoint-(nLineWidth>>1), 0, nXPoint-(nLineWidth>>1), nHeight-1, nLineWidth, FLAG_LT_HORILINE);
            }
        }
    }
}

void    CtmLineTable::DrawLine(short left, short top, short right, short bottom, short nLineWidth, WORD wType)
{
    short   i;

    if((left < 0) || (top < 0) || (right < 0) || (bottom < 0))  return;
    _SetColor(m_clAccrLine1);
     if(wType & FLAG_LT_VERTLINE)   _Line(left+4, top, right-4, bottom);
     if(wType & FLAG_LT_HORILINE)   _Line(left, top+4, right, bottom - 4);
     if(wType==(FLAG_LT_HORILINE|STYLE_LT_PCORNERLINE))
     {	
     	
     	_Line(left,top+4,left+4,top);
     	_Line(left,bottom-4,left+4,bottom);
     }
     if(wType==(FLAG_LT_VERTLINE|STYLE_LT_NCORNERLINE))	_Line(right-4,top,right,top+4);
     if(wType==(FLAG_LT_HORILINE|STYLE_LT_NCORNERLINE)) _Line(right+nLineWidth,bottom+nLineWidth-4,right+nLineWidth-3,bottom+nLineWidth-1);
     
   	 //if(wType==(FLAG_LT_VERTLINE|STYLE_LT_PCORNERLINE)) 
    _SetColor(m_clMainLine);
    for(i=0; i<nLineWidth; i++)
    {
    	if(wType==(FLAG_LT_HORILINE|STYLE_LT_PCORNERLINE))
    	{	
    		 _Line(left+i+1,bottom-4,left+4,bottom-i-1);
    		 _Line(left+i+1,top+4,left+4,top+i+1);
    	} 
    	if(wType==(FLAG_LT_VERTLINE|STYLE_LT_NCORNERLINE)) _Line(right-4,top+i+1,right-i-1,top+4);
        if(wType==(FLAG_LT_HORILINE|STYLE_LT_NCORNERLINE)) _Line(left+nLineWidth-i-1,bottom+nLineWidth-4,left+nLineWidth-4,bottom+nLineWidth-i-1);
        if(wType&0x00C0)
        {
        	if(wType & FLAG_LT_VERTLINE)  _Line(left+4, top+i+1, right-4, bottom+i+1);
        	if(wType & FLAG_LT_HORILINE)  _Line(left+i+1, top+4, right+i+1, bottom-4);
        }
        else
        {
        	 if(wType & FLAG_LT_VERTLINE)   _Line(left+1, top+i+1, right-1, bottom+i+1);
        	 if(wType & FLAG_LT_HORILINE)   _Line(left+i+1, top+1, right+i+1, bottom-1);	
        }
    }
    _SetColor(m_clAccrLine2);
    if(wType & FLAG_LT_VERTLINE)   _Line(left+4, top+nLineWidth+1, right-4, bottom+nLineWidth+1);
    if(wType & FLAG_LT_HORILINE)   _Line(left+nLineWidth+1, top+4, right+nLineWidth+1, bottom - 4);
    if(wType==(FLAG_LT_HORILINE|STYLE_LT_PCORNERLINE))
    {
    	  _Line(left+nLineWidth+1,bottom-4,left+4,bottom-nLineWidth-1);
    	  _Line(left+nLineWidth+1,top+4,left+4,top+nLineWidth+1);
    }
    if(wType==(FLAG_LT_VERTLINE|STYLE_LT_NCORNERLINE))	_Line(right-4,top+nLineWidth+1,right-nLineWidth-1,top+4);
    if(wType==(FLAG_LT_HORILINE|STYLE_LT_NCORNERLINE))  _Line(right-1,bottom+nLineWidth-4,right+nLineWidth-4,bottom-1);
}

void    CtmLineTable::DrawSingle()
{
	short       nHeight = m_rectWindow.bottom - m_rectWindow.top;
	short       nWidth  = m_rectWindow.right  - m_rectWindow.left;
	short       nLineWidth = m_linetable.nMainLineWidth;

	if((nHeight <= (nLineWidth + 2)) || (nHeight <= (nLineWidth + 2)))  return;
	_SetViewPort(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);

	if(STYLE_CORNERTYPE&m_linetable.wType)
	{
		// draw left
		if((m_linetable.wType & STYLE_NOTSHOW_LEFT) == 0)
		{
		    DrawLine( 0, 0, 0, nHeight-1, nLineWidth, FLAG_LT_HORILINE|STYLE_LT_PCORNERLINE);
		}
		
		// draw top
		if((m_linetable.wType & STYLE_NOTSHOW_TOP) == 0)
		{
		    DrawLine(0, 0, nWidth-1,  0, nLineWidth, FLAG_LT_VERTLINE|STYLE_LT_NCORNERLINE);
		}
		
		// draw bottom
		if((m_linetable.wType & STYLE_NOTSHOW_RIGHT) == 0)
		{
		    DrawLine(0, nHeight-1-(nLineWidth+1), nWidth-1, nHeight-1-(nLineWidth+1), nLineWidth, FLAG_LT_VERTLINE|STYLE_LT_PCORNERLINE);
		}
		
		// draw right
		if((m_linetable.wType & STYLE_NOTSHOW_BOTTOM) == 0)
		{
		    DrawLine( nWidth-1-(nLineWidth+1), 0, nWidth-1-(nLineWidth+1), nHeight-1, nLineWidth, FLAG_LT_HORILINE|STYLE_LT_NCORNERLINE);
		}
	}
	else
	{
		// draw left
		if((m_linetable.wType & STYLE_NOTSHOW_LEFT) == 0)
		{
		    DrawLine( 0, 0, 0, nHeight-1, nLineWidth, FLAG_LT_HORILINE);
		}
		
		// draw top
		if((m_linetable.wType & STYLE_NOTSHOW_TOP) == 0)
		{
		    DrawLine(0, 0, nWidth-1,  0, nLineWidth, FLAG_LT_VERTLINE);
		}
		
		// draw bottom
		if((m_linetable.wType & STYLE_NOTSHOW_RIGHT) == 0)
		{
		    DrawLine(0, nHeight-1-(nLineWidth+1), nWidth-1, nHeight-1-(nLineWidth+1), nLineWidth, FLAG_LT_VERTLINE);
		}
		
		// draw right
		if((m_linetable.wType & STYLE_NOTSHOW_BOTTOM) == 0)
		{
		    DrawLine( nWidth-1-(nLineWidth+1), 0, nWidth-1-(nLineWidth+1), nHeight-1, nLineWidth, FLAG_LT_HORILINE);
		}
	}
}

void    CtmLineTable::Clear()
{
        if (!m_bBackGround) 
        {
        		m_BackImage.FreeImage();
            m_BackImage.CopyImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right,
                              m_rectWindow.bottom);
            m_bBackGround = TRUE; 
        }
        else
        {
            m_BackImage.ShowImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right,
                              m_rectWindow.bottom);
        }
}
