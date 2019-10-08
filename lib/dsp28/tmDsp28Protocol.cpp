#include	"../../package.h"
#include	"tmDsp28Protocol.h"


IMPLEMENT_DYNCREATE(CtmDsp28Protocol, CtmHostProtocol)

char*	pszProtocols[] =
{
	"HostProtocol_Actual"		,	//	1	the request no
	"HostProtocol_ReadMotor"	,	//	2	the request no
	"HostProtocol_WriteMotor"	,	//	3	the request no
	"HostProtocol_Alarm"		,	//	4	the request no
	"HostProtocol_Curve"		,	//	5	the request no
	"HostProtocol_Command"		,	//	6	the request no
	"HostProtocol_LineStatus"	,	//	7	the request no
	"HostProtocol_MoldSetStatus",	//	8	the request no
	"HostProtocol_MoldSet"		,	//	9	the request no
	"HostProtocol_Temper"			//	10	the request no
	//"HostProtocol_ADJust"		,	//	11	the request no
	//"HostProtocol_ADZero"		,	//	12	the request no
	
};

CtmDsp28Protocol::CtmDsp28Protocol():CtmHostProtocol()
{
	memset(m_pTemp, 0, sizeof(m_pTemp));
}

CtmDsp28Protocol::~CtmDsp28Protocol()
{
	
}

int		CtmDsp28Protocol::CreateSelf()
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
	m_pTemp[i] = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_ReadMotor", "CtmDsp28CommonDiagRead"));
	AddProtocol(m_pTemp[i]);
	i++;
	
	m_pTemp[i] = static_cast<CtmProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_WriteMotor", "CtmDsp28CommonDiagWrite"));
	AddProtocol(m_pTemp[i]);

	return 0;	
}

int		CtmDsp28Protocol::FreeSelf()
{
	unsigned int i = 0;
	for (i = 0; i < sizeof(pszProtocols) / sizeof(char*); i++)
		g_pPackage->ReleaseClassInstance(pszProtocols[i], m_pTemp[i]);
	
	g_pPackage->ReleaseClassInstance("HostProtocol_ReadMotor", m_pTemp[i]);
	i++;
	g_pPackage->ReleaseClassInstance("HostProtocol_WriteMotor", m_pTemp[i]);
	
	return CtmHostProtocol::FreeSelf();
	
}


#ifndef	D_BEOBJECT_CTMDSP28PROTOCOL	//	defineName=D_BEOBJECT_+CASE(ClassName) 
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
		(static_cast<CtmDsp28Protocol*>(pResult))->CreateSelf();	
	return pResult;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmDsp28Protocol*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

