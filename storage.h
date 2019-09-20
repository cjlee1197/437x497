/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmSram			                                             |
|  Task     : CtmSram header file					                         |
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
#ifndef	D_STORAGE
#define	D_STORAGE
#include	"common.h" 
#include	"files.h"
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
#ifdef	D_3354
#define		tmSRAM_NAME				"/dev/sram0"//"/dev/mtd0"
#else
#define		tmSRAM_NAME				"/dev/sram0"
#endif
#define		MOUNT_SUCCESS	0
#define		MOUNT_FAIL		-1
#define		UMOUNT_SUCCESS	0x00001000
#define		UMOUNT_FAIL		0x00002000

const	int	FILE_NAME_MAX_LEN	= 256;
const	int	HMI_SAVE_FILE_MAX	= 256;

extern	int		g_SramWriteFd;
extern	int		g_SramWriteFlag;
/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef enum STROAGETYPE
{
	STORAGE_TYPE_START	= 0,
	STORAGE_TYPE_SRAM	,		//	sram
	STORAGE_TYPE_MMCARD	,		//	mmcard
	STORAGE_TYPE_USB	,		//	usb
	STORAGE_TYPE_FLASH	,		//	flash
	STORAGE_TYPE_DISK	,		//	Disk
	STORAGE_TYPE_INET	,		//	iNet
	STORAGE_TYPE_FLASHEX		//	falshEx is out of control for the Linux os
}	tmStorageType;

typedef enum OIL_FILEID
{
	//	the files to saving, not only in the sram
	ID_STORAGE_NOFILE		= 0					,
	ID_STORAGE_FILE_START	= 1					,
	ID_START_SRAMFILE 		= ID_STORAGE_FILE_START	, 	
	ID_DB_SRAMFILE			= ID_START_SRAMFILE	,	//	db file				:sram, mmcard.
	ID_DB2_SRAMFILE								,	//	db2 filew			:sram-db2
	ID_CURVEDATA_SRAMFILE						,	//	curvedata file		:sram.
	ID_ERROR_SRAMFILE							,	//	error file			:sram, mmcard.
	ID_MONI_SRAMFILE							,	//	moni file			:sram, mmcard.
	ID_RECORDSET_SRAMFILE						,	//	modify record file	:sram.
	ID_MOLDSET_SRAMFILE							,	//	moldset file		:sram, mmcard.
	ID_MANUFACTURE_SRAMFILE						,	//	manufacture file	:sram.        
	ID_ONOFFSTATE_SRAMFILE						,	//	onoff state 		:sram.     	//James add for HT  
	ID_DATASTATE_SRAMFILE						,	//	datastate file		:sram, buffer
	ID_DATAENERGY_SRAMFILE						,	//	data energy file	:sram, flash, mmcard
	ID_DATAPOWERONOFF_SRAMFILE					,	//	data power onoff file :sram
	ID_DATAENERGY2_SRAMFILE						,	//	data energy2 file	:sram, flash, mmcard
	ID_DATAPPH_SRAMFILE							,	//	data PPH file		:sram, flash, mmcard
	ID_TEMPCURVE_SRAMFILE						,	//	Temp Curve			:sram
	
	ID_MACHINE_FILE								,	//	machine 			:flashex, mmcard.
	ID_MACHINEB_FILE							,	//	machineb 			:flashex, mmcard.
	ID_MACHINE_MACH_FILE						,	//	machine 			:flashex, mmcard.
	ID_MACHINEB_MACH_FILE						,	//	machineb 			:flashex, mmcard.
	ID_DATABLE_FILE								,	//	da table			:flashex, mmcard.
	ID_DATABLE_MACH_FILE						,	//	da table			:flashex, mmcard.
	ID_INSTALL_FILE								,	//  stop machine		:flashex, mmcard.
	ID_SYSTEMX_FILE								,	//	mmiconfig			:flashex, mmcard.
	ID_USERM_FILE								,	//	userm				:flashex, mmcard.
	ID_END_SRAMFILE								,
	ID_MAC_FILE				=	ID_END_SRAMFILE	,	//	mac address			:flash
	ID_MMI_PROGRAM_FILE							,	//	mmi 				:mmcard
	ID_ZEROSET_FILE								,	//	Zero				:Flash
    
    //ID_CURVE_FILE								,	//	CURVE			    :MMCArd     //Sunny<20070911><<<<<
    //ID_VERSION_FILE								,	//	VERSION			    :MMCArd
    //ID_DEFAULTVALUE_FILE						,	//	DEFAULTVALUE	    :MMCArd
    //ID_MINVALUE_FILE							,	//	MINVALUE		    :MMCArd
    //ID_MAXVALUE_FILE							,	//	MAXVALUE		    :MMCArd     //Sunny<20070911>>>>>>

	ID_TEMP_CURVE1_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE2_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE3_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE4_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE5_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE6_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE7_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE8_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE9_FILE                          ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE10_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE11_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE12_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE13_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE14_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE15_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE16_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE17_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE18_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE19_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE20_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE21_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE22_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE23_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE24_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE25_FILE                         ,
	ID_TEMP_CURVE26_FILE                         ,   //  temp message        :MMCARD
	ID_TEMP_CURVE27_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE28_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE29_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE30_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE31_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE32_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE33_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE34_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE35_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE36_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE37_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE38_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE39_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE40_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE41_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE42_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE43_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE44_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE45_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE46_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE47_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE48_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE49_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE50_FILE                         ,   //  temp curve1         :MMCArd
	ID_TEMP_CURVE51_FILE                         ,   //  temp message        :MMCARD
	ID_TEMP_CURVE52_FILE                         ,   //  temp set            :MMCARD
    ID_TEMP_CURVE53_FILE                         ,
    ID_TEMP_CURVE54_FILE                         ,
    ID_TEMP_CURVE55_FILE                         ,
    ID_TEMP_CURVE56_FILE                         ,
    ID_TEMP_CURVE57_FILE                         ,
    ID_TEMP_CURVE58_FILE                         ,
    ID_TEMP_CURVE59_FILE                         ,
    ID_TEMP_CURVE60_FILE                         ,
    ID_TEMP_CURVE61_FILE                         ,
    ID_TEMP_CURVE62_FILE                         ,
    ID_TEMP_CURVE63_FILE                         ,
    ID_TEMP_CURVE64_FILE                         ,
    ID_TEMP_MESSAGE_FILE                         ,
    ID_WARM_CURVE1_FILE                          ,
    ID_WARM_CURVE2_FILE                          ,
    ID_WARM_CURVE3_FILE                          ,
    ID_WARM_CURVE4_FILE                          ,
    ID_WARM_CURVE5_FILE                          ,
    ID_WARM_CURVE6_FILE                          ,
    ID_WARM_CURVE7_FILE                          ,
    ID_WARM_CURVE8_FILE                          ,
    ID_WARM_CURVE9_FILE                          ,
    ID_WARM_CURVE10_FILE                         ,
    ID_WARM_CURVE11_FILE                         ,
    ID_WARM_CURVE12_FILE                         ,
    ID_WARM_CURVE13_FILE                         ,
    ID_WARM_CURVE14_FILE                         ,
    ID_WARM_CURVE15_FILE                         ,
    ID_WARM_CURVE16_FILE                         ,
    ID_WARM_CURVE17_FILE                         ,
    ID_WARM_CURVE18_FILE                         ,
    ID_WARM_CURVE19_FILE                         ,
    ID_WARM_CURVE20_FILE                         ,
    ID_WARM_CURVE21_FILE                         ,
    ID_WARM_CURVE22_FILE                         ,
    ID_WARM_CURVE23_FILE                         ,
    ID_WARM_CURVE24_FILE                         ,
    ID_WARM_CURVE25_FILE                         ,
    ID_WARM_CURVE26_FILE                         ,
    ID_WARM_CURVE27_FILE                         ,
    ID_WARM_CURVE28_FILE                         ,
    ID_WARM_CURVE29_FILE                         ,
    ID_WARM_CURVE30_FILE                         ,
    ID_WARM_CURVE31_FILE                         ,
    ID_WARM_CURVE32_FILE                         ,
    ID_WARM_CURVE33_FILE                         ,
    ID_WARM_CURVE34_FILE                         ,
    ID_WARM_CURVE35_FILE                         ,
    ID_WARM_CURVE36_FILE                         ,
    ID_WARM_CURVE37_FILE                         ,
    ID_WARM_CURVE38_FILE                         ,
    ID_WARM_CURVE39_FILE                         ,
    ID_WARM_CURVE40_FILE                         ,
    ID_WARM_CURVE41_FILE                         ,
    ID_WARM_CURVE42_FILE                         ,
    ID_WARM_CURVE43_FILE                         ,
    ID_WARM_CURVE44_FILE                         ,
    ID_WARM_CURVE45_FILE                         ,
    ID_WARM_CURVE46_FILE                         ,
    ID_WARM_CURVE47_FILE                         ,
    ID_WARM_CURVE48_FILE                         ,
    ID_WARM_CURVE49_FILE                         ,
    ID_WARM_CURVE50_FILE                         ,
    ID_WARM_CURVE51_FILE                         ,
    ID_WARM_CURVE52_FILE                         ,
    ID_WARM_CURVE53_FILE                         ,
    ID_WARM_CURVE54_FILE                         ,
    ID_WARM_CURVE55_FILE                         ,
    ID_WARM_CURVE56_FILE                         ,
    ID_WARM_CURVE57_FILE                         ,
    ID_WARM_CURVE58_FILE                         ,
    ID_WARM_CURVE59_FILE                         ,
    ID_WARM_CURVE60_FILE                         ,
    ID_WARM_CURVE61_FILE                         ,
    ID_WARM_CURVE62_FILE                         ,
    ID_WARM_CURVE63_FILE                         ,
    ID_WARM_CURVE64_FILE                         ,
    ID_WARM_MESSAGE_FILE                         ,
    ID_TEMP_SET_FILE                             ,
	ID_STORAGE_FILE_END
}	tmFileIDType;

typedef	struct	tagHMISTORAGEFILE
{
	tmFileIDType	FileID;
	void*			pFileInfo;
}	tmHmiStorageFile;

typedef	struct	tagHMIMANFILE
{
	tmFileIDType	FileID;
	char*			pszFilePath;
	char*			pszFileName;
}	tmHmiMMCardFile, tmHMIFlashFile, tmHMIDiskFile, tmHMIUsbFile;

/*===========================================================================+
|           Class declaration - CtmStorageFile		                         |
+===========================================================================*/
class	CtmStorageFile
{
	public:
		CtmStorageFile()	
		{
			memset(m_HMIFiles, 0 , sizeof(m_HMIFiles));
		};
		CtmStorageFile(void* pInitData) 
		{
			memset(m_HMIFiles, 0 , sizeof(m_HMIFiles));
		};
	public:
		virtual	int	Check()														= 0;
		virtual	int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer)	= 0;
		virtual	int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer)		= 0;
		virtual	int	Delete(int nFlag = -1)										= 0;
		virtual	int	ReInit(void* pInitData)										= 0;
		virtual	int	ReInit(int	nFlag)											= 0;
	protected:
		tmHmiStorageFile	m_HMIFiles[HMI_SAVE_FILE_MAX];
	
};

/*===========================================================================+
|           Class declaration - CtmSram		                                 |
+===========================================================================*/
class CtmSram: public CtmStorageFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/		
	public:
		static	CtmSram*	GetInstance();
		static	CtmSram*	GetInstance(void* pInitData);
		static	void		ReleaseInstance();
	protected:
		CtmSram();
		CtmSram(void* pInitData);
		virtual	~CtmSram();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		int	Check();
		int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int	Delete(int nFlag = -1);
		int	ReInit(void* pInitData)	{return 0;};
		int	ReInit(int	nFlag) {return 0;};
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		static	CtmSram*	m_pInstance;
		static	int			m_nReadCount;
		static	int			m_nWriteCount;
		int					m_nReadFd;	
		int					m_nWriteFd;
		int					m_nRead2Fd;	
		int					m_nWrite2Fd;
		pthread_mutex_t		m_ReadMutex;					// 消息進程鎖
		pthread_mutex_t		m_WriteMutex;					// 消息進程鎖
};

class	CtmFlashEx:public CtmStorageFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/		
	public:
		static	CtmFlashEx*	GetInstance();
		static	CtmFlashEx*	GetInstance(void* pInitData);
		static	void		ReleaseInstance();
	protected:
		CtmFlashEx();
		CtmFlashEx(void* pInitData);
		virtual	~CtmFlashEx();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		static int	WritePthread(BYTE* pcBuffer);
		int	Check();
		int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int	Delete(int nFlag = -1);
		int	ReInit(void* pInitData)	{return 0;};
		int	ReInit(int	nFlag) {return 0;};
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		static	CtmFlashEx*	m_pInstance;
		pthread_t	m_FlashEXRecordThreadID;	
		static	int			m_nReadCount;
		static	int			m_nWriteCount;
        static  int         m_nFlashSize;
		static int			m_nReadFd;	
		static int			m_nWriteFd;
		static unsigned int 		m_nPosition;
		static int 					m_nSize;
		static BYTE* 				m_pcBuffer;
		static pthread_mutex_t		m_ReadMutex;					// 消息進程鎖
		static pthread_mutex_t		m_WriteMutex;					// 消息進程鎖
};



/*===========================================================================+
|           Class declaration - CtmDiskFile		                             |
+===========================================================================*/
class CtmDiskFile: public CtmStorageFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/	
	public:
		CtmDiskFile();
		CtmDiskFile(void* pInitData);
		virtual ~CtmDiskFile();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		int	Check();
		int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int	Delete(int nFlag = -1);
		int	ReInit(void* pInitData);
		int	ReInit(int	nFlag)	;
		int	ReName(int nFileID, char*	pszFileName);			//James add 2011/8/2 下午 09:00:22
		int	CheckFileExist(int nFileID);
	protected:
		virtual	int	    Init(void* pInitData);
		virtual	void    ExistDir(char* pszPath);
		virtual	int		Open(char* pszName);
		virtual	void	Close();
		virtual	void 	GetManageFile(tagHMIMANFILE* pManFile, int nFileNum);
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	protected:
		int		m_nFd;
		int		m_nMountNum;
		char	m_szFileName[FILE_NAME_MAX_LEN];
		CtmFile	m_File;
		
	private:
		static	tmHMIDiskFile	m_DiskFiles[];
		
};

typedef	CtmDiskFile CtmFlashFile;

/*===========================================================================+
|           Class declaration - CtmMountFile	                             |
+===========================================================================*/
class CtmMountFile: public CtmDiskFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmMountFile();
		CtmMountFile(void* pInitData);
		~CtmMountFile();
		/*-------------------------------------------------------------------------------+
		|        Operations                                                              |
		+-------------------------------------------------------------------------------*/
	public:
		int	Check()	{return m_nFd;};
		int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int	Delete(int nFlag = -1);
		int	ReName(int nFileID, char*	pszFileName){return -1;};			//James add 2011/8/2 下午 09:00:22
		int	CheckFileExist(int nFileID){return -1;}
		int	ReInit(void* pInitData)	{return CtmDiskFile::ReInit(pInitData);};
		int	ReInit(int	nFlag)	{return CtmDiskFile::ReInit(nFlag);};
		int	WriteBlock(unsigned int nPosition, int nSize, BYTE* pcBuffer){return CtmMountFile::Write(nPosition, nSize, pcBuffer);};
		virtual BOOL UMount(BOOL bMsg = TRUE, BOOL bPromp = TRUE) {return FALSE;};
	    virtual BOOL Mount(BOOL bMsg = TRUE, BOOL bPromp = TRUE)  {return FALSE;};
	    
	protected:
	    
	    virtual	void GetFileInfo(char* pszPath);
	    virtual	int  ReInitMountFile(void* pInitData, tagHMIMANFILE* pMountFile);
	    
	    int		Open(char* pszName);
	    void 	GetManageFile(tagHMIMANFILE* pManFile, int nFileNum);
	    
	    /*------------------------------------------------------------------------------+
		|           Attributes                                                          |
		+------------------------------------------------------------------------------*/
	protected:
		int		m_nMountStatus;	
		long	m_lAvailSize;		
	    BOOL    m_bReplace;
		char	m_szMountPath[FILE_NAME_MAX_LEN];
		static	pthread_mutex_t	m_mutex;
};

/*===========================================================================+
|           Class declaration - CtmMMCard		                             |
+===========================================================================*/
class CtmMMCardFile: public CtmMountFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/	
	public:
		static	CtmMMCardFile*	GetInstance();
		static	CtmMMCardFile*	GetInstance(void* pInitData);
		static	void		ReleaseInstance();
	protected:
		CtmMMCardFile();
		CtmMMCardFile(void* pInitData);
		~CtmMMCardFile();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		int	ReInit(void* pInitData);
		int	ReInit(int	nFlag)	{return CtmMountFile::ReInit(nFlag);};
		int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int	Delete(int nFlag = -1);
		int	ReName(int nFileID, char*	pszFileName);			//James add 2011/8/2 下午 09:00:22
		int	CheckFileExist(int nFileID);
		BOOL	Mount(BOOL bMsg = TRUE, BOOL bPromp = TRUE);
		BOOL	UMount(BOOL bMsg = TRUE, BOOL bPromp = TRUE);
	protected:
		
	/*---------------------------------------------------------------------                                                          ---------+
	|           Attributes														|
	+------------------------------------------------------------------------------*/
	private:
		static	tmHmiMMCardFile	m_MMCardFiles[];
		static	CtmMMCardFile*	m_pInstance;
};

/*===========================================================================+
|           Class declaration - CtmUsbFile		                             |
+===========================================================================*/
class   CtmUsbFile: public CtmMountFile
{
    /*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
	    static	void		ReleaseInstance();
	    static CtmUsbFile* 	GetInstance(void* pInitData);
	    static CtmUsbFile* 	GetInstance();
	protected:
	    CtmUsbFile();
	    CtmUsbFile(void* pInitData);
	    ~CtmUsbFile();
	    /*-------------------------------------------------------------------------------+
		|        Operations                                                              |
		+-------------------------------------------------------------------------------*/
	public:
		int	ReInit(void* pInitData);
		int	ReInit(int	nFlag)	{return CtmMountFile::ReInit(nFlag);};
		int	Write(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int Read(unsigned int nPosition, int nSize, BYTE* pcBuffer);
		int	Delete(int nFlag = -1);
		int	ReName(int nFileID, char*	pszFileName);			//James add 2011/8/2 下午 09:00:22
		int	CheckFileExist(int nFileID);
		BOOL UMount(BOOL bMsg = TRUE, BOOL bPromp = TRUE);		//	overwrite
	    BOOL Mount(BOOL bMsg = TRUE, BOOL bPromp = TRUE);		//	overwrite
	    
	    pthread_attr_t  m_ThreadAttr;
	    pthread_t 		m_OpenThreadID;
	    static	void	ThreadUSBhotplug(void *pData);
	    static	CtmUsbFile*	m_pSelf;
	protected:
	    
	    /*------------------------------------------------------------------------------+
		|           Attributes                                                          |
		+------------------------------------------------------------------------------*/
	private:
	  	static	tmHMIUsbFile	m_UsbFiles[];
	    static  char*   		m_pszUsbDev[];    
	    static  CtmUsbFile* 	m_pInstance;
};
#endif
