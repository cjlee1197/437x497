/*==========================================================================+
|  Class    :                                                  				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : ���t��                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: ���l��Framebuffer�C                                           |
+==========================================================================*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "_video.h"
#include "tmconfig.h"
#include    "utils.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
int		_Init_FB();
void	_Restore_FB();

#if defined(D_ARM) && !defined(D_PXA270)
//#ifdef	D_ARM && #ifndef	D_PXA270
#define            FLASH_LCD_INFO            "/dev/mtdblock1"
#else
#define            FLASH_LCD_INFO            "/dev/mtdblock4"
#endif

#define            FLASH_LCD_POSI           0x80
#define			   FLASH_LCD_PARA			0xC0
#define            PXAREG_LCD_SIGNAL     	0x12345678

typedef void * (* GUIMATRIX)(void *);
pthread_t	g_GUIMatrixThread;
char*		g_FBMem = NULL;
char*		g_FB2Mem = NULL;
//int			g_nRotate = 90;
XYRANGE		g_XYRange;
int			g_nCommand = 0;
int			g_nSetView_StartX = 0;;
int			g_nSetView_EndX = 0;
int			g_nSetView_StartY = 0;
int			g_nSetView_EndY = 0;
void 		GUIMatrixRun(WORD	wCode);
int			g_FbIotcol = 0;

sem_t 	g_SemGUI;
int			g_Rotation = 0;   //0=normal  1=90degree  2= 180 degree  3=270 degree
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
_FRAMEBUFFERINFO		_fbData =				//Framebuffer���Ѽ�
{
	-1, 0
};

_DRIVERINFO		g_infoDrv =
{
	0, 0, 0, 0, 0, NULL,
	_Init_FB,
	_Restore_FB
};

_DRIVER_LCD_PARA 	g_Drive_LCD_Para =
{
	0xFFFFFFFF,	0xFFFFFFFF,	0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF
};
/*==========================================================================+
|	Function	: _FB_Quit													|
|	Remark		: �{�ǰh�X�ɡA�������}���U�س]��							|
+==========================================================================*/
void	_Restore_FB()
{
	munmap(g_FB2Mem, g_infoDrv.nLength);
	//if (g_infoDrv.pcMem != NULL)
	//	delete g_infoDrv.pcMem;
	//if (g_FBMem != NULL)
	//	delete g_FBMem;

	//�h�X�ϧμҦ�
	if(_fbData.ntty >= 0)
	{
		ioctl(_fbData.ntty, KDSETMODE, KD_TEXT);
		close(_fbData.ntty);
	}

	if(_fbData.nfb >= 0)
	{
		close(_fbData.nfb);
	}
}

/*==========================================================================+
|  	Function	: _FB_Init                                 					|
|	Return		: 0		���l��Framebuffer���\	 		                    |
|				  �䥦	���~�N�X											|
|	Remark		: ���l��Framebuffer											|
+==========================================================================*/
int	_Init_FB()
{
	const char	*pszDev;

	struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    int	nHeight = 0, nWidth = 0;

    nWidth	= CtmConfig::GetInstance()->GetLCDWidth();
	nHeight = CtmConfig::GetInstance()->GetLCDHeight();

	//�����t�m�����S���o���ӡA�q�{���N�O640*480
	if(nWidth == 0 || nHeight == 0)
	{
		nWidth 	= 640;
		nHeight = 480;
	}

	//���o�]�ƪ��W��
	pszDev	= getenv("FRAMEBUFFER");
	if(pszDev == NULL)
		pszDev	= "/dev/fb0";

	//���}Framebuffer�]��
	_fbData.nfb	= open(pszDev, O_RDWR, 0);
	if(_fbData.nfb < 0)
	{
//		//printf("Couldn't Open %s:%s\n", pszDev,strerror(errno));
		return -1;
	}


	g_FbIotcol = _fbData.nfb;
	if(ioctl(_fbData.nfb, FBIOGET_VSCREENINFO, &vinfo) < 0)
	{
		perror("Couldn't get console hardware info:");
		return -1;
	}
	//#ifdef D_PXA270
	#ifdef D_ARM
	// add by 2008/6/25 01:04�U��  �վ�LCD�����v
	//if(!(vinfo.xres == nWidth && vinfo.yres == nHeight))
	//{
	//	vinfo.xres = vinfo.xres_virtual = nWidth;
	//	vinfo.yres = vinfo.yres_virtual = nHeight;
    //
	//	if(ioctl(_fbData.nfb, FBIOPUT_VSCREENINFO, &vinfo) < 0)
	//    {
	//        perror("Couldn't get console hardware info:");
	//        return -1;
	//    }
	//
	//	close(_fbData.ntty);
	//
	//	_Init_FB_Parameter(nWidth, nHeight);
	//	usleep(50);
	//
	//	_fbData.nfb	= open(pszDev, O_RDWR, 0);
	//	if(_fbData.nfb < 0)
	//	{
	//		printf("Couldn't Open %s:%s \n", pszDev, strerror(errno));
	//		return -1;
	//	}
	//}



	g_Drive_LCD_Para.signal = 0x12345678; //CtmConfig::GetInstance()->GetLCDSignal();
	g_Drive_LCD_Para.pixclock = CtmConfig::GetInstance()->GetLCDPixclock();
	g_Drive_LCD_Para.lccr0 = CtmConfig::GetInstance()->GetLCDLccr0();
	g_Drive_LCD_Para.lccr3 = CtmConfig::GetInstance()->GetLCDLccr3();
	g_Drive_LCD_Para.xres = CtmConfig::GetInstance()->GetLCDXres();
	g_Drive_LCD_Para.yres = CtmConfig::GetInstance()->GetLCDYres();
	g_Drive_LCD_Para.hsync_len = CtmConfig::GetInstance()->GetLCDHsync_len();
	g_Drive_LCD_Para.vsync_len = CtmConfig::GetInstance()->GetLCDVsync_len();
	g_Drive_LCD_Para.left_margin = CtmConfig::GetInstance()->GetLCDLeft_margin();
	g_Drive_LCD_Para.upper_margin = CtmConfig::GetInstance()->GetLCDUpper_margin();
	g_Drive_LCD_Para.right_margin = CtmConfig::GetInstance()->GetLCDRight_margin();
	g_Drive_LCD_Para.lower_margin = CtmConfig::GetInstance()->GetLCDLower_margin();
	g_Drive_LCD_Para.sync = CtmConfig::GetInstance()->GetLCDSync();
	g_Drive_LCD_Para.reserved = CtmConfig::GetInstance()->GetLCDReserved();

	//printf("%d, %d, %d, %d, \n %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \n",
	//		g_Drive_LCD_Para.signal, g_Drive_LCD_Para.pixclock, g_Drive_LCD_Para.lccr0, g_Drive_LCD_Para.lccr3,
	//		g_Drive_LCD_Para.xres, g_Drive_LCD_Para.yres, g_Drive_LCD_Para.hsync_len, g_Drive_LCD_Para.vsync_len,
	//		g_Drive_LCD_Para.left_margin, g_Drive_LCD_Para.upper_margin, g_Drive_LCD_Para.right_margin, g_Drive_LCD_Para.lower_margin,
	//		g_Drive_LCD_Para.sync, g_Drive_LCD_Para.reserved);

	_Write_LCD_Parameter();
	#endif

	//#endif
/*
	#ifdef D_PXA270
	#ifdef D_ARM

	if (CtmConfig::GetInstance()->GetLCDSizeFlag() < 9)
	{
		if(CtmConfig::GetInstance()->GetLCDSizeFlag() == 0)		//8�s�G��
		{
			vinfo.pixclock = 70000;
		}
		else if( CtmConfig::GetInstance()->GetLCDSizeFlag() == 1)	//10�o�G��
		{
			vinfo.pixclock = 40000;
		}

		if(ioctl(_fbData.nfb, FBIOPUT_VSCREENINFO, &vinfo) < 0)
		{
			perror("Couldn't get console hardware info:");
			return -1;
		}
		close(_fbData.ntty);
		usleep(50);

		_fbData.nfb = open(pszDev, O_RDWR, 0);
		if(_fbData.nfb < 0)
		{
			printf("Couldn't Open %s:%s\n", pszDev,strerror(errno));
			return -1;
		}
	}
	#endif
	#endif
*/
	//���oFramebuffer���U�ثH��
	if(ioctl(_fbData.nfb, FBIOGET_FSCREENINFO, &finfo) < 0)
	{
		perror("Couldn't get console hardware info:");
		return -1;
	}
/*
	//printf("fb_fix_screeninfo data:\n");
	//printf("ID              :%s\n", 	finfo.id			);
	//printf("Start Mem       :%08lx\n",	finfo.smem_start	);
	//printf("Men Length      :%08x\n",	finfo.smem_len		);
	//printf("FB_TYPE_        :%08x\n",	finfo.type			);
	//printf("FB_VISUAL_      :%08x\n",	finfo.visual		);
	//printf("mmio_start		:%08lx\n",	finfo.mmio_start	);
	//printf("mmio_len		:%08x\n",	finfo.mmio_len		);
	//printf("xpanstep		:%08x\n",	finfo.xpanstep		);
	//printf("accel			:%08x\n",	finfo.accel			);
	//printf("fb_var_screeninfo data:\n");
	//printf("xres			:%d\n",		vinfo.xres			);
	//printf("yres			:%d\n",		vinfo.yres			);
	//printf("xres_virtual	:%d\n",		vinfo.xres_virtual	);
	//printf("yres_virtual	:%d\n",		vinfo.yres_virtual	);
	//printf("xoffset			:%d\n",		vinfo.xoffset		);
	//printf("yoffset			:%d\n",		vinfo.yoffset		);
	//printf("bits_per_pixel	:%d\n",		vinfo.bits_per_pixel);
	//printf("grayscale		:%d\n",		vinfo.grayscale		);
	//printf("nonstd			:%x\n",		vinfo.nonstd		);
	//printf("activate		:%x\n",		vinfo.activate		);
	//printf("height			:%d\n",		vinfo.height		);
	//printf("width			:%d\n",		vinfo.width			);
	//printf("accel_flags		:%d\n",		vinfo.accel_flags	);
	//printf("pixclock		:%d\n",		vinfo.pixclock		);
	//printf("left_margin		:%d\n",		vinfo.left_margin	);
	//printf("right_margin	:%d\n",		vinfo.right_margin	);
	//printf("upper_margin	:%d\n",		vinfo.upper_margin	);
	//printf("lower_margin	:%d\n",		vinfo.lower_margin	);
	//printf("hsync_len		:%d\n",		vinfo.hsync_len		);
	//printf("vsync_len		:%d\n",		vinfo.vsync_len		);
	//printf("sync			:%d\n",		vinfo.sync			);
	//printf("vmode			:%d\n",		vinfo.vmode			);
//	//printf("rotate			:%d\n",		vinfo.rotate		);
	//printf("color info\n");
	//printf("Red offset		:%d\n",		vinfo.red.offset	);
	//printf("Red Length		:%d\n",     vinfo.red.length	);
	//printf("green offset	:%d\n",		vinfo.green.offset	);
	//printf("green Length	:%d\n",     vinfo.green.length	);
	//printf("blue offset		:%d\n",		vinfo.blue.offset	);
	//printf("blue Length		:%d\n",     vinfo.blue.length	);
*/
	//=======
	//�M�g���s
	//=======
	g_infoDrv.nLength	= finfo.smem_len;
	g_infoDrv.nBits		= vinfo.bits_per_pixel;
	g_infoDrv.nX		= vinfo.xres;
	g_infoDrv.nY		= vinfo.yres;

	//�]�m�M�g���s�j�p
	g_infoDrv.pcMem   		= new 	char[2048*g_infoDrv.nX*4];
	g_FBMem			= new 	char[2048*g_infoDrv.nX*4];
	printf("g_infoDrv.nLength=%d bit=%d  x=%d y=%d bpp=%d \n", g_infoDrv.nLength, g_infoDrv.nBits, g_infoDrv.nX, g_infoDrv.nY, g_infoDrv.nBits);
	//g_FB2Mem
	g_FB2Mem = (char *)mmap(	NULL,
									g_infoDrv.nLength,
									PROT_READ|PROT_WRITE,
									MAP_SHARED,
									_fbData.nfb,
									0
								  );


	if(g_FB2Mem == (char *)-1)
	{
		fprintf(stderr, "Unable to memory map the video hardware\n");
		return -1;
	}

	//���{�ǰh�X���A�۰ʫ��_�ܭ������Ҧ��C
	atexit(_Restore_FB);

	//�i�J�ϧμҦ�
	_fbData.ntty	= open("/dev/tty0",O_RDWR, 0);
	if (_fbData.ntty < 0)
	{
		fprintf(stderr, "Open console terminal error!");
		return -1;
	}

	atexit(_Restore_FB);

	if (ioctl(_fbData.ntty, KDSETMODE, KD_GRAPHICS) == -1)
	{
		fprintf(stderr, "Change console terminal to Graphic fail!");
		return -1;
	}

	//memset(g_infoDrv.pcMem, 0, g_infoDrv.nLength);
	//memset();


    sem_init(&g_SemGUI, 0, 0);
	GUIMatrixRun(0);
	//pthread_create(&g_GUIMatrixThread, NULL, (GUIMATRIX)GUIMatrixRun, NULL);
	return 0;
}

void _Init_FB_Parameter(int nWidth, int	nHeight)
{
    int fd, flag_write = 1;
    unsigned int dwData;
    unsigned short nW, nH;

    if ((fd = open(FLASH_LCD_INFO, O_RDWR)) >= 0)
    {
        lseek(fd, FLASH_LCD_POSI, 0);
        read(fd, &dwData, 4);
        if (dwData == PXAREG_LCD_SIGNAL)
        {
                read(fd, &dwData, 4);
                nW = dwData & 0xFFFF;
                nH = (dwData >> 16) & 0xFFFF;
                if (nW == nWidth && nH == nHeight)
                        flag_write = 0;
        }
        else
        {
                dwData = PXAREG_LCD_SIGNAL;
                lseek(fd, FLASH_LCD_POSI, 0);
                write(fd, &dwData, 4);
        }
        if (1 == flag_write)
        {
                dwData = nHeight;
                dwData = dwData << 16 | nWidth;
                lseek(fd, FLASH_LCD_POSI+4, 0);
                write(fd, &dwData, 4);
        }
        close(fd);
    }
}

unsigned long  	*g_VirRows[2048];				//���VViewPort���@���Ʋ�
unsigned long  	*g_FBRows[2048];				//���VViewPort���@���Ʋ�
unsigned long  	*g_FB2Rows[2048];				//���VViewPort���@���Ʋ�

void	ChangePos()
{
	printf("ChangePos()\n");
	//return ;
	long			lOldColor = 0;
	int		nCurX  = g_infoDrv.nX, nCurY = g_infoDrv.nY;
	//int		nSetX = 768, nSetY = 1366;
	#ifdef	D_X86
	int		nSetX = 800, nSetY = 600;
	#else
	int		nSetX = 600, nSetY = 1024;
	#endif
	int		nRealX = 0, nRealY = 0;
	int		nStartX = 0, nStartY = 0;
	XYRANGE		XYRange;
	XYRANGE		XYUpdateRange;
//checkagain:
	//sem_wait (&g_SemGUI);
	XYUpdateRange.y1 = 0;
	XYUpdateRange.y2 = 0;
	XYUpdateRange.x1 = 0;
	XYUpdateRange.x2 = 0;

	//printf("ChangePos \n");
	if(g_nCommand == 1)
		return ;
	else
	{
		if(g_nCommand == 2)
		{
			printf("set update ALL 1 nSetY=%d, nSetX=%d \n", nSetY, nSetX);
			printf("sX=%d, sY=%d, eX=%d, eY=%d \n", g_nSetView_StartX, g_nSetView_StartY,
			g_nSetView_EndX, g_nSetView_EndY);
			for(int i = g_nSetView_StartY; i < g_nSetView_EndY; i++)
			{
				for(int j = g_nSetView_StartX; j < g_nSetView_EndX; j++)
				{
					if(g_Rotation == 1)
					{
						nRealX = nCurX- i - 1;
						nRealY = j;
					}
					else if(g_Rotation == 3)
					{
						nRealX = i;//nCurY- i - 1;
						nRealY = nCurY - j -1;
					}
					else if(g_Rotation == 0)
					{
							nRealX = j;
							nRealY = i;
					}
					if(nRealY < 0)
						nRealY = 0;
					//pFBMemTemp = pFBMem+ j*nCurX + (nCurY-i);
					//printf("nRealY=%d nRealX=%d nCurY=%d, j=%d\n", nRealY, nRealX, nCurY, j);
					#ifdef	D_X86
					if(nRealY >= 1280  || nRealX >=1280)
					#else
					if(nRealY >= 600  || nRealX >=1024)
					#endif
					{
						;printf("error nRealY=%d nRealX=%d \n", nRealY, nRealX);
					}
					lOldColor = g_FB2Rows[nRealY][nRealX];
					if( lOldColor != g_VirRows[i][j])
					{
						//g_FBRows[i][j] = g_VirRows[i][j];
						;g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
					}
					//printf("x=%d y=%d x2=%d y2=%d \n", i, j,  j*nCurX , (nCurY-i));
				}
			}
			#ifdef	D_X86
			for(int i = 0; i < 1024; i++)
			#else
			for(int i = 0; i < 600; i++)
			#endif
			//for(int i = 0; i < 300; i++)
			{
				//for(int j = 0; j < nSetX; j++)
				//{
				//if(memcmp(g_FB2Rows[nCurY-i-1],g_VirRows[nCurY-i-1], nSetY *4) != 0)
				//printf("i=%d \n", i);
				//memcpy(g_FB2Rows[nCurY-i-1], g_FBRows[nCurY-i-1], nSetY *4);
				//printf("nSetY=%d \n", nSetY);
					memcpy(g_FB2Rows[i], g_FBRows[i], (nSetY) *4);
				//}
				//printf("i=%d \n", i);
			}
			//printf("set update ALL 2\n");
			g_XYRange.flag == FALSE;
			g_XYRange.x1 = 0;
			g_XYRange.x2 = 0;
			g_XYRange.y1 = 0;
			g_XYRange.y2 = 0;
			g_nCommand = 0;
		}
		else if(g_nCommand == 3)
		{
			printf("set update ALL 2 nSetY=%d \n", nSetY, nSetX);
			//nSetViewX =
			for(int i = 0; i < nSetY; i++)
			{
				for(int j = 0; j < nSetX; j++)
				{
					if(g_Rotation == 1)
					{
						nRealX = nCurX- i - 1;
						nRealY = j;
					}
					else if(g_Rotation == 3)
					{
						nRealX = i;//nCurY- i - 1;
						nRealY = nCurY - j -1;
					}
					else if(g_Rotation == 0)
					{
							nRealX = j;
							nRealY = i;
					}
					//pFBMemTemp = pFBMem+ j*nCurX + (nCurY-i);
					lOldColor = g_FB2Rows[nRealY][nRealX];
					if( lOldColor != g_VirRows[i][j])
					{
						//g_FBRows[i][j] = g_VirRows[i][j];
						g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
					}
					//printf("x=%d y=%d x2=%d y2=%d \n", i, j,  j*nCurX , (nCurY-i));
				}
			}
			for(int i = 0; i < nSetX; i++)
			//for(int i = 0; i < 300; i++)
			{
				//for(int j = 0; j < nSetX; j++)
				//{
				//if(memcmp(g_FB2Rows[nCurY-i-1],g_VirRows[nCurY-i-1], nSetY *4) != 0)
				//printf("i=%d \n", i);
				//memcpy(g_FB2Rows[nCurY-i-1], g_FBRows[nCurY-i-1], nSetY *4);
					memcpy(g_FB2Rows[i], g_FBRows[i], (nSetY) *4);
				//}
				//printf("i=%d \n", i);
			}
			//printf("set update ALL 2\n");
			g_XYRange.flag == FALSE;
			g_XYRange.x1 = 0;
			g_XYRange.x2 = 0;
			g_XYRange.y1 = 0;
			g_XYRange.y2 = 0;
			g_nCommand = 0;
		}
		else
		{
		//	return ;
			if(g_XYRange.flag == TRUE)
			{

				XYRange.x1 = g_XYRange.x1 -2;
				XYRange.y1 = g_XYRange.y1 -2;
				XYRange.x2 = g_XYRange.x2 +2;
				XYRange.y2 = g_XYRange.y2 +2;

				if(XYRange.x1 < 0)
					XYRange.x1 = 0;
				if(XYRange.x2 > nSetX)
					XYRange.x2 = nSetX;
				if(XYRange.y1 < 0)
					XYRange.y1 = 0;
				if(XYRange.y2 > nSetY)
					XYRange.y2 = nSetY;

				//printf("update %d.%d  %d.%d \n", XYRange.x1, XYRange.y1, XYRange.x2, XYRange.y2);
				for(int i = XYRange.y1; i < XYRange.y2; i++)
				{
					for(int j = XYRange.x1; j < XYRange.x2; j++)
					{
						nRealX = i;//nCurY- i - 1;
						nRealY = nCurY - j-1;
						//pFBMemTemp = pFBMem+ j*nCurX + (nCurY-i);
						lOldColor = g_FB2Rows[nRealY][nRealX];
						if( lOldColor != g_VirRows[i][j])
						{
							g_FBRows[nRealY][nRealX] = g_VirRows[i][j];

							if(nStartX == 0)
							{
								XYUpdateRange.x1 = nRealX;
								XYUpdateRange.y1 = nRealY;
								nStartX = 1;
							}
							else
							{
								if(XYUpdateRange.x1 > nRealX)
									XYUpdateRange.x1 = nRealX;
								if(XYUpdateRange.y1 > nRealY)
									XYUpdateRange.y1 = nRealY;
							}

							if(XYUpdateRange.x2 < nRealX)
								XYUpdateRange.x2 = nRealX;
							if(XYUpdateRange.y2 < nRealY)
								XYUpdateRange.y2 = nRealY;
							//g_FB2Rows[i][j] = g_FBRows[i][j];
						}
						//printf("x=%d y=%d x2=%d y2=%d \n", i, j,  j*nCurX , (nCurY-i));
					}
				}
				//printf("update2 y1=%d y2=%d x2=%d x1=%d\n", XYUpdateRange.y1, XYUpdateRange.y2, XYUpdateRange.x2, XYUpdateRange.x1);
				//for(int i = 0; i < nSetX; i++)
				if(XYUpdateRange.y1 > 0)
					XYUpdateRange.y1 = XYUpdateRange.y1 - 1;
				if(XYUpdateRange.y2 > 0)
					XYUpdateRange.y2 = XYUpdateRange.y2 + 1;
				for(int i = XYUpdateRange.y1; i < XYUpdateRange.y2; i++)
				{
					//for(int j = 0; j < nSetX; j++)
					//{
					//if(memcmp(g_FB2Rows[nCurY-i-1],g_VirRows[nCurY-i-1], nSetY *4) != 0)
					//printf("i=%d \n", i);
					//memcpy(g_FB2Rows[nCurY-i-1], g_FBRows[nCurY-i-1], nSetY *4);
						memcpy(g_FB2Rows[i]+XYUpdateRange.x1, g_FBRows[i]+XYUpdateRange.x1, (XYUpdateRange.x2-XYUpdateRange.x1+1) *4);
					//}
					//printf("i=%d \n", i);
				}
				XYUpdateRange.y1 = 0;
				XYUpdateRange.y2 = 0;

				g_XYRange.flag = FALSE;
			}
		}
	}
	//printf("end \n");

//	else
//	{
//		for(int i = 0; i < nSetY; i++)
//		{
//			for(int j = 0; j < nSetX; j++)
//			{
//				nRealX = i;//nCurY- i - 1;
//				nRealY = nCurY - j-1;
//				//pFBMemTemp = pFBMem+ j*nCurX + (nCurY-i);
//				lOldColor = g_FB2Rows[nRealY][nRealX];
//				if( lOldColor != g_VirRows[i][j])
//				{
//					g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
//					//g_FB2Rows[i][j] = g_FBRows[i][j];
//				}
//				//printf("x=%d y=%d x2=%d y2=%d \n", i, j,  j*nCurX , (nCurY-i));
//			}
//		}
//	}
	//goto checkagain;
	printf("ChangePos() End\n");
}

void 	GUIMatrixRun(WORD	wCode)
{
//	pthread_detach(pthread_self());
//	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
//	{
//
//	}

	unsigned long	*pVirMem;
	unsigned long	*pFBMem;
	unsigned long	*pFB2Mem;
	unsigned long	*pFBMemTemp;
	pVirMem		= (unsigned long *)g_infoDrv.pcMem;
	pFBMem		= (unsigned long *)g_FBMem;
	pFB2Mem		= (unsigned long *)g_FB2Mem;
	int				Offset = 0;
	long			lOldColor = 0;



	int		nCurX  = g_infoDrv.nX, nCurY = g_infoDrv.nY;
	int		nSetX = 600, nSetY = 1024;
	int		nRealX = 0, nRealY = 0;


	//for(int i = 0; i < 1024; i++)
	for(int i = 0; i < 1280; i++)
	{
		g_VirRows[i] 	= pVirMem + Offset;
		g_FBRows[i]  	= pFBMem + Offset;
		Offset	 	+= g_infoDrv.nX;
	}

	Offset = 0;
	for(int i = 0; i < nCurY; i++)
	{
		g_FB2Rows[i]  	= pFB2Mem + Offset;
		Offset	 	+= g_infoDrv.nX;
	}

	printf("Drv.nX=%d, nCurX=%d nCurY=%d nSetX=%d nSetY=%d \n",g_infoDrv.nX, nCurX, nCurY, nSetX, nSetY);
//	while (TRUE)
//	{
//checkagain:
		//sem_wait (&g_SemGUI);

		//printf("sem_wait 1 \n");
		//for(int i = 0; i < nSetY; i++)
		//{
		//	for(int j = 0; j < nSetX; j++)
		//	{
		//		nRealX = i;//nCurY- i - 1;
		//		nRealY = nCurY - j-1;
		//		//pFBMemTemp = pFBMem+ j*nCurX + (nCurY-i);
		//		lOldColor = g_FB2Rows[nRealY][nRealX];
		//		if( lOldColor != g_VirRows[i][j])
		//		{
		//			g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
		//			//g_FB2Rows[i][j] = g_FBRows[i][j];
		//		}
		//		//printf("x=%d y=%d x2=%d y2=%d \n", i, j,  j*nCurX , (nCurY-i));
		//	}
		//}

		//for(int i = 0; i < nSetX; i++)
		//{
		//	//for(int j = 0; j < nSetX; j++)
		//	//{
		//	//if(memcmp(g_FB2Rows[nCurY-i-1],g_VirRows[nCurY-i-1], nSetY *4) != 0)
		//	//printf("i=%d \n", i);
		//		memcpy(g_FB2Rows[nCurY-i-1], g_FBRows[nCurY-i-1], nSetY *4);
		//	//}
		//	//printf("i=%d \n", i);
		//}
//		usleep(50*1000);
		//sem_wait (&g_SemGUI);
		//goto checkagain;
		//printf("test 1 \n");
		//usleep(10*1000);
//	}
}

void	_Write_LCD_Parameter()
{
	int fd;

    if(0 == CtmConfig::GetInstance()->GetLCDParaInit())
    {
	    if ((fd = open(FLASH_LCD_INFO, O_RDWR)) >= 0)
	    {
	        lseek(fd, FLASH_LCD_PARA, 0);
	        write(fd, &g_Drive_LCD_Para, sizeof(_DRIVER_LCD_PARA));
	        close(fd);
	        CtmConfig::GetInstance()->SetLCDParaInit(1);
	    }
	}
}


void __far  _VideoPowerOn()
{
}
void __far  _VideoPowerOff()
{
}

void __far  _VideoBrighter(BYTE Brighter)       //wy042400
{
}

void __far  _IOTest(BYTE Data)       //Bw061902
{
}
