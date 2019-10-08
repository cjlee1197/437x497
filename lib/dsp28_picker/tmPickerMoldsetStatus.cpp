#include	"../../tmdebug.h"
#include	"tmPickerMoldsetStatus.h"


IMPLEMENT_DYNCREATE(CtmPickerMoldsetStatus, CtmDsp28Protocol)

CtmPickerMoldsetStatus::CtmPickerMoldsetStatus(): CtmDsp28Protocol()
{
	
}

CtmPickerMoldsetStatus::~CtmPickerMoldsetStatus()
{
	
}


int		CtmPickerMoldsetStatus::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 8;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmPickerMoldsetStatus::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
	
	
	g_tmDebugInfo->LogData("LineStatus.DSP28", pOutData, nResult);
	return nResult;
}



#ifndef	D_BEOBJECT_CTMPICKERMOLDSETSTATUS	//	defineName=D_BEOBJECT_+CASE(ClassName) 
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 ®æ§ÓCreate 2¶∏
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
