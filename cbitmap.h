/*==========================================================================+
|  Class    : CtmBitMap                                        				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : ���t��                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/06/23                                                    |
|  Remark	: �Ϥ����C                                           |
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
#define		tmBMP_ERROR_SUCCESS		0		//�L���~
#define		tmBMP_ERROR_FILE		1		//�ϧΤ�󤣦s�b
#define		tmBMP_ERROR_STYLE		2		//�ϧΤ��榡���~
#define		tmBMP_ERROR_RECT		3		//��ܭS��W�X

#define		tmBMP_FILLRECT			0x0001	//��ϥR�����w�϶�
#define		tmBMP_KEYCOLOR			0x0002	//�ھڳz����e
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

typedef struct tagBITMAPFILEHEADER 
{ 
	WORD	bfType; 				// ��Ϥ�������A������BM 
	DWORD 	bfSize; 				// ��Ϥ�󪺤j�p�A�H�r�`����� 
	WORD	bfReserved1; 			// ��Ϥ��O�d�r�A������0 
	WORD	bfReserved2; 			// ��Ϥ��O�d�r�A������0
	DWORD 	bfOffBits; 				// ��ϼ��u���_�l��m�A�H�۹�_���
									// ����Y�������q��ܡA�H�r�`����� 
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
	DWORD 	biSize; 				// �����۩ҥe�Φr�`�� 
	long	biWidth; 				// ��Ϫ��e�סA�H��������� 
	long	biHeight; 				// ��Ϫ����סA�H��������� 
	WORD 	biPlanes; 				// �ؼг]�ƪ��ŧO�A������1 
	WORD 	biBitCount;				// �C�ӹ����һݪ���ơA�����O1(����), 
									// 4(16��)�A8(256��)��24(�u�m��)���@ 
	DWORD 	biCompression; 			// ������Y�����A�����O 0(�����Y), 
									// 1(BI_RLE8���Y��)��2(BI_RLE4���Y��)���@ 
	DWORD 	biSizeImage; 			// ��Ϫ��j�p�A�H�r�`����� 
	long	biXPelsPerMeter; 		// ��Ϥ�������v�A�C�̹����� 
	long	biYPelsPerMeter; 		// ��ϫ�������v�A�C�̹����� 
	DWORD 	biClrUsed;				// ��Ϲ�ڨϥΪ��C������C���	
	DWORD 	biClrImportant;			// �����ܹL�{�����n���C��� 
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
	BYTE	rgbBlue;				// �Ŧ⪺�G��(�ȭS��0-255) 
	BYTE	rgbGreen; 				// ��⪺�G��(�ȭS��0-255) 
	BYTE	rgbRed; 				// ���⪺�G��(�ȭS��0-255) 
	BYTE	rgbReserved;			// �O�d�A������0 
}	RGBQUAD;

typedef struct tagBITMAPINFO 
{ 
	BITMAPINFOHEADER 	bmiHeader; 		// ��ϫH���Y 
	RGBQUAD 			bmiColors[1]; 	// �C��� 
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
