/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmMachineBuild									             |
|  Task     : build the correct interfaces			                         |
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
#include	"machinefactor.h"
#include	"tmdebug.h"
//#include	"memwatch.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//=======
//      Initialize static class member
//=======
CtmMachineBuild*	CtmMachineBuild::m_pInstance 	= NULL;
CtmStorageFile*		CtmMachineBuild::m_pSram 		= NULL;
CtmStorageFile*		CtmMachineBuild::m_pMMCard		= NULL;
CtmStorageFile*		CtmMachineBuild::m_pFlash		= NULL;
CtmStorageFile*		CtmMachineBuild::m_pUsb			= NULL;
CtmStorageFile*		CtmMachineBuild::m_pFlashEX		= NULL;
	
CtmRecordFileEx*	CtmMachineBuild::m_pSramRecordFile		= NULL;    
CtmRecordFileEx*	CtmMachineBuild::m_pMMCardRecordFile	= NULL;
CtmRecordFileEx*	CtmMachineBuild::m_pFlashRecordFile		= NULL;
CtmRecordFileEx*	CtmMachineBuild::m_pUsbRecordFile	 	= NULL;
CtmRecordFileEx*	CtmMachineBuild::m_pFlashEXRecordFile	= NULL;

	
	
	
	
	
	
/*==========================================================================+
|           Class implementation - CtmMachineBuild                          |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmMachineBuild::CtmMachineBuild()
{
	m_nMachineType = CtmConfig::GetInstance()->GetMachineType();
}

CtmMachineBuild::~CtmMachineBuild()
{
	
	if (m_pSram != NULL)
	{
		((CtmSram*)m_pSram)->ReleaseInstance();
		m_pSram = NULL;
	}
	
	if (m_pMMCard != NULL)
	{
		((CtmMMCardFile*)m_pMMCard)->ReleaseInstance();
		m_pMMCard = NULL;
	}
	
	if (m_pSramRecordFile	!= NULL) 
	{
		delete m_pSramRecordFile	;
		m_pSramRecordFile = NULL;
	}
	if (m_pMMCardRecordFile != NULL) 
	{
		delete m_pMMCardRecordFile ;
		m_pMMCardRecordFile = NULL;
	}
	
	if (m_pFlashRecordFile	!= NULL) 
	{
		delete m_pFlashRecordFile ;
		m_pFlashRecordFile = NULL;
	}
	
	
	
	
	
}   
    
CtmMachineBuild*	CtmMachineBuild::GetInstance()
{
	if (m_pInstance == NULL) m_pInstance = new CtmMachineBuild();
	return m_pInstance;
}

void	CtmMachineBuild::ReleaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  BuildCCurveData()											|	
|  Task     :  Build the interface that it's control curve data             |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
CtmCurveData*	CtmMachineBuild::BuildCCurveData()
{
	CtmCurveData* pResult = NULL;
	switch(m_nMachineType)
	{
		case MACHINE_TYPE_SIM:			//	全電Sim
		//    pResult = new CtmCurveSimData();
			break;
		case MACHINE_TYPE_OIL:			//	油壓J6
		case MACHINE_TYPE_OIL_DC:
		case MACHINE_TYPE_OIL_J5:
			pResult = new CtmCurveOilData();
			break;
		default:
			pResult = new CtmCurveOilData();
			break;
	}
	return pResult;
}

/*--------------------------------------------------------------------------+
|  Function :  BuildCQuality()												|	
|  Task     :  Build the interface that it's control Quality                |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
CtmQuality*	CtmMachineBuild::BuildCQuality()
{
	CtmQuality*	pResult = NULL;
	switch(m_nMachineType)
	{
		case MACHINE_TYPE_SIM:			//	全電Sim
			pResult = new CtmSimQuality();
			break;
		case MACHINE_TYPE_OIL:			//	油壓J6
		case MACHINE_TYPE_OIL_DC:
		case MACHINE_TYPE_OIL_J5:
			pResult = new CtmOilQuality();
			break;
		default:
			pResult = new CtmOilQuality();
			break;
	}
	return pResult;
}

/*--------------------------------------------------------------------------+
|  Function :  BuildCRecordSet()											|	
|  Task     :  Build the interface that it's control recordset              |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
CRecordSet*		CtmMachineBuild::BuildCRecordSet()
{
	CRecordSet*	pResult = NULL;
	switch(m_nMachineType)
	{
		case MACHINE_TYPE_SIM:			//	全電Sim
			pResult = new COilRecordSet();
			break;
		case MACHINE_TYPE_OIL:			//	油壓J6
		case MACHINE_TYPE_OIL_DC:
		case MACHINE_TYPE_OIL_J5:
			pResult = new CSimRecordSet();
			break;
		default:
			pResult = new CSimRecordSet();
			break;
	}
	return pResult;
}

/*--------------------------------------------------------------------------+
|  Function :  BuildCRecordSet()											|	
|  Task     :  Build the interface that it's control Sram		            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
CtmRecordFileEx* CtmMachineBuild::BuildCStorageRecordFile(tmStorageType Type)
{
	CtmRecordFileEx* pResult = NULL;
	switch(Type)
	{
		case STORAGE_TYPE_SRAM:			//	sram
			if (m_pSramRecordFile == NULL) m_pSramRecordFile = new CtmOilSramRecordFile();			
			pResult = m_pSramRecordFile;
			break;
		case STORAGE_TYPE_MMCARD:			//	mmcard
			if (m_pMMCardRecordFile == NULL) m_pMMCardRecordFile = new CtmMMCardRecordFile();
			pResult = m_pMMCardRecordFile;
			break;
		case STORAGE_TYPE_FLASH:
			if (m_pFlashRecordFile == NULL) m_pFlashRecordFile = new CtmFlashRecordFile();
			pResult = m_pFlashRecordFile;
			break;
		case STORAGE_TYPE_USB:
			if (m_pUsbRecordFile == NULL) m_pUsbRecordFile = new CtmUsbRecordFile();
			pResult = m_pUsbRecordFile;
			break;
		case STORAGE_TYPE_FLASHEX:
			//g_tmDebugInfo->PrintDebugInfo("Create m_pFlashEXRecordFile EX!\n");
			if (m_pFlashEXRecordFile == NULL) m_pFlashEXRecordFile = new CtmFlashEXRecordFile();
			pResult = m_pFlashEXRecordFile;
			break;
	}
	return pResult;
}

void	CtmMachineBuild::FreeStorageRecordFile(tmStorageType StorageType)
{
	switch(StorageType)
	{
		case STORAGE_TYPE_SRAM:			//	sram
			delete m_pSramRecordFile;
			m_pSramRecordFile = NULL;
			break;
		case STORAGE_TYPE_MMCARD:			//	mmcard
			delete m_pMMCardRecordFile;
			m_pMMCardRecordFile = NULL;
			break;
		case STORAGE_TYPE_FLASH:
			delete m_pFlashRecordFile;
			m_pFlashRecordFile = NULL;
			break;
		case STORAGE_TYPE_USB:			//	usb
			delete m_pUsbRecordFile;
			m_pUsbRecordFile = NULL;
			break;
		case STORAGE_TYPE_FLASHEX:
			delete m_pFlashEXRecordFile;
			m_pFlashEXRecordFile = NULL;
			break;
	}
}


CtmStorageFile* CtmMachineBuild::BuildStorage(tmStorageType Type)
{
	CtmStorageFile*	pResult = NULL;
	switch(Type)
	{
		case STORAGE_TYPE_SRAM:
			if (m_pSram == NULL)	m_pSram = CtmSram::GetInstance();
			pResult = m_pSram;
			break;
		case STORAGE_TYPE_MMCARD:
			if (m_pMMCard == NULL)	m_pMMCard = CtmMMCardFile::GetInstance();
			pResult = m_pMMCard;
			break;
		case STORAGE_TYPE_FLASH:
		case STORAGE_TYPE_DISK:
			if (m_pFlash == NULL)	m_pFlash = new CtmFlashFile();
			pResult = m_pFlash;
			break;
		case STORAGE_TYPE_USB:
			if (m_pUsb == NULL)		m_pUsb = CtmUsbFile::GetInstance();
			pResult = m_pUsb;
			break;
		case STORAGE_TYPE_FLASHEX:
			g_tmDebugInfo->PrintDebugInfo("Create Flash EX!\n");
			if (m_pFlashEX == NULL)	m_pFlashEX = CtmFlashEx::GetInstance();
			pResult = m_pFlashEX;
			break;
		
			
			
	}
	//m_pFlash
	return pResult;
}

BOOL	CtmMachineBuild::GetStorageFileName(tmStorageType Type, tmFileIDType FileID, char* pszOutFileName)
{
	BOOL	bResult = FALSE;
	if (pszOutFileName != NULL)
	{
		switch(Type)
		{
			case STORAGE_TYPE_SRAM:
				break;
			
			default:
				break;
		}
	}
	return bResult;
}
