#include	"../../tmdebug.h"
#include	"../../package.h"
#include	"../../database.h"
#include	"../libComm.h"
#include	"../../tmshmsg.h"
#include	"tmM3ActualValue.h"


IMPLEMENT_DYNCREATE(CtmM3ActualValue, CtmDsp28Protocol)

CtmM3ActualValue::CtmM3ActualValue(): CtmDsp28Protocol()
{
	//m_pCommonRead	= NULL;
}

CtmM3ActualValue::~CtmM3ActualValue()
{
	
}


int		CtmM3ActualValue::CreateSelf()
{
	CtmProtocol::CreateSelf();
	//m_pCommonRead = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_ReadMotor"));
		
	m_nSendType	= 1;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	//if (m_pCommonRead != NULL)
	//{
	//	m_pCommonRead->SetSendType(m_nSendType);
	//	m_pCommonRead->SetRevType(m_nRevType);
	//}
	
	return 0;
}

int		CtmM3ActualValue::FreeSelf()
{
	CtmProtocol::FreeSelf();
	
	//g_pPackage->ReleaseClassInstance("HostProtocol_ReadMotor", m_pCommonRead);
	
	return 0;
	
}


int		CtmM3ActualValue::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int		nResult = 0;
	if (pOutData == NULL)	return nResult;
	
	WORD wTemp 	= 0;
	m_nCounter++;
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos, &wTemp, sizeof(wTemp));		
	nResult = m_SendHeader.GetSize();
	
	//printf("CtmM3ActualValue nInputLen=%d \n", nInputLen);
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
		//printf("CtmM3ActualValue::ComposeProtocol  nInputLen = %d\n", nInputLen);
		for (int i = 0; i < nInputLen; i++)
		{
			lTempID = plID[i];
			lTempID -= 0x10000000;
			//printf("CtmM3ActualValue::ComposeProtocol  ID = %x\n", plID[i]);
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

int		CtmM3ActualValue::ParseProtocol	(void* pInData,		int nInputLen)
{
	int	nResult = 0;
//	int	nResult = m_pCommonRead->ParseProtocol(pInData, nInputLen);
	//======================
	//	the append action
	//======================
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	nResult = SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
	
	SendMsg(MSG_SH_AUSTONE_COMMANDACTUAL, wCount, 0, NULL);
	return nResult;
}

int		CtmM3ActualValue::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
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
		pData 			= (BYTE*)pData + sizeof(ID);	
		ID				= ID + 0x10000000;
		
		Type			= *(BYTE*)pData;
		
		memcpy(&Type, pData, sizeof(Type));
		pData			= (BYTE*)pData + sizeof(Type);
		
		memcpy(&State, pData, sizeof(State));
		pData			= (BYTE*)pData + sizeof(State);
		
		memcpy(&Value, pData, sizeof(Value));	//James modify 2010/5/28 支持float類型
		pData			= (BYTE*)pData + sizeof(Value);
		
		
		//printf("CtmM3ActualValue::SetDataToDataBase ID=%x , Type = %d, State=%d Value=%ld \n", ID, Type, State, Value);
		if 	(State == 1)
		{
			if(DB_SUCCESS == g_pDatabase->Write(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)
			{
				nResult++;
			}
		}
		//else if(State == 0)
		//{
		//	if(DB_SUCCESS == SetIDValue(ID, 0))
		//	{
		//		nResult++;
		//	}	
		//}
	}
	
	return nResult;
}


#ifndef	D_BEOBJECT_CTMM3ACTUALVALUE
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
