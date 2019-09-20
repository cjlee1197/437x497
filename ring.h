/*Created by J.Wong 2017/1/18 11:20:08*/
#pragma once
#include "wnd.h"
#include "wndx2.h"

class CRingPrivate;

class CtmRing:public CtmWnd
{
	DECLARE_DYNCREATE(CtmRing)
public:
	CtmRing();
	~CtmRing();
	
	void    Show();
	void    Update();
	void    Clear();
	BOOL 	CreateA();
	void 	SetPropertys();
	WORD    OnKey(WORD wKey);
		
	void   	OnGetFocus();
	void    OnLoseFocus();
		
	void	WndProc(int message, WPARAM wParam, LPARAM lParam);
	
	virtual void	SetPercent(float percent);
	virtual float	GetPrercent()const;
	
	virtual void	SetColor(uint color);
	virtual uint	GetColor()const;
	
	virtual void	SetLeftColor(uint color);
	virtual uint	GetLeftColor()const;
	
	virtual void	SetTickInterval(float interval);
	virtual float 	GetTickInterval()const;
	
	virtual void	SetClockwise(BOOL clockwise);
	virtual BOOL	GetClockwise()const;
	
	
	
protected:
	CRingPrivate* d;
	
protected:	
	bool IsNeedResize(CRingPrivate* d);
	void Resize(CRingPrivate* d);
	
	bool IsPenChanged(CRingPrivate* d);
	void PenChanged(CRingPrivate* d);
	
	bool IsTransparentChanged(CRingPrivate* d);
	bool TouchAreaChanged(CRingPrivate* d, const CPoint& pt);
	void GetDBInfo(CRingPrivate* d);
	void SyncDBValue(CRingPrivate* d);
	bool IsOverLimited(CRingPrivate* d);
	bool IsOverLastLimited(CRingPrivate* d);
	void ResetPrompt(CRingPrivate* d);
	void Prepare(CRingPrivate* d);
	
	char* dbid;
	char* maxdbid;
	char* mindbid;
	char* lastdbid;
	char* lastmaxdbid;
	char* lastmindbid;
	char* pszPrompt;
	char* pszPromptLan;
	char* pszPromptID;
};

class CRingPrivate;
class CtmRings:public CtmRing
{
	DECLARE_DYNCREATE(CtmRings)
public:
	CtmRings();
	~CtmRings();
	
	void    Show();
	void 	SetPropertys();
		
	
	void	SetLastColor(uint color);
	uint	GetLastColor()const;
	void	SetLastPercent(float percent);
	float 	GetLastPercent()const;
protected:
	bool IsPenChanged(CRingPrivate* d);
	void PenChanged(CRingPrivate* d);

};