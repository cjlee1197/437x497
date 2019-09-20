/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CMulStrings					   				  					|
|  Task     : CMulStrings header file                   			      		|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                   |
|  Link     :G++(GCC) 3.2                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : shousu fan                                                   	|
|  Version  : V1.00                                                         |
|  Creation : 2006                                        	             	|
|  Revision :                                                               |
+==========================================================================*/


#include "multilanguage.h"
#include "stable.h"
#include "tmconfig.h"       //netdigger 2007/4/5
#include "cfont.h"          //netdigger 2007/4/5

const		char*		LANGUAGE_STRING = "string";


CMulStrings::CMulStrings(MULSTRINFO* pInfo)
{
	InitData();
	ReadMulStrings(pInfo);
}

CMulStrings::CMulStrings()
{
	InitData();
}

CMulStrings::~CMulStrings()
{
	if (m_pStrs != NULL) 
	{
		delete m_pStrs;
		m_pStrs = NULL;
	}
	
	for (int i = 0; i < MULSTR_MAXNUM; i++)
	{
		if (m_pszMulStrID[i] != NULL)	//	when m_pszMulStrID is null, m_pszMulStr is null too
		{
			delete []m_pszMulStrID[i];
			delete []m_pszMulStr[i];
		}
		else break;
	}
	
}



/*---------------------------------------------------------------------------+
|  Function :              AppendString    		                             |
|  Task     :	將字串ID和字串對應起來，且保存								 |
+----------------------------------------------------------------------------+
|  Call     :	void CMulStrings::AppendString(char* pszKey, char* pszValue) |
|				從實際應用角度上看，一般不會直接調用此函數					 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  												                 |
+---------------------------------------------------------------------------*/
void CMulStrings::AppendString(char* pszKey, char* pszValue)
{
	int iTemp;
	iTemp = m_pStrs->GetStrCount();
	if (m_pszMulStrID[iTemp] != NULL) delete []m_pszMulStrID[iTemp];
	if (m_pszMulStr[iTemp] != NULL) delete []m_pszMulStr[iTemp];
	m_pszMulStrID[iTemp] = new char[strlen(pszKey) + 1];
	m_pszMulStr[iTemp] = new char[strlen(pszValue) + 1];
	strcpy(m_pszMulStrID[iTemp], pszKey);
	strcpy(m_pszMulStr[iTemp], pszKey);
	
	m_pStrs->AddString(m_pszMulStrID[iTemp]);
	
}


/*---------------------------------------------------------------------------+
|  Function :              GetString    		                             |
|  Task     :	根據給定的字串ID而獲取字串									 |
+----------------------------------------------------------------------------+
|  Call     :	void CMulStrings::AppendString(char* pszKey, char* pszValue) |
|				從實際應用角度上看，一般不會直接調用此函數					 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  	如果沒有找到的話，返回NULL					                 |
+---------------------------------------------------------------------------*/
char* CMulStrings::GetString(char* pszKey)
{	
	int index;
	
	if (pszKey != NULL)
	{
		int len = strlen(pszKey);
		//remove right space
		for (int i = len - 1; i >= 0; i--)
		{
			if ((pszKey[i] == ' ') || (pszKey[i] == '\t'))  pszKey[i] = '\0';
			else break;
		}
		
		//remove left space
		for (int i = 0; i < len; i++)
		{
			if ((pszKey[i] == ' ') || (pszKey[i] == '\t')) pszKey++;	
			else break;
		}
	}
	
		
	
	
	index = m_pStrs->GetIndex(pszKey);
	return GetString(index);
	/*
	if (index >= 0) 
	{
		return m_pszMulStr[index];
	}
	else return NULL;	
	*/
}

char*	CMulStrings::GetString(int	nIndex)
{
	if (nIndex < 0) return NULL;
	
//	char*	pszTemp = m_pszMulStr[nIndex];

	//return m_pszMulStr[nIndex];
	return m_pszMulStr[nIndex];//netdigger 2007/4/9
//	strncpy(m_szOut, pszTemp, sizeof(m_szOut));
//	return m_szOut;
}


int	CMulStrings::GetStrKeyIndex(char* pszKey)
{
	int result = -1;
	result = m_pStrs->GetIndex(pszKey);
	return result;
}

char* CMulStrings::operator[] (char* pszKey)
{	
	
	return GetString(pszKey);	
}

char*	CMulStrings::operator[] (int nIndex)
{
	return	GetString(nIndex);
}

/*---------------------------------------------------------------------------+
|  Function :              InitData	    		                             |
|  Task     :	初始化字串的ID和字串										 |
+----------------------------------------------------------------------------+
|  Call     :																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  												                 |
+---------------------------------------------------------------------------*/
void CMulStrings::InitData()
{
	m_pStrs = NULL;	
	for (int i = 0; i < MULSTR_MAXNUM; i++)
	{
		m_pszMulStrID[i] = NULL;
		m_pszMulStr[i] = NULL;		
	}
}



/*---------------------------------------------------------------------------+
|  Function :              ReadMulStrings 		                             |
|  Task     :	從指定的文件中獲取字串信息（包括字串的ID）					 |
+----------------------------------------------------------------------------+
|  Call     :																 |
|                                                                            |
|  Parameter:   MULSTRINFO* pInfo:是字串文件的信息                           |
|                                                                            |
|  Return   :  	讀取的字串數								                 |
+---------------------------------------------------------------------------*/
int CMulStrings::ReadMulStrings(MULSTRINFO* pInfo)
{
	char temp[1024];
	int iResult = 0;	
	
	if (pInfo != NULL)
	{		
		strcpy(temp, pInfo->strDir);
		strcat(temp, pInfo->strFileName);
		if(access(temp,F_OK))		return 	iResult;		
		m_strFile.OpenFile(temp);
		iResult = 	m_strFile.ReadSection(LANGUAGE_STRING, m_pszMulStrID, 0);
		m_strFile.ReadSectionValues(LANGUAGE_STRING, m_pszMulStr, 0);
		if (m_pStrs == NULL) 
		{
			m_pStrs = new CtmStrings;		
			for (int i = 0; i < iResult; i++) 
			{
				m_pStrs->AddString(m_pszMulStrID[i]);
			}
		}
		m_strFile.CloseFile();		
	}
	
	return iResult;
}

//	fans add 2007/3/21 05:57下午

/*==========================================================================+
|           Class declaration - CtmMultiLanguage                            |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmMultiLanguage::CtmMultiLanguage(MULSTRINFO* pInfo)
{
//	m_szOut[0] = '\0';
	InitData();
	m_strFile.SetValueCode(pInfo->nValueCode);
	ReadMulStrings(pInfo);
}

CtmMultiLanguage::CtmMultiLanguage()
{
//	m_szOut[0] = '\0';
	InitData();
}

CtmMultiLanguage::~CtmMultiLanguage()
{
	if (m_pStrs != NULL) 
	{
		delete m_pStrs;
		m_pStrs = NULL;
	}
	
	for (int i = 0; i < MULSTR_MAXNUM; i++)
	{
		if (m_pszMulStrID[i] != NULL)	//	when m_pszMulStrID is null, m_pszMulStr is null too
		{
			delete []m_pszMulStrID[i];
			delete []m_pszMulStr[i];
		}
		else break;
	}
	
}



/*---------------------------------------------------------------------------+
|  Function :              AppendString    		                             |
|  Task     :	將字串ID和字串對應起來，且保存								 |
+----------------------------------------------------------------------------+
|  Call     :	void CtmMultiLanguage::AppendString(char* pszKey, char* pszValue) |
|				從實際應用角度上看，一般不會直接調用此函數					 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  												                 |
+---------------------------------------------------------------------------*/
void CtmMultiLanguage::AppendString(char* pszKey, char* pszValue)
{
	//int iTemp;
	//iTemp = m_pStrs->GetStrCount();
	//if (m_pszMulStrID[iTemp] != NULL) delete []m_pszMulStrID[iTemp];
	//if (m_pszMulStr[iTemp] != NULL) delete []m_pszMulStr[iTemp];
	//m_pszMulStrID[iTemp] = new char[strlen(pszKey) + 1];
	//m_pszMulStr[iTemp] = new char[strlen(pszValue) + 1];
	//strcpy(m_pszMulStrID[iTemp], pszKey);
	//strcpy(m_pszMulStr[iTemp], pszKey);
	//
	//m_pStrs->AddString(m_pszMulStrID[iTemp]);
	
}


/*---------------------------------------------------------------------------+
|  Function :              GetString    		                             |
|  Task     :	根據給定的字串ID而獲取字串									 |
+----------------------------------------------------------------------------+
|  Call     :						 										 |
|																			 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  	如果沒有找到的話，返回NULL					                 |
+---------------------------------------------------------------------------*/
char* CtmMultiLanguage::GetString(char* pszKey)
{	
	int index;
	
	if (pszKey != NULL)
	{
		int len = strlen(pszKey);
		//remove right space
		for (int i = len - 1; i >= 0; i--)
		{
			if ((pszKey[i] == ' ') || (pszKey[i] == '\t'))  pszKey[i] = '\0';
			else break;
		}
		
		//remove left space
		for (int i = 0; i < len; i++)
		{
			if ((pszKey[i] == ' ') || (pszKey[i] == '\t')) pszKey++;	
			else break;
		}
	}
	
		
	
	
	//index = m_pStrs->GetIndex(pszKey);
	if (m_pStrs->FindElement(pszKey, &m_ElementOut))
	{
		return GetString(m_ElementOut.uiIndex - 1);	
	}
	else return NULL;
	
	/*
	if (index >= 0) 
	{
		return m_pszMulStr[index];
	}
	else return NULL;	
	*/
}

int		CtmMultiLanguage::SetMultiCode(int nCode)
{
	m_nCode=nCode;
}

char* CtmMultiLanguage::operator[] (char* pszKey)
{
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
	{
		//printf("pszKey=%s\n",pszKey);
		if(m_nCode==0)
		{
			if(g_pParseXMLViewStr==NULL)
				return GetString(pszKey);
			int nIndex = g_pParseXMLViewStr->GetRecordIndex(pszKey);
			//printf("nIndex=%d\n",nIndex);
			if(nIndex>=0)
			{
				char* pszutf8=g_pParseXMLViewStr->GetRecordStr(nIndex).putf8code;
				if(pszutf8==NULL)
				{	
					return pszKey;
				}
				return pszutf8;
			}
			else
			{
				//printf("pszKey=%s\n",pszKey);
				return pszKey;	
			}
		}
		else
		{
			char* str=GetString(pszKey);
			SetMultiCode(0);
			return str;
		}
	}
	else
	{
		return GetString(pszKey);
	}	
}

/*---------------------------------------------------------------------------+
|  Function :              GetString    		                             |
|  Task     :	根據給定的字串index而獲取字串									 |
+----------------------------------------------------------------------------+
|  Call     :						 										 |
|																			 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  	如果沒有找到的話，返回NULL					                 |
+---------------------------------------------------------------------------*/
char*	CtmMultiLanguage::GetString(int	nIndex)
{
	if (nIndex < 0) return NULL;
	
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
	{
		if(m_nCode==0)
		{
			if(g_pParseXMLViewStr!=NULL)
				return g_pParseXMLViewStr->GetRecordStr(nIndex+1).putf8code;
			else
				return m_pszMulStr[nIndex];
		}
		else
			return m_pszMulStr[nIndex];
	}
	else
	{
		return m_pszMulStr[nIndex];	
	}
	
//	char*	pszTemp = m_pszMulStr[nIndex];

	//return m_pszMulStr[nIndex];
//	return m_pszMulStr[nIndex];     //netdigger 2007/4/10
//	strncpy(m_szOut, pszTemp, sizeof(m_szOut));
//	return m_szOut;
}

char*	CtmMultiLanguage::operator[] (int nIndex)
{
		return	GetString(nIndex);	
}

char*	CtmMultiLanguage::GetStringID(int	nIndex)
{
	if (nIndex < 0) return NULL;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
	{
		if(g_pParseXMLViewStr)
			return g_pParseXMLViewStr->GetRecordStrID(nIndex+1);
		return m_pszMulStrID[nIndex];
	}
	else
		return m_pszMulStrID[nIndex];
}

/*---------------------------------------------------------------------------+
|  Function :   GetStrKeyIndex(char* pszKey)	                             |
|  Task     :	根據給定的字串ID而獲取字串									 |
+----------------------------------------------------------------------------+
|  Call     :						 										 |
|																			 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  	如果沒有找到的話，返回NULL					                 |
+---------------------------------------------------------------------------*/
int	CtmMultiLanguage::GetStrKeyIndex(char* pszKey)
{
	int result = -1;
	//result = m_pStrs->GetIndex(pszKey);
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
	{
		if(g_pParseXMLViewStr)
		{
			result=g_pParseXMLViewStr->GetRecordIndex(pszKey)-1;
		}
		else
		{
			if (m_pStrs->FindElement(pszKey, &m_ElementOut))
				result = m_ElementOut.uiIndex - 1;	
		}
				
	}
	else
	{
		if (m_pStrs->FindElement(pszKey, &m_ElementOut))
			result = m_ElementOut.uiIndex - 1;
	}
	return result;
}

/*---------------------------------------------------------------------------+
|  Function :              InitData	    		                             |
|  Task     :	初始化字串的ID和字串										 |
+----------------------------------------------------------------------------+
|  Call     :																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :  												                 |
+---------------------------------------------------------------------------*/
void CtmMultiLanguage::InitData()
{
	m_nCode=0;
	m_pStrs = NULL;	
	for (int i = 0; i < MULSTR_MAXNUM; i++)
	{
		m_pszMulStrID[i] = NULL;
		m_pszMulStr[i] = NULL;		
	}
}



/*---------------------------------------------------------------------------+
|  Function :              ReadMulStrings 		                             |
|  Task     :	從指定的文件中獲取字串信息（包括字串的ID）					 |
+----------------------------------------------------------------------------+
|  Call     :																 |
|                                                                            |
|  Parameter:   MULSTRINFO* pInfo:是字串文件的信息                           |
|                                                                            |
|  Return   :  	讀取的字串數								                 |
+---------------------------------------------------------------------------*/
int CtmMultiLanguage::ReadMulStrings(MULSTRINFO* pInfo)
{
	char temp[1024];
	int iResult = 0;
	int i       = 0, j;
	unsigned short *pwCode;
	
	if (pInfo != NULL)
	{		
		strcpy(temp, pInfo->strDir);
		strcat(temp, pInfo->strFileName);
		m_strFile.SetValueCode(pInfo->nValueCode);
		if(access(temp,F_OK))		return 	iResult;
		m_strFile.OpenFile(temp);
		iResult = 	m_strFile.ReadSection(LANGUAGE_STRING, m_pszMulStrID, 0);
		m_strFile.ReadSectionValues(LANGUAGE_STRING, m_pszMulStr, 0);

		if (m_pStrs == NULL) 
		{
			m_pStrs = new CtmHashTable(iResult);		
			//m_pStrsValueToKey = new CtmHashTable;			
		}
		m_pStrs->ClearElement();
		tmHashElement* pTempElement = new tmHashElement;
		for (int i = 0; i < iResult; i++) 
		{
			pTempElement->pKey = m_pszMulStrID[i];
			pTempElement->pData = m_pszMulStr[i];
			pTempElement->pLNode = NULL;
			pTempElement->pRNode = NULL;
			if(CtmConfig::GetInstance()->GetFontCode() != tmFT_CODE_TECH)
			    pTempElement->uiSizeOfData = strlen(m_pszMulStr[i]) + 1;
			else
			{
			    pwCode  = (unsigned short *)m_pszMulStr[i];
			    j       = 0;
			    while(pwCode[j ++] != 0);
			    
			    pTempElement->uiSizeOfData = j << 1;			    
			}
			m_pStrs->AddElement(pTempElement);
		}
		delete pTempElement;
		m_strFile.CloseFile();		
	}
	
	return iResult;
}

BOOL	CtmMultiLanguage::SetMulStr(char*	pszKey,char*	pszMulStr)
{
	int	nIndexSrc = 0, nIndexDes = 0;
	char*	nTemp = NULL;
	
	nIndexSrc	= GetStrKeyIndex(pszKey);
	nIndexDes 	= GetStrKeyIndex(pszMulStr);
	
	if(MULSTR_MAXNUM > nIndexSrc && nIndexSrc > 0 && 
		MULSTR_MAXNUM > nIndexDes && nIndexDes > 0)
	{
		nTemp = m_pszMulStr[nIndexSrc];
		m_pszMulStr[nIndexSrc] = m_pszMulStr[nIndexDes];
		m_pszMulStr[nIndexDes] = nTemp;
	}
	else
		return FALSE;
	return TRUE;
}

