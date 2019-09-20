#include	"tmpackclass.h"

IMPLEMENT_DYNCREATE(CtmPackClass, CObject)

CtmPackClass::CtmPackClass(): CObject()
{
	
}

CtmPackClass::~CtmPackClass()
{
	
}

int		CtmPackClass::RegFunc(char* pszFuncName, pRegFunc pFunc)
{
	int	nResult = 0;
	tmRegFunc* pTempFunc = m_pRegFunc;
	
	if (m_pRegFunc != NULL && pszFuncName[0] != 0)	
	{
		while (TRUE)
		{ 
			if (strcmp(pszFuncName, pTempFunc->szFuncName) == 0)	//	有重復的名
			{
				nResult 			= CNT_INT_REGFUNC_OVERRIDE;		//	表示將會覆蓋原有的函數
				pTempFunc->pFunc 	= pFunc;
				break;
			}
			else
				nResult++;
			
			
			if (pTempFunc->pNext == NULL)
			{
				tmRegFunc*	pNewRegFunc 	= new tmRegFunc;
				memset(pNewRegFunc, 0, sizeof(tmRegFunc));
				
				strncpy(pNewRegFunc->szFuncName, pszFuncName, 255);
				pNewRegFunc->pFunc 	= pFunc;
				pTempFunc->pNext	= pNewRegFunc;
				break;	
			}
			else
				pTempFunc = pTempFunc->pNext;
		}
	}
	else if (m_pRegFunc == NULL && pszFuncName[0] != 0)
	{
		m_pRegFunc = new tmRegFunc;
		memset(m_pRegFunc, 0, sizeof(tmRegFunc));
		strncpy(m_pRegFunc->szFuncName, pszFuncName, 255);
		m_pRegFunc->pFunc = pFunc;
	}
	else
	{
		nResult = -1;
	}
	
	return nResult;
	
	
}

int		CtmPackClass::ExeRegFunc(char* pszFuncName, int nParaNum, void* pParas[])
{
	int	nResult 		= CNT_INT_REGFUNC_RETURN_NOREG;
	tmRegFunc* pTempFunc = m_pRegFunc;
	
	if (m_pRegFunc != NULL && pszFuncName[0] != 0)	
	{
		while (TRUE)
		{ 
			if (strcmp(pszFuncName, pTempFunc->szFuncName) == 0)	//	找到了名稱
			{
				//printf("packclass Exe %s\n", pszFuncName);
				nResult = (this->*pTempFunc->pFunc)(nParaNum, pParas);
				break;
			}
				
			
			if (pTempFunc->pNext == NULL)
				break;
			else
				pTempFunc = pTempFunc->pNext;
		}
	}
	
	return nResult;
	
}

void	CtmPackClass::FreeRegFunc(tmRegFunc* pRegs)
{
	if (pRegs == NULL)
		return;
	else
	{
		FreeRegFunc(pRegs->pNext);		
		delete pRegs;
		return;
	}
}

int		CtmPackClass::CreateSelf()
{
	m_pRegFunc 	= NULL;
}

int		CtmPackClass::FreeSelf()
{
	FreeRegFunc(m_pRegFunc);
}
