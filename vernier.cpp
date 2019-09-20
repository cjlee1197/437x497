/*Created by J.Wong 2017/04/20*/
#include "vernier_p.h"
#include "drawtext.h"
#include "vernier.h"
#include "utils.h"

CVernier::CVernier(CAxisRect* axisRect):
mAxisRect(axisRect),
mVernierPen(CPen(0x60000000,7,Gemini::SolidLine)),
mBrush(0x50acacac),
mBorderPen(0xff9a9a9a)
{
	mFont.width=16;
	mFont.height=16;
	mFont.color=Gemini::red;
	mType=vtSingle;
	mOrientation=Gemini::Horizontal;
	
}

CVernier::~CVernier()
{
	
}

void 	CVernier::setAxisRect(CAxisRect* axisRect)
{
	if(mAxisRect)
		mAxisRect=axisRect;
}

std::string CVernier::getTickLabel(CPlotAxis::AxisType type)const
{
	if(mAxisRect)
	{
		CPlotAxis* axis=mAxisRect->axis(type);
		if(!axis) return std::string();
		switch(type)
		{
			case CPlotAxis::atLeft:
				return axis->ticker()->getTickLabel(value1, axis->numberFormat()[0], axis->numberPrecision());
			case CPlotAxis::atTop:
				return axis->ticker()->getTickLabel(value2, axis->numberFormat()[0], axis->numberPrecision());
			case CPlotAxis::atRight:
				return axis->ticker()->getTickLabel(value2, axis->numberFormat()[0], axis->numberPrecision());
			case CPlotAxis::atBottom:
				return axis->ticker()->getTickLabel(value1, axis->numberFormat()[0], axis->numberPrecision());
		}
		
	}
}

CAxisPainterPrivate::TickLabelData CVernier::getTickLabelData(CPlotAxis::AxisType type,const PlotFont& font, const std::string& text)const
{
	CAxisPainterPrivate::TickLabelData result;
	bool useBeautifulPowers = false;
	bool numberMultiplyCross= false;
	int ePos=-1;
	int eLast=-1;
	if(!mAxisRect) return result;
	CPlotAxis* axis=mAxisRect->axis(type);
	
	if(!axis) return result;
	bool abbreviateDecimalPowers=axis->scaleType()==CPlotAxis::Logarithmic;
	if(axis->numberBeautifulPowers())
	{
		ePos = text.find_first_of('e');
		if(ePos>0&&isdigit(text.at(ePos-1)))
		{
			eLast=ePos;
			while(eLast+1<text.size()&&(text.at(eLast+1)=='+'||text.at(eLast+1)=='-'||isdigit(text.at(eLast+1))))
				++eLast;
			if(eLast>ePos)
				useBeautifulPowers=true;
		}
	}
	result.baseFont=font;
	if(useBeautifulPowers)
	{
		result.basePart=text.substr(0,ePos);
		result.suffixPart=text.substr(eLast+1,text.size());
		if(abbreviateDecimalPowers&&result.basePart=="1")
			result.basePart="10";
		else
		{
			result.basePart+=(numberMultiplyCross? std::string(1,'*'):std::string(1,'*'))+std::string("10");
		}
		result.expPart=text.substr(ePos+1,eLast-ePos);
		while(result.expPart.length()>2&&result.expPart.at(1)=='0')
			result.expPart.erase(1,1);
		if(!result.expPart.empty()&&result.expPart.at(0)=='+')
			result.expPart.erase(0,1);
		result.expFont=font;
		result.expFont.width*=0.65;
		result.expFont.height*=0.65;
				
		result.expBounds=CRect(0,0,(result.expFont.width*result.expPart.length()>>1)+1,result.expFont.height+2);
		result.baseBounds=CRect(0,0,(result.baseFont.width*result.basePart.length()>>1)+1, result.baseFont.height+2);
		if(!result.suffixPart.empty())
			result.suffixBounds=CRect(0,0,(result.baseFont.width*result.basePart.length()>>1)+1, result.baseFont.height+2);
		result.totalBounds=result.baseBounds.adjusted(0,0,result.expBounds.width()+result.suffixBounds.width()+2,0);
	}
	else
	{
		result.basePart=text;
		result.totalBounds=CRect(0,0,(result.baseFont.width*result.basePart.length()>>1)+1,result.baseFont.height+2);
	}
	result.totalBounds.moveTopLeft(CPoint(0,0));
	result.rotatedTotalBounds=result.totalBounds;	
	return result;
}


void CVernier::placeTickLabel(CPlotAxis::AxisType type,CPlotPainter* painter, double position, int distanceToAxis, const std::string& text, CSize* tickLabelsSize)
{
	if(!mAxisRect) return;
	CAxisPainterPrivate::TickLabelData data=getTickLabelData(type, mFont, text);
	CRect boundsRect=CRect(CPoint(0,0),data.totalBounds.size());
	CRect rect=mAxisRect->rect();
	switch(type)
	{
		case CPlotAxis::atLeft:
			boundsRect.translate(mSinglePixelPoint.x()+6,rect.bottom()-data.totalBounds.size().height()-2-distanceToAxis);
			if(boundsRect.right()>=rect.right())
			{
				boundsRect.translate(-data.totalBounds.size().width()-12,0);
			}
			
			break;
		case CPlotAxis::atRight:
			boundsRect.translate(mDoublePixelPoint.x()+6,rect.y()+2);
			if(boundsRect.right()>=rect.right())
			{
				boundsRect.translate(-data.totalBounds.size().width()-12,0);
			}
			break;
		case CPlotAxis::atTop:
			boundsRect.translate(rect.right()-data.totalBounds.size().width()-2,mDoublePixelPoint.y()+6);
			if(boundsRect.bottom()>=rect.bottom())
			{
				boundsRect.translate(0,-data.totalBounds.size().height()-12);
			}
			break;
		case CPlotAxis::atBottom:
			boundsRect.translate(rect.left()+2+distanceToAxis,mSinglePixelPoint.y()+6);
			if(boundsRect.bottom()>=rect.bottom())
			{
				boundsRect.translate(0,-data.totalBounds.size().height()-12);
			}
			break;
		default:
			break;
	}
	if(painter)
	{
		
		painter->save();
		painter->setPen(mBorderPen);
		painter->setBrush(mBrush);
		painter->drawRect(boundsRect);
		painter->restore();
		CImage image=((CImage*)painter->device())->copy(boundsRect);
		CPlotPainter labelTickPainter(&image);
		drawTickLabel(&labelTickPainter,0,0,data);
		CTransform oldTransform=painter->transform();
		painter->translate(boundsRect.topLeft()); 
		painter->setRenderHint(CPainter::SmoothPixmapTransform);
		painter->drawImage(CPoint(0,0), image);
		painter->setTransform(oldTransform);
	}
}

void CVernier::drawTickLabel(CPlotPainter* painter, double x, double y, const CAxisPainterPrivate::TickLabelData& labelData)const
{
	if(g_pDrawText)
	{
		if(!labelData.expPart.empty())
		{
			g_pDrawText->SetFontSize(labelData.baseFont.width,labelData.baseFont.height,0x01);
			g_pDrawText->SetFontColor(labelData.baseFont.color);
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)labelData.basePart.c_str(),CRect(CPoint(0,0),labelData.totalBounds.size()),labelData.basePart.size());
			g_pDrawText->SetFontSize(labelData.expFont.width,labelData.expFont.height,0x01);
			g_pDrawText->SetFontColor(labelData.expFont.color);
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)labelData.expPart.c_str(),CRect(CPoint(labelData.baseBounds.width()+1,0),labelData.totalBounds.size()),labelData.expPart.size());

		}
		else
		{
			g_pDrawText->SetFontSize(labelData.baseFont.width,labelData.baseFont.height,0x01);
			g_pDrawText->SetFontColor(labelData.baseFont.color);
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)labelData.basePart.c_str(),CRect(CPoint(0,0),labelData.totalBounds.size()),labelData.basePart.size());
		}			
	
	}
}

void CVernier::setBorderPen(const CPen& pen)
{
	mBorderPen=pen;
}

void CVernier::setBrush(const CBrush& brush)
{
	mBrush=brush;
}

void CVernier::setFont(const PlotFont& font)
{
	mFont=font;
}

void CVernier::setVernierPen(const CPen& pen)
{
	mVernierPen=pen;
}

void CVernier::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aeOther);
}

void CVernier::draw(CPlotPainter* painter)
{
	if(!mVisible)	return ;
	if(!mAxisRect)	return ;
	if(!painter)	return ;
	CRect rect=mAxisRect->rect();
	painter->save();
	if(mType==vtSingle)
	{
		painter->setPen(mVernierPen);
		if(mOrientation==Gemini::Horizontal)
		{
			painter->drawLine(CPointF(mSinglePixelPoint.x()+0.5f,rect.y()-2),CPointF(mSinglePixelPoint.x()+0.5f, rect.bottom()+2));
		}
		else
		{
			painter->drawLine(CPointF(rect.x()-2,mSinglePixelPoint.y()-0.5f),CPointF(rect.right()+2,mSinglePixelPoint.y()-0.5f));
		}
	}
	else
	{
		painter->setAntialiasing(true);
		painter->setBrush(CBrush(mVernierPen.color()));
		painter->setPen(mVernierPen);
			
		CLineEnding lineEnding;
		lineEnding.setStyle(CLineEnding::FlatArrow);
		if(mOrientation==Gemini::Horizontal)
		{
			lineEnding.setInverted(true);
			int y=rect.y()+mAxisRect->axis(CPlotAxis::atTop)->tickLengthIn()+2;
			CLineF baseLine=CLineF(CPointF(mDoublePixelPoint.x()+0.5f, y+lineEnding.length()),CPointF(mDoublePixelPoint.x()+0.5f,y));
			CVector2D baseLineVector(baseLine.dx(), baseLine.dy());
			//draw
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength()*(lineEnding.inverted()?-1:1),-baseLineVector);
			
			y=rect.bottom()-mAxisRect->axis(CPlotAxis::atBottom)->tickLengthIn()-2;
			baseLine=CLineF(CPointF(mSinglePixelPoint.x()+0.5f, y-lineEnding.length()),CPointF(mSinglePixelPoint.x()+0.5f,y));
			baseLineVector=CVector2D(baseLine.dx(), baseLine.dy());
			//draw
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength()*(lineEnding.inverted()?-1:1),-baseLineVector);
		}
		else
		{
			lineEnding.setInverted(true);
			int x=rect.x()+mAxisRect->axis(CPlotAxis::atLeft)->tickLengthIn()+2;
			CLineF baseLine=CLineF(CPointF(x+lineEnding.length(),mSinglePixelPoint.y()-0.5f),CPointF(x,mSinglePixelPoint.y()-0.5f));
			CVector2D baseLineVector(baseLine.dx(), baseLine.dy());
			//draw
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength()*(lineEnding.inverted()?-1:1),-baseLineVector);
			
			x=rect.right()-mAxisRect->axis(CPlotAxis::atRight)->tickLengthIn()-2;
			baseLine=CLineF(CPointF(x-lineEnding.length(),mDoublePixelPoint.y()-0.5f),CPointF(x,mDoublePixelPoint.y()-0.5f));
			baseLineVector=CVector2D(baseLine.dx(), baseLine.dy());
			//draw
			lineEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lineEnding.realLength()*(lineEnding.inverted()?-1:1),-baseLineVector);
		}
	}
	painter->restore();
	
	CPlotAxis::AxisType singleType[2]={
		CPlotAxis::atLeft,
		CPlotAxis::atBottom
	};
	
	CPlotAxis::AxisType doubleType[2]={
		CPlotAxis::atRight,
		CPlotAxis::atTop
	};
	
	
	
	//draw textLabel
	if(vtSingle==mType)
	{
		
		if(mOrientation==Gemini::Horizontal)
		{
			std::string label=getTickLabel(singleType[0]);
			CSize labelSize;
			placeTickLabel(singleType[0],painter,0,(rect.height()-mFont.height)/2.0+0.5f,label,&labelSize);
		}
		else
		{
			std::string label=getTickLabel(singleType[1]);
			CSize labelSize;
			placeTickLabel(singleType[1],painter,0,(rect.width()-mFont.height*label.size())/2.0+0.5f,label,&labelSize);
		}
	}
	else
	{
		if(mOrientation==Gemini::Horizontal)
		{
			std::string label=getTickLabel(singleType[0]);
			CSize labelSize;
			placeTickLabel(singleType[0],painter,0,0,label,&labelSize);
			
			label=getTickLabel(doubleType[0]);
			placeTickLabel(doubleType[0],painter,0,0,label,&labelSize);
		}
		else
		{
			std::string label=getTickLabel(singleType[1]);
			CSize labelSize;
			placeTickLabel(singleType[1],painter,0,0,label,&labelSize);
			
			label=getTickLabel(doubleType[1]);
			placeTickLabel(doubleType[1],painter,0,0,label,&labelSize);
		}
	}
	
}


void CVernier::setOrientation(WORD orientation)
{
	mOrientation=orientation;
}

void CVernier::setVernierType(WORD type)
{
	mType=type;
}

void CVernier::setSinglePixelPoint(const CPoint& point)
{
	mSinglePixelPoint=point;
	if(mOrientation==Gemini::Horizontal)
	{
		value1=mAxisRect->axis(CPlotAxis::atBottom)->pixelToCoord(point.x());
	}
	else
	{
		value1=mAxisRect->axis(CPlotAxis::atLeft)->pixelToCoord(point.y());
	}
}

void CVernier::setDoublePixelPoint(const CPoint& point)
{
	if(mType==vtDouble)
	{
		mDoublePixelPoint=point;
		if(mOrientation==Gemini::Horizontal)
		{
			value2=mAxisRect->axis(CPlotAxis::atTop)->pixelToCoord(point.x());
		}
		else
		{
			value2=mAxisRect->axis(CPlotAxis::atRight)->pixelToCoord(point.y());
		}
	}
//	std::cout<<"Current Type is vtSingle"<<std::endl;
}

void CVernier::setSingleValue(double value)
{
	value1=value;
	CPoint pt;
	if(mOrientation==Gemini::Horizontal)
	{
		pt.setY(mAxisRect->rect().bottom());
		pt.setX(mAxisRect->axis(CPlotAxis::atBottom)->coordToPixel(value));
	}
	else
	{
		pt.setX(mAxisRect->rect().left());
		pt.setY(mAxisRect->axis(CPlotAxis::atLeft)->coordToPixel(value));
	}
	mSinglePixelPoint=pt;
}
void CVernier::setDoubleValue(double value)
{
	
	if(mType==vtDouble)
	{
		value2=value;
		CPoint pt;
		if(mOrientation==Gemini::Horizontal)
		{
			pt.setY(mAxisRect->rect().top());
			pt.setX(mAxisRect->axis(CPlotAxis::atTop)->coordToPixel(value));
		}
		else
		{
			pt.setX(mAxisRect->rect().right());
			pt.setY(mAxisRect->axis(CPlotAxis::atRight)->coordToPixel(value));
		}
		mDoublePixelPoint=pt;
	}
}

double CVernier::doubleValue()const
{
	if(mType==vtDouble)
	{
		return value2;
	}
//	std::cout<<"Current Type is vtSingle, doubleValue was not founded"<<std::endl;
	return 0;
}




CVernierPrivate::CVernierPrivate()
{
	init();
}

CVernierPrivate::~CVernierPrivate()
{
	destroy();
}


void CVernierPrivate::init()
{
	properties.all = 0;
	transparent = false;
	touched = false;
	focus = false;
	x = 0;
	y = 0;
	size=CSize(240,100);
	marginLeft=20;
	marginRight=20;
	marginTop=20;
	marginBottom=20;
	
	value= CRange::minRange;
	exValue=CRange::minRange;
	
	rangeLower=0;
	rangeUpper=10;
	
	exRangeLower=0;
	exRangeUpper=10;
	
	vernierType=CVernier::vtSingle;
	orientation=Gemini::Horizontal;
	oldBgc=0;
	
	axisRect=new CAxisRect;
	axisRect->setRect(CRect(CPoint(0,0),size)-CMargin(marginLeft, marginTop,marginRight, marginBottom));
	
	vernier=new CVernier(axisRect);
	
	borderColor=0xff9a9a9a;
	borderWidth=1;
	textBackground=0x50acacac;
		
	vernierColor=0x60000000;
	vernierWidth=4;
	
	fontWidth=16;
	fontHeight=16;
	fontColor=Gemini::red;
	
	rangeReversed=false;
	tickCount=6;
	tickLabelPadding=2;
	axisColor=Gemini::black;
	axisWidth=1;
	tickColor=Gemini::black;
	tickWidth=1;
	subTickColor=Gemini::black;
	subTickWidth=1;
	tickLabelFontWidth=16;
	tickLabelFontHeight=16;
	tickLabelFontColor=Gemini::black;
	tickLengthIn=6;
	subTickLengthIn=4;
	
	numberPrecision=1;
	exNumberPrecision=1;
	
}

void CVernierPrivate::destroy()
{
	delete axisRect;
	delete vernier;
}


IMPLEMENT_DYNCREATE(CtmVernier, CtmWnd)

CtmVernier::CtmVernier()
{
	strcpy(TYPEName, "CtmVernier");
	d=new CVernierPrivate();
	
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);
		if(d->axisRect)
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setTickLabelSide(CPlotAxis::lsInSide);
			d->axisRect->axis(CPlotAxis::atTop)->setTickLabelSide(CPlotAxis::lsInSide);
			d->axisRect->axis(CPlotAxis::atRight)->setTickLabelSide(CPlotAxis::lsInSide);
			d->axisRect->axis(CPlotAxis::atBottom)->setTickLabelSide(CPlotAxis::lsInSide);
		}
	}
	
	transparent=FALSE;
	numberFormat=NULL;
	
	memset(dbid,0,sizeof(dbid));
	memset(maxid,0,sizeof(maxid));
	memset(minid,0,sizeof(minid));
	maxdb[0]=TRUE;
	maxdb[1]=TRUE;
	mindb[0]=TRUE;
	mindb[1]=TRUE;
}

CtmVernier::~CtmVernier()
{
	delete d;
}


static inline void drawBase(CVernierPrivate* d)
{
	if(d)
	{
		CPlotPainter painter(&d->fmImage);
		if(d->axisRect->axis(CPlotAxis::atLeft)->visible())
		{
			d->axisRect->axis(CPlotAxis::atLeft)->draw(&painter);
		}
		if(d->axisRect->axis(CPlotAxis::atTop)->visible())
		{
			d->axisRect->axis(CPlotAxis::atTop)->draw(&painter);
		}
		if(d->axisRect->axis(CPlotAxis::atRight)->visible())
		{
			d->axisRect->axis(CPlotAxis::atRight)->draw(&painter);
		}
		if(d->axisRect->axis(CPlotAxis::atBottom)->visible())
		{
			d->axisRect->axis(CPlotAxis::atBottom)->draw(&painter);
		}
	}
}
#include "datagrid_p.h"
static inline void drawRuler(CVernierPrivate* d)
{
	if(d)
	{
		CPlotPainter painter(&d->image);
		d->vernier->draw(&painter);
		
	}
}

void CtmVernier::ClearPropertiesBit()
{
	//清除所有属性检查位
	if(d)
	{
		d->properties.all=0;
	}
}
	
static void copyBackground(CVernierPrivate* d, uint bgc)
{
	if(d)
	{
		if(d->transparent)
			{
				if(!d->bgImage.isNull())
					d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
			}
			else
			{
				d->fmImage.fill((uint)bgc);
			}
	}
}		
	
void    CtmVernier::Show()
{
	if(d)
	{
		if(IsTransParentChanged(d)||IsNeedResize(d))
		{
			if(IsNeedResize(d))
				Resize(d);
			
			d->oldBgc=m_bgc;
			
			if(d->transparent)
			{
				if(!d->bgImage.isNull())
					d->fmImage=d->bgImage.copy(CRect(CPoint(0,0),d->size));	
			}
			else
			{
				d->fmImage.fill((uint)m_bgc);
			}
			d->properties.all=0x00FF;
		}
		if(m_bVisible)
		{
			CheckPropertiesChanged();
			
			if(m_bgc!=d->oldBgc)
			{
				d->oldBgc=m_bgc;
				d->fmImage.fill((uint)m_bgc); 
				d->properties.all=0x00FF;
			}
			
			if(d->properties.all!=0)
			{
				if((d->properties.all&0x03))
				{
					copyBackground(d, m_bgc);
					drawBase(d);
				}

				if(d->properties.all&0x0F)
				{
					Clear();
					drawRuler(d); 
				}
				_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
				_PutImage(0,0,d->image);
					
			}
			ClearPropertiesBit();
		}
		
	}
}

void    CtmVernier::Update()
{
	Show();
}

void    CtmVernier::Clear()
{
	if(d)
	{
		
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
	}
}

 static inline void prepareBase(CVernierPrivate* d)
{
	if(d)
	{
		d->vernier->setVernierType(d->vernierType);
		d->vernier->setOrientation(d->orientation+1);
		if((d->orientation+1)==Gemini::Vertical)
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setVisible(true);
			d->axisRect->axis(CPlotAxis::atRight)->setVisible(true);
			d->axisRect->axis(CPlotAxis::atTop)->setVisible(false);
			d->axisRect->axis(CPlotAxis::atBottom)->setVisible(false);
		}
		else
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setVisible(false);
			d->axisRect->axis(CPlotAxis::atRight)->setVisible(false);
			d->axisRect->axis(CPlotAxis::atTop)->setVisible(true);
			d->axisRect->axis(CPlotAxis::atBottom)->setVisible(true);
		}
	}
}

BOOL 	CtmVernier::CreateA()
{
	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
	prepareBase(d);
	return true;
}


void 	CtmVernier::SetPropertys()
{
	CtmWnd::SetPropertys();
	SetPropInfo((char*)"margin-left",		tmINT, &d->marginLeft);
	SetPropInfo((char*)"margin-right", 		tmINT, &d->marginRight);
	SetPropInfo((char*)"margin-top",		tmINT, &d->marginTop);
	SetPropInfo((char*)"margin-bottom", 	tmINT, &d->marginBottom);
	SetPropInfo((char*)"transparent", 		tmBOOL,&d->transparent);
	SetPropInfo((char*)"touched", 			tmBOOL,&d->touched);
	
	//ruler:
	SetPropInfo((char*)"vernier-type", 		tmWORD, &d->vernierType);
	SetPropInfo((char*)"orientation", 		tmWORD, &d->orientation);
	SetPropInfo((char*)"border-color",		tmINT, 	&d->borderColor);
	SetPropInfo((char*)"border-width",		tmINT,  &d->borderWidth);
	SetPropInfo((char*)"text-background", 	tmINT,&d->textBackground);
	SetPropInfo((char*)"vernier-color", 	tmINT, 	&d->vernierColor);
	SetPropInfo((char*)"vernier-width", 	tmINT, 	&d->vernierWidth);
	
	SetPropInfo((char*)"font-width", 		tmINT, &d->fontWidth);
	SetPropInfo((char*)"font-height", 		tmINT, &d->fontHeight);
	SetPropInfo((char*)"font-color", 		tmINT, &d->fontColor);
	SetPropInfo((char*)"value", 			tmDOUBLE, &d->value);
	SetPropInfo((char*)"exvalue", 			tmDOUBLE, &d->exValue);
	
	//axis:
	SetPropInfo((char*)"range-lower", 		tmDOUBLE, &d->rangeLower);
	SetPropInfo((char*)"range-upper", 		tmDOUBLE, &d->rangeUpper);
	SetPropInfo((char*)"exrange-lower", 	tmDOUBLE, &d->exRangeLower);
	SetPropInfo((char*)"exrange-upper", 	tmDOUBLE, &d->exRangeUpper);
	SetPropInfo((char*)"range-reversed", 	tmBOOL, &d->rangeReversed);
	SetPropInfo((char*)"tick-count", 		tmINT, &d->tickCount);
	SetPropInfo((char*)"ticklabel-padding", tmINT, &d->tickLabelPadding);
	SetPropInfo((char*)"ticklabel-fontwidth", 	tmINT, &d->tickLabelFontWidth);
	SetPropInfo((char*)"ticklabel-fontheight", 	tmINT, &d->tickLabelFontHeight);
	SetPropInfo((char*)"ticklabel-fontcolor", 	tmINT, &d->tickLabelFontColor);
	SetPropInfo((char*)"axis-color", 		tmINT, &d->axisColor);
	SetPropInfo((char*)"axis-width", 		tmINT, &d->axisWidth);
	SetPropInfo((char*)"tick-color", 		tmINT, &d->tickColor);
	SetPropInfo((char*)"tick-width", 		tmINT, &d->tickWidth);
	SetPropInfo((char*)"subtick-color", 	tmINT, &d->subTickColor);
	SetPropInfo((char*)"subtick-width", 	tmINT, &d->subTickWidth);
	SetPropInfo((char*)"precision", 		tmINT, &d->numberPrecision);
	SetPropInfo((char*)"exprecision",		tmINT, &d->exNumberPrecision);
	SetPropInfo((char*)"number-format", 	tmSTR, &numberFormat);
	SetPropInfo((char*)"tick-length", 		tmINT, &d->tickLengthIn);
	SetPropInfo((char*)"subtick-length", 	tmINT, &d->subTickLengthIn);
	
	
	SetPropInfo((char*)"dbid0", 	tmSTR, 	&dbid[0]);
	SetPropInfo((char*)"dbid1", 	tmSTR, 	&dbid[1]);
	SetPropInfo((char*)"minid0",	tmSTR,	&minid[0]);
	SetPropInfo((char*)"minid1",	tmSTR,	&minid[1]);
	SetPropInfo((char*)"maxid0",	tmSTR, 	&maxid[0]);
	SetPropInfo((char*)"maxid1",	tmSTR, 	&maxid[1]);
	SetPropInfo((char*)"mindb0",	tmBOOL, &mindb[0]);
	SetPropInfo((char*)"mindb1",	tmBOOL, &mindb[1]);
	SetPropInfo((char*)"maxdb0",	tmBOOL, &maxdb[0]);
	SetPropInfo((char*)"maxdb1",	tmBOOL, &maxdb[1]);
	
}


void CtmVernier::GetDBInfo(CVernierPrivate* d)
{
	if(d)
	{
		if(dbid[0])
		{
			DBVIEWVALUE dbViewValue;
			dbViewValue =GetDBValue(dbid[0]);
			int muti=GetDecimalPowerVal(d->numberPrecision);
			d->value =(double)dbViewValue.lValue/muti;
			if(maxdb[0])
			{
				d->rangeUpper = (double)dbViewValue.lMaxValue/muti;
			}
			else
			{
				if(maxid[0])
				{
					
					d->rangeUpper=(double)GetDBValue(maxid[0]).lValue/muti;
				}
			}
			
			if(mindb[0])
			{
				d->rangeLower=(double)dbViewValue.lMinValue/muti;
			}
			else
			{
				if(minid[0])
				{
					d->rangeLower=(double)GetDBValue(minid[0]).lValue/muti;
				}
			}
		}
		if(d->vernier->vernierType()==CVernier::vtDouble)
		{
			if(dbid[1])
			{
				DBVIEWVALUE dbViewValue;
				dbViewValue =GetDBValue(dbid[1]);
				int muti=GetDecimalPowerVal(d->exNumberPrecision);
				d->exValue =(double)dbViewValue.lValue/muti;
				if(maxdb[1])
				{
					d->exRangeUpper = (double)dbViewValue.lMaxValue/muti;
				}
				else
				{
					if(maxid[1])
					{
						
						d->exRangeUpper=(double)GetDBValue(maxid[1]).lValue/muti;
					}
				}
				
				if(mindb[1])
				{
					d->exRangeLower=(double)dbViewValue.lMinValue/muti;
				}
				else
				{
					if(minid[1])
					{
						d->exRangeLower=(double)GetDBValue(minid[1]).lValue/muti;
					}
				}
			}
		}
	}
}


WORD    CtmVernier::OnKey(WORD wKey)
{
	;
}
		
void   	CtmVernier::OnGetFocus()
{
	if(d)
	{
		d->focus = true;
		CtmWnd::DrawGetFocus();
	}
}

void    CtmVernier::OnLoseFocus()
{
	if(d)
	{
		d->focus=false;
		CtmWnd::DrawLoseFocus();
	}
}
		
void	CtmVernier::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	;
}

	
void 	CtmVernier::Resize(CVernierPrivate* d)
{
	if(d)
	{
		d->fmImage=CImage(CSize(GetClientWidth(),GetClientHeight()),CImage::Format_RGB32);
		d->size=CSize(GetClientWidth(),GetClientHeight());
		d->x=m_rectClient.left;
		d->y=m_rectClient.top;
		_SetViewPort(d->x,d->y,d->x+d->size.width(),d->y+d->size.height());
		d->bgImage=d->fmImage.copy(CRect(CPoint(0,0),d->size));
		//获取新位置处的背景色
		if(!d->bgImage.isNull())
			_GetImage(0,0,d->bgImage);
		
		//更新AxisRect图层
		if(d->axisRect)
		{
			
			d->axisRect->setRect(CRect(CPoint(0, 0),d->size)
						-CMargin(d->marginLeft,d->marginTop,d->marginRight, d->marginBottom));
			
		}
		
	}
}

bool	CtmVernier::IsNeedResize(CVernierPrivate* d)
{
	if(d)
	{
		return (CRect(CPoint(m_rectClient.left,m_rectClient.top),CSize(GetClientWidth(),GetClientHeight()))
			!=CRect(CPoint(d->x,d->y),d->size));
	}
	return false;
}

bool 	CtmVernier::IsTransParentChanged(CVernierPrivate* d)
{
	if(d)
	{
		if(d->transparent!=transparent)
		{
			d->transparent=transparent;
			return true;
		}
	}
	return false;
}



//private helper functions: check properties changes
void CtmVernier::SetMargin(int left, int top, int right, int bottom)
{
	CRect rect=CRect(CPoint(0, 0),d->size)-CMargin(left, top, right, bottom);
	if(rect!=d->axisRect->rect())
	{
		d->axisRect->setRect(rect);
		d->properties.changed.margin=1;
	}
}



//ruler:
void CtmVernier::SetVernierType(WORD type)
{
	if(d->vernier->vernierType()!=type)
	{
		d->vernier->setVernierType(type);
		d->properties.changed.rulerBase=1;
	}
}

void CtmVernier::SetOrientation(WORD orientation)
{
	if(d->vernier->orientation()!=orientation)
	{
		d->vernier->setOrientation(orientation);
		d->properties.changed.rulerBase=1;
		if(orientation==Gemini::Vertical)
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setVisible(true);
			d->axisRect->axis(CPlotAxis::atRight)->setVisible(true);
			d->axisRect->axis(CPlotAxis::atTop)->setVisible(false);
			d->axisRect->axis(CPlotAxis::atBottom)->setVisible(false);
		}
		else
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setVisible(false);
			d->axisRect->axis(CPlotAxis::atRight)->setVisible(false);
			d->axisRect->axis(CPlotAxis::atTop)->setVisible(true);
			d->axisRect->axis(CPlotAxis::atBottom)->setVisible(true);
		}
	}
}

void CtmVernier::SetBorderPen(int width, int color)
{
	CPen pen=d->vernier->borderPen();
	pen.setWidth(width);
	pen.setColor(color);
	if(pen!=d->vernier->borderPen())
	{
		d->vernier->setBorderPen(pen);
		d->properties.changed.rulerBase=1;
	}
}


void CtmVernier::SetTextBackground(int color)
{
	CBrush brush=d->vernier->brush();
	if(brush.color()!=color)
	{
		brush.setColor(color);
		d->vernier->setBrush(brush);
		d->properties.changed.rulerBase=1;
	}
}

void CtmVernier::SetVernierPen(int width, int color)
{
	CPen pen=d->vernier->vernierPen();
	pen.setWidth(width);
	pen.setColor(color);
	if(pen!=d->vernier->vernierPen())
	{
		d->vernier->setVernierPen(pen);
		d->properties.changed.rulerBase=1;
	}
}


void CtmVernier::SetFont(int width, int height, int color)
{
	PlotFont font=d->vernier->font();
	if(font.width!=width||font.height!=height||font.color!=color)
	{
		font.width=width;
		font.height=height;
		font.color=color;
		d->vernier->setFont(font);
		d->properties.changed.rulerBase=1;
	}
}

void CtmVernier::SetValue(double value)
{
	if(!fuzzyIsNull(value-d->vernier->singleValue()))
	{
		d->vernier->setSingleValue(value);
		d->properties.changed.valueBase=1;
	}
}

void CtmVernier::SetExValue(double value)
{
	if(!fuzzyIsNull(value-d->vernier->doubleValue()))
	{
		d->vernier->setDoubleValue(value);
		d->properties.changed.valueBase=1;
	}
}

//axis:

void CtmVernier::SetRange(double rangeLower, double rangeUpper)
{

	if(d->axisRect->axis(CPlotAxis::atLeft)->range()!=CRange(rangeLower,rangeUpper))
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setRange(CRange(rangeLower, rangeUpper));
		d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
		
		d->vernier->setSingleValue(d->value);
		
		d->properties.changed.axisBase=1;
	}
	
	if(d->axisRect->axis(CPlotAxis::atBottom)->range()!=CRange(rangeLower,rangeUpper))
	{
		d->axisRect->axis(CPlotAxis::atBottom)->setRange(CRange(rangeLower, rangeUpper));
		d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
		
		d->vernier->setSingleValue(d->value);
		
		d->properties.changed.axisBase=1;
	}
	
	
	if(d->vernier->vernierType()!=CVernier::vtDouble)
	{
		if(d->axisRect->axis(CPlotAxis::atRight)->range()!=CRange(rangeLower,rangeUpper))
		{
			d->axisRect->axis(CPlotAxis::atRight)->setRange(CRange(rangeLower, rangeUpper));
			d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
			
			d->vernier->setSingleValue(d->value);
			
			d->properties.changed.axisBase=1;
		}
		
		if(d->axisRect->axis(CPlotAxis::atTop)->range()!=CRange(rangeLower,rangeUpper))
		{
			d->axisRect->axis(CPlotAxis::atTop)->setRange(CRange(rangeLower, rangeUpper));
			d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
			
			d->vernier->setSingleValue(d->value);
			
			d->properties.changed.axisBase=1;
		}
	}
		
		
}

void CtmVernier::SetExRange(double rangeLower, double rangeUpper)
{
	if(d->vernier->vernierType()==CVernier::vtDouble)
	{
		if(d->axisRect->axis(CPlotAxis::atRight)->range()!=CRange(rangeLower,rangeUpper))
		{
			d->axisRect->axis(CPlotAxis::atRight)->setRange(CRange(rangeLower, rangeUpper));
			d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
			
			d->vernier->setDoubleValue(d->exValue);
			
			d->properties.changed.axisBase=1;
		}
		
		if(d->axisRect->axis(CPlotAxis::atTop)->range()!=CRange(rangeLower,rangeUpper))
		{
			d->axisRect->axis(CPlotAxis::atTop)->setRange(CRange(rangeLower, rangeUpper));
			d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
			
			d->vernier->setDoubleValue(d->exValue);
			
			d->properties.changed.axisBase=1;
		}
	}
}

void CtmVernier::SetRangeReversed(BOOL reversed)
{
	BOOL rangeReversed=d->axisRect->axis(CPlotAxis::atLeft)->rangeReversed();
	if(rangeReversed!=reversed)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setRangeReversed(reversed);
		d->axisRect->axis(CPlotAxis::atTop)->setRangeReversed(reversed);
		d->axisRect->axis(CPlotAxis::atRight)->setRangeReversed(reversed);
		d->axisRect->axis(CPlotAxis::atBottom)->setRangeReversed(reversed);
		d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
		
		d->vernier->setSingleValue(d->value);
		d->vernier->setDoubleValue(d->exValue);
		
		d->properties.changed.axisBase=1;
	}
}

void CtmVernier::SetTickCount(int count)
{
	int tickCount=d->axisRect->axis(CPlotAxis::atLeft)->ticker()->tickCount();
	
	if(tickCount!=count)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->ticker()->setTickCount(count);
		d->axisRect->axis(CPlotAxis::atTop)->ticker()->setTickCount(count);
		d->axisRect->axis(CPlotAxis::atRight)->ticker()->setTickCount(count);
		d->axisRect->axis(CPlotAxis::atBottom)->ticker()->setTickCount(count);
		
		d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
		
		d->vernier->setSingleValue(d->value);
		d->vernier->setDoubleValue(d->exValue);
		
		d->properties.changed.axisBase=1;
	}

}

void CtmVernier::SetTickLabelPadding(int padding)
{
	int labelPadding=d->axisRect->axis(CPlotAxis::atLeft)->tickLabelPadding();
	
	if(labelPadding!=padding)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setTickLabelPadding(padding);
		d->axisRect->axis(CPlotAxis::atTop)->setTickLabelPadding(padding);
		d->axisRect->axis(CPlotAxis::atRight)->setTickLabelPadding(padding);
		d->axisRect->axis(CPlotAxis::atBottom)->setTickLabelPadding(padding);
		d->properties.changed.axisBase=1;
	}
}

void CtmVernier::SetTickLabelFont(int width, int height, int color)
{
	PlotFont font;
	font.width=width;
	font.height=height;
	font.color=color;
	PlotFont labelFont=d->axisRect->axis(CPlotAxis::atLeft)->tickLabelFont();
	
	if(labelFont.width!=width||labelFont.height!=height||labelFont.color!=color)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setTickLabelFont(font);
		d->axisRect->axis(CPlotAxis::atTop)->setTickLabelFont(font);
		d->axisRect->axis(CPlotAxis::atRight)->setTickLabelFont(font);
		d->axisRect->axis(CPlotAxis::atBottom)->setTickLabelFont(font);
		d->properties.changed.axisBase=1;
	}
	
}

void CtmVernier::SetAxisPen(int width, int color)
{
	CPen pen=d->axisRect->axis(CPlotAxis::atLeft)->basePen();
	pen.setColor(color);
	pen.setWidth(width);
	if(pen!=d->axisRect->axis(CPlotAxis::atLeft)->basePen())
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setBasePen(pen);
		d->axisRect->axis(CPlotAxis::atTop)->setBasePen(pen);
		d->axisRect->axis(CPlotAxis::atRight)->setBasePen(pen);
		d->axisRect->axis(CPlotAxis::atBottom)->setBasePen(pen);
		d->properties.changed.axisBase=1;
	}
}

void CtmVernier::SetTickPen(int width, int color)
{
	CPen pen=d->axisRect->axis(CPlotAxis::atLeft)->tickPen();
	pen.setColor(color);
	pen.setWidth(width);
	if(pen!=d->axisRect->axis(CPlotAxis::atLeft)->tickPen())
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setTickPen(pen);
		d->axisRect->axis(CPlotAxis::atTop)->setTickPen(pen);
		d->axisRect->axis(CPlotAxis::atRight)->setTickPen(pen);
		d->axisRect->axis(CPlotAxis::atBottom)->setTickPen(pen);
		d->properties.changed.axisBase=1;
	}
}

void CtmVernier::SetSubTickPen(int width, int color)
{
	CPen pen=d->axisRect->axis(CPlotAxis::atLeft)->subTickPen();
	pen.setColor(color);
	pen.setWidth(width);
	if(pen!=d->axisRect->axis(CPlotAxis::atLeft)->subTickPen())
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setSubTickPen(pen);
		d->axisRect->axis(CPlotAxis::atTop)->setSubTickPen(pen);
		d->axisRect->axis(CPlotAxis::atRight)->setSubTickPen(pen);
		d->axisRect->axis(CPlotAxis::atBottom)->setSubTickPen(pen);
		d->properties.changed.axisBase=1;
	}
}


void CtmVernier::SetNumberPrecision(int precision, int exPrecision)
{
	int numberPrecision=d->axisRect->axis(CPlotAxis::atLeft)->numberPrecision();
	int exNumberprecision=d->axisRect->axis(CPlotAxis::atRight)->numberPrecision();
	if(d->vernier->vernierType()==CVernier::vtSingle)
	{
		if(numberPrecision!=precision)
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setNumberPrecision(precision);
			d->axisRect->axis(CPlotAxis::atTop)->setNumberPrecision(precision);
			d->axisRect->axis(CPlotAxis::atRight)->setNumberPrecision(precision);
			d->axisRect->axis(CPlotAxis::atBottom)->setNumberPrecision(precision);
			
			d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
			d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
			d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
			d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
			d->properties.changed.axisBase=1;
		}
	}
	else
	{
		if(numberPrecision!=precision)
		{
			d->axisRect->axis(CPlotAxis::atLeft)->setNumberPrecision(precision);
			d->axisRect->axis(CPlotAxis::atBottom)->setNumberPrecision(precision);
			d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
			d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
			d->properties.changed.axisBase=1;
		}
		if(exNumberprecision!=exPrecision)
		{
			d->axisRect->axis(CPlotAxis::atRight)->setNumberPrecision(exPrecision);
			d->axisRect->axis(CPlotAxis::atTop)->setNumberPrecision(exPrecision);
			d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
			d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
		}
	}
}

void CtmVernier::SetNumberFormat(std::string format)
{
	std::string numberFormat=d->axisRect->axis(CPlotAxis::atLeft)->numberFormat();
	if(numberFormat!=format)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setNumberFormat(format);
		d->axisRect->axis(CPlotAxis::atTop)->setNumberFormat(format);
		d->axisRect->axis(CPlotAxis::atRight)->setNumberFormat(format);
		d->axisRect->axis(CPlotAxis::atBottom)->setNumberFormat(format);
		
		d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
		
		d->properties.changed.axisBase=1;
	}
}

void CtmVernier::SetTickLength(int length)
{
	int tickLengthIn=d->axisRect->axis(CPlotAxis::atLeft)->tickLengthIn();
	if(tickLengthIn!=length)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setTickLengthIn(length);
		d->axisRect->axis(CPlotAxis::atTop)->setTickLengthIn(length);
		d->axisRect->axis(CPlotAxis::atRight)->setTickLengthIn(length);
		d->axisRect->axis(CPlotAxis::atBottom)->setTickLengthIn(length);
		
		d->properties.changed.axisBase=1;
	}
}

void CtmVernier::SetSubTickLength(int length)
{
	int tickLengthIn=d->axisRect->axis(CPlotAxis::atLeft)->subTickLengthIn();
	if(tickLengthIn!=length)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setSubTickLengthIn(length);
		d->axisRect->axis(CPlotAxis::atTop)->setSubTickLengthIn(length);
		d->axisRect->axis(CPlotAxis::atRight)->setSubTickLengthIn(length);
		d->axisRect->axis(CPlotAxis::atBottom)->setSubTickLengthIn(length);
		
		d->properties.changed.axisBase=1;
	}
}


void CtmVernier::CheckPropertiesChanged()
{
	GetDBInfo(d);
	SetMargin(d->marginLeft, d->marginTop, d->marginRight, d->marginBottom);
	SetVernierType(d->vernierType);
	SetOrientation(d->orientation+1);
	SetBorderPen(d->borderWidth,d->borderColor);
	SetTextBackground(d->textBackground);
	SetVernierPen(d->vernierWidth,d->vernierColor);
	SetFont(d->fontWidth, d->fontHeight, d->fontColor);
	SetRange(d->rangeLower, d->rangeUpper);
	SetExRange(d->exRangeLower, d->exRangeUpper);
	SetRangeReversed(d->rangeReversed);
	SetTickCount(d->tickCount);
	SetTickLabelFont(d->tickLabelFontWidth, d->tickLabelFontHeight, d->tickLabelFontColor);
	SetAxisPen(d->axisWidth, d->axisColor);
	SetTickPen(d->tickWidth, d->tickColor);
	SetSubTickPen(d->subTickWidth, d->subTickColor);
	if(numberFormat)
		SetNumberFormat(numberFormat);
	SetNumberPrecision(d->numberPrecision,d->exNumberPrecision);
	SetTickLength(d->tickLengthIn);
	SetSubTickLength(d->subTickLengthIn);
	SetValue(d->value);
	SetExValue(d->exValue);
}




