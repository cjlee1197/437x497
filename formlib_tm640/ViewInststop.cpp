/*===========================================================================+
|  Class    : View initial Stop machine                                      |
|  Task     : View initial Stop machine                                      |
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
#include 	"ViewInststop.h"
/*===========================================================================+
|           External                                                         |
+===========================================================================*/
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
 CtmWnd* m_pwndEditMachineID	= NULL;
 CtmWnd* m_pwndEditOutFctyMonth = NULL;
 CtmWnd* m_pwndEditOutFctyDay   = NULL;
 CtmWnd* m_pwndEditOutFctyYear  = NULL;
 CtmWnd* m_pwndOptionConfirm	= NULL;
 CtmWnd* m_pwndOptionCancel		= NULL;
 
 
int			m_iKey = 0;
/*---------------------------------------------------------------------------+
|           code segment                                                     |
+---------------------------------------------------------------------------*/
//  --  editMachineID		
//  --  editOutFctyMonth    
//  --  editOutFctyDay      
//  --  editOutFctyYear  
//  --  optionConfirm
//	--  optionCancel           
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
void	SetDisabledControl();
/*===========================================================================+
|           Class implementation - ViewInstall                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+----------------------- ----------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	m_pwndEditMachineID	    = pwndSender->FindControlFromName("editMachineID");
    m_pwndEditOutFctyMonth  = pwndSender->FindControlFromName("editOutFctyMonth");
    m_pwndEditOutFctyDay    = pwndSender->FindControlFromName("editOutFctyDay");
    m_pwndEditOutFctyYear   = pwndSender->FindControlFromName("editOutFctyYear");
    m_pwndOptionConfirm		= pwndSender->FindControlFromName("optionConfirm");
    m_pwndOptionCancel		= pwndSender->FindControlFromName("optionCancel");
    
    CreateSet(pwndSender);
    
	return TRUE;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void       CreateSet(CtmWnd* pwndSender)
{
//	long	lTemp;
	char    szMachineID[10];
	tmDATE	currDate;
	
	memset(szMachineID, 0, 10);
	if(GetDBValue("OPER_HMI_NULL_NULL_STATE").lValue == 0xA5)
	{
		//all control are disabled
		GetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
		if(m_pwndEditMachineID 	  != NULL)
		{ 
			m_pwndEditMachineID->SetPropValueT("text", szMachineID);
			m_pwndEditMachineID->Update();
		}
		currDate.month  = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH").lValue;
		currDate.day    = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY").lValue;
		currDate.year   = GetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR").lValue;
		if(m_pwndEditMachineID!=NULL)	m_pwndEditMachineID		->SetPropValueT("enabled",(double)FALSE);
		if(m_pwndEditOutFctyMonth!=NULL)m_pwndEditOutFctyMonth  ->SetPropValueT("enabled",(double)FALSE);
		if(m_pwndEditOutFctyDay!=NULL)	m_pwndEditOutFctyDay    ->SetPropValueT("enabled",(double)FALSE);
		if(m_pwndEditOutFctyYear!=NULL)	m_pwndEditOutFctyYear   ->SetPropValueT("enabled",(double)FALSE);
		if(m_pwndOptionConfirm!=NULL)	m_pwndOptionConfirm		->SetPropValueT("enabled",(double)FALSE);
		if(m_pwndOptionCancel!=NULL)	m_pwndOptionCancel		->SetPropValueT("enabled",(double)FALSE);
		
	}
	//else	GetDate(&currDate);
	//if(m_pwndEditMachineID 	  != NULL)
	//{ 
	//	m_pwndEditMachineID->SetPropValueT("text", szMachineID);
	//	m_pwndEditMachineID->Update();
	//}
	//if(m_pwndEditOutFctyMonth != NULL) 
	//{
	//	m_pwndEditOutFctyMonth->SetPropValueT("value", (double)currDate.month);
	//	m_pwndEditOutFctyMonth->Update();
	//}
	//if(m_pwndEditOutFctyDay   != NULL) 
	//{
	//	m_pwndEditOutFctyDay->SetPropValueT("value", (double)currDate.day);
	//	m_pwndEditOutFctyDay->Update();
	//}
	//if(m_pwndEditOutFctyYear  != NULL)
	//{
	//	lTemp = (currDate.year%100);
	//	m_pwndEditOutFctyYear->SetPropValueT("value", (double)lTemp);
	//	m_pwndEditOutFctyYear->Update();
	//}
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        OnUpdateA(CtmWnd* pwndSender)
{
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	tmDATE      dateOut;
	BYTE        cState;
	char		szMachineID[10];
	CtmWnd* 	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == m_pwndOptionConfirm)
	{
		//if( m_iKey == _ENTER )
		//{
			if(m_pwndEditOutFctyYear != NULL)
				m_pwndEditOutFctyYear->GetPropValueT("value",&dateOut.year,sizeof(dateOut.year));
			//printf("dateOut.year = %d\n",dateOut.year);
			if(m_pwndEditOutFctyMonth != NULL)
				m_pwndEditOutFctyMonth->GetPropValueT("value",&dateOut.month,sizeof(dateOut.month));
			//printf("dateOut.month = %d\n",dateOut.month);
			if(m_pwndEditOutFctyDay != NULL)
				m_pwndEditOutFctyDay->GetPropValueT("value",&dateOut.day,sizeof(dateOut.day));
			//printf("dateOut.day = %d\n",dateOut.day);
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
	    	if(m_pwndEditMachineID != NULL)
	    		m_pwndEditMachineID->GetPropValueT("text",szMachineID,10);
	    	SetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
	    	SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", 1);
	    	//printf("write %d-%d-%d\n",dateOut.month,dateOut.day,dateOut.year);
	    	//SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR" , dateOut.year);
	        //SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH", dateOut.month);
	        //SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY"  , dateOut.day);
	        SaveOperation();
	        MsgBox("          OK!!!");
	   //  }
	}
	return wIDControl;
}
WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	tmDATE      dateOut;
	BYTE        cState;
	char		szMachineID[10];
	CtmWnd* 	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == m_pwndOptionConfirm)
	{
		if( m_iKey == _ENTER )
		{
			if(m_pwndEditOutFctyYear != NULL)
				m_pwndEditOutFctyYear->GetPropValueT("value",&dateOut.year,sizeof(dateOut.year));
			//printf("dateOut.year = %d\n",dateOut.year);
			if(m_pwndEditOutFctyMonth != NULL)
				m_pwndEditOutFctyMonth->GetPropValueT("value",&dateOut.month,sizeof(dateOut.month));
			//printf("dateOut.month = %d\n",dateOut.month);
			if(m_pwndEditOutFctyDay != NULL)
				m_pwndEditOutFctyDay->GetPropValueT("value",&dateOut.day,sizeof(dateOut.day));
			//printf("dateOut.day = %d\n",dateOut.day);
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
	    	if(m_pwndEditMachineID != NULL)
	    		m_pwndEditMachineID->GetPropValueT("text",szMachineID,10);
	    	SetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
	    	SetDBValue("OPER_HMI_NULL_NULL_STAGENOW", 1);
	    	//printf("write %d-%d-%d\n",dateOut.month,dateOut.day,dateOut.year);
	    	//SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTYEAR" , dateOut.year);
	        //SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTMONTH", dateOut.month);
	        //SetDBValue("OPER_HMI_NULL_NULL_DATEMACHOUTDAY"  , dateOut.day);
	        SaveOperation();
	        MsgBox("          OK!!!");
	     }
	}
	return wIDControl;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	m_iKey = wKey;
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	return wKey;
}
