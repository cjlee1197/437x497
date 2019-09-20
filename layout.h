/*Created by J.Wong 2017/03/01*/
#pragma once

#include "wnd.h"
#include "wndx2.h"

class CLayoutPrivate;
class CPoint;

class CtmLayout:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmLayout);
	
	public:
		CtmLayout();
		~CtmLayout();
	public:
		void	Show();
		void	Update();
		void	Clear();
		BOOL	CreateA();
		void	SetPropertys();
		WORD	OnKey(WORD wKey);
		
		void	OnGetFocus();
		void	OnLoseFocus();
		void	WndProc(int message,WPARAM wParam, LPARAM lParam);
		
		virtual void addWnd(CtmWnd* wnd,bool autoDestroy=false);
		virtual void addWnds(CtmWnd** wnds, int nums=1,bool autoDestroy=false);
		virtual void addLayout(CtmLayout* layout,bool autoDestroy=false);
		virtual void addLayouts(CtmLayout** layouts, int nums=1, bool autoDestroy= false);

		
		virtual bool removeWnd(CtmWnd* wnd);
		virtual bool removeWnd(int pos=0);
		virtual bool removeWnds(char* className);
		virtual bool removeWnd(char* Name);
		
		virtual bool removeLayout(CtmLayout* layout);
		virtual bool removeLayout(int pos=0);
		virtual CtmWnd* getWidget(char* Name);
		virtual CtmLayout* getLayout(char* Name);
		
		virtual void SetSpace(int pos, WORD space);
	
	private:
		CLayoutPrivate* d;
		bool IsNeedResize(CLayoutPrivate* d);
		void Resize(CLayoutPrivate* d);
		bool ElementsChanged(CLayoutPrivate* d);
		bool ElementsCoordinateChanged(CLayoutPrivate* d);
		CtmWnd* TouchAreaControls(CLayoutPrivate* d, const CPoint& pt);
		
		
		
		
	
};


// class CtmHLayout:public CtmLayout
// {
	// /*---------------------------------------------------------------------------+
	// |	        Constructor and destructor                                       |
	// +---------------------------------------------------------------------------*/	
	// DECLARE_DYNCREATE(CtmHLayout);
	
	// private:
		// CLayoutPrivate* d;
// };


// class CtmVLayout:public CtmLayout
// {
	// /*---------------------------------------------------------------------------+
	// |	        Constructor and destructor                                       |
	// +---------------------------------------------------------------------------*/	
	// DECLARE_DYNCREATE(CtmVLayout);
	
	// private:
		// CLayoutPrivate* d;
// };

