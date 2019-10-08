/*===========================================================================+
|  Class    : CtmControl_CurveExData	                                     |
|  Task     : CurveData Service Function	    	                     	 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)4.3.2                                                  |
|  Link     : G++(GCC)4.3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 07/26/2011	                                                 |
|  Revision : 07/26/2011  				                                     |
+===========================================================================*/
#include	"tmControl_CurveExData.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"

IMPLEMENT_DYNCREATE(CtmControl_CurveExData, CtmControl)
/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
//此结构体来用转换内部存储index， 如果排序超过35个，则要修改lib Contrl中的MAX_CURVE_NUM_NEW才可以
int		u_CurveNubCovert[][2] = 
{
	{CURVE_DATA_CLAMP_OPEN, 				0},
	{CURVE_DATA_CLAMP_CLOSE, 				1},
	{CURVE_DATA_CLAMP,						2},
	{CURVE_DATA_EJECT_ADV,					3},
	{CURVE_DATA_EJECT_RET,					4},
	{CURVE_DATA_EJECT,						5},
	{CURVE_DATA_INJECT,						6},
	{CURVE_DATA_INJECT_FORCE,				7},
	{CURVE_DATA_INJECT_SUCKBACK,			8},
	{CURVE_DATA_INJECT_HOLD,				9},
	{CURVE_DATA_CHARGE,						10},
	{CURVE_DATA_CHARGE_TORQUE,				11},
	{CURVE_DATA_NOZL_ADV,					12},
	{CURVE_DATA_NOZL_RET,					13},
	{CURVE_DATA_NOZL,						14},
	{CURVE_DATA_CAM,						15},
	{CURVE_DATA_INJECT_LAST,				16},
	{CURVE_DATA_INJECT_HOLD_LAST,			17},	
	{CURVE_DATA_INJECTB,					18},
	{CURVE_DATA_INJECTB_HOLD,				19},
	{CURVE_DATA_CHARGEB,					20},
	{CURVE_DATA_INJECTB_LAST,				21},
	{CURVE_DATA_INJECTB_HOLD_LAST,			22},
	{CURVE2_SETDATA_CLAMP_OPEN_PRESS_POS_SPEED_TIME,		23},
	{CURVE2_SETDATA_CLAMP_CLOSE_PRESS_POS_SPEED_TIME,		24},
	{CURVE2_SETDATA_EJECT_ADV_PRESS_POS_SPEED_TIME,			25},
	{CURVE2_SETDATA_EJECT_REC_PRESS_POS_SPEED_TIME,			26},
	{CURVE2_SETDATA_INJECT_PRESS_POS_SPEED_TIME,			27},
	{CURVE2_SETDATA_CHARGE_PRESS_POS_SPEED_TIME,			28},
	{CURVE2_SETDATA_NOZL_ADV_PRESS_POS_SPEED_TIME,			29},
	{CURVE2_SETDATA_NOZL_REC_PRESS_POS_SPEED_TIME,			30},
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
|  Function : CtmControl_CurveExData::CtmControl_CurveExData()				         |
|  Task     :  CtmControl_CurveExData篶硑ㄧ计                                    |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_CurveExData::CtmControl_CurveExData(): CtmControl()
{ 
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveExData::~CtmControl_CurveExData()				             |
|  Task     :  CtmControl_CurveExData猂篶ㄧ计                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_CurveExData::~CtmControl_CurveExData()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveExData::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_CurveExData::CreateSelf()
{
	InitControl();
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveExData::FreeSelf()				             	     |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_CurveExData::FreeSelf()
{
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_CurveExData::InitControl()					             |
|  Task     :  Init Error Message 					                     	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_CurveExData::InitControl()
{
	int	nCtype = 0;
	memset(m_aryState, 0, sizeof(m_aryState));
	memset(m_aryIndex, 0, sizeof(m_aryIndex));
	//memset(m_aryPoints, 0, sizeof(m_aryPoints));
	
	memset(m_ary2State, 0, sizeof(m_ary2State));
	memset(m_ary2Index, 0, sizeof(m_ary2Index));
//	memset(m_ary2Points, 0, sizeof(m_ary2Points));
//	memset(m_ary2LastPoints, 0, sizeof(m_ary2LastPoints));		//James add 2012/7/12 17:07:11
	memset(m_ary2Complete, 0, sizeof(m_ary2Complete));
	//memset(m_ary2Count, MAX_CURVEEX_NUM_NEW, sizeof(m_ary2Count));
	
	memset(m_curvedata, 0,sizeof(m_curvedata));
	memset(m_curveLastdata, 0, sizeof(m_curveLastdata));		//James add for injectlast curve 2012/7/12 18:47:417

	for(int i = 0; i < MAX_CURVEEX_NUM_NEW; i++)
	{
		m_aryPoints_MaxNum[i] 	= MAX_POINT_NUM;
		m_ary2Points_MaxNum[i]	= MAX_POINT_NUM; 
		m_ary2Count[i]			= 0;
	}
	//针对射出和保压特别定制2000个点
	//Inject A
	nCtype = CurveNubCovert(CURVE_DATA_INJECT);
	if(nCtype >=0)
		m_ary2Points_MaxNum[nCtype] = MAX_POINT_NUM_NEWDEFINE;
		
	nCtype = CurveNubCovert(CURVE_DATA_INJECT_FORCE);
	if(nCtype >=0)
		m_ary2Points_MaxNum[nCtype] = MAX_POINT_NUM_NEWDEFINE;
		
	nCtype = CurveNubCovert(CURVE_DATA_INJECT_HOLD);
	if(nCtype >=0)
		m_ary2Points_MaxNum[nCtype] = MAX_POINT_NUM_NEWDEFINE;
	nCtype = CurveNubCovert(CURVE_DATA_CLAMP_OPEN);
	if(nCtype >=0)
		m_ary2Points_MaxNum[nCtype] = MAX_POINT_NUM_NEWDEFINE;
		
	nCtype = CurveNubCovert(CURVE_DATA_CLAMP_CLOSE);
	if(nCtype >=0)
		m_ary2Points_MaxNum[nCtype] = MAX_POINT_NUM_NEWDEFINE;

	nCtype = CurveNubCovert(CURVE_DATA_CHARGE);
	if(nCtype >=0)
		m_ary2Points_MaxNum[nCtype] = MAX_POINT_NUM_NEWDEFINE;
				
	for(int i = 0; i < MAX_CURVE_NUM_NEW; i++)
	{
		m_aryPoints[i] 	= NULL;
		m_ary2Points[i]	= NULL;
		//printf("m_aryPoints_MaxNum[i]=%d \n", m_aryPoints_MaxNum[i]);
		m_aryPoints[i] 	= new  tmCurveOilPoint[m_aryPoints_MaxNum[i]];
		m_ary2Points[i] = new  tmCurve2Point[m_aryPoints_MaxNum[i]];
		if(m_aryPoints[i] == NULL)
			printf("new memery error \n");
		if(m_ary2Points[i] == NULL)
			printf("new2 memery error \n");
			
		for(int j = 0; j < MAX_CURVE_TEMPNUM; j++)
		{
			m_curveLastdata[i][j].nState		= 0;
			m_curveLastdata[i][j].nCurveNum		= 0;
			m_curveLastdata[i][j].nCount		= 0;
			m_curveLastdata[i][j].curve 		= new tmCURVE_DATASET[m_ary2Points_MaxNum[i]];
		}
		m_curvedata[i].nState	= 0;
		m_curvedata[i].nCurveNum	= 0;
		m_curvedata[i].nCount	= 0;
		m_curvedata[i].curve 	= new tmCURVE_DATASET[m_ary2Points_MaxNum[i]];
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
int		CtmControl_CurveExData::SetData(int nType, double* dX, double* dY, int nNum)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
		
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		m_aryState[nCType] = 1;
		for (int i = 0; i < nNum; i++)
		{
			m_aryPoints[nCType][m_aryIndex[nCType]].dX = *(dX + i);
			m_aryPoints[nCType][m_aryIndex[nCType]].dY = *(dY + i);
			//printf("dx = %f,dy = %f\n",*dX,*dY);		
			m_aryIndex[nCType]++;
			if (m_aryIndex[nCType] >= m_aryPoints_MaxNum[nCType])
				m_aryIndex[nCType]--;
		}	
		return m_aryIndex[nCType];
	}
	return -1;
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
int		CtmControl_CurveExData::SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
		
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		m_aryState[nCType] = 1;
		for (int i = 0; i < nNum; i++)
		{
			m_aryPoints[nCType][m_aryIndex[nCType]].dX = *(dX + i);
			m_aryPoints[nCType][m_aryIndex[nCType]].dY = *(dY + i);
			//printf("dx = %f,dy = %f\n",*dX,*dY);		
			m_aryIndex[nCType]++;
			if (m_aryIndex[nCType] >= m_aryPoints_MaxNum[nCType])
				m_aryIndex[nCType]--;
		}	
		return m_aryIndex[nCType];
	}
	return -1;
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
int		CtmControl_CurveExData::SetData(int	nType, void* pInputData, int nNum)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
		
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		m_aryState[nCType] = 1;
		for (int i = 0; i < nNum; i++)
		{
			if(nCType < MAX_CURVE_NUM_NEW)
			{
				m_aryPoints[nCType][m_aryIndex[nCType]].dX = ((tmCurveOilPoint*)pInputData + i)->dX;
				m_aryPoints[nCType][m_aryIndex[nCType]].dY = ((tmCurveOilPoint*)pInputData + i)->dY;
				m_aryIndex[nCType]++;
				if (m_aryIndex[nCType] >= m_aryPoints_MaxNum[nCType])
					m_aryIndex[nCType]--;
			}
		}	
		return m_aryIndex[nCType];
	}
	return -1;
}

/*--------------------------------------------------------------------------+
|  Function :  SetData2(int	nType, void* pInputData, int nNum)				|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveExData::SetData2(int	nType, void* pInputData, int nNum)
{
	int	nCType = CurveNubCovert(nType);
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		if(m_ary2Complete[nCType] == 0)
		{
			//printf("SetData2 complete \n");
			m_ary2Index[nCType] = 0;
		}
		m_ary2State[nCType] = 1;
		m_ary2Complete[nCType] = 1;
		for (int i = 0; i < nNum; i++)
		{
			if(nCType < MAX_CURVE_NUM_NEW)
			{
				#ifndef D_5528
				if(m_ary2Index[nCType] > 0)
				{
					if( (m_ary2Points[nCType][m_ary2Index[nCType]-1].dX1 == ((tmCurve2Point*)pInputData + i)->dX1) &&
						(m_ary2Points[nCType][m_ary2Index[nCType]-1].dY1 == ((tmCurve2Point*)pInputData + i)->dY1))
					{
						//printf("continue type=%d \n", nCType);
						continue;	
					}
				}
				#endif
				m_ary2Points[nCType][m_ary2Index[nCType]].dX1 = ((tmCurve2Point*)pInputData + i)->dX1;
				m_ary2Points[nCType][m_ary2Index[nCType]].dY1 = ((tmCurve2Point*)pInputData + i)->dY1;
				m_ary2Points[nCType][m_ary2Index[nCType]].dX2 = ((tmCurve2Point*)pInputData + i)->dX2;
				m_ary2Points[nCType][m_ary2Index[nCType]].dY2 = ((tmCurve2Point*)pInputData + i)->dY2;
				m_ary2Index[nCType]++;
				///printf("m_ary2Index[%d]=%d, state=%d\n", nCType, m_ary2Index[nCType], m_ary2State[nCType]);
				if (m_ary2Index[nCType] >= m_aryPoints_MaxNum[nCType])
					m_ary2Index[nCType]--;
			}
		}
		return m_ary2Index[nCType];
	}
	return -1;
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
int		CtmControl_CurveExData::GetData(int nType, void* pOutData,int TempDataIndex)
{
	int Result = 0;
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		switch (nType)
		{
			//set curve
			//case 
			;
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetData2(int nType, void* pOutData)							|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveExData::GetData2(int nType, void* pOutData,int TempDataIndex)
{
	int Result = 0;
	//int	nTempType = 0;
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		//if(nType == CURVE_DATA_INJECT_LAST)
		//{
		//	nTempType = CURVE_DATA_INJECT;
		//	if(TempDataIndex >=0 && TempDataIndex < MAX_CURVE_TEMPNUM)
		//	{
		//		Result = m_ary2LastCount[0][TempDataIndex];
		//		memcpy(pOutData, m_ary2LastPoints[0][TempDataIndex], m_ary2LastCount[0][TempDataIndex] * sizeof(tmCurve2Point));
		//	}
		//}
		//else
		{
			Result = m_ary2Index[nCType];
			memcpy(pOutData, m_ary2Points[nCType], m_ary2Index[nCType] * sizeof(m_ary2Points[nCType][0]));
		}
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
void	CtmControl_CurveExData::SetComplete(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		m_aryState[nCType] = CURVE_END2;
		m_aryIndex[nCType] = 0;
	}
}

/*--------------------------------------------------------------------------+
|  Function :  SetComplete2(int nType)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmControl_CurveExData::SetComplete2(int nType)
{
//	printf("SetComplete2=%d nType=%d \n", SetComplete2, nType);
	//int nTemp = 0;
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		//printf("SetComplete2 type=%d coutn=%d \n", nType, m_ary2Index[nType] );
		m_ary2State[nCType] = CURVE_END2;
		m_ary2Complete[nCType] = 0;
		
//		nTemp = m_ary2Index[nType];
//		//Inject Curve 
//		if(nType == CURVE_DATA_INJECT)
//		{
//			if(m_ary2LastIndex[0] >=MAX_CURVE_TEMPNUM)
//				m_ary2LastIndex[0] = 0;
//			memcpy(m_ary2LastPoints[0][m_ary2LastIndex[0]], m_ary2Points[nType], nTemp*sizeof(tmCurve2Point));
//			m_ary2LastCount[0][m_ary2LastIndex[0]] 	= nTemp;		//SetCount
//			m_ary2LastIndex[0]++;
//		}
		//m_ary2Index[nType] = 0;
		//memset(m_ary2Points[nType], 0, sizeof(m_ary2Points[nType]));
	}
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
int		CtmControl_CurveExData::SetState(int nType, int nState)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		m_aryState[nCType] = nState;
		return m_aryIndex[nCType];
	}
	return -1;
}

/*--------------------------------------------------------------------------+
|  Function :  SetState2(int nType, int nState)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveExData::SetState2(int nType, int nState)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		m_ary2State[nCType] = nState;
		return m_ary2Index[nCType];
	}
	return -1;
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
int		CtmControl_CurveExData::GetState(int nType)	
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		return m_aryState[nCType];
	}
	return -1;
}

/*--------------------------------------------------------------------------+
|  Function :  GetState2(int nType)											|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmControl_CurveExData::GetState2(int nType)	
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		return m_ary2State[nCType];
	}
	return -1;
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
void    CtmControl_CurveExData::ClearBuffer(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
   	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
   	{
 		memset(m_aryPoints[nCType],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[nCType]);
   	}
   	else
   	{
   		for(int i = 0; i < CURVE2_LASTCURVE_MAXTYPE; i++)
		{
			memset(m_curveLastdata[i],0,sizeof(tmCURVE_DATASET)*m_aryPoints_MaxNum[nCType]);
		}
		for(int i = 0; i < MAX_CURVEEX_NUM_NEW; i++)
		{
			memset(m_aryPoints[i],0,sizeof(tmCurveOilPoint)*m_aryPoints_MaxNum[i]);
		}
   	}
}

void	CtmControl_CurveExData::SetPointCount(int nType, int nCount)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
	if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
	{
		if(m_ary2Count[nCType] > 0)
		{
			;//delete
		}
		m_ary2Count[nCType] = nCount;
	}
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
void    CtmControl_CurveExData::ClearBuffer2(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
    if(nCType < MAX_CURVE_NUM_NEW && nCType >= 0)
   	{
 		memset(m_ary2Points[nCType], 0, sizeof(tmCurve2Point)*m_aryPoints_MaxNum[nCType]);
   	}
   	else
   	{
   		for(int i = 0; i < MAX_CURVEEX_NUM_NEW; i++)
		{
			memset(m_ary2Points[i],0,sizeof(tmCurve2Point)*m_aryPoints_MaxNum[nCType]);
		}
		for(int i = 0; i < CURVE2_LASTCURVE_MAXTYPE; i++)
		{
			memset(m_curveLastdata[i],0,sizeof(tmCURVE_DATASET)*m_aryPoints_MaxNum[nCType]);
		}
   	}
}

void	CtmControl_CurveExData::Clear(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
		
	switch(nCType)
	{
		/*case 	CURVE_DATA_INJECT_LAST:
			for(int i = 0; i < MAX_CURVE_TEMPNUM; i++)
				m_curveLastdata[0][i].nCount = 0;
			break;
		case 	CURVE_DATA_INJECT_HOLD_LAST:
			for(int i = 0; i < MAX_CURVE_TEMPNUM; i++)
				m_curveLastdata[1][i].nCount = 0;
			break;
		case 	CURVE_DATA_INJECTB_LAST:
			for(int i = 0; i < MAX_CURVE_TEMPNUM; i++)
				m_curveLastdata[2][i].nCount = 0;
			break;
		case 	CURVE_DATA_INJECTB_HOLD_LAST:
			for(int i = 0; i < MAX_CURVE_TEMPNUM; i++)
				m_curveLastdata[3][i].nCount = 0;
			break;
		*/
		default:
			if (nCType >= 0 && nCType < MAX_CURVE_NUM)
			{
				//printf("000000000000000000 m_curvedata[nCType].nCount= %d\n",m_curvedata[nCType].nCount);
				m_curvedata[nCType].nCount = 0;
				//printf("111111111111111111 m_curvedata[nCType].nCount= %d\n",m_curvedata[nCType].nCount);
			}
			break;
	}
}

int		CtmControl_CurveExData::AddPoint(int nType, void* pData, int nCurveNum)
{
	int	nCType = CurveNubCovert(nType);
	if(nCType <0 )
		return 0;
		
	if(nCType < 0 || nCType >= MAX_CURVEEX_NUM_NEW ||  
		nCurveNum <= 0 || nCurveNum > MAX_POINT_PER_SET)
		return -1;
		
	//printf("GetCurveState(nType):%d\n",GetCurveState(nType));	
	if (GetCurveState(nType) == CURVE_END2 /*|| GetCurveState(nType) == CURVE_DONE*/)
	{
		Clear(nType);
	}
	if (pData == NULL)
	{
		SetCurveState(nType, CURVE_END2);
		//printf("pData == NULL\n");
		//Save last curve data James add 2012/7/13 9:00:317
		switch(nType)
		{
			case CURVE_DATA_INJECT:
				if(m_ary2LastIndex[0] >=MAX_CURVE_TEMPNUM)
					m_ary2LastIndex[0] = 0;
				memcpy(&m_curveLastdata[0][m_ary2LastIndex[0]], &m_curvedata[nType], sizeof(tmHYB5CURVE));
				//printf("m_ary2LastIndex[0]=%d nCount=%d \n", m_ary2LastIndex[0], m_curvedata[nType].nCount);
				m_ary2LastIndex[0]++;
				break;
			case CURVE_DATA_INJECT_HOLD:
				if(m_ary2LastIndex[1] >=MAX_CURVE_TEMPNUM)
					m_ary2LastIndex[1] = 0;
				memcpy(&m_curveLastdata[1][m_ary2LastIndex[1]], &m_curvedata[nType], sizeof(tmHYB5CURVE));
				//printf("m_ary2LastIndex[0]=%d nCount=%d \n", m_ary2LastIndex[1], m_curvedata[nType].nCount);
				m_ary2LastIndex[1]++;
				break;	
			case CURVE_DATA_INJECTB:
				if(m_ary2LastIndex[2] >=MAX_CURVE_TEMPNUM)
					m_ary2LastIndex[2] = 0;
				memcpy(&m_curveLastdata[2][m_ary2LastIndex[2]], &m_curvedata[nType], sizeof(tmHYB5CURVE));
				//printf("m_ary2LastIndex[0]=%d nCount=%d \n", m_ary2LastIndex[0], m_curvedata[nType].nCount);
				m_ary2LastIndex[2]++;
				break;
			case CURVE_DATA_INJECTB_HOLD:
				if(m_ary2LastIndex[3] >=MAX_CURVE_TEMPNUM)
					m_ary2LastIndex[3] = 0;
				memcpy(&m_curveLastdata[3][m_ary2LastIndex[3]], &m_curvedata[nType], sizeof(tmHYB5CURVE));
				//printf("m_ary2LastIndex[0]=%d nCount=%d \n", m_ary2LastIndex[3], m_curvedata[nType].nCount);
				m_ary2LastIndex[3]++;
				break;
		}
		//if(nType == CURVE_DATA_INJECT)
		//{
		//	if(m_ary2LastIndex[0] >=MAX_CURVE_TEMPNUM)
		//		m_ary2LastIndex[0] = 0;
		//	memcpy(&m_curveLastdata[0][m_ary2LastIndex[0]], &m_curvedata[nType], sizeof(tmHYB5CURVE));
		//	//printf("m_ary2LastIndex[0]=%d nCount=%d \n", m_ary2LastIndex[0], m_curvedata[nType].nCount);
		//	m_ary2LastIndex[0]++;
		//}
		//
		return 0;
	}
	//if (m_curvedata[nType].nCount > 0 &&
	//	m_curvedata[nType].curve[m_curvedata[nType].nCount - 1].dX == ((tmCURVE_DATASET *)pData)->dX)
	//{
	//	return 0;
	//}
	if (m_curvedata[nCType].nCount < m_ary2Points_MaxNum[nCType])
	{
		memcpy(&(m_curvedata[nCType].curve[m_curvedata[nCType].nCount]), pData, sizeof(tmCURVE_DATASET));
		//if(nCType==0)
		//printf("m_curvedata[nCType].nCount=%d dx=%lf, dY=%lf \n", m_curvedata[nCType].nCount, m_curvedata[nCType].curve[m_curvedata[nCType].nCount].dX,
		//m_curvedata[nCType].curve[m_curvedata[nCType].nCount].dY[0]);
		m_curvedata[nCType].nCount++;
		//if(nCType==CURVE_DATA_CHARGE)
		//	printf(":::::::m_curvedata[nCType].nCount=%d\n",m_curvedata[nCType].nCount);		
		m_curvedata[nCType].nCurveNum = nCurveNum;
		m_curvedata[nCType].nState = CURVE_UPDATE;
		if(m_curvedata[nCType].nCount>=m_ary2Points_MaxNum[nCType])
			m_curvedata[nCType].nCount--;
	}
	return 0;
}

int		CtmControl_CurveExData::GetCurveState(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	if(nCType < 0 || nCType >= MAX_CURVEEX_NUM_NEW)
		return 0;
	
	return m_curvedata[nCType].nState;
}

void	CtmControl_CurveExData::SetCurveState(int nType, int nState)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return ;
	if(nCType >=0 && nCType < MAX_CURVEEX_NUM_NEW)
		m_curvedata[nCType].nState = nState;
}

int		CtmControl_CurveExData::GetCurveLastIndex(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType <0 )
		return 0;
	int nIndex = 0;
	switch(nCType)
	{
		case 0:
			nIndex = m_ary2LastIndex[0];
			break;
		case 1:
			nIndex = m_ary2LastIndex[1];
			break;
		case 2:
			nIndex = m_ary2LastIndex[2];
			break;
		case 3:
			nIndex = m_ary2LastIndex[3];
			break;
		default:
			break;	
	}
	return 	nIndex;
}

int		CtmControl_CurveExData::GetCurveData(int nType, void *pData, int nPoint, int TempDataIndex)
{
	int nCount = 0;
	//int	nTempType = 0;
	int	nCType = CurveNubCovert(nType);
	//printf("nType=%d,nCType=%d\n",nType,nCType);
	
	if(nCType < 0 || nCType >= MAX_CURVEEX_NUM_NEW || nPoint <= 0)
		return 0;
	else
	{
		//Last Inject curve James add 2012/7/12 18:54:48
		switch(nType)
		{
			case 	CURVE_DATA_INJECT_LAST:
				if(TempDataIndex >=0 && TempDataIndex < MAX_CURVE_TEMPNUM)
				{
					if (nPoint >= m_curveLastdata[0][TempDataIndex].nCount)
					{
						memcpy(pData, m_curveLastdata[0][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * m_curveLastdata[0][TempDataIndex].nCount);
						nCount = m_curveLastdata[0][TempDataIndex].nCount;
					}
					else
					{
						memcpy(pData, m_curveLastdata[0][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * nPoint);
						nCount = nPoint;
					}
					//printf("nPoint=%d 2=%d \n", nPoint, m_curveLastdata[0][TempDataIndex].nCount);
				}
				break;
			case	CURVE_DATA_INJECT_HOLD_LAST:
				if(TempDataIndex >=0 && TempDataIndex < MAX_CURVE_TEMPNUM)
				{
					if (nPoint >= m_curveLastdata[1][TempDataIndex].nCount)
					{
						memcpy(pData, m_curveLastdata[1][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * m_curveLastdata[1][TempDataIndex].nCount);
						nCount = m_curveLastdata[1][TempDataIndex].nCount;
					}
					else
					{
						memcpy(pData, m_curveLastdata[1][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * nPoint);
						nCount = nPoint;
					}
					//printf("nPoint=%d 2=%d \n", nPoint, m_curveLastdata[1][TempDataIndex].nCount);
				}
				break;
			case CURVE_DATA_INJECTB_LAST:
				if(TempDataIndex >=0 && TempDataIndex < MAX_CURVE_TEMPNUM)
				{
					if (nPoint >= m_curveLastdata[2][TempDataIndex].nCount)
					{
						memcpy(pData, m_curveLastdata[2][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * m_curveLastdata[2][TempDataIndex].nCount);
						nCount = m_curveLastdata[2][TempDataIndex].nCount;
					}
					else
					{
						memcpy(pData, m_curveLastdata[2][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * nPoint);
						nCount = nPoint;
					}
					//printf("nPoint=%d 2=%d \n", nPoint, m_curveLastdata[0][TempDataIndex].nCount);
				}
				break;
				
			case	CURVE_DATA_INJECTB_HOLD_LAST:
				if(TempDataIndex >=0 && TempDataIndex < MAX_CURVE_TEMPNUM)
				{
					if (nPoint >= m_curveLastdata[3][TempDataIndex].nCount)
					{
						memcpy(pData, m_curveLastdata[3][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * m_curveLastdata[3][TempDataIndex].nCount);
						nCount = m_curveLastdata[3][TempDataIndex].nCount;
					}
					else
					{
						memcpy(pData, m_curveLastdata[3][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * nPoint);
						nCount = nPoint;
					}
					//printf("nPoint=%d 2=%d \n", nPoint, m_curveLastdata[1][TempDataIndex].nCount);
				}
				break;
			default:
				///if(nCType==CURVE_DATA_CHARGE)
				///	 printf("nCType=%d,nPoint=%d,m_curvedata[nCType].nCount=%d\n",nCType,nPoint,m_curvedata[nCType].nCount);					
				if (nPoint >= m_curvedata[nCType].nCount)
				{
					nCount = m_curvedata[nCType].nCount;
					memcpy(pData, m_curvedata[nCType].curve, sizeof(tmCURVE_DATASET) * nCount);
					
				}
				else
				{
					memcpy(pData, m_curvedata[nCType].curve, sizeof(tmCURVE_DATASET) * nPoint);
					nCount = nPoint;
				}
				//if (GetCurveState(nType) == CURVE_UPDATE)
				//	SetCurveState(nType, CURVE_NONE);
				//if (GetCurveState(nType) == CURVE_END2)
				//	SetCurveState(nType, CURVE_DONE);
				break;
		}
		//if(nType == CURVE_DATA_INJECT_LAST)
		//{
		//	nTempType = CURVE_DATA_INJECT;
		//	
		//	if(TempDataIndex >=0 && TempDataIndex < MAX_CURVE_TEMPNUM)
		//	{
		//		if (nPoint >= m_curveLastdata[0][TempDataIndex].nCount)
		//		{
		//			memcpy(pData, m_curveLastdata[0][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * m_curveLastdata[0][TempDataIndex].nCount);
		//			nCount = m_curveLastdata[0][TempDataIndex].nCount;
		//		}
		//		else
		//		{
		//			memcpy(pData, m_curveLastdata[0][TempDataIndex].curve, sizeof(tmCURVE_DATASET) * nPoint);
		//			nCount = nPoint;
		//		}
		//		printf("nPoint=%d 2=%d \n", nPoint, m_curveLastdata[0][TempDataIndex].nCount);
		//	}
		//}
		//else
		//{
		//	if (GetCurveState(nType) == CURVE_UPDATE)
		//		SetCurveState(nType, CURVE_NONE);
		//	if (GetCurveState(nType) == CURVE_END2)
		//		SetCurveState(nType, CURVE_DONE);
		//	
		//	if (nPoint >= m_curvedata[nType].nCount)
		//	{
		//		memcpy(pData, m_curvedata[nType].curve, sizeof(tmCURVE_DATASET) * m_curvedata[nType].nCount);
		//		nCount = m_curvedata[nType].nCount;
		//	}
		//	else
		//	{
		//		memcpy(pData, m_curvedata[nType].curve, sizeof(tmCURVE_DATASET) * nPoint);
		//		nCount = nPoint;
		//	}
		//}
		return nCount;
	}
}

BOOL	CtmControl_CurveExData::CurveUpdate(int nType)
{
	int	nCType = CurveNubCovert(nType);
	
	if(nCType < 0)
		return FALSE;
	return (GetCurveState(nType) == CURVE_UPDATE || 
			GetCurveState(nType) ==	CURVE_END2);
}


int		CtmControl_CurveExData::CurveNubCovert(int nType)
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

#ifndef	D_BEOBJECT_CTMCONTRL_CURVEEXDATA
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
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
