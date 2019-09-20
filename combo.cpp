/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*================================================================+
|  Class    : Windows Extension                                   |
|  Task     : Windows Extension Manager                           |
|-----------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                         |
|  Link     : G++(GCC)3.2                                         |
|  Call     :                                                     |
|-----------------------------------------------------------------|
|  Author   : Jeckey                                              |
|  Version  : V1.00                                               |
|  Creation : 10/10/2003                                          |
|  Revision :                                                     |
+================================================================*/

#include	"combo.h"
#include	"utils.h"
#include	"stable.h"

/*============================================================+
|           Class implementation - Combox	                  |
+============================================================*/
/*-------------------------------------------------------------+
|           Constructor and destructor                         |
+-------------------------------------------------------------*/
CtmCombo::CtmCombo(tmWND* pwnd,tmCOMBO* pcombo) : CtmWnd()
{
	/*
	tmWND       wndEdit;
	tmWND       wndList;
	short       nRight;
	short       nBottom;
	
	SetBorderWidth(m_wndattr.wBorderWidthCombo);
	CtmWnd::Create(pwnd);
	
	m_pcombo = pcombo;
	Set1();
	
	nRight  = GetClientRight();
	nBottom = GetClientBottom();
	MKWND(wndEdit, NULL, tmWS_BORDER_INSET, 0, 0,               nRight, m_pcFont->m_wHeight+2-1, m_hwndMe, &g_pFont16)
	MKWND(wndList, NULL, tmWS_BORDER_INSET, 0, m_pcFont->m_wHeight+2+4, nRight, nBottom,         m_hwndMe, &g_pFont16)
	m_pedit = new CtmEdit();//   (&wndEdit, &m_combo.edit);	fans mark
	m_plist = new CtmListBox(&wndList, &m_combo.list);
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
		Show();
	*/
}
CtmCombo::~CtmCombo()
{
	delete  m_pedit;        //wy122999
	delete  m_plist;        //wy122999
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                Get()                                           |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmCombo::Get()				/* Virtual function */
{
	m_pedit->Get();
	m_plist->Get();
	strcpy(m_pcombo->edit.psz, m_sz);              
	m_pcombo->list.nIndex = m_combo.list.nIndex;   
}

void CtmCombo::Set()				/* Virtual function */
{
	Set1();
	m_pedit->Set();
	m_plist->Set();
}
/*---------------------------------------------------------------------------+
|  Function :                Show()                                          |
|  Task     : Create the items and show them                                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void CtmCombo::Show()				/* Virtual function */
{
	m_pedit->DrawBorder();                  // The border need to be redrawed
	m_plist->DrawBorder();                  // The border need to be redrawed
	m_pedit->Show();
	m_plist->Show();
}

void CtmCombo::Update()			/* Virtual function */
{
	if (!m_bOnFocus)
    {
	    Set();
	    Show();
    }
}
/*---------------------------------------------------------------------------+
| Function  : void OnGetFocus()							          |
| Task	  : 					 								|
+----------------------------------------------------------------------------+
|  Call     : 	                                  						|
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

void CtmCombo::OnGetFocus()		/* Virtual function */
{
	m_pedit->OnGetFocus();
	m_bOnFocus = TRUE;
}
/*---------------------------------------------------------------------------+
| Function  : void OnLoseFocus()							          		 |
| Task	  : 					 											 |
+----------------------------------------------------------------------------+
|  Call     : 	                                  							 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

void CtmCombo::OnLoseFocus()		/* Virtual function */
{
	m_pedit->OnLoseFocus();
	m_bOnFocus = FALSE;
}
/*---------------------------------------------------------------------------+
| Function  : WORD OnKey() 								          			 |
| Task	  : deal with the Key press message									 |
+----------------------------------------------------------------------------+
|  Call     : 	                                  							 |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

WORD CtmCombo::OnKey(WORD wKey)		/* Virtual function */
{
	switch (wKey)
	    {
	    case _KEY_UPFIELD:
	    case _KEY_DOWNFIELD:
	    case _UARROW:
	    case _EXT_UARROW:
	    case _DARROW:
	    case _EXT_DARROW:
	    case _PAGEUP:
	    case _EXT_PAGEUP:
	    case _PAGEDN:
	    case _EXT_PAGEDN:
	        wKey = m_plist->OnKey(wKey);
	        m_plist->Get();
	        strcpy(m_combo.edit.psz, m_combo.list.pszList[m_combo.list.nIndex]);
	        m_pedit->Update();
	        break;
	    default:
	        wKey = m_pedit->OnKey(wKey);
	        break;
	    }
	
	return wKey;
}

void CtmCombo::Set1()
{
	m_combo = *(m_pcombo);
	StrNCpy(m_sz, m_combo.edit.psz, m_combo.edit.wMaxChar);
	m_combo.edit.psz = m_sz;                // Update the local copy of edit buffer
}
