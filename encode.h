#ifndef D_ENCODE_H
#define D_ENCODE_H
#include <iconv.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define 	CONST_UTF8_CHARLEN	512*6
class CtmEncode{
	public:
	CtmEncode();
	CtmEncode(const  char* fromcharset,const  char* tocharset);
	~CtmEncode();	

	void  SetFromCharset(const  char* fromcharset);
	void	SetToCharset(const  char* tocharset);
	void	SetCharset(const  char* fromcharset, const  char* tocharset);
	const  char* GetFromCharset(){return m_fromcharset;};
	const  char* GetToCharset(){return	m_tocharset; };
	int Utf8ConvertUnicode(unsigned char* utf8,unsigned long* unicode, int* pLen = NULL);
	int UnicodeConvertUtf8(unsigned long* unicode,unsigned char* utf8);
	int	CodeChange(unsigned char* inbuf,unsigned long* unicode);
	int	CodeChange(unsigned long* unicode,unsigned char* inbuf);
	int	CovertToUnicodeTable(unsigned long* unicode,int nCount);
	private:
		 int code_convert( char *from_charset,  char *to_charset,
		 char *inbuf,int inlen, char *outbuf,int outlen);
		 int enc_utf8_to_unicode_one( unsigned char* pInput, unsigned long *Unic, int inbytes);
		 int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize);
		 void unicode_converse(unsigned long* unic, int start, int end);
		 void word_converse(unsigned long* unic);
	private:
		 char* 		m_fromcharset;
		 char* 		m_tocharset;
		 char 		 m_utf8[CONST_UTF8_CHARLEN];
		//unsigned long*		m_unicode;
		//unsigned char*		m_inputbuf;
};
#endif