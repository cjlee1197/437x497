#include	"../../tmdebug.h"
#include	"../../package.h"
#include	"../../database.h"
#include	"tmDsp28ActualValue.h"


IMPLEMENT_DYNCREATE(CtmDsp28ActualValue, CtmDsp28Protocol)

CtmDsp28ActualValue::CtmDsp28ActualValue(): CtmDsp28Protocol()
{
	//m_pCommonRead	= NULL;
}

CtmDsp28ActualValue::~CtmDsp28ActualValue()
{
	
}


int		CtmDsp28ActualValue::CreateSelf()
{
	CtmProtocol::CreateSelf();
	//m_pCommonRead = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_ReadMotor"));
		
	m_nSendType	= 1;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	//if (m_pCommonRead != NULL)
	//{
	//	m_pCommonRead->SetSendType(m_nSendType);
	//	m_pCommonRead->SetRevType(m_nRevType);
	//}
	
	return 0;
}

int		CtmDsp28ActualValue::FreeSelf()
{
	CtmProtocol::FreeSelf();
	
	//g_pPackage->ReleaseClassInstance("HostProtocol_ReadMotor", m_pCommonRead);
	
	return 0;
	
}


int		CtmDsp28ActualValue::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int		nResult = 0;
	if (pOutData == NULL)	return nResult;
	
	WORD wTemp 	= 0;
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos, &wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	
	if (pInData == NULL)
	{
		memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());	
		
		
		nResult = nResult + m_SendFooter.GetSize();	
		wTemp	= nResult;
		
		m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
		
		//	set element num
		wTemp	= nInputLen;		//James add 2008/5/7 03:08下午
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));		
		
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());		
			
	}
	else
	{
		WORD*	pwID = (WORD*)((BYTE*)pOutData + nResult);
		long*	plID = (long*)pInData;

		for (int i = 0; i < nInputLen; i++)
		{
			*pwID = *(long*)plID;
			pwID++;
			plID++;
			nResult = nResult + 2;
			
		}
		memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());
		
		
		//	set datasize
		nResult = nResult + m_SendFooter.GetSize();		
		wTemp	= nResult;
		m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
		
		//	set element num
		wTemp	= nInputLen;
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		//	set data counter
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	}
	return nResult;
}

int		CtmDsp28ActualValue::ParseProtocol	(void* pInData,		int nInputLen)
{
//	int	nResult = m_pCommonRead->ParseProtocol(pInData, nInputLen);
	
	//======================
	//	the append action
	//======================
	//James modify 2008/8/21 from Zhouly
	//回傳實際壓力和流量值
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));

	return SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
//	long Value337 = (short int)GetDBValue("MACH_OTHR_OTHR_NULL_W0337").lValue;
//	long Value339 = (short int)GetDBValue("MACH_OTHR_OTHR_NULL_W0339").lValue;
//	long Value338 = GetDBValue("MACH_OTHR_OTHR_NULL_W0338").lValue;
//	
//	
//	float	fTemp = ((Value337 - Value339)  * 250.0) / 32767;
//	SetDBValue("SYSX_OTHERS_OTHERS_NULL_DSP28REALPRESSURE", 
//				(long)(round(fTemp)), FALSE);
//	
//	fTemp = (Value338 * 999.0) / 32767;
//	SetDBValue("SYSX_OTHERS_OTHERS_NULL_DSP28REALFLOW", (long)(round(fTemp)), FALSE);
	
//	return nResult;
}


#ifndef	D_BEOBJECT_CTMDSP28ACTUALVALUE
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 防止Create 2次
	if(pResult == NULL)
	{
		pResult = Create(Name);
		if (pResult != NULL)
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
