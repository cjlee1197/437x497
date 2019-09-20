#include	"readdata.h"
#include	"tmconfig.h"

IMPLEMENT_DYNCREATE(CtmReadData, CtmPackClass)
IMPLEMENT_DYNCREATE(CtmReadPuerData, CtmPackClass)

int		ReadDataStr(CtmWnd* pwndSource, char* pszOut[], int nNum )
{
	int nResult = 0;
	
	if (pwndSource != NULL)
	{
		char	sz[20];
		if (nNum > 0)
		{
			for (int i = 0; i < nNum; i++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					pwndSource->GetPropValueT(VALUESTR_VALUE0 + i, pszOut[i]);
				}
				else
				{
					sprintf(sz, "value%d", i);
					pwndSource->GetPropValueT(sz, pszOut[i]);
				}
				if (pszOut[i][0] != '\0') 
				{
					nResult++;
				}
			}
		}
		else
		{
			for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					pwndSource->GetPropValueT(VALUESTR_VALUE0 + i, pszOut[i]);
				}
				else
				{
					sprintf(sz, "value%d", i);
					pwndSource->GetPropValueT(sz, pszOut[i]);
				}
				if (pszOut[i][0] != '\0') 
				{
					nResult++;
				}
				else break;
			}			
			
		}
	}
	
	return nResult;
}

int		ReadDataInt(CtmWnd* pwndSource, int	nOut[],   int nNum)
{
	int nResult = 0;
	
	if (pwndSource != NULL)
	{
		char	sz[20];
		if (nNum > 0)
		{
			for (int i = 0; i < nNum; i++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					pwndSource->GetPropValueT(VALUEINT_VALUE0 + i, &nOut[i]);
				}
				else
				{
					sprintf(sz, "value%d", i);
					pwndSource->GetPropValueT(sz, &nOut[i]);
				}
				//printf("1 sz=%snOut[%d]=%x \n", sz, i, nOut[i]);
				if (nOut[i] != (int)0xFFFFFFFF) nResult++;
			}
		}
		else
		{
			for (int i = 0; i < CNT_INT_MAX_VALUES_NUM; i++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					pwndSource->GetPropValueT(VALUEINT_VALUE0 + i, &nOut[i]);
				}
				else
				{
					sprintf(sz, "value%d", i);
					pwndSource->GetPropValueT(sz, &nOut[i]);
				}
				//printf("2 sz=%s nOut[%d]=%x \n", sz, i, nOut[i]);
				if (nOut[i] != (int)0xFFFFFFFF) nResult++;
			}
		}
	}
	
	return nResult;
}

CtmReadData::CtmReadData(): CtmPackClass()
{
	m_pwndStr	= NULL;
	m_pwndInt	= NULL;
}

CtmReadData::~CtmReadData()
{
	
}


CtmWnd*		CtmReadData::OpenFile(CParseObject* pParse, char* pszFileName)
{
	CtmWnd*	pwnd = NULL;
	//if (CheckFileExist(pszFileName))	//	 the file is exist
	if(pParse->OpenFile(pszFileName))
	{
		//pParse->OpenFile(pszFileName);
		pParse->CloseFile();	
		pwnd 	= pParse->m_aryControls[0].control;
	}
	
	return pwnd;	
}

BOOL	CtmReadData::OpenFileStr(char* pszFileName)
{
	BOOL	bResult = FALSE;
	
	m_pwndStr 	= OpenFile(&m_ParseStr, pszFileName);
	bResult		= (m_pwndStr != NULL);	
	return bResult;	
}

BOOL	CtmReadData::OpenFileInt(char* pszFileName)
{
	BOOL	bResult = FALSE;	
	m_pwndInt 	= OpenFile(&m_ParseInt, pszFileName);
	bResult		= (m_pwndInt != NULL);	
	return bResult;	
}
	

int		CtmReadData::ReadData(char* pszOut[], int nNum)
{
	return ReadDataStr(m_pwndStr, pszOut, nNum);
}

int		CtmReadData::ReadData(int	nOut[],   int nNum)
{
	return ReadDataInt(m_pwndInt, nOut, nNum);
}
	

CtmWnd* CtmReadData::GetControlStr()
{
	return m_pwndStr;
}

CtmWnd* CtmReadData::GetControlInt()
{
	return	m_pwndInt;
}

BOOL		CtmReadData::CheckFileExist(char* pszFileName)
{
	BOOL	bResult = FALSE;
	
	if (pszFileName != NULL)
	{
		int nFd = open(pszFileName, O_RDONLY);		
		if (nFd > 0)
		{
			bResult = TRUE;
			close(nFd);
		}
	}
	
	return bResult;
}


CtmReadPuerData::CtmReadPuerData(): CtmPackClass()
{
	m_pwnd = NULL;
}

CtmReadPuerData::~CtmReadPuerData()
{
	
}

int		CtmReadPuerData::GetNum()
{
	if (m_pwnd == NULL) return 0;
	else
	{
		int nResult = 0;
		CtmWnd* pwnd = NULL;
		
		char	sz[64];
		
		if (m_nFileType == 1)				//	CtmValueInt
		{
			int	nTemp = 0;
			
			//	get the self's propertys
			for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					m_pwnd->GetPropValueT(VALUEINT_VALUE0 + j, &nTemp);
				}
				else
				{
					sprintf(sz, "value%d", j);
					m_pwnd->GetPropValueT(sz, &nTemp);
				}
				if (nTemp != (int)0xFFFFFFFF) 
					nResult++;
				else
					break;
			}
			
			//	get the propertys from the components of it
			for (int i = 0; i < m_pwnd->GetControlCount(); i++)
			{
				pwnd = m_pwnd->Controls[i];
				
				for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
				{
					if(CtmConfig::GetInstance()->GetBinaryType() == 1)
					{
						pwnd->GetPropValueT(VALUEINT_VALUE0 + j, &nTemp);
					}
					else
					{
						sprintf(sz, "value%d", j);
						pwnd->GetPropValueT(sz, &nTemp);
					}
					if (nTemp != (int)0xFFFFFFFF) 
						nResult++;
					else
						break;
				}
				
			}
			
		}
		else if (m_nFileType == 2)		//	CtmValueStr
		{
			char	szValue[64];
			
			for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					m_pwnd->GetPropValueT(VALUESTR_VALUE0 + j, szValue);
				}
				else
				{
					sprintf(sz, "value%d", j);
					m_pwnd->GetPropValueT(sz, szValue);
				}
				if (szValue[0] != '\0') 
					nResult++;
				else
					break;
			}
			
			for (int i = 0; i < m_pwnd->GetControlCount(); i++)
			{
				pwnd = m_pwnd->Controls[i];
				
				for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
				{
					if(CtmConfig::GetInstance()->GetBinaryType() == 1)
					{
						pwnd->GetPropValueT(VALUESTR_VALUE0 + j, szValue);
					}
					else
					{
						sprintf(sz, "value%d", j);
						pwnd->GetPropValueT(sz, szValue);
					}
					if (szValue[0] != '\0') 
						nResult++;
					else
						break;
				}
				
			}
			
		}
		else nResult = 0;
		
		
		return nResult;
	}
	
}

int		CtmReadPuerData::FileType()
{
	
	if (m_pwnd == NULL) return 0;
	else
	{
		if (m_pwnd->Is("CtmValueInt")) 		return 1;
		else if (m_pwnd->Is("CtmValueStr"))	return 2;
		else 								return 0;
	}
}

CtmWnd*	CtmReadPuerData::Open(char* pszFileName)
{
	if (m_Parse.OpenFile(pszFileName))	//	 the file is exist
	{
		//m_Parse.OpenFile(pszFileName);
		m_Parse.CloseFile();	
		m_pwnd 	= m_Parse.m_aryControls[0].control;
	}
	else m_pwnd = NULL;
	m_nFileType = FileType();
	
	return m_pwnd;
}

int		CtmReadPuerData::Read(char*	pszOut[], 	int nNum)
{
	if (m_nFileType != 2) return 0;
	else
	{
		int 	nResult = 0;
		CtmWnd*	pwnd = NULL;		
		char	szValue[64];
		char	sz[20];
			
		for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
		{
			if(CtmConfig::GetInstance()->GetBinaryType() == 1)
			{
				m_pwnd->GetPropValueT(VALUESTR_VALUE0 + j, szValue);
			}
			else
			{
				sprintf(sz, "value%d", j);
				m_pwnd->GetPropValueT(sz, szValue);
			}
			if (szValue[0] != '\0') 
			{
				strcpy(pszOut[nResult], szValue);
				nResult++;
				if (nNum > 0 && nResult >= nNum) break;
			}
			else
				break;
		}
		
		for (int i = 0; i < m_pwnd->GetControlCount(); i++)
		{
			pwnd = m_pwnd->Controls[i];
			for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					pwnd->GetPropValueT(VALUESTR_VALUE0 + j, szValue);
				}
				else
				{
					sprintf(sz, "value%d", j);
					pwnd->GetPropValueT(sz, szValue);
				}
				if (szValue[0] != '\0') 
				{
					strcpy(pszOut[nResult], szValue);
					nResult++;
					if (nNum > 0 && nResult >= nNum) break;
				}
				else
					break;
			}
		}
		
		return nResult;
	}
	
}

int		CtmReadPuerData::Read(int	nOut[],		int nNum )
{
	if (m_nFileType != 1) return 0;
	else
	{
		CtmWnd*	pwnd 	= NULL;
		int nResult 	= 0;		
		int	nTemp		= 0;
		char	sz[20];
		for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
		{
			if(CtmConfig::GetInstance()->GetBinaryType() == 1)
			{
				m_pwnd->GetPropValueT(VALUEINT_VALUE0 + j, &nTemp);
			}
			else
			{
				sprintf(sz, "value%d", j);
				m_pwnd->GetPropValueT(sz, &nTemp);
			}
			if (nTemp != (int)0xFFFFFFFF) 
			{
				nOut[nResult] = nTemp;
				nResult++;
				if (nNum > 0 && nResult >= nNum) break;
			}
			else
				break;
		}
		
		for (int i = 0; i < m_pwnd->GetControlCount(); i++)
		{
			pwnd = m_pwnd->Controls[i];
			for (int j = 0; j < CNT_INT_MAX_VALUES_NUM; j++)
			{
				if(CtmConfig::GetInstance()->GetBinaryType() == 1)
				{
					m_pwnd->GetPropValueT(VALUEINT_VALUE0 + i, &nTemp);
				}
				else
				{
					sprintf(sz, "value%d", i);
					pwnd->GetPropValueT(sz, &nTemp);
				}
				if (nTemp != (int)0xFFFFFFFF) 
				{
					nOut[nResult] = nTemp;
					nResult++;
					if (nNum > 0 && nResult >= nNum) break;
				}
				else
					break;
			}
		}
		
		return nResult;
	}
	
}

void	CtmReadPuerData::Close()
{
	//delete m_Parse.m_aryControls[0].control;
	delete m_pwnd;
	m_pwnd = NULL;
	m_Parse.m_aryControls[0].control = NULL;
	m_nFileType	= 0;
}


BOOL	CtmReadPuerData::CheckFileExist(char* pszFileName)
{
	BOOL	bResult = FALSE;
	
	if (pszFileName != NULL)
	{
		int nFd = open(pszFileName, O_RDONLY);		
		if (nFd > 0)
		{
			bResult = TRUE;	
			close(nFd);
		}
	}
	
	return bResult;
}


