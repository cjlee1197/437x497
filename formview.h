 /* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
//##ModelId=447FB607028C
#ifndef		D_FORMVIEW
#define		D_FORMVIEW
#include	"dialog.h" 

#define		VIEW_TYPE_COMMON			1
#define		VIEW_TYPE_INPUTFORM			2
#define		VIEW_TYPE_LOGINFORM			4
#define		VIEW_TYPE_TOUCH_MSG		  5 //Mario add 2016/1/20 ?? 02:48:49
/*===========================================================================+
|           Class declaration - FormView                                     |
+===========================================================================*/
class CtmFormView : public CtmDialog
{
	/*---------------------------------------------------------------------------+
	|           Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmFormView)
	public:                      
	    CtmFormView();
	    ~CtmFormView();
	    
	/*---------------------------------------------------------------------------+
	|           Attributes                                                       |
	+---------------------------------------------------------------------------*/
	private:
		//	fans add event
		WORD	(*OnChangeA)	(CtmWnd* pwnd, WORD wIDControl);
		BOOL	(*OnCreateA)	(CtmWnd* pwnd);
	    void	(*OnShowA)		(CtmWnd* pwnd);
	    WORD	(*OnKeyA)		(CtmWnd* pwnd, WORD wKey);
	    void	(*OnUpdateA)	(CtmWnd* pwnd);
	    void	(*OnUpdateB)	(CtmWnd* pwnd, void* pData);
	    void	(*OnDestroyA)	(CtmWnd* pwnd);
	    WORD	(*OnChangeBM)	(CtmWnd* pwnd, WORD wIDControl);
	    void	(*OnMessage)	(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam);	//	fans 2008/6/16 11:28上午
	    WORD	(*OnMouseDown)  (CtmWnd* pwnd, WORD wIDControl);
	    WORD	(*OnMouseUp)	(CtmWnd* pwnd, WORD wIDControl);
	    
	    char*	m_pszOnChangeName;
	    char*	m_pszOnCreateName;
	    char*	m_pszOnShow;
	    char*	m_pszOnKey;
	    char*	m_pszUpdate;
	    char*	m_pszUpdateB;
	    char*	m_pszDestroy;
	    char*	m_pszChangeBM;
	    char*	m_pszOnMessage;			//	fans 2008/6/16 11:30上午
	    char*	m_pszOnMouseDown;		//	James add 2011/10/6 06:51下午
	    char*	m_pszOnMouseUp;			//	James add 2011/10/6 06:51下午
	    
	    char*	m_pszPageNo;
	    char*	m_pszPrintName;
	    
	    //add by J.Wang
	    WORD	m_wDoubleHint;
	    long 	m_lHintDuration;
	    
	    CtmWnd*	m_aryUpdateWnd[256];
	    int		m_nUpdateNum;
	    static  int		m_nAutoESC;
		int		m_nIsCounter;
	    
	    BOOL	m_bCanStopUpdate;
	    BOOL	m_bPrivateUpdate;
	    
	    CtmWnd*	m_pwndMouseDown;
	    BOOL	m_TOUCH_MSG_Down;
	    WORD	m_wViewType;
		int		m_iFBIndex;			//FB Index
		int		m_iStartX;			//FB2 StartX
		int		m_iStartY;			//FB2 StartY
	   
	    pthread_t 		m_ThreadID;
	    static	void	ThreadOnLineHelp(CtmFormView *pFormView);
	    int	 	m_nFlag;
		int		m_nClickCount;
		CtmWnd*	m_pOldControl;
	protected:
		WORD	m_wSaveKey;				//	2010-1-15 10:07:09
		WORD	m_wKey;
		long 	m_lHelpTimer;
		//BOOL	m_DrawHelpFlag;
	/*---------------------------------------------------------------------------+
	|           Operations                                                       |
	+---------------------------------------------------------------------------*/
	public: 
	    BOOL	CreateA();			//	fans add
		WORD	OnKey1(WORD wKey);	//	fans add
		WORD	OnKey2(WORD wKey);	//	fans add
		void	Clear();			//	// <FANS> 2006/11/28 04:10下午
		void    Update();
		void	UpdateB(void* pData);
	
		WORD    OnKey(WORD wKey);
		
		BOOL	MouseUp(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16 
		BOOL	MouseDown(short x_pos,short y_pos,int key_flag);		//James addd 2010/3/16 
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);	// fans 2008/6/16 11:29上午
		
		
		void	SetPropertys();
		void	SetPropertys_Nub();
	/**/WORD    OnChange(WORD wIDControl);
	/**/void    Show();
		WORD	OnMouseUPEvent(WORD wIDControl);
		WORD	OnMouseDownEvent(WORD wIDControl);
	
	/*---------------------------------------------------------------------------+
	|           Helpers                                                          |
	+---------------------------------------------------------------------------*/
	private:
		WORD	OnChangeBMEvent(WORD wIDControl);

};

#endif
