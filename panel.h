/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC) 3.2                                                    |
|  Link     :G++(GCC) 3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey		                                                   |
|  Version  : V1.00                                                          |
|  Creation : 17/11/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef		D_PANEL
#define		D_PANEL

#include	"wnd.h"

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
#define     MKPANEL(panel, bot,bow,bog, bit,biw,big, f, b ,bTrans) \
            panel.bevelOuter.wType=bot; panel.bevelOuter.wWidth=bow; panel.bevelOuter.wGap=bog; \
            panel.bevelInner.wType=bit; panel.bevelInner.wWidth=biw; panel.bevelInner.wGap=big; \
            panel.fgc=f; panel.bgc=b; panel.bTransparent=bTrans;
            
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     tmBEVEL_NONE          0x0000
#define     tmBEVEL_PLAIN         0x0001
#define     tmBEVEL_RAISE         0x0002
#define     tmBEVEL_INSET         0x0003
#define     tmBEVEL_SHADOW        0x0004
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmBEVEL
    {
    WORD        wType;
    WORD        wWidth;
    WORD        wGap;
    } tmBEVEL;

typedef struct tagtmPANEL
    {
    tmBEVEL       bevelOuter;
    tmBEVEL       bevelInner;
    FGC         fgc;
    BGC         bgc;
    BOOL        bTransparent;
    } tmPANEL;
/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- CtmPanel                                     |
+============================================================================*/
class	 CtmPanel	:	public CtmWnd
{
/*----------------------------------------------------------------------------+
|           Constructor and destructor                                        |
+----------------------------------------------------------------------------*/
public:
    CtmPanel();
    CtmPanel(tmWND* pwnd, tmPANEL* ppanel);
    ~CtmPanel();

/*------------------------------------------------------------------------------+
|           Attributes                                                          |
+------------------------------------------------------------------------------*/
private:
	tmPANEL*  m_ppanel;
/*-------------------------------------------------------------------------------+
|        Operations                                                              |
+-------------------------------------------------------------------------------*/
public:
	BOOL    Create(tmWND* pwnd, tmPANEL* ppanel);
/**/void    Show();
};

#endif
