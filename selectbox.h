/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class	: CSelctBox                                                        |
|  Task     : CSelctBox header file                                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : netdigger                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/04/2003                                                     |
|  Revision : V2.00                                                          |
|             											          |
+===========================================================================*/
#ifndef		D_DSELECTBOX
#define		D_DSELECTBOX


#include	"wnd.h"
#include    "wndx2.h"
#include	"tmconfig.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

typedef struct tagtmSELECT      
{
    BOOL        bSelect;
    char*       psz;
    char*       pszPrompt;
    char*		poldsz;
} 	tmSELECT;

/*===========================================================================+
|           Class declaration - SelectBox                                    |
+===========================================================================*/
class CtmSelectBox : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmSelectBox)
public:
    CtmSelectBox(tmWND* pwnd, tmSELECT* pselect);
    ~CtmSelectBox();
    
    //	fans add
    CtmSelectBox();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmSELECT* 	m_pselect;
    tmSELECT	m_select;			//	fans add
    char*		m_pszTextID;		//	fans add
    char*		m_pszPromptID;		//	fans add
    int			m_igroup;
    BGC			m_bgcOld;
    //int			m_nIndex;
    BOOL        	m_bBackGround;
    ImageBackground	m_BackImage;
    BOOL    	m_bSelect;
	CtmBitMap	m_bmpSelected;
	CtmBitMap	m_bmpUnSelected;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
/**/void    Get()               {m_pselect->bSelect = m_bSelect;};
/**/void    Set();
/**/void    Show();
/**/void    Update();

/**/void    OnGetFocus();
/**/void    OnLoseFocus();
/**/WORD    OnKey(WORD wKey);
	
	
	//	fans add
	BOOL 	CreateA();
	void 	SetPropertys();
	void 	SetPropertys_Nub();
	void 	Clear();

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void	Select();
};

#endif
