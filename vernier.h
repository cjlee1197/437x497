/*Created by J.Wong 2017/04/20*/

#pragma once
#include "wnd.h"
#include "wndx2.h"
#include <guilite/IGui>

class CVernierPrivate;
class CVernier;

class CtmVernier:public CtmWnd
{
	DECLARE_DYNCREATE(CtmVernier)
	public:
		CtmVernier();
		virtual ~CtmVernier();
		
		void    Show();
		void    Update();
		void    Clear();
		BOOL 	CreateA();
		void 	SetPropertys();
		WORD    OnKey(WORD wKey);
		
		void   	OnGetFocus();
		void    OnLoseFocus();
		
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		
	private:
		CVernierPrivate* d;

		char* numberFormat;
		BOOL   transparent;
		
		
		void 	Resize(CVernierPrivate* d);
		bool	IsNeedResize(CVernierPrivate* d);
		bool 	IsTransParentChanged(CVernierPrivate* d);
		
		
		
		void SetMargin(int left, int top, int right, int bottom);
		void SetVernierType(WORD type);
		void SetOrientation(WORD orientation);
		void SetBorderPen(int width, int color);
		void SetTextBackground(int color);
		void SetVernierPen(int width, int color);
		void SetFont(int width, int height, int color);
		void SetValue(double value);
		void SetExValue(double value);
		void SetRange(double rangeLower, double rangeUpper);
		void SetExRange(double rangeLower, double rangeUpper);
		void SetRangeReversed(BOOL reversed);
		void SetTickCount(int count);
		void SetTickLabelPadding(int padding);
		void SetTickLabelFont(int width, int height, int color);
		void SetAxisPen(int width, int color);
		void SetTickPen(int width, int color);
		void SetSubTickPen(int width, int color);
		void SetNumberPrecision(int precision, int exPrecision);
		void SetNumberFormat(std::string format);
		void SetTickLength(int length);
		void SetSubTickLength(int length);
		void GetDBInfo(CVernierPrivate* d);
		
		void CheckPropertiesChanged();
		void ClearPropertiesBit();
		
		
		char* dbid[2];
		
		char* maxid[2];
		char* minid[2];
		BOOL maxdb[2];
		BOOL mindb[2];
		
};