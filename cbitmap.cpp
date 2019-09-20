/*==========================================================================+
|  Class    : CtmBitMap                          	          				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/07/15                                                    |
|  Remark	: 顯示圖片。		 	                                        |
+==========================================================================*/
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#include	"cbitmap.h"
#include	"_video.h"
#include	"gui.h"
#include	"utils.h"
#include 	"decodepng.h"
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
#ifdef	D_32BIT
extern 	unsigned long   *g_Rows[tmMAXVIDEO_Y];
#else
extern 	unsigned short  *g_Rows[tmMAXVIDEO_Y];			//指向ViewPort的一維數組
#endif

/* 綴樟 */
int getNext(int i, int m, int n)
{
    return (i%n)*m + i/n;
}
 
/* ヶ�� */
int getPre(int i, int m, int n)
{
    return (i%m)*n + i/m;
}
 
/* 揭燴眕狟梓i峈れ萸腔遠 */
void movedata(int *mtx, int i, int m, int n)
{
    int temp = mtx[i];  // 婃湔
    int cur = i;       // 絞ヶ狟梓
    int pre = getPre(cur, m, n);
    while(pre != i)
    {
        mtx[cur] = mtx[pre];
        cur = pre;
        pre = getPre(cur, m, n);
    }
    mtx[cur] = temp;
}
 
/* 蛌离ㄛ撈悜遠揭燴垀衄遠 */
void transpose(int *mtx, int m, int n)
{
    for(int i=0; i<m*n; ++i)
    {
        int next = getNext(i, m, n);
        while(next > i) // �臙窴睆騧昐±犉佽隴笭葩
            next = getNext(next, m, n);
        if(next == i)   // 揭燴絞ヶ遠 
            movedata(mtx, i, m, n);
    }
}
 
/* 怀堤撻淝 */
void print(int *mtx, int m, int n)
{
    for(int i=0; i<m*n; ++i)
    {
        if((i+1)%n == 0)
            //cout <<mtx[i] << "\n";
            printf("[%d][%d] %08x \n",(i+1)/n,(i+1)%n,mtx[i]);
        else
           // cout <<mtx[i] << " ";
           printf("[%d][%d] %08x ",(i+1)/n,(i+1)%n,mtx[i]);
    }
}
/*==========================================================================+
|          Function implementation                                          |
+==========================================================================*/
char new_png_file[128];
/*==========================================================================+
|          Class implementation - CtmFont                                   |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|          Constructor and destructor                                       |
+--------------------------------------------------------------------------*/
CtmBitMap::CtmBitMap()
{
	#ifdef	D_32BIT
	m_KeyColor		= RGB2PIXEL888(0xFF, 0xFF, 0xFF);
	#else
	m_KeyColor		= RGB2PIXEL(0xFF, 0xFF, 0xFF);
	#endif
	m_pBuff			= NULL;
	m_pDoubleBuffer	= NULL;
	m_lWidth		= 0;
	m_lHeight		= 0;
	memset(&m_Info,0,sizeof(m_Info));
	memset(&m_Header,0,sizeof(m_Header));
}

CtmBitMap::~CtmBitMap()
{
	if(m_pBuff != NULL)
		free(m_pBuff);
}
/*==========================================================================+
		check_png_file();
	check if the bmp file is exists.If not ,will convert to png file	
*/
int check_png_file(char *file)
{
	char *cp = NULL;
	struct stat st;
	int  nLength = 0;
	
	bzero(new_png_file,sizeof(new_png_file));
	
	//cp=file;
	cp=strstr(file,".bmp");
	if(cp != NULL)
	{
		strcpy(new_png_file, file);
		nLength = strlen(new_png_file);
		strcpy(new_png_file+(nLength-4),".png");
		new_png_file[nLength+1] = '\0';
		//pf=new_png_file;
		//cp=file;
		//
		//while (*cp != '.'){
		//	//printf("%c\n",*cp);
		//	*pf++=*cp++;
		//}
		//strcpy(pf,".png");
		//*(pf+4)='\0';
	}
	else
	{
		cp=strstr(file,".png");
		if(cp != NULL)
			strcpy(new_png_file, file);
	}
	//printf (" new_png_file end = <%s> \n",new_png_file);

	if(! stat(new_png_file,&st)){
		//printf("  --<yes> have new png file\n");
		return 0;
	}
	else
	{
		//printf("  --<no > png file\n");
		return 1;
	}
	return 0;
}

/*==========================================================================+
		LoadPNG(char *filename);
	Load the png file	
	add <2008/7/3 11:47上午>
*/
int CtmBitMap::LoadPNG(char *pngfile)
{
	BYTE*	pBuff;
	BYTE*	pByte;
	int		i, j, AddCols;
	#ifdef	D_32BIT
	DWORD* 	pWord;
	#else
	WORD* 	pWord;
	#endif
	imagehdr *pimage=NULL;
	BITMAPFILEHEADER *pHeader;
	BITMAPINFOHEADER *pInfo;
	pHeader=&m_Header;
	pInfo=&m_Info.bmiHeader;

	//printf("--New file in Create-- = %s\n", pngfile);
	
	pimage=loadpngfile(pngfile,0);
	if( pimage== NULL)
	{
		//printf("  Load PNG error !\n");
		return tmBMP_ERROR_FILE;
	}
		
/*----------------------------------------------------------------------------*/
	// Fill these header's info manually.	
	pHeader->bfType			= 0x4D42;
	pHeader->bfSize			= pimage->width*pimage->height*3
			+ sizeof(BITMAPFILEHEADER) 
			+ sizeof(BITMAPINFOHEADER); 			// 位圖文件的大小，以字節為單位 
	pHeader->bfReserved1	= 0; 					// 位圖文件保留字，必須為0 
	pHeader->bfReserved2	= 0; 					// 位圖文件保留字，必須為0
	pHeader->bfOffBits		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 				// 位圖數据的起始位置，以相對于位圖
	
	pInfo->biSize			= sizeof(BITMAPINFOHEADER); 				// 本結构所占用字節數 
	pInfo->biWidth			= pimage->width; 				// 位圖的寬度，以像素為單位 
	pInfo->biHeight			= pimage->height; 				// 位圖的高度，以像素為單位 
	pInfo->biPlanes			= 1; 				// 目標設備的級別，必須為1 
	pInfo->biBitCount		= 24;				// 每個像素所需的位數，必須是1(雙色), 4(16色)，8(256色)或24(真彩色)之一 
	pInfo->biCompression	= 0; 			// 位圖壓縮類型，必須是 0(不壓縮), 1(BI_RLE8壓縮型)或2(BI_RLE4壓縮型)之一 
	pInfo->biSizeImage		= pimage->width * pimage->height * 3; 			// 位圖的大小，以字節為單位 
	pInfo->biXPelsPerMeter	= 0x00; 		// 位圖水平分辨率，每米像素數 
	pInfo->biYPelsPerMeter	= 0x00; 		// 位圖垂直分辨率，每米像素數 
	pInfo->biClrUsed		= 0x00;				// 位圖實際使用的顏色表中的顏色數	
	pInfo->biClrImportant	= 0x00;			// 位圖顯示過程中重要的顏色數
/*----------------------------------------------------------------------------*/
	if(m_pBuff != NULL)		//如果原先有圖片，則清空
	{
		free(m_pBuff);
		m_pBuff	= NULL;
	}
	m_pBuff = (char *)malloc( pimage->width * pimage->height 
								* g_infoDrv.nBits >> 3);
	if((pimage->width  * pimage->bpp / 8) % 4 == 0)
	{
		AddCols	= 0;
	}
	else
	{
		AddCols	= 4 - (pimage->width * pimage->height / 8) % 4;
	}
	AddCols= pimage->pitch % pimage->width;
	//printf("  pitch/width=%d,extral=%d,width=%d,pitch=%d\n",
		//		(pimage->pitch % pimage->width),
			//	AddCols,pimage->width,pimage->pitch);
	#ifdef	D_32BIT
	pWord	= (DWORD *)m_pBuff;
	pByte	= (BYTE *)pimage->imagebits;
	if(pimage->bpp == 24 && g_infoDrv.nBits == 32)
	#else
	pWord	= (WORD *)m_pBuff;
	pByte	= (BYTE *)pimage->imagebits;
	if(pimage->bpp == 24 && g_infoDrv.nBits == 16)
	#endif
	{
		for(j = 0; j < pimage->height; j ++)
		{
			for(i = 0; i < pimage->width ; i ++)
			{
				#ifdef	D_32BIT
				*pWord	= RGB565TORGB888(RGB2PIXEL(*(pByte + 0), *(pByte + 1), *(pByte + 2)));
				#else
				*pWord	= RGB2PIXEL(*(pByte + 0), *(pByte + 1), *(pByte + 2));
				#endif
				pWord	++;
				pByte	= pByte + 3;
			}
			pByte	+= AddCols;			
		}
	}
	
	m_lHeight	= pimage->height;
	m_lWidth	= pimage->width ;
	
	//printf("  ----Load PNG done,< %d x %d >\n",m_lHeight,m_lWidth);
	//print_image2(pimage);
	
	//	pimage->imagebits 
	//	fans add 2008/12/26 06:53下午,memory Free
	if (pimage->imagebits != NULL)	free(pimage->imagebits);
	
	free(pimage);
	/*----------------------------------------------------------------------------*/
	return 0;
}
/*==========================================================================+
|	function	Create														|
+---------------------------------------------------------------------------+
|	Parameter	FileName	圖片的文件名									|
|	return		0			成功											|
|				非零		錯誤代碼										|
|	remark		把圖片讀入內存，準備顯示									|
+==========================================================================*/
int		CtmBitMap::Create(char *FileName)
{
	DWORD	FileLength;
	int		FileHandle;
	int		DataLength;
	BYTE*	pBuff = NULL;
	BYTE*	pByte;
	int		i, j, AddCols;
	#ifdef	D_32BIT
	DWORD* 	pWord;
	#else
	WORD* 	pWord;
	#endif
	char 	namebuf[128];

	bzero(namebuf,sizeof(namebuf));
	
	m_lHeight	= 0;
	m_lWidth	= 0;
	/*----------------------------------------------------------*/
	//  check png first
	/*----------------------------------------------------------*/
	//printf("--FileName in Create-- = %s\n", FileName);
	if( check_png_file(FileName) == 0)	//Yes have png
	{
		//printf(" ----will load png  < %s > \n",new_png_file);	
		if( LoadPNG(new_png_file) ==0)	//success 
			return 0;
	}
		
	if((FileHandle = open(FileName, O_RDONLY)) == -1)
	{
		//printf("\n--Can't find the BMP file %s\n", FileName);
		return tmBMP_ERROR_FILE;
	}
	
	
	/*----------------------------------------------------------*/
	//  check bmp first
	/*----------------------------------------------------------*/
	/*
	if((FileHandle = open(FileName, O_RDONLY)) == -1)
	{
		//printf("\n--Can't find the BMP file %s\n", FileName);
		if( check_png_file(FileName) != 0)
			return tmBMP_ERROR_FILE;
		else
		{
			printf(" ----will load png  < %s > \n",new_png_file);	
			LoadPNG(new_png_file);
			return 0;
		}
	}
	*/
	 /* Open the BMP file done ,will use old way */
	
	FileLength = lseek(FileHandle, 0, SEEK_END);
	if(FileLength < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
	{
//		//printf("The style of %s  Error\n", FileName);
		return tmBMP_ERROR_STYLE;
	}
	
	if(m_pBuff != NULL)		//如果原先有圖片，則清空
	{
		free(m_pBuff);
		m_pBuff	= NULL;
	}
	
//	//printf("FileLength is %d\n", FileLength);
	//讀取資料
	lseek(FileHandle, 0, SEEK_SET);
	read(FileHandle, &m_Header, sizeof(BITMAPFILEHEADER));
	read(FileHandle, &m_Info, 	sizeof(BITMAPINFOHEADER));
	
/*	//printf("bfType is %04x\n", 		m_Header.bfType);
	//printf("bfSize is %08x\n", 		m_Header.bfSize);
	//printf("bfReserved1 is %04x\n", m_Header.bfReserved1);
	//printf("bfReserved2 is %04x\n", m_Header.bfReserved2);
	//printf("bfOffBits is %08x\n", 	m_Header.bfOffBits);*/
	
	if(m_Header.bfType != 0x4D42)
	{
//		//printf("The style of %s  isn't BMP\n", FileName);
		return tmBMP_ERROR_STYLE;
	}
	
	DataLength	= 0;
	if(m_Info.bmiHeader.biBitCount == 24)
	{
		DataLength = m_Header.bfSize - sizeof(BITMAPFILEHEADER)
					 - sizeof(BITMAPINFOHEADER);
	}
	
	pBuff	= (BYTE *)malloc(DataLength);
	lseek(FileHandle, m_Header.bfOffBits, SEEK_SET);
	read(FileHandle, pBuff, DataLength);
	close(FileHandle);
	
	m_pBuff = (char *)malloc(m_Info.bmiHeader.biWidth 
								* m_Info.bmiHeader.biHeight 
								* g_infoDrv.nBits >> 3);
	if((m_Info.bmiHeader.biWidth * m_Info.bmiHeader.biBitCount / 8) % 4 == 0)
	{
		AddCols	= 0;
	}
	else
	{
		AddCols	= 4 - (m_Info.bmiHeader.biWidth 
					* m_Info.bmiHeader.biBitCount / 8) % 4;
	}
//	//printf("AddCols is %d\n", AddCols);
/*	for(i = 0; i < 10; i ++)
		//printf("%02x ", *(pBuff + i));*/
	#ifdef	D_32BIT
	pWord	= (DWORD *)m_pBuff;
	pByte	= pBuff;
	if(m_Info.bmiHeader.biBitCount == 24 && g_infoDrv.nBits == 32)
	#else
	pWord	= (WORD *)m_pBuff;
	pByte	= pBuff;
	if(m_Info.bmiHeader.biBitCount == 24 && g_infoDrv.nBits == 16)
	#endif
	{
		for(j = 0; j < m_Info.bmiHeader.biHeight; j ++)
		{
			for(i = 0; i < m_Info.bmiHeader.biWidth; i ++)
			{
				#ifdef D_32BIT
				*pWord  = RGB565TORGB888(RGB2PIXEL(*(pByte + 2), *(pByte + 1), *pByte));
				#else
				*pWord	= RGB2PIXEL(*(pByte + 2), *(pByte + 1), *pByte);
				#endif
				pWord	++;
				pByte	= pByte + 3;
			}
			pByte	+= AddCols;			
		}
	}

	free(pBuff);
	
	m_lHeight	= m_Info.bmiHeader.biHeight;
	m_lWidth	= m_Info.bmiHeader.biWidth;
	
	return 0;
}

/*==========================================================================+
|	function	SetKeyColor													|
+---------------------------------------------------------------------------+
|	Parameter	Color		透明色											|
|	return		0			成功											|
|				非零		錯誤代碼										|
|	remark		設置圖片中某種顏色不顯示									|
+==========================================================================*/
int		CtmBitMap::SetKeyColor(DWORD Color, DWORD MinBGColor, DWORD MaxBGColor)
{
	#ifdef	D_32BIT
	m_KeyColor	= RGB565TORGB888(Color);
	#else
	m_KeyColor	= Color;
	#endif
	
	m_MaxBGColor = MaxBGColor;
	m_MinBGColor = MinBGColor;
	return 0;
}

/*==========================================================================+
|	function	Show														|
+---------------------------------------------------------------------------+
|	Parameter	rect		顯示圖片的范圍									|
|				AFlag		一些參數										|
|	return		0			成功											|
|				非零		錯誤代碼										|
|	remark		在rect規定的范圍內顯示圖片 									|
+==========================================================================*/
int		CtmBitMap::Show(RECT rect, WORD AFlag)
{
	short 	Temp;
	int		i,j;
	int		Row, Col;
	#ifdef	D_32BIT
	DWORD*	pBuff;
	#else
	WORD*	pBuff;
	#endif
	
	if(m_pBuff == NULL)
	{
//		printf("CtmBitMap::Show m_pBuff=NULL\n");
		return 1;
	}
	
	if(rect.left > rect.right) 
	{
		Temp		= rect.right;
		rect.right	= rect.left;
		rect.left	= Temp;
	}
	if(rect.top > rect.bottom)
	{
		Temp		= rect.bottom;
		rect.bottom	= rect.top;
		rect.top	= Temp;
	}
	
	if(rect.left > m_nRgnMax_X && rect.top > m_nRgnMax_Y
		&& rect.right < m_nRgnMin_X && rect.bottom < m_nRgnMin_Y)
	{
		return tmBMP_ERROR_RECT;
	}
	
	if(rect.left < m_nRgnMin_X)			rect.left 	= m_nRgnMin_X;
	if(rect.top < m_nRgnMin_Y)			rect.top	= m_nRgnMin_Y;
	if(rect.right > m_nRgnMax_X)		rect.right	= m_nRgnMax_X;
	if(rect.bottom > m_nRgnMax_Y)		rect.bottom	= m_nRgnMax_Y;
	
	if(rect.right - rect.left + 1 > m_Info.bmiHeader.biWidth)	
		Col	= m_Info.bmiHeader.biWidth;
	else
		Col	= rect.right - rect.left + 1;
		
	if(rect.bottom - rect.top + 1 > m_Info.bmiHeader.biHeight)
	{
		Row	= m_Info.bmiHeader.biHeight;
		
		#ifdef	D_32BIT
		pBuff	= (DWORD *)m_pBuff;
		#else
		pBuff	= (WORD *)m_pBuff;
		#endif
	}
	else
	{
		Row	= rect.bottom - rect.top + 1;
		
		#ifdef	D_32BIT
		pBuff	= (DWORD *)(m_pBuff + (m_Info.bmiHeader.biHeight - Row) * m_Info.bmiHeader.biWidth * sizeof(DWORD));
		#else
		pBuff	= (WORD *)(m_pBuff + (m_Info.bmiHeader.biHeight - Row) * m_Info.bmiHeader.biWidth * sizeof(WORD));
		#endif
	}
	if((AFlag & tmBMP_FILLRECT) != 0)
	{
		//目前隻支持單一擴充
		//if(rect.left - rect.right + 1 == m_Info.biWidth 
		//	&& )
	}

	//pBuff	= (WORD *)m_pBuff;
	#ifdef	D_32BIT
	unsigned long**		pTemp = NULL;
	#else
	unsigned short**	pTemp = NULL;
	#endif
	if (m_pDoubleBuffer != NULL)
	{
		pTemp = m_pDoubleBuffer->GetBuffer();
		m_pDoubleBuffer->InitBuffer(0);
	}
	else
		pTemp = g_Rows;
	if(AFlag & tmBMP_KEYCOLOR)
	{
		for(i = Row - 1; i >= 0; i --)
		{
			for(j = 0; j < Col; j ++)
			{
				//if(*pBuff != 0xffffff)
				//printf("*pBuff=%x m_KeyColor=%x \n", *pBuff, m_KeyColor);
				if (*pBuff != m_KeyColor) 
				{
					if (m_pDoubleBuffer != NULL)
						pTemp[rect.top + i][rect.left + j] = *pBuff;
					else
						pTemp[i][j] = *pBuff;
				}
				else
				{
					if (m_pDoubleBuffer != NULL) 
						pTemp[i][j] = g_Rows[rect.top + i][rect.left + j];
						
				}
				
				pBuff	++;
			}
			pBuff	+=	m_Info.bmiHeader.biWidth	- Col;
		}
	}
	else
	{
		if (m_pDoubleBuffer != NULL)
		{
			for(i = Row - 1; i >= 0; i --)
			{
				for(j = 0; j < Col; j ++)
				{
					pTemp[rect.top + i][rect.left + j] = *pBuff;
					pBuff	++;
				}
				pBuff	+=	m_Info.bmiHeader.biWidth	- Col;
			}
		}
		else			//James add for copy bmp 2010/8/17 
		{
			for(i = Row - 1; i >= 0; i --)
			{
				#ifdef	D_32BIT
				memcpy(pTemp[i], pBuff, Col*4);
				#else
				memcpy(pTemp[i], pBuff, Col*2);
				#endif
				pBuff	+=	m_Info.bmiHeader.biWidth;
			}	
		}
	}
	
	if (m_pDoubleBuffer != NULL)
		m_pDoubleBuffer->DrawWithDoubleBuffer();
	return 0;
}

void		CtmBitMap::SetDoubleBuffer(CtmGDIDoubleBuffer* pDoubleBuffer)
{
	m_pDoubleBuffer = pDoubleBuffer;
}


#ifdef	D_32BIT
int			CtmBitMap::SetImageBuffer(RECT rect, DWORD** pBuff)
#else
int			CtmBitMap::SetImageBuffer(RECT rect, WORD** pBuff)
#endif
{
    short 	Temp;
    int		i,j;
    int		Row, Col;
    #ifdef	D_32BIT
    DWORD*	pTemp = NULL;
    #else
    WORD*	pTemp = NULL;
	#endif
	
    if(pBuff == NULL)
    {
        return 1;
    }

    if(rect.left > rect.right)
    {
        Temp		= rect.right;
        rect.right	= rect.left;
        rect.left	= Temp;
    }
    if(rect.top > rect.bottom)
    {
        Temp		= rect.bottom;
        rect.bottom	= rect.top;
        rect.top	= Temp;
    }
		//printf("1.rect %d  %d  %d  %d  ///  %d  %d \n",rect.left,rect.top,rect.right,rect.bottom,rect.right-rect.left,rect.bottom-rect.top);
    if(rect.left > m_nRgnMax_X && rect.top > m_nRgnMax_Y
        && rect.right < m_nRgnMin_X && rect.bottom < m_nRgnMin_Y)
    {
        return tmBMP_ERROR_RECT;
    }

    /*if(rect.left < m_nRgnMin_X)			rect.left 	= m_nRgnMin_X;
    if(rect.top < m_nRgnMin_Y)			rect.top	= m_nRgnMin_Y;
    if(rect.right > m_nRgnMax_X)		rect.right	= m_nRgnMax_X;
    if(rect.bottom > m_nRgnMax_Y)		rect.bottom	= m_nRgnMax_Y;*/
		//printf("m_nRgnMin_X %d   m_nRgnMax_X %d  m_nRgnMin_Y %d  m_nRgnMax_Y %d \n",m_nRgnMin_X,m_nRgnMax_X,m_nRgnMin_Y,m_nRgnMax_Y	);
		//printf("2.rect %d  %d  %d  %d  ///  %d  %d \n",rect.left,rect.top,rect.right,rect.bottom,rect.right-rect.left,rect.bottom-rect.top);
    if(rect.right - rect.left + 1 > m_Info.bmiHeader.biWidth)
        Col	= m_Info.bmiHeader.biWidth;
    else
        Col	= rect.right - rect.left + 1;

    if(rect.bottom - rect.top + 1 > m_Info.bmiHeader.biHeight)
    {
        Row	= m_Info.bmiHeader.biHeight;
        
				//printf("111.Col = %d  Row = %d\n",Col,Row);
        #ifdef	D_32BIT
        pTemp	= (DWORD *)m_pBuff;
        #else
        pTemp	= (WORD *)m_pBuff;
        #endif
    }
    else
    {
        Row	= rect.bottom - rect.top + 1;
				//printf("222.Col = %d  Row = %d\n",Col,Row);
        #ifdef	D_32BIT
        pTemp	= (DWORD *)(m_pBuff + (m_Info.bmiHeader.biHeight - Row) * m_Info.bmiHeader.biWidth * sizeof(DWORD));
        #else
        pTemp	= (WORD *)(m_pBuff + (m_Info.bmiHeader.biHeight - Row) * m_Info.bmiHeader.biWidth * sizeof(WORD));
        #endif
    }
    if(pTemp == NULL || m_Info.bmiHeader.biSize == 0) return 1;
    //printf("m_Info.bmiHeader.biHeight = %d, biWidth = %d\n", m_Info.bmiHeader.biHeight, m_Info.bmiHeader.biWidth);
    for(i = Row - 1; i >= 0; i --)
    {
        #ifdef	D_32BIT
        memcpy(pBuff[i], pTemp, Col*4);
        #else
        memcpy(pBuff[i], pTemp, Col*2);
        #endif
        pTemp	+=	m_Info.bmiHeader.biWidth;
    }

}

int		SaveScreen(char* szDir)
{
	int			nLength, i, j;
	char		*pBMP;
	BYTE		*pRed, *pGreen, *pBlue;
	BITMAPFILEHEADER	*pHeader;
	BITMAPINFOHEADER	*pInfo;
	#ifdef	D_32BIT
	DWORD				Pixel;
	DWORD				*pwPixel;
	#else
	WORD				Pixel;
	WORD				*pwPixel;
	#endif
	char				cPixel;
	int 				screen_x;
	int      			screen_y;


	if(g_Rotation==0 || g_Rotation==2)
	{
		screen_x=g_infoDrv.nX;
		screen_y=g_infoDrv.nY;
	}
	else if(g_Rotation==1 || g_Rotation==3)
	{
		screen_x = g_infoDrv.nY;
		screen_y = g_infoDrv.nX;
	}
	//printf("222.screen_x = %d  screen_y=%d\n",screen_x,screen_y);
	nLength	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
				+ screen_y * screen_x * 3;
	pBMP	= (char *)malloc(nLength);
	
	pHeader	= (BITMAPFILEHEADER	*)pBMP;
	pInfo	= (BITMAPINFOHEADER	*)(pBMP + sizeof(BITMAPFILEHEADER));
	
	pHeader->bfType			= 0x4D42;
	pHeader->bfSize			= nLength; 				// 位圖文件的大小，以字節為單位 
	pHeader->bfReserved1	= 0; 					// 位圖文件保留字，必須為0 
	pHeader->bfReserved2	= 0; 					// 位圖文件保留字，必須為0
	pHeader->bfOffBits		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 				// 位圖數据的起始位置，以相對于位圖
	
	pInfo->biSize			= sizeof(BITMAPINFOHEADER); 				// 本結构所占用字節數 
	pInfo->biWidth			= screen_x; 				// 位圖的寬度，以像素為單位 
	pInfo->biHeight			= screen_y; 				// 位圖的高度，以像素為單位 
	pInfo->biPlanes			= 1; 				// 目標設備的級別，必須為1 
	pInfo->biBitCount		= 24;				// 每個像素所需的位數，必須是1(雙色), 4(16色)，8(256色)或24(真彩色)之一 
	pInfo->biCompression	= 0; 			// 位圖壓縮類型，必須是 0(不壓縮), 1(BI_RLE8壓縮型)或2(BI_RLE4壓縮型)之一 
	pInfo->biSizeImage		= screen_x* screen_y * 3; 			// 位圖的大小，以字節為單位 
	pInfo->biXPelsPerMeter	= 0x00; 		// 位圖水平分辨率，每米像素數 
	pInfo->biYPelsPerMeter	= 0x00; 		// 位圖垂直分辨率，每米像素數 
	pInfo->biClrUsed		= 0x00;				// 位圖實際使用的顏色表中的顏色數	
	pInfo->biClrImportant	= 0x00;			// 位圖顯示過程中重要的顏色數
	
	pRed	= (BYTE *)(pBMP + pHeader->bfOffBits + 2);
	pGreen	= (BYTE *)(pBMP + pHeader->bfOffBits + 1);
	pBlue	= (BYTE *)(pBMP + pHeader->bfOffBits );
//	_SetViewPort(0, 0, g_infoDrv.nX - 1, g_infoDrv.nY - 1);
	#ifdef	D_32BIT
	pwPixel		= (DWORD *)g_infoDrv.pcMem;
	#else
	pwPixel		= (WORD *)g_infoDrv.pcMem;
	#endif
	for(j = screen_y - 1; j >= 0; j --)
	{
		for(i = 0; i < screen_x; i ++)
		{
			Pixel	= *(pwPixel + j * g_infoDrv.nX + i);//g_Rows[j][i];
			#ifdef	D_32BIT
			*pRed	= (Pixel & 0xFF0000)>> 16;
			*pGreen	= (Pixel & 0xFF00)	>> 8;
			*pBlue	= (Pixel & 0xFF);
			pRed 	+= 3;
			pGreen 	+= 3;
			pBlue 	+= 3;
			#else
			*pRed	= (Pixel & 0xF800)	>> 8;
			*pGreen	= (Pixel & 0x7E0)	>> 3;
			*pBlue	= (Pixel & 0x1F) 	<< 3;
			pRed 	+= 3;
			pGreen 	+= 3;
			pBlue 	+= 3;
			#endif
		}
	}

	
	SaveScreenToMMC(pBMP, nLength, szDir);
	
	free(pBMP);
	return 0;
}





