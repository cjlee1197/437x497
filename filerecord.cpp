/* Copyright (C), 2015-2020, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CFileRecord					   				  				|
|  Task     : CFileRecord code file                	   			      		|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 4.7                                                   |
|  Link     :G++(GCC) 4.7                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James	                                                   		|
|  Version  : V1.00                                                         |
|  Creation : 2016.03.08												    |
|  Revision :                                                               |
+==========================================================================*/

#include 	"filerecord.h"
#include 	"cfont.h"
#include	"tmcontrol.h"
#include	"recordset.h"
#include	"./lib/libDataSave.h"
#include	"oper_log.h"

tmFileRecordInfo	CManageFile::m_aFileInfo[] =
{
	//Filetype, 		   HeadSize,  Vers, Fileformat, nCount, FileName
	{FILE_RECORD_MONI, 		8,	0,	0001, 0, 10000, 	"Moni.cdb"},
	{FILE_RECORD_ALARM, 	8,	0,	0001, 0, 10000, 	"Alarm.cdb"},
	{FILE_RECORD_MODIFYLOG,	8,	0,	0001, 0, 10000, 	"ModifyLog.cdb"},
	{FILE_RECORD_LINESTATE, 8,	0,	0001, 0, 10000, 	"LineState.cdb"},
	{FILE_RECORD_OPSTATE, 	8,	0,	0001, 0, 10000, 	"OPState.cdb"},
	{FILE_RECORD_ENEGERY, 	8,	0,	0001, 0, 10000, 	"Enegery.cdb"},
	{FILE_RECORD_POWERONOFF,8,	0,	0001, 0, 10000, 	"PowerOnOff.cdb"},
	{FILE_RECORD_ENEGERY2, 	8,	0,	0001, 0, 10000, 	"Enegery2.cdb"},
	{FILE_RECORD_PPH, 		8,	0,	0001, 0, 10000, 	"PPH.cdb"},
	{FILE_RECORD_OPER_LOG, 		8,	0,	0001, 0, 10000, 	"OperLog.cdb"},
};

CManageFile*	CManageFile::m_pInstance = NULL;
/*---------------------------------------------------------------------------+
|  Function : CFileRecord::CFileRecord(const char* pszFileName)              |
|  Task     : ???的构造函?                                               |
+----------------------------------------------------------------------------+
|  Call     :								                                 |
|                                                                            |
|  Parameter: 要??的文件名?               -                               |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CFileRecord::CFileRecord(const char* pszFileName)
{
	m_IsClosed = true;
	OpenFile(pszFileName);
}

/*---------------------------------------------------------------------------+
|  Function : CFileRecord::~CFileRecord() 					                 |
|  Task     : 析构函?                                               		 |
+----------------------------------------------------------------------------+
|  Call     :								                                 |
|                                                                            |
|  Parameter: ?                      -                               		 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CFileRecord::~CFileRecord()
{
	if (!m_IsClosed) 
	{
		CloseFile();
	}
}

bool CFileRecord::OpenFile(const char* pszFileName)
{
	if (!m_IsClosed) CloseFile();
	
	m_Fd = open(pszFileName, O_RDWR | O_CREAT);
	m_IsClosed = false;	
	return true;
}

void CFileRecord::CloseFile()
{
	close(m_Fd);
	m_Fd = -1;
	m_IsClosed = true;
}

CFileRecord::CFileRecord()
{	
	m_IsClosed 	= true;
}


int	CFileRecord::WriteRecord(int nPos, void* pRecord, int nSize)
{
	int	nResult = -1;
	if(m_Fd >=0 && pRecord != NULL)
	{
		lseek(m_Fd, nPos, SEEK_SET);
		nResult = write(m_Fd, (char*)pRecord, nSize);
	}
	return nResult;
}

int	CFileRecord::WriteRecordM(int nPos, void* pRecord, int nSize)
{
	int	nResult = -1;
	if(m_Fd >=0 && pRecord != NULL)
	{
		nResult = write(m_Fd, (char*)pRecord, nSize);
	}
	return nResult;
}

int	CFileRecord::ReadRecord(int nPos, void* pRecord, int nSize)
{
	int	nResult = -1;
	if(m_Fd >=0 && pRecord != NULL)
	{
		lseek(m_Fd, nPos, SEEK_SET);
		nResult = read(m_Fd, (char*)pRecord, nSize);
	}
	return nResult;
}

int	CFileRecord::ReadRecordM(int nPos, void* pRecord, int nSize)
{
	int	nResult = -1;
	if(m_Fd >=0 && pRecord != NULL)
	{
		nResult = read(m_Fd, (char*)pRecord, nSize);
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : CManageFile::CManageFile()                                     |
|  Task     : ???的构造函?                                                   |
+----------------------------------------------------------------------------+
|  Call     :								                                 |
|                                                                            |
|  Parameter:                             -                                  |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CManageFile::CManageFile()
{
	Init();
}

CManageFile::~CManageFile()
{
	
}

CManageFile* CManageFile::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new CManageFile();
	}
	return m_pInstance;
}

void	CManageFile::ReleaseInstance()
{
	if(m_pInstance != NULL)
		delete m_pInstance;
	m_pInstance = NULL;
}

void	CManageFile::Init()
{
	for(int i = 0; i < FILE_MAXCOUNT; i++)
	{
		m_pFileInstance[i] 	= NULL;
	}
	
	m_aFileInfo[FILE_RECORD_MONI].nSize 		= GetMoniStructSize();
	m_aFileInfo[FILE_RECORD_ALARM].nSize 		= sizeof(tmDB_ERROR);
	m_aFileInfo[FILE_RECORD_MODIFYLOG].nSize 	= sizeof(tmModifyRecordSet_Oil);
	m_aFileInfo[FILE_RECORD_LINESTATE].nSize	= sizeof(ONOFFSTATE);
	m_aFileInfo[FILE_RECORD_OPSTATE].nSize		= sizeof(STATEMODE);					
	m_aFileInfo[FILE_RECORD_ENEGERY].nSize		= sizeof(TMCAPACITY);
	m_aFileInfo[FILE_RECORD_POWERONOFF].nSize	= sizeof(TMPOWERONOFF);
	m_aFileInfo[FILE_RECORD_ENEGERY2].nSize		= sizeof(AMMETERDATA);
	m_aFileInfo[FILE_RECORD_PPH].nSize			= sizeof(TMPPHDATA);
	m_aFileInfo[FILE_RECORD_OPER_LOG].nSize		= sizeof(tmOper_log);
}


CFileRecord*		CManageFile::GetFileInstance(int nType)
{
	char	pszFileName[100];
	char	pszFilePath[256];
	
	memset(pszFileName, 0, sizeof(pszFileName));
	memset(pszFilePath, 0, sizeof(pszFilePath));
	
	if(CtmConfig::GetInstance()->GetRecordFilePath() != NULL)
		strcpy(pszFilePath, CtmConfig::GetInstance()->GetRecordFilePath());
	sprintf(pszFileName, "%s%s", pszFilePath, m_aFileInfo[nType].pszFileName);
	//printf("pszFilePath=%s pszFileName=%s \n", pszFilePath, pszFileName);
	//strcpy(pszFileName, m_aFileInfo[nType].pszFileName);
	if(m_pFileInstance[nType] == NULL)
	{
		//printf("pszFileName=%s \n", pszFileName);
		m_pFileInstance[nType] = new CFileRecord(pszFileName);
	}
	return m_pFileInstance[nType];

}

int		CManageFile::WriteRecord(int	nType, int nRecordNo, void* pRecord, int nLength)
{
	int	nResult = 0;
	CFileRecord*	pFileRecord = NULL;
	int	nHeadLen = 0, nPos = 0, nSize = 0;
	
	if(nType < 0 || nType > FILE_MAXCOUNT)
		return -1;
	
	pFileRecord = GetFileInstance(nType);
	nHeadLen	= m_aFileInfo[nType].nHeadSize;
	nSize		= m_aFileInfo[nType].nSize;
	if(pFileRecord != NULL)		
	{
		if(nRecordNo == 0)	//WriteHead
		{
			//if(nLength <= nSize)
				pFileRecord->WriteRecord(0 + 4, pRecord, nHeadLen);
		}
		else
		{
			if(nLength <= nSize)
			{
				nPos = (nRecordNo-1)* nSize + nHeadLen + 4;
				nResult = pFileRecord->WriteRecord(nPos, pRecord, nLength);
				//printf("write nPos=%d nLength=%d nResult=%d \n", nPos, nLength, nResult);
			}
			else
			{
				printf("write length error \n");
			}
		}
	}
	
	return nResult;
}

int		CManageFile::ReadRecord(int nType,  int nRecordNo, void* pRecord, int nLength)
{
	int	nResult = 0;
	CFileRecord*	pFileRecord = NULL;
	int	nHeadLen = 0, nPos = 0, nSize = 0;
	
	if(nType < 0 || nType > FILE_MAXCOUNT)
		return -1;
	
	//printf("read type=%d nRecordNo=%d nLength=%d\n",nType, nRecordNo, nLength);
	pFileRecord = GetFileInstance(nType);
	nHeadLen	= m_aFileInfo[nType].nHeadSize;
	nSize		= m_aFileInfo[nType].nSize;
	if(pFileRecord != NULL)		
	{
		if(nRecordNo == 0)	//ReadHead
		{
			//if(nLength <=nHeadLen) 
				nResult = pFileRecord->ReadRecord(0+4, pRecord, nHeadLen);
				
		}
		else
		{
			if(nLength <= nSize)
			{
				nPos = (nRecordNo-1)* nSize + nHeadLen + 4;
				nResult = pFileRecord->ReadRecord(nPos, pRecord, nLength);
				//printf("pos=%d nSize=%d nResult=%d \n", nPos, nSize, nResult);
			}
			else
			{
				printf("read length error nLength=%d nSize=%d \n", nLength, nSize);
			}
		}
	}
	return nResult;
}

int		CManageFile::DeleteRecord(int nType)
{
	int	nResult = 0;
	CFileRecord*	pFileRecord = NULL;
	int	nHeadLen = 0, nPos = 0, nVers = 0, nReadVers = 0;
	tmFileRecordHeadInfo	HeadInfo;
	
	if(nType < 0 || nType > FILE_MAXCOUNT)
		return -1;
	
	pFileRecord = GetFileInstance(nType);
	nHeadLen	= m_aFileInfo[nType].nHeadSize;
	nVers		= m_aFileInfo[nType].nVers;
	if(pFileRecord != NULL)		
	{
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		pFileRecord->WriteRecord(0, &nVers, sizeof(nVers));
		pFileRecord->WriteRecord(0+4, &HeadInfo, sizeof(HeadInfo));
		nResult = TRUE;
	}
	return nResult;
}

int		CManageFile::CheckData(int nType)
{
	int	nResult = 0;
	CFileRecord*	pFileRecord = NULL;
	int	nHeadLen = 0, nPos = 0, nVers = 0, nReadVers = 0;
	tmFileRecordHeadInfo	HeadInfo;
	
	if(nType < 0 || nType > FILE_MAXCOUNT)
		return -1;
	
	pFileRecord = GetFileInstance(nType);
	nHeadLen	= m_aFileInfo[nType].nHeadSize;
	nVers		= m_aFileInfo[nType].nVers;
	if(pFileRecord != NULL)		
	{
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		pFileRecord->ReadRecord(0, &nReadVers, sizeof(nReadVers));
		//printf("HeadInfo.nVers=%d nVers =%d \n", nReadVers, nVers);
		if(nReadVers != nVers)
		{
			pFileRecord->WriteRecord(0, &nVers, sizeof(nVers));
			pFileRecord->WriteRecord(0+4, &HeadInfo, sizeof(HeadInfo));
			nResult = FALSE;
		}
		else
		{
			nResult = TRUE;
		}
	}
	return nResult;
}
