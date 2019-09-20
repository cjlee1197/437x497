/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/

/*===========================================================================+
|  Class    : Select Edit                                                    |
|  Task     : Select Edit header file                                        |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Leo                                                            |
|  Version  : V1.00                                                          |
|  Creation : 11/07/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef 	D_LINETABLE
#define		D_LINETABLE

#include 	"wnd.h"
#include	"wndx2.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
// 當nVertRows和nHoriCols同時小於等於0時為單框
#define     STYLE_NOTSHOW_LEFT              0x0001  // 單框時不顯示左線條
#define     STYLE_NOTSHOW_TOP               0x0002  // 單框時不顯示上線條
#define     STYLE_NOTSHOW_RIGHT             0x0004  // 單框時不顯示右線條
#define     STYLE_NOTSHOW_BOTTOM            0x0008  // 單框時不顯示下線條

#define     STYLE_LT_NOVERTLINE             0x0010  // 制表格時橫線不顯示
#define     STYLE_LT_NOHORILINE             0x0020  // 制表格時豎線不顯示
#define		STYLE_LT_PCORNERLINE			0x0040	//		   訇薹峈淏腔晚褒盄
#define		STYLE_LT_NCORNERLINE			0x0080	//		   訇薹峈蛹腔晚褒盄
#define     STYLE_LT_VERTTITLE              0x1000  // 制表格時橫線標題表格
#define     STYLE_LT_HORITITLE              0x2000  // 制表格時豎線標題表格
#define     STYLE_LT_VERTUNEVEN             0x4000  // 制表格時橫線不均等
#define     STYLE_LT_HORIUNEVEN             0x8000  // 制表格時豎線不均等

#define     STYLE_TRANSPARENT               0x0100  // 背景為透明
#define     STYLE_SINGLELINE                0x0200  // 都為單線條

#define		STYLE_CORNERTYPE				0x0400  //隅砱晚褒瑞跡
#define		STYLE_LT_LTCORNERTOP			0x2040
#define		STYLE_LT_LBCORNERTOP			0x2080
#define		STYLE_LT_RTCORNERTOP			0x2040
#define		STYLE_LT_RBCORNERTOP			0x2080

#define     MAX_LT_POINTS                   20
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagLINETABLE
    {
    WORD            wType;
    short           nMainLineWidth;
    tmCOLOR         tmclMainLine;
    tmCOLOR         tmclAccrLine1;
    tmCOLOR         tmclAccrLine2;
    short           nVertRows;
    short           nHoriCols;
    short*          pnVertpoints;
    short*          pnHoripoints;
    } tmLINETABLE;
/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- CtmListBox                                   |
+============================================================================*/
class		CtmLineTable  : public CtmWnd
{
	/*----------------------------------------------------------------------------+
	|           Constructor and destructor                                        |
	+----------------------------------------------------------------------------*/
	DECLARE_DYNCREATE(CtmLineTable)
	public:
		CtmLineTable(tmWND* pwnd,tmLINETABLE* plinetable);
		CtmLineTable();	//	fans
		~CtmLineTable();

	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/
	private:
	    tmLINETABLE     m_linetable;
	    tmLINETABLE*    m_plinetable;

        WORD            m_wType;
	    DWORD           m_clMainLine;
	    DWORD           m_clAccrLine1;
	    DWORD           m_clAccrLine2;
	    DWORD           m_clDot;
	    ImageBackground	m_BackImage;		//	fans add
    	BOOL        		m_bBackGround;

	    short           m_anVertpoints[MAX_LT_POINTS];
        short           m_anHoripoints[MAX_LT_POINTS];
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/
	public:
	    void    Set();
	    void    GetColor(tmCOLOR *pLineColor, WORD wIDLine);
	    void    SetColor(tmCOLOR *pLineColor, WORD wIDLine);
	/**/void    Show();
	/**/void    Update();
		//	fans add
		BOOL 	CreateA();
		void 	SetPropertys();
		void 	SetPropertys_Nub();
		void    Clear();

	/*---------------------------------------------------------------------------+
	|           Helpers                                                          |
	+---------------------------------------------------------------------------*/
	private:
		void    DealFrame();
	    void    DrawLine(short left, short top, short right, short bottom, short nLineWidth, WORD wType);
	    void    DrawSingle();
};
#endif
