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
#include	"cfont.h"
#include	"gui.h"
#include	"drawtext.h"
#include 	"tmconfig.h"
#include 	"utils.h"
//#include	"memwatch.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/


/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
extern	int			g_RecordFlag;
/*==========================================================================+
|          Function implementation                                          |
+==========================================================================*/
/*==========================================================================+
|          Class implementation - CtmFont                                   |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|          Constructor and destructor                                       |
+--------------------------------------------------------------------------*/
CtmFont::CtmFont()
{
	//printf("CtmFont::CtmFont \n");
	#ifdef	D_32BIT
	m_dColor			= RGB2PIXEL888(0x0, 0x0, 0x0);
	#else
	m_dColor			= RGB2PIXEL(0x0, 0x0, 0x0);
	#endif
	m_wFormat			= tmFONT_PLAIN;
	m_pBuff				= NULL;
	m_nUNType			= 0;
	m_wUNWidth			= 16;
	m_wUNHeight			= 16;
	memset(m_szUN,0,sizeof(m_szUN));
	m_encode.SetCharset("utf-8","utf-8");
}

CtmFont::~CtmFont()
{
}

/*==========================================================================+
|	function:	Show(short x, short y, DWORD Code)							|
|	task	:	根據X，Y顯示字												|
+---------------------------------------------------------------------------+
|	call	:	CtmFont::Show(short x, short y, DWORD dCode)				|
|																			|
|	parameter:	X		X軸坐標												|
|				Y		Y軸坐標												|
|				dCode	字符代碼											|
|																			|
|	return	:	0		成功												|
|				其它	錯誤代碼											|
+==========================================================================*/
int	CtmFont::Create(tmFONT* Font)
{
	//printf("CtmFont::CtmFont Create \n");
	if(Font->pBuff == NULL)
		return 1;	
	m_Font		= *Font;
	m_pBuff		= m_Font.pBuff;
	
	InitFont(m_Font.wType);

	switch(m_Font.FT_Code)
	{
		case tmFT_CODE_GB2312:
			m_CodeBound	= 0xA0;
			break;
		case tmFT_CODE_BIG5:
			m_CodeBound = 0xA0;
			break;
		case tmFT_CODE_TECH:
			break;
		default:
			break;
	}
	return 0;
}

/*==========================================================================+
|	function:	SetColor(DWORD dColor)										|
|	task	:	設置字體顯示顏色，並返回原先的顏色							|
|---------------------------------------------------------------------------|
|	call	:	CtmFont::SetColor(DWORD dColor)								|
|																			|
|	parameter:	dColor		新的顏色										|
|																			|
|	return	:	原先的顏色													|
+==========================================================================*/
DWORD CtmFont::SetColor(DWORD dColor)
{
	DWORD	OldColor;
	DWORD Bit24Color;
	
	
	Bit24Color=RGB565TORGB888(dColor);	
	OldColor	= m_dColor;
	#ifdef	D_32BIT
	m_dColor	= Bit24Color;
	#else
	m_dColor	= dColor;
	#endif
	return OldColor;
}

DWORD	CtmFont::SetBGColor(DWORD dColor)
{
	DWORD	OldColor;
	DWORD Bit24Color;
	
	Bit24Color=RGB565TORGB888(dColor);	
	OldColor	= m_dBGColor;
	#ifdef	D_32BIT
	m_dBGColor	= Bit24Color;
	#else
	m_dBGColor	= dColor;
	#endif
	return OldColor;	
}

void    CtmFont::SetFontColor(DWORD dColor)				//兼容老版本的
{
	SetColor(dColor);
}
/*==========================================================================+
|	function:	SetFormat(WORD	wFormat)									|
|	task	:	設置字體顯示形式並返回原來的形式							|
+---------------------------------------------------------------------------+
|	call	:	CtmFont::SetFormat(WORD	wFormat)							|
|																			|
|	parameter:	wFormat		新的顯示格式									|
|																			|
|	return	:	原先的顯示格式												|
+==========================================================================*/
WORD	CtmFont::SetFormat(WORD	wFormat)
{
	WORD	OldFormat;
	
	OldFormat	= m_wFormat;
	m_wFormat	= wFormat;
	return	OldFormat;
}
/*==========================================================================+
|	function:	Show(short x, short y, DWORD Code)							|
|	task	:	根據X，Y顯示字												|
+---------------------------------------------------------------------------+
|	call	:	CtmFont::Show(short x, short y, DWORD dCode)				|
|																			|
|	parameter:	X		X軸坐標												|
|				Y		Y軸坐標												|
|				dCode	字符代碼											|
|																			|
|	return	:	0		成功												|
|				其它	錯誤代碼											|
+==========================================================================*/
int	CtmFont::Show(short nX, short nY, DWORD dCode)
{
	WORD	X_Start, X_End, Y_Start, Y_End;
	int		i, j, k;
	WORD	x, y;
	int		RowBytes;
	BYTE*	pBuf;
	BYTE*	pCode;
	DWORD	Offset;
		
	if(m_pBuff == NULL)	return tmFT_ERROR_BUFF;
	
	//=======
	//初始化變量，主要為防止編譯時出現警告
	//=======
	X_Start	= Y_Start
			= X_End
			= Y_End
			= 0;
	
	if(nX <  0)		X_Start	= -nX;
	if(nY <  0)		Y_Start	= -nY;		
	if(nX >= 0)		X_Start	= 0;
	if(nY >= 0)		Y_Start = 0;
	
	if(dCode <= 0xFF)
	{
		//=======
		//計算字體是否完全超出ViewPort范圍
		//=======
		if(nX <= -m_wWidth / 2	|| nX >= g_Length_X)	return tmFT_ERROR_AREA;
		if(nY <= -m_wHeight		|| nY >= g_Length_Y)	return tmFT_ERROR_AREA;
				
	
		if(nX <  g_Length_X - m_wWidth / 2)	X_End	= m_wWidth / 2 - 1;
		if(nY <  g_Length_Y - m_wHeight	  )	Y_End	= m_wHeight  - 1;
		if(nX >= g_Length_X - m_wWidth / 2)	X_End	= g_Length_X - nX - 1;
		if(nY >= g_Length_Y - m_wHeight	  )	Y_End	= g_Length_Y - nY - 1;
		
		Offset	= dCode * m_iCharLen / 2;
	}
	else
	{
		//=======
		//計算字體是否完全超出ViewPort范圍
		//=======
		if(nX <= -m_wWidth  || nX >= g_Length_X)	return tmFT_ERROR_AREA;
		if(nY <= -m_wHeight || nY >= g_Length_Y)	return tmFT_ERROR_AREA;		
		
		//========
		//計算字體和ViewPort的剪切
		//========
		
		if(nX <  g_Length_X - m_wWidth )	X_End	= m_wWidth	- 1;
		if(nY <  g_Length_Y - m_wHeight)	Y_End	= m_wHeight - 1;
		if(nX >= g_Length_X - m_wWidth )	X_End	= g_Length_X - nX - 1;
		if(nY >= g_Length_Y - m_wHeight)	Y_End	= g_Length_Y - nY - 1;
		
		//=======
		//計算字體在字庫中的位置
		//暫時用判斷，等24點陣字庫做完了，就可以整合一起。
		//=======		
		pCode	= (BYTE *)&dCode;
		if(m_Font.FT_Code == tmFT_CODE_GB2312)
		{
			if(m_Font.wType == tmDOTFONT_16)
			{
				Offset	= (*pCode - 0xA1 + (*(pCode + 1) - 0xA1) * 94) * m_iCharLen
						+ 4096;
			}
			else
			{
				Offset	= (*pCode - 0xA1 + (*(pCode + 1) - 0xA1) * 94) * m_iCharLen;
			}
		}
		else if(m_Font.FT_Code == tmFT_CODE_BIG5)
		{
			if(m_Font.wType == tmDOTFONT_16)
			{
				int	nRow;
/*
				if(*(pCode + 1) > 0x7E && *(pCode + 1) < 0xA1)	return tmFT_ERROR_TEXT;
				
				nRow	= *(pCode + 1) - 0x40;
				if(*(pCode + 1) >= 0xA1)	
					nRow	= nRow - (0xA1 - 0x7F);
				Offset	= ((*pCode - 0xA1) * 157 + nRow) * m_iCharLen + 4096;
*/
				if(*pCode > 0x7E && *pCode < 0xA1)	return tmFT_ERROR_TEXT;
				
				nRow	= *pCode - 0x40;
				if(*pCode >= 0xA1)	
					nRow	= nRow - (0xA1 - 0x7F);
				Offset	= ((*(pCode + 1) - 0xA1) * 157 + nRow) * m_iCharLen + 4096;

/*				
				nRow	= *(pCode + 1) - 0x40;
				if(*(pCode + 1) >= 0xA1)	
					nRow	= nRow - (0xA1 - 0x7F);
				
				Offset	= ((*pCode - 0xA0) * 157 + nRow) * m_iCharLen + 4096
							+ 0x20;
*/
			}
			else
				return tmFT_ERROR_TEXT;
		}
		else
			return tmFT_ERROR_TEXT;
	}

	if(nX < 0)	nX	= 0;
	if(nY < 0)	nY	= 0;
	
	//=======
	//代碼已經支持字體的剪切
	//=======
	if((m_wWidth & 0x07) != 0)
		RowBytes	= (m_wWidth >> 3) + 1;
	else
		RowBytes	= m_wWidth >> 3;
	pBuf	= m_pBuff + Offset + Y_Start * RowBytes;
	
	y		= nY;	
	RowBytes= (X_End >> 3) - (X_Start >> 3);
	
	for(i = Y_Start; i <= Y_End; i ++)
	{
		x	= nX;
		j	= X_Start;
		for(k = 0; k < RowBytes; k ++)
		{			
			int	l;
			l	= j + 8 - (j & 0x07);
			for(; j < l; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					g_Rows[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}

		if((X_End & 0x07) != 0)
		{
			for(; j < X_End; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					g_Rows[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}
		y ++;
	}
	return tmFT_SUCCESS;
}


//	fans add 2007/2/6 08:12上午
#ifdef	D_32BIT
int		CtmFont::ShowD(unsigned long** pSource, short nX, short nY, DWORD dCode)
#else
int		CtmFont::ShowD(unsigned short** pSource, short nX, short nY, DWORD dCode)
#endif
{
	WORD	X_Start, X_End, Y_Start, Y_End;
	int		i, j, k;
	WORD	x, y;
	int		RowBytes;
	BYTE*	pBuf;
	BYTE*	pCode;
	DWORD	Offset;
		
	if(m_pBuff == NULL)	return tmFT_ERROR_BUFF;
	
	//=======
	//初始化變量，主要為防止編譯時出現警告
	//=======
	X_Start	= Y_Start
			= X_End
			= Y_End
			= 0;
	
	if(nX <  0)		X_Start	= -nX;
	if(nY <  0)		Y_Start	= -nY;		
	if(nX >= 0)		X_Start	= 0;
	if(nY >= 0)		Y_Start = 0;
	
	if(dCode <= 0xFF)
	{
		//=======
		//計算字體是否完全超出ViewPort范圍
		//=======
		if(nX <= -m_wWidth / 2	|| nX >= g_Length_X)	return tmFT_ERROR_AREA;
		if(nY <= -m_wHeight		|| nY >= g_Length_Y)	return tmFT_ERROR_AREA;
				
	
		if(nX <  g_Length_X - m_wWidth / 2)	X_End	= m_wWidth / 2 - 1;
		if(nY <  g_Length_Y - m_wHeight	  )	Y_End	= m_wHeight  - 1;
		if(nX >= g_Length_X - m_wWidth / 2)	X_End	= g_Length_X - nX - 1;
		if(nY >= g_Length_Y - m_wHeight	  )	Y_End	= g_Length_Y - nY - 1;
		
		Offset	= dCode * m_iCharLen / 2;
	}
	else
	{
		//=======
		//計算字體是否完全超出ViewPort范圍
		//=======
		if(nX <= -m_wWidth  || nX >= g_Length_X)	return tmFT_ERROR_AREA;
		if(nY <= -m_wHeight || nY >= g_Length_Y)	return tmFT_ERROR_AREA;		
		
		//========
		//計算字體和ViewPort的剪切
		//========
			
		if(nX <  g_Length_X - m_wWidth )	X_End	= m_wWidth	- 1;
		if(nY <  g_Length_Y - m_wHeight)	Y_End	= m_wHeight - 1;
		if(nX >= g_Length_X - m_wWidth )	X_End	= g_Length_X - nX - 1;
		if(nY >= g_Length_Y - m_wHeight)	Y_End	= g_Length_Y - nY - 1;
		
		//=======
		//計算字體在字庫中的位置
		//暫時用判斷，等24點陣字庫做完了，就可以整合一起。
		//=======		
		pCode	= (BYTE *)&dCode;
		if(m_Font.FT_Code == tmFT_CODE_GB2312)
		{
			if(m_Font.wType == tmDOTFONT_16)
			{
				Offset	= (*pCode - 0xA1 + (*(pCode + 1) - 0xA1) * 94) * m_iCharLen
						+ 4096;
			}
			else
			{
				Offset	= (*pCode - 0xA1 + (*(pCode + 1) - 0xA1) * 94) * m_iCharLen;
			}
		}
		else if(m_Font.FT_Code == tmFT_CODE_BIG5)
		{
			if(m_Font.wType == tmDOTFONT_16)
			{
				int	nRow;
/*
				if(*(pCode + 1) > 0x7E && *(pCode + 1) < 0xA1)	return tmFT_ERROR_TEXT;
				
				nRow	= *(pCode + 1) - 0x40;
				if(*(pCode + 1) >= 0xA1)	
					nRow	= nRow - (0xA1 - 0x7F);
				Offset	= ((*pCode - 0xA1) * 157 + nRow) * m_iCharLen + 4096;
*/
				if(*pCode > 0x7E && *pCode < 0xA1)	return tmFT_ERROR_TEXT;
				
				nRow	= *pCode - 0x40;
				if(*pCode >= 0xA1)	
					nRow	= nRow - (0xA1 - 0x7F);
				Offset	= ((*(pCode + 1) - 0xA1) * 157 + nRow) * m_iCharLen + 4096;

/*				
				nRow	= *(pCode + 1) - 0x40;
				if(*(pCode + 1) >= 0xA1)	
					nRow	= nRow - (0xA1 - 0x7F);
				
				Offset	= ((*pCode - 0xA0) * 157 + nRow) * m_iCharLen + 4096
							+ 0x20;
*/
			}
			else
				return tmFT_ERROR_TEXT;
		}
		else
			return tmFT_ERROR_TEXT;
	}

	if(nX < 0)	nX	= 0;
	if(nY < 0)	nY	= 0;
	
	//=======
	//代碼已經支持字體的剪切
	//=======
	if((m_wWidth & 0x07) != 0)
		RowBytes	= (m_wWidth >> 3) + 1;
	else
		RowBytes	= m_wWidth >> 3;
	pBuf	= m_pBuff + Offset + Y_Start * RowBytes;
	
	y		= nY;	
	RowBytes= (X_End >> 3) - (X_Start >> 3);
	
	for(i = Y_Start; i <= Y_End; i ++)
	{
		x	= nX;
		j	= X_Start;
		for(k = 0; k < RowBytes; k ++)
		{			
			int	l;
			l	= j + 8 - (j & 0x07);
			for(; j < l; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					pSource[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}

		if((X_End & 0x07) != 0)
		{
			for(; j < X_End; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					pSource[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}
		y ++;
	}
	return tmFT_SUCCESS;
}

/*==========================================================================+
|	function:	ShowTM(short x, short y, DWORD Code)						|
|	task	:	根據X，Y顯示字,TM編碼										|
+---------------------------------------------------------------------------+
|	call	:	CtmFont::Show(short x, short y, DWORD dCode)				|
|																			|
|	parameter:	X		X軸坐標												|
|				Y		Y軸坐標												|
|				dCode	字符代碼											|
|																			|
|	return	:	0		成功												|
|				其它	錯誤代碼											|
+==========================================================================*/
//add by J.Wong for unicode 2016/7/19 10:52:28

int     CtmFont::ShowString(short nX, short nY, unsigned int* uniString, int nLen)
{
	int		nXStart = 0, nYStart = 0, nXEnd = 0, nYEnd = 0;
	if(g_pDrawText != NULL)
	{
		nXStart = nX;
		nYStart = nY;
		nXEnd	= g_Length_X; 
		nYEnd	= g_Length_Y;
		//printf("xS=%d YS=%d xE=%d yE=%d \n", nXStart, nYStart, nXEnd, nYEnd);
		g_pDrawText->SetFontColor(m_dColor);
		//printf("DrawText 1 \n");
		g_pDrawText->SetFontSize(m_wUNWidth,m_wUNHeight,(int)m_wAlign);
		g_pDrawText->DrawText(g_Rows,uniString, nXStart, nYStart,nXEnd, nYEnd, 0, 1, nLen);
		//printf("DrawText 2 \n");
		//text->SetBgcColor(0xFFFF22);//0x638ED0
	}
}

int     CtmFont::ShowString(short nX, short nY, char *pszString, int nType, int Unicode)
{
    WORD    *pCode;
	int     nXPos   = nX;
	int		nXStart = 0, nYStart = 0, nXEnd = 0, nYEnd = 0;
	BOOL	bIsRecord=TRUE;
	//printf("1 ShowString Unicode=%d \n", Unicode);
    if(pszString == NULL)   return tmFT_ERROR_TEXT;
    if(m_pBuff == NULL)	    return tmFT_ERROR_BUFF; 
        
	//printf("2 ShowString Unicode=%d nType=%d \n", Unicode, nType);
	
	if(Unicode >= 1)		//James add for Freetype 2015/4/27 13:02:04
	{
		//printf("pszString=%s Unicode=%d \n", pszString, Unicode);
		int	nIndex = 0;
		//if(Unicode == 2)
		//{
		char* pszDest=NULL;
		if(pszString!=NULL)
		{
			pszDest=(char*)malloc(strlen(pszString)+1);
			memset(pszDest,0,strlen(pszString)+1);
			Trim(pszDest,pszString,strlen(pszString)+1);
		}
		nIndex = g_pParseXMLRecord->GetRecordIndex(pszDest);
//		if(nIndex<0)
//		{
//			long lID=strtol(reinterpret_cast<char*>(pszString),NULL,16);
//			if(lID>=0)
//			{
//				nIndex = g_pParseXMLRecord->GetRecordIndex(lID);
//			}	
//		}
		//}
		if(nIndex<0)
		{
			nIndex = g_pParseXMLViewStr->GetRecordIndex(pszDest);
			bIsRecord=FALSE;
		}
		//printf("Unicode=%d nIndex=%d \n", Unicode, nIndex);
		if(pszDest!=NULL)
		{
			free(pszDest);	
		}
		if(nIndex >=0)
		{
			STRINGUNICODE	stringUnicode;
			if(bIsRecord)
			{
				stringUnicode = g_pParseXMLRecord->GetRecordStr(nIndex);
			}
			else
				stringUnicode = g_pParseXMLViewStr->GetRecordStr(nIndex);
			//nWidth = stringUnicode.nLen * 2 *;
			//printf("len=%d m_iCharLen=%d \n", stringUnicode.nLen, m_iCharLen);
			if(g_pDrawText != NULL)
			{
				nXStart = nX;
				nYStart = nY;
				nXEnd	= g_Length_X; 
				nYEnd	= g_Length_Y;
				//printf("xS=%d YS=%d xE=%d yE=%d \n", nXStart, nYStart, nXEnd, nYEnd);
				g_pDrawText->SetFontColor(m_dColor);
				//printf("DrawText 1 \n");
				g_pDrawText->SetFontSize(m_wUNWidth,m_wUNHeight,(int)m_wAlign);
				g_pDrawText->DrawText(g_Rows,(unsigned int*)stringUnicode.pdwStrUnicode, nXStart, nYStart,nXEnd, nYEnd, 0, 1, stringUnicode.nLen);
				//printf("DrawText 2 \n");
				//text->SetBgcColor(0xFFFF22);//0x638ED0
			}
		}
		else if(pszString!=NULL)
		{
			char*	pwch2=NULL;
			if(pszString[0]=='\0')	return tmFT_SUCCESS;
			if(nType == tmFT_CODE_TECH)
			{	
				memset(m_szUN,0,sizeof(m_szUN));
				char* temp=m_szUN;
				 pCode   = (WORD *)pszString;
				 while(*pCode != 0x0000)
				 {
				 	*temp=pszString[0];
				 	if(pszString[1]!=0)
				 	{
				 	 	*++temp=pszString[1];		
				 	}
				 	++temp;
				 	++pCode;
				 	++++pszString;	
				 }
				 pwch2=m_szUN; 	
			}
			else
			{
				pwch2=(char*)pszString;
			}
			STRINGUNICODE	stringUnicode;
			int nLength	= (strlen(pwch2)+2)*4;
			
			unsigned long* unicode=(unsigned long*)malloc(nLength);
			int nLen = 0, nUTFLen = 0;
			
			nLen = m_encode.Utf8ConvertUnicode((unsigned char*)pwch2,unicode, &nUTFLen);
			if(CtmConfig::GetInstance()->GetFontConvert())
			{
				m_encode.CovertToUnicodeTable(unicode,nLen);
			}
			stringUnicode.pdwStrUnicode = (DWORD*)malloc(nLen*sizeof(DWORD));
			stringUnicode.putf8code		=NULL;
			stringUnicode.nLen			= nLen;
			stringUnicode.nUTFLen		= nUTFLen;
			memset(stringUnicode.pdwStrUnicode, 0, nLen*sizeof(DWORD));
			for(int j = 0; j < nLen ;j++)
			{
				stringUnicode.pdwStrUnicode[j] = *(unicode+j);
			}
			
			if(unicode!=NULL)
			{
				free(unicode);
				unicode=NULL;
			}
			
			if(g_pDrawText != NULL)
			{
				nXStart = nX;
				nYStart = nY;
				nXEnd	= g_Length_X/*(stringUnicode.nUTFLen+2) * m_wUNWidth/2*/;
				nYEnd	= g_Length_Y;
				g_pDrawText->SetFontColor(m_dColor);
				g_pDrawText->SetFontSize(m_wUNWidth,m_wUNHeight,(int)m_wAlign);
				g_pDrawText->DrawText(g_Rows,(unsigned int*)stringUnicode.pdwStrUnicode, nXStart, nYStart,nXEnd, nYEnd, 0, 1, stringUnicode.nLen);
			}
			if(stringUnicode.pdwStrUnicode!=NULL)
			{
				free(stringUnicode.pdwStrUnicode);		
			}
		}
	}
	else
	{
	    if(nType != tmFT_CODE_TECH)
	    {
	        while(*pszString != 0)
	        {
	            if(*pszString == '\n')
	            {
	                nY += m_wHeight;
	                pszString ++;
	                nX  = nXPos;
	                continue;
	            }
	            SetTMStreamBuf( nX, nY, *pszString);
	            pszString ++;
	            nX  += m_wWidth >> 1;
	        }
	    }
	    else
	    {
	        pCode   = (WORD *)pszString;    
	        while(*pCode != 0x0000)
	        {
	            if(*pCode == '\n')
	            {
	                nY += m_wHeight;
	                pCode ++;
	                nX  = nXPos;
	                continue;
	            }
				//printf("nX=%d nY=%d \n", nX, nY);
	           	SetTMStreamBuf( nX, nY, *pCode);
	            if(*pCode >= FT_DIVISION)           nX  += m_wWidth;
	            else                                nX  += m_wWidth >> 1;               
	            pCode ++;
	        }
	    }
	}
    return tmFT_SUCCESS;
}

int		CtmFont::SetUNFontType(int nType, WORD wWidth, WORD wHeight, WORD wAlign)
{
	m_nUNType 	= nType;
	m_wUNWidth	= wWidth;	
	m_wUNHeight	= wHeight;
	m_wAlign	= wAlign;
}

#ifdef	D_32BIT
int		CtmFont::ShowStringD(unsigned long** pSource, short nX, short nY, char *pszString, int nType)
#else
int		CtmFont::ShowStringD(unsigned short** pSource, short nX, short nY, char *pszString, int nType)
#endif
{
    WORD    *pCode;
	int     nXPos   = nX;
	
    if(pszString == NULL)   return tmFT_ERROR_TEXT;
    if(m_pBuff == NULL)	    return tmFT_ERROR_BUFF; 
        
    if(nType != tmFT_CODE_TECH)
    {
        while(*pszString != 0)
        {
            if(*pszString == '\n')
            {
                nY += m_wHeight;
                pszString ++;
                nX  = nXPos;
                continue;
            }
            ShowTMD(pSource, nX, nY, *pszString);
            pszString ++;
            nX  += m_wWidth >> 1;
        }
    }
    else
    {
        pCode   = (WORD *)pszString;    
        while(*pCode != 0x0000)
        {
            if(*pCode == '\n')
            {
                nY += m_wHeight;
                pCode ++;
                nX  = nXPos;
                continue;
            }
            ShowTMD(pSource, nX, nY, *pCode);
            if(*pCode >= FT_DIVISION)           nX  += m_wWidth;
            else                                nX  += m_wWidth >> 1;               
            pCode ++;
        }
    }
    return tmFT_SUCCESS;
}

#ifdef	D_32BIT
int		CtmFont::ShowTMD(unsigned long** pSource, short nX, short nY, DWORD dCode)
#else
int		CtmFont::ShowTMD(unsigned short** pSource, short nX, short nY, DWORD dCode)
#endif
{
    BYTE    *pBuf;
    WORD	X_Start, X_End, Y_Start, Y_End;
    int		RowBytes;
    int		i, j, k;
	WORD	x, y;
    
    if(nX <  0)		X_Start	= -nX;
    else            X_Start	= 0;
	if(nY <  0)		Y_Start	= -nY;		
	else    		Y_Start = 0;    
    
    if(dCode < FT_DIVISION)    // halfwidth
    {
        //=======
		//計算字體是否完全超出ViewPort范圍
		//=======
		if(nX <= -m_wWidth / 2	|| nX >= g_Length_X)	return tmFT_ERROR_AREA;
		if(nY <= -m_wHeight		|| nY >= g_Length_Y)	return tmFT_ERROR_AREA;
				
	
		if(nX <  g_Length_X - m_wWidth / 2)	X_End	= m_wWidth / 2 - 1;
		if(nY <  g_Length_Y - m_wHeight	  )	Y_End	= m_wHeight  - 1;
		if(nX >= g_Length_X - m_wWidth / 2)	X_End	= g_Length_X - nX - 1;
		if(nY >= g_Length_Y - m_wHeight	  )	Y_End	= g_Length_Y - nY - 1;
        
        RowBytes	= m_wWidth >> 4;
        if((m_wWidth / 2 & 0x07) != 0)  RowBytes ++;       
                 
        if(dCode * RowBytes * m_wHeight > m_Font.nDivision) return tmFT_ERROR_TEXT;  
        pBuf   = m_pBuff + dCode * RowBytes * m_wHeight + Y_Start * RowBytes;   
        
    }
    else
    {
        //=======
		//計算字體是否完全超出ViewPort范圍
		//=======
		if(nX <= -m_wWidth  || nX >= g_Length_X)	return tmFT_ERROR_AREA;
		if(nY <= -m_wHeight || nY >= g_Length_Y)	return tmFT_ERROR_AREA;		
		
		//========
		//計算字體和ViewPort的剪切
		//========
			
		if(nX <  g_Length_X - m_wWidth )	X_End	= m_wWidth	- 1;
		if(nY <  g_Length_Y - m_wHeight)	Y_End	= m_wHeight - 1;
		if(nX >= g_Length_X - m_wWidth )	X_End	= g_Length_X - nX - 1;
		if(nY >= g_Length_Y - m_wHeight)	Y_End	= g_Length_Y - nY - 1;
		    
        RowBytes	= m_wWidth >> 3;
        if((m_wWidth & 0x07) != 0)  RowBytes ++;
        if(m_Font.nDivision + (dCode - FT_DIVISION) * RowBytes * m_wHeight > m_Font.lSize)  return tmFT_ERROR_TEXT;
        pBuf   = m_pBuff + m_Font.nDivision + (dCode - FT_DIVISION) * RowBytes * m_wHeight + Y_Start * RowBytes;
    }
    
	//=======
	//代碼已經支持字體的剪切
	//=======
	if(nX < 0)	nX	= 0;
	if(nY < 0)	nY	= 0;
	y		= nY;	
	RowBytes= (X_End >> 3) - (X_Start >> 3);
	
    for(i = Y_Start; i <= Y_End; i ++)
	{
		x	= nX;
		j	= X_Start;
		for(k = 0; k < RowBytes; k ++)
		{			
			int	l;
			l	= j + 8 - (j & 0x07);
			for(; j < l; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					pSource[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}

		if((X_End & 0x07) != 0)
		{
			for(; j <= X_End; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					pSource[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}
		y ++;
	}
    
    return tmFT_SUCCESS;
}
/*==========================================================================+
|	function:	ShowFontString( nX,  nY,  *pszString,  nLen,  nType)		|
|	task	:	Depending on the *pszString to show received font flow.		|
+---------------------------------------------------------------------------+
|	call	:	CrmWnd::ShowFontStream(...)									|
|																			|
|	parameter:	X		X coordinate										|
|				Y		Y coordinate										|
|			pszString	Font stream encoding								|
|			   nLen		Font stream encoding length							|
|			  nType		Determine the decision in English or Chinese Width  |
|																			|
|	return	:	0		Success												|
|				Other	Error code											|
+==========================================================================*/
int     CtmFont::ShowFontString(short nX, short nY, char *pszString, int nLen, int nType)	//rw.ying
{
	if(pszString == NULL)   return tmFT_ERROR_TEXT;
	int		nTotalNum = 0;
	nTotalNum = nLen/m_iCharLen;
	int		nWidth = 0;
	BYTE    *pBuf = NULL;
	
	for(int i = 0; i < nTotalNum; i++)
	{
		pBuf = (BYTE*)pszString;
		if(nType == 0)
			nWidth = m_wWidth;
		else
			nWidth = m_wWidth / 2;
		ShowStream(nX, nY, nWidth, pBuf);
		pszString += m_iCharLen;
		nX += m_wWidth;
	}
	return tmFT_SUCCESS;
}
/*==========================================================================+
|	function:	ShowStream(short nX,short nY,int nWidth, BYTE *pBuf)		|
|	task	:	According to font flow encoding and print on the screen 	|
+---------------------------------------------------------------------------+
|	call	:	CtmFont::ShowFontString(...) or CtmFont::SetTMStreamBuf() |
|																			|
|	parameter:	X		X coordinate										|
|				Y		Y coordinate										|
|				nWidth	Decision font width									|
|				pBuf	Point to the font flow								|
|																			|
|	return	:	0		Success												|
|				Other	Error code											|
+==========================================================================*/
int		CtmFont::ShowStream(short nX,short nY,int nWidth, BYTE *pBuf)		//rw.ying 2011/3/23 11:35上午
{
	if(pBuf == NULL)
		return tmFT_ERROR_TEXT;
    WORD	X_Start, X_End, Y_End, Y_Start;
    int		i, j, k;
	WORD	x, y;
	int		RowBytes;
    if(nX <  0)		X_Start	= -nX;
    else            X_Start	= 0;
    if(nY <  0)		Y_Start	= -nY;		
	else    		Y_Start = 0;   
    	
   	if(nX <= -nWidth	|| (nX + nWidth -1) >= g_Length_X)	
   		return tmFT_ERROR_AREA;
	if(nY <= -m_wHeight		|| nY >= g_Length_Y)
		return tmFT_ERROR_AREA;				
	
	if(nX <  g_Length_X - nWidth)		X_End	= nWidth - 1;
	if(nY <  g_Length_Y - m_wHeight)	Y_End	= m_wHeight  - 1;
	if(nX >= g_Length_X - nWidth)		X_End	= g_Length_X - nX - 1;
	if(nY >= g_Length_Y - m_wHeight)	Y_End	= g_Length_Y - nY - 1;       
    
	if(nX < 0)	nX	= 0;
	if(nY < 0)	nY	= 0;
	y		= nY;	
	RowBytes= (X_End >> 3) - (X_Start >> 3);

	for(i = Y_Start; i <= Y_End; i ++)
	{
		x	= nX;
		j	= X_Start;
		for(k = 0; k < RowBytes; k ++)
		{			
			int	l;
			l	= j + 8 - (j & 0x07);
			for(; j < l; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					g_Rows[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}	
		if((X_End & 0x07) != 0)
		{
			for(; j <= X_End; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					g_Rows[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}
		y ++;
	}
	return tmFT_SUCCESS;
}

int		CtmFont::ShowUNStream(short nX, short nY, char* pszString)
{
//	int	nIndex = 0;
//	if(pszString == NULL)
//		return tmFT_ERROR_TEXT;
//	WORD	nWidth = m_wWidth;
//    WORD	X_Start, X_End, Y_End, Y_Start;
//    int		i, j, k;
//	WORD	x, y;
//	int		RowBytes;
//    if(nX <  0)		X_Start	= -nX;
//    else            X_Start	= 0;
//    if(nY <  0)		Y_Start	= -nY;		
//	else    		Y_Start = 0;   
//    
//   	//if(nX <= -nWidth	|| (nX + nWidth -1) >= g_Length_X)	
//   	//	return tmFT_ERROR_AREA;
//	if(nY <= -m_wHeight		|| nY >= g_Length_Y)
//		return tmFT_ERROR_AREA;				
//	
//	if(nX <  g_Length_X - nWidth)		X_End	= nWidth - 1;
//	if(nY <  g_Length_Y - m_wHeight)	Y_End	= m_wHeight  - 1;
//	if(nX >= g_Length_X - nWidth)		X_End	= g_Length_X - nX - 1;
//	if(nY >= g_Length_Y - m_wHeight)	Y_End	= g_Length_Y - nY - 1;       
//    
//	if(nX < 0)	nX	= 0;
//	if(nY < 0)	nY	= 0;
//	y		= nY;	
//	
	
/*	RowBytes= (X_End >> 3) - (X_Start >> 3);


	for(i = Y_Start; i <= Y_End; i ++)
	{
		x	= nX;
		j	= X_Start;
		for(k = 0; k < RowBytes; k ++)
		{			
			int	l;
			l	= j + 8 - (j & 0x07);
			for(; j < l; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					g_Rows[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}	
		if((X_End & 0x07) != 0)
		{
			for(; j <= X_End; j ++)
			{
				if(1 << 7 - j + (k << 3) & *pBuf)
					g_Rows[y][x]	= m_dColor;
				x ++;
			}
			pBuf ++;
		}
		y ++;
	}*/
	return tmFT_SUCCESS;
}
/*==========================================================================+
|	function:	SetTMStreamBuf(short nX, short nY, DWORD dCode)				|
|	task	:	According to dCode to find the offset in buffer memory		|
+---------------------------------------------------------------------------+
|	call	:	CtmFont::ShowString(...)									|
|																			|
|	parameter:	X		X coordinate										|
|				Y		Y coordinate										|
|				dCode	The index offset in the font exist buf				|
|																			|
|	return	:	0		Success												|
|				Other	Error code											|
+==========================================================================*/
int		CtmFont::SetTMStreamBuf(short nX, short nY, DWORD dCode)	//rw.ying 2011/3/20 01:44下午
{
	BYTE*	pBuf 		= NULL;
	int		nWidth 		= 0;	
	int		RowBytes 	= 0;
	WORD	Y_Start;
	if(nY <  0)		Y_Start	= -nY;
	else    		Y_Start = 0;
    if(dCode < FT_DIVISION)
    {
    	nWidth = m_wWidth / 2;
        RowBytes	= m_wWidth >> 4;
        if((m_wWidth / 2 & 0x07) != 0)  RowBytes ++;
        if(dCode * RowBytes * m_wHeight > m_Font.nDivision) return tmFT_ERROR_TEXT;
        if(RowBytes > 1 && dCode * 48 > m_Font.nDivision)
 			return tmFT_ERROR_TEXT;
        pBuf   = m_pBuff + dCode * RowBytes * m_wHeight + Y_Start * RowBytes;   
    }
    else
    {
    	nWidth = m_wWidth;
        RowBytes	= m_wWidth >> 3;
        if((m_wWidth & 0x07) != 0)  RowBytes ++;
        if(m_Font.nDivision + (dCode - FT_DIVISION) * RowBytes * m_wHeight > m_Font.lSize)  return tmFT_ERROR_TEXT;
        pBuf   = m_pBuff + m_Font.nDivision + (dCode - FT_DIVISION) * RowBytes * m_wHeight + Y_Start * RowBytes;
    }
    //printf("m_Font.lSize=%d H=%d W=%d\n", m_Font.lSize, m_wHeight, m_wWidth);
    ShowStream(nX, nY, nWidth, pBuf);
}

void		CtmFont::InitFont(int nWidth)			//rw.ying 2011/3/24 01:59下午
{
	m_wWidth 	= nWidth;
	m_wHeight 	= ((m_wWidth>>3) + (m_wWidth&7?1:0)) * 8;
	m_iCharLen 	= m_wHeight/8*m_wHeight;
}

