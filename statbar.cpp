/*===========================================================================+
|  Class    : StatBar                                                        |
|  Task     : Status Bar                                                     |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 02/20/1995                                                     |
|  Revision : 04/10/1996                                                     |
+===========================================================================*/
#include 	<dlfcn.h>
#include    "stable.h"
#include    "utils.h"
#include    "statbar.h"
#include	"init.h"
#include	"tmconfig.h"
#include	"static.h"
#include	"main.h"
//#include	"memwatch.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
const	int		EVENT_ONSTATUSBAR_UPDATENAME_DEFAULT_LEN	= 10;
const	int		EVENT_ONSTATUSBAR_CREATENAME_DEFAULT_LEN	= 10;
#define		tmLOG_WIDTH				60
#define     MODE_MANUAL				0x0000		//手動
#define     MODE_SEMI_AUTO			0x0001		//半自動
#define     MODE_TIME_AUTO			0x0002		//時間自動
#define     MODE_JOG				0x0003		//存動
#define     MODE_ADJUST_AUTO		0x0004		//自動調模
#define		MODE_HOMING				0x0005		//歸零
#define		MODE_AUTO_PURGE			0x0006
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
WORD			g_wError		= FALSE;					// control.cpp
WORD			g_wI32		 	= FALSE;					// iNet狀態
WORD			g_wTaskCommStat	= FALSE;
WORD			g_wOPSatus		= 0;
WORD			g_wStep			= 255;
WORD			g_wMotor		= FALSE;
WORD			g_wHeater		= FALSE;

/*===========================================================================+
|           Class implementation - StatusBar                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmStatusBar, CtmWnd)
IMPLEMENT_DYNCREATE(CtmPromptBar, CtmWnd)
CtmStatusBar::CtmStatusBar() : CtmWnd()
{
	strcpy(TYPEName, "CtmPromptBar");
	
	
	OnUpdateA 		= NULL;
	OnCreateA 		= NULL;
	OnDestroyA		= NULL;
	OnMessage		= NULL;	//	fans 2008/6/16 11:31上午
	m_pszUpdate 	= new char[EVENT_ONSTATUSBAR_UPDATENAME_DEFAULT_LEN];
	m_pszCreate 	= new char[EVENT_ONSTATUSBAR_CREATENAME_DEFAULT_LEN];
	m_pszOnMessage	= new char[EVENT_ONSTATUSBAR_UPDATENAME_DEFAULT_LEN];	//	fans 2008/6/16 11:31上午
	m_pszDestroy 	= new char[11];
	strcpy(m_pszUpdate, 	"OnUpdateA");
	strcpy(m_pszCreate, 	"OnCreateA");
	strcpy(m_pszOnMessage,	"OnMessage");	//	fans 2008/6/16 11:31上午
	strcpy(m_pszDestroy,	"OnDestroyA");

}

CtmStatusBar::~CtmStatusBar()
{
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnDestroyA = (void(*)(CtmWnd*))dlsym(GetLibHandle(), m_pszDestroy);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnDestroyA = NULL;
			////cout << "Load " << m_pszDestroy << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnDestroyA = NULL;
	if (OnDestroyA != NULL) OnDestroyA(this);
	
	OnUpdateA 		= NULL;
	OnCreateA 		= NULL;
	OnDestroyA 		= NULL;
	OnMessage		= NULL;	//	fans 2008/6/16 11:31上午
	//delete []m_pszUpdate;
	//delete []m_pszCreate;
	//delete []m_pszDestroy;
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
BOOL        CtmStatusBar::Create(tmWND* pwnd)
{
	
	/*	fans mark
	int		i;
	WORD	wFieldWidth;

	if (!CtmWnd::Create(pwnd))
	    return FALSE;
	m_pbmpBackground = g_aIniBmp[STATE_BARID].pIniBmp;
	Clear();
	
	wFieldWidth			= (GetClientWidth() - tmLOG_WIDTH) / tmMAX_STATFIELD;
	
	for(i = 0; i < tmMAX_STATFIELD; i ++)
	{
		m_Field[i].wStyle		= tmWS_BORDER_NONE | tmWS_TRANSPARENT;
		m_Field[i].hwndParent	= m_hwndMe;
		m_Field[i].rect.top		= GetmClientTop() 	+ 2;
		m_Field[i].rect.bottom	= GetClientBottom() - 2;
		m_Field[i].rect.left	= tmLOG_WIDTH + wFieldWidth * i + 1;
		m_Field[i].rect.right	= m_Field[i].rect.left + wFieldWidth;
		m_Field[i].pcFont		= &g_pFont16;
		
		m_pwndField[i]			= new	CtmCartoon(&m_Field[i]);
		m_pwndField[i]->Set(&g_Cartoon[i]);
	}*/
	
	return TRUE;
}

//	fans add
BOOL 	CtmStatusBar::CreateA()
{
		int		i;
	WORD	wFieldWidth;

	CtmWnd::CreateA();
	m_pbmpBackground = g_aIniBmp[STATE_BARID].pIniBmp;
	Clear();
	
	//	load Dll
	///*	fansTest
	char	libName[512]; 
	void*	libHandle = NULL;	
	char*	pszError = NULL;  
	
	// <FANS> 2006/11/20 09:50上午
	//strcpy(libName, "./formlib/");
	//strcat(libName, Name);
	//strcat(libName, ".so");
	sprintf(libName, "%s%s%s", CtmConfig::GetInstance()->GetFormlibPath(), Name, ".so");
	libHandle = dlopen(libName, RTLD_LAZY);	
	if (libHandle != NULL)
	{
		SetLibHandle(libHandle);
	}
	
	if (GetLibHandle() != NULL)
	{
		#ifdef	D_PXA270
		OnCreateA = (BOOL(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszCreate);
		#else
		OnCreateA = (BOOL(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszCreate);
		#endif
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnCreateA = NULL;
			//cout << "Load " << m_pszCreate << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnCreateA = NULL;
		
	if (OnCreateA != NULL) OnCreateA(this);
	
	
	
	//*/
	return TRUE;
}

void        CtmStatusBar::Redraw(RECT rect)
{
	short	i;
	
	Clear();
	
	for (i = 0; i < GetControlCount(); i++)
		Controls[i]->Update();
	
	/*	fans mark
	for (i = 0; i < tmMAX_STATFIELD; i++)
		m_pwndField[i]->Update();
		*/

}

void		CtmStatusBar::Show()
{
	CtmWnd::Show();
}

void        CtmStatusBar::Show(short nFieldNo, short Index)
{

}

void	CtmStatusBar::Clear()
{
	RECT rect;
	MKRECT(rect, 0, 0, GetClientWidth(), GetClientHeight())
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
	m_pbmpBackground->Show(rect, NULL);	
	
	MKRECT(rect, 0, 0, tmLOGO_WIDTH, tmLOGO_HEIGHT)
	_SetViewPort(2, 2, tmLOGO_WIDTH+2, tmLOGO_HEIGHT+2);
	//m_bmpLogo.Create(tmBMP_LOGO);
	//m_bmpLogo.Show(rect, NULL);
}

void	CtmStatusBar::Update()
{
	/*	fans add
	short	i;
	
	
	g_wOPSatus = GetDBValue("TECH_OP_MODE", 0).lValue;
	g_wStep	   = GetDBValue("TECH_OP_STEP", 0).lValue;
	g_wMotor   = GetDBValue("TECH_MOTOR_STATUS", 0).lValue;
	g_wHeater  = GetDBValue("TECH_HEATER_STATUS", 0).lValue;
	g_wError   = GetDBValue("TECH_ALARMSTATUS", 0).lValue;		//<<yang 2006/3/18 
	
	
	for (i = 0; i < GetControlCount(); i++)
		Controls[i]->Update();
	*/
	
	/*
	for (int i = 0; i < GetControlCount(); i++)
		Controls[i]->Update();
	*/
	///*	fansTest
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		#ifdef	D_PXA270
		OnUpdateA = (void(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszUpdate);
		#else
		OnUpdateA = (void(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszUpdate);
		#endif
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnUpdateA = NULL;
			//cout << "Load " << m_pszUpdate << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnUpdateA = NULL;
	if (OnUpdateA != NULL) OnUpdateA(this);
	//*/
}

void		CtmStatusBar::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnMessage = (void(*)(CtmWnd*, int, WPARAM, LPARAM))dlsym(GetLibHandle(), m_pszOnMessage);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnMessage = NULL;
			////cout << "Load " << m_pszOnChangeName << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnMessage = NULL;
	
	if (OnMessage != NULL)
	{
		return OnMessage(this, message, wParam, lParam);
	}
	else
		return;	
}

void		CtmStatusBar::SetPropertys()
{
	CtmWnd::SetPropertys();	
	SetPropInfo("OnUpdateA",	tmPROCEDURE,	&m_pszUpdate);
	SetPropInfo("OnCreateA", 	tmPROCEDURE,	&m_pszCreate);
	SetPropInfo("OnDestroyA",	tmPROCEDURE,	&m_pszDestroy);
	SetPropInfo("OnMessage",	tmPROCEDURE,	&m_pszOnMessage);	//	fans 2008/6/16 11:32上午
	
}

void		CtmStatusBar::SetPropertys_Nub()
{
	CtmWnd::SetPropertys();	
	SetPropInfo(STATUSBAR_UPDATEA,	tmPROCEDURE,	&m_pszUpdate);
	SetPropInfo(STATUSBAR_CREATEA, 	tmPROCEDURE,	&m_pszCreate);
	SetPropInfo(STATUSBAR_DESTROYA,	tmPROCEDURE,	&m_pszDestroy);
	SetPropInfo(STATUSBAR_MESSAGEA,	tmPROCEDURE,	&m_pszOnMessage);	//	fans 2008/6/16 11:32上午
	
}

/*===========================================================================+
|           Class implementation - PromptBar                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmPromptBar::CtmPromptBar() : CtmWnd()
{
	strcpy(TYPEName, "CtmPromptBar");
	m_nErrorShowNum = 0;
	m_pDoubleBuffer	= NULL;
}

CtmPromptBar::~CtmPromptBar()
{
	delete m_pDoubleBuffer;
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
BOOL        CtmPromptBar::Create(tmWND* pwnd)
{
	return TRUE;
}

//	fans add
BOOL	CtmPromptBar::CreateA()
{
	SetBorderWidth(0);
	CtmWnd::CreateA();
    
	m_pbmpBackground = g_aIniBmp[PROMPT_BARID].pIniBmp;
	Clear();
	*m_sz0 = '\0';
	
	int j = 0;
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmWnd"))
		{
			Controls[i]->SetFGColor(WHITE);
			Controls[i]->SetBGColor(m_wndattr.bgcStatBar);
		}
	}
	
	
	return TRUE;
}

void        CtmPromptBar::Redraw(RECT rect)
{
	ScreenToClient(&rect);
	Clear();
	//	fans mark
	//m_wndField[0].DrawBorder();
	
	//	fans add
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmWnd"))		
			Controls[i]->DrawBorder();
		
	}
}

void        CtmPromptBar::Show(short nFieldNo, char* pszMsg)
{
	RECT rect;
	RECT rectBuffer;
	WORD    *pwSrc, *pwDst;
	
	int			nRightSpa	= 140;
	CtmWnd*	pwnd 			= NULL;	
	CtmWnd*	pwndTime 	= NULL;
	pwndTime = FindControlFromName("DateTime");
	
	if (pwndTime != NULL)
	{
		int	nTimeRight 	= 0;
		int	nTimeLeft		= 0;
		pwndTime->GetPropValueT("right", &nTimeRight);
		pwndTime->GetPropValueT("left", &nTimeLeft);
				
		nRightSpa				= nTimeRight - nTimeLeft;
	}
	
	
	
	switch(nFieldNo)
	{
		case -1:
			//CtmWnd*	pwnd = NULL;
			pwnd = FindControlFromName("StaticPrompt");
			
			rectBuffer.left 	= m_rectClient.left;
			rectBuffer.top		= m_rectClient.top;					
			rectBuffer.right	= m_rectClient.right -  nRightSpa - m_wBorderWidth;
			rectBuffer.bottom	= m_rectClient.bottom;
			
			if (m_pDoubleBuffer == NULL)
			{			
				m_pDoubleBuffer = new CtmGDIDoubleBuffer();
				m_pDoubleBuffer->SetRegion(rectBuffer);
				m_pDoubleBuffer->InitBuffer(m_bgc);
				m_pbmpBackground->SetDoubleBuffer(m_pDoubleBuffer);
			}
			else				 
				m_pDoubleBuffer->SetRegion(rectBuffer);
				
			MKRECT(rect, 0, 0, GetClientWidth()-nRightSpa, GetClientHeight()- m_wBorderWidth - 1)
        	_SetViewPort(m_rectClient.left, m_rectClient.top, 
            				m_rectClient.right -  nRightSpa - m_wBorderWidth,m_rectClient.bottom);
        	m_pbmpBackground->Show(rect, NULL);
        	
			if (pwnd == NULL)
			{
				//MKRECT(rect, 0, 0, GetClientWidth()-140, GetClientHeight())
				//_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
				//m_pbmpBackground->Show(rect, NULL);	
				SetFGColor(WHITE);
				CtmWnd::ShowExtent(0, 0, m_sz0);
			}
			else
			{
				//MKRECT(rect, 0, 0, GetClientWidth()-140, GetClientHeight())
				//_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
				//m_pbmpBackground->Show(rect, NULL);
				printf("m_sz0=%s\n",m_sz0);
				pwnd->SetPropValueT("text", m_sz0);
				pwnd->Update();
			}
			break;
	    case 1:
	      break;
	//  case 2:
	//      m_wndField2.Clear();
	//      m_wndField2.Show(0, 0, pszMsg);
	//      break;
	//  case 3:
	//      m_wndField3.Clear();
	//      m_wndField3.Show(0, 0, pszMsg);
	//      break;
			default:
				if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			    {
			    	int   nLen=0;
					if(CtmConfig::GetInstance()->GetLanguageType()==1)
					{
						pwSrc   = (WORD *)pszMsg;
		    			pwDst   = (WORD *)m_sz0;
		    			if(pwSrc==NULL||pwDst==NULL)	return;
		    			//int nCount=0;
		    			while(*pwDst != 0 && *pwSrc != 0)
		    			{
		    				//nCount++;
		    			    if(*pwDst != *pwSrc)
		    			    {
		    			    	//printf("not same ,break .\n");   
		    			    	break;
		    			    }
		    			    pwDst ++;
		    			    pwSrc ++;
		    			  
		    			}
		    			//printf("*pwDst=%d,pwSrc=%d\n",*pwDst,*pwSrc);
		    			//printf("nCount=%d\n",nCount);
		    			if(*pwDst == 0 && *pwSrc == 0)  return;
		    			char* temp=m_sz0;
		    			pwSrc   = (WORD *)pszMsg;
					 	while(*pwSrc != 0)
					 	{
					 		*temp=pszMsg[0];
					 		//printf("%02x ",*temp&0xFF);
					 		if(pszMsg[1]!=0)
					 		{
					 	 		*++temp=pszMsg[1];
					 	 		//printf("%02x ",*temp&0xFF);		
					 		}
					 		else
					 		{
					 			*++temp=0;	
					 		}
					 		++temp;
					 		++pwSrc;
					 		++++pszMsg;
					 		nLen++;
					 		if(nLen>=tmMAX_TEXT_COL){		
					 			break;
					 		}	
					 	}
					 	pwDst=(WORD*)temp;
					 	*pwDst=0;
					 	//printf("\n");
					}
					else
					{
		    			pwSrc   = (WORD *)pszMsg;
		    			pwDst   = (WORD *)m_sz0;
		    			if(pwSrc==NULL||pwDst==NULL)	return;
		    			while(*pwDst != 0 && *pwSrc != 0)
		    			{
		    			    if(*pwDst != *pwSrc)    break;
		    			    pwDst ++;
		    			    pwSrc ++;
		    			}
		    			if(*pwDst == 0 && *pwSrc == 0)  return;
		    			pwSrc   = (WORD *)pszMsg;
		    			pwDst   = (WORD *)m_sz0;
		    			do
		    			{
		    			    *pwDst  = *pwSrc;
		    			    pwDst ++;
		    			    pwSrc ++;
		    			   	nLen  ++;
		    			    if(nLen>=tmMAX_TEXT_COL){		
					 			break;
					 		}	 
		    			}
		    			while(*pwSrc != 0);
		    			*pwDst  = 0;
		    		}
	    		}
    			else
    			{
    			    if(strncmp(m_sz0, pszMsg, tmMAX_TEXT_COL) == 0)		return;
    				StrNCpy(m_sz0, pszMsg, tmMAX_TEXT_COL);
    			}
			    //Clear();
				//	fans	2006/12/17 02:05下午	add&edit
				
				pwnd = FindControlFromName("StaticPrompt");
	
				rectBuffer.left 	= m_rectClient.left;
				rectBuffer.top		= m_rectClient.top;					
				rectBuffer.right	= m_rectClient.right -  nRightSpa - m_wBorderWidth;
				rectBuffer.bottom	= m_rectClient.bottom;
					
				if (m_pDoubleBuffer == NULL)
				{			
					m_pDoubleBuffer = new CtmGDIDoubleBuffer();
					m_pDoubleBuffer->SetRegion(rectBuffer);
					m_pDoubleBuffer->InitBuffer(m_bgc);
					m_pbmpBackground->SetDoubleBuffer(m_pDoubleBuffer);
				}
				else					 
					m_pDoubleBuffer->SetRegion(rectBuffer);
								
				MKRECT(rect, 0, 0, GetClientWidth() - nRightSpa - m_wBorderWidth, GetClientHeight() - m_wBorderWidth - 1);
            	_SetViewPort(m_rectClient.left, m_rectClient.top, 
            				m_rectClient.right -  nRightSpa - m_wBorderWidth,m_rectClient.bottom);
            	m_pbmpBackground->Show(rect, NULL);
            	
				if (pwnd == NULL)
				{
					//MKRECT(rect, 0, 0, GetClientWidth()-140, GetClientHeight())
					//_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
					//m_pbmpBackground->Show(rect, NULL);	
					SetFGColor(WHITE);
					CtmWnd::ShowExtent(0, 0, m_sz0);
				}
				else
				{
					//MKRECT(rect, 0, 0, GetClientWidth()-140, GetClientHeight())
					//_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
					//m_pbmpBackground->Show(rect, NULL);
					pwnd->SetPropValueT("text", m_sz0);
					pwnd->Update();
				}
			break;
    }
    
}

void	CtmPromptBar::ShowAlarm(BOOL bShow)
{	
	tmDATE    date;
	tmTIME    time;
	char      sz[256];                     // Maximum character : YYYY/MM/DD
	CtmWnd*	pwnd = NULL;
	pwnd = FindControlFromName("StaticPrompt");
	CtmWnd*	pwndTime = NULL;
	pwndTime = FindControlFromName("DateTime");
	
	if (pwnd != NULL)
	{
		((CtmStaticX2*)pwnd)->SetCopyBackImage(FALSE);
	}
	if (pwndTime != NULL)
	{
		((CtmStaticX2*)pwndTime)->SetCopyBackImage(FALSE);
	}
	
	//if (bShow) m_nErrorShowNum++;		//Mario debug
	//else m_nErrorShowNum--;		//Mario debug
		
	//if (m_nErrorShowNum < 0) m_nErrorShowNum = 0;		//Mario debug
	
		
	//if (m_nErrorShowNum > 0) m_pbmpBackground = g_aIniBmp[PROMPT_BARIDR].pIniBmp;		//Mario debug
	if (bShow) m_pbmpBackground = g_aIniBmp[PROMPT_BARIDR].pIniBmp;		//Mario debug
	else m_pbmpBackground = g_aIniBmp[PROMPT_BARID].pIniBmp;
		
	Clear();	
	//char	szTemp[tmMAX_TEXT_COL];
	//memcpy(szTemp, m_sz0, tmMAX_TEXT_COL);
	//strncpy(szTemp, m_sz0, tmMAX_TEXT_COL * 2);
	Show(-1, NULL);
	
	
	//James add 2008/4/25 09:37上午 解決promp刷掉時間的問題
	//GetTime(&time);  		
	//GetDate(&date);
    //
	//if(CtmConfig::GetInstance()->GetKeyFlag()==1)
	//	sprintf(sz, "%04u.%02u.%02u/%02u:%02u", date.year, date.month, date.day, time.hour, time.minute);
  	//else
  	//	sprintf(sz, "%02u.%02u/%02u:%02u", date.month, date.day, time.hour, time.minute);	//Sunny<20080319> del year
  	//
  	////sprintf(sz, "%02u.%02u/%02u:%02u", date.month, date.day, time.hour, time.minute);	//Sunny<20080319> del year
  	//
    //g_pMainFrame->GetPromptBar()->GetControlA(0)->Clear();  
    //g_pMainFrame->GetPromptBar()->GetControlA(0)->ShowExtent(0, 1, sz, 0);
}

void	CtmPromptBar::Clear()
{
	RECT rect;
	if (m_pDoubleBuffer != NULL) 
		m_pDoubleBuffer->SetRegion(m_rectClient);
	else
	{
		m_pDoubleBuffer = new CtmGDIDoubleBuffer();
		m_pDoubleBuffer->SetRegion(m_rectClient);
		m_pDoubleBuffer->InitBuffer(m_bgc);
		m_pbmpBackground->SetDoubleBuffer(m_pDoubleBuffer);
	}
		
	MKRECT(rect, 0, 0, GetClientWidth() - m_wBorderWidth - 1, GetClientHeight()  - m_wBorderWidth - 1)
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
//	m_pbmpBackground.Create(PROMPT_BAR_DIR);
	m_pbmpBackground->Show(rect, NULL);	
}
