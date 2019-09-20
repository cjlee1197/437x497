/* Copyright (C), 2003-2010, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Gerry	                                                        |
|  Version  : V1.00                                                          |
|  Creation : 14/9/2010                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef D_SWITCHBOX
#define D_SWITCHBOX

#include 	"wnd.h"
#include	"wndx2.h"

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/

#define     MAX_STRING_NUM    20
const	int	CNT_INT_MAX_DB_NUM = 15;

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagSWITCHBOX
{
    WORD            wType;          //
    CtmBitMap		*pBmp;		//bmp pointer
    char*           pStr;    	  	//¦r¦ê«ü°w
    char*			pPrompt;			//´£¥Ü¦r¦ê
} tmSWITCHBOX;
/*===========================================================================+
|           Class declaration - OptionBox                                    |
+===========================================================================*/
class CtmSwitchBox	: public CtmWnd
{
/*---------------------------------------------------------------------------+
|  		  Constructor and destructor								|
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmSwitchBox)
public:
	CtmSwitchBox(tmWND* pwnd,tmSWITCHBOX* pswitch);
	CtmSwitchBox();
	~CtmSwitchBox();

/*---------------------------------------------------------------------------+
|		Attributes											|
+---------------------------------------------------------------------------*/
private:
	tmSWITCHBOX		m_switch;
	tmSWITCHBOX*	m_pswitch;
	short       	m_nIndex;               // Current selection
	int 			m_nSpace;
	
	ImageBackground	m_BackImage;
	BOOL        	m_bBackGround;
	
	short  			m_nX;
    short   		m_nY;
	
	char*			m_pszPromptID;
	
	char*			m_pszbmpNames[MAX_STRING_NUM];
	char*			m_pszStrs[MAX_STRING_NUM];
	char*			m_pszStrIDs[MAX_STRING_NUM];
	char*			m_pszDBID[CNT_INT_MAX_DB_NUM];
/*----------------------------------------------------------------------------+
|		Options												 |
+----------------------------------------------------------------------------*/
public:
	
	void	Show();
	void 	Update();
	
    void	Switch();
    int		GetIndex() { return m_nIndex; }
    BOOL	SetIndex(int nIndex);
	void	OnGetFocus();
	void	OnLoseFocus();
	WORD	OnKey(WORD wKey);
	
	//	fans add
	BOOL 	CreateA();
	void 	SetPropertys();
	void	Clear();
private:
    void  	Up();
    void	Down();
    void	ShowString();
    void	ShowBmp();
    void	GetEntry();
};
#endif
