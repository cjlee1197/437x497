#include "dial_p.h"
#include "dial.h"
#include "colortable.h"
#include "utils.h"


extern BOOL g_bFocusChanged;
enum ClockDirection
{
	AntiClockwise=0,
	Clockwise=1	
};

enum DialStyle
{
	Normal=0,
	WaterBall=1
};


CDialPrivate::CDialPrivate()
{
	init();	
}
	
CDialPrivate::~CDialPrivate()
{
	
}

void CDialPrivate::init()
{
	value=0;
	min=0.0;
	max=100.0;
	tickInterval=1.0;
	oldValue=0;
	
	touched=false;
	transparent=false;
	oldTransparent=false;
	clockwise=Clockwise;
	oldClockwise=clockwise;
	
	innerColor=Gemini::dark_blue_level2;
	outerColor=Gemini::dark_blue_level0;
	color=Gemini::dark_blue_level0;
	spanAngle=360;
	oldSpanAngle=spanAngle;
	scaleDiv=10;
	penWidth=2;
	mgn=CMargin(1,1,1,1);
	size=CSize(100,100);
	x=0;
	y=0;
	focus=false;
	redraw=false;
	positive=false;
	properchanged=false;
	maxdb=true;
	mindb=true;
	precision=0;
	wDialStyle=0;
	wOldDialStyle=0;
	offset=0;
	
	memset(szPrompt0,0,tmMAX_TEXT_COL*2);
}



void	CtmDial::SetValue(float value)
{
	if(d)
		d->value=value;
}

float	CtmDial::GetValue()const
{
	if(d)
		return d->value;
	return 0;
}
	
void	CtmDial::SetMax(float max)
{
	if(d)
		d->max=max;
}

float	CtmDial::GetMax()const
{
	if(d)
		return d->max;
	return 0;
}
	
void 	CtmDial::SetMin(float min)
{
	if(d)
		d->min=min;
}

float	CtmDial::GetMin()const
{
	if(d)
		return d->min;
	return 0;
}
	
void	CtmDial::SetOuterColor(uint color)
{
	if(d)
		d->outerColor=color;
}

uint	CtmDial::GetOuterColor()const
{
	if(d)
		return d->outerColor;
	return 0;
}

void	CtmDial::SetInnerColor(uint color)
{
	if(d)
		d->innerColor=color;
}

uint	CtmDial::GetInnerColor()const
{
	if(d)
		return d->innerColor;
	return 0;
}

void	CtmDial::SetColor(uint color)
{
	if(d)
		d->color=color;
}

uint	CtmDial::GetColor()const
{
	if(d)
		return d->color;
	return 0;
}
	
void	CtmDial::SetClockwise(BOOL clockwise)
{
	if(d)
		d->clockwise=clockwise;
}

BOOL	CtmDial::GetClockwise()const
{
	if(d)
		return d->clockwise;
	return 0;
}
	
void	CtmDial::SetSpanAngle(float angle)
{
	if(d)
		d->spanAngle=angle;
}

float	CtmDial::GetSpanAngle()const
{
	if(d)
		return d->spanAngle;
	return 0;
}
	
void	CtmDial::SetTickInterval(float tick)
{
	if(d)
		d->tickInterval=tick;
}

float	CtmDial::GetTickInterval()const
{
	if(d)
		return d->tickInterval;
	return 0;
}
	
void	CtmDial::SetDialDiv(WORD div)
{
	if(d)
		d->scaleDiv=div;
}

WORD	CtmDial::GetDialDiv()const
{
	if(d)
		return d->scaleDiv;
	return 0;
}

IMPLEMENT_DYNCREATE(CtmDial, CtmWnd)
CtmDial::CtmDial():CtmWnd()
{
	strcpy(TYPEName, "CtmDial");
	d=new CDialPrivate();
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);	
	}
	
	pszPrompt=0;
	pszPromptLan=0;
	pszPromptID=0;
	
	dbid=0;
	maxdbid=0;
	mindbid=0;
}

CtmDial::~CtmDial()
{
	delete d;
}

BOOL 	CtmDial::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
}


void 	CtmDial::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		SetPropInfo((char*)"value",tmFLOAT,&d->value);
		SetPropInfo((char*)"min",tmFLOAT,&d->min);
		SetPropInfo((char*)"max",tmFLOAT,&d->max);
		SetPropInfo((char*)"tick-interval",tmFLOAT,&d->tickInterval);
		SetPropInfo((char*)"scale-div",tmWORD,&d->scaleDiv);	
		SetPropInfo((char*)"color",tmDWORD,&d->color);
		SetPropInfo((char*)"inner-color",tmDWORD,&d->innerColor);
		SetPropInfo((char*)"outer-color",tmDWORD,&d->outerColor);
		SetPropInfo((char*)"touched",tmBOOL, &d->touched);
		SetPropInfo((char*)"transparent",tmBOOL,&d->transparent);
		SetPropInfo((char*)"clockwise",tmBOOL,&d->clockwise);
		SetPropInfo((char*)"span-angle",tmFLOAT,&d->spanAngle);
		SetPropInfo((char*)"private-changed", tmBOOL, 	&d->properchanged);
		SetPropInfo((char*)"precision",tmWORD, &d->precision);
		SetPropInfo((char*)"dbid0",tmSTR, &dbid);
		SetPropInfo((char*)"maxid",tmSTR,&maxdbid);
		SetPropInfo((char*)"minid",tmSTR,&mindbid);
		SetPropInfo((char*)"maxdb", tmBOOL, &d->maxdb);
		SetPropInfo((char*)"mindb", tmBOOL, &d->mindb);
		SetPropInfo((char*)"prompt",tmSTR, &pszPromptLan);
		SetPropInfo((char*)"promptID",tmSTR, &pszPromptID);
		
		SetPropInfo((char*)"dial-style",tmWORD, &d->wDialStyle);
	}	
}

void    CtmDial::Update()
{
	//auto test coding
	if(!d->focus)
	{
		GetDBInfo(d);
		if(d->value!=d->oldValue)
		{
			d->oldValue=d->value;
			d->properchanged=true;
		}
	}
	Show();	
}

void    CtmDial::Clear()
{
	if(d)
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));	
}

void   	CtmDial::OnGetFocus()
{
	if(d)
	{
		GetDBInfo(d);
		Update();
		d->oldValue=d->value;
		d->focus	=true;
		CtmWnd::DrawGetFocus();
		Prepare(d);
		Prompt(d->szPrompt0);
	}
}

void    CtmDial::OnLoseFocus()
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

bool CtmDial::IsNeedResize(CDialPrivate* d)
{
	if(d)
		return m_rectClient.left!=d->x||m_rectClient.top!=d->y||CSize(GetClientWidth(),GetClientHeight())!=d->size;
	else
		return false;
}

void CtmDial::Resize(CDialPrivate* d)
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

static inline void DrawPanel(CDialPrivate* d,int colorControl)
{
	if(d)
	{
		if(d->redraw)
		{
			CPainter painter(&d->fmImage);
			
			
			CRect bgRect=CRect(CPoint(0,0),d->size);
			int square=mMin(d->size.width(),d->size.height());
			CRect outerRect=CRect(CPoint(0,0),CSize(square,square));
			CMargin outerMgn=d->mgn+d->penWidth+1;
			
			CPoint cp=bgRect.center();
			CPoint op=outerRect.center();
			outerRect.translate(cp-op);
			outerRect-=outerMgn;
			d->spanAngle=mMin(360.0,mMax(0.0,d->spanAngle));
			
			CRect inRect=CRect(CPoint(0,0),outerRect.size()*4/5);
			CPoint ip=inRect.center();
			inRect.translate(cp-ip);
			float scaleAlngle=d->spanAngle/d->scaleDiv;
			float halfSpanAngle=d->spanAngle/2.0;
			
			int len=(outerRect.width()-inRect.width())/2;
			len=len*4/5;
			CLineF lineF=CLineF(CPoint(0,0),CPoint(len,0));
			CPoint start=CPoint(outerRect.right()-len,outerRect.top()+outerRect.width()/2);
			lineF.translate(start);
			std::vector<CLineF> lines;
			lines.resize(d->scaleDiv+1);
			float IR=(outerRect.width()-2*len)/2;
			
			for(int i=0; i<=d->scaleDiv; ++i)
			{
				float angle=-halfSpanAngle+90+scaleAlngle*i;
				float x=IR*(1-cos((angle)/180.0*3.1415926));
				float y=IR*(sin((angle)/180.0*3.1415926));
				lines[i]=lineF.translated(-x,-y);
				lines[i].setAngle(angle);
			}
			
			painter.setRenderHints(CPainter::Antialiasing,true);
			painter.save();
			
			CPen pen(Gemini::white_level1);
			CConicalGradient conical(cp.x(),cp.y(),-halfSpanAngle+90);
			float changed=d->spanAngle/360;
			if(d->clockwise==Clockwise)
			{
				conical.setColorAt(0,d->color);
				conical.setColorAt(changed,Gemini::white_level1);
			}
			else
			{
				
				conical.setColorAt(0,Gemini::white_level1);
				//颜色减1,否则无法达到想要的效果(GUI渲染使用颜色叠加hash算法)	
				conical.setColorAt(changed,d->color-1);
			}
			CGradientStops stop=conical.stops();
			if(colorControl)
			{
				CBrush conBrush(conical);
				painter.setBrush(conBrush);
			}
			else
			{
				painter.setBrush(CBrush(d->color));
			}
			CPainterPath path;
			CPainterPath help;
			outerRect+=CMargin(1,1,0,0);
			inRect+=CMargin(0,0,-1,-1);
			help.arcMoveTo(outerRect,halfSpanAngle+90);
			CPoint mpt=help.currentPosition().toPoint();
			
			path.arcMoveTo(inRect,-halfSpanAngle+90);
			CPoint spt=path.currentPosition().toPoint();
			path.arcTo(inRect,(-halfSpanAngle+90),d->spanAngle);
			path.lineTo(mpt);
			path.arcTo(outerRect,halfSpanAngle+90,-d->spanAngle);
			path.lineTo(spt);
			painter.drawPath(path);
			
			painter.restore();
			
			pen.setWidth(d->penWidth);
			pen.setColor(d->outerColor);
			painter.setPen(pen);
			
			//painter.drawRect(outerRect);
			painter.drawArc(outerRect,(-halfSpanAngle+90)*16,d->spanAngle*16);
			
			pen.setColor(d->innerColor);
			painter.setPen(pen);
			painter.drawArc(inRect,(-halfSpanAngle+90)*16,d->spanAngle*16);
			painter.drawLines((CLineF*)lines.data(),lines.size());
		}
	}
}

static inline void DrawHandle(CDialPrivate* d)
{
	if(d)
	{
		CRect bgRect=CRect(CPoint(0,0),d->size);
		int square=mMin(d->size.width(),d->size.height());
		CRect outerRect=CRect(CPoint(0,0),CSize(square,square));
		CMargin outerMgn=d->mgn+d->penWidth+1;
			
		CPoint cp=bgRect.center();
		CPoint op=outerRect.center();
		outerRect.translate(cp-op);
		outerRect-=outerMgn;
		d->spanAngle=mMin(360.0,mMax(0.0,d->spanAngle));
		d->value=mMin(d->max,mMax(d->min,d->value));
		float angle=0;
		if((d->max-d->min)>0)
			angle=d->value/(d->max-d->min)*d->spanAngle;
		CRect inRect=CRect(CPoint(0,0),outerRect.size()*4/5);
		CPoint ip=inRect.center();
		inRect.translate(cp-ip);
		float scaleAlngle=d->spanAngle/d->scaleDiv;
		float halfSpanAngle=d->spanAngle/2.0;
		int len=(outerRect.width()-inRect.width())/4;
		CRect handleRect=inRect+CMargin(len,len,len,len);
		
		CPainter painter(&d->image);
		painter.setRenderHints(CPainter::Antialiasing,true);
		CPen pen;
		CBrush brush(Gemini::dark_blue_level0);
		pen.setBrush(brush);
		painter.setBrush(brush);
		painter.setPen(pen);	
		CPainterPath path;
		
		CRect ellRect=CRect(handleRect.topLeft(),CSize(len*3,len*3));
		ellRect.translate(handleRect.center()-ellRect.center());
		path.addEllipse(ellRect);
		CRect arcRect=CRect(handleRect.topLeft(),CSize(4*len,4*len));
		arcRect.translate(handleRect.center()-arcRect.center());
		path.arcMoveTo(arcRect,-30);
		CPoint spt=path.currentPosition().toPoint();
		path.arcTo(arcRect,-30,61);
		path.lineTo(handleRect.right(),handleRect.center().y());
		path.lineTo(spt);
		painter.translate(ellRect.center());
		if(d->clockwise==Clockwise)
		{
			painter.rotate(-halfSpanAngle-90+angle);
			
		}
		else
		{
			painter.rotate(halfSpanAngle-90-angle);	
		}
		painter.translate(-ellRect.center());
		
		painter.drawPath(path);	
	}
}

static inline void DrawAcceDial(CDialPrivate* d)	
{
	if(d)
	{
		d->spanAngle=mMin(360.0,mMax(0.0,d->spanAngle));
		d->value=mMin(d->max,mMax(d->min,d->value));
		
		
		float startAngle=(360.0-d->spanAngle)/2;
		
		CPainter painter((CPaintDevice*)&d->image);
		
		int square=mMin(d->size.width(),d->size.height());
		CRect r=CRect(CPoint(0,0),CSize(square,square));
		r-=CMargin(2,2,4,4);
		int space=r.width()/10;
		CRect center=r-CMargin(space,space,space+2,space+2);
		float targetAngle=0;
		float stepValue=0;
		if((d->max-d->min)>0)
		{
			targetAngle=d->value/(d->max-d->min)*d->spanAngle;
			stepValue=d->value/(d->max-d->min)*center.height();
		}
		
		
		painter.setPen(CPen(d->innerColor));
		painter.setRenderHints(CPainter::Antialiasing,true);
		painter.save();
		painter.translate(r.center());
		int div=r.width()/2;
		int scaleHandleLen=r.width()/16;
		float A=center.width()/16;
		float B=center.width()/18;
		double rotateAngle=d->spanAngle/div;
		if(d->clockwise==Clockwise)
		{
			painter.rotate(startAngle);
		}
		else
		{
			painter.rotate(startAngle+d->spanAngle);
		}
		
		float hasDraw=0;
		for(int i=0; i<div+1; i++)
		{
			if(hasDraw<=targetAngle&&targetAngle!=0){
				painter.setPen(CPen(d->innerColor,2));
			}
			else
				 painter.setPen(CPen(d->outerColor,2));

			painter.drawLine(CLineF(0,r.height()/2.0,0,r.height()/2.0-scaleHandleLen));
			if(d->clockwise==Clockwise)
				painter.rotate(rotateAngle);
			else
				painter.rotate(-rotateAngle);
			hasDraw+=rotateAngle;
		}
		 painter.restore();

		
		if(targetAngle)
		{
			 //draw water
			 double mCycleFactorW=(double)(2*M_PI/center.width());
			 CVector<double> firstWaterLine;
			 CVector<double> secondWaterLine;
			 
			 for(int i=0; i<center.width();++i)
			 {
				 firstWaterLine<<((double)(B*mSin(mCycleFactorW*i+d->offset)));
				 secondWaterLine<<((double)(A*mSin(mCycleFactorW*i+10+d->offset)));
			 }
			 d->offset++;
			 CPainterPath path;
			 int len=center.width();
			 double clipRadius=len/2.0;
			 path.addEllipse(center);
			 painter.setClipPath(path);
			 painter.translate(center.x(),center.bottom()-stepValue);
			 for(int i=0; i<center.width();++i)
			 {
				 painter.drawLine(i,firstWaterLine[i], i, center.width());
				 painter.drawLine(i,secondWaterLine[i], i, center.width());
			 }
		}
	}
}

void    CtmDial::Show()
{

	if(IsTransparentChanged(d)||IsNeedResize(d)||IsSpanAngleChanged(d)||IsClockwiseChanged(d)
		||d->wOldDialStyle!=d->wDialStyle)
	{
		d->wOldDialStyle=d->wDialStyle;
		IsSpanAngleChanged(d);
		IsClockwiseChanged(d);
		if(IsNeedResize(d))
			Resize(d);
		if(!d->transparent)
		{
			d->fmImage.fill((uint)m_bgc);	
		}
		else
		{
			if(!d->bgImage.isNull())
				d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
		}
		d->properchanged=true;
		d->redraw =true;
	}
	else
	{
		d->redraw =false;
		
	}
	if(d->focus)
	{
		d->properchanged=true;
	}
	if(d->properchanged)
	{
		if(m_bVisible)
		{
			if(d->wDialStyle==WaterBall)
			{
				Clear();
				if(!d->focus)
				{
					GetDBInfo(d);
				}
				DrawAcceDial(d);
				
			}
			else
			{
				DrawPanel(d,m_nBgcColorControl);
				Clear();
				if(!d->focus)
				{
					GetDBInfo(d);
				}
				DrawHandle(d);
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

static inline void Tick(CDialPrivate* d,bool valuences)
{
	if(d)
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
}

//process event

WORD    CtmDial::OnKey(WORD wKey)
{
	WORD			wResult=wKey;
	if(!d)			return wKey;
	if(!d->focus)	return wKey;
	switch (wKey)
	{
		case 0x6D00: 	//down
		case _KEY_DOWNFIELD:
			Tick(d,0);
			Update();
			wResult=_NULL_KEY;
			break;
		case 0x6800:	//up
		case _KEY_UPFIELD:
			Tick(d,1);
			Update();
			wResult=_NULL_KEY;
			break;
		default:
			break;	
	}
	return wResult;	
}


bool CtmDial::TouchAreaChanged(CDialPrivate* d, const CPoint& pt)
{
	bool res=false;
	CPainterPath spanPath;
	CPainterPath leftPath;
	d->value=mMin(d->max,mMax(d->min,d->value));
	float currentAngle=0;
	if((d->max-d->min)>0)
		currentAngle=d->value/(d->max-d->min)*d->spanAngle;
	float halfSpanAngle=d->spanAngle/2;
	CRectF spanRect=CRectF(CPoint(0,0),d->size);
	CPointF cen=spanRect.center();
	CPoint cmp=pt+CPoint(-m_rectClient.left,-m_rectClient.top);
	if(d->clockwise==Clockwise)
	{
		spanPath.arcMoveTo(spanRect,-90);
		CPointF start=spanPath.currentPosition();
		spanPath.arcTo(spanRect,-90,-currentAngle+halfSpanAngle-180);
		spanPath.lineTo(cen);
		spanPath.lineTo(start);
		
		leftPath.arcMoveTo(spanRect,-90);
		leftPath.arcTo(spanRect,-90,currentAngle+180+halfSpanAngle);
		leftPath.lineTo(cen);
		leftPath.lineTo(start);
	}
	else
	{
		spanPath.arcMoveTo(spanRect,-90);
		CPointF start=spanPath.currentPosition();
		spanPath.arcTo(spanRect,-90,currentAngle+180-halfSpanAngle);
		spanPath.lineTo(cen);
		spanPath.lineTo(start);
		
		leftPath.arcMoveTo(spanRect,-90);
		leftPath.arcTo(spanRect,-90,currentAngle-halfSpanAngle-180);
		leftPath.lineTo(cen);
		leftPath.lineTo(start);	
	}
	
	if(spanPath.contains(cmp))
	{
		Tick(d,0);
		res=true;
	}
	else if(leftPath.contains(cmp))
	{
		Tick(d,1);
		res=true;	
	}
	return res;	
}

void	CtmDial::WndProc(int message, WPARAM wParam, LPARAM lParam)
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



bool CtmDial::IsTransparentChanged(CDialPrivate* d)
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

bool CtmDial::IsClockwiseChanged(CDialPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->clockwise!=d->oldClockwise)
		{
			d->oldClockwise=d->clockwise;	
			res=true;
		}
	}
	return res;		
}

bool CtmDial::IsSpanAngleChanged(CDialPrivate* d)
{
	bool res=false;
	if(d)
	{
		if(d->spanAngle!=d->oldSpanAngle)
		{
			d->oldSpanAngle=d->spanAngle;	
			res=true;
		}
	}
	return res;	
}

void CtmDial::GetDBInfo(CDialPrivate* d)
{
	if(d)
	{
		if(dbid)
		{
			DBVIEWVALUE dbViewValue;
			dbViewValue =GetDBValue(dbid);
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


void CtmDial::SyncDBValue(CDialPrivate* d)
{
	if(d)
	{
		int64 llValue=mRound(d->value*GetDecimalPowerVal(d->precision));
		int64 llOldValue=mRound(d->oldValue*GetDecimalPowerVal(d->precision));
		if(llValue!=llOldValue)
		{
			if(dbid!=NULL)
			{
				if(!IsOverLimited(d))
					SetDBValue(dbid,llValue);
			}
		}
	}
}

bool CtmDial::IsOverLimited(CDialPrivate* d)
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

void CtmDial::ResetPrompt(CDialPrivate* d)
{
	if(d)
	{
		if(pszPromptID)
			pszPrompt=g_MultiLanguage[pszPromptID];
		else
			pszPrompt=pszPromptLan;
	}
}

void CtmDial::Prepare(CDialPrivate* d)
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
			if(dbid)
			{
				lDataID=g_pDatabase->GetDataID(dbid);
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









	