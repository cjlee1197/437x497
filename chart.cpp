/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"chart.h"
#include	"utils.h"

//short       CtmChart::m_anScale[10] = {1,2,5,5,5,10,10,10,10,10};
short       CtmChart::m_anScale[20] = {1,2,2,5,5,5,8,8,10,10,10,12,12,15,15,15,18,18,20,20};//netdigger 2005/12/2

/*===========================================================================+
|           Class implementation - Chart                                     |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmChart::CtmChart(tmWND* pwnd, tmCHART* pchart) : CtmWnd()
{
SetBorderWidth(m_wndattr.wBorderWidthChart);
m_pchart = pchart;
CtmWnd::Create(pwnd);
	
m_Image.CopyImage(m_rectClient.left, m_rectClient.top, \
	m_rectClient.right, m_rectClient.bottom);

ScaleNumber();                          // Must be called before call ScaleCategory()
ScaleCategory();

if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
    CtmChart::Show();
}

CtmChart::~CtmChart()
{
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : Show()                                                         |
|  Task     : Show a chart in this window                                    |
+----------------------------------------------------------------------------+
|  Call     : CtmChart::Show()                                                 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmChart::Show()              /* Virtual function */
{	
tmCHART       chart = *m_pchart;          // Move date member to stack to speed up the process

short       i;
short       j;
char        sz[8];                      // 1 sign + 5 digits + 1 terminator '\0'
char        szFormat[tmMAX_TEXT_COL+1];
WORD        wCharWidth;                 // Character's width in pixel
WORD        wCharHeight_Half;           // Half of character's height in pixel

BOOL        bVertical = m_bVertical;
short       nBarChart = m_nBarChart;
short       nXYChart  = m_nXYChart;

tmCHARTSCALE  scaleNAxis = m_scaleNAxis;
tmCHARTSCALE  scaleCAxis = m_scaleCAxis;
tmCHARTPOS    posNAxis;                   // Position parameter for number axis
tmCHARTPOS    posCAxis;                   // Position parameter for category axis

WORD        wLineStyleMask;
WORD        wLineStyleMaskOld = _GetLineStyle();
short       nScale;
long        lDelta;

#define     OVER_LIMIT          0       // 4
short       nN;                         // Current chart point on number axis
short       nC;                         // Current chart point on category axis
short       nX;                         // X direction
short       nY;                         // Y direction

wCharWidth = wCharHeight_Half = GetFont()->m_wHeight >> 1;
/****************************************************************************+
|       Clear plotting area and/or scale number area                         |
+****************************************************************************/
if (!(chart.wTypeCategoryAxis & tmCHART_TRANSPARENT))
    {
    	_SetViewPort(m_rectClient.left, m_rectClient.top, \
			m_rectClient.right, m_rectClient.bottom);
		m_Image.ShowImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, \
			m_rectWindow.bottom);	
    	/*
    if (chart.nMaxNumberAxis == chart.nMinNumberAxis) Clear();
    else                                              Clear(chart.rectChart);
        */
    _SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
    _SetColor(m_wndattr.bgcChart);                                               //ST081099 FOR tmCHART BACKGROUND COLOR
    _Rectangle(_GFILLINTERIOR, chart.rectChart.left,  chart.rectChart.top, chart.rectChart.right, chart.rectChart.bottom);
    _SetColor(m_wndattr.fgcChartScale1);
    _Rectangle(_GBORDER, chart.rectChart.left,  chart.rectChart.top, chart.rectChart.right, chart.rectChart.bottom);
    }

/****************************************************************************+
|       Vertical chart                                                       |
+****************************************************************************/
if (bVertical)
    {
    //******
    //      Number axis preprocess
    //******
    if (chart.wTypeNumberAxis & tmCHART_AXIS_REVERSE) // If the number axis is from large to small
        {
        nY = 1;
        posNAxis.lScale = ((long)chart.rectChart.top) << tmCHART_SCALEFACTOR;
        lDelta = scaleNAxis.lDelta;
        }
    else
        {
        nY = -1;
        posNAxis.lScale = ((long)chart.rectChart.bottom) << tmCHART_SCALEFACTOR;
        lDelta = -scaleNAxis.lDelta;
        }
        
    if (chart.wTypeNumberAxis & tmCHART_AXIS_ONUR)    // If the number axis is on upper right side
        {
        posNAxis.nScaleBegin = chart.rectChart.right;
        posNAxis.nScaleEnd   = chart.rectChart.right-4;

        posNAxis.nScaleTextBegin = chart.rectChart.right+2;
        strcpy(szFormat, "%d");;
        }
    else
        {
        posNAxis.nScaleBegin = chart.rectChart.left;
        posNAxis.nScaleEnd   = chart.rectChart.left+4;

        i = CountDigit((WORD)scaleNAxis.nMax) + 1;  // Add 1 due to nMax may overflow from 99.. to 100..
        posNAxis.nScaleTextBegin = chart.rectChart.left-2-wCharWidth*i;
        sprintf(szFormat, "%%%dd", i);              // szFormat shall be "%wd"
        }

    if (chart.wTypeNumberAxis & tmCHART_AXIS_GRID)
        {
        posNAxis.nScaleBegin = chart.rectChart.left;
        posNAxis.nScaleEnd   = chart.rectChart.right;
        
        wLineStyleMask = 0xCCCC;
        }
    else
        wLineStyleMask = 0xFFFF;

    //******
    //      Plot Number axis
    //******
    if (!(chart.wTypeCategoryAxis & tmCHART_TRANSPARENT)) /*goto CCHART_PLOT_VCHART;*/
        {
        sprintf(sz, szFormat, i = scaleNAxis.nMin);
        CtmWnd::Show(posNAxis.nScaleTextBegin, (short)(posNAxis.lScale >> tmCHART_SCALEFACTOR)-wCharHeight_Half, sz);
    
		_SetLineStyle(wLineStyleMask);
        _SetColor(m_wndattr.fgcChartScale2);
        while ((i += scaleNAxis.nDelta) < scaleNAxis.nMax)
            {
            posNAxis.lScale += lDelta;
            nScale = (short)(posNAxis.lScale >> tmCHART_SCALEFACTOR);
            _MoveTo(posNAxis.nScaleBegin+1, nScale);
            _LineTo(posNAxis.nScaleEnd-1,   nScale);
            
            //******
            //      Scale number
            //******
            sprintf(sz, szFormat, i);
            CtmWnd::Show(posNAxis.nScaleTextBegin, nScale-wCharHeight_Half, sz);
            }
		_SetLineStyle(wLineStyleMaskOld);
    
        if (i <= scaleNAxis.nMax)
            {
            sprintf(sz, szFormat, i);
            CtmWnd::Show(posNAxis.nScaleTextBegin, (short)((posNAxis.lScale + lDelta) >> tmCHART_SCALEFACTOR)-wCharHeight_Half, sz);
            }
        }

    //******
    //      Category axis preprocess
    //******
    if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
        {
        nX = -1;
        posCAxis.lScale = ((long)chart.rectChart.right) << tmCHART_SCALEFACTOR;
        lDelta = -scaleCAxis.lDelta;
        if (nXYChart == 0)
            posCAxis.lScale -= scaleCAxis.lBarWidth >> 1;   // There is 1 lBarWidth gap between the first bar and the number axis
        }                                                   // The starting scale is 1/2 lBarWidth on category axis
    else
        {
        nX = 1;
        posCAxis.lScale = ((long)chart.rectChart.left) << tmCHART_SCALEFACTOR;
        lDelta = scaleCAxis.lDelta;
        if (nXYChart == 0)
            posCAxis.lScale += scaleCAxis.lBarWidth >> 1;
        }
        
    if (chart.wTypeCategoryAxis & tmCHART_AXIS_ONUR)
        {
        posCAxis.nScaleBegin = chart.rectChart.top;
        posCAxis.nScaleEnd   = chart.rectChart.top+4;
        }
    else
        {
        posCAxis.nScaleBegin = chart.rectChart.bottom;
        posCAxis.nScaleEnd   = chart.rectChart.bottom-4;
        }

    if (chart.wTypeCategoryAxis & tmCHART_AXIS_GRID)
        {
        posCAxis.nScaleBegin = chart.rectChart.bottom;
        posCAxis.nScaleEnd   = chart.rectChart.top;
        
        wLineStyleMask = 0xCCCC;
        }
    else
        wLineStyleMask = 0xFFFF;

    //******
    //      Plot Category axis
    //******
    if (!(chart.wTypeCategoryAxis & tmCHART_TRANSPARENT)) /*goto CCHART_PLOT_VCHART;*/
        {
		_SetLineStyle(wLineStyleMask);
        _SetColor(m_wndattr.fgcChartScale2);
        if (nXYChart != 0)
            {
            i = scaleCAxis.nMin;
            while ((i += scaleCAxis.nDelta) < scaleCAxis.nMax)
                {
                posCAxis.lScale += lDelta;
                nScale = (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                _MoveTo(nScale, posCAxis.nScaleBegin-1);
                _LineTo(nScale, posCAxis.nScaleEnd+1);
                }
            }
        else
        {
            for (j=1; j<(short)chart.wDataCategory; j++)
            {
                posCAxis.lScale += lDelta;
                nScale = (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                _MoveTo(nScale, posCAxis.nScaleBegin-1);
                _LineTo(nScale, posCAxis.nScaleEnd+1);
            }
        }
		_SetLineStyle(wLineStyleMaskOld);
        }

    /*******
    |       Plot Vertical chart
    +******/
//  CCHART_PLOT_VCHART:

	if (chart.wTypeNumberAxis & tmCHART_AXIS_REVERSE)
        {
        _SetClipRgn(m_rectClient.left + chart.rectChart.left,
                    m_rectClient.top  + chart.rectChart.top,
                    m_rectClient.left + chart.rectChart.right,
                    m_rectClient.top  + chart.rectChart.bottom + OVER_LIMIT); 
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            _SetViewOrg(m_rectClient.left + chart.rectChart.right, m_rectClient.top + chart.rectChart.top);
        else
            _SetViewOrg(m_rectClient.left + chart.rectChart.left,  m_rectClient.top + chart.rectChart.top);
        }
    else
        {
        _SetClipRgn(m_rectClient.left + chart.rectChart.left,
                    m_rectClient.top  + chart.rectChart.top - OVER_LIMIT,
                    m_rectClient.left + chart.rectChart.right,
                    m_rectClient.top  + chart.rectChart.bottom);
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            _SetViewOrg(m_rectClient.left + chart.rectChart.right, m_rectClient.top + chart.rectChart.bottom);
        else
            _SetViewOrg(m_rectClient.left + chart.rectChart.left,  m_rectClient.top + chart.rectChart.bottom);
        }

	if (chart.wTypeNumberAxis & tmCHART_AXIS_REVERSE)
        {
        _SetViewPort(m_rectClient.left + chart.rectChart.left,
                    m_rectClient.top  + chart.rectChart.top,
                    m_rectClient.left + chart.rectChart.right,
                    m_rectClient.top  + chart.rectChart.bottom + OVER_LIMIT); 
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            _SetViewOrg(m_rectClient.left + chart.rectChart.right, m_rectClient.top + chart.rectChart.top);
        else
            _SetViewOrg(m_rectClient.left + chart.rectChart.left,  m_rectClient.top + chart.rectChart.top);
        }
    else
        {
        _SetViewPort(m_rectClient.left + chart.rectChart.left,
                    m_rectClient.top  + chart.rectChart.top - OVER_LIMIT,
                    m_rectClient.left + chart.rectChart.right,
                    m_rectClient.top  + chart.rectChart.bottom);
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            _SetViewOrg(m_rectClient.left + chart.rectChart.right, m_rectClient.top + chart.rectChart.bottom);
        else
            _SetViewOrg(m_rectClient.left + chart.rectChart.left,  m_rectClient.top + chart.rectChart.bottom);
        }

    for (i=0; (WORD)i<chart.wDataSeries; i++)
        {
        _SetColor(*(chart.pfgc+i));        
        switch (*(chart.pnType+i) & 0x0FFF)
            {
            case tmCHART_LINE:
                posCAxis.lScale = scaleCAxis.lBarWidth * (i+1/*Gap*/) + (scaleCAxis.lBarWidth >> 1);
                nN = nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                nC = nX * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                _Ellipse(_GFILLINTERIOR, nC-2, nN-2, nC+2, nN+2);
                _MoveTo(nC, nN);
                for (j=1; (WORD)j<chart.wDataCategory; j++)
                    {
                    posCAxis.lScale += scaleCAxis.lDelta;
                    nN = nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nX * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                    _Ellipse(_GFILLINTERIOR, nC-2, nN-2, nC+2, nN+2);
                    _LineTo(nC, nN);
                    }
                break;
            case tmCHART_DOT:
                posCAxis.lScale = scaleCAxis.lBarWidth * (i+1/*Gap*/) + (scaleCAxis.lBarWidth >> 1);
                for (j=0; (WORD)j<chart.wDataCategory; j++)
                    {
                    nN = nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nX * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                    _Rectangle(_GFILLINTERIOR, nC-2, nN-2, nC+2, nN+2);
                    posCAxis.lScale += scaleCAxis.lDelta;
                    }
                break;
            case tmCHART_XYLINE:            	       	
            	nN = nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                nC = nX * (short)(scaleCAxis.lRatio * (*((short*)chart.pDescDataCategory) - scaleCAxis.nMin) >> tmCHART_SCALEFACTOR);
                _MoveTo(nC, nN);
                for (j=1; (WORD)j<chart.wDataCategory; j++)
                    {
                    nN = nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nX * (short)(scaleCAxis.lRatio * (*((short*)chart.pDescDataCategory+j) - scaleCAxis.nMin) >> tmCHART_SCALEFACTOR);
                    _LineTo(nC, nN);
                    }
				break;
            case tmCHART_XYDOT:
                for (j = 0; (WORD)j < chart.wDataCategory; j++)
                {
                    nN = nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nX * (short)(scaleCAxis.lRatio * (*((short*)chart.pDescDataCategory+j) - scaleCAxis.nMin) >> tmCHART_SCALEFACTOR);
                    _Ellipse(_GFILLINTERIOR, nC-2, nN-2, nC+2, nN+2);
                }
                    
                break;
            default:
                posCAxis.lScale = scaleCAxis.lBarWidth * (i+1/*Gap*/);
                for (j=0; (WORD)j<chart.wDataCategory; j++)
                    { 
                    _Rectangle(_GFILLINTERIOR,
                               nX * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR),
                               nY * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR),
                               nX * (short)((posCAxis.lScale + scaleCAxis.lBarWidth) >> tmCHART_SCALEFACTOR),
                               nY * (short)(1/*Not plot on the axis*/));
                    posCAxis.lScale += scaleCAxis.lDelta;
                    }
                break;
            }
        }
    }
/****************************************************************************+
|       Horizontal chart                                                     |
+****************************************************************************/
else
    {
    //******
    //      Number axis preprocess
    //******
    if (chart.wTypeNumberAxis & tmCHART_AXIS_REVERSE) // If the number axis is from large to small
        {
        nX = -1;
        posNAxis.lScale = ((long)chart.rectChart.right) << tmCHART_SCALEFACTOR;
        lDelta = -scaleNAxis.lDelta;
        }
    else
        {
        nX = 1;
        posNAxis.lScale = ((long)chart.rectChart.left) << tmCHART_SCALEFACTOR;
        lDelta = scaleNAxis.lDelta;
        }
        
    if (chart.wTypeNumberAxis & tmCHART_AXIS_ONUR)    // If the number axis is on upper right side
        {
        posNAxis.nScaleBegin = chart.rectChart.top;
        posNAxis.nScaleEnd   = chart.rectChart.top+4;

        posNAxis.nScaleTextBegin = chart.rectChart.top-2-(wCharHeight_Half<<1);
        strcpy(szFormat, "%d");;
        }
    else
        {
        posNAxis.nScaleBegin = chart.rectChart.bottom;
        posNAxis.nScaleEnd   = chart.rectChart.bottom-4;

        posNAxis.nScaleTextBegin = chart.rectChart.bottom+2;
        strcpy(szFormat, "%d");
        }

    if (chart.wTypeNumberAxis & tmCHART_AXIS_GRID)
        {
        posNAxis.nScaleBegin = chart.rectChart.bottom;
        posNAxis.nScaleEnd   = chart.rectChart.top;
        
        wLineStyleMask = 0xCCCC;
        }
    else
        wLineStyleMask = 0xFFFF;

    //******
    //      Plot Number axis
    //******
    if (!(chart.wTypeCategoryAxis & tmCHART_TRANSPARENT)) /*goto CCHART_PLOT_HCHART;*/
        {
        sprintf(sz, szFormat, i = scaleNAxis.nMin);
        CtmWnd::Show((short)(posNAxis.lScale >> tmCHART_SCALEFACTOR)-wCharWidth*strlen(sz)/2, posNAxis.nScaleTextBegin, sz);
    
		_SetLineStyle(wLineStyleMask);
        _SetColor(m_wndattr.fgcChartScale2);
        
        while ((i += scaleNAxis.nDelta) < scaleNAxis.nMax)
        {
            posNAxis.lScale += lDelta;
            nScale = (short)(posNAxis.lScale >> tmCHART_SCALEFACTOR);
            _MoveTo(nScale, posNAxis.nScaleBegin);
            _LineTo(nScale, posNAxis.nScaleEnd);
            //******
            //      Scale number
            //******
            sprintf(sz, szFormat, i);
            CtmWnd::Show(nScale-wCharWidth*strlen(sz)/2, posNAxis.nScaleTextBegin, sz);
        }
		_SetLineStyle(wLineStyleMaskOld);
    
        if (i <= scaleNAxis.nMax)
            {
            sprintf(sz, szFormat, i);
            CtmWnd::Show((short)((posNAxis.lScale + lDelta) >> tmCHART_SCALEFACTOR)-wCharWidth*strlen(sz)/2, posNAxis.nScaleTextBegin, sz);
            }
        }

    //******
    //      Category axis preprocess
    //******
    if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
        {
        nY = 1;
        posCAxis.lScale = ((long)chart.rectChart.top) << tmCHART_SCALEFACTOR;
        lDelta = scaleCAxis.lDelta;
        if (nXYChart == 0)
            posCAxis.lScale += scaleCAxis.lBarWidth >> 1;
        }
    else
        {
        nY = -1;
        posCAxis.lScale = ((long)chart.rectChart.bottom) << tmCHART_SCALEFACTOR;
        lDelta = -scaleCAxis.lDelta;
        if (nXYChart == 0)
            posCAxis.lScale -= scaleCAxis.lBarWidth >> 1;
        }
        
    if (chart.wTypeCategoryAxis & tmCHART_AXIS_ONUR)
        {
        posCAxis.nScaleBegin = chart.rectChart.right;
        posCAxis.nScaleEnd   = chart.rectChart.right-4;
        }
    else
        {
        posCAxis.nScaleBegin = chart.rectChart.left;
        posCAxis.nScaleEnd   = chart.rectChart.left+4;
        }

    if (chart.wTypeCategoryAxis & tmCHART_AXIS_GRID)
        {
        posCAxis.nScaleBegin = chart.rectChart.left;
        posCAxis.nScaleEnd   = chart.rectChart.right;
        
        wLineStyleMask = 0xCCCC;
        }
    else
        wLineStyleMask = 0xFFFF;

    //******
    //      Plot Category axis
    //******
    if (!(chart.wTypeCategoryAxis & tmCHART_TRANSPARENT)) /*goto CCHART_PLOT_HCHART;*/
        {
		_SetLineStyle(wLineStyleMask);
        _SetColor(m_wndattr.fgcChartScale3);
        if (nXYChart != 0)
            {
            i = scaleCAxis.nMin;
            while ((i += scaleCAxis.nDelta) < scaleCAxis.nMax)
                {
                posCAxis.lScale += lDelta;
                nScale = (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                _MoveTo(posCAxis.nScaleBegin, nScale);
                _LineTo(posCAxis.nScaleEnd,   nScale);
                }
            }
        else
            {
            for (j=1; j<(short)chart.wDataCategory; j++)
                {
                posCAxis.lScale += lDelta;
                nScale = (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                _MoveTo(posCAxis.nScaleBegin, nScale);
                _LineTo(posCAxis.nScaleEnd,   nScale);
                }
            }
		_SetLineStyle(wLineStyleMaskOld);
        }

    /*******
    |       Plot Horizontal chart
    +******/
//  CCHART_PLOT_HCHART:

if (chart.wTypeNumberAxis & tmCHART_AXIS_REVERSE)
        {
        _SetClipRgn(m_rectClient.left + chart.rectChart.left - OVER_LIMIT,
                    m_rectClient.top  + chart.rectChart.top,
                    m_rectClient.left + chart.rectChart.right,
                    m_rectClient.top  + chart.rectChart.bottom);
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            _SetViewOrg(m_rectClient.left + chart.rectChart.right, m_rectClient.top + chart.rectChart.top);
        else
            _SetViewOrg(m_rectClient.left + chart.rectChart.right, m_rectClient.top + chart.rectChart.bottom);
        }
    else
        {
        _SetClipRgn(m_rectClient.left + chart.rectChart.left,
                    m_rectClient.top  + chart.rectChart.top,
                    m_rectClient.left + chart.rectChart.right + OVER_LIMIT,
                    m_rectClient.top  + chart.rectChart.bottom);
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            _SetViewOrg(m_rectClient.left + chart.rectChart.left, m_rectClient.top + chart.rectChart.top);
        else
            _SetViewOrg(m_rectClient.left + chart.rectChart.left, m_rectClient.top + chart.rectChart.bottom);
        }

    for (i=0; (WORD)i<chart.wDataSeries; i++)
        {
        if (chart.wTypeCategoryAxis & tmCHART_AXIS_REVERSE)
            {
            }
        else
            {
            posNAxis.lScale = 0;
            posCAxis.lScale = (((long)scaleCAxis.nPixel) << tmCHART_SCALEFACTOR) - scaleCAxis.lBarWidth * (i+1);
            }

        _SetColor(*(chart.pfgc+i));
        switch (*(chart.pnType+i) & 0x0FFF)
            {
            case tmCHART_LINE:
                posCAxis.lScale = scaleCAxis.lBarWidth * (i+1/*Gap*/) + (scaleCAxis.lBarWidth >> 1);
                nN = nX * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                nC = nY * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                _Ellipse(_GFILLINTERIOR, nN-2, nC-2, nN+2, nC+2);
                _MoveTo(nN, nC);
                for (j=1; (WORD)j<chart.wDataCategory; j++)
                    {
                    posCAxis.lScale += scaleCAxis.lDelta;
                    nN = nX * (short)(posNAxis.lScale + scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nY * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                    _Ellipse(_GFILLINTERIOR, nN-2, nC-2, nN+2, nC+2);
                    _LineTo(nN, nC);
                    }
                break;
            case tmCHART_DOT:
                posCAxis.lScale = scaleCAxis.lBarWidth * (i+1/*Gap*/) + (scaleCAxis.lBarWidth >> 1);
                for (j=0; (WORD)j<chart.wDataCategory; j++)
                    {
                    nN = nX * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nY * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR);
                    _Rectangle(_GFILLINTERIOR, nN-2, nC-2, nN+2, nC+2);
                    posCAxis.lScale += scaleCAxis.lDelta;
                    }
                break;
            case tmCHART_XYLINE:
                nN = nX * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                nC = nY * (short)(scaleCAxis.lRatio * (*((short*)chart.pDescDataCategory) - scaleCAxis.nMin) >> tmCHART_SCALEFACTOR);
                _MoveTo(nN, nC);
                for (j=1; (WORD)j<chart.wDataCategory; j++)
                    {
                    nN = nX * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nY * (short)(scaleCAxis.lRatio * (*((short*)chart.pDescDataCategory+j) - scaleCAxis.nMin) >> tmCHART_SCALEFACTOR);
                    _LineTo(nN, nC);
                    }
                break;
            case tmCHART_XYDOT:
                for (j=0; (WORD)j<chart.wDataCategory; j++)
                    {
                    nN = nX * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR);
                    nC = nY * (short)(scaleCAxis.lRatio * (*((short*)chart.pDescDataCategory+j) - scaleCAxis.nMin) >> tmCHART_SCALEFACTOR);
                    _Ellipse(_GFILLINTERIOR, nN-2, nC-2, nN+2, nC+2);
                    }
                break;
//          case tmCHART_BAR:
            default:
                posCAxis.lScale = scaleCAxis.lBarWidth * (i+1/*Gap*/);
                for (j=0; (WORD)j<chart.wDataCategory; j++)
                    {
                    _Rectangle(_GFILLINTERIOR,
                               nX * (short)(1/*Not plot on the axis*/),
                               nY * (short)((posCAxis.lScale + scaleCAxis.lBarWidth) >> tmCHART_SCALEFACTOR),
                               nX * (short)(scaleNAxis.lRatio * (*(*(chart.ppnValue+i)+j) - scaleNAxis.nMin) >> tmCHART_SCALEFACTOR),
                               nY * (short)(posCAxis.lScale >> tmCHART_SCALEFACTOR));
                    posCAxis.lScale += scaleCAxis.lDelta;
                    }
                break;
            }
        }
    }
}

void        CtmChart::Update()            /* Virtual function */
{
if ((m_pchart->nMaxNumberAxis == m_pchart->nMinNumberAxis) ||
    (m_scaleNAxis.nMin != m_pchart->nMinNumberAxis)        ||
    (m_scaleNAxis.nMax != m_pchart->nMaxNumberAxis))
    ScaleNumber();
if ((m_pchart->nMaxCategoryAxis == m_pchart->nMinCategoryAxis) ||
    (m_scaleCAxis.nMin != m_pchart->nMinCategoryAxis)          || 
    (m_scaleCAxis.nMax != m_pchart->nMaxCategoryAxis))
    ScaleCategory();

CtmChart::Show();
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void         CtmChart::CountChart()
{
	short       i;
	
	m_nBarChart =
	m_nXYChart  = 0;
	for (i=0; i<(short)m_pchart->wDataSeries; i++)
	    if      (*(m_pchart->pnType+i) & tmCHART_BAR) m_nBarChart++;
	    else if (*(m_pchart->pnType+i) & tmCHART_XY)  m_nXYChart++;
}

void         CtmChart::ScaleNumber()
{
	tmCHART       chart = *m_pchart;
	short       i;
	short       j;
	WORD        wCharWidth;                 // Character's width in pixel
	WORD        wCharHeight;                // Character's height in pixel
	
	wCharHeight = GetFont()->m_wHeight;
	wCharWidth  = wCharHeight >> 1;
	//******
	//      Auto scaling
	//******
	if (chart.nMaxNumberAxis == chart.nMinNumberAxis)
	    {
	    m_scaleNAxis.nMin = 0;
	    m_scaleNAxis.nMax = 0;
	    for (i=0; (WORD)i<chart.wDataSeries; i++)
	        for (j=0; (WORD)j<chart.wDataCategory; j++)
	        {
	        if (*(*(chart.ppnValue+i)+j) > m_scaleNAxis.nMax) 
	        	m_scaleNAxis.nMax = *(*(chart.ppnValue+i)+j);
	        }
	    m_scaleNAxis.nMax = (m_scaleNAxis.nMax / 10 + 1) * 10;          // Advance to number of 10
	    }
	else
	    {
	    m_scaleNAxis.nMin = chart.nMinNumberAxis;
	    m_scaleNAxis.nMax = chart.nMaxNumberAxis;
	    }
	
	//******
	//      Calculate the value to pixel conversion ratio and the scale mark on number axis
	//******
	if (m_bVertical = (*chart.pnType > 0))
	    {
	    m_scaleNAxis.nPixel = chart.rectChart.bottom - chart.rectChart.top;
	    m_scaleCAxis.nPixel = chart.rectChart.right - chart.rectChart.left;
	    j = wCharHeight;                                                // Minimum gap between vertical scale mark is character's height
	    }
	else
	    {
	    m_scaleNAxis.nPixel = chart.rectChart.right - chart.rectChart.left;
	    m_scaleCAxis.nPixel = chart.rectChart.bottom - chart.rectChart.top;
	    j = wCharWidth * CountDigit((WORD)m_scaleNAxis.nMax);           // Minimum gap between horizontal scale mark is the width of scale number
	    }
	
	if (m_scaleNAxis.nMax <= m_scaleNAxis.nMin) 
		m_scaleNAxis.nMax = m_scaleNAxis.nMin + 1;
	
	m_scaleNAxis.lRatio = (((long)m_scaleNAxis.nPixel) << tmCHART_SCALEFACTOR) / (m_scaleNAxis.nMax - m_scaleNAxis.nMin);
	m_scaleNAxis.nDelta = (m_scaleNAxis.nMax - m_scaleNAxis.nMin) / (m_scaleNAxis.nPixel / j);
	
	//******
	//      Round the scale delta to 1, 2, 5, 10, 20, 50, 100 ...
	//******
	i = (short)Power(10, CountDigit((WORD)m_scaleNAxis.nDelta)-1);              // Find the power increment of scale is 10 or 100 or ...
	m_scaleNAxis.nDelta = m_anScale[(m_scaleNAxis.nDelta+i-1)/((i>1)?i-1:1)]*i; // Find the number increment of scale is 1 or 2 or 3 or ... then convert to a fixed increment
	m_scaleNAxis.lDelta = m_scaleNAxis.nDelta * m_scaleNAxis.lRatio;
}

void         CtmChart::ScaleCategory()
{
tmCHART       chart = *m_pchart;
short       i;

CountChart();
if (m_nXYChart != 0)
    {
    //******
    //      Auto scaling
    //******
    if (chart.nMaxCategoryAxis == chart.nMinCategoryAxis)
        {
        m_scaleCAxis.nMin =
        m_scaleCAxis.nMax = *((short*)chart.pDescDataCategory);
        for (i=1; i<(short)chart.wDataCategory; i++)
            {
            if      (*((short*)chart.pDescDataCategory+i) < m_scaleCAxis.nMin) 
            	m_scaleCAxis.nMin = *((short*)chart.pDescDataCategory+i);
            else if (m_scaleCAxis.nMax < *((short*)chart.pDescDataCategory+i)) 
            	m_scaleCAxis.nMax = *((short*)chart.pDescDataCategory+i);
            }
        m_scaleCAxis.nMax = (m_scaleCAxis.nMax / 10 + 1) * 10;      // Advance to number of 10
        }
    else
        {
        m_scaleCAxis.nMin = chart.nMinCategoryAxis;
        m_scaleCAxis.nMax = chart.nMaxCategoryAxis;
        }

    //******
    //      Calculate the value to pixel conversion ratio and the scale mark on category axis
    //******
    if (m_scaleCAxis.nMax <= m_scaleCAxis.nMin) m_scaleCAxis.nMax = m_scaleCAxis.nMin + 1;
    m_scaleCAxis.lRatio = (((long)m_scaleCAxis.nPixel) << tmCHART_SCALEFACTOR) / (m_scaleCAxis.nMax - m_scaleCAxis.nMin);
    m_scaleCAxis.nDelta = (m_scaleCAxis.nMax - m_scaleCAxis.nMin) / (m_scaleCAxis.nPixel / GetFont()->m_wHeight);
	//******
    //      Round the scale delta to 1, 2, 5, 10, 20, 50, 100 ...
    //******
    i = (short)Power(10, CountDigit((WORD)m_scaleCAxis.nDelta)-1);              // Find the power increment of scale is 10 or 100 or ...
	m_scaleCAxis.nDelta = m_anScale[(m_scaleCAxis.nDelta+i-1)/((i>1)?i-1:1)]*i; // Find the number increment of scale is 1 or 2 or 3 or ... then convert to a fixed increment
    m_scaleCAxis.lDelta = m_scaleCAxis.nDelta * m_scaleCAxis.lRatio;
    }
else
    {
    //******
    //      Calculate the width, gap and scale mark on category axis for non-XY chart
    //******
    m_scaleCAxis.lBarWidth = ((long)m_scaleCAxis.nPixel << tmCHART_SCALEFACTOR) / ((chart.wDataSeries + 1/*Gap*/) * chart.wDataCategory + 1/*Gap*/);
    m_scaleCAxis.lDelta    = m_scaleCAxis.lBarWidth * (chart.wDataSeries + 1/*Gap*/);
    }
}
