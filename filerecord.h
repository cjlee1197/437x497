/* Copyright (C), 2015-2020, Techmation. Co., Ltd.*/ 

/*==========================================================================+
|  Class    : CFileRecord					   				  				|
|  Task     : CFileRecord header file                   			      	|
|---------------------------------------------------------------------------|
|  Compile  :G++(GCC) 4.7                                                   |
|  Link     :G++(GCC) 4.7                                                   |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                   	|
|  Version  : V1.00                                                         |
|  Creation : 2016.03.08                                        	             	|
|  Revision :                                                               |
+==========================================================================*/


#ifndef D_CFILERECORD_H
#define D_CFILERECORD_H

#include	<fstream>
#include	<string>
#include 	<iostream>
using namespace std;

/*==========================================================================+
|           struct                                                        |
+==========================================================================*/

#define		FILE_MAXCOUNT		100
#define		FILE_MAXSTRINGLEN 	1024

typedef enum	FILE_RECORD
{
	FILE_RECORD_MONI = 0,
	FILE_RECORD_ALARM,
	FILE_RECORD_MODIFYLOG,
	FILE_RECORD_LINESTATE,
	FILE_RECORD_OPSTATE,
	FILE_RECORD_ENEGERY,
	FILE_RECORD_POWERONOFF,
	FILE_RECORD_ENEGERY2,
	FILE_RECORD_PPH,
	FILE_RECORD_OPER_LOG,
};

typedef	struct	tagFileRecordHeadInfo
{
	int		nCount;
	int		nIndex;
}tmFileRecordHeadInfo;

typedef struct tagFileRecordInfo
{
	int		nFileType;
	int		nHeadSize;
	int		nFileFormat;
	int		nVers;
	int		nSize;
	int		nCount;
	char	pszFileName[100];
}tmFileRecordInfo;

/*==========================================================================+
|           Class declaration - CFileRecord                                	|
+==========================================================================*/
class CFileRecord
{	
	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
	public:  	
		CFileRecord();
		CFileRecord(const char* pszFileName);    
		virtual ~CFileRecord();
		
  	/*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
	public:
		bool 	OpenFile(const char* pszFileName);
		void 	CloseFile(); 
	
		int		WriteRecord(int	nPos, void* pRecord, int nSize);
		int		WriteRecordM(int nPos, void* pRecord, int nSize);
		int		ReadRecord(int	nPos, void* pRecord, int nSize);
		int		ReadRecordM(int	nPos, void* pRecord, int nSize);
		
	protected:    
	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
	private:
		int 		m_Fd;
		bool 		m_IsClosed;
};

class	CManageFile
{
	public:  	
		CManageFile();   
		virtual ~CManageFile();
		
		static CManageFile* 	GetInstance();
		static void			ReleaseInstance();
		void	Init();
		
		int		WriteRecord(int	nType, int nRecordNo, void* pRecord, int nLength);
		int		ReadRecord(int nType,  int nRecordNo, void* pRecord, int nLength);
		int		DeleteRecord(int nType);
		int		CheckData(int nType);
		CFileRecord*		GetFileInstance(int nType);
	public:
		CFileRecord*		m_pFileInstance[FILE_MAXCOUNT];
		static  			tmFileRecordInfo m_aFileInfo[FILE_MAXCOUNT];
	private:
		static	CManageFile*		m_pInstance;
};

#endif 
