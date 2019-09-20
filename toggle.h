/*Created by J.Wong 2017/2/6 9:04:04*/
#pragma once
#include "wnd.h"
#include "wndx2.h"

class CTogglePrivate;
class CPoint;

class CtmToggle:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmToggle)
	public:
		CtmToggle();
		~CtmToggle();

		void    Show();
		void    Update();
		void    Clear();
		BOOL 	CreateA();
		void 	SetPropertys();
		WORD    OnKey(WORD wKey);
		
		void   	OnGetFocus();
		void    OnLoseFocus();
		
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
	
		void	SetSelectedColor(uint color);
		uint	GetSelectedColor()const;
		void 	SetUnselectedColor(uint color);
		uint	GetUnselectedColor()const;
		void 	SetOutlineColor(uint color);
		uint	GetOutlineColor()const;
		void	SetStatus(bool status);
		bool	GetStatus()const;
	private:
		CTogglePrivate* d;
		bool IsTransparentChanged(CTogglePrivate* d);
		bool IsNeedResize(CTogglePrivate* d);
		bool IsOutlineChanged(CTogglePrivate* d);
		
		bool TouchAreaChanged(CTogglePrivate* d, const CPoint& pt);
		void Resize(CTogglePrivate* d);
		
		void GetDBInfo(CTogglePrivate* d);
		void SyncDBValue(CTogglePrivate* d);
		void ResetPrompt(CTogglePrivate* d);
		void Prepare(CTogglePrivate* d);
		bool GetImage(CTogglePrivate* d);
		
		char* 	dbid;
		char* 	pszPromptID;
		char* 	pszPromptLan;
		char* 	pszPrompt;
		
		char*   selectImagePath;
		char*   unSelectImagePath;

};