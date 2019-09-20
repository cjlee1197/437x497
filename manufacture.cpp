/*===========================================================================+
|  Function : iNet                                                           |
|  Task     : Produce manage service routine file                            |
|----------------------------------------------------------------------------|
|  Compile  : 			                                                     |
|  Link     : 			                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : zholy                               		                     |
|  Version  : V1.00                                                          |
|  Creation : 03/04/1997                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	"manufacture.h"
#include	"utils.h"
#include	"storage.h"
#include	"tmshmsg.h"
#include	"moldControl.h"
#include 	"commonaction.h"


#define     SEC_                            60
#define     MIN_                            (60*SEC_)
#define     HOU_                            (60*MIN_)
#define     DAY_                            (24*HOU_)
#define     MON_                            (30*DAY_)
#define     YEA_                            (12*MON_)

extern 	DB_PRODMANAGE	g_ProManage;
extern 	USERPRODUCTS	g_UserProductInfo;

CtmManufacturePlan*  CtmManufacturePlan::m_pInstance	= NULL;

char g_szPlanScheduleNumber0[18];
char g_szPlanScheduleNumber1[18];

/*==========================================================================+
|           Class implementation - CtmManufacturePlan                       |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmManufacturePlan::CtmManufacturePlan(): CTask()
{
	m_nPlanSize		= CalculatePlanSize();
	m_nMaxPlanCount	= CONST_MAX_PLANCOUNT;
	m_bSortFlag = TRUE;
	memset(&m_manufactureplancur, 0, sizeof(DB_PRODMANAGE));
	memset(&m_manufactureplanHeadInfo, 0, sizeof(DB_PRODMANAGE));
	memset(&m_manufactureplanTemp, 0, sizeof(DB_PRODMANAGE));
	
	memset(g_szPlanScheduleNumber0, 0, sizeof(g_szPlanScheduleNumber0));
    memset(g_szPlanScheduleNumber1, 0, sizeof(g_szPlanScheduleNumber1));
	
	RegisterMessage(MSG_DSP54_GET_OPMODE, m_idMe);
	RegisterMessage(MSG_DSP54_GET_SHOTCOUNT, m_idMe);
	RegisterMessage(MSG_SH_MANUFACTURE_SCHEDULE_STATUS, m_idMe);
	RegisterMessage(MSG_SH_MANUFACTURE_SHOTCOUNT, m_idMe);
	
}

CtmManufacturePlan::~CtmManufacturePlan()
{
	UnRegisterMessage(MSG_DSP54_GET_OPMODE, m_idMe);
	UnRegisterMessage(MSG_DSP54_GET_SHOTCOUNT, m_idMe);
	UnRegisterMessage(MSG_SH_MANUFACTURE_SCHEDULE_STATUS, m_idMe);	
	UnRegisterMessage(MSG_SH_MANUFACTURE_SHOTCOUNT, m_idMe);
	
}

CtmManufacturePlan*	CtmManufacturePlan::GetInstance()
{
	if (m_pInstance == NULL) m_pInstance = new CtmManufacturePlan();
	
	return m_pInstance;
}

void	CtmManufacturePlan::ReleaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance	= NULL;
	}
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  Save()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CtmManufacturePlan::Save(DB_PRODMANAGE *pdbplan, tmStorageType StorageType)
{
	int	nRecord;
	
	if (pdbplan == NULL) return FALSE;
	
	nRecord = Find(pdbplan->acPlanScheduleNumber, StorageType);
	if (nRecord == -1)
	{
		nRecord = FindFree();
		if (nRecord == -1) 
			return FALSE;
		
		if (Save(pdbplan, nRecord, StorageType))
		{
			DB_PRODMANAGE *pdbplaninfo	= (DB_PRODMANAGE *)GetPlanInfo();
			if (pdbplaninfo == NULL) return FALSE;
				
			pdbplaninfo->wCount++;
			SavePlanInfo();
			RecordSort();
			return TRUE;
		}
		else return FALSE;	
	}
	else return Save(pdbplan, nRecord, StorageType);
	
	
}

/*--------------------------------------------------------------------------+
|  Function :  ResetPlanStorage()											|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CtmManufacturePlan::ResetPlanStorage()
{
	m_pPlanFile->GetInstance()->EraseRecord(ID_MANUFACTURE_SRAMFILE, STORAGE_TYPE_SRAM);
	return TRUE;
}

/*--------------------------------------------------------------------------+
|  Function :  Find()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmManufacturePlan::Find(char* pszNum, tmStorageType StorageType)
{
	char	acNum[18];
	int		i	= 1;
	
	while (i <= m_nMaxPlanCount)
	{
		memset(acNum, 0, sizeof(acNum));
		m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, i, acNum, 
												sizeof(acNum), STORAGE_TYPE_SRAM);
		if (strcmp(pszNum, acNum) == 0) return i;
		i++;
	}
	return -1;
}

/*--------------------------------------------------------------------------+
|  Function :  Read()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CtmManufacturePlan::Read(char* pszNum, tmStorageType StorageType)
{
	int		nRecord	= 0;
	
	nRecord	= Find(pszNum, StorageType);
	
	if ((pszNum == NULL) || (nRecord == -1)) return FALSE;
	
	return Read(nRecord);
}

/*--------------------------------------------------------------------------+
|  Function :  Delete()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CtmManufacturePlan::Delete(char* pszNum, tmStorageType StorageType)
{
	int		nRecord;
	
	nRecord = Find(pszNum, StorageType);
	if ((pszNum == NULL) || (nRecord == -1)) return FALSE;
	
	return Delete(nRecord);
}

/*--------------------------------------------------------------------------+
|  Function :  GetCurrentPlan()												|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmManufacturePlan::GetCurrentPlan()
{
	if (GetPlanInfo() == NULL) return NULL;
	
	if (m_manufactureplanHeadInfo.nCurrentIndex == 0) return NULL;
	
	if (Read(m_manufactureplanHeadInfo.nCurrentIndex))
	{
		memcpy(&m_manufactureplancur, &m_manufactureplanTemp, sizeof(DB_PRODMANAGE));
		return (char *)&m_manufactureplancur;
	}
	else return NULL;

}

BOOL	CtmManufacturePlan::SaveCurrentPlan()
{	
	if (GetPlanInfo() == NULL || m_manufactureplanHeadInfo.nCurrentIndex == 0) return FALSE;

	return (Save(&m_manufactureplancur, m_manufactureplanHeadInfo.nCurrentIndex, STORAGE_TYPE_SRAM));
}

BOOL	CtmManufacturePlan::SetCurrentPlan(char* pszNum, tmStorageType	StorageType)
{
	int				nRecord;
	DB_PRODMANAGE	*pdbplaninfo;
	
	if (pszNum == NULL) nRecord	= 0;
	else nRecord	= Find(pszNum, StorageType);
	if (nRecord == -1) return FALSE;
	else
	{
		pdbplaninfo	= (DB_PRODMANAGE *)GetPlanInfo();
		if (pdbplaninfo == NULL) return FALSE;
		
		pdbplaninfo->nCurrentIndex	= nRecord;
		return SavePlanInfo();
	}
}

/*--------------------------------------------------------------------------+
|  Function :  ReadPlan()													|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmManufacturePlan::ReadPlan(int nIndex, tmStorageType StorageType)
{	
	nIndex = GetRealIndex(nIndex);
	if (nIndex < 0)	return NULL;
	
	if (Read(nIndex, StorageType)) return (char *)&m_manufactureplanTemp;
	else return NULL;
}

char*	CtmManufacturePlan::ReadPlan(char* pszNum, tmStorageType StorageType)
{
	if (Read(pszNum, StorageType)) return (char *)&m_manufactureplanTemp;
	else return NULL;
}

/*--------------------------------------------------------------------------+
|  Function :  GetPlanInfo()												|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmManufacturePlan::GetPlanInfo()
{
	static	BOOL	bGet = FALSE;
	if (!Read(0, STORAGE_TYPE_SRAM)) 
		return NULL;
	else
	{
		if (!bGet)
		{
			char acNum[18];
			WORD i = 1;
			WORD wTemp = 0;
			while (i <= m_nMaxPlanCount)
			{
				memset(acNum, 0, sizeof(acNum));
				m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, i, acNum, 
				      sizeof(acNum), STORAGE_TYPE_SRAM);  
				if (acNum[0] != '\0')
				wTemp++;
				i++;
			}
			memcpy(&m_manufactureplanHeadInfo, &m_manufactureplanTemp, sizeof(m_manufactureplanTemp));
			m_manufactureplanHeadInfo.wCount = wTemp;
			
			bGet = TRUE;
		}
			
		return (char *)&m_manufactureplanHeadInfo;
	}
}

BOOL	CtmManufacturePlan::SavePlanInfo()
{
	return (Save(&m_manufactureplanHeadInfo, 0, STORAGE_TYPE_SRAM));
}

/*--------------------------------------------------------------------------+
|  Function :  GetMaxNum()													|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmManufacturePlan::GetMaxNum()
{
	return m_nMaxPlanCount;
}

int		CtmManufacturePlan::GetPlanSize()
{
	return m_nPlanSize;
}

/*--------------------------------------------------------------------------+
|  Function :  GetPlanTotalSize()											|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmManufacturePlan::GetPlanTotalSize()
{
	return m_nPlanSize*m_nMaxPlanCount;
}

/*--------------------------------------------------------------------------+
|  Function :  CalculatePlanSize()											|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmManufacturePlan::CalculatePlanSize()
{
	return sizeof(DB_PRODMANAGE);
}

//===========================================================================

/*--------------------------------------------------------------------------+
|  Function :  Save()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CtmManufacturePlan::Save(DB_PRODMANAGE* pdbplan, int nIndex, tmStorageType StorageType)
{

	if (pdbplan == NULL || nIndex < 0) return FALSE;
	int	nTemp = 0;
	nTemp = m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex, 
														pdbplan, STORAGE_TYPE_SRAM);
	
	if(nIndex > 0) RecordSort();
	if (nTemp > 0) SendMsg(MSG_SH_MANUFACTURE_ACTION_SAVE, nIndex, 0, NULL);
	
	return (nTemp > 0);
}

/*--------------------------------------------------------------------------+
|  Function :  FindFree()													|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
// result ---> index (include head information)
int		CtmManufacturePlan::FindFree()
{
	int		i;
	char	acNum[18];	
	
	for (i = 1;i <= m_nMaxPlanCount;i++)
	{

		if (m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, i, acNum, 
													sizeof(acNum), STORAGE_TYPE_SRAM) <= 0)
		continue;
		if (acNum[0] == '\0') break;
	}
	if (i > m_nMaxPlanCount) return -1;
	else return i;
}

/*--------------------------------------------------------------------------+
|  Function :  Read()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL		CtmManufacturePlan::Read(int nIndex, tmStorageType StorageType)
{
	int		nTemp	= -1;
	
	if (nIndex < 0) return FALSE;
	nTemp = m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, 
													nIndex, &m_manufactureplanTemp, 
													STORAGE_TYPE_SRAM);
	return (nTemp > 0);
	
}

/*--------------------------------------------------------------------------+
|  Function :  Delete()														|
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL	CtmManufacturePlan::Delete(int nIndex)
{
	DB_PRODMANAGE		dbplanTemp;
	if (nIndex < 0) return FALSE;

	if (m_pPlanFile->GetInstance()->DeleteRecord(ID_MANUFACTURE_SRAMFILE, 
													nIndex, STORAGE_TYPE_SRAM) <= 0)
		return FALSE;
	else
	{
		SendMsg(MSG_SH_MANUFACTURE_ACTION_DEL, nIndex, 0, NULL);
		DB_PRODMANAGE *pdbplaninfo	= (DB_PRODMANAGE *)GetPlanInfo();
		if (pdbplaninfo == NULL) return FALSE;
		
		while(nIndex < pdbplaninfo->wCount)
		{
			if (m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, 
													nIndex + 1, &dbplanTemp, 
													STORAGE_TYPE_SRAM) > 0)	
			{
				m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex, 
														&dbplanTemp, STORAGE_TYPE_SRAM);	
			}
			else return FALSE;
			
			nIndex++;
		}
		
		if (m_pPlanFile->GetInstance()->DeleteRecord(ID_MANUFACTURE_SRAMFILE, 
													nIndex, STORAGE_TYPE_SRAM) <= 0)
		return FALSE;
		
		if (pdbplaninfo->wCount > 0)
			pdbplaninfo->wCount--;
		//Caozz mark 2013/4/8 下午 04:23:36
		//SetCurrentPlan(NULL);
		//SetScheduleStatus(0, 0);	//	produce in the no plan state
		SavePlanInfo();
		return TRUE;
	}
}


void	CtmManufacturePlan::SetProduceStatus(WORD wMachineMode)
{
	static	WORD	wOldMode = 0xFFFF;
	static	WORD	wOldMotor= 0;
	
	if (wMachineMode == wOldMode && wOldMotor == g_flMotorStatus)  return;		
	wOldMode	= wMachineMode;
	wOldMotor	= g_flMotorStatus;
	
	int nIndex = _ScanBitR(wMachineMode);
	DB_PRODMANAGE *pdbplaninfo = (DB_PRODMANAGE *)GetPlanInfo();
	
	switch (nIndex)
	{
		case -1:	//	hand
			if(g_flMotorStatus == 0xFFFF && pdbplaninfo != NULL)
			{
				pdbplaninfo->wPlanProduceStatus = 12;						//	待機	
			}
			else if (pdbplaninfo != NULL)
			{
				pdbplaninfo->wPlanProduceStatus = MANUFACTSTAT_HALTMACHINE;	//	停機
			}
			else return;
			break;
		case 0:		//	自動
		case 1:		//	半自動
		case 2:		//	電眼自動
		case 3:		//	時間自動
			if (pdbplaninfo != NULL && pdbplaninfo->nCurrentIndex == 0)	
			{
				pdbplaninfo->wPlanProduceStatus = MANUFACTSTAT_NOBILL;
			}
			else if (pdbplaninfo != NULL && pdbplaninfo->nCurrentIndex != 0)
			{
				pdbplaninfo->wPlanProduceStatus = MANUFACTSTAT_PRODUCT;
			}
			else return;
			break;
		case 4:		//	粗調模
		case 11:	//	自動調模
			if (pdbplaninfo != NULL)
			{
				pdbplaninfo->wPlanProduceStatus = MANUFACTSTAT_ADJUSTMOLD;
			}
			else return;
			break;
		default:
			return;
	}
	SendMsg(MSG_SH_INET_MANU_PRODUCE_STATUS, pdbplaninfo->wPlanProduceStatus, 0, NULL);
}

void	CtmManufacturePlan::SetScheduleStatus(int nIndex, int nStatus)
{
	tmDATE	dtDate;
    tmTIME	tmTime;
    GetDate(&dtDate);
    GetTime(&tmTime);
   
    //	change the plan info (In fact, it's the first)
    DB_PRODMANAGE *pdbplaninfo	= (DB_PRODMANAGE *)GetPlanInfo();
	if (nIndex == 0)	//	the current plan
	{
		DB_PRODMANAGE *pdbplan	= (DB_PRODMANAGE *)GetCurrentPlan();
		if (pdbplan != NULL)	//	now, there is a current plan
		{
			//	change the current plan 
			//if ((pdbplan->dwGoodProductCount + pdbplan->dwBadProductCount) > 0)
    		//{
    		//	pdbplan->wPlanScheduleStatus= BILLSTAT_PAUSE;
    		//	pdbplan->dtLastPauseDate	= tmDATE2DATE(dtDate);
    		//	pdbplan->tmLastPauseTime	= tmTime;
    		//}
    		//else pdbplan->wPlanScheduleStatus = BILLSTAT_WAIT;
    			
    		//pdbplan->wPlanScheduleStatus = BILLSTAT_WAIT;  Caozz Mark 2011/8/1 下午 04:38:22
    		pdbplan->wPlanScheduleStatus = BILLSTAT_PAUSE;	//Caozz add 2011/8/1 下午 04:38:39
    		strcpy(g_szPlanScheduleNumber0, pdbplan->acPlanScheduleNumber);
    		SaveCurrentPlan();
    		
    		
    		//	save the plan info, cacle the current plan; in fact, write the currentindex of the head
    		if (pdbplaninfo != NULL)
    		{
    			pdbplaninfo->dwSortNo++;
    			//printf("pdbplaninfo->dwSortNo = %d\n", pdbplaninfo->dwSortNo);
    			if(g_flMotorStatus == 0xFFFF)
    				pdbplaninfo->wPlanProduceStatus = 12;
    			else
    				pdbplaninfo->wPlanProduceStatus = MANUFACTSTAT_HALTMACHINE;
    				
    			SendMsg(MSG_SH_INET_MANU_PRODUCE_STATUS, pdbplaninfo->wPlanProduceStatus, 0, NULL);
    			
	    		//SendMsg(MSG_SH_INET_MANU_SCHEDULE_STATUS, pdbplaninfo->nCurrentIndex, pdbplan->wPlanScheduleStatus, NULL);
    			SendMsg(MSG_SH_INET_MANU_SCHEDULE_STATUS, (WPARAM)g_szPlanScheduleNumber0, pdbplan->wPlanScheduleStatus, NULL);
    			SetCurrentPlan(NULL);
    			
    			g_UserProductInfo.dwUserBadCount 	= 0;		//Caozz 2010/9/16 12:00下午
				g_UserProductInfo.dwUserProdCount 	= 0; 		//清為無單時，本班不良品和良品數清0
	    		SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT", 0);	//計劃總數清零
	    		SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", 0);		//包裝數清零
    			SetDBValueM("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", 0);
	    		SetDBValueM("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", 0);
	    		pdbplaninfo->dwGoodProductCount = 0;
    			pdbplaninfo->dwBadProductCount	= 0;
    		}
    		else return;			
		}
		else					
		{
			if (pdbplaninfo != NULL)
    		{
    			if(g_flMotorStatus == 0xFFFF)
    				pdbplaninfo->wPlanProduceStatus = 12;
    			else
    				pdbplaninfo->wPlanProduceStatus = MANUFACTSTAT_HALTMACHINE;
    			SavePlanInfo();
    			SendMsg(MSG_SH_INET_MANU_PRODUCE_STATUS, pdbplaninfo->wPlanProduceStatus, 0, NULL);
    		}
			
		}
	}
	else
	{
		if (pdbplaninfo->wCount == 0) return;
		else
		{
			DB_PRODMANAGE 	plan;
			DB_PRODMANAGE*	pdbplan = NULL;
			
			if (m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, 
													nIndex, &m_manufactureplanTemp, 
													STORAGE_TYPE_SRAM) > 0)
				pdbplan = &m_manufactureplanTemp;
				
			if (pdbplan != NULL)
			{
				switch(nStatus)
	    		{
	    		    case BILLSTAT_PRODUCTING:
	                    pdbplan->dtFactBeginDate	= tmDATE2DATE(dtDate);
	    			    pdbplan->tmFactBeginTime	= tmTime;
	    			    break;
	    			case BILLSTAT_FINISHED:
	    			    pdbplan->dtFactEndDate		= tmDATE2DATE(dtDate);
	    			    pdbplan->tmFactEndTime		= tmTime;
	    			    break;
	    			default: 
	    				break;
	    		}
	    		pdbplan->wPlanScheduleStatus = nStatus;
	    		memcpy(&plan, pdbplan, sizeof(DB_PRODMANAGE));
	    		//nIndex = GetRealIndex(nIndex);
	    		strcpy(g_szPlanScheduleNumber1, plan.acPlanScheduleNumber);
	    		m_bSortFlag = FALSE;
	    		if (Save(pdbplan, nIndex, STORAGE_TYPE_SRAM))
	    		{
	    			//	當前的計劃單就是修改的計劃單
	    			if (nIndex == pdbplaninfo->nCurrentIndex)
	    				SendMsg(MSG_SH_INET_MANU_SCHEDULE_STATUS, (WPARAM)g_szPlanScheduleNumber1, nStatus, NULL);
	    			else
	    			{
	    				if (nStatus == BILLSTAT_PRODUCTING)
	    				{
	    					m_bSortFlag = TRUE;
	    					
	    					SetScheduleStatus(0, BILLSTAT_WAIT);
	    					//pdbplaninfo->nCurrentIndex = nIndex;
	    					if(pdbplaninfo->nCurrentIndex == 0)
	    						RecordSort();
	    					pdbplaninfo->nCurrentIndex = 1;
	    					//memcpy(&m_manufactureplancur, &plan, sizeof(DB_PRODMANAGE));
	    					//SaveCurrentPlan();	
	    					//	must save the plan info that is a head of the all plans
	    					SavePlanInfo();
	    					
	    					//	read the moldset to the memory
	    					DB_MOLDHDR tempMoldHead;
	    					char	szCurrentMold[128];
	    					sprintf(szCurrentMold, "%s",CtmMoldSet::GetInstance()->GetCurrentMold());
	    					memcpy(tempMoldHead.acID, plan.acMoldID,sizeof(tempMoldHead.acID));
	    					if(strcmp(szCurrentMold, tempMoldHead.acID) == 0)
	    					{
	    						MsgBox(g_MultiLanguage["MSG_MOLD_COVEREDMOLD"], tmFT_CODE_TECH);
	    						if(g_bMsgBoxFlag)	
	    						{
	    							CtmMoldSet::GetInstance()->SaveToCurrentMold(tempMoldHead,STORAGE_TYPE_FLASH);
	    							SendMsg(MSG_SH_INET_DEAL_MOLD,0, 0, NULL);
	    						}
	    					}
	    					else 
	    					{
	    						CtmMoldSet::GetInstance()->SaveToCurrentMold(tempMoldHead,STORAGE_TYPE_FLASH);
	    						SendMsg(MSG_SH_INET_DEAL_MOLD,0, 0, NULL);	
	    					}
	    					//	clear the real shot count, tell the system the event happened
	    					SetDBValueM("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", 0);
	    					SetDBValueM("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", 0);
	    					//	set unit count to database
	    					SetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT", plan.wUnitCount);	    					
	    					
	    					//	set shot count to database with the good products num
	    					//Caozz Mark  //計劃總數為iNet傳來的值
	    					//SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT", 
	    					//	long(ceil(double(plan.dwPlanProduceCount - plan.dwGoodProductCount) / 
	    					//			  double(plan.wUnitCount))));
	    					SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT", plan.dwPlanProduceCount);
	    					g_UserProductInfo.dwUserBadCount 	= 0;		//Caozz 2010/8/18 02:59下午
							g_UserProductInfo.dwUserProdCount 	= 0; 		//讀取計劃單後，本班不良品和良品數清0
	    				}
	    				
	    				SendMsg(MSG_SH_INET_MANU_PRODUCE_STATUS, pdbplaninfo->wPlanProduceStatus, 0, NULL);
	    				//SendMsg(MSG_SH_INET_MANU_SCHEDULE_STATUS, nIndex, nStatus, NULL);
	    				SendMsg(MSG_SH_INET_MANU_SCHEDULE_STATUS, (WPARAM)g_szPlanScheduleNumber1, nStatus, NULL);
	    				
	    				//	memcpy(g_MoldHead.acID,tempMoldHead.acID, sizeof(tempMoldHead.acID));
	    			}
	    		}
	    		m_bSortFlag = TRUE;
			}
			
		}
	}
	
}

void	CtmManufacturePlan::SetProductCount(int nBadProduct)
{
	DB_PRODMANAGE 		*pdbplaninfo = (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetPlanInfo();
	DB_PRODMANAGE 		*pdbplan	 = (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetCurrentPlan();
	static	BOOL		 bClear 		 = FALSE;
	
	if (nBadProduct > g_UserProductInfo.dwUserProdCount) 
			nBadProduct = g_UserProductInfo.dwUserProdCount;
			
	g_UserProductInfo.dwUserBadCount += nBadProduct;
	if (nBadProduct > 0)	
		g_UserProductInfo.dwUserProdCount -= nBadProduct;
	else					
		g_UserProductInfo.dwUserProdCount += GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
	if ((pdbplan != NULL && pdbplaninfo != NULL )&&(pdbplaninfo->nCurrentIndex != 0))
	{
		if(pdbplan->wPlanScheduleStatus	 == BILLSTAT_FINISHED)	return;
			
		if (nBadProduct > pdbplan->dwGoodProductCount) 
			nBadProduct = pdbplan->dwGoodProductCount;
		
		pdbplan->dwBadProductCount += nBadProduct;
		
		
		//====================================================================================//
		// calculate the good product count & shotcount
		//====================================================================================//
		
		//	re-get the unitcount &  re-calculate the shotcount
		pdbplan->wUnitCount = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
		//long	lRealShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
		if (nBadProduct > 0)	// the message from the viewplan, in the view the message only send the badproduct num	
			pdbplan->dwGoodProductCount -= nBadProduct;
		else					// the message from the dsp(taskdsp), in the dsp the message haven't the badprodurct
			pdbplan->dwGoodProductCount += GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
		//	re-set the shotcount	
		//printf("dwPlanProduceCount=%d,dwGoodProductCount=%d,wUnitCount=%d,lRealShotCount=%d\n",pdbplan->dwPlanProduceCount,pdbplan->dwGoodProductCount,pdbplan->wUnitCount,lRealShotCount);
		//if(pdbplan->dwPlanProduceCount <= pdbplan->dwGoodProductCount)
		//{
		//		SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT",lRealShotCount);
		//}
		//else
		//{
		//	SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT", 
		//			long(ceil(double(pdbplan->dwPlanProduceCount - pdbplan->dwGoodProductCount) / 
		//				double(pdbplan->wUnitCount))) + 
		//				lRealShotCount);
		//}
		SaveCurrentPlan();
		memcpy(&g_ProManage, pdbplan, sizeof(DB_PRODMANAGE));
		SendMsg(MSG_SH_INET_MANU_PRODUCT_COUNT, pdbplaninfo->nCurrentIndex, nBadProduct, NULL);
		
	}
	else if (pdbplaninfo != NULL && pdbplaninfo->nCurrentIndex == 0)
	{
		SendMsg(MSG_SH_INET_MANU_PRODUCT_COUNT, 0, nBadProduct, NULL);
	}
}

void	CtmManufacturePlan::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	 switch (message)
	{
		case MSG_DSP54_GET_OPMODE:
			SetProduceStatus(wParam);
			break;
		case MSG_DSP54_GET_SHOTCOUNT:
			SetProductCount(wParam);
			break;
		case MSG_SH_MANUFACTURE_SCHEDULE_STATUS:
			if (wParam > 0) wParam = GetRealIndex((int)wParam);
			SetScheduleStatus(wParam, lParam);
			break;	
		case MSG_SH_MANUFACTURE_SHOTCOUNT:
		{
			DB_PRODMANAGE 		*pdbplaninfo = (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetPlanInfo();
			DB_PRODMANAGE 		*pdbplan	 = (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetCurrentPlan();
			MsgBox(g_MultiLanguage["MSG_MOLD_SHOTREACH"], tmFT_CODE_TECH);
		    if(g_bMsgBoxFlag)	
		    {
		    	if ((pdbplan != NULL && pdbplaninfo != NULL )&&(pdbplaninfo->nCurrentIndex != 0))
				{
					SetScheduleStatus(pdbplaninfo->nCurrentIndex, BILLSTAT_FINISHED);
					SetCurrentPlan(NULL);
					SetScheduleStatus(0, 0);	//	produce in the no plan state
				}
				SendMsg(MSG_SH_MANUFACTURE_ENDPROD, 0, 0, NULL);	//結束生產 2010/9/29 02:33下午
		    }
		    else
		    {
		    	SendMsg(MSG_SH_MANUFACTURE_CONTINUEPROD, 0, 0, NULL);//輸入次品數 
		    }
		}
			break;
		default:
			break;
	}
}


int		CtmManufacturePlan::GetRealIndex(int nLogicIndex)
{
	//if (nLogicIndex > m_manufactureplanHeadInfo.wCount) return -1;
		
	char	acNum[18];
	int		i	= 1;
	int		nTemp = 1;
	while (i <= m_nMaxPlanCount)
	{
		memset(acNum, 0, sizeof(acNum));
		m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, i, acNum, 
												sizeof(acNum), STORAGE_TYPE_SRAM);		
		if (acNum[0] != '\0')
		{
			if(nLogicIndex == nTemp)	return i;
			else						nTemp++;
		}
		i++;
	}	
	return -1;
}

void    SetPlanScheduleStatus(TNET_PLANSTATUS* pScheduleStatus, DB_PRODMANAGE* pProdManage)
{
	if (pScheduleStatus == NULL || pProdManage == NULL) return;
	strcpy(pScheduleStatus->acPlanScheduleNumber, pProdManage->acPlanScheduleNumber);
	pScheduleStatus->wPlanScheduleStatus    = pProdManage->wPlanScheduleStatus;
	pScheduleStatus->dwGoodProductCount     = pProdManage->dwGoodProductCount;
	pScheduleStatus->dwBadProductCount      = pProdManage->dwBadProductCount;
	pScheduleStatus->dtFactBeginDate        = pProdManage->dtFactBeginDate;
	pScheduleStatus->tmFactBeginTime        = pProdManage->tmFactBeginTime;
	pScheduleStatus->dtFactEndDate          = pProdManage->dtFactEndDate;  
	pScheduleStatus->tmFactEndTime          = pProdManage->tmFactEndTime;  
	pScheduleStatus->dtLastPauseDate        = pProdManage->dtLastPauseDate;
	pScheduleStatus->tmLastPauseTime        = pProdManage->tmLastPauseTime;
	pScheduleStatus->dwSortNo        		= pProdManage->dwSortNo;
}

void	CtmManufacturePlan::RecordSort()
{
	if(!m_bSortFlag) return;
	int nIndex = 1;
	DB_PRODMANAGE dbCurrentPlan, dbNextPlan;
	DB_PRODMANAGE *pdbplaninfo	= (DB_PRODMANAGE *)GetPlanInfo();
	if (pdbplaninfo == NULL) return;
	
	for(int i = 1; i < pdbplaninfo->wCount; i++)
	{
		for(nIndex = 1; nIndex <= pdbplaninfo->wCount - i; nIndex++)
		{
			if (m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, nIndex , 
														&dbCurrentPlan, STORAGE_TYPE_SRAM) <=0 )	
			return;
			
			if (m_pPlanFile->GetInstance()->ReadRecord(ID_MANUFACTURE_SRAMFILE, nIndex + 1 , 
														&dbNextPlan, STORAGE_TYPE_SRAM) <=0 )	
			return;
			
			if(dbCurrentPlan.wPlanScheduleStatus == dbNextPlan.wPlanScheduleStatus)
			{
				if(dbCurrentPlan.dwSortNo == dbNextPlan.dwSortNo)
				{
					if(DateTimeToValue(dbCurrentPlan.dtPlanBeginDate, dbCurrentPlan.tmPlanBeginTime) > 
							DateTimeToValue(dbNextPlan.dtPlanBeginDate, dbNextPlan.tmPlanBeginTime))
					{
						m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex, 
														&dbNextPlan, STORAGE_TYPE_SRAM);
						
						m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex + 1, 
														&dbCurrentPlan, STORAGE_TYPE_SRAM);
					}
					else continue;	
				}
				else if(dbCurrentPlan.dwSortNo > dbNextPlan.dwSortNo)
				{
					m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex, 
													&dbNextPlan, STORAGE_TYPE_SRAM);
					
					m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex + 1, 
													&dbCurrentPlan, STORAGE_TYPE_SRAM);
				}
				else continue;	
			}
			else if(dbCurrentPlan.wPlanScheduleStatus < dbNextPlan.wPlanScheduleStatus)
			{
				m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex + 1, 
												&dbCurrentPlan, STORAGE_TYPE_SRAM);
												
				
				m_pPlanFile->GetInstance()->WriteRecord(ID_MANUFACTURE_SRAMFILE, nIndex, 
												&dbNextPlan, STORAGE_TYPE_SRAM);
				
				
			}
			else continue;	
		}
	}
}

DWORD	CtmManufacturePlan::DateTimeToValue(DATE date, tmTIME time)
{
	return 	(date.year*YEA_) + (date.month * MON_) + (date.day * DAY_) + 
			(time.hour * HOU_) + (time.minute * MIN_) + time.second ;
}
