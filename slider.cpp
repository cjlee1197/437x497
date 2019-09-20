/*Created by J.Wong 2017/1/13 16:11:03*/
#include "slider_p.h"
#include "slider.h"
#include "colortable.h"
#include "utils.h"
//#include <guilite/IUtils>

extern BOOL g_bFocusChanged;

enum Orientation{
	Horizontal,
	Vertial	
};

enum FlowDirection{
	LeftToRight,
	RightToLeft,
	TopToBottom,
	BottomToTop	
};

enum ScaleStyle{
	None=0x00,
	Above=0x01,
	Below=0x02,
	Left=0x010,
	Right=0x20,
	BothSides=0x22
};



CSliderPrivate::CSliderPrivate()
{
	init();	
}

CSliderPrivate::~CSliderPrivate()
{

}

void CSliderPrivate::init()
{
	max=99;
	min=0;
	value=0;
	tickInterval=1;
	size=CSize(100,20);
	penWidth=1;
	outlineWidth=1;
	
	mgn=CMargin(1,1,1,1);
	
	slotColor=Gemini::light_sky;
	oldSlotColor=slotColor;
	
	flowColor=Gemini::dark_blue_level0;
	outlineColor=Gemini::dark_gray_level2;
	scaleMin=min;
	scaleMax=max;
	oldScaleMin=scaleMin;
	oldScaleMax=scaleMax;
	orientation=Horizontal;
	oldOrientation=orientation;
	showFlowCap=false;
	showSlotCap=false;
	oldShowSlotCap=false;
	showScale=false;
	oldShowScale=false;
	showOutline=false;
	oldShowOutline=showOutline;
	redrawSlot=false;
	flowDirection=LeftToRight;
	scaleStyle=None;
	oldScaleStyle=scaleStyle;
	scaleDiv=5;
	oldScaleDiv=5;
	oldValue=0;
	x=0;
	y=0;
	focus=false;
	touched=false;
	transparent=false;
	oldTransparent=false;
	//attribute used for test
	positive =0;
	properchanged=false;
	
	maxdb =true;
	mindb =true;
	memset(szPrompt0,0,tmMAX_TEXT_COL*2);
	precision=0;
		
}



static inline void DrawSlot(CSliderPrivate* d)
{
	if(d)
	{
		if(d->redrawSlot)
		{
			// draw slot border
			CPainter painter(&d->fmImage);
			CPen pen;
			painter.setRenderHints(CPainter::Antialiasing,true);
			pen.setWidth(d->penWidth);
			pen.setColor(d->slotColor);
			pen.setCapStyle(Gemini::RoundCap);
			painter.setPen(pen);
			CRect rect(CPoint(0,0),d->size);
			rect-=(d->mgn+1);
			
			if(d->orientation==Vertial)
			{
				painter.drawLine((rect.left()+rect.right())/2,rect.top(),(rect.left()+rect.right())/2,rect.bottom());
				if(d->showSlotCap)
				{
					//draw cap
					painter.save();
					painter.setPen(CPen(pen.color(),1));
					painter.setBrush(CBrush(pen.color()));
					if(d->flowDirection==BottomToTop)
						painter.drawEllipse(CPointF((rect.left()+rect.right())/2,rect.bottom()),d->penWidth,d->penWidth*0.9);
					else if(d->flowDirection==TopToBottom)
						painter.drawEllipse(CPointF((rect.left()+rect.right())/2,rect.top()),d->penWidth,d->penWidth*0.9);
					painter.restore();
				}
			}
			else if(d->orientation==Horizontal)
			{
				painter.drawLine(rect.left(),(rect.top()+rect.bottom())/2,rect.right(),(rect.top()+rect.bottom())/2);	
				if(d->showSlotCap)
				{
					painter.setPen(CPen(pen.color(),1));
					painter.setBrush(CBrush(pen.color()));
					//draw cap
					if(d->flowDirection==LeftToRight)
						painter.drawEllipse(CPointF(rect.left(),(rect.top()+rect.bottom())/2),d->penWidth*0.9,d->penWidth);
					else if(d->flowDirection==RightToLeft)
						painter.drawEllipse(CPointF(rect.right(),(rect.top()+rect.bottom())/2),d->penWidth*0.9,d->penWidth);
					painter.restore();
				}	
			}	
		}	
	}		
}


static inline void DrawScaled(CSliderPrivate* d)
{
	if(d)
	{
		if(d->redrawSlot)
		{
			if(d->showScale)
			{
				float acScaleMin=mMax(d->scaleMin,d->min);
				float acScaleMax=mMin(d->scaleMax,d->max);
				CPainterPath path;
				CPainter painter(&d->fmImage);
				CPen pen;
				pen.setWidth(1);
				pen.setColor(Gemini::gray_level3);
				painter.setPen(pen);
				CRect rect(CPoint(0,0),d->size);
				rect-=(d->mgn-1);
				if(d->orientation==Vertial)
				{
					float len=rect.height()*(acScaleMax-acScaleMin)/(d->max-d->min);
					d->scaleDiv=d->scaleDiv>2?d->scaleDiv:2;
					float distance=len/d->scaleDiv;
					path.moveTo(d->penWidth,d->penWidth);
					path.lineTo(d->penWidth,d->penWidth+distance*d->scaleDiv);
					path.moveTo(d->penWidth,d->penWidth);
					path.lineTo(d->penWidth*0.4f,d->penWidth);
					for(int i=1; i<d->scaleDiv;++i)
					{
						path.moveTo(d->penWidth,d->penWidth+i*distance);
						path.lineTo(d->penWidth*0.6f,d->penWidth+i*distance);
					}
					path.moveTo(d->penWidth,d->penWidth+distance*(d->scaleDiv));
					path.lineTo(d->penWidth*0.4f,d->penWidth+distance*(d->scaleDiv));
					
					CPainterPath complex=path;
					
					if(d->flowDirection==BottomToTop)
						path.translate(0,rect.height()*(d->max-acScaleMax)/(d->max-d->min));
					else if(d->flowDirection==TopToBottom)
						path.translate(0,rect.height()*(acScaleMin-d->min)/(d->max-d->min));
					
					if((d->scaleStyle&0x30)==Right)
					{

						painter.translate(d->penWidth,d->penWidth);
						painter.rotate(180);
						painter.translate(-rect.right(),-d->penWidth-len);
						if(d->flowDirection==BottomToTop)
						{
							painter.translate(0,-rect.height()*(d->max-acScaleMax)/(d->max-d->min));
							painter.drawPath(complex);
						}
						else if(d->flowDirection==TopToBottom)
						{
							painter.translate(0,-rect.height()*(acScaleMin-d->min)/(d->max-d->min));
							painter.drawPath(complex);
						}
					}
					else if((d->scaleStyle&0x30)==Left)
					{
						painter.drawPath(path);	
					}
					else if((d->scaleStyle&0x30)==0x30)
					{
						painter.drawPath(path);
						painter.translate(d->penWidth,d->penWidth);
						painter.rotate(180);
						painter.translate(-rect.right(),-d->penWidth-len);
						if(d->flowDirection==BottomToTop)
						{
							painter.translate(0,-rect.height()*(d->max-acScaleMax)/(d->max-d->min));
							painter.drawPath(complex);
						}
						else if(d->flowDirection==TopToBottom)
						{
							painter.translate(0,-rect.height()*(acScaleMin-d->min)/(d->max-d->min));
							painter.drawPath(complex);
						}
					}	
				}
				else if(d->orientation==Horizontal)
				{
					
					float len=rect.width()*(acScaleMax-acScaleMin)/(d->max-d->min);
					d->scaleDiv=d->scaleDiv>2?d->scaleDiv:2;
					float distance=len/d->scaleDiv;
					path.moveTo(d->penWidth,d->penWidth);
					path.lineTo(d->penWidth+distance*d->scaleDiv,d->penWidth);
					path.moveTo(d->penWidth,d->penWidth);
					path.lineTo(d->penWidth,d->penWidth*0.4f);
					for(int i=1; i<d->scaleDiv;++i)
					{
						path.moveTo(d->penWidth+i*distance,d->penWidth);
						path.lineTo(d->penWidth+i*distance,d->penWidth*0.6f);
					}
					path.moveTo(d->penWidth+distance*(d->scaleDiv),d->penWidth);
					path.lineTo(d->penWidth+distance*(d->scaleDiv),d->penWidth*0.4f);
					CPainterPath complex=path;
					
					if(d->flowDirection==RightToLeft)
						path.translate(rect.width()*(d->max-acScaleMax)/(d->max-d->min),0);
					else if(d->flowDirection==LeftToRight)
						path.translate(rect.width()*(acScaleMin-d->min)/(d->max-d->min),0);
					
					
					if((d->scaleStyle&0x03)==Below)
					{
						painter.translate(d->penWidth,d->penWidth);
						painter.rotate(180);
						painter.translate(-d->penWidth-len,-rect.bottom());
						if(d->flowDirection==RightToLeft)
						{
							painter.translate(-rect.width()*(d->max-acScaleMax)/(d->max-d->min),0);
							painter.drawPath(complex);
						}
						else if(d->flowDirection==LeftToRight)
						{
							painter.translate(-rect.width()*(acScaleMin-d->min)/(d->max-d->min),0);
							painter.drawPath(complex);
						}
					}	
					else if((d->scaleStyle&0x03)==Above)
					{
						painter.drawPath(path);
					}
					else if((d->scaleStyle&0x03)==0x03)
					{
						painter.drawPath(path);
						painter.translate(d->penWidth,d->penWidth);
						painter.rotate(180);
						painter.translate(-d->penWidth-len,-rect.bottom());
						if(d->flowDirection==RightToLeft)
						{
							painter.translate(-rect.width()*(d->max-acScaleMax)/(d->max-d->min),0);
							painter.drawPath(complex);
						}
						else if(d->flowDirection==LeftToRight)
						{
							painter.translate(-rect.width()*(acScaleMin-d->min)/(d->max-d->min),0);
							painter.drawPath(complex);
						}
					}	
				}	
			}	
		}		
	}
}
static inline void DrawFlow(CSliderPrivate* d)
{
	if(d)
	{
		CPainter painter(&d->image);
		CPen pen;
		painter.setRenderHints(CPainter::Antialiasing,true);
		pen.setWidth(d->penWidth>1?d->penWidth-1:1);
		pen.setColor(d->flowColor);
		pen.setCapStyle(Gemini::RoundCap);
		painter.setPen(pen);
		CRect rect(CPoint(0,0),d->size);
		rect-=(d->mgn+1);
		d->value=mMin(d->max,mMax(d->min,d->value));
		float scaled=0.0f;
		if((d->max-d->min)>0)
			scaled=d->value/(d->max-d->min);
			
		if(d->orientation==Vertial)
		{
			
			if(d->flowDirection==BottomToTop)
			{
				int end=rect.bottom()-scaled*rect.height()+0.5f;
				painter.drawLine((rect.left()+rect.right())/2,rect.bottom(),(rect.left()+rect.right())/2,end);
				painter.save();
				painter.setBrush(CBrush(pen.color()));
				painter.setPen(CPen(pen.color(),1));
				if(d->showFlowCap)
				//draw cap
					painter.drawEllipse(CPointF((rect.left()+rect.right())/2,end),d->penWidth,d->penWidth);
				if(d->showSlotCap&&scaled)
						painter.drawEllipse(CPointF((rect.left()+rect.right())/2,rect.bottom()),d->penWidth,d->penWidth*0.9);
				painter.restore();
			}
			else if(d->flowDirection==TopToBottom)
			{
				int end=(rect.top()+scaled*rect.height()+0.5f);
				
				painter.drawLine((rect.left()+rect.right())/2,rect.top(),(rect.left()+rect.right())/2,end);
				painter.save();
				painter.setBrush(CBrush(pen.color()));
				painter.setPen(CPen(pen.color(),1));
				if(d->showFlowCap)
					//draw cap
					painter.drawEllipse(CPointF((rect.left()+rect.right())/2,end),d->penWidth,d->penWidth);
				if(d->showSlotCap&&scaled)
						painter.drawEllipse(CPointF((rect.left()+rect.right())/2,rect.top()),d->penWidth,d->penWidth*0.9);
				painter.restore();
			}
		}
		else if(d->orientation==Horizontal)
		{
			if(d->flowDirection==LeftToRight)
			{
				int end=rect.left()+scaled*rect.width()+0.5f;
				painter.drawLine(rect.left(),(rect.top()+rect.bottom())/2,end,(rect.top()+rect.bottom())/2);
				painter.save();
				painter.setBrush(CBrush(pen.color()));
				painter.setPen(CPen(pen.color(),1));
				if(d->showFlowCap)
				//draw cap
					painter.drawEllipse(CPointF(end,(rect.top()+rect.bottom())/2),d->penWidth,d->penWidth);
				if(d->showSlotCap&&scaled)
					painter.drawEllipse(CPointF(rect.left(),(rect.top()+rect.bottom())/2),d->penWidth*0.9,d->penWidth);
				painter.restore();
			}
			else if(d->flowDirection==RightToLeft)
			{
				int end=rect.right()-scaled*rect.width()+0.5f;
				painter.drawLine(rect.right(),(rect.top()+rect.bottom())/2,end,(rect.top()+rect.bottom())/2);
				painter.save();
				painter.setBrush(CBrush(pen.color()));
				painter.setPen(CPen(pen.color(),1));
				if(d->showFlowCap)
				//draw cap
					painter.drawEllipse(CPointF(end,(rect.top()+rect.bottom())/2),d->penWidth,d->penWidth);
				if(d->showSlotCap&&scaled)
					painter.drawEllipse(CPointF(rect.right(),(rect.top()+rect.bottom())/2),d->penWidth*0.9,d->penWidth);
				painter.restore();
			}		
		}		
	}	
}

static inline void Tick(CSliderPrivate* d,bool valuences)
{
	if(d)
	{
		if(d->flowDirection==LeftToRight)
		{
			if(valuences)
			{
				d->value+=d->tickInterval;
				d->value=mMin(d->value,d->max);
			}
			else
			{
				d->value-=d->tickInterval;
				d->value=mMax(d->value,d->min);
			}
		}
		else if(d->flowDirection==RightToLeft)
		{
			if(valuences)
			{
				d->value-=d->tickInterval;
				d->value=mMax(d->value,d->min);
			}
			else
			{
				d->value+=d->tickInterval;
				d->value=mMin(d->value,d->max);
			}	
		}
		else if(d->flowDirection==TopToBottom)
		{
			if(valuences)
			{
				d->value+=d->tickInterval;
				d->value=mMin(d->value,d->max);
			}
			else
			{
				d->value-=d->tickInterval;
				d->value=mMax(d->value,d->min);
			}
		}
		else if(d->flowDirection==BottomToTop)
		{
			if(valuences)
			{
				d->value-=d->tickInterval;
				d->value=mMax(d->value,d->min);
			}
			else
			{
				d->value+=d->tickInterval;
				d->value=mMin(d->value,d->max);
			}	
		}		
	}	
}

void CtmSlider::Resize(CSliderPrivate* d)
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

void CtmSlider::PenChanged(CSliderPrivate* d)
{
	if(d)
	{
		d->penWidth=mMin(d->size.width(),d->size.height())/4;
		d->penWidth=d->penWidth>1?d->penWidth-1:1;
		d->mgn=CMargin(1,1,1,1)+d->penWidth;	
	}	
}

bool CtmSlider::IsPenChanged(CSliderPrivate* d)
{
	if(d)
	{
		int tmp=mMin(d->size.width(),d->size.height())/4;
		tmp=tmp>1?tmp-1:1;
		return (d->penWidth!=tmp);	
	}
	else
		return false;
}

bool CtmSlider::IsNeedResize(CSliderPrivate* d)
{
	if(d)
		return m_rectClient.left!=d->x||m_rectClient.top!=d->y||CSize(GetClientWidth(),GetClientHeight())!=d->size;
	else
		return false;	
}


bool CtmSlider::IsSlotChanged(CSliderPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->oldShowSlotCap!=d->showSlotCap)
		{
			d->oldShowSlotCap=d->showSlotCap;
			res=true;
		}
		if(d->showOutline!=d->oldShowOutline)
		{
			d->oldShowOutline=d->showOutline;
			res=true;
		}
	}
	return res;	
}


bool CtmSlider::IsScaleChanged(CSliderPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->showScale!=d->oldShowScale)
		{
			d->oldShowScale=d->showScale;
			res=true;
		}
		if(d->scaleDiv!=d->oldScaleDiv)
		{
			d->oldScaleDiv=d->scaleDiv;	
			res=true;
		}
		if(d->scaleMax!=d->oldScaleMax)
		{
			d->oldScaleMax=d->scaleMax;
			res=true;
		}
		if(d->scaleMin!=d->oldScaleMin)
		{
			d->oldScaleMin=d->scaleMin;
			res=true;
		}
		if(d->scaleStyle!=d->oldScaleStyle)
		{
			d->oldScaleStyle=d->scaleStyle;
			res=true;
		}
	}
	return res;
}

bool CtmSlider::IsTransparentChanged(CSliderPrivate* d)
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

bool CtmSlider::IsOrientationChanged(CSliderPrivate* d)
{
	if(d)
	{
		if(d->orientation!=d->oldOrientation)
		{
			d->oldOrientation=d->orientation;
			return true;
		}
	}
	return false;	
}
IMPLEMENT_DYNCREATE(CtmSlider, CtmWnd)

CtmSlider::CtmSlider():CtmWnd()
{
	strcpy(TYPEName, "CtmSlider");
	d=new CSliderPrivate();
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);
		//d->fmImage.fill((uint32)m_bgc);
	}
	
	for(int i=0; i<SLIDER_MAXDBNUM;++i)
	{
		dbid[i]=0;
	}
	maxdbid =NULL;
	mindbid =NULL;
	
	pszPrompt=NULL;
	pszPromptLan=NULL;
	pszPromptID=NULL;
}

CtmSlider::~CtmSlider()
{
	delete d;
}

void    CtmSlider::Show()
{
	//CTime time;
	//time.start();
	if(IsTransparentChanged(d)||IsNeedResize(d)||IsPenChanged(d)||
	   IsSlotChanged(d)||IsScaleChanged(d)||IsOrientationChanged(d))
	{
		IsSlotChanged(d);
		IsScaleChanged(d);
		IsOrientationChanged(d);
		if(IsNeedResize(d))
		{
			Resize(d);
		}
		if(IsPenChanged(d))
			PenChanged(d);
			
		if(!d->transparent)
		{
			d->fmImage.fill((uint)m_bgc);	
		}
		else
		{
			if(!d->bgImage.isNull())
				d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		}
		d->redrawSlot=true;
		d->properchanged=true;
	}
	else
	{
		d->redrawSlot=false;	
	}
	//printf("1:time:%d\n",time.elapsed());
	//time.restart();
	if(d->focus)
	{
		d->properchanged=true;
	}
	if(d->properchanged)
	{
		if(m_bVisible)
		{
			DrawSlot(d);
			DrawScaled(d);
			Clear();
			if(!d->focus)
			{
				GetDBInfo(d);
			}
			DrawFlow(d);
			//printf("2:time:%d\n",time.elapsed());
			//time.restart();
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->image);
			//printf("3:time:%d\n",time.elapsed());
		 }
		 else
		 {
			_SetViewPort(m_rectClient.left,m_rectClient.top,m_rectClient.right,m_rectClient.bottom);
			_PutImage(0,0,d->bgImage);	
		 }
		 d->properchanged=false;
	}
}

void    CtmSlider::Update()
{
	//auto test coding
	/*
	if(d->positive)
	{
		d->value+=5;
	}
	else
	{
		d->value-=5;	
	}
	if(d->value>=d->max)
	{
		d->positive=0;
	}
	else if(d->value<=d->min)
	{
		d->positive=1;
	}
	*/
	if(!d->focus)
	{
		GetDBInfo(d);
		if(d->oldValue!=d->value)
		{
			d->oldValue=d->value;
			d->properchanged=true;
		}
	}
	Show();
}

void    CtmSlider::Clear()
{
	if(d)
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
}

BOOL 	CtmSlider::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
}





void 	CtmSlider::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"slot-color",	tmDWORD, &d->slotColor);
		SetPropInfo((char*)"flow-color",	tmDWORD, &d->flowColor);
		SetPropInfo((char*)"max",			tmFLOAT, &d->max);
		SetPropInfo((char*)"min",			tmFLOAT, &d->min);
		SetPropInfo((char*)"value",			tmFLOAT, &d->value);
		SetPropInfo((char*)"tick-interval",	tmFLOAT, &d->tickInterval);
		SetPropInfo((char*)"show-scale", 	tmBOOL,	 &d->showScale);
		SetPropInfo((char*)"show-slotcap", 	tmBOOL,  &d->showSlotCap);
		SetPropInfo((char*)"show-flowcap",	tmBOOL,	 &d->showFlowCap);
		SetPropInfo((char*)"show-outline", 	tmBOOL,	 &d->showOutline);
		
		SetPropInfo((char*)"orientation",	tmWORD,  &d->orientation);
		SetPropInfo((char*)"flow-direction",tmWORD,  &d->flowDirection);
		SetPropInfo((char*)"scale-style",	tmWORD,	 &d->scaleStyle);
		SetPropInfo((char*)"scale-div",		tmWORD,	 &d->scaleDiv);
		SetPropInfo((char*)"scale-max",		tmFLOAT, &d->scaleMax);
		SetPropInfo((char*)"scale-min",		tmFLOAT, &d->scaleMin);
		
		SetPropInfo((char*)"touched",		tmBOOL,	 &d->touched);
		SetPropInfo((char*)"transparent",	tmBOOL,	 &d->transparent);
		SetPropInfo((char*)"private-changed", tmBOOL, 	&d->properchanged);
		
		char sz[10];
		for(int i=0; i<SLIDER_MAXDBNUM; ++i)
		{
			sprintf(sz,"dbid%i", i);
			SetPropInfo(sz, tmSTR, &dbid[i]);
		}
		
		SetPropInfo((char*)"maxid", tmSTR,  &maxdbid);
		SetPropInfo((char*)"minid", tmSTR,  &mindbid);
		SetPropInfo((char*)"maxdb", tmBOOL, &d->maxdb);
		SetPropInfo((char*)"mindb", tmBOOL, &d->mindb);
		
		SetPropInfo((char*)"prompt", tmSTR, &pszPromptLan);
		SetPropInfo((char*)"promptID", tmSTR, &pszPromptID);
		SetPropInfo((char*)"precision", tmWORD, &d->precision);
	}
		
}

void 	CtmSlider::SetOrientation(int orientation)
{
	if(d)
		d->orientation=orientation;
		
}

int		CtmSlider::GetOrientation()const
{
	if(d)
		return d->orientation;
	else
		return 0;
}

void	CtmSlider::SetFlowCap(bool cap)
{
	if(d)
		d->showFlowCap=cap;
}

void	CtmSlider::SetSlotCap(bool cap)
{
	if(d)
		d->showSlotCap=cap;
}

bool 	CtmSlider::IsShowFlowCap()const
{
	if(d)
		return d->showFlowCap;
	else 
		return 0;
}

bool 	CtmSlider::IsShowSlotCap()const
{
	if(d)
		return d->showSlotCap;
	else
		return 0;
}
		
void 	CtmSlider::SetScaleStyle(int style)
{
	if(d)
		d->scaleStyle=style;
}

int 	CtmSlider::GetScaleStyle()const
{
	if(d)
		return d->scaleStyle;
	else
		return 0;
}

void 	CtmSlider::SetScale(bool scale)
{
	if(d)
		d->showScale=scale;
}

bool	CtmSlider::IsShowScale()const
{
	if(d)
		return d->showScale;
	else
		return 0;
}
		
void	CtmSlider::SetFlowDirection(int direction)
{
	if(d)
		d->flowDirection=direction;
}

int		CtmSlider::GetFlowDirection()const
{
	if(d)
		return d->flowDirection;
	else
		return 0;
}
		
void 	CtmSlider::SetOutline(bool outline)
{
	if(d)
		d->showOutline=outline;
	
}

bool	CtmSlider::IsShowOutline()const
{
	if(d)
		return d->showOutline;
	else
		return 0;
}
		
void	CtmSlider::SetScaleMin(float min)
{
	if(d)
		d->scaleMin=min;
}

void	CtmSlider::SetScaleMax(float max)
{
	if(d)
		d->scaleMax=max;
}
void	CtmSlider::SetScale(float min, float max)
{
	SetScaleMin(min);
	SetScaleMax(max);
}

void	CtmSlider::SetScaleDiv(int div)
{
	if(d)
		d->scaleDiv=div;
}

float	CtmSlider::GetScaleMax()const
{
	if(d)
		return d->scaleMax;
	else
		return 0;
}

float	CtmSlider::GetScaleMin()const
{
	if(d)
		return d->scaleMin;
	else
		return 0;
}

int 	CtmSlider::GetScaleDiv()const
{
	if(d)
		return d->scaleDiv;
	else
		return 0;
}
	
void	CtmSlider::SetSlotColor(uint color)
{
	if(d)
		d->slotColor=color;
}

void 	CtmSlider::SetFlowColor(uint color)
{
	if(d)
		d->flowColor=color;

}

void	CtmSlider::SetColor(uint slotColor, uint flowColor)
{
	SetSlotColor(slotColor);
	SetFlowColor(flowColor);
}

uint		CtmSlider::GetSlotColor()const
{
	if(d)
		return d->slotColor;
	else
		return 0;
}

uint		CtmSlider::GetFlowColor()const
{
	if(d)
		return d->flowColor;
	else
		return 0;
}

void	CtmSlider::SetValue(float value)
{
	if(d)
		d->value=value;
}

void	CtmSlider::SetMax(float max)
{
	if(d)
		d->max=max;
}

void	CtmSlider::SetMin(float min)
{
	if(d)
		d->min=min;
}

float	CtmSlider::GetValue()const
{
	if(d)
		return d->value;
	else 
		return 0;
}

float	CtmSlider::GetMin()const
{
	if(d)
		return d->min;
	else
		return 0;
}

float	CtmSlider::GetMax()const
{
	if(d)
		return d->max;
	else
		return 0;
}		

void	CtmSlider::SetTickInterVal(float ival)
{
		if(d)
		  d->tickInterval=ival;
}

float	CtmSlider::GetTickInterVal()const
{
	if(d)
		return d->tickInterval;
	else
		return 0;
}

void    CtmSlider::OnGetFocus()
{
		if(d)
		{
			GetDBInfo(d);
			Update();
			d->oldValue=d->value;
			d->focus = true;
			CtmWnd::DrawGetFocus();
			Prepare(d);
			Prompt(d->szPrompt0);
		}
}

void    CtmSlider::OnLoseFocus()
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

//Event Process

WORD    CtmSlider::OnKey(WORD wKey)
{
	WORD			wResult=wKey;
	if(!d)			return wKey;
	if(!d->focus)	return wKey;
	//printf("wKey=%08x,down=%08x,up=%08x\n",wKey,_KEY_DOWNFIELD,_KEY_UPFIELD);
	switch (wKey)
	{
		case 0x6D00: 	//down
		case _KEY_DOWNFIELD:
			if(d->orientation==Vertial)
			{
				Tick(d,1);
				Update();
				wResult=_NULL_KEY;
			}
			break;
		case 0x6800:	//up
		case _KEY_UPFIELD:
			if(d->orientation==Vertial)
			{
				Tick(d,0);
				Update();
				wResult=_NULL_KEY;
			}
			break;
		case _KEY_LEFTFIELD:
			if(d->orientation==Horizontal)
			{
				Tick(d,0);
				Update();
				wResult=_NULL_KEY;
			}
			break;
		case _KEY_RIGHTFIELD:
			if(d->orientation==Horizontal)
			{
				Tick(d,1);
				Update();
				wResult=_NULL_KEY;
			}
			break;
		default:
			break;	
	}
	return wResult;		
}	


bool CtmSlider::TouchAreaChanged(CSliderPrivate* d,const CPoint& control)
{
	bool res=false;
	if(d)
	{
		CPoint cmp=control+CPoint(-m_rectClient.left,-m_rectClient.top);
		d->value=mMin(d->max,mMax(d->min,d->value));
		float scaled=0;
		if((d->max-d->min)>0)
			scaled=d->value/(d->max-d->min);
		CRect rect=CRect(CPoint(0,0),d->size);
		rect-=(d->mgn+1);
		if(d->orientation==Vertial)
		{
			int end=0;
			if(d->flowDirection==TopToBottom)
			{
				end=rect.top()+scaled*rect.height()+0.5f;
				
			}
			else if(d->flowDirection==BottomToTop)
			{
				end=rect.bottom()-scaled*rect.height()+0.5f;
			}
			rect+=(d->mgn+1);
			CRect top(rect.left(),rect.top(),rect.right(),end);
			CRect bottom(rect.left(),end,rect.right(),rect.bottom());
			if(top.contains(cmp))
			{
				Tick(d,0);
				res=true;
			}
			else if(bottom.contains(cmp))
			{
				Tick(d,1);
				res=true;	
			}
		}
		else if(d->orientation==Horizontal)
		{
			int end=0;
			if(d->flowDirection==LeftToRight)
			{
				end=rect.left()+scaled*rect.width()+0.5f;
			}
			else if(d->flowDirection==RightToLeft)
			{
				end=rect.right()-scaled*rect.width()+0.5f;	
			}
			rect+=(d->mgn+1);
			CRect left(rect.left(),rect.top(),end-rect.left()+1,rect.height());
			CRect right(end,rect.top(),rect.right()-end+1,rect.height());
			if(left.contains(cmp))
			{
				Tick(d,0);
				res=true;
			}
			else if(right.contains(cmp))
			{
				Tick(d,1);
				res=true;	
			}
		}	
	}
	return res;	
}

void	CtmSlider::WndProc(int message, WPARAM wParam, LPARAM lParam)
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

void CtmSlider::GetDBInfo(CSliderPrivate* d)
{
	if(d)
	{
		if(dbid[0])
		{
			DBVIEWVALUE dbViewValue;
			dbViewValue =GetDBValue(dbid[0]);
			int muti=GetDecimalPowerVal(d->precision);
			d->value =(float)dbViewValue.lValue/muti;
			if(d->maxdb)
			{
				d->max = (float)dbViewValue.lMaxValue/muti;
			}
			else
			{
				if(maxdbid)
				{
					
					d->max=(float)GetDBValue(maxdbid).lValue/muti;
				}
			}
			
			if(d->mindb)
			{
				d->min=(float)dbViewValue.lMinValue/muti;
			}
			else
			{
				if(mindbid)
				{
					d->min=(float)GetDBValue(mindbid).lValue/muti;
				}
			}
		}
	}
}


void CtmSlider::SyncDBValue(CSliderPrivate* d)
{
	if(d)
	{
		int64 llValue=mRound(d->value*GetDecimalPowerVal(d->precision));
		int64 llOldValue=mRound(d->oldValue*GetDecimalPowerVal(d->precision));
		if(llValue!=llOldValue)
		{
			if(dbid[0]!=NULL)
			{
				if(!IsOverLimited(d))
					SetDBValue(dbid[0],llValue);
			}
		}
	}
}

bool CtmSlider::IsOverLimited(CSliderPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->value<d->min||d->value>d->max)
		{
			res=true;
		}
	}
	return res;
}


void CtmSlider::ResetPrompt(CSliderPrivate* d)
{
	if(d)
	{
		if(pszPromptID)
			pszPrompt=g_MultiLanguage[pszPromptID];
		else
			pszPrompt=pszPromptLan;
	}
}

void CtmSlider::Prepare(CSliderPrivate* d)
{
	if(d)
	{
		ResetPrompt(d);
		char szMin[256];
		char szMax[256];
		char* pszMin=szMin;
		char* pszMax=szMax;
		
		long lDataID=0;
		sprintf(pszMin, "%.*lf",d->precision,d->min);
		sprintf(pszMax, "%.*lf",d->precision,d->max);
		A2A(szMin,0,m_pcFont->GetFontCode());
		A2A(szMax,0,m_pcFont->GetFontCode());
		if(g_bDemoDataID)
		{
			if(dbid[0])
			{
				lDataID=g_pDatabase->GetDataID(dbid[0]);
			}
		}
		if(CtmConfig::GetInstance()->GetFontConvert())
		{
			if(m_pcFont->GetFontCode()==tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0,0,tmMAX_TEXT_COL*2);
					tm_sprintf(d->szPrompt0,"%x %t: %t %t: %t %t", lDataID, g_MultiLanguage["MSG_MIN"],szMin,
					g_MultiLanguage["MSG_MAX"],szMax, pszPrompt);
				}
				else
				{
					memset(d->szPrompt0, 0,tmMAX_TEXT_COL*2);
					tm_sprintf(d->szPrompt0,"%t: %t %t: %t %t", g_MultiLanguage["MSG_MIN"],szMin,
					g_MultiLanguage["MSG_MAX"],szMax, pszPrompt);
				}
			}
			else
			{
				if(pszPrompt!=NULL)
				{
					if((strlen(pszPrompt)+strlen(g_MultiLanguage["MSG_MIN"])+strlen(g_MultiLanguage["MSG_MAX"])+
					strlen(szMin)+strlen(szMax)+5)<=tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0,"%s:%s %s:%s %s",g_MultiLanguage["MSG_MIN"],szMin,g_MultiLanguage["MSG_MAX"],szMax,pszPrompt);
					}
				}
				else
					sprintf(d->szPrompt0,"%s:%s %s:%s",g_MultiLanguage["MSG_MIN"],szMin,g_MultiLanguage["MSG_MAX"],szMax);
			}
		}
		else
		{
			if(m_pcFont->GetFontCode()==tmFT_CODE_TECH)
			{
				if(g_bDemoDataID)
				{
					memset(d->szPrompt0,0,tmMAX_TEXT_COL*2);
					tm_sprintf(d->szPrompt0,"%x %t %t: %t %t: %t", lDataID, pszPrompt, g_MultiLanguage["MSG_MIN"],szMin,
					g_MultiLanguage["MSG_MAX"],szMax);
				}
				else
				{
					memset(d->szPrompt0, 0,tmMAX_TEXT_COL*2);
					tm_sprintf(d->szPrompt0,"%t %t: %t %t: %t",pszPrompt,g_MultiLanguage["MSG_MIN"],szMin,
					g_MultiLanguage["MSG_MAX"],szMax);
				}
			}
			else
			{
				if(pszPrompt!=NULL)
				{
					if((strlen(pszPrompt)+strlen(g_MultiLanguage["MSG_MIN"])+strlen(g_MultiLanguage["MSG_MAX"])+
					strlen(szMin)+strlen(szMax)+5)<=tmMAX_TEXT_COL*2)
					{
						sprintf(d->szPrompt0,"%s %s:%s %s:%s",pszPrompt,g_MultiLanguage["MSG_MIN"],szMin,g_MultiLanguage["MSG_MAX"],szMax);
					}
				}
				else
					sprintf(d->szPrompt0,"%s:%s %s:%s",g_MultiLanguage["MSG_MIN"],szMin,g_MultiLanguage["MSG_MAX"],szMax);
			}
		}
	}
}
