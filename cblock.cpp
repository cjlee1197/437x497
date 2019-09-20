/*===========================================================================+
|  Class    : TaskComm                                                       |
|  Task     : Communication Service Routine                                  |
|----------------------------------------------------------------------------|
|  Compile  : GCC V3.3.2 -                                                   |
|  Link     : GCC V3.3.2 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Yang		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 03/24/2007                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"cblock.h"
#include	"files.h"
#include	"database.h"
#include	"tmdebug.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmBlock::CtmBlock()
{
	m_BlockBuff = NULL;
}

CtmBlock::~CtmBlock()
{
	if (m_BlockBuff != NULL)	free(m_BlockBuff);
}
/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmBlock::Create(char* pszFileName)
{
	CtmFile		File;
	DWORD		nFileSize;
	
	if(File.Open(pszFileName) != tmFILE_GOOD)	return FALSE;
	nFileSize	= File.Seek(0, SEEK_END);
	File.Seek(0, SEEK_SET);
	m_BlockBuff	= (char *)malloc(nFileSize);
	if (m_BlockBuff == NULL) 
		{
		File.Close();
		return FALSE;
		}
	File.Read(m_BlockBuff, nFileSize);
	File.Close();
	m_pBlockHeader = (BLOCKHEADER *)m_BlockBuff;
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BLOCKDATA*	CtmBlock::GetBlockData(WORD wBlockID)
{
	int		i;
	WORD	wTotal;
	BLOCKDATA*		pBlockData = NULL;
	BLOCKITEM		BlockItem;
	
	wTotal	= m_pBlockHeader->wTotal;
	for (i=0; i<wTotal; i++)
		{
		BlockItem = m_pBlockHeader->pBlockAttr[i];
	
		
		if (BlockItem.wIndex == wBlockID)
			{
			pBlockData = (BLOCKDATA *)(m_BlockBuff + BlockItem.dwOffset);
			break;
			}
		}
		
	
	return	pBlockData;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	CtmBlock::SetBlock(BYTE* pcData, WORD	wBlockID)
{
	WORD	wOffset = 0, wElement, wDBIndex;
	int		i;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
	
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)	return 0;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return 0;
	
	for (i=0; i<wElement; i++)
		{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		//if (10 == wBlockID)
		//{
		//	g_tmDebugInfo->LogDebugInfo("/mnt/nfs/Monitor.log",
		//								"StrID=%s, Index=%d, Value=%d, len=%d",
		//								g_pDatabase->GetString(wDBIndex),
		//								wDBIndex,
		//								dbValue.lData,
		//								dbValue.DataType.wLength
		//								);
		//}
	
		if (dbValue.DataType.wType != TYPE_STRING)
		{
			memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
		}
		else
			strncpy((char *)(pcData + wOffset), dbValue.pcData, dbValue.DataType.wLength);
		wOffset += dbValue.DataType.wLength;
		}
	return wOffset+sizeof(WORD);
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	CtmBlock::SetDefBlock(BYTE* pcData, WORD	wBlockID)
{
	WORD	wOffset = 0, wElement, wDBIndex;
	int		i;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
	
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)	return 0;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return 0;
	for (i=0; i<wElement; i++)
		{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->ReadDefault(wDBIndex);
		if (dbValue.DataType.wType != TYPE_STRING)
			memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
		else
			strncpy((char *)(pcData + wOffset), dbValue.pcData, dbValue.DataType.wLength);
		wOffset += dbValue.DataType.wLength;
		}
	return wOffset+sizeof(WORD);
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	CtmBlock::GetBlock(BYTE* pcData, WORD wBlockID, BOOL bSave)
{
	WORD	wOffset = 0, wElement, wDBIndex;
	int		i;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
	
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)	return 0;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return 0;
	for (i=0; i<wElement; i++)
	{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		
		if (dbValue.DataType.wType != TYPE_STRING)
		{
			memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
		}
		else
			strncpy(dbValue.pcData, (char *)(pcData + wOffset), dbValue.DataType.wLength);
		wOffset += dbValue.DataType.wLength;
		
		
		g_pDatabase->Write(&dbValue, bSave);
		}
	
	return wOffset+sizeof(WORD);
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmBlock::SetBlockValue(BYTE* pcData, WORD wBlockID, char* pszID, DWORD wValue)
{
	WORD	wOffset = 0, wElement, wDBIndex;
	int		i, nIndex;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
	
	if ((nIndex		= g_pDatabase->GetIndex(pszID)) < 0)return FALSE;
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)	return FALSE;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return FALSE;
	
	for (i=0; i<wElement; i++)
		{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		if (nIndex == wDBIndex)
			{
			switch(dbValue.DataType.wType)
				{
				case TYPE_BOOL:
					dbValue.wData = (WORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_BYTE:
					dbValue.ucData = (WORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_WORD:
					dbValue.wData = (WORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_DWORD:
					dbValue.dwData = wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_CHAR:
					dbValue.cData = (WORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_SHORT:
					dbValue.nData = (WORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_INT:
					dbValue.lData = (DWORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_FLOAT:
					dbValue.fData = (DWORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_DOUBLE:
					dbValue.dData = (DWORD)wValue;
					memcpy((pcData + wOffset), dbValue.acData, dbValue.DataType.wLength);
					break;
				case TYPE_STRING:
					
					break;
				case TYPE_UNKNOWN:
				default:
					break;
				}
			return	TRUE;
			}
		wOffset += dbValue.DataType.wLength;
		}
//	printf("Can not set ID=%s\n", pszID);
	return	FALSE;
}   	

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmBlock::GetBlockValue(BYTE* pcData, WORD	wBlockID, char* pszID, DWORD* pdwValue)
{
	WORD	wOffset = 0, wElement, wDBIndex;
	int		i, nIndex;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
	
	if ((nIndex		= g_pDatabase->GetIndex(pszID)) < 0)return FALSE;
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)	return FALSE;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return FALSE;
	
	for (i=0; i<wElement; i++)
		{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		if (nIndex == wDBIndex)
			{
			switch(dbValue.DataType.wType)
				{
				case TYPE_BOOL:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.wData;
					break;
				case TYPE_BYTE:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.ucData;
					break;
				case TYPE_WORD:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.wData;
					break;
				case TYPE_DWORD:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.dwData;
					break;
				case TYPE_CHAR:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.cData;
					break;
				case TYPE_SHORT:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.nData;
					break;
				case TYPE_INT:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = dbValue.lData;
					break;
				case TYPE_FLOAT:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = (DWORD)dbValue.fData;
					break;
				case TYPE_DOUBLE:
					memcpy(dbValue.acData, (pcData + wOffset), dbValue.DataType.wLength);
					*pdwValue = (DWORD)dbValue.dData;
					break;
				case TYPE_STRING:
					
					break;
				case TYPE_UNKNOWN:
				default:
					break;
				}
			return	TRUE;
			}
		wOffset += dbValue.DataType.wLength;
		}
//	printf("Can not find ID=%s\n", pszID);
	return	FALSE;
}

WORD	CtmBlock::GetBlockCount(WORD	wBlockID)
{
	//BLOCKDATA	BlockData;
	BLOCKDATA	*pBlockData;
	
	//return	BlockData.dwFactorTotal;
	
	pBlockData	= GetBlockData(wBlockID);
	if (pBlockData == NULL) return 0;
	else return pBlockData->dwFactorTotal;
}//zholy070629

DWORD	CtmBlock::GetBlockSize(WORD wBlockID)
{
	DWORD		dwSize = 0;
	WORD		wDBIndex = 0;
	WORD		wCount = GetBlockCount(wBlockID);
	BLOCKDATA*	pBlockData= GetBlockData(wBlockID);
	DBVALUE		dbValue;
	
	if (pBlockData == NULL) 
		return	dwSize;
	for (int i=0; i<wCount; i++)
	{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		dwSize += dbValue.DataType.wLength;
	}
	
	return	dwSize;
}


/*---------------------------------------------------------------------------+
|  Function : SearchIndexFromBlock(WORD	wBlockID, DWORD	dwIndex)             |
|  Task     : „G«ü©wªºBlock¤¤¬d§ä„±…@†V®Æ¬O§_¦s¦b           				 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :TRUE ¦³¡A  FLASE ÆÓ                  							 |
+---------------------------------------------------------------------------*/
long	CtmBlock::SearchIndexFromBlock(WORD	wBlockID, DWORD	dwIndex)
{
	char	cszDBID[256] = "\0";
	DWORD	dwOffset = 0;
	WORD	wElement = 0, wDBIndex = 0;
	int		i = 0;
	BLOCKDATA*		pBlockData = NULL;
	DBVALUE	dbValue;
	
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)
		return 0;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)
		return 0;
	for(i = 0; i < wElement; i++)
	{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		if(wDBIndex == dwIndex)
		{
			return dwOffset;
		}
		dwOffset += dbValue.DataType.wLength;
	}
	return -1;
}

/*---------------------------------------------------------------------------+
|  Function :   GetDataID(WORD	wBlockID, DWORD	wOffsetID)                   |
|  Task     : „G³q¹LoffsetID¦b«ü©wªºBlock¤¤¬d§äDataID          				 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :TRUE ¦³¡A  FLASE ÆÓ                  							 |
+---------------------------------------------------------------------------*/
WORD	CtmBlock::GetDataID(WORD	wBlockID, WORD	wOffsetID)
{
	BLOCKDATA*		pBlockData = NULL;
	WORD	wElement = 0, wDBIndex = 0, wCountNub = 0;
	DWORD	dwOffset = 0;
	DBVALUE	dbValue;
	
	if ((pBlockData	= GetBlockData(wBlockID)) == NULL)
		return -1;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)
		return -1;
	
	for(int i = 0; i < wElement; i++)
	{
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex);
		
		if((wOffsetID*sizeof(WORD)) == dwOffset)
		{
			return g_pDatabase->IndexToDataID(wDBIndex);
		}
		dwOffset += dbValue.DataType.wLength;
	}
	
	return -1;
}
