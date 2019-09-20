/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CParse/CParseObject		   				  					|
|  Task     : CParse/CParseObject header file          			      		|
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


#include "parsepule.h"
#include "CObject.h"
//#include "porpertylist.h"

#define		BINARY_VERSION_1_0_0_0		0x1000000
#define		BINARY_VERSION_1_0_0_1		0x1000001
#define		BINARY_PROPERTY_LEN			1
#define		BINARY_CLASSTYPE_LEN		1
#define		BINARY_PROPERTYINDEX_LEN	2

#define		TM_BYTE_SCRIPT_LEN			1
#define		TM_SHORT_SCRIPT_LEN			2   
#define		TM_INT_SCRIPT_LEN			4
#define		TM_WORD_SCRIPT_LEN			2
#define		TM_DWORD_SCRIPT_LEN			4
#define		TM_LONG_SCRIPT_LEN			4
#define		TM_FLOAT_SCRIPT_LEN			4
#define		TM_INT64_SCRIPT_LEN			8
#define		TM_DOUBLE_SCRIPT_LEN		8
#define		TM_ENUM_SCRIPT_LEN			2
#define		TM_STRUCT_SCRIPT_LEN		2
#define		TM_STR_SCRIPT_LEN			8
#define		TM_USTR_LEN					8
#define		TM_CHAR_LEN					1
#define		TM_BOOL_LEN					2
#define		TM_CLASS_LEN				8
#define		TM_PROCEDURE_LEN			8
#define		TM_UNKNOW_LEN				2
extern		BOOL			g_bOldReInitFlag;

WORD	u_wDataTypeLen[] = 
{	TM_BYTE_SCRIPT_LEN , 		TM_SHORT_SCRIPT_LEN, 	TM_INT_SCRIPT_LEN, 		TM_WORD_SCRIPT_LEN,
	TM_DWORD_SCRIPT_LEN, 		TM_LONG_SCRIPT_LEN, 	TM_FLOAT_SCRIPT_LEN, 	TM_INT64_SCRIPT_LEN,
	TM_DOUBLE_SCRIPT_LEN, 		TM_ENUM_SCRIPT_LEN, 	TM_STRUCT_SCRIPT_LEN, 	TM_STR_SCRIPT_LEN,
	TM_USTR_LEN, TM_CHAR_LEN, 	TM_BOOL_LEN, TM_CLASS_LEN, TM_PROCEDURE_LEN, 	TM_UNKNOW_LEN};


//fstream m_file;
CtmTextFile m_binFile;
char ArithOperator[4] = {'+', '-', '*', '/'};
CParse::CParse()
{
	isFileOpen = false;
	m_bBinary = FALSE;
	pFile = NULL;
}

CParse::~CParse()
{
	if (isFileOpen) CloseFile();
}

/*---------------------------------------------------------------------------+
|  Function : bool CParse::OpenFile(char* psFileName)                        |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :是否打開了,如果打開返回true                                     |
+---------------------------------------------------------------------------*/
bool CParse::OpenFile(char* psFileName)
{
	//printf("psFileName=%s \n", psFileName);
	if (isFileOpen) CloseFile();
	
	char cszTempName[256], cszEndName[5] = ".tpf";
	struct stat st;
	char* pDot = NULL;
	char cDot = '.';
	int		nDotLen = 0, nFileNameLen = 0;
	
	memset(cszTempName, 0, sizeof(cszTempName));
	if(psFileName == NULL)
		return FALSE;
	
	nFileNameLen	= strlen(psFileName);
	if(nFileNameLen >4)
		pDot = strrchr(&psFileName[nFileNameLen-4], cDot);
	
	//printf("pDot=%s \n", pDot);
	if(pDot != NULL)
	{
		nDotLen = strlen(pDot);
		if(nDotLen < 2)
			nDotLen = 0;
		//printf("nDotLen=%d \n", nDotLen);
	}
	else
	{
		nDotLen = 0;	
	}
	if(0 == strcmp(&psFileName[strlen(psFileName)-nDotLen], cszEndName))	//後綴名為.tpf	James add 2010/8/5
	{
		strcpy(cszTempName, psFileName);
		m_bBinary = TRUE;
	}
	else
	{
		strcpy(cszTempName, psFileName);							//優先查找.tpf	James add 2010/8/5
		strcpy(&cszTempName[strlen(psFileName)-nDotLen], cszEndName);
		//printf("cszTempName=%s \n",cszTempName);
		if(! stat(cszTempName,&st))
		{
			m_bBinary = TRUE;
		}
		else
		{
			strcpy(cszTempName, psFileName);
			m_bBinary = FALSE;
		}
	}
	//printf("m_bBinary=%d cszTempName=%s \n", m_bBinary, cszTempName);
	
	if(m_bBinary)
	{
		isFileOpen = m_binFile.Open(cszTempName);
		m_binFile.Seek(0, SEEK_SET);
	}
	else
	{
		//m_file.open(cszTempName, ios::in);
		//m_file.seekg(0);
		//isFileOpen = m_file.good();
		struct stat info;
		memset(&info,0,sizeof(info));
		stat(cszTempName,&info);
		if(S_ISREG(info.st_mode))
		{ 
			pFile = fopen(cszTempName, "r");
			if(pFile != NULL)
			{
				fseek(pFile, 0, SEEK_SET);
				isFileOpen = TRUE;
			}
		}
	}
	
	//isFileOpen = true;
	return isFileOpen;
}


/*---------------------------------------------------------------------------+
|  Function : void CParse::CloseFile()				                         |
|  Task     : 關閉打開的文件                                                 |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :							                                     |
+---------------------------------------------------------------------------*/
void CParse::CloseFile()
{
	if (isFileOpen) 
	{
		if(m_bBinary)
		{
			m_binFile.Close();
		}
		else
		{
			//m_file.clear();
			//m_file.close(); 
			fclose(pFile);
			pFile = NULL;
			
		}
		isFileOpen = false;
	}
}


/*---------------------------------------------------------------------------+
|  Function : bool CParse::RTrim(char* pszValue)	                         |
|  Task     : 將字串的右邊空格/tab鍵去除                                     |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :							                                     |
+---------------------------------------------------------------------------*/
bool CParse::RTrim(char* pszValue)
{
	if (pszValue != NULL)
	{
		for (int i = strlen(pszValue) - 1; i > 0; i--)
		{
			if ((pszValue[i] == ' ') || (pszValue[i] == '\t'))  pszValue[i] = '\0';
			else break;
		}
		return true;
	}
	else return false;
}


/*---------------------------------------------------------------------------+
|  Function : char* CParse::LTrim(char* pszValue)	                         |
|  Task     : 將字串的左邊空格/tab鍵去除                                     |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   : 返回的是去除了的字串		                                     |
+---------------------------------------------------------------------------*/
char* CParse::LTrim(char* pszValue)
{
	int	len = 0;
	char* pszTemp = pszValue;
	if (pszValue != NULL)
	{
		len = strlen(pszValue);
		for (int i = 0; i < len; i++)
		{
			if ((pszTemp[i] == ' ') || (pszTemp[i] == '\t')) pszValue++;	
			else break;
		}
		return pszValue;
	}
	else return NULL;
	
	return NULL;
}


/*---------------------------------------------------------------------------+
|  Function : int CParse::GetOneLine(char* pszValue, bool isVaild)           |
|  Task     : 獲取文件中的當前行		                                     |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter: pszValue	讀取的字串      -                                    |
|             isVaild	標記是否去除最後一個字符，因Windows和Linux是不同的	 |
|  Return   : 一共有多少有效的字符		                                     |
+---------------------------------------------------------------------------*/
int CParse::GetOneLine(char* pszValue, bool isVaild)
{
	int iResult = 0;
	char strTemp[MAX_STRING_NUM]={0};
	//m_file.getline(strTemp, MAX_STRING_NUM);
	if(pFile == NULL) return iResult;
	fgets(strTemp, MAX_STRING_NUM, pFile);
	
	char* pszTemp = NULL;
	pszTemp = strchr(strTemp, '\r');
	if(pszTemp != NULL)*pszTemp = '\0';
	
	iResult = strlen(strTemp);
	//if (isVaild)	//	remove the last control char, in linux ,the last char '\r'?? (i forget it)
	//{
	//	if (iResult > 1)
	//	{
	//		strTemp[iResult - 1] = '\0';
	//		iResult--;
	//	}
	//}
	strcpy(pszValue, strTemp);
	return iResult;

}

/*---------------------------------------------------------------------------+
|  Function : bool CParse::ReduceLine(char* psLine, char* psKey, 			 |
									  char* pszValue, char Separate)		 |
|  Task     : 按照相關的規則，分解字串	                                     |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter: psLine	將要被分解的字串                                     |
|             psKey		分解出來的Key值										 |
|             pszValue	分解出來的Value										 |
|             Separate	分隔字符											 |
|  Return   : 是否分解成功				                                     |
+---------------------------------------------------------------------------*/
bool CParse::ReduceLine(char* psLine, char* psKey, char* pszValue, char Separate)
{
	char* pstrTemp;
	bool bResult = false;
	pstrTemp = strchr(psLine, Separate);
	bResult = (pstrTemp != NULL);
	if (bResult)
	{
		//	獲取值		
		pstrTemp++;			
		strcpy(pszValue, pstrTemp);
		//	獲取鍵
		*(--pstrTemp) = '\0';			
		pstrTemp = psLine;			
		pstrTemp = LTrim(pstrTemp);	//	忽略左邊的空格
		RTrim(pstrTemp);			//	忽略右邊的空格
		strcpy(psKey, pstrTemp);
	}	
	return bResult;
}


/*---------------------------------------------------------------------------+
|  Function : ParseLine(char* psKey, char* Value, char Separate)			 |
|  Task     : 按照相關的規則，分解字串	                                     |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter: 								                                 |
|             																 |
|             																 |
|             																 |
|  Return   : 								                                 |
+---------------------------------------------------------------------------*/
int CParse::ParseLine(char* psKey, char* Value, char Separate)
{
	char strTemp[MAX_STRING_NUM];
	int iResult = 0;
	iResult = GetOneLine(strTemp);	
	if (iResult > 0) 
	{
		if (!ReduceLine(strTemp, psKey, Value, Separate)) 
			iResult = 0;
	}
	return iResult;

}



CParseObject::CParseObject():CParse()
{
	m_iControlCount = 0;
	pthread_mutex_init(&m_pthreadParseFileLock,NULL);
	pthread_mutex_init(&m_pthreadInitFileLock,NULL);
	InitData();
}

CParseObject::~CParseObject()
{
	
	//	請注意，只會釋放其第一個元件
	//	這是由於第一個元件，從理論上講總是屬於宿主，只要釋放了宿主，其所擁有的元件將會自動地被釋放
	if (m_aryControls[0].control != NULL) 
	{
		delete m_aryControls[0].control;		
		m_aryControls[0].control = NULL;
	}
	
	
	for (int i = 0; i < MAX_CONTROL_NUM; i++)
	{
		if (m_aryControls[i].controlName != NULL) 
		{
			delete []m_aryControls[i].controlName;				
			m_aryControls[i].controlName = NULL;			
			
		}
		m_aryControls[i].control = NULL;

	}
	pthread_mutex_destroy(&m_pthreadParseFileLock);
	pthread_mutex_destroy(&m_pthreadInitFileLock);


}


/*---------------------------------------------------------------------------+
|  Function : void CParseObject::InitData()									 |
|  Task     : 初始化相關的變量			                                     |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter: 								                                 |
|             																 |
|             																 |
|             																 |
|  Return   : 								                                 |
+---------------------------------------------------------------------------*/
void CParseObject::InitData()
{
	m_iControlCount = 0;
	for( int i = 0; i < MAX_CONTROL_NUM; i++)
	{
		
		m_aryControls[i].controlName = NULL;
		m_aryControls[i].control = NULL;
		
		m_aryLevel[i] = -1;		
	}
	//m_iLayer = -1;
	////cout << "m_iLayer = " << m_iLayer << endl;	
	
}


/*---------------------------------------------------------------------------+
|  Function : void CParseObject::ReInitData()								 |
|  Task     : 重新初始化相關的變量			                                 |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter: 								                                 |
|             																 |
|             																 |
|             																 |
|  Return   : 								                                 |
|  Note 	: 需要注意， 在釋放m_aryControls[0]的時候，會講其所擁有的元件釋放|
|             默認的情況下，所解析出來的所有元件都是從屬于第一個元件		 |
+---------------------------------------------------------------------------*/
void CParseObject::ReInitData()
{
	//printf("g_bOldReInitFlag=%d \n", g_bOldReInitFlag);
	m_iControlCount = 0;
	pthread_mutex_lock(&m_pthreadInitFileLock);
	if (m_aryControls[0].control != NULL) 
	{
		//cout << "destroy control[0] is " << m_aryControls[0].control->Name << endl;
		//cout << "destroy control[0] addr is " << m_aryControls[0].control << endl;
		delete m_aryControls[0].control;	
		m_aryControls[0].control=NULL;	
	}
	for( int i = 0; i < MAX_CONTROL_NUM; i++)
	{
		if (m_aryControls[i].controlName != NULL) 
		{
			//printf("m_aryControls[%d]=%s\n",i,m_aryControls[i].controlName);
			delete[] m_aryControls[i].controlName;
			m_aryControls[i].controlName = NULL;
		}
		m_aryControls[i].control = NULL;
		m_aryLevel[i] = -1;
	}
	pthread_mutex_unlock(&m_pthreadInitFileLock);
	//SetPropertyListValue();				//James add 2010/8/16
}



bool CParseObject::OpenFile(char* psFileName)
{
	pthread_mutex_lock(&m_pthreadParseFileLock);
	CParse::OpenFile(psFileName);
	//printf("psFileName=%s \n", psFileName);
	//printf("nValue=%d\n",GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
	if(m_bBinary)
	{
		DWORD	dwVersion = 0;
		m_binFile.Seek(0, SEEK_SET);
		m_binFile.Read(&dwVersion, 4);
		//m_binFile.Seek(0, SEEK_SET);
		//printf("dwVersion=%x \n", dwVersion);
		if(dwVersion == BINARY_VERSION_1_0_0_0)
			ParseObject_Bin2();
		else if(dwVersion == BINARY_VERSION_1_0_0_1)
			ParseObject_Bin3();
		else
			ParseObject_Bin();
	}
	else
	{
		//printf("nValue=%d\n",GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
		ParseObject();
		//printf("nValue=%d\n",GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
	}
	
	pthread_mutex_unlock(&m_pthreadParseFileLock);
	return true;
}



void* CParseObject::CreateObject(char* pszName)
{
	return Create(pszName);
	
}

int CParseObject::ParseObject(char DefineSymbol, char Separate, char* pszEndStr)
{
	int  	m_iLayer = -1;					//	標記結束符層次
	char tempLine[MAX_STRING_NUM], tempKey[MAX_STRING_NUM], tempValue[MAX_STRING_NUM];
	char* pstrTemp=0;
	void* pResult = NULL;
	int ObjectNum = 0, ObjectIndex = 0, tempIndex = 0;
	//add	2014/8/11 11:40:53
	if(!g_bOldReInitFlag)
		ReInitData();
	if(pFile == NULL) return ObjectNum;
	while(!feof(pFile))
	{
		
		pstrTemp = tempLine;
		GetOneLine(pstrTemp);
		pstrTemp = LTrim(pstrTemp);
		//printf("reduce Line =%s\n", pstrTemp);
		if (strlen(pstrTemp) < 3) break;		
		
		if (strcmp(pstrTemp, pszEndStr) == 0)	//	定義結束符
		{
			m_iLayer--;
			tempIndex++;
			ObjectIndex = ObjectNum - tempIndex;
		}
		else
		{		
			pstrTemp = NULL;
			pstrTemp = strchr(tempLine, Separate);
			if (pstrTemp == NULL)		//	not property, is define component
			{
				
				pstrTemp = strchr(tempLine, DefineSymbol);
				if (pstrTemp != NULL)
				{					
					ReduceLine(tempLine, tempKey, tempValue, DefineSymbol);
					ObjectNum++;
					m_iLayer++;					
					ObjectIndex = ObjectNum;
					pResult = CreateObject(tempValue);			
					if (m_aryControls[ObjectNum - 1].controlName != NULL) 
					{
						delete[] m_aryControls[ObjectNum - 1].controlName;
						m_aryControls[ObjectNum - 1].controlName=NULL;//delete []m_aryControls[ObjectNum - 1].controlName;
					}
					m_aryControls[ObjectNum - 1].controlName = new char[strlen(tempValue)+1];//new char[strlen(tempValue) + 1];					
					strcpy(m_aryControls[ObjectNum - 1].controlName, tempValue);
					m_aryControls[ObjectNum - 1].control = (CtmWnd*)pResult;
					//printf("create tempValue=%s \n", tempKey);
					
					if (pResult != NULL)
					{
						pstrTemp = LTrim(tempKey);	//	忽略左邊的空格
						strcpy(m_aryControls[ObjectNum - 1].control->Name, pstrTemp);
						m_aryControls[ObjectNum - 1].control->SetPropertys();
						m_aryLevel[ObjectNum - 1] = m_iLayer;		
						//cout << m_aryControls[ObjectNum - 1].control->Name << " SetParent; ";
						m_aryControls[ObjectNum - 1].control->SetParent(FindParent((ObjectNum - 1), (m_iLayer - 1)));
					}
				}
				
			}
			else	//非定義符號
			{	
				if (ReduceLine(tempLine, tempKey, tempValue, Separate))
				{
					pstrTemp = LTrim(tempKey);	//	忽略左邊的空格
				
					SetPropertyInfo(m_aryControls[ObjectIndex - 1].control, pstrTemp, tempValue);
				}
			}
		}
	}

	//m_file.clear();
	
	m_iControlCount = ObjectNum;
	return ObjectNum;
}

/*---------------------------------------------------------------------------+
|  Function : int 	CParse::ParseObject_Bin(char cEndStr)	                 |
|  Task     : 二進制解析規則---完整屬性版本		                             |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :							                                     |
+---------------------------------------------------------------------------*/
int CParseObject::ParseObject_Bin(char cEndStr)
{
	int  	m_iLayer = -1;					//	標記結束符層次
	char tempLine[MAX_STRING_NUM], tempKey[MAX_STRING_NUM], tempValue[MAX_STRING_NUM];
	DWORD	dwVersion = 0;
	CLASSINFO	classInfo;
	BYTE		bClassType = 0xFF;
	char* pstrTemp;
	void* pResult = NULL;
	int ObjectNum = 0, ObjectIndex = 0, tempIndex = 0;
	int	nFilePos = 0, nFileSize = 0, nClassStringLen = 0, nClassPropertyLen = 0;
	
	ReInitData();
	
	nFileSize = m_binFile.Seek(0, SEEK_END);		//Read File Length 
	m_binFile.Seek(0, SEEK_SET);
	m_binFile.Read(&dwVersion, 4);
	while (!m_binFile.IsEOF())
	{
		pstrTemp = tempLine;
		//printf("nClassStringLen=%d \n", nClassStringLen);
		if(nClassStringLen > 0)	//移動之前字串的長度
		{
			nFilePos = m_binFile.Seek(0, SEEK_CUR);
			m_binFile.Seek(nFilePos+nClassStringLen, SEEK_SET);
		}
		nClassStringLen = 0;		//Init class string len
		m_binFile.Read(&bClassType, BINARY_CLASSTYPE_LEN);
		//printf("bClassType=%d nFilePos=%x \n", bClassType, nFilePos);
		if(bClassType != 0xFF)		//New Class
		{
			m_binFile.Read(&classInfo, 8);
			//printf("wVersion=%d Type=%x add=%d, len=%d\n", dwVersion, bClassType, classInfo.dwClassInstanceAdd,
			//classInfo.dwClassInstanceLen);
			if(classInfo.dwClassInstanceAdd > nFileSize)
			{
				//printf("Read ClassName Error \n");
				break;	
			}
			if(classInfo.dwClassInstanceLen > MAX_STRING_NUM)
			{
				//printf("Read ClassLen Error \n");
				break;	
			}
			if(bClassType < CLASS_MAXCOUNT)
			{
				strcpy(tempValue, tm_ClassList.pszClassName[bClassType]);
				//printf("tempValue=%s ObjectNum=%d m_iLayer=%d \n", tempValue, ObjectNum, m_iLayer);
				ObjectNum++;
				m_iLayer++;
				ObjectIndex = ObjectNum;
				pResult = CreateObject(tempValue);
				if (m_aryControls[ObjectNum - 1].controlName != NULL)
					delete []m_aryControls[ObjectNum - 1].controlName;
				m_aryControls[ObjectNum - 1].controlName = new char[strlen(tempValue) + 1];
				
				strcpy(m_aryControls[ObjectNum - 1].controlName, tempValue);
				m_aryControls[ObjectNum - 1].control = (CtmWnd*)pResult;
				if (pResult != NULL)
				{
					nFilePos = m_binFile.Seek(0, SEEK_CUR);
					m_binFile.Seek(classInfo.dwClassInstanceAdd, SEEK_SET);
					m_binFile.Read(pstrTemp, classInfo.dwClassInstanceLen);	//Read Class
					m_binFile.Seek(nFilePos, SEEK_SET);
					nClassStringLen += classInfo.dwClassInstanceLen;
					pstrTemp[classInfo.dwClassInstanceLen] = '\0';
					//printf("Class Name=%s \n", pstrTemp);
					memset(m_aryControls[ObjectNum - 1].control->Name,0,sizeof(m_aryControls[ObjectNum - 1].control->Name));
					strcpy(m_aryControls[ObjectNum - 1].control->Name, pstrTemp);
					m_aryControls[ObjectNum - 1].control->SetPropertys();
					m_aryLevel[ObjectNum - 1] = m_iLayer;
					m_aryControls[ObjectNum - 1].control->SetParent(FindParent((ObjectNum - 1), (m_iLayer - 1)));
				}
				
				for(int i = 0; i <tm_ClassList.dwPropertyCount[bClassType]; i++)				//Set Property
				{
					nClassPropertyLen = u_wDataTypeLen[tm_ClassList.pszClassProperty[bClassType][i].dwPropType];
					strcpy(pstrTemp, tm_ClassList.pszClassProperty[bClassType][i].pszProperty);	//Proerty name
					switch(tm_ClassList.pszClassProperty[bClassType][i].dwPropType)
					{
						case tmSTR:
						case tmPROCEDURE:
							m_binFile.Read(&classInfo, 8);
							if(classInfo.dwClassInstanceLen > 0 && classInfo.dwClassInstanceLen < MAX_STRING_NUM)
							{
								nFilePos = m_binFile.Seek(0, SEEK_CUR);
								m_binFile.Seek(classInfo.dwClassInstanceAdd, SEEK_SET);
								m_binFile.Read(tempValue, classInfo.dwClassInstanceLen);	//Read Class
								m_binFile.Seek(nFilePos, SEEK_SET);
								tempValue[classInfo.dwClassInstanceLen] = '\0';
								//	printf("classInfo.dwClassInstanceLen=%d nFilePos=%x tempValue=%s \n", classInfo.dwClassInstanceLen, nFilePos, tempValue);
								nClassStringLen += classInfo.dwClassInstanceLen;
								SetPropertyInfo_bin(m_aryControls[ObjectIndex - 1].control, pstrTemp, tempValue);
							}
							else		//property is NULL
							{
								;
							}
							break;
						case tmUSTR:
							m_binFile.Read(&classInfo, 8);
							if(classInfo.dwClassInstanceLen > 0 && classInfo.dwClassInstanceLen < MAX_STRING_NUM)
							{
								nFilePos = m_binFile.Seek(0, SEEK_CUR);
								m_binFile.Seek(classInfo.dwClassInstanceAdd, SEEK_SET);
								m_binFile.Read(tempValue, classInfo.dwClassInstanceLen);	//Read Class
								m_binFile.Seek(nFilePos, SEEK_SET);
								tempValue[classInfo.dwClassInstanceLen] = '\0';
								//printf("USTR classInfo.dwClassInstanceLen=%d nFilePos=%x tempValue=%s \n", classInfo.dwClassInstanceLen, nFilePos, tempValue);
								nClassStringLen += classInfo.dwClassInstanceLen;
								SetPropertyInfo_bin(m_aryControls[ObjectIndex - 1].control, pstrTemp, tempValue);
							}
							else		//property is NULL
							{
								;
							}
							break;
						default :
							m_binFile.Read(tempValue, nClassPropertyLen);								//Proerty value
							SetPropertyInfo_bin(m_aryControls[ObjectIndex - 1].control, pstrTemp, tempValue);
							break;
					}
				}
			}
		}
		else						//Class End
		{
			m_iLayer--;
			tempIndex++;
			ObjectIndex = ObjectNum - tempIndex;
		}
	}
	m_iControlCount = ObjectNum;
	return ObjectNum;
}

/*---------------------------------------------------------------------------+
|  Function : int CParse::ParseObject_Bin2(char cEndStr)	                 |
|  Task     : 二進制解析規則---和txt屬性一致		                         |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :							                                     |
+---------------------------------------------------------------------------*/
int CParseObject::ParseObject_Bin2(char cEndStr)
{
	int  	m_iLayer = -1;					//	標記結束符層次
	char 	tempLine[MAX_STRING_NUM], tempKey[MAX_STRING_NUM], tempValue[MAX_STRING_NUM];
	DWORD	dwVersion = 0;
	CLASSINFO	classInfo;
	BYTE		bClassType = 0xFF;
	char* 	pstrTemp;
	void* 	pResult = NULL;
	int 	ObjectNum = 0, ObjectIndex = 0, tempIndex = 0;
	DWORD	nFilePos = 0, nFileSize = 0, nClassStringLen = 0, nClassPropertyLen = 0;
	DWORD	dwPropertyLen = 0, dwPropertyValueLen = 0;
	
	ReInitData();
	
	//clock_t start, finish, start1= 0, start2 = 0,start3= 0,start4= 0;
	
	//start = clock();
	while (!m_binFile.IsEOF())
	{
		pstrTemp = tempLine;
		m_binFile.Read(&bClassType, BINARY_CLASSTYPE_LEN);
		if(bClassType == 0x00)			//Class End
		{
			m_iLayer--;
			tempIndex++;
			ObjectIndex = ObjectNum - tempIndex;
		}
		else if(bClassType == 0xFF)		//New Class
		{
			//start1 = clock();
			m_binFile.Read(&nClassStringLen, BINARY_PROPERTY_LEN);
			
			if(nClassStringLen > MAX_STRING_NUM)
			{
				//printf("Read ClassLen Error \n");
				break;	
			}

			m_binFile.Read(tempValue, nClassStringLen);
			tempValue[nClassStringLen] = '\0';
			ObjectNum++;
			m_iLayer++;
			ObjectIndex = ObjectNum;
			pResult = CreateObject(tempValue);
			if (m_aryControls[ObjectNum - 1].controlName != NULL)
				delete []m_aryControls[ObjectNum - 1].controlName;
			m_aryControls[ObjectNum - 1].controlName = new char[strlen(tempValue) + 1];
			
			strcpy(m_aryControls[ObjectNum - 1].controlName, tempValue);
			m_aryControls[ObjectNum - 1].control = (CtmWnd*)pResult;
			if (pResult != NULL)
			{
				m_binFile.Read(&nClassStringLen, BINARY_PROPERTY_LEN);
				if(nClassStringLen > MAX_STRING_NUM)
				{
					//printf("Read Instance ClassLen Error \n");
					break;	
				}
				m_binFile.Read(pstrTemp, nClassStringLen);
				pstrTemp[nClassStringLen] = '\0';
				strcpy(m_aryControls[ObjectNum - 1].control->Name, pstrTemp);
				m_aryControls[ObjectNum - 1].control->SetPropertys();
				m_aryLevel[ObjectNum - 1] = m_iLayer;
				m_aryControls[ObjectNum - 1].control->SetParent(FindParent((ObjectNum - 1), (m_iLayer - 1)));
			}
			//start4 = clock();
			//start2 += (start4-start1);
		}
		else		//Set Property 
		{
			//start1 = clock();
			m_binFile.Read(&dwPropertyLen, BINARY_PROPERTY_LEN);
			if(dwPropertyLen > MAX_STRING_NUM)
			{
				//printf("Read Instance Property Error \n");
				break;		
			}
			m_binFile.Read(pstrTemp, dwPropertyLen);
			pstrTemp[dwPropertyLen] = '\0';
			
			m_binFile.Read(&dwPropertyValueLen, BINARY_PROPERTY_LEN);
			if(dwPropertyValueLen > MAX_STRING_NUM)
			{
				//printf("Read Instance Property Value Error \n");
				break;	
			}
			m_binFile.Read(tempValue, dwPropertyValueLen);
			tempValue[dwPropertyValueLen] = '\0';
			//printf("pstrTemp=%s tempValue=%s \n", pstrTemp, tempValue);
			SetPropertyInfo_bin(m_aryControls[ObjectIndex - 1].control, pstrTemp, tempValue);
			//start4 = clock();
			//start3 += (start4-start1);
		}
	}
	//finish = clock();
	//printf("Parsepule start=%d finish=%d count=%d start2=%d start3=%d\n", start/1000, finish/1000, 
	//(finish - start)/1000, start2, start3);
	m_iControlCount = ObjectNum;
	return ObjectNum;
}

/*---------------------------------------------------------------------------+
|  Function : int CParse::ParseObject_Bin3(char cEndStr)	                 |
|  Task     : 二進制解析規則---和txt屬性一致		                         |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :							                                     |
+---------------------------------------------------------------------------*/
int CParseObject::ParseObject_Bin3(char cEndStr)
{
	int  	m_iLayer 	= -1;					//	標記結束符層次
	char 	tempLine[MAX_STRING_NUM], tempKey[MAX_STRING_NUM], tempValue[MAX_STRING_NUM];
	DWORD	dwVersion 	= 0;
	CLASSINFO	classInfo;
	BYTE		bClassType = 0xFF;
	char* 	pstrTemp;
	void* 	pResult 	= NULL;
	int 	ObjectNum 	= 0, ObjectIndex 	= 0, tempIndex 			= 0;
	DWORD	nFilePos	= 0, nFileSize 		= 0, nClassStringLen 	= 0, nClassPropertyLen = 0;
	DWORD	dwPropertyLen = 0, dwPropertyValueLen = 0, dwPropertyIndex = 0;
	
	ReInitData();
	
	//clock_t start, finish, start1= 0, start2 = 0,start3= 0,start4= 0;
	//printf("ParseObject_Bin3 \n");
	//start = clock();
	while (!m_binFile.IsEOF())
	{
		pstrTemp = tempLine;
		m_binFile.Read(&bClassType, BINARY_CLASSTYPE_LEN);
		//printf("bClassType=%x \n", bClassType);
		if(bClassType == 0x00)			//Class End
		{
			m_iLayer--;
			tempIndex++;
			ObjectIndex = ObjectNum - tempIndex;
		}
		else if(bClassType == 0xFF)		//New Class
		{
			//start1 = clock();
			m_binFile.Read(&nClassStringLen, BINARY_PROPERTY_LEN);
			
			if(nClassStringLen > MAX_STRING_NUM)
			{
				printf("Read ClassLen Error \n");
				break;	
			}

			m_binFile.Read(tempValue, nClassStringLen);
			tempValue[nClassStringLen] = '\0';
			ObjectNum++;
			m_iLayer++;
			ObjectIndex = ObjectNum;
			pResult = CreateObject(tempValue);
			if (m_aryControls[ObjectNum - 1].controlName != NULL)
				delete []m_aryControls[ObjectNum - 1].controlName;
			m_aryControls[ObjectNum - 1].controlName = new char[strlen(tempValue) + 1];
			
			strcpy(m_aryControls[ObjectNum - 1].controlName, tempValue);
			m_aryControls[ObjectNum - 1].control = (CtmWnd*)pResult;
			if (pResult != NULL)
			{
				m_binFile.Read(&nClassStringLen, BINARY_PROPERTY_LEN);
				if(nClassStringLen > MAX_STRING_NUM)
				{
					printf("Read Instance ClassLen Error \n");
					break;	
				}
				m_binFile.Read(pstrTemp, nClassStringLen);
				//printf("pstrTemp=%s \n", pstrTemp);
				pstrTemp[nClassStringLen] = '\0';
				strcpy(m_aryControls[ObjectNum - 1].control->Name, pstrTemp);
				m_aryControls[ObjectNum - 1].control->SetPropertys_Nub();
				//start4 = clock();
				//start2 += (start4-start1);
				m_aryLevel[ObjectNum - 1] = m_iLayer;
				m_aryControls[ObjectNum - 1].control->SetParent(FindParent((ObjectNum - 1), (m_iLayer - 1)));
			}
		}
		else		//Set Property 
		{
			//start1 = clock();
			m_binFile.Read(&dwPropertyIndex, BINARY_PROPERTYINDEX_LEN);
			if(dwPropertyIndex > MAX_CONTROLS)
			{
				printf("Read Instance Property Index Error \n");
				break;	
			}
			m_binFile.Read(&dwPropertyValueLen, BINARY_PROPERTY_LEN);
			//printf("index=%d ValueLen=%d \n",dwPropertyIndex, dwPropertyValueLen);
			if(dwPropertyValueLen > MAX_STRING_NUM)
			{
				printf("Read Instance Property Value Error \n");
				break;	
			}
			m_binFile.Read(tempValue, dwPropertyValueLen);
			tempValue[dwPropertyValueLen] = '\0';
			if(dwPropertyIndex > 0)
				dwPropertyIndex -= 1;
			SetPropertyInfo_bin(m_aryControls[ObjectIndex - 1].control, dwPropertyIndex, tempValue);
			//start4 = clock();
			//start3 += (start4-start1);
		}
	}
	//finish = clock();
	//printf("Parsepule start=%d finish=%d count=%d start2=%d start3=%d\n", start/1000, finish/1000, 
	//(finish - start)/1000, start2, start3);
	m_iControlCount = ObjectNum;
	return ObjectNum;
}

//	從已有的元件中獲得屬於哪個元件的
CtmWnd* CParseObject::FindParent(int iStartIndex,int iLevel)
{
	CtmWnd* pResult = NULL;
	switch (iLevel)
	{
		case -1:
			break;
		case 0:
			pResult = m_aryControls[0].control;
			break;
		default:
			for (int i = iStartIndex; i >= 0 ; i--)
			{
				if (m_aryLevel[i] == iLevel) 
				{
					pResult = m_aryControls[i].control;
					break;
				}
			}
			break;
	}
	return pResult;
}


CtmWnd*		CParseObject::GetMainControl()
{
	int iTemp = m_iControlCount;
	if (iTemp <= 0) return NULL;
	return m_aryControls[0].control;
}


CtmWnd*		CParseObject::GetControl(int index)
{
	if ((index < 0) || (index >= m_iControlCount)) return NULL;
	else return m_aryControls[index].control;
	
}

/*---------------------------------------------------------------------------+
|  Function : bool CParse::SetPropertyInfo_bin(CtmWnd* pwndSender, 			 |
|						char* PropertyName, void* Value)	                 |
|  Task     : 二進制解析賦屬性			  			                         |
+----------------------------------------------------------------------------+
|  Call     : 																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :							                                     |
+---------------------------------------------------------------------------*/
bool 	CParseObject::SetPropertyInfo_bin(CtmWnd* pwndSender, char* PropertyName, void* Value)
{
	char* pszTemp = NULL;
	if (pwndSender != NULL)
	{
		for(int i = 0; i < pwndSender->GetPropCount(); i++)		
		{
			if (strcmp(pwndSender->Propertys[i].pszName, PropertyName) == 0)
			{
				switch(pwndSender->Propertys[i].ptype)
				{
				case tmBYTE:
					*(BYTE*)(pwndSender->Propertys[i].propAddress) = *(BYTE*)Value;
					break;
				case tmSHORT:
					*(short*)(pwndSender->Propertys[i].propAddress) = *(short*)Value;
					break;
				case tmWORD:
					*(WORD*)(pwndSender->Propertys[i].propAddress) = *(WORD*)Value;
					break;
				case tmINT:
					*(int*)(pwndSender->Propertys[i].propAddress) = *(int*)Value;
					break;
				case tmDWORD:
					*(DWORD*)(pwndSender->Propertys[i].propAddress) = *(DWORD*)Value;
					break;
				case tmLONG:
					*(long*)(pwndSender->Propertys[i].propAddress) = *(long*)Value;
					break;
				case tmINT64:
					*(long long*)(pwndSender->Propertys[i].propAddress) = *(long long*)Value;
					break;					
				case tmFLOAT:
					*((float*)pwndSender->Propertys[i].propAddress) = *(float*)Value;
					break;
				case tmDOUBLE:
					*((double*)pwndSender->Propertys[i].propAddress) = *(double*)Value;
					break;
				case tmBOOL:
					*(BOOL*)pwndSender->Propertys[i].propAddress = *(BOOL*)Value;
					break;
				case tmUSTR:
				case tmSTR:
				case tmPROCEDURE:
					pszTemp = *(char**)pwndSender->Propertys[i].propAddress;
					
					if (Value == NULL)
					{
						if ((*(char**)pwndSender->Propertys[i].propAddress) != NULL)
							delete  [](*(char**)pwndSender->Propertys[i].propAddress);
						
						(*(char**)pwndSender->Propertys[i].propAddress) = NULL;
						break;
					}			
					else if ( ((*(char**)pwndSender->Propertys[i].propAddress) != NULL) &&
					   	 (strlen((*(char**)pwndSender->Propertys[i].propAddress)) <= strlen((char*)Value)) )
					{
						delete [](*(char**)pwndSender->Propertys[i].propAddress);
						(*(char**)pwndSender->Propertys[i].propAddress) = new char[strlen((char*)Value) + 1];
					}
					else if ((*(char**)pwndSender->Propertys[i].propAddress) == NULL)
					{
						(*(char**)pwndSender->Propertys[i].propAddress) = new char[strlen((char*)Value) + 1];
					}
					strcpy((*(char**)pwndSender->Propertys[i].propAddress),(char*)Value);
					break;	
				case tmUnknown:
					//cout << "Error data Type" << endl;
					break;
				default:
					break;
				}
				break;
			}
		}
		return true;
		
	}	
	else return false;
}

bool 	CParseObject::SetPropertyInfo_bin(CtmWnd* pwndSender, DWORD	dwIndex, void* Value)
{
	char* pszTemp = NULL;
	//printf("");
	if (pwndSender != NULL)
	{
		switch(pwndSender->Propertys[dwIndex].ptype)
		{
		case tmBYTE:
			*(BYTE*)(pwndSender->Propertys[dwIndex].propAddress) = *(BYTE*)Value;
			break;
		case tmSHORT:
			*(short*)(pwndSender->Propertys[dwIndex].propAddress) = *(short*)Value;
			break;
		case tmWORD:
			*(WORD*)(pwndSender->Propertys[dwIndex].propAddress) = *(WORD*)Value;
			break;
		case tmINT:
			*(int*)(pwndSender->Propertys[dwIndex].propAddress) = *(int*)Value;
			break;
		case tmDWORD:
			*(DWORD*)(pwndSender->Propertys[dwIndex].propAddress) = *(DWORD*)Value;
			break;
		case tmLONG:
			*(long*)(pwndSender->Propertys[dwIndex].propAddress) = *(long*)Value;
			break;
		case tmINT64:
			*(long long*)(pwndSender->Propertys[dwIndex].propAddress) = *(long long*)Value;
			break;					
		case tmFLOAT:
			*((float*)pwndSender->Propertys[dwIndex].propAddress) = *(float*)Value;
			break;
		case tmDOUBLE:
			*((double*)pwndSender->Propertys[dwIndex].propAddress) = *(double*)Value;
			break;
		case tmBOOL:
			*(BOOL*)pwndSender->Propertys[dwIndex].propAddress = *(BOOL*)Value;
			break;
		case tmUSTR:
		case tmSTR:
		case tmPROCEDURE:
			pszTemp = *(char**)pwndSender->Propertys[dwIndex].propAddress;
			
			if (Value == NULL)
			{
				if ((*(char**)pwndSender->Propertys[dwIndex].propAddress) != NULL)
					delete  [](*(char**)pwndSender->Propertys[dwIndex].propAddress);
				
				(*(char**)pwndSender->Propertys[dwIndex].propAddress) = NULL;
				break;
			}			
			else if ( ((*(char**)pwndSender->Propertys[dwIndex].propAddress) != NULL) &&
			   	 (strlen((*(char**)pwndSender->Propertys[dwIndex].propAddress)) <= strlen((char*)Value)) )
			{
				delete [](*(char**)pwndSender->Propertys[dwIndex].propAddress);
				(*(char**)pwndSender->Propertys[dwIndex].propAddress) = new char[strlen((char*)Value) + 1];
			}
			else if ((*(char**)pwndSender->Propertys[dwIndex].propAddress) == NULL)
			{
				(*(char**)pwndSender->Propertys[dwIndex].propAddress) = new char[strlen((char*)Value) + 1];
			}
			strcpy((*(char**)pwndSender->Propertys[dwIndex].propAddress),(char*)Value);
			break;	
		case tmUnknown:
			break;
		default:
			break;
		}
		return true;
		
	}	
	else return false;
}

bool 	CParseObject::SetPropertyInfo(CtmWnd* pwndSender, char* PropertyName, void* Value)
{

	char* pszTemp = NULL;
	char pszKey[256];
	char pszValue[256];
	long lNum1, lNum2;
	float fNum1, fNum2;
	tmPropInfo*  pProperty=NULL;
	if (pwndSender != NULL)
	{
		pProperty=pwndSender->GetProperty(PropertyName);
		//for(int i = 0; i < pwndSender->GetPropCount(); i++)		
		{

			//if (strcmp(pProperty->pszName, PropertyName) == 0)
			if(pProperty)
			{
				switch(pProperty->ptype)
				{
				case tmBYTE:
					if (IsNum((char*)Value))		//James add 2015/01/21
					{
						*(BYTE*)(pProperty->propAddress) = atoi((char*)Value);
					}
					//*(BYTE*)(pwndSender->Propertys[i].propAddress) = atoi((char*)Value);
					break;
				case tmSHORT:
					if (IsNum((char*)Value))
					{
						*(short*)(pProperty->propAddress) = strtol((char*)Value, NULL, 0);
					}
					else
					{
						for (int j = 0; j < 4; j++)
						{
							if (ReduceLine((char*)Value, pszKey, pszValue, ArithOperator[j]))
							{
								if (IsNum(pszKey)) lNum1 = strtol(pszKey, NULL, 0);
								else lNum1 = *(WORD*)pwndSender->GetPropValue(pszKey);
								if (IsNum(pszValue)) lNum2 = strtol(pszValue, NULL, 0);
								else lNum2 = *(WORD*)pwndSender->GetPropValue(pszValue);
								switch (ArithOperator[j])
								{
									case '+':
										*(short*)(pProperty->propAddress) = lNum1 + lNum2;
										break;
									case '-':
										*(short*)(pProperty->propAddress) = lNum1 - lNum2;
										break;
									case '*':
										*(short*)(pProperty->propAddress) = lNum1 * lNum2;
										break;
									case '/':
										*(short*)(pProperty->propAddress) = lNum1 / lNum2;
										break;
										
								}
							}
						}
					}
					break;
				case tmWORD:
					
					if (IsNum((char*)Value))
					{
						*(WORD*)(pProperty->propAddress) = strtol((char*)Value, NULL, 0);
					}
					else
					{
						for (int j = 0; j < 4; j++)
						{
							if (ReduceLine((char*)Value, pszKey, pszValue, ArithOperator[j]))
							{
								if (IsNum(pszKey)) lNum1 = strtol(pszKey, NULL, 0);
								else lNum1 = *(WORD*)pwndSender->GetPropValue(pszKey);
								if (IsNum(pszValue)) lNum2 = strtol(pszValue, NULL, 0);
								else lNum2 = *(WORD*)pwndSender->GetPropValue(pszValue);
								switch (ArithOperator[j])
								{
									case '+':
										*(WORD*)(pProperty->propAddress) = lNum1 + lNum2;
										break;
									case '-':
										*(WORD*)(pProperty->propAddress) = lNum1 - lNum2;
										break;
									case '*':
										*(WORD*)(pProperty->propAddress) = lNum1 * lNum2;
										break;
									case '/':
										*(WORD*)(pProperty->propAddress) = lNum1 / lNum2;
										break;
										
								}
							}
						}
					}
					break;
					
				case tmINT:
					
					if (IsNum((char*)Value))
					{
						*(int*)(pProperty->propAddress) = strtol((char*)Value, NULL, 0);
						
					}
					else
					{
						for (int j = 0; j < 4; j++)
						{
							if (ReduceLine((char*)Value, pszKey, pszValue, ArithOperator[j]))
							{
								if (IsNum(pszKey)) lNum1 = strtol(pszKey, NULL, 0);
								else lNum1 = *(int*)pwndSender->GetPropValue(pszKey);
								if (IsNum(pszValue)) lNum2 = strtol(pszValue, NULL, 0);
								else lNum2 = *(int*)pwndSender->GetPropValue(pszValue);
								switch (ArithOperator[j])
								{
									case '+':
										*(int*)(pProperty->propAddress) = lNum1 + lNum2;
										break;
									case '-':
										*(int*)(pProperty->propAddress) = lNum1 - lNum2;
										break;
									case '*':
										*(int*)(pProperty->propAddress) = lNum1 * lNum2;
										break;
									case '/':
										*(int*)(pProperty->propAddress) = lNum1 / lNum2;
										break;
										
								}
							}
						}
					}
					break;
					
				case tmDWORD:
				case tmLONG:
					if (IsNum((char*)Value))
					{
						*(long*)(pProperty->propAddress) = strtoul((char*)Value, NULL, 0);
					}
					else
					{
						for (int j = 0; j < 4; j++)
						{
							if (ReduceLine((char*)Value, pszKey, pszValue, ArithOperator[j]))
							{
								if (IsNum(pszKey)) lNum1 = strtoul(pszKey, NULL, 0);
								else lNum1 = *(long*)pwndSender->GetPropValue(pszKey);
								if (IsNum(pszValue)) lNum2 = strtoul(pszValue, NULL, 0);
								else lNum2 = *(long*)pwndSender->GetPropValue(pszValue);
								switch (ArithOperator[j])
								{
									case '+':
										*(long*)(pProperty->propAddress) = lNum1 + lNum2;
										break;
									case '-':
										*(long*)(pProperty->propAddress) = lNum1 - lNum2;
										break;
									case '*':
										*(long*)(pProperty->propAddress) = lNum1 * lNum2;
										break;
									case '/':
										*(long*)(pProperty->propAddress) = lNum1 / lNum2;
										break;
										
								}
							}
						}
					}
					break;

				case tmINT64:
					if (IsNum((char*)Value))
					*(long long*)(pProperty->propAddress) = strtoll((char*)Value, NULL, 0);
					break;					
				case tmFLOAT:
					if (IsNum((char*)Value))
						*((float*)pProperty->propAddress) = atof((char*)Value);
					else
					{
						for (int j = 0; j < 4; j++)
						{
							if (ReduceLine((char*)Value, pszKey, pszValue, ArithOperator[j]))
							{
								if (IsNum(pszKey)) fNum1 = strtoul(pszKey, NULL, 0);
								else fNum1 = *(float*)pwndSender->GetPropValue(pszKey);
								if (IsNum(pszValue)) fNum2 = strtoul(pszValue, NULL, 0);
								else fNum2 = *(float*)pwndSender->GetPropValue(pszValue);
								switch (ArithOperator[j])
								{
									case '+':
										*(float*)(pProperty->propAddress) = fNum1 + fNum2;
										break;
									case '-':
										*(float*)(pProperty->propAddress) = fNum1 - fNum2;
										break;
									case '*':
										*(float*)(pProperty->propAddress) = fNum1 * fNum2;
										break;
									case '/':
										*(float*)(pProperty->propAddress) = fNum1 / fNum2;
										break;
										
								}
							}
						}
					}
					break;
				case tmDOUBLE:
					*((double*)pProperty->propAddress) = atof((char*)Value);
					break;
				case tmBOOL:
					if (strcmp((char*)Value, "true") == 0 || strcmp((char*)Value, "TRUE") ==0)
						*(BOOL*)pProperty->propAddress = TRUE;
					else
						*(BOOL*)pProperty->propAddress = FALSE;					
					break;
				case tmUSTR:
				case tmSTR:
				case tmPROCEDURE:
					pszTemp = *(char**)pProperty->propAddress;
					
					if (Value == NULL)
					{
						if ((*(char**)pProperty->propAddress) != NULL)
						{
							//delete  [](*(char**)pwndSender->Propertys[i].propAddress);
							free(*(char**)pProperty->propAddress);
							(*(char**)pProperty->propAddress) = NULL;
						}
						break;
					}			
					else if ( ((*(char**)pProperty->propAddress) != NULL) &&
					   	 (strlen((*(char**)pProperty->propAddress)) <= strlen((char*)Value)) )
					{
						//delete [](*(char**)pwndSender->Propertys[i].propAddress);
						free(*(char**)pProperty->propAddress);
						(*(char**)pProperty->propAddress) = (char*)malloc(strlen((char*)Value) + 3);//new char[strlen((char*)Value) + 1];
					}
					else if ((*(char**)pProperty->propAddress) == NULL)
					{
						(*(char**)pProperty->propAddress) = (char*)malloc(strlen((char*)Value) + 3);//new char[strlen((char*)Value) + 1];
					}
					memset((*(char**)pProperty->propAddress),0,strlen((char*)Value) + 3);
					strcpy((*(char**)pProperty->propAddress),(char*)Value);
					
						
					break;	
				/*
				case tmPROCEDURE:
					while(g_FunctionSet[ii].pszFunctionName != NULL)
					{
						if (strcmp(g_FunctionSet[ii].pszFunctionName, (char*)Value) == 0)
						{
							*(void**)pwndSender->Propertys[i].propAddress = g_FunctionSet[ii].pFunction;
							break;
						}
						ii++;
					}
					break;
					*/
				case tmUnknown:
					//cout << "Error data Type" << endl;
					break;
				default:
					break;
				}	//	switch's end
				//break;
			}	//	if strcmp's end
		}	//	for's end
		return true;
		
	}	
	else return false;
}

bool 	CParseObject::IsNum(char* pszExp)
{
	int i;
	int	len;
	bool isHex = false;
	if (pszExp == NULL) return false;

	len = strlen(pszExp);

	for (i = 0; i < len; i++)
	{
		if (i != 1)
		{
			if (isHex)
			{
				if	(
					((pszExp[i] > 'f') && (pszExp[i] <= 'z'))||
					((pszExp[i] > 'F') && (pszExp[i] <= 'Z'))||
					((pszExp[i] == '+')||
					 (pszExp[i] == '-')||
					 (pszExp[i] == '*')||
					 (pszExp[i] == '/'))
					)
				if(pszExp[0] != '-')
					return false;
			}
			else
			{
				if	(
					((pszExp[i] >= 'a') && (pszExp[i] <= 'z'))||
					((pszExp[i] >= 'A') && (pszExp[i] <= 'Z'))||
					((pszExp[i] == '+')||
					 (pszExp[i] == '-')||
					 (pszExp[i] == '*')||
					 (pszExp[i] == '/'))
					)
				if(pszExp[0] != '-')
					return false;
			}
		}
		else
		{
			if ((pszExp[i] == 'x')||(pszExp[i] == 'X')) 
			{
				isHex = true;
				//break;
			}
			else if	(
					((pszExp[i] >= 'a') && (pszExp[i] <= 'z'))||
					((pszExp[i] >= 'A') && (pszExp[i] <= 'Z'))||
					((pszExp[i] == '+')||
					 (pszExp[i] == '-')||
					 (pszExp[i] == '*')||
					 (pszExp[i] == '/'))
					)
				return false;
		}
	}

	return true;
	
}
