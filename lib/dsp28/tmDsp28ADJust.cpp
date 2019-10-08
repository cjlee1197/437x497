#include	"../../tmdebug.h"
#include	"tmDsp28ADJust.h"


IMPLEMENT_DYNCREATE(CtmDsp28ADJust, CtmProtocol)

CtmDsp28ADJust::CtmDsp28ADJust(): CtmProtocol()
{
	
}

CtmDsp28ADJust::~CtmDsp28ADJust()
{
	
}


int		CtmDsp28ADJust::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 2;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmDsp28ADJust::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int		nResult = 0;
	if (pOutData == NULL)	return nResult;
	
	WORD wTemp 	= 0;
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	
	if (pInData == NULL)
	{
		memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());	
		
		
		nResult = nResult + m_SendFooter.GetSize();	
		wTemp	= nResult;
		
		m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
		
		//	set element num
		wTemp	= nInputLen;		//James add 2008/5/7 03:08¤U¤È
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

int		CtmDsp28ADJust::ParseProtocol(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	return SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
}


#ifndef	D_BEOBJECT_CTMDSP28ADJUST
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
