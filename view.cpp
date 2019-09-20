/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : View                                                           |
|  Task     : View                                                           |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 05/02/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"view.h"
#include	"main.h"	//	fans add
#include	"utils.h"	//	fans add
#include	"parsepule.h"	//	fans add
#include	"tmconfig.h"
#include 	<guilite/IUtils>
//#include	"memwatch.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

#define		MAX_PARSEVIEW_NUM 		20
static		WORD		g_ParseViewCount=0;
extern		CtmMainFrame*       g_pMainFrame; 
extern		BOOL				g_bRestartSystem;
BOOL		g_bRefreshForm=FALSE;
BOOL		g_bRefreshForm2=FALSE;
extern		int		g_nViewChange;
CParseObject* ParseView;	//	fans add
CParseObject* ParseView2;	//	fans add
CParseObject* ParseView1; // Wong add
CParseObject*	g_ParseView[20];
static  BOOL	g_bReInitFlag=FALSE;
static	char	szOldCmd[256] = "\0";	
BOOL			g_bOldReInitFlag=FALSE;
char			g_pszCmd[20][256];
struct timeval _tstart, _tend;
long long 	lTime = 0;
BOOL	g_bFocusChanged=FALSE;
BOOL	g_bFocusChanged2=FALSE;

char		g_pszFormScripts[15][64];
int		g_nViewTaborder[16]={0};
extern	sem_t 	g_SemGUI;

extern	int			g_nCommand;

//extern		BOOL	g_bProcessViewCmdFlag;
extern		BOOL	g_bSubToolbarFlag;
extern int			g_nSetView_StartX;
extern int			g_nSetView_EndX;
extern int			g_nSetView_StartY;
extern int			g_nSetView_EndY;
extern int			g_FbIotcol;
/*===========================================================================+
|           Class implementation - View                                      |
+===========================================================================*/
IMPLEMENT_DYNCREATE(CtmView, CtmWnd)
CtmView::CtmView() : CtmWnd()
{
	//	fans add

	strcpy(TYPEName, "CtmView");
	m_pszOverViewName = NULL;
	m_pszBmpBackPath = NULL;
	m_pszOldFormName[0] = '\0';
	m_pszFormName[0] = '\0';
	m_pszOldFormName2[0] = '\0';
	m_pszFormName2[0] = '\0';
}

CtmView::~CtmView()
{
	if (ParseView != NULL)
	{
		ParseView->m_aryControls[0].control = NULL;	//	這樣就避免了釋放2次的錯誤
		//printf( "ParseView delete \n ");
	}
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
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
BOOL        CtmView::Create(tmWND* pwnd)
{
	if (!CtmWnd::Create(pwnd)) return FALSE;
	m_bGetFocus = FALSE;
	m_bOnChange = FALSE;                    // St072601 for save time 
	m_pwndFocus = NULL;

	m_bGetFocus2 = FALSE;					//James add for view2
	m_bOnChange2 = FALSE;
	m_pwndFocus2 = NULL;
	printf("clear \n");

	g_wIDView = 0;
	_SetViewPort(m_rectClient.left, m_rectClient.top,
		m_rectClient.right, m_rectClient.bottom);

	m_pbmpBackGround = g_aIniBmp[BMP_OVERVIEWID].pIniBmp;
	OnCommand(ID_VIEW_OVERVIEW);
	return TRUE;
}

//	fans add
BOOL	CtmView::CreateA()
{
	CtmWnd::CreateA();
		
	
	m_bGetFocus = FALSE;
	m_bOnChange = FALSE;                    // St072601 for save time 
	m_pwndFocus = NULL;

	m_bGetFocus2 = FALSE;
	m_bOnChange2 = FALSE;
	m_pwndFocus2 = NULL;
	//printf("clear2 \n");

	g_wIDView = 0;
	
	_SetViewPort(0, 0,
		m_rectClient.right, m_rectClient.bottom);
	SetBmpBack(m_pszBmpBackPath);
	
	return TRUE;
}

//	fans add
void	CtmView::SetBmpBack(char* pszFileName)
{
	if (pszFileName == NULL) 
	{
		m_pbmpBackGround = NULL;
		return;// m_pbmpBackGround = g_aIniBmp[BMP_BACKGROUNDID].pIniBmp;
	}
	else 
	{		
		int i = 0;
		bool b_find = FALSE;
		while (g_aIniBmp[i].cBMPPath[0] != '\0')
		{
			if (strcmp(g_aIniBmp[i].cBMPPath, pszFileName) == 0)
			{
				m_pbmpBackGround = g_aIniBmp[i].pIniBmp; 
				b_find = true;
				////cout << g_aIniBmp[i].cBMPPath << " finded the bitmap!!" << endl;
				break;
			}
			i++;
		}
		
		if (!b_find)
		{
			g_aIniBmp[i].pIniBmp = new CtmBitMap;
			
			
			if (g_aIniBmp[i].pIniBmp->Create(pszFileName) == tmBMP_ERROR_FILE)
			{
				m_pbmpBackGround = NULL;
			}
			else
			{
				strncpy(g_aIniBmp[i].cBMPPath, pszFileName, 256);
				m_pbmpBackGround = g_aIniBmp[i].pIniBmp; 	
			}
		}
		
		/*
		m_bmpBack.Create(pszFileName);
		m_pbmpBackGround = &m_bmpBack;
		*/
	}
	
	
	RECT	rectRedraw;
	MKRECT(rectRedraw, 0, 0, GetClientWidth()-1, GetClientHeight()-1)
	
	if (m_pbmpBackGround != NULL)
		m_pbmpBackGround->Show(rectRedraw, 0);
	
}

//	fans add
void 	CtmView::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	SetPropInfo("overviewname", 	tmSTR,		&m_pszOverViewName);
	SetPropInfo("bmpbackpath", 		tmSTR,		&m_pszBmpBackPath);	
}


void 	CtmView::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	
	SetPropInfo(VIEW_OVERVIEWNAME, 	tmSTR,		&m_pszOverViewName);
	SetPropInfo(VIEW_BMPBACKPATH, 		tmSTR,		&m_pszBmpBackPath);	
}

void        CtmView::Update()
{
	int	nCount = GetControlCount();
	
//	printf("nCount =%d \n", nCount );
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmFormView"))
		{
			Controls[i]->Update();
		}
	}
	if(g_ptmControlServer != NULL && g_ptmControlServer->ErrorControl() != NULL)
	g_ptmControlServer->ErrorControl()->Error_Control();
}

void		CtmView::OnResize()
{
}

void        CtmView::Redraw(RECT rect)
{
	RECT	rectRedraw;
	MKRECT(rectRedraw, 0, 0, GetClientWidth()-1, GetClientHeight()-1)
	
	if (m_pbmpBackGround != NULL)
		m_pbmpBackGround->Show(rectRedraw, 0);
	m_pwndFocus->DrawBorder();
	m_pwndFocus->Redraw(rect);
}

//	fans add
void    	CtmView::Show()
{
	
	CtmWnd::Show();
		
	//SetBmpBack(m_pszBmpBackPath);// <FANS> 2006/11/22 04:33下午
}

void        CtmView::OnGetFocus()
{
	
	if(m_pwndFocus == NULL) 
	{
		return;
	}
	if (!m_bGetFocus)
    { 
		m_pwndFocus->OnGetFocus();
		m_bGetFocus = TRUE;
    }

}

void        CtmView::OnGetFocus2()
{

	if(m_pwndFocus2 == NULL)
	{
		return;
	}
	if (!m_bGetFocus2)
    {
		m_pwndFocus2->OnGetFocus();
		m_bGetFocus2 = TRUE;
    }

}

void        CtmView::OnLoseFocus()
{
	if ((m_bGetFocus) && (m_pwndFocus != NULL))
    {
    	g_bFocusChanged|=0x01;
		m_pwndFocus->OnLoseFocus();
		m_bGetFocus = FALSE;
		g_bFocusChanged&=0xF0;
	}
}
void        CtmView::OnLoseFocus2()
{
	if ((m_bGetFocus2) && (m_pwndFocus2 != NULL))
    {
    	g_bFocusChanged2=TRUE;
		m_pwndFocus2->OnLoseFocus();
		m_bGetFocus2 = FALSE;
		g_bFocusChanged2=FALSE;
    }
}

WORD        CtmView::OnKey(WORD wKey)
{
	//printf("CtmView::OnKey wKey=%x \n", wKey);
	//Sunny<20080128><<<<<add Switch language
//	if (wKey == _KEY_LANGUAGE)//_KEY_UPFIELD use this key test,ok!
//	{
//		int languageindex = 0;
//		int LanguageNum  =0;
//		languageindex = CtmConfig::GetInstance()->GetCurrentIndex();
//    	LanguageNum = CtmConfig::GetInstance()->GetLanguageNum();
//    	if((languageindex < 0)||(languageindex > LanguageNum-2))	languageindex = 0;
//		else languageindex++;
//		CtmConfig::GetInstance()->SetCurrentIndex(languageindex);
//		//cout << "languageindex:" <<languageindex<< endl;
//		
//		g_pInit->Init_System();
//		
//    	//::Reset();
//	}
	//Sunny<20080128>add>>>>>

	if (m_pwndFocus != NULL)	//	fans add
	{
		return m_pwndFocus->OnKey(wKey);
	}
	else 
	{
		////cout << "m_pwndFocus is NULL!" << endl;
	}
}

WORD        CtmView::OnCommand(char *pszCmd)
{
	 extern BOOL  g_bStopViewUpdate;
	 g_bStopViewUpdate = FALSE;
	 //gettimeofday(&_tend,NULL);
	//printf("End Respond:%ld\n", _tend.tv_usec);
	gettimeofday(&_tstart, NULL);
	CtmConfig*	pconfig = NULL;
	BOOL		bFindScripts=FALSE;
	BOOL		bSetParent=FALSE;
	if(pszCmd == NULL || pszCmd[0] == '\0')
	{
		//g_bProcessViewCmdFlag = TRUE;
		return FALSE;
	}
	char	szCmd[256]= "\0";
	//static	CtmView*     pwndOldView = NULL;
	char  szFormScripts[128]= "\0";
	memset(szFormScripts,0,sizeof(szFormScripts)/sizeof(szFormScripts[0]));
	memset(szCmd, 0, sizeof(szCmd));
	//memset(szOldCmd, 0, sizeof(szOldCmd));


	strncpy(m_pszFormName, pszCmd, sizeof(m_pszFormName));
	sprintf(szCmd, "%s%s", pconfig->GetInstance()->GetFormPath(), pszCmd);
	if(access(szCmd,F_OK))
	{
		//g_bProcessViewCmdFlag = TRUE;
		return FALSE;
	}
	if (strcmp(m_pszOldFormName, szCmd) != 0)
	{
		strncpy(m_pszOldFormName, szCmd, 256);
	}
	else
	{
		//g_bProcessViewCmdFlag = TRUE;
		return TRUE;
	}
	RegionUpdateStop = TRUE;
	//printf("m_pszOldFormName=%s,szCmd=%s\n",m_pszOldFormName,szCmd);
	g_nViewChange = 1;	//prepare change
	g_nCommand    = 1;
	//ioctl(g_FbIotcol, 0xFFFF11, NULL);
	g_wIDView = m_wCmd;
	char	pszFormName[256];// = NULL;
	pszFormName[0] = '\0';
	if (m_pwndFocus != NULL)
	{
		strcpy(pszFormName, m_pwndFocus->Name);
	}

	clock_t start, finish;
	double  duration;

	int nIndex=-1;
	// add by Wong 2014/12/8 20:04:02
	 if(g_bRestartSystem)
	 {
		g_bRestartSystem=FALSE;
		if(ParseView1!=NULL)
		{
			ParseView1->OpenFile(szOldCmd);
			ParseView1->CloseFile();
			ParseView1=NULL;
		}
		memset(szOldCmd,0,sizeof(szOldCmd));
		g_bReInitFlag=FALSE;
		g_bOldReInitFlag=FALSE;
	 }
	 OnLoseFocus();
	 if(g_bRefreshForm)
	 {
	 	g_bRefreshForm=FALSE;
	 	if(ParseView!=NULL)
	 	{
	 		//printf("here 1 ...ParseView=%x \n", ParseView);
	 		ParseView->ReInitData();
	 	}

	 }
	//printf("szOldCmd=%s,m_pszOldFormName=%s,szCmd=%s\n",szOldCmd,m_pszOldFormName,szCmd);
	if((nIndex= SearchParseView(szOldCmd))>=0)
	{

		g_bOldReInitFlag=TRUE;
		ParseView1 = g_ParseView[nIndex];
	 	//printf("here 2 ...ParseView1=%x nIndex=%d \n", ParseView1, nIndex);
		ParseView1->ReInitData();
	}
	else
	{
		g_bOldReInitFlag=FALSE;
	}
	if((nIndex= SearchParseView(szCmd))>=0)
	{
		//printf("nIndex=%d \n", nIndex);
		ParseView1 = g_ParseView[nIndex];
		bFindScripts=TRUE;
	}
	else
	{
		for(int i=0;i<sizeof(g_pszFormScripts)/sizeof(g_pszFormScripts[0]);i++)
		{
			sprintf(szFormScripts, "%s%s", pconfig->GetInstance()->GetFormPath(), g_pszFormScripts[i]);
			//printf("szFormScripts=%s szCmd=%s \n", szFormScripts, szCmd);
			if(strcmp(szFormScripts,szCmd)==0)
			{
				bFindScripts=TRUE;
				break;
			}
		}
		if(bFindScripts)
		{
			if(g_bReInitFlag)
			{
				//printf("szOldCmd=%s\n",szOldCmd);
				//printf("here 2 ...ParseView=%x \n", ParseView);
				ParseView->ReInitData();
				g_bReInitFlag=FALSE;
			}
			ParseView1 = new CParseObject;
			//printf("Create:%p\n",ParseView1);
			ParseView1->OpenFile(szCmd);
			//printf("here 9..\n");
			ParseView1->CloseFile();

			//m_pwndFocus = ParseView1->m_aryControls[0].control;
			//m_pwndFocus->SetParent(g_pMainFrame->GetCurrentView());
			//m_pwndFocus->CreateA();

			AddParseView(ParseView1);
			//printf("===add new szCmd=%s \n", szCmd);
			AddFormCmd(szCmd);
		}
		else
		{
			if(ParseView==NULL)
				ParseView = new CParseObject;
			g_bReInitFlag=TRUE;
			//printf("Create2:%x szCmd=%s \n",ParseView, szCmd);
			ParseView->OpenFile(szCmd);
			ParseView->CloseFile();
		}

	}
	if (m_pwndFocus != NULL)
	{
		char	szImagePath[128];
		if(g_bSubToolbarFlag)
		{
			sprintf(szImagePath, "%s/Background1.bmp", CtmConfig::GetInstance()->GetBmpPath());
			Controls[0]->SetPropValueT("imagepath", szImagePath);
			Controls[0]->Show();
		}
		else
		{
			sprintf(szImagePath, "%s/Background.bmp", CtmConfig::GetInstance()->GetBmpPath());
			Controls[0]->SetPropValueT("imagepath", szImagePath);
			Controls[0]->Show();
		}
	    m_pwndFocus = NULL;
	}

	m_bPreControl = FALSE;
	//printf("bFindScripts=%d \n", bFindScripts);
	if(bFindScripts)
	{
		//memcpy(&ParseView2, ParseView1, sizeof(ParseView2));
		//printf("sizeof =%d \n", sizeof(ParseView2));
		if(g_bReInitFlag)
		{
			//printf("szOldCmd=%s\n",szOldCmd);
			ParseView->ReInitData();
			g_bReInitFlag=FALSE;
		}
		//printf("here 2 ParseView1->m_aryControls[0].control->Name=%s...\n", ParseView1->m_aryControls[0].control->Name);
		m_pwndFocus = ParseView1->m_aryControls[0].control;
	}
	else
	{
		m_pwndFocus = ParseView->m_aryControls[0].control;
		//m_pwndFocus->SetParent(g_pMainFrame->GetCurrentView());
		//m_pwndFocus->CreateA();
	}
	//printf("here 2-1 ...\n");
	m_pwndFocus->SetControlCountZero(g_pMainFrame->GetCurrentView());

	//gettimeofday(&_tend, NULL);
	//lTime = (_tend.tv_sec * 1000000 + _tend.tv_usec) - (_tstart.tv_sec * 1000000 + _tstart.tv_usec);
	//printf("Show0.11  %s need time: %d us\n", pszCmd, lTime);
	//printf("here 2-2 ...\n");
	m_pwndFocus->SetParent(g_pMainFrame->GetCurrentView());

	//gettimeofday(&_tend, NULL);
	//lTime = (_tend.tv_sec * 1000000 + _tend.tv_usec) - (_tstart.tv_sec * 1000000 + _tstart.tv_usec);
	//printf("Show0.12  %s need time: %d us\n", pszCmd, lTime);
	//printf("here 2-3 ...\n");
	//gettimeofday(&_tend, NULL);
	//lTime = (_tend.tv_sec * 1000000 + _tend.tv_usec) - (_tstart.tv_sec * 1000000 + _tstart.tv_usec);
	//printf("Show0.2  %s need time: %d us\n", pszCmd, lTime);
	//printf("here 3 ...\n");
	Prompt((char*)NULL);
	m_pwndFocus->CreateA();
	m_pwndFocus->Show();
	
	if(CtmConfig::GetInstance()->GetPrintTimeFlag()==1)
	{
		gettimeofday(&_tend, NULL);
		lTime = (_tend.tv_sec * 1000000 + _tend.tv_usec) - (_tstart.tv_sec * 1000000 + _tstart.tv_usec);
		printf("Show  %s need time: %d us\n", pszCmd, lTime);
	}
	if (pszFormName[0] != '\0')
		if (strcmp(pszFormName, m_pwndFocus->Name) == 0) m_bOnChange = TRUE;

	if (m_bOnChange)
	 {
	    	m_bOnChange = FALSE;                    // St1123'02 for save time
	  }
	//printf("here 4 ...\n");
	OnGetFocus();
	//printf("here 5 ...\n");
	g_nViewChange = 2; //change over

	//if(g_nSetView_StartX > m_pwndFocus->m_rectWindow.left)
//	g_nSetView_StartX = m_pwndFocus->GetWindowRECT().left;
//	g_nSetView_StartY = m_pwndFocus->GetWindowRECT().top;
//	g_nSetView_EndX = m_pwndFocus->GetWindowRECT().right;
//	g_nSetView_EndY = m_pwndFocus->GetWindowRECT().bottom;


	//g_nCommand    = 2;
	//ChangePos();
	//ioctl(g_FbIotcol, 0xFFFF12, NULL);
	//usleep(50*1000);
	//printf("1 reOpen szOldCmd=%s szCmd=%s \n", szOldCmd, szCmd);
	if((nIndex= SearchParseView(szOldCmd))>=0)
	{
		//printf("nValue=%d\n",GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
		ParseView1 = g_ParseView[nIndex];
		ParseView1->OpenFile(szOldCmd);
		//printf("nValue=%d\n",GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
		ParseView1->CloseFile();
		//printf("nValue=%d\n",GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
		
		//ParseView1->m_aryControls[0].control->SetParent(pwndOldView);
		//ParseView1->m_aryControls[0].control->CreateA();
		//printf("2 reOpen szOldCmd=%s szCmd=%s \n", szOldCmd, szCmd);
	}
	//printf("here 6 ...\n");
	strncpy(szOldCmd, szCmd, 128);		//James add 2014/5/17 9:47:40
	//pwndOldView = g_pMainFrame->GetCurrentView();
	if((nIndex= SearchParseView(szOldCmd))>=0)
	{
		ParseView1 = g_ParseView[nIndex];
	}
	else
	{
		ParseView1=NULL;
	}
	return TRUE;
}


WORD        CtmView::OnCommand2(char *pszCmd)
{
	CtmConfig*	pconfig = NULL;
	BOOL		bFindScripts=FALSE;
	BOOL		bSetParent=FALSE;

	if(pszCmd == NULL || pszCmd[0] == '\0')
	{
		return FALSE;
	}
	char	szCmd[256]= "\0";

	char  szFormScripts[128]= "\0";
	memset(szFormScripts,0,sizeof(szFormScripts)/sizeof(szFormScripts[0]));
	memset(szCmd, 0, sizeof(szCmd));

	strncpy(m_pszFormName, pszCmd, sizeof(m_pszFormName));
	sprintf(szCmd, "%s%s", pconfig->GetInstance()->GetFormPath(), pszCmd);
	if (strcmp(m_pszOldFormName2, szCmd) != 0)
	{
		strncpy(m_pszOldFormName2, szCmd, 256);
	}
	else return TRUE;

	g_nViewChange = 1;	//prepare change
	g_nCommand    = 1;
//	ioctl(g_FbIotcol, 0xFFFF11, NULL);
	g_wIDView = m_wCmd;
	char	pszFormName[256];// = NULL;
	pszFormName[0] = '\0';
	if (m_pwndFocus2 != NULL)
	{
		strcpy(pszFormName, m_pwndFocus2->Name);
	}

	OnLoseFocus2();

	if(ParseView2==NULL)
		ParseView2 = new CParseObject;
	g_bReInitFlag=TRUE;
	//printf("Create2:%x szCmd=%s \n",ParseView, szCmd);
	ParseView2->OpenFile(szCmd);
	ParseView2->CloseFile();

	//printf("m_pwndFocus2=%x \n", m_pwndFocus2);
	//if (m_pwndFocus2 != NULL)
	//{
	//	//Show background
	if(Controls[0] != NULL)
	    Controls[0]->Show();
	 //   m_pwndFocus2 = NULL;
	//}
	if(ParseView2->m_aryControls[0].control == NULL)			//James add 2013/4/28 11:07:46
		return FALSE;
	RegionUpdateStop = TRUE;
	m_pwndFocus2 = ParseView2->m_aryControls[0].control;
	//printf("here 2-1 ...\n");
	m_pwndFocus2->SetControlCountZero(g_pMainFrame->GetCurrentView2());

	m_pwndFocus2->SetParent(g_pMainFrame->GetCurrentView2());

	m_pwndFocus2->CreateA();

	//Prompt((char*)NULL);
	m_pwndFocus2->Show();

	//printf("m_pwndFocus2=%x \n", m_pwndFocus2);
	if (pszFormName[0] != '\0')
		if (strcmp(pszFormName, m_pwndFocus2->Name) == 0) m_bOnChange2 = TRUE;

	if (m_bOnChange2)
	{
		m_bOnChange2 = FALSE;                    // St1123'02 for save time
	}

	OnGetFocus2();
	g_nViewChange = 2; //change over

	//g_nSetView_StartX = m_pwndFocus->m_rectWindow.left;
	//g_nSetView_StartY = m_pwndFocus->m_rectWindow.top;
	//g_nSetView_EndX = m_pwndFocus->m_rectWindow.right;
	//g_nSetView_EndY = m_pwndFocus->m_rectWindow.bottom;

	//g_nCommand    = 3;
	//ChangePos();
	//ioctl(g_FbIotcol, 0xFFFF13, NULL);
	return TRUE;
}

void 		CtmView::ClearOldName()
{
	int	nIndex = -1;
/*	int nIndex=-1;
	if((nIndex=SearchParseView(m_pszOldFormName))>=0)
	{
		//OnLoseFocus();
		if (m_pwndFocus != NULL)
		{
 			Controls[0]->Show();
  			m_pwndFocus = NULL;
		}

		ParseView2 = g_ParseView[nIndex];
		g_bOldReInitFlag=FALSE;
		ParseView2->OpenFile(m_pszOldFormName);
		ParseView2->CloseFile();
		m_bPreControl = FALSE;
		if(ParseView2->m_aryControls[0].control == NULL)			//James add 2013/4/28 11:07:46
		return ;
		m_pwndFocus = ParseView2->m_aryControls[0].control;
		//m_pwndFocus->SetControlCountZero(g_pMainFrame->GetCurrentView());
		m_pwndFocus->SetParent(g_pMainFrame->GetCurrentView());
//		m_pwndFocus->CreateA();
//		m_pwndFocus->GetParent()->GetParent()->GetControlA(2)->CreateA();
//		m_pwndFocus->GetParent()->GetParent()->GetControlA(2)->Show();
//		printf("Name=%s\n",m_pwndFocus->GetParent()->GetParent()->GetControlA(2)->Name);
		Prompt((char*)NULL);
		m_pwndFocus->Show();
		OnGetFocus();
		//ParseView2->OpenFile(m_pszOldFormName);
		//ParseView2->CloseFile();
	    }
//	    else
//	    	memset(m_pszOldFormName, 0, sizeof(m_pszOldFormName));
*/
		if((nIndex= SearchParseView(m_pszOldFormName))>=0)
		{
			if(ParseView1 != NULL)
			{
				g_bOldReInitFlag=TRUE;
				OnLoseFocus();
				if (m_pwndFocus != NULL)
				{
					char	szImagePath[128];
					if(g_bSubToolbarFlag)
					{
						sprintf(szImagePath, "%s/Background1.bmp", CtmConfig::GetInstance()->GetBmpPath());
						Controls[0]->SetPropValueT("imagepath", szImagePath);
						Controls[0]->Show();
					}
					else
					{
						sprintf(szImagePath, "%s/Background.bmp", CtmConfig::GetInstance()->GetBmpPath());
						Controls[0]->SetPropValueT("imagepath", szImagePath);
						Controls[0]->Show();
					}
					m_pwndFocus = NULL;
				}
				ParseView1 = g_ParseView[nIndex];
				//printf("clear old Name ParseView1=%x nIndex=%d \n", ParseView1, nIndex);
				ParseView1->ReInitData();
				ParseView1->OpenFile(m_pszOldFormName);
				ParseView1->CloseFile();

				if(ParseView1->m_aryControls[0].control == NULL)			//James add 2013/4/28 11:0
					return ;

				ParseView1->m_aryControls[0].control->SetParent(g_pMainFrame->GetCurrentView());
				m_pwndFocus = ParseView1->m_aryControls[0].control;
				ParseView1->m_aryControls[0].control->CreateA();

				Prompt((char*)NULL);
				if(m_pwndFocus != NULL)
				m_pwndFocus->Show();
				OnGetFocus();
				g_bRefreshForm=TRUE;
			}
		}
		else
		{
			if(ParseView != NULL)
			{
				OnLoseFocus();
				if (m_pwndFocus != NULL)
				{
					char	szImagePath[128];
					if(g_bSubToolbarFlag)
					{
						sprintf(szImagePath, "%s/Background1.bmp", CtmConfig::GetInstance()->GetBmpPath());
						Controls[0]->SetPropValueT("imagepath", szImagePath);
						Controls[0]->Show();
					}
					else
					{
						sprintf(szImagePath, "%s/Background.bmp", CtmConfig::GetInstance()->GetBmpPath());
						Controls[0]->SetPropValueT("imagepath", szImagePath);
						Controls[0]->Show();
					}
					m_pwndFocus = NULL;
				}
					//printf("ParseView reinit Data m_pszOldFormName=%s \n", m_pszOldFormName);
					ParseView->ReInitData();
					ParseView->OpenFile(m_pszOldFormName);
					ParseView->CloseFile();
					m_bPreControl = FALSE;
					if(ParseView->m_aryControls[0].control == NULL)			//James add 2013/4/28 11:0
						return ;

					ParseView->m_aryControls[0].control->SetParent(g_pMainFrame->GetCurrentView());
					m_pwndFocus = ParseView->m_aryControls[0].control;
					ParseView->m_aryControls[0].control->CreateA();

				Prompt((char*)NULL);
				if(m_pwndFocus != NULL)
					m_pwndFocus->Show();
				OnGetFocus();
				g_bRefreshForm=TRUE;
			}
		}

}
WORD        CtmView::OnCommand(WORD wCmd)
{


	return FALSE;
}

WORD        CtmView::OnCommand2(WORD wCmd)
{


	return FALSE;
}

void		CtmView::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	if (m_pwndFocus != NULL)
		m_pwndFocus->WndProc(message, wParam, lParam);
}

WORD        CtmView::OnChange(WORD wIDControl)
{
	m_bOnChange = TRUE;
	return m_pwndFocus->OnChange(wIDControl);
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

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
void CtmView::View_()
{

}

#ifndef   D_NHLP     //Chuntzu 2004/8/3 add
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
void  CtmView::Help()
{
	/*	fans mark
CtmDlgHelp    DlgHelp;

DlgHelp.DoModal();
*/
}
#endif   //D_NHLP

void CtmView::UpdateFrameTitle()
{
	char        szTitle[tmMAX_TEXT_COL+1];
	//sprintf(szTitle, "%s - %s", m_szView, dbmhdr.szID);               
	sprintf(szTitle, "%s", m_szView);               
	g_pMainFrame->SetTitle(szTitle);
}


 int	CtmView::SearchParseView(char* pszCmd)
 {
 	int nIndex=-1;
 	for(int i=0; i<g_ParseViewCount;i++)
 	{
 		//printf("g_pszCmd[i]=%s pszCmd=%s\n", g_pszCmd[i], pszCmd);
 		if(strcmp(g_pszCmd[i],pszCmd)==0)
 		{	
 			nIndex=i;
 			break;
 		}
 	}
 	return nIndex;
 }
 
void	CtmView::AddParseView(CParseObject* ParseView)
{
	if(g_ParseViewCount>=MAX_PARSEVIEW_NUM)
	{
		return ;
	}
	g_ParseView[g_ParseViewCount]=ParseView;
	
}

 void	CtmView::AddFormCmd(char* pszCmd)
 {
 	//printf("AddFormCmd g_ParseViewCount=%d pszCmd=%s \n", g_ParseViewCount, pszCmd);
 	memset(g_pszCmd[g_ParseViewCount],0,sizeof(g_pszCmd[g_ParseViewCount]));
 	strcpy(g_pszCmd[g_ParseViewCount++],pszCmd);

 }