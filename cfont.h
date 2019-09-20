/*==========================================================================+
|  Class    : CFont                                            				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/06/23                                                    |
|  Remark	: 初始化Framebuffer。                                           |
+==========================================================================*/
#ifndef	_CFONT_H
#define	_CFONT_H

#include	"common.h"
#include	"ParseXMLRecord.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
/*==========================================================================+
|           Macro definition                                                |
+==========================================================================*/
#define		tmMAX_FONTNUM			100
#define		tmNULL_FONT				-1

#define		tmFT_SUCCESS			0
#define		tmFT_ERROR_FILE			1		//打不開字庫文件
#define		tmFT_ERROR_SIZE			2		//字體大小不存在
#define		tmFT_ERROR_TEXT			3		//字符串錯誤
#define		tmFT_ERROR_AREA			4		//指定范圍超出ViewPort
#define		tmFT_ERROR_BUFF			5		//無法分配點陣空間

#define     tmFONT_PLAIN			0x0000
#define     tmFONT_OPAQUE			0x8000
#define     tmFONT_BOLD           	0x0001
#define     tmFONT_UNDERLINE      	0x0002
#define     tmFONT_SHADOW         	0x0004
#define     tmFONT_EMBOSS         	0x0008
#define     tmFONT_INSET          	0x0010

#define		tmDOTFONT_16			16
#define		tmDOTFONT_24			24
#define     FT_DIVISION     0x5000

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef	enum	tagtmFT_CODE
{
	tmFT_CODE_UNICODE		= 0,
	tmFT_CODE_GB2312,
	tmFT_CODE_BIG5,
	tmFT_CODE_TECH,						//宏訊自定義編碼
	tmFT_CODE_MAX
}	tmFT_CODE;

typedef struct tagtmCFONT
{
    DWORD		iSize;					//Struct Size
    //tmFT_CODE	FT_Code;				//code page	// <FANS> 2006/11/20 10:59上午mark
    int			FT_Code;
    WORD		wType;					//字體類型
    int         nDivision;
    long        lSize;
	BYTE		*pBuff;					//Font Buff
}	tmFONT;

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - CtmFont                                		|
+==========================================================================*/
class	CtmFont
{
public:
	CtmFont();
	virtual	~CtmFont();
	
public:
	WORD	m_wWidth;					//字體的寬度
	WORD	m_wHeight;					//字體的高度
	WORD	m_wFormat;
	DWORD	m_iCharLen;
	
	int		m_nUNType;
	WORD	m_wUNWidth;
	WORD	m_wUNHeight;
	WORD	m_wAlign;
	
	int		Create(tmFONT* Font);	
	DWORD	SetColor(DWORD dColor);
	DWORD	SetBGColor(DWORD dColor);
	WORD	SetFormat(WORD wFormat);
	void	SetFontColor(DWORD dColor);				//兼容老版本的
	int     GetFontCode()   {return m_Font.FT_Code;};
	#ifdef	D_32BIT
	int		ShowD(unsigned long** pSource, short nX, short nY, DWORD dCode);
	#else
	int		ShowD(unsigned short** pSource, short nX, short nY, DWORD dCode);
	#endif
	
	int		Show(short nX, short nY, DWORD dCode);
    int     ShowString(short nX, short nY, char *pszString, int nType, int Unicode = 0);
	int     ShowString(short nX, short nY, unsigned int* uniString, int nLen);
	#ifdef	D_32BIT
    int		ShowStringD(unsigned long** pSource, short nX, short nY, char *pszString, int nType);
	#else
    int		ShowStringD(unsigned short** pSource, short nX, short nY, char *pszString, int nType);
    #endif
    int     ShowFontString(short nX, short nY, char *pszString, int nLen, int nType);   //rw.ying 2011/1/13 10:32上午

	int		SetUNFontType(int nType, WORD wWidth, WORD wHeight,WORD wAlign=1);
private:	
	DWORD	m_dColor;				//Font Color
	DWORD	m_dBGColor;
	CtmEncode	m_encode;
	char	m_szUN[1024];
	DWORD	m_CodeBound;
	tmFONT	m_Font;
	BYTE*	m_pBuff;
	int     m_nDivision;
	#ifdef	D_32BIT
	int		ShowTMD(unsigned long** pSource, short nX, short nY, DWORD dCode);
	#else
	int		ShowTMD(unsigned short** pSource, short nX, short nY, DWORD dCode);
	#endif
	
	//int		ShowTM_16(short nX, short nY, DWORD dCode);
	//int		ShowTM_24(short nX, short nY, DWORD dCode);
	
	int		ShowStream(short nX,short nY,int nWidth, BYTE *pBuf);		//rw.ying
	int		SetTMStreamBuf(short nX, short nY, DWORD dCode);
	int		ShowUNStream(short nX, short nY, char* pszString);
	void	InitFont(int nWidth);
};

#endif

