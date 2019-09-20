/*==========================================================================+
|  Class    : Meter  Control						     					|
|  Task     : Meter  Control file          	            					|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Caozz	                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2014/05/19                                                    |
|  Revision :                                                               |
+==========================================================================*/
#include	"meter.h"
#include	"utils.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Class implementation - Meter                                    |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmMeter, CtmWnd)
CtmMeter::~CtmMeter()
{
	m_BgImage.FreeImage();   
	 delete m_pDoubleBuffer;
}

CtmMeter::CtmMeter(): CtmWnd()
{
	strcpy(TYPEName, "CtmMeter");

	m_pbmpMeter 		= NULL;
	m_pszMeterPath 		= NULL;
	m_pszPointerPath	= NULL;
	
	m_wPointerWidth 	= 0;
	m_wPointerHeight 	= 0;
	m_PointerORG.ix 	= 0;
	m_PointerORG.iy 	= 0;
	m_pointMeterORG.ix 	= 0;
	m_pointMeterORG.iy 	= 0;
	m_fStartAngle 	 	= 0;
	m_fEndAngle 	 	= 0;
	m_fMaxAngle 	 	= 0;
	
	m_pszDBID 			= NULL;
	m_pszMaxID 			= NULL;
	m_pszMinID			= NULL;
	
	m_fValue 	 		= 0;
	m_fMin 	 			= 0;
	m_fMax 	 			= 0;
	m_fUnit				= 0;
	
	m_bCopy 			= FALSE;
	m_pDoubleBuffer		= NULL;
	m_dwKeyColor		= 0xFFFF;
	m_wImageType		= 0;
	m_wDirection		= POINTERR_FORWARD;
	m_wInterval			= 0;
	
	m_pDialPlateBuff 	= NULL;
	m_pPointerBuff		= NULL;
	m_pBackBuff			= NULL;
	
	m_pPointerORGCoordinate		= NULL;
	m_pPointerCoordinate		= NULL;
	m_pPointerCoordinateFlag	= NULL;
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
void    CtmMeter::Show()
{
	if (!m_bCopy)
	{
		CopyBackground(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right,m_rectWindow.bottom);
		m_bCopy = TRUE;
	}
	
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	GetDBInfo();
	
	if (m_wnd.wStyle & tmWS_SHOW)
	{
		SetDialPlateToDoubleBuffer();
		SetPointerToDoubleBuffer();
		
		if (m_pDoubleBuffer != NULL)
			m_pDoubleBuffer->DrawWithDoubleBuffer();
	
		CtmWnd::Show();	
		
	}

}

void    CtmMeter::Update()
{
	SetDoubleBuffer();
	
	//if(m_pDoubleBuffer == NULL)
		//Clear();
	Show();
}

void	CtmMeter::Clear()
{
	//if (m_bCopy)
	//		ShowBackground(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
//		m_BgImage.ShowImage(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
	
}

void 	CtmMeter::SetPropertys()
{
	CtmWnd::SetPropertys();

	SetPropInfo("type", 			tmWORD, 	&m_wImageType);
	SetPropInfo("direction", 		tmWORD, 	&m_wDirection);
	SetPropInfo("interval", 		tmWORD, 	&m_wInterval);
	SetPropInfo("keycolor", 		tmLONG,		&m_dwKeyColor);
	SetPropInfo("meterpath",		tmSTR,		&m_pszMeterPath);
	SetPropInfo("pointerpath",		tmSTR,		&m_pszPointerPath);
	
	SetPropInfo("pointer.width", 	tmWORD, 	&m_wPointerWidth);
	SetPropInfo("pointer.height", 	tmWORD, 	&m_wPointerHeight);
	SetPropInfo("pointerorg.x", 	tmWORD, 	&m_PointerORG.ix);
	SetPropInfo("pointerorg.y", 	tmWORD, 	&m_PointerORG.iy);
	
	SetPropInfo("origin.x", 		tmWORD, 	&m_pointMeterORG.ix);
	SetPropInfo("origin.y", 		tmWORD, 	&m_pointMeterORG.iy);
	SetPropInfo("startangle",		tmFLOAT, 	&m_fStartAngle);
	SetPropInfo("endangle",			tmFLOAT, 	&m_fEndAngle);
	
	SetPropInfo("dbid0",			tmSTR, 		&m_pszDBID);
	SetPropInfo("maxid",			tmSTR, 		&m_pszMaxID);
	SetPropInfo("minid",			tmSTR, 		&m_pszMinID);
	SetPropInfo("value", 			tmFLOAT, 	&m_fValue);	
	SetPropInfo("min", 				tmFLOAT, 	&m_fMin);
	SetPropInfo("max", 				tmFLOAT, 	&m_fMax);
}


BOOL 	CtmMeter::CreateA()
{
	CtmWnd::CreateA();	
	SetDoubleBuffer();
	InitBuff();
	SetImageBuffer();
	GetPointerORGCoordinate();
}


void	CtmMeter::SetDoubleBuffer()
{
	if (m_pDoubleBuffer == NULL)
	{
		m_pDoubleBuffer = new CtmGDIDoubleBuffer(m_rectClient);
		m_pDoubleBuffer->InitBuffer(m_bgc);
	}
}

CtmBitMap*	CtmMeter::GetBmpImage(char* pszImagePath)
{
	int i = 0;
	bool b_find = false;
	
	CtmBitMap* pBmpImage = NULL;
	while (g_aIniBmp[i].cBMPPath[0] != '\0' && i < IMAGE_MAXNUM)
	{
		if (strcmp(g_aIniBmp[i].cBMPPath, pszImagePath) == 0)
		{
			pBmpImage = g_aIniBmp[i].pIniBmp; 
			b_find = true;
			break;
		}
		i++;
	}	
	if (!b_find)
	{
		if(i < IMAGE_MAXNUM)
		{
			g_aIniBmp[i].pIniBmp = new CtmBitMap;
			g_aIniBmp[i].pIniBmp->Create(pszImagePath);
			strncpy(g_aIniBmp[i].cBMPPath, pszImagePath, 256);
			pBmpImage = g_aIniBmp[i].pIniBmp; 
		}
		else
		{
			m_bmpImage.Create(pszImagePath);	
			pBmpImage = &m_bmpImage;	
		}
		
	}
	
	return pBmpImage;
}

void	CtmMeter::DestroyBuffer()
{
	if (m_pDialPlateBuff != NULL)
	{
		for (int i = 0; i < (m_rectClient.bottom - m_rectClient.top + 1); i++)
		{
			if(*(m_pDialPlateBuff + i) != NULL)
			{
				delete [](*(m_pDialPlateBuff + i));
				*(m_pDialPlateBuff + i) = NULL;
			}
		}
		delete []m_pDialPlateBuff;
		m_pDialPlateBuff = NULL;
	}
	
	if (m_pPointerBuff != NULL)
	{
		for (int i = 0; i < (m_wPointerHeight + 1); i++)
		{
			if(*(m_pPointerBuff + i) != NULL)
			{
				delete [](*(m_pPointerBuff + i));
				*(m_pPointerBuff + i) = NULL;
			}
		}
		delete []m_pPointerBuff;
		m_pPointerBuff = NULL;
	}
	
	if (m_pBackBuff != NULL)
	{
		for (int i = 0; i < (m_rectClient.bottom - m_rectClient.top + 1); i++)
		{
			if(*(m_pBackBuff + i) != NULL)
			{
				delete [](*(m_pBackBuff + i));
				*(m_pBackBuff + i) = NULL;
			}
		}
		delete []m_pBackBuff;
		m_pBackBuff = NULL;
	}
	
	if (m_pPointerORGCoordinate != NULL)
	{
		for (int i = 0; i < (m_wPointerHeight + 1); i++)
		{
			if(*(m_pPointerORGCoordinate + i) != NULL)
			{
				delete [](*(m_pPointerORGCoordinate + i));
				*(m_pPointerORGCoordinate + i) = NULL;
			}
		}
		delete []m_pPointerORGCoordinate;
		m_pPointerORGCoordinate = NULL;
	}
	
	if (m_pPointerCoordinate != NULL)
	{
		for (int i = 0; i < (m_wPointerHeight + 1); i++)
		{
			if(*(m_pPointerCoordinate + i) != NULL)
			{
				delete [](*(m_pPointerCoordinate + i));
				*(m_pPointerCoordinate + i) = NULL;
			}
		}
		delete []m_pPointerCoordinate;
		m_pPointerCoordinate = NULL;
	}
	
	
	if (m_pPointerCoordinateFlag != NULL)
	{
		for (int i = 0; i < m_wViewHeight; i++)
		{
			if(*(m_pPointerCoordinateFlag + i) != NULL)
			{
				delete [](*(m_pPointerCoordinateFlag + i));
				*(m_pPointerCoordinateFlag + i) = NULL;
			}
		}
		delete []m_pPointerCoordinateFlag;
		m_pPointerCoordinateFlag = NULL;
	}
	
		
}


void	CtmMeter::InitBuff()
{
	m_wViewHeight = GetClientHeight();
	m_wViewWidth = 	GetClientWidth();
	DestroyBuffer();
	
	#ifdef	D_32BIT
	m_pDialPlateBuff = new DWORD* [(m_rectClient.bottom - m_rectClient.top + 1)];
	#else
	m_pDialPlateBuff = new WORD* [(m_rectClient.bottom - m_rectClient.top + 1)];
	#endif
	if (m_pDialPlateBuff != NULL)
	{
		for (int i = 0; i < (m_rectClient.bottom - m_rectClient.top + 1); i++)
		{
			#ifdef	D_32BIT
			*(m_pDialPlateBuff + i) = new DWORD [(m_rectClient.right - m_rectClient.left + 1)];
			memset(*(m_pDialPlateBuff + i), 0, (m_rectClient.right - m_rectClient.left + 1)*sizeof(DWORD));
			#else
			*(m_pDialPlateBuff + i) = new WORD [(m_rectClient.right - m_rectClient.left + 1)];
			memset(*(m_pDialPlateBuff + i), 0, (m_rectClient.right - m_rectClient.left + 1)*sizeof(WORD));
			#endif
		}
	}
	
	#ifdef	D_32BIT
	m_pPointerBuff	= new DWORD* [(m_wPointerHeight + 1)];
	#else
	m_pPointerBuff	= new WORD* [(m_wPointerHeight + 1)];
	#endif
	
	if (m_pPointerBuff != NULL)
	{
		for (int i = 0; i < (m_wPointerHeight + 1); i++)
		{
			#ifdef	D_32BIT
			*(m_pPointerBuff + i) = new DWORD [(m_wPointerWidth + 1)];
			memset(*(m_pPointerBuff + i), 0, (m_wPointerWidth + 1)*sizeof(DWORD));
			#else
			*(m_pPointerBuff + i) = new WORD [(m_wPointerWidth + 1)];
			memset(*(m_pPointerBuff + i), 0, (m_wPointerWidth + 1)*sizeof(WORD));
			#endif
		}
	}
	
	#ifdef	D_32BIT
	m_pBackBuff = new DWORD* [(m_rectClient.bottom - m_rectClient.top + 1)];
	#else
	m_pBackBuff = new WORD* [(m_rectClient.bottom - m_rectClient.top + 1)];
	#endif
	
	if (m_pBackBuff != NULL)
	{
		for (int i = 0; i < (m_rectClient.bottom - m_rectClient.top + 1); i++)
		{
			#ifdef	D_32BIT
			*(m_pBackBuff + i) = new DWORD [(m_rectClient.right - m_rectClient.left + 1)];
			memset(*(m_pBackBuff + i), 0, (m_rectClient.right - m_rectClient.left + 1)*sizeof(DWORD));
			#else
			*(m_pBackBuff + i) = new WORD [(m_rectClient.right - m_rectClient.left + 1)];
			memset(*(m_pBackBuff + i), 0, (m_rectClient.right - m_rectClient.left + 1)*sizeof(WORD));
			#endif
		}
	}
	
	m_pPointerORGCoordinate	= new tmPoint* [(m_wPointerHeight + 1)];
	
	if (m_pPointerORGCoordinate != NULL)
	{
		for (int i = 0; i < (m_wPointerHeight + 1); i++)
		{
			*(m_pPointerORGCoordinate + i) = new tmPoint [(m_wPointerWidth + 1)];
			memset(*(m_pPointerORGCoordinate + i), 0, (m_wPointerWidth + 1)*sizeof(tmPoint));
		}
	}
	
	
	m_pPointerCoordinate	= new tmPoint* [(m_wPointerHeight + 1)];
	
	if (m_pPointerCoordinate != NULL)
	{
		for (int i = 0; i < (m_wPointerHeight + 1); i++)
		{
			*(m_pPointerCoordinate + i) = new tmPoint [(m_wPointerWidth + 1)];
			memset(*(m_pPointerCoordinate + i), 0, (m_wPointerWidth + 1)*sizeof(tmPoint));
		}
	}
	
	m_pPointerCoordinateFlag	= new BOOL* [m_wViewHeight];
	
	if (m_pPointerCoordinateFlag != NULL)
	{
		for (int i = 0; i < m_wViewHeight; i++)
		{
			*(m_pPointerCoordinateFlag + i) = new BOOL [m_wViewWidth];
			memset(*(m_pPointerCoordinateFlag + i), 0, m_wViewWidth*sizeof(BOOL));
		}
	}
	
	
}


void	CtmMeter::SetImageBuffer()
{
	RECT rectDialPlat, rectPointer;
	
	if (m_pszMeterPath == NULL || m_pszPointerPath == NULL) 
	{
		return;
	}
	
	m_pbmpMeter = GetBmpImage(m_pszMeterPath);
	m_pbmpPointer = GetBmpImage(m_pszPointerPath);
	
	MKRECT(rectDialPlat,   0, 0,  m_wViewWidth-1,  m_wViewHeight-1)
	//printf("rectDialPlat.left = %d, %d, %d, %d\n", rectDialPlat.left, rectDialPlat.right, rectDialPlat.top, rectDialPlat.bottom);
	m_pbmpMeter->SetImageBuffer(rectDialPlat,  m_pDialPlateBuff);
	
	MKRECT(rectPointer,   0, 0,  m_wPointerWidth-1,  m_wPointerHeight-1)
	//printf("rectPointer.left = %d, %d, %d, %d\n", rectPointer.left, rectPointer.right, rectPointer.top, rectPointer.bottom);
	m_pbmpPointer->SetImageBuffer(rectPointer,  m_pPointerBuff);
}


void	CtmMeter::GetDBInfo()
{
	if(m_pszDBID != NULL)
		m_fValue = g_pDatabase->Read(m_pszDBID).fData;
		
	if(m_pszMaxID != NULL)
		m_fMax = g_pDatabase->Read(m_pszMaxID).fData;
		
	if(m_pszMinID != NULL)
		m_fMin = g_pDatabase->Read(m_pszMinID).fData;
	
	if(m_wDirection == POINTERR_BACKWARD)
	{
		if(m_fEndAngle >= 180)
			m_fMaxAngle = (360-m_fEndAngle) + m_fStartAngle;
		else
			m_fMaxAngle = m_fStartAngle - m_fEndAngle;
	}
	else
	{
		if(m_fStartAngle >= 180)
			m_fMaxAngle = (360-m_fStartAngle) + m_fEndAngle;
		else
			m_fMaxAngle = m_fEndAngle - m_fStartAngle;
	}
	
	
	if(m_fValue >= m_fMax) m_fValue = m_fMax;
	if(m_fValue <= m_fMin) m_fValue = m_fMin;
		
	m_fUnit = (m_fMax - m_fMin) / m_fMaxAngle;
	//printf("m_fValue = %f, m_fMax = %f, m_fMin = %f, m_fUnit = %f\n", m_fValue, m_fMax, m_fMin, m_fUnit);
}

void	CtmMeter::CopyBackground(WORD left, WORD top, WORD right, WORD bottom)
{
	int i, j;
	int x1, y1, x2, y2;
	x1 = 0;
	y1 = 0;
	x2 = m_wViewWidth-1;
	y2 = m_wViewHeight-1;
	_SetViewPort(left, top, right, bottom);
	for(i = y1; i <= y2; i ++)
	{
		for(j = x1; j <= x2; j ++)			
		{
			m_pBackBuff[i][j]= g_Rows[i][j];
		}
	}
	
}

void	CtmMeter::GetPointerORGCoordinate()
{
	int i,j;
	tmPoint	pointTemp;
	
	pointTemp.ix = m_pointMeterORG.ix - m_PointerORG.ix;
	pointTemp.iy = m_pointMeterORG.iy - m_PointerORG.iy;
	
	for(i = 0; i < m_wPointerHeight; i++)
	{
		for(j = 0; j < m_wPointerWidth; j++)
		{
			m_pPointerORGCoordinate[i][j].ix = pointTemp.ix+j;
			m_pPointerORGCoordinate[i][j].iy = pointTemp.iy+i;
			
			//printf("m_pPointerORGCoordinate[i][j].ix = %d, %d\n", m_pPointerORGCoordinate[i][j].ix, m_pPointerORGCoordinate[i][j].iy);
		}
	}
	
}

void	CtmMeter::SetDialPlateToDoubleBuffer()
{
	#ifdef	D_32BIT
	unsigned long**		pTemp = NULL;
	#else
	unsigned short**	pTemp = NULL;	
	#endif
	int		Row, Col;
	
	if (m_pDoubleBuffer != NULL)
	{
		pTemp = m_pDoubleBuffer->GetBuffer();
	}
	if(pTemp == NULL) return;
		
	
	Row = m_wViewHeight;
	Col = m_wViewWidth;
	
	int i, j;
	if(m_wImageType & tmBMP_KEYCOLOR)
	{
		for(i = 0; i < Row; i++)
		{
			for(j = 0; j < Col; j++)
			{
				#ifdef	D_32BIT
				if (m_pDialPlateBuff[i][j] != RGB565TORGB888(m_dwKeyColor))
				#else
				if (m_pDialPlateBuff[i][j] != m_dwKeyColor) 
				#endif
				{
					pTemp[i][j] = m_pDialPlateBuff[i][j];
				}
				else
				{
					pTemp[i][j] = m_pBackBuff[i][j];
				}
			}
		}
	}
	else
	{
		for(i = 0; i < Row; i++)
		{
			#ifdef	D_32BIT
			memcpy(pTemp[i], m_pDialPlateBuff[i], (Col+1)*4);
			#else
			memcpy(pTemp[i], m_pDialPlateBuff[i], (Col+1)*2);
			#endif
		}
	}
}


void	CtmMeter::SetPointerToDoubleBuffer()
{
	float	fAngle = 0;
	int i, j;
	#ifdef	D_32BIT
	DWORD**	pTemp = NULL;
	#else
	WORD**	pTemp = NULL;
	#endif
	
	int		nPointerStartY =  m_pPointerORGCoordinate[0][0].iy;
	
	if (m_pDoubleBuffer != NULL)
	{
		pTemp = m_pDoubleBuffer->GetBuffer();
	}
	if(pTemp == NULL) return;
	
	InitPointerFlag();
	
	fAngle = GetPointerAngle();
	
	for(i = 0; i < m_wPointerHeight; i++)
	{
		for(j = 0; j < m_wPointerWidth; j++)
		{
			m_pPointerCoordinate[i][j] = GetPointCoordinate(fAngle, m_pPointerORGCoordinate[i][j], m_pointMeterORG,nPointerStartY);
			
			#ifdef	D_32BIT
			if(m_pPointerBuff[i][j] != RGB565TORGB888(m_dwKeyColor))
			#else
			if(m_pPointerBuff[i][j] != m_dwKeyColor)
			#endif
			{
				pTemp[m_pPointerCoordinate[i][j].iy][m_pPointerCoordinate[i][j].ix] = m_pPointerBuff[i][j];
			}
			m_pPointerCoordinateFlag[m_pPointerCoordinate[i][j].iy][m_pPointerCoordinate[i][j].ix] = TRUE;
		}
	}
	
	OptimizePoint();
	
	//if(m_wImageType & tmBMP_KEYCOLOR)
	//{
	//	for(i = 0; i < m_wViewHeight; i++)
	//	{
	//		for(j = 0; j < m_wViewWidth; j++)
	//		{
	//			if (pTemp[i][j] == m_dwKeyColor) 
	//			{
	//				pTemp[i][j] = m_pDialPlateBuff[i][j];
	//			}
	//		}
	//	}
	//}
	
}

float	CtmMeter::GetPointerAngle()
{
	float	fStartAngle = 0;
	float	fAngle = 0;
	float	fValue = 0;
	float	fUnitValue = 0;
	float	fTemp = 0;
	
	if(m_wInterval != 0)
	{
		if(m_wInterval < INTERVAL_MIN) m_wInterval = INTERVAL_MIN;
		if(m_wInterval > m_fMaxAngle) m_wInterval = m_fMaxAngle;
		fValue = m_fValue;
		
		fUnitValue = m_fMax / m_wInterval;
		
		fTemp = m_fValue / fUnitValue;
		
		if((fTemp - (int)fTemp) > 0)
			fValue = ((int)fTemp + 1) * fUnitValue;
		
	}
	else
	{
		fValue = m_fValue;
	}	
	
	
	if(m_wDirection == POINTERR_BACKWARD)
	{
		fValue = m_fMax - fValue;
		fStartAngle = m_fEndAngle;
	}
	else
	{
		fStartAngle = m_fStartAngle;
	}
	
	fAngle = (fValue - m_fMin) / m_fUnit;
	
	fAngle += fStartAngle;
	
	if(fAngle >= 360) fAngle = fAngle - 360;
	
	return fAngle;
}


tmPoint	CtmMeter::GetPointCoordinate(float fAngle, tmPoint point, tmPoint pointORG, int nPointerStartY)
{
	float	fAngleTemp = 0;
	tmPoint	pointCoordinate;
	memset(&pointCoordinate, 0, sizeof(pointCoordinate));
	
	tmPoint	pointTemp;
	int		nLength = 0;
	//printf("pointORG x = %d, y = %d\n", pointORG.ix, pointORG.iy);
	//printf("point x = %d, y = %d\n", point.ix, point.iy);
	
	if(point.ix <= pointORG.ix)
	{
		nLength = pointORG.ix - point.ix;
	}
	else
	{
		nLength = point.ix - pointORG.ix;
	}
	
	if(fAngle >= 90 && fAngle < 180)
	{
		fAngleTemp = 180 - fAngle;
	}
	else if(fAngle >= 180 && fAngle < 270)
	{
		fAngleTemp = fAngle - 180;
	}
	else if(fAngle >= 270 && fAngle < 360)
	{
		fAngleTemp = 360 - fAngle;
	}
	else
	{
		fAngleTemp = fAngle;
	}
		
	pointTemp.ix = (float)nLength*cos(fAngleTemp*PI/180)+0.5;
	pointTemp.iy = (float)nLength*sin(fAngleTemp*PI/180)+0.5;
	
	tmPoint	pointOffset1;
	tmPoint	pointOffset2;
	pointOffset1.ix = (float)(point.iy - nPointerStartY)*cos((90-fAngleTemp)*PI/180)+0.5;
	pointOffset1.iy = (float)(point.iy - nPointerStartY)*sin((90-fAngleTemp)*PI/180)+0.5;
	
	pointOffset2.ix = (float)(pointORG.iy - nPointerStartY)*cos((90-fAngleTemp)*PI/180)+0.5;
	pointOffset2.iy = (float)(pointORG.iy - nPointerStartY)*sin((90-fAngleTemp)*PI/180)+0.5;
	
	
	//printf("pointTemp.ix = %d, y = %d, fAngleTemp = %f\n", pointTemp.ix, pointTemp.iy, fAngleTemp);
	//printf("pointOffset1.ix = %d, y = %d, fAngleTemp = %f\n", pointOffset1.ix, pointOffset1.iy, fAngleTemp);
	if(point.ix <= pointORG.ix)
	{
		if(fAngle >= 90 && fAngle < 180)
		{
			pointCoordinate.ix = pointORG.ix + pointTemp.ix - pointOffset1.ix + pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy - pointTemp.iy - pointOffset1.iy + pointOffset2.iy;
		}
		else if(fAngle >= 180 && fAngle < 270)
		{
			pointCoordinate.ix = pointORG.ix + pointTemp.ix + pointOffset1.ix - pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy + pointTemp.iy - pointOffset1.iy + pointOffset2.iy;
		}
		else if(fAngle >= 270 && fAngle < 360)
		{
			pointCoordinate.ix = pointORG.ix - pointTemp.ix + pointOffset1.ix - pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy + pointTemp.iy + pointOffset1.iy - pointOffset2.iy;
		}
		else
		{
			pointCoordinate.ix = pointORG.ix - pointTemp.ix - pointOffset1.ix + pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy - pointTemp.iy + pointOffset1.iy - pointOffset2.iy;
		}
	}
	else
	{
		if(fAngle >= 90 && fAngle < 180)
		{
			pointCoordinate.ix = pointORG.ix - pointTemp.ix - pointOffset1.ix + pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy + pointTemp.iy - pointOffset1.iy + pointOffset2.iy;
		}                                                                                       
		else if(fAngle >= 180 && fAngle < 270)                                                  
		{                                                                                       
			pointCoordinate.ix = pointORG.ix - pointTemp.ix + pointOffset1.ix - pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy - pointTemp.iy - pointOffset1.iy + pointOffset2.iy;
		}                                                                                       
		else if(fAngle >= 270 && fAngle < 360)                                                  
		{                                                                                       
			pointCoordinate.ix = pointORG.ix + pointTemp.ix + pointOffset1.ix - pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy - pointTemp.iy + pointOffset1.iy - pointOffset2.iy;
		}                                                                                       
		else                                                                                    
		{                                                                                       
			pointCoordinate.ix = pointORG.ix + pointTemp.ix - pointOffset1.ix + pointOffset2.ix;
			pointCoordinate.iy = pointORG.iy + pointTemp.iy + pointOffset1.iy - pointOffset2.iy;
		}
	}
	
	 
	//printf("pointCoordinate.ix = %d, y = %d\n", pointCoordinate.ix, pointCoordinate.iy);
	return pointCoordinate;
}


void	CtmMeter::InitPointerFlag()
{
	if (m_pPointerCoordinateFlag != NULL)
	{
		for (int i = 0; i < m_wViewHeight; i++)
		{
			memset(*(m_pPointerCoordinateFlag + i), 0, m_wViewWidth*sizeof(BOOL));
		}
	}
}
void	CtmMeter::OptimizePoint()
{
	#ifdef	D_32BIT
	DWORD**	pTemp = NULL;
	#else
	WORD**	pTemp = NULL;
	#endif
	
	if (m_pDoubleBuffer != NULL)
	{
		pTemp = m_pDoubleBuffer->GetBuffer();
	}
	if(pTemp == NULL) return;
	
	BYTE	r, g, b, r1, g1, b1 = 0;
	
	#ifdef	D_32BIT
	DWORD	Color1, Color2;
	#else
	WORD	Color1, Color2;
	#endif
	
	
	int i,j;
	
	int 	nHeight = m_wViewHeight - 1;
	int		nWidth = m_wViewWidth - 2;
	for(i = 1; i < nHeight; i++)
	{
		for(j = 1; j < nWidth; j++)
		{
			if(m_pPointerCoordinateFlag[i][j] && !m_pPointerCoordinateFlag[i][j+1])
			{
				//pTemp[j+1][i] = pTemp[j][i];
				//pTemp[j+1][i] = (pTemp[j][i] + pTemp[j+2][i] + pTemp[j+1][i-1] + pTemp[j+1][i+1])/4;
				
				//printf("m_pPointerCoordinateFlag [%d][%d] = %d\n", i, j, m_pPointerCoordinateFlag[i][j]);
				_ColorToRGB(pTemp[i][j], &r, &g, &b);	
				_ColorToRGB(pTemp[i][j+2], &r1, &g1, &b1);
				
				Color1 = RGB2PIXEL888(r1 - (r1 - r) / 2, 
									g1 - (g1 - g) / 2, 
									b1 - (b1 - b) / 2);
				
				_ColorToRGB(pTemp[i-1][j+1], &r, &g, &b);	
				_ColorToRGB(pTemp[i+1][j+1], &r1, &g1, &b1);
				
				Color2 = RGB2PIXEL888(r1 - (r1 - r) / 2, 
									g1 - (g1 - g) / 2, 
									b1 - (b1 - b) / 2);
									
				_ColorToRGB(Color1, &r, &g, &b);	
				_ColorToRGB(Color2, &r1, &g1, &b1);
				
				pTemp[i][j+1] = RGB2PIXEL888(r1 - (r1 - r) / 2, 
									g1 - (g1 - g) / 2, 
									b1 - (b1 - b) / 2);
			}
		}
	}
	
}

