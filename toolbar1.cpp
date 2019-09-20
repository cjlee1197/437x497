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

/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
const	int		EVENT_ONTOOLBAR_KEYSTART_NAME_DEFAULT_LEN		= 7;
const	int		EVENT_ONTOOLBAR_KEYEND_NAME_DEFAULT_LEN		= 20;
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CParseObject*	m_parseButton = NULL;		//	�M���ΨӸѪRToolButton��

/*===========================================================================+
|           Class implementation - tmToolBar                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmToolBar, CtmWnd)
CtmToolBar::CtmToolBar() : CtmWnd()
{
	strcpy(TYPEName, "CtmToolBar");
	m_wSP = 0;
	m_bBmpButton = TRUE;					//<<yang 2005/10/21 
	m_fgc = m_wndattr.fgcToolBar1;
	m_bgc = m_wndattr.bgcToolBar1;
	
	
	//	fans add
	m_pszBmpBackPath = NULL;
	m_pszBmpUpPath = NULL;
	m_pszBmpDownPath = NULL;
	m_pszMainToolName = NULL;
	m_pszCurrentToolName = NULL;
	m_pszCurrentFormName = NULL;
	OnToolBarKeyA = NULL;
	OnToolBarKeyEnd = NULL;
	m_pszOnKey = new char[EVENT_ONTOOLBAR_KEYSTART_NAME_DEFAULT_LEN];
	m_pszOnKeyEnd = new char[EVENT_ONTOOLBAR_KEYEND_NAME_DEFAULT_LEN];
	strcpy(m_pszOnKey, "OnKeyA");
	strcpy(m_pszOnKeyEnd, "OnKeyAEnd");
	
	if (m_parseButton == NULL) m_parseButton = new CParseObject();
	
	for (int i = 0; i < tmMAX_TOOLNEST; i++) m_ToolStack[i].pszBarName = NULL;

}

//	fans add
void 	CtmToolBar::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("bmpbackpath",		tmSTR,		&m_pszBmpBackPath);
	SetPropInfo("bmpuppath",		tmSTR,		&m_pszBmpUpPath);
	SetPropInfo("bmpdownpath",		tmSTR,		&m_pszBmpDownPath);
	SetPropInfo("maintool",			tmSTR,		&m_pszMainToolName);
	SetPropInfo("OnKeyA",			tmPROCEDURE,&m_pszOnKey);
	SetPropInfo("OnKeyAEnd",		tmPROCEDURE,&m_pszOnKeyEnd);
}

//	fans add
BOOL	CtmToolBar::CreateA()
{
	CtmWnd::CreateA();

	if (m_bBmpButton)						//<<yang 2005/10/21 
	{
		if (m_pszBmpUpPath != NULL) m_pbmpUp.Create(m_pszBmpUpPath);
		else m_pbmpUp.Create(tmTOOLBAR_BUTTONUP);
		
		if (m_pszBmpDownPath != NULL) m_pbmpDown.Create(m_pszBmpDownPath);
		else m_pbmpDown.Create(tmTOOLBAR_BUTTONDONW);
	}
	
	if (m_pszBmpBackPath == NULL) 
		m_pbmpBackground = g_aIniBmp[TOOLBAR_BACKGROUNDID].pIniBmp;
	/*
	else 
	{
		//	�ЫعϤ�,���O�·Ъ��O�Ыت��ݭn����
	}
	*/
	
	//	load Dll
	char	libName[512]; 
	void*	libHandle = NULL;
	sprintf(libName, "%s%s%s", CtmConfig::GetInstance()->GetFormlibPath(), Name, ".so");
	// <FANS> 2006/11/20 09:50�W��
	//strcpy(libName, "./formlib/");
	//strcat(libName, Name);
	//strcat(libName, ".so");
	libHandle = dlopen(libName, RTLD_LAZY);	
	
	SetLibHandle(libHandle);
	
	
	
	
	
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
	
	m_pszCurrentToolName = new char[strlen(pszFileName) + 1];
	strcpy(m_pszCurrentToolName, pszFileName);
	Clear();
	//========
	//	�Ы�Button
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
	else return;
	//	�N�ѪR�X�Ӫ�Button�]�m�J�D
	for (int i = 0; i < m_parseButton->GetControlCount(); i++)
	{
		m_parseButton->m_aryControls[i].control->SetParent(this);	
		((CtmToolButton*)m_parseButton->m_aryControls[i].control)->SetUpBmp(&m_pbmpUp);
		((CtmToolButton*)m_parseButton->m_aryControls[i].control)->SetDownBmp(&m_pbmpDown);
		m_parseButton->m_aryControls[i].control->CreateA();
		
	}
	
	ShowAllButton();
/****************************************************************************+
|           put Key                                                          |
+****************************************************************************/	
	WORD wKey = _NULL_KEY;
	KEYFLAG keyflag;
	while (_TestKeyFlag((KEYFLAG __far*)&keyflag) != _NULL_KEY) wKey =_GetKey();	
	
	//	���i��ToolBar�W���٦���L������A�ҥH�٬O�n�ϧO��ݪ�
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
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
		{
			((CtmToolButton*)Controls[i])->SetFGColor(m_wndattr.fgcToolBar2);
			((CtmToolButton*)Controls[i])->Update();
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
		//	�R�����OButton,�]�N�O�P��Button
		//==========
		if (Controls[i] != NULL)
			if (Controls[i]->Is("CtmToolButton"))
			{
				delete Controls[i];
				Controls[i] = NULL;
			}
	}
}

CtmToolBar::~CtmToolBar()
{
	//===========
	//	�p�G���Nm_parseButton->m_aryControls[0]������m��NULL���ܡA�b����m_parseButton���ɭ�
	//	�|���s����@���A���O�ѩ�m_aryControls[0]�O�q�ݤ_CtmToolBar���A�ҥHCtmToolBar�N�|�۰�����
	//	���q�ݪ��Ҧ�����
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
	
	if (m_pszCurrentFormName != NULL) delete []m_pszCurrentFormName;
	//Destroy();
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
	
	
	tmTOOLSTACK	tempStack;
	
	iButtonIndex = FindButton(wKey);
	
	if (iButtonIndex >= 0)	
	{
		
		pbutton = (CtmToolButton*)Controls[iButtonIndex];		
		
		//	�i�H��m�~�񪺤�k���w
		if (OnToolBarKeyA == NULL)
		{
			if (GetLibHandle() != NULL)
			{
				OnToolBarKeyA = (WORD(*)(CtmToolBar*, CtmButton*, WORD, int))dlsym(GetLibHandle(), m_pszOnKey);
				char* pszError;
				pszError = dlerror();	//	�ݭn�˴����~
				if (pszError != NULL) OnToolBarKeyA = NULL;
			}
		}
		if (OnToolBarKeyA != NULL) OnToolBarKeyA(this, pbutton, wKey, iButtonIndex);
		
		pbutton->GetPropValueT("formname", 	szWndName, 	sizeof(szWndName));		
		pbutton->GetPropValueT("group", 	&wGroup, 	sizeof(wGroup));
		pbutton->GetPropValueT("action",	&wAction, 	sizeof(wAction));
		pbutton->GetPropValueT("barname", 	szBarNameTemp, sizeof(szBarNameTemp));
		//=========
		//	�H�U���o�y�ܬO�ݭn�Ҽ{��
		//=========
		BOOL	bContinue = TRUE;
		if (szWndName[0] == '\0' && szBarNameTemp[0] == '\0') 
		{
			bContinue = FALSE;
			//return wKey;
		}
		
		if (bContinue)
		{
			if (szBarNameTemp[0] != '\0')
			{
				pszLinkBar = new char[strlen(szBarNameTemp) + 1];
				strcpy(pszLinkBar, szBarNameTemp);
			}
			
			//=======
			//       Process Button visual behavior
			//=======
			switch (wGroup)
			{
				case 0:
					pbutton->Press(tmBUTTON_DOWNUP);
					break;
				case 1:
					//cout << "wGroup is 1 " << endl;
					for (i = 0; i < GetControlCount(); i++)
					{
						if (Controls[i]->Is("CtmToolButton"))
							if (*(WORD*)Controls[i]->GetPropValue("group") > wGroup)
								((CtmToolButton*)Controls[i])->Press(tmBUTTON_UP);
					}				
					pbutton->Press(tmBUTTON_DOWNUP);
					break;
				default:
					
					//FreshDraw(pbutton);// <FANS> 2006/11/14 02:18�U��
					for (i = 0; i < GetControlCount(); i++)
					{
						if (Controls[i]->Is("CtmToolButton"))
							if (*(WORD*)Controls[i]->GetPropValue("group") == wGroup)
							{
								((CtmToolButton*)Controls[i])->Press(tmBUTTON_UP);
							}
					}				
					pbutton->Press(tmBUTTON_DOWN);
					
					break;
			}
			
			
			//=======
			//       Preset command and action code
			//=======
			
			switch (wAction & 0xFF00)
			{
				case TOOLBAR_NEXT:	
					
					if (pszLinkBar == NULL) return _NULL_KEY;
					CreateButtons(pszLinkBar);
					delete []pszLinkBar;
					return _NULL_KEY;
				case TOOLBAR_SELECT:
					
					if (pszLinkBar != NULL)
					{
						tempStack.pszBarName = m_pszCurrentToolName;
						tempStack.iViewIndex = iButtonIndex;
						PushS(tempStack);
						CreateButtons(pszLinkBar);
						delete []pszLinkBar;
					}
					return _NULL_KEY;
				case TOOLBAR_RETURN:
					
					tempStack = PopS();
					CreateButtons(tempStack.pszBarName, tempStack.iViewIndex);
					return _NULL_KEY;
				case TOOLBAR_RETURNHOME:
				
					tempStack = PopS();
					CreateButtons(m_pszMainToolName);
					return _NULL_KEY;
				case ID_TOOL_RESET:
					
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
						if (m_pszCurrentFormName != NULL)
						{
							if (strcmp(szWndName, m_pszCurrentFormName) != 0)
							{
								delete []m_pszCurrentFormName;
								m_pszCurrentFormName = new char[strlen(szWndName) + 1];
								strcpy(m_pszCurrentFormName, szWndName);
							}
							else break;
						}
						else
						{
							m_pszCurrentFormName = new char[strlen(szWndName) + 1];
							strcpy(m_pszCurrentFormName, szWndName);
						}
//						printf("toolbar form name =%s\n", szWndName);
						::PutCommand(szWndName);
					}
					break;
			}
		}
		
		//	�i�H��m�~�񪺤�k���w
		if (OnToolBarKeyEnd == NULL)
		{
			if (GetLibHandle() != NULL)
			{
				OnToolBarKeyEnd = (WORD(*)(CtmToolBar*, CtmButton*, WORD, int))dlsym(GetLibHandle(), m_pszOnKeyEnd);
				char* pszError;
				pszError = dlerror();	//	�ݭn�˴����~
				if (pszError != NULL) OnToolBarKeyEnd = NULL;
			}
		}
		if (OnToolBarKeyEnd != NULL) OnToolBarKeyEnd(this, pbutton, wKey, iButtonIndex);
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
	
	MKRECT(rect, 0, 0, GetClientWidth(), GetClientHeight())
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
	m_pbmpBackground->Show(rect, 0);
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

void	CtmToolBar::FreshDraw(CtmToolButton* pbtn)
{
	const	int dif = 10;
	
	int index = -1;
	short left,top,right,bottom;
	short leftA,topA,rightA,bottomA;
	left = top = right = bottom = 0;
	
	leftA = topA = rightA = bottomA = 0;
	Clear();			//	�M���즳��Toolbar
	
	//	�o�X�J�IButton���ǦC��
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
			if (Controls[i] == pbtn)
			{
				index = i;
				break;
			}
	}
	
	
	//	�N�Ҧ���Button���ХH�Τj�p�i���ܴ�
	for (int i = 0; i < GetControlCount(); i++)
	{
		if (Controls[i]->Is("CtmToolButton"))
		{
		//	//cout << "ToolButton is " << Controls[i]->Name << endl;
			left = *(short*)Controls[i]->GetPropValue("left");
			top = *(short*)Controls[i]->GetPropValue("top");
			right = *(short*)Controls[i]->GetPropValue("right");
			bottom = *(short*)Controls[i]->GetPropValue("bottom");
			/*
			//cout << "Left  is " << left << ";"				 
				 << "right  is " << right << endl;
			*/
				 
			if (Controls[i] != pbtn)
			{
				if (i > index) 
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
			
			
			//	�ݭn���s���
			Controls[i]->CreateA();
			((CtmToolButton*)Controls[i])->SetFGColor(m_wndattr.fgcToolBar2);
			((CtmToolButton*)Controls[i])->Update();
			
			/*
			//cout << "LeftA is " << leftA << ";"				 
				 << "rightA is " << rightA << endl;
			*/
				 				 
			
			//	�N���д_��M�j�p�_��
			Controls[i]->SetPropValueT("left", left);
			Controls[i]->SetPropValueT("right", right);
			Controls[i]->SetPropValueT("top", top);
			Controls[i]->SetPropValueT("bottom", bottom);
			
		}
	}
	
}
