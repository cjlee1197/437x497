/*Created by J.Wong 2017/03/13*/
#pragma once
#include "wnd.h"
#include "wndx2.h"
#include <guilite/IGui>
class CBars;
class CGraph;
class CLegend;
class CPlotGrid;
class CPlotAxis;
class CPlotPrivate;


class CBarsGroupWrapper;
class CBarsWrapper;
class CGraphWrapper;
class CLegendWrapper;
class CPlotGridWrapper;
class CPlotAxisWrapper;
class CCrossRulerWrapper;

class CtmPlot:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmPlot)
	public:
		CtmPlot();
		~CtmPlot();
		
		CPlotAxisWrapper 	GetAxis(CPlotAxis::AxisType type)const;	//left,top, bottom, right
		CLegendWrapper 		GetLegend()const;
		CPlotGridWrapper	GetGrid(Gemini::Orientation type)const;	//horizontal, vertical
		CBarsGroupWrapper	GetBarsGroup()const;
		CCrossRulerWrapper  GetCrossRuler()const;
		
		CBarsWrapper 	 	GetBars(int index)const;
		CGraphWrapper		GetGraph(int index)const;
		
		CBarsWrapper	 	GetBars(const std::string& name)const;
		CGraphWrapper  	 	GetGraph(const std::string& name)const;
		
		
		void 	AddBars(const std::string& name, CPlotAxis::AxisType keyType=CPlotAxis::atBottom, CPlotAxis::AxisType valueType=CPlotAxis::atLeft);
		void	AddGraph(const std::string& name, CPlotAxis::AxisType keyType=CPlotAxis::atBottom, CPlotAxis::AxisType valueType=CPlotAxis::atLeft);
		
		bool 	RemoveBars(CBarsWrapper&	bars)const;
		bool 	RemoveGraph(CGraphWrapper& graph)const;
	
		
		
		
		
		
		void    Show();
		void    Update();
		void    Clear();
		BOOL 	CreateA();
		void 	SetPropertys();
		WORD    OnKey(WORD wKey);
		
		void   	OnGetFocus();
		void    OnLoseFocus();
		
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		
		CPlotPrivate* d_func()const{return d;}
		
	private:
		CPlotPrivate* d;
		
		char* leftAxisLabelName;
		char* rightAxisLabelName;
		char* topAxisLabelName;
		char* bottomAxisLabelName;
		
		int   marginLeft;
		int   marginRight;
		int   marginBottom;
		int   marginTop;
		
	
		
		
		int   legendLeft;
		int   legendTop;
		int   legendHeight;
		int   legendWidth;
		
		BOOL   transparent;
		
		
	
		
		bool 	RemoveBars(const std::string& name)const;
		bool 	RemoveGraph(const std::string& name)const;
		void 	Resize(CPlotPrivate* d);
		bool	IsNeedResize(CPlotPrivate* d);
		bool 	IsTransParentChanged(CPlotPrivate* d);
		
		void 	SetMarginLeft(int left);
		void 	SetMarginRight(int right);
		void 	SetMarginTop(int top);
		void 	SetMarginBottom(int bottom);
		void 	SetMargin(const CMargin& margin);
		
		int 	GetMarginLeft()const;
		int 	GetMarginRight()const;
		int 	GetMarginTop()const;
		int 	GetMarginBottom()const;
		CMargin GetMargin()const;
		
		void 	SetLegendLeft(int left);
		void 	SetLegendTop(int top);
		void 	SetLegendWidth(int width);
		void 	SetLegendHeight(int height);
		void 	SetLegendRect(const CRect& rect);
		

		
		int 	GetLegendLeft()const;
		int 	GetLegendTop()const;
		int 	GetLegendWidth()const;
		int 	GetLegendHeight()const;
		
		
		
		void   CheckProperties();
		void   ClearPropertiesBit();
		
		
		
		
		
		
		
};