/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : MainFrm                                                        |
|  Task     : Main Frame                                                     |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"mainfrm.h"
#include	"utils.h"
#include	"init.h"
#include	"parsepule.h"
#include	"moldControl.h"
#include	"commonaction.h"
#include 	"drawtext.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
const int PROMPTBAR_INDEX = 0;
const int VIEW_INDEX = 1;
const int STATUSBAR_INDEX = 2;
const int TOOLBAR_INDEX = 3;
const int TITLEBMP_INDEX =4;
const int TITLE_INDEX =5;
const int SUBTOOLBAR_INDEX =6;
const int VIEW_INDEX2 =7;
CtmWnd*		pwndButtonPrivi = NULL;
CtmWnd*		pwndPriviNum	= NULL;
CtmWnd*		pwndSaveScreen = NULL;
CtmWnd*		pwndBtnServo = NULL;
CtmWnd*		pwndBtnUser = NULL;
struct timeval _atend;  
IMPLEMENT_DYNCREATE(CtmMainFrame, CtmWnd)

extern int			g_nSetView_StartX;
extern int			g_nSetView_EndX;
extern int			g_nSetView_StartY;
extern int			g_nSetView_EndY;
/*===========================================================================+
|           Class implementation - MainFrame                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmMainFrame::CtmMainFrame() : CtmWnd()
{
	strcpy(TYPEName, "CtmMainFrame");
	m_pwndTool   = NULL;
	m_pwndSubTool= NULL;
	m_pwndStatus = NULL;
	m_pwndView = NULL;
	m_pwndView2 = NULL;
	m_pszToolBar = NULL;
	memset(&m_RectPriv, 0, sizeof(m_RectPriv));
}

CtmMainFrame::~CtmMainFrame()
{
	

}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : BOOL Create(WND* pwnd)                                         |
|  Task     : Create the main frame window                                   |
+----------------------------------------------------------------------------+
|  Call     : bStatus = CMainFrame::Create(pwnd)                             |
|                                                                            |
|  Parameter: pwnd                      - Pointer to WND structure           |
|                                                                            |
|  Return   : bStatus                   - Status                             |
|                                         TRUE  : Success                    |
|                                         FALSE : Fail                       |
+---------------------------------------------------------------------------*/
BOOL        CtmMainFrame::Create(tmWND* pwnd)
{

	
	CtmWnd::Create(pwnd);

	
	
	return TRUE;
}


//	fans add
BOOL        CtmMainFrame::CreateA()
{
	
	RegionUpdateStop = TRUE;
	


	//	mainframe的前4個元件是一定的


	//==========
	//	0: promptbar;1:view;2:statusbar;3:toolbar
	//==========
	
	//	index 0               
	m_pwndPrompt = (CtmPromptBar*)GetControlA(PROMPTBAR_INDEX);
	//	index 1
	m_pwndView = (CtmView*)GetControlA(VIEW_INDEX);
	//printf("m_pwndView=%x \n",m_pwndView );
	m_pwndView2 = (CtmView*)GetControlA(VIEW_INDEX2);
	//printf("m_pwndView2=%x \n",m_pwndView2 );
	//m_pwndView->SetFGColor(tmVIEW_FGCOLOR);
	//m_pwndView->SetBGColor(m_wndattr.bgc);
	//m_pwndView->OnGetFocus();	
	//	index 2
	m_pwndStatus = (CtmStatusBar*)GetControlA(STATUSBAR_INDEX);
			
	//	index 3
	m_pwndTool = (CtmToolBar*)GetControlA(TOOLBAR_INDEX);
	
	m_pwndBmpTitle=GetControlA(TITLEBMP_INDEX);
	m_pwndSubTool = (CtmSubToolBar*)GetControlA(SUBTOOLBAR_INDEX);
	
	CtmWnd::CreateA();
	CtmWnd* pwnd = NULL;
	pwnd = FindControlFromName("StaticTitle");
	
	if (pwnd != NULL)
		pwnd->GetPropValueT("left", &m_nTitlePos, sizeof(m_nTitlePos));
	if(pwnd != NULL)
	{
		pwndButtonPrivi = pwnd->FindControlFromName("buttonPrivilege");
		
		if(pwndButtonPrivi != NULL)
    		m_RectPriv = pwndButtonPrivi->GetWindowRECT();
	}
	if(m_pwndStatus != NULL)
	{
		pwndButtonPrivi = m_pwndStatus->FindControlFromName("buttonPrivilege");
		
		if(pwndButtonPrivi != NULL)
    		m_RectPriv = pwndButtonPrivi->GetWindowRECT();
	}
	if(pwndButtonPrivi==NULL)
	{
		if(m_pwndBmpTitle != NULL)
		{
			pwndButtonPrivi = m_pwndBmpTitle->FindControlFromName("buttonPrivilege");
			
			if(pwndButtonPrivi != NULL)
	    		m_RectPriv = pwndButtonPrivi->GetWindowRECT();
		}
	}
	pwndSaveScreen	= m_pwndStatus->FindControlFromName("SaveScreenBtn");
	pwndBtnUser	= m_pwndStatus->FindControlFromName("UserLoginBtn");
	pwndBtnServo		= m_pwndStatus->FindControlFromName("ServoBtn");

	
	return TRUE;
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
void        CtmMainFrame::Update()
{
	GetControlA(STATUSBAR_INDEX)->Update();
	m_pwndView->Update();
	if(m_pwndView2 != NULL)
		m_pwndView2->Update();
}


void        CtmMainFrame::Redraw(RECT rect)
{

	SetBGColor(m_wndattr.bgc);
	RECT rectX;
	if (Controls[TITLE_INDEX] != NULL)
		if (Controls[TITLE_INDEX]->Is("CtmStaticX2"))
			Controls[TITLE_INDEX]->Update();

	
	rectX = m_pwndView->GetWindowRECT();
	if (!(((rect.left > rectX.right) || (rect.right < rectX.left)) ||
      ((rect.top > rectX.bottom) || (rect.bottom < rectX.top))))
    {
		m_pwndView->DrawBorder();
		m_pwndView->Redraw(rect);
    }

	rectX = m_pwndTool->GetWindowRECT();
	if (!(((rect.left > rectX.right) || (rect.right < rectX.left)) ||
      ((rect.top > rectX.bottom) || (rect.bottom < rectX.top))))
    {
		m_pwndTool->DrawBorder();
		m_pwndTool->Redraw(rect);
	}
	
	rectX =m_pwndPrompt->GetWindowRECT();
	if (!(((rect.left > rectX.right) || (rect.right < rectX.left)) ||
      ((rect.top > rectX.bottom) || (rect.bottom < rectX.top))))
    {
		m_pwndPrompt->DrawBorder();
		m_pwndPrompt->Redraw(rect);
	}
}

void        CtmMainFrame::OnGetFocus()
{
	m_pwndView->OnGetFocus();
}

void        CtmMainFrame::OnLoseFocus()
{
	m_pwndView->OnLoseFocus();
}

WORD        CtmMainFrame::OnKey(WORD wKey)
{
	char pszMsg[4];	
	
	//printf("CtmMainFrame::OnKey wKey=%x \n", wKey);
	switch (wKey)
	{
				case _INSERT:
				case _EXT_INSERT:
					Prompt(pszMsg);
					break;				
				case _KEY_LANGUAGE:
				//case 0x1910:
				//case 0x61:
					{
						char	pszCurrentToolName[256], pszCurrentFormName[256];
						int languageindex = 0;
						int LanguageNum  =0;
						languageindex = CtmConfig::GetInstance()->GetCurrentIndex();
				    	LanguageNum = CtmConfig::GetInstance()->GetLanguageNum();
				    	if((languageindex < 0)||(languageindex > LanguageNum-2))	languageindex = 0;
						else languageindex++;
						CtmConfig::GetInstance()->SetCurrentIndex(languageindex);
						//cout << "languageindex:" <<languageindex<< endl;
						
						memset(pszCurrentToolName, 0, sizeof(pszCurrentToolName));
						memset(pszCurrentFormName, 0, sizeof(pszCurrentFormName));
						
						if(g_pParseXMLRecord)
						{
							delete g_pParseXMLRecord;
							g_pParseXMLRecord=NULL;
						}
						if(g_pParseXMLRecord==NULL)
						{
							char*	pPath = CtmConfig::GetInstance()->GetLanguagepath();
							char*	pFileName = CtmConfig::GetInstance()->GetRecordXMLFileName();
							char	pszFilePath[256];
							memset(pszFilePath, 0, sizeof(pszFilePath));
							sprintf(pszFilePath, "%s%s", pPath, pFileName);
							g_pParseXMLRecord = new CtmParseXMLRecord();
							//printf("1:pszFilePath=%s\n",pszFilePath);
							g_pParseXMLRecord->LoadXMLData(pszFilePath);
						}
						if(g_pParseXMLViewStr)
						{
							delete 	g_pParseXMLViewStr;
							g_pParseXMLViewStr=NULL;
						}
						if(g_pParseXMLViewStr == NULL)
						{
							char*	pPath = CtmConfig::GetInstance()->GetLanguagepath();
							char*	pFileName = CtmConfig::GetInstance()->GetViewXMLFileName();
							char	pszFilePath[256];
							memset(pszFilePath, 0, sizeof(pszFilePath));
							sprintf(pszFilePath, "%s%s", pPath, pFileName);
							//printf("2:pszFilePath=%s\n",pszFilePath);
							g_pParseXMLViewStr = new CtmParseXMLRecord();
							g_pParseXMLViewStr->LoadXMLData(pszFilePath);
						}
						if(g_pDrawText!=NULL)	
						{
							delete 	g_pDrawText;
							g_pDrawText=NULL;
						}
						if(g_pDrawText == NULL)
						{
							char* pPath	= CtmConfig::GetInstance()->GetLanguagepath();
							char* pFTFileName = CtmConfig::GetInstance()->GetFontFTFileName();
							char	pszFilePath[256];
							if(access("/usr/local/fonts", F_OK)==0)
							{
								sprintf(pszFilePath, "/usr/local/fonts/%s", pFTFileName);
								if(pFTFileName != NULL)
								{
									if(access(pszFilePath, F_OK)==0)
									{
										;
									}else{
										memset(pszFilePath, 0,sizeof(pszFilePath));
										sprintf(pszFilePath, "%s%s", pPath, pFTFileName);
									}
								}
							}else{
								memset(pszFilePath, 0,sizeof(pszFilePath));
								sprintf(pszFilePath, "%s%s", pPath, pFTFileName);
							}
							g_pDrawText = new CtmDrawText(pszFilePath);	
							g_pDrawText->SetEnCoding(FT_ENCODING_UNICODE);
						}
			    		g_pInit->Init_System();
			    		SendMsg(MSG_WND_UPDATEVIEW, 0,0, NULL);
						if(m_pwndTool->m_pszCurrentToolName != NULL)
							strcpy(pszCurrentToolName, m_pwndTool->m_pszCurrentToolName);
						if(m_pwndTool->m_pszCurrentFormName != NULL)
							strcpy(pszCurrentFormName, m_pwndTool->m_pszCurrentFormName);
						
						//printf("pszCurrentToolName=%s pszCurrentFormName=%s Index=%d \n", 
						//pszCurrentToolName, pszCurrentFormName, m_pwndTool->m_nToolIndex);
						m_pwndView->ClearOldName();
						m_pwndTool->CreateButtons(pszCurrentToolName, m_pwndTool->m_nToolIndex);
						//m_pwndView->OnCommand(pszCurrentFormName);
						m_pwndStatus->Update();
				    	//::Reset();
					}
					break;
				default:
//					char test[30];
						//extern BOOL g_bSubToolbarFlag;
						g_nSetView_StartX = this->GetWindowRECT().left;
						g_nSetView_StartY = this->GetWindowRECT().top;
						g_nSetView_EndX = this->GetWindowRECT().right;
						g_nSetView_EndY = this->GetWindowRECT().bottom;
					//if ((m_pwndTool != NULL) && (!m_pwndTool->OnKey(wKey)))	return _NULL_KEY;
					if((m_pwndTool != NULL) && (!m_pwndTool->OnKey(wKey))) return _NULL_KEY;
					int ret=m_pwndView->OnKey(wKey);   // Route the key to View if not in Menu or ToolBar
					//g_bSubToolbarFlag=FALSE;
					return ret;
			}
	
	return _NULL_KEY;
}

//	tong add
WORD        CtmMainFrame::OnCommand(char* pszCmd)
{
	return m_pwndView->OnCommand(pszCmd);               // Route the command to View
}


WORD        CtmMainFrame::OnCommand(WORD wCmd)
{
	return m_pwndView->OnCommand(wCmd);               // Route the command to View
}

WORD        CtmMainFrame::OnChange(WORD wIDControl)
{
	return m_pwndView->OnChange(wIDControl);          // Route the event to View
}

BOOL		CtmMainFrame::MouseDown(short x_pos,short y_pos,int key_flag)
{
	RECT    rect;
    
	if(pwndButtonPrivi != NULL)
	{
		rect = pwndButtonPrivi->GetWindowRECT();   
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
			((CtmButton*)pwndButtonPrivi)->Press(tmBUTTON_DOWN);
	}
	if(pwndSaveScreen != NULL)
	{
		rect = pwndSaveScreen->GetWindowRECT();   
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
			((CtmButton*)pwndSaveScreen)->Press(tmBUTTON_DOWN);
	}
	if(pwndBtnUser != NULL)
	{
		rect = pwndBtnUser->GetWindowRECT();   
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
			((CtmButton*)pwndBtnUser)->Press(tmBUTTON_DOWN);
	}
	if(pwndBtnServo != NULL)
	{
		rect = pwndBtnServo->GetWindowRECT();   
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
			((CtmButton*)pwndBtnServo)->Press(tmBUTTON_DOWN);
	}
	return TRUE;
}

BOOL			CtmMainFrame::MouseUp(short x_pos,short y_pos,int key_flag)
{
    RECT    rect;
    if(pwndButtonPrivi != NULL)
    { 
    	rect = pwndButtonPrivi->GetWindowRECT();  
    	if((x_pos<m_RectPriv.right && x_pos>m_RectPriv.left)&& (y_pos<m_RectPriv.bottom && y_pos>m_RectPriv.top))
    	{
 
			((CtmButton*)pwndButtonPrivi)->Press(tmBUTTON_UP);
			Login();
		}
	}
	if(pwndSaveScreen != NULL)
	{ 
		((CtmButton*)pwndSaveScreen)->Press(tmBUTTON_UP);
		rect = pwndSaveScreen->GetWindowRECT();  
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
			SendMsg(MSG_KEY,0x6300,0,NULL);
	}
	if(pwndBtnUser != NULL)
	{ 
		((CtmButton*)pwndBtnUser)->Press(tmBUTTON_UP);
		rect = pwndBtnUser->GetWindowRECT();  
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
			MsgBoxCall("msgboxUserLogin.txt");
	}
	if(pwndBtnServo != NULL)
	{ 
    	//((CtmButton*)pwndBtnServo)->Press(tmBUTTON_UP); // 以StateBar來彈起
		rect = pwndBtnServo->GetWindowRECT();  
		if((x_pos<rect.right && x_pos>rect.left)&& (y_pos<rect.bottom && y_pos>rect.top))
				;
	}
	return TRUE;
}
void		CtmMainFrame::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
       case MSG_MOUSELUP:
       		MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
       		break;
       case MSG_MOUSELDOWN:
       		MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_WND_UPDATEVIEW:
        	UpdateMainFrom();
        	break;
        default:
        	break;
    }
	for (int i = 0; i < m_controlCount; i++)
		Controls[i]->WndProc(message, wParam, lParam);
}


void		CtmMainFrame::UpdateMainFrom()
{
	char	pszCurrentToolName[256], pszCurrentFormName[256];
	memset(pszCurrentToolName, 0, sizeof(pszCurrentToolName));
	memset(pszCurrentFormName, 0, sizeof(pszCurrentFormName));
	if(m_pwndTool->m_pszCurrentToolName != NULL)
		strcpy(pszCurrentToolName, m_pwndTool->m_pszCurrentToolName);
	if(m_pwndTool->m_pszCurrentFormName != NULL)
		strcpy(pszCurrentFormName, m_pwndTool->m_pszCurrentFormName);
	
	//m_pwndView->OnCommand(pszCurrentFormName);
	m_pwndStatus->Update();
	m_pwndStatus->Show();
	
	m_pwndView->ClearOldName();
	m_pwndTool->CreateButtons(pszCurrentToolName, m_pwndTool->m_nToolIndex);	
}

void 		CtmMainFrame::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	SetPropInfo("toolbar", 	tmSTR,		&m_pszToolBar);
}

void 		CtmMainFrame::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	
	SetPropInfo(MAINFRM_TOOLBAR, 	tmSTR,		&m_pszToolBar);
}

void    CtmMainFrame::SetTitle(char* pszTitle)
{
	//printf("SetTitle1\n");
	short left = 0;
	short right =0;
	short oldleft = 0;
	short oldright =0;
	short showleft = 0;
	short showright = 0;
	int   nTitleLen;
	CtmWnd* pwnd = NULL;
	
	if (pszTitle == NULL) return;
	pwnd = FindControlFromName("Bmptitle");
	
	
	if ((pwnd != NULL) && (pwnd->Is("CtmImageBox")))
	{
		//pwnd->GetPropValueT("left", &showleft, sizeof(showleft));
		//pwnd->GetPropValueT("right", &showright, sizeof(showright));			
		pwnd->Show();
	}
	else
	{
		pwnd = FindControlFromName("statusbar0");
		//if (pwnd != NULL) pwnd->GetPropValueT("right", &showleft, sizeof(showleft));
		//else showleft =0;
		//this->GetPropValueT("right", &showright, sizeof(showright));
	}
	
	//printf("left = %d, right = %d\n", showleft, showright);
	pwnd = FindControlFromName("StaticTitle");//Controls[TITLE_INDEX];
	//printf("pwnd=%d\n",pwnd);
	if (pwnd != NULL)
	{
		if (pwnd->Is("CtmStaticX2"))
		{
			char	szTile[256];
			if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)       //netdigger 2007/4/10
			{
    			if (pszTitle != NULL)
    				sprintf(szTile, "%s", pszTitle);
    					
    			else szTile[0] = '\0';
    			nTitleLen   = strlen(pszTitle) * tmDOTFONT_16 >> 2;
		    }
		    else
		    {
		        unsigned short *pwSrc = NULL, *pwDst = (unsigned short *)szTile;
		        char           *pszSrc = NULL; 
		        tmSIZE        size;   
		        
//		        nTitleLen   = 0;   
		        if(pszTitle != NULL)
		        {
		            pwSrc   = (unsigned short *)pszTitle;
		            while(*pwSrc != 0)
		            {
		                *pwDst  = *pwSrc;
		                pwDst ++;
		                pwSrc ++;
//		                nTitleLen += 2;
		            }
		            //pwDst++;	            
		            *pwDst   = 0;
		        }
		        else    pwDst  = 0;
		           
		    }
		    pwnd->SetPropValueT("text", szTile);
		    tmPARAGRAPH paragraph;
		    paragraph.wAlign = tmPARA_ALIGN_CENTER;
		    pwnd->SetParagraph(paragraph);
			pwnd->Update();						
		}
	}
	if(pwndPriviNum != NULL)
	{
		pwndPriviNum->SetPropValueT("value", g_iPrivilege);
		pwndPriviNum->Update();
	}
	
}	
void    CtmMainFrame::SetTitle2(char* pszTitle)
{
	//printf("SetTitle2\n");
	CtmWnd* pwnd2 = NULL;
	pwnd2 = FindControlFromName("StaticTitle2");
	//printf("pwnd2=%d\n",pwnd2);
	if(pwnd2 != NULL)
	{
		//printf("pwnd2 exist\n");
		//James add 2008/4/15 06:00下午  將畫面名稱和摸具名稱分開處理
		if (pwnd2->Is("CtmStaticX2"))
		{
			char	szTile[128];
			if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)       //netdigger 2007/4/10
			{
    		sprintf(szTile, "--%s",pszTitle/*CtmMoldSet::GetInstance()->GetCurrentMold()*/);
		  }
		  else
		  {
		  	unsigned short *pwSrc, *pwDst = (unsigned short *)szTile;
		  	char           *pszSrc; 
		  	tmSIZE        size;   
		      
		  	pszSrc  = pszTitle;
	    	while(*pszSrc != 0)
	    	{
	    		*pwDst   = *pszSrc;
	      	pwDst ++;
	      	pszSrc ++;
	    	}
	            *pwDst   = 0;
			}
			//printf("szTile=%s\n",szTile);
			pwnd2->SetPropValueT("text", szTile);
			pwnd2->Update();						
		}	
	}
}
void    CtmMainFrame::SetUserID(char* pszUserID)
{
	printf("SetUserID\n");
	CtmWnd* pwndUserID = NULL;
	pwndUserID = FindControlFromName("StrUserID");
	if(pwndUserID != NULL)
	{
		printf("pwndUserID exist\n");
		if (pwndUserID->Is("CtmStaticX2"))
		{
			char	szTile[128];
			if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)       //netdigger 2007/4/10
			{
    		sprintf(szTile, "%s",pszUserID);
		  }
		  else
		  {
		  	unsigned short *pwSrc, *pwDst = (unsigned short *)szTile;
		  	char           *pszSrc; 
		  	tmSIZE        size;   
		      
		  	pszSrc  = pszUserID;
	    	while(*pszSrc != 0)
	    	{
	    		*pwDst   = *pszSrc;
	      	pwDst ++;
	      	pszSrc ++;
	    	}
	            *pwDst   = 0;
			}
			printf("pszUserID=%s\n",pszUserID);
			pwndUserID->SetPropValueT("text", szTile);
			pwndUserID->Update();						
		}	
	}
}


RECT	CtmMainFrame::GetViewRect()
{
	RECT	rect;
	if(m_pwndView != NULL)
	{
		m_pwndView->GetPropValueT("left", 	&rect.left, 	sizeof(rect.left));
		m_pwndView->GetPropValueT("right", 	&rect.right, 	sizeof(rect.right));
		m_pwndView->GetPropValueT("top", 	&rect.top, 		sizeof(rect.top));
		m_pwndView->GetPropValueT("bottom", &rect.bottom, 	sizeof(rect.bottom));
	}
	return rect;
}
