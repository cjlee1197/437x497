/*===========================================================================+
|  Class    : CtmControl_Function	                                        	 |
|  Task     : Mold Service Function	    	                     			 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 01/09/2009	                                                 |
|  Revision : 01/09/2009 				                                     |
+===========================================================================*/
#include	"tmControl_Function.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"
#include	"../../selvat_ancestor.h"
#include	"../../iotable.h"
#include	"../../tmshmsg.h"

IMPLEMENT_DYNCREATE(CtmControl_Function, CtmControl)

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
char*			g_apszOnTimeHeater[] = {
	"HEAT_TEMPER_HEATERON_NULL_SUNPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_SUNPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_SUNPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_SUNPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_SUNPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_SUNPREWARMMIN",
	"HEAT_TEMPER_HEATERON_NULL_MONPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_MONPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_MONPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_MONPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_MONPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_MONPREWARMMIN",
	"HEAT_TEMPER_HEATERON_NULL_TUEPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_TUEPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_TUEPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_TUEPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_TUEPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_TUEPREWARMMIN",
	"HEAT_TEMPER_HEATERON_NULL_WEDPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_WEDPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_WEDPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_WEDPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_WEDPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_WEDPREWARMMIN",
	"HEAT_TEMPER_HEATERON_NULL_THRPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_THRPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_THRPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_THRPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_THRPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_THRPREWARMMIN",
	"HEAT_TEMPER_HEATERON_NULL_FRIPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_FRIPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_FRIPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_FRIPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_FRIPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_FRIPREWARMMIN",
	"HEAT_TEMPER_HEATERON_NULL_SATPREWARMOPTION",
	"TIME_TEMPER_HEATERON_NULL_SATPREWARMHOUR",
	"TIME_TEMPER_HEATERON_NULL_SATPREWARMMIN",
	"HEAT_TEMPER_HEATEROFF_NULL_SATPREWARMOPTION",
	"TIME_TEMPER_HEATEROFF_NULL_SATPREWARMHOUR",
	"TIME_TEMPER_HEATEROFF_NULL_SATPREWARMMIN"
};

BOOL	m_bHeaterPowerAutoOn 	= FALSE;          //wy102999 for heater auto on
BOOL	m_bHeaterPowerAutoOff 	= FALSE;          //yuc add <2008/5/26 01:24下午>
BOOL	m_bHeaterOnFlag = FALSE;	
BOOL	m_bHeaterOffFlag = FALSE;

BOOL	m_bDryingOnFlag = FALSE;//SLJ2016-12-19 10:40:49	
BOOL	m_bDryingOffFlag = FALSE;
BOOL	m_bDryingPowerAutoOn 	= FALSE;
BOOL	m_bDryingPowerAutoOff 	= FALSE;
unsigned char on_dryinghour=0;
unsigned char on_dryingminu=0;
unsigned char off_dryinghour=0;
unsigned char off_dryingminu=0;

/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/	
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Function::CtmControl_Function()				     |
|  Task     :  CtmControl_Function構造函數                                   |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Function::CtmControl_Function(): CtmControl()
{
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Function::~CtmControl_Function()				     |
|  Task     :  CtmControl_Function析構函數                                   |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Function::~CtmControl_Function()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Function::CreateSelf()				             	 |
|  Task     :  Initialize                                                    |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Function::CreateSelf()
{
	InitControl();
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Function::FreeSelf()				             	 |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Function::FreeSelf()
{
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Function::InitControl()					         |
|  Task     :  Init              					                     	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_Function::InitControl()
{
	
}

/*---------------------------------------------------------------------------+
|  Function :  void   SetCoreEffectPN()                                      |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  SetCoreEffectPN()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmControl_Function::_SetCoreEffectPN(BOOL bSendDSP)
{
	int         i =0;
	WORD        wIndex =0, wDataID =0, wValue =0, wCoreShift =0, wCoreRank =0;
	long        l_OpnMaxTral =0, l_OpnMinTral =0;
	float   tempv = 0;

	if(g_pExTable->GetTableValue(0,"D_7LL")) /*2012-6-20 */
	{
		l_OpnMaxTral =GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED71").lValue;
		l_OpnMinTral =GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED72").lValue;
	}
	else if(g_pExTable->GetTableValue(0,"D_SPCMLD"))  //tyl 2014.02.24
	{
		tempv = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED71").lValue;
		tempv = tempv/10;
		l_OpnMaxTral =(long)(GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue*tempv);
		l_OpnMinTral =(long)(GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue*tempv);
	}
	else if(g_pExTable->GetTableValue(0,"D_BOARDSPC"))
	{
		l_OpnMaxTral =(long)(GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue*1.5);
		l_OpnMinTral =(long)(GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue*1.5);
	}
	else 	if(g_pExTable->GetTableValue(0,"D_OPNMAXMINEFFPOS") || g_pExTable->GetTableValue(0,"D_SZFTVRT") || g_pExTable->GetTableValue(0,"D_7AKFY")) /*2012-12-18 14:30:48 *//*2016-6-22 豐鐵重新使用正負有效區*/
	{
		l_OpnMaxTral =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50").lValue;
		l_OpnMinTral =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue;
	}
	else
	{
		l_OpnMaxTral = l_OpnMinTral = GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue;
    }

	wCoreRank = GetDBValue("MCONF_COREPULLER_COREPULLER_NULL_SET").lValue;
	for ( i=0; i<wCoreRank; i++)                         //Steven 2004/3/22 add
    {
	    wCoreShift = (g_pDatabase->GetDataID("MSET_CORB_CORB_NULL_CONTROLOPTION") - g_pDatabase->GetDataID("MSET_CORA_CORA_NULL_CONTROLOPTION"))*i;
	    wDataID = g_pDatabase->GetDataID("MSET_CORA_IN_NULL_OILPLUSPOSITIONOFFSET")+wCoreShift;
	    wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_CORA_IN_NULL_ACTPOSITIONOPTION") + wCoreShift);
	    
	    switch (GetDBValue(wIndex).lValue)
        {
        case 0:
            wValue = GetOilVat(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue+l_OpnMaxTral);
            if(g_pExTable->GetTableValue(0,"D_SCLMPM"))/*中鴻泰 開模位置有效區 倍數處理*/
            	wValue =wValue/2;
            else if(g_pExTable->GetTableValue(0,"D_SPCMLD"))  //tyl 2014.02.24
            	wValue =(WORD)(wValue/tempv);
             else if(g_pExTable->GetTableValue(0,"D_BOARDSPC"))
            	wValue =(WORD)(wValue/1.5);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);

            wValue = GetOilVat(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue-l_OpnMinTral);
            if(g_pExTable->GetTableValue(0,"D_SCLMPM"))
            	wValue =(WORD)(wValue/2);
            else if(g_pExTable->GetTableValue(0,"D_SPCMLD"))  //tyl 2014.02.24
            	wValue =(WORD)(wValue/tempv);
             else if(g_pExTable->GetTableValue(0,"D_BOARDSPC"))
            	wValue =(WORD)(wValue/1.5);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        case 1:
        	wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_CORA_IN_NULL_SETMOLDPOSITION") + wCoreShift);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue + l_OpnMaxTral);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue - l_OpnMinTral);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        default:
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID), 0,FALSE);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), 0,FALSE);
            break;
        }
    	if (bSendDSP)
        {
       		wIndex = g_pDatabase->DataIDToIndex(wDataID);
       		//James modify 2008/6/6 09:06上午
   		     g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
   		     //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
   		     wIndex = g_pDatabase->DataIDToIndex(wDataID+1);
   		     g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
   		     //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
        }
        
	    wDataID = g_pDatabase->GetDataID("MSET_CORA_OUT_NULL_OILPLUSPOSITIONOFFSET") + wCoreShift;
	    wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_CORA_OUT_NULL_ACTPOSITIONOPTION") + wCoreShift);
	    switch (GetDBValue(wIndex).lValue)
        {
        case 0:
        	wValue = GetOilVat(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue + l_OpnMaxTral);
            if(g_pExTable->GetTableValue(0,"D_SCLMPM"))
            	wValue =wValue/2;
            else if(g_pExTable->GetTableValue(0,"D_SPCMLD"))  //tyl 2014.02.24
            	wValue =(WORD)(wValue/tempv);
            else if(g_pExTable->GetTableValue(0,"D_BOARDSPC"))
            	wValue =(WORD)(wValue/1.5);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);

            wValue = GetOilVat(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue - l_OpnMinTral);
            if(g_pExTable->GetTableValue(0,"D_SCLMPM"))
            	wValue =wValue/2;
            else if(g_pExTable->GetTableValue(0,"D_SPCMLD"))  //tyl 2014.02.24
            	wValue =(WORD)(wValue/tempv);
            else if(g_pExTable->GetTableValue(0,"D_BOARDSPC"))
            	wValue =(WORD)(wValue/1.5);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        case 1:
        	wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_CORA_OUT_NULL_SETMOLDPOSITION") + wCoreShift);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue + l_OpnMaxTral);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue - l_OpnMinTral);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        default:
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID), 0,FALSE);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), 0,FALSE);
            break;
        }
    	//James modify 2008/6/6 09:06上午
    	if (bSendDSP)
        {
	        wIndex = g_pDatabase->DataIDToIndex(wDataID);
	        g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
	        //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
	        wIndex = g_pDatabase->DataIDToIndex(wDataID+1);
	        g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
	        //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
        }
    }

  if(CtmConfig::GetInstance()->GetCom2Type() == 1)
  	_SetCoreEffectPN2(bSendDSP);
}

void      CtmControl_Function::_SetEjectEffectPN(BOOL bSendDSP)
{
	_AD         adOpenEndPos =0;                // 033E 開模五段終止位置      
	WORD		wIndex = 0;     
	long        l_OpnMaxTral =0, l_OpnMinTral =0;
	
	adOpenEndPos = GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue;
	if(g_pExTable->GetTableValue(0,"D_BOARDSPC")) 
		adOpenEndPos =(_AD)(adOpenEndPos/1.5);

	if(g_pExTable->GetTableValue(0,"D_7WE")) /*//Leedan2012-6-19*/
	{
		l_OpnMaxTral = 2000;
		l_OpnMinTral = 40;
	}
	else	if(g_pExTable->GetTableValue(0,"D_7LL")) /*2012-6-20 */
	{
		l_OpnMaxTral =GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED71").lValue;
		l_OpnMinTral =GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED72").lValue;
	}
	else 	if(g_pExTable->GetTableValue(0,"D_7FTNEGCLMP")) /* zyh  2013-12-30 for 7FT 開模有效區無負有效區*/
	{
	 	l_OpnMaxTral =GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue;
		l_OpnMinTral =0;
	}	
	else 	if(g_pExTable->GetTableValue(0,"D_OPNMAXMINEFFPOS") || g_pExTable->GetTableValue(0,"D_SZFTVRT") || g_pExTable->GetTableValue(0,"D_7AKFY")) /*2012-12-18 14:30:54 *//*2016-6-22 豐鐵重新使用正負有效區*/
	{
		l_OpnMaxTral =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50").lValue;
		l_OpnMinTral =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue;
	}
	else
	{
		l_OpnMaxTral = l_OpnMinTral =GetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION").lValue;
	}

	SetDBValue("MSET_MLD_OPEN_END_OILPLUSPOSITIONOFFSET", GetOilVat(adOpenEndPos+l_OpnMaxTral),FALSE);
	SetDBValue("MSET_MLD_OPEN_END_OILMINUSPOSITIONOFFSET", GetOilVat(adOpenEndPos-l_OpnMinTral),FALSE);
	// James modify 2008/6/6 09:07上午
	if (bSendDSP)
    {
    	wIndex = g_pDatabase->GetIndex("MSET_MLD_OPEN_END_OILPLUSPOSITIONOFFSET");
    	g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX,wIndex);
    	//g_ptaskrs232->SendControlIndex("MSET_MLD_OPEN_END_OILPLUSPOSITIONOFFSET");
    	wIndex = g_pDatabase->GetIndex("MSET_MLD_OPEN_END_OILMINUSPOSITIONOFFSET");
    	g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX,wIndex);
    	//g_ptaskrs232->SendControlIndex("MSET_MLD_OPEN_END_OILMINUSPOSITIONOFFSET");
    }

  if(CtmConfig::GetInstance()->GetCom2Type() == 1)
  	_SetEjectEffectPN2(bSendDSP);
}
/*---------------------------------------------------------------------------+
|  Function :  void   SetCoreEffectPN2()                                      |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  SetCoreEffectPN2()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmControl_Function::_SetCoreEffectPN2(BOOL bSendDSP)
{
	int         i =0;
	WORD        wIndex =0, wDataID =0, wValue =0, wCoreShift =0, wCoreRank =0;

	wCoreRank = GetDBValue("MCONF_2_COREPULLER_COREPULLER_NULL_SET").lValue;
	for ( i=0; i<wCoreRank; i++)                         //Steven 2004/3/22 add
    {
	    wCoreShift = (g_pDatabase->GetDataID("MSET_2_CORB_CORB_NULL_CONTROLOPTION") - g_pDatabase->GetDataID("MSET_2_CORA_CORA_NULL_CONTROLOPTION"))*i;
	    wDataID = g_pDatabase->GetDataID("MSET_2_CORA_IN_NULL_OILPLUSPOSITIONOFFSET")+wCoreShift;
	    wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_2_CORA_IN_NULL_ACTPOSITIONOPTION") + wCoreShift);
	    
	    switch (GetDBValue(wIndex).lValue)
        {
        case 0:
            wValue = GetOilVat(GetDBValue("MSET_2_MLD_OPEN_STEP5_POSITION").lValue+GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);
            wValue = GetOilVat(GetDBValue("MSET_2_MLD_OPEN_STEP5_POSITION").lValue-GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        case 1:
        	wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_2_CORA_IN_NULL_SETMOLDPOSITION") + wCoreShift);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue + GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue - GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        default:
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID), 0,FALSE);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), 0,FALSE);
            break;
        }
    	if (bSendDSP)
        {
       		wIndex = g_pDatabase->DataIDToIndex(wDataID);
       		//James modify 2008/6/6 09:06上午
       		if(g_ptaskCom2dsp != NULL)
   		     g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
   		     //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
   		     wIndex = g_pDatabase->DataIDToIndex(wDataID+1);
   		     if(g_ptaskCom2dsp != NULL)
   		     g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
   		     //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
        }
        
	    wDataID = g_pDatabase->GetDataID("MSET_2_CORA_OUT_NULL_OILPLUSPOSITIONOFFSET") + wCoreShift;
	    wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_2_CORA_OUT_NULL_ACTPOSITIONOPTION") + wCoreShift);
	    switch (GetDBValue(wIndex).lValue)
        {
        case 0:
        	wValue = GetOilVat(GetDBValue("MSET_2_MLD_OPEN_STEP5_POSITION").lValue + GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);
            wValue = GetOilVat(GetDBValue("MSET_2_MLD_OPEN_STEP5_POSITION").lValue - GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        case 1:
        	wIndex = g_pDatabase->DataIDToIndex(g_pDatabase->GetDataID("MSET_2_CORA_OUT_NULL_SETMOLDPOSITION") + wCoreShift);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue + GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID), wValue,FALSE);
        	wValue = GetOilVat(GetDBValue(wIndex).lValue - GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue);
        	SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), wValue,FALSE);
            break;
        default:
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID), 0,FALSE);
            SetDBValue(g_pDatabase->DataIDToIndex(wDataID + 1), 0,FALSE);
            break;
        }
    	//James modify 2008/6/6 09:06上午
    	if (bSendDSP)
        {
	        wIndex = g_pDatabase->DataIDToIndex(wDataID);
	        if(g_ptaskCom2dsp != NULL)
	        g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
	        //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
	        wIndex = g_pDatabase->DataIDToIndex(wDataID+1);
	        if(g_ptaskCom2dsp != NULL)
	        g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX,g_pDatabase->GetString(wIndex));
	        //g_ptaskrs232->SendControlIndex(g_pDatabase->GetString(wIndex));
        }
    }
}

void      CtmControl_Function::_SetEjectEffectPN2(BOOL bSendDSP)
{
	_AD         adOpenEndPos =0;                // 033E 開模五段終止位置      
	WORD		wIndex = 0; 
	
	adOpenEndPos = GetDBValue("MSET_2_MLD_OPEN_STEP5_POSITION").lValue;
	SetDBValue("MSET_2_MLD_OPEN_END_OILPLUSPOSITIONOFFSET", GetOilVat(adOpenEndPos+GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue),FALSE);
	SetDBValue("MSET_2_MLD_OPEN_END_OILMINUSPOSITIONOFFSET", GetOilVat(adOpenEndPos-GetDBValue("MACH_2_COR_COR_NULL_EFFECTPOSITION").lValue),FALSE);
	// James modify 2008/6/6 09:07上午
	if (bSendDSP)
    {
    	wIndex = g_pDatabase->GetIndex("MSET_2_MLD_OPEN_END_OILPLUSPOSITIONOFFSET");
    	if(g_ptaskCom2dsp != NULL)
    	g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX,wIndex);
    	//g_ptaskrs232->SendControlIndex("MSET_MLD_OPEN_END_OILPLUSPOSITIONOFFSET");
    	wIndex = g_pDatabase->GetIndex("MSET_2_MLD_OPEN_END_OILMINUSPOSITIONOFFSET");
    	if(g_ptaskCom2dsp != NULL)
    	g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX,wIndex);
    	//g_ptaskrs232->SendControlIndex("MSET_MLD_OPEN_END_OILMINUSPOSITIONOFFSET");
    }
}

void		CtmControl_Function::_OnTimeHeater()
{
	tmDATE	Date;
	tmTIME	Time;
	GetDate(&Date);
	
	
	if (Date.dayofweek > 6 || Date.dayofweek < 0) 
		return;
		
	//unsigned char HeaterPowerAutoOff=0;
	unsigned char on_hour=0;
	unsigned char on_minu=0;
	unsigned char off_hour=0;
	unsigned char off_minu=0;
	on_hour		=(BYTE)GetDBValue(g_apszOnTimeHeater[Date.dayofweek*6+1]).lValue;
	on_minu		=(BYTE)GetDBValue(g_apszOnTimeHeater[Date.dayofweek*6+2]).lValue;
	off_hour		=(BYTE)GetDBValue(g_apszOnTimeHeater[Date.dayofweek*6+4]).lValue;
	off_minu		=(BYTE)GetDBValue(g_apszOnTimeHeater[Date.dayofweek*6+5]).lValue;
	
	m_bHeaterPowerAutoOn = GetDBValue(g_apszOnTimeHeater[Date.dayofweek*6]).lValue;
	m_bHeaterPowerAutoOff= GetDBValue(g_apszOnTimeHeater[Date.dayofweek*6+3]).lValue;
	GetTime(&Time);	
	//printf("  AutoOn=%d\t,  AutoOff=%d\n  %d:%d.    %d:%d.\n",m_bHeaterPowerAutoOn,m_bHeaterPowerAutoOff,on_hour,on_minu,off_hour,off_minu);
	/*---------------------------*/
	// only do once
	/*---------------------------*/
	if(m_bHeaterPowerAutoOn)
	{
		if(Time.hour >on_hour || (Time.hour == on_hour && Time.minute >=on_minu))
		{
			//printf("  on\n");
			if(!m_bHeaterOnFlag)
			{
				if(!g_flHeaterStatus)
	    		{
	    			//printf("  1send on\n");
	    			g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_HEATER & 0x00FF); // Directly send panel command to avoid keyboard buffer full
	    			usleep(100*1000);
	    			g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_BREAK  & 0x00FF);
	    			m_bHeaterOnFlag=TRUE;
	    		}
	    	}
	    }
	    else m_bHeaterOnFlag=FALSE;
	}
	else m_bHeaterOnFlag = FALSE;	//	fans add 2008/10/17 10:48上午
		
	if(m_bHeaterPowerAutoOff)
	{	
	    if(Time.hour >off_hour || (Time.hour == off_hour && Time.minute >= off_minu))
	    {
	    	//printf("  off\n");
			if(!m_bHeaterOffFlag)
			{
				if(g_flHeaterStatus)
	    		{
	    			//printf("  2send off\n");
	    			g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_HEATEROFF & 0x00FF); // Directly send panel command to avoid keyboard buffer full
	    			usleep(100*1000);
	    			g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_BREAK  & 0x00FF);
	    			m_bHeaterOffFlag=TRUE;
	    			
	    			if(g_pExTable->GetTableValue(0,"D_7EY"))
	    			{
	    				if(!GetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL41DATA").lValue)//SBJ for 7EY 20141212
	    					SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL41DATA", 1);
	    			}
	    		}
	    	}
	    }
	    else 
	    {
	    	m_bHeaterOffFlag=FALSE;
	    	if(g_pExTable->GetTableValue(0,"D_7EY"))
	    	{
	    		if(GetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL41DATA").lValue)
	    			SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL41DATA", 0);
	    	}
	    }
	}
	else 
	{
		m_bHeaterOffFlag = FALSE;	//	fans add 2008/10/17 10:48上午
		if(g_pExTable->GetTableValue(0,"D_7EY"))
	    	SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL41DATA", 0);
	}
	
	
	if(g_pExTable->GetTableValue(0,"D_DRYING"))/* SLJ 2016-12-21 for  7FK_611  tempheat*/
	{
		int temp = GetDBValue("SYSX_OTHERS_OTHERS_FLOAT_RESERVED2").lValue;
		if(temp == 1234)
		{
			WORD	wTempData[42];
 	    	char	szPath[256];
	    	char	m_szCopyFile[256];
        	char	pszDB[256];
        	     
        	memset(wTempData, 0, sizeof(wTempData));
	    	memset(pszDB, 0, sizeof(szPath));
	    	memset(szPath, 0, sizeof(szPath));
	    	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	    	sprintf(szPath, "%s", "/usr/Data");
	    	sprintf(m_szCopyFile, "%s/tempdata.cdb", szPath);
	    	 
	    	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
        	
			if (nFd >= 0)
			{
				read(nFd, &wTempData, sizeof(wTempData));
			}
 			close(nFd);
        	
 			
			on_dryinghour	=(BYTE)wTempData[Date.dayofweek];
			on_dryingminu	=(BYTE)wTempData[Date.dayofweek+7];
			off_dryinghour	=(BYTE)wTempData[Date.dayofweek+21];
			off_dryingminu	=(BYTE)wTempData[Date.dayofweek+28];
			m_bDryingPowerAutoOn = wTempData[Date.dayofweek+14];
			m_bDryingPowerAutoOff= wTempData[Date.dayofweek+35];
			
			SetDBValue("SYSX_OTHERS_OTHERS_FLOAT_RESERVED2", 0);
		}
		if(m_bDryingPowerAutoOn)
		{
			if(Time.hour >on_dryinghour || (Time.hour == on_dryinghour && Time.minute >=on_dryingminu))
			{
				if(!m_bDryingOnFlag)
				{
	    			//printf("DRYING ON\n");
	    			SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL5DATA", 1);
	    			m_bDryingOnFlag=TRUE;

	    		}
	    	}
	    	else m_bDryingOnFlag=FALSE;
		}
		else m_bDryingOnFlag = FALSE;
			
		if(m_bDryingPowerAutoOff)
		{	
	    	if(Time.hour >off_dryinghour || (Time.hour == off_dryinghour && Time.minute >= off_dryingminu))
	    	{
				if(!m_bDryingOffFlag)
				{
	    			//printf("DRYING OFF\n");
	    			SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL5DATA", 0);
	    			m_bDryingOffFlag=TRUE;
	    		}
	    	}
	    	else  m_bDryingOffFlag=FALSE;
		}
		else m_bDryingOffFlag = FALSE;	
		
	}
}


void	CtmControl_Function::_CountZero()
{
	DWORD		dwValue;
	DWORD		dwProductPerCount= GetDBValue(TECH_GENER_PRODUCTPERCOUNT).lValue;
	DWORD		dwShotCount		 = GetDBValue(TECH_GENER_SETSHOTCOUNT).lValue;
	static		DWORD	dwStartPackCount= 0;
	
	if (GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue < dwStartPackCount)		
		dwStartPackCount = GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue;
	if (dwShotCount > 0 && dwShotCount <= GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue * dwProductPerCount)
		{
		dwValue = GetDBValue(TECH_MLD_GENER_OPEN_TOTALSETZERO).lValue | SHOTCOUNT_SETZERO;
		SetDBValue(TECH_MLD_GENER_OPEN_TOTALSETZERO, dwValue);
		dwValue = dwValue - SHOTCOUNT_SETZERO;
		g_pDatabase->Write(TECH_MLD_GENER_OPEN_TOTALSETZERO, &dwValue);
		}
	if (GetDBValue(TECH_GENER_PACKSHOTCOUNT).lValue > 0 &&
		GetDBValue(TECH_GENER_PACKSHOTCOUNT).lValue <= (GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue-dwStartPackCount) * dwProductPerCount)
		{
		dwValue = GetDBValue(TECH_MLD_GENER_OPEN_TOTALSETZERO).lValue | PACKCOUNT_SETZERO;
		SetDBValue(TECH_MLD_GENER_OPEN_TOTALSETZERO, dwValue);
		dwStartPackCount = GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue;
		dwValue = GetDBValue(TECH_MLD_GENER_OPEN_TOTALSETZERO).lValue - PACKCOUNT_SETZERO;
		g_pDatabase->Write(TECH_MLD_GENER_OPEN_TOTALSETZERO, &dwValue);
		}
	if (GetDBValue(TECH_GENER_PACKSHOTCOUNT).lValue > 0 && GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue > 0)
		SetDBValue(TECH_INTERFACE_PACKSHOTCOUNT, (GetDBValue(TECH_INTERFACE_SHOTCOUNT).lValue-dwStartPackCount) * dwProductPerCount);
}

void	CtmControl_Function::_ClearZeroOil()
{
	//DWORD		dwSetShotCount	 = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT").lValue;
	//DWORD		dwRealShotCount  = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	//DWORD		dwSetPackCount	 = GetDBValue("PROD_PRODUCT_PACK_NULL_SETSHOUCOUNT").lValue;
	//
	//static		DWORD	dwStartPackCount= 0;
	//
	//if (dwRealShotCount < dwStartPackCount)
	//	dwStartPackCount = dwRealShotCount;
	//if (dwSetShotCount > 0 && dwSetShotCount <= dwRealShotCount)
	//	{
	//	g_ptaskrs232->SendStatus(COMM_S_REACHSHOTCOUNT/*Reach shot count*/);
	//	}
	//if (dwSetPackCount > 0 && dwSetPackCount <= (dwRealShotCount - dwStartPackCount))
	//	{
	//	SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", 0);
	//	dwStartPackCount = dwRealShotCount;
	//	g_ptaskrs232->SendStatus(COMM_S_REACHPACKCOUNT/*Reach pack count*/);
	//	}
}

/*---------------------------------------------------------------------------+
|  Function :  int   GetStageInstall()                                       |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  GetStageInstall()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    CtmControl_Function::_GetInitPasswd_Install(char* pcMachineID, char* pcPassword, 
							tmDATE dtMachineDateOut)
{
	//LEO20070404
    GetInitialPassword(pcMachineID, pcPassword, dtMachineDateOut,  VER2HEX());///Sunny<20080630>
    					//(WORD)GetDBValue("VERS_HMI_NULL_NULL_PRGORAMSYSTEM").lValue);
}

/*---------------------------------------------------------------------------+
|  Function :  int   GetStageInstall()                                       |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  GetStageInstall()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int       CtmControl_Function::_GetStage_Install(char* pcPassword, char* pcMachineID, 
							tmDATE dtStartStage)
{
	//LEO20070404
	//printf("stage :%d\n",GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue);
    return GetStageInstall(pcPassword, pcMachineID, dtStartStage, 
    						GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue,VER2HEX());///Sunny<20080630>
    						//GetDBValue("VERS_HMI_NULL_NULL_PRGORAMSYSTEM").lValue);
}

/*---------------------------------------------------------------------------+
|  Function :  int   CheckTimeOut_Install()                                  |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  CheckTimeOut_Install()                                        |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL        CtmControl_Function::_CheckTimeOut_Install()
{
	//LEO20070404
	tmDATE    date, dtMachineSell, dtTimeOut, dtTimeoutAlarm;
	tmTIME		time, dtOutTime;
	long		  lDaysTimeoutPeriod = 0, lStageNow = 0, lDaysTimeoutAlarmPeriod = 0;
	BOOL		  bResult = FALSE;

	GetDate(&date);
	GetTime(&time);
	dtMachineSell.year  = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR").lValue;
	dtMachineSell.month = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH").lValue;
	dtMachineSell.day   = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY").lValue;
	dtTimeOut.year  	= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue;
	dtTimeOut.month 	= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH").lValue;
	dtTimeOut.day   	= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY").lValue;
	dtTimeoutAlarm.year = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue;
	dtTimeoutAlarm.month= GetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH").lValue;
	dtTimeoutAlarm.day  = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY").lValue;
	lDaysTimeoutPeriod  = GetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD").lValue;
	lStageNow			= GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;
	lDaysTimeoutAlarmPeriod = GetDBValue("OPER_HMI_NULL_NULL_DAYSALARMPERIOD").lValue;

	dtOutTime.hour 		= GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPHOUR").lValue;     
	dtOutTime.minute 	= GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPMINUTE").lValue;
	dtOutTime.second 	= GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPSECOND").lValue;         
	
	GetAccDate(&dtMachineSell, lDaysTimeoutPeriod*lStageNow, &dtTimeOut);
	GetAccDate(&dtTimeOut, -lDaysTimeoutAlarmPeriod, &dtTimeoutAlarm);

	if((date.year >  dtTimeOut.year) || 
	  ((date.year == dtTimeOut.year) && (date.month > dtTimeOut.month)) || 
	  ((date.year == dtTimeOut.year) && (date.month == dtTimeOut.month) && (date.day > dtTimeOut.day))||
	  ((date.year == dtTimeOut.year) && (date.month == dtTimeOut.month) && (date.day == dtTimeOut.day) &&
	  (time.hour > dtOutTime.hour))  || 
	  ((date.year == dtTimeOut.year) && (date.month == dtTimeOut.month) && (date.day == dtTimeOut.day) &&
	  (time.hour == dtOutTime.hour)  && (time.minute > dtOutTime.minute)) ||
	  ((date.year == dtTimeOut.year) && (date.month == dtTimeOut.month) && (date.day == dtTimeOut.day) &&
	  (time.hour == dtOutTime.hour)  && (time.minute == dtOutTime.minute) && (time.second >= dtOutTime.second)))
	  		bResult = TRUE;
	    
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR"	 , dtMachineSell.year  );
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH" , dtMachineSell.month );
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY"	 , dtMachineSell.day   );
	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR"		 , dtTimeOut.year  	   );
	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH"	 , dtTimeOut.month 	   );
	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY"		 , dtTimeOut.day   	   );
	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR" , dtTimeoutAlarm.year );
	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH", dtTimeoutAlarm.month);
	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY"	 , dtTimeoutAlarm.day  );    
	    
	return	bResult;
}

/*---------------------------------------------------------------------------+
|  Function :  WORD   StartUp_Install()                                      |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  StartUp_Install()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD       CtmControl_Function::_StartUp_Install(char* pcPassword)
{
	//LEO20070404
	WORD        wPrivilege = 0;
	int         nStage;
	char        szinitial[10], szMachineID[10];
	tmDATE      dtMachineSell, dtTimeOut;
	BOOL		bTimeOut = FALSE;
	dtMachineSell.year  = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR").lValue;
	dtMachineSell.month = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH").lValue;
	dtMachineSell.day   = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY").lValue;
	
	dtTimeOut.year  	= GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR").lValue;
	dtTimeOut.month 	= GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH").lValue;
	dtTimeOut.day   	= GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY").lValue;
	
	GetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
	GetInitialPassword(szMachineID, szinitial, dtTimeOut,VER2HEX());///Sunny<20080630>
	
	//printf("szMachineID:%s ;szinitial:%s ;pcPassword:%s ;dtTimeOut:%d \n",szMachineID,szinitial,pcPassword,dtTimeOut);
	if (strcmp(szinitial, pcPassword) == 0)//initial Password
    {
    	wPrivilege =  MAX_PRIVILEGE;
    }
	else if ((nStage = GetStage_Install(pcPassword, szMachineID, dtMachineSell)) > 0) //Stage Password
    {
		if (nStage <= GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue)
        {
        	if (nStage == GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue)
            {
            	//printf("SetDBValue:TIMEOUTFLAG FALSE :%d \n",bTimeOut);
            	bTimeOut =FALSE;//Sunny<20080829>
             char	psz1[4];
		         memset(psz1,0,sizeof(psz1));        
	           GetDBString("VERS_HMI_NULL_NULL_PROGRAMMMIVERS",psz1,3);// YANGJX 2010-3-15
	           
	           if(g_pExTable->GetTableValue(0,"D_MACTYPE")) //YANGY2010-1-13 17:
            {
	         	  ;	
            }	
            else
            {
	          if(strcmp(psz1,"7LU") !=0)
            	SetDBString("OPER_HMI_NULL_NULL_MACHINEID","MACH0000", 9);//Sunny<20080829>
            }
            
	        	SetDBValue("OPER_HMI_NULL_NULL_STAGES", g_pDatabase->ReadDefault("OPER_HMI_NULL_NULL_STAGES").dwData);//Sunny<20080829>
	        	SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", g_pDatabase->ReadDefault("OPER_HMI_NULL_NULL_STAGENOW").dwData);//Sunny<20080829>
            	SetDBValue("OPER_HMI_NULL_NULL_STATE", 0);
            	SetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG", FALSE);
            }
            else if(nStage <= GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue)//Sunny<20080829>
            {
            	//printf("nStage is time out! \n");
            	bTimeOut =TRUE;
            }
			else if (nStage > GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue)
            {
            	SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", nStage);
            	bTimeOut = CheckTimeOut_Install();
            	//printf("nStage>STAGENOW---bTimeOut:%d \n",bTimeOut);
            	SetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG",bTimeOut);
            }    	
        	SaveOperation();
        	if(bTimeOut)	wPrivilege = 0;
        	else			wPrivilege = 1;
        }
        else	 wPrivilege = 0;
	}
	else	wPrivilege = 0;

	return wPrivilege;
}

/*---------------------------------------------------------------------------+
|  Function :  WORD   JudgeFactoryOnWork_Install()                           |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  JudgeFactoryOnWork_Install()                                  |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD		CtmControl_Function::_JudgeFactoryOnWork_Install()
{
	//LEO20070404
	tmTIME	ttime;

	GetTime(&ttime);
	if( ( ttime.hour   >  GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPHOUR").lValue) ||
		((ttime.hour   == GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPHOUR").lValue) &&
		( ttime.minute >= GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPMINUTE").lValue)))
		return		0;
	else	return		1;
}

/*---------------------------------------------------------------------------+
|  Function :  void   SaveOperation(OPERATION* pOperation, int nWRflag)      |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  SaveOperation()                                  			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CtmControl_Function::_SaveOperation()
{
	//LEO20070404
	OPERATION  operation;
  memset(&operation, 0, sizeof(operation));	
	SetDBValue("OPER_HMI_NULL_NULL_CRC", CountOperationCRC());
	ReadOperationFromSRAM(&operation);
	SaveCurrentRTCTime(&operation);
	DealOperation(&operation, 1);
}
/*---------------------------------------------------------------------------+
|  Function :  void   DealOperation(OPERATION* pOperation, int nWRflag)      |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  DealOperation()                                  			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CtmControl_Function::_DealOperation(OPERATION* pOperation, int nWRflag)
{
	//LEO20070404
	int 		fd; 
	
	if (pOperation == NULL)	return;
	#ifdef D_PXA270
	if ((fd = open("/etc/init.d/install", O_RDWR|O_CREAT)) < 0)
	#else
	if ((fd = open("/conf/install", O_RDWR|O_CREAT)) < 0)
	#endif
	{
		//	printf("install open error, %s\n", strerror(errno));
		return;
	}
	if(lseek(fd, 0, 0) < 0)		
		;//printf("could not seek file, %s\n", strerror(errno));
	if(nWRflag == 1)
	{
		if (g_wInstWay == 1) //Sunny<20090421> inst use month way
			pOperation->cDaysTimeoutPeriod |= 0x1000;
		if(write(fd, (char *)pOperation, sizeof(OPERATION)) != sizeof(OPERATION))	
			;//printf("could not read install, %s\n", strerror(errno));
	}
	else
	{
		if(read(fd, (char *)pOperation, sizeof(OPERATION)) != sizeof(OPERATION))	
			;//printf("could not read install, %s\n", strerror(errno));
		if (pOperation->cDaysTimeoutPeriod & 0x1000)//Sunny<20090421> inst use month way
		{
			g_wInstWay=1;
			pOperation -> cDaysTimeoutPeriod &= 0x0FFF;
		}
		else g_wInstWay = 0;
	}
	close(fd);
}

/*---------------------------------------------------------------------------+
|  Function :  int    ReadOperationFromSRAM(OPERATION* pOperation)    		 |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  ReadOperationFromSRAM()                            			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Function::_ReadOperationFromSRAM(OPERATION* pOperation)
{
	//LEO20070404
	if(pOperation == NULL)	return	-1;
	pOperation->dwFlag = GetDBValue("OPER_HMI_NULL_NULL_FLAG").lValue;	
	GetDBString("OPER_HMI_NULL_NULL_MACHINEID", pOperation->cMachineID, 9);
	pOperation->dateMachineOut.year = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR").lValue;
	pOperation->dateMachineOut.month = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH").lValue;
	pOperation->dateMachineOut.day = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY").lValue;
	pOperation->dateMachineSell.year = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR").lValue;
	pOperation->dateMachineSell.month = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH").lValue;
	pOperation->dateMachineSell.day = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY").lValue;
	pOperation->cState = GetDBValue("OPER_HMI_NULL_NULL_STATE").lValue;
	pOperation->cStages = GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue;
	pOperation->cStageNow = GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;
	pOperation->cDaysTimeoutPeriod = GetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD").lValue;
	pOperation->dateTimeout.year = GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue;
	pOperation->dateTimeout.month = GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH").lValue;
	pOperation->dateTimeout.day = GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY").lValue;
	pOperation->cDaysTimeoutAlarmPeriod = GetDBValue("OPER_HMI_NULL_NULL_DAYSALARMPERIOD").lValue;
	pOperation->dateTimeoutAlarm.year = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue;
	pOperation->dateTimeoutAlarm.month = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH").lValue;
	pOperation->dateTimeoutAlarm.day = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY").lValue;
	pOperation->timeStop.hour = GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPHOUR").lValue;
	pOperation->timeStop.minute = GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPMINUTE").lValue;
	pOperation->timeStop.second = GetDBValue("OPER_HMI_NULL_NULL_TIMESTOPSECOND").lValue;
	pOperation->nMinuteTimeoutAlarmInterval = GetDBValue("OPER_HMI_NULL_NULL_ALARMINTERVAL").lValue;
	pOperation->lMinute = GetDBValue("OPER_HMI_NULL_NULL_MINUTE").lValue;
	pOperation->lMinuteMotor = GetDBValue("OPER_HMI_NULL_NULL_MINUTEMOTOR").lValue;
	pOperation->lMinuteAuto = GetDBValue("OPER_HMI_NULL_NULL_MINUTEAUTO").lValue;
	pOperation->bTimeout = GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue;
	pOperation->wCRC = GetDBValue("OPER_HMI_NULL_NULL_CRC").lValue;
	GetDBString("SYS_HMI_NULL_NULL_CUSTOMERID", pOperation->cMachineType, 10);// YANGJX 2010-1-18
	
	return	0;
}

/*---------------------------------------------------------------------------+
|  Function :  int    WriteOperationToSRAM(OPERATION* pOperation)    		 |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  WriteOperationToSRAM()                            			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Function::_WriteOperationToSRAM(OPERATION* pOperation)
{
	//LEO20070404
	if(pOperation == NULL)	return	-1;
	SetDBValue("OPER_HMI_NULL_NULL_FLAG", pOperation->dwFlag);
	SetDBString("OPER_HMI_NULL_NULL_MACHINEID", pOperation->cMachineID, 9);
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR", pOperation->dateMachineOut.year);
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH", pOperation->dateMachineOut.month);
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY", pOperation->dateMachineOut.day);
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR", pOperation->dateMachineSell.year);
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH", pOperation->dateMachineSell.month);
	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY", pOperation->dateMachineSell.day);
	SetDBValue("OPER_HMI_NULL_NULL_STATE", pOperation->cState);
	SetDBValue("OPER_HMI_NULL_NULL_STAGES", pOperation->cStages);
	SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", pOperation->cStageNow);
	SetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD", pOperation->cDaysTimeoutPeriod);
	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR", pOperation->dateTimeout.year);
	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH", pOperation->dateTimeout.month);
	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY", pOperation->dateTimeout.day);
	SetDBValue("OPER_HMI_NULL_NULL_DAYSALARMPERIOD", pOperation->cDaysTimeoutAlarmPeriod);
	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR", pOperation->dateTimeoutAlarm.year);
	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH", pOperation->dateTimeoutAlarm.month);
	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY", pOperation->dateTimeoutAlarm.day);
	SetDBValue("OPER_HMI_NULL_NULL_TIMESTOPHOUR", pOperation->timeStop.hour);
	SetDBValue("OPER_HMI_NULL_NULL_TIMESTOPMINUTE", pOperation->timeStop.minute);
	SetDBValue("OPER_HMI_NULL_NULL_TIMESTOPSECOND", pOperation->timeStop.second);
	SetDBValue("OPER_HMI_NULL_NULL_ALARMINTERVAL", pOperation->nMinuteTimeoutAlarmInterval);
	SetDBValue("OPER_HMI_NULL_NULL_MINUTE", pOperation->lMinute);
	SetDBValue("OPER_HMI_NULL_NULL_MINUTEMOTOR", pOperation->lMinuteMotor);
	SetDBValue("OPER_HMI_NULL_NULL_MINUTEAUTO", pOperation->lMinuteAuto);
	SetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG", pOperation->bTimeout);
	SetDBValue("OPER_HMI_NULL_NULL_CRC", pOperation->wCRC);
	SetDBString("SYS_HMI_NULL_NULL_CUSTOMERID", pOperation->cMachineType, 9);
	return	0;
}

/*---------------------------------------------------------------------------+
|  Function :  void		DealOperationStartMachine()				    		 |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  DealOperationStartMachine()                         			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CtmControl_Function::_DealOperationStartMachine()
{
	//LEO20070404
	OPERATION 	operation;
	memset(&operation, 0, sizeof(operation));
	int 		fd;            								    //JOYCE2009-11-18 
	BOOL		bExit  = FALSE;    								//JOYCE2009-11-18 
	DWORD		dwFlag = GetDBValue("OPER_HMI_NULL_NULL_FLAG").lValue;
	
	//<<<JOYCE2009-11-18 add
	#ifdef D_PXA270
	if ((fd = open("/etc/init.d/install", O_RDWR|O_CREAT)) < 0)
	#else
	if ((fd = open("/conf/install", O_RDWR|O_CREAT)) < 0)
	#endif
	{
		//printf("install open error, %s\n", strerror(errno));
		bExit = TRUE;
	}
	if(lseek(fd, 0, 0) < 0)		
		;
  //>>>JOYCE2009-11-18 add
	if(dwFlag == CONST_INST_FLAG)
	{
		if(GetDBValue("OPER_HMI_NULL_NULL_CRC").lValue == CountOperationCRC())
		{	
			if(bExit != TRUE) //JOYCE2009-11-18 解決停車方式g_wInstWay為1有時關電不保存的問題
			{
				OPERATION 	OperationX;
				memset(&OperationX, 0, sizeof(OperationX));
				if(read(fd, (char *)&OperationX, sizeof(OperationX)) != sizeof(OperationX))
					;//printf("could not read install, %s\n", strerror(errno));
				if (OperationX.cDaysTimeoutPeriod & 0x1000)	g_wInstWay=1;
			}
			
			SaveOperation();
		}
		else
		{
			DealOperation(&operation, 0);
			WriteOperationToSRAM(&operation);
		}
	}
	else
	{
		DealOperation(&operation, 0);
		if(operation.dwFlag != CONST_INST_FLAG)
		{
			SetDBValue("OPER_HMI_NULL_NULL_FLAG", CONST_INST_FLAG);
			SaveOperation();
		}
		else
			WriteOperationToSRAM(&operation);
	}
}

/*---------------------------------------------------------------------------+
|  Function :  WORD		CountOperationCRC()				    		 		 |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  CountOperationCRC()                               			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD		CtmControl_Function::_CountOperationCRC()
{
	//LEO20070404
	OPERATION 	operation;
	WORD		wCRC;
	memset(&operation, 0, sizeof(operation));
	ReadOperationFromSRAM(&operation);
	wCRC = CheckSum((WORD *)&operation, sizeof(OPERATION)-4);
	
	return	wCRC;
}

/*---------------------------------------------------------------------------+
|  Function :  GetBlockValueState(int nBlockIndex, tmStorageType SrcStorageType, tmStorageType DstStorageType)	 |
|  Task     :  將DA曲線值從  SrcStorageType讀取，存到 DstStorageType中去 有兩個條件，DstStorageType要為0， 
|				SrcStorageType不為0											|
+----------------------------------------------------------------------------+
|  Call     :  GetBlockValueState()                               			 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int			CtmControl_Function::_GetBlockValueState(int nBlockID, tmStorageType StorageType)
{
	int nSize = 0;
	WORD	wOffset = 0, wElement, wDBIndex;
	int		i;
	BOOL	bSave = FALSE;
	BOOL 	bResult = FALSE;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
    nSize = g_pBlock->GetBlockSize(nBlockID);
    
    if (nSize > 0)
    {
        BYTE*   pTemp = NULL;
        pTemp = new BYTE[nSize];
        bResult = CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATABLE_FILE, 1, 
                                                                      pTemp, StorageType)  == nSize;
        //if (bResult) bResult = g_pBlock->GetBlock(pTemp, m_nBlockIndex) == nSize;
		if (!bResult) g_pBlock->SetBlock(pTemp, nBlockID);
		if ((pBlockData	= g_pBlock->GetBlockData(nBlockID)) == NULL)	return 0;
		if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return 0;
		for (i=0; i<wElement; i++)
		{
			
			wDBIndex = pBlockData->pwDBIndex[i];
			dbValue = g_pDatabase->Read(wDBIndex);
			
			if (dbValue.DataType.wType != TYPE_STRING)
			{
				memcpy(dbValue.acData, (pTemp + wOffset), dbValue.DataType.wLength);
			}
			else
				strncpy(dbValue.pcData, (char *)(pTemp + wOffset), dbValue.DataType.wLength);
			wOffset += dbValue.DataType.wLength;
			
			//printf("wDBIndex=%d dbValue.wData=%d wElement=%d\n", wDBIndex, dbValue.wData, wElement);
			if(dbValue.wData != 0)
			{
				bSave = TRUE;
				break;
			}
		}
        
        delete []pTemp;
    }
	return bSave;
}

WORD		CtmControl_Function::_VER2HEX()	//Sunny<20080630>通過版本資料值計算停機功能需要用到的客戶代碼值
{
	char    szMMIVersion[10];
    GetDBString("VERS_HMI_NULL_NULL_PROGRAMMMIVERS", szMMIVersion, 6);
    WORD	wData;
   
    wData = (szMMIVersion[1]-'A'+10)*100 +(szMMIVersion[2]-'A'+10);
    wData =(wData/1000) *0x1000 +((wData%1000)/100) *0x100 + ((wData%100)/10) *0x10+(wData%10);
    
    return	wData;
}

void		CtmControl_Function::SaveCurrentRTCTime(OPERATION* pOperation)
{
	tmDATE	tmdate;
	tmTIME	tmtime;
	GetDate(&tmdate);
	GetTime(&tmtime);
	if(tmdate.year <= 2007)
	{
		SendMsg(MSG_USER_SH_PROMPTSTOPTIMEERROR, 0, 0, NULL);
	}
	else
	{
		memcpy(&pOperation->CurrentData, &tmdate, sizeof(tmdate));
		memcpy(&pOperation->CurrentTime, &tmtime, sizeof(tmtime));
	}
}

#ifndef	D_BEOBJECT_CTMCONTROL_FUNCTION
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
