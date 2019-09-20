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
#ifndef	D_SRAMRW
#define	D_SRAMRW

#include	"common.h" 
#include	"files.h"
#include	"storage.h"

/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/


const	int	SRAM_CHECK_LEN			= 4;
const	int	SRAM_ERROR_HEAD_NUM		= 1;	//	there're 1 records that's defined to head info in error info.
const	int	SRAM_MONI_HEAD_NUM		= 1;	//	there're 1 records that's defined to head info in moni info.
const	int	SRAM_MANU_HEAD_NUM		= 1;	//	there're 1 records that's defined to head info in moni info.<<<zholy070411

#define		SRAM_START_NUM			64
/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/

typedef struct tmTagFileRuler
{
    tmFileIDType    FileID;
    int             nExtend;
}   tmFileRuler;

typedef struct  tmTagFileRulerInfo
{
    tmFileIDType    FileID;
    int             nSize;
    int             nTotalSize;
    int             nPos;
    int             nCheckLen;
    int             nCheckValue;    
    int             nHeadNum;
}   tmFileRulerInfo;


/*===========================================================================+
|           Class declaration - CtmRecordFileEx                            	|
+===========================================================================*/
class CtmRecordFileEx
{
	public:
		CtmRecordFileEx()	{};
		CtmRecordFileEx(CtmStorageFile* pStorage) 
		{
			m_pStorage = pStorage;
		};
		virtual	~CtmRecordFileEx()	{};
	public:
		virtual	int		GetSize(int nType) 																= 0;
		virtual	int		ReadRecord  (int nType, int nRecordNo, void* pRecord) 							= 0;
    	virtual	int 	ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength)				= 0;
    	virtual	int		ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset) = 0;
    	virtual	int		WriteRecord (int nType, int nRecordNo, void* pRecord)							= 0;
    	virtual	int		WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength)				= 0;
    	virtual	int		WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)	= 0;
    	virtual	int		DeleteRecord(int nType, int nRecordNo)											= 0;
    	virtual	int 	EraseRecord (int nType, int nRecordNo)											= 0;
    	virtual	int		DeleteRecord(int nType)															= 0;
    	virtual	int		EraseRecord (int nType)															= 0;
    	virtual	BOOL 	CheckData(int nType)															= 0;
    	
    	virtual	void	SetStorage(CtmStorageFile* pStroage)	{m_pStorage = pStroage;};
    	virtual	CtmStorageFile*	GetStroage()	{return m_pStorage;};

    
    protected:
        
    	CtmStorageFile*	m_pStorage;
};

/*===========================================================================+
|           Class declaration - CtmHmiRecordFile                          	|
+===========================================================================*/
class CtmHmiRecordFile: public CtmRecordFileEx
{
	public:
		CtmHmiRecordFile();		
		CtmHmiRecordFile(CtmStorageFile* pStorage);
		~CtmHmiRecordFile();
	public:
		
		int		GetSize(int nType) 																;
		int		ReadRecord  (int nType, int nRecordNo, void* pRecord) 							;
		int 	ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength)				;
		int		ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset) ;
		int		WriteRecord (int nType, int nRecordNo, void* pRecord)							;
		int		WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength)				;
		int		WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)	;
		int		DeleteRecord(int nType, int nRecordNo)											;
		int 	EraseRecord (int nType, int nRecordNo)											;
		int		DeleteRecord(int nType)															;
		int		EraseRecord (int nType)															;
		BOOL 	CheckData(int nType)	{return TRUE;}											;
		
		
	protected:
		virtual	void	InitData();
    	virtual	int		WriteToStorage(int nPos, int nLength, void* pRecord, int nPerSize);
		virtual	int 	ReadFromStorage(int nPos, int nLength, void* pRecord, int nPerSize);
		virtual int     FindFileType(tmFileRulerInfo* pSource, tmFileIDType FileID, int nStartNo, int nEndNo);
		
		virtual	int		GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID);
		virtual	int		GetErrorSize(tmFileRulerInfo* pErrorFileInfo);
		virtual	int		GetDBSize(tmFileRulerInfo* pDBFileInfo);
		virtual	int		GetDB2Size(tmFileRulerInfo* pDBFileInfo);
		virtual	int		GetMoniSize(tmFileRulerInfo* pMoniFileInfo);
		virtual	int		GetCurveDataSize(tmFileRulerInfo* pCurveFileInfo);
		virtual	int		GetModifySetSize(tmFileRulerInfo* pRecordFileInfo);
		virtual	int		GetMoldSetSize(tmFileRulerInfo* pMoldFileInfo);
		virtual	int		GetManufacturePlanSize(tmFileRulerInfo* pManuFileInfo);//zholy070411
		virtual	int		GetMachineSize(tmFileRulerInfo*	pMachineFileInfo);	  	//James add 2008/2/18 05:24¤U¤È
		virtual	int		GetMachineBSize(tmFileRulerInfo*	pMachineFileInfo);
		virtual int		GetDATableSize(tmFileRulerInfo*	pDATableFileInfo);
		virtual int		GetSystemxSize(tmFileRulerInfo*	pDATableFileInfo);
		virtual int		GetUserSize(tmFileRulerInfo*	pDATableFileInfo);
		virtual int		GetDataStateSize(tmFileRulerInfo*	pDataStateFileInfo);
		virtual int		GetDataEnergy(tmFileRulerInfo*	pDataEnergyFileInfo);
		virtual int		GetDataEnergy2(tmFileRulerInfo*	pDataEnergy2FileInfo);
		virtual int		GetDataPPH(tmFileRulerInfo*	pDataEnergy2FileInfo);
		virtual int		GetDataPowerOnOff(tmFileRulerInfo*	pDataPowerOnOffFileInfo);
		virtual int		GetOnOffStateSize(tmFileRulerInfo*	pOnOffStateFileInfo);
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/		
    protected:
        tmFileRulerInfo m_aStorageFileInfo[ID_STORAGE_FILE_END - ID_STORAGE_FILE_START];
        int		m_nIndexInFiles;
};

/*===========================================================================+
|           Class declaration - CtmOilSramRecordFile                            |
+===========================================================================*/
class 	CtmOilSramRecordFile: public CtmHmiRecordFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:		
		CtmOilSramRecordFile(CtmStorageFile* pStorage);
		CtmOilSramRecordFile() ;
		~CtmOilSramRecordFile();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		//==================
		//	inherited from CtmHmiRecordFile, and override it
		//===================
		int		GetSize(int nType) 																;
	    BOOL 	CheckData(int nType)															;
	protected:		   		
		//==================
		//	inherited from CtmHmiRecordFile, and override it
		//===================
		void	InitData();
		int		GetPos(int nType);
		int		GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID);
		virtual	int	GetTempCurveSize(tmFileRulerInfo* pTempCurveFileInfo);
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
        static  tmFileRulerInfo m_aFileInfo[];
};

/*===========================================================================+
|           Class declaration - CtmMMCardRecordFile                          |
+===========================================================================*/
class	CtmMMCardRecordFile: public CtmHmiRecordFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmMMCardRecordFile(CtmStorageFile* pStorage);
		CtmMMCardRecordFile() ;
		~CtmMMCardRecordFile();
		
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		int		GetSize(int nType) 																;
    	BOOL 	CheckData	(int nType)															;    
    protected:
        virtual int GetMachineSize(tmFileRulerInfo* pMachineFileInfo);
    	void	InitData();
    	int		GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID);
    
    private:
    	int		GetMachineSize();
    	int		GetMMISize();
    	int		CalculateMachineSize();
    /*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
    private:
		static  tmFileRulerInfo m_aFileInfo[];
};

/*===========================================================================+
|           Class declaration - CtmUsbRecordFile	                          |
+===========================================================================*/
class	CtmUsbRecordFile: public CtmMMCardRecordFile 
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmUsbRecordFile(CtmStorageFile* pStorage);
		CtmUsbRecordFile() ;
		~CtmUsbRecordFile();
};

/*===========================================================================+
|           Class declaration - CtmFlashRecordFile	                          |
+===========================================================================*/
class	CtmFlashRecordFile: public CtmMMCardRecordFile
{
    /*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmFlashRecordFile(CtmStorageFile* pStorage);
		CtmFlashRecordFile() ;
		~CtmFlashRecordFile();
		
	public:
		int		GetSize(int nType);
	protected:
		int		GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID);
		
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		static  tmFileRulerInfo m_aFileInfo[];
};

/*===========================================================================+
|           Class declaration - CtmFlashXRecordFile	                          |
+===========================================================================*/
class	CtmFlashEXRecordFile: public CtmHmiRecordFile
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		CtmFlashEXRecordFile(CtmStorageFile* pStorage);
		CtmFlashEXRecordFile() ;
		~CtmFlashEXRecordFile();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		//==================
		//	inherited from CtmHmiRecordFile, and override it
		//===================
		int		GetSize(int nType) 																;
	    BOOL 	CheckData(int nType)															;
	protected:		   		
		//==================
		//	inherited from CtmHmiRecordFile, and override it
		//===================
		void	InitData();
		int		GetPos(int nType);
		int		GetMoldSetSize(tmFileRulerInfo* pMoldFileInfo);
		//virtual	int	GetTempCurveSize(tmFileRulerInfo* pTempCurveFileInfo);
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
        static  tmFileRulerInfo m_aFileInfo[];
};

/*===========================================================================+
|           Class declaration - CtmRecordFileEx                            |
+===========================================================================*/
class CtmSarmRecordFileControl
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		static	CtmSarmRecordFileControl*	GetInstance();
		static	void		ReleaseInstance();
	protected:
		CtmSarmRecordFileControl();
		virtual	~CtmSarmRecordFileControl();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		int		GetSize		(int nType, tmStorageType StorageType = STORAGE_TYPE_SRAM) 					;
		int		ReadRecord  (int nType, int nRecordNo, void* pRecord, 
							 tmStorageType StorageType = STORAGE_TYPE_SRAM) 								;
		int 	ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength,
							 tmStorageType StorageType = STORAGE_TYPE_SRAM)								;
	    int		ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset,
	    					 tmStorageType StorageType = STORAGE_TYPE_SRAM) 								;
	    int		WriteRecord (int nType, int nRecordNo, void* pRecord,
	    					 tmStorageType StorageType = STORAGE_TYPE_SRAM)								;
	    int		WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength,
	    					 tmStorageType StorageType = STORAGE_TYPE_SRAM)								;
	    int		WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset,
	    					 tmStorageType StorageType = STORAGE_TYPE_SRAM)								;
	    int		DeleteRecord(int nType, int nRecordNo, tmStorageType StorageType = STORAGE_TYPE_SRAM)	;
	    int 	EraseRecord (int nType, int nRecordNo, tmStorageType StorageType = STORAGE_TYPE_SRAM)	;
	    int		DeleteRecord(int nType, tmStorageType StorageType = STORAGE_TYPE_SRAM)					;
	    int		EraseRecord (int nType, tmStorageType StorageType = STORAGE_TYPE_SRAM)					;
	    BOOL 	CheckData(int nType, tmStorageType StorageType = STORAGE_TYPE_SRAM)	;
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		static	CtmSarmRecordFileControl*	m_pInstance;
		static	CtmRecordFileEx*			m_pSramFile;	
		
		//pthread_mutex_t						m_WriteMutex;	
};

extern	int		g_nDB2Size;
#endif


