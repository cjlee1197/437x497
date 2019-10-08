#include	"../../tmdebug.h"
#include	"../../database.h"
#include	"tmM3Alarm.h"
#include	"../../tmcontrol.h"
#include	"../../utils.h"
#include	"../../tmshmsg.h"


IMPLEMENT_DYNCREATE(CtmM3Alarm, CtmDsp28Protocol)

CtmM3Alarm::CtmM3Alarm(): CtmDsp28Protocol()
{
	
}

CtmM3Alarm::~CtmM3Alarm()
{
	
}


int		CtmM3Alarm::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 4;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3Alarm::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	if (nInputLen == 0 || pInData == NULL)		return 0;
		
	int		nResult = 0;
	//目前警報只接收，不發送給n2c
/*	DBVALUE	db;
	
	
	
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
			nResult 		+= 8;
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
	
	
	memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());	*/
	return	nResult;
}

int		CtmM3Alarm::ParseProtocol	(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;
		
	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	
	int		nResult	= 0;
	BYTE	Type	= 0;
	BYTE	State	= 0;
	WORD	ID 		= 0;
	
	void*	pData 	= (BYTE*)pInData + m_RevHeader.GetSize();
	int		ERR_BEGIN	= g_MultiLanguage.GetStrKeyIndex("AUSTONE_ALARM_01");
	
	for (int i = 0; i < wCount; i++)
	{
		Type			= *(BYTE*)pData;					//警報類型 1為提示 2為警報
		pData		 	= (BYTE*)pData + sizeof(Type);		
		
		State			= *(BYTE*)pData;					//警報狀態
		pData			= (BYTE*)pData + sizeof(State);
		
		ID				= *(WORD*)pData;					//警報Index
		pData			= (BYTE*)pData + sizeof(ID);
		
		//等待Kernel處理 ---Error字串ERR_0F_TEMPERR，需要處理
		//===========
		//
		//===========
		if(1 == Type)				//Message
		{
			//SendMsg(MSG_USER_SH_PROMPTERROR, ERR_BEGIN+ID-1, 0, NULL);
		}
		else if(2 == Type)			//Error
		{
			//SendMsg(MSG_USER_SH_PROMPTERROR, ERR_BEGIN+ID-1, 0, NULL);
			//SendMsg(MSG_USER_SH_ERROR_ADD, 0, 0, NULL);
		}
		else if(0 == Type)
		{
			//SendMsg(MSG_USER_SH_ERROR_FIX, 0, 0, NULL);
			//SendMsg(MSG_USER_SH_PROMPTNULL, 0, 0, NULL);
		}
		if (State == 0)				//it's ok
			nResult++;
	}
	
	return nResult;
}


#ifndef	D_BEOBJECT_CTMM3ALARM
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
