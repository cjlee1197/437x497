/*===========================================================================+
|  Class    : ViewSyst library                                               |
|  Task     : ViewSyst library action source file                            |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   :			                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/


#include "ViewSystemSet.h"
#include "../utils.h"
#include "../commonaction.h"
#include "../language.h"
#include "../selectedit.h"
#include "../main.h"
#include "../tmcontrol.h"


/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

#define     C21_START       2000

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

CtmWnd*		m_pwndSwitchScreenSaver   	= NULL;		//螢保開關		//ScreenSaverSwitch
CtmWnd*		u_pwndEditScreenSaverIdle		= NULL;		//閒置time		//EditScreenSaverIdle
CtmWnd*		m_pwndEditScreenSaverAuto 	= NULL;		//執行啟動		//EditScreenSaverAuto
CtmWnd*		m_pwndBtnScreenSaverAlarm 	= NULL;		//警告時解除  //BtnScreenSaverAlarm
CtmWnd*		m_pwndBtnScreenSaverPreset	= NULL;		//還原預設		//BtnScreenSaverPreset
CtmWnd*		m_pwndSelectLanguage   			= NULL;		//語言選擇		//LanguageSelectEdit
CtmWnd*		m_pwndDateEdit[6]   				= {NULL};	//日期				//DateEdit                               	
CtmWnd*		m_pwndBtnCancel 						= NULL;		//取消				//BtnSystemSetCancel
CtmWnd*		m_pwndBtnOK 								= NULL;		//儲存				//BtnSystemSetOK

int 			u_pwndDateEditCnt						= 0;

BOOL		u_bEnterKey						= 	FALSE;


int 		languageindex 				= 	-1;
int 		Unitindex 						= 	-1;
char		languageset[64]				=		"\0";


/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	m_pwndSwitchScreenSaver   = pwndSender->FindControlFromName("ScreenSaverSwitch");   
	u_pwndEditScreenSaverIdle	= pwndSender->FindControlFromName("EditScreenSaverIdle"); 
	m_pwndEditScreenSaverAuto = pwndSender->FindControlFromName("EditScreenSaverAuto"); 
	m_pwndBtnScreenSaverAlarm = pwndSender->FindControlFromName("BtnScreenSaverAlarm"); 
	m_pwndBtnScreenSaverPreset= pwndSender->FindControlFromName("BtnScreenSaverPreset");
	m_pwndSelectLanguage   		= pwndSender->FindControlFromName("LanguageSelectEdit");  
	u_pwndDateEditCnt   			= GetSpecialControlNum(pwndSender,"DateEdit","CtmEditX1", m_pwndDateEdit);         
  m_pwndBtnCancel           = pwndSender->FindControlFromName("BtnSystemSetCancel");
	m_pwndBtnOK 					    = pwndSender->FindControlFromName("BtnSystemSetOK");      		

	GetTime();
	//取得目前語言(0=中文,1=English)
	languageindex = CtmConfig::GetInstance()->GetCurrentIndex();
	((CtmSelectEdit*)m_pwndSelectLanguage)->SetIndex(languageindex);
	m_pwndSelectLanguage->Update();
	RobotStatus = OP_EDIT_MODE;
	return TRUE;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	/*if(pwndTemp == m_pwndSelectLanguage )	//語言設定
	{
		if(((CtmSelectEdit*)m_pwndSelectLanguage)->GetIndex() != languageindex)
		{
			languageindex	= ((CtmSelectEdit*)m_pwndSelectLanguage)->GetIndex();
			CtmConfig::GetInstance()->SetCurrentIndex(languageindex);
    	g_pMainFrame->ChangeLanguage();
		}
	}*/

	return wIDControl;
}

WORD		OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*			pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == m_pwndBtnOK)
	{
		tmDATE          date;
		tmTIME          time;
		long long       PartYear;
    long long 			lTemp = 0;
		m_pwndDateEdit[0]->GetPropValueT("value", &PartYear,sizeof(PartYear));
		m_pwndDateEdit[1]->GetPropValueT("value", &date.month,sizeof(date.month));
		date.year = PartYear+C21_START;
		m_pwndDateEdit[2]->GetPropValueT("value", &date.day,sizeof(date.day));
		
		m_pwndDateEdit[3]->GetPropValueT("value", &time.hour,sizeof(time.hour));
		m_pwndDateEdit[4]->GetPropValueT("value", &time.minute,sizeof(time.minute));
		m_pwndDateEdit[5]->GetPropValueT("value", &time.second,sizeof(time.second));
		SetDate(date);
		SetTime(time);
		system("./ReadCpuWriteToRtc");
	}
	else if(pwndTemp == m_pwndBtnCancel)
	{
		GetTime();
	}
	return wIDControl;
} 

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/


void		GetTime()
{
	tmDATE        date;
	tmTIME        time;

	GetDate(&date);
	
	GetTime(&time);
	long long temp = 0;
	
	if 	((date.year > 2099 	|| date.year < 2000) 	||
		(date.month < 1 	|| date.month > 12)		||
		(date.day <= 0 		|| date.day > 31) 		||
		(time.hour > 23) 							||
		(time.minute > 59)							||
		(time.second > 59)	)
	{
		date.year 	= 2007;
		date.month 	= 1;
		date.day 	= 1;
		time.hour	= 0;
		time.minute	= 0;
		time.second = 0;
	}
	else
	{
		if (date.month == 2)
		{
			if (date.year % 4 != 0) 
			{
				if (date.day > 28) 
				{
					date.year 	= 2007;
					date.month 	= 1;
					date.day 	= 1;
					time.hour	= 0;
					time.minute	= 0;
					time.second = 0;
				}
				
			}
			else
			{
				if (date.day > 29) 
				{
					date.year 	= 2007;
					date.month 	= 1;
					date.day 	= 1;
					time.hour	= 0;
					time.minute	= 0;
					time.second = 0;
				}
			}
							
		}
		else if  (date.month == 1 || date.month == 3 || date.month == 5 ||
				date.month == 7 || date.month == 8 || date.month == 10 ||
				date.month == 12)
		{
			if (date.day > 31) 
			{
				date.year 	= 2007;
				date.month 	= 1;
				date.day 	= 1;
				time.hour	= 0;
				time.minute	= 0;
				time.second = 0;
			}
		}
		else
		{
			if (date.day > 30)
			{
				date.year 	= 2007;
				date.month 	= 1;
				date.day 	= 1;
				time.hour	= 0;
				time.minute	= 0;
				time.second = 0;
			}
		}
		
	}
	
	if (m_pwndDateEdit[0]  != NULL) 
	{
		temp = date.year % 100;
		m_pwndDateEdit[0]->SetPropValueT("value", temp);
		m_pwndDateEdit[0]->Update();
	}
	if(m_pwndDateEdit[1]  != NULL) 
	{
	
		temp = date.month;
		m_pwndDateEdit[1]->SetPropValueT("value", temp);
		m_pwndDateEdit[1]->Update();
	}
	//	 the other way
	if(m_pwndDateEdit[2]  != NULL) 
	{

		temp = date.day;
		m_pwndDateEdit[2]->SetPropValueT("value", temp);
		m_pwndDateEdit[2]->Update();
	}
	if(m_pwndDateEdit[3]   != NULL) 
	{
		temp = time.hour;
		m_pwndDateEdit[3]->SetPropValueT("value", temp);
		m_pwndDateEdit[3]->Update();
	}
	if(m_pwndDateEdit[4] != NULL) 
	{
		temp = time.minute;
		m_pwndDateEdit[4]->SetPropValueT("value", temp);
		m_pwndDateEdit[4]->Update();
		
	}
	if(m_pwndDateEdit[5] != NULL) 
	{
		temp = time.second;
		m_pwndDateEdit[5]->SetPropValueT("value", temp);
		m_pwndDateEdit[5]->Update();
	}
}

