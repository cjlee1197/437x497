/*
*	Created by J.Wong 2016/3/14 9:33:05
*/

#ifndef D_LINEEDIT
#define D_LINEEDIT
#include "wnd.h"
#include "wndx2.h"
#include "libxmlcomm.h"
#include <map>


/*===========================================================================+
|           Class declaration - LineEdit                                     |
+===========================================================================*/



class CtmLineEdit:public CtmWnd
{
	/*---------------------------------------------------------------------------+
	|	        Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/	
	DECLARE_DYNCREATE(CtmLineEdit)
	public:
		CtmLineEdit();
		~CtmLineEdit();
	/*---------------------------------------------------------------------------+
	|           Operations                                                       |
	+---------------------------------------------------------------------------*/	
	public:
		void    Show();
		void    Update();
		void    DrawBorder();
		void    Clear();
		BOOL 	CreateA();
		void 	SetPropertys();
		void	SetCopyBackImage(BOOL bCopy);
		void	SetContent(char* pszContent,WORD wLen);
		void	SetContext(std::map<string, tmControl> context);
		void	SetContext(std::map<int, tmControl> context);
		int		StepContent(int hstep=0, int vstep=0);
		int		LocalRow(int tab=0);
		int		LocalRow(char* pszName=0);
		int		GetRows();
		int		GetMaxCols();
		int		SetStyle(int mode=0);
		
		void		ShowImage(RECT rect, int nIndex);
		CtmBitMap* 	GetBmpImage(char* pszImagePath);	
	private:
		
		
		int			getLine(char* pszInContent,char* pszOutContent);
		int	 		getLines(char* pszInContent);
		int			getLines(unsigned int* pUnicode, int nLen);
		
		int 		getRows(unsigned int* pUnicode, int nLen);
		int			getRows(char*  pszInContent);
		char*		setStartLine(char* pszInContent,int vstep=0);
		unsigned int* 	setStartLine(unsigned int* pUnicode,int* pnLen, int vstep=0);
		void		appendContent(char* appendString);
		
	private:
		class CtmPrivateData;
		CtmPrivateData*		m_data;
		char**				m_pszContext;
};

#endif