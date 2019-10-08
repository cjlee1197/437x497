#include	"tmFreeDataG.h"



CtmFreeDataG*		g_pFreeDataG			= NULL;
int					g_nFreeDataCount	= 0;

IMPLEMENT_DYNCREATE(CtmFreeDataG, CtmFreeData)
IMPLEMENT_DYNCREATE(CtmSpecDataG, CtmSpecData)


CtmFreeDataG::CtmFreeDataG(): CtmFreeData()
{
	
}

CtmFreeDataG::~CtmFreeDataG()
{
	
}

int	CtmFreeDataG::CreateSelf()
{
	memset(m_szIDFile, 		sizeof(m_szIDFile), 	0);
	memset(m_szValueFile, 	sizeof(m_szValueFile), 	0);
	
	for (int i = 0; i < CNT_INT_MAX_FREE_NUM; i++)
	{
		m_szFreeDataID[i] = new char[64];
		memset(m_szFreeDataID[i], 0, 64);
	}
	
	return 0;
}

int	CtmFreeDataG::FreeSelf()
{
	for (int i = 0; i < CNT_INT_MAX_FREE_NUM; i++)
	{
		delete [](m_szFreeDataID[i]);
	}
	return CtmFreeData::FreeSelf();
}

int		CtmFreeDataG::LoadIDTable(char* pszIDTableFile)
{
	int nResult = 0;
	
	for (int i = 0; i < CNT_INT_MAX_FREE_NUM; i++)
	{
		memset(m_szFreeDataID[i], 0, 64);
	}
	
	if (pszIDTableFile != NULL)
	{
		strcpy(m_szIDFile, pszIDTableFile);
		
		m_ReadData.OpenFileStr(m_szIDFile);
		m_ReadData.ReadData(m_szFreeDataID);
		//printf("%s =%d\n", m_szIDFile, nTemp);
		m_ReadData.CloseFileStr();
		nResult = 0;
	}
	else 
	{
		m_szIDFile[0] 	= '\0';
		nResult 		= -1;
	}
	
	return nResult;
}

int		CtmFreeDataG::SearchIDFromTable(char* cszIDName)
{
	int 	i = 0;
	BOOL	bFlag = FALSE;
	while(m_szFreeDataID[i][0] != '\0')
	{
		if( 0 == strcmp(cszIDName, m_szFreeDataID[i++]))
		{
			bFlag = TRUE;
			break;
		}
	}
	if( !bFlag )
		i = 0;
	return	i;
}

int		CtmFreeDataG::SearchIndexFromTable(int nIndex)
{
	int 	i = 0;
	BOOL	bFlag = FALSE;
	char    cszDBID[256] = "\0";
	
	strcpy(cszDBID, g_pDatabase->GetString(nIndex));	
	if( cszDBID[0] == '\0' )
		return 0;
	while(m_szFreeDataID[i][0] != '\0')
	{
		if( 0 == strcmp(cszDBID, m_szFreeDataID[i++]))
		{
			bFlag = TRUE;
			break;
		}
	}
	if( !bFlag )
		i = 0;
	return	i;
}

int		CtmFreeDataG::SaveDataToFile(char* pszFile)
{
	//printf("CtmFreeDataG::SaveDataToFile(char* pszFile)\n");
	int nResult 	= -1;
	if (m_szIDFile[0] != '\0' && pszFile != NULL)
	{
		int nMemSize 	= GetSize();
		
		if (nMemSize > 0)
		{
			BYTE*	pMemTemp = new BYTE[nMemSize];
			
			//=========================================
			// read the values put into the memory, the values are from the database
			//=========================================
			int 	i = 0;
			WORD	wOffSet = 0;
			DBVALUE	dbValue;
			while(m_szFreeDataID[i][0] != '\0')
			{
				//printf("m_szFreeDataID[%d]=%s\n", i, m_szFreeDataID[i]);
				dbValue = g_pDatabase->Read(m_szFreeDataID[i]);
				if (dbValue.DataType.wType != TYPE_STRING)
				{
					memcpy((pMemTemp + wOffSet), dbValue.acData, dbValue.DataType.wLength);
				}
				else
					strncpy((char *)(pMemTemp + wOffSet), dbValue.pcData, dbValue.DataType.wLength);
				wOffSet += dbValue.DataType.wLength;
				
				i++;
			}
			
			
			//=========================================
			// write the memory to the file
			//=========================================	
			int nFd = open(pszFile, O_RDWR | O_CREAT);
			if (nFd >= 0)
			{
				write(nFd, pMemTemp, wOffSet);
				close(nFd);
				nResult = 0;
			}
			
			delete[] pMemTemp;
		}
		
	}
	
	
	return nResult;
	
}

int		CtmFreeDataG::GetDataFromFile(char* pszFile)
{
	int nResult = -1;
	if (pszFile != NULL && m_szIDFile[0] != '\0')
	{
		int nFd = open(pszFile, O_RDWR);
		if (nFd >= 0)
		{
			int nSize = lseek(nFd, 0, SEEK_END);
			
			if (nSize > 0)
			{
				//=======================================================
				//read the values from the file to the memory
				//=======================================================
				BYTE*	pTempMem = new BYTE[nSize];
				lseek(nFd, 0, SEEK_SET);
				read(nFd, pTempMem, nSize);
				
				
				//=======================================================
				//	write the values to database
				//=======================================================
				int		i 		= 0;
				WORD	wOffSet = 0;
				DBVALUE	dbValue;
				while(m_szFreeDataID[i][0] != '\0' && wOffSet < nSize)
				{
					//printf("CtmFreeDataG::GetDataFromFile, %d =%s\n", i,  m_szFreeDataID[i]);
					dbValue = g_pDatabase->Read(m_szFreeDataID[i]);
					
						
					if (dbValue.DataType.wType != TYPE_STRING)
						memcpy(dbValue.acData, (pTempMem + wOffSet), dbValue.DataType.wLength);
					else
						strncpy(dbValue.pcData, (char *)(pTempMem + wOffSet), dbValue.DataType.wLength);
					g_pDatabase->Write(&dbValue, TRUE);
					wOffSet += dbValue.DataType.wLength;
					i++;
				}
				
				delete[] pTempMem;
			}
			
			close(nFd);
		}
	}
	
	return nResult;
}

int 	CtmFreeDataG::GetSize()
{
	int nResult = 0;
	int i 		= 0;
	
	DBVALUE	dbValue;
	while(m_szFreeDataID[i][0] != '\0')
	{
		dbValue = g_pDatabase->Read(m_szFreeDataID[i]);
		nResult += dbValue.DataType.wLength;
		i++;
	}
	
	return nResult;
	
}

CtmSpecDataG::CtmSpecDataG()
{
	
}

CtmSpecDataG::~CtmSpecDataG()
{
	
}


int		CtmSpecDataG::CreateSelf()
{
	//sprintf(m_szSpecDataFile, "%s", "/usr
	char	szPath[256];
	//	================================================
	//	check the spec data path, if no dir and create it; "/usr/data/spec"
	//	================================================
	sprintf(szPath, "%s", "/usr/Data/spec");
	ExistDir(szPath);	
	sprintf(m_szSpecDataFile, "%s/spec.cdb", szPath);
	if (access(m_szSpecDataFile, R_OK) != 0)
	{
		int nFd = open(m_szSpecDataFile, O_RDWR | O_CREAT);
		ftruncate(nFd, tmEndSpecType * CNT_INT_SPECDAT_LEN);
		close(nFd);
	}
	
	
	
	sprintf(m_szSpecScriptPath, "%s", "./lib/hmi_group/spec");
	ExistDir(m_szSpecScriptPath);	
	
	#ifdef	D_3354
		sprintf(m_szCopyPath, "%s", "/media/sda1/");
	#else
		sprintf(m_szCopyPath, "%s", "/mnt/usb/");
	#endif	
	ExistDir(m_szCopyPath);
	
	#ifdef	D_3354
		sprintf(m_szCopyPath, "%s", "/media/sda1/spec");
	#else
		sprintf(m_szCopyPath, "%s", "/mnt/usb/spec");
	#endif	
	ExistDir(m_szCopyPath);
	
	sprintf(m_szCopyFile, "%s/spec.cdb", m_szCopyPath);
	if (access(m_szCopyFile, R_OK) != 0)
	{
		int nFd= open(m_szCopyFile, O_RDWR | O_CREAT);//creat(m_szSpecDataFile,777);
		ftruncate(nFd, tmEndSpecType * CNT_INT_SPECDAT_LEN);
		close(nFd);
	}
	
	
	for (int i = 0; i < CNT_INT_MAX_FREE_NUM; i++)
	{
		m_szDBDataID[i] = new char[64];
		memset(m_szDBDataID[i], 0, 64);
	}
	
	return 0;
}

int		CtmSpecDataG::FreeSelf()
{                     
	return 0;         
}                

     
int		CtmSpecDataG::SaveFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize)
{           
	//	check the valid values,(spectype fixednum & nsize)
	if (nSpecType >= tmEndSpecType)			nSpecType = tmEndSpecType - 1;
	if (nSize > CNT_INT_FIXDATA_TOTALLEN) 		nSize 	  = CNT_INT_FIXDATA_TOTALLEN;
	if (nFixedNum < 0)				 		nFixedNum = 0;
	if (nFixedNum >= CNT_INT_FIXDATA_NUM) 	nFixedNum = CNT_INT_FIXDATA_NUM - 1;
	
	//	calcualte the postion base on the spectype&fixednum
	int nPos = GetPos(nSpecType, nFixedNum);
	
	
	return WriteToFile(m_szSpecDataFile, pBuf, nSize, nPos);         
}                     

int		CtmSpecDataG::SaveDBData(int nSpecType)
{
	char szFile[256];
	char szBuffer[CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM];
	
	memset(szBuffer, 0, sizeof(szBuffer));
	//	compose the filename: path + "/nSpecType.cdb"
	GetDBFile(nSpecType, szFile);
	
	//	get the dbid array
	m_nIDNum = ReadDBID(szFile, m_szDBDataID);	
	
	//	Get the value from the database, and write to the special buffer
	int nSize = GetBufferFromDB(szBuffer, sizeof(szBuffer), m_szDBDataID, m_nIDNum);
	 
	//	calcualte the postion base on the spectype&fixednum
	int nPos = GetPos(nSpecType, CNT_INT_SPECDB_START);
	
	return WriteToFile(m_szSpecDataFile, szBuffer, nSize, nPos);
} 

int		CtmSpecDataG::SaveDBDataPos(int WritePos,int nSpecType)
{
	char szFile[256];
	char szBuffer[CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM];
	
	memset(szBuffer, 0, sizeof(szBuffer));
	//	compose the filename: path + "/nSpecType.cdb"
	GetDBFile(nSpecType, szFile);
	//	get the dbid array
	m_nIDNum = ReadDBID(szFile, m_szDBDataID);	
	//	Get the value from the database, and write to the special buffer
	int nSize = GetBufferFromDB(szBuffer, sizeof(szBuffer), m_szDBDataID, m_nIDNum);
	//printf("nSize=%d\n",nSize);
	//	calcualte the postion base on the spectype&fixednum
	int nPos = GetPos(WritePos, CNT_INT_SPECDB_START);
	
	return WriteToFile(m_szSpecDataFile, szBuffer, nSize, nPos);
}                          

int		CtmSpecDataG::CleanDBData(int WritePos)
{
	char szBuffer[CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM];
	
	memset(szBuffer, 0, sizeof(szBuffer));
	
	int nSize = CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM;
	
	int nPos = GetPos(WritePos, CNT_INT_SPECDB_START);
	
	return WriteToFile(m_szSpecDataFile, szBuffer, nSize, nPos);
}  

int		CtmSpecDataG::GetFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize)
{   
	if (nSize > CNT_INT_FIXDATA_TOTALLEN) nSize = CNT_INT_FIXDATA_TOTALLEN;
		
	int nPos = GetPos(nSpecType, nFixedNum);
	
	         
	return ReadFromFile(m_szSpecDataFile, pBuf, nSize, nPos);     
}                     


// get the data from the spec file, and save to the database 
int		CtmSpecDataG::GetDBData(int nSpecType)
{
	//	check the valid values,(spectype fixednum & nsize)
	if (nSpecType >= tmEndSpecType)			nSpecType = tmEndSpecType - 1;
		
	char szFile[256];
	char szBuffer[CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM];
	
	//	compose the filename: path + "/nSpecType.dec"
	GetDBFile(nSpecType, szFile);
	
	//	get the dbid array
	m_nIDNum = ReadDBID(szFile, m_szDBDataID);	
	
	//	calcualte the postion base on the spectype&fixednum
	int nPos = GetPos(nSpecType, CNT_INT_SPECDB_START);
	
	//	Get the value from the database, and write to the special buffer
	//printf("nPos=%d sizeofBuff=%d \n", nPos, sizeof(szBuffer));
	int nLen = ReadFromFile(m_szSpecDataFile, szBuffer, sizeof(szBuffer), nPos);
	//	write to the database from the special buffer
	return WriteToDB(szBuffer, nLen, m_szDBDataID, m_nIDNum);
	
}     

int		CtmSpecDataG::GetDBDataPos(int WritePos,int nSpecType)
{
	//	check the valid values,(spectype fixednum & nsize)
	if (nSpecType >= tmEndSpecType)			nSpecType = tmEndSpecType - 1;
		
	char szFile[256];
	char szBuffer[CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM];
	
	memset(szBuffer, 0, sizeof(szBuffer));
	//	compose the filename: path + "/nSpecType.dec"
	GetDBFile(nSpecType, szFile);
	
	//	get the dbid array
	m_nIDNum = ReadDBID(szFile, m_szDBDataID);	
	//	calcualte the postion base on the spectype&fixednum
	int nPos = GetPos(WritePos, CNT_INT_SPECDB_START);
	//	Get the value from the database, and write to the special buffer
	int nLen = ReadFromFile(m_szSpecDataFile, szBuffer, sizeof(szBuffer), nPos);
	
	//	write to the database from the special buffer
	return WriteToDB(szBuffer, nLen, m_szDBDataID, m_nIDNum);
	
}                    



int		CtmSpecDataG::SaveFixedDataTo()                        
{
	int nResult = 0;
	for (int i = tmSpecStart; i < tmEndSpecType; i++)
		nResult += SaveFixedDataTo(i);
	
	return nResult;         
}                     


int		CtmSpecDataG::SaveFixedDataTo(int nSpecType)
{
	return WriteSpecTo(m_szSpecDataFile, m_szCopyFile, nSpecType, 0, 
						CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM);
}                     


int		CtmSpecDataG::SaveDBDataTo()
{                     
	int nResult = 0;
	for (int i = tmSpecStart; i < tmEndSpecType; i++)
		nResult += SaveDBDataTo(i);
	return nResult;
}                     

int		CtmSpecDataG::SaveDBDataTo(int nSpecType)
{
	return WriteSpecTo(m_szSpecDataFile, m_szCopyFile, nSpecType, CNT_INT_SPECDB_START, 
							CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM);
}                     


int		CtmSpecDataG::SaveDataTo()
{                     
	return WriteSpecTo(m_szSpecDataFile, m_szCopyFile, tmSpecStart, 0, CNT_INT_SPECDAT_LEN * tmEndSpecType);
}          
                  

int		CtmSpecDataG::SaveAllDataTo()
{       
	int nResult = 0;              
	SaveDataTo();
	
	char	szSourceFile[256];
	char	szDestFile[256];
	for (int i = tmSpecStart; i < tmEndSpecType; i++)
	{
		GetDBFile(i, szSourceFile);
		sprintf(szDestFile, "%s/%d.spc", m_szCopyPath, i);
		CopyFile(szSourceFile, szDestFile);
		
	}
	
	return nResult;         
}                     
int		CtmSpecDataG::GetFixedDataFrom()
{                     
	int nResult = 0;

	for (int i = tmSpecStart; i < tmEndSpecType; i++)
		nResult += GetFixedDataFrom(i);
	return nResult;     
}                     


int		CtmSpecDataG::GetFixedDataFrom(int nSpecType)
{                    
	return WriteSpecTo(m_szCopyFile, m_szSpecDataFile, nSpecType, 0, 
						CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM);          
}                     



int		CtmSpecDataG::GetDBDataFrom()
{                     
	int nResult = 0;
	for (int i =  tmSpecStart; i < tmEndSpecType; i++)
		nResult += GetDBDataFrom(i);
	return nResult;          
}                     




int		CtmSpecDataG::GetDBDataFrom(int nSpecType)
{                     
	return WriteSpecTo(m_szCopyFile, m_szSpecDataFile, nSpecType, CNT_INT_SPECDB_START, 
							CNT_INT_SPECDAT_LEN - CNT_INT_FIXDATA_LEN * CNT_INT_FIXDATA_NUM);
}                     



int		CtmSpecDataG::GetDataFrom()
{
	return WriteSpecTo(m_szCopyFile, m_szSpecDataFile, tmSpecStart, 0, CNT_INT_SPECDAT_LEN * tmEndSpecType);
}                     



int		CtmSpecDataG::GetAllDataFrom()
{                     
	GetDataFrom();
	
	char	szSourceFile[256];
	char	szDestFile[256];
	for (int i = tmSpecStart; i < tmEndSpecType; i++)
	{
		GetDBFile(i, szDestFile);
		sprintf(szSourceFile, "%s/%d.spc", m_szCopyPath, i);
		CopyFile(szSourceFile, szDestFile);
		
	}
	return 0;         
}                     


int		CtmSpecDataG::WriteToFile(char* pszFile, char* pBuf, int nSize, int nPos)
{
	int nResult = -1;
	if (pBuf != NULL && nSize > 0 && nPos >= 0 && 
		access(pszFile, R_OK) == 0)
	{
		int nFd = open(pszFile, O_RDWR);
		lseek(nFd, nPos, SEEK_SET);
		nResult = write(nFd, pBuf, nSize);
		
		close(nFd);
	}
	//printf("nResult=%d nSize=%d nPos=%d \n",nResult, nSize, nPos);
	return nResult;
}

int		CtmSpecDataG::ReadFromFile(char* pszFile, char* pOut, int nSize, int nPos)
{
	int nResult = -1;
	if (pszFile != NULL && pszFile[0] != '\0' &&  
		nSize > 0 && nPos >= 0 && 
		access(pszFile, R_OK) == 0)
	{
		int nFd = open(pszFile, O_RDWR);
		lseek(nFd, nPos, SEEK_SET);
		nResult = read(nFd, pOut, nSize);
		close(nFd);
	}
	//printf("read nResult=%d nSize=%d nPos=%d \n", nResult, nSize, nPos);
	return nResult;
}

int		CtmSpecDataG::GetPos(int nSpecType, int nFixedNum)
{
	return nSpecType * CNT_INT_SPECDAT_LEN + CNT_INT_FIXDATA_LEN * nFixedNum;
}

int		CtmSpecDataG::ReadDBID(char* pszIDFile, char* pszID[])
{
	int nResult = 0;
	
	if (pszIDFile != NULL && pszIDFile[0] != '\0' && access(pszIDFile, R_OK) == 0)
	{
		m_ReadData.OpenFileStr(pszIDFile);
		nResult = m_ReadData.ReadData(pszID);
		m_ReadData.CloseFileStr();
	}
	
	return nResult;
}

int		CtmSpecDataG::GetDBFile(int nSpecType, char* pszOutFileName)
{
	int nResult = -1;
	sprintf(pszOutFileName, "%s/%d.spc", m_szSpecScriptPath, nSpecType);	
	if (access(pszOutFileName, R_OK) == 0) 
		nResult = 0;
	//else pszOutFilename[0] = '\0';
	return nResult;
}

int		CtmSpecDataG::GetBufferFromDB(char* pOut,int nSize, char* pszID[], int nNum)
{
	int nResult = 0;
	//char	cszStringID[128];
	
	DBVALUE	dbValue;
	for (int i = 0; i < nNum; i++)
	{
		memset(&dbValue, 0, sizeof(dbValue));
		if (nResult >= nSize) break;
		dbValue = g_pDatabase->Read(pszID[i]);
		//printf("pszID[%d]:%s Value=%d length=%d \n",i,pszID[i], dbValue.lData, dbValue.DataType.wLength);
		if (dbValue.DataType.wType != TYPE_STRING)
			memcpy(pOut + nResult, dbValue.acData, dbValue.DataType.wLength);
		else
		{
			//memset(cszStringID, 0, sizeof(cszStringID));
			//g_pDatabase->Reads(g_pDatabase->GetIndex(pszID[i]));
			memcpy(pOut + nResult, g_pDatabase->Reads(g_pDatabase->GetIndex(pszID[i])), dbValue.DataType.wLength);
		}
		nResult += dbValue.DataType.wLength;
	}
	return nResult;
}

int		CtmSpecDataG::WriteToDB(char* pIn, int nSize, char* pszID[], int nNum)
{
	int nResult = 0;
	DBVALUE	dbValue;
	for (int i = 0; i < nNum; i++)
	{
		if (nResult >= nSize) break;
		memset(&dbValue, 0 , sizeof(dbValue));
		dbValue = g_pDatabase->Read(pszID[i]);
		
		if (dbValue.DataType.wType != TYPE_STRING)
		{
			memcpy(dbValue.acData, pIn + nResult, dbValue.DataType.wLength);
			g_pDatabase->Write(pszID[i], dbValue.acData, TRUE);
		}
		else
			g_pDatabase->Writes(pIn + nResult, g_pDatabase->GetIndex(pszID[i]), dbValue.DataType.wLength, TRUE);
		 //printf("%s\n",pIn + nResult);
		nResult += dbValue.DataType.wLength;
	}
	
	return nResult;
	
}

int		CtmSpecDataG::WriteSpecTo(char* pszSource, char* pszDest, int nSpecType, int nFixedNum, int nSize)
{
	int nResult = 0;
	int nPos	= 0;
	int nReadLen= 0;
	char* pbuf  = new char[nSize];
	
	#ifdef	D_3354
		sprintf(m_szCopyPath, "%s", "/media/sda1/spec");
	#else
		sprintf(m_szCopyPath, "%s", "/mnt/usb/spec");
	#endif	
	
	ExistDir(m_szCopyPath);
	
	sprintf(m_szCopyFile, "%s/spec.cdb", m_szCopyPath);
	if (access(m_szCopyFile, R_OK) != 0)
	{
		int nFd= open(m_szCopyFile, O_RDWR | O_CREAT);//creat(m_szSpecDataFile,777);
		ftruncate(nFd, tmEndSpecType * CNT_INT_SPECDAT_LEN);
		close(nFd);
	}
	
	//	calculate the postion base on the spectype & fixednum
	nPos 		= GetPos(nSpecType, nFixedNum);
	//	read buffer from the local spec file
	nReadLen 	= ReadFromFile(pszSource,  pbuf, nSize, nPos);
	//	write buffer to the special spec file
	nResult		= WriteToFile(pszDest, pbuf, nReadLen, nPos);
	delete []pbuf;
	
	return nResult;
}

int		CtmSpecDataG::CopyFile(char* pszSource, char* pszDest)
{
	int nResult = -1;
	
	if (pszSource == NULL || pszSource[0] == '\0' ||
		pszDest == NULL || pszDest[0] == '\0' ||
		access(pszSource, R_OK) != 0)
		return nResult;
	
	char	szCommand[512];
	sprintf(szCommand, "cp -rf %s %s", pszSource, pszDest);
	nResult = system(szCommand);
	return nResult;
}


#ifndef	D_BEOBJECT_CTMDEFAULTG

CObject*	CreateInstance(char* Name)
{
	if (g_pFreeDataG == NULL)
	{
		CObject*	pResult = Create(Name);
		
		if (pResult != NULL)
		{
			g_nFreeDataCount++;
			g_pFreeDataG = static_cast<CtmFreeDataG*>(pResult);			
			g_pFreeDataG->CreateSelf();	
		}		
	}
	else g_nFreeDataCount++;
		
		
	if (g_pSpecData == NULL)
	{
		CObject* pResult = Create("CtmSpecDataG");
		if (pResult != NULL)
		{
			g_pSpecData = static_cast<CtmSpecDataG*>(pResult);		
			g_pSpecData->CreateSelf();
		}
	}
		
	////printf("g_pSelevatG=%X\n", g_pFreeDataG);
	
	return static_cast<CObject*>(g_pFreeDataG);
}

void	ReleaseInstance(CObject* pInstance)
{
	if (g_nFreeDataCount > 0)
		g_nFreeDataCount--;
	
	if (g_pFreeDataG <= 0)
	{
		if (g_pFreeDataG != NULL)
			g_pFreeDataG->FreeSelf();
		delete g_pFreeDataG;
		g_pFreeDataG = NULL;
	}
}
#endif


