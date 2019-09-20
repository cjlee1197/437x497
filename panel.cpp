/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmPanel                                                       |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey		                                                   |
|  Version  : V1.00                                                          |
|  Creation : 17/11/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"panel.h"

/*===========================================================================+
|           Class implementation - CtmPanel	                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd												|
+---------------------------------------------------------------------------*/
CtmPanel::CtmPanel() : CtmWnd()
{
}

CtmPanel::	CtmPanel(tmWND* pwnd, tmPANEL* ppanel) :CtmWnd()
{
	Create(pwnd, ppanel);
}

CtmPanel::	~CtmPanel()
{
}

BOOL        CtmPanel::Create(tmWND* pwnd, tmPANEL* ppanel)
{
	SetBorderWidth(m_wndattr.wBorderWidthPanel);
	CtmWnd::Create(pwnd);
	
	short       i;
	short       nBorderWidthLT;
	short       nBorderWidthRB;
	tmBEVEL*    pbevel=NULL;
	
	for (i=0; i<2; i++)
	    {
	    switch (i)
	        {
	        case 0:
	            pbevel = &(ppanel->bevelOuter);
	            break;
	        case 1:
	            pbevel = &(ppanel->bevelInner);
	            break;
	        }
	    switch (pbevel->wType)
	        {
	        case tmBEVEL_PLAIN:
	        case tmBEVEL_RAISE:
	        case tmBEVEL_INSET:
	            nBorderWidthLT =
	            nBorderWidthRB = pbevel->wWidth + pbevel->wGap;
	            break;
	        case tmBEVEL_SHADOW:
	            nBorderWidthLT = pbevel->wGap;
	            nBorderWidthRB = pbevel->wWidth + pbevel->wGap;
	            break;
	        default:
	            nBorderWidthLT =
	            nBorderWidthRB = 0;
	            break;
	        }
	    CtmWnd::SetClientRECT(nBorderWidthLT, nBorderWidthLT, -nBorderWidthRB, -nBorderWidthRB);
	    }
	SetFGColor(ppanel->fgc);
	SetBGColor(ppanel->bgc);
	
	m_ppanel = ppanel;
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
	    Show();
	
	return TRUE;
}

void        CtmPanel::Show()              /* Virtual function */
{
	short       i;
	short       j;
	short       nX = 0;
	short       nY = 0;
	short       nRight  = m_rectWindow.right - m_rectWindow.left;
	short       nBottom = m_rectWindow.bottom - m_rectWindow.top;
	FGC         fgcLT;
	FGC         fgcRB;
	tmBEVEL*    pbevel=NULL;
	
	_SetViewPort(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
	for (i=0; i<2; i++)
	    {
	    switch (i)
	        {
	        case 0:
	            pbevel = &(m_ppanel->bevelOuter);
	            break;
	        case 1:
	            pbevel = &(m_ppanel->bevelInner);
	            break;
	        }
	    switch (pbevel->wType)
	        {
	        case tmBEVEL_PLAIN:
	            fgcLT = m_wndattr.fgcBorder1;
	            fgcRB = m_wndattr.fgcBorder1;
	            goto _SHOW_DRAWBEVEL;
	        case tmBEVEL_RAISE:
	            fgcLT = m_wndattr.fgcBorder3;
	            fgcRB = m_wndattr.fgcBorder4;
	            goto _SHOW_DRAWBEVEL;
	        case tmBEVEL_INSET:
	            fgcLT = m_wndattr.fgcBorder4;
	            fgcRB = m_wndattr.fgcBorder3;
	
	            _SHOW_DRAWBEVEL:
	            for (j=0; j<(short)pbevel->wWidth; j++)
	                {
	                if (m_ppanel->bTransparent) _SetColor(MAGENTA);
	                else                        _SetColor(fgcLT);
	                _MoveTo(    nX+j, nBottom-j);
	                _LineTo(    nX+j,      nY+j);
	                _LineTo(nRight-j,      nY+j);
	                if (!(m_ppanel->bTransparent)) _SetColor(fgcRB);
	                _LineTo(nRight-j, nBottom-j);
	                _LineTo(  1+nX+j, nBottom-j);
	
	                if (m_ppanel->bTransparent)
	                    {
	                    _MoveTo(    nX+j+1, nBottom-j-1);
	                    _LineTo(    nX+j+1,      nY+j+1);
	                    _LineTo(nRight-j-1,      nY+j+1);
	                    _LineTo(nRight-j-1, nBottom-j-1);
	                    _LineTo(  1+nX+j+1, nBottom-j-1);
	    
	                    _MoveTo(    nX+j+2, nBottom-j-2);
	                    _LineTo(    nX+j+2,      nY+j+2);
	                    _LineTo(nRight-j-2,      nY+j+2);
	                    _LineTo(nRight-j-2, nBottom-j-2);
	                    _LineTo(  1+nX+j+2, nBottom-j-2);
	                    }
	                }
	            nX += pbevel->wWidth;
	            nY += pbevel->wWidth;
	            nRight  -= pbevel->wWidth;
	            nBottom -= pbevel->wWidth;
	            break;
	        case tmBEVEL_SHADOW:
	            if (m_ppanel->bTransparent) _SetColor(BLUE);
	            else                        _SetColor(m_wndattr.fgcShadow);
	            _Rectangle(_GFILLINTERIOR, nRight -pbevel->wWidth+1, nY+pbevel->wWidth, nRight, nBottom);
	            _Rectangle(_GFILLINTERIOR, nX+pbevel->wWidth, nBottom-pbevel->wWidth+1, nRight, nBottom);
	            nRight  -= pbevel->wWidth;
	            nBottom -= pbevel->wWidth;
	            break;
	        default:
	            break;
	        }
	    if (!(m_ppanel->bTransparent))
	        {
	        _SetColor(m_bgc);
	        _Rectangle(_GFILLINTERIOR, nX, nY, nRight, nBottom);
	        }
	    nX += pbevel->wGap;
	    nY += pbevel->wGap;
	    nRight  -= pbevel->wGap;
	    nBottom -= pbevel->wGap;
	    }
}
