/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

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
#include	"static.h"
#include	"font.h"
#include 	"utils.h"
//#include	"memwatch.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

#define				tmBDWIDTH_STATIC			1

/*===========================================================================+
|           Class implementation - StaticBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmStaticX2, CtmWnd)
CtmStatic::CtmStatic(tmWND* pwnd, tmSTATIC* pstatic) : CtmWnd()
{
	m_paragraph.wAlign=33;
}

CtmStatic::~CtmStatic()
{
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CtmStatic::Update()           /* Virtual function */
{
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
	{
		Clear();
		Show();
	}
}

void 		CtmStatic:: Clear()
{
	CtmWnd:: Clear();
	/*if(m_wnd.wStyle & tmWS_ONBMP)
	{		
		fprintf(stderr," bmp");
		HDC hdc = GetDC(m_hwndGUI);	
		FillBoxWithBitmap(hdc,m_rectClient.left,m_rectClient.top,0,0,&m_bmp);
		ReleaseDC(hdc);	
	}*/
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

CtmStaticX1::CtmStaticX1(tmWND* pwnd, tmSTATIC* pstatic) : CtmWnd()
{
	
	
	
}

CtmStaticX1::~CtmStaticX1()
{
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CtmStaticX1::Update()         /* Virtual function */
{
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
	{
	  	Clear();
		Show();
	}
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*==========================================================================+
|           Class implementation - StaticBox                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/


//	fans add
CtmStaticX2::CtmStaticX2() : CtmWnd()
{
	strcpy(TYPEName, "CtmStaticX2");
	
	m_bOnlineHelp	= FALSE;
	
	m_static.psz 	= NULL;
	m_pszTextID 	= NULL;
	m_psziNetStream = NULL;
	m_wiNetLen		= 0;
	m_wiNetType		= 0;
	m_nOffSet		= 0;
	m_fgc 			=0xffff;
	//m_pstatic = &m_static;	
	m_bgcOld 		= 0;
	m_bBackGround 	= FALSE;
   	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	for (int i = 0; i < MAX_STRING_NUM_STATIC; i++)
	{
		m_pszTextIDEX[i] = NULL;
	}

}

CtmStaticX2::~CtmStaticX2()
{
	m_BackImage.FreeImage();
}
    
void        CtmStaticX2::DrawBorder()     
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
        _SetColor(0xFFFFFFFF);
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
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CtmStaticX2::Show()               /* Virtual function */
{     
	//printf("m_iFontUNType=%d \n", m_iFontUNType);
	if(m_rectWindow.left==m_rectWindow.right&&m_rectWindow.bottom==m_rectWindow.top)
		return;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	if (!m_bVisible)
	{
		if (m_wnd.wStyle & tmWS_SHOW) m_wnd.wStyle -= tmWS_SHOW;
	}
	else 
	{
		if (!(m_wnd.wStyle & tmWS_SHOW)) m_wnd.wStyle += tmWS_SHOW;
	}
	
	//Clear();
	if (m_wnd.wStyle & tmWS_SHOW)            // Show the content if requested
	{
		DrawBorder();
		if (m_pszTextID != NULL)
		{	 
				StrIDToStr(m_pszTextID, &m_static.psz, m_bOnlineHelp);
		}
        Clear();
        
        if(m_iFontUNType >= 1)
        {
		 	if(m_static.psz != NULL)
			{
				CtmWnd::ShowExtent_UN(m_nOffSet, m_static.psz,3);
					
			}
        }
        else
        {
			if (m_static.psz != NULL)//	fans add
			{  
				//	fans modify 2009-3-24 18:37:24
				//CtmWnd::ShowExtent(0, m_static.psz);
				CtmWnd::ShowExtent(m_nOffSet, m_static.psz);
			}
		}
		
		if(m_psziNetStream != NULL)
			CtmWnd::ShowFontStream(1, 0, m_psziNetStream, m_wiNetLen, m_wiNetType);
		
	}
	else
	{     
		DrawBorder();
		if (m_bgcOld == 0)
			m_bgcOld = m_bgc;
		
 		if (GetParent() != NULL) 
 			m_bgc = GetParent()->GetBGColor();
 		 
 		CtmWnd::SetClientRECT(0, 0, 0, 0);       // Restore the area for vertical cursor
 		Clear();
 		CtmWnd::SetClientRECT(0, 0, 0, 0);       // Make room for vertical cursor	
 		m_bgc = m_bgcOld;
 	}
 	//	fans add     
    CtmWnd::Show();	
	ChangePosNow(GetClientRECT()); 
	//ChangePosRegionSum(GetClientRECT());
}


void        CtmStaticX2::Update()           /* Virtual function */
{
	//Clear();
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
		Show();      
}

void        CtmStaticX2::Update2()           /* Virtual function */
{
	m_bBackGround = FALSE;
	Clear();
	BOOL	bResult=IsInterSection(this->m_rectWindow.left, this->m_rectWindow.right, 
		this->m_rectWindow.top, this->m_rectWindow.bottom);
	if(!bResult)
		Show();      
}

//	fans add
BOOL 	CtmStaticX2::CreateA()				/* virtual function */
{
	//James add  for freetype 2015/5/4 14:26:24
	WORD	wHeight = 0;
	
	SetBorderWidth(tmBDWIDTH_STATIC);
	CtmWnd::CreateA();
	
	if(m_iFontUNType >= 1)	
	{
		wHeight = m_iFontUNHeight ;//+ 2;
	}
	else
	{
		wHeight = m_pcFont->m_wHeight;	
	}
	//printf("wHeight=%d \n", wHeight);
	//	fans add 2009-3-24 13:58:41
	if (m_paragraph.wAlign & tmPARA_ALIGN_CENTERY)
	{		
		m_nOffSet = (m_wnd.rect.bottom - m_wnd.rect.top - wHeight) / 2;
	}
	else if (m_paragraph.wAlign & tmPARA_ALIGN_TOP)
	{
		m_nOffSet = 0;
	}
	else if (m_paragraph.wAlign & tmPARA_ALIGN_BOTTOM)
	{
		m_nOffSet = m_wnd.rect.bottom - m_wnd.rect.top - wHeight - m_wBorderWidth;
	}
	else 
		;
	if (m_nOffSet < 0) m_nOffSet = 0;
		
	if (m_bOnlineHelp)
		m_pcFont = g_pFontHelp->GetFont(m_iFontType);
}

//	fans add
void 	CtmStaticX2::SetPropertys()			/* virtual function */
{
	CtmWnd::SetPropertys();	
	SetPropInfo("align",	tmWORD, &m_paragraph.wAlign);
	SetPropInfo("text", 	tmUSTR, 	&m_static.psz);
	SetPropInfo("textID",	tmSTR,	&m_pszTextID);
	SetPropInfo("stream", 	tmFontStream,	&m_psziNetStream);		//rw.ying
	SetPropInfo("iNetFontLen", 		tmWORD,	&m_wiNetLen);			//Caozz add 2012/6/11 ¤U¤È 01:29:16
	SetPropInfo("iNetFontType", 	tmWORD,	&m_wiNetType);			//Caozz add 2012/6/11 ¤U¤È 01:29:16
	SetPropInfo("onlinehelp", tmBOOL,	&m_bOnlineHelp);	//Gerry add for online help
	
	char	sz[10];
	for (int i = 0; i < MAX_STRING_NUM_STATIC; i++)
	{
		sprintf(sz, "%s%d", "textID", i + 1);
		SetPropInfo(sz,	tmSTR,	&m_pszTextIDEX[i]);
	}
}

void 	CtmStaticX2::SetPropertys_Nub()			/* virtual function */
{
	CtmWnd::SetPropertys_Nub();	
	SetPropInfo(STATICX2_ALIGN,	tmWORD, &m_paragraph.wAlign);
	SetPropInfo(STATICX2_TEXT, 	tmUSTR, 	&m_static.psz);
	SetPropInfo(STATICX2_TEXTID,	tmSTR,	&m_pszTextID);
	
	for (int i = 0; i < MAX_STRING_NUM_STATIC; i++)
	{
		SetPropInfo(STATICX2_TEXTID+i+1,	tmSTR,	&m_pszTextIDEX[i]);
	}
}

void    CtmStaticX2::Clear()
{
	
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	
	if (m_wnd.wStyle & tmWS_TRANSPARENT) 
    {
        if (!m_bBackGround) 
        {
            m_BackImage.CopyImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
            m_bBackGround = TRUE; 
        }
        else
        {
            m_BackImage.ShowImage(m_rectClient.left, m_rectClient.top, m_rectClient.right,
                              m_rectClient.bottom);
        }
    }
    else
    {
        CtmWnd::Clear();
    }
}

//bool 	CtmStaticX2::SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType)//rw.ying 2011/3/17 02:52¤U¤È
//{
//	bool	bResult = FALSE;
//	char	pszName[48] = "stream";
//	if(nLen <= 0)
//		return bResult;
//	
//	if(strcmp(pszName, pszPropName) == 0)
//		bResult = CtmWnd::SetPropValueT(pszPropName, pszValue, nLen, nType);
//	if(bResult)
//	{
//		m_wiNetLen 	= nLen;
//		m_wiNetType 	= nType;
//	}
//	return bResult;
//}
