/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmMachineBuild		                                         |
|  Task     : CtmMachineBuild header file			                         |
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
#ifndef	D_MACHINEFACTOR
#define	D_MACHINEFACTOR

#include	"tmconfig.h"
#include	"qualitycontrol.h"
#include	"curvedata.h"
#include	"recordset.h"
#include	"storage.h"
#include	"sram.h"
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/


/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef struct	tagMODULEINFO
{
	int		iModuleID;
	char	szModuleName[50];
}	tmModuleInfo;

typedef struct tagLOGICMODULE
{
	tmModuleInfo	ModuleInfo;
	void*			pModuleIntance;
}	tmLogicModule;

/*===========================================================================+
|           Class declaration - CtmMachineBuild                              |
+===========================================================================*/
class	CtmMachineBuild
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	public:
		static	CtmMachineBuild*	GetInstance();
		static	void				ReleaseInstance();
	protected:
		CtmMachineBuild();
		virtual	~CtmMachineBuild();	
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
		CtmCurveData*	BuildCCurveData();
		CtmQuality*		BuildCQuality();
		CRecordSet*		BuildCRecordSet();
		CtmRecordFileEx* BuildCStorageRecordFile(tmStorageType Type = STORAGE_TYPE_SRAM);
		void			FreeStorageRecordFile(tmStorageType StorageType);
		CtmStorageFile* BuildStorage(tmStorageType StorageType);
								
		BOOL			GetStorageFileName(tmStorageType Type, 
											tmFileIDType FileID,
											char* pszOutFileName);
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
		static			CtmMachineBuild*	m_pInstance;
		
		static			CtmStorageFile*		m_pSram;	
		static			CtmStorageFile*		m_pMMCard;
		static			CtmStorageFile*		m_pUsb;
		static			CtmStorageFile*		m_pFlash;
		static			CtmStorageFile*		m_pFlashEX;
		
		static			CtmRecordFileEx*	m_pSramRecordFile;
		static			CtmRecordFileEx*	m_pMMCardRecordFile;
		static			CtmRecordFileEx*	m_pFlashRecordFile;
		static			CtmRecordFileEx*	m_pUsbRecordFile;
		static			CtmRecordFileEx*	m_pFlashEXRecordFile;
		
		char*			m_pMMCardInitData;
		char*			m_pFlashInitData;
		
		int				m_nMachineType;
};

#endif
