/*==========================================================================+
|  Class    : Class TouchScreen	                                            |
|  Task     : Class TouchScreen file                          				|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------| 
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|           : V2.00   James                                                 |
|  Creation : 2006/12/21                                                     |
|  Revision : 2006/12/21                                                    |
|           : 2010/01/25    V2.00                                           |
+==========================================================================*/
#include 	"tmTouchScreen.h"
#include 	"../../main.h"
#include 	"../../tmconfig.h"
#include 	"../../utils.h"
#include	"../../cursor.h"
#include  "../../taskmoni.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		TOUCH_TOKEN_END					0x0000
#define		TOUCH_TOKEN_START				0x0001

#define     TOUCH_COLDREST          		0x81
#define     TOUCH_SETRATE          			0x8B

#define		TOUCH_DOWN						0x01
#define		TOUCH_UP						0x02
#define     TOUCH_MOVE              		0x03

#define		TOUCH_REVERSE_X					0x01
#define		TOUCH_REVERSE_Y					0x02
#define		TOUCH_CHANGE_XY					0x04

#define     TOUCH_STATUS_COORDINATE_MASK	0xB0
#define     TOUCH_STATUS_COORDINATE		    0x80

//Mouse message
#define		MOUSE_LEFT_CLICK				0x01
#define		MOUSE_RIGHT_CLICK				0x02
#define		MOUSE_MIDDLE_CLICK				0x04

// Recive pacekt
#define 	FIRSTDATA						0
#define 	COORDINATE						1
#define 	CMDRESPONSE						2

#define		TOUCH_MAXBITPOINT				4095

#define		TOUCH_MAX_DIFF					150//40

#define WORD_ASSEMBLY(byte1, byte2)	(((byte2) << 7) | (byte1))
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
#define		TS_DEV_NAME					"/dev/ads7843"
#define 	USB_MOUSE3   				"/dev/input/event0"
#define 	USB_MOUSE   				"/dev/input/event1"
#define 	USB_MOUSE2   				"/dev/mouse"
#define 	USB_MOUSE5  				"/dev/input/event0"
#define		USB_MOUSE4					"/dev/input/event0"//"/dev/input/touchscreen0"

#define		TS_XCHANGE_XY					0x0110
#define		TS_REVERSE_X					0x0111
#define		TS_REVERSE_Y					0x0112
#define		TS_RES_X						0x0102
#define		TS_RES_Y						0x0103
#define		TS_MAX_X						0x0100
#define		TS_MAX_Y						0x0101
#define		TS_MIN_X						0x0104
#define		TS_MIN_Y						0x0105
#define		TS_CALCULATE					0x0113
#define		TS_DATA_SIGN					0x12345670
#if defined(D_ARM) && !defined(D_PXA270)
#define		TS_DATA_FLASH					"/dev/mtdblock1"
#else
#define		TS_DATA_FLASH					"/dev/mtdblock4"
#endif
#define		TS_DATA_POSI					64

int		g_nWaitTime = 0;
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
typedef	struct pmtouch
{
 	long 	lTime;
 	long	lSecond;
 	WORD	wType;
 	WORD	wCode;
 	DWORD	dwValue;
}PMTOUCH;
PMTOUCH	u_PMTouch;

typedef	struct untiecTouch
{
	long	lTime;
	long	lSecond;
	WORD	wType;
 	WORD	wCode;
 	DWORD	dwValue;		
}UNTICTOUCH;
UNTICTOUCH	u_UNtiecTouch;
//
//typedef	struct ts_sample {
//	int		x;
//	int		y;
//	unsigned int	pressure;
//	struct timeval	tv;
//}TS_SAMPLE;
//

TSSAVE	g_TSSave;
int		g_TSAdjustState;
int		m_nMouseFd = -1;
BOOL	g_bMouseDown = FALSE;

pthread_t		m_TSReadthread;
pthread_t		m_MouseReadthread;
pthread_t		m_MouseReadthread_State;

extern		int					g_nViewChange; 
void	CalculatePosition(touchscreen *buffin, touchscreen *buffout);
void 	Calculate_AtomPosition(touchscreen *buffin, touchscreen *buffout);
/*==========================================================================+
|           Class implementation - CtmTouchScreen                           |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmTouchScreen, CTouchScreen)

CtmTouchScreen::CtmTouchScreen() : CTouchScreen()
{
	//printf("CtmTouchScreen::Constructor\n");
}

CtmTouchScreen::~CtmTouchScreen()
{

}

int		CtmTouchScreen::CreateSelf()
{
	CtmPackClass::CreateSelf();
		
	m_nTokenState			= TOUCH_TOKEN_END;
	m_nState				= TOUCH_UP;

    m_TouchData.nDataPos    = 0;
    m_TouchData.nDataType   = 0;
	
	m_dXScale   = 1.0;
	m_dYScale   = 1.0;
	
	memset(&m_TSRun, 0, 	sizeof(m_TSRun));
	Create();
	
	return 0;
	
}

int		CtmTouchScreen::FreeSelf()
{
	Close();	
	CtmPackClass::FreeSelf();
	return 0;
}

int		CtmTouchScreen::Create(void* pDevice)
{
	//printf("touchpanel type:%d\n", CtmConfig::GetInstance()->GetTouchPanlType());
	if(CtmConfig::GetInstance()->GetTouchPanlType() & 2)
	{
	    m_rs232.wChannel            = _COM1;			//	fans modify for TW
		m_rs232.config.DataLength   = COM_DL_8;
		m_rs232.config.StopBit      = COM_SB_1;
		m_rs232.config.Parity       = COM_PR_NONE;
		m_rs232.config.BaudRate     = COM_BR_9600;
		m_rs232.config.Handshake    = COM_HS_NONE;     //Chuntzu 2004/8/18 all use handshake none //Chuntzu 2004/8/5 add
	
		m_rs232.wRxBufferSize   	= 0;
		m_rs232.fpRxBuffer      	= NULL;
		m_rs232.wTxBufferSize   	= 16;
		m_rs232.fpTxBuffer      	= m_acTxBuffer;
		m_rs232.pRSFunc         	= Touch_RS232Func;
		m_rs232.pClass          	= this;
	
		m_pDevice	= (CDevice *)::Create("CRS232");
		if(m_pDevice == NULL)	
		{
			return DEVICE_ERR_DEVICE;
		}
	}
	#ifdef D_PXA270
	ReadTSParameter(&g_TSSave);
					
	//if (g_TSSave.tssignal != TS_DATA_SIGN)
	//{
	//	memset(&g_TSSave, 0, sizeof(g_TSSave));
	//	
	//	g_TSSave.tsmax.x 	= 1950;//1855;//1964;	//1910;
	//	g_TSSave.tsmax.y 	= 1940;//1952;//1916;	//1910;
	//	g_TSSave.tsmin.x 	= 45;//100;//68;	//115;
	//	g_TSSave.tsmin.y 	= 77;//66;//77;	//115;
	//	g_TSSave.tsdelta.x 	= 0;
	//	g_TSSave.tsdelta.y 	= 0;
	//	g_TSSave.tsangle 	= 2;//TOUCH_CHANGE_XY + TOUCH_REVERSE_X + TOUCH_REVERSE_Y;
	//	g_TSSave.tspixel.x 	= g_infoDrv.nX;	//800;
	//	g_TSSave.tspixel.y 	= g_infoDrv.nY;	//600;
	//	g_TSSave.tssignal	= TS_DATA_SIGN;
	//	g_TSSave.tscalculate= 0;
	//	
	//	//the default ts parameters for 5 points method
	//	g_TSSave.tscalib.tsBase.x = 157;//1840;
	//	g_TSSave.tscalib.tsBase.y = 1786; //1735;
	//	g_TSSave.tscalib.lcdBase.x = 50;
	//	g_TSSave.tscalib.lcdBase.y = 50;
	//	g_TSSave.tscalib.dKx = 2.390714; //-2.372857;
	//	g_TSSave.tscalib.dKy = -3.094000;//-2.962000;
	//	
	//	m_TSAdjust		 	= 1;
	//	
	//	//write to ts para to flash
	//	//SaveTSParameter(g_TSSave);
	//}
	#endif
	
	int	nTouchType = CtmConfig::GetInstance()->GetTouchPanlType();
	if (nTouchType > 0)
	{
		memset(&g_TSSave, 0, sizeof(g_TSSave));
		//printf("nTouchType=%x \n", nTouchType);
		if (nTouchType == 0x1001)		//4 will atom 12.1'
			m_TouchIni.OpenFile("/work/touch4_config.ini");
		else if (nTouchType == 0x2001) 	//5 will atom 12.1' 
			m_TouchIni.OpenFile("/work/touch5_config.ini");
		else if(nTouchType == 0x0004)	//4 will 270 12.1
			m_TouchIni.OpenFile("/usr/touch4_270_config.ini");
		else if(nTouchType == 0x0008)	//4 will 270 12.1
			m_TouchIni.OpenFile("/usr/touchm_3354_config.ini");
		else if(nTouchType == 0x0010)	//4 will 270 12.1
			m_TouchIni.OpenFile("/usr/touchm_3354_5_config.ini");
		else
			m_TouchIni.OpenFile("/work/touch_config.ini");
			
		g_TSSave.tsmax.x = m_TouchIni.ReadInteger("adjust", "tsmax.x", 0);
		g_TSSave.tsmax.y = m_TouchIni.ReadInteger("adjust", "tsmax.y", 0);
		g_TSSave.tsmin.x = m_TouchIni.ReadInteger("adjust", "tsmin.x", 0);
		g_TSSave.tsmin.y = m_TouchIni.ReadInteger("adjust", "tsmin.y", 0);
		g_TSSave.tsdelta.x 	= m_TouchIni.ReadInteger("adjust", "tsdelta.x", 0);
		g_TSSave.tsdelta.y 	= m_TouchIni.ReadInteger("adjust", "tsdelta.y", 0);
		g_TSSave.tsangle	= m_TouchIni.ReadInteger("adjust", "tsangle", 0);
		g_TSSave.tspixel.x	= m_TouchIni.ReadInteger("adjust", "tspixel.x", 0);
		g_TSSave.tspixel.y	= m_TouchIni.ReadInteger("adjust", "tspixel.y", 0);
		g_TSSave.tssignal	= m_TouchIni.ReadInteger("adjust", "tssignal", 0);
		
		g_TSSave.tscalib.tsBase.x	= m_TouchIni.ReadInteger("tscalib", "tsBase.x", 0);
		g_TSSave.tscalib.tsBase.y	= m_TouchIni.ReadInteger("tscalib", "tsBase.y", 0);
		g_TSSave.tscalib.lcdBase.x	= m_TouchIni.ReadInteger("tscalib", "lcdBase.x", 0);
		g_TSSave.tscalib.lcdBase.y	= m_TouchIni.ReadInteger("tscalib", "lcdBase.y", 0);
		g_TSSave.tscalib.dKx		= (float)m_TouchIni.ReadInteger("tscalib", "dKx", 0)/1000000;
		g_TSSave.tscalib.dKy		= (float)m_TouchIni.ReadInteger("tscalib", "dKy", 0)/1000000;
		
		//printf("max.x=%d, max.y=%d, min.x=%d, min.y=%d delta.x=%d, delta.y=%d tsangle=%d,pixel.x=%d, pixel.y=%d, signal=%x \n",
		// g_TSSave.tsmax.x, g_TSSave.tsmax.y, g_TSSave.tsmin.x, g_TSSave.tsmin.y,
		// g_TSSave.tsdelta.x, g_TSSave.tsdelta.y, g_TSSave.tsangle, g_TSSave.tspixel.x,
		// g_TSSave.tspixel.y, g_TSSave.tssignal);
		// printf(" base.x=%d, base.y=%d, lcdbase.x=%d, lcdbase.y=%d, kx=%f, ky=%f  \n"
		// , g_TSSave.tscalib.tsBase.x, g_TSSave.tscalib.tsBase.y,
		// g_TSSave.tscalib.lcdBase.x, g_TSSave.tscalib.lcdBase.y, g_TSSave.tscalib.dKx, g_TSSave.tscalib.dKy);
		m_TouchIni.CloseFile();
//		
//		g_TSSave.tsmax.x 	= 1000;//1855;//1964;	//1910;
//		g_TSSave.tsmax.y 	= 1000;//1952;//1916;	//1910;
//		g_TSSave.tsmin.x 	= 75;//100;//68;	//115;
//		g_TSSave.tsmin.y 	= 75;//66;//77;	//115;
//		g_TSSave.tsdelta.x 	= 0;
//		g_TSSave.tsdelta.y 	= 0;
//		g_TSSave.tsangle 	= 0;//2;//TOUCH_CHANGE_XY + TOUCH_REVERSE_X + TOUCH_REVERSE_Y;
//		g_TSSave.tspixel.x 	= g_infoDrv.nX;	//800;
//		g_TSSave.tspixel.y 	= g_infoDrv.nY;	//600;
//		g_TSSave.tssignal	= TS_DATA_SIGN;
//		g_TSSave.tscalculate= 0;
//		
//		//the default ts parameters for 5 points method
//		g_TSSave.tscalib.tsBase.x = 130;//1840;
//		g_TSSave.tscalib.tsBase.y = 149; //1735;
//		g_TSSave.tscalib.lcdBase.x = 50;
//		g_TSSave.tscalib.lcdBase.y = 50;
//		g_TSSave.tscalib.dKx = 1.099286; //-2.372857;
//		g_TSSave.tscalib.dKy = 1.458000;//-2.962000;
//		
		m_TSAdjust		 	= 1;	
	}
	//else
	//{
	//	m_TSAdjust		 	= 0;		//no use
	//}
	//g_TSSave.tsangle 	= 2;			//const
	//set driver para
	if(g_TSSave.tspixel.x == 0)
		g_TSSave.tspixel.x = g_infoDrv.nX;
	if(g_TSSave.tspixel.y == 0)
		g_TSSave.tspixel.y = g_infoDrv.nY;
	//}		
	//printf("g_TSSave.tspixel.x=%d y=%d \n", g_TSSave.tspixel.x, g_TSSave.tspixel.y);
	m_TSRun.tsdata.x    = g_TSSave.tspixel.x;
	m_TSRun.tsdata.y    = g_TSSave.tspixel.y;
	SetOffset(g_TSSave.tsdelta.x,  g_TSSave.tsdelta.y);
	
	Open();
    return DEVICE_SUCCESS;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	CtmTouchScreen::Open()
{
	int nTouchType = 0;
	
	nTouchType = CtmConfig::GetInstance()->GetTouchPanlType();
		
	if(nTouchType & 2)
	{
		printf("open rs232\n");
		Open_RS232();
	}
	else if (nTouchType & 4)
	{
		printf("open ads7843\n");
		Open_Ads7843();
	}
	//printf("nTouchType = %x\n",nTouchType);
	/*if (nTouchType & 1)
	{
		printf("open usb mouse\n");
		Open_UsbMouse();
	}
	else if(nTouchType & 8)
	{
		Open_UsbMouse2();	
	}
	else if(nTouchType & 16)*/
	//{
		//printf("Open_UsbMouse4\n");
		Open_UsbMouse4();	
	//}
	return DEVICE_SUCCESS;
}

/*---------------------------------------------------------------------------+
|  Function : CtmTouchScreen::Open_RS232()                                   |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	CtmTouchScreen::Open_RS232()
{
	BYTE    bCmdPacket[8];
	int     nWriteCount;
		
	m_pDevice->Create(&m_rs232);
	
	//=======
	// Send Cold Rest Command
	//=======
	memset(bCmdPacket, 0, 8);
	bCmdPacket[5]   = TOUCH_COLDREST;
	m_pDevice->Write((BYTE *)bCmdPacket, 6, &nWriteCount);
	
	memset(bCmdPacket, 0, 8);
	bCmdPacket[5]   = TOUCH_SETRATE;
	bCmdPacket[6]   = 5;
	m_pDevice->Write((BYTE *)bCmdPacket, 7, &nWriteCount);
	
	
	m_nX    = 399;
	m_nY    = 299;
	
	return DEVICE_SUCCESS;
}

/*---------------------------------------------------------------------------+
|  Function : CtmTouchScreen::Open_Ads7843()                                   |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	CtmTouchScreen::Open_Ads7843()
{
	int		ret;
	m_TSRun.tshandle = open(TS_DEV_NAME, O_RDWR);
	
	if (m_TSRun.tshandle <= 0)
	{
		//printf("open ads7843 error\n");
		return DEVICE_ERR_DEVICE;
	}
	ret = pthread_create(&m_TSReadthread, NULL, (PRSRUN)Touch_ADS7843Func, &m_TSRun);
	if (ret)	return DEVICE_ERR_DEVICE;
	
	ioctl(m_TSRun.tshandle, TS_MAX_X, g_TSSave.tsmax.x);
	ioctl(m_TSRun.tshandle, TS_MAX_Y, g_TSSave.tsmax.y);
	ioctl(m_TSRun.tshandle, TS_MIN_X, g_TSSave.tsmin.x);
	ioctl(m_TSRun.tshandle, TS_MIN_Y, g_TSSave.tsmin.y);
	
	ioctl(m_TSRun.tshandle, TS_CALCULATE, g_TSSave.tscalculate);
	ioctl(m_TSRun.tshandle, TS_RES_X, g_TSSave.tspixel.x);
	ioctl(m_TSRun.tshandle, TS_RES_Y, g_TSSave.tspixel.y);

	SetAngle(g_TSSave.tsangle);
	return DEVICE_SUCCESS;	
}

WORD	CtmTouchScreen::Open_UsbMouse()
{
	int		ret;
	printf("USB_MOUSE5 = %s\n",USB_MOUSE5);
	if((m_nMouseFd = open(USB_MOUSE5, O_RDONLY)) == -1)
	{
        printf("Failed to open %s\n", USB_MOUSE);
        if ((m_nMouseFd = open(USB_MOUSE2, O_RDONLY)) == -1)
        {
        	printf("Failed to open %s\n", USB_MOUSE2);
        	 if ((m_nMouseFd = open(USB_MOUSE3, O_RDONLY)) == -1)
		        {
		        	printf("Failed to open %s\n", USB_MOUSE3);
		        	return DEVICE_ERR_DEVICE;
		        }
		        else
		        {
		        	printf("m_nMouseFd=%d USB_MOUSE3=%s \n", m_nMouseFd, USB_MOUSE3);	
		        }
        	return DEVICE_ERR_DEVICE;
        }
    }
    ret = pthread_create(&m_MouseReadthread, NULL, (PRSRUN)USB_MouseFunc, NULL);
    if (ret)	return DEVICE_ERR_DEVICE;
    return DEVICE_SUCCESS;	
}


WORD	CtmTouchScreen::Open_UsbMouse2()
{
	int		ret, ret2;
	
	if((m_nMouseFd= open(USB_MOUSE3, O_RDONLY)) == -1)
	{
        printf("Failed to open %s\n", USB_MOUSE3);
        if ((m_nMouseFd = open(USB_MOUSE, O_RDONLY)) == -1)
        {
        	printf("Failed to open %s\n", USB_MOUSE2);
        	 //if ((m_nMouseFd = open(USB_MOUSE3, O_RDONLY)) == -1)
		     //   {
		     //   	printf("Failed to open %s\n", USB_MOUSE3);
		     //   	return DEVICE_ERR_DEVICE;
		     //   }
		     //   else
		     //   {
		     //   	printf("m_nMouseFd=%d USB_MOUSE3=%s \n", m_nMouseFd, USB_MOUSE3);	
		     //   }
        	;//return DEVICE_ERR_DEVICE;
        }
    }
    printf("m_nMouseFd =%d  \n", m_nMouseFd);
    ret = pthread_create(&m_MouseReadthread, NULL, (PRSRUN)USB_MouseFunc2, NULL);
    ret2 = pthread_create(&m_MouseReadthread_State, NULL, (PRSRUN)USB_MouseFunc2_Check, NULL);
    if (ret)	return DEVICE_ERR_DEVICE;
    if (ret2)	return DEVICE_ERR_DEVICE;
    return DEVICE_SUCCESS;		
}

WORD	CtmTouchScreen::Open_UsbMouse4()
{
	int		ret;
	
	m_TSRun.tshandle = open(USB_MOUSE4, O_RDONLY);
	
	if (m_TSRun.tshandle <= 0)
	{
		//printf("open Open_UsbMouse4 error\n");
		return DEVICE_ERR_DEVICE;
	}
	ret = pthread_create(&m_TSReadthread, NULL, (PRSRUN)USB_MouseFunc4, &m_TSRun);
	if (ret)	return DEVICE_ERR_DEVICE;
	
    return DEVICE_SUCCESS;		
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	    CtmTouchScreen::Close()
{
	if(m_pDevice != NULL)
	{
		delete m_pDevice;
		m_pDevice	= NULL;
	}
	if (m_TSRun.tshandle > 0)
		close(m_TSRun.tshandle);
	if(m_nMouseFd > 0)
		close(m_nMouseFd);
	return DEVICE_SUCCESS;
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    CtmTouchScreen::SetAngle(int nAngle)
{
    //bit0 = x; bit1 = y; bit2 =x,y swap
    if(_TestBit(nAngle, 0))
    	ioctl(m_TSRun.tshandle, TS_REVERSE_X, 1);
    else
    	ioctl(m_TSRun.tshandle, TS_REVERSE_X, 0);
    
    if(_TestBit(nAngle, 1))
    	ioctl(m_TSRun.tshandle, TS_REVERSE_Y, 1);
    else
    	ioctl(m_TSRun.tshandle, TS_REVERSE_Y, 0);
    
    if(_TestBit(nAngle, 2))
    	ioctl(m_TSRun.tshandle, TS_XCHANGE_XY, 1);	
    else
    	ioctl(m_TSRun.tshandle, TS_XCHANGE_XY, 0);	
}

/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    CtmTouchScreen::SetOffset(int nX, int nY)
{
    m_TSRun.tsdelta.x += nX;
    m_TSRun.tsdelta.y += nY;
    g_TSSave.tsdelta.x = m_TSRun.tsdelta.x;
    g_TSSave.tsdelta.y = m_TSRun.tsdelta.y;
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTouchScreen::SetScale(int nCoor, double dScale)       |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    CtmTouchScreen::SetScale(int nCoor, double dScale)
{
    if(nCoor == 0)
    	m_dXScale  *= dScale;
    if(nCoor == 1)
    	m_dYScale  *= dScale;
}

BOOL	CtmTouchScreen::SetAdjustState(int	nState)
{
	g_TSAdjustState = nState;
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTouchScreen::SetAdjustData(void*	pData)			     |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTouchScreen::SetAdjustData(void*	pData)
{
	XY	max, min;
	int	nAnagle = 0, nResult = 0, nMaxIndex = 0, nMinIndex = 0, nTempMaxValue = 0, nTempMinValue = 0;
	TSSAVE	TSTempSave;
	//==================================================
	//step1: calculate x,y Max
	//step2: calculate x,y Min
	//step3: calculate Anage(x change or y change or x swap y)
	//step4: save data
	//==================================================
	XY* Data = (XY*)pData;
	memset(&TSTempSave, 0, sizeof(TSTempSave));
	//step1-2 :calculate x,y Max calculate x,y Min
	min.x = max.x = Data[0].x;
	min.y = max.y = Data[0].y;
	for(int i = 1 ; i < 5; i++)
	{
		//printf("Data[%d].x=%d, Data[%d].y=%d", i, Data[i].x,i, Data[i].y);
		if(max.x < Data[i].x)
			max.x = Data[i].x;
		if(min.x > Data[i].x)
			min.x = Data[i].x;
		if(max.y < Data[i].y)
			max.y = Data[i].y;
		if(min.y > Data[i].y)
			min.y = Data[i].y;
	}
	
	//calculate max index and min index
	nMinIndex = nMaxIndex = 0;
	nTempMinValue = nTempMaxValue = max.x - Data[0].x + max.y - Data[0].y;
	for(int i = 1 ; i < 5; i++)
	{
		if(nTempMinValue < (max.x - Data[i].x + max.y - Data[i].y))	
		{
			nTempMinValue = max.x - Data[i].x + max.y - Data[i].y;
			nMinIndex = i;
		}
		if(nTempMaxValue > (max.x - Data[i].x + max.y - Data[i].y))
		{
			nTempMaxValue = max.x - Data[i].x + max.y - Data[i].y;
			nMaxIndex = i;
		}
	}
	max.x = max.x + (max.x-min.x)/(g_infoDrv.nX) * 10;
	min.x = (min.x - (max.x-min.x)/(g_infoDrv.nX) * 10) > 0 ? 
			(min.x - (max.x-min.x)/(g_infoDrv.nX) * 10) : 0;
	
	max.y = max.y + (max.y-min.y)/(g_infoDrv.nY) * 10;
	min.y = (min.y - (max.y-min.y)/(g_infoDrv.nY) * 10) > 0 ? 
			(min.y - (max.y-min.y)/(g_infoDrv.nY) * 10) : 0;
	//step3:
	//check x,y ned swap?
	if(abs(Data[1].x-Data[4].x) < abs(Data[1].x - Data[2].x))	//x, y need swap
	{
		nAnagle |= 0x04;
	}
	
	switch(nMaxIndex)
	{
		case 0:
			return 1;			//Error
			break;
		case 1:					//x,y need change
			nAnagle |= 0x01;
			nAnagle |= 0x02;
			break;
		case 2:					//x need change
			nAnagle |= 0x01;
			break;
		case 3:					//not need
			break;
		case 4:					//y need change
			nAnagle |= 0x02;
			break;
	}
	TSTempSave.tssignal		= TS_DATA_SIGN;
	TSTempSave.tsmax.x		= max.x;
	TSTempSave.tsmax.y		= max.y;
	TSTempSave.tsmin.x		= min.x;
	TSTempSave.tsmin.y		= min.y;
	TSTempSave.tsdelta.x 	= 0;
	TSTempSave.tsdelta.y 	= 0;
	TSTempSave.tspixel.x 	= g_infoDrv.nX;
	TSTempSave.tspixel.y 	= g_infoDrv.nY;
	TSTempSave.tscalculate	= 0;
	TSTempSave.tsangle 		= nAnagle;
	
	//printf("nMaxIndex=%d nMinIndex=%d\n", nMaxIndex, nMinIndex);
	printf("signal=%d max.x=%d, max.y=%d, min.x=%d. min.y=%d, delta.x=%d,delta.y=%d, pixel.x=%d, pixel.y=%d, tsca=%d, tsangle=%d \n", 
	TSTempSave.tssignal, TSTempSave.tsmax.x, TSTempSave.tsmax.y, TSTempSave.tsmin.x, TSTempSave.tsmin.y,
	TSTempSave.tsdelta.x, TSTempSave.tsdelta.y, TSTempSave.tspixel.x, TSTempSave.tspixel.y, TSTempSave.tscalculate,
	TSTempSave.tsangle);
	
	memcpy(&g_TSSave, &TSTempSave, sizeof(TSTempSave));
	SaveTSParameter(g_TSSave);
	
	return nResult;
}

int		CtmTouchScreen::SetAdjustData(XY *lcdPoints, XY *tsPoints, int nCount)
{
	int ret = 0;
	
	if (nCount == 3)		//three points method
		ret = SetCalibration3(nCount, lcdPoints, tsPoints, &g_TSSave.tsmatrix);
	else					//five points method
		ret = SetCalibration5(nCount, lcdPoints, tsPoints, &g_TSSave.tscalib);
	
	if (ret == 0)
		SaveTSParameter(g_TSSave);
	
	return ret;
}

int		CtmTouchScreen::SetCalibration3(int nCount, XY *lcdPoints, XY *tsPoints, TSMATRIX *matrixPtr)
{
	int  retValue = 0;
	/*
	printf("Calibration:[(%d,%d)-(%d,%d)], [(%d,%d)-(%d,%d)],[(%d,%d)-(%d,%d)]\n", 
			lcdPoints[0].x,lcdPoints[0].y, tsPoints[0].x, tsPoints[0].y,
			lcdPoints[1].x,lcdPoints[1].y, tsPoints[1].x, tsPoints[1].y,
			lcdPoints[2].x,lcdPoints[2].y, tsPoints[2].x, tsPoints[2].y);
	*/		 
    matrixPtr->sDivisor = ((tsPoints[0].x - tsPoints[2].x) * (tsPoints[1].y - tsPoints[2].y)) -
                         ((tsPoints[1].x - tsPoints[2].x) * (tsPoints[0].y - tsPoints[2].y)) ;

    if (matrixPtr->sDivisor == 0)
    {
        retValue = -1 ;
    }
    else
    {
        matrixPtr->sA = ((lcdPoints[0].x - lcdPoints[2].x) * (tsPoints[1].y - tsPoints[2].y)) -
                        ((lcdPoints[1].x - lcdPoints[2].x) * (tsPoints[0].y - tsPoints[2].y)) ;

        matrixPtr->sB = ((tsPoints[0].x - tsPoints[2].x) * (lcdPoints[1].x - lcdPoints[2].x)) -
                        ((lcdPoints[0].x - lcdPoints[2].x) * (tsPoints[1].x - tsPoints[2].x)) ;

        matrixPtr->sC = (tsPoints[2].x * lcdPoints[1].x - tsPoints[1].x * lcdPoints[2].x) * tsPoints[0].y +
                        (tsPoints[0].x * lcdPoints[2].x - tsPoints[2].x * lcdPoints[0].x) * tsPoints[1].y +
                        (tsPoints[1].x * lcdPoints[0].x - tsPoints[0].x * lcdPoints[1].x) * tsPoints[2].y ;

        matrixPtr->sD = ((lcdPoints[0].y - lcdPoints[2].y) * (tsPoints[1].y - tsPoints[2].y)) -
                        ((lcdPoints[1].y - lcdPoints[2].y) * (tsPoints[0].y - tsPoints[2].y)) ;

        matrixPtr->sE = ((tsPoints[0].x - tsPoints[2].x) * (lcdPoints[1].y - lcdPoints[2].y)) -
                        ((lcdPoints[0].y - lcdPoints[2].y) * (tsPoints[1].x - tsPoints[2].x)) ;

        matrixPtr->sF = (tsPoints[2].x * lcdPoints[1].y - tsPoints[1].x * lcdPoints[2].y) * tsPoints[0].y +
                        (tsPoints[0].x * lcdPoints[2].y - tsPoints[2].x * lcdPoints[0].y) * tsPoints[1].y +
                        (tsPoints[1].x * lcdPoints[0].y - tsPoints[0].x * lcdPoints[1].y) * tsPoints[2].y ;
    }
	/*
	printf("A:%d B:%d C:%d D:%d E:%d F:%d divisor:%d\n",
			matrixPtr->sA, matrixPtr->sB, matrixPtr->sC, 
			matrixPtr->sD, matrixPtr->sE, matrixPtr->sF, matrixPtr->sDivisor);
	*/	
    return retValue;
}


void	CtmTouchScreen::SwapXY(XY *point)
{
	short tmp;
	
	tmp = point->x;
	point->x = point->y;
	point->y = tmp;
}
int		CtmTouchScreen::SetCalibration5(int nCount, XY *lcdPoints, XY *tsPoints, TSCALIB *calibPtr)
{
	int nDiff[4], i = 0;
	XY	point;
	TSCALIB tsCalibTmp;
	
	g_TSSave.tsangle = 0;
	memset(&tsCalibTmp, 0, sizeof(TSCALIB));
	/*
	printf("Calibration:\n"
			"A(%4d,%4d)-(%4d,%4d)\n"
			"B(%4d,%4d)-(%4d,%4d)\n"
			"C(%4d,%4d)-(%4d,%4d)\n"
			"D(%4d,%4d)-(%4d,%4d)\n"
			"E(%4d,%4d)-(%4d,%4d)\n", 
			lcdPoints[0].x,lcdPoints[0].y, tsPoints[0].x, tsPoints[0].y,
			lcdPoints[1].x,lcdPoints[1].y, tsPoints[1].x, tsPoints[1].y,
			lcdPoints[2].x,lcdPoints[2].y, tsPoints[2].x, tsPoints[2].y,
			lcdPoints[3].x,lcdPoints[3].y, tsPoints[3].x, tsPoints[3].y,
			lcdPoints[4].x,lcdPoints[4].y, tsPoints[4].x, tsPoints[4].y);
	*/		
	//point 0 and point1 must be at the same level
	if (abs(tsPoints[0].x - tsPoints[1].x) < abs(tsPoints[0].y - tsPoints[1].y))
	{
		g_TSSave.tsangle |= TOUCH_CHANGE_XY;		//swap x, y
		//printf("swap x y\n");
		for (i = 0; i < nCount; i++)
			SwapXY(&tsPoints[i]);
	}
	
	if (tsPoints[0].x > tsPoints[1].x)
		g_TSSave.tsangle |= TOUCH_REVERSE_X;
	if (tsPoints[0].y > tsPoints[2].y)
		g_TSSave.tsangle |= TOUCH_REVERSE_Y;
		
	
	//printf("angle:%d\n", g_TSSave.tsangle);
	
	nDiff[0] = tsPoints[0].y - tsPoints[1].y;
	nDiff[1] = tsPoints[2].y - tsPoints[3].y;
	nDiff[2] = tsPoints[0].x - tsPoints[2].x;
	nDiff[3] = tsPoints[1].x - tsPoints[3].x;
	printf("diff:%d, %d, %d, %d\n", nDiff[0], nDiff[1], nDiff[2], nDiff[3]);
	
	if (abs(nDiff[0]) > TOUCH_MAX_DIFF || abs(nDiff[1]) > TOUCH_MAX_DIFF ||
		abs(nDiff[2]) > TOUCH_MAX_DIFF || abs(nDiff[3]) > TOUCH_MAX_DIFF)
	{
		printf("exceed max error\n");
		return -1;
	}
	
	tsCalibTmp.dKx = ((double)(tsPoints[1].x - tsPoints[0].x) / (double)(lcdPoints[1].x - lcdPoints[0].x) + 
				   (double)(tsPoints[3].x - tsPoints[2].x) / (double)(lcdPoints[3].x - lcdPoints[2].x)) / 2;
	tsCalibTmp.dKy = ((double)(tsPoints[2].y - tsPoints[0].y) / (double)(lcdPoints[2].y - lcdPoints[0].y) + 
				   (double)(tsPoints[3].y - tsPoints[1].y) / (double)(lcdPoints[3].y - lcdPoints[1].y)) / 2;
	//printf("kx=%f, ky=%f\n", tsCalibTmp.dKx, tsCalibTmp.dKy);		   
	memcpy(&(tsCalibTmp.tsBase), &tsPoints[0], sizeof(tsPoints[0]));
	memcpy(&(tsCalibTmp.lcdBase), &lcdPoints[0], sizeof(lcdPoints[0]));
	
	point.x = (short)((tsCalibTmp.dKx) * (lcdPoints[4].x - tsCalibTmp.lcdBase.x) + tsCalibTmp.tsBase.x);
	point.y = (short)((tsCalibTmp.dKy) * (lcdPoints[4].y - tsCalibTmp.lcdBase.y) + tsCalibTmp.tsBase.y);
	
//	printf("check diff:%d, %d\n", abs(tsPoint[4].x - point.x), abs(tsPoint[4].y - point.y));
	if (abs(tsPoints[4].x - point.x) > TOUCH_MAX_DIFF || abs(tsPoints[4].y - point.y) > TOUCH_MAX_DIFF)
	{
		printf("exceed max error 2\n");
		return -1;
	}
	
	memcpy(calibPtr, &tsCalibTmp, sizeof(TSCALIB));
	/*
	printf("Base ts point(%d,%d), base lcd point(%d,%d), dx:%f, dy:%f\n",
			(tsCalib->tsBase).x, (tsCalib->tsBase).y,
			(tsCalib->lcdBase).x, (tsCalib->lcdBase).y,
			tsCalib->dKx, tsCalib->dKy);
	*/
	return 0;
}

int		GetLCDPoint5(XY *lcdPoint, XY *tsPoint, TSCALIB *calibPtr)
{
	if (calibPtr->dKx == 0.0 || calibPtr->dKy == 0.0)
		return -1;
			
	lcdPoint->x = (short)((tsPoint->x - (calibPtr->tsBase).x) / calibPtr->dKx + (calibPtr->lcdBase).x);
	lcdPoint->y = (short)((tsPoint->y - (calibPtr->tsBase).y) / calibPtr->dKy + (calibPtr->lcdBase).y);
	
	return 0;
}

int		GetLCDPoint3(XY *lcdPoint, XY *tsPoint, TSMATRIX *matrixPtr)
{
	int  retValue = 0;

    if ( matrixPtr->sDivisor != 0)
    {
        lcdPoint->x = ((matrixPtr->sA * tsPoint->x) +
                       (matrixPtr->sB * tsPoint->y) +
                       matrixPtr->sC
                       ) / matrixPtr->sDivisor;

        lcdPoint->y = ((matrixPtr->sD * tsPoint->x) +
                       (matrixPtr->sE * tsPoint->y) +
                       matrixPtr->sF
                       ) / matrixPtr->sDivisor;
    }
    else
    {
        retValue = -1 ;
    }

    return retValue;
}
/*---------------------------------------------------------------------------+
|  Function : void	Touch_ADS7843Func(void* pdata)				             |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	Touch_ADS7843Func(void* pdata)
{	
	touchscreen 	nreadbuf, oreadbuf, realreadbuf;
	MSG				msg;
	struct TSRun	*tsrun = (struct TSRun *)pdata;
	
	pthread_detach(pthread_self());	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	nreadbuf.p = oreadbuf.p = 0;
	nreadbuf.x = oreadbuf.x = tsrun->tsdelta.x;
	nreadbuf.y = oreadbuf.y = tsrun->tsdelta.y;
	int			nMouseLDownFlag = 0;
	while(1)
	{
		read(tsrun->tshandle, (void *)&nreadbuf, sizeof(touchscreen));
		
		if(g_nViewChange != 1)
			;
		else
			continue;
			
		if(g_TSAdjustState == 1)		//如果是在調整狀態，則將驅動讀取的值存到realreadbuf中，不進行計算
		{
			memcpy(&realreadbuf, &nreadbuf, sizeof(touchscreen));
		}
		
		if(!g_TSSave.tscalculate)		//tscalculate = 0 由HMI自己計算x,y排列， tscalculate = 1 由驅動計算x,y排列
		{
			//nreadbuf = CalculatePosition(nreadbuf);
			CalculatePosition(&nreadbuf, &nreadbuf);
		}	
		//=======
		// 判斷是否在移動
		//=======
		if((oreadbuf.x - nreadbuf.x) * (oreadbuf.x - nreadbuf.x) + 
			(oreadbuf.y - nreadbuf.y) * (oreadbuf.y - nreadbuf.y) > 6 && 
			oreadbuf.p && nreadbuf.p)
		{		
	    	tsrun->tsdata.x	= nreadbuf.x - tsrun->tsdelta.x;
			tsrun->tsdata.y	= nreadbuf.y - tsrun->tsdelta.y;
			tsrun->tsstate	= TOUCH_MOVE;
			if(nMouseLDownFlag)
			{
		    	msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSEMOVE;
		    	msg.wParam	= (tsrun->tsdata.x << 16) | tsrun->tsdata.y;
		    	msg.lParam	= tsrun->tsstate;
		    	//g_pApplication->QueueMessage(&msg);
		    }
		    else
		    {
		    	msg.hwnd	= (HWND)NULL;		 
	    		msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELDOWN;
	    		msg.wParam	= (tsrun->tsdata.x << 16) | tsrun->tsdata.y;
	    		msg.lParam	= tsrun->tsstate;
	    		nMouseLDownFlag = TRUE;
	    		g_pApplication->QueueMessage(&msg); ;	
		    }
		}
		
		//=======
		// Pose the message of Mouse up and mouse down
		//=======
		//printf("oreadbuf.p=%d nreadbuf.p=%d x=%d, y=%d g_bMouseDown=%d save=%d g_TSAdjustState=%d\n", 
		//oreadbuf.p, nreadbuf.p, nreadbuf.x, nreadbuf.y, g_bMouseDown, g_TSSave.tscalculate, g_TSAdjustState);
	    if(oreadbuf.p == 1 && nreadbuf.p == 0)
	    {
	    	tsrun->tsdata.x	= nreadbuf.x - tsrun->tsdelta.x;
			tsrun->tsdata.y	= nreadbuf.y - tsrun->tsdelta.y;
	    	tsrun->tsidle 	= FALSE;
			tsrun->tsstate	= TOUCH_UP;
			
			if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
	    	{
	    		msg.hwnd	= (HWND)NULL;		 
	    		msg.message	= MSG_MOUSELADJUST;
	    		msg.wParam	= (realreadbuf.x << 16) | realreadbuf.y;
	    		msg.lParam	= tsrun->tsstate;
	    		g_pApplication->QueueMessage(&msg);
	    	}
			
			if(nMouseLDownFlag)
			{
		   	 	msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSELUP;
		    	msg.wParam	= (tsrun->tsdata.x << 16) | tsrun->tsdata.y;
		    	msg.lParam	= tsrun->tsstate;
		    	nMouseLDownFlag = FALSE;
		    	g_pApplication->QueueMessage(&msg);
		    }
	    	//g_bMouseDown= FALSE;
	    	Beep();
	    	//printf("touch up\n");
		}
		else if(/*g_bMouseDown == FALSE && */oreadbuf.p == 0 && nreadbuf.p == 1)
		{
			//g_bMouseDown = TRUE;
			tsrun->tsdata.x	= nreadbuf.x - tsrun->tsdelta.x;
			tsrun->tsdata.y	= nreadbuf.y - tsrun->tsdelta.y;
	    	tsrun->tsidle 	= TRUE;
			tsrun->tsstate	= TOUCH_DOWN;
			
	    	if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
	    	{
	    		msg.hwnd	= (HWND)NULL;		 
	    		msg.message	= MSG_MOUSELADJUST;
	    		msg.wParam	= (realreadbuf.x << 16) | realreadbuf.y;
	    		msg.lParam	= tsrun->tsstate;
	    		g_pApplication->QueueMessage(&msg);
	    	}
			
		    msg.hwnd	= (HWND)NULL;		 
	    	msg.message	= MSG_MOUSELDOWN;
	    	msg.wParam	= (tsrun->tsdata.x << 16) | tsrun->tsdata.y;
	    	msg.lParam	= tsrun->tsstate;
	    	nMouseLDownFlag = TRUE;
	    	g_pApplication->QueueMessage(&msg);
	    	
	    	//printf("touch down\n");
		}
		oreadbuf.p = nreadbuf.p;
		oreadbuf.x = nreadbuf.x;
		oreadbuf.y = nreadbuf.y;		
		
		if(g_ptaskCmd->IsIdle())			//如果屏保，則發生cancel按鍵取消屏保
			g_ptaskCmd->SetIdle(FALSE);
	}
}

/*---------------------------------------------------------------------------+
|  Function : void	USB_MouseFunc(void* pdata)				             	 |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
struct input_event { 
	struct  timeval  time;    //偌瑩奀潔      
	WORD type;//濬倰       
	WORD code;//猁耀攜傖妦繫偌瑩      
	DWORD value;//岆偌狟遜岆庋溫 
};

void	USB_MouseFunc(void* pdata)
{
	pthread_detach(pthread_self());	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	MSG				msg;
	int			nMouseLDownFlag = 0, nMouseMDownFlag = 0, nMouseRDownFlag = 0;
	
	short	m_nX = 0, m_nY = 0;
	while(1)
    {
        char data[6] ={0};    
       // INPUTMOUSE	Mouse;
        /*
        data的數據格式:
        data0:00xx 1xxx   ----低三位是按鍵值---左中右分別為01 02 04, 第4/5位分別代表x、y移動方向，右上方x/y>0,左下方xy<0
        data1:取值範圍-127~127，代表x軸移動偏移量
        data2:取值範圍-127~127，代表y軸移動偏移量            
        */
        usleep(1000);    
        read(m_nMouseFd, data, sizeof(data)); 		//MOUSEDEV_EMUL_PS2方式每次採樣數據為3個字節，多讀不會出錯，只返回成功讀取的數據數
        
       	//printf("1=%d, 2=%d, 3=%d,4=%d, 5=%d \n", Mouse.inputtime.tv_sec, Mouse.inputtime.tv_usec,
       	//Mouse.wType, Mouse.wCode, Mouse.dwValue);
        //printf("\n");
       printf("1=%d, 2=%d, 3=%d, 4=%d, 5=%d, 6=%d \n", data[0], data[1], data[2], data[3], data[4], data[5]);
       if(data[0] & MOUSE_LEFT_CLICK)		//左鍵被按
       //if(wCode = 1)下
        {
        	if (nMouseLDownFlag)
        	{
        		msg.hwnd	= (HWND)NULL;		 
	    		msg.message	= MSG_MOUSEMOVE;		//MSG_MOUSEMOVE;
	    		msg.wParam	= (m_nX << 16) | m_nY;
	    		msg.lParam	= 0;
	    		//g_pApplication->QueueMessage(&msg); 
        	}
        	else
        	{
        		msg.hwnd	= (HWND)NULL;		 
	    		msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELDOWN;
	    		msg.wParam	= (m_nX << 16) | m_nY;
	    		msg.lParam	= 0;
	    		nMouseLDownFlag = TRUE;
	    		g_pApplication->QueueMessage(&msg); 
	    	}
	    	
	    	if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
		    {
		    	msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSELADJUST;
		    	msg.wParam	= (m_nX << 16) | m_nY;
		    	msg.lParam	= TOUCH_DOWN;
		    	g_pApplication->QueueMessage(&msg);
		    }
        }
        else
        {
        	if(nMouseLDownFlag == TRUE)
        	{
        		msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSELUP;		//MSG_MOUSELUP;
		    	msg.wParam	= (m_nX << 16) | m_nY;
		    	msg.lParam	= 0;
		    	nMouseLDownFlag = FALSE;
		    	g_pApplication->QueueMessage(&msg); 
		    	if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
			    {
			    	msg.hwnd	= (HWND)NULL;		 
			    	msg.message	= MSG_MOUSELADJUST;
			    	msg.wParam	= (m_nX << 16) | m_nY;
			    	msg.lParam	= TOUCH_UP;
			    	g_pApplication->QueueMessage(&msg);
			    }
        	}		
        }
        
        if(data[0] & MOUSE_MIDDLE_CLICK)	//中間鍵鍵被按下
        {
        	msg.hwnd	= (HWND)NULL;		 
	    	msg.message	= MSG_MOUSEMDOWN;
	    	msg.wParam	= (m_nX << 16) | m_nY;
	    	msg.lParam	= 0;
	    	nMouseMDownFlag = TRUE;
	    	g_pApplication->QueueMessage(&msg); 
        }
        else
        {
        	if(nMouseMDownFlag == TRUE)
        	{
        		msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSEMUP;		//MSG_MOUSEMUP;
		    	msg.wParam	= (m_nX << 16) | m_nY;
		    	msg.lParam	= 0;
		    	nMouseMDownFlag = FALSE;
		    	g_pApplication->QueueMessage(&msg); 	
        	}	
        }
        
        
        if(data[0] & MOUSE_RIGHT_CLICK)		//右鍵被按下
        {
        	msg.hwnd	= (HWND)NULL;		 
	    	msg.message	= MSG_MOUSERDOWN;
	    	msg.wParam	= (m_nX << 16) | m_nY;
	    	msg.lParam	= 0;
	    	nMouseRDownFlag = TRUE;
	    	g_pApplication->QueueMessage(&msg);
        }
        else
        {
        	if(nMouseRDownFlag == TRUE)
        	{
        		msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSERUP;		//MSG_MOUSERUP;
		    	msg.wParam	= (m_nX << 16) | m_nY;
		    	msg.lParam	= 0;
		    	nMouseRDownFlag = FALSE;
		    	g_pApplication->QueueMessage(&msg); 	
        	}	
        }
        
        m_nX += data[1];
        m_nY += -data[2];


        int maxX;
        int maxY;
        #define VERTICAL_SCREEN
        #ifdef  VERTICAL_SCREEN
        	maxX=g_infoDrv.nY;
        	maxY=g_infoDrv.nX;
        #else
			maxX=g_infoDrv.nX;
        	maxY=g_infoDrv.nY;
        #endif
        if(m_nX > maxX-1)
        	m_nX = maxX-1;
        if(m_nX < 0)
        	m_nX = 0;
        if(m_nY > maxY-1)
        	m_nY = maxY-1;
        if(m_nY < 0)
        	m_nY = 0;
        //msg.hwnd	= (HWND)NULL;		 
    	//msg.message	= MSG_MOUSEMOVE;
    	//msg.wParam	= (m_nX << 16) | m_nY;
    	//msg.lParam	= TOUCH_MOVE;//m_nState;
    	//g_pApplication->QueueMessage(&msg); 
    	
    	if(g_ptaskCmd->IsIdle())			//如果屏保，則發生cancel按鍵取消屏保
    		g_ptaskCmd->SetIdle(FALSE);

        g_CursorX = m_nX;
        g_CursorY = m_nY;
        g_CursorChange = 1;
	}
}

void	USB_MouseFunc2_Check(void* pdata)
{
	pthread_detach(pthread_self());	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		
	int	nFd  = open("/dev/fb0", O_RDWR, 0);
	int	nOldValue1 = 0, nNewValue1 = 0;
	int	nOldValue2 = 0, nNewValue2 = 0;
	
	if(nFd >= 0)
	{
		nOldValue1 = ioctl(nFd, 0x1004, NULL);
		nOldValue2 = ioctl(nFd, 0x1005, NULL);
	}
	
	while(1)
    {
    	if(nFd >= 0)
        {
        	nNewValue1 = ioctl(nFd, 0x1004, NULL);	
        	nNewValue2 = ioctl(nFd, 0x1005, NULL);	
        	
        	if(nOldValue2 != nNewValue2)
        	{
        		if(m_nMouseFd >= 0)
				{
					close(m_nMouseFd);
					m_nMouseFd = -1;
				}
				nOldValue2 = nNewValue2;
        	}
        	
        	if((nOldValue1 != nNewValue1))
        	{
        		int		ret;
				if(m_nMouseFd >= 0)
				{
					close(m_nMouseFd);
					printf("close 1 \n");
					m_nMouseFd = -1;
				}
				
				char	szPath[32];
				int i = 0;
				for(i = 0; i <= 20; i++)
				{
					sprintf(szPath, "/dev/input/event%d", i);
					if((m_nMouseFd = open(szPath, O_RDONLY)) == -1)
					{
			    	    printf("Failed to open %s\n", szPath);
			    	    break;
			    	}
			    	else
			    	{
			    		sleep(1);
			    		close(m_nMouseFd);
						printf("close szPath %s \n", szPath);
						m_nMouseFd = -1;
			    	    continue;
			    	}
				}
				printf("i = %d \n", i);
				if(i == 0)
				{
					sprintf(szPath, "/dev/input/event%d", i);
					if((m_nMouseFd = open(szPath, O_RDONLY)) == -1)
					{
			    	    printf("Failed to open %s\n", szPath);
			    	}
				}
				else
				{
					sprintf(szPath, "/dev/input/event%d", i-1);
					if((m_nMouseFd = open(szPath, O_RDONLY)) == -1)
					{
			    	    printf("Failed to open %s\n", szPath);
			    	}
				}
				
				//if((m_nMouseFd = open(USB_MOUSE3, O_RDONLY)) == -1)
				//{
			    //    printf("Failed to open %s\n", USB_MOUSE);
			    //    if ((m_nMouseFd = open(USB_MOUSE, O_RDONLY)) == -1)
			    //    {
			    //    	printf("Failed to open %s\n", USB_MOUSE2);
			    //    	//if ((m_nMouseFd = open(USB_MOUSE3, O_RDONLY)) == -1)
				//        //{
				//        //	printf("Failed to open %s\n", USB_MOUSE3);
				//        //	return DEVICE_ERR_DEVICE;
				//        //}
				//        //else
				//        {
				//        	printf("m_nMouseFd=%d USB_MOUSE3=%s \n", m_nMouseFd, USB_MOUSE3);	
				//        }
			    //    	//return DEVICE_ERR_DEVICE;
    			//		sleep(1);
			    //    	continue;
			    //    }
			    //}
			    //else
			    printf("recreate m_nMouseFd=%d \n", m_nMouseFd);
			    {
        			pthread_cancel(m_MouseReadthread);
					pthread_join(m_MouseReadthread,NULL);
					sleep(1);
			    	ret = pthread_create(&m_MouseReadthread, NULL, (PRSRUN)USB_MouseFunc2, NULL);
					//printf("====touch screen reopen nOldValue1=%d nNewValue1=%d \n", nOldValue1, nNewValue1);
        			nOldValue1 = nNewValue1;
        		}
        	}
        }
        g_nWaitTime++;
    	sleep(1);
	}
	close(nFd);
}

void	USB_MouseFunc2(void* pdata)
{
	pthread_detach(pthread_self());	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	MSG				msg;
	int			nMouseLDownFlag = 0, nMouseMDownFlag = 0, nMouseRDownFlag = 0, nTouchIndex = 0;
	touchscreen 	nreadbuf, oreadbuf;
	static	short	m_nOldX = 0, m_nOldY = 0;
	static	int		nMouseLastDownFlag = 0;
	static	int		nLostCount = 0;
	static	int		nFirstTouch = 0;
	
	while(1)
    {
        char data[16] ={0};    
       // INPUTMOUSE	Mouse;
        /*
        data的數據格式:
        data0:00xx 1xxx   ----低三位是按鍵值---左中右分別為01 02 04, 第4/5位分別代表x、y移動方向，右上方x/y>0,左下方xy<0
        data1:取值範圍-127~127，代表x軸移動偏移量
        data2:取值範圍-127~127，代表y軸移動偏移量            
        */
        
        usleep(10);    
        int nLen = read(m_nMouseFd, &u_UNtiecTouch, sizeof(u_UNtiecTouch)); 		//MOUSEDEV_EMUL_PS2方式每次採樣數據為3個字節，多讀不會出錯，只返回成功讀取的數據數
        
        if(g_nWaitTime < 1)
        	continue ;
       	//printf("u_UNtiecTouch.wCode=%x value=%x \n", u_UNtiecTouch.wCode, u_UNtiecTouch.dwValue);
        switch(u_UNtiecTouch.wCode)
        {
        	case 1:				//Read X
        		nreadbuf.x = u_UNtiecTouch.dwValue & 0xFFFF;
        		//nMouseLDownFlag = (u_UNtiecTouch.dwValue & 0xFF00) >> 0xFF;
        		break;
        	case 3:				//Read Y
        		nreadbuf.y = u_UNtiecTouch.dwValue & 0xFFFF;
        		nMouseLDownFlag = (u_UNtiecTouch.dwValue & 0x00FF0000) >> 16;
        		//nMouseLDownFlag = (u_UNtiecTouch.dwValue & 0x00FF0000) >> 16;
        		//nTouchIndex		= (u_UNtiecTouch.dwValue & 0xFF000000) >> 24;
        		
        		printf("nreadbuf.x=%d nreadbuf.y=%d nMouseLDownFlag=%d \n", nreadbuf.x, nreadbuf.y, nMouseLDownFlag);
        		if(g_TSAdjustState == 1)
				{
					oreadbuf.x = nreadbuf.x;
					oreadbuf.y = nreadbuf.y;
				}
				else
					CalculatePosition(&nreadbuf, &oreadbuf);
        		//printf("down=%d nTouchIndex=%d x=%d y=%d rx=%d ,ry=%d flag=%d \n", nMouseLDownFlag, nTouchIndex, oreadbuf.x, oreadbuf.y,
        		//nreadbuf.x, nreadbuf.y, nMouseLastDownFlag);
        		
        		//if(nTouchIndex == 1)
        		{
	        		if(nMouseLDownFlag)		//down
	        		{
	        			//printf("nMouseLastDownFlag = %d m_nOldX=%d m_nOldY=%d oreadbuf.x=%d oreadbuf.y=%d \n", nMouseLastDownFlag,
	        			// m_nOldX, m_nOldY, oreadbuf.x, oreadbuf.y);
	        			if(((m_nOldX != oreadbuf.x) || (m_nOldY != oreadbuf.y)) && (nMouseLastDownFlag == 0))
	        			{
					    	//printf("down oreadbuf.x=%d y=%d nLostCount = %d\n", oreadbuf.x, oreadbuf.y, nLostCount);
	        				if(nLostCount >=1)
	        				{
		        				nMouseLastDownFlag 	= 1;
		        				m_nOldX				= oreadbuf.x;
		        				m_nOldY				= oreadbuf.y;
		        				
			        			msg.hwnd	= (HWND)NULL;		 
					    		msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELUP;
					    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
					    		msg.lParam	= 0;
					    		
					    		nLostCount = 0;
					    		//printf("down oreadbuf.x=%d y=%d \n", oreadbuf.x, oreadbuf.y);
					    		if(g_nWaitTime > 1)
					    		{
					    			g_pApplication->QueueMessage(&msg); 
					    			Beep();
					    		}
					    		;//nFirstTouch++;
					    	}
					    	nLostCount++;
				    	}
	        		}
	        		else					//up
	        		{
	        			msg.hwnd	= (HWND)NULL;		 
			    		msg.message	= MSG_MOUSELUP;		//MSG_MOUSELUP;
			    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
			    		msg.lParam	= 0;
			    		
			    		nLostCount = 0;
						//printf("up oreadbuf.x=%d y=%d \n", oreadbuf.x, oreadbuf.y);
			    		nMouseLastDownFlag = 0;
			    		if(g_nWaitTime > 1)
	        			{
			    			g_pApplication->QueueMessage(&msg); 	
			    		}
	        		}
	        	}
	        	if(g_nWaitTime > 1)
	        	{
        			if(g_ptaskCmd->IsIdle())			//如果屏保，則發生cancel按鍵取消屏保
        				g_ptaskCmd->SetIdle(FALSE);
						}
        		break;
        };
	}
}

void	USB_MouseFunc5(void* pdata)
{
////	pthread_detach(pthread_self());	
////	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
////	MSG				msg;
////	int			nMouseLDownFlag = 0, nMouseMDownFlag = 0, nMouseRDownFlag = 0, nTouchIndex = 0;
////	touchscreen 	nreadbuf, oreadbuf, realreadbuf;
////	
////	static	short	m_nOldX = 0, m_nOldY = 0;
////	static	int		nMouseLastDownFlag = 0;
////	static	int		nLostCount = 0;
////	
////	struct TSRun	*tsrun = (struct TSRun *)pdata;
////	nreadbuf.p = oreadbuf.p = 0;
////	nreadbuf.x = oreadbuf.x = tsrun->tsdelta.x;
////	nreadbuf.y = oreadbuf.y = tsrun->tsdelta.y;
////	
////	int		nMouseMove = 0;
////	short	m_nX = 0, m_nY = 0;
////	int		nTransFlag = 0;
////	
////	struct tsdev *ts;
////	
////	ts = ts_open (USB_MOUSE4, 0);
////	
////	if (!ts) {
////		printf("device error \n");
////		return ;
////	}
////
////	if (ts_config(ts)) {
////		printf("ts_config error \n");
////		return ;
////	}
////	
////	while(1)
////    {
////    	struct input_event ev;
////		struct ts_sample samp;
////		int ret;
////		
////		//usleep(1000);
////	
////		ret = ts_read(ts, &samp, 1);
////		
////		if (ret != 1)
////			continue;
////			
////		printf("%6d %6d %6d\n", 
////			samp.x, samp.y, samp.pressure);
////		//ret = read(tsrun->tshandle, &u_PMTouch, sizeof(u_PMTouch));
////		
////		if(samp.pressure == 0)
////		{
////			msg.hwnd	= (HWND)NULL;		 
////			msg.message	= MSG_MOUSELUP;				//MSG_MOUSELDOWN;
////			msg.wParam	= (samp.x << 16) | samp.y;
////			msg.lParam	= TOUCH_UP;
////			nMouseLDownFlag = 0;
////			g_pApplication->QueueMessage(&msg);
////			//printf("up m_nX=%d m_nY=%d \n", m_nX, m_nY);	
////			printf("up oreadbuf.x=%d oreadbuf.y=%d \n", samp.x, samp.y);	
////			Beep();	
////			
////			if( g_ptaskCmd!= NULL)
////				g_ptaskCmd->SetIdle(FALSE);
////		}
////		else
////		{
////			if(nMouseLDownFlag == 1)
////			{
////				;
////			}
////			else
////			{
////				msg.hwnd	= (HWND)NULL;		 
////				msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELDOWN;
////				msg.wParam	= (samp.x << 16) | samp.y;
////				msg.lParam	= TOUCH_DOWN;
////				nMouseLDownFlag = 1;
////				g_pApplication->QueueMessage(&msg);		
////				printf("down oreadbuf.x=%d oreadbuf.y=%d \n", samp.x, samp.y);	
////				
////				if( g_ptaskCmd!= NULL)
////					g_ptaskCmd->SetIdle(FALSE);
////			}
////		}
////		//switch (ev.type)
////		//{
////		//	case EV_SYN:
////		//		break;
////		//	case EV_ABS:
////		//		switch (ev.code)
////		//		{
////		//			case ABS_X:
////		//				nreadbuf.x 	= ev.value;
////		//				break;
////		//			case ABS_Y:
////		//				nreadbuf.y 	= ev.value;
////		//				break;
////		//			case ABS_PRESSURE:
////		//				nreadbuf.p = ev.value;
////		//				break;
////		//		}
////		//	break;
////		//}
////		//printf("wtype=%d code=%d value=%d \n", u_PMTouch.wType, u_PMTouch.wCode, u_PMTouch.dwValue);
////	////	switch(u_PMTouch.wType)
////	////	{
////	////		case 3:			//Read x y
////	////			if(u_PMTouch.wCode == 0)			//Read X
////	////			{
////	////				nreadbuf.x = m_nX = u_PMTouch.dwValue;
////	////				nTransFlag |= 1;				//Transing
////	////				//printf("read x nTransFlag=%x \n", nTransFlag);
////	////			}
////	////			else if(u_PMTouch.wCode == 1)		//Read Y
////	////			{
////	////				nreadbuf.y = m_nY = u_PMTouch.dwValue;
////	////				nTransFlag |= 2;				//Transing
////	////				//printf("read y nTransFlag=%x \n", nTransFlag);
////	////			}	
////	////			if(nMouseLDownFlag == 1)
////	////			{
////	////				nMouseMove = 1;	
////	////			}
////	////			else
////	////			{
////	////				nMouseMove = 0;
////	////			}				
////	////			break;
////	////		case 1:			//
////	////			if(u_PMTouch.wCode == 330)			//Down or Up
////	////			{
////	////				if(u_PMTouch.dwValue == 1)		//Down
////	////				{
////	////					nMouseLDownFlag = 1;
////	////					nMouseMove = 0;
////	////				}
////	////				else if(u_PMTouch.dwValue == 0)		//Up
////	////				{
////	////					nMouseLDownFlag = 0;
////	////					nMouseMove = 0;
////	////				}
////	////			}
////	////			else
////	////			{
////	////				;//error	
////	////			}
////	////			nTransFlag |= 4;						//Transing
////	////			break;
////	////		case 0:			//End Trans
////	////			nTransFlag |= 8;						//Trans end
////	////			break;
////	////	}
////	////	//printf("nMouseLDownFlag=%d nMouseMove=%d nTransFlag=%x \n", nMouseLDownFlag, nMouseMove, nTransFlag);
////	////	if(nTransFlag & 0x8)		//New Data
////	////	{
////	////		//printf("nreadbuf.x=%d nreadbuf.y=%d,  oreadbuf.x=%d, oreadbuf.y=%d \n",
////	////		// nreadbuf.x, nreadbuf.y, oreadbuf.x, oreadbuf.y);
////	////		printf("x=%d y=%d flag=%d \n", nreadbuf.x, nreadbuf.y, nMouseLDownFlag);
////	////		if(g_TSAdjustState == 1)
////	////		{
////	////			oreadbuf.x = nreadbuf.x;
////	////			oreadbuf.y = nreadbuf.y;
////	////		}
////	////		else
////	////			CalculatePosition(&nreadbuf, &oreadbuf);
////	////		//printf("");
////	////		//Calculate_AtomPosition(&nreadbuf, &oreadbuf);
////	////		if(nTransFlag & 0x4)
////	////		{
////	////			if(nTransFlag & 0x3)	//Read X Y
////	////			{
////	////				if(nMouseLDownFlag == 1)
////	////				{
////	////					if(nMouseMove == 1) 
////	////					{
////	////			    		msg.hwnd	= (HWND)NULL;	
////	////			    		msg.message	= MSG_MOUSEMOVE;
////	////			    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////			    		msg.lParam	= TOUCH_MOVE;
////	////		    			//printf("Move1 m_nX=%d m_nY=%d \n", m_nX, m_nY);	
////	////		    			//printf("Move1 oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
////	////			    	}
////	////			    	else
////	////			    	{
////	////			    		if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
////	////				    	{
////	////				    		msg.hwnd	= (HWND)NULL;		 
////	////				    		msg.message	= MSG_MOUSELADJUST;
////	////				    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////				    		msg.lParam	= TOUCH_DOWN;
////	////				    		g_pApplication->QueueMessage(&msg);
////	////				    	}
////	////			    	
////	////			    		msg.hwnd	= (HWND)NULL;		 
////	////			    		msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELDOWN;
////	////			    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////			    		msg.lParam	= TOUCH_DOWN;
////	////			    		nMouseLDownFlag = 1;
////	////			    		g_pApplication->QueueMessage(&msg);	
////	////		    			printf("Down m_nX=%d m_nY=%d \n", oreadbuf.x, oreadbuf.y);	
////	////		    			//printf("Down oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
////	////			    	}
////	////			    }
////	////			    else
////	////			    {
////	////			    	if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
////	////			    	{
////	////			    		msg.hwnd	= (HWND)NULL;		 
////	////			    		msg.message	= MSG_MOUSELADJUST;
////	////			    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////			    		msg.lParam	= TOUCH_UP;
////	////			    		g_pApplication->QueueMessage(&msg);
////	////			    	}
////	////			    	
////	////			    	msg.hwnd	= (HWND)NULL;		 
////	////		    		msg.message	= MSG_MOUSELUP;				//MSG_MOUSELDOWN;
////	////		    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////		    		msg.lParam	= TOUCH_UP;
////	////		    		nMouseLDownFlag = 0;
////	////		    		g_pApplication->QueueMessage(&msg);
////	////		    		//printf("up m_nX=%d m_nY=%d \n", m_nX, m_nY);	
////	////		    		printf("up oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
////	////    				Beep();	
////	////    				
////	////	   				if( g_ptaskCmd!= NULL)
////	////						g_ptaskCmd->SetIdle(FALSE);
////	////			    }
////	////		    	nTransFlag &= ~(0x3);
////	////			}
////	////			else
////	////			{
////	////				if(nMouseLDownFlag == 0)
////	////				{
////	////					if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
////	////			    	{
////	////			    		msg.hwnd	= (HWND)NULL;		 
////	////			    		msg.message	= MSG_MOUSELADJUST;
////	////			    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////			    		msg.lParam	= TOUCH_UP;
////	////			    		g_pApplication->QueueMessage(&msg);
////	////			    	}
////	////					
////	////					msg.hwnd	= (HWND)NULL;		 
////	////		    		msg.message	= MSG_MOUSELUP;				//MSG_MOUSELDOWN;
////	////		    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////		    		msg.lParam	= TOUCH_UP;
////	////		    		nMouseLDownFlag = 0;
////	////		    		g_pApplication->QueueMessage(&msg);
////	////		    		//printf("Up m_nX=%d m_nY=%d \n", m_nX, m_nY);	
////	////		    		printf("up oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);
////	////    				Beep();	
////	////    				
////	////	   				if( g_ptaskCmd!= NULL)
////	////						g_ptaskCmd->SetIdle(FALSE);
////	////				}
////	////			}
////	////			nTransFlag &= ~(0x4);
////	////	    	nTransFlag &= ~(0x8);
////	////	    }
////	////	   	else
////	////	   	{
////	////	   		if(nMouseLDownFlag == 1)
////	////			{
////	////				if(nMouseMove == 1) 
////	////				{
////	////		    		msg.hwnd	= (HWND)NULL;	
////	////		    		msg.message	= MSG_MOUSEMOVE;
////	////		    		msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
////	////		    		msg.lParam	= TOUCH_MOVE;
////	////		    		//printf("Move m_nX=%d m_nY=%d \n", m_nX, m_nY);	
////	////		    		//printf("Move oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
////	////		    	}
////	////		    }		
////	////	   	}
////	////	   	;//printf("idle \n");
////	////	}
////	}
}
/*
void	USB_MouseFunc4(void* pdata)
{
	pthread_detach(pthread_self());	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	MSG				msg;
	int			nMouseLDownFlag = 0, nMouseMDownFlag = 0, nMouseRDownFlag = 0, nTouchIndex = 0;
	touchscreen 	nreadbuf, oreadbuf, realreadbuf;
	
	static	short	m_nOldX = 0, m_nOldY = 0;
	static	int		nMouseLastDownFlag = 0;
	static	int		nLostCount = 0;
	int			g_Rotation = 3;   //0=normal  1=90degree  2= 180 degree  3=270 degree
	
	struct TSRun	*tsrun = (struct TSRun *)pdata;
	nreadbuf.p = oreadbuf.p = 0;
	nreadbuf.x = oreadbuf.x = tsrun->tsdelta.x;
	nreadbuf.y = oreadbuf.y = tsrun->tsdelta.y;
	
	int		nMouseMove = 0;
	short	m_nX = 0, m_nY = 0;
	int		nTransFlag = 0;
	while(1)
	{
		int ret;
		usleep(1000);
		ret = read(tsrun->tshandle, &u_PMTouch, sizeof(u_PMTouch));
		printf("wtype=%d code=%d value=%d \n", u_PMTouch.wType, u_PMTouch.wCode, u_PMTouch.dwValue);
		switch(u_PMTouch.wType)
		{
			case 3:			//Read x y
				if(u_PMTouch.wCode == 0)			//Read X
				{
					nreadbuf.x = m_nX = u_PMTouch.dwValue;
					nTransFlag |= 1;				//Transing
				}
				else if(u_PMTouch.wCode == 1)		//Read Y
				{
					nreadbuf.y = m_nY = u_PMTouch.dwValue;
					nTransFlag |= 2;				//Transing
				}	
				break;
			case 1:			//
				if(u_PMTouch.wCode == 330)			//Down or Up
				{
					if(u_PMTouch.dwValue == 1)		//Down
					{
						nMouseLDownFlag = 1;
					}
				}
				nTransFlag |= 4;						//Transing
				break;
			case 0:			//End Trans
				nTransFlag |= 8;						//Trans end
				break;
		}
		if(nTransFlag & 0x8)		//New Data
		{
			if(g_TSAdjustState == 1)
			{
				oreadbuf.x = nreadbuf.x;
				oreadbuf.y = nreadbuf.y;
			}
			else
				CalculatePosition(&nreadbuf, &oreadbuf);
			if(nTransFlag & 0x4)
			{
				if(nMouseLDownFlag == 1)
				{
				  if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
				  {
				  	msg.hwnd	= (HWND)NULL;		 
				    msg.message	= MSG_MOUSELADJUST;
				   	msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
				    msg.lParam	= TOUCH_DOWN;
				    g_pApplication->QueueMessage(&msg);
				  }
				  msg.hwnd	= (HWND)NULL;		 
				  msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELDOWN;
				  SetMsgwParam(&msg,m_nX,m_nY,g_Rotation);
				  //msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
				  msg.lParam	= TOUCH_DOWN;
				  nMouseLDownFlag = 2;
				  g_pApplication->QueueMessage(&msg);	
			  	//printf("Down m_nX=%d m_nY=%d \n", oreadbuf.x, oreadbuf.y);	
			  	//printf("Down oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
				}
				else if(nMouseLDownFlag == 2)
				{
					if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
					{
						msg.hwnd	= (HWND)NULL;		 
						msg.message	= MSG_MOUSELADJUST;
						msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
						msg.lParam	= TOUCH_UP;
						g_pApplication->QueueMessage(&msg);
					}
					msg.hwnd	= (HWND)NULL;		 
					msg.message	= MSG_MOUSELUP;				//MSG_MOUSELDOWN;
				  SetMsgwParam(&msg,m_nX,m_nY,g_Rotation);
					//msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
					msg.lParam	= TOUCH_UP;
					nMouseLDownFlag = 0;
					g_pApplication->QueueMessage(&msg);
					//printf("up m_nX=%d m_nY=%d \n", m_nX, m_nY);	
					//printf("up oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
	    			//Beep();	
					if( g_ptaskCmd!= NULL)
						g_ptaskCmd->SetIdle(FALSE);
				}
				nTransFlag &= ~(0x3);
				nTransFlag &= ~(0x4);
			}
			else
			{
				if(nTransFlag & 0x3 && nMouseLDownFlag==2)	//Read X Y
				{
					msg.hwnd	= (HWND)NULL;	
					msg.message	= MSG_MOUSEMOVE;
				  SetMsgwParam(&msg,m_nX,m_nY,g_Rotation);
					//msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
					if(MouseMoveStep==65536)	MouseMoveStep=1;
					else MouseMoveStep++;
					msg.lParam	= MouseMoveStep;
					g_pApplication->QueueMessage(&msg);
					//printf("Move m_nX=%d m_nY=%d \n", m_nX, m_nY);	
					//printf("Move oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
					nTransFlag &= ~(0x3);
			  }		
		  }
		 nTransFlag &= ~(0x8);
		}
	}
}
*/

void	USB_MouseFunc4(void* pdata)
{
	pthread_detach(pthread_self());	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	MSG				msg;
	int			nMouseLDownFlag = 0, nMouseMDownFlag = 0, nMouseRDownFlag = 0, nTouchIndex = 0;
	touchscreen 	nreadbuf, oreadbuf, realreadbuf;
	
	static	short	m_nOldX = 0, m_nOldY = 0;
	static	int		nMouseLastDownFlag = 0;
	static	int		nLostCount = 0;
	int			g_Rotation = 3;   //0=normal  1=90degree  2= 180 degree  3=270 degree
	
	struct TSRun	*tsrun = (struct TSRun *)pdata;
	nreadbuf.p = oreadbuf.p = 0;
	nreadbuf.x = oreadbuf.x = tsrun->tsdelta.x;
	nreadbuf.y = oreadbuf.y = tsrun->tsdelta.y;
	
	int		nMouseMove = 0;
	short	m_nX = 0, m_nY = 0;
	int		nTransFlag = 0;
	while(1)
	{
		int ret;
		//usleep(10); 
		ret = read(tsrun->tshandle, &u_PMTouch, sizeof(u_PMTouch));
		printf("\n\nUSB_MouseFunc4 Read !!!!!!!!!!!!!!!!!!!!!\n\n");
		if( g_ptaskCmd!= NULL)
			g_ptaskCmd->SetIdle(FALSE);
		// 邊緣偏移
		if((u_PMTouch.wCode == 0 || u_PMTouch.wCode == 1 || u_PMTouch.wCode == 53 || u_PMTouch.wCode == 54) && u_PMTouch.dwValue<10) u_PMTouch.dwValue=10;
		if( (u_PMTouch.wCode == 1 || u_PMTouch.wCode == 54) && u_PMTouch.dwValue > 590) u_PMTouch.dwValue =590;
		if( (u_PMTouch.wCode == 0 || u_PMTouch.wCode == 53) && u_PMTouch.dwValue >1014) u_PMTouch.dwValue =1014;
		printf("wtype=%d code=%d value=%d \n", u_PMTouch.wType, u_PMTouch.wCode, u_PMTouch.dwValue);
		switch(u_PMTouch.wType)
		{
			case 3:			//Read x y
				if(u_PMTouch.wCode == 0 || u_PMTouch.wCode == 53)			//Read X
				{
					if(nreadbuf.x != u_PMTouch.dwValue)
					{
						nreadbuf.x = m_nX = u_PMTouch.dwValue;
						nTransFlag |= 1;				//Transing
					}
				}
				else if(u_PMTouch.wCode == 1 || u_PMTouch.wCode == 54)		//Read Y
				{
					if(nreadbuf.y != u_PMTouch.dwValue)
					{
						nreadbuf.y = m_nY = u_PMTouch.dwValue;
						nTransFlag |= 2;				//Transing
					}
				}	
				else if(u_PMTouch.wCode == 57 && u_PMTouch.dwValue!=0)		//else touch
				{
					nTransFlag |= 16;
				}	
				
				break;
			case 1:			//
				if(u_PMTouch.wCode == 330)			//Down or Up
				{
					if(u_PMTouch.dwValue == 1)		//Down
					{
						nMouseLDownFlag = 1;
					}
				}
				nTransFlag |= 4;						//Transing
				break;
			case 0:			//End Trans
				if(/*u_PMTouch.wCode == 2 && */nTransFlag!=0)			
				{
					nTransFlag |= 8;						//Trans end
					break;
				}
		}
		if(nTransFlag & 0x10)
		{
			nreadbuf.x = m_nX = oreadbuf.x;
			nreadbuf.y = m_nY = oreadbuf.y;
			nTransFlag=0;
		}
		else if(nTransFlag & 0x8)		//New Data
		{
			if(g_TSAdjustState == 1)
			{
				oreadbuf.x = nreadbuf.x;
				oreadbuf.y = nreadbuf.y;
			}
			else
				CalculatePosition(&nreadbuf, &oreadbuf);
			if(nTransFlag & 0x4)
			{
				if(nMouseLDownFlag == 1)
				{
				  if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
				  {
				  	msg.hwnd	= (HWND)NULL;		 
				    msg.message	= MSG_MOUSELADJUST;
				   	msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
				    msg.lParam	= TOUCH_DOWN;
				    g_pApplication->QueueMessage(&msg);
				  }
				  msg.hwnd	= (HWND)NULL;		 
				  msg.message	= MSG_MOUSELDOWN;		//MSG_MOUSELDOWN;
				  SetMsgwParam(&msg,m_nX,m_nY,g_Rotation);
				  //msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
				  msg.lParam	= TOUCH_DOWN;
				  nMouseLDownFlag = 2;
				  g_pApplication->QueueMessage(&msg);	
			  	//printf("Down m_nX=%d m_nY=%d \n", oreadbuf.x, oreadbuf.y);	
			  	//printf("Down oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
				}
				else if(nMouseLDownFlag == 2)
				{
					if(g_TSAdjustState == 1)		//0 == adjust ok, 1 == adjust ing
					{
						msg.hwnd	= (HWND)NULL;		 
						msg.message	= MSG_MOUSELADJUST;
						msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
						msg.lParam	= TOUCH_UP;
						g_pApplication->QueueMessage(&msg);
					}
					msg.hwnd	= (HWND)NULL;		 
					msg.message	= MSG_MOUSELUP;				//MSG_MOUSELDOWN;
				  SetMsgwParam(&msg,m_nX,m_nY,g_Rotation);
					//msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
					msg.lParam	= TOUCH_UP;
					nMouseLDownFlag = 0;
					g_pApplication->QueueMessage(&msg);
					//printf("up m_nX=%d m_nY=%d \n", m_nX, m_nY);	
					//printf("up oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
	    			//Beep();	
				}
				nTransFlag &= ~(0x3);
				nTransFlag &= ~(0x4);
			}
			else
			{
				if(nTransFlag & 0x3 && nMouseLDownFlag==2)	//Read X Y
				{
					msg.hwnd	= (HWND)NULL;	
					msg.message	= MSG_MOUSEMOVE;
				  SetMsgwParam(&msg,m_nX,m_nY,g_Rotation);
					//msg.wParam	= (oreadbuf.x << 16) | oreadbuf.y;
					if(MouseMoveStep==65536)	MouseMoveStep=1;
					else MouseMoveStep++;
					msg.lParam	= MouseMoveStep;
					g_pApplication->QueueMessage(&msg);
					//printf("Move m_nX=%d m_nY=%d \n", m_nX, m_nY);	
					//printf("Move oreadbuf.x=%d oreadbuf.y=%d \n", oreadbuf.x, oreadbuf.y);	
					nTransFlag &= ~(0x3);
			  }		
		  }
		 nTransFlag &= ~(0x8);
		}
	}
}


void	SetMsgwParam(MSG *msg,int x, int y, int Rotation)
{
	touchscreen tmp;
	
	if(Rotation==0)
	{
		tmp.x=x;
		tmp.y=y;
	}
	else	if(Rotation==3)
	{
		tmp.x=y;
		tmp.y=1024-x-1;
	}
	
	g_nTouchX = tmp.x;
	g_nTouchY = tmp.y;
  g_CursorX = tmp.x;
  g_CursorY = tmp.y;
  //g_nTouchView = 1.0;
	//printf("coord.x = %d  coord.y=%d\n",x,y);
	//printf("tmp.x = %d  tmp.y=%d\n",tmp.x,tmp.y);
	msg->wParam	= (tmp.x << 16) | tmp.y;
}
		
TOUCHSCREEN	CalculatePosition(touchscreen 	readbuf)
{
	short tmp;
	TOUCHSCREEN tmpbuf;
	
	//swap x,y
	if(g_TSSave.tsangle & 0x04)
	{
		tmp 		= readbuf.y;
		readbuf.y 	= readbuf.x;
		readbuf.x 	= tmp;
	}
	
	//calculate Position by set order
	tmpbuf.p = readbuf.p;
	//printf("x=%d , y=%d , m=%d  \n", readbuf.x, readbuf.y, readbuf.m);
	if(g_TSSave.tsmax.x != g_TSSave.tsmin.x)
	{
		tmpbuf.x = (g_TSSave.tsangle & 0x01) ? ((g_TSSave.tsmax.x - readbuf.x) * g_TSSave.tspixel.x) / (g_TSSave.tsmax.x - g_TSSave.tsmin.x) :
		((readbuf.x - g_TSSave.tsmin.x) * g_TSSave.tspixel.x) / (g_TSSave.tsmax.x - g_TSSave.tsmin.x);
	}
	if(g_TSSave.tsmax.y != g_TSSave.tsmin.y)
	{
		tmpbuf.y = (g_TSSave.tsangle & 0x02) ? ((g_TSSave.tsmax.y - readbuf.y) * g_TSSave.tspixel.y) / (g_TSSave.tsmax.y - g_TSSave.tsmin.y) :
		((readbuf.y - g_TSSave.tsmin.y) * g_TSSave.tspixel.y) / (g_TSSave.tsmax.y - g_TSSave.tsmin.y);
	}
	
	tmpbuf.x = tmpbuf.x < g_TSSave.tspixel.x ? tmpbuf.x : g_TSSave.tspixel.x - 1;
	tmpbuf.y = tmpbuf.y < g_TSSave.tspixel.y ? tmpbuf.y : g_TSSave.tspixel.y - 1;
	
	tmpbuf.x = tmpbuf.x >= 0 ? tmpbuf.x : 0;
	tmpbuf.y = tmpbuf.y >= 0 ? tmpbuf.y : 0;
	tmpbuf.m = readbuf.m;
	
	//printf("g_TSSave.tsmax.y=%d g_TSSave.tspixel.y=%d \n", g_TSSave.tsmax.y, g_TSSave.tspixel.y);
	return tmpbuf;
}

void Calculate_AtomPosition(touchscreen *buffin, touchscreen *buffout)
{
	XY	tsPoint;
	float	fPer_x = 0, fPer_y = 0;
	tsPoint.x = buffin->x;
	tsPoint.y = buffin->y;
	
	if(g_TSSave.tspixel.x < 640)
		g_TSSave.tspixel.x = 640;
	if(g_TSSave.tspixel.y < 480)
		g_TSSave.tspixel.y = 480;
	
	if(tsPoint.x < g_TSSave.tsmin.x)
		tsPoint.x = 0;
	if(tsPoint.x > g_TSSave.tsmax.x)
		tsPoint.x = g_TSSave.tsmax.x-g_TSSave.tsmin.x;
		
	if(tsPoint.y < g_TSSave.tsmin.y)
		tsPoint.y = 0;
	if(tsPoint.y > g_TSSave.tsmax.y)
		tsPoint.y = g_TSSave.tsmax.y-g_TSSave.tsmin.y;
	
	if(g_TSSave.tsmax.x != g_TSSave.tsmin.x)
		buffout->x = (g_TSSave.tspixel.x * tsPoint.x ) / (float)(g_TSSave.tsmax.x-g_TSSave.tsmin.x);
	if(g_TSSave.tsmax.y != g_TSSave.tsmin.y)
		buffout->y = (g_TSSave.tspixel.y * tsPoint.y ) / (float)(g_TSSave.tsmax.y-g_TSSave.tsmin.y);	
}

void CalculatePosition(touchscreen *buffin, touchscreen *buffout)
{
	XY	lcdPoint;
	XY	tsPoint;
	short tmp;
	
	int nTouchType = 0;
	TOUCHSCREEN tmpbuf;
	
	nTouchType = CtmConfig::GetInstance()->GetTouchPanlType();
	
	tsPoint.x = buffin->x;
	tsPoint.y = buffin->y;
	
	
	//if(g_TSSave.tsangle & 0x02)
	//{
	//	tsPoint.y = g_infoDrv.nY - tsPoint.y;
	//}
	
	//printf("1 (%d, %d)-> \n", tsPoint.x, tsPoint.y);
	
	
	//printf("2 (%d, %d)-> \n", tsPoint.x, tsPoint.y);
	if(nTouchType == 0x0008)
	{
		tmpbuf = CalculatePosition(*buffin);
		buffout->x = tmpbuf.x;
		buffout->y = tmpbuf.y;
	}
	else if(nTouchType == 0x0010)
	{
		tmpbuf = CalculatePosition(*buffin);
		buffout->x = tmpbuf.x;
		buffout->y = tmpbuf.y;
	}
	else if(nTouchType == 0x10000)
	{
		buffout->x = buffin->x;
		buffout->y = buffin->y;
	}
	else
	{
		if(g_TSSave.tsangle & 0x04)
		{
			tmp = tsPoint.x;
			tsPoint.x = tsPoint.y;
			tsPoint.y = tmp;
		}
		GetLCDPoint5(&lcdPoint, &tsPoint, &g_TSSave.tscalib);
	
		//GetLCDPoint3(&lcdPoint, &tsPoint, &g_TSSave.tsmatrix);
		
		//printf("1 (%d, %d)\n", lcdPoint.x, lcdPoint.y);
		//if(g_TSSave.tsangle & 0x01)
		//{
		//	lcdPoint.x = g_infoDrv.nX - lcdPoint.x;
		//}
		//memcpy(buffout, buffin, sizeof(touchscreen));
		buffout->x = lcdPoint.x;
		buffout->y = lcdPoint.y;
	}
	//printf("2 (%d, %d)\n", buffout->x, buffout->y);
}

/*---------------------------------------------------------------------------+
|  Function : void	Touch_RS232Func(void* pClass, char *pData, int nLength)  |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	Touch_RS232Func(void* pClass, char *pData, int nLength)
{
	CtmTouchScreen	*pInput	= (CtmTouchScreen *)pClass;
	pInput->ReadData(pData, nLength);
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTouchScreen::ReadData(char *pData, int nLength)       |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmTouchScreen::ReadData(char *pData, int nLength)
{
	unsigned char 	cData	= *(unsigned char *)pData;

    if((cData & TOUCH_STATUS_COORDINATE_MASK) == TOUCH_STATUS_COORDINATE)
    {
    	m_TouchData.nDataType   = COORDINATE;
        m_TouchData.bDataBuff[0]= cData;
        m_TouchData.nDataPos    = 1;
    }
    else
    {
        if(m_TouchData.nDataType == FIRSTDATA)  return;
        
        m_TouchData.nDataType   = COORDINATE;
        m_TouchData.bDataBuff[m_TouchData.nDataPos] = cData;
        m_TouchData.nDataPos ++;
       	if(m_TouchData.nDataPos == TOUCH_DATA_LEN)
        {
            DealData();
            //=======
        	// Reset the Data buffer statuts
        	//=======
        	m_TouchData.nDataType   = FIRSTDATA;
        	m_TouchData.nDataPos    = 0;
        }
    }
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTouchScreen::DealData()			                     |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmTouchScreen::DealData()
{
    int 			nX, nY, nTemp;
    MSG				msg;
    int             nState;
    
    if(m_TouchData.bDataBuff[2] > 0x1F || m_TouchData.bDataBuff[2] > 0x1F)  return;
    
	//=======
	// Convert coordinate of touch panel into screen's
	//=======
	nX  = WORD_ASSEMBLY(m_TouchData.bDataBuff[1], m_TouchData.bDataBuff[2]);	
	nY  = WORD_ASSEMBLY(m_TouchData.bDataBuff[3], m_TouchData.bDataBuff[4]);
	//printf("x1=%x, x2=%x, x3=%x, x4=%x nX=%x, nY=%x \n", m_TouchData.bDataBuff[1], m_TouchData.bDataBuff[2],
	//	m_TouchData.bDataBuff[3], m_TouchData.bDataBuff[4], nX, nY);
	switch(g_TSSave.tsangle)
	{
	    case 3:
	        nTemp   = nX;
	        nX      = nY;
	        nY      = TOUCH_MAXBITPOINT - nTemp;
	    	break;
	    case 2:
	        nX      = TOUCH_MAXBITPOINT - nX;
	        nY      = TOUCH_MAXBITPOINT - nY;
	   		break;
	    case 1:
	        nTemp   = nY;
	        nY      = nX;
	        nX      = TOUCH_MAXBITPOINT - nTemp;
	    	break;	     
	}

	nX  = ((nX * m_TSRun.tsdata.x >> 12) + m_TSRun.tsdelta.x) * (int)m_dXScale;
	nY  = ((nY * m_TSRun.tsdata.y >> 12) + m_TSRun.tsdelta.y) * (int)m_dYScale;

	if(nX < 0)
	{
	    nX  = 0;
	}
	if(nY < 0)
	{
	    nY  = 0;
	}
	
	if(nX > m_TSRun.tsdata.x - 1)
	{
	    nX  = m_TSRun.tsdata.x - 1;
	}
	if(nY > m_TSRun.tsdata.y - 1)
	{
	    nY  = m_TSRun.tsdata.y - 1;
	}
	
	
	//=======
	// Pose the message of Mouse up and mouse down
	//=======
	nState  = m_TouchData.bDataBuff[0] & 0x07;
	
	
	//printf("nState=%d \n", nState);
	//printf("IsIdle=%d \n", g_ptaskCmd->IsIdle());
	if(g_ptaskCmd->IsIdle())			//如果屏保，則發生cancel按鍵取消屏保
		g_ptaskCmd->SetIdle(FALSE);
	
	switch(nState)						//James modify for touch down and up
	{
		case TOUCH_DOWN:
			if(!g_bMouseDown)
			{
				m_nX	= nX;
				m_nY	= nY;
				
				msg.hwnd	= (HWND)NULL;		 
		    	msg.message	= MSG_MOUSELDOWN;
		    	msg.wParam	= (m_nX << 16) | m_nY;
		    	msg.lParam	= TOUCH_DOWN;
		    	g_pApplication->QueueMessage(&msg);
		    	g_bMouseDown = TRUE;
		    }
		    else
		    {
		    	//=======
				// 判斷是是否的移動
				//=======
		    	if((m_nX - nX) * (m_nX - nX) + (m_nY - nY) * (m_nY - nY) > 3)
				{
			    	m_nX	= nX;
					m_nY	= nY;
					
			    	msg.hwnd	= (HWND)NULL;
			    	msg.message	= MSG_MOUSEMOVE;
			    	msg.wParam	= (m_nX << 16) | m_nY;
			    	msg.lParam	= TOUCH_MOVE;//m_nState;
			    	//g_pApplication->QueueMessage(&msg); 
				}
		    }
			break;
		case TOUCH_UP:
			m_nX	= nX;
			m_nY	= nY;
					
			msg.hwnd	= (HWND)NULL;
	    	msg.message	= MSG_MOUSELUP;
	    	msg.wParam	= (m_nX << 16) | m_nY;
	    	msg.lParam	= TOUCH_UP;//0;
	        //g_pCursor->Draw(m_nX, m_nY);  
	    	g_pApplication->QueueMessage(&msg);
	    	g_bMouseDown = FALSE;
			break;
		default:
			break;	
	}
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTouchScreen::SaveTSParameter()			             |
|  Task     : Save ts para to flash                                          |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTouchScreen::SaveTSParameter(TSSAVE tsSave)
{
	int nFd = 0, nResult = 0;
	
	printf("save ts parameter\n");
	printf("signal:%X\nangle:%d\ncalculate:%d\n"
		"tsbase.x:%d\ntsbase.y:%d\nlcdbase.x:%d\nlcdbase.y:%d\nkx:%f\nky:%f\n",
		tsSave.tssignal, tsSave.tsangle, tsSave.tscalculate,
		tsSave.tscalib.tsBase.x, tsSave.tscalib.tsBase.y, 
		tsSave.tscalib.lcdBase.x, tsSave.tscalib.lcdBase.y,
		tsSave.tscalib.dKx, tsSave.tscalib.dKy);
	
	if ((nFd = open(TS_DATA_FLASH, O_RDWR)) < 0)
	{
		printf("%s flash open error, %s\n", TS_DATA_FLASH, strerror(errno));
		return FALSE;
	}
	if(lseek(nFd, TS_DATA_POSI, 0) < 0)
	{	
		printf("could not seek to device, %s\n", strerror(errno));
		return FALSE;
	}
	//printf("write ok \n");
	nResult = write(nFd, &tsSave, sizeof(tsSave));
	close(nFd);
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTouchScreen::ReadTSParameter(TSSave* pTSSave)         |
|  Task     : Read ts data from flash                                        |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmTouchScreen::ReadTSParameter(TSSave* pTSSave)
{
	int nFd = 0;
	TSSAVE u_tssave;
	if ((nFd = open(TS_DATA_FLASH, O_RDWR)) < 0)
	{
		printf("flash open error, %s\n", strerror(errno));
		return ;
	}
	if(lseek(nFd, TS_DATA_POSI, 0) < 0)
	{	
		printf("could not seek to device, %s\n", strerror(errno));
		return ;
	}
	read(nFd, &u_tssave, sizeof(u_tssave));
	memcpy(pTSSave, &u_tssave, sizeof(u_tssave));
	close(nFd);
	/*
	printf("signal=%x, angle=%d, delta.x=%d, delta.y=%d, max.x=%d, max.y=%d, min.x=%d, min.y=%d \n", 
			g_TSSave.tssignal, g_TSSave.tsangle, g_TSSave.tsdelta.x, g_TSSave.tsdelta.y,
			g_TSSave.tsmax.x, g_TSSave.tsmax.y, g_TSSave.tsmin.x, g_TSSave.tsmin.y);
	
	TSMATRIX *matrixPtr = &g_TSSave.tsmatrix;
	
	printf("A:%d B:%d C:%d D:%d E:%d F:%d divisor:%d\n",
			matrixPtr->sA, matrixPtr->sB, matrixPtr->sC, 
			matrixPtr->sD, matrixPtr->sE, matrixPtr->sF, matrixPtr->sDivisor);
	
	TSCALIB *tsCalib = &g_TSSave.tscalib;
	printf("Base ts point(%d,%d), base lcd point(%d,%d), dx:%f, dy:%f\n",
			(tsCalib->tsBase).x, (tsCalib->tsBase).y,
			(tsCalib->lcdBase).x, (tsCalib->lcdBase).y,
			tsCalib->dKx, tsCalib->dKy);
	*/
}

#ifndef	D_BEOBJECT_CtmTouchScreen
CObject*	CreateInstance(char* Name)
{
	if (g_pTouch == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CtmTouchScreen*>(pResult))->CreateSelf();
		g_pTouch = static_cast<CtmTouchScreen*>(pResult);
	}
	return g_pTouch;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmTouchScreen*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

