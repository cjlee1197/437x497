/*
*	created by J.Wong 2016/5/3 14:25:26
*/

#ifndef _PDFVIEW_H
#define _PDFVIEW_H

#include "wnd.h"
#include "wndx2.h"
/*===========================================================================+
|           Class declaration - PDFView                                      |
+===========================================================================*/

class CtmPDFView:public CtmWnd
{
	
	DECLARE_DYNCREATE(CtmPDFView)
	public:
		CtmPDFView();
		~CtmPDFView();
		
		void    Show();
		void    Update();
		void    DrawBorder();
		void    Clear();
		BOOL 	CreateA();
		WORD    OnKey(WORD wKey);
		void 	SetPropertys();
		void   	OnGetFocus();
		void    OnLoseFocus();
		void	OpenFile(char* pszFile);
		void	SetTitle(char* pszTitle);
		void	PageDown();
		void	PageUp();
		void	ZoomIn();
		void	ZoomOut();
	private:
		class CtmPrivateData;
		CtmPrivateData*		m_data;
		
		void	initTitle();
		void	initScrollbar();
};
#endif