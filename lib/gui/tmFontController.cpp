
#include	"tmFontController.h"
#include	"../../inifile.h"
#include	"../../drawtext.h"
#include	"../../tmconfig.h"

int	u_nFontCount = 0;
CIniFile	 m_FontIni;	

IMPLEMENT_DYNCREATE(CtmFontController, CtmFontControl)

CtmFontController::CtmFontController(): CtmFontControl()
{
	m_pFont16 		= NULL;
	m_pFont24 		= NULL;	
	m_pFont20		= NULL;
	m_pFont16Buf 	= NULL;
	m_pFont24Buf 	= NULL;
	m_pFont20Buf 	= NULL;
	m_pszFont16		= NULL;
	m_pszFont24		= NULL;
	m_pszFont20		= NULL;
}

CtmFontController::~CtmFontController()
{
	
}


int		CtmFontController::CreateSelf()
{
	CtmFontControl::CreateSelf();
			
	InitFont();
		
	return 0;
}





int		CtmFontController::FreeSelf()
{
	CtmFontControl::FreeSelf();
		
	UnInitFont();
	return 0;
}


int 		CtmFontController::InitFont()
{
	int n16Size = 0;
	int n24Size = 0;
	int	n20Size	= 0;
	char*	pFTFileName = NULL;
	char*	pPath = NULL;
	char	pszFilePath[256];
	
	m_FontIni.OpenFile("outerconfig.ini");
	
	m_FontIni.ReadString("font",	"font16", 			NULL, &m_pszFont16);
	m_FontIni.ReadString("font",	"font24", 			NULL, &m_pszFont24);
	m_FontIni.ReadString("font",	"font20", 			NULL, &m_pszFont20);
	m_FontIni.CloseFile();
	
	
	
	
	n16Size = GetFontFileSize(m_pszFont16);
	n24Size = GetFontFileSize(m_pszFont24);
	n20Size	= GetFontFileSize(m_pszFont20);
	
	if (n16Size > 0) m_pFont16Buf = (BYTE*)malloc(n16Size);
	else			 perror("no font16 file!\n");
	
	if (n24Size > 0) m_pFont24Buf = (BYTE*)malloc(n24Size);
	else			 perror("no font24 file!\n");
		
	if (n20Size > 0) m_pFont20Buf = (BYTE*)malloc(n20Size);
	else			 perror("no font20 file!\n");

	//James add 2015/4/27 11:04:36 for Freetype
	if(g_pDrawText == NULL)
	{
		//pPath	= CtmConfig::GetInstance()->GetLanguagepath();
		pFTFileName = CtmConfig::GetInstance()->GetFontFTFileName();
		//sprintf(pszFilePath, "%s%s", pPath, pFTFileName);
		sprintf(pszFilePath, "/usr/local/fonts/%s", pFTFileName);
		
		//if(pFTFileName != NULL)
		//	printf("InitFont() pFTFileName=%s, pszFilePath=%s \n", pFTFileName, pszFilePath);
		//else
		//	printf("InitFont() pFTFileName NULL\n");
		g_pDrawText = new CtmDrawText(pszFilePath);		//msyh.ttc yahei-changgui  msyhl.ttc yahei-xiti  simhei.ttf heiti  simsun.ttc songti
		g_pDrawText->SetEnCoding(FT_ENCODING_UNICODE);
	}
	//g_pDrawText->SetFontSize(16,16);
	

	m_pFont16 = LoadFont(m_pszFont16, 3, 16, m_pFont16Buf);
	m_pFont24 = LoadFont(m_pszFont24, 3, 24, m_pFont24Buf);
	m_pFont20 = LoadFont(m_pszFont20, 3, 20, m_pFont20Buf);
	
	return 0;
}

int 	CtmFontController::UnInitFont()
{
	delete m_pFont16;
	delete m_pFont24;
	delete m_pFont20;
	
	if (m_pFont16Buf != NULL)	free(m_pFont16Buf);
	if (m_pFont24Buf != NULL)	free(m_pFont24Buf);
	if (m_pFont20Buf != NULL)	free(m_pFont20Buf);
		
	delete	[]m_pszFont16;
	delete	[]m_pszFont24;
	delete	[]m_pszFont20;
	
	return 0;
}


CtmFont*	CtmFontController::GetFont(int nType)
{
	switch(nType)
	{
		case 0:
			return m_pFont16;
		case 1:
			return m_pFont24;
		case 2:
			return m_pFont20;
		default:
			return NULL;			
	}
	
}

CtmFont*	CtmFontController::SetFontType(int nType, int nWidth, int nHeight)
{
	if(g_pDrawText != NULL)
		g_pDrawText->SetFontSize(nWidth,nHeight);
	
}

CtmFont*	CtmFontController::LoadFont(char* pszPath, int nFontCode, int nFontSize, BYTE* pBuf)
{
	int		nFileHandle, nFileSize;	//	nFileSize is the font file's size	
	int		nHalfCount;				//	it's is monocase num; 
	int		nDivision, nTempFontSize;
	
	nFileHandle = open(pszPath, O_RDONLY);
	if (nFileHandle < 0) return NULL;	
	nFileSize	= lseek(nFileHandle, 0, SEEK_END);	//	get the file's size
	
	//	get the monocase num
	lseek(nFileHandle, 0, SEEK_SET);
	read(nFileHandle, &nHalfCount, sizeof(nHalfCount));

	if (pBuf == NULL)	return NULL;				
	//	can't read the next content(
	if(read(nFileHandle, pBuf, nFileSize - sizeof(nHalfCount)) == -1)       
		return NULL;
	close(nFileHandle);
	
	
	nTempFontSize	= int(ceil((double)nFontSize / (double)8) * 8);
	
	//	Create the font class
	tmFONT	fontTM	= 
	{	
	    sizeof(tmFONT),	
		nFontCode, 
		nFontSize,
		nHalfCount * nTempFontSize * (int)(ceil((double)nTempFontSize / (double)8 / (double)2)),
		nFileSize - 4,
		pBuf
	};	
	
	CtmFont* pFont	= new CtmFont();
	pFont->Create(&fontTM);		
	return pFont;
}


int		CtmFontController::GetMonoCaseNum(char* pszPath)
{
	int nResult = 0;
	
	if (pszPath != NULL)
	{
		int nFileHandle = open(pszPath, O_RDONLY);
		if (nFileHandle >= 0)
		{
			lseek(nFileHandle, 0, SEEK_SET);
			read(nFileHandle, &nResult, sizeof(nResult));
			close(nFileHandle);
		}

	}

	return nResult;
}

int		CtmFontController::GetFontFileSize(char* pszPath)
{
	int nResult = 0;

	if (pszPath != NULL)
	{
		int nFileHandle = open(pszPath, O_RDONLY);
		if (nFileHandle >= 0)
		{
			nResult = lseek(nFileHandle, 0, SEEK_END);
			nResult = nResult - 4;
			close(nFileHandle);
		}
	}
	
		

	return nResult;

}



#ifndef	D_BEOBJECT_CTMFONTCONTROLLER	//	defineName=D_BEOBJECT_+CASE(ClassName) 
CObject*	CreateInstance(char* Name)
{
	if (g_pFont == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CtmPackClass*>(pResult))->CreateSelf();
		u_nFontCount++;
		g_pFont = (static_cast<CtmFontController*>(pResult));
	}
	else u_nFontCount++;
		
	return static_cast<CObject*>(g_pFont);
}

void	ReleaseInstance(CObject* pInstance)
{
	u_nFontCount--;
	if (u_nFontCount <= 0)
	{
		if (pInstance != NULL)
			(static_cast<CtmPackClass*>(pInstance))->FreeSelf();
		delete pInstance;
		pInstance 		= NULL;
		u_nFontCount 	= 0;
	}
}
#endif
