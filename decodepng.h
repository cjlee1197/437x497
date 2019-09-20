
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
//#include "jpeglib.h"
#include "common.h"
#include "main.h"

#ifndef _GEN_H
#define _GEN_H
//typedef unsigned short WORD;
//typedef unsigned char BYTE;
//typedef unsigned int DWORD;

typedef int             MWCOORD;        /* device coordinates*/
typedef int             MWBOOL;         /* boolean value*/
typedef unsigned char   MWUCHAR;        /* unsigned char*/
typedef unsigned char   UCHAR;        /* unsigned char*/
typedef unsigned long   MWCOLORVAL;     /* device-independent color value*/
typedef unsigned short  IMAGEBITS;    /* bitmap image unit size*/
typedef unsigned long   MWTIMEOUT;      /* timeout value */
typedef unsigned long   MWTEXTFLAGS;    /* MWTF_ text flag*/
/*------------------------------------------------------------*/
typedef unsigned short MWPIXELVAL;

/*------------------------------------------------------------*/
/*
typedef struct tagBITMAPFILEHEADER 
{ 
	WORD	bfType; 				
	DWORD 	bfSize; 				
	WORD	bfReserved1; 		
	WORD	bfReserved2; 			
	DWORD 	bfOffBits; 				
									
}
BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{ 
	DWORD 	biSize; 				
	long	biWidth; 			
	long	biHeight; 		
	WORD 	biPlanes; 	
	WORD 	biBitCount;				
						
	DWORD 	biCompression; 			
					
	DWORD 	biSizeImage; 	
	long	biXPelsPerMeter; 		
	long	biYPelsPerMeter; 	
	DWORD 	biClrUsed;	
	DWORD 	biClrImportant;
}
BITMAPINFOHEADER;

typedef struct tagRGBQUAD 
{ 
	BYTE	rgbBlue;				
	BYTE	rgbGreen; 			
	BYTE	rgbRed; 		
	BYTE	rgbReserved;	
}	RGBQUAD;

typedef struct tagBITMAPINFO 
{ 
	BITMAPINFOHEADER 	bmiHeader; 		
	RGBQUAD 			bmiColors[1]; 	
}	BITMAPINFO; 
*/
/*------------------------------------------------------------*/

typedef struct {
	UCHAR r;
	UCHAR g;
	UCHAR b;
	UCHAR _padding;
} PALENTRY;

typedef struct {
	int             width;          /* image width in pixels*/
	int             height;         /* image height in pixels*/
	int             planes;         /* # image planes*/
	int             bpp;            /* bits per pixel (1, 4 or 8)*/
	int             pitch;          /* bytes per line*/
	int             bytesperpixel;  /* bytes per pixel*/
	int             compression;    /* compression algorithm*/
	int             palsize;        /* palette size*/
	long            transcolor;     /* transparent color or -1 if none*/
	PALENTRY *      palette;        /* palette*/
	UCHAR 	*	imagebits;      /* image bits (dword right aligned)*/
} imagehdr;

#define EPRINTF printf
#define DPRINTF printf

/*-------------------------------------------------------*/
#define MWIMAGE_UPSIDEDOWN 1
#define MWIMAGE_RGB     2      /* compression flag: RGB not BGR bytes*/
#define MWIMAGE_ALPHA_CHANNEL 4
#define MWPF_PALETTE 2
/*
 * Pixel formats
 * Note the two pseudo pixel formats are never returned by display drivers,
 * but rather used as a data structure type in GrArea.  The other
 * types are both returned by display drivers and used as pixel packing
 * specifiers.
 */
#define MWPF_RGB           0    /* pseudo, convert from packed 32 bit RGB*/
#define MWPF_PIXELVAL      1    /* pseudo, no convert from packed PIXELVAL*/
#define MWPF_PALETTE       2    /* pixel is packed 8 bits 1, 4 or 8 pal index*/
#define MWPF_TRUECOLOR0888 3    /* pixel is packed 32 bits 8/8/8 truecolor*/
#define MWPF_TRUECOLOR888  4    /* pixel is packed 24 bits 8/8/8 truecolor*/
#define MWPF_TRUECOLOR565  5    /* pixel is packed 16 bits 5/6/5 truecolor*/
#define MWPF_TRUECOLOR555  6    /* pixel is packed 16 bits 5/5/5 truecolor*/
#define MWPF_TRUECOLOR332  7    /* pixel is packed 8 bits 3/3/2 truecolor*/
#define MWPF_TRUECOLOR8888 8    /* pixel is packed 32 bits 8/8/8/8 truecolor with alpha */

/*
 * Conversion from MWCOLORVAL to MWPIXELVAL
 */
/* create 32 bit 8/8/8/8 format pixel from ARGB colorval (0xAABBGGRR)*/
/* In this format, alpha is preserved. */
#define COLOR2PIXEL8888(c)      \
        ((((c) & 0xff) << 16) | ((c) & 0xff00ff00ul) | (((c) & 0xff0000) >> 16))

/* create 24 bit 8/8/8 format pixel from RGB colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL888(c)       \
        ((((c) & 0xff) << 16) | ((c) & 0xff00) | (((c) & 0xff0000) >> 16))

/* create 16 bit 5/6/5 format pixel from RGB colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL565(c)       \
        ((((c) & 0xf8) << 8) | (((c) & 0xfc00) >> 5) | (((c) & 0xf80000) >> 19))

/* create 16 bit 5/5/5 format pixel from RGB colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL555(c)       \
        ((((c) & 0xf8) << 7) | (((c) & 0xf800) >> 6) | (((c) & 0xf80000) >> 19))

/* create 8 bit 3/3/2 format pixel from RGB colorval (0x00BBGGRR)*/
/* In this format, alpha is ignored. */
#define COLOR2PIXEL332(c)       \
        (((c) & 0xe0) | (((c) & 0xe000) >> 11) | (((c) & 0xc00000) >> 22))

/*-------------------------------------------------------*/
/* Color defines*/
#define MWARGB(a,r,g,b) ((MWCOLORVAL)(((unsigned char)(r)|\
                                (((unsigned)(unsigned char)(g))<<8))|\
                                (((unsigned long)(unsigned char)(b))<<16)|\
                                (((unsigned long)(unsigned char)(a))<<24)))
#define MWRGB(r,g,b)    MWARGB(255,(r),(g),(b))         /* rgb full alpha*/
#define MW0RGB(r,g,b)   MWARGB(0,(r),(g),(b))           /* rgb no alpha*/

typedef struct {
        MWUCHAR r;
        MWUCHAR g;
        MWUCHAR b;
        MWUCHAR _padding;
} MWPALENTRY;

/*-------------------------------------------------------*/
/* Buffered input functions to replace stdio functions*/
typedef struct {  		/* structure for reading images from buffer   */
	unsigned char *start;   /* The pointer to the beginning of the buffer */
	unsigned long offset;   /* The current offset within the buffer       */
	unsigned long size;     /* The total size of the buffer               */
} buffer_t;


imagehdr *loadpngfile(char *path,  int flags);
int decode_png(buffer_t * src, imagehdr *pimage);
int decode_jpeg(buffer_t * src, imagehdr *pimage, int fast_grayscale);
imagehdr *decode_img(buffer_t * src, char *path, int flags);

int decode_pixel(imagehdr *pimage);
int set_colormap(int start,int max,PALENTRY *palette);

/*========================================================*/
// From decode.c
/*========================================================*/
void print_image(imagehdr *image);
int print_image2(imagehdr *pimage);
int pr_imagebits(imagehdr *pimage);


int GdImageBufferRead(buffer_t *buffer, void *dest, unsigned long size);
void GdImageBufferSeekTo(buffer_t *buffer, unsigned long offset);
void GdComputeImagePitch(int bpp, int width, int *pitch, int *bytesperpixel);

int save_to_bmp(imagehdr *image,int len);





extern PALENTRY mwstdpal8[256];

#endif	/* ifndef _GEN_H */
