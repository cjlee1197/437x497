/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey                                                         |
|  Version  : V1.00                                                          |
|  Creation : 11/04/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef 	D_LISTBOX
#define		D_LISTBOX

#include 	"wnd.h"
#include	"scrollbar.h"
#include	"init.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		tmMAX_ITEMS		 50

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
#define     MKLIST(list, i, c, pl, pp) \
            list.nIndex=i; list.nCount=c; list.pszList=pl; list.pszPrompt=pp;
#define     MKLIST1(list, i, pl, pp) \
            list.nIndex=i; list.pszList=pl; list.pszPrompt=pp;
#define     MKLIST2(list, i, pp) \
            list.nIndex=i; list.pszPrompt=pp;
            
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmLIST
    {
    short       nIndex;                 // Current selection
    short       nCount;                 // Number of list item
    char**      pszList;
    char*       pszPrompt;
    } tmLIST;
    
/*============================================================================+
|            Function and Class prototype                                     |
+============================================================================*/
/*============================================================================+
|           Class declaration -- CtmListBox                                   |
+============================================================================*/
class		CtmListBox  : public CtmWnd
{
/*----------------------------------------------------------------------------+
|           Constructor and destructor                                        |
+----------------------------------------------------------------------------*/
public:
	CtmListBox(tmWND* pwnd, tmLIST* plist);
	~CtmListBox();

/*------------------------------------------------------------------------------+
|           Attributes                                                          |
+------------------------------------------------------------------------------*/
private:
    tmLIST*       	m_plist;
    short       	m_nIndex;               // Current selection
    short       	m_nStart;               // Item index of the first line in the window
    short       	m_nPageSize;            // Number of items can be displayed in the window
    tmSCROLLBAR   	m_scrollbar;
    CtmScrollBar* 	m_pScrollBar;
    
/*-------------------------------------------------------------------------------+
|        Operations                                                              |
+-------------------------------------------------------------------------------*/
public:
/**/void    Get()               {m_plist->nIndex = m_nIndex;};
/**/void    Set();
/**/void    Show();
/**/void    Update();

/**/void    OnGetFocus();
/**/void    OnLoseFocus();
/**/WORD    OnKey(WORD wKey);

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void    Select(short nIndex);
    void    Scroll(short nDelta);
};
#endif
