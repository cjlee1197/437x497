#ifndef D_DRAWTEXT_H
#define D_DRAWTEXT_H

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
#include "fontcache.h"
#include <guilite/IGui>


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
        CtmDrawText(const char *pszFontFile);
		~CtmDrawText();
	public:
        FT_Error 		Init();
		void			Free();
        FT_Error		LoadFontFace(const char* pszFontFile);
        void            FreeFontFace();
		const			char* GetFontFile(){return m_pszFontFile;};
		FT_Error		SetEnCoding(FT_Encoding encoding);
		FT_Encoding		GetEncoding()	{return m_enCoding ;};
		FT_Error		SetFontSize(FT_UInt Width,FT_UInt Height,int nAlign=1);
        FT_UInt			GetFontHeight(){return m_nHeight;};
		FT_UInt			GetFontWidth(){return m_nWidth;};
        FT_UInt			GetFontAlign(){return m_nAlign;}
       	void            SetFontAngle(int angle);
        int				SetFontAngle(){return m_nAngle;};
		void			SetFontColor(unsigned int color);
		void			SetBgcColor(unsigned int color);
		unsigned int 	GetFontColor(){return m_nFontColor;};
		unsigned int	GetBgcColor(){return m_nBgcColor;};
		
		int				GetFontWidth(unsigned int* pdwCode);
		int				GetFontWidth(unsigned char* cCode,SIZE size);
		int				SetHorizontalOffset(int nOffsetBytes);
		
        #ifdef	D_32BIT
        void			DrawText(unsigned long** pSource,unsigned int* pdwCode,int nStartX, int nStartY,int nEndX, int nEndY,int angle=0, int flag=0, int nLen= -1);
    	void			DrawText(unsigned long** pSource,unsigned char* pcCode,int nStartX, int nStartY,int nEndX, int nEndY,int angle=0, int flag=0, int nLen= -1);
    	void 			DrawText(unsigned long** pSource,unsigned short *pwCode, int nStartX, int nStartY, int nEndX, int nEndY,int angle=0, int flag=0, int nLen= -1);
        void			DrawText(CImage& image, unsigned char* pcCode, CRect rect, int nLen=-1);
		void			DrawText(CImage& image, unsigned int* pwCode, CRect rect, int nLen=-1);
        unsigned int  	DrawWord(CImage& image,unsigned int dwCode, int nStartX, int nStartY, int nEndX, int nEndY, int angle, int flag);
        unsigned int    DrawWord(unsigned long** pSource,unsigned int dwCode, int nStartX, int nStartY,int nEndX, int nEndY, int angle=0, int flag=0);
        #else
        void			DrawText(unsigned short** pSource,unsigned int* pdwCode,int nStartX, int nStartY,int nEndX, int nEndY,int angle=0, int flag=0, int nLen= -1);
    	void			DrawText(unsigned short** pSource,unsigned char* pcCode,int nStartX, int nStartY,int nEndX, int nEndY,int angle=0, int flag=0, int nLen= -1);
    	void 			DrawText(unsigned short** pSource,unsigned short *pwCode, int nStartX, int nStartY, int nEndX, int nEndY,int angle=0, int flag=0, int nLen= -1);
        unsigned int    DrawWord(unsigned short** pSource,unsigned int dwCode, int nStartX, int nStartY,int nEndX, int nEndY, int angle=0, int flag=0);
        #endif
        
        int				GetTextLength(unsigned int* dwCode, int uLen=-1);
		int				GetTextLength(unsigned char* cCode, int uLen=-1);
		int				GetTextLength(unsigned char* cCode, SIZE size,int uLen=-1);
		CPoint 			GetCharPosition(unsigned char* cCode, int uLen, unsigned char cKey,SIZE size,CRect rect,unsigned short wAlign, int offset_bytes=0);
	private:
		Glyph*			GetGlyph(unsigned int dwCode, SIZE size);
		bool 			GetGlyph(unsigned int dwCode, int size, Glyph& glyph);
		int 			ResetHorizontalOffset();	
	private:
		FT_Library    m_pFTLib;
		FT_Face       m_pFTFace;
        FT_Error      m_error;
        char*         m_pszFontFile;
		FT_Encoding	  m_enCoding;
		FT_Glyph      m_glyph;
		FT_UInt		  m_nHeight;
		FT_UInt		  m_nWidth;
		int			  m_nAlign;
		FT_Matrix	  m_matrix;
		FT_Pos		  m_nAscender;
		FT_Pos		  m_nTop;
		FT_Pos		  m_nSlotHeight;
		FT_Bitmap 	  m_nBitmap;
		int 		  m_nAngle;
	    float 		  m_fConva;
		unsigned 	 int m_nFontColor;
		unsigned	 int m_nBgcColor;
		int			 m_nUnicodeCount;
		FontCache*	 m_cache;
		TMMATRICS	 m_matrics;
		int			 m_nOffsetHalfWidth;
		int			 m_nTotalWidth;
		int			 m_nOffset;	
};

extern	CtmDrawText*	g_pDrawText;

#endif
