#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"tmDsp28Moldset.h"


IMPLEMENT_DYNCREATE(CtmDsp28Moldset, CtmDsp28Protocol)
//IMPLEMENT_DYNCREATE(CtmDsp28CommonDiagWrite, CtmDsp28Moldset)

CtmDsp28Moldset::CtmDsp28Moldset(): CtmDsp28Protocol()
{
	
}

CtmDsp28Moldset::~CtmDsp28Moldset()
{
	
}


int		CtmDsp28Moldset::CreateSelf()
{
	CtmProtocol::CreateSelf();
	
	m_nSendType	= 3;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmDsp28Moldset::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
	
	wTemp	= 0;
	for (int i = 0; i < nInputLen; i++)
	{
		wID	=g_pDatabase->DataIDToIndex(plID[i]);
		db = g_pDatabase->Read(wID);
		
		if (db.dwState == DB_SUCCESS)
		{
			*(WORD*)pTemp	= plID[i];			
			pTemp			= (BYTE*)((BYTE*)pTemp + 2);
				
			*(BYTE*)pTemp	= db.DataType.wType;			
			
			pTemp			= (BYTE*)((BYTE*)pTemp + 2);
			
			if (db.DataType.wLength <= 4)	memcpy(pTemp, db.acData, db.DataType.wLength);
			else							memcpy(pTemp, db.acData, 4);
			
			pTemp		= (BYTE*)((BYTE*)pTemp + 4);	
			nResult 	+= 8;
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

int		CtmDsp28Moldset::ParseProtocol	(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;
		
	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	int		nResult	= 0;
	WORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	void*	pData 	= (BYTE*)pInData + m_RevHeader.GetSize();
	
	for (int i = 0; i < wCount; i++)
	{
		ID				= *(WORD*)pData;
		
		pData 			= (BYTE*)pData + sizeof(ID);	
		
		Type			= *(BYTE*)pData;
		
		pData			= (BYTE*)pData + sizeof(Type);
		State			= *(BYTE*)pData;
		
		pData			= (BYTE*)pData + sizeof(State);
		Value			= *(long*)pData;
		
		pData			= (BYTE*)pData + sizeof(Value);
		
		if (State == 0)	//	it's ok
			nResult++;
	}
	
	return nResult;
}


#ifndef	D_BEOBJECT_CTMDSP28MOLDSET
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
