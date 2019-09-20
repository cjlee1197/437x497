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
// ��nVertRows�MnHoriCols�P�ɤp�󵥩�0�ɬ����
#define     STYLE_NOTSHOW_LEFT              0x0001  // ��خɤ���ܥ��u��
#define     STYLE_NOTSHOW_TOP               0x0002  // ��خɤ���ܤW�u��
#define     STYLE_NOTSHOW_RIGHT             0x0004  // ��خɤ���ܥk�u��
#define     STYLE_NOTSHOW_BOTTOM            0x0008  // ��خɤ���ܤU�u��

#define     STYLE_LT_NOVERTLINE             0x0010  // ����ɾ�u�����
#define     STYLE_LT_NOHORILINE             0x0020  // ����ɽݽu�����
#define		STYLE_LT_PCORNERLINE			0x0040	//		   б��Ϊ���ı߽���
#define		STYLE_LT_NCORNERLINE			0x0080	//		   б��Ϊ���ı߽���
#define     STYLE_LT_VERTTITLE              0x1000  // ����ɾ�u���D���
#define     STYLE_LT_HORITITLE              0x2000  // ����ɽݽu���D���
#define     STYLE_LT_VERTUNEVEN             0x4000  // ����ɾ�u������
#define     STYLE_LT_HORIUNEVEN             0x8000  // ����ɽݽu������

#define     STYLE_TRANSPARENT               0x0100  // �I�����z��
#define     STYLE_SINGLELINE                0x0200  // ������u��

#define		STYLE_CORNERTYPE				0x0400  //����߽Ƿ��
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
