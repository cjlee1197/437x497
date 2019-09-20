/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"formview.h"
#include	"utils.h"
#include	"app.h"
#include	"commonaction.h"
#include	"main.h"
#include	"tmconfig.h"
#include	"gui_ex.h"
#include	"tmdebug.h"
#include	"freedata.h"
//#include	"memwatch.h"
#include	"tmshmsg.h"

#include	"switchbox.h"
#include	"canMaster.h"
#include 	"spinbox.h"
#include 	<guilite/IGui>
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/

typedef void *(* PRSRUN)(void *);

const	int		EVENT_ONFORM_CHANGE_NAME_DEFAULT_LEN	= 16;
const	int		EVENT_ONFORM_CHANGEBM_NAME_DEFAULT_LEN	= 16;
const	int		EVENT_ONFORM_CREATE_NAME_DEFAULT_LEN	= 16;
const	int		EVENT_ONFORM_SHOW_NAME_DEFAULT_LEN		= 8;
const	int		EVENT_ONFORM_KEY_NAME_DEFAULT_LEN		= 7;
const	int		EVENT_ONFORM_UPDATE_NAME_DEFAULT_LEN	= 16;
const	int		EVENT_ONFORM_DESTROY_NAME_DEFAULT_LEN	= 16;
#ifdef D_PXA270
const	int		CNT_INT_CMD_KEY_LOCK       				= 0xC082;
#else
const	int		CNT_INT_CMD_KEY_LOCK       				= 0x2000;
#endif
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
extern      CApp*               g_pApplication; // Application processor

extern		CtmKeyBoard*	g_ptaskKB;
extern		CtmGpio*	g_pgpio;
extern		BOOL			g_bClearOldName;
int				touch_down_control_index = -1;

int		CtmFormView::m_nAutoESC=0;
#include	"switchbox.h"
/*===========================================================================+
|           Class implementation - FormView                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmFormView, CtmDialog)

CtmFormView::CtmFormView() :  CtmDialog()
{
	strcpy(TYPEName, "CtmFormView");
	
	OnChangeA 	= NULL;
	OnCreateA 	= NULL; 
	OnShowA 	= NULL;
	OnKeyA 		= NULL;	
	OnUpdateA 	= NULL;
	OnUpdateB	= NULL;
	OnDestroyA 	= NULL;
	OnChangeBM	= NULL;
	OnMessage	= NULL;	//	fans 2008/6/16 11:31上午
	m_pszOnMouseDown	= NULL;
	m_pszOnMouseUp		= NULL;
	//	fans add 2009/3/17 10:42上午
	m_bPrivateUpdate	= TRUE;
	m_bCanStopUpdate	= FALSE;
	
	m_pszPrintName 		= NULL;
	m_pszPageNo 		= NULL;
	m_wDoubleHint		= 0;
	m_lHintDuration		= 500;
	AddTimer(&m_lHintDuration);
	m_lHelpTimer = 3000;				//Caozz add 2012/5/10 下午 03:26:53
	AddTimer(&m_lHelpTimer);
	//J.Wang add 2015/6/3 13:35:45
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;
	else
		m_iFontUNType		= 0;
	m_DrawHelpFlag		= FALSE;		//Caozz add 2012/5/10 下午 04:56:42
	
	//long long  llOptionOk = GetDBValue("MCONF_HMI_NULL_NULL_OPTIONOKOPTION").lValue;
	
	int nOptionOK = CtmConfig::GetInstance()->GetOKKey();
	if (nOptionOK == 1)
		m_wSaveKey			= _ENTER;	//	2010-1-15 9:55:51
	else
		m_wSaveKey			= _NULL_KEY;	//	2010-1-15 9:55:51
	
	for (int i = 0; i < 256; i++)
		m_aryUpdateWnd[i] = NULL;
	m_nUpdateNum = 0;
	//賤樵苤瑩攫拸楊豖堤.add by J.Wong 2015/12/8 14:34:11
	m_wViewType	 = 0;
	m_pszOnChangeName 	= new char[EVENT_ONFORM_CHANGE_NAME_DEFAULT_LEN];
	m_pszOnCreateName 	= new char[EVENT_ONFORM_CREATE_NAME_DEFAULT_LEN];
	m_pszOnShow 		= new char[EVENT_ONFORM_SHOW_NAME_DEFAULT_LEN];
	m_pszOnKey 			= new char[EVENT_ONFORM_KEY_NAME_DEFAULT_LEN];
	m_pszUpdate 		= new char[EVENT_ONFORM_UPDATE_NAME_DEFAULT_LEN];
	m_pszUpdateB		= new char[EVENT_ONFORM_UPDATE_NAME_DEFAULT_LEN];
	m_pszDestroy 		= new char[EVENT_ONFORM_DESTROY_NAME_DEFAULT_LEN];
	m_pszChangeBM		= new char[EVENT_ONFORM_CHANGEBM_NAME_DEFAULT_LEN];
	m_pszOnMessage		= new char[EVENT_ONFORM_UPDATE_NAME_DEFAULT_LEN];	//	fans 2008/6/16 11:31上午
	m_pszOnMouseDown	= new char[EVENT_ONFORM_UPDATE_NAME_DEFAULT_LEN];
	m_pszOnMouseUp		= new char[EVENT_ONFORM_UPDATE_NAME_DEFAULT_LEN];
	
	strcpy(m_pszOnChangeName, 	"OnChangeA");
	strcpy(m_pszChangeBM,		"OnChangeBM"); 
	strcpy(m_pszOnCreateName, 	"OnCreateA");
	strcpy(m_pszOnShow, 		"OnShowA");
	strcpy(m_pszOnKey, 			"OnKeyA");
	strcpy(m_pszUpdate, 		"OnUpdateA");
	strcpy(m_pszUpdateB,		"OnUpdateB");
	strcpy(m_pszOnMessage,		"OnMessage");	//	fans 2008/6/16 11:31上午
	strcpy(m_pszDestroy, 		"OnDestroyA");
	strcpy(m_pszOnMouseDown, 	"OnMouseDown");
	strcpy(m_pszOnMouseUp, 		"OnMouseUp");
	m_pwndMouseDown	= NULL;
	m_nFlag = 0;
//	pthread_create(&m_ThreadID, NULL, (PRSRUN)ThreadOnLineHelp, this);
}


CtmFormView::~CtmFormView()
{
	
	///*	fansTest
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
	//*/
	OnDestroyA 	= NULL;
	OnChangeA 	= NULL;
	OnCreateA 	= NULL;
	OnShowA 	= NULL;
	OnKeyA 		= NULL;
	OnUpdateA 	= NULL;
	OnChangeBM 	= NULL;
	OnMessage	= NULL;	//	fans 2008/6/16 11:31上午
	OnMouseUp	= NULL;
	OnMouseDown = NULL;
//	dlclose(GetLibHandle());
	DeleteTimer(&m_lHelpTimer);
	DeleteTimer(&m_lHintDuration);
//	pthread_cancel(m_ThreadID);
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/


BOOL	CtmFormView::CreateA()
{

//	char*	pszAry[200];
//	int		iNum = 0;
//	char*	pszTemp = NULL;
//	int		iTabOrder =-2;
//	
//	
//	
//	for (int i = 0; i < GetControlCount(); i++)
//	{
//		if (Controls[i]->Is("CtmEditX1") || Controls[i]->Is("CtmSelectEdit") || Controls[i]->Is("CtmEditX2"))
//		{
//			Controls[i]->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
//			pszTemp = *(char**)Controls[i]->GetPropValue("dbid0");
//			
//			if ((iTabOrder >= -2) && pszTemp != NULL)
//				pszAry[iNum++] = pszTemp;
//		}
//	}
//	//if (iNum > 0) 
//	//{
//	//	g_ptasksiem->ReqValues(iNum, 3, 0, pszAry);
//	//}
//	
//	int j = 0;
//	for (int i = 0; i < GetControlCount(); i++)
//	{
//		if (Controls[i]->Is("CtmEditX1")) j++;
//	}
	
		
	BOOL        bStatus;
	//if(!g_bClearOldName)
	{
		bStatus = CtmDialog::CreateA();
		m_nUpdateNum = GetSpecialControlNum(this,  "dbid0", m_aryUpdateWnd);	
		StrIDToStr(m_pszTitleID, &m_pszTitle);
		if (g_pMainFrame != NULL) 
		{
			g_pMainFrame->SetTitle(m_pszTitle);
		}
		
		//add by J.Wang 2015/7/17 16:35:55
		m_nClickCount=0;
		m_pOldControl=NULL;
		m_nIsCounter=1000;
		//m_nTabStop = 0;
		
	   	
		//	load Dll
		///*	fansTest
		char	libName[512]; 
		void*	libHandle = NULL;	
		char*	pszError = NULL;
		sprintf(libName, "%s%s%s", CtmConfig::GetInstance()->GetFormlibPath(), Name, ".so");
		libHandle = dlopen(libName, RTLD_LAZY);	
		
		pszError = dlerror();	//	需要檢測錯誤
		
		if (pszError != NULL)
			perror(pszError);
		
		//printf("Will Load so name is %s \n", libName);
		if (libHandle != NULL)
		{
			SetLibHandle(libHandle);
		}
		else
		{
			//========
			//	裝載通用的Dll，此項代碼如果放到Main裏面，雖然只需要裝載一次，
			//	但是不得不多用一個全局變量，增加了耦合，應該避免
			//========		
			
			sprintf(libName, "%s%s", CtmConfig::GetInstance()->GetFormlibPath(), "libcommon.so");
			libHandle = dlopen(libName, RTLD_NOW);
			SetLibHandle(libHandle);
			pszError = dlerror();	//	需要檢測錯誤
			if (pszError != NULL)
			{
				
				//cout << pszError << endl;
			}
		}
		
		
		
		if (GetLibHandle() != NULL)
		{
			OnCreateA = (BOOL(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszOnCreateName);
			pszError = dlerror();	//	需要檢測錯誤
			if (pszError != NULL)
			{
				OnCreateA = NULL;
				//cout << "Load " << m_pszOnCreateName << "  function is Error!Info: " << pszError << endl;
			}
			
		}
		else
		{
			//cout << Name << " Load dll failed!!" << endl;
			OnCreateA= NULL;
		}
		if (OnCreateA != NULL)	OnCreateA(this);
		SendMsg(MSG_WND_FORM_CREATE, 0, 0, this);
		
		m_pCurrentFocus = NULL;
	}
//	else
//	{
//		g_bClearOldName=FALSE;
//		
//		if (g_pMainFrame != NULL) 
//		{
//			StrIDToStr(m_pszTitleID, &m_pszTitle);
//			g_pMainFrame->SetTitle(m_pszTitle);
//		}	
//	}
	return TRUE;

}

void	CtmFormView::Clear()
{
	CtmWnd* 	pwndTemp = NULL;
	
	if (Get_TabStop() >= 0)
	{
		pwndTemp = FindControlFromTab(Get_TabStop());
		
		if (pwndTemp != NULL)
		{
			
			if (pwndTemp->Is("CtmEdit") || pwndTemp->Is("CtmEditX1") || pwndTemp->Is("CtmEditX2"))
			{
				
				((CtmEdit*)pwndTemp)->EnableCursor(FALSE);

			}
		}
	}
	CtmWnd::Clear();
}



void        CtmFormView::Update()         /* Virtual function */
{	
	if(g_nTouchView>0)
	{
		g_nTouchView--;
		if(g_nTouchView==0)
			UnEnableMouseCursor();
	}	
		
	//	fans modify 2009/3/16 06:12下午	
	if (!m_bCanStopUpdate)	//	can't stop update
	{
		OnFastUpdateCommon(m_aryUpdateWnd, m_nUpdateNum);
	}
	else if (m_bCanStopUpdate && m_bPrivateUpdate)	//	can stop update and PrivateUpdate is true
	{
		OnFastUpdateCommon(m_aryUpdateWnd, m_nUpdateNum);
	}
	else
	{
		;
	}
	//printf("m_nAutoESC %d , m_nIsCounter=%d...\n",m_nAutoESC,m_nIsCounter);
	//add by J.Wang 2015/7/17 16:36:09
//	if(m_nAutoESC)
//	{
//		m_nIsCounter-=10;
//		if(m_nIsCounter<=0)
//		{
//			m_nAutoESC=0;
//			m_nIsCounter=1000;
//			OnKey(_KEY_CLEAR);	
//		}	
//	}
	
	//if( 0 != strcmp( this->Name,"ViewFast" ) && 
	//	0 != strcmp( this->Name,"ViewVers" ))
	//{
	//	//cout << "this->Name =" << this->Name << endl;
	//	OnFastUpdateCommon(m_aryUpdateWnd, m_nUpdateNum);
	//}
	
	///*	fansTest
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnUpdateA = (void(*)(CtmWnd*))dlsym(GetLibHandle(), m_pszUpdate);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnUpdateA = NULL;
			////cout << "Load " << m_pszUpdate << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnUpdateA = NULL;
	if (OnUpdateA != NULL) 
	{
		OnUpdateA(this);
		//else
		//	ChangePosRegion();
	}
	if(RegionUpdateStop)
		ChangePos();
	if(m_lHintDuration<=0)
	{	
		
		char* pszCurrentFormName=g_pMainFrame->GetCurrentToolBar()->m_pszCurrentFormName;
//			if(m_wDoubleHint>=2)
//				OnlineHelpXml(pszCurrentFormName,-1);
//			else 
			if(m_wDoubleHint>0)	
			{
				CtmWnd*	pWndControl=FindControlFromTab(m_nTabStop);
				char*	pControlName=0;
				if(pWndControl)
				{
					pControlName=pWndControl->Name;	
				}
				OnlineHelpXml(pszCurrentFormName,m_nTabStop,pControlName);
			}
			m_wDoubleHint	=0;
			m_lHintDuration	=300;
	}
	//*/
//	if (m_pCurrentFocus && HelpTimeout() && 
//		(m_pCurrentFocus->HasOnlineHelp() || SetOnlineHelp(Name, m_pCurrentFocus) > 0))
//	{
//		m_pCurrentFocus->DrawOnlineHelp();
//		m_DrawHelpFlag = TRUE;
//		
//		//if(g_ptaskKB->GetKeyUp()) m_lHelpTimer = 3000;
//		////printf("g_ptaskKB->GetKeyUp() = %d, m_lHelpTimer = %d, m_pCurrentFocus = %s\n", g_ptaskKB->GetKeyUp(), m_lHelpTimer, m_pCurrentFocus->Name);			
//		//if (((m_wKey == 0x600A || m_wKey == 0x02D) || !(g_ptaskKB->GetKeyUp())) && m_lHelpTimer < 0)
//		if(m_nFlag == 1)
//		{
//			char helpStrID[128];
//			CtmWnd* pwndFocus = NULL;
//			
//			for (int i = 0; i < GetControlCount(); i++)
//			{
//				if (Controls[i]->OnFocus())
//				{
//					pwndFocus = Controls[i];
//					break;
//				}
//			}	
//			
//			if (pwndFocus && HelpTimeout())
//			{
//				memset(helpStrID, 0, sizeof(helpStrID));
//				//pwndFocus->GetPropValueT("helpstrID0", helpStrID, sizeof(helpStrID));
//				strcpy(helpStrID, pwndFocus->GetHelpStrID(0));
//				if (helpStrID[0])
//					OnlineHelp(pwndFocus);
//			}
//		}
//	}
//	else 
//	{
//		m_lHelpTimer = 3000;
//		m_DrawHelpFlag = FALSE;
//	}
}

void	CtmFormView::UpdateB(void* pData)
{
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnUpdateB = (void(*)(CtmWnd*, void*))dlsym(GetLibHandle(), m_pszUpdateB);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnUpdateA = NULL;
			////cout << "Load " << m_pszUpdate << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnUpdateB = NULL;
		
	if (OnUpdateB != NULL) OnUpdateB(this, pData);
}


WORD        CtmFormView::OnKey(WORD wKey) /* Virtual function */
{
	//add by J.Wang 2015/7/17 16:35:42
//	if(m_nAutoESC)
//	{
//		m_nIsCounter=1000;	
//	}
	//test for panel key redirect.
	#ifdef D_X86
	if (wKey == 0x6700)
		wKey = _KEY_UPFIELD;
	if (wKey == 0x6c00)
		wKey = _KEY_DOWNFIELD;
	if (wKey == 0x6900)
		wKey = _KEY_LEFTFIELD;
	if (wKey == 0x6a00)
		wKey = _KEY_RIGHTFIELD;
	#endif
	#ifdef D_X86
	if(wKey==0x6D00)	//for test
	{
		if(!m_wDoubleHint)	m_lHintDuration=300;
		m_wDoubleHint++;
		//return _NULL_KEY;
	}
	#else
		if(wKey==0xFEFE)
		{
			if(!m_wDoubleHint)	m_lHintDuration=300;
			m_wDoubleHint++;
			//return wKey;
		}
	#endif
	///*fansTest
	#ifndef      D_DEMO
	if( CtmConfig::GetInstance()->GetKeyLockFlag() == 0)			//鍵盤Data鎖 1軟體鎖 0為硬體鎖
	{
			#ifdef	D_PXA270
			if (g_nRTCFd > 0)
			{
				int		nStat = ioctl(g_nRTCFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#else
			if (g_nBatteryFd > 0)
			{
				int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#endif
		
	}
	else if( CtmConfig::GetInstance()->GetKeyLockFlag() == 1)
	{
		//當為over畫面和password畫面時候extag = 10000，不用鎖鍵盤
		int Type = 0;
		GetPropValueT("extag",&Type,sizeof(Type));
		if(Type < 10000)
		{
			if(g_bKeyLockFlag)
				return _NULL_KEY;
		}
	}
	else if(CtmConfig::GetInstance()->GetKeyLockFlag() == 2)
	{
		#ifdef	D_PXA270
			if (g_nRTCFd > 0)
			{
				int		nStat = ioctl(g_nRTCFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#else
			if (g_nBatteryFd > 0)
			{
				int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
		#endif
		//當為over畫面和password畫面時候extag = 10000，不用鎖鍵盤
		int Type = 0;
		GetPropValueT("extag",&Type,sizeof(Type));
		if(Type < 10000)
		{
			if(g_bKeyLockFlag)
				return _NULL_KEY;
		}
	}
	
	else if(CtmConfig::GetInstance()->GetKeyLockFlag() == 4)
	{
		//printf("g_dwM3Key[1]=%x\n",g_dwM3Key[1]);
		if(!(g_dwM3Key[1]&0x2000000))
		{
			//printf("g_dwM3Key[1]=%x\n",g_dwM3Key[1]);
			return _NULL_KEY;
		}
	}
	#endif
	if ((wKey == 0x600A || wKey == 0x02D) && m_DrawHelpFlag) 	//for online help
	{
		m_lHelpTimer = 3000;		
	}
	m_wKey = wKey;
	
	char*	pszError = NULL;
	//printf("wKey=%x,OnKeyA=%x\n",wKey,OnKeyA);
	if (OnKeyA != NULL)
	{
		return OnKeyA(this, wKey);
	}
	else
	{
		if (GetLibHandle() != NULL)
		{
			OnKeyA = (WORD(*)(CtmWnd*, WORD))dlsym(GetLibHandle(), m_pszOnKey);
			pszError = dlerror();	//	需要檢測錯誤
			if (pszError != NULL)
			{
				OnKeyA = NULL;
				//cout << "Load " << m_pszOnKey << "  function is Error!Info: " << pszError << endl;
			}
		}		
		
		if (OnKeyA != NULL)	return OnKeyA(this, wKey);
		else return OnKey1(wKey);
    }
    //*/
    return wKey; //	fansTest
}


void		CtmFormView::SetPropertys()
{
	CtmDialog::SetPropertys();
	
	SetPropInfo("OnChangeA", 	tmPROCEDURE,	&m_pszOnChangeName);
	SetPropInfo("OnChangeBM", 	tmPROCEDURE,	&m_pszChangeBM);
	SetPropInfo("OnCreateA",	tmPROCEDURE,	&m_pszOnCreateName);
	SetPropInfo("OnShowA",		tmPROCEDURE,	&m_pszOnShow);
	SetPropInfo("OnKeyA",		tmPROCEDURE,	&m_pszOnKey);
	SetPropInfo("OnUpdateA",	tmPROCEDURE,	&m_pszUpdate);
	SetPropInfo("OnDestroyA",	tmPROCEDURE,	&m_pszDestroy);
	SetPropInfo("OnMessage",	tmPROCEDURE,	&m_pszOnMessage);	//	fans 2008/6/16 11:32上午
	SetPropInfo("OnMouseDown",	tmPROCEDURE,	&m_pszOnMouseDown);
	SetPropInfo("OnMouseUp",	tmPROCEDURE,	&m_pszOnMouseUp);
	SetPropInfo("pageno",		tmSTR,			&m_pszPageNo);
	SetPropInfo("OnUpdateB",	tmPROCEDURE,	&m_pszUpdateB);
	SetPropInfo("printname", 	tmSTR, 			&m_pszPrintName);
	
	//	fans add 2009/3/17 10:43上午
	SetPropInfo("canstop",		tmBOOL,			&m_bCanStopUpdate);
	SetPropInfo("privateupdate",tmBOOL,			&m_bPrivateUpdate);
	
	SetPropInfo("viewtype",		tmWORD,			&m_wViewType);
	
	SetPropInfo("savekey",		tmWORD,			&m_wSaveKey);
	
	SetPropInfo("FBIndex",		tmINT,			&m_iFBIndex);
	SetPropInfo("FBStartX",		tmINT,			&m_iStartX);
	SetPropInfo("FBStartY",		tmINT,			&m_iStartY);
}

void		CtmFormView::SetPropertys_Nub()
{
	CtmDialog::SetPropertys_Nub();
	
	SetPropInfo(FORMVIEW_ONCHANGEA, 	tmPROCEDURE,	&m_pszOnChangeName);
	SetPropInfo(FORMVIEW_ONCHANGEBM, 	tmPROCEDURE,	&m_pszChangeBM);
	SetPropInfo(FORMVIEW_ONCREATEA,		tmPROCEDURE,	&m_pszOnCreateName);
	SetPropInfo(FORMVIEW_ONSHOWA,		tmPROCEDURE,	&m_pszOnShow);
	SetPropInfo(FORMVIEW_ONKEYA,		tmPROCEDURE,	&m_pszOnKey);
	SetPropInfo(FORMVIEW_ONUPDATEA,		tmPROCEDURE,	&m_pszUpdate);
	SetPropInfo(FORMVIEW_ONDESTROYA,	tmPROCEDURE,	&m_pszDestroy);
	SetPropInfo(FORMVIEW_ONMESSAGE	,	tmPROCEDURE,	&m_pszOnMessage);	//	fans 2008/6/16 11:32上午
	SetPropInfo(FORMVIEW_PAGENO,		tmSTR,			&m_pszPageNo);
	SetPropInfo(FORMVIEW_ONUPDATEB,		tmPROCEDURE,	&m_pszUpdateB);
	SetPropInfo(FORMVIEW_PRINTNAME, 	tmSTR, 			&m_pszPrintName);
	
	//	fans add 2009/3/17 10:43上午
	SetPropInfo(FORMVIEW_CANSTOP,		tmBOOL,			&m_bCanStopUpdate);
	SetPropInfo(FORMVIEW_PRIVATEUPDATE,	tmBOOL,			&m_bPrivateUpdate);
	
	SetPropInfo(FORMVIEW_SAVEKEY,		tmWORD,			&m_wSaveKey);
}

WORD    CtmFormView::OnChange(WORD wIDControl)   
{
	///*fansTest
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnChangeA = (WORD(*)(CtmWnd*, WORD))dlsym(GetLibHandle(), m_pszOnChangeName);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnChangeA = NULL;
			////cout << "Load " << m_pszOnChangeName << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnChangeA = NULL;
	
	CtmDialog::OnChange(wIDControl);	
	if (OnChangeA != NULL)
	return OnChangeA(this, wIDControl);
	//*/
	return	wIDControl;	//	fansTest
}

void    CtmFormView::Show()
{
	//printf("CtmFormView::Show() Name = %s \n",this->Name);
	//GetParent()->OnLoseFocus();
	//Clear();	
	
	
	CtmDialog::Show();
	///*fansTest
	//this->GetParent()->OnGetFocus();
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnShowA = (void(*)(CtmWnd*))dlsym(GetLibHandle(), m_pszOnShow);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnShowA = NULL;
			////cout << "Load " << m_pszOnShow << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnShowA = NULL;
	
	if (OnShowA != NULL) OnShowA(this);
	//*/
	Goto(0);
	
		
	// <FANS> 2006/11/23 12:41下午 test
	//_GuiInitEx();
	
	//clock_t start, finish;
	//double  duration;
	//start = clock();
	//if (strcmp(Name, "ViewPassWord") == 0)
	//{
	//	printf("password!!!\n");
	//}
	//_FillRegionWithLightEx(80, 80,  4, 138, 223);
	//_FillRegionEx(80, 80,  200, 0, 0);
	//
	//finish = clock();
	//duration = (double)(finish - start) / CLOCKS_PER_SEC;	
	//printf( "Init array time=%f\n", duration );
	
}

WORD	CtmFormView::OnChangeBMEvent(WORD wIDControl)
{
	///*fansTest
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnChangeBM = (WORD(*)(CtmWnd*, WORD))dlsym(GetLibHandle(), m_pszChangeBM);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnChangeBM = NULL;
			////cout << "Load " << m_pszOnChangeName << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnChangeBM = NULL;
	
	if (OnChangeBM != NULL)
	{
		return OnChangeBM(this, wIDControl);
	}
	else
		return	wIDControl;	
}


WORD	CtmFormView::OnMouseDownEvent(WORD wIDControl)
{
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnMouseDown = (WORD(*)(CtmWnd*, WORD))dlsym(GetLibHandle(), m_pszOnMouseDown);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnMouseDown = NULL;
			////cout << "Load " << m_pszOnChangeName << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else OnMouseDown = NULL;
	
	if (OnMouseDown != NULL)
	{
		return OnMouseDown(this, wIDControl);
	}
	else
		return	wIDControl;	
}

WORD	CtmFormView::OnMouseUPEvent(WORD wIDControl)
{
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnMouseUp = (WORD(*)(CtmWnd*, WORD))dlsym(GetLibHandle(), m_pszOnMouseUp);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnMouseUp = NULL;
			////cout << "Load " << m_pszOnChangeName << "  function is Error!Info: " << pszError << endl;
		}		
	}
	else
		OnMouseUp = NULL;
	if (OnMouseUp != NULL)
	{
		return OnMouseUp(this, wIDControl);
	}
	else
		return	wIDControl;	
}

BOOL	CtmFormView::MouseDown(short x_pos,short y_pos,int key_flag)
{
	if(!g_bMute)
	{
		Beep(); //cjlee 2019/5/16 上午 08:51:33
	}

	//int this_privilege = -1;
	//this->GetPropValueT("privilege", &this_privilege, sizeof(this_privilege));
	//if(g_iPrivilege <= this_privilege) return 1;
		
	int     i, nSNub = -1;
    RECT    rect;
    WORD    wKey;
    
    CtmWnd  *pControl = NULL;
    
    rect    = GetWindowRECT();
    touch_down_control_index = -1;
  if(m_wViewType == VIEW_TYPE_TOUCH_MSG && (x_pos < rect.right && x_pos > rect.left && y_pos < rect.bottom && y_pos > rect.top))
	{
	  m_TOUCH_MSG_Down=TRUE;
	  return TRUE;
	}
	if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
  {
      return 1;
  }
    
  for (i = 0; i < GetControlCount(); i++)
	{
		BOOL temp_enabled = TRUE;
		Controls[i]->GetPropValueT("enabled", &temp_enabled, sizeof(temp_enabled));	
	    //if(Controls[i]->GetTabOrder() < -1 && !Controls[i]->Is("CtmMemo")) continue;
	    if(Controls[i]->GetTabOrder() < -1 && !Controls[i]->Is("CtmMemo") || temp_enabled == FALSE) continue; //Mario 配合手動模式做法
	    rect    = Controls[i]->GetWindowRECT();
	    if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
	        continue;
	    else
	    	nSNub = i;
	}
	
	if (nSNub >= 0)
	{
		int Controls_privilege = -1;
		Controls[nSNub]->GetPropValueT("privilege", &Controls_privilege, sizeof(Controls_privilege));
		//printf("g_iPrivilege=%d   Controls_privilege=%d\n",g_iPrivilege,Controls_privilege);
		if(g_iPrivilege < Controls_privilege) 
		{
			Prompt(g_MultiLanguage["ROBOT_STR_PRIVILEGE_NO_ENOUGH"],0);
			return 1;
		}
		touch_down_control_index = nSNub;
		if(strstr(Controls[nSNub]->TYPEName, "CtmFileExplorer"))
		{
			if(m_nTabStop != Controls[nSNub]->GetTabOrder())
			{
				OnLoseFocus();									//對上一個元件LoseFocus
				m_nTabStop = Controls[nSNub]->GetTabOrder();	//獲取當前元件的taborder
				OnGetFocus();									//設置當前元件的Focus
			}
			OnMouseDownEvent(Controls[nSNub]->GetTabOrder());
		}
		//James add 因selectedit 沒有下拉列表，改值沒有OnChange事件。
//		printf("m_pwndMouseDown:%x\n", m_pwndMouseDown);
		if (m_pwndMouseDown!= NULL && Controls[nSNub] != m_pwndMouseDown && strstr(m_pwndMouseDown->TYPEName, "SelectEdit") != NULL)
	    {
	       	//OnChangeBMEvent(m_pwndMouseDown->GetTabOrder());			//Mario Mark but i dont know have what error
	       	//OnChange(m_pwndMouseDown->GetTabOrder());							//Mario Mark but i dont know have what error
	       	//m_pwndMouseDown = NULL;																//Mario Mark but i dont know have what error
	       	//Mario  估計是給無下拉式選單的SelectEdit所使用的特殊處理，但會影響到有下拉選單SelectEdit，故此處註解
	       	;
	    }
	    else if (m_pwndMouseDown!= NULL && Controls[nSNub] != m_pwndMouseDown && strstr(m_pwndMouseDown->TYPEName, "CtmSelectBit") != NULL)
	    {
	       	OnChangeBMEvent(m_pwndMouseDown->GetTabOrder());
	       	OnChange(m_pwndMouseDown->GetTabOrder());
	       	m_pwndMouseDown = NULL;
	    }
	    
		//return 0;
		m_pwndMouseDown = Controls[nSNub];
		//	printf("%s mouse down\n", Controls[nSNub]->Name);
		if(Controls[nSNub] != NULL && strstr(Controls[nSNub]->TYPEName, "Button") != NULL)
		{
			int u_nWndExTag = -1;
			(CtmButton*)(Controls[nSNub])->GetPropValueT("extag", &u_nWndExTag, sizeof(u_nWndExTag));
			if(u_nWndExTag != 20001 && u_nWndExTag !=20002)
			((CtmButton*)Controls[nSNub])->Press(tmBUTTON_DOWN);
			OnMouseDownEvent(m_pwndMouseDown->GetTabOrder());
		}
		else if(Controls[nSNub] != NULL && strstr(Controls[nSNub]->TYPEName, "CtmEdit") != NULL)
		{
			if(Controls[nSNub]->GetTabOrder() == -1 )
			{
				int u_nWndExTag = -1;
				Controls[nSNub]->GetPropValueT("extag", &u_nWndExTag, sizeof(u_nWndExTag));
				SendMsg(MSG_SH_APP_FORMVIEW, u_nWndExTag, 0, NULL);
			}
			else
			{
				int pTabOrder=nSNub;
				pControl    = FindControlFromTab(m_nTabStop);
				if(Controls[nSNub] == pControl)
				{
					pTabOrder = OnMouseDownEvent(m_pwndMouseDown->GetTabOrder());
					if(FindControlFromTab(pTabOrder) != Controls[nSNub])
					m_pwndMouseDown	=	NULL;
				}
			}
		}
		else if(Controls[nSNub] != NULL && strstr(Controls[nSNub]->TYPEName, "Toggle") != NULL)
		{
			OnMouseDownEvent(m_pwndMouseDown->GetTabOrder());
		}
		else if(Controls[nSNub] != NULL && strstr(Controls[nSNub]->TYPEName, "DataGrid") != NULL)
		{
			OnMouseDownEvent(m_pwndMouseDown->GetTabOrder());
		}
		
	}
	return TRUE;
}

BOOL	CtmFormView::MouseUp(short x_pos,short y_pos,int key_flag)
{
	//int this_privilege = -1;
	//this->GetPropValueT("privilege", &this_privilege, sizeof(this_privilege));
	//if(g_iPrivilege <= this_privilege) return 1;
	int     i, nSNub = -1;
    RECT    rect;
    WORD    wKey;
    CtmWnd  *pControl = NULL;
    rect    = GetWindowRECT();
  if(m_TOUCH_MSG_Down==TRUE && m_wViewType == VIEW_TYPE_TOUCH_MSG && (x_pos < rect.right && x_pos > rect.left && y_pos < rect.bottom && y_pos > rect.top))
  {
  	m_TOUCH_MSG_Down=FALSE;
  	OnChange(0);
  }    

    
  if(m_pwndMouseDown != NULL && strstr(m_pwndMouseDown->TYPEName, "Button"))						//還原toolbar背景圖片
	{
		int u_nWndExTag = -1;
		(CtmButton*)(m_pwndMouseDown)->GetPropValueT("extag", &u_nWndExTag, sizeof(u_nWndExTag));
		if(u_nWndExTag != 20001 && u_nWndExTag !=20002)
		{
			((CtmButton*)m_pwndMouseDown)->Press(tmBUTTON_UP);
		}
	}
	if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
  {
   	if (m_wViewType == VIEW_TYPE_INPUTFORM)			//Gerry add for input form 2010/9/25 下午 04:24:06
    {												//if click on the outside of the view, it will be disappeared.
    	OnKey(_ENTER);
    }
    return 1;
  }
  
	for (i = 0; i < GetControlCount(); i++)
	{
		if(!Controls[i]->GetVisible()) continue;
	    if(Controls[i]->GetTabOrder() < -1 && !Controls[i]->Is("CtmMemo")) continue;
	    rect    = Controls[i]->GetWindowRECT();
	    if(x_pos > rect.right+2 || x_pos < rect.left-2 || y_pos > rect.bottom+2 || y_pos < rect.top-2)
	        continue;
	    else
	    	nSNub = i;
	}
	if(touch_down_control_index != nSNub) return FALSE;
	pControl    = FindControlFromTab(m_nTabStop);
	if(m_nTabStop<0) pControl=NULL;
  /*if(pControl != NULL)
  {
  	if (m_wViewType == VIEW_TYPE_COMMON && m_pwndMouseDown != NULL && !m_pwndMouseDown->Is("CtmMemo"))
  	{
	      wKey    = pControl->OnKey(_ENTER);
	      if(wKey == _NULL_KEY)   
	      {
	      	if (strstr(pControl->TYPEName, "CtmEdit") != NULL)
	      	{
	      		
					m_nAutoESC=1;
					if(CtmConfig::GetInstance()->GetKeyLockFlag() == 4)
	      			{
	      				if(!(g_dwM3Key[1]&0x2000000))
	      				{
	      						
	      				}
	      				else
	      				{
	      					InputBox(pControl);	
	      				}	
	      			}
	      			else
	      			{
	        			InputBox(pControl);
	        		}
	        		m_nAutoESC=0;
	        		m_nIsCounter=1000;
	      		return FALSE;
	      	}
	      }
	      
	      if (strstr(pControl->TYPEName, "SelectEdit") != NULL)
	      {
	      	OnChange(pControl->GetTabOrder());
	      }
	  }
  }
   */
   
  if (nSNub < 0)
  {
    m_nAutoESC=1;
		return FALSE;
	}
		
	int Controls_privilege = -1;
	Controls[nSNub]->GetPropValueT("privilege", &Controls_privilege, sizeof(Controls_privilege));
	//printf("g_iPrivilege=%d   Controls_privilege=%d\n",g_iPrivilege,Controls_privilege);
	if(g_iPrivilege < Controls_privilege) 
	{
		Prompt(g_MultiLanguage["ROBOT_STR_PRIVILEGE_NO_ENOUGH"],0);
		return 1;
	}
		
	if(m_pwndMouseDown == Controls[nSNub])			//如果按下去和鬆開的元件一致，則有效，否則無效
  {
		/*if(strstr(Controls[nSNub]->TYPEName, "CtmMTextMemo"))
		{
			//OnLoseFocus();									//對上一個元件LoseFocus
			m_nTabStop = Controls[nSNub]->GetTabOrder();	//獲取當前元件的taborder
			//OnGetFocus();									//設置當前元件的Focus
		}*/
   	if(Controls[nSNub]->Is("CtmMemo"))
   		;
   	else
   	{
	   	if(pControl != Controls[nSNub])
	   	{
	    	/*if (m_wViewType == VIEW_TYPE_COMMON ||strstr(Controls[nSNub]->TYPEName, "Button") == NULL)
	   		{
					BOOL temp_enabled = TRUE;
					Controls[nSNub]->GetPropValueT("enabled", &temp_enabled, sizeof(temp_enabled));	
					if(temp_enabled)
					{
						OnLoseFocus();									//對上一個元件LoseFocus
						m_nTabStop = Controls[nSNub]->GetTabOrder();	//獲取當前元件的taborder
						OnGetFocus();									//設置當前元件的Focus
					}
				}
				else */if ((strstr(Controls[nSNub]->TYPEName, "CtmEdit") != NULL 
										|| strstr(Controls[nSNub]->TYPEName, "SelectEdit") != NULL 
										|| strstr(Controls[nSNub]->TYPEName, "CtmCheckBox") != NULL 
										|| strstr(Controls[nSNub]->TYPEName, "CtmMTextMemo") != NULL)
										&& m_nTabStop != Controls[nSNub]->GetTabOrder())
	   		{
	   			if(Controls[nSNub]->GetTabOrder() >= 0 )
	   			{
						OnLoseFocus();									//對上一個元件LoseFocus
						m_nTabStop = Controls[nSNub]->GetTabOrder();	//獲取當前元件的taborder
						pControl = Controls[nSNub];
						OnGetFocus();			
					}		
	   		}
			}
			m_bChangeTabStop = TRUE; 
		}
		
		if(pControl != NULL)
		{
			if ((strstr(pControl->TYPEName, "CtmEdit") != NULL ||strstr(pControl->TYPEName,"SpinBox"))&& m_wViewType != VIEW_TYPE_LOGINFORM)
			{
				if (pControl == Controls[nSNub])
				{
					if(strstr(pControl->TYPEName,"SpinBox"))
					{
						CPoint pt=CPoint(x_pos, y_pos);
						bool res=((CtmSpinBox*)pControl)->InputBoxContains(pt);
						if(!res)
						{
							return TRUE;
						}
						if(CtmConfig::GetInstance()->GetKeyLockFlag() == 4)
	        		{
	        			if(!(g_dwM3Key[1]&0x2000000))
	        			{
	        				;	
	        			}
	        			else
	        			{
	        				m_nAutoESC=1;
	        				InputBox(pControl);
	        				m_nAutoESC=0;
	        				m_nIsCounter=1000;	
	        			}	
	        		}
	        		else
	        		{
	        			m_nAutoESC=1;
		        		InputBox(pControl);
		        		m_nAutoESC=0;
	        			m_nIsCounter=1000;
		        	}
		    		InputBox(pControl);
					}
					else 
		    		InputBox(pControl);
				}
			}
			else if(strstr(pControl->TYPEName, "CtmEdit") != NULL && m_wViewType == VIEW_TYPE_LOGINFORM)
			{
				if (pControl == Controls[nSNub])
				{
					;
				}	
				else
				{
					OnMouseUPEvent(Controls[nSNub]->GetTabOrder());
				}
			}
			else if(strstr(pControl->TYPEName, "SelectEdit") != NULL || 
				strstr(pControl->TYPEName, "CtmCheckBox") != NULL )
        	{
        		if(pControl == Controls[nSNub])		//第二次在同一個元件點擊，則發生值的變化		
        		{
        			OnKey(_KEY_TEXTINPUT);
        		}
        	}
        	else if(strstr(pControl->TYPEName, "CtmOptionBox") != NULL ||
					strstr(pControl->TYPEName, "CtmSelectBox") != NULL)
			{
				if(pControl == Controls[nSNub])
				{
					if(m_pOldControl!=pControl)
					{
						if(!m_nClickCount)
						{
							m_pOldControl=pControl;
							m_nClickCount=1;
							pControl->OnKey(_ENTER);
							OnChange(pControl->GetTabOrder());
						}
						else
						{
							;	
						}
					}
					else
					{
						if(m_nClickCount)
						{
							m_nClickCount=0;
							pControl->OnKey(_ESC);
							OnChange(pControl->GetTabOrder());
						}
						else
						{
							m_nClickCount=1;
							pControl->OnKey(_ENTER);
							OnChange(pControl->GetTabOrder());
						}
					}
					
				}	
			}
			else if(strstr(pControl->TYPEName, "CtmSelectBit") != NULL)
			{
				if(pControl == Controls[nSNub])		//第二次在同一個元件點擊，則發生值的變化		
        		{
        			OnKey(_KEY_TEXTINPUT);
        		}
        		else 								//第一次GetFocus不需要發送任何值變化
        		{
        			;
        		}	
			}
		}
		
		if(strstr(Controls[nSNub]->TYPEName, "Button") != NULL)    
		{
			WORD	wKey = 0;
			BOOL temp_enabled = TRUE;
			Controls[nSNub]->GetPropValueT("key", &wKey, sizeof(wKey));
			Controls[nSNub]->GetPropValueT("enabled", &temp_enabled, sizeof(temp_enabled));	
			if(temp_enabled)
				OnMouseUPEvent(Controls[nSNub]->GetTabOrder());
			if (wKey != 0) OnKey(wKey);
		}
		
		else if(strstr(Controls[nSNub]->TYPEName, "Toggle") != NULL)
		{
			OnMouseUPEvent(Controls[nSNub]->GetTabOrder());
		}
		
		else if(Controls[nSNub]->Is("CtmDataGrid") != NULL)
		{
			OnMouseUPEvent(Controls[nSNub]->GetTabOrder());
		}
		
		else if (Controls[nSNub]->Is("CtmSwitchBox"))
        {
        	((CtmSwitchBox *)Controls[nSNub])->Switch();
        }
		
		//if(Controls[nSNub]->Is("CtmToolButton"))
		//{
		//	((CtmToolButton*)m_pwndMouseDown)->Press(tmBUTTON_UP);
		//}
		
		
	}
	else
	{
		;
	}
	return TRUE;
}

void	CtmFormView::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	//if(message==MSG_MOUSEMOVE)
	//	printf("lParam = %d ,MouseMoveStep=%d\n",lParam,MouseMoveStep);
	int touch_process_done=0;
	for (int i = 0; i < GetControlCount(); i++)
  {
		if(message==MSG_MOUSELUP || message==MSG_MOUSELDOWN || (message==MSG_MOUSEMOVE && lParam==MouseMoveStep))
		{
			if (Controls[i]->Is("CtmMemo"))
			{
				Controls[i]->WndProc(message, wParam, lParam);
				touch_process_done=1;
				break;
			}
    }
 	}	
 	if(touch_process_done==0)
 	{
		for (int i = 0; i < GetControlCount(); i++)
	  {
			/* || Controls[i]->Is("CtmCurveEx")|| Controls[i]->Is("CtmFlowChar") 
			||Controls[i]->Is("CtmListView")||Controls[i]->Is("CtmSlider")||Controls[i]->Is("CtmRing")
			||Controls[i]->Is("CtmRings")||Controls[i]->Is("CtmDial")||Controls[i]->Is("CtmToggle")
			||Controls[i]->Is("CtmDoubleSpinBox")||Controls[i]->Is("CtmPlot")||Controls[i]->Is("CtmDataGrid")*/
			if(message==MSG_MOUSELUP || message==MSG_MOUSELDOWN || (message==MSG_MOUSEMOVE && lParam==MouseMoveStep))
			{
				if (Controls[i]->Is("CtmMemo")|| Controls[i]->Is("CtmCurveEx") || Controls[i]->Is("CtmFileExplorer")
					  ||Controls[i]->Is("CtmSpinBox")|| Controls[i]->Is("CtmMTextMemo"))
				{
					Controls[i]->WndProc(message, wParam, lParam);
					break;
				}
	    }
	 	}	
	}
	switch(message)
	{
		case MSG_MOUSELUP:
			EnableMouseCursor(wParam >> 16,wParam & 0x0000FFFF);
			MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
			break;
		case MSG_MOUSELDOWN:
			EnableMouseCursor(wParam >> 16,wParam & 0x0000FFFF);
			MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
			break;
		case MSG_MOUSEMOVE:
			EnableMouseCursor(wParam >> 16,wParam & 0x0000FFFF);
			break;
  	default:
			break;
	}
	//printf("formview wndproc  3\n");
	char* pszError = NULL;
	if (GetLibHandle() != NULL)
	{
		OnMessage = (void(*)(CtmWnd*, int, WPARAM, LPARAM))dlsym(GetLibHandle(), m_pszOnMessage);
		pszError = dlerror();	//	需要檢測錯誤
		if (pszError != NULL)
		{
			OnMessage = NULL;
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

WORD	CtmFormView::OnKey1(WORD wKey)
{
	//#ifndef      D_DEMO
	//if (g_nBatteryFd > 0)
	//{
	//	int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
	//	//g_tmDebugInfo->PrintDebugInfo("Lock DAta = %d\n", nStat);
	//	if (nStat) return _NULL_KEY;
	//}
	//#endif
	#ifndef      D_DEMO
	if( CtmConfig::GetInstance()->GetKeyLockFlag() == 0)			//鍵盤Data鎖 1軟體鎖 0為硬體鎖 2為都使用
	{
			#ifdef	D_PXA270
			if (g_nRTCFd > 0)
			{
				int		nStat = ioctl(g_nRTCFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#else
			if (g_nBatteryFd > 0)
			{
				int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#endif
		
	}
	else if( CtmConfig::GetInstance()->GetKeyLockFlag() == 1)
	{
		//當為over畫面和password畫面時候extag = 10000，不用鎖鍵盤
		int Type = 0;
		GetPropValueT("extag",&Type,sizeof(Type));
		if(Type < 10000)
		{
			if(g_bKeyLockFlag)
				return _NULL_KEY;
		}
	}
	else if(CtmConfig::GetInstance()->GetKeyLockFlag() == 2)
	{
		#ifdef	D_PXA270
			if (g_nRTCFd > 0)
			{
				int		nStat = ioctl(g_nRTCFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#else
			if (g_nBatteryFd > 0)
			{
				int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
		#endif
		//當為over畫面和password畫面時候extag = 10000，不用鎖鍵盤
		int Type = 0;
		GetPropValueT("extag",&Type,sizeof(Type));
		if(Type < 10000)
		{
			if(g_bKeyLockFlag)
				return _NULL_KEY;
		}
	}
	#endif
	MSG         msg;
	CtmWnd* 	pwndTemp = NULL;	//	fans add
    if (m_nTabStop >= 0)
    {
    	pwndTemp = FindControlFromTab(m_nTabStop);
    	
    	if (pwndTemp != NULL)     	
    	{
    		wKey = pwndTemp->OnKey(wKey);
    	}
    	else 
    	{
    		return _NULL_KEY;
    	}
    switch (wKey)
        {
        case _KEY_LEFTFIELD:
        case _KEY_RIGHTFIELD:
        case _KEY_UPFIELD:
        case _KEY_DOWNFIELD:
    
        case _UARROW:
        case _EXT_UARROW:
        case _SHIFT_TAB:
        case _DARROW:
        case _EXT_DARROW:
        case _TAB:
        case _ENTER:
		case _KEY_SELECT:							//<<yang 2005/12/8 
        case (WORD)_EXT_ENTER:
        	
            break;
        default:
        	
            return _NULL_KEY;
        }
    m_bChangeTabStop = FALSE;                // Clear the flag to synchronize with CtmDialog::Goto(wIDControl)
    
    int	nTabStop = m_nTabStop;
    //	fans modify for the ok key 2010-1-15 9:56:16
    if (m_wSaveKey == _NULL_KEY || m_wSaveKey == wKey)  	
    {
    	OnChangeBMEvent(m_nTabStop); 
    	CtmDialog::OnLoseFocus();//CtmDialog::OnLose();//
    }
	else
	{
		CtmDialog::OnLose();
	}
    
        
    pwndTemp->Get();
    
    //	fans modify for the ok key
    if (m_wSaveKey == _NULL_KEY || m_wSaveKey == wKey)
    {
	    msg.wMessage = _SYS_CHANGE;
	    msg.wIDControl = nTabStop;
	    g_pApplication->SetMessage(CMD_SYSTEM, &msg);
	}
	else
	{
		;	//	do nothing
	}
	
	
    if (!m_bChangeTabStop)
        {
        	
        switch (wKey)
            {
            case _KEY_LEFTFIELD:
                FindLeftTabStop();
                break;
            case _KEY_RIGHTFIELD:
                FindRightTabStop();
                break;
            case _KEY_UPFIELD:
                FindUpTabStop();
                break;
            case _KEY_DOWNFIELD:
                FindDownTabStop();
                break;
            case _UARROW:
            case _EXT_UARROW:
            case _SHIFT_TAB:
                FindLastTabStop();
                break;
            case _DARROW:
            case _EXT_DARROW:
            case _TAB:
            case _ENTER:
            case (WORD)_EXT_ENTER:
                //FindNextTabStop();		//Mario Mark 2015/3/13 下午 05:12:13 較符合觸控板本 可能會導致錯誤
                break;
    
			case _KEY_SELECT:							//<<yang 2005/12/8 
            default:
                break;
            }
            
        	CtmDialog::OnGetFocus();                      // Update focus should be the last thing due to object parameters may be changed during _SYS_CHANGE process
        	
        }
        
        
              
    }

    return _NULL_KEY;
}

WORD	CtmFormView::OnKey2(WORD wKey)
{
	#ifndef      D_DEMO
	if( CtmConfig::GetInstance()->GetKeyLockFlag() == 0)			//鍵盤Data鎖 1軟體鎖 0為硬體鎖
	{
			#ifdef	D_PXA270
			if (g_nRTCFd > 0)
			{
				int		nStat = ioctl(g_nRTCFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#else
			if (g_nBatteryFd > 0)
			{
				int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#endif
		
	}
	else if( CtmConfig::GetInstance()->GetKeyLockFlag() == 1)
	{
		//當為over畫面和password畫面時候extag = 10000，不用鎖鍵盤
		int Type = 0;
		GetPropValueT("extag",&Type,sizeof(Type));
		if(Type < 10000)
		{
			if(g_bKeyLockFlag)
				return _NULL_KEY;
		}
	}
	else if(CtmConfig::GetInstance()->GetKeyLockFlag() == 2)
	{
		#ifdef	D_PXA270
			if (g_nRTCFd > 0)
			{
				int		nStat = ioctl(g_nRTCFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
			#else
			if (g_nBatteryFd > 0)
			{
				int		nStat = ioctl(g_nBatteryFd, CNT_INT_CMD_KEY_LOCK, 0);//	
				if (nStat && wKey != _KEY_CLEAR) return _NULL_KEY;
			}
		#endif
		//當為over畫面和password畫面時候extag = 10000，不用鎖鍵盤
		int Type = 0;
		GetPropValueT("extag",&Type,sizeof(Type));
		if(Type < 10000)
		{
			if(g_bKeyLockFlag)
				return _NULL_KEY;
		}
	}
	#endif
	
	return CtmDialog::OnKey(wKey);
}

void	CtmFormView::ThreadOnLineHelp(CtmFormView *pFormView)
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		
	}
	while(1)
	{
		if(pFormView->m_DrawHelpFlag)
		{
			if(g_ptaskKB->GetKeyUp()) pFormView->m_lHelpTimer = 3000;	
			
			if (((pFormView->m_wKey == 0x600A || pFormView->m_wKey == 0x02D) || !(g_ptaskKB->GetKeyUp())) && pFormView->m_lHelpTimer < 0)
				pFormView->m_nFlag = 1;
			else
				pFormView->m_nFlag = 0;
		}
		
		usleep(20*1000);
	}
}
	