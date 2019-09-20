/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmDeBug			                                             |
|  Task     : CtmDeBug header file					                         |
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


#ifndef	D_TMDEBUGCLASS
#define	D_TMDEBUGCLASS
#include	<cstddef>
#include 	<string.h>
#include	<time.h>
#include    <stdio.h>
#include	<stdlib.h>
#include 	<sys/types.h>
#include 	<sys/stat.h>
#include 	<fcntl.h>
#include 	<unistd.h>



#define		DEBUG_FILENAME	 "/work/Debug.log"

typedef	struct tagtmDebugDATE
{
	unsigned char	day;
	unsigned char	month;
	unsigned short	year;
	unsigned char	dayofweek;
}	tmDebugDate;

typedef	struct	tagtmDebugTIME
{
	unsigned char	hour;
	unsigned char	minute;
	unsigned char	second;
	unsigned char	hsecond;
}	tmDebugTime;


//	在調試的時候，指定的打印以及其他保存在文件中的信息
typedef struct tagIDebugInfo
{
	virtual	int		PrintDebugInfo(const  char* fmt , ... ) 				= 0;
	virtual	int		LogDebugInfo(char* pszFileName, const  char* fmt, ...) 	= 0;
	virtual	int		PrintLogInfo(char* pszFileName, const  char* fmt, ...) 	= 0;
	virtual	int		ClearLog(char* pszFileName)								= 0;	
	virtual	int		LogData(char* pszFileName, void* pData, int	nSize)		= 0;
} ItmDebugInfo;

//	在調試的時候，使用的內存分配文件，需要說明的是，僅僅對類或結構有效
typedef struct tagIDebugClassMemory
{
	//virtual	void*	operator	new(size_t size)			{return ::new(size);}			;
	//virtual	void*	operator	new[](siz_t size)			{return ::new[](size);}			;
	//virtual	void	operator	delete(void* p)				{return ::delete(p);}			;
	//virtual	void	operator	delete[](void* p)			{return ::delete[] (p);}		;
	
} ItmDebugClassMemory;

//	在調試的時候，加入的斷點
typedef struct tagItmDebugBreakPoint
{
	
} ItmDebugBreakPoint;

//	調試的時候，對堆棧的監控
typedef struct tagItmDebugWatch
{
	
} ItmDebugWatch;


class	CtmDebugFile
{
public:
	CtmDebugFile();
	CtmDebugFile(char *pszName, int flags = O_RDWR);
	~CtmDebugFile();
public:
	int		Open(const char *pszName, int flags = O_RDWR);
	int 	Close();
	
	long	Read(void *buf, long count);
	long	Write(const void *buf, long count);
	long	Seek(long Offset, int whence);
	int		Truncate(long Length);
	int		GetFileHandle()			{return m_fd;};
private:
	int		m_fd;							// file Descriptor
};

class	CtmDebugInfoBasic: public ItmDebugInfo
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
public:
	CtmDebugInfoBasic();
	virtual	~CtmDebugInfoBasic();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
public:
	int		PrintDebugInfo(const char* fmt , ... );
	int		LogDebugInfo(char* pszFileName, const  char* fmt, ...);
	int		PrintLogInfo(char* pszFileName, const  char* fmt, ...);
	int		ClearLog(char* pszFileName);
	
	int		LogData(char* pszFileName, void* pData, int	nSize);
protected:
	bool	IsOldFile(char* pszFileName);
	int		OpenDebugLog(char* pszFileName);
	int		CloseDebugLog();
	
	void	GetDebugTime(tmDebugTime* ptime);
	void	GetDebugTime(char* pszTime, char cSeparator);
	void	GetDebugDate(tmDebugDate* pdate);
	void	GetDebugDate(char* pszDate, char cSeparator);
	
protected:
	char	m_szFileName[512];
	int		m_nFd;
	CtmDebugFile	m_File;
	
	
private:
	//static	const	unsigned char*	m_pszDebugDefaultFile = "/usr/tmDebugInfo.txt";
};

class	CtmDebugClassMemoryBasic: public ItmDebugClassMemory
{
public:
	CtmDebugClassMemoryBasic();
	virtual	~CtmDebugClassMemoryBasic();
};

class	CtmDebugBreakPointBasic: public ItmDebugBreakPoint
{
public:
	CtmDebugBreakPointBasic();
	virtual	~CtmDebugBreakPointBasic();
};

class	CtmDebugWatchBasic: public ItmDebugWatch
{
public:
	CtmDebugWatchBasic();
	virtual	~CtmDebugWatchBasic();
};


class	CtmDebuggerCpp
{
	
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
protected:
	CtmDebuggerCpp();
	virtual ~CtmDebuggerCpp();
	
public:
	static	CtmDebuggerCpp*	GetInstance();
	static	void			ReleaseInstace();
	
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
public:
	ItmDebugInfo* 			GetDebugInfoInterface();
	ItmDebugClassMemory*	GetDebugMemoryInterface()	{return NULL;};
	ItmDebugBreakPoint*		GetDebugBreakPointInterface() {return NULL;};
	ItmDebugWatch*			GetDebugWatchInterface() {return NULL;};
	
	void	SetDebugInfoInterface(ItmDebugInfo* pIDebugInfo);
	void	SetDebugMemoryInterface(ItmDebugClassMemory* pIDebugMemory) {};
	void	SetDebugBreakPointInterface(ItmDebugBreakPoint* pIDebugBreakPoint){};
	void	SetDebugWatchInterface(ItmDebugWatch* pIDebugWatch){};
protected:
	ItmDebugInfo*		 m_pIDebugInfo;
	ItmDebugClassMemory* m_pIDebugMemory;
	ItmDebugBreakPoint*	 m_pIDebugBreak;
	ItmDebugWatch*		 m_pIDebugWatch;
private:
	static CtmDebuggerCpp*	 m_pInstance;
	static int				 m_nCount;
};



extern		ItmDebugInfo*	g_tmDebugInfo;

#endif
