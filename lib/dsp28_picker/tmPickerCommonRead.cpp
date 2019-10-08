#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"../../main.h"	//	g_pApplication
#include	"tmPickerCommonRead.h"



IMPLEMENT_DYNCREATE(CtmPickerCommonRead, CtmDsp28Protocol)
IMPLEMENT_DYNCREATE(CtmPickerCommonDiagRead, CtmPickerCommonRead)

long lSaveSramID[] = 
{
	0x034A,
	0x034B,	
	0x034C,	
	0x034D
};

CtmPickerCommonRead::CtmPickerCommonRead(): CtmDsp28Protocol()
{
	
}

CtmPickerCommonRead::~CtmPickerCommonRead()
{
	
}


int		CtmPickerCommonRead::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 2;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmPickerCommonRead::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
		//printf("CtmDsp28ActualValue::ComposeProtocol  nInputLen = %d\n", nInputLen);
		for (int i = 0; i < nInputLen; i++)
		{
			//printf("CtmDsp28ActualValue::ComposeProtocol  ID = %x\n", plID[i]);
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

int		CtmPickerCommonRead::ParseProtocol(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	//printf("pInData = %x\n",pInData);
	//printf("wCount=%d nInputLen=%d \n", wCount, nInputLen);
	return SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
}

int		CtmPickerCommonRead::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	
  //printf("nCount = %d\n",nCount);	
	if (nCount <= 0)	return 0;
	
	//WORD	ID 		= 0;
	long	ID = 0;
	
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{
		//for(int j = 0; j < 8; j++)
		//{
		//	printf("2Byte%d = %x ", j, *bTemp);
		//	bTemp++;
		//}
		//printf(" \n");
		
	//	ID				= *(WORD*)pData;
	  memcpy(&ID, pData, sizeof(ID));
		
		pData 			= (BYTE*)pData + sizeof(ID);	
		
		//Type			= *(BYTE*)pData;
		memcpy(&Type, pData, sizeof(Type));
		
		/*switch(Type)
		{
			case TYPE_BOOL:
				break;
			case TYPE_BYTE:
				break;
			case TYPE_WORD:
				break;
			case TYPE_DWORD:
				break;
			default:
				break;
		}*/
		pData			= (BYTE*)pData + sizeof(Type);
	//	State			= *(BYTE*)pData;
  	memcpy(&State, pData, sizeof(State));
		
		pData			= (BYTE*)pData + sizeof(State);
	//	Value			= *(long*)pData;
		memcpy(&Value, pData, sizeof(Value));
		pData			= (BYTE*)pData + sizeof(Value);
		
		pData			= (BYTE*)pData + 10*sizeof(BYTE); // for 437x497 to 28335 parse // cjlee add 2019/1/24 下午 12:56:23 
		//	need to check the data's type and state 
		//if(ID == 0x800005 || ID == 0x800006 || ID == 0x800771 || ID == 0x800772 || ID == 0x800773 )
		//printf("2 ID=%x Value=%x State=%x Type=%x \n", ID, Value, State, Type);	
		if 	(State == 1)
		{
			for(WORD k = 0; k < sizeof(lSaveSramID) / sizeof(long); k++)
			{
				if( lSaveSramID[k] == ID)
				{
					bSend = TRUE;
					break;
				}
				else
					bSend = FALSE;
			}
			
			if(bSend)
			{
				if(DB_SUCCESS == g_pDatabase->WriteDefault(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)
				//if (DB_SUCCESS == SetDBValue(g_pDatabase->DataIDToIndex(ID), Value, bSend))
				{
					nResult++;
				}
			}
			else
			{
				if(DB_SUCCESS == g_pDatabase->Write(g_pDatabase->DataIDToIndex(ID),&Value, bSend).dwState)
				//if (DB_SUCCESS == SetDBValue(g_pDatabase->DataIDToIndex(ID), Value, bSend))
				{
					nResult++;
				}
			}
		}
	}
	
	return nResult;
}

CtmPickerCommonDiagRead::CtmPickerCommonDiagRead(): CtmPickerCommonRead()
{
	
}


CtmPickerCommonDiagRead::~CtmPickerCommonDiagRead()
{
}


int		CtmPickerCommonDiagRead::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	//printf("CtmPickerCommonDiagRead::CreateSelf()\n");
	memset(m_nAryDiagValues, 0, sizeof(m_nAryDiagValues));	
	m_nSendType	= 0x8000 | 2;
	m_nRevType	= (m_nSendType + 1000) | 0x8000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}

int		CtmPickerCommonDiagRead::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int nSendType 	= m_nSendType;
	int nResult		= 0;
	m_nSendType		= m_nSendType & 0x7FFF;
	WORD	wTemp	= m_nSendType;
		
	m_SendHeader.SetPartValue(m_nTypePos, 	&wTemp, sizeof(wTemp));
	nResult = CtmPickerCommonRead::ComposeProtocol(pOutData, pInData, nInputLen, pEx);	
	
	m_nSendType  	= nSendType;
	return nResult;
}


int		CtmPickerCommonDiagRead::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
	printf("11111\n");
	if (nCount > 128) nCount = 128;
		
	if (nCount <= 0)	return 0;
	
	
	WORD	ID 		= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	long	Value	= 0;
	
	int		nResult	= 0;

	for (int i = 0; i < nCount; i++)
	{
		ID				= *(WORD*)pData;		
		pData 			= (BYTE*)pData + sizeof(ID);		
		
		Type			= *(BYTE*)pData;	
		pData			= (BYTE*)pData + sizeof(Type);
		State			= *(BYTE*)pData;
		
		pData			= (BYTE*)pData + sizeof(State);
		Value			= *(long*)pData;
		
		//前面是存ID，后面是存Value
		if( i > 128)
			i = 128;
		m_nAryDiagValues[i] = ID;
		if(nCount > 128)
			nCount = 128;
		m_nAryDiagValues[nCount+i] = Value;
		
		pData			= (BYTE*)pData + sizeof(Value);	
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

#ifndef	D_BEOBJECT_CTMPICKERCOMMONREAD
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
