/*Created by J.Wong 2017/05/08*/
#pragma once
#include "wnd.h"
#include "wndx2.h"
#include <guilite/IGui>

class CGridDataWrapper;

class CtmDataGrid:public CtmWnd
{
	DECLARE_DYNCREATE(CtmDataGrid)
	public:
		
		CtmDataGrid();
		~CtmDataGrid();
		
		
		CGridDataWrapper GetDataGrid()const;
		
		void    Show();
		void    Update();
		void    Clear();
		BOOL 	CreateA();
		void 	SetPropertys();
		WORD    OnKey(WORD wKey);
		
		void   	OnGetFocus();
		void    OnLoseFocus();
		
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		CDataGridPrivate* d_func()const;
		
	private:
		CDataGridPrivate* d;
		
		int   marginLeft;
		int   marginRight;
		int   marginBottom;
		int   marginTop;
		BOOL  transparent;
		
		
		void   CheckPropertiesChanged();
		void   ClearPropertiesBit();
		
		void 	Resize(CDataGridPrivate* d);
		bool	IsNeedResize(CDataGridPrivate* d);
		bool 	IsTransParentChanged(CDataGridPrivate* d);
		
		void 	CheckPenChanged(CDataGridPrivate* d);
		
		void 	CheckPenStyleChanged(CDataGridPrivate* d);
		void 	CheckPenWidthChanged(CDataGridPrivate* d);
		void 	CheckPenColorChanged(CDataGridPrivate* d);
		
		void 	CheckHeaderChanged(CDataGridPrivate* d);
		void 	CheckHorzScrollVisibleChanged(CDataGridPrivate* d);
		void 	CheckVertScrollVisibleChanged(CDataGridPrivate* d);
		void 	CheckScrollVisibleChanged(CDataGridPrivate* d);

		void 	CheckVerzGridVisibleChanged(CDataGridPrivate* d);
		void 	CheckHorzGridVisibleChanged(CDataGridPrivate* d);
		void 	CheckGridVisibleChanged(CDataGridPrivate* d);
		void 	CheckColCountChanged(CDataGridPrivate* d);
		void 	CheckRowCountChanged(CDataGridPrivate* d);
		void 	CheckGridCountChanged(CDataGridPrivate* d);
		void 	CheckMarginChanged(CDataGridPrivate* d);
		void 	CheckPaddingChanged(CDataGridPrivate* d);
		
		void 	CheckVertScrollMaxLimitChanged(CDataGridPrivate* d);
		void 	CheckHorzScrollMaxLimitChanged(CDataGridPrivate* d);
		void 	CheckScrollMaxLimitChanged(CDataGridPrivate* d);

};