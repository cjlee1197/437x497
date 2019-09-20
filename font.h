#ifndef 	__D__CFONTCONTROL
#define		__D__CFONTCONTROL
#include	"tmpackclass.h"
#include	"cfont.h"

class	CtmFontControl: public CtmPackClass
{
	DECLARE_DYNCREATE(CtmFontControl)	
	public:
		CtmFontControl();
		~CtmFontControl();
	public:
		virtual	CtmFont*	GetFont(int nType);
		virtual	CtmFont*	GetDefaultFont();
		virtual	int			Sprintf(CtmFont* pFont, void* pOut, char* fmt, ...);
	protected:
		virtual	CtmFont*	LoadFont(char* pszPath, int nFontCode, int nFontSize, BYTE* pBuf);	
		virtual	int			TransToTMStr(void* pOut, char* fmt, va_list ap);
			
};
	
	
extern	CtmFontControl*			g_pFont;
extern	CtmFontControl*			g_pFontHelp;
	
#endif

