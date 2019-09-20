/*
*	Created by J.Wong
*/

#include "lineedit.h"
#include "tmconfig.h"
#include "encode.h"
#include "drawtext.h"
#include "libxmlcomm.h"
#include "imagebox.h"
#include "cbitmap.h"

const	  int		MAXLEN=4096*16;
const	  int		MAXLINE=1024;
const	  int		MAXCONTROLS=256;
IMPLEMENT_DYNCREATE(CtmLineEdit, CtmWnd)

class CtmLineEdit::CtmPrivateData{
	public:
		CtmPrivateData():m_wOffset(0),m_wHeigt(150),m_wWidth(200),m_wLen(0)
			,m_lines(0),m_fonts(0),m_vstep(0),m_hstep(0),m_rows(100),m_cols(100),m_maxcols(0),
			m_bBackGround(0),m_bWrap(1),m_high_color(65513),m_is_high_show(FALSE){
			
			memset(m_szContent,0,sizeof(m_szContent)/sizeof(char));
			memset(m_uniContent,0,sizeof(m_uniContent));
			memset(m_uniDetail,0,sizeof(m_uniDetail));
			memset(m_pIcon,0,sizeof(m_pIcon));
			m_encode.SetCharset("utf-8","utf-8");
			m_IconAttr.wStyle 		= 2;
			m_IconAttr.cImagePath 	= NULL;
			m_IconAttr.dwKeyColor	= 63519;
			m_IconAttr.wType		= 0x30;
			m_nMode					= 0;
			m_bmp					= 0;
		}
	public:
		WORD	m_wOffset;
		WORD	m_wHeigt;
		WORD	m_wWidth;
		char	m_szContent[MAXLEN];
		unsigned int 	m_uniContent[MAXLEN];
		unsigned int 	m_uniDetail[MAXLEN];
		WORD	m_wLen;
		WORD	m_lines;
		WORD	m_fonts;
		int		m_vstep;
		int		m_hstep;
		WORD	m_rows;
		WORD	m_cols;
		WORD	m_maxcols;
		int		m_high_color;
		bool	m_is_high_show;
		CtmEncode	m_encode;
		CtmBitMap*  m_pIcon[MAXCONTROLS];
		CtmBitMap*	m_bmp;
		std::map<int,tmControl> 	m_context;
		std::map<string,tmControl>  m_szcontext;
		std::map<int,string>	m_row_img;
		std::map<int, int>		m_index_row;
		std::map<int, int>		m_id_row;
		std::map<string, int>	m_strid_row;
		tmIMAGEBOX				m_IconAttr;
		ImageBackground			m_BackImage;
		BOOL					m_bBackGround;
		BOOL					m_bWrap;
		int						m_nMode;
		
		unsigned int**			m_uniContext;
		uint					m_isTitle[32];	
};

CtmLineEdit::CtmLineEdit():CtmWnd()
{
	strcpy(TYPEName, "CtmLineEdit");
	m_data	=	new CtmPrivateData();
	m_pszContext	=	0;
	if(CtmConfig::GetInstance()->GetLanguageType()==1)
   		m_iFontUNType		= 1;
	else
		m_iFontUNType		= 0;	
}

CtmLineEdit::~CtmLineEdit()
{
	for(int i=0; i<m_data->m_lines; i++)
	{
			delete[] m_pszContext[i];
			delete[] m_data->m_uniContext[i];
	}
//	for(int i=0; i<m_data->m_lines; i++)
//	{
//		if(m_data->m_pIcon[i])
//		{
//			delete m_data->m_pIcon[i];	
//		}	
//	}
	if(m_data->m_bmp)
		delete m_data->m_bmp;
	delete[] m_pszContext;
	delete[] m_data->m_uniContext;
	delete m_data;	
}

BOOL 	CtmLineEdit::CreateA()
{
	WORD	wFontHeight=0;
	WORD	wFontWidth=0;
	CtmWnd::CreateA();
	SetBorderWidth(1);
	m_data->m_wHeigt=m_rectWindow.bottom  - m_rectWindow.top+1;
	m_data->m_wWidth=m_rectWindow.right  - m_rectWindow.left-8;
	if(m_iFontUNType >= 1)	
	{
		wFontHeight = m_iFontUNHeight;
		wFontWidth  = m_iFontUNWidth;
		if (m_data->m_wOffset < 0) m_data->m_wOffset = 0;
		m_data->m_lines=m_data->m_wHeigt/(2*wFontHeight);
		//m_data->m_fonts=m_data->m_wWidth*2/wFontWidth;
		m_pszContext=new char*[m_data->m_lines];
		m_data->m_uniContext=new unsigned int*[m_data->m_lines];
		for(int i=0; i<m_data->m_lines; i++)
		{
			m_pszContext[i]=new char[MAXLINE];
			m_data->m_uniContext[i]=new unsigned int[MAXLINE];
			//m_data->m_pIcon[i]=new CtmBitMap();
		}
		
		
	}
	
}

void 	CtmLineEdit::SetPropertys()
{
	CtmWnd::SetPropertys();	
	SetPropInfo("align",	tmWORD, &m_paragraph.wAlign);
	SetPropInfo("highcolor",tmLONG, &m_data->m_high_color);
	SetPropInfo("highshow",	tmBOOL, &m_data->m_is_high_show);
}

void    CtmLineEdit::Show()
{
	RECT rect;
	memset(&rect,0,sizeof(RECT));
	Clear();
	DrawBorder();
	if(m_iFontUNType>=1)
	{
		
		if(m_data->m_nMode)
		{
			int nRight  = m_rectWindow.right  - m_rectWindow.left;
			int nBottom = m_rectWindow.bottom - m_rectWindow.top;
			_SetColor(0x3C415);
	        _MoveTo(    8*m_iFontUNWidth-4,  nBottom);
	        _LineTo(    8*m_iFontUNWidth-4,        0);
	        for(int i=1; i<m_data->m_lines; i+=1)
	       	{
	       		 _MoveTo(  0,  				   i*2*m_iFontUNHeight);
	       		 _LineTo(  nRight, 			   i*2*m_iFontUNHeight);	
	       	}
	    }
	    int isShowIcon[MAXCONTROLS]={0};
	    int nWidth=0;
	 	int nHeight=0;
	 	
	 	
	 	for(int i=0; i<m_data->m_lines; i++)
	 	{
	 		//printf("i=%d,m_data->m_wOffset=%d\n",i,m_data->m_wOffset,(2*m_data->m_lines-1)*m_iFontUNHeight);
	 		if(m_data->m_wOffset<(2*m_data->m_lines-1)*m_iFontUNHeight)
	 		{
	 			if(!m_data->m_hstep)
	 			{
			 		std::map<int,string>::iterator it=m_data->m_row_img.find(i+m_data->m_vstep);
			 		if(it!=m_data->m_row_img.end())
			 		{
			 			int rows=m_data->m_index_row[i+m_data->m_vstep];
			 			//if((i+rows)>m_data->m_lines)	rows=m_data->m_lines-i;
			 			m_data->m_pIcon[i]=GetBmpImage((char*)it->second.c_str());
			 			nHeight=m_data->m_pIcon[i]->m_lHeight;
			 			nWidth=m_data->m_pIcon[i]->m_lWidth;
			 			//if(nHeight>=rows*m_iFontUNHeight*2)	nHeight=rows*m_iFontUNHeight*2;
				 		rows=nHeight/(m_iFontUNHeight*2)+1;
				 		if((i+rows)>m_data->m_lines)	rows=m_data->m_lines-i;
				 		if(nWidth>=(m_rectWindow.right  - m_rectWindow.left-4))
				 		{
				 				nWidth=m_rectWindow.right  - m_rectWindow.left-4;
				 		}
				 		if(nWidth>=8*m_iFontUNWidth-4)
				 		{
				 			m_data->m_wOffset+=nHeight;	
				 		}
				 		
				 		for(int j=i; j<(i+rows); j++)
			 			{
			 				
			 				isShowIcon[j]=1;
			 			}
				 		rect.left=0;
						rect.right=nWidth;
						rect.top=i*2*m_iFontUNHeight;
				 		rect.bottom=rect.top+nHeight;
				 		ShowImage(rect,i);	
			 		}
			 	
		 			else
		 			{
		 				;
		 			}
		 			_SetViewPort(m_rectWindow.left, 
						m_rectWindow.top, 
						m_rectWindow.right, 
						m_rectWindow.bottom
					);
		 		}
		 		if(m_data->m_bWrap)
		 		{
		 			if(*m_data->m_uniContext[i])
		 			{
		 				g_pDrawText->SetHorizontalOffset(m_data->m_hstep);
				 		if(m_data->m_nMode)
				 		{
				 			if(m_data->m_is_high_show&&m_data->m_isTitle[i])
				 			{
					 			int len=g_pDrawText->GetTextLength(m_data->m_uniContext[i], 0);
					 			_FillRect(m_nBgcColorControl, m_data->m_high_color, m_data->m_high_color, 
		    						8*m_iFontUNWidth, m_data->m_wOffset, 8*m_iFontUNWidth+len, m_data->m_wOffset+2*m_iFontUNHeight, m_wDestColor, m_nColorChange);
		    				}
				 			CtmWnd::ShowExtent(8*m_iFontUNWidth,m_data->m_wOffset+m_iFontUNHeight/2,m_data->m_uniContext[i],-1);
				 			
				 		}
				 		else
				 		{
				 			if(isShowIcon[i])
				 			{
				 				if(nWidth>=8*m_iFontUNWidth-4)
				 				{
				 					if(m_data->m_is_high_show&&m_data->m_isTitle[i])
				 					{
					 					int len=g_pDrawText->GetTextLength(m_data->m_uniContext[i], 0);
					 					_FillRect(m_nBgcColorControl, m_data->m_high_color, m_data->m_high_color, 
		    								4, m_data->m_wOffset, 4+len, m_data->m_wOffset+2*m_iFontUNHeight, m_wDestColor, m_nColorChange);
		    						}
				 					CtmWnd::ShowExtent(4,m_data->m_wOffset+m_iFontUNHeight/2,m_data->m_uniContext[i],-1);
				 				}
				 				else
				 				{
				 					if(m_data->m_is_high_show&&m_data->m_isTitle[i])
				 					{
					 					int len=g_pDrawText->GetTextLength(m_data->m_uniContext[i], 0);
					 					_FillRect(m_nBgcColorControl, m_data->m_high_color, m_data->m_high_color, 
		    								4+nWidth, m_data->m_wOffset, 4+len+nWidth, m_data->m_wOffset+2*m_iFontUNHeight, m_wDestColor, m_nColorChange);
		    						}
				 					CtmWnd::ShowExtent(nWidth+4,m_data->m_wOffset+m_iFontUNHeight/2,m_data->m_uniContext[i],-1);
				 				}
				 			}
				 			else
				 			{
				 				if(m_data->m_is_high_show&&m_data->m_isTitle[i])
				 				{
					 					int len=g_pDrawText->GetTextLength(m_data->m_uniContext[i], 0);
					 					_FillRect(m_nBgcColorControl, m_data->m_high_color, m_data->m_high_color, 
		    								4, m_data->m_wOffset, 4+len, m_data->m_wOffset+2*m_iFontUNHeight, m_wDestColor, m_nColorChange);
		    					}
				 				CtmWnd::ShowExtent(4,m_data->m_wOffset+m_iFontUNHeight/2,m_data->m_uniContext[i],-1);	
				 			}
				 		}
				 		m_data->m_wOffset+=	2*m_iFontUNHeight;	
		 			}
		 				
		 		}
		 		else
		 		{
				 	if(*m_pszContext[i]!=0)
				 	{
				 		g_pDrawText->SetHorizontalOffset(m_data->m_hstep);
				 		if(m_data->m_nMode)
				 			CtmWnd::ShowExtent(8*m_iFontUNWidth,m_data->m_wOffset+m_iFontUNHeight/2,m_pszContext[i],0);
				 		else
				 		{
				 			if(isShowIcon[i])
				 			{
				 				if(nWidth>=8*m_iFontUNWidth-4)
				 					CtmWnd::ShowExtent(4,m_data->m_wOffset+m_iFontUNHeight/2,m_pszContext[i],0);
				 				else
				 					CtmWnd::ShowExtent(nWidth+4,m_data->m_wOffset+m_iFontUNHeight/2,m_pszContext[i],0);
				 			}
				 			else
				 			{
				 				CtmWnd::ShowExtent(4,m_data->m_wOffset+m_iFontUNHeight/2,m_pszContext[i],0);	
				 			}
				 		}
				 		m_data->m_wOffset+=	2*m_iFontUNHeight;
				 	}
				}
		 	}
	 	}	
	}
}

void    CtmLineEdit::Update()
{
	Show();
}

void	CtmLineEdit::SetContent(char* pszContent,WORD wLen)
{
	if(!pszContent)		return ;
	if(wLen>MAXLEN)		m_data->m_wLen=MAXLEN;
	else				m_data->m_wLen=wLen;
	m_data->m_wOffset	= 0;
	memset(m_data->m_szContent,0,sizeof(m_data->m_szContent)/sizeof(char));
	memcpy(m_data->m_szContent,pszContent,wLen);
	if(m_iFontUNType >= 1)
	{
		if(m_data->m_bWrap)
		{
			int nLen = 0, nUTFLen = 0;
			nLen=m_data->m_encode.Utf8ConvertUnicode((unsigned char*)m_data->m_szContent,(unsigned long*)m_data->m_uniContent,&nUTFLen);		
			m_data->m_rows=getRows(m_data->m_uniContent,nLen);
		}
	 	else
	 	{
	 		m_data->m_rows=getRows(m_data->m_szContent);
		}
	}
}

void    CtmLineEdit::DrawBorder()
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




void	CtmLineEdit::SetCopyBackImage(BOOL bCopy)
{
	
}

/*
* @param : pszContent: modify context
* @param : step:	positive forward or negative backward characters
* @return: return counts of the longest line
*/
int		CtmLineEdit::StepContent(int hstep, int vstep)	
{
	//find maxrows and maxcols
	
	if((m_data->m_hstep+hstep)<=(m_data->m_maxcols/2+m_data->m_maxcols)&&(m_data->m_hstep+hstep)>=0) 
		m_data->m_hstep+=hstep;
	if((m_data->m_vstep+vstep)<=m_data->m_rows&&(m_data->m_vstep+vstep)>=0)
		m_data->m_vstep+=vstep;
	if(!m_data->m_bWrap)
		getLines(m_data->m_szContent);
	else
	{
			int nLen = 0, nUTFLen = 0;
			int offset=0;
			memset(m_data->m_uniContent,0,sizeof(m_data->m_uniContent));
			nLen=m_data->m_encode.Utf8ConvertUnicode((unsigned char*)m_data->m_szContent,(unsigned long*)m_data->m_uniContent,&nUTFLen);
			unsigned int* uniContent=setStartLine(m_data->m_uniContent, &offset,m_data->m_vstep);	
			getLines(uniContent, nLen-offset);
	}
	//printf("rows=%d,cols=%d\n",m_data->m_rows,m_data->m_cols);
	//printf("m_data->m_vstep=%d,m_data->m_hstep=%d\n",m_data->m_vstep,m_data->m_hstep);
	return 0;
	
}

/*
* @param : pszInContent: pass context
* @param : pszOutContent: return context
* @return: return counts
*/
int		CtmLineEdit::getLine(char* pszInContent,char* pszOutContent)
{
	int nResult=0;
	if(!pszInContent||!pszOutContent)	return nResult;
	char* pTemp=pszInContent;
	while(!*pTemp) pTemp++;
	while(*pTemp)
	{
		nResult++;
		if(*pTemp=='\n')	break;
		pTemp++;
	}
	if(nResult)
	{
		if(nResult>MAXLINE-1)
		{
			
			memcpy(pszOutContent,pszInContent,MAXLINE-1);
			pszOutContent[MAXLINE-1]=0;	
		}
		else
		{
			memcpy(pszOutContent,pszInContent,nResult);
			int nLen = 0, nUTFLen = 0;
			DWORD* pwUnicode=new DWORD[MAXLINE*4];
			nLen=m_data->m_encode.Utf8ConvertUnicode((unsigned char*)pszOutContent,pwUnicode,&nUTFLen);
			m_data->m_cols=nLen;
			m_data->m_maxcols=max(m_data->m_maxcols,m_data->m_cols);
			delete[] pwUnicode;
		}	
	}
	return --nResult;		
}

int	 CtmLineEdit::getLines(char* pszInContent)
{
	int nResult=0;
	int offset=0;
	
	char* pszStartContent=setStartLine(pszInContent,m_data->m_vstep);
	//printf("pszStartContent=%x\n",pszStartContent);
	for(int i=0; i<m_data->m_lines; i++)
	{
		
		memset(m_pszContext[i],0,MAXLINE);
		nResult=getLine(pszStartContent+offset,m_pszContext[i]);
		//printf("nResult=%d\n",nResult);
		offset+=++nResult;
		
	}
	return 0;	
}
int		CtmLineEdit::getRows(char*  pszInContent)
{
	int nResult=0;
	if(!pszInContent)	return nResult;
	char* pTemp=pszInContent;
	int flag=0;
	while(*pTemp)
	{
		flag=0;
		if(*pTemp=='\n')
		{
			flag=1;
			nResult++;
		}
		pTemp++;
	}
	if(!flag)
		nResult++;
	return nResult;	
}

char*	CtmLineEdit::setStartLine(char* pszInContent,int vstep)
{
	int nResult=0;
	if(!pszInContent||!vstep)	return pszInContent;
	char* pTemp=pszInContent;
	while(*pTemp)
	{
		if(*pTemp=='\n')
		{
			nResult++;
			if(nResult==vstep)
			{	
				pTemp++;
				break;
			}
		}
		pTemp++;
	}
	return pTemp;		
} 

int		CtmLineEdit::GetRows()
{
	return m_data->m_rows+1;	
}

int		CtmLineEdit::GetMaxCols()
{
	//printf("cols=%d\n",m_data->m_maxcols);
	return m_data->m_maxcols-1;	
}

void	CtmLineEdit::SetContext(std::map<string, tmControl > context)
{
		m_data->m_szcontext=context;
		int result=0;
		std::map<string, tmControl>::iterator it=m_data->m_szcontext.begin();
		for(;it!=m_data->m_szcontext.end();++it)
		{
			if(it->second.img)
			{
				m_data->m_row_img.insert(pair<int,string>(result,string(reinterpret_cast<char*>(it->second.img))));
			}
			if(it->second.id)
			{
				//printf("id=%s\n",it->second.id);
				string id(reinterpret_cast<const char*>(it->second.id));
				if(it->second.buddy)
				{
					//printf("id=%s,buddy=%s\n",it->second.id,it->second.buddy);
					string buddy(reinterpret_cast<const char*>(it->second.buddy));
					map<string, int>::iterator relative=m_data->m_strid_row.find(buddy);
					if(relative!=m_data->m_strid_row.end())
					{
						m_data->m_strid_row.insert(pair<string, int>(id,relative->second));	
					}		
				}
				else
				{
						m_data->m_strid_row.insert(pair<string, int>(id,result));
				}
				//printf("id=%d,result=%d\n",id, result);
			}
			int ntemp=0;
			if(m_data->m_bWrap)
			{
				int nLen = 0, nUTFLen = 0;
				memset(m_data->m_uniDetail,0,sizeof(m_data->m_uniDetail));
				nLen=m_data->m_encode.Utf8ConvertUnicode(reinterpret_cast<unsigned char*>(it->second.detail),(unsigned long*)m_data->m_uniDetail,&nUTFLen);		
				ntemp=getRows(m_data->m_uniDetail, nLen);
			}
	 		else
	 		{
	 			ntemp=getRows(reinterpret_cast<char*>(it->second.detail));
			}
			//int ntemp=getRows(reinterpret_cast<char*>(it->second.detail));
			if(it->second.img)
			{
				m_data->m_index_row.insert(pair<int, int>(result,ntemp));	
			}
			result+=ntemp;
			
			//appendContent(reinterpret_cast<char*>(it->second.detail));	
		}
}

void	CtmLineEdit::appendContent(char* appendString)
{
	;	
}

int		CtmLineEdit::SetStyle(int mode)
{
	m_data->m_nMode=mode;
}

void	CtmLineEdit::ShowImage(RECT rect,int nIndex)
{
	RECT view_rect;
	_SetViewPort(m_rectWindow.left+rect.left, 
						m_rectWindow.top+rect.top, 
						m_rectWindow.right+rect.left, 
						m_rectWindow.bottom
					);
	view_rect.left=0;
	view_rect.right=rect.right-rect.left;
	view_rect.top=0;
	view_rect.bottom=rect.bottom-rect.top;
	if(nIndex>=0&&nIndex<m_data->m_lines)
	{			
		m_data->m_pIcon[nIndex]->SetKeyColor(m_data->m_IconAttr.dwKeyColor);
		m_data->m_pIcon[nIndex]->Show(view_rect,m_data->m_IconAttr.wStyle);
	}	
}


CtmBitMap*		CtmLineEdit::GetBmpImage(char* pszImagePath)
{
	int i = 0;
	bool b_find = false;
	
	CtmBitMap* pBmpImage = NULL;
	while (g_aIniBmp[i].cBMPPath[0] != '\0' && i < IMAGE_MAXNUM)
	{
		if (strcmp(g_aIniBmp[i].cBMPPath, pszImagePath) == 0)
		{
			pBmpImage = g_aIniBmp[i].pIniBmp; 
			b_find = true;
			//cout << Name << " finded the bitmap!!" << endl;
			break;
		}
		i++;
	}	
	if (!b_find)
	{
		if(i < IMAGE_MAXNUM)
		{
			if(!access(pszImagePath,F_OK))
			{
				strcpy(g_aIniBmp[i].cBMPPath, pszImagePath);
				g_aIniBmp[i].pIniBmp = new CtmBitMap;
				g_aIniBmp[i].pIniBmp->Create(pszImagePath);
				pBmpImage = g_aIniBmp[i].pIniBmp;
			}
			else
			{
				if(!m_data->m_bmp)
					m_data->m_bmp=new CtmBitMap;
				m_data->m_bmp->Create(pszImagePath);
				pBmpImage=m_data->m_bmp;	
			} 
		}
		else
		{
			if(!m_data->m_bmp)
				m_data->m_bmp=new CtmBitMap;
			m_data->m_bmp->Create(pszImagePath);
			pBmpImage=m_data->m_bmp;	
		}
		
	}
	
	return pBmpImage;
}

void    CtmLineEdit::Clear()
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

int		CtmLineEdit::LocalRow(int tab)
{
	map<int, int>::iterator id=m_data->m_id_row.find(tab);
	if(id!=m_data->m_id_row.end())	return id->second;
	return 0;	
}

int		CtmLineEdit::LocalRow(char* pszName)
{
	if(!pszName)		return 0;
	string key(pszName);
	map<string, int>::iterator id=m_data->m_strid_row.find(key);
	
	if(id!=m_data->m_strid_row.end())	return id->second;
	return 0;	
}
void	CtmLineEdit::SetContext(std::map<int, tmControl > context)
{
		m_data->m_context=context;
		int result=0;
		std::map<int, tmControl>::iterator it=m_data->m_context.begin();
		for(;it!=m_data->m_context.end();++it)
		{
			if(it->second.img)
			{
				m_data->m_row_img.insert(pair<int,string>(result,string(reinterpret_cast<char*>(it->second.img))));
			}
			if(it->second.id)
			{
				//printf("id=%s\n",it->second.id);
				string id(reinterpret_cast<const char*>(it->second.id));
				if(it->second.buddy)
				{
					//printf("id=%s,buddy=%s\n",it->second.id,it->second.buddy);
					string buddy(reinterpret_cast<const char*>(it->second.buddy));
					map<string, int>::iterator relative=m_data->m_strid_row.find(buddy);
					if(relative!=m_data->m_strid_row.end())
					{
						m_data->m_strid_row.insert(pair<string, int>(id,relative->second));	
					}		
				}
				else
				{
						m_data->m_strid_row.insert(pair<string, int>(id,result));
				}
				//printf("id=%d,result=%d\n",id, result);
			}
			int ntemp=0;
			if(m_data->m_bWrap)
			{
				int nLen = 0, nUTFLen = 0;
				memset(m_data->m_uniDetail,0,sizeof(m_data->m_uniDetail));
				if(reinterpret_cast<unsigned char*>(it->second.detail))
				{
					nLen=m_data->m_encode.Utf8ConvertUnicode(reinterpret_cast<unsigned char*>(it->second.detail),(unsigned long*)m_data->m_uniDetail,&nUTFLen);		
					ntemp=getRows(m_data->m_uniDetail, nLen);
				}
			}
	 		else
	 		{
	 			ntemp=getRows(reinterpret_cast<char*>(it->second.detail));
			}
			//int ntemp=getRows(reinterpret_cast<char*>(it->second.detail));
			if(it->second.img)
			{

				m_data->m_index_row.insert(pair<int, int>(result,ntemp));	
			}
			result+=ntemp;
			
			//appendContent(reinterpret_cast<char*>(it->second.detail));	
		}
}



//add by J.Wong for wrap function 2016/7/19 9:42:14

int			CtmLineEdit::getLines(unsigned int* pUnicode, int nLen)
{
	int nRows=0;
	int Line=0;
	int curIndex=0;
	int nCount=0;
	//m_data->m_lines
	for(int i=0; i<m_data->m_lines; i++)
	{
		memset(m_data->m_uniContext[i],0,MAXLINE);
			
	}
	memset(m_data->m_isTitle,0,sizeof(uint)*32);
	for(int i=0; i<nLen; i++)
	{
		if(curIndex>=m_data->m_lines)	 break;
			
		int nWidth=g_pDrawText->GetFontWidth(pUnicode+i);
		//printf("curIndex=%d,line=%d,width=%d\n",curIndex,Line+nWidth,nWidth);
		nCount++;
		if(pUnicode[i]=='\n')
		{
		
			int redirect=i;
			while(redirect>0)
			{
				--redirect;
				//if(pUnicode[--redirect]=='\t'||pUnicode[redirect]==' ') continue;

				if((pUnicode[redirect]==':')||(pUnicode[redirect]==0xFF1A))
				{
					m_data->m_isTitle[curIndex]=1;
				}
				break;
			}
			//printf("nCount=%d,curIndex=%d\n",nCount,curIndex);
			//memset(m_data->m_uniContext[curIndex],0,MAXLINE);
			if(Line)
				memcpy(m_data->m_uniContext[curIndex++],pUnicode+i+1-nCount,(nCount-1)*4);	
			else
				memcpy(m_data->m_uniContext[curIndex++],&pUnicode[i],4);	
			nRows++;
			nCount=0;
			Line=0;
		}
		else
		if((Line+nWidth)>(m_data->m_wWidth+1))
		{
			//memset(m_data->m_uniContext[curIndex],0,MAXLINE);
			memcpy(m_data->m_uniContext[curIndex++],pUnicode+i+1-nCount,(nCount-1)*4);
			i--;
			Line=0;
			nCount=0;
			nRows++;
		}
		else
			Line+=nWidth;		
	}
	if(nCount)
	{
		memcpy(m_data->m_uniContext[curIndex++],pUnicode+nLen-nCount,(nCount-1)*4);	
	}
	return 0;
}

int 		CtmLineEdit::getRows(unsigned int* pUnicode, int nLen)
{
	int nRows=0;
	int Line=0;
	int flag=0;
	for(int i=0; i<nLen; i++)
	{
		flag=0;
		int nWidth=g_pDrawText->GetFontWidth(pUnicode+i);
		if(pUnicode[i]=='\n')
		{
			flag=1;
			Line=0;	
			nRows++;
		}
		else if(Line+nWidth>(m_data->m_wWidth+1))
		{
			flag=1;
			i--;
			Line=0;
			nRows++;
		}
		Line+=nWidth;	
	}
	if(!flag)
		nRows++;
	return nRows;
}

unsigned int* 		CtmLineEdit::setStartLine(unsigned int* pUnicode, int* pnLen,int vstep)
{
	int nResult=0;
	if(!pUnicode||!vstep)	return pUnicode;
	unsigned int* pTemp=pUnicode;
	int Line=0;
	int offset=0;
	while(*pTemp)
	{
		int nWidth=g_pDrawText->GetFontWidth(pTemp);
		if(*pTemp=='\n')
		{
			nResult++;
			Line=0;
			if(nResult==vstep)
			{	
				pTemp++;
				offset++;
				break;
			}
		}
		else if((Line+nWidth)>(m_data->m_wWidth+1))
		{
			//printf("line=%d,%d\n",Line+nWidth,m_data->m_wWidth+4);
			//pTemp--;
			offset--;
			Line=0;
			nResult++;
			if(nResult==vstep)
			{	
				//pTemp++;
				//offset++;
				break;
			}	
		}
		Line+=nWidth;
		pTemp++;
		offset++;
	}
	*pnLen=offset;
	return pTemp;		
}