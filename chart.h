/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef	D_CHART
#define	D_CHART
#include	"wnd.h"
#include	"wndx2.h"

//******
//      Chart
//******
#define     tmCHART_BAR           0x0001
#define     tmCHART_LINE          0x0002
#define     tmCHART_DOT           0x0004
#define     tmCHART_XYLINE        0x0020
#define     tmCHART_XYDOT         0x0040
#define     tmCHART_XY            0x0060
#define     tmCHART_V_BAR         0x0001
#define     tmCHART_V_LINE        0x0002
#define     tmCHART_V_DOT         0x0004
#define     tmCHART_V_XYLINE      0x0020
#define     tmCHART_V_XYDOT       0x0040
#define     tmCHART_H_BAR         0x8001
#define     tmCHART_H_LINE        0x8002
#define     tmCHART_H_DOT         0x8004
#define     tmCHART_H_XYLINE      0x8020
#define     tmCHART_H_XYDOT       0x8040

#define     tmCHART_AXIS_REVERSE  0x0001  // Scale of axis is from large to small
#define     tmCHART_AXIS_ONUR     0x0010  // Position of scale if on upper right side
#define     tmCHART_AXIS_GRID     0x0100  // Full grid line on chart area
#define     tmCHART_TRANSPARENT   0x8000

#define     tmCHART_MAX_SERIES    8
#define     tmCHART_MAX_CATEGORY  16
#define     tmCHART_SCALEFACTOR   15     // 2**15 = 32768

typedef struct tagtmCHART
    {
    RECT        rectChart;              // Chart area in the window
    WORD        wDataSeries;            // Number of data series
    WORD        wDataCategory;          // Number of data category or data point
//  char*       pszDescDataSeries;      // Description of each data series
    void*       pDescDataCategory;      // Description of each data category
//  char*       pszLabelNumberAxis;     // Label of number axis
//  char*       pszLabelCategoryAxis;   // Label of category axis
    short*      pnType;                 // Type of each data series, must be all vertical or all horizontal
    FGC*        pfgc;                   // Color of each data series
    short**     ppnValue;               // Data value --- Current version does not support negative number
    short       nMinNumberAxis;         // Minimum scale in number axis
    short       nMaxNumberAxis;         // Maximum scale in number axis, it will auto scale if Minimum == Maximum
    short       nMinCategoryAxis;       // Minimum scale in category axis
    short       nMaxCategoryAxis;       // Maximum scale in category axis, it will auto scale if Minimum == Maximum

    WORD        wTypeNumberAxis;        // Type of number axis
    WORD        wTypeCategoryAxis;      // Type of category axis
    } tmCHART;

typedef struct tagtmCHARTSCALE
    {
    short       nMin;                   // Minimum of scale
    short       nMax;                   // Maximum of scale
    short       nPixel;                 // Number of pixel between chart area
    short       nDelta;                 // Delta between each scale
    long        lDelta;                 // Scaled (<<CHART_SCALEFACTOR) delta
    long        lRatio;                 // Ratio of scaled (<<CHART_SCALEFACTOR) nPixel to (nMax - nMin)
    long        lBarWidth;
    } tmCHARTSCALE;

typedef struct tagtmCHARTPOS
    {
    long        lScale;
    short       nScaleBegin;
    short       nScaleEnd;
    short       nScaleTextBegin;
    } tmCHARTPOS;


/*===========================================================================+
|           Class declaration - Chart                                        |
+===========================================================================*/
class CtmChart : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmChart(tmWND* pwnd, tmCHART* pchart);
    ~CtmChart();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    static short    	m_anScale[20];//netdigger 2005/12/2
    tmCHART*          	m_pchart;

    BOOL            	m_bVertical;
    short           	m_nBarChart;
    short           	m_nXYChart;

    tmCHARTSCALE      	m_scaleNAxis;       // Scale parameter for number axis
    tmCHARTSCALE      	m_scaleCAxis;       // Scale parameter for category axis
   	ImageBackground 	m_Image;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Show();
/**/void    Update();

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void     CountChart();
    void     ScaleNumber();
    void     ScaleCategory();
};

#endif
