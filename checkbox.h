/* Copyright (C), 2003-2008, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : Windows Extension                                              |
|  Task     : Windows Extension Manager header file                          |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Jeckey	                                                        |
|  Version  : V1.00                                                          |
|  Creation : 29/10/2003                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef D_CHECKBOX
#define D_CHECKBOX

#include 	"wnd.h"
#include	"wndx2.h"

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmCHECK      
    {
    BOOL        bCheck;
    char*       psz;
    char*       pszPrompt;
    } tmCHECK;
/*===========================================================================+
|           Class declaration - OptionBox                                    |
+===========================================================================*/
class CtmCheckBox	: public CtmWnd
{
/*---------------------------------------------------------------------------+
|  		  Constructor and destructor								|
+---------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CtmCheckBox)
public:
	CtmCheckBox(tmWND* pwnd,tmCHECK* pcheck);
	CtmCheckBox();		//	fans add
	~CtmCheckBox();

/*---------------------------------------------------------------------------+
|		Attributes											|
+---------------------------------------------------------------------------*/
private:
	BOOL  		m_bCheck;	 // if the checkbox is checked
	tmCHECK*	m_pcheck;
	tmCHECK		m_check;		//	fans add
	char*		m_pszTextID;	//	fans add
	char*		m_pszPromptID;	//	fans add
	char*		m_pszPrompt;
	ImageBackground	m_BackImage;	//	fans add
	BOOL        	m_bBackGround;
/*----------------------------------------------------------------------------+
|		Options												 |
+----------------------------------------------------------------------------*/
public:
	
	void	Show();
	void	Clear();
	void	Get()	{m_pcheck->bCheck = m_bCheck;};
	void	Set()	{m_bCheck = m_pcheck->bCheck;};
	void 	Update();
	
	void	OnGetFocus();
	void	OnLoseFocus();
	WORD	OnKey(WORD wKey);
	
	
	//	fans add
	BOOL 	CreateA();
	void 	SetPropertys();
	void 	SetPropertys_Nub();
private:
    void    Check();
};
#endif
