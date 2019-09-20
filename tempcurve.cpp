/*===========================================================================+
|  Class    : View Temperature2                                              |
|  Task     : View Temperature2                                              |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                      |
|  Version  : V1.00                                                          |
|  Creation : 08/23/2005                                                     |
|  Revision : V1.10                                                          |
+===========================================================================*/
#include    "tempcurve.h"
#include 	"main.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		TMEP_UPDATECOUNTER		(1024*3)										//<<yang 2005/12/5 
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
long		lTempUpdateCounter	=	TMEP_UPDATECOUNTER;									//<<yang 2005/12/5 
/*---------------------------------------------------------------------------+
|           View Content - ViewTemperature2                                  |
+---------------------------------------------------------------------------*/
#define     WFONT               8
#define     HFONT               16
/*
static      CFONT               cf = {HFONT, FONT_PLAIN};
*/
#define     HSTNEW              22            
#define     HEDNEW              21            
#define     DeltaH              23 
#define     NWGrid              10

/*******
|       Initialize static class member
*******/
WORD         CtmTempCurve::m_wStatic[] = {
            VW_TEMP_TEMPRANKSELECT

        };
tmSTATIC       CtmTempCurve::m_static[] = {
            {NULL/*g_pStr[VW_TEMP_TEMPRANKSELECT     ]*/}
            };

tmCURVE       CtmTempCurve::m_curve[] = 
{
    {
    	g_pStr[VW_TEMP_TIMEUNIT], 
    	g_pStr[VW_TEMP_TEMPUNIT],
    	g_pStr[VW_TEMP_SECUNIT], 
    	g_pStr[VW_TEMP_CUNIT], 
    	200, TRUE,0,3,70,30,200, 0, 100, 10, 0
    },
    {
    	g_pStr[VW_TEMP_TIMEUNIT], 
    	g_pStr[VW_TEMP_TEMPUNIT],
    	g_pStr[VW_TEMP_SECUNIT], 
    	g_pStr[VW_TEMP_CUNIT], 
    	200, TRUE,0,3,70,30,200, 0, 100, 10, 0
    },
    {
    	g_pStr[VW_TEMP_TIMEUNIT], 
    	g_pStr[VW_TEMP_TEMPUNIT],
    	g_pStr[VW_TEMP_SECUNIT], 
    	g_pStr[VW_TEMP_CUNIT], 
    	200, TRUE,0,3,70,30,200, 0, 100, 10, 0
	},
    {
    	g_pStr[VW_TEMP_TIMEUNIT], 
    	g_pStr[VW_TEMP_TEMPUNIT],
    	g_pStr[VW_TEMP_SECUNIT], 
    	g_pStr[VW_TEMP_CUNIT], 
    	200, TRUE,0,3,70,30,200, 0, 100, 10, 0
	}
};

tmEDITX1       CtmTempCurve::m_editx1[] = {
            {0  , 1, 1, 4  			, g_pStr[VW_TEMP_TEMPRSCAPTION		]}
};



tmDIALOG_1     CtmTempCurve::m_dlg_1[] = {
            {tmDLG_STATICX2 ,{tmWS_BORDER_NONE|tmWS_TRANSPARENT, {32, 17,WST_10+30,HSTNEW}}},

            {tmDLG_EDITX1   ,{tmWS_BORDER_PLAIN|tmWS_TRANSPARENT, {145+56*0,  15,WED_2+30  ,HEDNEW}}},

			{tmDLG_CURVE    ,{tmWS_TRANSPARENT, {20		,60  	,370	,200}}},
			{tmDLG_CURVE    ,{tmWS_TRANSPARENT, {400	,60  	,370	,200}}},
			{tmDLG_CURVE    ,{tmWS_TRANSPARENT, {20 	,280  	,370	,200}}},
			{tmDLG_CURVE    ,{tmWS_TRANSPARENT, {400 	,280  	,370	,200}}},
			
			{tmDLG_END}
			
            };
/*===========================================================================+
|           Class implementation - ViewTemperature2                          |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmTempCurve::CtmTempCurve(tmWND* pwnd) : CtmFormView()
{
	short       i;
	
	m_nRankIndex		= 0;
	AddTimer(&lTempUpdateCounter);
	CtmDialog::Make(m_dlg, m_dlg_1);
	for (i = 0		  ; i < ID_MAX_STATIC  	; i++) m_static[i].psz 	= g_pStr[m_wStatic[i]];
	for (i = IDS_BEGIN; i < IDS_END			; i++) m_dlg[i].pobj 	= &m_static[i-IDS_BEGIN];
	for (i = IDE_BEGIN; i < IDE_END			; i++) m_dlg[i].pobj 	= &m_editx1[i-IDE_BEGIN];
	for (i = IDC_BEGIN; i < IDC_END			; i++) m_dlg[i].pobj 	= &m_curve[i-IDC_BEGIN];
	
	Set();
	if (m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue == 4)
	{
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE3].pobj))->VisibleFlag = FALSE;
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE4].pobj))->VisibleFlag = FALSE;
	}
	CtmFormView::Create(pwnd, m_dlg);
	CaptionShow();
	
	((tmCURVE*)(m_dlg[IDC_TEMPCURVE3].pobj))->VisibleFlag = TRUE;
	((tmCURVE*)(m_dlg[IDC_TEMPCURVE4].pobj))->VisibleFlag = TRUE;
}

CtmTempCurve::~CtmTempCurve()
{
	DeleteTimer(&lTempUpdateCounter);
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void    CtmTempCurve::CaptionShow()
{
	
	_SetViewPort(m_rectClient.left, m_rectClient.top, \
		m_rectClient.right, m_rectClient.bottom);

	CtmWnd::ShowExtent(240, 22, g_pStr[VW_TEMP_SETTEMPCURVE]);
	_SetColor(GREEN);
	_Rectangle(_GFILLINTERIOR, 330, 70, 350, 80);
	
	CtmWnd::ShowExtent(370, 22, g_pStr[VW_TEMP_TEMPCURVEMAX]);
	_SetColor(RED);
	_Rectangle(_GFILLINTERIOR, 460, 70, 480, 80);

	CtmWnd::ShowExtent(500, 22, g_pStr[VW_TEMP_TEMPCURVEMIN]);
	_SetColor(BLUE);
	_Rectangle(_GFILLINTERIOR, 590, 70, 610, 80);
	
	CtmWnd::ShowExtent(630, 22, g_pStr[VW_TEMP_REALTEMPCURVE]);
	_SetColor(YELLOW);
	_Rectangle(_GFILLINTERIOR, 720, 70, 740, 80);
	
	switch(m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue)
	{
		case 1:
			CtmWnd::ShowExtent(160, 60 , g_pStr[VW_TEMP_REALTEMP1]);
			CtmWnd::ShowExtent(550, 60 , g_pStr[VW_TEMP_REALTEMP2]);
			CtmWnd::ShowExtent(160, 280, g_pStr[VW_TEMP_REALTEMP3]);
			CtmWnd::ShowExtent(550, 280, g_pStr[VW_TEMP_REALTEMP4]);
			break;
		case 2:
			CtmWnd::ShowExtent(160, 60 , g_pStr[VW_TEMP_REALTEMP5]);
			CtmWnd::ShowExtent(550, 60 , g_pStr[VW_TEMP_REALTEMP6]);
			CtmWnd::ShowExtent(160, 280, g_pStr[VW_TEMP_REALTEMP7]);
			CtmWnd::ShowExtent(550, 280, g_pStr[VW_TEMP_REALTEMP8]);
			break;
		case 3:
			CtmWnd::ShowExtent(160, 60 , g_pStr[VW_TEMP_REALTEMP9]);
			CtmWnd::ShowExtent(550, 60 , g_pStr[VW_TEMP_REALTEMP10]);
			CtmWnd::ShowExtent(160, 280, g_pStr[VW_TEMP_REALTEMP11]);
			CtmWnd::ShowExtent(550, 280, g_pStr[VW_TEMP_REALTEMP12]);
			break;
		case 4:
			CtmWnd::ShowExtent(160, 60 , g_pStr[VW_TEMP_REALTEMP13]);
			CtmWnd::ShowExtent(550, 60 , g_pStr[VW_TEMP_REALTEMP14]);
			break;		
	} 
}

void	CtmTempCurve::ReadInterface()
{
	int i = 0;
	char szID[200];
	
	for(i = 0; i < 21; i ++)
	{
		sprintf(szID, "TECH_SETTEMP_NSETTING_%d", i + 1);
		m_AllInterface[i]	= g_pDatabase->Read(szID).nData;
		if(m_AllInterface[i] <= 0 || m_AllInterface[i] >= 999)
			m_AllInterface[i] = 0xFFFF;
	}
}

void    CtmTempCurve::Set()
{
	TASKDBARRAY templog;
	tmCURVE		*pCurve	= m_curve;
	char		szID[200];	
	tmDB_TEMPT	temp	= g_ptaskDBLog->m_TempInfo;
	int i;	
	
	m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue = m_nRankIndex + 1;
	ReadInterface();

	for(int j = m_nRankIndex * 4; j < m_nRankIndex * 4 + 4; j++)
	{
		for(i = 0; i < 10; i ++)
			pCurve->CurveLine[i].nCount	= 0;
		
		if ((j < 14) && (m_AllInterface[j] != 0xffff))
		{
			templog = g_ptaskDBLog->ReadData(j);
			((tmCURVE*)(m_dlg[j + IDC_BEGIN - m_nRankIndex * 4].pobj))->VisibleFlag = TRUE;
		}
		else
		{
			templog = g_ptaskDBLog->ReadData(j);
			((tmCURVE*)(m_dlg[j + IDC_BEGIN- m_nRankIndex * 4].pobj))->VisibleFlag = FALSE;
			continue;
		}

		pCurve->ShowPointCount = templog.MaxNum;
//		//printf("CtmTempCurve::Set m_curve[%d].ShowPointCount=%d\n",j, pCurve->ShowPointCount);
		
		int nBeeLineCount = 0;
		//=======
		// 設定值上限
		//=======
		sprintf(szID, "TECH_SETTEMP_NMAX_%d", j + 1);
		pCurve->Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0xFF, 0x00, 0x00);
		pCurve->Beeline[nBeeLineCount].nValue	= g_pDatabase->Read(szID).nData;
		nBeeLineCount ++;
		
		//======
		// 設定值下限
		//======
		sprintf(szID, "TECH_SETTEMP_NMIN_%d",j + 1);
		pCurve->Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0x00, 0x00, 0xFF);
		pCurve->Beeline[nBeeLineCount].nValue	= g_pDatabase->Read(szID).nData;
		nBeeLineCount ++;

		int		nCurveLine	= 0;		
		//=======
		// 設定曲線
		//=======
		for(i = 0; i < templog.MaxNum; i ++)
		{
			pCurve->CurveLine[nCurveLine].nValue[i]	= templog.SettingValue[i];
			if (pCurve->AxisY_MINVALUE > pCurve->CurveLine[nCurveLine].nValue[i])
				pCurve->AxisY_MINVALUE = pCurve->CurveLine[nCurveLine].nValue[i];
		}
		pCurve->CurveLine[nCurveLine].dwColor	= RGB2PIXEL(0x00,0xFF,0x00);
		pCurve->CurveLine[nCurveLine].nCount		= templog.MaxNum;
		nCurveLine ++;		
		
		//=======
		// 實際曲線
		//=======
		for(i = 0; i < templog.MaxNum; i ++)
		{
			pCurve->CurveLine[nCurveLine].nValue[i]	= templog.DBArray[i];
			if (pCurve->AxisY_MAXVALUE < pCurve->CurveLine[nCurveLine].nValue[i])
				pCurve->AxisY_MAXVALUE = pCurve->CurveLine[nCurveLine].nValue[i];
		}
		pCurve->CurveLine[nCurveLine].dwColor	= RGB2PIXEL(0xFF,0xFF,0x00);
		pCurve->CurveLine[nCurveLine].nCount		= templog.MaxNum;
		nCurveLine ++;
		
		if(temp.wStandardEv[j] != 0xFFFF && temp.wStandardEv[j] != 0)
		{
			if(temp.wStandardEv[j] % 100 != 0)	temp.wStandardEv[j] += 100;
			pCurve->Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0xEE, 0x22, 0xEE);
			pCurve->Beeline[nBeeLineCount].nValue	= pCurve->Setting_Value[0] + temp.wStandardEv[j]  / 100;
			nBeeLineCount ++;
			pCurve->Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0xEE, 0xCC, 0x33);
			pCurve->Beeline[nBeeLineCount].nValue	= pCurve->Setting_Value[0] - temp.wStandardEv[j]  / 100;
			nBeeLineCount ++;
		}
		pCurve->nBeeLineCount	= nBeeLineCount;
		
		pCurve ++;
	}
}

void        CtmTempCurve::Update()
{
	if (lTempUpdateCounter<=0)
		{
//		dbmmiconfig.cTempCurveRankSelect = m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue;

		Set();
		
		if (m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue < 4)
		{		
			m_dlg[IDC_TEMPCURVE1].wType = tmWS_TRANSPARENT;
			m_dlg[IDC_TEMPCURVE2].wType = tmWS_TRANSPARENT;
			m_dlg[IDC_TEMPCURVE3].wType = tmWS_TRANSPARENT;
			m_dlg[IDC_TEMPCURVE4].wType = tmWS_TRANSPARENT;
		}	
		
		if (m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue == 4)
		{
			((tmCURVE*)(m_dlg[IDC_TEMPCURVE3].pobj))->VisibleFlag = FALSE;
			((tmCURVE*)(m_dlg[IDC_TEMPCURVE4].pobj))->VisibleFlag = FALSE;
		}
	
		GetControl(IDC_TEMPCURVE1)->Update(); 
		GetControl(IDC_TEMPCURVE2)->Update();
		GetControl(IDC_TEMPCURVE3)->Update();
		GetControl(IDC_TEMPCURVE4)->Update();
		
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE1].pobj))->VisibleFlag = TRUE;
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE2].pobj))->VisibleFlag = TRUE;
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE3].pobj))->VisibleFlag = TRUE;
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE4].pobj))->VisibleFlag = TRUE;
		CaptionShow();
		lTempUpdateCounter	= TMEP_UPDATECOUNTER;
		}
}



WORD        CtmTempCurve::OnChange(WORD wIDControl)
{
	m_nRankIndex = m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue - 1;
	
	Set();
	if (m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue < 4)
	{		
		m_dlg[IDC_TEMPCURVE1].wType = tmWS_TRANSPARENT;
		m_dlg[IDC_TEMPCURVE2].wType = tmWS_TRANSPARENT;
		m_dlg[IDC_TEMPCURVE3].wType = tmWS_TRANSPARENT;
		m_dlg[IDC_TEMPCURVE4].wType = tmWS_TRANSPARENT;
	}	
	
	if (m_editx1[IDE_RANKSELECT-IDE_BEGIN].lValue == 4)
	{
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE3].pobj))->VisibleFlag = FALSE;
		((tmCURVE*)(m_dlg[IDC_TEMPCURVE4].pobj))->VisibleFlag = FALSE;
	}

	GetControl(IDC_TEMPCURVE1)->Update(); 
	GetControl(IDC_TEMPCURVE2)->Update();
	GetControl(IDC_TEMPCURVE3)->Update();
	GetControl(IDC_TEMPCURVE4)->Update();
	
	((tmCURVE*)(m_dlg[IDC_TEMPCURVE1].pobj))->VisibleFlag = TRUE;
	((tmCURVE*)(m_dlg[IDC_TEMPCURVE2].pobj))->VisibleFlag = TRUE;
	((tmCURVE*)(m_dlg[IDC_TEMPCURVE3].pobj))->VisibleFlag = TRUE;
	((tmCURVE*)(m_dlg[IDC_TEMPCURVE4].pobj))->VisibleFlag = TRUE;
	CaptionShow();
	
	return 0;
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
