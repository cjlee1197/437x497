/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmDeBug 											             |
|  Task     : all CtmDeBug's write and read			                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision : 							                                     |
+===========================================================================*/

#include	<stdarg.h>	//	fans add
//#include	<time.h>
#include	"tmdebug.h"
//#include	"utils.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
ItmDebugInfo*		g_tmDebugInfo = CtmDebuggerCpp::GetInstance()->GetDebugInfoInterface();

CtmDebuggerCpp*	 CtmDebuggerCpp::m_pInstance = NULL;
int				 CtmDebuggerCpp::m_nCount = 0;

/*==========================================================================+
|           Class implementation - CtmDebugFile                                 	|
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                     	|
+--------------------------------------------------------------------------*/
CtmDebugFile::CtmDebugFile()
{
	m_fd = -1;
}

CtmDebugFile::CtmDebugFile(char *pszName, int flags)
{
	Open(pszName, flags);
}

CtmDebugFile::~CtmDebugFile()
{
	if(m_fd >= 0)
		Close();
}
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*==========================================================================+
|	function:	Open(const char *pszName, int flags)						|
|	remark	:	Open file													|
|---------------------------------------------------------------------------|
|	call	:	Open(pszName, flags)										|
|	parameter:	pszName		:	File Name									|
|				flags		: 	open file flags								|
|																			|
|	return	:	0			:  	success										|
|				¡L?		: 	error										|
+==========================================================================*/			
int		CtmDebugFile::Open(const char *pszName, int flags)
{
	if(m_fd >= 0)	Close();
	m_fd	= open(pszName, flags);
	if(m_fd < 0)
	{
//		//printf("Open %s : %s\n", pszName, strerror(errno));
		m_fd	= 0;		
		return -1;
	}
	fchmod(m_fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	
	return 0;
}

/*==========================================================================+
|	function:	Close()														|
|	remark	:	Close file													|
|---------------------------------------------------------------------------|
|	call	:	Close()														|
|																			|
|	return	:	0			:  	success										|
|				¡L?		: 	error										|
+==========================================================================*/
int		CtmDebugFile::Close()
{
	int		nResult;
	
	if(m_fd <= 0)	return -1;	
	nResult	= close(m_fd);	
	m_fd	= 0;
	
	return nResult;
}

/*==========================================================================+
|	function:	Read(const void *buf, long count)							|
|	remark	:	Read file													|
|---------------------------------------------------------------------------|
|	call	:	Read(buf, count)											|
|	para	:	Buf			:	read buf									|
|				count		: 	Max read count								|
|																			|
|	return	:	0			:  	eof											|
|				>0			:	read count									|
|				-1			: 	error										|
+==========================================================================*/
long	CtmDebugFile::Read(void *buf, long count)
{
	if(m_fd <=0)	return -1;
	return	read(m_fd, buf, count);
}

/*==========================================================================+
|	function:	Write(const void *buf, long count)							|
|	remark	:	Write file													|
|---------------------------------------------------------------------------|
|	call	:	Write(buf, count)											|
|	para	:	Buf			:	Write buf									|
|				count		: 	Max Write count								|
|																			|
|	return	:	>0			:	Write count									|
|				-1			: 	error										|
+==========================================================================*/
long	CtmDebugFile::Write(const void *buf, long count)
{
	if(m_fd <=0)	return -1;
	return write(m_fd, buf, count);
}

/*==========================================================================+
|	function:	Seek(long Offset, int whence)								|
|	remark	:	move file point to offset									|
|---------------------------------------------------------------------------|
|	call	:	Seek(Offset, whence)										|
|	para	:	Offset		:	Offset										|
|				whence		: 	SEEK_SET		:							|
|							:	SEEK_CUR		:							|
|							:	SEEK_END		:							|
|																			|
|	return	:	>0			:	file point position							|
|				-1			: 	error										|
+==========================================================================*/
long	CtmDebugFile::Seek(long Offset, int whence)
{
	if(m_fd <=0)	return -1;
	return lseek(m_fd, Offset, whence);
}

/*==========================================================================+
|	function:	Truncate(long Length)										|
|	remark	:	Change file Length											|
|---------------------------------------------------------------------------|
|	call	:	Truncate(Length)											|
|	para	:	Length		:	New Length									|
|																			|
|	return	:	0			:	success										|
|				-1			: 	error										|
+==========================================================================*/
int		CtmDebugFile::Truncate(long Length)
{
	if(m_fd <=0)	return -1;
	return ftruncate(m_fd, Length);
}




/*---------------------------------------------------------------------------+
|  Function : void GetDebugTime(TIME* ptime)                                      |
|  Task     : Get system time                                                |
+----------------------------------------------------------------------------+
|  Call     : GetDebugTime(ptime)                                                 |
|                                                                            |
|  Parameter: ptime                     - Pointer to TIME structure          |
|                                                                            |
|  Return   : *ptime                    - Current system time                |
+---------------------------------------------------------------------------*/
void        CtmDebugInfoBasic::GetDebugTime(tmDebugTime* ptime)
{
	time_t			Time;
	struct	tm		tmTime;
	
	time(&Time);
	tmTime	=*localtime(&Time);
	
	ptime->hour		= tmTime.tm_hour;
	ptime->minute	= tmTime.tm_min;
	ptime->second	= tmTime.tm_sec;
	ptime->hsecond	= 0;
}

/*---------------------------------------------------------------------------+
|  Function : void GetDebugTime(char* pszTime, char cSeparator)                   |
|  Task     : Get system time                                                |
+----------------------------------------------------------------------------+
|  Call     : GetDebugTime(pszTime, cSeparator)                                   |
|                                                                            |
|  Parameter: pszTime                   - Pointer to time string             |
|             cSeparator                - Separator among the H,M,S          |
|                                                                            |
|  Return   : *pszTime                  - Current system time                |
|                                         Format : HH:MM:SS                  |
+---------------------------------------------------------------------------*/
void 	CtmDebugInfoBasic::GetDebugTime(char* pszTime, char cSeparator)
{
	tmDebugTime        time;
	
	GetDebugTime(&time);
	if (cSeparator != 0)
	    sprintf(pszTime, "%02u%c%02u%c%02u", time.hour, cSeparator, time.minute, cSeparator, time.second);
	else
	    sprintf(pszTime, "%02u%02u%02u", time.hour, time.minute, time.second);
}

/*---------------------------------------------------------------------------+
|  Function : void GetDebugDate(DATE* pdate)                                      |
|  Task     : Get system date                                                |
+----------------------------------------------------------------------------+
|  Call     : GetDebugDate(pdate)                                                 |
|                                                                            |
|  Parameter: pdate                     - Pointer to DATE structure          |
|                                                                            |
|  Return   : *pdate                    - Current system date                |
+---------------------------------------------------------------------------*/
void        CtmDebugInfoBasic::GetDebugDate(tmDebugDate* pdate)
{
	time_t			Time;
	struct	tm		tmTime;
	
	time(&Time);
	tmTime	=*localtime(&Time);
	
	pdate->day		= tmTime.tm_mday;
	pdate->month	= tmTime.tm_mon 	+ 1;
	pdate->year		= tmTime.tm_year 	+ 1900;
	pdate->dayofweek= tmTime.tm_wday;
}

/*---------------------------------------------------------------------------+
|  Function : void GetDebugDate(char* pszDate, char cSeparator)                   |
|  Task     : Get system date                                                |
+----------------------------------------------------------------------------+
|  Call     : GetDebugDate(pszDate, cSeparator)                                   |
|                                                                            |
|  Parameter: pszDate                   - Pointer to date string             |
|             cSeparator                - Separator among the year,month,day |
|                                                                            |
|  Return   : *pszDate                  - Current system date                |
|                                         Format : MM/DD/YY                  |
+---------------------------------------------------------------------------*/
void        CtmDebugInfoBasic::GetDebugDate(char* pszDate, char cSeparator)
{
	tmDebugDate        date;
	
	GetDebugDate(&date);
	
	if (cSeparator != 0)
	    sprintf(pszDate, "%02u%c%02u%c%02u", date.month, cSeparator, date.day, cSeparator, date.year%100);
	else
	    sprintf(pszDate, "%02u%02u%02u", date.month, date.day, date.year%100);
}

/*==========================================================================+
|           Class implementation - CtmDebugInfoBasic   	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmDebugInfoBasic::CtmDebugInfoBasic()
{
	m_szFileName[0] = '\0';
	m_nFd = 0;
}


CtmDebugInfoBasic::~CtmDebugInfoBasic()
{
	CloseDebugLog();
}


int		CtmDebugInfoBasic::PrintDebugInfo(const char* fmt , ... )
{
	#ifndef D_DEBUG_TM
		return 0;
	#endif
	char   buf[1024];   
	va_list   ap;   
	va_start(ap,   fmt);   
	int   len   =   vsprintf(buf,   fmt,   ap);   
	va_end(ap);
	return printf(buf);
}

int		CtmDebugInfoBasic::LogDebugInfo(char* pszFileName, const char* fmt, ...)
{
	#ifndef D_DEBUG_TM
		return 0;
	#endif
	
	
	char   buf[1024];   
	va_list   ap;   
	va_start(ap,   fmt);   
	int   len   =   vsprintf(buf,   fmt,   ap);   
	va_end(ap);
	
	
	OpenDebugLog(pszFileName);
	
	if (m_File.GetFileHandle() > 0)
	{
		char	szDate[128];
		char	szTime[128];
		GetDebugDate(szDate,'-');
		GetDebugTime(szTime,':');
		char	szLine[1024 + 256];
		sprintf(szLine, "%s %s DebugInfo: %s\n", szDate, szTime, buf);
		//m_File.AppendLine(szLine);
		m_File.Seek(0, SEEK_END);
		m_File.Write(szLine, strlen(szLine));
		
		//if (m_File.Seek(0, SEEK_END) > 1024 * 1024)
		//	m_File.Truncate(1024 * 1024);
	}

	return 0;
}

int		CtmDebugInfoBasic::PrintLogInfo(char* pszFileName, const char* fmt, ...)
{
	#ifndef D_DEBUG_TM
		return 0;
	#endif
	char   buf[1024];   
	va_list   ap;   
	va_start(ap,   fmt);   
	int   len   =   vsprintf(buf,   fmt,   ap);   
	va_end(ap);
	printf(buf);
	
	OpenDebugLog(pszFileName);
	
	if (m_File.GetFileHandle() > 0)
	{
		char	szDate[128];
		char	szTime[128];
		GetDebugDate(szDate,'-');
		GetDebugTime(szTime,':');
		char	szLine[1024 + 256];
		sprintf(szLine, "%s %s DebugInfo: %s\n", szDate, szTime, buf);
		//m_File.AppendLine(szLine);
		m_File.Seek(0, SEEK_END);
		m_File.Write(szLine, strlen(szLine));
		//if (m_File.Seek(0, SEEK_END) > 1024 * 1024)
		//	m_File.Truncate(1024 * 1024);
	}
	
	return 0;
}

int		CtmDebugInfoBasic::LogData(char* pszFileName, void* pData, int	nSize)
{
	#ifndef D_DEBUG_TM
		return 0;
	#endif
	
	if (pszFileName == NULL) return 0;
	
	int	nResult = 0;
	
	CtmDebugFile	DataFile;
	
	DataFile.Open(pszFileName, O_RDWR | O_CREAT);
	DataFile.Seek(0, SEEK_END);
	nResult = DataFile.Write(pData, nSize);
	DataFile.Close();
	
	return nResult;	
}

int		CtmDebugInfoBasic::ClearLog(char* pszFileName)
{
	#ifndef D_DEBUG_TM
		return 0;
	#endif
	OpenDebugLog(pszFileName);
	
	if (m_File.GetFileHandle() > 0)
	{
		//m_File.Clear();
		m_File.Truncate(0);
	}
}

bool	CtmDebugInfoBasic::IsOldFile(char* pszFileName)
{
	bool	bResult = false;
	
	if (pszFileName == NULL)
		bResult = strcmp(m_szFileName, DEBUG_FILENAME) ==0;
	else 
		bResult = strcmp(m_szFileName, pszFileName) == 0;
	
	return bResult;
}


int		CtmDebugInfoBasic::OpenDebugLog(char* pszFileName)
{
	if (!IsOldFile(pszFileName))
	{
		CloseDebugLog();
		if (pszFileName == NULL)
			strcpy(m_szFileName, DEBUG_FILENAME)	;
		else 
			strcpy(m_szFileName, pszFileName);
			
		m_File.Open(m_szFileName, O_RDWR | O_CREAT);
		m_nFd = m_File.GetFileHandle();
	}
	return 0;
}

int		CtmDebugInfoBasic::CloseDebugLog()
{
	if (m_nFd > 0)
	m_File.Close();
	
	m_nFd = 0;
	return 0;
}

/*==========================================================================+
|           Class implementation - CtmDebugClassMemoryBasic                 |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmDebugClassMemoryBasic::CtmDebugClassMemoryBasic()
{
	
}


CtmDebugClassMemoryBasic::~CtmDebugClassMemoryBasic()
{
	
}

/*==========================================================================+
|           Class implementation - CtmDebugBreakPointBasic                  |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmDebugBreakPointBasic::CtmDebugBreakPointBasic()
{
	
}


CtmDebugBreakPointBasic::~CtmDebugBreakPointBasic()
{
	
}

/*==========================================================================+
|           Class implementation - CtmDebugBreakPointBasic                  |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmDebugWatchBasic::CtmDebugWatchBasic()
{
	
}


CtmDebugWatchBasic::~CtmDebugWatchBasic()
{
	
}

/*==========================================================================+
|           Class implementation - CtmDebuggerCpp		                 |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmDebuggerCpp::CtmDebuggerCpp()
{
	m_pIDebugInfo = NULL;
	m_pIDebugMemory = NULL;
	m_pIDebugBreak = NULL;
	m_pIDebugWatch = NULL;
	
	ItmDebugInfo* pIDebugInfo = new CtmDebugInfoBasic(); 
	SetDebugInfoInterface(pIDebugInfo);
	SetDebugMemoryInterface(NULL);       
	SetDebugBreakPointInterface(NULL);
	SetDebugWatchInterface(NULL);               

}


CtmDebuggerCpp::~CtmDebuggerCpp()
{
	delete m_pIDebugInfo;  
	delete m_pIDebugMemory;
	delete m_pIDebugBreak; 
	delete m_pIDebugWatch;
}


CtmDebuggerCpp*	CtmDebuggerCpp::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CtmDebuggerCpp();
		
	m_nCount++;
	return m_pInstance;
	
}

void CtmDebuggerCpp::ReleaseInstace()
{
	m_nCount--;
	if (m_nCount <= 0) 
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


void	CtmDebuggerCpp::SetDebugInfoInterface(ItmDebugInfo* pIDebugInfo)
{
	m_pIDebugInfo = pIDebugInfo;
}

ItmDebugInfo* 			CtmDebuggerCpp::GetDebugInfoInterface()
{
	return m_pIDebugInfo;
}
