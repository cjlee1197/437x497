/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"optionbox.h"
#include	"utils.h"
#include 	"tmconfig.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define				tmBDWIDTH_OPBOX			1
#define				tmBGCOLOR_OPBOX1		BRIGHTWHITE
#define				tmBGCOLOR_OPBOX2		BLACK
#define				tmBGCOLOR_OPBOX3		BRIGHTWHITE
#define				tmBGCOLOR_OPBOX4		BLACK

#define		tmSELECT_BMP		"./res/control/select2.bmp"
#define		tmUNSELECT_BMP		"./res/control/select1.bmp"
extern	BOOL g_bFocusChanged;

/*===========================================================================+
|           Class implementation - OptionBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmOptionBox, CtmWnd)
CtmOptionBox::CtmOptionBox(tmWND* pwnd, tmOPTION* poption) : CtmWnd()
{
	SetBorderWidth(m_wndattr.wBorderWidthOptionBox);
	CtmWnd::Create(pwnd);
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
	{
			m_iFontUNType=1;
	}
	else
	{
			m_iFontUNType=0;
	}
	m_poption = poption;
	m_nIndex = poption->nIndex;
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
		Show();
}

CtmOptionBox::CtmOptionBox():CtmWnd()
{
	strcpy(TYPEName, "CtmOptionBox");	

	SetBorderWidth(m_wndattr.wBorderWidthOptionBox);
	m_nIndex = 0;
	m_option.nIndex = 0;
	m_option.nCount = 0;
	for (int i = 0; i < MAXOPTINSELECT; i++)
	{
		m_option.pitem[i].nX = 0;
		m_option.pitem[i].nY = 0;
		m_option.pitem[i].psz = NULL;
		m_psztSetlectID[i] = NULL;
	}
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
	{
			m_iFontUNType=1;
	}
	else
	{
			m_iFontUNType=0;
	}
	//	fans	2006/12/15 04:37下午
	for (int i = 0; i < MAXOPTINSELECT; i++)	m_pszDBID[i] = NULL;	
	m_iStartVaule = 0;
	m_option.pszPrompt = NULL;
	m_pszPromptID = NULL;
	m_poption = &m_option;
	m_bBackGround = FALSE;
}

CtmOptionBox::~CtmOptionBox()
{
	
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

BOOL	CtmOptionBox::CreateA()
{

	CtmWnd::CreateA();

	//m_nIndex = m_option.nIndex;
	StrIDToStr(m_pszPromptID, &m_option.pszPrompt);
	for (int i = 0; i < MAXOPTINSELECT; i++)
	{
		if(m_iFontUNType > 0)
		{	
			printf("here .....................\n");	
			if(m_psztSetlectID[i]!=NULL)
			{
				if(m_option.pitem[i].psz!=NULL)
				{

					free(m_option.pitem[i].psz);
					m_option.pitem[i].psz=NULL;
				}

				int len=strlen(m_psztSetlectID[i])+1;
				m_option.pitem[i].psz=(char*)malloc(len);
				memcpy(m_option.pitem[i].psz,m_psztSetlectID[i],len);
			}	
		}
		else
		{
			if(m_psztSetlectID[i]!=NULL)
				StrIDToStr(m_psztSetlectID[i], &m_option.pitem[i].psz);

		}
	}
		
	//	fans	2006/12/15 04:49下午
	if (m_pszDBID[0] != NULL)
	{
		long long lTemp = GetDBValue(m_pszDBID[0]).lValue;
		lTemp -= m_iStartVaule;
		if (lTemp >= m_option.nCount) lTemp = m_option.nCount - 1;
		else if (lTemp < 0) lTemp = 0;
		m_nIndex = (int)lTemp;
	}
	Get();
	m_nOldIndex = m_nIndex;	//	fans add 2006/12/29 11:03上午

	return TRUE;
}	

void        CtmOptionBox::Get()              /* Virtual function */
{
	#if     defined(D_INET)||defined(D_MDFY)           
	g_wOldValue = (WORD)m_option.nIndex;             //BW 0611'02
	#endif
	
	//	fans 2007/9/18 11:40上午
	if (m_nIndex < 0 || m_nIndex >= m_option.nCount)
		m_nIndex = 0;
		
	m_option.nIndex = m_nIndex;
}

void        CtmOptionBox::Show()          /* Virtual function */
{

	short       i;
	short       nX;
	short       nY;
	short       nYOffset = (m_pcFont->m_wHeight - 16) >> 1;
	
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{
		Clear();
		_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
		for (i=0; i<m_option.nCount; i++)
    	{
		    nX = m_option.pitem[i].nX;
		    nY = m_option.pitem[i].nY + nYOffset;
		    _SetColor(m_wndattr.fgcOptionBox1);
		    _Ellipse(_GFILLINTERIOR, nX+1, nY+1, nX+15, nY+15);
		    _SetColor(m_wndattr.fgcOptionBox2);
		    _Arc(nX, nY, nX+16, nY+16, nX+15, nY, nX, nY+15);
			
			if (m_option.pitem[i].psz != NULL) 
			{
				printf("option: here .........\n");
				CtmWnd::ShowExtent(nX+24, nY-nYOffset, m_option.pitem[i].psz);
			}
				
   		}
		Select(m_nIndex);		
		CtmWnd::Show();
	}
	else
	{
		DrawBorder();
		if (m_bgcOld == 0)
			m_bgcOld = m_bgc;		
 		if (GetParent() != NULL) 
 			m_bgc = GetParent()->GetBGColor(); 		
 		CtmWnd::SetClientRECT(-1, 0, 1, 0);       // Restore the area for vertical cursor
 		Clear();
 		CtmWnd::SetClientRECT(1, 0, -1, 0);       // Make room for vertical cursor	
 		m_bgc = m_bgcOld;
	}
	

}

void        CtmOptionBox::Update()        /* Virtual function */
{
	//	fans modify 2006/12/29 11:03上午

	if (!m_bOnFocus)
    {
    	if ((m_nIndex != m_nOldIndex) || m_bPropertyChanged)
		{
			//Set();
			Show();
			m_bPropertyChanged = FALSE;		//James add 2008/3/27 09:36上午
		}
	}
}

void        CtmOptionBox::OnGetFocus()    /* Virtual function */
{
	CtmWnd::DrawGetFocus();
	::Prompt(m_option.pszPrompt);		//Sunny<20070822>打開
}

void        CtmOptionBox::OnLoseFocus()   /* Virtual function */
{
	//	fans	2006/12/15 04:49下午

	if (m_pszDBID[0] != NULL&&!g_bFocusChanged)
	{
		if (m_nIndex != m_option.nIndex)
		{
			
			SetViewDBValue(m_pszDBID[0], m_nIndex + m_iStartVaule);
//			printf("option save DB=%s, %d\n", m_pszDBID[0], m_nIndex + m_iStartVaule);
		}
	}
	Get();	
	
	CtmWnd::DrawLoseFocus();

}

int		CtmOptionBox::OnLose()
{
	
	if (m_pszDBID[0] != NULL)
	{
		long long lTemp = GetDBValue(m_pszDBID[0]).lValue;
		lTemp -= m_iStartVaule;
		if (lTemp >= m_option.nCount) lTemp = m_option.nCount - 1;
		else if (lTemp < 0) lTemp = 0;
		m_nIndex = (int)lTemp;
	}
	Get();
	CtmWnd::DrawLoseFocus();
	m_bPropertyChanged = TRUE;
	Update();
	
	return 0;
}

WORD        CtmOptionBox::OnKey(WORD wKey)/* Virtual function */
{
	long long  llOptionOk = GetDBValue("MCONF_HMI_NULL_NULL_OPTIONOKOPTION").lValue;   //OK鍵功能選擇
	
	switch (wKey)
	{
		case _KEY_LEFTFIELD:
		case _LARROW:
		case _EXT_LARROW:
			Select(m_nIndex - 1);
			break;
		case _KEY_RIGHTFIELD:
		case _RARROW:
		case _EXT_RARROW:
			Select(m_nIndex + 1);
			break;
		case _CTRL_C:	//James modify _CTRL_C for _ESC
			Select(m_option.nIndex);
			return wKey;
		default:
			return wKey;
    }


	return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmOptionBox::Select(short nIndex)
{

	short       nX;
	short       nY;
	short       nYOffset = (m_pcFont->m_wHeight - 16) >> 1;
	
	if      (nIndex < 0)                  nIndex = m_option.nCount - 1;
	else if (nIndex >= m_option.nCount) nIndex = 0;
	
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	nX = m_option.pitem[m_nIndex].nX;
	nY = m_option.pitem[m_nIndex].nY + nYOffset;
	_SetColor(m_wndattr.fgcOptionBox3);
	_Rectangle(_GFILLINTERIOR, nX+4, nY+4, nX+12, nY+12);
	
	nX = m_option.pitem[nIndex].nX;
	nY = m_option.pitem[nIndex].nY + nYOffset;
	_SetColor(m_wndattr.fgcOptionBox4);
	_Ellipse(_GFILLINTERIOR, nX+4, nY+4, nX+12, nY+12);
	
	m_nIndex = nIndex;
	m_nOldIndex = m_nIndex;	//	fans add	2006/12/29 11:02上午
}


void	CtmOptionBox::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	SetPropInfo("selectindex", 		tmINT,		&m_nIndex);
	SetPropInfo("selectcount", 		tmSHORT,	&m_option.nCount);
	SetPropInfo("prompt",			tmUSTR,		&m_option.pszPrompt);
	SetPropInfo("promptID",			tmSTR,		&m_pszPromptID);
	SetPropInfo("startvalue", 		tmINT,		&m_iStartVaule);
	char sz[10];
	for (int i = 0; i < MAXOPTINSELECT; i++)
	{
		sprintf(sz, "%s%d", "dbid", i);
		SetPropInfo(sz,		tmSTR, 		&m_pszDBID[i]);	
	}
	
	char Name[25];
	char temp[3];
	for (int i = 0 ; i < MAXOPTINSELECT; i++)
	{
		temp[0] = '\0';	
		sprintf(temp, "%d", i);
		
		strcpy(Name, "selectoption_nx");		
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSHORT,	&(m_option.pitem[i].nX));
		
		strcpy(Name, "selectoption_ny");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSHORT,	&(m_option.pitem[i].nY));
		
		strcpy(Name, "selectoption_text");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmUSTR,	&m_option.pitem[i].psz);
	
		strcpy(Name, "selectoption_textID");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_psztSetlectID[i]);
	}

	
}

void    CtmOptionBox::Clear()
{
	if (m_wnd.wStyle & tmWS_TRANSPARENT) 
    {
        if (!m_bBackGround) 
        {
            m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
            m_bBackGround = TRUE;
        }
        m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
        
    }
    else
    {
        CtmWnd::Clear();
    }
}


  bool    CtmOptionBox::TouchDownEvent(int x, int y)
  {
  	RECT rect =GetWindowRECT();
  	if(x>rect.right||x<rect.left||y>rect.bottom||y<rect.top)
  		return false;
  	return true;

  }
 bool    CtmOptionBox::TouchUpEvent(int x, int y)
 {
 	RECT rect =GetWindowRECT();
	if(x>rect.right||x<rect.left||y>rect.bottom||y<rect.top)
  		return false;
  	if(m_inTouchArea)
  	{
  		m_inTouchArea=false;
	  	if(x>=rect.left&&x<(rect.right>>1))
	  		Select(m_nIndex-1);
	  	else
	  		Select(m_nIndex+1);

	}
  	return true;

 }
void CtmOptionBox::WndProc(int message, WPARAM wParam, LPARAM lParam)
{

	int x=wParam>>16;
	int y=wParam&0xFFFF;
	switch(message)
	{
		case MSG_MOUSELUP:
			TouchUpEvent(x,y);
			break;
		case MSG_MOUSELDOWN:
			m_inTouchArea=TouchDownEvent(x, y);
			break;
		default:
			break;

	}
}
