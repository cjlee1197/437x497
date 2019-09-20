/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : ScrollBar                                                      |
|  Task     : scroll bar header file                                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                         |
|  Version  : V1.00                                                          |
|  Creation : 20/10/2008                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef 	D_SCROLLBAR
#define		D_SCROLLBAR

#include 	"wnd.h"
#include	"button.h"
/*---------------------------------------------------------------------------+
|			Constants									  	     |	
+---------------------------------------------------------------------------*/

//******
//      Scroll bar
//******
#define     tmSCROLLBAR_H         0x0001
#define     tmSCROLLBAR_V         0x0002

#define		tmSCROLLBAR_BUTTON		40		//	the button size
#define		tmSCROLLBAR_BUTTON1		28		//
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmSCROLLBAR
    {
    WORD        wType;
//  WORD        wMajorChange;           // Increment/Decrement value when press PgUp/PgDn
//  WORD        wMinorChange;           // Increment/Decrement value when press UpArrow/DnArror
    } tmSCROLLBAR;
	
/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- ScrollBar                                    |
+============================================================================*/
class CtmScrollBar: public CtmWnd
{
	DECLARE_DYNCREATE(CtmScrollBar)
/*----------------------------------------------------------------------------+
|           Constructor and destructor                                        |
+----------------------------------------------------------------------------*/
public:
	CtmScrollBar();
	~CtmScrollBar();

/*------------------------------------------------------------------------------+
|           Attributes                                                          |
+------------------------------------------------------------------------------*/
private:
   
    //	fans add
    DWORD			m_dwRange;			//	the scroolbar range, not pixes
    DWORD			m_dwPosition;			//	the button postion, not pixes
    DWORD			m_dwOldPosition;
    WORD			m_wIncrement;		//	the move pixes when press the up/down button
    WORD			m_wScrollbarType;	//	the scroolbar type, 1 is hor,2 is ver
    
    CtmButton		m_button1;
    CtmButton		m_button2;
    CtmButton		m_button;
    RECT			m_buttonRect;
    
    WORD			m_wReg;				//	the scrollbar region, is pixes
    BOOL			m_bPaintFixed;
    BOOL			m_bPaintUnFixed;
    BOOL			m_bFirstPaintFixed;	//	the fixed buttons need paint one time only.
    int				m_buttonHight;		//Mario add 20150113
    BOOL			MoveButtomFlag;
    WORD			MoveButtomPos_Y;
    
/*-------------------------------------------------------------------------------+
|        Operations                                                              |
+-------------------------------------------------------------------------------*/
public:
/**/BOOL	CreateA();
/**/void	Show();
	void    Update();
	void   	Clear();
	void 	SetPropertys();
	void	SetPropertys_Nub();

	void	SetMaxReg(DWORD dwReg);
	void	SetPos(DWORD dwPos);
	RECT	GetMoveButtonRECT();
	WORD  GetIncrement();		
	void	SetButtonHight(int MaxRows);			
	void	SetPosByStep(int nStep=0);
	DWORD   GetPos();		
	void  SetButtonPress(int wButton);
	void  SetAllButtonUp();
	void		WndProc(int message, WPARAM wParam, LPARAM lParam);
	BOOL		MouseDown(short x_pos,short y_pos,int key_flag);
	BOOL		MouseUp(short x_pos,short y_pos,int key_flag);
	BOOL		MouseMove(short x_pos,short y_pos,int key_flag);
	int		GetButtonHight() {return m_buttonHight;};
protected:
	void	SetButtonPropertys();
	void	SetButtonPropertys_Nub();
	void	Paint();
	void	PaintFixedButton();
	void	PaintUnFixedButton();
	void	CalculateReg();
	void	ClearUnFixedButton();
private:
	void	DrawUp();
	void	DrawDown();
	
};

#endif
