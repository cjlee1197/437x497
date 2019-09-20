/* Copyright (C), 1996-2006, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Shousu. fan                                                    |
|  Version  : V1.00                                                          |
|  Creation : 09/12/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef  	D_BINDLINE
#define		D_BINDLINE
#include	"wnd.h"
#include	"curve.h"
#include	"edit.h"



const 	int		DRAW_TYPE_POINT = 0x10;
const 	int		DRAW_TYPE_DELAY = 0x20;
const 	int		DRAW_TYPE_CONTINUE = 0x30;

const	int		DRAW_FIX_UP = 1;
const	int		DRAW_FIX_RIGHT = 2;
const	int		DRAW_FIX_DOWN = 3;
const	int		DRAW_FIX_LEFT = 4;


typedef struct tagtmBindPar
{
	CtmWnd* 	pwndBindControl;			//	捆綁的元件
	int			iType;						//	圖形的類型：0x10點、0x20 Delay、0x30 continue
	int			iFix;						//	与綁定元件的相對方位：1為上，2為右，3為下，4為坐
	DWORD 		dwCommonColor;				//	圖形的色彩
	DWORD 		dwFocusColor;				//	線條的焦點色彩	
	void*		pTag;						//	這是一個附加參數,其真正的使用可能是存儲的一些其他信息
} tmBindPar;






class CtmBindLine: public CtmWnd
{
	DECLARE_DYNCREATE(CtmBindLine)
	
	protected:
		tmBindPar	m_bindControl;    	
	    	
		
		char*		m_pszBindControlName;    	
    	char* 		m_pszText;    	
    	char* 		m_pszTextID;
    	DWORD		m_dwColor;
    	
    	
	public:    
		CtmBindLine();    
		~CtmBindLine();		
	protected:    	
		virtual void	InitControl();
		
		
		virtual void 	GetBindControls();			//	從元件的名稱集合m_pszBindControlName尋找到對應的元件
    	virtual void 	DrawText(char* pszText);    
    	virtual void 	DrawIdLine(DWORD dwColor);    	
    	virtual void 	DrawShowLine(DWORD dwColor);
	public:    	
		BOOL 	CreateA();    	
		void 	SetPropertys();    	
		void    Show();    	
		void    Update();		    	
		void	DrawBindLine(DWORD dwColor);						//	繪畫出各個部分的線段
		
		void	SetOnFocus();											//	焦點
		void	SetLoseFocus();

};


//	直接與Curve相關聯
class CtmBindCurveLine: public CtmBindLine
{
	DECLARE_DYNCREATE(CtmBindCurveLine)
	
	protected:
		CCurve*	m_pBindCurve;						//	相關聯的元件
		
		
		int		m_iPointCount;						//	與Curve相關聯的點數,這數值是計算出來的，而不是用戶指定的
		int		m_iFirstPointIndex;					//	第一個點與Curve元件是哪個點相關聯
		int		m_iSecondPointIndex;				//	第二個點與Curve元件是哪個點相關聯
	
	public:    
		CtmBindCurveLine();    
		~CtmBindCurveLine();	
		
		void 	SetPropertys();
	
	protected:
		virtual tmPoint		GetIDLineFirstPoint();		
		virtual tmPoint 	GetIDLineSecondPoint();
		void		DrawIdLine(DWORD dwColor);		//	virtual function
		void 		GetBindControls();				//	virtual funciton
	
	private:
		//	根據給定的定點和邊長以及方向，繪出三角形
		void		DrawTrianglePoint(tmPoint PointVertex, int len, int iFix = DRAW_FIX_DOWN);	
		void		DrawPoint(tmPoint Point, int size, int iFix = DRAW_FIX_DOWN);
		
		//	繪畫虛線
		void		DrawDashed(tmPoint SPoint, tmPoint EPoint, int iBrokenLen = 4);
};


//	與EditX1相關聯,以便可以獲得Edit中的資料
/*
class CtmBindECurveLine: public CtmBindCurveLine
{
	DECLARE_DYNCREATE(CtmBindECurveLine)
	
	
	
	protected:
		CtmEditX1* m_petSourceData;					//	數據來源的那個Edit
		char*	m_pszEditName;						//	數據來源的那個Edit
	protected:
		tmPoint		GetIDLineFirstPoint();
		tmPoint 	GetIDLineSecondPoint();
	
	
};
*/

#endif
