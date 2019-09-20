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
	CtmWnd* 	pwndBindControl;			//	���j������
	int			iType;						//	�ϧΪ������G0x10�I�B0x20 Delay�B0x30 continue
	int			iFix;						//	�O�j�w���󪺬۹���G1���W�A2���k�A3���U�A4����
	DWORD 		dwCommonColor;				//	�ϧΪ���m
	DWORD 		dwFocusColor;				//	�u�����J�I��m	
	void*		pTag;						//	�o�O�@�Ӫ��[�Ѽ�,��u�����ϥΥi��O�s�x���@�Ǩ�L�H��
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
		
		
		virtual void 	GetBindControls();			//	�q���󪺦W�ٶ��Xm_pszBindControlName�M������������
    	virtual void 	DrawText(char* pszText);    
    	virtual void 	DrawIdLine(DWORD dwColor);    	
    	virtual void 	DrawShowLine(DWORD dwColor);
	public:    	
		BOOL 	CreateA();    	
		void 	SetPropertys();    	
		void    Show();    	
		void    Update();		    	
		void	DrawBindLine(DWORD dwColor);						//	ø�e�X�U�ӳ������u�q
		
		void	SetOnFocus();											//	�J�I
		void	SetLoseFocus();

};


//	�����PCurve�����p
class CtmBindCurveLine: public CtmBindLine
{
	DECLARE_DYNCREATE(CtmBindCurveLine)
	
	protected:
		CCurve*	m_pBindCurve;						//	�����p������
		
		
		int		m_iPointCount;						//	�PCurve�����p���I��,�o�ƭȬO�p��X�Ӫ��A�Ӥ��O�Τ���w��
		int		m_iFirstPointIndex;					//	�Ĥ@���I�PCurve����O�����I�����p
		int		m_iSecondPointIndex;				//	�ĤG���I�PCurve����O�����I�����p
	
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
		//	�ھڵ��w���w�I�M����H�Τ�V�Aø�X�T����
		void		DrawTrianglePoint(tmPoint PointVertex, int len, int iFix = DRAW_FIX_DOWN);	
		void		DrawPoint(tmPoint Point, int size, int iFix = DRAW_FIX_DOWN);
		
		//	ø�e��u
		void		DrawDashed(tmPoint SPoint, tmPoint EPoint, int iBrokenLen = 4);
};


//	�PEditX1�����p,�H�K�i�H��oEdit�������
/*
class CtmBindECurveLine: public CtmBindCurveLine
{
	DECLARE_DYNCREATE(CtmBindECurveLine)
	
	
	
	protected:
		CtmEditX1* m_petSourceData;					//	�ƾڨӷ�������Edit
		char*	m_pszEditName;						//	�ƾڨӷ�������Edit
	protected:
		tmPoint		GetIDLineFirstPoint();
		tmPoint 	GetIDLineSecondPoint();
	
	
};
*/

#endif
