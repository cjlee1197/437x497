/*Created by J.Wong 2017/2/6 9:40:06*/

#include "toggle_p.h"
#include "toggle.h"
#include "colortable.h"
#include "utils.h"
#include "drawtext.h"

extern BOOL	g_bFocusChanged;


static inline int64 clearBit(int64 lValue, WORD bit)
{
	lValue&= ~(0x1LL << bit);
	
	return lValue;
}

static inline int64 setBit(int64 lValue, WORD bit)
{
	return lValue|(0x1LL << bit);
}

CTogglePrivate::CTogglePrivate()
{
	init();
}

CTogglePrivate::~CTogglePrivate()
{
	
}

void CTogglePrivate::init()
{
		oldStatus = false;
		status = false;
		selectColor = Gemini::blue_level2;
		unSelectColor = Gemini::gray_level0;
		outlineColor =  Gemini::dark_gray_level2;
		oldOutlineColor = Gemini::dark_gray_level0;
		redraw =false;
		touched = false;
		transparent = false;
		oldTransparent = false;
		size = CSize(60,20);
		mgn = CMargin(1,1,1,1);
		x = 0;
		y = 0;
		focus = false;
		positive = false;
		properchanged=false;
		usebit=false;
		bit=0;
		
		memset(szPrompt0,0,tmMAX_TEXT_COL*2);
		
		startIndex=0;
		
		replaceImage=false;
}

IMPLEMENT_DYNCREATE(CtmToggle, CtmWnd)

CtmToggle::CtmToggle():CtmWnd()
{
	strcpy(TYPEName, "CtmToggle");
	d=new CTogglePrivate();
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);
		//d->fmImage.fill((uint32)m_bgc);
	}
	pszPromptID=0;
	pszPromptLan=0;
	pszPrompt=0;
	selectImagePath=0;
	unSelectImagePath=0;
	dbid=0;
}

CtmToggle::~CtmToggle()
{
	delete d;
}

void    CtmToggle::Update()
{
	if(!d->focus)
	{
		GetDBInfo(d);
		if(d->oldStatus!=d->status)
		{
			d->oldStatus=d->status;
			d->properchanged=true;
		}
	}
	Show();
}

void    CtmToggle::Clear()
{
	if(d)
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
}

BOOL 	CtmToggle::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
}

void 	CtmToggle::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"selected-color",	tmDWORD, &d->selectColor);
		SetPropInfo((char*)"unselected-color",	tmDWORD, &d->unSelectColor);
		SetPropInfo((char*)"outline-color", 	tmDWORD, &d->outlineColor);
		SetPropInfo((char*)"status", 			tmBOOL,	 &d->status);
		
		SetPropInfo((char*)"touched",			tmBOOL,	 &d->touched);
		SetPropInfo((char*)"transparent",		tmBOOL,	 &d->transparent);
		SetPropInfo((char*)"private-changed", 	tmBOOL,  &d->properchanged);
		SetPropInfo((char*)"use-bit", 			tmBOOL,  &d->usebit);
		SetPropInfo((char*)"bit", 				tmWORD,	 &d->bit);
		SetPropInfo((char*)"prompt",			tmSTR, 	 &pszPromptLan);
		SetPropInfo((char*)"promptID",			tmSTR, 	 &pszPromptID);
		SetPropInfo((char*)"dbid0",				tmSTR, 	 &dbid);
		
		//该属性仅当不使用bit位表示时有效
		SetPropInfo((char*)"start-index",		tmWORD,	 &d->startIndex);
		
		SetPropInfo((char*)"selected-imagepath", tmSTR,  &selectImagePath);
		SetPropInfo((char*)"unselected-imagepath",tmSTR, &unSelectImagePath);
		SetPropInfo((char*)"image-replace", tmBOOL, 	&d->replaceImage);
	}
		
}



void    CtmToggle::OnGetFocus()
{
		if(d)
		{
			GetDBInfo(d);
			Update();
			d->oldStatus=d->status;
			d->focus = true;
			CtmWnd::DrawGetFocus();
			Prepare(d);
			Prompt(d->szPrompt0);
		}
}

void    CtmToggle::OnLoseFocus()
{
		if(d)
		{
			d->focus = false;
			CtmWnd::DrawLoseFocus();
			if(!g_bFocusChanged)
			{
				SyncDBValue(d);
			}
			d->properchanged=true;
			Show();
		}
}



void	CtmToggle::SetSelectedColor(uint color)
{
	if(d)
		d->selectColor=color;
}

uint	CtmToggle::GetSelectedColor()const
{
	if(d)
		return d->selectColor;
	return 0;
}

void 	CtmToggle::SetUnselectedColor(uint color)
{
	if(d)
		d->unSelectColor=color;
}

uint	CtmToggle::GetUnselectedColor()const
{
	if(d)
		return d->unSelectColor;
	return 0;
}

void 	CtmToggle::SetOutlineColor(uint color)
{
	if(d)
		d->outlineColor=color;
}

uint	CtmToggle::GetOutlineColor()const
{
	if(d)
		return d->outlineColor;
	return 0;
}


void	CtmToggle::SetStatus(bool status)
{
	if(d)
		d->status=status;
}

bool	CtmToggle::GetStatus()const
{
	if(d)
		return d->status;
	return 0;	
}

void CtmToggle::Resize(CTogglePrivate* d)
{
	if(d)
	{
		if(d->transparent)
		{
			//透明色,如果buffer中存在背景怎将其贴到原位置
			// if(!d->bgImage.isNull())
			// {
				// _SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());	
				// _PutImage(0,0,d->bgImage);
			// }
		}
		d->fmImage=CImage(CSize(GetClientWidth(),GetClientHeight()),CImage::Format_RGB32);
		d->size=CSize(GetClientWidth(),GetClientHeight());
		d->x=m_rectClient.left;
		d->y=m_rectClient.top;
		_SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());
		d->bgImage=d->fmImage.copy(CRect(CPoint(0,0),d->size));
		//获取新位置处的背景色
		if(!d->bgImage.isNull())
			_GetImage(0,0,d->bgImage);
	}
}

bool CtmToggle::IsNeedResize(CTogglePrivate* d)
{
	if(d)
		return m_rectClient.left!=d->x||m_rectClient.top!=d->y||CSize(GetClientWidth(),GetClientHeight())!=d->size;
	else
		return false;	
}


bool CtmToggle::IsTransparentChanged(CTogglePrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->transparent!=d->oldTransparent)
		{
			d->oldTransparent=d->transparent;	
			res=true;
		}
	}	
	return res;
}

bool CtmToggle::IsOutlineChanged(CTogglePrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->outlineColor!=d->oldOutlineColor)
		{
			d->oldOutlineColor=d->outlineColor;
			res = true;
		}
	}
	return res;
}


static inline void DrawSlot(CTogglePrivate* d,BGC bgc)
{
	if(d)
	{
		if(d->redraw)
		{
			CPainter painter(&d->fmImage);
			CPen pen;
			painter.setRenderHints(CPainter::Antialiasing,true);
			pen.setWidth(1);
			pen.setColor(d->outlineColor);
			painter.setPen(pen);
			CRect rect(CPoint(0,0),d->size);
			rect-=(d->mgn+2);
			
			CPainterPath path;
			
			int len=mMin(rect.width(),rect.height());
			CRectF left=CRect(rect.x(),rect.y(),len,len);
			CRectF right=CRect(rect.right()-len,rect.y(), len, len);
			CPointF leftCenter=left.center();
			CPointF rightCenter=right.center();
			path.arcMoveTo(left,90);
			path.arcTo(left,90,180);
			path.lineTo(rightCenter.x(),right.bottom());
			path.arcTo(right, -90,180);
			path.lineTo(leftCenter.x(),left.top());
			if(!d->transparent)
			{
				painter.setBrush(CBrush(bgc));
			}
			painter.drawPath(path);
					
		}	
	}	
}

static inline void DrawButton(CTogglePrivate* d)
{
	if(d)
	{
		CPainter painter(&d->image);
		CPen pen;
		painter.setRenderHints(CPainter::Antialiasing,true);
		pen.setWidth(1);
		pen.setColor(d->outlineColor);
		painter.setPen(pen);
		CRect rect(CPoint(0,0),d->size);
		rect-=(d->mgn+2);
		int len=mMin(rect.width(),rect.height());
		CRectF left=CRect(rect.x(),rect.y(),len,len);
		CRectF right=CRect(rect.right()-len,rect.y(), len, len);
		CRectF btnRect;
		btnRect=(d->status)?(right-(d->mgn+2)):(left-(d->mgn+2));
		int btncolor=(d->status)?(d->selectColor):(d->unSelectColor);
		CBrush brush(btncolor);
		painter.setBrush(brush);
		painter.drawEllipse(btnRect);
		// const char* pszText=(d->status)?"ON":"OFF";
		// if(g_pDrawText)
		// {
			// btnRect-=(d->mgn+2);
			// int fontsize=mMin(((int)btnRect.width())/3<<1,(int)btnRect.height());
			// int half=(int)(btnRect.height()-fontsize)>>1;
			// btnRect-=CMargin(0,half,0,half);
			// g_pDrawText->SetFontSize(fontsize,fontsize,0x02);
			// g_pDrawText->DrawText(d->image,(unsigned char*)pszText,btnRect.toRect(),3);
		// }
	}
}
static inline void DrawImage(CTogglePrivate* d)
{
	if(d)
	{
		if(!d->selectImage.isNull()&&!d->unSelectImage.isNull())
		{
			if(d->status)
			{
				//
				CRect rect=CRect(CPoint(0,0),d->size);
				rect-=CMargin(2,2,2,2);
				CRect selectRect=d->selectImage.rect();
				rect&=selectRect;
				for(int i=0; i<rect.height(); ++i)
				{
					memcpy(d->image.scanLine(i+2)+2*sizeof(DWORD),d->selectImage.constScanLine(rect.height()-i-1),rect.width()*sizeof(DWORD));
				}
			}
			else
			{
				//
				CRect rect=CRect(CPoint(0,0),d->size);
				rect-=CMargin(2,2,2,2);
				CRect unSelectRect=d->unSelectImage.rect();
				rect&=unSelectRect;
				for(int i=0; i<rect.height(); ++i)
				{
					memcpy(d->image.scanLine(i+2)+2*sizeof(DWORD),d->unSelectImage.constScanLine(rect.height()-i-1),rect.width()*sizeof(DWORD));
				}
			}
		}
	}
}

void    CtmToggle::Show()
{
	if(IsNeedResize(d)|IsTransparentChanged(d)||
	   IsOutlineChanged(d))
	{
		if(IsNeedResize(d))
		{
			Resize(d);
		}
		if(!d->bgImage.isNull())
			d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		d->redraw=true;
		d->properchanged=true;
	}
	else
	{
		d->redraw=false;	
	}
	
	if(d->focus)
	{
		d->properchanged=true;
	}
	if(d->properchanged)
	{
		if(m_bVisible)
		{
			if(!d->focus)
			{
				GetDBInfo(d);
			}
			if(d->replaceImage)
			{
				Clear();
				GetImage(d);
				DrawImage(d);
			}
			else
			{
				DrawSlot(d,m_bgc);
				Clear();
				DrawButton(d);
			}
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
		 }
		 else
		 {
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->bgImage);	
		 }
		 d->properchanged=false;
	}
}


//Event Process

WORD    CtmToggle::OnKey(WORD wKey)
{
	WORD			wResult=wKey;
	if(!d)			return wKey;
	if(!d->focus)	return wKey;
	switch (wKey)
	{
		case _KEY_LEFTFIELD:
			d->status=false;
			Update();
			wResult=_NULL_KEY;
			break;
		case _KEY_RIGHTFIELD:
			d->status=true;
			Update();
			wResult=_NULL_KEY;
			break;
		case _ENTER:
			d->status=(!d->status);
			Update();
			break;
		default:
			break;	
	}
	return wResult;		
}

bool CtmToggle::TouchAreaChanged(CTogglePrivate* d,const CPoint& control)
{
	bool res=false;
	if(d)
	{
		CPoint cmp=control+CPoint(-m_rectClient.left,-m_rectClient.top);
		CRect rect=CRect(CPoint(0,0),d->size);
		if(rect.contains(cmp))
		{
			d->status=(!d->status);
			res=true;	
		}
	}
	return res;	
}	

void	CtmToggle::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	if(!d->touched) return ;
	int x=wParam>>16;
	int y=wParam&0x0000FFFF;
	switch(message)
	{
		case MSG_MOUSELUP:
			{
				bool changed=TouchAreaChanged(d,CPoint(x,y));
				if(changed)	Update();
			}
			break;
		case MSG_MOUSELDOWN:
			break;
		default:
			break;
	}
}

void CtmToggle::GetDBInfo(CTogglePrivate* d)
{
	if(d)
	{
		if(dbid!=0)
		{
			DBVIEWVALUE dbViewValue;
			dbViewValue =GetDBValue(dbid);
			if(d->usebit)
			{
				d->status=(dbViewValue.lValue>>d->bit)&0x1;
			}
			else
			{
				d->status=(dbViewValue.lValue-d->startIndex)>0?1:0;
				
			}
		}
	}
}

void CtmToggle::SyncDBValue(CTogglePrivate* d)
{
	if(d)
	{
		if(dbid)
		{
			if(d->status!=d->oldStatus)
			{
				if(d->usebit)
				{
					DBVIEWVALUE dbViewValue;
					dbViewValue =GetDBValue(dbid);
					long long  lValue=dbViewValue.lValue;
					if(d->status)
						lValue=setBit(lValue, d->bit);
					else
						lValue=clearBit(lValue, d->bit);
					SetDBValue(dbid, lValue);
				}
				else
				{
					
					SetDBValue(dbid, d->status+d->startIndex);
				}
			}
		}
	}
}

void CtmToggle::ResetPrompt(CTogglePrivate* d)
{
	if(d)
	{
		if(pszPromptID)
			pszPrompt=g_MultiLanguage[pszPromptID];
		else
			pszPrompt=pszPromptLan;
	}
}

void CtmToggle::Prepare(CTogglePrivate* d)
{
	if(d)
	{
		ResetPrompt(d);
		long lDataID=0;
		if(g_bDemoDataID)
		{
			if(dbid)
			{
				lDataID=g_pDatabase->GetDataID(dbid);
			}
			if(m_pcFont->GetFontCode()==tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0,0,tmMAX_TEXT_COL*2);
					tm_sprintf(d->szPrompt0, "%x %t", lDataID, pszPrompt);
				}
				else
				{
					memset(d->szPrompt0,0,tmMAX_TEXT_COL*2);
					tm_sprintf(d->szPrompt0, "%t", pszPrompt);
				}
			}
			else
			{
				if(pszPrompt)
				{
					if(strlen(pszPrompt)<tmMAX_TEXT_COL*2)
						sprintf(d->szPrompt0,"%s",pszPrompt);
				}
			}
		}
	}
}

bool CtmToggle::GetImage(CTogglePrivate* d)
{
	if(d)
	{
		bool tmp[2]={false};
		CtmBitMap* pBmpImage[2] = {NULL};
		if(selectImagePath&&unSelectImagePath)
		{
			int i = 0;
			
			while (g_aIniBmp[i].cBMPPath[0] != '\0' && i < IMAGE_MAXNUM)
			{
				if (!tmp[0]&&strcmp(g_aIniBmp[i].cBMPPath, selectImagePath) == 0)
				{
					pBmpImage[0] = g_aIniBmp[i].pIniBmp; 
					tmp[0] =true;
				}
				if(!tmp[1]&&strcmp(g_aIniBmp[i].cBMPPath, unSelectImagePath) == 0)
				{
					pBmpImage[1] = g_aIniBmp[i].pIniBmp; 
					tmp[1] =true;
				}
				if(tmp[0]&&tmp[1])
				{
					break;
				}
				i++;
			}	
			if (!tmp[0]||!tmp[1])
			{
				if(!tmp[0])
				{
					if(i < IMAGE_MAXNUM)
					{
						g_aIniBmp[i].pIniBmp = new CtmBitMap;
						g_aIniBmp[i].pIniBmp->Create(selectImagePath);
						strncpy(g_aIniBmp[i].cBMPPath, selectImagePath, 256);
						pBmpImage[0] = g_aIniBmp[i].pIniBmp;
						i++;
					}
					else
					{
						d->m_bmpImage[0].Create(selectImagePath);	
						pBmpImage[0] = &d->m_bmpImage[0];
					}
				}
				
				if(!tmp[1])
				{
					if(i < IMAGE_MAXNUM)
					{
						g_aIniBmp[i].pIniBmp = new CtmBitMap;
						g_aIniBmp[i].pIniBmp->Create(unSelectImagePath);
						strncpy(g_aIniBmp[i].cBMPPath, unSelectImagePath, 256);
						pBmpImage[1] = g_aIniBmp[i].pIniBmp;
					}
					else
					{
						d->m_bmpImage[1].Create(unSelectImagePath);	
						pBmpImage[1] = &d->m_bmpImage[1];
					}
				}
			}
		}
		if(pBmpImage[0]&&pBmpImage[1])
		{
			d->selectImage=CImage((const uint8*)pBmpImage[0]->Data(),pBmpImage[0]->GetWidth(),pBmpImage[0]->GetHeight(),CImage::Format_RGB32);
			d->unSelectImage=CImage((const uint8*)pBmpImage[1]->Data(),pBmpImage[1]->GetWidth(),pBmpImage[1]->GetHeight(),CImage::Format_RGB32);
		}
	}
	
}




	

