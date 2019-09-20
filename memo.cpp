/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : ScrollBar                                                      |
|  Task     : Windows Extension Manager                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                         |
|  Version  : V1.00                                                          |
|  Creation : 20/10/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"memo.h"
#include	"tmconfig.h"
#include 	"taskcnc.h" //Mario debug
#include    "utils.h"	//Mario debug

/*----------------------------------------------------------------------------+
|               Constants											 |
+----------------------------------------------------------------------------*/


/*===========================================================================+
|           Class implementation - ScrollBar	                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
|----------------------------------------------------------------------------|
|Parameter: pwnd												|
|           nMaxline the total lines				    			     |
|           nCount the length of the list                                    |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmMemo, CtmWnd)
IMPLEMENT_DYNCREATE(CtmCustomMemo, CtmWnd)			//Mario debug
IMPLEMENT_DYNCREATE(CtmTextMemo, CtmCustomMemo)		//Mario debug
//IMPLEMENT_DYNCREATE(CtmLTextMemo, CtmWnd)			//Mario debug
IMPLEMENT_DYNCREATE(CtmMTextMemo, CtmWnd)			//Mario debug


CtmMemo::CtmMemo():CtmWnd()
{
	strcpy(TYPEName, "CtmMemo");
	
	memset(m_pszShow, 	0, sizeof(m_pszShow));
	memset(m_pszIDShow, 0, sizeof(m_pszIDShow));	
	memset(m_pszStream, 0, sizeof(m_pszStream));
	
	memset(m_pszImagePath, 0, sizeof(m_pszImagePath));
	memset(m_pImages, 0, sizeof(m_pImages));
	
	for(int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		m_wszStreamLen[i] = 0;
		m_wStreamType[i] = 0;
	}
	m_wStartStrIndex 	= 0;
	m_bCopyBg			= FALSE;
	m_bFocus			= TRUE;
	m_wFocusColor		= 0x2008;
	m_wFocusIndex		= 0;
	m_wOldFocusIndex	= m_wFocusIndex;
	m_wFocusStrIndex	= m_wFocusIndex;
	m_bBar				= TRUE;
		
	m_wMemType			= MEMO_TYPE_PLAIN_TEXT;
	m_dwKeyColor		= 0;
	
	m_nEntWidth			= 0;
	m_nEntHeight		= 0;
	m_bCalcSize			= FALSE;
	MoveButtonDownFalg = FALSE;//Maril add 2015/1/20 ¤W¤È 11:35:55
	Down_Y = 0; 		//Mario add 2015/1/20 ¤W¤È 11:37:16
}

CtmMemo::~CtmMemo()
{
	m_scrollbar.SetParent(NULL);
	DelImages();
}


void    CtmMemo::DrawBorder()
{
	
	short nRight  = m_rectWindow.right  - m_rectWindow.left;
	short nBottom = m_rectWindow.bottom - m_rectWindow.top;
	g_MemoCurCoordinate.nTop	=	m_rectWindow.top;
	g_MemoCurCoordinate.nBottom	=	m_rectWindow.bottom;
	g_MemoCurCoordinate.nRight	=	m_rectWindow.right ;
	g_MemoCurCoordinate.nLeft	=	m_rectWindow.left;	
	_SetViewPort(	m_rectWindow.left, 
					m_rectWindow.top, 
					m_rectWindow.right, 
					m_rectWindow.bottom
				);
	 _SetColor(0);
   
   
    _Rectangle(_GBORDER, 0, 0, nRight, nBottom);

    _SetColor(m_bgc);
    for (int i=1; (WORD) i< (m_wBorderWidth); i++)
    {
    	
        _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
    }
}

BOOL 	CtmMemo::CreateA()
{
	CtmWnd::CreateA();
	SetInitScrollbar();
}

WORD	CtmMemo::GetMaxRows()
{
	return (m_wnd.rect.bottom - m_wnd.rect.top) / m_nEntHeight;
}


WORD	CtmMemo::GetStrsNum()
{
	WORD	wResult = 0;
	GetShowText();
	for (int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		if (m_pszShow[i] != NULL)
			wResult++;
		if (m_pszStream[i] != NULL)
			wResult++;
	}
	return wResult;
}

void	CtmMemo::GetShowText()
{
	for (int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		if(m_iFontUNType > 0)
		{
			if(m_pszIDShow[i] != NULL)
			{
				//J.Wong add for freetype 2015/5/28 9:43:55
				if(m_pszShow[i]!=NULL)
				{
					free(m_pszShow[i]);
					m_pszShow[i]=NULL;	
				}
				m_pszShow[i]=(char*)malloc(strlen(m_pszIDShow[i])+1);
				memcpy(m_pszShow[i],m_pszIDShow[i],strlen(m_pszIDShow[i])+1);
				//if(m_pszIDShow[i] != NULL)	StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
			    //printf("i=%d str1=%s str2=%s\n", i, m_pszIDShow[i],m_pszShow[i]);
			}
		}
		else
		{
			if(m_pszIDShow[i] != NULL)	StrIDToStr(m_pszIDShow[i], &m_pszShow[i]);
		}
	}
}


void	CtmMemo::SetFocusRow(WORD wOldFocusIndex, WORD wNewFocusIndex)
{
	
	_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
	_SetColor((short)m_bgc);
	_Rectangle(_GFILLINTERIOR, 0, wOldFocusIndex * m_nEntHeight, 
			GetClientRight(), (wOldFocusIndex + 1) * m_nEntHeight);
	
	_SetColor((short)m_wFocusColor);
	_Rectangle(_GFILLINTERIOR, 0, wNewFocusIndex * m_nEntHeight, 
			GetClientRight(), (wNewFocusIndex + 1) * m_nEntHeight);
	
}


void	CtmMemo::DelImages()
{
	int i;
	
	for (i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		if (m_pImages[i])
		{
			m_pImages[i]->SetParent(NULL);
			delete m_pImages[i];
			m_pImages[i] = NULL;
		}
	}
}

CtmBitMap*	CtmMemo::GetBitMap(char *imagePath)
{
	int i = 0;
	bool bFind = FALSE;
	CtmBitMap *pBitMap = NULL;
	
	while (g_aIniBmp[i].cBMPPath[0] != '\0')
	{
		if (strcmp(g_aIniBmp[i].cBMPPath, imagePath) == 0)
		{
			pBitMap = g_aIniBmp[i].pIniBmp; 
			bFind = TRUE;
			
			break;
		}
		i++;
	}
	
	if (!bFind)
	{
		g_aIniBmp[i].pIniBmp = new CtmBitMap;
		g_aIniBmp[i].pIniBmp->Create(imagePath);
		strncpy(g_aIniBmp[i].cBMPPath, imagePath, 256);

		pBitMap = g_aIniBmp[i].pIniBmp; 
	}
	
	return pBitMap;
}

void	CtmMemo::CalcSize()
{
	int nWidth = 5;
	int nHeight = 0;
	CtmBitMap *pBitMap = NULL;
	
	if (m_bCalcSize)
		return;
	
	for (int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		if (m_pszImagePath[i] != NULL)
		{
			pBitMap = GetBitMap(m_pszImagePath[i]);
			
			if (nWidth < pBitMap->GetWidth())
				nWidth = pBitMap->GetWidth();
			if (nHeight < pBitMap->GetHeight())
				nHeight = pBitMap->GetHeight();
		}
	}
	
	if (nHeight < MEMO_UNIT_HEIGHT)
		nHeight = MEMO_UNIT_HEIGHT;
	
	m_nEntWidth = nWidth;
	m_nEntHeight = nHeight;
	
	m_bCalcSize = TRUE;
}

void	CtmMemo::ShowImages()
{
	int j 	= 0;
	int nY 	= 0;
	int	k	= 0;
	int nPadding;
	CtmBitMap *pBitMap = NULL;
	int		nHeight = 0;
	
	if (!(m_wMemType & MEMO_TYPE_IMAGE))
		return;
	
	DelImages();
	
	CalcSize();
	
	for (int i = m_wStartStrIndex; i < CNT_INT_MEMO_MAX; i++)
	{
		if (m_pszImagePath[i] != NULL)
		{
			
			pBitMap = GetBitMap(m_pszImagePath[i]);
			nHeight = pBitMap->GetHeight();
			
			nPadding = (m_nEntHeight - nHeight) / 2;
			
			nY	= j * (m_nEntHeight);
			
			if (nY + m_nEntHeight > m_wnd.rect.bottom - m_wnd.rect.top)
				break;
			else
			{
				if (m_pImages[i] == NULL)
				{
					m_pImages[i] = new CtmImageBox();
					m_pImages[i]->SetPropertys();
					
					m_pImages[i]->SetParent(this);
					
					m_pImages[i]->SetPropValueT("style", 3);
					m_pImages[i]->SetPropValueT("type", 2);
					m_pImages[i]->SetPropValueT("wtype", 0x30);
					m_pImages[i]->SetPropValueT("keycolor", m_dwKeyColor);
					m_pImages[i]->SetPropValueT("left", (double)0);
					m_pImages[i]->SetPropValueT("right", m_nEntWidth);
					m_pImages[i]->SetPropValueT("top", nY + nPadding);
					m_pImages[i]->SetPropValueT("bottom", nY + nPadding + m_nEntHeight);
				
					m_pImages[i]->CreateA();
					
					m_pImages[i]->SetPropValueT("imagepath", m_pszImagePath[i]);
					m_pImages[i]->Update();
				}
			}
			
			j++;
		}
		else 
			k++;
	}
}

void	CtmMemo::ShowStrs(WORD wOldFocusIndex, WORD wNewFocusIndex)
{
	int j 	= 0;
	int nY 	= 0;
	int	k	= 0;
	int nLeft = 5;
	
	CalcSize();
	if (m_wMemType & MEMO_TYPE_IMAGE)
		nLeft = m_nEntWidth;
	
	SetFocusRow(wOldFocusIndex, wNewFocusIndex);
	for (int i = m_wStartStrIndex; i < CNT_INT_MEMO_MAX; i++)
	{
		if (m_pszShow[i] != NULL || m_pszStream[i] != NULL)
		{
			nY	= j * m_nEntHeight; 
			if (nY + m_nEntHeight > m_wnd.rect.bottom - m_wnd.rect.top)	
				break;
			else
			{	
				//printf("i=%d, StrIndex=%d, focusIndex=%d\n", i, m_wStartStrIndex, wNewFocusIndex);
				if (i - m_wStartStrIndex - k == wNewFocusIndex)
				{
					int nOldColor;
					nOldColor 	= m_fgc;					
					m_fgc 		= (m_wFocusColor ^ 0xFFFF); 
					
					if(m_wszStreamLen[i] == 0 && m_pszStream[i] == NULL && m_wMemType & MEMO_TYPE_PLAIN_TEXT)
					{
						//printf("m_pszShow[i]=%s nLeft=%d nY=%d\n", m_pszShow[i], nLeft, nY+5);
						CtmWnd::ShowExtent(nLeft, nY + 5, m_pszShow[i]);
					}
					else
						CtmWnd::ShowFontStream(1, nY, m_pszStream[i], m_wszStreamLen[i], m_wStreamType[i]);
					
					m_wFocusStrIndex = i;
					m_fgc		= nOldColor;
				}
				else
				{
					//printf("m_wszStreamLen[i] is %d\n", m_wszStreamLen[i]);
					if(m_wszStreamLen[i] == 0 && m_pszStream[i] == NULL && m_wMemType & MEMO_TYPE_PLAIN_TEXT)
					{
						//printf("2 m_pszShow[i]=%s \n", m_pszShow[i]);
						CtmWnd::ShowExtent(nLeft, nY + 5, m_pszShow[i]);//CtmWnd::ShowExtent(5, nY + 5, m_pszShow[i]);
					}
					else
					{
						CtmWnd::ShowFontStream(1, nY, m_pszStream[i], m_wszStreamLen[i], m_wStreamType[i]);
					}
				}
			}
			j++;
		}
		else k++;
	}

	ShowScrollbar();
	ShowImages();
	ChangePosNow(GetClientRECT());
}


void CtmMemo::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("focuscolor", 		tmWORD, &m_wFocusColor);
	SetPropInfo("startstrindex", 	tmWORD, &m_wStartStrIndex);
	SetPropInfo("foucusindex",		tmWORD, &m_wFocusIndex);
	SetPropInfo("foucusstrIndex",	tmWORD, &m_wFocusStrIndex);
	SetPropInfo("bar",				tmBOOL, &m_bBar);
	SetPropInfo("memtype",			tmWORD, &m_wMemType);
	SetPropInfo("keycolor",			tmLONG, &m_dwKeyColor);
	
	
	char Name[32];
	for (int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{
		sprintf(Name, "str%d", i);		
		SetPropInfo(Name, 	tmUSTR,	&m_pszShow[i]);
	
		sprintf(Name, "strID%d", i);
		SetPropInfo(Name, 	tmSTR,	&m_pszIDShow[i]);
		
		sprintf(Name, "stream%d", i);
		SetPropInfo(Name, 	tmFontStream,	&m_pszStream[i]);
		
		sprintf(Name, "imagepath%d", i);
		SetPropInfo(Name, tmSTR, &m_pszImagePath[i]);
	}
}
void	CtmMemo::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
        case MSG_MOUSELUP:
    		MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSELDOWN:
        	MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSEMOVE:
        	MouseMove(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }
}

BOOL	CtmMemo::MouseMove(short x_pos,short y_pos,int key_flag)
{
	if(MoveButtonDownFalg)
	{
			int jumpreg = m_scrollbar.GetIncrement();
			if( (y_pos-Down_Y) > jumpreg)
			{		
				if(m_wStartStrIndex < GetStrsNum()-GetMaxRows())
				{
					Down_Y+=jumpreg;
					ScrollDown();
				}
			}
			else if( (y_pos-Down_Y) < !jumpreg )
			{
				if(m_wStartStrIndex > 0)
				{
					Down_Y-=jumpreg;
					ScrollUp();
				}	
			}
	}
	else
	{
		RECT    rect 		=GetWindowRECT();
		int i;	
		for (i = 1; i <= GetMaxRows(); i++)
	 	{
	 		if (y_pos <= rect.top + i * m_nEntHeight)
	 			break;
	 	}
	 	
	 	if (i > GetStrsNum())
	 		return 1;
	 	
	 	if (i > GetMaxRows())
	 		i = GetMaxRows();
	 	
	 	if (m_wFocusIndex != i - 1)
	 	{
	 		m_wOldFocusIndex = m_wFocusIndex;
	 		m_wFocusIndex = i - 1;
	 		
	 		Clear();
			DrawBorder();
			ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
	 	}
	}
	return TRUE;
}
	
BOOL	CtmMemo::MouseDown(short x_pos,short y_pos,int key_flag)
{
	RECT    rect 		=GetWindowRECT();
	RECT 		MoveButtonRect = m_scrollbar.GetMoveButtonRECT();
	int 	i 			= 0;

	if(x_pos > rect.right || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
    {
    	if (m_bVisible)
    		_PutKeyTail(_ENTER);
        return 1;
    }
    
    if (x_pos >= m_scrollbar.GetWindowRECT().left)
    {
    	if (y_pos < m_scrollbar.GetWindowRECT().top + tmSCROLLBAR_BUTTON)
    	{
				m_scrollbar.SetButtonPress(1);
    		ScrollUp();
    	}
    	else if (y_pos > m_scrollbar.GetWindowRECT().bottom - tmSCROLLBAR_BUTTON)
    	{
				m_scrollbar.SetButtonPress(3);
    		ScrollDown();	
    	}
    	else if(y_pos > MoveButtonRect.top && y_pos < MoveButtonRect.bottom)		//Maio dd 2015/1/20 ?W?E 11:36:11
			{										
    		Down_Y = y_pos;																										//MoveButton
				MoveButtonDownFalg = TRUE;
				m_scrollbar.SetButtonPress(2);
			}
    	else
    		;
    		
    	return TRUE;
    }
   	
  for (i = 1; i <= GetMaxRows(); i++)
 	{
 		if (y_pos <= rect.top + i * m_nEntHeight)
 			break;
 	}
 	
 	if (i > GetStrsNum())
 		return 1;
 	
 	if (i > GetMaxRows())
 		i = GetMaxRows();
 	
 	if (m_wFocusIndex != i - 1)
 	{
 		m_wOldFocusIndex = m_wFocusIndex;
 		m_wFocusIndex = i - 1;
 		
 		Clear();
		DrawBorder();
		ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
 	}

 	
 	return TRUE;
}

BOOL	CtmMemo::MouseUp(short x_pos,short y_pos,int key_flag)
{
 	if (MoveButtonDownFalg)
	{
		MoveButtonDownFalg = FALSE;
	}
 	m_scrollbar.SetAllButtonUp();
	
	RECT    rect;
 
	rect    = GetWindowRECT();
	
	if(x_pos > m_scrollbar.GetWindowRECT().left || x_pos < rect.left || y_pos > rect.bottom || y_pos < rect.top)
	{
		return 1;
	}
	else
		_PutKeyTail(_ENTER);
 	
 	return TRUE;
}

void CtmMemo::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	SetPropInfo(MEMO_FOCUSCOLOR, 		tmWORD, &m_wFocusColor);
	SetPropInfo(MEMO_STARTSTRINDEX, 	tmWORD, &m_wStartStrIndex);
	SetPropInfo(MEMO_FOCUSINDEX,		tmWORD, &m_wFocusIndex);
	SetPropInfo(MEMO_FOCUSSTRINDEX,		tmWORD, &m_wFocusStrIndex);
	SetPropInfo(MEMO_BAR,				tmBOOL, &m_bBar);
	
	
	char Name[10];
	for (int i = 0; i < CNT_INT_MEMO_MAX; i++)
	{	
		SetPropInfo(MEMO_STR0 + i*2, 	tmUSTR,	&m_pszShow[i]);
		SetPropInfo(MEMO_STRID0 + i*2, 	tmSTR,	&m_pszIDShow[i]);
	}
}

WORD    CtmMemo::OnKey(WORD wKey)
{
	WORD	wResult 	= wKey;
	WORD	wMaxRows	= GetMaxRows();
	WORD	wStrsNum	= GetStrsNum();
	WORD	wOldIndex	= m_wStartStrIndex;
	
	switch (wKey)
	{
		case _DARROW:
		case _KEY_DOWNFIELD:		
			//m_wStartStrIndex
			
			//	the bottom line
			if ((m_wFocusIndex + 1) == wMaxRows)
			{
				//	only m_wStartStrIndex with increment
				if ((m_wStartStrIndex + m_wFocusIndex + 1) < wStrsNum)
				{
					m_wStartStrIndex++;
					Clear();
					DrawBorder();
					//GetShowText();
					ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
				}
			}
			else if (m_wFocusIndex <= wMaxRows)
			{
				if ((m_wStartStrIndex + m_wFocusIndex + 1) < wStrsNum)
				{
					m_wOldFocusIndex = m_wFocusIndex;
					m_wFocusIndex++;
					Clear();
					DrawBorder();
					//GetShowText();
					ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
				}				
			}
			else perror("FoucusIndex > wMaxRows\n");				
			wResult = _NULL_KEY;
			break;
		case _UARROW:
		case _KEY_UPFIELD:
			if (m_wFocusIndex == 0)
			{
				//	only m_wStartStrIndex with increment
				
				if (m_wStartStrIndex > 0)
					m_wStartStrIndex--;
				Clear();
				DrawBorder();
				GetShowText();
				ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
				
			}
			else
			{
				m_wOldFocusIndex = m_wFocusIndex;
				m_wFocusIndex--;
				Clear();
				DrawBorder();
				GetShowText();
				ShowStrs(m_wOldFocusIndex, m_wFocusIndex);						
			}
			
			wResult	= _NULL_KEY;
			break;
		default:
			return wResult;
	}
	
//	SetScrollbar(wStrsNum);
	return wResult;
}

void        CtmMemo::Show()           /* Virtual function */
{
	//	check the visible property
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	
	
	if (m_wnd.wStyle & tmWS_SHOW)
	{
		//	wtype == 0x8800		
		if ((m_wnd.wStyle & tmWS_REDRAW) && (m_wnd.wStyle & tmWS_FRAMEWND))
		{
			if (!m_bCopyBg)
			{
				m_bCopyBg = TRUE;		
				CopyBg();						//<<yang 2005/11/23 	
			}
		}
		Clear();
		DrawBorder();
		GetShowText();
		ShowStrs(m_wOldFocusIndex, m_wFocusIndex);	
		
		//ShowScrollbar();		
	}
	else
	{
		if (m_bCopyBg)
		{
			ShowBg();
			FreeBg();
			m_bCopyBg = FALSE;
		}
		else CtmWnd::Clear();
		ChangePosNow(GetClientRECT());
	}
}

void    	CtmMemo::Update()
{
	if (m_bPropertyChanged) Show();
}


void		CtmMemo::ShowScrollbar()
{
	if (m_bBar)
	{
		SetScrollbar(GetStrsNum());
		UpdateScrollbar();
	}
}

void		CtmMemo::UpdateScrollbar()
{
	if (m_bBar)
		m_scrollbar.Update();
}

void		CtmMemo::SetInitScrollbar()
{
	if(CtmConfig::GetInstance()->GetBinaryType() == 1)
		m_scrollbar.SetPropertys_Nub();
	else
		m_scrollbar.SetPropertys();
	
	//	===========================
	//	set the scrollbar parent
	//	===========================
	m_scrollbar.SetParent(this);
	
	//	===========================
	//	the scrollbar position and size
	//	===========================
	m_scrollbar.SetPropValueT("left", 	m_wnd.rect.right - m_wnd.rect.left - 40);
	m_scrollbar.SetPropValueT("top", 	double(0) - 2);
	m_scrollbar.SetPropValueT("right", 	m_wnd.rect.right - m_wnd.rect.left);
	m_scrollbar.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top);
	m_scrollbar.SetPropValueT("taborder", double(-3));
	
	
	//	===========================
	//	the scrollbar type
	//	===========================
	m_scrollbar.SetPropValueT("fgc",	m_fgc);
	m_scrollbar.SetPropValueT("bgc",	m_bgc);
	
	m_scrollbar.CreateA();

	//SetScrollbar(GetStrsNum());
}

void		CtmMemo::SetScrollbar(WORD wRange)
{
	int nRange = wRange - (m_wnd.rect.bottom - m_wnd.rect.top) / m_nEntHeight;

	if (nRange < 0) nRange = 0;
	m_scrollbar.SetMaxReg(nRange+1);
	m_scrollbar.SetPos(m_wStartStrIndex);
	
	//UpdateScrollbar();
}

void		CtmMemo::SetFontLen(WORD wOffset, int nFontlen, int nType)
{
	m_wszStreamLen[wOffset] = nFontlen;
	m_wStreamType[wOffset] = nType;
}
void		CtmMemo::ScrollUp()
{
	if (m_wStartStrIndex <= 0)
		return;
	
    m_wStartStrIndex--;
    
    if (m_wFocusIndex < GetMaxRows() - 1)
    {
    	m_wOldFocusIndex = m_wFocusIndex;
    	m_wFocusIndex++;
    }
    	
    Clear();
	DrawBorder();
	ShowStrs(m_wOldFocusIndex, m_wFocusIndex);
}

void		CtmMemo::ScrollDown()
{
	
	if (m_wStartStrIndex + GetMaxRows() >= GetStrsNum())
		return;
    
    m_wStartStrIndex++;
    
    if (m_wFocusIndex > 0)
    {
    	m_wOldFocusIndex = m_wFocusIndex;
    	m_wFocusIndex--;
    }	
    Clear();
	DrawBorder();
	ShowStrs(m_wOldFocusIndex, m_wFocusIndex);
    
}
WORD		CtmMemo::GetStartStrIndex()
{
	 return m_wStartStrIndex;
}
	
	
CtmCustomMemo::CtmCustomMemo(): CtmWnd()
{
	
	memset(&m_MajorRect, 0, sizeof(m_MajorRect));
}

CtmCustomMemo::~CtmCustomMemo()
{
	
}
	
	
CtmTextMemo::CtmTextMemo(): CtmCustomMemo()
{
	m_ptaskCurrsor 					= NULL;
	m_nCurrentCol 					= 0;
	m_nCurrentRow 					= 0;
	m_nHideCol							= 0;
	m_nHideRow							= 0;
	m_nMode									= CNT_INT_MEMO_MODE_INSERT;
	m_scrollbar_bgc					= 65535;
	
	m_nSelectStartRow				= 0;
	m_nSelectStartCol				= 0;
	m_nSelectEndRow					= 0;
	m_nSelectEndCol					= 0;
	m_dwSelectColor					= 55068;
	m_dwLineColor						= 2047;//LIGHTCYAN;//55555;	//	red?
	m_dwLineFGC							= 0xffff;
	
	m_nRowWithColor					= -1;
	m_nUserRowWithColor			= -2;
	
	m_wStartStrIndex 				= 0;
	m_bBar									= FALSE;
	
	m_bLoadFullFile 				= FALSE;
	m_bEditFlag							= TRUE;
	m_nCurrsorCorrect				= 0;
	
	m_bPaintCurrentRowFlag	= FALSE;			//20140530 Roy add
}

CtmTextMemo::~CtmTextMemo()
{
	if (m_ptaskCurrsor != NULL)	m_ptaskCurrsor->SetOnCursor(FALSE);
	DisableCurrsor();
	
	if(m_bBar == TRUE)
	{	
		m_scrollbar.SetParent(NULL);
	}
}

BOOL		CtmTextMemo::CreateA()
{
	m_nInputMode = 0;
	CtmCustomMemo::CreateA();
		
	//	calcuate the effective region
	CalcReg();
	
	if( m_strlist.GetNum() <1)
		m_strlist.Insert(0, "\n");
	if(m_bBar == TRUE)	SetInitScrollbar();
	ChangePosNow(GetClientRECT());
}

void    CtmTextMemo::DrawBorder()	//20111010
{
	
	short nRight  = m_rectWindow.right  - m_rectWindow.left;
	short nBottom = m_rectWindow.bottom - m_rectWindow.top;
	_SetViewPort(	m_rectWindow.left, 
					m_rectWindow.top, 
					m_rectWindow.right, 
					m_rectWindow.bottom
				);
	 _SetColor(0);
   
   
    _Rectangle(_GBORDER, 0, 0, nRight, nBottom);

    _SetColor(m_bgc);

    for (int i=1; (WORD) i< (m_wBorderWidth); i++)
    {
        _Rectangle(_GBORDER, i, i, nRight-i, nBottom-i);
    }
}

void		CtmTextMemo::CalcReg()
{
	//	1. cacluate the region
	//	2. cacluate the contained rows & cols
	if (m_pcFont != NULL)
	{
		m_VisualRect = m_rectClient;
		if(m_bBar && GetStrNum() > m_nContainRow)
		{
			m_nContainRow = (m_VisualRect.bottom - m_VisualRect.top + 1) / m_pcFont->m_wHeight;
			m_nContainCol = (m_VisualRect.right - m_VisualRect.left + 1-25) / (m_pcFont->m_wWidth >> 1);
		}
		else
		{
			m_nContainRow = (m_VisualRect.bottom - m_VisualRect.top + 1) / m_pcFont->m_wHeight;
			m_nContainCol = (m_VisualRect.right - m_VisualRect.left + 1) / (m_pcFont->m_wWidth >> 1);
		}
	}
	
}

WORD		CtmTextMemo::OnKey(WORD wKey)
{
	
	WORD	key = _NULL_KEY;
	//printf("OnKey--m_nMode:%d  wKey:%x \n",m_nMode,wKey);
	switch (m_nMode)
	{
		case CNT_INT_MEMO_MODE_DISABLE:
			break;
		case CNT_INT_MEMO_MODE_INSERT: 
			ShowCurrsor(FALSE);
			
			if (wKey >= 0x20 && wKey <= 0x80) 	Insert(wKey);
			else								key = Control(wKey);
				
			ShowCurrsor(TRUE);
			
			break;
		case CNT_INT_MEMO_MODE_COVER:
			break;
		default:
			break;
	}
	
	return key;
}

void		CtmTextMemo::InsertNewLine()
{
	char ac[2];
	ac[0] = '\0';
	int	nLen = 0;
	if (m_nCurrentRow + m_nHideRow >= m_strlist.GetNum())				//	Append the new string 
	{
		if (m_strlist.GetNum() > 0)
		{
			int strlist_GetNum = m_strlist.GetNum();
			nLen = strlen(m_strlist.IndexOf(strlist_GetNum - 1));
			if (m_nCurrentCol + m_nHideCol >= nLen)
				m_strlist.Add(ac);
		}
		else
			m_strlist.Add(ac);
	}
	else
	{
		const char* pszStr = m_strlist.IndexOf(m_nCurrentRow + m_nHideRow);
		nLen = strlen(pszStr);
		
		char*		pszTemp = NULL;
		char*		pszTemp2 = NULL;
		
		if (m_nCurrentCol + m_nHideCol >= nLen)
			m_strlist.Insert(m_nCurrentRow + m_nHideRow + 1, ac);			///	insert the new string
		else
		{
			pszTemp = new char[nLen + 1];
			pszTemp2 = new char[nLen + 1];
			memcpy(pszTemp2, pszStr, nLen + 1);
			
			memset(pszTemp, 0, nLen + 1);
			memcpy(pszTemp, pszTemp2, m_nCurrentCol + m_nHideCol);
			m_strlist.Modify(m_nCurrentRow + m_nHideRow, pszTemp);
				
			memset(pszTemp, 0, nLen + 1);
			memcpy(pszTemp, pszTemp2 + m_nCurrentCol + m_nHideCol, nLen - (m_nCurrentCol + m_nHideCol));
			m_strlist.Insert(m_nCurrentRow + m_nHideRow + 1, pszTemp);
			
			delete []pszTemp;
			delete []pszTemp2;
		}
	}
	//m_strlist.Insert(m_nCurrentRow, ac);
}

WORD		CtmTextMemo::Control(WORD wKey)
{
	
	if 	(wKey == 0x1C0A)					//	EnterKey
	{
		InsertNewLine();
		
		m_nCurrentCol 	= 0;
		m_nHideCol 		= 0;
		MoveCurrsorD();
		//Clear();
		OnPaint();
	}
	else if (wKey == 0x6A00)				//	right
	{
		MoveCurrsorR();
	}
	else if (wKey == 0x6900)				//	left
	{
		MoveCurrsorL();
	}
	else if (wKey == 0x6700)				//	up
	{
		MoveCurrsorU();
	}
	else if (wKey == 0x6c00)					//	down
	{
		MoveCurrsorD();
	}
	else if (wKey == 0x0e08)				//	back
	{
		DelFrontChar();
	}
	else if (wKey == 0x6f00)				//	del
	{
		DelBackChar();	//20110929
	}
	else if (wKey == 0x6b00)				//	end
	{
		MoveCurrsorEnd();
	}
	else if (wKey == 0x6600)				// 	home
	{
		MoveCurrsorHome();
		
	}
	else if (wKey == 0x6d00)				//	pagedown
	{
		PageDown();
	}
	else if (wKey == 0x6800)				//	pageup
	{
		PageUp();
	}
	else
		
	{
		return wKey;
	}
	
	return _NULL_KEY;
	
	//SetCurrsorPos(m_nCurrentCol, m_nCurrentRow);
}

void		CtmTextMemo::MoveCurrsorEnd()
{
	int nLen = GetStrLen(m_nCurrentRow + m_nHideRow);
	
	
	if (m_nCurrentCol + m_nHideCol >= nLen) return;
	else
	{
		if (m_nContainCol >= nLen) 	
			m_nCurrentCol = m_nContainCol - m_nHideCol - 1;
		else						
		{
			if (m_nHideCol < nLen  - m_nContainCol)
			{
				m_nHideCol = nLen - m_nContainCol + 1;
			}
			
			m_nCurrentCol = m_nContainCol -  1;
			//Clear();
			//OnPaint();
		}
		
		OnPaint();
	}
}
		
void		CtmTextMemo::MoveCurrsorHome()
{
	if (m_nHideCol > 0)
	{
		m_nCurrentCol 	= 0;
		m_nHideCol		= 0;
		
		//Clear();
		OnPaint();
		//CorrectCurrsorCurrentPos();
	}
	else 
	{
		m_nCurrentCol 	= 0;
		OnPaint();
		
	}
}

void		CtmTextMemo::PageDown()
{
	//	row number of page is m_nContainRow
	//============================================
	//	there're 3 following cases:
	//		1. the strings displayed remain, 
	//			but the currsor move to the end of the last line 
	//		2. show the next page strings,
	//		3. show the part of the current stirngs and the next page strings
	//============================================
	int nStrNum = m_strlist.GetNum();
	int	nLen 	= 0;//GetStrLen(m_nCurrentRow + m_nHideRow);
	
	
	if (m_nContainRow >= nStrNum)
	{
		//	move the currsor to the end of the last line
		m_nCurrentRow = nStrNum - 1;
		//m_nCurrentCol = GetStrLen(m_nCurrentRow + m_nHideRow);
		MoveCurrsorHome();//MoveCurrsorEnd(); 20110915 modify
		return;
	}
	else if (m_nCurrentRow + m_nHideRow + m_nContainRow <= nStrNum)
	{
		m_nHideRow += m_nContainRow;
		m_nCurrentRow = m_nContainRow - 1;
		//m_nCurrentCol = GetStrLen(m_nCurrentRow + m_nHideRow);
		MoveCurrsorHome();//MoveCurrsorEnd(); 20110915 modify	
		
		//	need to repaint
		//Clear();
		OnPaint();
	}
	else
	{
		m_nHideRow = nStrNum - m_nCurrentRow - 1;
		m_nCurrentRow = m_nContainRow - 1;
		//m_nCurrentCol = GetStrLen(m_nCurrentRow + m_nHideRow);
		
		//	need to repaint
		//Clear();
		OnPaint();
		
	}
	
	
	
}

void		CtmTextMemo::PageUp()
{
	//============================================
	//	there're 2 following cases:
	//		1. show the pervious page strings
	//		2. show the start page strings
	//============================================	
	int nStrNum = m_strlist.GetNum();
	if (m_nHideRow >= m_nContainRow)
	{
		m_nCurrentRow = 0;
		m_nHideRow -= m_nContainRow;
		MoveCurrsorHome();
		
		//Clear();
		OnPaint();
	}
	else
	{
		bool bRepaint = false;
		if (m_nHideRow > 0)	bRepaint = true;
		
		m_nCurrentRow 	= 0;
		m_nHideRow		= 0;
		
		if (bRepaint) 
		{
			//Clear();
			OnPaint();
		}
		
		MoveCurrsorHome();
		
	}
}

void		CtmTextMemo::DelFrontChar()
{
	if (m_strlist.GetNum() > 0)
	{
		//	1. delete the character
		
		const char* pszStr 	= m_strlist.IndexOf(m_nCurrentRow + m_nHideRow);
		int	nLen			= strlen(pszStr); 
		char* pszTemp 		= NULL;

		if (m_nCurrentCol == 0 && m_nHideCol == 0)			//	the position is the string start
		{
			if (m_nCurrentRow + m_nHideRow == 0) return;
			else
			{
				const char* pszStr1 = m_strlist.IndexOf(m_nCurrentRow + m_nHideRow - 1);
				int nLen1 = strlen(pszStr1);
				pszTemp = new char[nLen + nLen1 + 1];
				
				memcpy(pszTemp, pszStr1, nLen1);
				memcpy(pszTemp + nLen1, pszStr, nLen);
				pszTemp[nLen + nLen1] ='\0';
				
				m_strlist.Modify(m_nCurrentRow + m_nHideRow - 1, pszTemp);
				m_strlist.Del(m_nCurrentRow + m_nHideRow);
				
				m_nCurrentCol = nLen1;
				delete []pszTemp;
				if(m_nHideRow+m_nContainRow<=m_strlist.GetNum())
					MoveCurrsorU();
				else if(m_nHideRow>=1)
					m_nHideRow--;
				else 
					MoveCurrsorU();
				OnPaint();
			}
		}
		else 		if (m_nCurrentCol + m_nHideCol > 0)										//	the position is middle or end
		{
			pszTemp = new char[nLen];		
			memset(pszTemp, 0, nLen);
			memcpy(pszTemp, pszStr, m_nCurrentCol + m_nHideCol - 1);
			memcpy(pszTemp + m_nCurrentCol + m_nHideCol - 1,
					pszStr + m_nCurrentCol + m_nHideCol,
					strlen(pszStr + m_nCurrentCol + m_nHideCol));
			m_strlist.Modify(m_nCurrentRow + m_nHideRow, pszTemp);
			
			MoveCurrsorL();
			delete []pszTemp;
			
			if (m_nCurrentCol == 0 && m_nHideCol > 0)
			{
				m_nHideCol = m_nHideCol - m_nContainCol;
				if (m_nHideCol < 0)	
				{
					m_nCurrentCol = m_nHideCol + m_nContainCol;
					m_nHideCol = 0;
				}
				else
					m_nCurrentCol = m_nContainCol;
			}
			
			//Clear();
			OnPaint();
			//CorrectCurrsorCurrentPos();
		}
		else
			printf("error!!! when the memo delete the front character %d\n", 
					m_nCurrentCol + m_nHideCol);
		
	}
	
}

void		CtmTextMemo::DelBackChar()	//20110929
{
	if (m_strlist.GetNum() > 0)
	{
		//	1. delete the character
		
		const char* pszStr 	= m_strlist.IndexOf(m_nCurrentRow + m_nHideRow);
		int	nLen			= strlen(pszStr); 
		char* pszTemp 		= NULL;
		if (m_nCurrentCol + m_nHideCol < nLen)				//	the position is middle or start
		{
			pszTemp = new char[nLen];		
			memset(pszTemp, 0, nLen);
			memcpy(pszTemp, pszStr, m_nCurrentCol + m_nHideCol );
			memcpy(pszTemp + m_nCurrentCol + m_nHideCol ,
					pszStr + m_nCurrentCol + m_nHideCol + 1,
					strlen(pszStr + m_nCurrentCol + m_nHideCol + 1));
			m_strlist.Modify(m_nCurrentRow + m_nHideRow, pszTemp);
			
			//MoveCurrsorR();
			delete []pszTemp;
			
			//if (m_nCurrentCol == 0 && m_nHideCol > 0)
			//{
			//	m_nHideCol = m_nHideCol - m_nContainCol;
			//	if (m_nHideCol < 0)	
			//	{
			//		m_nCurrentCol = m_nHideCol + m_nContainCol;
			//		m_nHideCol = 0;
			//	}
			//	else
			//		m_nCurrentCol = m_nContainCol;
			//}
			
			//Clear();
			OnPaint();
			//CorrectCurrsorCurrentPos();
		}
		else if (m_nCurrentCol+m_nHideCol == nLen)			//	the position is the string end
		{
			if (m_nCurrentRow + m_nHideRow == m_strlist.GetNum()-1) return;	//the last row
			else
			{
				const char* pszStr1 = m_strlist.IndexOf(m_nCurrentRow + m_nHideRow + 1);
				int nLen1 = strlen(pszStr1);
				pszTemp = new char[nLen + nLen1 + 1];
				
				memcpy(pszTemp, pszStr, nLen);
				memcpy(pszTemp + nLen, pszStr1, nLen1);
				pszTemp[nLen + nLen1] ='\0';
				
				m_strlist.Modify(m_nCurrentRow + m_nHideRow , pszTemp);
				m_strlist.Del(m_nCurrentRow + m_nHideRow +1);
				
				m_nCurrentCol = nLen;
				delete []pszTemp;
				//MoveCurrsorD();
				//Clear();
				OnPaint();
				//CorrectCurrsorCurrentPos();
			}
		}
		else
			printf("error!!! when the memo delete the back character %d\n", 
					m_nCurrentCol + m_nHideCol);
		
	}
	
}

void		CtmTextMemo::Insert(WORD wKey)
{
	//	===========================================================//
	//	Check whether the Currsor is beyond the scope of the strings
	//	1. y pos is beyond, need to append new string
	//	2. x pos is beyond, need to append characterf in the cureent string
	//	3. x pos is not beyond, need to insert the character int the postion of cureent string
	//	===========================================================//
	char ac[2];
	ac[0] 		= char(wKey);
	ac[1] 		= '\0';
	int nLen 	= 0;
	char* pszTemp = ac;
	//printf("Insert--wKey:%x  ac[0]:%c \n",wKey,ac[0]);
	if ((m_nCurrentRow + m_nHideRow) >= m_strlist.GetNum())	//	the y position
	{
		AddNewStr(ac);										//	append new string;			
		nLen = strlen(ac);	
	}
	else 													//	the x position			
	{
		const	char* pszStr = m_strlist.IndexOf(m_nHideRow + m_nCurrentRow);
		nLen = strlen(pszStr);
		pszTemp = new char[nLen + 1 + 1];				//	second 1 is '\0', frist 1 is insert charcter
		memset(pszTemp, 0, nLen + 1 + 1);
		if ((m_nCurrentCol + m_nHideCol) >= nLen)			//	append new character
		{
			memcpy(pszTemp, pszStr, nLen);
			pszTemp[nLen] 	= ac[0];						//	append the press charcter
			pszTemp[nLen + 1] = '\0';
			m_strlist.Modify(m_nHideRow + m_nCurrentRow, pszTemp);
		}
		else												//	insert character
		{
			memcpy(pszTemp, pszStr, m_nCurrentCol + m_nHideCol);
			pszTemp[m_nCurrentCol + m_nHideCol] = ac[0];
			memcpy(pszTemp + m_nCurrentCol + m_nHideCol + 1, 
					pszStr + m_nCurrentCol + m_nHideCol,
					nLen -  (m_nCurrentCol + m_nHideCol));
			
			pszTemp[nLen + 1] = '\0';
			m_strlist.Modify(m_nHideRow + m_nCurrentRow, pszTemp);;	
			
			
		}
		delete []pszTemp;
	}
	
	ClearLine(m_nCurrentRow, m_nCurrentCol, m_bgc, nLen - m_nCurrentCol - m_nHideCol);
	
	MoveCurrsorR();		//	the Currsor moved right
	OnPaint();
	
	
}

void		CtmTextMemo::ClearLine(int nRow, int nStartCharIndex, DWORD Color, int nNum)
{
	Clear();	
}

void		CtmTextMemo::CorrectCurrsorCurrentPos()
{
	int nHideRow_Old = 0;	//20110810 add 
	if(!m_bEditFlag && m_bLoadFullFile == FALSE)		//20110928 add (!m_bEditFlag &&)
	{
		nHideRow_Old = m_nHideRow;
		m_nHideRow = 0;
	}
	
	//	fix on the col, the Currsor can't go beyond the string's length
	if (m_nCurrentCol + m_nHideCol > GetStrLen(m_nCurrentRow + m_nHideRow))
		m_nCurrentCol = GetStrLen(m_nCurrentRow + m_nHideRow) - m_nHideCol;
	//	fix on the row, the Currsor can't go byond the string num
	int strlist_GetNum = m_strlist.GetNum();					//m_strlist.GetNum()
	if(m_nHideRow!=0 && m_nHideRow + m_nContainRow > strlist_GetNum)
	{
		m_nHideRow = m_nHideRow=strlist_GetNum-m_nContainRow;
		if(m_nHideRow<0)m_nHideRow=0;
	}
	if (m_nCurrentRow + m_nHideRow > strlist_GetNum- 1)
		m_nCurrentRow = strlist_GetNum - m_nHideRow -1;
	if(m_nCurrentRow+1 > m_nContainRow) 
		m_nCurrentRow=m_nContainRow-1;
	if (m_nCurrentCol < 0) m_nCurrentCol = 0;
	if (m_nCurrentRow < 0) m_nCurrentRow = 0;
	//printf("m_nCurrentRow is %d\tm_nHideRow is %d\tGetNum() is %d\n",m_nCurrentRow,m_nHideRow,m_strlist.GetNum());
	//printf("CorrectCurrsorCurrentPos--m_nCurrentCol:%d  m_nCurrsorCorrect:%d \n",m_nCurrentCol,m_nCurrsorCorrect);
	//printf("m_nCurrentCol + m_nCurrsorCorrect = %d   m_nCurrentRow = %d\n",m_nCurrentCol + m_nCurrsorCorrect,m_nCurrentRow);
	SetCurrsorPos(m_nCurrentCol + m_nCurrsorCorrect, m_nCurrentRow);
	if(!m_bEditFlag && m_bLoadFullFile == FALSE)		//20110928 add (!m_bEditFlag &&)
		m_nHideRow = nHideRow_Old;
}

void		CtmTextMemo::MoveCurrsorR()
{
	int strlist_GetNum = m_strlist.GetNum();
	/*if(!m_bEditFlag)	//20110825 add
	{
		m_nCurrentCol++;
		m_nHideCol++;
		OnPaint();
		CorrectCurrsorCurrentPos();
	}
	else
	{*/
		//printf("m_nContainCol = %d\n",m_nContainCol);
		//printf("m_nCurrentCol = %d\n",m_nCurrentCol);
		if (m_nCurrentCol + m_nHideCol < GetStrLen(m_nCurrentRow + m_nHideRow))
		{
			m_nCurrentCol++;	
			if (m_nCurrentCol + 1 > m_nContainCol)		
			{
				m_nCurrentCol = m_nContainCol - 1;
				m_nHideCol++;
				//Clear();
				//OnPaint();							//	beacause the hide col increase
			}
			OnPaint();
		}	
		else if (m_nCurrentRow + m_nHideRow < strlist_GetNum - 1)//Mario add
		{
			m_nCurrentRow++;
			m_nHideCol = 0;
			m_nCurrentCol = 0;
			if (m_nCurrentRow >= m_nContainRow)  
			{
				m_nCurrentRow = m_nContainRow - 1;
				m_nHideRow++;							//	beacause the hide row increase
				//Clear();										
			}
			OnPaint();
			//CorrectCurrsorCurrentPos();
		}
	//}
	
}

void		CtmTextMemo::MoveCurrsorL()
{
	if(m_nCurrentCol + m_nHideCol > 0)
	{
		if(m_nCurrentCol > 0) m_nCurrentCol--;
		else 
		{
			//OnPaint();
			m_nHideCol--;
		}
	}
	else if((m_nCurrentCol + m_nHideCol <= 0) && (m_nCurrentRow + m_nHideRow) > 0)
	{
		BOOL Paintflag = FALSE;
		if(m_nCurrentRow > 0) m_nCurrentRow--;
		else 
		{
			Paintflag = TRUE;
			m_nHideRow--;
		}
		m_nCurrentCol = GetStrLen(m_nCurrentRow + m_nHideRow);
		m_nHideCol = 0;		//ROW->COL
		if(m_nCurrentCol >= m_nContainCol)
		{
			Paintflag = TRUE;
			m_nHideCol = m_nCurrentCol - m_nContainCol+1;
			m_nCurrentCol = m_nContainCol-1;
		}
		//if(Paintflag)  OnPaint();
			
	}
	
	OnPaint();
}

void		CtmTextMemo::MoveCurrsorD()
{
	int strlist_GetNum = m_strlist.GetNum();
	if (m_nCurrentRow + m_nHideRow < strlist_GetNum - 1)
	{
		m_nCurrentRow++;
		if (m_nCurrentRow >= m_nContainRow)  
		{
			m_nCurrentRow = m_nContainRow - 1;
			m_nHideRow++;
			//OnPaint();								//	beacause the hide row increase
		}
		OnPaint();
	}

}

void		CtmTextMemo::MoveCurrsorU()
{
	m_nCurrentRow--;
	if (m_nCurrentRow < 0)		
	{
		m_nCurrentRow = 0;
		m_nHideRow--;
		if (m_nHideRow < 0) 	m_nHideRow = 0;
		else					
		{
			//OnPaint();		//	beacause the hide row decrease
		}
	}
	OnPaint();

}

int			CtmTextMemo::GetStrLen(int nIndex)
{
	if (nIndex >= m_strlist.GetNum()) 	return 0;
	else 								return strlen(m_strlist.IndexOf(nIndex));
}

void		CtmTextMemo::AddNewStr(char* pszStr)
{
	m_strlist.Add(pszStr);
}

void		CtmTextMemo::GetStr(int nIndex, char* pszStr)
{
	const char* tempStr = NULL;
	tempStr = m_strlist.IndexOf(nIndex);
	strcpy(pszStr,tempStr);
}

void		CtmTextMemo::OnGetFocus()         /* Virtual function */
{
	if(!m_nInputMode)
	{
		m_bOnFocus = TRUE;
		EnableCurrsor();
	}
}

void		CtmTextMemo::OnLoseFocus()
{
	m_bOnFocus = FALSE;
	DisableCurrsor();
}
		
void		CtmTextMemo::EnableCurrsor()
{
	TASKCURSOR  taskCurrsor;
	
	if (m_ptaskCurrsor == NULL)
	{
		taskCurrsor.pwnd = this;
		m_ptaskCurrsor = new CTaskCursor(NULL, &taskCurrsor);
	}
	SetCurrsorPos(m_nCurrentCol, m_nCurrentRow);
	ShowCurrsor(TRUE);
	
}

void		CtmTextMemo::DisableCurrsor()
{
	if (m_ptaskCurrsor != NULL)
	{
		ShowCurrsor(FALSE);
		delete m_ptaskCurrsor;
		m_ptaskCurrsor = NULL;
	}
}

void		CtmTextMemo::ShowCurrsor(BOOL bOn)
{
	if (m_ptaskCurrsor != NULL && m_bEnabled)
		m_ptaskCurrsor->DisplayCursor(bOn);
}

void		CtmTextMemo::SetCurrsorPos(int nCol, int nRow)
{
	if (m_ptaskCurrsor != NULL)
	{
		int x, y;
		TransPos(nCol, nRow, &x, &y);
		m_ptaskCurrsor->SetCursorPosition(x, y);
		//printf("SetCurrsorPos--nCol:%d nRow:%d X:%d Y:%d \n",nCol,nRow,x,y);
	}
	
}

//	note: this function calculate with the BorderWidth
void		CtmTextMemo::TransPos(int nCol, int nRow, int* pOutX, int* pOutY)
{
	if (m_pcFont != NULL)
	{
		if (pOutX != NULL)
			*pOutX = m_wBorderWidth + nCol * (m_pcFont->m_wWidth >> 1);
		if (pOutY != NULL)
			*pOutY = m_wBorderWidth + nRow * m_pcFont->m_wHeight;
	}
}

bool		CtmTextMemo::IsSelectState()
{
	return !(m_nSelectStartRow == m_nSelectEndRow && m_nSelectStartCol == m_nSelectEndCol);
}

void		CtmTextMemo::CancelSelectState()
{
	m_nSelectStartRow = 0;
	m_nSelectStartCol = 0;
	m_nSelectEndRow	  = 0;
	m_nSelectEndCol	  = 0;	
}

void		CtmTextMemo::Show()
{
	CtmWnd::Show();	//	in fact, draw the border
		
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add	
		OnPaint();
	else
		Clear();
}

void		CtmTextMemo::GotoLine(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_strlist.GetNum())
	{ 
		if (m_bOnFocus)		ShowCurrsor(FALSE);
		OnPaint();
		if (m_bOnFocus)		ShowCurrsor(TRUE);		
		return;
	}
	else
	{
		if (m_bOnFocus)		ShowCurrsor(FALSE);
		m_nHideCol 		= 0;
		m_nHideRow 		= nIndex;
		m_nCurrentRow 	= 0;
		m_nCurrentCol 	= 0;
		OnPaint();
		if (m_bOnFocus)		ShowCurrsor(TRUE);		
	}
}

int		CtmTextMemo::Find(char* psz)
{
	return m_strlist.Find(psz);
}

int		CtmTextMemo::Find(int nStartIndex, int nEndIndex, char* psz)
{
	return m_strlist.Find(nStartIndex, nEndIndex, psz);
}

void		CtmTextMemo::Update()
{
	Show();
}

void 		CtmTextMemo::SetPropertys()
{
	CtmCustomMemo::SetPropertys();
		
	SetPropInfo("mode", 				tmINT, 		&m_nMode);
	
	SetPropInfo("select_color", 		tmDWORD, 	&m_dwSelectColor);
	SetPropInfo("select_startr", 		tmINT, 		&m_nSelectStartRow);
	SetPropInfo("select_endr",			tmINT, 		&m_nSelectEndRow);
	SetPropInfo("select_startc",		tmINT, 		&m_nSelectStartCol);
	SetPropInfo("select_endc",			tmINT, 		&m_nSelectEndCol);
	
	SetPropInfo("user_row",				tmINT, 		&m_nUserRowWithColor);
	SetPropInfo("user_color",           tmDWORD,    &m_dwLineColor);
	SetPropInfo("user_fgc",           	tmDWORD,    &m_dwLineFGC);
	SetPropInfo("bar",					tmBOOL, 	&m_bBar);
	
	//	only read properties
	SetPropInfo("current_row",			tmINT, 		&m_nCurrentRow);
	SetPropInfo("current_col",			tmINT, 		&m_nCurrentCol);
	SetPropInfo("hide_row",				tmINT, 		&m_nHideRow);
	SetPropInfo("hide_col",				tmINT, 		&m_nHideCol);
	SetPropInfo("contain_row",			tmINT, 		&m_nContainRow);	
	SetPropInfo("contain_col",			tmINT, 		&m_nContainCol);

}

void		CtmTextMemo::OnPaint()
{
	ShowCurrsor(FALSE);
	CorrectCurrsorCurrentPos();
	CalcReg();
	
	m_nSelectStartRow = m_nCurrentRow+m_nHideRow;
	m_nSelectEndRow = m_nCurrentRow+m_nHideRow;
	int nHideRow_Old =0;
	
	if(m_strlist.GetNum() < m_nHideRow)	m_nHideRow =0;	//20110823 add

	//printf("m_nHideRow = %d\n",m_nHideRow);
	//printf("m_bEditFlag = %d\n",m_bEditFlag);
	//printf("m_bLoadFullFile = %d\n",m_bLoadFullFile);
	if(!m_bEditFlag && m_bLoadFullFile == FALSE)		//20110928 add (!m_bEditFlag &&)
	{
		nHideRow_Old = m_nHideRow;
		m_nHideRow = 0;
	}
	
	char pszStr[256]="\0";//const char* pszStr = NULL;//		2012/7/20 modify
	
	Clear();
	if (m_nUserRowWithColor ==-1 && m_bEnabled)
	{
		PaintSelect(m_nSelectStartRow, m_nSelectEndRow, 
					m_nSelectStartCol, m_nSelectEndCol, 
					m_dwSelectColor);
	}
	else if(1/*m_bEnabled*/)
	{
		PaintRow(m_nUserRowWithColor);
	}
	//printf("m_nHideRow:%d  GetNum:%d  nHideRow_Old:%d m_nContainRow:%d \n",m_nHideRow,m_strlist.GetNum(),nHideRow_Old,m_nContainRow);
	for (int i = m_nHideRow, j = 0; (i < ( m_strlist.GetNum()-nHideRow_Old )) && (j < m_nContainRow); i++, j++)//20110810 add -nHideRow_Old
	{
		strcpy(pszStr,m_strlist.IndexOf(i));//pszStr = m_strlist.IndexOf(i);	//	2012/7/20 modify,avoid shutoff
		PaintStr(i - m_nHideRow, m_nHideCol, pszStr);
		//printf("LineNo:%d  pszStr:%s \n",i - m_nHideRow,pszStr);
	}
	if(!m_bEditFlag && m_bLoadFullFile == FALSE)		m_nHideRow = nHideRow_Old;	//20110928 add (!m_bEditFlag &&)	
	DrawBorder();	//20111010	
	
  ShowScrollbar();
  ChangePosNow(GetClientRECT());
	ShowCurrsor(TRUE);
}

void		CtmTextMemo::OnPaint2()	//many row
{
	CalcReg();
	int nHideRow_Old =0;
	
	if(m_strlist.GetNum() < m_nHideRow)	m_nHideRow =0;	//20110823 add

	if(!m_bEditFlag && m_bLoadFullFile == FALSE)		//20110928 add (!m_bEditFlag &&)
	{
		nHideRow_Old = m_nHideRow;
		m_nHideRow = 0;
	}
	
	char pszStr[256]="\0";//const char* pszStr = NULL;//		2012/7/20 modify
	
	Clear();
	if (m_nUserRowWithColor ==-1 && m_bEnabled)
	{
		PaintSelect(m_nSelectStartRow, m_nSelectEndRow, 
					m_nSelectStartCol, m_nSelectEndCol, 
					m_dwSelectColor);
	}
	else if(1/*m_bEnabled*/)
	{
		PaintRow(m_nUserRowWithColor);
	}
	//printf("m_nHideRow:%d  GetNum:%d  nHideRow_Old:%d m_nContainRow:%d \n",m_nHideRow,m_strlist.GetNum(),nHideRow_Old,m_nContainRow);
	for (int i = m_nHideRow, j = 0; (i < ( m_strlist.GetNum()-nHideRow_Old )) && (j < m_nContainRow); i++, j++)//20110810 add -nHideRow_Old
	{
		strcpy(pszStr,m_strlist.IndexOf(i));//pszStr = m_strlist.IndexOf(i);	//	2012/7/20 modify,avoid shutoff
		PaintStr(i - m_nHideRow, m_nHideCol, pszStr);
		//printf("LineNo:%d  pszStr:%s \n",i - m_nHideRow,pszStr);
	}
	if(!m_bEditFlag && m_bLoadFullFile == FALSE)		m_nHideRow = nHideRow_Old;	//20110928 add (!m_bEditFlag &&)	
	DrawBorder();	//20111010	
	
  ShowScrollbar();
  ChangePosNow(GetClientRECT());
}
void		CtmTextMemo::PaintStr(int nRow, int nHideCol, const char* psz)
{
    if (psz == NULL) return;
        
	int y;
	
	char*   pszTemp = NULL;
	int     nStrLen =strlen(psz);
	char    szStr[128];
	bool    bTemp   = (nStrLen >= sizeof(szStr));	
	memset(szStr, 0, sizeof(szStr));
	
	if (bTemp)
	{
		pszTemp = new char[nStrLen + 1];
		memset(pszTemp, 0, nStrLen + 1);
	}
	else pszTemp = szStr;
	
	if (nStrLen > nHideCol)
	    memcpy(pszTemp, psz + nHideCol, nStrLen - nHideCol);
	
	
	TransPos(0, nRow, NULL, &y);
	
	if (pszTemp[0] != 0)
	{
	    CtmWnd::ShowExtent(m_wBorderWidth, y, pszTemp, 0);
	}  
    if (bTemp)
        delete []pszTemp;
}

void		CtmTextMemo::PaintSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol, DWORD dwColor)
{
	int start_row=m_nSelectStartRow-m_nHideRow;
	if(start_row < 0 ) start_row=0;
	int end_row=m_nSelectEndRow-m_nHideRow;
	if(end_row+1 > m_nContainRow ) end_row=m_nContainRow-1;
	
	if(start_row > end_row)	return;
	else if(start_row == end_row && nStartCol > nEndCol)	return;
	//else if(nStartRow == nEndRow && nStartCol == nEndCol )	return;	//20110706 add
	else if	(start_row < 0 || end_row < 0 || nStartCol < 0 || nEndCol < 0) return;
	//if 		(nStartRow == nEndRow && nStartCol == nEndCol)	return;
	//else if	(nStartRow < 0 || nEndRow < 0 || nStartCol < 0 || nEndCol < 0) return;
	else
	{
		if(m_bPaintCurrentRowFlag == FALSE)									//20140530 Roy add
		{
			int nTemp = 0;
			for (int i = start_row; i <= end_row; i++)
				{
					FillLine(i, dwColor);
				}
			}
			/*
			//=========================================
			//	it consists of 3 parts:
			//		1. header line,
			//		2. middle whole lines
			//		3. end line
			//==========================================
			if (nEndRow - nStartRow + 1 > m_strlist.GetNum() - m_nHideRow)
				nEndRow = m_strlist.GetNum() - m_nHideRow - nStartRow - 1;
			
			//	fill the header line
			if (nEndRow > nStartRow) 	
			{
				nTemp = GetStrLen(nStartRow + m_nHideRow) - m_nHideCol;
				
				if (nTemp > m_nContainCol) nTemp = m_nContainCol;
				FillLine(nStartRow, nStartCol, nTemp, dwColor);
			}
			else						
			{
				nTemp = GetStrLen(nStartRow + m_nHideRow) - m_nHideCol;
				
				if (nTemp > m_nContainCol) nTemp = m_nContainCol;				
				if (nTemp > nEndCol) nTemp = nEndCol;
					
				FillLine(nStartRow, nStartCol, nTemp, dwColor);
				//return;
			}
			
			
			//	fill the middle line
			if (nEndRow - nStartRow > 1)
			{
				for (int i = nStartRow + 1; i < nEndRow; i++)
				{
					nTemp = GetStrLen(i + m_nHideRow) - m_nHideCol;
					
					if (nTemp > m_nContainCol) nTemp = m_nContainCol;
					
					FillLine(i, 0, nTemp, dwColor);
				}
			}
	
			//	fill the end line
			if (nEndRow - nStartRow > 0)
			{
				if (nTemp > m_nContainCol) nTemp = m_nContainCol;
				if (nTemp > nEndCol) nTemp = nEndCol;
					
				FillLine(nEndRow, 0, nEndCol, dwColor);
			}	
		}
		else																								//20140530 Roy add for CtmLTextMemo.PaintCurrentRow
		{
			FillLine(nStartRow, 0, m_nContainCol, dwColor);
		}
		*/
		//20110523 add<<<<<	After Paint SlectContain,Then show the content
		char pszStr[256]="\0";//const char* pszStr = NULL;//		2012/7/20 modify
		for (int i = m_nHideRow, j = 0; (i < m_strlist.GetNum()) && (j < m_nContainRow); i++, j++)
		{
			strcpy(pszStr,m_strlist.IndexOf(i));//pszStr = m_strlist.IndexOf(i);	//	2012/7/20 modify
			PaintStr(i - m_nHideRow, m_nHideCol, pszStr);
		}
		//20110523 add>>>>>
		
	}
}

void		CtmTextMemo::DelSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol)//Sunny<20110609> add
{
	if(nStartRow > nEndRow)	return;
	else if(nStartRow == nEndRow && nStartCol > nEndCol)	return;
	else if (nStartRow == nEndRow && nStartCol == nEndCol)	return;
	else if	(nStartRow < 0 || nEndRow < 0 || nStartCol < 0 || nEndCol < 0) return;
	else
	{
		int nTemp = 0;
		if (nStartRow > nEndRow)
		{
			nTemp = nStartRow;
			nStartRow = nEndRow;
			nEndRow = nTemp;
		}
		
		//=========================================
		//	it consists of 3 parts:
		//		1. header line,
		//		2. middle whole lines
		//		3. end line
		//==========================================
		if (nEndRow - nStartRow + 1 > m_strlist.GetNum() - m_nHideRow)
			nEndRow = m_strlist.GetNum() - m_nHideRow - nStartRow - 1;

		
		//Del the end line
		if (nEndRow - nStartRow > 0)
		{
			const char* pszStr 	= m_strlist.IndexOf(nEndRow);
			int	nLen			= strlen(pszStr); 
			char* pszTemp 		= NULL;
			pszTemp = new char[nLen];		
			memset(pszTemp, 0, nLen);
			memcpy(pszTemp, pszStr+nEndCol, nLen-nEndCol);
			m_strlist.Modify(nEndRow, pszTemp);
			delete []pszTemp;
			//printf("the end line--pszTemp:%s \n",pszTemp);
		}
		else if(nEndRow == nStartRow)	//20110825 add else
		{
			const char* pszStr 	= m_strlist.IndexOf(nEndRow);
			int	nLen			= strlen(pszStr); 
			char* pszTemp 		= NULL;
			pszTemp = new char[nLen];		
			memset(pszTemp, 0, nLen);
			memcpy(pszTemp, pszStr+nEndCol, nLen-nEndCol);
			m_strlist.Modify(nEndRow, pszTemp);
			delete []pszTemp;
		}
		
		//	Del the middle line
		if (nEndRow - nStartRow > 0)
		{
			for (int i = nEndRow-1; i > nStartRow; i--)
			{
				DelRow(i);
			}
		}
		
		//Del the header line
		if (nEndRow - nStartRow > 0)	//20110825 add if
		{
			const char* pszStrStartRow 	= m_strlist.IndexOf(nStartRow);
			int	nLen			= strlen(pszStrStartRow); 
			if(nStartCol > nLen )nStartCol = nLen;
        	
        	const char* pszStrStartRowNext 	= m_strlist.IndexOf(nStartRow+1);
			int	nLenStartRowNext			= strlen(pszStrStartRowNext);
			
			char* pszTemp 		= NULL;
			pszTemp = new char[nLen+nLenStartRowNext];		
			memset(pszTemp, 0, nLen+nLenStartRowNext);
			memcpy(pszTemp, pszStrStartRow, nStartCol);
			strcat(pszTemp, pszStrStartRowNext);
			
			m_strlist.Modify(nStartRow, pszTemp);
			//printf("the header line--pszTemp:%s \n",pszTemp);
			delete []pszTemp;
			DelRow(nStartRow+1);
		}
		
		//20110523 add<<<<<	After modify SlectContain,Then show the content
		char pszStr[256]="\0";//const char* pszStr = NULL;//		2012/7/20 modify
		for (int i = m_nHideRow, j = 0; (i < m_strlist.GetNum()) && (j < m_nContainRow); i++, j++)
		{
			strcpy(pszStr,m_strlist.IndexOf(i));//pszStr = m_strlist.IndexOf(i);	//	2012/7/20 modify
			PaintStr(i - m_nHideRow, m_nHideCol, pszStr);
		}
		//20110523 add>>>>>
	}	
}

//====================================================================
//	fill the specify line & length with color
//	nRow: the line be need to fill
//	nStartCol, nCol: the length be need to fill
//	the 3 paramters are not pixels are the Character's width
//====================================================================
void		CtmTextMemo::FillLine(int nRow, int nStartCol, int nEndCol, DWORD dwColor)
{
	int x, y;
	RECT 	rect;
	
	TransPos(nStartCol, nRow, &x, &y);
	rect.left 	= x;
	rect.top 	= y;
	

	
	TransPos(nEndCol, nRow + 1, &x, &y);
	rect.right  = x+8/*+21*/;
	rect.bottom = y;
	
	_SetColor(dwColor);
	_Rectangle(	_GFILLINTERIOR,    
	        	    		rect.left,         
		       		    	rect.top,          
	        			    rect.right,
	   					    rect.bottom);
}

void		CtmTextMemo::FillLine(int nRow, DWORD dwColor)
{
	int x, y;
	RECT 	rect;
	
				
	TransPos(0, nRow, &x, &y);
	rect.left 	= x;
	rect.top 	= y;
	

	
	TransPos(m_nContainCol, nRow+1, &x, &y);
	rect.right  = x+8/*+21*/;
	rect.bottom = y;
	
	/*_SetViewPort(	rect.left, 
					rect.top, 
					rect.right, 
					rect.bottom
				);*/
	_SetColor(dwColor);
	_Rectangle(	_GFILLINTERIOR,    
	        	    		rect.left,         
		       		    	rect.top,          
	        			    rect.right,
	   					    rect.bottom);
}

void		CtmTextMemo::PaintLine(int nIndex)
{
	nIndex -= m_nHideRow;
	
	PaintRow(nIndex);
	
}

void		CtmTextMemo::PaintRow(int nRow)
{
	if (m_nRowWithColor >= 0)
	{
		FillLine(m_nRowWithColor, 0, m_nContainCol, m_bgc);
		m_nRowWithColor = -1;
	}
	
	
	if (nRow >= 0 && nRow < m_nContainRow)
	{
		FillLine(nRow, 0, m_nContainCol, m_dwLineColor);
		m_nRowWithColor = nRow;
	}
	
}                               

int		CtmTextMemo::GetStrNum()
{
	return m_strlist.GetNum();
}

int		CtmTextMemo::GetCurrentCol()
{
	return m_nCurrentCol;
}

int		CtmTextMemo::GetCurrentRow()
{
	return m_nCurrentRow;
}

int		CtmTextMemo::GetCurentStrCol()
{
	return m_nCurrentCol + m_nHideCol;
}

int		CtmTextMemo::GetCurrentStrRow()
{
	return m_nCurrentRow + m_nHideRow;
}

int		CtmTextMemo::GetContainRow()
{
	return m_nContainRow;
}

int		CtmTextMemo::GetContainCol()
{
	return m_nContainCol;
}

void		CtmTextMemo::DelRow(int nIndex)//20110519
{
	m_strlist.Del(nIndex);
}

const char*		CtmTextMemo::GetCurrentString()
{
	if (GetStrNum() < m_nCurrentRow + m_nHideRow + 1)
		return NULL;
	else
		return m_strlist.IndexOf(m_nCurrentRow + m_nHideRow);
}

const char*		CtmTextMemo::GetVisibleString(int nRow)
{
	if ((GetStrNum() < nRow + m_nHideRow + 1) || 
		(nRow >= m_nContainRow))	//	beyond the region
		return NULL;
	else
		return m_strlist.IndexOf(nRow + m_nHideRow);
}

const char*		CtmTextMemo::GetString(int nIndex)
{
	if (GetStrNum() < nIndex + 1)	return NULL;
	else 									return m_strlist.IndexOf(nIndex);
}

//20110701 add <<<<<
void		CtmTextMemo::ShowScrollbar()
{
	if (m_bBar)
	{
	//printf("ShowScrollbar--:%d \n",GetStrNum());
		if(GetStrNum() > m_nContainRow)
		{
			SetScrollbar(GetStrNum());
			//printf("4-ShowScrollbar--:%d \n",GetStrNum());
			UpdateScrollbar();
		}
	}
}

void		CtmTextMemo::UpdateScrollbar()
{
	if (m_bBar)
		m_scrollbar.Update();
}

void		CtmTextMemo::SetInitScrollbar()
{
	m_scrollbar.SetPropertys();
	
	//	===========================
	//	set the scrollbar parent
	//	===========================
	m_scrollbar.SetParent(this);
	
	//	===========================
	//	the scrollbar position and size
	//	===========================
	m_scrollbar.SetPropValueT("left", 	m_wnd.rect.right - m_wnd.rect.left -40);
	m_scrollbar.SetPropValueT("top", 	double(0)-2);
	m_scrollbar.SetPropValueT("right", 	m_wnd.rect.right - m_wnd.rect.left	);
	m_scrollbar.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top);
	
	
	//	===========================
	//	the scrollbar type
	//	===========================
	m_scrollbar.SetPropValueT("fgc",	m_fgc);
	m_scrollbar.SetPropValueT("bgc",	m_scrollbar_bgc);
	m_scrollbar.CreateA();

}


void		CtmTextMemo::SetScrollbar(WORD wRange)
{
	int nRange = wRange - m_nContainRow;
	//printf("3-MaxReg:%d SetPos:%d \n",nRange,m_wStartStrIndex);
	if (nRange < 0) nRange = 0;
	m_scrollbar.SetMaxReg(/*wRange-1*/nRange);	//20110706 modify
	m_scrollbar.SetPos(m_nHideRow);
	m_scrollbar.SetButtonHight(m_nContainRow);

}

/*=======================================================================================
Mario add

modify  form  CtmLTextMemo
add  scrollbar   


========================================================================================*/


CtmMTextMemo::CtmMTextMemo(): CtmWnd()
{
	strcpy(TYPEName, "CtmMTextMemo");
	m_LineNoMemo.SetPropertys();
	m_ContentMemo.SetPropertys();
	
	m_LineNoMemo.SetParent(this);
	m_ContentMemo.SetParent(this);
	m_bLineNoVisable= true;
	m_nLineNoLen	= 2;//4;		//	the line no default max is FFFF;
	m_pszFile		= NULL;
	
	m_LineNoMemo.m_fgc				= m_fgc;
	m_LineNoMemo.m_bgc				= LIGHTGRAY;	
	
	m_ContentMemo.m_bEditFlag	= FALSE;
	m_nStartIndex				= 0;
	m_pszPrompt					= " ";	//20120110 add
	MoveButtonDownFalg = FALSE;		//!O2¢FXE?!M¢FDo?O!¡Ó_3QA¢FG?U
	Down_Y=0;											//?o|b!O2¢FXE?!M¢FDoREaoYRy?D-E
	MoveMemoStrFalg = FALSE;		  //Memo?!M¢FDo?O!¡Ó_3QA¢FG?U
	Down_X=0;											//?o|b!O2¢FXE?!M¢FDoREaoYRy?D-E
	StrMoveFlag = FALSE;					//?e!A}MEMO_MOUSE_MOVE_RANGE--!Li!A?}!OOA2!O!O¢FDa¢FDk!PA¢FXE¢FD\!Aa
	NowMaxCol   = 0;							//MouseMove?e?~-poa
  copy_start = NULL;  
	m_ContentMemo.m_bEnabled = m_bEnabled;
}

CtmMTextMemo::~CtmMTextMemo()
{
	m_LineNoMemo.SetParent(NULL);
	m_ContentMemo.SetParent(NULL);
	//m_scrollbar.SetParent(NULL);
}

BOOL		CtmMTextMemo::CreateA()
{
	CtmWnd::CreateA();
	
	m_ContentMemo.m_bEnabled = m_bEnabled;
	m_ContentMemo.m_strlist.Clear();
	InitLineNoMemo();
	InitContentMemo();
	/*
	if(m_bBar)
		SetInitScrollbar();*/
	
}

WORD		CtmMTextMemo::OnKey(WORD wKey)
{
	WORD Key = wKey;
	
	if(m_bEnabled)
	{
		Key = m_ContentMemo.OnKey(wKey);
		LineNoMemoPaint();
	}
	return Key;
}

void		CtmMTextMemo::Show()
{
	CtmWnd::Show();	//	in fact, draw the border
	
	m_ContentMemo.Show();
	LineNoMemoPaint();

}

void		CtmMTextMemo::LineNoMemoPaint()	//20110812
{
	if (m_bLineNoVisable)
	{
		//20110707 add <<<<<<
		//printf("m_ContentMemo.m_nContainRow is %d\n",m_ContentMemo.m_nContainRow);
		//printf("m_LineNoMemo.m_strlist.GetNum() is %d\n",m_LineNoMemo.m_strlist.GetNum());
		int strlist_GetNum = m_ContentMemo.m_nContainRow/*m_LineNoMemo.m_strlist.GetNum()*/;
		int MaxNum = m_ContentMemo.m_nContainRow + m_ContentMemo.m_nHideRow;
		if(MaxNum > m_ContentMemo.GetStrNum()) MaxNum=m_ContentMemo.GetStrNum();
		int nLineNoLenNew = 2;
		while(MaxNum > 99)
		{
			MaxNum = MaxNum /10;
			nLineNoLenNew ++ ;
		}
		if(nLineNoLenNew != m_nLineNoLen)
		{
			//printf("1LineNoMemoPaint--m_nLineNoLen:%d nLineNoLenNew:%d  m_ContentMemo.m_bEditFlag:%d \n",m_nLineNoLen,nLineNoLenNew,m_ContentMemo.m_bEditFlag);
			//static int nLineNoLenOld = m_nLineNoLen;
			
			m_nLineNoLen = nLineNoLenNew;
			InitLineNoMemo();
			InitContentMemo();
			m_ContentMemo.Show();
			/*if(m_ContentMemo.m_bEditFlag)
			{
				if(nLineNoLenOld > m_nLineNoLen )	m_ContentMemo.m_nCurrsorCorrect--;
				else								m_ContentMemo.m_nCurrsorCorrect++;
			}*/
			//nLineNoLenOld = m_nLineNoLen;
		}
		//20110707 add >>>>>>
		static int CurrosorCorrect =-1;	//20120323 modify(0)->(-1)
		if(m_ContentMemo.m_nCurrsorCorrect != CurrosorCorrect)
		{
			CurrosorCorrect = m_ContentMemo.m_nCurrsorCorrect;
			if(m_ContentMemo.m_nCurrsorCorrect <0)	m_ContentMemo.m_nCurrsorCorrect =0;	//20120323 add	
			Update();
			m_ContentMemo.OnPaint();//20110824 add  20120323 move to front
		}
		char* pszTemp = new char[m_nLineNoLen + 1];
		for (int i = 0; i < m_LineNoMemo.GetStrNum(); i++)
		{
			if(m_ContentMemo.m_nHideRow  + i+1 > m_ContentMemo.GetStrNum())
				pszTemp[0]='\0';
			else
				sprintf(pszTemp, "%ld", m_ContentMemo.m_nHideRow  + i+1);
			m_LineNoMemo.m_strlist.Modify(i, pszTemp);	
		}
		delete[] pszTemp;
		m_LineNoMemo.Show();
	}
}

void		CtmMTextMemo::PaintSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol, DWORD dwColor) 
{
	m_ContentMemo.m_bEditFlag = TRUE;
	LoadFromFile(m_pszFile, 0, TRUE);
	m_ContentMemo.PaintSelect(nStartRow, nEndRow, nStartCol, nEndCol, dwColor);
}

void		CtmMTextMemo::DelSelect(int nStartRow, int nEndRow, int nStartCol, int nEndCol)
{
	m_ContentMemo.m_bEditFlag = TRUE;
	LoadFromFile(m_pszFile, 0, TRUE);
	m_ContentMemo.DelSelect(nStartRow, nEndRow, nStartCol, nEndCol);
	SaveToFile(m_pszFile);
}

void		CtmMTextMemo::DelRow(int nIndex)
{
	m_ContentMemo.m_bEditFlag = TRUE;
	LoadFromFile(m_pszFile, 0, TRUE);
	m_ContentMemo.DelRow(nIndex);
	SaveToFile(m_pszFile);
}

int			CtmMTextMemo::LoadFromFile(const char* pszFile, int nStartRow, BOOL bLoadOnceFlag)
{
	//printf("111LoadFromFile---m_bEditFlag:%d  m_bLoadFullFile:%d\n",m_ContentMemo.m_bEditFlag,m_ContentMemo.m_bLoadFullFile);
	#ifndef	D_TEST
		if(m_ContentMemo.m_bEditFlag && (m_ContentMemo.m_bLoadFullFile) )
		{
			//printf("222LoadFromFile---pszFile:%s  Old:%s\n",pszFile,m_pszFile);
			if(g_pRxCNCData != NULL && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MDI_MODE  && strcmp(m_pszFile,pszFile)==0)	return 0;
		}
	#endif
	//printf("333LoadFromFile---pszFile:%s nStartRow:%d  m_bLoadFullFile:%d\n",pszFile,nStartRow,m_ContentMemo.m_bLoadFullFile);
	//strcpy((char*)&m_pszFile,(char*)&pszFile);//20110823 modify //m_pszFile = pszFile;
	m_pszFile = (char*)pszFile;
	m_ContentMemo.m_bLoadFullFile = bLoadOnceFlag;
	return m_ContentMemo.LoadFromFile(m_pszFile, nStartRow,bLoadOnceFlag);
}
		
int			CtmMTextMemo::SaveToFile(const char* pszFile)
{
	/*return*/m_ContentMemo.SaveToFile(pszFile);
	m_ContentMemo.m_bEditFlag = FALSE;
	#ifndef	D_TEST
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MDI_MODE)	LoadFromFile(m_pszFile, 0, TRUE);	//MDI_Mode	20111205 add
	else
	#endif
		LoadFromFile(m_pszFile, m_nStartIndex, FALSE);
	return 1;
}
		
void		CtmMTextMemo::OnGetFocus()
{
	m_ContentMemo.m_bEditFlag = TRUE;
	if(m_pszFile != NULL)	LoadFromFile(m_pszFile,0,TRUE);	//20110810 add	
	
	if (m_pszPrompt != NULL)	Prompt(m_pszPrompt,2);	//20120110 add 0116 add 2
		
	return m_ContentMemo.OnGetFocus();	
}
		
void		CtmMTextMemo::OnLoseFocus()
{
	m_ContentMemo.m_bEditFlag = FALSE;
	return m_ContentMemo.OnLoseFocus();	
}
		
void		CtmMTextMemo::InitLineNoMemo()
{
	//	0. Set the lineno memo parent
	//	1. position & size;
	//	2. show type
	//	3. bgc & fgc
	//	4. font type & taborer
	//	4. CreateA
	//	5. set the fix num string
	
	
	if (m_bLineNoVisable)
	{	
		m_LineNoMemo.m_wnd.rect.left 	= 0;
		m_LineNoMemo.m_wnd.rect.right	= m_LineNoMemo.m_wnd.rect.left +  m_wBorderWidth * 2 + 2+
										(m_pcFont->m_wWidth >> 1) * m_nLineNoLen;
		m_LineNoMemo.m_wnd.rect.top 	= 0;
		m_LineNoMemo.m_wnd.rect.bottom	= m_wnd.rect.bottom - m_wnd.rect.top;
		
		m_LineNoMemo.m_wnd.wStyle		= m_wnd.wStyle;
		
				
		
		m_LineNoMemo.m_iFontType		= m_iFontType;
		m_LineNoMemo.m_taborder			= -2;				
		

		
		m_LineNoMemo.CreateA();	
		
		//	the lineno memo strings num is const-containtrow
		for (int i = m_LineNoMemo.GetStrNum(); i < m_LineNoMemo.m_nContainRow; i++)
			m_LineNoMemo.m_strlist.Add("");
	}
	else
	{
		//	this is becuase the Content Memo need to use the rect to calculate the region
		memset(&m_LineNoMemo.m_wnd.rect, 0, sizeof(m_LineNoMemo.m_wnd.rect));
	}
}

void		CtmMTextMemo::InitContentMemo()
{
		m_ContentMemo.m_wnd.rect.left 	= m_LineNoMemo.m_wnd.rect.right -1;		//20110707 -1
		m_ContentMemo.m_wnd.rect.right	= m_ContentMemo.m_wnd.rect.left  + 
		                                    m_wnd.rect.right -  m_wnd.rect.left - 
		                                    (m_LineNoMemo.m_wnd.rect.right - m_LineNoMemo.m_wnd.rect.left);//-25;
		m_ContentMemo.m_wnd.rect.top 	= 0;
		m_ContentMemo.m_wnd.rect.bottom	= m_wnd.rect.bottom - m_wnd.rect.top;		
		m_ContentMemo.m_wnd.wStyle		= m_wnd.wStyle;		
		//printf("m_ContentMemo.left:%d \n",m_ContentMemo.m_wnd.rect.left);
	    m_ContentMemo.m_fgc				= m_fgc;
	    m_ContentMemo.m_bgc				= m_bgc;	
		m_ContentMemo.m_iFontType		= m_iFontType;
		m_ContentMemo.m_taborder		= -2;
		
		m_ContentMemo.m_bBar = TRUE;
		m_ContentMemo.CreateA();
		
}

void		CtmMTextMemo::GotoLine(int nIndex)
{
	//printf("-----CtmMTextMemo::GotoLine   nIndex:%d \n",nIndex);
	m_nStartIndex = nIndex;
	m_ContentMemo.GotoLine(nIndex);
	LineNoMemoPaint();
}


int		CtmMTextMemo::Find(char* psz)
{
	return m_ContentMemo.Find(psz);
}

int		CtmMTextMemo::Find(int nStartIndex, int nEndIndex, char* psz)
{
	return m_ContentMemo.Find(nStartIndex, nEndIndex, psz);
}

void		CtmMTextMemo::Update()
{
	//OnLoseFocus();		//Mario add 2015/2/23 ?U?E 02:31:01
	InitContentMemo();//20110517 add
	
	Show();
	//OnGetFocus();			//Mario add 2015/2/23 ?U?E 02:31:14
}

void		CtmMTextMemo::Update2()   //!¡ÓiAU?!M¢FDo?j?pRE!A¢FDIUpdate2¢FDi¢FDH-!N¢FD?¢FDu?D|i!Mm!C
{
	//printf("1m_ContentMemo.m_nHideRow is %d\n",m_ContentMemo.m_nHideRow);
	if(m_ContentMemo.m_nContainRow < m_ContentMemo.m_nCurrentRow+1)
	{
		m_ContentMemo.m_nHideRow += m_ContentMemo.m_nCurrentRow-2;
		m_ContentMemo.m_nCurrentRow = 2;
		if(m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow > GetStrNum())
		{
			m_ContentMemo.m_nCurrentRow += m_ContentMemo.m_nHideRow - (GetStrNum() - m_ContentMemo.m_nContainRow);
			m_ContentMemo.m_nHideRow = GetStrNum() - m_ContentMemo.m_nContainRow;
		}
	}
	else if(m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow > GetStrNum())
	{
		int temp = m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow -  GetStrNum();
		if(m_ContentMemo.m_nHideRow >= temp)
		{
			m_ContentMemo.m_nCurrentRow +=temp;
			m_ContentMemo.m_nHideRow-=temp;
		}
		else
		{
			m_ContentMemo.m_nCurrentRow += m_ContentMemo.m_nHideRow;
			m_ContentMemo.m_nHideRow -= m_ContentMemo.m_nHideRow;
		}
	}
	
	//m_ContentMemo.CorrectCurrsorCurrentPos();
	m_ContentMemo.OnPaint();
	LineNoMemoPaint();
	//printf("2m_ContentMemo.m_nHideRow is %d\n",m_ContentMemo.m_nHideRow);
	//Update();
}

void		CtmMTextMemo::SetPropertys()
{
	CtmWnd::SetPropertys();
		
	SetPropInfo("mode", 				tmINT, 		&m_ContentMemo.m_nMode);
	
	SetPropInfo("no_bgc",               tmDWORD,    &m_LineNoMemo.m_bgc);
	SetPropInfo("no_fgc",               tmDWORD,    &m_LineNoMemo.m_fgc);
	
	SetPropInfo("user_row",				tmINT, 		&m_ContentMemo.m_nUserRowWithColor);
	SetPropInfo("user_color",           tmDWORD,    &m_ContentMemo.m_dwLineColor);
	SetPropInfo("user_fgc",           	tmDWORD,    &m_ContentMemo.m_dwLineFGC);
	
	SetPropInfo("colorcontrol", 		tmINT,		&m_ContentMemo.m_nBgcColorControl);
	SetPropInfo("colorchange",			tmINT,		&m_ContentMemo.m_nColorChange);
	SetPropInfo("destcolor",			tmWORD,		&m_ContentMemo.m_wDestColor);
	//SetPropInfo("promptID",				tmSTR,		&m_pszPromptID);	//20120110 add
	//SetPropInfo("Cprivilege",			tmINT,		&m_ContentMemo.m_iPrivilege);	//20120104 add

	//	only read properties
	SetPropInfo("current_row",			tmINT, 		&m_ContentMemo.m_nCurrentRow);
	SetPropInfo("current_col",			tmINT, 		&m_ContentMemo.m_nCurrentCol);
	SetPropInfo("hide_row",				tmINT, 		&m_ContentMemo.m_nHideRow);
	SetPropInfo("hide_col",				tmINT, 		&m_ContentMemo.m_nHideCol);
	SetPropInfo("contain_row",			tmINT, 		&m_ContentMemo.m_nContainRow);
	SetPropInfo("contain_col",			tmINT, 		&m_ContentMemo.m_nContainCol);
	
	SetPropInfo("Scrollbar_bgc",tmDWORD,&m_ContentMemo.m_scrollbar_bgc);		//Mario add 2015/5/21 ?W?E 08:48:52
}

//New add by Roy!o
void		CtmMTextMemo::PaintCurrentRow(int nRow, DWORD dwColor)
{
	m_ContentMemo.m_bPaintCurrentRowFlag	= TRUE;
	m_ContentMemo.PaintSelect(nRow, nRow, 0, m_ContentMemo.GetContainCol(), dwColor);
	m_ContentMemo.m_bPaintCurrentRowFlag	= FALSE;
}
//20140530!o
void		CtmMTextMemo::DelRow_New(int start,int end)
{	
	m_ContentMemo.m_bEditFlag = TRUE;
	//LoadFromFile(m_pszFile, 0, TRUE);
	//printf("start is %d end=%d\n",start,end);
	//printf("m_ContentMemo.m_strlist.GetNum() is %d\n",m_ContentMemo.m_strlist.GetNum());
	int del_line_cnt = end-start+1;
	if(del_line_cnt<0) return;
	if(end < m_ContentMemo.GetStrNum())
	{
		for(int i=0 ; i<del_line_cnt ; i++)
			m_ContentMemo.DelRow(start);
	}
	LineNoMemoPaint();
	m_ContentMemo.OnPaint();
}


void		CtmMTextMemo::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
		case MSG_MOUSELUP:
			MouseUp(wParam >> 16, wParam & 0x0000FFFF, 0);
       		break;
        case MSG_MOUSELDOWN:
        	MouseDown(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        case MSG_MOUSEMOVE:
        	MouseMove(wParam >> 16, wParam & 0x0000FFFF, 0);
        	break;
        default:
        	break;
    }	
}

BOOL		CtmMTextMemo::MouseMove(short x_pos,short y_pos,int key_flag)
{
	/*if(!m_ContentMemo.m_bEditFlag)
		return TRUE;*/
	if(MoveButtonDownFalg)			//?W?U!O2¢FXE?!M¢FDo
	{
		int jumpreg = m_ContentMemo.m_scrollbar.GetIncrement();
		if( (y_pos-Down_Y) > jumpreg)
		{		
			//printf("down y_pos = %d , Down_Y=%d jumpreg = %d\n",y_pos,Down_Y,jumpreg);
			if(m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow < m_ContentMemo.GetStrNum())
			{
				//printf("m_ContentMemo.m_nHideRow = %d , m_ContentMemo.m_nContainRow=%d m_ContentMemo.GetStrNum() = %d\n",
			//	m_ContentMemo.m_nHideRow,m_ContentMemo.m_nContainRow,m_ContentMemo.GetStrNum());
				int line = ((double)(y_pos-Down_Y) / (double)jumpreg)+0.5;
				int next_m_nHideRow = m_ContentMemo.m_nHideRow;
				//if(m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow + line>= GetStrNum())
				//		return TRUE;
				next_m_nHideRow += line;
				if(next_m_nHideRow + m_ContentMemo.m_nContainRow > m_ContentMemo.GetStrNum())
					next_m_nHideRow = m_ContentMemo.GetStrNum()-m_ContentMemo.m_nContainRow;
				if(next_m_nHideRow<0) next_m_nHideRow=0;
				
				line=next_m_nHideRow-m_ContentMemo.m_nHideRow;
				Down_Y += line*jumpreg;
				m_ContentMemo.m_nCurrentRow -= line;
				if(m_ContentMemo.m_nCurrentRow<0)m_ContentMemo.m_nCurrentRow=0;
				m_ContentMemo.m_nHideRow = next_m_nHideRow;
				m_ContentMemo.OnPaint();
				LineNoMemoPaint();
			}
		}
		else if( (y_pos-Down_Y) < -jumpreg )
		{
			if(m_ContentMemo.m_nHideRow > 0)
			{
				int line = ((double)(y_pos-Down_Y) / (double)-jumpreg)+0.5;
				Down_Y -= jumpreg*line;
				m_ContentMemo.m_nCurrentRow += line;
				if(m_ContentMemo.m_nCurrentRow > m_ContentMemo.m_nContainRow )
					m_ContentMemo.m_nCurrentRow = m_ContentMemo.m_nContainRow;
				m_ContentMemo.m_nHideRow -= line;
				if(m_ContentMemo.m_nHideRow<0) m_ContentMemo.m_nHideRow=0;
				m_ContentMemo.OnPaint();
				LineNoMemoPaint();
				 
			}	
		}
	}
	else	if(MoveMemoStrFalg)		//¢FDa¢FDkci|2!O!O!Li
	{
		if(!StrMoveFlag)
		{
			if(abs(x_pos-Down_X) > MEMO_MOUSE_MOVE_RANGE)
				StrMoveFlag = TRUE;
		}
		if(!StrMoveFlag)
		{
			;	
		}
		else if((Down_X-x_pos) > MEMO_MOUSE_MOVE_RANGE1*(m_pcFont->m_wWidth >> 1))
		{
			int old_m_nHideCol = m_ContentMemo.m_nHideCol;
			m_ContentMemo.m_nHideCol+=MEMO_MOUSE_MOVE_RANGE1;
			Down_X-=MEMO_MOUSE_MOVE_RANGE1*(m_pcFont->m_wWidth >> 1);
			if(m_ContentMemo.m_nHideCol + m_ContentMemo.m_nContainCol > NowMaxCol+2)
				m_ContentMemo.m_nHideCol = NowMaxCol - m_ContentMemo.m_nContainCol +2;
			if(m_ContentMemo.m_nHideCol < 0)
				m_ContentMemo.m_nHideCol = 0;
			if(old_m_nHideCol != m_ContentMemo.m_nHideCol)
			{
				m_ContentMemo.m_nCurrentCol -= m_ContentMemo.m_nHideCol -old_m_nHideCol;
				//if(m_ContentMemo.m_nCurrentCol > m_ContentMemo.m_nContainCol)
				//	m_ContentMemo.m_nCurrentCol = m_ContentMemo.m_nContainCol;|1?e|V?¢FG?Y-n¢FX¢Gg3o-O-!N¢FD?
				//m_ContentMemo.CorrectCurrsorCurrentPos();
				m_ContentMemo.OnPaint();
			}
		}
		else if((x_pos-Down_X) > MEMO_MOUSE_MOVE_RANGE1*(m_pcFont->m_wWidth >> 1))
		{
			int old_m_nHideCol = m_ContentMemo.m_nHideCol;
			m_ContentMemo.m_nHideCol-=MEMO_MOUSE_MOVE_RANGE1;
			Down_X+=MEMO_MOUSE_MOVE_RANGE1*(m_pcFont->m_wWidth >> 1);
			if(m_ContentMemo.m_nHideCol < 0)
				m_ContentMemo.m_nHideCol = 0;
			if(old_m_nHideCol != m_ContentMemo.m_nHideCol)
			{
				m_ContentMemo.m_nCurrentCol += old_m_nHideCol - m_ContentMemo.m_nHideCol;
				if(m_ContentMemo.m_nCurrentCol >= m_ContentMemo.m_nContainCol)
					m_ContentMemo.m_nCurrentCol = m_ContentMemo.m_nContainCol-1;
				//m_ContentMemo.CorrectCurrsorCurrentPos();
				m_ContentMemo.OnPaint();
			}
		}
	}
	return TRUE;
}
BOOL		CtmMTextMemo::MouseDown(short x_pos,short y_pos,int key_flag)
{
	if(!m_ContentMemo.m_bEditFlag)
		return TRUE;
	RECT    rect 		= GetWindowRECT();
	RECT		ScrollbarRect  = m_ContentMemo.m_scrollbar.GetWindowRECT();
	RECT 		MoveButtonRect = m_ContentMemo.m_scrollbar.GetMoveButtonRECT();
	
	if(x_pos > ScrollbarRect.left && x_pos < ScrollbarRect.right && y_pos > ScrollbarRect.top && y_pos<ScrollbarRect.bottom && m_ContentMemo.GetStrNum() > m_ContentMemo.m_nContainRow )	//!M!L|bscrollbar
	{
		if(y_pos < ScrollbarRect.top + tmSCROLLBAR_BUTTON)	//up
		{
			m_ContentMemo.m_scrollbar.SetButtonPress(1);
			if(m_ContentMemo.m_nHideRow < 1)
			{
					return TRUE;
			}
			else
			{	
				if(m_ContentMemo.m_nCurrentRow < m_ContentMemo.m_nContainRow-1 )
					m_ContentMemo.m_nCurrentRow++;
				m_ContentMemo.m_nHideRow--;
				
				///m_ContentMemo.ShowCurrsor(FALSE);
				m_ContentMemo.OnPaint();
				//m_ContentMemo.CorrectCurrsorCurrentPos();
				//m_ContentMemo.ShowCurrsor(TRUE);
				LineNoMemoPaint();/*
				static WORD CurrosorCorrect =-1;	//20120323 modify(0)->(-1)
				if(m_ContentMemo.m_nCurrsorCorrect != CurrosorCorrect)
				{
					CurrosorCorrect = m_ContentMemo.m_nCurrsorCorrect;
					if(m_ContentMemo.m_nCurrsorCorrect <0)	m_ContentMemo.m_nCurrsorCorrect =0;	//20120323 add	
					Update();
					m_ContentMemo.CorrectCurrsorCurrentPos();//20110824 add  20120323 move to front
				}*/
			}
		}
		else if(y_pos > ScrollbarRect.bottom - tmSCROLLBAR_BUTTON)	// Down
		{
			m_ContentMemo.m_scrollbar.SetButtonPress(3);
			if(m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow >= GetStrNum())
			{
					return TRUE;
			}
			else
			{
				if(m_ContentMemo.m_nCurrentRow > 0 )
					m_ContentMemo.m_nCurrentRow--;
				m_ContentMemo.m_nHideRow++;
				//m_ContentMemo.ShowCurrsor(FALSE);
				m_ContentMemo.OnPaint();
				///m_ContentMemo.CorrectCurrsorCurrentPos();
				//m_ContentMemo.ShowCurrsor(TRUE);
				LineNoMemoPaint();/*
				static WORD CurrosorCorrect =-1;	//20120323 modify(0)->(-1)
				if(m_ContentMemo.m_nCurrsorCorrect != CurrosorCorrect)
				{
					CurrosorCorrect = m_ContentMemo.m_nCurrsorCorrect;
					if(m_ContentMemo.m_nCurrsorCorrect <0)	m_ContentMemo.m_nCurrsorCorrect =0;	//20120323 add	
					Update();
					m_ContentMemo.CorrectCurrsorCurrentPos();//20110824 add  20120323 move to front
				}*/
			}
		}
		else if(y_pos > MoveButtonRect.top && y_pos < MoveButtonRect.bottom)		//Move
		{
			MoveButtonDownFalg = TRUE;
			m_ContentMemo.m_scrollbar.SetButtonPress(2);
    	Down_Y = y_pos;
		}
		else				//!LS|3?o?sao|a?e
		{
			int	ScrollbarButtonHight = m_ContentMemo.m_scrollbar.GetButtonHight();
			int length = ScrollbarRect.bottom-ScrollbarRect.top - 2*tmSCROLLBAR_BUTTON-ScrollbarButtonHight;
			int pos = y_pos-ScrollbarRect.top - tmSCROLLBAR_BUTTON-ScrollbarButtonHight/2;
			if(length<1)length=1;
			if(pos<1) pos=1;
			if(pos > length) pos = length;
			int temp = (m_ContentMemo.GetStrNum()- m_ContentMemo.m_nContainRow) * pos / length ;
			GotoLine(temp);
		}
	}
	else if (x_pos > m_LineNoMemo.GetWindowRECT().right && x_pos< MoveButtonRect.left && y_pos > rect.top && y_pos < rect.bottom)//A2!O!O|i!Mm|b?s?e¢FXI?o
	{
		Down_X = x_pos;
		MoveMemoStrFalg = TRUE;
		NowMaxCol = 0;
		for (int i=m_ContentMemo.m_nHideRow ; i < m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow && i < GetStrNum(); i++)
			if(NowMaxCol < m_ContentMemo.GetStrLen(i)) NowMaxCol = m_ContentMemo.GetStrLen(i);
	}

	return TRUE;
}

BOOL		CtmMTextMemo::MouseUp(short x_pos,short y_pos,int key_flag)
{
	
	RECT    rect 		= GetWindowRECT();
	RECT		ScrollbarRect  = m_ContentMemo.m_scrollbar.GetWindowRECT();
	int 	i 			= 0;
	int 	j				=0;
	
	if (x_pos > m_LineNoMemo.GetWindowRECT().right && x_pos< ScrollbarRect.left && y_pos > rect.top && y_pos < rect.bottom)//A2!O!O|i!Mm|b?s?e¢FXI?o
	{
		if(m_ContentMemo.m_bEditFlag && !StrMoveFlag)
		{
			//m_ContentMemo.ShowCurrsor(FALSE);
			for (i = 0; i <= m_ContentMemo.m_nContainRow; i++)
			{
				if (y_pos <= rect.top + i * m_pcFont->m_wHeight)		//!¡ÓPA_A2!O!O!M!L2A!|X|a
				break;
			}
			
			if(i <= m_ContentMemo.m_nContainRow)
				m_ContentMemo.m_nCurrentRow=i-1;
				
			for (j = 0; j <=m_ContentMemo.m_nContainCol ; j++)
			{
				if (x_pos <= m_LineNoMemo.GetWindowRECT().right + j *( m_pcFont->m_wWidth >> 1))		//!¡ÓPA_A2!O!O!M!L2A!|X|C
				break;
			}
			
			int old_m_nHideCol = m_ContentMemo.m_nHideCol;
			
			if(m_ContentMemo.m_nHideCol > m_ContentMemo.GetStrLen(m_ContentMemo.m_nCurrentRow + m_ContentMemo.m_nHideRow) -MEMO_MOUSE_MOVE_RANGE2)
				m_ContentMemo.m_nHideCol = m_ContentMemo.GetStrLen(m_ContentMemo.m_nCurrentRow + m_ContentMemo.m_nHideRow)-MEMO_MOUSE_MOVE_RANGE2;
			if (m_ContentMemo.m_nHideCol < 0)
				m_ContentMemo.m_nHideCol = 0;
			if(j <= m_ContentMemo.m_nContainCol)
				m_ContentMemo.m_nCurrentCol=j-1;
				
			//if(old_m_nHideCol != m_ContentMemo.m_nHideCol)
				m_ContentMemo.OnPaint();
		
		
			//m_ContentMemo.CorrectCurrsorCurrentPos();
			//m_ContentMemo.ShowCurrsor(TRUE);
		}
	}
	
	if(!m_ContentMemo.m_bEditFlag)
		return TRUE;
	if (MoveButtonDownFalg)
	{
		MoveButtonDownFalg = FALSE;
	}
	if(MoveMemoStrFalg)
	{
		MoveMemoStrFalg = FALSE;	
		StrMoveFlag = FALSE;
	}
	if( m_ContentMemo.GetStrNum() > m_ContentMemo.m_nContainRow)
		m_ContentMemo.m_scrollbar.SetAllButtonUp();
	
  return TRUE;
}

void		CtmMTextMemo::AddNewStr(char* pszStr)
{
	//m_ContentMemo.ShowCurrsor(FALSE);
	m_ContentMemo.AddNewStr(pszStr);
	//m_ContentMemo.CorrectCurrsorCurrentPos();
	//m_ContentMemo.ShowCurrsor(TRUE);
}

void		CtmMTextMemo::AddNewStr(int nIndex, char* pszStr,BOOL AddHide)
{
	//m_ContentMemo.ShowCurrsor(FALSE);
	if(AddHide)
	{
		m_ContentMemo.AddNewStr(nIndex+m_ContentMemo.m_nHideRow, pszStr);
		m_ContentMemo.m_nCurrentRow = nIndex+m_ContentMemo.m_nHideRow+1;
	}
	else 
	{
		m_ContentMemo.AddNewStr(nIndex, pszStr);
		m_ContentMemo.m_nCurrentRow = nIndex+1;
	}
	m_ContentMemo.m_nCurrentCol = 0;
	m_ContentMemo.m_nHideRow = 0;
	if(m_ContentMemo.m_nCurrentRow >= m_ContentMemo.m_nHideRow + m_ContentMemo.m_nContainRow-1)
	{
		m_ContentMemo.m_nHideRow = m_ContentMemo.m_nCurrentRow - (m_ContentMemo.m_nContainRow-1);
		m_ContentMemo.m_nCurrentRow = m_ContentMemo.m_nContainRow-1;
	}
	m_ContentMemo.m_bEditFlag = m_ContentMemo.m_bLoadFullFile = TRUE;
//printf("index = %d ,  m_ContentMemo.m_nCurrentRow = %d , m_ContentMemo.m_nHideRow = %d \n",nIndex,m_ContentMemo.m_nCurrentRow,m_ContentMemo.m_nHideRow);
	//m_ContentMemo.CorrectCurrsorCurrentPos();
	//m_ContentMemo.ShowCurrsor(TRUE);
}

void		CtmMTextMemo::GetStr(char* pszStr)
{ 
	//printf("m_ContentMemo.m_nCurrentRow+m_ContentMemo.m_nHideRow = %d\n",m_ContentMemo.m_nCurrentRow+m_ContentMemo.m_nHideRow);
	if(m_ContentMemo.GetStrNum() > 0)
		m_ContentMemo.GetStr(m_ContentMemo.m_nCurrentRow+m_ContentMemo.m_nHideRow, pszStr);
}

void		CtmMTextMemo::GetStr(int nIndex, char* pszStr,BOOL AddHide)
{
	//printf("nIndex+m_ContentMemo.m_nHideRow = %d\n",nIndex+m_ContentMemo.m_nHideRow);
	if(AddHide && nIndex+m_ContentMemo.m_nHideRow < m_ContentMemo.GetStrNum())
		m_ContentMemo.GetStr(nIndex+m_ContentMemo.m_nHideRow, pszStr);
	else if(!AddHide && nIndex < m_ContentMemo.GetStrNum())
		m_ContentMemo.GetStr(nIndex, pszStr);
	else pszStr=NULL;
}

void		CtmMTextMemo::AddNewRow()
{
	
}


//?i?UAER!Nao?oReao|a?A(?D?!M¢FDoao|a?A)!A!LA¢FDi3]cwAC|a¢FDH?I3Q?i?Uao?oRe|ico?!M¢FDoao2A!|X|a
void   CtmMTextMemo::SetSelectWithFileLine(int SelectFileLine ,int color ,int SelectOnMemoLine)	
{
	//SelectFileLine -n?i?UaoAER!N?oRe
	//SelectOnMemoLine?i?U	ao?oRe|ico?!M¢FDoao|a?A
	
	
	if(SelectFileLine < 1) 
	{
		SetPropValueT("user_row",-1);
		Update();
		//GotoLine(SelectFileLine-SelectOnMemoLine);
	}
	else 
	{
		if(SelectFileLine > GetStrNum()) SelectFileLine = GetStrNum();	
		
		if(SelectOnMemoLine < 0)  SelectOnMemoLine = AUTO_MODE_SELECT_LINE;
		if(SelectOnMemoLine > m_ContentMemo.m_nContainRow-1) SelectOnMemoLine = AUTO_MODE_SELECT_LINE;
		
		
		if(SelectFileLine < SelectOnMemoLine) SelectOnMemoLine = SelectFileLine;
		
		if ((SelectFileLine + m_ContentMemo.m_nContainRow -SelectOnMemoLine) > GetStrNum() )		
		{
			if(GetStrNum() >= m_ContentMemo.m_nContainRow)
				SelectOnMemoLine = m_ContentMemo.m_nContainRow - (GetStrNum() - SelectFileLine);
			else
				SelectOnMemoLine = SelectFileLine; 
		}	
		
		
		SetPropValueT("user_row",SelectOnMemoLine-1);
		SetPropValueT("user_color",color);
		GotoLine(SelectFileLine-SelectOnMemoLine);
	}
	//Update();
	//PaintCurrentRow(SelectOnMemoLine-1, color);
}   


int CtmMTextMemo::SetSelectStartLine(int start)
{
	if(start < 0 || start > m_ContentMemo.m_nSelectEndRow) return m_ContentMemo.m_nSelectStartRow+1;
	return SetSelectRegion(start,m_ContentMemo.m_nSelectEndRow);
}

int CtmMTextMemo::SetSelectEndLine(int end)
{
	if(end > m_ContentMemo.GetStrNum()-1 || end < m_ContentMemo.m_nSelectStartRow) return m_ContentMemo.m_nSelectEndRow+1;
	return SetSelectRegion(m_ContentMemo.m_nSelectStartRow,end);
}

int CtmMTextMemo::SetSelectRegion(int start , int end)
{
		m_ContentMemo.m_nSelectStartRow = start;
		m_ContentMemo.m_nSelectEndRow = end;
		m_ContentMemo.OnPaint2();
		return 0;
}

void CtmMTextMemo::SetSelectPtr(int* start , int* end)
{
		start = &m_ContentMemo.m_nSelectStartRow;
		end = &m_ContentMemo.m_nSelectEndRow;
}


int CtmMTextMemo::CopyLine()
{
	//clear last copy
	memo_copy* tmp;
	while(copy_start!=NULL)
	{
		tmp=copy_start;
		copy_start = copy_start->next;
		if(tmp->str != NULL)
			free(tmp->str);
		free(tmp);
	}
	tmp=NULL;
	//printf("CopyLine  %d ~ %d \n",m_ContentMemo.m_nSelectStartRow,m_ContentMemo.m_nSelectEndRow);
	for(int i=m_ContentMemo.m_nSelectStartRow ; i <= m_ContentMemo.m_nSelectEndRow ; i++ )
	{
		if(tmp==NULL)
			tmp=(memo_copy*)malloc(sizeof(memo_copy));
		else
		{
			tmp->next=(memo_copy*)malloc(sizeof(memo_copy));
			tmp = tmp->next;
		}
		if(copy_start==NULL) copy_start=tmp;
		tmp->next = NULL;
		tmp->str_len = m_ContentMemo.GetStrLen(i);
		//printf("i = %d tmp->str_len = %d\n",i,tmp->str_len);
		tmp->str=(char*)malloc(tmp->str_len+1);
		if(tmp->str_len>0)
			memcpy(tmp->str,m_ContentMemo.GetString(i),tmp->str_len);
		tmp->str[tmp->str_len]='\0';
		printf("i = %d tmp->str = %s\n",i,tmp->str);

	}
	return 0;
}
int CtmMTextMemo::PatseLine()
{
	if(copy_start==NULL) return-1;
	memo_copy* tmp = copy_start;
	
	m_ContentMemo.ShowCurrsor(FALSE);
	m_ContentMemo.m_nSelectStartRow = m_ContentMemo.m_nCurrentRow + m_ContentMemo.m_nHideRow;
	printf("start m_nCurrentRow = %d   m_nHideRow = %d\n",m_ContentMemo.m_nCurrentRow,m_ContentMemo.m_nHideRow);
	while(tmp != NULL)
	{
		/*while(m_ContentMemo.m_nCurrentRow+1 > m_ContentMemo.m_nContainRow) 
		{
			m_ContentMemo.m_nCurrentRow--;
			m_ContentMemo.m_nHideRow++;
		}*/
		printf("tmp->str = %s\n",tmp->str);
		AddNewStr(m_ContentMemo.m_nCurrentRow, tmp->str);
		//m_ContentMemo.m_nCurrentRow++;
		printf("mid m_nCurrentRow = %d   m_nHideRow = %d\n",m_ContentMemo.m_nCurrentRow,m_ContentMemo.m_nHideRow);
		tmp = tmp->next;
	}
	//m_ContentMemo.m_nCurrentRow--;
	m_ContentMemo.m_nSelectEndRow = m_ContentMemo.m_nCurrentRow + m_ContentMemo.m_nHideRow-1;
	//printf("m_nHideRow = %d\n",m_ContentMemo.m_nHideRow);
	printf("end m_nCurrentRow = %d   m_nHideRow = %d\n",m_ContentMemo.m_nCurrentRow,m_ContentMemo.m_nHideRow);
	printf("m_nSelectStartRow = %d   m_nSelectEndRow = %d\n",m_ContentMemo.m_nSelectStartRow,m_ContentMemo.m_nSelectEndRow);
	LineNoMemoPaint();
	m_ContentMemo.OnPaint2();
	m_ContentMemo.CorrectCurrsorCurrentPos();
	m_ContentMemo.ShowCurrsor(TRUE);
	return 0;
}


//copy_start