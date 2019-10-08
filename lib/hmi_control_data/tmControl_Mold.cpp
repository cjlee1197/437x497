/*===========================================================================+
|  Class    : CtmControl_Mold	                                        	 |
|  Task     : Mold Service Function	    	                     			 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008  				                                     |
+===========================================================================*/
#include	"tmControl_Mold.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"
#include	"../../moldControl.h"
#include	"../../commonaction.h"
#include	"../../tmconfig.h"
#include	"../../readdata.h"
#include 	"../../tmshmsg.h"
#include	"../../iotable.h"


IMPLEMENT_DYNCREATE(CtmControl_Mold, CtmControl)

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
#define		DATA_MIN_SIZE 			4
#define		CURRENT_MOLD_NAME		"MHDR_MODULE_NULL_NULL_MODULENAME"

extern	BOOL	g_biNetStatus;

int		n7LUMoldIDValue[80];
static	char*	g_apsz7LUMoldID[] = {
							"MSET_IJU_IJU_NULL_W071F",
							"MSET_OTHR_OTHR_NULL_W0860",
	            "MSET_OTHR_OTHR_NULL_W087C",
	            "MSET_OTHR_OTHR_NULL_W087D",
	            "MSET_OTHR_OTHR_NULL_W085D",
	            "MSET_OTHR_OTHR_NULL_W085C",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE74",
	            "MSET_MLD_OPEN_DRAIN_PRESSURE",
	            "MSET_MLD_OPEN_DRAIN_FLOW",
	            "MSET_OTHR_OTHR_NULL_W0834",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE67",
	            "MSET_SCRW_INJ_NULL_BEFOREINJUFWDTIME",
	            "MSET_TMP1_HEATERON_NULL_W07EB",
	            "MSET_TMP1_HEATERON_NULL_W07EC",
	            "MSET_TMP1_HEATERON_NULL_W07ED",
	            "MSET_TMP1_HEATERON_NULL_W07EE",
	            "MSET_TMP1_HEATERON_NULL_W07EF",
	            "MSET_TMP1_HEATERON_NULL_W07F0",
	            "MSET_TMP1_HEATERON_NULL_W07F1",
	            "MSET_TMP1_HEATERON_NULL_W07F2",
	            "MSET_TMP1_HEATERON_NULL_W07F3",
	            "MSET_MDH_MDH_FAST_PRESSURE",
	            "MSET_MDH_MDH_FAST_FLOW",
	            "MSET_MDH_MDH_SLOW_FLOW",
	            "MSET_MDH_FWD_NULL_COUNTER",
	            "MSET_MDH_MDH_NULL_POSITION",
	            "MSET_OTHR_OTHR_NULL_W087B",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE77",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE78",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE79",
	            "MSET_ALARM_ON_NULL_HEATEROFFOPTION",
	            "MSET_OTHR_OTHR_NULL_W0835",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE51",//FUNC 1
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE52",
	            "MSET_OTHR_OTHR_NULL_W085A",
	            "MSET_OTHR_OTHR_NULL_W085B",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE57",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE60",
	            "MSET_OTHR_OTHR_NULL_W0863",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE62",
	            "MSET_IJU_IJU_NULL_W0720",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE54",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE72",//FUNC 2
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE58",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE59",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE73",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE61",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE55",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE76",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE56",
	            "MSET_OTHR_OTHR_NULL_W087A",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE53",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE69",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE70",
	            "MSET_OTHR_OTHR_NULL_OTHERMACHINE71",
	            "MSET_GATE_GATE_NULL_W0631",		//CLMPBUFFER
	            "MSET_GATE_GATE_NULL_W062D",
	            "MSET_GATE_GATE_NULL_W062F",
	            "MSET_GATE_GATE_NULL_W0634",
	            "MSET_GATE_GATE_NULL_W062E",
	            "MSET_GATE_GATE_NULL_W0630",
	            "MSET_GATE_GATE_NULL_W0635",
	            "MSET_GATE_GATE_NULL_W062B",
	            "MSET_GATE_GATE_NULL_W0629",
	            "MSET_GATE_GATE_NULL_W0632",
	            "MSET_GATE_GATE_NULL_W0637",
	            "MSET_GATE_GATE_NULL_W062C",
	            "MSET_GATE_GATE_NULL_W062A",
	            "MSET_GATE_GATE_NULL_W0633",
	            "MSET_GATE_GATE_NULL_W0638",
	            "MSET_GATE_GATE_NULL_W063D",
	            "MSET_GATE_GATE_NULL_W063B",
	            "MSET_GATE_GATE_NULL_W063E",
	            "MSET_GATE_GATE_NULL_W063C",
          		NULL
            };
/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/
tmBlockInfo CtmControl_Mold::m_aBlockInfo[] =
{
	{BLOCK_ZSET_ID				,ID_ZEROSET_FILE},
	{BLOCK_NEWPOWERMATCH_ID		,ID_DATABLE_FILE},
	{BLOCK_MACHINE_ID			,ID_MACHINE_FILE},     //Sunny<20070911><<<<<
	{BLOCK_MACHINEB_ID			,ID_MACHINEB_FILE},
	{BLOCK_NEWPOWERMATCH_MACH_ID,ID_DATABLE_MACH_FILE},
	{BLOCK_MACHINE_MACH_ID		,ID_MACHINE_MACH_FILE},
	{BLOCK_MACHINEB_MACH_ID		,ID_MACHINEB_MACH_FILE},
	{BLOCK_SYSTEMX_ID			,ID_SYSTEMX_FILE}, 
	{BLOCK_USERM_ID				,ID_USERM_FILE}
	//{BLOCK_MACHINE_ID,      ID_VERSION_FILE},
	//{BLOCK_MACHINE_ID,      ID_DEFAULTVALUE_FILE},
	//{BLOCK_MACHINE_ID,      ID_MINVALUE_FILE},
	//{BLOCK_MACHINE_ID,      ID_MAXVALUE_FILE}
	
};
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/	
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::CtmControl_Mold()				             |
|  Task     :  CtmControl_Mold構造函數                                       |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Mold::CtmControl_Mold(): CtmControl()
{
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::~CtmControl_Mold()				             |
|  Task     :  CtmControl_Mold析構函數                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Mold::~CtmControl_Mold()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::CreateSelf()
{
	//InitControl();
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::FreeSelf()				             	     |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::FreeSelf()
{
	if (m_pContent != NULL)
	{
		delete []m_pContent;
		m_pContent = NULL;
	}
	if (m_pHeads != NULL)
	{
		delete []m_pHeads;
		m_pHeads = NULL;
	}
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::AddControl(void *pParas[])  	     			 |
|  Task     :  Save Error Record to sram and show promp	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   WORD   pParas[0]        - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::AddControl(void *pParas[])
{
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::ModifyControl(void *pParas[])  	     		 |
|  Task     :  Modify Error Message 					                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   WORD   pParas[0]        - 		                  			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::ModifyControl(void *pParas[])
{
	return FALSE;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::FindControl(void *pParas[], void *pFindPara) |
|  Task     :  Search Error Message 					                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   void *   pFindPara      - 			               			 |
|               void *pParas[]          - 存儲返回的警報訊息                 |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::FindControl(void *pParas[], void *pFindPara)
{
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::DeleteControl(void *pDeletePara)             |
|  Task     :  Clear All Error Message 					                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   void *pDeletePara       - 		                   			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::DeleteControl(void *pDeletePara)
{
	
	return TRUE;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::FixControl(void *pFixPara)		             |
|  Task     :  Fix Error Message 					                     	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   void *pFixPara          -                        			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::FixControl(void *pFixPara)
{
	                                                                                                              
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::InitControl()					             |
|  Task     :  Init Error Message 					                     	 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_Mold::InitControl()
{
	
    m_nBlockIndex = -1;
    m_nDataType = ID_STORAGE_NOFILE;
	
	m_pContent = NULL;
	m_pHeads = NULL;
	char*	pszTemp = NULL;
	m_nCurrentIndex = g_pDatabase->GetIndex(CURRENT_MOLD_NAME);
	pszTemp = g_pDatabase->Reads(m_nCurrentIndex);
	
	if (pszTemp != NULL)
	{
		strncpy(m_szCurrentMoldName, pszTemp, g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength);//sizeof(m_szCurrentMoldName));
	}
	else
	{
		strcpy(m_szCurrentMoldName, "Default");
	}
	m_nMoldSetSize = CalculateSize();	//	need to calculate size from yang's block to calculate
		
	m_nMaxMoldSet = CtmConfig::GetInstance()->GetMoldSetMaxNum();
    if (m_nMaxMoldSet > 0) 
    {
        m_pHeads = new DB_MOLDHDR[m_nMaxMoldSet];
        m_pContent = new BYTE[m_nMoldSetSize];
        memset(m_pHeads, 0, sizeof(DB_MOLDHDR) * m_nMaxMoldSet);
        memset(m_pContent, 0, sizeof(m_nMoldSetSize));
    }
}

/*---------------------------------------------------------------------------+
|  Function :CtmControl_Mold::Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)|
|  Task     :讀取指定的模具資料                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                                                                |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::Read(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)
{
    BOOL	bResult = FALSE;
    bResult = (MoldSetHead.acID[0] != '\0');

    if (bResult) 
    {
        int nRecord = 0;
        nRecord = Find(MoldSetHead, StorageType);
        if (nRecord > 0) 
        {
            bResult = Read(nRecord, StorageType);
        }
        else bResult = FALSE;
    }

    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function :CtmControl_Mold::Read(int nIndex, tmStorageType StorageType)    |
|  Task     :讀取指定的模具資料                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                                                                |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::Read(int nIndex, tmStorageType StorageType)
{
    DB_MOLDHDR14	TempHead14;
    memset(&TempHead14, 0, sizeof(TempHead14));		//fuxy, 2012-2-22, add 初始化資料 
    BOOL    bResult = FALSE;
    int     nCount = 0;
    nCount = m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, nIndex, m_pContent, StorageType);
    bResult = (nCount > 0);
    if (bResult) 
    {
       	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
       	{
        	memcpy(&m_Head, m_pContent, sizeof(m_Head));
       	}
        else
        {
        	memcpy(&TempHead14, m_pContent, sizeof(TempHead14));
       		MoldName14TO40(&TempHead14, &m_Head);
        }
        
/*wangli2016-3-9 14:26:41  mark，修正 向HMI存儲模具后，導致當前工藝的inet上下限功能失效的問題*/
//        if(g_biNetStatus)
//        {
//        	memcpy(g_MoldHead.acID, m_Head.acID, sizeof(m_Head.acID));	//2013-1-15  iNet  讀取的模具   
//    		memcpy(g_MoldHead.szMaterial, m_Head.szMaterial, sizeof(m_Head.szMaterial));
//    		memcpy(g_MoldHead.szColor, m_Head.szColor,sizeof(m_Head.szColor));
//    		SendMsg(MSG_SH_REV_INET_MOLD_LIMIT, 0, 0, NULL);	//2013-1-15  申請模具上下限 
//    		g_biNetLimit  = FALSE;
//    	}
    	
        if (m_Head.acID[0] == '\0') 
        {
            bResult = FALSE;
        }
    }
    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)|
|  Task     :保存特定的模具資料                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                                                                |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::Save(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)
{
	BOOL	bResult = FALSE;
    bResult = (MoldSetHead.acID[0] != '\0');
	DB_MOLDHDR14	TempHead14;
	memset(&TempHead14, 0, sizeof(TempHead14));		//fuxy, 2012-2-22, add 初始化資料 
	if (bResult)
	{
		int       nRecord = Find(MoldSetHead, StorageType);
		if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
       	{
			memcpy(m_pContent, &MoldSetHead, sizeof(DB_MOLDHDR));
		}
		else 
		{
			MoldName40TO14(&MoldSetHead, &TempHead14);
			memcpy(m_pContent, &TempHead14, sizeof(TempHead14));
		}
	
		if (nRecord > 0)		//	exist, and override
		{
			//	fans modiy 2009-4-15 9:49:10
			SendMsg(MSG_MOLDSET_OVERWRITE, nRecord, StorageType, NULL);
			//if (g_bFindMoldSetMsg)
			//	MsgBox(g_MultiLanguage["VM_MLD_OVERWRITE"], tmFT_CODE_TECH);
			if(g_bMsgBoxFlag)		//James add 2008/7/22 獲得msgbox的值，決定是否覆蓋模組
				bResult = Save(nRecord, StorageType);
		}
		else 					//	append
		{
			nRecord = FindFree(StorageType);
			SendMsg(MSG_MOLDSET_ADD, nRecord, StorageType, NULL);	//	fans add 2009-4-15 9:49:56
			if (nRecord > 0)
			{
				bResult = Save(nRecord, StorageType);
			}
			else bResult = FALSE;
		}
	}
	
	return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Save(int nIndex,tmStorageType StorageType)    |
|  Task     :保存特定的模具資料                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                                                                |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::Save(int nIndex,tmStorageType StorageType)
{
	BOOL	bResult = FALSE;
	int nCount = m_pMoldSetFile->GetInstance()->WriteRecord(ID_MOLDSET_SRAMFILE, nIndex,
                                               m_pContent,StorageType);
    bResult = (nCount > 0);
    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)|
|  Task     :刪除特定的模具資料                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                                                                |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::Delete(DB_MOLDHDR MoldSetHead, tmStorageType StorageType)
{
    BOOL    bResult = FALSE;
    if (MoldSetHead.acID[0] != '\0') 
    {
        bResult = TRUE;
    }

    if (bResult) 
    {
        bResult = !(strcmp(MoldSetHead.acID, m_szCurrentMoldName) == 0);
    }
    
	int nRecord = Find(MoldSetHead, StorageType);
    if (bResult) 
    {
        if (nRecord > 0)
        {
            Delete(nRecord, StorageType);
        }
    }
    else 
    {
    	#ifdef	D_PXA270
    	if( StorageType != STORAGE_TYPE_USB )
    	#else
    	if( StorageType != STORAGE_TYPE_MMCARD )
    	#endif
    	{
    		//	fans modify 2009-4-15 9:51:58
    		SendMsg(MSG_MOLDSET_DEL_CUR, 0, StorageType, NULL);
        	//MsgBox(g_MultiLanguage["VM_MOLD_DELETE_CURRENT"], tmFT_CODE_TECH);
        }
        else		//James add 解決不能刪除MMC卡里面和面板同名的模具 2007/12/11 03:00下午
        {
        	if (nRecord > 0)
	        {
	            Delete(nRecord, StorageType);
	        }	
        }
    }

    return bResult;


}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetCurrentMold()                              |
|  Task     : 獲取當前的模具名稱                                             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   : 當前的模具名稱                                                 |
+---------------------------------------------------------------------------*/
char*	CtmControl_Mold::GetCurrentMold()
{
	char*	pszTemp = NULL;
	m_nCurrentIndex = g_pDatabase->GetIndex(CURRENT_MOLD_NAME);
	pszTemp = g_pDatabase->Reads(m_nCurrentIndex);
	//printf("%s dataIndex = %d\n", CURRENT_MOLD_NAME, m_nCurrentIndex);
	
	if (pszTemp != NULL)
	{
		strncpy(m_szCurrentMoldName, pszTemp, g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength);
	}
	else
	{
		strcpy(m_szCurrentMoldName, "Default");
	}
	
	return m_szCurrentMoldName;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Delete(int nIndex,tmStorageType StorageType)  |
|  Task     : 刪除特定的模具資料    其本質是將m_Head中的名稱設置為NULL       |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   : 是否已經成功刪除:TRUE/FALSE                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::Delete(int nIndex,tmStorageType StorageType)
{
    BOOL	bResult = FALSE;
    DB_MOLDHDR  TempHead;
	DB_MOLDHDR14  TempHead14;
    memset(&TempHead, 0, sizeof(TempHead));
    memset(&TempHead14, 0, sizeof(TempHead14));
    
	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
	{
    	bResult = m_pMoldSetFile->GetInstance()->WriteRecord(ID_MOLDSET_SRAMFILE, nIndex, &TempHead, 
				sizeof(TempHead), StorageType) > 0;
	}
	else 
	{
		bResult = m_pMoldSetFile->GetInstance()->WriteRecord(ID_MOLDSET_SRAMFILE, nIndex, &TempHead14, 
				sizeof(TempHead14), StorageType) > 0;	
	}
    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::FindFree(tmStorageType StorageType)           |
|  Task     : 查找文件中的空餘的位置,來決定保存資料在文件中的位置            |
|             位置序號,如果是0的話表明沒有找到    		                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   : 位置序號,如果是0的話表明沒有找到                               |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::FindFree(tmStorageType StorageType)
{
	int nResult = 0;
    DB_MOLDHDR  TempHead;
	DB_MOLDHDR14  TempHead14;
	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
	{
		do
		{
	        memset(&TempHead, 0, sizeof(TempHead));
			nResult++;
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, nResult, 
	                                                      &TempHead, sizeof(TempHead),StorageType) <= 0)
				break;
		}while (TempHead.acID[0] != '\0' && nResult <= m_nMaxMoldSet);
		
		if (nResult > m_nMaxMoldSet && TempHead.acID[0] != '\0') nResult = 0;
	}
	else 
	{
		do
		{
	        memset(&TempHead14, 0, sizeof(TempHead14));
			nResult++;
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, nResult, 
	                                                      &TempHead14, sizeof(TempHead14),StorageType) <= 0)
				break;
		}while (TempHead14.acID[0] != '\0' && nResult <= m_nMaxMoldSet);
		
		if (nResult > m_nMaxMoldSet && TempHead14.acID[0] != '\0') nResult = 0;	
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Find(DB_MOLDHDR MoldSetHead, tmStorageType	StorageType)|
|  Task     : 查找特定的模具資料         			                         |
|             資料的序號,如果是0的話表明沒有找到		                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::Find(DB_MOLDHDR MoldSetHead, tmStorageType	StorageType)
{
	int	nResult = 0;
	DB_MOLDHDR	TempHead;
	DB_MOLDHDR14	TempHead14;
	
	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
	{
		for (int i = 0; i <  m_nMaxMoldSet; i++)
		{
			memset(&TempHead, 0, sizeof(TempHead));
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, i + 1, &TempHead, 
						sizeof(TempHead), StorageType) <= 0)
			{
				break;
			}
			
			if ((strcmp(TempHead.acID, MoldSetHead.acID) == 0))
			{
				nResult = i + 1;
				break;
			}
		}	
	}
	else 
	{
		for (int i = 0; i <  m_nMaxMoldSet; i++)
		{
			memset(&TempHead14, 0, sizeof(TempHead14));
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, i + 1, &TempHead14, 
						sizeof(TempHead14), StorageType) <= 0)
			{
				break;
			}
			MoldName14TO40(&TempHead14, &TempHead);
			if ((strcmp(TempHead.acID, MoldSetHead.acID) == 0))
			{
				nResult = i + 1;
				break;
			}
		}
	}
	
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetMoldSetHead()                              |
|  Task     : 獲得頭         			                                     |
|             無論是保存還是獨取,之後的動作都需要使用該函數來獲取頭信息		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
DB_MOLDHDR*	CtmControl_Mold::GetMoldSetHead()
{
	return	&m_Head;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetMoldSetHead(int nStart, int Num, tmStorageType StorageType)|
|  Task     : 獲得頭信息數組         			                             |
|             並不能保證在得到的所有信息不為空,原因是有可能並沒有保存        |
| nStart + Num個資料 ， 這樣我們將會得到後面的信息全部為空                   |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
DB_MOLDHDR*	CtmControl_Mold::GetMoldSetHead(int nStart, int Num, tmStorageType StorageType)
{
	int	nSize = sizeof(DB_MOLDHDR);
	memset(m_pHeads, 0, nSize * m_nMaxMoldSet);
	
	if (Num > m_nMaxMoldSet) Num = m_nMaxMoldSet;
	if (nStart <= 0) nStart = 1;
	
	int	j = 0;
	
	DB_MOLDHDR14	TempHead14;
	
	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
	{
		for (int i = nStart; i <= m_nMaxMoldSet; i++)
		{
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, i, m_pHeads + j, nSize, 0,
					StorageType) <= 0) 
			{
				(m_pHeads + j)->acID[0] = '\0';
				break;
			}
			
			if ((m_pHeads + j)->acID[0] != '\0')
				j++;
			
			if (j >= Num)	break;
		}
	}
	else 
	{
		for (int i = nStart; i <= m_nMaxMoldSet; i++)
		{
			memset(&TempHead14, 0, sizeof(TempHead14));
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, i, &TempHead14, sizeof(TempHead14), 0,
					StorageType) <= 0) 
			{
				TempHead14.acID[0] = '\0';
				break;
			}
			MoldName14TO40(&TempHead14, m_pHeads + j);
			
			if ((m_pHeads + j)->acID[0] != '\0')
				j++;
			
			if (j >= Num)	break;
		}	
	}
	return	m_pHeads;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetMoldSetContent()                           |
|  Task     : 獲得頭信息         				                             |
|             無論是保存還是獨取,之後的動作都需要使用該函數來獲取內容信息    |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BYTE*	CtmControl_Mold::GetMoldSetContent()
{
	return	m_pContent;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetMaxMoldSetNum()                            |
|  Task     : 獲取允許保存慔具的最大數目         				             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::GetMaxMoldSetNum()
{
	return	m_nMaxMoldSet;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetMoldSetNum(tmStorageType	StorageType)     |
|  Task     : 獲取現在保存慔具數目              				             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::GetMoldSetNum(tmStorageType	StorageType)
{
	int	nResult = 0;
    DB_MOLDHDR  TempHead;
    DB_MOLDHDR14	TempHead14;
	
	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
	{
		for (int i = 1; i <= m_nMaxMoldSet; i++)
		{
	        memset(&TempHead, 0, sizeof(TempHead));
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, i, &TempHead, 
	                                                      sizeof(TempHead), StorageType) <= 0) 
	        	break;
			if (TempHead.acID[0] != '\0') 
			{
				nResult++;
			}
		}
	}
	else 
	{
		for (int i = 1; i <= m_nMaxMoldSet; i++)
		{
	        memset(&TempHead14, 0, sizeof(TempHead14));
			if (m_pMoldSetFile->GetInstance()->ReadRecord(ID_MOLDSET_SRAMFILE, i, &TempHead14, 
	                                                      sizeof(TempHead14), StorageType) <= 0) 
	            break;
			if (TempHead14.acID[0] != '\0') 
			{
				nResult++;
			}
		}	
	}
	return	nResult;
}


/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType )  |
|  Task     : 發送當前模組資料到主機              				             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL CtmControl_Mold::SaveToCurrentMold(DB_MOLDHDR MoldSetHead, tmStorageType StorageType ) 
{
    BOOL    bResult = FALSE;
    if(g_pExTable->GetTableValue(0,"D_7LUNEW"))
    {
    	for (int i=0; g_apsz7LUMoldID[i]!=NULL; i++)
		{
			n7LUMoldIDValue[i] = GetDBValue(g_apsz7LUMoldID[i]).lValue;
		}
	}
	
	
    bResult = Read(MoldSetHead, StorageType);
    if (bResult) 
    {

		if(CtmConfig::GetInstance()->GetINETFlag())
		{
	    	if(g_biNetStatus)/*wangli2016-3-29 18:42:04  代碼從 Read(int nIndex, tmStorageType StorageType) 移到這裡，修正 向HMI存儲模具后，導致當前工藝的inet上下限功能失效的問題*/
	        {
	        	memcpy(g_MoldHead.acID, m_Head.acID, sizeof(m_Head.acID));	//2013-1-15  iNet  讀取的模具   
	    		memcpy(g_MoldHead.szMaterial, m_Head.szMaterial, sizeof(m_Head.szMaterial));
	    		memcpy(g_MoldHead.szColor, m_Head.szColor,sizeof(m_Head.szColor));
	    		SendMsg(MSG_SH_REV_INET_MOLD_LIMIT, 0, 0, NULL);	//2013-1-15  申請模具上下限 
	    		g_biNetLimit  = FALSE;	//無模具上下限 
	    	}
	    	else
				g_biNetLimit  = FALSE;	 /*wangli2016-3-21 16:16:05 當前為有inet上下限的模具，當網絡斷了之後，讀取本地的模具后，之前的inet上下限仍然存在的問題*/
	    }
        g_pBlock->GetBlock(m_pContent, BLOCK_MOLDSETX_ID);   //  From BlockBuffer to Database

        if(g_pExTable->GetTableValue(0,"D_7LUNEW"))
      	{
      		for (int i=0; g_apsz7LUMoldID[i]!=NULL; i++)
					{
							SetDBValue(g_apsz7LUMoldID[i],n7LUMoldIDValue[i], FALSE); 			
					}
      	}

        if(g_ptaskdsp != NULL)	
        	{
        		g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSET); //if(g_ptaskrs232 != NULL)	g_ptaskrs232->SendControlMoldset();		
        		if (CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC)
        			g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSETB);
        	}		
        if(g_ptaskCom2dsp != NULL) g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLMOLDSET);//Com2 machine must send moldset
    }
    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SaveToCurrentMold()   						 |
|  Task     : 保存當前模組              				                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL CtmControl_Mold::SaveToCurrentMold() 
{
	BOOL    bResult = FALSE;
    DB_MOLDHDR14	TempHead14;
	memset(&TempHead14, 0, sizeof(TempHead14));		//fuxy, 2012-2-22, add 初始化資料 
	
	g_pBlock->SetBlock(m_pContent, BLOCK_MOLDSETX_ID);  //  From Database To BlockBuffer
    
	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
	{
		memcpy(&m_Head, m_pContent,  sizeof(m_Head));
	}
	else 
	{
		memcpy(&TempHead14, m_pContent,  sizeof(TempHead14));	
		MoldName14TO40(&TempHead14, &m_Head);
	}
    memcpy(m_szCurrentMoldName, m_Head.acID, sizeof(m_szCurrentMoldName));
    
    #ifdef	D_NOWRITE_FALSH
    	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
    	if(GetFileType(CHECKFILETYPE))
    		nStorageType = STORAGE_TYPE_FLASH;
    	bResult = Save(m_Head, nStorageType);
    #else
    	#ifdef D_X86
    	bResult = Save(m_Head, STORAGE_TYPE_FLASH);
    	#else
    	bResult = Save(m_Head, STORAGE_TYPE_SRAM);
    	#endif
    #endif
    
    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetMoldSetHeadAsynch() 						 |
|  Task     : 獲得模組資料的頭          				                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
DB_MOLDHDR* CtmControl_Mold::GetMoldSetHeadAsynch() 
{
    return  m_pHeads;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType)|
|  Task     : 保存當前模組資料到指定媒介				                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	CtmControl_Mold::SaveCurrentDB(DB_MOLDHDR MoldSetHead,tmStorageType StorageType)
{
	BOOL    bResult = FALSE;
	DB_MOLDHDR14	TempHead14;
	memset(&TempHead14, 0, sizeof(TempHead14));		//fuxy, 2012-2-22, add 初始化資料 
	g_pBlock->SetBlock(m_pContent, BLOCK_MOLDSETX_ID); 			//  From Database To BlockBuffer

	if(g_pDatabase->Read(CURRENT_MOLD_NAME).DataType.wLength == MOLDNAME_LENGTH)
   	{
    	memcpy(m_pContent, &MoldSetHead, sizeof(MoldSetHead) - 4);	//	4 byte for mold unit product and media
		memcpy(&m_Head, m_pContent, sizeof(MoldSetHead));
		memcpy(&m_szCurrentMoldName, m_Head.acID, sizeof(m_szCurrentMoldName));
   	}
    else
    {
    	//MoldName40TO14(&MoldSetHead,&TempHead14);
    	//memcpy(m_pContent, &TempHead14, sizeof(TempHead14) - 4);
    	memcpy(&m_Head, &MoldSetHead, sizeof(MoldSetHead));
    	m_Head.cUnitCount = *(WORD*)((BYTE*)m_pContent+ sizeof(TempHead14) - 4);
    	m_Head.Media = *(WORD*)((BYTE*)m_pContent+ sizeof(TempHead14) - 2);
		memcpy(&m_szCurrentMoldName, m_Head.acID, sizeof(m_szCurrentMoldName));
    }
    
	bResult = Save(MoldSetHead, StorageType);
	
	char*	pszTemp = NULL;
	m_nCurrentIndex = g_pDatabase->GetIndex(CURRENT_MOLD_NAME);
	pszTemp = g_pDatabase->Reads(m_nCurrentIndex);
	return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SaveAs(tmStorageType StorageType)             |
|  Task     : 將模組資料保存到指定的媒介				                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL 	CtmControl_Mold::SaveAs(tmStorageType StorageType)
{
    BOOL bResult = FALSE;
    if (m_nBlockIndex >= 0)
    {
        int nSize = 0;
        nSize = g_pBlock->GetBlockSize(m_nBlockIndex);
        if (nSize > 0)
        {
            BYTE*   pTemp = NULL;
            pTemp = new BYTE[nSize];
            g_pBlock->SetBlock(pTemp, m_nBlockIndex);
            g_tmDebugInfo->PrintDebugInfo("blockINdex =%d, m_nDataType=%d, size=%d\n", 
            							m_nBlockIndex,m_nDataType, nSize);
            bResult = CtmSarmRecordFileControl::GetInstance()->WriteRecord(m_nDataType, 1, 
                                                                           pTemp, StorageType) == nSize;
            delete []pTemp;
        }
    }

    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SaveAs(tmStorageType SourceStorage, tmStorageType DestStorage)|
|  Task     : 將模組資料保存從指定的媒介保存到另外一個媒介				     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL 	CtmControl_Mold::SaveAs(tmStorageType SourceStorage, tmStorageType DestStorage)
{
    BOOL bResult = FALSE;
    if (m_nBlockIndex >= 0)
    {
        int nSize = 0;
        nSize = g_pBlock->GetBlockSize(m_nBlockIndex);
        if (nSize > 0)
        {
            BYTE*   pTemp = NULL;
            pTemp = new BYTE[nSize];
            bResult = CtmSarmRecordFileControl::GetInstance()->ReadRecord(m_nDataType, 1, 
                                                                          pTemp, SourceStorage)  == nSize;
            if (bResult)
                bResult = CtmSarmRecordFileControl::GetInstance()->WriteRecord(m_nDataType, 1, 
                                                                          pTemp, DestStorage)  == nSize;
            delete []pTemp;
        }
    }

    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Read(tmStorageType StorageType)               |
|  Task     : 讀取指定媒介的模組資料				                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL 	CtmControl_Mold::Read(tmStorageType StorageType)
{
    BOOL bResult = FALSE;
    if (m_nBlockIndex >= 0)
    {
        int nSize = 0;
        nSize = g_pBlock->GetBlockSize(m_nBlockIndex);
        if (nSize > 0)
        {
            BYTE*   pTemp = NULL;
            pTemp = new BYTE[nSize];
            bResult = CtmSarmRecordFileControl::GetInstance()->ReadRecord(m_nDataType, 1, 
                                                                          pTemp, StorageType)  == nSize;
            if (bResult) bResult = g_pBlock->GetBlock(pTemp, m_nBlockIndex) == nSize;
            delete []pTemp;
        }
    }

    return bResult;
   
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::Delete(tmStorageType StorageType)             |
|  Task     : 刪除指定媒介的模組資料				                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL 	CtmControl_Mold::Delete(tmStorageType StorageType)
{
    BOOL bResult = FALSE;
    if (m_nBlockIndex >= 0)
    {
        bResult = CtmSarmRecordFileControl::GetInstance()->DeleteRecord(m_nDataType, StorageType) == 0;
    }
    return bResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::FindBlockInfoNo(int nBlockIndex)              |
|  Task     : 通過Block Index 查找Block 對于的相關ID           				 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	 	CtmControl_Mold::FindBlockInfoNo(int nBlockIndex)
{
	int nResult = -1;
	for (unsigned int i = 0; i < sizeof(m_aBlockInfo) / sizeof(m_aBlockInfo[0]); i++)
	{
		if (m_aBlockInfo[i].nBlockIndex == nBlockIndex)
		{
			nResult = i;
			break;					
		}
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::FindBlockInfoNo(tmFileIDType FileID)          |
|  Task     : 通過FileIDType結構，查找Block 對應的相關ID           			 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	 	CtmControl_Mold::FindBlockInfoNo(tmFileIDType FileID)
{
	int nResult = -1;
	for (unsigned int i = 0; i < sizeof(m_aBlockInfo) / sizeof(m_aBlockInfo[0]); i++)
	{
		if (m_aBlockInfo[i].FileID == FileID)
		{
			nResult = i;
			break;					
		}
	}
	return nResult;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SetBlockIndex(int nBlockIndex)                |
|  Task     : 設置當前Block的Index           			                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void 	CtmControl_Mold::SetBlockIndex(int nBlockIndex)
{
	m_nDataType = ID_STORAGE_NOFILE;
    m_nBlockIndex = -1;
    int nIndex = FindBlockInfoNo(nBlockIndex);
    if (nIndex >= 0)
    {
    	m_nDataType = m_aBlockInfo[nIndex].FileID;
    	if( m_aBlockInfo[nIndex].nBlockIndex > 100000)
    		m_nBlockIndex = m_aBlockInfo[nIndex].nBlockIndex - 100000;
    	else
    		m_nBlockIndex = m_aBlockInfo[nIndex].nBlockIndex;
    }
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetSize(int nBlockIndex)                      |
|  Task     : 根據Block Index 獲得Block的大小                			     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	 	CtmControl_Mold::GetSize(int nBlockIndex)
{
	int	nResult = 0;
	//int nIndex = FindBlockInfoNo(nBlockIndex);
	nResult = g_pBlock->GetBlockSize(m_nBlockIndex);
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::GetSize(int nBlockIndex)                      |
|  Task     : 根據FileIDType 獲得Block的大小                			     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	 	CtmControl_Mold::GetSize(tmFileIDType FileID)
{
	int	nResult = 0;
	int nIndex = FindBlockInfoNo(FileID);
	nResult = g_pBlock->GetBlockSize(nIndex);
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Mold::SendControlMoldSet()   					    |
|  Task     : 發送模組資料              				                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   			            - 	                     		    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Mold::SendControlMoldSet()
{
	char szTemp[64];
	char* pszSendFileName = new char[64];
	
	char*  szSendList[200];
	CIniFile			SendN2CIni;
	CtmReadData			ReadData;
	DWORD				dwSendCount = 0;
	
	for (int i = 0; i < 200; i++)
	{
		szSendList[i] = new char[128];
		memset(szSendList[i], 128, 0);
	}
	
	if(SendN2CIni.OpenFile("outerconfig.ini"))
	{	
		for(int i = 1; i <= GetDBValue("AUSTON_PARAMETER_N2C_DRIVER_NUM").lValue; i++)
		{
			sprintf(szTemp , "MoldSendFile%d", i);
			SendN2CIni.ReadString("SendDsp28", szTemp, NULL, &pszSendFileName);
			
			if(pszSendFileName[0] != '\0')	ReadData.OpenFileStr(pszSendFileName);
			dwSendCount += ReadData.ReadData(&szSendList[dwSendCount]);
			
		}
		sprintf(szTemp , "MoldSendFile%d", 9);
		SendN2CIni.ReadString("SendDsp28", szTemp, NULL, &pszSendFileName);
		
		if(pszSendFileName[0] != '\0')	ReadData.OpenFileStr(pszSendFileName);
		
		dwSendCount += ReadData.ReadData(&szSendList[dwSendCount]);
		
		g_ptaskdsp28_N2C->WriteValue(9, dwSendCount, szSendList,NULL);
	}
	
	delete [] pszSendFileName;
	for (int i = 0; i < 200; i++)
	{
		delete [] szSendList[i];
	}
	return  dwSendCount;
	
}

 /*! \fn int		CtmControl_Mold::CalculateSize()
 *  \brief 計block提供的大小,
 *  \return 存儲每一筆模具的大小
 */
int		CtmControl_Mold::CalculateSize()
{
    return g_pBlock->GetBlockSize(BLOCK_MOLDSETX_ID);
}

int		CtmControl_Mold::GetMoldSize()
{
	return m_nMoldSetSize;
}

void		CtmControl_Mold::MoldName14TO40(DB_MOLDHDR14* p_MoldHead14, DB_MOLDHDR* p_MoldHead40)
{
	memcpy(p_MoldHead40->acID, p_MoldHead14->acID, sizeof(p_MoldHead40->acID));
	memcpy(&(p_MoldHead40->DateSave), &(p_MoldHead14->DateSave), sizeof(p_MoldHead40->DateSave));
   	memcpy(p_MoldHead40->szMaterial, p_MoldHead14->szMaterial, sizeof(p_MoldHead14->szMaterial));
	memcpy(p_MoldHead40->szColor,  p_MoldHead14->szColor, sizeof(p_MoldHead14->szColor));
	p_MoldHead40->cUnitCount = p_MoldHead14->cUnitCount;			
    p_MoldHead40->Media		 = p_MoldHead14->Media;		
}
void		CtmControl_Mold::MoldName40TO14(DB_MOLDHDR* p_MoldHead40, DB_MOLDHDR14* p_MoldHead14)
{
	memcpy(p_MoldHead14->acID, p_MoldHead40->acID, sizeof(p_MoldHead14->acID));
	memcpy(&(p_MoldHead14->DateSave), &(p_MoldHead40->DateSave), sizeof(p_MoldHead40->DateSave));
   	memcpy(p_MoldHead14->szMaterial, p_MoldHead40->szMaterial, sizeof(p_MoldHead14->szMaterial));
	memcpy(p_MoldHead14->szColor,  p_MoldHead40->szColor, sizeof(p_MoldHead14->szColor));
	p_MoldHead14->cUnitCount = p_MoldHead40->cUnitCount;			
    p_MoldHead14->Media		 = p_MoldHead40->Media;	
}

#ifndef	D_BEOBJECT_CTMCONTROL_MOLD
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
	{
		(static_cast<CtmPackClass*>(pResult))->CreateSelf();
	}
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmPackClass*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}

#endif
