/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewAlarmHistory.h"
#include	"../taskcnc.h"
#include	"../tmshmsg.h"
 
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )

#define ALARM_MAX_NUM 30

/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
CtmWnd*		u_pwndStaticAlarmNO[ALARM_MAX_NUM] 		=	{NULL};
int				u_nStaticAlarmNOCnt										= 0;
CtmWnd*		u_pwndStaticAlarmGroup[ALARM_MAX_NUM] =	{NULL};
int				u_nStaticAlarmGroupCnt								= 0;
CtmWnd*		u_pwndStaticAlarmID[ALARM_MAX_NUM] 		=	{NULL};
int				u_nStaticAlarmIDCnt										= 0;
CtmWnd*		u_pwndStaticAlarmDate[ALARM_MAX_NUM] 	=	{NULL};
int				u_nStaticAlarmDateCnt									= 0;
CtmWnd*		u_pwndStaticAlarmTime[ALARM_MAX_NUM] 	=	{NULL};
int				u_nStaticAlarmTimeCnt									= 0;
CtmWnd*		u_pwndStaticAlarmSay[ALARM_MAX_NUM] 	=	{NULL};
int				u_nStaticAlarmSayCnt									= 0;

CtmWnd*		u_pwndEditStartNO 										=	NULL;


int				StartNO						=	1;
int				MaxNO							=	1;
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/


BOOL		OnCreateA(CtmWnd* pwndSender)
{
	
	u_nStaticAlarmNOCnt			= GetSpecialControlNum(pwndSender, "HistoryNO", "CtmStaticX2",		u_pwndStaticAlarmNO 		);
	u_nStaticAlarmGroupCnt	= GetSpecialControlNum(pwndSender, "HistoryGroup", "CtmStaticX2",	u_pwndStaticAlarmGroup 	);
	u_nStaticAlarmIDCnt			= GetSpecialControlNum(pwndSender, "HistoryID", "CtmStaticX2",		u_pwndStaticAlarmID 		);
	u_nStaticAlarmDateCnt		= GetSpecialControlNum(pwndSender, "HistoryDate", "CtmStaticX2",	u_pwndStaticAlarmDate 	);
	u_nStaticAlarmTimeCnt		= GetSpecialControlNum(pwndSender, "HistoryTime", "CtmStaticX2",	u_pwndStaticAlarmTime 	);
	u_nStaticAlarmSayCnt		= GetSpecialControlNum(pwndSender, "HistorySay", "CtmStaticX2",		u_pwndStaticAlarmSay 		);
	
	
	u_pwndEditStartNO				= pwndSender->FindControlFromName("EditGoToNO");
	
	
	ShowAlarmHistory();
	ini();
	return	TRUE;
}	

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	return	wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
	if(wKey == _ENTER)
	{
		CtmWnd*		pwndTemp = NULL; 
		pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
		
		if(pwndTemp == u_pwndEditStartNO)
		{
			int llValue = 0;
			u_pwndEditStartNO->GetPropValueT("value", &llValue, sizeof(llValue));
			StartNO = llValue;
			ShowAlarmHistory();
		}
	}
	return	wKey;
}
void		OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
	{
		switch(wParam)
		{
			case	_F6:
				StartNO-=ALARM_MAX_NUM;
				if(StartNO < 1) StartNO = 1;
				((CtmFormView*)pwndSender)->OnLoseFocus();	
				u_pwndEditStartNO->SetPropValueT("value",StartNO);
				u_pwndEditStartNO->Update(); 
				((CtmFormView*)pwndSender)->OnGetFocus();	
				ShowAlarmHistory();
				Now_Push_Btn->CreateA();			
				Now_Push_Btn->UpdateAll();		 
				Now_Push_Btn->Update();		 
			break;
			case	_F7:
				StartNO+=ALARM_MAX_NUM;
				if(StartNO > MaxNO-ALARM_MAX_NUM+1) StartNO = MaxNO-ALARM_MAX_NUM+1;
				if(StartNO<1) StartNO=1;
				((CtmFormView*)pwndSender)->OnLoseFocus();	
				u_pwndEditStartNO->SetPropValueT("value",StartNO);
				u_pwndEditStartNO->Update();
				((CtmFormView*)pwndSender)->OnGetFocus();	
				ShowAlarmHistory();
				Now_Push_Btn->CreateA();			
				Now_Push_Btn->UpdateAll();		
				Now_Push_Btn->Update();		
			break;
		}
	}
}


void		OnUpdateA(CtmWnd* pwndSender)
{
	ShowAlarmHistory();
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/

void		ShowAlarmHistory()		//顯示機械手臂目前的座標位置  copy from ViewManualMode.cpp
{
	static	WORD	wStartNo_Old =0;
	if((wStartNo_Old == StartNO)&& wStartNo_Old != 0) return;
  wStartNo_Old = StartNO;
    
    
  WORD		nCount;
	tmDB_ERROR*	pErrorMsg;
	
	char Temp[128];
	char szTemp[256];
	
	pErrorMsg 	= (tmDB_ERROR*)(g_ptmControlServer->ErrorControl()->GetErrorIndex());
	nCount 			= (short)pErrorMsg->wCount;
	printf("ShowAlarmHistory 333 nCount=%d\n",nCount);
	MaxNO = nCount;
	for(int i = 0 ; i < ALARM_MAX_NUM ; i++)
	{
		if ((pErrorMsg = (tmDB_ERROR*)(g_ptmControlServer->ErrorControl()->GetErrorLog(i+StartNO))) == NULL)
		{
      u_pwndStaticAlarmNO[i]->SetPropValueT("text","ROBOT_STR_DUMMY");
      u_pwndStaticAlarmGroup[i]->SetPropValueT("text","ROBOT_STR_DUMMY");
      u_pwndStaticAlarmID[i]->SetPropValueT("text","ROBOT_STR_DUMMY");
      u_pwndStaticAlarmDate[i]->SetPropValueT("text","ROBOT_STR_DUMMY");
      u_pwndStaticAlarmTime[i]->SetPropValueT("text","ROBOT_STR_DUMMY");
	   	u_pwndStaticAlarmSay[i] -> SetPropValueT("textID","ROBOT_STR_DUMMY");
			continue;	
		}
		if (pErrorMsg->wSource == tmOIL_ERRORFLAG)
		{
			if(u_pwndStaticAlarmNO[i] != NULL )
			{
	  		long lAlarmNo = StartNO+i;
	  		if(i<=ALARM_MAX_NUM)	//No.
	   			sprintf(Temp ,"%d",lAlarmNo);
				CodeChange(szTemp, Temp);
        u_pwndStaticAlarmNO[i]->SetPropValueT("text",szTemp);
			}
			if(u_pwndStaticAlarmGroup[i] != NULL )
			{
		  	switch(pErrorMsg->OilError.dwShotCount)	//OPER,INTER,MOTION,MLC
				{
					case ID_OPER://1:
						sprintf(Temp ,"OPER");
						break;
					case ID_INTER://2
						sprintf(Temp ,"INT");
						break;
					case ID_MOTION://3
						sprintf(Temp ,"MOT");
						break;
					case ID_MLC://4
						sprintf(Temp ,"MLC");
						break;
					case ID_DRIVERX://5
						sprintf(Temp ,"DRIVERX");
						break;
					case ID_DRIVERY://6
						sprintf(Temp ,"DRIVERY");
						break;
					case ID_DRIVERZ://7
						sprintf(Temp ,"DRIVERZ");
						break;
					case ID_DRIVERA://8
						sprintf(Temp ,"DRIVERA");
						break;
					case ID_DRIVERB://9
						sprintf(Temp ,"DRIVERB");
						break;
					case ID_DRIVERC://10
						sprintf(Temp ,"DRIVERC");
						break;
					case ID_DSP://11
						sprintf(Temp ,"PLC");
						break;
					default:
						break;	
				}
				CodeChange(szTemp, Temp);
        u_pwndStaticAlarmGroup[i]->SetPropValueT("text",szTemp);
			}
			if(u_pwndStaticAlarmID[i] != NULL )
			{
				//sprintf(Temp ,"0x%X",pErrorMsg->OilError.wID);
				sprintf(Temp ,"%04d",pErrorMsg->OilError.dwID);
				CodeChange(szTemp, Temp);
        u_pwndStaticAlarmID[i]->SetPropValueT("text",szTemp);
			}
			if(u_pwndStaticAlarmDate[i] != NULL )
			{
				sprintf(Temp ,"%02d/%02d/%02d",pErrorMsg->OilError.dateStart.year%100,pErrorMsg->OilError.dateStart.month, pErrorMsg->OilError.dateStart.day);
	   		CodeChange(szTemp, Temp);
        u_pwndStaticAlarmDate[i]->SetPropValueT("text",szTemp);
			}
			
			if(u_pwndStaticAlarmTime[i] != NULL )
			{
				sprintf(Temp ,"%02d:%02d:%02d",pErrorMsg->OilError.timeStart.hour, pErrorMsg->OilError.timeStart.minute, pErrorMsg->OilError.timeStart.second);
	    	CodeChange(szTemp, Temp);
        u_pwndStaticAlarmTime[i]->SetPropValueT("text",szTemp);
			}
				
			if(u_pwndStaticAlarmSay[i] != NULL )
			{
				int 	nErrorStrIndex =-1;
				char	szErrorText[40];
    		BOOL	ShowErrorNo=FALSE;
				szErrorText[0]='\0';
				//u_pwndStaticAlarmSay[i] -> SetPropValueT("textID","ROBOT_STR_DUMMY");
				//u_pwndStaticAlarmSay[i] -> Update();
				switch(pErrorMsg->OilError.dwShotCount)	//OPER,INTER,MOTION,MLC
				{
					case ID_OPER: //CNC_ALARM_OP://1:
						sprintf(szErrorText, "ROBOT_ALARM_OPER%03d", pErrorMsg->OilError.dwID);
						break;
					case ID_INTER: //CNC_ALARM_MLC://2
						sprintf(szErrorText, "ROBOT_ALARM_INTER%03d", pErrorMsg->OilError.dwID);
						break;
					case ID_MOTION: //CNC_ALARM_INT://3
						sprintf(szErrorText, "ROBOT_ALARM_MOT%03d", pErrorMsg->OilError.dwID);
						break;
					case ID_MLC: //CNC_ALARM_MOT://4
						sprintf(szErrorText, "ROBOT_ALARM_MLC%03d", pErrorMsg->OilError.dwID);
						break;
					case ID_DRIVERX: //CNC_ALARM_DRIVERX://5
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_AXIS1_001");
						ShowErrorNo = TRUE;
						break;
					case ID_DRIVERY: //CNC_ALARM_DRIVERY://6
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_AXIS2_001");
						ShowErrorNo = TRUE;
						break;
					case ID_DRIVERZ: //CNC_ALARM_DRIVERZ://7
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_AXIS3_001");
						ShowErrorNo = TRUE;
						break;
					case ID_DRIVERA: //CNC_ALARM_DRIVERA://8
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_AXIS4_001");
						ShowErrorNo = TRUE;
						break;
					case ID_DRIVERB: //CNC_ALARM_DRIVERB://9
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_AXIS5_001");
						ShowErrorNo = TRUE;
						break;
					case ID_DRIVERC: //CNC_ALARM_DRIVERC://10
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_AXIS6_001");
						ShowErrorNo = TRUE;
						break;
					case ID_DSP: //CNC_ALARM_DSP://11
						//sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
						sprintf(szErrorText, "ROBOT_ALARM_DSP%03d", pErrorMsg->OilError.dwID);
						break;
					default:
						sprintf(szErrorText, "ROBOT_STR_DUMMY");
						break;	
				}
				nErrorStrIndex = g_MultiLanguage.GetStrKeyIndex(szErrorText);
					
				if(ShowErrorNo)
				{
					char temp[10],szTemp[20],ErrorStr[256];
					memset(temp, 0, sizeof(temp));
					memset(szTemp, 0, sizeof(szTemp));
					memset(ErrorStr, 0, sizeof(ErrorStr));
					sprintf(temp,"%d\0",pErrorMsg->OilError.dwID);
					CodeChange(szTemp,temp);
					Double_byte_String_Merger(ErrorStr,g_MultiLanguage[nErrorStrIndex],szTemp);
		    	u_pwndStaticAlarmSay[i] -> SetPropValueT("text",ErrorStr);
		    }
		    else
		    {
		    	u_pwndStaticAlarmSay[i] -> SetPropValueT("text",g_MultiLanguage[nErrorStrIndex]);
		    }
			}
		}
		u_pwndStaticAlarmNO[i] 		->Update();
		u_pwndStaticAlarmGroup[i]	->Update();
		u_pwndStaticAlarmID[i] 		->Update();
		u_pwndStaticAlarmDate[i] 	->Update();
		u_pwndStaticAlarmTime[i] 	->Update();
		u_pwndStaticAlarmSay[i] 	->Update();
	}
}

void    CodeChange(char *pDst, char *pSrc)		//輔助用函式，單字節轉化為雙字節處理
{
	//供StaticX2元件的text屬性使用(目前)
	WORD	*pwDst = (WORD *)pDst;
    
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}

void ini()
{
	int temp = MaxNO-ALARM_MAX_NUM+1;
	if(temp <= 0 ) temp = 1; 
	u_pwndEditStartNO->SetPropValueT("max",temp);
	u_pwndEditStartNO->SetPropValueT("value",1);
	u_pwndEditStartNO->Update();
}

void	Double_byte_String_Merger(char* out, char* str1 , char* str2) 
{
	if(str1 == NULL || str2 == NULL)  
	{
		//printf("NULL\n");
		return;
	}
	
	WORD *pszDst,*pszSrc;
	pszDst = (WORD*)out;
	
  pszSrc=(WORD*)str1;
	while(*pszSrc != 0) 
  {
  	*pszDst ++ = *pszSrc ++;
  }
  
  pszSrc=(WORD*)str2;
  while(*pszSrc != 0) 
  {
  	*pszDst ++ = *pszSrc ++;
  }
  out = (char*)pszDst;
}