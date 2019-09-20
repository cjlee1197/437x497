/*==========================================================================+
|  Class    :                                                  				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                               			|
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: 圖形庫			                       						|
+==========================================================================*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include	"gui.h"
#include 	"tmconfig.h"

#include 	<guilite/IGui>

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagtmIMAGE
{
    unsigned short  wWidth;
    unsigned short  wHeight;
    unsigned short  wColorBit;
    unsigned short  wWidthPixel;
    unsigned short  wHeightPixel;
    unsigned char   acMAP[];
} tmIMAGE;
BOOL	u_CanvasFlag=TRUE;
#ifdef	D_32BIT
unsigned long	*pCanvas;
unsigned long	* pTempBuffer;
unsigned long	*pMem;						//指向FrameBuffer的指針。
unsigned long  	*g_Rows[tmMAXVIDEO_Y];				//指向ViewPort的一維數組
#else
unsigned short	*pCanvas;
unsigned short	*pTempBuffer;
unsigned short	*pMem;						//指向FrameBuffer的指針。
unsigned short  *g_Rows[tmMAXVIDEO_Y];				//指向ViewPort的一維數組
#endif
unsigned short	g_Length_X;					//ViewPort的X軸長度
unsigned short	g_Length_Y;					//ViewPort的Y軸長度

unsigned long	Mem_Length;

#ifdef	D_32BIT
unsigned long	Color_FG;
unsigned long	Color_BG;
#else
unsigned short	Color_FG;
unsigned short	Color_BG;
#endif

short	m_nCurrent_X;					//當前畫的點X軸坐標
short	m_nCurrent_Y;					//當前畫的點Y軸坐標
unsigned short	m_nView_X;						//當前視口原點X軸坐標
unsigned short	m_nView_Y;						//當前視口原點Y軸坐標
short	m_nRgnMin_X,m_nRgnMin_Y, m_nRgnMax_X, m_nRgnMax_Y;

short			m_nOrg_X;					// 原點坐標
short			m_nOrg_Y;

short			m_WriteMode		= _GPSET;

int		g_nRotate = 0;
int		g_ViewX = 768;
int		g_ViewY	= 800;
int		g_CurX 	= g_infoDrv.nX;
int		g_CurY	= g_infoDrv.nY;

void	_Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
int		_Circle(int Control, int x1, int y1, unsigned short r);
int		_HLine(short x1, short x2, unsigned short y);
void	_MoveTo(short x, short y);
int		_VLine(short y1, short y2, unsigned short x);
int		_ClipLine(short *px1, short *py1, short *px2, short *py2);

/*==========================================================================+
|  	Function	: GUI_Init                                 					|
|	Return		: 0		初始化圖形庫成功	 		                    	|
|				  其它錯誤代碼												|
|	Remark		: 初始化圖形庫												|
+==========================================================================*/
int GUI_Init()
{
	g_infoDrv._init_Video();
	#ifdef	D_32BIT
	pMem		= (unsigned long *)g_infoDrv.pcMem;
	pCanvas		= (unsigned long *)malloc(g_infoDrv.nX*g_infoDrv.nY*sizeof(unsigned long));
	#else
	pMem		= (unsigned short *)g_infoDrv.pcMem;
	pCanvas		= (unsigned short *)malloc(g_infoDrv.nX*g_infoDrv.nY*sizeof(unsigned short));
	#endif
	memset(pCanvas,0,sizeof(g_infoDrv.nLength));
	memset(pMem,0,sizeof(g_infoDrv.nLength));
	Mem_Length	= g_infoDrv.nLength;
	_SetViewPort(0, 0, g_infoDrv.nX - 1, g_infoDrv.nY - 1);
	Color_FG	= 0xFFFF;
	Color_BG	= 0x0000;
	m_nCurrent_X	= 0;
	m_nCurrent_Y	= 0;
	return 0;
}

int Set_SetCanvas()
{
	if(CtmConfig::GetInstance()->GetCanvasFlag()==1)
	{
		pTempBuffer=pCanvas;
		pCanvas=pMem;
		pMem=pTempBuffer;
	}
	else
	{
		SetCanvasFlag(FALSE);	
	}	
}
int FlushCanvas(RECT *rect,int ncount)
{
	int i=0;
	if(u_CanvasFlag)
	{
		for(int i=0;i<ncount;i++)
		{
			//printf("rect.top=%d,rect.bottom=%d,rect.left=%d,rect.right=%d\n",rect[i].top,rect[i].bottom,rect[i].left,rect[i].right);
			#ifdef D_32BIT
			for(int j=rect[i].top; j<rect[i].bottom; j++)
			{
				memcpy(pCanvas+(j*g_infoDrv.nX+rect[i].left),
				pMem+(j*g_infoDrv.nX+rect[i].left),(rect[i].right-rect[i].left-1)*4);	
			}
			//memcpy(pCanvas+rect[i].top*g_infoDrv.nX,pMem+rect[i].top*g_infoDrv.nX,(rect[i].bottom-rect[i].top)*g_infoDrv.nX*4);
			#else
			for(int j=rect[i].top; j<rect[i].bottom; j++)
			{
				memcpy(pCanvas+(j*g_infoDrv.nX+rect[i].left),
				pMem+(j*g_infoDrv.nX+rect[i].left),(rect[i].right-rect[i].left-1)*2);	
			}
			#endif
		
		}
	}
	Set_SetCanvas();
	SetCanvasFlag(FALSE);
}
int SetCanvasFlag(BOOL	flag)
{
	u_CanvasFlag=flag;	
}
/*==========================================================================+
|		函數名稱：	_Arc													|
|		參數：		x1, y, x2, y2圓弧所在橢圓的邊框							|
|					x3, y3圓弧起始點和橢圓中心連線上的點					|
|					x4, y4圓弧終止點和橢圓中心連線上的點					|
+---------------------------------------------------------------------------+
|		說明：	從(x3,y3)點和橢圓中心的連線和橢圓的交點逆時鐘畫到			|
|				(x4,y4)點和橢圓中心的連線和橢圓的交點。						|
+==========================================================================*/
void	_Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	int	Center_X, 	Center_Y;			//橢圓中心點座標
	int	a,	b;							//橢圓A、B軸長度
	int	Quad_1, Quad_2, Quad_3, Quad_4;	//4像限畫圓弧標志
										//0表示不畫，1表示全畫，2表示上半部分
										//3畫下半部分，4畫兩線所夾部分
										//5畫兩張所夾部外
	int Quad_X3, Quad_X4;				//兩個點的所在的像限
	int	x_3, x_4, y_3, y_4;				//座標原點平移後的兩個點的坐標
	
	int	d1, d2, d3, x, y;
	int	dx, dy, dx_1, dy_1, dx1, dy1;
	int	dx3, dy3, dx4, dy4, A3, B3, A4, B4;
	float	k1 = 0.0, k2 = 0.0;
	
	a	= (x2 - x1) >> 1;
	b	= (y2 - y1) >> 1;
	Center_X	= x1 + a;				//計算橢圓中心點座標
	Center_Y	= y1 + b;
	
	if(abs(a) == abs(b))				//如果是圓的圓弧，則另外函數畫。
	{
		//_Circle(Control, cx, cy, abs(a));	
		return;
	}
	a	= abs(a);						//計算出A、B軸長度。
	b	= abs(b);
	
	x_3	= x3 - Center_X;				//計算原點平移到(Center_x, Center_y)
	y_3	= y3 - Center_Y;				//後兩個點的坐標。
	x_4	= x4 - Center_X;
	y_4	= y4 - Center_Y;
	
	A3	= abs(y_3);
	B3	= abs(x_3);
	A4	= abs(y_4);
	B4	= abs(x_4);

	if(A3 != 0)
		k1 = (float)B3 / (float)A3;
	if(A4 != 0)
		k2 = (float)B4 / (float)A4;
	
	if(x_3 > 0 && y_3 >= 0)				//分別判斷兩個點所處的像限
		Quad_X3	= 1;
	else if(x_3 <= 0 && y_3 > 0)
		Quad_X3	= 2;
	else if(x_3 < 0 && y_3 <= 0)
		Quad_X3	= 3;
	else
		Quad_X3	= 4;
		
	if(x_4 > 0 && y_4 >= 0)
		Quad_X4	= 1;
	else if(x_4 <= 0 && y_4 > 0)
		Quad_X4	= 2;
	else if(x_4 < 0 && y_4 <= 0)
		Quad_X4	= 3;
	else
		Quad_X4	= 4;
	
	//計算每個像限的畫圖方式
	if(Quad_X3 == 1)
	{
		switch(Quad_X4)
		{
		case 1:	if(A3 != 0 && A4 != 0 && k1 < k2)
				{
					Quad_1	= 5;
					Quad_2	= 1;
					Quad_3	= 1;
					Quad_4	= 1;
				}
				else if(A3 != 0 && A4 != 0 && k1 >= k2)
				{
					Quad_1	= 4;
					Quad_2	= 0;
					Quad_3	= 0;
					Quad_4	= 0;
				}
				else if(A3!=0 && A4 == 0)
				{
					Quad_1	= 3;
					Quad_2	= 1;
					Quad_3	= 1;
					Quad_4	= 1;
				}
				else
				{
					Quad_1	= 4;
					Quad_2	= 0;
					Quad_3	= 0;
					Quad_4	= 0;
				}
				break;
		case 2:	
				Quad_1		= 3;
				Quad_2		= 3;
				Quad_3		= 1;
				Quad_4		= 1;
				break;
		case 3:	
				Quad_1		= 3;
				Quad_2		= 0;
				Quad_3		= 2;
				Quad_4		= 1;
				break;
		case 4:
				Quad_1		= 3;
				Quad_2		= 0;
				Quad_3		= 0;
				Quad_4		= 3;
				break;
		}
	}
	else if(Quad_X3 == 2)
	{
		switch(Quad_X4)
		{
		case 1:
				Quad_1		= 2;
				Quad_2		= 2;
				Quad_3		= 0;
				Quad_4		= 0;
				break;
		case 2:	
				if(A3 != 0 && A4 != 0 && k1 <= k2)
				{
					Quad_1	= 0;
					Quad_2	= 4;
					Quad_3	= 0;
					Quad_4	= 0;
				}
				else if(A3 != 0 && A4 != 0 && k1 > k2)
				{
					Quad_1	= 1;
					Quad_2	= 5;
					Quad_3	= 1;
					Quad_4	= 1;
				}
				else if(A3!=0 && A4 == 0)
				{
					Quad_1	= 0;
					Quad_2	= 2;
					Quad_3	= 0;
					Quad_4	= 0;
				}
				else
				{
					Quad_1	= 1;
					Quad_2	= 3;
					Quad_3	= 1;
					Quad_4	= 1;
				}
				break;
		case 3:
				Quad_1		= 1;
				Quad_2		= 2;
				Quad_3		= 2;
				Quad_4		= 1;
				break;
		case 4:
				Quad_1		= 1;
				Quad_2		= 2;
				Quad_3		= 0;
				Quad_4		= 3;
				break;
		}
	}
	else if(Quad_X3 == 3)
	{
		switch(Quad_X4)
		{
		case 1:
				Quad_1		= 2;
				Quad_2		= 1;
				Quad_3		= 3;
				Quad_4		= 0;
				break;
		case 2:
				Quad_1		= 0;
				Quad_2		= 3;
				Quad_3		= 3;
				Quad_4		= 0;
				break;
		case 3:
				if(A3 != 0 && A4 != 0 && k1 >= k2)
				{
					Quad_1	= 0;
					Quad_2	= 0;
					Quad_3	= 4;
					Quad_4	= 0;
				}
				else if(A3 != 0 && A4 != 0 && k1 < k2)
				{
					Quad_1	= 1;
					Quad_2	= 1;
					Quad_3	= 5;
					Quad_4	= 1;
				}
				else if(A3!=0 && A4 == 0)
				{
					Quad_1	= 1;
					Quad_2	= 1;
					Quad_3	= 3;
					Quad_4	= 1;
				}
				else
				{
					Quad_1	= 0;
					Quad_2	= 0;
					Quad_3	= 3;
					Quad_4	= 0;
				}
				break;
		case 4:
				Quad_1		= 1;
				Quad_2		= 1;
				Quad_3		= 3;
				Quad_4		= 3;
				break;
		}
	}
	else if(Quad_X3 == 4)
	{
		switch(Quad_X4)
		{
		case 1:
				Quad_1		= 2;
				Quad_2		= 1;
				Quad_3		= 1;
				Quad_4		= 2;
				break;
		case 2:
				Quad_1		= 0;
				Quad_2		= 3;
				Quad_3		= 1;
				Quad_4		= 2;
				break;
		case 3:
				Quad_1		= 0;
				Quad_2		= 0;
				Quad_3		= 2;
				Quad_4		= 2;
				break;
		case 4:
				if(A3 != 0 && A4 != 0 && k1 <= k2)
				{
					Quad_1	= 0;
					Quad_2	= 0;
					Quad_3	= 0;
					Quad_4	= 4;
				}
				else if(A3 != 0 && A4 != 0 && k1 > k2)
				{
					Quad_1	= 1;
					Quad_2	= 1;
					Quad_3	= 1;
					Quad_4	= 5;
				}
				else if(A3!=0 && A4 == 0)
				{
					Quad_1	= 0;
					Quad_2	= 0;
					Quad_3	= 0;
					Quad_4	= 2;
				}
				else
				{
					Quad_1	= 1;
					Quad_2	= 1;
					Quad_3	= 1;
					Quad_4	= 3;
				}
				break;
		}
	}

	x_3	= abs(x_3);
	y_3	= abs(y_3);
	x_4	= abs(x_4);
	y_4	= abs(y_4);
	
	x	= 0; 
	y	= b;
	
	
	dx	= 3 * b * b;					//X每加1，
	dy	= 2 * a * a * (1 - b);
	dx_1= 2 * b * b;
	dy_1= 2 * a * a;
	dx1	= b * b;
	dy1 = a * -a;
	d2	= b * b;
	d3	= a * a * (b + b - 1) / 2; 
	d1	= d2 + a * a * (1 - 4 * b) / 4; 
	
	dx3	= 0 * A3;
	dy3	= b * B3;
	dx4	= 0 * A4;
	dy4	= b * B4;
	
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	if(g_nRotate == 90)
	{
		if(Quad_2 == 1 || Quad_2 == 2)
		{
			nCurX = Center_X;
			nCurY = Center_Y + y;
			
			nSetX = nCurY;
			nSetY = g_CurY - nCurX -1;
			g_Rows[nSetY][nSetX]	= Color_FG;
		}
		else if(Quad_4 ==1 || Quad_4 == 2)
		{
			nCurX = Center_X;
			nCurY = Center_Y - y;
			
			nSetX = nCurY;
			nSetY = g_CurY - nCurX -1;
			g_Rows[nSetY][nSetX]	= Color_FG;
		}
	}
	else
	{
		if(Quad_2 == 1 || Quad_2 == 2)
			g_Rows[Center_Y + y][Center_X]	= Color_FG;
		else if(Quad_4 ==1 || Quad_4 == 2)
			g_Rows[Center_Y - y][Center_X]	= Color_FG;
	}
	
	//畫上半圓弧
	while(d2 < d3)
	{
		if(d1 < 0)
		{
			d1	+= dx;
			dx	+= dx_1;

			x	++;
		}
		else
		{
			d1	+= dx + dy;
			dx	+= dx_1;
			dy	+= dy_1;
			d3	+= dy1;
			dy3	-= B3;
			dy4	-= B4;
			x	++;
			y	--;
		}		
		d2	+= dx1;
		dx3	+= A3;
		dx4	+= A4; 		
		
		int	nCurX = 0, nCurY = 0;
		int	nSetX = 0, nSetY = 0;
		if(g_nRotate == 90)
		{
			switch(Quad_1)
			{
			case 1:	
				nCurX = Center_X + x;
				nCurY = Center_Y + y;
				
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
				break;	
			case 2:
				nCurX = Center_X + x;
				nCurY = Center_Y + y;
				
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
				if(Quad_X3 == 1 && dx3 <= dy3 || Quad_X4 == 1 && dx4 <= dy4)
					g_Rows[nSetY][nSetX]	= Color_FG;
				break;
			case 3:	
				nCurX = Center_X + x;
				nCurY = Center_Y + y;
				
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				if(Quad_X3 == 1 && dx3 >= dy3 || Quad_X4 == 1 && dx4 >= dy4)
					g_Rows[nSetY][nSetX]	= Color_FG;
				break;
			case 4:
				nCurX = Center_X + x;
				nCurY = Center_Y + y;
				
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				if(dx3 >= dy3 && dx4 <= dy4)
					g_Rows[nSetY][nSetX]	= Color_FG;
				break;
			case 5:
				nCurX = Center_X + x;
				nCurY = Center_Y + y;
				
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				if(dx3 >= dy3 || dx4 <= dy4)
					g_Rows[nSetY][nSetX]	= Color_FG;
				break;
			}	
			switch(Quad_2)
			{
				case 1:	
					nCurX = Center_X - x;
					nCurY = Center_Y + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					nCurX = Center_X - x;
					nCurY = Center_Y + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(Quad_X3 == 2 && dx3 <= dy3 || Quad_X4 == 2 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					nCurX = Center_X - x;
					nCurY = Center_Y + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(Quad_X3 == 2 && dx3 >= dy3 || Quad_X4 == 2 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					nCurX = Center_X - x;
					nCurY = Center_Y + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					nCurX = Center_X - x;
					nCurY = Center_Y + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}
			switch(Quad_3)
			{
				case 1:
					nCurX = Center_X - x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					nCurX = Center_X - x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(Quad_X3 == 3 && dx3 <= dy3 || Quad_X4 == 3 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					nCurX = Center_X - x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(Quad_X3 == 3 && dx3 >= dy3 || Quad_X4 == 3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					nCurX = Center_X - x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(dx3 >= dy3 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					nCurX = Center_X - x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(dx3 >= dx3 || dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}
			switch(Quad_4)
			{
				case 1:
					nCurX = Center_X + x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					nCurX = Center_X + x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(Quad_X3 == 4 && dx3 <= dy3 || Quad_X4 == 4 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					nCurX = Center_X + x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(Quad_X3 == 4 && dx3 >= dy3 || Quad_X4 == 4 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					nCurX = Center_X + x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					nCurX = Center_X + x;
					nCurY = Center_Y - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}
		}
		else
		{
			switch(Quad_1)
			{
			case 1:	g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 1 && dx3 <= dy3 || Quad_X4 == 1 && dx4 <= dy4)
						g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 1 && dx3 >= dy3 || Quad_X4 == 1 && dx4 >= dy4)
						g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			case 4:	if(dx3 >= dy3 && dx4 <= dy4)
						g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			case 5:	if(dx3 >= dy3 || dx4 <= dy4)
						g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			}	
			switch(Quad_2)
			{
			case 1:	g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 2 && dx3 <= dy3 || Quad_X4 == 2 && dx4 <= dy4)
						g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 2 && dx3 >= dy3 || Quad_X4 == 2 && dx4 >= dy4)
						g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			case 4:	if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			case 5:	if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			}
			switch(Quad_3)
			{
			case 1:	g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 3 && dx3 <= dy3 || Quad_X4 == 3 && dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 3 && dx3 >= dy3 || Quad_X4 == 3 && dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			case 4:	if(dx3 >= dy3 && dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			case 5:	if(dx3 >= dx3 || dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			}
			switch(Quad_4)
			{
			case 1:	g_Rows[Center_Y - y][Center_X = x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 4 && dx3 <= dy3 || Quad_X4 == 4 && dx4 <= dy4)
					{
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
	//					////printf("gui.cpp:_Arc()dx3=%d, dy3=%d  ",dx3,dy3);
					}
					break;
			case 3:	if(Quad_X3 == 4 && dx3 >= dy3 || Quad_X4 == 4 && dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			case 4:	if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			case 5:	if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			}
		}
	}
	
	d1	= (int)(sqrt(b * (x + 0.5)) + sqrt(a * (y - 1)) - sqrt(a * b));
	dx	= 2 * b * b * (x + 1);
	dy	= a * a * (-2 * y + 3);
	while(y > 0)
	{
		if(d1 < 0)
		{
			d1	+= dx + dy;
			dx	+= dx_1;
			dy	+= dy_1;
			dx3	+= A3;
			dx4	+= A4;
			x	++;
			y	--;
		}
		else
		{
			d1	+= dy;
			dy	+= dy_1;
			y	--;
		}
		
		dy3	-= B3;
		dy4	-= B4;
		
		int	nCurX = 0, nCurY = 0;
		int	nSetX = 0, nSetY = 0;
		if(g_nRotate == 90)
		{
			nCurX = Center_X + x;
			nCurY = Center_Y + y;
			
			nSetX = nCurY;
			nSetY = g_CurY - nCurX -1;
			switch(Quad_1)
			{
				case 1:
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					if(Quad_X3 == 1 && dx3 <= dy3 || Quad_X4 == 1 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					if(Quad_X3 == 1 && dx3 >= dy3 || Quad_X4 == 1 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					if(dx3 >= dy3 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					if(dx3 >= dy3 || dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}	
			nCurX = Center_X - x;
			nCurY = Center_Y + y;
			
			nSetX = nCurY;
			nSetY = g_CurY - nCurX -1;
			switch(Quad_2)
			{
				case 1:
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					if(Quad_X3 == 2 && dx3 <= dy3 || Quad_X4 == 2 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					if(Quad_X3 == 2 && dx3 >= dy3 || Quad_X4 == 2 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}
			nCurX = Center_X - x;
			nCurY = Center_Y - y;
			
			nSetX = nCurY;
			nSetY = g_CurY - nCurX -1;
			switch(Quad_3)
			{
				case 1:
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					if(Quad_X3 == 3 && dx3 <= dy3 || Quad_X4 == 3 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					if(Quad_X3 == 3 && dx3 >= dy3 || Quad_X4 == 3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					if(dx3 >= dy3 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					if(dx3 >= dx3 || dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}
			nCurX = Center_X + x;
			nCurY = Center_Y - y;
			
			nSetX = nCurY;
			nSetY = g_CurY - nCurX -1;
			switch(Quad_4)
			{
				case 1:
					g_Rows[nSetY][nSetX]	= Color_FG;
					break;	
				case 2:
					if(Quad_X3 == 4 && dx3 <= dy3 || Quad_X4 == 3 && dx4 <= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 3:
					if(Quad_X3 == 4 && dx3 >= dy3 || Quad_X4 == 3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 4:
					if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
				case 5:
					if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[nSetY][nSetX]	= Color_FG;
					break;
			}
		}
		else
		{
			switch(Quad_1)
			{
			case 1:	g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 1 && dx3 <= dy3 || Quad_X4 == 1 && dx4 <= dy4)
						g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 1 && dx3 >= dy3 || Quad_X4 == 1 && dx4 >= dy4)
						g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			case 4:	if(dx3 >= dy3 && dx4 <= dy4)
					g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			case 5:	if(dx3 >= dy3 || dx4 <= dy4)
					g_Rows[Center_Y + y][Center_X + x]	= Color_FG;
					break;
			}	
			switch(Quad_2)
			{
			case 1:	g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 2 && dx3 <= dy3 || Quad_X4 == 2 && dx4 <= dy4)
						g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 2 && dx3 >= dy3 || Quad_X4 == 2 && dx4 >= dy4)
						g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			case 4:	if(dx3 <= dy3 && dx4 >= dy4)
					g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			case 5:	if(dx3 <= dy3 || dx4 >= dy4)
					g_Rows[Center_Y + y][Center_X - x]	= Color_FG;
					break;
			}
			switch(Quad_3)
			{
			case 1:	g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 3 && dx3 <= dy3 || Quad_X4 == 3 && dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 3 && dx3 >= dy3 || Quad_X4 == 3 && dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			case 4:	if(dx3 >= dy3 && dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			case 5:	if(dx3 >= dx3 || dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X - x]	= Color_FG;
					break;
			}
			switch(Quad_4)
			{
			case 1:	g_Rows[Center_Y - y][Center_X = x]	= Color_FG;
					break;	
			case 2:	if(Quad_X3 == 4 && dx3 <= dy3 || Quad_X4 == 3 && dx4 <= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			case 3:	if(Quad_X3 == 4 && dx3 >= dy3 || Quad_X4 == 3 && dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			case 4:	if(dx3 <= dy3 && dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			case 5:	if(dx3 <= dy3 || dx4 >= dy4)
						g_Rows[Center_Y - y][Center_X + x]	= Color_FG;
					break;
			}
		}
	}
	
//	int	nCurX = 0, nCurY = 0;
//	int	nSetX = 0, nSetY = 0;
	if(g_nRotate == 90)
	{
		nCurX = Center_X - x_3;
		nCurY = Center_Y - y_3;
		
		nSetX = nCurY;
		nSetY = g_CurY - nCurX -1;
		g_Rows[nSetY][nSetX]	= Color_FG;
		
		nCurX = Center_X + x_4;
		nCurY = Center_Y + y_4;
		
		nSetX = nCurY;
		nSetY = g_CurY - nCurX -1;
		g_Rows[nSetY][nSetX]	= Color_FG;
	}
	else
	{
		g_Rows[Center_Y - y_3][Center_X - x_3]	= Color_FG;
		g_Rows[Center_Y + y_4][Center_X + x_4]	= Color_FG;
	}
	
}

int	_Circle(int Control, int x1, int y1, unsigned short r)
{
	int x 	= 0, 	y	= r;
	int dx 	= 3,	dy	= 2 - r - r;
	int	d	= 1 - r;
	int	i;
	int	X_Start, X_End;
	//如果整個圓的范圍超出了ViewPort，則反回0；
	if(x1 - r >= m_nRgnMax_X || x1 + r < m_nRgnMin_X 
			|| y1 - r >= m_nRgnMax_Y|| y1 + r < m_nRgnMin_Y)
		return 0;
	
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	
	if(Control == _GBORDER)
	{		
		if(g_nRotate == 90)
		{
			if(y1 + y > m_nRgnMin_Y && y1 + y < m_nRgnMax_Y)
			{
				if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
				{
					nCurX = x1 + x;
					nCurY = y1 + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
				if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
				{
					nCurX = x1 - x;
					nCurY = y1 + y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
			if(y1 - y > m_nRgnMin_Y && y1 - y < m_nRgnMax_Y)
			{
				if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
				{
					nCurX = x1 + x;
					nCurY = y1 - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
				if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
				{
					nCurX = x1 - x;
					nCurY = y1 - y;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
			
			if(y1 + x > m_nRgnMin_Y && y1 + x < m_nRgnMax_Y)
			{
				if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
				{
					nCurX = x1 + y;
					nCurY = y1 + x;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
				if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)
				{
					nCurX = x1 - y;
					nCurY = y1 + x;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
			if(y1 - x > m_nRgnMin_Y && y1 - x < m_nRgnMax_Y)
			{
				if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
				{
					nCurX = x1 + y;
					nCurY = y1 - x;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
				if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)
				{
					nCurX = x1 - y;
					nCurY = y1 - x;
					
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
		}
		else
		{
			if(y1 + y > m_nRgnMin_Y && y1 + y < m_nRgnMax_Y)
			{
				if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
					g_Rows[y1 + y][x1 + x]	= Color_FG;
				if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
					g_Rows[y1 + y][x1 - x]	= Color_FG;
			}
			if(y1 - y > m_nRgnMin_Y && y1 - y < m_nRgnMax_Y)
			{
				if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
					g_Rows[y1 - y][x1 + x]	= Color_FG;
				if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
					g_Rows[y1 - y][x1 - x]	= Color_FG;
			}
			
			if(y1 + x > m_nRgnMin_Y && y1 + x < m_nRgnMax_Y)
			{
				if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
					g_Rows[y1 + x][x1 + y]	= Color_FG;
				if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)
					g_Rows[y1 + x][x1 - y]	= Color_FG;
			}
			if(y1 - x > m_nRgnMin_Y && y1 - x < m_nRgnMax_Y)
			{
				if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
					g_Rows[y1 - x][x1 + y]	= Color_FG;
				if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)		
					g_Rows[y1 - x][x1 - y]	= Color_FG;
			}
		}
		
		while(x < y)
		{
			if(d < 0)
			{
				d	+= dx;
				dx	+= 2;
				x	++;
			}
			else
			{
				d	+= (dx + dy);
				dx	+= 2;
				dy	+= 2;
				x	++;
				y	--;
			}
			
			if(g_nRotate == 90)
			{
				if(y1 + y > m_nRgnMin_Y && y1 + y < m_nRgnMax_Y)
				{
					if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
					{
						nCurX = x1 + x;
						nCurY = y1 + y;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
					{
						nCurX = x1 - x;
						nCurY = y1 + y;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				if(y1 - y > m_nRgnMin_Y && y1 - y < m_nRgnMax_Y)
				{
					if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
					{
						nCurX = x1 + x;
						nCurY = y1 - y;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
					{
						nCurX = x1 - x;
						nCurY = y1 - y;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				
				if(y1 + x > m_nRgnMin_Y && y1 + x < m_nRgnMax_Y)
				{
					if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
					{
						nCurX = x1 + y;
						nCurY = y1 + x;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)
					{
						nCurX = x1 - y;
						nCurY = y1 + x;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				if(y1 - x > m_nRgnMin_Y && y1 - x < m_nRgnMax_Y)
				{
					if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
					{
						nCurX = x1 + y;
						nCurY = y1 - x;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)
					{
						nCurX = x1 - y;
						nCurY = y1 - x;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
			}
			else
			{
				if(y1 + y > m_nRgnMin_Y && y1 + y < m_nRgnMax_Y)
				{
					if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
						g_Rows[y1 + y][x1 + x]	= Color_FG;
					if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
						g_Rows[y1 + y][x1 - x]	= Color_FG;
				}
				if(y1 - y > m_nRgnMin_Y && y1 - y < m_nRgnMax_Y)
				{
					if(x1 + x > m_nRgnMin_X && x1 + x < m_nRgnMax_X)
						g_Rows[y1 - y][x1 + x]	= Color_FG;
					if(x1 - x > m_nRgnMin_X && x1 - x < m_nRgnMax_X)
						g_Rows[y1 - y][x1 - x]	= Color_FG;
				}
				
				if(y1 + x > m_nRgnMin_Y && y1 + x < m_nRgnMax_Y)
				{
					if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
						g_Rows[y1 + x][x1 + y]	= Color_FG;
					if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)
						g_Rows[y1 + x][x1 - y]	= Color_FG;
				}
				if(y1 - x > m_nRgnMin_Y && y1 - x < m_nRgnMax_Y)
				{
					if(x1 + y > m_nRgnMin_X && x1 + y < m_nRgnMax_X)
						g_Rows[y1 - x][x1 + y]	= Color_FG;
					if(x1 - y > m_nRgnMin_X && x1 - y < m_nRgnMax_X)		
						g_Rows[y1 - x][x1 - y]	= Color_FG;
				}
			}
/*			
			Rows[y1 + y][x1 + x]	= Color_FG;
			Rows[y1 + y][x1 - x]	= Color_FG;
			Rows[y1 - y][x1 + x]	= Color_FG;
			Rows[y1 - y][x1 - x]	= Color_FG;
			
			Rows[y1 + x][x1 + y]	= Color_FG;
			Rows[y1 - x][x1 + y]	= Color_FG;
			Rows[y1 + x][x1 - y]	= Color_FG;
			Rows[y1 - x][x1 - y]	= Color_FG;	
*/	
		}
		return 1;
	}
	
	
	if(y1 >= m_nRgnMin_Y && y1 < m_nRgnMax_Y)
	{
		if(x1 - r < m_nRgnMin_X)
			X_Start	= m_nRgnMin_X;
		else
			X_Start = x1 - r;
		if(x1 + r > m_nRgnMax_X)	
			X_End	= m_nRgnMax_X;
		else
			X_End	= x1 + r;
		
		if(g_nRotate == 90)
		{
			for(i = X_Start; i <= X_End; i++)
			{
				nCurX = i;
				nCurY = y1;
				
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX] = Color_FG;
			}
		}
		else
		{
			for(i = X_Start; i <= X_End; i++)
				g_Rows[y1][i] = Color_FG;
		}
	}

	while(x < y)
	{		
		if(d < 0)
		{
			d	+= dx;
			dx	+= 2;
			x	++;
		}
		else
		{
			d	+= (dx + dy);
			dx	+= 2;
			dy	+= 2;
			
			if(x1 - x < m_nRgnMin_X)
				X_Start	= m_nRgnMin_X;
			else
				X_Start = x1 - x;
			if(x1 + x > m_nRgnMax_X)	
				X_End	= m_nRgnMax_X;
			else
				X_End	= x1 + x;
			
			if(g_nRotate == 90)
			{
				if(y1 + y <= m_nRgnMax_Y)
				{				
					for(i= X_Start; i<= X_End; i ++)
					{
						nCurX = i;
						nCurY = y1 + y;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				if(y1 - y >= m_nRgnMin_Y)
				{
					for(i= X_Start; i <= X_End; i ++)
					{
						nCurX = i;
						nCurY = y1 - y;
						
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}	
				}
				y	--;
				x	++;
			}
			else
			{
				if(y1 + y <= m_nRgnMax_Y)
				{				
					for(i= X_Start; i<= X_End; i ++)
					{
						g_Rows[y1 + y][i]	= Color_FG;
					}
				}
				if(y1 - y >= m_nRgnMin_Y)
				{
					for(i= X_Start; i <= X_End; i ++)
					{
						g_Rows[y1 - y][i]	= Color_FG;
					}	
				}
				y	--;
				x	++;
			}			
		}
		
		if(x1 - y < m_nRgnMin_X)
			X_Start	= m_nRgnMin_X;
		else
			X_Start	= x1 - y;
		if(x1 + y > m_nRgnMax_X)
			X_End	= m_nRgnMax_X;
		else
			X_End	= x1 + y;
			
		if(g_nRotate == 90)
		{
			if(y1 + x <= m_nRgnMax_Y)
			{			
				for(i = X_Start; i <= X_End; i ++)
				{
					nCurX = i;
					nCurY = y1 + x;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
			if(y1 - x >= m_nRgnMin_Y)
			{
				for(i = X_Start; i <= X_End; i ++)
				{
					nCurX = i;
					nCurY = y1 - x;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
		}
		else
		{
			if(y1 + x <= m_nRgnMax_Y)
			{			
				for(i = X_Start; i <= X_End; i ++)
					g_Rows[y1 + x][i]	= Color_FG;
			}
			if(y1 - x >= m_nRgnMin_Y)
			{
				for(i = X_Start; i <= X_End; i ++)
					g_Rows[y1 - x][i]	= Color_FG;
			}
		}
	}
/*	
	g_Rows[y1][x1 - r]	= Color_FG;
	g_Rows[y1][x1 + r]	= Color_FG;
*/
	return 1;
}

void	_Ellipse(int Control, int x1, int y1, int x2, int y2)
{
	int		a, b, x, y;
	int		d1, d2, d3;
	int		dx, dy, dx_1, dy_1, dx1, dy1;
	int		cx, cy, i;
	int		Start_X, End_X, m_nCurrent_Y;
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	
	a	= (x2 - x1) >> 1;
	b	= (y2 - y1) >> 1;
	cx	= x1 + a;
	cy	= y1 + b;
	
	if(abs(a) == abs(b))
	{
		_Circle(Control, cx, cy, abs(a));
		return;
	}
	a	= abs(a);
	b	= abs(b);
	
	x	= 0; 
	y	= b;
	if(Control == _GBORDER)
	{
		if(g_nRotate == 90)
		{
			if(cy + y >= m_nRgnMin_Y && cy + y <= m_nRgnMax_Y)
			{
				if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
				{
					nCurX = cx + x;
					nCurY = cy + y;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
				if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
				{
					nCurX = cx - x;
					nCurY = cy + y;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
			if(cy - y >= m_nRgnMin_Y && cy - y <= m_nRgnMax_Y )
			{
				if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
				{
					nCurX = cx + x;
					nCurY = cy - y;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
				if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
				{
					nCurX = cx - x;
					nCurY = cy - y;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
		}
		else
		{
			if(cy + y >= m_nRgnMin_Y && cy + y <= m_nRgnMax_Y)
			{
				if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
					g_Rows[cy + y][cx + x]	= Color_FG;
				if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
					g_Rows[cy + y][cx - x]	= Color_FG;	
			}
			if(cy - y >= m_nRgnMin_Y && cy - y <= m_nRgnMax_Y )
			{
				if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
					g_Rows[cy - y][cx + x]	= Color_FG;
				if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
					g_Rows[cy - y][cx - x]	= Color_FG;	
			}
		}
		
		dx	= 3 * b * b;					//X每加1，
		dy	= 2 * a * a * (1 - b);
		dx_1= 2 * b * b;
		dy_1= 2 * a * a;
		dx1	= b * b;
		dy1 = a * -a;
		d2	= b * b;
		d3	= a * a * (b + b - 1) / 2; 
		d1	= d2 + a * a * (1 - 4 * b) / 4; 
		
		//畫上半圓弧
		while(d2 < d3)
		{
			if(d1 < 0)
			{
				d1	+= dx;
				dx	+= dx_1; 
				x	++;
			}
			else
			{
				d1	+= dx + dy;
				dx	+= dx_1;
				dy	+= dy_1;
				d3	+= dy1;
				x	++;
				y	--;
			}		
			d2	+= dx1;		
			
			if(g_nRotate == 90)
			{
				if(cy + y >= m_nRgnMin_Y && cy + y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
					{
						nCurX = cx + x;
						nCurY = cy + y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
					{
						nCurX = cx - x;
						nCurY = cy + y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				if(cy - y >= m_nRgnMin_Y && cy - y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
					{
						nCurX = cx + x;
						nCurY = cy - y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
					{
						nCurX = cx - x;
						nCurY = cy - y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;	
					}
				}
			}
			else
			{
				if(cy + y >= m_nRgnMin_Y && cy + y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
						g_Rows[cy + y][cx + x]	= Color_FG;
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
						g_Rows[cy + y][cx - x]	= Color_FG;	
				}
				if(cy - y >= m_nRgnMin_Y && cy - y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
						g_Rows[cy - y][cx + x]	= Color_FG;
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
						g_Rows[cy - y][cx - x]	= Color_FG;	
				}
			}
/*			
			g_Rows[cy + y][cx + x]	= Color_FG;
			g_Rows[cy + y][cx - x]	= Color_FG;
			g_Rows[cy - y][cx + x]	= Color_FG;
			g_Rows[cy - y][cx - x]	= Color_FG;	
*/	 
		}
		
		d1	= (int)(sqrt(b * (x + 0.5)) + sqrt(a * (y - 1)) - sqrt(a * b));
		dx	= 2 * b * b * (x + 1);
		dy	= a * a * (-2 * y + 3);
		while(y > 0)
		{
			if(d1 < 0)
			{
				d1	+= dx + dy;
				dx	+= dx_1;
				dy	+= dy_1;
				x	++;
				y	--;
			}
			else
			{
				d1	+= dy;
				dy	+= dy_1;
				y	--;
			}
			
			if(g_nRotate == 90)
			{
				if(cy + y >= m_nRgnMin_Y && cy + y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
					{
						nCurX = cx + x;
						nCurY = cy + y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
					{
						nCurX = cx - x;
						nCurY = cy + y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				if(cy - y >= m_nRgnMin_Y && cy - y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
					{
						nCurX = cx + x;
						nCurY = cy - y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
					{
						nCurX = cx - x;
						nCurY = cy - y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}

			}
			else
			{
				if(cy + y >= m_nRgnMin_Y && cy + y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
						g_Rows[cy + y][cx + x]	= Color_FG;
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
						g_Rows[cy + y][cx - x]	= Color_FG;	
				}
				if(cy - y >= m_nRgnMin_Y && cy - y <= m_nRgnMax_Y)
				{
					if(cx + x >= m_nRgnMin_X && cx + x <= m_nRgnMax_X)
						g_Rows[cy - y][cx + x]	= Color_FG;
					if(cx - x >= m_nRgnMin_X && cx - x <= m_nRgnMax_X)
						g_Rows[cy - y][cx - x]	= Color_FG;	
				}
			}
/*			
			g_Rows[cy + y][cx + x]	= Color_FG;
			g_Rows[cy + y][cx - x]	= Color_FG;
			g_Rows[cy - y][cx + x]	= Color_FG;
			g_Rows[cy - y][cx - x]	= Color_FG;
*/			
		}
		return;
	}

	dx	= 3 * b * b;					//X每加1，
	dy	= 2 * a * a * (1 - b);
	dx_1= 2 * b * b;
	dy_1= 2 * a * a;
	dx1	= b * b;
	dy1 = a * -a;
	d2	= b * b;
	d3	= a * a * (b + b - 1) / 2; 
	d1	= d2 + a * a * (1 - 4 * b) / 4; 
	
	//畫上半圓弧
	while(d2 < d3)
	{
		if(d1 < 0)
		{
			d1	+= dx;
			dx	+= dx_1; 
			x	++;
		}
		else
		{
			d1	+= dx + dy;
			dx	+= dx_1;
			dy	+= dy_1;
			d3	+= dy1;
			if(cx - x >= m_nRgnMin_X)
				Start_X	= cx - x;
			else
				Start_X	= m_nRgnMin_X;
			if(cx + x <= m_nRgnMax_X)
				End_X	= cx + x;
			else
				End_X	= m_nRgnMax_X;
			m_nCurrent_Y	= cy + y;
			
			if(g_nRotate == 90)
			{
				if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
				{
					for(i = Start_X; i <= End_X; i ++)
					{
						nCurX = i;
						nCurY = m_nCurrent_Y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
				m_nCurrent_Y	= cy - y;
				if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
				{
					for(i = Start_X; i <= End_X; i ++)
					{
						nCurX = i;
						nCurY = m_nCurrent_Y;
							
						nSetX = nCurY;
						nSetY = g_CurY - nCurX -1;
						g_Rows[nSetY][nSetX]	= Color_FG;
					}
				}
			}
			else
			{
				if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
					for(i = Start_X; i <= End_X; i ++)
						g_Rows[m_nCurrent_Y][i]	= Color_FG;
				m_nCurrent_Y	= cy - y;
				if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
					for(i = Start_X; i <= End_X; i ++)
						g_Rows[m_nCurrent_Y][i]	= Color_FG;
			}
/*					
			for(i= cx - x; i<= cx + x; i ++)
			{
				g_Rows[cy + y][i]	= Color_FG;
				g_Rows[cy - y][i]	= Color_FG;
			}
*/		
			x	++;
			y	--;
		}		
		d2	+= dx1;	
	}	
	if(cx - x >= m_nRgnMin_X)
		Start_X	= cx - x;
	else
		Start_X	= m_nRgnMin_X;
	if(cx + x <= m_nRgnMax_X)
		End_X	= cx + x;
	else
		End_X	= m_nRgnMax_X;
	m_nCurrent_Y	= cy + y;
	
	if(g_nRotate == 90)
	{
		if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
		{
			for(i = Start_X; i <= End_X; i ++)
			{
				nCurX = i;
				nCurY = m_nCurrent_Y;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
			}
		}
		m_nCurrent_Y	= cy - y;
		if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
		{
			for(i = Start_X; i <= End_X; i ++)
			{
				nCurX = i;
				nCurY = m_nCurrent_Y;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
			}
		}
	}
	else
	{
		if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
			for(i = Start_X; i <= End_X; i ++)
				g_Rows[m_nCurrent_Y][i]	= Color_FG;
		m_nCurrent_Y	= cy - y;
		if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
			for(i = Start_X; i <= End_X; i ++)
				g_Rows[m_nCurrent_Y][i]	= Color_FG;
	}
/*					
	for(i= cx - x; i<= cx + x; i ++)
	{
		g_Rows[cy + y][i]	= Color_FG;
		g_Rows[cy - y][i]	= Color_FG;
	}
*/	
	d1	= (int)(sqrt(b * (x + 0.5)) + sqrt(a * (y - 1)) - sqrt(a * b));
	dx	= 2 * b * b * (x + 1);
	dy	= a * a * (-2 * y + 3);
	while(y > 0)
	{
		if(d1 < 0)
		{
			d1	+= dx + dy;
			dx	+= dx_1;
			dy	+= dy_1;
			y	--;
			x	++;
		}
		else
		{
			d1	+= dy;
			dy	+= dy_1;
			y	--;
		}
		if(cx - x >= m_nRgnMin_X)
			Start_X	= cx - x;
		else
			Start_X	= m_nRgnMin_X;
		if(cx + x <= m_nRgnMax_X)
			End_X	= cx + x;
		else
			End_X	= m_nRgnMax_X;
			
		
		if(g_nRotate == 90)
		{
			m_nCurrent_Y	= cy + y;
			if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
			{
				for(i = Start_X; i <= End_X; i ++)
				{
					nCurX = i;
					nCurY = m_nCurrent_Y;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
			m_nCurrent_Y	= cy - y;
			if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
			{
				for(i = Start_X; i <= End_X; i ++)
				{
					nCurX = i;
					nCurY = m_nCurrent_Y;
						
					nSetX = nCurY;
					nSetY = g_CurY - nCurX -1;
					g_Rows[nSetY][nSetX]	= Color_FG;
				}
			}
		}
		else
		{
			m_nCurrent_Y	= cy + y;
			if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
				for(i = Start_X; i <= End_X; i ++)
					g_Rows[m_nCurrent_Y][i]	= Color_FG;
			m_nCurrent_Y	= cy - y;
			if(m_nCurrent_Y >= m_nRgnMin_Y && m_nCurrent_Y <= m_nRgnMax_Y)
				for(i = Start_X; i <= End_X; i ++)
					g_Rows[m_nCurrent_Y][i]	= Color_FG;
		}
/*
		for(i= cx - x; i<= cx + x; i ++)
		{
			g_Rows[cy + y][i]	= Color_FG;
			g_Rows[cy - y][i]	= Color_FG;
		}
*/		
	}
	return;
}

/*==========================================================================+
|	Bresenham直線算法														|
+==========================================================================*/
void	_Line(short x1, short y1, short x2, short y2)
{
	//	fans add 2009/7/10 09:15上午
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) return;
	
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	
	int dx, dy;
	int	x,	y;
	int	const1,	const2;
	int p, tmp, inc;
	
	dx	= x2 - x1;
	dy	= y2 - y1;
	
	//設置直線的最後一點。	
	m_nCurrent_X	= x2;
	m_nCurrent_Y	= y2;
	
	x	= x1;
	y	= y1;
	
	if(dx * dy > 0)
		inc	= 1;
	else
		inc	= -1;
	
	if(abs(dx) > abs(dy))
	{
		if(dx < 0)
		{
			/*將2a, 3a象限方向的直線變換到1a, 4a象限方向去*/
			tmp	= x1; 				
			x1	= x2;
			x2	= tmp;
			y	= y2;
		}
		dx	= abs(dx);
		dy	= abs(dy);
		
		p		= 2 * dy - dx;
		const1	= 2 * dy;
		const2	= 2 * (dy - dx);
		
		if(g_nRotate == 90)
		{
			for(x = x1; x <= x2; x++)
			{
	//			if(y >= 0 && x >= 0 && x < g_Length_X && y < g_Length_Y)
				nCurX = x;
				nCurY = y;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
				if(p < 0)
					p	+= const1;
				else
				{
					p	+= const2;
					y	+= inc;
				} 
			}
		}
		else
		{
			for(x = x1; x <= x2; x++)
			{
	//			if(y >= 0 && x >= 0 && x < g_Length_X && y < g_Length_Y)
				g_Rows[y][x]	= Color_FG;
				if(p < 0)
					p	+= const1;
				else
				{
					p	+= const2;
					y	+= inc;
				} 
			}
		}
	}
	else
	{
		if (dy < 0)
		{
			/* 將3b, 4b象限方向的直線變換到2b, 1b象限方向去. */
			tmp	= y1; 		
			y1	= y2; 			
			y2	= tmp;			
			x	= x2;
		}
		dx	= abs(dx);
		dy	= abs(dy);
		
		
		p		= 2 * dx - dy;
		const1	= 2 * dx;
		const2	= 2 * (dx - dy);

		if(g_nRotate == 90)
		{
			for(y = y1; y <= y2; y++)
			{
	//			if(y >= 0 && x >= 0 && x < g_Length_X && y < g_Length_Y)	
				nCurX = x;
				nCurY = y;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;		
				if(p < 0)
					p	+= const1;
				else
				{
					p	+= const2;
					x	+= inc;
				}
			}
		}
		else
		{
			for(y = y1; y <= y2; y++)
			{
	//			if(y >= 0 && x >= 0 && x < g_Length_X && y < g_Length_Y)				
					g_Rows[y][x]	= Color_FG;
				if(p < 0)
					p	+= const1;
				else
				{
					p	+= const2;
					x	+= inc;
				}
			}
		}
	}
}

int	_LineTo(short x, short y)
{
	x	= x + m_nOrg_X;
	y	= y + m_nOrg_Y;
	
	if(x == m_nCurrent_X)
		return _VLine(m_nCurrent_Y, y, m_nCurrent_X);
	if(y == m_nCurrent_Y)
		return _HLine(m_nCurrent_X, x, m_nCurrent_Y);
	else
	{
		if(_ClipLine(&m_nCurrent_X, &m_nCurrent_Y, &x, &y) != 0) return 1;
		_Line(m_nCurrent_X, m_nCurrent_Y, x, y);
	}

	return 0;
}

int _HLine(short x1, short x2, unsigned short y)
{
	int 	i;
	
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	
	//如果Y座標在ViewPort范圍之外，則退出
	if(y > m_nRgnMax_Y)
		return 0;
	if (x1 > x2)
	{
		i	= x1;
		x1	= x2;
		x2	= i;
		i	= 0;		//臨時性標志，０表示X1>X2，１則反之
	}
	else
		i	= 1;
	//哪果整條線都在ViewPort范圍之外，則退出	
	if(x2 < m_nRgnMin_X || x1 > m_nRgnMax_X)
		return 0;
	//如果線超出范圍，則裁剪成ViewPort范圍內的	
	if(x1 < m_nRgnMin_X)	x1	= m_nRgnMin_X;
	if(x2 > m_nRgnMax_X) 	x2	= m_nRgnMax_X;

	//設置線畫完後的當前點
	if(i == 1)
		m_nCurrent_X	= x2;
	else
		m_nCurrent_X	= x1;
	m_nCurrent_Y		= y;
	
	if(g_nRotate == 90)
	{
		switch(m_WriteMode)
		{
		case _GPSET:
			for (i = x1; i <= x2; i++)
			{
				nCurX = i;
				nCurY = m_nCurrent_Y;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
			}
			break;
		case _GXOR:
			for (i = x1; i <= x2; i++)
			{
				nCurX = i;
				nCurY = m_nCurrent_Y;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]   ^= Color_FG;
			}
			break;
		}
	}
	else
	{
		switch(m_WriteMode)
		{
		case _GPSET:
			for (i = x1; i <= x2; i++)
				g_Rows[m_nCurrent_Y][i]	= Color_FG;
			break;
		case _GXOR:
			for (i = x1; i <= x2; i++)
				g_Rows[m_nCurrent_Y][i]   ^= Color_FG;
			break;
		}
	}
	return 1;
}

void _MoveTo(short x, short y)
{
	m_nCurrent_X	= x + m_nOrg_X;
	m_nCurrent_Y	= y + m_nOrg_Y;
}


/*==========================================================================+
|  	Function	: _Rectangle                             					|
|	Parameter	: Control	_GFILLINTERIOR為填充							|
|							_GBORDER隻畫邊框								|
|				  x1,y1		矩形左上角坐標									|
|				  x2,y2		矩形右下角坐標									|
|	Return		: 0			沒有進行任何繪圖		                    	|
|				  非零		進行過繪圖										|
|	Remark		: 畫矩形或者填充矩形										|
+==========================================================================*/
int	_Rectangle(int Control, int x1, int y1, int x2, int y2)
{
	//printf("_Rectangle %d.%d  %d.%d \n", x1, y1, x2, y2);
	
	if(g_XYRange.flag == TRUE)
	{
		if(g_XYRange.x1 > x1)
			g_XYRange.x1 = x1;
		if(g_XYRange.y1 > y1)
			g_XYRange.y1 = y1;
		if(g_XYRange.x2 < x2)
			g_XYRange.x2 = x2;
		if(g_XYRange.y2 < y2)
			g_XYRange.y2 = y2;
	}
	else
	{
		g_XYRange.x1 = x1;
		g_XYRange.y1 = y1;
		g_XYRange.x2 = x2;
		g_XYRange.y2 = y2;
		g_XYRange.flag = TRUE;
	}
	int	i,	j, temp;
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	//畫矩形
	if(Control == _GBORDER)	
	{
		m_nCurrent_X	= x1;
		_VLine(y1, y2, m_nCurrent_X);	
		_HLine(x1, x2, y2);
		_VLine(y2, y1, x2);
		_HLine(x2, x1, y1);	
		return 1;
	}
	
	//填充矩形
	if(x1 > x2)
	{
		temp	= x1;
		x1		= x2;
		x2		= temp;
	}	
	if(y1 > y2)
	{
		temp	= y1;
		y1		= y2;
		y2		= y1;
	}
	
	//判斷屏幕坐標是否完全超出ViewPort范圍。
	if(x1 > m_nRgnMax_X || x2 < m_nRgnMin_X 
			|| y1 > m_nRgnMax_Y || y2 < m_nRgnMin_Y)
		return 0; 
	//判斷屏幕坐標是否超出范圍，並隻畫在ViewPort范圍的部分
	if(x1 < m_nRgnMin_X)	x1	= m_nRgnMin_X;
	if(y1 < m_nRgnMin_Y)	y1	= m_nRgnMin_Y;
	if(x2 > m_nRgnMax_X)	x2	= m_nRgnMax_X;
	if(y2 > m_nRgnMax_Y)	y2	= m_nRgnMax_Y;

	if(g_nRotate == 90)
	{
		for(i = y1; i <= y2; i ++)
		{
			for (j = x1; j <= x2; j ++)
			{
				nCurX = j;
				nCurY = i;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
			}
		}
	}
	else
	{
		for(i = y1; i <= y2; i ++)
		{
			for (j = x1; j <= x2; j ++)
				g_Rows[i][j]	= Color_FG;
		}
	}
	return 1;
}

/*==========================================================================+
|  	Function	: _SetColor                             					|
|	Parameter	: color	顏色值												|
|	Return		: -1	失敗				 		                    	|
|				  其它	原來的顏色值										|
|	Remark		: 設置當前畫筆顏色											|
+==========================================================================*/
int		_SetColor(unsigned short Color)
{
	short	OldColor;
	
	OldColor	= Color_FG;
	#ifdef	D_32BIT
	Color_FG	= RGB565TORGB888(Color);
	#else
	Color_FG	= Color;
	#endif
	return OldColor;	
}

/*==========================================================================+
|  	Function	: _SetWriteMode                            					|
|	Parameter	: action		- 新的畫點模式								|
|	Return		: 原來的畫點模式			 		                    	|
|	Remark		: 設置當前畫點模式											|
+==========================================================================*/
short	_SetWriteMode(short action)
{
	short	OldMode;
	
	OldMode		= m_WriteMode;
	m_WriteMode	= action;
	return OldMode;
}
/*==========================================================================+
|  	Function	: _SetViewPort                             					|
|	Parameter	: x1, y1	ViewPort左上角屏幕坐標							|
|				  x2, y2	ViewPort右下角屏幕坐標							|
|	Return		: 0		設置ViewProt成功	 		                    	|
|				  其它	錯誤代碼											|
|	Remark		: 設置ViewPort												|
+==========================================================================*/
int	_SetViewPort(int x1, int y1, int x2, int y2)
{
	int		i, temp;
	int		Offset;
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	int	nSetX1 = 0, nSetY1 = 0, nSetX2 = 0, nSetY2 = 0;
	if(x1 > x2)
	{
		temp	= x1;
		x1		= x2;
		x2		= temp;
	}	
	if(y1 > y2)
	{
		temp	= y1;
		y1		= y2;
		y2		= y1;
	}
	
	//printf("_SetViewPort %d.%d %d.%d \n", x1, y1, x2, y2);
	if(g_XYRange.flag == TRUE)
	{
		if(g_XYRange.x1 > x1)
			g_XYRange.x1 = x1;
		if(g_XYRange.y1 > y1)
			g_XYRange.y1 = y1;
		if(g_XYRange.x2 < x2)
			g_XYRange.x2 = x2;
		if(g_XYRange.y2 < y2)
			g_XYRange.y2 = y2;
	}
	else
	{
		g_XYRange.x1 = x1;
		g_XYRange.y1 = y1;
		g_XYRange.x2 = x2;
		g_XYRange.y2 = y2;
		g_XYRange.flag = TRUE;
	}
	
	//判斷屏幕坐標是否超出范圍,如果超出了，直接返回。
	if(x1 < 0 || x2 >= g_infoDrv.nX || y1 < 0 || y2 >= g_infoDrv.nY)
		;//return 1;
	
	g_Length_X	= x2 - x1 + 1;
	g_Length_Y	= y2 - y1 + 1;

	Offset	= y1 * g_infoDrv.nX + x1;
	for(i = 0; i < g_Length_Y; i++)
	{
		g_Rows[i] = pMem + Offset;
		Offset	 +=g_infoDrv.nX;
	}
	
	
	m_nView_X		= x1;
	m_nView_Y		= y1;
	
	_SetClipRgn(x1, y1, x2, y2);
	
	m_nOrg_X		= 0;
	m_nOrg_Y		= 0;
	
	m_nCurrent_X	= 0;
	m_nCurrent_Y	= 0;
	return 0;
}

void	_SetClipRgn(short x1, short y1, short x2, short y2)
{
	if(x1 > x2)	return;	
	if(y1 > y2) return;
	
	//判斷屏幕坐標是否超出范圍,如果超出了，直接返回。
	if(x1 < m_nView_X || x2 >= m_nView_X + g_Length_X  
			|| y1 < m_nView_Y || y2 >= m_nView_Y + g_Length_Y)
		return;
	
	m_nRgnMin_X		= x1 - m_nView_X;
	m_nRgnMin_Y		= y1 - m_nView_Y;
	
	m_nRgnMax_X		= x2 - m_nView_X;
	m_nRgnMax_Y		= y2 - m_nView_Y;
}

int _VLine(short y1, short y2, unsigned short x)
{
	int	i;
	int	nCurX = 0, nCurY = 0;
	int	nSetX = 0, nSetY = 0;
	//如果x座標在ViewPort范圍之外，則退出
	if(x > m_nRgnMax_X)
		return 0;
	if (y1 > y2)
	{
		i	= y1;
		y1	= y2;
		y2	= i;
		i	= 0;		//臨時性標志，０表示X1>X2，１則反之
	}
	else
		i	= 1;
	//哪果整條線都在ViewPort范圍之外，則退出	
	if(y2 < m_nRgnMin_Y || y1 > m_nRgnMax_Y)
		return 0;
	//如果線超出范圍，則裁剪成ViewPort范圍內的	
	if(y1 < m_nRgnMin_Y)	y1	= m_nRgnMin_Y;
	if(y2 > m_nRgnMax_Y) 	y2	= m_nRgnMax_Y;

	//設置線畫完後的當前點
	m_nCurrent_X		= x;
	if(i == 1)
		m_nCurrent_Y	= y2;
	else
		m_nCurrent_Y	= y1;	
	
	//=======
	// 根據當前writemode畫線
	//=======
	if(g_nRotate == 90)
	{
		switch(m_WriteMode)
		{
		case _GPSET:
			for(i = y1; i <= y2; i++)
			{
				nCurX = m_nCurrent_X;
				nCurY = i;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]	= Color_FG;
			}
			break;
		case _GXOR:
			for(i = y1; i <= y2; i++)
			{
				nCurX = m_nCurrent_X;
				nCurY = i;
					
				nSetX = nCurY;
				nSetY = g_CurY - nCurX -1;
				g_Rows[nSetY][nSetX]   ^= Color_FG;
			}
			break;
		}
	}
	else
	{
		switch(m_WriteMode)
		{
		case _GPSET:
			for(i = y1; i <= y2; i++)
				g_Rows[i][m_nCurrent_X]	= Color_FG;
			break;
		case _GXOR:
			for(i = y1; i <= y2; i++)
				g_Rows[i][m_nCurrent_X]   ^= Color_FG;
			break;
		}
	}
	return 1;
}

long	_ImageSize(int x1, int y1, int x2, int	y2)
{
	long Size;
	
	Size	= ((abs(x2 - x1) + 1) * (abs(y2 - y1) + 1) * g_infoDrv.nBits >> 3) + 
				sizeof(tmIMAGE);
	
	return Size;
}

void	_GetImage(int x1, int y1, int x2, int y2, char* image)
{
	tmIMAGE*	pimage;
	int			i, j;
	
	pimage		= (tmIMAGE*)image;
	
	//=========
	// 整理坐標，使第一點永遠在第二點左上角
	//=========
	if(x1 > x2)
	{
		i	= x2;
		x2	= x1;
		x1	= i;
	}	
	if(y1 > y2)
	{
		i	= y2;
		y2	= y1;
		y1	= i;
	}
	
	//=======
	// 處理坐標，使其不超過viewport
	//=======
	if(x1 >= g_Length_X || y1 >= g_Length_Y)
		return;
	
	if(x1 < 0)			x1 = 0;
	if(y1 < 0)			y1 = 0;
	
	if(x2 >= g_Length_X)	x2 	= g_Length_X - 1;
	if(y2 >= g_Length_Y)	y2	= g_Length_Y - 1;
	
	//=======
	//  設置圖片信息
	//=======
	pimage->wWidth		= x2 - x1 + 1;
	pimage->wHeight		= y2 - y1 + 1;
	pimage->wColorBit	= g_infoDrv.nBits;
	pimage->wWidthPixel	= 0;
	pimage->wHeightPixel= 0;
	
	
	//=======
	// 從屏幕讀取畫面
	//=======
	switch(g_infoDrv.nBits)
	{
	case 8:
		break;
	case 16:
		{
			unsigned short	*pWord;
			
			pWord	= (unsigned short *)&pimage->acMAP[0];
	/*		
			for(i = m_nView_Y + y1; i <= m_nView_Y + y2; i++)
			{
				for(j = m_nView_X + x1; j <= m_nView_X + x2; i ++)
				{
					*pWord		= *(pMem + i * g_infoDrv.nX + j);
					pWord	++;
				}
				
			}
	*/
			for(i = y1; i <= y2; i ++)
			{
				for(j = x1; j <= x2; j ++)			
				{
					*pWord		= g_Rows[i][j];
					pWord	++;
				}
			}
		}
		break;
	case 24:
		break;	
	case 32:
		{
			DWORD	*pWord;
		
			pWord	= (DWORD*)&pimage->acMAP[0];
	
			for(i = y1; i <= y2; i ++)
			{
				for(j = x1; j <= x2; j ++)			
				{
					*pWord		= g_Rows[i][j];
					pWord	++;
				}
			}
		}
		break;
	}
	
	return;
}

/*Created by J.Wong 2017/1/15 9:51:58*/

void 	_CombineImage(int x, int y, CImage& dst, const CImage& src)
{
	if(dst.isNull()||src.isNull())	return ;
	CRect dstRect=CRect(CPoint(0,0),dst.size());
	CRect srcRect=CRect(CPoint(x, y),src.size());
	dstRect&=srcRect;
	if(dstRect.isValid())
	{
		for(int i=0;i<dstRect.height();++i)
		{
			uint32* dstLine=reinterpret_cast<uint32*>(dst.scanLine(dstRect.top()+i)+dstRect.left());
			const uint32* srcLine=reinterpret_cast<const uint32*>(src.constScanLine(i));
			for(int j=0; j<dstRect.width();++j)
			{
				uint8 BR=(dstLine[j]&0xFF0000)>>16;
           		uint8 BG=(dstLine[j]&0xFF00)>>8;
           		uint8 BB=(dstLine[j]&0xFF);
				
				uint8 FA=(srcLine[j]&0xFF000000)>>24;
				uint8 FR=(srcLine[j]&0xFF0000)>>16;
           		uint8 FG=(srcLine[j]&0xFF00)>>8;
           		uint8 FB=(srcLine[j]&0xFF);
           		
       //    	float scale=FA/255.0;
       //		float rscale=1-scale;
       			uint8 scale=FA;
       			uint8 rscale=0xFF-scale;
				dstLine[j]=RGB2PIXEL888(uint8((BR*rscale+FR*scale)>>8),
                   									uint8((BG*rscale+FG*scale)>>8),
                  									uint8((BB*rscale+FB*scale)>>8));	
				//printf("dstLine[%d][%d]=%08x\n",i, j, dstLine[j]);
			}	
		}	
	}
}

void	_GetImage(int x, int y, CImage& img)
{
	int xMin=mMax(x,m_nRgnMin_X);
	int yMin=mMax(y, m_nRgnMin_Y);
	int xMax=mMin(img.width(),m_nRgnMax_X);
	int yMax=mMin(img.height(),m_nRgnMax_Y);
//	printf("width=%d,height=%d\n",img.width(),img.height());
//	printf("xMax=%d,yMax=%d\n",xMax,yMax);
	for(int i=yMin; i<yMax; i++)
	{ 
		memcpy(img.scanLine(i)+xMin*4,g_Rows[i]+xMin*4,xMax*4);
	}	
}
void	_PutImage(int x, int y, const CImage& img, bool alphaMask)
{
	int xMin=mMax(x,m_nRgnMin_X);
	int yMin=mMax(y, m_nRgnMin_Y);
	int xMax=mMin(img.width(),m_nRgnMax_X);
	int yMax=mMin(img.height(),m_nRgnMax_Y);
	if(alphaMask==false)
	{
		for(int i=yMin; i<yMax; i++)
		{ 
			memcpy(g_Rows[i]+xMin*4,img.constScanLine(i)+xMin*4,xMax*4);
		}
		
	}
	//扡摯蚚誧怓睿囀瞄怓杅擂腔け楛蝠誑準都瘧奀,絞ヶ茞璃殤凳狟祥膘祜妏蚚
	else
	{
		for(int i=yMin; i<yMax; i++)
		{ 
			const uint32* line=reinterpret_cast<const uint32*>(img.constScanLine(i)+xMin*4);
			for(int j=xMin;j<xMax;j++)
			{
				uint8 BR=(g_Rows[i][j]&0xFF0000)>>16;
           		uint8 BG=(g_Rows[i][j]&0xFF00)>>8;
           		uint8 BB=(g_Rows[i][j]&0xFF);
				
				uint8 FA=(line[j]&0xFF000000)>>24;
				uint8 FR=(line[j]&0xFF0000)>>16;
           		uint8 FG=(line[j]&0xFF00)>>8;
           		uint8 FB=(line[j]&0xFF);
           		
           		float scale=FA/255.0;
				g_Rows[i][j]=RGB2PIXEL888(uint8((BR*(1-scale)+(FR*scale)+0.5f)),
                   									uint8((BG*(1-scale)+(FG*scale)+0.5f)),
                  									uint8((BB*(1-scale)+(FB*scale)+0.5f)));
			
			}
				
		}
	}	
}


void	_PutImage(int x, int y, const char* image, short action)
{
	int offsetx1 = 0, offsety1 = 0, offsetx2 = 0, offsety2 = 0;
	int	i, j;
	int	x1, y1, x2 ,y2;
	tmIMAGE*	pimage;
	
	//=======
	// 判斷圖片是否超過viewport座標
	//=======
	if(x >= g_Length_X || y >= g_Length_Y)	return;
	
	pimage	= (tmIMAGE*)image;
	
	x1	= x;
	y1	= y;
	x2	= x + pimage->wWidth 	- 1;
	y2	= y + pimage->wHeight 	- 1;
	
	if(x < 0)
	{
		offsetx1	= -x;
		x			= 0;
	}
	if(y < 0)
	{
		offsety1	= -y;
		y			= 0;
	}
	if(x2 >= g_Length_X)
	{
		offsetx2	=  x2 - g_Length_X - 1;
		x2			= g_Length_X - 1;
	}
	if(y2 >= g_Length_Y)
	{
		offsety2	= y2 - g_Length_Y - 1;
		x2			= g_Length_Y -1;
	}
	
	switch(pimage->wColorBit)
	{
	case 8:
		break;
	case 16:
		unsigned short*	pWord;
		
		pWord	= (unsigned short*)&pimage->acMAP[0];
		pWord  += pimage->wWidth * offsety1;
		for(i = y1; i <= y2; i ++)
		{
			pWord	+= offsetx1;
			for(j = x1; j <= x2; j++)
			{
				g_Rows[i][j]	= *pWord;
				pWord ++;
			}
			pWord	+=	offsetx2;
		}
		break;
	case 24:
		break;
	case 32:
	{
		unsigned long*	pWord;
		
		pWord	= (unsigned long*)&pimage->acMAP[0];
		pWord  += pimage->wWidth * offsety1;
		for(i = y1; i <= y2; i ++)
		{
			pWord	+= offsetx1;
			for(j = x1; j <= x2; j++)
			{
				g_Rows[i][j]	= *pWord;
				pWord ++;
			}
			pWord	+=	offsetx2;
		}
		break;
	}
	}
	return;
}

void	_SetViewOrg(short x, short y)
{
	m_nOrg_X		= x - m_nView_X;
	m_nOrg_Y		= y - m_nView_Y;
}

void	_SetLineStyle(unsigned short nMask)
{
}

unsigned short	_GetLineStyle()
{
	return 0;
}


/*==========================================================================+
|	Cohen-Sutherland Line Clipping算法										|
|---------------------------------------------------------------------------|
| 這種算法和Bresenham直線算法結合，在有剪切和沒剪切下，會產生微小差異		|
+==========================================================================*/
int		_ClipLine(short *px1, short *py1, short *px2, short *py2)
{
	unsigned short	nC1 = 0, nC2 = 0;		// 線起點座標相對於viewport狀態
	unsigned short nOldC1 = 0;
	
	//=======
	// 判斷線的兩個端點相對於viewport的狀態
	//=======
	if(*px1 < m_nRgnMin_X)			// 如果在矩形的左邊
		nC1	|= 0x01;
	else if(*px1 > m_nRgnMax_X)		// 如果在矩形的右邊
		nC1 |= 0x02;
	
	if(*py1 < m_nRgnMin_Y)			// 如果在矩形的下面
		nC1 |= 0x04;
	else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的上面
		nC1	|= 0x08;
		
	if(*px2 < m_nRgnMin_X)			// 如果在矩形的左邊
		nC2	|= 0x01;
	else if(*px2 > m_nRgnMax_X)		// 如果在矩形的右邊
		nC2 |= 0x02;
	
	if(*py2 < m_nRgnMin_Y)			// 如果在矩形的下面
		nC2 |= 0x04;
	else if(*py2 > m_nRgnMax_Y)		// 如果在矩形的上面
		nC2	|= 0x08;
		
	if((nC1 & nC2) != 0)			return 1;	// 和viewport沒有交點
	if(nC1 == 0 && nC2 == 0)	return 0;	// 線段都在viewprot內
	
	while(nC1 != 0 || nC2 != 0)
	{
		//=======
		// 計算出線段和矩形左右邊所在的直線的交點
		//=======
		if(nC1 != 0)
		{
			if(*px1	< m_nRgnMin_X)
			{
				*py1	= (m_nRgnMin_X - *px1) * (*py2 - *py1) / 
							(*px2 - *px1) + *py1;
				*px1	= m_nRgnMin_X;
				
				nC1		= 0x00;
				if(*py1 < m_nRgnMin_Y)			// 如果在矩形的下面
					nC1 |= 0x04;
				else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的上面
					nC1	|= 0x08;
				nOldC1 ++;
			}
			else if(*px1 > m_nRgnMax_X)
			{
				*py1	= (m_nRgnMax_X - *px1) * (*py2 - *py1) /	
							(*px2 - *px1) + *py1;
				*px1	= m_nRgnMax_X;
				
				nC1		= 0x00;
				if(*py1 < m_nRgnMin_Y)			// 如果在矩形的下面
					nC1 |= 0x04;
				else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的上面
					nC1	|= 0x08;
				
				nOldC1 ++;
			}
			if((nC1 & nC2) != 0)			return 1;
			if(nC1 == 0)	continue;
				
			//=======
			// 計算出線段和矩形上下邊所在的直線的交點
			//=======	
			if(*py1	< m_nRgnMin_Y)
			{
				*px1	= (m_nRgnMin_Y - *py1) * (*px2 - *px1) / 
							(*py2 - *py1) + *px1;
				*py1	= m_nRgnMin_Y;
				
				nC1		= 0x00;
				if(*px1 < m_nRgnMin_X)			// 如果在矩形的左面
					nC1 |= 0x01;
				else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的右邊面
					nC1	|= 0x02;
		nOldC1 ++;
			}
			else if(*py1 > m_nRgnMax_Y)
			{
				*px1	= (m_nRgnMax_Y - *py1) * (*px2 - *px1) / 
							(*py2 - *py1) + *px1;
				*py1	= m_nRgnMax_Y;
				nC1		= 0x00;
				if(*px1 < m_nRgnMin_X)			// 如果在矩形的左面
					nC1 |= 0x01;
				else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的右邊面
					nC1	|= 0x02;
				nOldC1 ++;
			}
		}
		
		if(nC2 != 0)
		{
			if(*px2 < m_nRgnMin_X)
			{
				*py2	= (m_nRgnMin_X - *px2) * (*py1 - *py2) / 
							(*px1 - *px2) + *py2;
				*px2	= m_nRgnMin_X;
				
				nC2		= 0x00;
				if(*py2 < m_nRgnMin_Y)			// 如果在矩形的下面
					nC2 |= 0x04;
				else if(*py2 > m_nRgnMax_Y)		// 如果在矩形的上面
					nC2	|= 0x08;
				nOldC1 ++;
			}
			else if(*px2 > m_nRgnMax_X)
			{
				*py2	= (m_nRgnMax_X - *px2) * (*py1 - *py2) / 
							(*px1 - *px2) + *py2;
				*px2	= m_nRgnMax_X;
				
				nC2		= 0x00;
				if(*py2 < m_nRgnMin_Y)			// 如果在矩形的下面
					nC2 |= 0x04;
				else if(*py2 > m_nRgnMax_Y)		// 如果在矩形的上面
					nC2	|= 0x08;
				nOldC1 ++;
			}
			if(nC2 == 0)	continue;
				
			//=======
			// 計算出線段和矩形上下邊所在的直線的交點
			//=======
			if(*py2	< m_nRgnMin_Y)
			{
				*px2	= (m_nRgnMin_Y - *py2) * (*px1 - *px2) / 
							(*py1 - *py2) + *px2;
				*py2	= m_nRgnMin_Y;
				nC2		= 0x00;
				if(*px1 < m_nRgnMin_X)			// 如果在矩形的左面
					nC2 |= 0x01;
				else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的右邊面
					nC2	|= 0x02;
		nOldC1 ++;
			}
			else if(*py2 > m_nRgnMax_Y)
			{
				*px2	= (m_nRgnMax_Y - *py2) * (*px1 - *px2) / 
							(*py1 - *py2) + *px2;
				*py2	= m_nRgnMax_Y;
				nC2		= 0x00;
				if(*px1 < m_nRgnMin_X)			// 如果在矩形的左面
					nC2 |= 0x01;
				else if(*py1 > m_nRgnMax_Y)		// 如果在矩形的右邊面
					nC2	|= 0x02;
				nOldC1 ++;
			}
		}
	}
	
//<<yang 2005/12/3 	////printf("Count=%d\n", nOldC1);
	return 0;
}

void	_ColorToRGB(WORD  wColor, BYTE* pOutR, BYTE* pOutG, BYTE* pOutB)
{
	BYTE r, g, b = 0;
	r	= (wColor >> 8) & 0xF8;
	g	= (wColor >> 3) & 0xFC;
	b	= (wColor << 3) & 0xF8;
	
	if (pOutR != NULL) *pOutR = r;
	if (pOutG != NULL) *pOutG = g;
	if (pOutB != NULL) *pOutB = b;
}

void	_ColorToRGB(DWORD  dwColor, BYTE* pOutR, BYTE* pOutG, BYTE* pOutB)
{
	BYTE r, g, b = 0;
	
	r	= (dwColor >> 16) & 0xFF;
	g	= (dwColor >> 8) & 0xFF;
	b	=  dwColor & 0xFF;
	
	if (pOutR != NULL) *pOutR = r;
	if (pOutG != NULL) *pOutG = g;
	if (pOutB != NULL) *pOutB = b;
}

void	_Fill(DWORD wFillType, WORD wBordeColor, WORD wFillColor, int x1, int y1,  
				WORD wDestColor, int nChange)
{
	
	#ifdef	D_32BIT
	DWORD BordeColor=0;
	DWORD FillColor =0;
	DWORD DestColor =0;
	
	BordeColor	=	RGB565TORGB888(wBordeColor);
	FillColor	=	RGB565TORGB888(wFillColor);
	DestColor	=  	RGB565TORGB888(wDestColor);
	#else
	WORD BordeColor=0;
	WORD FillColor =0;
	WORD DestColor =0;
	
	BordeColor	=	wBordeColor;
	FillColor	=	wFillColor;
	DestColor	=  	wDestColor;
	#endif
	
	
	if(x1 > m_nRgnMax_X || x1 < m_nRgnMin_X 
			|| y1 > m_nRgnMax_Y || y1 < m_nRgnMin_Y)
		return ; 
	if(x1 < m_nRgnMin_X)	x1	= m_nRgnMin_X;
	if(y1 < m_nRgnMin_Y)	y1	= m_nRgnMin_Y;
		
		
	WORD	wTemp = 0;
	BYTE	r, g, b, r1, g1, b1, rTemp, gTemp, bTemp = 0;
	_ColorToRGB(wFillColor, &r, &g, &b);	
	_ColorToRGB(wDestColor, &r1, &g1, &b1);
	
	
	if (wFillType & _COLOR_XCHANGE)		//	the color changes with the x
	{
		if (nChange == -1 ||
			(!(_COLOR_INDE & wFillType) && !(_COLOR_DEIN & wFillType)) )	
			nChange = m_nRgnMax_X;
		
		int	nRange = nChange - m_nRgnMin_X + 1;
		
		//printf("fill xmax=%d, xmin=%d, nRange=%d\n", m_nRgnMax_X, m_nRgnMin_X, nRange);
		for (int i = m_nRgnMin_X, ii = i - m_nRgnMin_X + 1; i <= x1 ; i++, ii++)
		{
			if (_COLOR_INCRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_DECRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_INDE & wFillType)
			{
				if (i <= nChange)
				{
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#endif
					//printf("1 increse color[%d]=%d\n", i, wFillColor);										
				}
				else
				{
					nRange = m_nRgnMax_X - nChange + 1;
					
					//ii = nRange - (m_nRgnMax_Y - i);
						//m_nRgnMax_Y - nChange + 1 - (m_nRgnMax_Y - i);
					ii = i + 1 - nChange;
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#endif
					//printf("1 decrese color[%d]=%d\n", i, wFillColor);
				}
			}
			else if (_COLOR_DEIN & wFillType)
			{
				
			}
			
			
			for (int j = y1; j >= m_nRgnMin_Y; j--)
			{
				if (g_Rows[j][i] == BordeColor)
					break;
				else
					g_Rows[j][i] = FillColor;
			}
			
			for (int j = y1 + 1; j <= m_nRgnMax_Y; j++)
			{
				if (g_Rows[j][i] == BordeColor)
					break;
				else
					g_Rows[j][i] = FillColor;
			}
		}
		
		for (int i = x1 + 1, ii = i - m_nRgnMin_X + 1; i <= m_nRgnMax_X; i++, ii++)
		{
			if (_COLOR_INCRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#endif
			else if (_COLOR_DECRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_INDE & wFillType)
			{
				if (i <= nChange)
				{
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#endif
					//printf("2 increse color[%d]=%d\n", i, wFillColor);										
				}
				else
				{
					//ii = nRange - (m_nRgnMax_Y - i);
					ii = i + 1 - nChange;	
					
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#endif
					//printf("2 decrese color[%d]=%d\n", i, wFillColor);
					
				}
				
			}
			else if (_COLOR_DEIN & wFillType)
			{
				
			}
			
			for (int j = y1; j >= m_nRgnMin_Y; j--)
			{
				if (g_Rows[j][i] == BordeColor)
					break;
				else
					g_Rows[j][i] = FillColor;
			}
			
			for (int j = y1 + 1; j <= m_nRgnMax_Y; j++)
			{
				if (g_Rows[j][i] == BordeColor)
					break;
				else
					g_Rows[j][i] = FillColor;
			}		
		}
		
	}
	else								//	the color changes with the y
	{
		if (nChange == -1 || 
			(!(_COLOR_INDE & wFillType) && !(_COLOR_DEIN & wFillType)) )	
			nChange = m_nRgnMax_Y;
		
		int		nRange =  nChange - m_nRgnMin_Y + 1;
		for (int i = m_nRgnMin_Y, ii = i - m_nRgnMin_Y + 1; i <= y1 ; i++, ii++)
		{
			if (_COLOR_INCRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_DECRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_INDE & wFillType)
			{
				if (i <= nChange)
				{
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#endif
					////printf("1 increse color[%d]=%d\n", i, wFillColor);										
				}
				else
				{
					nRange = m_nRgnMax_Y - nChange + 1;
					
					//ii = nRange - (m_nRgnMax_Y - i);
						//m_nRgnMax_Y - nChange + 1 - (m_nRgnMax_Y - i);
					ii = i + 1 - nChange;
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#endif
					////printf("1 decrese color[%d]=%d\n", i, wFillColor);
				}
			}
			else if (_COLOR_DEIN & wFillType)
			{
				
			}
			
			
			
			for (int j = x1; j >= m_nRgnMin_X; j--)
			{
				if (g_Rows[i][j] == BordeColor)
					break;
				else
					g_Rows[i][j] = FillColor;
			}
			
			for (int j = x1 + 1; j <= m_nRgnMax_X; j++)
			{
				if (g_Rows[i][j] == BordeColor)
					break;
				else
					g_Rows[i][j] = FillColor;
			}
		}
		
		
		for (int i = y1 + 1, ii = i - m_nRgnMin_Y + 1; i <= m_nRgnMax_Y; i++, ii++)
		{
			if (_COLOR_INCRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#endif
			else if (_COLOR_DECRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_INDE & wFillType)
			{
				if (i <= nChange)
				{
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#endif
					////printf("2 increse color[%d]=%d\n", i, wFillColor);										
				}
				else
				{
					//ii = nRange - (m_nRgnMax_Y - i);
					ii = i + 1 - nChange;	
					
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#endif
					////printf("2 decrese color[%d]=%d\n", i, wFillColor);
					
				}
				
			}
			else if (_COLOR_DEIN & wFillType)
			{
				
			}
			
			for (int j = x1; j >= m_nRgnMin_X; j--)
			{
				if (g_Rows[i][j] == BordeColor)
					break;
				else
					g_Rows[i][j] = FillColor;
			}
			
			for (int j = x1 + 1; j <= m_nRgnMax_X; j++)
			{
				if (g_Rows[i][j] == BordeColor)
					break;
				else
					g_Rows[i][j] = FillColor;
			}		
		}
	}
}

void	_FillRect(DWORD wFillType, WORD wBordeColor, WORD wFillColor, 
			  int x1, int y1, int x2, int y2, 
			  WORD wDestColor, int nChange)
{
	#ifdef	D_32BIT
	DWORD BordeColor=0;
	DWORD FillColor =0;
	DWORD DestColor =0;
	
	BordeColor	=	RGB565TORGB888(wBordeColor);
	FillColor	=	RGB565TORGB888(wFillColor);
	DestColor	=  	RGB565TORGB888(wDestColor);
	#else
	WORD BordeColor=0;
	WORD FillColor =0;
	WORD DestColor =0;
	
	BordeColor	=	wBordeColor;
	FillColor	=	wFillColor;
	DestColor	=  	wDestColor;
	#endif
	
	int temp = 0;
	//填充矩形
	if(x1 > x2)
	{
		temp	= x1;
		x1		= x2;
		x2		= temp;
	}	
	if(y1 > y2)
	{
		temp	= y1;
		y1		= y2;
		y2		= y1;
	}
	
	//判斷屏幕坐標是否完全超出ViewPort范圍。
	if(x1 > m_nRgnMax_X || x2 < m_nRgnMin_X 
			|| y1 > m_nRgnMax_Y || y2 < m_nRgnMin_Y)
		return ; 
	//判斷屏幕坐標是否超出范圍，並隻畫在ViewPort范圍的部分
	if(x1 < m_nRgnMin_X)	x1	= m_nRgnMin_X;
	if(y1 < m_nRgnMin_Y)	y1	= m_nRgnMin_Y;
	if(x2 > m_nRgnMax_X)	x2	= m_nRgnMax_X;
	if(y2 > m_nRgnMax_Y)	y2	= m_nRgnMax_Y;

		
		
	WORD	wTemp = 0;
	BYTE	r, g, b, r1, g1, b1, rTemp, gTemp, bTemp = 0;
	_ColorToRGB(wFillColor, &r, &g, &b);	
	_ColorToRGB(wDestColor, &r1, &g1, &b1);
	
	
	if (wFillType & _COLOR_XCHANGE)		//	the color changes with the x
	{
		if (nChange == -1 ||
			(!(_COLOR_INDE & wFillType) && !(_COLOR_DEIN & wFillType)) )	
			nChange = x2;
		
		int	nRange = nChange - x1 + 1;
		
		
		
		for (int i = x1, ii = i - x1 + 1; i <= x2; i++, ii++)
		{
			if (_COLOR_INCRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_DECRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_INDE & wFillType)
			{
				if (i <= nChange)
				{
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#endif
					//printf("2 increse color[%d]=%d\n", i, wFillColor);										
				}
				else
				{
					//ii = nRange - (y2 - i);
					ii = i + 1 - nChange;	
					
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r - (r1 - r) * ii / nRange, 
											g - (g1 - g) * ii / nRange, 
											b - (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r - (r1 - r) * ii / nRange, 
											g - (g1 - g) * ii / nRange, 
											b - (b1 - b) * ii / nRange);
					#endif
					//printf("2 decrese color[%d]=%d\n", i, wFillColor);
					
				}
				
			}
			else if (_COLOR_DEIN & wFillType)
			{
				
			}
			
			for (int j = y1; j >= y1; j--)
			{
				g_Rows[j][i] = FillColor;
			}
			
			for (int j = y1 + 1; j <= y2; j++)
			{
				g_Rows[j][i] = FillColor;
			}		
		}
		
	}
	else								//	the color changes with the y
	{
		if (nChange == -1 || 
			(!(_COLOR_INDE & wFillType) && !(_COLOR_DEIN & wFillType)) )	
			nChange = y2;
		
		int		nRange =  nChange - y1 + 1;
		
		
		
		for (int i = y1, ii = i - y1 + 1; i <= y2; i++, ii++)
		{
			if (_COLOR_INCRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
										g + (g1 - g) * ii / nRange, 
										b + (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_DECRESE & wFillType)
				#ifdef	D_32BIT
				FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#else
				FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
				#endif										
			else if (_COLOR_INDE & wFillType)
			{
				if (i <= nChange)
				{
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r + (r1 - r) * ii / nRange, 
											g + (g1 - g) * ii / nRange, 
											b + (b1 - b) * ii / nRange);
					#endif
					////printf("2 increse color[%d]=%d\n", i, wFillColor);										
				}
				else
				{
					//ii = nRange - (y2 - i);
					ii = i + 1 - nChange;	
					
					#ifdef	D_32BIT
					FillColor = RGB2PIXEL888(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#else
					FillColor = RGB2PIXEL(	r1 - (r1 - r) * ii / nRange, 
										g1 - (g1 - g) * ii / nRange, 
										b1 - (b1 - b) * ii / nRange);
					#endif
					////printf("2 decrese color[%d]=%d\n", i, wFillColor);
					
				}
				
			}
			else if (_COLOR_DEIN & wFillType)
			{
				
			}
			
			for (int j = x1; j >= x1; j--)
			{
				g_Rows[i][j] = FillColor;
			}
			
			for (int j = x1 + 1; j <= x2; j++)
			{
				g_Rows[i][j] = FillColor;
			}		
		}
	}
}


void	_ArcRect(WORD wColor, int x1, int y1, int x2, int y2)
{
	int temp;
	if(x1 > x2)
	{
		temp	= x1;
		x1		= x2;
		x2		= temp;
	}	
	if(y1 > y2)
	{
		temp	= y1;
		y1		= y2;
		y2		= y1;
	}
	
	if(x1 > m_nRgnMax_X || x2 < m_nRgnMin_X || y1 > m_nRgnMax_Y || y2 < m_nRgnMin_Y)
		return ; 
	if(x1 < m_nRgnMin_X)	x1	= m_nRgnMin_X;
	if(y1 < m_nRgnMin_Y)	y1	= m_nRgnMin_Y;
	if(x2 > m_nRgnMax_X)	x2	= m_nRgnMax_X;
	if(y2 > m_nRgnMax_Y)	y2	= m_nRgnMax_Y;
		
	//void	_Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	
	
	if (y2 - y1 < 30)
	{
		int	arc_x1, arc_y1, arc_x2, arc_y2, arc_x3, arc_y3, arc_x4, arc_y4;
		
		arc_x1 = x1;
		arc_y1 = y1 + 1;
		arc_x2 = arc_x1 + 10;
		arc_y2 = arc_y1 + 6;
		
		arc_x3 = arc_x1 + 3;
		arc_y3 = y1;
		arc_x4 = arc_x1;
		arc_y4 = y1 + 3;
		
		//_Arc(arc_x1, arc_y1, arc_x2, arc_y2, arc_x3, arc_y3, arc_x4, arc_y4);
		//_Ellipse(_GBORDER, arc_x1, arc_y1, arc_x2, arc_y2);
		//	left top
		
		//	right top
		
		//	left bottom
		
		//	right bottom
	}
	else if (y2 - y1 < 50)
	{
	}
	else
	{
		
	}
}

