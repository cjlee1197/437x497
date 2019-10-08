#include	"../../tmdebug.h"
#include	"../../package.h"
#include	"../../database.h"
#include	"tmPickerActualValue.h"


IMPLEMENT_DYNCREATE(CtmPickerActualValue, CtmDsp28Protocol)

CtmPickerActualValue::CtmPickerActualValue(): CtmDsp28Protocol()
{
	//m_pCommonRead	= NULL;
}

CtmPickerActualValue::~CtmPickerActualValue()
{
	
}


int		CtmPickerActualValue::CreateSelf()
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

int		CtmPickerActualValue::FreeSelf()
{
	CtmProtocol::FreeSelf();
	
	//g_pPackage->ReleaseClassInstance("HostProtocol_ReadMotor", m_pCommonRead);
	
	return 0;
	
}


int		CtmPickerActualValue::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
		wTemp	= nInputLen;		//James add 2008/5/7 03:08下午
		m_SendHeader.SetPartValue(m_nElementNumPos, &wTemp, sizeof(wTemp));
		
		wTemp	= m_nCounter;		
		m_SendHeader.SetPartValue(m_nDataCountPos,	&wTemp, sizeof(wTemp));		
		
		
		memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());		
			
	}
	else
	{
		/*
		WORD*	pwID = (WORD*)((BYTE*)pOutData + nResult);
		long*	plID = (long*)pInData;

		for (int i = 0; i < nInputLen; i++)
		{
			*pwID = *(long*)plID;
			pwID++;
			plID++;
	  	nResult = nResult + 2;
		}*/
	
		long*	plID = (long*)pInData;
    	
    BYTE*   pTemp = (BYTE*)pOutData + nResult;
    int		nOffset = 0;
		//printf("CtmPickerActualValue::ComposeProtocol  nInputLen = %d\n", nInputLen);
		for (int i = 0; i < nInputLen; i++)
		{
			//printf("CtmPickerActualValue::ComposeProtocol  ID = %x\n", plID[i]);
			memcpy(pTemp + nOffset, &plID[i], sizeof(DWORD));
			nResult = nResult + 4;
			nOffset = nOffset + 4;
		}		
		//20140415 add
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

int		CtmPickerActualValue::ParseProtocol	(void* pInData,		int nInputLen)
{
	
  //	int	nResult = m_pCommonRead->ParseProtocol(pInData, nInputLen);
	
	//======================
	//	the append action
	//======================
	//James modify 2008/8/21 from Zhouly
	//回傳實際壓力和流量值
	
	//printf("nInputLen = %d  ,header =%d\n",nInputLen, m_RevFooter.GetSize() + m_RevHeader.GetSize());
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));

  return SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
  
}

int		CtmPickerActualValue::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	
	if (nCount <= 0)	return 0;
	
	//WORD	ID 		= 0;
	long	ID = 0;
	
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{	
	//	ID				= *(WORD*)pData;
	  memset(&ID, 0 ,sizeof(ID));
	  memcpy(&ID, pData, sizeof(ID));
		
		pData 			= (BYTE*)pData + sizeof(ID);	
		
		//Type			= *(BYTE*)pData;
		memset(&Type, 0, sizeof(Type));
		memcpy(&Type, pData, sizeof(Type));
		
		pData			= (BYTE*)pData + sizeof(Type);
	//	State			= *(BYTE*)pData;
		memset(&State, 0, sizeof(State));
  	memcpy(&State, pData, sizeof(State));
		
		pData			= (BYTE*)pData + sizeof(State);
	//	Value			= *(long*)pData;
	  memset(&Value, 0, sizeof(Value));
		memcpy(&Value, pData, sizeof(Value));
		pData			= (BYTE*)pData + sizeof(Value);
		
		pData			= (BYTE*)pData + 10*sizeof(BYTE); // for 437x497 to 28335 parse // cjlee add 2019/1/24 下午 12:56:23 
		
		//	need to check the data's type and state 
		//if(ID == 0x800703)
		//if(ID == 0x800600 || ID == 0x800602)// || ID == 0x800305)
		//	printf("2 ID=%X Value=%d State=%d Type=%d \n", ID, Value, State, Type);	
		if(State == 1)
		{		
		  //	printf("ID = %x, Value = %d\n",ID,Value);
				//if(DB_SUCCESS == g_pDatabase->WriteDefault(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)  //20170801
				//if (DB_SUCCESS == SetDBValue(g_pDatabase->DataIDToIndex(ID), Value, bSend))
				//{
				//	nResult++;
				//}
				
				if(DB_SUCCESS == g_pDatabase->Write(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)
				//if (DB_SUCCESS == SetDBValue(g_pDatabase->DataIDToIndex(ID), Value, bSend))
				{
					nResult++;
				}		
		}
	}
	
	return nResult;
}

#ifndef	D_BEOBJECT_CTMPickerACTUALVALUE
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
