/*==========================================================================+
|  Class    : ParseXMLRecord 			                                    |
|  Task     : ParseXMLRecord source file                          			|
|---------------------------------------------------------------------------|
|  Compile  : G++ V4.7.3 -                                                  |
|  Link     : G++ V4.7.3 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2015/04/26                                                    |
|  Revision : 			                                                    |
+==========================================================================*/

#include "main.h"
#include "ParseXMLRecord.h"

//IMPLEMENT_DYNCREATE(CtmParseXMLRecord, CtmPackClass)

CtmParseXMLRecord *g_pParseXMLRecord = NULL;
CtmParseXMLRecord *g_pParseXMLViewStr= NULL;

//class  CStrings*		m_pIDList = NULL;			//record DataID Hash 
	
CtmParseXMLRecord::CtmParseXMLRecord()
{
//	m_pParseXML = static_cast<CParseXML*>(g_pPackage->CreateClassInstance("tmParseXML"));
	
//	m_ParseXML.InitParse(m_pParseXML);
//	//#ifdef	D_X86
//	char*	pPath = CtmConfig::GetInstance()->GetLanguagepath();
//	char*	pFileName = CtmConfig::GetInstance()->GetViewXMLFileName();
//	char	pszFilePath[256];
//	memset(pszFilePath, 0, sizeof(pszFilePath));
//	//printf("pPath=%s pFileName=%s \n", pPath, pFileName);
//	sprintf(pszFilePath, "%s%s", pPath, pFileName);
//	printf("pszFilePath=%s \n", pszFilePath);
//	LoadXMLData(pszFilePath);
	//#else
	//LoadXMLData("record.xml");
	//#endif
	m_nCount = 0;
	m_nLength = 0;
	for(int i = 0; i < XML_MAXSTRING; i++)
	{
		m_pszRecordDataID[i] = NULL;
	}
	m_pStrUnicode=NULL;
	//printf("g_pParseXMLRecord constrction success\n");
}

CtmParseXMLRecord::~CtmParseXMLRecord()
{
//	g_pPackage->ReleaseClassInstance("tmParseXML", g_pParseXMLRecord);
	if(m_nCount > 0)
	{
		for(int i = 0; i < m_nCount; i++)
		{
			if(m_pStrUnicode[i].pdwStrUnicode != NULL)
			{
				free(m_pStrUnicode[i].pdwStrUnicode);
				m_pStrUnicode[i].pdwStrUnicode = NULL;	
			}
			if(m_pStrUnicode[i].putf8code!=NULL)
			{
				free(m_pStrUnicode[i].putf8code);
				m_pStrUnicode[i].putf8code=NULL;
			}
			if(m_pszRecordDataID[i] != NULL)
			{
				free(m_pszRecordDataID[i]);
				m_pszRecordDataID[i] = NULL;	
			}
		}	
	}
	if (m_pIDList	!= NULL)
		delete m_pIDList;
}

int	CtmParseXMLRecord::CalculateCount(char* pszFileName)
{
	int		nCount = 0;
	xmlChar* pContext = NULL;
	xmlChar* pConStringText = NULL;
	
	xmlDocPtr 		m_xmldoc;	
	xmlNodePtr 		m_curNode;
	
	xmlKeepBlanksDefault (0);
	m_xmldoc = xmlParseFile(pszFileName);
	
	if(m_xmldoc != NULL)
	{
		m_curNode = (xmlNodePtr)xmlDocGetRootElement(m_xmldoc);
		if(m_curNode != NULL)
		{
			if (!xmlStrcmp(m_curNode->name, (const xmlChar *)"resources"))
			{
				m_curNode = m_curNode->xmlChildrenNode;
				while(m_curNode != NULL)
				{
					pContext = (xmlChar *)xmlGetProp(m_curNode, (const xmlChar*)"name");
					
					if(pContext!=NULL)
					{
						nCount++;
						xmlFree(pContext);
						pContext = NULL;
					}
					m_curNode = m_curNode->next;
				}
			}
		}
		xmlFreeDoc(m_xmldoc);
		xmlCleanupParser();
	}
	return nCount;
	
}

int CtmParseXMLRecord::LoadXMLData(char* pszFileName)
{
	xmlChar* pContext = NULL;
	xmlChar* pConStringText = NULL;
	STRINGSINFO	Info;
	CtmEncode* encode;
	int		nIndex = 0;
	xmlDocPtr 		m_xmldoc;	
	xmlNodePtr 		m_curNode;
	bool		bConvertFlag=FALSE;
	int i = 0, j = 0, nLength = 0;
	
	if ( pszFileName == NULL) 
	{
		printf("pszFileNameList is NULL\n");
		return -1;
	}
	char* pFTFileName = CtmConfig::GetInstance()->GetFontFTFileName();
	//printf("pFTFileName=%s\n",pFTFileName);
	bConvertFlag=CtmConfig::GetInstance()->GetFontConvert();
	m_nCount = CalculateCount(pszFileName);
	
	if(m_nCount >0 && m_nCount < XML_MAXSTRING)
	{
		m_pStrUnicode 	= new STRINGUNICODE[m_nCount];
	}
	//printf("m_nCount=%d \n", m_nCount);
	
	
	encode= new CtmEncode("ks_c_5601-1987","utf-8");		//korea
	//encode= new CtmEncode("GBK","utf-8");				//chinese
	//printf("::::pszFileName=%s \n", pszFileName);
	xmlKeepBlanksDefault (0);
	m_xmldoc = xmlParseFile(pszFileName);
	
	if(m_xmldoc != NULL)
	{
		m_curNode = (xmlNodePtr)xmlDocGetRootElement(m_xmldoc);
		if(m_curNode != NULL)
		{
			if (!xmlStrcmp(m_curNode->name, (const xmlChar *)"resources"))
			{
				m_curNode = m_curNode->xmlChildrenNode;
				while(m_curNode != NULL)
				{
					pContext = (xmlChar *)xmlGetProp(m_curNode, (const xmlChar*)"name");
					//pConStringText = (xmlChar*)xmlGetProp(m_curNode, (const xmlChar*)"value");
					
					if(pContext!=NULL)
					{
						pConStringText	= xmlNodeGetContent(m_curNode);
						//printf("pContext=%s \n", pContext);
						nLength = strlen((char*)pContext) + 1;
						//printf("1 m_nCount=%d nLength=%d \n", m_nCount, nLength);
						if(m_pszRecordDataID[nIndex]== NULL)
							m_pszRecordDataID[nIndex] =  (char*)malloc(nLength);
						strcpy(m_pszRecordDataID[nIndex], (char*)pContext);
						//printf("123 \n");
						if(pConStringText==NULL)
						{
							xmlFree(pContext);
							pContext = NULL;
							m_curNode = m_curNode->next;
							continue;
						}
						//printf("124 \n");
						char* pwch2=(char*)pConStringText;
						//printf("%s\n",pwch2);
						nLength	= (strlen(pwch2)+2)*4;
						unsigned long* unicode=(unsigned long*)malloc(nLength);
						int nLen = 0, nUTFLen = 0;
						nLen = encode->Utf8ConvertUnicode((unsigned char*)pwch2,unicode, &nUTFLen);
						if(bConvertFlag)
						{
							encode->CovertToUnicodeTable(unicode,nLen);	
						}
						//printf("nLen=%d strlen=%d nUTFLen=%d \n", nLen, strlen(pwch2), nUTFLen);
						//printf("125 \n");
						m_pStrUnicode[nIndex].pdwStrUnicode = (DWORD*)malloc(nLen*sizeof(DWORD));
						m_pStrUnicode[nIndex].nLen			= nLen;
						m_pStrUnicode[nIndex].nUTFLen		= nUTFLen;
						m_pStrUnicode[nIndex].putf8code		=(char*)malloc(nLength/2);
						//printf("126 \n");
						memset(m_pStrUnicode[nIndex].pdwStrUnicode, 0, nLen*sizeof(DWORD));
						//printf("nLen=%d nIndex=%d \n", nLen, nIndex);
						for(int j = 0; j < nLen ;j++)
						{
							m_pStrUnicode[nIndex].pdwStrUnicode[j] = *(unicode+j);
							//printf("code=%x \n", m_pStrUnicode[nIndex].pdwStrUnicode[j]);
						}
						memset(m_pStrUnicode[nIndex].putf8code,0,nLength/2);
						memcpy(m_pStrUnicode[nIndex].putf8code,pwch2,strlen(pwch2));
						
						if(unicode!=NULL)
						{
							free(unicode);
							unicode=NULL;
						}
						nIndex++;
						//printf("nIndex=%d  \n", nIndex);
						m_nLength += nLength;
					}
					//printf("free 1\n");
					if(pConStringText != NULL)
					{
						xmlFree(pConStringText);
						pConStringText = NULL;
					}
					if(pContext != NULL)
					{
						xmlFree(pContext);
						pContext = NULL;
					}
					m_curNode = m_curNode->next;
				}
			}
		}
		xmlFreeDoc(m_xmldoc);
		xmlCleanupParser();
	}
	delete encode;
	
	//printf("free 4 m_nCount=%d m_nLength=%d \n", m_nCount, m_nLength);
	Info.nCount	= m_nCount;
	Info.nSize	= m_nLength;
	m_pIDList	= new CStrings;
	m_pIDList->Create(&Info);
	for(int i = 0; i < m_nCount; i++)
	{
		//printf("pszRecordDataID[%d]=%s \n", i, m_pszRecordDataID[i]);
		m_pIDList->AddString(m_pszRecordDataID[i]);
	}
	return 0;
}

int	CtmParseXMLRecord::GetRecordIndex(long lDataID)
{
	int	nIndex = 0;
	char 			cszStrID[256];
	
	sprintf(cszStrID, "%08x", lDataID);
	//printf("cszStrID=%s \n", cszStrID);
	nIndex = m_pIDList->GetIndex(cszStrID);
	return nIndex;
}

int CtmParseXMLRecord::GetRecordIndex(char* pszStringID)
{
	int	nIndex = -1;
	//printf(" 1 \n");
	if(pszStringID != NULL)
		nIndex = m_pIDList->GetIndex(pszStringID);
	//printf(" 2 \n");
	return nIndex;	
}

STRINGUNICODE	CtmParseXMLRecord::GetRecordStr(int nIndex)
{
	if(nIndex < m_nCount && nIndex >= 0)
		return m_pStrUnicode[nIndex];
	else
		return m_pStrUnicode[0];
}

