#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"../../main.h"	//	g_pApplication
#include	"../../tmshmsg.h"
#include	"../libComm.h"
#include	"tmM3CommonRead.h"
#include	"../../utils.h"



IMPLEMENT_DYNCREATE(CtmM3CommonRead, CtmDsp28Protocol)
IMPLEMENT_DYNCREATE(CtmM3CommonDiagRead, CtmDsp28Protocol)
//long lSaveSramID[] = 
//{
//	0x034A,
//	0x034B,	
//	0x034C,	
//	0x034D
//};

CtmM3CommonRead::CtmM3CommonRead(): CtmDsp28Protocol()
{
	
}

CtmM3CommonRead::~CtmM3CommonRead()
{
	
}


int		CtmM3CommonRead::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 2;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3CommonRead::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
		wTemp	= nInputLen;		//James add 2008/5/7 03:08下午
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));		
		
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());		
			
	}
	else
	{
		long*	plID = (long*)pInData;
		long	lTempID = 0;
    	
    	BYTE*   pTemp = (BYTE*)pOutData + nResult;
    	int		nOffset = 0;
		
		for (int i = 0; i < nInputLen; i++)
		{
			lTempID = plID[i];
			lTempID -= 0x10000000;
			memcpy(pTemp + nOffset, &lTempID, sizeof(DWORD));
			nResult = nResult + 4;
			nOffset = nOffset + 4;
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

int		CtmM3CommonRead::ParseProtocol(void* pInData,		int nInputLen)
{
	int nResult = 0;
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	//printf("CtmM3CommonRead::ParseProtocol\n");
	nResult = SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, TRUE);
	SendMsg(MSG_SH_AUSTONE_COMMANDREAD, wCount, 0, NULL);		//James add for update readdata 2010/6/27
	//printf("wCount=%d nInputLen=%d \n", wCount, nInputLen);
	return nResult;
}

int		CtmM3CommonRead::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	if (nCount <= 0)	return 0;
	

	DWORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{
		
		memcpy(&ID, pData, sizeof(ID));		
		pData 			= (BYTE*)pData + sizeof(ID);	
		ID				= ID + 0x10000000;
		
		Type			= *(BYTE*)pData;
		
		memcpy(&Type, pData, sizeof(Type));
		pData			= (BYTE*)pData + sizeof(Type);
		
		memcpy(&State, pData, sizeof(State));
		pData			= (BYTE*)pData + sizeof(State);
		
		memcpy(&Value, pData, sizeof(Value));	//James modify 2010/5/28 支持float類型
		pData			= (BYTE*)pData + sizeof(Value);
		
		//printf("CtmM3CommonRead::SetDataToDataBase  plID = %x Value=%d State=%d \n", ID, Value, State);
		if 	(State == 1)
		{
			if(DB_SUCCESS == g_pDatabase->Write(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)
			{
				nResult++;
			}
		}
	}
	
	return nResult;
}

CtmM3CommonDiagRead::CtmM3CommonDiagRead(): CtmM3CommonRead()
{
	
}


CtmM3CommonDiagRead::~CtmM3CommonDiagRead()
{
}


int		CtmM3CommonDiagRead::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	//printf("CtmM3CommonDiagRead::CreateSelf()\n");
	memset(m_nAryDiagValues, 0, sizeof(m_nAryDiagValues));	
	m_nSendType	= 0x8000 | 2;
	m_nRevType	= (m_nSendType + 1000) | 0x8000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}

int		CtmM3CommonDiagRead::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int nSendType 	= m_nSendType;
	int nResult		= 0;
	m_nSendType		= m_nSendType & 0x7FFF;
	WORD	wTemp	= m_nSendType;
		
	m_SendHeader.SetPartValue(m_nTypePos, 	&wTemp, sizeof(wTemp));
	nResult = CtmM3CommonRead::ComposeProtocol(pOutData, pInData, nInputLen, pEx);	
	
	m_nSendType  	= nSendType;
	return nResult;
}


int		CtmM3CommonDiagRead::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	if (nCount > 128) nCount = 128;
		
	if (nCount <= 0)	return 0;
	
	
	DWORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{
		memcpy(&ID, pData, sizeof(ID));		
		pData 			= (BYTE*)pData + sizeof(ID);	
		ID				= ID + 0x10000000;
		
		Type			= *(BYTE*)pData;
		
		memcpy(&Type, pData, sizeof(Type));
		pData			= (BYTE*)pData + sizeof(Type);
		
		memcpy(&State, pData, sizeof(State));
		pData			= (BYTE*)pData + sizeof(State);
		
		memcpy(&Value, pData, sizeof(Value));	//James modify 2010/5/28 支持float類型
		pData			= (BYTE*)pData + sizeof(Value);
		
		//前面是存ID，后面是存Value
		if( i > 128)
			i = 128;
		m_nAryDiagValues[i] = ID;
		if(nCount > 128)
			nCount = 128;
		memcpy(&m_nAryDiagValues[nCount+i], &Value, sizeof(Value));
		
		nResult++;
	}
	
	
	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	msg.message	= MSG_DSP28_READ_DIAG;	
	msg.wParam	= nResult;	
	msg.lParam	= (long)m_nAryDiagValues;
	g_pApplication->QueueMessage(&msg);
	
	return nResult;
}

#ifndef	D_BEOBJECT_CTMM3COMMONREAD
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 防止Create 2次
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
