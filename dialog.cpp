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
#include 	<time.h>
#include	"dialog.h"
#include 	"utils.h"
#include 	"_video.h"


#include	"main.h"
#include	"app.h"


CtmWnd* g_pwndCurFocus = NULL;

/*===========================================================================+
|           Class implementation - Dialog                                    |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmDialog, CtmWnd)

CtmDialog::CtmDialog() : CtmWnd()
{
	//	fans add
	strcpy(TYPEName, "CtmDialog");
	m_nItemCount = 0;
	m_nTabStop = -1;
	m_bGetFocus = FALSE;
	m_pszTitle = NULL;
    m_pszTitleID = NULL;	
    m_pszBmpBackPath = NULL;
    
    m_pCurrentFocus = NULL;
    m_DrawHelpFlag = FALSE;
}

CtmDialog::CtmDialog(tmWND* pwnd, tmDIALOG* pdlg) : CtmWnd()
{
	m_bGetFocus = FALSE;
	Create(pwnd, pdlg);
}

CtmDialog::~CtmDialog()
{
	
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CtmDialog::Make(tmDIALOG* pdlg, tmDIALOG_1* pdlg_1)
{
short       i = 0;
while (pdlg_1[i].wType != tmDLG_END)
    {
    pdlg[i].wType = pdlg_1[i].wType;
    pdlg[i].wnd.wStyle = pdlg_1[i].wnd.wStyle;
    pdlg[i].wnd.rect.left   = pdlg_1[i].wnd.rect.left;
    pdlg[i].wnd.rect.top    = pdlg_1[i].wnd.rect.top;
    pdlg[i].wnd.rect.right  = pdlg_1[i].wnd.rect.left + pdlg_1[i].wnd.rect.right - 1;
    pdlg[i].wnd.rect.bottom = pdlg_1[i].wnd.rect.top + pdlg_1[i].wnd.rect.bottom - 1;
    i++;
    }
pdlg[i].wType = tmDLG_END;
}

BOOL        CtmDialog::Create(tmWND* pwnd, tmDIALOG* pdlg)
{
	CtmWnd::Create(pwnd);
	
	
	BOOL        bOK = TRUE;

	tmWNDATTR     wndattr;
	
	GetWindowAttribute(&wndattr);
	m_pbuttonOK = NULL;
	m_pbuttonCancel = NULL;
	m_pbuttonExit = NULL;
	m_nItemCount = 0;
	m_nTabStop = -1;
	m_bPrompt = TRUE;                   //St052501 for message box not show prompt
	
	SetWindowAttribute(&wndattr);
	return bOK;
}

//fans add
BOOL		CtmDialog::CreateA()
{
	if (GetParent() != NULL) 
	{
		((CtmView*)GetParent())->SetBmpBack(m_pszBmpBackPath);
	}
	
	CtmWnd::CreateA();
	
	return TRUE;
}

//	fans add
void	CtmDialog::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	SetPropInfo("title", 		tmUSTR,		&m_pszTitle);
	SetPropInfo("titleID", 		tmSTR,		&m_pszTitleID);
	SetPropInfo("bmpbackpath", 	tmSTR,		&m_pszBmpBackPath);	
}

void	CtmDialog::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	
	SetPropInfo(WND_TITLE, 		tmUSTR,		&m_pszTitle);
	SetPropInfo(WND_TITLEID, 	tmSTR,		&m_pszTitleID);
	SetPropInfo(DIALOG_BMPBACKPATH, tmSTR,	&m_pszBmpBackPath);	
}

    
		

void        CtmDialog::Get()              /* Virtual function */
{

    //	fans add
	for (int i = 0; i < GetControlCount(); i++)
		if (Controls[i] != NULL) Controls[i]->Get();
}

void        CtmDialog::Set()              /* Virtual function */
{
    //	fans add
	for (int i = 0; i < GetControlCount(); i++)
		if (Controls[i] != NULL) Controls[i]->Set();
}

void        CtmDialog::Show()             /* Virtual function */
{
	CtmWnd::Show();
}

void        CtmDialog::Redraw(RECT rect)  /* Virtual function */
{
	
	/*
	ScreenToClient(&rect);
	if((m_wnd.wStyle & tmWS_TRANSPARENT) == 0)
		Clear(rect);

	//	fans add
	CtmWnd* pwndTemp = NULL;
	for (int i = 0; i < GetControlCount(); i++)
	{
		Controls[i]->DrawBorder();
		Controls[i]->Show();
		
		
	}
	if (m_nTabStop >= 0) pwndTemp = FindControlFromTab(m_nTabStop);
	if (pwndTemp != NULL) pwndTemp->OnGetFocus();
	*/

}

void        CtmDialog::OnGetFocus()       /* Virtual function */
{	
	CtmWnd* pwndTemp = NULL;
	//if ((!m_bGetFocus) && (m_nTabStop != -1))	fans mark
	if  ((!m_bGetFocus) && (m_nTabStop >=0 ))	//	fans add
	{
   		//OnGetFocus(m_nTabStop);
	   	int	nTab = m_nTabStop;
   		//int i = 0;
   		while (m_nTabStop <= m_iMaxTab)		//Caozz Modify 2012/8/31 上午 10:48:47  bug: May not found the taborder
   		{
			pwndTemp = FindControlFromTab(m_nTabStop);
			if ((pwndTemp != NULL) && pwndTemp->GetVisible() && pwndTemp->GetEnabled())
			{
				//printf("%s OnGetFocus\n",pwndTemp->TYPEName);
				pwndTemp->OnGetFocus();
				m_bGetFocus = TRUE;
				
				
				
				break;
			}
			//i++;
			FindNextTabStop();
			if (m_nTabStop == nTab) 
			{
				m_nTabStop = -1;
				break;
			}
			//m_nTabStop++;
		}
   	}
   	
   	if (m_nTabStop >= 0)
   	{
   		pwndTemp = FindControlFromTab(m_nTabStop);
   		
	   	if ((pwndTemp != NULL) && pwndTemp->GetVisible() && pwndTemp->GetEnabled())
		{
			if (m_pCurrentFocus != pwndTemp)
			{
				ResetHelpTimer();
				
				if (m_pCurrentFocus)
				{
					//if(m_pCurrentFocus!=NULL)
						//printf("last %s m_pCurrentFocus\n",m_pCurrentFocus->TYPEName);
					m_pCurrentFocus->DrawOnlineHelp(FALSE);
					//m_pCurrentFocus->Update();
					m_DrawHelpFlag = FALSE;	
					//printf("Undraw previous Wnd:%s\n", m_pCurrentFocus->Name);
				}
				//if(m_pCurrentFocus!=NULL)
				//printf("this %s m_pCurrentFocus\n",m_pCurrentFocus->TYPEName);
				m_pCurrentFocus = pwndTemp;
				//printf("%s Get Focus\n", m_pCurrentFocus->Name);
			}
			
		}
	}
	
}

void        CtmDialog::OnLoseFocus()      /* Virtual function */
{
	if  ((m_bGetFocus) && (m_nTabStop >=0 ))	//	fans add
    OnLoseFocus1(m_nTabStop);
}


int			CtmDialog::OnLose()
{
	if  ((m_bGetFocus) && (m_nTabStop >=0 ))
	{
		CtmWnd* pwndTemp = FindControlFromTab(m_nTabStop);		
		if (pwndTemp != NULL)	
		{
			pwndTemp->OnLose();
			m_bGetFocus = FALSE;
		}
	}
	return 0;
}


WORD        CtmDialog::OnKey(WORD wKey)   /* Virtual function */
{
//
/* 	fans mark
if (m_nTabStop != -1)
    {
    wKey = m_aitem[m_nTabStop].pwnd->OnKey(wKey);
*/
//	fas add

if (m_nTabStop >= 0)
	{
		
		CtmWnd* pwndTemp = FindControlFromTab(m_nTabStop);
		if (pwndTemp != NULL) 	wKey = pwndTemp->OnKey(wKey);
			
		
    switch (wKey)
        {
        case _NULL_KEY:                 // It is not a field terminator
            break;
        case _KEY_LEFTFIELD:
            OnLoseFocus();
            FindLeftTabStop();
            //OnGetFocus();
            break;
        case _KEY_RIGHTFIELD:
            OnLoseFocus();
            FindRightTabStop();
            //OnGetFocus();
            break;
        case _KEY_UPFIELD:
            OnLoseFocus();
            FindUpTabStop();
            //OnGetFocus();
            break;
        case _KEY_DOWNFIELD:
            OnLoseFocus();
            FindDownTabStop();
            //OnGetFocus();
            break;
        case _UARROW:
        case _EXT_UARROW:
        case _SHIFT_TAB:
            OnLoseFocus();
            FindLastTabStop();
            OnGetFocus();
            break;
        case _DARROW:
        case _EXT_DARROW:
        case _TAB:
            OnLoseFocus();
            FindNextTabStop();
            OnGetFocus();
            break;
        case _ESC:
//          OnLoseFocus();
            m_bOK = FALSE;
            ::Exit();
            /*	fans mark
            if (m_pbuttonCancel != NULL) m_pbuttonCancel->Press(tmBUTTON_DOWNUP);
            else if (m_pbuttonExit != NULL) m_pbuttonExit->Press(tmBUTTON_DOWNUP);
            */
            break;
        case _ENTER:
        case (WORD)_EXT_ENTER:
//          OnLoseFocus();
            Get();
            m_bOK = TRUE;
            ::Exit();
            /*	fans mark
            if (m_pbuttonOK != NULL) m_pbuttonOK->Press(tmBUTTON_DOWNUP);
            else if (m_pbuttonExit != NULL) m_pbuttonExit->Press(tmBUTTON_DOWNUP);
            */
            break;
        default:
            break;
        }
    }
else
    {
    switch (wKey)
        {
        case _ESC:
            m_bOK = FALSE;
            ::Exit();
            /*	fans mark
            if (m_pbuttonCancel != NULL) m_pbuttonCancel->Press(tmBUTTON_DOWNUP);
            else if (m_pbuttonExit != NULL) m_pbuttonExit->Press(tmBUTTON_DOWNUP);
            */
            break;
        case _ENTER:
        case (WORD)_EXT_ENTER:
            m_bOK = TRUE;
            ::Exit();
            /*	fans mark
            if (m_pbuttonOK != NULL) m_pbuttonOK->Press(tmBUTTON_DOWNUP);
            else if (m_pbuttonExit != NULL) m_pbuttonExit->Press(tmBUTTON_DOWNUP);
            */
            break;
        default:
            break;
        }
    }

return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|  Function : DoModal()                                                      |
|  Task     : Show dialog and get data                                       |
+----------------------------------------------------------------------------+
|  Call     : bOK = CtmDialog::DoModal()                                       |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   : bOK                       - Dialog is terminated by OK key     |
+---------------------------------------------------------------------------*/
BOOL        CtmDialog::DoModal()
{
	MSG         msg;
	

	if(this->GetParent()!=NULL)
	{
		MsgBoxRegion.top = this->GetClientRECT().top + this->GetParent()->GetClientRECT().top;
		MsgBoxRegion.bottom = this->GetClientRECT().bottom + this->GetParent()->GetClientRECT().top;
		MsgBoxRegion.left = this->GetClientRECT().left + this->GetParent()->GetClientRECT().left;
		MsgBoxRegion.top = this->GetClientRECT().right + this->GetParent()->GetClientRECT().left;
	}
	else
	{
		MsgBoxRegion.top = this->GetClientRECT().top;
		MsgBoxRegion.bottom = this->GetClientRECT().bottom;
		MsgBoxRegion.left = this->GetClientRECT().left;
		MsgBoxRegion.right = this->GetClientRECT().right;
	}
	msg.wMessage = _SYS_NEWSESSION;
	msg.pwnd  = this;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
	
	return m_bOK;
}

void        CtmDialog::Goto(WORD wIDControl)
{
	OnLoseFocus();
	//if (FindControlFromTab(wIDControl) != NULL)
		m_nTabStop = wIDControl;
	//if (!(m_aitem[m_nTabStop].wType & tmDLG_TABSTOP))	fans mark
    //	FindNextTabStop();
	OnGetFocus();
	m_bChangeTabStop = TRUE;                 // Set the flag to tell CFormView::OnKey() that do not change focus
}

void        CtmDialog::Update(WORD wIDControl)
{
	/*	fans mark
	m_aitem[wIDControl].pwnd->Update();
	*/
	
	//	fans add 
	CtmWnd* pwndTemp = NULL;
	pwndTemp = FindControlFromTab(wIDControl);
	
	if (pwndTemp != NULL) pwndTemp->Update();
	
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmDialog::FindTabStop(int iWay)
{
	
	
}

int		CtmDialog::FindNearTabStop(char* pszAlign, char* pszWay, int iWay)
{
	int result = -1;
	
	//	當前的焦點元件
	CtmWnd* pwnd = FindControlFromTab(m_nTabStop);	
	if (pwnd == NULL) return result;
	
	
	
	if (iWay >= 0)		//	表示光標的移動是向大的坐標方向
	{
		result = FindNearBig(pszAlign, pszWay, pwnd);		
	}
	else
	{
		result = FindNearSmall(pszAlign, pszWay, pwnd);
	}

}

int		CtmDialog::FindNearBig(char* pszAlign, char* pszWay, CtmWnd* pwndCurrent)
{
	int result = -1;
	
	int iAlignment = 0;				//	記錄每個元件在對齊方向上的坐標值
	int iWayment = 0;				//	記錄每個元件在光標移動方向上的坐標值
	int iCurrentAlign = 0;			//	當前的對齊標準，上下是以Left為對齊標準；左右是Top為對齊標準
	int	iCurrentWay = 0;			//	當前的元件在光標移動方向上的坐標值
	
	int	idifAlignment = 99999;
	int idifWayment = 99999;
	iCurrentWay = *(WORD*)pwndCurrent->GetPropValue(pszWay);
	iCurrentAlign = *(WORD*)pwndCurrent->GetPropValue(pszAlign);
	
	int iType = 0;
	
	for (int i = 0; i < GetControlCount(); i++)
	{
		Controls[i]->GetPropValueT("wtype", &iType, sizeof(iType));
		
		if ((Controls[i]->GetTabOrder() >=0) && (Controls[i] != pwndCurrent)&& (iType & tmWS_SHOW) && Controls[i]->GetVisible() && Controls[i]->GetEnabled())
		{
			iAlignment =  *(WORD*)Controls[i]->GetPropValue(pszAlign);
			iWayment = *(WORD*)Controls[i]->GetPropValue(pszWay);
			if (iWayment > iCurrentWay)
			{
				if (idifAlignment == 0)
				{
					if (iAlignment - iCurrentAlign != 0) continue;
					if (idifWayment > iWayment - iCurrentWay)
					{
						idifWayment = iWayment - iCurrentWay;
						result = i;
						continue;
					}
					else if (idifWayment == iWayment - iCurrentWay)
					{
						if (Controls[i]->GetTabOrder() < Controls[result]->GetTabOrder())
							result = i;
						continue;
					}
					else continue;
				}
				
				if (iAlignment - iCurrentAlign == 0)
				{
					idifAlignment = 0;
					result = i;
					idifWayment = iWayment - iCurrentWay;
					continue;
				}
				else
				{
					if (idifWayment > iWayment - iCurrentWay)
					{
						idifAlignment = abs(iAlignment - iCurrentAlign);
						idifWayment = iWayment - iCurrentWay;
						result =i;
						continue;
					}
					else if (idifWayment == iWayment - iCurrentWay)
					{
						if (idifAlignment == abs(iAlignment - iCurrentAlign))
						{
							if (Controls[i]->GetTabOrder() < Controls[result]->GetTabOrder())
								result = i;
						}
						else if (idifAlignment > abs(iAlignment -iCurrentAlign))
						{
							result = i;
							idifAlignment = abs(iAlignment - iCurrentAlign);
						}
						
						continue;
					}
					else
					{
						if (iAlignment - iCurrentAlign == 0)
						{
							idifWayment = iWayment - iCurrentWay;
							idifAlignment = 0;
							result = i;
						}
						continue;
					}
				}
				
			}	//	end of --- if (iWayment > iCurrentWay)
			
		}	//	end of --- if ((Controls[i]->GetTabOrder() >=0) && (Controls[i] != pwnd))
	}	//	end of --- for (int i = 0; i < GetControlCount(); i++)
	
	if (result < 0)
	{
		idifWayment = 0;
		for (int i = 0; i < GetControlCount(); i++)
		{
			
			Controls[i]->GetPropValueT("wtype", &iType, sizeof(iType));
			if ((Controls[i]->GetTabOrder() >=0) && (Controls[i] != pwndCurrent) && (iType & tmWS_SHOW) &&Controls[i]->GetVisible() && Controls[i]->GetEnabled())
			{
				iWayment = *(WORD*)Controls[i]->GetPropValue(pszWay);
				iAlignment = *(WORD*)Controls[i]->GetPropValue(pszAlign);
				
				if ((abs(iWayment - iCurrentWay) > idifWayment) && (iAlignment - iCurrentAlign == 0))
				{
					result = i;
					idifWayment = abs(iWayment - iCurrentWay);
				}
			}
		}
	}
	
	if (result >= 0)
	{
		if (Controls[result] != pwndCurrent)
		{
			m_nTabStop = Controls[result]->GetTabOrder();
			Controls[result]->OnGetFocus();
			m_bGetFocus = TRUE;
		}
	}
	
	return result;
}

int		CtmDialog::FindNearSmall(char* pszAlign, char* pszWay, CtmWnd* pwndCurrent)
{
	
	int result = -1;
	
	int iAlignment = 0;				//	記錄每個元件在對齊方向上的坐標值
	int iWayment = 0;				//	記錄每個元件在光標移動方向上的坐標值
	int iCurrentAlign = 0;			//	當前的對齊標準，上下是以Left為對齊標準；左右是Top為對齊標準
	int	iCurrentWay = 0;			//	當前的元件在光標移動方向上的坐標值
	
	int	idifAlignment = 99999;
	int idifWayment = 99999;
	iCurrentWay = *(WORD*)pwndCurrent->GetPropValue(pszWay);
	iCurrentAlign = *(WORD*)pwndCurrent->GetPropValue(pszAlign);
	
	int iType = 0;
	
	for (int i = 0; i < GetControlCount(); i++)
	{
		Controls[i]->GetPropValueT("wtype", &iType, sizeof(iType));
		if ((Controls[i]->GetTabOrder() >=0) && (Controls[i] != pwndCurrent) && (iType & tmWS_SHOW) && Controls[i]->GetVisible() && Controls[i]->GetEnabled())
		{
			iWayment = *(WORD*)Controls[i]->GetPropValue(pszWay);
			iAlignment = *(WORD*)Controls[i]->GetPropValue(pszAlign);
			if (iWayment < iCurrentWay)		
			{
				
				//=============
				//	檢測是否上一個是否左對齊,如果是的話,將不會往下執行
				//=============
				if (idifAlignment == 0)
				{
					if (iAlignment - iCurrentAlign != 0) continue;
					if (idifWayment > abs(iWayment - iCurrentWay))		//	檢測上一個上下的垂直距離是否比現在的大
					{
						idifWayment = abs(iWayment - iCurrentWay);
						result = i;
						continue;
					}
					else if (idifWayment == abs(iWayment - iCurrentWay))	//	如果上一個上下的垂直距離與現在的相等,則需要檢測TabOrder
					{
						if (Controls[i]->GetTabOrder() < Controls[result]->GetTabOrder())
							result = i;
						continue;
					}
					else continue;
				}
				
				//=============
				//	首先需要檢測左對齊的條件
				//=============
				if (iAlignment - iCurrentAlign == 0)		
				{
					
					idifAlignment = 0;
					result = i;
					idifWayment = abs(iWayment - iCurrentWay);
					continue;
					
				}
				else	//	左邊是沒有對齊的
				{
					if (idifWayment > abs(iWayment - iCurrentWay))		//	檢測上一個上下的垂直距離是否比現在的大
					{
						idifAlignment = abs(iAlignment - iCurrentAlign);
						idifWayment = iWayment - iCurrentWay;
						result = i;
						continue;
					}
					else if (idifWayment == abs(iWayment - iCurrentWay))	//	如果上一個上下的垂直距離與現在的相等,則需要檢測TabOrder
					{
						if (iAlignment - iCurrentAlign == 0)		//	現有元件是左對齊
						{
							idifWayment = abs(iWayment - iCurrentWay);
							idifAlignment = 0;
							result = i; 
						}
						else								//	不是左對齊
						{
							if (idifAlignment == abs(iAlignment - iCurrentAlign))	
							{
								if (Controls[i]->GetTabOrder() < Controls[result]->GetTabOrder())
								{
									idifAlignment = abs(iAlignment - iCurrentAlign);
									result = i;
								}
							}
							else if (idifAlignment > abs(iAlignment - iCurrentAlign)) 	//	尋找與需要對齊元件，左邊最近的
							{
								result = i;
								idifAlignment = abs(iAlignment - iCurrentAlign);
							}
							
						}
						continue;
					}
					else 
					{
						if (iAlignment - iCurrentAlign == 0)
						{
							idifWayment = abs(iWayment - iCurrentWay);
							idifAlignment = 0;
							result = i; 
						}
						continue;
					}
				}
			}
		}
	}
	
	
	if (result < 0)
	{
		idifWayment = 0;
		for (int i = 0; i < GetControlCount(); i++)
		{
			
			Controls[i]->GetPropValueT("wtype", &iType, sizeof(iType));
			if ((Controls[i]->GetTabOrder() >=0) && (Controls[i] != pwndCurrent) && (iType & tmWS_SHOW) && Controls[i]->GetVisible() && Controls[i]->GetEnabled())
			{
				iWayment = *(WORD*)Controls[i]->GetPropValue(pszWay);
				iAlignment = *(WORD*)Controls[i]->GetPropValue(pszAlign);
				
				if ((abs(iWayment - iCurrentWay) > idifWayment) && (iAlignment - iCurrentAlign == 0))
				{
					result = i;
					idifWayment = abs(iWayment - iCurrentWay);
				}
			}
		}
	}
	
	
	if(result >=0)
	{
		m_nTabStop = Controls[result]->GetTabOrder();
		Controls[result]->OnGetFocus();
		m_bGetFocus = TRUE;
		
	}
	
	return result;
}

void  CtmDialog::FindNextTabStop()
{
	//	fans add	
	//if ((++m_nTabStop) > FindMaxTabOrder()) m_nTabStop = 0;
	if ((++m_nTabStop) > m_iMaxTab) m_nTabStop = 0;
}

void  CtmDialog::FindLastTabStop()
{
	//	fans add
	if ((--m_nTabStop) < 0) m_nTabStop = 0;
	else if (m_nTabStop == 0) m_nTabStop = m_iMaxTab;//FindMaxTabOrder();	
}

void  CtmDialog::FindDownTabStop()
{
	
	CtmWnd*	pwnd = FindControlFromTab(m_nTabStop);
	CtmWnd* pwndDownControl = NULL;
	if (pwnd == NULL) return;
	
	char szControlName[256];// = NULL;
	int iTabOrder = 0xFFFFFFFF;
	int iType = 0;
	BOOL bVisible = FALSE;
	BOOL bEnabled = FALSE;
	
	int i = 0;
	
	//	循環的最多次數就是畫面上的所有控件個數
	while (i < GetControlCount())
	{
		//	查詢目前焦點元件的左移焦點元件
		pwnd->GetPropValueT("downcontrol", szControlName, sizeof(szControlName));
		//	如果沒有的話，需按照正常的執行
		if (szControlName[0] =='\0')// == NULL) 
		{
			FindNearBig("left", "top", pwnd);
			break;
		}
		else
		{
			pwndDownControl = FindControlFromName(szControlName);
			
			if (pwndDownControl!= NULL)
			{
				pwndDownControl->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
				pwndDownControl->GetPropValueT("wtype", &iType, sizeof(iType));
				pwndDownControl->GetPropValueT("visible", &bVisible, sizeof(bVisible));
				pwndDownControl->GetPropValueT("enabled", &bEnabled, sizeof(bEnabled));
				
				if ((iTabOrder >= 0) && (iType & tmWS_SHOW) && bVisible && bEnabled)
				{
					m_nTabStop = iTabOrder;
					pwndDownControl->OnGetFocus();
					m_bGetFocus = TRUE;
					break;
				}
				else
				{
					FindNearBig("left", "top", pwndDownControl);
					break;
				}
			}
			else 
			{
				FindNearBig("left", "top", pwnd);
				break;
			}
		}
		i++;
	}	
	
	
}

void  CtmDialog::FindUpTabStop()
{
    CtmWnd*	pwnd = FindControlFromTab(m_nTabStop);
    CtmWnd*	pwndUpControl = NULL;
	if (pwnd == NULL) return;
	
	char szControlName[256];// = NULL;
	int iTabOrder = 0xFFFFFFFF;
	int iType = 0;
	BOOL bVisible = FALSE;
	BOOL bEnabled = FALSE;
	int i = 0;
	
	//	循環的最多次數就是畫面上的所有控件個數
	while (i < GetControlCount())
	{
		//	查詢目前焦點元件的左移焦點元件
		pwnd->GetPropValueT("upcontrol", szControlName, sizeof(szControlName));
		
		//	如果沒有的話，需按照正常的執行
		if (szControlName[0] == '\0')
		{
			FindNearSmall("left", "top", pwnd);
			break;
		}
		else
		{
			pwndUpControl = FindControlFromName(szControlName);
			
			if (pwndUpControl!= NULL)
			{
				pwndUpControl->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
				pwndUpControl->GetPropValueT("wtype", &iType, sizeof(iType));
				pwndUpControl->GetPropValueT("visible", &bVisible, sizeof(bVisible));
				pwndUpControl->GetPropValueT("enabled", &bEnabled, sizeof(bEnabled));
				
				if ((iTabOrder >= 0) && (iType & tmWS_SHOW) && bVisible && bEnabled)
				{
					m_nTabStop = iTabOrder;
					pwndUpControl->OnGetFocus();
					m_bGetFocus = TRUE;
					break;
				}
				else
				{
					FindNearSmall("left", "top", pwndUpControl);
					break;
				}
			}
			else
			{
				FindNearSmall("left", "top", pwnd);
				break;
			}
		}
		i++;
	}	
	
    
}

void  CtmDialog::FindLeftTabStop()
{
	CtmWnd*	pwnd = FindControlFromTab(m_nTabStop);
	CtmWnd*	pwndLeftControl = NULL;
	if (pwnd == NULL) return;
	
	
	char szControlName[256];// = NULL;
	int iTabOrder = 0xFFFFFFFF;
	int iType = 0;
	BOOL bVisible = FALSE;
	BOOL bEnabled = FALSE;
	int i = 0;
	
	//	循環的最多次數就是畫面上的所有控件個數
	while (i < GetControlCount())
	{
		//	查詢目前焦點元件的左移焦點元件
		pwnd->GetPropValueT("leftcontrol", szControlName, sizeof(szControlName));
		
		//	如果沒有的話，需按照正常的執行
		if (szControlName[0] == '\0') 
		{
			FindNearSmall("top", "left", pwnd);
			break;
		}
		else
		{
			pwndLeftControl = FindControlFromName(szControlName);
			
			if (pwndLeftControl!= NULL)
			{
				pwndLeftControl->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
				pwndLeftControl->GetPropValueT("wtype", &iType, sizeof(iType));
				pwndLeftControl->GetPropValueT("visible", &bVisible, sizeof(bVisible));
				pwndLeftControl->GetPropValueT("enabled", &bEnabled, sizeof(bEnabled));
				
				if ((iTabOrder >= 0) && (iType & tmWS_SHOW) && bVisible && bEnabled)
				{
					m_nTabStop = iTabOrder;
					pwndLeftControl->OnGetFocus();
					m_bGetFocus = TRUE;
					break;
				}
				else
				{
					FindNearSmall("top", "left", pwndLeftControl);
					break;
				}
			}
			else
			{
				FindNearSmall("top", "left", pwnd);
				break;
			}			
		}
		i++;
	}	
	
	

}

void  CtmDialog::FindRightTabStop()
{
	CtmWnd*	pwnd = FindControlFromTab(m_nTabStop);
	CtmWnd* pwndRightControl = NULL;
	if (pwnd == NULL) return;
	
	
	char szControlName[256];// = NULL;
	int iTabOrder = 0xFFFFFFFF;
	int iType = 0;
	BOOL bVisible = FALSE;
	BOOL bEnabled = FALSE;
	int i = 0;
	
	//	循環的最多次數就是畫面上的所有控件個數
	while (i < GetControlCount())
	{
		//	查詢目前焦點元件的左移焦點元件
		pwnd->GetPropValueT("rightcontrol", szControlName, sizeof(szControlName));
		
		//	如果沒有的話，需按照正常的執行
		if (szControlName[0] == '\0') 
		{
			FindNearBig("top", "left", pwnd);
			break;
		}
		else
		{
			pwndRightControl = FindControlFromName(szControlName);
			
			if (pwndRightControl!= NULL)
			{
				pwndRightControl->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
				pwndRightControl->GetPropValueT("wtype", &iType, sizeof(iType));
				pwndRightControl->GetPropValueT("visible", &bVisible, sizeof(bVisible));
				pwndRightControl->GetPropValueT("enabled", &bEnabled, sizeof(bEnabled));
				
				if ((iTabOrder >= 0) && (iType & tmWS_SHOW) && bVisible && bEnabled)
				{
					m_nTabStop = iTabOrder;
					pwndRightControl->OnGetFocus();
					m_bGetFocus = TRUE;
					break;
				}
				else
				{
					FindNearBig("top", "left", pwndRightControl);
					break;	
				}
			}
			else
			{
				FindNearBig("top", "left", pwnd);
				break;	
			}	
		}
		i++;
	}	
	
}





void  CtmDialog::OnGetFocus(short nTabStop)
{
	/*
	CtmWnd* pwndTemp = NULL;
	if (nTabStop < 0) return;
	
	if (!m_bGetFocus) 
	{
		pwndTemp = FindControlFromTab(nTabStop);
		if (pwndTemp != NULL)
		{
			pwndTemp->OnGetFocus();
			m_bGetFocus = TRUE;
		}
	}
	*/
	
	
}

void  CtmDialog::OnLoseFocus1(short nTabStop)
{
	/* fans mark
	if (m_bGetFocus) m_aitem[m_nTabStop].pwnd->OnLoseFocus(); //BW1204'02 m_bGetFocus judge
	m_bGetFocus = FALSE;
	*/
	
	//	fans add
	CtmWnd* pwndTemp = NULL;
	if (m_bGetFocus)
	{
		pwndTemp = FindControlFromTab(nTabStop);
		if (pwndTemp != NULL)
		{
			pwndTemp->OnLoseFocus();
			m_bGetFocus = FALSE;
			
//			for (int i = 0; i < GetControlCount(); i++)
//			{
//				if (Controls[i]->GetAi() != NULL && Controls[i]->GetAi()->GetDbWnd() == pwndTemp)
//				{
//					Controls[i]->GetAi()->ExeAiOp();
//					Controls[i]->Update();
//				}
//				else if (Controls[i]->GetAi() != NULL)
//				{
//					char szID[128];
//					pwndTemp->GetPropValueT("dbid0", szID);
//					
//					if (szID[0] != '\0')
//					{
//						if (strcmp(szID, Controls[i]->GetAi()->GetDBID())==0)
//						{
//							Controls[i]->GetAi()->ExeAiOp();
//							Controls[i]->Update();
//						}
//					}
//				}
//			}
//			
//			if (pwndTemp->GetAi() != NULL) 
//			{
//				pwndTemp->GetAi()->ExeAiOp();
//				pwndTemp->Update();
//			}
		}
	}
}

