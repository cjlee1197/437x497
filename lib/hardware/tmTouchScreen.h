/*==========================================================================+
|  Class    : Class TouchScreen                                             |
|  Task     : Class TouchScreen header file                          		|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|           : V2.00   James                                                 |
|  Creation : 2006/12/21                                                    |
|  Revision : 2006/12/21                                                    |
|           : 2010/01/25    V2.00                                           |
+==========================================================================*/
#ifndef D__DTMTOUCHSCREEN
#define	D__DTMTOUCHSCREEN

#include "../../rs232.h"
#include "../../touchscreen.h"
#include 	"../../main.h"
#include 	"../../tmconfig.h"
#include 	"../../utils.h"
#include	"../../cursor.h"

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void *(* PRSRUN)(void *);

//ts para
typedef struct touchscreen
{
    short 			p;					//1 = press key down 0 = press key up
    short 			x;					//x pos
    short 			y;					//y pos
    short 			m;					//accumulated millisecs 
}TOUCHSCREEN;

/*
 * according to the equation:
 * X = AX'+BY'+C
 * Y = DX'+EY'+F
 * we need to save the six coefficients for calculating the LCD location. 
*/
typedef struct TSMatrix
{
	short			sA;	/* A=sA/sDivisor */
	short			sB; /* B=sB/sDivisor */
	short			sC; /* C=sC/sDivisor */
	short			sD; /* D=sD/sDivisor */
	short			sE; /* E=sE/sDivisor */
	short			sF; /* F=sF/sDivisor */
	short			sDivisor; //zoom factor
} TSMATRIX;

typedef struct TSCalibration
{
	XY			tsBase;
	XY			lcdBase;
	double		dKx;
	double		dKy;
} TSCALIB;

//current ts para
typedef	struct TSRun
{
	int				tshandle;			//current hanle
	int				tsstate;			//touch state 0=down 1=move 2=up
	touchscreen		tsdata;				//current ts data
	XY				tsdelta;			//off data
	BOOL			tsidle;				//false =press key up, true = press key down
}TSRUN;

//flash save ts para
typedef	struct TSSave {
	unsigned int	tssignal;			//touch panl identification point
	int				tsangle;			//touch panl angle type	bit0 = x; bit1 = y; bit2 =x,y swap
	int				tscalculate;		//touch panl calculate by order
	XY				tsdelta;			//touch panl hiden offset
	XY				tsmax;				//touch panl max AD data
	XY				tsmin;				//touch panl min AD data
	XY				tspixel;			//width and length
	TSCALIB			tscalib;			//calibration parameter
	TSMATRIX		tsmatrix;
}TSSAVE;

extern	void	Touch_RS232Func(void* pClass, char *pData, int nLength);
extern	void	Touch_ADS7843Func(void* pdata);
extern  void	USB_MouseFunc(void* pdata);	
extern  void	USB_MouseFunc2(void* pdata);	
extern  void	USB_MouseFunc2_Check(void* pdata);	
extern  void	USB_MouseFunc4(void* pdata);	
extern  void	USB_MouseFunc5(void* pdata);	

class CtmTouchScreen: public CTouchScreen
{
	friend	void	Touch_RS232Func(void* pClass, char *pData, int nLength);
	friend	void	Touch_ADS7843Func(void* pdata);
	friend  void	USB_MouseFunc(void* pdata);			//James add 2010/4/26 for usb mouse
	friend  void	USB_MouseFunc2(void* pdata);			//James add 2010/4/26 for usb mouse
	friend  void	USB_MouseFunc2_Check(void* pdata);			//James add 2010/4/26 for usb mouse check
	friend  void	USB_MouseFunc4(void* pdata);			//James add 2010/4/26 for usb mouse
	friend  void	USB_MouseFunc5(void* pdata);			//James add 2010/4/26 for usb mouse
	DECLARE_DYNCREATE(CtmTouchScreen)	
	public:
		CtmTouchScreen();
		~CtmTouchScreen();
	public:	
		int		CreateSelf();
		int		FreeSelf();
		
		void    SetAngle(int nAngle);
		void    SetOffset(int nX, int nY);
		void    SetScale(int nCoor, double dScale);
		int		SetAdjustData(XY *lcdPoints, XY *tsPoints, int nCount); 
		int		SetAdjustData(void*	pData);
		BOOL	SetAdjustState(int	nState);
		int		SaveTSParameter(TSSAVE tsSave);
		void	ReadTSParameter(TSSave* pTSSave);
		
	protected:
		int		Create(void* pDevice = NULL);
		WORD	Open();
		WORD	Open_RS232();
		WORD	Open_Ads7843();
		WORD	Open_UsbMouse();
		WORD	Open_UsbMouse2();
		WORD	Open_UsbMouse4();
    	WORD    Close();
    private:
    	void	ReadData(char *pData, int nLength);		
    	void    DealData();
    	int		SetCalibration3(int nCount, XY *lcdPoints, XY *tsPoints, TSMATRIX *matrixPtr);
    	int		SetCalibration5(int nCount, XY *lcdPoint, XY *tsPoint, TSCALIB *tsCalib);
    	void	SwapXY(XY *point);
	protected:
		CDevice			*m_pDevice;
		char			m_szTxBuffer[16];
		pthread_t		m_TSReadthread;
		pthread_t		m_MouseReadthread;
	private:
		TSRUN	m_TSRun;
	public:
		short	m_TSAdjust;
		CIniFile	m_TouchIni;
	private:
		tmRS232	m_rs232;
		
		TOUCHDATA   m_TouchData;
		short	m_nX;
		short	m_nY;
		int		m_nTokenState;
		int		m_nState;					// Ĳ�N���A
		
		double  m_dXScale;
		double  m_dYScale;
		BYTE	m_acTxBuffer[16];
};

extern	void	Touch_RS232Func(void* pClass, char *pData, int nLength);
extern	void	Touch_ADS7843Func(void* pdata);
extern	TOUCHSCREEN	CalculatePosition(touchscreen 	readbuf);
void	SetMsgwParam(MSG *msg,int x, int y, int Rotation);

#ifndef	D_BEOBJECT_CTMTOUCSHSCREEN
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
