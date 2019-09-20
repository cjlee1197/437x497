#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "decodepng.h"
void
image_buffer_init(buffer_t *buffer, void *startdata, int size)
{
	buffer->start = (unsigned char *)startdata;
	buffer->size = size;
	buffer->offset = 0;
}
/*
int main(int argc,char **argv)
{
	//loadfile("./test.jpg",0);
	if(argc <=1){
		printf(" Error Usage\nUsage:%s <png file>\n",argv[0]);
		return 0;
	}
	loadpngfile(argv[1],0);
	return 0;
}
*/
/*-------------------------------------------------------------*/
imagehdr *
decode_img(buffer_t * src, char *path, int flags)
{
	int      loadOK = 0;
	imagehdr *pimage;

	/* allocate image struct*/
	pimage = (imagehdr *)malloc(sizeof(imagehdr));
	if(!pimage) {
		return 0;
	}
	pimage->imagebits = NULL;
	pimage->palette = NULL;
	pimage->transcolor = -1L;
	
	//loadOK = decode_jpeg(src, pimage, flags);
	loadOK = decode_png(src, pimage);

	if (loadOK == 0) {
		EPRINTF("GdLoadImageFromFile: unknown image type\n");
		goto err;               /* image loading error*/
	}
	if (loadOK != 1)
		goto err;               /* image loading error*/

	/*------------------------*/
	//print_image2(pimage);
	/*------------------------*/

	/*------------------------*/
	//pr_imagebits(pimage);
	/*------------------------*/

	/*------------------------*/
	//decode_pixel(pimage);
	/*------------------------*/
	return (pimage);
	err:
	free(pimage);
	return NULL;                       /* image loading error*/
}
/*-------------------------------------------------------------*/

/**
 * Load an image from a file.
 *
 * @param path The file containing the image data.
 * @param flags If nonzero, JPEG images will be loaded as grayscale.  Yuck!
 */
imagehdr *
loadpngfile(char *path, int flags)
{
	int fd, id;
	struct stat s;
	void *buffer = NULL;
	buffer_t src;
	imagehdr *image_header=NULL;
	/*
	int i;
	for(i=0;i<strlen(path);i++)
		printf("  len=%d,  %c \n",strlen(path),path[i]);
	*/
	//printf("---file in loadpngfile =%s\n",path);
	
	fd = open(path, O_RDONLY);
	if (fd < 0 || fstat(fd, &s) < 0) {
		EPRINTF("LoadImageFromFile: can't open image: %s\n", path);
		return NULL;
	}

	buffer = (void *)malloc(s.st_size);
	if (!buffer) {
		EPRINTF("LoadImageFromFile: Couldn't malloc image %s\n", path);
		close(fd);
		return NULL;
	}

	if (read(fd, buffer, s.st_size) != s.st_size) {
		EPRINTF("LoadImageFromFile: Couldn't load image %s\n", path);
		close(fd);
		return NULL;
	}

	image_buffer_init(&src, buffer, s.st_size);
	image_header=decode_img(&src,path,flags);
	
	//printf("\n--load pngfile done\n");
	free(buffer);
	close(fd);
	if(image_header == NULL){
		return NULL;
	}	
	return image_header;
}

/*-------------------------------------------------------------*/
void
GdImageBufferSeekTo(buffer_t *buffer, unsigned long offset)
{
	if (offset < buffer->size)
		buffer->offset = offset;
}

int
GdImageBufferRead(buffer_t *buffer, void *dest, unsigned long size)
{
	unsigned long copysize;

	if (buffer->offset == buffer->size)
		return 0;       /* EOF*/

	if (buffer->offset + size > buffer->size)
		copysize = buffer->size - buffer->offset;
	else 
		copysize = size;

	memcpy(dest, buffer->start + buffer->offset, copysize);

	buffer->offset += copysize;
	return copysize;
}

/*-------------------------------------------------------------*/
#define PIX2BYTES(n)    (((n)+7)/8)
void
GdComputeImagePitch(int bpp, int width, int *pitch, int *bytesperpixel)
{
	int     linesize;
	int     bytespp = 1;

	if(bpp == 1)
		linesize = PIX2BYTES(width);
	else if(bpp <= 4)
		linesize = PIX2BYTES(width<<2);
	else if(bpp <= 8)
		linesize = width;
	else if(bpp <= 16) {
		linesize = width * 2;
		bytespp = 2;
	} else if(bpp <= 24) {
		linesize = width * 3;
		bytespp = 3;
	} else {
		linesize = width * 4;
		bytespp = 4;
	}

	/* rows are DWORD right aligned*/
	*pitch = (linesize + 3) & ~3;
	*bytesperpixel = bytespp;
}
/*-------------------------------------------------------------*/
int save_to_bmp(imagehdr *image,int len)
{
	int fd=-1;
	char *pBMP;
	int i=0,j=0;

	BYTE		*pRed, *pGreen, *pBlue;
	BITMAPFILEHEADER	*pHeader;
	BITMAPINFOHEADER	*pInfo;
	WORD				Pixel;
	WORD				*pwPixel;	

	len=sizeof(BITMAPFILEHEADER)+ sizeof( BITMAPINFOHEADER)+ image->height * image->width *4;
	pBMP	= (char *)malloc(len);

	pHeader	= (BITMAPFILEHEADER	*)pBMP;
	pInfo	= (BITMAPINFOHEADER	*)(pBMP + sizeof(BITMAPFILEHEADER));

	pHeader->bfType		= 0x4D42;
	pHeader->bfSize		= len;
	pHeader->bfReserved1	= 0; 		
	pHeader->bfReserved2	= 0; 	
	pHeader->bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 		

	pRed	= (BYTE *)(pBMP + pHeader->bfOffBits + 2);
	pGreen	= (BYTE *)(pBMP + pHeader->bfOffBits + 1);
	pBlue	= (BYTE *)(pBMP + pHeader->bfOffBits );

	pwPixel		= (WORD *)(pBMP+sizeof( BITMAPINFOHEADER)
				+ sizeof(BITMAPFILEHEADER)	);
		
	for(j = image->height- 1; j >= 0; j --) {
		for(i = 0; i < image->pitch; i ++) {
			Pixel	= *(pwPixel + j * image->width + i);//g_Rows[j][i];

			pRed 	+= 3;
			pGreen 	+= 3;
			pBlue 	+= 3;
		}
	}
	fd=open("./ret.bmp",O_RDWR);
	write(fd,pBMP,len);
	close(fd);
	return 0;
}
/*-------------------------------------------------------------*/
int print_image2(imagehdr *pimage)
{
	printf("<<------------------------------------------------\n");
	printf("width=\t%d\n",pimage->width);
	printf("height=\t%d\n",pimage->height);
	printf("planes=\t%d\n",pimage->planes);
	printf("bpp=\t%d\n",pimage->bpp);
	printf("pitch=\t%d\n",pimage->pitch);
	printf("bytesperpixel=\t%d\n",pimage->bytesperpixel);
	printf("compression=\t%d\n",pimage->compression);
	printf("palsize=\t%d\n",pimage->palsize);
	printf("transcolor=\t%ld\n",pimage->transcolor);
	printf("------------------------------------------------>>\n");
	return 0;
}
/*------------------------*/
int pr_imagebits(imagehdr *pimage)
{
        int i=0;
        //int j=0;
        UCHAR *pbit;
        pbit=pimage->imagebits;
        for(i=0;i<pimage->height*pimage->pitch;i++){
                if(pimage->imagebits[i]== 0x4a)
                        printf("  %c",'r');
                else if(pimage->imagebits[i]== 0xa5)
                        printf("  %c",'g');
                else if(pimage->imagebits[i]== 0x14)
                        printf("  %c",'b');
                else if(pimage->imagebits[i]== 0xfd)
                        printf("  %c",'w');
                else if(pimage->imagebits[i]== 0xec)
                        printf("  %c",'y');
                else
                        printf(" %02x",pimage->imagebits[i]);
                if((i+1)%pimage->pitch == 0)
                        printf("\n");
        }
        return 0;
}

void print_image(imagehdr *image)
{
	DPRINTF("Image:\n\n");
	DPRINTF("height: %d\n", image->height);
	DPRINTF("width: %d\n", image->width);
	DPRINTF("planes: %d\n", image->planes);
	DPRINTF("bpp: %d\n", image->bpp);
	DPRINTF("compression: %d\n", image->compression);
	DPRINTF("palsize: %d\n", image->palsize);

	/*
	for (i=0;i<image->palsize;i++)
		DPRINTF("palette: %d, %d, %d\n", image->palette[i].r,
			image->palette[i].g, image->palette[i].b);

	for(i=0;i<(image->width*image->height);i++)
		DPRINTF("imagebits: %d\n", image->imagebits[i]);
		*/
}

/*==========================================================================
 * Copyright (c) 2000, 2001, 2003 Greg Haerr <greg@censoft.com>
 * Portions Copyright (c) 2000 Alex Holden <alex@linuxhacker.org>
 *
 * Image decode routine for PNG files
 *
 * Currently for simplicity we get the PNG library to convert the file to
 * 24 bit RGB format with no alpha channel information even if we could
 * potentially store the image more efficiently by taking note of the image
 * type and depth and acting accordingly. Similarly, > 8 bits per channel,
 * gamma correction, etc. are not supported.
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <png.h>
#include <pngstruct.h>
/* png_jmpbuf() macro is not defined prior to libpng-1.0.6*/
#ifndef png_jmpbuf
#define png_jmpbuf(png_ptr)	((png_ptr)->jmpbuf)
#endif

/* This is a quick user defined function to read from the buffer instead of from the file pointer */
static void
png_read_buffer(png_structp pstruct, png_bytep pointer, png_size_t size)
{
	GdImageBufferRead((buffer_t *)pstruct->io_ptr,pointer,size);
	return ;
}

int 
decode_png(buffer_t * src, imagehdr *pimage)
{
	unsigned char hdr[8], **rows;
	png_structp state;
	png_infop pnginfo;
	png_uint_32 width, height;
	int bit_depth, colourtype, i;

	GdImageBufferSeekTo(src, 0UL);

	if(GdImageBufferRead(src, hdr, 8) != 8)
		return 0;

	if(png_sig_cmp(hdr, 0, 8))
		return 0;

	if(!(state = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
						NULL, NULL))) goto nomem;

	if(!(pnginfo = png_create_info_struct(state))) {
		png_destroy_read_struct(&state, NULL, NULL);
		goto nomem;
	}

	if(setjmp(png_jmpbuf(state))) {
		png_destroy_read_struct(&state, &pnginfo, NULL);
		return 2;
	}

	/* Set up the input function */
	png_set_read_fn(state, src, png_read_buffer);
	/* png_init_io(state, src); */

	png_set_sig_bytes(state, 8);
	png_read_info(state, pnginfo);
	png_get_IHDR(state, pnginfo, &width, &height, &bit_depth, &colourtype,
							NULL, NULL, NULL);

	pimage->width = width;
	pimage->height = height;
	pimage->bpp = 24;
	pimage->planes = 1;
	GdComputeImagePitch(pimage->bpp, pimage->width, &pimage->pitch,
						&pimage->bytesperpixel);
	pimage->compression = MWIMAGE_RGB;
	if(!(pimage->imagebits = (UCHAR *)malloc(pimage->pitch * pimage->height))) {
		png_destroy_read_struct(&state, &pnginfo, NULL);
		goto nomem;
	}
	if(!(rows = (unsigned char **)malloc(pimage->height * sizeof(unsigned char *)))) {
		png_destroy_read_struct(&state, &pnginfo, NULL);
		goto nomem;
	}
	for(i = 0; i < pimage->height; i++)
		rows[pimage->height-1-i] = pimage->imagebits + (i * pimage->pitch);

	png_set_expand(state);
	if(bit_depth == 16)
		png_set_strip_16(state);
	if(colourtype & PNG_COLOR_MASK_ALPHA)
		png_set_strip_alpha(state);
	if(colourtype == PNG_COLOR_TYPE_GRAY ||
			colourtype == PNG_COLOR_TYPE_GRAY_ALPHA)
	png_set_gray_to_rgb(state);

	png_read_image(state, rows);

	png_read_end(state, NULL);
	free(rows);
	png_destroy_read_struct(&state, &pnginfo, NULL);

	return 1;

nomem:
	EPRINTF(" DecodePNG: Out of memory\n");
	return 2;
}

/*========================================================================*/
#include "assert.h"
/*
 * @param pimage Structure describing the image.
 */
int 
decode_pixel(imagehdr *pimage)
{
	MWCOORD x;
	MWCOORD y;
	MWCOORD minx;
	MWCOORD maxx;
	//MWUCHAR bitvalue = 0;
	int bitcount;
	MWUCHAR *imagebits;
	MWCOORD height, width;
	int bpp;
	MWPIXELVAL pixel;
	//int clip;
	int extra, linesize;
	int rgborder;
	MWCOLORVAL cr;
	MWCOORD yoff;
	unsigned long transcolor;
	//MWPIXELVAL convtable[256];

	assert(pimage);

	height = pimage->height;
	width = pimage->width;
	x=width;
	y=height;

	/* determine if entire image is clipped out, save clipresult for later*/

	transcolor = pimage->transcolor;
	bpp = pimage->bpp;
	
	printf("======================================================\n");
	printf("  transcolor=%ld\n",pimage->transcolor);
	printf("  bpp=%d\n",pimage->bpp);
	if(pimage->palette)
		printf(" pimage->palette Not empty\n");
	else 
		printf(" pimage->palette was Empty \n");
	printf("======================================================\n");
	
	/*
	 * Merge the images's palette and build a palette index conversion table.
	 */
	if (pimage->bpp <= 8) {
		return 2;
	}

	minx = x;
	maxx = x + width - 1;
	imagebits = pimage->imagebits;

	/* check for bottom-up image*/
	if( ! (pimage->compression & MWIMAGE_UPSIDEDOWN)) {
		printf(" UP------------\n");
		y += height - 1;
		yoff = -1;
	} else{
		printf(" UP============\n");
		yoff = 1;
	}

#define PIX2BYTES(n)	(((n)+7)/8)
	/* imagebits are dword aligned*/
	switch(pimage->bpp) {
	default:
	case 8:
		linesize = width;
		break;
	case 32:
		linesize = width*4;
		break;
	case 24:
		linesize = width*3;
		break;
	case 4:
		linesize = PIX2BYTES(width<<2);
		break;
	case 1:
		linesize = PIX2BYTES(width);
		break;
	}

	extra = pimage->pitch - linesize;

	/* 24bpp RGB rather than BGR byte order?*/
	rgborder = pimage->compression & MWIMAGE_RGB; 
	printf("  pitch=%d\nlinesize=%d\nwidth=%d\n",pimage->pitch,linesize,width);
	printf("  linesize=%d\n",linesize);
	printf("  extra=%d\n",extra);
	printf("  rgborder=%d\n",rgborder);
	printf("======================================================\n");

	/* bpp==24 for png */
 	if ((bpp == 24) || (bpp == 32)) {
		long trans;
		while (height > 0) {
			/* RGB rather than BGR byte order? */
			trans = cr = rgborder
			        ? MWRGB(imagebits[0], imagebits[1],
			                imagebits[2])
			        : MWRGB(imagebits[2], imagebits[1],
			                imagebits[0]);
			
			//printf("imagebits=%d,%d,%d\n",imagebits[0],imagebits[1],
			               // imagebits[2]);
			imagebits += 3;
			
			/* handle transparent color */
			//pixel = COLOR2PIXEL565(cr);
			
			//printf("pixel=%d\n",pixel);
			if (x++ == maxx) {
			        x = minx;
			        y += yoff;
			        height--;
			        imagebits += extra;
			//printf(" ------------next line will be %d ----------------\n",height);
			}
		}
	}
	return 2;
}

