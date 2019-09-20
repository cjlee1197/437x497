/*==========================================================================+
|  Class    :                                                  				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : ���t��                                               			|
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: �ϧήw			                       						|
+==========================================================================*/
#ifndef		_GUI_H
#define		_GUI_H

#include	"_video.h"

#define _GPSET			3
#define _GPRESET		2
#define _GAND			1
#define _GOR			0
#define _GXOR			4

#define	_GBORDER		0			//�Ϋe����e���
#define	_GFILLINTERIOR	1			//�έI�����R

//	fans add
#define	_COLOR_DECRESE	0x00000002			//	
#define	_COLOR_INCRESE	0x00000004			//	
#define	_COLOR_DEIN		0x00000008			//	�C����ܲH�M�Z�A�ܱj
#define	_COLOR_INDE		0x00000010			//	�C����ܱj�M�Z�A�ܲH
#define	_COLOR_XCHANGE	0x00000100			//	
#define	_COLOR_YCHANGE	0x00000200

int		GUI_Init();
int		_Circle(int Control, int x1, int y1, unsigned short r);	//fans add
void	_Ellipse(int Control, int x1, int y1, int x2, int y2);
void	_Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void	_Line(short x1, short y1, short x2, short y2);
int		_LineTo(short x, short y);
void 	_MoveTo(short x, short y);
int		_Rectangle(int Control, int x1, int y1, int x2, int y2);
int		_SetColor(unsigned short Color);
short	_SetWriteMode(short action);
int		_SetViewPort(int x1, int y1, int x2, int y2);
void	_SetViewOrg(short x, short y);
void	_SetClipRgn(short x1, short y1, short x2, short y2);
void	_SetLineStyle(unsigned short nMask);
unsigned short	_GetLineStyle();

long	_ImageSize(int x1, int y1, int x2, int	y2);
void	_GetImage(int x1, int y1, int x2, int y2, char* image);
void	_PutImage(int x, int y, const char* image, short action);

//	fans add 2008/10/14 04:39�U��
void	_ColorToRGB(DWORD dwColor, BYTE* pOutR, BYTE* pOutG, BYTE* pOutB);
void	_ColorToRGB(WORD wColor, BYTE* pOutR, BYTE* pOutG, BYTE* pOutB);
void	_Fill(DWORD wFillType, WORD wBordeColor, WORD wFillColor, 
			  int x1, int y1, WORD wDestColor = 0xFFFF, int nChange = -1);
void	_FillRect(DWORD wFillType, WORD wBordeColor, WORD wFillColor, 
			  int x1, int y1, int x2, int y2, 
			  WORD wDestColor = 0xFFFF, int nChange = -1);

void	_ArcRect(WORD wColor, int x1, int y1, int x2, int y2);

//add by J.Wong 2017/1/15
class	CImage;
void	_PutImage(int x, int y, const CImage& img,bool alphMask=false);
void	_GetImage(int x, int y, CImage& img);
void 	_CombineImage(int x, int y, CImage& dst, const CImage& src);
int Set_SetCanvas();
int FlushCanvas(RECT *rect,int ncount);
int SetCanvasFlag(BOOL	flag=TRUE);
extern	unsigned short	g_Length_X;				//ViewPort��X�b����
extern	unsigned short	g_Length_Y;				//ViewPort��Y�b����
#ifdef	D_32BIT
extern 	unsigned long   *g_Rows[tmMAXVIDEO_Y];
extern	unsigned long	*pMem;
#else
extern 	unsigned short  *g_Rows[tmMAXVIDEO_Y];			//���VViewPort���@���Ʋ�
extern 	unsigned short	*pMem;
#endif
extern  short	m_nRgnMin_X,m_nRgnMin_Y, m_nRgnMax_X, m_nRgnMax_Y;


#endif
