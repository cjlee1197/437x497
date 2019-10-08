#ifndef	D__TMFONTHELPCONTROLLER_V01
#define	D__TMFONTHELPCONTROLLER_V01

#include	"../../font.h"


class	CtmFontHelpController:  public CtmFontControl
{
	DECLARE_DYNCREATE(CtmFontHelpController)
	public:
		CtmFontHelpController();
		~CtmFontHelpController();
	
	public:
		CtmFont*	GetFont(int nType);

		
		//int		ParseProtocol	(void* pInData,		int nInputLen);		
		
		//	override 
		int		CreateSelf();	//	overwrite
		int		FreeSelf();
	protected:
		CtmFont*	LoadFont(char* pszPath, int nFontCode, int nFontSize, BYTE* pBuf);
	
		virtual	int InitFont();
		virtual	int UnInitFont();

	private:
		int			GetFontFileSize(char* pszPath);
		int			GetMonoCaseNum(char* pszPath);

	protected:
		CtmFont*	m_pFont16;
		CtmFont*	m_pFont24;
		CtmFont*	m_pFont20;
		
		BYTE*		m_pFont16Buf;
		BYTE*		m_pFont24Buf;
		BYTE*		m_pFont20Buf;
		
		char*		m_pszFont16;
		char*		m_pszFont24;
		char*		m_pszFont20;
	
};

#ifndef	D_BEOBJECT_CTMFONTHELPCONTROLLER	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
