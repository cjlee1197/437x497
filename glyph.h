#ifndef D_GLYPH_H
#define D_GLYPH_H
#include <string.h>
typedef struct tagtmMatrics
{
	int height;
	int width;
	int horibearingX;
	int horibearingY;
	int horiadvance;
	int ascender;
	
}TMMATRICS;

class Glyph
{
	public:
		Glyph();
		~Glyph();
		Glyph& operator=(Glyph&);
		Glyph(Glyph&);
	public:
		void 	SetBitmapHeight(int nHeight)	{m_nBitmapHeight=nHeight;};
		void	SetBitmapWidth(int nWidht)		{m_nBitmapWidth=nWidht;};
		void	SetMatricsHeight(int nHeight)	{m_nMatrics.height=nHeight;};
		void	SetMatricsWidth(int nWidth)		{m_nMatrics.width=nWidth;};
		void 	SetMatricsHoriBearingX(int nHoriBearingX)	{m_nMatrics.horibearingX=nHoriBearingX;};
		void	SetMatricsHoriBearingY(int nHoriBearingY)	{m_nMatrics.horibearingY=nHoriBearingY;};
		void	SetMatricsAdvance(int nHoriAdvance)			{m_nMatrics.horiadvance=nHoriAdvance;};
		void	SetMatricsAsender(int nAscender)				{m_nMatrics.ascender=nAscender;};
		void	SetMatrics(TMMATRICS& matrics)				{memcpy(&m_nMatrics,&matrics,sizeof(TMMATRICS));};
		void	SetBimapBufferSize(int nBuffersize)			{m_nBitmapBufferSize=nBuffersize;};
		
		int		GetBitmapHeight()		{return m_nBitmapHeight;};
		int		GetBitmapWidth()		{return m_nBitmapWidth; };
		int 	GetMatricsHeight()		{return m_nMatrics.height;};
		int		GetMatricsWidth()		{return m_nMatrics.width;};
		int		GetMatricsHoriBearingX()	{return m_nMatrics.horibearingX;};
		int 	GetMatricsHoriBearingY()	{return m_nMatrics.horibearingY;};
		int 	GetMatricsAdvance()			{return m_nMatrics.horiadvance;};
		int		GetMatricsAsender()			{return m_nMatrics.ascender;};
		TMMATRICS GetMatrics()				{return m_nMatrics;};
		
		void	NewBitmapBuffer(int nSize);
		unsigned char* GetBitmapBuffer()	{return m_pBitmapBuffer;};
		void	CopyToBuffer(unsigned char* Src, int nSize)
		{
			if(m_pBitmapBuffer!=NULL&&nSize==m_nBitmapBufferSize)
			{
				memcpy(m_pBitmapBuffer,Src,nSize);	
			}	
		};	
			
	private:
		int 	m_nBitmapHeight;
		int 	m_nBitmapWidth;
		TMMATRICS	m_nMatrics;
		int		m_nBitmapBufferSize;
		unsigned char* m_pBitmapBuffer;
	private:
		void Copy(Glyph&);
		
};
#endif /*D_GLYPH_H*/