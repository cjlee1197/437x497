/*==========================================================================+
|  Class    : _vido.h                                                 		|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: 初始化Framebuffer以及圖形庫。                                 |
+==========================================================================*/
#ifndef		_VIDEO_H
#define		_VIDEO_H

#include	"common.h"

#define		tmMAXVIDEO_X			1920//1600					//分辨率-X
#define		tmMAXVIDEO_Y			1440//1200					//分辨率-Y
#define		tmVIDEO_BITS		16						//分辨率-顏色位數

//=======
//       Control parameters for _rectangle
//=======
#define     _tmGBORDER            2         /* draw outline only */
#define     _tmGFILLINTERIOR      3         /* fill using current fill mask */

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct _DEVICEINFO
{
	int				nLength;		//映射內存的長度
	int				nBits;			//色深
	unsigned short	wPort;			//IO端口
	int				nX;
	int				nY;
	char			*pcMem;			//映射內存的指針
	
	
	int		(*_init_Video)();
	void	(*_restor_Video)();
}	_DRIVERINFO;

typedef	struct tag_FRAMEBUFFERINFO
{
	int		nfb;					//Framebuffer設備句柄
	int		ntty;					//TTY設備句柄
}	_FRAMEBUFFERINFO;

//初始 全部0xFFFF
typedef	struct tag_LCD_PARAMETER
{
	unsigned int signal;
	unsigned int pixclock;
	unsigned int lccr0; 
	unsigned int lccr3; 
	unsigned short xres; 
	unsigned short yres; 
	unsigned short hsync_len; 
	unsigned short vsync_len; 
	unsigned short left_margin;
	unsigned short upper_margin;
	unsigned short right_margin;
	unsigned short lower_margin;
	unsigned short sync;
	unsigned short reserved; 
} _DRIVER_LCD_PARA;

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
extern _DRIVERINFO		g_infoDrv;


#define     GR_MOR_R            0x03CC  //Miscellaneous Output Register, Read
#define     GR_MOR_W            0x03C2  //Miscellaneous Output Register, Write
#define     GR_ISR0             0x03C2  //Input Status Register 0
#define     GR_ISR1_M           0x03BA  //Input Status Register 1, Mono
#define     GR_ISR1_C           0x03DA  //Input Status Register 1, Color
#define     GR_FCR_R            0x03CA  //Feature Control Register, Read
#define     GR_FCR_WM           0x03BA  //Feature Control Register, Write, Mono
#define     GR_FCR_WC           0x03DA  //Feature Control Register, Write, Color
#define     GR_VSER             0x03C3  //Video Subsystem Enable Register
//**********
//          Sequencer Register
//**********
#define     SR_INDX             0x03C4  //Index Register
#define     SR_DATA             0x03C5  //Data  Register

#define     SR_RR               0x00    //Reset Register
#define     SR_CMR              0x01    //Clocking Mode Register
#define     SR_MAMR             0x02    //MAp Mask Register
#define     SR_CMSR             0x03    //Character Map Select Register
#define     SR_MEMR             0x04    //MEmory Mode Register
//**********
//          CRTC Register
//**********
#define     CR_INDX_M           0x03B4  //Index Register, Mono
#define     CR_DATA_M           0x03B5  //Data  Register, Mono
#define     CR_INDX_C           0x03D4  //Index Register, Color
#define     CR_DATA_C           0x03D5  //Data  Register, Color

#define     CR_HTR              0x00    //Horizontal Total Register
#define     CR_HDEER            0x01    //Horizontal Display Enable End Register
#define     CR_HSBR             0x02    //Horizontal Start Blanking Register
#define     CR_HEBR             0x03    //Horizontal End   Blanking Register
#define     CR_HSRR             0x04    //Horizontal Start Retrace  Register
#define     CR_HERR             0x05    //Horizontal End   Retrace  Register
#define     CR_VTR              0x06    //Vertical Total Register
#define     CR_OVR              0x07    //OVerflow Register
#define     CR_PRSR             0x08    //Preset Row Scan Register
#define     CR_MSLR             0x09    //Maximum Scan Line Register
#define     CR_CSR              0x0A    //Cursor Start Register
#define     CR_CER              0x0B    //Cursor End   Register
#define     CR_SAHR             0x0C    //Start Address High Register
#define     CR_SALR             0x0D    //Start Address Low  Register
#define     CR_CLHR             0x0E    //Cursor Location High Register
#define     CR_CLLR             0x0F    //Cursor Location Low  Register
#define     CR_VSRR             0x10    //Vertical Start Retrace Register
#define     CR_VERR             0x11    //Vertical End   Retrace Register
#define     CR_VDEER            0x12    //Vertical Display Enable End Register
#define     CR_OFR              0x13    //OFfset Register
#define     CR_ULR              0x14    //Underline Location Register
#define     CR_VSBR             0x15    //Vertical Start Blanking Register
#define     CR_VEBR             0x16    //Vertical End   Blanking Register
#define     CR_CMCR             0x17    //CRTC Mode Control Register
#define     CR_LCR              0x18    //Line Compare Register
//**********
//          Graphics Register
//**********
#define     GR_INDX             0x03CE  //Index Register
#define     GR_DATA             0x03CF  //Data  Register

#define     GR_SRR              0x00    //Set/Reset Register
#define     GR_ESRR             0x01    //Enable Set/Reset Register
#define     GR_CCR              0x02    //Color Compare Register
#define     GR_DRR              0x03    //Data Rotate Register
#define     GR_FSR              0x03    //Function Select Register
#define     GR_RMSR             0x04    //Read Map Select Register
#define     GR_GMR              0x05    //Graphics Mode Register
struct      tagGR_GMR
            {
            BYTE    GGMR_WRITE_MODE     :2;
            BYTE                        :1;
            BYTE    GGMR_READ_MODE      :1;
            BYTE    GGMR_ODD_EVEN       :1;
            BYTE    GGMR_SHIFT_MODE     :1;
            BYTE    GGMR_256_COLOR      :1;
            BYTE                        :1;
            };
#define     MASK_GGMR_WRITE_MODE        0x0003
#define     MASK_GGMR_READ_MODE         0x0008
#define     MASK_GGMR_ODD_EVEN          0x0010
#define     MASK_GGMR_SHIFT_MODE        0x0020
#define     MASK_GGMR_256_COLOR         0x0040
#define     GR_MR               0x06    //Miscellaneous Register
#define     GR_CDCR             0x07    //Color Don't Care Register
#define     GR_BMR              0x08    //Bit Mask Register
//**********
//          Attribute Register
//**********
#define     AR_INDX             0x03C0  //Index Register
#define     AR_DATA_R           0x03C1  //Data  Register, Read
#define     AR_DATA_W           0x03C0  //Data  Register, Write

#define     AR_PR0              0x00    //Palette Register
#define     AR_PR1              0x01    //Palette Register
#define     AR_PR2              0x02    //Palette Register
#define     AR_PR3              0x03    //Palette Register
#define     AR_PR4              0x04    //Palette Register
#define     AR_PR5              0x05    //Palette Register
#define     AR_PR6              0x06    //Palette Register
#define     AR_PR7              0x07    //Palette Register
#define     AR_PR8              0x08    //Palette Register
#define     AR_PR9              0x09    //Palette Register
#define     AR_PRA              0x0A    //Palette Register
#define     AR_PRB              0x0B    //Palette Register
#define     AR_PRC              0x0C    //Palette Register
#define     AR_PRD              0x0D    //Palette Register
#define     AR_PRE              0x0E    //Palette Register
#define     AR_PRF              0x0F    //Palette Register
#define     AR_MCR              0x10    //Mode Control Register
#define     AR_OCR              0x11    //Overscan Color Register
#define     AR_CPER             0x12    //Color Plane Enable Register
#define     AR_HPPR             0x13    //Horizontal PEL Panning Register
#define     AR_CSR              0x14    //Color Select Register
//**********
//          RAM DAC Register
//**********
#define     RD_MASK             0x03C6  //Mask  Register
#define     RD_INDX_R           0x03C7  //Index Register, Read
#define     RD_INDX_W           0x03C8  //Index Register, Write
#define     RD_DATA             0x03C9  //Data  Register
//**********
//          GDC Segment Select Register - ET4000
//**********
#define     GD_SSR              0x03CD  //Segment Select Register

//          Gray Color Summing
//          I = R * 0.3 + G * 0.59 + B * 0.11

/*---------------------------------------------------------------------------+
;|          Memory-mapped I/O segment address                                |
;+--------------------------------------------------------------------------*/
//**********
//
//**********
#define     VSEGMENT            0xA000  //Video memory segment address

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
/*---------------------------------------------------------------------------+
;| Function : void _SetFont(FONT font)                                       |
;| Task     : Set font attribution                                           |
;+---------------------------------------------------------------------------+
;| Call     : _SetFont(font)                                                 |
;|                                                                           |
;| Parameter: font                      - Font attribution                   |
;|                                                                           |
;| Return   :                           -                                    |
;+--------------------------------------------------------------------------*/
//id __far  _SetFont(FONT font);

/*---------------------------------------------------------------------------+
;| Function : void _SetFontColor(FGC fgc)                                    |
;| Task     : Set font color                                                 |
;+---------------------------------------------------------------------------+
;| Call     : _SetFontColor(fgc)                                             |
;|                                                                           |
;| Parameter: fgc                       - Font color : ForeGround Color      |
;|                                                                           |
;| Return   :                           -                                    |
;+--------------------------------------------------------------------------*/
void __far  _SetFontColor(FGC fgc);

/*---------------------------------------------------------------------------+
;| Function : void _ShowFont(short        nX,                                |
;|                           short        nY,                                |
;|                           BYTE __far*  fpsFont,                           |
;|                           SIZE         size)                              |
;| Task     : Show a bit map font to screen                                  |
;|            Font width <= 24 pixels                                        |
;|            Font color <= 16 colors                                        |
;+---------------------------------------------------------------------------+
;| Call     : _ShowFont(nX, nY, fpsFont, font)                               |
;|                                                                           |
;| Parameter: nX                        - Upper left X position              |
;|            nY                        - Upper left Y position              |
;|            fpsFont                   - Pointer to bit map font pattern    |
;|            size                      - Font Attribution                   |
;|                                                                           |
;| Return   :                           -                                    |
;+--------------------------------------------------------------------------*/
void __far  _ShowFont(short       nX,
                      short       nY,
                      BYTE __far* fpsFont,
                      tmSIZE        size);

/*---------------------------------------------------------------------------+
;| Function :                                                                |
;| Task     :                                                                |
;+---------------------------------------------------------------------------+
;| Call     :                                                                |
;|                                                                           |
;| Parameter:                           -                                    |
;|                                                                           |
;| Return   :                           -                                    |
;+--------------------------------------------------------------------------*/
void __far	_Init_FB_Parameter(int	nHeight, int nWidth);
void __far	_Write_LCD_Parameter();
void __far  _VideoPowerOn();
void __far  _VideoPowerOff();
//#ifdef  D_LCDB    //Anny2002/10/11 DELETE
void __far  _VideoBrighter(BYTE Brighter);       //wy042400
//#endif
void __far  _IOTest(BYTE Data);       //Bw061902

extern		_FRAMEBUFFERINFO		_fbData;
extern	void	ChangePos(int app_update=0);
extern	BOOL  RegionUpdateStop;
extern	BOOL  RegionUpdateStop_APP;
extern	void	ChangePosNow(RECT rect);
extern	void	ChangePos(int x1,int y1,int x2,int y2);
extern	void	ChangePosRegionSum(RECT rect);
extern	void	ChangePosRegion();
extern 	XYRANGE		g_XYRange;
extern	int	g_Rotation;
extern	int	g_nCommand;
//extern	float	g_nTouchView;		
extern	int	g_nTouchView;		
extern	int	g_nTouchX;
extern	int	g_nTouchY;
extern	RECT	MsgBoxRegion;
extern	void	EnableMouseCursor(int x,int y);
extern	void	UnEnableMouseCursor();
extern	int		MouseMoveStep;
#endif
