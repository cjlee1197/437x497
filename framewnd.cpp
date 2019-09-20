/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : FrameWnd                                                       |
|  Task     : Frame window file                                              |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : netdigger                                                      |
|  Version  : V1.00                                                          |
|  Creation : 2003/09/10                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"framewnd.h"

CtmFrameWnd*	CtmFrameWnd::m_pFirst = NULL;
RECT	g_topRect;

CtmFrameWnd::CtmFrameWnd():CtmWnd()
{
	m_pNext 	= m_pFirst;
	m_pFirst 	= this;
	m_hwndGUI	= HWND_INVALID;
}

CtmFrameWnd::~CtmFrameWnd()
{
	if(m_hwndGUI != HWND_INVALID)		
		SendMessage(m_hwndGUI,MSG_CLOSE,0,0);
	if(m_pFirst == this)
	{
		m_pFirst = m_pNext;
		return;
	}
	
	for(CtmFrameWnd *pWnd = m_pFirst; m_pFirst!=NULL; pWnd = pWnd->m_pNext)
	{
		if(pWnd->m_pNext == this)
		{
			pWnd->m_pNext = m_pNext;
			return ;
		}
	}	

}

void	CtmFrameWnd::Clear(RECT rect)
{
	HDC hdc = GetDC(m_hwndGUI);
	SetBrushColor(hdc,m_bgc);
	FillBox(hdc,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
	ReleaseDC(hdc);
}
int	CtmFrameWnd::Create(tmWND* pwnd)
{
	MAINWINCREATE CreateInfo;
	
	m_wnd = *pwnd; 
	g_topRect = m_wnd.rect;	
	
	InitCreateInfo (&CreateInfo);		
	m_hwndGUI = CreateMainWindow (&CreateInfo);
		
	if (m_hwndGUI != HWND_INVALID) 
	{	
        ShowWindow (m_hwndGUI, SW_SHOWNORMAL);		
        return 0;
    }
    
	return -1;
}

int CtmFrameWnd::WndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
	{
        case MSG_CREATE:			
			::PostMessage(hWnd,tmMSG_CREATE,0,0);			
        	break;		
		case MSG_TIMER:
		case MSG_CHAR:
			//PostMessage(hWnd,MSG_CLOSE,0,0);
			break;
		case MSG_PAINT:			
			break;
        case MSG_CLOSE:
        	
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            m_hwndGUI = HWND_INVALID;
			break;
       	return 0;
    }
	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int	CtmFrameWnd::FrameWndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	for(CtmFrameWnd *pWnd = m_pFirst; pWnd!=NULL; pWnd = pWnd->m_pNext)
	{		
		if( pWnd->m_hwndGUI == hWnd)
		{
			return	pWnd->WndProc(hWnd,message,wParam,lParam);
		}
	}
	
	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void CtmFrameWnd::InitCreateInfo(PMAINWINCREATE pCreateInfo)
{	
	pCreateInfo->dwStyle = WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_TRANSPARENT|WS_EX_TOPMOST;
    pCreateInfo->spCaption = "MMI";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(11);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = FrameWndProc;
    pCreateInfo->lx = m_wnd.rect.left; 
    pCreateInfo->ty = m_wnd.rect.top;
    pCreateInfo->rx = m_wnd.rect.right;
    pCreateInfo->by = m_wnd.rect.bottom;
    pCreateInfo->iBkColor = m_wndattr.bgc;
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = m_wnd.hwndGUI;	

}
