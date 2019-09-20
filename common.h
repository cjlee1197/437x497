/*===========================================================================+
|  System   : Common                                                         |
|  Task     : Common header file                                             |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 03/30/1995                                                     |
|  Revision : 03/25/1996                                                     |
+===========================================================================*/

#ifndef     D_COMMON
#define     D_COMMON

#include	<ctype.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<math.h>
#include	<pthread.h>
#include 	<semaphore.h>
#include 	<signal.h>
#include    <stdio.h>
#include	<stdlib.h>
#include    <string.h>
#include 	<termios.h>
#include	<time.h>
#include	<unistd.h>
#include 	<dlfcn.h>

#include	<linux/fb.h>
#include	<linux/kd.h>
#include	<sys/ioctl.h>
#include	<sys/mman.h>
#include	<sys/stat.h>
#include	<sys/time.h>
#include	<sys/timex.h>
#include	<sys/types.h>

#include 	<sys/socket.h>
#include 	<netinet/in.h>

#include	<getopt.h>
#include	<sys/socket.h>
#include	<net/if.h>
#include	<sys/mount.h>
#include	<dirent.h>
#include	<sys/vfs.h>
#include	"tmmsg.h"
#ifdef		D_X86		//yang 2006/11/29 
#pragma pack (2) 
#endif
/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
//#define RGB565TORGB888(c) ((((c)>>8)&0xf8)<<16 | (((c)>>3)&0xfc)<<8 | ((c)<<3)&0xf8 )
#define RGB565TORGB888(c) ((((c)>>8)&0xf8 | (((c)>>11)&0x7))<<16 | (((c)>>3)&0xfc | (((c)>>5)&0x3))<<8 | ((c)<<3)&0xf8 | ((c)&0x7) )

#define RGB2PIXEL888(r,g,b)	\
	(((r) << 16) | ((g) << 8) | (b))

#define RGB2PIXEL565(r,g,b)	\
	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

//#ifdef	D_32BIT
//#define	RGB2PIXEL	RGB2PIXEL888
//#else
//#define	RGB2PIXEL	RGB2PIXEL565
//#endif

#define	RGB2PIXEL	RGB2PIXEL565

#define     B2W(b)              ((WORD)((BYTE)(*(b))) << 8) + (BYTE)(*(b+1))    // Fetch 2 continuous bytes pointed by b to a word
#define     ABS(n)              (((n) >= 0) ? (n) : -(n))


/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		PACKED	__attribute__((packed, aligned(1)))

#define		FALSE				0
#define		TRUE				1			
//=======
// 資料類型定義
//=======

#define		TYPE_UNKNOWN	0
#define		TYPE_BOOL		1
#define		TYPE_BYTE		2
#define		TYPE_WORD		3
#define		TYPE_DWORD		4
#define		TYPE_CHAR		5
#define		TYPE_SHORT		6
#define		TYPE_INT		7
#define		TYPE_FLOAT		8
#define		TYPE_DOUBLE		9
#define		TYPE_STRING		10

#define     BLACK               RGB2PIXEL(0,0,0)
#define     BLUE                RGB2PIXEL(0,0,165)
#define     GREEN               RGB2PIXEL(0,165,0)
#define     CYAN                RGB2PIXEL(120,160,255)
#define     RED                 RGB2PIXEL(165,0,0)
#define     MAGENTA             RGB2PIXEL(168,0,255)
#define     BROWN               RGB2PIXEL(120,255,168)
#define     WHITE               RGB2PIXEL(228,228,228)
#define     GRAY                RGB2PIXEL(84,84,84)
#define     LIGHTBLUE           RGB2PIXEL(0,0,255)
#define     LIGHTGREEN          RGB2PIXEL(0,255,0)
#define     LIGHTCYAN           RGB2PIXEL(0,255,255)
#define     LIGHTRED            RGB2PIXEL(255,0,0)
#define     LIGHTMAGENTA        RGB2PIXEL(255,0,255)
#define     YELLOW              RGB2PIXEL(255,255,0)
#define     BRIGHTWHITE         RGB2PIXEL(252,252,252)

#define     tmCHINESE_CODE        0xA0 

#define     LIGHTGRAY           BROWN

#define		tmWIDTH				800
#define		tmHEIGHT			600

#define     tmMASK_BIT0           0x01
#define     tmMASK_BIT1           0x02
#define     tmMASK_BIT2           0x04
#define     tmMASK_BIT3           0x08
#define     tmMASK_BIT4           0x10
#define     tmMASK_BIT5           0x20
#define     tmMASK_BIT6           0x40
#define     tmMASK_BIT7           0x80
#define     tmMASK_BIT8           0x0100
#define     tmMASK_BIT9           0x0200
#define     tmMASK_BIT10          0x0400
#define     tmMASK_BIT11          0x0800
#define     tmMASK_BIT12          0x1000
#define     tmMASK_BIT13          0x2000
#define     tmMASK_BIT14          0x4000
#define     tmMASK_BIT15          0x8000

//=========
// 			_IODef
//=========

#define     tmSSD_BANK            4
#define     tmSSD_SECTSZB         256
#define     tmSSD_DATASZB         (32768*tmSSD_BANK)            // (SSD_DATASZK*1024)
#define     tmSSD_DATASZS         (128*tmSSD_BANK)              // (SSD_DATASZB/SSD_SECTSZB)

//======
//			View
//======
#define		tmVIEW_FGCOLOR			BLACK
#define		tmVIEW_BGCOLOR			LIGHTGRAY

//=======
//		Database
//=======
#define     _CE2FA              1
#define     _MM2IN              1
#define     _BA2PS              1
#define     tmPREC_POSI         1
#define     tmMIN_PRIVILEGE       0
#define     tmMAX_PRIVILEGE       9
#define     tmLOGIN_INVALIDID         0x8001
#define     tmLOGIN_INVALIDPASSWORD   0x8002

#define     tmSPDMUL              1
#define     tmPREC_SPD            0

//=======
//		DBCtrl
//=======
#define     tmDSPHYDRMAX0                65530
#define     tmDSPHYDRMAX1                65531
#define     tmDSPHYDRMAX2                65532
#define     tmMAX_daInjPres              177  //Sunny<20070912>
#define		tmMAX_daPres			        140// 210    //Sunny<20070912> amend
#define     tmMAX_daSpd                    99
#define     tmMAX_daSpd_66                 66
#define     tmMAX_daSpd_170               170

#define     tmMAX_daPresCls               200
#define     tmMAX_daPresNzl               100
#define     tmMAX_daSpdNzl                80
#define     tmMAX_daRamp                  999

#define     tmMAX_daDly_2                 99
#define     tmMAX_daDly_3                 999
#define     tmMAX_daDly_4                 9999
#define     tmMAX_daOut                   4095
#define     tmMAX_daGroup                 8

#define     tmMAX_adPos_2                 99
#define     tmMAX_adPos_3                 999
#define     tmMAX_adPos_4                 9999
#define     tmMAX_adPos_5                 99999
#define     tmMAX_adPos_6                 999999
#define     tmMAX_adPos_500               500
#define     tmMAX_wTime_65                65535

#define     tmMAX_wTime_2                 99
#define     tmMAX_wTime_3                 999
#define     tmMAX_wTime_4                 9999
#define     tmMAX_wTime_5                 99999
#define     tmMAX_wTime_500               500
#define     tmMAX_wTemp                   450*_CE2FA         // 399
#define     tmMAX_wTemp_2                 99
#define     tmMAX_wTemp_9                 9
#define     tmMAX_wTemp_10                10
#define     tmMAX_wTemp_15                15
#define     tmMAX_wTemp_50                50
#define     tmMAX_wTemp_60                60
#define     tmMAX_wTemp_65                65
#define     tmMAX_wTemp_70                70
#define     tmMAX_wTemp_120               120
#define     tmMAX_wTemp_200               200
#define     tmMAX_wTemp_300               300
#define     tmMAX_daPercent               100
#define     tmMAX_wTons_1000              1000

#define     tmMAX_wTempState              3
#define     tmMAX_wCount_1                9					//<<yang 2006/2/8 
#define     tmMAX_wCount_2                99
#define     tmMAX_wCount_3                999
#define     tmMAX_wCount_4                9999
#define     tmMAX_wCount_5                99999
#define     tmMAX_wCount_6                999999
#define     tmMAX_wCount_7                9999999
#define     tmMAX_wCount_8                99999999
#define     tmMAX_wCount_9                999999999
#define     tmMAX_wCount_10               9999999999
#define     tmMAX_wMotor_1800             1800
#define     tmMAX_wMotor_300              300
#define     tmMAX_wMotor_100              100
#define     tmMAX_wMotor_6                6
#define     tmMAX_ALARM_6000              6000
#define     tmMAX_daTravel                0x3000
#define     tmMAX_daTrvlDEC               30000
#define     tmMAX_wWORD                   0xFFFF  //65535
#define     tmMAX_daTravel54              0x9999
#define     tmMAX_daTrvlDEC54             65535

#define     tmMAX_wRpm                    999
#define     tmMAX_dwShotCount             99999
#define     tmMAX_wPackcount              9999

#define     tmMAX_wBOOL                   1
#define     tmMAX_wSwitch                 1
#define     tmMAX_wFunction1              1
#define     tmMAX_wFunction2              2
#define     tmMAX_wFunction3              3
#define     tmMAX_wFunction4              4
#define     tmMAX_wFunction5              5
#define     tmMAX_wFunction8              8     //Anny2006/2/20
#define     tmMAX_wFunction10             10
#define     tmMAX_wFunction12             12    //Anny2006/2/19
#define     tmMAX_wFunction18             18    //Anny2006/2/19

#define     tmDB_INDEX_NULL               0xFFFF 
#define     tmMAX_HEXCount_3              0x0999
#define     tmMAX_HEXCount_4              0x9999
#define     tmMAX_wMoldSet                100

#define     tmMAX_daVel_4               9999        //<<<Anny2006/2/13
#define     tmMAX_daVel_5               99999
#define     tmMAX_daVel_6               999999

#define     tmMAX_daPres_4              9999 
#define     tmMAX_daPres_5              99999

#define     tmMAX_adPos_7               9999999

#define     tmMAX_wTime_6               999999

#define     tmMAX_daPercent10000        10000
#define     tmMAX_wAccelDecel_7         9999999
#define     tmMAX_wAccelDecel_8         99999999
#define     tmMAX_wAccelDecel_9         999999999
#define     tmMAX_wForce_5              99999
#define		tmMAX_wForce_6              999999
#define     tmMAX_wForce                9999999
#define     tmMAX_wForce_8              99999999
#define     tmMAX_wForce_9              999999999
#define     tmMAX_wJerks                9999999
#define     tmMAX_wJerks_8              99999999
#define     tmMAX_wJerk_Angular_8       99999999
#define     tmMAX_wWork                 99999999
#define     tmMAX_wWork_7               9999999
#define     tmMAX_wWork_9               999999999
#define     tmMAX_wTorque_8             99999999
#define     tmMAX_wTemp_9999            9999
#define     tmMAX_wTemp_999999          999999
#define     tmMAX_wTime_7               9999999   //Anny2006/3/16
#define     tmMAX_daSpe_4               9999        //>>>Anny2006/2/13

//=======
//		DataBase
//=======

//=======
//		Timer
//=======
#define		tmTIMERID						1
#define		TIMER_STATUS					2

//=======
//		NoWin Object Handle
//=======

#define     tmMAX_WINDOWS         		500
#define		tmHANDEL_CRSCOMM			tmMAX_WINDOWS+1

//=======
// MAX
//=======
#define     MAX_TASKS           128

//=======
// System Error ID
//=======

#define ERR_OK                   0
#define ERR_INV_HWND            -1
#define ERR_INV_TASK            -1
#define ERR_QUEUE_FULL          -2

#define ERR_INVALID_HANDLE      -3
#define ERR_INVALID_HMENU       -4
#define ERR_INVALID_POS         -5
#define ERR_INVALID_ID          -6
#define ERR_RES_ALLOCATION      -7

#define ERR_CTRLCLASS_INVNAME   -8
#define ERR_CTRLCLASS_INVLEN    -9
#define ERR_CTRLCLASS_MEM       -10
#define ERR_CTRLCLASS_INUSE     -11

#define ERR_ALREADY_EXIST       -12
#define ERR_NO_MATCH            -13
#define ERR_BAD_OWNER           -14

#define HWND_DESKTOP        0
#define HWND_INVALID        0xFFFFFFFF
#define	HTSK_MAIN			MAX_TASKS + 1
#define	HTSK_INVALID		0xFFFFFFFF
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef     short               BOOL;
typedef     unsigned char       BYTE;
typedef     unsigned short      WORD;
typedef     long                LONG;
typedef     unsigned long int   DWORD;
typedef     DWORD               FGC;    	// Foreground color
typedef     DWORD               BGC;    	// Background color
typedef     WORD                tmHWND;   	// Handle of window
typedef 	unsigned int    	UINT;
typedef 	UINT            	WPARAM;
typedef 	DWORD           	LPARAM;
typedef		time_t				UTIME;
typedef     DWORD               _TIMEL;
typedef     DWORD               _COUNTL;
typedef     DWORD               _FLAGL;

typedef		char				INT8;
typedef		short				INT16;
typedef		long				INT32;
typedef		long long			INT64;

typedef		unsigned char		UINT8;
typedef		unsigned short		UINT16;
typedef		unsigned long		UINT32;
typedef		unsigned long long	UINT64;
typedef		unsigned int		UINT;

typedef		unsigned int		HTSK;
typedef 	unsigned int 		HWND;

typedef		unsigned int		HTSK;
typedef 	unsigned int 		HWND;

#define		__far
#define		__near
#define		__huge	

typedef struct tagtmSIZE
{
    short   w;
    short   h;
} tmSIZE;

typedef	struct tagtmDATE
{
	BYTE	day;
	BYTE	month;
	WORD	year;
	BYTE	dayofweek;
	#if		defined(D_X86)||defined(D_3354)
	BYTE	bR1;
	WORD	wR2;
	#endif
}	tmDATE;

typedef	struct	tagtmTIME
{
	BYTE	hour;
	BYTE	minute;
	BYTE	second;
	BYTE	hsecond;
}__attribute__((packed, aligned(2))) tmTIME;

typedef struct	tagtmDateTime
{
	tmDATE	date;
	tmTIME	time;
} tmDATETIME;

typedef struct tagRECT
{
    short   left;
    short   top;
    short   right;
    short   bottom;
}	RECT;

typedef	struct tagPOINT    //netdigger 2007/4/17
{
	int 	nX;
	int 	nY;
} POINT;

typedef struct tagXY
    {
    short   x;
    short   y;
    } XY;
	
typedef struct tagXYRANGE
    {
    short   x1;
    short   y1;
    short   x2;
    short   y2;
	short   flag;
    } XYRANGE;
    

typedef struct tagCW
{
    BYTE    cL;
    BYTE    cH;
} CHL;

typedef union
{
    CHL     c;
    WORD    w;
} CW;

typedef struct tagtmCOLOR
{
    BYTE    r;
    BYTE    g;
    BYTE    b;
} tmCOLOR;


//	fans add 2009-12-22 15:47:41
typedef struct	tagtmGPSData
{
	double 	Longitude;
	double	Latitude;
	double	Altitude;
	char 	DataTime[8];
}tmGPSData;
    
/*---------------------------------------------------------------------------+
|           I/O type                                                         |
+---------------------------------------------------------------------------*/
typedef     WORD                _DA;
typedef     WORD                _AD;
typedef     WORD                _TIMER;
typedef     WORD                _COUNT;
typedef     WORD                _FLAG;

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
#define		WORDCH(w)			((w>>8) | (w<<8))

#define     tmLOWORD(l)           ((WORD)(l))
#define     tmHIWORD(l)           ((WORD)((DWORD)(l) >> 16))
#define     tmLOBYTE(w)           ((BYTE)(w))
#define     tmHIBYTE(w)           ((BYTE)((WORD)(w) >> 8))

#define		LOBYTE(w)			tmLOBYTE(w)
#define		HIBYTE(w)			tmHIBYTE(w)

//	fans add

#endif
