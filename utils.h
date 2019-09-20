/*===========================================================================+
|  Function : Utilities                                                      |
|  Task     : Utilities header file                                          |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 04/27/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
 
#ifndef     D_UTILS
#define     D_UTILS

#include    "common.h"
#include    "wnd.h"
#include	"msgbox.h"
#include    "files.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     DATE_OK             0
#define     TIME_OK             0
#define     DATE_ERROR_YEAR     1
#define     DATE_ERROR_MONTH    2
#define     DATE_ERROR_DAY      3
#define     TIME_ERROR_HOUR     4
#define     TIME_ERROR_MINUTE   5
#define     TIME_ERROR_SECOND   6

#define     ROC_START           1911 
#define     C20_START           1900

#ifdef	D_ATOM
#define     RTC_DEVICE      "/dev/rtc2"
#else
#define     RTC_DEVICE      "/dev/rtc"
#endif
#define		BATTERY_DEVICE	"/dev/battery"

//	2009-4-14 11:45:51 fans add 
#ifdef	D_3354
#define		SAVE_USB_DIR				"/media/sda1/"
#define		SAVE_MMC_DIR				"/media/sda1/"
#else
#define		SAVE_USB_DIR				"/mnt/usb/"
#define		SAVE_MMC_DIR				"/mnt/usb/"
#endif

#define     MAX_PAGE_CHARCOUNT  12// е倍龄篈 <<LEO20060920

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
enum        MONTH               {JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC};

typedef struct tagMONIUPDATE              // St1217'01 for moni lose
    {
    DWORD       dwShotCount;         
    BOOL        bChange;             
    } MONIUPDATE;
    
typedef struct tagPAGELIST      //<<LEO20060920
    {
    char        acPagePassword[12];
    char*       pszIDView;
    } tmPAGELIST;
    
typedef   struct	tmtagDriverParam{
	//ID, 	dwAddr, dwValue, 
	//bDataType =0 无效, 1=BYTE, 2=WORD ,3=DWORD, 4=FLOAT,5=INT 
	//bDataPrecision, bDataWritable=0只读 2可读可写 3只写 
	union
	{
		WORD	wTotal;
		WORD	wID;
	};
	
	DWORD	dwAddr;
	DWORD	dwValue;
	BYTE	bDataType;
	BYTE	bDataPrecision;
	BYTE	bDataWritable;
}DRIVERPARAM;

//add by J.Wang 2015/7/7 16:32:59     
typedef struct tmSelvatParam
{
	double L1;
	double L2;
	double L3;
	/*TM标准版计算公式所需参数*/
	double L4;
	double L5;
	/*************************/
	/*双马计算公式所需参数*/
	/*使用TM标准版以下两个参数给-1或者不赋值*/
	double M;
	double D;
	double H1;
	double H2;
	double An;
	double exSm;
	BOOL   bUsed;
}TMSELVATPARAM; 

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
//******
//      BCD2BIN convert the BCD value in byte (BCD) to binary value in ax
//******
#define     BCD2BIN(BCD)        __asm                               \
                                {                                   \
                                __asm   mov     ah,BCD              \
                                __asm   shr     ax,4                \
                                __asm   shr     al,4                \
                                __asm   aad                         \
                                }

//******
//      BCD2ASC convert the BCD value in byte (BCD) to ASCII value in ax
//******
#define     BCD2ASC(BCD)        __asm                               \
                                {                                   \
                                __asm   mov     ah,BCD              \
                                __asm   shr     ax,4                \
                                __asm   shr     al,4                \
                                __asm   add     ax,3030h            \
                                __asm   xchg    ah,al               \
                                }

//******
//      BIN2BCD convert the binary value in byte (BIN) to BCD value in al
//******
#define     BIN2BCD(BIN)        __asm                               \
                                {                                   \
                                __asm   mov     al,BYTE PTR BIN     \
                                __asm   aam                         \
                                __asm   shl     ah,4                \
                                __asm   or      al,ah               \
                                }

//******
//
//******
#define     BCD2BINW(w)         (((w)>>12)*1000+(((w)>>8)&0x000F)*100+(((w)>>4)&0x000F)*10+((w)&0x000F))
#define		DSPCMMANDTYPE		(CtmConfig::GetInstance()->GetCommType())
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
void		OpenRTC();
void		CloseRTC();
void		GetRTCDateTime(void * pdate);
int			SetRTCDate(tmDATE date);
int			SetRTCTime(tmTIME time);
void		GetRTCDate(tmDATE* pDate);	//	fans add 2006/12/30 10:37と
void		GetRTCTime(tmTIME *pTime);	//	fans add 2006/12/30 10:38と

void		OpenBattery();				//yang 2007/7/31 for testing battery
void		CloseBattery();				
void		TestBatPower();				

void        GetDate(tmDATE* pdate);
void        GetDate(char* pszDate, char cSeparator);
void        CheckDate();                             //St1213'01 for check date
WORD        SetDate(tmDATE  date);
WORD        SetDate(char* pszDate, char cSeparator);
WORD        CvtDate(char* pszDate, char cSeparator, tmDATE* pdate);
WORD        CvtDate(tmDATE* pdate);
void        GetTime(tmTIME* ptime);
void        GetTime(char* pszTime, char cSeparator);
void		GetSiemensDate(DWORD dwDate, tmDATE* pdate);						//<<yang 2006/3/8 
void        GetSiemensTime(DWORD dwTime, tmTIME* ptime);						//<<yang 2006/3/8 
WORD        SetTime(tmTIME  atime);
WORD        SetTime(char* pszTime, char cSeparator);
WORD        CvtTime(char* pszTime, char cSeparator, tmTIME* ptime);

void        StrNCpy (char* psz1, char* psz2, int nCount);
void        StrNSet (char* psz,  char  ch,   int nCount);
void        StrSubst(char* psz1, char* psz2, int n);
int			StrLenTM(char* psz);	//	fans add 2008/7/21 12:51と
void        GetFileName(char* pszName, char* pszPath);

void        F2A(char* psz, short nPrecision, long long ll);
void        LL2A(char* psz, short nPrecision, long long ll, int nCodeType = 0);		//<<yang 2006/3/3
void        A2A(char* psz, short nPrecision, int nCodeType);			//Sunny<20070829> 
long long	A2F(char* psz, short nPrecision);						//<<yang 2006/10/20 
WORD        HEX2BCD(WORD value);    //St032300 for ClsHiPres Max Value
WORD        DEC2HEX(WORD wValue/*,WORD* cDecimal*/);   //St040700 for Meter Adjust
void		StrIDToStr2(char* pszID, char** psz, int nFont = tmFT_CODE_TECH);

long        Power(int nBase, int n);
WORD        CountDigit(WORD wValue);
WORD        CountDigit(long lValue);
WORD        CountDigit(long long llValue);					//<<yang 2006/3/3 

void        Msg(short nMsg);
void        Msg(char* pszMsg);
void        MsgBeep(short nMsg);
void        MsgBeep(char* pszMsg);
void        MsgExt(short nMsg);
void        MsgExt(char* pszMsg);
void        MsgBeepExt(short nMsg);
void        MsgBeepExt(char* pszMsg);

void        Prompt(short nMsg);
void        Prompt(char* pszMsg,int AddMsgFlag = 0, int MsgShowType = 1);
void        Sec5Prompt(char* pszMsg,int AddMsgFlag = 0, int MsgShowType = 1);
void		PromptE(char* pszMsg, int MsgShowType = 1);
void        PromptBeep(short nMsg);
void        PromptBeep(char* pszMsg);
void        PromptStat(char* pszMsg);              //Bw0122'03
void		SendPrompMsg(char* pszMsg, int AddMsgFlag);

BOOL        MsgBox(short nMsg);
BOOL        MsgBox(char* pszMsg, int nType = 0);
BOOL        	MsgBox(char* pszMsg, char* pszMsg2,int nType = 0);
BOOL        Login(char*	pszFormName = NULL);
BOOL					FileProcess();
BOOL					MsgBoxConfirm(char* pszMsg, int nType =0);
BOOL					MsgBoxSpecial(int nType);
BOOL					MsgBoxCall(char* MsgBoxID);
BOOL					MsgBoxCall(char* MsgBoxID ,char* pszMsg);
BOOL					MsgBoxCall(char* MsgBoxID,int char_num , ...);		//20180307 add by Mario  MsgB㊣ㄧΑ
BOOL					WorkCoordSeatCall(int f_NO);
BOOL        StartUp();
BOOL		InstallLogin(char*	pszFormName = NULL);
BOOL		MsgForm(char* pszForName);
void		OnlineHelp(CtmWnd *pwnd, char* pszFormName = "OnLineHelp.txt");

BOOL        ScreenSaverForm();	//Sunny<20070404>
BOOL		TouchScreenAdjustForm();
void        Sleep(long lCount);
void        Wait(long lCount);
void        ResetTime();
void        Reset();
void        Exit();
void		Delay(DWORD	dwHSecond);
void        AssignCommandTarget (CtmWnd* pwnd);
void        ReleaseCommandTarget(CtmWnd* pwnd);

BOOL        GetBypass();
void        SetBypass(BOOL bBypass);

void        PutKeyHead(WORD wKey);
void        PutKeyTail(WORD wKey);
void        PutCommand(WORD wCmd);
void        PutCommand(char* pszCmd);

void        Speech(BYTE cIndex);
void        Speech2(BYTE cIndex);       ////wy120999 add dor delay speech 100ms

int			GetDecimalPowerVal(WORD wNPower);		//<<yang 2006/2/15 

void		LCDPowerOn();
void		LCDPowerOff();
void		LCDBackgroundOn();
void		LCDBackgroundOff();
void		Beep();

void		SaveScreenToMMC(char* pcBuff, int nLength, char* szDir = NULL);
void		UpdateHMI();
void		ExistDir(char* pszPath);
// function for stop machine LEO20070404
void		StringToHex(char* psz,long* pData);
void        GetAccDate(tmDATE* pSDate, int nAccDays, tmDATE* pEDate);
void        GetRFIDLoginPassword(char* pcVendorCode, char* pcPassword,WORD wStages=0,WORD wPayStages=0);
void        GetInitialPassword(char* pcMachineID, char* pcPassword, 
								tmDATE dtMachineDateOut, WORD wSystem);
int         GetStageInstall(char* pcPassword, char* pcMachineID, 
								tmDATE dtStartStage, BYTE cStages, WORD wSystem);
WORD   		CheckSum(WORD *data,WORD len);

WORD		CRC16(WORD wCrc, char *pData, int nLen);
DWORD 		CRC32(char *pData, int nLen);
DWORD 		CRC32(char *filename);

void		Trim(char* pszDes, char* pszSrc,  int nLength);

double	Pxa270DBConvert(double dInput);
void		InputBox(CtmWnd* pwnd);


void		InitHelpTimer();
void		DelHelpTimer();
BOOL		HelpTimeout();
void		ResetHelpTimer();

int		GetKernelVers1();
int		GetKernelVers2();
int		GetKernelVers3();

BOOL	SaveRecordView(long lRecordID,long lOldValue, long lNewValue);
void	OnlineHelpXml(char* pszFormName,int tab=-1,char*pszName=0);


BOOL	IsInterSection(int nLeft, int nRight, int nTop, int nBottom);
template <class T>
T		ReturnMaxNum(const T X, const T Y);
template <class T>
T 		ReturnMinNum(const T X, const T Y);

int		InitCrossMoldTable(TMSELVATPARAM* param,double S0=0);
double 	TurnMoldToCross(double Sm);
void	ResetCrossMoldTable();


//add by J.Wang 2015/7/14 14:05:00
typedef struct tmTMSELVATFUNC
{
	static 	double         GetOilVat(double x);
 	static	double         OilToPosi(double y);
 	static	double		   GetMaxPosi();
}TMSELVATFUNC;

int			SetOnlineHelp(char *pszName, CtmWnd *pwnd);
//int			ParseErrorID(char* pszErrorStr, DWORD dwErrorID);
//int			ErrorIDToStr(char* szTemp, char* szStrID ,WORD wErrorID, WORD *pwDst);

char*	StrToke_TE( char* Tok,char* ptr=NULL); 
unsigned int BKDRHash(WORD* str, unsigned int len);

int 		System(char* pszCmd);

#ifdef      __cplusplus
extern      "C" {
#endif

/*---------------------------------------------------------------------------+
;| Function : WORD __far _ScanBit (WORD wFlag)                               |
;|            BOOL __far _TestBit (WORD wFlag, WORD wIndex)                  |
;|            WORD __far _ClearBit(WORD wFlag, WORD wIndex)                  |
;|            WORD __far _SetBit  (WORD wFlag, WORD wIndex)                  |
;| Task     : Bit maniplation                                                |
;+---------------------------------------------------------------------------+
;| Call     : wIndex = _ScanBit (wFlag)                                      |
;|            bOn    = _TestBit (wFlag, wIndex)                              |
;|            wFlag  = _ClearBit(wFlag, wIndex)                              |
;|            wFlag  = _SetBit  (wFlag, wIndex)                              |
;|                                                                           |
;| Parameter:                           -                                    |
;|                                                                           |
;| Return   :                           -                                    |
;+--------------------------------------------------------------------------*/
short	_ScanBitF(WORD wFlag);
short	_ScanBitR(WORD wFlag);
BOOL	_TestBit (WORD wFlag, WORD wIndex);
WORD	_ClearBit(WORD wFlag, WORD wIndex);
WORD	_SetBit  (WORD wFlag, WORD wIndex);
DATE	tmDATE2DATE(tmDATE tmdate);

#ifdef      __cplusplus
}
#endif

//	fans add
int printfTM( const char *fmt , ... );


//	fans add 2009-4-1 11:19:14
//=====================================================
//	techmation string function
//=====================================================
int		tm_sprintf(void* pDest, const char* fmt, ...);
int		tm_strlen(void* psz, BOOL bTech = FALSE);
int		_tm_chartoword(WORD* pDest, char* pSource);
int		_tm_chartoword(WORD* pDest, WORD* pSource);
int		tm_sprintfE(void* pDest, const char* fmt, ...);

/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      MONIUPDATE  g_MoniUpdate;       // St1217'01 for moni lose
extern      WORD        g_wIDView;								//yang 2006/4/19 
extern      WORD        g_flMotorStatus;						// 520 皑笷篈
extern      WORD        g_flHeaterStatus;						// 521 筿荐篈
extern      WORD        g_flHeaterStatusB;						// 家放诀ㄏノ

extern      WORD        g_flHeater2Status;
extern      WORD        g_flMotor2Status;

extern      BOOL        g_bShortCutKeyEnterView;				// е倍龄篈 <<LEO20060920
extern      char        g_acSavePageAndPassword[];				// е倍龄篈 <<LEO20060920
extern      short       g_nSaveCharCount;						// е倍龄篈 <<LEO20060920
extern		BYTE		g_cAirBlastCKey;
extern		BYTE		g_cAirBlastDKey;
extern      WORD        g_wOperationCount;                     //<Sunny20070327> 巨璸计
extern      BOOL        g_bScreenSaverRelease;					//玂篈
extern      WORD		g_wOPStatus;
extern		int			g_nBatteryFd;
extern		int			g_nRTCFd;
extern		WORD		g_wInstWay;								//Sunny<20090421> inst way: month or day
extern		int			g_nUpdateDevice;						//James add 2009/12/17
extern		CtmWnd*		g_pWndInput;
extern		CtmWnd*		g_pwndHelp;
extern		CtmWnd*		g_pWndInputForm;
extern		CtmWnd*		g_pWndDataSetForm;
extern		CtmWnd*		g_pWndDataSetForm2;
extern		long long 	g_lDataSetValue;
extern		char		g_pszDataTextID[256];
extern		BOOL		g_bAutoHotPlug;							//James add for hotplug 2010/12/1 と 11:06:10
extern		char		g_cPszFileName[];						//James add for rename filename 2011/8/3
extern		char				g_pFileExplPath[1024];
extern		BOOL		g_bInitDone;
extern		TMSELVATPARAM	u_tmSelvatParam;
extern		BOOL		g_bDemoDataID;
extern		DRIVERPARAM 	u_DriverParam[];
#endif
