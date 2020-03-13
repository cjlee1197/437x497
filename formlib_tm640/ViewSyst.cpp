/*===========================================================================+
|  Class    : ViewSyst library                                               |
|  Task     : ViewSyst library action source file                            |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include "ViewSyst.h"
#include "../utils.h"
#include "../commonaction.h"
#include "../language.h"
#include "../selectedit.h"
#include "../main.h"
//#include	"../control.h"
#include	"../tmcontrol.h"
//#include "../taskinet.h"
#include	"../lib/libiNetComm.h"
//	fans add 2009/5/21 03:12下午
#include	"../ethnet.h"
#include	"../tmshmsg.h"
#include	"../iotable.h"
//#include	"../selvat_ancestor.h"
#include	"../lib/dsp54/tmSelvatG.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     RTC_START       1900
#define     C21_START       2000
#define     EDIT_MAC        "editMAC_"
#define     EDIT_IP         "editIP_"

#define BACKLIGHT_LEVEL "/usr/Data/BL_LEVEL"
#define BACKLIGHT_PATH "/sys/class/backlight/backlight/brightness"
#define BgLight_Width 30
#define BgLight_Position 125-BgLight_Width

#define		NO					0  //
#define		OK					1  //
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*		pwndSelectLanguage     	= NULL;
CtmWnd*     pwndEditMonth        	= NULL;
CtmWnd*     pwndEditYear         	= NULL;
CtmWnd*     pwndEditDay          	= NULL;
CtmWnd*     pwndEditHour         	= NULL;
CtmWnd*     pwndEditMinute       	= NULL;
CtmWnd*     pwndEditSecond       	= NULL;
CtmWnd*     pwndStaticMessage    	= NULL;
CtmWnd*     pwndEditConfirm       = NULL;
CtmWnd*     pwndMOTORONTIME			  = NULL;  //ZHBA 2011-5-13
CtmWnd*     pwndStaticMTRTIMEUNIT = NULL;  //ZHBA 2011-5-13
CtmWnd*     pwndStaticMOTORONTIME	= NULL;  //ZHBA 2011-5-13

//CtmWnd*     pwndEditUNITTRANSFER  = NULL;

CtmWnd*		pwndEditIP0		 		= NULL;
CtmWnd*		pwndEditIP1        		= NULL;
CtmWnd*     pwndEditIP2        		= NULL;
CtmWnd*     pwndEditIP3        		= NULL;
CtmWnd*     pwndEditConfirmNet 	    = NULL;
CtmWnd*     pwndEditConfirmMac 	    = NULL;

CtmWnd*     pwndStaticLogOut      	= NULL;
CtmWnd*     pwndEditLogOut        	= NULL;
CtmWnd*     pwndStaticSampling     	= NULL;
CtmWnd*     pwndEditSampling       	= NULL;
CtmWnd*     pwndeditPUSHKEYOIL      = NULL;
CtmWnd*     pwndStaticPUSHKEYOIL    = NULL;
CtmWnd*     pwndeditPOSIUNIT    	= NULL;
CtmWnd*     pwndeditTEMPUNIT    	= NULL;
CtmWnd*     pwndeditPRESUNIT    	= NULL;
CtmWnd*     pwndSYST_BRITISHUNIT    = NULL;

CtmWnd*     pwndeditUNITTRANSOPT    = NULL;
CtmWnd*     pwndeditINJEMAXDPEED    = NULL;
CtmWnd*     pwndeditHMITYPE    		= NULL;
CtmWnd*     pwndSelectZERO1    		= NULL;


CtmWnd*     pwndStaticUNIT1      	= NULL;
CtmWnd*     pwndStaticUNIT2      	= NULL;

CtmWnd*     pwndStaticVW_ACT_PRS     = NULL;

CtmWnd*     pwndeditOPENDAY      = NULL; /*zyh  2014/2/13 for 7NG*/
CtmWnd*     pwndeditOPENHOUR      	= NULL;
CtmWnd*     pwndeditOPENMINUTE      = NULL;
CtmWnd*     pwndeditTIME_CLEAN      = NULL;

CtmWnd*     pwndeditAUTOTIMEDAY      = NULL;
CtmWnd*     pwndeditAUTOTIMEHOUR      	= NULL;
CtmWnd*     pwndeditAUTOTIMEMINUTE      = NULL;

CtmWnd*     pwndeditMOTORON_HOUR      = NULL;
CtmWnd*     pwndeditMOTORON_MINUTE    = NULL;
CtmWnd*     pwndeditHEATERON_HOUR     = NULL;
CtmWnd*     pwndeditHEATERON_MIN      = NULL;

CtmWnd*		pwndPassWord	 		= NULL;

CtmWnd*		pwndImgBgLight	 		= NULL; // 背光數值
CtmWnd*		pwndImgBgLightBar	 	= NULL; // 背光數值條

CtmWnd*   pwndBtnConfirm	= NULL; // 確認 Btn
CtmWnd*   pwndBtn_Beep		= NULL; // 按鍵音 Btn

CtmWnd* 	pwndLocalMac[6];
CtmWnd* 	pwndLocalIP[4];

CtmWnd*   pwndeditLCDPOWERON 	    = NULL;
int 		languageindex = 0, LogoIndex = 0;
int 		LG_selected =0;
bool  	 	biNetControlsVisible 	= FALSE;
BOOL  	 	btimeSetVisible 	= FALSE;
BYTE		m_acPhysicalAddr[6], m_acPhysicalAddr0[6];
BYTE		acRemoteIP[4], acLocalIP[16];
 
tmDATE      u_OldDate;               // Wain 2007/04/17 Add <<
tmTIME      u_OldTime;               // Wain 2007/04/17 Add >>
BOOL		u_biNet = FALSE;		//	fans add 2009/5/21 03:08下午
BOOL		u_bKeyLock = FALSE;	

CIniFile	m_DefaultIni;	

BOOL pMsgBox = FALSE;
int nOldPresUnitCh = 0;
int nIndex = 0;
long lMaxSpd1 = 0;
long lMaxSpd2 = 0;	
DWORD		m_dwOpenTime = 0; /*zyh  2014/2/13 */
DWORD		m_dwOpenTimeDay = 0;
WORD		m_dwOpenTimeHour = 0; 
WORD		m_wOpenTimeMinute = 0;
DWORD		m_dwAutoTime = 0; /*for 7YL */
DWORD		m_dwAutoTimeDay = 0;
WORD		m_dwAutoTimeHour = 0; 
WORD		m_dwAutoTimeMinute = 0;
BOOL 		m_b7TR  	=	FALSE;
BOOL 		b_PushKey  	=	FALSE;
BOOL 		b_7LG		=	FALSE;
BOOL 		b_7HDUnit  	=	FALSE;
BOOL 		b_7ZS		=	FALSE;
BOOL 		b_7YL  		=	FALSE;
BOOL 		b_7ASDCUnit =	FALSE;
BOOL 		b_7ASUnit  	=	FALSE;
BOOL 		b_7ZTUnit  	=	FALSE;
BOOL 		b_7AS		=	FALSE;
BOOL 		b_7ZF  		=	FALSE;
BOOL 		b_7LN  		=	FALSE;
BOOL		b_AutoTime	=	FALSE;
WORD        OilDataOld  = 0,OilDataNew = 0;
int         iPosUnitOld  = 0,iPosUnitNew = 0;
int         iTempUnitOld  = 0,iTempUnitNew = 0;
int         iPresUnitOld  = 0,iPresUnitNew = 0;
int         iUnitOld  = 0,iUnitNew = 0;
char 		u_cszPwd[20];
char*	u_pszLogoImagePath[] =		//fuxy, 2010-1-15 
{
	"LogoCN.bmp",
	"LogoEN.bmp"
};
//long     lunit=0;

static	char*	g_apszSpdUnitTransIDSys[] = {       //TYL 2015.05.15  For 0HT 619&61A   3 EJET
	  "SYS_HMI_SCREW_INJECT_NULL_MAXFLOW",
	  "SYS_HMI_SCREW_HOLDPRESSURE_NULL_MAXFLOW",
	  "SYS_HMI_SCREW2_INJECT_NULL_MAXFLOW",
	  "SYS_HMI_SCREW2_HOLDPRESSURE_NULL_MAXFLOW"
};

static	char*	g_apszInjeMoldID[] = {      //TYL 2015.05.15  For 0HT 619&61A   3 EJET
	  "MSET_SCRW_INJ_STEP1_FLOW",
	  "MSET_SCRW_INJ_STEP2_FLOW",
	  "MSET_SCRW_INJ_STEP3_FLOW",
	  "MSET_SCRW_INJ_STEP4_FLOW",
	  "MSET_SCRW_INJ_STEP5_FLOW",
	  "MSET_SCRW_INJ_STEP6_FLOW"
			};

static	char*	g_apszInjeMoldBID[] = {           //TYL TYL 2014.04.24 for 0HT  SPEED  MoldB
	  "MSETB_SCRW2_INJ_STEP1_FLOW",
	  "MSETB_SCRW2_INJ_STEP2_FLOW",
	  "MSETB_SCRW2_INJ_STEP3_FLOW",
	  "MSETB_SCRW2_INJ_STEP4_FLOW",
	  "MSETB_SCRW2_INJ_STEP5_FLOW",
	  "MSETB_SCRW2_INJ_STEP6_FLOW"
			};

static	char*	g_apszHoldMoldID[] = {      //TYL 2015.05.15  For 0HT 619&61A   3 EJET
	  "MSET_SCRW_HLDP_STEP1_FLOW",
	  "MSET_SCRW_HLDP_STEP2_FLOW",
	  "MSET_SCRW_HLDP_STEP3_FLOW",
	  "MSET_SCRW_HLDP_STEP4_FLOW",
	  "MSET_SCRW_HLDP_STEP5_FLOW"
			};
			
static	char*	g_apszHoldMoldBID[] = {      //TYL 2015.05.15  For 0HT 619&61A   3 EJET
	  "MSETB_SCRW2_HLDP_STEP1_FLOW",
	  "MSETB_SCRW2_HLDP_STEP2_FLOW",
	  "MSETB_SCRW2_HLDP_STEP3_FLOW",
	  "MSETB_SCRW2_HLDP_STEP4_FLOW",
	  "MSETB_SCRW2_HLDP_STEP5_FLOW"
			};

static	char*	g_apszPrsUnitTransIDMach[] = {           //TYL 2014.04.17 for 0HT Mach
	  "MACH_MLD_CLOSE_NULL_W0093",  
	  "MACH_MLD_CLOSE_NULL_W0094",
	  "MACH_MLD_CLOSE_SETUP_PRESSURE",
	  "MACH_MLD_OPEN_SETUP_PRESSURE",
	  "MACH_SHUT_SHUT_NULL_PRESSURE",
	  "MACH_SCRW_INJ_SETUP_PRESSURE",
	  "MACH_ACC_STORAGE_NULL_PRESSURE",
	  "MACH_SCRW_RCV_SETUP_PRESSURE",
	  "MACH_EJT_EJT_SETUP_PRESSURE",
	  "MACH_DOOR_DOOR_NULL_PRESSURE",
	  "MACH_DOOR_DOOR_SLOW_PRESSURE",
	  "MACH_OTHR_OTHR_NULL_W03C1",
	  "MACH_IJU_IJU_SETUP_PRESSURE",
	  "MACH_IJU_BWD_SLOW_PRESSURE"  
			};

static	char*	g_apszPrsUnitTransIDMachB[] = {           //TYL 2014.04.17 for 0HT MachB
	  "MACHB_SHUT2_SHUT_NULL_PRESSURE", 
	  "MACHB_SCRW2_INJ_SETUP_PRESSURE",
	  "MACHB_SCRW2_RCV_SETUP_PRESSURE",
	  "MACHB_EJT2_EJT_SETUP_PRESSURE",
	  "MACHB_IJU2_IJU_SETUP_PRESSURE",
	  "MACHB_IJU2_BWD_SLOW_PRESSURE"
			};
			
static	char*	g_apszPrsUnitTransID[] = {          //TYL 2014.11.05 for 0HT MoldSet
      "MSET_SCRW_INJ_STEP1_PRESSURE",   //InjeA
      "MSET_SCRW_INJ_STEP2_PRESSURE",
      "MSET_SCRW_INJ_STEP3_PRESSURE",
      "MSET_SCRW_INJ_STEP4_PRESSURE",
      "MSET_SCRW_INJ_STEP5_PRESSURE",
      "MSET_SCRW_INJ_STEP6_PRESSURE",
      "MSET_MLD_CLOSE_STEP1_PRESSURE",
      "MSET_MLD_CLOSE_STEP2_PRESSURE",
      "MSET_MLD_CLOSE_STEP3_PRESSURE",
      "MSET_MLD_CLOSE_PROTECT_PRESSURE",
      "MSET_MLD_CLOSE_CLAMPUP_PRESSURE",
      "MSET_MLD_OPEN_STEP1_PRESSURE",   //Open
      "MSET_MLD_OPEN_STEP2_PRESSURE",
      "MSET_MLD_OPEN_STEP3_PRESSURE",
      "MSET_MLD_OPEN_STEP4_PRESSURE",
      "MSET_MLD_OPEN_STEP5_PRESSURE",
      "MSET_SCRW_HLDP_STEP1_PRESSURE",  //HoldA
      "MSET_SCRW_HLDP_STEP2_PRESSURE",
      "MSET_SCRW_HLDP_STEP3_PRESSURE",
      "MSET_SCRW_HLDP_STEP4_PRESSURE",
      "MSET_SCRW_HLDP_STEP5_PRESSURE",
      "MSET_SCRW_CUSHION_NULL_PRESSURE",//HoldA Trans
      "MSET_SCRW_RCV_STEP1_BACKPRESSURE", //ChrgA BackPrs  
      "MSET_SCRW_RCV_STEP2_BACKPRESSURE",
      "MSET_SCRW_RCV_STEP3_BACKPRESSURE",
      "MSET_SCRW_RCV_STEP4_BACKPRESSURE",
      "MSET_SCRW_RCV_STEP5_BACKPRESSURE",
      "MSET_SCRW_DCMP_NULL_PRESSURE", 
      "MSET_SCRW_RCV_STEP1_PRESSURE",//ChrgA Prs
      "MSET_SCRW_RCV_STEP2_PRESSURE", 
      "MSET_SCRW_RCV_STEP3_PRESSURE",
      "MSET_SCRW_RCV_STEP4_PRESSURE",
      "MSET_SCRW_RCV_STEP5_PRESSURE",
      "MSET_EJT_FWD_STEP1_PRESSURE",   //EjetA
      "MSET_EJT_FWD_NULL_PRESSURE",
      "MSET_EJT_BWD_NULL_VIBRATEPRESSURE",
      "MSET_EJT_BWD_NULL_PRESSURE",
      "MSET_OTHR_OTHR_NULL_W0833",   //EjetC
      "MSET_OTHR_OTHR_NULL_W0836",
      "MSET_OTHR_OTHR_NULL_W0839",
      "MSET_OTHR_OTHR_NULL_W083C", 
      "MSET_CORA_IN_NULL_PRESSURE",  //CoreA
      "MSET_CORA_OUT_NULL_PRESSURE",
      "MSET_CORB_IN_NULL_PRESSURE",  //CoreB
      "MSET_CORB_OUT_NULL_PRESSURE",
      "MSET_CORC_IN_NULL_PRESSURE",  //CoreC
      "MSET_CORC_OUT_NULL_PRESSURE",
      "MSET_CORD_IN_NULL_PRESSURE",  //CoreD
      "MSET_CORD_OUT_NULL_PRESSURE",
      "MSET_CORE_IN_NULL_PRESSURE",  //CoreE
      "MSET_CORE_OUT_NULL_PRESSURE",
      "MSET_CORF_IN_NULL_PRESSURE",  //CoreF
      "MSET_CORF_OUT_NULL_PRESSURE",
      "MSET_IJU_FWD_NULL_PRESSURE",  //NozlA
      "MSET_IJU_FWD_SLOW_PRESSURE",
      "MSET_IJU_BWD_NULL_PRESSURE",
      "MSET_MLD_OPEN_DRAIN_PRESSURE"
			};

static	char*	g_apszPrsUnitTransIDMoldB[] = {           //TYL 2014.11.05 for 0HT MoldSetB
	  "MSETB_SCRW2_INJ_STEP1_PRESSURE",  //InjeB
      "MSETB_SCRW2_INJ_STEP2_PRESSURE",
      "MSETB_SCRW2_INJ_STEP3_PRESSURE",
      "MSETB_SCRW2_INJ_STEP4_PRESSURE",
      "MSETB_SCRW2_INJ_STEP5_PRESSURE",
      "MSETB_SCRW2_INJ_STEP6_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP1_PRESSURE",//HoldB
      "MSETB_SCRW2_HLDP_STEP2_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP3_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP4_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP5_PRESSURE",
      "MSETB_SCRW2_RCV_STEP1_BACKPRESSURE", //ChrgB BackPrs
      "MSETB_SCRW2_RCV_STEP2_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP3_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP4_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP5_BACKPRESSURE",
      "MSETB_SCRW2_DCMP_NULL_PRESSURE",
      "MSETB_SCRW2_RCV_STEP1_PRESSURE",//ChrgB Prs
      "MSETB_SCRW2_RCV_STEP2_PRESSURE", 
      "MSETB_SCRW2_RCV_STEP3_PRESSURE",
      "MSETB_SCRW2_RCV_STEP4_PRESSURE",
      "MSETB_SCRW2_RCV_STEP5_PRESSURE",
      "MSETB_EJT2_FWD_STEP1_PRESSURE",   //EjetB
      "MSETB_EJT2_FWD_NULL_PRESSURE",
      "MSETB_EJT2_BWD_VIBRATE_PRESSURE",
      "MSETB_EJT2_BWD_NULL_PRESSURE",
      "MSETB_LOCT2_LOCATE_NULL_PRESSURE",  //RoteB
      "MSETB_LOCT2_BWD_NULL_PRESSURE",
      "MSETB_IJU2_FWD_NULL_PRESSURE",  //NozlB
      "MSETB_IJU2_FWD_SLOW_PRESSURE",
      "MSETB_IJU2_BWD_NULL_PRESSURE" 
			};

static	char*	g_apszPrsUnitTransIDSys[] = {           //TYL 2014.04.17 for 0HT PressLimit
	  "SYSX_MOLD_OPEN_FAST_MAXPRESSURE",  
	  "SYSX_MOLD_CLOSE_PROTECT_MAXPRESSURE",
	  "SYSX_MOLD_OPEN_STEP1_MAXPRESSURE",
      //"MACH_OTHR_OTHR_NULL_W03B7",
	  "SYS_HMI_SCREW_INJECT_NULL_MAXFORCE",
	  "SYS_HMI_SCREW_HOLDPRESSURE_NULL_MAXFORCE",
	  "SYS_HMI_SCREW2_INJECT_NULL_MAXFORCE",
	  "SYS_HMI_SCREW2_HOLDPRESSURE_NULL_MAXFORCE",
	  "SYSX_SCREW_RECOVERY_NULL_MAXPRESSURE",
	  "SYSX_SCREW_DECOMP_NULL_MAXPRESSURE",
	  "SYSX_SCREW_RECOVERY_NULL_MAXBACKPRESSSURE",
	  "SYSX_SCREW2_RECOVERY_NULL_MAXPRESSURE",
	  "SYSX_SCREW2_DECOMP_NULL_MAXPRESSURE",
	  "SYSX_SCREW2_RECOVERY_NULL_MAXBACKPRESSSURE",
	  "SYS_HMI_EJECTOR_NULL_NULL_MAXFORCE",
	  "SYSX_COREPULLERD_COREPULLER_NULL_MAXPRESSURE",
	  "SYSX_INJUNIT_INJUNIT_NULL_MAXPRESSURE"
			};

static	char*	g_MSET_SetTempID[] = {       //zsq 2015-2-2 for 7HX TempUnitExchange (Temp SetVal&PID)
			"MSET_TMP1_HEATERON_CHL1_DEGREE",
			"MSET_TMP1_HEATERON_CHL2_DEGREE",
			"MSET_TMP1_HEATERON_CHL3_DEGREE",
			"MSET_TMP1_HEATERON_CHL4_DEGREE",
			"MSET_TMP1_HEATERON_CHL5_DEGREE",
			"MSET_TMP1_HEATERON_CHL6_DEGREE",
			"MSET_TMP1_HEATERON_CHL7_DEGREE",
			"MSET_TMP1_HEATERON_CHL8_DEGREE",
			"MSET_TMP1_HEATERON_CHL9_DEGREE",
			"MSET_TMP1_HEATERON_NULL_W07EB",
			"MSET_TMP1_HEATERON_NULL_W07EC",
			"MSET_TMP1_HEATERON_NULL_W07ED",
			"MSET_TMP1_HEATERON_NULL_W07EE",
			"MSET_TMP1_HEATERON_NULL_W07EF",
			"MSET_TMP1_HEATERON_NULL_W07F0",
			"MSET_TMP1_HEATERON_NULL_W07F1",
			"MSET_TMP1_HEATERON_NULL_W07F2",
			"MSET_TMP1_HEATERON_NULL_W07F3",
			"MSET_TMP1_HEATERON_NULL_W07E4",
			};
static	char*	g_MSET_SetTempOILTOTPID[] = {/* for 7AS DC*/
			"MSET_TMP1_HEATERON_CHL10_DEGREE",//第10段溫度設定值
			"MSET_OTHR_OTHR_NULL_SETTESTFLOW",//料溫溫度上限設定值
			"MACH_TMP1_HEATERON_WAIT_DEGREE",/*軟啟動過渡溫度*/
			};
static	char*	g_MACH_SetTempID[] = {      //zsq 2015-2-2 for 7HX TempUnitExchange
			"MACH_ALARM_ON_NULL_MAXOILTMPOFFSET",//油箱油溫上限
			"MACH_ALARM_ON_NULL_MINOILTMPOFFSET",//油箱油溫下限
			"MACH_ALARM_ON_NULL_TMPCTCHECKDEGREE",//電熱連續加溫檢查度數
			"MACH_TMPOIL_COOLERON_NULL_DEGREEOFFSET",//油溫高開冷卻器
			"MACH_TMPOIL_COOLEROFF_NULL_DEGREEOFFSET",//油溫低關冷卻器
			"MACH_TMP1_HEATEROFF_WARM_DEGREE",//保溫溫度
			"MACH_TMPMLD_HEATEROFF_WARM_DEGREE",//模溫保溫溫度
			"MACH_TMPMLD_HEATERON_WAIT_DEGREE",//中間過渡溫度
			};
static	char*	g_MACH_SetTempID_NoOil[] = {
			"MACH_ALARM_ON_NULL_TMPCTCHECKDEGREE",//電熱連續加溫檢查度數
			"MACH_TMP1_HEATEROFF_WARM_DEGREE",//保溫溫度
			"MACH_TMPMLD_HEATEROFF_WARM_DEGREE",//模溫保溫溫度
			"MACH_TMPMLD_HEATERON_WAIT_DEGREE",//中間過渡溫度
			};
static	char*	g_MACH_TempUpLowID[] = {       //zsq 2015-2-2 for 7HX TempUnitExchange
			"MACH_ALARM_ON_NULL_MAXTMPOFFSET",//料管溫度上限值
			"MACH_ALARM_ON_NULL_MINTMPOFFSET",//料管溫度下限值
			"MACH_TMP1_COOLERON_NULL_DEGREEOFFSET",//料管冷卻開偏差值
			"MACH_TMP1_COOLEROFF_NULL_DEGREEOFFSET",//料管冷卻關偏差值
			"MACH_ALARM_ON_NULL_MINMOLDTMPOFFSET",//模溫下限警報值
			"MACH_ALARM_ON_NULL_MAXMOLDTMPOFFSET",//模溫上限警報值
			};
static	char*	g_MSET_SetMoldTempID[] = {       //zsq 2015-3-3 for 7HX TempUnitExchange
			"TEMP2_TMP2_HEATERON_STEP1_DEGREE", //模溫設定值
			"TEMP2_TMP2_HEATERON_STEP2_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP3_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP4_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP5_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP6_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP7_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP8_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP9_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP10_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP11_DEGREE",
			"TEMP2_TMP2_HEATERON_STEP12_DEGREE",
		};
		
static	char*	g_MACH_TempWaterValveID[] = {
			"MACH_OTHR_OTHR_NULL_SPECIALDATA33",	/*料溫水閥開啟溫度*/
			"MACH_OTHR_OTHR_NULL_SPECIALDATA34",	/*料溫水閥關閉溫度*/
		};

static	char*	g_apszSetTempID3[] = {       
			"MACH_OTHR_OTHR_NULL_W03AC"
			};   

static	char*	g_MACH_TempMiddleTempID[] = {
			"MACH_TMP1_HEATERON_WAIT_DEGREE",	/*軟啟動過渡溫度*/
		};
			
static	char*	g_apszPosTransID[] = {       /*zyh 2016-8-9 for 7HD & 7AS*/
			"MSET_MLD_OPEN_STEP1_POSITION",
			"MSET_MLD_OPEN_STEP2_POSITION",
			"MSET_MLD_OPEN_STEP3_POSITION",
			"MSET_MLD_OPEN_STEP4_POSITION",
			"MSET_MLD_OPEN_STEP5_POSITION",
			"MSET_MLD_OPEN_NULL_LINKPOSITION",
			"MSET_MLD_OPEN_END_OILPLUSPOSITIONOFFSET",
			"MSET_MLD_OPEN_END_OILMINUSPOSITIONOFFSET",
			"MACH_COR_COR_NULL_EFFECTPOSITION",
			"MSET_MLD_CLOSE_STEP1_POSITION",
			"MSET_MLD_CLOSE_STEP2_POSITION",
			"MSET_MLD_CLOSE_STEP3_POSITION",
			"MSET_MLD_CLOSE_NULL_INJECTMOLDINGOPTION",
			"MSET_MLD_CLOSE_PROTECT_POSITION",
			"MSET_MLD_CLOSE_CLAMPUP_POSITION",
			"MSET_SCRW_INJ_NULL_W06D6",
			"MSET_GATE1_OPEN_NULL_WHILEINJECTPOSITION",
			"MSET_GATE2_OPEN_NULL_WHILEINJECTPOSITION",
			"MSET_GATE3_OPEN_NULL_WHILEINJECTPOSITION",
			"MSET_GATE4_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE5_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE1_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE2_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE3_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE4_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE5_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE6_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE7_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE8_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE9_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE10_OPEN_NULL_WHILEINJECTPOSITION",
            "MSET_GATE6_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE7_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE8_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE9_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_GATE10_CLOSE_NULL_WHILEINJECTPOSITION",
            "MSET_SCRW_INJ_STEP1_POSITION",
            "MSET_SCRW_INJ_STEP2_POSITION",
            "MSET_SCRW_INJ_STEP3_POSITION",
            "MSET_SCRW_INJ_STEP4_POSITION",
            "MSET_SCRW_INJ_STEP5_POSITION",
            "MSET_SCRW_INJ_STEP6_POSITION",
            "MSET_SCRW_CUSHION_NULL_POSITION",
            "MSET_SCRW_INJ_NULL_WHILESEALPOSITION",
            "MSET_SCRW_RCV_STEP1_POSITION",
            "MSET_SCRW_RCV_STEP2_POSITION",
            "MSET_SCRW_RCV_STEP3_POSITION",
            "MSET_SCRW_RCV_STEP4_POSITION",
            "MSET_SCRW_RCV_STEP5_POSITION",
            "MSET_SCRW_DCMP_NULL_POSITION",
            "MSET_SCRW_DCMP_NULL_POSITIONOFFSET",
            "MSET_SCRW_DCMP_NULL_BEFORERECOVERYOFFSET",
            "MSET_SCRW_RCV_NULL_FLOWMOLDINGPOSITION",
			"MSET_EJT_FWD_STEP1_POSITION",
            "MSET_EJT_FWD_NULL_POSITION",
            "MSET_EJT_BWD_STEP1_POSITION",
            "MSET_EJT_BWD_NULL_POSITION",
            "MSET_EJT_EJT_NULL_VIBRATEPOSITION",
            "MSET_BLST1_ON_START_POSITION",
            "MSET_BLST2_ON_START_POSITION",
            "MSET_BLST3_ON_START_POSITION",
            "MSET_BLST4_ON_START_POSITION",
            "MSET_BLST5_ON_START_POSITION",
            "MSET_BLST6_ON_START_POSITION",
            "MACH_EJT_EJT_NULL_EFFECTPOSITION",
            "MACH_EJT_BWD_NULL_MAXPOSITION",
  			"MSET_CORA_IN_NULL_SETMOLDPOSITION",
  			"MSET_CORA_IN_NULL_POSITION",
  			"MSET_CORA_IN_SLOW_POSITION",
  			"MSET_CORA_IN_NULL_OILPLUSPOSITIONOFFSET",
  			"MSET_CORA_IN_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",
            "MSET_CORA_OUT_SLOW_POSITION",
            "MSET_CORA_OUT_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORA_OUT_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORB_IN_NULL_SETMOLDPOSITION",
            "MSET_CORB_IN_NULL_POSITION",
            "MSET_CORB_IN_SLOW_POSITION",
            "MSET_CORB_IN_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORB_IN_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",
            "MSET_CORB_OUT_SLOW_POSITION",
            "MSET_CORB_OUT_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORB_OUT_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORC_IN_NULL_SETMOLDPOSITION",
            "MSET_CORC_IN_NULL_POSITION",
            "MSET_CORC_IN_SLOW_POSITION",
            "MSET_CORC_IN_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORC_IN_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",
            "MSET_CORC_OUT_SLOW_POSITION",
            "MSET_CORC_OUT_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORC_OUT_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORD_IN_NULL_SETMOLDPOSITION",
            "MSET_CORD_IN_NULL_POSITION",
            "MSET_CORD_IN_SLOW_POSITION",
            "MSET_CORD_IN_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORD_IN_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",
            "MSET_CORD_OUT_SLOW_POSITION",
            "MSET_CORD_OUT_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORD_OUT_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORE_IN_NULL_SETMOLDPOSITION",
            "MSET_CORE_IN_NULL_POSITION",
            "MSET_CORE_IN_SLOW_POSITION",
            "MSET_CORE_IN_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORE_IN_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",
            "MSET_CORE_OUT_SLOW_POSITION",
            "MSET_CORE_OUT_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORE_OUT_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORF_IN_NULL_SETMOLDPOSITION",
            "MSET_CORF_IN_NULL_POSITION",
            "MSET_CORF_IN_SLOW_POSITION",
            "MSET_CORF_IN_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORF_IN_NULL_OILMINUSPOSITIONOFFSET",
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",
            "MSET_CORF_OUT_SLOW_POSITION",
            "MSET_CORF_OUT_NULL_OILPLUSPOSITIONOFFSET",
            "MSET_CORF_OUT_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_IJU_FWD_SLOW_POSITION",
            "MSET_IJU_FWD_NULL_POSITION",
			"MSET_IJU_BWD_NULL_POSITION",
			"MACH_IJU_BWD_SLOW_POSITION",
            "MSET_MDH_MDH_NULL_POSITION",
            "MSET_SCRW_INJ_NULL_BEFOREINJUFWDPOSITIONOFFSET",
            "MSET_PRODUCT_PRODUCT_NULL_W0700",/*機械手中途下降位置*/
            "SYS_HMI_NULL_NULL_MINROTARYLINKPOSN",/*開模聯動位置下限*/
            "SYSX_MOLD_OPEN_STEP1_MAXPOSITION",/*開模一慢位置上限*/
            "MONI_SCRW_INJ_END_DELTAVALUE2POSITION",
            "MONI_ALARM_ON_NULL_MAXINJECTEND2POSITION",
            "MONI_SCRW_INJ_END_LAST2POSITION",
            "MONI_ALARM_ON_NULL_MININJECTEND2POSITION",
            "MONI_SCRW_CUSHION_NULL_DELTAVALUEPOSITION",
            "MONI_ALARM_ON_NULL_MAXCUSHIONPOSITION",
            "MONI_SCRW_CUSHION_NULL_LASTPOSITION",
            "MONI_ALARM_ON_NULL_MINCUSHIONPOSITION",
            "MONI_SCRW_INJ_END_DELTAVALUEPOSITION",
            "MONI_ALARM_ON_NULL_MAXINJECTENDPOSITION",
            "MONI_SCRW_INJ_END_LASTPOSITION",
            "MONI_ALARM_ON_NULL_MININJECTENDPOSITION",
            "MONI_SCRW_INJ_NULL_DELTAVALUEPOSITION",
            "MONI_ALARM_ON_NULL_MAXSTARTINJECTPOSITION",
            "MONI_SCRW_INJ_START_LASTPOSITION",
            "MONI_ALARM_ON_NULL_MINSTARTINJECTPOSITION",
			}; 
			
static	char*	g_apsz12ValveID[] = {/*for  ZT*/
			"MSET_UEJT_BWD_NULL_FLOW",
            "MSET_CORA_CORA_NULL_MIXOPTION",
            "MSET_ROBOT_ROBOT_NULL_MOLDOPENOPTION",
            "MSET_OTHR_OTHR_NULL_OPTION",
            "MSET_MLD_CLOSE_NULL_W04B4",
			};
			
static	char*	g_apszPosTransID_DC[] = {/*zyh 2017-3-21 for 7AS DC */
			"MSETB_SCRW2_INJ_STEP1_POSITION",//InjeB
			"MSETB_SCRW2_INJ_STEP2_POSITION",
			"MSETB_SCRW2_INJ_STEP3_POSITION",
			"MSETB_SCRW2_INJ_STEP4_POSITION",
			"MSETB_SCRW2_INJ_STEP5_POSITION",
			"MSETB_SCRW2_CUSHION_NULL_POSITION",
			"MSETB_SCRW2_RCV_STEP1_POSITION",//ChrgB
			"MSETB_SCRW2_RCV_STEP2_POSITION",
			"MSETB_SCRW2_RCV_STEP3_POSITION",
			"MSETB_SCRW2_RCV_STEP4_POSITION",
			"MSETB_SCRW2_RCV_STEP5_POSITION",
			"MSETB_SCRW2_DCMP_NULL_POSITIONOFFSET",
			"MSETB_SCRW2_DCMP_NULL_BEFORERECOVERYPOSITION",
			"MSETB_SCRW2_RCV_NULL_FLOWMOLDINGPOSITION",
			"MONIB_SCRW2_INJ_END_LASTPOSITION",
			"MSETB_EJT2_FWD_STEP1_POSITION",//EjectB
			"MSETB_EJT2_FWD_NULL_POSITION",
			"MSETB_EJT2_BWD_STEP1_POSITION",
			"MSETB_EJT2_BWD_NULL_POSITION",
			"MACHB_EJT2_EJT_NULL_EFFECTPOSITION",
			"MACHB_EJT2_BWD_NULL_MAXPOSITION",
			"MSETB_IJU2_FWD_SLOW_POSITION",//NozlB
			"MSETB_IJU2_FWD_NULL_POSITION",
			"MACHB_IJU2_BWD_SLOW_POSITION",
			"MSETB_IJU2_BWD_NULL_POSITION",
			"MONIB_SCRW2_INJ_END_DELTAVALUE2POSITION",
			"MONIB_ALARM_ON_NULL_MAXINJECT2END2POSITION",
			"MONIB_SCRW2_INJ_END_LAST2POSITION",
			"MONIB_ALARM_ON_NULL_MININJECT2END2POSITION",
			"MONIB_SCRW2_CUSHION_NULL_DELTAVALUEPOSITION",
			"MONIB_ALARM_ON_NULL_MAXCUSHION2POSITION",
			"MONIB_SCRW2_CUSHION_NULL_LASTPOSITION",
			"MONIB_ALARM_ON_NULL_MINCUSHION2POSITION",
			"MONIB_SCRW2_INJ_END_DELTAVALUEPOSITION",
			"MONIB_ALARM_ON_NULL_MAXINJECT2ENDPOSITION",
			"MONIB_SCRW2_INJ_END_LASTPOSITION",
			"MONIB_ALARM_ON_NULL_MININJECT2ENDPOSITION",
			"MONIB_SCRW2_INJ_NULL_DELTAVALUEPOSITION",
			"MONIB_ALARM_ON_NULL_MAXSTARTINJECT2POSITION",
			"MONIB_SCRW2_INJ_NULL_LASTPOSITION",
			"MONIB_ALARM_ON_NULL_MINSTARTINJECT2POSITION",
		};
			
static	char*	g_apszPrsTransID[] = {       /*zyh 2016-8-9 for 7HD & 7AS*/
      "MSET_MLD_CLOSE_STEP1_PRESSURE",  //Close
      "MSET_MLD_CLOSE_STEP2_PRESSURE",
      "MSET_MLD_CLOSE_STEP3_PRESSURE",
      "MSET_MLD_CLOSE_PROTECT_PRESSURE",
      "MSET_MLD_CLOSE_CLAMPUP_PRESSURE",
      "MSET_MLD_OPEN_STEP1_PRESSURE",   //Open
      "MSET_MLD_OPEN_STEP2_PRESSURE",
      "MSET_MLD_OPEN_STEP3_PRESSURE",
      "MSET_MLD_OPEN_STEP4_PRESSURE",
      "MSET_MLD_OPEN_STEP5_PRESSURE",
      "MSET_MLD_OPEN_DRAIN_PRESSURE",
      "MACH_MLD_CLOSE_SETUP_PRESSURE",
      "MACH_MLD_OPEN_SETUP_PRESSURE",
      "SYSX_MOLD_OPEN_FAST_MAXPRESSURE",
      "SYSX_MOLD_CLOSE_PROTECT_MAXPRESSURE",
      "SYSX_MOLD_OPEN_STEP1_MAXPRESSURE",
      "MSET_SCRW_INJ_STEP1_PRESSURE",   //InjeA
      "MSET_SCRW_INJ_STEP2_PRESSURE",
      "MSET_SCRW_INJ_STEP3_PRESSURE",
      "MSET_SCRW_INJ_STEP4_PRESSURE",
      "MSET_SCRW_INJ_STEP5_PRESSURE",
      "MSET_SCRW_INJ_STEP6_PRESSURE",
      "MSET_SCRW_HLDP_STEP1_PRESSURE",  //HoldA
      "MSET_SCRW_HLDP_STEP2_PRESSURE",
      "MSET_SCRW_HLDP_STEP3_PRESSURE",
      "MSET_SCRW_HLDP_STEP4_PRESSURE",
      "MSET_SCRW_HLDP_STEP5_PRESSURE",
      "MSET_SCRW_CUSHION_NULL_PRESSURE",//HoldA Trans
      "MACH_SHUT_SHUT_NULL_PRESSURE",/*噴嘴動作壓力*/
      "MACH_SCRW_INJ_SETUP_PRESSURE",
      "MACH_ACC_STORAGE_NULL_PRESSURE",
      "SYS_HMI_SCREW_INJECT_NULL_MAXFORCE",
	  "SYS_HMI_SCREW_HOLDPRESSURE_NULL_MAXFORCE",
      "MSET_SCRW_RCV_STEP1_BACKPRESSURE", //ChrgA BackPrs  
      "MSET_SCRW_RCV_STEP2_BACKPRESSURE",
      "MSET_SCRW_RCV_STEP3_BACKPRESSURE",
      "MSET_SCRW_RCV_STEP4_BACKPRESSURE",
      "MSET_SCRW_RCV_STEP5_BACKPRESSURE",
      "MSET_SCRW_DCMP_NULL_PRESSURE",   
      "MSET_SCRW_RCV_STEP1_PRESSURE",//ChrgA Prs
      "MSET_SCRW_RCV_STEP2_PRESSURE", 
      "MSET_SCRW_RCV_STEP3_PRESSURE",
      "MSET_SCRW_RCV_STEP4_PRESSURE",
      "MSET_SCRW_RCV_STEP5_PRESSURE",
      "MACH_SCRW_RCV_SETUP_PRESSURE",
      "SYSX_SCREW_RECOVERY_NULL_MAXPRESSURE",
	  "SYSX_SCREW_DECOMP_NULL_MAXPRESSURE",
	  "SYSX_SCREW_RECOVERY_NULL_MAXBACKPRESSSURE", 
      "MSET_SCRW_RCV_NULL_FLOWMOLDINGPRESSURE",
      "MSET_EJT_FWD_STEP1_PRESSURE",   //EjetA
      "MSET_EJT_FWD_NULL_PRESSURE",
      "MSET_EJT_BWD_NULL_VIBRATEPRESSURE",
      "MSET_EJT_BWD_NULL_PRESSURE",
      "MACH_EJT_EJT_SETUP_PRESSURE",
	  "MACH_DOOR_DOOR_NULL_PRESSURE",
	  "MACH_DOOR_DOOR_SLOW_PRESSURE",     
      "MSET_CORA_IN_NULL_PRESSURE",  //CoreA
      "MSET_CORA_OUT_NULL_PRESSURE",
      "MSET_CORB_IN_NULL_PRESSURE",  //CoreB
      "MSET_CORB_OUT_NULL_PRESSURE",
      "MSET_CORC_IN_NULL_PRESSURE",  //CoreC
      "MSET_CORC_OUT_NULL_PRESSURE",
      "MSET_CORD_IN_NULL_PRESSURE",  //CoreD
      "MSET_CORD_OUT_NULL_PRESSURE",
      "MSET_CORE_IN_NULL_PRESSURE",  //CoreE
      "MSET_CORE_OUT_NULL_PRESSURE",
      "MSET_CORF_IN_NULL_PRESSURE",  //CoreF
      "MSET_CORF_OUT_NULL_PRESSURE",
      "SYSX_COREPULLERD_COREPULLER_NULL_MAXPRESSURE",
      "MSET_IJU_FWD_NULL_PRESSURE",  //NozlA
      "MSET_IJU_FWD_SLOW_PRESSURE",
      "MSET_IJU_BWD_NULL_PRESSURE",
      "MACH_IJU_BWD_SLOW_PRESSURE",
      "MACH_IJU_IJU_SETUP_PRESSURE",
      "SYSX_INJUNIT_INJUNIT_NULL_MAXPRESSURE",
      "MACH_TMPOIL_HEATERON_NULL_PREHEATPRESSURE",/*油溫預熱壓力*/
      "MSET_MDH_MDH_FAST_PRESSURE",//adj in pre
      "SYSX_MOLDHEIGHT_MOLDHEIGHT_NULL_MAXPRESSURE",/*調模壓力上限*/
      "SYS_HMI_EJECTOR_NULL_NULL_MAXFORCE",
      "MONI_SCRW_CUSHION_NULL_DELTAVALUEPRESSURE",
      "MONI_ALARM_ON_NULL_MAXCUSHIONPRESSURE",
      "MONI_SCRW_CUSHION_NULL_LASTPRESSURE",
      "MONI_ALARM_ON_NULL_MINCUSHIONPRESSURE",
			};
static	char*	g_apszPrsTransID_DC[] = {/*zyh 2017-3-21 for 7AS DC */
      "MSETB_SCRW2_INJ_STEP1_PRESSURE",  //InjeB
      "MSETB_SCRW2_INJ_STEP2_PRESSURE",
      "MSETB_SCRW2_INJ_STEP3_PRESSURE",
      "MSETB_SCRW2_INJ_STEP4_PRESSURE",
      "MSETB_SCRW2_INJ_STEP5_PRESSURE",
      "MSETB_SCRW2_INJ_STEP6_PRESSURE",
      "MSETB_SCRW2_CUSHION_NULL_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP1_PRESSURE",//HoldB
      "MSETB_SCRW2_HLDP_STEP2_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP3_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP4_PRESSURE",
      "MSETB_SCRW2_HLDP_STEP5_PRESSURE",
      "MACHB_SCRW2_INJ_SETUP_PRESSURE",//InjeBP
      "SYS_HMI_SCREW2_INJECT_NULL_MAXFORCE",//InjeBS
      "SYS_HMI_SCREW2_HOLDPRESSURE_NULL_MAXFORCE",
      "MSETB_SCRW2_RCV_STEP1_PRESSURE",//ChrgB
      "MSETB_SCRW2_RCV_STEP2_PRESSURE",
      "MSETB_SCRW2_RCV_STEP3_PRESSURE",
      "MSETB_SCRW2_RCV_STEP4_PRESSURE",
      "MSETB_SCRW2_RCV_STEP5_PRESSURE",
      "MSETB_SCRW2_DCMP_NULL_PRESSURE",
      "MSETB_SCRW2_RCV_STEP1_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP2_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP3_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP4_BACKPRESSURE",
      "MSETB_SCRW2_RCV_STEP5_BACKPRESSURE",
      "MACHB_SCRW2_RCV_SETUP_PRESSURE",//ChrgBP
      "SYSX_SCREW2_RECOVERY_NULL_MAXPRESSURE",//ChrgBS
      "SYSX_SCREW2_DECOMP_NULL_MAXPRESSURE",
      "SYSX_SCREW2_RECOVERY_NULL_MAXBACKPRESSSURE",
      "MSETB_RMLD_ROTA_NULL_PRESSURE",//Rote
      "MSETB_RMLD_ROTA_SLOW_PRESSURE",
      "MSETB_RMLD_BWD_NULL_PRESSURE",
	  "MSETB_RMLD_BWD_SLOW_PRESSURE",
      "MSETB_SCRW2_DCMP_NULL_W13B0",
      "MSETB_SCRW2_DCMP_NULL_W13B2",
      "MSETB_LOCT2_LOCATE_NULL_PRESSURE",
      "MSETB_LOCT2_BWD_NULL_PRESSURE",
      "MSETB_EJT2_FWD_STEP1_PRESSURE",//EjectB
      "MSETB_EJT2_FWD_NULL_PRESSURE",
      "MSETB_EJT2_BWD_VIBRATE_PRESSURE",
      "MSETB_EJT2_BWD_NULL_PRESSURE",
      "MACHB_EJT2_EJT_SETUP_PRESSURE",//EjectBP
      "MSETB_IJU2_FWD_NULL_PRESSURE",//NozlB
      "MSETB_IJU2_FWD_SLOW_PRESSURE",
      "MSETB_IJU2_BWD_NULL_PRESSURE",
      "MACHB_IJU2_BWD_SLOW_PRESSURE",
      "MACHB_IJU2_IJU_SETUP_PRESSURE",//NozlBP
			};
		
/*cjlee ↓*/
CtmWnd*   pwndBtn_LGChoose[20] = {NULL}; // 語言選擇 選擇鈕
CtmWnd*   pwndBtn_LGStr[20] = {NULL}; // 語言選擇 文字
char* LGChoose_String[] = // 語言選擇 選擇鈕 名稱
{
	"Choose_LG0",
	"Choose_LG1",
	"Choose_LG2",
	"Choose_LG3",
};
char* LGBtn_String[] = // 語言選擇 文字 名稱
{
	"String_LG0",
	"String_LG1",
	"String_LG2",
	"String_LG3",
};
char* LG_String[] = // 語言選擇 文字
{
	"DLG_SYSTEM_LANGUAGE_CHINESE",  // 中文
	"DLG_SYSTEM_LANGUAGE_ENGLISH",  // English
	"DLG_SYSTEM_SIMCHINESE",  // 簡體中文
	"DLG_SYSTEM_LANGUAGE_TURKISH",  // Trukish
	"DLG_SYSTEM_LANGUAGE_SPANISH",  // Spanish
	"DLG_SYSTEM_LANGUAGE_GREECE",  // Greece
	"DLG_SYSTEM_LANGUAGE_PORTUGUESE",  // Protuguese
	"DLG_SYSTEM_IRANIAN",  // Iranian
	"DLG_SYSTEM_RUSSIAN",  // Russian
	"DLG_SYSTEM_JAPANESE",  // Japan
	"DLG_SYSTEM_KOREA", // Korea
	"DLG_SYSTEM_VIETNAM", // Vietnam
};
int LG_Page=0;
char* ChooseBox_ImgPath[] = //  選擇 顯示圖片
{
	"res_tm640/pic/picker/Choose_No.bmp",
	"res_tm640/pic/picker/Choose_OK.bmp",
};
char* Beep_ImgPath[] = //  按鍵音 顯示圖片
{
	"res_tm640/pic/Beep_ON.bmp",
	"res_tm640/pic/Beep_OFF.bmp",
};
/*cjlee ↑*/

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
    long long       PartYear;
    BGC TempBgc     = RGB2PIXEL(49, 89, 114);
    
	//	Get the iNet func
	u_biNet = CtmConfig::GetInstance()->GetINETFlag();
	u_bKeyLock= CtmConfig::GetInstance()->GetKeyLockFlag();
	
	OnCreateCommon(pwndSender);

	pwndSelectLanguage = pwndSender->FindControlFromName("SelecteditLANGUAGE");
	
	pwndStaticLogOut = pwndSender->FindControlFromName("StaticSLEEPLOGOUT");
	pwndEditLogOut   = pwndSender->FindControlFromName("editCONFIRMSLEEPLOGOUT");

  	pwndeditLCDPOWERON = pwndSender->FindControlFromName("editLCDPOWERON"); //ZHBA 2010-12-9 TEST
	pwndMOTORONTIME		= pwndSender->FindControlFromName("editMOTORONTIME");  
  	pwndStaticMOTORONTIME= pwndSender->FindControlFromName("StaticVW_MONI1_MACHRUNTIME");
  	pwndStaticMTRTIMEUNIT= pwndSender->FindControlFromName("StaticMACHRUNTIME_UNIT");
  	pwndStaticSampling	= pwndSender->FindControlFromName("StaticSAMPLE");	//2013-4-22 
  	pwndEditSampling		= pwndSender->FindControlFromName("editSAMPLE");
  	pwndeditPUSHKEYOIL		= pwndSender->FindControlFromName("editPUSHKEYOIL");
  	pwndStaticPUSHKEYOIL	= pwndSender->FindControlFromName("StaticPUSHKEYOIL");
  	pwndeditPOSIUNIT		= pwndSender->FindControlFromName("editPOSIUNITOPT");
  	pwndeditTEMPUNIT		= pwndSender->FindControlFromName("editTEMPUNITOPT");
  	pwndeditPRESUNIT		= pwndSender->FindControlFromName("editPRSUNITOPT");
  	
  	pwndSYST_BRITISHUNIT	= pwndSender->FindControlFromName("editSYST_BRITISHUNIT");/*zyh 2016-9-13 海達英制轉換使用一個開關*/
  
  	pwndeditUNITTRANSOPT	= pwndSender->FindControlFromName("editUNITTRANSOPT");
  	pwndeditINJEMAXDPEED	= pwndSender->FindControlFromName("editINJEMAXDPEED");
  	pwndeditHMITYPE			= pwndSender->FindControlFromName("editHMITYPE");
  	pwndSelectZERO1			= pwndSender->FindControlFromName("SelectZERO1");
  	
  	pwndImgBgLight			= pwndSender->FindControlFromName("ImgBgLight"); // 背光數值
  	pwndImgBgLightBar		= pwndSender->FindControlFromName("ImgBgLightBar"); // 背光數值條
  	
  	pwndBtnConfirm  = pwndSender->FindControlFromName("BtnConfirm"); // 確認Btn cjlee 2019/9/11 上午 10:09:48
  	pwndBtn_Beep  = pwndSender->FindControlFromName("Btn_Beep"); // 按鍵音 Btn cjlee 2019/9/12 下午 03:50:39
		// 取得指標 語言選擇 選擇鈕
		for(int i = 0; i < sizeof(LGChoose_String)/sizeof(LGChoose_String[0]); i++ )
		{
			pwndBtn_LGChoose[i] = pwndSender->FindControlFromName(LGChoose_String[i]); // 語言選擇 選擇鈕
		} 
		// 取得指標 語言選擇 文字
		for(int i = 0; i < sizeof(LGBtn_String)/sizeof(LGBtn_String[0]); i++ )
		{
			pwndBtn_LGStr[i] = pwndSender->FindControlFromName(LGBtn_String[i]); // 語言選擇 文字
		} 
		
	
  	m_b7TR 		= g_pExTable->GetTableValue(0,"D_7TR");
  	//b_PushKey  	= g_pExTable->GetTableValue(0,"D_PUSHKY");	
  	if(g_pExTable->GetTableValue(0,"D_PUSHKY") || g_pExTable->GetTableValue(0,"D_7TR") || g_pExTable->GetTableValue(0,"D_7AS"))
		b_PushKey  = TRUE;

	b_7LG 		= g_pExTable->GetTableValue(0,"D_7LG");
	b_7HDUnit	= g_pExTable->GetTableValue(0,"D_7HDUNIT");
	b_7ZS 		= g_pExTable->GetTableValue(0,"D_7ZS");
	b_7YL		= g_pExTable->GetTableValue(0,"D_7YL");
	b_7ASDCUnit = g_pExTable->GetTableValue(0,"D_7ASDCUN");
	b_7ASUnit 	= g_pExTable->GetTableValue(0,"D_7ASUNIT");
	b_7ZTUnit 	= g_pExTable->GetTableValue(0,"D_7ZTUNIT");
	b_7AS	 	= g_pExTable->GetTableValue(0,"D_7AS");
	b_7ZF		= g_pExTable->GetTableValue(0,"D_7ZF");
	b_7LN		= g_pExTable->GetTableValue(0,"D_7LN");
	b_AutoTime		= g_pExTable->GetTableValue(0,"D_AUTOTIME");

	if((pwndStaticMessage = pwndSender->FindControlFromName("StaticMESSAGE")) != NULL)
	    pwndStaticMessage->SetPropValueT("bgc", TempBgc);

    pwndEditMonth  = pwndSender->FindControlFromName("editMONTH");
    pwndEditYear   = pwndSender->FindControlFromName("editYEAR");
    pwndEditDay    = pwndSender->FindControlFromName("editDAY");
    pwndEditHour   = pwndSender->FindControlFromName("editHOUR");
    pwndEditMinute = pwndSender->FindControlFromName("editMINUTE");
    pwndEditSecond = pwndSender->FindControlFromName("editSECOND");
    pwndEditConfirm= pwndSender->FindControlFromName("editCONFIRM");
   
    pwndEditIP0  = pwndSender->FindControlFromName("editIP_0");
    pwndEditIP1  = pwndSender->FindControlFromName("editIP_1");
    pwndEditIP2  = pwndSender->FindControlFromName("editIP_2");
    pwndEditIP3  = pwndSender->FindControlFromName("editIP_3");
    
    pwndStaticVW_ACT_PRS      =  pwndSender->FindControlFromName("StaticVW_ACT_PRS");
	
    pwndStaticUNIT1  = pwndSender->FindControlFromName("StaticEFFECT_UNITTRANSFER1");
    pwndStaticUNIT2  = pwndSender->FindControlFromName("StaticEFFECT_UNITTRANSFER2");
    
    pwndeditOPENDAY  = pwndSender->FindControlFromName("editOPENDAY");  /*zyh  2014/2/13 for 7NG*/
    pwndeditOPENHOUR  = pwndSender->FindControlFromName("editOPENHOUR");
    pwndeditOPENMINUTE  = pwndSender->FindControlFromName("editOPENMINUTE");
    pwndeditTIME_CLEAN  = pwndSender->FindControlFromName("editTIME_CLEAN");
    
    pwndeditAUTOTIMEDAY  = pwndSender->FindControlFromName("editAUTOTIMEDAY");
    pwndeditAUTOTIMEHOUR  = pwndSender->FindControlFromName("editAUTOTIMEHOUR");
    pwndeditAUTOTIMEMINUTE  = pwndSender->FindControlFromName("editAUTOTIMEMINUTE");
    
    pwndeditMOTORON_HOUR  	= pwndSender->FindControlFromName("editMOTORON_HOUR");
    pwndeditMOTORON_MINUTE  = pwndSender->FindControlFromName("editMOTORON_MINUTE");
    pwndeditHEATERON_HOUR  	= pwndSender->FindControlFromName("editHEATERON_HOUR");
    pwndeditHEATERON_MIN 	= pwndSender->FindControlFromName("editHEATERON_MINUTE");
    
    pwndPassWord		= pwndSender->FindControlFromName("editPassWord");
   
   	if(m_b7TR)
   	{
	    if(!(g_pExTable->GetTableValue(0,"D_INCH")))
	    {
			if(pwndStaticUNIT2 != NULL)
			{
				pwndStaticUNIT2->SetPropValueT("visible",(double)0); 		
				pwndStaticUNIT2->Update();    	    	
			}
	    }
	    if(g_pExTable->GetTableValue(0,"D_MTRTIME") || g_pExTable->GetTableValue(0,"D_7TRMTRTIME"))
	    	GetACTTime(pwndMOTORONTIME);
	    else
	    {
	       	SetVisible(pwndStaticMOTORONTIME, FALSE, 0x30);
	       	SetVisible(pwndStaticMTRTIMEUNIT, FALSE, 0x30);
	       	SetVisible(pwndMOTORONTIME, FALSE, 0x30);
	    }
		if(g_pExTable->GetTableValue(0,"D_STSPFUN"))
		{			
			GetHour(pwndeditMOTORON_HOUR);//2017-4-20 
			GetMinute(pwndeditMOTORON_MINUTE);
			GetHour(pwndeditHEATERON_HOUR);
			GetMinute(pwndeditHEATERON_MIN);
		}
	    
	}
    
    if((!m_b7TR) &&(!g_pExTable->GetTableValue(0,"D_SELSAMPLE"))&&(!g_pExTable->GetTableValue(0,"D_7GLSTYLE")))	//2013-4-22 
    {
    	SetVisible(pwndStaticSampling, FALSE, 0x30);
    	SetVisible(pwndEditSampling, FALSE, 0x30);
    }
    
    pwndEditConfirmNet  = pwndSender->FindControlFromName("editCONFIRMNET");
    pwndEditConfirmMac  = pwndSender->FindControlFromName("editCONFIRMMAC");
    
//    pwndEditUNITTRANSFER=pwndSender->FindControlFromName("editUNITTRANSFER");
//    lunit =GetDBValue("MCONF_HMI_NULL_NULL_UNITTRANSFER").lValue;
    GetSpecialControlNum(pwndSender, EDIT_IP, "CtmEditX1", pwndLocalIP);
    GetSpecialControlNum(pwndSender, EDIT_MAC, "CtmEditX2", pwndLocalMac);
   
    CreateSet(pwndSender); 
    Setselect(pwndSender);
    SetVisible(pwndEditConfirmMac, FALSE, 0x30);
    // 取得語言 index
    languageindex = CtmConfig::GetInstance()->GetCurrentIndex();
    if(languageindex < 0)	languageindex = 0;
    if(languageindex != 0)	LogoIndex =1;    
    if(pwndSelectLanguage != NULL)
    {	
    	languageindex = CtmConfig::GetInstance()->GetCurrentIndex();
    	if(languageindex < 0)	languageindex = 0;
      if(languageindex != 0)	LogoIndex =1;
    	((CtmSelectEdit *)pwndSelectLanguage)->SetIndex(languageindex);
    	pwndSelectLanguage->Update();
    }
    // 顯示當前顯示語言
    LG_selected = languageindex;
    LG_Page=LG_selected/4;
    UpdateImg_LG_Choose(); // Update Img
    UpdateLGString();
	
    //****
    //  取得日期時間                                                                // Wain 2007/04/17 Add <<
    //****
    if(pwndEditMonth != NULL) pwndEditMonth->GetPropValueT("value", &u_OldDate.month,sizeof(u_OldDate.month));
    if(pwndEditYear  != NULL) pwndEditYear->GetPropValueT("value", &PartYear,sizeof(PartYear));
	if((PartYear>=0) && (PartYear <38))	      u_OldDate.year = PartYear+C21_START;
	else if((PartYear>70) && (PartYear <100)) u_OldDate.year = PartYear+RTC_START;
	else                                      u_OldDate.year = 0;
	if(u_OldDate.year > 0)
	{
		if(pwndEditYear  != NULL) pwndEditDay->GetPropValueT("value", &u_OldDate.day,sizeof(u_OldDate.day));
    }
    
    if(pwndEditHour   != NULL) pwndEditHour->GetPropValueT("value", &u_OldTime.hour,sizeof(u_OldTime.hour));
	if(pwndEditMinute != NULL) pwndEditMinute->GetPropValueT("value", &u_OldTime.minute,sizeof(u_OldTime.minute));
	if(pwndEditSecond != NULL) pwndEditSecond->GetPropValueT("value", &u_OldTime.second,sizeof(u_OldTime.second));   // Wain 2007/04/17 Add >>

	if(g_pExTable->GetTableValue(0,"D_0HTPRSUNIT"))
	{
		nOldPresUnitCh = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED48").lValue;
  		nIndex = nOldPresUnitCh;
  	}
	
	if(g_pExTable->GetTableValue(0,"D_0HTSPDUNIT"))
	{
		lMaxSpd1 = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED50").lValue; 		
 		lMaxSpd2 = lMaxSpd1;
 		if(pwndStaticVW_ACT_PRS!=NULL)
 		{
 			if(lMaxSpd1==999)				
 				pwndStaticVW_ACT_PRS->SetPropValueT("textID","VW_UNIT_PERCENT");			
 			else			
 				pwndStaticVW_ACT_PRS->SetPropValueT("textID","VW_PARA4_MMS");
 			pwndStaticVW_ACT_PRS->Update();
 		}
	}
	if(g_pExTable->GetTableValue(0,"D_7TYTEST"))
        {
        	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED53", 0);
        	if(pwndSelectZERO1!=NULL)
        	{
        		int zerosel = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED53").lValue;
        		if(zerosel==1)
        		{
        		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED49", 0);
        		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED52", 0);
        	    }
        	}
        }
	
	if(g_pExTable->GetTableValue(0,"D_SOFTLOCK"))//TYL 2013.10.21 for 7LU SOFTLOCK
	{
	    if(pwndSelectLanguage != NULL)
	      pwndSelectLanguage->UpdateAll();
	}
	
	if(u_biNet)//wangli2015-1-4 10:43:05 進入系統畫面后，光標不經過IP，直接到網絡連線，修正可以重置的問題
	{
		GetIPAddr();	
		
		if(pwndEditConfirmNet != NULL)
		{
			if((acRemoteIP[0] == 0xFF) &&(acRemoteIP[1] == 0) &&(acRemoteIP[2] == 0) &&(acRemoteIP[3] == 0xFF))		//判斷IP，增加“重置”選項 
			{
				pwndEditConfirmNet->SetPropValueT("index",3);		
				pwndEditConfirmNet->SetPropValueT("maxindex",3);			
			}
			else
			{
				pwndEditConfirmNet->SetPropValueT("index",2);		
				pwndEditConfirmNet->SetPropValueT("maxindex",2);
			}
			pwndEditConfirmNet->UpdateAll();
			pwndEditConfirmNet->Update();
		}
	}
	
	if(b_PushKey)
	{
		OilDataOld   =  GetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON").lValue;
		if(m_b7TR||b_7YL||b_7LN)
			OilDataOld = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED25").lValue;
		if(pwndeditPUSHKEYOIL!=NULL)
		{
			if(pwndeditPUSHKEYOIL->Is("CtmEditX2"))
				pwndeditPUSHKEYOIL->SetPropValueT("value",OilDataOld);
			else if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    		((CtmSelectEdit*)pwndeditPUSHKEYOIL)->SetIndex(OilDataOld);
	    	if(b_7YL)
	    		pwndeditPUSHKEYOIL->Update();
	    	if(b_7ZF)
	    		pwndeditPUSHKEYOIL->Update();
	    	if(b_7LN)
	    		pwndeditPUSHKEYOIL->Update();
	    }
	}
	
	if(b_7LG||b_7ZS ||b_7LN) /* zyh 2015-8-6 利廣曲軸畫面 手動打推力座需要動態密碼保護 */
	{
		SetVisible(pwndStaticPUSHKEYOIL, FALSE, 0x30);
    	SetVisible(pwndeditPUSHKEYOIL, FALSE, 0x35);	
	}
	
	if(m_b7TR || b_7HDUnit || b_7ASDCUnit ||b_7ASUnit||b_7ZTUnit)
   	{
   	 	if(pwndEditSampling != NULL)
   	 	{
   	 		int iClmp = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED56").lValue;
   	 		if(pwndeditPUSHKEYOIL!=NULL)
   	 		{
   	 			SetVisible(pwndeditPUSHKEYOIL, FALSE, 0x30);
   	 			SetVisible(pwndStaticPUSHKEYOIL, FALSE, 0x30);
   	 			if(iClmp)
   	 			{
   	 				SetVisible(pwndeditPUSHKEYOIL, TRUE, 0x23);
   	 				SetVisible(pwndStaticPUSHKEYOIL, TRUE, 0x23);
   	 			}
   	 		}
   	 	}
   	 	
   	 	if(pwndeditPOSIUNIT!=NULL)
   	 	{
   	 		iPosUnitOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue;
   	 	}
   	 	
   	 	if(pwndeditTEMPUNIT!=NULL)
   	 	{
   	 		iTempUnitOld = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue;
   	 	}
   	 	
   	 	if(pwndeditPRESUNIT!=NULL)
   	 	{
   	 		iPresUnitOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue;
   	 	}
   	 	
   	 	if(pwndSYST_BRITISHUNIT != NULL)
   	 	{
   	 		iUnitOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue;
   	 	}
   	}

	// 顯示當前背光
	ImgBglight_Update(ReadBgLight());
	
	// UpdateImg 按鍵聲音
	pwndBtn_Beep->SetPropValueT("upbitmap", Beep_ImgPath[g_bMute]);
	pwndBtn_Beep->CreateA();
	pwndBtn_Beep->Show();
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("OnChangeA\n");
//	if(pwndSelectLanguage!=NULL)
//	{
//		long select_value=0;
//		select_value = ((CtmSelectEdit *)pwndSelectLanguage)->GetIndex();
//		printf("select_value=%d\n",select_value);
//	}	
	tmDATE          date;
	tmTIME          time;
	long long       PartYear;
    long long 		lTemp = 0;
    char			cszPwd[20];
    CtmWnd*         pwnd      = NULL;

    if((pwnd = pwndSender->FindControlFromTab(wIDControl)) == NULL) return wIDControl;
	if((pwndEditMonth == pwnd) || (pwndEditYear == pwnd) || (pwndEditDay == pwnd))
    {
        if(pwndEditMonth != NULL) pwndEditMonth->GetPropValueT("value", &date.month,sizeof(date.month));
        if(pwndEditYear  != NULL) pwndEditYear->GetPropValueT("value", &PartYear,sizeof(PartYear));
		if((PartYear>=0) && (PartYear <38))	      date.year = PartYear+C21_START;
		else if((PartYear>70) && (PartYear <100)) date.year = PartYear+RTC_START;
		else                                      date.year = 0;
		if(date.year > 0)
		{
    		pwndEditDay->GetPropValueT("value", &date.day,sizeof(date.day));
    	      
            SetDate(date);	
        }
        
        #ifdef	D_3354
        system("./ReadCpuWriteToRtc");
//      	CloseRTC();
//      	system("hwclock --systohc");
//      	OpenRTC();
      	#endif
      	
         if ((date.year != u_OldDate.year) || (date.month != u_OldDate.month)|| (date.day != u_OldDate.day))
             ClearTemp();           // Wain 2007/04/17 Add                                    
    }
    else if((pwndEditHour   == pwnd) || (pwndEditMinute == pwnd) || (pwndEditSecond == pwnd))
    {
        if(pwndEditHour   != NULL) pwndEditHour->GetPropValueT("value", &time.hour,sizeof(time.hour));
		if(pwndEditMinute != NULL) pwndEditMinute->GetPropValueT("value", &time.minute,sizeof(time.minute));
		if(pwndEditSecond != NULL) pwndEditSecond->GetPropValueT("value", &time.second,sizeof(time.second));		    		              	
    	time.hsecond = 0;
    	//pszID = new char[50];
    	/*if(SetTime(time) != TIME_OK)	strcpy(pszID, "MSG_DATETIME_INVALIDTIME");
    	else
        {*/              
		SetTime(time);
      	
      	#ifdef	D_3354
      	system("./ReadCpuWriteToRtc");
//      	CloseRTC();
//      	system("hwclock --systohc");
//      	OpenRTC();
      	#endif
      	
        if ((time.hour != u_OldTime.hour) ||(time.minute != u_OldTime.minute) ||(time.second != u_OldTime.second))  // Wain 2007/04/17 Add
		     ClearTemp(); 
    }
    else if(pwndEditConfirm == pwnd && pwndSelectLanguage != NULL)
    {
    	if(pwndEditConfirm->Is("CtmEditX1"))
    		pwndEditConfirm->GetPropValueT("value", &lTemp,sizeof(lTemp));
    	else if(pwndEditConfirm->Is("CtmSelectEdit"))
    		lTemp = ((CtmSelectEdit*)pwndEditConfirm)->GetIndex();
    	if(lTemp == 1&&
    			languageindex != ((CtmSelectEdit *)pwndSelectLanguage)->GetIndex())
    	{	
			if(g_pExTable->GetTableValue(0,"D_LOGOLANG"))		//if(versionnumber[0] == "7CM"), 根據語言，切換中英文Logo 
				SetLogoToLanguage();
    		languageindex = ((CtmSelectEdit *)pwndSelectLanguage)->GetIndex();
    		CtmConfig::GetInstance()->SetCurrentIndex(languageindex);
    		//::Reset();
    		//g_pDatabase->Reset();
    		long long temp = 0;
    		if(pwndEditConfirm->Is("CtmEditX1"))
    			pwndEditConfirm->SetPropValueT("value",temp);
    		else if(pwndEditConfirm->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwndEditConfirm)->SetIndex(temp);
    		pwndEditConfirm->Update();
    		
    		SendMsg(MSG_KEY, _KEY_LANGUAGE,0,NULL);
    		
    		//system("sh restart_hmi.sh"); // 重啟 mmi
    		//system("reboot");
    	}
    }
    else  if((pwnd == pwndLocalIP[0]) ||(pwnd == pwndLocalIP[1]) ||(pwnd == pwndLocalIP[2]) ||(pwnd == pwndLocalIP[3]))		//fuxy, 2011-1-21，修改IP地址  
    {
		GetIPAddr();	//fuxy, 2011-1-20，獲取修改後的IP  
		
		if(pwndEditConfirmNet != NULL)
		{
			if((acRemoteIP[0] == 0xFF) &&(acRemoteIP[1] == 0) &&(acRemoteIP[2] == 0) &&(acRemoteIP[3] == 0xFF))		//判斷IP，增加“重置”選項 
			{
				pwndEditConfirmNet->SetPropValueT("index",3);		
				pwndEditConfirmNet->SetPropValueT("maxindex",3);			
			}
			else
			{
				pwndEditConfirmNet->SetPropValueT("index",2);		
				pwndEditConfirmNet->SetPropValueT("maxindex",2);
			}
			pwndEditConfirmNet->UpdateAll();
			pwndEditConfirmNet->Update();
		}
  	}	
  	
  	if(g_pExTable->GetTableValue(0,"D_0HTPRSUNIT"))
	{
		if(pwnd == pwndeditUNITTRANSOPT  &&  pwndeditUNITTRANSOPT != NULL)
  		{
  			nIndex = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED48").lValue;
			if(nOldPresUnitCh != nIndex)
			{
				TransferPrsUnit(g_apszPrsUnitTransID,sizeof(g_apszPrsUnitTransID)/sizeof(g_apszPrsUnitTransID[0]));
				TransferPrsUnit(g_apszPrsUnitTransIDMoldB,sizeof(g_apszPrsUnitTransIDMoldB)/sizeof(g_apszPrsUnitTransIDMoldB[0]));
				TransferPrsUnit(g_apszPrsUnitTransIDMach,sizeof(g_apszPrsUnitTransIDMach)/sizeof(g_apszPrsUnitTransIDMach[0]));
				TransferPrsUnit(g_apszPrsUnitTransIDMachB,sizeof(g_apszPrsUnitTransIDMachB)/sizeof(g_apszPrsUnitTransIDMachB[0]));
				TransferPrsUnit(g_apszPrsUnitTransIDSys,sizeof(g_apszPrsUnitTransIDSys)/sizeof(g_apszPrsUnitTransIDSys[0]));	
				
				if(g_pExTable->GetTableValue(0,"D_HIPRES"))
				{	     	
					long lLowprs = GetDBValue("MACH_OTHR_OTHR_NULL_W03B7").lValue;		
					if((nIndex == 1) && (lLowprs != 0xFFFF))//PSI
						lLowprs = (DWORD)(lLowprs*145/10);
					else if((nOldPresUnitCh==1) && (lLowprs != 0xFFFF))
						lLowprs = (DWORD)(lLowprs*10/145);
					SetDBValue("MACH_OTHR_OTHR_NULL_W03B7", lLowprs);
				}			
				
				nOldPresUnitCh = nIndex;
				usleep(1000*1000);
				MsgBox(g_MultiLanguage["VW_PRESSUNITHELP"],3);
		      	//Prompt(g_MultiLanguage["MSG_DUMMY"]);
			}
  		}
  	}
//ZHBA 2010-12-9 test LCD
 	if(g_pExTable->GetTableValue(0,"D_0HTSPDUNIT"))
 	{
 		if(pwnd == pwndeditINJEMAXDPEED  &&  pwndeditINJEMAXDPEED != NULL)
		{			
			lMaxSpd2 = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED50").lValue;
			//printf("lMaxSpd1:%d  lMaxSpd2:%d\n",lMaxSpd1,lMaxSpd2);
			if(lMaxSpd1 != lMaxSpd2)
			{			
				SetSpedValue();
				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
				usleep(1000*1000);
				//MsgBox(g_MultiLanguage["VW_PRESSUNITHELP"],3);
 				lMaxSpd1 = lMaxSpd2; 				
			}	
		}
 	}

  	if(g_pExTable->GetTableValue(0,"D_OPENTIME")) /*zyh  2014/2/13 for 7NG OpenTime Clean*/
  	{
  	  	if((pwndeditTIME_CLEAN != NULL)&&(pwndeditTIME_CLEAN == pwnd))	
  	  	{
  	  	  	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue == 1)
  	  	  	{
  	  	  		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50", 0);
  	  	  		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", 0);
  	  	  	}
  	  	  
  	  	}
  	}
  	if(b_AutoTime) /*for 7YL*/
  	{
  	  	if((pwndeditTIME_CLEAN != NULL)&&(pwndeditTIME_CLEAN == pwnd))	
  	  	{
  	  	  	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue == 1)
  	  	  	{
  	  	  		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50", 0);
  	  	  		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", 0);
  	  	  	}
  	  	  
  	  	}
  	}
  	
  	if(b_PushKey)
  	{
  		if(b_7LG||b_7ZS)  // 7LG
  		{
  			if(pwnd == pwndeditPUSHKEYOIL  &&  pwndeditPUSHKEYOIL != NULL)
  			{
    			pwndeditPUSHKEYOIL->GetPropValueT("value", &OilDataNew,sizeof(OilDataNew));
    			
  				if(OilDataOld != OilDataNew)
  				{  				
  					
  					MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
  					if(g_bMsgBoxFlag)
  					{
  						OilDataOld = OilDataNew;
  						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALOPTION", 0x1234);
  						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON", OilDataNew);
  						pwndeditPUSHKEYOIL->SetPropValueT("value",OilDataNew);
  						SavePushKeyOilData();
  						//printf("OilTable:%d\n",GetOilTable());
  						//if(b_7LG) /* zyh 2015-8-11 利廣面板打完推力座要把推力座表中十字頭最大位置賦給組態里的機械行程 */
  						//{
  							int		m_aryCross[256];
							int		m_nCrossNum =0;
  							char* pTemp = new char[256*4];
  			
  							//if(GetOilTable()) /* 判斷有對應的推力座表 */
							/*{
    							m_nCrossNum = g_pSelvat->GetCrossNum();
								memcpy(m_aryCross,((CtmSelvatG*)g_pSelvat)->GetCrossTable(),sizeof(m_aryCross));				
								memcpy(pTemp,(char*)m_aryCross,sizeof(m_aryCross));
								delete[] pTemp;
								//printf("m_nCrossNum=%d\n",m_nCrossNum);
  								SetDBValue("CONF_AD1_WRITE_CHL2_TRAVEL", DEC2HEX(m_aryCross[m_nCrossNum]));
								if(g_ptaskdsp != NULL)
									g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
  							}*/
  						//}
  						MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);
  					}
					else
					{
						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON", OilDataOld);
						pwndeditPUSHKEYOIL->SetPropValueT("value",OilDataOld);
				 			((CtmFormView *)pwndSender)->Goto(0);
					}
					pwndeditPUSHKEYOIL->Update();
  				}
   	 		}
   		}
   	 	if(m_b7TR)
   	 	{
   	 		if(pwnd == pwndeditPUSHKEYOIL  &&  pwndeditPUSHKEYOIL != NULL)
  			{
  				OilDataNew = ((CtmSelectEdit*)pwndeditPUSHKEYOIL)->GetIndex();
  				if(OilDataNew!=OilDataOld)
  				{  	
  					int iHMIType = 0;	
  					long EKOil = OilDataNew;//zsq 20150602 For Touch						      				
	    			MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);	
	    			if(!g_bMsgBoxFlag)
	    			{
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataOld);	    						
	    			}
	    			else
	    			{
	    				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataNew);
	    				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED25",OilDataNew);
	    				if(pwndeditHMITYPE!=NULL)
	    				{
	    					pwndeditHMITYPE->GetPropValueT("value", &iHMIType, sizeof(iHMIType));
	    				}
	    				long long lPushData = 0;
	    				if(EKOil==0)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON", 0);
						}
						else if(EKOil==1)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD080);//0xD080
							lPushData = 0xD080;
							if(iHMIType)
								lPushData = 0xD500;
							
						}
						else if(EKOil==2)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD100);//0xD100
							lPushData = 0xD100;
							if(iHMIType)
								lPushData = 0xD600;
						}	
						else if(EKOil==3)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD130);//0xD130
							lPushData = 0xD130;
							if(iHMIType)
								lPushData = 0xD650;
						}
						else if(EKOil==4)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD160);//0xD160
							lPushData = 0xD160;
							if(iHMIType)
								lPushData = 0xD700;
						}
						else if(EKOil==5)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD200);//0xD200
							lPushData = 0xD200;
							if(iHMIType)
								lPushData = 0xD800;
						}
						else if(EKOil==6)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD250);//0xD250
							lPushData = 0xD250;
							if(iHMIType)
								lPushData = 0xD900;
						}
						else if(EKOil==7)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD300);//0xD300
							lPushData = 0xD300;
							if(iHMIType)
								lPushData = 0xDA05;
						}
						else if(EKOil==8)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD350);//0xD350
							lPushData = 0xD350;
							if(iHMIType)
								lPushData = 0xDA30;
						}
						else if(EKOil==9)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD400);//0xD400
							lPushData = 0xD400;
							if(iHMIType)
								lPushData = 0xDA50;
						}
						else if(EKOil==10)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD450);//0xD450
							lPushData = 0xD450;
							if(iHMIType)
								lPushData = 0xDA80;
						}
						else if(EKOil==11)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD500);//0xD500
							lPushData = 0xD500;
							if(iHMIType)
								lPushData = 0xDB20;
						}
						else if(EKOil==12)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD600);//0xD600
							lPushData = 0xD600;
							if(iHMIType)
								lPushData = 0xDB80;
						}
						else if(EKOil==13)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD650);//0xD650
							lPushData = 0xD650;
							if(iHMIType)
								lPushData = 0xDC30;
						}
						else if(EKOil==14)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD700);//0xD700
							lPushData = 0xD700;
							if(iHMIType)
								lPushData = 0xDD00;
						}
						else if(EKOil==15)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD800);//0xD800
							lPushData = 0xD800;
							//if(iHMIType)
								//lPushData = 0xDA50;
						}
						else if(EKOil==16)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xD900);//0xD900
							lPushData = 0xD900;
							//if(iHMIType)
								//lPushData = 0xDA80;
						}
						else if(EKOil==17)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xDA05);//0xDA05
							lPushData = 0xDA05;
							//if(iHMIType)
								//lPushData = 0xDB00;
						}
						else if(EKOil==18)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xDA30);//0xDA30
							lPushData = 0xDA30;
							//if(iHMIType)
								//lPushData = 0xDB20;
						}
						else if(EKOil==19)
						{
							//SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",0xDA50);//0xDA50
							lPushData = 0xDA50;
							//if(iHMIType)
								//lPushData = 0xDB80;
						}
						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",lPushData);//0xDA50
						if(GetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON").lValue < 0xDA30)    //zhouy 2016.8.18 for 7TR push< 0xDA30
	    					SetDBValue("MSET_OTHR_OTHR_NULL_W0854",0);
	    				else
	    					SetDBValue("MSET_OTHR_OTHR_NULL_W0854",1);
						usleep(1000*1000);
						Prompt(g_MultiLanguage["MSG_DUMMY"]);
	    				SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALOPTION", 0x1234);
	    				OilDataOld = OilDataNew;	
	    				SavePushKeyOilData();
	    				MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);
	    			}
	    			((CtmFormView *)pwndSender)->Goto(0);	
				}
  			}
  		}
  		if(b_7YL)
  		{
  			if(pwnd == pwndeditPUSHKEYOIL  &&  pwndeditPUSHKEYOIL != NULL)
  			{
  				OilDataNew = ((CtmSelectEdit*)pwndeditPUSHKEYOIL)->GetIndex();
  				if(OilDataNew!=OilDataOld)
  				{						      				
	    			//MsgBox(g_MultiLanguage["VW_KEYOILDATA_HELP"],3);	
	    			MsgForm("msgbox_PushKey.txt");
	    			if(!g_bMsgBoxFlag)
	    			{
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataOld);						
	    			}
	    			else
	    			{
	    				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataNew);
	    				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED25",OilDataNew);
	    				long long lPushData = 0;
	    				if(OilDataNew==0)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON", 0);
						}
						else if(OilDataNew==1)
						{
							lPushData = 0x98;
						}
						else if(OilDataNew==2)
						{
							//lPushData = 0x118;
							lPushData = 0x120;  //亞力士機型改變，AX120---使用新的120推力座數據  TYL 2016.12.21
						}	
						else if(OilDataNew==3)
						{
							lPushData = 0x138;
						}
						else if(OilDataNew==4)  //亞力士新機型：AX178---使用新的178推力座數據  TYL 2016.12.21
						{
							lPushData = 0x178;
						}
						else if(OilDataNew==5)
						{
							lPushData = 0x228;
						}
						else if(OilDataNew==6)  //亞力士新機型:AX228---使用新的220推力座數據  TYL 2016.12.23
						{
							lPushData = 0x220;  //為了兼容舊的228推力座數據，新的228數據改為220  
						}
						else if(OilDataNew==7)
						{
							lPushData = 0x268;
						}
						else if(OilDataNew==8)
						{
							lPushData = 0x338;
						}
						else if(OilDataNew==9)
						{
							lPushData = 0x428;
						}
						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",lPushData);
						usleep(1000*1000);
						Prompt(g_MultiLanguage["MSG_DUMMY"]);
	    				SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALOPTION", 0x1234);
	    				OilDataOld = OilDataNew;
	    				SavePushKeyOilData();
	    				MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);
	    			}
	    			((CtmFormView *)pwndSender)->Goto(0);	
				}
  			}
  		}
  		if(b_7ZF)
  		{
  			if(pwnd == pwndeditPUSHKEYOIL)
  			{
  				OilDataNew = ((CtmSelectEdit*)pwndeditPUSHKEYOIL)->GetIndex();
  				if(OilDataNew!=OilDataOld)
  				{						      					
	    			MsgForm("msgbox_PushKey.txt");
	    			if(!g_bMsgBoxFlag)
	    			{
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataOld);						
	    			}
	    			else
	    			{
	    				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataNew);
	    				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED25",OilDataNew);
	    				long long lPushData = 0;
	    				if(OilDataNew==0)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON", 0);
						}
						else if(OilDataNew==1)	//機型改變，ZF800---使用800推力座數據
						{
							lPushData = 0x800;
						}
						else if(OilDataNew==2)	//機型改變，ZF900---使用900推力座數據
						{
							//lPushData = 0x118;
							lPushData = 0x900;
						}	
						else if(OilDataNew==3)//機型改變，ZF1100---使用1100推力座數據
						{
							lPushData = 0xA10;
						}
						else if(OilDataNew==4)  //機型改變，ZF1680---使用1680推力座數據
						{
							lPushData = 0xA68;
						}
						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",lPushData);
						usleep(1000*1000);
						Prompt(g_MultiLanguage["MSG_DUMMY"]);
	    				SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALOPTION", 0x1234);
	    				OilDataOld = OilDataNew;
	    				SavePushKeyOilData();
	    				MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);
	    			}
	    			((CtmFormView *)pwndSender)->Goto(0);	
				}
  			}
  		}   	
  		
   	if(b_7LN)
  		{
  			if(pwnd == pwndeditPUSHKEYOIL)
  			{
  				OilDataNew = ((CtmSelectEdit*)pwndeditPUSHKEYOIL)->GetIndex();
  				if(OilDataNew!=OilDataOld)
  				{						      					
	    			MsgForm("msgbox_PushKey.txt");
	    			if(!g_bMsgBoxFlag)
	    			{
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataOld);						
	    			}
	    			else
	    			{
	    				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
	    				if(pwndeditPUSHKEYOIL->Is("CtmSelectEdit"))
	    					((CtmSelectEdit*)pwnd)->SetIndex(OilDataNew);
	    				SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED25",OilDataNew);
	    				long long lPushData = 0;
	    				if(OilDataNew==0)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON", 0);
						}
						else if(OilDataNew==1)	//機型改變，LM90S---使用90推力座數據
						{
							lPushData = 0x90;
						}
						else if(OilDataNew==2)	//機型改變，LM120S---使用120推力座數據
						{
							//lPushData = 0x118;
							lPushData = 0x120;
						}	
						else if(OilDataNew==3)//機型改變，LM160S---使用160推力座數據
						{
							lPushData = 0x160;
						}
						else if(OilDataNew==4)  //機型改變，LM200---使用200推力座數據
						{
							lPushData = 0x200;
						}
						SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON",lPushData);
						usleep(1000*1000);
						Prompt(g_MultiLanguage["MSG_DUMMY"]);
	    				SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALOPTION", 0x1234);
	    				OilDataOld = OilDataNew;
	    				SavePushKeyOilData();
	    				MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);
	    			}
	    			((CtmFormView *)pwndSender)->Goto(0);	
				}
  			}
  		}
   	} 
   	if(b_7LG||b_7ZS||b_7LN) /* zyh 2015-8-6 利廣曲軸畫面 手動打推力座需要動態密碼保護 */
	{
		if(pwnd == pwndPassWord)
		{
			memset(cszPwd, 0, sizeof(cszPwd));
			pwndPassWord->GetPropValueT("text", cszPwd, sizeof(cszPwd));	
			
			if(0 == strcmp(GetAdminPassWord(), cszPwd))
			{
				//printf("success\n");
				SetVisible(pwndStaticPUSHKEYOIL, TRUE, 0x24);
    			SetVisible(pwndeditPUSHKEYOIL, TRUE, 0x23);
			}
			else
			{
				Prompt(g_MultiLanguage["MSG_LOGIN_INVALIDPASSWORD"], 1);
			}
		}	
	}
   	 
   	if(m_b7TR)
   	{
   		if(pwnd == pwndEditSampling  &&  pwndEditSampling != NULL)
   	 	{
   	 		int iClmp = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED56").lValue;
   	 		if(pwndeditPUSHKEYOIL!=NULL)
   	 		{
   	 			if(!iClmp)
   	 			{
   	 				SetVisible(pwndeditPUSHKEYOIL, FALSE, 0x30);
   	 				SetVisible(pwndStaticPUSHKEYOIL, FALSE, 0x30);
   	 			}
   	 			else
   	 			{
   	 				SetVisible(pwndeditPUSHKEYOIL, TRUE, 0x23);
   	 				SetVisible(pwndStaticPUSHKEYOIL, TRUE, 0x23);
   	 			}
   	 		}
   	 	}
   	 	
   	 	if(pwnd == pwndeditPOSIUNIT  &&  pwndeditPOSIUNIT != NULL)
   	 	{
   	 		iPosUnitNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue;
   	 		if(iPosUnitOld!=iPosUnitNew)
   	 		{
   	 			char*	pszPath	= NULL;
   	 			char szPath[128];
   	 			int nCount =0, nIndex =0;
				m_DefaultIni.OpenFile("outerconfig.ini");
				m_DefaultIni.ReadString("selvat","Path", NULL, &pszPath);
				for(nIndex =strlen(pszPath); nIndex >0; nIndex--)
				{
					if(pszPath[nIndex] =='/')
						nCount++;
					if(nCount ==2)
						break;
				}
				strncpy(szPath, pszPath, nIndex+1);
				
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
   	 			{
   	 				SetDBValue("SYSX_OTHERS_OTHERS_NULL_VALOPTION", 0x1234);
   	 				if(iPosUnitNew==1)
   	 				{
   	 					//sprintf(pszPath, "./res_7TRQ8/out/oildata_inch/");
   	 					if(g_pExTable->GetTableValue(0,"D_MLDPOINT"))
   	 						sprintf(szPath, "%soildata_inch_clm2/", szPath);
   	 					else
   	 						sprintf(szPath, "%soildata_inch/", szPath);
						m_DefaultIni.WriteString("selvat","Path",  szPath);  //20160330
						m_DefaultIni.CloseFile();
   	 				}
   	 				else if(iPosUnitNew==0)
   	 				{
   	 					//sprintf(pszPath, "./res_7TRQ8/out/oildata/");
   	 					if(g_pExTable->GetTableValue(0,"D_MLDPOINT"))
   	 						sprintf(szPath, "%soildata_clm2/", szPath);
   	 					else
   	 						sprintf(szPath, "%soildata/", szPath);
						m_DefaultIni.WriteString("selvat","Path",  szPath);
						m_DefaultIni.CloseFile();
   	 				}
   	 				MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);
   	 			}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89", iPosUnitOld);
					
					if(pwndeditPOSIUNIT->Is("CtmEditX2"))
						pwndeditPOSIUNIT->SetPropValueT("value",iPosUnitOld);
					else if(pwndeditPOSIUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndeditPOSIUNIT)->SetIndex(iPosUnitOld);
					((CtmFormView *)pwndSender)->Goto(0);
				}
   	 		}
   	 	}
   	 	
   	 	if(pwnd == pwndeditTEMPUNIT  &&  pwndeditTEMPUNIT != NULL)
   	 	{
   	 		iTempUnitNew = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue;
   	 		if(iTempUnitOld!=iTempUnitNew)
   	 		{
   	 			MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
   	 			if(g_bMsgBoxFlag)
   	 			{
   	 				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
   	 				if(iTempUnitNew==1)
   	 				{
   	 					SetTempValueFahr();
   	 					g_MultiLanguage.SetMulStr("VW_AUSTONE_C","VW_AUSTONE_FAHRTEMP");
   	 				}
   	 				else
   	 				{
   	 					SetTempValueCels();
   	 					g_MultiLanguage.SetMulStr("VW_AUSTONE_FAHRTEMP","VW_AUSTONE_C");
   	 				}
   	 				usleep(1000*1000);
   	 				Prompt(g_MultiLanguage["MSG_DUMMY"]);
   	 				iTempUnitOld = iTempUnitNew;
   	 				((CtmFormView *)pwndSender)->Goto(pwnd->GetTabOrder());
   	 			}
   	 			else
   	 			{
   	 				SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iTempUnitOld);
   	 				
   	 				if(pwndeditTEMPUNIT->Is("CtmEditX2"))
						pwndeditTEMPUNIT->SetPropValueT("value",iTempUnitOld);
					else if(pwndeditTEMPUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndeditTEMPUNIT)->SetIndex(iTempUnitOld);
	    			iTempUnitNew = iTempUnitOld;
					((CtmFormView *)pwndSender)->Goto(0);
				}
				iTempUnitOld = iTempUnitNew;
   	 		}
   	 	}
   	 	
   	 	if(pwnd == pwndeditPRESUNIT  &&  pwndeditPRESUNIT != NULL)
   	 	{
   	 		iPresUnitNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue;
			if(iPresUnitOld!=iPresUnitNew)
			{
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
				{
					if(iPresUnitNew==1)
					{
						g_MultiLanguage.SetMulStr("VW_UNIT_BAR","VW_UNIT_PSI");
						g_MultiLanguage.SetMulStr("VW_UNIT_PRESSURE","VW_UNIT_PSIPRES");
					}
					else
					{
						g_MultiLanguage.SetMulStr("VW_UNIT_PSI","VW_UNIT_BAR");
						g_MultiLanguage.SetMulStr("VW_UNIT_PSIPRES","VW_UNIT_PRESSURE");
					}
					iPresUnitOld  = iPresUnitNew;
					((CtmFormView *)pwndSender)->Goto(pwnd->GetTabOrder());
				}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iPresUnitOld);
					
					if(pwndeditPRESUNIT->Is("CtmEditX2"))
						pwndeditPRESUNIT->SetPropValueT("value",iPresUnitOld);
					else if(pwndeditPRESUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndeditPRESUNIT)->SetIndex(iPresUnitOld);
	    			iPresUnitNew  = iPresUnitOld;
					((CtmFormView *)pwndSender)->Goto(0);
				}
				
			}		
   	 	}
   	}
   	
   	if(b_7HDUnit)/*zyh  2016-9-5*/
   	{
   	 	if(pwnd == pwndSYST_BRITISHUNIT)/*zyh 2016-9-13 海達英制轉換使用一個開關*/
   	 	{
   	 		iUnitNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue;
   	 		if(iUnitOld!=iUnitNew)
   	 		{
   	 			char*		pszPath	= NULL;
				m_DefaultIni.OpenFile("outerconfig.ini");
				m_DefaultIni.ReadString("selvat","Path", 	NULL, &pszPath);
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
   	 			{
   	 				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
   	 				if(iUnitNew==1)
   	 				{
   	 					g_MultiLanguage.SetMulStr("VW_CHARGE_MM","VW_UNIT_INCH1");/*zyh 2016-9-13 位置英制單位處理*/
   	 					sprintf(pszPath, "./res_7HDQ8/out/oildata/in/");
						m_DefaultIni.WriteString("selvat","Path",  pszPath);
						m_DefaultIni.CloseFile();
						SetPosValueInch();
						SetConfPosValueInch();
						
						g_MultiLanguage.SetMulStr("VW_AUSTONE_C","VW_AUSTONE_FAHRTEMP");/*zyh 2016-9-13 溫度英制單位處理*/
						SetTempValueFahr();
						
						g_MultiLanguage.SetMulStr("VW_UNIT_BAR","VW_UNIT_PSI");/*zyh 2016-9-13 壓力英制單位處理*/
						SetPresValuePsi();
   	 				}
   	 				else if(iUnitNew==0)
   	 				{
   	 					g_MultiLanguage.SetMulStr("VW_UNIT_INCH1","VW_CHARGE_MM");/*zyh 2016-9-13 位置英制單位處理*/
   	 					sprintf(pszPath, "./res_7HDQ8/out/oildata/");
						m_DefaultIni.WriteString("selvat","Path",  pszPath);
						m_DefaultIni.CloseFile();
						SetPosValueMM();
						SetConfPosValueMM();
						
						g_MultiLanguage.SetMulStr("VW_AUSTONE_FAHRTEMP","VW_AUSTONE_C");/*zyh 2016-9-13 溫度英制單位處理*/
						SetTempValueCels();
						
						g_MultiLanguage.SetMulStr("VW_UNIT_PSI","VW_UNIT_BAR");/*zyh 2016-9-13 壓力英制單位處理*/
						SetPresValueBar();
   	 				}
   	 				iUnitOld =iUnitNew;
   	 				usleep(1000*1000); 
   	 				if(g_ptaskdsp != NULL)	
					{
						g_ptaskdsp->WriteValue(COMM_SENDCONTROLMACHINE);
							usleep(1000*1000); 
						g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSET);
							usleep(1000*1000); 
						g_ptaskdsp->WriteValue(COMM_SENDPOWERMATCH2);
							usleep(1000*1000);
						g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
							usleep(1000*1000); 
					}
					SetDBValue("SYSX_OTHERS_OTHERS_FLOAT_RESERVED96", 1);/*zyh 2016-9-9  位置單位發生改變 的flag*/
					
					SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iUnitNew);/*原溫度英制單位選擇*/
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iUnitNew);/*原壓力英制單位選擇*/
					Prompt(g_MultiLanguage["MSG_DUMMY"]);
   	 			}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89", iUnitOld);/*英制單位選擇*/
					SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iUnitOld);/*原溫度英制單位選擇*/
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iUnitOld);/*原壓力英制單位選擇*/
					
					if(pwndSYST_BRITISHUNIT->Is("CtmEditX2"))
						pwndSYST_BRITISHUNIT->SetPropValueT("value",iUnitOld);
					else if(pwndSYST_BRITISHUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndSYST_BRITISHUNIT)->SetIndex(iUnitOld);
				}
				((CtmFormView *)pwndSender)->Goto(0);
				MsgForm("msgbox_NoExit.txt");
   	 		}
   	 	}
   	}
   	
   	if(b_7ASDCUnit || b_7ASUnit)/*zyh  2017-3-21*/
   	{
   	 	if(pwnd == pwndSYST_BRITISHUNIT)/*zyh 2017-3-21 海雄英制轉換使用一個開關*/
   	 	{
   	 		iUnitNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue;
   	 		if(iUnitOld!=iUnitNew)
   	 		{
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
   	 			{
   	 				char*	pszPath	= NULL;
					m_DefaultIni.OpenFile("outerconfig.ini");
					m_DefaultIni.ReadString("selvat","Path", 	NULL, &pszPath);
   	 				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
   	 				if(iUnitNew==1)
   	 				{
   	 					g_MultiLanguage.SetMulStr("VW_CHARGE_MM","VW_INJECT_INCH");/*zyh 2017-3-21 位置英制單位處理*/
   	 					sprintf(pszPath, "./res_7AS/out/oildata/in/");
						m_DefaultIni.WriteString("selvat","Path",  pszPath);
						m_DefaultIni.CloseFile();
						SetPosValueRealInch();
						//SetConfPosValueInch_7ASDC();
						
						g_MultiLanguage.SetMulStr("VW_AUSTONE_C","VW_AUSTONE_FAHRTEMP");/*zyh 2017-3-21 溫度英制單位處理*/
						SetTempValueFahr();
						
						g_MultiLanguage.SetMulStr("VW_UNIT_BAR","VW_UNIT_PSI");/*zyh 2017-3-21 壓力英制單位處理*/
						SetPresValuePsi();
   	 				}
   	 				else if(iUnitNew==0)
   	 				{
   	 					g_MultiLanguage.SetMulStr("VW_INJECT_INCH","VW_CHARGE_MM");/*zyh 2017-3-21 位置英制單位處理*/
   	 					sprintf(pszPath, "./res_7AS/out/oildata/");
						m_DefaultIni.WriteString("selvat","Path",  pszPath);
						m_DefaultIni.CloseFile();
						SetPosValueRealMM();
						//SetConfPosValueMM_7ASDC();
						
						g_MultiLanguage.SetMulStr("VW_AUSTONE_FAHRTEMP","VW_AUSTONE_C");/*zyh 2017-3-21 溫度英制單位處理*/
						SetTempValueCels();
						
						g_MultiLanguage.SetMulStr("VW_UNIT_PSI","VW_UNIT_BAR");/*zyh 2017-3-21 壓力英制單位處理*/
						SetPresValueBar();
   	 				}
   	 				iUnitOld =iUnitNew;
   	 				usleep(1000*1000); 
   	 				if(g_ptaskdsp != NULL)	
					{
						g_ptaskdsp->WriteValue(COMM_SENDCONTROLMACHINE);
							usleep(1000*1000); 
						g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSET);
							usleep(1000*1000); 
						g_ptaskdsp->WriteValue(COMM_SENDPOWERMATCH2);
							usleep(1000*1000);
						g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
							usleep(1000*1000); 
					}
					SetDBValue("SYSX_OTHERS_OTHERS_FLOAT_RESERVED96", 1);/*zyh 2017-3-21  位置單位發生改變 的flag*/
					
					SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iUnitNew);/*原溫度英制單位選擇*/
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iUnitNew);/*原壓力英制單位選擇*/
					Prompt(g_MultiLanguage["MSG_DUMMY"]);
					MsgForm("msgbox_NoExit.txt");
   	 			}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89", iUnitOld);/*英制單位選擇*/
					SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iUnitOld);/*原溫度英制單位選擇*/
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iUnitOld);/*原壓力英制單位選擇*/
					
					if(pwndSYST_BRITISHUNIT->Is("CtmEditX2"))
						pwndSYST_BRITISHUNIT->SetPropValueT("value",iUnitOld);
					else if(pwndSYST_BRITISHUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndSYST_BRITISHUNIT)->SetIndex(iUnitOld);
				}
				((CtmFormView *)pwndSender)->Goto(0);
   	 		}
   	 	}
   	}
   	if(g_pExTable->GetTableValue(0,"D_7TYTEST"))
        {
        	if(pwnd == pwndSelectZERO1)
        	{
        		int zerosel = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED53").lValue;
        		if(zerosel==1)
        		{
        		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED49", 0);
        		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED52", 0);
        		SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED53", 0);
        	    }
        	}
        }

	if(b_7ZTUnit)/*for 7ZT*/
   	{
   	 	if(pwnd == pwndeditPOSIUNIT)
   	 	{
   	 		iPosUnitNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue;
   	 		if(iPosUnitOld!=iPosUnitNew)
   	 		{
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
   	 			{
   	 				char*	pszPath	= NULL;
					m_DefaultIni.OpenFile("outerconfig.ini");
					m_DefaultIni.ReadString("selvat","Path", 	NULL, &pszPath);
   	 				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
   	 				if(iPosUnitNew==1)
   	 				{
   	 					g_MultiLanguage.SetMulStr("VW_CHARGE_MM","VW_INJECT_INCH");/* 位置英制單位處理*/
   	 					sprintf(pszPath, "./res_7ZTQ8/out/oildata/in/");
						m_DefaultIni.WriteString("selvat","Path",  pszPath);
						m_DefaultIni.CloseFile();
						SetPosValueRealInch();
						//SetConfPosValueInch_7ASDC();
						
   	 				}
   	 				else if(iPosUnitNew==0)
   	 				{
   	 					g_MultiLanguage.SetMulStr("VW_INJECT_INCH","VW_CHARGE_MM");/* 位置英制單位處理*/
   	 					sprintf(pszPath, "./res_7ZTQ8/out/oildata/7zt/");
						m_DefaultIni.WriteString("selvat","Path",  pszPath);
						m_DefaultIni.CloseFile();
						SetPosValueRealMM();
						//SetConfPosValueMM_7ASDC();
   	 				}
   	 				iPosUnitOld =iPosUnitNew;
   	 				usleep(1000*1000); 
					SetDBValue("SYSX_OTHERS_OTHERS_FLOAT_RESERVED96", 1);/* 位置單位發生改變 的flag*/

					Prompt(g_MultiLanguage["MSG_DUMMY"]);
					MsgForm("msgbox_NoExit.txt");
   	 			}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89", iPosUnitOld);/*英制單位選擇*/
					
					if(pwndeditPOSIUNIT->Is("CtmEditX2"))
						pwndeditPOSIUNIT->SetPropValueT("value",iPosUnitOld);
					else if(pwndeditPOSIUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndeditPOSIUNIT)->SetIndex(iPosUnitOld);
				}
				((CtmFormView *)pwndSender)->Goto(0);
   	 		}
   	 	}
   	 	if(pwnd == pwndeditTEMPUNIT)
   	 	{
   	 		iTempUnitNew = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue;
   	 		if(iTempUnitOld!=iTempUnitNew)
   	 		{
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
   	 			{
   	 				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
   	 				if(iTempUnitNew==1)
   	 				{	
						g_MultiLanguage.SetMulStr("VW_AUSTONE_C","VW_AUSTONE_FAHRTEMP");/*溫度英制單位處理*/
						SetTempValueFahr();
   	 				}
   	 				else if(iTempUnitNew==0)
   	 				{			
						g_MultiLanguage.SetMulStr("VW_AUSTONE_FAHRTEMP","VW_AUSTONE_C");/* 溫度英制單位處理*/
						SetTempValueCels();
   	 				}
   	 				iTempUnitOld =iTempUnitNew;
   	 				usleep(1000*1000); 
					SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iTempUnitNew);/*原溫度英制單位選擇*/
					Prompt(g_MultiLanguage["MSG_DUMMY"]);
					MsgForm("msgbox_NoExit.txt");
   	 			}
				else
				{
					SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38", iTempUnitOld);/*原溫度英制單位選擇*/
					
					if(pwndeditTEMPUNIT->Is("CtmEditX2"))
						pwndeditTEMPUNIT->SetPropValueT("value",iTempUnitOld);
					else if(pwndeditTEMPUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndeditTEMPUNIT)->SetIndex(iTempUnitOld);
				}
				((CtmFormView *)pwndSender)->Goto(0);
   	 		}
   	 	}
   	 	if(pwnd == pwndeditPRESUNIT)
   	 	{
   	 		iPresUnitNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue;
   	 		if(iPresUnitOld!=iPresUnitNew)
   	 		{
				MsgBox(g_MultiLanguage["MSG_DATA_CONFIRM"],3);
				if(g_bMsgBoxFlag)
   	 			{
   	 				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
   	 				if(iPresUnitNew==1)
   	 				{

						g_MultiLanguage.SetMulStr("VW_UNIT_BAR","VW_UNIT_PSI");/* 壓力英制單位處理*/
						SetPresValuePsi();
   	 				}
   	 				else if(iPresUnitNew==0)
   	 				{
						g_MultiLanguage.SetMulStr("VW_UNIT_PSI","VW_UNIT_BAR");/* 壓力英制單位處理*/
						SetPresValueBar();
   	 				}
   	 				iPresUnitOld =iPresUnitNew;
   	 				usleep(1000*1000); 
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iPresUnitNew);/*原壓力英制單位選擇*/
					Prompt(g_MultiLanguage["MSG_DUMMY"]);
					MsgForm("msgbox_NoExit.txt");
   	 			}
				else
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88", iPresUnitOld);/*原壓力英制單位選擇*/
					
					if(pwndeditPRESUNIT->Is("CtmEditX2"))
						pwndeditPRESUNIT->SetPropValueT("value",iPresUnitOld);
					else if(pwndeditPRESUNIT->Is("CtmSelectEdit"))
	    				((CtmSelectEdit*)pwndeditPRESUNIT)->SetIndex(iPresUnitOld);
				}
				((CtmFormView *)pwndSender)->Goto(0);
   	 		}
   	 	}
   	}
	return wIDControl;
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*         pwnd      = NULL;
    long long 		lTemp = 0;
	BOOL			bModifyMac = FALSE;
	BYTE       		Temp0 = 0;
	pwnd	= pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)		return wIDControl;
    
    if(pwndEditConfirmNet == pwnd)//確認iNet重新連線
    {
    	if(pwnd->Is("CtmEditX1"))
    		pwnd->GetPropValueT("value", &lTemp,sizeof(lTemp));
    	else if(pwnd->Is("CtmSelectEdit"))
    		lTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
    	
        long long lConst0 = 0;
        
        if(lTemp==1 && u_biNet)//表示有iNet功能
        {
            GetIPAddr();
			SendMsg(MSG_SH_INET_RESTART, 0, 0, NULL);		//2009-5-27

    		if(pwnd->Is("CtmEditX1"))
    			pwnd->SetPropValueT("value",lConst0);
    		else if(pwnd->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwnd)->SetIndex(lConst0);
            pwnd->Update();

	        g_pDatabase->WriteDefault("SYS_HMI_NULL_NULL_NETIP1", &acRemoteIP[0], TRUE);	//fuxy,2010-7-16, for 不能重置服務器IP  
	        g_pDatabase->WriteDefault("SYS_HMI_NULL_NULL_NETIP2", &acRemoteIP[1], TRUE);
	        g_pDatabase->WriteDefault("SYS_HMI_NULL_NULL_NETIP3", &acRemoteIP[2], TRUE);
	        g_pDatabase->WriteDefault("SYS_HMI_NULL_NULL_NETIP4", &acRemoteIP[3], TRUE);
    	}
    	else if(lTemp==2 && u_biNet)
    	{
    	    SetVisible(pwndEditConfirmMac, TRUE, 0x23);
    	    
    	    GetIPAddr();	//fuxy, 2011-1-20 
    	    
			if((acRemoteIP[0] == 0xFF) &&(acRemoteIP[1] == 0) &&(acRemoteIP[2] == 0) &&(acRemoteIP[3] == 0xFF))		//判斷IP地址 
			{
    			for(int i=2; i<6; i++)
	    		{
	    			if(pwndLocalMac[i] != NULL)
	    		    {
	    				pwndLocalMac[i] ->SetPropValueT("enabled", TRUE);
	    				pwndLocalMac[i] ->Update();
	    			}
	    		}
	    	}
	    	
    		if(pwnd->Is("CtmEditX1"))
    			pwnd->SetPropValueT("value",lConst0);
    		else if(pwnd->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwnd)->SetIndex(lConst0);
            pwnd->Update();
    		
    	}	
    }    
    else  if(pwndEditConfirmMac == pwnd)		//fuxy, 2011-1-21，修改Mac地址後，確認修改  
    {
        long long lConst0 = 0;
    	if(pwnd->Is("CtmEditX1"))
    		pwnd->GetPropValueT("value", &lTemp,sizeof(lTemp));
    	else if(pwnd->Is("CtmSelectEdit"))
    		lTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
    	
    	if(lTemp == 1)
    	{
	    	for(int i=0; i<6; i++)		//取修改後的Mac地址 
    		{
    			pwndLocalMac[i] ->GetPropValueT("value", &Temp0, sizeof(Temp0));
    			m_acPhysicalAddr0[i] = Temp0;
    		}
    		
    		for(int i=2; i<6; i++)
    		{
    			//printf("m_acPhysicalAddr0[%d]:%4X \n", i, m_acPhysicalAddr0[i]);
    			if(m_acPhysicalAddr0[i] != m_acPhysicalAddr[i])
    			{
    				bModifyMac = TRUE;		//Mac地址有變化 
    				break;
    			}
    			else
    				bModifyMac = FALSE;		//Mac地址無變化
    		}
    		//printf("bModifyMac:%d \n", bModifyMac);
    		if(bModifyMac)
    		{
    			for(int i=2;i<6;i++)
	    		{
	    			if(pwndLocalMac[i] != NULL)
	    		    {
	    				pwndLocalMac[i] ->SetPropValueT("enabled", (double)FALSE);
	    				pwndLocalMac[i] ->Update();
	    			}
	    		}
    			SetPhysicalAddr(m_acPhysicalAddr0);		//實際把Mac地址，寫入網卡硬件 
    			MsgBox(g_MultiLanguage["MSG_RESET_RESTART"], tmFT_CODE_TECH);
    		}
    	
    		if(pwnd->Is("CtmEditX1"))
    			pwnd->SetPropValueT("value",lConst0);
    		else if(pwnd->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwnd)->SetIndex(lConst0);
            pwnd->Update();
    	}
    }	
		
	return wIDControl;
}

void	OnShowA(CtmWnd* pwndSender)
{
	return;
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
		printf("Toolbar\n");
}
WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA\n wKey=%d\n",wKey);
	int Bglight = ReadBgLight();
	char cmd[128];
	
	switch(wKey)
	{
//		case 2:
//			::PutCommand("Reset.txt");
//			break;
//		case 3:
//			::PutCommand("Vers_IML.txt");
//			break;			
		case 12:		
			Bglight++;
			if(Bglight>10)	
				Bglight=10;
			sprintf(cmd,"./SetBL %d",Bglight);
			printf("cmd = %s\n",cmd);
			system(cmd);
			ImgBglight_Update(Bglight);
			break;
		case 13:
			Bglight--;
			if(Bglight<=1)	
				Bglight=1;
			sprintf(cmd,"./SetBL %d",Bglight);
			printf("cmd = %s\n",cmd);
			ImgBglight_Update(Bglight);
			system(cmd);
			break;			
//		case 99:
//			//::PutCommand("Func.txt");
//			::PutCommand("Index.txt"); // cjlee 2019/4/6 下午 05:10:43
//			break;
		case 31: // LG_Last
			if(LG_Page>0)
				LG_Page--;
			UpdateLGString();
			UpdateImg_LG_Choose();
			break;
		case 32: // LG_Next
			if(LG_Page<2)
				LG_Page++;
			UpdateLGString();
			UpdateImg_LG_Choose();
			break;
		case 70: // Home
			::PutCommand("Over.txt"); // cjlee 2019/4/17 下午 06:56:41
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
		case 88: // Beep
			g_bMute=!g_bMute;
			pwndBtn_Beep->SetPropValueT("upbitmap", Beep_ImgPath[g_bMute]);
			pwndBtn_Beep->CreateA();
			pwndBtn_Beep->Show();
			break;
		default:
			break;
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else                              return _NULL_KEY;
}
/*---------------------------------------------------------------------------+
|  Function : OnMouseUp()                       	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == pwndBtnConfirm) // 確認
	{
   	MsgBox(g_MultiLanguage["CHANGE_LG_CONFIRM"], tmFT_CODE_TECH); // 詢問確認
   	if(!g_bMsgBoxFlag)	return wIDControl;
   		
		if(languageindex != LG_selected) // 新語言選擇
  	{	
			if(g_pExTable->GetTableValue(0,"D_LOGOLANG"))		//if(versionnumber[0] == "7CM"), 根據語言，切換中英文Logo 
				SetLogoToLanguage();
				languageindex = LG_selected;
				CtmConfig::GetInstance()->SetCurrentIndex(languageindex);
				//g_pDatabase->Reset();
				usleep(1000*1000);
				printf("Change LG done\n");
				
				SendMsg(MSG_KEY, _KEY_LANGUAGE,0,NULL);
				//system("reboot"); // 重啟
  	}
	}
	
	for(int i = 0; i < sizeof(LGChoose_String)/sizeof(LGChoose_String[0]); i++ )
	{
		if((pwnd == pwndBtn_LGChoose[i])||(pwnd == pwndBtn_LGStr[i])) // 語言選擇 選擇紐 || 文字 被選擇
		{
			LG_selected=i+LG_Page*4;
			printf("LG_selected=%d\n",LG_selected);
			UpdateImg_LG_Choose();
		}
	}
	
  return wIDControl;	
}



void  	OnUpdateA(CtmWnd* pwndSender)
{
    if((m_b7TR)&&(g_pExTable->GetTableValue(0,"D_MTRTIME") || g_pExTable->GetTableValue(0,"D_7TRMTRTIME")))
		GetACTTime(pwndMOTORONTIME);
		 
	if(g_pExTable->GetTableValue(0,"D_OPENTIME")) /*zyh  2014/2/13 for 7NG*/
	{
		GetOpenTimeDay(pwndeditOPENDAY);
		GetOpenTimeHour(pwndeditOPENHOUR);	
		GetOpenTimeMinute(pwndeditOPENMINUTE);
	}
	if(b_AutoTime) /*for 7YL*/
	{
		GetAutoTimeDay(pwndeditAUTOTIMEDAY);
		GetAutoTimeHour(pwndeditAUTOTIMEHOUR);	
		GetAutoTimeMinute(pwndeditAUTOTIMEMINUTE);
	}
	
	if(g_pExTable->GetTableValue(0,"D_STSPFUN")&& m_b7TR)
	{
		GetHour(pwndeditMOTORON_HOUR);//2017-4-20 
		GetMinute(pwndeditMOTORON_MINUTE);
		GetHour(pwndeditHEATERON_HOUR);
		GetMinute(pwndeditHEATERON_MIN);
	}
}
/*---------------------------------------------------------------------------+
|  Function : OnDestroyA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void    OnDestroyA(CtmWnd* pwndSender)
{ 
	printf("OnDestroyA\n");
	;
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void        CreateSet(CtmWnd* pwndSender)
{
	tmDATE        date;
	tmTIME        time;
	//tmDATETIME 		datetime;
	//GetRTCDateTime(&datetime);
	GetDate(&date);
	
	GetTime(&time);
	long long temp = 0;
	
	if 	((date.year > 2099 	|| date.year < 2000) 	||
		(date.month < 1 	|| date.month > 12)		||
		(date.day <= 0 		|| date.day > 31) 		||
		(time.hour > 23) 							||
		(time.minute > 59)							||
		(time.second > 59)	)
	{
		date.year 	= 2007;
		date.month 	= 1;
		date.day 	= 1;
		time.hour	= 0;
		time.minute	= 0;
		time.second = 0;
	}
	else
	{
		if (date.month == 2)
		{
			if (date.year % 4 != 0) 
			{
				if (date.day > 28) 
				{
					date.year 	= 2007;
					date.month 	= 1;
					date.day 	= 1;
					time.hour	= 0;
					time.minute	= 0;
					time.second = 0;
				}
			}
			else
			{
				if (date.day > 29) 
				{
					date.year 	= 2007;
					date.month 	= 1;
					date.day 	= 1;
					time.hour	= 0;
					time.minute	= 0;
					time.second = 0;
				}
			}
		}
		else if  (date.month == 1 || date.month == 3 || date.month == 5 ||
				date.month == 7 || date.month == 8 || date.month == 10 ||
				date.month == 12)
		{
			if (date.day > 31) 
			{
				date.year 	= 2007;
				date.month 	= 1;
				date.day 	= 1;
				time.hour	= 0;
				time.minute	= 0;
				time.second = 0;
			}
		}
		else
		{
			if (date.day > 30)
			{
				date.year 	= 2007;
				date.month 	= 1;
				date.day 	= 1;
				time.hour	= 0;
				time.minute	= 0;
				time.second = 0;
			}
		}
	}
	
	if (pwndEditYear  != NULL) 
	{
		temp = date.year % 100;
		pwndEditYear->SetPropValueT("value", temp);
		pwndEditYear->Update();
	}
	if(pwndEditMonth  != NULL) 
	{
		temp = date.month;
		pwndEditMonth->SetPropValueT("value", temp);
		pwndEditMonth->Update();
	}
	//	 the other way
	if(pwndEditDay    != NULL) 
	{
		temp = date.day;
		pwndEditDay->SetPropValueT("value", temp);
		pwndEditDay->Update();
	}
	if(pwndEditHour   != NULL) 
	{
		temp = time.hour;
		pwndEditHour->SetPropValueT("value", temp);
		pwndEditHour->Update();
	}
	if(pwndEditMinute != NULL) 
	{
		temp = time.minute;
		pwndEditMinute->SetPropValueT("value", temp);
		pwndEditMinute->Update();
	}
	if(pwndEditSecond != NULL) 
	{
		temp = time.second;
		pwndEditSecond->SetPropValueT("value", temp);
		pwndEditSecond->Update();
	}
	//*******************
	//停機功能啟用,時間為不可設定
	//*******************
	if(GetDBValue("OPER_HMI_NULL_NULL_STATE").lValue == 0xA5)
		btimeSetVisible =TRUE;
  	else
  	btimeSetVisible =FALSE;
		
	//if(g_pExTable->GetTableValue(0,"D_TRNEWSTOP"))   //Dongxl.2013-12-16 10:27:57
  	if((m_b7TR)&&(GetDBValue("CONF_CODE_READ_NULL_XCODEDATA6").lValue & 0x2000)) //@EA bit13
  	{
  		if(GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED70").lValue&0x0001)
  			btimeSetVisible =TRUE;
  		else
  			btimeSetVisible =FALSE;
  	}
	
	if(btimeSetVisible)
	{
		if (pwndEditYear  != NULL)
			pwndEditYear->SetPropValueT("enabled", (double)FALSE);
		if (pwndEditMonth  != NULL)
			pwndEditMonth->SetPropValueT("enabled",(double)FALSE);
		if (pwndEditDay  != NULL)
			pwndEditDay->SetPropValueT("enabled", (double)FALSE);
		if (pwndEditHour  != NULL)
			pwndEditHour->SetPropValueT("enabled", (double)FALSE);
		if (pwndEditMinute  != NULL)
			pwndEditMinute->SetPropValueT("enabled", (double)FALSE);
		if (pwndEditSecond  != NULL)
			pwndEditSecond->SetPropValueT("enabled", (double)FALSE);
	}
	if (pwndEditYear  != NULL)	pwndEditYear->Update();
	if (pwndEditMonth  != NULL)	pwndEditMonth->Update();
	if (pwndEditDay  != NULL)		pwndEditDay->Update();
	if (pwndEditHour  != NULL)	pwndEditHour->Update();
	if (pwndEditMinute  != NULL)	pwndEditMinute->Update();
	if (pwndEditSecond  != NULL)	pwndEditSecond->Update();
	if(!u_biNet)	//2009-5-27, if(g_ptaskinet == NULL)
	{
		biNetControlsVisible =FALSE;
		for(int i=0;i<4;i++)
			SetVisible(pwndLocalIP[i], biNetControlsVisible, 0x33);
		SetVisible(pwndEditConfirmNet, biNetControlsVisible, 0x33);
		for(int i=0;i<6;i++)
		   	SetVisible(pwndLocalMac[i], biNetControlsVisible, 0x33);

	    if(!u_bKeyLock)
	    {	
	    	SetVisible(pwndStaticLogOut, biNetControlsVisible,0x30);
	    	SetVisible(pwndEditLogOut, biNetControlsVisible,0x30);	    
	    }
	}
	else
	{	
		GetRealPhysicalAddr(m_acPhysicalAddr);		//2009-5-27
		for(int i=0;i<6;i++)
		{	
			if(pwndLocalMac[i] != NULL)
			{
				pwndLocalMac[i] ->SetPropValueT("value", m_acPhysicalAddr[i]);	//2011-3-3, for Mac 
				pwndLocalMac[i] ->Update();
			}
		}
	}
}

void   SetLogoToLanguage()		//fuxy, 2010-5-10, 根據語言，切換中英文Logo 
{
	//char    sz[64];
	int LangNew = ((CtmSelectEdit *)pwndSelectLanguage)->GetIndex();
    if(LangNew == 0)
	{
	 	LogoIndex =0;
  	}
    else  if((LangNew != 0) && (languageindex == 0))
	{
	 	LogoIndex =1;
  	}
    if(u_pszLogoImagePath[LogoIndex] != NULL)
    {
    	char LogoImagePath[20];
	    char	psz[4];
      	memset(psz,0,sizeof(psz));        
	    GetDBString("VERS_HMI_NULL_NULL_PROGRAMMMIVERS",psz,3); //WangHJ2011-6-10 FOR DIFFERENT COMPANY
	    
	    memset(LogoImagePath, 0, sizeof(LogoImagePath));
	    
	    sprintf(LogoImagePath, "%s%s", psz,u_pszLogoImagePath[LogoIndex]);
//		sprintf(sz,"%s",u_pszLogoImagePath[LogoIndex]);
		//sprintf(sz,"%s%s",CtmConfig::GetInstance()->GetBmpPath(),u_pszLogoImagePath[LogoIndex]);
	   	CtmConfig::GetInstance()->SetLogoPath(LogoImagePath);		//fuxy, 2010-1-15 
	}
}

void	Setselect(CtmWnd* pwndSender)
{
	int LanguageNum = 0;
	char sz[10];
	char * psz = NULL;
	int 	i;
	LanguageNum = CtmConfig::GetInstance()->GetLanguageNum();
	if(pwndSelectLanguage != NULL)
		pwndSelectLanguage->SetPropValueT("maxindex" ,LanguageNum);
	for(i = 0; i < LanguageNum; i++)
	{
		//WORD    *pw;
		//sprintf(sz,"%s%d","str",i);
		//psz = g_MultiLanguage[CtmConfig::GetInstance()->GetLanguageShowStr(i)];
		sprintf(sz,"%s%d","strID",i);
		psz = CtmConfig::GetInstance()->GetLanguageShowStr(i);
		//pw  = (WORD *)psz;
		//while(*pw != 0)  printf("%04x ", *pw ++);
		//printf("\n");
		if(pwndSelectLanguage != NULL)
			pwndSelectLanguage->SetPropValueT(sz ,psz);
	}
}

/*---------------------------------------------------------------------------+
|  Function : ClearTemp()                                                    |
|  Task     : 清除溫度記錄                                                   |
+----------------------------------------------------------------------------+
|  Call     : ClearTemp()                                                    |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 無                                                             |
+---------------------------------------------------------------------------*/
void    ClearTemp()
{
	if (g_ptaskDBLog != NULL)
    	g_ptaskDBLog->Reset();
}
	

void	GetACTTime(CtmWnd* pwnd)
{
 	char	pszDB[256];

 	if(pwnd != NULL)
	{
	  	pwnd->GetPropValueT("dbid1",pszDB,sizeof(pszDB));
		long lTemp = GetDBValue(pszDB).lValue;
		lTemp = lTemp * 100 / 60;
		pwnd ->SetPropValueT("value", lTemp);
		pwnd ->Update();
	}
}

void		GetIPAddr()		//fuxy, 2011-1-20, add 
{
	long long TempValue = 0;
	for(int i=0; i<4; i++)
	{
		if(pwndLocalIP[i] !=NULL)
			pwndLocalIP[i] ->GetPropValueT("value", &TempValue,sizeof(TempValue));
	
		acRemoteIP[i] = (BYTE)TempValue;
	}
}

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType)
{
	if(pwnd != NULL)
	{
        if(bVisible)
        {
	      	if(wType)
	      		  pwnd->SetPropValueT("wtype",wType); 
	      	else
	      		  pwnd->SetPropValueT("wtype",0x23);
	    }
	    else
        {
	      	if(wType)
	      		  pwnd->SetPropValueT("wtype",wType); 
	      	else
          	pwnd->SetPropValueT("wtype",0x33);
        }  	
		pwnd->Update();

		pwnd->SetPropValueT("visible",(double)bVisible); 	
		pwnd->Update();
	}
}

void	GetOpenTimeDay(CtmWnd* pwnd)/*zyh  2014/2/13 Get Day*/
{
 	if(pwnd != NULL)
	{
		m_dwOpenTime = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;
		m_dwOpenTimeDay = m_dwOpenTime / 1440;
		pwnd ->SetPropValueT("value", m_dwOpenTimeDay);
		pwnd ->Update();
	}
}

void	GetOpenTimeHour(CtmWnd* pwnd)/*zyh  2014/2/13 Get Hour*/
{
	if(pwnd != NULL)
	{
		m_dwOpenTime = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;
		m_dwOpenTimeHour = (m_dwOpenTime-m_dwOpenTimeDay*24*60) / 60;
		pwnd ->SetPropValueT("value", m_dwOpenTimeHour);
		pwnd ->Update();
	}
}

void	GetOpenTimeMinute(CtmWnd* pwnd)/*zyh  2014/2/13 Get Minute*/
{
	if(pwnd != NULL)
	{
		m_dwOpenTime = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;
		m_wOpenTimeMinute = (m_dwOpenTime-m_dwOpenTimeDay*24*60) % 60;
		pwnd ->SetPropValueT("value", m_wOpenTimeMinute);
		pwnd ->Update();
	}
}
void	GetAutoTimeDay(CtmWnd* pwnd)/*for 7YL*/
{
 	if(pwnd != NULL)
	{
		m_dwAutoTime = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;
		m_dwAutoTimeDay = m_dwAutoTime / 1440;
		pwnd ->SetPropValueT("value", m_dwAutoTimeDay);
		pwnd ->Update();
	}
}

void	GetAutoTimeHour(CtmWnd* pwnd)/*for 7YL*/
{
	if(pwnd != NULL)
	{
		m_dwAutoTime = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;
		m_dwAutoTimeHour = (m_dwAutoTime-m_dwAutoTimeDay*24*60) / 60;
		pwnd ->SetPropValueT("value", m_dwAutoTimeHour);
		pwnd ->Update();
	}
}

void	GetAutoTimeMinute(CtmWnd* pwnd)/*for 7YL*/
{
	if(pwnd != NULL)
	{
		m_dwAutoTime = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;
		m_dwAutoTimeMinute = (m_dwAutoTime-m_dwAutoTimeDay*24*60) % 60;
		pwnd ->SetPropValueT("value",m_dwAutoTimeMinute);
		pwnd ->Update();
	}
}

void   SetSpedValue()
{	
	//printf("lMaxSpd1:%d  lMaxSpd2:%d\n",lMaxSpd1,lMaxSpd2);
	for (int i=0; i<(int)(sizeof(g_apszSpdUnitTransIDSys)/sizeof(g_apszSpdUnitTransIDSys[0])); i++)
 	{
    	long lTemp = GetDBValue(g_apszSpdUnitTransIDSys[i]).lValue;
    	if((lMaxSpd1!=0)&&(lMaxSpd1!=65535))
      		lTemp = lMaxSpd2*lTemp/lMaxSpd1;
	 	SetDBValue(g_apszSpdUnitTransIDSys[i], lTemp, FALSE);	
 	}
	
	for (int i=0; i<(int)(sizeof(g_apszInjeMoldID)/sizeof(g_apszInjeMoldID[0])); i++)
	{
    	long lTemp = GetDBValue(g_apszInjeMoldID[i]).lValue;
    	if((lMaxSpd1!=0)&&(lMaxSpd1!=65535))
      		lTemp = lMaxSpd2*lTemp/lMaxSpd1;
	  	SetDBValue(g_apszInjeMoldID[i], lTemp, FALSE);	
	}
	
	for (int i=0; i<(int)(sizeof(g_apszInjeMoldBID)/sizeof(g_apszInjeMoldBID[0])); i++)
	{
    	long lTemp = GetDBValue(g_apszInjeMoldBID[i]).lValue;
    	if((lMaxSpd1!=0)&&(lMaxSpd1!=65535))
      		lTemp = lMaxSpd2*lTemp/lMaxSpd1;
	  	SetDBValue(g_apszInjeMoldBID[i], lTemp, FALSE);	
	}
	
	for (int i=0; i<(int)(sizeof(g_apszHoldMoldID)/sizeof(g_apszHoldMoldID[0])); i++)
	{
    	long lTemp = GetDBValue(g_apszHoldMoldID[i]).lValue;
    	if((lMaxSpd1!=0)&&(lMaxSpd1!=65535))
      		lTemp = lMaxSpd2*lTemp/lMaxSpd1;
	  	SetDBValue(g_apszHoldMoldID[i], lTemp, FALSE);	
	}
	
	for (int i=0; i<(int)(sizeof(g_apszHoldMoldBID)/sizeof(g_apszHoldMoldBID[0])); i++)
	{
    	long lTemp = GetDBValue(g_apszHoldMoldBID[i]).lValue;
    	if((lMaxSpd1!=0)&&(lMaxSpd1!=65535))
      		lTemp = lMaxSpd2*lTemp/lMaxSpd1;
	  	SetDBValue(g_apszHoldMoldBID[i], lTemp, FALSE);	
	}
}

void   TransferPrsUnit(char* pszData[],int Length)
{
	DWORD lTemp = 0;
  	int   nUnitOpt =GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED48").lValue;		     
	
	for(int i=0; i < Length; i++)
	{
		lTemp = GetDBValue(pszData[i]).lValue;
		
		if((nUnitOpt == 1) && (lTemp != 0xFFFF))//PSI
			lTemp = (DWORD)(lTemp*145/10);
		else if((nOldPresUnitCh==1) && (lTemp != 0xFFFF))
			lTemp = (DWORD)(lTemp*10/145);
		SetDBValue(pszData[i], lTemp, FALSE);	
	}
}

void		SavePushKeyOilData() /* tyl  2015/6/19 for 7TR */
{	
	WORD 	wOilData[2];
   	char	szPath[256];
	char	m_szCopyFile[256];
	      
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/OilData");	      
	ExistDir(szPath);	
	sprintf(m_szCopyFile, "%s/OilData.cdb", szPath);
	      	      	
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{						
		wOilData[0] =(WORD)GetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON").lValue;
		//if(m_b7TR||b_7YL) 
		wOilData[1] =(DWORD)GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED25").lValue;
		write(nFd, &wOilData, sizeof(wOilData));
		close(nFd);
	}
}

//管理員密碼計算=廠商代碼+年+月+日
char*		GetAdminPassWord()
{
	char	Password[7]; 
	char	ver[3];
	
	GetVersionToNumber(&ver[0]);
	DateToString(&Password[0]);
	
  	sprintf(u_cszPwd, "%s%s", ver,Password);
	//printf("2 u_cszPwd=%s \n", u_cszPwd);
	
	return u_cszPwd;
}

void		GetVersionToNumber(char* buff)
{
	int 	a[4];
	char	str[4];
	char	sz[10];
  	memset(str,0,sizeof(str));        
  	memset(sz,0,sizeof(sz));        
	GetDBString("VERS_HMI_NULL_NULL_PROGRAMMMIVERS",str,3);

	for(int i=0;i<3;i++)
 	{
		if(str[i] != '\0') 
		{ 
			if((str[i] >= 'A')&&(str[i] <= 'C')) 
				a[i] = 7;
			else if((str[i] >= 'D')&&(str[i] <= 'F'))
				a[i] = 8;
			else if((str[i] >= 'G')&&(str[i] <= 'I'))
				a[i] = 9;
			else if((str[i] >= 'J')&&(str[i] <= 'L'))
				a[i] = 4;
			else if((str[i] >= 'M')&&(str[i] <= 'O'))
				a[i] = 5;
			else if((str[i] >= 'P')&&(str[i] <= 'R'))
				a[i] = 6;
			else if((str[i] >= 'S')&&(str[i] <= 'U'))
				a[i] = 1;
			else if((str[i] >= 'V')&&(str[i] <= 'X'))
				a[i] = 2;
			else if((str[i] >= 'Y')&&(str[i] <= 'Z'))
				a[i] = 3;
			else if(str[i] == '7')
				a[i] = 7;
			else if(str[i] == '0')
				a[i] = 0;
			else
				a[i] = 8;
		}
 		else
	    	a[i] = 8;
 	}
  	sprintf(sz,"%d%d%d",a[0],a[1],a[2]);
  	strcpy(buff,sz);
}

void DateToString(char* buff)             //時間轉化成字符串
{
  	tmDATE  CurTime;
  	char  String[7];
  	int   temp; 
  
  	GetDate(&CurTime);
  	String[6]='\0';
  	String[5]=CurTime.day-CurTime.day/10*10+48;                       //day
  	String[4]=CurTime.day/10+48;                                      //day
  	String[3]=CurTime.month-CurTime.month/10*10+48;                   //month
  	String[2]=CurTime.month/10+48;                                    //month
  	temp=CurTime.year-CurTime.year/100*100;                           //year的後兩位
  	String[1]=temp-temp/10*10+48;
  	String[0]=temp/10+48;
  	strcpy(buff,String);
}

void	SetTempValueFahr()
{
	long lTemp =0;

  	for(unsigned int i = 0; i < sizeof(g_MSET_SetTempID) / sizeof(g_MSET_SetTempID[0]);i++)
  	{
  		lTemp = GetDBValue(g_MSET_SetTempID[i]).lValue;
  		if(lTemp != 0xFFFF)
      		lTemp = (((lTemp*90)/5+320)+5)/10;
      	//printf("Fahr :%d,%d\n",i,lTemp);
  		SetDBValue(g_MSET_SetTempID[i],lTemp,FALSE);
  	}
  	if(b_7ASDCUnit)
  	{
  		for(unsigned int i = 0; i < sizeof(g_MACH_SetTempID_NoOil) / sizeof(g_MACH_SetTempID_NoOil[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_SetTempID_NoOil[i]).lValue;
  			if(lTemp != 0xFFFF)
    	  		lTemp = (((lTemp*90)/5+320)+5)/10;
  			SetDBValue(g_MACH_SetTempID_NoOil[i],lTemp,FALSE);
  		}
  	}
  	else
  	{
  		for(unsigned int i = 0; i < sizeof(g_MACH_SetTempID) / sizeof(g_MACH_SetTempID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_SetTempID[i]).lValue;
  			if(lTemp != 0xFFFF)
    	  		lTemp = (((lTemp*90)/5+320)+5)/10;
  			SetDBValue(g_MACH_SetTempID[i],lTemp,FALSE);
  		}	
  	}
  	
  	for(unsigned int i = 0; i < sizeof(g_apszSetTempID3) / sizeof(g_apszSetTempID3[0]);i++)
  	{
  		lTemp = GetDBValue(g_apszSetTempID3[i]).lValue;
  		if(lTemp != 0xFFFF)
      		lTemp = (((lTemp*90)/5+320)+5)/10;
  		SetDBValue(g_apszSetTempID3[i],lTemp,FALSE);
  	}
  	for(unsigned int i = 0; i < sizeof(g_MACH_TempUpLowID) / sizeof(g_MACH_TempUpLowID[0]);i++)
  	{
  		lTemp = GetDBValue(g_MACH_TempUpLowID[i]).lValue;
  		if(lTemp != 0xFFFF)
      		lTemp = ((lTemp*90/5)+5)/10;
  		SetDBValue(g_MACH_TempUpLowID[i],lTemp,FALSE);
  	}
  	if(CtmConfig::GetInstance()->GetDspCom2Flag() == 1)
 	{
      	for(unsigned int i = 0; i < sizeof(g_MSET_SetMoldTempID) / sizeof(g_MSET_SetMoldTempID[0]);i++)
      	{
      		lTemp = GetDBValue(g_MSET_SetMoldTempID[i]).lValue;
	  		if(lTemp != 0xFFFF)
   		   		lTemp = (((lTemp*90)/5+320)+5)/10;
      		SetDBValue(g_MSET_SetMoldTempID[i],lTemp,FALSE);
      	}
	}
	if(b_7HDUnit)
	{
		for(unsigned int i = 0; i < sizeof(g_MACH_TempWaterValveID) / sizeof(g_MACH_TempWaterValveID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_TempWaterValveID[i]).lValue;
  			if(lTemp != 0xFFFF)
    	  		lTemp = (((lTemp*90)/5+320)+5)/10;
  			SetDBValue(g_MACH_TempWaterValveID[i],lTemp,FALSE);
  		}	
	}
	if(m_b7TR)
	{
		for(unsigned int i = 0; i < sizeof(g_MACH_TempMiddleTempID) / sizeof(g_MACH_TempMiddleTempID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_TempMiddleTempID[i]).lValue;
  			if(lTemp != 0xFFFF)
      			lTemp = (((lTemp*90)/5+320)+5)/10;
  			SetDBValue(g_MACH_TempMiddleTempID[i],lTemp,FALSE);
  		}
	}
	if(b_7ASDCUnit || b_7ASUnit||b_7ZTUnit)
	{
		for(unsigned int i = 0; i < sizeof(g_MSET_SetTempOILTOTPID) / sizeof(g_MSET_SetTempOILTOTPID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MSET_SetTempOILTOTPID[i]).lValue;
  			if(lTemp != 0xFFFF)
    	  		lTemp = (((lTemp*90)/5+320)+5)/10;
    	  	//printf("Fahr :%d,%d\n",i,lTemp);
  			SetDBValue(g_MSET_SetTempOILTOTPID[i],lTemp,FALSE);
  		}	
	}
}

void	SetTempValueCels()
{
	long lTemp =0;

  	for(unsigned int i = 0; i < sizeof(g_MSET_SetTempID) / sizeof(g_MSET_SetTempID[0]);i++)
  	{
  		lTemp = GetDBValue(g_MSET_SetTempID[i]).lValue;
  		if(lTemp != 0xFFFF)
  		{
            if(lTemp > 31)
  				lTemp = (((lTemp-32)*50/9)+5)/10;
      		else
      			lTemp=0;
      		//printf("Cels :%d,%d\n",i,lTemp);
 		}
  		SetDBValue(g_MSET_SetTempID[i],lTemp,FALSE);
  	}
  	if(b_7ASDCUnit)
  	{
  		for(unsigned int i = 0; i < sizeof(g_MACH_SetTempID_NoOil) / sizeof(g_MACH_SetTempID_NoOil[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_SetTempID_NoOil[i]).lValue;
  			if(lTemp != 0xFFFF)
  			{
    	        if(lTemp > 31)
    	  			lTemp = (((lTemp-32)*50/9)+5)/10;
    	  		else
    	  			lTemp=0;
 			}
  			SetDBValue(g_MACH_SetTempID_NoOil[i],lTemp,FALSE);
  		}	
  	}
  	else
  	{
  		for(unsigned int i = 0; i < sizeof(g_MACH_SetTempID) / sizeof(g_MACH_SetTempID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_SetTempID[i]).lValue;
  			if(lTemp != 0xFFFF)
  			{
    	        if(lTemp > 31)
    	  			lTemp = (((lTemp-32)*50/9)+5)/10;
    	  		else
    	  			lTemp=0;
 			}
  			SetDBValue(g_MACH_SetTempID[i],lTemp,FALSE);
  		}	
  	}
  	
  	for(unsigned int i = 0; i < sizeof(g_apszSetTempID3) / sizeof(g_apszSetTempID3[0]);i++)
  	{
  		lTemp = GetDBValue(g_apszSetTempID3[i]).lValue;
  		if(lTemp != 0xFFFF)
  		{
            if(lTemp > 31)
      			lTemp = (((lTemp-32)*50/9)+5)/10;
      		else
      			lTemp=0;
 		}
  		SetDBValue(g_apszSetTempID3[i],lTemp,FALSE);
  	}
  	for(unsigned int i = 0; i < sizeof(g_MACH_TempUpLowID) / sizeof(g_MACH_TempUpLowID[0]);i++)
  	{
  		lTemp = GetDBValue(g_MACH_TempUpLowID[i]).lValue;
  		if(lTemp != 0xFFFF)
      		lTemp = ((lTemp*50/9)+5)/10;
  		SetDBValue(g_MACH_TempUpLowID[i],lTemp,FALSE);
  	}
  	if(CtmConfig::GetInstance()->GetDspCom2Flag() == 1)
  	{
      	for(unsigned int i = 0; i < sizeof(g_MSET_SetMoldTempID) / sizeof(g_MSET_SetMoldTempID[0]);i++)
      	{
      		lTemp = GetDBValue(g_MSET_SetMoldTempID[i]).lValue;
      		if(lTemp != 0xFFFF)
      		{
            	if(lTemp > 31)
      				lTemp = (((lTemp-32)*50/9)+5)/10;
      			else
      				lTemp=0;
 			}
      		SetDBValue(g_MSET_SetMoldTempID[i],lTemp,FALSE);
      	}
	}
	if(b_7HDUnit)
	{
		for(unsigned int i = 0; i < sizeof(g_MACH_TempWaterValveID) / sizeof(g_MACH_TempWaterValveID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_TempWaterValveID[i]).lValue;
  			if(lTemp != 0xFFFF)
    	  		lTemp = (((lTemp-32)*50/9)+5)/10;
  			SetDBValue(g_MACH_TempWaterValveID[i],lTemp,FALSE);
  		}	
	}
	if(m_b7TR)
	{
		for(unsigned int i = 0; i < sizeof(g_MACH_TempMiddleTempID) / sizeof(g_MACH_TempMiddleTempID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MACH_TempMiddleTempID[i]).lValue;
  			if(lTemp != 0xFFFF)
  			{
				if(lTemp > 31)
      				lTemp = (((lTemp-32)*50/9)+5)/10;
      			else
      				lTemp=0;
 			}
  			SetDBValue(g_MACH_TempMiddleTempID[i],lTemp,FALSE);
  		}
	}
	if(b_7ASDCUnit || b_7ASUnit||b_7ZTUnit)
	{
  		for(unsigned int i = 0; i < sizeof(g_MSET_SetTempOILTOTPID) / sizeof(g_MSET_SetTempOILTOTPID[0]);i++)
  		{
  			lTemp = GetDBValue(g_MSET_SetTempOILTOTPID[i]).lValue;
  			if(lTemp != 0xFFFF)
  			{
    	        if(lTemp > 31)
  					lTemp = (((lTemp-32)*50/9)+5)/10;
    	  		else
    	  			lTemp=0;
    	  		//printf("Cels :%d,%d\n",i,lTemp);
 			}
  			SetDBValue(g_MSET_SetTempOILTOTPID[i],lTemp,FALSE);
  		}
	}
}

void  SetPosValueInch()/*zyh 2016-9-5*/
{
	long lTemp;
	    
    for(int i = 0; i < (int)sizeof(g_apszPosTransID) / sizeof(g_apszPosTransID[0]);i++)
    {
    	lTemp = GetDBValue(g_apszPosTransID[i]).lValue;
    	if(lTemp != 0xFFFF)
    	{
	  		lTemp = lTemp*32/25.4*10;/*1.26 = 32/25.4 */
	  		if(lTemp%10 >= 5)
    			lTemp = lTemp*0.1+1;
    		else
    			lTemp = lTemp*0.1;
	  	}
	  		//printf("Fahr :%d,%d\n",i,lTemp);
    	SetDBValue(g_apszPosTransID[i],lTemp,FALSE);
    }
}

void  SetPosValueRealInch()
{
	long lTemp;
	    
    for(int i = 0; i < (int)sizeof(g_apszPosTransID) / sizeof(g_apszPosTransID[0]);i++)
    {
    	lTemp = GetDBValue(g_apszPosTransID[i]).lValue;
    	if(lTemp != 0xFFFF)
    	{
	  		lTemp = lTemp*10/25.4;
	  		/*if(lTemp%10 >= 5)
    			lTemp = lTemp*0.1+1;
    		else
    			lTemp = lTemp*0.1;*/
	  	}
	  		//printf("Fahr :%d,%d\n",i,lTemp);
    	SetDBValue(g_apszPosTransID[i],lTemp,FALSE);
    }
    if(b_7ZTUnit)
    {
    	for(int i = 0; i < (int)sizeof(g_apsz12ValveID) / sizeof(g_apsz12ValveID[0]);i++)
    	{
    		lTemp = GetDBValue(g_apsz12ValveID[i]).lValue;
    		if(lTemp != 0xFFFF)
    		{
		  		lTemp = lTemp*10/25.4;
		  	}
    		SetDBValue(g_apsz12ValveID[i],lTemp,FALSE);
    	}
    }
    
    if(b_7ASDCUnit)
    {
    	for(int i = 0; i < (int)sizeof(g_apszPosTransID_DC) / sizeof(g_apszPosTransID_DC[0]);i++)
    	{
    		lTemp = GetDBValue(g_apszPosTransID_DC[i]).lValue;
    		if(lTemp != 0xFFFF)
    		{
		  		lTemp = lTemp*10/25.4;
		  		/*if(lTemp%10 >= 5)
    				lTemp = lTemp*0.1+1;
    			else
    				lTemp = lTemp*0.1;*/
		  	}
		  		//printf("Fahr :%d,%d\n",i,lTemp);
    		SetDBValue(g_apszPosTransID_DC[i],lTemp,FALSE);
    	}	
    }
    if(b_7AS)
    {
    	lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED88").lValue;/*機絞畫面的模板行程*/
    	if(lTemp != 0xFFFF)
    	{
		  	lTemp = lTemp*10/25.4;
		}
    	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED88", lTemp,FALSE);
    }
}

void  SetConfPosValueInch()/*zyh 2016-9-5 組態頁面前4根電子尺和機械行程處理*/
{
	long 	lTemp = 0,lTemp2 = 0,lTemp3 = 0;
	char	pDataID1[80],pDataID2[80],pDataID3[80];
	WORD   	PrecisionDate = 0;
	    
    for(int i = 0; i < 4; i++)
    {
    	int j = i;
    	sprintf(pDataID1, "CONF_AD1_WRITE_CHL%d_PHYSICALLENGTH", (i+1));
    	sprintf(pDataID2,"%s","CONF_AD1_WRITE_CHL1_LENGTHP1MM");
    	lTemp2 = (GetDBValue(pDataID2).lValue>>(4*i))&0x000F;
    	//printf("lTemp211=%d\n",lTemp2);
    	lTemp2 = lTemp2*32/25.4*100;
    	//printf("lTemp222=%d\n",lTemp2);
    	if(lTemp2%10 >= 5)
    		lTemp2 = lTemp2*0.1+1;
    	else
    		lTemp2 = lTemp2*0.1;
    	//printf("lTemp233=%d\n",lTemp2);
    	lTemp = ((HEX2BCD)(GetDBValue(pDataID1).lValue)*32/25.4*100)+lTemp2;
    	//printf("new11=%d\n",lTemp);
    	if(lTemp%10 >= 5)
    		lTemp = lTemp*0.1+1;
    	else
    		lTemp = lTemp*0.1;
    	//printf("new22=%d\n",lTemp);
		SetDBValue(pDataID1, DEC2HEX(lTemp));
		if(j==0)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0xFFF0);
		else if(j==1)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0xFF0F);
		else if(j==2)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0xF0FF);
		else if(j==3)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0x0FFF);
		//printf("PrecisionDate=%d\n",PrecisionDate);
		//printf("\n");
		SetDBValue(pDataID2,PrecisionDate);
		sprintf(pDataID3, "CONF_AD1_WRITE_CHL%d_TRAVEL", (i+1));
		lTemp3 = (HEX2BCD)(GetDBValue(pDataID3).lValue);
		//printf("lTemp3old=%d\n",lTemp3);
		lTemp3 = lTemp3*32/25.4*10;
		if(lTemp3%10 >= 5)
    		lTemp3 = lTemp3*0.1+1;
    	else
    		lTemp3 = lTemp3*0.1;
		//printf("lTemp3new=%d\n",lTemp3);
		//printf("\n");
		SetDBValue(pDataID3, DEC2HEX(lTemp3*10));
    }
    if(g_ptaskdsp != NULL)	
		g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
}

void  SetPosValueMM()/*zyh 2016-9-5*/
{
	long lTemp;
	 
    for(int i = 0; i < (int)sizeof(g_apszPosTransID) / sizeof(g_apszPosTransID[0]);i++)
    {
    	lTemp = GetDBValue(g_apszPosTransID[i]).lValue;
    	if(lTemp != 0xFFFF)
    	{
	  		lTemp = lTemp*25.4/32*10;/*0.794 = 25.4/32*/
	  		if(lTemp%10 >= 5)
    			lTemp = lTemp*0.1+1;
    		else
    			lTemp = lTemp*0.1;
	  	}
	  		//printf("Fahr :%d,%d\n",i,lTemp);
    	SetDBValue(g_apszPosTransID[i],lTemp, FALSE);
    }
}
void  SetPosValueRealMM()
{
	long lTemp;
	 
    for(int i = 0; i < (int)sizeof(g_apszPosTransID) / sizeof(g_apszPosTransID[0]);i++)
    {
    	lTemp = GetDBValue(g_apszPosTransID[i]).lValue;
    	if(lTemp != 0xFFFF)
    	{
	  		lTemp = lTemp*25.4/10;
	  		/*if(lTemp%10 >= 5)
    			lTemp = lTemp*0.1+1;
    		else
    			lTemp = lTemp*0.1;*/
	  	}
	  		//printf("Fahr :%d,%d\n",i,lTemp);
    	SetDBValue(g_apszPosTransID[i],lTemp, FALSE);
    }
    if(b_7ZTUnit)
    {
    	for(int i = 0; i < (int)sizeof(g_apsz12ValveID) / sizeof(g_apsz12ValveID[0]);i++)
    	{
    		lTemp = GetDBValue(g_apsz12ValveID[i]).lValue;
    		if(lTemp != 0xFFFF)
    		{
		  		lTemp = lTemp*25.4/10;
		  	}
    		SetDBValue(g_apsz12ValveID[i],lTemp, FALSE);
    	}
    }
    
    if(b_7ASDCUnit)
    {
    	for(int i = 0; i < (int)sizeof(g_apszPosTransID_DC) / sizeof(g_apszPosTransID_DC[0]);i++)
    	{
    		lTemp = GetDBValue(g_apszPosTransID_DC[i]).lValue;
    		if(lTemp != 0xFFFF)
    		{
		  		lTemp = lTemp*25.4/10;
		  		/*if(lTemp%10 >= 5)
    				lTemp = lTemp*0.1+1;
    			else
    				lTemp = lTemp*0.1;*/
		  	}
		  		//printf("Fahr :%d,%d\n",i,lTemp);
    		SetDBValue(g_apszPosTransID_DC[i],lTemp, FALSE);
    	}	
    }
    if(b_7AS)
    {
    	lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED88").lValue;/*機絞畫面的模板行程*/
    	if(lTemp != 0xFFFF)
    	{
		  	lTemp = lTemp*25.4/10;
		}
    	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED88", lTemp,FALSE);
    }
}

void  SetConfPosValueMM()/*zyh 2016-9-5 組態頁面前4根電子尺和機械行程處理*/
{
	long 	lTemp = 0,lTemp2 = 0,lTemp3 = 0;
	char	pDataID1[80],pDataID2[80],pDataID3[80];
	WORD   	PrecisionDate = 0;
	    
    for(int i = 0; i < 4; i++)
    {
    	int j = i;
    	sprintf(pDataID1, "CONF_AD1_WRITE_CHL%d_PHYSICALLENGTH", (i+1));
    	sprintf(pDataID2,"%s","CONF_AD1_WRITE_CHL1_LENGTHP1MM");
    	lTemp2 = (GetDBValue(pDataID2).lValue>>(4*i))&0x000F;
    	lTemp2 = lTemp2*25.4/32*100;
    	if(lTemp2%10 >= 5)
    		lTemp2 = lTemp2*0.1+1;
    	else
    		lTemp2 = lTemp2*0.1;
    	//printf("lTemp2=%d\n",lTemp2);
    	lTemp = ((HEX2BCD)(GetDBValue(pDataID1).lValue)*25.4/32*100)+lTemp2;
    	//printf("new=%d\n",lTemp);
    	if(lTemp%10 >= 5)
    		lTemp = lTemp*0.1+1;
    	else
    		lTemp = lTemp*0.1;
		SetDBValue(pDataID1, DEC2HEX(lTemp));
		if(j==0)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0xFFF0);
		else if(j==1)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0xFF0F);
		else if(j==2)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0xF0FF);
		else if(j==3)
			PrecisionDate = (((lTemp % 10)) << (4*j))+(WORD)(GetDBValue(pDataID2).lValue&0x0FFF);
		//printf("PrecisionDate=%d\n",PrecisionDate);
		SetDBValue(pDataID2,PrecisionDate);
		sprintf(pDataID3, "CONF_AD1_WRITE_CHL%d_TRAVEL", (i+1));
		lTemp3 = (HEX2BCD)(GetDBValue(pDataID3).lValue);
		//printf("lTemp3old=%d\n",lTemp3);
		lTemp3 = lTemp3*25.4/32*10;
		if(lTemp3%10 >= 5)
    		lTemp3 = lTemp3*0.1+1;
    	else
    		lTemp3 = lTemp3*0.1;
		//printf("lTemp3new=%d\n",lTemp3);
		//printf("\n");
		SetDBValue(pDataID3, DEC2HEX(lTemp3*10));
    }
    if(g_ptaskdsp != NULL)	
		g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
}

void   SetPresValuePsi()
{
	char pDataID[128];
	long lTemp,lTemp2;
	int i;
	for (i=0; i<(int)(sizeof(g_apszPrsTransID)/sizeof(g_apszPrsTransID[0])); i++)
	{
      lTemp = GetDBValue(g_apszPrsTransID[i]).lValue;
      if(lTemp != 0xFFFF)
	      lTemp = (lTemp*145+5)/10 ;
	  SetDBValue(g_apszPrsTransID[i], lTemp,FALSE);	
	}
	if(b_7ASDCUnit)
	{
		for (i=0; i<(int)(sizeof(g_apszPrsTransID_DC)/sizeof(g_apszPrsTransID_DC[0])); i++)
		{
    	  lTemp = GetDBValue(g_apszPrsTransID_DC[i]).lValue;
    	  if(lTemp != 0xFFFF)
		      lTemp = (lTemp*145+5)/10 ;
		  SetDBValue(g_apszPrsTransID_DC[i], lTemp,FALSE);	
		}	
	}
	
	for ( int i = 0; i < 16; i++)    //for ( int i = 0; i < 16; i+=2)
    {
    	sprintf(pDataID,"CONF_PVL%d_READ_NULL_MAXDATA",i+1);
    	lTemp = GetDBValue(pDataID).lValue;
		if((lTemp != 999) && (lTemp != 99))	/*判斷是否為壓力通道*/  	
		{
    		for ( int j = 0; j < 16; j++)
    		{
				sprintf(pDataID,"PMATCH2_PVL%d_READ_CHL%d_DATA",i+1,j+1);
	    		lTemp = GetDBValue(pDataID).lValue;
			    if(lTemp != 0xFFFF)
			  	    lTemp = (lTemp*145+5)/10 ;
				SetDBValue(pDataID, lTemp,FALSE);
			}
    	}	
    }
    
    lTemp2 = (HEX2BCD)(GetDBValue("CONF_CODE_WRITE_NULL_MAXCLAMPUPPRESSURE").lValue);
    if(lTemp2 != 0 && lTemp2 != 0xFFFF)
    {
    	lTemp2 = (lTemp2*145+5)/10;
    	SetDBValue("CONF_CODE_WRITE_NULL_MAXCLAMPUPPRESSURE",(DEC2HEX)(lTemp2*10));
    	if(g_ptaskdsp != NULL)
			g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
    }
    	
	SaveBlock("/usr/Data/DA/da.cdb", BLOCK_NEWPOWERMATCH_ID);
}

void   SetPresValueBar()
{
	char pDataID[128];
	long lTemp,lTemp2;
	int i;
	for (i=0; i<(int)(sizeof(g_apszPrsTransID)/sizeof(g_apszPrsTransID[0])); i++)
	{
      lTemp = GetDBValue(g_apszPrsTransID[i]).lValue;
      if(lTemp != 0xFFFF)
	      lTemp = (lTemp*100/145+5)/10 ;
	  SetDBValue(g_apszPrsTransID[i], lTemp,FALSE);	
	}
	if(b_7ASDCUnit)
	{
		for (i=0; i<(int)(sizeof(g_apszPrsTransID_DC)/sizeof(g_apszPrsTransID_DC[0])); i++)
		{
    	  lTemp = GetDBValue(g_apszPrsTransID_DC[i]).lValue;
    	  if(lTemp != 0xFFFF)
		      lTemp = (lTemp*100/145+5)/10 ;
		  SetDBValue(g_apszPrsTransID_DC[i], lTemp,FALSE);	
		}	
	}

	for ( int i = 0; i < 16; i++)    //for ( int i = 0; i < 16; i+=2)
    {
    	sprintf(pDataID,"CONF_PVL%d_READ_NULL_MAXDATA",i+1);
    	lTemp = GetDBValue(pDataID).lValue;
		if((lTemp != 999) && (lTemp != 99))	/*判斷是否為壓力通道*/  	
		{
    		for ( int j = 0; j < 16; j++)
    		{					
	    		sprintf(pDataID,"PMATCH2_PVL%d_READ_CHL%d_DATA",i+1,j+1);
    			lTemp = GetDBValue(pDataID).lValue;
			    if(lTemp != 0xFFFF)
				      lTemp = (lTemp*100/145+5)/10 ;
				SetDBValue(pDataID, lTemp,FALSE);
			}
    	}	
    }
    
    lTemp2 = (HEX2BCD)(GetDBValue("CONF_CODE_WRITE_NULL_MAXCLAMPUPPRESSURE").lValue);
    if(lTemp2 != 0 && lTemp2 != 0xFFFF)
    {
    	lTemp2 = (lTemp2*100/145+5)/10 ;
    	SetDBValue("CONF_CODE_WRITE_NULL_MAXCLAMPUPPRESSURE",(DEC2HEX)(lTemp2*10));
    	if(g_ptaskdsp != NULL)
			g_ptaskdsp->WriteValue(COMM_SENDCONFIG);
    }
    
	SaveBlock("/usr/Data/DA/da.cdb", BLOCK_NEWPOWERMATCH_ID);
}

BOOL	SaveBlock(char* pszFileName, WORD wBlockID)
{
	int nSize = 0;
	int nFd = 0;
	BYTE* pbyBuffer = NULL;
	
	//printf("pszFileName = %s, wBlockID = %d\n", pszFileName, wBlockID);
	if(wBlockID >= 0)
	{
		nSize = g_pBlock->GetBlockSize(wBlockID);
		//printf("nSize = %d\n", nSize);
		if(nSize > 0)
		{	
			
			nFd = open(pszFileName, O_RDWR | O_CREAT | O_SYNC);
			if (nFd > 0)
			{
				pbyBuffer = new BYTE[nSize];
				g_pBlock->SetBlock(pbyBuffer, wBlockID);
				
				write(nFd, pbyBuffer, nSize);
				close(nFd);
				
				delete [] pbyBuffer;
				return TRUE;
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

void	GetHour(CtmWnd* pwnd)	
{
   char	pszDB[256];

 	if(pwnd != NULL)	
	{
	  	pwnd->GetPropValueT("dbid1",pszDB,sizeof(pszDB));
		long lTemp = GetDBValue(pszDB).lValue;
		lTemp = (lTemp %(60 *24)) /60;		
		pwnd ->SetPropValueT("value", lTemp);
		pwnd ->Update();
	}
}
void	GetMinute(CtmWnd* pwnd)	
{
   char	pszDB[256];

 	if(pwnd != NULL)	
	{
	  	pwnd->GetPropValueT("dbid1",pszDB,sizeof(pszDB));
		long lTemp = GetDBValue(pszDB).lValue;
		lTemp = lTemp %60;
		//printf("lTemp3:%d \n", lTemp);
		pwnd ->SetPropValueT("value", lTemp);
		pwnd ->Update();
	}
}

/*---------------------------------------------------------------------------+
|  Function : ReadBgLight()                                                  |
|  Task     : 讀取背光亮度                                                   |
+----------------------------------------------------------------------------+
|  Call     : 						                                                   |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 背光亮度                                                       |
+---------------------------------------------------------------------------*/
int	ReadBgLight()	
{
	FILE *BL_FILE = NULL; 
	int read_level=0;
	BL_FILE = fopen(BACKLIGHT_LEVEL,"r");
	if(BL_FILE != NULL)
	{
		fscanf(BL_FILE,"%d",&read_level);
		printf("Read Level = %d \n",read_level);
		fclose(BL_FILE);
		return read_level;
	}
}
/*---------------------------------------------------------------------------+
|  Function : ImgBglight_Update()                                            |
|  Task     : 更新背光亮度條顯示                                             |
+----------------------------------------------------------------------------+
|  Call     : 						                                                   |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 				                                                       |
+---------------------------------------------------------------------------*/
void ImgBglight_Update(int Bglight)
{
	pwndImgBgLightBar->CreateA();
	pwndImgBgLightBar->Show();
	pwndImgBgLight->SetPropValueT("left",(BgLight_Position+Bglight*BgLight_Width));
	pwndImgBgLight->SetPropValueT("right",(BgLight_Position+Bglight*BgLight_Width+BgLight_Width));
	pwndImgBgLight->CreateA();
	pwndImgBgLight->Show();
}
/*---------------------------------------------------------------------------+
|  Function : UpdateImg_LG_Choose()                                          |
|  Task     : 更新語言選擇顯示	                                             |
+----------------------------------------------------------------------------+
|  Call     : 						                                                   |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 				                                                       |
+---------------------------------------------------------------------------*/
void UpdateImg_LG_Choose()// Update Img
{
	for(int i = 0; i < sizeof(LGChoose_String)/sizeof(LGChoose_String[0]); i++ )
	{
		if(pwndBtn_LGChoose[i]!=NULL)
		{
			if( (i+LG_Page*4)==LG_selected )
				{
					pwndBtn_LGChoose[i]->SetPropValueT("upbitmap",ChooseBox_ImgPath[OK]);
				}
			else
				pwndBtn_LGChoose[i]->SetPropValueT("upbitmap",ChooseBox_ImgPath[NO]);
			pwndBtn_LGChoose[i]->CreateA();
			pwndBtn_LGChoose[i]->Update();
			printf("CreateA pwndBtn_LGChoose[%d]\n",i);
		}
	} 
}
/*---------------------------------------------------------------------------+
|  Function : UpdateLGString()                                          |
|  Task     : 更新語言選擇文字	                                             |
+----------------------------------------------------------------------------+
|  Call     : 						                                                   |
|                                                                            |
|  Parameter:                                                                |
|  Return   : 				                                                       |
+---------------------------------------------------------------------------*/
void UpdateLGString()// Update Img
{
	printf("LG_Page=%d\n",LG_Page);
	for(int i = 0; i < sizeof(LGBtn_String)/sizeof(LGBtn_String[0]); i++ )
	{
		pwndBtn_LGStr[i]->SetPropValueT("captionID",LG_String[i+LG_Page*(sizeof(LGBtn_String)/sizeof(LGBtn_String[0]))]);
		pwndBtn_LGStr[i]->CreateA();
		pwndBtn_LGStr[i]->Update();
	}
}
