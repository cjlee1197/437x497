/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmCurveOilData and CtmCurveDataControl etc		             |
|  Task     : control the sram file 				                         |
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
#include	"sram.h"
#include	"tmconfig.h"
#include	"recordset.h"
#include	"curvedata.h"
#include	"tmcontrol.h"
#include	"moldControl.h"
#include	"machinefactor.h"
#include    "taskdblog.h"
#include	"cblock.h"
#include	"manufacture.h"
#include	"tmdebug.h"
#include	"tmcontrol.h"
#include	"./lib/libDataSave.h"
//#include	"memwatch.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/


//=======
//      Initialize static class member
//=======							
int		g_nDB2Size = 0;

//	FileID, nSize, nTotalSize, nPos, nCheckLen, nCheckValue, nHeadNum
tmFileRulerInfo CtmOilSramRecordFile::m_aFileInfo[] = 
{
    {ID_DB_SRAMFILE,        0,  0,  SRAM_START_NUM+8, 	4,  12345678,   0},
    {ID_DB2_SRAMFILE, 		0,  0,  80000, 	4,  53456789,   0},
    {ID_CURVEDATA_SRAMFILE, 0,  0,  -1, 	4,  23456789,   0},
    {ID_ERROR_SRAMFILE,     0,  0,  -1, 	4,  34567890,   SRAM_ERROR_HEAD_NUM},
    {ID_MONI_SRAMFILE,      0,  0,  -1, 	4,  45678901,   SRAM_MONI_HEAD_NUM},
    {ID_RECORDSET_SRAMFILE, 0,  0,  -1, 	4,  56789012,   1},
    //
    {ID_MANUFACTURE_SRAMFILE,0, 0,  -1, 	4,  78901234,   SRAM_MANU_HEAD_NUM},
	{ID_ONOFFSTATE_SRAMFILE,0, 0, 	-1,		4,  78912340,	1},					// James add  2014/11/11 for HT 
   	{ID_DATASTATE_SRAMFILE, 0, 0, 	400000,	4,	78912345,	1},
    {ID_DATAENERGY_SRAMFILE, 0, 0, 	-1,	4,  78912534,	1},
    {ID_DATAPOWERONOFF_SRAMFILE,0,0,-1, 4,  78912535,	1},
    {ID_DATAENERGY2_SRAMFILE,0,0,	-1, 4,  78912536,	1},
   	{ID_DATAPPH_SRAMFILE,0,0,	-1, 4,  78912536,	1},
   	{ID_TEMPCURVE_SRAMFILE, 0,  0,  524288, 4,  89012345,   0},
    //{ID_MOLDSET_SRAMFILE,   0,  0,  -1, 	4,  67890123,   0}
};

//==================================
//	CtmMMCardRecordFile
//==================================
//	FileID, nSize, nTotalSize, nPos, nCheckLen, nCheckValue, nHeadNum
tmFileRulerInfo CtmMMCardRecordFile::m_aFileInfo[] = 
{
    {ID_DB_SRAMFILE,        0,  0,  0, 	0,  0,  0},
    {ID_ERROR_SRAMFILE,     0,  0,  0, 	0,  0,  SRAM_ERROR_HEAD_NUM},
    {ID_MONI_SRAMFILE,      0,  0,  0, 	0,  0,  SRAM_MONI_HEAD_NUM},
    {ID_RECORDSET_SRAMFILE, 0,  0,  0, 	0,  0,  0},
    {ID_MOLDSET_SRAMFILE,   0,  0,  0, 	0,  0,  0},
    {ID_MACHINE_FILE,		0,	0,	0, 	0,  0,  0},
    {ID_MACHINEB_FILE,		0,	0,	0, 	0,  0,  0},
    {ID_DATABLE_FILE,		0,	0,	0,	0,	0,	0},
    {ID_MACHINE_MACH_FILE ,	0,	0,	0,	0,	0,	0},
    {ID_MACHINEB_MACH_FILE ,0,	0,	0,	0,	0,	0},
	{ID_DATABLE_MACH_FILE ,	0,	0,	0,	0,	0,	0},
    {ID_SYSTEMX_FILE,		0,	0,	0,	0,	0,	0},
    {ID_USERM_FILE,			0,	0,	0,	0,	0,	0}
};

//==================================
//	CtmFlashRecordFile
//==================================
//	FileID, nSize, nTotalSize, nPos, nCheckLen, nCheckValue, nHeadNum
tmFileRulerInfo CtmFlashRecordFile::m_aFileInfo[] = 
{
    {ID_DB_SRAMFILE,        0,  0,  0, 	0,  0,  0},
    {ID_ERROR_SRAMFILE,     0,  0,  0, 	0,  0,  SRAM_ERROR_HEAD_NUM},
    {ID_MONI_SRAMFILE,      0,  0,  0, 	0,  0,  SRAM_MONI_HEAD_NUM},
    {ID_RECORDSET_SRAMFILE, 0,  0,  0, 	0,  0,  0},
    {ID_MOLDSET_SRAMFILE,   0,  0,  0, 	0,  0,  0},
    {ID_MACHINE_FILE,		0,	0,	0, 	0,  0,  0},
    {ID_MACHINEB_FILE,		0,	0,	0, 	0,  0,  0},
    {ID_DATABLE_FILE,		0,	0,	0,	0,	0,	0},
    {ID_MACHINE_MACH_FILE ,	0,	0,	0,	0,	0,	0},
    {ID_MACHINEB_MACH_FILE ,0,	0,	0,	0,	0,	0},
	{ID_DATABLE_MACH_FILE ,	0,	0,	0,	0,	0,	0},
    {ID_SYSTEMX_FILE,		0,	0,	0,	0,	0,	0},
    {ID_USERM_FILE,			0,	0,	0,	0,	0,	0},
    {ID_ZEROSET_FILE,		0,	0,	0,	0,	0,	0}
};

//==================================
//	CtmFlashXRecordFile
//==================================
//	FileID, nSize, nTotalSize, nPos, nCheckLen, nCheckValue, nHeadNum
tmFileRulerInfo CtmFlashEXRecordFile::m_aFileInfo[] = 
{
	{ID_DB_SRAMFILE,   		0,  0,  0, 4,  12345678,   0},
    {ID_MOLDSET_SRAMFILE,   0,  0,  40000, 4,  67890123,   0},
    {ID_DATABLE_FILE,      	0,  0,  630000, 4,  91234567,   0},
    {ID_MACHINE_FILE,     	0,  0,  631100, 4,  01234567,   0},
    {ID_MACHINEB_FILE,     	0,  0,  635100, 4,  01234567,   0}
};
	
CtmSarmRecordFileControl*	CtmSarmRecordFileControl::m_pInstance = NULL;
CtmRecordFileEx*			CtmSarmRecordFileControl::m_pSramFile = NULL;
//CtmRecordFileEx*			CtmSarmRecordFileControl::m_pMMCardFile = NULL;	
/*==========================================================================+
|           Class implementation - CtmHmiRecordFile		    		        |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmHmiRecordFile::CtmHmiRecordFile(): CtmRecordFileEx()
{
	m_nIndexInFiles = -1;
	InitData();
}


CtmHmiRecordFile::CtmHmiRecordFile(CtmStorageFile* pStorage): CtmRecordFileEx(pStorage)
{
	InitData();
}

CtmHmiRecordFile::~CtmHmiRecordFile()
{
	
}

int CtmHmiRecordFile::FindFileType(tmFileRulerInfo* pSource, tmFileIDType FileID, int nStartNo, int nEndNo) 
{
    int nResult = -1;
    for (int i = nStartNo; i <= nEndNo; i++ ) 
    {
        if (pSource[i].FileID == FileID) 
        {
            nResult = i;
            break;
        }
    }
    return nResult;
}

void	CtmHmiRecordFile::InitData()
{
    //  fans add : 20070504
    memset(m_aStorageFileInfo,      0   , sizeof(m_aStorageFileInfo));
}

int		CtmHmiRecordFile::GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID)
{
	int nResult = 0;
	
	m_nIndexInFiles = FindFileType(pSource, FileID, 0, nSourceNum - 1);
           
    if (m_nIndexInFiles >= 0)
    {
		switch (FileID)
		{
			case ID_DB_SRAMFILE:										//	db file
				nResult = GetDBSize(&pSource[m_nIndexInFiles]);
				//printf("DB Size=%d \n", nResult);
				//m_nIndexInFiles = FindFileType(pSource, ID_CURVEDATA_SRAMFILE, 0, nSourceNum - 1);
				//printf("Curve size=%d \n", GetCurveDataSize(&pSource[m_nIndexInFiles]));
				//printf("1 pSource[m_nIndexInFiles+1]->nPos=%d \n", pSource[m_nIndexInFiles+1].nPos);
				//James add for change dbsram size 
				if((CtmConfig::GetInstance()->GetDBSramSize() > pSource[m_nIndexInFiles+1].nPos) && 
					(CtmConfig::GetInstance()->GetDBSramSize() < 350000) && (CtmConfig::GetInstance()->GetDBSramSize() > 0))
				{
					(&pSource[m_nIndexInFiles+1])->nPos = 	CtmConfig::GetInstance()->GetDBSramSize()+8;
				}
				//printf("2 pSource[m_nIndexInFiles+1]->nPos=%d \n", pSource[m_nIndexInFiles+1].nPos);
				break;
			case ID_DB2_SRAMFILE:
				nResult = GetDB2Size(&pSource[m_nIndexInFiles]);
				break;
			case ID_ERROR_SRAMFILE:									    //	error
					nResult = GetErrorSize(&pSource[m_nIndexInFiles]);
				//printf("ERROR Size=%d \n", nResult);
				break;
			case ID_MONI_SRAMFILE:										//	moni file	
					nResult = GetMoniSize(&pSource[m_nIndexInFiles]);
				//printf("MONI Size=%d \n", nResult);
				break;
			case ID_CURVEDATA_SRAMFILE:									//	curvedata file
					nResult = GetCurveDataSize(&pSource[m_nIndexInFiles]);
				//printf("CURVE Size=%d \n", nResult);
				break;
			case ID_RECORDSET_SRAMFILE:									//	modify record file
					nResult = GetModifySetSize(&pSource[m_nIndexInFiles]);
				//printf("RECORD Size=%d \n", nResult);
				break;
			case ID_MOLDSET_SRAMFILE:									//	moldset file
					nResult = GetMoldSetSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_MANUFACTURE_SRAMFILE:								//  manufacture plan file zholy070411
					nResult = GetManufacturePlanSize(&pSource[m_nIndexInFiles]);
				//printf("MANUFACTURE Size=%d \n", nResult);
				break;
			case ID_MACHINE_FILE:										// machine file
			case ID_MACHINE_MACH_FILE:
					nResult = GetMachineSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_MACHINEB_FILE:										// machineb file
			case ID_MACHINEB_MACH_FILE:
					nResult = GetMachineBSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_DATABLE_FILE:										// da table file
			case ID_DATABLE_MACH_FILE:
					nResult = GetDATableSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_SYSTEMX_FILE:
					nResult = GetSystemxSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_USERM_FILE:
					nResult = GetUserSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_ONOFFSTATE_SRAMFILE:
					nResult = GetOnOffStateSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_DATASTATE_SRAMFILE:
					nResult = GetDataStateSize(&pSource[m_nIndexInFiles]);
				break;
			case ID_DATAENERGY_SRAMFILE:
					nResult = GetDataEnergy(&pSource[m_nIndexInFiles]);
				break;
			case ID_DATAPOWERONOFF_SRAMFILE:
					nResult = GetDataPowerOnOff(&pSource[m_nIndexInFiles]);
				break;
			case ID_DATAENERGY2_SRAMFILE:
					nResult = GetDataEnergy2(&pSource[m_nIndexInFiles]);
				break;
			case ID_DATAPPH_SRAMFILE:
					nResult = GetDataPPH(&pSource[m_nIndexInFiles]);
				break;
			default:
				break;
		}
	}
	
	return nResult;
}

int		CtmHmiRecordFile::GetSize(int nType)
{
	int	nResult = 0;
	return nResult;
}

int		CtmHmiRecordFile::GetErrorSize(tmFileRulerInfo* pErrorFileInfo)
{
	int nResult = 0;
    if (pErrorFileInfo->nSize <= 0) 
    {
        nResult = sizeof(tmDB_ERROR);
        pErrorFileInfo->nSize = nResult;
        pErrorFileInfo->nTotalSize = 
            (CtmConfig::GetInstance()->GetErrorMaxNum() + pErrorFileInfo->nHeadNum) * nResult;
    }

    return nResult;	
}

int		CtmHmiRecordFile::GetDBSize(tmFileRulerInfo* pDBFileInfo)
{
	int nResult = 0;
    if (pDBFileInfo->nSize <= 0) 
    {
        CtmFile	File;
		//Sunny<20070622> amend
		char	szTemp[512];
    	sprintf(szTemp, "%s", CtmConfig::GetInstance()->GetDBAttr());		
		if (File.Open(szTemp) == tmFILE_GOOD)
        //if (File.Open("res/db/attr") == tmFILE_GOOD)
        {
            File.Seek(0, SEEK_SET);
            File.Read(&nResult, sizeof(nResult));
            File.Close();
        }
        pDBFileInfo->nSize = nResult;
        pDBFileInfo->nTotalSize = nResult;
    }
    
    return nResult;
}

int		CtmHmiRecordFile::GetDB2Size(tmFileRulerInfo* pDBFileInfo)
{
	int nResult = 0;
    if (pDBFileInfo->nSize <= 0) 
    {
        pDBFileInfo->nSize = g_nDB2Size;
        pDBFileInfo->nTotalSize = g_nDB2Size;
        //printf("init DB2 g_nDB2Size=%d \n", g_nDB2Size);
    }
    
    return nResult;
}

int		CtmHmiRecordFile::GetMoniSize(tmFileRulerInfo* pMoniFileInfo)
{
	int nResult = 0;
    if (pMoniFileInfo->nSize <= 0) 
    {
    	//James add 在lib返回監測結構大小
        //nResult = sizeof(tmDB_MONI2);
        nResult = GetMoniStructSize();
        pMoniFileInfo->nSize = nResult;
        pMoniFileInfo->nTotalSize = 
            (CtmConfig::GetInstance()->GetMoniMaxNum() + pMoniFileInfo->nHeadNum) * nResult;
    }
    return nResult;
}

int		CtmHmiRecordFile::GetCurveDataSize(tmFileRulerInfo* pCurveFileInfo)
{
	int nResult = 0, nCurveNum = 0;
    if (pCurveFileInfo->nSize <= 0) 
    {
        nResult = sizeof(DBCURVE);
        pCurveFileInfo->nSize = nResult;
		
		if(0 == CtmConfig::GetInstance()->GetCommType())
			nCurveNum = MAX_CURVE_NUM;
        pCurveFileInfo->nTotalSize = (nCurveNum + pCurveFileInfo->nHeadNum) * nResult;
    }

    return nResult;	
}

int		CtmHmiRecordFile::GetModifySetSize(tmFileRulerInfo* pRecordFileInfo)
{
	int nResult = 0;
    if (pRecordFileInfo->nSize <= 0) 
    {
        nResult = sizeof(tmModifyRecordSet_Oil);
        pRecordFileInfo->nSize = nResult;
        pRecordFileInfo->nTotalSize = 
            (CtmConfig::GetInstance()->GetRecordSetMaxNum() + pRecordFileInfo->nHeadNum) * nResult;
    }

    return nResult;
}

int		CtmHmiRecordFile::GetMoldSetSize(tmFileRulerInfo* pMoldFileInfo)
{
	int nResult = 0;
    if (pMoldFileInfo->nSize <= 0) 
    {
    	if(g_pBlock != NULL )
        nResult = g_pBlock->GetBlockSize(BLOCK_MOLDSETX_ID); // CtmMoldSet::GetInstance()->GetMoldSize();
        pMoldFileInfo->nSize = nResult;
        pMoldFileInfo->nTotalSize = 
        	(CtmConfig::GetInstance()->GetMoldSetMaxNum() + pMoldFileInfo->nHeadNum) * nResult;
    }

    return nResult;
}

int		CtmHmiRecordFile::GetManufacturePlanSize(tmFileRulerInfo* pManuFileInfo)//zholy070411
{
	int	nResult = 0;
	
	if (pManuFileInfo->nSize <= 0)
	{
		nResult = CtmManufacturePlan::GetInstance()->GetPlanSize();
		pManuFileInfo->nSize = nResult;
        pManuFileInfo->nTotalSize = CtmManufacturePlan::GetInstance()->GetPlanTotalSize() + nResult * pManuFileInfo->nHeadNum;
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMachineSize(tmFileRulerInfo*	pMachineFileInfo)			|	
|  Task     :  獲得機器資料包大小                           		 		|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::GetMachineSize(tmFileRulerInfo*	pMachineFileInfo)
{
	int	nResult = 0;
	
	if (pMachineFileInfo->nSize <= 0 )
	{
		nResult = g_pBlock->GetBlockSize(BLOCK_MACHINE_ID);
		pMachineFileInfo->nSize = nResult;
		pMachineFileInfo->nTotalSize = nResult;
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMachineBSize(tmFileRulerInfo*	pMachineFileInfo)			|	
|  Task     :  獲得機器資料包大小                           		 		|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::GetMachineBSize(tmFileRulerInfo*	pMachineFileInfo)
{
	int	nResult = 0;
	
	if (pMachineFileInfo->nSize <= 0 )
	{
		nResult = g_pBlock->GetBlockSize(BLOCK_MACHINEB_ID);
		pMachineFileInfo->nSize = nResult;
		pMachineFileInfo->nTotalSize = nResult;
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDATableSize(tmFileRulerInfo*	pMachineFileInfo)			|	
|  Task     :  獲得比例閥曲線資料包大小                           		 	|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::GetDATableSize(tmFileRulerInfo*	pDATableFileInfo)
{
	int	nResult = 0;
	
	if (pDATableFileInfo->nSize <= 0 )
	{
		nResult = g_pBlock->GetBlockSize(BLOCK_NEWPOWERMATCH_ID);
		pDATableFileInfo->nSize = nResult;
		pDATableFileInfo->nTotalSize = nResult;
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  GetSystemxSize(tmFileRulerInfo*	pMachineFileInfo)		    |	
|  Task     :  獲得Systemx資料包大小                           		 	    |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::GetSystemxSize(tmFileRulerInfo*	pDATableFileInfo)
{
	int	nResult = 0;
	
	if (pDATableFileInfo->nSize <= 0 )
	{
		nResult = g_pBlock->GetBlockSize(BLOCK_SYSTEMX_ID);
		pDATableFileInfo->nSize = nResult;
		pDATableFileInfo->nTotalSize = nResult;
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  GetUserSize(tmFileRulerInfo*	pMachineFileInfo)			|	
|  Task     :  獲得User資料包大小                           		 	|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::GetUserSize(tmFileRulerInfo*	pDATableFileInfo)
{
	int	nResult = 0;
	
	if (pDATableFileInfo->nSize <= 0 )
	{
		nResult = g_pBlock->GetBlockSize(BLOCK_USERM_ID);
		pDATableFileInfo->nSize = nResult;
		pDATableFileInfo->nTotalSize = nResult;
	}
	
	return nResult;
}

int		CtmHmiRecordFile::GetOnOffStateSize(tmFileRulerInfo*	pOnOffStateFileInfo)
{
	int	nResult = 0;
	
	if (pOnOffStateFileInfo->nSize <= 0 )
	{
		nResult = sizeof(ONOFFSTATE);
		pOnOffStateFileInfo->nSize = nResult;
		pOnOffStateFileInfo->nTotalSize = (CtmConfig::GetInstance()->GetDataStateMaxNum()+ pOnOffStateFileInfo->nHeadNum) * nResult;
	}
	
	return nResult;
}

//讀取DataState資料大小，測試暫定100筆
int		CtmHmiRecordFile::GetDataStateSize(tmFileRulerInfo*	pDataStateFileInfo)
{
	int	nResult = 0;
	
	if (pDataStateFileInfo->nSize <= 0 )
	{
		nResult = sizeof(STATEMODE);
		pDataStateFileInfo->nSize = nResult;
		pDataStateFileInfo->nTotalSize = (CtmConfig::GetInstance()->GetDataStateMaxNum()+ pDataStateFileInfo->nHeadNum) * nResult;
	}
	
	return nResult;
}

//讀取DataEnergy資料大小
int		CtmHmiRecordFile::GetDataEnergy(tmFileRulerInfo*	pDataEnergyFileInfo)
{
	int	nResult = 0;
	
	if (pDataEnergyFileInfo->nSize <= 0 )
	{
		nResult = sizeof(TMCAPACITY);
		pDataEnergyFileInfo->nSize = nResult;
		pDataEnergyFileInfo->nTotalSize = (CtmConfig::GetInstance()->GetDataEnergyMaxNum() + pDataEnergyFileInfo->nHeadNum) * nResult;
	}
	
	return nResult;
}

//讀取DataPowerOnOff資料大小
int		CtmHmiRecordFile::GetDataPowerOnOff(tmFileRulerInfo*	pDataPowerOnOffFileInfo)
{
	int	nResult = 0;
	
	if (pDataPowerOnOffFileInfo->nSize <= 0 )
	{
		nResult = sizeof(TMPOWERONOFF);
		pDataPowerOnOffFileInfo->nSize = nResult;
		pDataPowerOnOffFileInfo->nTotalSize = (CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum() + pDataPowerOnOffFileInfo->nHeadNum) * nResult;
	}
	
	return nResult;
}

int		CtmHmiRecordFile::GetDataEnergy2(tmFileRulerInfo*	pDataEnergy2FileInfo)
{
	int	nResult = 0;
	int nMax = CtmConfig::GetInstance()->GetDataEnergy2MaxNum();
	if (pDataEnergy2FileInfo->nSize <= 0 )
	{
		nResult = sizeof(AMMETERDATA);//GetEnergy2StructSize();
		pDataEnergy2FileInfo->nSize = nResult;
		if(nMax <=0 )
			nMax = 1;
		pDataEnergy2FileInfo->nTotalSize = (nMax + pDataEnergy2FileInfo->nHeadNum) * nResult;
	}
	
	return nResult;
}
int		CtmHmiRecordFile::GetDataPPH(tmFileRulerInfo*	pDataPPHFileInfo)
{
	int	nResult = 0;
	int nMax = CtmConfig::GetInstance()->GetDataEnergy2MaxNum();
	if (pDataPPHFileInfo->nSize <= 0 )
	{
		nResult = sizeof(TMPPHDATA);
		pDataPPHFileInfo->nSize = nResult;
		if(nMax <=0 )
			nMax = 1;
		pDataPPHFileInfo->nTotalSize = (nMax + pDataPPHFileInfo->nHeadNum) * nResult;
	}
	
	return nResult;
}
/*--------------------------------------------------------------------------+
|  Function :  WriteToStorage(int nPos, int nLength, void* pRecord, int nPerSize)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::WriteToStorage(int nPos, int nLength, void* pRecord, int nPerSize)
{
	int Result = -1;
	int nLen = 0; 
	
	if (nLength < 0) nLen = nPerSize;
	else nLen = nLength;
	
	Result = m_pStorage->Write(nPos, nLen, (BYTE*)pRecord);
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadFromStorage(int nPos, int nLength, void* pRecord, int nPerSize)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int 	CtmHmiRecordFile::ReadFromStorage(int nPos, int nLength, void* pRecord, int nPerSize)
{
	int Result = -1;
	int nLen = 0;
	if (nLength < 0) nLen = nPerSize;
	else nLen = nLength;
	Result = m_pStorage->Read(nPos, nLen, (BYTE*)pRecord);
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadRecord(int nType, int nRecordNo, void* pRecord)			|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::ReadRecord  (int nType, int nRecordNo, void* pRecord)
{
	return ReadRecord(nType, nRecordNo, pRecord, -1);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadRecord(int nType, int nRecordNo, void* pRecord, int nLength)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int 	CtmHmiRecordFile::ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength)
{
	return ReadRecord(nType, nRecordNo, pRecord, nLength, 0);
}
    
/*--------------------------------------------------------------------------+
|  Function :  ReadRecord(int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)
{	
    int nResult = 0;
    int nIndex = 0;
   	nIndex = FindFileType(m_aStorageFileInfo, (tmFileIDType)nType, 0, sizeof(m_aStorageFileInfo) / sizeof(tmFileRulerInfo));
    if (nIndex >= 0) 
    {
        m_pStorage->ReInit(nType);
        if (CheckData(nType) && m_aStorageFileInfo[nIndex].nSize > 0 ) 
        {
            nRecordNo = nRecordNo + m_aStorageFileInfo[nIndex].nHeadNum;
 
            int nPos = (nRecordNo - 1) * m_aStorageFileInfo[nIndex].nSize +
                m_aStorageFileInfo[nIndex].nPos + nOffset;
            
            nResult = ReadFromStorage(nPos, nLength, pRecord, m_aStorageFileInfo[nIndex].nSize); 
        }
    }

   return nResult;
}


/*--------------------------------------------------------------------------+
|  Function :  WriteRecord(int nType, int nRecordNo, void* pRecord)			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::WriteRecord (int nType, int nRecordNo, void* pRecord)
{
	//if (nType == 1)
	//	printf("CtmHmiRecordFile::WriteRecord (nType,nRecordNo,pRecord)\n");
	return	WriteRecord(nType, nRecordNo, pRecord, -1);	//	-1 for autosize
}

/*--------------------------------------------------------------------------+
|  Function :  WriteRecord(int nType, int nRecordNo, void* pRecord, int nLength)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength)
{
	//if (nLength > 20000 || nLength == -1)
	//	printf("CtmHmiRecordFile::WriteRecord (nType,nRecordNo,pRecord, nLength)\n");
	return	WriteRecord(nType, nRecordNo, pRecord, nLength, 0);
}

/*--------------------------------------------------------------------------+
|  Function :  WriteRecord(int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)
{
	//if (nLength > 20000 || nLength == -1)
	//printf("CtmHmiRecordFile::WriteRecord (%d,%d,pRecord, %d, %d)\n", nType, nRecordNo, 
	//			nLength, nOffset);
    int nResult = 0;
    int nPos = -1;
    int nIndex = FindFileType(m_aStorageFileInfo, (tmFileIDType)nType, 0, sizeof(m_aStorageFileInfo) / sizeof(tmFileRulerInfo) - 1);

    if (nIndex >= 0) 
    {
        m_pStorage->ReInit(nType);
        nRecordNo = nRecordNo + m_aStorageFileInfo[nIndex].nHeadNum;
        if (!CheckData(nType)) 
        {
        	//printf("CheckData Error \n");
            nPos = m_aStorageFileInfo[nIndex].nPos - m_aStorageFileInfo[nIndex].nCheckLen;
			
			//printf("m_aStorageFileInfo[nIndex].nCheckLen=%d \n", m_aStorageFileInfo[nIndex].nCheckLen);
            if (m_aStorageFileInfo[nIndex].nCheckLen > 0) 
            {
            	//printf("pos=%d size=%d \n", m_aStorageFileInfo[nIndex].nPos, m_aStorageFileInfo[nIndex].nSize);
                if (m_aStorageFileInfo[nIndex].nPos >= 0 && m_aStorageFileInfo[nIndex].nSize > 0) 
                {
                    DeleteRecord(nType);
                    nResult = m_pStorage->Write(nPos,
                                                m_aStorageFileInfo[nIndex].nCheckLen, 
                                                (BYTE*)(&m_aStorageFileInfo[nIndex].nCheckValue));
               		// printf("nPos=%d len=%d value=%x \n", nPos, m_aStorageFileInfo[nIndex].nCheckLen, m_aStorageFileInfo[nIndex].nCheckValue);
                }
            }
        }

        nPos = (nRecordNo - 1) * m_aStorageFileInfo[nIndex].nSize + m_aStorageFileInfo[nIndex].nPos +
            nOffset;
        //if(nType >= 8 && nType <= 13)
       // printf("nType = %d, nPos = %d, nLength = %d, nSize = %d\n", nType, nPos, nLength, m_aStorageFileInfo[nIndex].nSize);
        if (nPos >= 0 && m_aStorageFileInfo[nIndex].nSize > 0) 
        {
            nResult = WriteToStorage(nPos, nLength, (BYTE*)pRecord, m_aStorageFileInfo[nIndex].nSize);
        }
  
    }

    return nResult;

}

/*--------------------------------------------------------------------------+
|  Function :	DeleteRecord(int nType, int nRecordNo)						|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::DeleteRecord(int nType, int nRecordNo)
{
	int Result = 0;
	Result = EraseRecord(nType, nRecordNo);
	return Result;	
}

/*--------------------------------------------------------------------------+
|  Function :	EraseRecord(int nType, int nRecordNo)						|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int 	CtmHmiRecordFile::EraseRecord (int nType, int nRecordNo)
{
    int nResult = 0;
    int nIndex = FindFileType(m_aStorageFileInfo, (tmFileIDType)nType, 0, sizeof(m_aStorageFileInfo) / sizeof(tmFileRulerInfo) - 1);
    
    if (nIndex >= 0) 
    {
         m_pStorage->ReInit(nType);
         if (m_aStorageFileInfo[nIndex].nPos >= 0 && CheckData(nType) &&
             m_aStorageFileInfo[nIndex].nSize >= 0) 
         {
             BYTE*  pTemp = new BYTE[m_aStorageFileInfo[nIndex].nSize];
             memset(pTemp,0, m_aStorageFileInfo[nIndex].nSize);
             nRecordNo = nRecordNo + m_aStorageFileInfo[nIndex].nHeadNum;
             nResult = m_pStorage->Write(m_aStorageFileInfo[nIndex].nPos + (nRecordNo - 1) * m_aStorageFileInfo[nIndex].nSize,
                                         m_aStorageFileInfo[nIndex].nSize, pTemp);                
             delete[] pTemp;
         }
    }
    return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :	DeleteRecord(int nType)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::DeleteRecord(int nType)
{
	int Result = 0;
	Result = EraseRecord(nType);
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :	EraseRecord (int nType)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmHmiRecordFile::EraseRecord (int nType)
{
    int nResult = 0;
    int nIndex = FindFileType(m_aStorageFileInfo, (tmFileIDType)nType, 0, sizeof(m_aStorageFileInfo) / sizeof(tmFileRulerInfo) - 1);

    if (nIndex >= 0) 
    {
        m_pStorage->ReInit(nType);
		
		if (m_aStorageFileInfo[nIndex].nCheckLen >0) 
        {
            int nPos = m_aStorageFileInfo[nIndex].nPos - m_aStorageFileInfo[nIndex].nCheckLen;
			if (nPos >= 0 && m_aStorageFileInfo[nIndex].nSize > 0) 
            {
                nResult = m_pStorage->Write(nPos, 
                                            m_aStorageFileInfo[nIndex].nCheckLen,
                                            (BYTE*)&m_aStorageFileInfo[nIndex].nCheckValue);
            }
        }
		
		if (m_aStorageFileInfo[nIndex].nPos >= 0 &&
            m_aStorageFileInfo[nIndex].nTotalSize > 0) 
        {
            BYTE*   pTemp = new BYTE[m_aStorageFileInfo[nIndex].nTotalSize];
            memset(pTemp, 0, m_aStorageFileInfo[nIndex].nTotalSize);
            
            nResult =  m_pStorage->Write(m_aStorageFileInfo[nIndex].nPos,
                                         m_aStorageFileInfo[nIndex].nTotalSize, pTemp);
            delete []pTemp;
        }

    }


    return nResult;
}

/*==========================================================================+
|           Class implementation - CtmOilSramRecordFile	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmOilSramRecordFile::CtmOilSramRecordFile(): CtmHmiRecordFile()
{
	//m_pStorage = CtmSram::GetInstance();
	m_pStorage = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_SRAM);
	InitData();
}

CtmOilSramRecordFile::CtmOilSramRecordFile(CtmStorageFile* pStorage): CtmHmiRecordFile(pStorage)
{
	InitData();
}

CtmOilSramRecordFile::~CtmOilSramRecordFile()
{
	
	//Destroy_ErrorLog();
}

void	CtmOilSramRecordFile::InitData()
{
    for (unsigned int i = 0; i < sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo); i++) 
    {
        GetSize(m_aFileInfo[i].FileID);
        GetPos(m_aFileInfo[i].FileID);
//		printf("id=%d, pos=%d, size=%d, totalSize=%d\n", 
//        								(int)m_aFileInfo[i].FileID,  
//        								m_aFileInfo[i].nPos, 
//        								m_aFileInfo[i].nSize, 
//        								m_aFileInfo[i].nTotalSize);
//        g_tmDebugInfo->PrintDebugInfo("id=%d, pos=%d, size=%d, totalSize=%d\n", 
//        								(int)m_aFileInfo[i].FileID,  
//        								m_aFileInfo[i].nPos, 
//        								m_aFileInfo[i].nSize, 
//        								m_aFileInfo[i].nTotalSize);
    }

    memcpy(m_aStorageFileInfo,      m_aFileInfo,    sizeof(m_aFileInfo));
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  GetSize(int nType)											|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/     

int	CtmOilSramRecordFile::GetTempCurveSize(tmFileRulerInfo* pTempCurveFileInfo)
{
	int nResult = 0;
	if (pTempCurveFileInfo->nSize <= 0)
    {
    	nResult = CTaskDBLog::GetTempSramSize(); 
        pTempCurveFileInfo->nSize = nResult;
        pTempCurveFileInfo->nTotalSize = nResult;
    }
	return nResult;
}
     
int		CtmOilSramRecordFile::GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID)
{
	int nResult = 0;
	nResult = CtmHmiRecordFile::GetEverySize(pSource, nSourceNum, FileID);
		
	if (m_nIndexInFiles >= 0)
	{
		switch (FileID)
		{
			case ID_TEMPCURVE_SRAMFILE:													
					nResult = GetTempCurveSize(&pSource[m_nIndexInFiles]);
				break;
			default:
				break;		
		}
	}
	
	return nResult;
}

int	CtmOilSramRecordFile::GetSize(int nType) 																
{
	return GetEverySize(m_aFileInfo, sizeof(m_aFileInfo) / sizeof(m_aFileInfo[0]), (tmFileIDType)nType);
}

/*--------------------------------------------------------------------------+
|  Function :  GetPos(int nType)											|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
int		CtmOilSramRecordFile::GetPos(int nType)
{
    int nResult = -1;
    int nIndex = FindFileType(m_aFileInfo, (tmFileIDType)nType, 0, 
                              sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo) - 1);

    if (nIndex == 0) 
    {
    	//printf("m_aFileInfo[nIndex].nPos=%d \n", m_aFileInfo[nIndex].nPos);
        //James modify 2008/11/10 10:20AM
        m_aFileInfo[nIndex].nPos = m_aFileInfo[nIndex].nCheckLen+m_aFileInfo[nIndex].nPos;
        nResult = m_aFileInfo[nType].nPos;
    }
    else
    {
        if (m_aFileInfo[nIndex].nPos < 0)
        {
            if (m_aFileInfo[nIndex - 1].nPos >= 0)            
                nResult = m_aFileInfo[nIndex - 1].nPos + m_aFileInfo[nIndex - 1].nTotalSize + 
                    m_aFileInfo[nIndex].nCheckLen;
            else
                 nResult = m_aFileInfo[nIndex].nCheckLen;

            m_aFileInfo[nIndex].nPos = nResult;

        }
    }

    return nResult;
}



/*--------------------------------------------------------------------------+
|  Function :	CheckData (int nType)										|	
|  Task     :   check the validity of the type data                         |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter: 	nType == -1, it means  check the all data's validity		|
|  				ID_END_SRAMFILE to ID_START_SRAMFILE, in the sram 			|
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/ 
BOOL CtmOilSramRecordFile::CheckData(int nType)
{
    BOOL    bResult= FALSE;
    int     nTempCheck = 0;
    int     nIndex = 0;
    switch(nType)
    {
    case -1:
        for(unsigned int i = 0; i < sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo); i++)
        {
            nIndex = FindFileType(m_aFileInfo, m_aFileInfo[i].FileID, 0, 
                                  sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo) - 1);

            if (nIndex >= 0) 
            {
                if (m_aFileInfo[nIndex].nCheckLen > 0)
                {
                    m_pStorage->Read(m_aFileInfo[nIndex].nPos - m_aFileInfo[nIndex].nCheckLen,
                                     m_aFileInfo[nIndex].nCheckLen,
                                     (BYTE*)&nTempCheck);
                   	//printf("nTempCheck =%d m_aFileInfo[nIndex].nCheckValue =%d\n", nTempCheck, m_aFileInfo[nIndex].nCheckValue);
                    bResult = (nTempCheck == m_aFileInfo[nIndex].nCheckValue);
                }
                else bResult = TRUE;

            }
            else bResult = FALSE;

            if (!bResult) break;
        }
        break;
    default:
        nIndex = FindFileType(m_aFileInfo, (tmFileIDType)nType, 0,
                              sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo) - 1);
        if (nIndex >= 0) 
        {
            if (m_aFileInfo[nIndex].nCheckLen > 0)
            {
                m_pStorage->Read(m_aFileInfo[nIndex].nPos - m_aFileInfo[nIndex].nCheckLen,
                                 m_aFileInfo[nIndex].nCheckLen,
                                 (BYTE*)&nTempCheck);
                 //printf("m_aFileInfo[nIndex].nPos=%d checklen=%d \n", m_aFileInfo[nIndex].nPos, m_aFileInfo[nIndex].nCheckLen
                // );
                // printf("2 nIndex=%d nTempCheck =%x m_aFileInfo[nIndex].nCheckValue =%x\n", nIndex, nTempCheck, m_aFileInfo[nIndex].nCheckValue);
                bResult = (nTempCheck == m_aFileInfo[nIndex].nCheckValue);
            }
            else bResult = TRUE;
        }
        else bResult = FALSE;
        break;
    }

    return bResult;
}



/*==========================================================================+
|           Class implementation - CtmSarmRecordFileControl		            |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmSarmRecordFileControl::CtmSarmRecordFileControl()
{
	m_pSramFile	= CtmMachineBuild::GetInstance()->BuildCStorageRecordFile();
	//pthread_mutex_init(&m_WriteMutex, NULL);
	//m_pSramFile	= NULL;
}

CtmSarmRecordFileControl::~CtmSarmRecordFileControl()
{
	//pthread_mutex_destroy(&m_WriteMutex);
	//if (m_pSramFile != NULL)
	//{
	//	delete m_pSramFile;
	//	m_pSramFile = NULL;
	//}
}

CtmSarmRecordFileControl*	CtmSarmRecordFileControl::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CtmSarmRecordFileControl;
	return	m_pInstance;
}

void		CtmSarmRecordFileControl::ReleaseInstance()
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
|  Function :  GetSize(int nType)											|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::GetSize(int nType, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->GetSize(nType);
	else return 0;
}										
				
/*--------------------------------------------------------------------------+
|  Function :  ReadRecord(int nType, int nRecordNo, void* pRecord)			|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/		
int	CtmSarmRecordFileControl::ReadRecord(int nType, int nRecordNo, void* pRecord, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->ReadRecord(nType, nRecordNo, pRecord);
	else return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadRecord(int nType, int nRecordNo, void* pRecord, int nLength)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int CtmSarmRecordFileControl::ReadRecord  (int nType, int nRecordNo, void* pRecord, int nLength, 
									tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->ReadRecord(nType, nRecordNo, pRecord, nLength);
	else return 0;
}		

/*--------------------------------------------------------------------------+
|  Function :  ReadRecord(int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::ReadRecord(int nType, int nRecordNo, void* pRecord, int nLength, int nOffset, 
								tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->ReadRecord(nType, nRecordNo, pRecord, nLength, nOffset);
	else return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteRecord (int nType, int nRecordNo, void* pRecord)		|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::WriteRecord (int nType, int nRecordNo, void* pRecord, tmStorageType StorageType)
{
	//printfTM("CtmSarmRecordFileControl::WriteRecord(4), storage=%d\n", StorageType);
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	//m_pSramFile->ReInit(nType);
	if (m_pSramFile != NULL) 
		return m_pSramFile->WriteRecord(nType, nRecordNo, pRecord);
	else return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, tmStorageType StorageType)
{
	//printfTM("CtmSarmRecordFileControl::WriteRecord(5), storage=%d\n", StorageType);
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->WriteRecord(nType, nRecordNo, pRecord, nLength);
	else return 0;
}			

/*--------------------------------------------------------------------------+
|  Function :  WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::WriteRecord (int nType, int nRecordNo, void* pRecord, int nLength, int nOffset, tmStorageType StorageType)
{
	//pthread_mutex_lock(&m_WriteMutex);
	int	nResult = 0;
	//if (nType == 8)
	//	printf("StorageType =%d\n", StorageType);
	//printfTM("CtmSarmRecordFileControl::WriteRecord(6), storage=%d\n", StorageType);
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) 
	{
		//if (nType == 8) printf("m_pSramFile != NULL\n");
		
		
		nResult	= m_pSramFile->WriteRecord(nType, nRecordNo, pRecord, nLength, nOffset);
		//pthread_mutex_unlock(&m_WriteMutex);
		return nResult;
	}
	else 
	{
		//pthread_mutex_unlock(&m_WriteMutex);
		//if (nType == 8) printf("m_pSramFile = NULL\n");
		return 0;
	}
}

/*--------------------------------------------------------------------------+
|  Function :  DeleteRecord(int nType, int nRecordNo)						|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::DeleteRecord(int nType, int nRecordNo, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->DeleteRecord(nType, nRecordNo);
	else return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  EraseRecord (int nType, int nRecordNo)						|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int CtmSarmRecordFileControl::EraseRecord (int nType, int nRecordNo, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->EraseRecord(nType, nRecordNo);
	else return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  DeleteRecord(int nType)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::DeleteRecord(int nType, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->DeleteRecord(nType);
	else return 0;
}	

/*--------------------------------------------------------------------------+
|  Function :  EraseRecord (int nType)										|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CtmSarmRecordFileControl::EraseRecord (int nType, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->EraseRecord(nType);
	else return 0;
}

BOOL 	CtmSarmRecordFileControl::CheckData(int nType, tmStorageType StorageType)
{
	m_pSramFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(StorageType);
	if (m_pSramFile != NULL) return m_pSramFile->CheckData(nType);
	else return FALSE;
}

CtmFlashEXRecordFile::CtmFlashEXRecordFile(CtmStorageFile* pStorage): CtmHmiRecordFile(pStorage)
{
	InitData();
}


CtmFlashEXRecordFile::CtmFlashEXRecordFile()
{
	m_pStorage = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_FLASHEX);
	InitData();
}

CtmFlashEXRecordFile::~CtmFlashEXRecordFile()
{
}

void	CtmFlashEXRecordFile::InitData()
{
	for (unsigned int i = 0; i < sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo); i++) 
    {
        GetSize(m_aFileInfo[i].FileID);
        GetPos(m_aFileInfo[i].FileID);
      	g_tmDebugInfo->PrintDebugInfo("FlashEX id=%d, pos=%d, size=%d, totalSize=%d\n", 
        								(int)m_aFileInfo[i].FileID,  
        								m_aFileInfo[i].nPos, 
        								m_aFileInfo[i].nSize, 
        								m_aFileInfo[i].nTotalSize);
    }

    memcpy(m_aStorageFileInfo,      m_aFileInfo,    sizeof(m_aFileInfo));
}

int		CtmFlashEXRecordFile::GetSize(int nType)
{
	return GetEverySize(m_aFileInfo, sizeof(m_aFileInfo) / sizeof(m_aFileInfo[0]), (tmFileIDType)nType);
}

int		CtmFlashEXRecordFile::GetMoldSetSize(tmFileRulerInfo* pMoldFileInfo)
{
	int nResult = 0;
    if (pMoldFileInfo->nSize <= 0) 
    {
        if(g_pBlock != NULL )
        nResult = g_pBlock->GetBlockSize(BLOCK_MOLDSETX_ID); // CtmMoldSet::GetInstance()->GetMoldSize();
        pMoldFileInfo->nSize = nResult;	
        pMoldFileInfo->nTotalSize = nResult * ( CtmConfig::GetInstance()->GetMoldSetMaxNum() + pMoldFileInfo->nHeadNum );
    }

    return nResult;
}


BOOL CtmFlashEXRecordFile::CheckData(int nType)
{
	return TRUE;
    BOOL    bResult= FALSE;
    int     nTempCheck = 0;
    int     nIndex = 0;
    switch(nType)
    {
    case -1:
        for(unsigned int i = 0; i < sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo); i++)
        {
            nIndex = FindFileType(m_aFileInfo, m_aFileInfo[i].FileID, 0, 
                                  sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo) - 1);
    
            if (nIndex >= 0) 
            {
                if (m_aFileInfo[nIndex].nCheckLen > 0)
                {
                    m_pStorage->Read(m_aFileInfo[nIndex].nPos - m_aFileInfo[nIndex].nCheckLen,
                                     m_aFileInfo[nIndex].nCheckLen,
                                     (BYTE*)&nTempCheck);
                    bResult = (nTempCheck == m_aFileInfo[nIndex].nCheckValue);
                }
                else bResult = TRUE;
    
            }
            else bResult = FALSE;
    
            if (!bResult) break;
        }
        break;
    default:
        nIndex = FindFileType(m_aFileInfo, (tmFileIDType)nType, 0,
                              sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo) - 1);
        if (nIndex >= 0) 
        {
            if (m_aFileInfo[nIndex].nCheckLen > 0)
            {
                m_pStorage->Read(m_aFileInfo[nIndex].nPos - m_aFileInfo[nIndex].nCheckLen,
                                 m_aFileInfo[nIndex].nCheckLen,
                                 (BYTE*)&nTempCheck);
                g_tmDebugInfo->PrintDebugInfo("check value =%d nTempCheck =%d \n", m_aFileInfo[nIndex].nCheckValue, nTempCheck);
                bResult = (nTempCheck == m_aFileInfo[nIndex].nCheckValue);
            }
            else bResult = TRUE;
        }
        else bResult = FALSE;
        break;
    }
    
    return bResult;
}


/*--------------------------------------------------------------------------+
|  Function :  GetPos(int nType)											|	
|  Task     :  根據存儲數據類型，計算FlashEX區域的nPos地址      			|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:  int nType:	數據分類		                              	|
|              原理: FlashEx區域對于每一種存儲數據分配的地址和大小是固定的, |
|					 nPos從0開始，前面四個字節一般為校驗數據。 此分配地址是 |
|					 自動分配，根據數據占用的最大空間分配地址				|
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmFlashEXRecordFile::GetPos(int nType)
{
    int nResult = -1;
    int nIndex = FindFileType(m_aFileInfo, (tmFileIDType)nType, 0, 
                              sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo) - 1);

    if (nIndex == 0) 
    {
        m_aFileInfo[nIndex].nPos = m_aFileInfo[nIndex].nCheckLen+m_aFileInfo[nIndex].nPos;
        nResult = m_aFileInfo[nType].nPos;
    }
    else
    {
        if (m_aFileInfo[nIndex].nPos < 0)
        {
            if (m_aFileInfo[nIndex - 1].nPos >= 0)            
                nResult = m_aFileInfo[nIndex - 1].nPos + m_aFileInfo[nIndex - 1].nTotalSize + 
                    m_aFileInfo[nIndex].nCheckLen;
            else
                 nResult = m_aFileInfo[nIndex].nCheckLen;

            m_aFileInfo[nIndex].nPos = nResult;

        }
    }

    return nResult;
}


CtmMMCardRecordFile::CtmMMCardRecordFile(CtmStorageFile* pStorage): CtmHmiRecordFile(pStorage)
{
	InitData();
}


CtmMMCardRecordFile::CtmMMCardRecordFile()
{
	m_pStorage = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_MMCARD);
	InitData();
}

		
CtmMMCardRecordFile::~CtmMMCardRecordFile()
{
	CtmMachineBuild::GetInstance()->FreeStorageRecordFile(STORAGE_TYPE_MMCARD);
}
		
void	CtmMMCardRecordFile::InitData()
{
	 for (unsigned int i = 0; i < sizeof(m_aFileInfo) / sizeof(tmFileRulerInfo); i++) 
    {
        GetSize(m_aFileInfo[i].FileID);
    }

    memcpy(m_aStorageFileInfo,      m_aFileInfo,    sizeof(m_aFileInfo));
}

int CtmMMCardRecordFile::GetMachineSize(tmFileRulerInfo* pMachineFileInfo) 
{
    int nResult = 0;

    return nResult;
}


int		CtmMMCardRecordFile::GetSize(int nType)
{
	return GetEverySize(m_aFileInfo, sizeof(m_aFileInfo) / sizeof(m_aFileInfo[0]), (tmFileIDType)nType);
}

int		CtmMMCardRecordFile::GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID)
{
	int	nResult = 0;
	nResult = CtmHmiRecordFile::GetEverySize(pSource, nSourceNum, FileID);
	CtmFile	File;
	
	if (m_nIndexInFiles >= 0) 
	{
		switch(FileID)
		{
	    case ID_MACHINE_FILE:
	    case ID_MACHINE_MACH_FILE:
	    	nResult = g_pBlock->GetBlockSize(BLOCK_MACHINE_ID);
	    	pSource[m_nIndexInFiles].nSize = nResult;	
	    	pSource[m_nIndexInFiles].nTotalSize = nResult;
	    	g_tmDebugInfo->PrintDebugInfo("MachineSize=%d\n", nResult);
	        break;
	    case ID_MACHINEB_FILE:
	    case ID_MACHINEB_MACH_FILE:
	    	nResult = g_pBlock->GetBlockSize(BLOCK_MACHINEB_ID);
	    	pSource[m_nIndexInFiles].nSize = nResult;	
	    	pSource[m_nIndexInFiles].nTotalSize = nResult;
	    	g_tmDebugInfo->PrintDebugInfo("MachinebSize=%d\n", nResult);
	        break;
	    case ID_DATABLE_FILE:
	    case ID_DATABLE_MACH_FILE:
	    	nResult = g_pBlock->GetBlockSize(BLOCK_NEWPOWERMATCH_ID);
	    	pSource[m_nIndexInFiles].nSize = nResult;	
	    	pSource[m_nIndexInFiles].nTotalSize = nResult;
	    	g_tmDebugInfo->PrintDebugInfo("BLOCK_NEWPOWERMATCH_ID=%d\n", nResult);
	    	break;
		case ID_SYSTEMX_FILE:
			nResult = g_pBlock->GetBlockSize(BLOCK_SYSTEMX_ID);
			pSource[m_nIndexInFiles].nSize = nResult;	
	    	pSource[m_nIndexInFiles].nTotalSize = nResult;
			break;
		case ID_USERM_FILE:
			nResult = g_pBlock->GetBlockSize(BLOCK_USERM_ID);
			pSource[m_nIndexInFiles].nSize = nResult;	
	    	pSource[m_nIndexInFiles].nTotalSize = nResult;
			break;
	    default:
	        break;
		}
	}	
	return nResult;
}

int		CtmMMCardRecordFile::CalculateMachineSize()
{
	
}

BOOL 	CtmMMCardRecordFile::CheckData	(int nType)
{
	return TRUE;
}

/*==========================================================================+
|           Class implementation - CtmUsbRecordFile				            |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmUsbRecordFile::CtmUsbRecordFile(): CtmMMCardRecordFile()
{
	m_pStorage = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_USB);
}

CtmUsbRecordFile::CtmUsbRecordFile(CtmStorageFile* pStorage): CtmMMCardRecordFile(pStorage)
{
	
}

CtmUsbRecordFile::~CtmUsbRecordFile()
{
	
}

/*==========================================================================+
|           Class implementation - CtmFlashRecordFile				            |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmFlashRecordFile::CtmFlashRecordFile(CtmStorageFile* pStorage): CtmMMCardRecordFile(pStorage)
{
    InitData();
}

CtmFlashRecordFile::CtmFlashRecordFile()
{
	//printf("new CtmFlashRecordFile!!!\n");
    m_pStorage = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_FLASH);
	InitData();
}

CtmFlashRecordFile::~CtmFlashRecordFile()
{

}

int		CtmFlashRecordFile::GetSize(int nType)
{
	return GetEverySize(m_aFileInfo, sizeof(m_aFileInfo) / sizeof(m_aFileInfo[0]), (tmFileIDType)nType);
}


int		CtmFlashRecordFile::GetEverySize(tmFileRulerInfo* pSource, int nSourceNum, tmFileIDType FileID)
{
	int nResult = 0;
	nResult = CtmMMCardRecordFile::GetEverySize(pSource, nSourceNum, FileID);
		
	if (m_nIndexInFiles >= 0) 
	{
		switch(FileID)
		{
	    case ID_DATABLE_FILE:
	    case ID_ZEROSET_FILE:    
	            nResult = CtmBlockControl::GetInstance()->GetSize(FileID);
	        break;
	    default:
	        break;
		}
	}
		
	return nResult;
	
}
