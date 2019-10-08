/*===========================================================================+
|  Class    : CtmControl_Error	                                             |
|  Task     : Alarm Server Function	    	                     			 |
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
#include	"tmControl_Error.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"
#include	"../../tmconfig.h"
#include	"../../iotable.h"//Andy 20081028
#include	"../dsp54/tmTableG.h"
#include	"../libiNetComm.h"
#include	"../../tmshmsg.h"
#include 	"../../commonaction.h"
#include	"../libComm.h"
#ifdef	D_3354
#include	"../../filerecord.h"
#endif

IMPLEMENT_DYNCREATE(CtmControl_Error, CtmControl)

#define		ERRORLOOPTIMER 	300
#define		DRIVE_NUM		2
#define 	ERRORSTRMAX_NUM	10
#define		ERROR_STR_LEN	1024
/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
int		CtmControl_Error::m_nErrorCount = 0;
char*	CtmControl_Error::m_pszError[64] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL,
											NULL, NULL, NULL, NULL, NULL, NULL, NULL};
tmDB_ERROR		CtmControl_Error::m_MsgCurrent;
tmDB_ERROR		CtmControl_Error::m_MsgIndex;

BOOL	b_AlarmState = TRUE;
BOOL	b_PromptBar  = FALSE; ///Andy 20100723
int     lTempOld =0,lTempNew=0;
int     lMoldTempOld =0,lMoldTempNew=0;
BOOL	b_Error2 = FALSE;

//Caozz add 2011/3/28 上午 11:06:40
BOOL	b_AddTimer = FALSE;
WORD	u_wErrorIndex = 0;
long	l_lLoopTimer = ERRORLOOPTIMER;
char 	u_szErrorStr[ERROR_STR_LEN];
BOOL	u_bErrorChangeFlag = FALSE;

WORD	wMoldTempErrStrIndex = 0, wTempErrStrIndex = 0;
WORD	wMoldTempFlag = 0, wTempFlag = 0;
WORD	wOldErrorStrIndex[8] = {0,0,0,0,0,0,0,0};
int		iAlarmNum = 0;
/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::CtmControl_Error()				             |
|  Task     :  CtmControl_Errorr構造函數                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Error::CtmControl_Error(): CtmControl()
{
	m_ErrorListHead = (tmErrorInfo*)malloc(sizeof(tmErrorInfo));
	m_ErrorListHead->pNext = NULL;

	m_iRealIndex = 0xFFFFFFFF;
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::~CtmControl_Error()				             |
|  Task     :  CtmControl_Error析構函數                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Error::~CtmControl_Error()
{

}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Error::CreateSelf()
{
	//InitControl();
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::FreeSelf()				             	     |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Error::FreeSelf()
{
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::AddErrorLog(DWORD dwID, WORD	wSource)       |
|  Task     : Add  Error log        					     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL   CtmControl_Error::AddErrorLog(DWORD dwID, WORD	wSource)
{
	pthread_mutex_lock(&m_MutexErrMsg);
	int	nIndex = 0;

	m_MsgCurrent.OilError.dwID = dwID;
	m_MsgCurrent.OilError.dwShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue +1;
	GetDate(&m_MsgCurrent.OilError.dateStart);
	GetTime(&m_MsgCurrent.OilError.timeStart);
	m_MsgCurrent.OilError.dateFixed.year = 0;
	m_MsgCurrent.wSource		= wSource;			//James modify 2011/3/10 下午 07:10:18

	if(((dwID&0xFF00FFFF) != 0x67000103) ||(((dwID&0xFF00FFFF) == 0x67000103) &&(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED20").lValue == 1)))	/*2013-4-17 電力計的相位偏差警報顯示與否*/
	{
		if (m_MsgIndex.wCount < CtmConfig::GetInstance()->GetErrorMaxNum())
			m_MsgIndex.wCount++;

		nIndex = m_MsgIndex.wIndex + 1;
		#ifdef	D_3354
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ALARM, nIndex, &m_MsgCurrent, sizeof(m_MsgCurrent));
	    else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, nIndex, &m_MsgCurrent);
		#else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, nIndex, &m_MsgCurrent);
		#endif

		if ((++m_MsgIndex.wIndex) >= (CtmConfig::GetInstance()->GetErrorMaxNum()))
			m_MsgIndex.wIndex = 0;
		#ifdef	D_3354
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ALARM, 0, &m_MsgIndex, sizeof(m_MsgIndex));
	    else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, 0, &m_MsgIndex);
		#else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, 0, &m_MsgIndex);
		#endif

		SendMsg(MSG_USER_SH_ERROR_ADD, nIndex, m_iRealIndex, NULL);		//2009-5-22
		ChangeErrorList(dwID, wSource, 0, 1);
	}

	pthread_mutex_unlock(&m_MutexErrMsg);
	
	iAlarmNum++;/*2017-5-11 記錄警報個數*/

	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : BOOL    CtmControl_Error::ClearErrorLog()               	     |
|  Task     : Clear Error log        					     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_Error::ClearErrorLog()
{
	pthread_mutex_lock(&m_MutexErrMsg);
	if (g_pMainFrame->GetPromptBar()->GetErrorCount() <= 0)
	{
		m_MsgIndex.wIndex =
		m_MsgIndex.wCount = 0;
		#ifdef	D_3354
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ALARM, 0, &m_MsgIndex, sizeof(m_MsgIndex));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE,0, &m_MsgIndex);
		#else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE,0, &m_MsgIndex);
		#endif
	}
	else
	{
		MsgBox(g_MultiLanguage["VM_ALARM_NOFIX"], tmFT_CODE_TECH);
	}
	pthread_mutex_unlock(&m_MutexErrMsg);

	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::Destroy_ErrorLog()               	     	 |
|  Task     : Destory Control   						     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_Error::Destroy_ErrorLog()
{
	pthread_mutex_destroy(&m_MutexErrMsg);
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::Error_Control()               	     		 |
|  Task     : Error Control   						     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void   	CtmControl_Error::Error_Control()
{
	if(0 == CtmConfig::GetInstance()->GetErrorControl())	//Caozz 2011/4/7 上午 10:10:03
	{
	 	Error_Control_Old();
	}
	else
	{
		Error_Control_New();
	}
}

void	CtmControl_Error::Error_Control_Card()
{
	DWORD	dwErrorStrIndex = 0; //dwID	 = 0, dwIndex = 0;
	static	DWORD dwOldErrorID[8] = {0};
	DWORD	dwNewErrorID = 0;
	int iErrNum =0;
	char*	pszErrorID[] = 
	{	
		"MACHINE_INTERFACE_ERROR_ERROR1_DWALL",
	};
	
	///if(CtmConfig::GetInstance()->GetMachineAxis(9)&&0x4)
	///	iErrNum = sizeof(pszErrorID)/sizeof(char*);
	///else
		iErrNum = sizeof(pszErrorID)/sizeof(char*);
	
	for(int i = 0; i < iErrNum; i++)
	{
		dwNewErrorID = GetDBValue(pszErrorID[i]).lValue;
		if(dwNewErrorID != dwOldErrorID[i])
		{
			printf("Error_Control_Card dwNewErrorID=%x dwOldErrorID[%d]=%x\n", dwNewErrorID, i, dwOldErrorID[i]);
			if((dwNewErrorID & 0xFFFF) == 0)
			{
				dwErrorStrIndex =  dwOldErrorID[i];
    			FixErrorLog((DWORD)dwErrorStrIndex, tmM3_ERRORFLAG);
    			SendMsg(MSG_USER_SH_PROMPTNULL, 0, 0, NULL);	
    			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED3", 0);			
			}
			else
			{
				//Prompt(g_MultiLanguage[wErrorStrIndex],1);
				if(dwOldErrorID[i] != 0)
				{
					FixErrorLog((DWORD)dwOldErrorID[i], tmM3_ERRORFLAG);					
				}
				dwOldErrorID[i] = dwNewErrorID;
				dwErrorStrIndex = dwNewErrorID;
				AddErrorLog((DWORD)dwErrorStrIndex, tmM3_ERRORFLAG);
				DWORD dwPickerErrorStrIndex = (dwErrorStrIndex & 0xFF000000)>>24;
			    if(dwPickerErrorStrIndex==11)
			    	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED3", 2);
			    else 
			    	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED3", 0);
			}			
			dwOldErrorID[i] = dwNewErrorID;
		}
	}
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::Error_Control_Old()           	     		 |
|  Task     : Error Control   						     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void   	CtmControl_Error::Error_Control_Old()
{
	short		i;
	WORD		cIndex = tmERROR_NULL;
	int 		RealIndex;
	WORD		wErrorFlag;	//wCoreErrorFlag;
	WORD		wErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY"); //JOYCE2008-11-7 給初始值
	WORD		wTempErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	DWORD		dwID, dwIndex;	// dwValue;

	int			ERR_BEGIN				= g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");
	int			ERR_0E_PLSCLSDOOR		= g_MultiLanguage.GetStrKeyIndex("ERR_0E_PLSCLSDOOR");
	int			ERR_0D_PLSOPNDOOR		= g_MultiLanguage.GetStrKeyIndex("ERR_0D_PLSOPNDOOR");

	dwID = g_pDatabase->GetDataID("STATE_ALARM_ON_STEP1_ERRORDATA");
	//James add HTK530 ---28警報字串顯示
	long long lDsp28ErrorID = GetDBValue("SYS_HMI_NULL_NULL_RESERVED40").lValue;
	int		  DSP28ERR_BEGIN	= g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR2");
	if(lDsp28ErrorID > 0)
	{
		Prompt(g_MultiLanguage[DSP28ERR_BEGIN+lDsp28ErrorID],1); //標準之錯誤字串顯示
	}

	for (i=tmMAX_OP_ERRORSAVE+1; i>=0; i--)		//JOYCE2009-2-10 tmMAX_OP_ERRORSAVE-1-->tmMAX_OP_ERRORSAVE+1
    {
    	if(i==14)  continue;  //JOYCE2009-2-10 add
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if ((wErrorFlag = GetDBValue(dwIndex).lValue))	//assignment and !zero;
        {
        	cIndex  = (char)i*16 + (char)_ScanBitR(wErrorFlag);  // Scan the 1's bit from left to right (Reverse)
        	RealIndex = (int)cIndex;		//用RealIndex傳遞Assign!!
        	m_iRealIndex = RealIndex;
        	if(g_pExTable != NULL)
        		wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&RealIndex,&dbeflg); //再用m_iRealIndex得到反饋結果(優先級)
        	//wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult); //再用iResult得到反饋結果(優先級)
        	//printf("result: %d,RealIndex: %d\n",iResult,RealIndex);
        	//優先級最低，則需要繼續掃描後面在WORD或BIT！
        	if(RealIndex == 0)
        		{
       				WORD wIndex1,wIndex2;
       				//一個從左邊掃描，一個從右邊掃描！！
       				wIndex1 = _ScanBitF(wErrorFlag);
       				wIndex2 = _ScanBitR(wErrorFlag);
       				//如果相等代表此WORD隻有一個BIT為ON，繼續掃描；若不等則有多個BIT，以後者為主
       				if(wIndex1 == wIndex2) continue;
       				else
       				{
       					_ClearBit(wErrorFlag,wIndex2);
       					cIndex = (char)i*16 + (char)wIndex1;
       					RealIndex = (int)cIndex;		//用iResult傳遞Assign!!
       					m_iRealIndex = RealIndex;
        				wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&RealIndex,&dbeflg); //再用iResult得到反饋結果(優先級)
       					break;
       				}
        		}
        	else	break;
        }
    }

	if (cIndex != tmERROR_NULL)
    {
    	if(b_AlarmState)	//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
			g_wErrorStatus = TRUE;

    	if (dbbuff.cIndexError != cIndex)
        {
        	if (dbbuff.cIndexMode != tmMODE_AUTOADJMOLD)
            {
            	if ((dbbuff.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)))
                {
	                dbbuff.bDelayedSpeech = TRUE;
	                dbbuff.lDelayedCount  = SPEECH_DELAYEDCOUNT;
	                dbbuff.bRepeatSpeech  = FALSE;
                }
            	else
                {
	                dbbuff.bDelayedSpeech = FALSE;
	                //::Speech(cIndex);
	                if ((dbbuff.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0D_PLSOPNDOOR - ERR_BEGIN)))
	                    dbbuff.bRepeatSpeech  = FALSE;
	                else
                    {
                    	dbbuff.bRepeatSpeech  = TRUE;
                    	dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;
                    }
                }
            }
			Prompt(g_MultiLanguage[wErrorStrIndex],1);  //標準之錯誤字串顯示
        	dbbuff.cIndexError = cIndex;
        }
    	else  //dbbuff.cIndexError != cIndex
        {
        	if ((dbbuff.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)) && (dbbuff.bDelayedSpeech) && (dbbuff.lDelayedCount <= 0))
            {
	            //::Speech(cIndex);
	            dbbuff.bDelayedSpeech = FALSE;
	            dbbuff.bRepeatSpeech  = TRUE;
	            dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;
            }
        	else if ((dbbuff.bRepeatSpeech) && (dbbuff.lRepeatCount <= 0))
            {
	            //::Speech(cIndex);
	            dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;
            }
	       	{
	        	Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示
	       	}
        	dbbuff.cIndexError = cIndex;
        }
    }
	else  //cIndex==ERROR_NULL
    {

	    g_wErrorStatus = FALSE;
	    if (dbbuff.cIndexError != cIndex)
        {
	        Prompt(g_MultiLanguage[wErrorStrIndex]);
	        g_pMainFrame->GetCurrentView()->OnLoseFocus();  //2009-8-26, add, 空警報
            g_pMainFrame->GetCurrentView()->OnGetFocus();
	        dbbuff.cIndexError = cIndex;
	        b_PromptBar = TRUE; ///Andy 20100723
        }
        else ///Andy 20100723
      	{
      		if(b_PromptBar)
      			{
      				SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF, NULL);
      				b_PromptBar = FALSE;
      			}
      	}
    }

	for (i = 0; i < (tmMAX_OP_ERRORSAVE+2); i++)	//JOYCE2009-2-10 tmMAX_OP_ERRORSAVE-->tmMAX_OP_ERRORSAVE+2
    {
 	   if(i==14)  continue;  //JOYCE2009-2-10 add
 	    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);

     	if((i == 0) && (GetDBValue(dwIndex).lValue == 1) && (g_pExTable->GetTableValue(0,"D_BLOW")))		//2010-12-15 記錄溫度與模溫分段警報
     	{
	   		if(GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue & 0x0008)
	    		{
          		lMoldTempNew = GetMoldTemperErrorStatus();
             	if(lMoldTempOld !=lMoldTempNew)
             	{
          	         cIndex = 0;
          	         RealIndex=cIndex + i*16;
	      	 		m_iRealIndex = RealIndex;
	      	 		wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&m_iRealIndex,&dbeflg);

          	         AddErrorLog((DWORD)wErrorStrIndex);

             	     	if(lMoldTempOld != 0)
             	      	{
             	       		wTempErrorStrIndex = wErrorStrIndex + lMoldTempOld - lMoldTempNew;
             	       		FixErrorLog((DWORD)wTempErrorStrIndex);
             	      	}
             	 		lMoldTempOld =lMoldTempNew;
             	}
	       	}
          else
          {
          		lTempNew     = GetTemperErrorStatus();
          		if(lTempOld !=lTempNew)
          		{
        	        cIndex = 0;
        	        RealIndex=cIndex + i*16;
	    			m_iRealIndex = RealIndex;
	    			wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&m_iRealIndex,&dbeflg);
        	        AddErrorLog((DWORD)wErrorStrIndex);

           	    	if(lTempOld != 0)
           	     	{
           	      		wTempErrorStrIndex = wErrorStrIndex + lTempOld - lTempNew;
           	      		FixErrorLog((DWORD)wTempErrorStrIndex);
           	     	}
           			lTempOld =lTempNew;
           	}
          }
     	}
      else  //標準警報
      {
 	    	if ((wErrorFlag = dbeflg.wErrorFlag[i] ^ GetDBValue(dwIndex).lValue))
 	      {
 	        	dbeflg.wErrorFlag[i] = GetDBValue(dwIndex).lValue; //第一次發生進來會先存入,做為第二次還原時間(FixErrorLog)之用
 	        	while (wErrorFlag)            //多個ERROR同時發生要全部都紀錄!
 				{
 					//g_tmDebugInfo->PrintDebugInfo("Error Index1 = 619\n");
 		            cIndex = (char)_ScanBitF(wErrorFlag);
 		            RealIndex=cIndex + i*16;  //先記憶體存住,可免相同使用之判斷式都重算
 		            //printf("RealIndex=%d g_wErrorStatus=%d b_AlarmState=%d \n", RealIndex, g_wErrorStatus, b_AlarmState);
 					m_iRealIndex = RealIndex;	//iResult = (int)RealIndex;
 					if(g_pExTable != NULL)
 						wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&RealIndex,&dbeflg);
 					//printf("result: %d,RealIndex: %d\n",iResult,RealIndex);
 					if(RealIndex <= 1)
 	            	{
 		                wErrorFlag = _ClearBit(wErrorFlag, cIndex); //先清掉該BIT再繼續
 		                g_wErrorStatus = FALSE;
 		                b_AlarmState = FALSE;
 		                continue;      //不存入紀錄檔的例外狀況
 	            	}
 	            	else			//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
 	            	{
 		            	g_wErrorStatus = TRUE;
 		            	b_AlarmState = TRUE;
 	            	}

 		            if (_TestBit(GetDBValue(dwIndex).lValue, cIndex))
 		            {
 		            	Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示
 		            	AddErrorLog((DWORD)wErrorStrIndex);
 		            }
 		            else
 	            	{
 		            	FixErrorLog((DWORD)wErrorStrIndex);
 		            	g_wErrorStatus_Dsp28 = FALSE;	//	clear the dsp28 error
 	            	}
 	            	wErrorFlag = _ClearBit(wErrorFlag, cIndex);
 				  }
 				}
 			}
		}

	WORD	wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD	wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	if((wDSPLIBDaysCode>=0x0707)&&(wDSPLIBYearCode==0x2010)||(wDSPLIBYearCode>=0x2011))
	{
		Error_Control2_Old();	//2013-9-17, for Error2
	}
}

void   	CtmControl_Error::Error_Control2_Old()	//2013-4-9, for Error2
{
	short		i = 0;
	WORD		cIndex = 0xFFFF, RealIndex = 0;
	WORD		wErrorFlag = 0;
	WORD		wErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	DWORD		dwID = 0, dwIndex = 0;
	int			iResult = 0;

	dwID = g_pDatabase->GetDataID("STATE_ALARM2_ON_STEP1_ERRORDATA");

	for (i = 0; i < 8; i++)
	{
	 	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
	 	if ((wErrorFlag = GetDBValue(dwIndex).lValue) >= 0x100)
	 		{
	        	b_Error2 = TRUE;	//fuxy, 2010-10-9
	        	cIndex  = GetDBValue(dwIndex).lValue;  // Scan the 1's bit from left to right (Reverse)
	        	m_iRealIndex = (int)cIndex;		//用iResult傳遞Assign!!
	        	iResult = m_iRealIndex + 0x10000000;//以表示error2
	        	wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg); //再用iResult得到反饋結果(優先級)
	 		}
	 	else break;
	}
		//show error
	if (dbbuff.cIndexError == tmERROR_NULL)//stat.error沒有警報時才show error2,否則兩個警報會來回閃動
	{
		if (cIndex != 0xFFFF)
	    {
	    	if(b_AlarmState)	//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
				g_wErrorStatus = TRUE;
	     	Prompt(g_MultiLanguage[wErrorStrIndex],1);
	     	dbbuff.cIndexError2 = cIndex;
	    }
		else  //cIndex==ERROR_NULL
	    {
		   if(b_Error2)/*JOYCE2011-11-24 不加此條件,導致bug:面板上顯示有警報,iNet的機器群覽畫面,沒有顯示警報狀態*/
		   	 g_wErrorStatus = FALSE;
		    b_Error2 = FALSE;		//fuxy, 2010-10-9
		    if (dbbuff.cIndexError2 != cIndex)
	        {
		        Prompt(g_MultiLanguage[wErrorStrIndex]);
		        g_pMainFrame->GetCurrentView()->OnLoseFocus();
		        g_pMainFrame->GetCurrentView()->OnGetFocus();
		        dbbuff.cIndexError2 = cIndex;
						b_PromptBar = TRUE; //JOYCE2010-10-22
	        }
      else //JOYCE2010-10-22
      	{
      		if(b_PromptBar)
      			{
     				g_pMainFrame->GetPromptBar()->ShowAlarm(FALSE);		//fuxy, 2012-1-12, for  警報紅條
     				SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF, NULL);
      				b_PromptBar = FALSE;
      			}
      	}
	    }
	}

		//record error
	for (i = 0; i < 8; i++)//改為掃描8個WORD
    {
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	wErrorFlag = dbeflg.wErrorFlag2[i] ^ GetDBValue(dwIndex).lValue;
    	if (wErrorFlag)
       	{
       		dbeflg.wErrorFlag2[i] = GetDBValue(dwIndex).lValue; //第一次發生進來會先存入,做為第二次還原時間(FixErrorLog)之用
        	RealIndex=wErrorFlag;  //先記憶體存住,可免相同使用之判斷式都重算
					iResult = (int)RealIndex+0x10000000;//以表示error2
					m_iRealIndex = RealIndex;
					wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
					//printf("result: %d,RealIndex: %d dbeflg.wErrorFlag2[%d ]=%d dwIndex=%d \n",iResult,RealIndex , i, dbeflg.wErrorFlag2[i], dwIndex);
					if(iResult <= 1)
          {
	            g_wErrorStatus = FALSE;
	            b_AlarmState = FALSE;
	            continue;      //不存入紀錄檔的例外狀況
          }
          else			//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
          {
	        	g_wErrorStatus = TRUE;
	        	b_AlarmState = TRUE;
          }

	        if (GetDBValue(dwIndex).lValue)
	        {
	        	Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示
	        	AddErrorLog((DWORD)wErrorStrIndex);
	        }
	        else
          {
	        	FixErrorLog((DWORD)wErrorStrIndex);
         }
				}
				//JOYCE2010-10-26 else break;
		}
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::Error_Control_New()           	     		 |
|  Task     : Error Control   						     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

void   	CtmControl_Error::Error_Control_New()
{
	short		i = 0;
	WORD		cIndex = tmERROR_NULL;
	int 		RealIndex = 0;
	WORD		wErrorFlag = 0;	//wCoreErrorFlag;
	WORD		wErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY"); //JOYCE2008-11-7 給初始值
	DWORD		dwID = 0, dwIndex = 0, dwValue = 0;
	WORD		wTempErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	BOOL		bSPdoor=FALSE,bSPbackdoor=FALSE,bSPcoreLoc=FALSE,bSPcorePos=FALSE,bSProbot=FALSE;
	int			iResult = 0;

	int			ERR_BEGIN				= g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");
	int			ERR_0E_PLSCLSDOOR		= g_MultiLanguage.GetStrKeyIndex("ERR_0E_PLSCLSDOOR");
	int			ERR_0D_PLSOPNDOOR		= g_MultiLanguage.GetStrKeyIndex("ERR_0D_PLSOPNDOOR");

	dwID = g_pDatabase->GetDataID("STATE_ALARM_ON_STEP1_ERRORDATA");
	//James add HTK530 ---28警報字串顯示
	long long lDsp28ErrorID = GetDBValue("SYS_HMI_NULL_NULL_RESERVED40").lValue;
	int		  DSP28ERR_BEGIN	= g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR2");
	if(lDsp28ErrorID > 0)
	{
		Prompt(g_MultiLanguage[DSP28ERR_BEGIN+lDsp28ErrorID],1); //標準之錯誤字串顯示
	}

	for (i=tmMAX_OP_ERRORSAVE+1; i>=0; i--)		//JOYCE2009-2-10 tmMAX_OP_ERRORSAVE-1-->tmMAX_OP_ERRORSAVE+1
    {
    	if(i==14)  continue;  //JOYCE2009-2-10 add
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if((i == 15) && (GetDBValue(dwIndex).lValue & 0x8000)) continue;/*20140124 ERROR+15的BIT15要跳過去,DSP(lib 20130927)把它作為 驅動器溫度狀態的warning flag*/

    	if ((wErrorFlag = GetDBValue(dwIndex).lValue))	//assignment and !zero;
        {
        	cIndex  = (char)i*16 + (char)_ScanBitR(wErrorFlag);  // Scan the 1's bit from left to right (Reverse)
        	RealIndex = (int)cIndex;		//用RealIndex傳遞Assign!!
        	m_iRealIndex = RealIndex;

        	if((g_pExTable->GetTableValue(0,"D_HALFTEMPSETPERCENT")) || (g_pExTable->GetTableValue(0,"D_7EY")))//SBJ 2014-4-9 9:11:47
        	{
        		if(m_iRealIndex ==109)
        		{
        			if(GetDBValue("MACH_TMP1_HEATERON_NULL_PERCENT").lValue)
        				SetDBValue("MACH_TMP1_HEATERON_NULL_PERCENT", 0);
        		}
        	}

        	if(g_pExTable != NULL)
        		wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&RealIndex,&dbeflg); //再用m_iRealIndex得到反饋結果(優先級)
        	//wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult); //再用iResult得到反饋結果(優先級)
        	//printf("result: %d,RealIndex: %d\n",iResult,RealIndex);
        	//優先級最低，則需要繼續掃描後面在WORD或BIT！
        	if(RealIndex == 0)
        		{
       				WORD wIndex1,wIndex2;
       				//一個從左邊掃描，一個從右邊掃描！！
       				wIndex1 = _ScanBitF(wErrorFlag);
       				wIndex2 = _ScanBitR(wErrorFlag);
       				//如果相等代表此WORD隻有一個BIT為ON，繼續掃描；若不等則有多個BIT，以後者為主
       				if(wIndex1 == wIndex2) continue;
       				else
       				{
       					_ClearBit(wErrorFlag,wIndex2);
       					cIndex = (char)i*16 + (char)wIndex1;
       					RealIndex = (int)cIndex;		//用iResult傳遞Assign!!
       					m_iRealIndex = RealIndex;
        				wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&RealIndex,&dbeflg); //再用iResult得到反饋結果(優先級)
       					break;
       				}
        		}
        	else	break;
        }
    }

	if (cIndex != tmERROR_NULL)
    {
    	if(b_AlarmState)	//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
			g_wErrorStatus = TRUE;

    	if (dbbuff.cIndexError != cIndex)
        {
        	if (dbbuff.cIndexMode != tmMODE_AUTOADJMOLD)
            {
            	if ((dbbuff.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)))
                {
	                dbbuff.bDelayedSpeech = TRUE;
	                dbbuff.lDelayedCount  = SPEECH_DELAYEDCOUNT;
	                dbbuff.bRepeatSpeech  = FALSE;
                }
            	else
                {
	                dbbuff.bDelayedSpeech = FALSE;
	                //::Speech(cIndex);
	                if ((dbbuff.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0D_PLSOPNDOOR - ERR_BEGIN)))
	                    dbbuff.bRepeatSpeech  = FALSE;
	                else
                    {
                    	dbbuff.bRepeatSpeech  = TRUE;
                    	dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;
                    }
                }
            }

			if(m_nErrorCount == 0)
				Prompt(g_MultiLanguage[wErrorStrIndex],1);  //標準之錯誤字串顯示
			dbbuff.cIndexError = cIndex;
        }
    	else  //dbbuff.cIndexError != cIndex
        {
        	if ((dbbuff.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)) && (dbbuff.bDelayedSpeech) && (dbbuff.lDelayedCount <= 0))
            {
	            //::Speech(cIndex);
	            dbbuff.bDelayedSpeech = FALSE;
	            dbbuff.bRepeatSpeech  = TRUE;
	            dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;
            }
        	else if ((dbbuff.bRepeatSpeech) && (dbbuff.lRepeatCount <= 0))
            {
	            //::Speech(cIndex);
	            dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;
            }
        	if(m_nErrorCount == 0)
						Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示

        	dbbuff.cIndexError = cIndex;
        }
    }
	else  //cIndex==ERROR_NULL
    {
    	g_wErrorStatus = FALSE;
	    if (dbbuff.cIndexError != cIndex)
        {
	        Prompt(g_MultiLanguage[wErrorStrIndex]);

	        g_pMainFrame->GetCurrentView()->OnLoseFocus();  //2009-8-26, add, 空警報
            g_pMainFrame->GetCurrentView()->OnGetFocus();
	        dbbuff.cIndexError = cIndex;
	        b_PromptBar = TRUE; ///Andy 20100723
        }
        else ///Andy 20100723
      	{
      		if(b_PromptBar)
      			{
      				if(m_nErrorCount == 0)
      				{
      					SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF, NULL);
      					///printf("11111 m_nErrorCount:%d   PromptBar:%d\n",m_nErrorCount,b_PromptBar);
      				}
      				b_PromptBar = FALSE;
      			}
      	}
    }

	WORD	wDSPLIBDaysCode = GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD	wDSPLIBYearCode = GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	for (i = 0; i < (tmMAX_OP_ERRORSAVE+2); i++)	//JOYCE2009-2-10 tmMAX_OP_ERRORSAVE-->tmMAX_OP_ERRORSAVE+2
    {
    	//if(i==14)  continue;  //JOYCE2009-2-10 add
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if((i == 15) && (GetDBValue(dwIndex).lValue & 0x8000)) continue;/*20140124 ERROR+15的BIT15要跳過去,DSP(lib 20130927)把它作為 驅動器溫度狀態的warning flag*/

		if(!(g_pExTable->GetTableValue(0,"D_TP2MTP")||g_pExTable->GetTableValue(0,"D_MTPFC")||g_pExTable->GetTableValue(0,"D_TPMTP")	/* 無料溫全部采用模溫控製(藍力壓機)*/  //模溫作料溫 或料溫作模溫  /*泰瑞部分料溫當模溫*/
		||g_pExTable->GetTableValue(0,"D_MTP2TP")||g_pExTable->GetTableValue(0,"D_7ZTMOLDTP")||g_pExTable->GetTableValue(0,"D_9TPMTP")))
		{
			if(((wDSPLIBDaysCode < 0x4703) &&(wDSPLIBYearCode==0x2014)|| (wDSPLIBYearCode <= 0x2013))||((GetDBValue("CONF_CODE_WRITE_NULL_DATA15").lValue & 0xFF00) ==0x7200))  //7900 LIB 4703, 7100 LIB 4704, 7200 按舊方式處理
			{
				if(i == 0)//JOYCE2012-3-30 循環警報方式bug修正:當ER0_TEMP為1,1C8如果有變化(由0變為8;由8變為0),則會出現無法消除警報'模溫偏差'或'溫度偏差'
				{
					dwValue = GetDBValue("STATE_ALARM_ON_STEP1_ERRORDATA").lValue;
					//printf("dwValue=%d\n",dwValue);
				    if(dwValue & 0x0001)  //ER0_TEMP  .SET     0   ; '溫度偏差      '
				    {
						dwValue = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
				  		if(dwValue & 0x0008)
				  		{
				  			//printf("iwTempFlag=%d\n",wTempFlag);
				  			if(wTempFlag == 0)
				  			{
						  		if(g_pExTable->GetTableValue(0,"D_MTP2TP")) /*ZHBA 2012-04-23*/
						  			wMoldTempErrStrIndex = g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");//溫度偏差
						  		else if(g_pExTable->GetTableValue(0,"D_CHRGMLDTP")) //Dongxl.2013-6-3 11:54:13
						  			wMoldTempErrStrIndex = g_MultiLanguage.GetStrKeyIndex("ERR_CHARGE_TEMPERR");//海達特殊機要求模溫偏差顯示為儲料溫度偏差
						  		else
				  					wMoldTempErrStrIndex = g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR2");//模溫偏差

				  				wMoldTempFlag = 1;
				  			}
				  			else	if(wTempFlag == 1)
				  			{
				  				wTempFlag = 2;
				  			}
				  		}
				  		else
				  		{
				  			if(wMoldTempFlag == 0)
				  			{
					  			wTempErrStrIndex = g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");//溫度偏差
					  			wTempFlag = 1;
				  			}
				  			else	if(wMoldTempFlag == 1)
				  			{
				  				wMoldTempFlag = 2;
				  			}
				  		}
				  	}
				    else
				  	{
				  		if(wTempFlag == 1)
				  		{
				  			wTempFlag = 2;
				  		}
				  		if(wMoldTempFlag == 1)
				  		{
				  			wMoldTempFlag = 2;
				  		}
				  	}
				}
			}
		}

    	if(i == 14)
    	{
    		wErrorFlag = dbeflg.wErrorFlag[i] ^ GetDBValue(dwIndex).lValue;
    		if(wErrorFlag&0x0020)//安全門不記錄,發生變化
    		{
    			if(bSPdoor)
    			{
    				dbeflg.wErrorFlag[0] &= 0xFFFD;
    				iResult = 1+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
    				if(iResult == 10)
    				{
    					FixErrorLog((DWORD)wErrorStrIndex);
    				}
    			}
    			if(bSPbackdoor)
    			{
    				dbeflg.wErrorFlag[3] &= 0xBFFF;
    				iResult = 62+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
    				if(iResult == 10)
    				{
    					FixErrorLog((DWORD)wErrorStrIndex);
    				}
    			}
    		}
    		if(wErrorFlag&0x001F)
    		{
    			if(bSPcoreLoc)
    			{
    				dbeflg.wErrorFlag[1] &= 0xF7FF;
    				iResult = 27+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
    				if(iResult == 10)	FixErrorLog((DWORD)wErrorStrIndex);
    			}
    			else	if(bSPcorePos)
    			{
    				dbeflg.wErrorFlag[2] &= 0xFFF7;
    				iResult = 35+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
    				if(iResult == 10)	FixErrorLog((DWORD)wErrorStrIndex);
    			}
    			else	if(bSProbot)
    			{
    				dbeflg.wErrorFlag[2] &= 0xFDFF;
    				iResult = 41+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
    				if(iResult == 10)	FixErrorLog((DWORD)wErrorStrIndex);
    			}
    		}
    		dbeflg.wErrorFlag[i] = GetDBValue(dwIndex).lValue;
    		continue;//第15個WORD要跳過去
    	}
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	wErrorFlag = dbeflg.wErrorFlag[i] ^ GetDBValue(dwIndex).lValue;
      	switch(i)
      	{
      		case	0:
      			if((~wErrorFlag&0x0002)&&(dbeflg.wErrorFlag[i]&0x0002))	bSPdoor 	 = TRUE;
      			break;
      		case	1:
      			if((~wErrorFlag&0x0800)&&(dbeflg.wErrorFlag[i]&0x0800))	bSPcoreLoc = TRUE;
      			break;
      		case	2:
      			if((~wErrorFlag&0x0008)&&(dbeflg.wErrorFlag[i]&0x0008))
      				if(!bSPcoreLoc&&!bSProbot)	bSPcorePos = TRUE; //條件必須互斥,否則就掛了
      			if((~wErrorFlag&0x0200)&&(dbeflg.wErrorFlag[i]&0x0200))
      				if(!bSPcoreLoc&&!bSPcorePos)	bSProbot	 = TRUE;
      			break;
      		case	3:
      			if((~wErrorFlag&0x4000)&&(dbeflg.wErrorFlag[i]&0x4000))	bSPbackdoor = TRUE;
      			break;
      		default:
      			break;
      	}

     	if((i == 0) && (GetDBValue(dwIndex).lValue == 1) && (g_pExTable->GetTableValue(0,"D_BLOW")))		//2010-12-15 記錄溫度與模溫分段警報
     	{
	   		if(GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue & 0x0008)
	    		{
          		lMoldTempNew = GetMoldTemperErrorStatus();
             	if(lMoldTempOld !=lMoldTempNew)
             	{
          	         cIndex = 0;
          	         RealIndex=cIndex + i*16;
	      	 		m_iRealIndex = RealIndex;
	      	 		wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&m_iRealIndex,&dbeflg);

          	         AddErrorLog((DWORD)wErrorStrIndex);

             	     	if(lMoldTempOld != 0)
             	      	{
             	       		wTempErrorStrIndex = wErrorStrIndex + lMoldTempOld - lMoldTempNew;
             	       		FixErrorLog((DWORD)wTempErrorStrIndex);
             	      	}
             	 		lMoldTempOld =lMoldTempNew;
             	}
	       	}
          else
          {
          		lTempNew     = GetTemperErrorStatus();
          		if(lTempOld !=lTempNew)
          		{
        	        cIndex = 0;
        	        RealIndex=cIndex + i*16;
	    			m_iRealIndex = RealIndex;
	    			wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&m_iRealIndex,&dbeflg);
        	        AddErrorLog((DWORD)wErrorStrIndex);

           	    	if(lTempOld != 0)
           	     	{
           	      		wTempErrorStrIndex = wErrorStrIndex + lTempOld - lTempNew;
           	      		FixErrorLog((DWORD)wTempErrorStrIndex);
           	     	}
           			lTempOld =lTempNew;
           	}
          }
     	}
      else  //標準警報
    	{
    	if (wErrorFlag)
       	{
        	dbeflg.wErrorFlag[i] = GetDBValue(dwIndex).lValue; //第一次發生進來會先存入,做為第二次還原時間(FixErrorLog)之用
        	  //printf("b:%d, wErrorFlag:%X,buf:%X,db:%X\n",i,wErrorFlag,dbeflg.wErrorFlag[i],GetDBValue(dwIndex).lValue);
        	while (wErrorFlag)            //多個ERROR同時發生要全部都紀錄!
					{
	            cIndex = (char)_ScanBitF(wErrorFlag);
	            RealIndex=cIndex + i*16;  //先記憶體存住,可免相同使用之判斷式都重算
	            //printf("RealIndex=%d g_wErrorStatus=%d b_AlarmState=%d \n", RealIndex, g_wErrorStatus, b_AlarmState);

				m_iRealIndex = RealIndex;	//iResult = (int)RealIndex;
				if(g_pExTable != NULL)
					wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&RealIndex,&dbeflg);
				//printf("result: %d,RealIndex: %d\n",iResult,RealIndex);
				if(RealIndex <= 1)
            	{
	                wErrorFlag = _ClearBit(wErrorFlag, cIndex); //先清掉該BIT再繼續
	                g_wErrorStatus = FALSE;
	                b_AlarmState = FALSE;
	                continue;      //不存入紀錄檔的例外狀況
            	}
            	else			//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
            	{
	            	g_wErrorStatus = TRUE;
	            	b_AlarmState = TRUE;
            	}

	            if (_TestBit(GetDBValue(dwIndex).lValue, cIndex))
	            {
	            	if(m_nErrorCount == 0)
						Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示
	            	AddErrorLog((DWORD)wErrorStrIndex);
	            	//printf("AddErrorLog wErrorStrIndex=%d, %d\n",wErrorStrIndex, wMoldTempErrStrIndex);
	            }
	            else
            	{
					if(!(g_pExTable->GetTableValue(0,"D_TP2MTP")||g_pExTable->GetTableValue(0,"D_MTPFC")||g_pExTable->GetTableValue(0,"D_TPMTP")||g_pExTable->GetTableValue(0,"D_MTP2TP")||g_pExTable->GetTableValue(0,"D_9TPMTP")||g_pExTable->GetTableValue(0,"D_7KD")))	/* 無料溫全部采用模溫控製(藍力壓機)*/
	            	{
		            	if(((wDSPLIBDaysCode < 0x4703) &&(wDSPLIBYearCode==0x2014)|| (wDSPLIBYearCode <= 0x2013))||((GetDBValue("CONF_CODE_WRITE_NULL_DATA15").lValue & 0xFF00) ==0x7200))  //7900 LIB 4703, 7100 LIB 4704， 7200 按舊方式處理
	    				{
		            		if(wMoldTempFlag == 2)//模溫偏差 //JOYCE2012-3-30
		            		{
		            			wMoldTempFlag = 0;
		            			FixErrorLog((DWORD)wMoldTempErrStrIndex);
		            		}
		            		else	if(wTempFlag == 2)//溫度偏差 //JOYCE2012-3-30
		            		{
		            			wTempFlag = 0;
		            			FixErrorLog((DWORD)wTempErrStrIndex);
		            		}
			            	else
			            	{
				            	FixErrorLog((DWORD)wErrorStrIndex);
				            }
			        	}
			        	else
			        	{
			        		//wMoldTempErrStrIndex = g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR2");
			            	//printf("FixErrorLog wErrorStrIndex=%d, %d\n",wErrorStrIndex, wMoldTempErrStrIndex);
			        		FixErrorLog((DWORD)wErrorStrIndex);
			        	}
	            	}
	            	else
	            		FixErrorLog((DWORD)wErrorStrIndex);
            	}
            	wErrorFlag = _ClearBit(wErrorFlag, cIndex);
					}
				}
			}
	}

	#ifdef	D_3354	
	if(CtmConfig::GetInstance()->GetPickerFlag())
		Error_Control_Card();
	#endif

	if(1 == CtmConfig::GetInstance()->GetAustoneFlag())
	{
	 	Dsp28Error_Control();		//James add 2010/5/29 06:47 for dsp28
	}
	if(1 == CtmConfig::GetInstance()->GetN2CFlag())
	{
	 	N2CError_Control();			//Caozz 2011/4/14 上午 11:38:14
	}

	if(1 == CtmConfig::GetInstance()->GetCom2Type())
	{
		Error_Control_COM2_New();
	}

  	//WORD	wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	//WORD	wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	if((wDSPLIBDaysCode>=0x0707)&&(wDSPLIBYearCode==0x2010)||(wDSPLIBYearCode>=0x2011))
	{
		Error_Control2_New();	//2013-9-17, for Error2
	}

	RollError();	//Caozz add 2011/3/28 下午 04:30:21
}

void   	CtmControl_Error::Error_Control_COM2_New()
{
	short		i = 0;
	WORD		cIndex = tmERROR_NULL;
	int 		RealIndex = 0;
	WORD		wErrorFlag_COM2 = 0;	//wCoreErrorFlag;
	DWORD		wErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY"); //JOYCE2008-11-7 給初始值
	DWORD		dwID = 0, dwIndex = 0;	// dwValue;
	BOOL		bSPdoor=FALSE,bSPbackdoor=FALSE,bSPcoreLoc=FALSE,bSPcorePos=FALSE,bSProbot=FALSE;
	int			iResult = 0;
	DWORD		dwTempValue = ERROR_OBJECT_COM2;
	DWORD		wErrorStrIndex_COM2 = 0;

	int			ERR_BEGIN				= g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");
	int			ERR_0E_PLSCLSDOOR		= g_MultiLanguage.GetStrKeyIndex("ERR_0E_PLSCLSDOOR");
	int			ERR_0D_PLSOPNDOOR		= g_MultiLanguage.GetStrKeyIndex("ERR_0D_PLSOPNDOOR");

	dwID = g_pDatabase->GetDataID("STATE_2_ALARM_ON_STEP1_ERRORDATA");
	//James add HTK530 ---28警報字串顯示
	long long lDsp28ErrorID = GetDBValue("SYS_HMI_NULL_NULL_RESERVED40").lValue;
	int		  DSP28ERR_BEGIN	= g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR2");
	if(lDsp28ErrorID > 0)
	{
		Prompt(g_MultiLanguage[DSP28ERR_BEGIN+lDsp28ErrorID],1); //標準之錯誤字串顯示
	}

	for (i=tmMAX_OP_ERRORSAVE+1; i>=0; i--)		//JOYCE2009-2-10 tmMAX_OP_ERRORSAVE-1-->tmMAX_OP_ERRORSAVE+1
    {
    	if(i==14)  continue;  //JOYCE2009-2-10 add
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if ((wErrorFlag_COM2 = GetDBValue(dwIndex).lValue))	//assignment and !zero;
        {
        	cIndex  = (char)i*16 + (char)_ScanBitR(wErrorFlag_COM2);  // Scan the 1's bit from left to right (Reverse)
        	RealIndex = (int)cIndex;		//用RealIndex傳遞Assign!!
        	m_iRealIndex = RealIndex;

        	if(g_pExTable != NULL)
        		wErrorStrIndex = g_pExTable->GetTableValue(9,(int*)&RealIndex,&dbeflg2); //再用m_iRealIndex得到反饋結果(優先級)
        	//wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult); //再用iResult得到反饋結果(優先級)
        	//printf("result: %d,RealIndex: %d\n",iResult,RealIndex);
        	//優先級最低，則需要繼續掃描後面在WORD或BIT！
        	if(RealIndex == 0)
        		{
       				WORD wIndex1,wIndex2;
       				//一個從左邊掃描，一個從右邊掃描！！
       				wIndex1 = _ScanBitF(wErrorFlag_COM2);
       				wIndex2 = _ScanBitR(wErrorFlag_COM2);
       				//如果相等代表此WORD隻有一個BIT為ON，繼續掃描；若不等則有多個BIT，以後者為主
       				if(wIndex1 == wIndex2) continue;
       				else
       				{
       					_ClearBit(wErrorFlag_COM2,wIndex2);
       					cIndex = (char)i*16 + (char)wIndex1;
       					RealIndex = (int)cIndex;		//用iResult傳遞Assign!!
       					m_iRealIndex = RealIndex;
        				wErrorStrIndex = g_pExTable->GetTableValue(9,(int*)&RealIndex,&dbeflg2); //再用iResult得到反饋結果(優先級)
       					break;
       				}
        		}
        	else	break;
        }
    }

	if (cIndex != tmERROR_NULL)
    {
    	if(b_AlarmState)	//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
			g_wErrorStatus = TRUE;

    	if (dbbuff2.cIndexError != cIndex)
        {
        	if (dbbuff2.cIndexMode != tmMODE_AUTOADJMOLD)
            {
            	if ((dbbuff2.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)))
                {
	                dbbuff2.bDelayedSpeech = TRUE;
	                dbbuff2.lDelayedCount  = SPEECH_DELAYEDCOUNT;
	                dbbuff2.bRepeatSpeech  = FALSE;
                }
            	else
                {
	                dbbuff2.bDelayedSpeech = FALSE;
	                //::Speech(cIndex);
	                if ((dbbuff2.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0D_PLSOPNDOOR - ERR_BEGIN)))
	                    dbbuff2.bRepeatSpeech  = FALSE;
	                else
                    {
                    	dbbuff2.bRepeatSpeech  = TRUE;
                    	dbbuff2.lRepeatCount   = SPEECH_REPEATCOUNT;
                    }
                }
            }

			if(m_nErrorCount == 0)
				Prompt(g_MultiLanguage[wErrorStrIndex],1);  //標準之錯誤字串顯示
			dbbuff2.cIndexError = cIndex;
        }
    	else  //dbbuff2.cIndexError != cIndex
        {
        	if ((dbbuff2.cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)) && (dbbuff2.bDelayedSpeech) && (dbbuff2.lDelayedCount <= 0))
            {
	            //::Speech(cIndex);
	            dbbuff2.bDelayedSpeech = FALSE;
	            dbbuff2.bRepeatSpeech  = TRUE;
	            dbbuff2.lRepeatCount   = SPEECH_REPEATCOUNT;
            }
        	else if ((dbbuff2.bRepeatSpeech) && (dbbuff2.lRepeatCount <= 0))
            {
	            //::Speech(cIndex);
	            dbbuff2.lRepeatCount   = SPEECH_REPEATCOUNT;
            }
	        // if (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue == ID_VIEW_OVERVIEW)     //wy112299 add for error load again
	       	//    {
	       	//yangpeng 2007/4/16 02:13下午錯誤信息要在每個界面中都提示!
       		//if (nErrorCount > 0)
       		//{
       	 	//	Prompt(g_MultiLanguage[wErrorStrIndex],1);  // James modifu 2008/7/17 10:00上午 配合顯示最新的Error Prompt(pszError[nErrorCount - 1],1);
    			//}
	       	//else
	       	{
	        	if(m_nErrorCount == 0)
					Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示

	       	}
        	//g_tmDebugInfo->PrintDebugInfo("Error Index0 = %d\n", ERR_BEGIN+cIndex);
        	dbbuff2.cIndexError = cIndex;
       		//    }
        }
    }
	else  //cIndex==ERROR_NULL
    {

    	g_wErrorStatus = FALSE;
	    if (dbbuff2.cIndexError != cIndex)
        {
	        Prompt(g_MultiLanguage[wErrorStrIndex]);

	        g_pMainFrame->GetCurrentView()->OnLoseFocus();  //2009-8-26, add, 空警報
            g_pMainFrame->GetCurrentView()->OnGetFocus();
	        dbbuff2.cIndexError = cIndex;
	        b_PromptBar = TRUE; ///Andy 20100723
        }
        else ///Andy 20100723
      	{
      		if(b_PromptBar)
      			{
      				//SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF, NULL);
      				b_PromptBar = FALSE;
      			}
      	}
    }

	for (i = 0; i < (tmMAX_OP_ERRORSAVE+2); i++)	//JOYCE2009-2-10 tmMAX_OP_ERRORSAVE-->tmMAX_OP_ERRORSAVE+2
    {
    	//if(i==14)  continue;  //JOYCE2009-2-10 add
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if(i == 14)
    	{
    		wErrorFlag_COM2 = dbeflg2.wErrorFlag[i] ^ GetDBValue(dwIndex).lValue;
    		if(wErrorFlag_COM2&0x0020)//安全門不記錄,發生變化
    		{
    			if(bSPdoor)
    			{
    				dbeflg2.wErrorFlag[0] &= 0xFFFD;
    				iResult = 1+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(9,(int*)&iResult,&dbeflg2);
    				if(iResult == 10)
    				{
							wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
    					FixErrorLog(wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
    				}
    			}
    			if(bSPbackdoor)
    			{
    				dbeflg2.wErrorFlag[3] &= 0xBFFF;
    				iResult = 62+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(9,(int*)&iResult,&dbeflg2);
    				if(iResult == 10)
    				{
							wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
    					FixErrorLog(wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
    				}
    			}
    		}
    		if(wErrorFlag_COM2&0x001F)
    		{
    			if(bSPcoreLoc)
    			{
    				dbeflg2.wErrorFlag[1] &= 0xF7FF;
    				iResult = 27+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(9,(int*)&iResult,&dbeflg2);
    				if(iResult == 10)
    					{
							wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
    					FixErrorLog(wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
    					}
    			}
    			else	if(bSPcorePos)
    			{
    				dbeflg2.wErrorFlag[2] &= 0xFFF7;
    				iResult = 35+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(9,(int*)&iResult,&dbeflg2);
    				if(iResult == 10)
    					{
							wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
    					FixErrorLog(wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
    					}
    			}
    			else	if(bSProbot)
    			{
    				dbeflg2.wErrorFlag[2] &= 0xFDFF;
    				iResult = 41+0x100;
    				wErrorStrIndex	=	g_pExTable->GetTableValue(9,(int*)&iResult,&dbeflg2);
    				if(iResult == 10)
    					{
							wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
    					FixErrorLog(wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
    					}
    			}
    		}
    		dbeflg2.wErrorFlag[i] = GetDBValue(dwIndex).lValue;
    		continue;//第15個WORD要跳過去
    	}
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	wErrorFlag_COM2 = dbeflg2.wErrorFlag[i] ^ GetDBValue(dwIndex).lValue;
    	//printf("COM2 wErrorFlag_COM2=%4x,dbeflg2.wErrorFlag[%d]=%4x\n",wErrorFlag_COM2,i,dbeflg2.wErrorFlag[i]);
      	switch(i)
      	{
      		case	0:
      			if((~wErrorFlag_COM2&0x0002)&&(dbeflg2.wErrorFlag[i]&0x0002))	bSPdoor 	 = TRUE;
      			break;
      		case	1:
      			if((~wErrorFlag_COM2&0x0800)&&(dbeflg2.wErrorFlag[i]&0x0800))	bSPcoreLoc = TRUE;
      			break;
      		case	2:
      			if((~wErrorFlag_COM2&0x0008)&&(dbeflg2.wErrorFlag[i]&0x0008))
      				if(!bSPcoreLoc&&!bSProbot)	bSPcorePos = TRUE; //條件必須互斥,否則就掛了
      			if((~wErrorFlag_COM2&0x0200)&&(dbeflg2.wErrorFlag[i]&0x0200))
      				if(!bSPcoreLoc&&!bSPcorePos)	bSProbot	 = TRUE;
      			break;
      		case	3:
      			if((~wErrorFlag_COM2&0x4000)&&(dbeflg2.wErrorFlag[i]&0x4000))	bSPbackdoor = TRUE;
      			break;
      		default:
      			break;
      	}
    	if (wErrorFlag_COM2)
       	{
        	dbeflg2.wErrorFlag[i] = GetDBValue(dwIndex).lValue; //第一次發生進來會先存入,做為第二次還原時間(FixErrorLog)之用
        	  //printf("b:%d, wErrorFlag:%X,buf:%X,db:%X\n",i,wErrorFlag,dbeflg2.wErrorFlag[i],GetDBValue(dwIndex).lValue);
        	while (wErrorFlag_COM2)            //多個ERROR同時發生要全部都紀錄!
			{
				//g_tmDebugInfo->PrintDebugInfo("Error Index1 = 619\n");
	            cIndex = (char)_ScanBitF(wErrorFlag_COM2);
	            RealIndex=cIndex + i*16;  //先記憶體存住,可免相同使用之判斷式都重算
	            //printf("RealIndex=%d g_wErrorStatus=%d b_AlarmState=%d \n", RealIndex, g_wErrorStatus, b_AlarmState);

				m_iRealIndex = RealIndex;	//iResult = (int)RealIndex;
				if(g_pExTable != NULL)
					wErrorStrIndex	=	g_pExTable->GetTableValue(9,(int*)&RealIndex,&dbeflg2);
				//printf("result: %d,RealIndex: %d\n",iResult,RealIndex);
				if(RealIndex <= 1)
            	{
	                wErrorFlag_COM2 = _ClearBit(wErrorFlag_COM2, cIndex); //先清掉該BIT再繼續
	                g_wErrorStatus = FALSE;
	                b_AlarmState = FALSE;
	                continue;      //不存入紀錄檔的例外狀況
            	}
            	else			//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
            	{
	            	g_wErrorStatus = TRUE;
	            	b_AlarmState = TRUE;
            	}

	            if (_TestBit(GetDBValue(dwIndex).lValue, cIndex))
	            {
	            	if(m_nErrorCount == 0)
						      Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示

								wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
								//printf("wErrorStrIndex_COM2=%d,wErrorStrIndex=%d\n",wErrorStrIndex_COM2,wErrorStrIndex);
	            	AddErrorLog((DWORD)wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
	            	/*//Andy 20081029
	            	nErrorCount++;
	            	*/
	            }
	            else
            	{
								wErrorStrIndex_COM2 =  wErrorStrIndex + (dwTempValue << 24);
	            	FixErrorLog((DWORD)wErrorStrIndex_COM2, tmCOM2_ERRORFLAG);
	            	/*//Andy 20081029
	            	nErrorCount--;
	            	pszError[nErrorCount] = NULL;
	            	*/
	            	//if (g_ptaskkeb != NULL) g_ptaskkeb->ErrorReset();
	            	//g_wErrorStatus_Dsp28 = FALSE;	//	clear the dsp28 error
            	}
            	wErrorFlag_COM2 = _ClearBit(wErrorFlag_COM2, cIndex);
			}
		}
	}
}

void	CtmControl_Error::Dsp28Error_Control()
{
	DWORD 	dwErrorID = GetDBValue("AUSTON_ACTUALDATA_ALARMNO").lValue;//GetDBValue("MACH_OTHR_OTHR_NULL_W3A20").lValue;
	static	BOOL bShowFlag = FALSE;
	static	DWORD	dwOldErrorID = 0;
	DWORD	dwTempValue = ERROR_OBJECT_AUSTONE;
	// /*ZHBA 2011-12-19 13 */ if(dwErrorID > 0 && dwErrorID < 0xFFFF)
	if((dwErrorID & 0x0000FFFF) > 0 && dwErrorID < 0xFFFFFFFF)/*高WORD:區分隨動時Austone警報來源;低WORD:警報具體ID*/
	{
		dwErrorID +=  dwTempValue << 24;
		if(dwOldErrorID != dwErrorID && dwOldErrorID != 0)
		{
			FixErrorLog((DWORD)(dwOldErrorID), tmAUSTONE_ERRORFLAG);
		}

		if(bShowFlag == FALSE || dwOldErrorID != dwErrorID)
		{
			g_ptmControlServer->ErrorControl()->AddErrorLog((DWORD)(dwErrorID), tmAUSTONE_ERRORFLAG);
		}
		dwOldErrorID = dwErrorID;
		bShowFlag = TRUE;
		g_wErrorStatus_Dsp28 = TRUE;
	}
	else
	{
		if(bShowFlag == TRUE)
		{
			bShowFlag = FALSE;

			FixErrorLog((DWORD)(dwOldErrorID), tmAUSTONE_ERRORFLAG);//SendMsg(MSG_USER_SH_ERROR_FIX, 0, ERR_BEGIN-ERR_DSP54BEGIN+wOldErrorID, NULL);
			g_wErrorStatus_Dsp28 = FALSE;
		}

	}

}

void	CtmControl_Error::N2CError_Control()
{
	DWORD dwErrorID;
	if(GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED74").lValue == 1) /*Austone壓力閉環和N2C單回路程式 FLAG*/
	dwErrorID= GetDBValue("AUSTON_ACTUALDATA_DWERROR").lValue;/*N2C(50038)*/
	else
	dwErrorID= GetDBValue("AUSTON_ACTUALDATA_ALARMNO").lValue;/*Austone(50037)*/
	static	BOOL bShowFlag = FALSE;
	static	DWORD	dwOldErrorID = 0;

	if(dwErrorID > 0x00010000 && (dwErrorID & 0x0000FFFF) > 0 && dwErrorID < 0xFFFFFFFF)
	{
		if(dwOldErrorID != dwErrorID && dwOldErrorID != 0)
		{
			FixErrorLog((DWORD)dwOldErrorID, tmN2C_ERRORFLAG);
		}

		if(bShowFlag == FALSE || dwOldErrorID != dwErrorID)
		{
			g_ptmControlServer->ErrorControl()->AddErrorLog((DWORD)dwErrorID, tmN2C_ERRORFLAG);
		}
		dwOldErrorID = dwErrorID;
		bShowFlag = TRUE;
		g_wErrorStatus_Dsp28 = TRUE;
	}
	else
	{
		if(bShowFlag == TRUE)
		{
			bShowFlag = FALSE;

			FixErrorLog((DWORD)dwOldErrorID, tmN2C_ERRORFLAG);//SendMsg(MSG_USER_SH_ERROR_FIX, 0, ERR_BEGIN-ERR_DSP54BEGIN+wOldErrorID, NULL);
			g_wErrorStatus_Dsp28 = FALSE;
		}
	}
}

void   	CtmControl_Error::Error_Control2_New()	//2013-9-17, for Error2
{
	short		i = 0;
	WORD		cIndex = 0xFFFF, RealIndex = 0;
	WORD		wErrorFlag = 0;
	WORD		wErrorStrIndex = g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY");
	DWORD		dwID = 0, dwIndex = 0;
	int			iResult = 0;
	BOOL    	bError2Flag = 0;

	dwID = g_pDatabase->GetDataID("STATE_ALARM2_ON_STEP1_ERRORDATA");

	for (i = 0; i < 8; i++)
	{
	 	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
	 	//printf("wAssign: %x\n", GetDBValue(dwIndex).lValue);
	 	if ((wErrorFlag = GetDBValue(dwIndex).lValue) >= 0x100)
	 		{
	        	b_Error2 = TRUE;	//fuxy, 2010-10-9
	        	cIndex  = GetDBValue(dwIndex).lValue;  // Scan the 1's bit from left to right (Reverse)
	        	m_iRealIndex = (int)cIndex;		//用iResult傳遞Assign!!
	        	iResult = m_iRealIndex + 0x10000000;//以表示error2
	        	wErrorStrIndex = g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg); //再用iResult得到反饋結果(優先級)
	 		}
	 	else break;
	}
		//show error
//	if (dbbuff.cIndexError == tmERROR_NULL)//stat.error沒有警報時才show error2,否則兩個警報會來回閃動
//	{
		if (cIndex != 0xFFFF)
	    {
	    	if(b_AlarmState)	//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
				g_wErrorStatus = TRUE;
       		if(m_nErrorCount == 0)
		     	Prompt(g_MultiLanguage[wErrorStrIndex],1);
	     	dbbuff.cIndexError2 = cIndex;
	    }
		else  //cIndex==ERROR_NULL
	    {
		    if(b_Error2)
		    	g_wErrorStatus = FALSE;
		    b_Error2 = FALSE;		//fuxy, 2010-10-9
		    if (dbbuff.cIndexError2 != cIndex)
	        {
		        Prompt(g_MultiLanguage[wErrorStrIndex]);
		        g_pMainFrame->GetCurrentView()->OnLoseFocus();
		        g_pMainFrame->GetCurrentView()->OnGetFocus();
		        dbbuff.cIndexError2 = cIndex;
				b_PromptBar = TRUE; //JOYCE2010-10-22
	        }
      		else //JOYCE2010-10-22
      		{
      			if(b_PromptBar)
      			{
     				if(m_nErrorCount == 0)
     				{
     					g_pMainFrame->GetPromptBar()->ShowAlarm(FALSE);		//fuxy, 2012-1-12, for  警報紅條
     					SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF, NULL);
     				}
      				b_PromptBar = FALSE;
      			}
      		}
	    }
//	}

		//record error
	for (i = 0; i < 8; i++)//改為掃描8個WORD
    {
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);

    	if (dbeflg.wErrorFlag2[i] != GetDBValue(dwIndex).lValue)	//error2,是以序號來標識,一個值表示一個警報,新舊值不同時進行處理, 2011-10-24
       	{
			if((dbeflg.wErrorFlag2[i] != 0)&&(GetDBValue(dwIndex).lValue != 0))//新舊值都不為0
			{
				wErrorFlag = GetDBValue(dwIndex).lValue;
				bError2Flag = TRUE;
			}
			else//新舊值,其中一個為0
			{
			   	wErrorFlag = dbeflg.wErrorFlag2[i] ^ GetDBValue(dwIndex).lValue;
				bError2Flag = FALSE;
			}

       		dbeflg.wErrorFlag2[i] = GetDBValue(dwIndex).lValue; //第一次發生進來會先存入,做為第二次還原時間(FixErrorLog)之用
        	RealIndex=wErrorFlag;  //先記憶體存住,可免相同使用之判斷式都重算
			iResult = (int)RealIndex+0x10000000;//以表示error2
			m_iRealIndex = RealIndex;
			wErrorStrIndex	=	g_pExTable->GetTableValue(CNT_TABLE_ERROR,(int*)&iResult,&dbeflg);
			//printf("result: %d,RealIndex: %d dbeflg.wErrorFlag2[%d ]=%d dwIndex=%d \n",iResult,RealIndex , i, dbeflg.wErrorFlag2[i], dwIndex);
			if(iResult <= 1)
          	{
	           	g_wErrorStatus = FALSE;
	           	b_AlarmState = FALSE;
	           	continue;      //不存入紀錄檔的例外狀況
          	}
          	else			//James add 2008/4/14 10:49上午  提示Error不ShowAlarm
          	{
	        	g_wErrorStatus = TRUE;
	        	b_AlarmState = TRUE;
          	}

	        if (GetDBValue(dwIndex).lValue)
	        {
	        	if(bError2Flag)//新舊值都不為0,需先清除舊警報
	        		FixErrorLog((DWORD)wOldErrorStrIndex[i]);

	        		if(m_nErrorCount == 0)
		       		Prompt(g_MultiLanguage[wErrorStrIndex],1); //標準之錯誤字串顯示
	        	AddErrorLog((DWORD)wErrorStrIndex);
	       	}
	       	else
          	{
	       		FixErrorLog((DWORD)wErrorStrIndex);
         	}

			wOldErrorStrIndex[i] = wErrorStrIndex;
		}
	}
}

//Caozz 2011/4/14 上午 11:17:01
int	CtmControl_Error::ParseErrorID(char* pszErrorStr, DWORD dwErrorID)
{
	WORD    *pwDst, *pwSrc;
	pwDst = (WORD *)pszErrorStr;

	int 	j = 0;
	if((dwErrorID & 0xFFFF0000) == 0)	//54警報
	{
		pwSrc    = (WORD *)g_MultiLanguage[dwErrorID];

		if(pwSrc != NULL)
		while(*pwSrc != 0 && j < 256)
		{
		    *pwDst ++   = *pwSrc ++;
		    j++;
		}
		*pwDst = 0;
		return j * 2;
	}
	//28警報
	BYTE byObject=0, bySubIndex=0;
	WORD wErrorID = 0;
	byObject 	= (dwErrorID & 0xFF000000) >> 24;
	bySubIndex 	= (dwErrorID & 0x00FF0000) >> 16;
	wErrorID	= dwErrorID & 0x0000FFFF;
	char szTemp[64], szStrID[64];
	memset(szTemp,0,sizeof(szTemp));
	memset(szStrID,0,sizeof(szStrID));
	switch(byObject)
	{
		case ERROR_OBJECT_N2C:
			sprintf(szTemp, "[N%d]", bySubIndex);
			sprintf(szStrID, "N2C_ALARM%d", wErrorID);
			//printf("szStrID= %s\n", szStrID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
		case ERROR_OBJECT_E2C:
			sprintf(szTemp, "[E%d]", bySubIndex);
			sprintf(szStrID, "E2C_ALARM%d", wErrorID);
			//printf("szStrID= %s\n", szStrID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
		case ERROR_OBJECT_COM2:        /*1HMI2DSP*/
			sprintf(szTemp, "[Com2]");
			return ErrorIDToStr_COM2(szTemp, wErrorID, pwDst);
			break;
		case ERROR_OBJECT_AUSTONE:
			sprintf(szTemp, "[A%d]", bySubIndex);
			sprintf(szStrID, "AUSTONE_ALARM%d", wErrorID);
			//printf("szStrID= %s\n", szStrID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
		case ERROR_OBJECT_SIEMENS:
			sprintf(szTemp, "[S%d]", bySubIndex);
			sprintf(szStrID, "SIEMENS_ALARM%d", wErrorID);
			//printf("szStrID= %s\n", szStrID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
		case ERROR_OBJECT_TM:
			sprintf(szTemp, "[T%d]", bySubIndex);
			sprintf(szStrID, "TM_ALARM%d", wErrorID);
			//printf("szStrID= %s\n", szStrID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
		case ERROR_AMMETER_TM:			//Rw.ying 2013-1-31
			sprintf(szTemp, "[Amt%d]", bySubIndex+1);
			if((wErrorID&0xFF00) == 0)
				sprintf(szStrID, "AMMETER_ALARM%d", wErrorID);
			else if((wErrorID&0xFF00) == 0x100)
				sprintf(szStrID, "AMMETER_VO_ALARM%d", wErrorID&0xFF);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
		case ERROR_OBJECT_TMIOT:
			sprintf(szTemp, "[PICKER]:");
			sprintf(szStrID, "PICKER_M3_ALARM%d", wErrorID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;						
		default:
			sprintf(szTemp, "[Un %d,%d]", byObject,bySubIndex);
			sprintf(szStrID, "UNKNOWN_ALARM%d", wErrorID);
			//printf("szStrID= %s\n", szStrID);
			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
			break;
	}
}
int	CtmControl_Error::ErrorIDToStr(char* szTemp, char* szStrID ,WORD wErrorID, WORD *pwDst)
{
	WORD   *pwSrc;
	int 	j = 0;
	if(g_MultiLanguage.GetStrKeyIndex(szStrID) == -1)
	{
		sprintf(szTemp, "%s Error: %d", szTemp, wErrorID);
		while(*szTemp != 0)
		{
			*pwDst ++ = *szTemp ++;
			j++;
		}
		*pwDst = 0;
		return j * 2;
	}
	else
	{
		while(*szTemp != 0)
		{
			*pwDst ++ = *szTemp ++;
			j ++;
		}
		*pwDst ++ =0x20;
		j ++;
		pwSrc    = (WORD *)g_MultiLanguage[szStrID];
		if(pwSrc != NULL)
		while(*pwSrc != 0 && j < ERROR_STR_LEN/ERRORSTRMAX_NUM/2) /*joyce20130319*增加保護,限製單個字符串長度為50個字,防止數組越界(按tm編碼算,50個中文字/50個英文)*/
		{
		    *pwDst ++   = *pwSrc ++;
		    j ++;
		}
		*pwDst = 0;
		return j * 2;
	}
}

int	CtmControl_Error::ErrorIDToStr_COM2(char* szTemp, WORD wErrorID, WORD *pwDst)
{
	WORD   *pwSrc;
	int 	j = 0;
	{
		while(*szTemp != 0)
		{
			*pwDst ++ = *szTemp ++;
			j ++;
		}
		*pwDst ++ =0x20;
		j ++;
		pwSrc    = (WORD *)g_MultiLanguage[wErrorID];
		if(pwSrc != NULL)
		while(*pwSrc != 0 && j < 256)
		{
		    *pwDst ++   = *pwSrc ++;
		    j ++;
		}
		*pwDst = 0;
		return j * 2;
	}
}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::RollError()    	     		 				 |
|  Task     : 滾動顯示錯誤						     	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmControl_Error::RollError()
{
	static int nLan =-1;
	int nLanguageIndex = CtmConfig::GetInstance()->GetCurrentIndex();
	if(NULL == m_ErrorListHead)        	return;

	if(nLan != nLanguageIndex) //20160405
	{
		nLan =nLanguageIndex;
		u_bErrorChangeFlag =TRUE;
	}

	int 		i = 0;
	static int 	nErrorStrLen = 0, nParseErrorLen = 0;
	char 		szErrorStr[512];
	char		szErrorNo[10];
	//WORD   	 	pszTemp[]   = {' ', ' ', ' ', ' '};
	char   	 	pszTemp[8]={0};
	for(int i=0; i<sizeof(pszTemp);i+=2)
	{
		pszTemp[i]=' ';  //20160330
	}

	memset(szErrorStr, 0, sizeof(szErrorStr));
	memset(szErrorNo, 0, sizeof(szErrorNo));

	tmErrorInfo* pCurrent = m_ErrorListHead->pNext;
	if(m_nErrorCount > 0)
	{
		if(!b_AddTimer)
		{
			::AddTimer(&l_lLoopTimer);
			b_AddTimer = TRUE;
		}

		if(m_nErrorCount == 1)
		{
			ParseErrorID(u_szErrorStr, pCurrent->dwErrorID);
			Prompt(u_szErrorStr,1);
		}
		else
		{
			if(u_bErrorChangeFlag)
			{
				nErrorStrLen = 0;
				memset(u_szErrorStr, 0, sizeof(u_szErrorStr));
				for(i = 0; i < ERRORSTRMAX_NUM && NULL != pCurrent; i++)
				{
					if(pCurrent->dwErrorID > 0)
					{
						char szTemp[8];   //20160330, char szTemp[4];
						sprintf(szTemp, "%d. ", i+1);
						CodeChange(szErrorNo, szTemp);

						memcpy(u_szErrorStr + nErrorStrLen, szErrorNo, 6);
						//memcpy(u_szErrorStr + nErrorStrLen, szTemp, strlen(szTemp));
						nErrorStrLen += 6;

						nParseErrorLen = ParseErrorID(szErrorStr, pCurrent->dwErrorID);
						memcpy(u_szErrorStr + nErrorStrLen, szErrorStr, nParseErrorLen);
						nErrorStrLen += nParseErrorLen;

						memcpy(u_szErrorStr + nErrorStrLen, pszTemp, sizeof(pszTemp));
						nErrorStrLen += sizeof(pszTemp);

					}
					pCurrent = pCurrent->pNext;
				}
				Prompt(u_szErrorStr,1);
			}
			if(l_lLoopTimer < 0)
			{
				char szError[1024];
				memset(szError,0,sizeof(szError));
				char* pszErrorTemp = u_szErrorStr;
				memcpy(pszErrorTemp + nErrorStrLen, pszErrorTemp, sizeof(WORD));
				int nLen =0;
				while(*(WORD*)pszErrorTemp != 0)
				{
					nLen++;
					memcpy(pszErrorTemp, pszErrorTemp + sizeof(WORD), sizeof(WORD));
					pszErrorTemp += sizeof(WORD);
				}
				//*(WORD*)pszErrorTemp = 0;
				memcpy(szError, pszErrorTemp, nLen<<1);  //20160330
				Prompt(u_szErrorStr,1);
				l_lLoopTimer = ERRORLOOPTIMER;
				u_bErrorChangeFlag = FALSE;
			}

		}
	}
	else
	{
		if(b_AddTimer)
		{
			::DeleteTimer(&l_lLoopTimer);
			b_AddTimer = FALSE;
			//SendMsg(MSG_USER_SH_PROMPTNULL, 0, 0, NULL);
		}

	}
}


//Caozz add 2011/3/31 上午 11:29:21
BOOL    CtmControl_Error::ChangeErrorList(DWORD dwErrorID, WORD wSource, WORD wGrade, int nFlag)
{
	if(m_nErrorCount > ERRORLISTMAX_NUM && nFlag == 1) return FALSE;
	if(NULL == m_ErrorListHead)        	return FALSE;

	tmErrorInfo* pCurrent = m_ErrorListHead->pNext;
	tmErrorInfo* pBack 	  = m_ErrorListHead;

	tmErrorInfo* pTemp = NULL;
	if(nFlag == 1)//AddErrorLog
	{
		while(NULL != pCurrent)
		{
			if(pCurrent->dwErrorID == dwErrorID) return FALSE;
			if(wGrade > pCurrent->wGrade) break;
			pBack = pCurrent;
			pCurrent = pCurrent->pNext;
		}

		pTemp =(tmErrorInfo*)malloc(sizeof(tmErrorInfo));
		pTemp->wSource 		= wSource;
		pTemp->wGrade 		= wGrade;
		pTemp->dwErrorID	= dwErrorID;
		pTemp->pNext 		= pBack->pNext;
		pBack->pNext		= pTemp;

		m_nErrorCount ++;
		u_bErrorChangeFlag = TRUE;

	}
	else if(nFlag == 0)//FixErrorLog
	{
		while(NULL != pCurrent)
		{
			if (pCurrent->dwErrorID == dwErrorID)
			{
				pBack->pNext = pCurrent->pNext;
				free(pCurrent);
				m_nErrorCount -- ;
				u_bErrorChangeFlag = TRUE;
				return TRUE;
			}
			pBack = pCurrent;
			pCurrent = pCurrent->pNext;
		}
	}
	return FALSE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::FixErrorLog(DWORD dwID, WORD	wSource)       |
|  Task     : Fix Error Index						     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:      WID                  -    Error ID                        |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_Error::FixErrorLog(DWORD dwID, WORD	wSource)
{
	WORD        i = 0;
	WORD        wnIndex = m_MsgIndex.wIndex  + 1;
	pthread_mutex_lock(&m_MutexErrMsg);

	for (i=0; i<m_MsgIndex.wCount; i++)
	{
	     if ((short int)(--wnIndex) < 0)         //James modify 2009/6/8 狟敁 07:50:28  --wnIndex  ---wnIndex -1
	    	wnIndex = CtmConfig::GetInstance()->GetErrorMaxNum() - 1;
        #ifdef	D_3354   
        if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_ALARM, wnIndex, &m_MsgCurrent, sizeof(m_MsgCurrent));
		else
	    	CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ERROR_SRAMFILE, wnIndex, &m_MsgCurrent);	//2009-5-21
	    #else
	    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ERROR_SRAMFILE, wnIndex, &m_MsgCurrent);	//2009-5-21
	    #endif

	    if ((m_MsgCurrent.OilError.dwID == dwID) && (m_MsgCurrent.OilError.dateFixed.year == 0) && m_MsgCurrent.wSource == wSource) 		//James modify 2007/9/11 06:02下午 add wSource 2011/3/11 上午 09:11:16
	    {
	        GetDate(&m_MsgCurrent.OilError.dateFixed);
	        GetTime(&m_MsgCurrent.OilError.timeFixed);
	        #ifdef	D_3354 
	        if(CtmConfig::GetInstance()->GetRecordType() == 1)
				CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ALARM, wnIndex, &m_MsgCurrent, sizeof(m_MsgCurrent));
			else
	        	CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, wnIndex, &m_MsgCurrent);//2009-5-21
	        #else
	        CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, wnIndex, &m_MsgCurrent);//2009-5-21
	        #endif

	        SendMsg(MSG_USER_SH_ERROR_FIX, wnIndex, m_iRealIndex, NULL);	//2009-5-21
	        ChangeErrorList(dwID, wSource, 0, 0);
	        pthread_mutex_unlock(&m_MutexErrMsg);
	        return TRUE;
	     }
	}

	ChangeErrorList(dwID, wSource, 0, 0);
	SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF, NULL);	//2009-5-21
	pthread_mutex_unlock(&m_MutexErrMsg);
	
	/*2017-5-11 修正當只出現提示性警報并消除時，警報圖標也會出現閃一下的問題*/
	if(iAlarmNum > 0)
		iAlarmNum--;
	if(iAlarmNum == 0)
	{
	  g_wErrorStatus = FALSE;
	}
	
	return FALSE;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::GetErrorIndex()             	     		 |
|  Task     : Get Error Index						     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:         		            -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void*	CtmControl_Error::GetErrorIndex()
{
	return &m_MsgIndex;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::GetErrorLog(int nIndex)     	     		 |
|  Task     : Get Error log							     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:  nIndex 		            - Error ID                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void*	CtmControl_Error::GetErrorLog(int nIndex)
{
	memset(&m_MsgCurrent, 0, sizeof(m_MsgCurrent));
	if (nIndex > m_MsgIndex.wCount) return &m_MsgCurrent;
	nIndex = m_MsgIndex.wIndex - nIndex;
	if (nIndex < 0) nIndex += CtmConfig::GetInstance()->GetErrorMaxNum();
	#ifdef	D_3354
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_ALARM, nIndex + 1, &m_MsgCurrent, sizeof(m_MsgCurrent));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ERROR_SRAMFILE, nIndex + 1, &m_MsgCurrent);
	#else
	CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ERROR_SRAMFILE, nIndex + 1, &m_MsgCurrent);
	#endif

	return &m_MsgCurrent;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::GetErrorLogCount()			  	     		 |
|  Task     : Get Error log	Count					     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                         			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD        CtmControl_Error::GetErrorLogCount()
{
	return	m_MsgIndex.wCount;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Error::Init_ErrorLog()				  	     		 |
|  Task     : InitError log						     	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                         			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CtmControl_Error::Init_ErrorLog()
{
	pthread_mutex_init(&m_MutexErrMsg, NULL);
	#ifdef	D_3354
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_ALARM))
		{
			//printf("Init Clear 1\n");
			memset(&m_MsgIndex, 0, sizeof(tmDB_ERROR));
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ALARM, 0, &m_MsgIndex, sizeof(m_MsgIndex));
		}
		else
		{
			//printf("Init Clear 2\n");
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_ALARM, 0, &m_MsgIndex, sizeof(m_MsgIndex));
		}
	}
	else
	{
	    if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_ERROR_SRAMFILE))
	    {
		    memset(&m_MsgIndex, 0, sizeof(tmDB_ERROR));
		    CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, 0, &m_MsgIndex);
	    }
	    else
	    {
		    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ERROR_SRAMFILE, 0, &m_MsgIndex);
	    }
	}
	#else
	if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_ERROR_SRAMFILE))
	{
	    memset(&m_MsgIndex, 0, sizeof(tmDB_ERROR));
	    CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ERROR_SRAMFILE, 0, &m_MsgIndex);
	}
	else
	{
	    CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ERROR_SRAMFILE, 0, &m_MsgIndex);
	}
	#endif
}


int	CtmControl_Error::GetTemperErrorStatus()
{
	char        sz[64];
	int         TemperStatus[10];
	int         Temp=0;
	for(int i = 0; i < 9; i++)
	{
		sprintf(sz,"TEMP_TMP1_HEATERONOFF_CHL%d_REALSTATUS",i+1);
		TemperStatus[i] = GetDBValue(sz).lValue & 0x0003;
		if((TemperStatus[i] != 0)&&(TemperStatus[i] != 1))
		{
			Temp =i+1;
			break;
		}
	}
	return Temp;
}

int	CtmControl_Error::GetMoldTemperErrorStatus()
{
	char        sz[64];
	int         TemperStatus[13];
	int         Temp=0;
	for(int i = 0; i < 12; i++)
	{
		sprintf(sz,"TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS",i+1);
		TemperStatus[i] = GetDBValue(sz).lValue & 0x0003;
		if((TemperStatus[i] != 0)&&(TemperStatus[i] != 1))
		{
			Temp =i+1;
			break;
		}
	}
	return Temp;
}

#ifndef	D_BEOBJECT_CTMCONTROL_ERROR
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
