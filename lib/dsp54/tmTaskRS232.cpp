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
#include	"tmTaskRS232.h"
#include	"../../main.h"
#include	"../../tmconfig.h"
#include	"../../commonaction.h"
#include	"../../curvedata.h" 
#include	"../../qualitycontrol.h"
#include	"../../manufacture.h"
#include    "../../tmdebug.h"
#include	"../../oil54struct.h"
//#include	"../../function.h"
#include	"../../selvat_ancestor.h"
#include	"../../default.h"
#include	"../../iotable.h"
#include	"../../package.h"
#include	"../libControl.h"
#include  "../../tmshmsg.h"
#include  "../../tmnbmsg.h"
#include	"../../rs485.h"//JOYCE2010-12-16 
#include	"../libDataSave.h"
#include  "../hmi_group/tmTaskAmmeterDLT.h"

IMPLEMENT_DYNCREATE(CtmTaskRS232, CTaskHost)
typedef void *(* PRSRUN)(void *);
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		COMM_TIME_485REQS			(2 * 1000) 		//JOYCE2010-12-16, 5S  (5 * 1000), 2012-6-14 to 1S
#define		COMM_TIME_485REQC			(5 *60 * 1000) 		//fuxy, 2012-3-13, 3 min = 3 *60 * 1000, 2012-6-14 to 5min 
#define     COMM_TIME_RESET             (3 * 1024)
#define     COMM_TIME_REQCONFIG         (3 * 1024)		//wy040600
#define			COMM_TIME_TEMPER						(5 * 1024)		//netdigger 2005/12/3
#define     TLED_SLICE							500
#define     COMM_TIME_SHTCNTECHO        (4 * 1024)    //JOYCE 2011-8-22

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

#define   LOWORD(l)           ((WORD)((DWORD)(l) & 0xFFFF)) //JOYCE2010-7-21 
#define   HIWORD(l)           ((WORD)((DWORD)(l) >> 16))    //JOYCE2010-7-21 
#define   UNWORD(hw,lw)       (((DWORD)(hw) << 16) | lw) 		//JOYCE2010-7-21 
#define 		MAX_PCTBL_NUM				  192 

BOOL        bSpeechDelayOnOff;              //wy120999 add dor delay speech 100ms
long        m_lCounterSpeechDelayOnOff;
WORD		dbgppc[tmMAX_GROUP_PC];
BOOL		m_bMucellRun = FALSE;		//fuxy, 2012-3-15
BOOL		bGearDist = FALSE; 
int		m_bAmmeter = 0;
WORD	wOperationModeOld = 0;
BOOL	u_wThreadStartRunFlag=FALSE;
extern	pthread_mutex_t	g_setFlagLock;

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

const       COMMMAP g_commmap[] = {
            {COMM_GETCOLDSTART      	, 	&CtmTaskRS232::GetColdStart    },
            {COMM_GETWARMSTART      	, 	&CtmTaskRS232::GetWarmStart    },
            {COMM_GETCONFIG         	, 	&CtmTaskRS232::GetConfig       },
            {COMM_GETPRODUCTION     	, 	&CtmTaskRS232::GetProduction   },
            {COMM_GETOPSTATE        	, 	&CtmTaskRS232::GetOPState      },
            {COMM_GETOPSTATE2        	, 	&CtmTaskRS232::GetOPState2     },//Andy 20091208
            {COMM_GETOPSTATUS       	, 	&CtmTaskRS232::GetOPStatus     },
            {COMM_GETOPMETER        	, 	&CtmTaskRS232::GetOPMeter      },
            {COMM_GETOPMETERZEROAGAIN ,		&CtmTaskRS232::GetOPMeterZeroAgain},
            {COMM_GETOPTEMPERATURE  	, 	&CtmTaskRS232::GetOPTemperature},
            {COMM_GETOPSUMMARY      	, 	&CtmTaskRS232::GetOPSummary    },      //St020101
            {COMM_GETCURVEDATA      	, 	&CtmTaskRS232::GetCurveData    },
            {COMM_GETCONTROLINDEX   	, 	&CtmTaskRS232::GetControlIndex },
            {COMM_GETDAOUTPUT       	, 	&CtmTaskRS232::GetDAOutput     },      // St1108'01 for D/A
            {COMM_GETDAOUTPUT2      	, 	&CtmTaskRS232::GetDAOutput2    },	//Andy 20061212 for New D/A
            {COMM_GETMONICURRENT    	, 	&CtmTaskRS232::GetMoniCurrent  },
            {COMM_GETMONILAST       	, 	&CtmTaskRS232::GetMoniLast     },
            {COMM_GETMONICURRENTB   	, 	&CtmTaskRS232::GetMoniCurrentB },
            {COMM_GETMONILASTB      	, 	&CtmTaskRS232::GetMoniLastB    },
            {COMM_GETMONIMAXMINB    	, 	&CtmTaskRS232::GetMoniMaxMinB  },
            {COMM_GETDIAGRM         	, 	&CtmTaskRS232::GetDiagRM       },
            {COMM_GETDIAGRI         	, 	&CtmTaskRS232::GetDiagRI       },
            {COMM_GETS_PROFILE01_I  	, 	&CtmTaskRS232::GetSProfile01I  },
            {COMM_GETD_PROFILE01_I  	, 	&CtmTaskRS232::GetDProfile01I  },
            {COMM_GETE_PROFILE01_I  	, 	&CtmTaskRS232::GetEProfile01I  },
            {COMM_GETS_PROFILE01_H  	, 	&CtmTaskRS232::GetSProfile01H  },
            {COMM_GETD_PROFILE01_H  	, 	&CtmTaskRS232::GetDProfile01H  },
            {COMM_GETE_PROFILE01_H  	, 	&CtmTaskRS232::GetEProfile01H  },
            {COMM_GETS_BPROFILE01_I 	, 	&CtmTaskRS232::GetSBProfile01I },
            {COMM_GETD_BPROFILE01_I 	, 	&CtmTaskRS232::GetDBProfile01I },
            {COMM_GETE_BPROFILE01_I 	, 	&CtmTaskRS232::GetEBProfile01I },
            {COMM_GETS_BPROFILE01_H 	, 	&CtmTaskRS232::GetSBProfile01H },
            {COMM_GETD_BPROFILE01_H 	, 	&CtmTaskRS232::GetDBProfile01H },
            {COMM_GETE_BPROFILE01_H 	, 	&CtmTaskRS232::GetEBProfile01H },
            {COMM_GETAUSTIONDRIVEINFO ,   &CtmTaskRS232::GetAustonDriveInfo},	//JOYCE2011-2-11 
						
						{0x3330		    						, 	&CtmTaskRS232::GetEcho         },    //Andy 20090619
						{COMM_SENDCONFIG		    	, 	&CtmTaskRS232::GetEcho         },    //Andy 20090306
            {COMM_SENDZEROSET		    	, 	&CtmTaskRS232::GetEcho         },    //Andy 20090306
            {COMM_SENDPANELBREAK    	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDCONTROLMACHINE	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDCONTROLMOLDSET	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDCONTROLMONITOR	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDPOWERMATCH    	, 	&CtmTaskRS232::GetEcho         },    //Chuntzu 2004/8/13 add
            {COMM_SENDPOWERMATCH2   	, 	&CtmTaskRS232::GetEcho		   	 },    //Andy 20061212 for New D/A
            {COMM_SENDCONTROLMACHINEB ,		&CtmTaskRS232::GetEcho         },
            {COMM_SENDCONTROLMOLDSETB ,		&CtmTaskRS232::GetEcho         },
            {COMM_SENDCONTROLMONITORB ,		&CtmTaskRS232::GetEcho         },
            {COMM_SENDSTATUS        	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDR_PROFILE01   	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDS_PROFILE01   	, 	&CtmTaskRS232::GetEcho         },
            {COMM_SENDCOMMAND       	, 	&CtmTaskRS232::GetEcho         },      //St0512'03
            {COMM_ECHO_FAILURE      	,		&CtmTaskRS232::GetEcho         },//Andy for echo failure 20090305	
            
            {COMM_NULL              	, 	NULL                        	 }
            };

WORD        g_wPassOPSum[] = {                 //St051101 add for update fast select
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

static      BYTE		g_acRxBuffer[4096];
static      BYTE		g_acTxBuffer[4096];
static      BYTE		g_acRxBuffer2[4096];//JOYCE2010-12-16 
static      BYTE		g_acTxBuffer2[4096];//JOYCE2010-12-16 
static      COMMFRAMERX	g_frameRx;
static      COMMFRAMETX	g_frameTx;
static      COMM485FRAMERX		g_frame485Rx;//JOYCE2010-12-16 
CtmTaskRS232*	CtmTaskRS232::m_pSelf = NULL;//JOYCE2011-2-11 

static	char*	g_apszSELVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            NULL
            }; 
/*JOYCE2013-8-16 �ƶ��ʱ���m*/
static	char*	g_apszSlideVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL11DATA",     //20130816
            NULL
            }; 
            

/*JOYCE2013-12-19 ���l��m�˴�*/
static	char*	g_apszCorePosnChkVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_COR_COR_NULL_W0597",                    /*���l��m�˴�*/
            NULL
            }; 

/*JOYCE2013-8-22 �h���`�g��m�i�]*/
static	char*	g_apszInjOpnPosnVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL1DATA",
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL2DATA",
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL3DATA",
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL4DATA",
            NULL
            }; 

/*JOYCE2013-3-20 �}�Ҥ��~���Ҧ�m*/
static	char*	g_apszOpnMidEjtVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_MLD_OPEN_NULL_W075F",
            NULL
            }; 

///*JOYCE2012-7-19 ���l�h�G*/
//static	char*	g_apszCRET2VatTransID[] = {
//            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
//            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
//            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
//            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
//            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
//            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
//            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
//            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
//            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
//            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
//            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
//            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
//            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
//            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
//            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
//            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
//            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
//            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
//            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add   
//          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
//            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
//            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
//            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
//            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
//            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
//            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
//            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
//            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
//            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
//            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
//            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
//            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
//            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
//            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
//            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2012-7-19*/
//            NULL
//            }; 

//JOYCE2011-5-10 �S���l�ϥ�
static	char*	g_apszCSPVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add   
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL2DATA",       // 04D1 ���l�l�ܽT�{��m   //JOYCE2011-5-10 No.11-026
            NULL
            }; 

/*JOYCE2012-2-7  No.12-007 ���㤹�\�ʧ@*/
static	char*	g_apszCAAVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add   
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL1DATA",       /* 08D0 ���㤹�\�ʧ@*/ //JOYCE2012-2-7 
            NULL
            }; 

/*JOYCE2012-4-11  D_AUPT ��O*/
static	char*	g_apszAUPTVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m  
            "MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            "MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            "MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            "MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            "MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            "MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            "MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            "MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            "MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            "MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add   
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            "MSET_OTHR_OTHR_NULL_MOLDSPECIAL11DATA",       /* 08D0 ��O*/ //JOYCE2012-4-11 
            NULL
            }; 

static	char*	g_apszWSMVatTransID[] = {
            "MSET_BLST1_ON_START_POSITION",								// 0330 ���ҧj��ʧ@��m 
            "MSET_BLST2_ON_START_POSITION",								// 0334 ���ҧj��ʧ@��m 
            "MSET_BLST3_ON_START_POSITION",								// 0333 �j��#3�ʧ@��m   
            "MSET_BLST4_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST5_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            "MSET_BLST6_ON_START_POSITION",								// 033D �j��#4�ʧ@��m   
            //"MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA ���Ҥ@�q�פ��m 
            //"MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB ���ҤG�q�פ��m 
            //"MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC ���ҤT�q�פ��m 
            //"MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD ���ҧC���פ��m 
            //"MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E ���Ұ����פ��m           */  //Chuntzu 2004/7/16 add for Vrt
            //"MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 ������Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            //"MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 �����}�Ҧ�m               */  //Chuntzu 2004/7/16 add for Vrt
            //"MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA �}�Ҥ@�q�פ��m 
            //"MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB �}�ҤG�q�פ��m 
            //"MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC �}�ҤT�q�פ��m 
            //"MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD �}�ҥ|�q�פ��m 
            //"MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE �}�Ҥ��q�פ��m 
            "MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 ���~���Ҧ�m         // St1111'02 add   
          	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   ���~���Ҧ�mB   201212-13
            "MSET_CORA_IN_NULL_SETMOLDPOSITION", 						// 020F ���lA�i�ʧ@��m  
            "MSET_CORA_OUT_NULL_SETMOLDPOSITION",						// 0210 ���lA�h�ʧ@��m  
            "MSET_CORB_IN_NULL_SETMOLDPOSITION", 						// 0211 ���lB�i�ʧ@��m  
            "MSET_CORB_OUT_NULL_SETMOLDPOSITION",						// 0212 ���lB�h�ʧ@��m  
            "MSET_CORC_IN_NULL_SETMOLDPOSITION", 						// 0213 ���lC�i�ʧ@��m  
            "MSET_CORC_OUT_NULL_SETMOLDPOSITION",						// 0214 ���lC�h�ʧ@��m  
            "MSET_CORD_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lD�i�ʧ@��m  
            "MSET_CORD_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lD�h�ʧ@��m  
            "MSET_CORE_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lE�i�ʧ@��m  
            "MSET_CORE_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lE�h�ʧ@��m  
            "MSET_CORF_IN_NULL_SETMOLDPOSITION", 						// 0215 ���lF�i�ʧ@��m  
            "MSET_CORF_OUT_NULL_SETMOLDPOSITION",						// 0216 ���lF�h�ʧ@��m  
            "MSET_SCRW_INJ_NULL_W06DD",                   // 02DD ��u�űҰʦ�m <20101105M_01_D0> //JOYCE2010-11-8 
            "MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 ����⤤�~�U����m //JOYCE2011-2-10 
            "MSET_COR_COR_NULL_W0595",                    /* 0195 �h�G�ʧ@��m  JOYCE2013-6-13*/
            NULL
            }; 

static	char*	g_apszVatTransMachID[] = {
			"MACH_MLD_OPEN_NULL_W0216"
			};
			
static	char*	g_apszEffectPosiID[] = {
			"MSET_CORA_IN_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORA_IN_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_CORA_OUT_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORA_OUT_NULL_OILMINUSPOSITIONOFFSET",//Core A
			"MSET_CORB_IN_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORB_IN_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_CORB_OUT_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORB_OUT_NULL_OILMINUSPOSITIONOFFSET",//Core B
			"MSET_CORC_IN_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORC_IN_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_CORC_OUT_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORC_OUT_NULL_OILMINUSPOSITIONOFFSET",//Core C
			"MSET_CORD_IN_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORD_IN_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_CORD_OUT_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORD_OUT_NULL_OILMINUSPOSITIONOFFSET",//Core D
			"MSET_CORE_IN_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORE_IN_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_CORE_OUT_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORE_OUT_NULL_OILMINUSPOSITIONOFFSET",//Core E
			"MSET_CORF_IN_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORF_IN_NULL_OILMINUSPOSITIONOFFSET",
			"MSET_CORF_OUT_NULL_OILPLUSPOSITIONOFFSET",
			"MSET_CORF_OUT_NULL_OILMINUSPOSITIONOFFSET",//Core F
			"MSET_MLD_OPEN_END_OILPLUSPOSITIONOFFSET",
			"MSET_MLD_OPEN_END_OILMINUSPOSITIONOFFSET",//Eject
			NULL,
			NULL,
			NULL,
			NULL
			};                          

static	char*   pDSPLub1[] = {
						"MACH_LUB_ON_NULL_SHOTCOUNT",                   //�۰ʼ�ƼҼ�    
						"MACH_ALARM_ON_NULL_ALARMLUBRICATTIME",         //���ĵ���ɶ�    
						"MACH_LUB_ON_NULL_DELAYTIME",                   //��ƶ��j�ɶ�    
						"MACH_LUB_ON_NULL_COUNTER",                     //�۰ʼ�Ʀ���    
						"MACH_LUB_ON_NULL_TIME",                        //�۰ʼ�ƫO���p��
            NULL
            };
static	char*   pDSPLub2[] = {
						"MACH_LUB2_ON_NULL_SHOTCOUNT",                 //�۰ʼ�ƼҼ�    
						"MACH_LUB_ON_NULL_W019E",                      //���ĵ���ɶ�    
						"MACH_SCRW_INJ_NULL_W0184",                    //��ƶ��j�ɶ�    
						"MACH_SCRW_INJ_NULL_W0185",                    //�۰ʼ�Ʀ���    
						"MACH_LUB2_ON_NULL_TIME",                      //�۰ʼ�ƫO���p��
            NULL
            };
static	char*   pDSPLub3[] = {
						"MACH_MTR_ON_NULL_W01D9",                      //�۰ʼ�ƼҼ�    
						"MACH_MTR_ON_NULL_W01D7",                      //���ĵ���ɶ�    
						"MACH_MTR_ON_NULL_W01DE",                      //��ƶ��j�ɶ�    
						"MACH_MTR_ON_NULL_W01DF",                      //�۰ʼ�Ʀ���    
						"MACH_MTR_ON_NULL_W01DC",                      //�۰ʼ�ƫO���p��(1DC&1DD)
            NULL
            };
static	char*   pHMILub1A[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED25",           //�۰ʼ�ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED26",           //���ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED27",           //��ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED28",           //�۰ʼ�Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",         //�۰ʼ�ƫO���p��
            NULL
            };
static	char*   pHMILub2A[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED36",           //�۰ʼ�ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED37",           //���ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED38",           //��ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED39",           //�۰ʼ�Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",         //�۰ʼ�ƫO���p��
          	 NULL
            };
static	char*   pHMILub3A[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED47",           //�۰ʼ�ƼҼ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED48",           //���ĵ���ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED49",           //��ƶ��j�ɶ�    
						"SYSX_OTHERS_OTHERS_INT_RESERVED50",           //�۰ʼ�Ʀ���    
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",         //�۰ʼ�ƫO���p��
            NULL
            };

int   pChkPCValue[] = {
						0x0110,         /*���ҧj�� #1  			*/
						0x0111,         /*���ҧj�� #2  			*/
						0x0112,         /*���ҧj�� #3  			*/
						0x0113,         /*���ҧj�� #1  			*/
						0x0114,         /*���ҧj�� #2  			*/
						0x0115,         /*���ҧj�� #3  			*/
						0x0000,         /*����              */
            };
            
static	char*	g_MSET_InjeHoldSpdID[] = {       //wangli2014-8-20 16:18:04
			"MSET_SCRW_INJ_STEP1_FLOW",
			"MSET_SCRW_INJ_STEP2_FLOW",
			"MSET_SCRW_INJ_STEP3_FLOW",
			"MSET_SCRW_INJ_STEP4_FLOW",
			"MSET_SCRW_INJ_STEP5_FLOW",
			"MSET_SCRW_INJ_STEP6_FLOW",
			"MSET_SCRW_HLDP_STEP1_FLOW",
			"MSET_SCRW_HLDP_STEP2_FLOW",
			"MSET_SCRW_HLDP_STEP3_FLOW",
			"MSET_SCRW_HLDP_STEP4_FLOW",
			"MSET_SCRW_HLDP_STEP5_FLOW",
			"MSET_SCRW_HLDP_STEP6_FLOW",
			};        


/*!!!!!!
|       Can not execute periodic C routine???
|       Use counter instead!!!
!!!!!!*/
static      long        g_lCounterLEDMotor;
static      BOOL        g_bLEDMotor = FALSE;
static      long        g_lCounterLEDAdjMold;
static      BOOL        g_bLEDAdjMold = FALSE;
static		long		g_lCounterTemp	= COMM_TIME_TEMPER;		//netdigger 2005/12/3
static		int			u_nMicroAdjCount= 0;					//LEO20070621
static		int			u_nAutoAdjCount = 0;					//LEO20070621

char**			g_apszVatTransID=NULL;

int	OPS_AUTO;
int	OPS_STATE_BEGIN;
int	OPS_SEMIAUTO;
int	OPS_TIMEAUTO;
int	OPS_SENSORAUTO;
int	OPS_MACROADJ;
int	OPS_AUTOADJMOLD;
WORD    LSBCount=0;//JOYCE2011-8-22 
WORD    HSBCount=0;//JOYCE2011-8-22 
BOOL 		m_bPowerOnLub = FALSE;
BOOL    m_bAutoKey = FALSE;
BOOL    m_bSendAutoKey = FALSE;
BOOL	u_bManualKey = FALSE;//JOYCE2011-2-11 
//BOOL    m_bUnitChange = FALSE;
BOOL    m_bADInitial = FALSE;
BOOL    m_bADNotEqual = FALSE;
DB_ADCONFIG	ADConfig[16];

int			iRequestSCnt = 0;	//fuxy, 2011-9-9, for Mucell 
int			nCalibrat = 0;

long		l_PMTEST = 0;
BOOL		bPumpTestStart = FALSE, bKeyPumpStart = FALSE, bAutoKey = FALSE, bMannulKey = FALSE;
int			iPumpTestEnd = 0, iProjNum = 0;

extern	WORD			g_bMsgFormKey ;	//Caozz 2010/12/9 �U�� 03:01:09
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmTaskRS232::CtmTaskRS232() : CTaskHost() 
{
	m_pSelf				= this;//JOYCE2011-2-11 
	tmRS232       rs232;	
 	rs232.wChannel 		   = _COM1;
 	memset(&rs232, 0, sizeof(rs232));	//	fans add 2008-8-31 10:18:24
 	
 	memset(&m_dbchartInject		,  0, sizeof(m_dbchartInject		));
 	memset(&m_dbchartHold			,  0, sizeof(m_dbchartHold			));
 	memset(&m_dbchartInjectB	,  0, sizeof(m_dbchartInjectB	  ));
 	memset(&m_dbchartHoldB		,  0, sizeof(m_dbchartHoldB		  ));
 	memset(&m_dbchartChrg			,	 0, sizeof(m_dbchartChrg			));
 	memset(&m_dbchartChrgB		,  0, sizeof(m_dbchartChrgB		  ));
 	memset(&m_dbchartInjectLmt,  0, sizeof(m_dbchartInjectLmt ));
 	memset(&m_dbchartHoldLmt	,  0, sizeof(m_dbchartHoldLmt	  ));
 	
	rs232.config.DataLength= COM_DL_8;
	rs232.config.StopBit   = COM_SB_1;
	rs232.config.Parity    = COM_PR_NONE;
	rs232.config.BaudRate  = COM_BR_38400;
	rs232.config.Handshake = COM_HS_NONE;     //Chuntzu 2004/8/18 all use handshake none //Chuntzu 2004/8/5 add

	rs232.wRxBufferSize = sizeof(g_acRxBuffer);
	rs232.fpRxBuffer    = (BYTE *)&g_acRxBuffer;
	rs232.wTxBufferSize = sizeof(g_acRxBuffer);
	rs232.fpTxBuffer    = (BYTE *)&g_acTxBuffer;			
//	rs232.wMode			= 1;				//James add 2010/9/8 �W�� 08:59:34  //JOYCE2011-1-7 test
	
	if (g_pExTable->GetTableValue(0,"D_WSMVAT"))
			g_apszVatTransID = g_apszWSMVatTransID;
	else  if (g_pExTable->GetTableValue(0,"D_SLIDE"))/*JOYCE2013-8-16 �ƶ��ʱ���m*/
			g_apszVatTransID = g_apszSlideVatTransID;		
	else	if (g_pExTable->GetTableValue(0,"D_CSP"))//JOYCE2011-5-10 No.11-026
			g_apszVatTransID = g_apszCSPVatTransID;
	else	if (g_pExTable->GetTableValue(0,"D_CLPALLOW"))/*JOYCE2012-2-7  No.12-007 ���㤹�\�ʧ@*/
			g_apszVatTransID = g_apszCAAVatTransID;
	else	if (g_pExTable->GetTableValue(0,"D_AUPT"))/*JOYCE2012-4-11  ��O*/
			g_apszVatTransID = g_apszAUPTVatTransID;
	//JOYCE2013-6-13 mark else	if (g_pExTable->GetTableValue(0,"D_CORERET2"))/*JOYCE2012-7-19 ���l�h�G*/
  //JOYCE2013-6-13 mark		g_apszVatTransID = g_apszCRET2VatTransID;
	else	if (g_pExTable->GetTableValue(0,"D_OPNMIDEJT"))/*JOYCE2013-3-20 �}�Ҥ��~���Ҧ�m*/
			g_apszVatTransID = g_apszOpnMidEjtVatTransID;
	else	if (g_pExTable->GetTableValue(0,"D_INJMP"))/*JOYCE2013-8-22 �h���`�g��m�i�]*/
			g_apszVatTransID = g_apszInjOpnPosnVatTransID;
	else	if (g_pExTable->GetTableValue(0,"D_COREPOSNCHK"))/*JOYCE2013-12-19 ���l��m�˴�*/
			g_apszVatTransID = g_apszCorePosnChkVatTransID;
	else
			g_apszVatTransID = g_apszSELVatTransID;

	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATE_ID, 			FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATE2_ID, 			FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATUS_ID,			FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_METER_ID, 			FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_TEMP_ID,				FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_COM2TEMP_ID,		FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONICURRENT_ID, FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONIBCURRENT_ID,FALSE);
	SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS"	, 	long(0));			//	�۰ʺʴ�	                                                
		
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
	m_bNewDA	=				//Andy 20061212 for New D/A
	bGotConfig	= FALSE;        //wy040600 wy113099

	m_nReqConfigTimes = 10;                    //BW0108'02  
	m_lCounterReqConfig = COMM_TIME_REQCONFIG;   //wy040600 wy113099
	AddCounter((long *)&m_lCounterReqConfig, m_idMe);   //wy040600 wy113099
	
	bSpeechDelayOnOff = FALSE;          //wy120999 add for delay speech 100ms
	m_lCounterSpeechDelayOnOff = 100;

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

	//<<JOYCE2011-2-11 add
	for(int i = 0; i < MAX_TRANSMIT_KEEP; i++)
	{
		m_Transmit.acBuffer[i] = new unsigned char[COMM_MAXTRANSIMT];
	}

	m_Transmit.wInput     	= m_Transmit.wOutput = 0;
	m_Transmit.wQueueEnd  	= MAX_TRANSMIT_KEEP;
	m_Transmit.bQueueFull 	= FALSE;
	m_bTransmitCmdEcho	  	= TRUE;
	//>>JOYCE2011-2-11 add

	m_bUseSlow7GL    = FALSE;//JOYCE2008-9-24 ADD
	m_bAdjustUse7GL  = FALSE;//JOYCE2008-9-24 ADD
	m_bAutoAdjust7GL = FALSE; //JOYCE2008-9-24 ADD
	m_bKeySuck7GL    = FALSE; //JOYCE2008-9-24 ADD
	lWaitCount			 = 0;
	wSendMoldStatus	 = 0;
	wSendConfStatus	 = 0;
	wSendBlockStatus = 0;
	wOldCMD					 = 0;
	m_wLubrCnt			 = 0;//JOYCE2011-1-8 
	wShotCntFlag     = 0;//JOYCE 2011-8-22
	dwOldCmd         = 0;//JOYCE 2011-8-22
 	m_bCloseFlag	 = FALSE; //JOYCE2011-2-11  �ѨMaustone����HMI�Mdsp54�q�T���_���D
 	m_bAustoneState	 = FALSE; //JOYCE2011-2-11 
	m_bWarmStartFlag = FALSE;
	m_bColdStartFlag = FALSE;/*JOYCE2013-9-12*/
	
	m_bMUCELL = g_pExTable->GetTableValue(0,"D_MUCELL");//JOYCE2010-12-16 �L�o�w��
	m_bMUCELL2= g_pExTable->GetTableValue(0,"D_MUCELL2");/*JOYCE2012-6-29 �L�o�w��new*/
	m_bInjPurgeMsg = FALSE;//JOYCE2011-4-26 for mucell
	m_bOLDLUB = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70").lValue;//0-NEW LUB 1-OLD LUB
	m_bSDOOR  = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED98").lValue & 0x0001;  //JOYCE2013-10-30 g_pExTable->GetTableValue(0,"D_SDOOR"); 
	m_b2SDOOR = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED98").lValue & 0x0002;  //JOYCE2013-10-30 g_pExTable->GetTableValue(0,"D_2SDOOR");
	m_bENERGY = g_pExTable->GetTableValue(0,"D_ENERGY");
	m_bAmmeter 	= CtmConfig::GetInstance()->GetAmmeterFlag();
	m_bDC     = CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC;
	m_bALogin = g_pExTable->GetTableValue(0,"D_ALOGIN");
	m_bStepLub = g_pExTable->GetTableValue(0,"D_STEPLUB"); /*JOYCE2012-7-12 No.12-028 ���i�����*/
	l_PMTEST = g_pExTable->GetTableValue(0,"D_PMTEST");	//JOYCE20130327 for PumpTest
	m_bCOM2Vlv = g_pExTable->GetTableValue(0,"D_GT16") || g_pExTable->GetTableValue(0,"D_GT10");
	m_bAustoneCfgFlag = CtmConfig::GetInstance()->GetAustoneFlag();
	m_bN2CFlag        = CtmConfig::GetInstance()->GetN2CFlag();
	m_bSVRT = g_pExTable->GetTableValue(0,"D_SVRT");/*JOYCE2014-1-17  �¤G�O*/
	m_bInjSpdUnit = g_pExTable->GetTableValue(0,"D_INJSPDUNIT");

	if((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 2) || (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 3))
		m_b3LUB = TRUE;

	//	fans add
	OPS_AUTO = g_MultiLanguage.GetStrKeyIndex("OPS_AUTO");
	OPS_STATE_BEGIN = g_MultiLanguage.GetStrKeyIndex("OPS_AUTO");
	OPS_SEMIAUTO = g_MultiLanguage.GetStrKeyIndex("OPS_SEMIAUTO");
	OPS_TIMEAUTO = g_MultiLanguage.GetStrKeyIndex("OPS_TIMEAUTO");
	OPS_SENSORAUTO = g_MultiLanguage.GetStrKeyIndex("OPS_SENSORAUTO");
	OPS_MACROADJ = g_MultiLanguage.GetStrKeyIndex("OPS_MACROADJ");
	OPS_AUTOADJMOLD = g_MultiLanguage.GetStrKeyIndex("OPS_AUTOADJMOLD");
	
	for(int i = 0; i < 6; i++)
	{
		m_dwInjectTime[i] = 0;	
	}

	for(int i = 0; i < 7; i++)
	{
		m_nAirPCPoint[i]  = -1;
	}
	
	if(m_bMUCELL || m_bMUCELL2)	CreaMuCell();//JOYCE2010-12-16 
	
 	if(g_pExTable->GetTableValue(0,"D_AUSTONE")) //JOYCE2011-2-11  add for austone
 		m_lCounter = COMM_TIME_RESET;
 	else
	m_lCounter = 20*1000;//COMM_TIME_RESET;     //9/6/2007 8:23AM TEST

	AddCounter((long *)&m_lCounter, m_idMe);
	AddCounter((long *)&m_lCounterW0, m_idMe);
	AddCounter((long *)&m_lCounterW1, m_idMe);
	AddCounter((long *)&m_lCounterBW1, m_idMe);
	AddCounter((long *)&m_tled.lCount, m_idMe);
	AddCounter((long *)&g_lCounterLEDMotor, m_idMe);  //ChunTzu 2004/3/19 add
	AddCounter((long *)&m_lCounter485State, m_idMe);
	AddCounter((long *)&m_lCounter485ReqS, m_idMe);
	AddCounter((long *)&m_lCounter485ReqC, m_idMe);		//fuxy, 2012-3-13 
	AddCounter((long *)&g_lCounterLEDAdjMold, m_idMe);             //Bw1222'02 for autoadjmold
	AddCounter(&g_lCounterTemp, m_idMe);//netdigger 2005/12/3
	AddCounter((long *)&m_lCounterShtCntEcho, m_idMe);//JOYCE 2011-8-22
	RegisterMessage(MSG_SH_INET_SENDMOLDSET, m_idMe);			//JOYCE2011-11-9 , for ŪiNet�Ҩ�Z, �Ҩ��ƶǥD��
	RegisterMessage(MSG_RS232, m_idMe);
	RegisterMessage(MSG_NB_HMI_SEND_SELVATGEND,m_idMe);
}

CtmTaskRS232::~CtmTaskRS232()
{
	if(m_nID >= 0)	DelDevice(m_nID);
	UnRegisterAllMessage(MSG_NB_HMI_SEND_SELVATGEND); 
	UnRegisterAllMessage(m_idMe);
	UnRegisterAllMessage(MSG_SH_INET_SENDMOLDSET);			//JOYCE2011-11-9, for ŪiNet�Ҩ�Z, �Ҩ��ƶǥD��
	DeleteCounter(&g_lCounterTemp, m_idMe);//netdigger 2005/12/3
	DeleteCounter((long *)&g_lCounterLEDAdjMold, m_idMe);          //Bw1222'02 for autoadjmold
	DeleteCounter((long *)&g_lCounterLEDMotor, m_idMe);  //ChunTzu 2004/3/19 add
	DeleteCounter((long *)&m_tled.lCount, m_idMe);
	DeleteCounter((long *)&m_lCounterW1, m_idMe);
	DeleteCounter((long *)&m_lCounterBW1, m_idMe);
	DeleteCounter((long *)&m_lCounterW0, m_idMe);
	DeleteCounter((long *)&m_lCounter, m_idMe);
	DeleteCounter((long *)&m_lCounterSpeechDelayOnOff, m_idMe);      //wy012500 
	DeleteCounter((long *)&m_lCounter485ReqC, m_idMe);		//fuxy, 2012-3-13 
	DeleteCounter((long *)&m_lCounter485ReqS, m_idMe); 		// Caozz 2010/12/16 �W�� 08:55:54
	DeleteCounter((long *)&m_lCounter485State, m_idMe);
	DeleteCounter((long *)&m_lCounterShtCntEcho, m_idMe);      //JOYCE 2011-8-22 
}

int		CtmTaskRS232::CreateSelf()
{	
	CTaskHost::CreateSelf();
	sem_init(&ThreadWait, 0, 0);
	pthread_create(&m_RunThreadID, NULL, (PRSRUN)_Run, NULL);
	return 0;
}

int		CtmTaskRS232::FreeSelf()
{
	sem_destroy(&ThreadWait);
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
void        CtmTaskRS232::Run()
{
	sem_post(&ThreadWait);
	u_wThreadStartRunFlag=TRUE;
}

void		CtmTaskRS232::Run_Pthread(void* pClass, char *pData, int nLength)
{
	//int         i, j;
	//int         nReadCount;
	long		lValue;
	//static	int nNum = 0;
	//long	lDsp28CommState = 0;
	//nNum ++;
	
	if(m_pSelf->m_bMUCELL || m_pSelf->m_bMUCELL2)	m_pSelf->RS485ReadToken();//JOYCE2010-12-16 
	//=======
	//      Special control process
	//=======
	if ((g_lCounterLEDMotor <= 0) && (g_flMotorStatus == 1)) //ChunTzu 2004/3/19 mark for motor led//wy030599 add for check 0,1,-1
    {
	    g_lCounterLEDMotor = 250;
	    if ((g_bLEDMotor = !g_bLEDMotor))	
	    	PanelMotorOn(); 
	    else
	    	_UnActLed(LED_PNL_TM_MOTOR_ON);
    }
	
    	
   //JOYCE2012-6-19 mark	if ((dbbuff.cIndexMode >= 11) && (dbbuff.cIndexMode <= 14) && (g_lCounterLEDAdjMold <= 0))  
   	if ((dbbuff.cIndexMode == 11) && (g_lCounterLEDAdjMold <= 0))
   {
	    g_lCounterLEDAdjMold = 250;	    
	    
	    //<<JOYCE2008-9-24 ADD FOR 7GL
	    if(m_pSelf->m_bAutoAdjust7GL == TRUE)
	    {	
	    	if((g_bLEDAdjMold = !g_bLEDAdjMold))
	    		PanelAutoAdjON7GL();
	    	else
	    		_UnActLed(LED_PNL_GL_AUTOADJ_OFF);
	    }
	    else
	    //>>JOYCE2008-9-24 ADD FOR 7GL
	    {	
	    	if((g_bLEDAdjMold = !g_bLEDAdjMold))
	    		PanelAutoAdjOn();
	    	else
	    		_UnActLed(LED_PNL_TM_ADJUSTO_ATUO_OFF);
	   	}
    }
    
    
/*******
|       Normal process
*******/
	if ((m_pSelf->m_cStateRx & COMM_STATE_WAIT00) && (m_pSelf->m_lCounterW0 <= 0))
	{
		m_pSelf->m_wLubrCnt = 0; //JOYCE2011-1-8 
		//printf("run Set 0xFF \n");
	    m_pSelf->SendPanel(0xFF);
	}
	if ((m_pSelf->m_cStateRx & COMM_STATE_WAIT01) && (m_pSelf->m_lCounterW1 <= 0))
	    m_pSelf->SendRProfile01();
	if ((m_pSelf->m_cStateRx & COMM_STATE_WAIT02) && (m_pSelf->m_lCounterW1 <= 0))
	    m_pSelf->SendSProfile01();
	if ((m_pSelf->wShotCntFlag == 1) && (m_pSelf->m_lCounterShtCntEcho <= 0)) //JOYCE 2011-8-22
	    m_pSelf->wShotCntFlag = 0;

	if (!m_pSelf->bGotConfig)   //wy040600
	{
		lValue = GetDBValue("CONF_CODE_WRITE_NULL_DATA14").lValue;
    	if((lValue == 0) && (m_pSelf->m_lCounterReqConfig <= 0)) // Version code
        {
	        m_pSelf->m_lCounterReqConfig = COMM_TIME_REQCONFIG;
	        m_pSelf->SendRequestConfig();
        }
	}
	
	if(m_pSelf->m_bCloseFlag == TRUE)//JOYCE2011-2-11 
    {
    	if(m_pSelf->m_lCounter < (COMM_TIME_RESET - 1000))
    	{
    		if (m_pSelf->m_prs232 != NULL)
				m_pSelf->m_prs232->Open();
    		m_pSelf->m_bCloseFlag = FALSE;	
    	}
    }
	ResValueFunc(NULL,NULL,0);
//	if  (m_prs232 != NULL && m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS)
//    {
//	    i = 0;
//	    g_wDspStatus = 1;
//	    while ((g_commmap[i].wCmd != g_frameRx.wCmd) && (g_commmap[i].wCmd != COMM_NULL)) i++;
//	    if (g_commmap[i].wCmd != COMM_NULL)
//	    {
//	    	m_nCommandIndex = g_commmap[i].wCmd;
//	        (this->*(g_commmap[i].npFunc))();
//	      wOldCMD = g_commmap[i].wCmd;
//				if (g_ptaskCmd->GetFocus() != NULL) 
//				g_ptaskCmd->GetFocus()->Update();
//			}
//
//   		for ( j=0; j<10; j++)	//JOYCE2011-2-11 for ( j=0; j<5; j++)
//        {
//        	if (m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS) //St020101 for add update speed
//            {
//            	i = 0;
//            	while ((g_commmap[i].wCmd != g_frameRx.wCmd) && (g_commmap[i].wCmd != COMM_NULL)) i++;
//            	if (g_commmap[i].wCmd != COMM_NULL)
//                {
//                	//if(g_commmap[i].wCmd == 0x3131)	printf("==cmd:%4X\n",g_commmap[i].wCmd);
//                	m_nCommandIndex = g_commmap[i].wCmd;
//                	(this->*(g_commmap[i].npFunc))();
//                	wOldCMD = g_commmap[i].wCmd;
//			        if (g_ptaskCmd->GetFocus() != NULL) 
//				    g_ptaskCmd->GetFocus()->Update();
//                }
//            }
//        	else 
//            	break;
//				}
//
//			if(u_bManualKey == TRUE || g_frameRx.wCmd == COMM_GETWARMSTART)//JOYCE2011-2-11 �����W�c�������A�ӾɭP�q�T�ϼ��_�u
//			  {
//    			m_lCounter = COMM_TIME_RESET*2;
//    			if(g_frameRx.wCmd == COMM_GETOPSTATUS)
//    				u_bManualKey = FALSE;
//    		}
//    	else
//    		m_lCounter = COMM_TIME_RESET;
//
//    	g_wTaskDSP54	= TRUE;
//
//    	if ((bSpeechDelayOnOff) && (m_lCounterSpeechDelayOnOff < 0))        //wy120999 add for delay speech 100ms
//        {
//	        bSpeechDelayOnOff = FALSE;
//	        ::DeleteCounter((long *)&m_lCounterSpeechDelayOnOff, m_idMe);
//	        m_lCounterSpeechDelayOnOff = 100;
//        }
//
//      if((1 == m_bAustoneCfgFlag) || (1 == m_bN2CFlag))		//�sAustone�q�T
//			{
//				if(g_wDspStatus == FALSE)		//James add for austone 2010/5/19
//				{
//					g_wDspStatus		= TRUE;
//					m_bTransmitCmdEcho 	= TRUE;
//				}
//				g_wDspStatus		= TRUE;
//				lDsp28CommState 	= 1;//GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;	//STNC.SLV_TALK  james add 2010/3/10 �P�_dsp28�O�_�s�u
//			}
//      if((1 == m_bAustoneCfgFlag) || (1 == m_bN2CFlag))		//�sAustone�q�T
//		 	{
//		 		if(lDsp28CommState > 0 /*&& nNum >=1 && m_bTransmitCmdEcho*/)		//���j240ms �~��o�@����Ƶ�dsp28 James add for austone 2010/5/19
//				    {
//				    	if(PopTransmitCmd(&m_TransmitBuffer))
//				    	{
//				    		//printf("SendTransmitQueue \n");
//				    		SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT);
//				    		m_bTransmitCmdEcho = FALSE;
//				    	}
//				    	nNum = 0;
//				    }
//		
//			    if(nNum >= 10)			//�W�ɭ��s�o�e
//				    {
//				    	m_bTransmitCmdEcho = TRUE;
//				    }
//			}
//    }
//	else if (m_lCounter < 0)
//    {
//	    Reset();            //20070815 mark �ѨM�s�W�q�T�}����down���{�H   // Reset the port if the line is idle for 3 seconds
//		g_wDspStatus = 0;
//	    m_lCounter = COMM_TIME_RESET;
//	    if(g_wTaskDSP54	== TRUE)
//	    {
//			if (g_ptaskCmd->GetFocus() != NULL)
//				g_ptaskCmd->GetFocus()->Update();	
//		}
//    }
    
    if (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue ==ID_VIEW_PANEL)     //KV2003/2/12 add for LED test
	{
		if (m_pSelf->m_tled.lCount <= 0) m_pSelf->LED();       // Service LED test
	}

	if((!m_pSelf->m_bOLDLUB) && (!m_pSelf->m_bStepLub))
		m_pSelf->ProcessLubricate(); //JOYCE2011-8-23 lubricate

	if(!m_bPowerOnLub) /*JOYCE2012-3-20 �}���۰ʼ�ƤѼ�*/
		{
			m_pSelf->SetPowerOnLubDefault();
			m_bPowerOnLub = TRUE;
		}

	if(m_pSelf->m_bMUCELL) //JOYCE2011-4-26 �M�z�����Ymessage
	{
		long long lVlvTemp = GetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47").lValue;//�j�s��X�]�֪�1�^value
	
		if((lVlvTemp == 0x8001) && (!g_wErrorStatus) && GetDBValue("MSET_SCRW_RCV_NULL_W046F").lValue)
		{
			SendMsg(MSG_NB_MUCELL_INJPURGE, 0, 0, NULL);
			m_pSelf->m_bInjPurgeMsg = TRUE;
		}
		else
		{
			if(m_pSelf->m_bInjPurgeMsg)
				{
				SendMsg(MSG_USER_SH_PROMPTNULL, 0, 0, NULL);
				m_pSelf->m_bInjPurgeMsg = FALSE;
				}

			if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12").lValue == 2)		//fuxy, 2012-3-13  �S������o�w��  �G���ծժ��^�_  ���� ���� 
			{
				if((!g_wErrorStatus)&& GetDBValue("MSET_SCRW_RCV_NULL_W046F").lValue 
					&& (GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED67").lValue &0x0000FFFF < 4000))
				{
						SendMsg(MSG_NB_MUCELL_CALIBRAT, 0, 0, NULL);
				}
			}

		}
	}
/*
	if(m_bMUCELL2)
	{
		if(m_bMucellRun && !m_bUnitChange)
			{
			m_bUnitChange =TRUE ;
			if(g_ptaskdsp != NULL) 
	    		g_ptaskdsp->ReqValues(MUCELL_COMM_TASKCODE_SEND_U, 0x60, 0,0, 0);
			}
	}	*/
	
	if(m_pSelf->m_bALogin || (l_PMTEST == 1)) /*JOYCE2012-6-18 ��������۰ʪ��A��,�ݭn��J�K�X�T�{*/ /*JOYCE2014-3-31 PUMPTEST*/
	{
		if(m_bSendAutoKey)
		{
	   	m_bSendAutoKey = FALSE;
	   	m_pSelf->SendPanel(0xFF);
	  }

	}

	if(m_pSelf->m_bSVRT)/*JOYCE2014-1-17  �¤G�O,�q�l�ؤ��i���m*/ 
	{
		if(m_bADNotEqual)
		{
			//printf("m_bADNotEqual= %d\n", m_bADNotEqual);
			m_bADNotEqual = FALSE;
			m_pSelf->SendConfig();
		}
	}

	if(l_PMTEST == 1)		//JOYCE20130327 for PumpTest
	{
		long lOPERSTEP1 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;
		long lOPERSUBSTEP1 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSUBSTEP1").lValue;
		long lOPERSTEP2 = GetDBValue("STATE_OTHR2_OTHR_NULL_OPERSTEP1").lValue;/*JOYCE2013-6-20 11:30:52 for �y�{19/20*/

		if(lOPERSTEP1  == 0x0400) /*�ɥΤ��l�i*/
		{
				if(lOPERSUBSTEP1  > 0x0005)  /*���l�iF*/
					iProjNum = (lOPERSUBSTEP1 &0x000F) *2 +5;
				else                        /*���l�iA~E*/
					iProjNum = (lOPERSUBSTEP1 &0x000F) *2 -1;
				SetDBValue("SYSX_SCREW_INJECT_NULL_PRECISIONOPTION", iProjNum, FALSE);
		}
		else  if(lOPERSTEP1  == 0x0800) /*�ɥΤ��l�h*/
		{
				if(lOPERSUBSTEP1 > 0x0005)  /*���l�hF*/
					iProjNum = lOPERSUBSTEP1 *2 +6;
				else                       /*���l�hA~E*/
					iProjNum = lOPERSUBSTEP1 *2;
				SetDBValue("SYSX_SCREW_INJECT_NULL_PRECISIONOPTION", iProjNum, FALSE);
		}
		else if(lOPERSTEP1 == 0x4008)
		{
				iProjNum = ((lOPERSUBSTEP1 >> 8) &0x000F) +10;
				SetDBValue("SYSX_SCREW_INJECT_NULL_PRECISIONOPTION", iProjNum, FALSE);
		}
		else if((lOPERSTEP2 == 0x0001) || (lOPERSTEP2 == 0x0002)) /*JOYCE2013-6-20 11:32:44 �ɥ�STAT.STEP2+0 bit0*/
		{
				iProjNum = lOPERSTEP2 + 18;
				SetDBValue("SYSX_SCREW_INJECT_NULL_PRECISIONOPTION", iProjNum, FALSE);
		}	
		
		if( (g_frameRx.commdata.OPSummary.wOperationStep[0] == 0x0400) && (_ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) == 1 || _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) == 3) /*JOYCE2012-6-21 �L�o��4��bit-�D�ʧ@���A*/
			&& ((GetDBValue("STATE_ALARM_ON_STEP4_ERRORDATA").lValue & 0x0010) == 0) && ( (iPumpTestEnd == 0) ||(bKeyPumpStart == TRUE)) && (bPumpTestStart == FALSE))		//�b�۰�, �ɥΤ��l�i�ʧ@���A�A  //2012-3-7, add ���۰� 
		{
			//printf("test run start \n");
			SendMsg(MSG_SH_INET_PUMPTEST_START, 0, 0,NULL);
			iProjNum = 0;
			SetDBValue("SYSX_SCREW_INJECT_NULL_PRECISIONOPTION", 0, FALSE);
			bPumpTestStart =TRUE;
		}
	}
}

void        CtmTaskRS232::SetStatusWindow(CtmWnd* pwndStatus)
{
}

void        CtmTaskRS232::ProcessShotCnt()//JOYCE2011-8-22 
{
		DWORD	dwDBValue =0;

    if(l_PMTEST == 1)	//JOYCE20130327 for PumpTest 
		{
			if(bPumpTestStart == TRUE)
			{
				iPumpTestEnd = 0;
				SendMsg(MSG_SH_INET_PUMPTEST_END, iPumpTestEnd, 0,NULL);
				//printf("test auto end \n");
				bPumpTestStart = FALSE;
			}
		}

     dwDBValue = UNWORD(HSBCount,LSBCount);
     SetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", dwDBValue, FALSE);

     dwDBValue = GetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT").lValue;
    	if(CtmConfig::GetInstance()->GetINETFlag() == 1)
    		{
    			DWORD	dwUNIT=GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
    			SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + dwUNIT, FALSE);
    		}
    	else
    		SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + 1, FALSE);
       
     //JOYCE2010-10-12 �ֿn�B��Ҽ�
		 DWORD dwTotalShotCount	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED36").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED35").lValue);/*JOYCE2014-5-21 �ѨM�ֿn�B��ҼƤ���bug, MARK UNWORD(HSBCount,LSBCount);*/
		 dwTotalShotCount += 1;
		 if(dwTotalShotCount > 999999999)	dwTotalShotCount = 0;
		 SetDBValue("SYS_HMI_NULL_NULL_RESERVED35",LOWORD(dwTotalShotCount),FALSE);
		 SetDBValue("SYS_HMI_NULL_NULL_RESERVED36",HIWORD(dwTotalShotCount),FALSE);
     
      //JOYCE2011-9-30  �ֿn�B��Ҽ� for lubricate
			if((!m_bOLDLUB) && (!m_bStepLub))
			{					
			dwTotalShotCount	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;
			dwTotalShotCount += 1;
			if(dwTotalShotCount > 999999999)	dwTotalShotCount = 0;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27",dwTotalShotCount,FALSE);
			}

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
             if (m_bDC) //James add 2008/8/13 09:42�W��
             {
							CtmQualityControl::GetInstance()->SetData(12,0);	//fuxy,2009-1-12
							CtmQualityControl::GetInstance()->SetData(13,0);
             	SendControlMonitorB();
							}
            }
        }
        //	fans add 2009/5/18 10:53�W��        
    	SendMsg(MSG_DSP54_GET_SHOTCOUNT, 0, 0, NULL);

		  if(m_bENERGY || m_bAmmeter)//�{�b���~�Ʋ֭p
			{          	 
        int 	NumPerUnit 	= GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
        DWORD	dwCurrentProduct 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED21").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED20").lValue);
        dwCurrentProduct  +=NumPerUnit;
				SetDBValue("SYS_HMI_NULL_NULL_RESERVED20",LOWORD(dwCurrentProduct),FALSE);
				SetDBValue("SYS_HMI_NULL_NULL_RESERVED21",HIWORD(dwCurrentProduct),FALSE);
			}
}

void		CtmTaskRS232::ProcessLubricate()  //JOYCE2011-8-3 lubricate
{
	DWORD	dwLubrData = 0;
	BOOL	bProcessFlag = FALSE;
	BOOL	bShotCntLub1Flag = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue & 0x0001; //bit0
	BOOL	bShotCntLub2Flag = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue & 0x0002; //bit1
	BOOL	bShotCntLub3Flag = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue & 0x0004; //bit2		
	long  lTemp = 0;
	long	lShotCntTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;/*��ڹB��Ҽ�*/
	long  lChangCntLub1= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue;/*��1�ռ�Ƥ����Ҽ�*/
	long  lChangCntLub2= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED51").lValue;/*��2�ռ�Ƥ����Ҽ�*/
	long  lChangCntLub3= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED52").lValue;/*��3�ռ�Ƥ����Ҽ�*/

  //printf("232 lChangCntLub1=%d,lChangCntLub2=%d,lChangCntLub3=%d\n",lChangCntLub1,lChangCntLub2,lChangCntLub3);
	if((!bShotCntLub1Flag)&&(lShotCntTemp > lChangCntLub1)) //only once; LUB1
		{
			bProcessFlag = TRUE;
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue | 0x0001;
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19",lTemp,FALSE);
			for(int i=0; i<5 ;i++)
				{
				if(pHMILub1A[i] != NULL)	dwLubrData = GetDBValue(pHMILub1A[i]).lValue;
 		 		if(pDSPLub1[i] != NULL)	SetDBValue(pDSPLub1[i], dwLubrData);
				}
    }
	
	if((!bShotCntLub2Flag)&&(lShotCntTemp > lChangCntLub2)) //only once; LUB2
		{
			bProcessFlag = TRUE;
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue | 0x0002;
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19",lTemp,FALSE);
			for(int i=0; i<5 ;i++)
				{
				if(pHMILub2A[i] != NULL)	dwLubrData = GetDBValue(pHMILub2A[i]).lValue;
 		 		if(pDSPLub2[i] != NULL)	SetDBValue(pDSPLub2[i], dwLubrData);
				}
    }
	
	if((!bShotCntLub3Flag)&&(lShotCntTemp > lChangCntLub3) && m_b3LUB) //only one; LUB3
		{
			bProcessFlag = TRUE;
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue | 0x0004;
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19",lTemp,FALSE);
			for(int i=0; i<5 ;i++)
				{
					if(pHMILub3A[i] != NULL)	dwLubrData = GetDBValue(pHMILub3A[i]).lValue;
 		 			
 		 			if(i == 4)
 		 				{
						SetDBValue("MACH_MTR_ON_NULL_W01DD",dwLubrData>>16);  //�۰ʼ�ƫO���p��
						SetDBValue("MACH_MTR_ON_NULL_W01DC",dwLubrData&0xFFFF);
 		 				}
 		 			else
 		 				{
		   			if(pDSPLub3[i] != NULL)	SetDBValue(pDSPLub3[i], dwLubrData);
 		 				}	
				}
    }
	
	if(bProcessFlag == TRUE)
		SaveLubricate();
}

void    CtmTaskRS232::SaveLubricate()  //JOYCE2011-8-23 lubricate
{
	DWORD dwLubrData[53];
	char*   pDataID1[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED20",
						"SYSX_OTHERS_OTHERS_INT_RESERVED21",
						"SYSX_OTHERS_OTHERS_INT_RESERVED22",
						"SYSX_OTHERS_OTHERS_INT_RESERVED23",
						"SYSX_OTHERS_OTHERS_INT_RESERVED24",
						"SYSX_OTHERS_OTHERS_INT_RESERVED25",
						"SYSX_OTHERS_OTHERS_INT_RESERVED26",
						"SYSX_OTHERS_OTHERS_INT_RESERVED27",
						"SYSX_OTHERS_OTHERS_INT_RESERVED28",
						"SYSX_OTHERS_OTHERS_INT_RESERVED29",
						"SYSX_OTHERS_OTHERS_INT_RESERVED30",
						"SYSX_OTHERS_OTHERS_INT_RESERVED31",
						"SYSX_OTHERS_OTHERS_INT_RESERVED32",
						"SYSX_OTHERS_OTHERS_INT_RESERVED33",
						"SYSX_OTHERS_OTHERS_INT_RESERVED34",
						"SYSX_OTHERS_OTHERS_INT_RESERVED35",
						"SYSX_OTHERS_OTHERS_INT_RESERVED36",
						"SYSX_OTHERS_OTHERS_INT_RESERVED37",
						"SYSX_OTHERS_OTHERS_INT_RESERVED38",
						"SYSX_OTHERS_OTHERS_INT_RESERVED39",
						"SYSX_OTHERS_OTHERS_INT_RESERVED40",
						"SYSX_OTHERS_OTHERS_INT_RESERVED41",
						"SYSX_OTHERS_OTHERS_INT_RESERVED42",
						"SYSX_OTHERS_OTHERS_INT_RESERVED43",
						"SYSX_OTHERS_OTHERS_INT_RESERVED44",
						"SYSX_OTHERS_OTHERS_INT_RESERVED45",
						"SYSX_OTHERS_OTHERS_INT_RESERVED46",
						"SYSX_OTHERS_OTHERS_INT_RESERVED47",
						"SYSX_OTHERS_OTHERS_INT_RESERVED48",
						"SYSX_OTHERS_OTHERS_INT_RESERVED49",
						"SYSX_OTHERS_OTHERS_INT_RESERVED50",
						"SYSX_OTHERS_OTHERS_INT_RESERVED51",
						"SYSX_OTHERS_OTHERS_INT_RESERVED52",
						"SYSX_OTHERS_OTHERS_INT_RESERVED53",
						"SYSX_OTHERS_OTHERS_INT_RESERVED54",
						"SYSX_OTHERS_OTHERS_INT_RESERVED55",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED20",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED22",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED24",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED26",
						"SYSX_OTHERS_OTHERS_INT_RESERVED56",      /*JOYCE2012-6-25 add 6�� ��Ʀ��ƤW�U�����w*/
						"SYSX_OTHERS_OTHERS_INT_RESERVED57",
						"SYSX_OTHERS_OTHERS_INT_RESERVED58",
						"SYSX_OTHERS_OTHERS_INT_RESERVED59",
						"SYSX_OTHERS_OTHERS_INT_RESERVED60",
						"SYSX_OTHERS_OTHERS_INT_RESERVED61",
						"SYSX_OTHERS_OTHERS_INT_RESERVED62",     /*JOYCE2012-9-14 �t�m�����W�[���ѽX*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED50",   /*JOYCE20130809 ��1�ռ�Ƥ����Ҽ�*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED51",   /*JOYCE20130809 ��2�ռ�Ƥ����Ҽ�*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED52",   /*JOYCE20130809 ��3�ռ�Ƥ����Ҽ�*/
            NULL
            };
	
	char*   pDataID2[] = {
            "MACH_LUB_ON_NULL_BYPASSOPTION",
						"SYSX_OTHERS_OTHERS_INT_RESERVED19",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED27",
            NULL
            };

   memset(dwLubrData, 0, sizeof(dwLubrData));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/lubricate1.cdb", szPath);    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);    
    if (nFd >= 0)
		{
		for(int i=0; i<53; i++)
			{
			if(pDataID1[i] != NULL)	dwLubrData[i] = GetDBValue(pDataID1[i]).lValue;
					//printf("tmTaskRS232   dwLubrData1[%d]:%d\n",i,dwLubrData[i]);
			}
		}
	write(nFd, &dwLubrData, sizeof(dwLubrData));
	close(nFd);

		//FILE2
    sprintf(m_szCopyFile, "%s/lubricate2.cdb", szPath);
    nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);    
    if (nFd >= 0)
		{
		for(int i=0; i<3 ;i++)
			{
			if(pDataID2[i] != NULL)	dwLubrData[i] = GetDBValue(pDataID2[i]).lValue;
					//printf("dwLubrData2[%d]:%d\n",i,dwLubrData[i]);
			}
		}
	write(nFd, &dwLubrData, sizeof(dwLubrData));
	close(nFd);
}

void	CtmTaskRS232::SetDefaultEffectPos() //JOYCE2011-8-26 �}�Ҧ�m���İϪ�l��
{
	WORD  PrecisionDate = GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
	WORD	temp = GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
	WORD	wClampPos;
	WORD	lEffectPos;
	
	wClampPos = (HEX2BCD(temp))*10+(PrecisionDate/(0x0001<<4) % 0x0010);
	lEffectPos = wClampPos/6; //�D���X�Ҧ�m�ت��ת�1/6
	if(wClampPos)  //�j�_60,�h�H1/6�ȧ@�����İ�,�p�_�h�Hdefault--60�@�����İ�;Ū����D���X�Ҧ�m�ت��סA60�@�����İ�
	{
		if(lEffectPos > 600)
		{
			SetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION", lEffectPos,FALSE);	
		}
	}
}

void	CtmTaskRS232::SetPowerOnLubDefault()
{
	int nPowerOnLub = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED8").lValue;
	if(!nPowerOnLub)
		SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION",0);/*�}���۰ʼ�ƤѼƬ�0,�h�e0��D��,�N��}���ɵL�ݰ����*/
	else
		{
			/*�}�����ɶ��t*/
			STATEMODE	tmStateModeHead;
			STATEMODE	tmStateMode;
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, 0, &tmStateModeHead);
			int		u_nPowerOnTime[3]	= {0};
			int		u_nPowerOffTime[3]	= {0};
			BOOL  u_bPowerOn = FALSE, u_bPowerOff = FALSE;
			//printf("nCount=%d\n",tmStateModeHead.nCount);
			if(tmStateModeHead.nCount > 1)
			{	
				for(int i=tmStateModeHead.nCount; i>0; i--)
				{
					CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, i, &tmStateMode);
					if(tmStateMode.nMode == DATAPROCESS_STATEMODE_POWERON)/*�����}���ɶ�*/
					{
						if(!u_bPowerOn)
							{	
								u_nPowerOnTime[0] = tmStateMode.date.year;
								u_nPowerOnTime[1] = tmStateMode.date.month;
								u_nPowerOnTime[2] = tmStateMode.date.day;
								u_bPowerOn =TRUE ;
								if(u_bPowerOff) 
									break;
							}
					}
					if(tmStateMode.nMode == DATAPROCESS_STATEMODE_POWEROFF)/*�W�������ɶ�*/
					{
						if(!u_bPowerOff)
							{	
								u_nPowerOffTime[0] = tmStateMode.date.year;
								u_nPowerOffTime[1] = tmStateMode.date.month;
								u_nPowerOffTime[2] = tmStateMode.date.day;
								u_bPowerOff =TRUE ;
								if(u_bPowerOn) 
									break;
							}
					}
				}

				if(u_bPowerOff) /*�L�����ɶ��h�����B�z*/					
					{
						if(u_nPowerOnTime[0] >= u_nPowerOffTime[0])/*year*/
							{
								long long lDifferDay = 0;
								int			  Py=360,Pm=30; //�t��
								
								lDifferDay = (u_nPowerOnTime[0] - u_nPowerOffTime[0])*Py
								           + (u_nPowerOnTime[1] - u_nPowerOffTime[1])*Pm
								           + (u_nPowerOnTime[2] - u_nPowerOffTime[2]);
								
								if(u_nPowerOnTime[0] > u_nPowerOffTime[0])/*year*/
									lDifferDay += 1;
								//printf("lDifferDay=%d\n",lDifferDay);
									
								if(lDifferDay >= 0)/*�}�����ѼƮt*/
									{
										if(lDifferDay >= nPowerOnLub)
										{
											SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION",1);/*�}�����ѼƮt > �}���۰ʼ�ƤѼ�,�h�e1��D��,�N��}���ɻݰ����*/
										}
										else
										{
											SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION",0);/*�}�����ѼƮt <= �}���۰ʼ�ƤѼ�,�h�e0��D��,�N��}���ɵL�ݰ����*/
										}
									}
							}
					}
			}
		}
}

void	CtmTaskRS232::Check_PCTBL()//joyce2012-12-19
{
	char pDataID[256];	
	int PCChannel=0, PCDataChannel=0;

	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED13", 0, FALSE);/*QMC*/

	for(int i=0; i<MAX_PCTBL_NUM; i++)
	{
		sprintf(pDataID, "CONF_CODE_WRITE_CHL%d_OUTPUTDEFINITION", i+1);
		
		for(int j=0; j< (int)(sizeof(pChkPCValue)/sizeof(pChkPCValue[0])); j++)
		{
		if(!(GetDBValue(pDataID).lValue & 0x8000)    /*�D��*/
			 && ((GetDBValue(pDataID).lValue & 0x03FF) == *(pChkPCValue+j))) /*��4��N���I���I��;0x0400��ܤ��޿�*/
			m_nAirPCPoint[j] = i;

			//printf("pChkPCValue[%d]=%X,m_nAirPCPoint[%d]=%d\n",j,*(pChkPCValue+j),j,m_nAirPCPoint[j]);
		}

		if((GetDBValue(pDataID).lValue & 0x0FFF) == 0x00EF)     /*��4��N���I���I��  PC--MCS�ϯ�*/
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED13", 1, FALSE); /*QMC�ϥ�,PC�۰ʷj��, O_MCS_ENA 0EFH ;MCS �ϯ�*/
	}	

	for(int i=0; i< (int)(sizeof(pChkPCValue)/sizeof(pChkPCValue[0])); i++)
	{
		if(m_nAirPCPoint[i] != -1)
		{
			PCChannel     = (m_nAirPCPoint[i] / 16) / 2;       /*0~1*/           /*0~15,16~31,32~47,48~63,......*/
			PCDataChannel = (m_nAirPCPoint[i] % 32) /16;       /*0~1*/           /*0~15,16~31,32~47,48~63,......*/

			sprintf(m_AirPCDataID[i], "STATE_IO%d_WRITE_NULL_REALDATA%d", PCChannel+1, PCDataChannel+1);
			//printf("m_AirPCDataID[%d]=%s\n",i,m_AirPCDataID[i]);
		}
	}	
}

/*�¤G�O------------------------------*/
void	 CtmTaskRS232::ReadADConfig()
{
	memset(ADConfig,0,sizeof(ADConfig));

	int 	nFd = open("/conf/ADConfig.cdb", O_RDWR);
	if (nFd == -1) /*file not exit;set flag*/
		m_bADInitial = TRUE;
	else	if(nFd > 0)
		read(nFd, &ADConfig, sizeof(ADConfig));
	
	close(nFd);
}

void	 CtmTaskRS232::SaveADConfig()  
{
	char	pDataID[128];
	for(int i =0; i<16; i++)
	{
		sprintf(pDataID,"CONF_AD1_WRITE_CHL%d_PHYSICALLENGTH",i+1);
		ADConfig[i].wPhysicalLength = GetDBValue(pDataID).lValue;

		sprintf(pDataID,"CONF_AD1_WRITE_CHL%d_TRAVEL",i+1);
		ADConfig[i].wTravel = GetDBValue(pDataID).lValue;
	}

	int 	nFd= open("/conf/ADConfig.cdb", O_RDWR | O_CREAT);
	
	write(nFd, &ADConfig, sizeof(ADConfig));
	close(nFd);	
}

void	 CtmTaskRS232::ProcessADConfig()
{
	char	pDataID[128];
	//printf("m_bADInitial= %d\n", m_bADInitial);
	ReadADConfig();

	if(m_bADInitial) /*��󤣦s�b,�����Ĥ@���q�T,����DSP�ت�,�ëO�s*/
		{
			SaveADConfig();
		}
	else	/*���A�Y���۵�,�hŪ�����I�ت�; �M�Z�brun()���ǵ�DSP*/
		{
			for(int i =0; i<16; i++)
			{
				sprintf(pDataID,"CONF_AD1_WRITE_CHL%d_PHYSICALLENGTH",i+1);
				if(ADConfig[i].wPhysicalLength != GetDBValue(pDataID).lValue)	
				{
					m_bADNotEqual = TRUE;
					break;
				}					

				sprintf(pDataID,"CONF_AD1_WRITE_CHL%d_TRAVEL",i+1);
				if(ADConfig[i].wTravel != GetDBValue(pDataID).lValue)
				{
					m_bADNotEqual = TRUE;
					break;
				}
			}
			
			if(m_bADNotEqual)
			{
				for(int i =0; i<16; i++)
				{
					sprintf(pDataID,"CONF_AD1_WRITE_CHL%d_PHYSICALLENGTH",i+1);
					SetDBValue(pDataID, ADConfig[i].wPhysicalLength);
        	
					sprintf(pDataID,"CONF_AD1_WRITE_CHL%d_TRAVEL",i+1);
					SetDBValue(pDataID, ADConfig[i].wTravel);
				}
			}	
	//printf("getconfig m_bADNotEqual= %d\n", m_bADNotEqual);
		}	
				
}

void	 CtmTaskRS232::SetClmpShutOffset()
{
		long lClmpShutOrig = GetDBValue("MACH_SHUT_SHUT_NULL_W0268").lValue;/*�Үy��m ��h����I*/
		long lClmpShutDest = GetDBValue("MSET_SCRW_INJ_NULL_W0775").lValue; /*�Үy��m ��h�ؼЦ�m*/
		long lGearDist     = GetDBValue("MACH_SHUT_SHUT_NULL_W026D").lValue;/*�ԧ����Z*/
		long lTemp = 0;
		if(lGearDist)
		{	
		  if(lClmpShutDest > lClmpShutOrig)
		  	lTemp = (lClmpShutDest - lClmpShutOrig) % lGearDist;
		  else	
		  	lTemp = (lClmpShutOrig - lClmpShutDest) % lGearDist;
			
			if(lTemp > (lGearDist/2)) /*JOYCE2014-5-28 �Үy��m��h���t������Ȧp�G�j�_�ԧ����Z��1/2,�h�i��B�z, �O���i�Үy��m��h���t�j<= (�ԧ����Z/2)*/
			{	
				lTemp = lGearDist - lTemp; 
				bGearDist = TRUE;
			}
			else
				bGearDist = FALSE;
		} 
		else
			lTemp = 0;	
		
		SetDBValue("MSET_SCRW_INJ_NULL_W0774", lTemp);/*�Үy��m��h���t*/
}

void	 CtmTaskRS232::SetTBarShutDest()
{
	long long	lTemp1  = 0;
	long long	lTemp2 = GetDBValue("MSET_SCRW_INJ_NULL_W0774").lValue;/*�Үy��m��h���t*/
	//long long lTemp3 = GetDBValue("SYS_HMI_NULL_NULL_RESERVED42").lValue;/*�}�ҶZ��*/
	long lClmpShutOrig = GetDBValue("MACH_SHUT_SHUT_NULL_W0268").lValue;/*�Үy��m ��h����I*/
	long lClmpShutDest = GetDBValue("MSET_SCRW_INJ_NULL_W0775").lValue; /*�Үy��m ��h�ؼЦ�m*/
	
	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W0269").lValue;     /*�ԧ�1 ��h����I*/
	if(lClmpShutDest > lClmpShutOrig)
	{
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2*10;
		else	
			lTemp1  = lTemp1 + lTemp2*10;
	}
	else
	{		
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2*10;
		else
			lTemp1  = lTemp1 - lTemp2*10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0776", lTemp1);              /*�ԧ�1 ��h�ؼЦ�m*/
	//SetDBValue("MSET_SCRW_INJ_NULL_W077C", lTemp1 + lTemp3*10);  /*�ԧ�1 �}�ҥؼЦ�m*/
	 			
	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W026A").lValue; /*�ԧ�2 ��h����I*/
	if(lClmpShutDest > lClmpShutOrig)
	{
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2*10;
		else	
			lTemp1  = lTemp1 + lTemp2*10;
	}
	else
	{		
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2*10;
		else
			lTemp1  = lTemp1 - lTemp2*10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0777", lTemp1);
  //SetDBValue("MSET_SCRW_INJ_NULL_W077D", lTemp1 + lTemp3*10);

	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W026B").lValue; /*�ԧ�3 ��h����I*/
	if(lClmpShutDest > lClmpShutOrig)
	{
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2*10;
		else	
			lTemp1  = lTemp1 + lTemp2*10;
	}
	else
	{		
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2*10;
		else
			lTemp1  = lTemp1 - lTemp2*10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0778", lTemp1);
	//SetDBValue("MSET_SCRW_INJ_NULL_W077E", lTemp1 + lTemp3*10);

	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W026C").lValue; /*�ԧ�4 ��h����I*/
	if(lClmpShutDest > lClmpShutOrig)
	{
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2*10;
		else	
			lTemp1  = lTemp1 + lTemp2*10;
	}
	else
	{		
		if(bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2*10;
		else
			lTemp1  = lTemp1 - lTemp2*10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0779", lTemp1);
	//SetDBValue("MSET_SCRW_INJ_NULL_W077F", lTemp1 + lTemp3*10);
}

void	 CtmTaskRS232::SetTBarShutDest2()
{
	long long	lTemp1  = 0;
	long long	lTemp2 = GetDBValue("MACH_SHUT_SHUT_NULL_W026D").lValue;   /*�ԧ� ���Z*/
	long long lTemp3 = GetDBValue("SYS_HMI_NULL_NULL_RESERVED42").lValue;/*�}�ҶZ��*/
	
	lTemp1 = GetDBValue("METER_AD1_READ_CHL11_RELATIVEPOSITION").lValue;     /*�ԧ�1 �{�b��m*/
	if(lTemp1 > (lTemp2*10))
		lTemp1  = lTemp1 - lTemp2*10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0776", lTemp1);              /*�ԧ�1 ��h�ؼЦ�m*/
	SetDBValue("MSET_SCRW_INJ_NULL_W077C", lTemp1 + lTemp3*10);  /*�ԧ�1 �}�ҥؼЦ�m*/
	 			
	lTemp1 = GetDBValue("METER_AD1_READ_CHL12_RELATIVEPOSITION").lValue;     /*�ԧ�2 �{�b��m*/
	if(lTemp1 > (lTemp2*10))
		lTemp1  = lTemp1 - lTemp2*10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0777", lTemp1);
  SetDBValue("MSET_SCRW_INJ_NULL_W077D", lTemp1 + lTemp3*10);

	lTemp1 = GetDBValue("METER_AD1_READ_CHL15_RELATIVEPOSITION").lValue;     /*�ԧ�3 �{�b��m*/
	if(lTemp1 > (lTemp2*10))
		lTemp1  = lTemp1 - lTemp2*10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0778", lTemp1);
	SetDBValue("MSET_SCRW_INJ_NULL_W077E", lTemp1 + lTemp3*10);

	lTemp1 = GetDBValue("METER_AD1_READ_CHL16_RELATIVEPOSITION").lValue;     /*�ԧ�4 �{�b��m*/
	if(lTemp1 > (lTemp2*10))
		lTemp1  = lTemp1 - lTemp2*10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0779", lTemp1);
	SetDBValue("MSET_SCRW_INJ_NULL_W077F", lTemp1 + lTemp3*10);
}

void	 CtmTaskRS232::SaveTBARData()
{
	WORD wTBARData[20];
	char*   pDataID[] = {
						"MACH_SHUT_SHUT_NULL_W0268",
						"MACH_SHUT_SHUT_NULL_W0269",
						"MACH_SHUT_SHUT_NULL_W026A",
						"MACH_SHUT_SHUT_NULL_W026B",
						"MACH_SHUT_SHUT_NULL_W026C",
						"MACH_SHUT_SHUT_NULL_W026D",
						"MSET_SCRW_INJ_NULL_W0774",
						"MSET_SCRW_INJ_NULL_W0775",
						"MSET_SCRW_INJ_NULL_W0776",
						"MSET_SCRW_INJ_NULL_W0777",
						"MSET_SCRW_INJ_NULL_W0778",
						"MSET_SCRW_INJ_NULL_W0779",
						"MSET_SCRW_INJ_NULL_W077C",
						"MSET_SCRW_INJ_NULL_W077D",
						"MSET_SCRW_INJ_NULL_W077E",
						"MSET_SCRW_INJ_NULL_W077F",
						"SYS_HMI_NULL_NULL_RESERVED42",
						"MACH_MDH_MDH_NULL_W002C",
						"MACH_MDH_MDH_NULL_W002D",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED87",
            NULL
            };
	
   memset(wTBARData, 0, sizeof(wTBARData));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/TBARData");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/TBARData.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
    
    if (nFd >= 0)
		{
		for(int i=0; i<20;i++)
			{
			if(pDataID[i] != NULL)	wTBARData[i] = GetDBValue(pDataID[i]).lValue;
					//printf("wTBARData22[%d]:%d\n",i,wTBARData[i]);
			}
		}
	write(nFd, &wTBARData, sizeof(wTBARData));
	close(nFd);
}

void		CtmTaskRS232::SaveParallelData()		/*zhongjw 2015-4-13 11:07:32 �¤G�������t�p�ƩM�Ա������m���i���m*/
{
	DWORD   dwCntData[11];
	
	char*   pDataID[] = {
            			"MSET_OTHR_OTHR_NULL_W084C",
            			"MSET_OTHR_OTHR_NULL_W084E",
            			"MSET_OTHR_OTHR_NULL_W0850",
            			"MSET_OTHR_OTHR_NULL_W0853",
            			"MSET_OTHR_OTHR_NULL_W0854",
            			"MSET_OTHR_OTHR_NULL_W0855",
            			"MSET_OTHR_OTHR_NULL_W0856",
            			"SYSX_OTHERS_OTHERS_DWORD_RESERVED99",
            			"SYSX_OTHERS_OTHERS_DWORD_RESERVED100",
            			"SYSX_OTHERS_OTHERS_DWORD_RESERVED101",
            NULL
            };

   memset(dwCntData, 0, sizeof(dwCntData));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/ParallelCnt.cdb", szPath);
    
    int nFd = open(m_szCopyFile, O_RDWR | O_CREAT);    
    if (nFd >= 0)
		{
		for(int i=0; i<11 ;i++)
			{
			if(pDataID[i] != NULL)	dwCntData[i] = GetDBValue(pDataID[i]).lValue;
			}
		}
	
	write(nFd, &dwCntData, sizeof(dwCntData));
	close(nFd);	
}

int		CtmTaskRS232::SetUnitChangetoBar(char pDataID[256])
{
	char	pszID[128];
  int   nPVLTemp = 0;
	long  lTemp = 0, lValue = 0; 

	if(m_bDC && m_bSVRT) /*JOYCE20130427  �⪩�����O�b11�q�D,����⪩���b15�q�D*/
		sprintf(pszID,"CONF_PVL15_READ_NULL_MAXDATA");
	else		
		sprintf(pszID,"CONF_PVL11_READ_NULL_MAXDATA");
	
	if(GetDBValue(pszID).lValue)
		nPVLTemp = GetDBValue(pszID).lValue;
	else
		nPVLTemp = 140;

	if(pDataID != NULL)
		{
			lValue = GetDBValue(pDataID).lValue;
			if(GetDBValue("MACH_MLD_CLOSE_CLAMPUP_ALARMTONES").lValue)
				lTemp = (lValue * nPVLTemp *10 / GetDBValue("MACH_MLD_CLOSE_CLAMPUP_ALARMTONES").lValue + 5) / 10;/*�����OX*nPVLTemp/��������, �M�Z�|�٤��J*/
			else
				lTemp = 0;
			return 	lTemp;
		}        
		
	return lTemp;
}

/*�¤G�O------------------------------*/

void	 CtmTaskRS232::ProcessAmmeterEnergy()
{
	if(g_ptaskAmmeter == NULL) return;

	N2CAMMETERCONFIG	N2CAmmeterConf;
	N2CAmmeterConf = *((N2CAMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));

	COM2AMMETERCONFIG	COM2AmmeterConf;
	COM2AmmeterConf = *((COM2AMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));

	DWORD  	dwAmmeter1=0, dwAmmeter2=0, dwAmmeter3=0;
	
	if((m_bAmmeter == 2) || (m_bAmmeter == 3))
	{
		//Ammeter1
		dwAmmeter1 	= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;
		//printf("into auto dwAmmeter1=%d\n",dwAmmeter1);
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED69", dwAmmeter1);//Ammeter1
	
		//Ammeter2
		if(N2CAmmeterConf.wCount > 1)
		{	
			dwAmmeter2 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue;
			//printf("into auto dwAmmeter2=%d\n",dwAmmeter2);
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED70", dwAmmeter2);//Ammeter2
		}
	
		//Ammeter3
		if(N2CAmmeterConf.wCount > 2)
		{	
			dwAmmeter3 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue;
			//printf("into auto dwAmmeter3=%d\n",dwAmmeter3);
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED71", dwAmmeter3);//Ammeter3
		}
	}
	else
	{		
		//Ammeter1
		if(COM2AmmeterConf.AmmeterInfo[0].wCTPara != 0)
			dwAmmeter1 	= (GetDBValue("AMMETER_HMI_REAL_POWER").lValue) *COM2AmmeterConf.AmmeterInfo[0].wCTPara/10;
		else
			dwAmmeter1 	= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;
		//printf("into auto dwAmmeter1=%d\n",dwAmmeter1);
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED69", dwAmmeter1);//Ammeter1
	
		//Ammeter2
		if(COM2AmmeterConf.wCount > 1)
		{	
			if(COM2AmmeterConf.AmmeterInfo[1].wCTPara != 0)
				dwAmmeter2 	= (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue) *COM2AmmeterConf.AmmeterInfo[1].wCTPara/10;
			else
				dwAmmeter2 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue;
			//printf("into auto dwAmmeter2=%d\n",dwAmmeter2);
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED70", dwAmmeter2);//Ammeter2
		}
	
		//Ammeter3
		if(COM2AmmeterConf.wCount > 2)
		{	
			if(COM2AmmeterConf.AmmeterInfo[2].wCTPara != 0)
				dwAmmeter3 	= (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue) *COM2AmmeterConf.AmmeterInfo[2].wCTPara/10;
			else
				dwAmmeter3 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue;
			//printf("into auto dwAmmeter3=%d\n",dwAmmeter3);
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED71", dwAmmeter3);//Ammeter3
		}
	}
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
void        CtmTaskRS232::SendConfig()
{
	int         nWriteCount;
	WORD		wLength;
	g_frameTx.wCmd = COMM_SENDCONFIG;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_CONFIG_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
	
	g_lCounterLEDMotor = 1536;
}

void        CtmTaskRS232::SendZeroSet()
{
	int         nWriteCount;
	WORD		wLength;
	
	g_frameTx.wCmd = COMM_SENDZEROSET;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_ZSET_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskRS232::SendRequestConfig()   //wy040600 wy113099
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDREQUESTCONFIG;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskRS232::SendPanel(BYTE cKey)
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
//<<JOYCE2008-9-24 ADD  FOR 7GL
if(cKey == (BYTE)(_KEY_PANEL_MOLDADJ & 0x00FF)) //�ʽռ�
    {
    if(m_bUseSlow7GL)     return;
    else if(m_bAutoAdjust7GL)     return; 
    else
        {
        if(m_bAdjustUse7GL)	m_bAdjustUse7GL  = FALSE;
        else	m_bAdjustUse7GL  = TRUE;
        }
    }

if(cKey == (BYTE)(_KEY_PANEL_MOTIONSTART & 0x00FF))//�۰ʽռ�
    {              
    if(m_bUseSlow7GL)     return;
    else if(m_bAdjustUse7GL)     return; 
    else 
    	  {
    	  if(m_bAutoAdjust7GL == FALSE)	m_bAutoAdjust7GL = TRUE;
    	  }
  	}

if(cKey == (BYTE)(_KEY_PANEL_SLOWUSE & 0x00FF))//�ռҺC�t
    {
    if(m_bAdjustUse7GL)   return;
    else if(m_bAutoAdjust7GL)     return; 
    else
        {
        if(m_bUseSlow7GL)	m_bUseSlow7GL = FALSE; 
        else  m_bUseSlow7GL = TRUE;
        }
    }
if(cKey == (BYTE)(_KEY_PANEL_SLOWUSE & 0x00FF))  cKey = (BYTE)_KEY_PANEL_MOLDADJ;

if(cKey == (BYTE)(_KEY_PANEL_SUCKBACK & 0x00FF)) //�g�h
     m_bKeySuck7GL    = TRUE;
else m_bKeySuck7GL    = FALSE;

//>>JOYCE2008-9-24 ADD    	

	//JOYCE2011-1-8 <<<
	if(cKey == (BYTE)(_KEY_PANEL_LUBRICATION & 0x00FF))	 //���U���key
	{
			if(m_wLubrCnt == 0)
				m_wLubrCnt = 1;
	}
	else if(cKey == 0xFF)//���key ��}
	{
			if(m_wLubrCnt == 1)
				m_wLubrCnt = 2;
	}

	if(m_wLubrCnt == 2)			//Wait Echo 2011-1-8 16:57:21
		{
			return;
		}
	//printf("m_wLubrCnt=%d \n", m_wLubrCnt);
	//>>>>

/*******
|       Special control process
*******/
	lValue = GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue;//JOYCE2011-9-2 bug,��Ʀ��~  OPER_HMI_NULL_NULL_TIMEOUTSTATUS
	if (lValue)                  //Chuntzu 2004/5/4 add for �����\��
    if ((cKey == (BYTE)(_KEY_PANEL_SEMIAUTO & 0x00FF)) || (cKey == (BYTE)(_KEY_PANEL_SENSORAUTO & 0x00FF)) ||
         (cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF)) ) return;

	if( (cKey == (BYTE)(_KEY_PANEL_HEATEROFF & 0x00FF))
		&& ((GetDBValue("CONF_CODE_WRITE_NULL_DATA15").lValue & 0xFF00) == 0x7200))		//2009-2-3, fuxy, for �ߦ����q���w����
	cKey = 0x0095;

	if(m_bALogin) /*JOYCE2012-6-18 ��������۰ʪ��A��,�ݭn��J�K�X�T�{*/
	{		
		if((cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF)) && (GetDBValue("MSET_OTHR_OTHR_NULL_PHOTOSENSOROPTION").lValue != 1))
		{
			WORD	wOperMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;

			if(wOperMode & 0x0008)	/*���۰ʪ��A*/
			{
				return;
			}
			else
			{
					if(m_bAutoKey == FALSE)
					{
						m_bAutoKey = TRUE;
						int ntemp = g_WndLoginPrivildge;
						g_WndLoginPrivildge = 5;
						MsgForm("password.txt");
						
						g_WndLoginPrivildge =ntemp;
						m_bAutoKey = FALSE;

						if(g_bMsgFormKey == _KEY_CLEAR)
						{
							return;
						}
						else  if(g_bMsgFormKey == _ENTER)
						{
							if(g_iPrivilege < 5)
							{
								MsgBox(g_MultiLanguage["MSG_LOGIN_INVALIDPRIVILEGE"],3);
								return;
							}
							else
							{
								m_bSendAutoKey = TRUE;
							}
						}
					}
					else
					{
						return;
					}
			}
		}
	}

	if(m_bMUCELL) //JOYCE2011-4-26 �M�z�����Y��,�����F�q��key�~,��Lkey�L��
	{
		long long lValveTemp = GetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47").lValue;	//�j�s��X�]�֪�1�^value
		if(lValveTemp == 0x8001)
		{
    		if ((cKey != (BYTE)(_KEY_PANEL_MOTOR & 0x00FF)) && (cKey != (BYTE)(_KEY_PANEL_MOTOROFF & 0x00FF)) &&
       			 (cKey != (BYTE)(_KEY_PANEL_HEATER & 0x00FF)) && (cKey != (BYTE)(_KEY_PANEL_HEATEROFF & 0x00FF))) 
       		return;		
		}
	}

	if(l_PMTEST == 1)		//JOYCE20130327 for PumpTest
	{
		if(cKey == (BYTE)(_KEY_PANEL_SEMIAUTO & 0x00FF) || cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF) )	//fuxy, 2010-12-8, for  Auto
		{
			if(_ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) == 1)	//�۰ʪ��A  /*JOYCE2012-6-21 �L�o��4��bit-�D�ʧ@���A*/
				return;
			if(_ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) == 3)	//�b�۰ʪ��A  /*JOYCE2012-6-21 �L�o��4��bit-�D�ʧ@���A*/
				return;
			else
			{
				if(g_pszUserID == NULL)
				{
					if(bAutoKey == FALSE)
					{
						bAutoKey = TRUE;
						MsgBox(g_MultiLanguage["VW_NET_LOGININFIRST"],3);
						bAutoKey = FALSE;
						return;
					}
					else
						return;
				}
				else
				{
					if(bAutoKey == FALSE)
					{
						bAutoKey = TRUE;
						MsgForm("PumpType.txt");
						
						if(g_bMsgFormKey == _KEY_CLEAR)
						{
							bAutoKey = FALSE;
							return;
						}
						else  if(g_bMsgFormKey == _ENTER)
						{
							bAutoKey = FALSE;
							iProjNum = 0;
							SetDBValue("SYSX_SCREW_INJECT_NULL_PRECISIONOPTION", 0, FALSE);
							bKeyPumpStart = TRUE;
							m_bSendAutoKey = TRUE;/*JOYCE2014-3-31 �Ȥ���X �`��������L��,�d��]��:�S���e��}����0xFF���D���A�ҥH�i��ץ�*/
						}
					}
					else
						return;
				}
			}
		}
	}

//JOYCE2011-8-2 test
//	if(cKey == (BYTE)(_KEY_PANEL_MOLDTHIN & 0x00FF))	 ReqValues(0x8000);
//	if(cKey == (BYTE)(_KEY_PANEL_MOLDTHICK & 0x00FF))	 ReqValues(0x8002);
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

void        CtmTaskRS232::SendControlIndex(char* pszData)
{
	int	nIndex = g_pDatabase->GetIndex(pszData);
	if (nIndex >= 0)
		SendControlIndex(nIndex);
}

void        CtmTaskRS232::SendControlIndex(int nIndex)
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
    else if(m_bInjSpdUnit)		//wangli2014-8-20 16:20:01  inje/holdpre unit is mm/s
       	{
       	   if (CheckDataID(pszData,g_MSET_InjeHoldSpdID,sizeof(g_MSET_InjeHoldSpdID) / sizeof(g_MSET_InjeHoldSpdID[0])))
       	   {
       		if((lValue != 0xFFFF)&&(GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue != 0))
       			g_frameTx.commdata.controlindex.wValue = (DWORD)((lValue * 990/ GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue + 5) /10 );//�|�٤��J    	 	  	
    	 	else
    	 	  	g_frameTx.commdata.controlindex.wValue =lValue;
    	   }
    	 	else
    	 	  	g_frameTx.commdata.controlindex.wValue =lValue;
       	}
    else
    {    	
        g_frameTx.commdata.controlindex.wValue = lValue;
    }
    
    
    CMDDATA cmddata;
    
  //  printf("g_frameTx.commdata.controlindex.wIndex =%x\n",g_frameTx.commdata.controlindex.wIndex);
    if ((g_frameTx.commdata.controlindex.wIndex == 0x3FE) || (g_frameTx.commdata.controlindex.wIndex == 0x3FD)) //Gwei 2006/2/16 �֪��j���X,�ϥ�WMACHSPECIALRESERVED46�S��Ѽ�
    	{
    		if(!m_bCOM2Vlv) /*COM1�֪�*/
    		{	
	    		cmddata.wCmd   = DB_OTHER_CMD_ID;
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wValue;//�NValue�s�Jindex�ǥD��//g_frameTx.commdata.controlindex.wIndex;
      	}
      	else	if(m_bCOM2Vlv && (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED94").lValue == 0))/*COM2�֪��ʤO����ܨϥΥD��1*/
      	{
	    		cmddata.wCmd   = DB_OTHER_CMD_ID;
        	if(g_frameTx.commdata.controlindex.wValue > 0)
        		g_frameTx.commdata.controlindex.wValue = 2;/*JOYCE2013-6-24 �אּ�֪�2,�]��COM1�D���ɥ�PC�֪�1��X�@��COM2�֪��n����*/
        	cmddata.wIndex = g_frameTx.commdata.controlindex.wValue;//�NValue�s�Jindex�ǥD��//g_frameTx.commdata.controlindex.wIndex;
      	}	
    	}
    else if (g_frameTx.commdata.controlindex.wIndex < DB_MACH_MAXINDEX_ID)
        {   
        cmddata.wCmd   = DB_MACH_CMD_ID;
        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex;
        						
        }
    else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSET_MAXINDEX_ID)
        {   
        cmddata.wCmd   = DB_MOLDSET_CMD_ID;
        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MACH_MAXINDEX_ID;
        }
    else if (g_frameTx.commdata.controlindex.wIndex < DB_POWERMATCH_MAXINDEX_ID)
        {
        if(m_bNewDA)  //Andy 20061212
	        {
	        cmddata.wCmd   = DB_NEWPOWERMATCH_CMD_ID;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MOLDSET_MAXINDEX_ID + 0x100;        	
	        }
        else
	        {	
	        cmddata.wCmd   = DB_POWERMATCH_CMD_ID;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MOLDSET_MAXINDEX_ID;
    		}
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

    else if (g_frameTx.commdata.controlindex.wIndex < 0x1000)
        return;   
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
        if(m_bNewDA)
	        {
	        cmddata.wCmd   = 0x0027;
	        cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x18B9;
	        }
        else return;
        }
      else if ((g_frameTx.commdata.controlindex.wIndex == 0x19B9)||(g_frameTx.commdata.controlindex.wIndex == 0x19CA)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x19DB)||(g_frameTx.commdata.controlindex.wIndex == 0x19EC)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x19FD)||(g_frameTx.commdata.controlindex.wIndex == 0x1A0E)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x1A1F)||(g_frameTx.commdata.controlindex.wIndex == 0x1A30)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x1A41)||(g_frameTx.commdata.controlindex.wIndex == 0x1A52)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x1A63)||(g_frameTx.commdata.controlindex.wIndex == 0x1A74)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x1A85)||(g_frameTx.commdata.controlindex.wIndex == 0x1A96)
      				||(g_frameTx.commdata.controlindex.wIndex == 0x1AA7)||(g_frameTx.commdata.controlindex.wIndex == 0x1AB8))
        {           	
					if(m_bCOM2Vlv)/*JOYCE 20140508 COM2�֪�16�դ����ޭ��տ�ܨϥ�,���n�e1E7���Ȭ�1��COM1*/
	        {
						for(int i=0; i<16; i++)
						{
							char	pDataID[128];
							sprintf(pDataID,"GATE_GATE%d_GATE1_NULL_MODEOPTION",i+1);
							long lValOpt = GetDBValue(pDataID).lValue;
							if(lValOpt)
								{
									g_frameTx.commdata.controlindex.wValue = 1;
									break;
								}
						}
	        	cmddata.wCmd   = 0x0021;
	        	cmddata.wIndex = 0x1E7;
	        }
        	else return;
        }
      else if(g_frameTx.commdata.controlindex.wIndex == 0x8826)//wangli2014-9-5 11:26:43 �g�X���u�e�����g�X�t�׳̤j��
      	{
          if(m_bInjSpdUnit)		//wangli2014-8-20 16:20:01  inje/holdpre unit is mm/s,�i���촫��
     	  {	
		    if(g_ptaskdsp != NULL)
                  g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSET);
	      	}
	      else return;

      }
    else
        return;

	  if ((g_frameTx.commdata.controlindex.wIndex == 0x3FE) || (g_frameTx.commdata.controlindex.wIndex == 0x3FD))	/*JOYCE20130515�֪��j���X D_VALO*/ 
		{
		   	if(g_frameTx.commdata.controlindex.wValue == 0)
		   	{
		   		cmddata.wValue = 0x8000;
			}
		}
		else
	    cmddata.wValue = g_frameTx.commdata.controlindex.wValue;
    cmddata.wEcho  = 0;
    	
//    printf("%s=%d DWORD=%d\n", pszData, lValue, wType == TYPE_DWORD);
    PushCmd(&cmddata);
    if (wType == TYPE_DWORD)
    	{
    	wType = TYPE_WORD;
    	lID++;
    	lValue = wHValue;
    	//printf("SendControlIndex HIWORD=%d ID=%d \n", lValue, lID);
    	goto SENDAGAIN;
    	}
    }
}

void        CtmTaskRS232::SendControlMachine()
{
	int         nWriteCount;
	WORD		wValue;
	WORD		wLength;
	DWORD		dwValue = 0;//JOYCE2011-8-22 
	WORD		wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD		wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	{
			
	}
  wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue & 0xFFEF | 0x8000 | 0x1000 | 0x0400 | 0x0080 | 0x0040 | 0x0020; //Sunny<20070824> amend

	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION", wValue, FALSE);
	
	SetDBValue("MACH_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	
	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue | 0x0004;
	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);
	
	if(!g_pExTable->GetTableValue(0,"D_TM3160") && !g_pExTable->GetTableValue(0,"D_TM3360"))//JOYCE2011-8-25 //�t�X��Z replace 386 ,add judgement
	{
	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue | 0x0002;//motor/heater code
	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);
	}

	//if(((dbconf.wControlSequenceFlagPLC[6]&0xFF00) ==0x8400)&&((dbconf.wHydraulicType[6]&0x000F) != 0)) //Andy 20070205 for 7HT_J5 guard against theft !!
	if(((GetDBValue("CONF_CODE_WRITE_NULL_DATA6").lValue&0xFF00) ==0x8400)
	&&((GetDBValue("CONF_CODE_WRITE_NULL_W0AE6").lValue&0x000F) != 0)) //Andy 20070205 for 7HT_J5 guard against theft !!
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue | 0x0010;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	} 
	if (m_bDC) //James add 2008/8/13 09:42�W��
	{
		if (!g_pExTable->GetTableValue(0,"D_MIX2"))//���h�����ݭn
			{	
				wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue | 0x0008;
				SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);
			}		
		if (GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue >= 0x6106 || (GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue >= 0x2010))
			{
				wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
				wValue |= 0x0002;
				SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
			}	
		else
			{
				wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue;
				wValue |= 0x2000;
				SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION", wValue, FALSE);
			}			
	}
	//#endif
	dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;//JOYCE2011-8-22  	DSP54�p�}�Ҽ�
	SetDBValue("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", dwValue,FALSE);        //JOYCE2011-8-22
	//JOYCE2011-8-22 wValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	//JOYCE2011-8-22 SetDBValue("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", wValue, FALSE);
	
	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
	wValue |= 0x0008;//JOYCE2010-12-29 0x0010 |0x0008;
	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	if (GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue >= 0x9C10 || (GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue >= 0x2010))
		{
			wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
			wValue |= 0x0040;
			SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);			
		}

	if ((wDSPLIBDaysCode >= 0x1805) &&(wDSPLIBYearCode==0x2011)|| (wDSPLIBYearCode >= 0x2012))//JOYCE2011-8-22 shotcnt dsp54 cnt
		{
			wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
			wValue |= 0x0200;
			SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);			
		}

	if(!m_bDC && ((wDSPLIBDaysCode >= 0x2B27)&&(wDSPLIBYearCode==0x2012)||(wDSPLIBYearCode >= 0x2013)))	//2012-12-11, >= 2B27 
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED16").lValue)		//���O�պA�e�� �h�����Ұ��d ��� 
			wValue |= 0x2000;
		else
			wValue &= 0xDFFF;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}
	else
	{
		SetDBValue("MSET_EJT_EJT_NULL_W05B8", 0xFFFF);	//�h�����Ұ��d 		
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED16", 0, FALSE);	//���O�պA�e�� �h�����Ұ��d ��� 
		
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue &= 0xDFFF;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}

	if(!m_bDC && ((wDSPLIBDaysCode >= 0x3305)&&(wDSPLIBYearCode==0x2013)||(wDSPLIBYearCode >= 0x2014)))/*joyce20130314 �ʴ��W�[�s�\��*/
		{
			wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
			wValue |= 0x4000;
			SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);			

			SetDBValue("MCONF_SCREW_INJECT_NULL_INJECHECKOPTION", 0, FALSE); /*�g�X�ʱ��ﶵ����*/
			SetDBValue("MACH_SCRW_CUSHION_NULL_OPTION", 0, FALSE);
		}
	else
		{
			wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
			wValue &= 0xBFFF;
			SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);			
		}	

	if(!m_bDC)  /*JOYCE20130515�֪��j���X D_VALO*/
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue |= 0x1000;	
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}

	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED6").lValue)
	{
    	SetDBValue("MACH_OTHR_OTHR_NULL_W01A9", GetCurrentMoldMedia(), FALSE);//wangli2014-7-3 14:02:07 �e�Ҹ����D��
	}

	dwValue = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
	if(dwValue & 0x0100) //�Yerror�w�s�b�F�~�h�M�����I�i�K�o�h�e�@����
	{
    	dwValue &= 0xFEFF;
    	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue,FALSE);
	}
	g_frameTx.wCmd = COMM_SENDCONTROLMACHINE;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID);
	
	//James add 2008/4/28 06:54�U�� 
//	if(1 == CtmConfig::GetInstance()->GetKeyFlag())
//	{
		//if (GetOilTable()) //���G�O���|�����D
		TransferOilVat(TRUE);
//	}
	
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
	m_bMachineSend = TRUE;
}

void        CtmTaskRS232::SendControlMoldset()
{
	int         nWriteCount;
	WORD				wLength,wDataID,wIndex,wSDOPNPOS,wSDPuslePOS,wSDPusleCNT;
	char*				pszID;
	
	SetDBValue("MSET_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	g_frameTx.wCmd = COMM_SENDCONTROLMOLDSET;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID);

     if(m_bInjSpdUnit)		//wangli2014-8-20 16:20:01  inje/holdpre unit is mm/s
       	{
		long lValue;
			for (int i=0;  i < (int)(sizeof(g_MSET_InjeHoldSpdID) / sizeof(g_MSET_InjeHoldSpdID[0])); i++)	
			{
				wDataID = g_pDatabase->GetDataID(g_MSET_InjeHoldSpdID[i]);
				lValue = GetDBValue(g_MSET_InjeHoldSpdID[i]).lValue;
				//printf("SYSX_INJECT_INJECT_NULL_MAXFLOW =%d\n",GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue);
				if((lValue != 0xFFFF)&&(GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue != 0))
					lValue = (lValue * 990 / GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue + 5)/10;
				wIndex = g_pDatabase->DataIDToIndex(wDataID);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID,lValue);
			}
       	}

	if(GetDBValue("MACH_SCRW_RCV_NULL_W0066").lValue)
		{
			long lValue;
			wDataID = g_pDatabase->GetDataID("MSET_SCRW_DCMP_NULL_BEFORERECOVERYOFFSET");
			
			lValue = GetDBValue("MSET_SCRW_DCMP_NULL_BEFORERECOVERYOFFSET").lValue;
			
			//printf("moldset : lValue=%d\n",lValue);
			if((lValue != 0xFFFF)&&(lValue < 10))
       				lValue = 10;
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID,lValue);
		}
	
	if (m_bDC) //James add 2008/8/13 09:42�W��
	{
		//printf("SelTempSendValue \n");
		SelTempSendValue();
	}
	
	//�B�z�ߨR���q�ʪ�
	if(m_bSDOOR)//JOYCE2011-11-3
		{
			wSDOPNPOS   = GetDBValue("MSET_SCRW_DCMP_NULL_W079F").lValue;//�w�����}�צ�m
			wSDPuslePOS = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED81").lValue;//��ߨR��{
			if(wSDPuslePOS)
				wSDPusleCNT = wSDOPNPOS/wSDPuslePOS;
			else	
				wSDPusleCNT = 0;

			wDataID = g_pDatabase->GetDataID("MSET_SCRW_DCMP_NULL_W079F");//�w�����}�צ�m
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, wSDPusleCNT);
		}
	if(m_b2SDOOR)
		{
			wSDOPNPOS   = GetDBValue("MSET_SCRW_DCMP_NULL_W079D").lValue/400;//�w�����}�צ�m
			wDataID = g_pDatabase->GetDataID("MSET_SCRW_DCMP_NULL_W079D");//�w�����}�צ�m
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, wSDOPNPOS);
		}
		
  if(m_bSVRT)
  {
  	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED104").lValue)
  	{
  		long lLimit = SetUnitChangetoBar("SYSX_OTHERS_OTHERS_INT_RESERVED75");
			long lTemp  = SetUnitChangetoBar("MSET_MLD_OPEN_NULL_W074D");
			long lSub   = lTemp - lLimit;
			if(lSub < 0)
				lSub = 0;
			SetDBValue("MSET_MLD_OPEN_NULL_W074F", lSub,FALSE);
	    g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, "MSET_MLD_OPEN_NULL_W074F", lSub);
			
			wDataID	= g_pDatabase->GetDataID("MSET_MLD_OPEN_NULL_W074D");
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, lTemp);
			
		}
 }
 
	
	//�B�z�ҷ�
	//long	lMoldTempType;
	char	psz[5];
	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//�P�_�ҷż���
	switch(psz[2])
	{
		case	'0':
			break;
		case	'1'://D_MDT10  com1, channel 11-20 
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=10; i<20; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
			break;
		case	'2':
			break;
		case	'3'://D_MDT4 	//JOYCE2010-7-20 add   com1, channel 6-9
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=5; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'4'://D_T3M6  com1, channel 4-9
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=3; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'5'://D_T3M5  com1, channel 4-8    //JOYCE2011-10-19 
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=3; i<8; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'6':
			break;
		case	'7':
			break;
		case	'8':
			break;
		case	'9'://D_T7M2  com1, channel 8-9
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=7; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}			
			break;
		case	'A':
			break;
		case	'B':
			break;
		case	'C':
			break;
		case	'D'://D_T5M14   com1, channel 6-9&&11-20
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=5; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
					for (int i=10; i<20; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}						
			break;
		case	'E': //JOYCE2011-2-17 COM1 2�q�ҷ�(CH8~9),40�qCOM2�ҷ�
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=7; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}			
			break;
		case	'F':	//D_T5M3   //JOYCE2011-5-20  com1, channel 6-8
				if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=5; i<8; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'H'://D_T12M4   com1, channel 7-8&&17-18 //JOYCE2011-6-10 
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=6; i<8; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
					for (int i=16; i<18; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}						
			break;
		case	'I'://D_T14M4   com1, channel 8-9&&18-19 //JOYCE2012-5-31 
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=7; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
					for (int i=17; i<19; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}						
			break;
		case	'J'://com1, channel 4-9&&11-20
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=3; i<9; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
					for (int i=10; i<20; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}						
			break;
		case	'K'://com1, channel 1-8    //JOYCE2012-9-11 
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=0; i<8; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'L':	//D_T5M2    //JOYCE2013-12-4   com1, channel 6-7
				if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=5; i<7; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'M':	//D_T6M2    //JOYCE2013-12-4   com1, channel 7-8
				if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=6; i<8; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'P':	//wangli2014-12-3 9:42:43  T11M4     com1, channel 13-16
				if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=12; i<16; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}
			break;
		case	'Q':/*wangli2015-6-1 13:30:54 T16M2 com1, channel 8,18*/
			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
				{
					wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
					for (int i=7; i<8; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
					for (int i=17; i<18; i++)	
						{
							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
							pszID = g_pDatabase->GetString(wIndex);
							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
						}	
				}						
			break;
		default:
			break;	
	}
	//#endif

	if(m_bCOM2Vlv)/*JOYCE 20140508 COM2�֪�16�դ����ޭ��տ�ܨϥ�,���n�e1E7���Ȭ�1��COM1*/
	{	
		for(int i=0; i<16; i++)
		{
			char	pDataID[128];
			sprintf(pDataID,"GATE_GATE%d_GATE1_NULL_MODEOPTION",i+1);
			long lValOpt = GetDBValue(pDataID).lValue;
			if(lValOpt)
				{
					wDataID = g_pDatabase->GetDataID("MSET_GATE2_GATE_NULL_MODEOPTION");
					wIndex = g_pDatabase->DataIDToIndex(wDataID);
					pszID = g_pDatabase->GetString(wIndex);
					g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 1);
					break;
				}
		}
	}
	

	if (GetOilTable()) TransferOilVat();
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);	

	//wangli2014-2-27 15:37:55  for���o�����աA��ǹq�y�Ȧb�}���ɡA�g�L�p��Z�ǵ��D���A���O�ۨ��Ȥ���
   if(l_PMTEST)
   	{
   		//printf("pmt\n");
   		char    pDataID0[64],pDataID1[64];
   		double  lTemp1;
   		long   Temp_CurrentRefer,Temp_CurrentPercent;
   	    for(int i=0 ;i <20 ;i++)
    	    {
    		if (i < 5 )
    			sprintf(pDataID0,"MSET_OTHR_OTHR_NULL_MOLDSPECIAL%dDATA",28+i);
    		else if(( i > 4 )&&( i < 10))
    			sprintf(pDataID0,"MSET_OTHR_OTHR_NULL_MOLDSPECIAL%dDATA",7+i-5);   			
    		else if(i > 9)
    			sprintf(pDataID0,"MSET_GATE%d_OPEN_NULL_WHILEINJECTTIME",1+i-10);


    		if (i < 10 )
    			sprintf(pDataID1,"MSET_OTHR_OTHR_NULL_MOLDSPECIAL%dDATA",33+i);
     		else if(i > 9)
    			sprintf(pDataID1,"MSET_GATE%d_OPEN_NULL_WHILEINJECTPOSITION",1+i-10);
   			
   			Temp_CurrentRefer=GetDBValue(pDataID0).lValue;		
   			Temp_CurrentPercent=GetDBValue(pDataID1).lValue;	
   			
   		    lTemp1 =Temp_CurrentRefer * (100 + Temp_CurrentPercent ) /100;
   		   // printf("lTemp1 =%d\n",(long)lTemp1);
   			SetDBValue(pDataID0, (long)lTemp1);
			SetDBValue(pDataID0, Temp_CurrentRefer, FALSE);
   		}    		
   	}

}

void        CtmTaskRS232::SendControlMonitor()
{
	int         nWriteCount;
	WORD		wLength, wOldValue1, wOldValue2;
	
	g_frameTx.wCmd = COMM_SENDCONTROLMONITOR;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID);
	//printf("SendControlMonitor Len=%X\n", wLength);
	if (GetOilTable())
    {
    	//wangli2014-12-1 15:45:33 �}�Ҳ��I�[�J�ʴ����ءAbit15�Q�α��A�ݭn�L�o
	    wOldValue1 = GetOilVat(GetDBValue("MONI_ALARM_ON_NULL_MAXOPENPOSITION").lValue & 0x7FFF) 
	                 | (GetDBValue("MONI_ALARM_ON_NULL_MAXOPENPOSITION").lValue & 0x8000);
	    wOldValue2 = GetOilVat(GetDBValue("MONI_ALARM_ON_NULL_MINOPENPOSITION").lValue & 0x7FFF)
	    			 | (GetDBValue("MONI_ALARM_ON_NULL_MINOPENPOSITION").lValue & 0x8000);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID, "MONI_ALARM_ON_NULL_MAXOPENPOSITION", wOldValue1);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID, "MONI_ALARM_ON_NULL_MINOPENPOSITION", wOldValue2);
    }
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskRS232::SendControlMachineB()
{
	int         nWriteCount;
	WORD		wLength;
	
	SetDBValue("MACHB_SCRW2_RCV_NULL_CHECKDATA", 0xA596, FALSE);
	g_frameTx.wCmd = COMM_SENDCONTROLMACHINEB;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MACHINEB_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskRS232::SendControlMoldsetB()
{
	int         nWriteCount;
	WORD		wLength,wDataID,wIndex;
	char*				pszID;
	
	SetDBValue("MSETB_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	g_frameTx.wCmd = COMM_SENDCONTROLMOLDSETB;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MOLDSETB_ID);
	//<<yang 2006/12/6 if (GetOilTable()) TransferOilVat();
	
	if (m_bDC && GetDBValue("MACHB_SCRW2_RCV_NULL_W1014").lValue)
		{
		long lValue;
		wDataID = g_pDatabase->GetDataID("MSETB_SCRW2_DCMP_NULL_BEFORERECOVERYPOSITION");
		lValue = GetDBValue("MSETB_SCRW2_DCMP_NULL_BEFORERECOVERYPOSITION").lValue;
		//printf("sendcontrol molssetb :lValue=%d\n",lValue);
		if((lValue != 0xFFFF)&&(lValue < 10))
   				lValue = 10;
		wIndex = g_pDatabase->DataIDToIndex(wDataID);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSETB_ID, pszID,lValue);
		}

	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

void        CtmTaskRS232::SendControlMonitorB()
{
	int         nWriteCount;
	WORD		wLength;
	
	g_frameTx.wCmd = COMM_SENDCONTROLMONITORB;
	wLength = g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMINB_ID);
	//printf("2 wLength: %X\n",wLength);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

WORD  		CtmTaskRS232::GetCurrentMoldMedia()
{
 WORD wTemp = 0;
 WORD m_nCurrentIndex = 0;
 
 m_nCurrentIndex = g_pDatabase->GetIndex(CURRENT_MOLD_MEDIA);
 wTemp = g_pDatabase->Readw(m_nCurrentIndex);
// printf("%s dataIndex = %d, wTemp=%d\n", CURRENT_MOLD_MEDIA, m_nCurrentIndex, wTemp);
  
 return wTemp;
}

void        CtmTaskRS232::SendPowerMatch()  // St1108'01 for D/A 
{
	int         i,j;
int         nWriteCount;
WORD		wLength, wDa_flag;
WORD		wValue, wIndex, wStartID, wStartTempID;

if (m_bNewDA)
	{
	wStartID	 = g_pDatabase->GetDataID("PMATCH2_PVL1_READ_CHL1_DATA");
	wStartTempID = g_pDatabase->GetDataID("DAOUT2_PVL1_WRITE_CHL1_DATA");
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
				//SetDBValue(wIndex, wValue, FALSE);
				}
			}
	    }
	}


	wStartID	 = g_pDatabase->GetDataID("PMATCH_PVL1_READ_CHL1_DATA");
	wStartTempID = g_pDatabase->GetDataID("DAOUT_PVL1_WRITE_CHL1_DATA");
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
				//SetDBValue(wIndex, wValue, FALSE);
			}
		}
    }

	if(m_bNewDA)  //Andy 20061212 for New D/A
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

int        CtmTaskRS232::SendDiagRM(DIAGRM diagrm)
{
	int         nWriteCount;
	m_nSendRM++;
	g_frameTx.wCmd = COMM_SENDDIAGRM;
	g_frameTx.commdata.diagrm.wAddressROM = diagrm.wAddressROM;
	g_frameTx.commdata.diagrm.wAddressRAM = diagrm.wAddressRAM;
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagrm)+2), &nWriteCount);
	return m_nSendRM;	//	fans add
}

void        CtmTaskRS232::SendDiagWM(DIAGWM diagwm)
{
	int         nWriteCount;
	WORD				wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD				wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	g_frameTx.wCmd = COMM_SENDDIAGWM;       //>>> wy070799
	g_frameTx.commdata.diagwm.wAddress 	= diagwm.wAddress;
	g_frameTx.commdata.diagwm.wData 	= diagwm.wData;
	if((wDSPLIBDaysCode>=0x9307)&&(wDSPLIBYearCode==0x2009)||(wDSPLIBYearCode>=0x2010))
	{
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0040;
    cmddata.wIndex = g_frameTx.commdata.diagwm.wAddress;
    cmddata.wValue = g_frameTx.commdata.diagwm.wData;
    cmddata.wEcho  = 0;
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
	}
	else
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagwm)+2), &nWriteCount);
}

void        CtmTaskRS232::SendDiagSM()
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGSM;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskRS232::SendDiagRI(DIAGRI diagri)
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGRI;
	//g_frameTx.commdata.diagri = diagri;
	g_frameTx.commdata.diagri.wAddress = diagri.wAddress;
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagri)+2), &nWriteCount);
}

void        CtmTaskRS232::SendDiagWO(DIAGWO diagwo)
{
	int         nWriteCount;
	WORD				wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD				wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	
	g_frameTx.wCmd = COMM_SENDDIAGWO;
	g_frameTx.commdata.diagwo.wAddress 	= diagwo.wAddress;
	g_frameTx.commdata.diagwo.wData 		= diagwo.wData;
	if((wDSPLIBDaysCode>=0x9307)&&(wDSPLIBYearCode==0x2009)||(wDSPLIBYearCode>=0x2010))
	{
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
	else
		WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagwo)+2), &nWriteCount);
}

void        CtmTaskRS232::SendDiagWO(WORD wAddress, WORD wData)
{
	DIAGWO      diagwo;
	
  diagwo.wAddress = wAddress;
	diagwo.wData    = wData;
	SendDiagWO(diagwo);
}

void        CtmTaskRS232::SendDiagSI()
{
	int         nWriteCount;
	
	g_frameTx.wCmd = COMM_SENDDIAGSI;
	WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
}

void        CtmTaskRS232::SendAssignInput(WORD wOldAddress, WORD wNewAddress)
{
	int         nWriteCount;
	WORD				wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD				wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	
	g_frameTx.wCmd = COMM_SENDASSIGNINPUT;
	g_frameTx.commdata.assignio.wOldAddress = wOldAddress;
	g_frameTx.commdata.assignio.wNewAddress = wNewAddress;
	if((wDSPLIBDaysCode>=0x9307)&&(wDSPLIBYearCode==0x2009)||(wDSPLIBYearCode>=0x2010))
	{
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0042;
    cmddata.wIndex = g_frameTx.commdata.assignio.wOldAddress;
    cmddata.wValue = g_frameTx.commdata.assignio.wNewAddress;
    cmddata.wEcho  = 0;
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
	}
	else
	WriteToken((BYTE*)&g_frameTx, (4+2), &nWriteCount);
}

void        CtmTaskRS232::SendAssignOutput(WORD wOldAddress, WORD wNewAddress)
{
	int         nWriteCount;
	WORD				wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD				wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	
	g_frameTx.wCmd = COMM_SENDASSIGNOUTPUT;
	g_frameTx.commdata.assignio.wOldAddress = wOldAddress;
	g_frameTx.commdata.assignio.wNewAddress = wNewAddress;
	if((wDSPLIBDaysCode>=0x9307)&&(wDSPLIBYearCode==0x2009)||(wDSPLIBYearCode>=0x2010))
	{
		CMDDATA cmddata;
		cmddata.wCmd   = 0x0043;
    cmddata.wIndex = g_frameTx.commdata.assignio.wOldAddress;
    cmddata.wValue = g_frameTx.commdata.assignio.wNewAddress;
    cmddata.wEcho  = 0;
    /*if (m_bMachineSend)*/ PushCmd(&cmddata);
	}
	else
	WriteToken((BYTE*)&g_frameTx, (4+2), &nWriteCount);
}

void        CtmTaskRS232::SendStatus(WORD wStatus)
{
	int         nWriteCount;
	WORD		wValue;
	char*		psz = "MACH_OTHR_OTHR_NULL_WERRORSTATUS1";

	wValue = GetDBValue(psz).lValue;
	wValue |= wStatus;
	
	SetDBValue(psz, wValue);
	
	//SendControlIndex(psz);              //Chuntzu 2004/10/11 add for send for 0x1C8
	
	g_frameTx.wCmd = COMM_SENDSTATUS;
	g_frameTx.commdata.wStatus = wStatus;
	WriteToken((BYTE*)&g_frameTx, (2+2), &nWriteCount);
}

void        CtmTaskRS232::SendRProfile01()
{
}

void        CtmTaskRS232::SendSProfile01()
{
	int         nWriteCount;
	if (m_cState == COMM_STATE_RUN)
	{
		m_cStateRx &= ~COMM_STATE_WAIT01;       // Clear Request Profile flag in case of communication error ???
		m_cStateRx |=  COMM_STATE_WAIT02;
		m_lCounterW1 = 512;
		m_bSendRProfile01 = FALSE;
		g_frameTx.wCmd = COMM_SENDS_PROFILE01;
		WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
	}
}

void        CtmTaskRS232::SendRBProfile01()
{
}

void        CtmTaskRS232::SendSBProfile01()
{
	int         nWriteCount;
	if (m_cState == COMM_STATE_RUN)
	{
		m_cStateRx &= ~COMM_STATE_WAITB01;       // Clear Request Profile flag in case of communication error ???
		m_cStateRx |=  COMM_STATE_WAITB02;
		m_lCounterBW1 = 512;
		m_bSendRBProfile01 = FALSE;
	
		g_frameTx.wCmd = COMM_SENDS_BPROFILE01;
		WriteToken((BYTE*)&g_frameTx, (0+2), &nWriteCount);
	}
}

void        CtmTaskRS232::SendCommand()               //St0512'03
{
	int         nWriteCount;

	g_frameTx.wCmd             = COMM_SENDCOMMAND;
	
	//James modify 2008/6/10 04:40�U��
	g_frameTx.commdata.cmddata.wCmd 	= m_CmddData.wCmd;
	g_frameTx.commdata.cmddata.wIndex 	= m_CmddData.wIndex;
	g_frameTx.commdata.cmddata.wValue 	= m_CmddData.wValue;
	g_frameTx.commdata.cmddata.wEcho 	= m_CmddData.wEcho;
	//printf("send commandwCmd=%x,wIndex=%x,wValue=%x,wEcho=%x\n",m_CmddData.wCmd,m_CmddData.wIndex,m_CmddData.wValue,m_CmddData.wEcho);	
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.cmddata)+2), &nWriteCount);
}

void        CtmTaskRS232::SendBlock61()
{
	int         nWriteCount;
	WORD				nIndex,wID,wDataCount=0;
	
	g_frameTx.wCmd = 0x3330;	
	for(int i=0;i<30;i++)
		{
			if(g_apszEffectPosiID[i] != NULL)
				{
					nIndex	= g_pDatabase->GetIndex(g_apszEffectPosiID[i]);
					wID			= g_pDatabase->IndexToDataID(nIndex);
					if(wID >= 0x0400)	wID = wID-0x400 + 0x1000;
					g_frameTx.commdata.block61.wIndexData[i*2]		= wID;
					g_frameTx.commdata.block61.wIndexData[i*2+1]	= GetDBValue(g_apszEffectPosiID[i]).lValue;	
					wDataCount++;
					//printf("i:%d,ID:%4X,Value:%d,wDataCount:%d\n",i,g_frameTx.commdata.block61.wIndexData[i*2],g_frameTx.commdata.block61.wIndexData[i*2+1],wDataCount);
				}
			else	break;
		}	
	g_frameTx.commdata.block61.wDataCount	= wDataCount;
	WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.block61)+2), &nWriteCount);
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmTaskRS232::GetColdStart()
{
	m_bColdStartFlag = TRUE;/*JOYCE2013-9-12*/
	u_nMicroAdjCount= 0;					//LEO20070621
	u_nAutoAdjCount = 0;					//LEO20070621
	Echo(COMM_ECHO_SUCCESS);
	SendZeroSet();
	m_cState = COMM_STATE_RESET;
	m_cStateRx &= ~COMM_STATE_WAIT00;
    {
	    g_flMotorStatus  = 0;
	    g_flHeaterStatus = 0;
    }

		if(m_bMUCELL) //JOYCE2011-4-26 �M�z�����Y
			{
				if(GetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47").lValue == 0x8001)
					SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47",0x8000);//�j�soff PC�]�֪�1�^
			}

	if(!m_bDC)  /*JOYCE20130515�֪��j���X D_VALO*/
	{
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED90", 0);	//all valve opt 
		SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47", 0);	//each valve opt 
	}
}

void	CtmTaskRS232::GetWarmStart()
{
	//=======
	//      Special control process
	//=======
	u_nMicroAdjCount= 0;					//LEO20070621
	u_nAutoAdjCount = 0;					//LEO20070621
	g_WarmStartFlag=TRUE; //Gwei 2005/5/20�@���E�_�ΡA�i���s�e�ݨD�ȵ��D��
	m_bWarmStartFlag =TRUE;//joyce2012-12-19

	if (g_flMotorStatus) 
    {
    	PanelMotorOn();
    }
 	else
    {
    	PanelMotorOff();
    }
	//James add �i�Ddsp28�����
	if(g_ptaskdsp28 != NULL)
	{
		//printf("Write to dsp28 Handl \n");
		int	nCommandHand	= 0x12345676;
		g_ptaskdsp28->WriteValue(6, 1, (int *)&nCommandHand);
	}
//<<JOYCE2008-9-24 add for 7GL  ��ʽƦ�
    if(m_bUseSlow7GL)//�ռҺC�tFLAG
    	{
    	m_bUseSlow7GL    = FALSE;
    	}
    if(m_bAdjustUse7GL)//�ʽռ�FLAG
    	{
    	m_bAdjustUse7GL  = FALSE;
    	}
    if(m_bAutoAdjust7GL)//�۰ʽռ�FLAG
    	{
    	m_bAutoAdjust7GL = FALSE;
    	}
//>>JOYCE2008-9-24 add

//JOYCE2011-2-11  add for austone
	if(g_pExTable->GetTableValue(0,"D_AUSTONE"))
	{
	  	u_bManualKey = TRUE;
    	m_lCounter = COMM_TIME_RESET *2;
   	}
/*******
|       Normal process
*******/
	Echo(COMM_ECHO_SUCCESS);
	m_cState = COMM_STATE_RUN;
	m_cStateRx &= ~COMM_STATE_WAIT00;

	if (m_bSendRProfile01)  SendRProfile01();
	//#ifdef   D_DC
	if (m_bDC) //James add 2008/8/13 09:42�W��
	{
		if (m_bSendRBProfile01) SendRBProfile01();
	}
	//#endif
//	if((CtmConfig::GetInstance()->GetDSP28Flag() == 1)&&(CtmConfig::GetInstance()->GetMachineType() == 0x1001))	
//		{
//			long long	lDSP28posi,lDSP54posi,lDelta;
//			lDSP28posi = GetDBValue("MACH_OTHR_OTHR_NULL_W0377").lValue;
//			lDSP54posi = GetDBValue("METER_AD1_READ_CHL2_RELATIVEPOSITION").lValue;
//			lDelta		 = (lDSP28posi - lDSP54posi*10)/10;
//			//printf("d2:%lld,d5:%lld,de:%lld\n",lDSP28posi,lDSP54posi,lDelta);
//			if((lDelta <= -10)||(lDelta >= 10))
//				{
//					WORD		offset;
//					offset  = (WORD)lDelta;
//					//printf("offset:%d,Delta:%ld\n",offset,lDelta);
//					SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA17",offset);
//	    	}	
//		}
	
	//printf("s1: %d, s2: %d, m_lCounter :%ld\n",wSendMoldStatus,wSendConfStatus,g_lCounterLEDMotor);
	if(wOldCMD != COMM_GETWARMSTART)
		{
			//printf("m_lCounter :%ld\n",g_lCounterLEDMotor);
		if(wSendMoldStatus == 1)
			SendControlMoldset();
		
		if(wSendBlockStatus == 1)
			SendBlock61();
		
		if((wSendConfStatus == 1)&&(g_lCounterLEDMotor > 0))
			SendConfig();
		else if((wSendConfStatus == 1)&&(g_lCounterLEDMotor < 0))//�N���Ե����F�a�A�L�������A�ɤB�[�ɤB
			wSendConfStatus = 2;

		if(m_bMUCELL) //JOYCE2011-4-26 �M�z�����Y
			{
				if(GetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47").lValue == 0x8001)
					SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA47",0x8000);//�j�soff PC�]�֪�1�^
			}

		if(l_PMTEST == 1)		//JOYCE20130327 for PumpTest
	   	bMannulKey = TRUE;
		}	
}

void  CtmTaskRS232::GetConfig()
{
	short       i;
	short       j;
	DWORD		dwValue, dwValue1, dwValue2;
	char*		pszID1 = "CONF_CODE_WRITE_NULL_DATA15";
	char*		pszID2 = "CONF_TMP1_HEATERON_NULL_MAXDEGREE";
/*******
|       Ignore the error configuration
*******/
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_CONFIG_ID, pszID1, &dwValue1);
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_CONFIG_ID, pszID2, &dwValue2);
	
	if( (dwValue1 == 0) || (dwValue2 == 0)) return;   // Maximum temperature        // Version code

    g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_CONFIG_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);

	if(m_bSysReset)
    {
    	if (g_pDefault != NULL)
    		g_pDefault->SetDefault();   
    	m_bSysReset = FALSE;
    	SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION",0);	//20071120 add�ѨM�C�����ҧ����m���X���S�����
    	if (GetOilTable()) SetVatDefPosi();
			SetDefaultEffectPos();//JOYCE2011-8-26 
    }

	if(m_bSVRT) /*JOYCE2014-1-17  �¤G�O,�q�l�ؤ��i���m*/ 
		{
			if(m_bWarmStartFlag)
			{
				m_bWarmStartFlag =FALSE;
			}
			else	
			{	
				ProcessADConfig();
			}
		}

	if (!bGotConfig)    //<WY2002/11/28 
    {
	    bGotConfig = TRUE;										//wy040600
	    ::DeleteCounter((long *)&m_lCounterReqConfig, m_idMe);	//wy040600
		SendControlIndex("MACH_TMP1_HEATERON_WARM_OPTION");
    	if (GetOilTable())
        {
        	/*JOYCE2013-9-12 mark,�ϥγ�ǫ��O�ǿ�35�����,�ɭP�}����key�ƭ�,��DSP�ܺC;�p�GDSP������,SendControlMoldset()�����B�z���O�y����,�ҥH�i�H�������B�z�覡,�אּ�u��HMI��W���Үɤ~����
        	dwValue = GetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION").lValue;
        	if (!dwValue) SendOilVat();
       		SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION", FALSE, FALSE);
        	*/
        	if(m_bColdStartFlag == FALSE)		SendOilVat(); /*JOYCE2013-9-12 HMI��W���Ү�,�������B�z�覡,�O��HMI/DSP��Ƥǰt*/
        }

    /* //JOYCE2012-1-17  �ץ����l���İϦb���m�����ҰʥX�{���~���D*/
    long long   opposite   = OilToPosi(GetDBValue("CONF_CODE_READ_CHL2_MAXPOSITION").lValue);
    long long   llOpeMaxPos= GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue;
    if(( llOpeMaxPos > opposite) && opposite )//wangli2014-12-18 11:55:57 opposite !=0
    	SetDBValue("MSET_MLD_OPEN_STEP5_POSITION", opposite, FALSE);

		SetCoreEffectPN(FALSE);
		SetEjectEffectPN(FALSE);                 //Chuntzu 2004/1/12 add for Eject Pos Effect Area    
    }

		if(m_bWarmStartFlag)
			m_bWarmStartFlag =FALSE;
		else	
	    Check_PCTBL(); /*joyce2012-12-19  PC Table Check; IO�洫�I�Z�ݭ��s���*/
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

	dbgppc[0] = 0;              //wy010902 add pc over 48 points.
	dbgppc[1] = 1;
	dwValue	= GetDBValue("CONF_CODE_WRITE_NULL_DATA13").lValue;
	if( (dwValue == 0x280) || 
     	(dwValue == 0x2800)||                   //<<<Anny2002/6/10 add
     	(dwValue == 0x3800)||                   //<<<Steven2002/7/8 add
     	(dwValue == 0x5800) ) dbgppc[2] = 2;    //<<<Steven2003/4/28 add
	else                      dbgppc[2] = 3;
		dbgppc[3] = 0;
	g_pDatabase->ComputeLimitValue(NULL);	//Yang 2006/12/29 
	
	wSendConfStatus = 2;
}

void  CtmTaskRS232::GetOPState()
{
	static	int nNum = 0;
	long	lDsp28CommState = 0;
	DWORD	dwValue, dwCmd, dwDataID, dwIndex, dwDBValue;
	DWORD  	dwCurrentAuto, dwTotalOther, dwTotalAuto, dwCurrentProduct;	//2008-9-9,fuxy, for �p������
	WORD		wDSPLIBDaysCode=GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD		wDSPLIBYearCode=GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;
	
	dwValue = GetDBValue("STATE_OTHR_OTHR_STEP1_REALTIME").lValue;
	g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_STATE_ID, "STATE_OTHR_OTHR_STEP1_REALTIME", dwValue);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATE_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
	
	if (m_cState != COMM_STATE_RUN)	m_cState = COMM_STATE_RUN;
	
	dwValue	= GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue;	//�ҷſ��
	if (!dwValue)  //Gwei 2005/9/1 ���F�B�z�ޱ�com2���ͪ�error�ӥ[
  	{
  		dwValue = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
   		if(dwValue & 0x0008) //�Yerror�w�s�b�F�~�h�M�����I�i�K�o�h�e�@����
   		{
	    	dwValue &= 0xFFF7;
	    	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue);
   		}
   		if(dwValue & 0x0100) //�Yerror�w�s�b�F�~�h�M�����I�i�K�o�h�e�@����
   		{
	    	dwValue &= 0xFEFF;
	    	SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue);
   		}
  	}
	else	if(g_wDsp2Status == 0)//JOYCE2011-4-1 add �bcom2�q�T�����\��,�ҷŶ}��on/off-->1C8(�`:com2�q�TOK�Z,232�|�ھ�errB�ҷŰ��t�B�z1C8����)
		{
			char	psz[5];
			GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//�P�_�ҷż���
			switch(psz[2]) //com2�ҷ�
			{
				case	'A': //com2 40
				case	'B': //com2 60
				case	'E': //JOYCE2011-2-17 COM1 2�q�ҷ�(CH8~9),40�qCOM2�ҷ�
					dwValue = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
					if(!(dwValue &0x0008))
					{
						dwValue |=0x0008;
						SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue);
		    	}
//					if(!(dwValue &0x0100))
//					{
//						dwValue |=0x0100;
//						SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue);
//		    		}
					break;
				default:
					break;	
			}
		}

	DWORD		dwEcho;
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_STATE_ID, "STATE_OTHR_OTHR_NULL_ECHODATA", &dwEcho);
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

	dwCmd			= GetDBValue("STATE_OTHR_OTHR_NULL_COMMANDDATA").lValue;
	dwDataID	= GetDBValue("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").lValue;
	dwValue		= GetDBValue("STATE_OTHR_OTHR_NULL_UPDATEDATA").lValue;

	if ((wDSPLIBDaysCode < 0x1805) &&(wDSPLIBYearCode==0x2011)|| (wDSPLIBYearCode < 0x2011))//JOYCE 2011-8-22 receive DSP ECHO,Flag clear
		{
			if ((dwOldCmd == DB_OTHER_CMD_ID) && (dwCmd == 0) && (dwDataID == 2) && (wShotCntFlag == 1))
			{
				wShotCntFlag = 0;
				dwOldCmd = 0;
			}
		}
	
	if(m_wLubrCnt ==2 && dwCmd == 0x30)//JOYCE2011-1-8 ����DSP��cmd-0x30���Z,�A�o�e0xFF�Mcmd-0x20
	{
			SetDBValue("MACH_OTHR_OTHR_NULL_WPANELKEYCODE", 0xFFFF, FALSE);
	    m_CmdPanelBreak.wCmd   = 0x20;
	    m_CmdPanelBreak.wIndex = g_pDatabase->Read("MACH_OTHR_OTHR_NULL_WPANELKEYCODE").lID;
	    m_CmdPanelBreak.wValue = 0xFFFF;
	   	m_wLubrCnt = 0;
	   	SendPanel(0xFF);
			//printf("State send \n");
	}
	else if(m_wLubrCnt ==1 && dwCmd == 0x30)
	{
			m_wLubrCnt = 0;
	}
	//printf("dwCmd=%4x, dwIndex=%4x, dwValue=%4x , dwEcho=%4x \n",dwCmd, dwDataID, dwValue, dwEcho);

	if (dwCmd == DB_MACH_CMD_ID)      
    {
    	if (dwDataID < DB_MACH_MAXINDEX_ID)                                                       //BW20030326 for not write to unsafe address
        {
	        dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
	        dwDBValue = GetDBValue(dwIndex).lValue;

	        WORD  	u_wOPStatus 	= _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) + 1; /*JOYCE2012-6-19 �L�o��4��bit-�D�ʧ@���A*/
					WORD 		wAmMeterPara	= GetDBValue("SYS_HMI_NULL_NULL_RESERVED34").lValue;
					dwCurrentAuto	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED19").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED18").lValue);
					dwTotalOther 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED31").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED30").lValue);
					dwTotalAuto 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED33").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED32").lValue);

	        if (dwDBValue != dwValue)  //BW20030307 Add for data save
        	{
            	SetDBValue(dwIndex, dwValue, FALSE);
            }

					if(m_bENERGY)//�p����
            {
            	if(dwDataID == 0x01A8)
            	{
            		if(u_wOPStatus)
            		{
								dwCurrentAuto += wAmMeterPara;
								SetDBValue("SYS_HMI_NULL_NULL_RESERVED18",LOWORD(dwCurrentAuto),FALSE);
								SetDBValue("SYS_HMI_NULL_NULL_RESERVED19",HIWORD(dwCurrentAuto),FALSE);
            	
								dwTotalAuto += wAmMeterPara;
								SetDBValue("SYS_HMI_NULL_NULL_RESERVED32",LOWORD(dwTotalAuto),FALSE);
								SetDBValue("SYS_HMI_NULL_NULL_RESERVED33",HIWORD(dwTotalAuto),FALSE);
            		}
            		else
            		{
            			dwTotalOther += wAmMeterPara;
									SetDBValue("SYS_HMI_NULL_NULL_RESERVED30",LOWORD(dwTotalOther),FALSE);
									SetDBValue("SYS_HMI_NULL_NULL_RESERVED31",HIWORD(dwTotalOther),FALSE);
            		}	
            	}
            }
            
            if((dwDataID == 0x0028)&&(dwValue == 0))
            {
              tmDATE  date;
              long		count=0;
              int			Py=360,Pm=30; //�t��
              GetDate(&date);
              count	= Py*(date.year%2000)+Pm*(date.month-1)+date.day;
              SetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION",count); 
							
							/*JOYCE2012-3-9 �s��ƥ\����,�ռҼ�Ʋ֭p�Ѽ� �B�z*/
							if((!m_bOLDLUB) && (!m_bStepLub))
								{
            		  SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54",0);
									SaveLubricate();
            		}
            }   

        	//<<<<<<<<<//JOYCE2011-8-22 	DSP54�p�}�Ҽ�
        	if(dwDataID == 0x01A4)	
        		{
		        LSBCount = dwValue;
        		}
        	if(dwDataID == 0x01A5)	
        		{
	        	HSBCount = dwValue;

		        dwDBValue = UNWORD(HSBCount,LSBCount);
		        if(GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue != dwDBValue)
		        		ProcessShotCnt();
         		}        		
       		//>>>>>>>>>//JOYCE2011-8-22 	
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

       		if(m_bSVRT)/*JOYCE2014-1-17 */
	        	{
	        		DWORD  dwOffSetTotCnt =0; /*zhongjw 2015-4-13 15:49:37���t�`�p��*/
	        		WORD   wNCnt = 0;
	        		
	        		if(dwDataID == 0x0833)
	        		{	
		        		//printf("dwValue=%d\n",dwValue);
		        		if(dwValue == 1)/*�۰ʽռҤU,�X�Ҥئ۰��k�s,DSP�W��flag��HMI,��HMI�i���k�s�ʧ@*/
		        		{
									SetDBValue("MSET_OTHR_OTHR_NULL_W0834", 0);/*FLAG��ƭȥ��M�s ��DSP*/
									SetDBValue("MSET_MLD_CLOSE_PROTECT_POSITION", 5);/*JOYCE2013-11-14 �b���k�s�ʧ@�ɡ]�]�A�۰ʽռҡ^,�ݭn�����ҧC���ת��ȵ��w���q�{��*/
									
									long long lTemp = 0, lTemp1 = 0, lTemp2 = 0, lTemp3 = 0;
									if(g_pExTable->GetTableValue(0,"D_DIGITAL") || b_28Clmp))/*�}���ҨϥμƦ��*/
										lTemp = GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue;
									else	                                      /*�}���Ҩϥμ�����*/
									{	
										long LengthTemp1 = (WORD)GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
										long LengthTemp2 = (WORD)GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
										long absolut = (long long )(HEX2BCD(LengthTemp1)*10+(LengthTemp2/(0x0001<<4))%0x0010);
										if((GetDBValue("MACH_AD1_WRITE_NULL_W0238").lValue > 0) && (GetDBValue("MACH_AD1_WRITE_NULL_W0238").lValue != 65535))
											lTemp = (GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue * absolut) / (65535 * 1000 / GetDBValue("MACH_AD1_WRITE_NULL_W0238").lValue); /*�Үy�������m=�]�����m*�ت��^/65535*/
										else	
											lTemp = (GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue * absolut) / 65535; /*�Үy�������m=�]�����m*�ت��^/65535*/
										//printf("absolut=%d   %d\n",absolut, GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue);
										//printf("lTemp=%d\n",lTemp);
									}
									SetDBValue("MSET_SCRW_INJ_NULL_W0775", lTemp);  /*�Үy��m ��h�ؼЦ�m: �O�������m*/
									
									lTemp2 = GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue;
									SetDBValue("MACH_AD1_WRITE_CHL2_HOMEPOSITIONOFFSET", lTemp2);  /*�X�Ҥ��k�s�I*/
									SetDBValue("MSET_PRODUCT_PRODUCT_NULL_W070F",lTemp2);/*JOYCE2014-1-22 �Үy���k�s��m�ݭn���Ҩ�ѼƳB�z,�O�s�MŪ���Ҩ���*/
											
									SetClmpShutOffset();/*�p��  �Үy��m��h���t*/
									SetTBarShutDest();  /*�p��  �ԧ�����h�ؼЦ�m*/
							    SaveTBARData(); 
							
						 			/*JOYCE2014-5-4 ��e�Ҩ�p��=E(��h�ؼЪ��Үy�����m) + X(�X�Ҿ���̤p��m) - A(�ϩʼҪO�p��) - C(�ҪO�����m)*/
						 			lTemp1 = lTemp + GetDBValue("MACH_MDH_MDH_NULL_W002D").lValue -GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue - GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED87").lValue;  
						 			if(lTemp1 < 0)
						 				lTemp1 = 0;
						 			SetDBValue("MSET_OTHR_OTHR_NULL_W083C", lTemp1);
									
									/*�D�����ϥγo�Ǽƾ�,�i�u�b�e�����i���s*/	
//									 /*20140504 ��ڼҪO�Z��=�}�Ҧ�m(�Үy�ع�ڭ�) + A(�ϩʼҪO�p��) + B(��e�Ҩ�p��)*/
//									lTemp1 = GetDBValue("METER_AD1_READ_CHL2_RELATIVEPOSITION").lValue + GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue + GetDBValue("MSET_OTHR_OTHR_NULL_W083C").lValue;
//									if(lTemp1 < 0)
//										lTemp1 = 0;
//									SetDBValue("MSET_OTHR_OTHR_NULL_W083D", lTemp1, FALSE);
//						
//									lTemp2 = lTemp1 - GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue;/*��ڼҫp�Z��=�i��ڼҪO�Z���j-�i�ϩʼҪO�p�סj*/
//									if(lTemp2 < 0)
//										lTemp2 = 0;
//									SetDBValue("MSET_OTHR_OTHR_NULL_W083E", lTemp2, FALSE); 
							
									 /*20131104 �}�Ҧ�{�W���� = D�}���Ҥ� + X�X�Ҿ���̤p��m -�]B��e�Ҩ�p�� + A�ϩʼҪO�p�ס^*/
									WORD	ntemp = 0;
									WORD  nPrecisionDate = 0;
									long long lMeterLength = 0, lMeterTravel;
								
									ntemp = (WORD)GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
									nPrecisionDate = (WORD)GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
									lMeterLength = (long long)(HEX2BCD(ntemp)*10 + (nPrecisionDate/(0x0001<<4)) % 0x0010);/*�q�l�ت�*/
								
									lMeterTravel = (long long)((HEX2BCD)(GetDBValue("CONF_AD1_WRITE_CHL2_TRAVEL").lValue)*10);/*�q�l�ؾ����{*/
									
									if(lMeterLength < lMeterTravel) 	/*�q�l�ؾ����{  �q�l�ت����p��*/
										ntemp = lMeterLength;
									else
										ntemp = lMeterTravel;
									lTemp3 = ntemp + GetDBValue("MACH_MDH_MDH_NULL_W002D").lValue - GetDBValue("MSET_OTHR_OTHR_NULL_W083C").lValue - GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue;
									if((lTemp3 < GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue) && (lTemp3 > 0))
										SetDBValue("MSET_MLD_OPEN_STEP5_POSITION", lTemp3);
							    SetDBValue("MSET_OTHR_OTHR_NULL_W0834", 1); /*�k�s�����Z,�A��FLAG��ƭ�2��DSP*/
									//printf("getopstate test1\n");
									
									if(g_ptaskdsp != NULL)
									g_ptaskdsp->WriteValue(COMM_SENDZEROSET);
									//printf("getopstate test2\n");
		        		}
		        		else	if(dwValue == 2)	/*JOYCE2013-10-28 �۰ʤU,�ԧ��L�R,�D���P�_�줣�b�ؼЦ�m,�ҥH���s�p��*/
		        		{
									SetTBarShutDest2();  /*�p��  �ԧ�����h�ؼЦ�m*/
		        		}	
		        	}
		        			        	
		        	else if(dwDataID == 0x084C)
		        	{
		        		if (dwDBValue < dwValue)
		        		{	
			        		wNCnt = dwValue -dwDBValue;
			            dwOffSetTotCnt =  GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED99").lValue;
			        		dwOffSetTotCnt += wNCnt;
									SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED99", dwOffSetTotCnt);
									SaveParallelData();
								}
							}
		        	
		        	else if(dwDataID == 0x084E)
		        	{ 
		        		if (dwDBValue < dwValue)
		        		{	
			        		wNCnt = dwValue -dwDBValue;   
			            dwOffSetTotCnt =  GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED100").lValue;
			        		dwOffSetTotCnt += wNCnt;
									SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED100", dwOffSetTotCnt);
									SaveParallelData();	
								}
		        	}
		        	
		        	else if(dwDataID == 0x0850)
		        	{
		        		if (dwDBValue < dwValue)
		        		{	
			        		wNCnt = dwValue -dwDBValue;
			           	dwOffSetTotCnt =  GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED101").lValue;
		    					dwOffSetTotCnt += wNCnt;
									SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED101", dwOffSetTotCnt);
									SaveParallelData();
		        		}			
		        	}
		        	else if(dwDataID == 0x853 || dwDataID == 0x854 || dwDataID == 0x855 || dwDataID == 0x856)
		        	{
		        		SaveParallelData();	
		        	}
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
	//Chuntzu 2004/7/30 add for �����
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
        	g_flHeaterStatus  = dwValue;
        }   
    	else if (dwDataID == 2)
        {
	       if(wShotCntFlag == 0)
        	{
						if ((wDSPLIBDaysCode < 0x1805) &&(wDSPLIBYearCode==0x2011)|| (wDSPLIBYearCode < 0x2011))//JOYCE 2011-8-22 receive DSP ECHO,Flag clear
							{
	        		wShotCntFlag = 1;
							dwOldCmd = DB_OTHER_CMD_ID;
			    		m_lCounterShtCntEcho = COMM_TIME_SHTCNTECHO;
			  			}

        		if(l_PMTEST == 1)//JOYCE20130327 for PumpTest
		        {
		        	if(bPumpTestStart == TRUE)
		        	{
		        		iPumpTestEnd = 0;
		        		SendMsg(MSG_SH_INET_PUMPTEST_END, iPumpTestEnd, 0,NULL);	//fuxy, 2010-12-22 
		        		//printf("test auto end \n");
		        		bPumpTestStart = FALSE;
		        	}
		        }

	        dwDBValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	        SetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", dwDBValue + dwValue, FALSE);
	        dwDBValue = GetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT").lValue;
        	if(CtmConfig::GetInstance()->GetINETFlag() == 1)
        		{
        			DWORD	dwUNIT=GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
        			SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + dwUNIT, FALSE);
        		}
        	else
        		SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + dwValue, FALSE);
	        
	        //JOYCE2010-10-12 �ֿn�B��Ҽ�
					DWORD dwTotalShotCount	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED36").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED35").lValue);
					dwTotalShotCount += dwValue;
					if(dwTotalShotCount > 999999999)	dwTotalShotCount = 0;
					SetDBValue("SYS_HMI_NULL_NULL_RESERVED35",LOWORD(dwTotalShotCount),FALSE);
					SetDBValue("SYS_HMI_NULL_NULL_RESERVED36",HIWORD(dwTotalShotCount),FALSE);
	        
	        //JOYCE2011-8-23  �ֿn�B��Ҽ� for lubricate
					if((!m_bOLDLUB) && (!m_bStepLub))
					{						
						dwTotalShotCount	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;
						dwTotalShotCount += dwValue;
						if(dwTotalShotCount > 999999999)	dwTotalShotCount = 0;
						SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27",dwTotalShotCount,FALSE);
					}
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
	                if (m_bDC) //James add 2008/8/13 09:42�W��
	                {
										CtmQualityControl::GetInstance()->SetData(12,0);	//fuxy,2009-1-12
										CtmQualityControl::GetInstance()->SetData(13,0);
	                	SendControlMonitorB();
									}
                }
            }
            //	fans add 2009/5/18 10:53�W��        
        	SendMsg(MSG_DSP54_GET_SHOTCOUNT, 0, 0, NULL);
					if(m_bENERGY || m_bAmmeter)//�{�b���~�Ʋ֭p
						{          	 
            int 	NumPerUnit 	= GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
            dwCurrentProduct 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED21").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED20").lValue);
            dwCurrentProduct  +=NumPerUnit;
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED20",LOWORD(dwCurrentProduct),FALSE);
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED21",HIWORD(dwCurrentProduct),FALSE);
						}
            
        	}
      	}
    	else if (dwDataID == 4)
        {
					g_wOperationCount = dwValue;//g_frameTx.cmddata.wValue;  	//<Sunny20070327> �ާ@�p��
		
					if(l_PMTEST == 1)		//JOYCE20130327 for PumpTest
					{
						if((bMannulKey == TRUE) && (bPumpTestStart == TRUE))
						{
							if(GetDBValue("STATE_ALARM_ON_STEP4_ERRORDATA").lValue & 0x0010)
								iPumpTestEnd = 1;
							else
								iPumpTestEnd = 2;
			    	   		//printf("test other end \n");
			    	   		SendMsg(MSG_SH_INET_PUMPTEST_END, iPumpTestEnd, 0, NULL);
							bPumpTestStart = FALSE;
							bMannulKey = FALSE;
						}
					}
        }
      else if (dwDataID == 9)/*JOYCE2012-7-9 HS_STAT_MCS(E70  QMC);DSP LIB 20120706*/
      	{
      		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED10", dwValue);
      	}
      else if(dwDataID == 10)/*�¤G�O��, �e�W�ԧ��i�p�� �ʴ���  m_bSVRT*/
      	{
      		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED81", dwValue, FALSE);
      		printf("TBAR1=%d,%d\n",dwValue,GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED81").lValue);
      	}	
      else if(dwDataID == 11)/*�¤G�O��,�e�U�ԧ��i�p�� �ʴ���  m_bSVRT*/
      	{
      		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED82", dwValue, FALSE);
      		//printf("TBAR2=%d,%d\n",dwValue,GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED82").lValue);
      	}	
      else if(dwDataID == 12)/*�¤G�O��,�Z�W�ԧ��i�p�� �ʴ���  m_bSVRT*/
      	{
      		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED83", dwValue, FALSE);
      		//printf("TBAR3=%d,%d\n",dwValue,GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED83").lValue);
     	}	
      else if(dwDataID == 13)/*�¤G�O��,�Z�U�ԧ��i�p�� �ʴ���  m_bSVRT*/
      	{
      		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED84", dwValue, FALSE);
      		//printf("TBAR4=%d,%d\n",dwValue,GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED84").lValue);
     	}	
      else if (dwDataID == 14)/*JOYCE2014-1-8 ���N�Ұʮɶ�,�˭p��*/
      	{
      		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED6", dwValue);
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

   	if((1 == m_bAustoneCfgFlag) || (1 == m_bN2CFlag))		//�sAustone�q�T
     	{
     		if(lDsp28CommState > 0 /*&& nNum >=1 && m_bTransmitCmdEcho*/)		//���j240ms �~��o�@����Ƶ�dsp28 James add for austone 2010/5/19
			    {
			    	if(PopTransmitCmd(&m_TransmitBuffer))
			    	{
			    		//printf("SendTransmitQueue \n");
			    		SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT);
			    		m_bTransmitCmdEcho = FALSE;
			    	}
			    	nNum = 0;
			    }

		    if(nNum >= 10)			//�W�ɭ��s�o�e
			    {
			    	m_bTransmitCmdEcho = TRUE;
			    }
		 }

	m_CmddData.wEcho = dwCmd;
	SendCommand();
	
	//James add 2008/3/14 04:29�U��
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
            		(dbbuff.cIndexMode == (char)tmMODE_MANUAL))   //Sunny<20070909> amend 0��ܮɶ��۰�
                {
                	dbbuff.bSwitchToAuto = TRUE;
	                dbbuff.wSwitchToAutoShutCount = 0;
                }
                dbbuff.cIndexMode = _ScanBitR(dbbuff.wOperationMode & 0x0FFF);/*JOYCE2012-6-19 �L�o��4��bit-�D�ʧ@���A*/
               	if (dbbuff.cIndexMode == 1) PanelSemiAutoOn();
                else if (dbbuff.cIndexMode == 2) 
                {
                		if(GetDBValue("MSET_OTHR_OTHR_NULL_PHOTOSENSOROPTION").lValue == 1) //�p�G�q���۰ʿ��1
                			PanelTimeAutoOn();
                		PanelSensorAutoOn();
                }		
                else if (dbbuff.cIndexMode == 3) 
                {
                		PanelTimeAutoOn();
                }
                else if (dbbuff.cIndexMode == 4) 	
                {	
                //JOYCE2008-9-24 MARK PanelAutoAdjOn(); 	
                //<<JOYCE2008-9-24 ADD FOR 7GL
                if(m_bUseSlow7GL == FALSE)
                    {
                    PanelAutoAdjOn();//�ʽռ�LED ON
                    }
                else if(m_bUseSlow7GL == TRUE)   
                    {
                    m_bAdjustUse7GL = FALSE;
                    PanelAdjSlow7GL();  //�ռҺC�tLED ON
                    }
                //>>JOYCE2008-9-24 ADD
                }	
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
			if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED76").lValue)/*wangli2015-4-14 14:03:39 �۰ʺʴ��^��ʪ��A����*/
				SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 0);
        
            //<<JOYCE2008-9-24 ADD FOR 7GL
    				if(m_bAutoAdjust7GL)//�۰ʽռ�FLAG
    					{
    					m_bAutoAdjust7GL = FALSE;
    					}
            //>>JOYCE2008-9-24 ADD
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
            }                                                           //>>> wy081299
       }
    }
	
	if(m_bENERGY)//�p������
	{
		dwCurrentAuto 			= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED19").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED18").lValue);
		dwCurrentProduct 		= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED21").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED20").lValue);
		WORD wWeightPerUnit 	= GetDBValue("SYS_HMI_NULL_NULL_RESERVED24").lValue;
		DWORD  dwEnergyPerUnit;
		if(dwCurrentProduct * wWeightPerUnit)
			dwEnergyPerUnit = (dwCurrentAuto*1000*10)/(dwCurrentProduct * wWeightPerUnit);//0.00��/����
		else
			dwEnergyPerUnit = 0;
		SetDBValue("SYS_HMI_NULL_NULL_RESERVED22",LOWORD(dwEnergyPerUnit),FALSE);
		SetDBValue("SYS_HMI_NULL_NULL_RESERVED23",HIWORD(dwEnergyPerUnit),FALSE);
		
		dwTotalOther 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED31").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED30").lValue);
		dwTotalAuto 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED33").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED32").lValue);
		if(dwCurrentAuto	>= 99999999)
		{
			dwCurrentAuto 	=0;		//2008-9-9, fuxy, �M�s
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED18", dwCurrentAuto, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED19", dwCurrentAuto, FALSE);
		}
		if(dwTotalOther 	>= 99999999)
		{
			dwTotalOther 	=0;
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED30", dwTotalOther, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED31", dwTotalOther, FALSE);
		}
		if(dwTotalAuto 		>= 99999999)
		{
			dwTotalAuto 	=0;
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED32", dwTotalAuto, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED33", dwTotalAuto, FALSE);
		}
		if(dwCurrentProduct	>= 99999999)//JOYCE2010-7-27
		{
			dwCurrentProduct 	=0;
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED20", dwCurrentProduct, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED21", dwCurrentProduct, FALSE);
		}
	}
}

void  CtmTaskRS232::GetOPState2()
{	
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATE2_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);

	if(g_pExTable->GetTableValue(0,"D_AUSTONE")) //JOYCE2011-2-11 �ϥ�Opstate2 �˼ƲĤG���ƾ�
		{
		if(GetDBValue("STATE_OTHR2_OTHR_NULL_W19CF").lValue == 1)   //(m_dbState2.wOperationReserved[0] == 1)
			m_bAustoneState = TRUE;//m_bAustoneState ���Austone�O�_�M54�q�T�W�C
		else
			m_bAustoneState = FALSE;
		}
}

void	CtmTaskRS232::GetOPSummary()                 //St020101 for add update speed
{
	int         i;
	int         j = 0;
	WORD		wValue, wDataID, wDataID2, wIndex;
	
	wValue = GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue;
	while ((g_wPassOPSum[j] != wValue) && (g_wPassOPSum[j] != NULL_ID)) j++;
	if (g_wPassOPSum[j] == NULL_ID) g_bOPSumPass = TRUE; 

	SetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE", g_frameRx.commdata.OPSummary.wOperationMode, FALSE);
	SetDBValue("MONI_OTHR_OTHR_NULL_CYCLEREALTIME", g_frameRx.commdata.OPSummary.tmlCycle, FALSE);
	
	if(m_bAmmeter)		//fuxy, 2012-5-17, �q����,  �O���۰ʶ}�l�ɪ���� 
	{
		//printf("GetOPSummary:%d, %d  \n", g_frameRx.commdata.OPSummary.wOperationMode, wOperationModeOld);
		if(wOperationModeOld != g_frameRx.commdata.OPSummary.wOperationMode) 
		{
			if((g_frameRx.commdata.OPSummary.wOperationMode == 3)||(g_frameRx.commdata.OPSummary.wOperationMode == 5)||(g_frameRx.commdata.OPSummary.wOperationMode == 9))
			{
				ProcessAmmeterEnergy();
			}
			wOperationModeOld = g_frameRx.commdata.OPSummary.wOperationMode;
		}
	}

	//	fans add 2009/5/15 01:21�U��
	SendMsg(MSG_DSP54_GET_OPMODE, g_frameRx.commdata.OPSummary.wOperationMode, 0, NULL);
	
	wDataID = g_pDatabase->GetDataID("STATE_OTHR_OTHR_NULL_OPERSUBSTEP1");
	for ( i = 0 ; i < tmMAX_OP_SUBSTEP ; i++ )
		{
		wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
		SetDBValue(wIndex, g_frameRx.commdata.OPSummary.wOperationSubStep[i], FALSE);
		}
	wDataID = g_pDatabase->GetDataID("STATE_OTHR_OTHR_NULL_OPERSTEP1");
	for ( i = 0 ; i < 5              ; i++ )
		{
		wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
		SetDBValue(wIndex, g_frameRx.commdata.OPSummary.wOperationStep[i], FALSE);
		}
	wDataID = g_pDatabase->GetDataID("STATUS_PVL1_WRITE_NULL_REALDATA");
	wDataID2 = g_pDatabase->GetDataID("METER_AD1_READ_CHL1_RELATIVEPOSITION");
	for ( i = 0 ; i < 4 ; i++ )
    {
    	wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
    	SetDBValue(wIndex, g_frameRx.commdata.OPSummary.daOutput[i], FALSE);
    	wIndex = g_pDatabase->DataIDToIndex(wDataID2+i);
    	SetDBValue(wIndex, g_frameRx.commdata.OPSummary.adPosi_Relative[i], FALSE);
    }

  SetDBValue("STATE_OTHR_OTHR_STEP1_REALTIME", g_frameRx.commdata.OPSummary.tmlActing[0], FALSE);
	SetDBValue("STATE_OTHR_OTHR_STEP2_REALTIME", g_frameRx.commdata.OPSummary.tmlActing[1], FALSE);
	Echo(COMM_ECHO_SUCCESS);

	if (wValue !=ID_VIEW_PANEL)     //BW0211'03 add for LED test
    {
    	wValue = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;
    	if (dbbuff.wOperationMode != GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue)
        {
        	dbbuff.wOperationMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;
        	if(dbbuff.wOperationMode)
            {
            	//if( _TestBit(dbbuff.wOperationMode, OPS_AUTO-OPS_STATE_BEGIN) && (dbbuff.cIndexMode == (char)tmMODE_MANUAL))
                if( _TestBit(dbbuff.wOperationMode, 0) && 
                	(dbbuff.cIndexMode == (char)tmMODE_MANUAL)) //Sunny<20070909> 0��ܮɶ��۰�
                {
                	dbbuff.bSwitchToAuto = TRUE;
                	//g_tmDebugInfo->PrintDebugInfo("1529 =%d\n", dbbuff.bSwitchToAuto);
                	dbbuff.wSwitchToAutoShutCount = 0;
                }
            	dbbuff.cIndexMode = _ScanBitR(dbbuff.wOperationMode & 0x0FFF);/*JOYCE2012-6-19 �L�o��4��bit-�D�ʧ@���A*/
            	
                if (dbbuff.cIndexMode == 1) PanelSemiAutoOn();
                else if (dbbuff.cIndexMode == 2) 
                {
                		if(GetDBValue("MSET_OTHR_OTHR_NULL_PHOTOSENSOROPTION").lValue == 1) //�p�G�q���۰ʿ��1
                			PanelTimeAutoOn();
                		PanelSensorAutoOn();
                }	
                else if (dbbuff.cIndexMode == 3) 
                {
                		PanelTimeAutoOn();
                }
                else if (dbbuff.cIndexMode == 4)
                {	
                //JOYCE2008-9-24 MARK PanelAutoAdjOn(); 	
                //<<JOYCE2008-9-24 ADD FOR 7GL
                if(m_bUseSlow7GL == FALSE)
                    {
                    PanelAutoAdjOn();//�ʽռ�LED ON
                    }
                else if(m_bUseSlow7GL == TRUE)   
                    {
                    m_bAdjustUse7GL = FALSE;
                    PanelAdjSlow7GL();  //�ռҺC�tLED ON
                    }
                //>>JOYCE2008-9-24 ADD
                }	
                else if (dbbuff.cIndexMode == 11)
                {
                	g_bLEDAdjMold = FALSE;
					g_lCounterLEDAdjMold = 0;
                }
            }
        	else
            {
            	dbbuff.cIndexMode = (char)tmMODE_MANUAL;
            	dbbuff.bSwitchToAuto = FALSE;
	            PanelManualOn();
 				if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED76").lValue)/*wangli2015-4-14 14:03:39 �۰ʺʴ��^��ʪ��A����*/
					SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 0);
            
            //<<JOYCE2008-9-24 ADD FOR 7GL
    				if(m_bAutoAdjust7GL) //�۰ʽռ�FLAG
    					{
    					m_bAutoAdjust7GL = FALSE;
    					}
            //>>JOYCE2008-9-24 ADD
            }
        }
    }
	g_bOPSumPass = FALSE;   //st031701 
}


void  CtmTaskRS232::GetCurveData()                 //Chuntzu 2005/1/20 add for new curve
{
	{
	tmDB_CHARTSTROKE1     chartstroke1;
	chartstroke1.nY1 = g_frameRx.commdata.curvedata.nY1;
	chartstroke1.nY2 = g_frameRx.commdata.curvedata.nY2;
	if (g_frameRx.commdata.curvedata.cnPoint == 0)
	{
	   switch (g_frameRx.commdata.curvedata.flCurveKind)
	     {
	        case 0:      //INJ
	            //printf("Inject curve end\n");
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALPRESS);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALSPEED);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_DXPOS);
	        	ClearChartInjectCount();
	        	//printf("Inject over!!!\n");
	        	SendMsg(MSG_SH_INET_INJECT_DATA, 0, 0, NULL);
	        	break;
	        case 1:      //HOLD
	        	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALPRESS);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALSPEED);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_DXPOS); 
	        	ClearChartHoldCount();
//	        	printf("hold over!!!\n");
	        	break;
	        case 2:      //INJ2
	        	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECTB_REALPRESS);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECTB_REALSPEED);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECTB_DXPOS);
	        	ClearChartInjectBCount();
	        	SendMsg(MSG_SH_INET_INJECTB_DATA, 0, 0, NULL);
	        	break;
	        case 3:      //HOLD2
	        	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLDB_REALPRESS);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLDB_REALSPEED);
	         	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLDB_DXPOS); 
	        	ClearChartHoldBCount();
	        	break;
	        default:
	        	break;
	     }
	}
	
	//	dY1 ; pressure,  dY2 :Speed
	double dX = 0, dY1 = 0, dY2 = 0,dX2=0; 
	dX 	= g_frameRx.commdata.curvedata.nX1;
	dY1 = g_frameRx.commdata.curvedata.nY1;
	dY2 = g_frameRx.commdata.curvedata.nY2;
	dX2	= g_frameRx.commdata.curvedata.nX2; 		// ���u�ɶ�	
	switch (g_frameRx.commdata.curvedata.flCurveKind)
	{
	    case 0:      //INJ
	        //printf("Add point inject!!\n");
	      chartstroke1.nX = g_frameRx.commdata.curvedata.nX2;
	    	AddChartInject(&chartstroke1);
	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALPRESS, &dX, &dY1, 1);
	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALSPEED, &dX, &dY2, 1);
	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_DXPOS, &dX2, &dX, 1);   //x=time,y=pos
	        break;
	    case 1:      //HOLD
	    	if (g_frameRx.commdata.curvedata.cnPoint == 0)
	    		{
	    			double d0X = 0, d0Y1 = 0, d0Y2 = 0,d0X2=0; 
	    			short	 d0index=0;
	    			tmDB_CHARTSTROKE1     chartstroke0;
    				if(m_dbchartInject.nIndex)	d0index = m_dbchartInject.nIndex-1;
    				chartstroke0.nX  = m_dbchartInject.nX [d0index];
    				chartstroke0.nY1 = m_dbchartInject.nY1[d0index];
    				chartstroke0.nY2 = m_dbchartInject.nY2[d0index];	  
    				AddChartHold(&chartstroke0);  			
	    			
	    			d0X 	= 0;
						d0Y1 = m_dbchartInject.nY1[d0index];
						d0Y2 = m_dbchartInject.nY2[d0index];
						d0X2	= m_dbchartInject.nX [d0index]; 		// ���u�ɶ�	
						
						if(m_dbchartInject.nX [d0index] >= (WORD)m_dbchartHold.nX0) //Andy 20090710                                                                                                                                                                                             //						if(m_dbchartInject.nX [d0index] >= (WORD)m_dbchartHold.nX0) //Andy 20090710
	    				{
	    					double HoldPOS=0;        //save pos              //czz    2009/10/27 07:11�U��                                                                                                                                                                                                                                   ////	    	printf("Hold dsp index = %d nX2 = %d, nX0 = %d\n", m_dbchartHold.nIndex,
	    					HoldPOS=dX;           
	    					d0X = m_dbchartInject.nX [d0index] - m_dbchartHold.nX0; 
	    					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_DXPOS, &d0X, &HoldPOS, 1);
	    				} 
	    			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, &d0X, &d0Y1, 1);
	    			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALSPEED, &d0X, &d0Y2, 1);
	    		}
	    	chartstroke1.nX = g_frameRx.commdata.curvedata.nX2;
	    	AddChartHold(&chartstroke1);
	    	if(g_frameRx.commdata.curvedata.nX2 >= (WORD)m_dbchartHold.nX0) //Andy 20090710                                                                                                                                                                                                     //	    		dX = g_frameRx.commdata.curvedata.nX2 - m_dbchartHold.nX0;
	    		{                                                                                                                                                                                                                                                                               ////	    	printf("Hold dsp point Num=%d\n", g_frameRx.commdata.curvedata.cnPoint);
	    			double HoldPOS=0;        //save pos              //czz    2009/10/27 07:11�U��                                                                                                                                                                                                                                   ////	    	printf("Hold dsp index = %d nX2 = %d, nX0 = %d\n", m_dbchartHold.nIndex,
	    			HoldPOS=dX;                                                                                                                                                                                                                                                                 ////    	    g_frameRx.commdata.curvedata.nX2, m_dbchartHold.nX0);
	    			dX = g_frameRx.commdata.curvedata.nX2 - m_dbchartHold.nX0;                                                                                                                                                                                                                  //	    	//printf("Hold dX = %f,dY1 = %f,dY2 = %f\n", dX, dY1, dY2);
	    			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_DXPOS, &dX, &HoldPOS, 1);                                                                                                                                                                                   //	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, &dX, &dY1, 1);
	    			//printf("hold time =%lf,speed=%lf,press=%lf,pos=%lf\n",dX,dY2,dY1,HoldPOS);
	    		}                                                                                                                                                                                                                                                                               //	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALSPEED, &dX, &dY2, 1);
	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, &dX, &dY1, 1);
	    	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALSPEED, &dX, &dY2, 1);
	        break;
	    case 2:      //INJ2
	        chartstroke1.nX  = g_frameRx.commdata.curvedata.nX2;
	        AddChartInjectB(&chartstroke1);
	        CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECTB_REALPRESS, &dX, &dY1, 1);	//add,2008-12-23
	    		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECTB_REALSPEED, &dX, &dY2, 1);
	    		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECTB_DXPOS, &dX2, &dX, 1);   //x=time,y=pos
	        break;
	    case 3:      //HOLD2
	    	//m_nDspCurveHoldBState = 1;
	    	if (g_frameRx.commdata.curvedata.cnPoint == 0)
	    		{
	    			double d0X = 0, d0Y1 = 0, d0Y2 = 0,d0X2=0; 
	    			short	 d0index=0;
	    			tmDB_CHARTSTROKE1     chartstroke0;
    				if(m_dbchartInjectB.nIndex)	d0index = m_dbchartInjectB.nIndex-1;
    				chartstroke0.nX  = m_dbchartInjectB.nX [d0index];
    				chartstroke0.nY1 = m_dbchartInjectB.nY1[d0index];
    				chartstroke0.nY2 = m_dbchartInjectB.nY2[d0index];	  
    				AddChartHoldB(&chartstroke0);  			
	    			
	    			d0X 	= 0;
						d0Y1 = m_dbchartInjectB.nY1[d0index];
						d0Y2 = m_dbchartInjectB.nY2[d0index];
						d0X2 = m_dbchartInjectB.nX [d0index]; 		// ���u�ɶ�	
						
						if(m_dbchartInjectB.nX [d0index] >= (WORD)m_dbchartHoldB.nX0) //Andy 20090710                                                                                                                                                                                             //						if(m_dbchartInject.nX [d0index] >= (WORD)m_dbchartHold.nX0) //Andy 20090710
	    				{
	    					double HoldPOS=0;        //save pos              //czz    2009/10/27 07:11�U��                                                                                                                                                                                                                                   ////	    	printf("Hold dsp index = %d nX2 = %d, nX0 = %d\n", m_dbchartHold.nIndex,
	    					HoldPOS=dX;           
	    					d0X = m_dbchartInjectB.nX [d0index] - m_dbchartHoldB.nX0; 
	    					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_DXPOS, &d0X, &HoldPOS, 1);
	    				} 
	    			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALPRESS, &d0X, &d0Y1, 1);
	    			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALSPEED, &d0X, &d0Y2, 1);
	    		}
	        chartstroke1.nX  = (short)g_frameRx.commdata.curvedata.nX2;
	        AddChartHoldB(&chartstroke1);
	        if(g_frameRx.commdata.curvedata.nX2 >= (WORD)m_dbchartHoldB.nX0)
	        	{
	        		double HoldPOS=0;        //save pos              //czz    2009/10/27 07:11�U��                                                                                                                                                                                                                                   ////	    	printf("Hold dsp index = %d nX2 = %d, nX0 = %d\n", m_dbchartHold.nIndex,
	    				HoldPOS=dX;   
	        		dX = g_frameRx.commdata.curvedata.nX2 - m_dbchartHoldB.nX0;	//add ,2009-2-19
	        		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_DXPOS, &dX, &HoldPOS, 1);    
	        	}	
	        
	        CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALPRESS, &dX, &dY1, 1);		//add,2008-12-23
	    		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALSPEED, &dX, &dY2, 1);
	        break;
	    default:
	        break;
		}
	}

	Echo(COMM_ECHO_SUCCESS);
}



void  CtmTaskRS232::GetOPStatus()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_STATUS_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetOPMeter()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_METER_ID, FALSE);

	//Sunny<20070816><<<<<	
	double dX = 0, dY1 = 0, dY2 = 0;
	long lOPERSTEP1 =GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;
	long lOPERSTEP4 =GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP4").lValue;
	tmDB_CHARTSTROKE1     chartstroke1;
	if (g_WarmStartFlag||(lOPERSTEP1 &0x0008)||(lOPERSTEP4 &0x0400))
	    {
	    	if(/*dbstat.wOperationStep[0]*/ lOPERSTEP1 &0x0008)
	    		{
    	 			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALBACKPRES);
    	 			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALRPM);
	            
        		ClearChartChrgCount();//ClearChartChargeCount();
        	}	
        if(/*dbstat.wOperationStep[0]*/ lOPERSTEP4 &0x0400)
	    		{
    	 			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGEB_REALBACKPRES);
    	 			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGEB_REALRPM);
	            
        		ClearChartChrgBCount();//ClearChartChargeCount();
        	}	
        if (g_WarmStartFlag) 
        	{
        		CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALBACKPRES);
    	 			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALRPM);	            
        		ClearChartChrgCount();
        		
        		CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGEB_REALBACKPRES);
    	 			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGEB_REALRPM);	            
        		ClearChartChrgBCount();
        		
        		g_WarmStartFlag = FALSE;	            
        	}	
	    }
	
	if (/*dbstat.wOperationStep[0]*/lOPERSTEP1&0x0020)
	    {
	        chartstroke1.nX  = GetDBValue("METER_AD1_READ_CHL1_RELATIVEPOSITION").lValue;//dbmetr.adPosi_Relative[0];
	        if(GetDBValue("SYSX_SCREW_RECOVERY_NULL_CURVEOPTION").lValue)
	        chartstroke1.nY1 = GetDBValue("METER_AD1_READ_CHL5_RELATIVEPOSITION").lValue;//dbmetr.adPosi_Relative[4];
	        else
	        chartstroke1.nY1 = 0;
					chartstroke1.nY2 = GetDBValue("MONI_SCRW_RCV_NULL_REALRPM").lValue;//dbmcur.cnChargeRPM;
	        AddChartCharge(&chartstroke1);
	        //printf("chartstroke1.nX:%ld\n",chartstroke1.nX);
	    	dX =chartstroke1.nX;
			dY1=chartstroke1.nY1;
			dY2=chartstroke1.nY2;
			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALBACKPRES, &dX, &dY1, 1);
			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALRPM, &dX, &dY2, 1);	
	    
	    if((dbbuff.cIndexMode == (char)tmMODE_MANUAL)||(dbbuff.cIndexMode == 4))//JOYCE2008-9-24 ADD FOR 7GL,���/�ʽռ�/�ռҺC�t��  �x��LED ON
	    		PanelICharge();                                                     //JOYCE2008-9-24 ADD FOR 7GL
	    }
		//<<JOYCE2008-9-24 ADD FOR 7GL  �x��LED ON
	else
		  {
	    	if(((dbbuff.cIndexMode == (char)tmMODE_MANUAL)||(dbbuff.cIndexMode == 4))
	    		 &&(lOPERSTEP1&0x0010)&&(!m_bKeySuck7GL))
	    			PanelICharge();                              //���F�x��key,�x�ư����O�g�h�ʧ@��,�x��LED���M�nON
	    	else			 
	    			PanelIChargeOFF7GL();
		  }
		  
	if (/*dbstat.wOperationStep[0]*/lOPERSTEP4&0x2000)
	    {
	        chartstroke1.nX  = GetDBValue("METER_AD1_READ_CHL8_RELATIVEPOSITION").lValue;//dbmetr.adPosi_Relative[0];
	        if(GetDBValue("SYSX_SCREW_RECOVERY_NULL_CURVEOPTION").lValue)
	        chartstroke1.nY1 = GetDBValue("METER_AD1_READ_CHL6_RELATIVEPOSITION").lValue;//dbmetr.adPosi_Relative[4];
	        else
	        chartstroke1.nY1 = 0;
					chartstroke1.nY2 = GetDBValue("MONIB_SCRW2_RCV_NULL_REALCOUNTER").lValue;//dbmcur.cnChargeRPM;
	        AddChartChargeB(&chartstroke1);
	        //printf("chartstroke1.nX:%ld\n",chartstroke1.nX);
	    		dX =chartstroke1.nX;
			dY1=chartstroke1.nY1;
			dY2=chartstroke1.nY2;
			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGEB_REALBACKPRES, &dX, &dY1, 1);
			CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGEB_REALRPM, &dX, &dY2, 1);	
	    }		  
		//>>JOYCE2008-9-24 ADD FOR 7GL  �x��LED ON
	//>>>>>>
//	if((CtmConfig::GetInstance()->GetDSP28Flag() == 2)&&(CtmConfig::GetInstance()->GetMachineType() == 0x1001))	
//		{
//			long long	lDSP28posi,lDSP54posi,lDelta;
//			lWaitCount++;
//			lDSP28posi = GetDBValue("MACH_OTHR_OTHR_NULL_W0377").lValue;
//			lDSP54posi = GetDBValue("METER_AD1_READ_CHL2_RELATIVEPOSITION").lValue;
//			lDelta		 = (lDSP28posi - lDSP54posi*10)/10;
//			//printf("d2:%lld,d5:%lld,de:%lld\n",lDSP28posi,lDSP54posi,lDelta);
//			if(((lDelta <= -10)||(lDelta >= 10))&&(lWaitCount >= 5))
//				{
//					lWaitCount = 0;
//					WORD		offset;
//					offset  = (WORD)lDelta;
//					//printf("offset:%d,Delta:%ld\n",offset,lDelta);
//					SetDBValue("MACH_OTHR_OTHR_NULL_SPECIALDATA17",offset);
//	    	}	
//		}

	if(g_pExTable->GetTableValue(0,"D_INJMTR780")) //JOYCE2011-4-11 No.11-017
		{
		long long lInjMtrMax = GetDBValue("CONF_CODE_READ_CHL1_MAXPOSITION").lValue;
		if(lInjMtrMax > 7800)
			{
			lInjMtrMax = 7800;
			SetDBValue("CONF_CODE_READ_CHL1_MAXPOSITION",lInjMtrMax);
			}
		}
			
	Echo(COMM_ECHO_SUCCESS);
	//return chartstroke1;
}

void  CtmTaskRS232::GetOPMeterZeroAgain()
{
	WORD	wValue = GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue;
	
	Echo(COMM_ECHO_SUCCESS);
	SetDBValue("MACH_AD1_WRITE_CHL2_HOMEPOSITIONOFFSET", wValue, FALSE);
	SendZeroSet();
}

void  CtmTaskRS232::GetOPTemperature()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_TEMP_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
	//=======
	// �o�e�����A���ūצ���//netdigger 2005/12/3
	//=======
	if(g_lCounterTemp <= 0)
	{
		MSG	msg;
		
		msg.message	= MSG_TEMP;
		g_pApplication->QueueMessage(&msg);
		g_lCounterTemp	= COMM_TIME_TEMPER;
	}
}

void  CtmTaskRS232::GetControlIndex()
{
	WORD		wDataID = g_frameRx.commdata.controlindex.wIndex, wIndex;

	wIndex = g_pDatabase->DataIDToIndex(wDataID);
	
	if ((GetOilTable()) && CheckOilVatID(g_pDatabase->GetString(wIndex)))
	    SetDBValue(wIndex, OilToPosi(g_frameRx.commdata.controlindex.wValue), FALSE);
	else    
	    SetDBValue(wIndex, g_frameRx.commdata.controlindex.wValue, FALSE);
	
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetProduction()
{

	Echo(COMM_ECHO_SUCCESS);        //Steven 2003/2/10 move to here
	if (dbbuff.bSwitchToAuto && (GetDBValue("STATE_ALARM_ONOFF_NULL_STATUS").lValue == 0) 
		&& (GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue != 0))
    {
    if (++dbbuff.wSwitchToAutoShutCount >= GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue)
        {
        dbbuff.bSwitchToAuto = FALSE;
        SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 1, FALSE);
       	CtmQualityControl::GetInstance()->SetData(MONI_DATATYPE_LIMITUP_ID,0);
				CtmQualityControl::GetInstance()->SetData(MONI_DATATYPE_LIMITLOWER_ID,0);
					
        SendControlMonitor();
			if (m_bDC) //James add 2008/8/13 09:42�W��
			{
       	CtmQualityControl::GetInstance()->SetData(12,0);		//fuxy,2009-1-12,add
				CtmQualityControl::GetInstance()->SetData(13,0);
				SendControlMonitorB();
			}
        }
    }
}

void  CtmTaskRS232::GetDAOutput()     // St1108'01 for D/A
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_DAOUT_ID);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_DAOUT2_ID);
	Echo(COMM_ECHO_SUCCESS);
	m_bGotDA = TRUE;
	m_bNewDA = FALSE;  //Andy 20070304
	if (m_cState != COMM_STATE_RUN) 
	    SendControlMachine();
	else
	    SendPowerMatch();
}

void CtmTaskRS232::GetDAOutput2()     // Andy 20061212 for New D/A
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_NEWDAOUT_ID);
	//Andy 20070314
	Echo(COMM_ECHO_SUCCESS);
	m_bGotDA = TRUE;
	m_bNewDA = TRUE;  //Andy 20061212 for New D/A flag!
	if (m_cState != COMM_STATE_RUN) 
	    SendControlMachine();
	else
	    SendPowerMatch();
}

void  CtmTaskRS232::GetMoniCurrent()
{
	DWORD		dwValue;
	
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONICURRENT_ID, FALSE);
	if (GetOilTable())
		{
	    g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_MONICURRENT_ID, "MONI_MLD_OPEN_NULL_REALPOSITION", &dwValue);
	    SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", OilToPosi(dwValue), FALSE);
	    }
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetMoniLast()
{
	DWORD		dwValue = 0, dwPosiValue = 0;
	
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONILAST_ID, FALSE);
	if (GetOilTable())
		{
	    g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer, BLOCK_MONILAST_ID, "MONI_MLD_OPEN_NULL_LASTPOSITION", &dwValue);
	    dwPosiValue = OilToPosi(dwValue);
	    g_pDatabase->Write("MONI_MLD_OPEN_NULL_LASTPOSITION", &dwPosiValue, FALSE );//wangli2014-12-2 14:52:54 �W�@�N�Ȥ��sSRAM,�����_�q��0
	    //SetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION", OilToPosi(dwValue), FALSE);
	    }
	Echo(COMM_ECHO_SUCCESS);
	
	//#ifndef   D_DC
	if (!m_bDC) //James add 2008/8/13 09:42�W��
	{
		dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
		if (dwValue == g_MoniUpdate.dwShotCount) //St1128'01  for moni lose problem
		{
		    g_MoniUpdate.bChange = TRUE;
		}    
		else
		{
	    if(GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue != 0)
		  {
		    if ((dwValue % GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue) == 0) 
		    	g_ptmControlServer->MoniControl()->AddMoni2();  
		  }
		    g_MoniUpdate.dwShotCount = dwValue;
		    g_MoniUpdate.bChange = FALSE;
		}
	}   
	//#endif
}

void  CtmTaskRS232::GetMoniCurrentB()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONIBCURRENT_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetMoniLastB()
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
	    if(GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue != 0)
	    {	
	    if ((dwValue % GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue) == 0) 
	    	 g_ptmControlServer->MoniControl()->AddMoni2();
	    	//AddMoni2();   
	    }
	    g_MoniUpdate.dwShotCount = dwValue;
	    g_MoniUpdate.bChange = FALSE;
	    }    
}

void  CtmTaskRS232::GetMoniMaxMinB()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer, BLOCK_MONIMAXMINB_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetDiagRM()
{
	m_nRevRM++;
	memcpy((char *)&m_dbdiagrm, g_frameRx.commdata.acBuffer, sizeof(tmDB_DIAGRM));
	
	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	msg.message	= MSG_DSP54_REVRM;	
	msg.lParam	= m_nRevRM;
	g_pApplication->QueueMessage(&msg);

	SendDiagSM();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetDiagRI()
{
	memcpy((char *)&m_dbdiagri, g_frameRx.commdata.acBuffer, sizeof(tmDB_DIAGRI));
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetAustonDriveInfo()
{
	//printf("GetAustonDriveInfo \n");
	Echo(COMM_ECHO_SUCCESS);	
	WORD wLength = 0;
	
	if(1 == m_bN2CFlag)		//�sAustone�q�T
	{
		if(g_ptaskdsp28_N2C != NULL)
		{
			wLength = *(WORD*)((BYTE*)g_frameRx.commdata.acBuffer+2);
			g_ptaskdsp28_N2C->WriteValue(COMM_GETAUSTIONDRIVEINFO, g_frameRx.commdata.acBuffer, wLength);	
		}
	}
	
	if(1 == m_bAustoneCfgFlag)		//�sAustone�q�T
	{
		if(g_ptaskdsp28_Austone != NULL)
		{
			wLength = *(WORD*)((BYTE*)g_frameRx.commdata.acBuffer+2);
			g_ptaskdsp28_Austone->WriteValue(COMM_GETAUSTIONDRIVEINFO, g_frameRx.commdata.acBuffer, wLength);	
		}
	}
	else													//��Austone�q�T
	{
		memcpy((char *)&m_dbAustonDriveInfo, g_frameRx.commdata.acBuffer, sizeof(tmDB_AUSTONDRIVEINFO));
	
		//Save motor and drive temper
		int	nTempValue = 0;
		nTempValue = (int)m_dbAustonDriveInfo.wOptState;
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED1", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAlarmNub;
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED2", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fMotorSpeed*10);
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED3", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fMotorTorqueForce*10);
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED4", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fDriverFlow*10);
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED5", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fDriveBusVoltage*10);
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED6", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fDriveTemper*10);
		//g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED7", &nTempValue, FALSE);
		g_pDatabase->Write("SYS_HMI_NULL_NULL_RESERVED30", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fMotorTemper*10);
		//g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED8", &nTempValue, FALSE);
		g_pDatabase->Write("SYS_HMI_NULL_NULL_RESERVED31", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[0];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED9", 	&nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[1];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED10", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[2];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED11", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[3];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED12", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[4];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED13", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[5];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED14", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[6];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED15", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[7];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED16", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[8];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED17", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[9];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED18", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[10];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED19", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[11];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED20", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[12];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED21", &nTempValue, FALSE);
		nTempValue = (int)m_dbAustonDriveInfo.wAustonReserved[13];
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED22", &nTempValue, FALSE);
	}
}

void  CtmTaskRS232::GetSProfile01I()
{
	ClearChartInjectCount();
	ClearChartHoldCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetDProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetEProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetSProfile01H()
{
	ClearChartHoldCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetDProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetEProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetSBProfile01I()
{
	ClearChartInjectBCount();
	ClearChartHoldBCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetDBProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetEBProfile01I()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetSBProfile01H()
{
	ClearChartHoldBCount();
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetDBProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void  CtmTaskRS232::GetEBProfile01H()
{
	Echo(COMM_ECHO_SUCCESS);
}

void	CtmTaskRS232::GetEcho()
{
if(g_frameRx.wCmd == COMM_ECHO_FAILURE)
{
	//printf("g_frameTx.wCmd: %4X\n",g_frameTx.wCmd);
	//lWaitCount++;
	//if(lWaitCount < 5)
	switch (g_frameTx.wCmd)
	    {
	    case 0x3330:
	    		SendBlock61();
	    		break;
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
	//printf("g_frameRx.wCmd: %4X\n",g_frameRx.wCmd);
	switch (g_frameRx.wCmd)
	    {
	    case 0x3330:
	    		SendBlock61();
	    		break;
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
	//lWaitCount = 0;
	if (m_bDC) //James add 2008/8/13 09:42�W��
	{
		switch (g_frameRx.wCmd)
	    {
	    	case COMM_SENDCONTROLMACHINE:
	    			if (m_cState != COMM_STATE_RUN)	
		        SendControlMoldset();
		        break;
	//#ifdef   D_DC         //Chuntzu 2004/7/30
	    	case COMM_SENDCONTROLMOLDSET:
		        if (m_cState == COMM_STATE_RUN)	
		        	{
		        		//printf("dc lWaitCount =%d \n", lWaitCount);
		        		if(lWaitCount) //2010-10-22 ADD judgement ����lWaitCount���t�ȬO���o�eMoldSet
		        			lWaitCount--;
		        		wSendMoldStatus = 2;
		        		if(lWaitCount == 1) 
		        			{
		        				wSendMoldStatus = 1;
		        				SendControlMoldset();
		        			}
		        	}	
		        if (m_bGotDA) SendPowerMatch();    //St1114'01
		        else          
	            {
		           	if (m_cState != COMM_STATE_RUN)	
		           		SendControlMachineB();
	            }
	        	break;
	    	case COMM_SENDPOWERMATCH:              //St1114'01
	    	case COMM_SENDPOWERMATCH2:     //Andy 20061212 for New D/A
	        	if (m_cState != COMM_STATE_RUN)	//	fans add 2007/9/29 10:48�W��
	        	{
		        	m_bGotDA = FALSE;
		        	SendControlMachineB();
		        }
				break;
		    case COMM_SENDCONTROLMACHINEB:
		        SendControlMoldsetB();
		        break;
		    case COMM_SENDCONTROLMOLDSETB:
		        if (m_cState != COMM_STATE_RUN)
		        	{
		        		SendControlMonitor();
		        		SendControlMonitorB();
		        	}	
		        break;
	//#endif
		    case COMM_SENDCONTROLMONITOR:
		        m_cState = COMM_STATE_RUN;
		        if (m_bSendRProfile01)  SendRProfile01();
	        	if (m_bSendRBProfile01) SendRBProfile01();
	        	break;
	      case COMM_SENDCONTROLMONITORB:
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
	//#ifdef   D_DC
		    case COMM_SENDR_BPROFILE01:
		        m_cStateRx &= ~COMM_STATE_WAITB01;
		        break;
		    case COMM_SENDS_BPROFILE01:
		        m_cStateRx &= ~COMM_STATE_WAITB02;
		        break;
	//#endif
		    default:
		        break;
	    }
	}
	else
	{
		switch (g_frameRx.wCmd)
	    {
	    	case 0x3330:
	    			wSendBlockStatus = 2;
	    			break;
	    	case COMM_SENDCONTROLMACHINE:
	    			if (m_cState != COMM_STATE_RUN)	
		        SendControlMoldset();
		        break;
		    case COMM_SENDCONTROLMOLDSET:
		    		if (m_cState == COMM_STATE_RUN)	
		        	{
		        		//printf("lWaitCount =%d \n", lWaitCount);
		        		if(lWaitCount) //2010-10-22 ADD judgement ����lWaitCount���t�ȬO���o�eMoldSet
		        			lWaitCount--;
		        		wSendMoldStatus = 2;
		        		if(lWaitCount == 1) 
		        			{
		        				wSendMoldStatus = 1;
		        				SendControlMoldset();
		        			}
		        		//printf("lWaitCount :%d\n", lWaitCount);	
		        	}	
		        if (m_bGotDA) SendPowerMatch();    //St1114'01
		        else         SendControlMonitor();
		        break;
		    case COMM_SENDPOWERMATCH:              //St1114'01
		    case COMM_SENDPOWERMATCH2:     //Andy 20061212 for New D/A
		    	if (m_cState != COMM_STATE_RUN)	//	fans add 2007/9/29 10:48�W��
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
}

void	CtmTaskRS232::Echo(WORD wCode)
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
void	CtmTaskRS232::Reset()
{
	if (m_prs232 == NULL) return;
	if(m_bCloseFlag == FALSE)
		m_prs232->Close();
	m_bCloseFlag = TRUE;
	///*if (!bGotConfig)*/ m_prs232->Change_BaudRate();	//20071128 Edision mark
	//m_prs232->Open();

	if(g_pExTable->GetTableValue(0,"D_AUSTONE"))  //JOYCE2011-2-11  add for austone
	{
  		if (((GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue >= 0x1111) &&(GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue = 0x2011))
  	 	|| (GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue > 0x2011))
	  	{
		 m_cState   = COMM_STATE_RESET;					//James add 2011/1/5 �U�� 02:40:01
		 //memset(&m_dbState2,		0, sizeof(m_dbState2));	//James add 2011/1/5 �U�� 07:42:21
		 m_bAustoneState = FALSE;						//James add 2011/1/5 �U�� 02:40:01
		 m_Transmit.wInput     	= m_Transmit.wOutput = 0;//James add 2011/1/5 �U�� 02:40:01
	  	}
	}
}

void	CtmTaskRS232::Rs485Reset()
{
	if (m_prs485 == NULL) return;
	m_prs485->Close();
	m_prs485->Open();
}

void     CtmTaskRS232::SelTempSendValue()
{
	short       i;
	WORD		wIndex, wDataID, wTempControl;
	char*		pszID;
	
	wTempControl = GetDBValue("SYSX_TEMPER_HEATERON_NULL_DUALOPTION").lValue;
	//Andy 20100309
	int	AGrouup=9,BGroup=9;
				if (g_pExTable->GetTableValue(0,"D_MIX2"))	AGrouup=BGroup=8;
	else	if (g_pExTable->GetTableValue(0,"D_MDT2"))	AGrouup=BGroup=7;
	else																							AGrouup=BGroup=9;
	//uper
	if (!((wTempControl + 1) & 0x0001))
	{
	wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
	for (i=0; i<AGrouup; i++)	
		{
		wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
		}
	}
	if (!((wTempControl + 1) & 0x0002))
	{
	wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL11_DEGREE");
	for (i=0; i<BGroup; i++)	
		{
		wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
		}
	}
}

BOOL     CtmTaskRS232::CheckOilVatID(char* pszData)
{
	short       i;

//	if(1 == CtmConfig::GetInstance()->GetKeyFlag())
	{
		for (i=0; i < (int)(sizeof(g_apszVatTransMachID) / sizeof(g_apszVatTransMachID[0])); i++)
	     	if (strcmp(pszData, g_apszVatTransMachID[i]) == 0) return TRUE;
	}
		
	//for (i=0; i < (int)(sizeof(g_apszVatTransID) / sizeof(g_apszVatTransID[0])); i++)
	for (i=0; g_apszVatTransID[i]!=NULL; i++)
	{
	  if (strcmp(pszData, g_apszVatTransID[i]) == 0)
		{
	    if(!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, �j���X�i�\�� 
	    {
	    	int j =0;
				char sz[128];
				if((i >=0) && (i <6))
				{
					j = i;
					if(j%2)
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x21);
					else
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x1E);
					if((GetDBValue(sz).lValue != 0) &&(GetDBValue(sz).lValue != 1) &&(GetDBValue(sz).lValue != 2))
						continue;
				}
			}
	 		
	 		return TRUE;
	 	}
	}
	
	return FALSE;
}

void     CtmTaskRS232::TransferOilVat()
{
	short       i;
	DWORD		dwOilVat;
	
	//for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
	for (i=0; g_apszVatTransID[i]!=NULL; i++)
	{
	    if(!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, �j���X�i�\�� 
	    {
	    	int j =0;
				char sz[128];
				if((i >=0) && (i <6))
				{
					j = i;
					if(j%2)
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x21);
					else
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x1E);
					if((GetDBValue(sz).lValue != 0) &&(GetDBValue(sz).lValue != 1) &&(GetDBValue(sz).lValue != 2))
						continue;
				}
			}

		g_pBlock->GetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, g_apszVatTransID[i], &dwOilVat);

    	if(i == 9)/*wangli2015-5-20 18:34:40 ���ҧC���פ��m*/
    		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",1,FALSE);/*wangli2015-5-20 18:12:03 �����O�y�ɡ]�G�O�� �¤G�O�S���^�A���ҧC���פ��m��׬�2 ��flag*/
		else
    		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",0,FALSE);	

	  g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, g_apszVatTransID[i], GetOilVat((int)dwOilVat));
	}
}

void     CtmTaskRS232::TransferOilVat(BOOL bTransferMach)//Andy 20070520 add;
{
	short   i;
	DWORD		dwOilVat; 
	 
	if(bTransferMach)
	{
	for (i=0; i<(int)(sizeof(g_apszVatTransMachID)/sizeof(g_apszVatTransMachID[0])); i++)
     {
     if(i==0)
     {
      if(GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue != 0xFFFF)
      {
      	if (g_pExTable->GetTableValue(0,"D_WSMVAT"))/*JOYCE2014-9-26 bug�ץ�,���Ӭ��ҪO��m,mark MSET_MLD_OPEN_STEP5_POSITION*/
      		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer,BLOCK_MACHINE_ID , g_apszVatTransMachID[i], 
      			GetOilVat((int)(GetDBValue("MSET_MLD_CLOSE_NULL_W04B7").lValue - GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue)));
      	else	
      		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer,BLOCK_MACHINE_ID , g_apszVatTransMachID[i], 
      			GetOilVat((int)(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue - GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue)));
    	}
      else return;
     }
     		else		//fuxy, 2009-2-2
     		{
			     g_pBlock->GetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, g_apszVatTransMachID[i], &dwOilVat);
			     g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, g_apszVatTransMachID[i], GetOilVat((int)dwOilVat));
     		}
     }
}

else  TransferOilVat();	
}

void     CtmTaskRS232::SendOilVat()
{
	short       i;
	
	//for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)    
	for (i=0; g_apszVatTransID[i]!=NULL; i++)
	{
	    if(!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, �j���X�i�\�� 
	    {
	    	int j =0;
				char sz[128];
				if((i >=0) && (i <6))
				{
					j = i;
					if(j%2)
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x21);
					else
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x1E);
					if((GetDBValue(sz).lValue != 0) &&(GetDBValue(sz).lValue != 1) &&(GetDBValue(sz).lValue != 2))
						continue;
				}
			}
	    if(i == 9)/*wangli2015-5-20 18:34:40 ���ҧC���פ��m*/
	    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",1,FALSE);/*wangli2015-5-20 18:12:03 �����O�y�ɡ]�G�O�� �¤G�O�S���^�A���ҧC���פ��m��׬�2 ��flag*/
		else
	    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",0,FALSE);		

		SendControlIndex(g_apszVatTransID[i]);
	}
}

void     CtmTaskRS232::SetVatDefPosi()
{
	short       i;
	WORD		wValue;
	
	//for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
	for (i=0; g_apszVatTransID[i]!=NULL; i++)
	{
	    if(!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, �j���X�i�\�� 
	    {
	    	int j =0;
				char sz[128];
				if((i >=0) && (i <6))
				{
					j = i;
					if(j%2)
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x21);
					else
						sprintf(sz,"MSET_BLST_ON_NULL_W04%2X",j/2+0x1E);
					if((GetDBValue(sz).lValue != 0) &&(GetDBValue(sz).lValue != 1) &&(GetDBValue(sz).lValue != 2))
						continue;
				}
			}

	    wValue = GetDBValue(g_apszVatTransID[i]).lValue;

    	if(i == 9)/*wangli2015-5-20 18:34:40 ���ҧC���פ��m*/
    		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",1,FALSE);/*wangli2015-5-20 18:12:03 �����O�y�ɡ]�G�O�� �¤G�O�S���^�A���ҧC���פ��m��׬�2 ��flag*/
   	 	else
    		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",0,FALSE);

	    SetDBValue(g_apszVatTransID[i], OilToPosi(wValue), FALSE);
	}
	if (g_pExTable->GetTableValue(0,"D_WSMVAT"))
		SetDBValue("MSET_MLD_CLOSE_NULL_W04B7",OilToPosi(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue),FALSE);	
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
BOOL        CtmTaskRS232::PushCmd(CMDDATA* pcmd)
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

BOOL	CtmTaskRS232::PopCmd(CMDDATA* pcmd)
{
	if ((m_CmdQueue.wInput != m_CmdQueue.wOutput) || (m_CmdQueue.bQueueFull))
	{
		//printf("PopCmd() Command=%d Index=%d Value=%d\n", pcmd->wCmd, pcmd->wIndex, pcmd->wValue);
	    pcmd->wCmd   = m_CmdQueue.CmdQueue  [m_CmdQueue.wOutput];
	    pcmd->wIndex = m_CmdQueue.IndexQueue[m_CmdQueue.wOutput];
	    pcmd->wValue = m_CmdQueue.ValueQueue[m_CmdQueue.wOutput++];
	    if (m_CmdQueue.wOutput >= m_CmdQueue.wQueueEnd) m_CmdQueue.wOutput = 0;
	    m_CmdQueue.bQueueFull = FALSE;
	    return TRUE;
	}
	
	return FALSE;
}

WORD	CtmTaskRS232::WriteToken(BYTE* pch, int nCount, int* pnWriteCount)
{
	if (m_prs232 != NULL) 
		return	m_prs232->WriteToken(pch, nCount, pnWriteCount);
	return 0xFFFF;	
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
BOOL	CtmTaskRS232::AddChartInject(tmDB_CHARTSTROKE1* pdbchartstroke1)
{
	if (m_dbchartInject.nIndex < tmMAX_STROKE)
    {
    m_dbchartInject.nX [m_dbchartInject.nIndex]   = pdbchartstroke1->nX;
    m_dbchartInject.nY1[m_dbchartInject.nIndex]   = pdbchartstroke1->nY1;
    m_dbchartInject.nY2[m_dbchartInject.nIndex++] = pdbchartstroke1->nY2;
    }

	return TRUE;
}

BOOL	CtmTaskRS232::AddChartHold  (tmDB_CHARTSTROKE1* pdbchartstroke1)
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

BOOL	CtmTaskRS232::AddChartInjectB(tmDB_CHARTSTROKE1* pdbchartstroke1)
{
	if (m_dbchartInjectB.nIndex < tmMAX_STROKE)
	    {
	    m_dbchartInjectB.nX [m_dbchartInjectB.nIndex]   = pdbchartstroke1->nX;
	    m_dbchartInjectB.nY1[m_dbchartInjectB.nIndex]   = pdbchartstroke1->nY1;
	    m_dbchartInjectB.nY2[m_dbchartInjectB.nIndex++] = pdbchartstroke1->nY2;
	    }
	
	return TRUE;
}

BOOL	CtmTaskRS232::AddChartHoldB  (tmDB_CHARTSTROKE1* pdbchartstroke1)
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

BOOL   CtmTaskRS232::AddChartCharge(tmDB_CHARTSTROKE1* pdbchartstroke1)		//Sunny<20070815>
{
	if (m_dbchartChrg.nIndex < tmMAX_STROKE)
	    {
	    m_dbchartChrg.nX [m_dbchartChrg.nIndex]   = pdbchartstroke1->nX;
	    m_dbchartChrg.nY1[m_dbchartChrg.nIndex]   = pdbchartstroke1->nY1;
	    m_dbchartChrg.nY2[m_dbchartChrg.nIndex++] = pdbchartstroke1->nY2;
	    }
	return TRUE;
}

BOOL   CtmTaskRS232::AddChartChargeB(tmDB_CHARTSTROKE1* pdbchartstroke1)		//Sunny<20070815>
{
	if (m_dbchartChrgB.nIndex < tmMAX_STROKE)
	    {
	    m_dbchartChrgB.nX [m_dbchartChrgB.nIndex]   = pdbchartstroke1->nX;
	    m_dbchartChrgB.nY1[m_dbchartChrgB.nIndex]   = pdbchartstroke1->nY1;
	    m_dbchartChrgB.nY2[m_dbchartChrgB.nIndex++] = pdbchartstroke1->nY2;
	    }
	return TRUE;
}

DWORD	CtmTaskRS232::GetInjectTime(int nIndex)
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

WORD	CtmTaskRS232::test2USB(WORD wOp,void* pOutData, int nSize)
{
	static	int		MountState= 1; //0:success 1:error
	static	int		Num				= 0; //
	static	int		FileType	= 0; //0:NoUse 1:write 2:read
					int 	fd;
		
	if (wOp == 0) //mount USB
	{
		FileType 	= 1;	//	write status;
		Num				= 0;	
	
		if (MountState != 0)
		{				
			#ifdef	D_PXA270
			MountState = mount("/dev/sda1", "/mnt/nfs", "vfat", 0, NULL);
			#else
			MountState = mount("/dev/mmca1", "/mnt/nfs", "vfat", 0, NULL);
			#endif
		}
		
		if (MountState != 0)
		{
			Prompt("NoUSB",2);	
		}
		else
		{
			Prompt("USBMount",2);
			system("rm -rf /mnt/nfs/test.txt");
		}	
	}
	else if(wOp == 1) //write USB
	{
		if (MountState == 0)
		{
			char* pTemp = new char[256];

			fd = open("/mnt/nfs/test.txt", O_RDWR | O_CREAT);			
	 		strcpy(pTemp,(char*)pOutData);
	 		if (FileType == 1)
			{
				lseek(fd, 0, SEEK_END);//handle ����󵲧�
				write(fd, pTemp, strlen(pTemp));				
		 		Prompt("writeUSB",2);
		 		Num++;
		 		close(fd);
			}
			else
			{
				close(fd);
				Prompt("USBNoAction", 2);
			}
			
			delete[] pTemp;
		}							
	} 
	else if(wOp == 2) //umount USB
	{
		usleep(1000 * 50);
		int umountState = umount("/mnt/nfs");	
				
		if (umountState == 0)            
		{
			MountState = 1;
			Prompt("USBRemove",2);
		}
		else
			Prompt("USBRemove Failed",2);
		
		Num = 0;
		FileType = 0;		
	}
	return	0;
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
int		CtmTaskRS232::WriteValue(int nDataID)
{
	WORD	wType;
	wType = nDataID;
	
	switch(wType)
	{
		case 0x3330:
			wSendBlockStatus = 1;
	    SendBlock61();
	    break;
		case COMM_SENDCONFIG:		
			wSendConfStatus = 1;
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
	  	{	  		
	  		//printf("open mold : %ld\n",GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue);
	  		//printf("write lWaitCount:%d\n",lWaitCount);
	  		if(lWaitCount == 0)
	  			{
	  				wSendMoldStatus = 1;
	  				SendControlMoldset();
	  			}	
	  		lWaitCount++;
	  		if(lWaitCount > 2) lWaitCount--;
	  	}	
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
	  case COMM_SENDCONTROLMACHINEB:
	    SendControlMachineB();
	    break;
	  case COMM_SENDCONTROLMOLDSETB:
	  	SendControlMoldsetB();
	  	break;
	  case COMM_SENDCONTROLMONITORB:
	  	SendControlMonitorB();
	  	break;
	  case COMM_INIT_MESSAGE:
	  	InitMessage();
	  	break;
	  default:
	  	break;
	 }
	 return 1;
}

int		CtmTaskRS232::WriteValue(int nType, int nHostAdreess1, int nHostAdreess2)
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
	
int		CtmTaskRS232::WriteValue(int nType, void* pInputData, int nSize)
{
	if (m_cState == COMM_STATE_RUN) //����talk���إߡA�_�h�����\�o�e�E�_���O
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
		case COMM_SENDTRANSMITDSP28://JOYCE2011-2-11 add for austone
			//printf("COMM_SENDTRANSMITDSP28 rs232 pInputData=%x \n", pInputData);
	    	if((1 == m_bAustoneCfgFlag) || (1 == m_bN2CFlag))
	    	{
		    	if(pInputData != NULL)
				{
					SendTransmit(pInputData, nSize);
				}
			}
			break;
		default:
			break;
	}
	return 1;
}


int		CtmTaskRS232::ReqValues(int nType)
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
	    case 0x8000:  //Andy 20091105 for usb TEST!
	    	test2USB(0);
	    	break;
	    case 0x8002:
	    	test2USB(2);
	    	break;	    
	    default:
			break;
	}
	return 1;	
}

int		CtmTaskRS232::ReqValues(int nType, int nHostAdreess1, int nHostAdreess2)
{
	return 0;
}

int 	CtmTaskRS232::ReqValues(int nType, void* pOutData, int nSize)
{
	switch(nType)
	{
		case COMM_GETDAFLAG:
			memcpy(pOutData, &m_bNewDA ,sizeof(m_bNewDA));
			break;
		case COMM_GETCOMMAND:
			memcpy(pOutData, &m_nCommandIndex ,sizeof(m_nCommandIndex));
			break;
		case COMM_GETDIAGRM:
			//*pData = (void*)GetDBDiagRM();
			//printf("GetDBDiagRM()=%x =%x\n", GetDBDiagRM(), pData);
			if(nSize == 0)
				memcpy(pOutData, GetDBDiagRM() ,sizeof(tmDB_DIAGRM));
			else
				memcpy(pOutData, GetDBDiagRM() ,nSize);
			break;
		case COMM_GETDIAGRI:
			//*pData = (void*)GetDBDiagRI();
			if(nSize == 0)
				memcpy(pOutData, GetDBDiagRI() ,sizeof(tmDB_DIAGRI));
			else
				memcpy(pOutData, GetDBDiagRI() ,nSize);
			break;
		case COMM_GETAUSTIONDRIVEINFO://JOYCE2011-2-11 
			if(nSize == 0)
				memcpy(pOutData, GetDBAustonDrive() ,sizeof(tmDB_AUSTONDRIVEINFO));
			else
				memcpy(pOutData, GetDBAustonDrive() ,nSize);
			break;
		case 0x6502:  //JOYCE2008-9-24 add,for 7GL flag
			memcpy(pOutData, &m_bUseSlow7GL ,sizeof(m_bUseSlow7GL));
			break;
		case 0x6503:    //ZHBA  2008-10-31 for 7CM injetime
			{
			DWORD  dInjTime[6];
			for(int i=0;i<6;i++)
			dInjTime[i] = GetInjectTime(i);
			if(nSize == 0)
				memcpy(pOutData, &dInjTime[0] ,sizeof(DWORD));
			else
				memcpy(pOutData, &dInjTime[0] ,nSize);
			}
			break;
		case COMM_GETAIRPCPOINT:    /*joyce212-12-19 PC point,0x6600*/
			if(nSize == 0)
				memcpy(pOutData, &m_nAirPCPoint[0] ,sizeof(int));
			else
				memcpy(pOutData, &m_nAirPCPoint[0] ,nSize);
			break;
		case COMM_GETAIRPCDATA:    /*joyce212-12-19 PC channel,0x6601*/
			if(nSize == 0)
				memcpy(pOutData, &m_AirPCDataID[0] ,sizeof(char));
			else
				memcpy(pOutData, &m_AirPCDataID[0] ,nSize);
			break;
		case 0x7000:	//2008-10-23
			memcpy(pOutData, &m_cState ,sizeof(m_cState));
			break;
		case 0x7001:	//2009-11-6	    	
			memcpy(pOutData, &wSendConfStatus ,sizeof(wSendConfStatus));
			if(wSendConfStatus == 2)	wSendConfStatus = 0;
			break;
    case 0x7002://JOYCE2011-2-11 
      	if(pOutData != NULL)
      	 {
      		 //ZHBA 2011-1-11 �P�_�D��lib����ݮe�s�¥D��
				 if (GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue < 0x1111 && (GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue <= 0x2011))
      		  if(!m_bAustoneState)m_bAustoneState =TRUE;

      		memcpy(pOutData, &m_bAustoneState ,sizeof(m_bAustoneState));
      	 }
   	break;
		case 0x8000:  //Andy 20091105 for usb TEST!
			test2USB(0);
			break;
		case 0x8001:
			test2USB(1, pOutData, nSize);
			break;
		case 0x8002:
			test2USB(2);
			break;
		default:
			break;
	}
	return 1;
}

int CtmTaskRS232::ReqValues(int nType, int nValue1, int nPrecision1, int nValue2, int nPrecision2)		//JOYCE2010-12-16 
{
	m_lCounter485ReqS = COMM_TIME_485REQS;	//fuxy, 2012-6-14 
	switch(nType)
	{
		case MUCELL_COMM_TASKCODE_SEND_B:
			Request_B(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_C:		//fuxy, 2012-3-12 
			m_lCounter485ReqC = COMM_TIME_485REQC;
			if(m_bMUCELL2)
				{
					Request_C2(nValue1, nPrecision1); //JOYCE2012-6-29 
				}
			else
				{
					nCalibrat = 1;
					Request_C(nValue1, nPrecision1);
				}	
			break;
		case MUCELL_COMM_TASKCODE_SEND_D:
			if(m_bMUCELL2)
				Request_D2(nValue1, nPrecision1);  //JOYCE2012-6-29 
			break;
		case MUCELL_COMM_TASKCODE_SEND_E:
			Request_E(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_F:
			if(m_bMUCELL2)
				Request_F2(nValue1, nPrecision1);  //JOYCE2012-6-29 
			break;
		case MUCELL_COMM_TASKCODE_SEND_G:
			Request_G(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_H:
			Request_H(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_J:
			Request_J(nValue1, nPrecision1);
			break;
//		case MUCELL_COMM_TASKCODE_SEND_L:		//fuxy, 2011-4-21, mark 
//			Request_L(nValue1, nValue2, nPrecision1, nPrecision2);
//			break;
		case MUCELL_COMM_TASKCODE_SEND_M:
			if(m_bMUCELL2)
				Request_M2(nValue1, nPrecision1);  //JOYCE2012-6-29 
			break;
		case MUCELL_COMM_TASKCODE_SEND_P:		//fuxy, 2011-4-21 
			Request_P(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_R:
			Request_R(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_S:
			//if(m_bMUCELL2)
			//	Request_S2(nValue1, nPrecision1);  //JOYCE2012-6-29 
			//else	
				Request_S(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_T:
			Request_T(nValue1, nPrecision1);
			break;
		case MUCELL_COMM_TASKCODE_SEND_U:
			if(m_bMUCELL2)
				Request_U2(nValue1, nPrecision1);  //JOYCE2012-6-29 
			break;
		default:
			break;
	}         
	
	return 1;
}
/*=====================================================================================
//
//MuCell Task //JOYCE2010-12-16 
//=====================================================================================*/
void	CtmTaskRS232::CreaMuCell()
{
	tmRS232       rs232;	
 	memset(&rs232, 0, sizeof(rs232));
 	rs232.wChannel 		   = _COM2;
	rs232.config.DataLength= COM_DL_8;
	rs232.config.StopBit   = COM_SB_1;
	rs232.config.Parity    = COM_PR_NONE;
	rs232.config.BaudRate  = COM_BR_9600;
	rs232.config.Handshake = COM_HS_NONE;

	rs232.wRxBufferSize = sizeof(g_acRxBuffer2);
	rs232.fpRxBuffer    = (BYTE *)&g_acRxBuffer2;
	rs232.wTxBufferSize = sizeof(g_acTxBuffer2);
	rs232.fpTxBuffer    = (BYTE *)&g_acTxBuffer2;
                                                	
    m_lCounter485State = COMM_TIME_RESET;	//Caozz 2010/12/16 �W�� 08:56:04
    AddCounter((long *)&m_lCounter485State, m_idMe);        
    m_lCounter485ReqS = COMM_TIME_485REQS;	//Caozz 2010/12/16 �W�� 08:56:04
    AddCounter((long *)&m_lCounter485ReqS, m_idMe);                                    
	m_lCounter485ReqC = COMM_TIME_485REQC; 		//fuxy, 2012-3-13 
    AddCounter((long *)&m_lCounter485ReqC, m_idMe);
    
	m_nID2				= AddDevice("CRS485", &rs232);
	m_prs485			= NULL;
	if (m_nID2 != -1) 
		m_prs485		= (CRS485 *)GetDevice(m_nID2);	
}

int		CtmTaskRS232::RS485ReadToken()
{
	int nReadCount = 0;
	if(m_prs485 != NULL)
	{
		if(m_prs485->ReadToken((BYTE*)&g_frame485Rx, sizeof(g_frame485Rx), &nReadCount) == COM_SUCCESS)
		{
			//printf("485Rx.bTaskCmd:%4X \n", g_frame485Rx.bTaskCmd);
			m_lCounter485ReqS = COMM_TIME_485REQS;	//fuxy, 2012-6-14 
			switch(g_frame485Rx.bTaskCmd)
			{
				case MUCELL_COMM_TASKCODE_REV_B:
					Get_B(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_C:
					if(m_bMUCELL2)
						Get_C2(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);		//JOYCE2012-6-29  
					else	
						Get_C(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);		//fuxy, 2012-3-13 
					break;
				case MUCELL_COMM_TASKCODE_REV_D:
					if(m_bMUCELL2)
						Get_D2(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);   //JOYCE2012-6-29 
					break;	
				case MUCELL_COMM_TASKCODE_REV_E:
					Get_E(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_F:
					if(m_bMUCELL2)
						Get_F2(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);   //JOYCE2012-6-29 
					break;	
				case MUCELL_COMM_TASKCODE_REV_G:
					Get_G(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_H:
					Get_H(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_J:
					Get_J(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
//				case MUCELL_COMM_TASKCODE_REV_L:	//fuxy, 2011-4-21, mark 
//					Get_L(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
//					break;
				case MUCELL_COMM_TASKCODE_REV_M:
					if(m_bMUCELL2)
						Get_M2(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);   //JOYCE2012-6-29 
					break;	
				case MUCELL_COMM_TASKCODE_REV_P:	//fuxy, 2011-4-21 
					Get_P(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_R:
					Get_R(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_S:
					if(m_bMUCELL2)
						Get_S2(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);  //JOYCE2012-6-29 
					else						
						Get_S(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_T:
					Get_T(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);
					break;
				case MUCELL_COMM_TASKCODE_REV_U:
					if(m_bMUCELL2)
						Get_U2(g_frame485Rx.commdata.acBuffer, g_frame485Rx.dwLen);   //JOYCE2012-6-29 
					break;	
				default:
					g_wDsp2Status = FALSE;
//					printf("Receive Cmd Error \n");
					break;
			}
		}
		else
		{
			//printf("test get else \n"); 
			if(iRequestSCnt > 1)		//fuxy, 2011-9-9 
			{
				Rs485Reset();
				g_wDsp2Status = FALSE; 
				m_bMucellRun  = FALSE; 		//fuxy, 2012-3-15 
//				printf("Rs485Reset :%d\n",iRequestSCnt);
				iRequestSCnt = 0;
			}				
		}
	}
	
	if(m_lCounter485ReqS < 0)	//require SCF status
	{
		ReqValues(MUCELL_COMM_TASKCODE_SEND_S, 0, 0, 0, 0);
		m_lCounter485ReqS = COMM_TIME_485REQS;
	}
	
	if(m_bMUCELL)
		{
			if((m_lCounter485ReqC < 0) && nCalibrat)	//fuxy, 2012-3-13 
			{
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12", 2, FALSE);		//�S������o�w��  �G���ծժ��^�_ 
				nCalibrat = 0;
				//printf("not Get_C \n");
			}
		}
	
	return 0;
}

//Get B
float		CtmTaskRS232::Get_B(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[5], cMinimum[5], cMaximum[5];
	float	fConfirm = 0,	fMinimum = 0, fMaximum = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	memcpy(cMinimum, psz+5, sizeof(cMinimum));
	memcpy(cMaximum, psz+10, sizeof(cMaximum));
	
	fConfirm = atof((char*)cConfirm);
	fMinimum = atof((char*)cMinimum);
	fMaximum = atof((char*)cMaximum);
	
	//printf("Get_B fConfirm=%f fMinimum=%f fMaximum=%f \n", fConfirm, fMinimum, fMaximum);
	return fConfirm;
}

//Get C
float		CtmTaskRS232::Get_C(BYTE*	psz, DWORD	dwLen)		//fuxy, 2012-3-13 
{
	BYTE	cConfirm[1];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	
	if((long)fConfirm == 0)
	{
		nCalibrat = 0;
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12", 0, FALSE);
	}
	else
	{
		nCalibrat = 3;
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12", 3, FALSE);		//2012-6-15 
		m_lCounter485ReqC = COMM_TIME_485REQC;
	}
	
	//printf("Get_C  %f \n", fConfirm);
	return 0;
}

/*		//fuxy, 2011-4-21, mark 
//Get D
float		CtmTaskRS232::Get_D(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[4];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	
	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED66", (long)fConfirm*100);	
	//printf("Get_D fConfirm=%f \n", fConfirm);
	return fConfirm;
}
*/
//Get E
float		CtmTaskRS232::Get_E(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[5];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	
	//printf("Get_E fConfirm=%f \n", fConfirm);
	return fConfirm;
}

//Get G
float		CtmTaskRS232::Get_G(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[4];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	
	//printf("Get_G fConfirm=%f\n", fConfirm);
	return fConfirm;
}

//Get H
float		CtmTaskRS232::Get_H(BYTE*	psz, DWORD	dwLen)
{
	//printf("Get_H \n");
	return 0;
}

//Get J
float		CtmTaskRS232::Get_J(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[6];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	
	//printf("Get_J fConfirm=%f\n", fConfirm);
	return fConfirm;
}
/*		//fuxy, 2011-4-21, mark 
//Get L
float		CtmTaskRS232::Get_L(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm;
	float	fConfirm = 0;
	
	cConfirm = *psz;
	
	fConfirm = atof((char*)&cConfirm);
	
	//printf("Get_L fConfirm=%f\n", fConfirm);
	return fConfirm;
}
*/
//Get R
float		CtmTaskRS232::Get_R(BYTE*	psz, DWORD	dwLen)
{
	//printf("Get_R \n");
	return 0;
}

//Get S
float		CtmTaskRS232::Get_S(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cSCFSetPoint[5], cAvgValue[5], cMinimum[5], cMaximum[5], cMinTime[5], cActPress[5], cProgrammStep[4], cStateByte[2];
	int  nOffset =0;
	float	fSCFSetPoint = 0, fAvgValue = 0, fMinimum = 0, fMaximum = 0, fMinTime = 0, fActPress = 0;
	int		nProgrammStep = 0, nStateByte = 0;
	
	memcpy(cSCFSetPoint, psz, sizeof(cSCFSetPoint));
	nOffset += 5;
	memcpy(cAvgValue, 	psz +nOffset, sizeof(cAvgValue));
	nOffset += 5;
	memcpy(cMinimum, 	psz +nOffset, sizeof(cMinimum));
	nOffset += 5;
	memcpy(cMaximum, 	psz +nOffset, sizeof(cMaximum));
	nOffset += 5;
	memcpy(cMinTime, 	psz +nOffset, sizeof(cMinTime));		//fuxy, 2011-4-21, add 
	nOffset += 5;
	memcpy(cActPress, 	psz +nOffset, sizeof(cActPress));		//fuxy, 2012-5-18, add �������O 
	nOffset += 5;
	memcpy(cProgrammStep, psz +nOffset, sizeof(cProgrammStep));
	nOffset += 4;
	memcpy(cStateByte, psz+nOffset, sizeof(cStateByte));
	
	fSCFSetPoint 	= atof((char*)cSCFSetPoint);
	fAvgValue 		= atof((char*)cAvgValue);
	fMinimum 		= atof((char*)cMinimum);
	fMaximum 		= atof((char*)cMaximum);
	fMinTime 		= atof((char*)cMinTime);		//fuxy, 2011-4-21, add 
	fActPress 		= atof((char*)cActPress);
	nProgrammStep 	= atoi((char*)cProgrammStep);
	nStateByte 		= atoi((char*)cStateByte);		//array to int 

  //JOYCE2011-8-2 test
/*	tmDATE  date;
	tmTIME  time;
	GetDate(&date);
	GetTime(&time);	
	char	pszTest[256];
	sprintf(pszTest,"Get_S fSCFSetPoint=%f,fAvgValue=%f,fMinimum=%f,fMaximum=%f,nProgrammStep=%d,cStateByte=%d, %d-%d:%d:%d \r\n", fSCFSetPoint, fAvgValue, fMinimum, fMaximum, nProgrammStep, nStateByte,date.day, time.hour, time.minute, time.second);	        
	ReqValues(0x8001, pszTest, strlen(pszTest));		//test 
*/	
	//printf("Get_S fSCFSetPoint=%f fAvgValue=%f fMinimum=%f fMaximum=%f nProgrammStep=%d cStateByte=%d \n", 
	//	fSCFSetPoint, fAvgValue, fMinimum, fMaximum, nProgrammStep, nStateByte);

	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED67", nProgrammStep);		//ProgrammStep,  //2012-6-14, ProgrammStep/10 & cProgrammStep[5]; 
	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED68", (long)(fAvgValue *100));//AVERAGE PERCENT 
	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED69", (long)(fMaximum  *100));	//MAX PERCENT	
	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED70", (long)(fMinimum  *100));	//MIN PERCENT
	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED66", (long)(fMinTime  *100));		//fuxy, 2011-4-21, add, �̤p����ɶ� 	fMinTime *100
	SetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED74", (long)(fActPress *10/145)); 	//2012-5-18, psi to bar 
	
	if((nProgrammStep == 2080) && (nCalibrat !=0))
	{
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED12", 0, FALSE);
		nCalibrat = 0;
	}
	
	iRequestSCnt--;		//fuxy, 2011-9-9 
//	printf("Get_S fSCFSetPoint=%f nProgrammStep=%d cStateByte=%d, iRequestSCnt=%d;   %d:%d:%d  \n", 
//		fSCFSetPoint, nProgrammStep, nStateByte, iRequestSCnt, time.hour, time.minute, time.second);
	
	
	g_wDsp2Status = TRUE;
	m_lCounter485State = COMM_TIME_RESET;
	
	if(m_bMucellRun == FALSE)		//fuxy, 2012-3-15     �q�T�إߦZ, �� �`�g���q/�`��ʤ��񵹵o�w��
	{
		DWORD	dwValue = 0;
		dwValue = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED71").lValue;
		if(g_ptaskdsp != NULL) 
	    	g_ptaskdsp->ReqValues(MUCELL_COMM_TASKCODE_SEND_E, dwValue * 10, 2,0, 0);		// �`�g���q 
	    
	    usleep(100*1000);				//100ms 
	    dwValue = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED72").lValue;
	    if(g_ptaskdsp != NULL) 
	    	g_ptaskdsp->ReqValues(MUCELL_COMM_TASKCODE_SEND_B, dwValue, 2,0, 0);			//�`��ʤ���
	    
	    usleep(100*1000);				//100ms 
	    dwValue = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED65").lValue;
	    if(g_ptaskdsp != NULL) 
	    	g_ptaskdsp->ReqValues(MUCELL_COMM_TASKCODE_SEND_G, dwValue*145/10, 0,0, 0);		//��l�������O 
	    
	    m_bMucellRun  = TRUE;
	}
	
	return 0;
}

//Get T
float		CtmTaskRS232::Get_T(BYTE*	psz, DWORD	dwLen)
{
	BYTE	bState;
	int		nState = 0;
	bState = *psz;
	
	nState = atoi((char*)&bState);
	//printf("Get_T nState=%d \n", nState);
	return 0;
}

//Get P
float		CtmTaskRS232::Get_P(BYTE*	psz, DWORD	dwLen)		//fuxy, 2011-4-21   ???
{
	BYTE	cConfirm[5];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	//printf("Get_P \n");
	return 0;
}

//Request B 
void	CtmTaskRS232::Request_B(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00C";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_B;	//'B';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%05d", nValue);
	SetPonit(cDataValue, nPrecision, 5);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("B cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+5+1, &nWriteCount);
}

//Request C 
void	CtmTaskRS232::Request_C(int	nValue, int nPrecision)		//fuxy, 2012-3-12 
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "007";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_C;	//'C';
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cTempValue, "%s%c", cDataLen, cTaskID);
	//printf("C cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1, &nWriteCount);	
}

/*		//fuxy, 2011-4-21, mark 
//Request D 
void	CtmTaskRS232::Request_D(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "007";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_D;	//'D';
	//JOYCE2010-12-6 BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cTempValue, "%s%c", cDataLen, cTaskID);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1, &nWriteCount);
}
*/
//Request E 
void	CtmTaskRS232::Request_E(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00D";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_E;	//'E';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%06d", nValue);
	SetPonit(cDataValue, nPrecision, 6);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("E cTempValue=%s \n", cTempValue);
//	tmTIME  time;
//	GetTime(&time);	
//	printf("request  E;  %d:%d:%d \n", time.hour, time.minute, time.second);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+6+1, &nWriteCount);
	
}

//Request G 
void	CtmTaskRS232::Request_G(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00B";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_G;	//'G';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%04d", nValue);
	SetPonit(cDataValue, nPrecision, 4);
	//printf("cDataValue=%d \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("G cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+4+1, &nWriteCount);
	
}

//Request H 
void	CtmTaskRS232::Request_H(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "007";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_H;	//'H';
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cTempValue, "%s%c", cDataLen, cTaskID);
	//printf("H cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1, &nWriteCount);
	
}

//Request J 
void	CtmTaskRS232::Request_J(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00D";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_J;	//'J';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%06d", nValue);
	SetPonit(cDataValue, nPrecision, 6);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("J cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+6+1, &nWriteCount);
}

/*		//fuxy, 2011-4-21, mark 
//Request L 
void	CtmTaskRS232::Request_L(int	nValue1, int nValue2, int nPrecision1, int nPrecision2)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "013";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_L;	//'L';
	BYTE	cDataValue1[6]	= "00000", cDataValue2[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue1, "%06d", nValue1);
	sprintf((char*)cDataValue2, "%06d", nValue2);
	
	SetPonit(cDataValue1, nPrecision1, 6);
	SetPonit(cDataValue2, nPrecision2, 6);
	
	//printf("cDataValue1=%s cDataValue1=%s \n", cDataValue1,cDataValue2);
	sprintf((char*)cTempValue, "%s%c%s%s", cDataLen, cTaskID, cDataValue1, cDataValue2);
	//printf("L cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+12+1, &nWriteCount);
}
*/

//Request R 
void	CtmTaskRS232::Request_R(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "007";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_R;	//'R';
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cTempValue, "%s%c", cDataLen, cTaskID);
	//printf("R cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1, &nWriteCount);
	
}

//Request S
void	CtmTaskRS232::Request_S(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "007";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_S;	//'S';
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cTempValue, "%s%c", cDataLen, cTaskID);
	iRequestSCnt++;		//fuxy, 2011-9-9 
//	printf("S cTempValue=%s \n", cTempValue);
//	tmTIME  time;
//	GetTime(&time);	
//	printf("request  S  Cnt:%d;  %d:%d:%d \n",iRequestSCnt, time.hour, time.minute, time.second);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1, &nWriteCount);
	
}

//Request T
void	CtmTaskRS232::Request_T(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "008";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_T;	//'T';
	BYTE	cData;
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	if(nValue == 0)
		cData = '0';
	else
		cData = '1';
	sprintf((char*)cTempValue, "%s%c%c", cDataLen, cTaskID, cData);
	//printf("T cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1+1, &nWriteCount);
	
}

//Request P 
void	CtmTaskRS232::Request_P(int	nValue, int nPrecision)		//fuxy, 2011-4-21 
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "007";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_P;	//'P';
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cTempValue, "%s%c", cDataLen, cTaskID);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1, &nWriteCount);
}

void	CtmTaskRS232::SetPonit(BYTE*	psz, WORD wPrecision, WORD wLen)
{
	//float fValue = ((float)lValue)/GetDecimalPowerVal(wPrecision);
	char	cszPointLeft[50], cszPonitRight[50];
	BYTE	cPoint = '.';
	
	memset(cszPointLeft, 0, sizeof(cszPointLeft));
	memset(cszPonitRight, 0, sizeof(cszPonitRight));
	
	if(psz != NULL)
	{
		if(wPrecision > 0 && wPrecision < wLen)
		{	
			memcpy(cszPointLeft, psz, wLen-wPrecision);
			memcpy(cszPonitRight, &psz[wLen-wPrecision], wPrecision);
		sprintf((char*)psz, "%s%c%s", &cszPointLeft[1], cPoint, cszPonitRight);
		}
		//printf("psz=%s cszPointLeft=%s cszPonitRight=%s\n", psz, cszPointLeft, cszPonitRight);
	}
}

/*=====================================================================================
//
//MuCell_2 Task //JOYCE2012-6-29  
//=====================================================================================*/
//Get C2
float		CtmTaskRS232::Get_C2(BYTE*	psz, DWORD	dwLen)		//fuxy, 2012-3-13 
{
	BYTE	cConfirm[1];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);
	//printf("Get_C2 fConfirm=%f \n", fConfirm);
	
	return 0;
}

//Get D2
float		CtmTaskRS232::Get_D2(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[6];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);	
	//printf("Get_D2 fConfirm=%f \n", fConfirm);

	return fConfirm;
}

//Get F2
float		CtmTaskRS232::Get_F2(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[6];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);	
	//printf("Get_F2 fConfirm=%f \n", fConfirm);

	return fConfirm;
}

//Get M2
float		CtmTaskRS232::Get_M2(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[6];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);	
	//printf("Get_M2 fConfirm=%f \n", fConfirm);

	return fConfirm;
}

//Get S2
float		CtmTaskRS232::Get_S2(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cMinFlow[6], cMaxFlow[6], cCurrentFlow[6], cSuctionPrs[6], cBoostPrs[6];
	BYTE  cPreMeterPrs[6], cPostMeterPrs[6], cDeliveryPrs[6], cDeliveryPrsDrop[6];
	BYTE	cAlarmBits[8], cSystemModel[11], cSerialNumer[5], cStateWord[4];
	int  nOffset =0;
	float	fMinFlow = 0, fMaxFlow = 0, fCurrentFlow = 0, fSuctionPrs = 0, fBoostPrs = 0;
	float	fPreMeterPrs = 0, fPostMeterPrs = 0, fDeliveryPrs = 0, fDeliveryPrsDrop = 0;
	float fAlarmBits = 0, fSystemModel = 0, fSerialNumer = 0;
	
	int		nStateWord = 0;
	
	memcpy(cMinFlow, psz, sizeof(cMinFlow));
	nOffset += 6;
	memcpy(cMaxFlow, 	psz +nOffset, sizeof(cMaxFlow));
	nOffset += 6;
	memcpy(cCurrentFlow, 	psz +nOffset, sizeof(cCurrentFlow));
	nOffset += 6;
	memcpy(cSuctionPrs, 	psz +nOffset, sizeof(cSuctionPrs));
	nOffset += 6;
	memcpy(cBoostPrs, 	psz +nOffset, sizeof(cBoostPrs));
	nOffset += 6;
	memcpy(cPreMeterPrs, 	psz +nOffset, sizeof(cPreMeterPrs)); 
	nOffset += 6;
	memcpy(cPostMeterPrs, psz +nOffset, sizeof(cPostMeterPrs));
	nOffset += 6;
	memcpy(cDeliveryPrs, psz +nOffset, sizeof(cDeliveryPrs));
	nOffset += 6;
	memcpy(cDeliveryPrsDrop, psz +nOffset, sizeof(cDeliveryPrsDrop));
	nOffset += 6;
	memcpy(cAlarmBits, psz +nOffset, sizeof(cAlarmBits));
	nOffset += 8;
	memcpy(cSystemModel, psz +nOffset, sizeof(cSystemModel));
	nOffset += 11;
	memcpy(cSerialNumer, psz +nOffset, sizeof(cSerialNumer));
	nOffset += 5;
	memcpy(cStateWord, psz+nOffset, sizeof(cStateWord));
	
	fMinFlow  	= atof((char*)cMinFlow);
	fMaxFlow 		= atof((char*)cMaxFlow);
	fCurrentFlow 	= atof((char*)cCurrentFlow);
	fSuctionPrs 	= atof((char*)cSuctionPrs);
	fBoostPrs 		= atof((char*)cBoostPrs);
	fPreMeterPrs 	= atof((char*)cPreMeterPrs);
	fPostMeterPrs	= atof((char*)cPostMeterPrs);
	fDeliveryPrs 	= atof((char*)cDeliveryPrs);
	fDeliveryPrsDrop 	= atof((char*)cDeliveryPrsDrop);

	fAlarmBits 	= atof((char*)cAlarmBits);
	fSystemModel 	= atof((char*)cSystemModel);
	fSerialNumer 	= atof((char*)cSerialNumer);

	nStateWord 		= atoi((char*)cStateWord);		//array to int 

  //JOYCE2011-8-2 test
/*	tmDATE  date;
	tmTIME  time;
	GetDate(&date);
	GetTime(&time);	
	char	pszTest[256];
	sprintf(pszTest,"Get_S fSuctionPrs=%f fBoostPrs=%f fPreMeterPrs=%f fPostMeterPrs=%f fDeliveryPrs=%f fDeliveryPrsDrop=%f fMinFlow=%f fMaxFlow=%f fCurrentFlow=%f, %d-%d:%d:%d \r\n", fSuctionPrs, fBoostPrs, fPreMeterPrs, fPostMeterPrs, fDeliveryPrs, fDeliveryPrsDrop,fMinFlow,fMaxFlow,fCurrentFlow,date.day, time.hour, time.minute, time.second);	        
	ReqValues(0x8001, pszTest, strlen(pszTest));		//test 
	*/
//	printf("Get_S fSuctionPrs=%f fBoostPrs=%f fPreMeterPrs=%f fPostMeterPrs=%f fDeliveryPrs=%f fDeliveryPrsDrop=%f fMinFlow=%f fMaxFlow=%f fCurrentFlow=%f\n", 
//		fSuctionPrs, fBoostPrs, fPreMeterPrs, fPostMeterPrs, fDeliveryPrs, fDeliveryPrsDrop,fMinFlow,fMaxFlow,fCurrentFlow);
	
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED60", (long)(fSuctionPrs *10));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED61", (long)(fBoostPrs *10));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED62", (long)(fPreMeterPrs *10));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED63", (long)(fPostMeterPrs *10));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED64", (long)(fDeliveryPrs *10));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED65", (long)(fDeliveryPrsDrop *10));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED67", (long)(fMinFlow *100));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED66", (long)(fMaxFlow *100));
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED68", (long)(fCurrentFlow *100));
	
	iRequestSCnt--;		//fuxy, 2011-9-9 
//	printf("Get_S fMinFlow=%f nProgrammStep=%d cStateByte=%d, iRequestSCnt=%d;   %d:%d:%d  \n", 
//		fMinFlow, nProgrammStep, nStateByte, iRequestSCnt, time.hour, time.minute, time.second);
	
	
	g_wDsp2Status = TRUE;
	m_lCounter485State = COMM_TIME_RESET;
	
	if(m_bMucellRun == FALSE)		//fuxy, 2012-3-15     �q�T�إߦZ, �� �`�g���q/�`��ʤ��񵹵o�w��
	{
		/*
		DWORD	dwValue = 0;
		dwValue = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED71").lValue;
		if(g_ptaskdsp != NULL) 
	    	g_ptaskdsp->ReqValues(MUCELL_COMM_TASKCODE_SEND_E, dwValue * 10, 2,0, 0);		// �`�g���q 
	    
	    usleep(100*1000);				//100ms 
	    dwValue = GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED72").lValue;
	    if(g_ptaskdsp != NULL) 
	    	g_ptaskdsp->ReqValues(MUCELL_COMM_TASKCODE_SEND_B, dwValue, 2,0, 0);			//�`��ʤ���
	    */
	    m_bMucellRun  = TRUE;
	}
	
	return 0;
}


//Get U2
float		CtmTaskRS232::Get_U2(BYTE*	psz, DWORD	dwLen)
{
	BYTE	cConfirm[2];
	float	fConfirm = 0;
	
	memcpy(cConfirm, psz, sizeof(cConfirm));
	
	fConfirm = atof((char*)cConfirm);	
	//printf("Get_U2 fConfirm=%f \n", fConfirm);

	return fConfirm;
}

//Request C2 
void	CtmTaskRS232::Request_C2(int	nValue, int nPrecision)		//fuxy, 2012-3-12 
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "008";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_C;	//'C';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%01d", nValue);
	SetPonit(cDataValue, nPrecision, 1);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("C cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1+1, &nWriteCount);	
}


//Request D2 
void	CtmTaskRS232::Request_D2(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00D";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_D;	//'D';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%06d", nValue);
	SetPonit(cDataValue, nPrecision, 6);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+6+1, &nWriteCount);
}

//Request F2 
void	CtmTaskRS232::Request_F2(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00D";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_F;	//'F';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%06d", nValue);
	SetPonit(cDataValue, nPrecision, 6);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+6+1, &nWriteCount);
}

//Request M2 
void	CtmTaskRS232::Request_M2(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "00D";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_M;	//'M';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%06d", nValue);
	SetPonit(cDataValue, nPrecision, 6);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+6+1, &nWriteCount);
}
/*
//Request S2 
void	CtmTaskRS232::Request_S2(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "008";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_S;	//'S';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%01d", nValue);
	SetPonit(cDataValue, nPrecision, 1);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+1+1, &nWriteCount);
}
*/
//Request U2 
void	CtmTaskRS232::Request_U2(int	nValue, int nPrecision)
{
	BYTE	cTempValue[50];
	BYTE	cDataLen[4] 	= "009";
	BYTE	cTaskID 		= MUCELL_COMM_TASKCODE_SEND_U;	//'U';
	BYTE	cDataValue[6]	= "00000";
	
	int		nWriteCount = 0;
	
	memset(cTempValue, 0, sizeof(cTempValue));
	
	sprintf((char*)cDataValue, "%02d", nValue);
	SetPonit(cDataValue, nPrecision, 2);
	//printf("cDataValue=%s \n", cDataValue);
	sprintf((char*)cTempValue, "%s%c%s", cDataLen, cTaskID, cDataValue);
	//printf("D cTempValue=%s \n", cTempValue);
	if(m_prs485 != NULL)
		m_prs485->WriteToken(cTempValue, 3+2+1, &nWriteCount);
}
/*=====================================================================================
//
//MuCell Task //JOYCE2010-12-16 
//=====================================================================================*/

void	CtmTaskRS232::WndProc(int message, WPARAM wParam, LPARAM lParam) 
{
	CTask::WndProc(message, wParam, lParam);

	switch (message)
	{
		case  MSG_SH_INET_SENDMOLDSET:		//JOYCE2011-11-9, for ŪiNet�Ҩ�Z,�Ҩ��ƶǥD��
			if(g_ptaskdsp != NULL)
    		{
    			g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSET); 
    			if (m_bDC)
    				g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSETB);
    		}
			break;
		case MSG_NB_HMI_SEND_SELVATGEND:
			{
   				// printf("message =%x\n",message);
		    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",0,FALSE);/*wangli2015-5-20 18:29:51 �M�� ���ҧC���פ��m��׬�2 ��flag*/
			}
		default:
			break;
	}
}

/*!	\fn void        CtmDsp54All::SendTransmit(void*	pInData, int nInputLen)
 *	\brief	��o�浧���
 *	\return 0
 */
void		CtmTaskRS232::SendTransmit(void*	pInData, int nInputLen)
{
	TRANSMITDATA	TranSmit;
	
	if(nInputLen > 0 && nInputLen <= COMM_MAXTRANSIMT)
	{
		memset(&TranSmit, 0, sizeof(TRANSMITDATA));
		memcpy(TranSmit.acBuffer, pInData, nInputLen);
		PushTransmitCmd(&TranSmit);
	}
}

/*!	\fn void        CtmDsp54All::SendTransmit(void*	pInData, int nInputLen)
 *	\brief	��o�浧���
 *	\return 0
 */
void		CtmTaskRS232::SendTransmitQueue(void*	pInData, int nInputLen)
{
	int         	nWriteCount;
	if(nInputLen > 0)
	{
		memset(g_frameTx.commdata.acBuffer, 0, COMM_MAXTRANSIMT);
		memcpy(g_frameTx.commdata.acBuffer, pInData, COMM_MAXTRANSIMT);
		//WORD	wCmd = *(WORD*)((BYTE*)pInData+4);
		//printf("SendTransmitQueue \n");
		g_frameTx.wCmd = COMM_SENDTRANSMITDSP28;
		WORD wLength = COMM_MAXTRANSIMT_RS232;//COMM_MAXTRANSIMT
		WriteToken((BYTE*)&g_frameTx, wLength+2, &nWriteCount);
		PopTransmitCount(1);
		m_bTransmitCmdEcho = TRUE;
	}
}


/*!	\fn void	CtmDsp54All::PushTransmitCmd(TRANSMITQUEUE* pTransmit)
 *	\brief		�N��o�����Jbuff
 *	\return 
 */
BOOL	CtmTaskRS232::PushTransmitCmd(TRANSMITDATA* pTransmit)
{
	//printf("m_Transmit.bQueueFull=%d m_Transmit.wInput=%d out=%d \n", m_Transmit.bQueueFull, m_Transmit.wInput, m_Transmit.wOutput);
	if (!m_Transmit.bQueueFull)
	{
		memcpy(m_Transmit.acBuffer[m_Transmit.wInput++], pTransmit->acBuffer, COMM_MAXTRANSIMT);
	    if (m_Transmit.wInput >= m_Transmit.wQueueEnd)
	        m_Transmit.wInput = 0;
	    if (m_Transmit.wInput == m_Transmit.wOutput)
	        m_Transmit.bQueueFull = TRUE;
	    //printf("m_Transmit.bQueueFull=%d  end\n", m_Transmit.bQueueFull);
	    return TRUE;
	}
	return FALSE;
}

/*!	\fn void	CtmDsp54All::PopTransmitCmd (TRANSMITQUEUE* pTransmit)
 *	\brief		�qbuff�����X���
 *	\return 
 */
BOOL	CtmTaskRS232::PopTransmitCmd (TRANSMITDATA* pTransmit)
{
	//printf("m_Transmit.wOutput=%d m_Transmit.wInput=%d m_Transmit.bQueueFull=%d \n", m_Transmit.wOutput, m_Transmit.wInput, m_Transmit.bQueueFull);
	if ((m_Transmit.wInput != m_Transmit.wOutput) || (m_Transmit.bQueueFull))
	{
		//James modify wOutPut++ --> wOutPut ; wati echo 
		memcpy(pTransmit->acBuffer, m_Transmit.acBuffer[m_Transmit.wOutput], COMM_MAXTRANSIMT);
	    if (m_Transmit.wOutput >= m_Transmit.wQueueEnd) m_Transmit.wOutput = 0;
	    m_Transmit.bQueueFull = FALSE;
	    return TRUE;
	}
	return FALSE;
}

/*!	\fn void	CtmDsp54All::PopTransmitCount (int	nCount)
 *	\brief		�btransmit buff�����ʤU�ЭӼ�
 *	\return 
 */
BOOL	CtmTaskRS232::PopTransmitCount (int	nCount)
{
	//printf("PopTransmitCount =%d \n", m_Transmit.wOutput);
	m_Transmit.wOutput += nCount;
	if (m_Transmit.wOutput >= m_Transmit.wQueueEnd) m_Transmit.wOutput = 0;
	m_Transmit.bQueueFull = FALSE;
	return TRUE;
}

void	CtmTaskRS232::ResValueFunc(void* pClass, char *pData, int nLength)
{
	int         i = 0, j = 0;
	int         nReadCount = 0;
	static	int nNum = 0;
	long	lDsp28CommState = 0;
	nNum ++;
	
	if  (m_pSelf->m_prs232 != NULL && m_pSelf->m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS)
    {
 			//printf("SYS_HMI_NULL_NULL_VIEWID=%d\n",  GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue);
   	//printf("nReadCount=%x \n", nReadCount);
    	if(nReadCount > 0)
    	{
		    i = 0;
		    g_wDspStatus = 1;
		    while ((g_commmap[i].wCmd != g_frameRx.wCmd) && (g_commmap[i].wCmd != COMM_NULL)) i++;
		    if (g_commmap[i].wCmd != COMM_NULL)
		    {
		    	m_pSelf->m_nCommandIndex = g_commmap[i].wCmd;
		        (m_pSelf->*(g_commmap[i].npFunc))();
		      	m_pSelf->wOldCMD = g_commmap[i].wCmd;  //ZHBA  2009-7-30 add

	      	//SendMsg(MSG_SH_HMI_UPDATE_VIEW, 0, 0, NULL);/*�e����s�B�z*/				
				//if (g_ptaskCmd->GetFocus() != NULL) 
				//	g_ptaskCmd->GetFocus()->Update();
			}
	   		for ( j=0; j<5; j++)
	        {
	        	if (m_pSelf->m_prs232->ReadToken((BYTE*)&g_frameRx, sizeof(g_frameRx), &nReadCount) == COM_SUCCESS) //St020101 for add update speed
	            {
	            	i = 0;
	            	while ((g_commmap[i].wCmd != g_frameRx.wCmd) && (g_commmap[i].wCmd != COMM_NULL)) i++;
	            	if (g_commmap[i].wCmd != COMM_NULL)
	                {
	                	m_pSelf->m_nCommandIndex = g_commmap[i].wCmd;
	                	(m_pSelf->*(g_commmap[i].npFunc))();
	                	m_pSelf->wOldCMD = g_commmap[i].wCmd;  //ZHBA  2009-7-30 add
				     // SendMsg(MSG_SH_HMI_UPDATE_VIEW, 0, 0, NULL);/*�e����s�B�z*/	
				        //if (g_ptaskCmd->GetFocus() != NULL) 
					    //g_ptaskCmd->GetFocus()->Update();
	                }
	            }
	        	else 
	            	break;
			}

			if(u_bManualKey == TRUE || g_frameRx.wCmd == COMM_GETWARMSTART)//JOYCE2011-2-11 �����W�c�������A�ӾɭP�q�T�ϼ��_�u
			  {
    				m_pSelf->m_lCounter = COMM_TIME_RESET*2;
    				if(g_frameRx.wCmd == COMM_GETOPSTATUS)
    				u_bManualKey = FALSE;
   			}
   		else
	    	m_pSelf->m_lCounter = COMM_TIME_RESET;
	    	g_wTaskDSP54	= TRUE;
	
	    	if ((bSpeechDelayOnOff) && (m_lCounterSpeechDelayOnOff < 0))        //wy120999 add for delay speech 100ms
	        {
		        bSpeechDelayOnOff = FALSE;
		        ::DeleteCounter((long *)&m_lCounterSpeechDelayOnOff, m_pSelf->m_idMe);
		        m_lCounterSpeechDelayOnOff = 100;
	        }
	    }

			if((1 == m_pSelf->m_bAustoneCfgFlag) || (1 == m_pSelf->m_bN2CFlag))		//��sAustone3q��T
			{
				if(g_wDspStatus == FALSE)		//James add for austone 2010/5/19
				{
					g_wDspStatus		= TRUE;
					m_pSelf->m_bTransmitCmdEcho 	= TRUE;
				}
				g_wDspStatus		= TRUE;
				lDsp28CommState 	= 1;
			}
			if((1 == m_pSelf->m_bAustoneCfgFlag) || (1 == m_pSelf->m_bN2CFlag))		
			{
				if(lDsp28CommState > 0)		
				{
					if(m_pSelf->PopTransmitCmd(&m_pSelf->m_TransmitBuffer))
					{
							    //printf("SendTransmitQueue \n");
						m_pSelf->SendTransmitQueue(&m_pSelf->m_TransmitBuffer, COMM_MAXTRANSIMT);
						m_pSelf->m_bTransmitCmdEcho = FALSE;
					}
					nNum = 0;
				}
					
				if(nNum >= 10)			
				{
					m_pSelf->m_bTransmitCmdEcho = TRUE;
				}
			}
   }
	else if (m_pSelf->m_lCounter < 0)
    {
	    m_pSelf->Reset();            //20070815 mark �ѨM�s�W�q�T�}����down���{�H   // Reset the port if the line is idle for 3 seconds
			g_wDspStatus = 0;
			m_pSelf->wSendConfStatus = 0;/*�q�T�_�u�Z,flag�M�s*/
	    m_pSelf->m_lCounter = COMM_TIME_RESET;

//	    if(g_wTaskDSP54	== TRUE)
//		   	SendMsg(MSG_SH_HMI_UPDATE_VIEW, 0, 0, NULL);	
	    //if(g_wTaskDSP54	== TRUE)
	    //{
		//	if (g_ptaskCmd->GetFocus() != NULL)
		//		g_ptaskCmd->GetFocus()->Update();	
		//}
    }
}

void	CtmTaskRS232::_Run(void *pData)/*�u�{��ƭ쫬*/
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		
	}
	//modify	2014/5/30 17:38:08
	while(1)
	{
		sem_wait(&m_pSelf->ThreadWait);/*�u�{�H���q�B�z�覡*/
		if(u_wThreadStartRunFlag)
		{
			pthread_mutex_lock(&g_setFlagLock);
			Run_Pthread(NULL,NULL,0);
			//ResValueFunc(NULL,NULL,0);
			u_wThreadStartRunFlag=FALSE;
			pthread_mutex_unlock(&g_setFlagLock);
		}
		//usleep(10000);/*�����u�{�H���q�B�z�覡*/
		
		
	}
}

BOOL     CtmTaskRS232::CheckDataID(char* pszData, char* pszData2[],int Length)//2009-9-30
{
	short       i = 0;		
	//printf("Length =%d\n",Length);
	for (i=0; i < Length; i++)
	{
	     if (strcmp(pszData, pszData2[i]) == 0) return TRUE;
	 }
	return FALSE;
}


#ifndef	D_BEOBJECT_CTMTASKRS232
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskdsp == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskHost*>(pResult))->CreateSelf();
		g_ptaskdsp = static_cast<CTaskHost*>(pResult);
	}
	return g_ptaskdsp;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskHost*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
