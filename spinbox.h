/*Created by J.Wong 2017/2/13 11:28:41*/
#pragma once
#include "wnd.h"
#include "wndx2.h"

class CSpinBoxPrivate;
class CPoint;

const int SPINBOX_MAXDBNUM=10;
class CtmSpinBox:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmSpinBox);
	
	public:
		CtmSpinBox();
		~CtmSpinBox();
	public:
		void	Show();
		void	Update();
		void	Clear();
		BOOL	CreateA();
		void	SetPropertys();
		WORD	OnKey(WORD wKey);
		WORD	GetDisplayBase();
		bool 	InputBoxContains(CPoint& pt);
		
		void	OnGetFocus();
		void	OnLoseFocus();
		void	WndProc(int message,WPARAM wParam, LPARAM lParam);
	protected:
		CSpinBoxPrivate* d;
		bool IsNeedResize(CSpinBoxPrivate* d);
		bool IsTransparentChanged(CSpinBoxPrivate* d);
		int TouchAreaChanged(CSpinBoxPrivate* d, const CPoint& pt);
		bool ShapeChanged(CSpinBoxPrivate* d);
		bool BackGroundChanged(CSpinBoxPrivate* d);
		void Resize(CSpinBoxPrivate* d);
		
		virtual void  EnableCursor(BOOL bEnable);
		virtual	void  DisplayCursor(BOOL bOn);
		virtual	BOOL  GetWaitStatus();
		virtual	void  CursorWait(BOOL bWait);
		virtual bool  ProcessOtherKey(WORD wKey);
		virtual void  SetCursorPosition(char acKey);
				void  ResetPrompt(CSpinBoxPrivate* d);
		virtual void  Prepare(CSpinBoxPrivate* d);
		virtual void  SyncDBValue(CSpinBoxPrivate* d);
		virtual bool  IsOverLimited(CSpinBoxPrivate* d);
		virtual bool  BackSpace(CSpinBoxPrivate* d);
				bool  IsEnabledChanged(CSpinBoxPrivate* d);
				void  GetDBInfo(CSpinBoxPrivate* d);
	protected:
	
		char* 	dbid[SPINBOX_MAXDBNUM];
		char* 	maxdbid;
		char* 	mindbid;
		
		char* pszPrompt;
		char* pszPromptLan;
		char* pszPromptID;
		char* suffix;
};

class CtmDoubleSpinBox:public CtmSpinBox
{
	DECLARE_DYNCREATE(CtmDoubleSpinBox);
	public:
		CtmDoubleSpinBox();
		~CtmDoubleSpinBox();
		void	Show();
		void	Update();
		void	Clear();
		BOOL	CreateA();
		void	SetPropertys();
		WORD	OnKey(WORD wKey);
		
		void	OnGetFocus();
		void	OnLoseFocus();
		void	WndProc(int message,WPARAM wParam, LPARAM lParam);
	protected:
		virtual void  SetCursorPosition(char acKey);
		virtual void  Prepare(CSpinBoxPrivate* d);
		virtual void  SyncDBValue(CSpinBoxPrivate* d);
		virtual bool  IsOverLimited(CSpinBoxPrivate* d);
		virtual bool  ProcessOtherKey(WORD wKey);
		virtual bool  BackSpace(CSpinBoxPrivate* d);		
};

