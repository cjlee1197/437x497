#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"../libComm.h"
#include	"../../tmshmsg.h"
#include	"tmM3CommonWrite.h"


IMPLEMENT_DYNCREATE(CtmM3CommonWrite, CtmDsp28Protocol)
IMPLEMENT_DYNCREATE(CtmM3CommonDiagWrite, CtmDsp28Protocol)

CtmM3CommonWrite::CtmM3CommonWrite(): CtmDsp28Protocol()
{
	
}

CtmM3CommonWrite::~CtmM3CommonWrite()
{
	
}


int		CtmM3CommonWrite::CreateSelf()
{
	CtmProtocol::CreateSelf();
	
	m_nSendType	= 3;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3CommonWrite::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	//printf("nInputLen=%d \n", nInputLen);
	if (nInputLen == 0 || pInData == NULL)		return 0;
		
	int		nResult = 0;
	DBVALUE	db;
	
	
	
	WORD	wID 	= 0;
	WORD	wTemp	= 0;
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());

	void*	pTemp 	= (BYTE*)pOutData + nResult;
	long*	plID	= (long*)pInData;
	long	lTempID = 0;
	
	wTemp	= 0;
	for (int i = 0; i < nInputLen; i++)
	{
		wID	=g_pDatabase->DataIDToIndex(plID[i]);
		memset(&db, 0, sizeof(db));
		db = g_pDatabase->Read(wID);
		
		if (db.dwState == DB_SUCCESS)
		{
			lTempID = plID[i];
			lTempID -= 0x10000000;
			//printf("CtmM3CommonWrite::ComposeProtocol  plID = %x\n", plID[i]);
			memcpy(pTemp, &lTempID, sizeof(lTempID));	
			
			pTemp			= ((BYTE*)pTemp + sizeof(DWORD));
							
			memcpy(pTemp, &db.DataType.wType, sizeof(WORD));
			
			pTemp			= ((BYTE*)pTemp + sizeof(WORD));
			
			//printf("db.DataType.wLength=%d \n", db.DataType.wLength);
			if (db.DataType.wLength <= 4)	memcpy(pTemp, db.acData, db.DataType.wLength);
			else							memcpy(pTemp, db.acData, 4);
			
			pTemp		= ((BYTE*)pTemp + sizeof(DWORD));	
			nResult 	+= 10;
			wTemp++;
		}
	}
	
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

int		CtmM3CommonWrite::ParseProtocol	(void* pInData,		int nInputLen)
{
	int nResult = 0;
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;
		
	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	long	dwID = 0;
	float fValue = 0;
	
	memcpy(&dwID, (BYTE*)pInData + m_RevHeader.GetSize(), sizeof(dwID));
	memcpy(&fValue, (BYTE*)pInData + m_RevHeader.GetSize()+6, sizeof(fValue));
	
	
	//printf("28 back dwID=%x fValue=%f \n", dwID, fValue);
//	nResult = SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
	
	SendMsg(MSG_SH_AUSTONE_COMMANDWRITE, wCount, 0, NULL);
	return nResult;
}

int		CtmM3CommonWrite::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	if (nCount <= 0)	return 0;
	
	DWORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value = 0;
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{
		memcpy(&ID, pData, sizeof(ID));		
		//printf("CtmM3CommonWrite::SetDataToDataBase  plID = %d\n", ID);
		pData 			= (BYTE*)pData + sizeof(ID);	
		ID				= ID + 0x10000000;
		
		Type			= *(BYTE*)pData;
		
		memcpy(&Type, pData, sizeof(Type));
		pData			= (BYTE*)pData + sizeof(Type);
		
		memcpy(&State, pData, sizeof(State));
		pData			= (BYTE*)pData + sizeof(State);
		
		memcpy(&Value, pData, sizeof(Value));	//James modify 2010/5/28 支持float類型
		pData			= (BYTE*)pData + sizeof(Value);
		
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

CtmM3CommonDiagWrite::CtmM3CommonDiagWrite(): CtmM3CommonWrite()
{
	
}

CtmM3CommonDiagWrite::~CtmM3CommonDiagWrite()
{
	
}

int		CtmM3CommonDiagWrite::CreateSelf()
{
	CtmProtocol::CreateSelf();
	
	m_nSendType	= 3 | 0x8000;
	m_nRevType	= (m_nSendType + 1000) | 0x8000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;	
}

int	CtmM3CommonDiagWrite::ComposeProtocol(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	if (nInputLen == 0 || pInData == NULL)		return 0;
		
	int nSendType 	= m_nSendType;
	m_nSendType		= m_nSendType & 0x7FFF;
	
	int		nResult = 0;
	
	DBVALUE	db;

	
	WORD	wID 	= 0;
	WORD	wTemp	= 0;
	m_nCounter++;
	wTemp = 0;
	
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());
	
	
	void*	pTemp 	= (BYTE*)pOutData + nResult;
	long*	plID	= (long*)pInData;
	long*	pValue	= (long*)pEx;
	
	wTemp	= 0;
	for (int i = 0; i < nInputLen; i++)
	{
		wID	=g_pDatabase->DataIDToIndex(plID[i]);
		memset(&db, 0, sizeof(db));
		db = g_pDatabase->Read(wID);
		
		if (db.dwState == DB_SUCCESS)
		{
			plID[i] -= 0x10000000;
			//printf("CtmM3CommonWrite::ComposeProtocol  plID = %x\n", plID[i]);
			memcpy(pTemp, &plID[i], sizeof(plID[i]));	
			
			pTemp			= ((BYTE*)pTemp + sizeof(DWORD));
							
			memcpy(pTemp, &db.DataType.wType, sizeof(WORD));
			
			pTemp			= ((BYTE*)pTemp + sizeof(WORD));
			
			memcpy(pTemp, &pValue[i], 4);
			
			pTemp		= ((BYTE*)pTemp + sizeof(DWORD));	
			nResult 	+= 10;
			wTemp++;
		}
	}
	
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
	
	wTemp	= m_nSendType;		
	m_SendHeader.SetPartValue(m_nTypePos, 	&wTemp, sizeof(wTemp));
	
	
	memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());	
	
	
	m_nSendType  	= nSendType;
	return	nResult;
	
}


#ifndef	D_BEOBJECT_CTMM3COMMONWRITE
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
