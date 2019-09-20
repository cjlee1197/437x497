/* Copyright (C), 1995-2000, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Files                                                          |
|  Task     : File Stream derived class header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/19/1995                                                     |
|  Revision : 05/07/1995                                                     |
|  Revision : V2.00                                                          |
|             05/07/1995 by C.C. Chen                                        |
|             - Add the CRecordFile class                                    |
+===========================================================================*/
#ifndef     D_FILES
#define     D_FILES

#include	"common.h" 

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     tmMAX_LINE_BUFFER		256+3
#define     tmTMP_BUFFER          	1024
#define		tmMAX_NAME				0xFF

#define     tmFILE_GOOD				0x0000
#define     tmFILE_EOF				0x0001
#define     tmFILE_FAIL				0x0002
#define     tmFILE_BAD				0x0004

#define     tmSTATE_NONE			0           //St1109'01 for 童宇揚
#define     tmSTATE_READ			1           // 讀取狀態
#define     tmSTATE_WRITE			2           // 寫入狀態
#define		tmMAX_PATH				260 /* max. length of full pathname */
#define		tmMAX_DRIVE				3   /* max. length of drive component */
#define		tmMAX_DIR				256 /* max. length of path component */
#define		tmMAX_FNAME				256 /* max. length of file name component */
#define		tmMAX_EXT				256 /* max. length of extension component */
#define		tmSRAM_SIZE				0x80000			// sram chip size


#define		tmMAX_FILENAME			20
#define		tmRAMFILE_MAX			10		//sram最多支持文件個數
#define		tmSRAM_STARTADDR		0x400	//保留1024個字節

#define		tmSRAM_P_SYSTEMSIGN		(tmSRAM_STARTADDR)
#define		tmSRAM_P_SYSTEM			(tmSRAM_P_SYSTEMSIGN	+ sizeof(long))
#define		tmSRAM_P_MACHINESIGN	(tmSRAM_P_SYSTEM		+ sizeof(tmDB_SYSTEMX))
#define     tmSRAM_P_MACHINE		(tmSRAM_P_MACHINESIGN	+ sizeof(long))
#define		tmSRAM_P_MOLDSETSIGN	(tmSRAM_P_MACHINE		+ sizeof(tmDB_MACHINE))
#define     tmSRAM_P_MOLDSET		(tmSRAM_P_MOLDSETSIGN	+ sizeof(long))
#define		tmSRAM_P_MONITORSIGN	(tmSRAM_P_MOLDSET		+ sizeof(tmDB_MOLDSETX)*(tmMAX_MOLDSET + 1))
#define     tmSRAM_P_MONITOR		(tmSRAM_P_MONITORSIGN	+ sizeof(long))
#define		tmSRAM_P_ERRORSIGN		(tmSRAM_P_MONITOR		+ sizeof(tmDB_MONI2)*(tmMAX_MONITOR + 1))
#define		tmSRAM_P_ERROR			(tmSRAM_P_ERRORSIGN		+ sizeof(long))
#define		tmSRAM_P_TEMPERSIGN		(tmSRAM_P_ERROR			+ sizeof(tmDB_ERROR)*(tmMAX_ERRORLOG + 2))
#define		tmSRAM_P_TEMPER			(tmSRAM_P_TEMPERSIGN	+ sizeof(long))

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef	struct tagtmRAMFILE
{
	char	*pszName;
	long	lOffset;
	long	lFlag;
	long	lSize;
}	tmRAMFILE;

typedef	struct tagtmRAMFILEINFO
{
	WORD	wVersion;
	long	lOffset;		// 文件在ram中的起始位置
	long	lLength;		// file length									//<<yang 2005/12/5 
	char	szName[tmMAX_FILENAME];		// 文件名	
}	tmRAMFILEINFO;

typedef	struct	tagtmLINEPOS
{
	long					lOffset;
	int						nLength;
	int						nLineNo;
	struct tagtmLINEPOS		*pPre;
	struct tagtmLINEPOS		*pNext;
}	LINEPOS;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
/*===========================================================================+
|           Class declaration - CtmFile										 |
+===========================================================================*/
class	CtmFile
{
public:
	CtmFile();
	CtmFile(char *pszName, int flags = O_RDWR);
	~CtmFile();
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
	char	m_cszLastName[256];							// file Descriptor
};

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
class CtmTextFile : public CtmFile
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmTextFile();
    CtmTextFile(char* pszName);
   	~CtmTextFile();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    int         m_nMode;
    int         m_nProt;
    char        m_pszName[tmMAX_NAME];
    int		    m_fd;                   // File descriptor
    
    LINEPOS	*m_pFirst;					// 列表首地址
    LINEPOS	*m_pLast;					// 列表最後節地址
    
    
    long		m_lCurPos;				// 當前讀寫位置

    int 		m_posCurLine;           // Pointer to current read/write line position
    int		    m_posNextLine;          // Pointer to next read/write line position
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	int			m_nCurLine;				// 當前行
    //void    Open();                                           
    //void    Open(char* pszName);
    //void    Close()                     {close(m_fd);};
    void    Clear();
    void    Rewind();

    int     ReadLine  (int nLineNo, char* pszString, int nBufferSize);
    int     WriteLine (int nLineNo, char* pszString);	//<WY2002/11/28 
    int     InsertLine(int nLineNo, char* pszString);	//<WY2002/11/28 
    int     DeleteLine(int nLineNo);					//<WY2002/11/28 
    int     AppendLine(char* pszString);
    
    //void    CopyFile();								//<WY2002/11/28 
    //void    CopyFile(char* pszFile);					//<WY2002/11/28 
    
/*---------------------------------------------------------------------------+
|  Function : IsEOF()                                                        |
|  Task     : Is it end of file                                              |
+----------------------------------------------------------------------------+
|  Call     : bEOF = IsEOF()                                                 |
|                                                                            |
|  Parameter: None                      -                                    |
|                                                                            |
|  Return   : IsEOF                     - End of file flag                   |
+---------------------------------------------------------------------------*/
    int		GetLineInfo();
    int		GetAllLineInfo();
    
   	BOOL    IsEOF();//             {return (m_posCurLine >= lseek(m_fd, 0, SEEK_END));};
  //  BOOL    IsGood()            {return !fail()/*good()*/;};

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
};


/*===========================================================================+
|           Class declaration - RecordFile                                   |
+===========================================================================*/
class CtmRecordFile
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmRecordFile();
    CtmRecordFile(char* pszName, UINT nRecordSize);
    ~CtmRecordFile();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    BOOL        m_bBypassWrite;         //St030101 for INET
    BOOL        m_bRenew;               //St030101 for INET
    int         m_nRecordNo;            //St1109'01 add for 童宇揚 (既當前要寫入文件的是第几個Record)
    WORD        m_wWriteState;          //St1109'01 add for 童宇揚 (文件的操作狀態)
    int         m_nMode;
    int         m_nProt;
    char        m_pszName[tmMAX_NAME];
    int		    m_fd;                   // File descriptor
	bool		m_bInSram;				// file  in sram
	long		m_lOffset;				// file offset in sram
	long		m_lSize;				// file size in sram
    UINT        m_nRecordSize;
    int			m_posCurRecord;         // Pointer to current read/write Record position
    int   		m_posNextRecord;        // Pointer to next read/write Record position

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Open();
    void    Open(char* pszName, UINT nRecordSize);
    void    OpenReadOnly(char* pszName, UINT nRecordSize);         //St030101 for INET
    void    Close()                     {if (m_fd != 0) close(m_fd);};
    void    Clear();
    void    Erase();                           //St041701 for clear data completely
    void    Rewind();

    int     ReadRecord  (int nRecordNo, void* pRecord);
    int     ReadRecord  (int nRecordNo, void* pRecord, int nLength);
    int     ReadRecord  (int nRecordNo, void* pRecord, int nLength, int nOffset);
    int     WriteRecord (int nRecordNo, void* pRecord);
    int     WriteRecord (int nRecordNo, void* pRecord, int nLength);
    int     WriteRecord (int nRecordNo, void* pRecord, int nLength, int nOffset);
    int     InsertRecord(int nRecordNo, void* pRecord);
    int     DeleteRecord(int nRecordNo);
    int     AppendRecord(void* pRecord);

    //void    CopyFile(fstream fsFile);          //St032101 add for INET
    //void    LoadFile(fstream fsFile);          //St040901
    
    BOOL    IsEOF();
    //BOOL    IsGood()            {return !fail()/*good()*/;};
    BOOL    IsRenew()           {return m_bRenew;};
    int     GetRecordNo()       {return m_nRecordNo;};   //St1109'01 add for 童宇揚(讀記錄號)
    WORD    GetWriteState()     {return m_wWriteState;}; //St1109'01 add for 童宇揚(讀操作狀態)
    void    ClearRenew()        {m_bRenew = FALSE;};
    int     Getfhandle()        {return m_fd;};
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
};

/*===========================================================================+
|			Function														 |
+===========================================================================*/



		



extern	tmRAMFILE	g_RamFileList[tmRAMFILE_MAX];
#endif
