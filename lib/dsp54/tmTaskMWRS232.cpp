/*===========================================================================+
|  Class    : TaskComm                                                       |
|  Task     : Communication Service Routine                                  |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/13/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"../../utils.h"
#include	"tmTaskMWRS232.h"
#include	"../../main.h"
#include	"../../tmconfig.h"
#include	"../../commonaction.h"
#include	"../../curvedata.h" 
#include	"../../qualitycontrol.h"
#include	"../../manufacture.h"
#include    "../../tmdebug.h"
#include	"../../oil54struct.h"
#include	"../../selvat_ancestor.h"
#include	"../../default.h"
#include	"../../iotable.h"
#include  	"../../tmshmsg.h"
#include    "../../tmdebug.h"


extern	pthread_mutex_t	g_setFlagLock;
IMPLEMENT_DYNCREATE(CtmTaskMWRS232, CTaskHost)
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define   COMM_TIME_RESET           (3 * 1024)
#define   COMM_TIME_REQCONFIG       (3 * 1024)		//wy040600
#define		COMM_TIME_TEMPER					(5 * 1024)		//netdigger 2005/12/3
#define   TLED_SLICE								500

#define		DB_MACH_CMD_ID						0x0020
#define		DB_MOLDSET_CMD_ID					0x0021
#define		DB_MONIMAX_CMD_ID					0x0022
#define		DB_POWERMATCH_CMD_ID			0x0023
#define		DB_MACHB_CMD_ID						0x0024
#define		DB_MOLDSETB_CMD_ID				0x0025
#define		DB_MONIMAXB_CMD_ID				0x0026
#define		DB_NEWPOWERMATCH_CMD_ID		0x0027
#define		DB_OTHER_CMD_ID						0x0030

#define		DB_MACH_MAXINDEX_ID				0x400
#define		DB_MOLDSET_MAXINDEX_ID		0x900
#define		DB_POWERMATCH_MAXINDEX_ID	0xA00
#define		DB_MONIMAX_MAXINDEX_ID		0xA01
#define		DB_MACHB_MAXINDEX_ID			0x1100
#define		DB_MOLDSETB_MAXINDEX_ID		0x1400
#define		DB_MONIMAXB_MAXINDEX_ID		0x1460


BOOL        bSpeechDelayOnOff2;              //wy120999 add dor delay speech 100ms
long        m_lCounterSpeechDelayOnOff2;
WORD		dbgppc2[tmMAX_GROUP_PC];
WORD		g_wTaskCom2DSP54;
WORD		wSendMold = 0; //JOYCE2011-4-1 
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

const       COMMMAP2 g_commmap2[] = {
            {COMM_GETCOLDSTART      		, &CtmTaskMWRS232::GetColdStart    		},
            {COMM_GETWARMSTART      		, &CtmTaskMWRS232::GetWarmStart    		},
            {COMM_GETCONFIG         		, &CtmTaskMWRS232::GetConfig       		},
            {COMM_GETPRODUCTION     		, &CtmTaskMWRS232::GetProduction   		},
            {COMM_GETOPSTATE        		, &CtmTaskMWRS232::GetOPState      		},
            {COMM_GETOPSTATUS       		, &CtmTaskMWRS232::GetOPStatus     		},
            {COMM_GETOPMETER        		, &CtmTaskMWRS232::GetOPMeter      		},
            {COMM_GETOPMETERZEROAGAIN		,	&CtmTaskMWRS232::GetOPMeterZeroAgain},
            {COMM_GETOPTEMPERATURE  		, &CtmTaskMWRS232::GetOPTemperature		},
            {COMM_GETOPSUMMARY      		, &CtmTaskMWRS232::GetOPSummary    		},      //St020101
            {COMM_GETCURVEDATA      		, &CtmTaskMWRS232::GetCurveData    		},
            {COMM_GETCONTROLINDEX   		, &CtmTaskMWRS232::GetControlIndex 		},
            {COMM_GETDAOUTPUT       		, &CtmTaskMWRS232::GetDAOutput     		},      // St1108'01 for D/A
            {COMM_GETDAOUTPUT2      		, &CtmTaskMWRS232::GetDAOutput2    		},	//Andy 20061212 for New D/A
            {COMM_GETMONICURRENT    		, &CtmTaskMWRS232::GetMoniCurrent  		},
            {COMM_GETMONILAST       		, &CtmTaskMWRS232::GetMoniLast     		},
            {COMM_GETMONICURRENTB   		, &CtmTaskMWRS232::GetMoniCurrentB 		},
            {COMM_GETMONILASTB      		, &CtmTaskMWRS232::GetMoniLastB    		},
            {COMM_GETMONIMAXMINB    		, &CtmTaskMWRS232::GetMoniMaxMinB  		},
            {COMM_GETDIAGRM         		, &CtmTaskMWRS232::GetDiagRM       		},
            {COMM_GETDIAGRI         		, &CtmTaskMWRS232::GetDiagRI       		},
            {COMM_GETS_PROFILE01_I  		, &CtmTaskMWRS232::GetSProfile01I  		},
            {COMM_GETD_PROFILE01_I  		, &CtmTaskMWRS232::GetDProfile01I  		},
            {COMM_GETE_PROFILE01_I  		, &CtmTaskMWRS232::GetEProfile01I  		},
            {COMM_GETS_PROFILE01_H  		, &CtmTaskMWRS232::GetSProfile01H  		},
            {COMM_GETD_PROFILE01_H  		, &CtmTaskMWRS232::GetDProfile01H  		},
            {COMM_GETE_PROFILE01_H  		, &CtmTaskMWRS232::GetEProfile01H  		},
            {COMM_GETS_BPROFILE01_I 		, &CtmTaskMWRS232::GetSBProfile01I 		},
            {COMM_GETD_BPROFILE01_I 		, &CtmTaskMWRS232::GetDBProfile01I 		},
            {COMM_GETE_BPROFILE01_I 		, &CtmTaskMWRS232::GetEBProfile01I 		},
            {COMM_GETS_BPROFILE01_H 		, &CtmTaskMWRS232::GetSBProfile01H 		},
            {COMM_GETD_BPROFILE01_H 		, &CtmTaskMWRS232::GetDBProfile01H 		},
            {COMM_GETE_BPROFILE01_H 		, &CtmTaskMWRS232::GetEBProfile01H 		},
           	
           	{COMM_SENDCONFIG		    		, &CtmTaskMWRS232::GetEcho         		},    //Andy 20090306
            {COMM_SENDZEROSET		    		, &CtmTaskMWRS232::GetEcho         		},    //Andy 20090306
            {COMM_SENDPANELBREAK    		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDCONTROLMACHINE		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDCONTROLMOLDSET		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDCONTROLMONITOR		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDPOWERMATCH    		, &CtmTaskMWRS232::GetEcho         		},    //Chuntzu 2004/8/13 add
            {COMM_SENDPOWERMATCH2   		, &CtmTaskMWRS232::GetEcho		   			},    //Andy 20061212 for New D/A
            {COMM_SENDCONTROLMACHINEB		, &CtmTaskMWRS232::GetEcho        		},
            {COMM_SENDCONTROLMOLDSETB		, &CtmTaskMWRS232::GetEcho        		},
            {COMM_SENDCONTROLMONITORB		, &CtmTaskMWRS232::GetEcho        		},
            {COMM_SENDSTATUS        		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDR_PROFILE01   		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDS_PROFILE01   		, &CtmTaskMWRS232::GetEcho         		},
            {COMM_SENDCOMMAND       		, &CtmTaskMWRS232::GetEcho         		},      //St0512'03
            {COMM_ECHO_FAILURE      		,	&CtmTaskMWRS232::GetEcho         		},//Andy for echo failure 20090305	
            	
            {COMM_NULL              		, NULL                        				}
            };

WORD        g_wPassOPSum2[] = {                 //St051101 add for update fast select
            ID_VIEW_OVERVIEW     ,
            ID_VIEW_ZEROSET      ,
            ID_VIEW_CLAMP        ,
            ID_VIEW_INJECT       ,
            ID_VIEW_CHARGE       ,
            ID_VIEW_OTHER        ,
            ID_VIEW_PARAFASTSET  ,
            ID_VIEW_CODE         ,
            NULL_ID              
            };

static	char*	g_apszVatTransID[] = {
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA 關模一段終止位置 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB 關模二段終止位置 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC 關模三段終止位置 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD 關模低壓終止位置 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E 關模高壓終止位置           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 高壓鎖模位置               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 高壓開模位置               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA 開模一段終止位置 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB 開模二段終止位置 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC 開模三段終止位置 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD 開模四段終止位置 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE 開模五段終止位置 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 中途托模位置         // St1111'02 add   
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F 中子A進動作位置  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 中子A退動作位置  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 中子B進動作位置  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 中子B退動作位置  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 中子C進動作位置  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 中子C退動作位置  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 中子D進動作位置  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 中子D退動作位置  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 中子E進動作位置  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 中子E退動作位置  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 中子F進動作位置  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 中子F退動作位置  
            "MSET_BLST1_ON_START_POSITION",								// 0330 公模吹氣動作位置 
            "MSET_BLST2_ON_START_POSITION",								// 0334 母模吹氣動作位置 
            "MSET_BLST3_ON_START_POSITION",								// 0333 吹氣#3動作位置   
            "MSET_BLST4_ON_START_POSITION",								// 033D 吹氣#4動作位置   
            "MSET_BLST5_ON_START_POSITION",								// 033D 吹氣#4動作位置   
            "MSET_BLST6_ON_START_POSITION" 								// 033D 吹氣#4動作位置   
            }; 

static	char*	g_apszVatTransMachID[] = {
			"MACH_MLD_OPEN_NULL_W0216"
			};                   
/*!!!!!!
|       Can not execute periodic C routine???
|       Use counter instead!!!
!!!!!!*/
static      BOOL        g_bLEDMotor = FALSE;
static      long        g_lCounterLEDAdjMold;
static      BOOL        g_bLEDAdjMold = FALSE;
static		long		g_lCounterTemp	= COMM_TIME_TEMPER;		//netdigger 2005/12/3
static		int			u_nMicroAdjCount= 0;					//LEO20070621
static		int			u_nAutoAdjCount = 0;					//LEO20070621

int	OPS_AUTO_2;
int	OPS_STATE_BEGIN_2;
int	OPS_SEMIAUTO_2;
int	OPS_TIMEAUTO_2;
int	OPS_SENSORAUTO_2;
int	OPS_MACROADJ_2;
int	OPS_AUTO_2ADJMOLD_2;

BYTE		g_acRxBuffer[4096];		//James move from gloab var
BYTE		g_acTxBuffer[4096];
static 		COMMFRAMERX	g_frameRx;
static		COMMFRAMETX	g_frameTx;
CtmTaskMWRS232*		CtmTaskMWRS232::m_pSelf = NULL;
static	BOOL				u_wThreadStartRunFlag_2=FALSE;
typedef 			void *(* PRSRUN)(void *);
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmTaskMWRS232::CtmTaskMWRS232(WORD wChannel): CTaskHost() 
{
	m_pSelf				= this;
 	memset(&rs232, 0, sizeof(rs232)); //Andy 20090309 for BUG;
 	rs232.wChannel 	= wChannel;	//James modify 2008/7/3 08:45上午
 	
 	if (rs232.wChannel == _COM2)
 		m_bCOM2 = TRUE;  //g_ptaskdsp2,雖是呼叫同一程式,m_bCOM2也同名,但在g_ptaskComm與g_ptaskComm2中已是不同位置
	else
		m_bCOM2 = FALSE; //g_ptaskdsp
		
	rs232.config.DataLength= COM_DL_8;
	rs232.config.StopBit   = COM_SB_1;
	rs232.config.Parity    = COM_PR_NONE;
	rs232.config.BaudRate  = COM_BR_38400;
	rs232.config.Handshake = COM_HS_NONE;     //Chuntzu 2004/8/18 all use handshake none //Chuntzu 2004/8/5 add
	
	rs232.wRxBufferSize = sizeof(g_acRxBuffer);
	rs232.fpRxBuffer    = (BYTE *)&g_acRxBuffer;
	rs232.wTxBufferSize = sizeof(g_acRxBuffer);
	rs232.fpTxBuffer    = (BYTE *)&g_acTxBuffer;
	
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE"	, long(0));   
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1"	, long(0));
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP2"	, long(0));
	
	SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS"	, 	0);			//	自動監測
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERSUBSTEP1", 0);	
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP3",	0);
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP4",	0);
	SetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP5",	0);
	                                                
		
	m_nID				= AddDevice("CRS232", &rs232);
	m_prs232			= NULL;
	if (m_nID != -1) 
		m_prs232		= (CRS232 *)GetDevice(m_nID);
	m_cState   = COMM_STATE_RESET;
	m_cStateRx =
	m_cStateTx = 0;
	
	m_bSendRProfile01  = FALSE;
	m_bSendRBProfile01 = FALSE;
	m_bGotDA	=               //St1114'01 for DA
	m_bNewDA2	=				//Andy 20061212 for New D/A
	bGotConfig	= FALSE;        //wy040600 wy113099

	m_nReqConfigTimes = 10;                    //BW0108'02  
	m_lCounterReqConfig = COMM_TIME_REQCONFIG;   //wy040600 wy113099
	AddCounter((long *)&m_lCounterReqConfig, m_idMe);   //wy040600 wy113099
	
	bSpeechDelayOnOff2 = FALSE;          //wy120999 add for delay speech 100ms
	m_lCounterSpeechDelayOnOff2 = 100;

	dbbuff.flMotorStatus = 0xFF;
	m_tled.lCount    = TLED_SLICE;
	m_tled.nIndex    = 0;
	m_wCmdState           = CMD_NONE;               // St0526'03 for Command send
	m_CmdQueue.wInput     = m_CmdQueue.wOutput = 0;
	m_CmdQueue.wQueueEnd  = MAX_COMMAND_KEEP;
	m_CmdQueue.bQueueFull = FALSE;
	m_bMachineSend        = FALSE;
	m_bSysReset           = GetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION").lValue;       //Steven 20040202 move to here
	m_nCommandIndex		  = -1;
	m_nSendRM			  = 0;	//	fans add 2008/6/16 11:19上午
	m_nRevRM			  = 0;	//	fans add 2008/6/16 11:19上午
	m_bResetClsOpn  = FALSE; //JOYCE2011-1-27

	m_bDACOM2 = g_pExTable->GetTableValue(0,"D_DACOM2");
	m_bDC     = CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC;
	m_bMold40    = g_pExTable->GetTableValue(0,"D_MDT40");
	
	//	fans add
	OPS_AUTO_2 = g_MultiLanguage.GetStrKeyIndex("OPS_AUTO_2");
	OPS_STATE_BEGIN_2 = g_MultiLanguage.GetStrKeyIndex("OPS_AUTO_2");
	OPS_SEMIAUTO_2 = g_MultiLanguage.GetStrKeyIndex("OPS_SEMIAUTO_2");
	OPS_TIMEAUTO_2 = g_MultiLanguage.GetStrKeyIndex("OPS_TIMEAUTO_2");
	OPS_SENSORAUTO_2 = g_MultiLanguage.GetStrKeyIndex("OPS_SENSORAUTO_2");
	OPS_MACROADJ_2 = g_MultiLanguage.GetStrKeyIndex("OPS_MACROADJ_2");
	OPS_AUTO_2ADJMOLD_2 = g_MultiLanguage.GetStrKeyIndex("OPS_AUTO_2ADJMOLD_2");
	
	for(int i = 0; i < 6; i++)
	{
		m_dwInjectTime[i] = 0;	
	}
	
	m_lCounter = COMM_TIME_RESET;
	AddCounter((long *)&m_lCounter, m_idMe);
	AddCounter((long *)&m_lCounterW0, m_idMe);
	AddCounter((long *)&m_lCounterW1, m_idMe);
	AddCounter((long *)&m_lCounterBW1, m_idMe);
	AddCounter((long *)&m_tled.lCount, m_idMe);
	AddCounter(&g_lCounterTemp, m_idMe);//netdigger 2005/12/3
	AddCounter(&m_lCounterClsOpn, m_idMe);//JOYCE2011-1-27
	RegisterMessage(MSG_RS232, m_idMe);
}

CtmTaskMWRS232::~CtmTaskMWRS232()
{
	if(m_nID >= 0)	DelDevice(m_nID);
	UnRegisterAllMessage(m_idMe);
	DeleteCounter(&g_lCounterTemp, m_idMe);//netdigger 2005/12/3
	DeleteCounter((long *)&m_tled.lCount, m_idMe);
	DeleteCounter((long *)&m_lCounterW1, m_idMe);
	DeleteCounter((long *)&m_lCounterBW1, m_idMe);
	DeleteCounter((long *)&m_lCounterW0, m_idMe);
	DeleteCounter((long *)&m_lCounter, m_idMe);
	DeleteCounter((long *)&m_lCounterSpeechDelayOnOff2, m_idMe);      //wy012500 
	DeleteCounter((long *)&m_lCounterClsOpn, m_idMe);//JOYCE2011-1-27
}

int		CtmTaskMWRS232::CreateSelf()
{
	CTaskHost::CreateSelf();
	pthread_create(&m_RunThreadID, NULL, (PRSRUN)_Run, NULL);
	return 0;
}

int		CtmTaskMWRS232::FreeSelf()
{
	 return CTaskHost::FreeSelf();
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
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
void        CtmTaskMWRS232::Run()
{
	 u_wThreadStartRunFlag_2=TRUE;
}

void        CtmTaskMWRS232::SetStatusWindow(CtmWnd* pwndStatus)
{
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
void        CtmTaskMWRS232::SendConfig()
{
	int         nWriteCount;
	WORD		wLength;
	g_frameTx.wCmd = COMM_SENDCONFIG;
	if(m_bCOM2)				//James add 2008/7/3 模溫機的config block為BLOCK_CONFIG2_ID
	{
		wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_CONFIG2_ID);	
	}
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendZeroSet()
{
	int         nWriteCount;
	WORD		wLength;
	
	g_frameTx.wCmd = COMM_SENDZEROSET;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_COM2ZSET_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendRequestConfig()   //wy040600 wy113099
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDREQUESTCONFIG;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendPanel(BYTE cKey)
{
	if (!g_bAction) return;
	
	int         nWriteCount;
	long		lValue;
	#ifdef  D_X86      //Sunny<20070907 add<<<<<
	if (cKey == (BYTE)(_KEY_PANEL_MOTIONSTART & 0x00FF))				//LEO20070621
	{
		if(u_nMicroAdjCount == 0)	u_nAutoAdjCount = 1;
		else						return;	
	}	
	if (cKey == (BYTE)(_KEY_PANEL_MOLDADJ & 0x00FF))					//LEO20070621
	{
		if((u_nAutoAdjCount == 1)&&(u_nMicroAdjCount == 0))
		{
		    u_nMicroAdjCount =0;
		    u_nAutoAdjCount =0;
		    SendPanel(cKey);
		    return;
		}

	if(u_nAutoAdjCount > 0)	return;
	if (u_nMicroAdjCount == 0)  u_nMicroAdjCount++;
	else if (u_nMicroAdjCount == 1)
		{
			#ifdef  D_ARM   //Sunny<20070907 add<<<<<
			u_nAutoAdjCount = 1;
			#endif          //Sunny<20070907 add>>>>>>
			u_nMicroAdjCount++;
			//g_tmDebugInfo->PrintDebugInfo("cKey2:%d,%d\n",u_nAutoAdjCount,u_nMicroAdjCount);
			SendPanel(cKey);
			usleep(1000);
			SendPanel((BYTE)(_KEY_PANEL_BREAK & 0x00FF));
			usleep(1000);
			u_nMicroAdjCount = 0;
		}	
	}
	#endif  //Sunny<20070907 add>>>>>>			
	/*******
	|       Special control process
	*******/
	lValue = GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue;//JOYCE2011-9-2 bug,資料有誤  OPER_HMI_NULL_NULL_TIMEOUTSTATUS
	if (lValue)                  //Chuntzu 2004/5/4 add for 停機功能
	    if ((cKey == (BYTE)(_KEY_PANEL_SEMIAUTO & 0x00FF)) || (cKey == (BYTE)(_KEY_PANEL_SENSORAUTO & 0x00FF)) ||
	         (cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF)) ) return;

	/*******
	|       Normal process
	*******/
	if (cKey != 0xFF)                       // Check if it is a panel break code
	{
	    cKey += (BYTE)(0x20 - 0x80);
	    m_CmdPanelBreak.wCmd   = 0;
	}
	else
    {
	    SetDBValue("MACH_OTHR_OTHR_NULL_WPANELKEYCODE", 0xFFFF, FALSE);
	    m_CmdPanelBreak.wCmd   = 0x20;
	    m_CmdPanelBreak.wIndex = g_pDatabase->Read("MACH_OTHR_OTHR_NULL_WPANELKEYCODE").lID;
	    m_CmdPanelBreak.wValue = 0xFFFF;
	    m_cStateRx |= COMM_STATE_WAIT00;
	    m_lCounterW0 = 512;
    }
	g_frameTx.wCmd = (WORD)COMM_SENDPANEL | (WORD)cKey;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendControlIndex(char* pszData)
{
	int	nIndex = g_pDatabase->GetIndex(pszData);
	if (nIndex >= 0)
		SendControlIndex(nIndex);
}

void        CtmTaskMWRS232::SendControlIndex(int nIndex)
{
	long	lID, lValue;
	char*	pszData;
	WORD	wType, wHValue, wLValue;
	
	if (nIndex < 0)		return ;
	lID		= g_pDatabase->IndexToDataID(nIndex);
	wType	= g_pDatabase->Read(nIndex).DataType.wType;
	pszData = g_pDatabase->GetString(nIndex);
	
	wHValue = tmHIWORD(GetDBValue(nIndex).lValue);
	wLValue = tmLOWORD(GetDBValue(nIndex).lValue);
	lValue = wLValue;
SENDAGAIN:

	if (pszData != NULL)
    {
	    g_frameTx.wCmd = COMM_SENDCONTROLINDEX;
	    g_frameTx.commdata.controlindex.wIndex = lID;
	    
	    if ((GetOilTable()) && CheckOilVatID(pszData))
	        g_frameTx.commdata.controlindex.wValue = GetOilVat(lValue);
	    else
	    {    	
	        g_frameTx.commdata.controlindex.wValue = lValue;
	    }
    
    CMDDATA cmddata;
    	if (g_frameTx.commdata.controlindex.wIndex == 0x3FE) //Gwei 2006/2/16 閥門強制輸出,使用WMACHSPECIALRESERVED46特殊參數
    	{
	    	cmddata.wCmd   = DB_OTHER_CMD_ID;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wValue;//將Value存入index傳主機//g_frameTx.commdata.controlindex.wIndex;
				//printf("g_frameTx.commdata.controlindex.wValue=%4x\n",g_frameTx.commdata.controlindex.wValue);
    	}
    	else if (g_frameTx.commdata.controlindex.wIndex == 0x3FD) /*JOYCE 20130514 GROUP B 閥門強制輸出,使用WMACHSPECIALRESERVED45特殊參數*/
    	{
	    	cmddata.wCmd   = 0x31;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wValue;//將Value存入index傳主機//g_frameTx.commdata.controlindex.wIndex;
				//printf("B g_frameTx.commdata.controlindex.wValue=%4x\n",g_frameTx.commdata.controlindex.wValue);
    	}
   	 	else if (g_frameTx.commdata.controlindex.wIndex < DB_MACH_MAXINDEX_ID )
        {   
        	//James add 此溫度加溫緩沖區不發送給模溫機
        	if( g_frameTx.commdata.controlindex.wIndex >= 0x290 && g_frameTx.commdata.controlindex.wIndex <= 0x2A3)
        	{
        		return;
        	}
        	else if( g_frameTx.commdata.controlindex.wIndex >= 0x220 && g_frameTx.commdata.controlindex.wIndex <= 0x22F)
        	{
        		return;
        	}
        	else
        	{
        		cmddata.wCmd   = DB_MACH_CMD_ID;
        		cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex;
        	}				
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSET_MAXINDEX_ID)
        {   
					if( g_frameTx.commdata.controlindex.wIndex >= 0x7A0 && g_frameTx.commdata.controlindex.wIndex <= 0x7B3)
        	{
        		return;
        	}
					else if( g_frameTx.commdata.controlindex.wIndex >= 0x7B4 && g_frameTx.commdata.controlindex.wIndex <= 0x7C7) //JOYCE2011-1-25 模溫加熱速率
        	{
        		return;
        	}
        	else if( g_frameTx.commdata.controlindex.wIndex == 0x710)/*JOYCE2012-3-9 COM1為座台進壓力流量,COM2為閥門壓力流量*/
        	{
        		if(GetDBValue("MSET_IJU_FWD_NULL_PRESSURE").lValue != GetDBValue("MSET_GATE_GATE_NULL_W0626").lValue)
        		return;             
        	}
        	else if( g_frameTx.commdata.controlindex.wIndex == 0x711)/*JOYCE2012-3-9 COM1為座台進壓力流量,COM2為閥門壓力流量*/
        	{
        		if(GetDBValue("MSET_IJU_FWD_NULL_FLOW").lValue != GetDBValue("MSET_GATE_GATE_NULL_W0627").lValue)
        		return;             
        	}
        	else if( g_frameTx.commdata.controlindex.wIndex >= 0x5E0 && g_frameTx.commdata.controlindex.wIndex <= 0x6EF)	//JOYCE2011-1-20 com2 閥門
        	{
        		if(g_frameTx.commdata.controlindex.wIndex == 0x626)/*JOYCE2012-3-9 閥門壓力*/
        			{
	        			cmddata.wCmd   = DB_MOLDSET_CMD_ID;
	        			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x626 + 0x310;/*DSP COM2閥門壓力使用310*/
	        		}
        		else	if(g_frameTx.commdata.controlindex.wIndex == 0x627)/*JOYCE2012-3-9 閥門流量*/
        			{
	        			cmddata.wCmd   = DB_MOLDSET_CMD_ID;
	        			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x627 + 0x311;/*DSP COM2閥門流量使用311*/
        			}
        		else		
        			return;
        	}
        	else if( g_frameTx.commdata.controlindex.wIndex >= 0x4C0 && g_frameTx.commdata.controlindex.wIndex <= 0x5CF)	//JOYCE2011-1-20 com2 閥門
        	{
        		return;
        	}
        	else
        	{
        		cmddata.wCmd   = DB_MOLDSET_CMD_ID;
        		cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MACH_MAXINDEX_ID;
        	}
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < DB_POWERMATCH_MAXINDEX_ID)
        {
        	if(m_bCOM2)
        		return;     /*//JOYCE2012-2-17 不發送*/
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < DB_MONIMAX_MAXINDEX_ID)
        {   
        	cmddata.wCmd   = DB_MONIMAX_CMD_ID;
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_POWERMATCH_MAXINDEX_ID;
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < 0xA61)
        	return;   
    	else if (g_frameTx.commdata.controlindex.wIndex < 0xAC1)
        {   
        	cmddata.wCmd   = DB_MONIMAX_CMD_ID;
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0xA60;
        }
		
		else if (g_frameTx.commdata.controlindex.wIndex < 0xF88)		//James add 2008/7/3 
        	return;
        else if (g_frameTx.commdata.controlindex.wIndex < 0xFC4)		//James add 2008/7/3 模溫機MACH資料發送(溫度加溫緩沖區)
        {   
        	int nIndex = 0;
	        cmddata.wCmd   = DB_MACH_CMD_ID;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0xF88;
	        if (cmddata.wIndex < 20)
	        {
	        	nIndex = g_pDatabase->GetIndex("MACH_TMP1_HEATERONOFF_RAMP1_DEGREE");
	            cmddata.wIndex += g_pDatabase->IndexToDataID(nIndex);             //0x290  20段
	        }
	        else
	        {
	            cmddata.wIndex -= 20;
	            nIndex = g_pDatabase->GetIndex("MACH_OTHR_OTHR_NULL_OTHERMACHINE1");
	            cmddata.wIndex += g_pDatabase->IndexToDataID(nIndex);  //WMACHINERESERVED0;    //0x2D0  40段=>0X380 Gwei 2005/10/19//20061122 change to WMACHOTHERRESERVED0;
	        }
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < 0x1000)		//James add 2008/7/3 模溫機MOLDSET資料發送(溫度實際值)
        {   
        	int nIndex = 0;
	        cmddata.wCmd   = DB_MOLDSET_CMD_ID;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0xFC4;
	       
	        if (cmddata.wIndex < 20) 
	        {
	        	nIndex = g_pDatabase->GetIndex("MSET_TMP1_HEATERON_CHL1_DEGREE");
	            cmddata.wIndex += (g_pDatabase->IndexToDataID(nIndex)-0x400);  //0x3A0  20段
	        }
	        else
            {
            	cmddata.wIndex -= 20;
            	nIndex = g_pDatabase->GetIndex("MSET_OTHR_OTHR_NULL_OTHERMACHINE1");
            	cmddata.wIndex += (g_pDatabase->IndexToDataID(nIndex)-0x400);  //(WMOLDRESERVED0-0x400);   //0x420  40段=>0x480 Gwei 2005/10/19//20061122 change to WMOLDOTHERRESERVED0
            }
            
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < DB_MACHB_MAXINDEX_ID)
        {   
        	cmddata.wCmd   = DB_MACHB_CMD_ID;
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1000;
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSETB_MAXINDEX_ID)
        {   
        	cmddata.wCmd   = DB_MOLDSETB_CMD_ID;
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MACHB_MAXINDEX_ID;
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < 0x1430)
        	return;   
    	else if (g_frameTx.commdata.controlindex.wIndex < DB_MONIMAXB_MAXINDEX_ID)
        {   
        	cmddata.wCmd   = DB_MONIMAXB_CMD_ID;
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1430;
        }
   		else if ((g_frameTx.commdata.controlindex.wIndex > 0x18B8)&&(g_frameTx.commdata.controlindex.wIndex < 0x19B9))   //Andy 20061212
        {   
        	if(m_bCOM2)
        		return;     /*//JOYCE2012-2-17 不發送*/
        }
      else if ((g_frameTx.commdata.controlindex.wIndex >= 0x19B9)&&(g_frameTx.commdata.controlindex.wIndex < 0x1AC9))   //Andy 20100323
        {           	
        	if(m_bCOM2)
	        {
	        	cmddata.wCmd   = 0x0021;
	        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x19B9+0x1E0;
	        }
        	else return;
        }
      else if (g_frameTx.commdata.controlindex.wIndex < 0x1B25)
        	return;  
      else if ((g_frameTx.commdata.controlindex.wIndex >= 0x1B25)&&(g_frameTx.commdata.controlindex.wIndex < 0x1B35))   //Andy 20100323
        {   
        	if(m_bCOM2)
	        {
	        	cmddata.wCmd   = DB_MACH_CMD_ID;
	        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1B25+0x220;
	        }
        	else return;
        }
      else if ((g_frameTx.commdata.controlindex.wIndex >= 0x1B35)&&(g_frameTx.commdata.controlindex.wIndex < 0x1C45))   //Andy 20100323
        {   
        	if(m_bCOM2)
	        {
	        	cmddata.wCmd   = 0x0021;
	        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1B35+0xC0;
	        }
        	else return;
        }
    	else if (g_frameTx.commdata.controlindex.wIndex < 0x1D6E)		//JOYCE2011-1-25  模溫機MOLDSET資料發送(溫度加熱速率)
        {   
        	if(m_bCOM2)
					{
	        	int nIndex = 0;
		        cmddata.wCmd   = DB_MOLDSET_CMD_ID;
		        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1D5A;
		       
		        nIndex = g_pDatabase->GetIndex("MSET_ALARM_ON_CHL1_MAXTMPDEGREEOFFSET");
		        cmddata.wIndex += (g_pDatabase->IndexToDataID(nIndex)-0x400);  //0x3B4  20段
	          }
        }
    	else if ((g_frameTx.commdata.controlindex.wIndex > 0x1ED1)&&(g_frameTx.commdata.controlindex.wIndex < 0x1FD2))//JOYCE2012-2-17 COM2 DA NEWPOWRMATCH
        {   
        	if(m_bCOM2)
	        {
	        	cmddata.wCmd   = DB_NEWPOWERMATCH_CMD_ID;
	        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1ED2 + 0x100;        	
	        }
        	else return;
        }
   		else if ((g_frameTx.commdata.controlindex.wIndex > 0x20D1)&&(g_frameTx.commdata.controlindex.wIndex < 0x21D2))//JOYCE2012-2-17 COM2 DA NEWPOWRMATCH2
        {   
        	if(m_bCOM2)
	        {
	        	cmddata.wCmd   = DB_NEWPOWERMATCH_CMD_ID;
	        	cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x20D2;
	        }
        	else return;
        }
   		else
        	return;

		  if ((g_frameTx.commdata.controlindex.wIndex == 0x3FE) || (g_frameTx.commdata.controlindex.wIndex == 0x3FD)) 	/*閥門強制輸出,使用WMACHSPECIALRESERVED46特殊參數, 2013-3-21 D_VALO*/ 
			{
			   	if(g_frameTx.commdata.controlindex.wValue == 0)
			   	{
			   		cmddata.wValue = 0x8000;
				}
			}
			else
	    cmddata.wValue = g_frameTx.commdata.controlindex.wValue;
	    cmddata.wEcho  = 0;
    	
    	PushCmd(&cmddata);
    	if (wType == TYPE_DWORD)
    	{
	    	wType = TYPE_WORD;
	    	lID++;
	    	lValue = wHValue;
	    	goto SENDAGAIN;
    	}
    }
}

void        CtmTaskMWRS232::SendControlMachine()
{
	int         nWriteCount;
	WORD		wValue;
	WORD		wLength;
	char 		cszDBID[256];
	DBVALUE		dbValue;
	
	short       i;
	//====================================================
	//	motor off key map, 91 is motoroff led's keyIndex
	//	heater off key map, 87 is heatoff led's keyIndex
	//====================================================
     //wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue & 0xFFEF | 0x8000 | 0x0010 | 0x0400 ;
     int nOff1 = _GetKeyMap(87), nOff2 = _GetKeyMap(91);
     if(((nOff1 == 0xFFFF) || (nOff1 == -1)) && ((nOff2 == 0xFFFF) || (nOff2 == -1)))
               wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue & 0xFFEF | 0x8000 | 0x0400 | 0x0080 | 0x0040; //Sunny<20070824> amend
     else 
               wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue & 0xFFEF | 0x8000 | 0x0010 | 0x0400 | 0x0080 | 0x0040; //Sunny<20070824> amend
     SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION", wValue, FALSE);
     
     SetDBValue("MACH_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
     
     //Sunny<20070824> <<<<<
     if(((nOff1 == 0xFFFF) || (nOff1 == -1)) && ((nOff2 == 0xFFFF) || (nOff2 == -1)))
               wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue & 0xFFF0 | 0x0004;
     else 
               wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue & 0xFFF0 | 0x0002 | 0x0004;
     SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);

	//if(((dbconf.wControlSequenceFlagPLC[6]&0xFF00) ==0x8400)&&((dbconf.wHydraulicType[6]&0x000F) != 0)) //Andy 20070205 for 7HT_J5 guard against theft !!
	if(((GetDBValue("CONF_CODE_WRITE_NULL_DATA6").lValue&0xFF00) ==0x8400)
	&&((GetDBValue("CONF_CODE_WRITE_NULL_W0AE6").lValue&0x000F) != 0)) //Andy 20070205 for 7HT_J5 guard against theft !!
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue | 0x0010;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}  
	//Sunny<20070824> >>>>>
	if (CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC) //James add 2008/8/13 09:42上午
	{
		//printf("SendControlMachineB() \n");
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue;
		if (GetDBValue("CONF_CODE_WRITE_NULL_DATA15").lValue >= 0x6106)
			wValue |= 0x0002;
		else
			wValue |= 0x2000;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION", wValue, FALSE);
	}
	wValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	SetDBValue("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", wValue, FALSE);
	
	//JOYCE2011-12-2 wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
	//JOYCE2011-12-2 wValue |= 0x0010;
	//JOYCE2011-12-2 SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);

	if(!m_bDC) //if ((g_pExTable->GetTableValue(0,"D_VALO")))
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue |= 0x1000;		//2012-9-18, 閥門off送0x8000  
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}

	g_frameTx.wCmd = COMM_SENDCONTROLMACHINE;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID);
	
	//James add 2008/4/28 06:54下午 
	if(1 == CtmConfig::GetInstance()->GetKeyFlag())
	{
		if (GetOilTable()) TransferOilVat(TRUE);
	}
	
	//James add 模溫機溫度資料的處理
	if (m_bCOM2)
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue;
		wValue &= (~0x2000);
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION", wValue, FALSE);
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue &= (~0x0042);
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
		memcpy(&dbValue, &g_pDatabase->Read("MACH_OTHR_OTHR_NULL_WVERSION1"), sizeof(dbValue));
		DspBufferChange(g_frameTx.commdata.acBuffer, "MACH_OTHR_OTHR_NULL_WVERSION1", dbValue,BLOCK_MACHINE_ID);
	  for ( i=0; i<20; i++) /*前面20段*/
	   	{
	   		sprintf(cszDBID, "TEMPR2_TMP2_HEATERONOFF_RAMP%d_DEGREE", i+1);
	   		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(dbValue));
	   		sprintf(cszDBID, "MACH_TMP1_HEATERONOFF_RAMP%d_DEGREE", i+1);
	   		DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MACHINE_ID);
			}
		for ( i=0; i<40; i++)/*后面40段*/
		{
				sprintf(cszDBID, "TEMPR2_TMP2_HEATERONOFF_RAMP%d_DEGREE", i+21);
	   		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(dbValue));
	   		if(i<16)
	   		sprintf(cszDBID, "MACH_OTHR_OTHR_NULL_OTHERMACHINE%d", i+1);
	   		else
	   		sprintf(cszDBID, "MACH_OTHR_OTHR_NULL_W0%3X", i+0x0390-16);	
	   		int temp=DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MACHINE_ID);
		}
		for ( i=0; i<16; i++)
	   	{
	   		sprintf(cszDBID, "ZSET2_AD1_WRITE_CHL%d_HOMEPOSITIONOFFSET", i+1);
	   		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(dbValue));
	   		sprintf(cszDBID, "MACH_AD1_WRITE_CHL%d_HOMEPOSITIONOFFSET", i+1);
	   		DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MACHINE_ID);
			}
	    
	    dbValue.wData = -1;
	    DspBufferChange(g_frameTx.commdata.acBuffer, "MACH_TMP1_HEATEROFF_WARM_DEGREE", dbValue,BLOCK_MACHINE_ID);
	    DspBufferChange(g_frameTx.commdata.acBuffer, "MACH_TMP1_HEATERONOFF_NULL_CYCLETIME", dbValue,BLOCK_MACHINE_ID);
	    DspBufferChange(g_frameTx.commdata.acBuffer, "MACH_TMP1_HEATERON_NULL_PERCENT", dbValue,BLOCK_MACHINE_ID);
	    DspBufferChange(g_frameTx.commdata.acBuffer, "MACH_ALARM_ON_NULL_MINOILTMPOFFSET", dbValue,BLOCK_MACHINE_ID);
	    DspBufferChange(g_frameTx.commdata.acBuffer, "MACH_ALARM_ON_NULL_MAXOILTMPOFFSET", dbValue,BLOCK_MACHINE_ID);
	}
	
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
	m_bMachineSend = TRUE;
}

void        CtmTaskMWRS232::SendControlMoldset()
{
	int         nWriteCount;
	WORD		wLength;
	WORD		wValue = 0;
	char 		cszDBID[256];
	int			i = 0;
	DBVALUE		dbValue;
	
	SetDBValue("MSET_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	
	g_frameTx.wCmd = COMM_SENDCONTROLMOLDSET;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID);
	if (CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC) //James add 2008/8/13 09:42上午
	{
		SelTempSendValue();
	}
	if (GetOilTable()) TransferOilVat();
	//James add 2008/7/3 模溫機對溫度的處理
	if (m_bCOM2) 
  	{
  		WORD	wMTSeg= GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED15").lValue;
  		for ( i=0; i<20; i++) /*前面20段*/
  		{
  			wValue = GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue;		//模溫開關
  			if(wValue && ((!m_bMold40) || (m_bMold40 && (i<wMTSeg))))//wangli2015-3-9 13:59:58模溫40段時，增加模溫段數判斷
  			{
  				sprintf(cszDBID, "TEMP2_TMP2_HEATERON_STEP%d_DEGREE", i+1);
  				memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(dbValue));
  				sprintf(cszDBID, "MSET_TMP1_HEATERON_CHL%d_DEGREE", i+1);
  				DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
  			}
  			else
  			{
  				sprintf(cszDBID, "MSET_TMP1_HEATERON_CHL%d_DEGREE", i+1);
  				dbValue.wData = 0;
  				DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
  			}

					//JOYCE2011-1-25 add  模溫機MOLDSET資料發送(溫度加熱速率)
  				sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_MAXTMPDEGREEOFFSET", i+1);
  				memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(dbValue));
  				sprintf(cszDBID, "MSET_ALARM_ON_CHL%d_MAXTMPDEGREEOFFSET", i+1);
  				DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
  		
  		}
  		for ( i=0; i<40; i++)/*后面40段*/
  		{
  			wValue = GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue;		//模溫開關
  			if(wValue && ((!m_bMold40) || (m_bMold40 && ((i+20)<wMTSeg))))//wangli2015-3-9 13:59:58模溫40段時，增加模溫段數判斷
  			{
  				sprintf(cszDBID, "TEMP2_TMP2_HEATERON_STEP%d_DEGREE", i+21);
  				memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(dbValue));
  				sprintf(cszDBID, "MSET_OTHR_OTHR_NULL_OTHERMACHINE%d", i+1);
  				DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
  			}
  			else
  			{
  				sprintf(cszDBID, "MSET_OTHR_OTHR_NULL_OTHERMACHINE%d", i+1);
  				dbValue.wData = 0;
  				DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
  			}	
  		}
  		for (int j=0, i=0; i<272; i++)
  		{
					int	nIndex;
					
					nIndex = g_pDatabase->GetIndex("GATE_GATE1_GATE1_NULL_MODEOPTION")+i;  
  				memcpy(&dbValue, &g_pDatabase->Read(nIndex), sizeof(dbValue));
  				nIndex = g_pDatabase->GetIndex("MSET_GATE1_GATE_NULL_MODEOPTION")+j;
  				j++;
  				if(g_pDatabase->Read(nIndex).DataType.wType == TYPE_DWORD)
  					{
  						int				nIndex2;
  						DBVALUE		dbValue2;
  						DWORD			dwTemp;
  						
  						i++;
  						nIndex2 = g_pDatabase->GetIndex("GATE_GATE1_GATE1_NULL_MODEOPTION")+i;  
  						memcpy(&dbValue2, &g_pDatabase->Read(nIndex2), sizeof(dbValue2));
  						dwTemp = dbValue2.wData;
  						dbValue.dwData = dbValue.wData + (dwTemp<<16);
  					}
  				DspBufferChange(g_frameTx.commdata.acBuffer, g_pDatabase->GetString(nIndex), dbValue,BLOCK_MOLDSET_ID);
  		}		
			for (int j=0, i=0; i<272; i++)
  		{
					int	nIndex;  				
  				nIndex = g_pDatabase->GetIndex("GATEB_GATE1_GATE1_NULL_MODEOPTION")+i;  
  				memcpy(&dbValue, &g_pDatabase->Read(nIndex), sizeof(dbValue));
  				nIndex = g_pDatabase->GetIndex("MSET_CORA_CORA_NULL_CONTROLOPTION")+j;
  				j++;
  				if(g_pDatabase->Read(nIndex).DataType.wType == TYPE_DWORD)
  					{
  						int				nIndex2;
  						DBVALUE		dbValue2;
  						DWORD			dwTemp;
  						
  						i++;
  						nIndex2 = g_pDatabase->GetIndex("GATEB_GATE1_GATE1_NULL_MODEOPTION")+i;  
  						memcpy(&dbValue2, &g_pDatabase->Read(nIndex2), sizeof(dbValue2));
  						dwTemp = dbValue2.wData;
  						dbValue.dwData = dbValue.wData + (dwTemp<<16);
  					}
  				DspBufferChange(g_frameTx.commdata.acBuffer, g_pDatabase->GetString(nIndex), dbValue,BLOCK_MOLDSET_ID);
  		}			

		for(i=0;i<40;i++)
			{
				DWORD dwValue1;
				char	pszID1[256];
				sprintf(pszID1, "MSET_OTHR_OTHR_NULL_OTHERMACHINE%d", i+41);
				g_pBlock->GetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID1, &dwValue1);
			}
    }

 		/*JOYCE2012-3-9 COM1為座台進壓力流量,COM2為閥門壓力流量*/
  	//0x710 pres
  	sprintf(cszDBID, "MSET_IJU_FWD_NULL_PRESSURE");
  	dbValue.wData = GetDBValue("MSET_GATE_GATE_NULL_W0626").lValue;
  	DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
  	//0x711 flow
  	sprintf(cszDBID, "MSET_IJU_FWD_NULL_FLOW");
  	dbValue.wData = GetDBValue("MSET_GATE_GATE_NULL_W0627").lValue;
  	DspBufferChange(g_frameTx.commdata.acBuffer, cszDBID, dbValue,BLOCK_MOLDSET_ID);
 	
 	
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendControlMonitor()
{
	int         nWriteCount;
	WORD		wLength, wOldValue1, wOldValue2;
	
	g_frameTx.wCmd = COMM_SENDCONTROLMONITOR;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID);
	if (GetOilTable())
    {
    	wOldValue1 = GetOilVat(GetDBValue("MONI_ALARM_ON_NULL_MAXOPENPOSITION").lValue);
    	wOldValue2 = GetOilVat(GetDBValue("MONI_ALARM_ON_NULL_MINOPENPOSITION").lValue);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID, "MONI_ALARM_ON_NULL_MAXOPENPOSITION", wOldValue1);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID, "MONI_ALARM_ON_NULL_MINOPENPOSITION", wOldValue2);
    }
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendControlMachineB()
{
	int         nWriteCount;
	WORD		wLength;
	
	SetDBValue("MACHB_SCRW2_RCV_NULL_CHECKDATA", 0xA596, FALSE);
	g_frameTx.wCmd = COMM_SENDCONTROLMACHINEB;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MACHINEB_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendControlMoldsetB()
{
	int         nWriteCount;
	WORD		wLength;
	
	SetDBValue("MSETB_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	g_frameTx.wCmd = COMM_SENDCONTROLMOLDSETB;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MOLDSETB_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendControlMonitorB()
{
	int         nWriteCount;
	WORD		wLength;
	
	g_frameTx.wCmd = COMM_SENDCONTROLMONITORB;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMINB_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskMWRS232::SendPowerMatch()  // St1108'01 for D/A 
{
	int         i,j;
int         nWriteCount;
WORD		wLength, wDa_flag;
WORD		wValue, wIndex, wStartID, wStartTempID;

	if(m_bDACOM2)
	{
		if (m_bNewDA2)
			{
			wStartID	 = g_pDatabase->GetDataID("PMATCH2_2_PVL1_READ_CHL1_DATA");
			wStartTempID = g_pDatabase->GetDataID("DAOUT2_2_PVL1_WRITE_CHL1_DATA");
			for ( i=0 ; i<16 ; i++)
			    {
			    wDa_flag = 0;
			        
			    for (int k=0 ; k<16 ;k++)
			    	{
			    	wIndex = g_pDatabase->DataIDToIndex(wStartID + i*16 + k);
			    	wValue = GetDBValue(wIndex).lValue;
				    if(wValue)
				    	{
				    	wDa_flag = 1;
				    	break;
						}
					}
			    if (wDa_flag == 0)
					{
					for ( j=0 ; j<16 ; j++) 
						{
						wIndex = g_pDatabase->DataIDToIndex(wStartTempID + i*16 + j);
						wValue = GetDBValue(wIndex).lValue;
						wIndex = g_pDatabase->DataIDToIndex(wStartID + i*16 + j);
						g_pDatabase->Writew(wValue, wIndex, FALSE);
						}
					}
			    }
			}
		
		
			wStartID	 = g_pDatabase->GetDataID("PMATCH_2_PVL1_READ_CHL1_DATA");
			wStartTempID = g_pDatabase->GetDataID("DAOUT_2_PVL1_WRITE_CHL1_DATA");
			for ( i=0 ; i<16 ; i++)
		    {
		    	wDa_flag = 0;
		        
		    	for (int k=0 ; k<16 ;k++)
		    	{
		    		wIndex = g_pDatabase->DataIDToIndex(wStartID + i*16 + k);
		    		wValue = GetDBValue(wIndex).lValue;
			   		if(wValue)
			    	{
			    		wDa_flag = 1;
			    		break;
					}
				}
		   	if (wDa_flag == 0)
				{
					for ( j=0 ; j<16 ; j++) 
					{
						wIndex = g_pDatabase->DataIDToIndex(wStartTempID + i*16 + j);
						wValue = GetDBValue(wIndex).lValue;
						wIndex = g_pDatabase->DataIDToIndex(wStartID + i*16 + j);
						g_pDatabase->Writew(wValue, wIndex, FALSE);
					}
				}
		    }
		
			if(m_bNewDA2)  //Andy 20061212 for New D/A
			{
				g_frameTx.wCmd = COMM_SENDPOWERMATCH2;
				wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_COM2NEWPOWERMATCH_ID);
				WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);	
			}
			else
			{
				g_frameTx.wCmd = COMM_SENDPOWERMATCH;
				wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_COM2POWERMATCH_ID);
				WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
			}
	}
	else
	{
		if(m_bCOM2)			//James add 2008/7/3 模溫機的操作
		{
			if(m_bNewDA2)  //Andy 20061212 for New D/A
			{
				g_frameTx.wCmd = COMM_SENDPOWERMATCH2;
				wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_NEWPOWERMATCH_ID);
				WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
			}   
			else
			{
				g_frameTx.wCmd = COMM_SENDPOWERMATCH;
				wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_POWERMATCH_ID);
				WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
			} 	
		}
	}
}

int        CtmTaskMWRS232::SendDiagRM(DIAGRM diagrm)
{
	int         nWriteCount;	
	m_nSendRM++;

	g_frameTx.wCmd = COMM_SENDDIAGRM;//0x3430;
	g_frameTx.commdata.diagrm.wAddressROM = diagrm.wAddressROM;
	g_frameTx.commdata.diagrm.wAddressRAM = diagrm.wAddressRAM;
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagrm)+2), &nWriteCount);
	
	return m_nSendRM;	//	fans add
}

void        CtmTaskMWRS232::SendDiagWM(DIAGWM diagwm)
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGWM;       //>>> wy070799
	g_frameTx.commdata.diagwm.wAddress 	= diagwm.wAddress;
	g_frameTx.commdata.diagwm.wData 		= diagwm.wData;
	
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0040;
    cmddata.wIndex = g_frameTx.commdata.diagwm.wAddress;
    cmddata.wValue = g_frameTx.commdata.diagwm.wData;
    cmddata.wEcho  = 0;
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
}

void        CtmTaskMWRS232::SendDiagSM()
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGSM;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendDiagRI(DIAGRI diagri)
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGRI;
	g_frameTx.commdata.diagri.wAddress = diagri.wAddress;
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagri)+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendDiagWO(DIAGWO diagwo)
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGWO;
	g_frameTx.commdata.diagwo.wAddress = diagwo.wAddress;
	g_frameTx.commdata.diagwo.wData = diagwo.wData;
	
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0041;
    cmddata.wIndex = g_frameTx.commdata.diagwo.wAddress;
    cmddata.wValue = g_frameTx.commdata.diagwo.wData;
    cmddata.wEcho  = 0;
    if(cmddata.wIndex&0100)
    	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagwo)+2), &nWriteCount);
    else
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
}

void        CtmTaskMWRS232::SendDiagWO(WORD wAddress, WORD wData)
{
	DIAGWO      diagwo;
	
	diagwo.wAddress = wAddress;
	diagwo.wData    = wData;
	SendDiagWO(diagwo);
}

void        CtmTaskMWRS232::SendDiagSI()
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGSI;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendAssignInput(WORD wOldAddress, WORD wNewAddress)
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDASSIGNINPUT;
	g_frameTx.commdata.assignio.wOldAddress = wOldAddress;
	g_frameTx.commdata.assignio.wNewAddress = wNewAddress;
	
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0042;
    cmddata.wIndex = g_frameTx.commdata.assignio.wOldAddress;
    cmddata.wValue = g_frameTx.commdata.assignio.wNewAddress;
    cmddata.wEcho  = 0;
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
}

void        CtmTaskMWRS232::SendAssignOutput(WORD wOldAddress, WORD wNewAddress)
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDASSIGNOUTPUT;
	g_frameTx.commdata.assignio.wOldAddress = wOldAddress;
	g_frameTx.commdata.assignio.wNewAddress = wNewAddress;
	
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0043;
    cmddata.wIndex = g_frameTx.commdata.assignio.wOldAddress;
    cmddata.wValue = g_frameTx.commdata.assignio.wNewAddress;
    cmddata.wEcho  = 0;
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
}

void        CtmTaskMWRS232::SendStatus(WORD wStatus)
{
	int         nWriteCount;
	WORD		wValue;
	char*		psz = "MACH_OTHR_OTHR_NULL_WERRORSTATUS1";

	wValue = GetDBValue(psz).lValue;
	wValue |= wStatus;
	
	SetDBValue(psz, wValue);
	
	g_frameTx.wCmd = COMM_SENDSTATUS;
	g_frameTx.commdata.wStatus = wStatus;
	WriteToken((BYTE*)&g_frameTx, (2+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendRProfile01()
{
}

void        CtmTaskMWRS232::SendSProfile01()
{
	int         nWriteCount;
	
	m_cStateRx &= ~COMM_STATE_WAIT01;       // Clear Request Profile flag in case of communication error ???
	m_cStateRx |=  COMM_STATE_WAIT02;
	m_lCounterW1 = 512;
	m_bSendRProfile01 = FALSE;
	
	g_frameTx.wCmd = COMM_SENDS_PROFILE01;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendRBProfile01()
{
}

void        CtmTaskMWRS232::SendSBProfile01()
{
	int         nWriteCount;
	
	m_cStateRx &= ~COMM_STATE_WAITB01;       // Clear Request Profile flag in case of communication error ???
	m_cStateRx |=  COMM_STATE_WAITB02;
	m_lCounterBW1 = 512;
	m_bSendRBProfile01 = FALSE;
	
	g_frameTx.wCmd = COMM_SENDS_BPROFILE01;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskMWRS232::SendCommand()               //St0512'03
{
	int         nWriteCount;

	g_frameTx.wCmd             = COMM_SENDCOMMAND;
	memcpy(&g_frameTx.commdata.cmddata, &m_CmddData, sizeof(m_CmddData));
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.cmddata)+2), &nWriteCount);
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmTaskMWRS232::GetColdStart()
{
	u_nMicroAdjCount= 0;					//LEO20070621
	u_nAutoAdjCount = 0;					//LEO20070621
	Echo(COMM_ECHO_SUCCESS);
	SendZeroSet();
	m_cState = COMM_STATE_RESET;
	m_cStateRx &= ~COMM_STATE_WAIT00;
    if (m_bCOM2)
    {
    	g_flHeaterStatusB = 0;
    }
	else
    {
	    g_flMotorStatus  = 0;
	    g_flHeaterStatus = 0;
    }

	if(!m_bDC) //if(g_pExTable->GetTableValue(0,"D_VALO"))
	{
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED90", 0);	//all valve opt 
		SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA46", 0);	//each valve opt 
		SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47", 0);	//each valve opt 
	}
}

void	CtmTaskMWRS232::GetWarmStart()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetConfig()
{
	short       i;
	short       j;
	DWORD		dwValue, dwValue1, dwValue2;
	char*		pszID1 = "CONF2_CODE_WRITE_NULL_DATA15";
	char*		pszID2 = "CONF2_TMP1_HEATERON_NULL_MAXDEGREE";
/*******
|       Ignore the error configuration
*******/
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_CONFIG2_ID, pszID1, &dwValue1);
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_CONFIG2_ID, pszID2, &dwValue2);
	
	if( (dwValue1 == 0) || (dwValue2 == 0)) return;   // Maximum temperature        // Version code
	
	if(m_bCOM2)			//James add 2008/7/3 如果是模溫機 則存到config2 中去
	{
		g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_CONFIG2_ID, FALSE);
	}
	Echo(COMM_ECHO_SUCCESS);

	if (!bGotConfig)    //<WY2002/11/28 
    {
	    bGotConfig = TRUE;										//wy040600
	    ::DeleteCounter((long *)&m_lCounterReqConfig, m_idMe);	//wy040600
		SendControlIndex("MACH_TMP1_HEATERON_WARM_OPTION");
		if(m_bCOM2)
			return; 
    }

/*******
|       Find 48 meaningfull points (3 groups) among 64 points of PC
*******/
	short       nOffset;
	short       nAssign;
	short       nMinAssign = 16;
	short       nMinGroup  = tmMAX_POINT_PC/16 - 1;
	WORD		wStartDataID, wDataID, wIndex;
	
	wStartDataID = g_pDatabase->GetDataID("CONF_CODE_WRITE_CHL1_OUTPUTDEFINITION");
	for (i=0; i<(tmMAX_POINT_PC/16); i++)
    {
	    nOffset = i * 16;
	    nAssign = 0;
	    wDataID = wStartDataID + nOffset;
	    for (j=0; j<16; j++)
	    {
	    	wIndex = g_pDatabase->DataIDToIndex(wDataID + j);
	    	if (!(GetDBValue(wIndex).lValue & tmASSIGN_NODEF)) nAssign++;
	    }
	    if (nMinAssign > nAssign)
        {
	        nMinAssign = nAssign;
	        nMinGroup  = i;
        }
    }

	dbgppc2[0] = 0;              //wy010902 add pc over 48 points.
	dbgppc2[1] = 1;
	dwValue	= GetDBValue("CONF_CODE_WRITE_NULL_DATA13").lValue;
	if( (dwValue == 0x280) || 
     	(dwValue == 0x2800)||                   //<<<Anny2002/6/10 add
     	(dwValue == 0x3800)||                   //<<<Steven2002/7/8 add
     	(dwValue == 0x5800) ) dbgppc2[2] = 2;    //<<<Steven2003/4/28 add
	else                      dbgppc2[2] = 3;
		dbgppc2[3] = 0;
	g_pDatabase->ComputeLimitValue(NULL);	//Yang 2006/12/29 
}

void  CtmTaskMWRS232::GetOPState()
{
	DWORD	dwValue, dwCmd, dwDataID, dwIndex, dwDBValue, dwValue2,dwValue3;
		
	dwValue = GetDBValue("STATE_OTHR_OTHR_STEP1_REALTIME").lValue;
	g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_STATE_ID, "STATE_OTHR_OTHR_STEP1_REALTIME", dwValue);
	
	if(wSendMold == 1)//JOYCE2011-4-1 在com2通訊不成功時,模溫開關從off選on,然后com2通訊OK時,send moldtemp(moldset)
	{
		SendControlMoldset();
		wSendMold = 0;
	}

	dwValue	= GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue;	//模溫選擇
	if (!dwValue)  //Gwei 2005/9/1 為了處理拔掉com2產生的error而加
  	{
  		dwValue = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
   		if(dwValue & 0x0008) //若error已存在了才去清掉它！可免得多送一次值
   		{
	    	dwValue &= 0xFFF7;
	    	//SendControlIndex("MACH_OTHR_OTHR_NULL_WERRORSTATUS1"); //Andy
	    	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue);
   		}
   		if(dwValue & 0x0100) //若error已存在了才去清掉它！可免得多送一次值
   		{
	    	dwValue &= 0xFEFF;
	    	//SendControlIndex("MACH_OTHR_OTHR_NULL_WERRORSTATUS1"); //Andy
	    	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue);
   		}
  	}
	else if (m_bCOM2)  //#define     COMM_S_COM2MOLDTEMPERROR    0x0008,特殊的4個error之一
    { 	//com2產生的error,傳給com1處理。但仍懷疑com2不通訊了為何會有error發生=>原來是插著com2時就已發生ERROR了！。
     	dwValue  = g_frameRx.commdata.state.wErrorFlag[0];//GetDBValue("STATE_ALARM_ON_STEP1_ERRORDATA").lValue;
     	dwValue2 = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
     	dwValue3 = g_frameRx.commdata.state.wErrorFlag[11];
     	if (((dwValue&0x0001)||((dwValue3&0x0200)>>9)) != ((dwValue2&COMM_S_COM2MOLDTEMPERROR)>>3)) 
        {
	        if(dwValue2&0x0008) dwValue2 &= 0xFFF7; //Gwei 2005/8/30修正
	        else     						dwValue2 |=0x0008;
	        SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue2);
        }
        //wangli2014-7-17 16:58:15
      	dwValue  = g_frameRx.commdata.state.wErrorFlag[3];//GetDBValue("STATE_ALARM_ON_STEP1_ERRORDATA").lValue;
     	dwValue2 = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
     	//if (((dwValue&0x0100)||(dwValue3==0x012F)) != ((dwValue2&0x0100)>>8)) 
     	
     	//printf("dwValue =%x\n",dwValue);
     	//printf("dwValue2 =%x\n",dwValue2);
     	if (((dwValue&0x0100)>>8) != ((dwValue2&0x0100)>>8)) 
        {
	        if(dwValue2&0x0100) dwValue2 &= 0xFEFF; 
	        else     						dwValue2 |=0x0100;
	        SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue2);
	       // printf("MACH_OTHR_OTHR_NULL_WERRORSTATUS1 =%x\n",dwValue2);
	        
        }
       
    }
	
	DWORD		dwEcho = 0;
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_COM2STATE_ID, "STATE2_OTHR_OTHR_NULL_ECHODATA", &dwEcho);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_COM2STATE_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
	if (m_cState != COMM_STATE_RUN)	m_cState = COMM_STATE_RUN;
	switch (m_wCmdState)
    {
    	case CMD_SEND:
        	if (dwEcho == m_wCmdSend) //m_wCmdSend 0x0020//Steven2003/12/12 change 0x0020 to m_wCmdSend for ms3110
            {
	            m_CmddData.wCmd = 0;
	            m_wCmdState = CMD_ECHO;
            }    
        	else if (dwEcho != 0)
            {
	            PushCmd(&m_CmddData);
	            m_wCmdSend      =
	            m_CmddData.wCmd = 0;
	            m_wCmdState = CMD_ECHO;
            }
        	break;
    	case CMD_ECHO:
        	if (dwEcho == 0)
            {
	            m_wCmdState = CMD_NONE;
	            dwDataID	= g_pDatabase->GetDataID("MACH_OTHR_OTHR_NULL_WERRORSTATUS1");
	            if ((m_wCmdSend == 0x0020)&&(m_CmddData.wIndex == dwDataID))             //Chuntzu 2004/10/12 add for new error status at 1C8
                {
                	if (m_CmddData.wValue | COMM_S_REACHSHOTCOUNT) 
                	{
	                	dwValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
	                	dwValue &= ~COMM_S_REACHSHOTCOUNT;
	                	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue, FALSE);
                	}
                	if (m_CmddData.wValue | COMM_S_REACHPACKCOUNT) 
                	{
	                	dwValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
	                	dwValue &= ~COMM_S_REACHPACKCOUNT;
	                	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue, FALSE);                	
                	}
                }
							dwDataID	= g_pDatabase->GetDataID("MACH_OTHR_OTHR_NULL_WPANELKEYCODE");
	            if ((m_wCmdSend == 0x0020) && (m_CmddData.wIndex == dwDataID))
                m_CmdPanelBreak.wCmd = 0;
            	m_wCmdSend  = 0;
            }
        	else 
            {
          		m_nCmdNull++;
            	if (m_nCmdNull>20) 
                {
	                m_nCmdNull      = 0; 
	                m_wCmdState     = CMD_NONE;
	                m_CmddData.wCmd = m_wCmdSend;
	                PushCmd(&m_CmddData);
	                m_wCmdSend      = 0;
                }
            }
        	break;
    	default:
        	break;
    }

	dwCmd			= g_frameRx.commdata.state.wCmd;
	dwDataID	= g_frameRx.commdata.state.wIndex;
	dwValue		= g_frameRx.commdata.state.wValue;
	//printf("dwCmd:%4X, dwDataID:%4X, dwValue:%4X\n", dwCmd, dwDataID, dwValue);
	if (dwCmd == DB_MACH_CMD_ID)      
    {
   	 	if (dwDataID < DB_MACH_MAXINDEX_ID)                                                       //BW20030326 for not write to unsafe address
        {
        	dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
        	dwDBValue = GetDBValue(dwIndex).lValue;
        	if (dwDBValue != dwValue)  //BW20030307 Add for data save
        	{
            	SetDBValue(dwIndex, dwValue, FALSE);
            }
        }
    }
	else if (dwCmd == DB_MOLDSET_CMD_ID)
    {
    	if (dwDataID < 0x500)                                                       //BW20030326 for not write to unsafe address
        {
	        dwDataID = dwDataID + DB_MACH_MAXINDEX_ID;
	        dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        dwDBValue = GetDBValue(dwIndex).lValue;
	        if (dwDBValue != dwValue)  //BW20030307 Add for data save
            {
	            if (GetOilTable() && CheckOilVatID(g_pDatabase->GetString(dwIndex)))
	                SetDBValue(dwIndex, OilToPosi(dwValue), FALSE);
            	else    
                	SetDBValue(dwIndex, dwValue, FALSE); 
            }
        }
    }
	else if (dwCmd == DB_MONIMAX_CMD_ID)                           //Steven 2004/2/11
    {
    	if (dwDataID < 0x61)                                                       //BW20030326 for not write to unsafe address
        {
        	if (dwDataID == 0)
        		SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", dwValue, FALSE);
        	else if (dwDataID < 0x61)
            {
	            dwDataID = dwDataID + DB_POWERMATCH_MAXINDEX_ID;
	            dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        	dwDBValue = GetDBValue(dwIndex-1).lValue;	//Why wIndex subtract 1
	            if (dwDBValue != dwValue)  //BW20030307 Add for data save
	                SetDBValue(dwIndex - 1, dwValue, FALSE);	//Why wIndex subtract 1
            }
        }
    }
	else if (dwCmd == DB_POWERMATCH_CMD_ID)                           //Steven 2004/2/11
    {
    	if (dwDataID < 0x100)                                                       //BW20030326 for not write to unsafe address
        {
	        dwDataID = dwDataID + DB_MOLDSET_MAXINDEX_ID;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        dwDBValue = GetDBValue(dwIndex).lValue;
	        if (dwDBValue != dwValue)  //BW20030307 Add for data save
	            SetDBValue(dwIndex, dwValue, FALSE);
        }
    }
	//Chuntzu 2004/7/30 add for 雙色機
	else if (dwCmd == DB_MACHB_CMD_ID)                           //Steven 2004/2/11
    {
    	if (dwDataID < 0x100)                                                       //BW20030326 for not write to unsafe address
        {
			dwDataID = dwDataID + 0x1000;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        dwDBValue = GetDBValue(dwIndex).lValue;
	        if (dwDBValue != dwValue)  //BW20030307 Add for data save
	            SetDBValue(dwIndex, dwValue, FALSE);
        }
    }
	else if (dwCmd == DB_MOLDSETB_CMD_ID)                           //Steven 2004/2/11
    {
    	if (dwDataID < 0x300)                                                       //BW20030326 for not write to unsafe address
        {
			dwDataID = dwDataID + DB_MACHB_MAXINDEX_ID;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        dwDBValue = GetDBValue(dwIndex).lValue;
	        if (dwDBValue != dwValue)  //BW20030307 Add for data save
	            SetDBValue(dwIndex, dwValue, FALSE);
        }
    }
	else if (dwCmd == DB_MONIMAXB_CMD_ID)                           //Steven 2004/2/11
    {
    	if (dwDataID < 0x030)                                                       //BW20030326 for not write to unsafe address
        {
			dwDataID = dwDataID + 0x1430;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        dwDBValue = GetDBValue(dwIndex).lValue;
	        if (dwDBValue != dwValue)  //BW20030307 Add for data save
	            SetDBValue(dwIndex, dwValue, FALSE);
        }
    }
    
	else if (dwCmd == DB_OTHER_CMD_ID)                           //Steven 2004/2/11
    {
    	if (dwDataID == 0)
        {
        	g_flMotorStatus = dwValue;
        }   
    	else if (dwDataID == 1)
        {
	        if(m_bCOM2) 		//James add 2008/7/3 模溫機的status
	        {
	        	g_flHeaterStatusB = dwValue;
	        }
					else
	        	g_flHeaterStatus  = dwValue;
        }   
    	else if (dwDataID == 2)
        {
	        dwDBValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	        SetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", dwDBValue + dwValue, FALSE);
	        dwDBValue = GetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT").lValue;
	        SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + dwValue, FALSE);
        	if (g_MoniUpdate.bChange)           //St1128'01 for moni lose problem
            {
	            dwDBValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	            g_ptmControlServer->MoniControl()->AddMoni2();
	            g_MoniUpdate.dwShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	            g_MoniUpdate.bChange = FALSE;
            }	
        	if (dbbuff.bSwitchToAuto && (GetDBValue("STATE_ALARM_ONOFF_NULL_STATUS").lValue == 0) 
        		&& (GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue != 0))  //Chuntzu 2004/9/15 add to C54
            {
            	if (++dbbuff.wSwitchToAutoShutCount >= GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue)
                {
	                dbbuff.bSwitchToAuto = FALSE;
	                SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 1, FALSE);
	                //fans add
	               	CtmQualityControl::GetInstance()->SetData(MONI_DATATYPE_LIMITUP_ID,0);
					CtmQualityControl::GetInstance()->SetData(MONI_DATATYPE_LIMITLOWER_ID,0);
	                SendControlMonitor();
	                if (CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC) //James add 2008/8/13 09:42上午
	                {
	                	SendControlMonitorB();
					}
                }
            }
        }
    	else if (dwDataID == 4)
        {
			g_wOperationCount = dwValue;//g_frameTx.cmddata.wValue;  	//<Sunny20070327> 操作計數
        }
    }
	if (m_wCmdState == CMD_NONE) 
    {
    	if (m_CmdPanelBreak.wCmd == 0x20)
        {
	        m_CmddData  = m_CmdPanelBreak;
	        m_wCmdSend  = m_CmddData.wCmd;        //Steven 2003/12/12 add
	        m_wCmdState = CMD_SEND;
	        m_nCmdNull  = 0;
        }    
    	else if (PopCmd(&m_CmddData)) 
        {
	        m_wCmdSend  = m_CmddData.wCmd;        //Steven 2003/12/12 add
	        m_wCmdState = CMD_SEND;
	        m_nCmdNull  = 0;
        }
    }
	m_CmddData.wEcho = dwCmd;
	SendCommand();
	
	//James add 2008/3/14 04:29下午
	DWORD lInjeTimeValue = 0;
	long long lInjeState = GetDBValue("STATE_OTHR_OTHR_NULL_COMMANDDATA").lValue;
	if(lInjeState == 0x0011)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[0] = (lInjeTimeValue << 16)|(g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if(lInjeState == 0x0012)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[1] = (lInjeTimeValue << 16)|(g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);	
	}
	else if(lInjeState == 0x0013)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[2] = (lInjeTimeValue << 16)|(g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);	
	}
	else if(lInjeState == 0x0014)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[3] = (lInjeTimeValue << 16)|(g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);	
	}
	else if(lInjeState == 0x0015)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[4] = (lInjeTimeValue << 16)|(g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);	
	}
	else if(lInjeState == 0x0016)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[5] = (lInjeTimeValue << 16)|(g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);	
	}
	else if(lInjeState == 0x0010)
	{
		for(int i = 0; i < 6; i++)
		{
			m_dwInjectTime[i] = 0;	
		}
	}
	
	if (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue !=ID_VIEW_PANEL)     //BW0211'03 add for LED test
    {
    	if (dbbuff.wOperationMode != GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue)
        {
        	dbbuff.wOperationMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;
        	if (dbbuff.wOperationMode)
            {
                if( _TestBit(dbbuff.wOperationMode, 0) &&   
            		(dbbuff.cIndexMode == (char)tmMODE_MANUAL))   //Sunny<20070909> amend 0表示時間自動
                {
                	dbbuff.bSwitchToAuto = TRUE;
	                dbbuff.wSwitchToAutoShutCount = 0;
                }
                dbbuff.cIndexMode = _ScanBitR(dbbuff.wOperationMode & 0x0FFF);/*JOYCE2012-6-19 過濾高4個bit-非動作狀態*/
                
               	if (dbbuff.cIndexMode == 1) PanelSemiAutoOn();
                else if (dbbuff.cIndexMode == 2) PanelSensorAutoOn();
                else if (dbbuff.cIndexMode == 3) 
                {
                		PanelTimeAutoOn();
                }
                else if (dbbuff.cIndexMode == 4) 	PanelAutoAdjOn(); 	
                	
                else if (dbbuff.cIndexMode == 11)
                {
                	u_nAutoAdjCount = 1;	//LEO20070621
                	g_bLEDAdjMold = FALSE;
	                g_lCounterLEDAdjMold = 0;
	            }
            }
        	else
            {
            	dbbuff.cIndexMode = (char)tmMODE_MANUAL;
            	dbbuff.bSwitchToAuto = FALSE;
            	PanelManualOn();
            }
        }
		if (dbbuff.flHeaterStatus != g_flHeaterStatus)
        {
        	dbbuff.flHeaterStatus = g_flHeaterStatus;
        	if(g_flHeaterStatus)	PanelHeaterOn();
        	else					PanelHeaterOff();
        }
		if (dbbuff.flMotorStatus != g_flMotorStatus)           //WY071299 ADD 
       	{
       		dbbuff.flMotorStatus = g_flMotorStatus;
        	if(g_flMotorStatus == 0xFFFF )   //WY030599
            {
            	PanelMotorOn();
            }
        	else if (g_flMotorStatus == 0)  //ChunTzu 2004/3/19
            {
            	PanelMotorOff();
            }
        	else 
            {
            	g_bLEDMotor = FALSE;
            	PanelMotorOff();
            }                                             //>>> wy081299
       }
    }
    
    if(m_bCOM2)
    {
	   //printf("g_flHeaterStatus:%d, g_flHeaterStatusB:%d \n",g_flHeaterStatus,g_flHeaterStatusB);
	   if(g_flHeaterStatus != g_flHeaterStatusB)
	    {
	    	WORD  wKeycode; 
		    if(g_flHeaterStatus == 0xFFFF)
		    {
		    	SendPanel((BYTE)(LOBYTE(wKeycode = _KEY_PANEL_HEATER)));
		    	usleep(10*1000);
		    }
		    else if(g_flHeaterStatus == 0)
		    {
		   	 	SendPanel((BYTE)(LOBYTE(wKeycode = _KEY_PANEL_HEATEROFF)));
		   	 	usleep(10*1000);
		   	}
	    }
		}
}

void	CtmTaskMWRS232::GetOPSummary()                 //St020101 for add update speed
{
	if (m_bCOM2)			//James add 2008/7/3 模溫機無此項操作
    {
    	Echo(COMM_ECHO_SUCCESS); 
    	return;
    }
}

void	CtmTaskMWRS232::GetCurveData()
{
	if (m_bCOM2)			//James add 2008/7/3 模溫機無此項操作
    {
    	Echo(COMM_ECHO_SUCCESS); 
    	return;
    }	
}

void  CtmTaskMWRS232::GetOPStatus()
{
	short       i = 0;
	DWORD		dwValue = 0, dwOffset = 0;
	char		cszDBID[256];
	DBVALUE		dbValue;

	if (m_bCOM2) 
    {
		if (GetDBValue("CONF2_CODE_WRITE_NULL_DATA16").lValue < 0x8215 && (GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue <= 0x2008))
    	{
    		/*for ( i=0; i<4; i++)  //中間20段後4是Da值？Gwei
	        {
	        	sprintf(cszDBID, "STATUS_PVL%d_WRITE_NULL_REALDATA", i+1);
	        	g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_STATUS_ID, cszDBID, dwValue);
	        	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", 37+i);
	        	SetDBValue(cszDBID, dwValue, FALSE);
	        	
	        	sprintf(cszDBID, "STATUS_PVL%d_WRITE_NULL_REALDATA", i+5);
	        	g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_STATUS_ID, cszDBID, dwValue);
	        	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", 37+i);
	        	SetDBValue(cszDBID,dwValue, FALSE);
	        }	*/

			for ( i=0; i<4; i++)  //中間20段後4是Da值？Gwei
		    {
        		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", 37+i);
        		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        		memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        		dwOffset+=dbValue.DataType.wLength;
        		SetDBValue(cszDBID, dbValue.wData, FALSE);
				}

			for ( i=0; i<4; i++)  //中間20段後4是Da值？Gwei
		    {
        		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", 37+i);
        		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        		memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        		dwOffset+=dbValue.DataType.wLength;
        		SetDBValue(cszDBID, dbValue.wData, FALSE);
		    }
      	}   
    }
    else
    {
		g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATUS_ID, FALSE);
	}
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetOPMeter()
{
	short       i;
	DWORD		dwValue = 0, dwOffset = 0;
	char 		cszDBID[256];
	DBVALUE		dbValue;

	if (m_bCOM2) 			//James add 模溫機使用
  {
		if (GetDBValue("CONF2_CODE_WRITE_NULL_DATA16").lValue < 0x8215 && (GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue <= 0x2008))
			{
			/*for( i=0; i<16; i++)  //中間20段是位置尺值？Gwei
		    {
	    	sprintf(cszDBID, "METER_AD1_READ_CHL%d_RELATIVEPOSITION", i+1);
		    	g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_METER_ID, cszDBID, dwValue);
		    	
		    	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", i+21);
		    	SetDBValue(cszDBID, dwValue, FALSE);
		    	
		    	sprintf(cszDBID, "METER_AD1_READ_CHL%d_RELATIVEPOSITION", i+1);
		    	g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_METER_ID, cszDBID, dwValue);
		    	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", i+21);
		    	SetDBValue(cszDBID, dwValue, FALSE);
		    }
			*/	
			for( i=0; i<16; i++)  //中間20段是位置尺值？Gwei
		    {
        		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", i+21);
        		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        		memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        		dwOffset+=dbValue.DataType.wLength;
        		SetDBValue(cszDBID, dbValue.wData, FALSE);
				}

			for( i=0; i<16; i++)  //中間20段是位置尺值？Gwei
		    {
        		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", i+21);
        		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        		memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        		dwOffset+=dbValue.DataType.wLength;
        		SetDBValue(cszDBID, dbValue.wData, FALSE);
		    }
			}
    else
			g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_COM2METER_ID, FALSE); 
	}
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetOPMeterZeroAgain()
{
	WORD	wValue = GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue;

	Echo(COMM_ECHO_SUCCESS);
	SetDBValue("MACH_AD1_WRITE_CHL2_HOMEPOSITIONOFFSET", wValue, FALSE);
	SendZeroSet();
}

void  CtmTaskMWRS232::GetOPTemperature()
{
	short       i;
	DWORD		dwValue = 0, dwOffset = 0;
	char		cszDBID[256];
	DBVALUE		dbValue;
	if (m_bCOM2) 				//James add 2008/7/3 模溫機溫度的處理
    {
    	for ( i=0; i<20; i++)  //前面20段是溫度實際值？Gwei
        {
        	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", i+1);
        	memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        	memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        	dwOffset+= dbValue.DataType.wLength;
        	SetDBValue(cszDBID, dbValue.wData, FALSE);
        }
        for(i = 0; i < 20; i++)
       	{
       		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", i+1);
        	memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        	memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        	dwOffset+= dbValue.DataType.wLength;
        	SetDBValue(cszDBID, dbValue.wData, FALSE);	
       	}
    }
	Echo(COMM_ECHO_SUCCESS);
	//=======
	// 發送消息，有溫度收到//netdigger 2005/12/3
	//=======
	if(g_lCounterTemp <= 0)
	{
		MSG	msg;
		
		msg.message	= MSG_TEMP;
		g_pApplication->QueueMessage(&msg);
		g_lCounterTemp	= COMM_TIME_TEMPER;
	}
}

void  CtmTaskMWRS232::GetControlIndex()
{
	if (m_bCOM2)				//James add 2008/7/3 模溫機出做此操作
    {
    	Echo(COMM_ECHO_SUCCESS); 
    	return;
    }
}

void  CtmTaskMWRS232::GetProduction()
{
	if (m_bCOM2)				//James add 2008/7/3 模溫機出做此操作
    {
    	Echo(COMM_ECHO_SUCCESS); 
    	return;
    }
}

void  CtmTaskMWRS232::GetDAOutput()     // St1108'01 for D/A
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_COM2DAOUT_ID);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_COM2DAOUT2_ID);
	Echo(COMM_ECHO_SUCCESS);
	m_bGotDA = TRUE;
	m_bNewDA2 = FALSE;  
	if (m_cState != COMM_STATE_RUN) 
	    SendControlMachine();
	else
	    SendPowerMatch();
}

void CtmTaskMWRS232::GetDAOutput2()     // Andy 20061212 for New D/A
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_COM2NEWDAOUT_ID);
	Echo(COMM_ECHO_SUCCESS);
	m_bGotDA = TRUE;
	m_bNewDA2 = TRUE;  //Andy 20070314
	if (m_cState != COMM_STATE_RUN) 
	    SendControlMachine();
	else
	    SendPowerMatch();
}

void  CtmTaskMWRS232::GetMoniCurrent()
{
	short 		i;
	DWORD		dwValue = 0, dwOffset = 0;
	char 		cszDBID[512];
	DBVALUE		dbValue;
	if (m_bCOM2) 		//James add 2008/7/3 模溫機監測的操作
    {
    	for ( i=0; i<20; i++) //後面20段是？Gwei
        {
        	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", i+41);
        	memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        	memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
       		dwOffset+=dbValue.DataType.wLength;
        	SetDBValue(cszDBID, dbValue.wData, FALSE);
        }
        for(i = 0; i < 20; i++)
        {
        	sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", i+41);
        	memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        	memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        	dwOffset+=dbValue.DataType.wLength;
        	SetDBValue(cszDBID, dbValue.wData, FALSE);
        }
    }
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetMoniLast()
{
	DWORD		dwValue = 0, dwOffset = 0;
	short       i;
	char 		cszDBID[512];
	DBVALUE		dbValue;
	
	if (m_bCOM2)		//James add 2008/7/3 模溫機監測的操作
    {
    	if (GetDBValue("CONF2_CODE_WRITE_NULL_DATA16").lValue >= 0x8215 && (GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue >= 0x2008) || (GetDBValue("CONF2_CODE_WRITE_NULL_MCHINEDATA1").lValue >= 0x2009))
    	{
    		for ( i=0; i<20; i++)  //中間20段是位置尺值？Gwei
    		{
        		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALDEGREE", i+21);
        		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        		memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        		dwOffset+=dbValue.DataType.wLength;
        		SetDBValue(cszDBID, dbValue.wData, FALSE);
    		}
    		for( i = 0; i < 20; i++)
    		{
        		sprintf(cszDBID, "TEMP_TMP2_HEATERONOFF_CHL%d_REALSTATUS", i+21);
        		memcpy(&dbValue, &g_pDatabase->Read(cszDBID), sizeof(DBVALUE));
        		memcpy(dbValue.acData, (g_frameRx.commdata.acBuffer + dwOffset), dbValue.DataType.wLength);
        		dwOffset+=dbValue.DataType.wLength;
        		SetDBValue(cszDBID, dbValue.wData, FALSE);
    		}
    	}     
    	Echo(COMM_ECHO_SUCCESS); 
    	return;	
	}
	if (GetOilTable())
	{
	    g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_MONILAST_ID, "MONI_MLD_OPEN_NULL_LASTPOSITION", &dwValue);
	    SetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION", OilToPosi(dwValue), FALSE);
    }
	Echo(COMM_ECHO_SUCCESS);

	#ifndef   D_DC
	dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	if (dwValue == g_MoniUpdate.dwShotCount) //St1128'01  for moni lose problem
    {
    	g_MoniUpdate.bChange = TRUE;
    }    
	else
    {
	    if ((dwValue % GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue) == 0)
	    	g_ptmControlServer->MoniControl()->AddMoni2();  
	    g_MoniUpdate.dwShotCount = dwValue;
	    g_MoniUpdate.bChange = FALSE;
    }    
	#endif
}

void  CtmTaskMWRS232::GetMoniCurrentB()
{
	DWORD		dwValue;

	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONIBCURRENT_ID, FALSE);
	if (GetOilTable())
	{
		dwValue = GetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION").lValue;
    	SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", OilToPosi(dwValue), FALSE);    
    }
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetMoniLastB()
{
	DWORD		dwValue;
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONIBLAST_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);

	dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	if (dwValue == g_MoniUpdate.dwShotCount) //St1128'01  for moni lose problem
    {
    	g_MoniUpdate.bChange = TRUE;
    }    
	else
    {
    	if ((dwValue % GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue) == 0)
    		g_ptmControlServer->MoniControl()->AddMoni2(); 
   	 	g_MoniUpdate.dwShotCount = dwValue;
    	g_MoniUpdate.bChange = FALSE;
    }    
}

void  CtmTaskMWRS232::GetMoniMaxMinB()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONIMAXMINB_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetDiagRM()
{
	m_nRevRM++;
	if (m_bCOM2)
	{
		memcpy((char *)&m_dbdiagrm, g_frameRx.commdata.acBuffer, sizeof(tmDB_DIAGRM));
	}
	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	msg.message	= MSG_DSP54_REVRM;	
	msg.lParam	= m_nRevRM;
	g_pApplication->QueueMessage(&msg);

	Echo(COMM_ECHO_SUCCESS);
	SendDiagSM();
}

void  CtmTaskMWRS232::GetDiagRI()
{
	if (m_bCOM2)
	{
		memcpy((char *)&m_dbdiagri, g_frameRx.commdata.acBuffer, sizeof(tmDB_DIAGRI));
	}
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetSProfile01I()
{
	ClearChartInjectCount();
	ClearChartHoldCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetDProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetEProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetSProfile01H()
{
	ClearChartHoldCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetDProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetEProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetSBProfile01I()
{
	ClearChartInjectBCount();
	ClearChartHoldBCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetDBProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetEBProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetSBProfile01H()
{
	ClearChartHoldBCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetDBProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskMWRS232::GetEBProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void	CtmTaskMWRS232::GetEcho()
{
	if(g_frameRx.wCmd == COMM_ECHO_FAILURE)
	{
		switch (g_frameTx.wCmd)
		    {
		    case COMM_SENDCONFIG:
		    		SendConfig();
		    		break;
		    case COMM_SENDZEROSET:
		    		SendZeroSet();
		    		break;
		    case COMM_SENDCONTROLMACHINE:
		        SendControlMachine();
		        break;
		    case COMM_SENDCONTROLMOLDSET:
		        SendControlMoldset();
		        break;
		    case COMM_SENDPOWERMATCH:              //St1114'01
		    case COMM_SENDPOWERMATCH2:     //Andy 20061212 for New D/A
		        SendPowerMatch();
		        break;
		    case COMM_SENDCONTROLMONITOR:
		        SendControlMonitor();
		        break;
		    case COMM_SENDCONTROLMONITORB:
		        SendControlMonitorB();
		        break;
		    case COMM_SENDCONTROLMACHINEB:
		        SendControlMachineB();
		        break;
		    case COMM_SENDCONTROLMOLDSETB:
		        SendControlMoldsetB();
		        break;
		    default:
		        break;
		    }	
	}	
	else if(g_frameRx.commdata.wResult > 0)
	{
		switch (g_frameRx.wCmd)
		    {
		    case COMM_SENDCONFIG:
		    		SendConfig();
		    		break;
		    case COMM_SENDZEROSET:
		    		SendZeroSet();
		    		break;
		    case COMM_SENDCONTROLMACHINE:
		        SendControlMachine();
		        break;
		    case COMM_SENDCONTROLMOLDSET:
		        SendControlMoldset();
		        break;
		    case COMM_SENDPOWERMATCH:              //St1114'01
		    case COMM_SENDPOWERMATCH2:     //Andy 20061212 for New D/A
		        SendPowerMatch();
		        break;
		    case COMM_SENDCONTROLMONITOR:
		        SendControlMonitor();
		        break;
		    case COMM_SENDCONTROLMONITORB:
		        SendControlMonitorB();
		        break;
		    case COMM_SENDCONTROLMACHINEB:
		        SendControlMachineB();
		        break;
		    case COMM_SENDCONTROLMOLDSETB:
		        SendControlMoldsetB();
		        break;
		    default:
		        break;
		    }	
	}
	else
	{
		switch (g_frameRx.wCmd)
	    {
	    	case COMM_SENDCONTROLMACHINE:
		        SendControlMoldset();
		        break;
		    case COMM_SENDCONTROLMOLDSET:
		        if (m_bGotDA) SendPowerMatch();    //St1114'01
		        else          SendControlMonitor();
		        break;
		    case COMM_SENDPOWERMATCH:              //St1114'01
		    case COMM_SENDPOWERMATCH2:     //Andy 20061212 for New D/A
		    	if (m_cState != COMM_STATE_RUN)	//	fans add 2007/9/29 10:48上午
		    	{
			        m_bGotDA = FALSE;
			        SendControlMonitor();
			    }
		        break;
		    case COMM_SENDCONTROLMONITOR:
		        m_cState = COMM_STATE_RUN;
		        if (m_bSendRProfile01)  SendRProfile01();
	        	break;
	    	case COMM_SENDPANELBREAK:
		        m_cStateRx &= ~COMM_STATE_WAIT00;
		        break;
		    case COMM_SENDR_PROFILE01:
		        m_cStateRx &= ~COMM_STATE_WAIT01;
		        break;
		    case COMM_SENDS_PROFILE01:
		        m_cStateRx &= ~COMM_STATE_WAIT02;
		        break;
		    default:
		        break;
	    }
	}
}

void	CtmTaskMWRS232::Echo(WORD wCode)
{
	int         nWriteCount;
	
	g_frameRx.commdata.wResult = wCode;
	WriteToken((BYTE*)&g_frameRx, (2+2), &nWriteCount);
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
//void	CtmTaskMWRS232::Reset()
//{
//	m_nRevRM 	= 0;
//	m_nSendRM 	= 0;
//	if (m_prs232 == NULL) return;
//
//	m_prs232->Close();
//	/*if (!bGotConfig)*/ m_prs232->Change_BaudRate();	//20071128 Edision mark
//	m_prs232->Open();
//}

void	CtmTaskMWRS232::ResetClose()//JOYCE2011-1-27 解決com2通訊有時需要60s左右才連上的問題
{
	m_nRevRM 	= 0;
	m_nSendRM 	= 0;
	if (m_prs232 == NULL) return;

	m_prs232->Close();
	 m_prs232->Change_BaudRate();
	m_lCounterClsOpn = 2*1024;
	m_bResetClsOpn = TRUE;
}

void	CtmTaskMWRS232::ResetOpen()//JOYCE2011-1-27 解決com2通訊有時需要60s左右才連上的問題 
{
	m_nRevRM 	= 0;
	m_nSendRM 	= 0;
	if (m_prs232 == NULL) return;

	m_prs232->Open();
	m_bResetClsOpn = FALSE;
}

void     CtmTaskMWRS232::SelTempSendValue()
{
	short       i;
	WORD		wIndex, wDataID, wTempControl;
	char*		pszID;

	wTempControl = GetDBValue("SYSX_TEMPER_HEATERON_NULL_DUALOPTION").lValue;
	if (!((wTempControl + 1) & 0x0001))
	{
		wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
		for (i=0; i<6; i++)	
		{
			wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
		}
	}
	if (!((wTempControl + 1) & 0x0002))
	{
		wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL11_DEGREE");
		for (i=0; i<6; i++)	
		{
			wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
		}
	}
}

BOOL     CtmTaskMWRS232::CheckOilVatID(char* pszData)
{
	short       i;

	if(1 == CtmConfig::GetInstance()->GetKeyFlag())
	{
		for (i=0; i < (int)(sizeof(g_apszVatTransMachID) / sizeof(g_apszVatTransMachID[0])); i++)
	     	if (strcmp(pszData, g_apszVatTransMachID[i]) == 0) return TRUE;
	}
	
	for (i=0; i < (int)(sizeof(g_apszVatTransID) / sizeof(g_apszVatTransID[0])); i++)
     	if (strcmp(pszData, g_apszVatTransID[i]) == 0) return TRUE;

	return FALSE;
}

void     CtmTaskMWRS232::TransferOilVat()
{
	short       i;
	DWORD		dwOilVat;

	for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
    {
     	g_pBlock->GetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, g_apszVatTransID[i], &dwOilVat);
     	g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, g_apszVatTransID[i], GetOilVat((int)dwOilVat));
    }
}

void     CtmTaskMWRS232::TransferOilVat(BOOL bTransferMach)//Andy 20070520 add;
{
	short       i;
  
	if(bTransferMach)
	{
		for (i=0; i<(sizeof(g_apszVatTransMachID)/sizeof(g_apszVatTransMachID[0])); i++)
	    {
	     	if(i==0)
	     	{
	      		if(GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue != 0xFFFF)
	      		{
	      			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer,BLOCK_MACHINE_ID , g_apszVatTransMachID[i], 
	      				GetOilVat((int)(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue - GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue)));
	    		}
	      		else return;
			}
	     }
	}

	else  TransferOilVat();	
}

void     CtmTaskMWRS232::SendOilVat()
{
	short       i;

	for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)    
    	SendControlIndex(g_apszVatTransID[i]);
}

void     CtmTaskMWRS232::SetVatDefPosi()
{
	short       i;
	WORD		wValue;
	for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
    {
    	wValue = GetDBValue(g_apszVatTransID[i]).lValue;
    	SetDBValue(g_apszVatTransID[i], OilToPosi(wValue), FALSE);
    }
}

void  CtmTaskRS232::LED()//modified by leo20071129
{
	m_tled.lCount = TLED_SLICE;
	_Save_Restore_LED();
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
BOOL        CtmTaskMWRS232::PushCmd(CMDDATA* pcmd)
{
	if (!m_CmdQueue.bQueueFull)
	{
	    m_CmdQueue.CmdQueue  [m_CmdQueue.wInput]   = pcmd->wCmd;
	    m_CmdQueue.IndexQueue[m_CmdQueue.wInput]   = pcmd->wIndex;
	    m_CmdQueue.ValueQueue[m_CmdQueue.wInput++] = pcmd->wValue;
	    if (m_CmdQueue.wInput >= m_CmdQueue.wQueueEnd)
	        m_CmdQueue.wInput = 0;
	    if (m_CmdQueue.wInput == m_CmdQueue.wOutput)
	        m_CmdQueue.bQueueFull = TRUE;
	    return TRUE;
	}
	
	return FALSE;
}

BOOL	CtmTaskMWRS232::PopCmd(CMDDATA* pcmd)
{
	if ((m_CmdQueue.wInput != m_CmdQueue.wOutput) || (m_CmdQueue.bQueueFull))
	{
	    pcmd->wCmd   = m_CmdQueue.CmdQueue  [m_CmdQueue.wOutput];
	    pcmd->wIndex = m_CmdQueue.IndexQueue[m_CmdQueue.wOutput];
	    pcmd->wValue = m_CmdQueue.ValueQueue[m_CmdQueue.wOutput++];
	    if (m_CmdQueue.wOutput >= m_CmdQueue.wQueueEnd) m_CmdQueue.wOutput = 0;
	    m_CmdQueue.bQueueFull = FALSE;
	    return TRUE;
	}
	
	return FALSE;
}

void	CtmTaskMWRS232::WriteToken(BYTE* pch, int nCount, int* pnWriteCount)
{
	if (m_prs232 != NULL) m_prs232->WriteToken(pch, nCount, pnWriteCount);
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
BOOL	CtmTaskMWRS232::AddChartInject(tmDB_CHARTSTROKE1* pdbchartstroke1)
{
	if (m_dbchartInject.nIndex < tmMAX_STROKE)
    {
    	m_dbchartInject.nX [m_dbchartInject.nIndex]   = pdbchartstroke1->nX;
    	m_dbchartInject.nY1[m_dbchartInject.nIndex]   = pdbchartstroke1->nY1;
    	m_dbchartInject.nY2[m_dbchartInject.nIndex++] = pdbchartstroke1->nY2;
    }

	return TRUE;
}

BOOL	CtmTaskMWRS232::AddChartHold  (tmDB_CHARTSTROKE1* pdbchartstroke1)
{
	if (m_dbchartHold.nIndex == 0)
    	m_dbchartHold.nX0 = pdbchartstroke1->nX;

	if (m_dbchartHold.nIndex < tmMAX_STROKE)
    {
    	m_dbchartHold.nX [m_dbchartHold.nIndex]   = pdbchartstroke1->nX - m_dbchartHold.nX0;
    	m_dbchartHold.nY1[m_dbchartHold.nIndex]   = pdbchartstroke1->nY1;
    	m_dbchartHold.nY2[m_dbchartHold.nIndex++] = pdbchartstroke1->nY2;
    }

	return TRUE;
}

BOOL	CtmTaskMWRS232::AddChartInjectB(tmDB_CHARTSTROKE1* pdbchartstroke1)
{
	if (m_dbchartInjectB.nIndex < tmMAX_STROKE)
    {
    	m_dbchartInjectB.nX [m_dbchartInjectB.nIndex]   = pdbchartstroke1->nX;
    	m_dbchartInjectB.nY1[m_dbchartInjectB.nIndex]   = pdbchartstroke1->nY1;
    	m_dbchartInjectB.nY2[m_dbchartInjectB.nIndex++] = pdbchartstroke1->nY2;
    }

return TRUE;
}

BOOL	CtmTaskMWRS232::AddChartHoldB  (tmDB_CHARTSTROKE1* pdbchartstroke1)
{
	if (m_dbchartHoldB.nIndex == 0)
    	m_dbchartHoldB.nX0 = pdbchartstroke1->nX;

	if (m_dbchartHoldB.nIndex < tmMAX_STROKE)
    {
    	m_dbchartHoldB.nX [m_dbchartHoldB.nIndex]   = pdbchartstroke1->nX - m_dbchartHoldB.nX0;
    	m_dbchartHoldB.nY1[m_dbchartHoldB.nIndex]   = pdbchartstroke1->nY1;
    	m_dbchartHoldB.nY2[m_dbchartHoldB.nIndex++] = pdbchartstroke1->nY2;
    }

	return TRUE;
}

BOOL   CtmTaskMWRS232::AddChartCharge(tmDB_CHARTSTROKE1* pdbchartstroke1)		//Sunny<20070815>
{
	if (m_dbchartChrg.nIndex < tmMAX_STROKE)
    {
    	m_dbchartChrg.nX [m_dbchartChrg.nIndex]   = pdbchartstroke1->nX;
    	m_dbchartChrg.nY1[m_dbchartChrg.nIndex]   = pdbchartstroke1->nY1;
    	m_dbchartChrg.nY2[m_dbchartChrg.nIndex++] = pdbchartstroke1->nY2;
    }
	return TRUE;
}

DWORD	CtmTaskMWRS232::GetInjectTime(int nIndex)
{
	if(nIndex >= 6)
	{
		return	m_dwInjectTime[5];
	}
	else if(nIndex < 0)
	{
		return	m_dwInjectTime[0];	
	}
	else
	{
		return 	m_dwInjectTime[nIndex];
	}
}

//將相關之放入指定的buffer中 //Andy 20100331 優化for循環
int		DspBufferChange(BYTE* pcData, char* cszDBID, DBVALUE SetdbValue, WORD wBlockID)
{
	WORD	wOffset = 0, wElement, wDBIndex, wDBIndex2;
	int		i;
	BLOCKDATA*		pBlockData;
	DBVALUE	dbValue;
		
	if ((pBlockData	= g_pBlock->GetBlockData(wBlockID)) == NULL)
		return 0;
	if ((wElement	= pBlockData->dwFactorTotal) <= 0)	return 0;
	
	wDBIndex2 = g_pDatabase->GetIndex(cszDBID);
	long	lID	= g_pDatabase->IndexToDataID(wDBIndex2);
				if(wBlockID == BLOCK_MACHINE_ID)	wOffset		= lID*2;
	else	if(wBlockID == BLOCK_MOLDSET_ID)	wOffset		= (lID-0x400)*2;
	else	return 0;
		wDBIndex = pBlockData->pwDBIndex[i];
		dbValue = g_pDatabase->Read(wDBIndex2);
		
			if (dbValue.DataType.wType != TYPE_STRING)
			{
				memcpy((pcData + wOffset), SetdbValue.acData, dbValue.DataType.wLength);
			}
			else
				strncpy((char *)(pcData + wOffset), SetdbValue.pcData, dbValue.DataType.wLength);
	return wOffset;//+sizeof(WORD);
}

/*--------------------------------------------------------------------------------------+
|  Function : WriteData(WORD wCommID)  												    |
|  Task     : The unified interface to send data ,invoked not as the respondence to DSP |
|             command but  in  views when needed.                                       |
+---------------------------------------------------------------------------------------|
|  Call     : 	 CRS232::WriteValue(nDataID)                        					|
|                                              		         							|
|                                                                            			|
|  Parameter: wCommID                   - Indicate the type of function           		|
|             							          	                        			|
|  Return   :         																	|
+---------------------------------------------------------------------------------------*/
int		CtmTaskMWRS232::WriteValue(int nDataID)
{
	WORD	wType;
	wType = nDataID;
	
	switch(wType)
	{
		case COMM_SENDCONFIG:		
			SendConfig();
			break;
		case COMM_SENDZEROSET:
			SendZeroSet();
			break;
		case COMM_SENDDIAGSI:								
			SendDiagSI();
			break;
		case COMM_SENDPOWERMATCH:
		case COMM_SENDPOWERMATCH2:
			SendPowerMatch();
			break;
		case COMM_SENDCONTROLMACHINE:
	    SendControlMachine();    
	    break;
	  case COMM_SENDCONTROLMOLDSET:
	  	if(g_wDsp2Status == 0)		//JOYCE2011-4-1 在com2通訊不成功時,模溫開關從off選on,然后com2通訊OK時,send moldtemp(moldset)
	  		wSendMold = 1;
	  	SendControlMoldset();
	  	break;
	  case COMM_SENDDIAGSM:
	  	SendDiagSM();
	  	break;
	  case COMM_BPROFILE01_R:
	  	SendRProfile01();
	  	break;
	  case COMM_BPROFILE01_S:
	  	SendSProfile01();
	  	break;
	  case COMM_SENDCONTROLMONITOR:
	  	SendControlMonitor();
	  	break;
	  case COMM_INIT_MESSAGE:
	  	InitMessage();
	  	break;
	  default:
	  	break;
	 }
	 return 1;
}

int		CtmTaskMWRS232::WriteValue(int nType, int nHostAdreess1, int nHostAdreess2)
{
	switch(nType)
	{
		case COMM_SENDSTATUS:
			SendStatus(nHostAdreess1);
			break;
		case  COMM_SENDCONTROLINDEX:
 			SendControlIndex(nHostAdreess1);
 			break;
 		case COMM_SENDPANEL:
			SendPanel(LOBYTE(nHostAdreess1));
			break;
		case COMM_SENDDIAGWO:
			SendDiagWO(nHostAdreess1,nHostAdreess2);					
			break;
		case COMM_SENDASSIGNINPUT:
			SendAssignInput(nHostAdreess1, nHostAdreess2);
			break;
		case COMM_SENDASSIGNOUTPUT:
			SendAssignOutput(nHostAdreess1, nHostAdreess2);
			break;
		default:
	   		break;
	}
	return 1;
}
	
int		CtmTaskMWRS232::WriteValue(int nType, void* pInputData, int nSize)
{
	if (m_cState == COMM_STATE_RUN)
	switch(nType)
	{
		case COMM_SENDDIAGRI:
			if (pInputData != NULL)
			{
				DIAGRI	Diagri;	
				if(nSize == 0)
					memcpy(&Diagri, pInputData ,sizeof(Diagri));
				else
					memcpy(&Diagri, pInputData ,nSize);
				SendDiagRI(Diagri);
			}
			break;
		case COMM_SENDDIAGWO:
			if (pInputData != NULL)
			{
				DIAGWO	diagwo;
				if(nSize == 0)
					memcpy(&diagwo, pInputData ,sizeof(diagwo));
				else
					memcpy(&diagwo, pInputData ,nSize);
				SendDiagWO(diagwo);
			}
			break;		
		case COMM_SENDDIAGRM:
		  	if (pInputData != NULL)
			{
				DIAGRM	diagrm;
				if(nSize == 0)
					memcpy(&diagrm, pInputData ,sizeof(diagrm));
				else
					memcpy(&diagrm, pInputData ,nSize);
				return SendDiagRM(diagrm);
			}
			break;		
		case COMM_SENDDIAGWM:
		  	if (pInputData != NULL)
			{
				DIAGWM	diagwm;
				if(nSize == 0)
					memcpy(&diagwm, pInputData ,sizeof(diagwm));
				else
					memcpy(&diagwm, pInputData ,nSize);
				SendDiagWM(diagwm);
			}
			break;
		default:
			break;
	}
	return 1;
}


int		CtmTaskMWRS232::ReqValues(int nType)
{
	switch(nType)
	{
		case COMM_GETCONFIG:
			GetConfig();
			break;
	    case COMM_GETS_PROFILE01_I:
	    	GetSProfile01I();
	    	break;
	    case COMM_GETD_PROFILE01_I:
	    	GetDProfile01I();
	    	break;
	    case COMM_GETE_PROFILE01_I:
	    	GetEProfile01I();
	    	break;
	    case COMM_GETS_PROFILE01_H:
	    	GetSProfile01H();
	    	break;
	    case COMM_GETD_PROFILE01_H:
	    	GetDProfile01H();
	    	break;
	    case COMM_GETE_PROFILE01_H:
	    	GetEProfile01H();
	    	break;
	    case COMM_GETS_BPROFILE01_I:
	    	GetSBProfile01I();
	    	break;
	    case COMM_GETD_BPROFILE01_I:
	    	GetDBProfile01I();
	    	break;
	    case COMM_GETE_BPROFILE01_I:
	    	GetEBProfile01I();
	    	break;
	    case COMM_GETS_BPROFILE01_H:
	    	GetSBProfile01H();
	    	break;
	    case COMM_GETD_BPROFILE01_H:
	    	GetDBProfile01H();
	    	break;
	    case COMM_GETE_BPROFILE01_H:
	    	GetEBProfile01H();
	    	break;
	    default:
			break;
	}
	return 1;	
}

int		CtmTaskMWRS232::ReqValues(int nType, int nHostAdreess1, int nHostAdreess2)
{
	return 0;
}

int 	CtmTaskMWRS232::ReqValues(int nType, void* pOutData, int nSize)
{
	switch(nType)
	{
		case COMM_GETDAFLAG:
			memcpy(pOutData, &m_bNewDA2 ,sizeof(m_bNewDA2));
			break;
		case COMM_GETCOMMAND:
			memcpy(pOutData, &m_nCommandIndex ,sizeof(m_nCommandIndex));
			
			break;
		case COMM_GETDIAGRM:
			if(nSize == 0)
				memcpy(pOutData, GetDBDiagRM() ,sizeof(tmDB_DIAGRM));
			else
				memcpy(pOutData, GetDBDiagRM() ,nSize);
			break;
		case COMM_GETDIAGRI:
			if(nSize == 0)
				memcpy(pOutData, GetDBDiagRI() ,sizeof(tmDB_DIAGRI));
			else
				memcpy(pOutData, GetDBDiagRI() ,nSize);
			break;
		default:
			break;
	}
	return 1;
}


void	CtmTaskMWRS232::Run_Pthread(void* pClass, char *pData, int nLength)
{
	//int         i,j;
	//int         nReadCount;
	long		lValue;
/*******
|       Normal process
*******/
	if ((m_pSelf->m_cStateRx & COMM_STATE_WAIT00) && (m_pSelf->m_lCounterW0 <= 0))
	    m_pSelf->SendPanel(0xFF);
	if ((m_pSelf->m_cStateRx & COMM_STATE_WAIT01) && (m_pSelf->m_lCounterW1 <= 0))
	    m_pSelf->SendRProfile01();
	if ((m_pSelf->m_cStateRx & COMM_STATE_WAIT02) && (m_pSelf->m_lCounterW1 <= 0))
	    m_pSelf->SendSProfile01();

	if (!m_pSelf->bGotConfig)   //wy040600
	{
		if (m_pSelf->m_bCOM2)			// James add 2008/7/3 模溫機的ver code config資料為 config2
		{
			lValue = GetDBValue("CONF2_CODE_WRITE_NULL_DATA14").lValue;
	    	if((lValue == 0) && (m_pSelf->m_lCounterReqConfig <= 0)) // Version code
	        {
		        m_pSelf->m_lCounterReqConfig = COMM_TIME_REQCONFIG;
		        m_pSelf->SendRequestConfig();
	        }
		}
	}
	 ResValueFunc(NULL,NULL,0);
//	if  (m_prs232 != NULL && m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS)
//    {
//	    i = 0;
//	    g_wDsp2Status = 1;
//	    while ((g_commmap2[i].wCmd != g_frameRx.wCmd) && (g_commmap2[i].wCmd != COMM_NULL)) i++;
//	    if (g_commmap2[i].wCmd != COMM_NULL)
//	    {
//	    	m_nCommandIndex = g_commmap2[i].wCmd;
//	    	(this->*(g_commmap2[i].npFunc))();
//				if(g_wDspStatus == 0)/*JOYCE2011-12-31 com1不通訊時才update,解決:COM1&COM2都通訊時,進診斷畫面DSP易當機*/
//					{
//					if (g_ptaskCmd->GetFocus() != NULL) 
//						g_ptaskCmd->GetFocus()->Update();
//					}
//			}
//		
//			for ( j=0; j<5; j++)
//        {
//        	if (m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS) //St020101 for add update speed
//            {
//            	i = 0;
//            	while ((g_commmap2[i].wCmd != g_frameRx.wCmd) && (g_commmap2[i].wCmd != COMM_NULL))	i++;
//            	if (g_commmap2[i].wCmd != COMM_NULL)
//                {
//                	m_nCommandIndex = g_commmap2[i].wCmd;
//                	(this->*(g_commmap2[i].npFunc))();
//									if(g_wDspStatus == 0)/*JOYCE2011-12-31 com1不通訊時才update,解決:COM1&COM2都通訊時,進診斷畫面DSP易當機*/
//									{
//			        		if (g_ptaskCmd->GetFocus() != NULL) 
//				    				g_ptaskCmd->GetFocus()->Update();
//				    			}
//                }
//            }
//        	else 
//            	break;
//				}
//    	m_lCounter = COMM_TIME_RESET;
//    	g_wTaskCom2DSP54	= TRUE;
//    }
//	else if (m_lCounter < 0)
//    {
//	   	ResetClose();//JOYCE2011-1-27 Reset();    //20070815 mark 解決連上通訊開機時down機現象   // Reset the port if the line is idle for 3 seconds
//			g_wDsp2Status = 0;
//	    m_lCounter = COMM_TIME_RESET;
//	    if((g_wTaskCom2DSP54	== TRUE) && (g_wDspStatus == 0))/*JOYCE2011-12-31 com1不通訊時才update,解決:COM1&COM2都通訊時,進診斷畫面DSP易當機*/
//	    {
//			if (g_ptaskCmd->GetFocus() != NULL)
//				g_ptaskCmd->GetFocus()->Update();	
//			}
//    }
//	else if ((m_lCounterClsOpn < 0)&&(m_bResetClsOpn))//JOYCE2011-1-27 解決com2通訊有時需要60s左右才連上的問題
//		{
//		ResetOpen();
//		}		
}

void	CtmTaskMWRS232::ResValueFunc(void* pClass, char *pData, int nLength)
{
	int         i = 0, j = 0;
	int         nReadCount = 0;
	
	    if(m_pSelf->m_prs232 != NULL && m_pSelf->m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS)
	    {
		    i = 0;
		    g_wDsp2Status = 1;
		    while ((g_commmap2[i].wCmd != g_frameRx.wCmd) && (g_commmap2[i].wCmd != COMM_NULL)) i++;
		    if (g_commmap2[i].wCmd != COMM_NULL)
		    {
		    	m_pSelf->m_nCommandIndex = g_commmap2[i].wCmd;
		    	(m_pSelf->*(g_commmap2[i].npFunc))();
//			if(g_wDspStatus == 0)
//			{
//				SendMsg(MSG_SH_HMI_UPDATE_VIEW, 0, 0, NULL);	
//			}
		  }
			
		for ( j=0; j<5; j++)
	        {
		        	if (m_pSelf->m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS) 
		           {
		            	       i = 0;
		            	       while ((g_commmap2[i].wCmd != g_frameRx.wCmd) && (g_commmap2[i].wCmd != COMM_NULL))	i++;
			                if (g_commmap2[i].wCmd != COMM_NULL)
			                {
				                	m_pSelf->m_nCommandIndex = g_commmap2[i].wCmd;
				                	(m_pSelf->*(g_commmap2[i].npFunc))();
				                	
//						if(g_wDspStatus == 0)
//						{
//							SendMsg(MSG_SH_HMI_UPDATE_VIEW, 0, 0, NULL);	
//						}
			                }
		           }
	        		else 
	            		break;
		}
	    	m_pSelf->m_lCounter = COMM_TIME_RESET;
	    	g_wTaskCom2DSP54	= TRUE;
	   }
	  
	else if (m_pSelf->m_lCounter < 0)
	{
		  m_pSelf-> ResetClose();	
		   g_wDsp2Status = 0;
		   m_pSelf-> m_lCounter = COMM_TIME_RESET;
		    
//		    if((g_wTaskCom2DSP54	== TRUE) && (g_wDspStatus == 0))
//		    {
//				SendMsg(MSG_SH_HMI_UPDATE_VIEW, 0, 0, NULL);		
//		    }
		    
	  }
	  
   	 else if ((m_pSelf->m_lCounterClsOpn < 0)&&(m_pSelf->m_bResetClsOpn))
	 {
			m_pSelf->ResetOpen();
	 }

}

void	CtmTaskMWRS232::_Run(void *pData)
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		
	}
	//modify	2014/5/30 17:38:08
	while(1)
	{
		//usleep(10*1000);
		if(u_wThreadStartRunFlag_2)
		{
			pthread_mutex_lock(&g_setFlagLock);
			Run_Pthread(NULL,NULL,0);
			//ResValueFunc(NULL,NULL,0);
			u_wThreadStartRunFlag_2=FALSE;
			pthread_mutex_unlock(&g_setFlagLock);
		}
		usleep(10000);
	}
}

#ifndef	D_BEOBJECT_CTMTASKMWRS232
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskCom2dsp == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskHost*>(pResult))->CreateSelf();
		g_ptaskCom2dsp = static_cast<CTaskHost*>(pResult);
	}
	return g_ptaskCom2dsp;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskHost*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

