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
#ifndef 	D_PROGRESSBAR
#define		D_PROGRESSBAR

#include 	"wnd.h"
#include	"gui_ex.h"
/*---------------------------------------------------------------------------+
|			Constants									  	     |	
+---------------------------------------------------------------------------*/

//******
//      Scroll bar
//******
#define     tmPROGRESS_H         0x0001
#define     tmPROGRESS_V         0x0002




/*============================================================================+
|           Class declaration -- ScrollBar                                    |
+============================================================================*/
class CtmProgressBar: public CtmWnd
{
	DECLARE_DYNCREATE(CtmProgressBar)
/*----------------------------------------------------------------------------+
|           Constructor and destructor                                        |
+----------------------------------------------------------------------------*/
public:
	CtmProgressBar();
	~CtmProgressBar();

/*------------------------------------------------------------------------------+
|           Attributes                                                          |
+------------------------------------------------------------------------------*/
private:
	
	long long		m_llMax;
	long long		m_llMin;
	long long		m_llValue;
	
	WORD			m_wPos;				//	0 - 100;	
   	char*			m_pszDBID;
   	char*			m_pszMaxID;
   	char*			m_pszMinID;
   	WORD			m_wBarType;
   	BOOL			m_bLimitDB;
   	CtmGDIDoubleBuffer	m_Buffer;
   	BOOL			m_bCopyBg;
   
    
/*-------------------------------------------------------------------------------+
|        Operations                                                              |
+-------------------------------------------------------------------------------*/
public:
/**/BOOL	CreateA();
/**/void	Show();
/**/void    DrawBorder();
/**/void    Update();
/**/void   	Clear();
/**/void 	SetPropertys();
	void 	SetPropertys_Nub();
	bool 	SetPropValueT(char* pszPropName, double Value);
	
protected:
	void	Paint();
	void	PaintH();
	void	PaintV();
	void	ClearH();
	void	ClearV();
	void	Invisible();
	void	GetPosBaseOnDB();
	void	SetRegInBuffer(int nSpace);
};

#endif
