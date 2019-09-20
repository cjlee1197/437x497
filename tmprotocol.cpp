#include	"tmprotocol.h"
#include	"database.h"


IMPLEMENT_DYNCREATE(CtmPartProtocol, CtmPackClass)
IMPLEMENT_DYNCREATE(CtmCommonPartProtocol, CtmPackClass)
IMPLEMENT_DYNCREATE(CtmProtocol, CtmPackClass)
IMPLEMENT_DYNCREATE(CtmHostProtocol, CtmPackClass)

/*==========================================================================+
|           Class implementation - CtmPartProtocol                          |
+==========================================================================*/
//
CtmPartProtocol::CtmPartProtocol()
{
	
}

CtmPartProtocol::~CtmPartProtocol()
{
	
}


int		CtmPartProtocol::SetPartValue(int nPos, void* pValue, int nSize)
{
	return 0;
}

int		CtmPartProtocol::GetPartValue(int nPos, void* pValue, int nSize)
{
	return 0;
}

void*	CtmPartProtocol::GetBuffer()
{
	return NULL;
}

int		CtmPartProtocol::GetSize()
{
	return 0;
}

int		CtmPartProtocol::GetMaxSize()
{
	return 0;
}

void	CtmPartProtocol::SetSize(int nSize)
{
	return;
}

void	CtmPartProtocol::SetMaxSize(int nSize)
{
	return;
}


/*==========================================================================+
|           Class implementation - CtmCommonPartProtocol                    |
+==========================================================================*/
CtmCommonPartProtocol::CtmCommonPartProtocol()
{
	m_pBuffer = NULL;
}

CtmCommonPartProtocol::~CtmCommonPartProtocol()
{
	
}

int		CtmCommonPartProtocol::CreateSelf()
{
	CtmPartProtocol::CreateSelf();	
	
	return 0;
}

int		CtmCommonPartProtocol::FreeSelf()
{
	delete []m_pBuffer;	
	return CtmPartProtocol::FreeSelf();
}


int		CtmCommonPartProtocol::SetPartValue(int nPos, void* pValue, int nSize)
{
	if (nPos > m_nMaxSize || nPos < 0) 	return -1;
	
	if ((nPos + nSize) > m_nMaxSize)	return -1;
		
	memcpy(&m_pBuffer[nPos], pValue, nSize);
	
	return nPos + nSize;
}

int		CtmCommonPartProtocol::GetPartValue(int nPos, void* pValue, int nSize)
{
	if (nPos > m_nMaxSize || nPos < 0) 	return -1;	
	if ((nPos + nSize) > m_nMaxSize)	return -1;		
	if (pValue == NULL)					return -1;
		
	memcpy(pValue, &m_pBuffer[nPos], nSize);
	
	return nPos + nSize;
}

void*	CtmCommonPartProtocol::GetBuffer()
{
	return m_pBuffer;
}


int		CtmCommonPartProtocol::GetSize()
{
	return m_nSize;
}

void	CtmCommonPartProtocol::SetSize(int nSize)
{
	m_nSize = nSize;
}

int		CtmCommonPartProtocol::GetMaxSize()
{
	return m_nMaxSize;
}


void	CtmCommonPartProtocol::SetMaxSize(int nSize)
{
	m_nMaxSize = nSize;
	
	if (m_pBuffer != NULL)
	{
		delete []m_pBuffer;		
	}
	
	m_pBuffer = new BYTE[nSize];
	memset(m_pBuffer, 0, nSize);
}


/*==========================================================================+
|           Class implementation - CtmCommonPartProtocol                    |
+==========================================================================*/
CtmProtocol::CtmProtocol()
{
	m_nSendType 	= -1;
	m_nRevType		= -1;
	m_nCounter      = 0;
	m_nDeviceID     = -1;
}


CtmProtocol::~CtmProtocol()
{
	
}


int		CtmProtocol::InitSend()
{
	return 0;	
}

int		CtmProtocol::InitRev()
{
    return 0;
}
void		CtmProtocol::SetDeviceID(int nID)
{
    m_nDeviceID = nID;
}
int		CtmProtocol::ComposeProtocol(void* pOutData, void* pInData, int nInputLen, void* pEx)
{
	return 0;
}

int		CtmProtocol::ComposeProtocol(int nCmd, void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	return 0;
}

int		CtmProtocol::ParseProtocol(void* pInData, int nInputLen)
{
	return 	0;
}


void	CtmProtocol::SetSendType(int nType)
{
	m_nSendType	= nType;
}

int		CtmProtocol::GetType()
{
	return m_nSendType;
}	

void	CtmProtocol::SetRevType(int nType)
{
	m_nRevType	= nType;
}

int		CtmProtocol::RevType()
{
	return m_nRevType;
}

int		CtmProtocol::SetDataToDataBase(void* pData, int nCount, BOOL bSend)
{
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

///////////////////////////////////////////////////////////////////
CtmHostProtocol::CtmHostProtocol(): CtmPackClass()
{
	memset(&m_List, 0, sizeof(m_List));		
	memset(m_pProtocols, 0, sizeof(m_pProtocols));
	m_nCount = 0;
}

CtmHostProtocol::~CtmHostProtocol()
{
	
}

int     CtmHostProtocol::SetDeviceID(int nID)
{
    CtmProtocol*	pResult = NULL;
    int nCount = 0;
    for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
	{
		pResult = m_pProtocols[i];
		
		if (pResult != NULL)
		{
			pResult->SetDeviceID(nID);
			nCount++;
		}
	}
	return nCount;
}

CtmProtocol*	CtmHostProtocol::GetSendProtocol(int nType)
{
	CtmProtocol*	pResult = NULL;
	
	for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
	{
		pResult = m_pProtocols[i];
		
		if (pResult != NULL)
		{
			if (pResult->GetType() == nType)
				break;
		}
	}
	
	return pResult;
}

CtmProtocol*	CtmHostProtocol::GetRevProtocol(int nType)
{
	CtmProtocol*	pResult = NULL;
	
	for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
	{
		pResult = m_pProtocols[i];
		
		if (pResult != NULL)
		{
			if (pResult->RevType() == nType)
				break;
		}
	}
	
	return pResult;
}

int		CtmHostProtocol::Compose(int nType, void* pOutData, void* pInData, int nInputLen, void* pEx)
{
	int				nResult = 0;
	CtmProtocol*	pProt	= GetSendProtocol(nType);
	
	if (pProt != NULL)
	{
		nResult = pProt->ComposeProtocol(pOutData, pInData, nInputLen, pEx);
	}
    
	return nResult;
}

int		CtmHostProtocol::Parse(void* pInData,int nInputLen)
{
	int 			nResult = 0;
	int				nType 	= GetType(pInData, nInputLen);
	CtmProtocol*	pProt	= GetRevProtocol(nType);
	
	if (pProt != NULL)
	{
		nResult = pProt->ParseProtocol(pInData, nInputLen);
	}
	
	return nResult;
}

int CtmHostProtocol::GetType(void* pInData, int nInputLen)
{
    return -1;
}

BOOL	CtmHostProtocol::AddProtocol(CtmProtocol* pProtocol)
{
	if (m_nCount >= CNT_INT_PROTOCOL_MAXNUM) return FALSE;
	else
	{
		for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
		{
			if (m_pProtocols[i] == NULL)
			{
				m_pProtocols[i] = pProtocol;
				m_nCount++;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL	CtmHostProtocol::DelProtocol(CtmProtocol* pProtocol)
{
	BOOL	bResult = FALSE;
	
	for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
	{
		if (m_pProtocols[i] == pProtocol)
		{
			bResult 		= TRUE;
			m_pProtocols[i] = NULL;
			m_nCount++;
			break;
		}
	}
	
	return bResult;
}

