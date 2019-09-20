/*	Simplifier operation version 2 	 	*/
/*	Created by J.Wong 2017/2/8 11:05:39	*/
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <unistd.h>   
#include <stdio.h>   
#include <fcntl.h>   
#include <ft2build.h>
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include <sys/ioctl.h>
#include <guilite/IGui>
#include <vector>


typedef struct TGlyph_
{

	FT_UInt index;/*字形索引*/
	FT_Vector pos; /*基线上字形原点*/
	FT_Glyph image; /*字形图像*/		
}TGlyph,*PGlyph;

#define MAX_GLYPHS 1024
#define	PI	3.1415926
#ifdef  _UNICODE
        typedef wchar_t     TCHAR;
        #define __T(x)      L##x
        #define _T(x)       __T(x)
#else
        #define __T(x)      x
        typedef char        TCHAR;
#endif

#define RGB2PIXEL888(r,g,b)	\
	(((r) << 16) | ((g) << 8) | (b))
	
#define RGB2PIXEL565(r,g,b)	\
	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

class CtmDrawText
{
	public:
		CtmDrawText();
		CtmDrawText(const char* pszFontFile);
		~CtmDrawText();
		
		/*初始化*/ 
		FT_Error 		Init();
		void	 		Free();
		FT_Error 		LoadFontFace(const char* pszFontFile);
		void	 		FreeFontFace();
		
		const 	char* 	GetFontFile()const {return m_pszFontFile ;}
		FT_Error 		SetEnCoding(FT_Encoding encoding);
		FT_Encoding 	GetEnCoding()const {return m_enCoding;}
		FT_Error 		SetFontSize(FT_UInt Width, FT_UInt Height, int nAlign=1);
		FT_UInt 		GetFontHeight()const {return m_nHeight;}
		FT_UInt 		GetFontWidth()const {return m_nWidth; }
		int				GetFontAlign()const{return m_nAlign; }
		
		//void 			SetFontAngle(int angle);
		//int 			GetFontAngle()const {return m_nAngle; }
		
		void 			SetFontColor(unsigned int color);
		void 			SetBgcColor(unsigned int color);
		unsigned int 	GetFontColor()const{return m_nFontColor ;}
		unsigned int 	GetBgcColor()const{return m_nBgcColor; }
		void			SetEmbolden(int embolden);
		int				GetEmbolden()const{return m_nEmbolden ; }
		#ifdef D_32BIT
			void	DrawText(unsigned long** pBuffer,unsigned int* pwCode,CRect rect,int nLen=-1, bool adjust=false,bool wrap=false);
			void	DrawText(unsigned long** pBuffer,unsigned char* pcCode,CRect rect, int nLen=-1, bool adjust=false,bool wrap=true);
		#else 
			void	DrawText(unsigned long** pBuffer,unsigned short* pwCode,CRect rect,int nLen=-1, bool adjust=false,bool wrap=false);
			void	DrawText(unsigned long** pBuffer,unsigned char* pcCode,CRect rect, int nLen=-1, bool adjust=false,bool wrap=true);
		#endif
		
	private:
		FT_Library 	m_pFTLib;
		FT_Face		m_pFTFace;
		FT_Error 	m_error;
		char*		m_pszFontFile;
		FT_Encoding	m_enCoding;
		TGlyph m_glyphs[MAX_GLYPHS];
		int	m_num_glyphs;
		
		FT_UInt m_nHeight;
		FT_UInt m_nWidth;
		int 	m_nAlign;
		FT_Matrix m_matrix;
		int		m_nEmbolden;
		
		unsigned int m_nFontColor;
		unsigned int m_nBgcColor;
};

extern CtmDrawText* g_pDrawText;