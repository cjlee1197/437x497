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
|  Creation : 20/10/2008                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include 	"scrollbar.h"
#include	"tmconfig.h"
#include	"utils.h"
#include	"tmshmsg.h"
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
IMPLEMENT_DYNCREATE(CtmScrollBar, CtmWnd)
CtmScrollBar::CtmScrollBar():CtmWnd()
{
	strcpy(TYPEName, "CtmScrollBar");
	m_dwRange			= 10;
	m_dwPosition			= 0;
	m_wScrollbarType	= tmSCROLLBAR_V;
	m_bFirstPaintFixed  = FALSE;
	//	the following variant, they would get the values in the CalculateReg function
	m_bPaintFixed		= FALSE;
	m_bPaintUnFixed		= FALSE;
	m_wIncrement		= 0;
	m_wReg				= 0;
	m_buttonHight = tmSCROLLBAR_BUTTON1;
	
  MoveButtomFlag = FALSE;
	MoveButtomPos_Y = -1;
	
}

CtmScrollBar::~CtmScrollBar()
{
	m_button1.SetParent(NULL);
	m_button2.SetParent(NULL);
	m_button.SetParent(NULL);
}

BOOL CtmScrollBar::CreateA()
{
	CtmWnd::CreateA();
		
	//	===============================================
	//
	//	===============================================
	CalculateReg();	
	if(CtmConfig::GetInstance()->GetBinaryType() == 1)
		SetButtonPropertys_Nub();
	else
		SetButtonPropertys();
}

/*-----------------------------------------------------------------------------+
|  				Operations									  |
+-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
| Function  : Create()									          |
| Task	  : 					 								|
+----------------------------------------------------------------------------+
|  Call     : 	                                  						|
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmScrollBar::Show()           /* Virtual function */
{
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
		Clear();
		Paint();
	}
	else CtmWnd::Clear();
}

void    CtmScrollBar::Update()
{
	Show();	
}

void   	CtmScrollBar::Clear()
{
	_SetViewPort(m_rectClient.left, 
				m_rectClient.top ,
				m_rectClient.right,
				m_rectClient.bottom);
    _SetColor(0);    
    _Rectangle(_GBORDER, 0, 0, GetClientRight(), GetClientBottom());
   	
    
    _SetViewPort(m_rectClient.left, 
				m_rectClient.top ,
				m_rectClient.right,
				m_rectClient.bottom);	
    _SetColor(m_bgc);
    _Rectangle(_GFILLINTERIOR, 1, 1, GetClientRight() - 1, GetClientBottom() - 1);
	
}

void	CtmScrollBar::SetMaxReg(DWORD dwReg)
{
	if(dwReg<2)	m_dwRange=2;
	m_dwRange	= dwReg;
}

void	CtmScrollBar::SetPos(DWORD dwPos)
{
    if(dwPos<=m_dwRange&&dwPos>=0)
		m_dwPosition	= dwPos;
}

void	CtmScrollBar::SetPosByStep(int nStep)
{
	 if((m_dwPosition+nStep)<m_dwRange&&(m_dwPosition+nStep)>=0)
		m_dwPosition	+= nStep;
		
}
DWORD   CtmScrollBar::GetPos()
{
		return m_dwPosition;
}

void 	CtmScrollBar::SetPropertys()
{
	CtmWnd::SetPropertys();
		
	SetPropInfo("barrange",		tmDWORD,		&m_dwRange);
	SetPropInfo("barposition",	tmDWORD,		&m_dwPosition);
	SetPropInfo("bartype",		tmWORD,			&m_wScrollbarType);
	
}

void 	CtmScrollBar::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
		
	SetPropInfo(SCROLLBAR_BARRANGE,		tmWORD,		&m_dwRange);
	SetPropInfo(SCROLLBAR_POSITION,		tmWORD,		&m_dwPosition);
	SetPropInfo(SCROLLBAR_TYPE,			tmWORD,		&m_wScrollbarType);
	
}

void	CtmScrollBar::Paint()
{
	CalculateReg();
	PaintFixedButton();
	PaintUnFixedButton();
}

void	CtmScrollBar::PaintFixedButton()
{
	if (m_bPaintFixed)
	{
		m_button1.Update();
		m_button2.Update();
		DrawUp();
		DrawDown();
	}	
}

//	todo ::
void	CtmScrollBar::DrawUp()
{
	;//printf("Todo CtmScrollBar::DrawUp() \n");
}

//	todo ::
void	CtmScrollBar::DrawDown()
{
	;//printf("Todo CtmScrollBar::DrawDown() \n");
}

void	CtmScrollBar::PaintUnFixedButton()
{
	if (m_bPaintUnFixed)
	{
		ClearUnFixedButton();
		
		if(m_wScrollbarType==tmSCROLLBAR_H)
		{
			if(m_dwRange< 2)	m_dwRange=2;
			m_buttonRect.left 	=tmSCROLLBAR_BUTTON+1+ m_dwPosition*(m_wReg - tmSCROLLBAR_BUTTON* 2-tmSCROLLBAR_BUTTON1-5)/(m_dwRange-1)/*m_wIncrement*/;
			m_buttonRect.top 	= 1;
			m_buttonRect.right 	= m_buttonRect.left + tmSCROLLBAR_BUTTON1;
			m_buttonRect.bottom	= m_wnd.rect.bottom - m_wnd.rect.top - 3;
		}
		else
		{
			//printf("m_wnd.rect.bottom=%d,m_wnd.rect.top=%d\n",m_wnd.rect.bottom,m_wnd.rect.top);
			if(m_dwRange< 2) m_dwRange=2;
		m_buttonRect.left 	= 1;
		m_buttonRect.top 	= tmSCROLLBAR_BUTTON + 1 + ((m_wReg - tmSCROLLBAR_BUTTON * 2 - m_buttonHight-1) * m_dwPosition / m_dwRange);
		m_buttonRect.right 	= m_wnd.rect.right - m_wnd.rect.left - 3;
		m_buttonRect.bottom	= m_buttonRect.top + m_buttonHight;
			//printf("m_buttonRect.top=%d,m_buttonRect.bottom=%d\n",m_buttonRect.top,m_buttonRect.bottom);
		}
	
		m_button.SetPropValueT("left",		m_buttonRect.left);
		m_button.SetPropValueT("top",		m_buttonRect.top);
		m_button.SetPropValueT("right",		m_buttonRect.right);
		m_button.SetPropValueT("bottom",	m_buttonRect.bottom);
		m_button.CalcRECT();
		m_button.Update();
		
	}
}

void	CtmScrollBar::CalculateReg()
{
	if(m_dwRange<2)	m_dwRange=2;
	switch (m_wScrollbarType)
	{
		case tmSCROLLBAR_V:
			m_wReg = m_rectClient.bottom - m_rectClient.top + 1;
			break;
		case tmSCROLLBAR_H:
			m_wReg = m_rectClient.right - m_rectClient.left + 1;
			break;
		default:
			perror("CtmScrollBar::CalculateReg() Error!\n");
			break;
	}
	
	if (m_wReg < tmSCROLLBAR_BUTTON * 2)
	{
		m_bPaintFixed	= FALSE;
		m_bPaintUnFixed	= FALSE;
	}
	else if (m_wReg < tmSCROLLBAR_BUTTON * 2 + m_buttonHight)
	{
		m_bPaintFixed	= TRUE;
		m_bPaintUnFixed	= FALSE;

		if (m_dwRange > 0)
		{
			m_wIncrement	= (m_wReg - tmSCROLLBAR_BUTTON * 2) / (m_dwRange-1);
			
		}
		else
		{ 
			m_wIncrement 	= 0;
		}
	}
	else
	{
		m_bPaintFixed	= TRUE;
		m_bPaintUnFixed	= TRUE;

		if (m_dwRange > 0)
		{
			m_wIncrement	= (m_wReg - tmSCROLLBAR_BUTTON * 2 - m_buttonHight) / m_dwRange;
		}
		else
		{
			m_wIncrement 	= 0;
			m_bPaintUnFixed	= FALSE;
		}
	}
	
//	if (m_wIncrement == 0 && m_dwRange > 0)	m_wIncrement = 1;
		
	
	

}

void	CtmScrollBar::ClearUnFixedButton()
{
	//m_button.Clear();
}

void	CtmScrollBar::SetButtonPropertys()
{
	m_button1.SetParent(this);
	m_button2.SetParent(this);
	m_button.SetParent(this);
	m_button1.SetPropertys();
	m_button2.SetPropertys();
	m_button.SetPropertys();
	if(m_wScrollbarType==tmSCROLLBAR_H)
	{
		m_button1.SetPropValueT("left", 	1);
		m_button1.SetPropValueT("top", 		1);
		m_button1.SetPropValueT("right", 	tmSCROLLBAR_BUTTON + 1);
		m_button1.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top - 3);
	}
	else
	{
		m_button1.SetPropValueT("left", 	1);
		m_button1.SetPropValueT("top", 		1);
		m_button1.SetPropValueT("right", 	m_wnd.rect.right - m_wnd.rect.left - 3);
		m_button1.SetPropValueT("bottom",	tmSCROLLBAR_BUTTON + 1);
	}
	m_button1.SetPropValueT("bgc",		57051);
	m_button1.SetPropValueT("wtype",	tmWS_SHOW);
	m_button1.CreateA();
	
	if(m_wScrollbarType==tmSCROLLBAR_H)
	{
		m_button2.SetPropValueT("left",		m_wnd.rect.right - tmSCROLLBAR_BUTTON-3);
		m_button2.SetPropValueT("top",		1);
		m_button2.SetPropValueT("right",	m_wnd.rect.right-3);
		m_button2.SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top - 3);
	}
	else
	{
		m_button2.SetPropValueT("left",		1);
		m_button2.SetPropValueT("top",		m_wnd.rect.bottom -m_wnd.rect.top-tmSCROLLBAR_BUTTON-3);
		m_button2.SetPropValueT("right",	m_wnd.rect.right - m_wnd.rect.left - 3);
		m_button2.SetPropValueT("bottom",	m_wnd.rect.bottom- m_wnd.rect.top- 3);
	}
	m_button2.SetPropValueT("bgc",		57051);
	m_button2.SetPropValueT("wtype",	tmWS_SHOW);
	m_button2.CreateA();

	if(m_wScrollbarType==tmSCROLLBAR_H)
	{
		m_buttonRect.left 	= tmSCROLLBAR_BUTTON + 1 + m_dwPosition * m_wIncrement;
		m_buttonRect.top 	= 1;
		m_buttonRect.right 	= m_buttonRect.left + tmSCROLLBAR_BUTTON1;
		m_buttonRect.bottom	= m_wnd.rect.bottom - m_wnd.rect.top - 3;
	}
	else
	{
		m_buttonRect.left 	= 1;
		m_buttonRect.top 	= tmSCROLLBAR_BUTTON + 1 + ((m_wReg - tmSCROLLBAR_BUTTON * 2 - m_buttonHight-1) * m_dwPosition /m_dwRange);
		m_buttonRect.right 	= m_wnd.rect.right - m_wnd.rect.left - 3;
		m_buttonRect.bottom	= m_buttonRect.top + m_buttonHight;
	}
	m_button.SetPropValueT("left",		m_buttonRect.left);
	m_button.SetPropValueT("top",		m_buttonRect.top);
	m_button.SetPropValueT("right",		m_buttonRect.right);
	m_button.SetPropValueT("bottom",	m_buttonRect.bottom);
		m_button.SetPropValueT("bgc",		57051/*0xFFFF*/);
	m_button.SetPropValueT("wtype",		tmWS_SHOW);
	
	m_button.CreateA();
}

RECT	CtmScrollBar::GetMoveButtonRECT()
{
	return m_button.GetWindowRECT();
}
void	CtmScrollBar::SetButtonPropertys_Nub()
{
	m_button1.SetParent(this);
	m_button2.SetParent(this);
	m_button.SetParent(this);
	m_button1.SetPropertys_Nub();
	m_button2.SetPropertys_Nub();
	m_button.SetPropertys_Nub();
	
	m_button1.SetPropValueT(WND_LEFT, 		1);
	m_button1.SetPropValueT(WND_TOP, 		1);
	m_button1.SetPropValueT(WND_RIGHT, 		m_wnd.rect.right - m_wnd.rect.left - 3);
	m_button1.SetPropValueT(WND_BOTTOM,		tmSCROLLBAR_BUTTON + 1);
	m_button1.SetPropValueT(WND_BGC,		57051/*0xFFFF*/);
	m_button1.SetPropValueT(WND_WTYPE,		tmWS_SHOW);
	m_button1.CreateA();
	
	
	m_button2.SetPropValueT(WND_LEFT,		1);
	m_button2.SetPropValueT(WND_TOP,		m_wnd.rect.bottom - tmSCROLLBAR_BUTTON);
	m_button2.SetPropValueT(WND_RIGHT,		m_wnd.rect.right - m_wnd.rect.left - 3);
	m_button2.SetPropValueT(WND_BOTTOM,		m_wnd.rect.bottom - 2);
	m_button2.SetPropValueT(WND_BGC,		57051/*0xFFFF*/);
	m_button2.SetPropValueT(WND_WTYPE,		tmWS_SHOW);
	m_button2.CreateA();


	m_buttonRect.left 	= 1;
	m_buttonRect.top 	= tmSCROLLBAR_BUTTON + 1 + ((m_wReg - tmSCROLLBAR_BUTTON * 2 - m_buttonHight-1) * m_dwPosition / m_dwRange);
	m_buttonRect.right 	= m_wnd.rect.right - m_wnd.rect.left - 2;
	m_buttonRect.bottom	= m_buttonRect.top + m_buttonHight;
	
	m_button.SetPropValueT(WND_LEFT,		m_buttonRect.left);
	m_button.SetPropValueT(WND_TOP,			m_buttonRect.top);
	m_button.SetPropValueT(WND_RIGHT,		m_buttonRect.right);
	m_button.SetPropValueT(WND_BOTTOM,		m_buttonRect.bottom);
	m_button.SetPropValueT(WND_BGC,			57051/*0xFFFF*/);
	m_button.SetPropValueT(WND_WTYPE,		tmWS_SHOW);
	
	m_button.CreateA();
}


WORD	CtmScrollBar::GetIncrement()
{
	return m_wIncrement;
}

void	CtmScrollBar::SetButtonHight(int MaxRows)
{
		m_buttonHight = (m_wReg - tmSCROLLBAR_BUTTON * 2)*MaxRows/(m_dwRange+MaxRows);
		
}

void 	CtmScrollBar::SetButtonPress(int wButton)
{
	switch (wButton)
	{
		case 1:	//UP Button
			if(m_bPaintFixed)
				m_button1.Press(tmBUTTON_DOWN);
			break;
		case 2:	//Move Button
			if(m_bPaintUnFixed)
				m_button.Press(tmBUTTON_DOWN);
			break;
		case 3:	//Down Button
			if(m_bPaintFixed)
				m_button2.Press(tmBUTTON_DOWN);
			break;
	}
	
}

void 	CtmScrollBar::SetAllButtonUp()
{
	if(m_bPaintUnFixed)
		m_button.Press(tmBUTTON_UP);
		
	if(m_bPaintFixed)
	{
		m_button1.Press(tmBUTTON_UP);
		m_button2.Press(tmBUTTON_UP);	
	}
}

void		CtmScrollBar::WndProc(int message, WPARAM wParam, LPARAM lParam)
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

BOOL		CtmScrollBar::MouseMove(short x_pos,short y_pos,int key_flag)
{
	if (MoveButtomFlag)
	{
		if(y_pos > MoveButtomPos_Y + GetIncrement())
		{
			if(m_dwPosition < m_dwRange)
			{
				MoveButtomPos_Y = MoveButtomPos_Y + GetIncrement();
				m_dwPosition++;
				Update();
				//SendMsg(MSG_SH_APP_SCROLLBAR,m_wPosition,NULL,NULL);
			}
		}
		else if (y_pos < MoveButtomPos_Y - GetIncrement())
		{
			if(m_dwPosition > 0)
			{
				MoveButtomPos_Y = MoveButtomPos_Y - GetIncrement();
				m_dwPosition--;
				Update();
				//SendMsg(MSG_SH_APP_SCROLLBAR,m_wPosition,NULL,NULL);
			}
		}
	}
	return TRUE;
}

BOOL		CtmScrollBar::MouseDown(short x_pos,short y_pos,int key_flag)
{
	RECT 	rect = GetWindowRECT();
  RECT  ButtonRect = GetMoveButtonRECT();
  
	if(x_pos > rect.left && x_pos < rect.right && y_pos > rect.top && y_pos < rect.bottom)
	{
		if(y_pos < rect.top + tmSCROLLBAR_BUTTON)
		{
			//UP
			SetButtonPress(1);
			if(m_dwPosition > 0)
			{
				m_dwPosition--;
				Update();
				//SendMsg(MSG_SH_APP_SCROLLBAR,m_wPosition,NULL,NULL);
			}
		}
		else if(y_pos > rect.bottom - tmSCROLLBAR_BUTTON )
		{
			//Down
			SetButtonPress(3);
			if(m_dwPosition < m_dwRange)
			{
				m_dwPosition++;
				Update();
				//SendMsg(MSG_SH_APP_SCROLLBAR,m_wPosition,NULL,NULL);
			}
		}
		else if(y_pos > ButtonRect.top && y_pos < ButtonRect.bottom)
		{
			//Move
			MoveButtomFlag =TRUE;
			MoveButtomPos_Y = y_pos;
			SetButtonPress(2);
		}
		
	}
	return TRUE;
}

BOOL		CtmScrollBar::MouseUp(short x_pos,short y_pos,int key_flag)
{
	SetAllButtonUp();
	MoveButtomFlag = FALSE;
	return TRUE;
}