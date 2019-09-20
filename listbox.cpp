/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : ListBox                                                        |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey                                                         |
|  Version  : V1.00                                                          |
|  Creation : 04/11/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include 	"listbox.h"
#include 	"common.h"
#include	"utils.h"

/*----------------------------------------------------------------------------+
|               Constants											 		  |
+----------------------------------------------------------------------------*/

/*===========================================================================+
|           Class implementation - ScrollBar	                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd															|
|           nMaxline the total lines				    			     	 |
|           nCount the length of the list                                    |
+---------------------------------------------------------------------------*/
CtmListBox::CtmListBox(tmWND* pwnd,tmLIST* plist) : CtmWnd()
{
	SetBorderWidth(m_wndattr.wBorderWidthListBox);
	SetBGColor(m_wndattr.bgcListBox1);
	CtmWnd::Create(pwnd);
		
	SetFGColor(BLACK);
	m_plist = plist;
	m_nPageSize = (GetClientHeight()+2) / (m_pcFont->m_wHeight+2);
	m_pScrollBar = NULL;
	Set();
	
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
	    Show();
}

CtmListBox::~CtmListBox()
{
	if (m_pScrollBar != NULL) delete m_pScrollBar;
}

/*---------------------------------------------------------------------------+
|  				Operations													 |
+---------------------------------------------------------------------------*/
void        CtmListBox::Set()             /* Virtual function */
{
	//tmWND         wnd;
	//
	//m_nIndex = (m_plist->nIndex < m_plist->nCount) ? m_plist->nIndex : (m_plist->nCount - 1);
	//m_nStart = (m_nIndex < m_nPageSize) ? 0 : (m_nIndex - m_nPageSize + 1);
	//
	//if (m_plist->nCount > m_nPageSize)
	//    {
	//    if (m_pScrollBar == NULL)
	//        {
	//        m_scrollbar.wType = tmSCROLLBAR_V;
	//        MKWND(wnd, NULL, tmWS_BORDER_NONE, GetClientRight()-16+1, 0, GetClientRight(), GetClientBottom(), m_hwndMe, &m_pcFont)
	//        m_pScrollBar = new CtmScrollBar(&wnd, &m_scrollbar);
	//        CtmWnd::SetClientRECT(0, 0, -16, 0);
	//        }
	//    }
	//else
	//    {
	//    if (m_pScrollBar != NULL)
	//        {
	//        delete m_pScrollBar;
	//        CtmWnd::SetClientRECT(0, 0, +16, 0);
	//        MKRECT(wnd.rect, GetClientRight()-16+1, 0, GetClientRight(), GetClientBottom())
	//        Clear(wnd.rect);
	//        m_pScrollBar = NULL;      // wy110999 add for width-expand no limit.
	//        }
	//    }
}

void        CtmListBox::Show()            /* Virtual function */
{
	short       i;
	short       nCount = ((m_plist->nCount-m_nStart) > m_nPageSize) ? m_nPageSize : (m_plist->nCount-m_nStart);
	short       nYGap = m_pcFont->m_wHeight+2;
	
	if (m_pScrollBar != NULL)
	    m_pScrollBar->Show();
	
	Clear();
	for (i=0; i<nCount; i++)
	    CtmWnd::Show(1/*For align with edit box*/, nYGap*i, m_plist->pszList[m_nStart+i]);
	
	if (m_nIndex >= 0) Select(m_nIndex);
}

void        CtmListBox::Update()          /* Virtual function */
{
	Set();
	Show();
}

void        CtmListBox::OnGetFocus()      /* Virtual function */
{
	CtmWnd::DrawGetFocus();
	::Prompt(m_plist->pszPrompt);
}

void        CtmListBox::OnLoseFocus()     /* Virtual function */
{
	CtmWnd::DrawLoseFocus();
}

WORD        CtmListBox::OnKey(WORD wKey)  /* Virtual function */
{
	short       nIndex = m_nIndex;
	short       nDelta;
	
	switch (wKey)
	    {
	/**/case _KEY_UPFIELD:
	    case _UARROW:
	    case _EXT_UARROW:
	        if (nIndex > 0)
	            {
	            if ((--nIndex) < m_nStart)
	                {
	                Scroll(-1);
	                m_nIndex--;             // Update m_nIndex to avoid clear again
	                }
	            Select(nIndex);
	            }
	        else if (nIndex < 0) 
	            Select(0);
	        break;
	/**/case _KEY_DOWNFIELD:
	    case _DARROW:
	    case _EXT_DARROW:
	        if (nIndex < (m_plist->nCount-1))
	            {
	            if ((++nIndex) >= (m_nStart+m_nPageSize))
	                {
	                Scroll(+1);
	                m_nIndex++;             // Update m_nIndex to avoid clear again
	                }
	            Select(nIndex);
	            }
	        break;
	    case _PAGEUP:
	    case _EXT_PAGEUP:
	        if (nIndex > 0)
	            {
	            nDelta = nIndex - m_nStart - (m_nPageSize - 1);
	            if (nDelta < 0) Scroll(nDelta);
	            Select(m_nStart);
	            }
	        else if (nIndex < 0)
	            Select(0);
	        break;
	    case _PAGEDN:
	    case _EXT_PAGEDN:
	        if (nIndex < (m_plist->nCount-1))
	            {
	            nDelta = nIndex - m_nStart;
	            if (nDelta > 0) Scroll(nDelta);
	            Select(((m_nStart+m_nPageSize) > m_plist->nCount) ? (m_plist->nCount-1) : (m_nStart+m_nPageSize-1));
	            }
	        break;
	    case _ESC:
	        Select(m_plist->nIndex);
	        return wKey;
	    default:
	        return wKey;
	    }
	
	return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void  CtmListBox::Select(short nIndex)
{
	RECT        rect;
	short       nYGap = m_pcFont->m_wHeight+2;
	FGC         fgcOld = GetFGColor();
	BGC         bgcOld = GetBGColor();
	
	rect.left   = 0;
	rect.right  = GetClientRight();
	
	if ((m_nIndex >= m_nStart) && (m_nIndex < (m_nStart + m_nPageSize)) && (m_nIndex != nIndex))
	    {
	    rect.top    = (m_nIndex - m_nStart) * nYGap;
	    rect.bottom = rect.top + m_pcFont->m_wHeight - 1;
	    Clear(rect);
	    CtmWnd::Show(1, rect.top, m_plist->pszList[m_nIndex]);
	    }
	
	if (nIndex < m_nStart)
	    Scroll(nIndex - m_nStart);
	else if (nIndex >= (m_nStart + m_nPageSize))
	    Scroll(nIndex - (m_nStart + m_nPageSize - 1));
	
	rect.top    = (nIndex - m_nStart) * nYGap;
	rect.bottom = rect.top + m_pcFont->m_wHeight - 1;
	SetFGColor(m_wndattr.fgcListBox2);
	SetBGColor(m_wndattr.bgcListBox2);
	Clear(rect);
	CtmWnd::Show(1, rect.top, m_plist->pszList[nIndex]);
	SetFGColor(fgcOld);
	SetBGColor(bgcOld);
	
	m_nIndex = nIndex;
}

void  CtmListBox::Scroll(short nDelta)
{
	short       i;
	short       nCount;
	short       nYGap = m_pcFont->m_wHeight+2;
	
	m_nStart += nDelta;
	if (m_nStart > (m_plist->nCount - m_nPageSize)) m_nStart = m_plist->nCount - m_nPageSize;
	if (m_nStart < 0) m_nStart = 0;
	nCount = ((m_plist->nCount-m_nStart) > m_nPageSize) ? m_nPageSize : (m_plist->nCount-m_nStart);
	Clear();
	for (i=0; i<nCount; i++)
	    CtmWnd::Show(1, nYGap*i, m_plist->pszList[m_nStart+i]);
}

