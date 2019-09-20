#include <stdarg.h>
#include	"font.h"


CtmFontControl*			g_pFont	= NULL;					
CtmFontControl*			g_pFontHelp	= NULL;	

IMPLEMENT_DYNCREATE(CtmFontControl, CtmPackClass)


CtmFontControl::CtmFontControl(): CtmPackClass()
{
	
}

CtmFontControl::~CtmFontControl()
{
	
}

CtmFont*	CtmFontControl::GetFont(int nType)
{
	return NULL;
}

CtmFont*	CtmFontControl::GetDefaultFont()
{
	return GetFont(0);
}

CtmFont*	CtmFontControl::LoadFont(char* pszPath, int nFontCode, int nFontSize, BYTE* pBuf)
{
	return NULL;
}

int			CtmFontControl::Sprintf(CtmFont* pFont, void* pOut, char* fmt, ...)
{
	int nResult = 0;
	
	if (pOut != NULL && fmt != NULL && pFont != NULL)
	{
		va_list ap;
		va_start(ap, fmt);
		switch (pFont->GetFontCode())
		{
			case tmFT_CODE_TECH:
				nResult = TransToTMStr(pOut, fmt, ap);
			 	break;
	
			default:
				nResult = vsprintf((char*)pOut, fmt, ap);
				break; 
		}
		
		va_end(ap);
	}
	
	return nResult;
}

int				CtmFontControl::TransToTMStr(void* pOut, char* fmt, va_list ap)
{
	int nResult = 0;
	
	
	return nResult;
}
