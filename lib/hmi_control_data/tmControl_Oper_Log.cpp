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
#include	"tmControl_Oper_Log.h"
#include	"../../utils.h"
#include	"../../tmdebug.h"
#include	"../../tmconfig.h"
#include	"../../iotable.h"//Andy 20081028
#include	"../libiNetComm.h"
#include	"../dsp54/tmTableG.h"
#include	"../../tmshmsg.h"
#include 	"../../tmnbmsg.h"
#include 	"../../commonaction.h" //JOYCE2011-4-11 add
#include	"../../filerecord.h"
#include "../../oper_log.h"

IMPLEMENT_DYNCREATE(CtmControl_Oper_Log, CtmControl)



//SYS_STAT_FOR_485  sysStat; 

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
#define MAX_OPER_LOG_COUNT 500


tmOper_log NowOperLog;
int wCount;
int wIndex;
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
|  Function : CtmControl_Oper_Log::CtmControl_Oper_Log()				             |
|  Task     :  CtmControl_Oper_Logºc³y¨ç¼Æ                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Oper_Log::CtmControl_Oper_Log(): CtmControl()
{

}
/*---------------------------------------------------------------------------+
|  Function : CtmControl_Oper_Log::~CtmControl_Oper_Log()				             |
|  Task     :  CtmControl_Oper_LogªRºc¨ç¼Æ                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControl_Oper_Log::~CtmControl_Oper_Log()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Oper_Log::CreateSelf()				             	 |
|  Task     :  Initialize Error Service                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Oper_Log::CreateSelf()
{
	printf("Start IniOperLog\n"); 
	Init_OperLog();
	printf("Over IniOperLog\n");
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Oper_Log::FreeSelf()				             	     |
|  Task     :  Relase Error Service                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControl_Oper_Log::FreeSelf()
{
	return 0;	
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Oper_Log::AddErrorLog(DWORD wID)               	     |
|  Task     : Add  Error log        					     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL   CtmControl_Oper_Log::AddOperLog(DWORD DBIndex , DWORD OldValue, DWORD NewValue)
{
	printf("AddOperLog\n");
	pthread_mutex_lock(&m_MutexOperLogMsg);
	
	NowOperLog.wDBIndex = DBIndex;
	NowOperLog.dOldValue = OldValue;
	NowOperLog.dNewValue = NewValue;
	GetDate(&NowOperLog.dateStart);
	GetTime(&NowOperLog.timeStart);
	
	if (wCount < MAX_OPER_LOG_COUNT) 
		wCount++;
	
	CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPER_LOG, wIndex+1, &NowOperLog, sizeof(tmOper_log));

	if ((++wIndex) >= MAX_OPER_LOG_COUNT) 
		wIndex = 0; 		
		
	tmFileRecordHeadInfo	HeadInfo;
	HeadInfo.nCount = wCount;
	HeadInfo.nIndex = wIndex;
	CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPER_LOG, 0, &HeadInfo, sizeof(HeadInfo));

	pthread_mutex_unlock(&m_MutexOperLogMsg);
	
	return TRUE;
}
/*---------------------------------------------------------------------------+
|  Function : BOOL    CtmControl_Oper_Log::ClearErrorLog()               	     |
|  Task     : Clear Error log        					     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL    CtmControl_Oper_Log::ClearOperLog()
{
	pthread_mutex_lock(&m_MutexOperLogMsg);
	
	tmFileRecordHeadInfo	HeadInfo;
	HeadInfo.nCount = wCount =0;
	HeadInfo.nIndex = wIndex =0;
	CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPER_LOG, 0, &HeadInfo, sizeof(HeadInfo));
		
	pthread_mutex_unlock(&m_MutexOperLogMsg);
	
	return TRUE;
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
void*	CtmControl_Oper_Log::GetOperLog(int nIndex)
{
	memset(&NowOperLog, 0, sizeof(NowOperLog));
	if (nIndex > MAX_OPER_LOG_COUNT)
		return &NowOperLog;
		
	nIndex = wIndex - nIndex;
	if (nIndex < 0)
		nIndex += MAX_OPER_LOG_COUNT;

	CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPER_LOG, nIndex + 1, &NowOperLog, sizeof(NowOperLog));
	return &NowOperLog;
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
WORD        CtmControl_Oper_Log::GetOperLogCount()
{
	return	wCount;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControl_Oper_Log::Init_ErrorLog()				  	     		 |
|  Task     : InitError log						     	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                         			 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CtmControl_Oper_Log::Init_OperLog()
{
	pthread_mutex_init(&m_MutexOperLogMsg, NULL);
	

	tmFileRecordHeadInfo	HeadInfo;
	if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_OPER_LOG))
	{
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPER_LOG, 0, &HeadInfo, sizeof(HeadInfo)); 
	}
	else
	{
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPER_LOG, 0, &HeadInfo, sizeof(HeadInfo));
	}
	
	wCount = HeadInfo.nCount;
	wIndex = HeadInfo.nIndex;
}

#ifndef	D_BEOBJECT_CTMCONTROL_OPER_LOG
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
