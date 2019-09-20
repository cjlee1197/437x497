/*Created by J.Wong 2017/1/20 9:54:09*/
#pragma once
#include "wnd.h"
#include "wndx2.h"

class	CDialPrivate;

class CtmDial:public CtmWnd
{
	DECLARE_DYNCREATE(CtmDial)
public:
	CtmDial();
	~CtmDial();
	
	void    Show();
	void    Update();
	void    Clear();
	BOOL 	CreateA();
	void 	SetPropertys();
	WORD    OnKey(WORD wKey);
		
	void   	OnGetFocus();
	void    OnLoseFocus();
		
	void	WndProc(int message, WPARAM wParam, LPARAM lParam);

	void	SetValue(float value);
	float	GetValue()const;
	
	void	SetMax(float max);
	float	GetMax()const;
	
	void 	SetMin(float min);
	float	GetMin()const;
	
	void	SetOuterColor(uint color);
	uint	GetOuterColor()const;
	void	SetInnerColor(uint color);
	uint	GetInnerColor()const;
	void	SetColor(uint color);
	uint	GetColor()const;
	
	void	SetClockwise(BOOL clockwise);
	BOOL	GetClockwise()const;
	
	void	SetSpanAngle(float angle);
	float	GetSpanAngle()const;
	
	void	SetTickInterval(float tick);
	float	GetTickInterval()const;
	
	void	SetDialDiv(WORD div);
	WORD	GetDialDiv()const;
	

private:
	CDialPrivate* d;

	bool IsNeedResize(CDialPrivate* d);
	void Resize(CDialPrivate* d);
	
	bool IsTransparentChanged(CDialPrivate* d);
	bool IsClockwiseChanged(CDialPrivate* d);
	bool IsSpanAngleChanged(CDialPrivate* d);
	bool TouchAreaChanged(CDialPrivate* d, const CPoint& pt);
	
	void GetDBInfo(CDialPrivate* d);
	void SyncDBValue(CDialPrivate* d);
	bool IsOverLimited(CDialPrivate* d);
	void ResetPrompt(CDialPrivate* d);
	void Prepare(CDialPrivate* d);
	
	char* dbid;
	char* maxdbid;
	char* mindbid;
	
	char* pszPrompt;
	char* pszPromptLan;
	char* pszPromptID;
};