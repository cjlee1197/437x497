/*===========================================================================+
|  Class    : View set Stop machine                                          |
|  Task     : View set Stop machine                                          |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Leo                                                            |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#include	"ViewSetinst.h"
/*===========================================================================+
|           External                                                         |
+===========================================================================*/
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd* m_pwndEditMachineID	     = NULL;
CtmWnd* m_pwndEditOutFctyMonth   = NULL;
CtmWnd* m_pwndEditOutFctyDay     = NULL;
CtmWnd* m_pwndEditOutFctyYear    = NULL;
CtmWnd* m_pwndEditUseMonth       = NULL;
CtmWnd* m_pwndEditUseDay         = NULL;
CtmWnd* m_pwndEditUseYear        = NULL;
CtmWnd* m_pwndEditPayStages      = NULL;
CtmWnd* m_pwndEditSinTimeOutDay  = NULL;
CtmWnd* m_pwndEditStopHour       = NULL;
CtmWnd* m_pwndEditStopMinute     = NULL;
CtmWnd* m_pwndEditAlarmDays      = NULL;
CtmWnd* m_pwndEditAlmDevisMinute = NULL;
CtmWnd* m_pwndEditOperationState = NULL;
CtmWnd* m_pwndEditCurrPayStage   = NULL;
CtmWnd* m_pwndEditTimeOutMonth   = NULL;
CtmWnd* m_pwndEditTimeOutDay     = NULL;
CtmWnd* m_pwndEditTimeOutYear    = NULL;
CtmWnd* m_pwndEditNextAlmMonth   = NULL;
CtmWnd* m_pwndEditNextAlmDay     = NULL;        
CtmWnd* m_pwndEditNextAlmYear    = NULL;        
CtmWnd* m_pwndEditOperationHour  = NULL;
CtmWnd* m_pwndButCancel			 = NULL;   

CtmWnd* m_pwndEditInstWay		 = NULL;

int			m_iKey = 0;     
long long 	m_PayStages=0;
long long   OldStage;

/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
//  --  editMachineID		//  --  editPayStages     	//	--	editCurrPayStage
//  --  editOutFctyMonth    //  --  editSinTimeOutDay   //	--  editTimeOutMonth
//  --  editOutFctyDay      //  --  editStopHour        //	--  editTimeOutDay  
//  --  editOutFctyYear     //  --  editStopMinute      //	--	editTimeOutYear 
//  --  editUseMonth        //  --  editAlarmDays       //	--	editNextAlmMonth
//  --  editUseDay          //  --  editAlmDevisMinute  //	--	editNextAlmDay  
//  --  editUseYear         //  --  editOperationState  //	--	editNextAlmYear 

//	--	editOperationHour
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
void	SetDisabledControl();
/*===========================================================================+
|           Class implementation - ViewInstall                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	m_pwndEditMachineID	    = pwndSender->FindControlFromName("editMachineID");
    m_pwndEditOutFctyMonth  = pwndSender->FindControlFromName("editOutFctyMonth");
    m_pwndEditOutFctyDay    = pwndSender->FindControlFromName("editOutFctyDay");
    m_pwndEditOutFctyYear   = pwndSender->FindControlFromName("editOutFctyYear");
    m_pwndEditUseMonth      = pwndSender->FindControlFromName("editUseMonth");
    m_pwndEditUseDay        = pwndSender->FindControlFromName("editUseDay");
    m_pwndEditUseYear       = pwndSender->FindControlFromName("editUseYear");
    m_pwndEditPayStages     = pwndSender->FindControlFromName("editPayStages");
    m_pwndEditSinTimeOutDay = pwndSender->FindControlFromName("editSinTimeOutDay");
    m_pwndEditStopHour      = pwndSender->FindControlFromName("editStopHour");
    m_pwndEditStopMinute    = pwndSender->FindControlFromName("editStopMinute");
    m_pwndEditAlarmDays     = pwndSender->FindControlFromName("editAlarmDays");
    m_pwndEditAlmDevisMinute= pwndSender->FindControlFromName("editAlmDevisMinute");
    m_pwndEditOperationState= pwndSender->FindControlFromName("editOperationState");
    m_pwndEditCurrPayStage  = pwndSender->FindControlFromName("editCurrPayStage");
    m_pwndEditTimeOutMonth  = pwndSender->FindControlFromName("editTimeOutMonth");
    m_pwndEditTimeOutDay    = pwndSender->FindControlFromName("editTimeOutDay");
    m_pwndEditTimeOutYear   = pwndSender->FindControlFromName("editTimeOutYear");
    m_pwndEditNextAlmMonth  = pwndSender->FindControlFromName("editNextAlmMonth");
    m_pwndEditNextAlmDay    = pwndSender->FindControlFromName("editNextAlmDay");
    m_pwndEditNextAlmYear   = pwndSender->FindControlFromName("editNextAlmYear");
    m_pwndEditOperationHour = pwndSender->FindControlFromName("editOperationHour");
    m_pwndButCancel			= pwndSender->FindControlFromName("Cancel_Button");
    
    m_pwndEditInstWay		= pwndSender->FindControlFromName("editInstWay");
    
    m_pwndEditPayStages->GetPropValueT("value",&m_PayStages,sizeof(m_PayStages));
    
    CreateSet(pwndSender);
    
	return TRUE;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void       CreateSet(CtmWnd* pwndSender)
{
	long	lTemp;
	char    szMachineID[10];
	
	GetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
	if(m_pwndEditMachineID 	  != NULL) 
	{
		m_pwndEditMachineID->SetPropValueT("text", szMachineID);
		m_pwndEditMachineID->Update();
	}
		

	if(m_pwndEditOperationState   != NULL)
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_STATE").lValue;
		if(lTemp == 0xA5)	lTemp = 1;
		else				lTemp = 0;
		m_pwndEditOperationState->SetPropValueT("value", lTemp);
		m_pwndEditOperationState->Update();
	}
	
	SetDisabledControlOther(); //JOYCE2008-8-27 add
	OldStage = GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;
	
	if(m_pwndEditInstWay    != NULL)	//Sunny<20090421> inst way
	{
        m_pwndEditInstWay->SetPropValueT("value", (long long)g_wInstWay);
		m_pwndEditInstWay->Update();
	
	}
}

void		SetDisabledControlOther()  //JOYCE2008-8-27 add  // show data
{
	long lTemp;

  if(m_pwndEditTimeOutMonth   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH").lValue;
		m_pwndEditTimeOutMonth->SetPropValueT("value", lTemp);
	}
  if(m_pwndEditTimeOutDay   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY").lValue;
		m_pwndEditTimeOutDay->SetPropValueT("value", lTemp);
	}
	if(m_pwndEditTimeOutYear   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue;
		m_pwndEditTimeOutYear->SetPropValueT("value", (double)lTemp);
	}
	if(m_pwndEditNextAlmMonth   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH").lValue;
		m_pwndEditNextAlmMonth->SetPropValueT("value", lTemp);
	}
	if(m_pwndEditNextAlmDay   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY").lValue;
		m_pwndEditNextAlmDay->SetPropValueT("value", lTemp);
	}
	if(m_pwndEditNextAlmYear   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue;
		m_pwndEditNextAlmYear->SetPropValueT("value", (double)lTemp);
	}
	if(m_pwndEditTimeOutMonth != NULL)	m_pwndEditTimeOutMonth->Update();
  if(m_pwndEditTimeOutDay   != NULL)	m_pwndEditTimeOutDay->Update();
	if(m_pwndEditTimeOutYear  != NULL)	m_pwndEditTimeOutYear->Update();
	if(m_pwndEditNextAlmMonth != NULL)	m_pwndEditNextAlmMonth->Update();
	if(m_pwndEditNextAlmDay   != NULL)	m_pwndEditNextAlmDay->Update();
	if(m_pwndEditNextAlmYear  != NULL)	m_pwndEditNextAlmYear->Update();
}

void	SetDisabledControl()
{
	long lTemp;
	if(m_pwndEditCurrPayStage   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;
		m_pwndEditCurrPayStage->SetPropValueT("value", lTemp);
	}
	if(m_pwndEditOperationHour   != NULL)	//Disabled
	{
		lTemp = GetDBValue("OPER_HMI_NULL_NULL_MINUTE").lValue;
		lTemp = lTemp * 100 / 60;
		m_pwndEditOperationHour->SetPropValueT("value", lTemp);	
	}
	if(m_pwndEditCurrPayStage != NULL)	m_pwndEditCurrPayStage->Update();
	if(m_pwndEditOperationHour!= NULL)	m_pwndEditOperationHour->Update();

	if(OldStage != GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue)  //JOYCE2008-8-27 add
	{
  	SetDisabledControlOther();
  	OldStage = GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;
	}

}

void	OnUpdateA(CtmWnd* pwndSender)
{
    SetDisabledControl();
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	int					ntDays;
	tmDATE      dateOut, dateSell, dtTempDATE;
	BYTE        cState;
	//char			psz[10];
	WORD				nDaysTimeoutPeriod, nDaysTimeoutAlarmPeriod, nStageNow;
	CtmWnd*	    pwnd = pwndSender->FindControlFromTab(wIDControl);	
	long long		lTemp=0;	
	
	if(pwnd == m_pwndEditInstWay )	//Sunny<20090421> inst way 
	{
		m_pwndEditInstWay->GetPropValueT("value",&g_wInstWay,sizeof(g_wInstWay));
	}
	else if(pwnd == m_pwndEditSinTimeOutDay)
	{
		if(m_pwndEditUseYear != NULL)
		{
			m_pwndEditUseYear->GetPropValueT("value", &lTemp,sizeof(lTemp));
			dtTempDATE.year = (WORD)lTemp;
		}
		if(m_pwndEditUseMonth != NULL)
		{
	   	m_pwndEditUseMonth->GetPropValueT("value",&lTemp,sizeof(lTemp));
	   	dtTempDATE.month = (BYTE)lTemp;
	  }
	  if(m_pwndEditUseDay != NULL)
	  {
	  	m_pwndEditUseDay->GetPropValueT("value",&lTemp,sizeof(lTemp));
	  	dtTempDATE.day = (BYTE)lTemp;
	  }
	  cState = CvtDate(&dtTempDATE);
		switch (cState)
    {
    	case DATE_ERROR_MONTH:
       	((CtmFormView *)pwndSender)->Goto(m_pwndEditUseMonth->GetTabOrder());
       	return wIDControl;
    	case DATE_ERROR_DAY:
    		((CtmFormView *)pwndSender)->Goto(m_pwndEditUseDay->GetTabOrder());
      	return wIDControl;
    	default:
        break;
    }
    if(m_pwndEditCurrPayStage != NULL)
    {
    	m_pwndEditCurrPayStage->GetPropValueT("value",&lTemp,sizeof(lTemp));
    	nStageNow = (WORD)lTemp;
    }
    if(m_pwndEditSinTimeOutDay != NULL)
    {
    	m_pwndEditSinTimeOutDay->GetPropValueT("value",&lTemp,sizeof(lTemp));
    	nDaysTimeoutPeriod = (WORD)lTemp;
    }
    ntDays 				= nStageNow * nDaysTimeoutPeriod;
    GetAccDate((tmDATE *)&dtTempDATE, ntDays, (tmDATE *)&dateOut);
    if(m_pwndEditTimeOutMonth != NULL)
    {
    	m_pwndEditTimeOutMonth->SetPropValueT("value", (double)dateOut.month);
    	m_pwndEditTimeOutMonth->Update();
    }
    if(m_pwndEditTimeOutDay != NULL)
   	{
    	m_pwndEditTimeOutDay->SetPropValueT("value", (double)dateOut.day);
   		m_pwndEditTimeOutDay->Update();
   	}
    if(m_pwndEditTimeOutYear != NULL)
    {
    	m_pwndEditTimeOutYear->SetPropValueT("value", (double)dateOut.year);
    	m_pwndEditTimeOutYear->Update();
    }
	}
	else if(pwnd == m_pwndEditAlarmDays)
	{
		if(m_pwndEditTimeOutMonth != NULL)
			m_pwndEditTimeOutMonth->GetPropValueT("value",&dtTempDATE.month,sizeof(dtTempDATE.month));
		if(m_pwndEditTimeOutDay != NULL)
			m_pwndEditTimeOutDay->GetPropValueT("value",&dtTempDATE.day,sizeof(dtTempDATE.day));
		if(m_pwndEditTimeOutYear != NULL)
			m_pwndEditTimeOutYear->GetPropValueT("value",&dtTempDATE.year,sizeof(dtTempDATE.year));
		CvtDate(&dtTempDATE);
		if(m_pwndEditAlarmDays != NULL)
			m_pwndEditAlarmDays->GetPropValueT("value",&nDaysTimeoutAlarmPeriod,sizeof(nDaysTimeoutAlarmPeriod));
		ntDays 		     = -(int)nDaysTimeoutAlarmPeriod;
		GetAccDate((tmDATE *)&dtTempDATE, ntDays, (tmDATE *)&dateOut);
		if(m_pwndEditNextAlmMonth != NULL)
		{
			m_pwndEditNextAlmMonth->SetPropValueT("value", (double)dateOut.month);
			m_pwndEditNextAlmMonth->Update();
		}
		if(m_pwndEditNextAlmDay != NULL)
		{
			m_pwndEditNextAlmDay->SetPropValueT("value", dateOut.day);
			m_pwndEditNextAlmDay->Update();
		}
		if(m_pwndEditNextAlmYear != NULL)
		{
			m_pwndEditNextAlmYear->SetPropValueT("value", (double)dateOut.year);
			m_pwndEditNextAlmYear->Update();
		}
	}
	else if(pwnd == m_pwndEditOperationState)
	{
		m_pwndEditOperationState->GetPropValueT("value",&lTemp,sizeof(lTemp));
		//printf("lTemp:%lld \n",lTemp);
		if(lTemp == 1)
		{
			
			if(m_pwndEditOutFctyYear != NULL)
				m_pwndEditOutFctyYear->GetPropValueT("value",&dateOut.year,sizeof(dateOut.year));
			if(m_pwndEditOutFctyMonth != NULL)
		   	m_pwndEditOutFctyMonth->GetPropValueT("value",&dateOut.month,sizeof(dateOut.month));
		  if(m_pwndEditOutFctyDay != NULL)
		  	m_pwndEditOutFctyDay->GetPropValueT("value",&dateOut.day,sizeof(dateOut.day));
		  if(m_pwndEditUseYear != NULL)
		  	m_pwndEditUseYear->GetPropValueT("value",&dateSell.year,sizeof(dateSell.year));  
		  if(m_pwndEditUseMonth != NULL)
		   	m_pwndEditUseMonth->GetPropValueT("value",&dateSell.month,sizeof(dateSell.month)); 
		  if(m_pwndEditUseDay != NULL)
		   	m_pwndEditUseDay->GetPropValueT("value",&dateSell.day,sizeof(dateSell.day));
		   	  
		    //檢測出廠日期
			cState = CvtDate(&dateOut);
    		switch (cState)
        	{
        		case DATE_ERROR_MONTH:
        			((CtmFormView *)pwndSender)->Goto(m_pwndEditOutFctyMonth->GetTabOrder());
            		return wIDControl;
        		case DATE_ERROR_DAY:
        			((CtmFormView *)pwndSender)->Goto(m_pwndEditOutFctyDay->GetTabOrder());
            		return wIDControl;
        		default:
            		break;
        	}
        	
			//檢測交付日期
			cState = CvtDate(&dateSell);
    		switch (cState)
        	{
        		case DATE_ERROR_MONTH:
        			((CtmFormView *)pwndSender)->Goto(m_pwndEditUseMonth->GetTabOrder());
            		return wIDControl;
        		case DATE_ERROR_DAY:
        			((CtmFormView *)pwndSender)->Goto(m_pwndEditUseDay->GetTabOrder());
            		return wIDControl;
        		default:
            		break;
        	}
        	 
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR", dateOut.year);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH", dateOut.month);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY", dateOut.day);
        	//printf("save OPER_HMI_NULL_NULL_STATE = 0XA5 \n");
        	SetDBValue("OPER_HMI_NULL_NULL_STATE", 0xA5);
					SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR", dateSell.year);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH", dateSell.month);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY", dateSell.day);
        	
        	
        	if(m_pwndEditPayStages != NULL)
        	{
				//printf("2lValue:%ld ;2PayStages:%ld \n",GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue,PayStages);
	        	//cout << "m_PayStages:"<< m_PayStages << "DBValue:"<<GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue<<endl;
	        	if (GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue != m_PayStages)
	            {
	            	SetDBValue("OPER_HMI_NULL_NULL_STAGES", m_PayStages);
	            	SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", 1);
	            	//printf("PayStages2:%lld \n",m_PayStages);
	            	//cout << "PayStages2:"<<m_PayStages<<endl;				
	            }
	            //else printf("PayStages3:%lld \n",PayStages);
	        }
	        if(m_pwndEditSinTimeOutDay != NULL)
	        {
	        		m_pwndEditSinTimeOutDay->GetPropValueT("value",&lTemp,sizeof(lTemp));
            	SetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD", lTemp);
            }
            					
            if(m_pwndEditAlarmDays != NULL)
            {
            	m_pwndEditAlarmDays->GetPropValueT("value",&lTemp,sizeof(lTemp));
            	SetDBValue("OPER_HMI_NULL_NULL_DAYSALARMPERIOD", lTemp);
            }
            if(m_pwndEditAlmDevisMinute != NULL)
            {
            	m_pwndEditAlmDevisMinute->GetPropValueT("value",&lTemp,sizeof(lTemp));	
            	SetDBValue("OPER_HMI_NULL_NULL_ALARMINTERVAL", lTemp);
            }
           
            SetDBValue("OPER_HMI_NULL_NULL_TIMESTOPSECOND", 0);	
            ntDays = GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue * 
        			 GetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD").lValue;
        			 
        	dateOut.year  	= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue;
			dateOut.month 	= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH").lValue;
			dateOut.day   	= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY").lValue;
        	GetAccDate((tmDATE *)&dateSell, ntDays, (tmDATE *)&dateOut);
        	ntDays = -(int)GetDBValue("OPER_HMI_NULL_NULL_DAYSALARMPERIOD").lValue;
        	dtTempDATE.year  = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue;
        	dtTempDATE.month = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH").lValue;
        	dtTempDATE.day   = GetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY").lValue;
        	GetAccDate((tmDATE *)&dateOut, ntDays, (tmDATE *)&dtTempDATE);
        	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR" , dateOut.year);
        	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH", dateOut.month);
        	SetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY"  , dateOut.day);	
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLYEAR" , dateSell.year);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLMONTH", dateSell.month);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEMACHSELLDAY"  , dateSell.day);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR" , dtTempDATE.year);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH", dtTempDATE.month);
        	SetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY"  , dtTempDATE.day);
		}
		else
		{
			if(m_pwndEditStopMinute != NULL)
			{
				m_pwndEditOperationState->GetPropValueT("value",&lTemp,sizeof(lTemp));
				SetDBValue("OPER_HMI_NULL_NULL_STATE", lTemp);
			}
			SetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG", FALSE);
			// enter overview operation				
		}
		SaveOperation();
	}
	if(pwnd == m_pwndEditPayStages)
  {
	  m_pwndEditPayStages->GetPropValueT("value",&m_PayStages,sizeof(m_PayStages));
	   // cout << "m_PayStages:"<< m_PayStages<<endl;	
	}
	//else if(pwnd == m_pwndButCancel)
    //{
    //	if(m_iKey == _ENTER)	Exit();
    //}
	return wIDControl;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	m_iKey = wKey;
	//if(m_iKey ==_KEY_CLEAR) Exit();
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
}
