/*==========================================================================+
|  Class    : Class Cursor	                                                |
|  Task     : Class Cursor header file                             			|
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
#ifndef D_DCURSOR
#define	D_DCURSOR

#include "rs232.h"
#include "icon.h"
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagCURSOR
{
	char	*pszName;
}CURSOR;
/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - CIcon                                     	|
+==========================================================================*/
class CCursor : public CObject
{
DECLARE_DYNCREATE(CCursor)
public:
	CCursor();
	~CCursor();
public:
	int		Open(void *pData);
	int		Close();
	
	void    Draw(RECT rect);
	void	Draw(int x, int y);			// �e����
	void    Draw();
	void	Clear(RECT rect);
	void	SetCurHwnd(HWND hWnd);		// �]�m��e���ЩҦb���f
	RECT	GetRect()		{return m_rect;};					// ���o���ЩҦb�x��
	int		GetPointX()		{return m_nX;};
	int		GetPointY()		{return m_nY;};

private:
	HWND	m_hWnd;						// ���ЩҦb���f�y�`
	
	int		m_nX;						// ���ЩҦb����
	int		m_nY;
	
	int		m_nWidth;
	int		m_nHeight;
	RECT	m_rect;
	POINT	m_point;
	
	#ifdef	D_32BIT
	DWORD*  m_pwBG;
	#else
	WORD*   m_pwBG;
	#endif
	int     m_nDrawBG;
	
	CURSOR	m_cursor;
	CIcon*	m_pIcon;	
	
	void    DrawBG(RECT rect);
	void    ReadBG(RECT rect);
	int     CutRect(RECT rectUp, RECT rectDown, RECT *prectNew);
		
};
extern  CCursor			    *g_pCursor;    //netdigger 2007/4/17

extern	int	g_CursorX;
extern	int	g_CursorY;
extern	int	g_CursorChange;

#endif
