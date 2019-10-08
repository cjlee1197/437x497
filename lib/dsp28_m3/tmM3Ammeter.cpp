#include	"../../tmdebug.h"
#include	"../../package.h"
#include	"../../database.h"
#include	"../libComm.h"
#include	"../../tmshmsg.h"
#include	"../../taskammeter.h"
#include	"tmM3Ammeter.h"


IMPLEMENT_DYNCREATE(CtmM3Ammeter, CtmDsp28Protocol)

CtmM3Ammeter::CtmM3Ammeter(): CtmDsp28Protocol()
{
	//m_pCommonRead	= NULL;
}

CtmM3Ammeter::~CtmM3Ammeter()
{
	
}


int		CtmM3Ammeter::CreateSelf()
{
	CtmProtocol::CreateSelf();
	//m_pCommonRead = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_ReadMotor"));
		
	m_nSendType	= 15;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}

int		CtmM3Ammeter::FreeSelf()
{
	CtmProtocol::FreeSelf();
	
	
	
	return 0;
	
}


int		CtmM3Ammeter::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
		wTemp	= 1;		
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));		
		
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());		
			
	}
	else
	{
		
		memcpy(((BYTE*)pOutData) + nResult, pInData, nInputLen);
		nResult += nInputLen;
		
		memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());
		
		//	set datasize
		nResult = nResult + m_SendFooter.GetSize();		
		wTemp	= nResult;
		m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
		
		//	set element num
		wTemp	= 1;
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		//	set data counter
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	}
	return nResult;
}

int		CtmM3Ammeter::ParseProtocol(void* pInData,		int nInputLen)
{
	
	int	nResult = 0;
	
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	if(g_ptaskAmmeter != NULL)
		g_ptaskAmmeter->ParseData((BYTE*)pInData +  m_RevHeader.GetSize(), nInputLen - m_RevHeader.GetSize() - m_RevFooter.GetSize());
	
	return nResult;
}

#ifndef	D_BEOBJECT_CTMN2CAMMETER
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;
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
