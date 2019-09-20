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

#ifndef     D_STATIC
#define     D_STATIC

#include	"wnd.h"
#include    "wndx2.h"

#define     HST                 (WB+HF*1 +WB)               // Static box height

#define     WST_1               (WB+WF*1 +WB)               // Static box width - 1  character
#define     WST_2               (WB+WF*2 +WB)               // Static box width - 2  characters
#define     WST_3               (WB+WF*3 +WB)               // Static box width - 3  characters
#define     WST_4               (WB+WF*4 +WB)               // Static box width - 4  characters
#define     WST_5               (WB+WF*5 +WB)               // Static box width - 5  characters
#define     WST_6               (WB+WF*6 +WB)               // Static box width - 6  characters
#define     WST_7               (WB+WF*7 +WB)               // Static box width - 7  characters
#define     WST_8               (WB+WF*8 +WB)               // Static box width - 8  characters
#define     WST_9               (WB+WF*9 +WB)               // Static box width - 9  characters
#define     WST_10              (WB+WF*10+WB)               // Static box width - 10 characters
#define     WST_11              (WB+WF*11+WB)               // Static box width - 11 characters
#define     WST_12              (WB+WF*12+WB)               // Static box width - 12 characters
#define     WST_13              (WB+WF*13+WB)               // Static box width - 13 characters
#define     WST_14              (WB+WF*14+WB)               // Static box width - 14 characters
#define     WST_15              (WB+WF*15+WB)               // Static box width - 15 characters
#define     WST_16              (WB+WF*16+WB)               // Static box width - 16 characters
#define     WST_17              (WB+WF*17+WB)               // Static box width - 17 characters
#define     WST_18              (WB+WF*18+WB)               // Static box width - 18 characters
#define     WST_19              (WB+WF*19+WB)               // Static box width - 19 characters
#define     WST_20              (WB+WF*20+WB)               // Static box width - 20 characters
#define     WST_21              (WB+WF*21+WB)               // Static box width - 21 characters
#define     WST_22              (WB+WF*22+WB)               // Static box width - 22 characters
#define     WST_23              (WB+WF*23+WB)               // Static box width - 23 characters
#define     WST_24              (WB+WF*24+WB)               // Static box width - 24 characters
#define     WST_25              (WB+WF*25+WB)               // Static box width - 25 characters
#define     WST_26              (WB+WF*26+WB)               // Static box width - 26 characters
#define     WST_27              (WB+WF*27+WB)               // Static box width - 27 characters
#define     WST_28              (WB+WF*28+WB)               // Static box width - 28 characters
#define     WST_29              (WB+WF*29+WB)               // Static box width - 29 characters
#define     WST_30              (WB+WF*30+WB)               // Static box width - 30 characters
#define     WST_31              (WB+WF*31+WB)               // Static box width - 31 characters
#define     WST_32              (WB+WF*32+WB)               // Static box width - 32 characters
#define     WST_33              (WB+WF*33+WB)               // Static box width - 33 characters
#define     WST_34              (WB+WF*34+WB)               // Static box width - 34 characters
#define     WST_35              (WB+WF*35+WB)               // Static box width - 35 characters
#define     WST_36              (WB+WF*36+WB)               // Static box width - 36 characters
#define     WST_37              (WB+WF*37+WB)               // Static box width - 37 characters
#define     WST_38              (WB+WF*38+WB)               // Static box width - 38 characters
#define     WST_39              (WB+WF*39+WB)               // Static box width - 39 characters
#define     WST_40              (WB+WF*40+WB)               // Static box width - 40 characters
#define     WST_41              (WB+WF*41+WB)               // Static box width - 41 characters
#define     WST_42              (WB+WF*42+WB)               // Static box width - 42 characters
#define     WST_43              (WB+WF*43+WB)               // Static box width - 43 characters
#define     WST_44              (WB+WF*44+WB)               // Static box width - 44 characters
#define     WST_46              (WB+WF*46+WB)               // Static box width - 46 characters
#define     WST_48              (WB+WF*48+WB)               // Static box width - 46 characters
#define     WST_60              (WB+WF*60+WB)               // Static box width - 60 characters
#define     WST_80              (WB+WF*80+WB)               // Static box width - 80 characters
#define     WST_100              (WB+WF*100+WB)             // Static box width -100 characters


#define     MKSTATIC(stat, string) 	stat.psz=string;

const		int				MAX_STRING_NUM_STATIC = 10;
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmSTATIC
{
	char*       psz;
} tmSTATIC;

/*===========================================================================+
|           Class declaration - StaticBox                                    |
+===========================================================================*/
class CtmStatic : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmStatic(tmWND* pwnd, tmSTATIC* pstatic);
    ~CtmStatic();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmSTATIC*	m_pstatic;
//	BITMAP 		m_bmp;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	void    Show()              {CtmWnd::Show(0, m_pstatic->psz);};
	void    Update();
	void    Update2();
	void	Clear();
};

/*===========================================================================+
|           Class declaration - StaticBox                                    |
+===========================================================================*/
class CtmStaticX1 : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmStaticX1(tmWND* pwnd, tmSTATIC* pstatic);
    ~CtmStaticX1();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmSTATIC* m_pstatic;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Show()              {CtmWnd::ShowExtent(0, 0, m_pstatic->psz);};
/**/void    Update();
};

/*===========================================================================+
|           Class declaration - StaticBox                                    |
+===========================================================================*/
class CtmStaticX2 : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmStaticX2)
public:

    CtmStaticX2();				//	fans add
    ~CtmStaticX2();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    BOOL        	m_bBackGround;
    tmSTATIC*   	m_pstatic;
    tmSTATIC    	m_static;			//	fans add
    char*	    	m_pszTextID;		//	fans add, text's id
    char*			m_psziNetStream;	//	rw.ying
    WORD			m_wiNetLen;			//	rw.ying
    WORD			m_wiNetType;		//	rw.ying
    char*	    	m_pszTextIDEX[MAX_STRING_NUM_STATIC];	// <FANS> 2006/11/27 01:53¤U¤È
   	BOOL			m_bOnlineHelp;
    BGC				m_bgcOld;
    ImageBackground	m_BackImage;	//	fans add
    
    short			m_nOffSet;					//	fans add 2009-3-24 13:58:47

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Show();              //{CWnd::Show(0, m_pstatic->psz);};
/**/void    Update();
/**/void    Update2();
/**/void    DrawBorder();
	void    Clear();
	//	fans add
	BOOL 	CreateA();
	void 	SetPropertys();
	void 	SetPropertys_Nub();
	void	SetCopyBackImage(BOOL bCopy) {m_bBackGround = bCopy;};
//	bool 	SetPropValueT(char* pszPropName, char* pszValue, int nLen, int nType);		//rw.ying 2011/3/17 02:52¤U¤È
};
#endif
