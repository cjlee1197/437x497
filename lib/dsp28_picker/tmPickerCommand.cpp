#include	"../../tmdebug.h"
#include	"tmPickerCommand.h"


IMPLEMENT_DYNCREATE(CtmPickerCommand, CtmDsp28Protocol)

CtmPickerCommand::CtmPickerCommand(): CtmDsp28Protocol()
{
	
}

CtmPickerCommand::~CtmPickerCommand()
{
	
}


int		CtmPickerCommand::CreateSelf()
{
	CtmProtocol::CreateSelf();
		
	m_nSendType	= 6;
	m_nRevType	= m_nSendType + 1000;	
	
	InitSend();		//	this is a virtual function, it can be override
	InitRev();		//	this is a virtual function, it can be override
	
	return 0;
}


int		CtmPickerCommand::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
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
	
  	*pInCmd += 0x12330000;                    //20140417 add 
    //printf("nInputLen = %d, pInData = %x\n", nInputLen, *pInCmd);
  
	for (int i = 0; i < nInputLen; i++)
	{	
		*pOutCmd	= *pInCmd;
		
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

int		CtmPickerCommand::ParseProtocol	(void* pInData,		int nInputLen)
{
	if (nInputLen < m_RevFooter.GetSize() + m_RevHeader.GetSize())	return -1;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	
	WORD wCount = 0;
	m_RevHeader.GetPartValue(m_nElementNumPos, &wCount, sizeof(wCount));
	
	long	Value	= 0;
	int		nResult	= 0;
	BYTE*	pData = NULL;
	pData = (BYTE*)pInData + m_RevHeader.GetSize();
	//printf("wCount=%d \n", wCount);
	for (int i = 0; i < wCount; i++)
	{
		Value			= *(long*)pData;
		pData			= (BYTE*)pData + sizeof(Value);
		
		//value為接收到 dsp28傳回的key值，如果需要處理，在這里就可以
		//printf("Value=%d \n", Value);
		nResult++;
	}
	
	return nResult;//SetDataToDataBase((BYTE*)pInData + m_RevHeader.GetSize(), wCount, FALSE);
}


#ifndef	D_BEOBJECT_CTMPICKERCOMMAND	//	defineName=D_BEOBJECT_+CASE(ClassName) 
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
