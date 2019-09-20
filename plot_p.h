/*Created by J.Wong 2017/03/13*/
#pragma once
#include <guilite/IGui>
#include <guilite/IUtils>
#include <guilite/IShare>
#include <iostream>
#include <map>
#include <stack>
#include "common.h"


 namespace Plot{
	typedef unsigned short WORD;
	enum AntialiasedElement{
		aeAxes =0x0001,
		aeGrid =0x0002,
		aeSubGrid =0x0004,
		aeLegend =0x0008,
		aeLengendItem = 0x0010,
		aePlottables = 0x0020,
		aeItems = 0x0040,
		aeScatters =0x0080,
		aeFills = 0x0100,
		aeZeroLine =0x0200,
		aeOther =0x8000,
		aeAll =0xFFFF,
		aeNone =0
	};
	enum SignDomain{
		sdNegative,
		sdBoth,
		sdPositive
	};
	
	enum SelectionType{
		stNone,
		stWhole,
		stSingleData,
		stDataRange,
		stMultipleDataRanges
	};
	
	enum MarginSide{
		msLeft=0x01,
		msRight=0x02,
		msTop=0x04,
		msBottom=0x08,
		msAll =0xff,
		msNone = 0x00
	};
	
	inline bool isInvalidData(double value)
	{
		return isnan(value)||isinf(value);
	}
	
	inline bool isInvalidData(double value1, double value2)
	{
		return (isInvalidData(value1)||isInvalidData(value2));
	}
	
	inline void setMarginValue(CMargin& margins, WORD side, int value)
	{
		switch(side)
		{
			case Plot::msLeft: margins.setLeft(value); break;
			case Plot::msRight: margins.setRight(value); break;
			case Plot::msTop: margins.setTop(value); break;
			case Plot::msBottom:margins.setBottom(value); break;
			default: break;
		}
	}
	inline int getMarginValue(const CMargin& margins, WORD side)
	{
		switch(side)
		{
			case Plot::msLeft: return margins.left();
			case Plot::msRight: return margins.right();
			case Plot::msTop: return margins.top();
			case Plot::msBottom: return margins.bottom();
			default: break;
		}
		return 0;
	}
}








template <class T>
class CPointer
{
	CWeakPointer<T> wp;
	public:
	inline CPointer();
	inline CPointer(T* p):wp(p,true){}
	inline void swap(CPointer& other) {wp.swap(other.swap);}
	inline CPointer<T>& operator=(T* p)
	{
		wp.assign(p);
		return this;
	}
	inline T* data()const
	{
		return wp.data();
	}
	inline T* operator->()const
	{
		return data();
	}
	inline T& operator*()const
	{
		return *data();
	}
	inline operator T*()const
	{
		return data();
	}
	inline bool isNull()const
	{
		return wp.isNull();
	}
	inline void clear()
	{
		wp.clear();
	}
};

template<class T>
inline bool operator==(const T* o, const CPointer<T>& p)
{
	return o==p.operator->();
}
template<class T>
inline bool operator==(const CPointer<T>& p, const T* o)
{
	return p.operator->()==o;
}
template<class T>
inline bool operator==(T* o, const CPointer<T>& p)
{
	return o==p.operator->();
}
template<class T>
inline bool operator==(const CPointer<T>& p ,T* o)
{
	return p.operator->()==o;
}
template<class T>
inline bool operator==(const CPointer<T>& p1, const CPointer<T>& p2)
{
	return p1.operator->()==p2.operator->();
}

template<class T>
inline bool operator!=(const T* o, const CPointer<T>& p)
{
	return o!=p.operator->();
}
template<class T>
inline bool operator!=(const CPointer<T>& p, const T* o)
{
	return p.operator->()!=o;
}
template<class T>
inline bool operator!=(T* o, const CPointer<T>& p)
{
	return o!=p.operator->();
}
template<class T>
inline bool operator!=(const CPointer<T>& p ,T* o)
{
	return p.operator->()!=o;
}
template<class T>
inline bool operator!=(const CPointer<T>& p1, const CPointer<T>& p2)
{
	return p1.operator->()!=p2.operator->();
}



class CRange
{
	public:
		double lower, upper;
		CRange();
		CRange(double lower, double upper);
		bool operator==(const CRange& other) const
		{
			return lower==other.lower&&upper==other.upper;
		}
		bool operator!=(const CRange& other)const
		{
			return !(*this==other);
		}
		
		CRange& operator+=(const double& value)
		{
			lower+=value;
			upper+=value;
			return *this;
		}
		CRange& operator-=(const double& value)
		{
			lower-=value;
			upper-=value;
			return *this;
		}
		CRange& operator*=(const double& value)
		{
			lower*=value;
			upper*=value;
			return *this;
		}
		CRange& operator/=(const double& value)
		{
			lower/=value;
			upper/=value;
			return *this;
		}
		
		friend inline const CRange operator+(const CRange&, double);
		friend inline const CRange operator-(double, const CRange&);
		friend inline const CRange operator-(const CRange&, double);
		friend inline const CRange operator*(const CRange&, double);
		friend inline const CRange operator*(double, const CRange&);
		friend inline const CRange operator/(const CRange& , double);
		
		double size()const
		{
			return upper-lower;
		}
		double center()const
		{
			return (upper+lower)*0.5;
		}
		void normalize()
		{
			if(lower>upper)
				mSwap(lower,upper);
		}
		void expand(const CRange& other);
		void expand(double includeCoord);
		CRange expanded(const CRange& other)const;
		CRange expanded(double includeCoord)const;
		CRange bounded(double lowerBound, double upperBound)const;
		CRange sanitizedForLogScale()const;
		CRange sanitizedForLinScale()const;
		bool contains(double value)const
		{
			return value>=lower&&value<=upper;
		}
		static bool validRange(double lower, double upper);
		static bool validRange(const CRange& range);
		static const double minRange;
		static const double maxRange;
};

inline std::ostream& operator<<(std::ostream& os,const CRange& range)
{
	os<<"CRange( upper: "<<range.upper<<" lower: "<<range.lower<<" )";
	return os;
}

inline const CRange operator+(const CRange& range, double value)
{
	CRange res(range);
	res+=value;
	return res;
}

inline const CRange operator+(double value, const CRange& range)
{
	return range+value;
}

inline const CRange operator*(const CRange& range, double value)
{
	CRange res(range);
	res*=value;
	return res;
}

inline const CRange operator*(double value, const CRange& range)
{
	return range*value;
}

inline const CRange operator-(const CRange& range, double value)
{
	CRange res(range);
	res-=value;
	return res;
}

inline const CRange operator/(const CRange& range, double value)
{
	CRange res(range);
	res/=value;
	return res;
}

class CDataRange{
	public:
		CDataRange();
		CDataRange(int begin, int end);
		bool operator==(const CDataRange& other)const{return mBegin ==other.mBegin&& mEnd==other.mEnd;}
		bool operator!=(const CDataRange& other)const{return !(*this==other);}
		
		int begin()const{return mBegin;}
		int end()const{return mEnd;}
		int size()const{return mEnd-mBegin;}
		int length()const{return size();}
		
		void setBegin(int begin){mBegin=begin;}
		void setEnd(int end){mEnd = end;}
		
		bool isValid()const{return (mEnd>=mBegin)&&(mBegin>=0);}
		bool isEmpty()const{return length()==0;}
		CDataRange bounded(const CDataRange& other)const;
		CDataRange expanded(const CDataRange& other)const;
		CDataRange intersection(const CDataRange& other)const;
		CDataRange adjusted(int changeBegin, int changeEnd)const{return CDataRange(mBegin+changeBegin, mEnd+changeEnd);}
		bool intersects(const CDataRange& other)const;
		bool contains(const CDataRange& other)const;
		
		private:
			int mBegin, mEnd;
};

class CVector2D
{
	public:
		CVector2D();
		CVector2D(double x, double y);
		CVector2D(const CPoint& point);
		CVector2D(const CPointF& point);
		
		double x()const{return mX;}
		double y()const{return mY;}
		double& rx(){return mX;}
		double& ry(){return mY;}
		
		void setX(double x){ mX=x;}
		void setY(double y){ mY=y;}
		
		double length()const{return mSqrt(mX*mX+mY*mY);}
		double lengthSquared()const{return mX*mX+mY*mY;}
		CPoint toPoint()const{return CPoint(mX, mY);}
		CPointF toPointF()const{return CPointF(mX, mY);}
		
		bool isNull()const{return mIsNull(mX)&&mIsNull(mY);}
		void normalize();
		CVector2D normalized()const;
		CVector2D perpendicular()const{return CVector2D(-mY, mX);}
		double dot(const CVector2D& vec)const{return mX*vec.mX+mY*vec.mY;}
		double distanceSquaredToLine(const CVector2D& start, const CVector2D& end)const;
		double distanceSquaredToLine(const CLineF& line)const;
		double distanceToStraightLine(const CVector2D& base,const CVector2D& direction)const;
		CVector2D& operator*=(double factor);
		CVector2D& operator/=(double divisor);
		CVector2D& operator+=(const CVector2D& vector);
		CVector2D& operator-=(const CVector2D& vector);
		
	private:
		double mX, mY;
		friend inline const CVector2D operator*(double factor, const CVector2D& vec);
		friend inline const CVector2D operator*(const CVector2D& vec, double factor);
		friend inline const CVector2D operator/(const CVector2D& vec, double divisor);
		friend inline const CVector2D operator+(const CVector2D& vec1, const CVector2D& vec2);
		friend inline const CVector2D operator-(const CVector2D& vec1, const CVector2D& vec2);
		friend inline const CVector2D operator-(const CVector2D& vec);
};


inline const CVector2D operator*(double factor, const CVector2D& vec)
{
	CVector2D v=vec;
	return v*=factor;
}
inline const CVector2D operator*(const CVector2D& vec, double factor)
{
	CVector2D v=vec;
	return v*=factor;
}
inline const CVector2D operator/(const CVector2D& vec, double divisor)
{
	CVector2D v=vec;
	return v/=divisor;
}
inline const CVector2D operator+(const CVector2D& vec1, const CVector2D& vec2)
{
	CVector2D v=vec1;
	return v+=vec2;
}
inline const CVector2D operator-(const CVector2D& vec1, const CVector2D& vec2)
{
	CVector2D v=vec1;
	return v-=vec2;
}
inline const CVector2D operator-(const CVector2D& vec)
{
	return CVector2D(-vec.mX, -vec.mY);
}

inline std::ostream& operator<<(std::ostream& os, const CVector2D& vec)
{
	
	return os<<"CVector2D(: "<<vec.x()<<","<<vec.y()<<" )";
}



typedef unsigned short WORD;

class CPlotPainter;
class CPlotAxis;
class CAxisRect;
class CLayerable;



typedef struct tagPlotFont
{
	int width;
	int height;
	int color;
}PlotFont;

class CPlotPainter:public CPainter
{
	public:
	CPlotPainter();
	explicit CPlotPainter(CPaintDevice* device);
	bool antialiasing()const{return testRenderHint(CPainter::Antialiasing);}
	void setAntialiasing(bool enabled);
	
	bool begin(CPaintDevice* device);
	void setPen(const CPen& pen);
	void setPen(int  color);
	void setPen(Gemini::PenStyle penStyle);
	void drawLine(const CLineF& line);
	void drawLine(const CPointF& p1, const CPointF& p2){ drawLine(CLineF(p1, p2));}
	
	void save();
	void restore();
	
	bool mIsAntialiasing;
	std::stack<bool> mAntialiasingStack;
};




class CScatterStyle
{
	public:
		enum ScatterProperty{
			spNone = 0x00,
			spPen = 0x01,
			spBrush = 0x02,
			spSize = 0x04,
			spShape = 0x08,
			spAll = 0xFF
		};
		
		enum ScatterShape{
			ssNone,
			ssDot,
			ssCross,
			ssPlus,
			ssCircle,
			ssDisc,
			ssSquare,
			ssDiamond,
			ssStar,
			ssTriangle,
			ssTriangleInverted,
			ssCrossSquare,
			ssPlusSquare,
			ssCrossCircle,
			ssPlusCircle,
			ssPeace,
			ssPixmap,
			ssCustom
		};
		
		CScatterStyle();
		/*CScatterStyle(const CScatterStyle& scatterStyle);
		CScatterStyle operator=(const CScatterStyle& scatterStyle);*/
		bool operator==(const CScatterStyle& scatterStyle)const {return !(*this!=scatterStyle);}
		bool operator!=(const CScatterStyle& scatterStyle)const;
		CScatterStyle(WORD shape, double size=6);
		CScatterStyle(WORD shape, int color, double size);
		CScatterStyle(WORD shape, int color, int fillColor, double size);
		CScatterStyle(WORD shape, const CPen& pen, const CBrush& brush, double size);
		CScatterStyle(const CImage& image);
		CScatterStyle(const CPainterPath& customPath, const CPen& pen, const CBrush& brush=Gemini::NoBrush, double size=6);
		
		double size() const {return mSize;}
		WORD shape()const {return mShape;}
		CPen pen()const {return mPen; }
		CBrush brush()const {return mBrush ;}
		CImage pixmap()const{return mPixmap;}
		CPainterPath customPath()const{return mCustomPath;}
		
		void setFromOther(const CScatterStyle& other, WORD properties);
		void setSize(double size);
		void setShape(WORD shape);
		void setPen(const CPen& pen);
		void setBrush(const CBrush& brush);
		void setPixmap(const CImage& pixmap);
		void setCustomPath(const CPainterPath& customPath);
		
		
		bool isNone()const {return mShape==ssNone;}
		bool isPenDefined()const{return mPenDefined;}
		void undefinedPen();
		void applyTo(CPlotPainter* painter, const CPen& defaultPen) const;
		void drawShape(CPlotPainter* painter, const CPointF& pos)const;
		void drawShape(CPlotPainter* painter, double x, double y)const;
		
	
		double mSize;
		WORD  mShape;
		CPen mPen;
		CBrush mBrush;
		CImage mPixmap;
		CPainterPath mCustomPath;
		
		bool mPenDefined;
		
};		

class CAbstractPlottable;

class CSelectionDecorator
{
	public:
		CSelectionDecorator();
		virtual ~CSelectionDecorator();
		
		CPen pen()const {return mPen;}
		CBrush brush()const {return mBrush;}
		CScatterStyle scatterStyle()const{return mScatterStyle;}
		WORD usedScatterProperties() const {return mUsedScatterProperties;}
		
		void setPen(const CPen& pen);
		void setBrush(const CBrush& brush);
		void setScatterStyle(const CScatterStyle& scatterStyle, WORD usedProperties=CScatterStyle::spPen);
		void setUsedScatterProperties(WORD usedProperties);
		void applyPen(CPlotPainter* painter) const;
		void applyBrush(CPlotPainter* painter)const;
		CScatterStyle getFinalScatterStyle(const CScatterStyle& unselectedStyle)const;
		virtual void copyFrom(const CSelectionDecorator* other);
		//virtual void drawDecoration(CPlotPainter* painter, CDataSelection selection);
		
		CPen mPen;
		CBrush mBrush;
		CScatterStyle mScatterStyle;
		WORD mUsedScatterProperties;
		CAbstractPlottable* mPlottable;
		virtual bool registerWidthPlottable(CAbstractPlottable* plottable);
		
	private:
		DISABLE_COPY(CSelectionDecorator)
		friend class CAbstractPlottable;
};



/*=========================Data Container===========================*/

template <class DataType>
inline bool mLessThanSortKey(const DataType& a, const DataType& b){
	return a.sortKey()<b.sortKey();
}

template <class DataType>
class CDataContainer
{
	public:
		typedef typename CVector<DataType>::const_iterator const_iterator;
		typedef typename CVector<DataType>::iterator iterator;
		
		CDataContainer();
		
		int size()const{return mData.size()-mPreallocsize;}
		bool isEmpty()const{return size()==0;}
		bool autoSqueeze()const{return mAutoSqueeze;}
		
		void setAutoSqueeze(bool enabled);
		
		void set(const CDataContainer<DataType>& data);
		void set(const CVector<DataType>& data, bool alreadySorted=false);
		void add(const CDataContainer<DataType>& data);
		void add(const CVector<DataType>& data, bool alreadySorted=false);
		void add(const DataType& data);
		void removeBefore(double sortKey);
		void removeAfter(double sortKey);
		void remove(double sortKeyFrom, double sortKeyTo);
		void remove(double sortKey);
		void clear();
		void sort();
		void squeeze(bool preAllocation=true, bool postAllocation=true);
		
		const_iterator constBegin()const{return mData.constBegin()+mPreallocsize;}
		const_iterator constEnd()const{return mData.constEnd();}
		iterator begin(){return mData.begin()+mPreallocsize;}
		iterator end(){return mData.end();}
		const_iterator findBegin(double sortKey, bool expandedRange=true)const;
		const_iterator findEnd(double sortKey, bool expandedRange=true)const;
		CRange keyRange(bool& foundRange,WORD signDomain=Plot::sdBoth);
		CDataRange dataRange()const{return CDataRange(0,size());}
		CRange valueRange(bool& foundRange, WORD signDomain=Plot::sdBoth,const CRange& inKeyRange=CRange());
		void limitIteratorToDataRange(const_iterator& begin, const_iterator& en, const CDataRange& dataRange)const;
		
	protected:
		bool mAutoSqueeze;
		CVector<DataType> mData;
		int mPreallocsize;
		int mPreallocIteration;
		
		void preallocationGrow(int minimumPreallocSize);
		void performAutoSqueeze();
};


class CLayer
{
	public:
		CLayer(const std::string& layerName);
		virtual ~CLayer();
		
		std::string name()const{return mName ;}
		int index()const{return mIndex;}
		CList<CLayerable*> children()const{return mChildren;}
		bool visible()const {return mVisible;}
		
		void setVisible(bool visible);
		void replot();
		
		std::string mName;
		int mIndex;
		CList<CLayerable*> mChildren;
		bool mVisible;
		
		void draw(CPlotPainter* painter);
		void addChild(CLayerable* layerable, bool prepend);
		void removeChild(CLayerable* layerable);
	
	private:
		DISABLE_COPY(CLayer)
		
		friend class CLayerable;
};


class CLayerable
{
	public:
		CLayerable(std::string targetLayer=std::string(), CLayerable* parentLayerable=0);
		virtual ~CLayerable();
		
		bool visible()const{return mVisible;}
		CLayerable* parentLayerable()const{return mParentLayerable;}
		CLayer* layer()const{return mLayer;}
		bool antialiased()const{return mAntialiased;}
		
		void setVisible(bool on);
		bool setLayer(CLayer* layer);
		bool setLayer(const std::string& layerName);
		void setAntialiased(bool enabled);
		
		bool realVisibility()const;
		
		bool mVisible;
		CLayerable* mParentLayerable;
		CLayer* mLayer;
		bool mAntialiased;
		
		virtual CRect clipRect()const;
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const =0;
		virtual void draw(CPlotPainter* painter)=0;
		
		void setParentLayerable(CLayerable* parentLayerable);
		bool moveToLayer(CLayer* layer, bool prepend);
		void applyAntialiasingHint(CPlotPainter* painter, bool localAntialiased, WORD element)const;
	private:
		DISABLE_COPY(CLayerable)
		friend class CLayer;
		friend class CAxisRect;
};


class CDataSelection{
	public:
		explicit CDataSelection();
		explicit CDataSelection(const CDataRange& range);
		bool operator==(const CDataSelection& other)const;
		bool operator!=(const CDataSelection& other)const{return !(*this==other);}
		
		CDataSelection& operator+=(const CDataSelection& other);
		CDataSelection& operator+=(const CDataRange& other);
		CDataSelection& operator-=(const CDataSelection& other);
		CDataSelection& operator-=(const CDataRange& other);
		friend inline const CDataSelection operator+(const CDataSelection& a, const CDataSelection& b);
		friend inline const CDataSelection operator+(const CDataRange& a, const CDataSelection& b);
		friend inline const CDataSelection operator+(const CDataSelection& a, const CDataRange& b);
		friend inline const CDataSelection operator+(const CDataRange& a, const CDataRange& b);
		friend inline const CDataSelection operator-(const CDataSelection& a, const CDataSelection& b);
		friend inline const CDataSelection operator-(const CDataSelection& a, const CDataRange& b);
		friend inline const CDataSelection operator-(const CDataRange& a, const CDataRange& b);
		friend inline const CDataSelection operator-(const CDataRange& a, const CDataSelection& b);
		
		int dataRangeCount()const{ return mDataRanges.size();}
		int dataPointCount()const;
		CDataRange dataRange(int index=0)const;
		CList<CDataRange> dataRanges()const{return mDataRanges;}
		CDataRange span()const;
		
		void addDataRange(const CDataRange& dataRange, bool simplify=true);
		void clear();
		bool isEmpty()const{return mDataRanges.isEmpty();}
		void simplify();
		
		void enforceType(WORD type);
		bool contains(const CDataSelection& other)const;
		CDataSelection intersection(const CDataRange& other)const;
		CDataSelection intersection(const CDataSelection& other)const;
		CDataSelection inverse(const CDataRange& outerRange)const;
	private:
		CList<CDataRange> mDataRanges;
		inline static bool lessThanDataRangeBegin(const CDataRange& a, const CDataRange& b){return a.begin()<b.begin();}
};



inline const CDataSelection operator+(const CDataSelection& a, const CDataSelection& b)
{
	CDataSelection result(a);
	result +=b;
	return result;
}

inline const CDataSelection operator+(const CDataRange& a, const CDataSelection& b)
{
	CDataSelection result(a);
	result+=b;
	return result;
}

inline const CDataSelection operator+(const CDataSelection& a, const CDataRange& b)
{
	CDataSelection result(a);
	result+=b;
	return result;
}

inline const CDataSelection operator+(const CDataRange& a, const CDataRange& b)
{
	CDataSelection result(a);
	result+=b;
	return result;
}

inline const CDataSelection operator-(const CDataSelection& a, const CDataSelection& b)
{
	CDataSelection result(a);
	result-=b;
	return result;
}

inline const CDataSelection operator-(const CDataRange& a, const CDataSelection& b)
{
	CDataSelection result(a);
	result-=b;
	return result;
}

inline const CDataSelection operator-(const CDataSelection& a, const CDataRange& b)
{
	CDataSelection result(a);
	result-=b;
	return result;
}

inline const CDataSelection operator-(const CDataRange& a, const CDataRange& b)
{
	CDataSelection result(a);
	result-=b;
	return result;
}



class CPlottableInterface
{
	public:
		virtual int dataCount()const = 0;
		virtual double dataMainKey(int index)const = 0;
		virtual double dataSortKey(int index)const = 0;
		virtual double dataMainValue(int index)const = 0;
		virtual CRange dataValueRange(int index)const = 0;
		virtual CPointF dataPixelPosition(int index)const = 0;
		virtual bool sortKeyIsMainKey()const = 0;
		virtual int findBegin(double sortKey, bool expandedRange=true)const = 0;
		virtual int findEnd(double sortKey, bool expandedRange=true) const = 0;
};


class CAbstractPlottable:public CLayerable
{
	public:
		CAbstractPlottable(CPlotAxis* keyAxis, CPlotAxis* valueAxis);
		virtual ~CAbstractPlottable();
		
		std::string name()const{return mName;}
		bool antialiasedFill()const{return mAntialiasedFill;}
		bool antialiasedScatters()const{return mAntialiasedScatters;}
		
		CPen pen()const{return mPen;}
		CBrush brush()const{return mBrush;}
		CPlotAxis* keyAxis()const{return mKeyAxis;}
		CPlotAxis* valueAxis()const{return mValueAxis;}
		WORD selectable()const{return mSelectable;}
		bool selected()const{return !mSelection.isEmpty();}
		CDataSelection selection()const{return mSelection;}
		CSelectionDecorator* selectionDeorator()const{return mSelectionDecorator;}
		void setName(const std::string& name);
		void setAntialiasedFill(bool enabled);
		void setAntialiasedScatters(bool enabled);
		void setPen(const CPen& pen);
		void setBrush(const CBrush& brush);
		void setKeyAxis(CPlotAxis* axis);
		void setValueAxis(CPlotAxis* axis);
		void setSelectable(WORD selectable);
		void setSelection(const CDataSelection selecton);
		void setSelectionDecorator(CSelectionDecorator* decorator);
		
		virtual CPlottableInterface* interface(){return 0;}
		virtual CRange getKeyRange(bool & foundRange, WORD inSignDomain=Plot::sdBoth) const = 0;
		virtual CRange getValueRange(bool& foundRange, WORD inSignDomain=Plot::sdBoth,const CRange& inKeyRange=CRange())const =0;

		void coordsToPixels(double key, double value, double& x, double& y)const;
		const CPointF coordsToPixels(double key, double value) const;
		void pixelsToCoords(double x, double y, double& key, double& value)const;
		void pixelsToCoords(const CPointF& pixelPos, double& key, double& value)const;
		void rescaleAxes(bool onlyEnlarge=false)const;
		void rescaleKeyAxis(bool onlyEnlarge=false)const;
		void rescaleValueAxis(bool onlyEnlarge=false, bool inKeyRange=false)const;;
		
		std::string mName;
		bool mAntialiasedFill, mAntialiasedScatters;
		CPen mPen;
		CBrush mBrush;
		CPlotAxis *mKeyAxis, *mValueAxis;
		WORD mSelectable;
		CDataSelection mSelection;
		CSelectionDecorator* mSelectionDecorator;
		
		virtual CRect clipRect() const;
		virtual void draw(CPlotPainter* painter)=0;
		virtual void drawLegendIcon(CPlotPainter* painter, const CRectF& rect) const =0;
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		
		void applyFillAntialiasingHint(CPlotPainter* painter)const;
		void applyScattersAntialiasingHint(CPlotPainter* painter)const;
		
		
		
		
	private:
		DISABLE_COPY(CAbstractPlottable);
		friend class CPlotAxis;
};




template <class DataType>
class CAbstractPlottable1D:public CAbstractPlottable, public CPlottableInterface
{
	public:
		CAbstractPlottable1D(CPlotAxis* keyAxis, CPlotAxis* valueAxis);
		virtual ~CAbstractPlottable1D();
		
		virtual int dataCount()const;
		virtual double dataMainKey(int index)const;
		virtual double dataSortKey(int index)const;
		virtual double dataMainValue(int index)const;
		virtual CRange dataValueRange(int index)const;
		virtual CPointF dataPixelPosition(int index)const;
		
		virtual bool sortKeyIsMainKey()const;
		virtual int findBegin(double sortKey, bool expandedRange=true)const;
		virtual int findEnd(double sortKey, bool expandedRange=true)const;
		virtual CPlottableInterface* interface(){ return this;}
		
		CSharedPointer<CDataContainer<DataType> > mDataContainer;
		
		void getDataSegments(CList<CDataRange>& seletedSegments, CList<CDataRange>& unselectedSegments)const;
		void drawPolyline(CPlotPainter* painter, const CVector<CPointF>& lineData)const;
	private:
		DISABLE_COPY(CAbstractPlottable1D)
};

template <class DataType>
CAbstractPlottable1D<DataType>::CAbstractPlottable1D(CPlotAxis* keyAxis, CPlotAxis* valueAxis)
:CAbstractPlottable(keyAxis, valueAxis),
mDataContainer(new CDataContainer<DataType>)
{
	
}

template <class DataType>
CAbstractPlottable1D<DataType>::~CAbstractPlottable1D()
{
	
}


template <class DataType>
int CAbstractPlottable1D<DataType>::dataCount()const
{
	return mDataContainer->size();
}

template <class DataType>
double CAbstractPlottable1D<DataType>::dataMainKey(int index) const
{
	if(index>=0&& index<mDataContainer->size())
	{
		return (mDataContainer->constBegin()+index)->mainKey();
	}
	else
	{
		std::cout<<"Error: index out of bounds"<<std::endl;
		return 0;
	}
}

template <class DataType>
double CAbstractPlottable1D<DataType>::dataSortKey(int index)const
{
	if(index>=0&&index<mDataContainer->size())
	{
		return (mDataContainer->constBegin()+index)->sortKey();
	}
	else
	{
		std::cout<<"Error: index out of bounds"<<std::endl;
		return 0;
	}
	
}


template <class DataType>
double CAbstractPlottable1D<DataType>::dataMainValue(int index) const
{
	if(index>=0&&index<mDataContainer->size())
	{
		return (mDataContainer->constBegin()+index)->mainValue();
	}
	else
	{
		std::cout<<"Error: index out of bounds"<<std::endl;
		return 0;
	}
}

template <class DataType>
CRange CAbstractPlottable1D<DataType>::dataValueRange(int index)const
{
	if(index>=0&&index<mDataContainer->size())
	{
		return (mDataContainer->constBegin()+index)->valueRange();
	}
	else
	{
		std::cout<<"Error: index out of bounds"<<std::endl;
		return CRange(0,0);
	}
}

template <class DataType>
CPointF CAbstractPlottable1D<DataType>::dataPixelPosition(int index)const
{
	if(index>=0&&index<mDataContainer->size())
	{
		const typename CDataContainer<DataType>::const_iterator it=mDataContainer->constBegin()+index;
		return coordsToPixels(it->mainKey(), it->mainValue());
	}
	else
	{
		
		std::cout<<"Error: index out of bounds"<<std::endl;
		return CPointF();
	}
}

template <class DataType>
bool CAbstractPlottable1D<DataType>::sortKeyIsMainKey()const
{
	return DataType::sortKeyIsMainKey();
}


template <class DataType>
int CAbstractPlottable1D<DataType>::findBegin(double sortKey, bool expandedRange)const
{
	return mDataContainer->findBegin(sortKey, expandedRange)-mDataContainer->constBegin();
}

template <class DataType>
int CAbstractPlottable1D<DataType>::findEnd(double sortKey, bool expandedRange)const
{
	return mDataContainer->findEnd(sortKey, expandedRange)-mDataContainer->constBegin();
}

template <class DataType>
void CAbstractPlottable1D<DataType>::getDataSegments(CList<CDataRange>& selectedSegments, CList<CDataRange>& unselectedSegments)const
{
	selectedSegments.clear();
	unselectedSegments.clear();
	if(mSelectable==Plot::stWhole)
	{
		if(selected())
			selectedSegments<<CDataRange(0,dataCount());
		else
			unselectedSegments<<CDataRange(0,dataCount());
	}
	else
	{
		CDataSelection sel(selection());
		sel.simplify();
		selectedSegments=sel.dataRanges();
		unselectedSegments=sel.inverse(CDataRange(0,dataCount())).dataRanges();
	}
}


template <class DataType>
void CAbstractPlottable1D<DataType>::drawPolyline(CPlotPainter* painter, const CVector<CPointF>& lineData)const
{
	if(painter->pen().style()==Gemini::SolidLine)
	{
		int i=0;
		bool lastIsNan=false;
		const int lineDataSize=lineData.size();
		while(i<lineDataSize&&(isnan(lineData.at(i).y())||isnan(lineData.at(i).x())))
			++i;
		if(painter->antialiasing()==true)
		{
			CPainterPath cubicPath;
			cubicPath.moveTo(lineData.at(i));
			++i;
			while(i<lineDataSize)
			{
				if(!isnan(lineData.at(i).y())&&!isnan(lineData.at(i).x()))
				{
					if(!lastIsNan)
					{
						//去重
						if(lineData.at(i-1)!=lineData.at(i))
						{
							CPointF pt1=CPointF(lineData.at(i-1));
							CPointF pt2=CPointF(lineData.at(i));
							if(fuzzyIsNull(pt1.x()-pt2.x())||fuzzyIsNull(pt1.y()-pt2.y()))
							{
								CPoint pt=(pt2+CPointF(0.5,0.5)).toPoint();
								cubicPath.lineTo(pt);
							}
							else
								cubicPath.cubicTo(pt1,pt2,pt2);
							
						}
					}
					else
						lastIsNan = false;
				}
				else
					lastIsNan = true;
				++i;
			}
			painter->drawPath(cubicPath);
		}
		else
		{
			++i;
			while(i<lineDataSize)
			{
				if(!isnan(lineData.at(i).y())&&!isnan(lineData.at(i).x()))
				{
					if(!lastIsNan)
					{
						//去重
						if(lineData.at(i-1)!=lineData.at(i))
						{
							CPointF pt1=CPointF(lineData.at(i-1));
							CPointF pt2=CPointF(lineData.at(i));
							painter->drawLine(pt1, pt2);
							
						}
					}
					else
						lastIsNan = false;
				}
				else
					lastIsNan = true;
				++i;
			}
		}
	}
	else
	{
		int segmentStart=0;
		int i=0;
		const int lineDataSize = lineData.size();
		while(i< lineDataSize)
		{
			if(isnan(lineData.at(i).y())||isnan(lineData.at(i).x())||isinf(lineData.at(i).y()))
			{
				painter->drawPolyline(lineData.constData()+segmentStart, i-segmentStart);
				segmentStart=i+1;
			}
			++i;
		}
		painter->drawPolyline(lineData.constData()+segmentStart, lineDataSize-segmentStart);
	}
}












class CLineEnding
{
	public:
		enum EndingStyle{
			None,
			FlatArrow,
			SpikeArrow,
			LineArrow,
			Disc,
			Square,
			Diamond,
			Bar,
			HalfBar,
			SkewedBar
		};
		
		
		CLineEnding();
		CLineEnding(WORD style, double width=8, double length=10, bool inverted=false);
		WORD style()const{return mStyle;}
		double width()const{return mWidth;}
		double length()const{return mLength;}
		bool inverted()const{return mInverted;}
		
		void setStyle(WORD style);
		void setWidth(double width);
		void setLength(double length);
		void setInverted(bool inverted);
		
		double boundingDistance()const;
		double realLength()const;
		void draw(CPlotPainter* painter, const CVector2D& pos, const CVector2D& dir) const;
		void draw(CPlotPainter* painter, const CVector2D& pos, double angle)const;
		void draw(CPainter* painter, const CVector2D& pos, double angle) const;
		void draw(CPainter* painter, const CVector2D& pos, const CVector2D& dir) const;
		WORD mStyle;
		double mWidth, mLength;
		bool mInverted;
};




class CAxisTicker
{
	public:
		enum TickStepStrategy
		{
			Readability,/*优先按照可读性更好的坐标刻度处理*/
			MeetTickCount/*优先按照设置的刻度数处理*/
		};
		
		CAxisTicker();
		virtual ~CAxisTicker();
		
		WORD tickStepStrategy()const{return mTickStepStrategy;}
		int tickCount()const{return mTickCount;}
		double tickOrigin()const{return mTickOrigin;}
		void setTickStepStrategy(WORD strategy);
		void setTickCount(int count);
		void setTickOrigin(double origin);
		
		virtual void generate(const CRange& range, char format,const int precision, CVector<double>& ticks, 
		CVector<double>*subTicks,std::vector<std::string>* tickLabels);
		
		WORD mTickStepStrategy;
		int mTickCount;
		double mTickOrigin;
		virtual double getTickStep(const CRange& range);
		virtual int getSubTickCount(double tickStep);
		virtual std::string getTickLabel(double tick, char format,int precision);
		virtual CVector<double> createTickVector(double tickStep, const CRange& range);
		virtual CVector<double> createSubTickVector(int subTickCount, const CVector<double>& ticks);
		virtual std::vector<std::string> createLabelVector(const CVector<double>&ticks,char format, int precision);
		void trimTicks(const CRange& range, CVector<double>&ticks, bool keepOneOutlier)const;
		double pickClosest(double target, const CVector<double>& candidates)const;
		double getMantissa(double input, double* magnitude=0)const;
		double cleanMantissa(double input)const;
};

class CAxisTickerLog:public CAxisTicker
{
	public:
		CAxisTickerLog();
		double logBase()const{return mLogBase;}
		int subTickCount()const{return mSubTickCount;}
		void setLogBase(double base);
		void setSubTickCount(int subTicks);
	
		double mLogBase;
		int mSubTickCount;
		
		double mLogBaseLnInv;
		virtual double getTickStep(const CRange& range);
		virtual int getSubTickCount(double tickStep);
		virtual CVector<double> createTickVector(double tickStep, const CRange& range);
};



class CAxisTickerTime:public CAxisTicker
{
	public:
		enum TimeUnit{
			Milliseconds,
			Seconds,
			Minutes,
			Hours,
			Days
		};
		
		CAxisTickerTime();
		
		std::string timeFormat()const {return mTimeFormat;}
		int fieldWidth(WORD unit) const 
		{
			std::map<WORD, int>::const_iterator it=mFieldWidth.find(unit);
			if(it!=mFieldWidth.end())
				return it->second;
			else
				return -1;
		}
		void setTimeFormat(const std::string& format);
		void setFieldWidth(TimeUnit unit, int width);
		std::string mTimeFormat;
		std::map<WORD,int>mFieldWidth;
		WORD mSmallestUnit, mBiggestUnit;
		std::map<WORD,std::string>mFormatPattern;
		
		virtual double getTickStep(const CRange& range);
		virtual int getSubTickCount(double tickStep);
		virtual std::string getTickLabel(double tick, char format, int precision);
		void replaceUnit(std::string& text, WORD unit, int value)const;
};

class CPlotAxis;

class CAxisRect:public CLayerable
{
	public:
	CAxisRect(bool setupDefaultAxes=true);
	virtual ~CAxisRect();
	
	CImage background() const{return mBackgroundImage;}
	CBrush backgroundBrush()const{return mBackgroundBrush;}
	bool backgroundScaled()const{return mBackgroundScaled;}
	WORD rangeZoom()const{return mRangeZoom;}
	CPlotAxis* rangeZoomAxis(WORD orientation);
	CList<CPlotAxis*>rangeZoomAxes(WORD orientation);
	double rangeZoomFactor(WORD orientation);
	
	void setBackground(const CImage& img);
	void setBackground(const CImage&, bool scaled);
	void setBackground(const CBrush& brush);
	void setBackgroundScaled(bool scaled);
	void setRangeZoom(WORD orientation);
	void setRangeZoomAxes(CPlotAxis* horizontal, CPlotAxis* vertical);
	void setRangeZoomAxes(CList<CPlotAxis*> horizontal, CList<CPlotAxis*> vertical);
	void setRangeZoomAxes(CList<CPlotAxis*> axes);
	void setRangeZoomFactor(double horizontal, double vertical);
	void setRangeZoomFactor(double factor);
	
	int axisCount(WORD type);
	CPlotAxis* axis(WORD type, int index=0);
	CList<CPlotAxis*> axes(WORD type);
	CList<CPlotAxis*>axes()const;
	CPlotAxis* addAxis(WORD type, CPlotAxis* axis=0);
	CList<CPlotAxis*> addAxes(WORD type);
	bool removeAxis(CPlotAxis* axis);
	void zoom(const CRectF& pixelRect);
	void zoom(const CRectF& pixelRect, const CList<CPlotAxis*>& affectedAxes);
	void setupFullAxesBox(bool connectRanges=false);
	//graphs
	
	
	
	int left()const{return mRect.left();}
	int right()const{return mRect.right();}
	int bottom()const{return mRect.bottom();}
	int top()const{return mRect.top();}
	int height()const{return mRect.height();}
	int width()const{return mRect.width();}
	CSize size()const{return mRect.size();}
	CPoint topLeft()const{return mRect.topLeft();}
	CPoint topRight()const{return mRect.topRight();}
	CPoint bottomLeft()const{return mRect.bottomLeft();}
	CPoint bottomRight()const{return mRect.bottomRight();}
	CPoint center()const{return mRect.center();}
	CRect rect()const{return mRect;}
	void setRect(const CRect& rect);
	//
	CBrush mBackgroundBrush;
	CImage mBackgroundImage;
	CImage mScaledBackgroundImage;
	bool mBackgroundScaled;
	WORD mRangeZoom;
	CList<CPlotAxis* > mRangeZoomHorzAxis, mRangeZoomVertAxis;
	double mRangeZoomFactorHorz, mRangeZoomFactorVert;
	CSize mMiniumSize, mMaximumSize;
	CRect mRect;
	CMargin mMargins, mMinimumMargins;
	std::map<WORD, CList<CPlotAxis*> > mAxes;
	
	virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
	virtual void draw(CPlotPainter* painter);
//	virtual int calcuteAutoMargin();
	void drawBackground(CPlotPainter* painter);
	void updateAxesOffset(WORD type);
	
		
		
		
	private:
		DISABLE_COPY(CAxisRect)
		
	
	
};

class CPlotGrid;
class CAxisPainterPrivate;

class CPlotAxis: public CLayerable
{
	public:
		enum AxisType{
			atLeft =0x01,
			atRight =0x02,
			atTop =0x04,
			atBottom = 0x08
		};
		enum LabelSide{
			lsInSide,
			lsOutSide
		};
		enum ScaleType{
			Linear,
			Logarithmic
		};
		
		
		enum SelectablePart{
			spNone =0,
			spAxis =0x001,
			spTickLabels =0x002,
			spAxisLabel =0x004
		};
	
		explicit CPlotAxis(CAxisRect* parent, WORD type);
		virtual ~CPlotAxis();
		
		WORD axisType()const {return mAxisType;}
		CAxisRect* axisRect()const{return mAxisRect;}
		WORD scaleType()const{return mScaleType;}
		const CRange range()const {return mRange;}
		bool rangeReversed()const{return mRangeReversed;}
		CAxisTicker* ticker()const{return mTicker;}
		bool ticks()const{return mTicks;}
		bool tickLabels()const{return mTickLabels;}
		int tickLabelPadding()const;
		PlotFont tickLabelFont()const{return mTickLabelFont;}
		double tickLabelRotation()const;
		WORD tickLabelSide()const;
		std::string numberFormat()const;
		int numberPrecision()const {return mNumberPrecision;}
		CVector<double> tickVector()const{return mTickVector;}
		std::vector<std::string> tickVectorLabels()const{return mTickVectorLabels;}
		int tickLengthIn()const;
		int tickLengthOut()const;
		bool subTicks()const{return mSubTicks;}
		int subTickLengthIn()const;
		int subTickLengthOut()const;
		CPen basePen()const{return mBasePen;}
		CPen tickPen()const{return mTickPen;}
		CPen subTickPen()const{return mSubTickPen;}
		PlotFont labelFont()const{return mLabelFont;}
		std::string label()const{return mLabel;}
		int labelPadding()const;
		int padding()const{return mPadding;}
		int offset()const;
		WORD selectedParts()const{return mSelectedParts;}
		WORD selectableParts()const{return mSelectableParts;}
		
		PlotFont selectedTickLabelFont()const{return mSelectedTickLabelFont;}
		PlotFont selectedLabelFont()const{return mSelectedLabelFont;}
		CPen selectedBasePen()const{return mSelectedBasePen;}
		CPen selectedTickPen()const{return mSelectedTickPen;}
		CPen selectedSubTickPen()const{return mSelectedSubTickPen;}
		CLineEnding lowerEnding()const;
		CLineEnding upperEnding()const;
		CPlotGrid* grid()const{return mGrid;}
		bool numberBeautifulPowers()const {return mNumberBeautifulPowers;}
		void setScaleType(WORD type);
		void setRange(const CRange& range);
		void setRange(double lower, double upper);
		void setRange(double position, double size, WORD align);
		void setRangeLower(double lower);
		void setRangeUpper(double upper);
		void setRangeReversed(bool reversed);
		void setTicker(CAxisTicker* ticker);
		void setTicks(bool show);
		void setTickLabels(bool show);
		void setTickLabelPadding(int padding);
		void setTickLabelFont(const PlotFont& font);
		void setTickLabelRotation(double degrees);
		void setTickLabelSide(WORD side);
		void setNumberFormat(const std::string& format);
		void setNumberPrecision(int precision);
		void setTickLength(int inside, int outside=0);
		void setTickLengthIn(int inside);
		void setTickLengthOut(int outside);
		void setSubTicks(bool show);
		void setSubTickLength(int inside, int outside=0);
		void setSubTickLengthIn(int inside);
		void setSubTickLengthOut(int outside);
		void setBasePen(const CPen& pen);
		void setTickPen(const CPen& pen);
		void setSubTickPen(const CPen& pen);
		void setLabelFont(const PlotFont& font);
		void setLabel(const std::string& text);
		void setLabelPadding(int padding);
		void setPadding(int pading);
		void setOffset(int offset);
		void setSelectedTickLabelFont(const PlotFont& font);
		void setSelectedLabelFont(const PlotFont& font);
		void setSelectedBasePen(const CPen& pen);
		void setSelectedTickPen(const CPen& pen);
		void setSelectedSubTickPen(const CPen& pen);
		void setSelectableParts(WORD selectableParts);
		void setSelectedParts(WORD seletedParts);
		void setLowerEnding(const CLineEnding& ending);
		void setUpperEnding(const CLineEnding& ending);
		
		WORD orientation()const{return mOrientation;}
		int pixelOrientation()const{return rangeReversed()!=(orientation()==Gemini::Vertical)?-1:1;}
		void moveRange(double diff);
		void scaleRange(double factor);
		void scaleRange(double factor, double center);
		void setScaleRatio(const CPlotAxis* otherAxis, double ratio=1.0);
//		void rescale(bool onlyVisiblePlottables=false);
		double pixelToCoord(double value)const;
		double coordToPixel(double value)const;
		WORD getPartAt(const CPointF& pos)const;
//
//
//
//		static WORD marginSideToAxisType(WORD side);
		static WORD orientation(WORD type){return type==atBottom||type==atTop? Gemini::Horizontal:Gemini::Vertical;}
		static WORD opposite(WORD type);
		
		WORD mAxisType;
		CAxisRect* mAxisRect;
		int mPadding;
		WORD mOrientation;
		WORD mSelectableParts, mSelectedParts;
		CPen mBasePen, mSelectedBasePen;
		std::string mLabel;
		PlotFont mLabelFont, mSelectedLabelFont;
		bool mTickLabels;
		PlotFont mTickLabelFont, mSelectedTickLabelFont;
		int mNumberPrecision;
		char mNumberFormat;
		bool mNumberBeautifulPowers;
		bool mTicks;
		bool mSubTicks;
		CPen mTickPen, mSelectedTickPen;
		CPen  mSubTickPen,mSelectedSubTickPen;
		CRange mRange;
		bool mRangeReversed;
		WORD mScaleType;
		CPlotGrid* mGrid;
		CAxisPainterPrivate* mAxisPainter;
		CAxisTicker* mTicker;
		CVector<double> mTickVector;
		std::vector<std::string> mTickVectorLabels;
		CVector<double> mSubTickVector;
		
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		virtual void draw(CPlotPainter* painter);
//		virtual WORD selectionCategory()const;
		
		
		
		
		
		void setupTickVectors();
		CPen getBasePen()const;
		CPen getTickPen()const;
		CPen getSubTickPen()const;
		PlotFont getTickLabelFont()const;
		PlotFont getLabelFont()const;
		
		bool mVisible;
		bool mAntialiased;
		
		void setAntialiased(bool enabled)
		{
			mAntialiased=enabled;
		}
		void setVisible(bool visible)
		{
			mVisible=visible;
		}
		
		bool antialiased()const
		{
			return mAntialiased;
		}
		bool visible()const
		{
			return mVisible;
		}
	private:
		DISABLE_COPY(CPlotAxis)
		
		friend class CPlotGrid;
		friend class CAxisRect;
		
		
};



class CAxisPainterPrivate
{
	public:
		CAxisPainterPrivate();
		virtual ~CAxisPainterPrivate();
		
		virtual void draw(CPlotPainter* painter);
		virtual int size()const;
		CRect axisSelectionBox()const { return mAxisSelectionBox;}
		CRect tickLabelSelectionBox()const {return mTickLabelsSelectionBox;}
		CRect labelSelectionBox()const {return mLabelSelectionBox;}
		WORD type;
		CPen basePen;
		CLineEnding lowerEnding, upperEnding;
		PlotFont labelFont;
		std::string label;
		int labelPadding;
		int tickLabelPadding;
		double tickLabelRotation;//目前仅支持0 90 180 360...角度旋转设置
		//标签位置
		WORD  tickLabelSide;
		bool substituteExponent;
		bool numberMultiplyCross;
		
		int tickLengthIn, tickLengthOut, subTickLengthIn, subTickLengthOut;
		
		CPen tickPen, subTickPen;
		PlotFont tickLabelFont;
		int tickLabelColor;
		CRect axisRect, viewportRect;
		double offset;
		bool abbreviateDecimalPowers;
		bool reversedEndings;
		
		CVector<double> subTickPositions;
		CVector<double> tickPositions;
		std::vector<std::string> tickLabels;

		struct TickLabelData
		{
			std::string basePart, expPart, suffixPart;
			CRect baseBounds, expBounds, suffixBounds, totalBounds, rotatedTotalBounds;
			PlotFont baseFont, expFont;
		};
		
		CRect mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox;
		
		
		virtual void placeTickLabel(CPlotPainter* painter, double position , int distanceToAxis, const std::string& text, CSize* tickLabelsSize);
		virtual void drawTickLabel(CPlotPainter* painter, double x, double y, const TickLabelData& labelData)const;
		virtual TickLabelData getTickLabelData(const PlotFont& font, const std::string& text)const;
		virtual CPointF getTickLabelDrawOffset(const TickLabelData& labelData)const;
		virtual void 	getMaxTickLabelSize(const PlotFont& font, const std::string& text, CSize*tickLabelsSize)const;
		
		
		
};




class CPlotGrid: public CLayerable
{
	public:
		CPlotGrid(CPlotAxis* parentAxis);
		
		bool subGridVisible()const{return mSubGridVisible;}
		bool antialiasedSubGrid()const{return mAntialiasedSubGrid;}
		bool antialiasedZeroLine()const{return mAntialiasedZeroLine;}
		CPen pen()const{return mPen;}
		CPen subGridPen()const{return mSubGridPen;}
		CPen zeroLinePen()const{return mZeroLinePen;}
		
		void setSubGridVisible(bool visible);
		void setAntialiasedSubGrid(bool enabled);
		void setAntialiasedZeroLine(bool enabled);
		void setPen(const CPen& pen);
		void setSubGridPen(const CPen& pen);
		void setZeroLinePen(const CPen& pen);
		void setParent(CPlotAxis* parent);
		CPlotAxis* parent()const;
		
		bool mSubGridVisible;
		bool mAntialiasedSubGrid, mAntialiasedZeroLine;
		CPen mPen, mSubGridPen, mZeroLinePen;
		CPlotAxis* mParentAxis;
		
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		virtual void draw(CPlotPainter*);
		void drawGridLines(CPlotPainter* painter)const;
		void drawSubGridLines(CPlotPainter* painter)const;
	private:
		friend class CPlotAxis;
};






template <class DataType>
CDataContainer<DataType>::CDataContainer():
mAutoSqueeze(true),
mPreallocsize(0),
mPreallocIteration(0)
{
	
}

template<class DataType>
void CDataContainer<DataType>::setAutoSqueeze(bool enabled)
{
	if(mAutoSqueeze!=enabled)
	{
		mAutoSqueeze = enabled;
		if(mAutoSqueeze)
			performAutoSqueeze();
	}
}

template<class DataType>
void CDataContainer<DataType>::set(const CDataContainer<DataType>& data)
{
	clear();
	add(data);
}

template <class DataType>
void CDataContainer<DataType>::set(const CVector<DataType>& data, bool alreadySorted)
{
	mData=data;
	mPreallocsize=0;
	mPreallocIteration=0;
	if(!alreadySorted)
		sort();
}

template <class DataType>
void CDataContainer<DataType>::add(const CDataContainer<DataType>& data)
{
	if(data.isEmpty())	return;
	const int n=data.size();
	const int oldSize=size();
	if(oldSize>0&&!mLessThanSortKey<DataType>(*constBegin(),*(data.constEnd()-1)))
	{
		if(mPreallocsize<n)
		{
			preallocationGrow(n);
		}
		mPreallocsize -=n;
		std::copy(data.constBegin(),data.constEnd(), begin());
	}
	else
	{
		mData.resize(mData.size()+n);
		std::copy(data.constBegin(), data.constEnd(), end()-n);
		if(oldSize>0&&!mLessThanSortKey<DataType>(*(constEnd()-n-1),*(constEnd()-n)))
			std::inplace_merge(begin(), end()-n, end(), mLessThanSortKey<DataType>);
	}
}

template <class DataType>
void CDataContainer<DataType>::add(const CVector<DataType>& data, bool alreadySorted)
{
	if(data.isEmpty())
		return ;
	if(isEmpty())
	{
		set(data, alreadySorted);
		return ;
	}
	const int n=data.size();
	const int oldSize=size();
	if(alreadySorted&&oldSize>0&&!mLessThanSortKey<DataType>(*constBegin(),*(data.constEnd()-1)))
	{
		if(mPreallocsize<n)
			preallocationGrow(n);
		mPreallocsize -=n;
		std::copy(data.constBegin(), data.constEnd(), begin());
	}
	else
	{
		mData.resize(mData.size()+n);
		std::copy(data.constBegin(), data.constEnd(), end()-n);
		if(!alreadySorted)
			std::sort(end()-n, end(), mLessThanSortKey<DataType>);
		if(oldSize>0&&mLessThanSortKey<DataType>(*(constEnd()-n-1),*(constEnd()-n)))
			std::inplace_merge(begin(), end()-n, end(), mLessThanSortKey<DataType>);
	}
	
}

template <class DataType>
void CDataContainer<DataType>::add(const DataType& data)
{
	if(isEmpty()||!mLessThanSortKey<DataType>(data,*(constEnd()-1)))
	{
		mData.append(data);
	}
	else if(mLessThanSortKey<DataType>(data, *constBegin()))
	{
		if(mPreallocsize<1)
			preallocationGrow(1);
		--mPreallocsize;
		*begin()=data;
	}
	else
	{
		typename CDataContainer<DataType>::iterator insertionPoint=std::lower_bound(begin(), end(), data, mLessThanSortKey<DataType>);
		mData.insert(insertionPoint, data);
	}
}

template <class DataType>
void CDataContainer<DataType>::removeBefore(double sortKey)
{
	typename CDataContainer<DataType>::iterator it=begin();
	typename CDataContainer<DataType>::iterator itEnd=std::lower_bound(begin(), end(), DataType::fromSortKey(sortKey),mLessThanSortKey<DataType>);
	mPreallocsize+=itEnd-it;
	if(mAutoSqueeze)
		performAutoSqueeze();
}


template <class DataType>
void CDataContainer<DataType>::removeAfter(double sortKey)
{
	typename CDataContainer<DataType>::iterator it=std::upper_bound(begin(), end(), DataType::fromSortKey(sortKey),mLessThanSortKey<DataType>);
	typename CDataContainer<DataType>::iterator itEnd=end();
	mData.erase(it, itEnd);
	if(mAutoSqueeze)
		performAutoSqueeze();
}

template <class DataType>
void CDataContainer<DataType>::remove(double sortKeyFrom, double sortKeyTo)
{
	if(sortKeyFrom>=sortKeyTo||isEmpty())	return;
	
	typename CDataContainer<DataType>::iterator it=std::lower_bound(begin(), end(), DataType::fromSortKey(sortKeyFrom), mLessThanSortKey<DataType>);
	typename CDataContainer<DataType>::iterator itEnd=std::upper_bound(it, end(), DataType::fromSortKey(sortKeyTo),mLessThanSortKey<DataType>);
	mData.erase(it, itEnd);
	if(mAutoSqueeze)
		performAutoSqueeze();
}


template <class DataType>
void CDataContainer<DataType>::remove(double sortKey)
{
	typename CDataContainer::iterator it=std::lower_bound(begin(), end(), DataType::fromSortKey(sortKey),mLessThanSortKey<DataType>);
	if(it!=end()&&it->sortKey()==sortKey)
	{
		if(it==begin())
			++mPreallocsize;
		else
			mData.erase(it);
	}
	if(mAutoSqueeze)
		performAutoSqueeze();
}


template <class DataType>
void CDataContainer<DataType>::clear()
{
	mData.clear();
	mPreallocIteration=0;
	mPreallocsize=0;
}

template<class DataType>
void CDataContainer<DataType>::sort()
{
	std::sort(begin(), end(), mLessThanSortKey<DataType>);
}

template <class DataType>
void CDataContainer<DataType>::squeeze(bool preAllocation, bool postAllocation)
{
	if(preAllocation)
	{
		if(mPreallocsize>0)
		{
			std::copy(begin(), end(), mData.begin());
			mData.resize(size());
			mPreallocsize=0;
		}
		mPreallocIteration = 0;
	}
	if(postAllocation)
		mData.squeeze();
}

template <class DataType>
typename CDataContainer<DataType>::const_iterator CDataContainer<DataType>::findBegin(double sortKey, bool expandedRange)const
{
	if(isEmpty())
		return constEnd();
	typename CDataContainer<DataType>::const_iterator it=std::lower_bound(constBegin(), constEnd(), DataType::fromSortKey(sortKey),mLessThanSortKey<DataType>);
	if(expandedRange&&it!=constBegin())
		--it;
	return it;
}

template <class DataType>
typename CDataContainer<DataType>::const_iterator CDataContainer<DataType>::findEnd(double sortKey, bool expandedRange)const
{
	if(isEmpty())
		return constEnd();
	typename CDataContainer<DataType>::const_iterator it=std::upper_bound(constBegin(), constEnd(), DataType::fromSortKey(sortKey),mLessThanSortKey<DataType>);
	if(expandedRange&&it!=constEnd())
		++it;
	return it;
}

template <class DataType>
CRange CDataContainer<DataType>::keyRange(bool& foundRange, WORD signDomain)
{
	if(isEmpty())
	{
		foundRange = false;
		return CRange();
	}
	
	CRange range;
	bool haveLower = false;
	bool haveUpper = false;
	double current;
	typename CDataContainer<DataType>::const_iterator it=constBegin();
	typename CDataContainer<DataType>::const_iterator itEnd=constEnd();
	if(signDomain==Plot::sdBoth)
	{
		if(DataType::sortKeyIsMainKey())
		{
			while(it!=itEnd)
			{
				if(!isnan(it->mainValue()))
				{
					range.lower=it->mainKey();
					haveLower = true;
					break;
				}
				++it;
			}
			it=itEnd;
			while(it!=constBegin())
			{
				--it;
				if(!isnan(it->mainValue()))
				{
					range.upper=it->mainKey();
					haveUpper=true;
					break;
				}
			}
		}
		else
		{
			while(it!=itEnd)
			{
				if(!isnan(it->mainValue()))
				{
					current=it->mainKey();
					if(current<range.lower||!haveLower)
					{
						range.lower=current;
						haveLower=true;
					}
					if(current>range.upper||!haveLower)
					{
						range.upper=current;
						haveUpper=true;
					}
				}
				++it;
			}
		}
	}else if(signDomain==Plot::sdNegative)
	{
		while(it!=itEnd)
		{
			if(!isnan(it->mainValue()))
			{
				current= it->mainKey();
				if((current<range.lower||!haveLower)&&current<0)
				{
					range.lower= current;
					haveLower = true;
				}
				if((current>range.upper||!haveUpper)&&current<0)
				{
					range.upper = current;
					haveUpper = true;
				}
			}
			++it;
		}
	}else if(signDomain==Plot::sdPositive)
	{
		while(it!=itEnd)
		{
			if(!isnan(it->mainValue()))
			{
				current=it->mainKey();
				if((current<range.lower||!haveLower)&&current>0)
				{
					range.lower = current;
					haveLower=true;
				}
				if((current>range.upper||!haveUpper)&&current>0)
				{
					range.upper = current;
					haveUpper = true;
				}
			}
			++it;
		}
	}
	foundRange = haveLower&&haveUpper;
	return range;
}



template <class DataType>
CRange CDataContainer<DataType>::valueRange(bool& foundRange, WORD signDomain, const CRange& inKeyRange)
{
	if(isEmpty())
	{
		foundRange = false;
		return CRange();
	}
	CRange range;
	const bool restrictKeyRange=inKeyRange!=CRange();
	bool haveLower=false;
	bool haveUpper=false;
	CRange current;
	typename CDataContainer<DataType>::const_iterator itBegin = constBegin();
	typename CDataContainer<DataType>::const_iterator itEnd = constEnd();
	
	if(DataType::sortKeyIsMainKey()&&restrictKeyRange)
	{
		itBegin  = findBegin(inKeyRange.lower);
		itEnd = findEnd(inKeyRange.upper);
	}
	
	if(signDomain==Plot::sdBoth)
	{
		for(typename CDataContainer<DataType>::const_iterator it=itBegin; it!=itEnd; ++it)
		{
			if(restrictKeyRange&&(it->mainKey()<inKeyRange.lower||it->mainKey()>inKeyRange.upper))
				continue;
			current = it->valueRange();
			if((current.lower<range.lower||!haveLower)&&!isnan(current.lower))
			{
				range.lower= current.lower;
				haveLower = true;
			}
			if((current.upper>range.upper||!haveUpper)&&isnan(current.upper))
			{
				range.upper = current.upper;
				haveUpper = true;
			}
		}
	}else if(signDomain==Plot::sdNegative)
	{
		for(typename CDataContainer<DataType>::const_iterator it=itBegin; it!=itEnd; ++it)
		{
			if(restrictKeyRange&&(it->mainKey()<inKeyRange.lower||it->mainKey()>inKeyRange.upper))
			{
				continue;
			}
			current = it->valueRange();
			if((current.lower<range.lower||!haveLower)&&current.lower<0&&!isnan(current.lower))
			{
				range.lower = current.lower;
				haveLower =true;
			}
			if((current.upper>range.upper||!haveUpper)&&current.upper<0&&!isnan(current.upper))
			{
				range.upper = current.upper;
				haveUpper = true;
			}
		}
	}else if(signDomain==Plot::sdPositive)
	{
		for(typename CDataContainer<DataType>::const_iterator it=itBegin; it!=itEnd; ++it)
		{
			if(restrictKeyRange&&(it->mainKey()<inKeyRange.lower||it->mainKey()>inKeyRange.upper))
				continue;
			current = it->valueRange();
			if((current.lower<range.lower||!haveLower)&&current.lower>0&&isnan(current.lower))
			{
				range.lower = current.lower;
				haveLower = true;
			}
			if((current.upper>range.upper||!haveUpper)&&current.upper>0&&!isnan(current.upper))
			{
				range.upper = current.upper;
				haveUpper=true;
			}
		}
	}
	foundRange=haveLower&&haveUpper;
	return range;
}

template <class DataType>
void CDataContainer<DataType>::limitIteratorToDataRange(const_iterator& begin, const_iterator& end, const CDataRange& dataRange)const
{
	CDataRange iteratorRange(begin-constBegin(), end-constBegin());
	iteratorRange = iteratorRange.bounded(dataRange.bounded(this->dataRange()));
	begin = constBegin()+iteratorRange.begin();
	end = constBegin()+iteratorRange.end();
}

template <class DataType>
void CDataContainer<DataType>::preallocationGrow(int minimumPreallocSize)
{
	if(minimumPreallocSize<=mPreallocsize)
		return ;
	int newPreallocSize=minimumPreallocSize;
	newPreallocSize+=(1u<<mBound(4,mPreallocIteration+4,15))-12;
	++mPreallocIteration;
	int sizeDifference = newPreallocSize-mPreallocsize;
	mData.resize(mData.size()+sizeDifference);
	std::copy_backward(mData.begin()+mPreallocsize, mData.end()-sizeDifference, mData.end());
	mPreallocsize=newPreallocSize;
}

template <class DataType>
void CDataContainer<DataType>::performAutoSqueeze()
{
	const int totalAlloc = mData.capacity();
	const int postAllocSize = totalAlloc-mData.size();
	const int usedSize = size();
	bool shrinkPostAllocation = false;
	bool shrinkPreAllocation = false;
	if(totalAlloc>650000)
	{
		shrinkPostAllocation=postAllocSize>usedSize*1.5;
		shrinkPreAllocation=mPreallocsize*10>usedSize;
	}else if(totalAlloc>1000)
	{
		shrinkPostAllocation = postAllocSize>usedSize*5;
		shrinkPreAllocation = mPreallocsize>usedSize*1.5;
	}
	
	if(shrinkPreAllocation||shrinkPostAllocation)
		squeeze(shrinkPreAllocation, shrinkPostAllocation);
}


class CCurveData{
	public:
		CCurveData():t(0),key(0),value(0){}
		CCurveData(double t, double key, double value):t(t),key(key),value(value){}
		inline double sortKey()const{return t;}
		inline static CCurveData fromSortKey(double sortKey){return CCurveData(sortKey,0,0);}
		inline static bool sortKeyIsMainKey(){return false;}
		
		inline double mainKey()const {return key;}
		inline double mainValue()const{return value;}
		inline CRange valueRange()const{return CRange(value, value);}
		double t, key, value;
};

typedef CDataContainer<CCurveData> CCurveDataContainer;


class CGraphData
{
	public:
		CGraphData():key(0),value(0){ }
		CGraphData(double key, double value):key(key),value(value){}
		inline double sortKey()const{return key;}
		inline static CGraphData fromSortKey(double sortKey){return CGraphData(sortKey,0);}
		inline static bool sortKeyIsMainKey(){return true;}
		
		inline double mainKey()const{return key;}
		inline double mainValue()const{return value;}
		
		inline CRange valueRange()const{return CRange(value, value);}
		
		double key, value;
};

typedef CDataContainer<CGraphData> CGraphDataContainer;



class CGraph:public CAbstractPlottable1D<CGraphData>
{
	public:
		enum LineStyle{
			lsNone,
			lsLine,
			lsStepLeft,
			lsStepRight,
			lsStepCenter,
			lsImpluse
		};
		
		explicit CGraph(CPlotAxis* keyAxis, CPlotAxis* valueAxis);
		virtual ~CGraph();
		
		CSharedPointer<CGraphDataContainer> data()const{return mDataContainer; }
		WORD lineStyle()const{return mLineStyle;}
		CScatterStyle scatterStyle()const{return mScatterStyle;}
		int scatterSkip()const{return mScatterSkip;}
		CGraph* channelFillGraph()const{return mChannelFillGraph;}
		bool adptivSampling()const{return mAdaptiveSampling;}
		
		void setData(CSharedPointer<CGraphDataContainer> data);
		void setData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted= false);
		void setLineStyle(WORD ls);
		void setScatterStyle(const CScatterStyle& style);
		void setScatterSkip(int skip);
		void setChannelFillGraph(CGraph* targetGraph);
		void setAdaptiveSampling(bool enabled);
		
		void addData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted = false);
		void addData(double key, double value);
		virtual CRange getKeyRange(bool& foundRange, WORD inSignDomain=Plot::sdBoth)const;
		virtual CRange getValueRange(bool& foundRange, WORD inSignDomain=Plot::sdBoth, const CRange& inKeyRange=CRange())const;
		
		WORD mLineStyle;
		CScatterStyle mScatterStyle;
		int mScatterSkip;
		CGraph* mChannelFillGraph;
		bool mAdaptiveSampling;
		
		
		virtual void draw(CPlotPainter* painter);
		virtual void drawLegendIcon(CPlotPainter* painter, const CRectF& rect)const;
		
		virtual void drawFill(CPlotPainter* painter, CVector<CPointF>* lines)const;
		virtual void drawScatterPlot(CPlotPainter* painter, const CVector<CPointF>& scatters, const CScatterStyle& style)const;
		virtual void drawLinePlot(CPlotPainter* painter, const CVector<CPointF>& lines)const;
		virtual void drawImplusePlot(CPlotPainter* painter,const CVector<CPointF>& lines)const;
		
		virtual void getOptimizedLineData(CVector<CGraphData>* lineData, const CGraphDataContainer::const_iterator& begin, const CGraphDataContainer::const_iterator& end)const;
		
		virtual void getOptimizedScatterData(CVector<CGraphData>* scatterData, CGraphDataContainer::const_iterator begin,  CGraphDataContainer::const_iterator end)const;

		void getVisibleDataBounds(CGraphDataContainer::const_iterator& begin, CGraphDataContainer::const_iterator& end, const CDataRange& rangeRestriction)const;
		void getLines(CVector<CPointF>* lines, const CDataRange& dataRange)const;
		void getScatters(CVector<CPointF>* scatters, const CDataRange& dataRange)const;
		
		CVector<CPointF> dataToLines(const CVector<CGraphData>& data)const;
		CVector<CPointF> dataToStepLeftLines(const CVector<CGraphData>& data)const;
		CVector<CPointF> dataToStepRightLines(const CVector<CGraphData>& data)const;
		CVector<CPointF> dataToStepCenterLines(const CVector<CGraphData>& data)const;
		CVector<CPointF> dataToImpluseLines(const CVector<CGraphData>& data)const;
		
		void addFillBasePoints(CVector<CPointF>* lines)const;
		void removeFillBasePoints(CVector<CPointF>* lines)const;
		CPointF lowerFillBasePoint(double lowerKey)const;
		CPointF upperFillBasePoint(double upperKey)const;
		const CPolygonF getChannelFillPolygon(const CVector<CPointF>* lines)const;
		
		int findIndexBelowX(const CVector<CPointF>* data, double x)const;
		int findIndexAboveX(const CVector<CPointF>* data, double x)const;
		int findIndexBelowY(const CVector<CPointF>* data, double y)const;
		int findIndexAboveY(const CVector<CPointF>* data, double y)const;
		double pointDistance(const CPointF& pixelPoint, CGraphDataContainer::const_iterator& closestData)const;
		
};


class CBars;

class CBarsGroup{
	public:
		enum SpacingType{
			stAbsolute,
			stAxisRectRatio,
			stPlotCoords
		};
		
		CBarsGroup();
		virtual ~CBarsGroup();
		
		WORD spacingType()const{return mSpacingType;}
		double spacing()const{return mSpacing;}
		
		void setSpacingType(WORD spacingType);
		void setSpacing(double spacing);
		
		CList<CBars*> bars()const{return mBars;}
		CBars* bars(int index)const;
		int size()const{return mBars.size();}
		bool isEmpty()const{return mBars.isEmpty();}
		void clear();
		bool contains(CBars* bars)const{return mBars.contains(bars);}
		void append(CBars* bars);
		void insert(int i,CBars* bars);
		void remove(CBars* bars);
		
		WORD mSpacingType;
		double mSpacing;
		CList<CBars*> mBars;
		
		void registerBars(CBars* bars);
		void unRegisterBars(CBars* bars);
		
		double keyPixelOffset(const CBars* bars, double keyCoord);
		double getPixelSpacing(const CBars* bars, double keyCoord);
		
	private:
		DISABLE_COPY(CBarsGroup)
		friend class CBars;
		
};



class CBarsData
{
	public:
		CBarsData();
		CBarsData(double key, double value);
		
		inline double sortKey()const{return key;}
		inline static CBarsData fromSortKey(double sortKey){return CBarsData(sortKey,0);}
		inline static bool sortKeyIsMainKey(){return true;}
		inline double mainKey()const{return key;}
		inline double mainValue()const{return value;}
		
		inline CRange valueRange()const{return CRange(value,value);}
		double key, value;
};


typedef CDataContainer<CBarsData> CBarsDataContainer;

class CBars:public CAbstractPlottable1D<CBarsData>
{
	public:
		enum WidthType{
			wtAbsolute,
			wtAxisRectRatio,
			wtPlotCoords
		};
		
		explicit CBars(CPlotAxis* keyAxis, CPlotAxis* valueAxis);
		
		virtual ~CBars();
		
		double width()const{return mWidth;}
		WORD widthType()const{return mWidthType;}
		CBarsGroup* barsGroup()const{return mBarsGroup;}
		double baseValue()const{return mBaseValue;}
		double stackingGap()const{return mStackingGap;}
		CBars* barBelow()const{return mBarBelow;}
		CBars* barAbove()const{return mBarAbove;}
		CSharedPointer<CBarsDataContainer> data()const{return mDataContainer;}
		
		void setData(CSharedPointer<CBarsDataContainer> data);
		void setData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted=false);
		void setWidth(double width);
		void setWidthType(WORD widthType);
		void setBarsGroup(CBarsGroup* barsGroup);
		void setBaseValue(double baseValue);
		void setStackingGap(double pixels);
		
		void addData(const CVector<double>& keys,const CVector<double>& values, bool alreadySorted=false );
		void addData(double key, double value);
		void moveBelow(CBars* bars);
		void moveAbove(CBars* bars);
		
		virtual CRange getKeyRange(bool& foundRange, WORD inSignDomain=Plot::sdBoth)const;
		virtual CRange getValueRange(bool& foundRange, WORD inSignDomain=Plot::sdBoth,const CRange& inKeyRange=CRange())const;
		virtual CPointF dataPixelPosition(int index)const;
		
		double mWidth;
		WORD mWidthType;
		CBarsGroup* mBarsGroup;
		double mBaseValue;
		double mStackingGap;
		CBars* mBarAbove;
		CBars* mBarBelow;
		
		
		virtual void draw(CPlotPainter* painter);
		virtual void drawLegendIcon(CPlotPainter* painter, const CRectF& rect)const;
		
		void getVisibleDataBounds(CBarsDataContainer::const_iterator& begin, CBarsDataContainer::const_iterator& end)const;
		CRectF getBarRect(double key, double value)const;
		void 	getPixelWidth(double key, double& lower, double& upper)const;
		double getStackedBaseValue(double key, bool positive)const;
		static void connectBars(CBars* lower, CBars* upper);
		friend class CBarsGroup;
		friend class CLegend;
};



class CLayout;
class CMarginGroup;
class CLayoutElement:public CLayerable
{
	public:
	 enum UpdatePhase{
		 upPreparation,
		 upMargins,
		 upLayout
	 };
	 
	 explicit CLayoutElement();
	 virtual ~CLayoutElement();
	 
	 CLayout* layout() const {return mParentLayout;}
	 
	 CRect rect() const {return mRect;}
	 CRect outerRect() const {return mOuterRect; }
	 CMargin margins()const {return mMargins;}
	 CMargin minimumMargins()const{return mMinimumMargins;}
	 WORD autoMargins()const{return mAutoMargins;}
	 CSize minimumSize()const{return mMinimumSize;}
	 CSize maximumSize()const{return mMaximumSize;}
	 CMarginGroup* marginGroup(WORD side)
	 {
		 if(side>=Plot::msLeft&&side<Plot::msAll)
		 {
			 return mMarginGroups[side];
		 }
		 return 0;
	 }
	 
	 std::map<WORD, CMarginGroup*> marginGroups()const{return mMarginGroups;}
	 
	 void setOuterRect(const CRect& rect);
	 void setMargins(const CMargin& margins);
	 void setMarginGroup(WORD sides, CMarginGroup* group);
	 void setMinimumMargins(const CMargin& margins);
	 void setAutoMargins(WORD marginSides);
	 void setMinimumSize(const CSize& size);
	 void setMinimumSize(int width, int height);
	 void setMaximumSize(const CSize& size);
	 void setMaximumSize(int width, int height);
	 
	 virtual void update(WORD phase);
	 virtual CSize minimumSizeHint()const;
	 virtual CSize maximumSizeHint()const;
	 virtual CList<CLayoutElement*> elements(bool recursive)const;
	 
	 CLayout* mParentLayout;
	 CSize mMinimumSize, mMaximumSize;
	 CRect mRect, mOuterRect;
	 CMargin mMargins, mMinimumMargins;
	 WORD mAutoMargins;
	 
	 virtual int calculateAutoMargin(WORD side);
	 virtual void layoutChanged();
	 
	 virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const{UNUSED_PARAM(painter);}
	 virtual void draw(CPlotPainter* painter){UNUSED_PARAM(painter);}
	 
	 std::map<WORD ,CMarginGroup*> mMarginGroups;
	private:
		DISABLE_COPY(CLayoutElement)
		friend class CLayout;
};



class CMarginGroup{
	public:
		CMarginGroup();
		virtual ~CMarginGroup();
		
		CList<CLayoutElement*> elements(WORD side)
		{
			if(side>=Plot::msLeft&&side<Plot::msAll) 
				return mChildren[side]; 
			else 
				return CList<CLayoutElement*>();
		}
		bool isEmpty()const;
		void clear();
		
	protected:
		std::map<WORD, CList<CLayoutElement*> > mChildren;
		
		virtual int commonMargin(WORD side);
		void addChild(WORD side, CLayoutElement* element);
		void removeChild(WORD side, CLayoutElement* element);
		
	private:
		DISABLE_COPY(CMarginGroup)
		friend class CLayoutElement;
		
};


class CLayout:public CLayoutElement
{
	public:
		explicit CLayout();
		virtual void update(WORD phase);
		virtual CList<CLayoutElement*>elements(bool recursive)const;
		
		virtual int elementCount()const =0;
		virtual CLayoutElement* elementAt(int index)const=0;
		virtual CLayoutElement* takeAt(int index)=0;
		virtual bool take(CLayoutElement* element) =0;
		virtual void simplify();
		
		bool removeAt(int index);
		bool remove(CLayoutElement* element);
		void clear();
		
		virtual void updateLayout();
		
		void sizeConstraintsChanged()const;
		void adoptElement(CLayoutElement* e);
		void releaseElement(CLayoutElement* e);
		CVector<int> getSectionSizes(CVector<int> maxSizes, CVector<int> minSizes, CVector<double>stretchFactors, int totalSize)const;
		
	private:
		DISABLE_COPY(CLayout)
		friend class CLayoutElement;
};

class CLegend;

class CAbstractLegendItem:public CLayoutElement
{
	public:
		explicit	CAbstractLegendItem(CLegend* parent);
		
		CLegend* parentLegend()const{return mParentLegend;}
		PlotFont font()const{return mFont;}
		int textColor()const {return mTextColor;}
		int selectedTextColor()const {return mSelectedTextColor;}
		PlotFont selectedFont()const{return mSelectedFont;}
		bool selectable()const{return mSelectable;}
		bool selected()const{return mSelected;}
		
		void setFont(const PlotFont& font);
		void setTextColor(int color){mTextColor = color;};
		void setSelectedFont(const PlotFont& font);
		void setSelectedTextColor(int color){mSelectedTextColor = color;};
		void setSelectable(bool selectable);
		void setSelected(bool selected);
		
		void selectionChanged(bool selected);
		void selectableChanged(bool selectable);
		
		CLegend* mParentLegend;
		PlotFont mFont;
		PlotFont mSelectedFont;
		int mTextColor;
		int mSelectedTextColor;
		bool mSelectable, mSelected;
		
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		virtual CRect clipRect()const;
		virtual void draw(CPlotPainter* painter)=0;
		
	private:
		DISABLE_COPY(CAbstractLegendItem)
		friend class CLegend;
};

class CPlottableLegendItem:public CAbstractLegendItem
{
	public:
		CPlottableLegendItem(CLegend* parent, CAbstractPlottable* plottable);
		
		CAbstractPlottable* plottable(){return mPlottable;}
		
		CAbstractPlottable* mPlottable;
		
		virtual void draw(CPlotPainter* painter);
		virtual CSize minimumSizeHint()const;
		CPen getIconBorderPen()const;
		PlotFont getFont()const;
		int getTextColor()const;
};

class CLayoutGrid:public CLayout
{
	public:
		enum FillOrder{
			foRowsFirst,
			foColumnsFirst
		};
		
		explicit CLayoutGrid();
		virtual ~CLayoutGrid();
		
		int rowCount()const{return mElements.size();}
		int columnCount()const{return mElements.size()>0? mElements.first().size():0;}
		
		CList<double> columnStretchFactors()const{return mColumnStretchFactors;}
		CList<double> rowStretchFactors()const{return mRowStretchFactors;}
		int columnSpacing()const{return mColumnSpacing;}
		int rowSpacing()const{return mRowSpacing;}
		
		int wrap()const {return mWrap;}
		WORD fillOrder()const{return mFillOrder;}
		
		void setColumnStretchFactor(int column, double factor);
		void setColumnStretchFactors(const CList<double>& factors);
		void setRowStretchFactor(int row, double factor);
		void setRowStretchFactors(const CList<double> & factors);
		void setColumnSpacing(int pixels);
		void setRowSpacing(int pixels);
		void setWrap(int count);
		void setFillOrder(WORD order, bool rearrange=true);
		
		virtual void updateLayout();
		virtual int elementCount()const{return rowCount()*columnCount();}
		virtual CLayoutElement* elementAt(int index)const;
		virtual CLayoutElement* takeAt(int index);
		virtual bool take(CLayoutElement* elements);
		virtual CList<CLayoutElement*> elements(bool recursive)const;
		virtual void simplify();
		virtual CSize minimumSizeHint()const;
		virtual CSize maximumSizeHint()const;

		
		CLayoutElement* element(int row, int column)const;
		bool addElement(int row, int column, CLayoutElement* elemnt);
		bool addElement(CLayoutElement* element);
		bool hasElement(int row, int column);
		void expandTo(int newRowCount, int newColumnCount);
		void insertRow(int newIndex);
		void insertColumn(int newIndex);
		int rowColToIndex(int row, int column)const;
		void indexToRowCol(int index, int& row, int& column)const;
		
		CList<CList<CLayoutElement*> >mElements;
		
		CList<double> mColumnStretchFactors;
		CList<double> mRowStretchFactors;
		
		int mColumnSpacing, mRowSpacing;
		int mWrap;
		WORD mFillOrder;
		
		void getMinimumRowColSizes(CVector<int>* minColWidths, CVector<int>* minRowHeights)const;

		void getMaximumRowColSizes(CVector<int>* maxColWidths, CVector<int>* maxRowHeights)const;
	private:
		DISABLE_COPY(CLayoutGrid)
};

class CLegend:public CLayoutGrid
{
	public:
		enum SelectablePart{
			spNone =0x000,
			spLegendBox = 0x001,
			spItems = 0x002
		};
		
		explicit CLegend();
		virtual ~CLegend();
		
		
		CPen borderPen()const{return mBorderPen;}
		CBrush brush()const{return mBrush;}
		PlotFont font()const{return mFont;}
		
		int textColor()const {return mTextColor;}
		int selectedTextColor()const {return mSelectedTextColor;}
		
		CSize iconSize()const{return mIconSize;}
		int iconTextPadding()const{return mIconTextPadding;}
		CPen iconBorderPen()const{return mIconBorderPen;}
		WORD selectableParts()const{return mSelectableParts;}
		WORD selectedParts()const;
		CPen selectedBorderPen()const{return mSelectedBorderPen;}
		CPen selectedIconBorderPen()const{return mSelectedIconBorderPen;}
		CBrush selectedBrush()const{return mSelectedBrush;}
		PlotFont selectedFont()const{return mSelectedFont;}
		
		
		void setBorderPen(const CPen& pen);
		void setBrush(const CBrush& brush);
		void setFont(const PlotFont& font);
		void setTextColor(int color);
		void setIconSize(const CSize& size);
		void setIconSize(int width, int height);
		void setIconBorderPen(const CPen& pen);
		void setIconTextPadding(int padding);
		void setSelectableParts(WORD selectableParts);
		void setSelectedParts(WORD selectedParts);
		
		void setSelectedBorderPen(const CPen& pen);
		void setSelectedIconBorderPen(const CPen& pen);
		void setSelectedBrush(const CBrush& brush);
		void setSelectedFont(const PlotFont& font);
		void setSelectedTextColor(int color);
		
		CAbstractLegendItem* item(int index)const;
		CPlottableLegendItem* itemWithPlottable(const CAbstractPlottable* plottable)const;
		int itemCount()const;
		bool hasItem(CAbstractLegendItem* item)const;
		bool hasItemWithPlottable(const CAbstractPlottable* plottable)const;
		bool addItem(CAbstractLegendItem* item);
		bool removeItem(int index);
		bool removeItem(CAbstractLegendItem* item);
		void clearItems();
		CList<CAbstractLegendItem*> selectedItems()const;
		
		void selectionChanged(WORD parts);
		void selectableChanged(WORD parts);
		
	
		CPen mBorderPen, mIconBorderPen;
		CBrush	mBrush;
		PlotFont mFont;
		CSize mIconSize;
		int mIconTextPadding;
		WORD mSelectableParts, mSelectedParts;
		CPen mSelectedBorderPen, mSelectedIconBorderPen;
		CBrush mSelectedBrush;
		PlotFont mSelectedFont;
		int mTextColor;
		int mSelectedTextColor;
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		virtual void draw(CPlotPainter* painter);
		void drawRulerLabel();
		void placeRulerLabel();
		CPen getBorderPen()const;
		CBrush getBrush()const;
	private:
		DISABLE_COPY(CLegend)
		friend class CAbstractLegendItem;
};

class CCrossRuler:public CLayoutGrid
{
	public:
		explicit CCrossRuler(CAxisRect* axisRect=0);
		virtual ~CCrossRuler();
		void 	setAxisRect(CAxisRect* axisRect);
		std::string getTickLabel(CPlotAxis::AxisType type)const;
		CAxisPainterPrivate::TickLabelData getTickLabelData(CPlotAxis::AxisType type,const PlotFont& font, const std::string& text)const;
		void placeTickLabel(CPlotAxis::AxisType type,CPlotPainter* painter, double position, int distanceToAxis, const std::string& text, CSize* tickLabelsSize);
		void drawTickLabel(CPlotPainter* painter, double x, double y, const CAxisPainterPrivate::TickLabelData& labelData)const;
		
		CPen rulerPen()const{return mRulerPen;}
		CPen borderPen()const{return mBorderPen;}
		CBrush brush()const{return mBrush;}
		PlotFont font()const{return mFont;}
		
		void setBorderPen(const CPen& pen);
		void setBrush(const CBrush& brush);
		void setFont(const PlotFont& font);
		void setRulerPen(const CPen& pen);
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		virtual void draw(CPlotPainter* painter);
		
		
		
		void setPixelPoint(const CPoint& pt);
		CPoint pixelPoint()const{return m_pt;}
		inline double left()const
		{
			return m_left;
		}
		
		inline double right()const
		{
			return m_right;
		}
		
		inline double top()const
		{
			return m_top;
		}
		
		inline double bottom()const
		{
			return m_bottom;
		}
		
		double m_left;
		double m_right;
		double m_top;
		double m_bottom;
		CPoint m_pt;
		CAxisRect* mAxisRect;
		CPen mBorderPen;
		CPen mRulerPen;
		PlotFont mFont;
		CBrush mBrush;
	
	private:
		DISABLE_COPY(CCrossRuler);
};

class CLayoutInset:public CLayout
{
	public:
		enum InsetPlacement{
			ipFree,
			ipBorderAligned
		};
		explicit CLayoutInset();
		virtual ~CLayoutInset();
		
		WORD insetPlacement(int index)const;
		WORD insetAlignment(int index)const;
		CRectF insetRect(int index)const;
		
		void setInsetPlacement(int index, WORD placement);
		void setInsetAlignment(int index, WORD alignment);
		void setInsetRect(int index, const CRectF& rect);
		
		virtual void updateLayout();
		virtual int elementCount()const;
		virtual CLayoutElement* elementAt(int index)const;
		virtual CLayoutElement* takeAt(int index);
		virtual bool take(CLayoutElement* element);
		virtual void simplify(){}
		
		void addElement(CLayoutElement* element, WORD alignment);
		void addElement(CLayoutElement* element, const CRectF& rect);
		
		
		CList<CLayoutElement*> mElements;
		CList<WORD> mInsetPlacement;
		CList<WORD> mInsetAlignment;
		CList<CRectF> mInsetRect;
	private:
		DISABLE_COPY(CLayoutInset)
};












/*Implement Controls and Wrapper*/

class CtmPlot;

class CPlotAxisWrapper
{
	private:
		CPlotAxis* axis;
		const CtmPlot* 	plot;
		void detach(){axis=0;plot=0;}
		friend class CtmPlot;
		friend class CPlotGridWrapper;
	public:	
		explicit CPlotAxisWrapper(CPlotAxis* axis=0,const CtmPlot* parent=0);
		CPlotAxisWrapper(const CPlotAxisWrapper& other);
		CPlotAxisWrapper operator=(const CPlotAxisWrapper& other);
		
		//getters:
		std::string name()const;
		
		CRange range()const;
		double rangeLower()const;
		double rangeUpper()const;
		int labelPadding()const;
		int tickLabelPadding()const;
		bool rangeReversed()const;
		int axisOffset()const;
		bool ticks()const;
		int tickCount()const;
		
		//setters:
		void setName(const std::string& name);
		void setAxisColor(int color);
		void setAxisPenStyle(Gemini::PenStyle style);
		void setAxisWidth(int width);
		
		void setTickerColor(int color);
		void setSubTickerColor(int color);
		void setTickerWidth(int width);
		void setSubTickerWidth(int width);
		void setTickerPenStyle(Gemini::PenStyle style);
		void setSubTickerPenStyle(Gemini::PenStyle style);
		
		void setRange(double lower, double upper);
		void setRange(const CRange& range);
		void setRangeLower(double lower);
		void setRangeUpper(double upper);
		void moveRange(double diff);
		void scaleRange(double factor);
		void scaleRange(double factor, double center);
		void setRangeReversed(bool reversed);
		void setLabelPadding(int padding);
		void setTickLabelPadding(int padding);
		
		void setAxisOffset(int offset);
		
		void setLabelFont(int width, int height, int color);
		void setTickLabelFont(int width,int height, int color);
		
		void setNumberFormat(const std::string& format);
		void setNumberPrecision(int precision);
		void setTickLabelRotation(int angle);
		void setTickStepStrategy(CAxisTicker::TickStepStrategy strategy);
		void setScaleStyle(CPlotAxis::ScaleType type);
		void setTickLengthIn(int inside);
		void setTickLengthOut(int outside);
		void setTickLength(int inside, int outside=0);
		
		
		void setSubTickLengthIn(int inside);
		void setSubTickLengthOut(int outside);
		void setSubTickLength(int inside, int outside=0);
		
		void setLowerEnding(CLineEnding::EndingStyle style);
		void setUpperEnding(CLineEnding::EndingStyle style);
		void setLowerEnding(CLineEnding::EndingStyle style, int width, int length, bool inverted);
		void setUpperEnding(CLineEnding::EndingStyle style, int width, int length, bool inverted);
		void setTickCount(int count);
		void setTicks(bool enable);
		
		void setVisible(bool visible);
		bool visible()const;
		
		
		
};

class CLegendWrapper
{
	private:
		CLegend* legend;
		const CtmPlot* plot;
		void detach(){legend=0;plot=0;}
		friend class CtmPlot;
	public:	
		explicit CLegendWrapper(CLegend* legend=0,const CtmPlot* parent=0);
		CLegendWrapper(const CLegendWrapper& other);
		CLegendWrapper operator=(const CLegendWrapper& other);
		
		bool visible()const;
		CRect rect()const;
		
		//setters:
		void	setRect(const CRect& rect);
		void 	setBorderColor(int color);
		void 	setBorderPenWidth(int width);
		void 	setBorderPenStyle(Gemini::PenStyle style);
		void	setBorderFillColor(int color);
		void 	setBorderFillStyle(Gemini::BrushStyle style);
		void 	setFont(int width, int height, int color);
		void 	setFillOrder(CLayoutGrid::FillOrder order);
		
		void  	setVisible(bool visible);
		
		
};

class CGraphWrapper
{
	private:
		CGraph* graph;
		const CtmPlot* plot;
		void detach(){graph=0;plot=0;}
		std::string name()const;
		friend class CtmPlot;
	public:	
		explicit CGraphWrapper(CGraph* graph=0,const CtmPlot* parent=0);
		CGraphWrapper(const CGraphWrapper& other);
		CGraphWrapper operator=(const CGraphWrapper& other);
		
		//setters:
		void setData(const CVector<double>& keys, const CVector<double>& values);
		void addData(const CVector<double>& keys, const CVector<double>& values, bool alreadySorted = false);
		void addData(double key, double value);
		
		void setPenColor(int color);
		void setPenWidth(int width);
		void setPenStyle(Gemini::PenStyle style);
		void setFillColor(int color);
		void setFillStyle(Gemini::BrushStyle style);
		
		void setLineStyle(CGraph::LineStyle ls);
		void setScatterStyle(const CScatterStyle& style);
		void setScatterStyle(CScatterStyle::ScatterShape shape,int borderColor, int fillColor, int size);
		void setScatterStyle(const CImage& image);
		void setScatterStyle(const CPainterPath& path, int borderColor, int fillColor, int size);
		
		void setAntialiased(bool enabled);
		
		void setVisible(bool visible);
		bool visible()const;
		
};

class CBarsWrapper
{
	private:
		CBars* bars;
		const CtmPlot* plot;
		void detach(){bars=0;plot=0;}
		std::string name()const;
		friend class CtmPlot;
	public:	
		explicit CBarsWrapper(CBars* bars=0,const CtmPlot* parent=0);
		CBarsWrapper(const CBarsWrapper& other);
		CBarsWrapper operator=(const CBarsWrapper& other);
		
		//setters:
		void setData(const CVector<double>& keys, const CVector<double>& values);
		void setPenColor(int color);
		void setPenWidth(int width);
		void setPenStyle(Gemini::PenStyle style);
		void setFillColor(int color);
		void setFillStyle(Gemini::BrushStyle style);
		
		void setWidth(double width);
		void setWidthType(CBars::WidthType type);
		
		void setBaseValue(double baseValue);
		void setStackingGap(double pixels);
		
		void moveBelow(const CBarsWrapper& barsWrapper);
		void moveAbove(const CBarsWrapper& barsWrapper);
		
		void addData(const CVector<double>& keys,const CVector<double>& values, bool alreadySorted=true );
		void addData(double key, double value);
		
		void setVisible(bool visible);
		bool visible()const;
		
};

class CPlotGridWrapper
{
	private:
		CPlotGrid* grid;
		const CtmPlot* plot;
		void detach(){grid=0;plot=0;}
		friend class CtmPlot;
	public:	
		explicit CPlotGridWrapper(CPlotGrid* grid=0,const CtmPlot* parent=0);
		CPlotGridWrapper(const CPlotGridWrapper& other);
		CPlotGridWrapper operator=(const CPlotGridWrapper& other);
		
		bool 	visible()const;
		//setters:
		void	setVisible(bool visible);
		void 	setSubGridVisible(bool visible);
		
		void 	setZeroLineColor(int color);
		void	setGridColor(int color);
		void 	setSubGridColor(int color);
		void	setZeroLineWidth(int width);
		void	setGridWidth(int width);
		void	setSubGridWidth(int width);
		void	setGridPenStyle(Gemini::PenStyle style);
		void 	setSubGridPenStyle(Gemini::PenStyle style);
		void 	setZeroLinePenStyle(Gemini::PenStyle style);
		void 	setRelationAxis(CPlotAxisWrapper axis);
		void 	setRelationAxis(CPlotAxis::AxisType type);
		
		
		
};

class CBarsGroupWrapper
{
	private:
		CBarsGroup* group;
		const CtmPlot* plot;
		void detach(){group=0;plot=0;}
		friend class CtmPlot;
	public:	
		explicit CBarsGroupWrapper(CBarsGroup* group=0,const CtmPlot* parent=0);
		CBarsGroupWrapper(const CBarsGroupWrapper& other);
		CBarsGroupWrapper operator=(const CBarsGroupWrapper& other);
};

class CCrossRulerWrapper
{
	private:
		CCrossRuler*  ruler;
		const CtmPlot* plot;
		void detach(){ruler=0; plot=0;}
		friend class CtmPlot;
	public:
		explicit CCrossRulerWrapper(CCrossRuler* ruler=0, const CtmPlot* parent=0);
		CCrossRulerWrapper(const CCrossRulerWrapper& other);
		CCrossRulerWrapper operator=(const CCrossRulerWrapper& other);
		
		
		//getters:
		double left()const;
		double right()const;
		double top()const;
		double bottom()const;
		CPoint pixelPoint()const;
		bool 	visible()const;
		
		//setters:
		void setVisible(bool visible);
		void setBorderColor(int color);
		void setBorderStyle(Gemini::PenStyle style);
		void setBorderPenWidth(int width);
		void setFillColor(int color);
		void setFillStyle(Gemini::BrushStyle style);
		void setRulerColor(int color);
		void setRulerStyle(Gemini::PenStyle style);
		void setRulerPenWidth(int width);
		void setPixelPoint(const CPoint& pixel);
		void setFont(int width, int height, int color);
		
		
		
		
};


class CPlotPrivate
{
	public:
		enum PlotType{
			ptGraph,
			ptBars
		};
		
		CPlotPrivate();
		~CPlotPrivate();
		void init();
		void destroy();
		
		
		//plot:
		CSize size;
		int marginLeft;
		int marginRight;
		int marginTop;
		int marginBottom;
		
		BOOL transparent;
		BOOL touched;
		
	
		bool focus;
		int x;
		int y;
		
		CImage  image;
		CImage  bgImage;
		CImage  fmImage;
		CImage  crossImage;
		
		CList<CAbstractPlottable*> plottable;
		CList<WORD> plottableType;//0 代表graph //1 代表 bars
		CLegend* legend;
		CPlotGrid* horzGrid;
		CPlotGrid* vertGrid;
		CAxisRect* axisRect;
		CBarsGroup* barsGroup;
		CCrossRuler* crossRuler;
		
		
		//axis:
		int axisColor;
		int tickerColor;
		int subTickerColor;
		
		int axisPenWidth;
		int tickerPenWidth;
		int subTickerPenWidth;
		
		WORD axisVisible;	//left,right,top,bottom
		WORD tickerVisible; //left,right,top,bottom
		WORD subtickerVisible;//left,right,top,bottom
		double leftTickerRangeUpper;
		double leftTickerRangeLower;
		
		double rightTickerRangeUpper;
		double rightTickerRangeLower;
		
		double topTickerRangeUpper;
		double topTickerRangeLower;
		
		double bottomTickerRangeUpper;
		double bottomTickerRangeLower;
		
		int leftLabelPadding;
		int rightLabelPadding;
		int topLabelPadding;
		int bottomLabelPadding;
		
		int leftTickLabelPadding;
		int rightTickLabelPadding;
		int topTickLabelPadding;
		int bottomTickLabelPadding;
		
		//label-name 数据类型为字串类型的在plot.h中定义
		
		
		int leftAxisOffset;
		int rightAxisOffset;
		int topAxisOffset;
		int bottomAxisOffset;
		
		
		PlotFont labelFont;
		PlotFont tickLabelFont;
		
		//number-format数据类型为字串类型在plot.h中定义
		
		int leftNumberPrecision;
		int rightNumberPrecision;
		int topNumberPrecision;
		int bottomNumberPrecision;
		WORD scaleType; //left,right,top,bottom
		
		WORD lowerEndingStyle;
		WORD upperEndingStyle;
		
		int lefTickLabelRotation;
		int rightTickLabelRotation;
		int topTickLabelRotation;
		int bottomTickLabelRotation;
		
		int leftTickerLengIn;
		int leftTickerLengout;
		int rightTickerLengIn;
		int rightTickerLengout;
		int topTickerLengIn;
		int topTickerLengout;
		int bottomTickerLengIn;
		int bottomTickerLengout;
		
		
		int leftSubtickerLengthIn;
		int leftSubtickerLengthOut;
		int rightSubtickerLengthIn;
		int rightSubtickerLengthOut;
		int topSubtickerLengthIn;
		int topSubtickerLengthOut;
		int bottomSubtickerLengthIn;
		int bttomSubtickerLengthOut;
	
		
		//legend:
		int legendLeft;
		int legendTop;
		int legendWidth;
		int legendHeight;
		
		PlotFont legendFont;
		
		int legendBorderColor;
		int legendBorderWidth;
		int legendFillColor;
		int oldBgc;
		
		//32bit:每个bit位代表元件属性是否发生变化
		union
		{
			DWORD all;
			struct
			{
				DWORD margin:1;
				DWORD gridBase:1;
				DWORD axisBase:1;
				DWORD graphBase:1;
				DWORD barsBase:1;
				DWORD legendBase:1;
				DWORD reverse0:1;
				DWORD reverse1:1;
				DWORD rulerBase:1;
				DWORD reserve:23;
			}changed;
		}properties;
		
		//注:由于脚本属性个数的限制，可将更多特性在代码逻辑中实现

};




		

