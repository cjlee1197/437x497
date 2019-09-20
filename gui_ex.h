/*==========================================================================+
|  Class    :                                                  				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : fans                                               			|
|  Version  : V1.00                                                         |
|  Creation : 2006/11/22                                                    |
|  Remark	: ex gui			                       						|
+==========================================================================*/

#ifndef		_GUI_EX_H
#define		_GUI_EX_H

#include	"gui.h"
#include	"_video.h"
#include	"common.h"

#define		BMP_GRIDINGPXEIL		16		//分割圖片的大小
typedef	struct tagScanPoint
{
	int x;
	int y;
	tagScanPoint*	pNext;
} tmScanPoint;

typedef struct tagPointEx
{
	int x;
	int y;
} tmPointEx;



//	這些函數將只會接受絕對的坐標

void	MemSet_4(void* dest, int nData, int nCount);
void	MemSet_2(void* dest, WORD wData, int nCount);

void	SetPtStackEmpty();
void	FreeStack(tmScanPoint* pStack);
void	PushStack(int x, int y);
void	PopStack(int* x, int* y);

void	_SetPointColorEx(int x, int y, int Color);									//	畫點
int		_GetPointColorEx(int x, int y);													//	獲取某點的色彩

void	_FillRegionEx(int x, int y, int NewColor);	//	填充某些區域
void	_FillRegionEx(tmPointEx*	points, int num, int Color);
void	_FillRegionWithLightEx(int x, int y, int r, int g, int b); 
void	_LineEx(int x0, int y0, int x1, int y1, int Color);		
void	_LineToEx(int x, int y, int Color);
void	_MoveToEx(int x, int y);							
//void	_VLineEx(short x0, short x1, short y, unsigned short Color);
//void	_HLineEx(short y0, short y1, short x, unsigned short Color);
void	_GuiInitEx();
//void FillPolygon(tmPointEx* lpPoints,int nCount,int nColor);

int		_GetColorWithLight(int r, int g, int b, 
						   int nDiffuseAngle, int nDiffuseQ,
						   int nReflectAngle, int nReflectQ,
						   int nEnvironmentR, int nEnvironmentG, int nEnvironmentB,
						   int nReflet = 4);
						   
						   
//	以下全部使用的是雙緩衝技術
#ifdef	D_32BIT
void	_DoubleBuffer(RECT rect, unsigned long* pSource);
#else
void	_DoubleBuffer(RECT rect, unsigned short* pSource);
#endif
int		_SetColorD(unsigned short Color);


class	CtmGDIDoubleBuffer
{
	
	public:
		CtmGDIDoubleBuffer(RECT rect);
		CtmGDIDoubleBuffer();
		virtual	~CtmGDIDoubleBuffer();
	public:
		void	DrawWithDoubleBuffer();
		void	InitFrameBuffer();
		void	DrawWithDoubleBuffer(int x, int y, int nStartX, int nStartY, int nEndX, int nEndY);
		void	DrawLineToInBuffer(int x, int y);
		void	DrawRectangleInBuffer(int Control, int x1, int y1, int x2, int y2);
		void	DrawLineInBuffer(int x1, int y1, int x2, int y2);
		void	DrawVLineInBuffer(int y1, int y2, int x);
		void	DrawHLineInBuffer(int x1, int x2, int y);
		void	DrawPointInBuffer(int x, int y, int Color);
		void	MoveToInBuffer(int x, int y);
		
		void	SetFillRegInBuffer(int x1, int y1, int x2, int y2);
		void	FillInBuffer(DWORD wFillType, WORD wBordeColor, WORD wFillColor, int x1, int y1,  
				WORD wDestColor = 0xFFFF, int nChange = -1);
		void	FillRectInBuffer(DWORD wFillType, WORD wBordeColor, WORD wFillColor, 
			  int x1, int y1, int x2, int y2, 
			  WORD wDestColor = 0xFFFF, int nChange = -1);
		
		void	SetColorInBuffer(int Color);
		void	SetWriteModeInBuffer(int action);
		void	SetPerLen(int nLen);
		void	SetRegion(RECT rect);
		
		int		GetPerLen();
		int		InitBuffer(int Color);
		int		InitBuffer();
		#ifdef	D_32BIT
		unsigned long** GetBuffer();
		#else
		unsigned short** GetBuffer();
		#endif
	private:
		BOOL	CreateBuffer();
		void	DestroyBuffer();	
		
		
	protected:
		int				m_nColor;		//	using color when drawing
		int				m_nWriteMode;	//	write mode
		#ifdef	D_32BIT
		unsigned long**  m_pBuffer;
		#else
		unsigned short**  m_pBuffer;
		#endif
		RECT			m_rect;
		int				m_nXMax;
		int				m_nYMax;
		int				m_nXMin;
		int				m_nYMin;
		int				m_nPerLen;
		#ifdef	D_32BIT
		unsigned long* m_FrameBuffer[2048];
		#else
		unsigned short* m_FrameBuffer[2048];
		#endif
		int				m_nCurrent_X;	
		int				m_nCurrent_Y;
};
#endif
