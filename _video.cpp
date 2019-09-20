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
BOOL  	RegionUpdateStop = FALSE;
BOOL  	RegionUpdateStop_APP = FALSE;
sem_t 	g_SemGUI;
int			g_Rotation = 3;   //0=normal  1=90degree  2= 180 degree  3=270 degree
RECT    RegionSum;
int	g_nTouchView = 0;	
int	g_nTouchX;
int	g_nTouchY;
pthread_mutex_t ChangeRegionSum = PTHREAD_MUTEX_INITIALIZER ;
pthread_mutex_t Changing = PTHREAD_MUTEX_INITIALIZER ;
RECT	MsgBoxRegion;
CtmBitMap  touch_bmp;
int touch_bmp_w=12,touch_bmp_h=19,read_bmp_flag=-1;
//DWORD touch_bmp_buffer[31][31];
#ifdef	D_32BIT
DWORD**  		touch_bmp_buffer;
#else
WORD**  		touch_bmp_buffer;
	#endif
	
int		MouseMoveStep=0;
int currsor_x=-1,currsor_y=-1;
/*int r=120;
int touch_image[30][30]={
{0,0,0,0,0,0,0,0,0,0,0,0,r,r,r,r,r,r,0,0,0,0,0,0,0,0,0,0,0,0},	//1
{0,0,0,0,0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0,0,0,0,0},	//2
{0,0,0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0,0,0},	//3
{0,0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0,0},	//4
{0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0},	//5
{0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0},	//6
{0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0},	//7
{0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0},	//8
{0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0},	//9
{0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0},	//10
{0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0},	//11
{0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0},	//12
{r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r},	//13
{r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r},	//14
{r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r},	//15
{r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r},	//16
{r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r},	//17
{r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r},	//18
{0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0},	//19
{0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0},	//20
{0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0},	//21
{0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0},	//22
{0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0},	//23
{0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0},	//24
{0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0},	//25
{0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0},	//26
{0,0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0,0},	//27
{0,0,0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0,0,0},	//28
{0,0,0,0,0,0,0,0,0,r,r,r,r,r,r,r,r,r,r,r,r,0,0,0,0,0,0,0,0,0},	//29
{0,0,0,0,0,0,0,0,0,0,0,0,r,r,r,r,r,r,0,0,0,0,0,0,0,0,0,0,0,0},	//30
};*/
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
	pthread_mutex_init(&ChangeRegionSum,NULL);
	pthread_mutex_init(&Changing,NULL);
	
  pthread_mutex_lock(&ChangeRegionSum);
	RegionSum.left =0;
	RegionSum.top =0;
	RegionSum.right =0;
	RegionSum.bottom =0;
	RegionSum.left=RegionSum.top=RegionSum.right=RegionSum.bottom=0;
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
  	pthread_mutex_unlock(&ChangeRegionSum);
		return -1;
	}


	g_FbIotcol = _fbData.nfb;
	if(ioctl(_fbData.nfb, FBIOGET_VSCREENINFO, &vinfo) < 0)
	{
		perror("Couldn't get console hardware info:");
  	pthread_mutex_unlock(&ChangeRegionSum);
		return -1;
	}
	//#ifdef D_PXA270
	#ifdef D_ARM
	
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

	//���oFramebuffer���U�ثH��
	if(ioctl(_fbData.nfb, FBIOGET_FSCREENINFO, &finfo) < 0)
	{
		perror("Couldn't get console hardware info:");
  	pthread_mutex_unlock(&ChangeRegionSum);
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
	//printf("g_infoDrv.nLength=%d bit=%d  x=%d y=%d bpp=%d \n", g_infoDrv.nLength, g_infoDrv.nBits, g_infoDrv.nX, g_infoDrv.nY, g_infoDrv.nBits);
	//g_FB2Mem
	g_FB2Mem = (char *)mmap(	NULL,
									g_infoDrv.nLength,
									PROT_READ|PROT_WRITE,
									MAP_SHARED,
									_fbData.nfb,
									0
								  );


  pthread_mutex_unlock(&ChangeRegionSum);
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

	//read touch bmp


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
        	//read touch bmp
        	
    }
}

void Read_Touch_Bmp()
{
	printf("Read_Touch_Bmp\n");
	if(touch_bmp.Create("res_tm640/456.bmp")!=0)
	{	
		read_bmp_flag=0;
		printf(" bmp %s error     !!!!!!!!!!!!!  %d\n","res_tm640/toucg_1.bmp",touch_bmp.Create("res_tm640/toucg_1.bmp"));
	}
	else
	{
		RECT bmp_rect;
		MKRECT(bmp_rect,0,0,touch_bmp_w,touch_bmp_h);
		
		
		#ifdef	D_32BIT
		touch_bmp_buffer = new DWORD* [touch_bmp_h+1];
		#else
		touch_bmp_buffer = new WORD* [touch_bmp_h+1];
		#endif
		if (touch_bmp_buffer != NULL)
		{
			for (int i = 0; i < touch_bmp_h+1 ; i++)
			{
				#ifdef	D_32BIT
				*(touch_bmp_buffer + i) = new DWORD [touch_bmp_w+1];
				memset(*(touch_bmp_buffer + i), 0, (touch_bmp_w+1)*sizeof(DWORD));
				#else
				*(touch_bmp_buffer + i) = new WORD [touch_bmp_w+1];
				memset(*(touch_bmp_buffer + i), 0, (touch_bmp_w+1)*sizeof(WORD));
				#endif
			}
		}
		touch_bmp.SetImageBuffer(bmp_rect,(DWORD**)touch_bmp_buffer);
		for(int py=0; py<touch_bmp_h;py++)
		{
			for(int px=0; px<touch_bmp_w;px++)
			{
				if(touch_bmp_buffer[py][px]!=0xff00ff)
					printf("**");
				else
					printf("  ");
			}
			printf("\n");
		}
		read_bmp_flag = 1;
	}
}

unsigned long CalTranRGB(unsigned long FB,unsigned long touch,float tran,int x,int y)
{
	float fb_tran=1-tran;
	float touch_tran=tran;
	/*float avg = (float)(touch&0XFF)/255;
	if(((float)((touch>>8)&0XFF)/255) <  avg)
	avg=((float)((touch>>8)&0XFF)/255);
	if(((float)((touch>>16)&0XFF)/255) <  avg)
	avg=((float)((touch>>16)&0XFF)/255);*/
	int len2 =((x-15)*(x-15))+((y-15)*(y-15));
	if(len2 > 4*4)
	{
		float avg = sqrt((float)len2)/sqrt(2*8*8);
		if(avg > 1) avg=1;
		touch_tran = tran*(1-avg);
		fb_tran = (1-touch_tran);
	}
	
	//fb_tran = (1-tran)+tran*avg;
	//touch_tran = tran*(1-avg);
	unsigned long r=(255-(FB&0XFF))		*(fb_tran) + (255-(touch&0XFF))				*(touch_tran);
	//printf("fbt=%f tot=%f\n",fb_tran,touch_tran);
	unsigned long g=(255-((FB>>8)&0XFF))	*(fb_tran) + (255-((touch>>8)&0XFF))	*(touch_tran);
	//printf("g fbt=%f tot=%f\n",fb_tran,touch_tran);
	unsigned long b=(255-((FB>>16)&0XFF))	*(fb_tran) + (255-((touch>>16)&0XFF))	*(touch_tran);
	//printf("B fbt=%f tot=%f\n",fb_tran,touch_tran);
	//printf("000 r=%d g=%d b=%d\n",FB&0XFF,(FB>>8)&0XFF,(FB>>16)&0XFF);
	//printf("111 r=%d g=%d b=%d\n",touch&0XFF,(touch>>8)&0XFF,(touch>>16)&0XFF);
	//printf("222 r=%d g=%d b=%d\n",r,g,b);
	unsigned long temp = (255-r)+((255-g)<<8)+((255-b)<<16);
	return temp;
	
}

void	ClearLastTouch()
{
	
	
}
unsigned long  	*g_VirRows[2048];				//���VViewPort���@���Ʋ�
unsigned long  	*g_FBRows[2048];				//���VViewPort���@���Ʋ�
unsigned long  	*g_FB2Rows[2048];				//���VViewPort���@���Ʋ�
int Stop_x1=599,Stop_x2=0,Stop_y1=1023,Stop_y2=0;


int		nRealX,nRealY;
void	ChangePos(int app_update)
{
	//printf("g_Rotation = %d\n",g_Rotation);
  pthread_mutex_lock(&Changing);
  if(read_bmp_flag == -1) Read_Touch_Bmp();
	int		Start_X,Start_Y;
	int		nSetX = 600, nSetY = 1024;
	int		nCurX = g_infoDrv.nX , nCurY = g_infoDrv.nY;
	
	if(!app_update && g_nCommand==1)
	{
		Start_X = 0;
		Start_Y =0;
		nSetX =600;
		nSetY =1024;
	}
	else if(!app_update && g_nCommand==2)
	{
		//Start_X = 0;
		//Start_Y =0;
		//nSetX =600;
		//nSetY =1024;
		Start_X = MsgBoxRegion.left-5;
		Start_Y =MsgBoxRegion.top-5;
		nSetX =MsgBoxRegion.right+5;
		nSetY =MsgBoxRegion.bottom+15;
		
		if(Start_X > Stop_x1) Start_X=Stop_x1-5;
		if(Start_Y > Stop_y1) Start_Y=Stop_y1-5;
		if(nSetX < Stop_x2) nSetX=Stop_x2+5;
		if(nSetY < Stop_y2) nSetY=Stop_y2+5;
	}
	else if(app_update && RegionUpdateStop==FALSE)
	{
		if(Stop_x1>=Stop_x2 || Stop_y1 >= Stop_y2)
		{
  		pthread_mutex_unlock(&Changing);
  		return;
  	}
		Start_X=Stop_x1-5;
		Start_Y=Stop_y1-5;
		nSetX=Stop_x2+5;
		nSetY=Stop_y2+5;
	}
	if(Start_X < 0) Start_X=0;
	if(Start_Y < 0) Start_Y=0;
	if(nSetX > 599) nSetX=599;
	if(nSetY > 1023) nSetY=1023;
		
	RegionSum.left = RegionSum.top = RegionSum.right = RegionSum.bottom =0;
	Stop_x1=599;
	Stop_x2=0;
	Stop_y1=1023;
	Stop_y2=0;

	//printf("@@@%d  %d  %d  %d  %d \n",Start_X,Start_Y,nSetX,nSetY);
	//int mx=9999,my=9999,Mx=0,My=0;
	for(int i = Start_Y; i < nSetY; i++)
	{
		for(int j = Start_X; j < nSetX; j++)
		{
			if(g_Rotation == 1)
			{
				nRealX = nCurX- i - 1;
				nRealY = j;
			}
			else if(g_Rotation == 3)
			{
				nRealX = i;
				nRealY = nCurY - j -1;
			}
			else if(g_Rotation == 0)
			{
					nRealX = j;
					nRealY = i;
			}
			/*if(mx>nRealX) mx=nRealX;
			if(my>nRealY) my=nRealY;
			if(Mx<nRealX) Mx=nRealX;
			if(My<nRealY) My=nRealY;*/
			if(nRealY < 0)
				nRealY = 0;
			if(i>=currsor_y&&j>=currsor_x&&i<currsor_y+touch_bmp_h&&j<currsor_x+touch_bmp_w)	
			{
				if(g_nTouchView>0&&touch_bmp_buffer[i-currsor_y][j-currsor_x]!=0xff00ff)
					g_FBRows[nRealY][nRealX] = touch_bmp_buffer[i-currsor_y][j-currsor_x];
				else
					g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
			}
			else
				g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
		}
	}
	
	/**add **/
	
	
	
	int i1;
	int	i2;
	//printf("nCurY = %d i1 = %d i2 = %d\n",nCurY,i1,i2);
	int y_data_len = (nSetY-Start_Y)*4;
	int tmp;
	if(g_Rotation == 1)
	{
		i1=Start_X;
		i2=nSetX;
		y_data_len = (nSetY-Start_Y)*4;
		tmp = nCurX- nSetY - 1;
	}
	else if(g_Rotation == 3)
	{
		i1=nCurY - nSetX;
		i2=nCurY - Start_X;
		tmp = Start_Y;
		y_data_len = (nSetY-Start_Y)*4;
		//printf("i1=%d  i2=%d tmp=%\n",i1,i2,tmp);
	}
	for(int i = i1; i < i2; i++)
			memcpy(&g_FB2Rows[i][tmp], &g_FBRows[i][tmp],y_data_len);
	RegionUpdateStop = FALSE;
	RegionUpdateStop_APP = FALSE;
	//printf("%d %d %d %d \n",mx,my,Mx,My);
  pthread_mutex_unlock(&Changing);
}


void	ChangePosNow(RECT rect)
{
  pthread_mutex_lock(&Changing);
	if(RegionUpdateStop || RegionUpdateStop_APP)
	{
		if(Stop_x1 > rect.left)
			Stop_x1=rect.left;
		if(Stop_x2 < rect.right)
			Stop_x2=rect.right;
		if(Stop_y1 > rect.top)
			Stop_y1=rect.top;
		if(Stop_y2 < rect.bottom)
			Stop_y2=rect.bottom;
		
  	pthread_mutex_unlock(&Changing);
		return;
	}
	ChangePos(rect.left-5,rect.top-5,rect.right+5,rect.bottom+5);
  pthread_mutex_unlock(&Changing);
}

void	ChangePos(int x1,int y1,int x2,int y2)
{
	if(RegionUpdateStop || RegionUpdateStop_APP)
		return;
	//printf("!!!%d  %d  %d  %d  \n",x1,y1,x2,y2);
  if(read_bmp_flag == -1) Read_Touch_Bmp();
	int		nCurX = g_infoDrv.nX , nCurY = g_infoDrv.nY;
	if(x1<0)x1=0;
	if(y1<0)y1=0;
	if(x2>599)x2=599;
	if(y2>1023)y2=1023;

	for(int i = y1; i < y2; i++)
	{
		for(int j = x1; j < x2; j++)
		{
			if(g_Rotation == 1)
			{
				nRealX = nCurX- i - 1;
				nRealY = j;
			}
			else if(g_Rotation == 3)
			{
				nRealX = i;
				nRealY = nCurY - j -1;
			}
			else if(g_Rotation == 0)
			{
					nRealX = j;
					nRealY = i;
			}
			if(nRealY < 0)
				nRealY = 0;
					
			if(g_nTouchView>0&&i>=currsor_y&&j>=currsor_x&&i<currsor_y+touch_bmp_h&&j<currsor_x+touch_bmp_w)
			{
				if(touch_bmp_buffer[i-currsor_y][j-currsor_x]!=0xff00ff)
					g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX] = touch_bmp_buffer[i-currsor_y][j-currsor_x];
				else
					g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
			}
			else
				g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
			//g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX];
		}
	}
}

void	EnableMouseCursor(int x,int y)
{
  if(read_bmp_flag == -1) Read_Touch_Bmp();
  if(read_bmp_flag !=1 ) return;
  UnEnableMouseCursor();
  pthread_mutex_lock(&Changing);
  currsor_x = x; 
  currsor_y = y;
	g_nTouchView=20;
	int y1,x1,y2,x2;
	int		nCurX = g_infoDrv.nX , nCurY = g_infoDrv.nY;
	//printf("x = %d , y = %d , nCurX=%d \n",x,y,nCurX);
	y1 = currsor_y;
	x1 = currsor_x;
	y2 = y1+touch_bmp_h;
	x2 = x1+touch_bmp_w;
	if(y2>1023) y2=1023;
	if(x2>599) 	x2=599;
	for(int i = y1; i < y2; i++)
	{
		for(int j = x1; j < x2; j++)
		{
			if(touch_bmp_buffer[i-currsor_y][j-currsor_x] == 0xff00ff) continue;
			if(g_Rotation == 1)
			{
				nRealX = nCurX- i - 1;
				nRealY = j;
			}
			else if(g_Rotation == 3)
			{
				nRealX = i;
				nRealY = nCurY - j -1;
			}
			else if(g_Rotation == 0)
			{
					nRealX = j;
					nRealY = i;
			}
			if(nRealY < 0)
				nRealY = 0;
			if(nRealX < 0)
				nRealX = 0;
			g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX] = touch_bmp_buffer[i-currsor_y][j-currsor_x];
		}
	}
  pthread_mutex_unlock(&Changing);
}

void	UnEnableMouseCursor()
{
	if(currsor_x==-1 && currsor_y==-1) 
	{
		g_nTouchView=0;
		return;
	}
  pthread_mutex_lock(&Changing);
	g_nTouchView=0;
	int y1,x1,y2,x2;
	int		nCurX = g_infoDrv.nX , nCurY = g_infoDrv.nY;
	y1 = currsor_y;
	x1 = currsor_x;
	y2 = y1+touch_bmp_h;
	x2 = x1+touch_bmp_w;
	if(y2>1023) y2=1023;
	if(x2>599) 	x2=599;
	for(int i = y1; i < y2; i++)
	{
		for(int j = x1; j < x2; j++)
		{
			if(g_Rotation == 1)
			{
				nRealX = nCurX- i - 1;
				nRealY = j;
			}
			else if(g_Rotation == 3)
			{
				nRealX = i;
				nRealY = nCurY - j -1;
			}
			else if(g_Rotation == 0)
			{
					nRealX = j;
					nRealY = i;
			}
			if(nRealY < 0)
				nRealY = 0;
			if(nRealX < 0)
				nRealX = 0;
			g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX] = g_VirRows[i][j];
			//g_FB2Rows[nRealY][nRealX] = g_FBRows[nRealY][nRealX];
		}
	}
  pthread_mutex_unlock(&Changing);
}

void 	GUIMatrixRun(WORD	wCode)
{
	unsigned long	*pVirMem;
	unsigned long	*pFBMem;
	unsigned long	*pFB2Mem;
	unsigned long	*pFBMemTemp;
	pVirMem		= (unsigned long *)g_infoDrv.pcMem;
	pFBMem		= (unsigned long *)g_FBMem;
	pFB2Mem		= (unsigned long *)g_FB2Mem;
	int				Offset = 0;

	int		nCurY = g_infoDrv.nY;

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
