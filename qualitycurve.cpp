/*===========================================================================+
|  Class    : View CtmQualityCurve                                           |
|  Task     : View CtmQualityCurve                                           |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeff.Xiao                                                      |
|  Version  : V1.00                                                          |
|  Creation : 08/23/2005                                                     |
|  Revision : V1.10                                                          |
+===========================================================================*/
#include    "qualitycurve.h"
#include 	"main.h"
#include	"taski32.h"
#include	"edit.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		QUALITY_UPDATECOUNTER		(1024*5)										//<<yang 2005/12/5 
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
long		lQualityUpdateCounter	=	QUALITY_UPDATECOUNTER;									//<<yang 2005/12/5 
extern		I32_MONILIMIT	g_MoniLimit;
extern		CtmRecordFile	g_MoniFile;
extern		I32_MONI		g_monisig;

#define		MoniMax()	g_MoniLimit.MoniMax
#define		MoniMin()	g_MoniLimit.MoniMin
/*---------------------------------------------------------------------------+
|           View Content - ViewTemperature2                                  |
+---------------------------------------------------------------------------*/
#define     WFONT               8
#define     HFONT               16

#define     HSTNEW              22            
#define     HEDNEW              21            
#define     DeltaH              23 
#define     NWGrid              10

/*******
|       Initialize static class member
*******/
WORD		g_awSelectStatic[] = {
		VW_MONI1_CYCLE,			
		VW_MONI1_MOLDCLS,		
		VW_MONI1_MOLDOPN,		
		VW_MONI1_INJECTIME,		
		VW_MONI1_CHARGE,		
		VW_MONI1_SUCKBACKTTIME,	
		VW_MONI1_EJECT,			
		VW_MONI1_MOLDOPNEND,	
		VW_MONI1_VTOPSW,		
		VW_MONI1_VTOPSW,		
		VW_MONI1_VTOPSW,		
		VW_MONI1_INJSTART,
		VW_MONI1_INJEND
};

WORD		g_awSelectPormpt[] = {
		VM_QUALITY_CIRCLETIMING_HELP,					
		VM_QUALITY_CLOCLAMPLPRETIMING_HELP,			
		VM_QUALITY_CLOCLAMPHPRETIMING_HELP,			
		VM_QUALITY_CLOCLAMPTIMING_HELP,				
		VM_QUALITY_OPENCLAMPPOSITION_HELP,				
		VM_QUALITY_OPENCLAMPTIMING_HELP,				
		VM_QUALITY_DOFFCLAMPTIMING_HELP,				
		VM_QUALITY_SHOTOUTTIMING_HELP,					
		VM_QUALITY_KEEPPRECONVERSEPRE_HELP,			
		VM_QUALITY_KEEPPRECONVERSETIMING_HELP,			
		VM_QUALITY_KEEPPRECONVERSEPOSITION_HELP,		
		VM_QUALITY_SHOTOUTSTART_HELP,					
		VM_QUALITY_SHOTOUTEND_HELP
};

WORD         CtmQualityCurve::m_wStatic[] = {
            VM_QUALITY_SELECTCURVE
        };
tmSTATIC       CtmQualityCurve::m_static[] = {
            {NULL	/*g_pStr[VW_TEMP_TEMPRANKSELECT     ]*/}
            };

tmCURVE       CtmQualityCurve::m_curve[] = {
           {g_pStr[VM_QUALITY_CLAMPUNIT],g_pStr[VW_TEMP_TIMEUNIT], \
           	g_pStr[VM_QUALITY_CLAMPUNIT], g_pStr[VW_TEMP_SECUNIT], \
           	50, TRUE,50,1,0,0,50, 0, 50, 0, 0
           	}
            };

tmSELECT       CtmQualityCurve::m_selectbox[] = {
			{1  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL},
			{0  , NULL, NULL}
			};

tmDIALOG_1     CtmQualityCurve::m_dlg_1[] = {
            {tmDLG_STATICX2 ,{tmWS_BORDER_NONE|tmWS_TRANSPARENT,{  32, 17,WST_10+46,HSTNEW}}},

            {tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*0 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*1 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*2 ,WED_8+30  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*3 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*4 ,WED_8+30  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*5 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*6 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*7 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*8 ,WED_8+46  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*9 ,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*10,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*11,WED_8+15  ,HEDNEW}}},
			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*12,WED_8+15  ,HEDNEW}}},
//			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*13,WED_8+15  ,HEDNEW}}},
//			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*14,WED_8+46  ,HEDNEW}}},
//			{tmDLG_SELECT   ,{tmWS_TRANSPARENT,{28,  50+(HST+5)*15,WED_8+46  ,HEDNEW}}},
			
			{tmDLG_CURVE   ,{tmWS_TRANSPARENT ,{150 ,60  ,630,400}}},
			{tmDLG_END}
            };
//static      WORD*       pdbctrl = &dbmset.daTemp_Setting1;
/*===========================================================================+
|           Class implementation - ViewTemperature2                          |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmQualityCurve::CtmQualityCurve(tmWND* pwnd) : CtmFormView()
{
	short       i;
	
	AddTimer(&lQualityUpdateCounter);
	m_KeyFlag = IDE_BEGIN;
	for (int i = IDE_BEGIN; i <= IDE_END; i++)
	{
		if (m_selectbox[i - IDE_BEGIN].bSelect == TRUE)
			m_KeyFlag = i;
	}
	ReadData(m_KeyFlag);
	
	CtmDialog::Make(m_dlg, m_dlg_1);
	for (i=0; i<ID_MAX_STATIC  ; i++) m_static[i].psz = g_pStr[m_wStatic[i]];
	for (i=0  ;i<ID_MAX_SELECT  ; i++) 
	    { 
	    m_selectbox[i].psz       = g_pStr[g_awSelectStatic[i]];
	    m_selectbox[i].pszPrompt = g_pStr[g_awSelectPormpt[i]];
	    }
	for (i=IDS_BEGIN; i<IDS_END; i++) m_dlg[i].pobj   = &m_static[i-IDS_BEGIN];
	for (i=IDE_BEGIN; i<IDE_END; i++) m_dlg[i].pobj   = &m_selectbox[i-IDE_BEGIN];
	for (i=IDC_BEGIN; i<IDC_END; i++) m_dlg[i].pobj   = &m_curve[i-IDC_BEGIN];

	Set();
	CtmFormView::Create(pwnd, m_dlg);
	CaptionShow();
}

CtmQualityCurve::~CtmQualityCurve()
{
	DeleteTimer(&lQualityUpdateCounter);
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	CtmQualityCurve::CaptionShow()
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
	
	
}

void        CtmQualityCurve::Set()
{
}

void        CtmQualityCurve::Update()
{
	if (lQualityUpdateCounter<=0)
		{
		ReadData(m_KeyFlag);
		GetControl(IDC_QUALITYCURVE)->Update();
		CaptionShow();
		lQualityUpdateCounter	= QUALITY_UPDATECOUNTER;
		}
}

void	CtmQualityCurve::ReadData(int Index)
{
	int i;
	int AverageValue;
	WORD MAXValue = 0;
	WORD MINValue = 0xffff;
	int	Sum = 0;
	WORD	wCount;
	DWORD	wShootIndex = 0;
	int			nIndex;
	double		nSig;
	I32_MONI 	m_tmpData;
	MONITOR		ReadMoni;
	int			nBeeLineCount	= 0;

	wCount		= g_ptaski32->GetMoniCount();
	nIndex		= g_ptaski32->GetMoniCount();
	
	m_curve[0].Beeline[0].dwColor	= RGB2PIXEL(0xFF, 0x00, 0x00);
	m_curve[0].Beeline[1].dwColor	= RGB2PIXEL(0x00, 0x00, 0xFF);
	switch(Index)
	{
		case IDE_SELECTBOX1:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = MoniMax().tmCycleTime;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = MoniMin().tmCycleTime;
			nSig					  = g_monisig.tmCycleTime;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX2:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = MoniMax().tmClampClose;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = MoniMin().tmClampClose;
			nSig					  = g_monisig.tmClampClose;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX3:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMax().tmClampOpen;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMin().tmClampOpen;
			nSig					  = g_monisig.tmClampOpen;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX4:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = MoniMax().tmInject;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = MoniMin().tmInject;
			nSig					  = g_monisig.tmInject;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX5:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMax().tmCharge;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMin().tmCharge;
			nSig					  = g_monisig.tmCharge;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX6:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMax().tmSuckBack;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMin().tmSuckBack;
			nSig					  = g_monisig.tmSuckBack;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX7:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMax().tmEject;
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)MoniMin().tmEject;
			nSig					  = g_monisig.tmEject;
			m_curve[0].nAxisY_Precision	= 3;
			break;
		case IDE_SELECTBOX8:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMax().adPosi_ClampOpenEnd * 100);
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMin().adPosi_ClampOpenEnd * 100);
			nSig					  = g_monisig.adPosi_ClampOpenEnd;
			m_curve[0].nAxisY_Precision	= 2;
			break;
		case IDE_SELECTBOX9:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMax().daPres_TurnToHold * 100);
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMin().daPres_TurnToHold * 100);
			nSig					  = g_monisig.daPres_TurnToHold;
			m_curve[0].nAxisY_Precision	= 2;
			break;
		case IDE_SELECTBOX10:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMax().tmTurnToHold * 100);
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMin().tmTurnToHold * 100);
			nSig					  = g_monisig.tmTurnToHold;
			m_curve[0].nAxisY_Precision	= 2;
			break;
		case IDE_SELECTBOX11:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMax().adPosi_TurnToHold * 100);
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMin().adPosi_TurnToHold * 100);
			nSig					  = g_monisig.adPosi_TurnToHold;
			m_curve[0].nAxisY_Precision	= 2;
			break;
		case IDE_SELECTBOX12:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMax().adPosi_InjectStart * 100);
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMin().adPosi_InjectStart * 100);
			nSig					  = g_monisig.adPosi_InjectStart;
			m_curve[0].nAxisY_Precision	= 2;
			break;
		case IDE_SELECTBOX13:
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMax().adPosi_InjectHoldEnd * 100);
			m_curve[0].Beeline[nBeeLineCount ++].nValue = (int)(MoniMin().adPosi_InjectHoldEnd * 100);
			nSig					  = g_monisig.adPosi_InjectHoldEnd;
			m_curve[0].nAxisY_Precision	= 2;
			break;
	}
	
	if(wCount < tmMAX_MONITOR)	nIndex	= 1;
	if (wCount != 0)
	{
		g_MoniFile.ReadRecord(nIndex + 2, &ReadMoni);
		wShootIndex	= ReadMoni.dwShotCount;
		for (i = 0; i < wCount; i++)
		{
			g_MoniFile.ReadRecord(nIndex + 2, &ReadMoni);
			m_tmpData	= ReadMoni.Moni;
			if(++nIndex >= tmMAX_MONITOR)	 nIndex  = 0;
				
			switch(Index)
			{
				case IDE_SELECTBOX1:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmCycleTime;
					break;
				case IDE_SELECTBOX2:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmClampClose;
					break;
				case IDE_SELECTBOX3:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmClampOpen;
					break;
				case IDE_SELECTBOX4:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmInject;
					break;
				case IDE_SELECTBOX5:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmCharge;
					break;
				case IDE_SELECTBOX6:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmSuckBack;
					break;
				case IDE_SELECTBOX7:
					m_curve[0].CurveLine[0].nValue[i] = m_tmpData.tmEject;
					break;
				case IDE_SELECTBOX8:
					m_curve[0].CurveLine[0].nValue[i] = (int)(m_tmpData.adPosi_ClampOpenEnd * 100);
					break;
				case IDE_SELECTBOX9:
					m_curve[0].CurveLine[0].nValue[i] = (int)(m_tmpData.daPres_TurnToHold * 100);
					break;
				case IDE_SELECTBOX10:
					m_curve[0].CurveLine[0].nValue[i] = (int)(m_tmpData.tmTurnToHold * 100);
					break;
				case IDE_SELECTBOX11:
					m_curve[0].CurveLine[0].nValue[i] = (int)(m_tmpData.adPosi_TurnToHold * 100);
					break;
				case IDE_SELECTBOX12:
					m_curve[0].CurveLine[0].nValue[i] = (int)(m_tmpData.adPosi_InjectStart * 100);
					break;
				case IDE_SELECTBOX13:
					m_curve[0].CurveLine[0].nValue[i] = (int)(m_tmpData.adPosi_InjectHoldEnd * 100);
					break;
			}
			
			Sum += m_curve[0].CurveLine[0].nValue[i];
		}
		m_curve[0].CurveLine[0].nCount	= wCount;
		m_curve[0].CurveLine[0].dwColor	= RGB2PIXEL(0xFF, 0xFF, 0x00);
		
//		//printf("MAXValue = %d	MINValue = %d\n", MAXValue, MINValue);
		if (Sum)
			AverageValue = Sum / wCount;
		else
			AverageValue = 0;
	
//		m_curve[0].Curve_MAXVALUE = MAXValue;
//		m_curve[0].Curve_MINVALUE = MINValue;
		
		m_curve[0].AxisX_MAXVALUE = wShootIndex + tmMAX_MONITOR;
		m_curve[0].AxisX_MINVALUE = wShootIndex;
		
		m_curve[0].AxisY_MAXVALUE = MAXValue;
		m_curve[0].AxisY_MINVALUE = MINValue;
		m_curve[0].Max_Count		= wCount;
		m_curve[0].ShowPointCount	= wCount;
		
		m_curve[0].Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0x00, 0xFF, 0x00);
		m_curve[0].Beeline[nBeeLineCount ++].nValue	= AverageValue;
		
		if(nSig	!= 0 && nSig != 0xFFFF && AverageValue != 0)
		{
//			//printf("AverageValue = %d, nSig=%f\n", AverageValue, nSig);
			m_curve[0].Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0xEE, 0x22, 0xEE);
			m_curve[0].Beeline[nBeeLineCount ++].nValue	= (int)(AverageValue + nSig);
			m_curve[0].Beeline[nBeeLineCount].dwColor	= RGB2PIXEL(0xEE, 0xCC, 0x33);
			m_curve[0].Beeline[nBeeLineCount ++].nValue	= (int)AverageValue - nSig;
		}
		m_curve[0].nBeeLineCount			= nBeeLineCount;

	}
	else
	{
		m_curve[0].ShowPointCount = 0;
		m_curve[0].AxisY_MAXVALUE = 0;
		m_curve[0].AxisY_MINVALUE = 0;
		m_curve[0].AxisX_MAXVALUE = tmMAX_MONITOR;
		m_curve[0].AxisX_MINVALUE = 0;
	}
}

WORD        CtmQualityCurve::OnChange(WORD wIDControl)
{	
	if(wIDControl == IDE_SELECTBOX9)
	{
		m_curve[0].YAxis_UNIT = g_pStr[VM_QUALITY_BAR];
		m_curve[0].Caption_YAxis = g_pStr[VM_QUALITY_PRES];
	}
	else if ((wIDControl == IDE_SELECTBOX8)||(wIDControl == IDE_SELECTBOX11)|| \
		(wIDControl == IDE_SELECTBOX12)||(wIDControl == IDE_SELECTBOX13))
	{
		m_curve[0].YAxis_UNIT = g_pStr[VM_QUALITY_MM];
		m_curve[0].Caption_YAxis = g_pStr[VM_QUALITY_POSI];
	}
	else
	{
		m_curve[0].YAxis_UNIT = g_pStr[VW_TEMP_SECUNIT];
		m_curve[0].Caption_YAxis = g_pStr[VW_TEMP_TIMEUNIT];		
	}
	
	if((m_selectbox[wIDControl - IDE_BEGIN].bSelect != FALSE) && (m_KeyFlag != wIDControl))
	{
		m_selectbox[m_KeyFlag - IDE_BEGIN].bSelect = FALSE;
		GetControl(m_KeyFlag)->Update();

		if (wIDControl == IDE_BEGIN)
		{
			m_selectbox[IDE_END-IDE_BEGIN].bSelect = FALSE;
			GetControl(IDE_END-IDE_BEGIN)->Update();
		}
		m_KeyFlag = wIDControl;
		ReadData(wIDControl);
		GetControl(IDC_QUALITYCURVE)->Update();
//		//printf("tmDB_MONI2 = %d\n", sizeof(tmDB_MONI2));
	}
	CaptionShow();
	return TRUE;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
