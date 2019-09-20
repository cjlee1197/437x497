/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CIniFile					   				  					|
|  Task     : CIniFile code file                	   			      		|
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



#include "inifile.h"
#include "cfont.h"
//#include	"memwatch.h"
// Removes an INI file entry for a specified key value.
// Attempting to delete a key in a nonexistent section or attempting to
// delete a nonexistent key are not errors. In these cases, DeleteKey does
// nothing. 
//fstream m_IniFile;
void CIniFile::DeleteKey(const char* pszSection, const char* pszIdent)
{
}

// Erases an entire section of an INI file.
void CIniFile::EraseSection(const char* pszSection)
{
}


// Reads all the key names from a specified section of an INI file into a
// string list
// 第三個參數標記需要讀取多少個值，如果是<=0的話，那麼將會讀取Section中的所有值
// 返回值是返回一共讀取了多少個值
int CIniFile::ReadSection(const char* pszSection, char** pszOutIdents, int iNum)
{
	//	get all keys
	return GetVaildLines(pszSection, pszOutIdents, tmKey, iNum);
}


// Retrieves a string value from an INI file. 2006/6/14 09:19上午
int CIniFile::ReadString(const char* pszSection, const char* pszIdent, const char* pszDefault, char** pszValue)
{
	if(!m_IniFile.is_open())	return 0;
	int	nResult = 0;
	*pszValue = new char[100 + 1];
	int iValueLen = 0;
	
	char* psValue = NULL;
	char* psValueOld = NULL;
	char* pSection = new char[strlen(pszSection) + 3];	//	the addtional 2 character, '[' and ']'
	char pTempValue[MAXSTRINGLEN];						//	one line string
		
	//	compose section name, it's "[SectionName]"
	sprintf(pSection, "[%s]", pszSection);
	
	/*
	strcpy(pSection, "[");
	strcat(pSection, pszSection);
	strcat(pSection, "]");
	*/
	
	m_IniFile.seekg(0);			//	move to start position
	
	while (!m_IniFile.eof())
	{
		memset(pTempValue, 0, MAXSTRINGLEN);    //netdigger 2007/4/9
		iValueLen = GetOneVaildLine(pTempValue);
		if (iValueLen <= 0) break;
		
		if (strcmp(pSection, pTempValue) == 0)
		{
			while (1)	//	need to find pszIdent in the section
			{
				memset(pTempValue, 0, MAXSTRINGLEN);    //netdigger 2007/4/9
				iValueLen = GetOneVaildLine(pTempValue);
				if ((iValueLen <= 1) || ((pTempValue[0] == '[') && (pTempValue[iValueLen - 1] == ']')))
				{
					if (*pszValue != NULL) 
					{
							delete [](*pszValue);
							*pszValue = NULL;
					}
					
					if (pszDefault != NULL) 
					{	
						*pszValue = new char[strlen(pszDefault) + 1];
						//printf("pszDefault=%s len=%d \n", pszDefault, strlen(pszDefault) + 1);
						stpcpy(*pszValue, pszDefault);
					}
					break;	
				}
				
				
				psValue = strchr(pTempValue, '=');	//	to get value is "=Value"
				psValueOld  = psValue + 1;               //netdigger 2007/4/9
				iValueLen  -= psValueOld - pTempValue; 
				if (psValue != NULL)
				{
					psValue = DealLine(pTempValue, psValue, tmKey);
					if (strcmp(psValue, pszIdent) == 0)
					{
						if (*pszValue != NULL) 
						{
							delete[] (*pszValue);
							//delete [](*pszValue);
							//printf("free \n");
							*pszValue = NULL;
						}
						*pszValue = new char[iValueLen + 2] ;//new char[iValueLen + 2];       //netdigger 2007/4/9
						memset(*pszValue, 0, iValueLen + 2);
						static	int nCount = 0;
						nCount++;
						//printf("nCount=%d psValueOld=%s iValueLen=%d \n",nCount, psValueOld, iValueLen);
						//if(nCount <2)
						strcpy(*pszValue, psValueOld);
						break;
					}
					
					
				}
			}
			break;
		}
		
	}
	
	m_IniFile.clear();
	if (pSection != NULL)
		delete []pSection;
//	if (*pszValue != NULL)
//		nResult = strlen(*pszValue);        //netdigger 2007/4/9
/*        
	for (int i = nResult - 1; i >= 0; i--)
	{
		if ( (*pszValue)[i] == 0x20)
		 	(*pszValue)[i] = '\0';
		else break;
	}
*/
	nResult = iValueLen + 2;
	return nResult;
}

int		CIniFile::ReadInteger(const char* pszSection, const char* pszIdent, const int nDefault)
{
	char*	pszTemp = NULL;
	char	sz[32];
	sprintf(sz, "%d", nDefault);
	int		nResult = 0;
	ReadString(pszSection, pszIdent, sz, &pszTemp);
	
	if (pszTemp == NULL) nResult = 0;
	else nResult = strtol(pszTemp, NULL, 0);
		
	if (pszTemp != NULL) 
	{
		delete []pszTemp;		
	}
	return nResult;
}


// Writes a string value to an INI file.
void 	CIniFile::WriteString(const char* pszSection, const char* pszIdent, const char* pszValue)
{
	if(!m_IniFile.is_open())	return;
	int notfound = 0;
	int addlen = 0;
	string line;
	string filebuff;
	string section = pszSection;
	section = "[" + section + "]";
	section += '\r';
	char pTempValue[MAXSTRINGLEN];
	char* psValue;
	
	m_IniFile.seekg(0, ios_base::beg);
	while(!notfound && getline(m_IniFile, line))
	{
		if(line.find("[") != string::npos ||
			line.find("]") != string::npos ||
			line.find("=") != string::npos)
		{
			filebuff += line;
			filebuff += '\n';
		}
		if(line == section)
		{
			line.clear();
			while(getline(m_IniFile, line))
			{
				if(line.find("[") != string::npos &&
						line.find("]") != string::npos)
				{
					notfound = 1;
					break;
				}
				
				sprintf(pTempValue, "%s", line.c_str());
				psValue = strchr(pTempValue, '=');
				psValue = DealLine(pTempValue, psValue, tmKey);
				//if(line.find(pszIdent) != string::npos)
				if(strcmp(psValue, pszIdent) == 0)
				{
					addlen = line.size() - strlen(pszIdent) - 2 - strlen(pszValue);
					filebuff += pszIdent;
					filebuff += "=";
					filebuff += pszValue;
					filebuff += '\r';
					filebuff += '\n';
					break;
				}
				filebuff += line;
				filebuff += '\n';
			}
		}
		line.clear();
	}
	m_IniFile.clear();
	m_IniFile.seekp(0, ios_base::beg);
	m_IniFile<< filebuff;
	while(addlen-- > 0)
		m_IniFile<< ' ';
}

void	CIniFile::WriteInteger( const char* pszSection, const char* pszIdent, const int nValue)
{
	char	szValue[32];
	
	sprintf(szValue, "%d", nValue);
	WriteString(pszSection, pszIdent, szValue);
}


CIniFile::CIniFile(const char* pszFileName)
{
	m_IsClosed = true;
	m_nValueCode = 0;
	OpenFile(pszFileName);
}



CIniFile::~CIniFile()
{
	if (!m_IsClosed) 
	{
		//cout << "Debug info: destroy cinifile and close file!" << endl;
		CloseFile();
	}
}


bool CIniFile::OpenFile(const char* pszFileName)
{
	//	the file is opend, and m_IsClosed is false;
	//printf("m_IsClosed=%d pszFileName=%s \n", m_IsClosed, pszFileName);
	if (!m_IsClosed) CloseFile();
	//if(m_nValueCode != tmFT_CODE_TECH)
	//    m_IniFile.open(pszFileName, ios_base::in|ios_base::out|ios_base::binary);		//	if INI file is not exist, create it	auto
	//else
	//{
		m_IniFile.clear();		//James add 2015/05/07
	    m_IniFile.open(pszFileName, ios_base::in|ios_base::out|ios_base::binary);
	//}
	
	m_IsClosed = false;	
	//if (m_IniFile.good()) 
		//cout << "Debug info: the file's Ok! (" << pszFileName << ")"  << endl;
	//;
	//m_IniFile.clear();		//jame mark 2015/05/07
	return true;
}

void CIniFile::CloseFile()
{
	if(m_IniFile.is_open())
	{
		#ifdef	D_3354
		m_IniFile.sync();
		#endif
		m_IniFile.close();
		m_IniFile.clear();			//James add 2015/05/07  close -> clear is right flow
	}
	m_IsClosed = true;
	//cout << "Debug info: close file" << endl;
}


CIniFile::CIniFile()
{	
	m_nValueCode =0;
	m_IsClosed = true;
}



// Reads the values from all keys within a section of an INI file into a
// string list.
// 第三個參數標記需要讀取多少個值，如果是<=0的話，那麼將會讀取Section中的所有值
// 返回值是返回一共讀取了多少個值
int CIniFile::ReadSectionValues(const char* pszSection, char** pszValues, int iValueNum)
{	
	//	get all values
	return GetVaildLines(pszSection, pszValues, tmValue, iValueNum);

}


// 獲取一行中的有效字符，本類中就是去除\r\n(0x0A,0x0D)
// 返回值是有幾個有效字符
// 有效字符儲存在pszValue中
int CIniFile::GetOneVaildLine(char* pszValue)
{	
	if(!m_IniFile.is_open())	return 0;
	int iResult = 0;
	if(m_nValueCode != tmFT_CODE_TECH)
	{
    	m_IniFile.getline(pszValue, MAXSTRINGLEN);	//	file auto move to the next line
    	iResult = strlen(pszValue);
    	
    	
    	if (iResult <= 0) return iResult;
    	else
    	{
    		//	按照Windows的Ini慣例，最後兩個字符總是\r\n，而\n在讀取的時候自動被去除了
    		if (pszValue[iResult - 1] == '\r') pszValue[--iResult] = '\0';		// the last char is null		
    		else pszValue[iResult] = '\0';		
    	}
    	return iResult;
    }
    
    char   *pcCode   = pszValue;
    m_IniFile.read(pcCode, 1);
    while(*pcCode != '\n' && *pcCode != '=' && m_IniFile.eof() == false)
    {
        iResult ++;
        pcCode ++;
        m_IniFile.read(pcCode, 1);
    }
    
    if(m_IniFile.eof() == true)
        return iResult;
    else if(*pcCode == '\n')
    {
        *pcCode       = 0;
        *(pcCode- 1)  = 0;
        return iResult - 1;
    }
    else
    {    
        iResult ++;
        pcCode ++;
    }
        
    WORD *pwCode    = (WORD *)pcCode;
    m_IniFile.read((char *)pwCode, 2);    
    while(*pwCode != 0x0A0D && m_IniFile.eof() == false)
    {        
        iResult += 2;
        pwCode  ++;
        m_IniFile.read((char *)pwCode, 2);
    }
    
    *pwCode     = 0;
    return iResult;

}


int CIniFile::GetVaildLines(const char* pszSection, char** pszValues, tmIniValueType ValueType, int iValueNum)
{
	int iNum = 0;
	int iValueLen = 0;
	char* psValue;
	char* pSection = new char[strlen(pszSection) + 3];	//	the addtional 2 character, '[' and ']'
	char pTempValue[MAXSTRINGLEN];			//	one line value
	int     nStringLen;         //netdigger 2007/4/5
	unsigned short*   pCode;    //netdigger 2007/4/5
	
	
	//	compose section name, it's "[SectionName]"
	strcpy(pSection, "[");
	strcat(pSection, pszSection);
	strcat(pSection, "]");
	
	//m_IniFile.clear(ios::failbit|ios::badbit|ios::eofbit);			//	Clears all error flags	
	//cout << "Debug info: file's state is " << m_IniFile.rdstate() << endl;
	
	m_IniFile.seekg(0);			//	move to start position
	
	while (!m_IniFile.eof())
	{
		memset(pTempValue, 0, MAXSTRINGLEN);    //netdigger 2007/4/9
		iValueLen = GetOneVaildLine(pTempValue);		
		if (iValueLen <=0) break;		
		
		//	find the section
		//printf("pSection=%x,pTempValue=%x\n",pSection,pTempValue);
		if (strcmp(pSection, pTempValue) == 0)
		{
			while (1)	//	get all vlaues in the special section
			{
				memset(pTempValue, 0, MAXSTRINGLEN);    //netdigger 2007/4/9
				iValueLen = GetOneVaildLine(pTempValue);
				if (iValueLen <= 1) break;	//	最起碼需要“x=”，兩個字符
				if ((pTempValue[0] == '[') && (pTempValue[iValueLen - 1] == ']')) break; // this is nex section
				psValue     = strchr(pTempValue, '=');	//	to get value is "=Value"
				
				iValueLen  -= psValue - pTempValue + 1;      //netdigger 2007/4/9 =去掉
				
				
				//	對於pszValues[iNum]而言,需要動態的分配內存(將原有的內存釋放了),以便節約內存
				//	也是由於不知道值到底有多大
				if (pszValues[iNum]!=NULL) 
				{
					delete [](pszValues[iNum]);
					pszValues[iNum] = NULL;
				}
					
				
				if ((psValue!=NULL))
				{
                    
					psValue = DealLine(pTempValue, psValue, ValueType);
					if(ValueType == tmKey)
					{
					    nStringLen  = strlen(psValue) + 1;					   
					}		
					else
					    nStringLen  = iValueLen + 2;
					    
					pszValues[iNum] = new char[nStringLen];
					memcpy(pszValues[iNum], psValue, nStringLen);
//					strcpy(pszValues[iNum], psValue);					
				}
				
				
				iNum++;
				if ((iValueNum > 0) && (iNum >= iValueNum))	break;			
				
			}
			break;
		}	
			
	}	
	m_IniFile.clear();
	delete []pSection;
	return iNum;
}

char* CIniFile::DealLine(char* pszOldStr, char* pszNewStr, tmIniValueType DealType)
{
	//	沒有改變psNewStr指針的位置，但是返回了新的位置
	switch (DealType)
	{
		case tmLine:	//	get all, include ident, "=" and value
			pszNewStr = pszOldStr;
			break;
		case tmKey:		//	only get ident, remove space
			*pszNewStr = '\0';			//	the first char is '=',
			pszNewStr = pszOldStr;			
			for (int i = strlen(pszNewStr) - 1; i >= 0; i--)
			{
				if ((pszNewStr[i] == ' ') || (pszNewStr[i] == '\t')) pszNewStr[i] = '\0';
				else break;
			}
			////cout << "Debug info: string id is " << pszNewStr << endl;			
			break;
		case tmValue:	//	only get value
			pszNewStr++;						//	move to "Value", passed the "="			
			break;
	}
	return pszNewStr;
	
}

int	CIniFile::CheckVaildSection(const char* pszSection, const char* pszIdent)
{
	char*	pszTemp = NULL;
	int		nResult = 0;
	ReadString(pszSection, pszIdent, NULL, &pszTemp);
	
	if (pszTemp == NULL)
		nResult = 0;
	else
		nResult = 1;
		
	if (pszTemp != NULL) 
	{
		delete []pszTemp;		
	}
	return nResult;		
}

