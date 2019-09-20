/*Created by J.Wong 2017/04/20*/
#pragma once
#include "plot_p.h"


class CVernier:public CLayoutGrid
{
	public:
		enum VernierType{
			vtSingle,
			vtDouble
		};
		explicit CVernier(CAxisRect* axisRect=0);
		virtual ~CVernier();
		void 	setAxisRect(CAxisRect* axisRect);
		std::string getTickLabel(CPlotAxis::AxisType type)const;
		CAxisPainterPrivate::TickLabelData getTickLabelData(CPlotAxis::AxisType type,const PlotFont& font, const std::string& text)const;
		void placeTickLabel(CPlotAxis::AxisType type,CPlotPainter* painter, double position, int distanceToAxis, const std::string& text, CSize* tickLabelsSize);
		void drawTickLabel(CPlotPainter* painter, double x, double y, const CAxisPainterPrivate::TickLabelData& labelData)const;
		
		CPen vernierPen()const{return mVernierPen;}
		CPen borderPen()const{return mBorderPen;}
		CBrush brush()const{return mBrush;}
		PlotFont font()const{return mFont;}
		
		void setBorderPen(const CPen& pen);
		void setBrush(const CBrush& brush);
		void setFont(const PlotFont& font);
		void setVernierPen(const CPen& pen);
		virtual void applyDefaultAntialiasingHint(CPlotPainter* painter)const;
		virtual void draw(CPlotPainter* painter);
		
		
		WORD orientation()const{return mOrientation;}
		WORD vernierType()const{return mType;}
		void setOrientation(WORD orientation);
		void setVernierType(WORD type);
		
		void setSingleValue(double value);
		void setDoubleValue(double value);
		
		void 	setSinglePixelPoint(const CPoint& point);
		void 	setDoublePixelPoint(const CPoint& point);
		CPoint  singlePixelPoint()const{return mSinglePixelPoint;}
		CPoint  doublePixelPoint()const{return mDoublePixelPoint;}
		
		double singleValue()const{return value1;};
		double doubleValue()const;
		
		
		WORD  mType;//单线or双线
		WORD  mOrientation;//水平or垂直
		CAxisRect* mAxisRect;
		CPen mBorderPen;
		CPen mVernierPen;
		PlotFont mFont;
		CBrush mBrush;
		CPoint mSinglePixelPoint;
		CPoint mDoublePixelPoint;
		
		double value1, value2;
		
		
	private:
		DISABLE_COPY(CVernier);
};

class CVernierPrivate{
	
public:	
	CVernierPrivate();
	virtual ~CVernierPrivate();
	CImage fmImage;
	CImage bgImage;
	CImage image;
	
	//axis:
	CSize size;
	int marginLeft;
	int marginRight;
	int marginTop;
	int marginBottom;
	
	void init();
	void destroy();
	
	BOOL transparent;
	BOOL touched;
		
	
	bool focus;
	int x;
	int y;
	
	CAxisRect* axisRect;
	CVernier* vernier;
	
	//32bit:每个bit位代表元件属性是否发生变化
	union
	{
		DWORD all;
		struct
		{
			DWORD margin:1;
			DWORD axisBase:1;
			DWORD valueBase:1;
			DWORD rulerBase:1;
			DWORD reserve:28;
		}changed;
	}properties;
	
	
	double value;
	double exValue;
	
	double rangeLower;
	double rangeUpper;
	
	double exRangeLower;
	double exRangeUpper;
	
	//双游标or单游标
	WORD vernierType;
	//水平or垂直方向尺子
	WORD orientation;
	
	int	oldBgc;
	
	
	//helper properties
	int borderColor;
	int borderWidth;
	int textBackground;
		
	int vernierColor;
	int vernierWidth;
	
	int fontWidth;
	int fontHeight;
	int fontColor;
	
	
	//axis:
	BOOL rangeReversed;
	int tickCount;
	int tickLabelPadding;
	int axisColor;
	int axisWidth;
	int tickColor;
	int tickWidth;
	int subTickColor;
	int subTickWidth;
	int tickLabelFontWidth;
	int tickLabelFontHeight;
	int tickLabelFontColor;
	int tickLengthIn;
	int subTickLengthIn;
	
	int numberPrecision;
	int exNumberPrecision;
	
	
	
};