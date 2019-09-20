/*
*	created by J.Wong 2016/5/3 14:21:58
*/

#include <poppler-document.h>
#include <poppler-image.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>
#include <errno.h>
#include <stdio.h>
#include "parsepdf.h"
#include <string.h>
using namespace std;


class CtmParsePDF::CtmPrivateData
{
	public:
		CtmPrivateData():m_height(0),m_width(0),m_curpage(0),m_maxpages(0),m_doc(0),m_hstep(0),m_vstep(0),
			m_zoom(72),m_pageHeight(0),m_pageWidth(0)
		{
			memset(&m_originPage,0,sizeof(m_originPage));	
		}
		~CtmPrivateData()
		{
			if(m_doc)
				delete m_doc;
		}
	public:
		int	m_height;
		int m_width;
		int m_vstep;
		int m_hstep;
		int	m_curpage;
		int m_maxpages;
		int m_zoom;
		TMPAGE m_originPage;
		int    m_pageHeight;
		int    m_pageWidth;
		poppler::image		m_image;
		poppler::document* 	m_doc;
			
};


CtmParsePDF::CtmParsePDF()
{
	m_data=new CtmPrivateData();	
}

CtmParsePDF::~CtmParsePDF()
{
	delete m_data;	
}


void	CtmParsePDF::openFile(char* pszFile)
{
	
	m_data->m_maxpages=0;
	if(!pszFile)	return ;
	if (!poppler::page_renderer::can_render()) 
	{
        perror("renderer compiled without Splash support");
        return ;
    }
	string strFile=string(pszFile);
	m_data->m_doc=poppler::document::load_from_file(strFile);
	if (!m_data->m_doc) 
	{
        perror("loading error");
        return;
    }
    if (m_data->m_doc->is_locked()) 
    {
        perror("encrypted document");
        return ;
    }
    if (m_data->m_curpage < 0 || m_data->m_curpage >= m_data->m_doc->pages()) 
    {
        perror("specified page number out of page count");
        return ;
    }
    m_data->m_originPage=origin();
    setPageHeight(m_data->m_originPage.height*m_data->m_zoom);
    setPageWidth(m_data->m_originPage.width*m_data->m_zoom);
    m_data->m_maxpages=m_data->m_doc->pages()-1;
    selectPage(0);
}

char*	CtmParsePDF::image()
{
	if(!m_data->m_image.is_valid())
	{
		perror("rendering failed");
		return 0;
	}
	return m_data->m_image.data();	
}

int		CtmParsePDF::height()
{
	return m_data->m_image.height();
}

int		CtmParsePDF::width()
{
	return m_data->m_image.width();
}

int		CtmParsePDF::selectPage(int nPage)
{
	m_data->m_vstep=0;
	m_data->m_hstep=0;
	stepInPage(nPage);
	
}

int		CtmParsePDF::pages()
{
	return m_data->m_maxpages;
}

int		CtmParsePDF::bytes_per_row()
{
	return m_data->m_image.bytes_per_row();
}

int		CtmParsePDF::setHeight(int height)
{
	m_data->m_height=height;
	return m_data->m_height;	
}

int		CtmParsePDF::setWidth(int width)
{
	m_data->m_width=width;
	return m_data->m_width;
}

void	CtmParsePDF::step(int hstep,int vstep)
{
	m_data->m_hstep+=hstep;
	if(m_data->m_hstep<0)	m_data->m_hstep=0;
	if(m_data->m_hstep>m_data->m_pageWidth) m_data->m_hstep=m_data->m_pageWidth;
	//printf("m_vstep=%d,vstep=%d\n",m_data->m_vstep,vstep);
	m_data->m_vstep+=vstep;
	//printf("m_vstep=%d\n",m_data->m_vstep);
	if(m_data->m_vstep<0)
	{
		m_data->m_vstep=m_data->m_pageHeight+vstep;
		m_data->m_curpage--;
		if(m_data->m_curpage<0)
		{
			m_data->m_vstep=0;
			m_data->m_curpage=0;
		}
	}
	else if(m_data->m_vstep>=m_data->m_pageHeight)
	{
		 m_data->m_vstep=0;
		 m_data->m_curpage++;
		if(m_data->m_curpage>m_data->m_maxpages)
		{	
			m_data->m_vstep=m_data->m_pageHeight;
			m_data->m_curpage=m_data->m_maxpages;
		}
	}
	//printf("m_data->curpage=%d\n",m_data->m_curpage);
	stepInPage(m_data->m_curpage);	
}

int		CtmParsePDF::stepInPage(int nPage)
{
	m_data->m_curpage=nPage;
	if(nPage<0)	m_data->m_curpage=0;
	if(nPage>m_data->m_maxpages) m_data->m_curpage=m_data->m_maxpages;
	if(!m_data->m_doc)
	{
		return  -1;	
	}
	std::auto_ptr<poppler::page> p(m_data->m_doc->create_page(m_data->m_curpage));
    
    if (!p.get()) 
    {
        perror("null page");
        return -1;
    }
    poppler::page_renderer pr;
    pr.set_render_hint(poppler::page_renderer::antialiasing, true);
    pr.set_render_hint(poppler::page_renderer::text_antialiasing, true);
    //printf("page=%d\n",nPage);
    m_data->m_image = pr.render_page(p.get(),m_data->m_zoom,m_data->m_zoom,m_data->m_hstep,m_data->m_vstep,m_data->m_width,m_data->m_height);
    return m_data->m_curpage;	
}

int		CtmParsePDF::curpage()
{
	return m_data->m_curpage;	
}

void	CtmParsePDF::setZoom(int zoom)
{
	 m_data->m_zoom=zoom;
	 setPageHeight(m_data->m_originPage.height*m_data->m_zoom);
	 setPageWidth(m_data->m_originPage.width*m_data->m_zoom);	
}


inline TMPAGE	CtmParsePDF::origin()
{
	TMPAGE page;
	memset(&page,0,sizeof(page));
	std::auto_ptr<poppler::page> p(m_data->m_doc->create_page(0));
    
    if (!p.get()) 
    {
        perror("null page");
        return page;
    }
    poppler::page_renderer pr;
    pr.set_render_hint(poppler::page_renderer::antialiasing, true);
    pr.set_render_hint(poppler::page_renderer::text_antialiasing, true);
    poppler::image image = pr.render_page(p.get(),1,1,-1,-1,-1,-1);
    
    page.width=image.width();
    page.height=image.height();
    return page;	
}

int		CtmParsePDF::pageHeight()
{
	return m_data->m_pageHeight;	
}

int		CtmParsePDF::pageWidth()
{
	return m_data->m_pageWidth;	
}

void	CtmParsePDF::setPageHeight(int height)
{
	m_data->m_pageHeight=height;
}

void	CtmParsePDF::setPageWidth(int width)
{
	m_data->m_pageWidth=width;
}
