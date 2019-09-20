/* Copyright (C), 2003-2010, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Gerry	                                                        |
|  Version  : V1.00                                                          |
|  Creation : 14/9/2010                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"switchbox.h"
#include	"utils.h"
//#include	"memwatch.h"
/*===========================================================================+
|           External variable                                                |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - OptionBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmSwitchBox, CtmWnd)

CtmSwitchBox::CtmSwitchBox(): CtmWnd()
{
	strcpy(TYPEName, "CtmSwitchBox");
	
	m_nIndex		= 0;
	m_bOnFocus    	=
	m_bChanged 		= FALSE;
	m_nSpace		= 1;
	m_nX			=
	m_nY			= 0;
	
	m_pszPromptID 	= NULL;
	
	m_switch.wType 	= 0;
	m_switch.pBmp 	= NULL;
	m_switch.pStr 	= NULL;
	m_switch.pPrompt= NULL;
	m_bBackGround 	= FALSE;
	m_pswitch		= &m_switch;
	for (int i = 0; i < MAX_STRING_NUM; i++)
	{
		m_pszbmpNames[i] 	= NULL;
		m_pszStrs[i]	  	= NULL;
		m_pszStrIDs[i]	  	= NULL;
	}
	
	for (int i = 0; i < CNT_INT_MAX_DB_NUM; i++)	m_pszDBID[i] = NULL;
}


CtmSwitchBox::~CtmSwitchBox()
{		
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	CtmSwitchBox::Update()
{
	if (!m_bOnFocus || m_bChanged)
	{
		m_bChanged = FALSE;
		GetEntry();
		Show();
	}
	else
	{
		Show();
	}
}
/*---------------------------------------------------------------------------+
| Function  : Show()										                 |
| Task	  : show a checkbox and its extern							         |
+----------------------------------------------------------------------------+
|  Call     : 	                                  						     |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSwitchBox::Show()
{
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	if (m_wnd.wStyle & tmWS_SHOW)
	{
		Clear();
		ShowBmp();
		ShowString();
		CtmWnd::Show();
	}
}

WORD	CtmSwitchBox::OnKey(WORD wKey)
{	
	switch (wKey)
    {
	    //case _KEY_SELECT:
	    case 46:
	        Switch();
	        break;
	    default:
	        return wKey;
    }

	return _NULL_KEY;
}

void CtmSwitchBox::Switch()
{
	int	oldIndex = m_nIndex;
	
	m_bChanged = TRUE;
	
	for (int i = 0; i < MAX_STRING_NUM; i++)
	{
		 m_nIndex++;
		 
		 if (m_nIndex == MAX_STRING_NUM)
			m_nIndex = 0;
	
		 if (m_pszbmpNames[m_nIndex] != NULL || m_pszStrIDs[m_nIndex] != NULL)
		 	break;
	}
	
	if (oldIndex == m_nIndex)
	{
		m_bChanged = FALSE;
		return;
	}
	
	GetEntry();
	Show();
}



//	fans add
void 	CtmSwitchBox::SetPropertys()
{
	char Name[20];
	char temp[3];
	
	CtmWnd::SetPropertys();
	SetPropInfo("index", 		tmINT	, 	&m_nIndex);
	SetPropInfo("promptID",		tmSTR	,	&m_pszPromptID);
	SetPropInfo("prompt",		tmSTR	,	&m_switch.pPrompt);
	SetPropInfo("space", 		tmINT	, 	&m_nSpace);
	
	for (int i = 0; i < MAX_STRING_NUM; i++)
	{
		temp[0] = '\0';	
		sprintf(temp, "%d", i);
		
		strcpy(Name, "bmpname");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszbmpNames[i]);
		
		strcpy(Name, "str");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszStrs[i]);
		
		strcpy(Name, "strID");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszStrIDs[i]);
	}
	
	for (int i = 0; i < CNT_INT_MAX_DB_NUM; i++)
	{
		temp[0] = '\0';	
		sprintf(temp, "%d", i);
		
		strcpy(Name, "dbid");
		strcat(Name, temp);
		SetPropInfo(Name, 	tmSTR,	&m_pszDBID[i]);
	}
}

BOOL 	CtmSwitchBox::CreateA()
{
	DBVIEWVALUE dbViewValue; 
			
	CtmWnd::CreateA();	
	
	if (m_pszDBID[0] != NULL)
	{
		dbViewValue = GetDBValue(m_pszDBID[0]);
		m_nIndex	= dbViewValue.lValue;
	}
	
	if (m_pszPromptID != NULL)
		StrIDToStr(m_pszPromptID, &m_switch.pPrompt);
	
	GetEntry();
	Show();
	
	return TRUE;
}

void    CtmSwitchBox::Clear()
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

void	CtmSwitchBox::GetEntry()
{
	char 	szPath[256];
	
	szPath[0] = '\0';
	
	if (m_pszStrIDs[m_nIndex] != NULL)
		StrIDToStr(m_pszStrIDs[m_nIndex], &m_switch.pStr);
	else if (m_switch.pStr != NULL)
	{
		delete [] m_switch.pStr;
		m_switch.pStr = NULL;
	}
	/*else if (m_pszStrIDs[0] != NULL)
		StrIDToStr(m_pszStrIDs[0], &m_switch.pStr);
	else
	*/
		
	if (m_pszbmpNames[m_nIndex] != NULL)
		sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), m_pszbmpNames[m_nIndex]);
	/*else if (m_pszbmpNames[0] != NULL)
		sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), m_pszbmpNames[0]);
	else
		;*/

	if (m_switch.pBmp != NULL)
	{
		delete m_switch.pBmp;
		m_switch.pBmp = NULL;
	}
	if (szPath[0] != 0)	
	{
		m_switch.pBmp = new CtmBitMap();
		m_switch.pBmp->Create(szPath);
	}
}

void	CtmSwitchBox::ShowString()
{
	tmSIZE        size;
	
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	
	if (m_pswitch->pStr == NULL)
		return;
		
    size = GetStringExtent((char*)m_pswitch->pStr);
    
    if (m_pswitch->pBmp != NULL)
    	m_nX = m_pswitch->pBmp->m_lWidth + m_nSpace * 2;
    else
    	m_nX = (m_rectClient.right - m_rectClient.left - size.w) >> 1;
    m_nY = (m_rectClient.bottom - m_rectClient.top - size.h) >> 1;
 
	CtmWnd::ShowExtent(m_nX, m_nY, m_pswitch->pStr);
}

void	CtmSwitchBox::ShowBmp()
{
	RECT		rect;
	short       nWidth  = GetClientWidth();
    short       nHeight = GetClientHeight(); 
    long		lbmpWidth;
    long		lbmpHeight;

	if (m_pswitch->pBmp == NULL)
		return;
	
	lbmpWidth 	= m_pswitch->pBmp->m_lWidth;
	lbmpHeight	= m_pswitch->pBmp->m_lHeight;

	MKRECT(rect, 0, 0, lbmpWidth - 1, lbmpHeight - 1);
	if (m_pswitch->pStr != NULL)	//靠左顯示
	{
		_SetViewPort(m_rectClient.left + m_nSpace,
					 m_rectClient.top + ((nHeight - lbmpHeight) >> 1),
					 m_rectClient.left + m_nSpace + lbmpWidth - 1, 
					 m_rectClient.top + ((nHeight - lbmpHeight) >> 1) + lbmpHeight - 1);
	}
	else		//居中顯示
	{
		_SetViewPort(m_rectClient.left + ((nWidth - lbmpWidth) >> 1),
					 m_rectClient.top + ((nHeight - lbmpHeight) >> 1),
					 m_rectClient.left + ((nWidth - lbmpWidth) >> 1) + lbmpWidth - 1,
					 m_rectClient.top + ((nHeight - lbmpHeight) >> 1) + lbmpHeight - 1);
	}
	
	m_pswitch->pBmp->Show(rect, tmBMP_KEYCOLOR);
}

BOOL	CtmSwitchBox::SetIndex(int nIndex)
{
	if((m_nIndex != nIndex) && (nIndex >=0) && (nIndex < MAX_STRING_NUM))
    {
    	m_bChanged = TRUE;
        m_nIndex = nIndex;
         if (m_pszDBID[0] != NULL)
    		SetViewDBValue(m_pszDBID[0], m_nIndex);
        return TRUE;
    }
    else
    	return FALSE; 
}

void	CtmSwitchBox::OnGetFocus()
{ 
	m_bOnFocus = TRUE; 
	if (m_pswitch->pPrompt != NULL)	Prompt(m_pswitch->pPrompt);
}

void	CtmSwitchBox::OnLoseFocus()
{ 
	m_bOnFocus = FALSE;
	
	if (m_pszDBID[0] != NULL && m_bChanged)
    {
    	SetViewDBValue(m_pszDBID[0], m_nIndex);
    }
}

