/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class	: CSelctBox                                                      |
|  Task     : CSelctBox file                                          		 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : netdigger                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/04/2003                                                     |
|  Revision : V2.00                                                          |
|             											                     |
+===========================================================================*/
#include	"selectbox.h"
#include	"utils.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
//#define		tmSELECT_BMP		"./res/control/select2.bmp"
//#define		tmUNSELECT_BMP		"./res/control/select1.bmp"

/*===========================================================================+
|           Class implementation - SelectBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmSelectBox, CtmWnd)
CtmSelectBox::CtmSelectBox(tmWND* pwnd,tmSELECT* pselect) : CtmWnd()
{
	//	fans add
	strcpy(TYPEName, "CtmSelectBox");
	m_select.psz = NULL;
	m_select.poldsz=NULL;
	m_select.pszPrompt = NULL;
	m_pselect = &m_select;
	m_pszTextID = NULL;
	m_pszPromptID = NULL;
	m_bBackGround 	= FALSE;
	SetBorderWidth(m_wndattr.wBorderWidthOptionBox);
	CtmWnd:: Create(pwnd);
	
	//	fans mark
	//	m_pselect = pselect;
	
	//	fans add
	memcpy(m_pselect, pselect, sizeof(tmSELECT));
	
	m_bSelect = m_pselect->bSelect;
	char	szPath[256];
	sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmBMP_SELECTED);
	m_bmpSelected.Create(szPath);
	sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmBMP_UNSELECTED);
	m_bmpUnSelected.Create(szPath);
	
	//	fans mark
	if(m_wnd.wStyle & tmWS_SHOW)		// show the externs if expected
	Show();
}

CtmSelectBox::~CtmSelectBox()
{
	m_BackImage.FreeImage();	
	
}


//	fans add
CtmSelectBox::CtmSelectBox(): CtmWnd()
{
	strcpy(TYPEName, "CtmSelectBox");
	//m_iFontUNType		= 1;
	m_select.psz = NULL;
	m_select.poldsz=NULL;
	m_select.pszPrompt = NULL;
	m_pszTextID = NULL;
	m_pszPromptID = NULL;
	m_bSelect = FALSE;
	m_igroup = 0;
	m_bgcOld = 0;
	m_pselect = &m_select;
	
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			
	else
		m_iFontUNType		= 0;
	m_bBackGround 	= FALSE;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	CtmSelectBox::Update()
{
//if (!m_bOnFocus)	2006/12/30 01:12¤U¤È	fans mark
    Set();
    Show();
}
/*---------------------------------------------------------------------------+
| Function  : Show()										          |
| Task	  : show a selectbox and its extern							|
+----------------------------------------------------------------------------+
|  Call     : 	                                  						|
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectBox::Show()
{
	BOOL		bRedraw=FALSE;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
 
    	
	if(m_wnd.wStyle & tmWS_SHOW)
	{

		short       nYOffset = (m_pcFont->m_wHeight - 16) >> 1;
	
		_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
		//J.Wang resused 2015/6/19 15:19:35
		Select();
		if(m_pselect->psz!=NULL)
		{
			if(m_pselect->poldsz == NULL)
			{
				bRedraw=TRUE;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
		    		int len = strlen(m_pselect->psz);
		    		m_pselect->poldsz = (char*)malloc(len+1);
		    		strcpy(m_pselect->poldsz, m_pselect->psz);
				}
				else
				{
					int len=0;
				     unsigned short* pwCode  = (unsigned short *)m_pselect->psz;
				     while(pwCode[len ++] != 0);
				     m_pselect->poldsz = (char*)malloc(len<<1);
				     memcpy( m_pselect->poldsz, m_pselect->psz, len << 1);
				}
			}
			else
			{
				int len1=0, len2=0;
				if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
				{
		    		len1 = strlen(m_pselect->psz);
		    		len2 = strlen(m_pselect->poldsz);
		    		
				}
				else
				{
				     unsigned short* pwCode  = (unsigned short *)m_pselect->psz;
				     while(pwCode[len1 ++] != 0);
				     pwCode  = (unsigned short *)m_pselect->poldsz;
				     while(pwCode[len2 ++] != 0);
				    
				}
				if(len1!=len2)
				{
					bRedraw=TRUE;
				}
				else
				{
					if(memcmp(m_pselect->poldsz,m_pselect->psz,len1)!=0)
					{
						bRedraw=TRUE;	
					}		
				}
				if(bRedraw)
				{
					free(m_pselect->poldsz);
					if(m_pcFont->GetFontCode() != tmFT_CODE_TECH)
					{
						m_pselect->poldsz = (char*)malloc(len1+1);
						strcpy(m_pselect->poldsz, m_pselect->psz);	
					}
					else
					{
						 m_pselect->poldsz = (char*)malloc(len1<<1);
				    	 memcpy( m_pselect->poldsz, m_pselect->psz, len1 << 1);	
					}
				}
						
			}
		}
		else
		{
			bRedraw=FALSE;
		}	
		if(bRedraw)
		{
			
			if (m_wnd.wStyle & tmWS_TRANSPARENT)
			{
				//CtmWnd::Show();
			  	Clear();
			}
			else
			{
				DrawBorder();
				CtmWnd::Clear();
			}
			if (m_pselect->psz != NULL)
				CtmWnd::ShowExtent(25, 0, m_pselect->psz);
			else
				CtmWnd::ShowExtent(25, 0, "", 0);
		}
		
		//	fans add
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


	ChangePosNow(GetClientRECT());
}
/*---------------------------------------------------------------------------+
| Function  : 		 OnGetFocus()							          |
| Task	  : 					 								|
+----------------------------------------------------------------------------+
|  Call     : 	                                  						|
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectBox::OnGetFocus()
{

	CtmWnd::DrawGetFocus();
	Set();
	Prompt(m_pselect->pszPrompt);

}
/*---------------------------------------------------------------------------+
| Function  : OnLoseFocus()													 |
| Task	  :																	 |
+----------------------------------------------------------------------------+
|  Call     :																 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmSelectBox::OnLoseFocus()
{

	CtmWnd::DrawLoseFocus();

}

WORD	CtmSelectBox::OnKey(WORD wKey)
{	
switch (wKey)
    {
    case _KEY_CLEAR:
    case _ESC:
        m_bSelect = FALSE;
        Select();
        m_bChanged = TRUE;                     // WJ 0620'03 add
        return _ENTER;
    case _ENTER:
    case (WORD)_EXT_ENTER:
        m_bSelect = TRUE;
        Select();
        m_bChanged = TRUE;                     // WJ 0620'03 add
        break;
    case _KEY_SELECT:								//<<yang 2005/12/7 
    	m_bSelect = TRUE;
        Select();
        m_bChanged = TRUE;
        return _KEY_SELECT;
    default:
        return wKey;
    }

return wKey;
}

void CtmSelectBox::Select()
{
	//short       nYOffset = (m_pcFont->m_wHeight - 16) >> 1;
	RECT	rect;
	WORD	wHeight = GetFont()->m_wHeight; 
	WORD	wWidth = GetFont()->m_wWidth;
	
	MKRECT(rect, 0, 0, wWidth-1, wHeight-1);
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.left+wWidth-1, m_rectClient.top+wHeight-1);
	if((m_wnd.wStyle & tmWS_REDRAW) && (m_wnd.wStyle & tmWS_TRANSPARENT))
	{
		m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
	}
	else
	{
		if (m_bSelect) 
		{
			m_bmpSelected.Show(rect, tmBMP_KEYCOLOR);		
			if (GetParent() != NULL)		
				for (int i = 0; i < GetParent()->GetControlCount(); i++)
				{
					if (GetParent()->Controls[i]->Is(TYPEName) && GetParent()->Controls[i] != this)				
						if (((CtmSelectBox*)GetParent()->Controls[i])->m_igroup == m_igroup && ((CtmSelectBox*)GetParent()->Controls[i])->m_bSelect)
						{
							((CtmSelectBox*)GetParent()->Controls[i])->m_bSelect = FALSE;
							GetParent()->Controls[i]->Update();						
						}
					
				}
		}
		else m_bmpUnSelected.Show(rect, tmBMP_KEYCOLOR);
	}
	  
}



//	fans add
void 	CtmSelectBox::SetPropertys()
{
	CtmWnd::SetPropertys();

	SetPropInfo("selected", 	tmBOOL, 	&m_bSelect);//m_select.bSelect);
	SetPropInfo("text",			tmUSTR,		&m_select.psz);
	SetPropInfo("prompt",		tmUSTR,		&m_select.pszPrompt);
	SetPropInfo("textID",		tmSTR,		&m_pszTextID);
	SetPropInfo("promptID", 	tmSTR,		&m_pszPromptID);	
	SetPropInfo("group", 		tmINT,		&m_igroup);

}

void 	CtmSelectBox::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();

	SetPropInfo(SELECTBOX_SELECTED, 	tmBOOL, 	&m_bSelect);//m_select.bSelect);
	SetPropInfo(SELECTBOX_TEXT,			tmUSTR,		&m_select.psz);
	SetPropInfo(SELECTBOX_PROMPT,		tmUSTR,		&m_select.pszPrompt);
	SetPropInfo(SELECTBOX_TEXTID,		tmSTR,		&m_pszTextID);
	SetPropInfo(SELECTBOX_PROMPTID, 	tmSTR,		&m_pszPromptID);	
	SetPropInfo(SELECTBOX_GROUP, 		tmINT,		&m_igroup);

}

//	fans add
BOOL 	CtmSelectBox::CreateA()
{
	////cout << "CtmSelectBox::CreateA()" << endl;
	SetBorderWidth(m_wndattr.wBorderWidthOptionBox);
	CtmWnd::CreateA();

	StrIDToStr(m_pszTextID, &m_select.psz);
	StrIDToStr(m_pszPromptID, &m_select.pszPrompt);
	
	char	szPath[256];
	sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmBMP_SELECTED);
	m_bmpSelected.Create(szPath);
	sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmBMP_UNSELECTED);
	m_bmpUnSelected.Create(szPath);
	
	//m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                       //   m_rectClient.bottom);
		
	return TRUE;
}

//	fans add
void    CtmSelectBox::Set()
{
	 //m_bSelect = m_pselect->bSelect;
	 //printf("m_pszTextID=%s\n",m_pszTextID);
	 StrIDToStr(m_pszTextID, &m_select.psz);
	// printf("%x,%x,%x,%x,%x,%x,%x,%x\n",m_select.psz[0],m_select.psz[1],m_select.psz[2],m_select.psz[3],m_select.psz[4],
	// m_select.psz[5],m_select.psz[6],m_select.psz[7],m_select.psz[8]);
	 StrIDToStr(m_pszPromptID, &m_select.pszPrompt);
}

void CtmSelectBox::Clear()
{
	if (!m_bBackGround)
	{
	    m_BackImage.CopyImage(m_rectWindow.left, m_rectWindow.top,
	      				  m_rectWindow.right, m_rectWindow.bottom);
	    m_bBackGround = TRUE;
	}
	else
	  	m_BackImage.ShowImage(m_rectWindow.left, m_rectWindow.top,
	                      m_rectWindow.right, m_rectWindow.bottom);
}



