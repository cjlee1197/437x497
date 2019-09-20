#include "drawtext.h"
#include <freetype/ftoutln.h> 

CtmDrawText* g_pDrawText;



CtmDrawText::CtmDrawText()
{
	m_pFTLib=NULL;
	m_pFTFace=NULL;
	m_error=0;
	m_num_glyphs=0;
	m_pszFontFile=NULL;
	m_enCoding=FT_ENCODING_UNICODE;
	m_nEmbolden = 0;
	m_nHeight=m_nWidth=16;
	m_matrix.xx=(FT_Fixed)(cos(0)*0x10000L );
	m_matrix.xy=(FT_Fixed)(-sin(0)*0x10000L);
	m_matrix.yx=(FT_Fixed)(sin(0)*0x10000L );
	m_matrix.yy=(FT_Fixed)(cos(0)*0x10000L );
	Init();
}

CtmDrawText::CtmDrawText(const char* pszFontFile)
{
	
	m_pFTLib = NULL;
	m_pFTFace = NULL;
	m_error = 0;
	m_num_glyphs=0;
	m_pszFontFile = NULL;
	m_enCoding = FT_ENCODING_UNICODE;
	m_nWidth=m_nHeight=16;
	m_nEmbolden =0;
	m_matrix.xx=(FT_Fixed)(cos(0)*0x10000L );
	m_matrix.xy=(FT_Fixed)(-sin(0)*0x10000L);
	m_matrix.yx=(FT_Fixed)(sin(0)*0x10000L );
	m_matrix.yy=(FT_Fixed)(cos(0)*0x10000L );
	
	Init();
	LoadFontFace(pszFontFile);
}

CtmDrawText::~CtmDrawText()
{
	FreeFontFace();
	if(m_pszFontFile!=NULL)
	{
		delete m_pszFontFile;
		m_pszFontFile=NULL;
	}
	Free();
}

void CtmDrawText::Free()
{
	if(m_pFTLib)
	{
		FT_Done_FreeType(m_pFTLib);
		m_pFTLib = NULL;
	}
}

FT_Error CtmDrawText::Init()
{
	m_error = FT_Init_FreeType(&m_pFTLib);
	if(m_error)
	{
		m_pFTLib = NULL;
		printf("Error:Init Freetype Libray\n");
	}
	return m_error;
}

void CtmDrawText::FreeFontFace()
{
	if(m_pFTFace)
	{
		FT_Done_Face(m_pFTFace);
		m_pFTFace = NULL;
		
	}
}

FT_Error CtmDrawText::LoadFontFace(const char* pszFontFile)
{
	if(m_pszFontFile!=NULL)
	{
		delete[] m_pszFontFile;
		m_pszFontFile = NULL;
	}
	
	m_pszFontFile = new char[strlen(pszFontFile)+1];
	memcpy(m_pszFontFile, pszFontFile, strlen(pszFontFile)+1);
	FreeFontFace();
	m_error = FT_New_Face(m_pFTLib, pszFontFile, 0, &m_pFTFace);
	return m_error;
}

FT_Error CtmDrawText::SetEnCoding(FT_Encoding encoding)
{
	if(m_pFTFace)
	{
		m_enCoding=encoding;
		m_error = FT_Select_Charmap(m_pFTFace, encoding);
		
	}
	return m_error;
}

FT_Error CtmDrawText::SetFontSize(FT_UInt width, FT_UInt height,int nAlign)
{
	if(m_pFTFace)
	{
		m_nHeight = height;
		m_nWidth = width;
		m_nAlign = nAlign;
		if(m_nWidth==0) m_nWidth=m_nHeight;
		m_error= FT_Set_Pixel_Sizes(m_pFTFace,m_nWidth,m_nHeight);
	}
	return m_error;
}

void CtmDrawText::SetFontColor(unsigned int color)
{
	m_nFontColor=color;
}

void CtmDrawText::SetBgcColor(unsigned int color)
{
	m_nBgcColor = color;
}

void		CtmDrawText::SetEmbolden(int embolden)
{
		m_nEmbolden=embolden;

}

static inline void compute_string_cbox(FT_BBox* abbox, PGlyph glyphs,FT_UInt num_glyphs)
{
	FT_BBox bbox;
	bbox.xMin=bbox.yMin=32000;
	bbox.xMax=bbox.yMax=-32000;
	for(int i=0; i<num_glyphs;++i)
	{
		FT_BBox glyph_bbox;
		FT_Glyph_Get_CBox(glyphs[i].image,FT_GLYPH_BBOX_PIXELS,&glyph_bbox); 

		if(glyph_bbox.xMin<bbox.xMin)
			bbox.xMin=glyph_bbox.xMin;
		if (glyph_bbox.yMin < bbox.yMin) 
			bbox.yMin = glyph_bbox.yMin; 

		if (glyph_bbox.xMax > bbox.xMax) 
			bbox.xMax = glyph_bbox.xMax; 

		if (glyph_bbox.yMax > bbox.yMax) 
			bbox.yMax = glyph_bbox.yMax; 	
	}
	if ( bbox.xMin > bbox.xMax ) 
	{ 
		bbox.xMin = 0; 
		bbox.yMin = 0; 
		bbox.xMax = 0; 
		bbox.yMax = 0; 
	}
	*abbox=bbox;	
}


#ifdef D_32BIT

static inline void ft2CopyBitmapToBuf(unsigned long** buffer, int x,
		int y, CSize size,
		FT_Bitmap bitmap, uint color)
{
	for(int i=y;i<y+bitmap.rows;i++)
	{
		for(int j=x; j<x+bitmap.width;j++)
		{
			unsigned char bR=buffer[i][j]>>16&0xFF;
			unsigned char bG=buffer[i][j]>>8&0xFF;
			unsigned char bB=buffer[i][j]&0xFF;
			
			unsigned char cR=color>>16&0xFF;
			unsigned char cG=color>>8&0xFF;
			unsigned char cB=color&0xFF;
			
			unsigned char gray=bitmap.buffer[(i-y) *bitmap.width + (j-x)];
			buffer[i][j]=RGB2PIXEL888((cR*gray+bR*(255-gray))>>8,
		               			(cG*gray+bG*(255-gray))>>8,
		               			(cB*gray+bB*(255-gray))>>8
				);	
		}	
	}	
}

void	CtmDrawText::DrawText(unsigned long** pBuffer,unsigned int* pwCode,CRect rect,int nLen, bool adjust,bool wrap)
{
	if(m_pFTFace==0) return;
	FT_GlyphSlot slot = m_pFTFace->glyph;
	FT_UInt glyph_index;
	FT_Bool use_kerning;
	FT_UInt previous=0;
	int pen_x=0, pen_y=0;
	
	FT_UInt num_glyphs=0;
	PGlyph glyph;
	
	use_kerning=FT_HAS_KERNING(m_pFTFace);
	if(nLen!=-1)
	{
		unsigned int* pTemp=pwCode;
		while(nLen>0)
		{
			
			glyph =m_glyphs;
			num_glyphs=mMin(nLen,MAX_GLYPHS);
			nLen-=num_glyphs;
			m_num_glyphs=0;
			PGlyph glyphs=glyph-m_num_glyphs;
			
			for(int i=0; i<num_glyphs; ++i)
			{
				if(pen_x>rect.width())
				{
					FT_BBox bbox;
					m_num_glyphs=glyph-glyphs-m_num_glyphs;
					compute_string_cbox(&bbox,glyph-m_num_glyphs,m_num_glyphs);
					int string_width=bbox.xMax-bbox.xMin;
					int string_height=bbox.yMax-bbox.yMin;
					FT_Vector start;
					start.x=0;
					start.y=0;
					glyphs=glyph-m_num_glyphs;
					for(int n=0;n<m_num_glyphs;++n)
					{
						FT_Glyph image;
						FT_Vector pen;
						FT_BBox cbbox;
						
						m_error = FT_Glyph_Copy(glyphs[n].image,&image);
						if(m_error) continue;
						FT_Matrix matrix;
						start.x=glyphs[n].pos.x<<6;
						start.y=glyphs[n].pos.y<<6;
						FT_Glyph_Transform(image, 0, &start);
		
						m_error = FT_Glyph_To_Bitmap( 
						&image, 
						FT_RENDER_MODE_NORMAL, 
						0, 
						1 );
		
						if ( !m_error ) 
						{ 
							FT_BitmapGlyph bit = (FT_BitmapGlyph)image; 
							ft2CopyBitmapToBuf(pBuffer,
							 bit->left,
							 string_height-bit->top,CSize(0,0),
							 bit->bitmap,m_nFontColor);
							 FT_Done_Glyph(image); 
						} 
					}
					
					pen_x = 0;
					pen_y +=string_height;
					previous = 0;
					if(pen_y>rect.height()) return;
				}
				glyph->index = FT_Get_Char_Index(m_pFTFace, pTemp[i]);
				if(use_kerning&&previous&&glyph->index)
				{
					FT_Vector delta;
					FT_Get_Kerning(m_pFTFace,previous,glyph->index,
							0,&delta);
							pen_x+=delta.x>>6;	
				}
				glyph->pos.x=pen_x;
				glyph->pos.y=pen_y;
				
				m_error = FT_Load_Glyph(m_pFTFace,glyph->index,FT_LOAD_DEFAULT);
				
				if(m_error) continue;
				FT_Outline_Embolden(&(m_pFTFace->glyph->outline),8);
				m_error=FT_Get_Glyph(m_pFTFace->glyph,&glyph->image);
				if(m_error) continue;

				FT_Glyph_Transform(glyph->image,0,&glyph->pos);
				pen_x+=slot->advance.x>>6;
				previous = glyph->index;
				glyph++;
			}
			
			//flush
			FT_BBox bbox;
			m_num_glyphs=glyph-glyphs-m_num_glyphs;
			compute_string_cbox(&bbox,glyph-m_num_glyphs,m_num_glyphs);
			int string_width=bbox.xMax-bbox.xMin;
			int string_height=bbox.yMax-bbox.yMin;
			FT_Vector start;
			start.x=0;
			start.y=0;
			glyphs=glyph-m_num_glyphs;
			for(int n=0;n<m_num_glyphs;++n)
			{
				FT_Glyph image;
				FT_Vector pen;
				FT_BBox cbbox;
						
				m_error = FT_Glyph_Copy(glyphs[n].image,&image);
				if(m_error) continue;
				FT_Matrix matrix;
				start.x=glyphs[n].pos.x<<6;
				start.y=glyphs[n].pos.y<<6;
				FT_Glyph_Transform(image, 0, &start);
		
				m_error = FT_Glyph_To_Bitmap( 
					&image, 
					FT_RENDER_MODE_NORMAL, 
					0, 
					1 );
		
				if ( !m_error ) 
				{ 
					FT_BitmapGlyph bit = (FT_BitmapGlyph)image; 
						ft2CopyBitmapToBuf(pBuffer,
						bit->left,
						string_height-bit->top,CSize(0,0),
						bit->bitmap,m_nFontColor);
					 FT_Done_Glyph(image); 
				} 
			}
			
			pTemp+=num_glyphs;
		}
	}
	else
	{
		unsigned int* pTemp=pwCode;
		printf("ele ........\n");
		while(*pTemp)
		{
			++pTemp;
		}
		
	}
			
}
		
void	CtmDrawText::DrawText(unsigned long** pBuffer,unsigned char* pcCode,CRect rect, int nLen, bool adjust,bool wrap)
{
			
}
#else 
static inline void ft2CopyBitmapToBuf(uint16** buffer, int x,
		int y, CSize size,
		FT_Bitmap bitmap,uint16 color)
{
	for(int i=y;i<y+bitmap.rows;i++)
	{
		for(int j=x; j<x+bitmap.width;j++)
		{
			unsigned char bR=buffer[i][j]&0xF800>>8;
			unsigned char bG=buffer[i][j]&0x7E0>>3;
			unsigned char bB=buffer[i][j]&0x1F<<3;
			
			unsigned char cR=color[&0xF800>>8;
			unsigned char cG=color&0x7E0>>3;
			unsigned char cB=color&0x1F<<3;
			
			unsigned char gray=bitmap.buffer[(i-y) *bitmap.width + (j-x)];
			buffer[i][j]=RGB2PIXEL565((cR*gray+bR*(255-gray))>>8,
		               			(cG*gray+bG*(255-gray))>>8,
		               			(cB*gray+bB*(255-gray))>>8
				);	
		}	
	}	
}
void	CtmDrawText::DrawText(unsigned short** pBuffer,unsigned short* pwCode,CRect rect,int nLen, bool adjust,bool wrap)
{
			
}
		
void	CtmDrawText::DrawText(unsigned short** pBuffer,unsigned char* pcCode,CRect rect, int nLen, bool adjust,bool wrap)
{
			
}
#endif

