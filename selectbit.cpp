/*==========================================================================+
|  Class    : Select Edit		                                            |
|  Task     : Select Edit source file                        				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : FAns                                                           |
|  Version  : V1.00                                                         |
|  Creation : 2010-1-6 18:48:21                                           	|
|  Revision : 			                                                    |
+==========================================================================*/

#include    "selectbit.h"
#include	"init.h"
#include    "utils.h"

#include	"commonaction.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/

/*==========================================================================+
|           Class implementation - CtmSelectBit                            |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmSelectBit, CtmSelectEdit)
CtmSelectBit::CtmSelectBit(): CtmSelectEdit()
{
	strcpy(TYPEName, "CtmSelectBit");

    m_wBitValue 	= 0;
    m_llValue		= 0;
    m_pszDBID		= NULL;
    m_nBitIndex 	= -1;
}
	

CtmSelectBit::~CtmSelectBit()
{
	
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                Get()                                           |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

BOOL 	CtmSelectBit::CreateA()
{
	CtmSelectEdit::CreateA();
	
	char  sz[4];
	char  szName[10];
	char  szID[256];
    for (int i = 0; i < CNT_INT_BIT_SHOW; i++)
    {
    	memset(szID, 	0, sizeof(szID));
    	memset(sz, 		0, sizeof(sz));
    	
    	sprintf(szName, "%s%d", "strID", i);
    	GetPropValueT(szName, szID, sizeof(szID));
    	
    	if (szID[0] == '\0')
    	{
    		char szTemp[2];
    		memset(szTemp, 0, sizeof(szTemp));
	    	sprintf(szTemp, "%d", i); 
	    	_tm_chartoword((WORD*)sz, szTemp);
	    	sprintf(szName, "%s%d", "str", i);
	    	SetPropValueT(szName, sz);
	    }
    }
    
	SetPropValueT("maxindex", 2);
	
	GetDBInfo();
	Set();
	return TRUE;
}

void 	CtmSelectBit::SetPropertys()
{
	//	dbid0,
	//	bitindex
	//	focuscorlor
	//	prompt
	//	promptID
	
	//SetPropInfo("value",		tmINT64,	&m_llValue);	//	just read;can't write value
	CtmSelectEdit::SetPropertys();
	SetPropInfo("dbid0", 		tmSTR,		&m_pszDBID);	//	override the dbid0 Property
	SetPropInfo("bitindex",		tmINT, 		&m_nBitIndex);	
	SetPropInfo("bitvalue",		tmWORD,		&m_wBitValue);	
	//SetPropInfo("focuscolor",	tmDWORD,	&m_bgcOnFocus);
}

void 	CtmSelectBit::SetPropertys_Nub()
{
	CtmSelectEdit::SetPropertys_Nub();
	SetPropInfo(SELECTBIT_DBID0, 		tmSTR,		&m_pszDBID);	//	override the dbid0 Property
	SetPropInfo(SELECTBIT_BITINDEX,		tmINT, 		&m_nBitIndex);	
	SetPropInfo(SELECTBIT_BITVALUE,		tmWORD,		&m_wBitValue);	
	//SetPropInfo("focuscolor",	tmDWORD,	&m_bgcOnFocus);
}


void    CtmSelectBit::OnGetFocus()
{
	CtmWnd::DrawGetFocus();
		
	WORD	szPrompt[256];
	GetPropValueT("prompt", szPrompt, sizeof(szPrompt));
	if (szPrompt[0] != 0)	Prompt((char*)szPrompt);
}


void	CtmSelectBit::OnLoseFocus()
{
	SetDBInfo();
	CtmSelectEdit::OnLoseFocus();
}

void	CtmSelectBit::GetDBInfo()
{
	if (m_pszDBID != NULL)
	{
		m_llValue = GetDBValue(m_pszDBID).lValue;
		if (m_nBitIndex >= 0) 
		{
			long long llValue = 1;
			llValue <<= m_nBitIndex;
			
			if (llValue & m_llValue) 	m_wBitValue = 1;
			else						m_wBitValue = 0;
		}
	}
	else
	{
		m_llValue 	= 0;
		m_wBitValue = 0;
	}
	
	
	SetIndex(m_wBitValue);
}

void	CtmSelectBit::SetDBInfo()
{
	m_wBitValue = GetIndex();
	
	if (m_pszDBID != NULL)
	{
		m_llValue = GetDBValue(m_pszDBID).lValue;	
		long long llValue = 1;
		llValue <<= m_nBitIndex;
		
		if (m_wBitValue)	m_llValue |= llValue;
		else				m_llValue &= (~llValue);		
		SetViewDBValue(m_pszDBID, m_llValue);
	}
	
}

bool 	CtmSelectBit::SetPropValueT(char* pszPropName, double Value)
{
	CtmSelectEdit::SetPropValueT(pszPropName, Value);
		
		
	if (strcmp(pszPropName, "bitvalue") == 0)
	{
		if (Value != 0) m_wBitValue = 1;
		else			m_wBitValue = 0;
			
		SetDBInfo();
	}
	
	return true;
	
}

bool 	CtmSelectBit::SetPropValueT(char* pszPropName, char* pszValue)
{
	return CtmSelectEdit::SetPropValueT(pszPropName, pszValue);
}

int		CtmSelectBit::OnLose()
{
	GetDBInfo();
	CtmSelectEdit::OnLose();
	return 0;
}
