/*==========================================================================+
|  Class    : strlist		                                            	|
|  Task     : strlist	head file                          					|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : Fasm		                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2009/11/02                                                   	|
|  Revision : 			                                                    |
+==========================================================================*/
#include "strlist.h"
#define		MAX_COMMAND_LEN 256



CtmStringList::CtmStringList(void)
{
	m_FileLenth =0;
	m_LocalStringCnt =0;
}

CtmStringList::~CtmStringList(void)
{
	Clear();
}

int		CtmStringList::Insert(int nIndex, const char* psz)
{
	string* pstr = new string(psz);
	m_Strs.insert(m_Strs.begin() + nIndex, pstr);
	return nIndex;
}

int		CtmStringList::Insert(int nIndex, string* pStr)
{
	
	m_Strs.insert(m_Strs.begin() + nIndex, pStr);
	return nIndex;
}

int		CtmStringList::Add(const char *psz)
{
	string* pstr = new string(psz);
	return Add(pstr);
}

int		CtmStringList::Add(string *pstr)
{
	m_Strs.push_back(pstr);
	return 0;
}

int		CtmStringList::Clear()
{
    int nCount = GetNum();
	if(nCount < m_LocalStringCnt)	m_LocalStringCnt = nCount;

	for (unsigned int i = 0; i < m_LocalStringCnt/*GetNum()*/; i++)
	{
		string* pstr = assign(i)/*m_Strs.at(i)*/;

		delete pstr;
	}

    if (nCount > 0)
	m_Strs.clear();
	return 0;
}


int		CtmStringList::Del()
{
	int nNum = GetNum();
	if (nNum > 0)
		return Del(nNum - 1);
	else
		return -1;
}

int		CtmStringList::Del(int nIndex)
{
	string* pstr = assign(nIndex)/*m_Strs.at(nIndex)*/;
	delete pstr;
	m_Strs.erase(m_Strs.begin() +  nIndex);
	return 0;
}

string*		CtmStringList::IndexOfStr(int nIndex)
{
	return assign(nIndex)/*m_Strs.at(nIndex)*/;
}

const char*	CtmStringList::IndexOf(int nIndex)
{
	string* pstr = assign(nIndex)/*m_Strs.at(nIndex)*/;
	return pstr->c_str();

	//return NULL;
}


int			CtmStringList::Modify(int nIndex, const char *psz)
{
	string* pstr = assign(nIndex)/*m_Strs.at(nIndex)*/;
	pstr->assign(psz);
	return 0;
}

int			CtmStringList::Modify(int nIndex, std::string *pStr)
{
	string* pstr = assign(nIndex)/*m_Strs.at(nIndex)*/;
	pstr->assign(pStr->c_str());
	return 0;
}

int			CtmStringList::SaveToFile(const char *pszFile)
{
	//fstream m_file(pszFile, ios::in | ios::out |ios::trunc);
	m_file.open(pszFile, ios::in | ios::out |ios::trunc);
	//printf("SaveToFile----pszFile:%s Open%d \n",pszFile,m_file.is_open());
	if (m_file.is_open())
	{
		m_file.clear();
		
		char* pszTemp = NULL;		//20110919 add
		pszTemp = new char[MAX_COMMAND_LEN];	//20110919 add
		
		char Endl[3];
		Endl[0] = 0x0D;
		Endl[1] = 0x0A;
		Endl[2] = '\0'; 
		//printf("SaveToFile----pszFile:%s GetNum():%d \n",pszFile,GetNum());
		for (unsigned int i = 0; i < GetNum(); i++)
		{
			memset(pszTemp, 0, MAX_COMMAND_LEN);
			string* pstr = IndexOfStr(i);
			memcpy(pszTemp,pstr->c_str(),strlen(pstr->c_str())); //20110919 add
			m_file << (string)pszTemp << Endl;		
			//m_file << (*pstr) << Endl;
			//printf("i:%d strlen:%d  %d \n",i,pstr->length(),Endl[2]);
		}

		delete []pszTemp;
		m_file.close();
		//m_file.flush();強制清空緩衝區，令其寫入文件
		m_file.sync(); //20110929add 清空流
		return 0;
	}
	else return -1;
	
}

int			CtmStringList::LoadFromFile(const char *pszFile,int nStartRow, BOOL bLoadOnceFlag)
{
	//static char szFile_Last[40]="\0";	//20110913 add
	Clear();
	m_FileLenth = 0;
	m_LocalStringCnt =0;

	m_file.close();//20110913 add
	m_file.sync(); //20110913add 清空流
	m_file.open(pszFile,ios::in);
	//printf("pszFile:%s is_open:%d \n",pszFile,m_file.is_open());
	if (m_file.is_open())
    {	
        BOOL bLargeFileFirst = FALSE;
        string* pstr = new string;  	
		WORD wRowNum = 0;
        //printf("LoadFromFile-----pszFile :%s strcmp:%d  nStartRow:%d \n",pszFile,strcmp(szFile_Last,pszFile),nStartRow);
		if(nStartRow >0 || bLoadOnceFlag /*|| strcmp(szFile_Last,pszFile)==0*/)	//Read Large File 		//20110913 add strcmp(szFile_Last,pszFile)==0
		{
			if(bLoadOnceFlag)	
			{
        		while(getline(m_file, *pstr, '\n'))
				{
					if ((*pstr)[pstr->size() - 1] == 0x0D)
					    (*pstr)[pstr->size() - 1] = '\0';
					Add(pstr);              
                    m_LocalStringCnt ++;    
					pstr = new string;
				}
				//printf("LoadFromFile-2pszFile:%s nStartRow:%d bLoadOnceFlag:%d m_LocalStringCnt:%d \n",pszFile,nStartRow,bLoadOnceFlag,m_LocalStringCnt);
			}
			else
			{
				while(getline(m_file, *pstr, '\n'))
				{
					if ((*pstr)[pstr->size() - 1] == 0x0D)
					    (*pstr)[pstr->size() - 1] = '\0';
					    
					m_FileLenth++;
					
					if(nStartRow < m_FileLenth && m_FileLenth < (nStartRow +STRINGLIST_LOAD_ONETIME_CNT))
					{
						Add(pstr);
						m_LocalStringCnt ++;
						pstr = new string;
					}
				}
			}
		}
		else if( nStartRow ==0 )	//First Open File
		{
			while(getline(m_file, *pstr, '\n'))
			{
				if(wRowNum < STRINGLIST_LOAD_ONETIME_CNT )//First Open Large File, Only Open 64 Rows   Sunny20110805 add
				{
					if ((*pstr)[pstr->size() - 1] == 0x0D)
					    (*pstr)[pstr->size() - 1] = '\0';
					Add(pstr);              
					m_LocalStringCnt++;                       
					pstr = new string;
					wRowNum ++;
				}
				else break;
			}		     
		}             
		delete pstr;
		//printf("CtmStringList::LoadFromFile---pszFile:%s nStartRow:%d bLoadOnceFlag:%d m_LocalStringCnt:%d \n",pszFile,nStartRow,bLoadOnceFlag,m_LocalStringCnt);
		//strcpy(szFile_Last,pszFile);		//20110913 add
		m_file.close();
		return 0;
	}
	else return -1;
	
	
	return -1;
}


unsigned int	CtmStringList::GetNum()
{
	//printf("CtmStringList::GetNum----m_FileLenth:%d \n",m_FileLenth);
	if(m_FileLenth !=0 )	return m_FileLenth;
	else					return static_cast<unsigned int>(m_Strs.size());
}

void	CtmStringList::SetNum(unsigned int nNum)
{
	m_Strs./*reserve*/resize(nNum);
}

int				CtmStringList::Find(int nStartIndex, int nEndIndex, char* psz)
{
	if (GetNum() == 0)	return -1;
	else
	{
		if (nStartIndex < 0 || nEndIndex >= GetNum()) return -1;
		else
		{
			string* pstr = NULL;//
			for (int i = nStartIndex; i < nEndIndex; i++)
			{
				pstr = assign(i)/*m_Strs.at(i)*/;
				if (pstr->find(psz) != string::npos)
					return i;
			}
			
			return -1;
		}
	}
}

int		CtmStringList::Find(char* psz)
{
	return Find(0, GetNum() - 1, psz);
}


string* CtmStringList::assign(int index)	//20111202 add std::vector 數組越界的定位 - 對std::out_of_range拋出异常進行處理 
{
	if(index > m_LocalStringCnt)	m_LocalStringCnt = index;	//20120106 add

	string* pstr;
	try
	{
		pstr = m_Strs.at(index);
	}
	catch(std::out_of_range &exc)
	{  
        std::cerr <<exc.what();//std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;	//20120106 modify
        return NULL; 
	}
	return pstr;
}

int	CtmStringList::AppendStr(int nIndex, char* psz)
{
	string* pstr = assign(nIndex);
	pstr->append(psz);
	return 0;
	//string* pstr = m_Strs.at(index);
	//pstr = pstr + psz;
	//m_Strs.insert(index, pstr); 
}
