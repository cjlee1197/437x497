#include	"showstyle.h"
#include	"wnd.h"

/*===========================================================================+
|           Class implementation - CtmDisplayStyle                           |
+===========================================================================*/
CtmDisplayStyle::CtmDisplayStyle()
{
	
}


CtmDisplayStyle::~CtmDisplayStyle()
{
	
}

void	CtmDisplayStyle::Init(void* pAryWnd[], int nWndNum)
{
	return;
}

void	CtmDisplayStyle::DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum)
{

		return;
}

void	CtmDisplayStyle::DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum)
{

		return;
}
/*===========================================================================+
|           Class implementation - CtmWndStyle		                         |
+===========================================================================*/
CtmWndStyle::CtmWndStyle(): CtmDisplayStyle()
{
	memset(m_pAryWnds,	0, sizeof(m_pAryWnds));
	memset(m_nNums,		0, sizeof(m_nNums));
	memset(m_nOldColors,0, sizeof(m_nOldColors));
	m_nCurrentGroup = 0;

}

CtmWndStyle::~CtmWndStyle()
{
	
}

bool	CtmWndStyle::Check(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	if (pSource == NULL) 	return false;
	
	return Check(pAryWnd, nShowColor, nWndNum);
	
}

bool	CtmWndStyle::Check(void* pAryWnd[], int nShowColor, int nWndNum)
{
	if (pAryWnd == NULL) 	return false;
		
	if (nWndNum <= 0) 		return false;
		
	return true;
}

void*	CtmWndStyle::FindComponent(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	CtmWnd*	pWnd = NULL;

	m_nCurrentGroup = GetCurrentGroup(pAryWnd, nWndNum);
	if (m_nCurrentGroup < 0) return NULL;

	long long* pllValues = new long long[m_nNums[m_nCurrentGroup]];
	int*		pIndexs	 = new int[m_nNums[m_nCurrentGroup]];

	//	Get the all Component's value
	long long llValue = 0;
	for (int i = 0; i < m_nNums[m_nCurrentGroup]; i++) 
	{
		if (NULL == m_pAryWnds[m_nCurrentGroup][i]) 
		{
			pllValues[i] = 0;
		}
		else
		{
			pWnd = static_cast<CtmWnd*>(m_pAryWnds[m_nCurrentGroup][i]);

			if (pWnd->GetPropValueT("value", &llValue)) 
				pllValues[i] = llValue;
			else 
				pllValues[i] = 0;
		}
		pIndexs[i] = i;
	}

	//	Sort From min to max
	long long llTemp = 0;
	int		  nTemp	 = 0;
	for (int i = 0; i < m_nNums[m_nCurrentGroup]; i++) 
	{
		for (int j = i + 1; j < m_nNums[m_nCurrentGroup]; j++) 
		{
			if (pllValues[j] < pllValues[i]) 
			{
				llTemp 		 = pllValues[i];
				pllValues[i] = pllValues[j];
				pllValues[j] = llTemp;

				nTemp		 = pIndexs[i];
				pIndexs[i]	 = pIndexs[j];
				pIndexs[j]	 = nTemp;

			}
		}
	}

	nTemp = 0;
	for (int i = 0; i < m_nNums[m_nCurrentGroup]; i++) 
	{
		if (llSource >= pllValues[i]) 
		{
			if (i < m_nNums[m_nCurrentGroup] - 1) 
			{
				if (pllValues[i + 1] > llSource) 
				{
					nTemp = i;
					break;
				}
			}
			else 
			{
				nTemp = i;
				break;
			}
		}
	}


	nTemp = pIndexs[nTemp];
	delete []pllValues;
	delete []pIndexs;

	return m_pAryWnds[m_nCurrentGroup][nTemp];

}

void*	CtmWndStyle::FindComponent(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	char*	pszDBID = static_cast<char*>(pSource);
	

	long long llDbValue = GetDBValue(pszDBID).lValue;

	return FindComponent(llDbValue, pAryWnd, nShowColor, nWndNum);
}

int		CtmWndStyle::SetOldColors(int nIndex, void* pExcept)
{
	int		nResult = 0;

	CtmWnd*	pWnd = NULL;
	int	nColor = 0;
	for (int i = 0; i < m_nNums[nIndex]; i++) 
	{
		if (pExcept != m_pAryWnds[nIndex][i] && m_pAryWnds[nIndex][i] != NULL) 
		{
			pWnd = static_cast<CtmWnd*>(m_pAryWnds[nIndex][i]);
			pWnd->GetPropValueT("bgc", &nColor);
			if (nColor != m_nOldColors[nIndex][i]) 
			{
				pWnd->SetPropValueT("bgc", m_nOldColors[nIndex][i]);
				pWnd->Update();
				nResult++;
			}
            
		}
	}
	return nResult;

}

bool	CtmWndStyle::SetColor(int nShowColor, void* pDest)
{
	bool bResult = false;
	
	if (pDest == NULL) return bResult;
		
	CtmWnd*	pWnd = static_cast<CtmWnd*>(pDest);
	

	BOOL	bEnabled = FALSE;
	WORD	wType	 = 0;
	pWnd->GetPropValueT("wtype", &wType);
	pWnd->GetPropValueT("enabled", &bEnabled);

	//	disvisible
	if (!(wType & tmWS_SHOW)) 
	{
		bResult = false;
	}
	else
	{
		if (!bEnabled) 
			pWnd->SetPropValueT("enabled", (double)TRUE);

		pWnd->SetPropValueT("bgc", nShowColor);
		pWnd->Update();

		if (!bEnabled) 
		{
			pWnd->SetPropValueT("enabled", (double)FALSE);
		}
		bResult = true;
		
	}

	return bResult;
}

int		CtmWndStyle::GetCurrentGroup(void* pAryWnd[], int nWndNum)
{
	return InitGroup(pAryWnd, nWndNum);
}

int		CtmWndStyle::InitGroup(void* pAryWnd[], int nWndNum)
{
	int nResult = -1;

	if (pAryWnd == NULL || nWndNum <= 0) return nResult;

	if (nWndNum >= CNT_MAX_EXACT_COMPONENT) nWndNum = CNT_MAX_EXACT_COMPONENT - 1;
	for (int i = 0; i < CNT_MAX_GROUP; i++) 
	{
		
		if (m_pAryWnds[i][0] == NULL || m_pAryWnds[i][0] == pAryWnd[0]) 
		{
			nResult = i;
			m_nNums[i] = nWndNum;
			break;
		}
	}

	for (int i = 0; i < nWndNum; i++) 
	{
		m_pAryWnds[nResult][i] = pAryWnd[i];
	}

	return nResult;


}

//	return get the old color's num
int		CtmWndStyle::InitColors(int nGroup, void* pAryWnd[], int nWndNum)
{
	int nResult = 0;

	if (nWndNum >= CNT_MAX_EXACT_COMPONENT) nWndNum = CNT_MAX_EXACT_COMPONENT - 1;
	CtmWnd*	pWnd 		= NULL;
	int		nOldColor 	= 0;

	for (int i = 0; i < nWndNum; i++)
	{
		if (m_pAryWnds[nGroup][i] != NULL)
		{
			pWnd = static_cast<CtmWnd*>(m_pAryWnds[nGroup][i]);
			pWnd->GetPropValueT("bgc", &nOldColor);
			m_nOldColors[nGroup][i] = nOldColor;
			nResult++;
		}
	}

	return nResult;

}

void	CtmWndStyle::Init(void* pAryWnd[], int nWndNum)
{
	int	nGroup = InitGroup(pAryWnd, nWndNum);

	if (nGroup >= CNT_MAX_GROUP || nGroup < 0) return;

	InitColors(nGroup,pAryWnd,nWndNum);
}


void	CtmWndStyle::DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	if (!Check(pSource, pAryWnd, nShowColor, nWndNum)) return;
	else
	{
		long long llValue = GetDBValue(static_cast<char*>(pSource)).lValue;
		DisplayColor(llValue, pAryWnd, nShowColor, nWndNum);
	}
}

void	CtmWndStyle::DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	if (!Check(pAryWnd, nShowColor, nWndNum)) 
	{
		return;
	}
	else
	{
		void*	pWnd = NULL;
		pWnd = FindComponent(llSource, pAryWnd, nShowColor, nWndNum);		
		SetColor(nShowColor, pWnd);
		SetOldColors(m_nCurrentGroup, pWnd);
	}

}
CtmStyleFrame0::CtmStyleFrame0(): CtmDisplayStyle()
{
	m_pStyle = NULL;
}

CtmStyleFrame0::CtmStyleFrame0(CtmDisplayStyle* pStyle): CtmDisplayStyle()
{
	m_pStyle = pStyle;
}

CtmStyleFrame0::~CtmStyleFrame0()
{
	
}


void	CtmStyleFrame0::SetStyle(CtmDisplayStyle* pStyle)
{
	m_pStyle = pStyle;
}

void	CtmStyleFrame0::Init(void* pAryWnd[], int nWndNum)
{
	if (m_pStyle != NULL) 
	{
		m_pStyle->Init(pAryWnd,nWndNum);
	}
	return;

}

void	CtmStyleFrame0::DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	if (m_pStyle != NULL) 
	{
		m_pStyle->DisplayColor(pSource, pAryWnd, nShowColor, nWndNum);
	}
	return;
}

void	CtmStyleFrame0::DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	if (m_pStyle != NULL) 
	{
		m_pStyle->DisplayColor(llSource, pAryWnd, nShowColor, nWndNum);
	}
	return;
}

/*===========================================================================+
|           Class implementation - CtmStyleFrame	                         |
+===========================================================================*/
CtmStyleFrame::CtmStyleFrame(): CtmStyleFrame0()
{
	
}

CtmStyleFrame::CtmStyleFrame(CtmDisplayStyle* pStyle): CtmStyleFrame0(pStyle)
{
	
}

CtmStyleFrame::~CtmStyleFrame()
{
	
}

void	CtmStyleFrame::GetOldColors(void* pAryWnd[], int nWndNum)
{
	if (nWndNum >= CNT_MAX_EXACT_COMPONENT) 
	{
		nWndNum = CNT_MAX_EXACT_COMPONENT - 1;
	}

	CtmWnd*	pWnd = NULL;
	for (int i = 0; i < nWndNum; i++) 
	{
		if (pAryWnd[i] != NULL)
		{
			pWnd = static_cast<CtmWnd*>(pAryWnd[i]);
			pWnd->GetPropValueT("bgc", &m_nOldColors[i]);
		}
	}
}

void	CtmStyleFrame::SetOldColors(void* pAryWnd[], int nWndNum)
{
	if (nWndNum >= CNT_MAX_EXACT_COMPONENT) 
	{
		nWndNum = CNT_MAX_EXACT_COMPONENT - 1;
	}

	CtmWnd*	pWnd = NULL;

	BOOL	bEnabled = FALSE;
	WORD	wType	 = 0;
	DWORD	dwBgc	 = 0;

	for (int i = 0; i < nWndNum; i++) 
	{
		bEnabled = FALSE;
		wType	 = 0;
		dwBgc	 = 0;
		if (pAryWnd[i] != NULL)
		{
			pWnd = static_cast<CtmWnd*>(pAryWnd[i]);

			pWnd->GetPropValueT("wtype", &wType);
			pWnd->GetPropValueT("enabled", &bEnabled);
			pWnd->GetPropValueT("bgc", &dwBgc);


			if (!(wType & tmWS_SHOW)) 
			{
				continue;
			}
			if (!bEnabled)	pWnd->SetPropValueT("enabled", (double)TRUE);

			if (dwBgc != m_nOldColors[i]) 
			{
				pWnd->SetPropValueT("bgc", m_nOldColors[i]);
				pWnd->Update();
			}

			if (!bEnabled) 
			{
				pWnd->SetPropValueT("enabled", (double)FALSE);
			}
		}
	}
}


void	CtmStyleFrame::DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	GetOldColors(pAryWnd, nWndNum);
	CtmStyleFrame0::DisplayColor(pSource, pAryWnd, nShowColor, nWndNum);
	usleep(30 * 1000);
	SetOldColors(pAryWnd, nWndNum);
	return;
}

void	CtmStyleFrame::DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum)
{
	GetOldColors(pAryWnd, nWndNum);
	CtmStyleFrame0::DisplayColor(llSource, pAryWnd, nShowColor, nWndNum);
	usleep(30 * 1000);
	SetOldColors(pAryWnd, nWndNum);
	return;
}

