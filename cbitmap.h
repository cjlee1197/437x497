/*==========================================================================+
|  Class    : CtmBitMap                                        				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/06/23                                                    |
|  Remark	: 圖片類。                                           |
+==========================================================================*/

#ifndef	_BITMAP_H
#define	_BITMAP_H

#include	"common.h"
#include	"gui_ex.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
/*==========================================================================+
|           Macro definition                                                |
+==========================================================================*/
#define		tmBMP_ERROR_SUCCESS		0		//無錯誤
#define		tmBMP_ERROR_FILE		1		//圖形文件不存在
#define		tmBMP_ERROR_STYLE		2		//圖形文件格式錯誤
#define		tmBMP_ERROR_RECT		3		//顯示范圍超出

#define		tmBMP_FILLRECT			0x0001	//把圖充滿指定區間
#define		tmBMP_KEYCOLOR			0x0002	//根據透明色畫
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

typedef struct tagBITMAPFILEHEADER 
{ 
	WORD	bfType; 				// 位圖文件的類型，必須為BM 
	DWORD 	bfSize; 				// 位圖文件的大小，以字節為單位 
	WORD	bfReserved1; 			// 位圖文件保留字，必須為0 
	WORD	bfReserved2; 			// 位圖文件保留字，必須為0
	DWORD 	bfOffBits; 				// 位圖數据的起始位置，以相對于位圖
									// 文件頭的偏移量表示，以字節為單位 
}
#ifdef	D_ARM						//yang 2006/11/29 
__attribute__((packed, aligned(2))) BITMAPFILEHEADER;
#else 
#ifdef	D_X86
BITMAPFILEHEADER;
#endif
#endif

typedef struct tagBITMAPINFOHEADER
{ 
	DWORD 	biSize; 				// 本結构所占用字節數 
	long	biWidth; 				// 位圖的寬度，以像素為單位 
	long	biHeight; 				// 位圖的高度，以像素為單位 
	WORD 	biPlanes; 				// 目標設備的級別，必須為1 
	WORD 	biBitCount;				// 每個像素所需的位數，必須是1(雙色), 
									// 4(16色)，8(256色)或24(真彩色)之一 
	DWORD 	biCompression; 			// 位圖壓縮類型，必須是 0(不壓縮), 
									// 1(BI_RLE8壓縮型)或2(BI_RLE4壓縮型)之一 
	DWORD 	biSizeImage; 			// 位圖的大小，以字節為單位 
	long	biXPelsPerMeter; 		// 位圖水平分辨率，每米像素數 
	long	biYPelsPerMeter; 		// 位圖垂直分辨率，每米像素數 
	DWORD 	biClrUsed;				// 位圖實際使用的顏色表中的顏色數	
	DWORD 	biClrImportant;			// 位圖顯示過程中重要的顏色數 
}
#ifdef	D_ARM						//yang 2006/11/29 
__attribute__((packed, aligned(2))) BITMAPINFOHEADER;
#else 
#ifdef	D_X86
BITMAPINFOHEADER;
#endif
#endif

typedef struct tagRGBQUAD 
{ 
	BYTE	rgbBlue;				// 藍色的亮度(值范圍為0-255) 
	BYTE	rgbGreen; 				// 綠色的亮度(值范圍為0-255) 
	BYTE	rgbRed; 				// 紅色的亮度(值范圍為0-255) 
	BYTE	rgbReserved;			// 保留，必須為0 
}	RGBQUAD;

typedef struct tagBITMAPINFO 
{ 
	BITMAPINFOHEADER 	bmiHeader; 		// 位圖信息頭 
	RGBQUAD 			bmiColors[1]; 	// 顏色表 
}	BITMAPINFO; 

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - CFont                                       |
+==========================================================================*/
class CtmBitMap
{
public:
	CtmBitMap();
	~CtmBitMap();
	
	long	m_lWidth;
	long	m_lHeight; 
	
	int 		LoadPNG(char *pngfile);
	int			Create(char* FileName);
	int			SetKeyColor(DWORD Color, DWORD MinBGColor = 0, DWORD MaxBGColor = 0);
	int			Show(RECT rect, WORD AFlag);
    #ifdef	D_32BIT
    int			SetImageBuffer(RECT rect, DWORD** pBuff);
	#else
	int			SetImageBuffer(RECT rect, WORD** pBuff);
	#endif
	
	void		SetDoubleBuffer(CtmGDIDoubleBuffer* pDoubleBuffer);
	long		GetWidth() { return m_lWidth; }
	long		GetHeight() { return m_lHeight; }
	char*		Data(){return m_pBuff;}
private:
	BITMAPINFO			m_Info;
	BITMAPFILEHEADER	m_Header;
	char*				m_pBuff;
	
	DWORD				m_KeyColor;
	DWORD				m_MaxBGColor;
	DWORD				m_MinBGColor;
	
	
	CtmGDIDoubleBuffer* m_pDoubleBuffer;	//	fans add 2008-10-22 10:55:09
};

int	SaveScreen(char* szDir = NULL);

#endif
