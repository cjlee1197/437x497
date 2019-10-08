/*===========================================================================+
|  Class    : CtmControl_Monitor	                                         |
|  Task     : Moni Service Function	    	                     			 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008  				                                     |
+===========================================================================*/
#include	"tmControl_Monitor.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"
#include	"../../tmconfig.h"
#include	"../libiNetComm.h"
#include	"../../tmshmsg.h"
#include	"../../iotable.h"
#ifdef	D_3354
#include	"../../filerecord.h"
#endif
IMPLEMENT_DYNCREATE(CtmControl_Monitor, CtmControl)

#define   UNWORD(hw,lw)       (((DWORD)(hw) << 16) | lw)
/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
int		CtmControl_Monitor::m_nShotCount = 0;
tmDB_MONI2	CtmControl_Monitor::m_MoniCurrent;
tmDB_MONI2	CtmControl_Monitor::m_MoniIndex;
	
DWORD	  dwOldPower  = 0;	
BOOL	  m_bAmmeter = FALSE;
/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL		m_b7AK		= FALSE;
BOOL		m_b7TR   	= FALSE;
BOOL		m_b7TRDC   	= FALSE;
BOOL		m_bBLOW  	= FALSE;
BOOL		m_b0HT3C 	= FALSE;
BOOL		m_bSPCLR 	= FALSE;
BOOL		m_b7ASINJ	= FALSE;
BOOL		m_b0HTMLD	= FALSE;
BOOL		m_bROTETIM	= FALSE;
BOOL		m_bOPNTIM	= FALSE;
BOOL		m_b7LJINJEND= FALSE;
BOOL		m_bBOASTQ8	= FALSE;
BOOL		m_bFuYao	= FALSE;
BOOL		m_bMANPWTIM	= FALSE;
BOOL		m_bFAST  	= FALSE;
BOOL		m_bOILPOS	= FALSE;
BOOL		b_7AKFY		= FALSE;
BOOL		m_bSERVROTE		= FALSE;
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/	
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::CtmControl_Monitor()				             |
|  Task     :  CtmControl_Monitorr構造函數                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Monitor::CtmControl_Monitor(): CtmControl()
{
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::~CtmControl_Monitor()				             |
|  Task     :  CtmControl_Monitor析構函數                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Monitor::~CtmControl_Monitor()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Monitor::CreateSelf()
{
	//InitControl();
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::FreeSelf()				             	 |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Monitor::FreeSelf()
{
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::AddMoni2()				             	 |
|  Task     :  Add Moni2 			                                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Monitor::AddMoni2()
{
	m_b7AK    = g_pExTable->GetTableValue(0,"D_7AK");//SBJ 20150702 修正測二邏輯︰儲料終止與冷卻時間
	m_b7TR    = g_pExTable->GetTableValue(0,"D_7TR");
	m_b7TRDC  = g_pExTable->GetTableValue(0,"D_7TRDC");
	m_bBLOW   = g_pExTable->GetTableValue(0,"D_BLOW");
	m_b0HT3C  = g_pExTable->GetTableValue(0,"D_0HT3C");
	m_bSPCLR  = g_pExTable->GetTableValue(0,"D_SPCLR");
	m_b7ASINJ = g_pExTable->GetTableValue(0,"D_7ASINJETIME");
	m_b0HTMLD = g_pExTable->GetTableValue(0,"D_MLDING");
	m_bBOASTQ8= g_pExTable->GetTableValue(0,"D_BOASTQ8");
	m_bROTETIM= g_pExTable->GetTableValue(0,"D_ROTETIME");
	m_bOPNTIM = g_pExTable->GetTableValue(0,"D_OPNTIME");
	m_b7LJINJEND = g_pExTable->GetTableValue(0,"D_7LJINJEND"); /* zyh  2014-12-4 */
	m_bFuYao = g_pExTable->GetTableValue(0,"D_FYSLIDE");
	m_bMANPWTIM= g_pExTable->GetTableValue(0,"D_MPMONI");
	m_bFAST= g_pExTable->GetTableValue(0,"D_FAST");
	m_bOILPOS= g_pExTable->GetTableValue(0,"D_7LUNEW");
	b_7AKFY= g_pExTable->GetTableValue(0,"D_7AKFY");
	m_bSERVROTE= g_pExTable->GetTableValue(0,"D_SERVROTE");
	m_bAmmeter = CtmConfig::GetInstance()->GetAmmeterFlag();

	if(1 == CtmConfig::GetInstance()->GetCom2Type())//JOYCE2011-8-17	1HMI2DSP
		AddMoni2_1HMI2DSP();
	else		
		AddMoni2_COMMON();
	
	return TRUE;
}

BOOL	CtmControl_Monitor::AddMoni2_COMMON()
{
	static	int	nShotCount = 0;
	tmDB_MONI2		DBMoni2;
	memset(&DBMoni2, 0, sizeof(DBMoni2)); //20160329, 初始化
	DWORD   tmTemp1 = 0, tmTemp2 = 0, tmTemp3 = 0, tmTemp4 = 0;
	DWORD	dwCurrentPower = 0;
	long	lOffset = 0, lAmmeterPara = 0;
		
	pthread_mutex_lock(&m_MutexMoni);
	int		nIndex = 0;		//2009-5-21

	if(GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue == 1)/* zyh 2016-3-18 解決開模數一模時對開模數進行歸零，然後接下來一模監測數據不會記錄的問題 */
		nShotCount = 0;
	
	DBMoni2.OilMoni2.dwShotCount			= GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	if (DBMoni2.OilMoni2.dwShotCount - nShotCount >= GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue)
	{
		nShotCount = DBMoni2.OilMoni2.dwShotCount;
		DBMoni2.OilMoni2.tmlCycle  				= GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue;
		DBMoni2.OilMoni2.tmlInject 				= GetDBValue("MONI_SCRW_INJ_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.tmlTurnToHold			= GetDBValue("MONI_SCRW_CUSHION_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.tmlCharge 				= GetDBValue("MONI_SCRW_RCV_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.daPres_TurnToHold		= GetDBValue("MONI_SCRW_CUSHION_NULL_LASTPRESSURE").lValue;
		DBMoni2.OilMoni2.adPosi_InjectEnd		= GetDBValue("MONI_SCRW_INJ_END_LAST2POSITION").lValue;
		DBMoni2.OilMoni2.cnChargeRPM			= GetDBValue("MONI_SCRW_RCV_NULL_LASTRPM").lValue;
		DBMoni2.OilMoni2.tmSuckBack				= GetDBValue("MONI_SCRW_DCMP_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.tmEject				= GetDBValue("MONI_EJT_EJT_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.tmEjectAdvance			= GetDBValue("MONI_EJT_FWD_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.tmEjectReturn			= GetDBValue("MONI_EJT_BWD_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.daPres_InjectPeak		= GetDBValue("MONI_SCRW_INJ_NULL_LASTHIGHPRESSURE").lValue;
		DBMoni2.OilMoni2.daPres_ChargePeak		= GetDBValue("MONI_SCRW_RCV_NULL_LASTHIGHPRESSURE").lValue;
		DBMoni2.OilMoni2.adPosi_InjectStart		= GetDBValue("MONI_SCRW_INJ_START_LASTPOSITION").lValue;
		DBMoni2.OilMoni2.adPosi_TurnToHold		= GetDBValue("MONI_SCRW_CUSHION_NULL_LASTPOSITION").lValue;
		DBMoni2.OilMoni2.adPosi_InjectCushion	= GetDBValue("MONI_SCRW_INJ_END_LASTPOSITION").lValue;
		DBMoni2.OilMoni2.adPosi_InjectStartB	= GetDBValue("MONIB_SCRW2_INJ_NULL_LASTPOSITION").lValue;
		DBMoni2.OilMoni2.adPosi_TurnToHoldB		= GetDBValue("MONIB_SCRW2_CUSHION_NULL_LASTPOSITION").lValue;
		DBMoni2.OilMoni2.adPosi_InjectCushionB	= GetDBValue("MONIB_SCRW2_INJ_END_LASTPOSITION").lValue;
		DBMoni2.OilMoni2.tmlInjectB 			= GetDBValue("MONIB_SCRW2_INJ_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.tmlChargeB 			= GetDBValue("MONIB_SCRW2_RCV_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.adPosi_OpenMoldEnd		= GetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION").lValue;
		//	DBMoni2.OilMoni2.daPres_InjecterB		= GetDBValue("daPres_InjecterB").lValue;
		//	DBMoni2.OilMoni2.daPres_ChargeBackB		= GetDBValue("daPres_ChargeBackB").lValue;
		
		if(g_pExTable->GetTableValue(0,"D_MONICHGT"))/*zhongjw 儲料時間作假 從進入自動開始10以內顯示實際值10模以後顯示前10模的平均值*/
		{
			DWORD DWChrgT = 0;
			DWORD wCnt = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED28").lValue;
		 	wCnt += 1;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED28",wCnt,FALSE);
			if(wCnt <= 10)
			{
				DWChrgT = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;
				DWChrgT += DBMoni2.OilMoni2.tmlCharge;
				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27",DWChrgT,FALSE);
			}
			else
			{
				DBMoni2.OilMoni2.tmlCharge =(GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue+5)/10;
			}
		}
		
		DBMoni2.OilMoni2.daPres_InjecterB		= 0;
		DBMoni2.OilMoni2.daPres_ChargeBackB		= 0;
		
		DBMoni2.OilMoni2.daPres_TurnToHoldB			= GetDBValue("MONIB_SCRW2_CUSHION_NULL_LASTPRESSURE").lValue;     //26
		DBMoni2.OilMoni2.tmlTurnToHoldB					= GetDBValue("MONIB_SCRW2_CUSHION_NULL_LASTTIME").lValue;         //27
		DBMoni2.OilMoni2.adPosi_InjectHoldEndB	= GetDBValue("MONIB_SCRW2_INJ_END_LAST2POSITION").lValue;         //28
		DBMoni2.OilMoni2.daPres_InjectMiddleB		= GetDBValue("MONIB_SCRW2_INJ_NULL_LASTHIGHPRESSURE").lValue;     //29
		DBMoni2.OilMoni2.daPres_ChargeMiddleB		= GetDBValue("MONIB_SCRW2_RCV_NULL_LASTHIGHPRESSURE").lValue;     //30
		DBMoni2.OilMoni2.InjectMaxSpeed					= GetDBValue("MONI_OTHR_OTHR_NULL_W0A58").lValue;           			//31
		DBMoni2.OilMoni2.InjectMaxSpeedB				= GetDBValue("MONIB_SCRW2_OTHR_NULL_W142A").lValue;           		//32
		
		DBMoni2.OilMoni2.wReservedMG[7]				= GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue; //20150824, TM
		
		if(g_pExTable->GetTableValue(0,"D_SPCINJ"))
		{
			int tempv = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED71").lValue;
			DBMoni2.OilMoni2.adPosi_InjectEnd		= (GetDBValue("MONI_SCRW_INJ_END_LAST2POSITION").lValue*tempv)/10;
			DBMoni2.OilMoni2.adPosi_InjectStart		= (GetDBValue("MONI_SCRW_INJ_START_LASTPOSITION").lValue*tempv)/10;
			DBMoni2.OilMoni2.adPosi_TurnToHold		= (GetDBValue("MONI_SCRW_CUSHION_NULL_LASTPOSITION").lValue*tempv)/10;
			DBMoni2.OilMoni2.adPosi_InjectCushion	= (GetDBValue("MONI_SCRW_INJ_END_LASTPOSITION").lValue*tempv)/10;
			DBMoni2.OilMoni2.adPosi_InjectCushion	= (GetDBValue("MONI_SCRW_INJ_END_LASTPOSITION").lValue*tempv)/10;
		}
		
		//<<<<<<<< 取件計時  //JOYCE2011-3-7 
		WORD	wDSPLIBDaysCode = GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
		WORD	wDSPLIBYearCode = GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;		
		
		tmTemp1 = GetDBValue("MONI_SCRW_RCV_NULL_LASTTIME").lValue + GetDBValue("MONI_SCRW_DCMP_NULL_LASTTIME").lValue; //儲料計時+射退計時
		tmTemp2 = GetDBValue("MSET_MLD_COOLING_NULL_TIME").lValue; //冷卻計時
		if(tmTemp1 > tmTemp2)
			tmTemp3 = tmTemp1;
		else
			tmTemp3 = tmTemp2;
		
		tmTemp4 = GetDBValue("MONI_MLD_CLOSE_NULL_LASTTIME").lValue +GetDBValue("MONI_SCRW_INJ_NULL_LASTTIME").lValue 
		           +GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue + tmTemp3;

		if((wDSPLIBDaysCode >= 0x7417) && (wDSPLIBYearCode == 0x2007) || (wDSPLIBYearCode >= 0x2008))
		{	 
			if(m_b7AK)
				DBMoni2.OilMoni2.tmTake	= GetDBValue("MONI_CORF_IN_NULL_REALTIME").lValue+(GetDBValue("MONI_CORF_OUT_NULL_REALTIME").lValue<<16);	//24
			else
				DBMoni2.OilMoni2.tmTake	= GetDBValue("MONI_OTHR_OTHR_NULL_W0A59").lValue+(GetDBValue("MONI_OTHR_OTHR_NULL_W0A5A").lValue<<16);	//24
		}
		else  //取件計時=循環計時-關模計時-射出計時-(冷卻計時與(儲料計時+射退計時)二者的較大值)-開模計時
		{
			if(GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue > tmTemp4)		
			{
				DBMoni2.OilMoni2.tmTake = GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue - tmTemp4;
			}
			else
			{
				DBMoni2.OilMoni2.tmTake = 0;
			}
			SetDBValue("MONI_OTHR_OTHR_NULL_W0A59",tmLOWORD(DBMoni2.OilMoni2.tmTake));//為了傳遞給inet的值,借用data
			SetDBValue("MONI_OTHR_OTHR_NULL_W0A5A",tmHIWORD(DBMoni2.OilMoni2.tmTake));//為了傳遞給inet的值,借用data
		}
		
		
		if((!m_bFuYao)&&(!b_7AKFY))
		{
			//冷卻計時
			SetDBValue("MONI_OTHR_OTHR_NULL_W0A5B",tmLOWORD(GetDBValue("MSET_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			SetDBValue("MONI_OTHR_OTHR_NULL_W0A5C",tmHIWORD(GetDBValue("MSET_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
		}

		//冷卻計時B 
		SetDBValue("MONIB_SCRW2_OTHR_NULL_W142B",tmLOWORD(GetDBValue("MSETB_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
		SetDBValue("MONIB_SCRW2_OTHR_NULL_W142C",tmHIWORD(GetDBValue("MSETB_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
		//>>>>>>>
		
		DBMoni2.OilMoni2.wReservedMG[0]			= GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue;
		DBMoni2.OilMoni2.wReservedMG[1]			= GetDBValue("MONI_MLD_CLOSE_NULL_LASTTIME").lValue;
		if(m_b7TR||g_pExTable->GetTableValue(0,"D_7DM"))
    	{
     		DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.wReservedMG[1]		= GetDBValue("MONI_MLD_CLOSE_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.wReservedMG[2]		= GetDBValue("MONI_MLD_CLOSE_PROTECT_LASTTIME").lValue;
			DBMoni2.OilMoni2.wReservedMG[3]		= GetDBValue("MONI_MLD_CLOSE_CLAMPUP_LASTTIME").lValue;
			if(m_b7TRDC)
	    	{
	    		DBMoni2.OilMoni2.wReservedMG[4] =GetDBValue("MONI_CORF_IN_NULL_LASTTIME").lValue+(GetDBValue("MONI_CORF_OUT_NULL_LASTTIME").lValue<<16);
	    	}
    	}
		else if(m_bBLOW) 
    	{
    		DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_CORA_IN_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.wReservedMG[1]		= GetDBValue("MONI_CORA_OUT_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.wReservedMG[2]		= GetDBValue("MONI_CORB_IN_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.wReservedMG[3]		= GetDBValue("MONI_CORB_OUT_NULL_LASTTIME").lValue;
    	}
		else  if(g_pExTable->GetTableValue(0,"D_MONIINJE1"))    
	    {
	    	DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MSET_SCRW_INJ_STEP1_PRESSURE").lValue;
	    	DBMoni2.OilMoni2.wReservedMG[1]		= GetDBValue("MSET_SCRW_HLDP_STEP1_PRESSURE").lValue;
	    }		
		
		if(g_pExTable->GetTableValue(0,"D_EGFST")||g_pExTable->GetTableValue(0,"D_7DY"))/*When cyclic done, get in to do it.*/
		{
			DBMoni2.OilMoni2.wReservedMG[0]				= GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue;//開模計時 //33
			DBMoni2.OilMoni2.wReservedMG[1]				= GetDBValue("MONI_MLD_CLOSE_NULL_LASTTIME").lValue;//關模計時 //34
			DBMoni2.OilMoni2.wReservedMG[2]				= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5E").lValue;//射出速度 //35  Last 2D
			DBMoni2.OilMoni2.wReservedMG[3]				= GetDBValue("MONI_OTHR_OTHR_NULL_W0A60").lValue;//儲料終點//36 Last 2F
			DBMoni2.OilMoni2.wReservedMG[4]				= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5D").lValue;//儲料終點-射出終點//37 Last 2C
		}
		if(m_b0HT3C)
    	{
     		DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5B").lValue;//C組射出計時值
			DBMoni2.OilMoni2.wReservedMG[1]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5C").lValue;//C組儲料計時值
			DBMoni2.OilMoni2.wReservedMG[2]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5D").lValue;//C組射出起點
			DBMoni2.OilMoni2.wReservedMG[3]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5E").lValue;//C組保壓起點
			DBMoni2.OilMoni2.tmEjectAdvance		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5F").lValue;//C組射出終點
			DBMoni2.OilMoni2.tmEjectReturn		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A60").lValue;//C組保壓轉換時間值
    	}
    	if(m_bSERVROTE)
    	{
    		DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5F").lValue;//
				DBMoni2.OilMoni2.wReservedMG[1]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A60").lValue;//
    	}
    	
	    if(m_bMANPWTIM)
	    {
	    	DBMoni2.OilMoni2.wReservedMG[5]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A60").lValue;
	    	DBMoni2.OilMoni2.wReservedMG[6]		= (GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue)-(GetDBValue("MONI_OTHR_OTHR_NULL_W0A60").lValue);
	    }
	    if(m_bROTETIM)
	    {
	    	DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_CORF_OUT_NULL_REALTIME").lValue;//CORE F BWD TIME
	    }	    
	    if(m_bOPNTIM)
	    	DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_MLD_OPEN_NULL_REALTIME").lValue;
	    if(m_bOILPOS)
	    {
	    	//DBMoni2.OilMoni2.wReservedMG[5]				= GetOilVat(GetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION").lValue);
	    	//DBMoni2.OilMoni2.wReservedMG[5]				= TMSELVATFUNC::GetOilVat((GetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION").lValue)*0.1)*10;
	    	//DBMoni2.OilMoni2.wReservedMG[5]				= TMSELVATFUNC::OilToPosi((GetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION").lValue)*0.1)*10;
	    	DBMoni2.OilMoni2.tmEject				= GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52").lValue;//推力座位置
	    	DBMoni2.OilMoni2.wReservedMG[6]			= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5F").lValue;
	    }
	    if(m_bFAST)
    		DBMoni2.OilMoni2.wReservedMG[6]				= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5F").lValue;	
	   	if(m_b7LJINJEND) /* zyh  2014-12-4 add for 7LJ  Posi_InjectEnd  */
	  	{
	  		if(GetDBValue("MSET_SCRW_CUSHION_NULL_OPTION").lValue == 0)  /* inject  to  hold  mode */
	  			DBMoni2.OilMoni2.adPosi_InjectEnd	= GetDBValue("MSET_SCRW_CUSHION_NULL_POSITION").lValue;
	  		else
		      DBMoni2.OilMoni2.adPosi_InjectEnd		= GetDBValue("MONI_SCRW_INJ_END_LAST2POSITION").lValue;
	    }
   		if(m_bFuYao||b_7AKFY)
	    {
	    	DBMoni2.OilMoni2.wReservedMG[0]		= GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue;//開模計時
			DBMoni2.OilMoni2.wReservedMG[1]		= GetDBValue("MONI_MLD_CLOSE_NULL_LASTTIME").lValue;//關模計時
			DBMoni2.OilMoni2.wReservedMG[4]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5B").lValue;//左移模時間
			DBMoni2.OilMoni2.wReservedMG[5]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5C").lValue;//右移模時間
	    }

  		if(m_bSPCLR)	//fuxy, 2012-7-5 for 7AK
	    {
	    	DBMoni2.OilMoni2.adPosi_InjectStartB= GetDBValue("MONI_CORD_IN_NULL_LASTTIME").lValue;	//左儲料終點
			DBMoni2.OilMoni2.adPosi_TurnToHoldB	= GetDBValue("MONI_CORD_OUT_NULL_LASTTIME").lValue;	//右儲料終點
	    	DBMoni2.OilMoni2.tmlInjectB			= UNWORD(GetDBValue("MONI_CORE_OUT_NULL_LASTTIME").lValue, GetDBValue("MONI_CORE_IN_NULL_LASTTIME").lValue); 	//左固化時間 
			DBMoni2.OilMoni2.tmlChargeB			= UNWORD(GetDBValue("MONI_CORF_OUT_NULL_LASTTIME").lValue, GetDBValue("MONI_CORF_IN_NULL_LASTTIME").lValue); 	//右固化時間
	    }

		if(m_b0HTMLD) /*ZHBA 2012-6-29 */
			DBMoni2.OilMoni2.wReservedMG[2]		= GetDBValue("MONI_OTHR_OTHR_NULL_W0A5D").lValue;//中鴻泰成品厚度
    		
	    if(m_b7ASINJ)//2010-10-18
	    {
	    	DWORD injtime[6];
		    if(g_ptaskdsp!=NULL)
				g_ptaskdsp->ReqValues(0x6503, (void*)&injtime[0], sizeof(injtime[0])*6); 
			
			for(int i=0;i<6;i++)
			{
				if(i < 4)
			 		DBMoni2.OilMoni2.wReservedMG[i]=injtime[i];	
			 	else if(i ==4)
			 		DBMoni2.OilMoni2.daPres_InjectMiddleB=injtime[i];//	B組射出尖峰壓力 (OIL)  29
			 	else if(i == 5)
			 		DBMoni2.OilMoni2.daPres_ChargeMiddleB=injtime[i];//	B組儲料尖峰壓力 (OIL) 30
			}
	    }
	    
	    if(m_b7AK)
	    {
	    	DBMoni2.OilMoni2.wReservedMG[0]				= GetDBValue("MONI_CORE_OUT_NULL_REALTIME").lValue;
	    	DBMoni2.OilMoni2.wReservedMG[1]				= GetDBValue("MONI_OTHR_OTHR_NULL_W0A60").lValue;//愛科儲料終點34//LAST 2F
	    }
	    
	    if(m_bBOASTQ8)/*Leedan2012-11-15 注吹機*/
	    {
	        DBMoni2.OilMoni2.wReservedMG[0]				= GetDBValue("MONI_CORC_IN_NULL_LASTTIME").lValue;//關模//33
			DBMoni2.OilMoni2.wReservedMG[1]				= GetDBValue("MONI_CORA_OUT_NULL_LASTTIME").lValue;//開模//34
			DBMoni2.OilMoni2.wReservedMG[2]				= GetDBValue("MONI_CORA_IN_NULL_LASTTIME").lValue;//轉位進//35
			DBMoni2.OilMoni2.wReservedMG[3]				= GetDBValue("MONI_CORB_IN_NULL_LASTTIME").lValue;//轉塔上升//36				
			DBMoni2.OilMoni2.adPosi_OpenMoldEnd	        = GetDBValue("MONI_CORB_OUT_NULL_LASTTIME").lValue;//轉塔下降//25
	    }
	    
	    if(m_bAmmeter)	//fuxy, 2012-5-17, 電表能耗, 計算這一模的能耗   
    	{
			dwOldPower 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED17").lValue;
			dwCurrentPower 	= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;  
			
			if(dwCurrentPower >= dwOldPower)
			{
				if(dwOldPower != 0)
				{
					lOffset = dwCurrentPower - dwOldPower;
					if(lOffset >= 0)			//if current power >= old power the add the offset
					{
						lAmmeterPara = lOffset;
					}
					else						//else offset = 0, not need add
					{
						lAmmeterPara = 0;
					}
					dwOldPower = dwCurrentPower;	
				}
				else
				{
					lAmmeterPara = dwCurrentPower;
					dwOldPower = dwCurrentPower;
				}
			}
			
		  	//if(CtmConfig::GetInstance()->GetAmmeterFlag() == 2)
    		if(m_bBOASTQ8)
   				DBMoni2.OilMoni2.daPres_InjecterB		  = lAmmeterPara*100;
   			else
   				DBMoni2.OilMoni2.dwReserved[0]				= lAmmeterPara*100;
        	//else
    		//	DBMoni2.OilMoni2.wReservedMG[0]				= lAmmeterPara;
    		
    		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED16", lAmmeterPara); 
    		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED17", dwOldPower);		//Old 
    	}
		DBMoni2.wSource = tmOIL_MONI2FLAG;
		if (m_MoniIndex.nCount < CtmConfig::GetInstance()->GetMoniMaxNum()) 
		{
			m_MoniIndex.nCount++;
		}
		nIndex	= m_MoniIndex.nIndex + 1;	//2009-5-21
		#ifdef	D_3354
		if(CtmConfig::GetInstance()->GetRecordType() == 1)		//james add new record moni
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, nIndex, &DBMoni2, sizeof(DBMoni2));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, nIndex, &DBMoni2);
		#else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, nIndex, &DBMoni2);
		#endif
		if ((++m_MoniIndex.nIndex) >= CtmConfig::GetInstance()->GetMoniMaxNum()) 
		{
			m_MoniIndex.nIndex = 0;    //St0219/02 change to here for monitor lose problem
		}
		
		#ifdef	D_3354
		if(CtmConfig::GetInstance()->GetRecordType() == 1)		//james add new record moni
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
		#else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
		#endif
	}
		
	SendMsg(MSG_USER_SH_MONI_ADD, nIndex, m_MoniIndex.nCount, NULL);	//2009-5-21
	pthread_mutex_unlock(&m_MutexMoni);
	return	TRUE;
}

BOOL	CtmControl_Monitor::AddMoni2_1HMI2DSP()
{
	static	int	nShotCount = 0;
	tmDB_MONI2		DBMoni2;
	memset(&DBMoni2, 0, sizeof(DBMoni2)); //20160329, 初始化
	DWORD   tmTemp1 = 0, tmTemp2 = 0, tmTemp3 = 0, tmTemp4 = 0;
	WORD		wSampleInterval = 0;
	
	pthread_mutex_lock(&m_MutexMoni);
	int			nIndex = 0;		//2009-5-21
	
	DBMoni2.OilMoni2.wReservedMG[3] = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED10").lValue;

	if(DBMoni2.OilMoni2.wReservedMG[3] == 1)     //COM1
	{
		DBMoni2.OilMoni2.dwShotCount			= GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
		wSampleInterval = GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue;
	}
	else if(DBMoni2.OilMoni2.wReservedMG[3] == 2)    //COM2
	{
		DBMoni2.OilMoni2.dwShotCount			= GetDBValue("PROD_2_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
		wSampleInterval = GetDBValue("SAMPLING_2_HMI_NULL_NULL_SAMPLEINTERVAL").lValue;
	}
			
	if (DBMoni2.OilMoni2.dwShotCount - nShotCount >= wSampleInterval)
	{
		nShotCount = DBMoni2.OilMoni2.dwShotCount;
		if(DBMoni2.OilMoni2.wReservedMG[3] == 1)     //COM1
		{
			DBMoni2.OilMoni2.tmlCycle  				= GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue;
			DBMoni2.OilMoni2.tmlInject 				= GetDBValue("MONI_SCRW_INJ_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmlTurnToHold			= GetDBValue("MONI_SCRW_CUSHION_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmlCharge 				= GetDBValue("MONI_SCRW_RCV_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.daPres_TurnToHold		= GetDBValue("MONI_SCRW_CUSHION_NULL_LASTPRESSURE").lValue;
			DBMoni2.OilMoni2.adPosi_InjectEnd		= GetDBValue("MONI_SCRW_INJ_END_LAST2POSITION").lValue;
			DBMoni2.OilMoni2.cnChargeRPM			= GetDBValue("MONI_SCRW_RCV_NULL_LASTRPM").lValue;
			DBMoni2.OilMoni2.tmSuckBack				= GetDBValue("MONI_SCRW_DCMP_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmEject				= GetDBValue("MONI_EJT_EJT_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmEjectAdvance			= GetDBValue("MONI_EJT_FWD_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmEjectReturn			= GetDBValue("MONI_EJT_BWD_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.daPres_InjectPeak		= GetDBValue("MONI_SCRW_INJ_NULL_LASTHIGHPRESSURE").lValue;
			DBMoni2.OilMoni2.daPres_ChargePeak		= GetDBValue("MONI_SCRW_RCV_NULL_LASTHIGHPRESSURE").lValue;
			DBMoni2.OilMoni2.adPosi_InjectStart		= GetDBValue("MONI_SCRW_INJ_START_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_TurnToHold		= GetDBValue("MONI_SCRW_CUSHION_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_InjectCushion	= GetDBValue("MONI_SCRW_INJ_END_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_InjectStartB	= GetDBValue("MONIB_SCRW2_INJ_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_TurnToHoldB		= GetDBValue("MONIB_SCRW2_CUSHION_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_InjectCushionB	= GetDBValue("MONIB_SCRW2_INJ_END_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.tmlInjectB 			= GetDBValue("MONIB_SCRW2_INJ_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmlChargeB 			= GetDBValue("MONIB_SCRW2_RCV_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.adPosi_OpenMoldEnd		= GetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.daPres_InjecterB		= 0;
			DBMoni2.OilMoni2.daPres_ChargeBackB		= 0;
			
			DBMoni2.OilMoni2.daPres_TurnToHoldB			= GetDBValue("MONIB_SCRW2_CUSHION_NULL_LASTPRESSURE").lValue;     //26
			DBMoni2.OilMoni2.tmlTurnToHoldB					= GetDBValue("MONIB_SCRW2_CUSHION_NULL_LASTTIME").lValue;         //27
			DBMoni2.OilMoni2.adPosi_InjectHoldEndB	= GetDBValue("MONIB_SCRW2_INJ_END_LAST2POSITION").lValue;         //28
			DBMoni2.OilMoni2.daPres_InjectMiddleB		= GetDBValue("MONIB_SCRW2_INJ_NULL_LASTHIGHPRESSURE").lValue;     //29
			DBMoni2.OilMoni2.daPres_ChargeMiddleB		= GetDBValue("MONIB_SCRW2_RCV_NULL_LASTHIGHPRESSURE").lValue;     //30
			DBMoni2.OilMoni2.InjectMaxSpeed					= GetDBValue("MONI_OTHR_OTHR_NULL_W0A58").lValue;           			//31
			DBMoni2.OilMoni2.InjectMaxSpeedB				= GetDBValue("MONIB_SCRW2_OTHR_NULL_W142A").lValue;           		//32
			
			//<<<<<<<< 取件計時  //JOYCE2011-3-7 
			WORD	wDSPLIBDaysCode = GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
			WORD	wDSPLIBYearCode = GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;		
			
			tmTemp1 = GetDBValue("MONI_SCRW_RCV_NULL_LASTTIME").lValue + GetDBValue("MONI_SCRW_DCMP_NULL_LASTTIME").lValue; //儲料計時+射退計時
			tmTemp2 = GetDBValue("MSET_MLD_COOLING_NULL_TIME").lValue; //冷卻計時
			if(tmTemp1 > tmTemp2)
				tmTemp3 = tmTemp1;
			else
				tmTemp3 = tmTemp2;
			
			tmTemp4 = GetDBValue("MONI_MLD_CLOSE_NULL_LASTTIME").lValue +GetDBValue("MONI_SCRW_INJ_NULL_LASTTIME").lValue 
			           +GetDBValue("MONI_MLD_OPEN_NULL_LASTTIME").lValue + tmTemp3;
			
			if((wDSPLIBDaysCode >= 0x7417) && (wDSPLIBYearCode == 0x2007) || (wDSPLIBYearCode >= 0x2008))
			{	 
				DBMoni2.OilMoni2.tmTake = GetDBValue("MONI_OTHR_OTHR_NULL_W0A59").lValue+(GetDBValue("MONI_OTHR_OTHR_NULL_W0A5A").lValue<<16);	//24
			}
			else  //取件計時=循環計時-關模計時-射出計時-(冷卻計時與(儲料計時+射退計時)二者的較大值)-開模計時
			{
				if(GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue > tmTemp4)		
				{
					DBMoni2.OilMoni2.tmTake = GetDBValue("MONI_OTHR_OTHR_NULL_CYCLELASTTIME").lValue - tmTemp4;
				}
				else
				{
					DBMoni2.OilMoni2.tmTake = 0;
				}
				SetDBValue("MONI_OTHR_OTHR_NULL_W0A59",tmLOWORD(DBMoni2.OilMoni2.tmTake));//為了傳遞給inet的值,借用data
				SetDBValue("MONI_OTHR_OTHR_NULL_W0A5A",tmHIWORD(DBMoni2.OilMoni2.tmTake));//為了傳遞給inet的值,借用data
			}
			
			//冷卻計時
			SetDBValue("MONI_OTHR_OTHR_NULL_W0A5B",tmLOWORD(GetDBValue("MSET_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			SetDBValue("MONI_OTHR_OTHR_NULL_W0A5C",tmHIWORD(GetDBValue("MSET_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
    	
			//冷卻計時B 
			SetDBValue("MONIB_SCRW2_OTHR_NULL_W142B",tmLOWORD(GetDBValue("MSETB_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			SetDBValue("MONIB_SCRW2_OTHR_NULL_W142C",tmHIWORD(GetDBValue("MSETB_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			//>>>>>>>
		}
		else if(DBMoni2.OilMoni2.wReservedMG[3] == 2)  //COM2
		{
			DBMoni2.OilMoni2.tmlCycle  				= GetDBValue("MONI_2_OTHR_OTHR_NULL_CYCLELASTTIME").lValue;
			DBMoni2.OilMoni2.tmlInject 				= GetDBValue("MONI_2_SCRW_INJ_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmlTurnToHold			= GetDBValue("MONI_2_SCRW_CUSHION_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmlCharge 				= GetDBValue("MONI_2_SCRW_RCV_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.daPres_TurnToHold		= GetDBValue("MONI_2_SCRW_CUSHION_NULL_LASTPRESSURE").lValue;
			DBMoni2.OilMoni2.adPosi_InjectEnd		= GetDBValue("MONI_2_SCRW_INJ_END_LAST2POSITION").lValue;
			DBMoni2.OilMoni2.cnChargeRPM			= GetDBValue("MONI_2_SCRW_RCV_NULL_LASTRPM").lValue;
			DBMoni2.OilMoni2.tmSuckBack				= GetDBValue("MONI_2_SCRW_DCMP_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmEject				= GetDBValue("MONI_2_EJT_EJT_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmEjectAdvance			= GetDBValue("MONI_2_EJT_FWD_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmEjectReturn			= GetDBValue("MONI_2_EJT_BWD_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.daPres_InjectPeak		= GetDBValue("MONI_2_SCRW_INJ_NULL_LASTHIGHPRESSURE").lValue;
			DBMoni2.OilMoni2.daPres_ChargePeak		= GetDBValue("MONI_2_SCRW_RCV_NULL_LASTHIGHPRESSURE").lValue;
			DBMoni2.OilMoni2.adPosi_InjectStart		= GetDBValue("MONI_2_SCRW_INJ_START_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_TurnToHold		= GetDBValue("MONI_2_SCRW_CUSHION_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_InjectCushion	= GetDBValue("MONI_2_SCRW_INJ_END_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_InjectStartB	= GetDBValue("MONIB_2_SCRW2_INJ_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_TurnToHoldB		= GetDBValue("MONIB_2_SCRW2_CUSHION_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.adPosi_InjectCushionB	= GetDBValue("MONIB_2_SCRW2_INJ_END_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.tmlInjectB 			= GetDBValue("MONIB_2_SCRW2_INJ_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.tmlChargeB 			= GetDBValue("MONIB_2_SCRW2_RCV_NULL_LASTTIME").lValue;
			DBMoni2.OilMoni2.adPosi_OpenMoldEnd		= GetDBValue("MONI_2_MLD_OPEN_NULL_LASTPOSITION").lValue;
			DBMoni2.OilMoni2.daPres_InjecterB		= 0;
			DBMoni2.OilMoni2.daPres_ChargeBackB		= 0;
			
			DBMoni2.OilMoni2.daPres_TurnToHoldB		= GetDBValue("MONIB_2_SCRW2_CUSHION_NULL_LASTPRESSURE").lValue;     //26
			DBMoni2.OilMoni2.tmlTurnToHoldB			= GetDBValue("MONIB_2_SCRW2_CUSHION_NULL_LASTTIME").lValue;         //27
			DBMoni2.OilMoni2.adPosi_InjectHoldEndB	= GetDBValue("MONIB_2_SCRW2_INJ_END_LAST2POSITION").lValue;         //28
			DBMoni2.OilMoni2.daPres_InjectMiddleB	= GetDBValue("MONIB_2_SCRW2_INJ_NULL_LASTHIGHPRESSURE").lValue;     //29
			DBMoni2.OilMoni2.daPres_ChargeMiddleB	= GetDBValue("MONIB_2_SCRW2_RCV_NULL_LASTHIGHPRESSURE").lValue;     //30
			DBMoni2.OilMoni2.InjectMaxSpeed			= GetDBValue("MONI_2_OTHR_OTHR_NULL_W0A58").lValue;           			//31
			DBMoni2.OilMoni2.InjectMaxSpeedB		= GetDBValue("MONIB_2_SCRW2_OTHR_NULL_W142A").lValue;           		//32
			
			//<<<<<<<< 取件計時  //JOYCE2011-3-7 
			WORD	wDSPLIBDaysCode = GetDBValue("CONF2_CODE_WRITE_NULL_DATA16").lValue;
			WORD	wDSPLIBYearCode = GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue;		
			
			tmTemp1 = GetDBValue("MONI_2_SCRW_RCV_NULL_LASTTIME").lValue + GetDBValue("MONI_2_SCRW_DCMP_NULL_LASTTIME").lValue; //儲料計時+射退計時
			tmTemp2 = GetDBValue("MSET_2_MLD_COOLING_NULL_TIME").lValue; //冷卻計時
			if(tmTemp1 > tmTemp2)
				tmTemp3 = tmTemp1;
			else
				tmTemp3 = tmTemp2;
			
			tmTemp4 = GetDBValue("MONI_2_MLD_CLOSE_NULL_LASTTIME").lValue +GetDBValue("MONI_2_SCRW_INJ_NULL_LASTTIME").lValue 
			           +GetDBValue("MONI_2_MLD_OPEN_NULL_LASTTIME").lValue + tmTemp3;
			
			if((wDSPLIBDaysCode >= 0x7417) && (wDSPLIBYearCode == 0x2007) || (wDSPLIBYearCode >= 0x2008))
			{	 
				DBMoni2.OilMoni2.tmTake = GetDBValue("MONI_2_OTHR_OTHR_NULL_W0A59").lValue+(GetDBValue("MONI_2_OTHR_OTHR_NULL_W0A5A").lValue<<16);	//24
			}
			else  //取件計時=循環計時-關模計時-射出計時-(冷卻計時與(儲料計時+射退計時)二者的較大值)-開模計時
			{
				if(GetDBValue("MONI_2_OTHR_OTHR_NULL_CYCLELASTTIME").lValue > tmTemp4)		
				{
					DBMoni2.OilMoni2.tmTake = GetDBValue("MONI_2_OTHR_OTHR_NULL_CYCLELASTTIME").lValue - tmTemp4;
				}
				else
				{
					DBMoni2.OilMoni2.tmTake = 0;
				}
				SetDBValue("MONI_2_OTHR_OTHR_NULL_W0A59",tmLOWORD(DBMoni2.OilMoni2.tmTake));//為了傳遞給inet的值,借用data
				SetDBValue("MONI_2_OTHR_OTHR_NULL_W0A5A",tmHIWORD(DBMoni2.OilMoni2.tmTake));//為了傳遞給inet的值,借用data
			}
		
			//冷卻計時
			SetDBValue("MONI_2_OTHR_OTHR_NULL_W0A5B",tmLOWORD(GetDBValue("MSET_2_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			SetDBValue("MONI_2_OTHR_OTHR_NULL_W0A5C",tmHIWORD(GetDBValue("MSET_2_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
		
			//冷卻計時B 
			SetDBValue("MONIB_2_SCRW2_OTHR_NULL_W142B",tmLOWORD(GetDBValue("MSETB_2_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			SetDBValue("MONIB_2_SCRW2_OTHR_NULL_W142C",tmHIWORD(GetDBValue("MSETB_2_MLD_COOLING_NULL_TIME").lValue));//為了傳遞給inet的值,借用data
			//>>>>>>>
		}
		
		DBMoni2.wSource = tmOIL_MONI2FLAG;
		if (m_MoniIndex.nCount < CtmConfig::GetInstance()->GetMoniMaxNum()) 
		{
			m_MoniIndex.nCount++;
		}
		nIndex	= m_MoniIndex.nIndex + 1;	//2009-5-21
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, nIndex, &DBMoni2);
		if ((++m_MoniIndex.nIndex) >= CtmConfig::GetInstance()->GetMoniMaxNum()) 
		{
			m_MoniIndex.nIndex = 0;    //St0219/02 change to here for monitor lose problem
		}
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
	}
		
	SendMsg(MSG_USER_SH_MONI_ADD, nIndex, m_MoniIndex.nCount, NULL);	//2009-5-21
	pthread_mutex_unlock(&m_MutexMoni);
	return	TRUE;
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::ClearMoniLog()             	             |
|  Task     : ClearMoniLog     							                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_Monitor::ClearMoniLog()
{
	m_MoniIndex.nIndex =
	m_MoniIndex.nCount = 0;
	//g_MoniFile.WriteRecord(1, &m_MoniIndex);
	//	fans modify
	#ifdef	D_3354	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE,0, &m_MoniIndex);
	#else
	CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE,0, &m_MoniIndex);
	#endif
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::Destroy_MoniLog()             	         |
|  Task     : Destroy_MoniLog     							                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_Monitor::Destroy_MoniLog()
{
	pthread_mutex_destroy(&m_MutexMoni);	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::GetMoniIndex()                	         |
|  Task     : GetMoniIndex     							                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void*	CtmControl_Monitor::GetMoniIndex()
{
	return	(void*)&m_MoniIndex;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::GetMoniLog(int nIndex)                	 |
|  Task     : GetMoniLog     							                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void*	CtmControl_Monitor::GetMoniLog(int nIndex)
{
	memset(&m_MoniCurrent, 0, sizeof(m_MoniCurrent));
	if (nIndex > m_MoniIndex.nCount) return &m_MoniCurrent;
	nIndex = m_MoniIndex.nIndex - nIndex;        // m_MsgIndex.wIndex points to the record to be added
	if (nIndex < 0) nIndex += CtmConfig::GetInstance()->GetMoniMaxNum();
	#ifdef	D_3354
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_MONI, nIndex+1, &m_MoniCurrent, sizeof(m_MoniCurrent));
	else
		CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_MONI_SRAMFILE, nIndex+1, &m_MoniCurrent);
	#else
	CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_MONI_SRAMFILE, nIndex+1, &m_MoniCurrent);
	#endif

	return (void*)&m_MoniCurrent;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::GetMoniSize()                	 			 |
|  Task     : GetMoniSize     							                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Monitor::_GetMoniStructSize()
{
	return sizeof(tmDB_MONI2);	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::GetMoniLogCount()                  	     |
|  Task     : GetMoniLogCount							                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD        CtmControl_Monitor::GetMoniLogCount()
{
	return	m_MoniIndex.nCount;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::Init_MoniLog()                   	     	 |
|  Task     : Init MonitorLog							                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                               	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CtmControl_Monitor::Init_MoniLog()
{
	pthread_mutex_init(&m_MutexMoni, NULL);
	#ifdef	D_3354
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_MONI))
		{
			memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2));
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
		}
		else
		{
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
		}

		if(	m_MoniIndex.nCount > CtmConfig::GetInstance()->GetMoniMaxNum() || m_MoniIndex.nCount < 0 ||
			m_MoniIndex.nIndex > m_MoniIndex.nCount || m_MoniIndex.nIndex < 0)
		{
			memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2));
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
		}
		if(m_MoniIndex.nCount > CtmConfig::GetInstance()->GetMoniMaxNum() && m_MoniIndex.nIndex != 0)
		{
			memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2));
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
		}
	}
	else
	{
	    if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_MONI_SRAMFILE))
	    {
		    memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2)); 
		    CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
	    }
	    else
	    {
		    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
	    }
	}
	#else
	if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_MONI_SRAMFILE))
	{
	    memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2)); 
	    CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
	}
	else
	{
	    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);
	}
	#endif
	
	                                                                       
	if(	m_MoniIndex.nCount > CtmConfig::GetInstance()->GetMoniMaxNum() || m_MoniIndex.nCount < 0 ||    
		m_MoniIndex.nIndex > m_MoniIndex.nCount || m_MoniIndex.nIndex < 0) 
	{                                                                      
		memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2));                       
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex);            
	}                                                                      
	if(m_MoniIndex.nCount > CtmConfig::GetInstance()->GetMoniMaxNum() && m_MoniIndex.nIndex != 0)      
	{                                                                 
		memset(&m_MoniIndex, 0, sizeof(tmDB_MONI2));                       
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE, 0, &m_MoniIndex); 
	} 
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Monitor::SaveMoni2(I32_MONI2* pI32Moni2) 	     	 |
|  Task     :  Save Error Record to sram and show promp	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   I32_MONI2   pI32Moni2   - Monitor struct                   	 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL		CtmControl_Monitor::SaveMoni2(I32_MONI2* pI32Moni2)
{
	tmDB_MONI2		DBMoni2;
	memset(&DBMoni2, 0, sizeof(DBMoni2)); //20160329, 初始化
	pthread_mutex_lock(&m_MutexMoni);
	DBMoni2.I32Moni = *pI32Moni2;
	DBMoni2.wSource = tmI32_MONI2FLAG;
	#ifdef	D_3354	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI,m_MoniIndex.nIndex + 1, &DBMoni2, sizeof(DBMoni2));
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE,m_MoniIndex.nIndex + 1, &DBMoni2);
	#else
	CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE,m_MoniIndex.nIndex + 1, &DBMoni2);
	#endif
    
	if(m_MoniIndex.nCount < CtmConfig::GetInstance()->GetMoniMaxNum())	m_MoniIndex.nCount ++;
	m_MoniIndex.nIndex ++;
	if(m_MoniIndex.nIndex >= CtmConfig::GetInstance()->GetMoniMaxNum())	m_MoniIndex.nIndex = 0;
	
	#ifdef	D_3354	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MONI, 0, &m_MoniIndex, sizeof(m_MoniIndex));
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE,0, &m_MoniIndex);
	#else
	CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_MONI_SRAMFILE,0, &m_MoniIndex);
	#endif	

	pthread_mutex_unlock(&m_MutexMoni);
	
	return	TRUE;
}

#ifndef	D_BEOBJECT_CTMCONTROL_MONITOR
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
	{
		(static_cast<CtmPackClass*>(pResult))->CreateSelf();
	}
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmPackClass*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}

#endif
