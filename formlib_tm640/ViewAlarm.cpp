/*===========================================================================+
|  Class    : ViewAlarm library                                              |
|  Task     : ViewAlarm library action source file                           |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                	     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                  		   |
|  Revision :           			                                     			     |  
+===========================================================================*/
#include 	"ViewAlarm.h"
#include 	"../commonaction.h"
#include	"../siemmessage.h"
#include	"../utils.h"
#include	"../selectedit.h"
#include	"../font.h"
#include	"../tmcontrol.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define	    EDIT_STARTNO			"editSTARTNO"			//	Start Errro NO.    
#define	    EDIT_RESET				"editRESET"				//	Reset              
#define	    EDIT_TOTALCOUNT 		"editTOTALCOUNT"		//	Error Total Count  
#define	    STATIC_NAME				"StaticTABLE"           //  The Static in Table

#define     ERROR_ROWS              20
#define	    ERROR_COLS              7
#define	    ERROR_LEN               512
#define     MAX_STATICTABLE_SIZE    50

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
char		m_szTable[ERROR_COLS * ERROR_ROWS][ERROR_LEN];
char 		m_acStaticNameTemp[MAX_STATICTABLE_SIZE];
short		m_nTableStaticCount[MAX_STATICTABLE_SIZE];
short		m_nCurrentTableRows = 0;

BOOL		g_bUpdate = TRUE;
int			g_nOldCount  = 0;
int			g_nOldIndex	 = 0;
long long	g_nOldStartNo= 0;
long		g_lUpdateCounter = 0;
long long	Count0 =0;
long long	Count1 =1;
WORD		g_awFixTimeFlag[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int			iStaticInfoNum = 0;

CtmWnd*		Static_Info[256] = {NULL};

CtmWnd*     pwndSTARTNO = NULL;
CtmWnd*		pwndRESET = NULL;
CtmWnd*		pwndTOTALCOUNT  = NULL;
CtmWnd*		pwndLine = NULL;

CtmWnd*			pwndButton_IO1       = NULL;
CtmWnd*			pwndButton_IO2       = NULL;
CtmWnd*			pwndButton_Alarm     = NULL;    

/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{	
	pwndButton_IO1     = pwndSender->FindControlFromName("Button_IO1"); 
	pwndButton_IO2     = pwndSender->FindControlFromName("Button_IO2");  
	pwndButton_Alarm   = pwndSender->FindControlFromName("Button_Alarm");  
	
    char    msz[4];
    int     i = 0;
       	
	pwndSTARTNO = pwndSender->FindControlFromName(EDIT_STARTNO);
	pwndRESET = pwndSender->FindControlFromName(EDIT_RESET);    
	pwndTOTALCOUNT = pwndSender->FindControlFromName(EDIT_TOTALCOUNT);	
	
	for(i=0; i<MAX_STATICTABLE_SIZE; i++)
    {
        sprintf(msz, "%02d", i);
        strcpy(m_acStaticNameTemp, STATIC_NAME);
        strcat(m_acStaticNameTemp, msz);
        m_nTableStaticCount[i] = GetSpecialControlNum(pwndSender, m_acStaticNameTemp, "CtmStaticX2", &Static_Info[ERROR_COLS*i]);
        if(m_nTableStaticCount[i] > 0)    m_nCurrentTableRows++;
    }
	//iStaticInfoNum = GetSpecialControlNum(pwndSender, STATIC_NAME, "CtmStaticX2", Static_Info[ERROR_COLS*);
	
	pwndLine = pwndSender->FindControlFromName("LineTableTEMP");
	if(g_pFont-> GetFont(0)->GetFontCode() == tmFT_CODE_TECH)
	    CreateSetMul(pwndSender);
	else    CreateSet(pwndSender);
	
	if (pwndSTARTNO != NULL)
	{
		pwndSTARTNO->GetPropValueT("value", &g_nOldStartNo,sizeof(g_nOldStartNo));
		//g_nOldStartNo = *(long long*)pwndSTARTNO->GetPropValue("value");
	}
	
	AddTimer(&g_lUpdateCounter);
	
	return TRUE;
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	long long TempValue =0;
	CtmWnd* pwnd = NULL;
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)		return wIDControl;
	
	if (strcmp(pwnd->Name, EDIT_RESET) == 0)	//	Reset
	{
		if (pwndRESET != NULL)	
		{
			if(pwndRESET->Is("CtmEditX1"))
				pwndRESET->GetPropValueT("value", &TempValue,sizeof(TempValue));
			else if(pwndRESET->Is("CtmSelectEdit"))
				TempValue = ((CtmSelectEdit*)pwndRESET)->GetIndex();
		}
			
		if (TempValue == 1)
		{
			MsgBox(g_MultiLanguage["VW_HUNTER_RESETALARM"], tmFT_CODE_TECH);
			{
				if (pwndRESET != NULL)
				{
					if(pwndRESET->Is("CtmEditX1"))
						pwndRESET->SetPropValueT("value", Count0);
					else if(pwndRESET->Is("CtmSelectEdit"))
					   ((CtmSelectEdit*)pwndRESET)->SetIndex(Count0);
					pwndRESET->Update();
				}				
				if (pwndSTARTNO != NULL)
				{
	        		pwndSTARTNO->SetPropValueT("value", Count1);
	        		pwndSTARTNO->SetPropValueT("max", Count1);
	        	 	pwndSTARTNO->Update();
	      		}	            
	      		g_ptmControlServer->ErrorControl()->ClearErrorLog(); 		//2008-8-28, ClearErrorLog();
	      		OnUpdateA(pwndSender);
	    	}
		}			
	}	
	
    return wIDControl;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	int       nCount;
//	long long nIndex;
	long long TempValue =0;
//	int 	  Temp;
	CtmWnd* pwnd = NULL;
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)		return wIDControl;
	
//		if (strcmp(pwnd->Name, EDIT_STARTNO) == 0)	//	Amend StartNO of the table
    if (pwnd == pwndSTARTNO)
		{
			nCount = g_ptmControlServer->ErrorControl()->GetErrorLogCount();	//2008-8-28, nCount = GetErrorLogCount();
			if (pwndSTARTNO != NULL) 
			{
				pwndSTARTNO->GetPropValueT("value", &TempValue,sizeof(TempValue));
				//TempValue = *(long long*)(pwndSTARTNO->GetPropValue("value"));
			}
			
			if (g_nOldStartNo != TempValue) g_bUpdate = TRUE;
	    	else g_bUpdate = FALSE;
	        
	        g_nOldStartNo = TempValue;

	        if (g_bUpdate)
	        	OnUpdateA(pwndSender);
		}

	return wIDControl;
}
void  OnDestroyA(CtmWnd* pwndSender)
{
		DeleteTimer(&g_lUpdateCounter);
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	int i = 0;
	
	if (g_lUpdateCounter < 0)
	{
//		printf("UpdateCounter: %ld, g_bUpdate:%d\n",g_lUpdateCounter,g_bUpdate);
		if(g_pFont-> GetFont(0)->GetFontCode() == tmFT_CODE_TECH)
	        CreateSetMul(pwndSender);
	  else    CreateSet(pwndSender);
		if (g_bUpdate)
		{
			//pwndSender->Clear(rect);
			if (pwndTOTALCOUNT != NULL) pwndTOTALCOUNT->Update();
			
			for (i = 0; i < ERROR_COLS*m_nCurrentTableRows; i++)
			{
				if(Static_Info[i] != NULL)
				{
					Static_Info[i]->SetPropValueT("text", m_szTable[i]);
					Static_Info[i]->Update();
				}
			}
			g_bUpdate = FALSE;
			if (pwndLine != NULL)
			pwndLine->Update();
		}
		g_lUpdateCounter = 400;
	}
}

void	OnShowA(CtmWnd* pwndSender)
{
		//DrawTableAlarm(pwndSender); //Sunny<20060728>
		return;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
//		case 0:
//			::PutCommand("Internal_IO.txt");
//			break;
//		case 1:
//			::PutCommand("Remote_IO.txt");
//			break;
//		case 2:
//			break;
		case 70: // Home
			::PutCommand("Over.txt");
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
		default:
			break;
	}
		if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
		else return _NULL_KEY;
}
/*---------------------------------------------------------------------------+
|  Function : OnMouseUp()                       	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	//printf("OnMouseUp\n");
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_IO1)
	{
		//printf("pwndButton_IO2\n");
		::PutCommand("Internal_IO.txt");
	}
	if(pwnd ==pwndButton_IO2)
	{
		//printf("pwndButton_IO2\n");
		::PutCommand("Remote_IO.txt");
	}
	/*
	if(pwnd ==pwndButton_Moni)
	{
		::PutCommand("Monitor.txt");
	}*/
	
  return wIDControl;	
}


/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void    CodeChange(char *pDst, char *pSrc)
{
    WORD    *pwDst  = (WORD *)pDst;
    
    while(*pSrc != 0)      *pwDst ++ = *pSrc ++;
    
    *pwDst  = 0;
    
}

void    CreateSetMul(CtmWnd* pwndSender)
{
  	int			i = 0;	//, j = 0;
	int			nIndex = 0;
	long long	tempValue;
	
	char     szTemp[512];
//	WORD    *pwDst, *pwSrc;
	
	short		nCount;
	tmDB_ERROR*	pErrorMsg;
	tmTIME		tmTime;
	tmDATE		tmDate;
	//int			ERR_BEGIN = g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");
	
	GetDate(&tmDate);
	GetTime(&tmTime);	
	if (pwndSTARTNO != NULL) 
	{
		pwndSTARTNO->GetPropValueT("value", &nIndex,sizeof(nIndex));
		//nIndex = *(long long*)pwndSTARTNO->GetPropValue("value");
	}
	pErrorMsg = (tmDB_ERROR*)(g_ptmControlServer->ErrorControl()->GetErrorIndex());	//2008-9-28, pErrorMsg = GetErrorIndex();
	nCount = (short)pErrorMsg->wCount;
	
	tempValue = nCount;
	if (pwndTOTALCOUNT != NULL)
		pwndTOTALCOUNT->SetPropValueT("value", tempValue);
	
	if (pwndSTARTNO != NULL)
	{
//		long long minValue =0;
//		if (tempValue >0) minValue = 1;
//		pwndSTARTNO->SetPropValueT("min", minValue);
		
		if (tempValue >0)
		  pwndSTARTNO->SetPropValueT("max", tempValue);
		else
			pwndSTARTNO->SetPropValueT("max", 1);
		pwndSTARTNO->Update();
	}	
	if (pErrorMsg->wCount != g_nOldCount || pErrorMsg->wIndex != g_nOldIndex)
	{
		g_bUpdate = TRUE;
		g_nOldCount = pErrorMsg->wCount;
		g_nOldIndex = pErrorMsg->wIndex;
	}
	
	for (i = 0; i < m_nCurrentTableRows; i++)
	{
    if( nIndex == 0)  nIndex=1;
//		printf("error index =%d, count=%d, i+nIndex =%d\n", i, nCount, i+ nIndex);
		if (i + nIndex > nCount || nCount == 0 ) break;
		if ((pErrorMsg = (tmDB_ERROR*)(g_ptmControlServer->ErrorControl()->GetErrorLog(i+nIndex))) == NULL)  	//2008-9-28, if ((pErrorMsg = GetErrorLog(i+nIndex)) == NULL) 
			break;	

		//printf("pErrorMsg->wSource=%d, %d\n", pErrorMsg->wSource, tmOIL_ERRORFLAG);
			if ((pErrorMsg->wSource == tmOIL_ERRORFLAG) || (pErrorMsg->wSource == tmCOM2_ERRORFLAG))
		{
			sprintf(szTemp, "%3d"			, i+nIndex);	
			//printf("get index=%s\n", szTemp);		
			CodeChange(m_szTable[i*ERROR_COLS+0], szTemp);
			sprintf(szTemp, "%2ld"			, pErrorMsg->OilError.dwID);
			CodeChange(m_szTable[i*ERROR_COLS+1], szTemp);
			sprintf(szTemp, "%6lu"			, pErrorMsg->OilError.dwShotCount);
			CodeChange(m_szTable[i*ERROR_COLS+2], szTemp);		

			//ZHBA 2011-4-15 ParseErrorID(m_szTable[i*ERROR_COLS+3], pErrorMsg->OilError.dwID);
			g_ptmControlServer->ErrorControl()->ParseErrorID(m_szTable[i*ERROR_COLS+3], pErrorMsg->OilError.dwID);
			sprintf(szTemp, "%02d:%02d:%02d"	, pErrorMsg->OilError.timeStart.hour, pErrorMsg->OilError.timeStart.minute, pErrorMsg->OilError.timeStart.second);
			CodeChange(m_szTable[i*ERROR_COLS+4], szTemp);
			sprintf(szTemp, "%02d/%02d/%02d"	, pErrorMsg->OilError.dateStart.month, pErrorMsg->OilError.dateStart.day, pErrorMsg->OilError.dateStart.year%100);
			CodeChange(m_szTable[i*ERROR_COLS+5], szTemp);
			if (pErrorMsg->OilError.dateFixed.year != 0)
			{
				sprintf(szTemp, "%02d:%02d:%02d", pErrorMsg->OilError.timeFixed.hour, pErrorMsg->OilError.timeFixed.minute, pErrorMsg->OilError.timeFixed.second);
				CodeChange(m_szTable[i*ERROR_COLS+6], szTemp);
				if (g_awFixTimeFlag[i] != 1) 
				{
					g_awFixTimeFlag[i] = 1;
					g_bUpdate = TRUE;
				}
		    }
		    else
			{
		        strcpy (szTemp, "??:??:??");
		        CodeChange(m_szTable[i*ERROR_COLS+6], szTemp);
				if (g_awFixTimeFlag[i] != 2) 
				{
					g_awFixTimeFlag[i] = 2;
					g_bUpdate = TRUE;
				}
			}									
		}
		else if (pErrorMsg->wSource == tmSIM_ERRORFLAG)
		{
			//printf("Oil Error Message !\n");
		}
		else if(pErrorMsg->wSource == tmAUSTONE_ERRORFLAG || pErrorMsg->wSource == tmN2C_ERRORFLAG || pErrorMsg->wSource == tmM3_ERRORFLAG 
			      || pErrorMsg->wSource == tmMNT_ERRORFLAG)		//James add 2011/3/11 ¤W¤È 09:05:52
		{
			sprintf(szTemp, "%3d"			, i+nIndex);	
			//printf("get index=%s\n", szTemp);		
			CodeChange(m_szTable[i*ERROR_COLS+0], szTemp);
			sprintf(szTemp, "%2ld"			, pErrorMsg->OilError.dwID & 0xFFFF);
			CodeChange(m_szTable[i*ERROR_COLS+1], szTemp);
			
			sprintf(szTemp, "%6lu"			, pErrorMsg->OilError.dwShotCount);
			CodeChange(m_szTable[i*ERROR_COLS+2], szTemp);
			
			//ZHBA 2011-4-15  ParseErrorID(m_szTable[i*ERROR_COLS+3], pErrorMsg->OilError.dwID);
			g_ptmControlServer->ErrorControl()->ParseErrorID(m_szTable[i*ERROR_COLS+3], pErrorMsg->OilError.dwID);
			sprintf(szTemp, "%02d:%02d:%02d"	, pErrorMsg->OilError.timeStart.hour, pErrorMsg->OilError.timeStart.minute, pErrorMsg->OilError.timeStart.second);
			CodeChange(m_szTable[i*ERROR_COLS+4], szTemp);
			
			sprintf(szTemp, "%02d/%02d/%02d"	, pErrorMsg->OilError.dateStart.month, pErrorMsg->OilError.dateStart.day, pErrorMsg->OilError.dateStart.year%100);
			CodeChange(m_szTable[i*ERROR_COLS+5], szTemp);
			
			if (pErrorMsg->OilError.dateFixed.year != 0)
			{
				sprintf(szTemp, "%02d:%02d:%02d", pErrorMsg->OilError.timeFixed.hour, pErrorMsg->OilError.timeFixed.minute, pErrorMsg->OilError.timeFixed.second);
				CodeChange(m_szTable[i*ERROR_COLS+6], szTemp);
				if (g_awFixTimeFlag[i] != 1) 
				{
					g_awFixTimeFlag[i] = 1;
					g_bUpdate = TRUE;
				}
		    }
		    else
			{
		        strcpy (szTemp, "??:??:??");
		        CodeChange(m_szTable[i*ERROR_COLS+6], szTemp);
				if (g_awFixTimeFlag[i] != 2) 
				{
					g_awFixTimeFlag[i] = 2;
					g_bUpdate = TRUE;
				}
			}
		}		
	}			
	for (i = i; i < m_nCurrentTableRows; i++)
	{
		sprintf(szTemp, "%3d", i+nIndex);
		CodeChange(m_szTable[i*ERROR_COLS+0], szTemp);
		sprintf(szTemp, "%2d", 0);
		CodeChange(m_szTable[i*ERROR_COLS+1], szTemp);
		sprintf(szTemp, "%6lu", (DWORD)0);
		CodeChange(m_szTable[i*ERROR_COLS+2], szTemp);
		strcpy(szTemp, "");
		CodeChange(m_szTable[i*ERROR_COLS+3], szTemp);
		CodeChange(m_szTable[i*ERROR_COLS+4], szTemp);
		CodeChange(m_szTable[i*ERROR_COLS+5], szTemp);
		CodeChange(m_szTable[i*ERROR_COLS+6], szTemp);
	}
	
}


void       CreateSet(CtmWnd* pwndSender)
{
	int			i = 0;
	int			nIndex = 0;
	long long	tempValue = 0;
	
	short		nCount = 0;
	tmDB_ERROR*	pErrorMsg;
	tmTIME		tmTime;
	tmDATE		tmDate;
	//int			ERR_BEGIN = g_MultiLanguage.GetStrKeyIndex("ERR_0F_TEMPERR");

	GetDate(&tmDate);
	GetTime(&tmTime);	
	if (pwndSTARTNO != NULL) 
	{
		pwndSTARTNO->GetPropValueT("value", &nIndex,sizeof(nIndex));
		//nIndex = *(long long*)pwndSTARTNO->GetPropValue("value");
	}
	pErrorMsg = (tmDB_ERROR*)(g_ptmControlServer->ErrorControl()->GetErrorIndex());	//2008-9-28, pErrorMsg = GetErrorIndex();
	nCount = (short)pErrorMsg->wCount;
	
	tempValue = nCount;
	if (pwndTOTALCOUNT != NULL)
		pwndTOTALCOUNT->SetPropValueT("value", tempValue);
	
	if (pwndSTARTNO != NULL)
	{
		long long minValue =1;
		if (tempValue >0) minValue = 1;
		pwndSTARTNO->SetPropValueT("min", minValue);
		
		pwndSTARTNO->SetPropValueT("max", tempValue);
		pwndSTARTNO->Update();
	}	
	if (pErrorMsg->wCount != g_nOldCount || pErrorMsg->wIndex != g_nOldIndex)
	{
		g_bUpdate = TRUE;
		g_nOldCount = pErrorMsg->wCount;
		g_nOldIndex = pErrorMsg->wIndex;
	}		
	
	for (i = 0; i < m_nCurrentTableRows; i++)
		{
		  if( nIndex == 0)  nIndex=1;
		  if (i + nIndex > nCount || nCount == 0) break;
		  if ((pErrorMsg = (tmDB_ERROR*)(g_ptmControlServer->ErrorControl()->GetErrorLog(i+nIndex))) == NULL) break;	//2008-9-28, if ((pErrorMsg = GetErrorLog(i+nIndex)) == NULL) break;	
		  if (pErrorMsg->wSource == tmOIL_ERRORFLAG)
			{
			  sprintf(m_szTable[i*ERROR_COLS+0], "%3d"			, i+nIndex);
			  sprintf(m_szTable[i*ERROR_COLS+1], "%2ld"			, pErrorMsg->OilError.dwID);
			  sprintf(m_szTable[i*ERROR_COLS+2], "%6lu"			, pErrorMsg->OilError.dwShotCount);
			  //sprintf(m_szTable[i*ERROR_COLS+3], "%-30.30s"		, g_MultiLanguage[/*ERR_BEGIN+*/pErrorMsg->OilError.dwID]);
			  sprintf(m_szTable[i*ERROR_COLS+4], "%02d:%02d:%02d"	, pErrorMsg->OilError.timeStart.hour, pErrorMsg->OilError.timeStart.minute, pErrorMsg->OilError.timeStart.second);
			  sprintf(m_szTable[i*ERROR_COLS+5], "%02d/%02d/%02d"	, pErrorMsg->OilError.dateStart.month, pErrorMsg->OilError.dateStart.day, pErrorMsg->OilError.dateStart.year%100);
			  if (pErrorMsg->OilError.dateFixed.year != 0)
			  	{
			  	sprintf(m_szTable[i*ERROR_COLS+6], "%02d:%02d:%02d", pErrorMsg->OilError.timeFixed.hour, pErrorMsg->OilError.timeFixed.minute, pErrorMsg->OilError.timeFixed.second);
			  	if (g_awFixTimeFlag[i] != 1) 
			  		{
			  		g_awFixTimeFlag[i] = 1;
			  		g_bUpdate = TRUE;
			  		}
		         }
		      else
			  	{
		          strcpy (m_szTable[i*ERROR_COLS+6], "??:??:??");
			  	if (g_awFixTimeFlag[i] != 2) 
			  		{
			  		g_awFixTimeFlag[i] = 2;
			  		g_bUpdate = TRUE;
			  		}
			  	}									
			}
		else if (pErrorMsg->wSource == tmSIM_ERRORFLAG)
			{
			//printf("Oil Error Message !\n");
			}
		}			
	for (i=i; i<m_nCurrentTableRows; i++)
	{
		sprintf(m_szTable[ERROR_COLS*i+0], "%3d", i+nIndex);
		sprintf(m_szTable[i*ERROR_COLS+1], "%2d", 0);
		sprintf(m_szTable[i*ERROR_COLS+2], "%6lu", (DWORD)0);
		strcpy(m_szTable[ERROR_COLS *i+3], "");
		strcpy(m_szTable[ERROR_COLS *i+4], "");
		strcpy(m_szTable[ERROR_COLS *i+5], "");
		strcpy(m_szTable[ERROR_COLS *i+6], "");
	}
}

