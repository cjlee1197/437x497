/*Created by J.Wong 2017/02/22*/

#include "spinbox_p.h"
#include "spinbox.h"
#include "colortable.h"
#include "drawtext.h"
#include "task.h"
#include "utils.h"

extern BOOL g_bFocusChanged;
char ninePad[10][4]=
{
	'0','0','0','0',
	'1','1','1','1',
	'2','2','2','2',
	'3','3','3','3',
	'4','4','4','4',
	'5','5','5','5',
	'6','6','6','6',
	'7','A','B','C',
	'8','D','E','F',
	'9','9','9','9'
};

enum IntegerBase{
	Decimal=0x0A,
	Octal=0x08,
	HexaDecimal=0x10
};

enum TextAlign{
	AlignLeft=0x0001,		//	left
	AlignCenter=0x0002, 	//	X.center
	AlignRight=0x0004, 		//	right
	AlignTop=0x0008,		//	top
	AlignBottom=0x0010,		//	bottom
	AlignYCenter=0x0020		//	Y.Center
};

enum FrameShadow{
	Sunken =0x01,
	Raised =0x02,
	Plain  =0x04
};

enum FrameShape{
	NoFrame=0x00,
	Panel=0x01,
	Box=0x02,
	RoundedPanel=0x04,
	UnderLine=0x05
};
enum TickStyle
{
	AllInRight,
	BothSides
	
};

enum GradientDirection{
	NoGradient=0x000,
	XGradient=0x100,
	YGradient=0x200,
	XYGradient=0x400
	
};

enum wGradientControl{
	GradientDestToSrc =0x02,
	GradientSrcToDest=0x04,
	GradientDestToSrcToDest=0x08,
	GradientSrcToDestToSrc=0x10
	
};


CSpinBoxPrivate::CSpinBoxPrivate()
{
	
	init();
}

CSpinBoxPrivate::~CSpinBoxPrivate()
{
	
	
}

void CSpinBoxPrivate::init()
{
	
	llValue=0;
	llMinValue=0;
	llMaxValue=99;
	llReInputValue=0;
	llLastValue=0;
	
	
	dValue=0;
	dMinValue=0;
	dMaxValue=99;
	dReInputValue=0;
	dLastValue=0;
	integerpart=0;
	decimalpart=0;
	showBits=4;
	/*显示进制*/
	displayIntegerBase=Decimal;

	/*步进大小*/
	tickInterval=1;
	dTickInterval=1;
	
	touched=false;
	transparent=false;
	oldTransparent=false;
	wFrameShadow=Plain;
	wFrameShape=Panel;
	wOldFrameShadow=wFrameShadow;
	wOldFrameShape=wFrameShape;
	hiddenTick=false;
	size=CSize(120,40);
	mgn=CMargin(1,1,1,1);
	wAlign=AlignLeft;
	tickStyle=AllInRight;
	x=0;
	y=0;
	focus=false;	
	positive=0;	
	
	linecolor0 =Gemini::dark_gray_level0;
	linecolor1 =Gemini::white_level1;
	destcolor =Gemini::light_sky;
	colorAt=0;
	oldColorAt=0;
	btnIndex=-1;
	wGradientDirection=NoGradient;
	wGradientControl=GradientDestToSrc;
	wOldGradientControl=0xFF;
	wOldGradientDirection=0xFF;
	
	
	maxdb =true;
	mindb =true;
	precision  =0;
	curdecimal =0;
	ptaskCursor =NULL;
	
	nKeyIndex=0;
	lastacKey=0;
	nFontSize=16;
	symbols=false;
	dotsymbols=false;
	
	
	memset(szPrompt0,0,tmMAX_TEXT_COL * 2);
	
	hint=false;
	properchanged=false;
	coordchanged=false;
	oldEnabled=true;
	oldBgc=Gemini::white_level1;
}


/*static inline function */

static inline int CountDigits(int64 value,uint16 base)
{
	WORD        wDigit;
	int64       llValue;

	if (value >= 0)
    {
		wDigit = 1;
		llValue = value;
    }
	else
    {
		wDigit = 2;
		llValue = -value;
    }
	while ((llValue /= base) != 0) wDigit++;
	return wDigit;
}

static inline std::string ToString(CSpinBoxPrivate* d, int& maxlen, char* suffix)
{
	if(d)
	{
		int nMinLen=CountDigits(d->llMinValue,d->displayIntegerBase);
		int nMaxLen=CountDigits(d->llMaxValue,d->displayIntegerBase);
		maxlen=mMax(nMaxLen,nMinLen);
		maxlen+=suffix? strlen(suffix)+1:0;
		char szText[65];
		char* pszText=szText;
		if(d->llValue<0||d->symbols)
		{
			szText[0]='-';
			++pszText;
		}
		if(d->displayIntegerBase==HexaDecimal)
		{
			sprintf(pszText,"%.*llX%s",d->showBits,mAbs(d->llValue),suffix?suffix:"");
		}
		else if(d->displayIntegerBase==Octal)
		{
			sprintf(pszText,"%.*llo%s",d->showBits,mAbs(d->llValue),suffix?suffix:"");
		}
		else
		{
			sprintf(pszText,"%lld%s",mAbs(d->llValue),suffix?suffix:"");
		}
		return std::string(szText);
	}
	return std::string();
	
}

void CtmSpinBox::GetDBInfo(CSpinBoxPrivate* d)
{
	if(d)
	{
		if (dbid[0] != NULL)
		{
			DBVIEWVALUE dbViewValue; 
			dbViewValue = GetDBValue(dbid[0]);
			//d->precision=dbViewValue.wwPrecision;
			int muti=GetDecimalPowerVal(d->precision);
			d->llValue = dbViewValue.lValue;
			d->dValue = (double)dbViewValue.lValue/muti;  
			
			
			if (d->maxdb)
			{
				d->llMaxValue = dbViewValue.lMaxValue;
				d->dMaxValue  = (double)dbViewValue.lMaxValue/muti;
			}
			else
			{
				if (maxdbid != NULL)
				{
					d->llMaxValue = GetDBValue(maxdbid).lValue;
					d->dMaxValue =(double)d->llMaxValue/muti;
				}
			}
			
			if (d->mindb)
			{
				d->llMinValue = dbViewValue.lMinValue;
				d->dMinValue = (double)dbViewValue.lMinValue/muti;
			}
			else
			{
				if (mindbid != NULL)
				{
					d->llMinValue = GetDBValue(mindbid).lValue;
					d->dMinValue = (double)d->llMinValue/muti;
				}
			}
		}
	}
}

static inline void Reset(CSpinBoxPrivate* d)
{
	d->symbols=false;
	d->dotsymbols=false;
	d->llReInputValue=0;
	d->dReInputValue=0;
	d->curdecimal=0;
	d->integerpart=0;
	d->decimalpart=0;
	d->hint=false;
	d->properchanged=true;
}

static inline void Tick(CSpinBoxPrivate* d, bool valuences)
{
	if(d)
	{
		
		if(valuences)
		{
			d->llValue+=d->tickInterval;
			d->llValue=mMin(d->llValue,d->llMaxValue);
			
			d->dValue+=d->dTickInterval;
			d->dValue=mMin(d->dValue,d->dMaxValue);
		}
		else
		{
			d->llValue-=d->tickInterval;
			d->llValue=mMax(d->llValue,d->llMinValue);
			
			d->dValue-=d->dTickInterval;
			d->dValue=mMax(d->dValue,d->dMinValue);
		}
		Reset(d);
			
	}
}

static inline void PaintSpinBoxBackground(CSpinBoxPrivate* d, BGC bgc)
{
	if(d)
	{
		CPainter painter(&d->fmImage);
		CRect rect=CRect(CPoint(0,0),d->size);
		rect-=CMargin(2,2,2,2);
		if(!d->transparent)
		{
			CPen pen(Gemini::NoPen);
			painter.setPen(pen);
			if(d->wGradientDirection!=NoGradient)
			{
				CLinearGradient linear(rect.topLeft(),rect.bottomRight());
				if(d->wGradientDirection==XGradient)
				{
					linear.setStart(rect.topLeft());
					linear.setFinal(rect.topRight());
					
				}
				else if(d->wGradientDirection==YGradient)
				{
					linear.setStart(rect.topLeft());
					linear.setFinal(rect.bottomLeft());
				}
				else
				{
					;
				}
				
				if(d->wGradientControl==GradientSrcToDest)
				{
					linear.setColorAt(0,bgc);
					linear.setColorAt(1,d->destcolor);
				}
				else if(d->wGradientControl==GradientSrcToDestToSrc)
				{
					linear.setColorAt(0,bgc);
					linear.setColorAt(d->colorAt,d->destcolor);
					linear.setColorAt(1,bgc);
				}
				else if(d->wGradientControl==GradientDestToSrcToDest)
				{
					linear.setColorAt(0,d->destcolor);
					linear.setColorAt(d->colorAt,bgc);
					linear.setColorAt(1,d->destcolor);
				}
				else
				{
					linear.setColorAt(0,d->destcolor);
					linear.setColorAt(1,bgc);
				}
				
				CBrush brush(linear);
				painter.setBrush(brush);
			}
			else
			{
				CBrush brush(bgc);
				painter.setBrush(brush);	
			}
			if(d->wFrameShape==RoundedPanel)
			{
				painter.drawRoundedRect(rect,3,3);
			}
			
			else
			{
				painter.drawRect(rect);
			}
				
		}
	}
	
}

static inline void DrawSpinBoxBorder(CSpinBoxPrivate* d)
{
	if(d)
	{
		CPainter painter(&d->fmImage);
		CRect rect=CRect(CPoint(0,0),d->size);
		rect-=CMargin(2,2,2,2);

		CBrush brush(Gemini::NoBrush);
		painter.setBrush(brush);
		if(Panel==d->wFrameShape)
		{
			CLine lines[4];
			lines[0]=CLine(rect.topLeft(),rect.bottomLeft());
			lines[1]=CLine(rect.topLeft(),rect.topRight());
			lines[2]=CLine(rect.bottomLeft(),rect.bottomRight());
			lines[3]=CLine(rect.bottomRight(),rect.topRight());
			if(Sunken==d->wFrameShadow)
			{

				CPen pen;
				pen.setColor(d->linecolor0);
				painter.setPen(pen);
				painter.drawLines(lines,2);
				pen.setColor(d->linecolor1);
				painter.setPen(pen);
				painter.drawLines(lines+2,2);
			}
			else if(Raised==d->wFrameShadow)
			{

				CPen pen;
				pen.setColor(d->linecolor1);
				painter.setPen(pen);
				painter.drawLines(lines,2);
				pen.setColor(d->linecolor0);
				painter.setPen(pen);
				painter.drawLines(lines+2,2);
			}
			else
			{
				CPen pen(d->linecolor0);
				painter.setPen(pen);
				painter.drawRect(rect);
			}
		}
		else if(Box==d->wFrameShape)
		{
			if(Sunken==d->wFrameShadow)
			{
					CPen pen;
					pen.setWidth(1);
					pen.setColor(d->linecolor1);
					painter.setPen(pen);
					painter.drawRect(rect);
					pen.setWidth(1);
					pen.setColor(d->linecolor0);
					painter.setPen(pen);
					rect-=CMargin(-1,-1,1,1);
					painter.drawRect(rect);
			}
			else if(Raised==d->wFrameShadow)
			{
					CPen pen;
					pen.setWidth(1);
					pen.setColor(d->linecolor0);
					painter.setPen(pen);
					painter.drawRect(rect);
					pen.setWidth(1);
					pen.setColor(d->linecolor1);
					painter.setPen(pen);
					rect-=CMargin(-1,-1,1,1);
					painter.drawRect(rect);
			}
			else
			{
				CPen pen(d->linecolor0);
				painter.setPen(pen);
				painter.drawRect(rect);
			}
		}
		else if(RoundedPanel==d->wFrameShape)
		{
			painter.setRenderHints(CPainter::Antialiasing,true);
			CPen pen(d->linecolor0);
			painter.setPen(pen);
			painter.drawRoundedRect(rect,3,3);
		}
		else if(UnderLine==d->wFrameShape)
		{
			CPen pen(d->linecolor0);
			pen.setWidth(2);
			painter.setPen(pen);
			rect+=CMargin(0,0,0,1);
			painter.drawLine(CLine(rect.bottomLeft(),rect.bottomRight()));
		}
		else
		{
			;
		}
			
	}
	
}

static inline void CalcBtnRect(CSpinBoxPrivate* d)
{
	if(d)
	{
		CRect rect=CRect(CPoint(0,0),d->size);
		rect-=CMargin(4,4,4,4);
		if(d->tickStyle==BothSides)
		{
			if(rect.width()>44)
			{
				d->btnRect[0]=CRect(rect.topLeft(),CSize(22,rect.height()));
				d->btnRect[1]=d->btnRect[0].translated(rect.width()-22,0);
			}
		}
		else
		{
			if(rect.width()>22)
			{
				CRect oRect=CRect(rect.topLeft(),CSize(22,rect.height()>>1));
				d->btnRect[1]=oRect.translated(rect.width()-22,0);
				d->btnRect[0]=d->btnRect[1].translated(0,(rect.height()>>1)+1);
			}
		}
	}
}

static inline void DrawPressDown(CSpinBoxPrivate* d)
{
	if(d)
	{
		CRect rect=CRect(CPoint(0,0),d->size);
		rect-=CMargin(2,2,2,2);
		if(d->tickStyle==BothSides)
		{
			if(rect.width()<=48)
				return ;
		}
		else
		{
			if(rect.width()<=24)
				return ;
		}
		
		
		CPainter painter(&d->image);
		painter.setRenderHints(CPainter::Antialiasing,true);
		painter.setPen(CPen(Gemini::NoPen));
		if(d->btnIndex>=0)
		{
			int index=d->btnIndex%2;
			CLinearGradient linear(d->btnRect[index].topLeft(),d->btnRect[index].bottomLeft());
			linear.setColorAt(0,Gemini::gray_level0);
			linear.setColorAt(1,Gemini::white_level1);
			CBrush brush(linear);
			painter.setBrush(brush);
			painter.drawRoundedRect(d->btnRect[index],2,2);
			CLine lines[2];
			CPen pen(Gemini::dark_gray_level2);
			pen.setWidth(3);
			painter.setPen(pen);
			CRect symrect=CRect(0,0,9,9);
			symrect.translate(d->btnRect[index].center()-symrect.center());
			CPoint center=symrect.center();
			lines[0]=CLine(CPoint(symrect.left(),center.y()),CPoint(symrect.right(),center.y()));
			lines[1]=CLine(CPoint(center.x(),symrect.top()),CPoint(center.x(),symrect.bottom()));
			if(index==0)
			{
				painter.drawLines(lines,1);
			}
			else
			{
				painter.drawLines(lines,2);
			}
		}
	}
	
}

static inline void DrawPressUp(CSpinBoxPrivate* d)
{
	if(d)
	{
		CRect rect=CRect(CPoint(0,0),d->size);
		rect-=CMargin(2,2,2,2);
		if(d->tickStyle==BothSides)
		{
			if(rect.width()<=48)
				return ;
		}
		else
		{
			if(rect.width()<=24)
				return ;
		}
		
		CPainter painter(&d->image);
		painter.setRenderHints(CPainter::Antialiasing,true);
		painter.setPen(CPen(Gemini::NoPen));
		
		if(d->btnIndex==-1)
		{

			for(int i=0; i<2; ++i)
			{
				CLinearGradient linear(d->btnRect[i].topLeft(),d->btnRect[i].bottomLeft());
				linear.setColorAt(0,Gemini::white_level1);
				linear.setColorAt(0.8,Gemini::light_gray_level1);
				CBrush brush(linear);
				painter.setBrush(brush);
				painter.drawRoundedRect(d->btnRect[i],2,2);
			}
			CLine lines[3];
			CPen pen(Gemini::dark_gray_level2);
			pen.setWidth(3);
			painter.setPen(pen);
			CRect rect=CRect(0,0,9,9);
			CRect downRect=rect.translated(d->btnRect[0].center()-rect.center());
			CRect upRect=rect.translated(d->btnRect[1].center()-rect.center());
			CPoint downcenter=downRect.center();
			CPoint upcenter=upRect.center();
			lines[0]=CLine(CPoint(downRect.left(),downcenter.y()),CPoint(downRect.right(),downcenter.y()));
			lines[1]=CLine(CPoint(upRect.left(),upcenter.y()),CPoint(upRect.right(),upcenter.y()));
			lines[2]=CLine(CPoint(upcenter.x(),upRect.top()),CPoint(upcenter.x(),upRect.bottom()));
			painter.drawLines(lines,3);
		}
		else
		{
			
			int index=1-(d->btnIndex%2);
			CLinearGradient linear(d->btnRect[index].topLeft(),d->btnRect[index].bottomLeft());
			linear.setColorAt(0,Gemini::white_level1);
			linear.setColorAt(0.8,Gemini::light_gray_level1);
			CBrush brush(linear);
			painter.setBrush(brush);
			painter.drawRoundedRect(d->btnRect[index],2,2);
			CLine lines[2];
			CPen pen(Gemini::dark_gray_level2);
			pen.setWidth(3);
			painter.setPen(pen);
			CRect rect=CRect(0,0,9,9);
			rect.translate(d->btnRect[index].center()-rect.center());
			CPoint center=rect.center();
			lines[0]=CLine(CPoint(rect.left(),center.y()),CPoint(rect.right(),center.y()));
			lines[1]=CLine(CPoint(center.x(),rect.top()),CPoint(center.x(),rect.bottom()));
			if(index==0)
			{
				painter.drawLines(lines,1);
			}
			else
			{
				painter.drawLines(lines,2);
				
			}
		}
	}
	
}

static inline void DrawSpinBoxText(CSpinBoxPrivate* d, FGC fgc, char* suffix)
{
	if(d)
	{
		CRect rect=CRect(CPoint(0,0),d->size);
		
		
		if(d->hiddenTick)
		{
			rect-=CMargin(2,2,2,2);
			d->ftRect=rect;
		}
		else
		{
			
			rect-=CMargin(2,2,2,2);
			//获取正文段矩形框的大小
			if(d->tickStyle==BothSides)
			{
				d->ftRect=rect-CMargin(d->btnRect[0].width(),0,d->btnRect[1].width(),0);
			}
			else
			{
				d->ftRect=rect-CMargin(0,0,d->btnRect[0].width(),0);
			}	
		}
		
		
		//绘制正文段
		if(g_pDrawText)
		{
			int nMaxLen=0;
			string text=ToString(d,nMaxLen,suffix);
			int fontWidth=((d->ftRect.width()<<1)/nMaxLen);
			d->nFontSize=mMin(d->ftRect.height(),fontWidth);
			if(AlignTop==(d->wAlign&AlignTop))			//	top
			{
				d->ftRect-=CMargin(0,0,0,d->ftRect.height()-d->nFontSize);
			}
			else if(AlignBottom==(d->wAlign&AlignBottom)) //bottom
			{
				d->ftRect-=CMargin(0,d->ftRect.height()-d->nFontSize,0,0);
			}
			else 										//center
			{
				int half=(d->ftRect.height()-d->nFontSize)>>1;
				d->ftRect-=CMargin(0,half,0,half);
			}
			int oldFontWidth=g_pDrawText->GetFontWidth();
			int oldFontHeight=g_pDrawText->GetFontHeight();
			int oldAlign=g_pDrawText->GetFontAlign();
			int oldFontColor=g_pDrawText->GetFontColor();
			g_pDrawText->SetFontSize(d->nFontSize,d->nFontSize,d->wAlign);
			g_pDrawText->SetFontColor(fgc);
			g_pDrawText->DrawText(d->image,(uint8*)text.c_str(),d->ftRect,text.size());
			g_pDrawText->SetFontSize(oldFontWidth,oldFontHeight,oldAlign);
			g_pDrawText->SetFontColor(oldFontColor);
			
		}
		// CPainter painter(&d->image);
		// painter.drawRect(d->ftRect);
		
		
		
		
	}
	
}

static inline int64 GetReInputValue(CSpinBoxPrivate* d)
{
	
	if(d)
	{
		int negative=1;
		int64 value=mAbs(d->llReInputValue);
		
		if(d->llMaxValue<d->llMinValue)
		{
			return 0;
		}
		if(d->llReInputValue<0)
		{
			negative=-1;
		}
		if(d->llMaxValue<=0)
		{
			if(d->llReInputValue<d->llMinValue
			||d->llReInputValue>0)
			{
				int a=value/d->displayIntegerBase;
				int b=value%d->displayIntegerBase;
				value=a/d->displayIntegerBase*d->displayIntegerBase+b;
				
			}
		}
		else if(d->llMinValue>=0)
		{
			if(d->llReInputValue>d->llMaxValue
			||d->llReInputValue<0)
			{
				int a=value/d->displayIntegerBase;
				int b=value%d->displayIntegerBase;
				value=a/d->displayIntegerBase*d->displayIntegerBase+b;
			}
		}
		else 
		{
			if(d->llReInputValue>0)
			{
				if(d->llReInputValue>d->llMaxValue)
				{
					int a=value/d->displayIntegerBase;
					int b=value%d->displayIntegerBase;
					value=a/d->displayIntegerBase*d->displayIntegerBase+b;
				}
			}
			else if(d->llReInputValue<0)
			{
				if(d->llReInputValue<d->llMinValue)
				{
					int a=value/d->displayIntegerBase;
					int b=value%d->displayIntegerBase;
					value=a/d->displayIntegerBase*d->displayIntegerBase+b;
				}
			}
			
		}
		return value*negative;
	}
	return 0;
}


/*class function */

IMPLEMENT_DYNCREATE(CtmSpinBox, CtmWnd)

CtmSpinBox::CtmSpinBox():CtmWnd()
{
	strcpy(TYPEName, "CtmSpinBox");
	d=new CSpinBoxPrivate();
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);
	}
	
	pszPrompt=NULL;
	pszPromptLan=NULL;
	pszPromptID=NULL;
	
	for(int i=0; i<SPINBOX_MAXDBNUM;++i)
	{
		dbid[i]=NULL;
	}
	
	maxdbid =NULL;
	mindbid =NULL;
	
	/*添加后缀信息*/
	suffix=NULL;
}

CtmSpinBox::~CtmSpinBox()
{
	delete d;
}


void	CtmSpinBox::Show()
{
	if(IsTransparentChanged(d)||IsNeedResize(d))
	{
		if(IsNeedResize(d))
		{
			d->coordchanged=true;
			Resize(d);
			
		}

		if(!d->bgImage.isNull())
			d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		else
			d->fmImage.fill((uint)m_bgc);
		
		d->properchanged=true;
		/* if(IsEnabledChanged(d))
		{
			d->oldEnabled=m_bEnabled;
			if(!m_bEnabled)
			{
				d->oldBgc=m_bgc;
				m_bgc=Gemini::dark_gray_level1;
			}
		} */
		if(m_bVisible)
		{
			//if(BackGroundChanged(d))
			{
					PaintSpinBoxBackground(d, m_bgc);
			}
			//if(ShapeChanged(d))
			{
					DrawSpinBoxBorder(d);
			}
		}
		/* if(!m_bEnabled)
		{
			m_bgc=d->oldBgc;
		} */
	}
	
	if(m_bVisible)
	{
		if(d->focus)
		{
				d->properchanged=true;
		}
		if(d->properchanged)
		{
			Clear();
			if(!d->hiddenTick)
			{
						
				CalcBtnRect(d);
				DrawPressUp(d);
				DrawPressDown(d);
			}
			if(!d->focus)
			{
				GetDBInfo(d);
			}
			DrawSpinBoxText(d,m_fgc,suffix);
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
			d->properchanged=false;
		}
	}
	else
	{
		_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
		_PutImage(0,0,d->bgImage);	
	}

}
void	CtmSpinBox::Update()
{
	if(!d->focus)
	{
		GetDBInfo(d);
		if(d->llValue!=d->llLastValue)
		{
			d->llLastValue=d->llValue;
			d->properchanged=true;
		}
	}
	Show();
}

void	CtmSpinBox::Clear()
{
	if(d)
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));

}

BOOL	CtmSpinBox::CreateA()
{
	m_wnd.wStyle=0x27;

	CtmWnd::CreateA();
	
}

void	CtmSpinBox::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"align", 		tmWORD, 	&d->wAlign);
		SetPropInfo((char*)"value",			tmINT64,	&d->llValue);
		SetPropInfo((char*)"max",			tmINT64,	&d->llMaxValue);
		SetPropInfo((char*)"min",			tmINT64,	&d->llMinValue);
		SetPropInfo((char*)"tick-interval", tmINT,		&d->tickInterval);
		SetPropInfo((char*)"integer-base",	tmWORD,		&d->displayIntegerBase);
		SetPropInfo((char*)"hidden-tick",	tmBOOL,		&d->hiddenTick);
		SetPropInfo((char*)"target-color",	tmDWORD,	&d->destcolor);
		SetPropInfo((char*)"colorAt",		tmFLOAT,	&d->colorAt);
		SetPropInfo((char*)"border-color", 	tmDWORD,	&d->linecolor0);
		SetPropInfo((char*)"tick-style",	tmWORD,		&d->tickStyle);
		SetPropInfo((char*)"frame-shadow",	tmWORD,		&d->wFrameShadow);
		SetPropInfo((char*)"frame-shape",	tmWORD,		&d->wFrameShape);
		SetPropInfo((char*)"suffix",		tmSTR,		&suffix);
		SetPropInfo((char*)"transparent",	tmBOOL,		&d->transparent);
		SetPropInfo((char*)"touched",		tmBOOL,		&d->touched);
		SetPropInfo((char*)"private-changed", tmBOOL, 	&d->properchanged);
		SetPropInfo((char*)"show-bits",		tmWORD,		&d->showBits);
		
		char sz[10];
		for(int i=0; i<SPINBOX_MAXDBNUM; ++i)
		{
			sprintf(sz,"dbid%i",i);
			SetPropInfo(sz, tmSTR, &dbid[i]);
		}
		
		SetPropInfo((char*)"maxid",	tmSTR, &maxdbid);
		SetPropInfo((char*)"minid", tmSTR, &mindbid);
		SetPropInfo((char*)"maxdb", tmBOOL,&d->maxdb);
		SetPropInfo((char*)"mindb", tmBOOL,&d->mindb);
		
		SetPropInfo((char*)"prompt", tmSTR,&pszPromptLan);
		SetPropInfo((char*)"promptID",tmSTR,&pszPromptID);
	}
}



WORD	CtmSpinBox::OnKey(WORD wKey)
{
	WORD			wResult=wKey;
	if((wKey&0xFF00)==0x9400)	return  _NULL_KEY;
	if(!d)			return wKey;
//	if(!d->focus)	return wKey;
	switch (wKey)
	{
		case 0x6D00: 	//down
		case _KEY_DOWNFIELD:
			{
				Tick(d,0);
				Update();
				wResult=_NULL_KEY;
			}
			break;
		case 0x6800:	//up
		case _KEY_UPFIELD:
			{
				Tick(d,1);
				Update();
				wResult=_NULL_KEY;
			}
			break;
		case _KEY_LEFTFIELD:
		case _KEY_RIGHTFIELD:
		case _UARROW:
		case _EXT_UARROW:
		case _DARROW:
		case _EXT_DARROW:
		case _TAB:
		case _SHIFT_TAB:
		case _ENTER:
		case (WORD)_EXT_ENTER:
			{
				d->hint=false;
				if(IsOverLimited(d))
				{
					Reset(d);
					SetCursorPosition(0);
					wResult=_NULL_KEY;
				}
				Update();
			}
			
			break;
		case _KEY_CLEAR:
			{
				d->hint=false;
				Reset(d);
				SetCursorPosition(0);
				wResult=_NULL_KEY;
				Update();
			}
			break;
		case _BKSP:
			{
				BackSpace(d);
				wResult=_NULL_KEY;
				Show();	
			}
			break;
		default:
			ProcessOtherKey(wKey);
			break;	
	}
	return wResult;
}

void	CtmSpinBox::OnGetFocus()
{
	if(d)
	{
		GetDBInfo(d);
		Update();
		d->llLastValue=d->llValue;
		d->dLastValue=d->dValue;
		Reset(d);
		d->focus = true;
		EnableCursor(TRUE);
		SetCursorPosition(0);
		DisplayCursor(TRUE);
		CtmWnd::DrawGetFocus();
		Prepare(d);
		Prompt(d->szPrompt0);
	}
}

void	CtmSpinBox::OnLoseFocus()
{
	if(d)
	{
			d->focus = false;
			CtmWnd::DrawLoseFocus();
			if(!g_bFocusChanged)
			{
				SyncDBValue(d);
			}
			EnableCursor(FALSE);
			d->properchanged=true;
			Show();
	}
	
}

void	CtmSpinBox::WndProc(int message,WPARAM wParam, LPARAM lParam)
{
	if(!d->touched) return ;
	int x=wParam>>16;
	int y=wParam&0x0000FFFF;
	switch(message)
	{
		case MSG_MOUSELUP:
			{
				int btnIndex=TouchAreaChanged(d,CPoint(x,y));
				if(btnIndex==d->btnIndex&&btnIndex>=0)
				{
					Tick(d,btnIndex);
					d->btnIndex=-1;
					Update();
				}
			}
			break;
		case MSG_MOUSELDOWN:
				d->btnIndex=TouchAreaChanged(d,CPoint(x,y));
				if(d->btnIndex>=0)
				{
					d->properchanged=true;
					Update();
				}
			break;
		default:
			break;
	}
}



int CtmSpinBox::TouchAreaChanged(CSpinBoxPrivate* d, const CPoint& pt)
{
	int btnIdx=-1;
	if(d)
	{
		CPoint cmp=pt+CPoint(-m_rectClient.left,-m_rectClient.top);
		if(d->btnRect[0].contains(cmp))
		{
			btnIdx=0;
		}
		else if(d->btnRect[1].contains(cmp))
		{
			btnIdx=1;
		}
		/*  else if(CRect(CPoint(0,0),d->size).contains(cmp))
		{
			OnGetFocus();
			::InputBox(this);
			OnLoseFocus();
		}  */
	}
	return btnIdx;	
	
}

bool CtmSpinBox::InputBoxContains(CPoint& pt)
{
	CPoint cmp=pt+CPoint(-m_rectClient.left,-m_rectClient.top);
	if(d->btnRect[0].contains(cmp)||d->btnRect[1].contains(cmp))
	{
		return false;
	}
	else if(CRect(CPoint(0,0),d->size).contains(cmp))
	{
		return true;
	}
	return false;
}

bool CtmSpinBox::ShapeChanged(CSpinBoxPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->wFrameShape!=d->wOldFrameShape||d->wFrameShadow!=d->wOldFrameShadow
		||d->oldLineColor0!=d->linecolor0)
		{
			d->wOldFrameShape=d->wFrameShape;
			d->wOldFrameShadow=d->wFrameShadow;
			d->oldLineColor0=d->linecolor0;
			res=true;
			
		}
		else if(d->coordchanged)
		{
			res=true;
		}
		
	}
	return res;
}


bool CtmSpinBox::BackGroundChanged(CSpinBoxPrivate* d)
{
  bool res=false;
  if(d)
  {
	  
	  d->wGradientDirection=m_nBgcColorControl&0x700;
	  d->wGradientControl=m_nBgcColorControl&0x1D;
	  
	  if(d->wGradientControl!=d->wOldGradientControl
		||d->wGradientDirection!=d->wOldGradientDirection
		||d->colorAt!=d->oldColorAt)
	  {
		 
		d->wOldGradientControl=d->wGradientControl;
		d->wOldGradientDirection=d->wGradientDirection;
		d->oldColorAt=d->colorAt;
		res=true;
	  }
	  else if(d->oldBgc!=m_bgc)
	  {
		 d->oldBgc=m_bgc;
		 res=true;
	  }
	  else if(d->coordchanged)
	  {
		  res=true;
	  }
  }
  return res;
}

bool CtmSpinBox::IsTransparentChanged(CSpinBoxPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->transparent!=d->oldTransparent)
		{
			d->wOldGradientControl=0xFF;
			d->wOldGradientDirection=0xFF;
			d->oldColorAt=0;
			d->oldTransparent=d->transparent;	
			res=true;
		}
	}	
	return res;
}

bool CtmSpinBox::IsNeedResize(CSpinBoxPrivate* d)
{
	if(d)
		return m_rectClient.left!=d->x||m_rectClient.top!=d->y||CSize(GetClientWidth(),GetClientHeight())!=d->size;
	else
		return false;	
}

void CtmSpinBox::Resize(CSpinBoxPrivate* d)
{
	if(d)
	{
		if(d->transparent)
		{
			//透明色,如果buffer中存在背景怎将其贴到原位置
			/* if(!d->bgImage.isNull())
			{
				_SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());	
				_PutImage(0,0,d->bgImage);
			} */
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
		
void  CtmSpinBox::EnableCursor(BOOL bEnable)
{
	if(d)
	{
		TASKCURSOR  taskcursor;
		if (bEnable)
		{
			if (d->ptaskCursor == NULL)
			{
				taskcursor.pwnd = this;
				d->ptaskCursor = new CTaskCursor(NULL, &taskcursor);
			}
		}
		else
		{
			if (d->ptaskCursor != NULL)
			{
				DisplayCursor(FALSE);
				delete d->ptaskCursor;
				d->ptaskCursor = NULL;
			}
		}
	}
}
void  CtmSpinBox::DisplayCursor(BOOL bOn)
{
	if(d)
	{
		if (d->ptaskCursor != NULL)
			d->ptaskCursor->DisplayCursor(bOn);
	}
}

BOOL  CtmSpinBox::GetWaitStatus()
{
	if(d)
	{
		if (d->ptaskCursor != NULL)
			return d->ptaskCursor->IsWaiting();
		else
			return FALSE;
	}
	return FALSE;

}

void  CtmSpinBox::CursorWait(BOOL bWait)
{
	if(d)
	{
		if (d->ptaskCursor != NULL)
			d->ptaskCursor->SetWaitStatus(bWait);
	}
}

bool  CtmSpinBox::ProcessOtherKey(WORD wKey)
{
	char        acKey[2];
	char		baseAscii='0';
	
	if (tmHIBYTE(wKey) == 0)
	{
		DisplayCursor(FALSE); 
		acKey[0] = tmLOBYTE(wKey);
	    acKey[1] = '\0';
		if ((acKey[0] >= '0') && (acKey[0] <= '9'))
		{
			if(d->displayIntegerBase==HexaDecimal)
			{
				if(acKey[0]=='7'||acKey[0]=='8')
				{
					if(GetWaitStatus()&&(acKey[0] == d->lastacKey))
					{
						d->llReInputValue/=d->displayIntegerBase;
						baseAscii=('A'-0xA);
						acKey[0]=ninePad[acKey[0]-'0'][++d->nKeyIndex];
						acKey[1]=acKey[0];
					}
					else
					{
						acKey[1]=acKey[0];
						d->lastacKey=acKey[0];
						d->nKeyIndex=0;
						CursorWait(TRUE);
					}
				}
				else
				{
					acKey[1]=acKey[0];
					CursorWait(FALSE);
				}
			}
			else
			{
				acKey[1]=acKey[0];
				CursorWait(FALSE);
			}
			
			d->llReInputValue=mAbs(d->llReInputValue)*d->displayIntegerBase+acKey[1]-baseAscii;
			if(d->symbols)
			{
				d->llReInputValue=-d->llReInputValue;
			}
			d->llReInputValue=GetReInputValue(d);
			d->llValue=d->llReInputValue;
			
		}
		else if((acKey[0]>='a')&&(acKey[0]<='f')||((acKey[0]>='A')&&(acKey[0]<='F')))
		{
			if(d->displayIntegerBase==HexaDecimal)
			{
				if(acKey[0]>='a')
				{
					baseAscii=('a'-0xA);
				}
				else
				{
					baseAscii=('A'-0xA);
				}
				acKey[1]=acKey[0];
				CursorWait(FALSE);
				d->llReInputValue=mAbs(d->llReInputValue)*d->displayIntegerBase+acKey[1]-baseAscii;
				if(d->symbols)
				{
					d->llReInputValue=-d->llReInputValue;
				}
				d->llReInputValue=GetReInputValue(d);
				d->llValue=d->llReInputValue;
			}
		}
		else if(acKey[0]=='-')
		{
			if(d->llReInputValue==0)
			{
				d->symbols=true;
			}
			CursorWait(FALSE);
		}
		Show();
		if(acKey[1])
			SetCursorPosition(acKey[1]);
		DisplayCursor(TRUE);
	}
	
	
}

void  CtmSpinBox::SetCursorPosition(char acKey)
{
	if(d->ptaskCursor!=NULL)
	{
		int maxlen;
		std::string str=ToString(d,maxlen,suffix);
		int offset_bytes=suffix?strlen(suffix)+1:0;
		SIZE size={d->nFontSize,d->nFontSize};
		d->ptaskCursor->SetCursorPosition((char*)str.c_str(),str.size()+1,acKey,size,d->ftRect,d->wAlign,offset_bytes);
	}
}

void  CtmSpinBox::ResetPrompt(CSpinBoxPrivate* d)
{
	if(d)
	{
		if(pszPromptID)
			pszPrompt=g_MultiLanguage[pszPromptID];
		else
			pszPrompt=pszPromptLan;	
	}
}

void  CtmSpinBox::Prepare(CSpinBoxPrivate* d)
{
	if(d)
	{
		ResetPrompt(d);
		char        szMin[256];
		char        szMax[256];
		long 		lDataID = 0;
		char*		pszMin=szMin;
		char* 		pszMax=szMax;
		if(d->llMinValue<0)
		{
			pszMin[0]='-';
			++pszMin;
		}
		if(d->llMaxValue<0)
		{
			pszMax[0]='-';
			++pszMax;
		}
		if (d->displayIntegerBase==HexaDecimal)
		{
			sprintf(pszMin, "%llX", mAbs(d->llMinValue));
			sprintf(pszMax, "%llX", mAbs(d->llMaxValue));
			A2A(szMin, 0, m_pcFont->GetFontCode());
			A2A(szMax, 0, m_pcFont->GetFontCode());
		}
		else if(d->displayIntegerBase==Octal)
		{
			sprintf(pszMin, "%llo", mAbs(d->llMinValue));
			sprintf(pszMax, "%llo", mAbs(d->llMaxValue));
			A2A(szMin, 0, m_pcFont->GetFontCode());
			A2A(szMax, 0, m_pcFont->GetFontCode());
		}
		else
		{
			LL2A(szMin, 0, d->llMinValue, m_pcFont->GetFontCode());    //netdigger 2007/4/12
			LL2A(szMax, 0, d->llMaxValue, m_pcFont->GetFontCode());    //netdigger 2007/4/12	
		}
		
		if(g_bDemoDataID)
		{
			if(dbid[0] != NULL)
			{
				lDataID  = g_pDatabase->GetDataID(dbid[0]);
			}
		}
		if(CtmConfig::GetInstance()->GetFontConvert())
		{
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%x %t: %t %t: %t %t", lDataID, 
								g_MultiLanguage["MSG_MIN"], szMin,
								g_MultiLanguage["MSG_MAX"], szMax,pszPrompt);        
				}
				else
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
				
					tm_sprintf(d->szPrompt0, "%t: %t %t: %t %t", 
									g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax,pszPrompt);
				}
			}
			else
			{	
				if (pszPrompt != NULL)
				{
					if ((strlen(pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
					strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0, "%s:%s %s:%s %s",g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax, pszPrompt);
					}
				}
				else sprintf(d->szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
			}	
		}
		else
		{
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%x %t %t: %t %t: %t", lDataID, 
							pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
							g_MultiLanguage["MSG_MAX"], szMax);        
				}
				else
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%t %t: %t %t: %t", 
									pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax);
				}
			}
			else
			{	
				if (pszPrompt != NULL)
				{
					if ((strlen(pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
					strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0, "%s %s:%s %s:%s", pszPrompt, g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
					}
				}
				else sprintf(d->szPrompt0, "%s:%s %s:%s", g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
			}
		}
	}
	
}

void  CtmSpinBox::SyncDBValue(CSpinBoxPrivate* d)
{
	if(d)
	{
		if(d->llLastValue!=d->llValue)
		{
			if(!IsOverLimited(d))
			{
				if(dbid[0]!=NULL)
				{
					SetDBValue(dbid[0],d->llValue);
				}
			}
		}
	}

}

bool  CtmSpinBox::IsOverLimited(CSpinBoxPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->llValue<d->llMinValue||d->llValue>d->llMaxValue)
		{
			res=true;
		}
	}
	return res;
	
}

bool CtmSpinBox::IsEnabledChanged(CSpinBoxPrivate* d)
{
	
	if(d->oldEnabled!=m_bEnabled)
		return true;
	return false;
}

WORD	CtmSpinBox::GetDisplayBase()
{
	if(d)
	{
		return d->displayIntegerBase;
	}
	return 0;
}



bool  CtmSpinBox::BackSpace(CSpinBoxPrivate* d)
{
	bool res=false;
	if(d)
	{
		d->llValue=
		d->llReInputValue/=d->displayIntegerBase;
		res=true;
		if(d->llValue==0)
		{
			Reset(d);
		}
	}
	return res;
}


/*Created by J.Wong 2017/02/23*/


/* static inline fuctions */

static inline std::string ToDoubleString(CSpinBoxPrivate* d, int& maxlen, char* suffix)
{
	if(d)
	{
		int nMinLen=CountDigits(d->dMinValue,d->displayIntegerBase);
		int nMaxLen=CountDigits(d->dMaxValue,d->displayIntegerBase);
		maxlen=mMax(nMaxLen,nMinLen);
		maxlen+=suffix? strlen(suffix)+1:0;
		if(d->precision)
		{
			maxlen+=d->precision+1;
		}
		char szText[65];
		char* pszText=szText;
		if(d->dValue<0||d->symbols)
		{
			szText[0]='-';
			++pszText;
		}
		if(d->hint)
		{
			if(d->dotsymbols)
			{
				if(d->curdecimal)
				{
					sprintf(pszText,"%.*lf%s",d->curdecimal,mAbs(d->dValue),suffix?suffix:"");
					
				}
				else
				{
					sprintf(pszText,"%d.%s",mAbs(d->integerpart),suffix?suffix:"");
				}
			}
			else
			{
				sprintf(pszText,"%d%s",mAbs(d->integerpart),suffix?suffix:"");
			}
		}
		else 
		{
			sprintf(pszText,"%.*lf%s",d->precision,mAbs(d->dValue),suffix?suffix:"");
		}
//		printf("text=%s,maxlen=%d\n",szText,maxlen);
		return std::string(szText);
	}
	return std::string();
	
}


static inline void DrawDoubleSpinBoxText(CSpinBoxPrivate* d, FGC fgc, char* suffix)
{
	if(d)
	{
		CRect rect=CRect(CPoint(0,0),d->size);
		
		
		if(d->hiddenTick)
		{
			rect-=CMargin(2,2,2,2);
			d->ftRect=rect;
		}
		else
		{
			
			rect-=CMargin(2,2,2,2);
			//获取正文段矩形框的大小
			if(d->tickStyle==BothSides)
			{
				d->ftRect=rect-CMargin(d->btnRect[0].width(),0,d->btnRect[1].width(),0);
			}
			else
			{
				d->ftRect=rect-CMargin(0,0,d->btnRect[0].width(),0);
			}	
		}
		
		
		//绘制正文段
		if(g_pDrawText)
		{
			int nMaxLen=0;
			string text=ToDoubleString(d,nMaxLen, suffix);
			int fontWidth=((d->ftRect.width()<<1)/nMaxLen);
			d->nFontSize=mMin(d->ftRect.height(),fontWidth);
			if(AlignTop==(d->wAlign&AlignTop))			//	top
			{
				d->ftRect-=CMargin(0,0,0,d->ftRect.height()-d->nFontSize);
			}
			else if(AlignBottom==(d->wAlign&AlignBottom)) //bottom
			{
				d->ftRect-=CMargin(0,d->ftRect.height()-d->nFontSize,0,0);
			}
			else 										//center
			{
				int half=(d->ftRect.height()-d->nFontSize)>>1;
				d->ftRect-=CMargin(0,half,0,half);
			}
			int oldFontWidth=g_pDrawText->GetFontWidth();
			int oldFontHeight=g_pDrawText->GetFontHeight();
			int oldAlign=g_pDrawText->GetFontAlign();
			int oldFontColor=g_pDrawText->GetFontColor();
			g_pDrawText->SetFontSize(d->nFontSize,d->nFontSize,d->wAlign);
			g_pDrawText->SetFontColor(fgc);
			g_pDrawText->DrawText(d->image,(uint8*)text.c_str(),d->ftRect,text.size());
			g_pDrawText->SetFontSize(oldFontWidth,oldFontHeight,oldAlign);
			g_pDrawText->SetFontColor(oldFontColor);
			
		}
		// CPainter painter(&d->image);
		// painter.drawRect(d->ftRect);
		
		
		
		
	}
	
}




static inline double GetDoubleReInputValue(CSpinBoxPrivate* d)
{
	if(d)
	{
		int negative=1;
		
	
		int value=0;
		if(d->curdecimal==0)
		{
			value=d->integerpart;
		}
		else
		{
			value=d->decimalpart;
		}
		
		if(d->dMaxValue<d->dMinValue)
		{
			return 0;
		}
		if(d->dReInputValue<0)
		{
			negative=-1;
		}
		if(d->dMaxValue<=0)
		{
			if(d->dReInputValue<d->dMinValue
			||d->dReInputValue>0)
			{
				int a=value/d->displayIntegerBase;
				int b=value%d->displayIntegerBase;
				value=a/d->displayIntegerBase*d->displayIntegerBase+b;
				if(d->curdecimal>1)
				{
					--d->curdecimal;
				}
			}
		}
		else if(d->dMinValue>=0)
		{
			if(d->dReInputValue>d->dMaxValue
			||d->dReInputValue<0)
			{
				int a=value/d->displayIntegerBase;
				int b=value%d->displayIntegerBase;
				value=a/d->displayIntegerBase*d->displayIntegerBase+b;
				if(d->curdecimal>1)
				{
					--d->curdecimal;
				}
			}
		}
		else 
		{
			if(d->dReInputValue>0)
			{
				if(d->dReInputValue>d->dMaxValue)
				{
					int a=value/d->displayIntegerBase;
					int b=value%d->displayIntegerBase;
					value=a/d->displayIntegerBase*d->displayIntegerBase+b;
					if(d->curdecimal>1)
					{
						--d->curdecimal;
					}
				}
			}
			else if(d->dReInputValue<0)
			{
				if(d->dReInputValue<d->dMinValue)
				{
					int a=value/d->displayIntegerBase;
					int b=value%d->displayIntegerBase;
					value=a/d->displayIntegerBase*d->displayIntegerBase+b;
					if(d->curdecimal>1)
					{
						--d->curdecimal;
					}
				}
			}
			
		}
//		printf("value=%ld\n",value);
		
		if(d->curdecimal==0)
			d->integerpart=value;
		else
		{
			d->decimalpart=value;
		}
//		printf("d->integerpart=%d,d->decimalpart=%d\n",d->integerpart,d->decimalpart);
//		printf("calc:%lf, depart:%lf\n",negative*((double)(d->integerpart)+(double)d->decimalpart/GetDecimalPowerVal(d->curdecimal)),((double)d->decimalpart/GetDecimalPowerVal(d->curdecimal)+d->integerpart)*negative);
		return negative*(d->integerpart+(double)d->decimalpart/GetDecimalPowerVal(d->curdecimal));
		
	}
	return 0;
	
	
}




/* class functions */

IMPLEMENT_DYNCREATE(CtmDoubleSpinBox, CtmSpinBox)

CtmDoubleSpinBox::CtmDoubleSpinBox():CtmSpinBox()
{
	strcpy(TYPEName, "CtmDoubleSpinBox");
}

CtmDoubleSpinBox::~CtmDoubleSpinBox()
{
	
}

void	CtmDoubleSpinBox::Show()
{
	if(IsTransparentChanged(d)||IsNeedResize(d))
	{
		if(IsNeedResize(d))
		{
			d->coordchanged=true;
			Resize(d);
		}	

		if(!d->bgImage.isNull())
			d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		else
			d->fmImage.fill((uint)m_bgc);
		d->properchanged=true;
		/* if(IsEnabledChanged(d))
		{
			d->oldEnabled=m_bEnabled;
			if(!m_bEnabled)
			{
				d->oldBgc=m_bgc;
				m_bgc=Gemini::dark_gray_level1;
			}
		} */
		if(m_bVisible)
		{
			//if(BackGroundChanged(d))
			{
					PaintSpinBoxBackground(d, m_bgc);
			}
			//if(ShapeChanged(d))
			{
					DrawSpinBoxBorder(d);
			}
		}
		/* if(!m_bEnabled)
		{
			m_bgc=d->oldBgc;
		} */
	}
	
	if(m_bVisible)
	{
		if(d->focus)
		{
			d->properchanged=true;
		}
		if(d->properchanged)
		{
			Clear();
			if(!d->hiddenTick)
			{
				
				CalcBtnRect(d);
				DrawPressUp(d);
				DrawPressDown(d);
			}
			if(!d->focus)
			{
				GetDBInfo(d);
			}
			DrawDoubleSpinBoxText(d,m_fgc, suffix);
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
			d->properchanged=false;
			d->coordchanged=false;
		}
	 }
	 else
	 {
	 	_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
	 	_PutImage(0,0,d->bgImage);	
	 }
}

void	CtmDoubleSpinBox::Update()
{
	if (!d->focus)
	{
		GetDBInfo(d);
		int64 llValue=mRound(d->dValue*GetDecimalPowerVal(d->precision));
		int64 llLastValue=mRound(d->dLastValue*GetDecimalPowerVal(d->precision));
		if(llValue!=llLastValue)
		{
			d->dLastValue=d->dValue;
			d->properchanged=true;
		}
	}
		Show();
}

void	CtmDoubleSpinBox::Clear()
{
	CtmSpinBox::Clear();
}

BOOL	CtmDoubleSpinBox::CreateA()
{
	if(d->precision<0||d->precision>6)
	{
		d->precision=0;
	}
	CtmSpinBox::CreateA();
}

void	CtmDoubleSpinBox::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"align", 		tmWORD, 	&d->wAlign);
		SetPropInfo((char*)"value",			tmDOUBLE,	&d->dValue);
		SetPropInfo((char*)"max",			tmDOUBLE,	&d->dMaxValue);
		SetPropInfo((char*)"min",			tmDOUBLE,	&d->dMinValue);
		SetPropInfo((char*)"tick-interval", tmDOUBLE,	&d->dTickInterval);
		SetPropInfo((char*)"hidden-tick",	tmBOOL,		&d->hiddenTick);
		SetPropInfo((char*)"target-color",	tmDWORD,	&d->destcolor);
		SetPropInfo((char*)"colorAt",		tmFLOAT,	&d->colorAt);
		SetPropInfo((char*)"border-color", 	tmDWORD,	&d->linecolor0);
		SetPropInfo((char*)"tick-style",	tmWORD,		&d->tickStyle);
		SetPropInfo((char*)"frame-shadow",	tmWORD,		&d->wFrameShadow);
		SetPropInfo((char*)"frame-shape",	tmWORD,		&d->wFrameShape);
		SetPropInfo((char*)"suffix",		tmSTR,		&suffix);
		SetPropInfo((char*)"transparent",	tmBOOL,		&d->transparent);
		SetPropInfo((char*)"touched",		tmBOOL,		&d->touched);
		SetPropInfo((char*)"precision",		tmWORD,		&d->precision);
		SetPropInfo((char*)"private-changed", tmBOOL, 	&d->properchanged);
		char sz[10];
		for(int i=0; i<SPINBOX_MAXDBNUM; ++i)
		{
			sprintf(sz,"dbid%i",i);
			SetPropInfo(sz, tmSTR, &dbid[i]);
		}
		
		SetPropInfo((char*)"maxid",	tmSTR, &maxdbid);
		SetPropInfo((char*)"minid", tmSTR, &mindbid);
		SetPropInfo((char*)"maxdb", tmBOOL,&d->maxdb);
		SetPropInfo((char*)"mindb", tmBOOL,&d->mindb);
		
		SetPropInfo((char*)"prompt", tmSTR,&pszPromptLan);
		SetPropInfo((char*)"promptID",tmSTR,&pszPromptID);
	}
}

WORD	CtmDoubleSpinBox::OnKey(WORD wKey)
{
	CtmSpinBox::OnKey(wKey);
}
		
void	CtmDoubleSpinBox::OnGetFocus()
{
	CtmSpinBox::OnGetFocus();
	
}

void	CtmDoubleSpinBox::OnLoseFocus()
{
	CtmSpinBox::OnLoseFocus();
}

void	CtmDoubleSpinBox::WndProc(int message,WPARAM wParam, LPARAM lParam)
{
	CtmSpinBox::WndProc(message, wParam, lParam);
}


void  CtmDoubleSpinBox::Prepare(CSpinBoxPrivate* d)
{
	if(d)
	{
		ResetPrompt(d);
		char        szMin[256];
		char        szMax[256];
		long 		lDataID = 0;
		char*		pszMin=szMin;
		char* 		pszMax=szMax;
		sprintf(pszMin, "%.*lf",d->precision,d->dMinValue);
		sprintf(pszMax, "%.*lf",d->precision,d->dMaxValue);
		A2A(szMin, 0, m_pcFont->GetFontCode());
		A2A(szMax, 0, m_pcFont->GetFontCode());
		
		if(g_bDemoDataID)
		{
			if(dbid[0] != NULL)
			{
				lDataID  = g_pDatabase->GetDataID(dbid[0]);
			}
		}
		if(CtmConfig::GetInstance()->GetFontConvert())
		{
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%x %t: %t %t: %t %t", lDataID, 
								g_MultiLanguage["MSG_MIN"], szMin,
								g_MultiLanguage["MSG_MAX"], szMax,pszPrompt);        
				}
				else
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
				
					tm_sprintf(d->szPrompt0, "%t: %t %t: %t %t", 
									g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax,pszPrompt);
				}
			}
			else
			{	
				if (pszPrompt != NULL)
				{
					if ((strlen(pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
					strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0, "%s:%s %s:%s %s",g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax, pszPrompt);
					}
				}
				else sprintf(d->szPrompt0, "%s:%s %s:%s"   , g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
			}	
		}
		else
		{
			if(m_pcFont->GetFontCode() == tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%x %t %t: %t %t: %t", lDataID, 
							pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
							g_MultiLanguage["MSG_MAX"], szMax);        
				}
				else
				{
					memset(d->szPrompt0, 0, tmMAX_TEXT_COL*2); 
					tm_sprintf(d->szPrompt0, "%t %t: %t %t: %t", 
									pszPrompt, g_MultiLanguage["MSG_MIN"], szMin,
									g_MultiLanguage["MSG_MAX"], szMax);
				}
			}
			else
			{	
				if (pszPrompt != NULL)
				{
					if ((strlen(pszPrompt) + strlen(g_MultiLanguage["MSG_MIN"]) + 
					strlen(szMin) + strlen(g_MultiLanguage["MSG_MAX"]) + strlen(szMax) + 5) <= tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0, "%s %s:%s %s:%s", pszPrompt, g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
					}
				}
				else sprintf(d->szPrompt0, "%s:%s %s:%s", g_MultiLanguage["MSG_MIN"], szMin, g_MultiLanguage["MSG_MAX"], szMax);
			}
		}
	}
	
}

void  CtmDoubleSpinBox::SyncDBValue(CSpinBoxPrivate* d)
{
	if(d)
	{
		int64 llValue=mRound(d->dValue*GetDecimalPowerVal(d->precision));
		int64 dLastValue=mRound(d->dLastValue*GetDecimalPowerVal(d->precision));
		if(dLastValue!=llValue)
		{
			if(!IsOverLimited(d))
			{
				if(dbid[0]!=NULL)
				{
					
					SetDBValue(dbid[0],llValue);
				}
			}
		}
	}
}

bool  CtmDoubleSpinBox::IsOverLimited(CSpinBoxPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->dValue<d->dMinValue||d->dValue>d->dMaxValue)
		{
			res=true;
		}
	}
	return res;
}


void  CtmDoubleSpinBox::SetCursorPosition(char acKey)
{
	if(d->ptaskCursor!=NULL)
	{
		int maxlen;
		std::string str=ToDoubleString(d,maxlen,suffix);
		int offset_bytes=suffix?strlen(suffix)+1:0;
		SIZE size={d->nFontSize,d->nFontSize};
		d->ptaskCursor->SetCursorPosition((char*)str.c_str(),str.size()+1,acKey,size,d->ftRect,d->wAlign,offset_bytes);
	}
}

bool  CtmDoubleSpinBox::ProcessOtherKey(WORD wKey)
{
	char        acKey[2];
	char		baseAscii='0';
	
	if (tmHIBYTE(wKey) == 0)
	{
		DisplayCursor(FALSE); 
		acKey[0] = tmLOBYTE(wKey);
	    acKey[1] = '\0';
		if ((acKey[0] >= '0') && (acKey[0] <= '9'))
		{
			d->hint=true;
			acKey[1]=acKey[0];
			//处理
			if(d->dotsymbols)
			{
				++d->curdecimal;
			}
			if(d->curdecimal==0)
			{
				d->integerpart=mAbs(d->integerpart)*d->displayIntegerBase+acKey[1]-baseAscii;
			}
			else
			{
				if(d->curdecimal>d->precision)
				{
					d->decimalpart/=d->displayIntegerBase;
					--d->curdecimal;
				}
				d->decimalpart=mAbs(d->decimalpart)*d->displayIntegerBase+acKey[1]-baseAscii;
			}
			d->dReInputValue=d->integerpart+(double)d->decimalpart/GetDecimalPowerVal(d->curdecimal);
			if(d->symbols)
			{
				d->dReInputValue=-d->dReInputValue;
			}
			d->dReInputValue=GetDoubleReInputValue(d);
			d->dValue=d->dReInputValue;
			
		}
		else if(acKey[0]=='-')
		{
			d->hint=true;
			if(d->dReInputValue==0)
			{
				d->symbols=true;
			}
		}
		else if(acKey[0]=='.')
		{
			if(d->precision>0)
			{
				d->hint=true;
				if(d->curdecimal==0)
				{
					d->dotsymbols=true;
					acKey[1]=acKey[0];
				}
			}
		}
		Show();
		if(acKey[1])
			SetCursorPosition(acKey[1]);
		DisplayCursor(TRUE);
	}
}

bool  CtmDoubleSpinBox::BackSpace(CSpinBoxPrivate* d)
{
	if(d)
	{
		if(d->curdecimal==0)
		{
			d->integerpart/=d->displayIntegerBase;
			d->dotsymbols=false;
		}
		else
		{
			d->decimalpart/=d->displayIntegerBase;
			--d->curdecimal;
		}
		d->dValue=
		d->dReInputValue=d->integerpart+(double)d->decimalpart/GetDecimalPowerVal(d->curdecimal);
		if(d->symbols)
		{
			d->dReInputValue=-d->dReInputValue;
			d->dValue=-d->dValue;
		}
		if(d->dValue==0)
		{
			Reset(d);
		}
	}
}

