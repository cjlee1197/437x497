/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : tmToolBar                                                      |
|  Task     : Tool Bar                                                       |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 08/05/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	<dlfcn.h>

#include	"toolbar.h"
#include	"stable.h"
#include	"cmddef.h"
#include	"utils.h"
#include	"init.h"
#include	"button.h"
#include	"parsepule.h"
#include	"tmconfig.h"
#include 	"commonaction.h"

//extern timeval _tstart;
//extern timeval _tend;
//extern long long lTime;
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
const	int		EVENT_ONTOOLBAR_KEYSTART_NAME_DEFAULT_LEN		= 7;
const	int		EVENT_ONTOOLBAR_KEYEND_NAME_DEFAULT_LEN		= 20;
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CParseObject*	m_parseButton = NULL;		//	專門用來解析ToolButton的
BOOL			g_bSubToolbarFlag = FALSE;
BOOL			g_bStopViewUpdate = FALSE;
char			g_pszCurrentFormName[64];
extern int		g_FbIotcol;
extern int      g_nCommand;

char 			m_pszWndName[256];
char 			m_pszBarName[256];
/*===========================================================================+
|           Class implementation - tmToolBar                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmToolBar, CtmWnd)
IMPLEMENT_DYNCREATE(CtmSubToolBar, CtmToolBar)
CtmToolBar::CtmToolBar() : CtmWnd()
{
	strcpy(TYPEName, "CtmToolBar");
	m_wSP = 0;
	m_bBmpButton = TRUE;					//<<yang 2005/10/21 
	m_dwKeyColor=0xFFFF;
	m_fgc = m_wndattr.fgcToolBar1;
	m_bgc = m_wndattr.bgcToolBar1;
	
	
	//	fans add
	m_pszBmpBackPath 		= NULL;
	m_pszBmpUpPath 			= NULL;
	m_pszBmpDownPath 		= NULL;
	m_pszMainToolName 		= NULL;
	m_pszCurrentToolName 	= NULL;
	//g_pszCurrentFormName 	= NULL;
	memset(g_pszCurrentFormName, 0, sizeof(g_pszCurrentFormName));
		
	OnCreateA 				= NULL;
	OnToolBarKeyA 			= NULL;
	OnToolBarKeyEnd 		= NULL;
	OnMessage				= NULL;	//	fans 2008/6/16 11:31上午
	
	m_pszOnCreateName 		= new char[EVENT_ONTOOLBAR_KEYEND_NAME_DEFAULT_LEN];
	m_pszOnKey 				= new char[EVENT_ONTOOLBAR_KEYSTART_NAME_DEFAULT_LEN];
	m_pszOnKeyEnd 			= new char[EVENT_ONTOOLBAR_KEYEND_NAME_DEFAULT_LEN];
	m_pszOnMessage			= new char[EVENT_ONTOOLBAR_KEYEND_NAME_DEFAULT_LEN];	//	fans 2008/6/16 11:31上午

	memset(m_pszBarName, 0, sizeof(m_pszBarName));
	memset(m_pszWndName, 0, sizeof(m_pszWndName));
	strcpy(m_pszOnCreateName,"OnCreateA");
	strcpy(m_pszOnKey, 		"OnKeyA");
	strcpy(m_pszOnKeyEnd, 	"OnKeyAEnd");
	strcpy(m_pszOnMessage,	"OnMessage");	//	fans 2008/6/16 11:31上午
	
	m_nToolIndex = 0;		//James add 2011/8/2 下午 05:16:00
	
	if (m_parseButton == NULL) m_parseButton = new CParseObject();
	
	for (int i = 0; i < tmMAX_TOOLNEST; i++) m_ToolStack[i].pszBarName = NULL;
	m_pbmpBackground = NULL;
}

CtmToolBar::~CtmToolBar()
{	
	//===========
	//	如果不將m_parseButton->m_aryControls[0]的控件置為NULL的話，在釋放m_parseButton的時候
	//	會重新釋放一次，但是由於m_aryControls[0]是從屬于CtmToolBar的，所以CtmToolBar將會自動釋放
	//	它從屬的所有元件
	//===========
	m_parseButton->m_aryControls[0].control = NULL;
	delete m_parseButton;
	m_parseButton = NULL;
	for (int i = 0; i < tmMAX_TOOLNEST; i++) 
	{
		if (m_ToolStack[i].pszBarName != NULL) delete []m_ToolStack[i].pszBarName;
		m_ToolStack[i].pszBarName = NULL;
	}
	
	if (m_pszCurrentToolName != NULL) delete []m_pszCurrentToolName;
	
	//if (g_pszCurrentFormName != NULL) delete []g_pszCurrentFormName;
		
	if (m_pszBmpBackPath != NULL) 	delete m_pbmpBackground;
	//Destroy();
}

//	fans add
void 	CtmToolBar::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("bmpbackpath",		tmSTR,		&m_pszBmpBackPath);
	SetPropInfo("bmpuppath",		tmSTR,		&m_pszBmpUpPath);
	SetPropInfo("bmpdownpath",		tmSTR,		&m_pszBmpDownPath);
	SetPropInfo("maintool",			tmSTR,		&m_pszMainToolName);
	SetPropInfo("OnCreateA",		tmPROCEDURE,&m_pszOnCreateName);
	SetPropInfo("OnKeyA",			tmPROCEDURE,&m_pszOnKey);
	SetPropInfo("OnKeyAEnd",		tmPROCEDURE,&m_pszOnKeyEnd);
	SetPropInfo("OnMessage",		tmPROCEDURE,	&m_pszOnMessage);	//	fans 2008/6/16 11:32上午
	SetPropInfo("keycolor", 		tmLONG,		&m_dwKeyColor);
}

//	fans add
void 	CtmToolBar::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	SetPropInfo(TOOLBAR_BMPBACKPATH,	tmSTR,		&m_pszBmpBackPath);
	SetPropInfo(TOOLBAR_BMPUPPATH,		tmSTR,		&m_pszBmpUpPath);
	SetPropInfo(TOOLBAR_BMPDOWNPAT,		tmSTR,		&m_pszBmpDownPath);
	SetPropInfo(TOOLBAR_BMPMAINTOOL,	tmSTR,		&m_pszMainToolName);
	SetPropInfo(FORMVIEW_ONCREATEA,		tmPROCEDURE,&m_pszOnCreateName);
	SetPropInfo(TOOLBAR_ONKEYA,			tmPROCEDURE,&m_pszOnKey);
	SetPropInfo(TOOLBAR_ONKEYAEND,		tmPROCEDURE,&m_pszOnKeyEnd);
	SetPropInfo(TOOLBAR_ONMESSAGE,		tmPROCEDURE,	&m_pszOnMessage);	//	fans 2008/6/16 11:32上午
	SetPropInfo(TOOLBAR_KEYCOLOR, 		tmLONG,		&m_dwKeyColor);
}

//	fans add
BOOL	CtmToolBar::CreateA()
{
	CtmWnd::CreateA();
	char	szPath[512];
	
	memset(m_pszBarName,0,sizeof(m_pszBarName));
	memset(m_pszWndName,0,sizeof(m_pszWndName));
	if (m_bBmpButton)						//<<yang 2005/10/21 
	{
		if (m_pszBmpUpPath != NULL) 
		{
			m_pbmpUp.SetKeyColor(m_dwKeyColor);
			m_pbmpUp.Create(m_pszBmpUpPath);
		}
		else 
		{
			sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmTOOLBAR_BUTTONUP);
			m_pbmpUp.SetKeyColor(m_dwKeyColor);
			m_pbmpUp.Create(szPath);
		}
		
		if (m_pszBmpDownPath != NULL)
		{
			m_pbmpDown.SetKeyColor(m_dwKeyColor);
			m_pbmpDown.Create(m_pszBmpDownPath);
		}
		else 
		{
			sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmTOOLBAR_BUTTONDONW);
			m_pbmpDown.SetKeyColor(m_dwKeyColor);
			m_pbmpDown.Create(szPath);
		}
	}
	
	if (m_pszBmpBackPath == NULL) 
		m_pbmpBackground = g_aIniBmp[TOOLBAR_BACKGROUNDID].pIniBmp;
	else
	{
		if(m_pbmpBackground != NULL)delete m_pbmpBackground;
		m_pbmpBackground = new CtmBitMap;
		m_pbmpBackground->Create(m_pszBmpBackPath);
	}
	/*
	else 
	{
		//	創建圖片,但是麻煩的是創建的需要釋放
	}
	*/
	
	//	load Dll
	///*	fansTest
	char	libName[512]; 
	void*	libHandle = NULL;
	sprintf(libName, "%s%s%s", CtmConfig::GetInstance()->GetFormlibPath(), Name, ".so");
	// <FANS> 2006/11/20 09:50上午
	//strcpy(libName, "./formlib/");
	//strcat(libName, Name);
	//strcat(libName, ".so");
	libHandle = dlopen(libName, RTLD_LAZY);	
	
	SetLibHandle(libHandle);
	//*/
	
	
	
	
	CreateButtons(m_pszMainToolName);
	
	return TRUE;
}

//	fans add
void	CtmToolBar::CreateButtons(char* pszFileName, int iViewIndex)
{
	if (m_pszCurrentToolName != NULL)
	{
		delete []m_pszCurrentToolName;
		m_pszCurrentToolName = NULL;
	}
	
	if(pszFileName != NULL)
	{
		m_pszCurrentToolName = new char[strlen(pszFileName) + 1];
		strcpy(m_pszCurrentToolName, pszFileName);
	}
	Clear();
	//========
	//	創建Button
	//========
	
	char	szTemp[256];
	sprintf(szTemp, "%s%s", CtmConfig::GetInstance()->GetToolBarPath(), pszFileName);
	//printf("mainToolBar =%s\n", szTemp);
	
	if (pszFileName != NULL) 
	{
		DeleteAllButton();
		m_parseButton->OpenFile(szTemp);		
		m_parseButton->CloseFile();	
	}
	else
	{
//		ioctl(g_FbIotcol, 0xFFFF12, NULL);
	 	return;
	}
	//	將解析出來的Button設置宿主
	CtmWnd* pwnd = NULL;
	for (int i = 0; i < m_parseButton->GetControlCount(); i++)
	{
		if (m_parseButton->m_aryControls[i].control->GetParent() == NULL)
		{
			pwnd = m_parseButton->m_aryControls[i].control;
			m_parseButton->m_aryControls[i].control->SetParent(this);
			((CtmToolButton*)m_parseButton->m_aryControls[i].control)->SetUpBmp(&m_pbmpUp);
			((CtmToolButton*)m_parseButton->m_aryControls[i].control)->SetDownBmp(&m_pbmpDown);
			//m_parseButton->m_aryControls[i].control->CreateA();
		}
		else
		{
			m_parseButton->m_aryControls[i].control->SetParent(pwnd);	
			
		}
	}
	
	for (int i = 0; i < m_parseButton->GetControlCount(); i++)
	{
		if (m_parseButton->m_aryControls[i].control->GetParent() == this)
			m_parseButton->m_aryControls[i].control->CreateA();
	}
	if (OnCreateA == NULL)
	{
		if (GetLibHandle() != NULL)
		{
			OnCreateA = (BOOL(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszOnCreateName);
			char* pszError;
			pszError = dlerror();	//	需要檢測錯誤
			if (pszError != NULL) OnCreateA = NULL;
		}
	}
	if (OnCreateA != NULL) OnCreateA(this);
		
	
	ShowAllButton();
	//g_nCommand=2;
	//ChangePos(GetClientRECT());
//	ioctl(g_FbIotcol, 0xFFFF12, NULL);
//	usleep(40*1000);
/****************************************************************************+
|           put Key                                                          |
+****************************************************************************/	
	WORD wKey = _NULL_KEY;
	KEYFLAG keyflag;
	while (_TestKeyFlag((KEYFLAG __far*)&keyflag) != _NULL_KEY) wKey =_GetKey();	
	
	//	有可能ToolBar上面還有其他的元件，所以還是要區別對待的
	int j = -1;
	WORD	wButtonKey = _NULL_KEY;
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
		{
			j++;
			if (j == iViewIndex)
			{
				Controls[i]->GetPropValueT("key", &wButtonKey, sizeof(wButtonKey));
				::PutKeyTail(wButtonKey);
				break;
			}
		}
	}
	
	if (wKey != _NULL_KEY) 
	{
		::PutKeyTail(wKey);
	}
}

//	fans add
void	CtmToolBar::ShowAllButton()
{
	//printf("CtmToolBar::ShowAllButton()\n");
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
		{
			int	wPrivilege=0;
			((CtmToolButton*)Controls[i])->GetPropValueT("privilege", &wPrivilege,sizeof(wPrivilege));
			//if(wPrivilege<=g_iPrivilege||(CtmConfig::GetInstance()->GetPrivSwitch()!=1))
			//{
				((CtmToolButton*)Controls[i])->SetFGColor(m_wndattr.fgcToolBar2);
				((CtmToolButton*)Controls[i])->Update();
			//}
		}
	}
	
}


//	fans add
int		CtmToolBar::FindButton(WORD	wKey)
{
	int iResult = -1;
	WORD	wButtonKey = _NULL_KEY;
	for (int i = 0; i < GetControlCount(); i++)
	{
		if(Controls[i]->Is("CtmToolButton"))
		{
			Controls[i]->GetPropValueT("key", &wButtonKey, sizeof(wButtonKey));
			if (wButtonKey == wKey)
			{
				iResult = i;
				break;
			}
		}
	}
	return iResult;
}


//	fans adds
void	CtmToolBar::DeleteAllButton()
{
	
	m_parseButton->m_aryControls[0].control = NULL;
	for (int i = 0; i < MAX_CONTROLS; i++)
	{
		//==========
		//	刪除的是Button,也就是銷毀Button
		//==========
		if (m_bPreControl) 
		{
			i--;
			m_bPreControl = FALSE;
		}
		
		if (Controls[i] != NULL)
		{
			if (Controls[i]->Is("CtmToolButton"))
			{
				delete Controls[i];
				//Controls[i] = NULL;
			}
		}
	}
}


    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/


void        CtmToolBar::Redraw(RECT rect) // Virtual function
{
	Clear();
    
    for (int i = 0; i < MAX_CONTROLS; i++)
	{
		//==========
		//	
		//==========
		if (Controls[i] != NULL)
			if (Controls[i]->Is("CtmToolButton"))
			{
				Controls[i]->DrawBorder();
				Controls[i]->Show();
			}
	}
}

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
WORD        CtmToolBar::OnKey(WORD wKey)
{	
	short       i = 0;
	WORD        wGroup;
	WORD        wAction;
	char        szWndName[256];
	char		szBarNameTemp[256];
	char*		pszLinkBar	 = NULL;
	int			iButtonIndex = 0;
	CtmToolButton* pbutton 	 = NULL;
	int	wPrivilege=0;
	
	//printf("tmToolBar::OnKey wKey=%x this->Name=%x \n", wKey, this->Name);
	//gettimeofday(&_tstart, NULL);
	tmTOOLSTACK	tempStack;
	memset(szWndName,0,sizeof(szWndName)/sizeof(szWndName[0]));
	memset(szBarNameTemp,0,sizeof(szBarNameTemp)/sizeof(szBarNameTemp[0]));
	iButtonIndex = FindButton(wKey);
	if (iButtonIndex >= 0)	
	{
		
		m_nToolIndex = iButtonIndex;		//james add 2011/8/2 下午 05:15:47
		pbutton = (CtmToolButton*)Controls[iButtonIndex];		
		
		//	可以放置外放的方法指針
		///*fansTest
		if (OnToolBarKeyA == NULL)
		{
			if (GetLibHandle() != NULL)
			{
				OnToolBarKeyA = (WORD(*)(CtmToolBar*, CtmButton*, WORD, int))dlsym(GetLibHandle(), m_pszOnKey);
				char* pszError;
				pszError = dlerror();	//	需要檢測錯誤
				if (pszError != NULL) OnToolBarKeyA = NULL;
			}
		}
		//*/
		pbutton->GetPropValueT("privilege", &wPrivilege,sizeof(wPrivilege));
		//if(wPrivilege<=g_iPrivilege||(CtmConfig::GetInstance()->GetPrivSwitch()!=1))
		//{
			pbutton->GetPropValueT("action",	&wAction, 	sizeof(wAction));
			if(wAction == TOOLBAR_SUBTOOLBAR)
			{
				g_bSubToolbarFlag = TRUE;
			}
			else
			{
				memset(m_pszBarName,0,sizeof(m_pszBarName));
				g_bSubToolbarFlag = FALSE;
			}
			
		WORD CheckRetrun=0;
		//}
		if (OnToolBarKeyA != NULL) CheckRetrun=OnToolBarKeyA(this, pbutton, wKey, iButtonIndex);
		
		pbutton->GetPropValueT("privilege", &wPrivilege,sizeof(wPrivilege));
		//if(wPrivilege<=g_iPrivilege||(CtmConfig::GetInstance()->GetPrivSwitch()!=1))
		//{
			pbutton->GetPropValueT("formname", 	szWndName, 	sizeof(szWndName));		
			//printf("formname = %s\n",szWndName);
			pbutton->GetPropValueT("group", 	&wGroup, 	sizeof(wGroup));
			if(CheckRetrun!=0) 
				pbutton->GetPropValueT("action",	&wAction, 	sizeof(wAction));
			else
				wAction=0;
			if(wAction == TOOLBAR_SUBTOOLBAR)
			{
				g_bSubToolbarFlag = TRUE;
				pbutton->GetPropValueT("subbarname", 	szBarNameTemp, sizeof(szBarNameTemp));
				//printf("subbarname = %s\n",szBarNameTemp);
			}
			else
			{
				g_bSubToolbarFlag = FALSE;
				pbutton->GetPropValueT("barname", 	szBarNameTemp, sizeof(szBarNameTemp));
				//printf("barname = %s\n",szBarNameTemp);
			}
			
		//}

		ioctl(g_FbIotcol, 0xFFFF11, 0);
		usleep(30*1000);
		//=========
		//	以下的這句話是需要考慮的
		//=========
		BOOL	bContinue = TRUE;
		if (szWndName[0] == '\0' && szBarNameTemp[0] == '\0') //無動作
		{
			bContinue = FALSE;
		}

		if(szBarNameTemp[0] == '\0')	//無toolbar動作
		{
			if(0==strcmp(szWndName, m_pszWndName))	//over動作相同
				bContinue=FALSE;	
			else
				strcpy(m_pszWndName, szWndName);
		}
		else if(szWndName[0] == '\0')	//無over動作
		{
			if(strcmp(szBarNameTemp, m_pszBarName)==0)	//toolbar動作相同
				bContinue=FALSE;
			else
				strcpy(m_pszBarName,szBarNameTemp);
		}
		if (bContinue)
		{
			if (szBarNameTemp[0] != '\0')	//toolbar動作
			{
				pszLinkBar = new char[strlen(szBarNameTemp) + 1];
				strcpy(pszLinkBar, szBarNameTemp);
			}
			else 
			{

				WORD	wGroupTemp = 0;
				//=======
				//       Process Button visual behavior
				//=======
	//			ioctl(g_FbIotcol, 0xFFFF11, NULL); 
	//			usleep(20*1000);
	
				switch (wGroup)	//判斷按鈕壓下動作  切換toolbar時不需動作
				{
					case 0:
						pbutton->Press(tmBUTTON_DOWNUP);
						break;
					case 1:
						//cout << "wGroup is 1 " << endl;
						for (i = 0; i < GetControlCount(); i++)
						{
							if (Controls[i]->Is("CtmToolButton"))
							{
								Controls[i]->GetPropValueT("privilege", &wPrivilege,sizeof(wPrivilege));
								Controls[i]->GetPropValueT("group", &wGroupTemp, sizeof(wGroupTemp));
								if (wGroupTemp > wGroup)
								{
									//if(wPrivilege<=g_iPrivilege||(CtmConfig::GetInstance()->GetPrivSwitch()!=1))
										((CtmToolButton*)Controls[i])->Press(tmBUTTON_UP);
								}
							}
						}
						pbutton->Press(tmBUTTON_DOWNUP);
						break;
					default:
						
						//FreshDraw(pbutton);// <FANS> 2006/11/14 02:18下午
						for (i = 0; i < GetControlCount(); i++)
						{
							if (Controls[i]->Is("CtmToolButton"))
							{
								Controls[i]->GetPropValueT("privilege", &wPrivilege,sizeof(wPrivilege));
								Controls[i]->GetPropValueT("group", &wGroupTemp, sizeof(wGroupTemp));
								if (wGroupTemp == wGroup)
								{
									//if(wPrivilege<=g_iPrivilege||(CtmConfig::GetInstance()->GetPrivSwitch()!=1))
											((CtmToolButton*)Controls[i])->Press(tmBUTTON_UP);
								}
							}
						}				
						pbutton->Press(tmBUTTON_DOWN);
						break;
				}
			}
//			if(!g_bSubToolbarFlag&&pszLinkBar==NULL)
			//{
//				ioctl(g_FbIotcol, 0xFFFF12, NULL); 
//				usleep(10*1000);
			//}
			//=======
			//       Preset command and action code
			//=======
			
			switch (wAction & 0xFF00)
			{
				case TOOLBAR_NEXT:	
					
					if (pszLinkBar == NULL) return _NULL_KEY;
					memset(m_pszBarName,0,sizeof(m_pszBarName));
					memset(m_pszWndName,0,sizeof(m_pszWndName));
					CreateButtons(pszLinkBar);
					delete []pszLinkBar;
					pszLinkBar = NULL;
					return _NULL_KEY;
				case TOOLBAR_SELECT:
					
					if (pszLinkBar != NULL)
					{
						tempStack.pszBarName = m_pszCurrentToolName;
						tempStack.iViewIndex = iButtonIndex;
						memset(m_pszBarName,0,sizeof(m_pszBarName));
						memset(m_pszWndName,0,sizeof(m_pszWndName));
						PushS(tempStack);
						//ioctl(g_FbIotcol, 0xFFFF11, NULL); 
						//usleep(30*1000);
						CreateButtons(pszLinkBar);						
						delete []pszLinkBar;
						pszLinkBar = NULL;
					}
					return _NULL_KEY;
				case TOOLBAR_RETURN:
					memset(m_pszBarName,0,sizeof(m_pszBarName));
					memset(m_pszWndName,0,sizeof(m_pszWndName));
					tempStack = PopS();
					CreateButtons(tempStack.pszBarName, tempStack.iViewIndex);
					return _NULL_KEY;
				case TOOLBAR_RETURNHOME:
					memset(m_pszBarName,0,sizeof(m_pszBarName));
					memset(m_pszWndName,0,sizeof(m_pszWndName));
					tempStack = PopS();
					CreateButtons(m_pszMainToolName);
					return _NULL_KEY;
				case ID_TOOL_RESET:
					memset(m_pszBarName,0,sizeof(m_pszBarName));
					memset(m_pszWndName,0,sizeof(m_pszWndName));
					m_wSP = 0;
					for (i = 0; i < tmMAX_TOOLNEST; i++) 
					{
						if (m_ToolStack[i].pszBarName != NULL)
						{
							delete []m_ToolStack[i].pszBarName;
							m_ToolStack[i].pszBarName = NULL;
						}
					}
					CreateButtons(m_pszMainToolName);
					return _NULL_KEY;
				case TOOLBAR_SUBTOOLBAR:
					g_bSubToolbarFlag = TRUE;
					if(!g_bStopViewUpdate)
						g_bStopViewUpdate = TRUE;
					if (pszLinkBar == NULL) break;//return _NULL_KEY;
					
					//printf("pszLinkBar = %s\n", pszLinkBar);
					(g_pMainFrame->GetSubToolBar())->SetPropValueT("maintool", pszLinkBar);
					(g_pMainFrame->GetSubToolBar())->CreateA();
					
					delete []pszLinkBar;
					pszLinkBar = NULL;
					
					return _NULL_KEY;
					
					break;
				default:
				
					break;
			}
			//=======
			//       Parse button command
			//=======
			switch	(wAction & 0xFF00)
			{
				case TOOLBAR_DUPL:
					
					break;
				default:
					
					if (szWndName[0] != '\0')
					{
						//if (g_pszCurrentFormName != NULL)
						// cjlee 2019/5/1 下午 01:02:11
//						if (g_pszCurrentFormName[0] != '\0')
//						{
//							if (strcmp(szWndName, g_pszCurrentFormName) != 0)
//							{
//								//delete []g_pszCurrentFormName;
//								//g_pszCurrentFormName = new char[strlen(szWndName) + 1];
//								memset(g_pszCurrentFormName, 0, sizeof(g_pszCurrentFormName));
//								strcpy(g_pszCurrentFormName	, szWndName);
//							}
//							else
//							{
//								g_bStopViewUpdate = FALSE;
//								 break;
//							}
//						}
//						else
//						{
//							//g_pszCurrentFormName = new char[strlen(szWndName) + 1];
//							memset(g_pszCurrentFormName, 0, sizeof(g_pszCurrentFormName));
//							strcpy(g_pszCurrentFormName, szWndName);
//						}
						// cjlee 2019/5/1 下午 01:02:10
						//printf("FormName=%s\n",g_pszCurrentFormName);
//						::PutCommand(g_pszCurrentFormName);
						//g_pMainFrame->GetCurrentView()->OnCommand(g_pszCurrentFormName);
						
					}
					break;
			}
		}
		
//		ioctl(g_FbIotcol, 0xFFFF11,NULL);
		//	可以放置外放的方法指針
		///*fansTest
		if (OnToolBarKeyEnd == NULL)
		{
			if (GetLibHandle() != NULL)
			{
				OnToolBarKeyEnd = (WORD(*)(CtmToolBar*, CtmButton*, WORD, int))dlsym(GetLibHandle(), m_pszOnKeyEnd);
				char* pszError;
				pszError = dlerror();	//	需要檢測錯誤
				if (pszError != NULL) OnToolBarKeyEnd = NULL;
			}
		}
		if (OnToolBarKeyEnd != NULL) OnToolBarKeyEnd(this, pbutton, wKey, iButtonIndex);
		//*/
	}
	if (pszLinkBar != NULL) delete []pszLinkBar;
	return wKey;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/


void        CtmToolBar::Clear()
{
	RECT rect;
	if(m_pszCurrentToolName == NULL)
	{
		MKRECT(rect, 0, 0, 0, 0)
	}
	else
	{
		MKRECT(rect, 0, 0, GetClientWidth(), GetClientHeight())
		_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
		//党淏toolbar匢佶
		CtmGDIDoubleBuffer*	pDoubleBuffer=new CtmGDIDoubleBuffer(m_rectClient);
		m_pbmpBackground->SetDoubleBuffer( pDoubleBuffer);
		m_pbmpBackground->Show(rect, 0);
		if(pDoubleBuffer!=NULL)
		{
			delete 	pDoubleBuffer;
		}
	}
	
}

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


void		CtmToolBar::PushS(tmTOOLSTACK	toolstack)
{
	if (m_wSP < tmMAX_TOOLNEST)
	{
		if (m_ToolStack[m_wSP].pszBarName != NULL) delete []m_ToolStack[m_wSP].pszBarName;
		m_ToolStack[m_wSP].pszBarName = new char[strlen(toolstack.pszBarName) + 1];
		m_ToolStack[m_wSP].iViewIndex = toolstack.iViewIndex;
		strcpy(m_ToolStack[m_wSP++].pszBarName, toolstack.pszBarName);
		
	}
}
tmTOOLSTACK		CtmToolBar::PopS()
{
	tmTOOLSTACK	result;
	result.pszBarName = NULL;
	result.iViewIndex = 0;
	if (m_wSP > 0)
	{		
		result.pszBarName = new char[strlen(m_ToolStack[--m_wSP].pszBarName) + 1];
		result.iViewIndex = m_ToolStack[m_wSP].iViewIndex;
		strcpy(result.pszBarName, m_ToolStack[m_wSP].pszBarName);
		//cout << "Pop toolbar is " << result.pszBarName << endl;
		delete []m_ToolStack[m_wSP].pszBarName;
		m_ToolStack[m_wSP].pszBarName = NULL;
		
	}
	return result;
}

void		CtmToolBar::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	
	switch(message)
    {
        case MSG_MOUSELUP:
            MouseClick(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }
	
	
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

BOOL 	CtmToolBar::MouseClick(short x_pos,short y_pos,int key_flag)
{
		//printf("CtmToolBar::MouseClick\n");
    int     i;
    RECT    rect;
    WORD    wKey;

    rect    = GetWindowRECT();
    if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
        return 1;
   for (i = 0; i < GetControlCount(); i++)
	{
	    rect    = Controls[i]->GetWindowRECT();
	    if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
	        continue;
	    else
	    {
	    	if(!Controls[i]->Is("CtmToolButton"))
	    	{
	    		//printf("Haha, it's not toolbutton....\n");
	    		continue;
	    	}	
	    	else       
	    		break;
	    }
	}
	if(i ==  GetControlCount())  return 0;
	
	int Controls_privilege = -1;
	Controls[i]->GetPropValueT("privilege", &Controls_privilege, sizeof(Controls_privilege));
	//printf("g_iPrivilege=%d   Controls_privilege=%d\n",g_iPrivilege,Controls_privilege);
	if(g_iPrivilege < Controls_privilege) 
	{
		Prompt(g_MultiLanguage["ROBOT_STR_PRIVILEGE_NO_ENOUGH"],0);
		return 0;
	}
	//add by J.Wong 2016/6/8 15:40:29
	if(!Controls[i]->Is("CtmToolButton"))	return 0;
	wKey    = *(WORD*)Controls[i]->GetPropValue("key");
 	OnKey(wKey);
	return 0;
}




void	CtmToolBar::FreshDraw(CtmToolButton* pbtn)
{
	const	int dif = 10;
	
	int index = -1;
	short left,top,right,bottom;
	short leftA,topA,rightA,bottomA;
	left = top = right = bottom = 0;
	
	leftA = topA = rightA = bottomA = 0;
	Clear();			//	清除原有的Toolbar
	
	//	得出焦點Button的序列號
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
			if (Controls[i] == pbtn)
			{
				index = i;
				break;
			}
	}
	
	
	//	將所有的Button坐標以及大小進行變換
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
		{
		//	//cout << "ToolButton is " << Controls[i]->Name << endl;
			Controls[i]->GetPropValueT("left",	&left, 	sizeof(left));
			Controls[i]->GetPropValueT("top", 	&top, 	sizeof(top));
			Controls[i]->GetPropValueT("right", 	&right, sizeof(right));
			Controls[i]->GetPropValueT("bottom", &bottom,sizeof(bottom));
			/*
			//cout << "Left  is " << left << ";"				 
				 << "right  is " << right << endl;
			*/
				 
			if (Controls[i] != pbtn)
			{
				//if (i > index) 
				if(*(short*)Controls[i]->GetPropValue("left") > *(short*)pbtn->GetPropValue("left"))
				{
					leftA = left + dif - (i - index);
					rightA = right + dif - (i - index);
				}
				else 
				{
					leftA =  left - dif + (index - i);
					rightA = right - dif + (index - i);
				}
				
												
				if (leftA <= 0) leftA = 0;
				if (rightA >= GetmClientRight()) rightA = GetmClientRight();
				
				Controls[i]->SetPropValueT("left", leftA);
				Controls[i]->SetPropValueT("right", rightA);				
			}
			else 
			{
				leftA = left - dif;
				rightA = right + dif;
				topA = top - 5;
				bottomA = bottom + 5;
				
				if (leftA <= 0) leftA = 0;
				if (rightA >= GetmClientRight()) rightA = GetmClientRight();
				
				Controls[i]->SetPropValueT("left", leftA);
				Controls[i]->SetPropValueT("right", rightA);
				Controls[i]->SetPropValueT("top", topA);
				Controls[i]->SetPropValueT("bottom", bottomA);					
			}
			
			
			//	需要重新顯示
			Controls[i]->CreateA();
			((CtmToolButton*)Controls[i])->SetFGColor(m_wndattr.fgcToolBar2);
			((CtmToolButton*)Controls[i])->Update();
			
			/*
			//cout << "LeftA is " << leftA << ";"				 
				 << "rightA is " << rightA << endl;
			*/
				 				 
			
			//	將坐標復原和大小復原
			Controls[i]->SetPropValueT("left", left);
			Controls[i]->SetPropValueT("right", right);
			Controls[i]->SetPropValueT("top", top);
			Controls[i]->SetPropValueT("bottom", bottom);
			
		}
	}
	
}


CtmSubToolBar::CtmSubToolBar() : CtmToolBar()
{
	
}


CtmSubToolBar::~CtmSubToolBar()
{
	
}


BOOL	CtmSubToolBar::CreateA()
{
	//printf("BOOL	CtmSubToolBar::CreateA()\n");
	
//	ioctl(g_FbIotcol, 0xFFFF11, NULL);
//	usleep(10*1000); 
	CtmWnd::CreateA();
	char	szPath[512];
	
	if (m_bBmpButton)						//<<yang 2005/10/21 
	{
		if (m_pszBmpUpPath != NULL) 
		{
			m_pbmpUp.SetKeyColor(m_dwKeyColor);
			m_pbmpUp.Create(m_pszBmpUpPath);
		}
		else 
		{
			sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), "SubButtonUp.bmp");
			m_pbmpUp.SetKeyColor(m_dwKeyColor);
			m_pbmpUp.Create(szPath);
		}
		
		if (m_pszBmpDownPath != NULL)
		{
			m_pbmpDown.SetKeyColor(m_dwKeyColor);
			m_pbmpDown.Create(m_pszBmpDownPath);
		}
		else 
		{
			sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), "SubButtonDown.bmp");
			m_pbmpDown.SetKeyColor(m_dwKeyColor);
			m_pbmpDown.Create(szPath);
		}
	}
	
	if (m_pszBmpBackPath == NULL) 
		m_pbmpBackground = g_aIniBmp[SUBTOOLBAR_BACKGROUNDID].pIniBmp;
	else
	{
		if(m_pbmpBackground != NULL)delete m_pbmpBackground;
		m_pbmpBackground = new CtmBitMap;
		m_pbmpBackground->Create(m_pszBmpBackPath);
	}
	/*
	else 
	{
		//	創建圖片,但是麻煩的是創建的需要釋放
	}
	*/
	
	//	load Dll
	///*	fansTest
	char	libName[512]; 
	void*	libHandle = NULL;
	sprintf(libName, "%s%s%s", CtmConfig::GetInstance()->GetFormlibPath(), "maintoolbar", ".so");
	// <FANS> 2006/11/20 09:50上午
	//strcpy(libName, "./formlib/");
	//strcat(libName, Name);
	//strcat(libName, ".so");
	libHandle = dlopen(libName, RTLD_LAZY);	
	
	SetLibHandle(libHandle);
	//*/
	
	
	
	//printf("m_pszMainToolName = %p\n", m_pszMainToolName);
	if(m_pszMainToolName)
		CreateButtons(m_pszMainToolName);
//	ioctl(g_FbIotcol, 0xFFFF12, NULL);
//	usleep(40*1000);
	return TRUE;
}

void	CtmSubToolBar::CreateButtons(char* pszFileName, int iViewIndex)
{
	//g_nCommand=1;
	if (m_pszCurrentToolName != NULL)
	{
		delete []m_pszCurrentToolName;
		m_pszCurrentToolName = NULL;
	}
	
	if(pszFileName != NULL)
	{
		m_pszCurrentToolName = new char[strlen(pszFileName) + 1];
		strcpy(m_pszCurrentToolName, pszFileName);
	}
	else
	{
		g_pMainFrame->GetCurrentView()->SetPropValueT("right", m_rectClient.right);
		g_pMainFrame->GetCurrentView()->SetPropValueT("bottom", m_rectClient.bottom);
		g_pMainFrame->GetCurrentView()->CreateA();
	}
	Clear();
	//========
	//	創建Button
	//========
	
	char	szTemp[256];
	sprintf(szTemp, "%s%s", CtmConfig::GetInstance()->GetToolBarPath(), pszFileName);
	//printf("mainToolBar =%s\n", szTemp);
	
	if (pszFileName != NULL) 
	{
		DeleteAllButton();
		m_parseButton->OpenFile(szTemp);		
		m_parseButton->CloseFile();	
	}
	else
	{
//	 	ioctl(g_FbIotcol, 0xFFFF12, NULL);
	 	return;
	}
	//	將解析出來的Button設置宿主
	CtmWnd* pwnd = NULL;
	for (int i = 0; i < m_parseButton->GetControlCount(); i++)
	{
		if (m_parseButton->m_aryControls[i].control->GetParent() == NULL)
		{
			pwnd = m_parseButton->m_aryControls[i].control;
			m_parseButton->m_aryControls[i].control->SetParent(this);
			((CtmToolButton*)m_parseButton->m_aryControls[i].control)->SetUpBmp(&m_pbmpUp);
			((CtmToolButton*)m_parseButton->m_aryControls[i].control)->SetDownBmp(&m_pbmpDown);
			//m_parseButton->m_aryControls[i].control->CreateA();
		}
		else
		{
			m_parseButton->m_aryControls[i].control->SetParent(pwnd);	
			
		}
	}
	
	for (int i = 0; i < m_parseButton->GetControlCount(); i++)
	{
		if (m_parseButton->m_aryControls[i].control->GetParent() == this)
			m_parseButton->m_aryControls[i].control->CreateA();
	}
	if (OnCreateA == NULL)
	{
		if (GetLibHandle() != NULL)
		{
			OnCreateA = (BOOL(*)(CtmWnd* pwnd))dlsym(GetLibHandle(), m_pszOnCreateName);
			char* pszError;
			pszError = dlerror();	//	需要檢測錯誤
			if (pszError != NULL) OnCreateA = NULL;
		}
	}
	if (OnCreateA != NULL) OnCreateA(this);
		
	
	ShowAllButton();
	//g_nCommand=3;
	//ChangePos(GetClientRECT());
	
/****************************************************************************+
|           put Key                                                          |
+****************************************************************************/	
	WORD wKey = _NULL_KEY;
	KEYFLAG keyflag;
	while (_TestKeyFlag((KEYFLAG __far*)&keyflag) != _NULL_KEY) wKey =_GetKey();	
	
	int j = -1;
	WORD	wButtonKey = _NULL_KEY;
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
		{
			j++;
			if (j == iViewIndex)
			{
				Controls[i]->GetPropValueT("key", &wButtonKey, sizeof(wButtonKey));
				//::PutKeyTail(wButtonKey);
				OnKey(wButtonKey);
				break;
			}
		}
	}
	
	if (wKey != _NULL_KEY) 
		{
		//::PutKeyTail(wKey);
		OnKey(wKey);
		}
}

BOOL 	CtmSubToolBar::MouseClick(short x_pos,short y_pos,int key_flag)
{
	if(!g_bSubToolbarFlag) return 0;
	int     i;
    RECT    rect;
    WORD    wKey;

    rect    = GetWindowRECT();
    
    if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
        return 1;
    
     for (i = 0; i < GetControlCount(); i++)
	{
	    rect    = Controls[i]->GetWindowRECT();
	    if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
	        continue;
	    else
	    {
	    	if(!Controls[i]->Is("CtmToolButton"))
	    	{
	    		//printf("Haha, it's not toolbutton....\n");
	    		continue;
	    	}	
	    	else       
	    		break;
	    }
	}
	if(i ==  GetControlCount())  return 0;
	//add by J.Wong 2016/6/8 15:40:29
	if(!Controls[i]->Is("CtmToolButton"))	return 0;
	wKey    = *(WORD*)Controls[i]->GetPropValue("key");
    OnKey(wKey);
    return 0;
}
