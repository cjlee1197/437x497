/*Created by J.Wong 2017/03/13*/
#include "plot_p.h"
#include "plot.h"
#include <math.h>
#include "drawtext.h"

const double CRange::minRange=1e-280;
const double CRange::maxRange=1e250;

CRange::CRange():lower(0),upper(0)
{
	
}

CRange::CRange(double lower, double upper):lower(lower),upper(upper)
{
	normalize();
}

void CRange::expand(const CRange& other)
{
	if(lower>other.lower||isnan(lower))
		lower=other.lower;
	if(upper<other.upper||isnan(upper))
		upper=other.upper;
}

void CRange::expand(double includeCoord)
{
	if(lower>includeCoord||isnan(lower))
		lower=includeCoord;
	if(upper<includeCoord||isnan(upper))
		upper=includeCoord;
}

CRange CRange::expanded(const CRange& other)const
{
	CRange res(*this);
	res.expand(other);
	return res;
}

CRange CRange::expanded(double includeCoord)const
{
	CRange res(*this);
	res.expand(includeCoord);
	return res;
}

CRange CRange::bounded(double lowerBound, double upperBound)const
{
	if(lowerBound>upperBound)
		mSwap(lowerBound, upperBound);
	CRange res(lower, upper);
	if(res.lower<lowerBound)
	{
		res.lower=lowerBound;
		res.upper=lowerBound+size();
		if(res.upper>upperBound||fuzzyCompare(size(), upperBound-lowerBound))
			res.upper=upperBound;
		
	}
	else if(res.upper>upperBound)
	{
		res.upper=upperBound;
		res.lower=upperBound-size();
		if(res.lower<lowerBound||fuzzyCompare(size(), upperBound-lowerBound))
			res.lower=lowerBound;
	}
	return res;
}


CRange CRange::sanitizedForLogScale()const
{
	double rangeFac=1e-3;
	CRange sanitizedRange(lower, upper);
	sanitizedRange.normalize();
	if(sanitizedRange.lower==0.0&&sanitizedRange.upper!=0.0)
	{
		if(rangeFac<sanitizedRange.upper*rangeFac)
			sanitizedRange.lower=rangeFac;
		else
			sanitizedRange.lower =sanitizedRange.upper*rangeFac;
	}
	else if(sanitizedRange.lower!=0&&sanitizedRange.upper==0)
	{
		if(-rangeFac>sanitizedRange.lower*rangeFac)
			sanitizedRange.upper=-rangeFac;
		else
			sanitizedRange.upper=sanitizedRange.lower*rangeFac;
	}else if(sanitizedRange.lower<0&&sanitizedRange.upper>0)
	{
		if(-sanitizedRange.lower>sanitizedRange.upper)
		{
			if(-rangeFac>sanitizedRange.lower*rangeFac)
				sanitizedRange.upper=-rangeFac;
			else
				sanitizedRange.upper=sanitizedRange.lower*rangeFac;
		}
		else{
			if(rangeFac<sanitizedRange.upper*rangeFac)
				sanitizedRange.lower=rangeFac;
			else
				sanitizedRange.lower=sanitizedRange.upper*rangeFac;
		}
	}
	return sanitizedRange;
}

CRange CRange::sanitizedForLinScale()const
{
	CRange sanitizedRange(lower, upper);
	sanitizedRange.normalize();
	return sanitizedRange;
}

bool CRange::validRange(double lower, double upper)
{
	return (lower>-maxRange&&upper<maxRange
	&&mAbs(lower-upper)>minRange&&mAbs(lower-upper)<maxRange
	&&!(lower>0&&isinf(upper/lower))&&!(upper>0&&isinf(lower/upper)));
}

bool CRange::validRange(const CRange& range)
{
	return validRange(range.lower, range.upper);
}


/*======================DataRange====================*/
CDataRange::CDataRange():mBegin(0),mEnd(0)
{
	
}

CDataRange::CDataRange(int begin, int end):
mBegin(begin),
mEnd(end)
{
	
}

CDataRange CDataRange::bounded(const CDataRange& other)const
{
	CDataRange result(intersection(other));
	if(result.isEmpty())
	{
		if(mEnd<=other.mBegin)
			result = CDataRange(other.mBegin, other.mBegin);
		else
			result = CDataRange(other.mEnd, other.mEnd);
	}
	return result;
}


CDataRange CDataRange::expanded(const CDataRange& other)const
{
	return CDataRange(mMin(mBegin, other.mBegin),mMax(mEnd, other.mEnd));
}

CDataRange CDataRange::intersection(const CDataRange& other)const
{
	CDataRange result(mMax(mBegin, other.mBegin),mMin(mEnd, other.mEnd));
	if(result.isValid())
		return result;
	else
		return CDataRange();
}

bool CDataRange::intersects(const CDataRange& other)const
{
	return !((mBegin>other.mBegin&&mBegin>=other.mEnd)||
			(mEnd<=other.mBegin&&mEnd<other.mEnd));
}

bool CDataRange::contains(const CDataRange& other)const
{
	return mBegin<=other.mBegin&&mEnd>=other.mEnd;
}


/*====================Vector 2D======================*/

CVector2D::CVector2D():
mX(0),
mY(0)
{
	
}
CVector2D::CVector2D(double x, double y):
mX(x),
mY(y)
{
	
}

CVector2D::CVector2D(const CPoint& point):
mX(point.x()),
mY(point.y())
{
	
}
CVector2D::CVector2D(const CPointF& point):
mX(point.x()),
mY(point.y())
{
	
}

/*the length of the vector is equal to 1*/
void CVector2D::normalize()
{
	double len=length();
	mX/=len;
	mY/=len;
}

CVector2D CVector2D::normalized()const
{
	CVector2D res(mX, mY);
	res.normalize();
	return res;
}

double CVector2D::distanceSquaredToLine(const CVector2D& start, const CVector2D& end)const
{
	CVector2D v(end-start);
	double vLengthSqr=v.lengthSquared();
	if(fuzzyIsNull(vLengthSqr))
	{
		double mu=v.dot(*this-start)/vLengthSqr;
		if(mu<0)
			return (*this-start).lengthSquared();
		else if(mu>1)
			return (*this-end).lengthSquared();
		else 
			return ((start+mu*v)-*this).lengthSquared();
	}else
		return (*this-start).lengthSquared();
}

double CVector2D::distanceSquaredToLine(const CLineF& line)const
{
	return distanceSquaredToLine(CVector2D(line.p1()), CVector2D(line.p2()));
}


CVector2D& CVector2D::operator*=(double factor)
{
	mX*=factor;
	mY*=factor;
	return *this;
}


CVector2D& CVector2D::operator/=(double divisor)
{
	mX/=divisor;
	mY/=divisor;
	return *this;
}


CVector2D& CVector2D::operator+=(const CVector2D& vec)
{
	mX+=vec.x();
	mY+=vec.y();
	return *this;
}

CVector2D& CVector2D::operator-=(const CVector2D& vec)
{
	mX-=vec.x();
	mY-=vec.y();
	return *this;
}


CDataSelection::CDataSelection()
{
	
}

CDataSelection::CDataSelection(const CDataRange& range)
{
	mDataRanges.append(range);
}

bool CDataSelection::operator==(const CDataSelection& other)const
{
	if(mDataRanges.size()!=other.mDataRanges.size())
	{
		return false;
	}
	for(int i=0; i<mDataRanges.size();++i)
	{
		if(mDataRanges.at(i)==other.mDataRanges.at(i))
			return false;
	}
	
	return true;
}

CDataSelection& CDataSelection::operator+=(const CDataSelection& other)
{
	mDataRanges<<other.mDataRanges;
	simplify();
	return *this;
}

CDataSelection& CDataSelection::operator+=(const CDataRange& range)
{
	addDataRange(range);
	return *this;
}

CDataSelection& CDataSelection::operator-=(const CDataSelection& other)
{
	for(int i=0; i<other.dataRangeCount();++i)
		*this-=other.dataRange(i);
	return *this;
}

CDataSelection& CDataSelection::operator-=(const CDataRange& other)
{
	if(other.isEmpty()||isEmpty())
		return *this;
	simplify();
	int i=0;
	while(i<mDataRanges.size())
	{
		const int thisBegin=mDataRanges.at(i).begin();
		const int thisEnd=mDataRanges.at(i).end();
		if(thisBegin>=other.end())
			break;
		if(thisEnd>=other.begin())
		{
			if(thisBegin>=other.begin())
			{
				if(thisEnd<=other.end())
				{
					mDataRanges.removeAt(i);
					continue;
				}
				else
				{
					mDataRanges[i].setBegin(other.end());
				}
			}
			else
			{
				if(thisEnd<=other.end())
				{
					mDataRanges[i].setEnd(other.begin());
				}
				else
				{
					mDataRanges[i].setEnd(other.begin());
					mDataRanges.insert(i+1, CDataRange(other.end(), thisEnd));
					break;
				}
			}
		}
		++i;
	}
	return *this;
}


int CDataSelection::dataPointCount()const
{
	int result=0;
	for(int i=0; i<mDataRanges.size();++i)
		result+=mDataRanges.at(i).length();
	return result;
}

CDataRange CDataSelection::dataRange(int index)const
{
	if(index>=0&&index<mDataRanges.size())
	{
		return mDataRanges.at(index);
	}
	else
	{
		return CDataRange();
	}
}

CDataRange CDataSelection::span()const
{
	if(isEmpty())
		return CDataRange();
	else
		return CDataRange(mDataRanges.first().begin(), mDataRanges.last().end());
}

void CDataSelection::addDataRange(const CDataRange& dataRange, bool simplify)
{
	mDataRanges.append(dataRange);
	if(simplify)
		this->simplify();
}

void CDataSelection::clear()
{
	mDataRanges.clear();
}

void CDataSelection::simplify()
{
	
	for(int i=mDataRanges.size()-1;i>=0;--i)
	{
		if(mDataRanges.at(i).isEmpty())
			mDataRanges.removeAt(i);
	}
	if(mDataRanges.isEmpty())
		return ;
	
	std::sort(mDataRanges.begin(), mDataRanges.end(), lessThanDataRangeBegin);
	int i=1;
	while(i<mDataRanges.size())
	{
		if(mDataRanges.at(i-1).end()>=mDataRanges.at(i).begin())
		{
			mDataRanges[i-1].setEnd(mMax(mDataRanges.at(i-1).end(), mDataRanges.at(i).end()));
			mDataRanges.removeAt(i);
		}
		else
			++i;
	}
}


void CDataSelection::enforceType(WORD type)
{
	
	simplify();
	switch(type)
	{
		case Plot::stNone:
		{
			mDataRanges.clear();
			break;
		}
		case Plot::stWhole:
		{
			break;
		}
		
		case Plot::stSingleData:
		{
			if(!mDataRanges.isEmpty())
			{
				if(mDataRanges.size()>1)
					mDataRanges=CList<CDataRange>()<<mDataRanges.first();
				if(mDataRanges.first().length()>1)
					mDataRanges.first().setEnd(mDataRanges.first().begin()+1);
			}
			break;
			
		}
		
		case  Plot::stDataRange:
		{
			mDataRanges=CList<CDataRange>()<<span();
			break;
		}
		
		case Plot::stMultipleDataRanges:
		{
			break;
		}
	}
}



bool CDataSelection::contains(const CDataSelection& other)const
{
	
	if(other.isEmpty())	return false;
	int otherIndex=0;
	int thisIndex=0;
	while(thisIndex<mDataRanges.size()&&otherIndex<other.mDataRanges.size())
	{
		if(mDataRanges.at(thisIndex).contains(other.mDataRanges.at(otherIndex)))
			++otherIndex;
		else
			++thisIndex;
	}
	
	return thisIndex<mDataRanges.size();
}


CDataSelection CDataSelection::intersection(const CDataRange& other)const
{
	CDataSelection result;
	for(int i=0; i<mDataRanges.size();++i)
		result.addDataRange(mDataRanges.at(i).intersection(other), false);
	result.simplify();
	return result;
}


CDataSelection CDataSelection::intersection(const CDataSelection& other)const
{
	CDataSelection result;
	for(int i=0; i<other.dataRangeCount();++i)
		result+=intersection(other.dataRange(i));
	result.simplify();
	return result;
}

CDataSelection CDataSelection::inverse(const CDataRange& outerRange)const
{
	if(isEmpty())
		return CDataSelection(outerRange);
	CDataRange fullRange=outerRange.expanded(span());
	CDataSelection result;
	if(mDataRanges.first().begin()!=fullRange.begin())
		result.addDataRange(CDataRange(fullRange.begin(),mDataRanges.first().begin()),false);
	for(int i=1; i<mDataRanges.size();++i)
		result.addDataRange(CDataRange(mDataRanges.at(i-1).end(), mDataRanges.at(i).begin()),false);
	if(mDataRanges.last().end()!= fullRange.end())
		result.addDataRange(CDataRange(mDataRanges.last().end(), fullRange.end()),false);
	result.simplify();
	return result;
}
/*==================Plot Painter=====================*/

CPlotPainter::CPlotPainter():
CPainter(),mIsAntialiasing(false)
{
	
}

CPlotPainter::CPlotPainter(CPaintDevice* device):
CPainter(device),
mIsAntialiasing(false)
{
	
}

void CPlotPainter::setPen(const CPen& pen)
{
	CPainter::setPen(pen);
}

void CPlotPainter::setPen(int color)
{
	CPainter::setPen(color);
}

void CPlotPainter::setPen(Gemini::PenStyle penStyle)
{
	CPainter::setPen(penStyle);
}

void CPlotPainter::drawLine(const CLineF& line)
{
	if(mIsAntialiasing)
	{
		CPainter::drawLine(line);
	}
	else
		CPainter::drawLine(line.toLine());
}


void CPlotPainter::setAntialiasing(bool enabled)
{
	CPainter::setRenderHint(CPainter::Antialiasing,enabled);
	if(mIsAntialiasing!=enabled)
	{
		mIsAntialiasing=enabled;
		if(mIsAntialiasing)
			CPainter::translate(0.5,0.5);
		else
			CPainter::translate(-0.5,-0.5);
	}
}

bool CPlotPainter::begin(CPaintDevice* device)
{
	bool res=CPainter::begin(device);
	return res;
}

void CPlotPainter::save()
{
	mAntialiasingStack.push(mIsAntialiasing);
	CPainter::save();
}

void CPlotPainter::restore()
{
	if(!mAntialiasingStack.empty())
	{
		mIsAntialiasing=mAntialiasingStack.top();
		mAntialiasingStack.pop();
	}
	else
		std::cout<<"Error: Unbalanced save/restore"<<std::endl;
	CPainter::restore();
}

/*==================Line Ending======================*/

CLineEnding::CLineEnding():
mStyle(None),
mWidth(8),
mLength(10),
mInverted(false)
{
	
}

CLineEnding::CLineEnding(WORD style, double width, double length, bool inverted):
mStyle(style),
mWidth(width),
mLength(length),
mInverted(inverted)
{
	
}

void CLineEnding::setStyle(WORD style)
{
	mStyle=style;
}

void CLineEnding::setWidth(double width)
{
	mWidth=width;
}

void CLineEnding::setLength(double length)
{
	mLength= length;
}

void CLineEnding::setInverted(bool inverted)
{
	mInverted=inverted;
}

double CLineEnding::boundingDistance()const
{
	switch(mStyle)
	{
		case None:
			return 0;
		case FlatArrow:
		case SpikeArrow:
		case LineArrow:
		case SkewedBar:
			return mSqrt(mWidth*mWidth+mLength*mLength);
		case Disc:
		case Square:
		case Diamond:
		case Bar:
		case HalfBar:
			return mWidth*1.42;
	}
	return 0;
}


double CLineEnding::realLength()const
{
	switch(mStyle)
	{
		case None:
		case LineArrow:
		case SkewedBar:
		case Bar:
		case HalfBar:
			return 0;
		case FlatArrow:
			return mLength;
		case Disc:
		case Square:
		case Diamond:
			return mWidth*0.5;
		case SpikeArrow:
			return mLength*0.8;
	}
	return 0;
}


void CLineEnding::draw(CPlotPainter* painter, const CVector2D& pos, double angle) const
{
	draw(painter, pos, CVector2D(mCos(angle),mSin(angle)));
}

void CLineEnding::draw(CPainter* painter, const CVector2D& pos, double angle) const
{
	draw(painter, pos, CVector2D(mCos(angle),mSin(angle)));
}


void CLineEnding::draw(CPainter* painter, const CVector2D& pos, const CVector2D& dir)const
{
	if(mStyle==None)
		return ;
	CVector2D lengthVec=dir.normalized()*mLength*(mInverted?-1:1);
	if(lengthVec.isNull())
		lengthVec = CVector2D(1,0);
	CVector2D widthVec=dir.normalized().perpendicular()*mWidth*0.5*(mInverted?-1:1);
	CPen penBackup=painter->pen();
	CBrush brushBackup=painter->brush();
	CPen miterPen=penBackup;
	miterPen.setJoinStyle(Gemini::MiterJoin);
	CBrush brush(painter->pen().color(),Gemini::SolidPattern);
	switch(mStyle)
	{
		case None: break;
		case FlatArrow:
		{
			CPointF points[3]={pos.toPointF(), (pos-lengthVec+widthVec).toPointF(),(pos-lengthVec-widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points,3);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}	
		case SpikeArrow:
		{
			CPointF points[4]={pos.toPointF(),(pos-lengthVec+widthVec).toPointF(),(pos-lengthVec*0.8).toPointF(),
								(pos-lengthVec-widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points, 4);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}
		case LineArrow:
		{
			CPointF points[3]={(pos-lengthVec+widthVec).toPointF(),pos.toPointF(), (pos-lengthVec-widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->drawConvexPolygon(points,3);
			painter->setPen(penBackup);
			break;
		}
		case Disc:
		{
			painter->setBrush(brush);
			painter->drawEllipse(pos.toPointF(), mWidth*0.5, mWidth*0.5);
			painter->setBrush(brushBackup);
			break;
		}
		case Square:
		{
			CVector2D widthVecPerp=widthVec.perpendicular();
			CPointF points[4]={(pos-widthVecPerp+widthVec).toPointF(),
								(pos-widthVecPerp-widthVec).toPointF(),
								(pos+widthVecPerp-widthVec).toPointF(),
								(pos+widthVecPerp+widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points, 4);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}
		case Diamond:
		{
			CVector2D widthVecPerp=widthVec.perpendicular();
			CPointF points[4]={(pos-widthVecPerp).toPointF(),
								(pos-widthVec).toPointF(),
								(pos+widthVecPerp).toPointF(),
								(pos+widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points,4);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}
		
		case Bar:
		{
			painter->drawLine((pos+widthVec).toPointF(),(pos-widthVec).toPointF());
			break;
		}
		
		case HalfBar:
		{
			painter->drawLine((pos+widthVec).toPointF(), pos.toPointF());
			break;
		}
		
		case SkewedBar:
		{
			if(fuzzyIsNull(painter->pen().widthF()))
			{
				painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)).toPointF(),
								(pos-widthVec-lengthVec*0.2*(mInverted?-1:1)).toPointF());
			}
			else
			{
				painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*mMax(1.0f,(float)painter->pen().widthF())*0.5f).toPointF(),
								(pos-widthVec-lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*mMax(1.0f,(float)painter->pen().widthF())*0.5f).toPointF());
			}
			break;
		}
	}
}

void CLineEnding::draw(CPlotPainter* painter, const CVector2D& pos, const CVector2D& dir)const
{
	if(mStyle==None)
		return ;
	CVector2D lengthVec=dir.normalized()*mLength*(mInverted?-1:1);
	if(lengthVec.isNull())
		lengthVec = CVector2D(1,0);
	CVector2D widthVec=dir.normalized().perpendicular()*mWidth*0.5*(mInverted?-1:1);
	CPen penBackup=painter->pen();
	CBrush brushBackup=painter->brush();
	CPen miterPen=penBackup;
	miterPen.setJoinStyle(Gemini::MiterJoin);
	CBrush brush(painter->pen().color(),Gemini::SolidPattern);
	switch(mStyle)
	{
		case None: break;
		case FlatArrow:
		{
			CPointF points[3]={pos.toPointF(), (pos-lengthVec+widthVec).toPointF(),(pos-lengthVec-widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points,3);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}	
		case SpikeArrow:
		{
			CPointF points[4]={pos.toPointF(),(pos-lengthVec+widthVec).toPointF(),(pos-lengthVec*0.8).toPointF(),
								(pos-lengthVec-widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points, 4);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}
		case LineArrow:
		{
			CPointF points[3]={(pos-lengthVec+widthVec).toPointF(),pos.toPointF(), (pos-lengthVec-widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->drawConvexPolygon(points,3);
			painter->setPen(penBackup);
			break;
		}
		case Disc:
		{
			painter->setBrush(brush);
			painter->drawEllipse(pos.toPointF(), mWidth*0.5, mWidth*0.5);
			painter->setBrush(brushBackup);
			break;
		}
		case Square:
		{
			CVector2D widthVecPerp=widthVec.perpendicular();
			CPointF points[4]={(pos-widthVecPerp+widthVec).toPointF(),
								(pos-widthVecPerp-widthVec).toPointF(),
								(pos+widthVecPerp-widthVec).toPointF(),
								(pos+widthVecPerp+widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points, 4);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}
		case Diamond:
		{
			CVector2D widthVecPerp=widthVec.perpendicular();
			CPointF points[4]={(pos-widthVecPerp).toPointF(),
								(pos-widthVec).toPointF(),
								(pos+widthVecPerp).toPointF(),
								(pos+widthVec).toPointF()};
			painter->setPen(miterPen);
			painter->setBrush(brush);
			painter->drawConvexPolygon(points,4);
			painter->setBrush(brushBackup);
			painter->setPen(penBackup);
			break;
		}
		
		case Bar:
		{
			painter->drawLine((pos+widthVec).toPointF(),(pos-widthVec).toPointF());
			break;
		}
		
		case HalfBar:
		{
			painter->drawLine((pos+widthVec).toPointF(), pos.toPointF());
			break;
		}
		
		case SkewedBar:
		{
			if(fuzzyIsNull(painter->pen().widthF()))
			{
				painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)).toPointF(),
								(pos-widthVec-lengthVec*0.2*(mInverted?-1:1)).toPointF());
			}
			else
			{
				painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*mMax(1.0f,(float)painter->pen().widthF())*0.5f).toPointF(),
								(pos-widthVec-lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*mMax(1.0f,(float)painter->pen().widthF())*0.5f).toPointF());
			}
			break;
		}
	}
}
/*==================Axis Ticker======================*/

CAxisTicker::CAxisTicker():
mTickStepStrategy(Readability),
mTickCount(5), mTickOrigin(0)
{
		
}

CAxisTicker::~CAxisTicker()
{
	
}

void CAxisTicker::setTickStepStrategy(WORD strategy)
{
	mTickStepStrategy=strategy;
}
void CAxisTicker::setTickCount(int count)
{
	if(count>0)
		mTickCount = count;
	else
		std::cout<<"Error: tick count mustbe greater than zero: "<<count<<std::endl;
}

/*相对于0点offset*/
void CAxisTicker::setTickOrigin(double origin)
{
	mTickOrigin = origin;
}

void CAxisTicker::generate(const CRange& range, char format,const int precision, CVector<double>& ticks, 
		CVector<double>*subTicks,std::vector<std::string>* tickLabels)
{
	//生成主刻度
	double tickStep= getTickStep(range);
	ticks = createTickVector(tickStep, range);
	trimTicks(range, ticks, true);//将刻度裁剪到设置的刻度范围内
	
	//生成小刻度
	if(subTicks)
	{
		if(ticks.size()>0)
		{
			*subTicks=createSubTickVector(getSubTickCount(tickStep),ticks);
			trimTicks(range, *subTicks,false);
		}
		else
		{
			*subTicks=CVector<double>();
		}
	}
	trimTicks(range, ticks, false);
	//生成刻度标签
	if(tickLabels)
	{
		*tickLabels = createLabelVector(ticks,format,precision);
	}
}


double CAxisTicker::getTickStep(const CRange& range)
{
	double exactStep = range.size()/(double)(mTickCount+1e-10);//增加1e-10防止整数出现上下抖动
	return cleanMantissa(exactStep);
}

int CAxisTicker::getSubTickCount(double tickStep)
{
	int result=1; //设置默认值为1
	//分离出整数部分和小数部分
	double epsilon=0.01;
	double intPartf;
	int intPart;
	double fracPart=modf(getMantissa(tickStep),&intPartf);
	intPart = intPartf;
	if(fracPart<epsilon||1.0-fracPart<epsilon)
	{
		if(1.0-fracPart<epsilon)
			++intPart;
		switch(intPart)
		{
			case 1: result= 4; break; //1.0->0.2 substep
			case 2: result= 3; break; //2.0->0.5 substep
			case 3: result= 2; break; //3.0->1.0 substep
			case 4: result= 3; break; //4.0->1.0 substep
			case 5: result= 4; break; //5.0->1.0 substep
			case 6: result= 2; break; //6.0->2.0 substep
			case 7: result= 6; break; //7.0->1.0 substep
			case 8: result= 3; break; //8.0->2.0 substep
			case 9: result= 2; break; //9.0->3.0 substep
		}
	}
	else
	{
		if(mAbs(fracPart-0.5)<epsilon)
		{
			switch(intPart)
			{
				case 1: result= 2; break; //1.5->0.5 substep
				case 2: result= 4; break; //2.5->0.5 substep
				case 3: result= 4; break; //3.5->0.7 substep
				case 4: result= 2; break; //4.5->1.5 substep
				case 5: result= 4; break; //5.5->1.1 substep
				case 6: result= 4; break; //6.5->1.3 substep
				case 7: result= 2; break; //7.5->2.5 substep
				case 8: result= 4; break; //8.5->1.7 substep
				case 9: result= 4; break; //9.5->1.9 substep
			}
		}
	}
	return result;
}

static inline int digitcount(int value)
{
	int result=1;
	while(value/=10) result++;
	return result;
}
std::string CAxisTicker::getTickLabel(double tick, char format,int precision)
{
	char label[128];
	switch(format)
	{
		case 'd':
			sprintf(label,"%d",(int)(tick));
			break;
		case 'e':
			sprintf(label,"%.*e",precision,tick);
			break;
		case 'E':
			sprintf(label,"%.*E",precision,tick);
			break;
		case 'g':
		{
			int intLen=digitcount(floor(tick));
			sprintf(label,"%.*g",precision+intLen,tick);
		}
			break;
		case 'G':
		{
			int intLen=digitcount(floor(tick));
			sprintf(label,"%.*G",precision+intLen,tick);
		}
			break;
		case 'x':
			sprintf(label,"%x",(int)tick);
			break;
		case 'X':
			sprintf(label,"%X",(int)tick);
			break;
		default:
			sprintf(label,"%.*lf",precision,tick);
			break;
	}
	return std::string(label);
}

CVector<double> CAxisTicker::createSubTickVector(int subTickCount, const CVector<double>& ticks)
{
	CVector<double> result;
	if(subTickCount<=0||ticks.size()<2)
		return result;
	result.reserve((ticks.size()-1)*subTickCount);
	for(int i=1; i<ticks.size(); ++i)
	{
		double subTickStep =(ticks.at(i)-ticks.at(i-1))/(double)(subTickCount+1);
		for(int k=1; k<=subTickCount; ++k)
			result.push_back(ticks.at(i-1)+k*subTickStep);
	}
	return result;
}

CVector<double> CAxisTicker::createTickVector(double tickStep, const CRange& range)
{
	CVector<double> result;
	int64 firstStep=floor((range.lower-mTickOrigin)/tickStep);
	int64 lastStep=ceil((range.upper-mTickOrigin)/tickStep);
	int tickcount= lastStep-firstStep+1;
	if(tickcount<0) tickcount = 0;
	result.resize(tickcount);
	for(int i=0; i<tickcount; ++i)
		result[i]=mTickOrigin+(firstStep+i)*tickStep;
	return result;
}


std::vector<std::string> CAxisTicker::createLabelVector(const CVector<double>&ticks, char format, int precision)
{
	std::vector<std::string> result;
	result.reserve(ticks.size());
	for(int i=0; i<ticks.size(); ++i)
		result.push_back(getTickLabel(ticks.at(i),format, precision));
	return result;
}

void CAxisTicker::trimTicks(const CRange& range, CVector<double>& ticks,bool keepOneOutlier) const
{
	bool lowFound = false;
	bool highFound = false;
	int lowIndex = 0;
	int highIndex = -1;
	for(int i=0; i<ticks.size(); ++i)
	{
		if(ticks.at(i)>=range.lower)
		{
			lowFound = true;
			lowIndex = i;
			break;
		}
	}
	
	for(int i=ticks.size()-1; i>=0; --i)
	{
		if(ticks.at(i)<=range.upper)
		{
			highFound = true;
			highIndex = i;
			break;
		}
	}
	
	if(highFound&&lowFound)
	{
		int trimFront = mMax(0, lowIndex-(keepOneOutlier?1:0));
		int trimBack = mMax(0, ticks.size()-(keepOneOutlier?2:1)-highIndex);
		if(trimFront>0||trimBack>0)
		{
			ticks=ticks.mid(trimFront, ticks.size()-trimFront-trimBack);
		}
	}else
	{
		ticks.clear();
	}
}

double CAxisTicker::pickClosest(double target, const CVector<double>& candidates)const
{
	if(candidates.size()==1)
		return candidates[0];
	CVector<double>::const_iterator it= std::lower_bound(candidates.begin(), candidates.end(),target);
	if(it==candidates.end())
		return *(it-1);
	else if(it==candidates.begin())
		return *it;
	else
		return target-*(it-1)<*it-target ? *(it-1):*it;
}

double CAxisTicker::getMantissa(double input, double* magnitude)const
{
	const double mag=mPow(10.0, mFloor(mLn(input)/mLn(10.0)));
	if(magnitude) *magnitude=mag;
	return input/mag;
}

double CAxisTicker::cleanMantissa(double input)const
{
	double magnitude;
	const double mantissa=getMantissa(input,&magnitude);
	switch(mTickStepStrategy)
	{
		case Readability:
		{
			CVector<double> tmp;
			tmp.push_back(double(1.0));
			tmp.push_back(double(2.0));
			tmp.push_back(double(2.5));
			tmp.push_back(double(5.0));
			tmp.push_back(double(10.0));
			return pickClosest(mantissa,tmp )*magnitude;
		}
		case MeetTickCount:
		{
			if(mantissa<=5.0)
			{
				return round(mantissa*2)/2.0*magnitude;
			}
			else
				return round(mantissa/2.0)*2.0*magnitude;
		}
	}
	return input;
}

/*================Ticker Log==================*/
/*================指数级坐标刻度处理==========*/

CAxisTickerLog::CAxisTickerLog()
:mLogBase(10.0),
mSubTickCount(8),
mLogBaseLnInv(1.0/mLn(mLogBase))
{
	
}

void CAxisTickerLog::setLogBase(double base)
{
	if(base>0)
	{
		mLogBase=base;
		mLogBaseLnInv=1.0/mLn(mLogBase);
	}else
	{
		std::cout<<"Error: log base has to be greater than zero:"<<base<<std::endl;
	}
}

void CAxisTickerLog::setSubTickCount(int subTicks)
{
	if(subTicks>=0)
		mSubTickCount=subTicks;
	else
		std::cout<<"Error: sub tick count can't be negative:"<<subTicks<<std::endl;
}


double CAxisTickerLog::getTickStep(const CRange& range)
{
	UNUSED_PARAM(range);
	return 1.0;
}

int CAxisTickerLog::getSubTickCount(double tickStep)
{
	UNUSED_PARAM(tickStep);
	return mSubTickCount;
}

CVector<double> CAxisTickerLog::createTickVector(double tickStep, const CRange& range)
{
	UNUSED_PARAM(tickStep);
	CVector<double> result;
	if(range.lower>0&&range.upper>0)//positive
	{
		double exactPowerStep= mLn(range.upper/range.lower)*mLogBaseLnInv/(double)(mTickCount+1e-10);
		double newLogBase =mPow(mLogBase, mMax(round(cleanMantissa(exactPowerStep)),1));
		double currentTick=mPow(newLogBase, mFloor(mLn(range.lower)/mLn(newLogBase)));
		result.push_back(currentTick);
		while(currentTick<range.upper&&currentTick>0)
		{
			currentTick*=newLogBase;
			result.push_back(currentTick);
		}
	}
	else if(range.lower<0&&range.upper<0)//negative
	{
		double exactPowerStep=mLn(range.lower/range.upper)*mLogBaseLnInv/(double)(mTickCount+1e-10);
		double newLogBase=mPow(mLogBase, mMax(round(cleanMantissa(exactPowerStep)),1));
		double currentTick = -mPow(newLogBase, mCeil(mLn(-range.lower)/mLn(newLogBase)));
		result.push_back(currentTick);
		while(currentTick<range.upper&&currentTick<0)
		{
			currentTick /=newLogBase;
			result.push_back(currentTick);
		}
	}
	else
	{
		std::cout<<"Error: Invalid range for logarithmic plot: "<<range.lower<<".."<<range.upper<<std::endl;
	}
	return result;
}

/*================处理时间坐标刻度==============================*/
/*============^^^^逻辑暂时未处理^^^^^=======================*/
CAxisTickerTime::CAxisTickerTime()
{
	
}
		
void CAxisTickerTime::setTimeFormat(const std::string& format)
{
	
}

void CAxisTickerTime::setFieldWidth(TimeUnit unit, int width)
{
	
}

		
double CAxisTickerTime::getTickStep(const CRange& range)
{
	return 0;
}

int CAxisTickerTime::getSubTickCount(double tickStep)
{
	return 0;
}

std::string CAxisTickerTime::getTickLabel(double tick, char format, int precision)
{
	return std::string();
}

void CAxisTickerTime::replaceUnit(std::string& text, WORD unit, int value)const
{
	
}



/*========================Axis Painter Private========================*/
CAxisPainterPrivate::CAxisPainterPrivate():
type(CPlotAxis::atLeft),
basePen(CPen(Gemini::black,0,Gemini::SolidLine, Gemini::SquareCap)),
lowerEnding(CLineEnding::None),
upperEnding(CLineEnding::None),
labelPadding(0),
tickLabelPadding(0),
tickLabelRotation(0),
tickLabelSide(CPlotAxis::lsOutSide),
substituteExponent(true),
numberMultiplyCross(false),
tickLengthIn(5),
tickLengthOut(0),
subTickLengthIn(2),
subTickLengthOut(0),
tickPen(CPen(Gemini::black,0,Gemini::SolidLine, Gemini::SquareCap)),
subTickPen(CPen(Gemini::black,0,Gemini::SolidLine, Gemini::SquareCap)),
offset(0),
abbreviateDecimalPowers(false),
reversedEndings(false)
{
	
}

CAxisPainterPrivate::~CAxisPainterPrivate()
{
	
}

void CAxisPainterPrivate::draw(CPlotPainter* painter)
{
	CPoint origin;
	switch(type)
	{
		case CPlotAxis::atLeft: 	origin=axisRect.bottomLeft()+CPoint(-offset,0); break;
		case CPlotAxis::atRight: 	origin=axisRect.bottomRight()+CPoint(offset,0); break;
		case CPlotAxis::atTop: 		origin=axisRect.topLeft()+CPoint(0,-offset); break;
		case CPlotAxis::atBottom:	origin=axisRect.bottomLeft()+CPoint(0, offset); break;
	}
	double xCor=0, yCor=0;
	switch(type)
	{
		case CPlotAxis::atTop: yCor=-1; break;
		case CPlotAxis::atRight: xCor=1; break;
		default: break;
	}
	
	int margin=0;
	//draw baseline
	CLineF baseLine;
	painter->setPen(basePen);
	if(CPlotAxis::orientation(type)==Gemini::Horizontal)
	{
		CPointF p1=origin+CPointF(xCor, yCor);
		CPointF p2=origin+CPointF(axisRect.width()+xCor, yCor);
		baseLine.setPoints(p1 , p2);
	}
	else
	{
		CPointF p1=origin+CPointF(xCor, yCor);
		CPointF p2=origin+CPointF(xCor, -axisRect.height()+yCor);
		baseLine.setPoints(p1,p2);
	}
	
	if(reversedEndings)
		baseLine=CLineF(baseLine.p2(), baseLine.p1());
	painter->drawLine(baseLine);
	
	//draw ticks:
	if(!tickPositions.empty())
	{
		painter->setPen(tickPen);
		int tickDir=(type==CPlotAxis::atBottom||type==CPlotAxis::atRight)?-1:1;
		if(CPlotAxis::orientation(type)==Gemini::Horizontal)
		{
			for(int i=0; i<tickPositions.size();++i)
				painter->drawLine(CLineF(tickPositions.at(i)+xCor,origin.y()-tickLengthOut*tickDir+yCor,
				tickPositions.at(i)+xCor, origin.y()+tickLengthIn*tickDir+yCor));
			
		}
		else
		{
			for(int i=0; i<tickPositions.size();++i)
				painter->drawLine(CLineF(origin.x()-tickLengthOut*tickDir+xCor, tickPositions.at(i)+yCor,
				origin.x()+tickLengthIn*tickDir+xCor, tickPositions.at(i)+yCor));
		}
	}
	
	//draw subticks:
	if(!subTickPositions.empty())
	{
		painter->setPen(subTickPen);
		 int tickDir =(type==CPlotAxis::atBottom||type==CPlotAxis::atRight)?-1:1;
		if(CPlotAxis::orientation(type)==Gemini::Horizontal)
		{
			for(int i=0; i<subTickPositions.size();++i)
				painter->drawLine(CLineF(subTickPositions.at(i)+xCor, origin.y()-subTickLengthOut*tickDir+yCor,
				subTickPositions.at(i)+xCor,origin.y()+subTickLengthIn*tickDir+yCor));
		}
		else
		{
			for(int i=0; i<subTickPositions.size();++i)
				painter->drawLine(CLineF(origin.x()-subTickLengthOut*tickDir+xCor, subTickPositions.at(i)+yCor,
				origin.x()+subTickLengthIn*tickDir+xCor, subTickPositions.at(i)+yCor));
		}
	}
	margin+=mMax(0,mMax(tickLengthOut, subTickLengthOut));

	//draw axis base endings:
	bool antialiasingBackup=painter->antialiasing();
	painter->setAntialiasing(true);
	painter->setBrush(CBrush(basePen.color()));
	painter->setPen(basePen);
	CVector2D baseLineVector(baseLine.dx(), baseLine.dy());
	if(lowerEnding.style()!=CLineEnding::None)
	{
		lowerEnding.draw(painter, CVector2D(baseLine.p1())-baseLineVector.normalized()*lowerEnding.realLength()*(lowerEnding.inverted()?-1:1),-baseLineVector);
	}
	if(upperEnding.style()!=CLineEnding::None)
	{
		upperEnding.draw(painter, CVector2D(baseLine.p2())+baseLineVector.normalized()*upperEnding.realLength()*(upperEnding.inverted()?-1:1),baseLineVector);
	}
	painter->setAntialiasing(antialiasingBackup);
	
	//tick labels:
	CSize tickLabelSize(0,0);
	if(!tickLabels.empty())
	{
		if(tickLabelSide==CPlotAxis::lsOutSide)
		{
			margin+=tickLabelPadding;
		}
		const int maxLabelIndex=mMin(tickPositions.size(), tickLabels.size());
		int distanceToAxis=margin;
		if(tickLabelSide==CPlotAxis::lsInSide)
		{
			distanceToAxis=-(mMax(tickLengthIn,subTickLengthIn)+tickLabelPadding);
		}
		for(int i=0; i<maxLabelIndex;++i)
			placeTickLabel(painter, tickPositions.at(i),distanceToAxis, tickLabels.at(i),&tickLabelSize);
		if(tickLabelSide==CPlotAxis::lsOutSide)
			margin+=(CPlotAxis::orientation(type)==Gemini::Horizontal)?tickLabelSize.height():tickLabelSize.width();
	}
	
	//axis label:
	CRect labelBounds;
	if(!label.empty())
	{
		margin+=labelPadding;
		labelBounds=CRect(0,0,(labelFont.width*(label.size()+1)>>1)+1,labelFont.height*1.5);
		if(g_pDrawText)
		{
			if(type==CPlotAxis::atLeft)
			{
				CRect rect(CPoint(0,0),
					labelBounds.size());
				CTransform rectTransform;
				rectTransform.translate(origin.x()-margin-labelBounds.height(),origin.y()-(axisRect.height()-labelBounds.width())*0.5+labelBounds.height());
				rectTransform.rotate(-90);
				CRect imageRect=rectTransform.mapRect(rect);
				CImage image=((CImage*)painter->device())->copy(imageRect);
				CTransform imageTransform;
				imageTransform.translate(imageRect.right(),imageRect.bottom());
				imageTransform.rotate(-90);
				CRect testRect=imageTransform.mapRect(image.rect());
				CImage rotateImage=image.transformed(imageTransform);
				
				g_pDrawText->SetFontSize(labelFont.width,labelFont.height,0x01);
				g_pDrawText->SetFontColor(labelFont.color);
				g_pDrawText->DrawText(rotateImage,(unsigned char*)label.c_str(),labelBounds-CMargin(1,1,1,1),label.size());
				CTransform oldTransform=painter->transform();
				painter->translate((origin.x()-margin-labelBounds.height()),origin.y()-(axisRect.height()-labelBounds.width())*0.5);
				painter->rotate(-90);
				painter->drawImage(CPoint(0,0),rotateImage);
				painter->setTransform(oldTransform); 
			}
			else if(type==CPlotAxis::atRight)
			{
				CRect rect(CPoint(0,0),
				labelBounds.size());
				CTransform rectTransform;
				rectTransform.translate(origin.x()+margin+labelBounds.height(),origin.y()-(axisRect.height()-labelBounds.width())*0.5+labelBounds.height());
				rectTransform.rotate(90);
				CRect imageRect=rectTransform.mapRect(rect);
				CImage image=((CImage*)painter->device())->copy(imageRect);
				CTransform imageTransform;
				imageTransform.translate(imageRect.right(),imageRect.bottom());
				imageTransform.rotate(90);
				CRect testRect=imageTransform.mapRect(image.rect());
				CImage rotateImage=image.transformed(imageTransform);
				
				g_pDrawText->SetFontSize(labelFont.width,labelFont.height,0x01);
				g_pDrawText->SetFontColor(labelFont.color);
				g_pDrawText->DrawText(rotateImage,(unsigned char*)label.c_str(),labelBounds-CMargin(1,1,1,1),label.size());
				CTransform oldTransform=painter->transform();
				painter->translate((origin.x()+margin+labelBounds.height()),origin.y()-(axisRect.height()+labelBounds.width())*0.5);
				painter->rotate(90);
				painter->drawImage(CPoint(0,0),rotateImage);
				painter->setTransform(oldTransform); 
			}
			else if(type==CPlotAxis::atTop)
			{
				CRect imageRect(CPoint(origin.x()+(axisRect.width()-labelBounds.width())*0.5,origin.y()-margin),labelBounds.size());
				CImage image=((CImage*)painter->device())->copy(imageRect);
				g_pDrawText->SetFontSize(labelFont.width,labelFont.height,0x01);
				g_pDrawText->SetFontColor(labelFont.color);
				g_pDrawText->DrawText(image,(unsigned char*)label.c_str(),labelBounds-CMargin(1,1,1,1),label.size());
				CTransform oldTransform=painter->transform();
				painter->translate(origin.x()+(axisRect.width()-labelBounds.width())*0.5,origin.y()-margin);
				painter->drawImage(CPoint(0,0),image);
				painter->setTransform(oldTransform); 
			}
			else if(type==CPlotAxis::atBottom)
			{
				CRect imageRect(CPoint(origin.x()+(axisRect.width()-labelBounds.width())*0.5,origin.y()+margin),labelBounds.size());
				CImage image=((CImage*)painter->device())->copy(imageRect);
				g_pDrawText->SetFontSize(labelFont.width,labelFont.height,0x01);
				g_pDrawText->SetFontColor(labelFont.color);
				g_pDrawText->DrawText(image,(unsigned char*)label.c_str(),labelBounds-CMargin(1,1,1,1),label.size());
				CTransform oldTransform=painter->transform();
				painter->translate(origin.x()+(axisRect.width()-labelBounds.width())*0.5,origin.y()+margin);
				painter->drawImage(CPoint(0,0),image);
				painter->setTransform(oldTransform); 
			}
		}
	}

}

int  CAxisPainterPrivate::size()const
{
	int result = 0;
	if(!tickPositions.empty())
		result+=mMax(0,mMax(tickLengthOut,subTickLengthOut));
	//calc size of tick labels
	if(tickLabelSide==CPlotAxis::lsOutSide)
	{
		CSize tickLabelsSize(0,0);
		if(!tickLabels.empty())
		{
			for(int i=0; i<tickLabels.size(); ++i)
				getMaxTickLabelSize(tickLabelFont, tickLabels.at(i),&tickLabelsSize);
			result+=type==Gemini::Horizontal?tickLabelsSize.height():tickLabelsSize.width();
			result+=tickLabelPadding;
		}
	}
	//calcute size of axis label
	
	if(!label.empty())
	{
		//计算字体size
		result+=labelFont.height+labelPadding;
	}
	return result;
}


void CAxisPainterPrivate::placeTickLabel(CPlotPainter* painter, double position, int distanceToAxis, const std::string& text, CSize* tickLabelsSize)
{
	if(text.empty())	return ;
	CSize finalSize;
	CPointF labelAnchor;
	
	//获取锚点
	switch(type)
	{
		case CPlotAxis::atLeft:		labelAnchor = CPointF(axisRect.left()-distanceToAxis-offset, position); break;
		case CPlotAxis::atRight: 	labelAnchor = CPointF(axisRect.right()+distanceToAxis+offset, position); break;
		case CPlotAxis::atTop:		labelAnchor = CPointF(position,axisRect.top()-distanceToAxis-offset);break;
		case CPlotAxis::atBottom: 	labelAnchor = CPointF(position, axisRect.bottom()+distanceToAxis+offset); break;
	}
	
	TickLabelData labelData=getTickLabelData(tickLabelFont,text);
	CPointF labelOffset=getTickLabelDrawOffset(labelData)+labelData.rotatedTotalBounds.topLeft();
	CImage image=((CImage*)painter->device())->copy(CRect((labelAnchor+labelOffset).toPoint(),labelData.totalBounds.size()));
	CPlotPainter labelTickPainter(&image);
	drawTickLabel(&labelTickPainter,-labelData.rotatedTotalBounds.topLeft().x(),-labelData.rotatedTotalBounds.topLeft().y(),labelData);
	CTransform oldTransform=painter->transform();
	painter->translate(labelAnchor+labelOffset);
	painter->rotate(tickLabelRotation);
	painter->setRenderHint(CPainter::SmoothPixmapTransform);
	painter->drawImage(CPoint(0,0), image);
	painter->setTransform(oldTransform);
}

void CAxisPainterPrivate::drawTickLabel(CPlotPainter* painter, double x, double y, const TickLabelData& labelData)const
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


CAxisPainterPrivate::TickLabelData CAxisPainterPrivate::getTickLabelData(const PlotFont& font, const std::string& text)const
{
	TickLabelData result;
	bool useBeautifulPowers = false;
	int ePos=-1;
	int eLast=-1;
	if(substituteExponent)
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
	if(!fuzzyIsNull(tickLabelRotation))
	{
		CTransform transform;
		transform.rotate(tickLabelRotation);
		result.rotatedTotalBounds=transform.mapRect(result.rotatedTotalBounds);
	}		
	return result;
}

CPointF CAxisPainterPrivate::getTickLabelDrawOffset(const TickLabelData& labelData)const
{
	bool doRotation =!fuzzyIsNull(tickLabelRotation);
	bool flip=fuzzyCompare(mAbs(tickLabelRotation),90.0);
	double radians=tickLabelRotation/180*M_PI;
	int x=0, y=0;
	if(((type==CPlotAxis::atLeft)&&(tickLabelSide==CPlotAxis::lsOutSide))||
	   ((type==CPlotAxis::atRight)&&(tickLabelSide==CPlotAxis::lsInSide)))
	   {
	   	    if(doRotation)
	   	    {
	   	    	if(tickLabelRotation>0)
	   	    	{
	   	    		x=-mCos(radians)*labelData.totalBounds.width();
	   	    		y=flip?-labelData.totalBounds.width()/2.0:-mSin(radians)*labelData.totalBounds.width()-mCos(radians)*labelData.totalBounds.height()/2.0;
	   	    	}
	   	    	else
	   	    	{
	   	    		x=-mCos(-radians)*labelData.totalBounds.width()-mSin(-radians)*labelData.totalBounds.height();
	   	    		y=flip?+labelData.totalBounds.width()/2.0:mSin(-radians)*labelData.totalBounds.width()-mCos(-radians)*labelData.totalBounds.height()/2.0;
	   	    		
	   	    	}
	   	    }
	   	    else
	   	    {
	   	    	x=-labelData.totalBounds.width();
	   	    	y=-labelData.totalBounds.height()/2.0;
	   	    }
	   }
	else 
	if(((type==CPlotAxis::atRight)&&(tickLabelSide==CPlotAxis::lsOutSide))||
	   ((type==CPlotAxis::atLeft)&&(tickLabelSide==CPlotAxis::lsInSide)))
	   {
			if(doRotation)
			{
				if(tickLabelRotation>0)
				{
					x=mSin(radians)*labelData.totalBounds.height();
					y=flip?-labelData.totalBounds.width()/2.0:-mCos(radians)*labelData.totalBounds.height()/2.0;
				}
				else
				{
					x=0;
					y=flip?labelData.totalBounds.width()/2.0:-mCos(-radians)*labelData.totalBounds.height()/2.0;
				}
			}
			else
			{
				x=0;
				y=-labelData.totalBounds.height()/2.0;
			}
	   }
	else
	if((type==CPlotAxis::atTop&&tickLabelSide==CPlotAxis::lsOutSide)||
	   (type==CPlotAxis::atBottom&&tickLabelSide==CPlotAxis::lsInSide))
	   {
			if(doRotation)
			{
				if(tickLabelRotation>0)
				{
					x=-mCos(radians)*labelData.totalBounds.width()+mSin(radians)*labelData.totalBounds.height()/2.0;
					y=-mSin(radians)*labelData.totalBounds.width()-mCos(radians)*labelData.totalBounds.height();
				}
				else
				{
					x=-mSin(-radians)*labelData.totalBounds.height()/2.0;
					y=-mCos(-radians)*labelData.totalBounds.height();
				}
			}
			else
			{
				x=-labelData.totalBounds.width()/2.0;
				y=-labelData.totalBounds.height();
			}
	   }
	else
	if((type==CPlotAxis::atBottom&&tickLabelSide==CPlotAxis::lsOutSide)||
	   (type==CPlotAxis::atTop&&tickLabelSide==CPlotAxis::lsInSide))
	   {
			if(doRotation)
			{
				if(tickLabelRotation>0)
				{
					x=mSin(radians)*labelData.totalBounds.height()/2.0;
					y=0;
				}
				else
				{
					x=-mCos(-radians)*labelData.totalBounds.width()-mSin(-radians)*labelData.totalBounds.height()/2.0;
					y=mSin(-radians)*labelData.totalBounds.width();
				}
				   
			}
			else
			{
				x=-labelData.totalBounds.width()/2.0;
				y=0;
			}
	   }
	   
	 return CPointF(x,y);
}

void CAxisPainterPrivate::getMaxTickLabelSize(const PlotFont& font, const std::string& text, CSize* tickLabelsSize)const
{
	
}




/*=====================Axis Rect=====================================*/

CAxisRect::CAxisRect(bool setupDefaultAxes):
mBackgroundBrush(Gemini::NoBrush),
mBackgroundScaled(true),
mRangeZoom(Gemini::Horizontal|Gemini::Vertical),
mRangeZoomFactorHorz(0.85),
mRangeZoomFactorVert(0.85),
mRect(0,0,100,100)
{
	mMiniumSize=CSize(50,50);
	mMinimumMargins=CMargin(15,15,15,15);
	mAxes.insert(std::pair<WORD,CList<CPlotAxis*> >(CPlotAxis::atLeft, CList<CPlotAxis*>()));
	mAxes.insert(std::pair<WORD,CList<CPlotAxis*> >(CPlotAxis::atRight,CList<CPlotAxis*>()));
	mAxes.insert(std::pair<WORD,CList<CPlotAxis*> >(CPlotAxis::atTop,CList<CPlotAxis*>()));
	mAxes.insert(std::pair<WORD,CList<CPlotAxis*> >(CPlotAxis::atBottom,CList<CPlotAxis*>()));
	
	if(setupDefaultAxes)
	{
		CPlotAxis* xAxis=addAxis(CPlotAxis::atBottom);
		CPlotAxis* yAxis=addAxis(CPlotAxis::atLeft);
		CPlotAxis* xAxis2=addAxis(CPlotAxis::atTop);
		CPlotAxis* yAxis2=addAxis(CPlotAxis::atRight);
		
		setRangeZoomAxes(xAxis, yAxis);
		xAxis->setVisible(true);
		yAxis->setVisible(true);
		xAxis2->setVisible(true);
		yAxis2->setVisible(true);
		xAxis->grid()->setVisible(true);
		yAxis->grid()->setVisible(true);
		xAxis2->grid()->setVisible(false);
		yAxis2->grid()->setVisible(false);
		xAxis2->grid()->setZeroLinePen(Gemini::NoPen);
		yAxis2->grid()->setZeroLinePen(Gemini::NoPen);
		
	}
}




CAxisRect::~CAxisRect()
{
	CList<CPlotAxis*> axesList=axes();
	for(int i=0; i<axesList.size(); ++i)
		removeAxis(axesList.at(i));
}


void CAxisRect::setRect(const CRect& rect)
{
	if(mRect!=rect)
	{
		mRect=rect;
	}
}
int CAxisRect::axisCount(WORD type)
{
	CList<CPlotAxis*> axes=mAxes[type];
	return axes.size();
}

CPlotAxis* CAxisRect::axis(WORD type, int index)
{
	CList<CPlotAxis*>ax(mAxes[type]);
	if(index>=0&&index<ax.size())
	{
		return ax.at(index);
	}
	else
	{
		return 0;
	}
}

CList<CPlotAxis*> CAxisRect::axes(WORD type)
{
	CList<CPlotAxis*> result;
	if(CPlotAxis::atLeft==(type&CPlotAxis::atLeft))
	{
		result.append(mAxes[CPlotAxis::atLeft]);
	}
	if(CPlotAxis::atRight==(type&CPlotAxis::atRight))
	{
		result.append(mAxes[CPlotAxis::atRight]);
	}
	if(CPlotAxis::atTop==(type&CPlotAxis::atTop))
	{
		result.append(mAxes[CPlotAxis::atTop]);
	}
	if(CPlotAxis::atBottom==(type&CPlotAxis::atBottom))
	{
		result.append(mAxes[CPlotAxis::atBottom]);
	}
	return result;
}

CList<CPlotAxis*> CAxisRect::axes()const
{
	CList<CPlotAxis*> result;
	std::map<WORD , CList<CPlotAxis*> >::const_iterator it=mAxes.begin();
	for(; it!=mAxes.end();++it)
	{
		result.append(it->second);
	}
	return result;
}

CPlotAxis* CAxisRect::addAxis(WORD type, CPlotAxis* axis)
{
	CPlotAxis* newAxis=axis;
	if(!newAxis)
	{
		newAxis=new CPlotAxis(this, type);
	}else{
		if(newAxis->axisType()!=type)
		{
			std::cout<<"Error: passed axis has different axis type than specified int type parameter"<<std::endl;
			return 0;
		}
		if(newAxis->axisRect()!=this)
		{
			std::cout<<"Error: passed axis has different axis rect"<<std::endl;
		}
		if(axes().contains(newAxis))
		{
			std::cout<<"Error: passed axis already  exist"<<std::endl;
			return 0;
		}
	}
	if(mAxes[type].size()>0)
	{
		bool invert=(type==CPlotAxis::atRight)||(type==CPlotAxis::atBottom);
		newAxis->setLowerEnding(CLineEnding(CLineEnding::HalfBar,6,10, !invert));
		newAxis->setUpperEnding(CLineEnding(CLineEnding::HalfBar,6,10, invert));
	}
	mAxes[type].push_back(newAxis);
	return newAxis;
}

CList<CPlotAxis*> CAxisRect::addAxes(WORD type)
{
	CList<CPlotAxis*> result;
	if(CPlotAxis::atLeft==(type&CPlotAxis::atLeft))
	{
		result.push_back(addAxis(CPlotAxis::atLeft));
	}
	if(CPlotAxis::atRight==(type&CPlotAxis::atRight))
	{
		result.push_back(addAxis(CPlotAxis::atRight));
	}
	if(CPlotAxis::atTop==(type&CPlotAxis::atTop))
	{
		result.push_back(addAxis(CPlotAxis::atTop));
	}
	if(CPlotAxis::atBottom==(type&CPlotAxis::atBottom))
	{
		result.push_back(addAxis(CPlotAxis::atBottom));
	}
	return result;
}

bool CAxisRect::removeAxis(CPlotAxis* axis)
{
	std::map<WORD , CList<CPlotAxis*> >::const_iterator it=mAxes.begin();
	for(; it!=mAxes.end();++it)
	{
		if(it->second.contains(axis))
		{
			mAxes.erase(it->first);
			delete axis;
			return true;
		}
	}
	return false;
}

void CAxisRect::zoom(const CRectF& pixelRect)
{
	zoom(pixelRect, axes());
}

void CAxisRect::zoom(const CRectF& pixelRect, const CList<CPlotAxis*>& affectedAxes)
{
	CList<CPlotAxis*>::const_iterator it=affectedAxes.begin();
	for(; it!=affectedAxes.end(); ++it)
	{
		CPlotAxis* axis=*it;
		if(!axis)	continue;
		CRange pixelRange;
		if(axis->orientation()==Gemini::Horizontal)
		{
			pixelRange=CRange(pixelRect.left(), pixelRect.right());
		}
		else 
			pixelRange=CRange(pixelRect.top(), pixelRect.bottom());
		axis->setRange(axis->pixelToCoord(pixelRange.lower), axis->pixelToCoord(pixelRange.upper));
	}
}

void CAxisRect::setupFullAxesBox(bool connectRanges)
{
	CPlotAxis *xAxis, *yAxis, *xAxis2, *yAxis2;
	if(axisCount(CPlotAxis::atBottom)==0)
		xAxis=addAxis(CPlotAxis::atBottom);
	else 
		xAxis=axis(CPlotAxis::atBottom);
	if(axisCount(CPlotAxis::atLeft)==0)
		yAxis=addAxis(CPlotAxis::atLeft);
	else
		yAxis=axis(CPlotAxis::atLeft);
	if(axisCount(CPlotAxis::atTop)==0)
		xAxis2=addAxis(CPlotAxis::atTop);
	else
		xAxis2=axis(CPlotAxis::atRight);
	if(axisCount(CPlotAxis::atRight)==0)
		yAxis2=addAxis(CPlotAxis::atRight);
	else
		yAxis2=axis(CPlotAxis::atRight);
	
	xAxis->setVisible(true);
	yAxis->setVisible(true);
	xAxis2->setVisible(true);
	yAxis2->setVisible(true);
	xAxis2->setTickLabels(false);
	yAxis2->setTickLabels(false);
	xAxis2->setRange(xAxis->range());
	xAxis2->setRangeReversed(xAxis->rangeReversed());
	xAxis2->setScaleType(xAxis->scaleType());
	xAxis2->setTicks(xAxis->ticks());
	xAxis2->setNumberFormat(xAxis->numberFormat());
	xAxis2->setNumberPrecision(xAxis->numberPrecision());
	xAxis2->ticker()->setTickCount(xAxis->ticker()->tickCount());
	xAxis2->ticker()->setTickOrigin(xAxis->ticker()->tickOrigin());
	
	yAxis2->setRange(yAxis->range());
	yAxis2->setRangeReversed(yAxis->rangeReversed());
	yAxis2->setScaleType(yAxis->scaleType());
	yAxis2->setTicks(yAxis->ticks());
	yAxis2->setNumberFormat(yAxis->numberFormat());
	yAxis2->setNumberPrecision(yAxis->numberPrecision());
	yAxis2->ticker()->setTickCount(yAxis->ticker()->tickCount());
	yAxis2->ticker()->setTickOrigin(yAxis2->ticker()->tickOrigin());
	
	
}

void CAxisRect::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aeAxes);
}

void CAxisRect::draw(CPlotPainter* painter)
{
	drawBackground(painter);
}

void CAxisRect::setBackground(const CImage& img)
{
	mBackgroundImage= img;
	mScaledBackgroundImage = CImage();
}

void CAxisRect::setBackground(const CBrush& brush)
{
	mBackgroundBrush= brush;
}
void CAxisRect::setBackground(const CImage& pm, bool scaled)
{
	mBackgroundImage= pm;
	mScaledBackgroundImage = CImage();
	mBackgroundScaled=scaled;
}

void CAxisRect::setBackgroundScaled(bool scaled)
{
	mBackgroundScaled=scaled;
}

CPlotAxis* CAxisRect::rangeZoomAxis(WORD type)
{
	if(type==Gemini::Horizontal)
		return mRangeZoomHorzAxis.empty()?0:mRangeZoomHorzAxis.first();
	else
		return mRangeZoomVertAxis.empty()?0:mRangeZoomVertAxis.first();
}

CList<CPlotAxis*> CAxisRect::rangeZoomAxes(WORD type)
{
	CList<CPlotAxis*> result;
	if(type==Gemini::Horizontal)
	{
		for(int i=0; i<mRangeZoomHorzAxis.size(); ++i)
		{
			if(mRangeZoomHorzAxis.at(i))
				result.push_back(mRangeZoomHorzAxis.at(i));
		}
	}
	else
	{
		for(int i=0; i<mRangeZoomVertAxis.size(); ++i)
		{
			if(mRangeZoomVertAxis.at(i))
				result.push_back(mRangeZoomVertAxis.at(i));
		}
	}
	return result;
}

double CAxisRect::rangeZoomFactor(WORD type)
{
	return (type==Gemini::Horizontal? mRangeZoomFactorHorz:mRangeZoomFactorVert);
}

void CAxisRect::setRangeZoom(WORD type)
{
	mRangeZoom =type;
}

void CAxisRect::setRangeZoomAxes(CPlotAxis* horizontal, CPlotAxis* vertical)
{
	CList<CPlotAxis*> horz, vert;
	if(horizontal)
		horz.push_back(horizontal);
	if(vertical)
		vert.push_back(vertical);
	setRangeZoomAxes(horz,vert);
}

void CAxisRect::setRangeZoomAxes(CList<CPlotAxis*>axes)
{
	CList<CPlotAxis*> horz, vert;
	CList<CPlotAxis*>::iterator it=axes.begin();
	for(; it!=axes.end(); ++it)
	{
		CPlotAxis* axis=*it;
		if(axis->orientation()==Gemini::Horizontal)
			horz.push_back(axis);
		else
			vert.push_back(axis);
	}
	setRangeZoomAxes(horz,vert);
}

void CAxisRect::setRangeZoomAxes(CList<CPlotAxis*> horizontal, CList<CPlotAxis*> vertical)
{
	//可能有内存泄漏
	mRangeZoomHorzAxis.clear();
	CList<CPlotAxis*>::iterator it=horizontal.begin();
	for(; it!=horizontal.end(); ++it)
	{
		CPlotAxis* ax=*it;
		if(ax)
			mRangeZoomHorzAxis.append(ax);
		else
			std::cout<<"Error: Invalid horizontal list"<<std::endl;
	}
	//可能有内存泄漏
	mRangeZoomVertAxis.clear();
	it=vertical.begin();
	for(; it!=vertical.end(); ++it)
	{
		CPlotAxis* ax=*it;
		if(ax)
			mRangeZoomVertAxis.append(ax);
		else
			std::cout<<"Error: Invalid vertical list"<<std::endl;
	}
}

void CAxisRect::setRangeZoomFactor(double factor)
{
	mRangeZoomFactorHorz=factor;
	mRangeZoomFactorVert=factor;
}


void CAxisRect::drawBackground(CPlotPainter* painter)
{
	if(mBackgroundBrush.style()!=Gemini::NoBrush)
		painter->fillRect(mRect, mBackgroundBrush);
	
	if(!mBackgroundImage.isNull())
	{
		if(mBackgroundScaled)
		{
			CSize scaledSize(mBackgroundImage.size());
			scaledSize.scale(mRect.size(),Gemini::KeepAspectRatioByExpanding);
			if(mScaledBackgroundImage.size()!=scaledSize)
				mScaledBackgroundImage=mBackgroundImage.scaled(mRect.size(),Gemini::KeepAspectRatioByExpanding,Gemini::SmoothTransformation);
			painter->drawImage(CPointF(mRect.topLeft()+CPoint(0,-1)),mScaledBackgroundImage, CRectF(CPoint(0,0),mRect.size())&mScaledBackgroundImage.rect());
		}
		else
		{
			painter->drawImage(CPointF(mRect.topLeft()+CPoint(0,-1)),mBackgroundImage, CRectF(CPoint(0,0),mRect.size()));
		}
	}
	
}

void CAxisRect::updateAxesOffset(WORD type)
{
	const CList<CPlotAxis*> axesList=mAxes[type];
	if(axesList.empty())
	{
		return ;
	}
	bool isFirstVisible=!axesList.first()->visible();
	for(int i=1; i<axesList.size(); ++i)
	{
		int offset=axesList.at(i-1)->offset()/*+axesList.at(i-1)->calculateMargin()*/;
		if(axesList.at(i)->visible())
		{
			if(!isFirstVisible)
				offset+=axesList.at(i)->tickLengthIn();
			isFirstVisible=false;
		}
		axesList.at(i)->setOffset(offset);
	}
}

CPlotAxis::CPlotAxis(CAxisRect* parent, WORD type):
mAxisType(type), mAxisRect(parent), mPadding(5),
mOrientation(orientation(type)),
mSelectableParts(spAxis|spTickLabels|spAxisLabel),
mSelectedParts(spNone),
mBasePen(CPen(Gemini::black,0,Gemini::SolidLine, Gemini::SquareCap)),
mSelectedBasePen(CPen(Gemini::blue,2)),
mLabel(),
mTickLabels(true),
mNumberPrecision(2),
mNumberFormat('g'),
mNumberBeautifulPowers(true),
mTicks(true),
mSubTicks(true),
mTickPen(CPen(Gemini::black,0,Gemini::SolidLine,Gemini::SquareCap)),
mSelectedTickPen(CPen(CPen(Gemini::blue,2))),
mSubTickPen(CPen(Gemini::black,0,Gemini::SolidLine,Gemini::SquareCap)),
mSelectedSubTickPen(CPen(Gemini::blue,2)),
mRange(0,5),
mRangeReversed(false),
mScaleType(Linear),
mGrid(new CPlotGrid(this)),
mAxisPainter(new CAxisPainterPrivate()),
mTicker(new CAxisTicker)
{
//	mGrid->setVisible(false);
	setAntialiased(false);
	if(type==atTop)
	{
		setTickLabelPadding(3);
		setLabelPadding(12);
	}
	else if(type==atRight)
	{
		setTickLabelPadding(7);
		setLabelPadding(12);
	}
	else if(type==atBottom)
	{
		setTickLabelPadding(3);
		setLabelPadding(12);
	}
	else if(type==atLeft)
	{
		setTickLabelPadding(5);
		setLabelPadding(10);
	}
	mLabelFont.width=16;
	mLabelFont.height=16;
	mLabelFont.color=Gemini::black;
	
	mSelectedLabelFont.width=16;
	mSelectedLabelFont.height=16;
	mSelectedLabelFont.color=Gemini::red;
	
	mTickLabelFont.width=16;
	mTickLabelFont.height=16;
	mTickLabelFont.color=Gemini::black;
	
	mSelectedTickLabelFont.width=16;
	mSelectedTickLabelFont.height=16;
	mSelectedTickLabelFont.color=Gemini::red;
}

CPlotAxis::~CPlotAxis()
{
	delete mAxisPainter;
	delete mTicker;
	delete mGrid;
}

int CPlotAxis::tickLabelPadding()const
{
	return mAxisPainter->tickLabelPadding;
}

double CPlotAxis::tickLabelRotation()const
{
	return mAxisPainter->tickLabelRotation;
}

WORD CPlotAxis::tickLabelSide()const
{
	return mAxisPainter->tickLabelSide;
}

std::string CPlotAxis::numberFormat()const
{
	std::string result;
	result=result+mNumberFormat;
	if(mNumberBeautifulPowers)
	{
		result=result+'b';
		if(mAxisPainter->numberMultiplyCross)
			result=result+'c';
	}
	return result;
}

int CPlotAxis::tickLengthIn()const
{
	return mAxisPainter->tickLengthIn;
}
int CPlotAxis::tickLengthOut()const
{
	return mAxisPainter->tickLengthOut;
}
int CPlotAxis::subTickLengthIn()const
{
	return mAxisPainter->subTickLengthIn;
}
int CPlotAxis::subTickLengthOut()const
{
	return mAxisPainter->subTickLengthOut;
}
int CPlotAxis::labelPadding()const
{
	return mAxisPainter->labelPadding;
}

int CPlotAxis::offset()const
{
	return mAxisPainter->offset;
}

CLineEnding CPlotAxis::lowerEnding()const
{
	return mAxisPainter->lowerEnding;
}

CLineEnding CPlotAxis::upperEnding()const
{
	return mAxisPainter->upperEnding;
}

void CPlotAxis::setScaleType(WORD type)
{
	if(mScaleType!=type)
	{
		mScaleType=type;
		if(mScaleType==Logarithmic)
			setRange(mRange.sanitizedForLogScale());
	}
}

void CPlotAxis::setRange(const CRange& range)
{
	if(range.lower==mRange.lower&&range.upper==mRange.upper)
		return ;
	if(!CRange::validRange(range)) return ;
	CRange oldRange =mRange;
	if(mScaleType==Logarithmic)
	{
		mRange=range.sanitizedForLogScale();
	}
	else
	{
		mRange=range.sanitizedForLinScale();
	}
}

void CPlotAxis::setSelectableParts(WORD selectable)
{
	if(mSelectableParts !=selectable)
	{
		mSelectableParts=selectable;
	}
}

void CPlotAxis::setSelectedParts(WORD selected)
{
	if(mSelectedParts!=selected)
	{
		mSelectedParts=selected;
	}
}

void CPlotAxis::setRange(double lower, double upper)
{
	if(lower==mRange.lower&&upper==mRange.upper)
		return;
	if(!CRange::validRange(lower, upper)) return ;
	CRange oldRange=mRange;
	mRange.lower=lower;
	mRange.upper =upper;
	if(mScaleType==Logarithmic)
	{
		mRange=mRange.sanitizedForLogScale();
	}
	else
	{
		mRange=mRange.sanitizedForLinScale();
	}
	
}

void CPlotAxis::setRangeLower(double lower)
{
	if(lower==mRange.lower)
		return;
	CRange oldRange=mRange;
	mRange.lower=lower;
	if(mScaleType==Logarithmic)
	{
		mRange=mRange.sanitizedForLogScale();
	}
	else
	{
		mRange=mRange.sanitizedForLinScale();
	}
}

void CPlotAxis::setRangeUpper(double upper)
{
	if(upper==mRange.upper)	return ;
	CRange oldRange=mRange;
	mRange.upper=upper;
	if(mScaleType==Logarithmic)
	{
		mRange=mRange.sanitizedForLogScale();
	}
	else
	{
		mRange=mRange.sanitizedForLinScale();
	}
}


void CPlotAxis::setRangeReversed(bool reversed)
{
	mRangeReversed=reversed;
}

void CPlotAxis::setTicker(CAxisTicker* ticker)
{
	if(ticker!=mTicker)
	{
		if(mTicker)
		{
			delete mTicker;
		}
		mTicker=ticker;
	}
}

void CPlotAxis::setTicks(bool show)
{
	mTicks=show;
}

void CPlotAxis::setTickLabels(bool show)
{
	mTickLabels=show;
	if(!mTickLabels)
	{
		mTickVectorLabels.clear();
	}
}

void CPlotAxis::setTickLabelPadding(int padding)
{
	mAxisPainter->tickLabelPadding=padding;
}

void CPlotAxis::setTickLabelFont(const PlotFont& font)
{
		mTickLabelFont=font;
}

void CPlotAxis::setTickLabelRotation(double degress)
{
	if(!fuzzyIsNull(degress-mAxisPainter->tickLabelRotation))
	{
		mAxisPainter->tickLabelRotation=mBound(-90.0,degress,90.0);
	}
}

void CPlotAxis::setTickLabelSide(WORD side)
{
	mAxisPainter->tickLabelSide=side;
}

void CPlotAxis::setNumberFormat(const std::string& format)
{
	if(format.empty())
	{
		return ;
	}
	else
	{
		mNumberFormat=format[0];
		if(format.length()<2)
		{
			mNumberBeautifulPowers=false;
			mAxisPainter->numberMultiplyCross=false;
			return;
		}
		if((format[1]=='b')&&(((format[0]=='e')||(format[0]=='g'))))
		{
			mNumberBeautifulPowers=true;
		}
		if(format.length()<3)
		{
			mAxisPainter->numberMultiplyCross=false;
			return;
		}
		if(format[2]=='c')
		{
			//cross
			mAxisPainter->numberMultiplyCross=true;
		}
		else if(format[2]=='d')
		{
			//dot
			mAxisPainter->numberMultiplyCross=false;
		}
		else
		{
			return ;
		}
	}
}

void CPlotAxis::setNumberPrecision(int precision)
{
	if(mNumberPrecision!=precision)
	{
		mNumberPrecision=precision;
	}
}

void CPlotAxis::setTickLength(int inside, int outside)
{
	setTickLengthIn(inside);
	setTickLengthOut(outside);
}

void CPlotAxis::setTickLengthIn(int inside)
{
	if(mAxisPainter->tickLengthIn!=inside)
	{
		mAxisPainter->tickLengthIn=inside;
	}
}

void CPlotAxis::setTickLengthOut(int outside)
{
	if(mAxisPainter->tickLengthOut!=outside)
	{
		mAxisPainter->tickLengthOut=outside;
	}
}

void CPlotAxis::setSubTicks(bool show)
{
	if(mSubTicks!=show)
	{
		mSubTicks=show;
	}
}


void CPlotAxis::setSubTickLength(int inside, int outside)
{
	setSubTickLengthIn(inside);
	setSubTickLengthOut(outside);
}
void CPlotAxis::setSubTickLengthIn(int inside)
{
	if(mAxisPainter->subTickLengthIn!=inside)
	{
		mAxisPainter->subTickLengthIn=inside;
	}
}

void CPlotAxis::setSubTickLengthOut(int outside)
{
	if(mAxisPainter->subTickLengthOut!=outside)
	{
		mAxisPainter->subTickLengthOut=outside;
	}
}

void CPlotAxis::setBasePen(const CPen& pen)
{
	mBasePen=pen;
}
void CPlotAxis::setTickPen(const CPen& pen)
{
	mTickPen=pen;
}

void CPlotAxis::setSubTickPen(const CPen& pen)
{
	mSubTickPen=pen;
}

void CPlotAxis::setLabelFont(const PlotFont& font)
{
	mLabelFont=font;
}

void CPlotAxis::setLabel(const std::string& text)
{
	if(mLabel!=text)
	{
		mLabel=text;
	}
}

void CPlotAxis::setLabelPadding(int padding)
{
	if(mAxisPainter->labelPadding!=padding)
	{
		mAxisPainter->labelPadding=padding;
	}
}

void CPlotAxis::setPadding(int padding)
{
	if(mPadding!=padding)
	{
		mPadding=padding;
	}
}

void CPlotAxis::setOffset(int offset)
{
	if(mAxisPainter->offset!=offset)
	{
		mAxisPainter->offset=offset;
	}
}

void CPlotAxis::setSelectedTickLabelFont(const PlotFont& font)
{
	mSelectedTickLabelFont=font;
}

void CPlotAxis::setSelectedLabelFont(const PlotFont& font)
{
	mSelectedLabelFont = font;
}

void CPlotAxis::setSelectedBasePen(const CPen& pen)
{
	mSelectedBasePen= pen;
}

void CPlotAxis::setSelectedTickPen(const CPen& pen)
{
	mSelectedTickPen=pen;
}

void CPlotAxis::setSelectedSubTickPen(const CPen& pen)
{
	mSelectedSubTickPen=pen;
}

void CPlotAxis::setLowerEnding(const CLineEnding& ending)
{
	mAxisPainter->lowerEnding=ending;
}
void CPlotAxis::setUpperEnding(const CLineEnding& ending)
{
	mAxisPainter->upperEnding = ending;
}

void CPlotAxis::moveRange(double diff)
{
	CRange oldRange=mRange;
	if(mScaleType==Linear)
	{
		mRange.lower+=diff;
		mRange.upper+=diff;
	}
	else{
		mRange.lower*=diff;
		mRange.upper*=diff;
	}
}

void CPlotAxis::scaleRange(double factor)
{
	scaleRange(factor, range().center());
}

void CPlotAxis::scaleRange(double factor, double center)
{
	CRange oldRange=mRange;
	if(mScaleType==Linear)
	{
		CRange newRange;
		newRange.lower=(mRange.lower-center)*factor+center;
		newRange.upper=(mRange.upper-center)*factor+center;
		if(CRange::validRange(newRange))
			mRange=newRange.sanitizedForLinScale();
	}
	else
	{
		if((mRange.upper<0&&center<0)||(mRange.upper>0&&center>0))
		{
			CRange newRange;
			newRange.lower=mPow(mRange.lower/center,factor)*center;
			newRange.upper=mPow(mRange.upper/center,factor)*center;
			if(CRange::validRange(newRange))
			{
				mRange=newRange.sanitizedForLogScale();
			}
		}
	}
}

void CPlotAxis::setRange(double position, double size, WORD align)
{
	if(align==0x01)
	{
		//left align
		setRange(position, position+size);
	}
	else if(align==0x02)
	{
		setRange(position-size,size);
	}
	else
	{
		setRange(position-size/2.0, position+size/2.0);
	}
}

void CPlotAxis::setScaleRatio(const CPlotAxis* otherAxis, double ratio)
{
	int otherPixelSize, ownPixelSize;
	if(otherAxis->orientation()==Gemini::Horizontal)
	{
		otherPixelSize=otherAxis->axisRect()->width();
	}
	else
		otherPixelSize=otherAxis->axisRect()->height();
	
	if(orientation()==Gemini::Horizontal)
	{
		ownPixelSize=axisRect()->width();
	}
	else
	{
		ownPixelSize=axisRect()->height();
	}
	double newRangeSize=ratio*otherAxis->range().size()*ownPixelSize/(double)otherPixelSize;
	setRange(range().center(),newRangeSize,0x03);
}

double CPlotAxis::pixelToCoord(double value)const
{
	if(orientation()==Gemini::Horizontal)
	{
		if(mScaleType==Linear)
		{
			if(!mRangeReversed)
				return(value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.lower;
			else
				return -(value-mAxisRect->left())/(double)mAxisRect->width()*mRange.size()+mRange.upper;
		}
		else
		{
			//Logarithmic
			if(!mRangeReversed)
			{
				return mPow(mRange.upper/mRange.lower,(value-mAxisRect->left())/(double)mAxisRect->width())*mRange.lower;
			}
			else
			{
				return mPow(mRange.upper/mRange.lower,(mAxisRect->left()-value)/(double)mAxisRect->width())*mRange.upper;
			}
		}
		
	}
	else
	{
		//Vertical
		if(mScaleType==Linear)
		{
			if(!mRangeReversed)
			{
/* 				std::cout<<mAxisRect->bottom()<<std::endl;
				std::cout<<value<<std::endl;
				std::cout<<mAxisRect->height()<<std::endl;
				std::cout<<mRange.size()<<std::endl;
				std::cout<<mRange.lower<<std::endl; */
				return (mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.lower;
			}
			else
			{
				return -(mAxisRect->bottom()-value)/(double)mAxisRect->height()*mRange.size()+mRange.upper;
			}
		}
		else
		{
			if(!mRangeReversed)
			{
				return mPow(mRange.upper/mRange.lower,(mAxisRect->bottom()-value)/(double)mAxisRect->height())*mRange.lower;
			}
			else
			{
				return mPow(mRange.upper/mRange.lower,(value-mAxisRect->bottom())/(double)mAxisRect->height())*mRange.upper;
			}
		}
	}
}

double CPlotAxis::coordToPixel(double value)const
{
	if(orientation()==Gemini::Horizontal)
	{
		if(mScaleType==Linear)
		{
			if(!mRangeReversed)
			{
				return (value-mRange.lower)/mRange.size()*mAxisRect->width()+mAxisRect->left();
			}
			else
			{
				return (mRange.upper-value)/mRange.size()*mAxisRect->width()+mAxisRect->left();
			}
		}
		else
		{
			if(value>=0&&mRange.upper<0) //invalid value for logarithmic scale, just draw it outside visible range;
			{
				return !mRangeReversed? mAxisRect->right()+200:mAxisRect->left()-200;
			}	
			else if(value<=0&&mRange.upper>0)
			{
				return !mRangeReversed? mAxisRect->left()-200:mAxisRect->right()+200;
			}
			else
			{
				//Logarithmic
				if(!mRangeReversed)
				{
					return mLn(value/mRange.lower)/mLn(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
				}
				else
				{
					return mLn(mRange.upper/value)/mLn(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
					
				}
			}
		}
	}
	else
	{
		//Vertical
		if(mScaleType==Linear)
		{
			if(!mRangeReversed)
			{
				return mAxisRect->bottom()-(value-mRange.lower)/mRange.size()*mAxisRect->height();
			}
			else
			{
				return mAxisRect->bottom()-(mRange.upper-value)/mRange.size()*mAxisRect->height();
				
			}
		}
		else
		{
			if(value>=0&&mRange.upper<0)
			{
				return !mRangeReversed? mAxisRect->top()-200: mAxisRect->bottom()+200;
			}
			else if(value<=0&&mRange.upper>0)
			{
				return !mRangeReversed? mAxisRect->bottom()+200:mAxisRect->top()-200;
			}
			//Logarithmic
			else 
			{
				if(!mRangeReversed)
				{
					return mAxisRect->bottom()-mLn(value/mRange.lower)/mLn(mRange.upper/mRange.lower)*mAxisRect->height();
				}
				else
				{
					return mAxisRect->bottom()-mLn(mRange.upper/value)/mLn(mRange.upper/mRange.lower)*mAxisRect->height();
				}
			}
		}
	}
}


WORD CPlotAxis::getPartAt(const CPointF& pos)const
{
	/* if(!mVisible)
		return spNone;
	
	if(mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
		return spAxis;
	else if(mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
		return spTickLabels;
	else if(mAxisPainter->labelSelectionBaox().contains(pos.toPoint()))
		return spAxisLabel;
	else
		return spNone; */
	return 0;

}

WORD CPlotAxis::opposite(WORD type)
{
	switch(type)
	{
		case atLeft: return atRight; break;
		case atRight: return atLeft; break;
		case atBottom: return atTop; break;
		case atTop: return atBottom; break;
		default: return atLeft; break;
	}
}

void CPlotAxis::draw(CPlotPainter* painter)
{
	CVector<double> subTickPositions;
	CVector<double> tickPositions;
	CVector<std::string>tickLabels;
	tickPositions.reserve(mTickVector.size());
	tickLabels.reserve(mTickVector.size());
	subTickPositions.reserve(mSubTickVector.size());
	
	if(mTicks)
	{
		for(int i=0; i<mTickVector.size(); ++i)
		{
			tickPositions.append(coordToPixel(mTickVector.at(i)));
			if(mTickLabels)
				tickLabels.append(mTickVectorLabels.at(i));
		}
		
		if(mSubTicks)
		{
			const int subTickCount=mSubTickVector.size();
			for(int i=0; i<subTickCount; ++i)
			{
				subTickPositions.append(coordToPixel(mSubTickVector.at(i)));
			}
		}
	}
	
	mAxisPainter->type=mAxisType;
	mAxisPainter->basePen=getBasePen();
	mAxisPainter->labelFont=getLabelFont();
	mAxisPainter->label=mLabel;
	mAxisPainter->substituteExponent=mNumberBeautifulPowers;
	mAxisPainter->tickPen=getTickPen();
	mAxisPainter->subTickPen=getSubTickPen();
	mAxisPainter->tickLabelFont=getTickLabelFont();
	mAxisPainter->axisRect=mAxisRect->rect();
	mAxisPainter->abbreviateDecimalPowers=mScaleType==Logarithmic;
	mAxisPainter->reversedEndings= mRangeReversed;
	mAxisPainter->tickLabels=tickLabels.toStdVector();
	mAxisPainter->tickPositions=tickPositions;
	mAxisPainter->subTickPositions=subTickPositions;
	mAxisPainter->draw(painter);
}


void CPlotAxis::setupTickVectors()
{
	if((!mTicks&&!mTickLabels&&mGrid->visible())||mRange.size()<=0) return;
	std::vector<std::string> oldLabels=mTickVectorLabels;
	mTicker->generate(mRange,mNumberFormat,mNumberPrecision,mTickVector, mSubTicks?&mSubTickVector:0,mTickLabels? &mTickVectorLabels:0);
	
}

CPen CPlotAxis::getBasePen()const
{
	return (mSelectedParts&spAxis)==spAxis? mSelectedBasePen:mBasePen;
}
CPen CPlotAxis::getTickPen()const
{
	return (mSelectedParts&spAxis)==spAxis? mSelectedTickPen:mTickPen;
}

CPen CPlotAxis::getSubTickPen()const
{
	return (mSelectedParts&spAxis)==spAxis?mSelectedSubTickPen:mSubTickPen;
}

PlotFont CPlotAxis::getTickLabelFont()const
{
	return (mSelectedParts&spTickLabels)==spTickLabels?mSelectedTickLabelFont:mTickLabelFont;
}

PlotFont CPlotAxis::getLabelFont()const
{
	return (mSelectedParts==spAxisLabel)==spAxisLabel?mSelectedLabelFont:mLabelFont;
}



void CPlotAxis::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aeAxes);
}

/*=======================Plot Grid===================================*/

CPlotGrid::CPlotGrid(CPlotAxis* parentAxis):
mParentAxis(parentAxis)
{
//	setParent(parentAxis);
	setPen(CPen(0xFFC8C8C8, 0, Gemini::DotLine));
	setSubGridPen(CPen(0xFFDCDCDC, 0, Gemini::DotLine));
	setZeroLinePen(CPen(0xFFC8C8C8, 0, Gemini::SolidLine));
	setSubGridVisible(false);
//	setAntialiased(false);
	setAntialiasedSubGrid(false);
	setAntialiasedZeroLine(false);
}

CPlotAxis* CPlotGrid::parent()const
{
	return mParentAxis;
}
void CPlotGrid::setParent(CPlotAxis* parentAxis)
{
	if(mParentAxis!=parentAxis)
	{
		mParentAxis=parentAxis;
	}
}

void CPlotGrid::setSubGridVisible(bool visible)
{
	mSubGridVisible=visible;
}

void CPlotGrid::setAntialiasedSubGrid(bool enabled)
{
	mAntialiasedSubGrid=enabled;
}

void CPlotGrid::setAntialiasedZeroLine(bool enabled)
{
	mAntialiasedZeroLine = enabled;
}

void CPlotGrid::setPen(const CPen& pen)
{
	mPen=pen;
}

void CPlotGrid::setSubGridPen(const CPen& pen)
{
	mSubGridPen = pen;
}

void CPlotGrid::setZeroLinePen(const CPen& pen)
{
	mZeroLinePen = pen;
}

void CPlotGrid::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	;
}

void CPlotGrid::draw(CPlotPainter* painter)
{
	if(!mParentAxis){
		std::cout<<"Error: invalid parent axis"<<std::endl;
		return ;
	}
	if(mVisible)
	{
		if(mParentAxis->subTicks()&&mSubGridVisible)
		{
			drawSubGridLines(painter);
		}
	
		drawGridLines(painter);
	}
}


void CPlotGrid::drawGridLines(CPlotPainter* painter)const
{
	if(!mParentAxis){
		std::cout<<"Error: invalid parent axis"<<std::endl;
	}
	const int tickCount=mParentAxis->mTickVector.size();
	double t;
	if(mParentAxis->orientation()==Gemini::Horizontal)
	{
		//draw zeroline:
		int zeroLineIndex=-1;
		if(mZeroLinePen.style()!=Gemini::NoPen&&mParentAxis->mRange.lower<0&&mParentAxis->mRange.upper>0)
		{
			painter->setPen(mZeroLinePen);
			//compare double to zero
			double epsilon=mParentAxis->range().size()*1e-6;
			for(int i=0; i<tickCount; ++i)
			{
				if(mAbs(mParentAxis->mTickVector.at(i))<epsilon)
				{
					zeroLineIndex=i;
					//x for vertical zeroline
					t=mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i));
					painter->drawLine(CLineF(t,mParentAxis->mAxisRect->bottom(),t,mParentAxis->mAxisRect->top()));
					break;
				}
			}
		}
		
		//draw grid lines:
		applyDefaultAntialiasingHint(painter);
		painter->setPen(mPen);
		for(int i=0; i<tickCount; ++i)
		{
			//don't draw a gridline on to of the zeroline
			if(i==zeroLineIndex) continue;
			t=mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i));
			painter->drawLine(CLineF(t, mParentAxis->mAxisRect->bottom(),t,mParentAxis->mAxisRect->top()));
		}
	}
	else
	{
		//vertical
		//draw zeroline
		int zeroLineIndex = -1;
		if(mZeroLinePen.style()!=Gemini::NoPen&&mParentAxis->mRange.lower<0&&mParentAxis->mRange.upper>0)
		{
			painter->setPen(mZeroLinePen);
			double epsilon=mParentAxis->mRange.size()*1e-6;
			for(int i=0; i<tickCount; ++i)
			{
				if(mAbs(mParentAxis->mTickVector.at(i))<epsilon)
				{
					zeroLineIndex=i;
					t= mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i));//yAxis
					painter->drawLine(CLineF(mParentAxis->mAxisRect->left(),t,mParentAxis->mAxisRect->right(),t));
					break;
				}
			}
		}
		//draw grid lines:
		applyDefaultAntialiasingHint(painter);
		painter->setPen(mPen);
		for(int i=0; i<tickCount; ++i)
		{
			if(i==zeroLineIndex) continue;
			t=mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i));
			painter->drawLine(CLineF(mParentAxis->mAxisRect->left(),t,mParentAxis->mAxisRect->right(),t));
		}
	}
}


void CPlotGrid::drawSubGridLines(CPlotPainter* painter)const
{
	if(!mParentAxis)
	{
		std::cout<<"Error: invalid parent axis"<<std::endl;
		return;
	}
	
	double t;
	painter->setPen(mSubGridPen);
	if(mParentAxis->orientation()==Gemini::Horizontal)
	{
		for(int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
		{
			t=mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i));
			painter->drawLine(CLineF(t, mParentAxis->mAxisRect->bottom(),t,mParentAxis->mAxisRect->top()));
		}
	}
	else
	{
		for(int i=0; i<mParentAxis->mSubTickVector.size(); ++i)
		{
			t=mParentAxis->coordToPixel(mParentAxis->mSubTickVector.at(i));
			painter->drawLine(CLineF(mParentAxis->mAxisRect->left(),t,mParentAxis->mAxisRect->right(),t));
		}
	}
}









/*===========================Layer =========================================*/

CLayerable::CLayerable(std::string targetLayer, CLayerable* parentLayerable)
:mVisible(true),
mParentLayerable(parentLayerable),
mLayer(0),
mAntialiased(true)
{
	
}

CLayerable::~CLayerable()
{
	if(mLayer)
	{
		mLayer->removeChild(this);
		mLayer = 0;
	}
}

void CLayerable::setVisible(bool on)
{
	mVisible = on;
}

bool CLayerable::setLayer(CLayer* layer)
{
	return moveToLayer(layer, false);
}

bool CLayerable::setLayer(const std::string& layerName)
{
	return true;
}

void  CLayerable::setAntialiased(bool enabled)
{
	mAntialiased = enabled;
}

bool CLayerable::realVisibility()const
{
	return mVisible&&(!mLayer||mLayer->visible())&&(!mParentLayerable||mParentLayerable->realVisibility());
}

void CLayerable::setParentLayerable(CLayerable* parentLayerable)
{
	mParentLayerable = parentLayerable;
}

bool CLayerable::moveToLayer(CLayer* layer, bool prepend)
{
//	CLayer* oldLayer= mLayer;
	if(mLayer)
		mLayer->removeChild(this);
	mLayer= layer;
	if(mLayer)
		mLayer->addChild(this, prepend);
	return true;
}

void CLayerable::applyAntialiasingHint(CPlotPainter* painter, bool localAntialiased, WORD element)const
{
	painter->setAntialiasing(localAntialiased);
}

CRect CLayerable::clipRect()const
{
	return CRect();
}


CLayer::CLayer(const std::string& layerName)
:mName(layerName),
mIndex(-1),
mVisible(true)
{
	
}

CLayer::~CLayer()
{
	while(!mChildren.isEmpty())
	{
		mChildren.last()->setLayer(0);
	}
}

void CLayer::setVisible(bool visible)
{
	mVisible= visible;
}

void CLayer::draw(CPlotPainter* painter)
{
	for(int i=0; i<mChildren.size(); ++i)
	{
		CLayerable* child= mChildren[i];
		if(child->realVisibility())
		{
			painter->save();
			painter->setClipRect(child->clipRect().translated(0,-1));
			child->applyDefaultAntialiasingHint(painter);
			child->draw(painter);
			painter->restore();
		}
	}
}

void CLayer::replot()
{
	;
}

void CLayer::addChild(CLayerable* layerable, bool prepend)
{
	if(!mChildren.contains(layerable))
	{
		if(prepend)
			mChildren.prepend(layerable);
		else
			mChildren.append(layerable);
	}
}

void CLayer::removeChild(CLayerable* layerable)
{
	mChildren.removeOne(layerable);
}


CAbstractPlottable::CAbstractPlottable(CPlotAxis* keyAxis, CPlotAxis* valueAxis):
	CLayerable(std::string(), valueAxis->axisRect()),mName(),
	mAntialiasedFill(true),
	mAntialiasedScatters(true),
	mPen(Gemini::black),
	mBrush(Gemini::NoBrush),
	mKeyAxis(keyAxis),
	mValueAxis(valueAxis),
	mSelectable(Plot::stWhole),
	mSelectionDecorator(0)
{
	if(keyAxis->orientation()==valueAxis->orientation())
		std::cout<<"Eroror: keyAxis and valueAxis mubst be orthogonal to each other"<<std::endl;
	setSelectionDecorator(new CSelectionDecorator);
}


CAbstractPlottable::~CAbstractPlottable()
{
	delete mSelectionDecorator;
}

void CAbstractPlottable::setSelection(CDataSelection selection)
{
	selection.enforceType(mSelectable);
	if(mSelection!=selection)
	{
		mSelection=selection;
	}
}

void CAbstractPlottable::setSelectable(WORD selectable)
{
	if(mSelectable!=selectable)
	{
		mSelectable=selectable;
		CDataSelection oldSelection=mSelection;
		mSelection.enforceType(mSelectable);
		if(mSelection!=oldSelection)
		{
			;
		}
	}
}

void CAbstractPlottable::setName(const std::string& name)
{
	mName = name;
}

void CAbstractPlottable::setAntialiasedFill(bool enabled)
{
	mAntialiasedFill=enabled;
}

void CAbstractPlottable::setAntialiasedScatters(bool enabled)
{
	mAntialiasedScatters=enabled;
}


void CAbstractPlottable::setPen(const CPen& pen)
{
	mPen= pen;
}

void CAbstractPlottable::setBrush(const CBrush& brush)
{
	mBrush= brush;
}

void CAbstractPlottable::setKeyAxis(CPlotAxis* axis)
{
	mKeyAxis= axis;
}

void CAbstractPlottable::setValueAxis(CPlotAxis* axis)
{
	mValueAxis = axis;
}

void CAbstractPlottable::setSelectionDecorator(CSelectionDecorator* decorator)
{
	if(decorator)
	{
		if(decorator->registerWidthPlottable(this))
		{
			if(mSelectionDecorator)
				delete mSelectionDecorator;
			mSelectionDecorator =decorator;
		}
	}
	else if(mSelectionDecorator)
	{
		delete mSelectionDecorator;
		mSelectionDecorator=0;
	}
}

void CAbstractPlottable::coordsToPixels(double key, double value, double& x, double& y)const
{
	CPlotAxis* keyAxis= mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	
	if(!keyAxis||!valueAxis)
	{
		std::cout<<"Error: invalid key or value axis"<<std::endl;
		return ;
	}
	
	if(keyAxis->orientation()==Gemini::Horizontal)
	{
		x=keyAxis->coordToPixel(key);
		y=mValueAxis->coordToPixel(value);
	}
	else
	{
		y= keyAxis->coordToPixel(key);
		x= valueAxis->coordToPixel(value);	
	}
}


const CPointF CAbstractPlottable::coordsToPixels(double key, double value)const
{
	CPlotAxis* keyAxis = mKeyAxis;
	CPlotAxis* valueAxis = mValueAxis;
	if(!keyAxis||!valueAxis)
	{
		std::cout<<"Error: Invalid key or value axis"<<std::endl;
		return CPointF();
	}
	
	if(keyAxis->orientation()==Gemini::Horizontal)
	{
		return CPointF(keyAxis->coordToPixel(key),valueAxis->coordToPixel(value));
	}
	else
	{
		return CPointF(valueAxis->coordToPixel(value), keyAxis->coordToPixel(key));
	}
}


void CAbstractPlottable::pixelsToCoords(double x, double y, double& key, double& value)const
{
	CPlotAxis* keyAxis = mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	
	if(!keyAxis||!valueAxis)
	{
		std::cout<<"Error: Invalid key or value axis"<<std::endl;
		return;
	}
	
	if(keyAxis->orientation()==Gemini::Horizontal)
	{
		key = keyAxis->pixelToCoord(x);
		value = valueAxis->pixelToCoord(y);
	}
	else
	{
		key = keyAxis->pixelToCoord(y);
		value = valueAxis->pixelToCoord(x);
	}
}



void  CAbstractPlottable::pixelsToCoords(const CPointF& pixelPos, double& key, double& value)const
{
	pixelsToCoords(pixelPos.x(), pixelPos.y(), key, value);	
}

void CAbstractPlottable::rescaleAxes(bool onlyEnlarge)const
{
	rescaleKeyAxis(onlyEnlarge);
	rescaleValueAxis(onlyEnlarge);
}


void CAbstractPlottable::rescaleKeyAxis(bool onlyEnlarge)const
{
	CPlotAxis* keyAxis = mKeyAxis;
	
	if(!keyAxis)
	{
		std::cout<<"Error: Invalid key axis"<<std::endl;
	}
	
	WORD signDomain = Plot::sdBoth;
	if(keyAxis->scaleType()==CPlotAxis::Logarithmic)
	{
		signDomain = (keyAxis->range().upper<0?Plot::sdNegative:Plot::sdPositive);
	}
	
	bool foundRange;
	
	CRange newRange =getKeyRange(foundRange, signDomain);
	if(foundRange)
	{
		if(onlyEnlarge)
			newRange.expand(keyAxis->range());
		if(!CRange::validRange(newRange))
		{
			double center=(newRange.lower+newRange.upper)*0.5;
			if(keyAxis->scaleType()==CPlotAxis::Linear)
			{
				newRange.lower = center-keyAxis->range().size()/2.0;
				newRange.upper = center+keyAxis->range().size()/2.0;
			}
			else
			{
				newRange.lower= center/mSqrt(keyAxis->range().upper/keyAxis->range().lower);
				newRange.upper= center*mSqrt(keyAxis->range().upper/keyAxis->range().lower);
			}
		}
		
		keyAxis->setRange(newRange);
	}
}


void CAbstractPlottable::rescaleValueAxis(bool onlyEnlarge, bool inKeyRange)const
{
	CPlotAxis* keyAxis= mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)
	{
		std::cout<<"Error: Invalid key or value axis";
		return ;	
	}
	
	WORD signDomain = Plot::sdBoth;
	if(valueAxis->scaleType()==CPlotAxis::Logarithmic)
		signDomain=(valueAxis->range().upper<0? Plot::sdNegative:Plot::sdPositive);

	bool foundRange;
	
	CRange newRange = getValueRange(foundRange, signDomain, inKeyRange? keyAxis->range():CRange());
	if(foundRange)
	{
		if(onlyEnlarge)
			newRange.expand(valueAxis->range());
		if(!CRange::validRange(newRange))
		{
			double center=(newRange.lower+newRange.upper)*0.5;
			if(valueAxis->scaleType()==CPlotAxis::Linear)
			{
				newRange.lower= center-valueAxis->range().size()/2.0;
				newRange.upper=center+valueAxis->range().size()/2.0;
					
			}else
			{
				newRange.lower=center/mSqrt(valueAxis->range().upper/valueAxis->range().lower);
				newRange.upper=center*mSqrt(valueAxis->range().upper/valueAxis->range().lower);	
			}	
		}
		valueAxis->setRange(newRange);	
	}
}


CRect CAbstractPlottable::clipRect()const
{
	if(mKeyAxis&&mValueAxis)
	{
		return mKeyAxis->axisRect()->rect()&mValueAxis->axisRect()->rect();
	}
	else
		return CRect();
}

void CAbstractPlottable::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aePlottables);	
}



void CAbstractPlottable::applyFillAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aePlottables);
}


void CAbstractPlottable::applyScattersAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aePlottables);	
}




CGraph::CGraph(CPlotAxis* keyAxis, CPlotAxis* valueAxis)
: CAbstractPlottable1D<CGraphData>(keyAxis,valueAxis)
{
	setPen(CPen(Gemini::blue,0));
	setBrush(Gemini::NoBrush);
	setLineStyle(lsLine);
	setScatterSkip(0);
	setChannelFillGraph(0);
	setAdaptiveSampling(true);
}

CGraph::~CGraph()
{
	
}

void CGraph::setData(CSharedPointer<CGraphDataContainer> data)
{
	mDataContainer = data;
}

void CGraph::setData(const CVector<double>& keys, const CVector<double>& values,bool alreadySorted)
{
	mDataContainer->clear();
	addData(keys, values, alreadySorted);
}

void CGraph::setLineStyle(WORD ls)
{
	mLineStyle= ls;
}

void CGraph::setScatterStyle(const CScatterStyle& style)
{
	mScatterStyle=style;
}

void CGraph::setScatterSkip(int skip)
{
	mScatterSkip=mMax(0, skip);
}


void CGraph::setChannelFillGraph(CGraph* targetGraph)
{
	if(targetGraph==this)
	{
		mChannelFillGraph=0;
		return ;
	}
	mChannelFillGraph = targetGraph;
}

void CGraph::setAdaptiveSampling(bool enabled)
{
	mAdaptiveSampling=enabled;
}

void CGraph::addData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted)
{
	if(keys.size()!=values.size())
	{
		std::cout<<"Error: keys and values have different sizes:"<<keys.size()<<" "<<values.size()<<std::endl;
		
	}
	const int n=mMin(keys.size(), values.size());
	CVector<CGraphData> tempData(n);
	CVector<CGraphData>::iterator it=tempData.begin();
	const CVector<CGraphData>::iterator itEnd=tempData.end();
	int i=0;
	while(it!=itEnd)
	{
		it->key = keys[i];
		it->value = values[i];
		++it;
		++i;
	}
	mDataContainer->add(tempData, alreadySorted);
}

void CGraph::addData(double key, double value)
{
	mDataContainer->add(CGraphData(key, value));
}

CRange CGraph::getKeyRange(bool& foundRange, WORD inSignDomain)const
{
	return mDataContainer->keyRange(foundRange, inSignDomain);
}

CRange CGraph::getValueRange(bool& foundRange, WORD inSignDomain, const CRange& inKeyRange)const
{
	return mDataContainer->valueRange(foundRange, inSignDomain, inKeyRange);
}


void CGraph::draw(CPlotPainter* painter)
{
	if(!mKeyAxis||!mValueAxis)
	{
		std::cout<<"Error: Invalid key or value axis"<<std::endl;
	}
	
	if(mKeyAxis->range().size()<=0||mDataContainer->isEmpty()) return;
	
	if(mLineStyle==lsNone&&mScatterStyle.isNone())	return ;
	CVector<CPointF> lines, scatters;
	CList<CDataRange> selectedSegments, unselectedSegments, allSegments;
	getDataSegments(selectedSegments, unselectedSegments);
	allSegments<<unselectedSegments<<selectedSegments;
	for(int i=0; i<allSegments.size(); ++i)
	{
		bool isSelectedSegment=i>=unselectedSegments.size();
		CDataRange lineDataRange=isSelectedSegment?allSegments.at(i):allSegments.at(i).adjusted(-1,1);
		getLines(&lines, lineDataRange);
		painter->setBrush(mBrush);
		painter->setPen(Gemini::NoPen);
		drawFill(painter,&lines);
		
		
		if(mLineStyle!=lsNone)
		{
			if(isSelectedSegment&&mSelectionDecorator)
				mSelectionDecorator->applyPen(painter);
			else		
				painter->setPen(mPen);
			painter->setBrush(Gemini::NoBrush);
			if(mLineStyle==lsImpluse)
				drawImplusePlot(painter, lines);
			else
				drawLinePlot(painter, lines);
		}
		
		
		////draw scatters:
		CScatterStyle finalScatterStyle=mScatterStyle;
		if(isSelectedSegment&&mSelectionDecorator)
			finalScatterStyle=mSelectionDecorator->getFinalScatterStyle(mScatterStyle);
		if(!finalScatterStyle.isNone())
		{
			getScatters(&scatters, allSegments.at(i));
			drawScatterPlot(painter,scatters, finalScatterStyle);
		}
		//test
		//drawLegendIcon(painter, CRectF(0,0,30,30).translated(mKeyAxis->axisRect()->left(),mValueAxis->axisRect()->top()));
	}
	
}


void CGraph::drawLegendIcon(CPlotPainter* painter, const CRectF& rect)const
{
	if(mVisible)
	{
		if(mBrush.style()!=Gemini::NoBrush)
		{
			applyFillAntialiasingHint(painter);
			painter->fillRect(CRectF(rect.left(),rect.top()+rect.height()/2.0,rect.width(),rect.height()/3.0),mBrush);
		}
		
		if(mLineStyle!=lsNone)
		{
			applyDefaultAntialiasingHint(painter);
			painter->setPen(mPen);
			painter->drawLine(CLineF(rect.left(),rect.top()+rect.height()/2.0,rect.right()+5,rect.top()+rect.height()/2.0));
		}
		
		if(!mScatterStyle.isNone())
		{
			applyScattersAntialiasingHint(painter);
			if(mScatterStyle.shape()==CScatterStyle::ssPixmap&&(mScatterStyle.pixmap().size().width()>rect.width()
			||mScatterStyle.pixmap().size().height()>rect.height()))
			{
				CScatterStyle scaledStyle(mScatterStyle);
				scaledStyle.setPixmap(scaledStyle.pixmap().scaled(rect.size().toSize(),Gemini::KeepAspectRatio, Gemini::SmoothTransformation));
				scaledStyle.applyTo(painter, mPen);
				scaledStyle.drawShape(painter, CRectF(rect).center());
			}
			else
			{
				mScatterStyle.applyTo(painter, mPen);
				mScatterStyle.drawShape(painter, CRectF(rect).center());
			}
		}
	}
}


void CGraph::getLines(CVector<CPointF>* lines, const CDataRange& dataRange)const
{
	if(!lines) return;
	CGraphDataContainer::const_iterator begin, end;
	getVisibleDataBounds(begin, end, dataRange);
	if(begin==end)
	{
		lines->clear();
		return;
	}
	
	CVector<CGraphData> lineData;
	if(mLineStyle!=lsNone)
	{
		getOptimizedLineData(&lineData, begin, end);
	}
	
	switch(mLineStyle)
	{
		case lsNone: lines->clear(); break;
		case lsLine: *lines=dataToLines(lineData); break;
		case lsStepLeft: *lines=dataToStepLeftLines(lineData); break;
		case lsStepRight: *lines=dataToStepRightLines(lineData); break;
		case lsStepCenter: *lines=dataToStepCenterLines(lineData);break;
		case lsImpluse: *lines=dataToImpluseLines(lineData);break;
	}
}


void CGraph::getScatters(CVector<CPointF>* scatters, const CDataRange& dataRange)const
{
	if(!scatters) return;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)
	{
		std::cout<<"Error: invalid key or value axis"<<std::endl;
		scatters->clear();
		return ;
	}
	
	CGraphDataContainer::const_iterator begin, end;
	getVisibleDataBounds(begin, end, dataRange);
	if(begin==end)
	{
		scatters->clear();
		return;
	}
	
	CVector<CGraphData> data;
	getOptimizedScatterData(&data, begin, end);
	scatters->resize(data.size());
	if(keyAxis->orientation()==Gemini::Vertical)
	{
		for(int i=0; i<data.size();++i)
		{
			if(!isnan(data.at(i).value))
			{
				(*scatters)[i].setX(valueAxis->coordToPixel(data.at(i).value));
				(*scatters)[i].setY(keyAxis->coordToPixel(data.at(i).key));
			}
		}
	}
	else
	{
		for(int i=0; i<data.size(); ++i)
		{
			if(!isnan(data.at(i).value))
			{
				(*scatters)[i].setX(keyAxis->coordToPixel(data.at(i).key));
				(*scatters)[i].setY(valueAxis->coordToPixel(data.at(i).value));
			}
		}
		
	}
}



CVector<CPointF> CGraph::dataToLines(const CVector<CGraphData>& data)const
{
	CVector<CPointF> result;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis) 
		return result;
	result.reserve(data.size()+2);
	result.resize(data.size());
	if(keyAxis->orientation()==Gemini::Vertical)
	{
		for(int i=0; i<data.size();++i)
		{
			result[i].setX(valueAxis->coordToPixel(data.at(i).value));
			result[i].setY(keyAxis->coordToPixel(data.at(i).key));
		}
	}
	else
	{
		for(int i=0; i<data.size(); ++i)
		{
			result[i].setX(keyAxis->coordToPixel(data.at(i).key));
			result[i].setY(valueAxis->coordToPixel(data.at(i).value));
		}
	}
	return result;
}


CVector<CPointF> CGraph::dataToStepLeftLines(const CVector<CGraphData>& data)const
{
	CVector<CPointF> result;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)	return result;
	result.reserve(data.size()*2+2);
	result.resize(data.size()*2);
	if(keyAxis->orientation()==Gemini::Vertical)
	{
		double lastValue=valueAxis->coordToPixel(data.first().value);
		for(int i=0; i<data.size();++i)
		{
			const double key=keyAxis->coordToPixel(data.at(i).key);
			result[i*2+0].setX(lastValue);
			result[i*2+0].setY(key);
			lastValue=valueAxis->coordToPixel(data.at(i).value);
			result[i*2+1].setX(lastValue);
			result[i*2+1].setY(key);
		}
	}
	else
	{
		double lastValue =valueAxis->coordToPixel(data.first().value);
		for(int i=0; i<data.size(); ++i)
		{
			const double key=keyAxis->coordToPixel(data.at(i).key);
			result[i*2+0].setX(key);
			result[i*2+0].setY(lastValue);
			lastValue=valueAxis->coordToPixel(data.at(i).value);
			result[i*2+1].setX(key);
			result[i*2+1].setY(lastValue);
		}
	}
	return result;
}


CVector<CPointF> CGraph::dataToStepRightLines(const CVector<CGraphData>& data)const
{
	CVector<CPointF> result;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis= mValueAxis;
	
	if(!keyAxis||!valueAxis)	return result;
	result.reserve(data.size()*2+2);
	result.resize(data.size()*2);
	if(keyAxis->orientation()==Gemini::Vertical)
	{
		double lastKey = keyAxis->coordToPixel(data.first().key);
		for(int i=0; i<data.size();++i)
		{
			const double value = valueAxis->coordToPixel(data.at(i).value);
			result[i*2+0].setX(value);
			result[i*2+0].setY(lastKey);
			lastKey=keyAxis->coordToPixel(data.at(i).key);
			result[i*2+1].setX(value);
			result[i*2+1].setY(lastKey);
		}
	}
	else
	{
		double lastKey=keyAxis->coordToPixel(data.first().key);
		for(int i=0; i<data.size();++i)
		{
			const double value=valueAxis->coordToPixel(data.at(i).value);
			result[i*2+0].setX(lastKey);
			result[i*2+0].setY(value);
			lastKey=keyAxis->coordToPixel(data.at(i).key);
			result[i*2+1].setX(lastKey);
			result[i*2+1].setY(value);
		}
	}
	return result;
}

CVector<CPointF> CGraph::dataToStepCenterLines(const CVector<CGraphData>& data)const
{
	CVector<CPointF> result;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)	return result;
	result.reserve(data.size()*2+2);
	result.resize(data.size()*2);
	
	if(keyAxis->orientation()==Gemini::Vertical)
	{
		double lastKey=keyAxis->coordToPixel(data.first().key);
		double lastValue=valueAxis->coordToPixel(data.first().value);
		result[0].setX(lastValue);
		result[0].setY(lastKey);
		for(int i=1; i<data.size(); ++i)
		{
			const double key=(keyAxis->coordToPixel(data.at(i).key)+lastKey)*0.5;
			result[i*2-1].setX(lastValue);
			result[i*2-1].setY(key);
			lastValue=valueAxis->coordToPixel(data.at(i).value);
			lastKey=keyAxis->coordToPixel(data.at(i).key);
			result[i*2+0].setX(lastValue);
			result[i*2+0].setY(key);
		}
		result[data.size()*2-1].setX(lastValue);
		result[data.size()*2-1].setY(lastKey);
	}
	else
	{
		double lastKey=keyAxis->coordToPixel(data.first().key);
		double lastValue=valueAxis->coordToPixel(data.first().value);
		result[0].setX(lastKey);
		result[0].setY(lastValue);
		for(int i=1; i<data.size();++i)
		{
			const double key=(keyAxis->coordToPixel(data.at(i).key)+lastKey)*0.5;
			result[i*2-1].setX(key);
			result[i*2-1].setY(lastValue);
			lastValue=valueAxis->coordToPixel(data.at(i).value);
			lastKey=keyAxis->coordToPixel(data.at(i).key);
			result[i*2+0].setX(key);
			result[i*2+0].setY(lastValue);
		}
		result[data.size()*2-1].setX(lastKey);
		result[data.size()*2-1].setY(lastValue);
	}
	return result;
}




CVector<CPointF> CGraph::dataToImpluseLines(const CVector<CGraphData>& data) const
{
	CVector<CPointF> result;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)	return result;
	
	result.resize(data.size()*2);
	if(keyAxis->orientation()==Gemini::Vertical)
	{
		for(int i=0; i<data.size(); ++i)
		{
			const double key =keyAxis->coordToPixel(data.at(i).key);
			result[i*2+0].setX(valueAxis->coordToPixel(0));
			result[i*2+0].setY(key);
			result[i*2+1].setX(valueAxis->coordToPixel(data.at(i).value));
			result[i*2+1].setY(key);
		}
	}
	else
	{
		for(int i=0; i<data.size(); ++i)
		{
			const double key= keyAxis->coordToPixel(data.at(i).key);
			result[i*2+0].setX(key);
			result[i*2+0].setY(valueAxis->coordToPixel(0));
			result[i*2+1].setX(key);
			result[i*2+1].setY(valueAxis->coordToPixel(data.at(i).value));
		}
	}
	return result;
}



void CGraph::drawFill(CPlotPainter* painter, CVector<CPointF>* lines)const
{
	if(mLineStyle==lsImpluse) return;
	if(painter->brush().style()==Gemini::NoBrush||(painter->brush().color()>>24&0xFF)==0)
		return ;
	applyFillAntialiasingHint(painter);
	if(!mChannelFillGraph)
	{
		addFillBasePoints(lines);
		painter->drawPolygon(CPolygonF(lines->toStdVector()));
		removeFillBasePoints(lines);
	}
	else
	{
		painter->drawPolygon(getChannelFillPolygon(lines));
	}
}

void CGraph::drawScatterPlot(CPlotPainter* painter, const CVector<CPointF>& scatters, const CScatterStyle& style)const
{
	applyScattersAntialiasingHint(painter);
	style.applyTo(painter, mPen);
	for(int i=0; i<scatters.size();++i)
		style.drawShape(painter, scatters.at(i).x(),scatters.at(i).y());
}

void CGraph::drawLinePlot(CPlotPainter* painter, const CVector<CPointF>& lines)const
{
	if(painter->pen().style()!=Gemini::NoPen&&(painter->pen().color()>>24&0xFF)!=0)
	{
		applyDefaultAntialiasingHint(painter);
		drawPolyline(painter, lines);
	}
}


void CGraph::drawImplusePlot(CPlotPainter* painter, const CVector<CPointF>& lines)const
{
	if(painter->pen().style()!=Gemini::NoPen&&(painter->pen().color()>>24&0xFF)!=0)
	{
		applyDefaultAntialiasingHint(painter);
		CPen oldPen=painter->pen();
		CPen newPen=painter->pen();
		newPen.setCapStyle(Gemini::FlatCap);
		painter->setPen(newPen);
		painter->drawLines(lines.toStdVector());
		painter->setPen(oldPen);
	}
}

void CGraph::getOptimizedLineData(CVector<CGraphData>* lineData, const CGraphDataContainer::const_iterator& begin, const CGraphDataContainer::const_iterator& end)const
{
	if(!lineData) return ;
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)	return ;
	if(begin==end)	return ;
	int dataCount=end-begin;
	int maxCount= std::numeric_limits<int>::max();
	if(mAdaptiveSampling)
	{
		double keyPixelSpan=mAbs(keyAxis->coordToPixel(begin->key)-keyAxis->coordToPixel((end-1)->key));
		if(2*keyPixelSpan+2<(double)std::numeric_limits<int>::max())
			maxCount=2*keyPixelSpan+2;
	}
	
	if(mAdaptiveSampling&&dataCount>=maxCount)
	{
		CGraphDataContainer::const_iterator it=begin;
		double minValue=it->value;
		double maxValue=it->value;
		CGraphDataContainer::const_iterator currentIntervalFirstPoint = it;
		int reversedFactor=keyAxis->pixelOrientation();
		int reversedRound=reversedFactor==-1?1:0;
		double currentIntervalStartKey=keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(begin->key)+reversedRound));
		double lastIntervalEndKey=currentIntervalStartKey;
		double keyEpsilon=mAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor));
		bool keyEpsilonVariable=keyAxis->scaleType()==CPlotAxis::Logarithmic;
		int intervalDataCount=1;
		++it;
		while(it!=end)
		{
			if(it->key<currentIntervalStartKey+keyEpsilon)
			{
				if(it->value<minValue)
				{
					minValue=it->value;
				}
				else if(it->value>maxValue)
				{
					maxValue=it->value;
				}
				++intervalDataCount;
			}
			else
			{
				if(intervalDataCount>=2)
				{
					if(lastIntervalEndKey<currentIntervalStartKey-keyEpsilon)
						lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.2,currentIntervalFirstPoint->value));
					lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.25, minValue));
					lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.75, maxValue));
					
					if(it->key>currentIntervalStartKey+keyEpsilon*2)
						lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.8,(it-1)->value));
				}
				else
				{
					lineData->append(CGraphData(currentIntervalFirstPoint->key, currentIntervalFirstPoint->value));
				}
				lastIntervalEndKey = (it-1)->key;
				minValue = it->value;
				maxValue = it->value;
				currentIntervalFirstPoint = it;
				currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(it->key)+reversedRound));
				if(keyEpsilonVariable)
					keyEpsilon=mAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor));
				intervalDataCount =1;
			}
			++it;
		}
		
		if(intervalDataCount>=2)
		{
			if(lastIntervalEndKey<currentIntervalStartKey-keyEpsilon)
				lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.2, currentIntervalFirstPoint->value));
			lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.25, minValue));
			lineData->append(CGraphData(currentIntervalStartKey+keyEpsilon*0.75, maxValue));
		}
		else
			lineData->append(CGraphData(currentIntervalFirstPoint->key, currentIntervalFirstPoint->value));
	}
	else
	{
		CGraphDataContainer::const_iterator it=begin;
		lineData->reserve(dataCount+2);
		while(it!=end)
		{
			lineData->append(*it);
			++it;
		}
	}
}


void CGraph::getOptimizedScatterData(CVector<CGraphData>* scatterData, CGraphDataContainer::const_iterator begin, CGraphDataContainer::const_iterator end)const
{
	if(!scatterData) return ;
	CPlotAxis* keyAxis= mKeyAxis;
	CPlotAxis* valueAxis = mValueAxis;
	if(!keyAxis||!valueAxis) return ;
	const int scatterModulo = mScatterSkip+1;
	const bool doScatterSkip =mScatterSkip>0;
	
	int beginIndex=begin-mDataContainer->constBegin();
	int endIndex=end-mDataContainer->constBegin();
	while(doScatterSkip&& begin!=end&& beginIndex%scatterModulo!=0)
	{
		++beginIndex;
		++begin;
	}
	if(begin==end) return ;
	int dataCount = end-begin;
	int maxCount=std::numeric_limits<int>::max();
	if(mAdaptiveSampling)
	{
		int keyPixelSpan=mAbs(keyAxis->coordToPixel(begin->key)-keyAxis->coordToPixel((end-1)->key));
		maxCount = 2*keyPixelSpan+2;
	}
	
	if(mAdaptiveSampling&& dataCount>=maxCount)
	{
		double valueMaxRange = valueAxis->range().upper;
		double valueMinRange = valueAxis->range().lower;
		CGraphDataContainer::const_iterator it=begin;
		int itIndex=beginIndex;
		double minValue=it->value;
		double maxValue=it->value;
		CGraphDataContainer::const_iterator minValueIt=it;
		CGraphDataContainer::const_iterator maxValueIt=it;
		CGraphDataContainer::const_iterator currentIntervalStart=it;
		int reversedFactor=keyAxis->pixelOrientation();
		int reversedRound=reversedFactor==-1?1:0;
		double currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(begin->key)+reversedRound));
		double keyEpsilon = mAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor));
		bool keyEpsilonVariable=keyAxis->scaleType()==CPlotAxis::Logarithmic;
		int intervalDataCount=1;
		if(!doScatterSkip)
			++it;
		else
		{
			itIndex +=scatterModulo;
			if(itIndex<endIndex)
				it+=scatterModulo;
			else
			{
				it=end;
				itIndex=endIndex;
				
			}
		}
		
		while(it!=end)
		{
			if(it->key<currentIntervalStartKey+keyEpsilon)
			{
				if(it->value<minValue&&it->value>valueMinRange&&it->value<valueMaxRange)
				{
					minValue=it->value;
					minValueIt=it;
				}
				else if(it->value>maxValue&&it->value>valueMinRange&&it->value<valueMaxRange)
				{
					maxValue = it->value;
					maxValueIt=it;
				}
				++intervalDataCount;
			}
			else
			{
				if(intervalDataCount>=2)
				{
					double valuePixelSpan=mAbs(valueAxis->coordToPixel(minValue)-valueAxis->coordToPixel(maxValue));
					int dataModulo = mMax(1, mRound(intervalDataCount/(valuePixelSpan/4.0)));
					CGraphDataContainer::const_iterator intervalIt=currentIntervalStart;
					int c=0;
					while(intervalIt!=it)
					{
						if(c%dataModulo==0||intervalIt==minValueIt||intervalIt==maxValueIt)
							scatterData->append(*intervalIt);
						++c;
						if(!doScatterSkip)
							++intervalIt;
						else
							intervalIt+=scatterModulo;
					}
				}
				else if(currentIntervalStart->value>valueMinRange&&currentIntervalStart->value<valueMaxRange)
					scatterData->append(*currentIntervalStart);
					minValue=it->value;
					maxValue=it->value;
					currentIntervalStart = it;
					currentIntervalStartKey = keyAxis->pixelToCoord((int)(keyAxis->coordToPixel(it->key)+reversedRound));
					if(keyEpsilonVariable)
						keyEpsilon =mAbs(currentIntervalStartKey-keyAxis->pixelToCoord(keyAxis->coordToPixel(currentIntervalStartKey)+1.0*reversedFactor));
					intervalDataCount=1;
			}
			if(!doScatterSkip)
				++it;
			else{
				itIndex+=scatterModulo;
				if(itIndex<endIndex)
				{
					it+=scatterModulo;
				}
				else
				{
					it =end;
					itIndex = endIndex;
				}
			}
			
		}
		if(intervalDataCount>=2)
		{
			double valuePixelSpan = mAbs(valueAxis->coordToPixel(minValue)-valueAxis->coordToPixel(maxValue));
			int dataModulo = mMax(1, mRound(intervalDataCount/(valuePixelSpan/4.0)));
			CGraphDataContainer::const_iterator intervalIt=currentIntervalStart;
			int intervalItIndex =intervalIt-mDataContainer->constBegin();
			int c=0;
			while(intervalIt!=it)
			{
				if((c%dataModulo==0||intervalIt==minValueIt||intervalIt==maxValueIt))
					scatterData->append(*intervalIt);
				++c;
				if(!doScatterSkip)
					++intervalIt;
				else
				{
					intervalItIndex+=scatterModulo;
					if(intervalItIndex<itIndex)
						intervalIt+=scatterModulo;
					else
					{
						intervalIt=it;
						intervalItIndex=itIndex;
					}
				}
			}
		}
		else if(currentIntervalStart->value>valueMinRange&&currentIntervalStart->value<valueMaxRange)
			scatterData->append(*currentIntervalStart);
	}
	else
	{
		CGraphDataContainer::const_iterator it=begin;
		int itIndex = beginIndex;
		scatterData->reserve(dataCount);
		while(it!=end)
		{
			scatterData->append(*it);
			if(!doScatterSkip)
				++it;
			else
			{
				itIndex+=scatterModulo;
				if(itIndex<endIndex)
					it+=scatterModulo;
				else
				{
					it=end;
					itIndex = endIndex;
				}
			}
		}
	}
}


void CGraph::getVisibleDataBounds(CGraphDataContainer::const_iterator& begin, CGraphDataContainer::const_iterator& end, const CDataRange& rangeRestriction)const
{
	if(rangeRestriction.isEmpty())
	{
		end= mDataContainer->constEnd();
		begin=end;
	}
	else
	{
		CPlotAxis* keyAxis=mKeyAxis;
		CPlotAxis* valueAxis=mValueAxis;
		if(!keyAxis||!valueAxis)
			return ;
		begin = mDataContainer->findBegin(keyAxis->range().lower);
		end = mDataContainer->findEnd(keyAxis->range().upper);
		mDataContainer->limitIteratorToDataRange(begin, end, rangeRestriction);
	}
}

void CGraph::addFillBasePoints(CVector<CPointF>*lines)const
{
	if(!mKeyAxis) return ;
	if(!lines) return ;
	if(lines->isEmpty()) return ;
	if(mKeyAxis->orientation()==Gemini::Vertical)
	{
		*lines<<upperFillBasePoint(lines->last().y());
		*lines<<lowerFillBasePoint(lines->first().y());
	}
	else
	{
		*lines<<upperFillBasePoint(lines->last().x());
		*lines<<lowerFillBasePoint(lines->first().x());
	}
}

void CGraph::removeFillBasePoints(CVector<CPointF>* lines)const
{
	if(!lines) return ;
	if(lines->isEmpty()) return ;
	lines->remove(lines->size()-2,2);
}

CPointF CGraph::lowerFillBasePoint(double lowerKey)const
{
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)
	{
		return CPointF();
	}
	
	CPointF point;
	if(valueAxis->scaleType()==CPlotAxis::Linear)
	{
		if(keyAxis->axisType()==CPlotAxis::atLeft)
		{
			point.setX(valueAxis->coordToPixel(0));
			point.setY(lowerKey);
		}
		else if(keyAxis->axisType()==CPlotAxis::atRight)
		{
			point.setX(valueAxis->coordToPixel(0));
			point.setY(lowerKey);
		}
		else if(keyAxis->axisType()==CPlotAxis::atTop)
		{
			point.setX(lowerKey);
			point.setY(valueAxis->coordToPixel(0));
		}
		else if(keyAxis->axisType()==CPlotAxis::atBottom)
		{
			point.setX(lowerKey);
			point.setY(valueAxis->coordToPixel(0));
		}
	}
	else
	{
		if(keyAxis->orientation()==Gemini::Vertical)
		{
			if((valueAxis->range().upper<0&&!valueAxis->rangeReversed())||(valueAxis->range().upper>0&&valueAxis->rangeReversed()))
				point.setX(keyAxis->axisRect()->right());
			else
				point.setX(keyAxis->axisRect()->left());
			point.setY(lowerKey);
		}else if(keyAxis->axisType()==CPlotAxis::atTop||keyAxis->axisType()==CPlotAxis::atBottom)
		{
			point.setX(lowerKey);
			if((valueAxis->range().upper<0&&!valueAxis->rangeReversed())||(valueAxis->range().upper>0&&valueAxis->rangeReversed()))
			{
				point.setY(keyAxis->axisRect()->top());
			}
			else
				point.setY(keyAxis->axisRect()->bottom());
		}
	}
	return point;
}

CPointF CGraph::upperFillBasePoint(double upperKey)const
{
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	if(!keyAxis||!valueAxis)	return CPointF();
	
	CPointF point;
	
	if(valueAxis->scaleType()==CPlotAxis::Linear)
	{
		if(keyAxis->axisType()==CPlotAxis::atLeft)
		{
			point.setX(valueAxis->coordToPixel(0));
			point.setY(upperKey);
		}
		else if(keyAxis->axisType()==CPlotAxis::atRight)
		{
			point.setX(valueAxis->coordToPixel(0));
			point.setY(upperKey);
		}
		else if(keyAxis->axisType()==CPlotAxis::atTop)
		{
			point.setX(upperKey);
			point.setY(valueAxis->coordToPixel(0));
		}
		else if(keyAxis->axisType()==CPlotAxis::atBottom)
		{
			point.setX(upperKey);
			point.setY(valueAxis->coordToPixel(0));
		}
	}
	else
	{
		if(keyAxis->orientation()==Gemini::Vertical)
		{
			if((valueAxis->range().upper<0&&!valueAxis->rangeReversed())||
			(valueAxis->range().upper>0&&valueAxis->rangeReversed()))
				point.setX(keyAxis->axisRect()->right());
			else
				point.setX(keyAxis->axisRect()->left());
			point.setY(upperKey);
		}
		else if(keyAxis->axisType()==CPlotAxis::atTop||keyAxis->axisType()==CPlotAxis::atBottom)
		{
			point.setX(upperKey);
			if((valueAxis->range().upper<0&&!valueAxis->rangeReversed())||
			(valueAxis->range().upper>0&&valueAxis->rangeReversed()))
				point.setY(keyAxis->axisRect()->top());
			else
				point.setY(keyAxis->axisRect()->bottom());
		}
	}
	return point;
}


const CPolygonF CGraph::getChannelFillPolygon(const CVector<CPointF>* lines)const
{
	if(!mChannelFillGraph)
		return CPolygonF();
	
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	
	if(!keyAxis||!valueAxis)	return CPolygonF(); 
	
	if(!mChannelFillGraph->mKeyAxis)
	{
		std::cout<<"Error: channel fill target key axis valid"<<std::endl;
		return CPolygonF();
	}
	
	
	if(mChannelFillGraph->mKeyAxis->orientation()!=keyAxis->orientation())
	{
		return CPolygonF();
	}
	
	if(!lines||lines->isEmpty()) return CPolygonF();
	CVector<CPointF> otherData;
	mChannelFillGraph->getLines(&otherData, CDataRange(0,mChannelFillGraph->dataCount()));
	
	if(otherData.isEmpty()) return CPolygonF();
	CVector<CPointF> thisData;
	thisData.reserve(lines->size()+otherData.size());
	for(int i=0; i<lines->size();++i)
	{
		thisData<<lines->at(i);
	}
	CVector<CPointF>* staticData =&thisData;
	CVector<CPointF>* croppedData=&otherData;
	
	if(keyAxis->orientation()==Gemini::Horizontal)
	{
		if(staticData->first().x()>staticData->last().x())
		{
			int size =staticData->size();
			for(int i=0; i<size/2;++i)
				mSwap((*staticData)[i],(*staticData)[size-1-i]);
		}
		if(croppedData->first().x()>croppedData->last().x())
		{
			int size = croppedData->size();
			for(int i=0; i<size/2; ++i)
				mSwap((*croppedData)[i],(*croppedData)[size-1-i]);
		}
		
		if(staticData->first().x()<croppedData->first().x())
			mSwap(staticData, croppedData);
		int lowBound = findIndexBelowX(croppedData, staticData->first().x());
		if(lowBound==-1) return CPolygonF();
		croppedData->remove(0,lowBound);
		
		if(croppedData->size()<2) return CPolygonF();
		double slope;
		if(croppedData->at(1).x()-croppedData->at(0).x()!=0)
			slope=(croppedData->at(1).y()-croppedData->at(0).y())/(croppedData->at(1).x()-croppedData->at(0).x());
		else
			slope=0;
		(*croppedData)[0].setY(croppedData->at(0).y()+slope*(staticData->first().x()-croppedData->at(0).x()));
		(*croppedData)[0].setX(staticData->first().x());
		
		if(staticData->last().x()>croppedData->last().x())
			mSwap(staticData, croppedData);
		int highBound= findIndexAboveX(croppedData, staticData->last().x());
		if(highBound==-1) return CPolygonF();
		croppedData->remove(highBound+1, croppedData->size()-(highBound+1));
		if(croppedData->size()<2) return CPolygonF();
		int li=croppedData->size()-1;
		if((croppedData->at(li).x()>croppedData->at(li-1).x())!=0)
			slope=(croppedData->at(li).y()-croppedData->at(li-1).y())/(croppedData->at(li).x()-croppedData->at(li-1).x());
		else
			slope = 0;
		(*croppedData)[li].setY(croppedData->at(li-1).y()+slope*(staticData->last().x()-croppedData->at(li-1).x()));
		(*croppedData)[li].setX(staticData->last().x());
	}
	else
	{
		if(staticData->first().y()<staticData->last().y())
		{
			int size =staticData->size();
			for(int i=0; i<size/2; ++i)
				mSwap((*staticData)[i],(*staticData)[size-1-i]);
		}
		if(croppedData->first().y()<croppedData->last().y())
		{
			int size = croppedData->size();
			for(int i=0; i<size/2; ++i)
				mSwap((*croppedData)[i], (*croppedData)[size-1-i]);
		}
		
		if(staticData->first().y()>croppedData->first().y())
			mSwap(staticData, croppedData);
		int lowBound=findIndexAboveY(croppedData, staticData->first().y());
		if(lowBound==-1) return CPolygonF();
		croppedData->remove(0,lowBound);
		if(croppedData->size()<2) return CPolygonF();
		double slope;
		if(croppedData->at(1).y()-croppedData->at(0).y()!=0)
			slope=(croppedData->at(1).x()-croppedData->at(0).x())/(croppedData->at(1).y()-croppedData->at(0).y());
		else
			slope=0;
		(*croppedData)[0].setX(croppedData->at(0).x()+slope*(staticData->first().y()-croppedData->at(0).y()));
		(*croppedData)[0].setY(staticData->first().y());
		
		if(staticData->last().y()<croppedData->last().y())
		{
			mSwap(staticData, croppedData);
		}
		
		int highBound=findIndexBelowY(croppedData, staticData->last().y());
		if(highBound==-1) return CPolygonF();
		int li=croppedData->size()-1;
		if(croppedData->at(li).y()-croppedData->at(li-1).y()!=0)
		{
			slope=(croppedData->at(li).x()-croppedData->at(li-1).x())/(croppedData->at(li).y()-croppedData->at(li-1).y());
		}
		else
			slope = 0;
		
		(*croppedData)[li].setX(croppedData->at(li-1).x()+slope*(staticData->last().y()-croppedData->at(li-1).y()));
		(*croppedData)[li].setY(staticData->last().y());
		
		for(int i=otherData.size()-1; i>=0; --i)
		{
			thisData<<otherData.at(i);
		}
		return CPolygonF(thisData.toStdVector());
	}
	
}

double CGraph::pointDistance(const CPointF& pixelPoint, CGraphDataContainer::const_iterator& closestData)const
{
	closestData = mDataContainer->constEnd();
	if(mDataContainer->isEmpty())
		return -1.0;
	if(mLineStyle==lsNone)
		return -1.0;
	
//	double minDistSqrt = std::numeric_limits<double>::max();
//	double posKeyMin, posKeyMax, dummy;
	return 0;
}

int CGraph::findIndexAboveX(const CVector<CPointF>* data, double x)const
{
	for(int i=data->size()-1;i>=0;--i)
	{
		if(data->at(i).x()<x)
		{
			if(i<data->size()-1)
				return i+1;
			else
				return data->size()-1;
		}
	}
	return -1;
}

int CGraph::findIndexBelowX(const CVector<CPointF>* data, double x)const
{
	for(int i=0;i<data->size();++i)
	{
		if(data->at(i).x()>x)
		{
			if(i>0)
				return i-1;
			else 
				return 0;
		}
	}
	return -1;
}

int CGraph::findIndexAboveY(const CVector<CPointF>* data, double y)const
{
	for(int i=0; i<data->size();++i)
	{
		if(data->at(i).y()<y)
		{
			if(i>0)
				return i-1;
			else
				return 0;
		}
	}
	return -1;
}

int CGraph::findIndexBelowY(const CVector<CPointF>* data, double y)const
{
	for(int i=data->size()-1; i>0;--i)
	{
		if(data->at(i).y()>y)
		{
			if(i<data->size()-1)
				return i+1;
			else
				return data->size()-1;
		}
	}
	return -1;
}




CScatterStyle::CScatterStyle():
mSize(6),
mShape(ssNone),
mPen(Gemini::NoPen),
mBrush(Gemini::NoBrush),
mPenDefined(false)
{
	
}

CScatterStyle::CScatterStyle(WORD shape, double size):
mSize(size),
mShape(shape),
mPen(Gemini::NoPen),
mBrush(Gemini::NoBrush),
mPenDefined(false)
{
	
}

CScatterStyle::CScatterStyle(WORD shape, int color, double size):
mSize(size),
mShape(shape),
mPen(CPen(color)),
mBrush(Gemini::NoBrush),
mPenDefined(true)
{
	
}

CScatterStyle::CScatterStyle(WORD shape, int color, int fillColor, double size):
mSize(size),
mShape(shape),
mPen(CPen(color)),
mBrush(CBrush(fillColor)),
mPenDefined(true)
{
	
}

CScatterStyle::CScatterStyle(WORD shape, const CPen& pen, const CBrush& brush, double size):
mSize(size),
mShape(shape),
mPen(pen),
mBrush(brush),
mPenDefined(pen.style()!=Gemini::NoPen)
{
	
}

CScatterStyle::CScatterStyle(const CImage& img):
mSize(5),
mShape(ssPixmap),
mPen(Gemini::NoPen),
mBrush(Gemini::NoBrush),
mPixmap(img),
mPenDefined(false)
{
	
}

CScatterStyle::CScatterStyle(const CPainterPath& customPath, const CPen& pen, const CBrush& brush, double size)
:mSize(size),
mShape(ssCustom),
mPen(pen),
mBrush(brush),
mCustomPath(customPath),
mPenDefined(pen.style()!=Gemini::NoPen)
{
	
}


/* CScatterStyle::CScatterStyle(const CScatterStyle& scatterStyle)
{
	mSize=scatterStyle.mSize;
	mShape=scatterStyle.mShape;
	mPen=scatterStyle.mPen;
	mBrush=scatterStyle.mBrush;
	mCustomPath=scatterStyle.mCustomPath;
	mPenDefined=scatterStyle.mPenDefined;
}
CScatterStyle CScatterStyle::operator=(const CScatterStyle& scatterStyle)
{
	return CScatterStyle(scatterStyle);
} */

bool CScatterStyle::operator!=(const CScatterStyle& scatterStyle)const
{
	bool result=false;
	result=(mShape!=scatterStyle.mShape);
	if(result) return result;
	result=(mSize!=scatterStyle.mSize||
		mPen!=scatterStyle.mPen||
		mBrush!=scatterStyle.mBrush||
		mPenDefined!=scatterStyle.mPenDefined);
	if(result) return result;
	
	if(mShape==ssPixmap)
	{
		result=(mPixmap!=scatterStyle.mPixmap);
	}
	else if(mShape==ssCustom)
	{
		result=(mCustomPath!=scatterStyle.mCustomPath);
	}
	else
	{
		;
	}
	return result;
}

void CScatterStyle::setFromOther(const CScatterStyle& other, WORD properties)
{
	if(spPen==(properties&spPen))
	{
		setPen(other.pen());
		if(!other.isPenDefined())
			undefinedPen();
	}
	if(spBrush==(properties&spBrush))
	{
		setBrush(other.brush());
	}
	if(spSize==(properties&spSize))
	{
		setSize(other.size());
	}
	if(spShape==(properties&spShape))
	{
		setShape(other.shape());
		if(other.shape()==ssPixmap)
			setPixmap(other.pixmap());
		else if(other.shape()==ssCustom)
			setCustomPath(other.customPath());
	}
}


void CScatterStyle::setSize(double size)
{
	mSize = size;
}

void CScatterStyle::setShape(WORD shape)
{
	mShape = shape;
}

void CScatterStyle::setPen(const CPen& pen)
{
	mPenDefined = true;
	mPen = pen;
}

void CScatterStyle::setBrush(const CBrush& brush)
{
	mBrush= brush;
}

void CScatterStyle::setPixmap(const CImage& pixmap)
{
	setShape(ssPixmap);
	mPixmap=pixmap;
}

void CScatterStyle::setCustomPath(const CPainterPath& path)
{
	setShape(ssCustom);
	mCustomPath= path;
}

void CScatterStyle::undefinedPen()
{
	mPenDefined = false;
}

void CScatterStyle::applyTo(CPlotPainter* painter, const CPen& defaultPen)const
{
	painter->setPen(mPenDefined?mPen:defaultPen);
	painter->setBrush(mBrush);
}

void CScatterStyle::drawShape(CPlotPainter* painter, const CPointF& pos)const
{
	drawShape(painter,pos.x(),pos.y());
}

void CScatterStyle::drawShape(CPlotPainter* painter, double x, double y)const
{
	double w=mSize/2.0;
	switch(mShape)
	{
		case ssNone: break;
		case ssDot:
		{
			painter->drawLine(CPointF(x, y), CPointF(x+0.0001, y));
			break;
		}
		case ssCross:
		{
			painter->drawLine(CLineF(x-w, y-w, x+w, y+w));
			painter->drawLine(CLineF(x-w, y+w, x+w, y-w));
			break;
		}
		case ssPlus:
		{
			painter->drawLine(CLineF(x-w, y, x+w, y));
			painter->drawLine(CLineF(x, y+w, x, y-w));
			break;
		}
		
		case ssCircle:
		{
			painter->drawEllipse(CPointF(x,y),w,w);
			break;
		}
		
		case ssDisc:
		{
			CBrush b=painter->brush();
			painter->setBrush(painter->pen().color());
			painter->drawEllipse(CPointF(x, y),w,w);
			painter->setBrush(b);
			break;
		}
		
		case ssSquare:
		{
			painter->drawRect(CRectF(x-w, y-w, mSize, mSize));
			break;
		}
		case ssDiamond:
		{
			painter->drawLine(CLineF(x-w, y, x, y-w));
			painter->drawLine(CLineF(x, y-w, x+w, y));
			painter->drawLine(CLineF(x+w, y, x,y+w));
			painter->drawLine(CLineF(x, y+w, x-w, y));
			break;
		}
		case ssStar:
		{
			painter->drawLine(CLineF(x-w,y, x+w, y));
			painter->drawLine(CLineF(x, y+w, x, y-w));
			painter->drawLine(CLineF(x-w*0.707,y-w*0.707,x+w*0.707,y+w*0.707));
			painter->drawLine(CLineF(x-w*0.707,y+w*0.707,x+w*0.707,y-w*0.707));
			break;
		}
		
		case ssTriangle:
		{
			painter->drawLine(CLineF(x-w, y+0.755*w, x+w, y+0.755*w));
			painter->drawLine(CLineF(x+w, y+0.755*w, x, y-0.977*w));
			painter->drawLine(CLineF(x, y-0.977*w, x-w, y+0.755*w));
			break;
		}
		
		case ssTriangleInverted:
		{
			painter->drawLine(CLineF(x-w, y-0.755*w, x+w, y-0.755*w));
			painter->drawLine(CLineF(x+w, y-0.755*w, x, y+0.977*w));
			painter->drawLine(CLineF(x, y+0.977*w, x-w, y-0.755*w));
			break;
		}
		
		case ssCrossSquare:
		{
			painter->drawLine(CLineF(x-w,y-w, x+w*0.95, y+w*0.95));
			painter->drawLine(CLineF(x-w, y+w*0.95, x+w*0.95, y-w));
			painter->drawRect(CRectF(x-w, y-w, mSize, mSize));
			break;
		}
		case ssPlusSquare:
		{
			painter->drawLine(CLineF(x-w,y, x+w*0.95,y));
			painter->drawLine(CLineF(x, y+w, x,y-w));
			painter->drawRect(CRect(x-w, y-w, mSize, mSize));
			break;
		}
		case ssCrossCircle:
		{
			painter->drawLine(CLineF(x-w*0.707, y-w*0.707, x+w*0.670,y+w*0.670));
			painter->drawLine(CLineF(x-w*0.707,y+w*0.670, x+w*0.670, y-w*0.707));
			painter->drawEllipse(CPointF(x,y),w,w);
			break;
		}
		case ssPlusCircle:
		{
			painter->drawLine(CLineF(x-w, y, x+w, y));
			painter->drawLine(CLineF(x, y+w, x, y-w));
			painter->drawEllipse(CPointF(x, y),w,w);
			break;
		}
		case ssPeace:
		{
			painter->drawLine(CLineF(x, y-w, x, y+w));
			painter->drawLine(CLineF(x, y, x-w*0.707, y+w*0.707));
			painter->drawLine(CLineF(x, y, x+w*0.707, y+w*0.707));
			painter->drawEllipse(CPointF(x, y),w,w);
			break;
		}
		
		case ssPixmap:
		{
			const double widthHalf=mPixmap.width()*0.5;
			const double heightHalf=mPixmap.height()*0.5;
			const CRectF clipRect=painter->clipBoundingRect().adjusted(-widthHalf, -heightHalf, widthHalf, heightHalf);
			if(clipRect.contains(x,y))
			{
				painter->drawImage(x-widthHalf, y-heightHalf, mPixmap);
			}
			break;
		}
		case ssCustom:
		{
			CTransform oldTransform=painter->transform();
			painter->translate(x, y);
			painter->scale(mSize/6.0,mSize/6.0);
			painter->drawPath(mCustomPath);
			painter->setTransform(oldTransform);
			break;
		}
	}
}


CSelectionDecorator::CSelectionDecorator():
mPen(0xff5050ff,2.5),
mBrush(Gemini::NoBrush),
mScatterStyle(CScatterStyle::ssNone, CPen(Gemini::blue,2),CBrush(Gemini::NoBrush), 6.0),
mUsedScatterProperties(CScatterStyle::spPen),
mPlottable(0)
{
	
}

CSelectionDecorator::~CSelectionDecorator()
{
	
}

void CSelectionDecorator::setPen(const CPen& pen)
{
	mPen=pen;
}

void CSelectionDecorator::setBrush(const CBrush& brush)
{
	mBrush = brush;
}

void CSelectionDecorator::setScatterStyle(const CScatterStyle& scatterStyle, WORD usedProperties)
{
	mScatterStyle=scatterStyle;
	setUsedScatterProperties(usedProperties);
}

void CSelectionDecorator::setUsedScatterProperties(WORD usedProperties)
{
	mUsedScatterProperties=usedProperties;
}

void CSelectionDecorator::applyPen(CPlotPainter* painter)const
{
	painter->setPen(mPen);
}
void CSelectionDecorator::applyBrush(CPlotPainter* painter)const
{
	painter->setBrush(mBrush);
}

CScatterStyle CSelectionDecorator::getFinalScatterStyle(const CScatterStyle& unselectedScatterStyle)const
{
	CScatterStyle result=unselectedScatterStyle;
	result.setFromOther(mScatterStyle, mUsedScatterProperties);
	if(!result.isPenDefined())
		result.setPen(mPen);
	return result;
}

void CSelectionDecorator::copyFrom(const CSelectionDecorator* other)
{
	setPen(other->pen());
	setBrush(other->brush());
	setScatterStyle(other->scatterStyle(), other->usedScatterProperties());
}

bool CSelectionDecorator::registerWidthPlottable(CAbstractPlottable* plottable)
{
	if(!mPlottable)
	{
		mPlottable = plottable;
		return true;
	}
	else
	{
		return false;
	}
}



/*===================Bars Group===================*/

CBarsGroup::CBarsGroup():
mSpacingType(stAbsolute),
mSpacing(4)
{
	
}

CBarsGroup::~CBarsGroup()
{
	clear();
}

void CBarsGroup::setSpacingType(WORD spacingType)
{
	mSpacingType=spacingType;
}

void CBarsGroup::setSpacing(double spacing)
{
	mSpacing=spacing;
}

CBars* CBarsGroup::bars(int index)const
{
	if(index>=0&&index<mBars.size())
		return mBars.at(index);
	return 0;
}

void CBarsGroup::clear()
{
	for(int i=0; i<mBars.size();++i)
	{
		CBars* bars=mBars.at(i);
		bars->setBarsGroup(0);
	}
}

void CBarsGroup::append(CBars* bars)
{
	if(bars)
	{
		if(!mBars.contains(bars))
			bars->setBarsGroup(this);
	}
}

void CBarsGroup::insert(int i, CBars* bars)
{
	if(bars)
	{
		if(!mBars.contains(bars))
		{
			bars->setBarsGroup(this);
		}
		mBars.move(mBars.indexOf(bars), mBound(0,i, mBars.size()-1));
	}
}

void CBarsGroup::remove(CBars* bars)
{
	if(bars)
	{
		if(mBars.contains(bars))
			bars->setBarsGroup(0);
	}
}


void CBarsGroup::registerBars(CBars* bars)
{
	if(!mBars.contains(bars))
	{
		mBars.append(bars);
	}
}

void CBarsGroup::unRegisterBars(CBars* bars)
{
	if(mBars.contains(bars))
	{
		mBars.removeOne(bars);
	}
}

double CBarsGroup::keyPixelOffset(const CBars* bars, double keyCoord)
{
	CList<const CBars*> baseBars;
	for(int i=0; i<mBars.size();++i)
	{
		CBars* b=mBars.at(i);
		while(b->barBelow())
			b=b->barBelow();
		if(!baseBars.contains(b))
			baseBars.append(b);
	}
	
	const CBars* thisBase=bars;
	while(thisBase->barBelow())
		thisBase =thisBase->barBelow();
	
	double result =0;
	int index = baseBars.indexOf(thisBase);
	if(index>=0)
	{
		if(baseBars.size()%2==1&&index==(baseBars.size()-1)/2)
		{
			return result;
		}
		else
		{
			double lowerPixelWidth, upperPixelWidth;
			int startIndex;
			int dir=(index<=(baseBars.size()-1)/2)?-1:1;
			if(baseBars.size()%2==0)
			{
				startIndex = baseBars.size()/2+(dir<0?-1:0);
				result+=getPixelSpacing(baseBars.at(startIndex),keyCoord)*0.5;
			}
			else{
				startIndex = (baseBars.size()-1)/2+dir;
				baseBars.at((baseBars.size()-1)/2)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
				result+=mAbs(upperPixelWidth-lowerPixelWidth)*0.5;
				result+=getPixelSpacing(baseBars.at((baseBars.size()-1)/2),keyCoord);
			}
			for(int i=startIndex; i!=index; i+=dir)
			{
				baseBars.at(i)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
				result+=mAbs(upperPixelWidth-lowerPixelWidth);
				result+=getPixelSpacing(baseBars.at(i),keyCoord);
			}
			
			baseBars.at(index)->getPixelWidth(keyCoord, lowerPixelWidth, upperPixelWidth);
			result+=mAbs(upperPixelWidth-lowerPixelWidth)*0.5;
			result*=dir*thisBase->keyAxis()->pixelOrientation();
		}
	}
	return result;
}


double CBarsGroup::getPixelSpacing(const CBars* bars, double keyCoord)
{
	switch(mSpacingType)
	{
		case stAbsolute:
		{
			return mSpacing;
		}
		case stAxisRectRatio:
		{
			if(bars->keyAxis()->orientation()==Gemini::Horizontal)
			{
				return bars->keyAxis()->axisRect()->width()*mSpacing;
			}
			else
				return bars->keyAxis()->axisRect()->height()*mSpacing;
		}
		case stPlotCoords:
		{
			double keyPixel =bars->keyAxis()->coordToPixel(keyCoord);
			return mAbs(bars->keyAxis()->coordToPixel(keyCoord+mSpacing)-keyPixel);
		}
	}
	return 0;
}

CBarsData::CBarsData():
key(0),
value(0)
{
	
}
CBarsData::CBarsData(double key, double value):
key(key),
value(value)
{
	
}



CBars::CBars(CPlotAxis* keyAxis, CPlotAxis* valueAxis):
CAbstractPlottable1D<CBarsData>(keyAxis, valueAxis),
mWidth(0.75),
mWidthType(wtPlotCoords),
mBarsGroup(0),
mBaseValue(0),
mStackingGap(0),
mBarBelow(0),
mBarAbove(0)
{
	setAntialiased(false);
	mPen.setColor(Gemini::blue);
	mPen.setStyle(Gemini::SolidLine);
	mBrush.setColor(0x2832ff1e);
	mBrush.setStyle(Gemini::SolidPattern);
	mSelectionDecorator->setBrush(CBrush(0xffa0a0ff));
}

CBars::~CBars()
{
	setBarsGroup(0);
	if(mBarBelow||mBarAbove)
		connectBars(mBarBelow,mBarAbove);
}

void CBars::setData(CSharedPointer<CBarsDataContainer> data)
{
	mDataContainer= data;
}

void CBars::setData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted)
{
	mDataContainer->clear();
	addData(keys, values, alreadySorted);
}

void CBars::setWidth(double width)
{
	mWidth=width;
}

void CBars::setWidthType(WORD widthType)
{
	mWidthType=widthType;
}


void CBars::setBarsGroup(CBarsGroup* barsGroup)
{
	if(mBarsGroup)
		mBarsGroup->unRegisterBars(this);
	mBarsGroup=barsGroup;
	if(mBarsGroup)
		mBarsGroup->registerBars(this);
}


void CBars::setBaseValue(double baseValue)
{
	mBaseValue= baseValue;
}

void CBars::setStackingGap(double pixels)
{
	mStackingGap = pixels;
}

void CBars::addData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted)
{
	if(keys.size()!=values.size())
	{
		std::cout<<"Error: keys and vallues have different sizes: "<<keys.size()<<","<<values.size()<<std::endl;
	}
	
	const int n=mMin(keys.size(),values.size());
	CVector<CBarsData> tempData(n);
	CVector<CBarsData>::iterator it=tempData.begin();
	const CVector<CBarsData>::iterator itEnd=tempData.end();
	
	int i=0;
	while(it!=itEnd)
	{
		it->key = keys[i];
		it->value = values[i];
		++it;
		++i;
	}
	
	mDataContainer->add(tempData, alreadySorted);
}

void CBars::addData(double key, double value)
{
	mDataContainer->add(CBarsData(key,value));
}

void CBars::moveBelow(CBars* bars)
{
	if(bars==this) return;
	if(bars&&(bars->keyAxis()!=mKeyAxis||bars->valueAxis()!=mValueAxis))
	{
		return ;
	}
	
	connectBars(mBarBelow,mBarAbove);
	if(bars)
	{
		if(bars->mBarBelow)
			connectBars(bars->mBarBelow,this);
		connectBars(this, bars);
	}
}

void CBars::moveAbove(CBars* bars)
{
	
	if(bars==this) return;
	if(bars&&(bars->keyAxis()!=mKeyAxis||bars->valueAxis()!=mValueAxis))
	{
		return ;
	}
	
	connectBars(mBarBelow,mBarAbove);
	if(bars)
	{
		if(bars->mBarAbove)
			connectBars(this,bars->mBarAbove);
		connectBars(bars,this);
	}
}



CRange CBars::getKeyRange(bool& foundRange, WORD inSignDomain)const
{
	CRange range;
	range = mDataContainer->keyRange(foundRange, inSignDomain);
	if(foundRange&&mKeyAxis)
	{
		double lowerPixelWidth, upperPixelWidth, keyPixel;
		
		getPixelWidth(range.lower, lowerPixelWidth, upperPixelWidth);
		keyPixel=mKeyAxis->coordToPixel(range.lower)+lowerPixelWidth;
		if(mBarsGroup)
			keyPixel+=mBarsGroup->keyPixelOffset(this, range.lower);
		const double lowerCorrected=mKeyAxis->pixelToCoord(keyPixel);
		if(!isnan(lowerCorrected)&&isinf(lowerCorrected)&&range.lower>lowerCorrected)
			range.lower=lowerCorrected;
		getPixelWidth(range.upper, lowerPixelWidth, upperPixelWidth);
		keyPixel=mKeyAxis->coordToPixel(range.upper)+upperPixelWidth;
		if(mBarsGroup)
			keyPixel+=mBarsGroup->keyPixelOffset(this, range.upper);
		const double upperCorrected=mKeyAxis->pixelToCoord(keyPixel);
		if(!isnan(upperCorrected)&&isinf(upperCorrected)&&range.upper<lowerCorrected)
			range.upper=upperCorrected;
		
	}
	return range;
}

CRange CBars::getValueRange(bool& foundRange, WORD inSignDomain, const CRange& inKeyRange)const
{
	
	CRange range;
	range.lower=mBaseValue;
	range.upper=mBaseValue;
	bool haveLower=true;
	bool haveUpper=true;
	
	CBarsDataContainer::const_iterator itBegin=mDataContainer->constBegin();
	CBarsDataContainer::const_iterator itEnd=mDataContainer->constEnd();
	
	if(inKeyRange!=CRange())
	{
		itBegin=mDataContainer->findBegin(inKeyRange.lower);
		itEnd=mDataContainer->findEnd(inKeyRange.upper);
	}
	
	for(CBarsDataContainer::const_iterator it=itBegin;it!=itEnd; ++it)
	{
		const double current=it->value+getStackedBaseValue(it->key, it->value>=0);
		if(isnan(current)) continue;
		if(inSignDomain==Plot::sdBoth||(inSignDomain==Plot::sdNegative&&current<0)||(inSignDomain==Plot::sdPositive&&current>0))
		{
			if(current<range.lower||!haveLower)
			{
				range.lower=current;
				haveLower=true;
			}
			if(current>range.upper||!haveUpper)
			{
				range.upper=current;
				haveUpper=true;
			}
		}
	}
	foundRange =true;
	return range;
}



CPointF CBars::dataPixelPosition(int index)const
{
	if(index>=0&&index<mDataContainer->size())
	{
		CPlotAxis* keyAxis=mKeyAxis;
		CPlotAxis* valueAxis=mValueAxis;
		if(!keyAxis||!valueAxis)	return CPointF();
		
		const CDataContainer<CBarsData>::const_iterator it=mDataContainer->constBegin();
		const double valuePixel=valueAxis->coordToPixel(getStackedBaseValue(it->value, it->value>=0)+it->value);
		const double keyPixel=keyAxis->coordToPixel(it->key)+(mBarsGroup?mBarsGroup->keyPixelOffset(this, it->key):0);
		if(keyAxis->orientation()==Gemini::Horizontal)
			return CPointF(keyPixel, valuePixel);
		else
			return CPointF(valuePixel, keyPixel);
	}
	return CPointF();
}



void CBars::draw(CPlotPainter* painter)
{
	if(!mKeyAxis||!mValueAxis)	return ;
	if(mDataContainer->isEmpty())	return;
	
	CBarsDataContainer::const_iterator visibleBegin, visibleEnd;
	
	getVisibleDataBounds(visibleBegin, visibleEnd);
	
	CList<CDataRange> selectedSegments, unSelectedSegments, allSegments;
	getDataSegments(selectedSegments, unSelectedSegments);
	allSegments<<unSelectedSegments<<selectedSegments;
	for(int i=0; i<allSegments.size();++i)
	{
		bool isSelectedSegment=i>=unSelectedSegments.size();
		
		CBarsDataContainer::const_iterator begin=visibleBegin;
		CBarsDataContainer::const_iterator end=visibleEnd;
		
		mDataContainer->limitIteratorToDataRange(begin, end, allSegments.at(i));
		if(begin==end)
			continue;
		for(CBarsDataContainer::const_iterator it=begin; it!=end; ++it)
		{
			if(isSelectedSegment&&mSelectionDecorator)
			{
				mSelectionDecorator->applyBrush(painter);
				mSelectionDecorator->applyPen(painter);
			}
			else
			{
				painter->setBrush(mBrush);
				painter->setPen(mPen);
			}
			applyDefaultAntialiasingHint(painter);
			CRectF r=getBarRect(it->key,it->value);
			painter->drawPolygon(r);
		}
	
	}
	
	
}


void CBars::drawLegendIcon(CPlotPainter* painter, const CRectF& rect)const
{
	if(mVisible)
	{
		applyDefaultAntialiasingHint(painter);
		painter->setBrush(mBrush);
		painter->setPen(mPen);
		CRectF r=CRectF(0,0,rect.width()*0.67, rect.height()*0.67);
		r.moveCenter(rect.center());
		painter->drawRect(r);
	}
}



void CBars::getVisibleDataBounds(CBarsDataContainer::const_iterator& begin, CBarsDataContainer::const_iterator& end)const
{
	if(!mKeyAxis)
	{
		begin=mDataContainer->constEnd();
		end=mDataContainer->constEnd();
		return;
	}
	
	if(mDataContainer->isEmpty())
	{
		begin=mDataContainer->constEnd();
		end = mDataContainer->constEnd();
		return ;
	}
	
	begin = mDataContainer->findBegin(mKeyAxis->range().lower);
	end = mDataContainer->findEnd(mKeyAxis->range().upper);
	
	double lowerPixelBound=mKeyAxis->coordToPixel(mKeyAxis->range().lower);
	double upperPixelBound=mKeyAxis->coordToPixel(mKeyAxis->range().upper);
	
	bool isVisible=false;
	
	CBarsDataContainer::const_iterator it = begin;
	
	while(it!=mDataContainer->constBegin())
	{
		--it;
		const CRectF barRect=getBarRect(it->key, it->value);
		if(mKeyAxis->orientation()==Gemini::Horizontal)
			isVisible=((!mKeyAxis->rangeReversed()&&barRect.right()>=lowerPixelBound)||(mKeyAxis->rangeReversed()&&barRect.left()<=lowerPixelBound));
		else
			isVisible=((!mKeyAxis->rangeReversed()&&barRect.top()<=lowerPixelBound)||(mKeyAxis->rangeReversed()&&barRect.bottom()>=lowerPixelBound));
		if(isVisible)
			begin=it;
		else
			break;
	}
	
	it = end;
	while(it!=mDataContainer->constEnd())
	{
		const CRectF barRect=getBarRect(it->key, it->value);
		if(mKeyAxis->orientation()==Gemini::Horizontal)
			isVisible=((!mKeyAxis->rangeReversed()&&barRect.left()<=upperPixelBound)||(mKeyAxis->rangeReversed()&&barRect.right()>=upperPixelBound));
		else
			isVisible=((!mKeyAxis->rangeReversed()&&barRect.bottom()>=upperPixelBound)||(mKeyAxis->rangeReversed()&&barRect.top()<=upperPixelBound));
		if(isVisible)
			end=it+1;
		else
			break;
		++it;
	}
}


CRectF CBars::getBarRect(double key, double value)const
{
	CPlotAxis* keyAxis=mKeyAxis;
	CPlotAxis* valueAxis=mValueAxis;
	
	if(!keyAxis||!valueAxis) return CRectF();
	double lowerPixelWidth, upperPixelWidth;
	getPixelWidth(key,lowerPixelWidth, upperPixelWidth);
	
	double base = getStackedBaseValue(key, value>=0);
	double basePixel=valueAxis->coordToPixel(base);
	double valuePixel = valueAxis->coordToPixel(base+value);
	double keyPixel = keyAxis->coordToPixel(key);
	
	if(mBarsGroup)
		keyPixel+=mBarsGroup->keyPixelOffset(this, key);
	
	double bottomOffset=(mBarBelow&&mPen.style()!=Gemini::NoPen?1:0)*(mPen.isCosmetic()?1:mPen.widthF());
	bottomOffset+=mBarBelow?mStackingGap:0;
	bottomOffset*=(value<0?-1:1)*valueAxis->pixelOrientation();
	if(mAbs(valuePixel-basePixel)<mAbs(bottomOffset))
	{
		//修正像素点小于1时绘制polygon的问题
		bottomOffset=(value<0?-1:1)*valueAxis->pixelOrientation();
		bottomOffset=(valuePixel-basePixel)==0?0:valuePixel-basePixel+bottomOffset;
	}
	if(keyAxis->orientation()==Gemini::Horizontal)
	{
		return CRectF(CPointF(keyPixel+lowerPixelWidth, valuePixel), CPointF(keyPixel+upperPixelWidth, basePixel+bottomOffset)).normalized();
	}
	else
	{
		return CRectF(CPointF(basePixel+bottomOffset, keyPixel+lowerPixelWidth), CPointF(valuePixel, keyPixel+upperPixelWidth)).normalized();
	}	
}


void CBars::getPixelWidth(double key, double& lower, double& upper)const
{
	lower=0;
	upper=0;
	switch(mWidthType)
	{
		case wtAbsolute:
		{
			if(mKeyAxis)
			{
				upper = mWidth*0.5*mKeyAxis->pixelOrientation();
				lower=-upper;
			}
			break;
		}
		case wtAxisRectRatio:
		{
			if(mKeyAxis&&mKeyAxis->axisRect())
			{
				if(mKeyAxis->orientation()==Gemini::Horizontal)
					upper=mKeyAxis->axisRect()->width()*mWidth*0.5*mKeyAxis->pixelOrientation();
				else
					upper=mKeyAxis->axisRect()->height()*mWidth*0.5*mKeyAxis->pixelOrientation();
				lower =-upper;
				
				
			}
			break;
		}
		case wtPlotCoords:
		{
			if(mKeyAxis)
			{
				double keyPixel = mKeyAxis->coordToPixel(key);
				upper =  mKeyAxis->coordToPixel(key+mWidth*0.5)-keyPixel;
				lower=   mKeyAxis->coordToPixel(key-mWidth*0.5)-keyPixel;
			}
			break;
		}
	}
}


double CBars::getStackedBaseValue(double key, bool positive)const
{
	if(mBarBelow)
	{
		double max = 0;
		double epsilon=mAbs(key)*(sizeof(key)==4?1e-6:1e-14);
		if(key==0)
			epsilon=(sizeof(key)==4?1e-6:1e-14);
		CBarsDataContainer::const_iterator it = mBarBelow->mDataContainer->findBegin(key-epsilon);
		CBarsDataContainer::const_iterator itEnd=mBarBelow->mDataContainer->findEnd(key+epsilon);
		while(it!=itEnd)
		{
			if(it->key>key-epsilon&&it->key<key+epsilon)
			{
				if((positive&&it->value>max)||(!positive&&it->value<max))
					max = it->value;
			}
			++it;
		}
		
		return max+mBarBelow->getStackedBaseValue(key, positive);
	}
	else
		return mBaseValue;
}

void CBars::connectBars(CBars* lower, CBars* upper)
{
	if(!lower&&!upper) return ;
	if(!lower)
	{
		if(upper->mBarBelow&& upper->mBarBelow->mBarAbove==upper)
			upper->mBarBelow->mBarAbove=0;
		upper->mBarBelow=0;
	}
	else if(!upper)
	{
		if(lower->mBarAbove&&lower->mBarAbove->mBarBelow==lower)
			lower->mBarAbove->mBarBelow=0;
		lower->mBarAbove=0;
	}
	else
	{
		if(lower->mBarAbove&&lower->mBarAbove->mBarBelow==lower)
			lower->mBarAbove->mBarBelow=0;
		
		if(upper->mBarBelow&& upper->mBarBelow->mBarAbove==upper)
			upper->mBarBelow->mBarAbove=0;
		
		
		lower->mBarAbove = upper;
		upper->mBarBelow = lower;
	}
}



CLayoutElement::CLayoutElement():
mParentLayout(0),
mMinimumSize(),
mMaximumSize(500,500),
mRect(0,0,0,0),
mOuterRect(0,0,0,0),
mMargins(0,0,0,0),
mMinimumMargins(0,0,0,0),
mAutoMargins(Plot::msAll)
{
	
}

CLayoutElement::~CLayoutElement()
{
	setMarginGroup(Plot::msAll, 0);
	if(reinterpret_cast<CLayout*>(mParentLayout))
		mParentLayout->take(this);
}

void CLayoutElement::setOuterRect(const CRect& rect)
{
	if(mOuterRect!=rect)
	{
		mOuterRect = rect;
		mRect=mOuterRect.adjusted(mMargins.left(), mMargins.top(),-mMargins.right(), -mMargins.bottom());
	}
}


void CLayoutElement::setMargins(const CMargin& margins)
{
	if(mMargins!=margins)
	{
		mMargins= margins;
		mRect=mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(),-mMargins.bottom());
	}
}


void CLayoutElement::setMinimumMargins(const CMargin& margins)
{
	if(mMinimumMargins!=margins)
	{
		mMinimumMargins=margins;
	}
}

void CLayoutElement::setAutoMargins(WORD sides)
{
	mAutoMargins=sides;
}


void CLayoutElement::setMinimumSize(const CSize& size)
{
	if(mMinimumSize!=size)
	{
		mMinimumSize = size;
		if(mParentLayout)
			mParentLayout->sizeConstraintsChanged();
	}
}


void CLayoutElement::setMinimumSize(int width, int height)
{
	setMinimumSize(CSize(width, height));
}

void CLayoutElement::setMaximumSize(const CSize& size)
{
	if(mMaximumSize!=size)
	{
		mMaximumSize=size;
		if(mParentLayout)
			mParentLayout->sizeConstraintsChanged();
	}
}

void CLayoutElement::setMaximumSize(int width, int height)
{
	setMaximumSize(CSize(width, height));
}


void CLayoutElement::setMarginGroup(WORD sides, CMarginGroup* group)
{
	CVector<WORD> sideVector;
	if((sides&Plot::msLeft)==Plot::msLeft) sideVector.append(Plot::msLeft);
	if((sides&Plot::msRight)==Plot::msRight) sideVector.append(Plot::msRight);
	if((sides&Plot::msTop)==Plot::msTop) sideVector.append(Plot::msTop);
	if((sides&Plot::msBottom)==Plot::msBottom) sideVector.append(Plot::msBottom);
	
	
	for(int i=0; i<sideVector.size(); ++i)
	{
		WORD side=sideVector.at(i);
		if(marginGroup(side)!=group)
		{
			CMarginGroup* oldGroup = marginGroup(side);
			if(oldGroup)
				oldGroup->removeChild(side, this);
			if(!group)
			{
				mMarginGroups.erase(side);
			}
			else
			{
				mMarginGroups[side]=group;
				group->addChild(side, this);
			}
		}
	}
}


void CLayoutElement::update(WORD phase)
{
	if(phase==upMargins)
	{
		if(mAutoMargins!=Plot::msNone)
		{
			CMargin newMargins=mMargins;
			CList<WORD> allMarginSides=CList<WORD>()<<Plot::msLeft<<Plot::msRight<<Plot::msTop<<Plot::msBottom;
			for(int i=0; i<allMarginSides.size();++i)
			{
				WORD side=allMarginSides[i];
				if((mAutoMargins&side)==side)
				{
					std::map<WORD, CMarginGroup*>::iterator it=mMarginGroups.find(side);
					if(it!=mMarginGroups.end())
						Plot::setMarginValue(newMargins, side, mMarginGroups[side]->commonMargin(side));
					else
						Plot::setMarginValue(newMargins, side, calculateAutoMargin(side));
					if(Plot::getMarginValue(newMargins,side)<Plot::getMarginValue(mMinimumMargins, side))
						Plot::setMarginValue(newMargins,side, Plot::getMarginValue(mMinimumMargins, side));
				}
			}
			setMargins(newMargins);
		}
	}
}

CSize CLayoutElement::minimumSizeHint()const
{
	return mMinimumSize;
}

CSize CLayoutElement::maximumSizeHint()const
{
	return mMaximumSize;
}

CList<CLayoutElement*> CLayoutElement::elements(bool recursive)const
{
	UNUSED_PARAM(recursive);
	return CList<CLayoutElement*>();
}

int CLayoutElement::calculateAutoMargin(WORD side)
{
	return mMax(Plot::getMarginValue(mMargins,side),Plot::getMarginValue(mMinimumMargins,side));
}

void CLayoutElement::layoutChanged()
{
	
}





CMarginGroup::CMarginGroup()
{
	mChildren.insert(std::pair<WORD, CList<CLayoutElement*> >(Plot::msLeft, CList<CLayoutElement*>()));
	mChildren.insert(std::pair<WORD, CList<CLayoutElement*> >(Plot::msTop, CList<CLayoutElement*>()));
	mChildren.insert(std::pair<WORD, CList<CLayoutElement*> >(Plot::msRight, CList<CLayoutElement*>()));
	mChildren.insert(std::pair<WORD, CList<CLayoutElement*> >(Plot::msBottom, CList<CLayoutElement*>()));
}


CMarginGroup::~CMarginGroup()
{
	clear();
}

bool CMarginGroup::isEmpty()const
{
	std::map<WORD, CList<CLayoutElement*> >::const_iterator it=mChildren.begin();
	for(;it!=mChildren.end();++it)
	{
		if(!it->second.isEmpty())
			return false;
	}
	return true;
}

void CMarginGroup::clear()
{
	std::map<WORD, CList<CLayoutElement*> >::iterator it=mChildren.begin();
	for(; it!=mChildren.end();++it)
	{
		const CList<CLayoutElement*> elements=it->second;
		for(int i=elements.size()-1;i>=0;--i)
		{
			elements.at(i)->setMarginGroup(it->first,0);
		}
	}
}

int CMarginGroup::commonMargin(WORD side)
{
	int result=0;
	if(side<Plot::msLeft||side>=Plot::msAll)
		return result;
	const CList<CLayoutElement*> elements=mChildren[side];
	for(int i=0; i<elements.size();++i)
	{
		if((elements.at(i)->autoMargins()&side)!=side)
		{
			continue;
		}
		int m=mMax(elements.at(i)->calculateAutoMargin(side),Plot::getMarginValue(elements.at(i)->minimumMargins(),side));
		if(m>result)
			result=m;
	}
	return result;
}


void CMarginGroup::addChild(WORD side, CLayoutElement* element)
{
	if(side<Plot::msLeft||side>=Plot::msAll)	return ;
	if(!mChildren[side].contains(element))
		mChildren[side].append(element);
}

void CMarginGroup::removeChild(WORD side, CLayoutElement* element)
{
	if(side<Plot::msLeft||side>=Plot::msAll)	return ;
	if(!mChildren[side].removeOne(element))
	{
		;
	}
}



CLayout::CLayout()
{
	
}

void CLayout::update(WORD phase)
{
	CLayoutElement::update(phase);
	
	if(phase==upLayout)
		updateLayout();
	const int elCount=elementCount();
	for(int i=0; i<elCount;++i)
	{
		if(CLayoutElement* el=elementAt(i))
			el->update(phase);
	}
}

CList<CLayoutElement*> CLayout::elements(bool recursive)const
{
	const int c=elementCount();
	CList<CLayoutElement*> result;
	result.reserve(c);
	for(int i=0; i<c;++i)
		result.append(elementAt(i));
	if(recursive)
	{
		for(int i=0; i<c;++i)
		{
			if(result.at(i))
				result<<result.at(i)->elements(recursive);
		}
	}
	
	return result;
}

void CLayout::simplify()
{
	
}

bool CLayout::removeAt(int index)
{
	if(CLayoutElement* el=takeAt(index))
	{
		delete el;
		return true;
	}
	else
		return false;
}

bool CLayout::remove(CLayoutElement* element)
{
	if(take(element))
	{
		delete element;
		return true;
	}
	else
	{
		return false;
	}
}

void CLayout::clear()
{
	for(int i=elementCount()-1;i>=0; --i)
	{
		if(elementAt(i))
			removeAt(i);
	}
	simplify();
}

void CLayout::sizeConstraintsChanged()const
{
	;
}

void CLayout::updateLayout()
{
	
}

void CLayout::adoptElement(CLayoutElement* el)
{
	if(el)
	{
		el->mParentLayout=this;
		el->setParentLayerable(this);
//		el->setParent(this);
		el->layoutChanged();
	}
}

void CLayout::releaseElement(CLayoutElement* el)
{
	if(el)
	{
		el->mParentLayout=0;
		el->setParentLayerable(0);
	}
}

CVector<int>  CLayout::getSectionSizes(CVector<int> maxSizes, CVector<int>minSizes, CVector<double> stretchFactors, int totalSize)const
{
	if(maxSizes.size()!=minSizes.size()||stretchFactors.size()!=maxSizes.size())
		return CVector<int>();
	if(stretchFactors.isEmpty())
		return CVector<int>();
	
	int sectionCount=stretchFactors.size();
	CVector<double> sectionSizes(sectionCount);
	int minSizeSum=0;
	for(int i=0; i<sectionCount; ++i)
		minSizeSum +=minSizes.at(i);
	if(totalSize<minSizeSum)
	{
		for(int i=0; i<sectionCount;++i)
		{
			stretchFactors[i] = minSizes.at(i);
			minSizes[i]=0;
		}
	}
	
	CList<int> minimumLockedSections;
	CList<int> unfinishedSections;
	
	for(int i=0; i<sectionCount; ++i)
		unfinishedSections.append(i);
	
	double freeSize = totalSize;
	int outerIterations =0;
	
	while(!unfinishedSections.isEmpty()&&outerIterations<sectionCount*2)
	{
		++outerIterations;
		int innerIterations=0;
		while(!unfinishedSections.isEmpty()&&innerIterations<sectionCount*2)
		{
			++innerIterations;
			int nextId = -1;
			double nextMax =1e12;
			for(int i=0; i<unfinishedSections.size();++i)
			{
				int secId=unfinishedSections.at(i);
				double hitsMaxAt=(maxSizes.at(secId)-sectionSizes.at(secId))/stretchFactors.at(secId);
				if(hitsMaxAt<nextMax)
				{
					nextMax = hitsMaxAt;
					nextId=secId;
				}
			}
			
			double stretchFactorSum=0;
			for(int i=0; i<unfinishedSections.size();++i)
				stretchFactorSum+=stretchFactors.at(unfinishedSections.at(i));
			double nextMaxLimit=freeSize/stretchFactorSum;
			
			if(nextMax< nextMaxLimit)
			{
				for(int i=0; i<unfinishedSections.size();++i)
				{
					sectionSizes[unfinishedSections.at(i)]+=nextMax*stretchFactors.at(unfinishedSections.at(i));
					freeSize -=nextMax*stretchFactors.at(unfinishedSections.at(i));
				}
				unfinishedSections.removeOne(nextId);
			}else
			{
				for(int i=0; i<unfinishedSections.size();++i)
					sectionSizes[unfinishedSections.at(i)]+=nextMaxLimit*stretchFactors.at(unfinishedSections.at(i));
				unfinishedSections.clear();
			}
		}
		bool foundMinimumViolation =false;
		for(int i=0; i<sectionSizes.size();++i)
		{
			if(minimumLockedSections.contains(i))
				continue;
			if(sectionSizes.at(i)<minSizes.at(i))
			{
				sectionSizes[i]=minSizes.at(i);
				foundMinimumViolation = true;
				minimumLockedSections.append(i);
			}
		}
		
		if(foundMinimumViolation)
		{
			freeSize=totalSize;
			for(int i=0; i<sectionCount; ++i)
			{
				if(!minimumLockedSections.contains(i))
					unfinishedSections.append(i);
				else
					freeSize=sectionSizes.at(i);
			}
			for(int i=0; i<unfinishedSections.size(); ++i)
			{
				sectionSizes[unfinishedSections.at(i)]=0;
			}
		}

	}
			
	CVector<int> result(sectionCount);
	for(int i=0; i<sectionCount;++i)
		result[i]=mRound(sectionSizes.at(i));
	return result;
	
}


CLayoutGrid::CLayoutGrid():
mColumnSpacing(5),
mRowSpacing(5),
mWrap(0),
mFillOrder(foRowsFirst)
{
	
}

CLayoutGrid::~CLayoutGrid()
{
	clear();
}

CLayoutElement* CLayoutGrid::element(int row, int column)const
{
	if(row>=0&&row<mElements.size())
	{
		if(column>=0&& column<mElements.first().size())
		{
			if(CLayoutElement* result = mElements.at(row).at(column))
				return result;
			else
				;
		}
		else
		{
			;
		}
		return 0;
	}
	return 0;
}


bool CLayoutGrid::addElement(int row, int column, CLayoutElement* element)
{
	if(!hasElement(row, column))
	{
		if(element&&element->layout())
			element->layout()->take(element);
		expandTo(row+1, column+1);
		mElements[row][column]=element;
		if(element)
			adoptElement(element);
		return true;
	}
	return false;
}

bool CLayoutGrid::addElement(CLayoutElement* element)
{
	int rowIndex = 0; 
	int colIndex = 0;
	if(mFillOrder==foColumnsFirst)
	{
		while(hasElement(rowIndex, colIndex))
		{
			++colIndex;
			if(colIndex>=mWrap&&mWrap>0)
			{
				colIndex = 0;
				++rowIndex;
			}
		}
	}
	else
	{
		while(hasElement(rowIndex, colIndex))
		{
			++rowIndex;
			if(rowIndex>=mWrap&&mWrap>0)
			{
				rowIndex = 0;
				++colIndex;
			}
		}
	}
	return addElement(rowIndex, colIndex, element);
}

bool CLayoutGrid::hasElement(int row, int column)
{
	if(row>=0&&row<rowCount()&&column>=0&&column<columnCount())
		return mElements.at(row).at(column);
	else
		return false;
}

void CLayoutGrid::setColumnStretchFactor(int column, double factor)
{
	if(column>=0&& column<columnCount())
	{
		if(factor>0)
			mColumnStretchFactors[column]=factor;
	}
}

void CLayoutGrid::setColumnStretchFactors(const CList<double>& factors)
{
	if(factors.size()==mColumnStretchFactors.size())
	{
		mColumnStretchFactors = factors;
		for(int i=0; i<mColumnStretchFactors.size();++i)
		{
			if(mColumnStretchFactors.at(i)<=0)
				mColumnStretchFactors[i]=1;
		}
	}
}

void CLayoutGrid::setRowStretchFactor(int row, double factor)
{
	if(row>=0&&row<rowCount())
	{
		if(factor>0)
			mRowStretchFactors[row] = factor;
	}
}

void CLayoutGrid::setRowStretchFactors(const CList<double>& factors)
{
	if(factors.size()==mRowStretchFactors.size())
	{
		mRowStretchFactors=factors;
		for(int i=0; i<mRowStretchFactors.size(); ++i)
		{
			if(mRowStretchFactors.at(i)<=0)
			{
				mRowStretchFactors[i]=1;
			}
		}
	}
}


void CLayoutGrid::setColumnSpacing(int pixels)
{
	mColumnSpacing=pixels;
}

void CLayoutGrid::setRowSpacing(int pixels)
{
	mRowSpacing = pixels;
}

void CLayoutGrid::setWrap(int count)
{
	mWrap = mMax(0,count);
}

void CLayoutGrid::setFillOrder(WORD order, bool rearrange)
{
	const int elCount = elementCount();
	CVector<CLayoutElement*> tempElements;
	if(rearrange)
	{
		tempElements.reserve(elCount);
		for(int i=0; i<elCount; ++i)
		{
			if(elementAt(i))
				tempElements.append(takeAt(i));
		}
		simplify();
	}
	mFillOrder=order;
	if(rearrange)
	{
		for(int i=0; i<tempElements.size();++i)
		{
			addElement(tempElements.at(i));
		}
	}
}

void CLayoutGrid::expandTo(int newRowCount, int newColumnCount)
{
	while(rowCount()<newRowCount)
	{
		mElements.append(CList<CLayoutElement*>());
		mRowStretchFactors.append(1);
	}
	
	int newColCount = mMax(columnCount(), newColumnCount);
	for(int i=0; i<rowCount(); ++i)
	{
		while(mElements.at(i).size()<newColCount)
			mElements[i].append(0);
	}
	while( mColumnStretchFactors.size()<newColCount)
		mColumnStretchFactors.append(1);
}

void CLayoutGrid::insertRow(int newIndex)
{
	if(mElements.isEmpty()||mElements.first().isEmpty())
	{
		expandTo(1,1);
		return ;
	}
	
	if(newIndex<0)
		newIndex=0;
	if(newIndex>rowCount())
		newIndex=rowCount();
	mRowStretchFactors.insert(newIndex,1);
	CList<CLayoutElement*> newRow;
	for(int col=0; col<columnCount();++col)
		newRow.append((CLayoutElement*)0);
	mElements.insert(newIndex, newRow);
}

void CLayoutGrid::insertColumn(int newIndex)
{
	if(mElements.isEmpty()||mElements.first().isEmpty())
	{
		expandTo(1,1);
		return ;
	}
	if(newIndex<0)
		newIndex = 0;
	if(newIndex>columnCount())
		newIndex=columnCount();
	mColumnStretchFactors.insert(newIndex,1);
	for(int row=0;row<rowCount();++row)
		mElements[row].insert(newIndex,(CLayoutElement*)0);
}

int CLayoutGrid::rowColToIndex(int row, int column)const
{
	if(row>=0&&row<rowCount())
	{
		if(column>=0&&column<columnCount())
		{
			switch(mFillOrder)
			{
				case foRowsFirst: return column*rowCount()+row;
				case foColumnsFirst: return row*columnCount()+column;
			}
		}		
	}
	return 0;
}

void CLayoutGrid::indexToRowCol(int index, int& row, int& column)const
{
	row = -1;
	column = -1;
	if(columnCount()==0||rowCount()==0)
		return ;
	if(index<0||index>=elementCount())
	{
		return ;
	}
	
	switch(mFillOrder)
	{
		case foRowsFirst:
		{
			column=index/rowCount();
			row=index%rowCount();
			break;
		}
		case foColumnsFirst:
		{
			row = index/columnCount();
			column = index%columnCount();
			break;
		}
	}
}


void CLayoutGrid::updateLayout()
{
	CVector<int> minColWidths, minRowHeights, maxColWidths, maxRowHeights;
	
	getMinimumRowColSizes(&minColWidths, &minRowHeights);
	getMaximumRowColSizes(&maxColWidths, &maxRowHeights);
	
	int totalRowSpacing = (rowCount()-1)*mRowSpacing;
	int totalColSpacing = (columnCount()-1)*mColumnSpacing;
	
	CVector<int> colWidths=getSectionSizes(maxColWidths, minColWidths, mColumnStretchFactors.toVector(),mRect.width()-totalColSpacing);
	CVector<int> rowHeights=getSectionSizes(maxRowHeights,minRowHeights, mRowStretchFactors.toVector(),mRect.height()-totalRowSpacing);
	
	int yOffset = mRect.top();
	for(int row=0; row<rowCount();++row)
	{
		if(row>0)
			yOffset+=rowHeights.at(row-1)+mRowSpacing;
		int xOffset = mRect.left();
		for(int col=0; col<columnCount();++col)
		{
			if(col>0)
				xOffset+=colWidths.at(col-1)+mColumnSpacing;
			if(mElements.at(row).at(col))
				mElements.at(row).at(col)->setOuterRect(CRect(xOffset,yOffset, colWidths.at(col),rowHeights.at(row)));
		}
	}
}

CLayoutElement* CLayoutGrid::elementAt(int index)const
{
	if(index>=0&&index<elementCount())
	{
		int row, col;
		indexToRowCol(index, row, col);
		return mElements.at(row).at(col);
	}
	return 0;
}


CLayoutElement* CLayoutGrid::takeAt(int index)
{
	if(CLayoutElement* el=elementAt(index))
	{
		releaseElement(el);
		int row,col;
		indexToRowCol(index, row, col);
		mElements[row][col]=0;
		return el;
	}
	else
	{
		return 0;
	}
}

bool CLayoutGrid::take(CLayoutElement* element)
{
	if(element)
	{
		for(int i=0; i<elementCount();++i)
		{
			if(elementAt(i)==element)
			{
				takeAt(i);
				return true;
			}
		}
	}
	return false;
}

CList<CLayoutElement*> CLayoutGrid::elements(bool recursive)const
{
	CList<CLayoutElement*> result;
	const int elCount = elementCount();
	result.reserve(elCount);
	for(int i=0; i<elCount; ++i)
		result.append(elementAt(i));
	if(recursive)
	{
		for(int i=0; i<elCount;++i)
		{
			if(result.at(i))
				result<<result.at(i)->elements(recursive);
		}
	}
	return result;
}



void CLayoutGrid::simplify()
{
	for(int row=rowCount()-1;row>=0;--row)
	{
		bool hasElements = false;
		for(int col=0;col<columnCount();++col)
		{
			if(mElements.at(row).at(col))
			{
				hasElements = true;
				break;
			}
		}
		if(!hasElements)
		{
			mRowStretchFactors.removeAt(row);
			mElements.removeAt(row);
			if(mElements.isEmpty())
				mColumnStretchFactors.clear();
		}
	}
	
	for(int col=columnCount()-1; col>=0;--col)
	{
		bool hasElements = false;
		for(int row =0; row<rowCount(); ++row)
		{
			if(mElements.at(row).at(col))
			{
				hasElements = true;
				break;
			}
		}
		if(!hasElements)
		{
			mColumnStretchFactors.removeAt(col);
			for(int row=0; row<rowCount(); ++row)
				mElements[row].removeAt(col);
		}
	}
}


CSize CLayoutGrid::minimumSizeHint()const
{
	CVector<int> minColWidths, minRowHeights;
	getMinimumRowColSizes(&minColWidths,&minRowHeights);
	CSize result(0,0);
	for(int i=0; i<minColWidths.size();++i)
		result.rWidth()+=minColWidths.at(i);
	for(int i=0;i<minRowHeights.size();++i)
		result.rHeight()+=minRowHeights.at(i);
	result.rWidth()+=mMax(0, columnCount()-1)*mColumnSpacing+mMargins.left()+mMargins.right();
	result.rHeight()+=mMax(0,rowCount()-1)*mRowSpacing+mMargins.top()+mMargins.bottom();
	return result;
}


CSize CLayoutGrid::maximumSizeHint()const
{
	CVector<int> maxColWidths, maxRowHeights;
	getMaximumRowColSizes(&maxColWidths,& maxRowHeights);
	CSize result(0,0);
	for(int i=0; i<maxColWidths.size();++i)
		result.setWidth(mMin(result.width()+maxColWidths.at(i),1024));
	for(int i=0; i<maxRowHeights.size();++i)
		result.setHeight(mMin(result.height()+maxRowHeights.at(i),1024));
	result.rWidth()+=mMax(0, columnCount()-1)*mColumnSpacing+mMargins.left()+mMargins.right();
	result.rHeight()+=mMax(0,rowCount()-1)*mRowSpacing+mMargins.top()+mMargins.bottom();
	return result;
}


void CLayoutGrid::getMinimumRowColSizes(CVector<int>* minColWidths, CVector<int>* minRowHeights)const
{
	*minColWidths=CVector<int>(columnCount(),0);
	*minRowHeights=CVector<int>(rowCount(),0);
	int size=(*minColWidths).size();
	for(int row=0; row<rowCount();++row)
	{
		for(int col=0;col<columnCount();++col)
		{
			if(mElements.at(row).at(col))
			{
				CSize minHint=mElements.at(row).at(col)->minimumSizeHint();
				CSize min=mElements.at(row).at(col)->minimumSize();
				CSize final(min.width()>0?min.width():minHint.width(),min.height()>0?min.height():minHint.height());
				if(minColWidths->at(col)<final.width())
					(*minColWidths)[col]=final.width();
				if(minRowHeights->at(row)<final.height())
					(*minRowHeights)[row]=final.height();
			}
		}
	}
}


void CLayoutGrid::getMaximumRowColSizes(CVector<int>* maxColWidths, CVector<int>* maxRowHeights)const
{
	*maxColWidths=CVector<int>(columnCount(),1024);
	*maxRowHeights=CVector<int>(rowCount(),1024);
	for(int row=0; row<rowCount();++row)
	{
		for(int col=0; col<columnCount();++col)
		{
			if(mElements.at(row).at(col))
			{
				CSize maxHint=mElements.at(row).at(col)->maximumSizeHint();
				CSize max=mElements.at(row).at(col)->maximumSize();
				CSize final(max.width()<1024?max.width():maxHint.width(),max.height()<1024?max.height():maxHint.height());
				if(maxColWidths->at(col)>final.width())
					(*maxColWidths)[col]=final.width();
				if(maxRowHeights->at(row)>final.height())
					(*maxRowHeights)[row]=final.height();
			}
		}
	}
}

CCrossRuler::CCrossRuler(CAxisRect* axisRect):
mAxisRect(axisRect),
mRulerPen(0xff6a6a6a),
mBrush(0x50acacac),
mBorderPen(0xff9a9a9a)
{
	mFont.width=16;
	mFont.height=16;
	mFont.color=Gemini::red;
	m_left=0;
	m_right=0;
	m_bottom=0;
	m_top=0;
}

CCrossRuler::~CCrossRuler()
{
	
}

void 	CCrossRuler::setAxisRect(CAxisRect* axisRect)
{
	if(axisRect)
		mAxisRect=axisRect;
}
	
std::string CCrossRuler::getTickLabel(CPlotAxis::AxisType type)const
{
	if(mAxisRect)
	{
		CPlotAxis* axis=mAxisRect->axis(type);
		if(!axis) return std::string();
		switch(type)
		{
			case CPlotAxis::atLeft:
				return axis->ticker()->getTickLabel(m_left, axis->numberFormat()[0], axis->numberPrecision());
			case CPlotAxis::atTop:
				return axis->ticker()->getTickLabel(m_top, axis->numberFormat()[0], axis->numberPrecision());
			case CPlotAxis::atRight:
				return axis->ticker()->getTickLabel(m_right, axis->numberFormat()[0], axis->numberPrecision());
			case CPlotAxis::atBottom:
				return axis->ticker()->getTickLabel(m_bottom, axis->numberFormat()[0], axis->numberPrecision());
		}
		
	}
}

CAxisPainterPrivate::TickLabelData CCrossRuler::getTickLabelData(CPlotAxis::AxisType type,const PlotFont& font, const std::string& text)const
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

void CCrossRuler::placeTickLabel(CPlotAxis::AxisType type,CPlotPainter* painter, double position, int distanceToAxis, const std::string& text, CSize* tickLabelsSize)
{
	if(!mAxisRect) return;
	bool ptInFingerArea=false;
	CAxisPainterPrivate::TickLabelData data=getTickLabelData(type, mFont, text);
	CRect boundsRect=CRect(CPoint(0,0),data.totalBounds.size());
	CRect rect=mAxisRect->rect();
	CRect fingerRect=CRect(CPoint(-24,-24),CSize(48,48));
	fingerRect.translate(m_pt);
	switch(type)
	{
		case CPlotAxis::atLeft:
			boundsRect.translate(rect.x()+2,m_pt.y()+2);
			if(boundsRect.bottom()>=rect.bottom())
			{
				boundsRect.translate(0,-data.totalBounds.size().height()-4);
			}
			if(boundsRect.intersects(fingerRect))
			{
				boundsRect.translate(fingerRect.right()-boundsRect.x(),0);
			}
			break;
		case CPlotAxis::atRight:
			boundsRect.translate(rect.right()-data.totalBounds.size().width()-2,m_pt.y()+2);
			if(boundsRect.bottom()>=rect.bottom())
			{
				boundsRect.translate(0,-data.totalBounds.size().height()-4);
			}
			if(boundsRect.intersects(fingerRect))
			{
				boundsRect.translate(-boundsRect.x()+fingerRect.x()-boundsRect.width(),0);
			}
			
			break;
		case CPlotAxis::atTop:
			boundsRect.translate(m_pt.x()+2,rect.y()+2);
			if(boundsRect.right()>=rect.right())
			{
				boundsRect.translate(-data.totalBounds.size().width()-4,0);
			}
			if(boundsRect.intersects(fingerRect))
			{
				boundsRect.translate(0,fingerRect.bottom()-boundsRect.y());
			}
			break;
		case CPlotAxis::atBottom:
			boundsRect.translate(m_pt.x()+2, rect.bottom()-data.totalBounds.size().height()-2);
			if(boundsRect.right()>=rect.right())
			{
				boundsRect.translate(-data.totalBounds.size().width()-4,0);
			}
			if(boundsRect.intersects(fingerRect))
			{
				boundsRect.translate(0,-boundsRect.y()+fingerRect.y()-boundsRect.height());
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

void CCrossRuler::drawTickLabel(CPlotPainter* painter, double x, double y, const CAxisPainterPrivate::TickLabelData& labelData)const
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

void CCrossRuler::setBorderPen(const CPen& pen)
{
	mBorderPen=pen;
}

void CCrossRuler::setBrush(const CBrush& brush)
{
	mBrush=brush;
}

void CCrossRuler::setFont(const PlotFont& font)
{
	mFont=font;
}

void CCrossRuler::setRulerPen(const CPen& pen)
{
	mRulerPen=pen;
}

void CCrossRuler::setPixelPoint(const CPoint& pt)
{
	if(mAxisRect)
	{
		m_left=mAxisRect->axis(CPlotAxis::atLeft)->pixelToCoord(pt.y());
		m_top=mAxisRect->axis(CPlotAxis::atTop)->pixelToCoord(pt.x());
		m_right=mAxisRect->axis(CPlotAxis::atRight)->pixelToCoord(pt.y());
		m_bottom=mAxisRect->axis(CPlotAxis::atBottom)->pixelToCoord(pt.x());
		m_pt=pt;
	}
}

void CCrossRuler::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aeOther);
}

void CCrossRuler::draw(CPlotPainter* painter)
{
	if(!mVisible)	return ;
	if(!mAxisRect)	return ;
	if(!painter)	return ;
	CRect rect=mAxisRect->rect();
	CPlotAxis::AxisType type[4]={
		CPlotAxis::atLeft,
		CPlotAxis::atTop,
		CPlotAxis::atRight,
		CPlotAxis::atBottom
	};

	if(rect.contains(m_pt))
	{
		painter->save();
		painter->setPen(mRulerPen);
		painter->drawLine(CPoint(rect.x()+1,m_pt.y()),CPoint(rect.right()-1,m_pt.y()));
		painter->drawLine(CPoint(m_pt.x(),rect.y()+1),CPoint(m_pt.x(),rect.bottom()-1));
		CRect focusRect=CRect(CPoint(-3,-3),CSize(6,6));
		focusRect.translate(m_pt);
		painter->drawRect(focusRect);
		painter->restore();
		for(int i=0; i<4; ++i)
		{
			std::string label=getTickLabel(type[i]);
			CSize labelSize;
			placeTickLabel(type[i],painter,0,0,label,&labelSize);
			
		}
	}
	
}



CLegend::CLegend()
{
	setFillOrder(CLayoutGrid::foRowsFirst);
	setWrap(0);
	setRowSpacing(3);
	setColumnSpacing(8);
	setMargins(CMargin(7,5,7,4));
	setAntialiased(false);
	setIconSize(32,18);
	setIconTextPadding(7);
	setSelectableParts(spLegendBox|spItems);
	setSelectedParts(spNone);
	setBorderPen(CPen(Gemini::black,0));
	setSelectedBorderPen(CPen(Gemini::blue,2));
	setIconBorderPen(CPen(Gemini::NoPen));
	setSelectedIconBorderPen(CPen(Gemini::blue,2));
	setBrush(CBrush(Gemini::white));
	setSelectedBrush(CBrush(Gemini::white));
	setTextColor(Gemini::black);
	setSelectedTextColor(Gemini::blue);
	mFont.width=16;
	mFont.height=16;
	mFont.color=Gemini::blue;
}


CLegend::~CLegend()
{
	clearItems();
}

WORD CLegend::selectedParts()const
{
	bool hasSelectedItems=false;
	for(int i=0; i<itemCount();++i)
	{
		if(item(i)&&item(i)->selected())
		{
			hasSelectedItems =true;
			break;
		}
	}
	if(hasSelectedItems)
		return mSelectedParts|spItems;
	else
		return mSelectedParts|~spItems;
	
}

void CLegend::setBrush(const CBrush& brush)
{
	mBrush = brush;
}

void CLegend::setBorderPen(const CPen& pen)
{
	mBorderPen=pen;
	std::cout<<mBorderPen.style()<<std::endl;
}

void CLegend::setFont(const PlotFont& font)
{
	mFont=font;
	for(int i=0; i<itemCount();++i)
	{
		if(item(i))
			item(i)->setFont(font);
	}
}



void CLegend::setTextColor(int color)
{
	mTextColor = color;
	for(int i=0; i<itemCount();++i)
	{
		if(item(i))
			item(i)->setTextColor(color);
	}
}

void CLegend::setIconSize(const CSize& size)
{
	mIconSize=size;
}

void CLegend::setIconSize(int width, int height)
{
	setIconSize(CSize(width, height));
}

void CLegend::setIconTextPadding(int padding)
{
	mIconTextPadding=padding;
}

void CLegend::setIconBorderPen(const CPen& pen)
{
	mIconBorderPen=pen;
}

void CLegend::setSelectableParts(WORD selectable)
{
	if(mSelectableParts!=selectable)
	{
		mSelectableParts=selectable;
		
	}
}

void CLegend::setSelectedParts(WORD selected)
{
	WORD newSelected  = selected;
	mSelectedParts=this->selectedParts();
	if(mSelectedParts!=newSelected)
	{
		if(((mSelectedParts&spItems)!=spItems)&&((newSelected&spItems)!=spItems))
		{
			newSelected&=~spItems;
		}
	}
	
	if(((mSelectableParts&spItems)==spItems)&&((newSelected&spItems)!=spItems))
	{
		for(int i=0; i<itemCount(); ++i)
		{
			if(item(i))
				item(i)->setSelected(false);
		}
	}
	mSelectedParts = newSelected;
	
}

void CLegend::setSelectedBorderPen(const CPen& pen)
{
	mSelectedBorderPen = pen;
}

void CLegend::setSelectedIconBorderPen(const CPen& pen)
{
	mSelectedIconBorderPen = pen;
}

void CLegend::setSelectedBrush(const CBrush& brush)
{
	mSelectedBrush=brush;
}

void CLegend::setSelectedFont(const PlotFont& font)
{
	mSelectedFont = font;
	
	for(int i=0; i<itemCount();++i)
	{
		if(item(i))
			item(i)->setSelectedFont(font);
	}
}


void CLegend::setSelectedTextColor(int color)
{
	mSelectedTextColor = color;
	for(int i=0; i<itemCount();++i)
	{
		if(item(i))
			item(i)->setSelectedTextColor(color);
	}
}

CAbstractLegendItem* CLegend::item(int index)const
{
	return reinterpret_cast<CAbstractLegendItem*>(elementAt(index));
}

CPlottableLegendItem* CLegend::itemWithPlottable(const CAbstractPlottable* plottable)const
{
	for(int i=0; i<itemCount();++i)
	{
		if(CPlottableLegendItem* pli=reinterpret_cast<CPlottableLegendItem*>(item(i)))
		{
			if(pli->plottable()==plottable)
				return pli;
		}
	}
	return 0;
}


int CLegend::itemCount()const
{
	return elementCount();
}

bool CLegend::hasItem(CAbstractLegendItem* item)const
{
	for(int i=0; i<itemCount();++i)
	{
		if(item==this->item(i))
			return true;
	}
	return false;
}

bool CLegend::hasItemWithPlottable(const CAbstractPlottable* plottable)const
{
	return itemWithPlottable(plottable);
}

bool  CLegend::addItem(CAbstractLegendItem* item)
{
	
	return addElement(item);
}

bool CLegend::removeItem(int index)
{
	if(CAbstractLegendItem* ali=item(index))
	{
		bool success=remove(ali);
		if(success)
			setFillOrder(fillOrder(),true);
		return success;
	}
	else
		return false;
}

bool CLegend::removeItem(CAbstractLegendItem* item)
{
	bool success = remove(item);
	if(success)
		setFillOrder(fillOrder(), true);
	return success;
}

void CLegend::clearItems()
{
	for (int i=itemCount()-1;i>=0; --i)
		removeItem(i);
}

CList<CAbstractLegendItem*> CLegend::selectedItems()const
{
	CList<CAbstractLegendItem*> result;
	for(int i=0; i<itemCount();++i)
	{
		if(CAbstractLegendItem* ali=item(i))
		{
			if(ali->selected())
				result.append(ali);
		}
	}
	return result;
}

void CLegend::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aeLegend);
}


CPen CLegend::getBorderPen()const
{
	return (mSelectedParts&spLegendBox)==spLegendBox?mSelectedBorderPen:mBorderPen;
}


CBrush CLegend::getBrush()const
{
	return (mSelectedParts&spLegendBox)==spLegendBox? mSelectedBrush:mBrush;
}

void CLegend::draw(CPlotPainter* painter)
{
	painter->setBrush(getBrush());
	painter->setPen(getBorderPen());
	painter->drawRect(mOuterRect);
}


CAbstractLegendItem::CAbstractLegendItem(CLegend* parent)
:mParentLegend(parent),
mFont(parent->font()),
mTextColor(parent->textColor()),
mSelectedFont(parent->selectedFont()),
mSelectedTextColor(parent->selectedTextColor()),
mSelectable(true),
mSelected(false)
{
	setLayer(std::string("legend"));
	setMargins(CMargin(0,0,0,0));
}

void CAbstractLegendItem::setFont(const PlotFont& font)
{
	mFont = font;
}


void CAbstractLegendItem::setSelectedFont(const PlotFont& font)
{
	mSelectedFont = font;
}



void CAbstractLegendItem::setSelectable(bool selectable)
{
	if(mSelectable!=selectable)
	{
		mSelectable = selectable;
	}
}

void CAbstractLegendItem::setSelected(bool selected)
{
	if(mSelected!=selected)
	{
		mSelected = selected;
	}
}
void CAbstractLegendItem::applyDefaultAntialiasingHint(CPlotPainter* painter)const
{
	applyAntialiasingHint(painter, mAntialiased, Plot::aeLengendItem);
}

CRect CAbstractLegendItem::clipRect()const
{
	return mOuterRect;
}


CPlottableLegendItem::CPlottableLegendItem(CLegend* parent, CAbstractPlottable*plottable):
CAbstractLegendItem(parent),
mPlottable(plottable)
{
	setAntialiased(false);
}

CPen CPlottableLegendItem::getIconBorderPen()const
{
	return mSelected?mParentLegend->selectedIconBorderPen():mParentLegend->iconBorderPen();
	
} 

int  CPlottableLegendItem::getTextColor()const
{
	return mSelected? mSelectedTextColor:mTextColor;
}

PlotFont CPlottableLegendItem::getFont()const
{
	return mSelected?mSelectedFont:mFont;
}

CSize CPlottableLegendItem::minimumSizeHint()const
{
	if(!mPlottable) return CSize();
	CSize result(0,0);
	CSize iconSize=mParentLegend->iconSize();
	//get textRect size
	CRect textRect=CRect(0,0,getFont().width*(mPlottable->name().size()+1)>>1,getFont().height);
	result.setWidth(iconSize.width()+mParentLegend->iconTextPadding()+textRect.width()+mMargins.left()+mMargins.right());
	result.setHeight(mMax(textRect.height(),iconSize.height())+mMargins.top()+mMargins.bottom());
	return result;
}

void CPlottableLegendItem::draw(CPlotPainter* painter)
{
	if(!mPlottable)	return ;
	CSizeF iconSize = mParentLegend->iconSize();
	//get textRect size;
	CRectF textRect=CRect(0,0,getFont().width*(mPlottable->name().size()+1)>>1,getFont().height);
	
	CRectF iconRect(mRect.topLeft(), iconSize);
	int textHeight=mMax(textRect.height(), iconSize.height());
	//draw text
	if(mPlottable->name().size()!=0)
	{
		if(g_pDrawText)
		{
			g_pDrawText->SetFontSize(getFont().width,getFont().height,0x01);
			g_pDrawText->SetFontColor(getFont().color);
			CRect interSect=CRect(CPoint(mRect.x()+iconSize.width()+mParentLegend->iconTextPadding(),mRect.y()),CSize(textRect.width(),textHeight))&mParentLegend->rect();
			g_pDrawText->DrawText(*(CImage*)(painter->device()),(unsigned char*)mPlottable->name().c_str(),interSect,mPlottable->name().size());
		}
	}
	painter->setPen(CPen(getTextColor()));
	painter->save();
	painter->setClipRect(iconRect,Gemini::IntersectClip);
	mPlottable->drawLegendIcon(painter, iconRect);
	painter->restore();
	
	if(getIconBorderPen().style()!=Gemini::NoPen)
	{
		painter->setPen(getIconBorderPen());
		painter->setBrush(Gemini::NoBrush);
		int halfPen=mCeil(painter->pen().widthF()*0.5)+1;
		painter->setClipRect(mOuterRect.adjusted(-halfPen, -halfPen,  halfPen, halfPen));
		painter->drawRect(iconRect);
	}	
}

CLayoutInset::CLayoutInset()
{
	
	
}

CLayoutInset::~CLayoutInset()
{
	clear();
}
		
WORD CLayoutInset::insetPlacement(int index)const
{
	if(elementAt(index))
	{
		return mInsetPlacement.at(index);
	}
	return ipFree;
}

WORD CLayoutInset::insetAlignment(int index)const
{
	if(elementAt(index))
	{
		return mInsetAlignment.at(index);
	}
	return 0;
}

CRectF CLayoutInset::insetRect(int index)const
{
	if(elementAt(index))
	{
		return mInsetRect.at(index);
	}
	return CRectF();
}
		
void CLayoutInset::setInsetPlacement(int index, WORD placement)
{
	if(elementAt(index))
	{
		mInsetPlacement[index]=placement;
	}
}

void CLayoutInset::setInsetAlignment(int index, WORD alignment)
{
	if(elementAt(index))
	{
		mInsetAlignment[index]=alignment;
	}
}

void CLayoutInset::setInsetRect(int index, const CRectF& rect)
{
	if(elementAt(index))
	{
		mInsetRect[index]=rect;
	}
}
		
void CLayoutInset::updateLayout()
{
	for(int i=0; i<mElements.size();++i)
	{
		CRect insetRect;
		CSize finalMaxSize, finalMinSize;
		CSize minSizeHint=mElements.at(i)->minimumSizeHint();
		CSize maxSizeHint=mElements.at(i)->maximumSizeHint();
		finalMinSize.setWidth(mElements.at(i)->minimumSize().width()>0?mElements.at(i)->minimumSize().width():minSizeHint.width());
		finalMinSize.setHeight(mElements.at(i)->minimumSize().height()>0?mElements.at(i)->minimumSize().height():minSizeHint.height());
		finalMaxSize.setWidth(mElements.at(i)->maximumSize().width()<1024?mElements.at(i)->maximumSize().width():maxSizeHint.width());
		finalMaxSize.setHeight(mElements.at(i)->maximumSize().height()<1024?mElements.at(i)->maximumSize().height():maxSizeHint.height());
		
		if(mInsetPlacement.at(i)==ipFree)
		{
			insetRect=CRect(rect().x()+rect().width()*mInsetRect.at(i).x(),
							rect().y()+rect().height()*mInsetRect.at(i).y(),
							rect().width()*mInsetRect.at(i).width(),
							rect().height()*mInsetRect.at(i).height());
			if(insetRect.size().width()<finalMinSize.width())
				insetRect.setWidth(finalMinSize.width());
			if(insetRect.size().height()<finalMinSize.height())
				insetRect.setHeight(finalMinSize.height());
			if(insetRect.size().width()>finalMaxSize.width())
				insetRect.setWidth(finalMaxSize.width());
			if(insetRect.size().height()>finalMaxSize.height())
				insetRect.setHeight(finalMaxSize.height());
		}
		else if(mInsetPlacement.at(i)==ipBorderAligned)
		{
			insetRect.setSize(finalMinSize);
			WORD al=mInsetAlignment.at(i);
			if((al&Gemini::AlignLeft)==Gemini::AlignLeft)
				insetRect.moveLeft(rect().x());
			else if((al&Gemini::AlignRight)==Gemini::AlignRight)
				insetRect.moveRight(rect().x()+rect().width());
			else
				insetRect.moveLeft(rect().x()+rect().width()*0.5-finalMinSize.width()*0.5);
			if((al&Gemini::AlignTop)==Gemini::AlignTop)
				insetRect.moveTop(rect().y());
			else if((al&Gemini::AlignBottom)==Gemini::AlignBottom)
				insetRect.moveBottom(rect().y()+rect().height());
			else
				insetRect.moveTop(rect().y()+rect().height()*0.5-finalMinSize.height()*0.5);
			
		}
		mElements.at(i)->setOuterRect(insetRect);
	}
}

int CLayoutInset::elementCount()const
{
	return mElements.size();
}

CLayoutElement* CLayoutInset::elementAt(int index)const
{
	if(index>=0&&index<mElements.size())
		return mElements.at(index);
	else
		return 0;
}

CLayoutElement* CLayoutInset::takeAt(int index)
{
	if(CLayoutElement* el=elementAt(index))
	{
		releaseElement(el);
		mElements.removeAt(index);
		mInsetPlacement.removeAt(index);
		mInsetAlignment.removeAt(index);
		mInsetRect.removeAt(index);
		return el;
	}
	return 0;
}

bool CLayoutInset::take(CLayoutElement* element)
{
	if(element)
	{
		for(int i=0; i<elementCount(); ++i)
		{
			if(elementAt(i)==element)
			{
				takeAt(i);
				return true;
			}
		}
		
	}
	return false;
}
		
void CLayoutInset::addElement(CLayoutElement* element, WORD alignment)
{
	if(element)
	{
		if(element->layout())
			element->layout()->take(element);
		mElements.append(element);
		mInsetPlacement.append(ipBorderAligned);
		mInsetAlignment.append(alignment);
		mInsetRect.append(CRectF(0.6,0.6,0.4,0.4));
		adoptElement(element);
	}
}

void CLayoutInset::addElement(CLayoutElement* element, const CRectF& rect)
{
	if(element)
	{
		if(element->layout())
			element->layout()->take(element);
		mElements.append(element);
		mInsetPlacement.append(ipFree);
		mInsetAlignment.append(Gemini::AlignRight|Gemini::AlignTop);
		mInsetRect.append(rect);
		adoptElement(element);
	}
}






















/*==============================================================================================*/

//AxisWraper
CPlotAxisWrapper::CPlotAxisWrapper(CPlotAxis* axis,const CtmPlot* parent)
{
	this->axis=axis;
	this->plot=parent;
}

CPlotAxisWrapper::CPlotAxisWrapper(const CPlotAxisWrapper& other)
{
	axis=other.axis;
	plot=other.plot;
}

CPlotAxisWrapper CPlotAxisWrapper::operator=(const CPlotAxisWrapper& other)
{
	axis=other.axis;
	plot=other.plot;
	return *this;
}


void CPlotAxisWrapper::setName(const std::string& name)
{
	if(axis)
	{
		if(name!=axis->label())
		{
			axis->setLabel(name);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

std::string CPlotAxisWrapper::name()const
{
	if(axis)
		return axis->label();
	return std::string();
}


CRange CPlotAxisWrapper::range()const
{
	if(axis)
		return axis->range();
	return CRange();
}

double CPlotAxisWrapper::rangeLower()const
{
	if(axis)
		return axis->range().lower;
	return 0;
}

double CPlotAxisWrapper::rangeUpper()const
{
	if(axis)
		return axis->range().upper;
	return 0;
}

int CPlotAxisWrapper::labelPadding()const
{
	if(axis)
		return axis->labelPadding();
	return 0;
}

int CPlotAxisWrapper::tickLabelPadding()const
{
	if(axis)
		return axis->tickLabelPadding();
	return 0;
}

bool CPlotAxisWrapper::rangeReversed()const
{
	if(axis)
		return axis->rangeReversed();
	return false;
}

int CPlotAxisWrapper::axisOffset()const
{
	if(axis)
		return axis->offset();
	return 0;
}

bool CPlotAxisWrapper::ticks()const
{
	if(axis)
		return axis->ticks();
	return false;
}
int CPlotAxisWrapper::tickCount()const
{
	if(axis)
		return axis->ticker()->tickCount();
	return 0;
}

void CPlotAxisWrapper::setAxisColor(int color)
{
	if(axis)
	{
		CPen pen=axis->basePen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				axis->setBasePen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.axisBase=1;
				}
			}
		}
	}
}

void CPlotAxisWrapper::setAxisPenStyle(Gemini::PenStyle style)
{
	if(axis)
	{
		CPen pen=axis->basePen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			axis->setBasePen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setAxisWidth(int width)
{
	if(axis)
	{
		CPen pen=axis->basePen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				axis->setBasePen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.axisBase=1;
				}
			}
		}
	}
}

void CPlotAxisWrapper::setTickerColor(int color)
{
	if(axis)
	{
		CPen pen=axis->tickPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				axis->setTickPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.axisBase=1;
				}
			}
		}
	}
}

void CPlotAxisWrapper::setSubTickerColor(int color)
{
	if(axis)
	{
		CPen pen=axis->subTickPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				axis->setSubTickPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.axisBase=1;
				}
			}
		}
	}
}

void CPlotAxisWrapper::setTickerWidth(int width)
{
	if(axis)
	{
		CPen pen=axis->tickPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				axis->setTickPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.axisBase=1;
				}
			}
		}
	}
}

void CPlotAxisWrapper::setSubTickerWidth(int width)
{
	if(axis)
	{
		CPen pen=axis->subTickPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				axis->setSubTickPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.axisBase=1;
				}
			}
		}
	}
}



void CPlotAxisWrapper::setTickerPenStyle(Gemini::PenStyle style)
{
	if(axis)
	{
		CPen pen=axis->tickPen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			axis->setTickPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setSubTickerPenStyle(Gemini::PenStyle style)
{
	if(axis)
	{
		CPen pen=axis->subTickPen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			axis->setSubTickPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setRange(double lower, double upper)
{
	if(axis)
	{
		if(CRange(lower, upper)!=axis->range())
		{
			axis->setRange(lower, upper);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setRange(const CRange& range)
{
	if(axis)
	{
		if(range!=axis->range())
		{
			axis->setRange(range);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setRangeLower(double lower)
{
	if(axis)
	{
		if(!fuzzyIsNull(axis->range().lower-lower))
		{
			axis->setRangeLower(lower);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setRangeUpper(double upper)
{
	if(axis)
	{
		if(!fuzzyIsNull(axis->range().upper-upper))
		{
			axis->setRangeUpper(upper);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::moveRange(double diff)
{
	if(axis)
	{
		if(!fuzzyIsNull(diff))
		{
			axis->moveRange(diff);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::scaleRange(double factor)
{
	if(axis)
	{
		if(!fuzzyIsNull(factor-1.0))
		{
			axis->scaleRange(factor);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::scaleRange(double factor, double center)
{
	//
}


void CPlotAxisWrapper::setRangeReversed(bool reversed)
{
	if(axis)
	{
		if(reversed!=axis->rangeReversed())
		{
			axis->setRangeReversed(reversed);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setLabelPadding(int padding)
{
	if(axis)
	{
		if(padding!=axis->labelPadding())
		{
			axis->setLabelPadding(padding);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickLabelPadding(int padding)
{
	if(axis)
	{
		if(padding!=axis->tickLabelPadding())
		{
			axis->setTickLabelPadding(padding);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setAxisOffset(int offset)
{
	if(axis)
	{
		if(offset!=axis->offset())
		{
			axis->setOffset(offset);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}



void CPlotAxisWrapper::setLabelFont(int width, int height, int color)
{
	if(axis)
	{
		PlotFont font=axis->labelFont();
		if(font.width!=width||font.height!=height||font.color!=color)
		{
			font.width=width;
			font.height=height;
			font.color=color;
			axis->setLabelFont(font);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickLabelFont(int width,int height, int color)
{
	if(axis)
	{
		PlotFont font=axis->tickLabelFont();
		if(font.width!=width||font.height!=height||font.color!=color)
		{
			font.width=width;
			font.height=height;
			font.color=color;
			axis->setTickLabelFont(font);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setNumberFormat(const std::string& format)
{
	if(axis)
	{
		if(format!=axis->numberFormat())
		{
			axis->setNumberFormat(format);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setNumberPrecision(int precision)
{
	if(axis)
	{
		if(precision!=axis->numberPrecision())
		{
			axis->setNumberPrecision(precision);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickLabelRotation(int angle)
{
	if(axis)
	{
		if(angle!=axis->tickLabelRotation())
		{
			axis->setTickLabelRotation(angle);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickStepStrategy(CAxisTicker::TickStepStrategy strategy)
{
	if(axis)
	{
		if(strategy!=axis->ticker()->tickStepStrategy())
		{
			axis->ticker()->setTickStepStrategy(strategy);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setScaleStyle(CPlotAxis::ScaleType type)
{
	if(axis)
	{
		if(type!=axis->scaleType())
		{
			axis->setScaleType(type);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickLengthIn(int inside)
{
	if(axis)
	{
		if(inside!=axis->tickLengthIn())
		{
			axis->setTickLengthIn(inside);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickLengthOut(int outside)
{
	if(axis)
	{
		if(outside!=axis->tickLengthOut())
		{
			axis->setTickLengthOut(outside);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickLength(int inside, int outside)
{
	if(axis)
	{
		if(inside!=axis->tickLengthIn()||outside!=axis->tickLengthOut())
		{
			axis->setTickLength(inside, outside);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setSubTickLengthIn(int inside)
{
	if(axis)
	{
		if(inside!=axis->subTickLengthIn())
		{
			axis->setSubTickLengthIn(inside);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
	
}

void CPlotAxisWrapper::setSubTickLengthOut(int outside)
{
	if(axis)
	{
		if(outside!=axis->subTickLengthOut())
		{
			axis->setSubTickLengthOut(outside);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setSubTickLength(int inside, int outside)
{
	if(axis)
	{
		if(inside!=axis->subTickLengthIn()||outside!=axis->subTickLengthOut())
		{
			axis->setSubTickLength(inside, outside);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setLowerEnding(CLineEnding::EndingStyle style)
{
	if(axis)
	{
		if(style!=axis->lowerEnding().style())
		{
			axis->lowerEnding().setStyle(style);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setUpperEnding(CLineEnding::EndingStyle style)
{
	if(axis)
	{
		if(style!=axis->upperEnding().style())
		{
			axis->upperEnding().setStyle(style);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setLowerEnding(CLineEnding::EndingStyle style, int width, int length, bool inverted)
{
	if(axis)
	{
		if(style!=axis->lowerEnding().style()||width!=axis->lowerEnding().width()
			||length!=axis->lowerEnding().length()||inverted!=axis->lowerEnding().inverted())
		{
			axis->lowerEnding().setStyle(style);
			axis->lowerEnding().setWidth(width);
			axis->lowerEnding().setLength(length);
			axis->lowerEnding().setInverted(inverted);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setUpperEnding(CLineEnding::EndingStyle style, int width, int length, bool inverted)
{
	if(axis)
	{
		if(style!=axis->upperEnding().style()||width!=axis->upperEnding().width()
			||length!=axis->upperEnding().length()||inverted!=axis->upperEnding().inverted())
		{
			axis->upperEnding().setStyle(style);
			axis->upperEnding().setStyle(style);
			axis->upperEnding().setWidth(width);
			axis->upperEnding().setLength(length);
			axis->upperEnding().setInverted(inverted);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTickCount(int count)
{
	if(axis)
	{
		if(count!=axis->ticker()->tickCount())
		{
			axis->ticker()->setTickCount(count);
			axis->setupTickVectors();
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setTicks(bool enable)
{
	if(axis)
	{
		if(enable!=axis->ticks())
		{
			axis->setTicks(enable);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}

void CPlotAxisWrapper::setVisible(bool visible)
{
	if(axis)
	{
		if(visible!=axis->visible())
		{
			axis->setVisible(visible);
			if(plot)
			{
				plot->d_func()->properties.changed.axisBase=1;
			}
		}
	}
}
bool CPlotAxisWrapper::visible()const
{
	if(axis)
	{
		return axis->visible();
	}
	return false;
}




//PlotGridWrapper
CPlotGridWrapper::CPlotGridWrapper(CPlotGrid* grid,const CtmPlot* parent)
{
	this->grid=grid;
	this->plot=parent;
}

CPlotGridWrapper::CPlotGridWrapper(const CPlotGridWrapper& other)
{
	grid=other.grid;
	plot=other.plot;
}

CPlotGridWrapper CPlotGridWrapper::operator=(const CPlotGridWrapper& other)
{
	grid=other.grid;
	plot=other.plot;
	return *this;
}

bool 	CPlotGridWrapper::visible()const
{
	if(grid)
		return grid->visible();
	return false;
}

void CPlotGridWrapper::setVisible(bool visible)
{
	if(grid)
	{
		if(grid->visible()!=visible)
		{
			grid->setVisible(visible);
			if(plot)
			{
				plot->d_func()->properties.changed.gridBase=1;
			}
		}
	}
}

void CPlotGridWrapper::setSubGridVisible(bool visible)
{
	if(grid)
	{
		if(grid->subGridVisible()!=visible)
		{
			grid->setSubGridVisible(visible);
			if(plot)
			{
				plot->d_func()->properties.changed.gridBase=1;
			}
		}
	}
}



void 	CPlotGridWrapper::setZeroLineColor(int color)
{
	if(grid)
	{
		CPen pen=grid->zeroLinePen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				grid->setZeroLinePen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.gridBase=1;
				}
			}
		}
	}
}

void	CPlotGridWrapper::setGridColor(int color)
{
	if(grid)
	{
		CPen pen=grid->pen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				grid->setPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.gridBase=1;
				}
			}
		}
	}
}

void 	CPlotGridWrapper::setSubGridColor(int color)
{
	if(grid)
	{
		CPen pen=grid->subGridPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				grid->setSubGridPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.gridBase=1;
				}
			}
		}
	}
}

void	CPlotGridWrapper::setZeroLineWidth(int width)
{
	if(grid)
	{
		CPen pen=grid->zeroLinePen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				grid->setZeroLinePen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.gridBase=1;
				}
			}
		}
	}
}

void	CPlotGridWrapper::setGridWidth(int width)
{
	if(grid)
	{
		CPen pen=grid->pen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				grid->setPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.gridBase=1;
				}
			}
		}
	}
}

void	CPlotGridWrapper::setSubGridWidth(int width)
{
	if(grid)
	{
		CPen pen=grid->subGridPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				grid->setSubGridPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.gridBase=1;
				}
			}
		}
	}
}

void	CPlotGridWrapper::setGridPenStyle(Gemini::PenStyle style)
{
	if(grid)
	{
		CPen pen=grid->pen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			grid->setPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.gridBase=1;
			}
		}
	}
}

void 	CPlotGridWrapper::setSubGridPenStyle(Gemini::PenStyle style)
{
	if(grid)
	{
		CPen pen=grid->subGridPen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			grid->setSubGridPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.gridBase=1;
			}
		}
	}
}

void 	CPlotGridWrapper::setZeroLinePenStyle(Gemini::PenStyle style)
{
	if(grid)
	{
		CPen pen=grid->zeroLinePen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			grid->setZeroLinePen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.gridBase=1;
			}
		}
	}
}

void 	CPlotGridWrapper::setRelationAxis(CPlotAxisWrapper wrapper)
{
	if(grid)
	{
		CPlotAxis* axis=grid->parent();
		CPlotAxis* relation=wrapper.axis;
		if(relation&&axis!=relation)
		{
			grid->setParent(relation);
			if(plot)
			{
				plot->d_func()->properties.changed.gridBase=1;
			}
		}
	}
}

void 	CPlotGridWrapper::setRelationAxis(CPlotAxis::AxisType type)
{
	if(grid)
	{
		CPlotAxis* axis=grid->parent();
		CPlotAxis* relation=0;
		if(plot)
		{
			relation=plot->d_func()->axisRect->axis(type);
			if(relation&&axis!=relation)
			{
				plot->d_func()->properties.changed.gridBase=1;
				grid->setParent(relation);
			}
		}
	}
}


//LegendWrapper
CLegendWrapper::CLegendWrapper(CLegend* legend, const CtmPlot* parent)
{
	this->legend=legend;
	this->plot=parent;
}

CLegendWrapper::CLegendWrapper(const CLegendWrapper& other)
{
	legend=other.legend;
	plot=other.plot;
}

CLegendWrapper CLegendWrapper::operator=(const CLegendWrapper& other)
{
	legend=other.legend;
	plot=other.plot;
	return *this;
}


bool CLegendWrapper::visible()const
{
	if(legend)
		return legend->visible();
	return false;
}

CRect CLegendWrapper::rect()const
{
	if(legend)
		return legend->rect();
	return CRect();
}

void	CLegendWrapper::setRect(const CRect& rect)
{
	if(legend)
	{
		if(legend->outerRect()!=rect)
		{
			legend->setOuterRect(rect);
			legend->updateLayout();
			if(plot)
			{
				plot->d_func()->properties.changed.legendBase=1;
			}
		}
	}
}

void  	CLegendWrapper::setVisible(bool visible)
{
	if(legend)
	{
		if(visible!=legend->visible())
		{
			legend->setVisible(visible);
			if(plot)
				plot->d_func()->properties.changed.legendBase=1;
		}
	}
}

void 	CLegendWrapper::setBorderColor(int color)
{
	if(legend)
	{
		CPen pen=legend->borderPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				legend->setBorderPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.legendBase=1;
				}
			}
		}
		
	}
}

void 	CLegendWrapper::setBorderPenWidth(int width)
{
	if(legend)
	{
		CPen pen=legend->borderPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				legend->setBorderPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.legendBase=1;
				}
			}
		}
	}
}

void 	CLegendWrapper::setBorderPenStyle(Gemini::PenStyle style)
{
	if(legend)
	{
		CPen pen=legend->borderPen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			legend->setBorderPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.legendBase=1;
			}
		}
	}
}

void	CLegendWrapper::setBorderFillColor(int color)
{
	if(legend)
	{
		CBrush brush=legend->brush();
		if(brush.style()!=Gemini::NoBrush)
		{
			brush.setColor(color);
			legend->setBrush(brush);
			if(plot)
			{
				plot->d_func()->properties.changed.legendBase=1;
			}
		}
	}
}

void 	CLegendWrapper::setBorderFillStyle(Gemini::BrushStyle style)
{
	if(legend)
	{
		CBrush brush=legend->brush();
		if(brush.style()!=style)
		{
			brush.setStyle(style);
			legend->setBrush(brush);
			if(plot)
			{
				plot->d_func()->properties.changed.legendBase=1;
			}
		}
	}
}

void 	CLegendWrapper::setFont(int width, int height, int color)
{
	if(legend)
	{
		PlotFont font=legend->font();
		if(font.width!=width||font.height!=height||font.color!=color)
		{
			font.width=width;
			font.height=height;
			font.color=color;
			legend->setFont(font);
			legend->updateLayout();
			if(plot)
			{
				plot->d_func()->properties.changed.legendBase=1;
			}
		}
	}
}

void 	CLegendWrapper::setFillOrder(CLayoutGrid::FillOrder order)
{
	if(legend)
	{
		if(order!=legend->fillOrder())
		{
			legend->setFillOrder(order);
			if(plot)
			{
				plot->d_func()->properties.changed.legendBase=1;
			}
		}
	}
}
		
//GraphWrapper

CGraphWrapper::CGraphWrapper(CGraph* graph,const CtmPlot* parent)
{
	this->graph=graph;
	this->plot=parent;
}

CGraphWrapper::CGraphWrapper(const CGraphWrapper& other)
{
	graph=other.graph;
	plot=other.plot;
}

CGraphWrapper CGraphWrapper::operator=(const CGraphWrapper& other)
{
	graph=other.graph;
	plot=other.plot;
	return *this;
}

std::string CGraphWrapper::name()const
{
	if(graph)
		return graph->name();
	return std::string();
}

void CGraphWrapper::setData(const CVector<double>& keys, const CVector<double>& values)
{
	if(graph)
	{
		graph->setData(keys, values);
		if(plot)
		{
			plot->d_func()->properties.changed.graphBase=1;
		}
	}
}

void CGraphWrapper::setPenColor(int color)
{
	if(graph)
	{
		CPen pen=graph->pen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				graph->setPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.graphBase=1;
				}
			}
		}
	}
}

void CGraphWrapper::setPenWidth(int width)
{
	if(graph)
	{
		CPen pen=graph->pen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				graph->setPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.graphBase=1;
				}
			}
		}
	}
}

void CGraphWrapper::setPenStyle(Gemini::PenStyle style)
{
	if(graph)
	{
		CPen pen=graph->pen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			graph->setPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}

void CGraphWrapper::setFillColor(int color)
{
	if(graph)
	{
		CBrush brush=graph->brush();
		if(brush.style()!=Gemini::NoBrush)
		{
			if(brush.color()!=color)
			{
				brush.setColor(color);
				graph->setBrush(brush);
				if(plot)
				{
					plot->d_func()->properties.changed.graphBase=1;
				}
			}
				
		}
	}
}

void CGraphWrapper::setFillStyle(Gemini::BrushStyle style)
{
	if(graph)
	{
		CBrush brush=graph->brush();
		if(brush.style()!=style)
		{
			brush.setStyle(style);
			graph->setBrush(brush);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}
	
void CGraphWrapper::setLineStyle(CGraph::LineStyle ls)
{
	if(graph)
	{
		if(ls!=graph->lineStyle())
		{
			graph->setLineStyle(ls);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}

void CGraphWrapper::setScatterStyle(const CScatterStyle& style)
{
	if(graph)
	{
		if(style!=graph->scatterStyle())
		{
			graph->setScatterStyle(style);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}

void CGraphWrapper::setScatterStyle(CScatterStyle::ScatterShape shape,int borderColor, int fillColor, int size)
{
	if(graph)
	{
		WORD selfShape=graph->scatterStyle().shape();
		if(selfShape!=CScatterStyle::ssPixmap&&selfShape!=CScatterStyle::ssCustom)//需要特殊处理
		{

			CScatterStyle style(shape,borderColor,fillColor,size);
			setScatterStyle(style);
		}
	}
}

void CGraphWrapper::setScatterStyle(const CImage& image)
{
	if(graph)
	{
		CScatterStyle style(image);
		if(graph->scatterStyle()!=style)
		{
			graph->setScatterStyle(style);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}

void CGraphWrapper::setScatterStyle(const CPainterPath& path, int borderColor, int fillColor, int size)
{
	if(graph)
	{
		CScatterStyle style(path, borderColor, fillColor, size);
		if(graph->scatterStyle()!=style)
		{
			graph->setScatterStyle(style);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}


void CGraphWrapper::addData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted)
{
	if(graph)
	{
		if(!keys.isEmpty()&&!values.isEmpty())
		{
			graph->addData(keys, values, alreadySorted);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}

void CGraphWrapper::addData(double key, double value)
{
	if(graph)
	{
		graph->addData(key, value);
		if(plot)
		{
			plot->d_func()->properties.changed.graphBase=1;
		}
	}
}

void CGraphWrapper::setAntialiased(bool enabled)
{
	if(graph)
	{
		if(enabled!=graph->antialiased())
		{
			graph->setAntialiased(enabled);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
		
	}
}

void CGraphWrapper::setVisible(bool visible)
{
	if(graph)
	{
		if(visible!=graph->visible())
		{
			graph->setVisible(visible);
			if(plot)
			{
				plot->d_func()->properties.changed.graphBase=1;
			}
		}
	}
}

bool CGraphWrapper::visible()const
{
	if(graph)
		return graph->visible();
	return false;
}


//BarsWrapper

CBarsWrapper::CBarsWrapper(CBars* bars,const CtmPlot* parent)
{
	this->bars=bars;
	this->plot=parent;
}

CBarsWrapper::CBarsWrapper(const CBarsWrapper& other)
{
	bars=other.bars;
	plot=other.plot;
}

CBarsWrapper CBarsWrapper::operator=(const CBarsWrapper& other)
{
	bars=other.bars;
	plot=other.plot;
	return *this;
}

std::string CBarsWrapper::name()const
{
	if(bars)
		return bars->name();
	return std::string();
}


void CBarsWrapper::setData(const CVector<double>& keys, const CVector<double>& values)
{
	if(bars)
	{
		bars->setData(keys, values);
		if(plot)
		{
			plot->d_func()->properties.changed.barsBase=1;
		}
	}
}

void CBarsWrapper::setPenColor(int color)
{
	if(bars)
	{
		CPen pen=bars->pen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.color()!=color)
			{
				pen.setColor(color);
				bars->setPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.barsBase=1;
				}
			}
		}
	}
}

void CBarsWrapper::setPenWidth(int width)
{
	if(bars)
	{
		CPen pen=bars->pen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(width!=pen.width())
			{
				pen.setWidth(width);
				bars->setPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.barsBase=1;
				}
			}
		}
	}
}

void CBarsWrapper::setPenStyle(Gemini::PenStyle style)
{
	if(bars)
	{
		CPen pen=bars->pen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			bars->setPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::setFillColor(int color)
{
	if(bars)
	{
		CBrush brush=bars->brush();
		if(brush.style()!=Gemini::NoBrush)
		{
			if(color!=brush.color())
			{
				brush.setColor(color);
				bars->setBrush(brush);
				if(plot)
				{
					plot->d_func()->properties.changed.barsBase=1;
				}
			}
		}
	}
}

void CBarsWrapper::setFillStyle(Gemini::BrushStyle style)
{
	if(bars)
	{
		CBrush brush=bars->brush();
		if(brush.style()!=style)
		{
			brush.setStyle(style);
			bars->setBrush(brush);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}


void CBarsWrapper::setWidth(double width)
{
	if(bars)
	{
		if(!fuzzyIsNull(width-bars->width()))
		{
			bars->setWidth(width);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::setWidthType(CBars::WidthType type)
{
	if(bars)
	{
		if(type!=bars->widthType())
		{
			bars->setWidthType(type);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::setBaseValue(double baseValue)
{
	if(bars)
	{
		if(!fuzzyIsNull(baseValue-bars->baseValue()))
		{
			bars->setBaseValue(baseValue);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
				
		}
	}
}

void CBarsWrapper::setStackingGap(double pixels)
{
	if(bars)
	{
		if(!fuzzyIsNull(pixels-bars->stackingGap()))
		{
			bars->setStackingGap(pixels);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::moveBelow(const CBarsWrapper& barsWrapper)
{
	if(bars&&barsWrapper.bars)
	{
		if(bars->barBelow()!=barsWrapper.bars)
		{
			bars->moveBelow(barsWrapper.bars);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::moveAbove(const CBarsWrapper& barsWrapper)
{
	if(bars&&barsWrapper.bars)
	{
		if(bars->barAbove()!=barsWrapper.bars)
		{
			bars->moveAbove(barsWrapper.bars);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::addData(const CVector<double>& keys,const CVector<double>& values, bool alreadySorted)
{
	if(bars)
	{
		if(!keys.isEmpty()&&!values.isEmpty())
		{
			bars->addData(keys, values, alreadySorted);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

void CBarsWrapper::addData(double key, double value)
{
	if(bars)
	{
		bars->addData(key, value);
		if(plot)
		{
			plot->d_func()->properties.changed.barsBase=1;
		}
	}
}

void CBarsWrapper::setVisible(bool visible)
{
	if(bars)
	{
		if(visible!=bars->visible())
		{
			bars->setVisible(visible);
			if(plot)
			{
				plot->d_func()->properties.changed.barsBase=1;
			}
		}
	}
}

bool CBarsWrapper::visible()const
{
	if(bars)
		return bars->visible();
	return false;
}

//BarsGroupWrapper

CBarsGroupWrapper::CBarsGroupWrapper(CBarsGroup* group,const CtmPlot* parent)
{
	this->group=group;
	this->plot=parent;
}

CBarsGroupWrapper::CBarsGroupWrapper(const CBarsGroupWrapper& other)
{
	group=other.group;
	plot=other.plot;
}

CBarsGroupWrapper CBarsGroupWrapper::operator=(const CBarsGroupWrapper& other)
{
	group=other.group;
	plot=other.plot;
	return *this;
}



CCrossRulerWrapper::CCrossRulerWrapper(CCrossRuler* ruler, const CtmPlot* parent)
{
	this->ruler=ruler;
	this->plot=parent;
}

CCrossRulerWrapper::CCrossRulerWrapper(const CCrossRulerWrapper& other)
{
	ruler=other.ruler;
	plot=other.plot;
}

CCrossRulerWrapper CCrossRulerWrapper::operator=(const CCrossRulerWrapper& other)
{
	ruler=other.ruler;
	plot=other.plot;
	return *this;
}
			
double CCrossRulerWrapper::left()const
{
	if(ruler)
	{
		return ruler->left();
	}
	return 0;
}

double CCrossRulerWrapper::right()const
{
	if(ruler)
	{
		return ruler->right();
	}
	return 0;
}

double CCrossRulerWrapper::top()const
{
	if(ruler)
	{
		return ruler->top();
	}
	return 0;
}

double CCrossRulerWrapper::bottom()const
{
	if(ruler)
	{
		return ruler->bottom();
	}
	return 0;
}

CPoint CCrossRulerWrapper::pixelPoint()const
{
	if(ruler)
	{
		return ruler->pixelPoint();
	}
	return CPoint();
}

bool 	CCrossRulerWrapper::visible()const
{
	if(ruler)
	{
		return ruler->visible();
	}
	return false;
}
			
//setters:
void CCrossRulerWrapper::setVisible(bool visible)
{
	if(ruler)
	{
		if(visible!=ruler->visible())
		{
			ruler->setVisible(visible);
			if(plot)
			{
				plot->d_func()->properties.changed.rulerBase=1;
			}
		}
	}
}

void CCrossRulerWrapper::setBorderColor(int color)
{
	if(ruler)
	{
		CPen pen=ruler->borderPen();
		
		if(pen.style()!=Gemini::NoPen)
		{
			if(color!=pen.color())
			{
				pen.setColor(color);
				ruler->setBorderPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.rulerBase=1;
				}
			}
		}
	}
}

void CCrossRulerWrapper::setBorderStyle(Gemini::PenStyle style)
{
	if(ruler)
	{
		CPen pen = ruler->borderPen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			ruler->setBorderPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.rulerBase=1;
			}
		}
	}
}

void CCrossRulerWrapper::setBorderPenWidth(int width)
{
	if(ruler)
	{
		CPen pen = ruler->borderPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(width!=pen.width())
			{
				pen.setWidth(width);
				ruler->setBorderPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.rulerBase=1;
				}
			}
		}
	}
}

void CCrossRulerWrapper::setFillColor(int color)
{
	if(ruler)
	{
		CBrush brush=ruler->brush();
		if(brush.style()!=Gemini::NoBrush)
		{
			if(color!=brush.color())
			{
				brush.setColor(color);
				ruler->setBrush(brush);
				if(plot)
				{
					plot->d_func()->properties.changed.rulerBase=1;
				}
			}
		}
	}
}

void CCrossRulerWrapper::setFillStyle(Gemini::BrushStyle style)
{
	if(ruler)
	{
		CBrush brush=ruler->brush();
		if(brush.style()!=style)
		{
			brush.setStyle(style);
			ruler->setBrush(brush);
			if(plot)
			{
				plot->d_func()->properties.changed.rulerBase=1;
			}
		}
	}
}

void CCrossRulerWrapper::setRulerColor(int color)
{
	if(ruler)
	{
		CPen pen=ruler->rulerPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(color!=pen.color())
			{
				pen.setColor(color);
				ruler->setRulerPen(pen);
				if(plot)
				{
					plot->d_func()->properties.changed.rulerBase=1;
				}
			}
		}
	}
}

void CCrossRulerWrapper::setRulerStyle(Gemini::PenStyle style)
{
	if(ruler)
	{
		CPen pen=ruler->rulerPen();
		if(pen.style()!=style)
		{
			pen.setStyle(style);
			ruler->setRulerPen(pen);
			if(plot)
			{
				plot->d_func()->properties.changed.rulerBase=1;
			}
		}
	}
}

void CCrossRulerWrapper::setRulerPenWidth(int width)
{
	if(ruler)
	{
		CPen pen = ruler->rulerPen();
		if(pen.style()!=Gemini::NoPen)
		{
			if(pen.width()!=width)
			{
				pen.setWidth(width);
				ruler->setRulerPen(pen);
				if(plot)
					plot->d_func()->properties.changed.rulerBase=1;
			}
		}
	}
}

void CCrossRulerWrapper::setPixelPoint(const CPoint& pixel)
{
	if(ruler)
	{
		if(pixel!=ruler->pixelPoint())
		{
			ruler->setPixelPoint(pixel);
			if(plot)
				plot->d_func()->properties.changed.rulerBase=1;
		}
	}
}

void CCrossRulerWrapper::setFont(int width, int height, int color)
{
	if(ruler)
	{
		PlotFont font=ruler->font();
		if(font.width!=width||font.height!=height||font.color!=color)
		{
			font.width=width;
			font.height=height;
			font.color=color;
			ruler->setFont(font);
			if(plot)
				plot->d_func()->properties.changed.rulerBase=1;
		}
	}
}
		



enum DirectionVisible{
	dvNone,
	dvLeft=0x01,
	dvRight=0x02,
	dvTop =0x04,
	dvBottom=0x08,
	dvAll=0x0F
};
CPlotPrivate::CPlotPrivate()
{
	init();
}

CPlotPrivate::~CPlotPrivate()
{
	destroy();
}

void CPlotPrivate::init()
{
	//plot:
		size=CSize(320,240);
		marginLeft=1;
		marginRight=1;
		marginTop=1;
		marginBottom=1;
		
		transparent=false;
		touched=false;
		
		properties.all=0;
		focus=false;
		x=0;
		y=0;

		axisRect=new CAxisRect;
		axisRect->setRect(CRect(CPoint(0,0),size)-CMargin(marginLeft, marginTop,marginRight, marginBottom));
		legend=new CLegend;

		horzGrid=new CPlotGrid(axisRect->axis(CPlotAxis::atLeft));
		vertGrid=new CPlotGrid(axisRect->axis(CPlotAxis::atBottom));
		
		barsGroup=new CBarsGroup;
		crossRuler=new CCrossRuler(axisRect);
		
		//axis:
		axisColor=Gemini::black;
		tickerColor=Gemini::black;
		subTickerColor=Gemini::black;
		
		axisPenWidth=1;
		tickerPenWidth=1;
		subTickerPenWidth=1;
		
		axisVisible=dvAll;	//left,right,top,bottom
		tickerVisible=dvAll; //left,right,top,bottom
		subtickerVisible=dvAll;//left,right,top,bottom
		leftTickerRangeUpper=10;
		leftTickerRangeLower=0;
		
		rightTickerRangeUpper=10;
		rightTickerRangeLower=0;
		
		topTickerRangeUpper=10;
		topTickerRangeLower=0;
		
		bottomTickerRangeUpper=10;
		bottomTickerRangeLower=0;
		
		leftLabelPadding=20;
		rightLabelPadding=20;
		topLabelPadding=20;
		bottomLabelPadding=20;
		
		leftTickLabelPadding=5;
		rightTickLabelPadding=5;
		topTickLabelPadding=5;
		bottomTickLabelPadding=5;
		
		//label-name 数据类型为字串类型的在plot.h中定义
		
		
		leftAxisOffset=0;
		rightAxisOffset=0;
		topAxisOffset=0;
		bottomAxisOffset=0;
		
		
		labelFont.width=16;
		labelFont.height=16;
		labelFont.color=Gemini::black;
		tickLabelFont.width=12;
		tickLabelFont.height=12;
		tickLabelFont.color=Gemini::black;
		
		//number-format数据类型为字串类型在plot.h中定义
		
		leftNumberPrecision=1;
		rightNumberPrecision=1;
		topNumberPrecision=0;
		bottomNumberPrecision=0;
		scaleType=0;
		
		lowerEndingStyle=CLineEnding::None;
		upperEndingStyle=CLineEnding::None;
		
		lefTickLabelRotation=0;
		rightTickLabelRotation=0;
		topTickLabelRotation=0;
		bottomTickLabelRotation=0;
		
		leftTickerLengIn=5;
		leftTickerLengout=0;
		rightTickerLengIn=5;
		rightTickerLengout=0;
		topTickerLengIn=5;
		topTickerLengout=0;
		bottomTickerLengIn=5;
		bottomTickerLengout=0;
		
		leftSubtickerLengthIn=3;
		leftSubtickerLengthOut=0;
		rightSubtickerLengthIn=3;
		rightSubtickerLengthOut=0;
		topSubtickerLengthIn=3;
		topSubtickerLengthOut=0;
		bottomSubtickerLengthIn=3;
		bttomSubtickerLengthOut=0;
		
		
		//legend:
		legendLeft=0;
		legendTop=0;
		legendWidth=160;
		legendHeight=80;
		
		legendFont.width=16;
		legendFont.height=16;
		legendFont.color=Gemini::black;
		
		legendBorderColor=Gemini::gray;
		legendBorderWidth=1;
		legendFillColor=0x60ffffff;
		oldBgc=0;
		
}

void CPlotPrivate::destroy()
{
		
		while(!plottable.isEmpty())
		{
			delete plottable.takeFirst(); 
		}
		
		delete legend;
		delete horzGrid;
		delete vertGrid;
		delete barsGroup;
		delete axisRect;
}




IMPLEMENT_DYNCREATE(CtmPlot, CtmWnd)



CtmPlot::CtmPlot()
{
	strcpy(TYPEName, "CtmPlot");
	d=new CPlotPrivate();
	leftAxisLabelName=NULL;
	rightAxisLabelName=NULL;
	topAxisLabelName=NULL;
	bottomAxisLabelName=NULL;
	
	marginLeft=40;
	marginRight=40;
	marginBottom=40;
	marginTop=40;
	transparent=false;
	
	if(d)
	{
		d->fmImage=CImage(d->size,CImage::Format_RGB32);
	}
}

CtmPlot::~CtmPlot()
{
	delete d;
}


void CtmPlot::Resize(CPlotPrivate* d)
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
		
		//更新CAxisRect图层
		if(d->axisRect)
		{
			
			d->axisRect->setRect(CRect(CPoint(0, 0),d->size)
						-CMargin(d->marginLeft,d->marginTop,d->marginRight, d->marginBottom));
			
		}
		
	}
}


CPlotAxisWrapper 	CtmPlot::GetAxis(CPlotAxis::AxisType type)const
{
	if(d)
	{
		return CPlotAxisWrapper(d->axisRect->axis(type),this);
	}
	return CPlotAxisWrapper(0);
}

CLegendWrapper 	CtmPlot::GetLegend()const
{
	if(d)
	{
		return CLegendWrapper(d->legend,this);
	}
	return CLegendWrapper(0);
}

CCrossRulerWrapper  CtmPlot::GetCrossRuler()const
{
	if(d)
	{
		return CCrossRulerWrapper(d->crossRuler,this);
	}
	return CCrossRulerWrapper(0);
}

CPlotGridWrapper 	CtmPlot::GetGrid(Gemini::Orientation type)const
{
	if(d)
	{
		if(type==Gemini::Horizontal)
		{
			return CPlotGridWrapper(d->horzGrid,this);
		}
		else if(type==Gemini::Vertical)
		{
			return CPlotGridWrapper(d->vertGrid,this);
		}
	}
	return CPlotGridWrapper(0);
}
		
CBarsWrapper 	 CtmPlot::GetBars(int index)const
{
	if(d)
	{
		int findIndex=0;
		int count=min(d->plottable.size(), d->plottableType.size());
		for(int i=0; i<count; ++i)
		{
			if(d->plottableType.at(i)==CPlotPrivate::ptGraph) //graph
				continue;
			else
			if(d->plottableType.at(i)==CPlotPrivate::ptBars) //bars
			{
				if(findIndex==index)// find it
					return CBarsWrapper(reinterpret_cast<CBars*>(d->plottable.at(i)),this);
				else
					findIndex++;
			}
		}
	}
	return CBarsWrapper(0);
}

CGraphWrapper	 CtmPlot::GetGraph(int index)const
{
	if(d)
	{
		int findIndex=0;
		int count=min(d->plottable.size(), d->plottableType.size());
		for(int i=0; i<count; ++i)
		{
			if(d->plottableType.at(i)==CPlotPrivate::ptGraph) //graph
			{
				if(findIndex==index)// find it
					return CGraphWrapper(reinterpret_cast<CGraph*>(d->plottable.at(i)),this);
				else
					findIndex++;
			}
			else
			if(d->plottableType.at(i)==CPlotPrivate::ptBars) //bars
			{
				continue;
				
			}
		}
	}
	return CGraphWrapper(0);
}

CBarsGroupWrapper	CtmPlot::GetBarsGroup()const
{
	if(d)
	{
		return CBarsGroupWrapper(d->barsGroup,this);
	}
	return CBarsGroupWrapper(0);
}
		
CBarsWrapper	CtmPlot::GetBars(const std::string& name)const
{
	if(d)
	{
		for(int i=0; i<d->plottable.size();++i)
		{
			if(d->plottable.at(i)->name()==name)
			{
				if(i<d->plottableType.size())
				{
					if(d->plottableType.at(i)==CPlotPrivate::ptBars)
						return CBarsWrapper(reinterpret_cast<CBars*>(d->plottable.at(i)),this);
				}
				return CBarsWrapper(0);
			}
		}
	}
	return CBarsWrapper(0);
}

CGraphWrapper  CtmPlot::GetGraph(const std::string& name)const
{
	if(d)
	{
		for(int i=0; i<d->plottable.size();++i)
		{
			if(d->plottable.at(i)->name()==name)
			{
				if(i<d->plottableType.size())
				{
					if(d->plottableType.at(i)==CPlotPrivate::ptGraph)
						return CGraphWrapper(reinterpret_cast<CGraph*>(d->plottable.at(i)),this);
				}
				return CGraphWrapper(0);
			}
		}
	}
	return CGraphWrapper(0);
}
		
void 	CtmPlot::AddBars(const std::string& name, CPlotAxis::AxisType keyType, CPlotAxis::AxisType valueType)
{
	if(d)
	{
		CBars* bars=GetBars(name).bars;
		if(bars)
		{
			bars->setKeyAxis(d->axisRect->axis(keyType));
			bars->setValueAxis(d->axisRect->axis(valueType));
			return ;
		}
		bars=new CBars(d->axisRect->axis(keyType), d->axisRect->axis(valueType));
		if(bars)
		{
			bars->setName(name);
			d->plottable.append(bars);
			d->plottableType.append(CPlotPrivate::ptBars);
			if(!d->legend->hasItemWithPlottable(bars))
				d->legend->addItem(new CPlottableLegendItem(d->legend,bars));
			if(d->barsGroup)
			{
				bars->setBarsGroup(d->barsGroup);
			}
		}
	}
}

void	CtmPlot::AddGraph(const std::string& name, CPlotAxis::AxisType keyType, CPlotAxis::AxisType valueType)
{
		CGraph* graph=GetGraph(name).graph;
		if(graph)
		{
			graph->setKeyAxis(d->axisRect->axis(keyType));
			graph->setValueAxis(d->axisRect->axis(valueType));
			return ;
		}
		graph=new CGraph(d->axisRect->axis(keyType), d->axisRect->axis(valueType));
		if(graph)
		{
			graph->setName(name);
			d->plottable.append(graph);
			d->plottableType.append(CPlotPrivate::ptGraph);
			if(!d->legend->hasItemWithPlottable(graph))
				d->legend->addItem(new CPlottableLegendItem(d->legend,graph));
		}
}
		
bool 	CtmPlot::RemoveBars(CBarsWrapper& bars)const
{
		
	bool result=RemoveBars(bars.name());
	bars.detach();
	return result;
}

bool 	CtmPlot::RemoveGraph(CGraphWrapper& graph)const
{
		
	bool result=RemoveGraph(graph.name());
	graph.detach();
	return result;
}

bool 	CtmPlot::RemoveBars(const std::string& name)const
{
	if(d)
	{
		for(int i=0; i<d->plottable.size();++i)
		{
			if(name==d->plottable.at(i)->name())
			{
				if(i<d->plottableType.size()&&(d->plottableType.at(i)==CPlotPrivate::ptBars))
				{
					d->legend->removeItem(d->legend->itemWithPlottable(d->plottable.at(i)));
					d->plottableType.removeAt(i);
					delete d->plottable.takeAt(i);
					return true;
				}
				
				return false;
			}

		}
	}
	return false;
}

bool 	CtmPlot::RemoveGraph(const std::string& name)const
{
	if(d)
	{
		for(int i=0; i<d->plottable.size();++i)
		{
			if(name==d->plottable.at(i)->name())
			{
				if(i<d->plottableType.size()&&(d->plottableType.at(i)==CPlotPrivate::ptGraph))
				{
					d->legend->removeItem(d->legend->itemWithPlottable(d->plottable.at(i)));
					d->plottableType.removeAt(i);
					delete d->plottable.takeAt(i);
					return true;
				}
				return false;
			}

		}
	}
	return false;
}

 static inline void prepareBase(CPlotPrivate* d)
{
	if(d)
	{
		d->axisRect->axis(CPlotAxis::atLeft)->setRange(d->leftTickerRangeLower,d->leftTickerRangeUpper);
		d->axisRect->axis(CPlotAxis::atTop)->setRange(d->topTickerRangeLower,d->topTickerRangeUpper);
		d->axisRect->axis(CPlotAxis::atRight)->setRange(d->rightTickerRangeLower,d->rightTickerRangeUpper);
		d->axisRect->axis(CPlotAxis::atBottom)->setRange(d->bottomTickerRangeLower,d->bottomTickerRangeUpper);
		d->axisRect->axis(CPlotAxis::atLeft)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atTop)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atRight)->setupTickVectors();
		d->axisRect->axis(CPlotAxis::atBottom)->setupTickVectors();
		d->legend->setBorderPen(CPen(d->legendBorderColor,d->legendBorderWidth));
		d->legend->setBrush(CBrush(d->legendFillColor));
		d->legend->setFont(d->legendFont);
	}
}

static inline void drawBase(CPlotPrivate* d)
{
	if(d)
	{
		CPlotPainter painter(&d->fmImage);
		if(d->horzGrid->visible())
			d->horzGrid->draw(&painter);
		if(d->vertGrid->visible())
			d->vertGrid->draw(&painter);
		if(d->axisRect->axis(CPlotAxis::atLeft)->visible())
			d->axisRect->axis(CPlotAxis::atLeft)->draw(&painter);
		if(d->axisRect->axis(CPlotAxis::atTop)->visible())
			d->axisRect->axis(CPlotAxis::atTop)->draw(&painter);
		if(d->axisRect->axis(CPlotAxis::atRight)->visible())
			d->axisRect->axis(CPlotAxis::atRight)->draw(&painter);
		if(d->axisRect->axis(CPlotAxis::atBottom)->visible())
			d->axisRect->axis(CPlotAxis::atBottom)->draw(&painter);
	}
}


static inline void drawLegend(CPlotPrivate* d)
{
	if(d)
	{
		//setLegendPropertys
		if(d->legend->visible())
		{

			CPlotPainter painter(&d->image);
			d->legend->draw(&painter);
			for(int i=0; i<d->legend->itemCount();++i)
			{
				d->legend->item(i)->draw(&painter);
			}
		}
	}
}

static inline void drawPlot(CPlotPrivate* d)
{
	if(d)
	{
		CPlotPainter painter(&d->image);
		painter.setClipRect(d->axisRect->rect());
		for(int i=0; i<d->plottable.size(); ++i)
		{
			if(d->plottable.at(i)->visible())
				d->plottable.at(i)->draw(&painter);
		}
	}
}

static inline void drawCrossRuler(CPlotPrivate* d)
{
	if(d)
	{
		CPlotPainter painter(&d->crossImage);
		d->crossRuler->draw(&painter);
	}
}

void   CtmPlot::CheckProperties()
{
	if(d)
	{
		//设置边距
		SetMargin(CMargin(marginLeft, marginTop, marginRight, marginBottom));
		if(d->properties.changed.margin)
		{
			if(d->axisRect)
			{
				d->axisRect->setRect(CRect(CPoint(0,0),d->size)-CMargin(d->marginLeft,d->marginTop, d->marginRight, d->marginBottom));
			}
		}
	}
	
	
}



void CtmPlot::ClearPropertiesBit()
{
	//清除所有属性检查位
	if(d)
	{
		d->properties.all=0;
	}
}

bool	CtmPlot::IsNeedResize(CPlotPrivate* d)
{
	if(d)
	{
		return (CRect(CPoint(m_rectClient.left,m_rectClient.top),CSize(GetClientWidth(),GetClientHeight()))
			!=CRect(CPoint(d->x,d->y),d->size));
	}
	return false;
}

bool 	CtmPlot::IsTransParentChanged(CPlotPrivate* d)
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

static void copyBackground(CPlotPrivate* d, uint bgc)
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

void    CtmPlot::Show()
{
	if(d)
	{
		CheckProperties();
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
			if(m_bgc!=d->oldBgc)
			{
				d->oldBgc=m_bgc;
				d->fmImage.fill((uint)m_bgc); 
				d->properties.all=0x00FF;
			}
			if(d->properties.all!=0)
			{
				//设置图列层矩形框大小
				if((d->properties.all&0x07))
				{
					copyBackground(d, m_bgc);
					drawBase(d);
				}
				if((d->properties.all&0x3F))
				{
					Clear();
					drawPlot(d);
					SetLegendRect(CRect(CPoint(legendLeft, legendTop),CSize(legendWidth, legendHeight)));
					drawLegend(d);
				}
				
				//十字卡尺绘制
				//d->properties.changed.rulerBase=1;
				if(d->properties.changed.rulerBase)
				{
					d->crossImage=d->image.copy(CRect(CPoint(0,0),d->size));
					drawCrossRuler(d); 
					_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
					_PutImage(0,0,d->crossImage);
					
				}
				else
				{
					_SetViewPort(m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom);
					_PutImage(0,0,d->image);
					
				}
				ClearPropertiesBit();
			}
		}
	}
}

void    CtmPlot::Update()
{
	Show();
}

void    CtmPlot::Clear()
{
	if(d)
	{
		
		d->image=d->fmImage.copy(CRect(CPoint(0,0),d->size));
	}
}

BOOL 	CtmPlot::CreateA()
{

	m_wnd.wStyle=0x27;
	CtmWnd::CreateA();
	prepareBase(d);
	return true;
}

void 	CtmPlot::SetPropertys()
{
	CtmWnd::SetPropertys();
	if(d)
	{
		//plot:
		SetPropInfo((char*)"margin-left",tmINT, &marginLeft);
		SetPropInfo((char*)"margin-right",tmINT, &marginRight);
		SetPropInfo((char*)"margin-top",tmINT, &marginTop);
		SetPropInfo((char*)"margin-bottom",tmINT,&marginBottom);
		SetPropInfo((char*)"transparent", tmBOOL, &transparent);
		SetPropInfo((char*)"touched",	tmBOOL, &d->touched);
		
		//axis:
		
		
		//legend:
		SetPropInfo((char*)"legend-left", tmINT, &legendLeft);
		SetPropInfo((char*)"legend-top", tmINT, &legendTop);
		SetPropInfo((char*)"legend-width", tmINT, &legendWidth);
		SetPropInfo((char*)"legend-height", tmINT, &legendHeight);
		SetPropInfo((char*)"legend-font-width",tmINT, &d->legendFont.width);
		SetPropInfo((char*)"legend-font-height",tmINT,&d->legendFont.height);
		SetPropInfo((char*)"legend-font-color",tmINT,&d->legendFont.color);
		
	}
}

static inline bool TouchAreaChanged(CPlotPrivate* d,const CPoint& control)
{
	bool res=false;
	if(d)
	{
		if(d->axisRect->rect().contains(control))
		{
			d->crossRuler->setPixelPoint(control);
			d->properties.changed.rulerBase=1;
		}
		else
		{
			d->crossRuler->setPixelPoint(CPoint(-1,-1));
			d->properties.changed.rulerBase=1;
			
		}
		
	}
	return res;	
}

WORD    CtmPlot::OnKey(WORD wKey)
{
	WORD			wResult=wKey;
	if(!d)			return wKey;
	if(!d->focus)	return wKey;
	CPoint pt=d->crossRuler->pixelPoint();
	if(!d->axisRect->rect().contains(pt))
	{
		d->crossRuler->setPixelPoint(d->axisRect->rect().center());
		pt=d->crossRuler->pixelPoint();
	}
	
	
	switch (wKey)
	{
		case 0x6D00: 	//down
		case _KEY_DOWNFIELD:
		d->crossRuler->setPixelPoint(pt+CPoint(0,1));
		d->properties.changed.rulerBase=1;
		wResult=_NULL_KEY;
		break;
		
		case 0x6800:	//up
		case _KEY_UPFIELD:
			d->crossRuler->setPixelPoint(pt+CPoint(0,-1));
			d->properties.changed.rulerBase=1;
			wResult=_NULL_KEY;
			break;
		case _KEY_LEFTFIELD:
			d->crossRuler->setPixelPoint(pt+CPoint(-1,0));
			d->properties.changed.rulerBase=1;
			wResult=_NULL_KEY;
			break;
		case _KEY_RIGHTFIELD:
			d->crossRuler->setPixelPoint(pt+CPoint(1,0));
			d->properties.changed.rulerBase=1;
			wResult=_NULL_KEY;
			break;
		default:
			break;	
	}
	Update();
	return wResult;		
}
		
void   	CtmPlot::OnGetFocus()
{
	if(d)
	{
		d->focus = true;
		CtmWnd::DrawGetFocus();
	}
}

void    CtmPlot::OnLoseFocus()
{
	if(d)
	{
		d->focus=false;
		CtmWnd::DrawLoseFocus();
	}
}

		
void	CtmPlot::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	if(!d->touched) return ;
	int x=wParam>>16;
	int y=wParam&0x0000FFFF;
	switch(message)
	{
		case MSG_MOUSELUP:
			{
				;
			}
			break;
		case MSG_MOUSELDOWN:
			{
				TouchAreaChanged(d,CPoint(x,y));
				Update();
			}
			break;
		default:
			break;
	}
}




void 	CtmPlot::SetMarginLeft(int left)
{
	if(d)
	{
		if(d->marginLeft!=left)
		{
			d->marginLeft=left;
			d->properties.changed.margin=1;
		}
	}
}

void 	CtmPlot::SetMarginRight(int right)
{
	if(d)
	{
		if(d->marginRight!=right)
		{
			d->marginRight=right;
			d->properties.changed.margin=1;
		}
	}
}

void 	CtmPlot::SetMarginTop(int top)
{
	if(d)
	{
		if(d->marginTop!=top)
		{
			d->marginTop=top;
			d->properties.changed.margin=1;
		}
	}
}
void 	CtmPlot::SetMarginBottom(int bottom)
{
	if(d)
	{
		if(d->marginBottom!=bottom)
		{
			d->marginBottom=bottom;
			d->properties.changed.margin=1;
		}
	}
}

void 	CtmPlot::SetMargin(const CMargin& margin)
{
	if(d)
	{
		if(CMargin(d->marginLeft, d->marginTop, d->marginRight, d->marginBottom)!=margin)
		{
			d->marginLeft=margin.left();
			d->marginRight=margin.right();
			d->marginBottom=margin.bottom();
			d->marginTop=margin.top();
			d->properties.changed.margin=1;
		}
	}
}
		
int 	CtmPlot::GetMarginLeft()const
{
	if(d)
	{
		return d->marginLeft;
	}
	return 0;
}

int 	CtmPlot::GetMarginRight()const
{
	if(d)
	{
		return d->marginRight;
	}
	return 0;
}

int 	CtmPlot::GetMarginTop()const
{
	if(d)
	{
		return d->marginTop;
	}
	return 0;
}

int 	CtmPlot::GetMarginBottom()const
{
	if(d)
	{
		return d->marginBottom;
	}
	return 0;
}

CMargin CtmPlot::GetMargin()const
{
	if(d)
	{
		return CMargin(d->marginLeft, d->marginTop, d->marginRight, d->marginBottom);
	}
	return CMargin();
}


void 	CtmPlot::SetLegendLeft(int left)
{
	if(d)
	{
		if(left!=d->legendLeft)
		{
			d->properties.changed.legendBase=1;
			d->legendLeft=left;
			CRect legendRect=CRect(CPoint(d->legendLeft+d->x, d->legendTop+d->y),CSize(d->legendWidth,d->legendHeight))&CRect(CPoint(0,0),d->size);
			CLegendWrapper(d->legend).setRect(legendRect);
		}	
	}
	
}

void 	CtmPlot::SetLegendTop(int top)
{
	if(d)
	{
		if(top!=d->legendTop)
		{
			d->properties.changed.legendBase=1;
			d->legendTop=top;
			CRect legendRect=CRect(CPoint(d->legendLeft+d->x, d->legendTop+d->y),CSize(d->legendWidth,d->legendHeight))&CRect(CPoint(0,0),d->size);
			CLegendWrapper(d->legend).setRect(legendRect);
		}
	}
}

void 	CtmPlot::SetLegendWidth(int width)
{
	if(d)
	{
		if(width!=d->legendWidth)
		{
			d->properties.changed.legendBase=1;
			d->legendWidth=width;
			CRect legendRect=CRect(CPoint(d->legendLeft+d->x, d->legendTop+d->y),CSize(d->legendWidth,d->legendHeight))&CRect(CPoint(0,0),d->size);
			CLegendWrapper(d->legend).setRect(legendRect);
		}
	}
}

void 	CtmPlot::SetLegendHeight(int height)
{
	if(d)
	{
		if(height!=d->legendHeight)
		{
			d->properties.changed.legendBase=1;
			d->legendHeight=height;
			CRect legendRect=CRect(CPoint(d->legendLeft+d->x, d->legendTop+d->y),CSize(d->legendWidth,d->legendHeight))&CRect(CPoint(0,0),d->size);
			CLegendWrapper(d->legend).setRect(legendRect);
		}
	}
}

void CtmPlot::SetLegendRect(const CRect& rect)
{
	if(d)
	{
		if(rect!=CRect(CPoint(d->legendLeft, d->legendTop),CSize(d->legendWidth, d->legendHeight)));
		{
			d->properties.changed.legendBase=1;
			d->legendTop=rect.top();
			d->legendLeft=rect.left();
			d->legendWidth=rect.width();
			d->legendHeight=rect.height();
			CRect legendRect=rect&CRect(CPoint(0,0),d->size);
			CLegendWrapper(d->legend).setRect(legendRect);
		}
	}
}
		
int 	CtmPlot::GetLegendLeft()const
{
	if(d)
	{
		return d->legendLeft;
	}
	return 0;
}

int 	CtmPlot::GetLegendTop()const
{
	if(d)
	{
		return d->legendTop;
	}
	return 0;
}

int 	CtmPlot::GetLegendWidth()const
{
	if(d)
	{
		return d->legendHeight;
	}
	return 0;
}

int 	CtmPlot::GetLegendHeight()const
{
	if(d)
	{
		return d->legendWidth;
	}
	return 0;
}





