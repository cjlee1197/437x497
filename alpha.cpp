/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
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
#include	"alpha.h"
#include	"utils.h"

/*===========================================================================+
|           Class implementation - Alpha bar                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmAlpha::CtmAlpha(tmWND* pwnd) : CtmWnd()
{
	CtmWnd::Create(pwnd);
	m_nIndex = 0;
	m_fgcOld = m_fgc;
	m_bgcOld = m_bgc;
	
	Redraw(m_rectClient);
}

CtmAlpha::~CtmAlpha()
{
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
void        CtmAlpha::Redraw(RECT rect)
{
	short       i;
	char        psz[] = "A";
	
	DrawBorder();
	Clear();
	for (i=0; i<26; i++)
	    {
	    Show(i * (8/*Character*/+2/*Gap*/), 0, psz);
	    (*psz)++;
	    }
	
	Select(0);
}

WORD        CtmAlpha::OnKey(WORD wKey)
{
switch (wKey)
    {
/**/case _KEY_LEFTFIELD:
    case _LARROW:
    case _EXT_LARROW:
        Select(-1);
        break;
/**/case _KEY_RIGHTFIELD:
    case _RARROW:
    case _EXT_RARROW:
        Select(+1);
        break;
    case _KEY_SELECT:
        ::PutKeyTail((WORD)('A' + (char)m_nIndex));
        break;
    default:
        return wKey;
    }

return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void		CtmAlpha::Select(short nDelta)
{
	char        psz[] = "A";
	
	*psz  = 'A' + (char)m_nIndex;
	ShowExtent(m_nIndex * (8+2), 0, psz);
	
	m_nIndex += nDelta;
	if      (m_nIndex < 0)  m_nIndex = 25;
	else if (m_nIndex > 25) m_nIndex = 0;
	
	*psz  = 'A' + (char)m_nIndex;
	m_fgc = BRIGHTWHITE;
	m_bgc = BLUE;
	ShowExtent(m_nIndex * (8+2), 0, psz);
	m_fgc = m_fgcOld;
	m_bgc = m_bgcOld;
}
