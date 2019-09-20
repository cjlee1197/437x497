/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager file		                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey		                                                   |
|  Version  : V1.00                                                          |
|  Creation : 29/10/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"groupbox.h"
#include	"utils.h"

/*===========================================================================+
|           Extent variable                                                  |
+===========================================================================*/
/*===========================================================================+
|           Class implementation - OptionBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmGroupBox:: CtmGroupBox(tmWND* pwnd,tmGROUP* pgroup) :CtmWnd()
{
	SetBorderWidth(m_wndattr.wBorderWidthGroupBox);
	CtmWnd::Create(pwnd);
	
	m_pgroup = pgroup;
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
	    Show();
}

CtmGroupBox:: ~CtmGroupBox()
{
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	CtmGroupBox::Show()
{
	tmSIZE        size = GetStringExtent(m_pgroup->psz);
	RECT        rect;
	short       nX;
	short       nY = size.h >> 1;
	short       nClientRight = GetClientRight();
	short       nClientBottom= GetClientBottom();
	
	size.w += 16;                           // Surround the string with leading and tailing blank
	switch (m_pgroup->wType)
	    { 
	    case tmGROUP_TOPLEFT:
	        MKRECT(rect, 8, 0, 8+size.w-1, size.h-1)
	        nX = 16;
	        break;
	    case tmGROUP_TOPCENTER:
	        nX = (GetClientWidth() - size.w - 4/*Frame*/) >> 1;
	        MKRECT(rect, nX, 0, nX+size.w-1, size.h-1)
	        nX += 8;
	        break;
	    case tmGROUP_TOPRIGHT:
	    default:
	        nX = GetClientWidth() - size.w - 8;
	        MKRECT(rect, nX, 0, nX+size.w-1, size.h-1)
	        nX += 8;
	        break;
	    };
	
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	_SetColor(m_wndattr.fgcBorder4);
	_MoveTo(           0, nClientBottom);
	_LineTo(           0,          nY-1);
	_LineTo(nClientRight,          nY-1);
	_SetColor(m_wndattr.fgcBorder3);
	_LineTo(nClientRight, nClientBottom);
	_LineTo(           1, nClientBottom);
	_SetColor(m_wndattr.fgcBorder3);
	_MoveTo(             1, nClientBottom-1);
	_LineTo(             1,              nY);
	_LineTo(nClientRight-1,              nY);
	_SetColor(m_wndattr.fgcBorder4);
	_LineTo(nClientRight-1, nClientBottom-1);
	_LineTo(             2, nClientBottom-1);
	
	CtmWnd::Clear(rect);
	CtmWnd::Show(nX, 0, m_pgroup->psz);
}
