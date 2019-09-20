/*==========================================================================+
|  Class    : Strings		                                            	|
|  Task     : Strings  file                          						|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/1/24                                                   	|
|  Revision : 			                                                    |
+==========================================================================*/
#include "strings.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
static const unsigned int f_iArrayPrime[] = {
	1			, 5			, 23		, 37		,
	53			, 97		, 193		, 389		,
	769			, 1543		, 3079		, 6151		,
	12289		, 24593		, 49157		, 98317		/*,
	196613		, 393241	, 786433	, 1572869	,
	3145739		, 6291469	, 12582917	, 25165843	,
	50331653	, 100663319	, 201326611	, 402653189	,
	805306457	, 1610612741*/
};
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - CStrings                                    |
+==========================================================================*/
CStrings::CStrings()
{
	m_nStrSize		= 0;
	m_nCount		= 0;
	m_nListCount	= 11;

	m_pStrings		= NULL;
	m_pStringList	= NULL;
	m_pStringIndex	= NULL;
}

CStrings::~CStrings()
{
	if(m_pStrings != NULL)			free(m_pStrings);
	if(m_pStringIndex	!= NULL)	free(m_pStringIndex);
	FreeList();
}

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function	:	Create(STRINGSINFO *pInfo)									|
|  Task		:	創建字串表													|
+---------------------------------------------------------------------------+
|  Call		:	nIndex	= GetNearPrime(pInfo)								|
|																			|
|  Parameter:	psz				-  字串										|
|																			|
|  Return	: 	nIndex			-  序號		-1為錯誤						|
+--------------------------------------------------------------------------*/
int		CStrings::Create(STRINGSINFO *pInfo)
{
	m_Info	= *pInfo;
	
	m_pStrings		= (char *)malloc(m_Info.nSize);
	m_nListCount	= GetNearPrime(m_Info.nCount);
	m_pStringList	= (STRING *)malloc(m_nListCount * sizeof(STRING));
	m_pStringIndex	= (char **)malloc(m_Info.nCount * sizeof(char *));
	
	memset(m_pStrings, 0, m_Info.nSize);
	memset(m_pStringList, 0, m_nListCount * sizeof(STRING));
	memset(m_pStringIndex, 0, m_Info.nCount * sizeof(char *));
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function	:	AddString(char *psz)										|
|  Task		:	添加一個字串												|
+---------------------------------------------------------------------------+
|  Call		:	nIndex	= AddString(psz)									|
|																			|
|  Parameter:	psz				-  字串										|
|																			|
|  Return	: 	nIndex			-  序號		-1為錯誤						|
+--------------------------------------------------------------------------*/
int		CStrings::AddString(char *psz)
{
	int		i, nCount;
	int		nIndex;
	int		nStrLen			= strlen(psz) + 1;
	STRING	info;

	if(psz == NULL)							return -1;
	if(m_pStrings	== NULL)				return -1;
	if(m_nStrSize + nStrLen > m_Info.nSize)	return -1;
	
	m_pStringIndex[m_nCount]	= m_pStrings + m_nStrSize;					//yang 2006/5/19 
	
	m_nStrSize += nStrLen;
	memcpy(m_pStringIndex[m_nCount], psz, nStrLen);
//	//printf("%s=%d\n", psz, m_nCount);
	info.nIndex		= m_nCount;
	info.pStr		= m_pStringIndex[m_nCount];
	info.pLNode		= NULL;
	info.pRNode		= NULL;
	
	if(m_nListCount > m_nCount)	nIndex	= AddList(&info);
	else
	{
		m_nListCount	= GetNearPrime(m_nCount);
		FreeList();
		m_pStringList	= (STRING *)calloc(m_nListCount, sizeof(STRING));
		memset(m_pStringList, 0, m_nListCount * sizeof(STRING));
		nCount = m_nCount;
		m_nCount = 0;
		for(i = 0; i < nCount; i++)
			AddString(m_pStringIndex[i]);
	}
	
	m_nCount ++;
	return nIndex;
}

/*--------------------------------------------------------------------------+
|  Function	:	GetIndex(char *psz)											|
|  Task		:	找出字串在字串列表中的序號									|
+---------------------------------------------------------------------------+
|  Call		:	nIndex	= GetIndex(psz)										|
|																			|
|  Parameter:	psz				-  字串										|
|																			|
|  Return	: 	nIndex			-  序號		-1為錯誤						|
+--------------------------------------------------------------------------*/
int		CStrings::GetIndex(char *psz)
{
	int		nResidue, nStrCmp;
	STRING	*pString;
	
	nResidue	= GetHash(psz) % m_nListCount;

	if(m_pStringList[nResidue].pStr == NULL)								//yang 2006/5/18
		return  -1;
	else if(strcmp(psz, m_pStringList[nResidue].pStr) == 0)						 
		return m_pStringList[nResidue].nIndex;
	
	pString	= m_pStringList + nResidue;
	
	do
	{
		nStrCmp = strcmp(psz, pString->pStr);
		if (nStrCmp == 0)		return	pString->nIndex;
		else if (nStrCmp > 0)	pString = pString->pRNode;
		else 					pString	= pString->pLNode;
	} while(pString != NULL);
	//printf("can not find str=%s\n", psz);
	return -1;
}

/*--------------------------------------------------------------------------+
|  Function	:	GetString(int nIndex)										|
|  Task		:	通過序號找出字串											|
+---------------------------------------------------------------------------+
|  Call		:	pStr	= GetIndex(nIndex)									|
|																			|
|  Parameter:	nIndex			-  字串序號									|
|																			|
|  Return	: 	pStr			-  字串指針									|
+--------------------------------------------------------------------------*/
char*		CStrings::GetString(int nIndex)
{
	if(nIndex >= m_nCount || nIndex < 0)	return NULL;
	return m_pStringIndex[nIndex];
}

/*--------------------------------------------------------------------------+
|  Function	:	GetHash(char *psz)											|
|  Task		:	取psz的哈希值												|
+---------------------------------------------------------------------------+
|  Call		:	nNumber	= GetHash(psz)										|
|																			|
|  Parameter:	nCount			-  需要計算的字串							|
|																			|
|  Return	: 	nNumber			-  哈希值，－1為錯誤						|
+--------------------------------------------------------------------------*/
int	CStrings::GetHash(char *psz)
{
	int i, strLen;
	int	Hash	= 0;
	
	if(psz == NULL)	return -1;
	strLen	= strlen(psz);	
	for(i = 0; i < strLen; i ++)
		Hash += (Hash ^ *(psz + i)) + (Hash << 26) + (Hash >> 6);
	
	return Hash & 0x7FFFFFFF;
}

/*--------------------------------------------------------------------------+
|  Function	:	GetNearPrime(int nCount)									|
|  Task		:	取离nCount最近的比nCount大的質數							|
+---------------------------------------------------------------------------+
|  Call		:	nNumber	= GetNearPrime(nCount)								|
|																			|
|  Parameter:	nCount			-   需要計算的數字							|
|																			|
|  Return	: 	nNumber			-  質數										|
+--------------------------------------------------------------------------*/
int	CStrings::GetNearPrime(int nCount)
{
	int i;
	if(nCount <0)			return 1;
	if(nCount <= 2)			return (nCount + 1);
	
	if(nCount % 2 != 0)	nCount += 2;
	else				nCount ++;
	
	i	= 2;
	nCount = nCount * 6 / 5;												//yang 2006/5/19 
	
	while(i <= (nCount >> 1) + 1)
	{
		if(nCount % i == 0)
		{
			nCount += 1;
			i		= 2;
			continue;
		}
		i ++;
	}
	
	return nCount;
}

/*--------------------------------------------------------------------------+
|  Function	:	AddList(STRING *pStrInfo)									|
|  Task		:	把字串加入哈希表											|
+---------------------------------------------------------------------------+
|  Call		:	nIndex	= AddList(pInfo)									|
|																			|
|  Parameter:	psz				-  字串										|
|																			|
|  Return	: 	nIndex			-  序號										|
+--------------------------------------------------------------------------*/
int		CStrings::AddList(STRING *pStrInfo)
{
	int		nResidue, nStrCmp;
	STRING	*pString 	= NULL;
	STRING	info		= *pStrInfo;
	STRING	*pPre		= NULL;
	
	nResidue	= GetHash(info.pStr) % m_nListCount;			//yang 2006/5/18 
	if(m_pStringList[nResidue].pStr == NULL)
	{
		m_pStringList[nResidue]			= info;
		m_pStringList[nResidue].pLNode	= NULL;
		m_pStringList[nResidue].pRNode	= NULL;
		
		return info.nIndex;
	}
	
	pString	= &m_pStringList[nResidue];
	do	{
		nStrCmp = strcmp(info.pStr, pString->pStr);		
		pPre = pString;
		if (nStrCmp > 0)			pString = pString->pRNode;
		else if (nStrCmp < 0)		pString = pString->pLNode;
		else return -1;
		} while (pString != NULL);
	
	pString	= (STRING *)malloc(sizeof(STRING));
	if (nStrCmp > 0)		pPre->pRNode = pString;
	else if (nStrCmp < 0)	pPre->pLNode = pString;
	*pString		= info;
	pString->pRNode = NULL;
	pString->pLNode = NULL;

	return info.nIndex;
}

/*--------------------------------------------------------------------------+
|  Function	:	FreeList()													|
|  Task		:	釋放哈希表													|
+---------------------------------------------------------------------------+
|  Call		:	FreeList()													|
|																			|
|  Parameter:					-  											|
|																			|
|  Return	: 					-  											|
+--------------------------------------------------------------------------*/
void	CStrings::FreeList()
{
	int		i;
	
	//=======
	// 釋放表格佔用的內存
	//=======
	if(m_pStringList != NULL)
	{
		for(i = 0; i < m_nListCount; i ++)								//yang 2006/5/19 
			{
			FreeNode(m_pStringList[i].pLNode);
			FreeNode(m_pStringList[i].pRNode);
			}
		free(m_pStringList);
	}
}

void	CStrings::FreeNode(STRING* pString)								//yang 2006/5/18 
{
	if (pString)
		{
		FreeNode(pString->pLNode);
		FreeNode(pString->pRNode);
		free(pString);
		}
}

//	fans add 2007/3/21 05:58下午
/*==========================================================================+
|           Class declaration - CtmHashTable                                |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmHashTable::CtmHashTable(unsigned int	uiNum)
{
	m_iHashSeed = GetPrime(uiNum);
	InitHash();
	
}

CtmHashTable::~CtmHashTable()
{
	FreeHash();
}

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  InitHash()													|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void			CtmHashTable::InitHash()
{
	memset(&m_HashTable, 0, sizeof(m_HashTable));
	m_HashTable.pElements = new tmHashElement[m_iHashSeed];
	memset(m_HashTable.pElements, 0, sizeof(tmHashElement) * m_iHashSeed);
}

/*--------------------------------------------------------------------------+
|  Function :  GetPrime(unsigned int uiNum)									|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
unsigned int	CtmHashTable::GetPrime(unsigned int uiNum)
{
	unsigned int uiResult 	= f_iArrayPrime[0];
	int			 iDif 		= abs(uiNum - f_iArrayPrime[0]);
	int			 iDif1		= iDif;
	for (int i = 0; i < sizeof(f_iArrayPrime) / sizeof(f_iArrayPrime[0]) - 1; i++)
	{
		
		iDif = abs(uiNum - uiResult);
		
		if (iDif > iDif1)
		{
			break;
		}
		iDif1 = iDif;
		uiResult = f_iArrayPrime[i];
		
	}
	return uiResult;
}

/*--------------------------------------------------------------------------+
|  Function :  FreeHash()													|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void			CtmHashTable::FreeHash()
{
	for (int i = 0; i < m_iHashSeed; i++)
	{
		if (m_HashTable.pElements[i].uiIndex == 0) continue;
		FreeNode(m_HashTable.pElements[i].pLNode);
		FreeNode(m_HashTable.pElements[i].pRNode);
	}
	delete []m_HashTable.pElements;
	m_HashTable.pElements = NULL;
}

/*--------------------------------------------------------------------------+
|  Function :  FreeNode(tmHashElement* pNode)								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void			CtmHashTable::FreeNode(tmHashElement* pNode)
{
	if (pNode != NULL)
	{
		FreeNode(pNode->pLNode);
		FreeNode(pNode->pRNode);
		
		delete pNode;
		pNode = NULL;
	}
	return;
}

/*--------------------------------------------------------------------------+
|  Function :  Hash(void iKey)												|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int				CtmHashTable::Hash(void* pKey)
{
	char* psz = (char*)pKey;	
	int i, strLen;
	int	Hash	= 0;
	
	if(psz == NULL)	return -1;
	strLen	= strlen(psz);	
	for(i = 0; i < strLen; i ++)
		Hash += (Hash ^ *(psz + i)) + (Hash << 26) + (Hash >> 6);
	
	return (Hash & 0x7FFFFFFF) % m_iHashSeed;
}

/*--------------------------------------------------------------------------+
|  Function :  FindElement(void* pKey, tmHashElement* pOutElement)			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL			CtmHashTable::FindElement(void* pKey, tmHashElement* pOutElement)
{
	BOOL	bResult = FALSE;
	int		iHash;
	iHash 	= Hash(pKey);
	int	iEqual = 0;
	
	if (pKey != NULL)
	{
		
		if (m_HashTable.pElements[iHash].uiIndex != 0)
		{
			iEqual = EqualKey(pKey, m_HashTable.pElements[iHash].pKey);
			
			if (iEqual == 0) 
			{
				memcpy(pOutElement, &m_HashTable.pElements[iHash], sizeof(tmHashElement));
				bResult = TRUE;
			}
			else
			{
				tmHashElement*	pTempElement = &m_HashTable.pElements[iHash];
				
				do 
				{
					iEqual = EqualKey(pKey, pTempElement->pKey);
					
					if (iEqual == 0) 
					{
						memcpy(pOutElement, pTempElement, sizeof(tmHashElement));
						bResult = TRUE;
						break;
					}
					else if (iEqual < 0) pTempElement = pTempElement->pLNode;
					else pTempElement = pTempElement->pRNode;
					
				} while (pTempElement != NULL);
				
			}
		}
		else bResult = FALSE;
		
	}
	else bResult = FALSE;
	
	return bResult;
}

/*--------------------------------------------------------------------------+
|  Function :  EqualKey(void* pKey, void* pHashKey)							|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int				CtmHashTable::EqualKey(void* pKey, void* pHashKey)
{
	int		iResult = 0;
	char* 	pszTempKey = (char*)pKey;
	char*	pszHashKey = (char*)pHashKey;
	iResult = strcmp(pszTempKey, pszHashKey);
	return iResult;
}

/*--------------------------------------------------------------------------+
|  Function :  FindElementEmptyPos(void* pKey)								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
tmHashElement*	CtmHashTable::FindElementEmptyPos(void* pKey)
{
	tmHashElement* pResult = NULL;
	
	int		iHash;
	iHash 	= Hash(pKey);
	int	iEqual = 0;
	
	if (pKey == NULL) pResult = NULL;
	else
	{
		if (m_HashTable.pElements[iHash].uiIndex == 0)	
		{
			pResult = &m_HashTable.pElements[iHash];	//	not find
		}
		else
		{
			iEqual = EqualKey(pKey, m_HashTable.pElements[iHash].pKey);
			
			if (iEqual == 0) pResult = NULL;
			else
			{
				tmHashElement*	pTempElement = &m_HashTable.pElements[iHash];
				
				do 
				{
					iEqual = EqualKey(pKey, pTempElement->pKey);
					pResult = pTempElement;
					
					if (iEqual == 0) 
					{
						pResult = NULL;	//	finded in the son's node
						break;
					}
					else if (iEqual < 0) pTempElement = pTempElement->pLNode;
					else pTempElement = pTempElement->pRNode;
					
				} while (pTempElement != NULL);
				
			}
		}		
	}
	
	return pResult;
}

/*--------------------------------------------------------------------------+
|  Function :  AddElement(tmHashElement* pElement)							|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL			CtmHashTable::AddElement(tmHashElement* pElement)
{
	BOOL			bResult = FALSE;
	int				iHash	= -1;
	int				iEqual 	= 0;
	tmHashElement*	pTempElement = NULL;
	
	if (pElement->pKey == NULL) bResult = FALSE;
	else
	{
		
		pTempElement = FindElementEmptyPos(pElement->pKey);
		if (pTempElement == NULL) bResult = FALSE;
		else
		{
			if (pTempElement->uiIndex == 0)
			{
				m_HashTable.uiCount++;
				m_HashTable.uiSize += pElement->uiSizeOfData;
				
				pTempElement->uiIndex 	= m_HashTable.uiCount;
				pTempElement->pKey 		= pElement->pKey;
				pTempElement->pData 	= pElement->pData;
				bResult					= TRUE;
				
			}
			else
			{
				iEqual = EqualKey(pElement->pKey, pTempElement->pKey);
				
				if (iEqual < 0)		//	save to left node
				{
					pTempElement->pLNode = new tmHashElement;
					
					m_HashTable.uiCount++;
					m_HashTable.uiSize += pElement->uiSizeOfData;
					
					pTempElement->pLNode->uiIndex 	= m_HashTable.uiCount;
					pTempElement->pLNode->pKey 		= pElement->pKey;
					pTempElement->pLNode->pData 	= pElement->pData;
					pTempElement->pLNode->pKey 		= pElement->pKey;
					pTempElement->pLNode->pLNode	= pElement->pLNode;
					pTempElement->pLNode->pRNode	= pElement->pRNode;
					bResult							= TRUE;
				}
				else			//	save to right node
				{
					pTempElement->pRNode = new tmHashElement;
					
					m_HashTable.uiCount++;
					m_HashTable.uiSize += pElement->uiSizeOfData;
					
					pTempElement->pRNode->uiIndex 	= m_HashTable.uiCount;
					pTempElement->pRNode->pKey 		= pElement->pKey;
					pTempElement->pRNode->pData 	= pElement->pData;
					pTempElement->pRNode->pKey 		= pElement->pKey;
					pTempElement->pRNode->pLNode	= pElement->pLNode;
					pTempElement->pRNode->pRNode	= pElement->pRNode;
					bResult							= TRUE;
				}
				
			}
		}
		
	}
	
	
	return bResult;
}

/*--------------------------------------------------------------------------+
|  Function :  ClearElement()												|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void			CtmHashTable::ClearElement()
{
	for (int i = 0; i < m_iHashSeed; i++)
	{
		FreeNode(m_HashTable.pElements[i].pLNode);
		FreeNode(m_HashTable.pElements[i].pRNode);
	}
	
	memset(m_HashTable.pElements, 0, m_iHashSeed * sizeof(tmHashElement));
	m_HashTable.uiCount = 0;
	m_HashTable.uiSize = 0;
}

/*--------------------------------------------------------------------------+
|  Function :  DelElement(void* pKey)										|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
//	not finished, and needn't it now
BOOL			CtmHashTable::DelElement(void* pKey)
{
	BOOL	bResult = FALSE;
	
	tmHashElement*	pTempElement = NULL;
	if (pKey == NULL) bResult = FALSE;
	else
	{
		pTempElement = FindElementEmptyPos(pKey);
		if (pTempElement == NULL) bResult = FALSE;
		else
		{
			//if pTempElement
			
			bResult = TRUE;
		}
	}
	return bResult;
}
