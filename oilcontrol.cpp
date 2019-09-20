/*===========================================================================+
|  Function : Control                                                        |
|  Task     : Machine Control Utilities                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) V3.2                                                  |
|  Link     : G++(GCC) V3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/20/1998                                                     |
|  Revision : 06/20/1998                                                     |
+===========================================================================*/
#include    "utils.h"
#include    "oilcontrol.h"
#include	"database.h"
#include	"tasksiem.h"
#include	"main.h"

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/
#define     SPEECH_REPEATCOUNT  10000
#define     SPEECH_DELAYEDCOUNT 20000


/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
tmDB_EFLAG		m_dberrorflg;
tmDB_BUFFER		m_dbbuffer = 
            {
        	0xffff,                         // wOperationMode
        	0xffff,                         // wOperationStep
        	0xffff,                         // wOperationSubStep
        	0xffff,                         // Operation mode index
        	tmERROR_NULL,               // Error code index
        	FALSE,                      // Change operation mode from manual to auto
        	0,                          // Shut count after changing operation mode from manual to auto
        	0,                          // flMotorStatus;
        	0,                          // flHeaterStatus;	
        	FALSE,                      // bDelayedSpeech;
        	0,                          // lDelayedCount;
        	FALSE,                      // bRepeatSpeech;
        	0                           // lRepeatCount;
            };
            


/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
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
void        Error_Control()
{
	short       i;
	WORD        wErrorFlag;
	BYTE		cIndex	= tmERROR_NULL;
	char		cIndexMode = dbbuff.cIndexMode;
	DWORD		dwValue, dwID, dwIndex;
	
	//	fans add
	int			ERR_BEGIN = g_MultiLanguage.GetStrKeyIndex("ERR_BEGIN");
	int			ERR_0D_PLSOPNDOOR = g_MultiLanguage.GetStrKeyIndex("ERR_0D_PLSOPNDOOR");
	int			ERR_0E_PLSCLSDOOR = g_MultiLanguage.GetStrKeyIndex("ERR_0E_PLSCLSDOOR");
	
	dwID = g_pDatabase->GetDataID("dbstat.wErrorFlag[0]");
	for (i=tmMAX_OP_ERRORSAVE-1; i>=0; i--)
    {
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if ((wErrorFlag = GetDBValue(dwIndex).lValue) != 0)
        {
	        cIndex = (char)i*16 + (char)_ScanBitR(wErrorFlag);  // Scan the 1's bit from left to right (Reverse)
	        break;
        }
    }
	HoldcIndex = cIndex;        ////wy120999 add dor delay speech 100ms
	if ((BYTE)cIndex != tmERROR_NULL)
    {
	    SetDBValue("TECH_ALARMSTATUS", 0, TRUE, 0);
	    if (dbbuff.cIndexError != cIndex)
        {
        	
        	if (cIndexMode != tmMODE_AUTOADJMOLD)
            {
            	if ((cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)))               
                {
	                dbbuff.bDelayedSpeech = TRUE;
	                dbbuff.lDelayedCount  = SPEECH_DELAYEDCOUNT;   
	                dbbuff.bRepeatSpeech  = FALSE;         
                }
            	else
                {
	                dbbuff.bDelayedSpeech = FALSE;
	                ::Speech(cIndex); 
	                if ((cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0D_PLSOPNDOOR - ERR_BEGIN)))
	                    dbbuff.bRepeatSpeech  = FALSE;
	                else
                    {
	                    dbbuff.bRepeatSpeech  = TRUE;
	                    dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT; 
                    }
                }
            }

            ::Msg(g_pStr[ERR_BEGIN+cIndex]);
        	dbbuff.cIndexError = cIndex;
        	
        }
    	else 
        {
        	
        	if ((cIndexMode == tmMODE_SEMIAUTO) && (cIndex == (ERR_0E_PLSCLSDOOR - ERR_BEGIN)) && (dbbuff.bDelayedSpeech) && (dbbuff.lDelayedCount <= 0))
            {
	            ::Speech(cIndex);
	            dbbuff.bDelayedSpeech = FALSE;
	            dbbuff.bRepeatSpeech  = TRUE;
	            dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT;   
            }
        	else if ((dbbuff.bRepeatSpeech) && (dbbuff.lRepeatCount <= 0))
            {
	            ::Speech(cIndex); 
	            dbbuff.lRepeatCount   = SPEECH_REPEATCOUNT; 
            }
        	if (GetDBValue("dbsyst.wIDView").lValue == ID_VIEW_OVERVIEW)     //wy112299 add for error load again
            {

            ::Msg(g_pStr[ERR_BEGIN+(BYTE)cIndex]);
            dbbuff.cIndexError = cIndex;
            }
            
        } 
    }             
	else
    {
	    SetDBValue("TECH_ALARMSTATUS", 0, FALSE, 0);
	   
	    if (dbbuff.cIndexError != cIndex)
        {
	        ::Msg(g_pStr[ERR_BEGIN+(BYTE)cIndex]);    
	        dbbuff.cIndexError = cIndex;
        }
       
    }
    
	for (i=0; i<tmMAX_OP_ERRORSAVE; i++)
    {
    	dwIndex = g_pDatabase->DataIDToIndex(dwID+i);
    	if ((wErrorFlag = dbeflg.wErrorFlag[i] ^ GetDBValue(dwIndex).lValue))
        {
	        dbeflg.wErrorFlag[i] = GetDBValue(dwIndex).lValue;    
	        while (wErrorFlag)
            {
            	cIndex = (char)_ScanBitF(wErrorFlag);
            	if (_TestBit(GetDBValue(dwIndex).lValue, cIndex))
                {
                	if( ((cIndex + i*16) != (ERR_0E_PLSCLSDOOR - ERR_BEGIN)) 
                    	&& ((cIndex + i*16) != (ERR_0D_PLSOPNDOOR - ERR_BEGIN)))
                    AddErrorLog(cIndex + i*16);
                }
            	else
                {
                	if( ((cIndex + i*16) != (ERR_0E_PLSCLSDOOR - ERR_BEGIN))
                    	&& ((cIndex + i*16) != (ERR_0D_PLSOPNDOOR - ERR_BEGIN)))
                    FixErrorLog(cIndex + i*16);
                }
            	wErrorFlag = _ClearBit(wErrorFlag, cIndex);
            }
        }
    }
}


WORD        GetErrorLogCount()
{
	SIMD_MSG	Msg;
	
	g_ErrorFile.ReadRecord(1, &Msg);
	return	Msg.nCount;	
}

/*tmDB_ERROR*	GetErrorLog(WORD wIndex)   // wIndex = 1 is the first (nearest) record
{
	
}
*/
BOOL        AddErrorLog(WORD wID)
{
	SIMD_MSG	Msg;
	
	Msg.wID = wID;
	Msg.dwShotCount = GetDBValue("dbprod.dwShotCountCurrent").lValue;
	GetDate(&Msg.dateStart);
	GetTime(&Msg.timeStart);
	Msg.dateFixed.year = 0;
	
	if (g_ptasksiem == NULL) return FALSE;
	g_ptasksiem->SaveMsg(1, &Msg);
	return TRUE;
}

BOOL        FixErrorLog(WORD wID)
{
	
}

BOOL        ClearErrorLog()
{
	
}
