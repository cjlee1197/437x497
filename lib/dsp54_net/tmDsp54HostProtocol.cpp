#include	"../../package.h"
#include	"tmDsp54HostProtocol.h"
#include	"../libComm.h"
IMPLEMENT_DYNCREATE(CtmDsp54HostProtocol, CtmHostProtocol)

WORD	wCmdTypeMap[][2] = {
//	COMM_ETH_SENDREQUESTREAD		, 0,
//	COMM_ETH_SENDUPDATEDATA			, 0,
//	COMM_ETH_SENDPANEL				, 0,
//	COMM_ETH_SENDPANELBREAK			, 0,
//	COMM_ETH_SENDCONTROLMACHINE1	, 0,
//	COMM_ETH_SENDCONTROLMACHINE2	, 0,
//	COMM_ETH_SENDCONTROLMOLDSET1	, 0,
//	COMM_ETH_SENDCONTROLMOLDSET2	, 0,
//	COMM_ETH_SENDCONTROLMACHINEB	, 0,
//	COMM_ETH_SENDCONTROLMOLDSETB1	, 0,
//	COMM_ETH_SENDCONTROLMOLDSETB2	, 0,
//	COMM_ETH_SENDDIAGRMRI			, 0,
//	COMM_ETH_SENDDIAGWM				, 0,
//	COMM_ETH_SENDDIAGWO				, 0,
//	COMM_ETH_SENDASSIGNINPUT		, 0,
//	COMM_ETH_SENDASSIGNOUTPUT		, 0,
//	COMM_ETH_SENDMONILIMIT			, 0,
//	COMM_ETH_SENDMONBILIMIT			, 0,
	COMM_ETH_SENDDLSTART			, 1,
	COMM_ETH_SENDDLDATA				, 1,
	COMM_ETH_SENDDLEND				, 1,
	COMM_ETH_SENDDLREADY			, 1,
	COMM_ETH_SENDDLSMALL			, 1,
	COMM_ETH_GETDLSTART				, 1,
	COMM_ETH_GETDLDATA				, 1,
	COMM_ETH_SENDRUNTIME			, 1,
	COMM_ETH_GETDLEND				, 1,
	COMM_ETH_GETDLREADY				, 1,
	COMM_ETH_GETDLSMALL				, 1,
	COMM_ETH_GETDLSTATE				, 1,
	COMM_ETH_SETDLSTATE				, 1,
	COMM_ETH_GETDLOLDVERS			, 1,
	COMM_ETH_SETDLOLDVERS			, 1,
	COMM_ETH_GETDLCMDSTATE			, 1,
	COMM_ETH_SELECT54				, 1,
	COMM_ETH_SETSUFFIXNAME			, 1,
	COMM_ETH_GETPACKCOUNT			, 1,
	COMM_ETH_SENDTYPE				, 1,
	COMM_ETH_SETBURNTYPE			, 1,
	COMM_ETH_MODE_TRANSFORM			, 1,
	COMM_ETH_MODE_NORMAL			, 1,
	COMM_ETH_GETTRANSFORM_ECHO		, 1,	
	
};

char*	pszProtocols[] =
{
	"HostProtocol_54netAll",
	"HostProtocol_54netUpdate"
};

CtmDsp54HostProtocol::CtmDsp54HostProtocol()
{
	memset(m_pTemp, 0, sizeof(m_pTemp));
}

CtmDsp54HostProtocol::~CtmDsp54HostProtocol()
{
	
}

int		CtmDsp54HostProtocol::Compose(int nType, void* pOutData, void* pInData, int nInputLen, void* pEx)
{
    int nCmd  		= nType;
    int nDataType 	= 0x0000;
	int	nResult 	= 0;
//	printf("nCmd = %x\n",nCmd);
	//James add 2009/9/4  處理多種協議Type
	for(int i = 0; i < sizeof(wCmdTypeMap) / sizeof(wCmdTypeMap[0]); i++)
	{
		if(nType == wCmdTypeMap[i][0])
		{
			nDataType = wCmdTypeMap[i][1];
		}
	}
	
	CtmProtocol*	pProt	= GetSendProtocol(nDataType);
	//printf("Compose nType=%x nDataType=%d pProt=%x \n", nType, nDataType, pProt);
	if (pProt != NULL)
	{
		nResult = pProt->ComposeProtocol(nCmd, pOutData, pInData, nInputLen, pEx);
	}

	return nResult;
}

CtmProtocol*	CtmDsp54HostProtocol::GetSendProtocol(int nType)
{
	CtmProtocol*	pResult = NULL;
	
	if(nType < CNT_INT_PROTOCOL_MAXNUM)
	{
		pResult = m_pProtocols[nType];
	}
	
	return pResult;
}

CtmProtocol*	CtmDsp54HostProtocol::GetRevProtocol(int nType)
{
	CtmProtocol*	pResult = NULL;
	
	if(nType < CNT_INT_PROTOCOL_MAXNUM)
	{
		pResult = m_pProtocols[nType];
	}
	
	return pResult;
}

int		CtmDsp54HostProtocol::Parse(void* pInData,int nInputLen)
{
	int nType  		= *(WORD*)((BYTE*)pInData + 6);
	int	nDataType 	= 0x0000;
	int nResult 	= 0;
	//James add 2009/9/4  處理多種協議Type
	for(int i = 0; i < sizeof(wCmdTypeMap) / sizeof(wCmdTypeMap[0]); i++)
	{
		if(nType == wCmdTypeMap[i][0])
		{
			nDataType = wCmdTypeMap[i][1];
		}
	}
	//printf("Parse nType=%x nDataType=%d \n", nType, nDataType);
	CtmProtocol*	pProt	= GetRevProtocol(nDataType);
	//printf("pProt=%x\n",pProt);
	if (pProt != NULL)
	{
		nResult = pProt->ParseProtocol(pInData, nInputLen);
	}
	
	return nResult;
}

int CtmDsp54HostProtocol::GetType(void* pInData, int nInputLen)
{
	if (nInputLen < 18) return -1;
	return *(WORD*)((BYTE*)pInData + 4);
}

int		CtmDsp54HostProtocol::CreateSelf()
{
	CtmHostProtocol::CreateSelf();
	unsigned int i = 0;
	
	for (i = 0; i < sizeof(pszProtocols) / sizeof(char*); i++)
	{
		m_pTemp[i] = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance(pszProtocols[i]));
		//printf("m_pTemp[%d]=%x \n", i,m_pTemp[i]);
		AddProtocol(m_pTemp[i]);
	}
	return 0;
}

int		CtmDsp54HostProtocol::FreeSelf()
{
	unsigned int i = 0;
	for (i = 0; i < sizeof(pszProtocols) / sizeof(char*); i++)
		g_pPackage->ReleaseClassInstance(pszProtocols[i], m_pTemp[i]);

	return CtmHostProtocol::FreeSelf();
	
}


#ifndef	D_BEOBJECT_CTMDSP54HOSTPROTOCOL
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
		(static_cast<CtmDsp54HostProtocol*>(pResult))->CreateSelf();
	return pResult;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmDsp54HostProtocol*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

