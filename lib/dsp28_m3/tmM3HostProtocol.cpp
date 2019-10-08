#include	"../../package.h"
#include	"tmM3HostProtocol.h"
#include	"../libComm.h"


IMPLEMENT_DYNCREATE(CtmM3HostProtocol, CtmHostProtocol)

WORD	wCmdTypeMap[][2] = {
	COMM_ETH_SENDDLSTART			, 10,
	COMM_ETH_SENDDLDATA				, 10,
	COMM_ETH_SENDDLEND				, 10,
	COMM_ETH_SENDDLREADY			, 10,
	COMM_ETH_SENDDLSMALL			, 10,
	COMM_ETH_GETDLSTART				, 10,
	COMM_ETH_GETDLDATA				, 10,
	COMM_ETH_GETDLEND				, 10,
	COMM_ETH_GETDLREADY				, 10,
	COMM_ETH_GETDLSMALL				, 10,
	COMM_ETH_GETDLSTATE				, 10,
	COMM_ETH_SETDLSTATE				, 10,
	COMM_ETH_GETDLOLDVERS			, 10,
	COMM_ETH_SETDLOLDVERS			, 10,
	COMM_ETH_GETDLCMDSTATE			, 10,
	COMM_ETH_SENDTYPE				, 10,
	COMM_ETH_SELECT28				, 10,
	COMM_ETH_SENDRUNTIME			, 10,
	COMM_ETH_SELECT54TRANSMIT		, 10				//James add 2010/5/13 for transmit 28 udpate
};

char*	pszProtocols[] =
{
	"HostM3Protocol_Actual"		,	//	1	the request no
	"HostM3Protocol_ReadMotor"		,	//	2	the request no
	"HostM3Protocol_WriteMotor"	,	//	3	the request no
	"HostM3Protocol_Alarm"			,	//	4	the request no
	"HostM3Protocol_Curve"			,	//	5	the request no
	"HostM3Protocol_Command"		,	//	6	the request no
	"HostM3Protocol_LineStatus"		,	//	7	the request no
	"HostM3Protocol_MoldSetStatus"	,	//	8	the request no
	"HostM3Protocol_MoldSet"		,	//	9	the request no
	"HostM3Protocol_Update"		,	//  10  the update program
//	"HostProtocol_Temper"			//	10	the request no
	"HostProtocol_ADJust"			,	//	11	the request no
	"HostProtocol_ADZero"			,	//	12	the request no
	"HostM3Protocol_SiemRead"		,	// 	
	"HostM3Protocol_SiemWrite"		,
	"HostM3Protocol_PWMTR"   
};

CtmM3HostProtocol::CtmM3HostProtocol():CtmHostProtocol()
{
	memset(m_pTemp, 0, sizeof(m_pTemp));
}

CtmM3HostProtocol::~CtmM3HostProtocol()
{
	
}

int		CtmM3HostProtocol::CreateSelf()
{
	CtmHostProtocol::CreateSelf();
	unsigned int i = 0;
	for (i = 0; i < sizeof(pszProtocols) / sizeof(char*); i++)
	{
		
		m_pTemp[i] = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance(pszProtocols[i]));
		AddProtocol(m_pTemp[i]);
		if (m_pTemp[i] != NULL)
		{
			m_pTemp[i]->SetSendType(i + 1);
			m_pTemp[i]->SetRevType(1000 + i + 1);
		}
	}
	m_pTemp[i] = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostM3Protocol_ReadMotor", "CtmM3CommonDiagRead"));
	AddProtocol(m_pTemp[i]);
	i++;
	
	m_pTemp[i] = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostM3Protocol_WriteMotor", "CtmM3CommonDiagWrite"));
	AddProtocol(m_pTemp[i]);
			
	return 0;	
}

int		CtmM3HostProtocol::Compose(int nType, void* pOutData, void* pInData, int nInputLen, void* pEx)
{
    //int nCmd  		= nType;
    int nDataType 	= nType;
	int	nResult 	= 0;
	BOOL bSendFlag	= FALSE;
	
	//James add 2009/9/4  處理多種協議Type
	for(int i = 0; i < sizeof(wCmdTypeMap) / sizeof(wCmdTypeMap[0]); i++)
	{
		if(nType == wCmdTypeMap[i][0])
		{
			nDataType = wCmdTypeMap[i][1];
			bSendFlag = TRUE;
		}
	}
	CtmProtocol*	pProt	= GetSendProtocol(nDataType);
	//printf("nDataType=%d nType=%x bSendFlag=%d pProt=%x \n", nDataType, nType, bSendFlag, pProt);
	if (pProt != NULL)
	{
		if(bSendFlag)
			nResult = pProt->ComposeProtocol(nType, pOutData, pInData, nInputLen, pEx);
		else  
			nResult = pProt->ComposeProtocol(pOutData, pInData, nInputLen, pEx);
	}

	return nResult;
}


CtmProtocol*	CtmM3HostProtocol::GetSendProtocol(int nType)
{
	CtmProtocol*	pResult = NULL;
	
	for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
	{
		pResult = m_pProtocols[i];
		
		if (pResult != NULL)
		{
			//printf("pResult->GetType()=%d nType=%d \n", pResult->GetType(), nType);
			if (pResult->GetType() == nType)
				break;
		} 
	}
	
	return pResult;
}

CtmProtocol*	CtmM3HostProtocol::GetRevProtocol(int nType)
{
	CtmProtocol*	pResult = NULL;
	
	for (int i = 0; i < CNT_INT_PROTOCOL_MAXNUM; i++)
	{
		pResult = m_pProtocols[i];
		
		if (pResult != NULL)
		{
			//printf("pResult->RevType()=%x \n", pResult->RevType());
			if (pResult->RevType() == nType)
				break;
		}
	}
	return pResult;
}

int		CtmM3HostProtocol::Parse(void* pInData,int nInputLen)
{
	int nCommState	= *(WORD*)pInData;
	int nType  		= 0;
	int	nDataType 	= 0;
	int nResult 	= 0;
	BOOL bGetFlag	= FALSE;
	//James add 2009/12/11 區分是更新還是正常通訊
	//if(nCommState != 0x5588)
	//	printf("nCommState=%x pInData=%x nInputLen=%d \n", nCommState, pInData, nInputLen);
	if(nCommState == 0x5588 || nCommState == 0)
	{
		nType 	= GetType(pInData, nInputLen);
	}
	else if(nCommState == 0x0002)
	{
		//nDataType2 	= *(WORD*)((BYTE*)pInData+2);
		//nDataType1 	= *(WORD*)((BYTE*)pInData+4);
		nDataType 	= *(WORD*)((BYTE*)pInData+6);
		//printf("nDataType=%x nDataType1=%x, nDataType2=%x \n", nDataType, nDataType1, nDataType2);
		for(int i = 0; i < sizeof(wCmdTypeMap) / sizeof(wCmdTypeMap[0]); i++)
		{
			if(nDataType == wCmdTypeMap[i][0])
			{
				nType = wCmdTypeMap[i][1] + 1000;
				bGetFlag = TRUE;
			}
		}
	}
	else		//Error
	{
		return -1;	
	}
	
	//printf("Parse nType=%x nDataType=%d \n", nType, nDataType);
	CtmProtocol*	pProt	= GetRevProtocol(nType/*+1000*/);
	//if(nCommState != 0x5588)
	//printf("cmd=%d pProt=%x \n", nType, pProt);
	if (pProt != NULL)
	{
		nResult = pProt->ParseProtocol(pInData, nInputLen);
	}
	
	return nResult;
}

int 	CtmM3HostProtocol::GetType(void* pInData, int nInputLen)
{
	if (nInputLen < 14) return -1;

	return *(WORD*)((BYTE*)pInData + 4); 
}


int		CtmM3HostProtocol::FreeSelf()
{
	unsigned int i = 0;
	for (i = 0; i < sizeof(pszProtocols) / sizeof(char*); i++)
		g_pPackage->ReleaseClassInstance(pszProtocols[i], m_pTemp[i]);
	
	g_pPackage->ReleaseClassInstance("HostM3Protocol_ReadMotor", m_pTemp[i]);
	i++;
	g_pPackage->ReleaseClassInstance("HostM3Protocol_WriteMotor", m_pTemp[i]);
	
	return CtmHostProtocol::FreeSelf();
	
}


#ifndef	D_BEOBJECT_CTMM3HOSTPROTOCOL	//	defineName=D_BEOBJECT_+CASE(ClassName) 
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 防止Create 2次
	if(pResult == NULL)
	{
		pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CtmM3HostProtocol*>(pResult))->CreateSelf();	
	}
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmM3HostProtocol*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

