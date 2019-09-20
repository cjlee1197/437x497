#ifndef __SHOW_STYLE__
#define __SHOW_STYLE__


const  	int	CNT_MAX_GROUP			= 5;
const	int	CNT_MAX_EXACT_COMPONENT = 10;

class   CtmDisplayStyle
{
public:
	CtmDisplayStyle();
	virtual	~CtmDisplayStyle();
public:
	virtual	void	Init(void* pAryWnd[], int nWndNum);
	virtual	void	DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum);	
	virtual	void	DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum);	
};


class	CtmWndStyle: public CtmDisplayStyle
{
public:
	CtmWndStyle();
	~CtmWndStyle();
public:
	void	Init(void* pAryWnd[], int nWndNum);
	void	DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum);	
	void	DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum);	
protected:
	virtual	bool	Check(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum);
	virtual	bool	Check(void* pAryWnd[], int nShowColor, int nWndNum);
	virtual	void*	FindComponent(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum);
	virtual	void*	FindComponent(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum);
	virtual	bool	SetColor(int nShowColor, void* pDest);
	virtual	int		SetOldColors(int nIndex, void* pExcept);
	virtual	int		GetCurrentGroup(void* pAryWnd[], int nWndNum);

	virtual	int		InitGroup(void* pAryWnd[], int nWndNum);
	virtual	int		InitColors(int nGroup, void* pAryWnd[], int nWndNum);
protected:
	
	void*	m_pAryWnds[CNT_MAX_GROUP][CNT_MAX_EXACT_COMPONENT];
	int		m_nNums[CNT_MAX_GROUP];
	int		m_nOldColors[CNT_MAX_GROUP][CNT_MAX_EXACT_COMPONENT];
	int		m_nCurrentGroup;
};

class	CtmStyleFrame0: public CtmDisplayStyle
{
public:
	CtmStyleFrame0();
	CtmStyleFrame0(CtmDisplayStyle* pStyle);
	~CtmStyleFrame0();
public:
	virtual	void	SetStyle(CtmDisplayStyle* pStyle);
	
	virtual void Init(void *pAryWnd[], int nWndNum);
	virtual void DisplayColor(void *pSource, void *pAryWnd[], int nShowColor, int nWndNum);
	virtual void DisplayColor(long long llSource, void *pAryWnd[], int nShowColor, int nWndNum);
protected:
	CtmDisplayStyle* m_pStyle;
};


class	CtmStyleFrame: public CtmStyleFrame0
{
public:
	CtmStyleFrame();
	CtmStyleFrame(CtmDisplayStyle* pStyle);
	~CtmStyleFrame();
public:
	//void	Init(void* pAryWnd[], int nWndNum);
	void	DisplayColor(void* pSource, void* pAryWnd[], int nShowColor, int nWndNum);
	void	DisplayColor(long long llSource, void* pAryWnd[], int nShowColor, int nWndNum);	
protected:
	void	GetOldColors(void* pAryWnd[], int nWndNum);

	void	SetOldColors(void* pAryWnd[], int nWndNum);
protected:
	int		m_nOldColors[CNT_MAX_EXACT_COMPONENT];
		
};


#endif
