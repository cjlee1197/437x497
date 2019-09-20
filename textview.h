/*
*	Created by J.Wong	2016/3/15 13:26:03
*/

#ifndef		D_TEXTVIEW
#define		D_TEXTVIEW

#include "wnd.h"
#include "wndx2.h"
#include <map>
#include "libxmlcomm.h"


/*===========================================================================+
|           Class declaration - TextView                                     |
+===========================================================================*/


class CtmTextView:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmTextView)
	public:
		CtmTextView();
		~CtmTextView();
	/*---------------------------------------------------------------------------+
	|           Operations                                                       |
	+---------------------------------------------------------------------------*/	
	public:
		void    Show();
		void    Update();
		void    DrawBorder();
		BOOL 	CreateA();
		void 	SetPropertys();
		void   	OnGetFocus();
		void    OnLoseFocus();
		WORD    OnKey(WORD wKey);
		void	SetText(char* pszContext);
		void	SetContext(std::map<int, tmControl> context);
		void	SetContext(std::map<string, tmControl> context);
		void	SetTitle(char* pszTitle);
		void	SetFooter(char* pszFooter);
		void	LocalRows(int tab=-1);
		void	LocalRows(char* pszName=0);
	private:
		void	initScrollbar();
		void	initTitle();
		void	initContext();	
	private:
		class CtmPrivateData;
		CtmPrivateData* m_data;
};

#endif