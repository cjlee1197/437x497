#include	"../../tmdebug.h"
#include	"../../tmshmsg.h"
#include	"tmM3Command.h"


IMPLEMENT_DYNCREATE(CtmM3Command, CtmDsp28Protocol)

CtmM3Command::CtmM3Command(): CtmDsp28Protocol()
{
	
}

CtmM3Command::~CtmM3Command()
{
	
}


int		CtmM3Command::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 6;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmM3Command::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int		nResult = 0;
	if (pOutData == NULL || nInputLen != 1)	return nResult;
	
	WORD wTemp 	= 0;
	
	m_nCounter++;
	
	//	set the head's cycle time
	if (pEx == NULL) 	wTemp = 0;
	else				wTemp = *(WORD*)pEx;
	m_SendHeader.SetPartValue(m_nCyclePos,		&wTemp, sizeof(wTemp));		
	
	//	get the head size
	nResult = m_SendHeader.GetSize();
	
	
	long*	pOutCmd 	= (long*)((BYTE*)pOutData + nResult);
	long*	pInCmd		= (long*)(pInData);
	
	for (int i = 0; i < nInputLen; i++)
	{	
		*pOutCmd	= *pInCmd;
		//printf("*pOutCmd=%d \n", *pOutCmd);
		pOutCmd++;
		pInCmd++;
		nResult = nResult + sizeof(long); 
	}
	//	write the footer to the out buffer
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
	
	//	write the header to the out buffer
	memcpy(pOutData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	return nResult;
}

int		CtmM3Command::ParseProtocol	(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	long	Value	= 0;
	int		nResult	= 0;
	BYTE*	pData = NULL;
	pData = (BYTE*)pInData + m_RevHeader.GetSize();
	for (int i = 0; i < wCount; i++)
	{
		//Value			= *(long*)pData;
		memcpy(&Value, pData, sizeof(pData));
		pData			= (BYTE*)pData + sizeof(Value);
		
		//value為接收到 n2c傳回的key值，如果需要處理，在這里就可以
		printf("ParseProtocol ID = 1006  Value=%x \n", (DWORD)Value);
		SendMsg(MSG_SH_AUSTONE_COMMANDCMD, Value, 0, NULL);
		nResult++;
	}
	
	return nResult;//SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
}


#ifndef	D_BEOBJECT_CTMN2CCOMMAND	//	defineName=D_BEOBJECT_+CASE(ClassName) 
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
