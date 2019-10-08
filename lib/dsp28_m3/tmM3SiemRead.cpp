#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"../../main.h"	//	g_pApplication
#include	"../../tmshmsg.h"
#include	"../libComm.h"
#include	"tmM3SiemRead.h"



IMPLEMENT_DYNCREATE(CtmM3SiemRead, CtmDsp28Protocol)
//long lSaveSramID[] = 
//{
//	0x034A,
//	0x034B,	
//	0x034C,	
//	0x034D
//};

CtmM3SiemRead::CtmM3SiemRead(): CtmDsp28Protocol()
{
	
}

CtmM3SiemRead::~CtmM3SiemRead()
{
	
}


int		CtmM3SiemRead::CreateSelf()
{
	CtmProtocol::CreateSelf();
	
	memset(m_nAryDiagValues, 0, sizeof(m_nAryDiagValues));	
	m_nSendType	= 13;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3SiemRead::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
		wTemp	= nInputLen;		//James add 2008/5/7 03:08§U§»
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));		
		
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());		
			
	}
	else
	{
		DWORD	dwID = *(long*)pInData;
		WORD	wReserve = 0;
		BYTE	Type	= nInputLen;
		BYTE	State	= 1;
		
		memcpy(((BYTE*)pOutData) + nResult, &dwID, sizeof(dwID));		
		nResult += sizeof(dwID);
			
		memcpy(((BYTE*)pOutData) + nResult, &wReserve, sizeof(wReserve));		
		nResult += sizeof(wReserve);
			
		memcpy(((BYTE*)pOutData) + nResult, &Type, sizeof(Type));	
		nResult += sizeof(Type);
		
		memcpy(((BYTE*)pOutData) + nResult, &State, sizeof(State));	
		nResult += sizeof(State);	
			
		memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());
		
		//	set datasize
		nResult = nResult + m_SendFooter.GetSize();		
		wTemp	= nResult;
		m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
		
		//	set element num
		wTemp	= 1;
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		//wTemp	= nInputLen;
		//m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		//	set data counter
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	}
	return nResult;
}

int		CtmM3SiemRead::ParseProtocol(void* pInData,		int nInputLen)
{
	int nResult = 0;
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	nResult = SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, TRUE);
	SendMsg(MSG_SH_AUSTONE_COMMANDREAD, wCount, 0, NULL);		//James add for update readdata 2010/6/27
	//printf("wCount=%d nInputLen=%d \n", wCount, nInputLen);
	return nResult;
}

int		CtmM3SiemRead::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	if (nCount <= 0)	return 0;
	

	DWORD	dwID 	= 0;
	WORD	wReserve= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	int		nResult	= 0;

	memcpy(&dwID, (BYTE*)pData + nResult, sizeof(dwID));
	nResult 		+= sizeof(dwID) ;
	
	memcpy(&wReserve, (BYTE*)pData + nResult, sizeof(wReserve));
	nResult 		+= sizeof(wReserve) ;
	
	memcpy(&Type, (BYTE*)pData + nResult, sizeof(Type));
	nResult 		+= sizeof(Type) ;
	
	memcpy(&State, (BYTE*)pData + nResult, sizeof(State));
	nResult 		+= sizeof(State) ;
	
	memcpy(&Value, (BYTE*)pData + nResult, sizeof(Value));	
	nResult 		+= sizeof(Value) ;
	
	memcpy(m_nAryDiagValues, pData ,nResult);
	
	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	msg.message	= MSG_DSP28_READ_DIAG;	
	msg.wParam	= CONST_RES_SIEMREAD_DIAG;	
	msg.lParam	= (long)m_nAryDiagValues;
	g_pApplication->QueueMessage(&msg);
	
	return nResult;
}

#ifndef	D_BEOBJECT_CTMM3SIEMREAD
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 ®æ§ÓCreate 2¶∏
	//if(pResult == NULL)
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
