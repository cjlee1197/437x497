/*
*	created by J.Wong 2016/5/3 14:25:26
*/

#include "pdfview.h"
#include "parsepdf.h"
#include "gui.h"
//#include "cbilinear.h"
#include "scrollbar.h"
#include "static.h"
#include "timer.h"

IMPLEMENT_DYNCREATE(CtmPDFView, CtmWnd)


class CtmPDFView::CtmPrivateData{
	
	public:
		CtmPrivateData():m_bBackGround(0),m_title(0),m_hscrollbar(0),m_vscrollbar(0),m_vpos(0),m_hpos(0),
		m_titlebgc(39897),m_barbgc(57182),m_bGetFocus(0),m_focus(1),m_perPage(0),m_stepPage(0),m_step(0),
		m_path(0),m_titleheight(20),m_titletype(0x27),m_direction(0),m_keyFlag(0),m_ltimer(500),m_zoom(72)
		{
			
		}
		
	public:
		ImageBackground			m_BackImage;
		BOOL					m_bBackGround;
		CtmParsePDF				m_parsepdf;
		CtmStaticX2*			m_title;
		CtmScrollBar*			m_hscrollbar;
		CtmScrollBar*			m_vscrollbar;
		BGC						m_titlebgc;
		BGC						m_barbgc;
		BOOL					m_focus;
		BOOL					m_bGetFocus;
		int						m_perPage;
		int						m_step;
		int						m_vpos;
		int						m_hpos;
		int						m_stepPage;
		char*					m_path;
		int						m_titletype;
		int						m_titleheight;
		
		int						m_direction;
		int						m_keyFlag;
		long					m_ltimer;
		int						m_zoom;
};

CtmPDFView::CtmPDFView()
{
	strcpy(TYPEName, "CtmPDFView");
	m_data=new CtmPrivateData();
	m_data->m_title	=	new CtmStaticX2();
	m_data->m_hscrollbar = new CtmScrollBar();
	m_data->m_vscrollbar = new CtmScrollBar();
	AddTimer(&m_data->m_ltimer);	
}

CtmPDFView::~CtmPDFView()
{
	if(m_data)
	{
		DeleteTimer(&m_data->m_ltimer);
		delete m_data->m_title;
		delete m_data->m_hscrollbar;
		delete m_data->m_vscrollbar;
		delete m_data;
	}
}

BOOL 	CtmPDFView::CreateA()
{
	CtmWnd::CreateA();
	SetBorderWidth(1);
	initTitle();
	initScrollbar();
	if(m_wnd.rect.right - m_wnd.rect.left<=20||m_wnd.rect.bottom - m_wnd.rect.top<=(28+m_data->m_titleheight)) return 0;
	m_data->m_parsepdf.setWidth(m_wnd.rect.right - m_wnd.rect.left-22);
	m_data->m_parsepdf.setHeight(m_wnd.rect.bottom - m_wnd.rect.top-(28+m_data->m_titleheight));
	OpenFile(m_data->m_path);
}

void	CtmPDFView::initTitle()
{
	if(m_wnd.rect.right - m_wnd.rect.left<=1||m_wnd.rect.bottom - m_wnd.rect.top<(5+m_data->m_titleheight))	return ;
	m_data->m_title->SetPropertys();
	m_data->m_title->SetParent(this);
	m_data->m_title->SetPropValueT("fontUNWidth",m_iFontUNWidth);
	m_data->m_title->SetPropValueT("fontUNHeight",m_iFontUNHeight);
	m_data->m_title->SetPropValueT("wtype",m_data->m_titletype);
	m_data->m_title->SetPropValueT("align",0x22);
	m_data->m_title->SetPropValueT("left",double(0));
	m_data->m_title->SetPropValueT("top", double(0));
	m_data->m_title->SetPropValueT("right",m_wnd.rect.right - m_wnd.rect.left-1);
	m_data->m_title->SetPropValueT("bottom",5+m_data->m_titleheight);
	m_data->m_title->CalcRECT();
	m_data->m_title->SetPropValueT("fgc",	m_fgc);
	m_data->m_title->SetPropValueT("bgc",	m_data->m_titlebgc);
	m_data->m_title->CreateA();	
}

void	CtmPDFView::initScrollbar()
{
	if(m_wnd.rect.right - m_wnd.rect.left<=20||m_wnd.rect.bottom-m_wnd.rect.top<=20)	return ;
	m_data->m_vscrollbar->SetPropertys();
	m_data->m_hscrollbar->SetPropertys();
	m_data->m_vscrollbar->SetParent(this);
	m_data->m_hscrollbar->SetParent(this);
	
	m_data->m_vscrollbar->SetPropValueT("bartype",2);
	m_data->m_vscrollbar->SetPropValueT("left", 	m_wnd.rect.right - m_wnd.rect.left-21);
	m_data->m_vscrollbar->SetPropValueT("top", 		5+m_data->m_titleheight);
	m_data->m_vscrollbar->SetPropValueT("right", 	m_wnd.rect.right - m_wnd.rect.left-1);
	m_data->m_vscrollbar->SetPropValueT("bottom",	m_wnd.rect.bottom-m_wnd.rect.top-20);
	m_data->m_vscrollbar->CalcRECT();
	m_data->m_vscrollbar->SetPropValueT("fgc",	m_fgc);
	m_data->m_vscrollbar->SetPropValueT("bgc",	m_data->m_barbgc);
	m_data->m_vscrollbar->CreateA();		
	
	m_data->m_hscrollbar->SetPropValueT("bartype",1);
	m_data->m_hscrollbar->SetPropValueT("left", 	double(0));
	m_data->m_hscrollbar->SetPropValueT("top", 		m_wnd.rect.bottom - m_wnd.rect.top-21);
	m_data->m_hscrollbar->SetPropValueT("right",	m_wnd.rect.right - m_wnd.rect.left);
	m_data->m_hscrollbar->SetPropValueT("bottom",	m_wnd.rect.bottom - m_wnd.rect.top-1);
	m_data->m_hscrollbar->CalcRECT();
	m_data->m_hscrollbar->SetPropValueT("fgc",	m_fgc);
	m_data->m_hscrollbar->SetPropValueT("bgc",	m_data->m_barbgc);
	m_data->m_hscrollbar->CreateA();		
}

void 	CtmPDFView::SetPropertys()
{
	CtmWnd::SetPropertys();	
	SetPropInfo("align",			tmWORD, 	&m_paragraph.wAlign);
	SetPropInfo("titlecolor",		tmLONG,	 	&m_data->m_titlebgc);
	SetPropInfo("barcolor",			tmLONG,	 	&m_data->m_barbgc);
	SetPropInfo("focus",			tmBOOL,		&m_data->m_focus);
	SetPropInfo("path",				tmSTR,		&m_data->m_path);
	SetPropInfo("titleheight",      tmLONG,		&m_data->m_titleheight);
	SetPropInfo("titletype",		tmLONG,		&m_data->m_titletype);
}

void    CtmPDFView::Update()
{
	Show();
	
}

void	CtmPDFView::Show()
{
	//Clear();
	DrawBorder();
	m_data->m_title->Update();
	m_data->m_hscrollbar->Update();
	m_data->m_vscrollbar->Update(); 
	
	_SetViewPort(m_rectWindow.left+2, 
						m_rectWindow.top+8+m_data->m_titleheight, 
						m_rectWindow.right-20, 
						m_rectWindow.bottom-20
					);
	DWORD* temp=(DWORD*)m_data->m_parsepdf.image();
	if(temp)
	{
		for(int j=0; j<m_data->m_parsepdf.height(); j++)
		{
			memcpy(g_Rows[j],temp,sizeof(DWORD)*m_data->m_parsepdf.width());
			temp+=m_data->m_parsepdf.width();
		}
	}
}

void    CtmPDFView::DrawBorder()
{
	if ((m_wnd.wStyle & tmWS_BORDER) == tmWS_BORDER_FRAME)
	{
		short       i;
		short       nRight;
		short       nBottom;
		
		_SetViewPort(	m_rectWindow.left, 
						m_rectWindow.top, 
						m_rectWindow.right, 
						m_rectWindow.bottom
					);
		nRight  = m_rectWindow.right  - m_rectWindow.left;
		nBottom = m_rectWindow.bottom - m_rectWindow.top;
		 _SetColor(m_wndattr.fgcBorder3);
        _Rectangle(_GBORDER, 0, 0, nRight, nBottom);
        _SetColor(0x7FFFFFFF);
        _MoveTo(       0,  nBottom);
        _LineTo(       0,        0);
        _LineTo(  nRight - 0,        0);
        
        _SetColor(m_wndattr.fgcBorder4);
        _MoveTo(nRight-1,        2);
        _LineTo(nRight-1,nBottom-1);
        _LineTo(       2,nBottom-1);
	}
	else
		CtmWnd::DrawBorder();
}

void    CtmPDFView::Clear()
{
	if (m_wnd.wStyle & tmWS_TRANSPARENT)
    {
    	if (!m_data->m_bBackGround)
    	{
        	m_data->m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top,
                              m_rectClient.right, m_rectClient.bottom);
       	 	m_data->m_bBackGround = TRUE;
    	}
   		else
    	{
        	m_data->m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top,
                              m_rectClient.right, m_rectClient.bottom);
    	}

    }
    else
    {
    	CtmWnd::Clear();
    }
}

WORD    CtmPDFView::OnKey(WORD wKey)
{
	WORD	wResult=wKey;
	if(wKey==0x6D00||wKey==0x6800||
		wKey==_KEY_DOWNFIELD||wKey==_KEY_UPFIELD||
		wKey==_KEY_LEFTFIELD||wKey==_KEY_RIGHTFIELD||wKey==46
		||wKey==0x69||wKey==0x6f)
	{
		if(m_data->m_ltimer<=0)
		{
			m_data->m_ltimer=500;	
		}
		else
		{
			//printf("m_data->m_ltimer=%d\n",m_data->m_ltimer);
		 	return _NULL_KEY;
		}	
	}
	
	m_data->m_stepPage=m_data->m_parsepdf.curpage();
	//printf("wKey=%x\n",wKey);
	//printf("D:%x U:%x L:%x R:%x\n",_KEY_DOWNFIELD,_KEY_UPFIELD,_KEY_LEFTFIELD,_KEY_RIGHTFIELD);
	switch (wKey)
	{
		case 0x69:
			ZoomIn();
			wResult=_NULL_KEY;
			break;
		case 0x6f:
			ZoomOut();
			wResult=_NULL_KEY;
			break;
		case 0x6D00: 	//page down
			PageDown();
			wResult=_NULL_KEY;
			break;	
		case _KEY_DOWNFIELD:
			m_data->m_direction=0;
			m_data->m_parsepdf.step(0,m_data->m_parsepdf.height());
			m_data->m_vscrollbar->SetPosByStep(m_data->m_parsepdf.height());
			Update();
			wResult=_NULL_KEY;
			break;
		case 0x6800:	//page up
			 PageUp();
			 wResult=_NULL_KEY;
			break;	
		case _KEY_UPFIELD:
			m_data->m_direction=1;
			m_data->m_parsepdf.step(0,-m_data->m_parsepdf.height());
			m_data->m_vscrollbar->SetPosByStep(-m_data->m_parsepdf.height());
			Update();
			wResult=_NULL_KEY;
			break;
		case _KEY_LEFTFIELD:
			m_data->m_parsepdf.step(-40,0);
			m_data->m_hscrollbar->SetPosByStep(-40);
			
			Update();
			wResult=_NULL_KEY;
			break;
		case _KEY_RIGHTFIELD:
			m_data->m_parsepdf.step(40,0);
			m_data->m_hscrollbar->SetPosByStep(40);
			Update();
			wResult=_NULL_KEY;
			break;
		case 46:
			if(m_data->m_direction)
			{
				PageUp();	
			}
			else
			{
				PageDown();	
			}
			wResult=_NULL_KEY;
			break;
		default:
			break;	
	}
	return wResult;		
}

void    CtmPDFView::OnGetFocus()
{
	if(m_data->m_focus)
	{
			m_data->m_bGetFocus = true;
			CtmWnd::DrawGetFocus();
	}
}
void    CtmPDFView::OnLoseFocus()
{
	if(m_data->m_focus)
	{
		m_data->m_bGetFocus = false;
		CtmWnd::DrawLoseFocus();
	}
}

void	CtmPDFView::OpenFile(char* pszFile)
{
	m_data->m_parsepdf.openFile(pszFile);
	m_data->m_perPage=m_data->m_parsepdf.pageHeight();/*m_wnd.rect.bottom - m_wnd.rect.top-(28+m_iFontUNHeight);*/
	m_data->m_step=m_wnd.rect.bottom - m_wnd.rect.top-(28+m_iFontUNHeight);
	if(m_data->m_parsepdf.pages())
	{
		char* psz=strrchr(pszFile,'/');
		if(!psz)	psz=pszFile;
		else		++psz;
		SetTitle(psz);
		m_data->m_vscrollbar->SetMaxReg(m_data->m_parsepdf.pages()*m_data->m_perPage);
		m_data->m_hscrollbar->SetMaxReg(m_data->m_parsepdf.pageWidth());
	}
}

void	CtmPDFView::SetTitle(char* pszTitle)
{
	if(pszTitle)
	{
		int nLen=strlen(pszTitle);
		char*	temp=new char[nLen+4];
		memset(temp,0,nLen+4);
		memcpy(temp,pszTitle,strlen(pszTitle));
		m_data->m_title->SetPropValueT("text",temp);
		delete temp;
	}
}


void	CtmPDFView::PageDown()
{
	if(m_data->m_stepPage>=m_data->m_parsepdf.pages())
		;
	else
		m_data->m_parsepdf.selectPage(++m_data->m_stepPage);
	m_data->m_vscrollbar->SetPosByStep(m_data->m_step);
	m_data->m_vpos=m_data->m_vscrollbar->GetPos();
	Update();
}

void	CtmPDFView::PageUp()
{
	if(m_data->m_stepPage<=0)
		;
	else
		m_data->m_parsepdf.selectPage(--m_data->m_stepPage);
	m_data->m_vscrollbar->SetPosByStep(-m_data->m_step);
	m_data->m_vpos=m_data->m_vscrollbar->GetPos();
	Update();
}

void	CtmPDFView::ZoomOut()
{
	m_data->m_hpos=m_data->m_hscrollbar->GetPos()/m_data->m_zoom;
	m_data->m_vpos=m_data->m_vscrollbar->GetPos()/m_data->m_zoom;
	m_data->m_zoom-=9;
	if(m_data->m_zoom<=9) m_data->m_zoom=9;
	m_data->m_parsepdf.setZoom(m_data->m_zoom);
	m_data->m_perPage=m_data->m_parsepdf.pageHeight();
	m_data->m_vscrollbar->SetMaxReg(m_data->m_parsepdf.pages()*m_data->m_perPage);
	m_data->m_hscrollbar->SetMaxReg(m_data->m_parsepdf.pageWidth()/*(m_rectWindow.right-m_rectWindow.left-20)*/);	
	m_data->m_parsepdf.selectPage(m_data->m_stepPage);
	m_data->m_vscrollbar->SetPos(m_data->m_vpos*m_data->m_zoom);
	m_data->m_hscrollbar->SetPos(m_data->m_hpos*m_data->m_zoom);
	Update();
}
void    CtmPDFView::ZoomIn()
{
	m_data->m_hpos=m_data->m_hscrollbar->GetPos()/m_data->m_zoom;
	m_data->m_vpos=m_data->m_vscrollbar->GetPos()/m_data->m_zoom;
	m_data->m_zoom+=9;
	if(m_data->m_zoom>=144) m_data->m_zoom=144;
	m_data->m_parsepdf.setZoom(m_data->m_zoom);
	m_data->m_perPage=m_data->m_parsepdf.pageHeight();
	m_data->m_vscrollbar->SetMaxReg(m_data->m_parsepdf.pages()*m_data->m_perPage);
	m_data->m_hscrollbar->SetMaxReg(m_data->m_parsepdf.pageWidth()/*(m_rectWindow.right-m_rectWindow.left-20)*/);
	m_data->m_parsepdf.selectPage(m_data->m_stepPage);
	m_data->m_vscrollbar->SetPos(m_data->m_vpos*m_data->m_zoom);
	m_data->m_hscrollbar->SetPos(m_data->m_hpos*m_data->m_zoom);
	Update();	
}

