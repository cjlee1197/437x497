/*===========================================================================+
|  Class    : View initial login machine                                     |
|  Task     : View initial login machine                                     |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#include	"ViewInstlogin.h"
#include	"../font.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

CtmWnd* pwndPassWord = NULL;
CtmWnd* pwndConfirm = NULL;
CtmWnd* pwndCancle 	= NULL;
int		m_iKey = 0;
BOOL	m_bCancelInstall =FALSE; 
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndPassWord = pwndSender->FindControlFromName("editInstLogin");
	pwndConfirm  = pwndSender->FindControlFromName("OK_Button");
	pwndCancle   = pwndSender->FindControlFromName("Cancel_Button");
	return TRUE;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	//char 	szPassWord[10];
	////long 	lTemp = 0; 
	//WORD 	wPrivilege;
	//char 	message[256];
	//tmDATE	currDate;
	//char 	szMachineID[10];
	//char	szOutData[20];
	//memset(szPassWord,0,10);
	//CtmWnd* pwnd = pwndSender->FindControlFromTab(wIDControl);
	//if(pwnd == pwndConfirm)
	//if( m_iKey == _ENTER )
	//{
	//	if(pwndPassWord != NULL)
	//		pwndPassWord->GetPropValueT("text",szPassWord,10);
	//	if(szPassWord == NULL || szPassWord[0] == '\0') return wIDControl;
	//	if(strcmp(szPassWord,"12345") == 0)
	//	{
	//		;//printf("password is %s\n",szPassWord);
	//	}
	//	else if (strcmp(szPassWord,"54321") == 0)
	//	{
	//		;//printf("password is %s\n",szPassWord);
	//	}
	//	else
	//	{
	//		wPrivilege = StartUp_Install(szPassWord);
	//		if(wPrivilege == MAX_PRIVILEGE)
	//		{
	//			if(m_iKey == _ENTER)	Exit();
	//		}
	//		else if(wPrivilege == 1)
	//		{
	//          Reset();			// 有問題?光標釋放問題
	//		}
	//		else
	//		{
	//			GetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
	//			currDate.month  = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH").lValue;
	//			currDate.day    = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY").lValue;
	//			currDate.year   = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR").lValue;
	//			sprintf(szOutData,"%d/%d/%d",currDate.month,currDate.day,currDate.year);
	//			if(g_pFont16->GetFontCode() == tmFT_CODE_TECH)
	//			{
	//				WORD    *pwSrc, *pwDst;
	//				memset(message, 0, 256);
	//				pwDst   = (WORD *)message;
	//				pwSrc   = (WORD *)g_MultiLanguage["VW_TIMEOUT"];
    //        		if(pwSrc != NULL)
    //        		{
    //        		    while(*pwSrc != 0)
    //        		    {
    //        		        *pwDst = *pwSrc;
    //        		        pwDst ++;
    //        		        pwSrc ++;
    //        		    }
    //        		    *pwDst ++    = '\n';
    //        		    pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_MACHINEID"];
    //        		    while(*pwSrc != 0)
    //        		    {
    //        		        *pwDst = *pwSrc;
    //        		        pwDst ++;
    //        		        pwSrc ++;
    //        		    }
    //        		    *pwDst ++    = ':';
    //        		    pwSrc   = (WORD *)szMachineID;
    //        		    while(*pwSrc != 0)
    //        		    {
    //        		        *pwDst = *pwSrc;
    //        		        pwDst ++;
    //        		        pwSrc ++;
    //        		    }
    //        		    *pwDst ++    = '\n';
    //        		    pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_OUTDATE"];
    //        		    while(*pwSrc != 0)
    //        		    {
    //        		        *pwDst = *pwSrc;
    //        		        pwDst ++;
    //        		        pwSrc ++;
    //        		    }
    //        		    *pwDst ++    = ':';
    //        		    
    //        		    //pwSrc   = (WORD *)szOutData;
    //        		    //while(*pwSrc != 0)
    //        		    //{
    //        		    //    *pwDst = *pwSrc;
    //        		    //    pwDst ++;
    //        		    //    pwSrc ++;
    //        		    //}
    //        		}
	//				MsgBox(message,3);
	//			}
	//			else
	//			{
	//				sprintf(message,"%s\n%s:%s\n%s:%s",g_MultiLanguage["VW_TIMEOUT"]
	//											,g_MultiLanguage["DLG_INSTALL_MACHINEID"],szMachineID
	//											,g_MultiLanguage["DLG_INSTALL_OUTDATE"],szOutData);
	//				MsgBox(message);
	//			}
    //
	//		}
	//	}
	//}
	////if(pwnd ==pwndCancle)
	//if( m_iKey == _KEY_CLEAR )
	//{
	//	if(GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue	== FALSE && m_iKey == _ENTER)
	//	{
	//		Reset();		// 有問題?光標釋放問題
	//	}
	//}
	return wIDControl;
}
 
WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	char 	szPassWord[10];
	//long 	lTemp = 0; 
	WORD 	wPrivilege;
	char 	message[256] ="\0";//message[256];	Jamess
	tmDATE	currDate, UseDate;
	char 	szMachineID[10]="\0", szStages[10]="\0", szStageNow[10]="\0";
	char	szOutData[20]="\0", szUseDate[20]="\0";
	int		Stages, StageNow;	//fuxy, 2008-11-12, add
	memset(szPassWord,0,10);
	m_iKey = wKey;
	if( m_iKey == _ENTER )			//用OK C 操作 James modify 2007/9/27 09:18上午
	{
		if(pwndPassWord != NULL)
			pwndPassWord->GetPropValueT("text",szPassWord,10);
		if(szPassWord == NULL || szPassWord[0] == '\0') return 0;
		if(strcmp(szPassWord,"12345") == 0)
		{
			;//printf("password is %s\n",szPassWord);
		}
		else if (strcmp(szPassWord,"54321") == 0)
		{
			;//printf("password is %s\n",szPassWord);
		}
		else
		{
			wPrivilege = StartUp_Install(szPassWord);
			if(wPrivilege == MAX_PRIVILEGE)
			{
				if(m_iKey == _ENTER)
				{
					//m_bCancelInstall=TRUE;
					if(g_bCancelInstall)//Sunny<20080828>
					{
						long long lTemp =0;
	        			SetDBValue("OPER_HMI_NULL_NULL_STATE", lTemp);
	        			SetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG", lTemp); //JOYCE2008-10-10 add
	        			SetDBString("OPER_HMI_NULL_NULL_MACHINEID","MACH0000", 9);
	        			SetDBValue("OPER_HMI_NULL_NULL_STAGES", g_pDatabase->ReadDefault("OPER_HMI_NULL_NULL_STAGES").dwData);
	        			SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", g_pDatabase->ReadDefault("OPER_HMI_NULL_NULL_STAGENOW").dwData);
	        			g_bCancelInstall =FALSE;
					}
					Exit();
					g_WndLoginFlag = TRUE; //JOYCE2008-11-10 ADD
				}
			}
			else if(wPrivilege == 1)
			{
			  g_bCancelInstall =FALSE;	
	          Reset();			// 有問題?光標釋放問題
			}
			else
			{
				GetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
				
				Stages 		= (char)GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue;		//fuxy, 2008-11-12, add
				StageNow 	= (char)GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;	//fuxy, 2008-11-12, add
				sprintf(szStages,"%d",Stages);		//fuxy, 2008-11-12, add
				sprintf(szStageNow,"%d",StageNow);	//fuxy, 2008-11-12, add
				
				UseDate.month  = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH").lValue;	//fuxy, 2008-11-12, add
				UseDate.day    = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY").lValue;
				UseDate.year   = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR").lValue;
				sprintf(szUseDate,"%d/%d/%d",UseDate.month,UseDate.day,UseDate.year);	//fuxy, 2008-11-12, add
				
				currDate.month  = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH").lValue;
				currDate.day    = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY").lValue;
				currDate.year   = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR").lValue;
				sprintf(szOutData,"%d/%d/%d",currDate.month,currDate.day,currDate.year);
				//printf("szMachineID:%s;szOutData:%s \n",szMachineID,szOutData);
				if(g_pFont-> GetFont(0)->GetFontCode() == tmFT_CODE_TECH)
				{
					WORD    *pwSrc = NULL, *pwDst = NULL;
					memset(message, 0, 256);
					pwDst   = (WORD *)message;
					pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_MACHINEID"];//g_MultiLanguage["VW_TIMEOUT"];
            		if(pwSrc != NULL)
            		{
            		    //while(*pwSrc != 0)
            		    //{
            		    //    *pwDst = *pwSrc;
            		    //    pwDst ++;
            		    //    pwSrc ++;
            		    //}
            		    //*pwDst ++    = '\n';
            		    //pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_MACHINEID"];
            		    while(*pwSrc != 0)
            		    {
            		        *pwDst = *pwSrc;
            		        pwDst ++;
            		        pwSrc ++;
            		    }
            		    *pwDst ++    = ':';
            		    //pwSrc   = (WORD *)szMachineID;
            		    //while(*pwSrc != 0)
            		    //{
            		    //    *pwDst = *pwSrc;
            		    //    pwDst ++;
            		    //    pwSrc ++;
            		    //    //printf("pwDst: %s; pwSrc: %s \n",pwDst,pwSrc);
            		    //}
            		    
            		    char *pszMachineID = szMachineID;
            		    while (*pszMachineID != 0)
            		    {
            		    	*pwDst = *pszMachineID;
            		    	pwDst ++;
            		    	pszMachineID++;
            		    	
            		    }
            		    *pwDst ++    = '\n';
            		    pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_OUTDATE"];
            		    while(*pwSrc != 0)
            		    {
            		        *pwDst = *pwSrc;
            		        pwDst ++;
            		        pwSrc ++;
            		    }
            		    *pwDst ++    = ':';
            		    
            		    char *pszOutData =szOutData;
            		    while(*pszOutData != 0)
            		    {
            		        *pwDst = *pszOutData;
            		        pwDst ++;
            		        pszOutData ++;
            		    }
//fuxy, 2008-11-12, add, 交付日期   	    
            		    *pwDst ++    = '\n';
            		    pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_SELLDATE"];
            		    while(*pwSrc != 0)
            		    {
            		        *pwDst = *pwSrc;
            		        pwDst ++;
            		        pwSrc ++;
            		    }
            		    *pwDst ++    = ':';
            		    pwSrc = NULL;
            		    char *pszUseDate =szUseDate;
            		    while(*pszUseDate != 0)
            		    {
            		        *pwDst = *pszUseDate;
            		        pwDst ++;
            		        pszUseDate ++;
            		    }

//fuxy, 2008-11-12, add, 總期數   		    
            		    *pwDst ++    = '\n';
            		    pwSrc   = (WORD *)g_MultiLanguage["EFFECT_STAGES"];
            		    while(*pwSrc != 0)
            		    {
            		        *pwDst = *pwSrc;
            		        pwDst ++;
            		        pwSrc ++;
            		    }
            		    *pwDst ++    = ':';
            		    
            		    char *pszStages =szStages;
            		    while(*pszStages != 0)
            		    {
            		        *pwDst = *pszStages;
            		        pwDst ++;
            		        pszStages ++;
            		    }

//fuxy, 2008-11-12, add, 當前期數   		    
            		    *pwDst ++    = '\n';
            		    pwSrc   = (WORD *)g_MultiLanguage["DLG_INSTALL_STAGENOW"];
            		    while(*pwSrc != 0)
            		    {
            		        *pwDst = *pwSrc;
            		        pwDst ++;
            		        pwSrc ++;
            		    }
            		    *pwDst ++    = ':';
            		    
            		    char *pszStageNow =szStageNow;
            		    while(*pszStageNow != 0)
            		    {
            		        *pwDst = *pszStageNow;
            		        pwDst ++;
            		        pszStageNow ++;
            		    }
            		}
					MsgBox(message,3);
				}
				else
				{
					sprintf(message,"%s:%s\n %s:%s\n %s:%s\n %s:%s\n %s:%s",g_MultiLanguage["DLG_INSTALL_MACHINEID"],szMachineID
												,g_MultiLanguage["DLG_INSTALL_OUTDATE"],szOutData
												,g_MultiLanguage["DLG_INSTALL_SELLDATE"],szUseDate		//fuxy,2008-11-12,add
												,g_MultiLanguage["DLG_INSTALL_STAGES"],szStages
												,g_MultiLanguage["DLG_INSTALL_STAGENOW"],szStageNow);
					MsgBox(message);
				}
    
			}
		}
	}
	if( m_iKey == _KEY_CLEAR )
	{
		if(GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue	== FALSE)
		{
			g_bCancelInstall =FALSE;
			//Reset();		// 有問題?光標釋放問題
			g_WndLoginFlag = FALSE;//JOYCE2008-11-10 add 解決"按cancel鍵,直接跳回到主畫面,操作很不方便"的問題
	    	Exit(); 
		}
	}
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
