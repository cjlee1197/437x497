#include	"../../tmdebug.h"
#include	"tmM3MoldsetStatus.h"


IMPLEMENT_DYNCREATE(CtmM3MoldsetStatus, CtmDsp28Protocol)

CtmM3MoldsetStatus::CtmM3MoldsetStatus(): CtmDsp28Protocol()
{
	
}

CtmM3MoldsetStatus::~CtmM3MoldsetStatus()
{
	
}


int		CtmM3MoldsetStatus::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 8;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3MoldsetStatus::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int		nResult = 0;
	if (pOutData == NULL || nInputLen != 1)	return nResult;
	
	WORD wTemp 	= 0;
	
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	
	
	long*	plID = (long*)((BYTE*)pOutData + nResult);
	
	*plID 	= *(long*)pInData;
	
	nResult = nResult + sizeof(long); 
	
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
	
	
	g_tmDebugInfo->LogData("LineStatus.N2C", pOutData, nResult);
	return nResult;
}


int		CtmM3MoldsetStatus::ParseProtocol	(void* pInData,		int nInputLen)
{
	int		nResult = 0;
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	if (wCount != 1) return -1;
		
	long	Value	= 0;
	BYTE*	pData = NULL;
	
	pData = (BYTE*)pInData + m_RevHeader.GetSize();
	
	memcpy(&Value, pData, sizeof(pData));
	pData = (BYTE*)pData + sizeof(Value);
	
	printf("CtmM3MoldsetStatus :: Value = %d\n", Value);
	return Value;
}



#ifndef	D_BEOBJECT_CTMM3MOLDSETSTATUS	//	defineName=D_BEOBJECT_+CASE(ClassName) 
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 ¨¾¤îCreate 2¦¸
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
