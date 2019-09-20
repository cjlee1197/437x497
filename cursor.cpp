/*==========================================================================+
|  Class    : Class Cursor	                                                |
|  Task     : Class Cursor file                             				|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/12/21                                                    |
|  Revision : 2006/12/21                                                    |
+==========================================================================*/
#include "cursor.h"
#include "gui.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define RECTAREA(rect)  ((rect.right - rect.left + 1) * \
                        (rect.bottom - rect.top + 1))
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
int	g_CursorX = 0;
int	g_CursorY = 0;
int	g_CursorChange = 0;
/*==========================================================================+
|           Class implementation - CCursor                                  |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/


CCursor			*g_pCursor	= NULL;    //netdigger 2007/4/17
IMPLEMENT_DYNCREATE(CCursor, CObject)



CCursor::CCursor()
{
	m_pIcon	= NULL;
	m_pwBG  = NULL;
	m_nDrawBG   = 0;
}

CCursor::~CCursor()
{
	Close();
	if(m_pwBG != NULL)   free(m_pwBG);
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
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
int		CCursor::Open(void *pData)
{
	ICON	icon;
	
	m_cursor	= *(CURSOR *)pData;
	
	if(m_cursor.pszName == NULL) return -1;
		
	icon.pszName	= m_cursor.pszName;
	m_pIcon			= new CIcon;
	m_pIcon->Create(&icon);
	
	m_nWidth	= m_pIcon->GetWidth();
	m_nHeight	= m_pIcon->GetHeight();
	
	m_point		= m_pIcon->GetPoint();
	
	m_nX    = 399;
	m_nY    = 299;
	
	m_rect.left		= m_nX - m_point.nX;
	m_rect.right	= m_nX - m_point.nX + m_nWidth - 1;
	m_rect.top		= m_nY - m_point.nY;
	m_rect.bottom	= m_nY - m_point.nY + m_nHeight - 1;
	
	if(m_pwBG != NULL)   free(m_pwBG);
	   
	#ifdef	D_32BIT
	m_pwBG   = (DWORD *)malloc(m_nWidth * m_nHeight * sizeof(DWORD));
	#else
	m_pwBG   = (WORD *)malloc(m_nWidth * m_nHeight * sizeof(WORD));
	#endif
	
	return 0;
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
int		CCursor::Close()
{
	if(m_pIcon != NULL)
	{
		delete m_pIcon;
		m_pIcon	= NULL;
	}
	
	return 0;
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
void	CCursor::Draw()
{
	if(m_pIcon == NULL) 	return;

//	_SetClipRgn(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
//	_SetViewPort(m_nX - m_point.nX, m_nY - m_point.nY, 
//						m_nX - m_point.nX + m_nWidth, m_nY - m_point.nY + m_nHeight);
    _SetViewPort(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	m_pIcon->Draw(0, 0);	
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
void	CCursor::Draw(int x, int y)
{
	if(m_pIcon == NULL) 	return;
	
	m_nX	= x;
	m_nY	= y;
	
//	printf("CCursor::Draw(%d, %d)\n", x, y);

	DrawBG(m_rect);
	
	m_rect.left		= m_nX - m_point.nX;
	m_rect.right	= m_nX - m_point.nX + m_nWidth - 1;
	m_rect.top		= m_nY - m_point.nY;
	m_rect.bottom	= m_nY - m_point.nY + m_nHeight - 1;
	
	if(m_rect.left < 0) m_rect.left = 0;
	if(m_rect.top < 0) m_rect.top = 0;

	if(m_rect.right > 599) m_rect.right = 598;
	if(m_rect.bottom > 1024) m_rect.bottom = 1022;
	
	ReadBG(m_rect);
	
	Draw();	
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
void	CCursor::Clear(RECT rect)
{
    RECT    rectNew;
    if(CutRect(m_rect, rect, &rectNew) == 0) return;
    
    DrawBG(rectNew);
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
void    CCursor::Draw(RECT rect)
{
    RECT    rectNew;
    if(CutRect(m_rect, rect, &rectNew) == 0) return;
//    printf("CCursor::Draw(rect) %d, %d, %d, %d\n", rect.left, rect.top, rect.right, rect.bottom);    
//    printf("CCursor::Draw(rect) %d, %d, %d, %d", rectNew.left, rectNew.top, rectNew.right, rectNew.bottom);
    ReadBG(rectNew);
//    Draw(); 

    if(m_pIcon != NULL)     
        m_pIcon->Draw(rectNew.left - m_rect.left, rectNew.top - m_rect.top,
                rectNew.right - rectNew.left + 1, rectNew.bottom - rectNew.top + 1);   
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
void    CCursor::DrawBG(RECT rect)
{
    int i;
    if(m_nDrawBG == 0)  return;
        
    int nWidth  = rect.right - rect.left + 1;
    int nHeight = rect.bottom - rect.top + 1;
    int nLeft   = rect.left - m_rect.left;
    int nTop    = rect.top - m_rect.top;

    _SetViewPort(rect.left, rect.top, rect.right, rect.bottom);
//    printf("CCursor::DrawBG(rect) %d, %d, %d, %d top = %d, nHeight = %d\n", rect.left, rect.top, rect.right, rect.bottom, nTop, nHeight);
    
    nHeight = (g_Length_Y > nHeight) ? nHeight : g_Length_Y;
    nWidth  = (g_Length_X > nWidth) ? nWidth : g_Length_X;
    
    for(i = 0; i < nHeight; i ++)
    {
        #ifdef	D_32BIT
        memcpy(g_Rows[i], m_pwBG + nLeft + (i + nTop) * m_nWidth, 4 * nWidth);
        #else
        memcpy(g_Rows[i], m_pwBG + nLeft + (i + nTop) * m_nWidth, 2 * nWidth);
        #endif
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
void CCursor::ReadBG(RECT rect)
{
    int i;
    int nWidth  = rect.right - rect.left + 1;
    int nHeight = rect.bottom - rect.top + 1;
    int nLeft   = rect.left - m_rect.left;
    int nTop    = rect.top - m_rect.top;
//    printf("CCursor::ReadBG(rect) %d, %d, %d, %d top = %d nHeight = %d\n", rect.left, rect.top, rect.right, rect.bottom, nTop, nHeight);    
    _SetViewPort(rect.left, rect.top, rect.right, rect.bottom);
    
    nHeight = (g_Length_Y > nHeight) ? nHeight : g_Length_Y;
    nWidth  = (g_Length_X > nWidth) ? nWidth : g_Length_X;
    
    for(i = 0; i < nHeight; i ++)
    {
        #ifdef	D_32BIT
        memcpy(m_pwBG + nLeft + (i + nTop) * m_nWidth,  g_Rows[i], 4 * nWidth);
        #else
        memcpy(m_pwBG + nLeft + (i + nTop) * m_nWidth,  g_Rows[i], 2 * nWidth);
        #endif
    }    
    m_nDrawBG   = 1;
}

/*--------------------------------------------------------------------------+
|  Function :                                                               |
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int     CCursor::CutRect(RECT rectUp, RECT rectDown, RECT *prectNew)
{
    int     nArea   = 0;
    
    prectNew->left    = (rectUp.left > rectDown.left) ? rectUp.left:rectDown.left;
    prectNew->top     = (rectUp.top > rectDown.top)   ? rectUp.top :rectDown.top;
    prectNew->right   = (rectUp.right < rectDown.right)   ? rectUp.right :rectDown.right;
    prectNew->bottom  = (rectUp.bottom < rectDown.bottom) ? rectUp.bottom:rectDown.bottom;
    
    if(prectNew->left <= prectNew->right && prectNew->top <= prectNew->bottom)
        nArea   = RECTAREA((*prectNew));      
    
    return nArea;
}

