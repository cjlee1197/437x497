#include "encode.h"
#include "arabictables.h"

CtmEncode::CtmEncode()
{
	m_fromcharset=NULL;
	m_tocharset=NULL;	
	//m_unicode=NULL;
	//m_inputbuf=NULL;
	memset(m_utf8,0,CONST_UTF8_CHARLEN);
}
CtmEncode::CtmEncode(const char* fromcharset,const  char* tocharset)
{
	assert(fromcharset);
	assert(tocharset);
	//m_unicode=NULL;
	//m_inputbuf=NULL;
	memset(m_utf8,0,CONST_UTF8_CHARLEN);
	m_fromcharset=new char[strlen(fromcharset)+1];
	m_tocharset=new  char[strlen(tocharset)+1];
	memcpy(m_fromcharset,fromcharset,strlen(fromcharset)+1);
	memcpy(m_tocharset,tocharset,strlen(tocharset)+1);
}

CtmEncode::~CtmEncode()
{
	if(m_fromcharset)
	{
		delete[] m_fromcharset;
		m_fromcharset=NULL;
	}
	if(m_tocharset)
	{
		delete[] m_tocharset;
		m_tocharset=NULL;
	}

}

void  CtmEncode::SetFromCharset(const  char* fromcharset)
{
	assert(fromcharset);
	if(m_fromcharset)
		delete m_fromcharset;
	m_fromcharset=new char[strlen(fromcharset)+1];
	memcpy(m_fromcharset,fromcharset,strlen(fromcharset)+1);
	
	
}
void	CtmEncode::SetToCharset(const  char* tocharset)
{
	assert(tocharset);
	if(m_tocharset)
		delete m_tocharset;
	m_tocharset=new char[strlen(tocharset)+1];
	memcpy(m_tocharset,tocharset,strlen(tocharset)+1);
}
void	CtmEncode::SetCharset(const  char* fromcharset, const  char* tocharset)
{
	SetFromCharset(fromcharset);
	SetToCharset(tocharset);
}

int CtmEncode::Utf8ConvertUnicode(unsigned char* utf8,unsigned long* unicode, int* pLen)
{
	int rc=0;
	int	nCount = 0;
	int	nUTF8Len = 0;
	while(utf8[0])
	{
		//printf("utf8=%x\n",utf8[0]);
		if((utf8[0]&0x80)==0x0)// 1byte
		{
			rc=enc_utf8_to_unicode_one( utf8, unicode, 1);
			
			nUTF8Len += 1;
			//printf("unicode=%x\n",*unicode);
		}
		else if((utf8[0]&0xE0)==0xC0)//2bytes
		{
			rc=enc_utf8_to_unicode_one( utf8, unicode, 2);
			nUTF8Len += 1;
		}
		else if((utf8[0]&0xF0)==0xE0)//3bytes
		{
			rc=enc_utf8_to_unicode_one( utf8, unicode, 3);
			nUTF8Len += 2;
		}
		else if((utf8[0]&0xF8)==0xF0)//4bytes
		{
			rc=enc_utf8_to_unicode_one( utf8, unicode, 4);
			nUTF8Len += 3;
		}
		else if((utf8[0]&0xFC)==0xF8)//5bytes
		{
			rc=enc_utf8_to_unicode_one( utf8, unicode,5);
			nUTF8Len += 4;
		}
		else if((utf8[0]&0xFE)==0xFC)//6bytes
		{
			rc=enc_utf8_to_unicode_one( utf8, unicode,6);
			nUTF8Len += 5;
		}
		utf8+=rc;
		//printf("rc=%d unicode=%x \n", rc, *unicode);
		unicode++;
		nCount++;
	} 
	nCount++;
	*unicode=0;
	if(pLen != NULL)
		memcpy(pLen, &nUTF8Len, sizeof(int));
	return nCount;
}
int CtmEncode::UnicodeConvertUtf8(unsigned long* unicode,unsigned char* utf8)
{
	int rc=0;
	while(*unicode)
	{
		rc=enc_unicode_to_utf8_one(*unicode, utf8,sizeof(*unicode)*2);
		utf8+=rc;
		unicode++;
	}
	utf8[0]=0;
	return 0;
}

int	CtmEncode::CodeChange(unsigned char* inbuf,unsigned long* unicode)
{
	assert(inbuf);
	assert(unicode);
	int outlen=CONST_UTF8_CHARLEN;
	int rc=code_convert(m_fromcharset,m_tocharset,(char*)inbuf,strlen((char*)inbuf),m_utf8,outlen);
	if(rc<0)	return -1;
	Utf8ConvertUnicode((unsigned char*)m_utf8,unicode);
	return 0;
}
int	CtmEncode::CodeChange(unsigned long* unicode,unsigned char* inbuf)
{
	assert(inbuf);
	assert(unicode);
	int outlen=CONST_UTF8_CHARLEN;
	UnicodeConvertUtf8(unicode,(unsigned char*) m_utf8);
	int rc=code_convert(m_tocharset,m_fromcharset,m_utf8,outlen,(char*)inbuf,sizeof((char*)inbuf)/sizeof(char));
	if(rc<0)	return -1;
	return 0;
}
int CtmEncode::code_convert(  char *from_charset,  char *to_charset,
 			 char *inbuf,int inlen,  char *outbuf,int outlen)
{
 	iconv_t cd;
	 char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if ((int)cd<=0) 		return -1;
	//printf("cd=%d\n",cd);
	memset(outbuf,0,outlen);
	int nResult=iconv(cd,pin,(size_t*)&inlen,pout,(size_t*)&outlen);
	//printf("nLen=%d\n",outlen);
	if(nResult<0)	return -1;
	//printf("nResult=%d\n",nResult);
	iconv_close(cd);
	return nResult;
}
int CtmEncode::enc_utf8_to_unicode_one(unsigned char* pInput, unsigned long *Unic, int inbytes)
{
 	assert(pInput != NULL && Unic != NULL);

    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;

    *Unic = 0x0; // 把 *Unic 初始化为全零
    int utfbytes = inbytes;
    unsigned char *pOutput = (unsigned char *) Unic;

    switch ( utfbytes )
    {
        case 1:
            *pOutput     = *pInput;
            utfbytes   = 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
//            if ( (b2 & 0xE0) != 0x80 )
//            {
//            		printf("in ...\n");
//                return 0;
//            }
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
//            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
//                return 0;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
//            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
//                    || ((b4 & 0xC0) != 0x80) )
//                return 0;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
//            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
//                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
//                return 0;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
//            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
//                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
//                    || ((b6 & 0xC0) != 0x80) )
//                return 0;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}
int CtmEncode::enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize)
{
	assert(pOutput != NULL);
    	assert(outSize >= 6);

    if ( unic <= 0x0000007F )
   	{
        	// * U-00000000 - U-0000007F:  0xxxxxxx
       	 	*pOutput     = (unic & 0x7F);
       		 return 1;
   	}
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )
   	{
       	 // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        	*(pOutput+1) = (unic & 0x3F) | 0x80;
        	*pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        	return 2;
  	}
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        	// * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        	*(pOutput+2) = (unic & 0x3F) | 0x80;
        	*(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
        	*pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        	return 3;
    }
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        	// * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        	*(pOutput+3) = (unic & 0x3F) | 0x80;
        	*(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
        	*(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
        	*pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        	return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        	// * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        	*(pOutput+4) = (unic & 0x3F) | 0x80;
        	*(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
        	*(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
        	*(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
        	*pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        	return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        	// * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        	*(pOutput+5) = (unic & 0x3F) | 0x80;
        	*(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
        	*(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
        	*(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
        	*(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
        	*pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        	  return 6;
	}

    	return 0;	
}

int  CtmEncode::CovertToUnicodeTable(unsigned long* unicode,int nCount)
{
	unsigned long* TempUnicode=new unsigned long[nCount];
	if(nCount==1)
	{
		if(unicode[0]==0x20)		TempUnicode[0]=unicode[0];
		else
		{
			int nSelfKind=CheckInTransform(unicode[0]);
			if(nSelfKind)
			{
				TempUnicode[0]=arabic_transform[unicode[0]-0x621][3];//alone
			}	
			else
			{
				TempUnicode[0]=unicode[0];	
			}
		}	
	}
	else if(nCount==2)
	{
		if(unicode[0]==0x20)	TempUnicode[0]=unicode[0];
		else
		{
			if(unicode[1]==0x20)	
			{
				TempUnicode[1]=unicode[1];
				int nSelfKind=CheckInTransform(unicode[0]);
				if(nSelfKind)
				{
					TempUnicode[0]=arabic_transform[unicode[0]-0x621][3];//alone	
				}
				else
				{
					TempUnicode[0]=unicode[0];	
				}
			}
			else
			{
				if((unicode[0]==0x644)&&
				((unicode[1]==0x622)||(unicode[1]==0x623)
				||(unicode[1]==0x625)||(unicode[1]==0x627)))
				{
					TempUnicode[0]=arabic_specs[unicode[1]-0x622][0];	
				}
				else
				{
					int frontKind=CheckInFrontSet(unicode[0]);
					int nextKind= CheckInNextSet(unicode[1]);
					int nSelfKind0=CheckInTransform(unicode[0]);
					int nSelfKind1=CheckInTransform(unicode[1]);
					if(nextKind)
					{
						//first
						if(nSelfKind0)
						{
							TempUnicode[0]=arabic_transform[unicode[0]-0x621][1];
						}
						else
						{
							TempUnicode[0]=unicode[0];
						}
					}
					else
					{
						
						if(nSelfKind0)
						{
							TempUnicode[0]=arabic_transform[unicode[0]-0x621][3];
						}
						else
						{
							TempUnicode[0]=unicode[0];
						}
					}
					if(frontKind)
					{
						//last
						if(nSelfKind1)
						{
							TempUnicode[1]=arabic_transform[unicode[1]-0x621][0];
						}
						else
						{
							TempUnicode[1]=unicode[1];	
						}
					}
					else
					{
						if(nSelfKind1)
						{
							TempUnicode[1]=arabic_transform[unicode[1]-0x621][3];
						}
						else
						{
							TempUnicode[1]=unicode[1];
						}	
					}
				}
				
			}
				
		}	
	}
	else if(nCount>=3)
	{
		int nIndex=0;
		int nStartIndex=1;
		if(unicode[0]==0x20)	TempUnicode[nIndex++]=unicode[0];
		else
		{
			int nextKind= CheckInNextSet(unicode[1]);
			int nSelfKind=CheckInTransform(unicode[0]);
			//printf("nextKind=%d\n",nextKind);
			if(nextKind)
			{
				if((unicode[0]==0x644)&&
					((unicode[1]==0x622)||(unicode[1]==0x623)||
					(unicode[1]==0x625)||(unicode[1]==0x627)))
				{
					
					TempUnicode[nIndex++]=arabic_specs[unicode[1]-0x622][0];
					nStartIndex++;	
				}
				else
				{
					if(nSelfKind)
					{
						TempUnicode[nIndex++]=arabic_transform[unicode[0]-0x621][1];	
					}
					else
					{
						TempUnicode[nIndex++]=unicode[0];
					}		
				}
			}
			else
			{
				
				if(nSelfKind)
				{
					TempUnicode[nIndex++]=arabic_transform[unicode[0]-0x621][3];
				}
				else
				{
					TempUnicode[nIndex++]=unicode[0];
				}		
			}
		}
		for(int i=nStartIndex; i<nCount-1;i++)
		{
			int nSelfKind=CheckInTransform(unicode[i]);
			if(!nSelfKind)	TempUnicode[nIndex++]=unicode[i];
			else
			{
				int frontKind=CheckInFrontSet(unicode[i-1]);
				int nextKind=CheckInNextSet(unicode[i+1]);
				if((unicode[i]==0x644)&&
					((unicode[i+1]==0x622)||(unicode[i+1]==0x623)||
					(unicode[i+1]==0x625)||(unicode[i+1]==0x627)))
				{
					if(frontKind)
					{
						TempUnicode[nIndex++]=arabic_specs[unicode[i+1]-0x622][1];
					}
					else
					{
						TempUnicode[nIndex++]=arabic_specs[unicode[i+1]-0x622][0];
					}
					i++;
				}
				else if(frontKind&&nextKind)
				{
					TempUnicode[nIndex++]=arabic_transform[unicode[i]-0x621][2];
				}
				else if(frontKind)
				{
					TempUnicode[nIndex++]=arabic_transform[unicode[i]-0x621][0];	
				}
				else if(nextKind)
				{
					TempUnicode[nIndex++]=arabic_transform[unicode[i]-0x621][1];
				}
				else
				{
					TempUnicode[nIndex++]=arabic_transform[unicode[i]-0x621][3];
				}
			}	
		}
		int nSelfKind=CheckInTransform(unicode[ nCount-1]);
		if(!nSelfKind)	TempUnicode[nIndex++]=unicode[nCount-1];
		else
		{
			int frontKind=	CheckInFrontSet(unicode[nCount-2]);
			if(frontKind)
			{
				if((unicode[nCount-2]==0x644)&&
					((unicode[nCount-1]==0x622)||(unicode[nCount-1]==0x623)||
					(unicode[nCount-1]==0x625)||(unicode[nCount-1]==0x627))) ;
				else	
				{
					TempUnicode[nIndex++]=arabic_transform[unicode[nCount-1]-0x621][0];	
				}
			}
			else
			{
				TempUnicode[nIndex++]=arabic_transform[unicode[nCount-1]-0x621][3];	
			}
		}
		
	}
	memcpy(unicode,TempUnicode,nCount*sizeof(unsigned long));
	if(nCount<=1)	return 0;
	while(!unicode[nCount-1])
	{
		nCount--;	
	}
	int nStart=0;
	while(unicode[nStart]==0x20)	nStart++;
	word_converse(unicode+nStart);
		
}

void CtmEncode::unicode_converse(unsigned long* unic ,int start,int end)
{
	if(*unic==0x0)	return;
	if(start>=end)
  			return;
	unsigned long temp;
	while(start!=end&&!CheckIsArabicSet(unic[start])) 			start++;
	while(start!=end&&!CheckIsArabicSet(unic[end]))   			end--; 
	
 	temp = unic[start];
 	unic[start]=unic[end];
 	unic[end]=temp;
 	start++;
	end--;
 	unicode_converse(unic,start,end); 
}

void CtmEncode::word_converse(unsigned long* unic)
{

	if(*unic==0x0)	return;
	int 	nPos=0;
	int		nEndPos=0;
	int 	nStartPos=0;
	bool 	bSetFlag=0;
	while(CheckIsArabicSet(unic[nPos]))
	{
		bSetFlag=1;
		nPos++;
	}
	if(!bSetFlag)
	{
		nPos++;			
	}
	nEndPos=nPos-1;
	while(unic[nEndPos]==0x20)	nEndPos--;
	while(unic[nStartPos]==0x20)	nStartPos++;		
	unicode_converse(unic,nStartPos,nEndPos);
	word_converse(unic+nPos);		
}