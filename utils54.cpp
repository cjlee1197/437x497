/*===========================================================================+
|  Function : Utilities                                                      |
|  Task     : Utilities                                                      |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50                                                     |
|  Link     : MSVC V1.50                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 04/27/1996                                                     |
|  Revision : 08/27/1997                                                     |
+===========================================================================*/
 
#include    "stable.h"
#include    "profile.h"
#include    "app.h"
#include    "utils.h"
#include    "statbar.h"
#include	"main.h"
#include	"mainfrm.h"	//	fans add
#include	"tmconfig.h"
#include	"commonaction.h"
#include	"tmdebug.h"
#include	"selvat_ancestor.h"
#include	"font.h"
#include	"readdata.h"
#include	"tmshmsg.h"
#include 	"recordset.h"
#include 	"onlinehelp.h"
#include 	"textview.h"
#include   <stdarg.h>	//	fans add
#include	<sys/io.h>
#include	<string.h>
#include    <map>
#include 	"spinbox.h"
#include	"canMaster.h"
/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      CApp*               g_pApplication; // Application processor -- Defined in "main.cpp"
extern 		CtmMainFrame*		g_pMainFrame;   // Main window -- Defined in "main.cpp"	fans add
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     MAX_FILE				10
#ifdef	D_PXA270
#define     LCD_CMD_BACKONOFF       0x4015
#define     LCD_CMD_SIGNONOFF       0x4012
#define     LCD_CMD_DISPONOFF       0x4013
#define     LCD_CMD_POWRONOFF       0x4014
#define		BUZ_CMD_ONOFF			0x4011
#define  	GET_STUT_BATTERY        0xC080
#ifdef	D_3354
#define		SAVE_CURRENTVIEW_PATH	"/media/sda1/print/"
#else
#define		SAVE_CURRENTVIEW_PATH	"/mnt/usb/print/"
#endif
#define		UPDATE_PROGRAM_CMD		"cp -dpRf /mnt/usb/hmi /usr/i86"
#define		UPDATE_SOURCE_PATH		"/mnt/usb/hmi"
#define		DEVICE_SAVE_NAME		"/dev/sda1"
#define		DEVICE_MOUNT_DIR		"/mnt/usb"
#else
#define     LCD_CMD_BACKONOFF       1
#define     LCD_CMD_SIGNONOFF       2
#define     LCD_CMD_DISPONOFF       3
#define     LCD_CMD_POWRONOFF       4
#define		BUZ_CMD_ONOFF			9


#ifdef	D_3354
#define		SAVE_CURRENTVIEW_PATH	"/media/sda1/print/"
#else
#define		SAVE_CURRENTVIEW_PATH	"/mnt/usb/print/"
#endif
#define		UPDATE_PROGRAM_CMD		"cp -dpRf /mnt/mmc/hmi /usr/i86"
#define		UPDATE_SOURCE_PATH		"/mnt/mmc/hmi"
#endif

#ifdef	D_3354
#define		SAVE_CURRENTVIEW_MMCPATH	"/media/sda1/print/"
#define		SAVE_CURRENTVIEW_USBPATH	"/media/sda1/print/"
#else
#define		SAVE_CURRENTVIEW_MMCPATH	"/mnt/usb/print/"
#define		SAVE_CURRENTVIEW_USBPATH	"/mnt/usb/print/"
#endif

#define		ERROR_OBJECT_N2C		1
#define		ERROR_OBJECT_E2C		2
#define 	ERROR_OBJECT_AUSTONE	100 
#define		ERROR_OBJECT_SIEMENS	101

#define		MMI_KERNEL_VERSION1		12
#define		MMI_KERNEL_VERSION2		20190910//20170426
#define		MMI_KERNEL_VERSION3		1
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
char 		szPropmt[15][256];

WORD        g_nDayOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};       //KV2002/12/05 ADD __based(__segname("SYSTEM"))

WORD        g_wIDView;								//yang 2006/4/19 
WORD        g_flMotorStatus    = 0;					// 520 馬達狀態
WORD        g_flHeaterStatus   = 0;					// 521 電熱狀態
WORD        g_flMotor2Status    = 0;					// 520 馬達2狀態
WORD        g_flHeater2Status   = 0;					// 521 電熱2狀態
WORD        g_flHeaterStatusB  = 0;					//模溫機使用
MONIUPDATE  g_MoniUpdate = {0, FALSE};    // St1217'01 for moni lose
BOOL        g_bShortCutKeyEnterView = FALSE;
char        g_acSavePageAndPassword[MAX_PAGE_CHARCOUNT];
short       g_nSaveCharCount = 0;
BYTE		g_cAirBlastCKey = 0;
BYTE		g_cAirBlastDKey = 0;
int			g_nRTCFd		= -1;
int			g_nBatteryFd	= -1;
int			g_ntmDrive		= -1;			//james add for 3354
WORD        g_wOperationCount = 0; 					//<Sunny20070327> 操作計數
BOOL		g_bInitDone;
BOOL		g_bScreenSaverRelease =FALSE;				//Sunny<20070405>
WORD		g_wOPStatus=0;    			//紀錄當前機器操作狀態
char		g_szTrimStr[1024];
WORD		g_wInstWay;					//Sunny<20090421> inst way
int			g_nUpdateDevice = -1;
CtmWnd*		g_pWndInput = NULL;
char				g_pFileExplPath[1024]		="//work/CNC/Prog/";

CtmWnd*		g_pwndHelp	= NULL;
CtmWnd*		g_pWndInputForm = NULL;
CtmWnd*		g_pWndDataSetForm = NULL;
CtmWnd*		g_pWndDataSetForm2 = NULL;
long long 	g_lDataSetValue = 0;
char		g_pszDataTextID[256];
static BOOL	bLogin = FALSE;
int			g_nViewChange = 0;
char		g_cPszFileName[256];
BOOL		g_bDemoDataID	= FALSE;

static WORD crc_tab16[256];
static DWORD crc_tab32[256];
static int  crc_tab16_init;
static int  crc_tab32_init;

static BOOL	 u_bHelpTimerInit;
static	long u_lHelpTimer;

static BOOL		u_bSetOnlineHelp;
static CtmReadData u_HelpReader;

static pthread_mutex_t	datetimeMutex, systemMutex;		/*Caozz add 2013/8/7 下午 01:25:12*/
static BOOL	bDateTimeMutexInit, bSystemMutex;	
extern BOOL	g_bFocusChanged;

TMSELVATPARAM	u_tmSelvatParam;
std::map<double, double> u_mCrossMoldMap;
#define	MAX_DIVISOR_NUM	2000

int		SetOnlineHelp(char *pszName, CtmWnd *pwnd)
{
	static int iRet = 0;	/* remain the old return value */
	int i, j = 0;
	char szName[64], szHelp[128], szChild[128];
	CtmWnd *pwndChild = NULL, *pwndHelp = NULL;
	
	if (u_bSetOnlineHelp)
		return iRet;
	
	u_bSetOnlineHelp = TRUE;
	
	memset(szHelp, 0, sizeof(szHelp));
	memset(szChild, 0, sizeof(szChild));
	
	do {
		if (pszName == NULL || pwnd == NULL)
		{
			iRet = -1;
			break;
		}	
		sprintf(szName, "%s%s.txt", CtmConfig::GetInstance()->GetHelpPath(), pszName);
		//printf("Set Online Help: %s\n", szName);
		if (!u_HelpReader.OpenFileStr(szName))
		{
			iRet = 0;
			break;
		}
		
		pwndHelp = u_HelpReader.GetControlStr();
		for (i = 0; i < pwndHelp->GetControlCount(); i++)
		{
			pwndChild = pwndHelp->Controls[i];
			pwndChild->GetPropValueT("default", szChild, sizeof(szChild));	/* dbid str */
			pwnd->GetPropValueT("dbid0", szHelp, sizeof(szHelp));
			
			j = 0;
			
			if ((szChild[0] != 0 && !strncmp(szChild, szHelp, sizeof(szHelp))) ||
				!strcmp(pwnd->Name, pwndChild->Name))	/* got it */
			{
				for (; j < MAX_HELP_NUM; j++)
				{
					sprintf(szName, "value%d", j);
					pwndChild->GetPropValueT(szName, szChild, sizeof(szChild));
					if (szChild[0] == 0)
						break;
						
					//sprintf(szName, "helpstrID%d", j);
					//pwnd->SetPropValueT(szName, szChild);
					pwnd->SetHelpStrID(szChild, j);
				}
				
				break;
			}
		}
		
		iRet = j;
	} while(0);

	return iRet;
}



void	InitHelpTimer()
{
	u_lHelpTimer = 3000;
	
	if (!u_bHelpTimerInit)
	{
		AddTimer(&u_lHelpTimer);
		u_bHelpTimerInit = TRUE;
	}
}

void	DelHelpTimer()
{
	DeleteTimer(&u_lHelpTimer);
	u_bHelpTimerInit = FALSE;
}

void	ResetHelpTimer()
{
	//printf("Reset Online Help\n");
	u_bSetOnlineHelp = FALSE;
	u_lHelpTimer = 3000;
}

BOOL	HelpTimeout()
{	
	return u_lHelpTimer < 0;
}
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|  Function : void GetDate(DATE* pdate)                                      |
|  Task     : Get system date                                                |
+----------------------------------------------------------------------------+
|  Call     : GetDate(pdate)                                                 |
|                                                                            |
|  Parameter: pdate                     - Pointer to DATE structure          |
|                                                                            |
|  Return   : *pdate                    - Current system date                |
+---------------------------------------------------------------------------*/
void        GetDate(tmDATE* pdate)
{
	if (!bDateTimeMutexInit)
	{
		bDateTimeMutexInit = TRUE;
		pthread_mutex_init(&datetimeMutex, NULL);
	}
	
	pthread_mutex_lock(&datetimeMutex);
#ifdef	D_X86
	time_t			Time;
	struct	tm		tmTime;

	time(&Time);
	tmTime	=*localtime(&Time);
	
	pdate->day		= tmTime.tm_mday;
	pdate->month	= tmTime.tm_mon 	+ 1;
	pdate->year		= tmTime.tm_year 	+ 1900;
	pdate->dayofweek= tmTime.tm_wday;
#else
#ifdef	D_ARM
	#ifdef	D_3354
	time_t			Time;
	struct	tm		tmTime;

	time(&Time);
	tmTime	=*localtime(&Time);
	
	pdate->day		= tmTime.tm_mday;
	pdate->month	= tmTime.tm_mon 	+ 1;
	pdate->year		= tmTime.tm_year 	+ 1900;
	pdate->dayofweek= tmTime.tm_wday;
	#else
	GetRTCDate(pdate);
	#endif
#endif	
#endif

	pthread_mutex_unlock(&datetimeMutex);
	
}

void		GetRTCDateTime(tmDATETIME* pdate)
{
	tmDATETIME  datetime;
	
	if(g_nRTCFd < 0)	return ;
	if((read(g_nRTCFd,&datetime, sizeof(tmDATETIME))) == sizeof(tmDATETIME))
		*pdate = datetime;
}

//	fans add 2006/12/30 10:31上午
void	GetRTCDate(tmDATE* pDate)
{
	tmDATETIME  datetime;
	pDate->year		= 2007;
	pDate->month	= 1;   
	pDate->day		= 1;   
	
	if(g_nRTCFd < 0)	return ;
	if((read(g_nRTCFd,&datetime, sizeof(tmDATETIME)))== sizeof(tmDATETIME))
	{
		pDate->year 	= datetime.date.year;
		pDate->month 	= datetime.date.month;
		pDate->day 		= datetime.date.day; 
		pDate->dayofweek= datetime.date.dayofweek;
		if (pDate->dayofweek == 7) pDate->dayofweek = 0;
	}
}

void	GetRTCTime(tmTIME *pTime)
{
	tmDATETIME  datetime;
	pTime->hour				= 0; 
	pTime->minute			= 0;
	pTime->second			= 0;
	if(g_nRTCFd < 0)	return ;
	if((read(g_nRTCFd,&datetime, sizeof(tmDATETIME)))== sizeof(tmDATETIME))
	{
		pTime->hour  	= datetime.time.hour;  
		pTime->minute 	= datetime.time.minute;  
		pTime->second 	= datetime.time.second;   
	}
	else		//20071127  開放
	{
		datetime.date.year		= 2007; 
		datetime.date.month		= 1;
		datetime.date.day		= 1;  
		datetime.time.hour		= 0; 
		datetime.time.minute	= 0;
		datetime.time.second	= 0;
		
		SetRTCDate(datetime.date);
		SetRTCTime(datetime.time);
		
	}   
}

int		SetRTCDate(tmDATE date)
{
	if(g_nRTCFd < 0) return DATE_ERROR_MONTH;
	
	//g_tmDebugInfo->PrintDebugInfo("Start SetRTCDate!\n");
	tmDATETIME  datetime;
	if(date.year < 2000)	date.year = date.year+2000;
	  
	if(read(g_nRTCFd, &datetime, sizeof(tmDATETIME)) != sizeof(tmDATETIME))
	{
		datetime.time.hour   = 0; 
    	datetime.time.minute = 0;
    	datetime.time.second = 0;
	}
	datetime.date.year	= date.year; 
    datetime.date.month	= date.month;
    datetime.date.day	= date.day;
    datetime.date.dayofweek = 0;
	if(write(g_nRTCFd, &datetime,sizeof(tmDATETIME)))
	{
		//g_tmDebugInfo->PrintDebugInfo("End SetRTCDate!\n");
		return DATE_OK;	
	}
	else
	{
		//g_tmDebugInfo->PrintDebugInfo("End SetRTCDate!\n");
		return DATE_ERROR_MONTH;
	}
	
}
int		SetRTCTime(tmTIME time)
{
	if(time.hour   > 23) return TIME_ERROR_HOUR;   
	if(time.minute > 59) return TIME_ERROR_MINUTE;
	if(time.second > 59) return TIME_ERROR_SECOND;
	if(g_nRTCFd < 0)	return TIME_ERROR_HOUR;
	tmDATETIME  datetime;
	 
	if((read(g_nRTCFd, &datetime, sizeof(tmDATETIME)))!= sizeof(tmDATETIME))
	{
		datetime.date.year	= 2007; 
	    datetime.date.month	= 1;
	    datetime.date.day	= 1;
	    datetime.date.dayofweek = 0;
	}
	datetime.time.hour   = time.hour; 
    datetime.time.minute = time.minute;
    datetime.time.second = time.second;
	if(write(g_nRTCFd, &datetime,sizeof(tmDATETIME)))
	{
		return DATE_OK;
	}
	else 
	{
		return TIME_ERROR_HOUR;
	}
}
void		GetSiemensDate(DWORD dwDate, tmDATE* pdate)						//<<yang 2006/3/8 
{
	time_t			Time;
	struct	tm		tmTime;
	
	Time = (time_t)((dwDate+((1992-1970)*365)+5)*24*60*60);
	tmTime	=*localtime(&Time);
	
	pdate->day		= tmTime.tm_mday;
	pdate->month	= tmTime.tm_mon 	+ 1;
	pdate->year		= tmTime.tm_year 	+ 1900;
	pdate->dayofweek= tmTime.tm_wday;
}

/*---------------------------------------------------------------------------+
|  Function : void GetDate(char* pszDate, char cSeparator)                   |
|  Task     : Get system date                                                |
+----------------------------------------------------------------------------+
|  Call     : GetDate(pszDate, cSeparator)                                   |
|                                                                            |
|  Parameter: pszDate                   - Pointer to date string             |
|             cSeparator                - Separator among the year,month,day |
|                                                                            |
|  Return   : *pszDate                  - Current system date                |
|                                         Format : MM/DD/YY                  |
+---------------------------------------------------------------------------*/
void        GetDate(char* pszDate, char cSeparator)
{
	tmDATE        date;
	
	GetDate(&date);
	
	if (cSeparator != 0)
	    sprintf(pszDate, "%02u%c%02u%c%02u", date.month, cSeparator, date.day, cSeparator, date.year%100);
	else
	    sprintf(pszDate, "%02u%02u%02u", date.month, date.day, date.year%100);
}

WORD        SetDate(tmDATE date)
{
	int nRet = DATE_ERROR_MONTH;
	if (!bDateTimeMutexInit)
	{
		bDateTimeMutexInit = TRUE;
		pthread_mutex_init(&datetimeMutex, NULL);
	}
	
	pthread_mutex_lock(&datetimeMutex);
#ifdef	D_X86
	time_t			Time;
	struct	tm		tmTime;
	time(&Time);
	tmTime	=*localtime(&Time);
	
	if (date.year>1900)	tmTime.tm_year = date.year - 1900;
	tmTime.tm_mon	= date.month	- 1;
	tmTime.tm_mday	= date.day;

	Time	= mktime(&tmTime);
	
	if(Time == (time_t)(-1))
		nRet = DATE_ERROR_MONTH;
	
	if(stime(&Time) == 0)	nRet = DATE_OK;
	else					nRet = DATE_ERROR_MONTH;
#else
#ifdef	D_ARM
	#ifdef D_3354
	time_t			Time;
	struct	tm		tmTime;
	time(&Time);
	tmTime	=*localtime(&Time);
	
	if (date.year>1900)	tmTime.tm_year = date.year - 1900;
	tmTime.tm_mon	= date.month	- 1;
	tmTime.tm_mday	= date.day;

	Time	= mktime(&tmTime);
	
	if(Time == (time_t)(-1))
		nRet = DATE_ERROR_MONTH;
	
	if(stime(&Time) == 0)	nRet = DATE_OK;
	else					nRet = DATE_ERROR_MONTH;
	#else
		SetRTCDate(date);
		nRet = DATE_OK;
	#endif
#endif
#endif
	pthread_mutex_unlock(&datetimeMutex);
	return nRet;
}

WORD        SetDate(char* pszDate, char cSeparator)
{
	int         nMonth;
	int         nDay;
	int         nYear;
	tmDATE        date;
	
	if (cSeparator != 0)
	    sscanf(pszDate, "%u%*c%u%*c%u", &nMonth, &nDay, &nYear);
	else
	    sscanf(pszDate, "%2u%2u%2u", &nMonth, &nDay, &nYear);
	
	date.year  = nYear;
	date.month = nMonth;
	date.day   = nDay;
	
	return SetDate(date);
}

int		GetKernelVers1()
{
	return MMI_KERNEL_VERSION1;
}

int		GetKernelVers2()
{
	return MMI_KERNEL_VERSION2;
}

int		GetKernelVers3()
{
	return MMI_KERNEL_VERSION3;
}

WORD        CvtDate(char* pszDate, char cSeparator, tmDATE* pdate)
{
	WORD        nMonth = pdate->month;
	WORD        nDay   = pdate->day;
	WORD        nYear  = pdate->year;
	
	if (((nYear % 4) == 0) && ((nYear % 100) != 0) || ((nYear % 400) == 0)) g_nDayOfMonth[FEB] = 29;
	else                                                                    g_nDayOfMonth[FEB] = 28;
	if ((nMonth < 1) || (nMonth > 12)) return DATE_ERROR_MONTH;
	if ((nDay < 1) || (nDay > g_nDayOfMonth[nMonth - 1])) return DATE_ERROR_DAY;
	nYear %= 100;
	
	if (cSeparator != 0)
	    sprintf(pszDate, "%02u%c%02u%c%02u", nMonth, cSeparator, nDay, cSeparator, nYear);
	else
	    sprintf(pszDate, "%02u%02u%02u", nMonth, nDay, nYear);
	
	return DATE_OK;
}

WORD        CvtDate(tmDATE* pdate)
{
	pdate->year %= 100;
	if (pdate->year > 90) pdate->year += C20_START;
	else                  pdate->year += C20_START + 100;
	if (((pdate->year % 4) == 0) && ((pdate->year % 100) != 0) || ((pdate->year % 400) == 0)) g_nDayOfMonth[FEB] = 29;
	else                                                                                      g_nDayOfMonth[FEB] = 28;
	if ((pdate->month < 1) || (pdate->month > 12)) return DATE_ERROR_MONTH;
	if ((pdate->day < 1) || (pdate->day > g_nDayOfMonth[pdate->month - 1])) return DATE_ERROR_DAY;
	
	return DATE_OK;
}

/*---------------------------------------------------------------------------+
|  Function : void GetTime(TIME* ptime)                                      |
|  Task     : Get system time                                                |
+----------------------------------------------------------------------------+
|  Call     : GetTime(ptime)                                                 |
|                                                                            |
|  Parameter: ptime                     - Pointer to TIME structure          |
|                                                                            |
|  Return   : *ptime                    - Current system time                |
+---------------------------------------------------------------------------*/
void        GetTime(tmTIME* ptime)
{
	if (!bDateTimeMutexInit)
	{
		bDateTimeMutexInit = TRUE;
		pthread_mutex_init(&datetimeMutex, NULL);
	}
	
	pthread_mutex_lock(&datetimeMutex);
#ifdef	D_X86
	time_t			Time;
	struct	tm		tmTime;
	
	time(&Time);
	tmTime	=*localtime(&Time);
	
	ptime->hour		= tmTime.tm_hour;
	ptime->minute	= tmTime.tm_min;
	ptime->second	= tmTime.tm_sec;
	ptime->hsecond	= 0;
#else
#ifdef	D_ARM
	#ifdef	D_3354
	time_t			Time;
	struct	tm		tmTime;
	
	time(&Time);
	tmTime	=*localtime(&Time);
	
	ptime->hour		= tmTime.tm_hour;
	ptime->minute	= tmTime.tm_min;
	ptime->second	= tmTime.tm_sec;
	ptime->hsecond	= 0;
	#else
	GetRTCTime(ptime);
	#endif
#endif
#endif
	pthread_mutex_unlock(&datetimeMutex);
}

void        GetSiemensTime(DWORD dwTime, tmTIME* ptime)												//<<yang 2006/3/8 
{
	ptime->hour		= dwTime/(60*60*1000);
	ptime->minute	= (dwTime-ptime->hour*60*60*1000)/(60*1000);
	ptime->second	= (dwTime-ptime->hour*60*60*1000-ptime->minute*60*1000)/1000;
	ptime->hsecond	= dwTime-ptime->hour*60*60*1000-ptime->minute*60*1000-ptime->second*1000;
}
/*---------------------------------------------------------------------------+
|  Function : void GetTime(char* pszTime, char cSeparator)                   |
|  Task     : Get system time                                                |
+----------------------------------------------------------------------------+
|  Call     : GetTime(pszTime, cSeparator)                                   |
|                                                                            |
|  Parameter: pszTime                   - Pointer to time string             |
|             cSeparator                - Separator among the H,M,S          |
|                                                                            |
|  Return   : *pszTime                  - Current system time                |
|                                         Format : HH:MM:SS                  |
+---------------------------------------------------------------------------*/
void 	GetTime(char* pszTime, char cSeparator)
{
	tmTIME        time;
	
	GetTime(&time);
	if (cSeparator != 0)
	    sprintf(pszTime, "%02u%c%02u%c%02u", time.hour, cSeparator, time.minute, cSeparator, time.second);
	else
	    sprintf(pszTime, "%02u%02u%02u", time.hour, time.minute, time.second);
}

/*---------------------------------------------------------------------------+
|  Function : WORD SetTime(char* pszTime, char cSeparator)                   |
|  Task     : Set system time                                                |
+----------------------------------------------------------------------------+
|  Call     : wStatus = SetTime(pszTime, cSeparator)                         |
|                                                                            |
|  Parameter: pszTime                   - Pointer to time string             |
|                                         Format : HH:MM:SS                  |
|             cSeparator                - Separator among the H,M,S          |
|                                                                            |
|  Return   : wStatus                   - Operation status                   |
|                                         TIME_OK          : OK              |
|                                         TIME_ERROR_HOUR  : Hour is wrong   |
|                                         TIME_ERROR_MINUTE: Minute is wrong |
|                                         TIME_ERROR_SECOND: Second is wrong |
+---------------------------------------------------------------------------*/
WORD        SetTime(tmTIME atime)
{
	int nRet = TIME_ERROR_HOUR;
	if (!bDateTimeMutexInit)
	{
		bDateTimeMutexInit = TRUE;
		pthread_mutex_init(&datetimeMutex, NULL);
	}
	
	pthread_mutex_lock(&datetimeMutex);
#ifdef	D_X86
	time_t			Time;
	struct	tm		tmTime;
	
	if (atime.hour   > 23) return TIME_ERROR_HOUR;
	if (atime.minute > 59) return TIME_ERROR_MINUTE;
	if (atime.second > 59) return TIME_ERROR_SECOND;
	
	time(&Time);
	tmTime	=*localtime(&Time);
	
	tmTime.tm_hour	= atime.hour;
	tmTime.tm_min	= atime.minute;
	tmTime.tm_sec	= atime.second; 
	
	Time	= mktime(&tmTime);
	if(Time == (time_t)(-1))
		nRet = TIME_ERROR_HOUR;
	
	if(stime(&Time) == 0)	nRet = DATE_OK;
	else					nRet = TIME_ERROR_HOUR;

	nRet = TIME_OK;
#else
#ifdef	D_ARM
	#ifdef D_3354
		time_t			Time;
		struct	tm		tmTime;
	
		if (atime.hour   > 23) return TIME_ERROR_HOUR;
		if (atime.minute > 59) return TIME_ERROR_MINUTE;
		if (atime.second > 59) return TIME_ERROR_SECOND;
	
		time(&Time);
		tmTime	=*localtime(&Time);
	
		tmTime.tm_hour	= atime.hour;
		tmTime.tm_min	= atime.minute;
		tmTime.tm_sec	= atime.second; 
	
		Time	= mktime(&tmTime);
		if(Time == (time_t)(-1))
			nRet = TIME_ERROR_HOUR;
	
		if(stime(&Time) == 0)	nRet = DATE_OK;
		else					nRet = TIME_ERROR_HOUR;

		nRet = TIME_OK;
	#else
		SetRTCTime(atime);
		nRet = DATE_OK;
	#endif
#endif
#endif
	pthread_mutex_unlock(&datetimeMutex);
	return nRet;
}

WORD        SetTime(char* pszTime, char cSeparator)
{
	int         nHour;
	int         nMinute;
	int         nSecond;
	tmTIME        time;
	
	if (cSeparator != 0)
	    sscanf(pszTime, "%u%*c%u%*c%u", &nHour, &nMinute, &nSecond);
	else
	    sscanf(pszTime, "%2u%2u%2u", &nHour, &nMinute, &nSecond);
	
	time.hour   = nHour;
	time.minute = nMinute;
	time.second = nSecond;
	
	return SetTime(time);
}

WORD        CvtTime(char* pszTime, char cSeparator, tmTIME* ptime)
{
	WORD        nHour   = ptime->hour;
	WORD        nMinute = ptime->minute;
	WORD        nSecond = ptime->second;
	
	if (nHour   > 23) return TIME_ERROR_HOUR;
	if (nMinute > 59) return TIME_ERROR_MINUTE;
	if (nSecond > 59) return TIME_ERROR_SECOND;
	
	if (cSeparator != 0)
	    sprintf(pszTime, "%02u%c%02u%c%02u", nHour, cSeparator, nMinute, cSeparator, nSecond);
	else
	    sprintf(pszTime, "%02u%02u%02u", nHour, nMinute, nSecond);
	
	return TIME_OK;
}

/*---------------------------------------------------------------------------+
|  Function : StrNCpy(char* psz1, char* psz2, int nCount)                    |
|  Task     : Copy nCount of byte from psz2 to psz1 with tailing '\0'        |
+----------------------------------------------------------------------------+
|  Call     : StrNCpy(psz1, psz2, nCount)                                    |
|                                                                            |
|  Parameter: psz1                      - Pointer to destination string      |
|             psz2                      - Pointer to source string           |
|             nCount                    - Number of byte to be copied        |
|                                                                            |
|  Return   : *psz1                     -                                    |
+---------------------------------------------------------------------------*/
void        StrNCpy(char* psz1, char* psz2, int nCount)
{
	strncpy(psz1, psz2, nCount);
	*(psz1 + nCount) = '\0';
}

void        StrNSet(char* psz, char ch, int nCount)
{
	for (int i=0; i<nCount; i++) *psz++ = ch;
	*psz = '\0';
}


int			StrLenTM(char* psz)
{
	int nResult = 0;
	if (psz != NULL)
	{
		unsigned short *pwCode;
		pwCode  = (unsigned short *)psz;
		while(pwCode[nResult ++] != 0);
		
		nResult *= 2;
		
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : StrSubst(char* psz1, char* psz2, int n)                        |
|  Task     : Substitute the %n in string1 by string2                        |
+----------------------------------------------------------------------------+
|  Call     : StrSubst(psz1, psz2, n)                                        |
|                                                                            |
|  Parameter: psz1                      - Pointer to source string 1         |
|             psz2                      - Pointer to source string 2         |
|             n                         - %n                                 |
|                                                                            |
|  Return   : *psz1                     - Substituted string                 |
+---------------------------------------------------------------------------*/
void        StrSubst(char* psz1, char* psz2, int n)
{
	char        pszMark[8];
	char        pszBuffer[256];
	char*       pszIndex;
	
	sprintf(pszMark, "%%%u", n);
	if ((pszIndex = strstr(psz1, pszMark)) != NULL)
	{
	    strcpy(pszBuffer, pszIndex);
	    strcpy(pszIndex, psz2);
	    strcat(pszIndex, pszBuffer+strlen(pszMark));
	}
}

/*---------------------------------------------------------------------------+
|  Function : F2A(char* psz, WORD wPrecision, long l)                        |
|             A2F(char* psz, WORD wPrecision)                                |
|  Task     : Use a long number to be a float number                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        F2A(char* psz, short nPrecision, long long ll)
{
short       nLength;
short       nDot;                       // Dot position or the Length of '0' to be filled between '.' and the Most Significant Digit
char*       pszPointer;

sprintf(psz, "%lld", ll);

if (nPrecision > 0)
    {
    nLength = strlen(psz);
    if (ll < 0)                          // Skip the minus sign
        {
        psz++;
        nLength--;
        }

    nDot = nPrecision - nLength;
    if (nDot < 0)
        {
        pszPointer = psz - nDot;
        memmove(pszPointer+1, pszPointer, nPrecision);
        *pszPointer = '.';
        *(pszPointer+nPrecision+1) = '\0';
        }
    else
        {
        nDot += 2/*"0."*/;
        memmove(psz+nDot, psz, nLength);
        memset(psz, '0', nDot);
        *(psz+1) = '.';
        *(psz+nLength+nDot) = '\0';
        }
    }
}

void        LL2A(char* psz, short nPrecision, long long ll, int nCodeType)					//<<yang 2006/3/3 
{
short       nLength;
short       nDot;                       // Dot position or the Length of '0' to be filled between '.' and the Most Significant Digit
char*       pszPointer;
int         i   = 0;
WORD        szTemp[56];

sprintf(psz, "%lld", ll);

if(nCodeType == tmFT_CODE_TECH)    //netdigger 2007/4/12
{
    memset(szTemp, 0, sizeof(szTemp));
    pszPointer = psz;
    while(*pszPointer != 0) szTemp[i ++]    = *pszPointer ++;
    memcpy(psz, szTemp, i * 2);
    psz[i * 2]      =
    psz[i * 2 + 1]  = 0;
    nLength = i;
    
    if (nPrecision > 0)
    {
        if (ll < 0)                          // Skip the minus sign
        {
            psz += 2;
            nLength   --;
        }
    
        nDot = nPrecision - nLength;
        if (nDot < 0)
        {
            pszPointer = psz - nDot * 2;
            memmove(pszPointer+2, pszPointer, nPrecision * 2);
            *pszPointer = '.';
            *(pszPointer + 1) =  0;
            *(pszPointer+nPrecision * 2 + 2) = '\0';
            *(pszPointer+nPrecision * 2 + 3) = '\0';
        }
        else
        {
            nDot += 2/*"0."*/;
            memmove(psz+nDot * 2, psz, nLength * 2);
            for(i = 0; i < nDot * 2; i +=2)
            {
                *(psz + i)  = '0';
                *(psz + i + 1)  = 0;
            }

            *(psz+2) = '.';
            
//            *(psz+nLength+nDot) = '\0';
			*(psz + nLength * 2 + nDot * 2) = 0;
			*(psz + nLength * 2 + nDot * 2 + 1) = 0;
        }
    }
}
else
{
    if (nPrecision > 0)
        {
        nLength = strlen(psz);
        if (ll < 0)                          // Skip the minus sign
            {
            psz++;
            nLength--;
            }
    
        nDot = nPrecision - nLength;
        if (nDot < 0)
            {
            pszPointer = psz - nDot;
            memmove(pszPointer+1, pszPointer, nPrecision);
            *pszPointer = '.';
            *(pszPointer+nPrecision+1) = '\0';
            }
        else
            {
            nDot += 2/*"0."*/;
            memmove(psz+nDot, psz, nLength);
            memset(psz, '0', nDot);
            *(psz+1) = '.';
            *(psz+nLength+nDot) = '\0';
            }
        }
}
}

void        A2A(char* psz, short nPrecision, int nCodeType)			//Sunny<20070829>
{
short       nLength;
short       nDot;                       // Dot position or the Length of '0' to be filled between '.' and the Most Significant Digit
char*       pszPointer;
int         i   = 0;
WORD        szTemp[56];

//sprintf(psz, "%lld", ll);

if(nCodeType == tmFT_CODE_TECH)    //netdigger 2007/4/12
{
    memset(szTemp, 0, sizeof(szTemp));
    pszPointer = psz;
    while(*pszPointer != 0) szTemp[i ++]    = *pszPointer ++;
    memcpy(psz, szTemp, i * 2);
    psz[i * 2]      =
    psz[i * 2 + 1]  = 0;
    nLength = i;
    
    if (nPrecision != 0)
    {
        //if (ll < 0)                          // Skip the minus sign
        //{
        //    psz += 2;
        //    nLength   --;
        //}
    
        nDot = nPrecision - nLength;
        if (nDot < 0)
        {
            pszPointer = psz - nDot * 2;
            memmove(pszPointer+2, pszPointer, nPrecision * 2);
            *pszPointer = '.';
            *(pszPointer + 1) =  0;
            *(pszPointer+nPrecision * 2 + 2) = '\0';
            *(pszPointer+nPrecision * 2 + 3) = '\0';
        }
        else
        {
            nDot += 2/*"0."*/;
            memmove(psz+nDot * 2, psz, nLength * 2);
            for(i = 0; i < nDot * 2; i +=2)
            {
                *(psz + i)  = '0';
                *(psz + i + 1)  = 0;
            }

            *(psz+2) = '.';
            
//            *(psz+nLength+nDot) = '\0';
			*(psz + nLength * 2 + nDot * 2) = 0;
			*(psz + nLength * 2 + nDot * 2 + 1) = 0;
        }
    }
}
else
{
    if (nPrecision != 0)
        {
        nLength = strlen(psz);
        //if (ll < 0)                          // Skip the minus sign
        //    {
        //    psz++;
        //    nLength--;
        //    }
    
        nDot = nPrecision - nLength;
        if (nDot < 0)
            {
            pszPointer = psz - nDot;
            memmove(pszPointer+1, pszPointer, nPrecision);
            *pszPointer = '.';
            *(pszPointer+nPrecision+1) = '\0';
            }
        else
            {
            nDot += 2/*"0."*/;
            memmove(psz+nDot, psz, nLength);
            memset(psz, '0', nDot);
            *(psz+1) = '.';
            *(psz+nLength+nDot) = '\0';
            }
        }
}
}


long long	A2F(char* psz, short nPrecision)
{
	long long	ll;
	short       nDelta;
	char        sz[tmMAX_TEXT_COL+1];
	char*       pszPointer;
	char*       pszTooMuchDot;
	
	if (nPrecision < 0) nPrecision = 0;
	if(psz[0] == '\0')
		strcpy(sz, "0");                        // Keep the original string
	else
		strcpy(sz, psz);                        // Keep the original string
	pszPointer = strchr(sz, '.');
	if (pszPointer != NULL)
	{
	    strcpy(pszPointer, pszPointer+1);
	    if ((pszTooMuchDot = strchr(pszPointer, '.')) != NULL)
	        *pszTooMuchDot = '\0';
	    nDelta = nPrecision - strlen(pszPointer);
	}
	else
	    nDelta = nPrecision;
	    
	if (nDelta < 0)
	    *(pszPointer+nPrecision) = '\0';
	else
	    StrNSet(sz+strlen(sz), '0', nDelta);
	//  for (short i=0; i<nDelta; i++) strcat(sz, "0");
	sscanf(sz, "%lld", &ll);											//<<yang 2006/10/20 
	
	return ll;
}
/*---------------------------------------------------------------------------+
|  Function : HEX2BCD(WORD wValue)                                           |
|             DEC2HEX(WORD wValue)                                           |
|  Task     : DEC and HEX transfer function                                  |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|  Parameter:                                                                |
|  Return   :                                                                |
+----------------------------------------------------------------------------+
|  Creation : 04/11/2000 by Steven for Config Meter Assign                   |
+---------------------------------------------------------------------------*/
WORD        HEX2BCD(WORD wValue)   //St041100 for Config Meter Assign
{
WORD        w; 
w = ((wValue&0xF000) / 0x1000)*1000 + ((wValue&0x0F00) / 0x0100)*100 + ((wValue&0x00F0) / 0x0010)*10 +wValue % 0x0010;
return   w;
}

WORD        DEC2HEX(WORD wValue/*,WORD* cDecimal*/)   //St040700 for Meter Adjust
{
WORD        w; 
w = (wValue/10000) * 0x1000 + ((wValue%10000)/1000) * 0x0100 + ((wValue%1000) / 100)* 0x0010 + (wValue % 100)/10;
/*cDecimal = wValue%10;*/ 
return   w;
}
/*---------------------------------------------------------------------------+
|  Function : Power(int nBase, int n)                                        |
|  Task     : Raise nBase to n                                               |
+----------------------------------------------------------------------------+
|  Call     : l = Power(nBase, n)                                            |
|                                                                            |
|  Parameter: nBase                     -                                    |
|             n                         -                                    |
|                                                                            |
|  Return   : l                         -                                    |
+---------------------------------------------------------------------------*/
long        Power(int nBase, int n)
{
long        l = 1;
while ((--n) >= 0) l *= nBase;
return l;
}

WORD        CountDigit(WORD wValue)
{
WORD        wDigit = 1;
while ((wValue /= 10) != 0) wDigit++;
return wDigit;
}

WORD        CountDigit(long lValue)
{
WORD        wDigit;
DWORD       dwValue;

if (lValue >= 0)
    {
    wDigit = 1;
    dwValue = lValue;
    }
else
    {
    wDigit = 2;
    dwValue = -lValue;
    }
while ((dwValue /= 10) != 0) wDigit++;
return wDigit;
}


WORD        CountDigit(long long llValue)					//<<yang 2006/3/3 
{
WORD				wDigit;
//unsigned int int	dwValue;

if (llValue >= 0)
    {
    wDigit = 1;
    llValue = llValue;
    }
else
    {
    wDigit = 2;
    llValue = -llValue;
    }
while ((llValue /= 10) != 0) wDigit++;
return wDigit;
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
void        Msg(short nMsg)
{
	//g_pwndPrompt->Show(0, g_pStr[nMsg]);
}

void        Msg(char* pszMsg)
{
//g_pwndPrompt->Show(0, pszMsg);	fans mark
	
	if (g_pMainFrame->GetPromptBar() != NULL)
		g_pMainFrame->GetPromptBar()->Show(0, pszMsg);	//	fans add
}

void        MsgBeep(short nMsg)
{
//::_Beep();
//g_pwndPrompt->Show(0, g_pStr[nMsg]);
}

void        MsgBeep(char* pszMsg)
{
//::_Beep();
	g_pMainFrame->GetPromptBar()->Show(0, pszMsg);	
}

void        MsgExt(short nMsg)
{
	//g_pwndStatus->Show(1, g_pStr[nMsg]);	netdigger 20050201
}

void        MsgExt(char* pszMsg)
{
	//g_pwndStatus->Show(1, pszMsg);	netdigger 20050201
}

void        MsgBeepExt(short nMsg)
{
//::_Beep();
	//g_pwndStatus->Show(1, g_pStr[nMsg]);	netdigger 20050201
}

void        MsgBeepExt(char* pszMsg)
{
//::_Beep();
	//g_pwndStatus->Show(1, pszMsg);	netdigger 20050201
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
void        Prompt(short nMsg)
{
	Prompt(g_MultiLanguage[nMsg]);
}

void PromptETech(char* pszMsg)
{
	
    WORD    szNew[256];
    WORD    sz[]   = {' ', 'E', 'R', ':', 0};
    WORD    *pszSrc, *pszDst;
    CtmWnd  *pwndStaticPrompt = NULL;
    
    if(pszMsg == NULL)  return;
        
    memset(szNew, 0, sizeof(szNew)); 
    
    if (g_pMainFrame->GetPromptBar() != NULL)
		pwndStaticPrompt = g_pMainFrame->GetPromptBar()->FindControlFromName("StaticPrompt");
	if(pwndStaticPrompt != NULL)
		pwndStaticPrompt->GetPropValueT("text", szNew, sizeof(szNew));
	else    return;
       
    pszSrc  = (WORD *)pszMsg;
    pszDst  = szNew;
    int i = 0;
    while(*pszDst != 0)
    { 
        if(pszDst[0] == sz[0] && 
        	pszDst[1] == sz[1] && 
        	pszDst[2] == sz[2] && 
        	pszDst[3] == sz[3])
            break;
        pszDst++;
        i++;
    }
    
    if(*pszDst == 0)    memcpy(pszDst, sz, sizeof(sz));    
    pszDst  += 4;
    i += 4;
    
    while(*pszSrc != 0) 
    {
    	
    	if (i * 2 >= 198)
    		break;
    	*pszDst ++ = *pszSrc ++;
    	i++;
    	}
    //printf("PromptTech =%d\n", i);
    *pszDst = 0;
    g_pMainFrame->GetPromptBar()->Show(0, (char *)szNew);
 
}


void        Prompt(char* pszMsg,int AddMsgFlag, int MsgShowType)
{
	if (g_pMainFrame->GetPromptBar() != NULL)
	{
		if (pszMsg != NULL)
		{
			if(AddMsgFlag == 0)
			{
			    if(0 == MsgShowType)
					SendPrompMsg(pszMsg, AddMsgFlag);
				else
				    g_pMainFrame->GetPromptBar()->Show(0, pszMsg);	//	fans add
			}
			if(AddMsgFlag == 1)				//yangpeng 2007/4/1	11:40	提示顯示在已有的提示字串後
			{
				if(g_pMainFrame->GetPromptBar()->GetFont()->GetFontCode() != tmFT_CODE_TECH)	
				    PromptE(pszMsg, MsgShowType);
				else     
				    PromptETech(pszMsg);
		    }
		    if (AddMsgFlag == 2)	//Sunny<20070706>多語言方式直接show一些必要的提示
		    {
				if(g_pMainFrame->GetPromptBar()->GetFont()->GetFontCode() == tmFT_CODE_TECH)
				{
    				char    szNew[256];
    				memset(szNew,0,sizeof(szNew));
    				CodeChange(szNew,pszMsg);
    				if(0 == MsgShowType)
				        SendPrompMsg(szNew, AddMsgFlag);
			        else
					    g_pMainFrame->GetPromptBar()->Show(0, szNew);
				}
				else	
				{
				    if(0 == MsgShowType)
						SendPrompMsg(pszMsg, AddMsgFlag);
					else
					    g_pMainFrame->GetPromptBar()->Show(0, pszMsg);	//	fans add
				}
		    	
		    }
		    
		}
		else 
		{
			WORD    wBlank  = 0;
			if(0 == MsgShowType)
				SendPrompMsg("", 0);
			else
			    g_pMainFrame->GetPromptBar()->Show(0, (char *)&wBlank);
		}
		
	}
}
//****************************
//提示顯示在已有的提示字串後
//****************************
void        PromptE(char* pszMsg, int MsgShowType)
{
	CtmWnd *pwndStaticPrompt = NULL;
	char 	szNew[256];
	char	sztemp[] = " ER:";
	char	*psz;
	memset(szNew,0,sizeof(szNew));
	if (pszMsg != NULL)
	{
		
		if (g_pMainFrame->GetPromptBar() != NULL)
			pwndStaticPrompt = g_pMainFrame->GetPromptBar()->FindControlFromName("StaticPrompt");
		if(pwndStaticPrompt != NULL)
			pwndStaticPrompt->GetPropValueT("text",szNew,sizeof(szNew));
		if(szNew != NULL && szNew[0] != '\0' && strstr(szNew,pszMsg) == 0)
		{
			psz = strstr(szNew,sztemp);
			if(psz == NULL)
				sprintf(szNew, "%s ER:%s", szNew, pszMsg);		//need check slop over???
			else
				strcpy(psz + 4,pszMsg);
			if(0 == MsgShowType)
				SendPrompMsg(szNew, 0);
			else
			    g_pMainFrame->GetPromptBar()->Show(0, szNew);
		}
		else if(szNew == NULL || szNew[0] == '\0')
		{
			sprintf(szNew," ER:%s",pszMsg);
			if(0 == MsgShowType)
				SendPrompMsg(szNew, 0);
			else
			    g_pMainFrame->GetPromptBar()->Show(0, szNew);
		}
	}
}

void        PromptBeep(short nMsg)
{
	Beep();
	Prompt(nMsg);
}

void        PromptBeep(char* pszMsg)
{
	Beep();
	Prompt(pszMsg);
}

void        PromptStat(char* pszMsg)    //BW<WY2003/01/21
{
	//g_pwndStatus->Show(3, pszMsg);	netdigger 20050201
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
BOOL        MsgBox(short nMsg)
{
	return MsgBox(g_MultiLanguage[nMsg], 3);
}

void    AscToTech(char *pDst, char *pSrc)
{
    WORD    *pwDst  = (WORD *)pDst;
    
    while(*pSrc != 0)     *pwDst ++ = *pSrc ++;
    
    *pwDst  = 0;
    
}

BOOL        MsgBox(char* pszMsg, int nType)
{
	
	//	fans add
	CtmFormView* pMsgbox;			//	這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject pParseMsgBox;
	
	char	szCmd[256];
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgbox.txt");
	pParseMsgBox.OpenFile(szCmd);
	
	//pParseMsgBox.OpenFile("res/form/msgbox.TXT");	// <FANS> 2006/11/21 10:53上午
	pParseMsgBox.CloseFile();
	pParseMsgBox.m_aryControls[0].control->CreateA();
//	pParseMsgBox.m_aryControls[0].control->Show();
	pMsgbox = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	for (int i = 0; i < pMsgbox->GetControlCount(); i++)
	{
		if (pMsgbox->Controls[i]->Is("CtmStaticX2"))
		{
			if (pszMsg != NULL)
			{
				if(pMsgbox->Controls[i]->GetFont()->GetFontCode() == tmFT_CODE_TECH && nType != tmFT_CODE_TECH)
				{
				    WORD    *pwDst;
				    int nLen    = strlen(pszMsg);
				    char    *pszTMMsg   = (char *)malloc((nLen + 1) * 2);
				    pwDst   = (WORD *)pszTMMsg;
				    
				    while(*pszMsg != 0)
				    {
				        *pwDst  = *pszMsg;
				        pwDst ++;
				        pszMsg ++;
				    }
				    
				    *pwDst   = 0;
				    pMsgbox->Controls[i]->SetPropValueT("text", pszTMMsg);
				}
				else
				{	
				    pMsgbox->Controls[i]->SetPropValueT("text", pszMsg);
				}
				//pMsgbox->Controls[i]->Update();
			}
			//StaticText
		}
		//if msgbox.Controls[i]
	}
	g_bFocusChanged|=0x10;
	bool bResult=pMsgbox->DoModal();
	g_bFocusChanged&=0x0F;
	return bResult;

}
BOOL	MsgBox(char* pszMsg, char* pszMsg2,int nType)
{
	
	//	fans add
	CtmFormView* pMsgbox;			//	這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject pParseMsgBox;
	
	char	szCmd[256];
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgbox.txt");
	pParseMsgBox.OpenFile(szCmd);
	
	//pParseMsgBox.OpenFile("res/form/msgbox.TXT");	// <FANS> 2006/11/21 10:53上午
	pParseMsgBox.CloseFile();
	pParseMsgBox.m_aryControls[0].control->CreateA();
//	pParseMsgBox.m_aryControls[0].control->Show();
	pMsgbox = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	
	int nStaticIndex1 =0;
	for (int i = 0; i < pMsgbox->GetControlCount(); i++)
	{
		if (pMsgbox->Controls[i]->Is("CtmStaticX2"))
		{
			if (pszMsg != NULL)
			{
				if(pMsgbox->Controls[i]->GetFont()->GetFontCode() == tmFT_CODE_TECH && nType != tmFT_CODE_TECH)
				{
				    WORD    *pwDst;
				    int nLen    = strlen(pszMsg);
				    char    *pszTMMsg   = (char *)malloc((nLen + 1) * 2);
				    pwDst   = (WORD *)pszTMMsg;
				    
				    while(*pszMsg != 0)
				    {
				        *pwDst  = *pszMsg;
				        pwDst ++;
				        pszMsg ++;
				    }
				    
				    *pwDst   = 0;
				    pMsgbox->Controls[i]->SetPropValueT("text", pszTMMsg);
				}
				else
				{	
				    pMsgbox->Controls[i]->SetPropValueT("text", pszMsg);
				}
				//pMsgbox->Controls[i]->Update();
			}
			nStaticIndex1 = i;
			break;
			//StaticText
		}
		//if msgbox.Controls[i]
	}
	
	for (int i = nStaticIndex1+1; i < pMsgbox->GetControlCount(); i++)
	{
		if (pMsgbox->Controls[i]->Is("CtmStaticX2"))
		{
			if (pszMsg2 != NULL)
			{
				if(pMsgbox->Controls[i]->GetFont()->GetFontCode() == tmFT_CODE_TECH && nType != tmFT_CODE_TECH)
				{
				    WORD    *pwDst;
				    int nLen    = strlen(pszMsg2);
				    char    *pszTMMsg   = (char *)malloc((nLen + 1) * 2);
				    pwDst   = (WORD *)pszTMMsg;
				    
				    while(*pszMsg2 != 0)
				    {
				        *pwDst  = *pszMsg2;
				        pwDst ++;
				        pszMsg2 ++;
				    }
				    
				    *pwDst   = 0;
				    pMsgbox->Controls[i]->SetPropValueT("text", pszTMMsg);
				}
				else
				{	
				    pMsgbox->Controls[i]->SetPropValueT("text", pszMsg2);
				}
				//pMsgbox->Controls[i]->Update();
			}
			break;
			//StaticText
		}
		//if msgbox.Controls[i]
	}
	
	return pMsgbox->DoModal();

}


#include <guilite/IGui>
#include "colortable.h"
BOOL        Login(char*	pszFormName)
{
	//	fans add
	if(CtmConfig::GetInstance()->GetKeyLockFlag()&0x04 == 4)
	{
	        if(!(g_dwM3Key[1]&0x2000000))
	        {
	        	return TRUE;
	        }
	}
	
	#ifndef      D_DEMO
	if (!bLogin)
	{
		CtmFormView* pPassWordForm;			//	這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
		CParseObject pParseMsgBox;	
		
		char	szCmd[256];
		bLogin = TRUE;
		//sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "password.txt");
		if(pszFormName != NULL)
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), pszFormName);
		else
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "password.txt");

		pParseMsgBox.OpenFile(szCmd);
		
		pParseMsgBox.CloseFile();
		pPassWordForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
		//pPassWordForm->SetParent(g_pMainFrame->GetCurrentView());
		pPassWordForm->SetParent(NULL);
	//	CImage bgImage=CImage(CSize(1024,768),CImage::Format_RGB32);
		//_SetViewPort(0,0,1024,768);
		//_GetImage(0,0,bgImage);
		//CImage testImage=bgImage.copy(CRect(CPoint(0,0),bgImage.size()));
		//CImage colorImage=CImage(CSize(1024,768),CImage::Format_RGB32);
		//colorImage.fill(Gemini::gray_level0&0x7FFFFFFF);
		//_CombineImage(0,0,testImage,colorImage);
		//_PutImage(0,0,testImage);
		pPassWordForm->CreateA();
		pPassWordForm->DoModal();
		//_PutImage(0,0,bgImage);
		bLogin = FALSE;
	}
	#endif
	return TRUE;
}



//New add by Roy↓
BOOL		FileProcess()		//MsgBox for file，Rewrite From Login()
{
	//ROY：此函式目前僅供檔案頁使用，目的在於產生擁有光標並能做處理的小視窗
	#ifndef		FILEPROCESS_MSGBOX
	
	CtmFormView*	pFileProcessForm;
	CParseObject	pParseMsgBox;
	
	char	szCmd[256];
	//ROY：預計加入個g_nFileProcessType來判斷要進行何種處理，以及顯示對應的小視窗
	switch(g_nFileProcessType)
	{
		case	0:		//CopyFile
			//break;
		case	1:		//RenameFile
			//break;
		case	2:		//CreateFile
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxFileControl.txt");
			break;
		default:
			//sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxCreateFile.txt");
			break;
	}
	
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();
	
	pFileProcessForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	pFileProcessForm->SetParent(NULL);
	pFileProcessForm->CreateA();
	pFileProcessForm->DoModal();
	
	#endif
	return TRUE;
}
//20140617↑

BOOL		MsgBoxConfirm(char* pszMsg, int nType)		//Rewrite From MsgBox(char* pszMsg, int nType)
{
	CtmFormView*	pMsgbox;			//這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject	pParseMsgBox;
	
	char					szCmd[256];		//ROY：msgbox的畫面腳本存放位置
	
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxConfirm.txt");
	
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();	
	pParseMsgBox.m_aryControls[0].control->CreateA();

	pMsgbox = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	
	for (int i = 0; i < pMsgbox->GetControlCount(); i++)
	{
		if (pMsgbox->Controls[i]->Is("CtmStaticX2"))
		{
			if (pszMsg != NULL)
			{
				if(pMsgbox->Controls[i]->GetFont()->GetFontCode() == tmFT_CODE_TECH && nType != tmFT_CODE_TECH)
				{
				    WORD    *pwDst;
				    int nLen    = strlen(pszMsg);
				    char    *pszTMMsg   = (char *)malloc((nLen + 1) * 2);
				    pwDst   = (WORD *)pszTMMsg;
				    
				    while(*pszMsg != 0)
				    {
				        *pwDst  = *pszMsg;
				        pwDst ++;
				        pszMsg ++;
				    }
				    
				    *pwDst   = 0;
				    pMsgbox->Controls[i]->SetPropValueT("text", pszTMMsg);
				}
				else
				{	
				    pMsgbox->Controls[i]->SetPropValueT("text", pszMsg);
				}
				//pMsgbox->Controls[i]->Update();
			}
			break;
		}
	}
	
	return pMsgbox->DoModal();
}
//20140624↑

BOOL		MsgBoxSpecial(int nType)
{
	
	CtmFormView*	pMsgBoxSpecialForm;
	CParseObject	pParseMsgBox;
	
	char	szCmd[256];

	switch(nType)
	{
		case	0:		//檔案頁
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxFileControl.txt");
			break;
		case	1:		//編輯頁->教點檢視
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxTeachPoint.txt");
			break;
		case	2:		//編輯頁->指令說明
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxCommandExplanation.txt");
			break;
		case	3:		//編輯頁->對話編輯
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxAddCommand.txt");
			break;/*
		case	4:		//編輯頁->對話編輯->移動指令
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxMoveCommand.txt");
			printf("szCmd is %s\n",szCmd);
			break;*/
		case	5:		//系統->範圍設定->修改時輸入密碼
			printf("PassWord.txt\n");
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "PassWord.txt");
			break;
		default:
			return TRUE;
			break;
	}
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();

	pMsgBoxSpecialForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	pMsgBoxSpecialForm->SetParent(NULL);
	pMsgBoxSpecialForm->CreateA();
	pMsgBoxSpecialForm->DoModal();
	
	return TRUE;
}
//20140715↑


BOOL		MsgBoxCall(char* MsgBoxID)		//20141013 add by Mario  MsgB呼叫函式
{
	CtmFormView*	pMsgBoxSpecialForm;
	CParseObject	pParseMsgBox;
	
	char	szCmd[256];

	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), MsgBoxID);
	//printf("szCmd = %s\n",szCmd);
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();

	pMsgBoxSpecialForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	pMsgBoxSpecialForm->SetParent(NULL);
	pMsgBoxSpecialForm->CreateA();
	pMsgBoxSpecialForm->DoModal();
	return TRUE;
}

BOOL		MsgBoxCall(char* MsgBoxID,int char_num , ...)		//20141013 add by Mario  MsgB呼叫函式
{
	CtmFormView*	pMsgbox;			//這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject	pParseMsgBox;
	char					szCmd[256];		//ROY：msgbox的畫面腳本存放位置
	va_list   ap;   
	va_start(ap,   char_num);
	
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), MsgBoxID);
	
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();	
	pParseMsgBox.m_aryControls[0].control->CreateA();

	pMsgbox = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	
	
	for (int i = 0,j=0; i < pMsgbox->GetControlCount() && j <char_num; i++)
	{
		if (pMsgbox->Controls[i]->Is("CtmStaticX2")/* && strcmp(pMsgbox->Controls[i]->Name, "StaticMessage") == 0*/)
		{
			char* pszMsg=va_arg(ap,char*);
			j++;
			if (pszMsg != NULL)
				//printf("pszMsg = %s\n",pszMsg);
				pMsgbox->Controls[i]->SetPropValueT("textID", pszMsg);
			else
			{ 
				//printf("break\n");
				break;
			}
		}
	}
	
	va_end(ap);
	return pMsgbox->DoModal();
}

BOOL		MsgBoxCall(char* MsgBoxID ,char* pszMsg)		//2015/4/17 可選擇msgbox檔案以及設定字串
{
	CtmFormView*	pMsgbox;			//這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject	pParseMsgBox;
	
	char					szCmd[256];		//ROY：msgbox的畫面腳本存放位置
	
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), MsgBoxID);
	
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();	
	pParseMsgBox.m_aryControls[0].control->CreateA();

	pMsgbox = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	
	for (int i = 0; i < pMsgbox->GetControlCount(); i++)
	{
		if (pMsgbox->Controls[i]->Is("CtmStaticX2") && strcmp(pMsgbox->Controls[i]->Name, "StaticMessage") == 0)
		{
			if (pszMsg != NULL)
			{
				pMsgbox->Controls[i]->SetPropValueT("textID", pszMsg);
			}
			break;
		}
	}
	
	return pMsgbox->DoModal();
}

BOOL		WorkCoordSeatCall(int f_NO)	//呼叫設定工做座標的視窗
{
		//	fans add
	CtmFormView* pMsgbox;			//	這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject pParseMsgBox;
	
	char	szCmd[256];
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "msgboxThreePointToWorkC.txt");
	pParseMsgBox.OpenFile(szCmd);
	pParseMsgBox.CloseFile();
	pParseMsgBox.m_aryControls[0].control->CreateA();
	pMsgbox = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	for (int i = 0; i < pMsgbox->GetControlCount(); i++)
	{
		if (strcmp(pMsgbox->Controls[i]->Name ,"EditSetWorkCoordNo") == 0)
		{
			pMsgbox->Controls[i]->SetPropValueT("value", f_NO);
			break;
		}
	}
	
	return pMsgbox->DoModal();
	
}




void	OnlineHelpXml(char* pszFormName,int tab,char* pszName)
{
	static int onlinehelp=0;
	
	if(!onlinehelp)
	{	
		onlinehelp++;	
		char	szCmd[256];
		CtmFormView* pHelpForm;
		CParseObject pParseHelp;
		char	szHelpScript[256];
		sprintf(szHelpScript,"%s%s\n",CtmConfig::GetInstance()->GetLanguagepathHelp(),pszFormName);
		if(strstr(szHelpScript,"chn"))
		{
			strcpy(szHelpScript+strlen(szHelpScript)-4,"chn.xml");
		}
		else if(strstr(szHelpScript,"chs"))
		{
			strcpy(szHelpScript+strlen(szHelpScript)-4,"chs.xml");
		}
		else if(strstr(szHelpScript,"eng"))
		{
			strcpy(szHelpScript+strlen(szHelpScript)-4,"eng.xml");	
		}
		else
		strcpy(szHelpScript+strlen(szHelpScript)-4,"xml");
		
		printf("szHelpScript=%s\n",szHelpScript);
		if(access(szHelpScript,F_OK))
		{
			onlinehelp=0;
			return;	
		}
		sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "OnlineHelp.txt");
		if(access(szCmd,F_OK))
		{
			onlinehelp=0;
			return;	
		}
		pParseHelp.OpenFile(szCmd);
	
		pParseHelp.CloseFile();
		pHelpForm = (CtmFormView*)pParseHelp.m_aryControls[0].control;
		//printf("help Form name:%s\n", pHelpForm->Name);
		pHelpForm->SetParent(NULL);
		pHelpForm->CreateA();
		
		//char	szHelpScript[256];
		//sprintf(szHelpScript,"%s%s\n",CtmConfig::GetInstance()->GetLanguagepathHelp(),pszFormName);
//		if(strstr(szHelpScript,"chn"))
//		{
//			strcpy(szHelpScript+strlen(szHelpScript)-4,"chn.xml");
//		}
//		else if(strstr(szHelpScript,"chs"))
//		{
//			strcpy(szHelpScript+strlen(szHelpScript)-4,"chs.xml");
//		}
//		else if(strstr(szHelpScript,"eng"))
//		{
//			strcpy(szHelpScript+strlen(szHelpScript)-4,"eng.xml");	
//		}
//		else
//		strcpy(szHelpScript+strlen(szHelpScript)-4,"xml");
		//printf("szHelpScript=%s\n",szHelpScript);
		CtmOnlineHelp xmlhelp(szHelpScript);
			
		for (int i = 0; i < pHelpForm->GetControlCount(); i++)
		{
			if(pHelpForm->Controls[i]->Is("CtmTextView"))
			{
				char* psz=0;
				((CtmTextView*)(pHelpForm->Controls[i]))->SetTitle(xmlhelp.GetTitle());
				((CtmTextView*)(pHelpForm->Controls[i]))->SetFooter(xmlhelp.GetFooter());
				((CtmTextView*)(pHelpForm->Controls[i]))->SetContext(xmlhelp.GetContext(-1)); 	
				((CtmTextView*)(pHelpForm->Controls[i]))->SetText(xmlhelp.GetDetailByID(psz)/*xmlhelp.GetDetailsByID(tab)*/); 
				if(xmlhelp.GetDetailByID(pszName)[0])
				{
				
					((CtmTextView*)(pHelpForm->Controls[i]))->LocalRows(pszName);
				}
			}	
		}
	
		pHelpForm->DoModal();
		onlinehelp--;
	}
}

void	OnlineHelp(CtmWnd *pwnd, char* pszFormName)
{
	CtmFormView* pHelpForm;
	CParseObject pParseHelp;	
	char	szCmd[256];
	g_pwndHelp = pwnd;	
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), pszFormName);
	//printf("%s\n", szCmd);
	pParseHelp.OpenFile(szCmd);
	
	pParseHelp.CloseFile();
	pHelpForm = (CtmFormView*)pParseHelp.m_aryControls[0].control;
	//printf("help Form name:%s\n", pHelpForm->Name);
	pHelpForm->SetParent(NULL);
	pHelpForm->CreateA();
	pHelpForm->DoModal();
}
/*---------------------------------------------------------------------------+
|  Function :  InstallCon                                                    |
|  Task     :  調用一個停機界面                                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL		InstallLogin(char*	pszFormName)
{
	static int	nCount = 0;
	
	if(nCount == 0 && !bLogin)
	{
		nCount++;
		CtmFormView* pPassWordForm;
		CParseObject pParseMsgBox;	
		
		char	szCmd[256];
		if(pszFormName != NULL)
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), pszFormName);
		else
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "Instlogin.txt");
		pParseMsgBox.OpenFile(szCmd);
		
		pParseMsgBox.CloseFile();
		pParseMsgBox.m_aryControls[0].control->CreateA();
		pPassWordForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
		
		pPassWordForm->DoModal();
		nCount--;
	}
	//if (g_pMainFrame->GetCurrentView()->Controls[0] != NULL)//增加判斷還原焦點部份
	//{
	//	if(((CtmFormView*)g_pMainFrame->GetCurrentView()->Controls[0])->FindMaxTabOrder()>=0)
	//	{
	//		g_pMainFrame->GetCurrentView()->Controls[0]->FindControlFromTab(
	//				((CtmFormView*)g_pMainFrame->GetCurrentView()->Controls[0])->Get_TabStop())->DrawGetFocus();
	//		((CtmFormView*)g_pMainFrame->GetCurrentView()->Controls[0])->OnGetFocus();
	//	}
	//}
	return TRUE;	
}

/*---------------------------------------------------------------------------+
|  Function :  InstallCon                                                    |
|  Task     :  彈出一個指定小畫面                                             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL		MsgForm(char* pszForName)
{
	
	CtmFormView* pPassWordForm;
	CParseObject pParseMsgBox;	
	
	char	szCmd[256];
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), pszForName);
	pParseMsgBox.OpenFile(szCmd);
	
	pParseMsgBox.CloseFile();
	pParseMsgBox.m_aryControls[0].control->CreateA();
	pPassWordForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	
	pPassWordForm->DoModal();
	
	return TRUE;	
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
void        Sleep(long lCount)
{
	sleep((unsigned int)lCount);
}

void        Wait(long lCount)
{
/*	MSG         msg;
	
	msg.wMessage = _SYS_WAIT;
	msg.lCounter = lCount;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);*/
}

void        ResetTime()
{
	MSG         msg;
	
//	//printf("ResetTime()\n");
	memset(&msg, 0, sizeof(MSG));
	msg.wMessage = _SYS_RESETTIME;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
}

void        Reset()
{
	MSG         msg;
	
	msg.wMessage = _SYS_RESET;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
}

void        Exit()
{
	MSG         msg;
	
	msg.wMessage = _SYS_EXIT;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
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
void        AssignCommandTarget (CtmWnd* pwnd)
{
	MSG         msg;
	
	msg.wMessage 	= _SYS_SETFOCUS;           // Assign command's target
	msg.pwnd 		= pwnd;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
}

void        ReleaseCommandTarget(CtmWnd* pwnd)
{
	MSG         msg;
	
	msg.wMessage = _SYS_KILLFOCUS;          // Release command's target
	msg.pwnd = pwnd;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
}

/*---------------------------------------------------------------------------+
|  Function :  ScreenSaverForm                                               |
|  Task     :   ScreenSaver                                                  |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL   ScreenSaverForm()//Sunny<20070404>
{
	CtmFormView* pScreenSaverForm;			//	這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject pParseMsgBox;	
	
	char	szCmd[256];
		
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "ScreenSaver.txt");

	pParseMsgBox.OpenFile(szCmd);
	
	pParseMsgBox.CloseFile();

	pParseMsgBox.m_aryControls[0].control->CreateA();
	pScreenSaverForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	//pScreenSaverForm->SetParent(g_pMainFrame);
	pScreenSaverForm->DoModal();
	
	return TRUE;
}

BOOL	TouchScreenAdjustForm()
{
	CtmFormView* pPassWordForm;			//	這裡之所以不需要釋放，是因pParseMsgBox會自動釋放它所有的元件
	CParseObject pParseMsgBox;	
	
	char	szCmd[256];
	bLogin = TRUE;
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "TouchScreenAdjust.txt");

	pParseMsgBox.OpenFile(szCmd);
	
	pParseMsgBox.CloseFile();
	pPassWordForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	//pPassWordForm->SetParent(g_pMainFrame->GetCurrentView());
	pPassWordForm->SetParent(NULL);
	pPassWordForm->CreateA();
	pPassWordForm->DoModal();
	
	//CtmFormView* pScreenSaverForm;
	//CParseObject pParseMsgBox;	
	//
	//char	szCmd[256];
	//	
	//sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "TouchScreenAdjust.txt");
    //
	//pParseMsgBox.OpenFile(szCmd);
	//pParseMsgBox.CloseFile();
    //
	//pParseMsgBox.m_aryControls[0].control->CreateA();
	//pScreenSaverForm = (CtmFormView*)pParseMsgBox.m_aryControls[0].control;
	//pScreenSaverForm->DoModal();
	
	return TRUE;	
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
BOOL        GetBypass()
{
MSG         msg;

msg.wMessage = _KEY_GETBYPASS;
g_pApplication->SetMessage(CMD_KEY, &msg);

return msg.bBypass;
}

void        SetBypass(BOOL bBypass)
{
MSG         msg;

msg.wMessage = _KEY_SETBYPASS;
msg.bBypass = bBypass;
g_pApplication->SetMessage(CMD_KEY, &msg);
}

void        PutKeyHead(WORD wKey)
{
MSG         msg;

msg.wMessage = _KEY_ADDHEAD;
msg.wKey = wKey;
g_pApplication->SetMessage(CMD_KEY, &msg);
}

void        PutKeyTail(WORD wKey)
{
MSG         msg;

msg.wMessage = _KEY_ADDTAIL;
msg.wKey = wKey;
g_pApplication->SetMessage(CMD_KEY, &msg);
}

void        PutCommand(WORD wCmd)
{
	MSG         msg;
	msg.wMessage = _SYS_COMMAND;
	msg.wCmd = wCmd;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
}


void PutCommand(char *pszCmd)
{
	MSG		msg;
	msg.wMessage = _SYS_COMMAND;
	msg.message	= _SYS_COMMAND;
	msg.lParam	= (LPARAM)pszCmd;
	g_pApplication->SetMessage(CMD_SYSTEM, &msg);	
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
void        Speech(BYTE cIndex)         //wy120999 add dor delay speech 100ms
{
}
void        Speech2(BYTE cIndex)
{
/*	
char        cData;

cData = g_acSpeech[cIndex];

    {
    __asm
        {
        push    dx

        mov     dx,SP_DATA
        mov     al,cData
        and     al,0x7F         //saty low
        out     dx,al

        pop     dx
        }
    }
*/
}

BOOL	_TestBit (WORD wFlag, WORD wIndex)
{
	/*bool		i=0x00; 			// jeckey have problem 20040405
	__asm
	("
		pushw	%%ax
		movw	%1,		%%ax
		btw		%%ax,	%2
		jc		_TB_ON
		movw	$0,		%0
		jmp		_TB_END
	_TB_ON:
		movw	$1,		%0
	_TB_END:		
		popw	%%ax
	"
	:"=g"(i)
	:"g"(BIndex),"g"(wFlag)
	//: "%ax"
	);
	return i;*/
	if(wFlag & (0x0001 << wIndex))
		return TRUE;
	else
		return FALSE;
/*	
	return wFlag & (0x0001 << wIndex)
	
	WORD    tmp;
    
    tmp = 0x0001;
    if (wIndex>0)
        tmp <<= wIndex;
    if ((wFlag&tmp) != 0)
        return TRUE;
    else
        return FALSE;
*/
}

short	_ScanBitR(WORD wFlag)
{
	/*static WORD i;				// jeckey have problem 20040405
	__asm
	("							
		pushw	%%ax 			
		bsfw	%1,		%%ax	
		movw	%0,		%%ax	
		popw	%%ax			
	"
	:"=m"(i)
	:"g"(wFlag)
	//:"%ax"
	);
	return i;*/
	BYTE    cBitID = 15;
	
    if (wFlag == 0) return -1;
    while (!(wFlag & tmMASK_BIT15))
    {
        wFlag <<= 1;               // find no error bit
        cBitID--;
    }
    return cBitID;
}
short		_ScanBitF(WORD wFlag)
{
	/*static WORD i;
	__asm
	("							
		pushw	%%ax 			
		bsrw	%1,		%%ax	
		movw	%0,		%%ax	
		popw	%%ax			
	"
	:"=g"(i)
	:"g"(wFlag)
	//:"%ax"
	);
	fprintf(stderr," ee %x",i);
	return i;*/
	BYTE    cBitID = 0;
	
    if (wFlag  == 0) return -1;
    while (!(wFlag & tmMASK_BIT0))
    {
        wFlag >>= 1;               // find no error bit
        cBitID++;
    }
    return cBitID;
}

WORD	_ClearBit(WORD wFlag, WORD wIndex)
{
	/*__asm
	("
		pushw	%%ax
		movw	%1,		%%ax
		btrw	%%ax,	%0
		popw	%%ax
	"
	:"=m"(wFlag)
	:"g"(wIndex)
	//: "%ax"
	);
	return	wFlag;*/
	wFlag  &= ~(0x0001 << wIndex);
	
	return wFlag;
/*	
	WORD    tmp;
    tmp = 0x0001;
    if (wIndex>0)
       tmp <<= wIndex;      
    tmp = tmp^0x0000;
    return (wFlag&tmp);
	return  (wFlag&0);
*/
}

WORD	_SetBit(WORD wFlag, WORD wIndex)
{
	return wFlag | (0x0001 << wIndex);
}

int		GetDecimalPowerVal(WORD wNPower)		//<<yang 2006/2/15 
{
	int		nValue = 1;
	short	i;

	if (wNPower > 0)
		for (i=1; i<=wNPower; i++)
			nValue = nValue * 10;
	
	return nValue;
}

/*--------------------------------------------------------------------------+
|  Function :  Delay()                                  					|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	Delay(DWORD	dwHSecond)
{
	usleep(dwHSecond*1000);//	fans	2006/12/9 01:48下午
	//	fans	2006/12/9 01:47下午
	//long	lCounter = dwHSecond;
	//
	//AddTimer(&lCounter);
	//while (lCounter > 0) ;
	//DeleteTimer(&lCounter);
}

struct lcd_backlight
{
	unsigned int pre_scaler;
	unsigned int period;
	unsigned int count;
};

void    _lcdpoweron()
{
	printf("lcd on \n");
	#ifdef	D_3354
	   
   	 		struct lcd_backlight	param;
		    param.pre_scaler 	= 0;
		    param.period 	= 100;
		    param.count 		= 101;
			if (_fbData.nfb == -1)
				_fbData.nfb	= open("/dev/fb0", O_RDWR, 0);
				
		    if (ioctl(_fbData.nfb, 0x40044632, &param) == -1) 
		    {
				;//perror("Error Setting backlight param");
		    }
			printf("3354 lcd on 1 _fbData.nfb=%d \n", _fbData.nfb);
	#elif defined(D_ATOM)
		printf("atom lcd on...\n");
		ioctl(g_nRTCFd, 0x1012/*LCD_CMD_POWRONOFF*/, 1);
	#else
		#ifdef	D_HMI270
		   printf("lcd on 1\n");
		    ioctl(g_nRTCFd, LCD_CMD_SIGNONOFF, 1);
		    usleep(200000);
		    ioctl(g_nRTCFd, LCD_CMD_POWRONOFF, 1);
		    usleep(100000);
		    ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 1);
	    #else
		    printf("lcd on 2 \n");
		    ioctl(g_nRTCFd, LCD_CMD_POWRONOFF, 1);
		    usleep(10000);
		    ioctl(g_nRTCFd, LCD_CMD_SIGNONOFF, 1);
		    usleep(10000);
		    ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 1);
	    #endif
    #endif
}

void    _lcdpoweroff()
{
	printf("lcd off \n");
	#ifdef	D_3354
	    if (_fbData.nfb == -1)
	    {
	        perror("Error: cannot open framebuffer device");
	    }
		else
		{
   	 		struct lcd_backlight	param;
		    param.pre_scaler 	= 0;
		    param.period 		= 100;
		    param.count 		= 0;
		
		    if (ioctl(_fbData.nfb, 0x40044632, &param) == -1) 
		    {
				;//perror("Error Setting backlight param");
		    }
		}
	#elif defined(D_ATOM)
		printf("atom lcd off ...\n");
		ioctl(g_nRTCFd, 0x1011/*LCD_CMD_SIGNONOFF*/, 0);
	#else
	    ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 0);
	    usleep(10000);
	    ioctl(g_nRTCFd, LCD_CMD_SIGNONOFF, 0);
	    usleep(10000);
	    ioctl(g_nRTCFd, LCD_CMD_POWRONOFF, 0);
	#endif
}

void		LCDPowerOn()
{
	_lcdpoweron();
}

void		LCDPowerOff()
{
	_lcdpoweroff();
}

void		LCDBackgroundOn()
{
	ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 1);
}

void		LCDBackgroundOff()
{
	ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 0);
}

void		Beep()
{
	//printf("u54_Beep\n");
#ifdef		D_ARM	
//#ifdef		D_HMI3354
	#ifdef	D_3354
		#ifdef	D_437X
			//printf("D_437x\n");
			_Gpio_buzzer(true);
			usleep(80000);
			_Gpio_buzzer(false);
		#else
		if(g_ntmDrive < 0)
		{
			g_ntmDrive = open("/dev/tmDrive", O_RDWR);
		}
		
		if(g_ntmDrive > 0)
		{
			ioctl(g_ntmDrive, 0, 0xC001);		//on	
			usleep(80000);
			ioctl(g_ntmDrive, 0, 0xC002);		//off
		}	
	
	//system("echo 1 > /sys/class/gpio/gpio19/value");
	//usleep(20000);
	//system("echo 0 > /sys/class/gpio/gpio19/value");
		#endif
	#else
	ioctl(g_nRTCFd, BUZ_CMD_ONOFF, 1);		//on
	usleep(80000);
	ioctl(g_nRTCFd, BUZ_CMD_ONOFF, 0);	//off
	#endif
	//Pxa255 使用
#else
	int console_fd;

	if(_fbData.ntty < 0)
	{
		if((console_fd = open ( "/dev/console", O_WRONLY)) == -1)
		{
			fprintf(stderr, "Failed to open console.\n");
			perror("console open");
			return;
		}
	}
	else	console_fd = _fbData.ntty;

	int magical_fairy_number = 476;

	ioctl(console_fd, KIOCSOUND, magical_fairy_number);
	usleep(75000);
	ioctl(console_fd, KIOCSOUND, 0);
	if(_fbData.ntty < 0)	close(console_fd);
#endif
}

void		OpenRTC()
{
//#ifdef		D_ARM
	g_nRTCFd = open(RTC_DEVICE, O_RDWR);
//#endif
}

void		CloseRTC()
{
#ifdef		D_ARM
	close(g_nRTCFd);
#endif
}
/*
void		SaveScreenToMMC(char* pcBuff, int nLength, char* szDir)
{
	char	szName[256];
	char	szFullName[256];
	char	szFullName_reName[256];
	char  szTemp1[256], szTemp2[256];
	int		nHandle;
	CtmWnd*	pCurrentView;
	int		nStatus;
	tmDATE        date;
	tmTIME        time;
	
	szName[0] = '\0';
	memset(szFullName, 0, sizeof(szFullName));
	memset(szFullName_reName, 0, sizeof(szFullName_reName));
	
	if (g_pMainFrame == NULL || g_pMainFrame->GetCurrentView() == NULL 
		|| (pCurrentView = g_pMainFrame->GetCurrentView()->GetControlA(1)) == NULL) return;
	if (szDir == NULL)
		strcpy(szFullName, SAVE_CURRENTVIEW_MMCPATH);
	else
		sprintf(szFullName, "%s%s%s", SAVE_MMC_DIR, szDir, "/");
	pCurrentView->GetPropValueT("printname", szName, sizeof(szName));
	if (szName == NULL || szName[0] == '\0')	
	{
		SendMsg(MSG_SH_SAVESCREEEN_NOVIEWNAME, 0, 0, NULL);
		return;
	}
	#ifdef	D_PXA270		//James add for pxa270 sd 優先找usb，然后再找sd 2009/3/19 10:56上午
	if(!g_wUSBStatus)
		nStatus = CtmUsbFile::GetInstance()->Mount(0);
	else
		nStatus = 1;
	
	if(nStatus < 1)
	{
		if(!g_wMMCStatus)
			nStatus = CtmMMCardFile::GetInstance()->Mount();
		else
			nStatus = 1;
	}
	else
	{
		if (szDir == NULL)
			strcpy(szFullName, SAVE_CURRENTVIEW_USBPATH);
		else
			sprintf(szFullName, "%s%s%s", SAVE_USB_DIR, szDir, "/");
	}
	#else
		#ifdef D_X86
			if(!g_wUSBStatus)
				nStatus = CtmUsbFile::GetInstance()->Mount(0);
			else
				nStatus = 1;
			
			if(nStatus < 1)
			{
				if(!g_wMMCStatus)
					nStatus = CtmMMCardFile::GetInstance()->Mount();
				else
					nStatus = 1;
			}
			else
			{
				if (szDir == NULL)
					strcpy(szFullName, SAVE_CURRENTVIEW_USBPATH);
				else
					sprintf(szFullName, "%s%s%s", SAVE_USB_DIR, szDir, "/");
			}
		#else
			if(!g_wMMCStatus)
				nStatus = CtmMMCardFile::GetInstance()->Mount();
			else
				nStatus = 1;
		#endif
	#endif
	//nStatus = mount("/dev/mmcda1", "/mnt/mmc", "vfat", 0, NULL);
	ExistDir(szFullName);
	//strcpy(szFullName_reName, szName);			//James add 2011/5/26 上午 09:57:30
	strcat(szFullName, szName);	
	
	#ifdef	D_PXA270
	if (nStatus < 1)
	#else
	if (nStatus < 1)	//Sunny<20080822> modify
	#endif
	{
		return;
	}
	
	nHandle = open(szFullName, O_RDWR);			//James add 2011/5/26 for rename saveformname
	if(nHandle < 0)			//No same name picture
	{
		nHandle = open(szFullName, O_RDWR | O_CREAT);
		if (nHandle < 0) return;
	}
	else					//have same name picture, need rename saveformname   formname + systemtime
	{
		close(nHandle);
		
		GetDate(&date);
		GetTime(&time);
		memset(szTemp1, 0, sizeof(szTemp1));
		memset(szTemp2, 0, sizeof(szTemp2));
		memcpy(szTemp1, szFullName, strlen(szFullName)-4);
		memcpy(szTemp2, &szFullName[strlen(szFullName)-4], 4);
		sprintf(szFullName_reName, "%s_%04d%02d%02d%02d%02d%02d%s", szTemp1, date.year, date.month, date.day,
		time.hour, time.minute, time.second, szTemp2);
		
		nHandle = open(szFullName_reName, O_RDWR | O_CREAT);
		//printf("nHandle 3=%d szFullName_reName=%s \n", nHandle, szFullName_reName);
		if (nHandle < 0) return;
	}
	
	fchmod(nHandle, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	write(nHandle, pcBuff, nLength);
	close(nHandle);
	//Prompt("saving data!", 2);
	//Prompt(g_MultiLanguage["VW_SAVING_DATA_NOW"], tmFT_CODE_TECH);
	SendMsg(MSG_SH_SAVESCREEEN_SAVINGDATA, 0, 0, NULL);
	if (g_wMMCStatus)			//James add for pxa270 sd 2009/3/19 10:56上午
	{
		CtmMMCardFile::GetInstance()->UMount();
	}
	if (g_wUSBStatus)
	{
		CtmUsbFile::GetInstance()->UMount();
	}
	//#ifdef	D_PXA270
	//CtmUsbFile::GetInstance()->UMount();
	//#else
	//CtmMMCardFile::GetInstance()->UMount();
	//#endif
	//umount("/mnt/mmc");
	//Prompt("", 2);
	//MsgBox(g_MultiLanguage["VW_COMPLETED_DATA_SAVE"], tmFT_CODE_TECH);
	SendMsg(MSG_SH_SAVESCREEEN_DATASAVE, 0, 0, NULL);
}
*/

void		SaveScreenToMMC(char* pcBuff, int nLength, char* szDir)
{
	char	szName[256];
	char	szFullName[256];
	char	szFullName_reName[256];
	char  	szTemp1[256], szTemp2[256];
	int		nHandle;
	CtmWnd*	pCurrentView;
	int		nStatus;
	tmDATE        date;
	tmTIME        time;
	
	szName[0] = '\0';
	memset(szFullName, 0, sizeof(szFullName));
	memset(szFullName_reName, 0, sizeof(szFullName_reName));
	
	if (g_pMainFrame == NULL || g_pMainFrame->GetCurrentView() == NULL 
		|| (pCurrentView = g_pMainFrame->GetCurrentView()->GetControlA(1)) == NULL) return;
	if (szDir == NULL)
		strcpy(szFullName, SAVE_CURRENTVIEW_MMCPATH);
	else
		sprintf(szFullName, "%s%s%s", SAVE_MMC_DIR, szDir, "/");
	pCurrentView->GetPropValueT("printname", szName, sizeof(szName));
	if (szName == NULL || szName[0] == '\0')	
	{
		MsgBoxConfirm(g_MultiLanguage["VM_NOTFOUND_VIEWNAME"], tmFT_CODE_TECH);
		return;
	}
	nStatus = 1;
	sprintf(szFullName, "./Data/ScreenShot/");
	ExistDir(szFullName);
	strcat(szFullName, szName);	
	
	if (nStatus < 1)
	{
		return;
	}
	
	nHandle = open(szFullName, O_RDWR);			//James add 2011/5/26 for rename saveformname
	if(nHandle < 0)			//No same name picture
	{
		nHandle = open(szFullName, O_RDWR | O_CREAT);
		if (nHandle < 0) return;
	}
	else					//have same name picture, need rename saveformname   formname + systemtime
	{
		close(nHandle);
		
		GetDate(&date);
		GetTime(&time);
		memset(szTemp1, 0, sizeof(szTemp1));
		memset(szTemp2, 0, sizeof(szTemp2));
		memcpy(szTemp1, szFullName, strlen(szFullName)-4);
		memcpy(szTemp2, &szFullName[strlen(szFullName)-4], 4);
		sprintf(szFullName_reName, "%s_%04d%02d%02d%02d%02d%02d%s", szTemp1, date.year, date.month, date.day,
		time.hour, time.minute, time.second, szTemp2);
		
		nHandle = open(szFullName_reName, O_RDWR | O_CREAT);
		//printf("nHandle 3=%d szFullName_reName=%s \n", nHandle, szFullName_reName);
		if (nHandle < 0) return;
	}

	fchmod(nHandle, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	write(nHandle, pcBuff, nLength);
	close(nHandle);
	Prompt(g_MultiLanguage["VW_SAVING_DATA_NOW"], tmFT_CODE_TECH);
	if (g_wMMCStatus)			//James add for pxa270 sd 2009/3/19 10:56?W?E
	{
		CtmMMCardFile::GetInstance()->UMount();
	}
	if (g_wUSBStatus)
	{
		CtmUsbFile::GetInstance()->UMount();
	}
	Prompt("", 2);
	MsgBoxConfirm(g_MultiLanguage["VW_COMPLETED_DATA_SAVE"], tmFT_CODE_TECH);
}
void		UpdateHMI()
{
	int		nStatus;
	
	nStatus = mount("/dev/mmcda1", "/mnt/mmc", "vfat", 0, NULL);
	ExistDir(UPDATE_SOURCE_PATH);
	system(UPDATE_PROGRAM_CMD);
	umount("/mnt/mmc");
	MsgBox("Copy is over. Please restart");
}

void		ExistDir(char* pszPath)
{
	DIR*		dir;
	
	dir = opendir(pszPath);
	if (!dir)
		mkdir(pszPath, 777);
	closedir(dir);
}


/*---------------------------------------------------------------------------+
|  Function :  void   StringToHex()                                          |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  StringToHex()                                                 |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		StringToHex(char* psz,long* pData)
{//LEO20070404
	char	ch;
	BYTE	i = 0,count = 0;
	long	result = 0;
	while((ch = *(psz+i))!='\0')
	{
		i++;
		count ++;
	}
	i = 0;
	while((((ch = *(psz+i))!='\0'))&&(count > 0))
	{
		if ((ch >= '0')&&(ch <= '9'))
		{
			result += ((ch-'0')*Power(16,count-1));
		}
		else if ((ch >= 'A')&&(ch <= 'F'))
		{
			result += ((ch-'A'+10)*Power(16,count-1));
		}
		else
		{
			*pData = 0;
			return;
		}
		count--;
		i++;
	}
	*pData = result;
}

/*---------------------------------------------------------------------------+
|  Function :  void   GetAccDate()           		                         |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  GetAccDate()              		                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        GetAccDate(tmDATE* pSDate, int nAccDays, tmDATE* pEDate)
{//LEO20070404
	tmDATE		TData;
	short       nDays;  
	
	TData.month = pSDate->month;
	nDays   	= pSDate->day;
	TData.year  = pSDate->year;

	if(g_wInstWay)	//Sunny<20090421> inst use month way
	{
		if(nAccDays <0)		//計算警報時間用到
		{
			nDays += nAccDays;
			while (nDays <= 0)
		    {
		        TData.month--;
		        if (TData.month == 0)
		        {
		            TData.month = 12;
		            TData.year--;
		        }
		        nDays += g_nDayOfMonth[TData.month-1];
		    }
		    pEDate->month = TData.month;
			pEDate->day   = nDays;
			pEDate->year  = TData.year;
		}
		else
		{
			nAccDays  = GetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD").lValue;//每個月的某日停機
			if(nAccDays>28)	nAccDays =28;	
			pEDate->day   = nAccDays;
			long lStageNow = GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue;
			pEDate->month = TData.month + lStageNow*1;
			
			if (pEDate->month % 12 == 0) 	
			{
				pEDate->year = TData.year + pEDate->month / 12 - 1;
				pEDate->month = 12;
			}
			else							
			{
				pEDate->year = TData.year + pEDate->month / 12;
				pEDate->month = pEDate->month % 12;
			}
						
			//if(pEDate->month >12)
			//{
			//	pEDate->month =pEDate->month - 12;
			//	pEDate->year  =TData.year+1;
			//}
			//else	pEDate->year  = TData.year;
		}
	}
	else
	{
		nDays += nAccDays;	
		if (nDays > 0)
		{
		    while (nDays > (int)g_nDayOfMonth[TData.month-1])
		    {
		        nDays -= g_nDayOfMonth[TData.month-1];
		        TData.month++;
		        if (TData.month == 13)
		        {
		            TData.month = 1;
		            TData.year++;
		        }
		    }
		}
		else
		{
		    while (nDays <= 0)
		    {
		        TData.month--;
		        if (TData.month == 0)
		        {
		            TData.month = 12;
		            TData.year--;
		        }
		        nDays += g_nDayOfMonth[TData.month-1];
		    }
		}
		pEDate->month = TData.month;
		pEDate->day   = nDays;
		pEDate->year  = TData.year;
	}

}

/*---------------------------------------------------------------------------+
|  Function :  WORD   GetManufCode()                                   |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  GetManufCode()                                          |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :@.WORD                           -                              |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function :  void   GetInitialPassword()                                   |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  GetInitialPassword()                                          |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        GetInitialPassword(char* pcMachineID, char* pcPassword, 
								tmDATE dtMachineDateOut, WORD wSystem)
{//LEO20070404
    WORD    wYear, wMonth, wDay;
    WORD    wTotal;
    tmDATE  dtMachineSystemDate;
    char    cPassword[7], cMachineID[9];
    char    cTemp;
    char    psz[10];
    int     i;

    GetDate(&dtMachineSystemDate);
    
    strcpy(cMachineID, pcMachineID);
    if (strlen(cMachineID)<8) 
    {
    	for(i=strlen(cMachineID); i<8; i++) cMachineID[i]=80;
    }
    cPassword[0] = '\0';
    if(1 == CtmConfig::GetInstance()->GetStopPWDType())		//new PWD
	{
	    wTotal  = (cMachineID[0]-30)*HIBYTE(wSystem)
	    		+ (cMachineID[1]-31)*LOBYTE(wSystem)
	            + (cMachineID[2]-44)*77 + (cMachineID[3]-27)*89
	            + (cMachineID[4]-36)*57 + (cMachineID[5]-24)*53
	            + (cMachineID[6]-27)*59 + (cMachineID[7]-16)*79;
	    wYear   = dtMachineSystemDate.year;
	    wMonth  = dtMachineSystemDate.month;
	    wDay    = dtMachineSystemDate.day;
	    wTotal  = wTotal + (wYear-2000)*277 + wMonth*131 + (wDay+10)*111;
	    wYear   = dtMachineDateOut.year;
	    wMonth  = dtMachineDateOut.month;
	    wDay    = dtMachineDateOut.day;
	    wTotal  = wTotal + (wYear-2000)*277 + wMonth*131 + (wDay+10)*111;
	}
	else 
	{
		wTotal  = (cMachineID[0]-40)*HIBYTE(wSystem)
	    		+ (cMachineID[1]-41)*LOBYTE(wSystem)
	            + (cMachineID[2]-46)*67 + (cMachineID[3]-29)*79
	            + (cMachineID[4]-38)*59 + (cMachineID[5]-28)*51
	            + (cMachineID[6]-29)*69 + (cMachineID[7]-18)*89;
	
	    wYear   = dtMachineSystemDate.year;
	    wMonth  = dtMachineSystemDate.month;
	    wDay    = dtMachineSystemDate.day;
	    wTotal  = wTotal + (wYear-2000)*289 + wMonth*109 + (wDay+10)*121;
	
	    wYear   = dtMachineDateOut.year;
	    wMonth  = dtMachineDateOut.month;
	    wDay    = dtMachineDateOut.day;
	    wTotal  = wTotal + (wYear-2000)*289 + wMonth*109 + (wDay+10)*121;			
	}
    sprintf(cPassword, "%4X", wTotal);
    cPassword[4] = '\0';
    sprintf(psz, "%2X", cPassword[2]+cPassword[3]);
    strcat(cPassword, psz);
    cPassword[6] = '\0';
    cTemp       = cPassword[0];
    cPassword[0]= cPassword[3];
    cPassword[3]= cTemp;
    strcpy(pcPassword, cPassword);
}

void        GetRFIDLoginPassword(char* pcMachineID, char* pcPassword,WORD wStages, WORD wPayStages)
{//LEO20070404
    WORD    wYear, wMonth, wDay;
    WORD    wTotal;
    tmDATE  dtMachineSystemDate;
    char    cPassword[7], cMachineID[9];
    char	cMaskCode[]="tia0512";
    char    cTemp;
    char    psz[10];
    int     i;

    GetDate(&dtMachineSystemDate);
    memset(cMachineID,0,sizeof(cMachineID)); 
    strncpy(cMachineID, pcMachineID,8);
    int nLen=strlen(cMachineID);
    if (nLen<8) 
    {
    	//for(i=strlen(cMachineID); i<8; i++) cMachineID[i]=80;
    	strncpy(cMachineID+nLen,cMaskCode,8-nLen);
    }
    for(int i=0; i<8; i++)
    {
    	printf("%c",cMachineID[i]);	
    }
    printf("\n");
    
    cPassword[0] = '\0';

	wTotal  = (cMachineID[0]-40)*31
	    	+ (cMachineID[1]-41)*52
	        + (cMachineID[2]-46)*67 + (cMachineID[3]-29)*79
	        + (cMachineID[4]-38)*59 + (cMachineID[5]-28)*51
	        + (cMachineID[6]-29)*69 + (cMachineID[7]-18)*89;
	
	 wYear   = dtMachineSystemDate.year;
	 wMonth  = dtMachineSystemDate.month;
	 wDay    = dtMachineSystemDate.day;
	 wTotal  = wTotal + (wYear-2000)*289 + wMonth*209 + (wDay+10)*121+(2015-1976)*169;			

    sprintf(cPassword, "%4X", wTotal);
    cPassword[4] = '\0';
    sprintf(psz, "%2X", cPassword[2]+cPassword[3]);
    strcat(cPassword, psz);
    cPassword[6] = '\0';
    cTemp       = cPassword[0];
    cPassword[0]= cPassword[3];
    cPassword[3]= cTemp;
    strcpy(pcPassword, cPassword);
}

/*---------------------------------------------------------------------------+
|  Function :  int   GetStageInstall()                                       |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :  GetStageInstall()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int         GetStageInstall(char* pcPassword, char* pcMachineID, 
							tmDATE dtStartStage, BYTE cStages, WORD wSystem)
{//LEO20070404
    WORD    wYear, wMonth, wDay;
    WORD    wTotal, wTotalStage;
    int     nStage;
    WORD    wTempData;
    long	lTempData;
    char    cValue[5], cMachineID[9];
    char    cTemp;
    int     i;

    strcpy(cMachineID, pcMachineID);
    if (strlen(cMachineID)<8) 
    {
    	for(i=strlen(cMachineID); i<8; i++) cMachineID[i]=80;
    }
    if(1 == CtmConfig::GetInstance()->GetStopPWDType())		//new PWD
	{
	    wTotal  = (cMachineID[0]-30)*HIBYTE(wSystem)
	            + (cMachineID[1]-31)*LOBYTE(wSystem)
	            + (cMachineID[2]-44)*77 + (cMachineID[3]-27)*89
	            + (cMachineID[4]-36)*57 + (cMachineID[5]-24)*53
	            + (cMachineID[6]-27)*59 + (cMachineID[7]-16)*79;
	    wYear   = dtStartStage.year;
	    wMonth  = dtStartStage.month;
	    wDay    = dtStartStage.day;
	    wTotal  = wTotal + (wYear-2000)*277 + wMonth*131 + (wDay+10)*111;
	}
	else
	{
		wTotal  = (cMachineID[0]-40)*HIBYTE(wSystem)
	    		+ (cMachineID[1]-41)*LOBYTE(wSystem)
	    		+ (cMachineID[2]-46)*67 + (cMachineID[3]-29)*79
	        	+ (cMachineID[4]-38)*59 + (cMachineID[5]-28)*51
	        	+ (cMachineID[6]-29)*69 + (cMachineID[7]-18)*89;
	    wYear   = dtStartStage.year;
	    wMonth  = dtStartStage.month;
	    wDay    = dtStartStage.day;
	    wTotal  = wTotal + (wYear-2000)*289 + wMonth*209 + (wDay+10)*121;
	}
	
    memcpy(cValue, &pcPassword[4], 2);
    cValue[2] = '\0';
    cTemp   =   pcPassword[0];
    pcPassword[0]=pcPassword[3];
    pcPassword[3]=cTemp;
    StringToHex(cValue,&lTempData);
    wTempData = (WORD)lTempData;
    wTotalStage = wTempData - pcPassword[3] - pcPassword[2];
    if (wTotalStage != (WORD)cStages) return 0;
    memcpy(cValue, pcPassword, 4);
    cValue[4] = '\0';
    StringToHex(cValue,(long *)&lTempData);
    wTempData = (WORD)lTempData;
    
    if ((wTempData-wTotal)%169) return 0;
    nStage = wTotalStage - (wTempData-wTotal)/169;
    if ((nStage<0)||(nStage>(int)wTotalStage)) nStage = 0;
    return nStage;
}

/*---------------------------------------------------------------------------+
|  Function :   CheckSum                                                     |
|  Task     :   Verify Data to transfer correctly                            |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD    CheckSum(WORD *data,WORD len)
{
    long sum=0;
    while(len>1)
    {
        sum+=*(data++);
        if (sum&0x80000000) sum=(sum&0xFFFF)+(sum>>16);
        len-=2;
    }
    if(len) sum+=(WORD)(*(BYTE *)data);
    while(sum>>16)
        sum=(sum&0xFFFF)+(sum>>16);
    return (WORD)~sum;
}


static void _InitCRC16Tab()
{
    int i, j;
    WORD crc, wc;

    for (i = 0; i < 256; i++)
    {
        crc  = 0;
        wc   = (WORD) i;

        for (j = 0; j < 8; j++)
        {
            if ( (crc ^ wc) & 0x0001 ) crc = ( crc >> 1 ) ^ 0xA001;
            else                      crc =   crc >> 1;

            wc = wc >> 1;
        }

        crc_tab16[i] = crc;
    }
	
    crc_tab16_init = TRUE;
}

static void _InitCRC32Tab()
{
	int i, j;
    DWORD crc;

    for (i = 0; i < 256; i++)
   	{
        crc = (DWORD)i;

        for (j = 0; j < 8; j++) {

            if (crc & 0x00000001L) crc = (crc >> 1) ^ 0xEDB88320L;
            else                   crc = crc >> 1;
        }

        crc_tab32[i] = crc;
    }

    crc_tab32_init = TRUE;
}

/*---------------------------------------------------------------------------+
|  Function :   _CRC16                             	                         |
|  Task     :   CRC-16 checksum					                             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
static WORD _CRC16(WORD crc, char c)
{
    WORD wTmp, wc;

    wc = 0x00ff & (WORD)c;

    if (!crc_tab16_init) _InitCRC16Tab();

    wTmp =  crc      ^ wc;
    crc = (crc >> 8) ^ crc_tab16[wTmp & 0xff];

    return crc;
}

static DWORD _CRC32(DWORD crc, char c)
{
	DWORD dwTmp, dwc;

    dwc = 0x000000ffL & (DWORD)c;

    if (!crc_tab32_init) _InitCRC32Tab();

    dwTmp = crc ^ dwc;
    crc = (crc >> 8) ^ crc_tab32[ dwTmp & 0xff ];

    return crc;
}

WORD	CRC16(WORD wCrc, char *pData, int nLen)
{
	while (nLen-- > 0)
	{
		wCrc = _CRC16(wCrc, *pData);
		pData++;
	}
	return wCrc;
}

DWORD CRC32(char *pData, int nLen)
{
	DWORD dwCRC = 0xffffffff;
	
	while (nLen-- > 0)
	{
		dwCRC = _CRC32(dwCRC, *pData);
		pData++;
	}
	
	dwCRC ^= 0xffffffff;
	
	return dwCRC;
}

DWORD CRC32(char *filename)
{
	DWORD dwCRC = 0xffffffff;
	FILE *fp;
	int	ch;
	
	if (fp = fopen(filename, "rb"))
	{
		while ((ch = fgetc(fp)) != EOF)
		{
			dwCRC = _CRC32(dwCRC, (char)ch);
		}
		
		fclose(fp);
	}
	
	dwCRC ^= 0xffffffff;
	
	return dwCRC;
}

/*---------------------------------------------------------------------------+
|  Function :   printfTM                                                     |
|  Task     :   print the info when debug			                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int printfTM( const char *fmt , ... )
{
	#ifdef D_DEBUG_TM
		char   buf[2048];   
		va_list   ap;   
		va_start(ap,   fmt);   
		int   len   =   vsprintf(buf,   fmt,   ap);   
		va_end(ap);   
		return printf(buf);   
	#else
		return 0;
	#endif
}






/*---------------------------------------------------------------------------+
|  Function :   OpenBattery                                                  |
|  Task     :   test battery power					                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		OpenBattery()				//yang 2007/7/31 for testing battery
{
#ifdef		D_ARM
	g_nBatteryFd = open(BATTERY_DEVICE, O_RDWR | O_NOCTTY);
#endif
}

/*---------------------------------------------------------------------------+
|  Function :   CloseBattery                                                 |
|  Task     :   test battery power					                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		CloseBattery()				
{
#ifdef		D_ARM
	 close(g_nBatteryFd);
#endif
}

/*---------------------------------------------------------------------------+
|  Function :   TestBatPower                                                 |
|  Task     :   test battery power					                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void		TestBatPower()				
{
#ifdef		D_ARM
	static	int	nCount = 60;
	static	int	nMinuteCount =0;
	int	nStat;
	if (++nCount >= 60)  //Sunny<20080218> 每分鐘檢查，連續五分鐘低電壓，再show出低電壓訊息
	{
		nStat = ioctl(g_nBatteryFd, 0x1000, 0);		
		if (!nStat)
		{
			if (++nMinuteCount >= 5)
			{
				Prompt(g_MultiLanguage["MSG_BATTRYLOWER"],1);  //Sunny<20080213>
				nMinuteCount =0;
			}			
		}		
		nCount =0;
	}
	
#endif
}

/*---------------------------------------------------------------------------+
|  Function : StrIDToStr													 |
|  Task     : 																 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	StrIDToStr2(char* pszID, char** psz, int nFont)
{
	char* pszTemp = NULL;
	int len = 0;
	unsigned short *pwCode;
	if (pszID != NULL)
	{
		pszTemp = g_MultiLanguage[pszID];
		
		if (pszTemp != NULL)
		{
			if (*psz != NULL) 
			{
				delete [](*psz);
				*psz = NULL;
			}
			if(nFont != tmFT_CODE_TECH)
			{
    			len = strlen(pszTemp);
    
    			*psz = new char[len + 1];
    			strcpy(*psz, pszTemp);
		    }
		    else
		    {
		    	
		        pwCode  = (unsigned short *)pszTemp;
		        while(pwCode[len ++] != 0);
		        *psz = new char[(len << 1)];
		        memcpy(*psz, pszTemp, len << 1);
		    }
		}
	}
}

void		Trim(char* pszDes, char* pszSrc,  int nLength)
{
	long	lLength;
	int 	i = 0;
	
	if (pszSrc == NULL || pszSrc[0] == '\0')
	{
		return;
	}
	lLength = strlen(pszSrc);
	//printf("pszSrc=%s pszDes=%s, nLength=%d, lLength=%d \n", pszSrc, pszDes, nLength, lLength);
	
	if (lLength > nLength)
	{
		return;
	}
	
	if (strcmp(pszSrc, pszDes) == 0)
	{
		return;
	}
	for (i=0; i<lLength; i++)
	{
		if (pszSrc[0] == ' ' || pszSrc[0] == '\t' || pszSrc[0] == '\b') 
		{
			pszSrc++;
			continue;
		}
		break;
	}
	lLength = strlen(pszSrc);
	memset(pszDes, 0, nLength);
	strcpy(pszDes, pszSrc);
	for (i=lLength-1; i>=0; i--)
	{
		if (pszDes[i] != ' ' && pszDes[i] != '\t' && pszDes[i] != '\b')
			break;
		pszDes[i] = '\0';
	}
	//printf("pszSrc=%s pszDes=%s, nLength=%d, lLength=%d \n", pszSrc, pszDes, nLength, lLength);
}


double	Pxa270DBConvert(double dInput)
{
	double dTemp = 0;
	DWORD	dwTemp1 = 0, dwTemp2 = 0;
	
	memcpy(&dwTemp1, &dInput, 4);
	memcpy(&dwTemp2, (BYTE*)&dInput + 4, 4);
	memcpy(&dTemp, &dwTemp2, 4);
	memcpy((BYTE*)&dTemp+4, &dwTemp1, 4);
	return 	dTemp;	
}

void		InputBox(CtmWnd* pwnd)
{
	static	int	nInputBox = 0;
	int		nViewTop = 10, nViewRight = 10;
	int		nInputXLength = 0, nInputYLength = 0;
	RECT	wndRect, inputRect, formviewRect;
	if (nInputBox == 0)
	{
		formviewRect.left 	= g_pMainFrame->GetViewRect().left;
		formviewRect.right 	= g_pMainFrame->GetViewRect().right;
		formviewRect.top 	= g_pMainFrame->GetViewRect().top;
		formviewRect.bottom = g_pMainFrame->GetViewRect().bottom;
		//printf("formviewRect.left=%d right=%d top=%d bottom=%d \n", 
		//formviewRect.left, formviewRect.right, formviewRect.top, formviewRect.bottom);
		nInputBox++;
		CtmFormView* pInputForm;			//	這裡之所以不需要釋放，是因ParseInputForm會自動釋放它所有的元件
		CParseObject ParseInputForm;	
		
		char	szCmd[256];
		bLogin = TRUE;
		//sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm.txt");
		
		//Gerry add for multiple input form style 2010/9/26 下午 04:20:51
		if (pwnd->Is("CtmEdit"))
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm_KB.txt");
		else if (pwnd->Is("CtmEditX1")||pwnd->Is("CtmDoubleSpinBox"))
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm_V.txt");
		else if (pwnd->Is("CtmEditX2"))
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm_HEX.txt");
		else if(pwnd->Is("CtmSpinBox"))
		{
			if(((CtmSpinBox*)pwnd)->GetDisplayBase()==16)
			{
				sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm_HEX.txt");
			}
			else
			{
				sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm_V.txt");
			}
		}
		else
			sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "InputForm_KB.txt");
				
		g_pWndInput	= pwnd;
		DWORD		oldbgc	= 0;
		
		if(g_pWndInput != NULL)
		{
			/*
			g_pWndInput->GetPropValueT("left", &wndRect.left, sizeof(wndRect.left));
			g_pWndInput->GetPropValueT("right", &wndRect.right, sizeof(wndRect.right));
			g_pWndInput->GetPropValueT("top", &wndRect.top, sizeof(wndRect.top));
			g_pWndInput->GetPropValueT("bottom", &wndRect.bottom, sizeof(wndRect.bottom));
			*/
			wndRect.left = g_pWndInput->GetmClientLeft();
			wndRect.top = g_pWndInput->GetmClientTop();
			wndRect.right = g_pWndInput->GetmClientRight();
			wndRect.bottom = g_pWndInput->GetmClientBottom();
			
		}
		//printf("wndRect l=%d, r=%d,t=%d, b=%d \n", wndRect.left, wndRect.right, wndRect.top, wndRect.bottom);
		ParseInputForm.OpenFile(szCmd);
		ParseInputForm.CloseFile();
		g_pWndInputForm = pInputForm = (CtmFormView*)ParseInputForm.m_aryControls[0].control;
		if(pInputForm != NULL)
		{
			pInputForm->GetPropValueT("left", &inputRect.left, sizeof(inputRect.left));
			pInputForm->GetPropValueT("right", &inputRect.right, sizeof(inputRect.right));
			pInputForm->GetPropValueT("top", &inputRect.top, sizeof(inputRect.top));
			pInputForm->GetPropValueT("bottom", &inputRect.bottom, sizeof(inputRect.bottom));
			/*
			inputRect.left = pInputForm->GetmClientLeft();
			inputRect.top = pInputForm->GetmClientTop();
			inputRect.right = pInputForm->GetmClientRight();
			inputRect.bottom = pInputForm->GetmClientBottom();*/
			//printf("2 inputRect l=%d, r=%d,t=%d, b=%d \n", inputRect.left, inputRect.right, inputRect.top, inputRect.bottom);
			
			nInputXLength = inputRect.right - inputRect.left;
			nInputYLength = inputRect.bottom - inputRect.top;
			
			/*if(wndRect.right +nViewRight + nInputXLength < formviewRect.right)//1.判斷放右
			{
				inputRect.left = wndRect.right + nViewRight;
				inputRect.right = inputRect.left + nInputXLength;
				
				
				
			}
			*/
			
			
			
			
			if(wndRect.right + nViewRight + nInputXLength < formviewRect.right)		//判斷超出右邊
			{
				if(wndRect.bottom + nViewTop + nInputYLength <  formviewRect.bottom)	//判斷超出下邊
				{
					inputRect.left 	= wndRect.right + nViewRight;
					inputRect.right = inputRect.left + nInputXLength;
					inputRect.top	= wndRect.top + nViewTop + 2;
					inputRect.bottom= inputRect.top + nInputYLength;
				}
				else if(wndRect.top - nViewTop - nInputYLength > formviewRect.top)				//判斷超出上邊
				{
					inputRect.left 	= wndRect.right + nViewRight;
					inputRect.right = inputRect.left + nInputXLength;
					inputRect.top	= formviewRect.bottom - nInputYLength - 2;
					inputRect.bottom= inputRect.top + nInputYLength;
				}
				else
				{
					inputRect.left 	= wndRect.right + nViewRight;
					inputRect.right = inputRect.left + nInputXLength;
					inputRect.top	= (formviewRect.bottom - nInputYLength)/2;
					inputRect.bottom= inputRect.top + nInputYLength;
				}				
			}
			else if((wndRect.left  - nViewRight - nInputXLength > formviewRect.left))			//判斷超出左邊
			{
				if(wndRect.bottom + nViewTop + nInputYLength <  formviewRect.bottom)	//判斷超出下邊
				{
					inputRect.left 	= wndRect.left - nViewRight - nInputXLength;
					inputRect.right = inputRect.left + nInputXLength;
					inputRect.top	= wndRect.top + nViewTop;
					inputRect.bottom= inputRect.top + nInputYLength;
				}
				else if(wndRect.top - nViewTop - nInputYLength > 0)				//判斷超出上邊
				{
					inputRect.left 	= wndRect.left - nViewRight - nInputXLength;
					inputRect.right = inputRect.left + nInputXLength;
					inputRect.top	= wndRect.bottom - nInputYLength;
					inputRect.bottom= inputRect.top + nInputYLength;
				}
				else
				{
					inputRect.left 	= wndRect.left - nViewRight - nInputXLength;
					inputRect.right = inputRect.left + nInputXLength;
					inputRect.top	= (formviewRect.bottom - nInputYLength)/2;
					inputRect.bottom= inputRect.top + nInputYLength;
				}	
			}
			else if (wndRect.bottom + nViewTop + nInputYLength < formviewRect.bottom)
			{
				inputRect.left = (formviewRect.right - nInputXLength) / 2;
				inputRect.right = inputRect.left + nInputXLength;
				inputRect.top = wndRect.bottom + nViewTop;
				inputRect.bottom = inputRect.top + nInputYLength;
			}
			else if (wndRect.top - nViewTop - nInputYLength > formviewRect.top)
			{
				inputRect.left = (formviewRect.right - nInputXLength) / 2;
				inputRect.right = inputRect.left + nInputXLength;
				inputRect.top = wndRect.top - nViewTop - nInputYLength;
				inputRect.bottom = inputRect.top + nInputYLength;
			}
			else
			{
				inputRect.left = (formviewRect.right - nInputXLength) / 2;
				inputRect.right = inputRect.left + nInputXLength;
				inputRect.top = (formviewRect.bottom - nInputYLength) / 2;
				inputRect.bottom = inputRect.top + nInputYLength;
			}
			
			pInputForm->SetProperty("left", &inputRect.left);
			pInputForm->SetProperty("top", &inputRect.top);
			pInputForm->SetProperty("right", &inputRect.right);
			pInputForm->SetProperty("bottom", &inputRect.bottom);
			
			//printf("inputRect l=%d, r=%d,t=%d, b=%d \n", inputRect.left, inputRect.right, inputRect.top, inputRect.bottom);
			pInputForm->SetParent(NULL);
			pInputForm->CreateA();
			pInputForm->DoModal();
			bLogin = FALSE;
		}
		
		nInputBox--;
		g_pWndInputForm = NULL;
		
		if(g_pWndInput != NULL)
			g_pWndInput->Update();
		g_pWndInput = NULL;
	}
}

DATE	tmDATE2DATE(tmDATE tmdate)
{
	DATE  	date;
	
	date.day		= tmdate.day;
	date.month		= tmdate.month;
	date.year		= tmdate.year;
	date.dayofweek	= tmdate.dayofweek;
	date.reserved	= 0;
	
	return date;
}



int		_tm_chartoword(WORD* pDest, char* pSource)
{
	int result = 0;
	
	if (pSource != NULL)
	{
		int len = strlen(pSource);
		for (int i = 0; i < len; i++)
		{
			*pDest	= *pSource;
			pDest++;
			pSource++;
			result++;
			
		}
		
	}
	return result;
}

int		_tm_chartoword(WORD* pDest, WORD* pSource)
{
	int i = 0;
	while (pSource != NULL && *pSource != 0)
	{
		*pDest = *pSource;
		pSource++;
		pDest++;
		i++;
	}	
	return i;
}


int		tm_sprintfE(void* pDest, const char* fmt, ...)
{
	//char* pTMSign	= "%T";
	//
	////	this is standard sprintf
	//if (memcmp(fmt, pTMSign, strlen(pTMSign)) != 0) 
	//{
	//	va_list argp;
	//	va_start(argp, fmt);
	//	int nResult = vsprintf((char*)pDest, fmt, argp);
	//	va_end(argp);
	//	return nResult;
	//}
	//else	//	this will change to TM Code
	//{
	//	char* pTempFmt 	= NULL;
	//	char szFormat[1024];
	//	char szValue[2048];
	//	WORD* pwDest = (WORD*)pDest;
	//	
	//	memset(szFormat, 0, sizeof(szFormat));
	//	
	//	va_list argp;
	//	va_start(argp, fmt);
	//	
	//	strcpy(szFormat, fmt + strlen(pTMSign));
	//	pTempFmt = szFormat;
	//	
	//	
	//	
	//	va_end(argp);
	//	
	//	return 0;
	//	
	//}
	
	return 0;
}

int		tm_sprintf(void* pDest, const char* fmt, ...)
{
		
	char*	pTm = "%t";
	
	va_list argp;			
	va_start(argp, fmt);	
	
	if (strstr(fmt, pTm) == NULL) 
	{
		int nResult = vsprintf((char*)pDest, fmt, argp);
		va_end(argp);
		return nResult;
	}
	else
	{
		WORD*	pTemp	= (WORD*)pDest;
		WORD*	pWord	= NULL;
		char*	plld	= "lld";
		char*	pchar	= (char*)strchr(fmt, '%');
		char	szValue[64];
		char	sz;
		
		char*	pc			= NULL;
		double	dtemp		= 0;
		long long llValue	= 0;
		
		while (pchar != NULL)
		{
			pchar++;					//	jump over the %
			
			if (strlen(pchar) >= 3 && memcmp(plld, pchar, 3) == 0)		//	long long valule
			{
				
					llValue = va_arg(argp, long long);
					sprintf(szValue, "%lld", llValue);					
					pchar 	= pchar + 3;
					pTemp += _tm_chartoword(pTemp, szValue);
					//	long long to word
			}
			else
			{
				switch (*pchar)
				{
					case 't':
						pWord = (WORD*)va_arg(argp, int);
						pTemp += _tm_chartoword(pTemp, pWord);
						break;
					case 'd':
						llValue = va_arg(argp, int);
						sprintf(szValue, "%d", llValue);
						pTemp += _tm_chartoword(pTemp, szValue);
						break;
					case 'x':
						llValue = va_arg(argp, int);
						sprintf(szValue, "%x", llValue);
						pTemp += _tm_chartoword(pTemp, szValue);
						break;
					case 'f':
						dtemp = va_arg(argp, double);
						sprintf(szValue, "%f", dtemp);
						pTemp += _tm_chartoword(pTemp, szValue);
						break;
					case 's':
						pc = va_arg(argp, char*);
						pTemp += _tm_chartoword(pTemp, pc);
						break;
					case 'c':							
						sz = va_arg(argp, int);
						*pTemp = sz;
						pTemp++;
						break;
					default:
						*pTemp = *pchar;
						pTemp++;
						break;
				}
				pchar++;
			}
				
			
			
			
			char*	pp	= pchar;
			pchar	= strchr(pchar, '%');
			if (pchar != NULL)
			{
				int strlen = pchar - pp;
				for (int i = 0; i < strlen; i++)
				{
					*pTemp = *pp;
					pTemp++;
					pp++;
				}
			}
		}
		
	}
	va_end(argp);
	
	return 0;
}

int		tm_strlen(void* psz, BOOL bTech)
{
	if (!bTech) return strlen((char*)psz);
	else
	{
	
		int nResult = 0;
		if (psz != NULL)
		{
			unsigned short *pwCode;
			pwCode  = (unsigned short *)psz;
			while(pwCode[nResult ++] != 0);
			
			nResult *= 2;
			
		}
		return nResult;
	}
}
void		SendPrompMsg(char* pszMsg, int AddMsgFlag)
{
	static 	WORD	wIndex = 0;
	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	if(wIndex >= 15 || wIndex < 0 )
		wIndex = 0;
	memset(&szPropmt[wIndex], 0, sizeof(szPropmt[wIndex]));
	msg.message	= 10001;	
	strcpy(szPropmt[wIndex], pszMsg);
	msg.wParam	= AddMsgFlag;	
	msg.lParam	= (long)szPropmt[wIndex];
	msg.pAdd    = pszMsg;
	wIndex++;
	g_pApplication->QueueMessage(&msg);
}

//int	ParseErrorID(char* pszErrorStr, DWORD dwErrorID)
//{
//	WORD    *pwDst, *pwSrc;
//	pwDst = (WORD *)pszErrorStr; 
//	
//	int 	j = 0;
//	if((dwErrorID & 0xFFFF0000) == 0)	//54警報
//	{
//		pwSrc    = (WORD *)g_MultiLanguage[dwErrorID];
//		
//		if(pwSrc != NULL)
//		while(*pwSrc != 0 && j < 256)
//		{
//		    *pwDst ++   = *pwSrc ++;
//		    j++;
//		}
//		*pwDst = 0;
//		return j * 2;
//	}
//	//28警報
//	BYTE byObject, bySubIndex;
//	WORD wErrorID;
//	byObject 	= (dwErrorID & 0xFF000000) >> 24; 
//	bySubIndex 	= (dwErrorID & 0x00FF0000) >> 16;
//	wErrorID	= dwErrorID & 0x0000FFFF; 
//	char szTemp[64], szStrID[64];
//	switch(byObject)
//	{
//		case ERROR_OBJECT_N2C:	
//			sprintf(szTemp, "[N%d]", bySubIndex);
//			sprintf(szStrID, "N2C_ALARM%d", wErrorID);
//			//printf("szStrID= %s\n", szStrID);
//			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
//			break;
//		case ERROR_OBJECT_E2C:
//			sprintf(szTemp, "[E%d]", bySubIndex);
//			sprintf(szStrID, "E2C_ALARM%d", wErrorID);
//			//printf("szStrID= %s\n", szStrID);
//			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
//			break;	
//		case ERROR_OBJECT_AUSTONE:
//			sprintf(szTemp, "[A%d]", bySubIndex);
//			sprintf(szStrID, "AUSTONE_ALARM%d", wErrorID);
//			//printf("szStrID= %s\n", szStrID);
//			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
//			break;
//		case ERROR_OBJECT_SIEMENS:
//			sprintf(szTemp, "[S%d]", bySubIndex);
//			sprintf(szStrID, "SIEMENS_ALARM%d", wErrorID);
//			//printf("szStrID= %s\n", szStrID);
//			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
//			break;
//		default:
//			sprintf(szTemp, "[Un %d,%d]", byObject,bySubIndex);
//			sprintf(szStrID, "UNKNOWN_ALARM%d", wErrorID);
//			//printf("szStrID= %s\n", szStrID);
//			return ErrorIDToStr(szTemp, szStrID ,wErrorID, pwDst);
//			break;	
//	}
//}
//int	ErrorIDToStr(char* szTemp, char* szStrID ,WORD wErrorID, WORD *pwDst)
//{
//	WORD   *pwSrc;
//	int 	j = 0;
//	if(g_MultiLanguage.GetStrKeyIndex(szStrID) == -1)
//	{	
//		sprintf(szTemp, "%s Error: %d", szTemp, wErrorID);
//		while(*szTemp != 0)     
//		{
//			*pwDst ++ = *szTemp ++;
//			j++;
//		}
//		*pwDst = 0;
//		return j * 2;
//	}
//	else
//	{
//		while(*szTemp != 0)     
//		{
//			*pwDst ++ = *szTemp ++;
//			j ++;
//		}
//		*pwDst ++ =0x20;
//		j ++;
//		pwSrc    = (WORD *)g_MultiLanguage[szStrID];
//		if(pwSrc != NULL)
//		while(*pwSrc != 0 && j < 256)
//		{
//		    *pwDst ++   = *pwSrc ++;
//		    j ++;
//		}
//		*pwDst = 0;
//		return j * 2;
//	}
//}

unsigned int BKDRHash(WORD* str, unsigned int len)
{
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;
   char szTemp[5];
  
   for(i = 0; i < len; str++, i++)
   {
      sprintf(szTemp, "%04x", *str);
      for(int j =0; j < 4; j++)
      	hash = (hash * seed) + (szTemp[j]);
   }

   return hash;
}

BOOL	SaveRecordView(long lRecordID,long lOldValue, long lNewValue)
{
	tmModifyRecordSet_Oil	TempRecord;	
	TempRecord.OldValue = lOldValue;
	TempRecord.NewValue = lNewValue;
	TempRecord.DBIndex = lRecordID;
	//printf("TempRecord.DBIndex=%x,lRecordID=%x\n",TempRecord.DBIndex,lRecordID);
	GetDate(&TempRecord.date);
	GetTime(&TempRecord.time);
	if (g_pszUserID != NULL && g_pszUserID[0] != '\0')
		strncpy(TempRecord.szID, g_pszUserID, sizeof(TempRecord.szID) - 1);
	else
		strcpy(TempRecord.szID, "NOLOGIN");
	if (TempRecord.OldValue != TempRecord.NewValue)
		return CRecordSetControl::GetInstance()->SaveModifyRecord(&TempRecord);
	return 0;
}

BOOL	IsInterSection(int nLeft, int nRight, int nTop, int nBottom)
{
	BOOL	bResult=FALSE;
	if(g_MemoCurCoordinate.nBottom<=0||g_MemoCurCoordinate.nRight<=0)	return bResult;
	if(ReturnMaxNum(nLeft,g_MemoCurCoordinate.nLeft)<=ReturnMinNum(nRight,g_MemoCurCoordinate.nRight)&&
		ReturnMaxNum(nTop,g_MemoCurCoordinate.nTop)<=ReturnMinNum(nBottom,g_MemoCurCoordinate.nBottom))
	{
		bResult=TRUE;
	}
	
	return bResult;	
}

template <class T>
T		ReturnMaxNum(const T X, const T Y)
{
	return X>=Y ? X:Y;
}

template <class T>  
T 		ReturnMinNum(const T X, const T Y)
{
	return X<=Y ? X:Y;
}

static double TurnCrossToMold(double S0,TMSELVATPARAM* param,double gamma=0)
{
	
	const double PI=3.1415926;
	double Sm=0;
	if(S0<=0.0)	return Sm;
	double alpha ,sita1,sita2,sita,fair;
	double n,demon,num;
	if(param->M<=0||param->D<=0)
	{
		//TM梓袧唳數呾鼠宒
		if(param->L3>0)
		{
			param->M=(param->L3*param->L3+param->L4*param->L4
			-param->L5*param->L5)/(2*param->L3);
			param->D=sqrt(param->L4*param->L4-param->M*param->M);
		}
			
	}
	num=(param->M);
	demon=sqrt(param->M*param->M+param->D*param->D);
	alpha=acos(num/demon);
	//printf("alpha=%lf\n",alpha);
	//printf("%lf,%lf\n",(param->H2-sqrt(param->M*param->M+param->D*param->D)*sin((alpha+param->An))),param->L1);
	gamma=asin((param->H2-sqrt(param->M*param->M+param->D*param->D)*sin((alpha+param->An)))
			/param->L1);
	//printf("gamma=%lf\n",gamma);
	n=sqrt(param->M*param->M+param->D*param->D)
	  *cos(alpha+param->An)-S0-(param->L1*cos(gamma));
	if(n>=0)
	sita1=PI-asin(param->H2/sqrt(n*n+param->H2*param->H2));
	else
	sita1=asin(param->H2/sqrt(n*n+param->H2*param->H2));	
	demon=2*sqrt(n*n+param->H2*param->H2)
	      *sqrt((param->M*param->M)+(param->D*param->D));
	num=(n*n)+(param->H2*param->H2)+(param->M*param->M)
		+(param->D*param->D)-(param->L1*param->L1);
	sita2=acos(num/demon);
	sita=PI-sita1-sita2;
	if(sita<alpha)	return Sm;
		
	fair=asin((param->L3*sin(sita-alpha)-param->H1)/param->L2);
	
	Sm=param->L3*cos(param->An)
	   +sqrt((param->L2*param->L2)-(param->L3*sin(param->An)-param->H1)*(param->L3*sin(param->An)-param->H1))
	   -param->L3*cos(sita-alpha)-param->L2*cos(fair);
	if(param->exSm<=0.0)
	{
		if(Sm<65535)
			return Sm;
		else
			return S0;
	}
	else 
	{
		if(Sm<=param->exSm)   
			return Sm;
		else if(Sm<65535)
		{
			return param->exSm;
		}
		else
		{
			return S0;	
		}
	}	
}

int InitCrossMoldTable(TMSELVATPARAM* param,double S0)
{
	//double factor=S0/MAX_DIVISOR_NUM;
	const double factor=0.25;
	int	nResult=0;  
	double Sm=0.0;//耀啣弇离
	while(Sm<param->exSm)
	{
		Sm=TurnCrossToMold(nResult*factor,param);
		u_mCrossMoldMap[Sm]=nResult*factor;
		nResult++;
	}
	
	return nResult;
}

double 	TurnMoldToCross(double Sm)
{
	double dResult=0;
	std::map<double, double>::iterator it_lower = u_mCrossMoldMap.lower_bound(Sm);
	if(it_lower==u_mCrossMoldMap.end())		return -1.0;
	dResult=it_lower->second; 
	return dResult;
}

static double	TurnMoldToCross(double Sm,TMSELVATPARAM* param)
{
	double A,B,C,D;
	double alpha,num,demon;
	if(Sm<0.0)		return 0.0;
	if(param->exSm>0.0)
	{
		if(Sm>param->exSm)	Sm=param->exSm;
	}
	if(param->M<=0||param->D<=0)
	{
		//TM梓袧唳數呾鼠宒
		if(param->L3>0)
		{
			param->M=(param->L3*param->L3+param->L4*param->L4
			-param->L5*param->L5)/(2*param->L3);
			param->D=sqrt(param->L4*param->L4-param->M*param->M);
		}
			
	}
	num=(param->M);
	demon=	sqrt(param->M*param->M+param->D*param->D);
	alpha=	acos(num/demon);
	A=	param->L3*cos(param->An)+
	  	sqrt(param->L2*param->L2-(param->L3*sin(param->An)-param->H1)*
	  	(param->L3*sin(param->An)-param->H1))-Sm;
	C=	sqrt(A*A+param->H1*param->H1);
	D=	atan(param->H1/A);
	B=	(A*A+param->L3*param->L3+param->H1*param->H1-param->L2*param->L2)/(2*param->L3);
	double 	tempA=	acos(B/C);
	double	sita=	tempA+D+alpha;
	double	OAn	=	asin((param->H2-sqrt(param->M*param->M+param->D*param->D)*sin(sita))/param->L1);
	double	n=	sqrt(param->M*param->M+param->D*param->D)*cos(sita)-param->L1*cos(OAn);
	double	gamma=asin((param->H2-sqrt(param->M*param->M+param->D*param->D)*sin((alpha+param->An)))
			/param->L1);
	double	S0=	sqrt(param->M*param->M+param->D*param->D)*cos(alpha+param->An)-param->L1*cos(gamma)-n;
	if(S0<65535)
		return	S0;
	else
		return  Sm;	
}

double        TMSELVATFUNC::GetOilVat(double x)
{
	return	TurnMoldToCross(x,&u_tmSelvatParam);
}
double        TMSELVATFUNC::OilToPosi(double y)
{
	return TurnCrossToMold(y,&u_tmSelvatParam);
}

double        TMSELVATFUNC::GetMaxPosi()
{
	double result=0;
	for(int i=100; i<=65535; i++)
	{
	 	double y=TurnCrossToMold(i,&u_tmSelvatParam);
	 	if(y>=result)	result=y;
	 	else
	 	{	
	 		u_tmSelvatParam.exSm=result;		
	 		break;
	 	}
	 	
	 }
	 return result;	
}

void	ResetCrossMoldTable()
{
	u_mCrossMoldMap.clear();	
}

DRIVERPARAM 	u_DriverParam[]={
	//ID, 	dwAddr, dwValue, bDataType =0 拸虴, 1=BYTE, 2=WORD ,3=DWORD, 4=FLOAT, bDataPrecision, bDataWritable=0硐黍 2褫黍褫迡 3硐迡 
	{{0},	0,		0,		0,		0,	0},
	{{1}, 	0,		0,		0,		0,	0},
	{{2}, 	0,		0,		0,		0,	0},	
	{{3}, 	0,		0,		0,		0,	0},
	{{4}, 	0,		0,		0,		0,	0},
	{{5}, 	0,		0,		0,		0,	0},
	{{6}, 	0,		0,		0,		0,	0},	
	{{7}, 	0,		0,		0,		0,	0},
	{{8}, 	0,		0,		0,		0,	0},
	{{9}, 	0,		0,		0,		0,	0},
	{{10}, 	0,		0,		0,		0,	0},	
	{{11}, 	0,		0,		0,		0,	0},
	{{12}, 	0,		0,		0,		0,	0},
	{{13}, 	0,		0,		0,		0,	0},
	{{14}, 	0,		0,		0,		0,	0},	
	{{15}, 	0,		0,		0,		0,	0},
	{{16}, 	0,		0,		0,		0,	0},
	{{17}, 	0,		0,		0,		0,	0},
	{{18}, 	0,		0,		0,		0,	0},	
	{{19}, 	0,		0,		0,		0,	0},
	{{20}, 	0,		0,		0,		0,	0},
	{{21}, 	0,		0,		0,		0,	0},
	{{22}, 	0,		0,		0,		0,	0},	
	{{23}, 	0,		0,		0,		0,	0},
	{{24}, 	0,		0,		0,		0,	0},
	{{25}, 	0,		0,		0,		0,	0},
	{{26}, 	0,		0,		0,		0,	0},	
	{{27}, 	0,		0,		0,		0,	0},
	{{28}, 	0,		0,		0,		0,	0},
	{{29}, 	0,		0,		0,		0,	0},
	{{30}, 	0,		0,		0,		0,	0},	
	{{31}, 	0,		0,		0,		0,	0},
	{{32}, 	0,		0,		0,		0,	0},
	{{33}, 	0,		0,		0,		0,	0},
	{{34}, 	0,		0,		0,		0,	0},	
	{{35}, 	0,		0,		0,		0,	0},
	{{36}, 	0,		0,		0,		0,	0},
	{{37}, 	0,		0,		0,		0,	0},
	{{38}, 	0,		0,		0,		0,	0},			 	
	{{39}, 	0,		0,		0,		0,	0},
	{{40}, 	0,		0,		0,		0,	0},
	{{41}, 	0,		0,		0,		0,	0},	
	{{42}, 	0,		0,		0,		0,	0},
	{{43}, 	0,		0,		0,		0,	0},
	{{44}, 	0,		0,		0,		0,	0},
	{{45}, 	0,		0,		0,		0,	0},	
	{{46}, 	0,		0,		0,		0,	0},
	{{47}, 	0,		0,		0,		0,	0},
	{{48}, 	0,		0,		0,		0,	0},
	{{49}, 	0,		0,		0,		0,	0},	
	{{50}, 	0,		0,		0,		0,	0},
	{{51}, 	0,		0,		0,		0,	0},
	{{52}, 	0,		0,		0,		0,	0},
	{{53}, 	0,		0,		0,		0,	0},	
	{{54}, 	0,		0,		0,		0,	0},
	{{55}, 	0,		0,		0,		0,	0},
	{{56}, 	0,		0,		0,		0,	0},
	{{57}, 	0,		0,		0,		0,	0},	
	{{58}, 	0,		0,		0,		0,	0},
	{{59}, 	0,		0,		0,		0,	0},
	{{60}, 	0,		0,		0,		0,	0},
	{{61}, 	0,		0,		0,		0,	0},	
	{{62}, 	0,		0,		0,		0,	0},
	{{63}, 	0,		0,		0,		0,	0},
	{{64}, 	0,		0,		0,		0,	0},	
	{{65}, 	0,		0,		0,		0,	0},
	{{66}, 	0,		0,		0,		0,	0},
	{{67}, 	0,		0,		0,		0,	0},
	{{68}, 	0,		0,		0,		0,	0},	
	{{69}, 	0,		0,		0,		0,	0},
	{{70}, 	0,		0,		0,		0,	0},
	{{71}, 	0,		0,		0,		0,	0},
	{{72}, 	0,		0,		0,		0,	0},	
	{{73}, 	0,		0,		0,		0,	0},
	{{74}, 	0,		0,		0,		0,	0},
	{{75}, 	0,		0,		0,		0,	0},
	{{76}, 	0,		0,		0,		0,	0},	
	{{77}, 	0,		0,		0,		0,	0},
	{{78}, 	0,		0,		0,		0,	0},
	{{79}, 	0,		0,		0,		0,	0},
	{{80}, 	0,		0,		0,		0,	0},	
	{{81}, 	0,		0,		0,		0,	0},
	{{82}, 	0,		0,		0,		0,	0},
	{{83}, 	0,		0,		0,		0,	0},
	{{84}, 	0,		0,		0,		0,	0},	
	{{85}, 	0,		0,		0,		0,	0},
	{{86}, 	0,		0,		0,		0,	0},
	{{87}, 	0,		0,		0,		0,	0},
	{{88}, 	0,		0,		0,		0,	0},	
	{{89}, 	0,		0,		0,		0,	0},
	{{90}, 	0,		0,		0,		0,	0},
	{{91}, 	0,		0,		0,		0,	0},
	{{92}, 	0,		0,		0,		0,	0},	
	{{93}, 	0,		0,		0,		0,	0},
	{{94}, 	0,		0,		0,		0,	0},
	{{95}, 	0,		0,		0,		0,	0},
	{{96}, 	0,		0,		0,		0,	0},	
	{{97}, 	0,		0,		0,		0,	0},
	{{98}, 	0,		0,		0,		0,	0},
	{{99}, 	0,		0,		0,		0,	0},
	{{100}, 	0,		0,		0,		0,	0},	
	{{101}, 	0,		0,		0,		0,	0},
	{{102}, 	0,		0,		0,		0,	0},
	{{103}, 	0,		0,		0,		0,	0},
	{{104}, 	0,		0,		0,		0,	0},	
	{{105}, 	0,		0,		0,		0,	0},
	{{106}, 	0,		0,		0,		0,	0},
	{{107}, 	0,		0,		0,		0,	0},
	{{108}, 	0,		0,		0,		0,	0},	
	{{109}, 	0,		0,		0,		0,	0},
	{{110}, 	0,		0,		0,		0,	0},
	{{111}, 	0,		0,		0,		0,	0},
	{{112}, 	0,		0,		0,		0,	0},	
	{{113}, 	0,		0,		0,		0,	0},
	{{114}, 	0,		0,		0,		0,	0},
	{{115}, 	0,		0,		0,		0,	0},
	{{116}, 	0,		0,		0,		0,	0},	
	{{117}, 	0,		0,		0,		0,	0},
	{{118}, 	0,		0,		0,		0,	0},
	{{119}, 	0,		0,		0,		0,	0},
	{{120}, 	0,		0,		0,		0,	0},	
	{{121}, 	0,		0,		0,		0,	0},
	{{122}, 	0,		0,		0,		0,	0},
	{{123}, 	0,		0,		0,		0,	0},
	{{124}, 	0,		0,		0,		0,	0},	
	{{125}, 	0,		0,		0,		0,	0},
	{{126}, 	0,		0,		0,		0,	0},
	{{127}, 	0,		0,		0,		0,	0},
	{{128}, 	0,		0,		0,		0,	0},	
	{{129}, 	0,		0,		0,		0,	0},
	{{130}, 	0,		0,		0,		0,	0},
	{{131}, 	0,		0,		0,		0,	0},
	{{132}, 	0,		0,		0,		0,	0},	
	{{133}, 	0,		0,		0,		0,	0},
	{{134}, 	0,		0,		0,		0,	0},
	{{135}, 	0,		0,		0,		0,	0},
	{{136}, 	0,		0,		0,		0,	0},	
	{{137}, 	0,		0,		0,		0,	0},
	{{138}, 	0,		0,		0,		0,	0},
	{{139}, 	0,		0,		0,		0,	0},
	{{140}, 	0,		0,		0,		0,	0},	
	{{141}, 	0,		0,		0,		0,	0},
	{{142}, 	0,		0,		0,		0,	0},
	{{143}, 	0,		0,		0,		0,	0},
	{{144}, 	0,		0,		0,		0,	0},	
	{{145}, 	0,		0,		0,		0,	0},
	{{146}, 	0,		0,		0,		0,	0},
	{{147}, 	0,		0,		0,		0,	0},
	{{148}, 	0,		0,		0,		0,	0},	
	{{149}, 	0,		0,		0,		0,	0},
	{{150}, 	0,		0,		0,		0,	0},
	{{151}, 	0,		0,		0,		0,	0},
	{{152}, 	0,		0,		0,		0,	0},	
	{{153}, 	0,		0,		0,		0,	0},
	{{154}, 	0,		0,		0,		0,	0},
	{{155}, 	0,		0,		0,		0,	0},
	{{156}, 	0,		0,		0,		0,	0},	
	{{157}, 	0,		0,		0,		0,	0},
	{{158}, 	0,		0,		0,		0,	0},
	{{159}, 	0,		0,		0,		0,	0},
	{{160}, 	0,		0,		0,		0,	0},	
	{{161}, 	0,		0,		0,		0,	0},
	{{162}, 	0,		0,		0,		0,	0},
	{{163}, 	0,		0,		0,		0,	0},
	{{164}, 	0,		0,		0,		0,	0},	
	{{165}, 	0,		0,		0,		0,	0},
	{{166}, 	0,		0,		0,		0,	0},
	{{167}, 	0,		0,		0,		0,	0},
	{{168}, 	0,		0,		0,		0,	0},
	{{169}, 	0,		0,		0,		0,	0},
	{{170}, 	0,		0,		0,		0,	0},	
	{{171}, 	0,		0,		0,		0,	0},
	{{172}, 	0,		0,		0,		0,	0},
	{{173}, 	0,		0,		0,		0,	0},
	{{174}, 	0,		0,		0,		0,	0},	
	{{175}, 	0,		0,		0,		0,	0},
	{{176}, 	0,		0,		0,		0,	0},
	{{177}, 	0,		0,		0,		0,	0},
	{{178}, 	0,		0,		0,		0,	0},	
	{{179}, 	0,		0,		0,		0,	0},
	{{180}, 	0,		0,		0,		0,	0},
	{{181}, 	0,		0,		0,		0,	0},
	{{182}, 	0,		0,		0,		0,	0},	
	{{183}, 	0,		0,		0,		0,	0},
	{{184}, 	0,		0,		0,		0,	0},
	{{185}, 	0,		0,		0,		0,	0},
	{{186}, 	0,		0,		0,		0,	0},	
	{{187}, 	0,		0,		0,		0,	0},
	{{188}, 	0,		0,		0,		0,	0},
	{{189}, 	0,		0,		0,		0,	0},
	{{190}, 	0,		0,		0,		0,	0},	
	{{191}, 	0,		0,		0,		0,	0},
	{{192}, 	0,		0,		0,		0,	0},
	{{193}, 	0,		0,		0,		0,	0},
	{{194}, 	0,		0,		0,		0,	0},	
	{{195}, 	0,		0,		0,		0,	0},
	{{196}, 	0,		0,		0,		0,	0},
	{{197}, 	0,		0,		0,		0,	0},
	{{198}, 	0,		0,		0,		0,	0},	
	{{199}, 	0,		0,		0,		0,	0},
	{{200}, 	0,		0,		0,		0,	0},
	{{201}, 	0,		0,		0,		0,	0},
	{{202}, 	0,		0,		0,		0,	0},	
	{{203}, 	0,		0,		0,		0,	0},
	{{204}, 	0,		0,		0,		0,	0},
	{{205}, 	0,		0,		0,		0,	0},
	{{206}, 	0,		0,		0,		0,	0},	
	{{207}, 	0,		0,		0,		0,	0},
	{{208}, 	0,		0,		0,		0,	0},
	{{209}, 	0,		0,		0,		0,	0},
	{{210}, 	0,		0,		0,		0,	0},	
	{{211}, 	0,		0,		0,		0,	0},
	{{212}, 	0,		0,		0,		0,	0},
	{{213}, 	0,		0,		0,		0,	0},
	{{214}, 	0,		0,		0,		0,	0},	
	{{215}, 	0,		0,		0,		0,	0},
	{{216}, 	0,		0,		0,		0,	0},
	{{217}, 	0,		0,		0,		0,	0},
	{{218}, 	0,		0,		0,		0,	0},	
	{{219}, 	0,		0,		0,		0,	0},
	{{220}, 	0,		0,		0,		0,	0},
	{{221}, 	0,		0,		0,		0,	0},
	{{222}, 	0,		0,		0,		0,	0},	
	{{223}, 	0,		0,		0,		0,	0},
	{{224}, 	0,		0,		0,		0,	0},
	{{225}, 	0,		0,		0,		0,	0},	
	{{226}, 	0,		0,		0,		0,	0},
	{{227}, 	0,		0,		0,		0,	0},
	{{228}, 	0,		0,		0,		0,	0},
	{{229}, 	0,		0,		0,		0,	0},	
	{{230}, 	0,		0,		0,		0,	0},
	{{231}, 	0,		0,		0,		0,	0},
	{{232}, 	0,		0,		0,		0,	0},
	{{233}, 	0,		0,		0,		0,	0},	
	{{234}, 	0,		0,		0,		0,	0},
	{{235}, 	0,		0,		0,		0,	0},
	{{236}, 	0,		0,		0,		0,	0},
	{{237}, 	0,		0,		0,		0,	0},	
	{{238}, 	0,		0,		0,		0,	0},
	{{239}, 	0,		0,		0,		0,	0},
	{{240}, 	0,		0,		0,		0,	0},
	{{241}, 	0,		0,		0,		0,	0},	
	{{242}, 	0,		0,		0,		0,	0},
	{{243}, 	0,		0,		0,		0,	0},
	{{244}, 	0,		0,		0,		0,	0},
	{{245}, 	0,		0,		0,		0,	0},	
	{{246}, 	0,		0,		0,		0,	0},
	{{247}, 	0,		0,		0,		0,	0},
	{{248}, 	0,		0,		0,		0,	0},
	{{249}, 	0,		0,		0,		0,	0},	
	{{250}, 	0,		0,		0,		0,	0},
	{{251}, 	0,		0,		0,		0,	0},
	{{252}, 	0,		0,		0,		0,	0},
	{{253}, 	0,		0,		0,		0,	0},	
	{{254}, 	0,		0,		0,		0,	0},
	{{255}, 	0,		0,		0,		0,	0},	
};


char*	StrToke_TE(char* Tok, char* ptr)
{
	static char* lastToken =NULL;
	char *tmp;
	if (ptr == NULL) 
	{
		ptr = lastToken;
		if (ptr == NULL)         
			return NULL;
	} 
	ptr += strspn(ptr, Tok); 
	tmp = strpbrk(ptr, Tok);
	if(tmp)
	{
		*tmp = '\0';
		lastToken = tmp + 1;
	} 
	else
		lastToken = NULL;
		
	return ptr;
 
}
