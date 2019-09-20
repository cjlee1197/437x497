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
int       CTaskDBLog::m_bNextWarmSaveFlagAddr 	=  0;								// Sram�Ψӧi�D�U���}���ɬO�_�n�O�sSRAM�����I��[�Ŧ��u���h
int       CTaskDBLog::m_nTempStartAddr      	=  0;                               // Sram�Ω�s��ūת����a�}
int       CTaskDBLog::m_nTempDataMessage    	=  0;                               // Sram�s�񥻦��ū׫H�����a�}
int       CTaskDBLog::m_nTempDataAddr       	=  0;                               // Sram�Ω�s��ū׼ƾڪ��a�}   
int       CTaskDBLog::m_nTempSetMessageAddr 	=  0;                               // Sram�Ω�s��ū׳]�w�ȫH�����a�}
int       CTaskDBLog::m_nTempSetDataAddr    	=  0;                               // Sram�Ω�s��ū׳]�w�ȼƾڦa�}

UTIME     CTaskDBLog::m_RecentCloseTime     	=  0;
UTIME     CTaskDBLog::m_Time                    =  0;    
UTIME     CTaskDBLog::m_NextStartTime           =  0;
BOOL      CTaskDBLog::m_bFlashThread            =  FALSE;                                               
BOOL      CTaskDBLog::m_bMMcThread              =  FALSE;  

int       CTaskDBLog::m_nMaxCurvePointNumber    =  0;
int       CTaskDBLog::m_nWarmCurvePointNumber   =  0;
 int      CTaskDBLog::m_nCurveUse               =  0; 
BOOL			CTaskDBLog::m_bNextWarmSaveFlag					= FALSE;
BOOL			CTaskDBLog::m_bWarmSaveFlag						= FALSE;			//�O�_�O�s�[�Ŧ��u���Ч�
BOOL			CTaskDBLog::m_bWarmStart						= FALSE;			//�[�Ŧ��u�O�_�w�g�}�l
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

//	//���եνu�{
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
	    TEMPSAVEBLOCK  TempBlock;	                // ����sram�O�s�H�� 		
		m_lTempNumber++;
   	 	now = GetNowTime();
        if (m_lTempNumber == 1)
            m_nStartTime = now;
    
        //========
		// �˴��]�w�ūסA���ܤƫh�O�s�Jsram
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
		// �N�U�ثH���O�s�Jsram
		//======== 
		if (m_nCount == 0)
		{
			if(m_NextStartTime != 0 && now - m_NextStartTime > 0)
				//���v�C���ʦs���u�ƾڮɥi��|�ᥢ���I�A�Ѥ_�C���w�ɾ����~�t�ֿn�A�i��|��2~3���I
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
        
        WriteSram(m_nTempStartAddr, sizeof(UTIME), (BYTE *)&now);                   // �g�J�̷s�O�s�ɶ�
        WriteSram(m_nTempDataAddr + (m_nCount-1)*sizeof(m_TempSave[0]), sizeof(m_TempSave[0]), 
        		(BYTE *)&m_TempSave[m_wSaveIndex]); //�g�J�O�s�ƾ�
        WriteSram(m_nTempDataMessage, sizeof(TEMPSAVEBLOCK), (BYTE *)&TempBlock);   // �g�J����sram�O�s�H��
		m_wSaveIndex++;
		if(m_wSaveIndex >= MAX_TEMPRECORDSAVED)
			m_wSaveIndex = 0;

		if(!m_bWarmSaveFlag)				
		{
			if(m_nWarmEndIndex == 0)			//�ݨD�ܧ󬰦p�G�S���ūצ��u��Ʈɤ~�۰ʰO���A�_�h��ʰO���ɤ~�O��
			{
				nNewHeaterStatusOn = g_flHeaterStatus;
				if(nNewHeaterStatusOn != nOldHeaterStatusOn && nNewHeaterStatusOn != 0)			//�}���Z�q���Ĥ@���}
				{
					SaveTempToFlash(FALSE);							//�Nbuf�����ƾڼg�J��󤤨ϥ[�Ŧ��u�P��e���u�O���P�B�C
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
		
		if(m_nCount >= ONEHOURPOINT/4/15)			//sram�����u�s��15�����s���󤤡]sram�W���t�Ϊ����,�DFlash�^
		{
	    	if(!m_bMMcThread)
	    	    SaveTempToFlash(m_bWarmSaveFlag);
		}
	}
}

/*------------------------------------------------------------------------------------+
|  Function : ReadCurrentData(WORD TempRank)                                          |
|  Task     : ���X�@����������e��                                                    |
+-------------------------------------------------------------------------------------+
|  Call     : ReadCurrentData(TempRank)                                               |
|                                                                                     |
|  Parameter: TempRank: �����Ǹ�                                                      |
|                                                                                     |
|                                                                                     |
|  Return   : ��������e��                                                            |
+------------------------------------------------------------------------------------*/
void		CTaskDBLog::ReadCurrentData(WORD TempRank, int TimeSelect, TEMPSAVESELECT *pDataLog)
{
	GetTempCurveData("TC", GetCurrentStartTime(TimeSelect), TempRank, TimeSelect, pDataLog);
}

/*------------------------------------------------------------------------------------+
|  Function : ReadCurrentSetData(WORD TempRank)                                       |
|  Task     : ���X�@����������e�]�w��                                                |
+-------------------------------------------------------------------------------------+
|  Call     : ReadCurrentSetData(TempRank)                                            |
|                                                                                     |
|  Parameter: TempRank: �����Ǹ�                                                      |
|                                                                                     |
|                                                                                     |
|  Return   : ��������e�]�w��                                                        |
+------------------------------------------------------------------------------------*/
void		CTaskDBLog::ReadCurrentSetData(WORD TempRank, int TimeSelect, TEMPSETSELECT *pDataLog)
{
	GetSetData(GetCurrentStartTime(TimeSelect), TempRank, TimeSelect, pDataLog);
}
/*------------------------------------------------------------------------------------+
|  Function : GetRealStartTime()                                                      |
|  Task     : ���X��e�Ȫ��}�l�ɶ�                                                    |
+-------------------------------------------------------------------------------------+
|  Call     : GetCurrentStartTime()                                                   |
|                                                                                     |
|  Parameter:                                                                         |
|                                                                                     |
|                                                                                     |
|  Return   : ��e�Ȫ��}�l�ɶ�                                                        |
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
    //Ū��󤤪��I
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
    
    if(bWarm && !m_bWarmSaveFlag)							//�٥��}�l�O���[�Ŧ��u���ܴN���NBUF�����I��ܥX��
    {
    	pDataLog->nNumber = i;
    	return ;
    }
    //Ūsram�����I
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
|  Task     : ���X�Y�q�ū�MAX_TEMPSELECT���I�ɶ����]�w��                                           |
+---------------------------------------------------------------------------------------+
|  Call     : GetSetData(Time, nSelectTemp)                                             |
|                                                                                       |
|  Parameter: Time:           �}�l�ɶ�(�q1970�~�H�Ӫ����)                              |
|             nSelectTemp:    �ū׬q��                                                  |          
|  Return   : �]�w�ū׼ƾڤΨ��I�H��                                                    |
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
    if(tmpTime < pSet[i].Time)							//�}�l�B�i�঳�L�Ī��I
    {
    	pDataLog->Select[m].Time = tmpTime-Time;
    	pDataLog->Select[m].Temp = -1;
    	m++;
    	tmpTime = pSet[i].Time < EndTime ? pSet[i].Time : EndTime;
    	pDataLog->Select[m].Time = tmpTime-Time;
    	pDataLog->Select[m].Temp = -1;
    	m++;	
    }
    while(tmpTime < EndTime)								//�ھڮɶ����q���
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
|  Task     : �O�s���JMMC                                                             |
+---------------------------------------------------------------------------------------+
|  Call     : SaveToMMc()                                                               |
|                                                                                       |
|  Parameter:                                                                           |
|  Return   : �L                                                                        |
+--------------------------------------------------------------------------------------*/
void            CTaskDBLog::SaveToMMc()
{
    if (!m_bMMcThread)
    {
        pthread_create(&m_MMcThread, NULL, (PRSRUN)SaveFileToMMc, NULL);                 // �Nsram���Ѿl����Ƽg�Jflash��
        
        //sem_wait(&g_dbBlogThreadWait);
        //CtmUsbFile::GetInstance()->UMount();
    	//Delay(3000);
    	//Prompt(g_MultiLanguage["VM_DATA_READ"]);
    	
    }
        
    
}

/*--------------------------------------------------------------------------------------+
|  Function : GetSelectTime(int nHour)                                                  |
|  Task     : ����Y�Ӥp�ɪ��q1970�~�H�Ӫ����                                          |
+---------------------------------------------------------------------------------------+
|  Call     : GetSelectTime(nHour)                                                      |
|                                                                                       |
|  Parameter: nHour: �@�ѥH�Ӫ��Y�Ӥp��                                                 |
|  Return   : �L                                                                        |
+--------------------------------------------------------------------------------------*/
UTIME           CTaskDBLog::GetSelectTime(int nHour)
{
    UTIME now;
    now = GetNowTime();     
    //****
    //  ������p�ɼƤp��{�b���p�ɼơA�h������ɶ��O���Ѫ�
    //****
    if ((now%DAY)/HOUR >= nHour)
    {        
        return  (now - (now%DAY) + nHour*HOUR);
    }
    //****
    //  ������p�ɼƤj��{�b���p�ɼơA�h������ɶ��O�Q�Ѫ�
    //****
    else
    {        
        return (now - (now%DAY) - (24-nHour)*HOUR);
    }
}

/*--------------------------------------------------------------------------------------+
|  Function : GetNowTime()                                                              |
|  Task     : ����{�b�q1970�~�H�Ӫ����                                                |
+---------------------------------------------------------------------------------------+
|  Call     : GetNowTime()                                                              |
|                                                                                       |
|  Parameter:                                                                           |
|  Return   : �L                                                                        |
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
|  Task     : �M��sram���H���í��s�O���H��                                   |
+----------------------------------------------------------------------------+
|  Call     : Reset()                                                        |
|                                                                            |
|  Parameter:                                                                |
|  Return   : �L                                                             |
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
|  Task     : ��osram�����t���j�p                                           |
+----------------------------------------------------------------------------+
|  Call     : GetSramSize()                                                  |
|                                                                            |
|  Parameter:                                                                |
|  Return   : sram���j�p                                                     |
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
|  Task     : ��l�ƪ��@�Ǽƾڳ]�m                                          |
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
    
    //���t�Υe��7K,"WarmCurveInfo.cdb"�M"CurveInfo.cdb"�U�e1024�r�`,���t��blocksize=512,�o�X�H�U����
	m_nMaxCurvePointNumber = ((((nPos[1] - nPos[0] - 9*1024)/512)/(2*m_nCurveUse))*512)/2;
	m_nWarmCurvePointNumber = m_nMaxCurvePointNumber;

	delete []pszCurveInfo;
	for (int i = 0; i < tmMAX_CH_TEMP; i++)
	    delete []szID[i];
}
/*---------------------------------------------------------------------------+
|  Function : InitTempSavedSram()                                            |
|  Task     : ��l��sram                                                     |
+----------------------------------------------------------------------------+
|  Call     : InitTempSavedSram()                                            |
|                                                                            |
|  Parameter:                                                                |
|  Return   : �L                                                             |
+---------------------------------------------------------------------------*/

void       CTaskDBLog::InitTempSavedSram()
{                  
    UTIME           now;
    ReadSram(m_nTempStartAddr, sizeof(UTIME), (BYTE *)&m_RecentCloseTime);                   // Ū�X�W�������t�ήɶ�           
    now = GetNowTime();       
    InitTempSet(now);
    SaveTempToFlash(m_bNextWarmSaveFlag);											//�O�sBUF����d���I
    if(m_bNextWarmSaveFlag)												
    {
    	m_bNextWarmSaveFlag = FALSE;
		WriteSram(m_bNextWarmSaveFlagAddr, sizeof(BOOL), (BYTE *)&m_bNextWarmSaveFlag);		
	}
}

/*---------------------------------------------------------------------------+
|  Function : ClearTempHistoryRecord()                                       |
|  Task     : �M��sram���H����                                               |
+----------------------------------------------------------------------------+
|  Call     : ClearTempHistoryRecord()                                       |
|                                                                            |
|  Parameter:                                                                |
|  Return   : �L                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::ClearTempHistoryRecord()
{      
    int   nTotalLength		 = sizeof(BOOL) + sizeof(UTIME) + sizeof(TEMPSAVEBLOCK) + 
        sizeof(TEMPSAVEDATA) + sizeof(int) + sizeof(TEMPSET)*MAX_TEMPSETSAVED;
    char  *pZero = new char[nTotalLength];
    
    memset(pZero, 0, nTotalLength);
    WriteSram(m_bNextWarmSaveFlagAddr, nTotalLength, (BYTE *)pZero);                          // �M����ڷū׫H��
    
    m_pCurveFile->Format();                 //�M���Ҧ��ūצ��u���
    
    delete []pZero;
}

/*---------------------------------------------------------------------------+
|  Function : ClearTempDataSram()                                            |
|  Task     : �M��sram���ƾڰ�                                               |
+----------------------------------------------------------------------------+
|  Call     : ClearTempDataSram()                                            |
|                                                                            |
|  Parameter:                                                                |
|  Return   : �L                                                             |
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
|  Task     : �N�s�x�_sram���ƾګ����Jflash���A�íק�s�_sram��flash�s�x�H�� |
+----------------------------------------------------------------------------+
|  Call     : SaveTempToFile()                                               |
|                                                                            |
|  Parameter:                                                                |
|  Return   : �L                                                             |
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
|  Task     : �N�s�x�_sram���ƾګ����J��󤤡A�íק�s�_�s�x�H��             |
+----------------------------------------------------------------------------+
|  Call     : SaveSramTemp(cName)                                            |
|                                                                            |
|  Parameter: cName: ���W�r���������r�Ŧ�                                  |
|  Return   : �ū׼ƾڦs�x�H��                                               |
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
	
	//�g���u�ƾڤ��
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
	
	//�g���u�H�����
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
|  Task     : �q���s��Ū�X�@�q�ƾڼg��sram�Y������m                         |
+----------------------------------------------------------------------------+
|  Call     : ReadSram(nPosition, nLength, pSource                           |
|                                                                            |
|  Parameter: nPosition: sram����������m                                    |
              nLength:   �qsram��Ū���u����                                  |
|             pSource:   Ū�J�ƾڦb���s���s�x���a�}                          |
|  Return   : �L                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::WriteSram(int nPosition, int nLength, BYTE *pSource)
{
    CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_TEMPCURVE_SRAMFILE, 1, pSource, nLength, nPosition); 
}
 
/*---------------------------------------------------------------------------+
|  Function : ReadSram(int nPosition, int Length, BYTE *pDesti)              |
|  Task     : �qsram�Y������m�XŪ�X�@�q�ƾڨ줺�s��                         |
+----------------------------------------------------------------------------+
|  Call     : ReadSram(nPosition, nLength, pDesti)                           |
|                                                                            |
|  Parameter: nPosition: sram����������m                                    |
              nLength:   �qsram��Ū���u����                                  |
|             pDesti :   �V���s�g�J�ƾڭ��a�}                                |
|  Return   : �L                                                             |
+---------------------------------------------------------------------------*/
void       CTaskDBLog::ReadSram(int nPosition, int nLength, BYTE *pDesti)
{  
    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_TEMPCURVE_SRAMFILE, 1, pDesti, nLength, nPosition);
}

/*---------------------------------------------------------------------------+
|  Function : MakeTime(tmDATETIME *tm)                                       |
|  Task     : �N�H�~����ܪ��ɶ���Ƭ����q1970�~1��1��0��0��0��ܤ������  |
|             �Ӫ��                                                         |
+----------------------------------------------------------------------------+
|  Call     : MakeTime(tmDATETIME *tm)                                       |
|                                                                            |
|  Parameter: tm :�H�~���覡��ܪ����                                     |
|                                                                            |
|  Return   : �q1970�~1��1��0��0��0��ܤ������                              |
+---------------------------------------------------------------------------*/
UTIME      CTaskDBLog::MakeTime(tmDATETIME *tm)
{
	UTIME res;
	int   year;

	year = tm->date.year - 1970;
	res = YEAR*year + DAY*((year+1)/4);									//���ʦ~���D�|�~���D�w�g�L�k�Ҽ{�F
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
|  Task     : ��o�Y�ɶ���_���p�ɬq�s�x�ɪ���m                             |
+----------------------------------------------------------------------------+
|  Call     : GetStartNumber(Time)                                           |
|                                                                            |
|  Parameter: Time: �ɶ�                                                     |              
|  Return   : ���ɶ���_�Y�p�ɬq�s�x����m                                   |
+---------------------------------------------------------------------------*/
int        CTaskDBLog::GetStartNumber(UTIME Time)
{
    return ((Time%DAY)%HOUR)/TEMP_TIME_COUNTER;
}

/*---------------------------------------------------------------------------+
|  Function : InitTempSet(UTIME Time)                                        |
|  Task     : �}���ɹ�O�s���]�w�Ⱦާ@�A�N�W�L24�p�ɳ����ȯd�U�@��           |
+----------------------------------------------------------------------------+
|  Call     : InitTempSet(Time)                                              |
|                                                                            |
|  Parameter: Time: �ɶ�                                                     |              
|  Return   : �L                                                             |
+---------------------------------------------------------------------------*/
void      CTaskDBLog::InitTempSet(UTIME Time)
{
    int             nLast=0;             // �]�w�ū׼ƶq
    INT16           aSetTemp[tmMAX_CH_TEMP];
    TEMPSET         TempSetValue;        
    memset(aSetTemp,0,sizeof(INT16)*tmMAX_CH_TEMP);
    memset(&TempSetValue,0,sizeof(TEMPSET));
    ReadSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast);
    
    //****
    //   �L�ū׳]�w��
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
|  Task     : ��o�Y�ɶ��}�lMAX_TEMPSELECT�I�ɶ������]�w�ū�                            |
+----------------------------------------------------------------------------+
|  Call     : SetTemp(Time)                                                  |
|                                                                            |
|  Parameter: Time: �ɶ�                                                     |              
|  Return   : �L                                                             |
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
    //  ���o�{�b�ū׭�
    //****
    TempSetValue.Time  =  Time;
    for (i = 0; i < m_nCurveUse; i++)
    {
        aNowTemp[i] = (INT16)g_pDatabase->Read(m_nSetTempID[i]).wData; 
        TempSetValue.Temp[i] = aNowTemp[i];              
    }    
    //****
    //  ���o�̫�O�s�ū׳]�w��
    //****
    ReadSram(m_nTempSetMessageAddr, sizeof(int), (BYTE *)&nLast);
    ReadSram(m_nTempSetDataAddr+ sizeof(TEMPSET)*(nLast-1), sizeof(TEMPSET), (BYTE *)&LastSetTemp);    
    memcpy(aLastTemp, (char *)&LastSetTemp +sizeof(UTIME) , tmMAX_CH_TEMP*sizeof(WORD)); 
    
    //****
    //  �Y�����A�h�s�x�{�b�ū׳]�w��
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
|  Task     : �N�ū׼ƾګO�s�Jmmc�d��                                                   |
+---------------------------------------------------------------------------------------+
|  Call     : SaveFileToMMc()                                                           |
|                                                                                       |
|  Parameter:                                                                           |
|  Return   : �L                                                                        |
+--------------------------------------------------------------------------------------*/
void            CTaskDBLog::SaveFileToMMc()
{
	pthread_detach(pthread_self());
	
    m_bMMcThread = TRUE;

//****
    //  �s�ū׼ƾ�
    //****        
    m_bMounted = pStorage->Mount(FALSE);				//	fans add 2007/9/6 06:13�U��
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
		//�s�̪񦱽u
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
	    //  �s�ū׫H��
	    //****
	    pStorage->ReInit(ID_TEMP_MESSAGE_FILE); 
	    pStorage->WriteBlock(0, sizeof(int), (BYTE*)&nBeginBlockIndex);
	    pStorage->WriteBlock(sizeof(int), sizeof(int), (BYTE*)&nEndBlockIndex);
	    pStorage->WriteBlock(sizeof(int)*2, sizeof(TEMPSAVEBLOCK)*MAXCURVESEGMENT, (BYTE*)BlockBuff);
	   
	    m_nNowOffset = nBakNowOffset;
	    
	    //�s�}����p�ɦ��u
	    TempBlock.nOffset = m_nWarmOffset;
		for (int i = 0; i < m_nCurveUse; i++)
		{
			sprintf(sz, "%s%d%s","WC", i+1,".cdb");
	    	m_pCurveFile->Open(sz);
	    	ReadSavedData(0, pWarmSwapTemper, m_nWarmCurvePointNumber*sizeof(INT16));
	    	m_pCurveFile->Close();
	    	
	    	if(m_nWarmOffset < m_nWarmCurvePointNumber && m_bWarmSaveFlag)			//�}�l�O���[�Ŧ��u�ɤ~�NBUF�����I�s�J���
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
	    if(m_bWarmSaveFlag)														////�}�l�O���[�Ŧ��u�ɤ~�NBUF�����H���s�J���
	    {
	    	nEndBlockIndex = m_nWarmEndIndex+1;
	   		memcpy(&WarmBlockBuff[nEndBlockIndex] , &TempBlock, sizeof(TEMPSAVEBLOCK));
	   	}
	   	else
	   		nEndBlockIndex = m_nWarmEndIndex;
	    //****
	    //  �s�ū׫H��
	    //****
	    pStorage->ReInit(ID_WARM_MESSAGE_FILE); 
	    pStorage->WriteBlock(0, sizeof(int), (BYTE*)&nBeginBlockIndex);
	    pStorage->WriteBlock(sizeof(int), sizeof(int), (BYTE*)&nEndBlockIndex);
	    pStorage->WriteBlock(sizeof(int)*2, sizeof(TEMPSAVEBLOCK)*WARMCURVESEGMENT, (BYTE*)WarmBlockBuff);
	    //****
	    //  �s�]�w�ū�
	    //****
	    BYTE *pawSet = (BYTE *)malloc(MAX_TEMPSETSAVED*sizeof(TEMPSET));
	    if(pawSet != NULL)
	    {
	    	memset(pawSet, 0, MAX_TEMPSETSAVED*sizeof(TEMPSET));
	    	ReadSram(m_nTempSetMessageAddr,sizeof(int), (BYTE *) &nNumber); 
	    	ReadSram(m_nTempSetMessageAddr+sizeof(int),MAX_TEMPSETSAVED*sizeof(TEMPSET), pawSet );
	
	    	pStorage->ReInit(ID_TEMP_SET_FILE);
	    	pStorage->WriteBlock(0, sizeof(int),(BYTE *)&m_nCurveUse);					//��X�{�b���b�ϥΪ����u���X���C
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
	
	//�g���u�ƾڤ��
	if(TempBlock.nOffset+TempBlock.nPointNumber >= m_nWarmCurvePointNumber)
	{
	    m_bNextWarmSaveFlag = FALSE;
		WriteSram(m_bNextWarmSaveFlagAddr, sizeof(BOOL), (BYTE *)&m_bNextWarmSaveFlag);		//���F�U���N���n�O�s�F
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
	
	//�g���u�H��
	memcpy(&m_WarmBlockBuff[++m_nWarmEndIndex] , &TempBlock, sizeof(TEMPSAVEBLOCK));
	
	//�g���u�H�����
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
