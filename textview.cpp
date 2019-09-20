/*
*	Created by J.Wong 2016/3/15 13:31:31
*/

#include "textview.h"
#include "scrollbar.h"
#include "static.h"
#include "lineedit.h"

#include "onlinehelp.h"

const int		MAXLEN=4096*16;		

IMPLEMENT_DYNCREATE(CtmTextView, CtmWnd)

class CtmTextView::CtmPrivateData
{
	public:
		CtmPrivateData():m_title(0),m_hscrollbar(0),m_vscrollbar(0),
		m_footer(0),m_footer_wtype(0x30),m_footer_bgc(39897),m_context_high_color(65513),
		m_footer_fontsize(16),m_footer_fgc(0x3A10),m_footer_align(0x0024),m_isHighShow(FALSE),
		m_context(0),m_titlebgc(39897),m_barbgc(57182),m_bGetFocus(0),m_focus(1),m_titleheight(20),m_titletype(0x27)
		{
			memset(m_szContext,0,sizeof(m_szContext)/sizeof(char));
		}
	public:
		BGC					m_titlebgc;
		BGC					m_barbgc;
		BOOL				m_bGetFocus;
		BOOL				m_focus;
		int					m_titleheight;
		int					m_titletype;
		int					m_footer_align;
		int					m_footer_wtype;
		int					m_footer_bgc;
		int					m_footer_fontsize;
		int					m_footer_fgc;
		int					m_context_high_color;
		BOOL				m_isHighShow;
		char				m_szContext[MAXLEN];
		CtmStaticX2*		m_title;
		CtmScrollBar*		m_hscrollbar;
		CtmScrollBar*		m_vscrollbar;
		CtmLineEdit*		m_context;
		CtmStaticX2*		m_footer;
		
};


CtmTextView::CtmTextView():CtmWnd()
{
	strcpy(TYPEName, "CtmTextView");
	m_data	=	new CtmPrivateData();
	m_data->m_title	=	new CtmStaticX2();
	m_data->m_footer = new CtmStaticX2();
	m_data->m_hscrollbar = new CtmScrollBar();
	m_data->m_vscrollbar = new CtmScrollBar();
	m_data->m_context = new CtmLineEdit(); 
}

CtmTextView::~CtmTextView()
{
	delete m_data->m_title;
	delete m_data->m_hscrollbar;
	delete m_data->m_vscrollbar;
	delete m_data->m_context;
	delete m_data->m_footer;
	delete m_data;
}
	
BOOL 	CtmTextView::CreateA()
{
	CtmWnd::CreateA();
	initTitle();
	initContext();
	initScrollbar();
	
}

void    CtmTextView::Show()
{
	DrawBorder();
	m_data->m_title->Update();
	m_data->m_footer->Update();
	m_data->m_context->SetContent(m_data->m_szContext,strlen(m_data->m_szContext)+1);
	m_data->m_context->StepContent(0,0);
	m_data->m_context->Update();
	m_data->m_hscrollbar->Update();
	m_data->m_vscrollbar->Update();
	
}

void    CtmTextView::Update()
{
		Show();
}

void    CtmTextView::DrawBorder()
{

		CtmWnd::DrawBorder();
}

		
void 	CtmTextView::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("titlecolor",		tmLONG,	 	&m_data->m_titlebgc);
	SetPropInfo("barcolor",			tmLONG,	 	&m_data->m_barbgc);
	SetPropInfo("focus",			tmBOOL,		&m_data->m_focus);
	SetPropInfo("highshow",			tmBOOL,		&m_data->m_isHighShow);
	SetPropInfo("titleheight",      tmLONG,		&m_data->m_titleheight);
	SetPropInfo("titletype",		tmLONG,		&m_data->m_titletype);
	SetPropInfo("footerbgc",		tmLONG,		&m_data->m_footer_bgc);
	SetPropInfo("footerfgc",		tmLONG,		&m_data->m_footer_fgc);
	SetPropInfo("footeralign",		tmWORD,		&m_data->m_footer_align);
	SetPropInfo("footerwtype",		tmWORD,		&m_data->m_footer_wtype);
	SetPropInfo("footerftsize",		tmWORD,		&m_data->m_footer_fontsize);
	SetPropInfo("highcolor",		tmLONG, 	&m_data->m_context_high_color);
}

void	CtmTextView::initScrollbar()
{
	m_data->m_vscrollbar->SetPropertys();
	m_data->m_hscrollbar->SetPropertys();
	m_data->m_vscrollbar->SetParent(this);
	m_data->m_hscrollbar->SetParent(this);
	
	m_data->m_vscrollbar->SetPropValueT("bartype",2);
	m_data->m_vscrollbar->SetPropValueT("left", 	m_wnd.rect.right - m_wnd.rect.left-21);
	m_data->m_vscrollbar->SetPropValueT("top", 		(double)0);
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

void	CtmTextView::initTitle()
{
	m_data->m_title->SetPropertys();
	m_data->m_title->SetParent(this);
	m_data->m_title->SetPropValueT("fontUNWidth",m_iFontUNWidth);
	m_data->m_title->SetPropValueT("fontUNHeight",m_iFontUNHeight);
	m_data->m_title->SetPropValueT("wtype",m_data->m_titletype);
	m_data->m_title->SetPropValueT("align",0x22);
	m_data->m_title->SetPropValueT("left",double(0));
	m_data->m_title->SetPropValueT("top", double(0));
	m_data->m_title->SetPropValueT("right",m_wnd.rect.right - m_wnd.rect.left-21);
	m_data->m_title->SetPropValueT("bottom",5+m_data->m_titleheight);
	m_data->m_title->CalcRECT();
	m_data->m_title->SetPropValueT("fgc",	m_fgc);
	m_data->m_title->SetPropValueT("bgc",	m_data->m_titlebgc);
	m_data->m_title->CreateA();
	
	
	m_data->m_footer->SetPropertys();
	m_data->m_footer->SetParent(this);
	m_data->m_footer->SetPropValueT("fontUNWidth",m_data->m_footer_fontsize);
	m_data->m_footer->SetPropValueT("fontUNHeight",m_data->m_footer_fontsize);
	m_data->m_footer->SetPropValueT("wtype",m_data->m_footer_wtype);
	m_data->m_footer->SetPropValueT("align",m_data->m_footer_align);
	m_data->m_footer->SetPropValueT("left",double(1));
	m_data->m_footer->SetPropValueT("top", m_wnd.rect.bottom-m_wnd.rect.top-40);
	m_data->m_footer->SetPropValueT("right",m_wnd.rect.right - m_wnd.rect.left-21);
	m_data->m_footer->SetPropValueT("bottom",m_wnd.rect.bottom-m_wnd.rect.top-21);
	m_data->m_footer->CalcRECT();
	m_data->m_footer->SetPropValueT("fgc",	m_data->m_footer_fgc);
	m_data->m_footer->SetPropValueT("bgc",m_data->m_footer_bgc);
	m_data->m_footer->CreateA();	
	
}


void	CtmTextView::initContext()
{
	m_data->m_context->SetParent(this);	
	m_data->m_context->SetPropertys();
	m_data->m_context->SetPropValueT("wtype",m_wnd.wStyle);
	m_data->m_context->SetPropValueT("fontUNWidth",m_iFontUNWidth);
	m_data->m_context->SetPropValueT("fontUNHeight",m_iFontUNHeight);
	m_data->m_context->SetPropValueT("left",double(0));
	m_data->m_context->SetPropValueT("top",6+m_data->m_titleheight);
	m_data->m_context->SetPropValueT("right",m_wnd.rect.right-m_wnd.rect.left-20);
	m_data->m_context->SetPropValueT("bottom",m_wnd.rect.bottom-m_wnd.rect.top-40);
	m_data->m_context->CalcRECT();
	m_data->m_context->SetPropValueT("fgc",	m_fgc);
	m_data->m_context->SetPropValueT("bgc",	m_bgc);
	m_data->m_context->SetPropValueT("highcolor",m_data->m_context_high_color);
	m_data->m_context->SetPropValueT("highshow",m_data->m_isHighShow);
	m_data->m_context->CreateA();		
}

void    CtmTextView::OnGetFocus()
{
		if(m_data->m_focus)
		{
			m_data->m_bGetFocus = true;
			CtmWnd::DrawGetFocus();
		}
}

void    CtmTextView::OnLoseFocus()
{
		if(m_data->m_focus)
		{
			m_data->m_bGetFocus = false;
			CtmWnd::DrawLoseFocus();
		}
}

WORD    CtmTextView::OnKey(WORD wKey)
{
	WORD	wResult=wKey;
	//printf("wKey=%x\n",wKey);
	//printf("D:%x U:%x L:%x R:%x\n",_KEY_DOWNFIELD,_KEY_UPFIELD,_KEY_LEFTFIELD,_KEY_RIGHTFIELD);
	switch (wKey)
	{
		case 0x6D00: 	//page down
		case _KEY_DOWNFIELD:
			m_data->m_context->StepContent(0,1);
			m_data->m_vscrollbar->SetMaxReg(m_data->m_context->GetRows());
			m_data->m_vscrollbar->SetPosByStep(1);
			Update();
			wResult=_NULL_KEY;
			break;
		case 0x6800:	//page up
		case _KEY_UPFIELD:
			m_data->m_context->StepContent(0,-1);
			m_data->m_vscrollbar->SetMaxReg(m_data->m_context->GetRows());
			m_data->m_vscrollbar->SetPosByStep(-1);
			Update();
			wResult=_NULL_KEY;
			break;
		case _KEY_LEFTFIELD:
			m_data->m_context->StepContent(-1,0);
			m_data->m_hscrollbar->SetMaxReg(m_data->m_context->GetMaxCols());
			m_data->m_hscrollbar->SetPosByStep(-1);
			Update();
			wResult=_NULL_KEY;
			break;
		case _KEY_RIGHTFIELD:
			m_data->m_context->StepContent(1,0);
			m_data->m_hscrollbar->SetMaxReg(m_data->m_context->GetMaxCols());
			m_data->m_hscrollbar->SetPosByStep(1);
			Update();
			wResult=_NULL_KEY;
			break;
		default:
			break;	
	}
	return wResult;	
}

void	CtmTextView::SetText(char* pszContext)
{
	if(!pszContext)	return;
	m_data->m_context->SetContent(pszContext,strlen(pszContext)+1);
	if((strlen(pszContext)+1)<MAXLEN)
		memcpy(m_data->m_szContext,pszContext,strlen(pszContext)+1);
	else
		memcpy(m_data->m_szContext,pszContext,MAXLEN-1);	
}

void	CtmTextView::SetTitle(char* pszTitle)
{
	if(pszTitle)
	{
		int nLen=strlen(pszTitle);
		char*	temp=new char[nLen+4];
		memset(temp,0,nLen+4);
		memcpy(temp,pszTitle,strlen(pszTitle));
		m_data->m_title->SetPropValueT("text",temp);
		delete[] temp;
	}
}

void	CtmTextView::SetFooter(char* pszFooter)
{
	if(pszFooter)
	{
		int nLen=strlen(pszFooter);
		char*	temp=new char[nLen+4];
		memset(temp,0,nLen+4);
		memcpy(temp,pszFooter,strlen(pszFooter));
		m_data->m_footer->SetPropValueT("text",temp);
		delete[] temp;	
	}
}

void	CtmTextView::SetContext(std::map<int, tmControl> context)
{
	m_data->m_context->SetContext(context);	
}

void	CtmTextView::SetContext(std::map<string, tmControl> context)
{
	m_data->m_context->SetContext(context);	
}

void	CtmTextView::LocalRows(int tab)
{
	if(tab<0)	return ;
	int nRows=m_data->m_context->LocalRow(tab);
	m_data->m_vscrollbar->SetPosByStep(nRows);
	m_data->m_context->StepContent(0,nRows);
}

void	CtmTextView::LocalRows(char* pszName)
{
	if(!pszName)	return;
	int nRows=m_data->m_context->LocalRow(pszName);
	m_data->m_vscrollbar->SetPosByStep(nRows);
	m_data->m_context->StepContent(0,nRows);	
}



	