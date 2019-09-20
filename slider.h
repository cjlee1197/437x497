/*Created by J.Wong 2017/1/13 16:53:30*/

#pragma once

#include "wnd.h"
#include "wndx2.h"

class CSliderPrivate;
class CPoint;
const int SLIDER_MAXDBNUM=10;

class CtmSlider:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmSlider)
	public:
		CtmSlider();
		~CtmSlider();
		
	public:
		void    Show();
		void    Update();
		void    Clear();
		BOOL 	CreateA();
		void 	SetPropertys();
		WORD    OnKey(WORD wKey);
		
		void   	OnGetFocus();
		void    OnLoseFocus();
		
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		
		/*
		*	0: Horizontal
		*	1: Vertcal
		*/
		void 	SetOrientation(int orientation);
		int		GetOrientation()const;
		void	SetFlowCap(bool cap);
		void	SetSlotCap(bool cap);
		bool 	IsShowFlowCap()const;
		bool 	IsShowSlotCap()const;
		/*
		*None=0x00,
		*Above=0x01,
		*Below=0x02,
		*Left=0x010,
		*Right=0x20,
		*BothSides=0x33
		*/
		
		void 	SetScaleStyle(int style);
		int 	GetScaleStyle()const;
		void 	SetScale(bool scale);
		bool	IsShowScale()const;
		
		/*
		*LeftToRight=0
		*RightToLeft=1
		*TopToBottom=2
		*BottomToTop=3	
		*/
		void	SetFlowDirection(int direction);
		int		GetFlowDirection()const;
		
		void 	SetOutline(bool outline);
		bool	IsShowOutline()const;
		
		void		SetScaleMin(float min);
		void		SetScaleMax(float max);
		void		SetScale(float min, float max);
		void		SetScaleDiv(int div);
		float		GetScaleMax()const;
		float		GetScaleMin()const;
		int 		GetScaleDiv()const;
		
		void		SetSlotColor(uint color);
		void 		SetFlowColor(uint color);
		void		SetColor(uint slotColor, uint flowColor);
		uint		GetSlotColor()const;
		uint		GetFlowColor()const;

		void		SetValue(float value);
		void		SetMax(float max);
		void		SetMin(float min);
		float		GetValue()const;
		float		GetMin()const;
		float		GetMax()const;		

		void		SetTickInterVal(float ival);
		float		GetTickInterVal()const;		
		
	private:
		CSliderPrivate* d;
		
		bool IsSlotChanged(CSliderPrivate* d);
		bool IsNeedResize(CSliderPrivate* d);
		bool IsPenChanged(CSliderPrivate* d);
		bool IsScaleChanged(CSliderPrivate* d);
		bool IsOrientationChanged(CSliderPrivate* d);
		bool IsTransparentChanged(CSliderPrivate* d);
		
		bool TouchAreaChanged(CSliderPrivate* d, const CPoint& pt);
		void PenChanged(CSliderPrivate* d);
		void Resize(CSliderPrivate* d);
		
		void GetDBInfo(CSliderPrivate* d);
		void SyncDBValue(CSliderPrivate* d);
		bool IsOverLimited(CSliderPrivate* d);
		
		
		void ResetPrompt(CSliderPrivate* d);
		void Prepare(CSliderPrivate* d);
		
		
		//SetPropInfo属性指针类型不宜放在privatedata类中
		char* pszPrompt;
		char* pszPromptLan;
		char* pszPromptID;
		
		char* dbid[SLIDER_MAXDBNUM];
		char* maxdbid;
		char* mindbid;
			
};