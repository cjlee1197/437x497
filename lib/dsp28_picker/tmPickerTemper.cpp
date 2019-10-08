#include	"../../tmdebug.h"
#include	"tmPickerTemper.h"


IMPLEMENT_DYNCREATE(CtmPickerTemper, CtmProtocol)

CtmPickerTemper::CtmPickerTemper(): CtmProtocol()
{
	
}

CtmPickerTemper::~CtmPickerTemper()
{
	
}


int		CtmPickerTemper::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 10;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmPickerTemper::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int		nResult = 0;
	if (pOutData == NULL)	return nResult;
	
	WORD wTemp 	= 0;
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	//printf("wTemp=%d \n", wTemp);
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	
	if (pInData == NULL)
	{
		memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());	
		
		
		nResult = nResult + m_SendFooter.GetSize();	
		wTemp	= nResult;
		
		m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
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

int		CtmPickerTemper::ParseProtocol	(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	short	Value[10]	= {0};
	int		nResult	= 0;
	BYTE*	pData = NULL;
	pData = (BYTE*)pInData + m_RevHeader.GetSize();
	//printf("wCount=%d \n", wCount);
	if(wCount > 10)
		g_tmDebugInfo->LogDebugInfo("/usr/i86/Temper.log", "Temper count=%d", wCount);
	for (int i = 0; i < wCount; i++)
	{
		Value[i]		= *(short*)pData;
		pData			= (BYTE*)pData + sizeof(Value[0]);
		
		//value為接收到 dsp28傳回的key值，如果需要處理，在這里就可以
		//printf("Value=%d \n", Value);
		nResult++;
	}
	g_tmDebugInfo->LogDebugInfo("/usr/i86/Temper.log","1=%d,2=%d,3=%d,4=%d,5=%d",
		Value[0],Value[1],Value[2],Value[3],Value[4]);
	
	return nResult;//SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
}


#ifndef	D_BEOBJECT_CTMPICKERTEMPER
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
