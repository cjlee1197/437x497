#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"tmM3Moldset.h"


IMPLEMENT_DYNCREATE(CtmM3Moldset, CtmDsp28Protocol)
//IMPLEMENT_DYNCREATE(CtmN2CCommonDiagWrite, CtmDsp28Protocol)

CtmM3Moldset::CtmM3Moldset(): CtmDsp28Protocol()
{
	//add by J.Wong 2016/8/23 16:14:01
	m_nCounter=1;
}

CtmM3Moldset::~CtmM3Moldset()
{
	
}


int		CtmM3Moldset::CreateSelf()
{
	CtmProtocol::CreateSelf();
	
	m_nSendType	= 9;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3Moldset::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	//printf("nInputLen=%d \n", nInputLen);
	if (nInputLen == 0 || pInData == NULL)
	{
		m_nCounter = 1;
		return 0;
	}
		
	int		nResult = 0;
	DBVALUE	db;
	
	
	
	WORD	wID 	= 0;
	WORD	wTemp	= 0;
	int		nIndex = 0;
//	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());

	void*	pTemp 	= (BYTE*)pOutData + nResult;
	long*	plID	= (long*)pInData;
	long	lTempID = 0;
	
	wTemp	= 0;
	//printf("Write InputLen=%d m_nCounter=%d \n", nInputLen, m_nCounter);
	for (int i = 0; i < nInputLen; i++)
	{
		lTempID = plID[i];
		nIndex	=g_pDatabase->DataIDToIndex(plID[i]);
		memset(&db, 0, sizeof(db));
		if(plID[i] >= 0x10000000)
		{
			db = g_pDatabase->Read(nIndex);
		
			if (db.dwState == DB_SUCCESS)
			{
				//printf("CtmM3Moldset::ComposeProtocol  plID = %x\n", plID[i]);
				lTempID -= 0x10000000;
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
		else
		{
			;//printf("================Error ID plID[i]=%ld \n", plID[i]);	
		}
	}
	
	memcpy(((BYTE*)pOutData) + nResult, m_SendFooter.GetBuffer(), m_SendFooter.GetSize());	
	nResult	= nResult + m_SendFooter.GetSize();
	
	//============
	//	set the header
	//============
	//printf("set Element=%d \n", wTemp);
	m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
	
	wTemp	= m_nCounter;		
	m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));
	
	wTemp	= nResult;
	m_SendHeader.SetPartValue(m_nDataSizePos,	&wTemp, sizeof(wTemp));
	
	
	memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	return	nResult;
}

int		CtmM3Moldset::ParseProtocol	(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;
		
	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	WORD wPackCount=0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	m_RevHeader.GetPartValue(m_nDataCountPos, &wPackCount, sizeof(wPackCount));
	
	//modify by J.Wong 2016/8/26 16:45:25
	if(m_nCounter<=wPackCount)
		m_nCounter=wPackCount+1;
	int		nResult	= 0;
	DWORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	void*	pData 	= (BYTE*)pInData + m_RevHeader.GetSize();
//	
//	for (int i = 0; i < wCount; i++)
//	{
//		memcpy(&ID, pData, sizeof(ID));		
//		//printf("CtmM3CommonWrite::SetDataToDataBase  plID = %d\n", ID);
//		pData 			= (BYTE*)pData + sizeof(ID);	
//		ID				= ID + 0x10000000;
//		
//		Type			= *(BYTE*)pData;
//		
//		memcpy(&Type, pData, sizeof(Type));
//		pData			= (BYTE*)pData + sizeof(Type);
//		
//		memcpy(&State, pData, sizeof(State));
//		pData			= (BYTE*)pData + sizeof(State);
//		
//		memcpy(&Value, pData, sizeof(Value));	//James modify 2010/5/28 支持float類型
//		pData			= (BYTE*)pData + sizeof(Value);
//		
//		if (State == 0)	//	it's ok
//			nResult++;
//	}
	
	return nResult;
}


#ifndef	D_BEOBJECT_CTMM3MOLDSET
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
