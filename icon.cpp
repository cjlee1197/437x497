/*==========================================================================+
|  Class    : Class Icon	                                                |
|  Task     : Class Icon file                              					|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/12/18                                                    |
|  Revision : 2006/12/18                                                    |
+==========================================================================*/
#include <string.h>
#include "icon.h"
#include "files.h"
#include "gui.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagRGBQUAD 
{ /* rgbq */
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFOHEADER
{
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD 	biPlanes;
	WORD 	biBitCount;
	DWORD 	biCompression;
	DWORD 	biSizeImage;
	LONG 	biXPelsPerMeter;
	LONG 	biYPelsPerMeter;
	DWORD 	biClrUsed;
	DWORD 	biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
	BITMAPINFOHEADER   	icheader;      // dib header
	RGBQUAD         	iccolors[1];   // color table
	BYTE            	icxor[1];      // dib bits for xor mask
	BYTE            	icand[1];      // dib bits for and mask
} iconimage, *lpiconimage;

typedef struct
{
	BYTE        bwidth;          // width, in pixels, of the image
	BYTE        bheight;         // height, in pixels, of the image
	BYTE        bcolorcount;     // number of colors in image (0 if >=8bpp)
	BYTE        breserved;       // reserved ( must be 0)
	WORD        wPointX;         // color planes
	WORD        wPointY;       	// bits per pixel
	DWORD       dwbytesinres;    // how many bytes in this resource?
	DWORD       dwimageoffset;   // where in the file is this image?
} icondirentry, *lpicondirentry;

typedef struct
{
	WORD           wIdReserved;   // reserved (must be 0)
	WORD           wIdType;       // resource type (1 for icons)
	WORD           wIdCount;      // how many images?
	icondirentry   Identries[1]; // an entry for each image (idcount of 'em)
} PACKED ICONDIR, *LPICONDIR;
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
/*==========================================================================+
|           Class implementation - CICON                                    |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CIcon, CObject)
CIcon::CIcon()
{
	m_pszName 	= NULL;
	m_pwQuad	= NULL;
	
	m_nType		= 0;
	m_nCount	= 0;
	m_nQuadCount= 0;
}

CIcon::~CIcon()
{
	Destroy();
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	CIcon::Create(void *pData)
{
	CtmFile	fileIcon;
	int 	nLineByte	= 0;
	
	ICONDIR	iconData;
	icondirentry	*piconinfo;
	BITMAPINFOHEADER	bmpheader;
	
	if(pData == NULL)	return -1;
	
	ICON	*pIcon	= (ICON *)pData;
	
	if(pIcon->pszName == NULL)	return -1;
	
	if(m_pszName != NULL) Destroy();
	
	m_pszName		= (char *)malloc(strlen(pIcon->pszName) + 1);
	strcpy(m_pszName, pIcon->pszName);
	
	fileIcon.Open(m_pszName, O_RDONLY);
	
	fileIcon.Read((char *)&iconData, sizeof(ICONDIR));
	
	m_nType			= iconData.wIdType;
	m_nCount		= iconData.wIdCount;
/*	
	printf("icon.idreserved =%d\n", iconData.wIdReserved);
	printf("icon.wIdType =%d\n", iconData.wIdType);
	printf("icon.wIdCount =%d\n", iconData.wIdCount);	
*/	
	piconinfo	= iconData.Identries;
	
	m_nWidth		= piconinfo->bwidth;
	m_nHeight		= piconinfo->bheight;
	m_nPointX		= piconinfo->wPointX;
	m_nPointY		= piconinfo->wPointY;
	m_nLength		= piconinfo->dwbytesinres;
	m_nOffset		= piconinfo->dwimageoffset;
	
//	fileIcon.Read((char *)&iconinfo, sizeof(icondirentry));
/*	
	printf("piconinfo->bwidth=%d\n", piconinfo->bwidth);
	printf("piconinfo->bheight=%d\n", piconinfo->bheight);
	printf("piconinfo->bcolorcount=%d\n", piconinfo->bcolorcount);
	printf("piconinfo->breserved=%d\n", piconinfo->breserved);
	printf("piconinfo->wPointX=%d\n", piconinfo->wPointX);
	printf("piconinfo->wPointY=%d\n", piconinfo->wPointY);
	printf("piconinfo->dwbytesinres=%ld\n", piconinfo->dwbytesinres);
	printf("piconinfo->dwimageoffset=%ld\n", piconinfo->dwimageoffset);
*/	
	
	fileIcon.Seek(m_nOffset, SEEK_SET);
	fileIcon.Read((char *)&bmpheader, sizeof(BITMAPINFOHEADER));
	
	m_nBitCount		= bmpheader.biBitCount;
	m_nQuadCount	= 0;
	switch(m_nBitCount)
	{
	case 1:
		m_nQuadCount	= 2;
		break;
	case 2:
		m_nQuadCount	= 4;
		break;
	case 4:
		m_nQuadCount	= 16;
		break;
	}
	
	if(m_nQuadCount != 0)	m_pwQuad	= (unsigned long *)malloc(m_nQuadCount * sizeof(unsigned long));	
	
/*		
	printf("bmpheader.biSize=%ld\n", 		bmpheader.biSize);
	printf("bmpheader.biWidth=%ld\n", 		bmpheader.biWidth);
	printf("bmpheader.biHeight=%ld\n", 		bmpheader.biHeight);
	printf("bmpheader.biPlanes=%d\n", 		bmpheader.biPlanes);
	printf("bmpheader.biBitCount=%d\n", 	bmpheader.biBitCount);
	printf("bmpheader.biCompression=%ld\n", bmpheader.biCompression);
	printf("bmpheader.biSizeImage=%ld\n", 	bmpheader.biSizeImage);
	printf("bmpheader.biXPelsPerMeter=%ld\n", bmpheader.biXPelsPerMeter);
	printf("bmpheader.biYPelsPerMeter=%ld\n", bmpheader.biYPelsPerMeter);
	printf("bmpheader.biClrUsed=%ld\n", 	bmpheader.biClrUsed);
	printf("bmpheader.biClrImportant=%ld\n",bmpheader.biClrImportant);
*/	
	fileIcon.Read(m_pwQuad, m_nQuadCount * sizeof(unsigned long));
	
	nLineByte	= m_nWidth * m_nBitCount / 8;
	
	while(nLineByte % 4)	nLineByte ++;
//	printf("m_nLineByte=%d\n", nLineByte);
	m_nLineByte	= nLineByte;
	
	m_pIconData	= (char *)malloc(nLineByte * m_nHeight);
	fileIcon.Read(m_pIconData, nLineByte * m_nHeight);
	
	nLineByte	= m_nWidth / 8;
	
	while(nLineByte % 4)	nLineByte ++;
	m_pXorData	= (char *)malloc(nLineByte * m_nHeight);
	
	fileIcon.Read(m_pXorData, nLineByte * m_nHeight);
	m_nXorLineByte	= nLineByte;
	
	fileIcon.Close();
	
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	CIcon::Destroy()
{
	if(m_pszName != NULL)
	{
		free(m_pszName);
		m_pszName	= NULL;
	}
	if(m_pIconData != NULL)
	{	
		free(m_pIconData);
		m_pIconData	= NULL;
	}
	if(m_pXorData != NULL)
	{
		free(m_pXorData);
		m_pXorData	= NULL;
	}
	
	if(m_pwQuad != NULL)
	{
		free(m_pwQuad);
		m_pwQuad	= NULL;
	}
	
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int     CIcon::Draw(int x, int y, int nWidth, int nHeight)
{
    switch(m_nBitCount)
	{
	case 1:
		return Draw1(x, y);
		break;
	case 4:	    
		return Draw4(x, y, nWidth, nHeight);
		break;
	}
	return -1;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int	CIcon::Draw(int x, int y)
{
	switch(m_nBitCount)
	{
	case 1:
		return Draw1(x, y);
		break;
	case 4:	    
		return Draw4(x, y);
		break;
	}
	return -1;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CIcon::Draw1(int x, int y)
{
	unsigned long 	*pwColor	= m_pwQuad;
	unsigned char	*pIconData, *pXorData;
	
	int nHeight, nWidth;
	
	nWidth  = (m_nWidth <= g_Length_X) ? m_nWidth : g_Length_X;
	nHeight = (m_nHeight <= g_Length_Y) ? m_nHeight : g_Length_Y;
	
	for(int i = nHeight - 1; i >= 0; i --)
	{
		pIconData	= (unsigned char *)m_pIconData + i * m_nLineByte;
		pXorData	= (unsigned char *)m_pXorData + i * m_nXorLineByte;
//		printf("y = %02x ", m_nWidth - i - 1);
		for(int j = 0; j < nWidth; j ++)
		{			
			if((*pIconData & (0x80 >> (j % 8))) == 0)
			{
				if((*pXorData & (0x80 >> (j % 8))) == 0)
				{
				    #ifdef	D_32BIT
				    g_Rows[y + m_nWidth - i - 1][x + j] = ((pwColor[0] & 0x00FF0000) | 
	                    								   (pwColor[0] & 0x0000FF00) | 
	                    								   (pwColor[0] & 0x000000FF));
	                #else
				    g_Rows[y + m_nWidth - i - 1][x + j] = ((((pwColor[0] & 0x00FF0000) >> 16) & 0xf8) << 8) | 
	                    ((((pwColor[0] & 0x0000FF00) >> 8) & 0xfc) << 3) | 
	                    (((pwColor[0] & 0x000000FF) & 0xf8) >> 3);
	                #endif
//					g_gdiFB.SetColor(pwColor[0]);
//					g_gdiFB.Pixel(x +  j, y + m_nWidth - i - 1);
//					printf("%d", 0);
				}
				else printf(" ");
			}
			else 
			{
                #ifdef	D_32BIT
                g_Rows[y + m_nWidth - i - 1][x + j] = ((pwColor[1] & 0x00FF0000) | 
	                    							   (pwColor[1] & 0x0000FF00) | 
	                    							   (pwColor[1] & 0x000000FF));
                #else
                g_Rows[y + m_nWidth - i - 1][x + j] = ((((pwColor[1] & 0x00FF0000) >> 16) & 0xf8) << 8) | 
                    ((((pwColor[1] & 0x0000FF00) >> 8) & 0xfc) << 3) | 
                    (((pwColor[1] & 0x000000FF) & 0xf8) >> 3);
                #endif
			}
			
			if((j % 8) == 7)
			{
				pXorData ++;
				pIconData ++;
			}			
		}
//		printf("\n");
	}
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CIcon::Draw4(int x, int y, int nWidth, int nHeight)
{
	unsigned long 	*pwColor	= m_pwQuad;
	unsigned char	*pIconData, *pXorData;
	char			cData;
	
	for(int i = 0; i < nHeight; i ++)
	{
	    pIconData	= (unsigned char *)m_pIconData + (m_nHeight - y - nHeight + i) * m_nLineByte;
		pXorData	= (unsigned char *)m_pXorData + (m_nHeight - y - nHeight + i) * m_nXorLineByte;
		
		pIconData  += x >> 1;
		pXorData   += x >> 3;
		
		if(x & 0x01)
		    cData		= pIconData[0] & 0x0F;
		else
		    cData		= (pIconData[0] & 0xF0) >> 4;
		
		for(int j = 0; j < nWidth; j ++)
		{			
			if(cData == 0)
			{
				if((*pXorData & (0x80 >> (j % 8))) == 0)
				{
                    #ifdef	D_32BIT
                    g_Rows[nHeight - i - 1][j] = ((pwColor[cData] & 0x00FF0000) | 
                        						  (pwColor[cData] & 0x0000FF00) | 
                        						  (pwColor[cData] & 0x000000FF));
                    #else
                    g_Rows[nHeight - i - 1][j] = ((((pwColor[cData] & 0x00FF0000) >> 16) & 0xf8) << 8) | 
                        ((((pwColor[cData] & 0x0000FF00) >> 8) & 0xfc) << 3) | 
                        (((pwColor[cData] & 0x000000FF) & 0xf8) >> 3);
                    #endif
				}
			}
			else
			{
				#ifdef	D_32BIT
				g_Rows[nHeight - i - 1][j] = ((pwColor[cData] & 0x00FF0000) | 
                        					  (pwColor[cData] & 0x0000FF00) | 
                        					  (pwColor[cData] & 0x000000FF));
               	
				#else
                g_Rows[nHeight - i - 1][j] = ((((pwColor[cData] & 0x00FF0000) >> 16) & 0xf8) << 8) | 
                    ((((pwColor[cData] & 0x0000FF00) >> 8) & 0xfc) << 3) | 
                    (((pwColor[cData] & 0x000000FF) & 0xf8) >> 3);
               	#endif
			}			
			
			if(((j+x) & 0x01) == 0)
			{
				cData		= pIconData[0] & 0x0F;
				
			}
			else
			{				
				pIconData 	++;
				cData		= (pIconData[0] & 0xF0) >> 4;
				if(((j + x) % 8) == 7)	pXorData ++;
			}			
		}	
	}
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CIcon::Draw4(int x, int y)
{
	unsigned long 	*pwColor	= m_pwQuad;
	unsigned char	*pIconData, *pXorData;
	char			cData;
	
	int nHeight, nWidth;
	
	nWidth  = (m_nWidth <= g_Length_X) ? m_nWidth : g_Length_X;
	nHeight = (m_nHeight <= g_Length_Y) ? 0 : m_nHeight - g_Length_Y;
	
	for(int i = nHeight; i < m_nHeight; i ++)
	{
		pIconData	= (unsigned char *)m_pIconData + i * m_nLineByte;
		pXorData	= (unsigned char *)m_pXorData + i * m_nXorLineByte;
		
		cData		= (pIconData[0] & 0xF0) >> 4;
		
		for(int j = 0; j < nWidth; j ++)
		{
			if(cData == 0)
			{
				if((*pXorData & (0x80 >> (j % 8))) == 0)
				{
                    #ifdef	D_32BIT
                    g_Rows[y + m_nHeight - i - 1][x + j] = ((pwColor[cData] & 0x00FF0000) | 
                        						  			(pwColor[cData] & 0x0000FF00) | 
                        						  			(pwColor[cData] & 0x000000FF));
                    #else
                    g_Rows[y + m_nHeight - i - 1][x + j] = ((((pwColor[cData] & 0x00FF0000) >> 16) & 0xf8) << 8) | 
                        ((((pwColor[cData] & 0x0000FF00) >> 8) & 0xfc) << 3) | 
                        (((pwColor[cData] & 0x000000FF) & 0xf8) >> 3);
                    #endif
				}
			}
			else
			{
				#ifdef	D_32BIT
				g_Rows[y + m_nHeight - i - 1][x + j] = ((pwColor[cData] & 0x00FF0000) | 
                        						  	    (pwColor[cData] & 0x0000FF00) | 
                        						  		(pwColor[cData] & 0x000000FF));
                #else
				g_Rows[y + m_nHeight - i - 1][x + j] = ((((pwColor[cData] & 0x00FF0000) >> 16) & 0xf8) << 8) | 
                    ((((pwColor[cData] & 0x0000FF00) >> 8) & 0xfc) << 3) | 
                    (((pwColor[cData] & 0x000000FF) & 0xf8) >> 3);
                #endif
			}			
			
			if((j & 0x01) == 0)
			{
				cData		= pIconData[0] & 0x0F;
				
			}
			else
			{				
				pIconData 	++;
				cData		= (pIconData[0] & 0xF0) >> 4;
				if((j % 8) == 7)	pXorData ++;
			}			
		}	
	}
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
POINT	CIcon::GetPoint()
{
	POINT	point;
	
	point.nX	= m_nPointX;
	point.nY	= m_nPointY;
	
	return point;
}

