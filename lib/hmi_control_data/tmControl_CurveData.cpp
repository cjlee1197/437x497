/*===========================================================================+
|  Class    : CtmControl_CurveData	                                         |
|  Task     : CurveData Service Function	    	                     	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 12/22/2008	                                                 |
|  Revision : 12/22/2008  				                                     |
+===========================================================================*/
#include	"tmControl_CurveData.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"
#include	"../../iotable.h"

IMPLEMENT_DYNCREATE(CtmControl_CurveData, CtmControl)

int 				CtmControl_CurveData::m_LastCurvePointIndex[MAX_CURVE_NUM_NEW];
tmOilCurveFileData	CtmControl_CurveData::m_aryLastCurvePoint[MAX_CURVE_NUM_NEW][MAX_CURVE_TEMPNUM];

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
//此结构体来用转换内部存储index， 如果排序超过35个，则要修改lib Contrl中的MAX_CURVE_NUM_NEW才可以
int		u_CurveNubCovert[][2] = 
{
	{CURVE_OIL_DATA_INJECT_REALPRESS, 		0},
	{CURVE_OIL_DATA_INJECT_REALSPEED, 		1},
	{CURVE_OIL_DATA_INJECT_REALPOS,			2},
	{CURVE_OIL_DATA_HOLD_REALPRESS,			3},
	{CURVE_OIL_DATA_HOLD_REALSPEED,			4},
	{CURVE_OIL_DATA_CHARRG_REALPRESS,		5},
	{CURVE_OIL_DATA_INJECT_SETPRESS,		6},
	{CURVE_OIL_DATA_INJECT_SETSPEED,		7},
	{CURVE_OIL_DATA_INJECT_SETPOS,			8},
	{CURVE_OIL_DATA_HOLD_SETPRESS,			9},
	{CURVE_OIL_DATA_HOLD_SETSPEED,			10},
	{CURVE_OIL_DATA_CHARGE_SETPRESS,		11},
	{CURVE_OIL_DATA_CHARGE_SETBACKPRES,		12},
	{CURVE_OIL_DATA_CHARGE_REALBACKPRES,	13},
	{CURVE_OIL_DATA_CHARGE_SETRPM,			14},
	{CURVE_OIL_DATA_CHARGE_REALRPM,			15},
	{CURVE_OIL_DATA_INJECT_DXPOS,			16},
	{CURVE_OIL_DATA_HOLD_DXPOS,				17},
	
	{CURVE_OIL_DATA_INJECTB_REALPRESS,		18},
	{CURVE_OIL_DATA_INJECTB_REALSPEED,		19},
	{CURVE_OIL_DATA_INJECTB_REALPOS,		20},
	{CURVE_OIL_DATA_HOLDB_REALPRESS,		21},
	{CURVE_OIL_DATA_HOLDB_REALSPEED,		22},
	{CURVE_OIL_DATA_CHARRGB_REALPRESS,		23},
	{CURVE_OIL_DATA_INJECTB_SETPRESS,		24},
	{CURVE_OIL_DATA_INJECTB_SETSPEED,		25},
	{CURVE_OIL_DATA_INJECTB_SETPOS,			26},
	{CURVE_OIL_DATA_HOLDB_SETPRESS,			27},
	{CURVE_OIL_DATA_HOLDB_SETSPEED,			28},
	{CURVE_OIL_DATA_CHARGEB_SETPRESS,		29},
	{CURVE_OIL_DATA_CHARGEB_SETBACKPRES,	30},
	{CURVE_OIL_DATA_CHARGEB_REALBACKPRES,	31},
	{CURVE_OIL_DATA_CHARGEB_SETRPM,			32},
	{CURVE_OIL_DATA_CHARGEB_REALRPM,		33},
	{CURVE_OIL_DATA_INJECTB_DXPOS,			34},
	{CURVE_OIL_DATA_HOLDB_DXPOS,			35},
	
	{CURVE_OIL_DATA_LASTTIME_INJECTB_REALPRESS,		-1},
	{CURVE_OIL_DATA_LASTTIME_INJECTB_REALSPEED,		-1},
	{CURVE_OIL_DATA_LASTTIME_INJECTB_REALPOS,		-1},
	{CURVE_OIL_DATA_LASTTIME_HOLDB_REALPRESS,		-1},
	{CURVE_OIL_DATA_LASTTIME_HOLDB_REALSPEED,		-1},
	{CURVE_OIL_DATA_LASTTIME_CHARRGB_REALPRESS,		-1},
};
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
|  Function : CtmControl_CurveData::CtmControl_CurveData()				         |
|  Task     :  CtmControl_CurveData篶硑ㄧ计                                    |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_CurveData::CtmControl_CurveData(): CtmControl()
{
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::~CtmControl_CurveData()				             |
|  Task     :  CtmControl_CurveData猂篶ㄧ计                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_CurveData::~CtmControl_CurveData()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_CurveData::CreateSelf()
{
	InitControl();
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::FreeSelf()				             	     |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_CurveData::FreeSelf()
{
	for(int i = 0; i < MAX_CURVE_NUM_NEW; i++)
	{
		if( m_aryPoints[i] !=NULL)
		{
			delete [] m_aryPoints[i];
			m_aryPoints[i] = NULL;
		}
		for(int j = 0; j < MAX_CURVE_TEMPNUM; j++)
		{
			if(m_aryLastCurvePoint[i][j].Points != NULL)
			{
				delete [] m_aryLastCurvePoint[i][j].Points;
				m_aryLastCurvePoint[i][j].Points = NULL;
			}
		}
	}
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::AddControl(void *pParas[])  	     	 |
|  Task     :  Save Error Record to sram and show promp	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   WORD   pParas[0]        - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_CurveData::AddControl(void *pParas[])
{
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::ModifyControl(void *pParas[])  	     		 |
|  Task     :  Modify Error Message 					                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   WORD   pParas[0]        - 		                  			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_CurveData::ModifyControl(void *pParas[])
{
	return FALSE;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::FindControl(void *pParas[], void *pFindPara) |
|  Task     :  Search Error Message 					                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   void *   pFindPara      - 			               			 |
|               void *pParas[]          - 纗牡厨癟                 |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_CurveData::FindControl(void *pParas[], void *pFindPara)
{
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::DeleteControl(void *pDeletePara)             |
|  Task     :  Clear All Error Message 					                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   void *pDeletePara       - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_CurveData::DeleteControl(void *pDeletePara)
{
	
	return TRUE;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::FixControl(void *pFixPara)		             |
|  Task     :  Fix Error Message 					                     	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   void *pFixPara          -                        			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_CurveData::FixControl(void *pFixPara)
{
	                                                                                                              
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveData::InitControl()					             |
|  Task     :  Init Error Message 					                     	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_CurveData::InitControl()
{
	int	nCtype = 0;
	memset(m_aryState, 0, sizeof(m_aryState));
	memset(m_aryIndex, 0, sizeof(m_aryIndex));
	memset(m_aryComplete, 0, sizeof(m_aryComplete));
	memset(m_LastCurvePointIndex,0,sizeof(m_LastCurvePointIndex));
	//memset(&m_oilData, 0, sizeof(m_oilData));
	//memset(m_aryPoints, 0, sizeof(m_aryPoints));
	//memset(m_aryLastCurvePoint,0,sizeof(m_aryLastCurvePoint));
	//默认曲线都设置600个点
	for(int i = 0; i < MAX_CURVE_NUM_NEW; i++)
	{
		m_aryPoints_MaxNum[i] = MAX_POINT_NUM;
	}
	//针对射出和保压特别定制2000个点
	//Inject A
	#ifdef D_5528
		nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECT_SETPRESS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECT_SETSPEED);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLD_SETPRESS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLD_SETSPEED);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	#endif	
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECT_REALPRESS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECT_REALSPEED);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECT_REALPOS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	//Hold A
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLD_REALPRESS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLD_REALSPEED);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECT_DXPOS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLD_DXPOS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	//Inject B
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECTB_REALPRESS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECTB_REALSPEED);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	nCtype = CurveNubCovert(CURVE_OIL_DATA_INJECTB_REALPOS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
	
	//Hold B
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLDB_REALPRESS);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	nCtype = CurveNubCovert(CURVE_OIL_DATA_HOLDB_REALSPEED);
	if(nCtype >=0)
		m_aryPoints_MaxNum[nCtype] = MAX_POINT_NUM_2000;
		
	//new Curve Data buffer
	for(int i = 0; i < MAX_CURVE_NUM_NEW; i++)
	{
		m_aryPoints[i] = NULL;
		//printf("m_aryPoints_MaxNum[i]=%d \n", m_aryPoints_MaxNum[i]);
		m_aryPoints[i] = (tmCurveOilPoint*)new  tmCurveOilPoint[m_aryPoints_MaxNum[i]];
		if(m_aryPoints[i] == NULL)
			printf("new memery error \n");
		for(int j = 0; j < MAX_CURVE_TEMPNUM; j++)
		{
			m_aryLastCurvePoint[i][j].nID     = 0;
			m_aryLastCurvePoint[i][j].nCount = 0;
			m_aryLastCurvePoint[i][j].Points = (tmCurveOilPoint*)new tmCurveOilPoint[m_aryPoints_MaxNum[i]];
		}
	}
}

/*--------------------------------------------------------------------------+
|  Function :  SetData(int nType, double* dX, double* dY, int nNum)			|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::SetData(int nType, double* dX, double* dY, int nNum)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	m_aryState[nCType] = 1;
	
	if(m_aryComplete[nCType] == 0)
		m_aryIndex[nCType] = 0;
	m_aryComplete[nCType] = 1;
	for (int i = 0; i < nNum; i++)
	{
		m_aryPoints[nCType][m_aryIndex[nCType]].dX = *(dX + i);
		m_aryPoints[nCType][m_aryIndex[nCType]].dY = *(dY + i);	
//		if(CURVE_OIL_DATA_HOLD_REALSPEED==nType)
//		{
//			printf("nCount=%d,nType=%d,dX=%lf,dY=%lf\n",m_aryIndex[nCType],nType,m_aryPoints[nCType][m_aryIndex[nCType]].dX,m_aryPoints[nCType][m_aryIndex[nCType]].dY);	
//		}	
		m_aryIndex[nCType]++;
		if (m_aryIndex[nCType] >= m_aryPoints_MaxNum[nCType]) m_aryIndex[nCType]--;
	}	
	return m_aryIndex[nCType];
}

/*--------------------------------------------------------------------------+
|  Function :  SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)			|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)
{
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  SetData(int	nType, void* pInputData, int nNum)				|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::SetData(int	nType, void* pInputData, int nNum)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
		
	m_aryState[nCType] = 1;
	
	if(m_aryComplete[nCType] == 0)
		m_aryIndex[nCType] = 0;
	m_aryComplete[nCType] = 1;
	for (int i = 0; i < nNum; i++)
	{
		if(nType < MAX_CURVE_NUM_NEW)
		{
			m_aryPoints[nCType][m_aryIndex[nCType]].dX = ((tmCurveOilPoint*)pInputData + i)->dX;
			m_aryPoints[nCType][m_aryIndex[nCType]].dY = ((tmCurveOilPoint*)pInputData + i)->dY;
			m_aryIndex[nCType]++;
			if (m_aryIndex[nCType] >= m_aryPoints_MaxNum[nCType]) m_aryIndex[nCType]--;
		}
	}	
	return m_aryIndex[nCType];
}

/*--------------------------------------------------------------------------+
|  Function :  GetMaxPosData()												|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetMaxPosData()
{
	long long opposite = 0,absolut = 0;
	WORD	temp1,temp2;
	int 	result;
	opposite = GetDBValue((char *)"CONF_CODE_READ_CHL1_MAXPOSITION").lValue;//20070815
	temp1 = (WORD)GetDBValue((char *)"CONF_AD1_WRITE_CHL1_PHYSICALLENGTH").lValue;
	temp2 = (WORD)GetDBValue((char *)"CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
	absolut = (long long )(HEX2BCD(temp1)*10+(temp2/(0x0001))%0x0010);
	if(opposite < absolut)
		result = opposite; 
	else
		result = absolut; 
	return result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetInjectPress(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetInjectPress(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW_INJECT_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		//Sunny<20080320> modify<<<<<甮Ρ絬X禸程莱赣琌纗沧ゎ竚+甮癶竚
		//EjetMaxPos = GetMaxPosData();// / Power(10, GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision);//GetDBValue(sz).lValue/Power(10,GetDBValue(CONF_CODE_READ_CHL1_MAXPOSITION).wPrecision);
		int chargesegment = GetDBValue((char *)"MCONF_SCREW_RECOVERY_NULL_RANK").lValue;
		sprintf(sz,	"%s%d%s", "MSET_SCRW_RCV_STEP",chargesegment,"_POSITION");
		EjetMaxPos = (GetDBValue(sz).lValue+GetDBValue((char *)"MSET_SCRW_DCMP_NULL_POSITIONOFFSET").lValue);
		//Sunny<20080320> modify>>>>>
		
		((tmCurveOilPoint*)pOutData)->dX = EjetMaxPos = 30000; //Andy 20091026 for New Curve;
		//((tmCurveOilPoint*)pOutData)->dX = 130;
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSET_SCRW_INJ_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSET_SCRW_INJ_STEP", j, "_PRESSURE");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetInjectSpeed(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetInjectSpeed(void* pOutData)	
{
	
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW_INJECT_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		//Sunny<20080320> modify<<<<<甮Ρ絬X禸程莱赣琌纗沧ゎ竚+甮癶竚
		//sprintf(sz,	"%s", "CONF_CODE_READ_CHL1_MAXPOSITION");			//甮程竚
		//EjetMaxPos = GetDBValue(sz).lValue;// / Power(10,GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision); 	
		int chargesegment = GetDBValue((char *)"MCONF_SCREW_RECOVERY_NULL_RANK").lValue;
		sprintf(sz,	"%s%d%s", "MSET_SCRW_RCV_STEP",chargesegment,"_POSITION");
		EjetMaxPos = (GetDBValue(sz).lValue+GetDBValue((char *)"MSET_SCRW_DCMP_NULL_POSITIONOFFSET").lValue);
		//Sunny<20080320> modify>>>>>
		  
		((tmCurveOilPoint*)pOutData)->dX = EjetMaxPos = 30000; //Andy 20091026 for New Curve;                      
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSET_SCRW_INJ_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;
			
			sprintf(sz, "%s%d%s", "MSET_SCRW_INJ_STEP", j, "_FLOW");
			lY = GetDBValue(sz).lValue;
			
			//	fans add
            if(!g_pExTable->GetTableValue(0,(char *)"D_INJSPDUNIT"))
			lY = lY * GetDBValue((char *)"SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue / (100*Power(10, GetDBValue((char *)"MSET_SCRW_INJ_STEP1_FLOW").wPrecision));
			//lY = lY * CtmConfig::GetInstance()->GetMaxInjectSpeed() / 100;
			
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			//((tmCurveOilPoint*)pOutData + i)->dX = lY;	
			((tmCurveOilPoint*)pOutData + i + 1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetInjectPos(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetInjectPos(void* pOutData)	
{
	return 0;	
}

/*--------------------------------------------------------------------------+
|  Function :  GetHoldPress(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetHoldPress(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW_HOLDPRESSURE_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	Result *= 2;
	if(Result > 0)
	{
		((tmCurveOilPoint*)pOutData)->dX = 0;
		for(int i = 0, j = 1; i <= Result; i += 2 ,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSET_SCRW_HLDP_STEP",j,"_TIME");
			lX = GetDBValue(sz).lValue;// / Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSET_SCRW_HLDP_STEP",j,"_PRESSURE");
			lY = GetDBValue(sz).lValue;//Power(10,GetDBValue(sz).wPrecision);
			
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i + 1)->dX = ((tmCurveOilPoint*)pOutData + i)->dX + lX;
			//printf("press Xpiont %d is %f, Ypiont %d is %f\n",
			//		i-1,((tmCurveOilPoint*)pOutData + i-1)->dX,i-1,((tmCurveOilPoint*)pOutData + i-1)->dY = lY);
			//printf("press Xpiont %d is %f, Ypiont %d is %f\n",
			//		i,((tmCurveOilPoint*)pOutData + i)->dX,i,((tmCurveOilPoint*)pOutData + i)->dY = lY);
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetHoldSpeed(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetHoldSpeed(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW_HOLDPRESSURE_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	Result *= 2;
	
	if(Result > 0)
	{
		((tmCurveOilPoint*)pOutData)->dX = 0;
		for(int i = 0, j = 1; i <= Result;i += 2, j++ )
		{
			sprintf(sz,	"%s%d%s", "MSET_SCRW_HLDP_STEP",j,"_TIME");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSET_SCRW_HLDP_STEP",j,"_FLOW");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			//	fans add
            if(!g_pExTable->GetTableValue(0,(char *)"D_INJSPDUNIT"))
			lY = lY * GetDBValue((char *)"SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue / (100*Power(10, GetDBValue((char *)"MSET_SCRW_HLDP_STEP1_FLOW").wPrecision));
			
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i + 1)->dX = ((tmCurveOilPoint*)pOutData + i)->dX +lX;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetChargeBackPres(void* pOutData)							|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetChargeBackPres(void* pOutData)	//Sunny<20070815>
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW_RECOVERY_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		sprintf(sz,	"%s", "CONF_CODE_READ_CHL1_MAXPOSITION");			//纗程竚
		EjetMaxPos = GetDBValue(sz).lValue;// / Power(10,GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision); 
		((tmCurveOilPoint*)pOutData)->dX = 0;//EjetMaxPos;                       
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSET_SCRW_RCV_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSET_SCRW_RCV_STEP", j, "_BACKPRESSURE");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetChargeRPM(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetChargeRPM(void* pOutData)	//Sunny<20070815>
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW_RECOVERY_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		sprintf(sz,	"%s", "CONF_CODE_READ_CHL1_MAXPOSITION");			//甮程竚
		EjetMaxPos = GetDBValue(sz).lValue;// / Power(10,GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision); 
		//printf("GetInjext speed max position is %lld. precision=%d\n", GetDBValue(sz).lValue, 
				//GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision);  
		((tmCurveOilPoint*)pOutData)->dX = 0;//EjetMaxPos;                       
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSET_SCRW_RCV_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSET_SCRW_RCV_STEP", j, "_FLOW");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			//	fans add
			lY = lY * GetDBValue((char *)"SYSX_INJECT_RECOVERY_NULL_MAXFLOW").lValue / (100*Power(10, GetDBValue((char *)"MSET_SCRW_RCV_STEP1_FLOW").wPrecision));
			//lY = lY * CtmConfig::GetInstance()->GetMaxChargeSpeed() / 100;
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetChargePress(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetChargePress(void* pOutData)	
{
	return 0;
}


/*--------------------------------------------------------------------------+
|  Function :  GetMaxPosBData()												|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetMaxPosBData()
{
	long long opposite = 0,absolut = 0;
	WORD	temp1 =0, temp2 =0;
	int 	result =0;
	opposite = GetDBValue((char *)"CONF_CODE_READ_CHL8_MAXPOSITION").lValue/**10*/;//20070815
	temp1 = (WORD)GetDBValue((char *)"CONF_AD1_WRITE_CHL8_PHYSICALLENGTH").lValue;
	temp2 = (WORD)GetDBValue((char *)"CONF_AD1_WRITE_CHL2_LENGTHP1MM").lValue;
	absolut = (long long )(HEX2BCD(temp1)*10+((temp2/(0x1000))>>12));
	if(opposite < absolut)
		result = opposite; 
	else
		result = absolut; 
	return result;
}


/*--------------------------------------------------------------------------+
|  Function :  GetInjectPress(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetInjectBPress(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW2_INJECT_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		//Sunny<20080320> modify<<<<<甮Ρ絬X禸程莱赣琌纗沧ゎ竚+甮癶竚
		//EjetMaxPos = GetMaxPosData();// / Power(10, GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision);//GetDBValue(sz).lValue/Power(10,GetDBValue(CONF_CODE_READ_CHL1_MAXPOSITION).wPrecision);
		int chargesegment = GetDBValue((char *)"MCONF_SCREW2_RECOVERY_NULL_RANK").lValue;
		sprintf(sz,	"%s%d%s", "MSETB_SCRW2_RCV_STEP",chargesegment,"_POSITION");
		EjetMaxPos = (GetDBValue(sz).lValue+GetDBValue((char *)"MSETB_SCRW2_DCMP_NULL_POSITIONOFFSET").lValue);
		//Sunny<20080320> modify>>>>>
		((tmCurveOilPoint*)pOutData)->dX = EjetMaxPos = 30000; //Andy 20091026 for New Curve;;
		//((tmCurveOilPoint*)pOutData)->dX = 130;
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSETB_SCRW2_INJ_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSETB_SCRW2_INJ_STEP", j, "_PRESSURE");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetInjectSpeed(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetInjectBSpeed(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW2_INJECT_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		//Sunny<20080320> modify<<<<<甮Ρ絬X禸程莱赣琌纗沧ゎ竚+甮癶竚
		int chargesegment = GetDBValue((char *)"MCONF_SCREW2_RECOVERY_NULL_RANK").lValue;
		sprintf(sz,	"%s%d%s", "MSETB_SCRW2_RCV_STEP",chargesegment,"_POSITION");
		EjetMaxPos = (GetDBValue(sz).lValue+GetDBValue((char *)"MSETB_SCRW2_DCMP_NULL_POSITIONOFFSET").lValue);
		//Sunny<20080320> modify>>>>>
		((tmCurveOilPoint*)pOutData)->dX = EjetMaxPos = 30000; //Andy 20091026 for New Curve;;                       
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSETB_SCRW2_INJ_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSETB_SCRW2_INJ_STEP", j, "_FLOW");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			//	fans add
			lY = lY * GetDBValue((char *)"SYSX_INJECT2_INJECT_NULL_MAXFLOW").lValue / (100*Power(10, GetDBValue((char *)"MSETB_SCRW2_INJ_STEP1_FLOW").wPrecision));
			//lY = lY * CtmConfig::GetInstance()->GetMaxInjectSpeed() / 100;
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetInjectPos(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetInjectBPos(void* pOutData)	
{
	return 0;	
}

/*--------------------------------------------------------------------------+
|  Function :  GetHoldPress(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetHoldBPress(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW2_HOLDPRESSURE_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	Result *= 2;
	if(Result > 0)
	{
		((tmCurveOilPoint*)pOutData)->dX = 0;
		for( int i = 0, j = 1; i <= Result; i += 2 ,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSETB_SCRW2_HLDP_STEP",j,"_TIME");
			lX = GetDBValue(sz).lValue;	// Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSETB_SCRW2_HLDP_STEP",j,"_PRESSURE");
			lY = GetDBValue(sz).lValue;	// Power(10,GetDBValue(sz).wPrecision);
			
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i + 1)->dX = ((tmCurveOilPoint*)pOutData + i)->dX + lX;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetHoldSpeed(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetHoldBSpeed(void* pOutData)	
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW2_HOLDPRESSURE_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	Result *= 2;
	
	if(Result > 0)
	{
		((tmCurveOilPoint*)pOutData)->dX = 0;
		for(int i = 0, j = 1; i <= Result;i += 2, j++ )
		{
			sprintf(sz,	"%s%d%s", "MSETB_SCRW2_HLDP_STEP",j,"_TIME");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSETB_SCRW2_HLDP_STEP",j,"_FLOW");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			//	fans add
			lY = lY * GetDBValue((char *)"SYSX_INJECT2_INJECT_NULL_MAXFLOW").lValue / (100*Power(10, GetDBValue((char *)"MSETB_SCRW2_HLDP_STEP1_FLOW").wPrecision));
			
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i + 1)->dX = ((tmCurveOilPoint*)pOutData + i)->dX +lX;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetChargeBackPres(void* pOutData)							|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetChargeBBackPres(void* pOutData)	//Sunny<20070815>
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW2_RECOVERY_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		sprintf(sz,	"%s", "CONF_CODE_READ_CHL8_MAXPOSITION");			//纗程竚
		EjetMaxPos = GetDBValue(sz).lValue;		//Power(10,GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision); 
		((tmCurveOilPoint*)pOutData)->dX = 0;	//EjetMaxPos;                       
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSETB_SCRW2_RCV_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSETB_SCRW2_RCV_STEP", j, "_BACKPRESSURE");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetChargeRPM(void* pOutData)									|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int 	CtmControl_CurveData::GetChargeBRPM(void* pOutData)	//Sunny<20070815>
{
	int	Result = 0;
	Result = GetDBValue((char *)"MCONF_SCREW2_RECOVERY_NULL_RANK").lValue;
	char	sz[128];
	double	lX = 0, lY = 0;
	int 	EjetMaxPos = 0;
	Result *= 2;
	if (Result > 0)
	{
		sprintf(sz,	"%s", "CONF_CODE_READ_CHL8_MAXPOSITION");			//甮程竚
		EjetMaxPos = GetDBValue(sz).lValue;		//Power(10,GetDBValue("MSET_SCRW_INJ_STEP1_POSITION").wPrecision); 
		((tmCurveOilPoint*)pOutData)->dX = 0;	//EjetMaxPos;                       
		for (int i = 0, j = 1; i < Result; i += 2,j++ )
		{
			sprintf(sz,	"%s%d%s", "MSETB_SCRW2_RCV_STEP", j, "_POSITION");
			lX = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			
			sprintf(sz, "%s%d%s", "MSETB_SCRW2_RCV_STEP", j, "_FLOW");
			lY = GetDBValue(sz).lValue;///Power(10,GetDBValue(sz).wPrecision);
			//	fans add
			lY = lY * GetDBValue((char *)"SYSX_INJECT2_RECOVERY_NULL_MAXFLOW").lValue / (100*Power(10, GetDBValue((char *)"MSETB_SCRW2_RCV_STEP1_FLOW").wPrecision));
			
			if(i != 0)
				((tmCurveOilPoint*)pOutData + i)->dX = ((tmCurveOilPoint*)pOutData + i-1)->dX;
			((tmCurveOilPoint*)pOutData + i+1)->dX = lX;
			((tmCurveOilPoint*)pOutData + i)->dY = lY;
			((tmCurveOilPoint*)pOutData + i + 1)->dY = lY;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetChargePress(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetChargeBPress(void* pOutData)	
{
	return 0;
}
/*--------------------------------------------------------------------------+
|  Function :  GetData(int nType, void* pOutData)							|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetData(int nType, void* pOutData,int TempDataIndex)
{
	int Result = 0;
//	tmOilCurveFileData LastTimeData;
	int	nCType = CurveNubCovert(nType);
	
	//if(nCType <0 )
	//	return 0;
		
	switch (nType)
	{
		#ifdef	D_5528
		case CURVE_OIL_DATA_INJECT_SETPRESS:
		case CURVE_OIL_DATA_INJECT_SETSPEED:
		case CURVE_OIL_DATA_HOLD_SETPRESS:
		case CURVE_OIL_DATA_HOLD_SETSPEED:    
		#endif
		
		case CURVE_OIL_DATA_INJECT_REALPRESS:           	//	inject pressure  
		case CURVE_OIL_DATA_INJECT_REALSPEED:               //	inject speed     
		case CURVE_OIL_DATA_INJECT_REALPOS:                 //	inject postion   
		case CURVE_OIL_DATA_HOLD_REALPRESS:					//	hold press 
		case CURVE_OIL_DATA_HOLD_REALSPEED:                 //	hold speed 
		case CURVE_OIL_DATA_CHARRG_REALPRESS:				//	charge pressure
		case CURVE_OIL_DATA_CHARGE_REALBACKPRES:			//  charge backpres real  //Sunny<20070815>
		case CURVE_OIL_DATA_CHARGE_REALRPM:				 	//  charge RPM real			
		
		case CURVE_OIL_DATA_INJECTB_REALPRESS:           	//	injectB pressure  
		case CURVE_OIL_DATA_INJECTB_REALSPEED:              //	injectB speed     
		case CURVE_OIL_DATA_INJECTB_REALPOS:                //	injectB postion   
		case CURVE_OIL_DATA_HOLDB_REALPRESS:				//	holdB press 
		case CURVE_OIL_DATA_HOLDB_REALSPEED:                //	holdB speed 
		case CURVE_OIL_DATA_CHARRGB_REALPRESS:				//	chargeB pressure
		case CURVE_OIL_DATA_CHARGEB_REALBACKPRES:			//  chargeB backpres real  //Sunny<20070815>
		case CURVE_OIL_DATA_CHARGEB_REALRPM:				//  chargeB RPM real			
		case CURVE_OIL_DATA_INJECT_DXPOS:					//  inject pos real  //James ad 2009/9/18 01:48と
		case CURVE_OIL_DATA_HOLD_DXPOS:						//  hold pos real	//czz add 2009/10/27 07:10と
		case CURVE_OIL_DATA_INJECTB_DXPOS:					//  inject pos real  //James ad 2009/9/18 01:48と
		case CURVE_OIL_DATA_HOLDB_DXPOS:						//  hold pos real	//czz add 2009/10/27 07:10と
			if(nCType >=0)
			{
			//printf("nCType=%d m_aryIndex[nCType]=%d \n", nCType, m_aryIndex[nCType]);
				Result = m_aryIndex[nCType];
				if(Result>0)
				{
					memcpy(pOutData, m_aryPoints[nCType], m_aryIndex[nCType] * sizeof(tmCurveOilPoint));
				}
			}
			break;
		#ifndef	D_5528
		case CURVE_OIL_DATA_INJECT_SETPRESS:				//	inject pressure setting
			Result = GetInjectPress(pOutData);
			break;
		case CURVE_OIL_DATA_INJECT_SETSPEED:				//	inject	speed settting 
			Result = GetInjectSpeed(pOutData);
			break;
		case CURVE_OIL_DATA_HOLD_SETPRESS:				//	hold press	setting    
			Result = GetHoldPress(pOutData);
			break;
		case CURVE_OIL_DATA_HOLD_SETSPEED	:				//  hold speed  setting    
			Result = GetHoldSpeed(pOutData);
			break;
		#endif		
		case CURVE_OIL_DATA_INJECTB_SETPRESS:			//	injectB pressure setting
			Result = GetInjectBPress(pOutData);
			break;
		case CURVE_OIL_DATA_INJECTB_SETSPEED	:			//	injectB	speed settting 
			Result = GetInjectBSpeed(pOutData);
			break;
		case CURVE_OIL_DATA_INJECT_SETPOS	:				//	inject pos seeting     
			Result = GetInjectPos(pOutData);
			break;
		case CURVE_OIL_DATA_INJECTB_SETPOS	:				//	injectB pos seeting     
			Result = GetInjectBPos(pOutData);
			break;
		case CURVE_OIL_DATA_HOLDB_SETPRESS	:				//	holdB press	setting    
			Result = GetHoldBPress(pOutData);
			break;
		case CURVE_OIL_DATA_HOLDB_SETSPEED	:				//  holdB speed  setting    
			Result = GetHoldBSpeed(pOutData);
			break;
		case CURVE_OIL_DATA_CHARGE_SETPRESS	:				//	charge press setting   			
			Result = GetChargePress(pOutData);
			break;
		case CURVE_OIL_DATA_CHARGEB_SETPRESS:			//	chargeB press setting   			
			Result = GetChargeBPress(pOutData);
			break;
		case CURVE_OIL_DATA_CHARGE_SETBACKPRES:				//  charge backpres setting//Sunny<20070815>
			Result = GetChargeBackPres(pOutData);
			break;
		case CURVE_OIL_DATA_CHARGEB_SETBACKPRES:			//  chargeB backpres setting//Sunny<20070815>
			Result = GetChargeBBackPres(pOutData);
			break;
		case CURVE_OIL_DATA_CHARGE_SETRPM:				    //  charge RPM setting
			Result = GetChargeRPM(pOutData);
			break;
		case CURVE_OIL_DATA_CHARGEB_SETRPM:				    //  chargeB RPM setting
			Result = GetChargeBRPM(pOutData);
			break;
		case CURVE_OIL_DATA_LASTTIME_INJECT_REALPRESS:		//get last time data yangpeng 2007/4/16 04:36と
		case CURVE_OIL_DATA_LASTTIME_INJECT_REALSPEED:
		case CURVE_OIL_DATA_LASTTIME_INJECT_REALPOS:	
		case CURVE_OIL_DATA_LASTTIME_HOLD_REALPRESS:	
		case CURVE_OIL_DATA_LASTTIME_HOLD_REALSPEED:
		case CURVE_OIL_DATA_LASTTIME_CHARRG_REALPRESS:
			
		case CURVE_OIL_DATA_LASTTIME_INJECTB_REALPRESS:		//get lastB time data yangpeng 2007/4/16 04:36と
		case CURVE_OIL_DATA_LASTTIME_INJECTB_REALSPEED:
		case CURVE_OIL_DATA_LASTTIME_INJECTB_REALPOS:	
		case CURVE_OIL_DATA_LASTTIME_HOLDB_REALPRESS:	
		case CURVE_OIL_DATA_LASTTIME_HOLDB_REALSPEED:
		case CURVE_OIL_DATA_LASTTIME_CHARRGB_REALPRESS:
			//Result = m_pCurveFile->GetInstance()->ReadRecord(ID_CURVEDATA_SRAMFILE, (nCType + 1)*MAX_CURVE_NUM, &LastTimeData);
			//if(LastTimeData.nID == nCType && Result != 0)
			//{	
			//	Result = LastTimeData.nCount;
			//	memcpy(pOutData, LastTimeData.Points,Result*sizeof(CURVEPOINT));
			//}
			break;
		case CURVE_OIL_DATA_INJECT_TEMPORARILY_REALPRESS:	//get last five data yangpeng 2007/4/16 04:36と
		case CURVE_OIL_DATA_INJECT_TEMPORARILY_REALSPEED:
		case CURVE_OIL_DATA_INJECT_TEMPORARILY_REALPOS:
		case CURVE_OIL_DATA_HOLD_TEMPORARILY_REALPRESS:
		case CURVE_OIL_DATA_HOLD_TEMPORARILY_REALSPEED:
		case CURVE_OIL_DATA_CHARRG_TEMPRARILY_REALPRESS:
		case CURVE_OIL_DATA_INJECT_TEMPORARILY_DXPOS:		//  inject  pos temporarily data
		case CURVE_OIL_DATA_HOLD_TEMPORARILY_DXPOS:	
		case CURVE_OIL_DATA_INJECTB_TEMPORARILY_REALPRESS:	//get lastB five data yangpeng 2007/4/16 04:36と
		case CURVE_OIL_DATA_INJECTB_TEMPORARILY_REALSPEED:
		case CURVE_OIL_DATA_INJECTB_TEMPORARILY_REALPOS:
		case CURVE_OIL_DATA_HOLDB_TEMPORARILY_REALPRESS:
		case CURVE_OIL_DATA_HOLDB_TEMPORARILY_REALSPEED:
		case CURVE_OIL_DATA_CHARRGB_TEMPRARILY_REALPRESS:
		
		    if(TempDataIndex < MAX_CURVE_TEMPNUM)
			{
				nCType = CurveNubCovert(nType - TEMPORARILY_TO_NEW);
				if(nCType >= 0)
				{
					Result = m_aryLastCurvePoint[nCType][TempDataIndex].nCount;
					if(Result>0)
					{
						memcpy(pOutData,m_aryLastCurvePoint[nCType][TempDataIndex].Points,
							Result * sizeof(CURVEPOINT));
					}
				}
			}
			else Result = 0;
			break;	
	}
	return Result;
	
}

/*--------------------------------------------------------------------------+
|  Function :  SetComplete(int nType)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmControl_CurveData::SetComplete(int nType)
{
	//yangpeng 2007/4/13 01:53と ,–贺Ρ絬兵纗,讽玡家㎝家Ρ絬
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
	
	int nTemp = m_aryIndex[nCType];
	m_aryComplete[nCType] = 0;
	
	//盢讽玡家计Ρ絬羬buffer
	if(m_LastCurvePointIndex[nCType] >= MAX_CURVE_TEMPNUM)
		m_LastCurvePointIndex[nCType] = 0; 
	m_aryLastCurvePoint[nCType][m_LastCurvePointIndex[nCType]].nID = nCType;
	m_aryLastCurvePoint[nCType][m_LastCurvePointIndex[nCType]].nCount = nTemp;
	bzero(m_aryLastCurvePoint[nCType][m_LastCurvePointIndex[nCType]].Points,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
	memcpy(m_aryLastCurvePoint[nCType][m_LastCurvePointIndex[nCType]].Points, m_aryPoints[nCType], 
											nTemp * sizeof(m_aryPoints[nCType][0]));
//	printf("nCType=%d count=%d m_LastCurvePointIndex[nCType]=%d,dX=%lf,dY=%lf\n", nCType, nTemp,m_LastCurvePointIndex[nCType],
//	m_aryPoints[nCType][nTemp-1].dX,m_aryPoints[nCType][nTemp-1].dY);
	m_LastCurvePointIndex[nCType]++;
	
	
	//m_pCurveFile->GetInstance()->WriteRecord(ID_CURVEDATA_SRAMFILE, nCType + 1, &m_oilData);
	
}

/*--------------------------------------------------------------------------+
|  Function :  SetState(int nType, int nState)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::SetState(int nType, int nState)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	m_aryState[nCType] = nState;
	return m_aryIndex[nCType];
}


/*--------------------------------------------------------------------------+
|  Function :  GetState(int nType)											|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveData::GetState(int nType)	
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	return m_aryState[nCType];
}										
/*--------------------------------------------------------------------------+
|  Function :   ClearBuffer()											    |	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void    CtmControl_CurveData::ClearBuffer(int nType)
{
	if(nType==-1)
	{
		for(int nID=CURVE_OIL_DATA_INJECT_REALPRESS; nID<=CURVE_OIL_DATA_HOLD_REALSPEED; nID++)
		{
			//printf("clear real ...\n");
			int	nCType = CurveNubCovert(nID);
			memset(m_aryPoints[nCType],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
 			m_aryIndex[nCType] = 0;
 		}
 		
 		for(int nID=CURVE_OIL_DATA_INJECT_SETPRESS; nID<=CURVE_OIL_DATA_HOLD_SETSPEED; nID++)
		{
			//printf("clear set ...\n");
			int	nCType = CurveNubCovert(nID);
			memset(m_aryPoints[nCType],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
 			m_aryIndex[nCType] = 0;
 		}
 		//printf("clear pos ...\n");
 		int	nCType = CurveNubCovert(CURVE_OIL_DATA_INJECT_DXPOS);
		memset(m_aryPoints[nCType],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
 		m_aryIndex[nCType] = 0;
		//add by J.Wong 2015/12/25 12:25:02
 		nCType = CurveNubCovert(CURVE_OIL_DATA_HOLD_DXPOS);
		memset(m_aryPoints[nCType],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
 		m_aryIndex[nCType] = 0;
		return ;
	}
	
	int	nCType = CurveNubCovert(nType);
	//if(nCType <0 )
	//	return ;
    if(nCType >= 0)
	{
		m_LastCurvePointIndex[nCType] = 0;
 		memset(m_aryPoints[nCType],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
 		m_aryIndex[nCType] = 0;
		//Clear LastCurvePoint Points
		for(int i = 0; i < MAX_CURVE_TEMPNUM; i++)
		{
			m_aryLastCurvePoint[nCType][i].nID		= 0;
			m_aryLastCurvePoint[nCType][i].nCount	= 0;
			memset(m_aryLastCurvePoint[nCType][i].Points,0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
		}
	}
	else  //??
	{
		for(int i = 0; i < MAX_CURVE_NUM_NEW; i++)
		{
			m_LastCurvePointIndex[i] = 0;
			memset(m_aryPoints[i],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
			m_aryIndex[i] = 0;
			//Clear LastCurvePoint Points Tempnum 10
			for(int j = 0; j < MAX_CURVE_TEMPNUM; j++)
			{
				m_aryLastCurvePoint[i][j].nID		= 0;
				m_aryLastCurvePoint[i][j].nCount	= 0;
				memset(m_aryLastCurvePoint[i][j].Points,0,sizeof(tmCurveOilPoint)*MAX_POINT_NUM);
			}
		}
	}
}

int		CtmControl_CurveData::CurveNubCovert(int nType)
{
	for(unsigned int i = 0; i < sizeof(u_CurveNubCovert)/(sizeof(int)*2); i++)
	{
		if(nType == u_CurveNubCovert[i][0])
		{
			return 	u_CurveNubCovert[i][1];
		}	
	}
	return -1;
}


#ifndef	D_BEOBJECT_CTMCONTRL_CURVEDATA
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
