#include	"aioperation.h"
#include	"selectedit.h"
#include	"main.h"			//	need the g_pApplication


IMPLEMENT_DYNAMIC(CtmAiOperation, CObject)

CtmAiOperation::CtmAiOperation()
{
	InitPara();
}

CtmAiOperation::CtmAiOperation(CtmWnd*	pwnd)
{
	InitPara();
	m_pAiWnd	= pwnd;
	
}

CtmAiOperation::~CtmAiOperation()
{
	
}

void	CtmAiOperation::InitPara()
{
	m_pAiWnd	= NULL;
	m_nOpType	= 0;
	m_dbWnd		= NULL;
	m_bOp		= FALSE;
	m_llValue	= -1;
	
	memset(m_szDBID, 	0	, sizeof(m_szDBID));
	memset(m_llValues, 	0xFF, sizeof(m_llValues));
}

void	CtmAiOperation::SetAiWnd(CtmWnd* pwnd)
{
	m_pAiWnd = pwnd;
}

CtmWnd* CtmAiOperation::GetAiWnd()
{
	return m_pAiWnd;
}

CtmWnd*	CtmAiOperation::GetDbWnd()
{
	return m_dbWnd;
}

void	CtmAiOperation::GetWndInfo()
{
	if (m_pAiWnd != NULL)
		m_bOp = m_pAiWnd->GetPropValueT(WND_AI_FLAG, &m_nOpType);
	
	if (m_bOp)
	{
		char szTemp[128];
		
		//	get the linking component
		m_pAiWnd->GetPropValueT(WND_AI_LINKWND, szTemp);
		if (szTemp[0] != '\0')
		{
			if (m_pAiWnd->GetParent() != NULL)
				m_dbWnd = m_pAiWnd->GetParent()->FindControlFromName(szTemp);
			else
				m_dbWnd = NULL;
		}	
		else 
			m_dbWnd = NULL;
		
		//	get the linking dbid
		if (m_dbWnd == NULL)
			m_pAiWnd->GetPropValueT(WND_AI_LINKDB, m_szDBID);
		
		//	get the value
		m_pAiWnd->GetPropValueT(WND_AI_VALUE, &m_llValue);
		
		//	get the values
		for (int i = 0; i < sizeof(m_llValues) / sizeof(m_llValues[0]); i++)
		{
			sprintf(szTemp, "%s%d", WND_AI_VALUE, i);
			m_pAiWnd->GetPropValueT(szTemp, &m_llValues[i]);
		}
	}
	
	
}

void	CtmAiOperation::SetYes(char* pszProperty)
{
	BOOL		bTemp 	= TRUE;
	BOOL		bOld	= TRUE;
	long long 	llValue	= 0;
	long long 	llValue1= 0;
	char		szID[128];
	
	
	m_pAiWnd->GetPropValueT("dbid0", szID);
	m_pAiWnd->GetPropValueT(pszProperty, &bOld);
	m_pAiWnd->GetPropValueT(WND_AI_DISIBLE_DEFAULT, &llValue1);
	
	llValue = GetDBValue(szID).lValue;
	
	m_pAiWnd->SetPropValueT(pszProperty, (double)bTemp);
	if (bTemp != bOld || llValue1 == llValue)
	{		
		m_pAiWnd->GetPropValueT(WND_AI_VISILBE_DEFAULT, &llValue);	
		if (szID != '\0' && llValue != AI_DEFAULT)
		{
			
			SetDBValue(szID, llValue);
		}
		else if (szID != '\0' && llValue == AI_DEFAULT)
		{
			
			SetDBValue(szID, GetDBDefaultValue(szID).lValue);
		}
		else
			;
	}
		
		
	//printf("Set YES %s visbleValue=%lld, llvalue=%lld\n", m_pAiWnd->Name, GetDBValue(szID).lValue, llValue);
					
}

void	CtmAiOperation::SetNo(char* pszProperty)
{
	BOOL		bTemp 	= FALSE;
	BOOL		bOld	= FALSE;
	long long 	llValue	= 0;
	long long	llValue1=0;
	char		szID[128];
	
	m_pAiWnd->GetPropValueT("dbid0", szID);
	m_pAiWnd->GetPropValueT(pszProperty, &bOld);
	m_pAiWnd->GetPropValueT(WND_AI_DISIBLE_DEFAULT, &llValue);	
	llValue1 = GetDBValue(szID).lValue;
	m_pAiWnd->SetPropValueT(pszProperty, (double)bTemp);
	
	if (bTemp != bOld || llValue1 != llValue)
	{
		
		if (szID != '\0' && llValue != AI_DEFAULT)
		{
			SetDBValue(szID, llValue);
		}
	}
	
	//printf("Set No %s GetDBValue=%lld, diValue=%lld\n", m_pAiWnd->Name, GetDBValue(szID).lValue, llValue);
				
}


int		CtmAiOperation::AiOp()
{
	if (m_bOp)
	{
		if (m_dbWnd == NULL) return AiOpDB();
		else
		{
			if (m_dbWnd->Is("CtmEditX1") || m_dbWnd->Is("CtmEditX2"))
				return AiOpEdit();
			else if (m_dbWnd->Is("CtmOptionBox")) 
				return AiOpOption();
			else if (m_dbWnd->Is("CtmSelectEdit"))
				return AiOpSelectEdit();
			return m_nOpType;
		}
	}	
	else return 0;
}

void	CtmAiOperation::ExeAiOp(int nAiOp)
{
	if (m_bOp)
	{		
		if (nAiOp & AI_OP_TYPE_VISIBLE)			//	set visible property
		{
			if 		(nAiOp & AI_OP_NO)		SetNo("visible");
			else if (nAiOp & AI_OP_YES)		SetYes("visible");
			else							;
		}
		
		if (nAiOp & AI_OP_TYPE_ENABLE)		//	set enable property
		{
			if 		(nAiOp & AI_OP_NO)		SetNo("enabled");
			else if (nAiOp & AI_OP_YES)		SetYes("enabled");
			else							;
		}
	}
	
	
	
}

void	CtmAiOperation::ExeAiOp()
{
	if (m_bOp)
	{
		ExeAiOp(AiOp());
		
		if (m_nOpType & AI_OP_TYPE_MESSAGE)
		{
			int	nMsgID 	= 0;
			int	nLParam	= 0;
			int	nWParam	= 0;
			m_pAiWnd->GetPropValueT(WND_AI_MSGID, &nMsgID);
			
			if (nMsgID != 0)
			{
				m_pAiWnd->GetPropValueT(WND_AI_LPARAM, &nLParam);
				m_pAiWnd->GetPropValueT(WND_AI_WPARAM, &nWParam);
				MSG			msg;
				memset(&msg, 0, sizeof(msg));
				msg.message = nMsgID;
				msg.lParam	= nLParam;
				msg.wParam	= nWParam;
				g_pApplication->QueueMessage(&msg);	
			}			
		}
	}
	
}


int		CtmAiOperation::Compare(long long llValue)
{
	if (m_llValue != -1)
	{
		if (llValue >= m_llValue)	return (m_nOpType & 0xFFFF0000 | AI_OP_YES);
		else 						return (m_nOpType & 0xFFFF0000 | AI_OP_NO);
	}
	else
	{		
		for (int i = 0; i < sizeof(m_llValues) / sizeof(m_llValues[0]); i++)
		{
			if (llValue == m_llValues[i]) 
				return (m_nOpType & 0xFFFF0000 | AI_OP_NO);
		} 
		return (m_nOpType & 0xFFFF0000 | AI_OP_YES);
	}
}

int		CtmAiOperation::AiOpEdit()
{
	long long llValue = 0;
	
	m_dbWnd->GetPropValueT("value", &llValue);
	
	return Compare(llValue);
}


int		CtmAiOperation::AiOpOption()
{
	int	nValue = 0;
	m_dbWnd->GetPropValueT("selectindex", &nValue);
	
	return Compare(nValue);
	
}

int		CtmAiOperation::AiOpSelectEdit()
{
	int nValue = 0;
	nValue = (static_cast<CtmSelectEdit*>(m_dbWnd))->GetIndex();
	
	return Compare(nValue);
}

int		CtmAiOperation::AiOpDB()
{
	long long llValue = 0;
	llValue = GetDBValue(m_szDBID).lValue;
	
	return Compare(llValue);
}

