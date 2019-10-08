#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"../libComm.h"
#include	"../../tmshmsg.h"
#include	"tmM3SiemWrite.h"


IMPLEMENT_DYNCREATE(CtmM3SiemWrite, CtmDsp28Protocol)

CtmM3SiemWrite::CtmM3SiemWrite(): CtmDsp28Protocol()
{
	
}

CtmM3SiemWrite::~CtmM3SiemWrite()
{
	
}


int		CtmM3SiemWrite::CreateSelf()
{
	CtmProtocol::CreateSelf();
	
	m_nSendType	= 14;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3SiemWrite::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	if (pInData == NULL)		return 0;
	int		nResult = 0;
	WORD	wTemp	= 0;
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());

	void*	pTemp 	= (BYTE*)pOutData + nResult;
	long*	plID	= (long*)pInData;
	
	WORD	wReserve = 0;
	BYTE	Type	= nInputLen;
	BYTE	State	= 1;
	
	long*	pValue	= (long*)pEx;
	
	memcpy((BYTE*)pTemp, plID, sizeof(long));		
	pTemp = (BYTE*)pTemp + sizeof(long);
	nResult += sizeof(long);
	
	memcpy((BYTE*)pTemp, &wReserve, sizeof(wReserve));		
	pTemp = (BYTE*)pTemp + sizeof(wReserve);
	nResult += sizeof(wReserve);
	
	memcpy((BYTE*)pTemp, &Type, sizeof(Type));		
	pTemp = (BYTE*)pTemp + sizeof(Type);
	nResult += sizeof(Type);
	
	memcpy((BYTE*)pTemp, &State, sizeof(State));		
	pTemp = (BYTE*)pTemp + sizeof(State);
	nResult += sizeof(State);
	
	memcpy((BYTE*)pTemp, pValue, sizeof(long));		
	pTemp =  (BYTE*)pTemp + sizeof(long);
	nResult += sizeof(long);
	
	wTemp	= 1;
	
	memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());	
	nResult	= nResult + m_SendFooter.GetSize();
	
	//============
	//	set the header
	//============
	m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
	
	wTemp	= m_nCounter;		
	m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));
	
	wTemp	= nResult;
	m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
	
	
	memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	return	nResult;
}

int		CtmM3SiemWrite::ParseProtocol	(void* pInData,		int nInputLen)
{
	int nResult = 0;
	//if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;
	//	
	//memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	//
	//WORD wCount = 0;
	//m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	//
	//nResult = SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
	//
	//SendMsg(MSG_SH_AUSTONE_COMMANDWRITE, wCount, 0, NULL);
	return nResult;
}

int		CtmM3SiemWrite::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	if (nCount <= 0)	return 0;
	
	WORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value = 0;
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{
		ID				= *(WORD*)pData;
		
		pData 			= (BYTE*)pData + sizeof(ID);	
		
		Type			= *(BYTE*)pData;
		
		pData			= (BYTE*)pData + sizeof(Type);
		State			= *(BYTE*)pData;
		
		pData			= (BYTE*)pData + sizeof(State);
		memcpy(&Value, pData, sizeof(Value));	//James modify 2010/5/28 支持float類型
		//Value = *(long*)pData;
		
		//memset(&dbValue, 0, sizeof(DBVALUE));
		//memcpy(dbValue, &g_pDatabase->Read(g_pDatabase->DataIDToIndex(ID)), sizeof(DBVALUE));
		pData			= (BYTE*)pData + sizeof(Value);
		//printf("ID=%x State=%d Type=%d Value=%d DBID=%s \n", ID, State, Type, Value, g_pDatabase->GetString(g_pDatabase->DataIDToIndex(ID)));
		if 	(State == 1 && ID >= AUSTONE_MINDATAID)
		{
			if(DB_SUCCESS == g_pDatabase->Write(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)
			{
				nResult++;
			}
			//printf("GetValue=%ld \n", GetDBValue(g_pDatabase->GetString(g_pDatabase->DataIDToIndex(ID))).lValue);
		}
	}
	
	return nResult;
}

#ifndef	D_BEOBJECT_CTMM3SIEMWRITE
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
