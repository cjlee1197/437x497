/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CRecordSet										             |
|  Task     : control the recordset that it's modified by user               |
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
#include	"recordset.h"
#include	"stable.h"
#include	"database.h"
#include	"machinefactor.h"
#include	"sram.h"
#include	"init.h" //Sunny<20070504>
#include	"font.h"
#include	"filerecord.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//=======
//      Initialize static class member
//=======	
int			g_RecordFlag = 0;											
CRecordSetControl*	CRecordSetControl::m_pInstance = NULL;	
CRecordSet*	CRecordSetControl::m_pCRecordSet = NULL;

/*==========================================================================+
|           Class implementation - COilRecordSet      	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
COilRecordSet::COilRecordSet()
{
	m_bSaved = false;
	m_bFileState = false;
	m_pszOutAction = NULL;
	m_pszOutAction = new char[512];
	m_nRecordSetNum	= 0;
	memset(&m_tmRecordSetCursor,0,sizeof(m_tmRecordSetCursor));
	OpenFile();
	GetMaxNum();
}

COilRecordSet::~COilRecordSet()
{
	if (m_pszOutAction != NULL) delete []m_pszOutAction;
	////m_cFile.Close();
}


/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  OpenFile()													|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/

void	COilRecordSet::OpenFile()
{
	//if (!m_bFileState)
	//{
	//	//m_cFile.Open(CtmConfig::GetInstance()->GetRecordSetFileName(), PerSize());
	//	m_bFileState = true;
	//}
	////m_cFile.Rewind();
}

/*--------------------------------------------------------------------------+
|  Function :  GetMaxNum()													|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		COilRecordSet::GetMaxNum()
{
	return m_nMaxNum = CtmConfig::GetInstance()->GetRecordSetMaxNum();
}

/*--------------------------------------------------------------------------+
|  Function :  SaveModifyRecord(void* pInData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	COilRecordSet::SaveModifyRecord(void* pInData)
{
	int	Result = 0;
	
	tmModifyRecordSet_Head	Head;
	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		memset(&Head, 0, sizeof(Head));
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_MODIFYLOG,0,&Head, sizeof(Head));
	
		if (Head.nCount < m_nMaxNum)
		{
			Head.nCount++;
		}
		if(Head.nIndex>=m_nMaxNum)
		{
			Head.nIndex=0;
		}
		Head.nIndex++;

		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MODIFYLOG, Head.nIndex , pInData, sizeof(tmModifyRecordSet_Oil)) > 0;
		
		SendMsg(MSG_RECORDSET_SAVE, Head.nCount, 0, NULL); 
		
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_MODIFYLOG, 0, &Head, sizeof(Head));
		//printf("Head.nIndex=%d,Head.nCount=%d\n",Head.nIndex,Head.nCount);
		m_bSaved = true;
	}
	else
	{
		if (!m_bSaved) m_nRecordSetNum = GetNum();
		m_pSramRecordFile->GetInstance()->ReadRecord(ID_RECORDSET_SRAMFILE,0,&m_tmRecordSetCursor);
		if (m_nRecordSetNum >= m_nMaxNum)
		{
			 m_nRecordSetNum=m_nMaxNum-1/*m_nRecordSetNum -= m_nMaxNum*/;
			 if(m_tmRecordSetCursor.nCursor>=m_nMaxNum)
			 {
				m_tmRecordSetCursor.nCursor=0;
			 }
			 m_tmRecordSetCursor.nCursor++;
		}
		OpenFile();
		
		m_nRecordSetNum++;
		BOOL	Result = FALSE;
		
		if(m_tmRecordSetCursor.nCursor!=0)
		{
			Result = m_pSramRecordFile->GetInstance()->WriteRecord(ID_RECORDSET_SRAMFILE,m_tmRecordSetCursor.nCursor /*m_nRecordSetNum*/, pInData, sizeof(tmModifyRecordSet_Oil), 0) > 0;
		}
		else
		{
			Result = m_pSramRecordFile->GetInstance()->WriteRecord(ID_RECORDSET_SRAMFILE,m_nRecordSetNum, pInData, sizeof(tmModifyRecordSet_Oil), 0) > 0;
		}
		if (Result)
			SendMsg(MSG_RECORDSET_SAVE, m_nRecordSetNum, 0, NULL); 
			
		if (!Result)
		{
			m_tmRecordSetCursor.nCursor--;
			m_nRecordSetNum--;
			if (m_nRecordSetNum < 0) m_nRecordSetNum = m_nMaxNum;
		}
		
		m_pSramRecordFile->GetInstance()->WriteRecord(ID_RECORDSET_SRAMFILE, 0, &m_tmRecordSetCursor, sizeof(tmModifyRecordSet_Oil));
		//printf("m_nRecordSetNum=%d,m_tmRecordSetCursor=%d\n",m_nRecordSetNum,m_tmRecordSetCursor.nCursor);
		m_bSaved = true;
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetModifyRecordS(int nIndex, void* pOutData)					|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	COilRecordSet::GetModifyRecordS(int nIndex, void* pOutData)
{
	BOOL	Result 	= FALSE;
	int		temp 	= 0;
	tmModifyRecordSet_Head	Head;
	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		memset(&Head, 0, sizeof(Head));
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_MODIFYLOG, 0, &Head, sizeof(Head));
	
		//if(Head.nIndex!=0)
		//	nIndex+=Head.nIndex;
		if(nIndex>m_nMaxNum)
		{
			nIndex-=m_nMaxNum;
		}
		Result = CManageFile::GetInstance()->ReadRecord(FILE_RECORD_MODIFYLOG, nIndex, pOutData, sizeof(m_tmRecordSetCursor)) > 0;
		
		if (((tmModifyRecordSet_Oil*)pOutData)->szID[0] == '\0') 
			Result = FALSE;
	}
	else
	{
		m_pSramRecordFile->GetInstance()->ReadRecord(ID_RECORDSET_SRAMFILE,0,&m_tmRecordSetCursor);
		//printf("Read m_tmRecordSetCursor=%d\n",m_tmRecordSetCursor.nCursor);
		if(m_tmRecordSetCursor.nCursor!=0)	nIndex+=m_tmRecordSetCursor.nCursor;
		if(nIndex>m_nMaxNum)
		{
			nIndex-=m_nMaxNum;
		}
		OpenFile();

		Result = m_pSramRecordFile->GetInstance()->ReadRecord(ID_RECORDSET_SRAMFILE, nIndex, pOutData) > 0;	
		if (((tmModifyRecordSet_Oil*)pOutData)->szID[0] == '\0') Result = FALSE;
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetModifyRecord(void* pOutData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		COilRecordSet::GetModifyRecord(void* pOutData)
{
	tmModifyRecordSet_Oil	Temp;
	memset(&Temp, 0, sizeof(Temp));
	int	Result = 1;
	OpenFile();
	for (int i = 1; i <= m_nMaxNum; i++)
	{
		if (GetModifyRecordS(i, &Temp))
		{
			memcpy((tmModifyRecordSet_Oil*)pOutData + Result - 1, &Temp, sizeof(Temp));
			Result++;
		}
		else break;
	}
	return	Result;
	
}


/*--------------------------------------------------------------------------+
|  Function :  QueryModifyRecord(int	nCondition, void*	pCondition, void* pOutData)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		COilRecordSet::QueryModifyRecord(int	nCondition, void*	pCondition, void* pOutData)
{
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  DeleteRecord(int nCondition, void* pCondition)				|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	COilRecordSet::DeleteRecord(int nCondition, void* pCondition)
{
	BOOL	Result = FALSE;
	switch(nCondition)
	{
		case Delete_ALL_CONDITION:
			////m_cFile.Close();
			//Result = remove(CtmConfig::GetInstance()->GetRecordSetFileName()) == 0;
			if(CtmConfig::GetInstance()->GetRecordType() == 1)
				Result = CManageFile::GetInstance()->DeleteRecord(FILE_RECORD_MODIFYLOG) > 0;
			else
				Result = m_pSramRecordFile->GetInstance()->DeleteRecord(ID_RECORDSET_SRAMFILE) > 0;
			if (Result) 
			{
				//printf("COilRecordSet::DeleteRecord recordnum=%d\n", m_nRecordSetNum);
				m_nRecordSetNum = 0;
				m_bFileState = false;
				
				SendMsg(MSG_RECORDSET_DEL, nCondition, 0, NULL); 
			}
			break;
	}
	
	return	Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetStr(DWORD nIndex)											|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	COilRecordSet::GetStr(DWORD nIndex)
{
	if (nIndex == 0xFFFFFFFF) return NULL;
	else return g_MultiLanguage[nIndex];
}

/*--------------------------------------------------------------------------+
|  Function :  GetAction(void* pData, int nNum)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	COilRecordSet::GetAction(void* pData, int nNum)
{
	DBRECORD Temp;
	Temp = g_pDatabase->GetDBRecord(((tmModifyRecordSet_Oil*)pData + nNum)->DBIndex);
	char*	Result = NULL;
	char* S1 = NULL, *S2 = NULL, *S3 = NULL, *S4 = NULL;
	//Sunny<20070504><<<<<
	char SS1[4], SS2[4], SS3[4], SS4[4];
	memset(SS1,0,4);
	memset(SS2,0,4);
	memset(SS3,0,4);
	memset(SS4,0,4);
	//strcpy(SS1, "");
	//strcpy(SS2, "");
	//strcpy(SS3, "");
	//strcpy(SS4, "");
	//Sunny<20070504>>>>>>
	S1 = GetStr(Temp.dwComponent);
	if (S1 == NULL) S1 = SS1;
	S2 = GetStr(Temp.dwMotion);
	if (S2 == NULL) S2 = SS2;
	S3 = GetStr(Temp.dwActPoint);
	if (S3 == NULL) S3 = SS3;
	S4 = GetStr(Temp.dwEffect);
	if (S4 == NULL) S4 = SS4;
	if(g_pFont->GetDefaultFont()->GetFontCode() == tmFT_CODE_TECH)	//Sunny<20070504>
	{
		WORD *pwDst,*pwSrc;
		int  j = 0;
		pwDst    = (WORD*)m_pszOutAction;	
		pwSrc 	 = (WORD *)S1;
		while(*pwSrc != 0 && j < 256)
		{
			*pwDst ++   = *pwSrc ++;
			j ++;
		}
		j = 0;
		*pwDst ++ =0x20;
		pwSrc =(WORD *)S2;
		while(*pwSrc != 0 && j < 256)
		{
			*pwDst ++   = *pwSrc ++;
			j ++;
		}
		j = 0;
		*pwDst ++ =0x20;
		pwSrc =(WORD *)S3;
		while(*pwSrc != 0 && j < 256)
		{
			*pwDst ++   = *pwSrc ++;
			j ++;
		}
		j = 0;
		*pwDst ++ =0x20;
		pwSrc =(WORD *)S4;
		while(*pwSrc != 0 && j < 256)
		{
			*pwDst ++   = *pwSrc ++;
			j ++;
		}
		*pwDst = 0;
		j = 0;	
	}
	else	sprintf(m_pszOutAction, "%s %s %s %s", S1, S2, S3, S4);
	
	Result = m_pszOutAction;
	return Result;
}


char*	COilRecordSet::GetAction_FT(void* pData, int nNum)
{
	char*	Result = NULL;
	long	lIndex = ((tmModifyRecordSet_Oil*)pData + nNum)->DBIndex;
	long	lDataID = 0;
	char	pszDataID[256];
	int		nLen = 0;
	lDataID = g_pDatabase->IndexToDataID(lIndex);
	if(lDataID<0)	lDataID=lIndex;
	memset(pszDataID, 0, sizeof(pszDataID));
	//printf("lIndex=%ld lDataID=%x \n", lIndex, lDataID);
	if(lDataID >=0)
	{
		sprintf(pszDataID, "%08x", lDataID);
		memset(m_pszOutAction, 0, sizeof(m_pszOutAction));
		nLen = strlen(pszDataID);
		for(int i = 0; i < nLen; i++)
			m_pszOutAction[i] = toupper(pszDataID[i]);
		m_pszOutAction[nLen] = '\0';
		//printf("pszDataID=%s m_pszOutAction=%s \n", pszDataID, m_pszOutAction);
		return m_pszOutAction;
	}
	else
	{
		return NULL;
	}
	
}

/*--------------------------------------------------------------------------+
|  Function :  GetNum()														|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		COilRecordSet::GetNum()
{
	int nResult = 0;
	tmModifyRecordSet_Head	Head;
	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		memset(&Head, 0, sizeof(Head));
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_MODIFYLOG, 0, &Head, sizeof(Head));
		nResult = Head.nCount;
	}
	else
	{
		if (m_bSaved)
			nResult = m_nRecordSetNum;
		else
		{
			char	szID[10];
			//m_cFile.Rewind();
			for (int i = 1; i <= m_nMaxNum; i++)
			{
				memset(szID, 0, sizeof(szID));
				if (m_pSramRecordFile->GetInstance()->ReadRecord(ID_RECORDSET_SRAMFILE, i, szID, sizeof(szID), 0) <= 0)
					break;
				//if (//m_cFile.ReadRecord(i, szID, sizeof(szID), 0) != tmFILE_GOOD) break;
				if (szID[0] != '\0')
				{
					//sprintf("GetNum the first id =%d\n", oilrecord.DBIndex);
					nResult++;
				}
			}
		}
	}
	//printf("recordNum = %d\n", nResult);
	return nResult;
}

/*==========================================================================+
|           Class implementation - CRecordSetControl  	                    |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CRecordSetControl::CRecordSetControl()
{
	m_pCRecordSet = CtmMachineBuild::GetInstance()->BuildCRecordSet();
		
}

CRecordSetControl::~CRecordSetControl()
{
	if (m_pCRecordSet != NULL)
	{
		delete m_pCRecordSet;
		m_pCRecordSet = NULL;
	}
}

CRecordSetControl*	CRecordSetControl::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CRecordSetControl;
	return	m_pInstance;
}

void		CRecordSetControl::ReleaseInstance()
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
|  Function :  SaveModifyRecord(void* pInData)								|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CRecordSetControl::SaveModifyRecord(void* pInData)
{
	return	m_pCRecordSet->SaveModifyRecord(pInData);
}

/*--------------------------------------------------------------------------+
|  Function :  GetModifyRecordS(int nIndex, char** pOutAction, void* pOutData)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CRecordSetControl::GetModifyRecordS(int nIndex, char** pOutAction, void* pOutData)
{
	BOOL	Result = FALSE;
	if (nIndex <= CtmConfig::GetInstance()->GetRecordSetMaxNum())
	{
		Result = m_pCRecordSet->GetModifyRecordS(nIndex, pOutData);
		if (Result) *pOutAction = m_pCRecordSet->GetAction(pOutData);
	}
	return Result;
}

BOOL	CRecordSetControl::GetModifyRecordS_FT(int nIndex, char** pOutAction, void* pOutData)
{
	BOOL	Result = FALSE;
	char*	pszValue = NULL;
	if (nIndex <= CtmConfig::GetInstance()->GetRecordSetMaxNum() && nIndex >=0)
	{
		Result = m_pCRecordSet->GetModifyRecordS(nIndex, pOutData);
		if (Result)
		{
			memset(m_pszValue, 0, sizeof(m_pszValue));
			*pOutAction  = m_pCRecordSet->GetAction_FT(pOutData);
			//printf("copy 1 \n");
			//if(pszValue != NULL)
			//	*pOutAction = m_pszValue;
			//	;//memcpy(*pOutAction, pszValue, strlen(pszValue));
			//printf("copy 2 \n");
		}
	}
	return Result;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMoidfyRecord(char**	pOutAction, void* pOutData)			|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CRecordSetControl::GetMoidfyRecord(char**	pOutAction, void* pOutData)
{
	int nResult = 0;
	nResult = m_pCRecordSet->GetModifyRecord(pOutData);
	for (int i = 0; i < nResult; i++)
	{
		*(pOutAction + i) = m_pCRecordSet->GetAction(pOutData, i);
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  QueryModifyRecord(int	nCondition, void*	pCondition, char** pOutAction, void* pOutData)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CRecordSetControl::QueryModifyRecord(int	nCondition, void*	pCondition, char** pOutAction, void* pOutData)
{
	int	 nResult = 0;
	nResult = m_pCRecordSet->QueryModifyRecord(nCondition, pCondition, pOutData);
	for (int i = 0; i < nResult; i++)
		*(pOutAction + i) = m_pCRecordSet->GetAction(pOutData, i);
		
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  DeleteRecord(int nCondition, void* pCondition)				|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CRecordSetControl::DeleteRecord(int nCondition, void* pCondition)
{
	return m_pCRecordSet->DeleteRecord(nCondition, pCondition);
}

/*--------------------------------------------------------------------------+
|  Function :  GetMaxNum()													|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CRecordSetControl::GetMaxNum()
{
	return	m_pCRecordSet->GetMaxNum();
}

/*--------------------------------------------------------------------------+
|  Function :  GetNum()														|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CRecordSetControl::GetNum()
{
	return m_pCRecordSet->GetNum();
}
