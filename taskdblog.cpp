/*===========================================================================+
|  Class    : TaskDBLog                                                      |
|  Task     : TaskDBLog file		                                         |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Wain                                                           |
|  Version  : V1.00                                                          |
|  Creation : 04/15/2007                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"taskdblog.h"
#include	"main.h"
#include    "utils.h"
#include    "tmconfig.h"
#include    "commonaction.h"
#include	"machinefactor.h"
#include	"tmdebug.h"
#include    "readdata.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
typedef void *(* PRSRUN)(void *);

static int month[12] = {
	0,
	DAY*(31), 
	DAY*(31+28),
	DAY*(31+28+31),
	DAY*(31+28+31+30),
	DAY*(31+28+31+30+31),
	DAY*(31+28+31+30+31+30),
	DAY*(31+28+31+30+31+30+31),
	DAY*(31+28+31+30+31+30+31+31),
	DAY*(31+28+31+30+31+30+31+31+30),
	DAY*(31+28+31+30+31+30+31+31+30+31),
	DAY*(31+28+31+30+31+30+31+31+30+31+30)
};

int       CTaskDBLog::m_nCount              	=  0;
int       CTaskDBLog::m_bNextWarmSaveFlagAddr 	=  0;								// Sram用來告訴下次開機時是否要保存SRAM中的點到加溫曲線中去
int       CTaskDBLog::m_nTempStartAddr      	=  0;                               // Sram用於存放溫度的首地址
int       CTaskDBLog::m_nTempDataMessage    	=  0;                               // Sram存放本次溫度信息的地址
int       CTaskDBLog::m_nTempDataAddr       	=  0;                               // Sram用於存放溫度數據的地址   
int       CTaskDBLog::m_nTempSetMessageAddr 	=  0;                               // Sram用於存放溫度設定值信息的地址
int       CTaskDBLog::m_nTempSetDataAddr    	=  0;                               // Sram用於存放溫度設定值數據地址

UTIME     CTaskDBLog::m_RecentCloseTime     	=  0;
UTIME     CTaskDBLog::m_Time                    =  0;    
UTIME     CTaskDBLog::m_NextStartTime           =  0;
BOOL      CTaskDBLog::m_bFlashThread            =  FALSE;                                               
BOOL      CTaskDBLog::m_bMMcThread              =  FALSE;  

int       CTaskDBLog::m_nMaxCurvePointNumber    =  0;
int       CTaskDBLog::m_nWarmCurvePointNumber   =  0;
 int      CTaskDBLog::m_nCurveUse               =  0; 
BOOL			CTaskDBLog::m_bNextWarmSaveFlag					= FALSE;
BOOL			CTaskDBLog::m_bWarmSaveFlag						= FALSE;			//是否保存加溫曲線的標志
BOOL			CTaskDBLog::m_bWarmStart						= FALSE;			//加溫曲線是否已經開始
UTIME     		CTaskDBLog::m_WarmStartTime						= 0;
int				CTaskDBLog::m_nWarmOffset						= 0;
int				CTaskDBLog::m_nNowOffset 						= 0;
int				CTaskDBLog::m_nNowSpaceTime 					= TEMP_TIME_COUNTER;
int				CTaskDBLog::m_nBeginBlockIndex 					= 0;   
int				CTaskDBLog::m_nEndBlockIndex 					= 0;
int				CTaskDBLog::m_nWarmBeginIndex					= 0;
int				CTaskDBLog::m_nWarmEndIndex						= 0;
TEMPSAVEBLOCK	CTaskDBLog::m_SaveBlockBuff[MAXCURVESEGMENT]    = {0};
TEMPSAVEBLOCK	CTaskDBLog::m_WarmBlockBuff[WARMCURVESEGMENT]   = {0};
CSimpleFile*	CTaskDBLog::m_pCurveFile 					= NULL;


CtmMountFile*	pStorage = NULL;
BOOL	  m_bMounted	= FALSE;
int		  nOldHeaterStatusOn = 0xFFFFFF;
int		  nNewHeaterStatusOn = 0xFFFFFF;
sem_t		g_dbBlogThreadWait;

//void PrintTime(UTIME Time)
//{
//	int     nMonth = 0, nDay = 0;                                                       
//    int     nHour = 0, nMinute = 0, nSecond = 0;                                        
//    struct	tm		tmTime;                                                             
//    tmTime	=*gmtime(&Time);                                                         
//    nDay	= tmTime.tm_mday;                                                           
//    nMonth  = tmTime.tm_mon + 1;                                                        
//    nHour   = tmTime.tm_hour;                                                           
//    nMinute	= tmTime.tm_min;                                                            
//    nSecond	= tmTime.tm_sec;                                                            
//	printf("**%02u%c%02u%c%02u%c%02u%c%02u\n", nMonth, '/', nDay, ' ', nHour, ':',nMinute, ':', nSecond);
//}
//
//void* 	SetTempThread(void *)
//{
//	int		i = 0, j = 0;
//	int		k = 10;
//	char	szID[200];
//	int		nSetTempID[tmMAX_CH_TEMP];
//	int		nRealTempID[tmMAX_CH_TEMP];
//	for(i = 0; i < tmMAX_CH_TEMP; i ++)
//	{
//		sprintf(szID, "%s%d%s", "MSET_TMP1_HEATERON_CHL",i + 1,"_DEGREE");		
//		nSetTempID[i]		= g_pDatabase->GetIndex(szID);
//		
//		sprintf(szID, "%s%d%s", "TEMP_TMP1_HEATERONOFF_CHL",i + 1,"_REALDEGREE");
//		nRealTempID[i]	= g_pDatabase->GetIndex(szID);
//	}
//	while(1)
//	{
//		j+=k;
//		if(j > 300)
//			k = -k;
//		else if(j<0)
//			k = -k;
//		for(i = 0; i < tmMAX_CH_TEMP; i ++)
//		{
//			SetDBValue(nSetTempID[i], j);
//			SetDBValue(nRealTempID[i], j+10);
//		}
//		sleep(10);
//	}
//}



/*==========================================================================+
|          Constructor and destructor                                       |
+==========================================================================*/
CTaskDBLog::CTaskDBLog() : CTask()
{                      
	
	//sem_init(&g_dbBlogThreadWait, 0, 0);
	#ifdef	D_ARM
		#ifdef	D_PXA270
		pStorage = (CtmMountFile*)CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_USB);
		#else
		pStorage = (CtmMountFile*)CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_MMCARD);
		#endif
	#else
		pStorage = (CtmMountFile*)CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_USB);
	#endif
	
	
    int nPosition             = 0;//256*1024;
    m_bNextWarmSaveFlagAddr   = nPosition;
    m_nTempStartAddr	  	  = m_bNextWarmSaveFlagAddr + sizeof(BOOL);
    m_nTempDataMessage 	  	  = m_nTempStartAddr + sizeof(UTIME);
    m_nTempDataAddr           = m_nTempDataMessage + sizeof(TEMPSAVEBLOCK);
    m_nTempSetMessageAddr     = m_nTempDataAddr + sizeof(TEMPSAVEDATA);
    m_nTempSetDataAddr        = m_nTempSetMessageAddr + sizeof(int);
    Set();
    LoadCurveInfo();
	InitTempSavedSram();
    m_WarmStartTime		= 0;
    m_wSaveIndex        = 0;      
    m_FlashThread		= 0;
    m_lTempNumber       = 0;
	m_lColCount 	    = 0;
	m_lColedCount	    = 0;
	m_lCounter		    = m_nNowSpaceTime*1000;
	m_lColAlter		    = 5;
	m_bCollsion		    = FALSE;
	AddCounter(&m_lCounter, m_idMe);
	m_bCollsion			= FALSE;

//	//測試用線程
//	pthread_t ThreadID;
//	pthread_create(&ThreadID, NULL, SetTempThread, NULL);
}

CTaskDBLog::~CTaskDBLog()
{	
	::DeleteCounter(&m_lCounter,  m_idMe);
	delete m_pCurveFile;
}

void		CTaskDBLog::LoadCurveInfo()
{
	ReadSram(m_bNextWarmSaveFlagAddr, sizeof(BOOL), (BYTE *)&m_bNextWarmSaveFlag);
	
	memset(m_WarmBlockBuff, 0, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT);
	memset(m_SaveBlockBuff, 0, sizeof(TEMPSAVEBLOCK)*MAXCURVESEGMENT);

	m_pCurveFile->Open("WarmCurveInfo.cdb");
	ReadSavedData(0, &m_nWarmBeginIndex,sizeof(int));
	ReadSavedData(sizeof(int), &m_nWarmEndIndex,sizeof(int));
	ReadSavedData(sizeof(int)*2, m_WarmBlockBuff, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT);
	m_pCurveFile->Close();
    
	m_pCurveFile->Open("CurveInfo.cdb");
	ReadSavedData(0, &m_nBeginBlockIndex,sizeof(int));
	ReadSavedData(sizeof(int), &m_nEndBlockIndex,sizeof(int));
	ReadSavedData(sizeof(int)*2, m_SaveBlockBuff, sizeof(TEMPSAVEBLOCK)*MAXCURVESEGMENT);
	m_pCurveFile->Close();

	if(m_nBeginBlockIndex > MAXCURVESEGMENT || m_nBeginBlockIndex < 0)
		m_nBeginBlockIndex = 0;
	if(m_nEndBlockIndex > MAXCURVESEGMENT || m_nEndBlockIndex < 0)
		m_nEndBlockIndex = 0;
	if(m_nWarmEndIndex > WARMCURVESEGMENT || m_nWarmEndIndex < 0)
		m_nWarmEndIndex = 0;
	
	m_nWarmOffset   = m_WarmBlockBuff[m_nWarmEndIndex].nOffset 
					+ m_WarmBlockBuff[m_nWarmEndIndex].nPointNumber;
	m_nNowSpaceTime = m_SaveBlockBuff[m_nEndBlockIndex].nSpaceTime;
	m_nNowOffset 	= m_SaveBlockBuff[m_nEndBlockIndex].nOffset 
					+ m_SaveBlockBuff[m_nEndBlockIndex].nPointNumber;
	if(m_nNowOffset >= m_nMaxCurvePointNumber)
			m_nNowOffset -= m_nMaxCurvePointNumber;
	if(m_nNowSpaceTime > 10 || m_nNowSpaceTime < 4)
		m_nNowSpaceTime = TEMP_TIME_COUNTER;
}

/*==========================================================================+
|          Operations                                                       |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|  Class    : CTaskDBLog                                                    |
|  Task     : read and write data to file                                   |
+---------------------------------------------------------------------------+
|  Call     : CTaskDBLog::Run()                                             |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void        CTaskDBLog::Run()
{
    if(m_lCounter <= 0)
	{
		m_lCounter		= m_nNowSpaceTime*1000 + m_lCounter;
		int			   nLostPoint = 0;
		UTIME          now;
	    TEMPSAVEBLOCK  TempBlock;	                // 本次sram保存信息 		
		m_lTempNumber++;
   	 	now = GetNowTime();
        if (m_lTempNumber == 1)
            m_nStartTime = now;
    
        //========
		// 檢測設定溫度，有變化則保存入sram
		//========
		SetTemp(now);
		for(int i = 0; i < m_nCurveUse; i ++)
		{
		    m_TempSet[m_wSaveIndex][i]	= g_pDatabase->Read(m_nSetTempID[i]).wData;
		} 
		for(int i = 0; i < m_nCurveUse; i ++)
		{		
		    m_TempSave[m_wSaveIndex][i]	= g_pDatabase->Read(m_nRealTempID[i]).wData;					
		}
		//========
		// 將各種信息保存入sram
		//======== 
		if (m_nCount == 0)
		{
			if(m_NextStartTime != 0 && now - m_NextStartTime > 0)
				//補償每次封存曲線數據時可能會丟失的點，由于每次定時器的誤差累積，可能會有2~3個點
			{
				nLostPoint = (now - m_NextStartTime)/m_nNowSpaceTime + 1;
				if(nLostPoint > 3)
					nLostPoint = 3;
				m_Time = now - nLostPoint*m_nNowSpaceTime;
				while(nLostPoint-- > 0)
				{
		    		WriteSram(m_nTempDataAddr + m_nCount*sizeof(m_TempSave[0]), sizeof(m_TempSave[0]), 
        					(BYTE *)&m_TempSave[m_wSaveIndex]);
        			m_nCount++;
        		}
			}
			else
			{
		    	m_Time = now;
        	}
		}
		
        TempBlock.StartTime    = m_Time;
        TempBlock.nPointNumber = ++m_nCount; 
        TempBlock.nSpaceTime   = m_nNowSpaceTime;
        
        WriteSram(m_nTempStartAddr, sizeof(UTIME), (BYTE *)&now);                   // 寫入最新保存時間
        WriteSram(m_nTempDataAddr + (m_nCount-1)*sizeof(m_TempSave[0]), sizeof(m_TempSave[0]), 
        		(BYTE *)&m_TempSave[m_wSaveIndex]); //寫入保存數據
        WriteSram(m_nTempDataMessage, sizeof(TEMPSAVEBLOCK), (BYTE *)&TempBlock);   // 寫入本次sram保存信息
		m_wSaveIndex++;
		if(m_wSaveIndex >= MAX_TEMPRECORDSAVED)
			m_wSaveIndex = 0;

		if(!m_bWarmSaveFlag)				
		{
			if(m_nWarmEndIndex == 0)			//需求變更為如果沒有溫度曲線資料時才自動記錄，否則手動記錄時才記錄
			{
				nNewHeaterStatusOn = g_flHeaterStatus;
				if(nNewHeaterStatusOn != nOldHeaterStatusOn && nNewHeaterStatusOn != 0)			//開機后電熱第一次開
				{
					SaveTempToFlash(FALSE);							//將buf中的數據寫入文件中使加溫曲線與當前曲線保持同步。
					m_bWarmSaveFlag = TRUE;
					return;
				}
			}
		}
		else
		{
			if(!m_bWarmStart)
			{
				m_WarmStartTime = m_Time;
				m_bWarmStart = TRUE;
				m_bNextWarmSaveFlag = TRUE;
				WriteSram(m_bNextWarmSaveFlagAddr, sizeof(BOOL), (BYTE *)&m_bNextWarmSaveFlag);
				m_nWarmOffset			= 0;
				m_nWarmBeginIndex 		= 0;
				m_nWarmEndIndex 		= 0;
    			memset(m_WarmBlockBuff, 0, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT);
    			m_pCurveFile->Open("WarmCurveInfo.cdb");
    			m_pCurveFile->Truncate();
    			m_pCurveFile->Close();
			}
		}
		
		if(m_nCount >= ONEHOURPOINT/4/15)			//sram中曲線存滿15分鐘存到文件中（sram上文件系統的文件,非Flash）
		{
	    	if(!m_bMMcThread)
	    	    SaveTempToFlash(m_bWarmSaveFlag);
		}
	}
}

/*------------------------------------------------------------------------------------+
|  Function : ReadCurrentData(WORD TempRank)                                          |
|  Task     : 取出一條曲縣的當前值                                                    |
+-------------------------------------------------------------------------------------+
|  Call     : ReadCurrentData(TempRank)                                               |
|                                                                                     |
|  Parameter: TempRank: 曲縣序號                                                      |
|                                                                                     |
|                                                                                     |
|  Return   : 曲縣的當前值                                                            |
+------------------------------------------------------------------------------------*/
void		CTaskDBLog::ReadCurrentData(WORD TempRank, int TimeSelect, TEMPSAVESELECT *pDataLog)
{
	GetTempCurveData("TC", GetCurrentStartTime(TimeSelect), TempRank, TimeSelect, pDataLog);
}

/*------------------------------------------------------------------------------------+
|  Function : ReadCurrentSetData(WORD TempRank)                                       |
|  Task     : 取出一條曲縣的當前設定值                                                |
+-------------------------------------------------------------------------------------+
|  Call     : ReadCurrentSetData(TempRank)                                            |
|                                                                                     |
|  Parameter: TempRank: 曲縣序號                                                      |
|                                                                                     |
|                                                                                     |
|  Return   : 曲縣的當前設定值                                                        |
+------------------------------------------------------------------------------------*/
void		CTaskDBLog::ReadCurrentSetData(WORD TempRank, int TimeSelect, TEMPSETSELECT *pDataLog)
{
	GetSetData(GetCurrentStartTime(TimeSelect), TempRank, TimeSelect, pDataLog);
}
/*------------------------------------------------------------------------------------+
|  Function : GetRealStartTime()                                                      |
|  Task     : 取出當前值的開始時間                                                    |
+-------------------------------------------------------------------------------------+
|  Call     : GetCurrentStartTime()                                                   |
|                                                                                     |
|  Parameter:                                                                         |
|                                                                                     |
|                                                                                     |
|  Return   : 當前值的開始時間                                                        |
+------------------------------------------------------------------------------------*/
UTIME              CTaskDBLog::GetCurrentStartTime(int TimeSelect)
{
    UTIME Time = GetNowTime() - TimeSelect;
    return m_nStartTime > Time ? m_nStartTime : Time;
    //return GetCurveStartTime() > Time ? GetCurveStartTime() : Time;
}        

UTIME				CTaskDBLog::GetCurveStartTime()
{
	UTIME StartTime = m_Time;
	int i = m_nBeginBlockIndex;
	while((m_SaveBlockBuff[i].StartTime == 0) && (i < MAXCURVESEGMENT))
		i++;
	if(i<MAXCURVESEGMENT)
		StartTime = m_SaveBlockBuff[i].StartTime;
	return StartTime;
}

UTIME              CTaskDBLog::GetWarmStartTime()
{
	UTIME StartTime = 0;
	if(m_bWarmStart)
		StartTime = m_WarmStartTime;
	else
	{
		int i = m_nWarmBeginIndex;
		while((m_WarmBlockBuff[i].StartTime == 0) && (i < WARMCURVESEGMENT))
			i++;
		if(i<WARMCURVESEGMENT)
			StartTime = m_WarmBlockBuff[i].StartTime;
	}
	return StartTime;
}

UTIME				CTaskDBLog::GetWarmEndTime()
{
	UTIME EndTime = 0;
	if(m_bNextWarmSaveFlag)
		EndTime = GetNowTime();
	else
	{
		EndTime = m_WarmBlockBuff[m_nWarmEndIndex].StartTime + 
			m_WarmBlockBuff[m_nWarmEndIndex].nPointNumber* m_WarmBlockBuff[m_nWarmEndIndex].nSpaceTime;
	}
	return EndTime;
}

void      CTaskDBLog::GetCurveData(char *cName, UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog, BOOL bWarm)
{
	
	TEMPSAVEBLOCK *pSaveBlockBuff 		= bWarm ? m_WarmBlockBuff : m_SaveBlockBuff;
	int				nBeginBlockIndex 	= bWarm ? m_nWarmBeginIndex : m_nBeginBlockIndex;
	int				nEndBlockIndex	 	= bWarm ? m_nWarmEndIndex : m_nEndBlockIndex; 
	const	int		nMaxCurvePointNumber= bWarm ? m_nWarmCurvePointNumber : m_nMaxCurvePointNumber;
		
	int				i = 0, j = 0;
    char            sz[64];
    UTIME			tempTime;
    int				tempIndex;
    INT16           aTemper[ONEHOURPOINT];
  	TEMPSAVEBLOCK  	SramBlock;
	TEMPSAVEDATA    SramTemper; 
	memset(sz,0,sizeof(sz)/sizeof(char));
	memset(aTemper,0,sizeof(INT16)*ONEHOURPOINT);
	memset(&SramBlock,0,sizeof(TEMPSAVEBLOCK));
	memset(&SramTemper,0,sizeof(TEMPSAVEDATA));
	ReadSram(m_nTempDataMessage, sizeof(TEMPSAVEBLOCK), (BYTE *)&SramBlock);
  	ReadSram(m_nTempDataAddr, sizeof(TEMPSAVEDATA), (BYTE *)&SramTemper);
		
	sprintf(sz, "%s%d%s",cName, nSelectTemp,".cdb");
    m_pCurveFile->Open(sz);
    tempIndex = nBeginBlockIndex;
    tempTime = Time;
    i = 0;
    //讀文件中的點
    while(tempTime < Time+TimeSelect)
    {	
    	if(tempTime < pSaveBlockBuff[tempIndex].StartTime)
    	{
    		pDataLog->Select[i].Time = tempTime-Time;
    		pDataLog->Select[i].Temp = -1;
    		i++;
    		if(Time+TimeSelect < pSaveBlockBuff[tempIndex].StartTime)
    		{
    			tempTime = Time+TimeSelect;
    			pDataLog->Select[i].Time = tempTime-Time;
    			pDataLog->Select[i].Temp = -1;
    			i++;
    		}
    		else
    		{
    			tempTime = pSaveBlockBuff[tempIndex].StartTime;
    			pDataLog->Select[i].Time = tempTime-Time;
    			pDataLog->Select[i].Temp = -1;
    			i++;
    			tempTime = pSaveBlockBuff[tempIndex].StartTime;
    		}
    	}
    	if(tempTime >= pSaveBlockBuff[tempIndex].StartTime &&
    			tempTime < pSaveBlockBuff[tempIndex].StartTime+
    			pSaveBlockBuff[tempIndex].nPointNumber*pSaveBlockBuff[tempIndex].nSpaceTime)
    	{
    		if(pSaveBlockBuff[tempIndex].nOffset+pSaveBlockBuff[tempIndex].nPointNumber > nMaxCurvePointNumber)
    		{
		    	ReadSavedData(pSaveBlockBuff[tempIndex].nOffset*sizeof(INT16), aTemper, 
		    				(nMaxCurvePointNumber - pSaveBlockBuff[tempIndex].nOffset)*sizeof(INT16));
		    	ReadSavedData(0,
		    				&aTemper[nMaxCurvePointNumber - pSaveBlockBuff[tempIndex].nOffset],
		    				(pSaveBlockBuff[tempIndex].nOffset + 
		    				pSaveBlockBuff[tempIndex].nPointNumber - 
		    				nMaxCurvePointNumber)*sizeof(INT16));
		    	
		    }   
		    else
		    {   
		    	ReadSavedData(pSaveBlockBuff[tempIndex].nOffset*sizeof(INT16), aTemper, 
		    						pSaveBlockBuff[tempIndex].nPointNumber*sizeof(INT16));
		    }
		    
		    j = (tempTime - pSaveBlockBuff[tempIndex].StartTime)/pSaveBlockBuff[tempIndex].nSpaceTime;
		    if(tempTime > pSaveBlockBuff[tempIndex].StartTime+j*pSaveBlockBuff[tempIndex].nSpaceTime)
		    {
		    	pDataLog->Select[i].Time = tempTime-Time;
		    	pDataLog->Select[i].Temp = aTemper[j];
		    	i++;
		    	j++;
		    	tempTime = pSaveBlockBuff[tempIndex].StartTime+j*pSaveBlockBuff[tempIndex].nSpaceTime;
		    }
		    while((j < pSaveBlockBuff[tempIndex].nPointNumber) && (tempTime < Time+TimeSelect))
		    {
		    	pDataLog->Select[i].Time = tempTime-Time;
		    	pDataLog->Select[i].Temp = aTemper[j];
		    	i++;
		    	j++;
		    	tempTime += pSaveBlockBuff[tempIndex].nSpaceTime;
		    }
		    if(tempIndex == nEndBlockIndex)
    			break;
			if(bWarm)
				tempIndex = tempIndex+1 > WARMCURVESEGMENT-1 ? 0 : tempIndex+1;
			else
				tempIndex = tempIndex+1 > MAXCURVESEGMENT-1 ? 0 : tempIndex+1;
    	}
    	if(tempTime >= pSaveBlockBuff[tempIndex].StartTime+
    			pSaveBlockBuff[tempIndex].nPointNumber*pSaveBlockBuff[tempIndex].nSpaceTime)
    	{
    		if(tempIndex == nEndBlockIndex)
    			break;
			if(bWarm)
				tempIndex = tempIndex+1 > WARMCURVESEGMENT-1 ? 0 : tempIndex+1;
			else
				tempIndex = tempIndex+1 > MAXCURVESEGMENT-1 ? 0 : tempIndex+1;
    	}
    }
    m_pCurveFile->Close();
    
    if(bWarm && !m_bWarmSaveFlag)							//還未開始記錄加溫曲線的話就不將BUF中的點顯示出來
    {
    	pDataLog->nNumber = i;
    	return ;
    }
    //讀sram中的點
	while(tempTime < Time+TimeSelect)
	{
		if(tempTime < SramBlock.StartTime)
    	{
    		pDataLog->Select[i].Time = tempTime-Time;
    		pDataLog->Select[i].Temp = -1;
    		i++;
    		if(bWarm && !m_bNextWarmSaveFlag)
    		{
    			tempTime = Time+TimeSelect;
    			pDataLog->Select[i].Time = tempTime-Time;
    			pDataLog->Select[i].Temp = -1;
    			i++;
    			pDataLog->nNumber = i;
    			return ;
    		}
    		if(Time+TimeSelect < SramBlock.StartTime)
    		{
    			tempTime = Time+TimeSelect;
    			pDataLog->Select[i].Time = tempTime-Time;
    			pDataLog->Select[i].Temp = -1;
    			i++;
    		}
    		else
    		{
    			tempTime = SramBlock.StartTime-1;
    			pDataLog->Select[i].Time = tempTime-Time;
    			pDataLog->Select[i].Temp = -1;
    			i++;
    			tempTime = SramBlock.StartTime;
    		}
    	}
    	else if(tempTime >= SramBlock.StartTime &&
    		tempTime < SramBlock.StartTime + SramBlock.nPointNumber*SramBlock.nSpaceTime)
    	{
    		j = (tempTime - SramBlock.StartTime)/SramBlock.nSpaceTime;
		    if(tempTime > SramBlock.StartTime+j*SramBlock.nSpaceTime)
		    {
		    	pDataLog->Select[i].Time = tempTime-Time;
		    	pDataLog->Select[i].Temp = SramTemper.SaveTemp[j][nSelectTemp-1];
		    	i++;
		    	j++;
		    	tempTime = SramBlock.StartTime+j*SramBlock.nSpaceTime;
		    }
    		while(j < SramBlock.nPointNumber && tempTime < Time+TimeSelect)
		    {
		    	pDataLog->Select[i].Time = tempTime-Time;
		    	pDataLog->Select[i].Temp = SramTemper.SaveTemp[j][nSelectTemp-1];
		    	i++;
		    	j++;
		    	tempTime += SramBlock.nSpaceTime;
		    }
    	}
    	else
    	{
    		break;
    	}
	}
    pDataLog->nNumber = i;
    return ;
}
void       CTaskDBLog::GetRealData(UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog, BOOL bWarm)
{
	if(bWarm)
		GetWarmCurveData("WC", Time, nSelectTemp, TimeSelect, pDataLog);
	else
		GetTempCurveData("TC", Time, nSelectTemp, TimeSelect, pDataLog);
}
/*--------------------------------------------------------------------------------------+
|  Function : GetSetData(UTIME Time, int nSelectTemp)                                   |
|  Task     : 取出某段溫度MAX_TEMPSELECT個點時間的設定值                                           |
+---------------------------------------------------------------------------------------+
|  Call     : GetSetData(Time, nSelectTemp)                                             |
|                                                                                       |
|  Parameter: Time:           開始時間(從1970年以來的秒數)                              |
|             nSelectTemp:    溫度段數                                                  |          
|  Return   : 設定溫度數據及取點信息                                                    |
+--------------------------------------------------------------------------------------*/
void       	CTaskDBLog::GetSetData(UTIME Time, int nSelectTemp, int TimeSelect, TEMPSETSELECT *pDataLog)
{
    int            i = 0,j = 0, m = 0;
    int            nLast;
    UTIME 		   nTempMaxTime = 0;
    UTIME		   tmpTime;
    UTIME		   EndTime;
    UTIME          now;
    TEMPSET        *pSet;
    
    now = GetNowTime();
    EndTime   = Time + TimeSelect > now ? now : Time + TimeSelect;
    ReadSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast);
    if ((nLast <= 0) || (nLast > MAX_TEMPSETSAVED))
    {
        pDataLog->nNumber = 0;
        return ;
    }
    
    pSet = new  TEMPSET[MAX_TEMPSETSAVED];
    ReadSram(m_nTempSetDataAddr, MAX_TEMPSETSAVED*sizeof(TEMPSET), (BYTE *)pSet);
  
	j = nLast-1;
    nTempMaxTime = pSet[j].Time;
    if(pSet[j].Time <= Time)						
    {
    	i = j;
    }
    else
    {
    	i = j - 1 < 0 ? MAX_TEMPSETSAVED - 1: j - 1;
    	while(i!=j)
    	{
    		if(pSet[i].Time <= Time)
    			break;
    		i = i - 1 < 0 ? MAX_TEMPSETSAVED - 1: i - 1;
    	}
    	if(i == j) 
    	{
    		i = j + 1 > MAX_TEMPSETSAVED - 1 ? 0: j + 1;
    	}
    }
    tmpTime = Time;
    m = 0;
    if(tmpTime < pSet[i].Time)							//開始處可能有無效的點
    {
    	pDataLog->Select[m].Time = tmpTime-Time;
    	pDataLog->Select[m].Temp = -1;
    	m++;
    	tmpTime = pSet[i].Time < EndTime ? pSet[i].Time : EndTime;
    	pDataLog->Select[m].Time = tmpTime-Time;
    	pDataLog->Select[m].Temp = -1;
    	m++;	
    }
    while(tmpTime < EndTime)								//根據時間分段賦值
    {
    	if(tmpTime >= nTempMaxTime)
    	{
    		pDataLog->Select[m].Time = tmpTime-Time;
    		pDataLog->Select[m].Temp = pSet[i].Temp[nSelectTemp-1];
    		m++;
    		tmpTime = EndTime;
    		pDataLog->Select[m].Time = tmpTime-Time;
    		pDataLog->Select[m].Temp = pSet[i].Temp[nSelectTemp-1];
    		m++;
    		break;
    	}
    	pDataLog->Select[m].Time = tmpTime-Time;
    	pDataLog->Select[m].Temp = pSet[i].Temp[nSelectTemp-1];
    	m++;	
    	i = i + 1 > MAX_TEMPSETSAVED - 1 ? 0 : i + 1;
    	tmpTime = pSet[i].Time;
    	if(tmpTime > EndTime)
   	 	{
    		tmpTime = EndTime;
    		pDataLog->Select[m].Time = tmpTime-Time;
    		pDataLog->Select[m].Temp = pDataLog->Select[m-1].Temp;
    		m++;
    	}
    	else
    	{	
    		pDataLog->Select[m].Time = tmpTime-Time;
    		pDataLog->Select[m].Temp = pDataLog->Select[m-1].Temp;
    		m++;
    	}
    }
    pDataLog->nNumber = m;
    delete []pSet; 
    return ;    
}

void			CTaskDBLog::UMountTread()
{
	pthread_detach(pthread_self());
	pStorage->UMount();
    m_bMounted = FALSE;
    m_bMMcThread = FALSE;
}
/*--------------------------------------------------------------------------------------+
|  Function : SaveToMMc()                                                               |
|  Task     : 保存文件入MMC                                                             |
+---------------------------------------------------------------------------------------+
|  Call     : SaveToMMc()                                                               |
|                                                                                       |
|  Parameter:                                                                           |
|  Return   : 無                                                                        |
+--------------------------------------------------------------------------------------*/
void            CTaskDBLog::SaveToMMc()
{
    if (!m_bMMcThread)
    {
        pthread_create(&m_MMcThread, NULL, (PRSRUN)SaveFileToMMc, NULL);                 // 將sram中剩餘的資料寫入flash中
        
        //sem_wait(&g_dbBlogThreadWait);
        //CtmUsbFile::GetInstance()->UMount();
    	//Delay(3000);
    	//Prompt(g_MultiLanguage["VM_DATA_READ"]);
    	
    }
        
    
}

/*--------------------------------------------------------------------------------------+
|  Function : GetSelectTime(int nHour)                                                  |
|  Task     : 獲取某個小時的從1970年以來的秒數                                          |
+---------------------------------------------------------------------------------------+
|  Call     : GetSelectTime(nHour)                                                      |
|                                                                                       |
|  Parameter: nHour: 一天以來的某個小時                                                 |
|  Return   : 無                                                                        |
+--------------------------------------------------------------------------------------*/
UTIME           CTaskDBLog::GetSelectTime(int nHour)
{
    UTIME now;
    now = GetNowTime();     
    //****
    //  選取的小時數小於現在的小時數，則選取的時間是今天的
    //****
    if ((now%DAY)/HOUR >= nHour)
    {        
        return  (now - (now%DAY) + nHour*HOUR);
    }
    //****
    //  選取的小時數大於現在的小時數，則選取的時間是昨天的
    //****
    else
    {        
        return (now - (now%DAY) - (24-nHour)*HOUR);
    }
}

/*--------------------------------------------------------------------------------------+
|  Function : GetNowTime()                                                              |
|  Task     : 獲取現在從1970年以來的秒數                                                |
+---------------------------------------------------------------------------------------+
|  Call     : GetNowTime()                                                              |
|                                                                                       |
|  Parameter:                                                                           |
|  Return   : 無                                                                        |
+--------------------------------------------------------------------------------------*/
UTIME      CTaskDBLog::GetNowTime()
{
   
	tmDATETIME      Time;
    UTIME           now;
	GetDate(&Time.date);
	GetTime(&Time.time);  
    now = MakeTime(&Time);

	return now;
}
/*---------------------------------------------------------------------------+
|  Function : Reset()                                                        |
|  Task     : 清除sram中信息並重新記錄信息                                   |
+----------------------------------------------------------------------------+
|  Call     : Reset()                                                        |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::Reset()
{
    m_nCount            = 0;
    m_lTempNumber   	= 0;
    m_wSaveIndex		= 0;
    m_NextStartTime 	= 0;
    m_nNowOffset        = 0;
    m_nBeginBlockIndex 	= 0;
    m_nEndBlockIndex 	= 0;
    memset(m_SaveBlockBuff, 0, sizeof(TEMPSAVEBLOCK)*MAXCURVESEGMENT);
    m_bWarmStart        = FALSE;
	m_nWarmOffset	    = 0;
	m_nWarmBeginIndex 	= 0;
	m_nWarmEndIndex 	= 0;
    memset(m_WarmBlockBuff, 0, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT);
    ClearTempHistoryRecord();

}

void       CTaskDBLog::ResetWarm()
{
	SaveTempToFlash(FALSE);
	m_nWarmOffset			= 0;
	m_nWarmBeginIndex 		= 0;
	m_nWarmEndIndex 		= 0;
    memset(m_WarmBlockBuff, 0, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT);
    m_pCurveFile->Open("WarmCurveInfo.cdb");
    m_pCurveFile->Truncate();
    m_pCurveFile->Close();
    m_bWarmStart = FALSE;
    m_bWarmSaveFlag = TRUE;
}

int      CTaskDBLog::GetCurveInUse()
{
    return m_nCurveUse;
}

/*---------------------------------------------------------------------------+
|  Function : GetSramSize()                                                  |
|  Task     : 獲得sram應分配的大小                                           |
+----------------------------------------------------------------------------+
|  Call     : GetSramSize()                                                  |
|                                                                            |
|  Parameter:                                                                |
|  Return   : sram的大小                                                     |
+---------------------------------------------------------------------------*/
int        CTaskDBLog::GetTempSramSize()
{
    int         nSize;
    nSize = sizeof(BOOL) + sizeof(UTIME) + sizeof(TEMPSAVEBLOCK) + 
        sizeof(TEMPSAVEDATA) + sizeof(int) + sizeof(TEMPSET)*MAX_TEMPSETSAVED;
    return nSize;
}
/*==========================================================================+
|          Helpers                                                          |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|  Class    : CTaskDBLog                                                    |
|  Task     : 初始化的一些數據設置                                          |
+---------------------------------------------------------------------------+
|  Call     : CTaskDBLog::Set()                                             |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void		CTaskDBLog::Set()
{
	printf("CTaskDBLog::Set()\n");
	int		i;
	char* szID[tmMAX_CH_TEMP];
	
	for (int i = 0; i < tmMAX_CH_TEMP; i++)
	{
	    szID[i] = new char[128];
	    memset(szID[i], 0, sizeof(char)*128);
	}
	char *pszCurveInfo = NULL;
    int nPos[2]={0};
    
	CIniFile	 m_Curveinfo;
	m_Curveinfo.OpenFile("outerconfig.ini");
	m_Curveinfo.ReadString("TempCurve", "CurveInfo", NULL, &pszCurveInfo);
	m_Curveinfo.CloseFile();
	
	CtmReadData	m_ReadData;
	m_ReadData.OpenFileStr(pszCurveInfo);
	
	ReadDataInt(m_ReadData.GetControlStr()->Controls[0], nPos, 2);
	int Sramfd = open("/dev/sram0", O_RDWR, 0);
    if(Sramfd < 0)
    	Sramfd = open("/usr/ReplaceSram.rps", O_RDWR);
    m_pCurveFile = new CSimpleFile(Sramfd, nPos[0], nPos[1]);

	m_nCurveUse = ReadDataStr(m_ReadData.GetControlStr()->Controls[1], szID, tmMAX_CH_TEMP);
	for(i = 0; i < m_nCurveUse; i ++)
		m_nSetTempID[i]		= g_pDatabase->GetIndex(szID[i]);
	ReadDataStr(m_ReadData.GetControlStr()->Controls[2], szID, m_nCurveUse);
	for(i = 0; i < m_nCurveUse; i ++)
		m_nRealTempID[i]	= g_pDatabase->GetIndex(szID[i]);
    
    //文件系統占據7K,"WarmCurveInfo.cdb"和"CurveInfo.cdb"各占1024字節,文件系統blocksize=512,得出以下公式
	m_nMaxCurvePointNumber = ((((nPos[1] - nPos[0] - 9*1024)/512)/(2*m_nCurveUse))*512)/2;
	m_nWarmCurvePointNumber = m_nMaxCurvePointNumber;

	delete []pszCurveInfo;
	for (int i = 0; i < tmMAX_CH_TEMP; i++)
	    delete []szID[i];
}
/*---------------------------------------------------------------------------+
|  Function : InitTempSavedSram()                                            |
|  Task     : 初始化sram                                                     |
+----------------------------------------------------------------------------+
|  Call     : InitTempSavedSram()                                            |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/

void       CTaskDBLog::InitTempSavedSram()
{                  
    UTIME           now;
    ReadSram(m_nTempStartAddr, sizeof(UTIME), (BYTE *)&m_RecentCloseTime);                   // 讀出上次關機系統時間           
    now = GetNowTime();       
    InitTempSet(now);
    SaveTempToFlash(m_bNextWarmSaveFlag);											//保存BUF中遺留的點
    if(m_bNextWarmSaveFlag)												
    {
    	m_bNextWarmSaveFlag = FALSE;
		WriteSram(m_bNextWarmSaveFlagAddr, sizeof(BOOL), (BYTE *)&m_bNextWarmSaveFlag);		
	}
}

/*---------------------------------------------------------------------------+
|  Function : ClearTempHistoryRecord()                                       |
|  Task     : 清除sram中信息區                                               |
+----------------------------------------------------------------------------+
|  Call     : ClearTempHistoryRecord()                                       |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::ClearTempHistoryRecord()
{      
    int   nTotalLength		 = sizeof(BOOL) + sizeof(UTIME) + sizeof(TEMPSAVEBLOCK) + 
        sizeof(TEMPSAVEDATA) + sizeof(int) + sizeof(TEMPSET)*MAX_TEMPSETSAVED;
    char  *pZero = new char[nTotalLength];
    
    memset(pZero, 0, nTotalLength);
    WriteSram(m_bNextWarmSaveFlagAddr, nTotalLength, (BYTE *)pZero);                          // 清除實際溫度信息
    
    m_pCurveFile->Format();                 //清除所有溫度曲線文件
    
    delete []pZero;
}

/*---------------------------------------------------------------------------+
|  Function : ClearTempDataSram()                                            |
|  Task     : 清除sram中數據區                                               |
+----------------------------------------------------------------------------+
|  Call     : ClearTempDataSram()                                            |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::ClearTempDataSram()
{
    int   nTotalLength    =  sizeof(TEMPSAVEDATA);
    char  *pZero;
    pZero = new char[nTotalLength];
    memset(pZero, 0xFF, sizeof(char)*nTotalLength);
    
    WriteSram(m_nTempDataAddr, nTotalLength, (BYTE *)pZero);
    delete []pZero;
}

/*---------------------------------------------------------------------------+
|  Function : SaveTempToFile()                                               |
|  Task     : 將存儲于sram中數據拷貝入flash中，並修改存于sram中flash存儲信息 |
+----------------------------------------------------------------------------+
|  Call     : SaveTempToFile()                                               |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::SaveTempToFlash(BOOL bSaveWarm)
{
    m_nCount = 0;
    SaveSramTemp("TC");
    if(bSaveWarm)
    	SaveToWarmCurve("WC");
} 

/*---------------------------------------------------------------------------+
|  Function : SaveSramTemp(char *cName)                                      |
|  Task     : 將存儲于sram中數據拷貝入文件中，並修改存于存儲信息             |
+----------------------------------------------------------------------------+
|  Call     : SaveSramTemp(cName)                                            |
|                                                                            |
|  Parameter: cName: 文件名字內均有的字符串                                  |
|  Return   : 溫度數據存儲信息                                               |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::SaveSramTemp(char *cName)
{   
	char			sz[64];
	TEMPSAVEBLOCK  	TempBlock;
	TEMPSAVEDATA    Temper;
	INT16           SwapTemper[ONEHOURPOINT];
	
	memset(sz,0,sizeof(sz)/sizeof(char));
	memset(&TempBlock,0,sizeof(TEMPSAVEBLOCK));
	memset(&Temper,0,sizeof(TEMPSAVEDATA));
	memset(SwapTemper,0,sizeof(INT16)*ONEHOURPOINT);
	
	ReadSram(m_nTempDataMessage, sizeof(TEMPSAVEBLOCK), (BYTE *)&TempBlock);
	ReadSram(m_nTempDataAddr, sizeof(TEMPSAVEDATA), (BYTE *)&Temper);
	TempBlock.nOffset = m_nNowOffset;
	
	//寫曲線數據文件
	if(TempBlock.nOffset+TempBlock.nPointNumber >= m_nMaxCurvePointNumber)
	{
		for (int i = 0; i < m_nCurveUse; i++)
		{
			for (int j = 0; j < TempBlock.nPointNumber; j++)
			{
				SwapTemper[j] = Temper.SaveTemp[j][i];
			}
			
			sprintf(sz, "%s%d%s", cName, i+1,".cdb");
		    m_pCurveFile->Open(sz);
		    WriteSavedData(TempBlock.nOffset*sizeof(INT16),
		    				SwapTemper, 
		    				(m_nMaxCurvePointNumber - TempBlock.nOffset)*sizeof(INT16));
		    WriteSavedData(0,
		    				&SwapTemper[m_nMaxCurvePointNumber - TempBlock.nOffset], 
		    				(TempBlock.nOffset + TempBlock.nPointNumber - m_nMaxCurvePointNumber)*sizeof(INT16));	    
		    m_pCurveFile->Close();
		}
		
		m_nNowOffset = TempBlock.nOffset + TempBlock.nPointNumber - m_nMaxCurvePointNumber;
	}
	else
	{
		for (int i = 0; i < m_nCurveUse; i++)
		{
			for (int j = 0; j < TempBlock.nPointNumber; j++)
			{
				SwapTemper[j] = Temper.SaveTemp[j][i];
			}
			
			sprintf(sz, "%s%d%s",cName, i+1,".cdb");
		    m_pCurveFile->Open(sz);
		    WriteSavedData(TempBlock.nOffset*sizeof(INT16), SwapTemper, TempBlock.nPointNumber*sizeof(INT16));
		    m_pCurveFile->Close();
		}
		m_nNowOffset = TempBlock.nOffset + TempBlock.nPointNumber;
	}
	
	//寫曲線信息文件
	AddOneBlock(m_SaveBlockBuff, &TempBlock, &m_nBeginBlockIndex, &m_nEndBlockIndex);
	m_pCurveFile->Open("CurveInfo.cdb");
	WriteSavedData(0, &m_nBeginBlockIndex, sizeof(int));
	WriteSavedData(sizeof(int), &m_nEndBlockIndex, sizeof(int));
	WriteSavedData(sizeof(int)*2+m_nBeginBlockIndex*sizeof(TEMPSAVEBLOCK), &m_SaveBlockBuff[m_nBeginBlockIndex], sizeof(TEMPSAVEBLOCK));
	WriteSavedData(sizeof(int)*2+m_nEndBlockIndex*sizeof(TEMPSAVEBLOCK), &m_SaveBlockBuff[m_nEndBlockIndex], sizeof(TEMPSAVEBLOCK));
	m_pCurveFile->Close();
	
	m_NextStartTime = TempBlock.StartTime + TempBlock.nPointNumber*TempBlock.nSpaceTime;
	return  ;    	  	
}
/*---------------------------------------------------------------------------+
|  Function : WriteSram(int nPosition, int Length, BYTE *pSource)            |
|  Task     : 從內存中讀出一段數據寫到sram某偏移位置                         |
+----------------------------------------------------------------------------+
|  Call     : ReadSram(nPosition, nLength, pSource                           |
|                                                                            |
|  Parameter: nPosition: sram中的偏移位置                                    |
              nLength:   從sram中讀數据長度                                  |
|             pSource:   讀入數據在內存中存儲首地址                          |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::WriteSram(int nPosition, int nLength, BYTE *pSource)
{
    CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_TEMPCURVE_SRAMFILE, 1, pSource, nLength, nPosition); 
}
 
/*---------------------------------------------------------------------------+
|  Function : ReadSram(int nPosition, int Length, BYTE *pDesti)              |
|  Task     : 從sram某偏移位置出讀出一段數據到內存中                         |
+----------------------------------------------------------------------------+
|  Call     : ReadSram(nPosition, nLength, pDesti)                           |
|                                                                            |
|  Parameter: nPosition: sram中的偏移位置                                    |
              nLength:   從sram中讀數据長度                                  |
|             pDesti :   向內存寫入數據首地址                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::ReadSram(int nPosition, int nLength, BYTE *pDesti)
{  
    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_TEMPCURVE_SRAMFILE, 1, pDesti, nLength, nPosition);
}

/*---------------------------------------------------------------------------+
|  Function : MakeTime(tmDATETIME *tm)                                       |
|  Task     : 將以年月日表示的時間轉化為按從1970年1月1日0時0分0秒至今的秒數  |
|             來表示                                                         |
+----------------------------------------------------------------------------+
|  Call     : MakeTime(tmDATETIME *tm)                                       |
|                                                                            |
|  Parameter: tm :以年月日方式表示的日期                                     |
|                                                                            |
|  Return   : 從1970年1月1日0時0分0秒至今的秒數                              |
+---------------------------------------------------------------------------*/
UTIME      CTaskDBLog::MakeTime(tmDATETIME *tm)
{
	UTIME res;
	int   year;

	year = tm->date.year - 1970;
	res = YEAR*year + DAY*((year+1)/4);									//滿百年的非閏年問題已經無法考慮了
	res += month[tm->date.month - 1];
	
	if ((tm->date.month > 2 ) && (tm->date.year%4 == 0))
		res += DAY;
			
	res += DAY*(tm->date.day-1);
	res += HOUR*tm->time.hour;
	res += MINUTE*tm->time.minute;
	res += tm->time.second;
	
	return res;
}


/*---------------------------------------------------------------------------+
|  Function : GetStartNumber(UTIME Time)                                     |
|  Task     : 獲得某時間位于本小時段存儲時的位置                             |
+----------------------------------------------------------------------------+
|  Call     : GetStartNumber(Time)                                           |
|                                                                            |
|  Parameter: Time: 時間                                                     |              
|  Return   : 此時間位于某小時段存儲的位置                                   |
+---------------------------------------------------------------------------*/
int        CTaskDBLog::GetStartNumber(UTIME Time)
{
    return ((Time%DAY)%HOUR)/TEMP_TIME_COUNTER;
}

/*---------------------------------------------------------------------------+
|  Function : InitTempSet(UTIME Time)                                        |
|  Task     : 開機時對保存的設定值操作，將超過24小時部分僅留下一組           |
+----------------------------------------------------------------------------+
|  Call     : InitTempSet(Time)                                              |
|                                                                            |
|  Parameter: Time: 時間                                                     |              
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void      CTaskDBLog::InitTempSet(UTIME Time)
{
    int             nLast=0;             // 設定溫度數量
    INT16           aSetTemp[tmMAX_CH_TEMP];
    TEMPSET         TempSetValue;        
    memset(aSetTemp,0,sizeof(INT16)*tmMAX_CH_TEMP);
    memset(&TempSetValue,0,sizeof(TEMPSET));
    ReadSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast);
    
    //****
    //   無溫度設定值
    //****
    if ((nLast <= 0) || (nLast > MAX_TEMPSETSAVED)) 				//alien modify	2008/10/17 10:30AM 1000->TEMPSETSAVED 
    {
        nLast = 1;
        TempSetValue.Time  = Time;
        for (int i = 0; i < m_nCurveUse; i++)
        {
            aSetTemp[i] = (INT16)g_pDatabase->Read(m_nSetTempID[i]).wData;
            TempSetValue.Temp[i] = aSetTemp[i];
        }         
        WriteSram(m_nTempSetDataAddr, sizeof(TEMPSET), (BYTE *)&TempSetValue);
        WriteSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast);
        
        return;
    }
}
/*---------------------------------------------------------------------------+
|  Function : SetTemp(UTIME Time)                                            |
|  Task     : 獲得某時間開始MAX_TEMPSELECT點時間內的設定溫度                            |
+----------------------------------------------------------------------------+
|  Call     : SetTemp(Time)                                                  |
|                                                                            |
|  Parameter: Time: 時間                                                     |              
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void      CTaskDBLog::SetTemp(UTIME Time)
{
    int         i;
    int         nLast=0;
    INT16       aNowTemp[tmMAX_CH_TEMP];
    INT16       aLastTemp[tmMAX_CH_TEMP];
    TEMPSET     TempSetValue;
    TEMPSET     LastSetTemp;
    memset(aNowTemp,0,sizeof(INT16)*tmMAX_CH_TEMP);
    memset(aLastTemp,0,sizeof(INT16)*tmMAX_CH_TEMP);
    memset(&TempSetValue,0,sizeof(TEMPSET));
    memset(&LastSetTemp,0,sizeof(TEMPSET));
    //****
    //  取得現在溫度值
    //****
    TempSetValue.Time  =  Time;
    for (i = 0; i < m_nCurveUse; i++)
    {
        aNowTemp[i] = (INT16)g_pDatabase->Read(m_nSetTempID[i]).wData; 
        TempSetValue.Temp[i] = aNowTemp[i];              
    }    
    //****
    //  取得最後保存溫度設定值
    //****
    ReadSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast);
    ReadSram(m_nTempSetDataAddr+ sizeof(TEMPSET)*(nLast-1), sizeof(TEMPSET), (BYTE *)&LastSetTemp);    
    memcpy(aLastTemp, (char *)&LastSetTemp +sizeof(UTIME) , tmMAX_CH_TEMP*sizeof(WORD)); 
    
    //****
    //  若不等，則存儲現在溫度設定值
    //****  
    for (i = 0; i < m_nCurveUse; i++)
    {    
        if (aNowTemp[i] !=  aLastTemp[i])
        {          
            nLast++;
            if (nLast > MAX_TEMPSETSAVED)
                nLast = 1;
            WriteSram(m_nTempSetDataAddr+ sizeof(TEMPSET)*(nLast-1), sizeof(TEMPSET), (BYTE *)&TempSetValue);   
            WriteSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast); 
            break; 
        }
    }        
}

/*--------------------------------------------------------------------------------------+
|  Function : SaveFileToMMc()                                                           |
|  Task     : 將溫度數據保存入mmc卡中                                                   |
+---------------------------------------------------------------------------------------+
|  Call     : SaveFileToMMc()                                                           |
|                                                                                       |
|  Parameter:                                                                           |
|  Return   : 無                                                                        |
+--------------------------------------------------------------------------------------*/
void            CTaskDBLog::SaveFileToMMc()
{
	pthread_detach(pthread_self());
	
    m_bMMcThread = TRUE;

//****
    //  存溫度數據
    //****        
    m_bMounted = pStorage->Mount(FALSE);				//	fans add 2007/9/6 06:13下午
    if (m_bMounted)
    {
    	SendMsg(MSG_TEMP_CURVE_SAVING, 0, 0, NULL);
	    char            sz[64];
    	int             nNumber;
    	int				nBeginBlockIndex = 0;
    	int				nEndBlockIndex = 0;
        int             nBakNowOffset = m_nNowOffset;
    	TEMPSAVEBLOCK	BlockBuff[MAXCURVESEGMENT];
    	TEMPSAVEBLOCK	WarmBlockBuff[WARMCURVESEGMENT];
    	INT16           *pSwapTemper = new INT16[m_nMaxCurvePointNumber];
    	INT16           *pWarmSwapTemper = new INT16[m_nWarmCurvePointNumber];
    	TEMPSAVEBLOCK  	TempBlock;
		TEMPSAVEDATA    Temper;
		INT16           SwapTemper[ONEHOURPOINT];
		
		ReadSram(m_nTempDataMessage, sizeof(TEMPSAVEBLOCK), (BYTE *)&TempBlock);
		ReadSram(m_nTempDataAddr, sizeof(TEMPSAVEDATA), (BYTE *)&Temper);
		//存最近曲線
		TempBlock.nOffset = m_nNowOffset;

	    for (int i = 0; i < m_nCurveUse; i++)
	    {
	    	sprintf(sz, "%s%d%s","TC", i+1,".cdb");
	    	m_pCurveFile->Open(sz);
	    	ReadSavedData(0, pSwapTemper, m_nMaxCurvePointNumber*sizeof(INT16));
	    	m_pCurveFile->Close();

	    	for (int j = 0; j < TempBlock.nPointNumber; j++)
			{
				SwapTemper[j] = Temper.SaveTemp[j][i];
			}
			if(TempBlock.nOffset+TempBlock.nPointNumber >= m_nMaxCurvePointNumber)
			{
				memcpy(&pSwapTemper[TempBlock.nOffset],
							SwapTemper, 
							(m_nMaxCurvePointNumber - TempBlock.nOffset)*sizeof(INT16));
				memcpy(pSwapTemper,
							&SwapTemper[m_nMaxCurvePointNumber - TempBlock.nOffset], 
							(TempBlock.nOffset + TempBlock.nPointNumber - m_nMaxCurvePointNumber)*sizeof(INT16));
			    m_nNowOffset = TempBlock.nOffset + TempBlock.nPointNumber - m_nMaxCurvePointNumber;
	    	}
	    	else
	    	{
	    		memcpy(&pSwapTemper[TempBlock.nOffset], SwapTemper, TempBlock.nPointNumber*sizeof(INT16));
	    		m_nNowOffset = TempBlock.nOffset + TempBlock.nPointNumber;
	    	}

	    	pStorage->ReInit(ID_TEMP_CURVE1_FILE + i);
	    	pStorage->WriteBlock(0, m_nMaxCurvePointNumber*sizeof(INT16), (BYTE*)pSwapTemper);
	    	usleep(500);
	    }
	     
	    nBeginBlockIndex = m_nBeginBlockIndex;
	    nEndBlockIndex = m_nEndBlockIndex;
	    memcpy(BlockBuff, m_SaveBlockBuff, MAXCURVESEGMENT*sizeof(TEMPSAVEBLOCK));
	    AddOneBlock(BlockBuff, &TempBlock,&nBeginBlockIndex, &nEndBlockIndex);  
	    //  存溫度信息
	    //****
	    pStorage->ReInit(ID_TEMP_MESSAGE_FILE); 
	    pStorage->WriteBlock(0, sizeof(int), (BYTE*)&nBeginBlockIndex);
	    pStorage->WriteBlock(sizeof(int), sizeof(int), (BYTE*)&nEndBlockIndex);
	    pStorage->WriteBlock(sizeof(int)*2, sizeof(TEMPSAVEBLOCK)*MAXCURVESEGMENT, (BYTE*)BlockBuff);
	   
	    m_nNowOffset = nBakNowOffset;
	    
	    //存開機兩小時曲線
	    TempBlock.nOffset = m_nWarmOffset;
		for (int i = 0; i < m_nCurveUse; i++)
		{
			sprintf(sz, "%s%d%s","WC", i+1,".cdb");
	    	m_pCurveFile->Open(sz);
	    	ReadSavedData(0, pWarmSwapTemper, m_nWarmCurvePointNumber*sizeof(INT16));
	    	m_pCurveFile->Close();
	    	
	    	if(m_nWarmOffset < m_nWarmCurvePointNumber && m_bWarmSaveFlag)			//開始記錄加溫曲線時才將BUF中的點存入文件中
	    	{
	    	    for (int j = 0; j < TempBlock.nPointNumber; j++)
		        {
			        SwapTemper[j] = Temper.SaveTemp[j][i];
		        }
	    		if(TempBlock.nOffset+TempBlock.nPointNumber >= m_nWarmCurvePointNumber)
				{
					TempBlock.nPointNumber = m_nWarmCurvePointNumber - TempBlock.nOffset;
				}
				memcpy(&pWarmSwapTemper[TempBlock.nOffset], SwapTemper, TempBlock.nPointNumber*sizeof(INT16));
	    	}
	    	pStorage->ReInit(ID_WARM_CURVE1_FILE + i);
	    	pStorage->WriteBlock(0, m_nWarmCurvePointNumber*sizeof(INT16), (BYTE*)pWarmSwapTemper);
	    	usleep(500);
		}
	    
	    nBeginBlockIndex = 0;
	    memcpy(WarmBlockBuff, m_WarmBlockBuff, WARMCURVESEGMENT*sizeof(TEMPSAVEBLOCK));
	    if(m_bWarmSaveFlag)														////開始記錄加溫曲線時才將BUF中的信息存入文件中
	    {
	    	nEndBlockIndex = m_nWarmEndIndex+1;
	   		memcpy(&WarmBlockBuff[nEndBlockIndex] , &TempBlock, sizeof(TEMPSAVEBLOCK));
	   	}
	   	else
	   		nEndBlockIndex = m_nWarmEndIndex;
	    //****
	    //  存溫度信息
	    //****
	    pStorage->ReInit(ID_WARM_MESSAGE_FILE); 
	    pStorage->WriteBlock(0, sizeof(int), (BYTE*)&nBeginBlockIndex);
	    pStorage->WriteBlock(sizeof(int), sizeof(int), (BYTE*)&nEndBlockIndex);
	    pStorage->WriteBlock(sizeof(int)*2, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT, (BYTE*)WarmBlockBuff);
	    //****
	    //  存設定溫度
	    //****
	    BYTE *pawSet = (BYTE *)malloc(MAX_TEMPSETSAVED*sizeof(TEMPSET));
	    if(pawSet != NULL)
	    {
	    	memset(pawSet, 0, MAX_TEMPSETSAVED*sizeof(TEMPSET));
	    	ReadSram(m_nTempSetMessageAddr,sizeof(int), (BYTE *) &nNumber); 
	    	ReadSram(m_nTempSetMessageAddr+sizeof(int),MAX_TEMPSETSAVED*sizeof(TEMPSET), pawSet );
	
	    	pStorage->ReInit(ID_TEMP_SET_FILE);
	    	pStorage->WriteBlock(0, sizeof(int),(BYTE *)&m_nCurveUse);					//輸出現在正在使用的曲線有幾條。
	    	pStorage->WriteBlock(sizeof(int), sizeof(int), (BYTE *)&nNumber);
	    	
	    	int nTempSetSize = sizeof(TEMPSET)-(tmMAX_CH_TEMP-m_nCurveUse)*sizeof(INT16);
	    	for(int i=0; i<MAX_TEMPSETSAVED; i++)
	    	{
	    	    pStorage->WriteBlock(sizeof(int)*2+i*nTempSetSize,
	    	        nTempSetSize, pawSet+i*sizeof(TEMPSET));
	    	}
	    	free(pawSet);
	    }
	    delete []pSwapTemper;
        delete []pWarmSwapTemper;
	}
    
    if (m_bMounted)
    {
    	sleep(2);
	    pStorage->UMount();
	    SendMsg(MSG_TEMP_CURVE_SAVED, 0, 0, NULL);
	}
	else
	{
		 SendMsg(MSG_TEMP_CURVE_NODEVICE, 0, 0, NULL);
	}
	m_bMounted = FALSE;
	m_bMMcThread = FALSE;
  	//sem_post(&g_dbBlogThreadWait);
}

void	CTaskDBLog::ExistDir(char* pszPath)
{
	DIR*		dir;
	
	dir = opendir(pszPath);
	if (!dir) mkdir(pszPath, 777);
	closedir(dir);
}

void	CTaskDBLog::SetSpaceTime(int nSpaceTime)
{
	if(nSpaceTime >= 4 && nSpaceTime <= 10 && m_nNowSpaceTime != nSpaceTime)
	{
		SaveTempToFlash(m_bWarmSaveFlag);
		m_nNowSpaceTime 	= nSpaceTime;
		m_lTempNumber   	= 0;
    	m_wSaveIndex		= 0;
	}
}


int		CTaskDBLog::GetSpaceTime()
{
	return m_nNowSpaceTime;
}

void	CTaskDBLog::SaveToWarmCurve(char *cName)
{
	if(m_nWarmOffset >= m_nWarmCurvePointNumber || m_nWarmEndIndex+1 >= WARMCURVESEGMENT)
	{
		return ;
	}

	char			sz[64];
	TEMPSAVEBLOCK  	TempBlock;
	TEMPSAVEDATA    Temper;
	INT16           SwapTemper[ONEHOURPOINT];
	
	memset(sz, 0, sizeof(sz)/sizeof(char));
	memset(&TempBlock,0,sizeof(TEMPSAVEBLOCK));
	memset(&Temper,0,sizeof(TEMPSAVEDATA));
	memset(SwapTemper,0,sizeof(SwapTemper));
	ReadSram(m_nTempDataMessage, sizeof(TEMPSAVEBLOCK), (BYTE *)&TempBlock);
	ReadSram(m_nTempDataAddr, sizeof(TEMPSAVEDATA), (BYTE *)&Temper);
	TempBlock.nOffset = m_nWarmOffset;
	
	//寫曲線數據文件
	if(TempBlock.nOffset+TempBlock.nPointNumber >= m_nWarmCurvePointNumber)
	{
	    m_bNextWarmSaveFlag = FALSE;
		WriteSram(m_bNextWarmSaveFlagAddr, sizeof(BOOL), (BYTE *)&m_bNextWarmSaveFlag);		//滿了下次就不要保存了
		TempBlock.nPointNumber = m_nWarmCurvePointNumber - TempBlock.nOffset;
	}
	for (int i = 0; i < m_nCurveUse; i++)
	{
		for (int j = 0; j < TempBlock.nPointNumber; j++)
		{
			SwapTemper[j] = Temper.SaveTemp[j][i];
		}
		
		sprintf(sz, "%s%d%s",cName, i+1,".cdb");
		m_pCurveFile->Open(sz);
	    WriteSavedData(TempBlock.nOffset*sizeof(INT16), SwapTemper, TempBlock.nPointNumber*sizeof(INT16));
	    m_pCurveFile->Close();
	}
	
	m_nWarmOffset = TempBlock.nOffset + TempBlock.nPointNumber;
	
	//寫曲線信息
	memcpy(&m_WarmBlockBuff[++m_nWarmEndIndex] , &TempBlock, sizeof(TEMPSAVEBLOCK));
	
	//寫曲線信息文件
	m_pCurveFile->Open("WarmCurveInfo.cdb");
	WriteSavedData(0, &m_nWarmBeginIndex,sizeof(int));
	WriteSavedData(sizeof(int), &m_nWarmEndIndex,sizeof(int));
	WriteSavedData(sizeof(int)*2+m_nWarmEndIndex*sizeof(TEMPSAVEBLOCK), &m_WarmBlockBuff[m_nWarmEndIndex], sizeof(TEMPSAVEBLOCK));
	m_pCurveFile->Close();
}

void	CTaskDBLog::GetWarmCurveData(char *cName, UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog)
{
    GetCurveData(cName, Time, nSelectTemp, TimeSelect, pDataLog, TRUE);
}

int		CTaskDBLog::ReadSavedData(int offset, void *buf, int length)
{
	return m_pCurveFile->Read(offset, buf, length);
}

int		CTaskDBLog::WriteSavedData(int offset, void *buf, int length)
{
	return m_pCurveFile->Write(offset, buf, length);
}

void  	CTaskDBLog::GetTempCurveData(char *cName, UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog)
{
	GetCurveData(cName, Time, nSelectTemp, TimeSelect, pDataLog, FALSE);
}

void	CTaskDBLog::AddOneBlock(TEMPSAVEBLOCK *BlockBuff,TEMPSAVEBLOCK *pTempBlock,
							 int *pBeginBlockIndex, int *pEndBlockIndex)
{
	int				nTempIndex = 0;
    int				nTemp = 0;
	*pEndBlockIndex = *pEndBlockIndex+1 > MAXCURVESEGMENT-1 ? 0 : *pEndBlockIndex+1;
	memcpy(&BlockBuff[*pEndBlockIndex] , pTempBlock, sizeof(TEMPSAVEBLOCK));
	if(*pBeginBlockIndex == *pEndBlockIndex)
		*pBeginBlockIndex = *pBeginBlockIndex+1 > MAXCURVESEGMENT-1 ? 0 : *pBeginBlockIndex+1;

	nTempIndex = *pBeginBlockIndex;
	while(nTempIndex != *pEndBlockIndex)
	{
		if(BlockBuff[nTempIndex].nOffset+BlockBuff[nTempIndex].nPointNumber > m_nMaxCurvePointNumber)
		{
			if(BlockBuff[nTempIndex].nOffset <= m_nNowOffset ||
				BlockBuff[nTempIndex].nOffset+
				BlockBuff[nTempIndex].nPointNumber-
				m_nMaxCurvePointNumber > m_nNowOffset)
			{
				nTemp = m_nNowOffset - BlockBuff[nTempIndex].nOffset;
				if(nTemp < 0)
					nTemp += m_nMaxCurvePointNumber;
				BlockBuff[nTempIndex].nOffset = m_nNowOffset;
				BlockBuff[nTempIndex].StartTime += nTemp*BlockBuff[nTempIndex].nSpaceTime;
				BlockBuff[nTempIndex].nPointNumber -= nTemp;
				break;
			}
		}
		else
		{
			if(BlockBuff[nTempIndex].nOffset <= m_nNowOffset &&
				BlockBuff[nTempIndex].nOffset+
				BlockBuff[nTempIndex].nPointNumber > m_nNowOffset)
			{
				nTemp = m_nNowOffset - BlockBuff[nTempIndex].nOffset;
				BlockBuff[nTempIndex].nOffset = m_nNowOffset;
				BlockBuff[nTempIndex].StartTime += nTemp*BlockBuff[nTempIndex].nSpaceTime;
				BlockBuff[nTempIndex].nPointNumber -= nTemp;
				break;
			}
		}
		nTempIndex = nTempIndex+1 > MAXCURVESEGMENT-1 ? 0 : nTempIndex+1;
	}
	if(nTempIndex != *pEndBlockIndex)
		*pBeginBlockIndex = nTempIndex;
}
