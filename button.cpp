/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager file                                 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision : 09/19/2003   Jeckey                                            |
+===========================================================================*/
#include	<string.h>

#include 	"button.h"
#include    "stable.h"
#include	"init.h"
#include 	"commonaction.h"




#include	"utils.h"
#include	"tmconfig.h"	//	2009-2-11 16:10:59 fans add
/*===========================================================================+
|           Ecternal                                                         |
+===========================================================================*/

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
/*===========================================================================+
|           Class implementation - Button                                    |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmButton, CtmWnd)
IMPLEMENT_DYNCREATE(CtmToolButton, CtmButton)
CtmButton::CtmButton() : CtmWnd()
{
	//	fans add
	//m_button.pContent 	= NULL;
	m_pszCaptionID 		= NULL;
	m_pszUpPath 		= NULL;
	m_pszDownPath 		= NULL;
	m_button.pbmpDown 	= NULL;
	m_button.pbmpUp 	= NULL;
	m_button.wType 		= tmMAX_BUTTON_TEXT;
	m_pbutton 			= &m_button;
	m_fgc 				= 0x0;	//Sunny<20080129> add
	m_nSpace			= 1;
	m_dwKeyColor		= 0x000000;
	m_wCaptionType		= 0;	
	m_bBmpStyle			=FALSE;	
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
		m_iFontUNType		=1;
	else
		m_iFontUNType		=0;
	for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	{
		m_button.pContent[i] = NULL;
		m_nX[i] = m_nY[i] = 0;
	}
	memset(m_pszCaptionIDs, 0, sizeof(m_pszCaptionIDs));
}

CtmButton::CtmButton(tmWND* pwnd, tmBUTTON* pbutton) : CtmWnd()
{
	//	fans add
	//m_button.pContent	= NULL;
	m_pszCaptionID 		= NULL;
	m_pszUpPath 		= NULL;
	m_pszDownPath 		= NULL;	
	m_button.pbmpDown 	= NULL;
	m_button.pbmpUp 	= NULL;
	for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	{
		m_button.pContent[i] = NULL;
		m_nX[i] = m_nY[i] = 0;
	}
	
	m_button.wType 		= tmBUTTON_BMP;
	m_pbutton 			= &m_button;
	memset(m_pszCaptionIDs, 0, sizeof(m_pszCaptionIDs));
	Create(pwnd, pbutton);
}

CtmButton::~CtmButton()
{
	//	fans add 2007/2/26 04:02下午
	if (m_pszUpPath != NULL)
	{
		if (m_button.pbmpUp != NULL) delete m_button.pbmpUp;
	}
	if (m_pszDownPath != NULL)
	{
		if (m_button.pbmpDown != NULL) delete m_button.pbmpDown;
	}
	for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	{
		if(m_button.pContent[i] != NULL) 
		{
			delete[] m_button.pContent[i];
			m_button.pContent[i] = NULL;
		}
			
	}
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
//	fans add
BOOL	CtmButton::CreateA()
{
	//tmPARAGRAPH   paragraph = {tmPARA_ALIGN_CENTER};
	SetBorderWidth(m_wndattr.wBorderWidthButton);
	//SetParagraph(paragraph);
	
	CtmWnd::CreateA();
	
	if (m_pszUpPath != NULL)
	{
		if (m_button.pbmpUp != NULL) delete m_button.pbmpUp;
		m_button.pbmpUp = new CtmBitMap();
		m_button.pbmpUp->SetKeyColor(m_dwKeyColor);
		m_button.pbmpUp->Create(m_pszUpPath);
	}
	if (m_pszDownPath != NULL)
	{
		if (m_button.pbmpDown != NULL) delete m_button.pbmpDown;
		m_button.pbmpDown = new CtmBitMap();
		m_button.pbmpDown->SetKeyColor(m_dwKeyColor);
		m_button.pbmpDown->Create(m_pszDownPath);
	}
	
	
	//StrIDToStr(m_pszCaptionID, &m_button.pContent);
	
	//	fans 2008/7/17 08:51下午	
	int		index 		= 0, nLen = 0;
	BOOL	bHaveID		= FALSE;
	char*	pszTemp		= NULL;
	char	szText[256];
	memset(szText, 0, sizeof(szText));
	
	
	if (m_pszCaptionID != NULL)
	{
		if(m_iFontUNType > 0)
		{
			index = nLen = strlen(m_pszCaptionID);
			if(m_button.pContent[0]!= NULL)
				delete m_button.pContent[0];
			m_button.pContent[0] = new char[nLen + 1];
			memset(m_button.pContent[0], 0, nLen+1);
			memcpy(m_button.pContent[0], m_pszCaptionID, nLen);
			strcpy(szText, m_pszCaptionID);
		}
		else
		{
			pszTemp	= g_MultiLanguage[m_pszCaptionID];
				
			if (pszTemp != NULL)
			{
				int nLen = StrLenTM(pszTemp);				
				memcpy(&szText[index], pszTemp, nLen - 2);
				index	= index + nLen - 2;				
			}
			//bHaveID = TRUE;
			if(m_button.pContent[0]!= NULL)
				delete m_button.pContent[0];
			m_button.pContent[0] = new char[index + 2];
			memcpy(m_button.pContent[0], szText, index + 2);
		}
	}
	
	
	for (int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	{
		if(m_iFontUNType > 0)
		{
			if (m_pszCaptionIDs[i] != NULL)
			{
				//index = nLen = strlen(m_pszCaptionIDs[i]);
				//strcpy(szText, m_pszCaptionIDs[i]);
				//if(m_wCaptionType & 0x01)
				{
					if(m_button.pContent[i]!= NULL)
						delete m_button.pContent[i];
					nLen = strlen(m_pszCaptionIDs[i]);
					m_button.pContent[i] = new char[nLen + 1];
					memset(m_button.pContent[i], 0, nLen+1);
					memcpy(m_button.pContent[i], m_pszCaptionIDs[i], nLen);
					//printf("i=%d text=%s \n", i, m_button.pContent[i]);
				}
			}
		}
		else
		{
			if (m_pszCaptionIDs[i] != NULL)
			{
				pszTemp 	= g_MultiLanguage[m_pszCaptionIDs[i]];
				//bHaveID 	= TRUE;
				nLen = 0;
				if (pszTemp != NULL)
				{
					nLen = StrLenTM(pszTemp);				
					
					if (index + nLen <= 256)
					{
						if(m_wCaptionType & 0x01)			//James add 2010/10/25 下午 03:04:41
							memset(szText, 0, sizeof(szText));
						memcpy(&szText[index], pszTemp, nLen - 2);
						index	= index + nLen - 2;				
					}
				}
				//James modify 2010/10/19 下午 08:05:04
				if(m_wCaptionType & 0x01)
				{
					//printf("2 i=%d text=%s \n", i, m_pszCaptionIDs[i]);
					if(m_button.pContent[i]!= NULL) delete m_button.pContent[i];
					m_button.pContent[i] = new char[nLen];//new char[index + 2];
					memset(m_button.pContent[i], 0, nLen);
					memcpy(m_button.pContent[i], szText, nLen/*index + 2*/);
					index = 0;
				}
			}
		}
	}
	
	if(m_wCaptionType & 0x01)
	{
		;	
	}
	else
	{
		if(m_iFontUNType > 0)
		{
			;
		}
		else
		{
			if(m_button.pContent[0]!= NULL) delete m_button.pContent[0];
			m_button.pContent[0] = new char[index + 2];	
			memcpy(m_button.pContent[0], szText, index + 2);
		}
	}
	//if (bHaveID)
	//{
	//	m_button.pContent[i] = new char[index + 2];
	//	
	//	memcpy(m_button.pContent[i], szText, index + 2);
	//}
	
	m_bUp = TRUE;
	m_bakFgc = GetFGColor();
	m_bakBgc = GetBGColor();
	m_wStyle  = GetStyle();      //St122900
	m_bActive = TRUE;            //St122900
	Set();
	return TRUE;
}

//	fans add
void 	CtmButton::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo("buttontype", 	tmWORD, 	&m_button.wType);
	SetPropInfo("align",		tmWORD, 	&m_paragraph.wAlign);
	SetPropInfo("space",		tmINT,		&m_nSpace);
	SetPropInfo("caption", 		tmUSTR,		&m_button.pContent[0]);
	SetPropInfo("captionID", 	tmSTR,		&m_pszCaptionID);
	SetPropInfo("upbitmap",		tmSTR,		&m_pszUpPath);
	SetPropInfo("downbitmap",	tmSTR,		&m_pszDownPath);
	SetPropInfo("bmpstyle",		tmBOOL,		&m_bBmpStyle);
	
	char	sz[20];
	for (int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	{
		sprintf(sz, "captionID%d", i);
		SetPropInfo(sz, 	tmSTR,		&m_pszCaptionIDs[i]);
	}
	SetPropInfo("keycolor", 	tmLONG,		&m_dwKeyColor);
	SetPropInfo("captiontype", 	tmWORD, 	&m_wCaptionType);
}

void 	CtmButton::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	SetPropInfo(BUTTON_BUTTONTYPE, 	tmWORD, 	&m_button.wType);
	SetPropInfo(BUTTON_ALIGN,		tmWORD, 	&m_paragraph.wAlign);
	SetPropInfo(BUTTON_SPACE,		tmINT,		&m_nSpace);
	SetPropInfo(BUTTON_CAPTION, 	tmUSTR,		&m_button.pContent[0]);
	SetPropInfo(BUTTON_CAPTIONID, 	tmSTR,		&m_pszCaptionID);
	SetPropInfo(BUTTON_UPBITMAP,	tmSTR,		&m_pszUpPath);
	SetPropInfo(BUTTON_DOWNBITMAP,	tmSTR,		&m_pszDownPath);
	SetPropInfo("bmpstyle",			tmBOOL,		&m_bBmpStyle);
	for (int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	{
		SetPropInfo(BUTTON_CAPTIONID0 + i, 	tmSTR,		&m_pszCaptionIDs[i]);
	}
	SetPropInfo(BUTTON_KEYCOLOR, 		tmLONG,		&m_dwKeyColor);
	SetPropInfo(BUTTON_CAPTIONTYPE, 		tmWORD,		&m_wCaptionType);
}

BOOL        CtmButton::Create(tmWND* pwnd, tmBUTTON* pbutton)
{
	tmPARAGRAPH   paragraph = {tmPARA_ALIGN_CENTER};

	SetBorderWidth(m_wndattr.wBorderWidthButton);
	SetParagraph(paragraph);

	CtmWnd::Create(pwnd);


	//m_pbutton = pbutton;	fans mark
	//	fans add
	memcpy(m_pbutton, pbutton, sizeof(tmBUTTON));
	m_bUp = TRUE;
//	m_fgc = GetFGColor();
	m_bgc = GetBGColor();
	m_wStyle  = GetStyle();      //St122900
	m_bActive = TRUE;            //St122900
	Set();
	if (m_wnd.wStyle & tmWS_SHOW)             // Show the content if requested
	   Show();
	return TRUE;
}

void        CtmButton::Set()              /* Virtual function */
{
    tmSIZE        size, size_temp;
	short       nWidth  = GetClientWidth();
	short       nHeight = GetClientHeight();
	WORD		wFontWidth = 0, wFontHeight = 0;
	memset(&size, 0, sizeof(size));
	if(m_wCaptionType & 0x01)
	{
		//printf("m_wCaptionType=%d \n", m_wCaptionType);
		//先計算總高度和最大長度
		for(int i = 0; i <tmBUTTON_CAPTION_NUM; i++)
		{
			if (m_pbutton->pContent[i] != NULL)
			{
				if(m_iFontUNType > 0)
				{
					size_temp = GetUNStringExtent((char*)m_pbutton->pContent[i]);
				}
				else
				{
					size_temp = GetStringExtent((char*)m_pbutton->pContent[i]);
				}
				if(size.w < size_temp.w)
					size.w = size_temp.w;
				size.h += size_temp.h;
				
				//printf("width=%d height=%d \n", size.w, size.h);
			}
			else
			{
				if(m_iFontUNType > 0)
					;
				else
					break;
			}
		}
	}
	else
	{
		if(m_iFontUNType > 0)
		{
			for(int i = 0; i <tmBUTTON_CAPTION_NUM; i++)
			{
				if (m_pbutton->pContent[i] != NULL)
				{
					size_temp = GetUNStringExtent((char*)m_pbutton->pContent[i]);
					size.w += size_temp.w;
					if(size.h < size_temp.h)
						size.h = size_temp.h;
				}
			}
		}
		else
		{
			if (m_pbutton->pContent[0] != NULL)
			{
				size = GetStringExtent((char*)m_pbutton->pContent[0]);	
			}	
		}
	}
	
	if(m_iFontUNType > 0)
	{
		wFontHeight = m_iFontUNHeight;
		wFontWidth 	= m_iFontUNWidth;
	}
	else
	{
		wFontHeight = m_pcFont->m_wHeight;
		wFontWidth 	= m_pcFont->m_wWidth;
	}
	
	if(m_wCaptionType & 0x01)
	{
		WORD	wStartY = 0;
		for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
		{
			//memset(&size, 0, sizeof(size));
			if (m_pbutton->pContent[i] != NULL)
			{
				if(m_iFontUNType > 0)
				{
					size_temp = GetUNStringExtent((char*)m_pbutton->pContent[i]);
				}
				else
				{
					size_temp = GetStringExtent((char*)m_pbutton->pContent[i]);
				}
				//printf("i=%d size.h=%d size_temp.h=%d nHeight=%d align=%d \n", i, size.h, size_temp.h, nHeight, m_paragraph.wAlign & 0xFFFF);
	
				switch (m_paragraph.wAlign & 0xFFFF)
				{
					case tmBUTTON_CAPTION_TOP:
						m_nY[i] = m_nSpace;
					break;
					case tmBUTTON_CAPTION_BOTTOM:
						m_nY[i] = nHeight - size.h - m_nSpace;
					break;
					default:
						m_nY[i] = ((nHeight - size.h) >> 1) + 1;
					break;
				}
				switch (m_pbutton->wType & 0x000F)
				{
				    case tmBUTTON_BMP:
				    default:
				    	if (m_pbutton->pContent[i] != NULL)
				    	{
				        	while (size_temp.h > nHeight) size_temp.h -= wFontHeight;
				        	m_nX[i] = ((nWidth - size_temp.w) >> 1) + 2;
				        }
				        break;
			    }
			    wStartY++;
		    }
		    else
			{
				if(m_iFontUNType > 0)
					;
				else
					break;	
			}
		}
	}
	else
	{
			if (m_paragraph.wAlign & tmBUTTON_CAPTION_TOP)
				m_nY[0] = m_nSpace;
			else if (m_paragraph.wAlign & tmBUTTON_CAPTION_BOTTOM)
			{
				if(m_iFontUNType > 0)
				{
					for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
					{
						if (m_pbutton->pContent[i] != NULL)
							m_nY[i] = nHeight - size.h - m_nSpace;
					}
				}
				else
				m_nY[0] = nHeight - size.h - m_nSpace;
			}
			else
			{
				if(m_iFontUNType > 0)
				{
					for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
					{
						if (m_pbutton->pContent[i] != NULL)
							m_nY[i] = ((nHeight - size.h) >> 1) + 1;
					}
				}
				else
					m_nY[0] = ((nHeight - size.h) >> 1) + 1;
			}
		
		switch (m_pbutton->wType & 0x000F)
		{
		    case tmBUTTON_BMP:
		    default:
		    	{
		    		if(m_iFontUNType > 0)
		    		{
		    			WORD	wStartx = 0, wxPos = 0;
							if(m_paragraph.wAlign==1) m_paragraph.wAlign=0X02;
		    			//m_paragraph.wAlign&=0xF0;
		    			//m_paragraph.wAlign|=0x02; // for test 2015/8/4 11:09:56
		    			if((m_paragraph.wAlign&0x01)==0x01) ;// left
		    				
		    			else if((m_paragraph.wAlign&0x02)==0x02) //center
		    			{
		    				int nTemp=nWidth - size.w;
		    				if((nWidth - size.w)<0)
		    				{
		    					 wStartx=0;	
		    				}
		    				else
		    				{
		    					wStartx = (nTemp >> 1) + 2;
		    				}
		    			}
		    			else if((m_paragraph.wAlign&0x04)==0x04) //right
		    			{
		    				wStartx = (nWidth - size.w);
		    				if(wStartx<0)	wStartx=0;
		    			}
		    			//printf("wStartx=%d size.w=%d nWidth=%d \n ", wStartx, size.w, nWidth);
		    			for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
		    			{
			    			if (m_pbutton->pContent[i] != NULL)
					    	{
					    		size = GetUNStringExtent((char*)m_pbutton->pContent[i]);
					        	m_nX[i] = wStartx + wxPos;
					        	wxPos += size.w;
					        	//printf("nWidth=%d w=%d h=%d x=%d wxPos=%d wStartx=%d \n", 
					        	//nWidth, size.w, size.h, m_nX[i], wxPos, wStartx);
					        }
					    }
		    		}
		    		else
		    		{
		    			if (m_pbutton->pContent != NULL)
				    	{
				        	while (size.h > nHeight)
				        		size.h -= wFontHeight;
				        	m_nX[0] = ((nWidth - size.w) >> 1) + 2;
				        }		
		    		}	
		    	}
		        break;
	    }	
	}
}

void        CtmButton::Show()             /* Virtual function */
{
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;			//James add for test 2015/5/17 21:20:43
	else
		m_iFontUNType		= 0;
	//	fans add 2010-4-29 9:53:29
	if (!m_bVisible)	m_wnd.wStyle &= (~tmWS_SHOW);	
	else 				m_wnd.wStyle != tmWS_SHOW;
	
	
	if (m_wnd.wStyle & tmWS_SHOW)	//	fans add
	{		
		if (!m_bActive)
    	{
			Disable();
			return;
    	}
		if (m_bUp) Up();
		else       Down();
		//	2009-2-11 16:10:18 fans modify
		if (GetParent()->Is("CtmToolBar"))
		{
			int	nMainFrameType = CtmConfig::GetInstance()->GetMainFrameType();
			
			if (nMainFrameType & 0x0001||m_bBmpStyle)	//	don't show the static component and don't show the text
			{
				//printf("nMainFrameType == 1 GetParent() = %s \n",GetParent()->Name);
				for (int i = 0; i < GetControlCount(); i++)
				{
					if (strcmp(Controls[i]->TYPEName, "CtmStaticX2") == 0)
						continue;
					Controls[i]->Show();
				}				
			}
			
			if (nMainFrameType & 0x0002)	//	don't show the ImageBox
			{
				//printf("nMainFrameType == 2 GetParent() = %s \n",GetParent()->Name);
				for (int i = 0; i < GetControlCount(); i++)
				{
					if (Controls[i]->Is("CtmImageBox"))
						continue;
					Controls[i]->Show();
				}				
			}	
			
			if (nMainFrameType == 0)
			{
				//printf("nMainFrameType == 0 GetParent() = %s \n",GetParent()->Name);
				for(int i = 0; i < GetControlCount(); i++)
				{
					Controls[i]->Show();			
				}
			}
		}
		else
		{
			for(int i = 0; i < GetControlCount(); i++)
			{
				Controls[i]->Show();			
			}
		}
	}
	else
	{
		DrawBorder();
		BGC bgcOld = 0;
		bgcOld = m_bgc;
		
 		if (GetParent() != NULL) 
 			m_bgc = GetParent()->GetBGColor();
 		
 		CtmWnd::SetClientRECT(0, 0, 0, 0);       // Restore the area for vertical cursor
 		Clear();
 		CtmWnd::SetClientRECT(0, 0, 0, 0);       // Make room for vertical cursor	
 		m_bgc = bgcOld;
	}
	
	ChangePosNow(GetClientRECT());
}

void        CtmButton::Update()           /* Virtual function */
{
	Set();
	Show();
}

void        CtmButton::SetFGColor(FGC fgc)
{
//	m_fgc = fgc;

	if (m_pbutton->wType & tmBUTTON_INVERSE)
    {
		if (m_bUp) CtmWnd::SetFGColor(m_fgc);
		else       CtmWnd::SetFGColor(m_bgc);
    }
	else
		CtmWnd::SetFGColor(m_fgc);
}

void        CtmButton::SetBGColor(BGC bgc)
{
	m_bgc = bgc;

	if (m_pbutton->wType & tmBUTTON_INVERSE)
    {
		if (m_bUp) CtmWnd::SetBGColor(m_bgc);
		else       CtmWnd::SetBGColor(m_fgc);
    }
	else
		CtmWnd::SetBGColor(m_bgc);
}

void        CtmButton::Press(WORD wFlag)
{
	char        szWndName[256];
	memset(szWndName,0,sizeof(szWndName));
	if (m_pbutton->wType & tmBUTTON_INVERSE)
    {
		switch (wFlag)
        {
			case tmBUTTON_DOWN:
				CtmWnd::SetFGColor(m_bakBgc);
				CtmWnd::SetBGColor(m_bakFgc);
				break;
			default:
				CtmWnd::SetFGColor(m_bakFgc);
				CtmWnd::SetBGColor(m_bakBgc);
				break;
        }
    }

	
	switch (wFlag)
    {
		case tmBUTTON_UP:
			
			if(m_wAction == 0xFE && m_pszWndName != NULL && m_pszWndName[0] != '\0')
			{
				strcpy(szWndName, m_pszWndName);
				/*if(m_iPrivilege <= g_iPrivilege)
				{
					PutCommand(m_pszWndName);
				}
				else
				{
					Login();
					if(g_WndLoginFlag) */PutCommand(m_pszWndName);
				//}
				
				
			}
			
			Up();
			break;
		case tmBUTTON_DOWN:
			
			Down();
			break;
		case tmBUTTON_DOWNUP:
			
			Down();
			Up();
			break;
    }
    
    if (GetParent()->Is("CtmToolBar"))
	{
		int	nMainFrameType = CtmConfig::GetInstance()->GetMainFrameType();
		
		if (nMainFrameType & 0x0001||m_bBmpStyle)	//	don't show the static component and don't show the text
		{
			for (int i = 0; i < GetControlCount(); i++)
			{
				if (strcmp(Controls[i]->TYPEName, "CtmStaticX2") == 0)
					continue;
				Controls[i]->Show();
			}				
		}
		
		if (nMainFrameType & 0x0002)	//	don't show the ImageBox
		{
			for (int i = 0; i < GetControlCount(); i++)
			{
				if (Controls[i]->Is("CtmImageBox"))
					continue;
				Controls[i]->Show();
			}				
		}	
		
		if (nMainFrameType == 0)
		{
			for(int i = 0; i < GetControlCount(); i++)
			{
				Controls[i]->Show();			
			}
		}
	}
	else
	{
	    for(int i = 0; i < GetControlCount(); i++)
		{
			Controls[i]->Show();			
		}
	}
	ChangePosNow(GetClientRECT());
	//add for button update by J.Wong 2016/7/14 17:19:56
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : Up()                                                           |
|  Task     : Show button up or down                                         |
+----------------------------------------------------------------------------+
|  Call     : CtmButton::Up()                                                  |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmButton::Up()
{
	short       nRight  = m_rectWindow.right  - m_rectWindow.left;
	short       nBottom = m_rectWindow.bottom - m_rectWindow.top;
	RECT 		rect;
	
	Clear();
	//printf("this is %s->%s , L=%d T=%d R=%d B=%d\n",this->TYPEName,this->Name,m_rectWindow.left,m_rectWindow.top,m_rectWindow.right,m_rectWindow.bottom);
	if((m_pbutton->wType & 0x000F) == tmBUTTON_BMP)		
	{
		if(m_pbutton->pbmpUp != NULL)
		{
			MKRECT(rect, 0, 0, GetClientWidth(), GetClientHeight())
			_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
			m_pbutton->pbmpUp->Show(rect, tmBMP_KEYCOLOR);
		}
	}
	else
	{	
		
		_SetViewPort(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
		_SetColor(m_bgc);
		_Rectangle(_tmGBORDER, 0, 0, nRight, nBottom);
		_SetColor(m_wndattr.fgcBorder3);
		_MoveTo(       1, nBottom-1);
		_LineTo(       1, 1);
		_LineTo(nRight-1, 1);
		_SetColor(m_wndattr.fgcBorder4);
		_LineTo(nRight-1, nBottom-1);
		_LineTo(       1, nBottom-1);
	}
	ShowContent();
	m_bUp = TRUE;
}

void	CtmButton::Down()
{
	short       nRight  = m_rectWindow.right  - m_rectWindow.left;
	short       nBottom = m_rectWindow.bottom - m_rectWindow.top;
	RECT 		rect;
	Clear();
	
	if((m_pbutton->wType & 0x000F) == tmBUTTON_BMP)		
	{
		if(m_pbutton->pbmpDown != NULL)
		{
			MKRECT(rect, 0, 0, GetClientWidth(), GetClientHeight())
			_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right,m_rectClient.bottom);
			m_pbutton->pbmpDown->Show(rect, tmBMP_KEYCOLOR);
		}
		ShowContent();
	}
	else
	{	
		
		_SetViewPort(m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom);
		_SetColor(m_bgc);
		_Rectangle(_GBORDER, 0, 0, nRight, nBottom);
		_SetColor(CtmWnd::m_bgc);
		_MoveTo(       1, nBottom-1);
		_LineTo(nRight-1, nBottom-1);
		_LineTo(nRight-1, 1);
		_SetColor(m_wndattr.fgcBorder4);
		_LineTo(       1, 1);
		_LineTo(       1, nBottom-1);
		CtmWnd::SetClientRECT(1, 1, 1, 1);        // Shift the client area 1 pixel
		ShowContent();
		CtmWnd::SetClientRECT(-1, -1, -1, -1);    // Restore it
	}
	m_bUp = FALSE;
}

/*---------------------------------------------------------------------------+
|  Function : Disable()                                                      |
|  Task     : Disable button                                                 |
+----------------------------------------------------------------------------+
|  Call     : CtmButton::Disable()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CtmButton::Disable()                  //St122900
{
	WORD        wStyle  = m_wStyle;

	//Clear();
	wStyle &= ~tmWS_BORDER;
	wStyle |=tmWS_BORDER_INSET;
	SetStyle(wStyle);
	DrawBorder();
	ShowContent();
	m_bActive = FALSE;
}

void        CtmButton::Enable()                    //St122900  
{
	m_bActive = TRUE;
	SetStyle(m_wStyle);
	Show();
}

void	CtmButton::ShowContent()
{
	//	2009-2-11 16:10:18 fans modify
	if (strcmp(GetParent()->TYPEName, "CtmToolBar") == 0)
	{
		int	nMainFrameType = CtmConfig::GetInstance()->GetMainFrameType();
		if (nMainFrameType & 0x0001||((nMainFrameType & 0x0002)&&m_bBmpStyle)) return;
	}
	
	if(m_iFontUNType > 0)	
	{
		;//printf("ShowContent 1 type=%d type=%d \n", m_pbutton->wType, m_wCaptionType);
	}
	
	if(m_wCaptionType & 0x01)
	{
		switch (m_pbutton->wType & 0x000F)
	    {	    
	    	case tmBUTTON_BMP:
	    		for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	    		{
					//printf("name i=%d text=%s x=%d y=%d \n", i, m_button.pContent[i], m_nX[i], m_nY[i]);
		    		if (m_button.pContent[i] != NULL)
		    		{
						ShowExtent(m_nX[i], m_nY[i], (char*)m_pbutton->pContent[i]);			//<<yang 2005/10/21 
						if(!strcmp(m_button.pContent[i],"")) break;
					}
					else
					{
						if(m_iFontUNType > 0)
							;
						else
							break;
					}
				}
				break;
			default:
				for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
	    		{
					if (m_button.pContent[i] != NULL)
					{
						ShowExtent(m_nX[i], m_nY[i], (char*)m_pbutton->pContent[i]);
						if(!strcmp(m_button.pContent[i],"")) break;
					}
				}
				break;
	    }
	}
	else
	{
		//printf("name=%s m_pbutton->pContent[0]=%s type=%x \n", this->Name, m_pbutton->pContent[0], m_pbutton->wType);
		
		if(m_iFontUNType > 0)	
		{
			switch (m_pbutton->wType & 0x000F)
			{	    
				case tmBUTTON_BMP:
					for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
					{
						//printf("i=%d text=%s x=%d y=%d \n", i, m_button.pContent[i], m_nX[0], m_nY[0]);
						if (m_button.pContent[i] != NULL)
						{
							ShowExtent(m_nX[i], m_nY[i], (char*)m_pbutton->pContent[i]);			//<<yang 2005/10/21 
							if(!strcmp(m_button.pContent[i],"")) break;
						}
						//else
						//break;
					}
					break;
				default:
					for(int i = 0; i < tmBUTTON_CAPTION_NUM; i++)
					{
						if (m_button.pContent[i] != NULL)
						{
							ShowExtent(m_nX[0], m_nY[0], (char*)m_pbutton->pContent[i]);
							if(!strcmp(m_button.pContent[i],"")) break;
						}
					}
					break;
			}

		}
		else
		{
			switch (m_pbutton->wType & 0x000F)
			{	    
				case tmBUTTON_BMP:
					if (m_button.pContent != NULL)
					ShowExtent(m_nX[0], m_nY[0], (char*)m_pbutton->pContent[0]);			//<<yang 2005/10/21 
				default:
					if (m_button.pContent != NULL)
					ShowExtent(m_nX[0], m_nY[0], (char*)m_pbutton->pContent[0]);
					break;
			}
		}
	}
}


void    CtmButton::Clear()
{
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


/*===========================================================================+
|           Class implementation - ButtonOK                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmButtonOK::CtmButtonOK(tmBUTTON_DEF* pbutton) : CtmButton()
{
	
}

CtmButtonOK::~CtmButtonOK()
{
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*===========================================================================+
|           Class implementation - ButtonCancel                              |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmButtonCancel::CtmButtonCancel(tmBUTTON_DEF* pbutton) : CtmButton()
{

}

CtmButtonCancel::~CtmButtonCancel()
{
}



/*===========================================================================+
|           Class implementation - CtmToolButton                             |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CtmToolButton::CtmToolButton():CtmButton()
{
	strcpy(TYPEName, "CtmToolButton");
	m_button.wType = tmBUTTON_BMP;
	m_wKey = _NULL_KEY;
	m_wAction = 0;		
	m_wGroup = 0;		
	m_pszWndName = NULL;
	m_pszToolBarName = NULL;
	m_pszSubToolBarName = NULL;
	m_wnd.wStyle = 0x20;
	
	m_bOnFocus = FALSE;
}

CtmToolButton::~CtmToolButton()
{
}

void	CtmToolButton::SetPropertys()
{
	CtmButton::SetPropertys();
	SetPropInfo("key", 		tmWORD, 	&m_wKey);
	SetPropInfo("group",	tmWORD, 	&m_wGroup);
	SetPropInfo("action",	tmWORD, 	&m_wAction);
	SetPropInfo("formname",	tmSTR, 		&m_pszWndName);	
	SetPropInfo("barname",	tmSTR,		&m_pszToolBarName);
	SetPropInfo("subbarname",	tmSTR,	&m_pszSubToolBarName);
}

void	CtmToolButton::SetPropertys_Nub()
{
	CtmButton::SetPropertys_Nub();
	SetPropInfo(TOOLBUTTON_KEY, 		tmWORD, 	&m_wKey);
	SetPropInfo(TOOLBUTTON_GROUP,		tmWORD, 	&m_wGroup);
	SetPropInfo(TOOLBUTTON_ACTION,		tmWORD, 	&m_wAction);
	SetPropInfo(TOOLBUTTON_FORMNAME,	tmSTR, 		&m_pszWndName);	
	SetPropInfo(TOOLBUTTON_BARNAME,		tmSTR,		&m_pszToolBarName);
}

void CtmToolButton::OnLoseFocus()		/* Virtual function */
{
    //CtmWnd::DrawLoseFocus();
    //Update();
}

void CtmToolButton::OnGetFocus()		/* Virtual function */
{
	//CtmWnd::DrawGetFocus();
}

void        CtmToolButton::Update() 
{
	//if (!m_bOnFocus)
	//{
		//Clear();
		CtmButton::Show();
	//}	
	//else CtmButton::Show();
}

WORD	CtmToolButton::OnKey(WORD	wKey)
{
	return wKey;
}
