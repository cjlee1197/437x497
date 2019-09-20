#include "glyph.h"

Glyph::Glyph()
{
	m_pBitmapBuffer=NULL;
}

Glyph::~Glyph()
{
	if(m_pBitmapBuffer!=NULL)
	{
		delete[] m_pBitmapBuffer; 
		m_pBitmapBuffer=NULL;
	}	
}

Glyph::Glyph(Glyph& item): m_pBitmapBuffer(NULL) {
	Copy(item);
}


Glyph& Glyph::operator=(Glyph& item) {
	if (&item == this) return *this;
	Copy(item);
	return *this;
}




void	Glyph::NewBitmapBuffer(int nSize)
{
	if(m_pBitmapBuffer!=NULL)	delete[] m_pBitmapBuffer;
	m_pBitmapBuffer=new unsigned char[nSize];
	
}

void Glyph::Copy( Glyph& item) {
	m_nBitmapHeight=item.m_nBitmapHeight;
	m_nBitmapWidth=item.m_nBitmapWidth;
	memcpy(&m_nMatrics,&item.m_nMatrics,sizeof(TMMATRICS));
	m_nBitmapBufferSize = item.m_nBitmapBufferSize;

	if (m_pBitmapBuffer != NULL) delete m_pBitmapBuffer;
	m_pBitmapBuffer = item.m_pBitmapBuffer;
	if (item.m_pBitmapBuffer == NULL) return;
	m_pBitmapBuffer = new unsigned char[m_nBitmapBufferSize];
	memcpy(m_pBitmapBuffer, item.m_pBitmapBuffer, m_nBitmapBufferSize);
}
