
#include	"tmDsp54All.h"
#include	"../../utils.h"
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
#include	"../../taskhost.h"
#include	"../libControl.h"
#include  "../../cblock.h"
#include	"../../tmshmsg.h"
#include	"../../tmnbmsg.h"
#include	"../../iotable.h"
#include	"../libDataSave.h"
#include 	"../../filerecord.h"
#include 	"../dsp28_m3/tmTaskDsp28_M3.h"

#define		DB_MACH1_CMD_ID				0x3330
#define		DB_MACH2_CMD_ID				0x3331
#define		DB_MOLDSET1_CMD_ID			0x3332
#define		DB_MOLDSET2_CMD_ID			0x3333
#define		DB_MONIMAX_CMD_ID			0x3530
#define		DB_POWERMATCH_CMD_ID		0x4243
#define		DB_MACHB_CMD_ID				0x3334
#define		DB_MOLDSETB1_CMD_ID			0x3335
#define		DB_MOLDSETB2_CMD_ID			0x3336
#define		DB_MONIMAXB_CMD_ID			0x3531
#define		DB_NEWPOWERMATCH_CMD_ID		0x4243
#define		DB_OTHER_CMD_ID				0x0030
#define		DB_CONFIG_CMD_ID			0x4241

#define		DB_MACH1_MAXINDEX_ID		0x200
#define		DB_MACH2_MAXINDEX_ID		0x400
#define		DB_MOLDSET1_MAXINDEX_ID		0x680
#define		DB_MOLDSET2_MAXINDEX_ID		0x900
#define		DB_POWERMATCH_MAXINDEX_ID	0xA00
#define		DB_MONIMAX_MAXINDEX_ID		0xA01
#define		DB_CONFIG_MAXINDEX_ID		0xD81
#define		DB_MACHB_MAXINDEX_ID		0x1100
#define		DB_MOLDSET1B_MAXINDEX_ID	0x1380
#define		DB_MOLDSET2B_MAXINDEX_ID	0x1400
#define		DB_MONIMAXB_MAXINDEX_ID		0x1460
#define		DB_NEWMACH2_MAXINDEX_ID   0x3200
#define		DB_NEWMACH3_MAXINDEX_ID   0x3800

#define		DB_MONINEWMAX_MAXINDEX_ID		0xAC1

//#define		COMM_ETH_SENDCONTROLNEWMACHINE1		0x3337
//#define		COMM_ETH_SENDCONTROLNEWMOLDSET1		0x3338
//#define		COMM_ETH_SENDCONTROLNEWMOLDSET2		0x3339

#define		DB_DSPUP_MACH_CMD_ID						0x0020
#define		DB_DSPUP_MOLDSET_CMD_ID					0x0021
#define		DB_DSPUP_MACHB_CMD_ID						0x0022
#define		DB_DSPUP_MOLDSETB_CMD_ID				0x0023
#define		DB_DSPUP_MACH2_CMD_ID						0x0024
#define		DB_DSPUP_MOLDSET2_CMD_ID				0x0025
#define		DB_DSPUP_MACH3_CMD_ID						0x0026
#define		DB_DSPUP_MOLDSET3_CMD_ID				0x0027

#define		BLOCK_FREECORE_ID			343
#define		BLOCK_NEWMACHINE2_ID			421
#define		BLOCK_NEWMOLDSET2_ID			422
#define		BLOCK_NEWMACHINE3_ID		441
#define		BLOCK_NEWMOLDSET3_ID		442

#define   LOWORD(l)           ((WORD)((DWORD)(l) & 0xFFFF)) //JOYCE2010-7-21 
#define   HIWORD(l)           ((WORD)((DWORD)(l) >> 16))    //JOYCE2010-7-21 
#define   UNWORD(hw,lw)       (((DWORD)(hw) << 16) | lw) 		//JOYCE2010-7-21 
#define 		MAX_PCTBL_NUM				  192
#define     MAX_PBTBL_NUM         128

#define     CURRENT_MOLD_MEDIA      "MHDR_MODULE_NULL_NULL_MODULEBYMEDIA"

IMPLEMENT_DYNCREATE(CtmDsp54All, CtmDsp54Protocol)
IMPLEMENT_DYNCREATE(CtmDsp54Protocol, CtmProtocol)

static      ETHNETFRAMETX	g_frameTx;
static      ETHNETFRAMERX	g_frameRx;
char**			g_apszVatTransID = NULL;

int     nValPBPoint = -1;

pthread_mutex_t			g_CommWriteMutex;									// 消息進程鎖
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
static	char*	g_apszSELVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	NULL
};

/*JOYCE2013-8-16 滑塊監控位置*/
static	char*	g_apszSlideVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL11DATA",     //20130816
	NULL
};

/*JOYCE2013-12-19 中子位置檢測*/
static	char*	g_apszCorePosnChkVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_COR_COR_NULL_W0597",                    /*中子位置檢測*/
	NULL
};

/*JOYCE2013-8-22 多次注射位置可設*/
static	char*	g_apszInjOpnPosnVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL1DATA",
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL2DATA",
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL3DATA",
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL4DATA",
	NULL
};

/*JOYCE2013-3-20 開模中途托模位置*/
static	char*	g_apszOpnMidEjtVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_MLD_OPEN_NULL_W075F",
	NULL
};

//JOYCE2011-5-10 特殊中子使用
static	char*	g_apszCSPVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL2DATA",       // 04D1 中子追蹤確認位置   //JOYCE2011-5-10 No.11-026
	NULL
};

/*JOYCE2012-2-7  No.12-007 夾具允許動作*/
static	char*	g_apszCAAVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL1DATA",       /* 08D0 夾具允許動作*/ //JOYCE2012-2-7
	NULL
};

/*JOYCE2012-4-11  D_AUPT 踏板*/
static	char*	g_apszAUPTVatTransID[] = {
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
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	"MSET_OTHR_OTHR_NULL_MOLDSPECIAL11DATA",       /* 08D0 踏板*/ //JOYCE2012-4-11
	NULL
};

static	char*	g_apszWSMVatTransID[] = {
	//"MSET_MLD_CLOSE_STEP1_POSITION",                       		// 01AA 關模一段終止位置
	//"MSET_MLD_CLOSE_STEP2_POSITION",                       		// 01AB 關模二段終止位置
	//"MSET_MLD_CLOSE_STEP3_POSITION",                       		// 01AC 關模三段終止位置
	//"MSET_MLD_CLOSE_PROTECT_POSITION",                 			// 01AD 關模低壓終止位置
	//"MSET_MLD_CLOSE_CLAMPUP_POSITION",                    		/* 008E 關模高壓終止位置           */  //Chuntzu 2004/7/16 add for Vrt
	//"MSET_CMPXMLD_CLOSE_CLAMPUP_POSITION",               		/* 0242 高壓鎖模位置               */  //Chuntzu 2004/7/16 add for Vrt
	//"MSET_CMPXMLD_OPEN_CLAMPDN_POSITION",                		/* 0245 高壓開模位置               */  //Chuntzu 2004/7/16 add for Vrt
	//"MSET_MLD_OPEN_STEP1_POSITION",                        		// 01CA 開模一段終止位置
	//"MSET_MLD_OPEN_STEP4_POSITION",                        		// 01CB 開模二段終止位置
	//"MSET_MLD_OPEN_STEP3_POSITION",                        		// 01CC 開模三段終止位置
	//"MSET_MLD_OPEN_STEP2_POSITION",                        		// 01CD 開模四段終止位置
	//"MSET_MLD_OPEN_STEP5_POSITION",                        		// 01CE 開模五段終止位置
	"MSET_MLD_OPEN_NULL_LINKPOSITION",							// 0322 中途托模位置         // St1111'02 add
	"MSETB_EJT2_EJT_NULL_W1178",                    // MOLD1 078   中途托模位置B   201212-13
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
	"MSET_SCRW_INJ_NULL_W06DD",                   // 02DD 抽真空啟動位置 <20101105M_01_D0> //JOYCE2010-11-8
	"MSET_PRODUCT_PRODUCT_NULL_W0700", 						// 0300 機械手中途下降位置 //JOYCE2011-2-10
	"MSET_COR_COR_NULL_W0595",                    /* 0195 退二動作位置  JOYCE2013-6-13*/
	NULL
};
/*MSET2 Zhongjw 2015-11-11 13:53:27*/
static	char*	g_apszVatTransMSET2ID[] = {
	"MSET2_BLST1_ON_START_POSITION",								// 000公模吹氣1動作位置
	"MSET2_BLST2_ON_START_POSITION",								// 008公模吹氣2動作位置
	"MSET2_BLST3_ON_START_POSITION",								// 010公模吹氣3動作位置
	"MSET2_MBLST1_ON_START_POSITION",								// 040母模吹氣1動作位置
	"MSET2_MBLST2_ON_START_POSITION",								// 048母模吹氣2動作位置
	"MSET2_MBLST3_ON_START_POSITION",								// 050母模吹氣3動作位置
	"MSET2_CORG_IN_NULL_SETMOLDPOSITION", 						// 0A4 中子G進動作位置
	"MSET2_CORG_OUT_NULL_SETMOLDPOSITION",						// 0B3 中子G退動作位置
	"MSET2_CORH_IN_NULL_SETMOLDPOSITION", 						// 0C4 中子H進動作位置
	"MSET2_CORH_OUT_NULL_SETMOLDPOSITION",						// 0D3 中子H退動作位置
	NULL
};

static	char*	g_apszVatTransMachID[] = {
	"MACH_MLD_OPEN_NULL_W0216"
};

static	char*   pDSPLub1[] = {
	"MACH_LUB_ON_NULL_SHOTCOUNT",                   //自動潤滑模數
	"MACH_ALARM_ON_NULL_ALARMLUBRICATTIME",         //潤滑警報時間
	"MACH_LUB_ON_NULL_DELAYTIME",                   //潤滑間隔時間
	"MACH_LUB_ON_NULL_COUNTER",                     //自動潤滑次數
	"MACH_LUB_ON_NULL_TIME",                        //自動潤滑保持計時
	NULL
};
static	char*   pDSPLub2[] = {
	"MACH_LUB2_ON_NULL_SHOTCOUNT",                 //自動潤滑模數
	"MACH_LUB_ON_NULL_W019E",                      //潤滑警報時間
	"MACH_SCRW_INJ_NULL_W0184",                    //潤滑間隔時間
	"MACH_SCRW_INJ_NULL_W0185",                    //自動潤滑次數
	"MACH_LUB2_ON_NULL_TIME",                      //自動潤滑保持計時
	NULL
};
static	char*   pDSPLub3[] = {
	"MACH_MTR_ON_NULL_W01D9",                      //自動潤滑模數
	"MACH_MTR_ON_NULL_W01D7",                      //潤滑警報時間
	"MACH_MTR_ON_NULL_W01DE",                      //潤滑間隔時間
	"MACH_MTR_ON_NULL_W01DF",                      //自動潤滑次數
	"MACH_MTR_ON_NULL_W01DC",                      //自動潤滑保持計時(1DC&1DD)
	NULL
};
static	char*   pHMILub1A[] = {
	"SYSX_OTHERS_OTHERS_INT_RESERVED25",           //自動潤滑模數
	"SYSX_OTHERS_OTHERS_INT_RESERVED26",           //潤滑警報時間
	"SYSX_OTHERS_OTHERS_INT_RESERVED27",           //潤滑間隔時間
	"SYSX_OTHERS_OTHERS_INT_RESERVED28",           //自動潤滑次數
	"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",         //自動潤滑保持計時
	NULL
};
static	char*   pHMILub2A[] = {
	"SYSX_OTHERS_OTHERS_INT_RESERVED36",           //自動潤滑模數
	"SYSX_OTHERS_OTHERS_INT_RESERVED37",           //潤滑警報時間
	"SYSX_OTHERS_OTHERS_INT_RESERVED38",           //潤滑間隔時間
	"SYSX_OTHERS_OTHERS_INT_RESERVED39",           //自動潤滑次數
	"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",         //自動潤滑保持計時
	NULL
};
static	char*   pHMILub3A[] = {
	"SYSX_OTHERS_OTHERS_INT_RESERVED47",           //自動潤滑模數
	"SYSX_OTHERS_OTHERS_INT_RESERVED48",           //潤滑警報時間
	"SYSX_OTHERS_OTHERS_INT_RESERVED49",           //潤滑間隔時間
	"SYSX_OTHERS_OTHERS_INT_RESERVED50",           //自動潤滑次數
	"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",         //自動潤滑保持計時
	NULL
};

int   pChkPCValue[] = {
	0x0300,         /*公模吹氣 #1  			*/
	0x0301,         /*公模吹氣 #2  			*/
	0x0302,         /*公模吹氣 #3  			*/
	0x0309,         /*母模吹氣 #1  			*/
	0x030A,         /*公模吹氣 #2  			*/
	0x030B,         /*公模吹氣 #3  			*/
	0x0000,         /*關模              */
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

////////////////////////////////////////////////////////////////////////////////////////////
const       COMMMAP g_commmap[] = {
	{COMM_ETH_GETCOLDSTART      , &CtmDsp54All::GetColdStart    },	//冷開機
	{COMM_ETH_GETWARMSTART      , &CtmDsp54All::GetWarmStart    },	//暖開機
	{COMM_ETH_GETREQUESTREAD	, &CtmDsp54All::GetReadData		},	//請求讀
	{COMM_ETH_GETUPDATEDATA		, &CtmDsp54All::GetUpdateData	},	//請求寫
	{COMM_ETH_GETCONFIG         , &CtmDsp54All::GetConfig       },	//組態資料
	{COMM_ETH_GETCONFIGLIMIT    , &CtmDsp54All::GetConfigLimit  },	//A/D & D/A上限值
	{COMM_ETH_GETDAOUTPUT       , &CtmDsp54All::GetDAOutput2    }, 	//DA曲線
	{COMM_ETH_GETOPSTATE        , &CtmDsp54All::GetOPState      },	//動作狀態
	{COMM_ETH_GETOPSTATUS       , &CtmDsp54All::GetOPStatus     },	//A/D & D/A顯示值
	{COMM_ETH_GETOPMETER        , &CtmDsp54All::GetCurveData    },	//射出曲線
	{COMM_ETH_GETAUSTIONDRIVEINFO , &CtmDsp54All::GetAustonDriveInfo    },	//Auston 資料 100H WORD
	{COMM_ETH_GETMONICURRENT    , &CtmDsp54All::GetMoniCurrent  },	//現模監測值
	{COMM_ETH_GETMONILAST       , &CtmDsp54All::GetMoniLast     },	//上模監測值
	{COMM_ETH_GETMONIMAXMIN    	, &CtmDsp54All::GetMoniMaxMin  	},	//監測上下限
	{COMM_ETH_GETMONICURRENTB   , &CtmDsp54All::GetMoniCurrentB },	//B座現模監測值
	{COMM_ETH_GETMONILASTB      , &CtmDsp54All::GetMoniLastB    },	//B座上模監測值
	{COMM_ETH_GETMONIMAXMINB    , &CtmDsp54All::GetMoniMaxMinB  },	//B座監測上下限
	{COMM_ETH_GETDIAGRMRI       , &CtmDsp54All::GetDiagRMRI     },	//DSP ROM&RAM資料
	// {COMM_ETH_GETDIAGRI         , &CtmDsp54All::GetDiagRI       },	//DSP INPUT資料

	{COMM_ETH_SENDPANEL				, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDPANELBREAK		, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMACHINE1	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMACHINE2	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMOLDSET1	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMOLDSET2	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMACHINEB	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMOLDSETB1	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLMOLDSETB2	, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCONTROLNEWMACHINE2, &CtmDsp54All::GetEcho},		//37  for freecore
	{COMM_ETH_SENDCONTROLNEWMOLDSET2_1, &CtmDsp54All::GetEcho},		//38  for freecore
	{COMM_ETH_SENDCONTROLNEWMOLDSET2_2, &CtmDsp54All::GetEcho},		//39
	{COMM_ETH_SENDCONTROLNEWMACHINE3, &CtmDsp54All::GetEcho},		//40  for machine3
	{COMM_ETH_SENDCONTROLNEWMOLDSET3_1, &CtmDsp54All::GetEcho},		//41  for moldset3_1
	{COMM_ETH_SENDCONTROLNEWMOLDSET3_2, &CtmDsp54All::GetEcho},		//42  for moldset3_2

	{COMM_ETH_SENDDIAGRMRI			, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDDIAGWM			, &CtmDsp54All::GetEcho},
	//{COMM_ETH_SENDDIAGSM			, &CtmDsp54All::GetEcho},
	//{COMM_ETH_SENDDIAGRI			, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDDIAGWO			, &CtmDsp54All::GetEcho},
	//{COMM_ETH_SENDDIAGSI			, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDASSIGNINPUT		, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDASSIGNOUTPUT		, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDMONILIMIT			, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDMONBILIMIT		, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDUPDATEDATA		, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDREQUESTREAD		, &CtmDsp54All::GetEcho},
	{COMM_SENDTRANSMITDSP28			, &CtmDsp54All::GetEcho},
	{COMM_ETH_SENDCOMMAND				, &CtmDsp54All::GetEcho},
	//{COMM_GETDAOUTPUT2      , &CtmDsp54All::GetDAOutput2    },
	//{COMM_GETOPMETERZEROAGAIN,&CtmDsp54All::GetOPMeterZeroAgain},
	//{COMM_GETOPTEMPERATURE  , &CtmDsp54All::GetOPTemperature},
	//{COMM_GETOPSUMMARY      , &CtmDsp54All::GetOPSummary    },
	//{COMM_GETCURVEDATA      , &CtmDsp54All::GetCurveData    },
	//{COMM_ETH_SENDCONTROLINDEX   , &CtmDsp54All::GetControlIndex },
	//{COMM_GETPRODUCTION     , &CtmDsp54All::GetProduction   },
	//{COMM_GETS_PROFILE01_I  , &CtmDsp54All::GetSProfile01I  },
	//{COMM_GETD_PROFILE01_I  , &CtmDsp54All::GetDProfile01I  },
	//{COMM_GETE_PROFILE01_I  , &CtmDsp54All::GetEProfile01I  },
	//{COMM_GETS_PROFILE01_H  , &CtmDsp54All::GetSProfile01H  },
	//{COMM_GETD_PROFILE01_H  , &CtmDsp54All::GetDProfile01H  },
	//{COMM_GETE_PROFILE01_H  , &CtmDsp54All::GetEProfile01H  },
	//{COMM_GETS_BPROFILE01_I , &CtmDsp54All::GetSBProfile01I },
	//{COMM_GETD_BPROFILE01_I , &CtmDsp54All::GetDBProfile01I },
	//{COMM_GETE_BPROFILE01_I , &CtmDsp54All::GetEBProfile01I },
	//{COMM_GETS_BPROFILE01_H , &CtmDsp54All::GetSBProfile01H },
	//{COMM_GETD_BPROFILE01_H , &CtmDsp54All::GetDBProfile01H },
	//{COMM_GETE_BPROFILE01_H , &CtmDsp54All::GetEBProfile01H },
	//{COMM_SENDPANELBREAK    , &CtmDsp54All::GetEcho         },
	//{COMM_SENDCONTROLMACHINE, &CtmDsp54All::GetEcho         },
	//{COMM_SENDCONTROLMOLDSET, &CtmDsp54All::GetEcho         },
	//{COMM_SENDCONTROLMONITOR, &CtmDsp54All::GetEcho         },
	//{COMM_SENDPOWERMATCH    , &CtmDsp54All::GetEcho         },
	//{COMM_SENDPOWERMATCH2   , &CtmDsp54All::GetEcho		    },
	//{COMM_SENDCONTROLMACHINEB,&CtmDsp54All::GetEcho         },
	//{COMM_SENDCONTROLMOLDSETB,&CtmDsp54All::GetEcho         },
	//{COMM_SENDCONTROLMONITORB,&CtmDsp54All::GetEcho         },
	//{COMM_SENDSTATUS        , &CtmDsp54All::GetEcho         },
	//{COMM_SENDR_PROFILE01   , &CtmDsp54All::GetEcho         },
	//{COMM_SENDS_PROFILE01   , &CtmDsp54All::GetEcho         },
	//{COMM_SENDCOMMAND       , &CtmDsp54All::GetEcho         },
	{COMM_NULL              , NULL                          }
};

long 		g_nWriteM3MoldPosDBID[] =
{
	0x0300FC202, 0x0300FC209,
	0x0300FC210, 0x0300FC217,
	0x0300FC21E, 0x0300FC302,
	0x0300FC309, 0x0300FC310,
	0x0300FC307, 0x0300FC31E,
};
long		g_nReadM3MoldPosDBID[] =
{
	0x300F1900,
};


WORD		dbgppc[tmMAX_GROUP_PC];
long		g_lCounterBootTime	= 0;
static		int			u_nMicroAdjCount = 0;					//LEO20070621
static		int			u_nAutoAdjCount = 0;					//LEO20070621
static      BOOL        g_bLEDAdjMold = FALSE;
static      BOOL        g_bLEDMotor = FALSE;
static      long        g_lCounterLEDAdjMold;
extern	WORD			g_bMsgFormKey ;	//Caozz 2010/12/9 下午 03:01:09

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmDsp54Protocol::CtmDsp54Protocol()
{
	m_nStartPos	    = 0;
	m_nPackSizePos	= 2;
	m_nTypePos		= 4;
	m_nCMDPos	    = 6;
	m_nIDPos		= 8;
	m_nDataLenPos   = 10;

	m_nCRCPos       = 0;
	m_nEndPos	    = 2;
}


CtmDsp54Protocol::~CtmDsp54Protocol()
{

}

int		CtmDsp54Protocol::InitSend()
{
	m_SendHeader.SetMaxSize(12);
	m_SendHeader.SetSize(12);
	m_SendFooter.SetMaxSize(4);
	m_SendFooter.SetSize(4);

	WORD	wTemp;
	wTemp	= 0x0002;
	m_SendHeader.SetPartValue(m_nStartPos, &wTemp, sizeof(wTemp));
	wTemp   = m_nSendType;
	m_SendHeader.SetPartValue(m_nTypePos, &wTemp, sizeof(wTemp));
	wTemp	= 0x0003;
	m_SendFooter.SetPartValue(m_nEndPos, &wTemp, sizeof(wTemp));
	return 0;
}

int		CtmDsp54Protocol::InitRev()
{
	m_RevHeader.SetMaxSize(12);		//	the Rev header
	m_RevHeader.SetSize(12);		//	the Rev header

	m_RevFooter.SetMaxSize(4);
	m_RevFooter.SetSize(4);

	WORD wTemp   = m_nRevType;
	m_SendHeader.SetPartValue(m_nTypePos, &wTemp, sizeof(wTemp));
	return 0;
}

CtmDsp54All::CtmDsp54All()
{
	m_nMaxLen			= 1490 * 2;
	m_cState   = 0;
	m_cStateRx = 0;
	m_cStateTx = 0;
	m_bGotConfig = FALSE;
	m_bGotDA = FALSE;
	m_bNewDA = FALSE;
	m_bGetPanlEcho = TRUE;
	m_wCmdState = CMD_NONE;
	m_CmdQueue.wInput     	= m_CmdQueue.wOutput = 0;
	m_CmdQueue.wQueueEnd  	= MAX_COMMAND_KEEP;
	m_CmdQueue.bQueueFull 	= FALSE;

	m_CmdEchoQueue.wInput     = m_CmdEchoQueue.wOutput = 0;
	m_CmdEchoQueue.wQueueEnd  = MAX_COMMAND_KEEP;
	m_CmdEchoQueue.bQueueFull = FALSE;

	m_bTransmitGetDataFlag = FALSE;

	for (int i = 0; i < MAX_TRANSMIT_KEEP; i++)
	{
		m_Transmit.acBuffer[i] = new unsigned char[COMM_MAXTRANSIMT_NET];
	}

	dbbuff.flMotorStatus = 0xFF;
	wMoldHeaterStatusOld = 0xFF;
	m_Transmit.wInput     	= m_Transmit.wOutput = 0;
	m_Transmit.wQueueEnd  	= MAX_TRANSMIT_KEEP;
	m_Transmit.bQueueFull 	= FALSE;
	m_bSysReset           	= GetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION").lValue;
	m_nCommandIndex		  	= -1;
	m_nSendRM			  	= 0;
	m_nRevRM			  	= 0;
	m_bSendRProfile01  		= FALSE;
	m_bSendRBProfile01 		= FALSE;
	m_bTransmitCmdEcho	  	= TRUE;
	m_bSendDataCounter		= 0;
	m_wGetDataCounter		= 0;
	memset(&m_CmdEchoData, 		0, 	sizeof(m_CmdEchoData));
	memset(&m_CmddData, 		0, 	sizeof(m_CmddData));
	memset(&m_CmdPanelBreak, 	0, 	sizeof(m_CmdPanelBreak));
	memset(&m_CmdDSPUpData, 	0, 	sizeof(m_CmdDSPUpData));

	m_lGetTransCount		= 0;
	m_lSendTransCount		= 0;
	m_nNum				= 0;
	pthread_mutex_init(&g_CommWriteMutex, NULL);
	pthread_mutex_init(&m_cache_lock, 0);
	for (int i = 0; i < 6; i++)
	{
		m_dwInjectTime[i] = 0;
	}

	for (int i = 0; i < 7; i++)
	{
		m_nAirPCPoint[i]  = -1;
	}
	SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS"	, 	long(0));			//	自動監測

	m_bColdStartFlag = FALSE;/*JOYCE2013-9-12*/

	m_bOLDLUB = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70").lValue;//0-NEW LUB 1-OLD LUB
	m_bAmmeter 	= CtmConfig::GetInstance()->GetAmmeterFlag();
	m_bDC     = CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC;
	m_bALogin = g_pExTable->GetTableValue(0, "D_ALOGIN");
	m_bStepLub = g_pExTable->GetTableValue(0, "D_STEPLUB"); /*JOYCE2012-7-12 No.12-028 遞進式潤滑*/
	m_bCOM2Vlv = g_pExTable->GetTableValue(0, "D_GT16") || g_pExTable->GetTableValue(0, "D_GT10");
	m_bInjSpdUnit = g_pExTable->GetTableValue(0, "D_INJSPDUNIT");
	m_bSVRT = g_pExTable->GetTableValue(0, "D_SVRT"); /*純二板*/
	bINJBlow = g_pExTable->GetTableValue(0, "D_INJBLOW"); /*zhongjw 2015-7-2 10:35:58 注吹機*/
	m_b28Clmp = g_pExTable->GetTableValue(0, "D_28CLAMP"); /*28控制開關模*/

	if ((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 2) || (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 3))
		m_b3LUB = TRUE;

	if (g_pExTable->GetTableValue(0, "D_WSMVAT"))
		g_apszVatTransID = g_apszWSMVatTransID;
	else  if (g_pExTable->GetTableValue(0, "D_SLIDE")) /*JOYCE2013-8-16 滑塊監控位置*/
		g_apszVatTransID = g_apszSlideVatTransID;
	else	if (g_pExTable->GetTableValue(0, "D_CSP")) //JOYCE2011-5-10 No.11-026
		g_apszVatTransID = g_apszCSPVatTransID;
	else	if (g_pExTable->GetTableValue(0, "D_CLPALLOW")) /*JOYCE2012-2-7  No.12-007 夾具允許動作*/
		g_apszVatTransID = g_apszCAAVatTransID;
	else	if (g_pExTable->GetTableValue(0, "D_AUPT")) /*JOYCE2012-4-11  踏板*/
		g_apszVatTransID = g_apszAUPTVatTransID;
	//JOYCE2013-6-13 mark else	if (g_pExTable->GetTableValue(0,"D_CORERET2"))/*JOYCE2012-7-19 中子退二*/
	//JOYCE2013-6-13 mark		g_apszVatTransID = g_apszCRET2VatTransID;
	else	if (g_pExTable->GetTableValue(0, "D_OPNMIDEJT")) /*JOYCE2013-3-20 開模中途托模位置*/
		g_apszVatTransID = g_apszOpnMidEjtVatTransID;
	else	if (g_pExTable->GetTableValue(0, "D_INJMP")) /*JOYCE2013-8-22 多次注射位置可設*/
		g_apszVatTransID = g_apszInjOpnPosnVatTransID;
	else	if (g_pExTable->GetTableValue(0, "D_COREPOSNCHK")) /*JOYCE2013-12-19 中子位置檢測*/
		g_apszVatTransID = g_apszCorePosnChkVatTransID;
	else
		g_apszVatTransID = g_apszSELVatTransID;

	g_lCounterBootTime = GetDBValue("MACH_OTHR_OTHR_NULL_W01CE").lValue * 10;
	AddCounter(&g_lCounterBootTime, m_idMe);
}

CtmDsp54All::~CtmDsp54All()
{
	for (int i = 0; i < MAX_TRANSMIT_KEEP; i++)
	{
		delete [] m_Transmit.acBuffer[i];
	}
	DeleteCounter(&g_lCounterBootTime, m_idMe);
}


int		CtmDsp54All::CreateSelf()
{
	CtmProtocol::CreateSelf();
	m_pData		= new BYTE[m_nMaxLen];
	m_nSendType	= 0x0000;
	m_nRevType	= 0x0000;

	InitSend();
	InitRev();

	return 0;
}

int		CtmDsp54All::FreeSelf()
{
	CtmProtocol::FreeSelf();
	delete [] m_pData;
	pthread_mutex_destroy(&g_CommWriteMutex);
	pthread_mutex_destroy(&m_cache_lock);
	return 0;
}

/*!	\fn int	CtmDsp54All::SaveDataToDB(void* buf, int nLen, WORD wFirstID)
 *	\brief	保存主機發送過來的更新數據
 *	\param	[void*]	buf			數據內容
 *	\param	[in]	nLen		數據長度
 *	\param	[WORD]	wFirstID	數據ID
 *	\return 0
 */
BOOL     CtmDsp54All::SaveDataToDB(void* buf, int nLen, WORD wFirstID)
{
	int			nResult 	= 0;
	int 		nTempIndex 	= -1;
	long long 	dwDBValue 	= 0, lRealOpenShotCount = 0;
	char 		*pData 		= (char *)buf;
	DBVALUE		data;
	DWORD	    wValue 		= 0, wDataID = wFirstID;

	while (nResult < nLen)
	{
		nTempIndex 	= 	g_pDatabase->DataIDToIndex(wFirstID++);
		if (nTempIndex >= 0)
		{
			data 		= 	g_pDatabase->Read(nTempIndex);
			char psz[256];
			strcpy(psz, g_pDatabase->GetString(nTempIndex));
			switch (data.DataType.wType)
			{
			case TYPE_BYTE:
				memcpy(&wValue, pData, sizeof(BYTE));
				break;
			case TYPE_WORD:
				memcpy(&wValue, pData, sizeof(WORD));
				break;
			case TYPE_DWORD:
				wFirstID++;
				memcpy(&wValue, pData, sizeof(DWORD));
				break;
			case TYPE_UNKNOWN:
			default:
				return FALSE;
				data.dwState	= DB_ERR_TYPE;
			}
			//printf("data.DataType.wType=%d nTempIndex=%d wFirstID=%d psz=%s value=%d\n", data.DataType.wType, nTempIndex, wFirstID, psz, wValue);
			g_pDatabase->Write(nTempIndex, &wValue);
			if (wDataID == 0x01A4)	//此數據為開模總數
			{
				if (wValue > GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue)
					lRealOpenShotCount = wValue - GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
				g_pDatabase->Write("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", &wValue);

				//SetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", dwDBValue+lRealOpenShotCount, FALSE);
				dwDBValue = GetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT").lValue;
				if (CtmConfig::GetInstance()->GetINETFlag() == 1)
				{
					DWORD	dwUNIT = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
					SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + lRealOpenShotCount * dwUNIT, FALSE);
				}
				else
					SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", dwDBValue + lRealOpenShotCount, FALSE);
				//g_tmDebugInfo->PrintDebugInfo("dwDataID:dbbuff.bSwitchToAuto:%d ,AutoShutCount=%d MachAlarmShotcount=%ld\n", dbbuff.bSwitchToAuto,dbbuff.wSwitchToAutoShutCount, GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue);

				//JOYCE2010-10-12 累積運行模數
				DWORD dwTotalShotCount	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED36").lValue, GetDBValue("SYS_HMI_NULL_NULL_RESERVED35").lValue); /*JOYCE2014-5-21 解決累積運行模數不對bug, MARK UNWORD(HSBCount,LSBCount);*/
				dwTotalShotCount += lRealOpenShotCount;
				if (dwTotalShotCount > 999999999)	dwTotalShotCount = 0;
				SetDBValue("SYS_HMI_NULL_NULL_RESERVED35", LOWORD(dwTotalShotCount), FALSE);
				SetDBValue("SYS_HMI_NULL_NULL_RESERVED36", HIWORD(dwTotalShotCount), FALSE);

				if (m_bSVRT)
				{
					dwTotalShotCount = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED113").lValue;
					dwTotalShotCount += 1;
					if (dwTotalShotCount > 1000)
						dwTotalShotCount = 1000;
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED113", dwTotalShotCount, FALSE);

					SaveOpnCntData();
				}

				//JOYCE2011-9-30  累積運行模數 for lubricate
				if ((!m_bOLDLUB) && (!m_bStepLub))
				{
					dwTotalShotCount	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;
					dwTotalShotCount += lRealOpenShotCount;
					if (dwTotalShotCount > 999999999)	dwTotalShotCount = 0;
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27", dwTotalShotCount, FALSE);
				}

				if (g_MoniUpdate.bChange)           //St1128'01 for moni lose problem
				{
					dwDBValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
					g_ptmControlServer->MoniControl()->AddMoni2();
					g_MoniUpdate.dwShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
					g_MoniUpdate.bChange = FALSE;
				}

				Echo(COMM_ECHO_SUCCESS);
				if (dbbuff.bSwitchToAuto && (GetDBValue("STATE_ALARM_ONOFF_NULL_STATUS").lValue == 0)
				        && (GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue != 0))  //Chuntzu 2004/9/15 add to C54
				{
					if (++dbbuff.wSwitchToAutoShutCount >= GetDBValue("MACH_ALARM_ON_NULL_SHOTCOUNT").lValue)
					{
						dbbuff.bSwitchToAuto = FALSE;
						//g_tmDebugInfo->PrintDebugInfo("1343 =%d\n", dbbuff.bSwitchToAuto);
						SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 1, FALSE);
						SetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED14", 1, FALSE);/*JOYCE20150225 FLAG,MONI LastValue-->ReferValue*/

						//fans add
						CtmQualityControl::GetInstance()->SetData(MONI_DATATYPE_LIMITUP_ID, 0);
						CtmQualityControl::GetInstance()->SetData(MONI_DATATYPE_LIMITLOWER_ID, 0);
						//MonitorSet();
						SendControlMonitor();
						//#ifdef  D_DC   //20061122  add!
						if (m_bDC) //James add 2008/8/13 09:42上午
						{
							CtmQualityControl::GetInstance()->SetData(12, 0);	//fuxy,2009-1-12
							CtmQualityControl::GetInstance()->SetData(13, 0);
							SendControlMonitorB();
						}
					}
				}

				SendMsg(MSG_DSP54_GET_SHOTCOUNT, 0, 0, NULL);

				if (m_bAmmeter) //現在產品數累計
				{
					int 	NumPerUnit 	= GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
					DWORD	dwCurrentProduct 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED21").lValue, GetDBValue("SYS_HMI_NULL_NULL_RESERVED20").lValue);
					dwCurrentProduct  += lRealOpenShotCount * NumPerUnit;
					SetDBValue("SYS_HMI_NULL_NULL_RESERVED20", LOWORD(dwCurrentProduct), FALSE);
					SetDBValue("SYS_HMI_NULL_NULL_RESERVED21", HIWORD(dwCurrentProduct), FALSE);
				}
			}

			if ((wDataID == 0x0028) && (wValue == 0))
			{
				tmDATE  date;
				long		count = 0;
				int			Py = 360, Pm = 30; //系數
				GetDate(&date);
				count	= Py * (date.year % 2000) + Pm * (date.month - 1) + date.day;
				SetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION", count);

				/*JOYCE2012-3-9 新潤滑功能資料,調模潤滑累計天數 處理*/
				if ((!m_bOLDLUB) && (!m_bStepLub))
				{
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54", 0);
					SaveLubricate();
				}
			}

			nResult 	+= 	data.DataType.wLength;
		}
		else
		{
			nResult 	+= 	sizeof(WORD);
		}
	}
	return (nResult == nLen);
}

BOOL    CtmDsp54All::DataIsValid(void* pInData, int nInputLen)
{
	WORD wTemp = 0;
	WORD wCRC = 0;
	WORD * pwTemp = NULL;

	memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
	memcpy(m_RevFooter.GetBuffer(),
	       (char *)pInData + (nInputLen - m_RevFooter.GetSize()), m_RevFooter.GetSize());
	//check 數據包長度是否正確
	m_RevHeader.GetPartValue(m_nPackSizePos, &wTemp, sizeof(WORD));
	if (wTemp << 1 != nInputLen)
	{
		cout << "PackSize is wrong, right is : " << nInputLen / 2 << endl;
		goto ERROR;
	}
	//check 傳送資料WORD總數是否正確
	m_RevHeader.GetPartValue(m_nDataLenPos, &wTemp, sizeof(WORD));
	if (wTemp << 1 != nInputLen - m_RevHeader.GetSize() - m_RevFooter.GetSize())
	{
		cout << "DataSize is wrong, right is : " << (nInputLen - m_RevHeader.GetSize() - m_RevFooter.GetSize()) / 2 << endl;
		goto ERROR;
	}
	//check CRC校驗是否正確
	m_RevFooter.GetPartValue(m_nCRCPos, &wTemp, sizeof(WORD));
	pwTemp = (WORD *)pInData + 1;
	for (int i = 0; i < (nInputLen - 6) >> 1; i++)
	{
		wCRC ^= *pwTemp++;
	}
	if (wCRC != wTemp)
	{
		cout << "CRC is wrong, right is : " << wCRC << endl;
		goto ERROR;
	}
	return TRUE;
ERROR:
	cout << "Data is not valid" << endl;
	return FALSE;
}

int		CtmDsp54All::ComposeProtocol(int nCmd, void* pOutData,	void* pInData, int nInputLen, void* pEx)
{

	int nResult = 0, nRealCmd = 0;
	DIAGWO		diagwo;
	//DIAGRI	Diagri;
	DIAGWM		diagwm;
	DIAGRMRI	diagrmri;

	if ( nCmd < 0xFFFF)
	{
		nRealCmd = GetCmdSwitch(nCmd);
	}
	else
	{
		nRealCmd = nCmd & 0xFFFF;
	}
	//printf("CtmDsp54All::ComposeProtocol nCmd=%x nRealCmd=%x\n", nCmd, nRealCmd);
	if (nRealCmd == 0)
		nRealCmd = nCmd;
	switch (nRealCmd)
	{
	case COMM_ETH_SENDREQUESTREAD:		// 12 MMI 請求讀
		SendRequestRead(*(WORD*)pInData);
		break;
	case COMM_ETH_SENDUPDATEDATA:		// 13 MMI 請求寫
		//printf("nInputLen=%d pInData=%d \n", nInputLen, *(int*)pInData);
		if (pInData != NULL)
		{
			if (nInputLen != 0)		//如果為0，則為字串ID，否則為數字ID
				SendControlIndex(*(int*)pInData);
			else
				SendControlIndex((char*)pInData);
		}
		//SendUpdateData(*(ETHNETFUNCTX*)pInData);
		break;
	case COMM_ETH_SENDPANEL:			// 2' ' 按鍵傳輸
		SendPanel(LOBYTE(*(int*)pInData));
		break;
	case COMM_ETH_SENDCONTROLMACHINE:
		SendControlMachine(1);
		SendControlMachine(2);
		break;
	case COMM_ETH_SENDCONTROLMACHINE1:	//30 上段參數資料
		SendControlMachine(1);
		break;
	case COMM_ETH_SENDCONTROLMACHINE2:	//31 下段參數資料
		SendControlMachine(2);
		break;
	case COMM_ETH_SENDCONTROLMOLDSET:
		SendControlMoldset(1);
		SendControlMoldset(2);
		break;
	case COMM_ETH_SENDCONTROLMOLDSET1:	//32 上段模組資料
		SendControlMoldset(1);
		break;
	case COMM_ETH_SENDCONTROLMOLDSET2:	//33 下段模組資料
		SendControlMoldset(2);
		break;
	case COMM_ETH_SENDCONTROLMACHINEB:	//34 B座參數資料
		SendControlMachineB();
		break;
	case COMM_ETH_SENDCONTROLMOLDSETB:
		SendControlMoldsetB(1);
		SendControlMoldsetB(2);
		break;
	case COMM_ETH_SENDCONTROLMOLDSETB1: //35 B座上段模組資料
		SendControlMoldsetB(1);
		break;
	case COMM_ETH_SENDCONTROLMOLDSETB2: //36 B座下段模組資料
		SendControlMoldsetB(2);
		break;
	case COMM_ETH_SENDCONTROLNEWMACHINE2:	//37 newmachine2
		SendControlNewMachine2(1);
		break;
	case COMM_ETH_SENDCONTROLNEWMOLDSET2:
		SendControlNewMoldset2(1);
		SendControlNewMoldset2(2);
		break;
	case COMM_ETH_SENDCONTROLNEWMOLDSET2_1:	//38 newmoldset2_1
		SendControlNewMoldset2(1);
		break;
	case COMM_ETH_SENDCONTROLNEWMOLDSET2_2:	//39 newmoldset2_2
		SendControlNewMoldset2(2);
		break;
	case COMM_ETH_SENDCONTROLNEWMACHINE3:	//40 newmachine3
		SendControlNewMachine3(1);
		break;
	case COMM_ETH_SENDCONTROLNEWMOLDSET3:
		SendControlNewMoldset3(1);
		SendControlNewMoldset3(2);
		break;
	case COMM_ETH_SENDCONTROLNEWMOLDSET3_1:	//41 newmoldset3_1
		SendControlNewMoldset3(1);
		break;
	case COMM_ETH_SENDCONTROLNEWMOLDSET3_2:	//42 newmoldset3_2
		SendControlNewMoldset3(2);
		break;
	case COMM_ETH_SENDDIAGRMRI:			//40 讀取DSP ROM&RAM + INPUT
		if (pInData != NULL)
		{
			if (nInputLen == 0)
				memcpy(&diagrmri, pInData , sizeof(diagrmri));
			else
				memcpy(&diagrmri, pInData , nInputLen);
			return SendDiagRMRI(diagrmri);
		}
		break;
	case COMM_ETH_SENDDIAGWM:			//41 寫入DSP RAM
		if (pInData != NULL)
		{
			if (nInputLen == 0)
				memcpy(&diagwm, pInData , sizeof(diagwm));
			else
				memcpy(&diagwm, pInData , nInputLen);
			SendDiagWM(diagwm);
		}
		break;
	case COMM_ETH_SENDDIAGWO:			//44 寫入DSP OUTPUT
		if (pInData != NULL)
		{
			if (nInputLen == 2)
				SendDiagWO(*(int*)pInData, *((int*)pInData + 1));
			else
			{
				if (nInputLen == 0)
					memcpy(&diagwo, pInData , sizeof(diagwo));
				else
					memcpy(&diagwo, pInData , nInputLen);
				SendDiagWO(diagwo);
			}
		}
		break;
	case COMM_ETH_SENDASSIGNINPUT:		//46 對換 DSP INPUT
		SendAssignInput(*(int*)pInData, *((int*)pInData + 1));
		break;
	case COMM_ETH_SENDASSIGNOUTPUT:		//47 對換 DSP OUTPUT
		SendAssignOutput(*(int*)pInData, *((int*)pInData + 1));
		break;
	case COMM_ETH_SENDMONILIMIT:		//50 監測值上下限
		SendControlMonitor();
		break;
	case COMM_ETH_SENDMONBILIMIT:		//51 B座監測值上下限
		SendControlMonitorB();
		break;
	case COMM_ETH_SENDCONNET:			//發送連線使用，因為dsp54開機不能主動發arp包
		SendConnect();
		break;
	case COMM_ETH_SENDCONFIG:
		SendConfig();
		break;
	case COMM_ETH_SENDSTATUS:
		SendStatus(*(WORD*)pInData);
		break;
	case COMM_GETSTATERX:
		memcpy(pOutData, &m_cStateRx, sizeof(m_cStateRx));
		break;
	case COMM_GETSTATE:
		memcpy(pOutData, &m_cState, sizeof(m_cState));
		break;
	case COMM_BGOTCONFIG:
		memcpy(pOutData, &m_bGotConfig, sizeof(m_bGotConfig));
		break;
	case COMM_ETH_GETDIAGRMRI:
		if (nInputLen == 0)
			memcpy(pOutData, GetDBDiagRMRI() , sizeof(tmDB_DIAGRMRI));
		else
			memcpy(pOutData, GetDBDiagRMRI() , nInputLen);
		break;
	case COMM_GETDAFLAG:
		memcpy(pOutData, &m_bNewDA , sizeof(m_bNewDA));
		break;
	case COMM_GETCOMMAND:
		memcpy(pOutData, &m_nCommandIndex , sizeof(m_nCommandIndex));
		break;
	case COMM_INIT_MESSAGE:
		InitMessage();
		break;
	case COMM_SENDTRANSMITDSP28:
		if (pInData != NULL)
		{
			//printf("=====SendTransmit nLen=%d \n", nInputLen);
			long	lDsp28CommState 	= GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;
			if (lDsp28CommState > 0)
				SendTransmit(pInData, nInputLen);
			//if(m_bTransmitGetDataFlag == TRUE)			//James add 2010/5/7 加快回复數據速度
			//{
			//	if(PopTransmitCmd(&m_TransmitBuffer))
			//	{
			//		SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT_NET);
			//		m_bTransmitCmdEcho = FALSE;
			//	}
			//	m_bTransmitGetDataFlag = FALSE;
			//}
		}
		break;
	case COMM_SENDTRANSMITCMD:
	{
		if (pInData != NULL)
		{
			WORD	wCmd = *(WORD*)pInData;
			switch (wCmd)
			{
			case 0x00:			//case 0 : reset buff
				m_Transmit.wInput     	= m_Transmit.wOutput = 0;
				m_Transmit.wQueueEnd  	= MAX_TRANSMIT_KEEP;
				m_Transmit.bQueueFull 	= FALSE;
				break;
			default:
				break;
			}
		}
	}
	break;
	case COMM_SENDTRANSMITBLOCK:

		break;
//		case 0x6503:    //ZHBA  2008-10-31 for 7CM injetime
//			{
//			DWORD  dInjTime[6];
//			for(int i=0;i<6;i++)
//			dInjTime[i] = GetInjectTime(i);
//			if(nSize == 0)
//				memcpy(pOutData, &dInjTime[0] ,sizeof(DWORD));
//			else
//				memcpy(pOutData, &dInjTime[0] ,nSize);
//			}
//			break;
	case COMM_GETAIRPCPOINT:    /*joyce212-12-19 PC point,0x6600*/
		if (nInputLen == 0)
			memcpy(pOutData, &m_nAirPCPoint[0] , sizeof(int));
		else
			memcpy(pOutData, &m_nAirPCPoint[0] , nInputLen);
		break;
	case COMM_GETAIRPCDATA:    /*joyce212-12-19 PC channel,0x6601*/
		if (nInputLen == 0)
			memcpy(pOutData, &m_AirPCDataID[0] , sizeof(char));
		else
			memcpy(pOutData, &m_AirPCDataID[0] , nInputLen);
		break;
	case	COMM_BOOTTIMECNTDOWN:  /*JOYCE20150127  BOOT WAIT TIME COUNT DOWN*/
		g_lCounterBootTime = GetDBValue("MACH_OTHR_OTHR_NULL_W01CE").lValue * 10;
		break;
	case 0x7771:		//Reset buff

		m_Transmit.wInput     	= m_Transmit.wOutput = 0;
		m_Transmit.wQueueEnd  	= MAX_TRANSMIT_KEEP;
		m_Transmit.bQueueFull 	= FALSE;

		break;
	case 0xFF01:   /*JOYCE2014-10-17 增加run(), 方便功能處理*/
		Run();
		break;
	default:
		break;
	}

	return nResult;
}

void	CtmDsp54All::Run()  /*JOYCE2014-10-17 增加run(), 方便功能處理*/
{
	if ((!m_bOLDLUB) && (!m_bStepLub))
		ProcessLubricate(); //JOYCE2011-8-23 lubricate

	if (!m_bPowerOnLub) /*JOYCE2012-3-20 開機自動潤滑天數*/
	{
		SetPowerOnLubDefault();
		m_bPowerOnLub = TRUE;
	}
	if (!m_bPowerOnLubHr) /*wangli2015-12-17 9:57:21 非開關模狀態 累計時間*/
	{
		ProsNotclmpTime();
		m_bPowerOnLubHr = TRUE;
	}

	if (m_bALogin) /*JOYCE2012-6-18 切換到全自動狀態時,需要輸入密碼確認*/
	{
		if (m_bSendAutoKey)
		{
			m_bSendAutoKey = FALSE;
			SendPanel(0xFF);
		}
	}

	BootTimeCntDown();   /*JOYCE20150127  BOOT WAIT TIME COUNT DOWN*/
}

BOOL CtmDsp54All::DoDealFunc(WORD wCmd)
{
	int i = 0;
	//long lValue= GetDBValue("ULMTV_DP_MDC_N_FL0").lValue;
	//printf("g_commmap[i].wCmd=%x lValue=%d \n", wCmd, lValue);
	//循環查找對應的CMD
	while ((g_commmap[i].wCmd != wCmd) && (g_commmap[i].wCmd != COMM_NULL)) i++;
	if (g_commmap[i].wCmd != COMM_NULL)
	{
		//查找到對應的CMD，調用對應的函數
		m_nCommandIndex = g_commmap[i].wCmd;

		(this->*(g_commmap[i].npFunc))();
		return TRUE;
	}
	//Echo(COMM_ECHO_FAILURE);
	return FALSE;
}

/*!	\fn int	CtmDsp54All::ParseProtocol	(void* pInData,		int nInputLen)
 *	\brief	解析主機發過來的數據
 *	\param	[void*]	pInData		數據內容
 *	\param	[in]	nInputLen	數據長度
 *	\return 0
 */
int		CtmDsp54All::ParseProtocol	(void* pInData,		int nInputLen)
{
	//printf("CtmDsp54All g_frameRx.wCmd=%x, nInputLen = %d \n", g_frameRx.wCmd, nInputLen);
	//檢查數據是否合法
	if (!DataIsValid(pInData, nInputLen))
	{
		Echo(COMM_ECHO_FAILURE);
		return -1;
	}
	//裁剪協議頭的前3個WORD，將后面數據內容拷貝到buffer中
	m_wGetDataCounter = *(WORD*)((char*)pInData + m_nTypePos);
	memcpy(&g_frameRx, (char*)pInData + m_nCMDPos, nInputLen - m_nCMDPos);
	//處理此命令
	DoDealFunc(g_frameRx.wCmd);
	return 0;
}
/*****************************************************************************/
/*                          HELP FUNCTION                                    */
/*****************************************************************************/
/*!	\fn void CtmDsp54All::SendRequestRead(int nCommand)
 *	\brief	MMI發送讀資料請求
 *  格式	COMMAND	 ID	  LEN		 DATA
 *           '12'   0000  1WORD    'BA'/'BB'......
 *	\param	[int]	nCommand		請求數據的CMD
 *	\return 0
 */
void        CtmDsp54All::SendRequestRead(WORD wCommand)
{
	int         nWriteCount;
	//printf("wCommand=%x \n", wCommand);
	g_frameTx.wCmd = COMM_ETH_SENDREQUESTREAD;		// 	AC MMI 請求讀
	g_frameTx.commdata.cmddata.wCmd = wCommand;				//	DATA 為請求讀的CMD類型
	WriteToken((BYTE*)&g_frameTx, 2, &nWriteCount);
}

/*!	\fn void CtmDsp54All::SendUpdateData(BYTE* pch, int nCommand)
 *	\brief	MMI發送寫資料請求
 *	\param	[BYTE*]	pch				發送數據的內容
 *	\param	[int]	nCommand		發送數據的CMD
 *	\param	[int]	nCount			發送數據總WORD數
 *	\param	[WORD]	wFirstID		發送數據的起始ID
 *  格式	COMMAND	  ID	  	LEN			DATA
 *           '13'   wFirstID    ID*2     'BA'/'BB'......
 *	\return 0
 */
void        CtmDsp54All::SendUpdateData(ETHNETFUNCTX ethnetFuncTX)
{
	int         nWriteCount;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		g_frameTx.wCmd = COMM_ETH_SENDUPDATEDATA;		// MMI 請求寫
		g_frameTx.commdata.cmddata.wCmd	=	ethnetFuncTX.wCmd;
		memcpy(&g_frameTx.commdata.acBuffer[sizeof(WORD)], ethnetFuncTX.acData, ethnetFuncTX.wLength);
		WriteToken((BYTE*)&g_frameTx, ethnetFuncTX.wLength + sizeof(WORD), &nWriteCount, ethnetFuncTX.wID);
	}
}

void		CtmDsp54All::SendConnect()
{
	BYTE	bByte = 0;
	SendData(m_nDeviceID, (char*)&bByte, 1);
}

/*!	\fn void        CtmDsp54All::SendRequestConfig()
 *	\brief	請求組態資料
 *	\return 0
 */
void        CtmDsp54All::SendRequestConfig()
{
	int         nWriteCount;
	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		g_frameTx.wCmd = COMM_SENDREQUESTCONFIG;
		WriteToken((BYTE*)&g_frameTx, (0 + 2), &nWriteCount);
	}
}

/*!	\fn void        CtmDsp54All::SendPanel(BYTE cKey)
 *	\brief	發送按鍵
 *          按鍵的發送將cmd的后面一個BYTE作為按鍵值發送
 *	\param	[BYTE]	cKey	按鍵值
 *	\return 0
 */
void        CtmDsp54All::SendPanel(BYTE cKey)
{
	if (!g_bAction) return;

	if ((cKey == 0xB3) && (GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue == 0)) /*wangli2016-5-30 13:55:03 模溫按鍵*/
		return;
	int         nWriteCount;
	long		lValue;
	//ETHNETFUNCTX	ethnetresendTx;

	/*if ( (cKey == (BYTE)(_KEY_PANEL_SEMIAUTO & 0x00FF)) || (cKey == (BYTE)(_KEY_PANEL_SENSORAUTO & 0x00FF)) ||
	 (cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF)))
	{
		DB_PRODMANAGE *pdbplaninfo	= (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetPlanInfo();
		if (pdbplaninfo != NULL)
		{
			if (pdbplaninfo->nCurrentIndex == 0) UpdateProduceStatus(MANUFACTSTAT_NOBILL);
			else UpdateProduceStatus(MANUFACTSTAT_PRODUCT);
		}
	}
	else if ((cKey == (BYTE)(_KEY_PANEL_MOLDTHIN & 0x00FF)) || (cKey == (BYTE)(_KEY_PANEL_MOLDTHICK & 0x00FF)))
	{
		UpdateProduceStatus(MANUFACTSTAT_ADJUSTMOLD);
	}
	*/
	lValue = GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue;//JOYCE2011-9-2 bug,資料有誤  OPER_HMI_NULL_NULL_TIMEOUTSTATUS
	if (lValue)                  //Chuntzu 2004/5/4 add for 停機功能
		if ((cKey == (BYTE)(_KEY_PANEL_SEMIAUTO & 0x00FF)) || (cKey == (BYTE)(_KEY_PANEL_SENSORAUTO & 0x00FF)) ||
		        (cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF)) ) return;

	if (m_bALogin) /*JOYCE2012-6-18 切換到全自動狀態時,需要輸入密碼確認*/
	{
		if ((cKey == (BYTE)(_KEY_PANEL_TIMEAUTO & 0x00FF)) && (GetDBValue("MSET_OTHR_OTHR_NULL_PHOTOSENSOROPTION").lValue != 1))
		{
			WORD	wOperMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;

			if (wOperMode & 0x0008)	/*全自動狀態*/
			{
				return;
			}
			else
			{
				if (m_bAutoKey == FALSE)
				{
					m_bAutoKey = TRUE;
					int ntemp = g_WndLoginPrivildge;
					g_WndLoginPrivildge = 5;
					MsgForm("password.txt");

					g_WndLoginPrivildge = ntemp;
					m_bAutoKey = FALSE;

					if (g_bMsgFormKey == _KEY_CLEAR)
					{
						return;
					}
					else  if (g_bMsgFormKey == _ENTER)
					{
						if (g_iPrivilege < 5)
						{
							MsgBox(g_MultiLanguage["MSG_LOGIN_INVALIDPRIVILEGE"], 3);
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

	/*******
	|       Normal process
	*******/
	if (cKey != 0xFF)
	{
		cKey += (BYTE)(0x20 - 0x80);
		m_CmdPanelBreak.wCmd   = 0;
	}
	else
	{
		//SetDBValue("MACH_OTHR_OTHR_NULL_WPANELKEYCODE", 0xFFFF, FALSE);
		m_CmdPanelBreak.wCmd   = COMM_ETH_SENDPANEL;
		//m_CmdPanelBreak.wIndex = g_pDatabase->Read("MACH_OTHR_OTHR_NULL_WPANELKEYCODE").lID;
		m_CmdPanelBreak.wValue = 0xFF;
		m_cStateRx |= COMM_STATE_WAIT00;
	}
	g_frameTx.wCmd = COMM_ETH_SENDPANEL;

	// printf("cKey =%x\n",cKey);

	WORD	wTempKey = (WORD)cKey;
	memcpy(g_frameTx.commdata.acBuffer, &wTempKey, sizeof(WORD));
	//printf("SendPanel g_frameTx.wCmd=%x \n", wTempKey);
	//if(m_bGetPanlEcho == TRUE || wTempKey == 0xFF)			//必須等收到dsp54的Echo才可以發送下一個key
	//{
	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		WriteToken((BYTE*)&g_frameTx, (0 + 2), &nWriteCount);
	}
	////加入到buffer，增加重發機制
	//ethnetresendTx.wCmd = g_frameTx.wCmd;
	//ethnetresendTx.wID	= 0;
	//ethnetresendTx.wLength	= 2;
	//memcpy(ethnetresendTx.acData, &wTempKey, sizeof(WORD));
	//AddResend(ethnetresendTx);
	m_bGetPanlEcho = FALSE;
	//}
}

/*!	\fn void        CtmDsp54All::SendControlMachine()
 *	\brief	發送Machine資料(分上下兩段發送)
 *	\return 0
 */
void        CtmDsp54All::SendControlMachine(WORD wType)
{
	int         nWriteCount;
	WORD        wValue;
	WORD        wLength, wDataID, wIndex;
	char*				pszID;
	DWORD		dwValue = 0;//JOYCE2011-8-22
	WORD		wDSPLIBDaysCode = GetDBValue("CONF_CODE_WRITE_NULL_DATA16").lValue;
	WORD		wDSPLIBYearCode = GetDBValue("CONF_CODE_WRITE_NULL_MCHINEDATA1").lValue;

	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION").lValue & 0xFFEF | 0x8000 | 0x1000 | 0x0400 | 0x0080 | 0x0040 | 0x0020; //Sunny<20070824> amend

	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION", wValue, FALSE);

	SetDBValue("MACH_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);

	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue | 0x0004 | 0x0200; //0x0200 射出曲線增加一組壓力速度資料 (WL151222)
	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);


	/*wangli2016-3-18  mark  3354 馬達 和電熱 都由一個按鍵控制*/
//	if(!g_pExTable->GetTableValue(0,"D_TM3160") && !g_pExTable->GetTableValue(0,"D_TM3360") && !g_pExTable->GetTableValue(0,"D_SK3360"))//JOYCE2011-8-25 //配合售后 replace 386 ,add judgement
//	{
//	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue | 0x0002;//motor/heater code
//	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);
//	}
//

	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2").lValue | 0x0080;//lub1.2手動操作鍵  分開控制
	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION2", wValue, FALSE);

	//if(((dbconf.wControlSequenceFlagPLC[6]&0xFF00) ==0x8400)&&((dbconf.wHydraulicType[6]&0x000F) != 0)) //Andy 20070205 for 7HT_J5 guard against theft !!
	if (((GetDBValue("CONF_CODE_WRITE_NULL_DATA6").lValue & 0xFF00) == 0x8400)
	        && ((GetDBValue("CONF_CODE_WRITE_NULL_W0AE6").lValue & 0x000F) != 0)) //Andy 20070205 for 7HT_J5 guard against theft !!
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue | 0x0010;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}
	if (m_bDC) //James add 2008/8/13 09:42上午
	{
		if (!g_pExTable->GetTableValue(0, "D_MIX2")) //夾層機不需要
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
	dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;//JOYCE2011-8-22  	DSP54計開模數
	SetDBValue("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", dwValue, FALSE);       //JOYCE2011-8-22
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

	if ((wDSPLIBDaysCode >= 0x1805) && (wDSPLIBYearCode == 0x2011) || (wDSPLIBYearCode >= 0x2012)) //JOYCE2011-8-22 shotcnt dsp54 cnt
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue |= 0x0200;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}

	if (!m_bDC && ((wDSPLIBDaysCode >= 0x2B27) && (wDSPLIBYearCode == 0x2012) || (wDSPLIBYearCode >= 0x2013)))	//2012-12-11, >= 2B27
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		if (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED16").lValue)		//面板組態畫面 多次托模停留 選擇
			wValue |= 0x2000;
		else
			wValue &= 0xDFFF;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}
	else
	{
		SetDBValue("MSET_EJT_EJT_NULL_W05B8", 0xFFFF);	//多次托模停留
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED16", 0, FALSE);	//面板組態畫面 多次托模停留 選擇

		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue &= 0xDFFF;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}

//	if(!m_bDC && ((wDSPLIBDaysCode >= 0x3305)&&(wDSPLIBYearCode==0x2013)||(wDSPLIBYearCode >= 0x2014)))/*joyce20130314 監測增加新功能*/
//		{
	wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
	wValue |= 0x4000;
	SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);

	SetDBValue("MCONF_SCREW_INJECT_NULL_INJECHECKOPTION", 0, FALSE); /*射出監控選項不用*/
	SetDBValue("MACH_SCRW_CUSHION_NULL_OPTION", 0, FALSE);
//		}
//	else
//		{
//			wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
//			wValue &= 0xBFFF;
//			SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
//		}

	if (!m_bDC) /*JOYCE20130515閥門強制輸出 D_VALO*/
	{
		wValue = GetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1").lValue;
		wValue |= 0x1000;
		SetDBValue("MACH_OTHR_OTHR_NULL_WVERSION1", wValue, FALSE);
	}

	if (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED6").lValue)
	{
		SetDBValue("MACH_OTHR_OTHR_NULL_W01A9", GetCurrentMoldMedia(), FALSE);//wangli2014-7-3 14:02:07 送模號給主機
	}

	if (GetDBValue("CONF_CODE_WRITE_NULL_W0BB0").lValue != 1)
		TempAutProcess(); /*wangli2016-1-21 16:21:30 對5580 主機 和5530主機 的溫度部分 HMI自動處理*/


	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_MACHINE_ID);

	if (bINJBlow)
	{
		for (int i = 0; i < 2; i++)
		{
			char	pDataID[128];
			sprintf(pDataID, "MACH_OTHR_OTHR_NULL_OTHERMACHINE%d", i + 1);
			long lVlaue = GetDBValue(pDataID).lValue;
			long lTemp = (int)(lVlaue * 4.095 + 0.5); /*傳給主機的值為(x/1000)*4095四捨五入*/
			wDataID	= g_pDatabase->GetDataID(pDataID);
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, pszID, lTemp);
		}
		long lTemp    = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED106").lValue;
		long lRotaPos =  GetDBValue("MSET_OTHR_OTHR_NULL_W0864").lValue;
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, "MACH_OTHR_OTHR_NULL_W0395", lRotaPos + lTemp);
		if (lTemp > lRotaPos)
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, "MACH_OTHR_OTHR_NULL_W0396", 0);
		else
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, "MACH_OTHR_OTHR_NULL_W0396", lRotaPos - lTemp );

	}

	if (m_bSVRT)
	{
		for (int i = 0; i < 2; i++)
		{
			char *pDataID[] = {
				"MACH_MLD_CLOSE_NULL_W0099",
				"MACH_AD1_WRITE_NULL_W023E",
			};
			long lVlaue = GetDBValue(pDataID[i]).lValue;
			long lTemp = (int)(lVlaue * 4.095 + 0.5); /*傳給主機的值為(x/1000)*4095四捨五入*/
			wDataID	= g_pDatabase->GetDataID(pDataID[i]);
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, pszID, lTemp);
		}

		for (int i = 0; i < 2; i++)
		{
			char *pDataID[] = {
				"MACH_OTHR_OTHR_NULL_W01B4",
				"MACH_AD1_WRITE_NULL_W023D",
			};
			long lVlaue = GetDBValue(pDataID[i]).lValue;
			long lTemp  = ChangeVoltage(pDataID[i], 2, lVlaue);
			wDataID	= g_pDatabase->GetDataID(pDataID[i]);
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, pszID, lTemp);
		}
	}

	if (m_b28Clmp) /*JOYCE2015-8-14  28控制開關模, 歸零點送0給主機*/
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, "MACH_AD1_WRITE_CHL2_HOMEPOSITIONOFFSET", 0);

//	wDataID = g_pDatabase->GetDataID("MACH_SCRW_INJ_NULL_FLOWINTERVALTIME");/*采樣間隔*/
//	wIndex  = g_pDatabase->DataIDToIndex(wDataID);
//	pszID   = g_pDatabase->GetString(wIndex);
//	wValue  = GetDBValue("MACH_SCRW_INJ_NULL_FLOWINTERVALTIME").lValue/10;
//	printf("tmDsp54All  wValue1=%d\n",GetDBValue("MACH_SCRW_INJ_NULL_FLOWINTERVALTIME").lValue);
//	printf("tmDsp54All  wValue2=%d\n",wValue);
//	g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID, pszID, wValue);


	//wStartID2 = wLength/4;
	//James add 2008/4/28 06:54下午
//	if(1 == CtmConfig::GetInstance()->GetKeyFlag())
//	{
	//if (GetOilTable()) //對於二板機會有問題
	TransferOilVat(TRUE);
//	}
	if (1 == wType)
	{
		//發送上段
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLMACHINE1;
		WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
	}
	else if (2 == wType)
	{
		//發送下段
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLMACHINE2;
		memcpy(g_frameTx.commdata.acBuffer, g_frameTx.commdata.acBuffer + wLength / 2, wLength / 2);
		WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
		m_bGetPanlEcho = TRUE;
	}

}

/*!	\fn void        CtmDsp54All::SendControlMoldset()
 *	\brief	發送Moldset資料(分上下兩段發送)
 *	\return 0
 */
void        CtmDsp54All::SendControlMoldset(WORD	wType)
{
	int         nWriteCount;
	WORD		wLength, wDataID, wIndex, wSDOPNPOS, wSDPuslePOS, wSDPusleCNT;
	char*				pszID;
	//WORD		wStartID1 = 0x00, wStartID2 = 0x00;

	SetDBValue("MSET_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_MOLDSET_ID);
	//wStartID2 = wLength/4;

	if (m_bInjSpdUnit)		//wangli2014-8-20 16:20:01  inje/holdpre unit is mm/s
	{
		long lValue;
		for (int i = 0;  i < (int)(sizeof(g_MSET_InjeHoldSpdID) / sizeof(g_MSET_InjeHoldSpdID[0])); i++)
		{
			wDataID = g_pDatabase->GetDataID(g_MSET_InjeHoldSpdID[i]);
			lValue = GetDBValue(g_MSET_InjeHoldSpdID[i]).lValue;
			//printf("SYSX_INJECT_INJECT_NULL_MAXFLOW =%d\n",GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue);
			if ((lValue != 0xFFFF) && (GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue != 0))
				lValue = (lValue * 990 / GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue + 5) / 10;
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, lValue);
		}
	}

	if (GetDBValue("MACH_SCRW_RCV_NULL_W0066").lValue)
	{
		long lValue;
		wDataID = g_pDatabase->GetDataID("MSET_SCRW_DCMP_NULL_BEFORERECOVERYOFFSET");

		lValue = GetDBValue("MSET_SCRW_DCMP_NULL_BEFORERECOVERYOFFSET").lValue;

		//printf("moldset : lValue=%d\n",lValue);
		if ((lValue != 0xFFFF) && (lValue < 10))
			lValue = 10;
		wIndex = g_pDatabase->DataIDToIndex(wDataID);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, lValue);
	}

	//#ifdef   D_DC									//Gwei 2005/5/21 新增,清54的溫度設定值
	if (m_bDC) //James add 2008/8/13 09:42上午
	{
		//printf("SelTempSendValue \n");
		SelTempSendValue();
	}

	//處理脈沖式電動門
	m_bSDOOR  = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED98").lValue & 0x0001;  //JOYCE2013-10-30 g_pExTable->GetTableValue(0,"D_SDOOR");
	m_b2SDOOR = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED98").lValue & 0x0002;  //JOYCE2013-10-30 g_pExTable->GetTableValue(0,"D_2SDOOR");
	if (m_bSDOOR) //JOYCE2011-11-3
	{
		wSDOPNPOS   = GetDBValue("MSET_SCRW_DCMP_NULL_W079F").lValue;//安全門開終位置
		wSDPuslePOS = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED81").lValue;//單脈沖行程
		if (wSDPuslePOS)
			wSDPusleCNT = wSDOPNPOS / wSDPuslePOS;
		else
			wSDPusleCNT = 0;

		wDataID = g_pDatabase->GetDataID("MSET_SCRW_DCMP_NULL_W079F");//安全門開終位置
		wIndex = g_pDatabase->DataIDToIndex(wDataID);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, wSDPusleCNT);
	}
	if (m_b2SDOOR)
	{
		wSDOPNPOS   = GetDBValue("MSET_SCRW_DCMP_NULL_W079D").lValue / 400; //安全門開終位置
		wDataID = g_pDatabase->GetDataID("MSET_SCRW_DCMP_NULL_W079D");//安全門開終位置
		wIndex = g_pDatabase->DataIDToIndex(wDataID);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, wSDOPNPOS);
	}

	if (m_bSVRT)
	{
		long lTemp = SetUnitChangetoBar("MSET_MLD_OPEN_NULL_W074D");
		wDataID	= g_pDatabase->GetDataID("MSET_MLD_OPEN_NULL_W074D");
		wIndex = g_pDatabase->DataIDToIndex(wDataID);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, lTemp);

		if (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED104").lValue)
		{
			long lLimit = SetUnitChangetoBar("SYSX_OTHERS_OTHERS_INT_RESERVED75");
			long lTemp  = SetUnitChangetoBar("MSET_MLD_OPEN_NULL_W074D");
			long lSub   = lTemp - lLimit;
			if (lSub < 0)
				lSub = 0;
			SetDBValue("MSET_MLD_OPEN_NULL_W074F", lSub, FALSE);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, "MSET_MLD_OPEN_NULL_W074F", lSub);

			wDataID	= g_pDatabase->GetDataID("MSET_MLD_OPEN_NULL_W074D");
			wIndex = g_pDatabase->DataIDToIndex(wDataID);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, lTemp);
		}
	}




	//處理模溫
	//long	lMoldTempType;
	char	psz[5];
	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS", psz, 4); //判斷模溫標識
	switch (psz[2])
	{
	case	'0':
		break;
	case	'1'://D_MDT10  com1, channel 11-20
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 10; i < 20; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
		break;
	case	'2':
		break;
	case	'3'://D_MDT4 	//JOYCE2010-7-20 add   com1, channel 6-9
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 5; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'4'://D_T3M6  com1, channel 4-9
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 3; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'5'://D_T3M5  com1, channel 4-8    //JOYCE2011-10-19
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 3; i < 8; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
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
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 7; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
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
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 5; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
			for (int i = 10; i < 20; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'E': //JOYCE2011-2-17 COM1 2段模溫(CH8~9),40段COM2模溫
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 7; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'F':	//D_T5M3   //JOYCE2011-5-20  com1, channel 6-8
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 5; i < 8; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'H'://D_T12M4   com1, channel 7-8&&17-18 //JOYCE2011-6-10
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 6; i < 8; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
			for (int i = 16; i < 18; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'I'://D_T14M4   com1, channel 8-9&&18-19 //JOYCE2012-5-31
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 7; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
			for (int i = 17; i < 19; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'J'://com1, channel 4-9&&11-20
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 3; i < 9; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
			for (int i = 10; i < 20; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'K'://com1, channel 1-8    //JOYCE2012-9-11
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 0; i < 8; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'L':	//D_T5M2    //JOYCE2013-12-4   com1, channel 6-7
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 5; i < 7; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'M':	//D_T6M2    //JOYCE2013-12-4   com1, channel 7-8
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 6; i < 8; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'P':	//wangli2014-12-3 9:42:43  T11M4     com1, channel 13-16
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 12; i < 16; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	case	'Q':/*wangli2015-6-1 13:30:54 T16M2 com1, channel 8,18*/
		if (!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
		{
			wDataID = g_pDatabase->GetDataID("MSET_TMP1_HEATERON_CHL1_DEGREE");
			for (int i = 7; i < 8; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
			for (int i = 17; i < 18; i++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
				pszID = g_pDatabase->GetString(wIndex);
				g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
			}
		}
		break;
	default:
		break;
	}
	//#endif

	if (m_bCOM2Vlv) /*JOYCE 20140508 COM2閥門16組中不管哪組選擇使用,都要送1E7的值為1給COM1*/
	{
		for (int i = 0; i < 16; i++)
		{
			char	pDataID[128];
			sprintf(pDataID, "GATE_GATE%d_GATE1_NULL_MODEOPTION", i + 1);
			long lValOpt = GetDBValue(pDataID).lValue;
			if (lValOpt)
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

	if (1 == wType)
	{
//printf("CtmDsp54All::SendControlMoldset  111\n");
		//發送上段
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSET1;
		WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
	}
	else if (2 == wType)
	{
//printf("CtmDsp54All::SendControlMoldset  222\n");
		//發送下段
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSET2;
		memcpy(g_frameTx.commdata.acBuffer, g_frameTx.commdata.acBuffer + wLength / 2, wLength / 2);
		WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
	}
}

/*!	\fn void        CtmDsp54All::SendControlMachineB()
 *	\brief	發送MachineB資料
 *	\return 0
 */
void        CtmDsp54All::SendControlMachineB()
{
	int         nWriteCount;
	WORD		wLength/*,wDataID,wIndex*/;
	//char*				pszID;
	WORD		wStartID1 = 0x00;

	SetDBValue("MACHB_SCRW2_RCV_NULL_CHECKDATA", 0xA596, FALSE);
	g_frameTx.wCmd = COMM_ETH_SENDCONTROLMACHINEB;
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MACHINEB_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_MACHINEB_ID);

//	wDataID = g_pDatabase->GetDataID("MACHB_SCRW2_INJ_NULL_W107C");/*采樣間隔*/
//	wIndex = g_pDatabase->DataIDToIndex(wDataID);
//	pszID = g_pDatabase->GetString(wIndex);
//	g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINEB_ID, pszID, GetDBValue("MACHB_SCRW2_INJ_NULL_W107C").lValue/10);

	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount, wStartID1);
}

/*!	\fn void        CtmDsp54All::SendControlMoldsetB()
 *	\brief	發送MachineB資料(分上下兩段發送)
 *	\return 0
 */
void        CtmDsp54All::SendControlMoldsetB(WORD wType)
{
	int         nWriteCount;
	WORD		wLength, wDataID, wIndex;
	char*				pszID;

	SetDBValue("MSETB_OTHR_OTHR_NULL_CHECKDATA", 0xA596, FALSE);
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MOLDSETB_ID);
	wLength = 0x500 * 2; //g_pBlock->GetBlockSize(BLOCK_MOLDSETB_ID);  /*HMI資料庫只有0x300,主機資料庫預留長度是0x500*/

	if (m_bDC && GetDBValue("MACHB_SCRW2_RCV_NULL_W1014").lValue)
	{
		long lValue;
		wDataID = g_pDatabase->GetDataID("MSETB_SCRW2_DCMP_NULL_BEFORERECOVERYPOSITION");
		lValue = GetDBValue("MSETB_SCRW2_DCMP_NULL_BEFORERECOVERYPOSITION").lValue;
		//printf("sendcontrol molssetb :lValue=%d\n",lValue);
		if ((lValue != 0xFFFF) && (lValue < 10))
			lValue = 10;
		wIndex = g_pDatabase->DataIDToIndex(wDataID);
		pszID = g_pDatabase->GetString(wIndex);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSETB_ID, pszID, lValue);
	}

	if (1 == wType)
	{
		//發送上段
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSETB1;
		WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
	}
	else if (2 == wType)
	{
		//發送下段
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSETB2;
		memcpy(g_frameTx.commdata.acBuffer, g_frameTx.commdata.acBuffer + wLength / 2, 0x100);
		WriteToken((BYTE*)&g_frameTx, 0x100, &nWriteCount);/*length,0x300-0x280=0x80 WORD=0x100 byte(資料庫0x300減去上半段長度0x280)*/
	}
}

void		CtmDsp54All::SendControlNewMachine2(WORD	wType)
{
	int         nWriteCount;
	WORD		wLength;
	//WORD		wStartID1 = 0x00, wStartID2 =0x00;

	SetDBValue("MACH2_COR_RESV_NULL_W00FF", 0xA596, FALSE);
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_NEWMACHINE2_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_NEWMACHINE2_ID);

	//printf("SendControlNewMachine wLength=%d \n", wLength);
	if (wLength > 0)
	{
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLNEWMACHINE2;
		WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
	}
}


void		CtmDsp54All::SendControlNewMoldset2(WORD	wType)
{
	int         nWriteCount;
	WORD		wLength;
	//WORD		wStartID1 = 0x00, wStartID2 =0x00;

	SetDBValue("MSET2_OTHER_OTHER_NULL_W03FF", 0xA596, FALSE);
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_NEWMOLDSET2_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_NEWMOLDSET2_ID);

	if (GetOilTable())
		TransferOilVat(TRUE, TRUE);

	if (wLength > 0)
	{
		if (1 == wType)
		{
			//printf("CtmDsp54All::SendControlNewMoldset2  1111 wLength=%d \n", wLength);
			g_frameTx.wCmd = COMM_ETH_SENDCONTROLNEWMOLDSET2_1;
			WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
		}
		else if (2 == wType)
		{
			//printf("CtmDsp54All::SendControlNewMoldset2  2222 wLength=%d \n", wLength);
			g_frameTx.wCmd = COMM_ETH_SENDCONTROLNEWMOLDSET2_2;
			memcpy(g_frameTx.commdata.acBuffer, g_frameTx.commdata.acBuffer + wLength / 2, wLength / 2);
			WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
		}
	}



}

void		CtmDsp54All::SendControlNewMachine3(WORD	wType)
{
	int         nWriteCount;
	WORD		wLength, wDataID, wIndex;
	char*				pszID;
	//WORD		wStartID1 = 0x00, wStartID2 =0x00;

	SetDBValue("MACH3_OTHER_OTHER_NULL_W01FF", 0xA596, FALSE);

	if (GetDBValue("CONF_CODE_WRITE_NULL_W0BB0").lValue != 1)
		TempAutProcess2();

	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_NEWMACHINE3_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_NEWMACHINE3_ID);

	/*wangli2016-9-29 9:16:36   移至畫面處理* /
	//	char	psz[5];
	//	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//判斷模溫標識
	//	switch(psz[2])
	//	{
	//		case	'R':/*wangli2015-10-13 17:23:38 5528 3300TON 特殊機 第3,4,5塊RMTP12做模溫*/
//			  wDataID = g_pDatabase->GetDataID("MACH3_TMP1_HEATERONOFF_RAMP1_DEGREE");
//			  for (int i=24; i<60; i++)
//			  	{
//			  		wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
//			  		pszID = g_pDatabase->GetString(wIndex);
//			  		if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)/*模溫開關選不用的時候*/
//			  		{
//			  			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_NEWMACHINE3_ID, pszID, (GetDBValue(pszID).lValue) & 0xFEFF);/*Bit 8給0*/
//			  		}
//			  		else
//			  		{
//			  			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_NEWMACHINE3_ID, pszID, (GetDBValue(pszID).lValue) | 0x0100);/*Bit 8給1*/
//			  		}
//			  	}
//
//			break;
//		default:
//			break;
//	}
	if (wLength > 0)
	{
		g_frameTx.wCmd = COMM_ETH_SENDCONTROLNEWMACHINE3;
		WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
	}
}


void		CtmDsp54All::SendControlNewMoldset3(WORD	wType)
{
	int         nWriteCount;
	WORD		wLength;
	//WORD		wStartID1 = 0x00, wStartID2 =0x00;


	SetDBValue("MSET3_OTHER_OTHER_NULL_W04FF", 0xA596, FALSE);
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_NEWMOLDSET3_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_NEWMOLDSET3_ID);

//  printf("SendControlNewMoldset3");
//	char	psz[5];
//	GetDBString("VERS_HMI_NULL_NULL_PROGRAMRESVERS",psz,4);//判斷模溫標識
//	switch(psz[2])
//	{
//		case	'R':/*wangli2015-10-13 17:23:38 5528 3300TON 特殊機 第3,4,5塊RMTP12做模溫*/
//			if(!GetDBValue("SYSX_TEMPERMOLD_HEATERON_NULL_MOLDHEATOPTION").lValue)
//				{
//					wDataID = g_pDatabase->GetDataID("MSET3_TMP1_HEATERON_CHL1_DEGREE");
//					for (int i=24; i<60; i++)
//						{
//							wIndex = g_pDatabase->DataIDToIndex(wDataID+i);
//							pszID = g_pDatabase->GetString(wIndex);
//							g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_NEWMOLDSET3_ID, pszID, 0);
//						}
//				}
//			break;
//		default:
//			break;
//	}

	if (wLength > 0)
	{
		if (1 == wType)
		{
			//printf("CtmDsp54All::SendControlNewMoldset3 1111 wLength=%d \n", wLength);
			g_frameTx.wCmd = COMM_ETH_SENDCONTROLNEWMOLDSET3_1;
			WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
		}
		else if (2 == wType)
		{
			//printf("CtmDsp54All::SendControlNewMoldset3 2222 wLength=%d \n", wLength);
			g_frameTx.wCmd = COMM_ETH_SENDCONTROLNEWMOLDSET3_2;
			memcpy(g_frameTx.commdata.acBuffer, g_frameTx.commdata.acBuffer + wLength / 2, wLength / 2);
			WriteToken((BYTE*)&g_frameTx, wLength / 2, &nWriteCount);
		}
	}
}

/*!	\fn void        CtmDsp54All::SendConfig()
 *	\brief	發送config資料(只需發送前面的0x140個WORD); 后來有加DB_CONFIG_CODE的block,長度要變
 *	\return 0
 */
void		CtmDsp54All::SendConfig()
{
	int         nWriteCount;
	WORD		wLength;

	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer + sizeof(WORD), BLOCK_CONFIG_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_CONFIG_ID);

	//printf("SendConfig wLength=%d \n", wLength);
	g_frameTx.wCmd = COMM_ETH_SENDUPDATEDATA;		// MMI 請求寫
	g_frameTx.commdata.cmddata.wCmd	=	COMM_ETH_GETCONFIG;
	m_wCmdSend  =	g_frameTx.commdata.cmddata.wCmd;
	m_wCmdState	= 	CMD_SEND;

	WriteToken((BYTE*)&g_frameTx, wLength + sizeof(WORD), &nWriteCount); /*后來有加DB_CONFIG_CODE的block,長度要變*/
}

/*!	\fn void        CtmDsp54All::SendDiagRMRI(DIAGRMRI diagrmri)
 *	\brief	讀取DSP ROM&RAM
 *	\param	[DIAGRM]	diagrm	ROM & RAM地址
 *	\return 0
 */
int        CtmDsp54All::SendDiagRMRI(DIAGRMRI diagrmri)
{
	int         nWriteCount;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		m_nSendRM++;
		g_frameTx.wCmd = COMM_ETH_SENDDIAGRMRI;
		g_frameTx.commdata.diagrmri.wAddressROM = diagrmri.wAddressROM;
		g_frameTx.commdata.diagrmri.dwAddressRAM = diagrmri.dwAddressRAM;
		g_frameTx.commdata.diagrmri.wAddressRI 	= diagrmri.wAddressRI;
		WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagrmri)), &nWriteCount);
	}
	return m_nSendRM;	//	fans add
}

/*!	\fn void        CtmDsp54All::SendDiagWM(DIAGWM diagwm)
 *	\brief	寫入DSP RAM
 *	\param	[DIAGWM]	diagrm	ROM & RAM地址
 *	\return 0
 */
void        CtmDsp54All::SendDiagWM(DIAGWM diagwm)
{
	int         nWriteCount;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		g_frameTx.wCmd = COMM_ETH_SENDDIAGWM;
		g_frameTx.commdata.diagwm.dwAddress 	= diagwm.dwAddress;
		g_frameTx.commdata.diagwm.wData 	= diagwm.wData;
		WriteToken((BYTE*)&g_frameTx, (sizeof(g_frameTx.commdata.diagwm)), &nWriteCount);
	}
}

/*!	\fn void       CtmDsp54All::SendDiagWO(DIAGWO diagwo)
 *	\brief	寫入DSP OUTPUT
 *	\param	[DIAGWO]	diagwo	DSP OUTPUT
 *	\return 0
 */
void        CtmDsp54All::SendDiagWO(DIAGWO diagwo)
{
	int         nWriteCount;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		g_frameTx.wCmd = COMM_ETH_SENDDIAGWO;
		g_frameTx.commdata.diagwo.wAddress 	= diagwo.wAddress;
		g_frameTx.commdata.diagwo.wData 	= diagwo.wData;
		WriteToken((BYTE*)&g_frameTx, sizeof(g_frameTx.commdata.diagwo), &nWriteCount);
	}
}

/*!	\fn void       CtmDsp54All::SendDiagWO(WORD wAddress, WORD wData)
 *	\brief	寫入DSP OUTPUT
 *	\param	[WORD]	wAddress
 *	\param	[WORD]	wData
 *	\return 0
 */
void        CtmDsp54All::SendDiagWO(WORD wAddress, WORD wData)
{
	DIAGWO      diagwo;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		diagwo.wAddress = wAddress;
		diagwo.wData    = wData;
		SendDiagWO(diagwo);
	}
}

/*!	\fn void        CtmDsp54All::SendAssignInput(WORD wOldAddress, WORD wNewAddress)
 *	\brief	對換 DSP INPUT
 *	\param	[WORD]	wOldAddress
  *	\param	[WORD]	wNewAddress
 *	\return 0
 */
void        CtmDsp54All::SendAssignInput(WORD wOldAddress, WORD wNewAddress)
{
	int         nWriteCount;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		g_frameTx.wCmd = COMM_ETH_SENDASSIGNINPUT;
		g_frameTx.commdata.assignio.wOldAddress = wOldAddress;
		g_frameTx.commdata.assignio.wNewAddress = wNewAddress;
		WriteToken((BYTE*)&g_frameTx, (4), &nWriteCount);
	}
}

/*!	\fn void        CtmDsp54All::SendAssignOutput(WORD wOldAddress, WORD wNewAddress)
 *	\brief	對換 DSP OUTPUT
 *	\param	[WORD]	wOldAddress
  *	\param	[WORD]	wNewAddress
 *	\return 0
 */
void        CtmDsp54All::SendAssignOutput(WORD wOldAddress, WORD wNewAddress)
{
	int         nWriteCount;

	if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
	{
		g_frameTx.wCmd = COMM_ETH_SENDASSIGNOUTPUT;
		g_frameTx.commdata.assignio.wOldAddress = wOldAddress;
		g_frameTx.commdata.assignio.wNewAddress = wNewAddress;
		WriteToken((BYTE*)&g_frameTx, (4), &nWriteCount);
	}
}

/*!	\fn void        CtmDsp54All::SendControlMonitor()
 *	\brief	監測值上下限
 *	\return 0
 */
void        CtmDsp54All::SendControlMonitor()
{
	int         nWriteCount;
	DWORD		wLength, dwOldValue1, dwOldValue2;
	WORD		wStartID1 = 0x00;

	g_frameTx.wCmd = COMM_ETH_SENDMONILIMIT;
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_MONIMAXMIN_ID);
	if (GetOilTable())
	{
		//wangli2014-12-1 15:45:33 開模終點加入監測項目，bit15被用掉，需要過濾
		dwOldValue1 = GetOilVat(GetDBValue("MONI_ALARM_ON_NULL_MAXOPENPOSITION").lValue & 0x7FFFFFFF)
		              | (GetDBValue("MONI_ALARM_ON_NULL_MAXOPENPOSITION").lValue & 0x80000000);
		dwOldValue2 = GetOilVat(GetDBValue("MONI_ALARM_ON_NULL_MINOPENPOSITION").lValue & 0x7FFFFFFF)
		              | (GetDBValue("MONI_ALARM_ON_NULL_MINOPENPOSITION").lValue & 0x80000000);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID, "MONI_ALARM_ON_NULL_MAXOPENPOSITION", dwOldValue1);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMIN_ID, "MONI_ALARM_ON_NULL_MINOPENPOSITION", dwOldValue2);
	}


	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount, wStartID1);
}

/*!	\fn void        CtmDsp54All::SendControlMonitorB()
 *	\brief	B座監測值上下限
 *	\return 0
 */
void        CtmDsp54All::SendControlMonitorB()
{
	int         nWriteCount;
	WORD		wLength;

	g_frameTx.wCmd = COMM_ETH_SENDMONBILIMIT;
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer, BLOCK_MONIMAXMINB_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_MONIMAXMINB_ID);
	WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount);
}

/*!	\fn void        CtmDsp54All::SendPowerMatch()
 *	\brief	發送DA曲線資料
 *	\return 0
 */
void        CtmDsp54All::SendPowerMatch()  // St1108'01 for D/A
{
	int         i, j;
	int         nWriteCount;
	WORD		wLength, wDa_flag;
	WORD		wValue, wIndex, wStartID, wStartTempID;

	//printf("SendPowerMatch m_bNewDA=%d \n", m_bNewDA);

	wStartID	 = g_pDatabase->GetDataID("PMATCH2_PVL1_READ_CHL1_DATA");
	wStartTempID = g_pDatabase->GetDataID("DAOUT2_PVL1_WRITE_CHL1_DATA");
	for ( i = 0 ; i < 16 ; i++)
	{
		wDa_flag = 0;

		for (int k = 0 ; k < 16 ; k++)
		{
			wIndex = g_pDatabase->DataIDToIndex(wStartID + i * 16 + k);
			wValue = GetDBValue(wIndex).lValue;
			if (wValue)
			{
				wDa_flag = 1;
				break;
			}
		}
		if (wDa_flag == 0)
		{
			for ( j = 0 ; j < 16 ; j++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wStartTempID + i * 16 + j);
				wValue = GetDBValue(wIndex).lValue;
				wIndex = g_pDatabase->DataIDToIndex(wStartID + i * 16 + j);
				g_pDatabase->Writew(wValue, wIndex, FALSE);
			}
		}
	}

	wStartID	 = g_pDatabase->GetDataID("PMATCH_PVL1_READ_CHL1_DATA");
	wStartTempID = g_pDatabase->GetDataID("DAOUT_PVL1_WRITE_CHL1_DATA");
	for ( i = 0 ; i < 16 ; i++)
	{
		wDa_flag = 0;

		for (int k = 0 ; k < 16 ; k++)
		{
			wIndex = g_pDatabase->DataIDToIndex(wStartID + i * 16 + k);
			wValue = GetDBValue(wIndex).lValue;
			if (wValue)
			{
				wDa_flag = 1;
				break;
			}
		}
		if (wDa_flag == 0)
		{
			for ( j = 0 ; j < 16 ; j++)
			{
				wIndex = g_pDatabase->DataIDToIndex(wStartTempID + i * 16 + j);
				wValue = GetDBValue(wIndex).lValue;
				wIndex = g_pDatabase->DataIDToIndex(wStartID + i * 16 + j);
				g_pDatabase->Writew(wValue, wIndex, FALSE);
				//SetDBValue(wIndex, wValue, FALSE);
			}
		}
	}

	g_frameTx.wCmd = COMM_ETH_SENDUPDATEDATA;//COMM_SENDPOWERMATCH2;
	g_frameTx.commdata.cmddata.wCmd = COMM_ETH_GETDAOUTPUT;
	g_pBlock->SetBlock(g_frameTx.commdata.acBuffer + sizeof(WORD), BLOCK_NEWPOWERMATCH_ID);
	wLength = g_pBlock->GetBlockSize(BLOCK_NEWPOWERMATCH_ID);
	WriteToken((BYTE*)&g_frameTx, wLength + 2, &nWriteCount);
}

void        CtmDsp54All::SendStatus(WORD wStatus)
{
	WORD		wValue;
	char*		psz = "MACH_OTHR_OTHR_NULL_WERRORSTATUS1";

	wValue = GetDBValue(psz).lValue;
	wValue |= wStatus;

	g_pDatabase->Write(psz, &wValue);
	SendControlIndex(psz);
	//SetDBValue(psz, wValue);
}

/*!	\fn void        CtmDsp54All::SendTrasmitCmd(long	lCmd)
 *	\brief	轉發cmd命令
 *	\return 0
 */
void		CtmDsp54All::SendTransmitCmd(long	lCmd)
{

}

/*!	\fn void        CtmDsp54All::SendTrasmitBlock(WORD	wBlockID)
 *	\brief	轉發block資料
 *	\return 0
 */
void		CtmDsp54All::SendTransmitBlock(WORD	wBlockID)
{

}

/*!	\fn void        CtmDsp54All::SendTransmit(void*	pInData, int nInputLen)
 *	\brief	轉發單筆資料
 *	\return 0
 */
void		CtmDsp54All::SendTransmit(void*	pInData, int nInputLen)
{
//	int         	nWriteCount;
	TRANSMITDATA	TranSmit;
	//printf("SendTransmit 1 nInputLen=%d \n", nInputLen );
	//WORD	wData = 0;
	//for(int i = 0; i < nInputLen; i+=2)
	//{
	//	wData = *(WORD*)((char*)pInData+i);
	//	printf("wData %d=%d ", i, wData);
	//}
	if (nInputLen > 0 && nInputLen <= COMM_MAXTRANSIMT_NET)
	{
		memset(&TranSmit, 0, sizeof(TRANSMITDATA));
		memcpy(TranSmit.acBuffer, pInData, nInputLen);
		PushTransmitCmd(&TranSmit);
		//memset(g_frameTx.commdata.acBuffer, 0, COMM_MAXTRANSIMT_NET);
		//memcpy(g_frameTx.commdata.acBuffer, pInData, nInputLen);
		//g_frameTx.wCmd = COMM_SENDTRANSMITDSP28;
		//WriteToken((BYTE*)&g_frameTx, COMM_MAXTRANSIMT_NET, &nWriteCount, 0);
	}
	//printf("SendTransmit \n" );
	//int	nIndex = g_pDatabase->GetIndex(pszData);
	//if (nIndex >= 0)
	//	SendTransmit(nIndex);
}

/*!	\fn void        CtmDsp54All::SendTransmit(void*	pInData, int nInputLen)
 *	\brief	轉發單筆資料
 *	\return 0
 */
void		CtmDsp54All::SendTransmitQueue(void*	pInData, int nInputLen)
{
	int         	nWriteCount;
	if (nInputLen > 0)
	{
		WORD	wTemp;
		wTemp	= 0x0002;
		m_SendHeader.SetPartValue(m_nStartPos, &wTemp, sizeof(wTemp));
		wTemp   = m_nSendType;
		m_SendHeader.SetPartValue(m_nTypePos, &wTemp, sizeof(wTemp));
		wTemp	= 0x0003;
		m_SendFooter.SetPartValue(m_nEndPos, &wTemp, sizeof(wTemp));
		memset(g_frameTx.commdata.acBuffer, 0, COMM_MAXTRANSIMT_NET);
		memcpy(g_frameTx.commdata.acBuffer, pInData, COMM_MAXTRANSIMT_NET);
		WORD	wCmd = *(WORD*)((BYTE*)pInData + 4);
		//printf("wCmd=%d\n",wCmd);
		//WORD	wElement = *(WORD*)((BYTE*)pInData+10);
		//long	dwID = 0;//*(long*)((BYTE*)pInData+12);
		//float	fValue = 0;
		//
		//memcpy(&dwID, (BYTE*)pInData+12, sizeof(long));
		//
		//printf("===SendTransmitQueue wCmd=%d \n ", wCmd );
		//if(dwID == 0x200fc309 ||
		//	dwID == 0x200fc310 ||
		//	dwID == 0x200fc317)
		//{
		//	printf("=============SendTransmitQueue wCmd=%d nInputLen=%d wElement=%d dwID=%ld\n", wCmd, nInputLen, wElement, dwID);

		//	memcpy(&fValue, (BYTE*)pInData+18, sizeof(float));
		//	printf("fValue=%lf \n", fValue);
		//}
		g_frameTx.wCmd = COMM_SENDTRANSMITDSP28;
		WriteToken((BYTE*)&g_frameTx, COMM_MAXTRANSIMT_NET, &nWriteCount, 0);
	}
	//printf("SendTransmitQueue ==\n");
}


/*!	\fn void        CtmDsp54All::SendTrasmit(int nIndex)
 *	\brief	轉發單筆資料
 *	\return 0
 */
//void		CtmDsp54All::SendTransmit(int nIndex)
//{
//long	lID = 0, lValue = 0;
//char*	pszData;
//WORD	wType = 0, wLength = 0;
//CMDDATA cmddata;
//
//if (nIndex < 0)		return ;
//lID		= g_pDatabase->IndexToDataID(nIndex);
//wType	= g_pDatabase->Read(nIndex).DataType.wType;
//pszData = g_pDatabase->GetString(nIndex);
//if (pszData != NULL)
//{
//	g_frameTx.wCmd 	= COMM_SENDTRANSMITDSP28;
//	//lValue 			= GetDBValue(nIndex).lValue;
//	//wLength			= COMM_MAXTRANSIMT_NET;
//	//WriteToken((BYTE*)&g_frameTx, wLength, &nWriteCount, 0);
//	cmddata.wCmd 	= COMM_SENDTRANSMITDSP28;
//	cmddata.wIndex 	= lID;
//	if(m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
//		PushCmd(&cmddata);
//}
//}

/*!	\fn void        CtmDsp54All::SendControlIndex(char* pszData)
 *	\brief	發送單筆資料
 *	\return 0
 */
void        CtmDsp54All::SendControlIndex(char* pszData)
{
	int	nIndex = g_pDatabase->GetIndex(pszData);
	if (nIndex >= 0)
		SendControlIndex(nIndex);
}

/*!	\fn void        CtmDsp54All::SendControlIndex(int nIndex)
 *	\brief	發送單筆資料
 *	\return 0
 */
void        CtmDsp54All::SendControlIndex(int nIndex)
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
		g_frameTx.wCmd = COMM_ETH_SENDUPDATEDATA;
		g_frameTx.commdata.controlindex.wIndex = (WORD)lID;
		if ((GetOilTable()) && CheckOilVatID(pszData))
			g_frameTx.commdata.controlindex.wValue = GetOilVat(lValue);
		else if (m_bInjSpdUnit)		//wangli2014-8-20 16:20:01  inje/holdpre unit is mm/s
		{
			if (CheckDataID(pszData, g_MSET_InjeHoldSpdID, sizeof(g_MSET_InjeHoldSpdID) / sizeof(g_MSET_InjeHoldSpdID[0])))
			{
				if ((lValue != 0xFFFF) && (GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue != 0))
					g_frameTx.commdata.controlindex.wValue = (DWORD)((lValue * 990 / GetDBValue("SYSX_INJECT_INJECT_NULL_MAXFLOW").lValue + 5) / 10 ); //四舍五入
				else
					g_frameTx.commdata.controlindex.wValue = lValue;
			}
			else
				g_frameTx.commdata.controlindex.wValue = lValue;
		}
		else
		{
			g_frameTx.commdata.controlindex.wValue = lValue;
		}

		CMDDATA cmddata;
		//規則: 如果是模組和參數資料，CMD不變，其它資料CMD統一為 0x3133
		//printf("SendControlIndex  index=%x,value=%d\n",g_frameTx.commdata.controlindex.wIndex,g_frameTx.commdata.controlindex.wValue);
		if ((g_frameTx.commdata.controlindex.wIndex == 0x3FE) || (g_frameTx.commdata.controlindex.wIndex == 0x3FD)) //Gwei 2006/2/16 閥門強制輸出,使用WMACHSPECIALRESERVED46特殊參數
		{
			if (!m_bCOM2Vlv) /*COM1閥門*/
			{
				cmddata.wCmd   = DB_OTHER_CMD_ID;
				cmddata.wIndex = g_frameTx.commdata.controlindex.wValue;//將Value存入index傳主機//g_frameTx.commdata.controlindex.wIndex;
			}
			else	if (m_bCOM2Vlv && (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED94").lValue == 0)) /*COM2閥門動力源選擇使用主機1*/
			{
				cmddata.wCmd   = DB_OTHER_CMD_ID;
				if (g_frameTx.commdata.controlindex.wValue > 0)
					g_frameTx.commdata.controlindex.wValue = 2;/*JOYCE2013-6-24 改為閥門2,因為COM1主機借用PC閥門1輸出作為COM2閥門泄壓鍵*/
				cmddata.wIndex = g_frameTx.commdata.controlindex.wValue;//將Value存入index傳主機//g_frameTx.commdata.controlindex.wIndex;
			}
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MACH1_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMACHINE1;
			cmddata.wCmd   = DB_MACH1_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MACH2_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMACHINE2;
			cmddata.wCmd   = DB_MACH2_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MACH1_MAXINDEX_ID;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSET1_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSET1;
			cmddata.wCmd   = DB_MOLDSET1_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MACH2_MAXINDEX_ID;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSET2_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSET2;
			cmddata.wCmd   = DB_MOLDSET2_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MOLDSET1_MAXINDEX_ID;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_POWERMATCH_MAXINDEX_ID)
		{
			cmddata.wCmd   = DB_NEWPOWERMATCH_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MOLDSET2_MAXINDEX_ID + 0x100;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MONIMAX_MAXINDEX_ID)
		{
			cmddata.wCmd   = DB_MONIMAX_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_POWERMATCH_MAXINDEX_ID + 0x8D;/*偏移量(0x119-0x08C)傳給DSP55*/
		}
		else if (g_frameTx.commdata.controlindex.wIndex < 0xA61)
			return;
		else if (g_frameTx.commdata.controlindex.wIndex < 0xAC1)
		{
			cmddata.wCmd   = DB_MONIMAX_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0xA61;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_CONFIG_MAXINDEX_ID)
		{
			return;   /*CONFIG結構體不需要單傳,只需要block傳值方式,因為主機不接受單傳方式,如果HMI送了會導致主機reset標準位功能無效*/
			//cmddata.wCmd   = DB_CONFIG_CMD_ID;
			//cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MONIMAX_MAXINDEX_ID;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < 0x1000)
			return;
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MACHB_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMACHINEB;
			cmddata.wCmd   = DB_MACHB_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1000;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSET1B_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSETB1;
			cmddata.wCmd   = DB_MOLDSETB1_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MACHB_MAXINDEX_ID;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MOLDSET2B_MAXINDEX_ID)
		{
			//g_frameTx.wCmd = COMM_ETH_SENDCONTROLMOLDSETB2;
			cmddata.wCmd   = DB_MOLDSETB2_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - DB_MOLDSET1B_MAXINDEX_ID;
		}
		else if (g_frameTx.commdata.controlindex.wIndex < 0x1430)
			return;
		else if (g_frameTx.commdata.controlindex.wIndex < DB_MONIMAXB_MAXINDEX_ID)
		{
			cmddata.wCmd   = DB_MONIMAXB_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x1430;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex > 0x18B8) && (g_frameTx.commdata.controlindex.wIndex < 0x19B9)) //Andy 20061212
		{
			cmddata.wCmd   = DB_NEWPOWERMATCH_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x18B9;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex == 0x19B9) || (g_frameTx.commdata.controlindex.wIndex == 0x19CA)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x19DB) || (g_frameTx.commdata.controlindex.wIndex == 0x19EC)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x19FD) || (g_frameTx.commdata.controlindex.wIndex == 0x1A0E)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x1A1F) || (g_frameTx.commdata.controlindex.wIndex == 0x1A30)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x1A41) || (g_frameTx.commdata.controlindex.wIndex == 0x1A52)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x1A63) || (g_frameTx.commdata.controlindex.wIndex == 0x1A74)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x1A85) || (g_frameTx.commdata.controlindex.wIndex == 0x1A96)
		         || (g_frameTx.commdata.controlindex.wIndex == 0x1AA7) || (g_frameTx.commdata.controlindex.wIndex == 0x1AB8))
		{
			if (m_bCOM2Vlv) /*JOYCE 20140508 COM2閥門16組中不管哪組選擇使用,都要送1E7的值為1給COM1*/
			{
				for (int i = 0; i < 16; i++)
				{
					char	pDataID[128];
					sprintf(pDataID, "GATE_GATE%d_GATE1_NULL_MODEOPTION", i + 1);
					long lValOpt = GetDBValue(pDataID).lValue;
					if (lValOpt)
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
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x2000) && (g_frameTx.commdata.controlindex.wIndex < 0x211B))
		{
			cmddata.wCmd   = DB_MONIMAX_CMD_ID;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x2000;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x3000) && (g_frameTx.commdata.controlindex.wIndex < 0x3200))
		{
			cmddata.wCmd   = COMM_ETH_SENDCONTROLNEWMACHINE2;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x3000;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x3200) && (g_frameTx.commdata.controlindex.wIndex < 0x3400))
		{
			cmddata.wCmd   = COMM_ETH_SENDCONTROLNEWMOLDSET2_1;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x3200;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x3400) && (g_frameTx.commdata.controlindex.wIndex < 0x3600))
		{
			cmddata.wCmd   = COMM_ETH_SENDCONTROLNEWMOLDSET2_2;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x3400;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x3600) && (g_frameTx.commdata.controlindex.wIndex < 0x3800))
		{
			cmddata.wCmd   = COMM_ETH_SENDCONTROLNEWMACHINE3;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x3600;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x3800) && (g_frameTx.commdata.controlindex.wIndex < 0x3A80))
		{
			cmddata.wCmd   = COMM_ETH_SENDCONTROLNEWMOLDSET3_1;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x3800;
		}
		else if ((g_frameTx.commdata.controlindex.wIndex >= 0x3A80) && (g_frameTx.commdata.controlindex.wIndex < 0x3D00))
		{
			cmddata.wCmd   = COMM_ETH_SENDCONTROLNEWMOLDSET3_2;
			cmddata.wIndex = g_frameTx.commdata.controlindex.wIndex - 0x3A80;
		}
		else if (g_frameTx.commdata.controlindex.wIndex == 0x8826) //wangli2014-9-5 11:26:43 射出曲線畫面的射出速度最大值
		{
			if (m_bInjSpdUnit)		//wangli2014-8-20 16:20:01  inje/holdpre unit is mm/s,進行單位換算
			{
				if (g_ptaskdsp != NULL)
					g_ptaskdsp->WriteValue(COMM_SENDCONTROLMOLDSET);
			}
			else return;

		}
		else
			return;

		if ((g_frameTx.commdata.controlindex.wIndex == 0x3FE) || (g_frameTx.commdata.controlindex.wIndex == 0x3FD))	/*JOYCE20130515閥門強制輸出 D_VALO*/
		{
			if (g_frameTx.commdata.controlindex.wValue == 0)
			{
				cmddata.wValue = 0x8000;
			}
		}
		else
			cmddata.wValue = g_frameTx.commdata.controlindex.wValue;
		cmddata.wEcho  = 0;

		/*
			發送單筆資料方法： 先將單筆資料放入消息隊列，然后再State中一筆一筆的發送。
		*/
		//printf("sendcontrolindex()  cmddata.wCmd=%x cmddata.wIndex=%d lID=%d \n", cmddata.wCmd, cmddata.wIndex, lID);
		if (m_cState == COMM_STATE_RUN && g_wDspStatus == TRUE)
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

/*!	\fn void        CtmDsp54All::SendCommand()
 *	\brief	發送command
 *	\return 0
 */
void        CtmDsp54All::SendCommand(CMDDATA* pcmd)               //St0512'03
{
	int         nWriteCount;
	WORD		wStartID1 = 0x00;
	WORD		wLength = 0;
	//TRASMITDATA	TrasmitData;
	//DBVALUE	db;

	//printf("SendCommand pcmd->wCmd=%x Index=%d value=%d \n", pcmd->wCmd, pcmd->wIndex, pcmd->wValue);
	if (pcmd->wCmd >= COMM_ETH_SENDCONTROLMACHINE1 && pcmd->wCmd <= COMM_ETH_SENDCONTROLMOLDSETB2)
	{
		PushEchoCmd(pcmd);
		g_frameTx.wCmd             = pcmd->wCmd;
		g_frameTx.commdata.cmdnetdata.wCmd = pcmd->wValue;
		wStartID1				   = pcmd->wIndex;
		wLength 				   = 2;
		//printf("write token wStartID1=%d \n", wStartID1);
		WriteToken((BYTE*)&g_frameTx, 2, &nWriteCount, wStartID1);
	}
	else if (pcmd->wCmd >= COMM_ETH_SENDCONTROLNEWMACHINE2 && pcmd->wCmd <= COMM_ETH_SENDCONTROLNEWMOLDSET3_2)
	{
		//James add for test freecore 2014/2/20 19:48:02
		//printf("New Machine and Moldset\n");
		PushEchoCmd(pcmd);
		g_frameTx.wCmd             = pcmd->wCmd;
		g_frameTx.commdata.cmdnetdata.wCmd = pcmd->wValue;
		wStartID1				   = pcmd->wIndex;
		wLength 				   = 2;
		WriteToken((BYTE*)&g_frameTx, 2, &nWriteCount, wStartID1);
	}
	else if (pcmd->wCmd == COMM_ETH_SENDPANEL)
	{
		g_frameTx.wCmd 		= COMM_ETH_SENDPANEL;
		memcpy(g_frameTx.commdata.acBuffer, &pcmd->wValue, sizeof(WORD));
		WriteToken((BYTE*)&g_frameTx, (0 + 2), &nWriteCount);
	}
	//else if(pcmd->wCmd == COMM_SENDTRANSMITDSP28)		//James add 2010/3/2 for transmit
	//{
	//	memset(&TrasmitData, 0, sizeof(TrasmitData));
	//	g_frameTx.wCmd 		= COMM_SENDTRANSMITDSP28;
	//	TrasmitData.wID		= pcmd->wValue;
	//	wIndex				= g_pDatabase->DataIDToIndex(pcmd->wValue);
	//	db 					= g_pDatabase->Read(wIndex);
	//	TrasmitData.bType	= db.DataType.wType;
	//	TrasmitData.bState	= 0;
	//	TrasmitData.lValue	= GetDBValue(wIndex).lValue;
	//	memcpy(g_frameTx.commdata.acBuffer, &TrasmitData, sizeof(TrasmitData));
	//	WriteToken((BYTE*)&g_frameTx, COMM_MAXTRANSIMT_NET, &nWriteCount);
	//	printf("SendComm transmit \n");
	//}
	else if (pcmd->wCmd == COMM_ETH_SENDCOMMAND) /*DSP XPLC上傳資料使用 echo*/
	{
		g_frameTx.wCmd 		= COMM_ETH_SENDCOMMAND;
		g_frameTx.commdata.cmddata.wCmd 	= m_CmdDSPUpData.wEcho;
		g_frameTx.commdata.cmddata.wIndex = m_CmdDSPUpData.wIndex;
		g_frameTx.commdata.cmddata.wValue = m_CmdDSPUpData.wValue;
		g_frameTx.commdata.cmddata.wEcho 	= m_CmdDSPUpData.wEcho;

		//printf("sendcommand wCmd=%x nIndex=%x, wValue=%x\n", g_frameTx.commdata.cmddata.wCmd, g_frameTx.commdata.cmddata.wIndex,
		//g_frameTx.commdata.cmddata.wValue);
		WriteToken((BYTE*)&g_frameTx, sizeof(g_frameTx.commdata.cmddata), &nWriteCount);
	}
	else
	{
		g_frameTx.wCmd             = COMM_ETH_SENDUPDATEDATA;
		//54net DATA發送只需發送 cmd+value
		if (pcmd->wCmd != g_frameTx.wCmd)
		{
			g_frameTx.commdata.cmdnetdata.wCmd 		= pcmd->wCmd;
			g_frameTx.commdata.cmdnetdata.wValue 	= pcmd->wValue;
			wStartID1								= pcmd->wIndex;
			wLength 				   				= sizeof(g_frameTx.commdata.cmdnetdata);
			WriteToken((BYTE*)&g_frameTx, sizeof(g_frameTx.commdata.cmdnetdata), &nWriteCount, wStartID1);
		}
	}
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmDsp54All::GetUpdateData()
{
	/*
		dsp54也會通過update來更新HMI的資料
	*/
	WORD wFirstID = *(WORD*)(g_frameRx.commdata.acBuffer);                  //取出DATA的FirstID
	WORD wLen = *(WORD*)(g_frameRx.commdata.acBuffer + 1 * sizeof(WORD));   //取出DATA的WORD總數
	WORD wCmd = *(WORD*)(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD));		//去除DATA的CMD
	WORD wDataID = 0, wBlockID = 0;
	switch (wCmd)
	{
	//Set
	case	COMM_ETH_SENDCONTROLMACHINE1:		//機器資料
		wBlockID = BLOCK_MACHINE_ID;
		break;
	case	COMM_ETH_SENDCONTROLMACHINE2:
		wBlockID = BLOCK_MACHINE_ID;
		wFirstID += 0x200;
		break;
	case	COMM_ETH_SENDCONTROLMOLDSET1:		//模具資料
		wBlockID = BLOCK_MOLDSET_ID;
		break;
	case	COMM_ETH_SENDCONTROLMOLDSET2:
		wBlockID = BLOCK_MOLDSET_ID;
		wFirstID += 0x280;
		break;
	case	COMM_ETH_SENDCONTROLMACHINEB:		//機器B資料
		wBlockID = BLOCK_MACHINEB_ID;
		break;
	case	COMM_ETH_SENDCONTROLMOLDSETB1:		//模具B資料
		wBlockID = BLOCK_MOLDSETB_ID;
		break;
	case	COMM_ETH_SENDCONTROLMOLDSETB2:
		wBlockID = BLOCK_MOLDSETB_ID;
		wFirstID += 0x180;
		break;
	case 	COMM_ETH_SENDCONTROLNEWMACHINE2:		//James add 0x3337 for free cord
		wBlockID = BLOCK_NEWMACHINE2_ID;
		break;
	case 	COMM_ETH_SENDCONTROLNEWMOLDSET2_1:		//James add 0x3338 for free cord
		wBlockID = BLOCK_NEWMOLDSET2_ID;
		break;
	case 	COMM_ETH_SENDCONTROLNEWMOLDSET2_2:		//James add 0x3339 for free cord
		wBlockID = BLOCK_NEWMOLDSET2_ID;
		wFirstID += 0x200;
		break;
	case 	COMM_ETH_SENDCONTROLNEWMACHINE3:		//James add 0x3430 for machine3
		wBlockID = BLOCK_NEWMACHINE3_ID;
		break;
	case 	COMM_ETH_SENDCONTROLNEWMOLDSET3_1:		//James add 0x3431 for moldset3_1
		wBlockID = BLOCK_NEWMOLDSET3_ID;
		break;
	case 	COMM_ETH_SENDCONTROLNEWMOLDSET3_2:		//James add 0x3432 for moldset3_2
		wBlockID = BLOCK_NEWMOLDSET3_ID;
		wFirstID += 0x280;
		break;
	case	COMM_ETH_SENDMONILIMIT:				//監測上下限
		wBlockID = BLOCK_MONIMAXMIN_ID;
		break;
	case	COMM_ETH_SENDMONBILIMIT:			//監測B上下限
		wBlockID = BLOCK_MONIMAXMINB_ID;
		break;
	//Get
	case	COMM_ETH_GETCONFIG:					//Config資料
		wBlockID = BLOCK_CONFIG_ID;
		break;
	case	COMM_ETH_GETCONFIGLIMIT:			//AD DA 上下限
		wBlockID = BLOCK_CONFIGLIMIT_ID;
		break;
	case	COMM_ETH_GETDAOUTPUT:				//DA輸出
		wBlockID = BLOCK_NEWDAOUT_ID;
		break;
	case	COMM_ETH_GETOPSTATE:				//及時資料
		wBlockID = BLOCK_STATE_ID;
		break;
	case	COMM_ETH_GETOPSTATUS:				//狀態資料
		wBlockID = BLOCK_STATUS_ID;
		break;
	case	COMM_ETH_GETMONICURRENT:			//當前監測資料
		wBlockID = BLOCK_MONICURRENT_ID;
		break;
	case	COMM_ETH_GETMONILAST:				//上一模監測資料
		wBlockID = BLOCK_MONILAST_ID;
		break;
	case	COMM_ETH_GETMONIMAXMIN:				//監測上下限
		wBlockID = BLOCK_MONIMAXMIN_ID;
		break;
	case	COMM_ETH_GETMONICURRENTB:			//監測B上下限
		wBlockID = BLOCK_MONIBCURRENT_ID;
		break;
	case	COMM_ETH_GETMONILASTB:				//上一模監測B資料
		wBlockID = BLOCK_MONIBLAST_ID;
		break;
	case	COMM_ETH_GETMONIMAXMINB:			//監測B上下限
		wBlockID = BLOCK_MONIMAXMINB_ID;
		break;
	default	:
		wBlockID = 0xFFFF;
		break;
	}
	//傳入BlockID 和Block offsetID 來獲得這筆資料的DataID
	wDataID = g_pBlock->GetDataID(wBlockID, wFirstID);
	if (wLen > 200)
	{
		int nTest = 50;
		while (nTest--)
		{
			Beep();
			usleep(100 * 1000);
			//printf("wDataID=%d wLen=%d \n", wDataID, wLen);
		}
	}
	if (wBlockID >= 0 && wDataID >= 0 && wLen > 1 && wLen < 200)		//James add 2013/5/8 13:16:44
	{
		if (SaveDataToDB(g_frameRx.commdata.acBuffer + 3 * sizeof(WORD), (wLen - 1)*sizeof(WORD), wDataID)) //寫入資料庫
			Echo(COMM_ECHO_SUCCESS);
		else
			Echo(COMM_ECHO_FAILURE);
	}
	else
	{
		Echo(COMM_ECHO_FAILURE);
	}

}

void    CtmDsp54All::GetReadData()
{
	int	nNewCmd = 0;
	Echo(COMM_ECHO_SUCCESS);
	WORD wTempCmd = *(WORD *)(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD));
	nNewCmd = wTempCmd + 0xFF0000;
	//  printf("wTempCmd = %x\n", wTempCmd);         //取出DATA，即請求讀的CMD
	ComposeProtocol(nNewCmd, NULL,	NULL, 0, NULL);
}

/*!	\fn void	CtmDsp54All::GetColdStart()
 *	\brief	冷開機(停止原來的接收，重新開始傳送與接收)
 *	\return
 */
void	CtmDsp54All::GetColdStart()
{
	Echo(COMM_ECHO_SUCCESS);
	m_bColdStartFlag = TRUE;/*JOYCE2013-9-12*/
	u_nMicroAdjCount	= 0;					//LEO20070621
	u_nAutoAdjCount 	= 0;					//LEO20070621
	//SendZeroSet();
	m_cState = COMM_STATE_RESET;
	m_cStateRx &= ~COMM_STATE_WAIT00;

	//Set the cmddate at the beginning of.
	m_CmdQueue.wInput     = m_CmdQueue.wOutput = 0;
	m_CmdQueue.wQueueEnd  = MAX_COMMAND_KEEP;
	m_CmdQueue.bQueueFull = FALSE;

	m_CmdEchoQueue.wInput     = m_CmdEchoQueue.wOutput = 0;
	m_CmdEchoQueue.wQueueEnd  = MAX_COMMAND_KEEP;
	m_CmdEchoQueue.bQueueFull = FALSE;
	g_wDspStatus		= FALSE;
	m_bTransmitCmdEcho	= TRUE;
	g_flMotorStatus  	= 0;
	g_flHeaterStatus 	= 0;
	g_flHeaterStatusB = 0;

	if (!m_bDC) /*JOYCE20130515閥門強制輸出 D_VALO*/
	{
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED90", 0);	//all valve opt
		SetDBValue("MSET3_GATE_OPEN_NULL_OUTPUT1", 0);	//each valve opt
	}
}

/*!	\fn void	CtmDsp54All::GetWarmStart()
 *	\brief	暖開機(暫時不使用)
 *	\return
 */
void	CtmDsp54All::GetWarmStart()
{
	//=======
	//      Special control process
	//=======
	Echo(COMM_ECHO_SUCCESS);
	u_nMicroAdjCount	= 0;					//LEO20070621
	u_nAutoAdjCount		= 0;					//LEO20070621
	g_WarmStartFlag		= TRUE; 					//Gwei 2005/5/20　給診斷用，可重新送需求值給主機
	g_wDspStatus		= TRUE;
	if (g_flMotorStatus)
	{
		PanelMotorOn();
	}
	else
	{
		PanelMotorOff();
	}
	if (m_b28Clmp)
	{
		if (GetDBValue("M3_CONF_CARD_LOCAL_CONFIG_ID").lValue)
			SetDBValue("M3_CONF_CARD_LOCAL_CONFIG_ID", 0);
	}

	//=======
	//       Normal process
	//=======
	m_cState 	= 	COMM_STATE_RUN;
	m_cStateRx 	&= 	~COMM_STATE_WAIT00;

//	UpdateProduceStatus(MANUFACTSTAT_HALTMACHINE);//zholy070425
}

/*!	\fn void	CtmDsp54All::GetConfig()
 *	\brief	config資料
 *  sizeof = 2C0H
 *	\return
 */
void  CtmDsp54All::GetConfig()
{
	Echo(COMM_ECHO_SUCCESS);
	DWORD		dwValue, dwValue1, dwValue2;
	char*		pszID1 = "CONF_CODE_WRITE_NULL_DATA15";
	char*		pszID2 = "CONF_TMP1_HEATERON_NULL_MAXDEGREE";
	/*******
	|       Ignore the error configuration
	*******/
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_CONFIG_ID, pszID1, &dwValue1);
	g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_CONFIG_ID, pszID2, &dwValue2);

	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_CONFIG_ID, FALSE);

	if (m_bSysReset)
	{
		if (g_pDefault != NULL)
			g_pDefault->SetDefault();
		m_bSysReset = FALSE;
		SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION", 0);		//20071120 add解決每次重啟均重置那幾筆特殊的資料
		if (GetOilTable()) SetVatDefPosi();
		SetDefaultEffectPos();//JOYCE2011-8-26
	}

	if (!m_bGotConfig)    											//<WY2002/11/28
	{
		m_bGotConfig = TRUE;										//wy040600
		//SendControlIndex("MACH_TMP1_HEATERON_WARM_OPTION");		//James mark 2009/6/10
		if (GetOilTable())
		{
			/*JOYCE2013-9-12 mark,使用單傳指令傳輸35筆資料,導致開機時key數值,傳DSP很慢;如果DSP有重啟,SendControlMoldset()中有處理推力座換算,所以可以取消此處理方式,改為只有HMI單獨重啟時才執行
			dwValue = GetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION").lValue;
			if (!dwValue) SendOilVat();
			SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION", FALSE, FALSE);
			*/
			if (m_bColdStartFlag == FALSE)		SendOilVat(); /*JOYCE2013-9-12 HMI單獨重啟時,維持此處理方式,保証HMI/DSP資料匹配*/
		}

		/* //JOYCE2012-1-17  修正中子有效區在重置首次啟動出現錯誤問題*/
		long long   opposite   = OilToPosi(GetDBValue("CONF_CODE_READ_CHL2_MAXPOSITION").lValue);
		long long   llOpeMaxPos = GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue;
		if ((llOpeMaxPos > opposite) && (opposite != 0))
			SetDBValue("MSET_MLD_OPEN_STEP5_POSITION", opposite, FALSE);

		SetCoreEffectPN(FALSE);
		SetEjectEffectPN(FALSE);                 //Chuntzu 2004/1/12 add for Eject Pos Effect Area

		if (m_b28Clmp) /*wangli2015-8-11 16:23:46 首次啟動 開關模尺 55資料送28*/
		{
			WORD temp = (WORD)GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
			int i = 1;
			WORD	PrecisionDate = (WORD)GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
			long long	visData = (long long)(HEX2BCD(temp) * 10 + (PrecisionDate / (0x0001 << (i * 4))) % 0x0010);
			SetDBValue("M3_CONF_CARD_LOCAL_CONFIG_MAX_RANGE_AD2", 10 * visData);
		}


	}

	if (GetDBValue("CONF_CODE_WRITE_NULL_W0BB0").lValue != 1)
		TempAutRecog(); /*wangli2016-1-21 15:12:53 溫度自動識別處理*/

	Check_PCTBL(); /*joyce2012-12-19  PC Table Check; IO交換點后需重新獲取*/
	Check_PBTBL();

	dbgppc[0] = 0;             					//wy010902 add pc over 48 points.
	dbgppc[1] = 1;
	dwValue	= GetDBValue("CONF_CODE_WRITE_NULL_DATA13").lValue;
	if ( (dwValue == 0x280) ||
	        (dwValue == 0x2800) ||                  //<<<Anny2002/6/10 add
	        (dwValue == 0x3800) ||                  //<<<Steven2002/7/8 add
	        (dwValue == 0x5800) ) dbgppc[2] = 2;    //<<<Steven2003/4/28 add
	else                      dbgppc[2] = 3;
	dbgppc[3] = 0;
	g_pDatabase->ComputeLimitValue(NULL);		//Yang 2006/12/29
}

/*!	\fn void	CtmDsp54All::GetConfigLimit()
 *	\brief	configlimit資料
 *  sizeof = 40H
 *	\return
 */
void  CtmDsp54All::GetConfigLimit()
{
	Echo(COMM_ECHO_SUCCESS);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_CONFIGLIMIT_ID, FALSE);
}

/*!	\fn void	CtmDsp54All::GetDAOutput2()
 *	\brief	獲得主機DA資料
 *	\return
 */
void CtmDsp54All::GetDAOutput2()     // Andy 20061212 for New D/A
{
	Echo(COMM_ECHO_SUCCESS);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_NEWDAOUT_ID);

	//Andy 20070314
	//m_bGotDA = TRUE;
	//m_bNewDA = TRUE;  				//Andy 20061212 for New D/A flag!
	//if (m_cState != COMM_STATE_RUN)
	//    SendControlMachine(1);
	//else
	SendPowerMatch();
}

void  CtmDsp54All::GetAustonDriveInfo()
{
	//printf("GetAustonDriveInfo g_ptaskdsp28=%x \n", g_ptaskdsp28);

	Echo(COMM_ECHO_SUCCESS);
	WORD wLength = 0;

	//printf("GetAustonDriveInfo \n");
	if (1 == CtmConfig::GetInstance()->GetAustoneFlag() || 1 == CtmConfig::GetInstance()->GetN2CFlag() || 1 == CtmConfig::GetInstance()->GetM3Flag())		//新Austone通訊
	{
		if (g_ptaskdsp28_Austone != NULL)
		{
			wLength = *(WORD*)((BYTE*)g_frameRx.commdata.acBuffer + 6);
			g_ptaskdsp28_Austone->WriteValue(COMM_GETAUSTIONDRIVEINFO, g_frameRx.commdata.acBuffer + 4, wLength);
		}
		if (g_ptaskdsp28_N2C != NULL)
		{
			wLength = *(WORD*)((BYTE*)g_frameRx.commdata.acBuffer + 6);
			g_ptaskdsp28_N2C->WriteValue(COMM_GETAUSTIONDRIVEINFO, g_frameRx.commdata.acBuffer + 4, wLength);
		}
		if (g_ptaskdsp28_M3 != NULL)
		{
			wLength = *(WORD*)((BYTE*)g_frameRx.commdata.acBuffer + 6);
			//printf("GetAustonDriveInfo()     wLength = %d\n", wLength);
			g_ptaskdsp28_M3->WriteValue(COMM_GETAUSTIONDRIVEINFO, g_frameRx.commdata.acBuffer + 4, wLength);
		}

		//WORD wType=*(WORD*)((BYTE*)g_frameRx.commdata.acBuffer+8);
		//switch(wType)
		{
			//case CONST_RES_LINESTATUS:
			//case CONST_RES_MOLDSET:
			//case CONST_RES_MOLDSETSTATUS:
			//m_bTransmitCmdEcho = TRUE;	//20140526
			//PopTransmitCount(1);        //20140526
			//add by J.Wong 覃淕彶善28Echo 婬蛌楷勦蹈爵腔訧蹋 2016/8/23 16:02:56
			m_nNum = 0;
			if (PopTransmitCmd(&m_TransmitBuffer))
			{

				usleep(2000);
				SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT_NET);
				//m_bTransmitCmdEcho = FALSE;
			}
			//break;
			//default:
			//	break;
		}
	}
	else if (g_ptaskdsp28 != NULL)
	{
		g_ptaskdsp28->WriteValue(COMM_GETAUSTIONDRIVEINFO, g_frameRx.commdata.acBuffer + 4, COMM_MAXTRANSIMT_NET);
	}
//	m_bTransmitGetDataFlag = TRUE;
	/*	memcpy((char *)&m_dbAustonDriveInfo, g_frameRx.commdata.acBuffer+2*sizeof(WORD), sizeof(tmDB_AUSTONDRIVEINFO));
		//printf("1 1=%d 2=%d 3=%f 4=%f 5=%f 6=%f 7=%f 8=%f \n", m_dbAustonDriveInfo.wOptState, m_dbAustonDriveInfo.wAlarmNub, m_dbAustonDriveInfo.fMotorSpeed,
		//m_dbAustonDriveInfo.fMotorTorqueForce, m_dbAustonDriveInfo.fDriverFlow, m_dbAustonDriveInfo.fDriveBusVoltage,
		//m_dbAustonDriveInfo.fDriveTemper, m_dbAustonDriveInfo.fMotorTemper);
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
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED7", &nTempValue, FALSE);
		nTempValue= (int)(m_dbAustonDriveInfo.fMotorTemper*10);
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED8", &nTempValue, FALSE);
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
		g_pDatabase->Write("SYSX_AUSTON_OTHERS_NULL_RESERVED22", &nTempValue, FALSE);*/
}

/*!	\fn void	CtmDsp54All::GetOPState()
 *	\brief	操作狀態
 *  size = 60H
 *	\return
 */
void  CtmDsp54All::GetOPState()
{
	//static	int nNum = 0;
	long	lDsp28CommState = 0;
	//DWORD	dwTempTime = 0;
	//printf("GetOPState g_frameRx.wCmd=%x \n", g_frameRx.wCmd);
	Echo(COMM_ECHO_SUCCESS);
	//dwTempTime = g_frameRx.commdata.status.dwActingTime[0];
	g_frameRx.commdata.status.dwActingTime[0] = g_frameRx.commdata.status.dwActingTime[1];
	//g_frameRx.commdata.status.dwActingTime[0] = g_frameRx.commdata.status.dwActingTime[1];	//James add
	m_nNum ++;
	//dwValue = GetDBValue("STATE_OTHR_OTHR_STEP1_REALTIME").lValue;
	//g_pBlock->SetBlockValue(g_frameRx.commdata.acBuffer+2*sizeof(WORD), BLOCK_STATE_ID, "STATE_OTHR_OTHR_STEP1_REALTIME", dwValue);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_STATE_ID, FALSE);

	if (m_cState != COMM_STATE_RUN)	//如果有及時資料傳輸，就說明通訊正常
		m_cState = COMM_STATE_RUN;
	g_flMotorStatus 	= GetDBValue("STATE_OTHR_OTHR_NULL_MOTOR").lValue;//dwValue;
	//cout << "g_flMotorStatus=" << g_flMotorStatus << endl;
	g_flHeaterStatus  	= GetDBValue("STATE_OTHR_OTHR_NULL_HEATER").lValue;//dwValue;
	g_flHeaterStatusB  = GetDBValue("STATE_OTHR_OTHR_NULL_MLDHTR").lValue;//dwValue;

	lDsp28CommState 	= GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;	//STNC.SLV_TALK  james add 2010/3/10 判斷dsp28是否連線

	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED6", GetDBValue("STATE_HEATER_WARM_CNTDOWN_TIME").lValue);/*JOYCE2014-10-16 防冷啟動時間,倒計時*/
	g_wOperationCount = LOWORD(GetDBValue("STATE_OTHR_OTHR_STEP4_REALTIME").lValue);//操作計數
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED10", GetDBValue("STATE_OTHR_OTHR_QMC_MCSENA").lValue, FALSE);          /*HS_STAT_MCS(E70  QMC歐規磁力模板)*/

	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED81", GetDBValue("STATE_TBAR1_FORWARD_TIME").lValue, FALSE); /*純二板機, 前上拉杆進計時 監測值  m_bSVRT*/
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED82", GetDBValue("STATE_TBAR2_FORWARD_TIME").lValue, FALSE); /*純二板機,前下拉杆進計時 監測值  m_bSVRT*/
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED83", GetDBValue("STATE_TBAR3_FORWARD_TIME").lValue, FALSE); /*純二板機,后上拉杆進計時 監測值  m_bSVRT*/
	SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED84", GetDBValue("STATE_TBAR4_FORWARD_TIME").lValue, FALSE); /*純二板機,后下拉杆進計時 監測值  m_bSVRT*/

	if (g_wDspStatus == FALSE)
	{
		g_wDspStatus		= TRUE;
		//m_bTransmitCmdEcho 	= TRUE;
		if (g_ptaskdsp28 != NULL && lDsp28CommState != 0) //James add 當dsp54和dsp28準備就緒才可發送，否則buff會增加
			g_ptaskdsp28->WriteValue(COMM_SENDTRANSMITBLOCK, (void*)NULL, 0);
	}
	g_wDspStatus	= TRUE;

	//printf("lDsp28CommState=%d \n", lDsp28CommState);
	//if(g_ptaskdsp28 != NULL)
	//{
	//	g_ptaskdsp28->WriteValue(COMM_SENDTRANSMITSTATE, (void*)&lDsp28CommState, 0);
	//}

	if (m_wCmdState == CMD_SEND)		//如果當前狀態還沒有收到Echo回復，就重新發送
	{
		m_nCmdNull++;
		//printf("m_wCmdState=%d m_nCmdNull=%d \n", m_wCmdState, m_nCmdNull);
		if (m_nCmdNull > 15 )			//如果超過5次及時資料還沒有收到Echo， 就認為沒收到
		{
			if (PopEchoCmd(&m_CmdEchoData))
			{
				m_wCmdSend  = m_CmdEchoData.wCmd;
				m_wCmdState = CMD_SEND;
				m_nCmdNull  = 0;
				PushCmd(&m_CmdEchoData);
			}
			else
			{
				m_nCmdNull = 0;
			}
			switch (m_wCmdSend)
			{
			case COMM_ETH_SENDCONFIG:
				m_nCmdNull = 0;
				SendConfig();
				break;
			default:
				break;
			}
		}
	}

	//printf("lDsp28CommState=%d \n", lDsp28CommState);
	//松開按鍵如果沒有收到，就馬上重新發送
	if (m_CmdPanelBreak.wCmd == COMM_ETH_SENDPANEL)
	{
		m_wCmdState = CMD_SEND;
		m_nCmdNull  = 0;
		SendCommand(&m_CmdPanelBreak);
	}
	//if(nNum >= 2)			//超時重新發送
	//{
	//	m_bTransmitCmdEcho = TRUE;
	//}

	//每次收到一個state，就發送一筆單筆資料
	if (PopCmd(&m_CmddData))
	{
		m_wCmdSend  = m_CmddData.wCmd;
		m_wCmdState = CMD_SEND;
		m_nCmdNull  = 0;
		SendCommand(&m_CmddData);
		//nNum = 0;
	}
	else if (lDsp28CommState > 0  /*&& m_nNum >=1 && !m_bTransmitCmdEcho*/)		//間隔240ms 才轉發一次資料給dsp28
	{
		if (PopTransmitCmd(&m_TransmitBuffer))
		{
			usleep(2000);
			//printf("m_nNum=%d lDsp28CommState=%d m_bTransmitCmdEcho=%d \n", m_nNum, lDsp28CommState, m_bTransmitCmdEcho);
			SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT_NET);
			//m_bTransmitCmdEcho = FALSE;   //20140526
			//printf("time out \n");
			//PopTransmitCount(1);						//20140526
			//m_bTransmitCmdEcho = TRUE;			//20140526
		}
		m_nNum = 0;
	}

	/*DSP XPLC上傳資料*/
	DWORD	dwValue = 0, dwCmd = 0, dwDataID = 0, dwIndex = 0, dwDBValue = 0;
	dwCmd			= GetDBValue("STATE_OTHR_OTHR_NULL_COMMANDDATA").lValue;
	dwDataID	= GetDBValue("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").lValue;
	dwValue		= GetDBValue("STATE_OTHR_OTHR_NULL_UPDATEDATA").lValue;
	//printf("dwCmd=%4x , dwDataID=%4x ,dwValue=%4x\n",dwCmd, dwDataID, dwValue);

	if (dwCmd == DB_DSPUP_MACH_CMD_ID)
	{
		if (dwDataID < DB_MACH2_MAXINDEX_ID)                                                       //BW20030326 for not write to unsafe address
		{
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
			dwDBValue = GetDBValue(dwIndex).lValue;
			if (dwDBValue != dwValue)  //BW20030307 Add for data save
			{
				SetDBValue(dwIndex, dwValue, FALSE);
			}
			if (dwDataID == 0x01AB)
			{
				if (dwValue == 2) /*wangli2015-12-10 17:23:23 累計非開關模狀態時間 清0*/
				{
					//  SetDBValue("MACH_LUB_ON_NULL_W019F",0);
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56", 0);
					SaveNotclmpTime();
				}

			}
			if (dwDataID == 0x01AC)
			{
				if (dwValue == 2) /*wangli2015-12-10 17:23:23 累計非開關模狀態時間 清0*/
				{
					//  SetDBValue("MACH_LUB_ON_NULL_W019F",0);
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57", 0);
					SaveNotclmpTime();
				}

			}
			if (dwDataID == 0x0190) /*wangli2016-1-20 9:20:57自動潤滑計數 ，0 表示 已潤滑成功*/
			{
				if (dwValue == 0)
				{
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56", 0); /*累計非開關模狀態時間 清0*/
					SaveNotclmpTime();
				}
			}

			if (dwDataID == 0x019A) /*wangli2016-1-20 9:20:57自動潤滑計數 ，0 表示 已潤滑成功*/
			{
				if (dwValue == 0)
				{
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57", 0); /*累計非開關模狀態時間 清0*/
					SaveNotclmpTime();
				}
			}
		}
	}
	else if (dwCmd == DB_DSPUP_MOLDSET_CMD_ID)
	{
		if (dwDataID < 0x500)                                                       //BW20030326 for not write to unsafe address
		{
			dwDataID = dwDataID + DB_MACH2_MAXINDEX_ID;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
			dwDBValue = GetDBValue(dwIndex).lValue;
			if (dwDBValue != dwValue)  //BW20030307 Add for data save
			{
				if (GetOilTable() && CheckOilVatID(g_pDatabase->GetString(dwIndex)))
					SetDBValue(dwIndex, OilToPosi(dwValue), FALSE);
				else
					SetDBValue(dwIndex, dwValue, FALSE);
			}

			if (m_bSVRT) /*JOYCE2014-1-17 */
			{
				DWORD  dwOffSetTotCnt = 0; /*zhongjw 2015-4-13 15:49:37偏差總計數*/
				WORD   wNCnt = 0;
				if (dwDataID == 0x0833)
				{
					//printf("dwValue=%d\n",dwValue);
					if (dwValue == 1) /*自動調模下,合模尺自動歸零,DSP上傳flag給HMI,由HMI進行歸零動作*/
					{
						SetDBValue("MSET_OTHR_OTHR_NULL_W0834", 0);/*FLAG資料值先清零 給DSP*/
						SetDBValue("MSET_MLD_CLOSE_PROTECT_POSITION", 5);/*JOYCE2013-11-14 在做歸零動作時（包括自動調模）,需要把關模低壓終的值給定為默認值*/

						long long lTemp = 0, lTemp1 = 0, lTemp2 = 0, lTemp3 = 0;
						if (g_pExTable->GetTableValue(0, "D_DIGITAL") || m_b28Clmp) /*開關模使用數位尺*/
							lTemp = GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue;
						else	                                      /*開關模使用模擬尺*/
						{
							long LengthTemp1 = (WORD)GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
							long LengthTemp2 = (WORD)GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
							long absolut = (long long )(HEX2BCD(LengthTemp1) * 10 + (LengthTemp2 / (0x0001 << 4)) % 0x0010);
							if ((GetDBValue("MACH_AD1_WRITE_NULL_W0238").lValue > 0) && (GetDBValue("MACH_AD1_WRITE_NULL_W0238").lValue != 65535))
								lTemp = (GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue * absolut) / (65535 * 1000 / GetDBValue("MACH_AD1_WRITE_NULL_W0238").lValue); /*模座的絕對位置=（絕對位置*尺長）/65535*/
							else
								lTemp = (GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue * absolut) / 65535; /*模座的絕對位置=（絕對位置*尺長）/65535*/
							//printf("absolut=%d   %d\n",absolut, GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue);
							//printf("lTemp=%d\n",lTemp);
						}
						SetDBValue("MSET_SCRW_INJ_NULL_W0775", lTemp);  /*模座位置 抱閘目標位置: 記錄絕對位置*/

						lTemp2 = GetDBValue("METER_AD1_READ_CHL2_ABSOLUTEPOSITION").lValue;
						if (m_b28Clmp)   /*JOYCE2015-8-14 注吹機28控制開關模, 歸零點28已經歸過了，55抓的是28送的位置; 如果55的歸零點記住, 55會重複再減一遍歸零點,所以不送主機*/
							SetDBValue("MACH_AD1_WRITE_CHL2_HOMEPOSITIONOFFSET", lTemp2, FALSE);  /*合模尺歸零點*/
						else
							SetDBValue("MACH_AD1_WRITE_CHL2_HOMEPOSITIONOFFSET", lTemp2);  /*合模尺歸零點*/
						SetDBValue("MSET_PRODUCT_PRODUCT_NULL_W070F", lTemp2); /*JOYCE2014-1-22 模座的歸零位置需要按模具參數處理,保存和讀取模具資料*/

						SetClmpShutOffset();/*計算  模座位置抱閘偏差*/
						SetTBarShutDest();  /*計算  拉杆的抱閘目標位置*/
						SaveTBARData();

						/*JOYCE2014-5-4 當前模具厚度=E(抱閘目標的模座絕對位置) + X(合模機械最小位置) - A(磁性模板厚度) - C(模板絕對位置)*/
						lTemp1 = lTemp + GetDBValue("MACH_MDH_MDH_NULL_W002D").lValue - GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue - GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED87").lValue;
						if (lTemp1 < 0)
							lTemp1 = 0;
						SetDBValue("MSET_OTHR_OTHR_NULL_W083C", lTemp1);

						/*主機不使用這些數據,可只在畫面中進行刷新*/
//									 /*20140504 實際模板距離=開模位置(模座尺實際值) + A(磁性模板厚度) + B(當前模具厚度)*/
//									lTemp1 = GetDBValue("METER_AD1_READ_CHL2_RELATIVEPOSITION").lValue + GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue + GetDBValue("MSET_OTHR_OTHR_NULL_W083C").lValue;
//									if(lTemp1 < 0)
//										lTemp1 = 0;
//									SetDBValue("MSET_OTHR_OTHR_NULL_W083D", lTemp1, FALSE);
//
//									lTemp2 = lTemp1 - GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue;/*實際模厚距離=【實際模板距離】-【磁性模板厚度】*/
//									if(lTemp2 < 0)
//										lTemp2 = 0;
//									SetDBValue("MSET_OTHR_OTHR_NULL_W083E", lTemp2, FALSE);

						/*20131104 開模行程上限值 = D開關模尺 + X合模機械最小位置 -（B當前模具厚度 + A磁性模板厚度）*/
						WORD	ntemp = 0;
						WORD  nPrecisionDate = 0;
						long long lMeterLength = 0, lMeterTravel;

						ntemp = (WORD)GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
						nPrecisionDate = (WORD)GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
						lMeterLength = (long long)(HEX2BCD(ntemp) * 10 + (nPrecisionDate / (0x0001 << 4)) % 0x0010); /*電子尺長*/

						lMeterTravel = (long long)((HEX2BCD)(GetDBValue("CONF_AD1_WRITE_CHL2_TRAVEL").lValue) * 10); /*電子尺機械行程*/

						if (lMeterLength < lMeterTravel) 	/*電子尺機械行程  電子尺長較小值*/
							ntemp = lMeterLength;
						else
							ntemp = lMeterTravel;
						lTemp3 = ntemp + GetDBValue("MACH_MDH_MDH_NULL_W002D").lValue - GetDBValue("MSET_OTHR_OTHR_NULL_W083C").lValue - GetDBValue("MSET_OTHR_OTHR_NULL_W083B").lValue;
						if ((lTemp3 < GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue) && (lTemp3 > 0))
							SetDBValue("MSET_MLD_OPEN_STEP5_POSITION", lTemp3);
						SetDBValue("MSET_OTHR_OTHR_NULL_W0834", 1); /*歸零完成后,再傳FLAG資料值2給DSP*/
						//printf("getopstate test1\n");

						if (!m_b28Clmp)   /*JOYCE2015-8-14 注吹機28控制開關模, 歸零點28已經歸過了，55抓的是28送的位置; 如果55的歸零點記住, 55會重複再減一遍歸零點*/
						{
							if (g_ptaskdsp != NULL)
								g_ptaskdsp->WriteValue(COMM_SENDZEROSET);
						}
						//printf("getopstate test2\n");
					}
					else	if (dwValue == 2)	/*JOYCE2013-10-28 自動下,拉杆過沖,主機判斷到不在目標位置,所以重新計算*/
					{
						SetTBarShutDest2();  /*計算  拉杆的抱閘目標位置*/
					}
				}

				else if (dwDataID == 0x084C)
				{
					if (dwDBValue < dwValue)
					{
						wNCnt = dwValue - dwDBValue;
						dwOffSetTotCnt =  GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED99").lValue;
						dwOffSetTotCnt += wNCnt;
						SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED99", dwOffSetTotCnt);
						SaveParallelData();
					}
				}

				else if (dwDataID == 0x084E)
				{
					if (dwDBValue < dwValue)
					{
						wNCnt = dwValue - dwDBValue;
						dwOffSetTotCnt =  GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED100").lValue;
						dwOffSetTotCnt += wNCnt;
						SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED100", dwOffSetTotCnt);
						SaveParallelData();
					}
				}

				else if (dwDataID == 0x0850)
				{
					if (dwDBValue < dwValue)
					{
						wNCnt = dwValue - dwDBValue;
						dwOffSetTotCnt =  GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED101").lValue;
						dwOffSetTotCnt += wNCnt;
						SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED101", dwOffSetTotCnt);
						SaveParallelData();
					}
				}
				else if (dwDataID == 0x853 || dwDataID == 0x854 || dwDataID == 0x855 || dwDataID == 0x856)
				{
					SaveParallelData();
				}
				else if (dwDataID == 0x852)
				{
					if (dwValue == 1)
					{
						//cout << "come in!!!!!!!" << endl;
						SaveTBarParalleltData();
					}
				}
				else if (dwDataID == 0x68E) /*zhongjw 2015-12-23 模座組態畫面，鎖模壓力監測燈警報次數不可重置*/
				{
					SaveWarningCnt();
				}

			}
		}
	}
	else if (dwCmd == DB_DSPUP_MACHB_CMD_ID)                           //Steven 2004/2/11
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
	else if (dwCmd == DB_DSPUP_MOLDSETB_CMD_ID)                           //Steven 2004/2/11
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
	else if (dwCmd == DB_DSPUP_MACH2_CMD_ID)
	{
		if (dwDataID < 0x200)
		{
			dwDataID = dwDataID + 0x3000;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
			dwDBValue = GetDBValue(dwIndex).lValue;
			if (dwDBValue != dwValue)
				SetDBValue(dwIndex, dwValue, FALSE);
		}
	}
	else if (dwCmd == DB_DSPUP_MOLDSET2_CMD_ID)
	{
		if (dwDataID < 0x400)
		{
			dwDataID = dwDataID + DB_NEWMACH2_MAXINDEX_ID;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
			dwDBValue = GetDBValue(dwIndex).lValue;
			if (dwDBValue != dwValue)
				SetDBValue(dwIndex, dwValue, FALSE);
		}
	}
	else if (dwCmd == DB_DSPUP_MACH3_CMD_ID)
	{
		if (dwDataID < 0x200)
		{
			dwDataID = dwDataID + 0x3600;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
			dwDBValue = GetDBValue(dwIndex).lValue;
			if (dwDBValue != dwValue)
				SetDBValue(dwIndex, dwValue, FALSE);
		}
	}
	else if (dwCmd == DB_DSPUP_MOLDSET3_CMD_ID)
	{
		if (dwDataID < 0x500)
		{
			dwDataID = dwDataID + DB_NEWMACH3_MAXINDEX_ID;
			dwIndex = g_pDatabase->DataIDToIndex(dwDataID);
			dwDBValue = GetDBValue(dwIndex).lValue;
			if (dwDBValue != dwValue)
				SetDBValue(dwIndex, dwValue, FALSE);
		}
	}

	m_CmdDSPUpData.wCmd = COMM_ETH_SENDCOMMAND;
	m_CmdDSPUpData.wEcho = dwCmd;
	SendCommand(&m_CmdDSPUpData);
//
//   			else if(lDsp28CommState > 0 && nNum >=2 && m_bTransmitCmdEcho)		//間隔240ms 才轉發一次資料給dsp28
//   			{
//   				if(PopTransmitCmd(&m_TransmitBuffer))
//   				{
//   					SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT_NET);
//   					m_bTransmitCmdEcho = FALSE;   //20140526
//   					//PopTransmitCount(1);						//20140526
//   					//m_bTransmitCmdEcho = TRUE;			//20140526
//   				}
//   				nNum = 0;
//   			}
//

// 	if(m_bAmmeter)		//fuxy, 2012-5-17, 電表能耗,  記錄自動開始時的能耗
//	{
//		//printf("GetOPState:%d, %d  \n", GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue, wOperationModeOld);
//		if(wOperationModeOld != GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue)
//		{
//			if((GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue == 3)||(GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue == 5)||(GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue == 9))
//			{
//				ProcessAmmeterEnergy();
//			}
//			wOperationModeOld = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;
//		}
//	}

	DWORD lInjeTimeValue = 0;
	long long lInjeState = GetDBValue("STATE_OTHR_OTHR_NULL_COMMANDDATA").lValue;
	if (lInjeState == 0x0011)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[0] = (lInjeTimeValue << 16) | (g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if (lInjeState == 0x0012)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[1] = (lInjeTimeValue << 16) | (g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if (lInjeState == 0x0013)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[2] = (lInjeTimeValue << 16) | (g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if (lInjeState == 0x0014)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[3] = (lInjeTimeValue << 16) | (g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if (lInjeState == 0x0015)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[4] = (lInjeTimeValue << 16) | (g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if (lInjeState == 0x0016)
	{
		lInjeTimeValue = g_pDatabase->Read("STATE_OTHR_OTHR_NULL_UPDATEDATA").wData;;
		m_dwInjectTime[5] = (lInjeTimeValue << 16) | (g_pDatabase->Read("STATE_OTHR_OTHR_NULL_INDEXADDRESSDATA").wData);
	}
	else if (lInjeState == 0x0010)
	{
		for (int i = 0; i < 6; i++)
		{
			m_dwInjectTime[i] = 0;
		}
	}

	if (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue != ID_VIEW_PANEL)    //BW0211'03 add for LED test
	{
		//if (dbbuff.wOperationMode != GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue)  /*JOYCE2016-5-26 解決3354手動燈有時不會亮的問題*/
		//  {
		dbbuff.wOperationMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue;
		if (dbbuff.wOperationMode)
		{
			if ( _TestBit(dbbuff.wOperationMode, 0) &&
			        (dbbuff.cIndexMode == (char)tmMODE_MANUAL))   //Sunny<20070909> amend 0表示時間自動
			{
				dbbuff.bSwitchToAuto = TRUE;
				dbbuff.wSwitchToAutoShutCount = 0;
			}
			dbbuff.cIndexMode = _ScanBitR(dbbuff.wOperationMode & 0x0FFF);/*JOYCE2012-6-19 過濾高4個bit-非動作狀態*/
			if (dbbuff.cIndexMode == 1) PanelSemiAutoOn();
			else if (dbbuff.cIndexMode == 2)
			{
				if (GetDBValue("MSET_OTHR_OTHR_NULL_PHOTOSENSOROPTION").lValue == 1) //如果電眼自動選擇1
					PanelTimeAutoOn();
				PanelSensorAutoOn();
			}
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

			if (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED76").lValue) /*wangli2015-4-14 14:03:39 自動監測回手動狀態關閉*/
				SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 0);
		}
		//  }
		if (dbbuff.flHeaterStatus != g_flHeaterStatus)
		{
			dbbuff.flHeaterStatus = g_flHeaterStatus;
			if (g_flHeaterStatus)	PanelHeaterOn();
			else					PanelHeaterOff();
		}
		if (dbbuff.flMotorStatus != g_flMotorStatus)           //WY071299 ADD
		{
			dbbuff.flMotorStatus = g_flMotorStatus;
			if (g_flMotorStatus == 0xFFFF )  //WY030599
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
		if (wMoldHeaterStatusOld != g_flHeaterStatusB) /*wangli2016-5-30 10:54:41 模溫*/
		{
			wMoldHeaterStatusOld =  g_flHeaterStatusB;
			if (g_flHeaterStatusB)
			{
				_ActLed(LED_PNL_TM_Spare15); /*wangli2016-5-30 13:36:07 模溫開*/
			}
			else
			{
				_ActLed(LED_PNL_TM_Spare16);/*wangli2016-5-30 13:36:43 模溫關*/
			}
		}
	}

	ClearNotclmptime();
	SetValMod();
}

/*!	\fn void	CtmDsp54All::GetOPStatus()
 *	\brief	A/D & D/A顯示值
 *  HYDR.PROG(10H) + POTM.POSN_PHY(10H) + POTM.A_POSN(10H) + TEMP.SUMTEM(14H) + TEMP.STRTEM(14H)
 *  size = 58H
 *	\return
 */
void  CtmDsp54All::GetOPStatus()
{
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_STATUS_ID, FALSE);
	Echo(COMM_ECHO_SUCCESS);
	static	int		nChrgStart = FALSE, nChrgBStart = FALSE;
	double dX = 0, dY1 = 0, dY2 = 0;
	long lOPERSTEP1 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;
	long lOPERSTEP4 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP4").lValue;

	//printf("lOPERSTEP1=%x lOPERSTEP4=%x \n", lOPERSTEP1, lOPERSTEP4);
	//if ((lOPERSTEP1 &0x0008)||(lOPERSTEP4 &0x0400))
	{
		if (nChrgStart == FALSE)
		{
			nChrgStart = TRUE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALBACKPRES);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALRPM);
		}
		if (nChrgBStart == FALSE)
		{
			nChrgBStart = TRUE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGEB_REALBACKPRES);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGEB_REALRPM);
		}
	}
	
	if (lOPERSTEP1 & 0x0020)
	{
		dX  = GetDBValue("METER_AD1_READ_CHL1_RELATIVEPOSITION").lValue;
		if (GetDBValue("SYSX_SCREW_RECOVERY_NULL_CURVEOPTION").lValue)
			dY1 = GetDBValue("METER_AD1_READ_CHL5_RELATIVEPOSITION").lValue;
		else
			dY1 = 0;
		dY2 = GetDBValue("MONI_SCRW_RCV_NULL_REALRPM").lValue;
		nChrgStart = TRUE;
		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALBACKPRES, &dX, &dY1, 1);
		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALRPM, &dX, &dY2, 1);
	}
	else
		nChrgStart = FALSE;
	
	if (lOPERSTEP4 & 0x2000)
	{
		dX  = GetDBValue("METER_AD1_READ_CHL8_RELATIVEPOSITION").lValue;
		if (GetDBValue("SYSX_SCREW_RECOVERY_NULL_CURVEOPTION").lValue)
			dY1 = GetDBValue("METER_AD1_READ_CHL6_RELATIVEPOSITION").lValue;
		else
			dY1 = 0;
		nChrgBStart = TRUE;
		dY2 = GetDBValue("MONIB_SCRW2_RCV_NULL_REALCOUNTER").lValue;
		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGEB_REALBACKPRES, &dX, &dY1, 1);
		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGEB_REALRPM, &dX, &dY2, 1);
	}
	else
		nChrgBStart = FALSE;
	

}

/*!	\fn void	CtmDsp54All::GetMoniCurrent()
 *	\brief	當前模監測資料
 *  size = 40H
 *	\return
 */
void  CtmDsp54All::GetMoniCurrent()
{
	DWORD		dwValue;

	//DWORD MoldHead , MmiConfig , Moldset ,MoldsetB , Monitor , Com2TempSet , MoldSetX , Moni;

	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONICURRENT_ID, FALSE);

//	MoldHead = g_pBlock->GetBlockSize(BLOCK_MOLDHDR_ID);
//	//printf("MoldHead=%d\n",MoldHead);
//	MmiConfig = g_pBlock->GetBlockSize(BLOCK_MMICONFIG_ID);
////	printf("MmiConfig=%d\n",MmiConfig);
//	Moldset = g_pBlock->GetBlockSize(BLOCK_MOLDSET_ID);
//	//printf("Moldset=%d\n",Moldset);
//	MoldsetB = g_pBlock->GetBlockSize(BLOCK_MOLDSETB_ID);
//	//printf("MoldsetB=%d\n",MoldsetB);
//	Monitor = g_pBlock->GetBlockSize(BLOCK_MONITOR_ID);
//	//printf("Monitor=%d\n",Monitor);
//	Com2TempSet = g_pBlock->GetBlockSize(63);
//	//printf("Com2TempSet=%d\n",Com2TempSet);
//	MoldSetX = g_pBlock->GetBlockSize(BLOCK_MOLDSETX_ID);
//	//printf("MoldSetX=%d\n",MoldSetX);
//	Moni = sizeof(tmDB_MONI2);
	//printf("Moni=%d\n",Moni);

	if (m_b28Clmp) /*開模終點縮小10倍傳給55*/
	{
		g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONICURRENT_ID, "MONI_MLD_OPEN_NULL_REALPOSITION", &dwValue);
		//	cout << "dwValue=" << dwValue << endl;
		dwValue = dwValue / 10;
		SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", dwValue, FALSE);
	}

	if (GetOilTable())
	{
		g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONICURRENT_ID, "MONI_MLD_OPEN_NULL_REALPOSITION", &dwValue);
		SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", OilToPosi(dwValue), FALSE);
	}

	//g_pBlock->GetBlockSize();
	Echo(COMM_ECHO_SUCCESS);
}

/*!	\fn void	CtmDsp54All::GetMoniLast()
 *	\brief	上一模監測資料
 *  size = 40H
 *	\return
 */
void  CtmDsp54All::GetMoniLast()
{
	DWORD		dwValue = 0, dwPosiValue = 0;

	Echo(COMM_ECHO_SUCCESS);

	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONILAST_ID, FALSE);
	if (m_b28Clmp) /*開模終點縮小10倍傳給55*/
	{
		g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONILAST_ID, "MONI_MLD_OPEN_NULL_LASTPOSITION", &dwValue);
		dwValue = dwValue / 10;
		SetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION", dwValue, FALSE);
	}

	if (GetOilTable())
	{
		g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONILAST_ID, "MONI_MLD_OPEN_NULL_LASTPOSITION", &dwValue);
		dwPosiValue = OilToPosi(dwValue);
		g_pDatabase->Write("MONI_MLD_OPEN_NULL_LASTPOSITION", &dwPosiValue, FALSE );//wangli2014-12-2 14:52:54 上一摸值不存SRAM,應該斷電為0
		//SetDBValue("MONI_MLD_OPEN_NULL_LASTPOSITION", OilToPosi(dwValue), FALSE);
	}

	if (!(m_bDC)) //James add 2008/8/13 09:42上午
	{
		dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
		if (dwValue == g_MoniUpdate.dwShotCount) //St1128'01  for moni lose problem
		{
			g_MoniUpdate.bChange = TRUE;
		}
		else
		{
			if (GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue != 0)
			{
				if ((dwValue % GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue) == 0)
					g_ptmControlServer->MoniControl()->AddMoni2();
			}
			g_MoniUpdate.dwShotCount = dwValue;
			g_MoniUpdate.bChange = FALSE;
		}
	}
}

/*!	\fn void	CtmDsp54All::GetMoniMaxMin()
 *	\brief	監測上下限資料
 *  size = 81H
 *	\return
 */
void  CtmDsp54All::GetMoniMaxMin()
{
	Echo(COMM_ECHO_SUCCESS);
	//g_pBlock->GetBlock(g_frameRx.commdata.acBuffer+2*sizeof(WORD), BLOCK_MONIMAXMIN_ID, FALSE);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONIMAXMIN_ID, FALSE);
}

/*!	\fn void	CtmDsp54All::GetMoniCurrentB()
 *	\brief	B座當前模監測資料
 *  size = 30H
 *	\return
 */
void  CtmDsp54All::GetMoniCurrentB()
{
	DWORD		dwValue;

	//Echo(COMM_ECHO_SUCCESS);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONIBCURRENT_ID, FALSE);

//	if(m_b28Clmp)/*開模終點縮小10倍傳給55*/
//	{
//		g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer+2*sizeof(WORD), BLOCK_MONIBCURRENT_ID, "MONI_MLD_OPEN_NULL_REALPOSITION", &dwValue);
//	  dwValue = dwValue/10;
//	  SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", dwValue, FALSE);
//	}
//	if (GetOilTable())
//	{
//		dwValue = GetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION").lValue;
//	    SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", OilToPosi(dwValue), FALSE);
//	}
}

/*!	\fn void	CtmDsp54All::GetMoniLastB()
 *	\brief	B座上一模監測資料
 *  size = 30H
 *	\return
 */
void  CtmDsp54All::GetMoniLastB()
{
	Echo(COMM_ECHO_SUCCESS);
	DWORD		dwValue;
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONIBLAST_ID, FALSE);

//	if(m_b28Clmp)/*開模終點縮小10倍傳給55*/
//	{
//		g_pBlock->GetBlockValue(g_frameRx.commdata.acBuffer+2*sizeof(WORD), BLOCK_MONIBLAST_ID, "MONI_MLD_OPEN_NULL_LASTPOSITION", &dwValue);
//	  dwValue = dwValue/10;
//	  SetDBValue("MONI_MLD_OPEN_NULL_REALPOSITION", dwValue, FALSE);
//	}
	dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	if (dwValue == g_MoniUpdate.dwShotCount) 		//St1128'01  for moni lose problem
	{
		g_MoniUpdate.bChange = TRUE;
	}
	else
	{
		if (GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue != 0)
		{
			if ((dwValue % GetDBValue("SAMPLING_HMI_NULL_NULL_SAMPLEINTERVAL").lValue) == 0)
				g_ptmControlServer->MoniControl()->AddMoni2();
		}
		g_MoniUpdate.dwShotCount = dwValue;
		g_MoniUpdate.bChange = FALSE;
	}
}

/*!	\fn void	CtmDsp54All::GetMoniMaxMinB()
 *	\brief	B座監測上下資料
 * size = 60H
 *	\return
 */
void  CtmDsp54All::GetMoniMaxMinB()
{
	Echo(COMM_ECHO_SUCCESS);
	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), BLOCK_MONIMAXMINB_ID, FALSE);
}


/*!	\fn void	CtmDsp54All::GetDiagRMRI()
 *	\brief	DSP ROM&RAM INPUT資料
 *	\return
 */
void  CtmDsp54All::GetDiagRMRI()
{
	Echo(COMM_ECHO_SUCCESS);
	m_nRevRM++;
	memcpy((char *)&m_dbdiagrmri, g_frameRx.commdata.acBuffer + 2 * sizeof(WORD), sizeof(tmDB_DIAGRMRI));

	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	msg.message	= MSG_DSP54_REVRM;
	msg.lParam	= m_nRevRM;
	g_pApplication->QueueMessage(&msg);
}


/*!	\fn void	CtmDsp54All::GetCurveData()
 *	\brief	射出曲線
 *  size = 20H
 *	\return
 */
void  CtmDsp54All::GetCurveData()
{
	tmDB_ETHCURVEDATA	EthCurveData;
	static	int		nInjeX = 0, nInjeY1 = 0, nInjeY2 = 0, nInjeX2 = 0;
#ifdef	D_5528
	static	int	nInjeY3 = 0;
	static	int	nInjeY4 = 0;
#endif

	static	int		nInjeBX = 0, nInjeBY1 = 0, nInjeBY2 = 0, nInjeBX2 = 0;
	static	BOOL	bInjStart = FALSE, bHoldStart = FALSE, nChagStart = FALSE;
	static 	BOOL	bInjStart2 = FALSE, bHoldStart2 = FALSE;
	static  BOOL 	bHoldReCal = FALSE;
#ifdef 	D_5528
	static 	BOOL	bInjeClear = FALSE;
#else
	static 	BOOL	bInjeClear = TRUE;
#endif
	Echo(COMM_ECHO_SUCCESS);
	memcpy(&EthCurveData, g_frameRx.commdata.acBuffer, sizeof(EthCurveData));

	//printf("EthCurveData.cnPoint=%d kind=%d \n", EthCurveData.cnPoint, EthCurveData.tmNewCurveData[0].flCurveKind);
	if (EthCurveData.cnPoint == 0)
	{
		switch (EthCurveData.tmNewCurveData[0].flCurveKind)
		{
		case 0:      //INJ
			bInjStart = FALSE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALPRESS);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALSPEED);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_DXPOS);
			ClearChartInjectCount();
			SendMsg(MSG_SH_INET_INJECT_DATA, 0, 0, NULL);
			break;
		case 1:      //HOLD
			bHoldStart = FALSE;
			bHoldReCal = FALSE;
			bInjeClear = TRUE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALPRESS);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALSPEED);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_DXPOS);
			ClearChartHoldCount();
			break;
		case 2:      //INJ2
			bInjStart2 = FALSE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECTB_REALPRESS);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECTB_REALSPEED);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECTB_DXPOS);
			ClearChartInjectBCount();
			SendMsg(MSG_SH_INET_INJECTB_DATA, 0, 0, NULL);
			break;
		case 3:      //HOLD2
			bHoldStart2 = FALSE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLDB_REALPRESS);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLDB_REALSPEED);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLDB_DXPOS);
			ClearChartHoldBCount();
			break;
		case 4: 	//Chrg Speed
			nChagStart = FALSE;
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALBACKPRES);
			CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALRPM);
			ClearChartChrgCount();
			break;
		default:
			break;
		}
	}

	if (EthCurveData.cnPoint <= 10)
	{
		for (int i = 0; i < EthCurveData.cnPoint; i++)
		{
			//	dY1 ; pressure,  dY2 :Speed
			double dX = 0, dY1 = 0, dY2 = 0, dX2 = 0;
			dX 	= EthCurveData.tmNewCurveData[i].nX1;
			dY1 = EthCurveData.tmNewCurveData[i].nY1;
			dY2 = EthCurveData.tmNewCurveData[i].nY2;
			dX2	= EthCurveData.tmNewCurveData[i].nX2; 		// 曲線時間
#ifdef	D_5528
			double	dY3 = EthCurveData.tmNewCurveData[i].nY3;
			double	dY4 = EthCurveData.tmNewCurveData[i].nY4;
			//printf("dX:%lf,dY1=%lf,dY2=%lf,dY3:%lf,dY4=%lf,dX2=%lf\n",dX,dY1,dY2,dY3,dY4,dX2);
#endif
			tmDB_CHARTSTROKE1     chartstroke1;
			chartstroke1.nY1 = EthCurveData.tmNewCurveData[i].nY1;
			chartstroke1.nY2 = EthCurveData.tmNewCurveData[i].nY2;
			//dX 	= dY1;
			switch (EthCurveData.tmNewCurveData[i].flCurveKind)
			{
			case 0:      //INJ
				if (!bInjStart)
				{
					bInjeClear = FALSE;
					CtmCurveDataControl::GetInstance()->ClearBuffer();
				}
				//printf("Inje:dX:%lf,dY1=%lf,dY2=%lf,dY3:%lf,dY4=%lf,dX2=%lf\n",dX,dY1,dY2,dY3,dY4,dX2);
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALPRESS, &dX, &dY1, 1);
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALSPEED, &dX, &dY2, 1);
#ifdef	D_5528
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_SETPRESS, &dX, &dY3, 1);
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_SETSPEED, &dX, &dY4, 1);
#endif
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_DXPOS, &dX2, &dX, 1);   //x=time,y=pos

				bInjStart = TRUE;
				nInjeX			= dX;			//Pos
				nInjeY1			= dY1;			//Real Press
				nInjeY2			= dY2;			//Real Speed
#ifdef	D_5528
				nInjeY3			= dY3;			//Set Press
				nInjeY4			= dY4;			//Set Speed
#endif
				nInjeX2			= dX2;			//Time
				break;
			case 1:      //HOLD
#ifdef	D_5528
				if (bInjeClear)
				{
					bInjeClear = FALSE;
					CtmCurveDataControl::GetInstance()->ClearBuffer();
				}
#endif
				if ((bInjStart == FALSE) && (bHoldStart == FALSE))
				{
					//printf("EthCurveData.tmNewCurveData[%d].nX2,nInjeX2=%d====================================%d\n",i,EthCurveData.tmNewCurveData[i].nX2,nInjeX2);
					double d0X = 0, d0Y1 = 0, d0Y2 = 0, d0X2 = 0;
					if (nInjeX2 > EthCurveData.tmNewCurveData[i].nX2)
					{
						printf("Clear Buffer....................................\n");
						CtmCurveDataControl::GetInstance()->ClearBuffer();
					}
					if (EthCurveData.tmNewCurveData[i].nX2 >= nInjeX2)
						d0X  = nInjeX2; //0;
					else
						d0X  = 0 ;
					d0Y1 	= nInjeY1;
					d0Y2	= nInjeY2;
					d0X2	= nInjeX;
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_DXPOS, &d0X, &d0X2, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, &d0X, &d0Y1, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALSPEED, &d0X, &d0Y2, 1);
#ifdef	D_5528
					double	d0Y3 = 0, d0Y4 = 0;
					d0Y3	= nInjeY3;
					d0Y4	= nInjeY4;
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_SETPRESS, &d0X, &d0Y3, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_SETSPEED, &d0X, &d0Y4, 1);
#endif

				}
				if (EthCurveData.tmNewCurveData[i].nX2 >= nInjeX2)
				{
					if (bHoldReCal)
					{
						dX2 = EthCurveData.tmNewCurveData[i].nX2 + nInjeX2;
					}
					else
					{
						dX2 = EthCurveData.tmNewCurveData[i].nX2 /* - nInjeX2*/;
					}
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_DXPOS, &dX2, &dX, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, &dX, &dY1, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALSPEED, &dX, &dY2, 1);
#ifdef D_5528
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_SETPRESS, &dX, &dY3, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_SETSPEED, &dX, &dY4, 1);
#endif
				}
				else if (EthCurveData.tmNewCurveData[i].nX2 >= 0)
				{
					bHoldReCal = TRUE;
#ifndef D_5528
					if (bInjeClear)
					{
						bInjeClear = FALSE;
						CtmCurveDataControl::GetInstance()->ClearBuffer();
						//CtmCurveDataControl::GetInstance()->ClearBuffer(CURVE_OIL_DATA_INJECT_REALSPEED);
						//CtmCurveDataControl::GetInstance()->ClearBuffer(CURVE_OIL_DATA_INJECT_DXPOS);
					}
#endif
					nInjeX2 = 0;
					dX2 = EthCurveData.tmNewCurveData[i].nX2 + nInjeX2;
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_DXPOS, &dX2, &dX, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, &dX, &dY1, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALSPEED, &dX, &dY2, 1);
#ifdef D_5528
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_SETPRESS, &dX, &dY3, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_SETSPEED, &dX, &dY4, 1);
#endif
				}
				bHoldStart = TRUE;
				break;
			case 2:      //INJ2
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECTB_REALPRESS, &dX, &dY1, 1);
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECTB_REALSPEED, &dX, &dY2, 1);
				break;
			case 3:      //HOLD2
				if (EthCurveData.cnPoint == 0)
				{
					double d0X = 0, d0Y1 = 0, d0Y2 = 0, d0X2 = 0;
					short	 d0index = 0;
					tmDB_CHARTSTROKE1     chartstroke0;
					if (m_dbchartInjectB.nIndex)	d0index = m_dbchartInjectB.nIndex - 1;
					chartstroke0.nX  = m_dbchartInjectB.nX [d0index];
					chartstroke0.nY1 = m_dbchartInjectB.nY1[d0index];
					chartstroke0.nY2 = m_dbchartInjectB.nY2[d0index];

					d0X 	= 0;
					d0Y1 = m_dbchartInjectB.nY1[d0index];
					d0Y2 = m_dbchartInjectB.nY2[d0index];
					d0X2 = m_dbchartInjectB.nX [d0index]; 		// 曲線時間

					if (m_dbchartInjectB.nX [d0index] >= (WORD)m_dbchartHoldB.nX0) //Andy 20090710
					{
						double HoldPOS = 0;      //save pos              //czz    2009/10/27 07:11下午
						HoldPOS = dX;
						d0X = m_dbchartInjectB.nX [d0index] - m_dbchartHoldB.nX0;
						CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_DXPOS, &d0X, &HoldPOS, 1);
					}
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALPRESS, &d0X, &d0Y1, 1);
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALSPEED, &d0X, &d0Y2, 1);
				}
				chartstroke1.nX  = (short)EthCurveData.tmNewCurveData[i].nX2;
				//printf("case 3 -1 \n");
				if (EthCurveData.tmNewCurveData[i].nX2 >= (WORD)m_dbchartHoldB.nX0)
				{
					double HoldPOS = 0;      //save pos              //czz    2009/10/27 07:11下午
					HoldPOS = dX;
					dX = EthCurveData.tmNewCurveData[i].nX2 - m_dbchartHoldB.nX0;	//add ,2009-2-19
					//printf("case 3 -1.0 \n");
					CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_DXPOS, &dX, &HoldPOS, 1);
					//printf("case 3 -1.1 \n");
				}
				//printf("case 3 -2 \n");
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALPRESS, &dX, &dY1, 1);
				//printf("case 3 -3 \n");
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLDB_REALSPEED, &dX, &dY2, 1);
				//printf("case 3 -4 \n");
				break;
			case 4:
				//chartstroke1.nX  = GetDBValue("METER_AD1_READ_CHL1_RELATIVEPOSITION").lValue;//dbmetr.adPosi_Relative[0];
				//CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALBACKPRES, &dX, &dY1, 1);
				CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALRPM, &dX, &dY2, 1);
				break;
			default:
				break;
			}
			//i++;
			//}//while(i<EthCurveData.cnPoint);
		}
	}
}

/*!	\fn void	CtmDsp54All::GetEcho()
 *	\brief	處理dsp54回傳的echo
 *	\return
 */
void	CtmDsp54All::GetEcho()
{
	WORD 			wResult = 0xFFFF, wLength = 0, wID;
	static	int nNum = 0;//20140613
	long	lDsp28CommState = 0;//20140613
	lDsp28CommState 	= GetDBValue("STATE_OTHR_OTHR_NULL_RSVCOFG12").lValue;//20140613
	CMDDATA 		cmddata;
	//wResult接收主機回傳DATA，如果是0表示接收成功，否則表示接受失敗，需要重新根據CMD傳輸數據
	wResult = *(WORD*)(g_frameRx.commdata.acBuffer + 2 * sizeof(WORD));
	wLength = *(WORD*)(g_frameRx.commdata.acBuffer + sizeof(WORD));
	wID     = *(WORD*)(g_frameRx.commdata.acBuffer);
	nNum ++;//20140613
	/*if(nNum >= 2)			//超時重新發送  //20140613
	{
		m_bTransmitCmdEcho = TRUE;
	}*/
// printf("wResult=%d\n",wResult);
	//printf("wLength=%d\n",wLength);
	//printf("wID=%d\n",wID);
	//printf("GetEcho g_frameRx.wCmd = %x\n", g_frameRx.wCmd);
	if (wResult == 0)
	{
		if (wLength == 1)
		{
			switch (g_frameRx.wCmd)
			{
			case COMM_ETH_SENDPANEL:
				m_CmdPanelBreak.wCmd = 0;
				break;
// deleted by J.Wong 奀唗祥夔悵痐 2016/8/23 16:01:42
//				case COMM_SENDTRANSMITDSP28:
//					PopTransmitCount(1);        //20140526
//					m_bTransmitCmdEcho = TRUE;	//20140526
//					m_nNum = 0;
//					if(PopTransmitCmd(&m_TransmitBuffer))
//    				{
//    					usleep(2000);
//    					SendTransmitQueue(&m_TransmitBuffer, COMM_MAXTRANSIMT_NET);
//    					m_bTransmitCmdEcho = FALSE;
//    				}
//					break;
			case COMM_ETH_SENDUPDATEDATA:		//如果是發送單筆資料，當收到dsp54的echo就清除狀態，否則沒有收到則要重新發送
			//case COMM_SENDTRANSMITDSP28:
			case COMM_ETH_SENDCONTROLMACHINE1:
			case COMM_ETH_SENDCONTROLMACHINE2:
			case COMM_ETH_SENDCONTROLMOLDSET1:
			case COMM_ETH_SENDCONTROLMOLDSET2:
			case COMM_ETH_SENDCONTROLMACHINEB:
			case COMM_ETH_SENDCONTROLMOLDSETB1:
			case COMM_ETH_SENDCONTROLMOLDSETB2:
			case COMM_ETH_SENDCONTROLNEWMACHINE2:
			case COMM_ETH_SENDCONTROLNEWMOLDSET2_1:
			case COMM_ETH_SENDCONTROLNEWMOLDSET2_2:
			case COMM_ETH_SENDCONTROLNEWMACHINE3:
			case COMM_ETH_SENDCONTROLNEWMOLDSET3_1:
			case COMM_ETH_SENDCONTROLNEWMOLDSET3_2:
				m_wCmdState = CMD_ECHO;
				m_wCmdSend	= 0;
				cmddata.wCmd 	= g_frameRx.wCmd;
				cmddata.wIndex 	= wID;
				DelEchoCmd(&cmddata);

				if (cmddata.wIndex == g_pDatabase->GetDataID("MACH_OTHR_OTHR_NULL_WERRORSTATUS1"))
				{
					DWORD	dwValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;

					if (dwValue | COMM_S_REACHSHOTCOUNT)
					{
						dwValue &= ~COMM_S_REACHSHOTCOUNT;
						SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue, FALSE);
					}
					if (dwValue | COMM_S_REACHPACKCOUNT)
					{
						dwValue &= ~COMM_S_REACHPACKCOUNT;
						SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", dwValue, FALSE);
					}
				}
				break;
			default:
				break;
			};
		}
		//0 表示接收成功 1 表示接受失敗
		return ;
	}

	//wLength 大于1 的話表示是需要重新發送整個block的資料，否則只是重新發送單筆資料
	if (wID == 0)
	{
		switch (g_frameRx.wCmd)
		{
		case COMM_ETH_SENDCONTROLMOLDSET1:
			SendControlMoldset(1);
			break;
		case COMM_ETH_SENDCONTROLMOLDSET2:
			SendControlMoldset(2);
			break;
		case COMM_ETH_SENDCONTROLMACHINE1:
			SendControlMachine(1);
			break;
		case COMM_ETH_SENDCONTROLMACHINE2:
			SendControlMachine(2);
			break;
		case COMM_ETH_SENDCONTROLMACHINEB:
			SendControlMachineB();
			break;
		case COMM_ETH_SENDCONTROLMOLDSETB1:
			SendControlMoldsetB(1);
			break;
		case COMM_ETH_SENDCONTROLMOLDSETB2:
			SendControlMoldsetB(2);
			break;
		case COMM_ETH_SENDCONTROLNEWMACHINE2:
			SendControlNewMachine2(1);
			break;
		case COMM_ETH_SENDCONTROLNEWMOLDSET2_1:
			SendControlNewMoldset2(1);
			break;
		case COMM_ETH_SENDCONTROLNEWMOLDSET2_2:
			SendControlNewMoldset2(2);
			break;
		case COMM_ETH_SENDCONTROLNEWMACHINE3:
			SendControlNewMachine3(1);
			break;
		case COMM_ETH_SENDCONTROLNEWMOLDSET3_1:
			SendControlNewMoldset3(1);
			break;
		case COMM_ETH_SENDCONTROLNEWMOLDSET3_2:
			SendControlNewMoldset3(2);
			break;
		case COMM_ETH_SENDUPDATEDATA:
			switch (m_wCmdSend)
			{
			case COMM_ETH_GETCONFIG:
				SendConfig();
				break;
			default:
				break;
			}
			break;
		case COMM_ETH_GETDAOUTPUT:
			break;
		default:
			break;
		}
	}
	else if (wID > 0)
	{
		//重新發送單筆資料需要區分模組、機器資料和 其它資料
		switch (g_frameRx.wCmd)
		{
		case COMM_ETH_SENDCONTROLMOLDSET1:
		case COMM_ETH_SENDCONTROLMOLDSET2:
		case COMM_ETH_SENDCONTROLMACHINE1:
		case COMM_ETH_SENDCONTROLMACHINE2:
		case COMM_ETH_SENDCONTROLMACHINEB:
		case COMM_ETH_SENDCONTROLMOLDSETB1:
		case COMM_ETH_SENDCONTROLMOLDSETB2:
			m_wCmdState 	= CMD_SEND;
			cmddata.wCmd 	= g_frameRx.wCmd;
			cmddata.wIndex 	= wID;
			PushCmd(&cmddata);
			break;
		case COMM_ETH_SENDUPDATEDATA:
			//m_wCmdState 	= CMD_SEND;
			//if(g_frameRx.wCmd != COMM_ETH_SENDUPDATEDATA)
			//{
			//	cmddata.wCmd 	= g_frameRx.wCmd;
			//	cmddata.wIndex 	= wID;
			///	PushCmd(&cmddata);
			//}
			//else if(m_wCmdSend != COMM_ETH_SENDUPDATEDATA)
			//{
			//	cmddata.wCmd 	= m_wCmdSend;
			//	cmddata.wIndex 	= wID;
			//	PushCmd(&cmddata);
			//}
			break;
		default:
			break;
		}
	}
}

/*!	\fn void	CtmDsp54All::Echo(WORD wCode)
 *	\brief	發送Echo給dsp54
 *	\return
 */
void	CtmDsp54All::Echo(WORD wCode)
{
	int         nWriteCount;
	WORD        wFirstID = 0;

	wFirstID = m_wGetDataCounter;              			//James modify 2010/7/7 07:32  改為回傳受到數據包的編號//取出傳送資料的起始ID
	g_frameRx.commdata.wResult = wCode;                 //設置回傳的DATA，0000 成功 0001失敗

	//printf("g_frameRx.wCmd=%x \n", g_frameRx.wCmd);
	if (wCode == COMM_ECHO_FAILURE &&                   //0x4141~0x6060 == 'AA'~'ZZ' 合法命令
	        (g_frameRx.wCmd > 0x6060  ||
	         g_frameRx.wCmd < 0x3030))      			//接受失敗而且命令未知的話傳0x0001
		g_frameRx.wCmd = 0x0001;

	WriteToken((BYTE*)&g_frameRx, sizeof(WORD), &nWriteCount, wFirstID);
}

/*!	\fn void	CtmDsp54All::PushCmd(CMDDATA* pcmd)
 *	\brief
 *	\return
 */
BOOL        CtmDsp54All::PushCmd(CMDDATA* pcmd)
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

/*!	\fn void	CtmDsp54All::PopCmd(CMDDATA* pcmd)
 *	\brief
 *	\return
 */
BOOL	CtmDsp54All::PopCmd(CMDDATA* pcmd)
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

/*!	\fn void	CtmDsp54All::PushEchoCmd(CMDDATA* pcmd)
 *	\brief
 *	\return
 */
BOOL        CtmDsp54All::PushEchoCmd(CMDDATA* pcmd)
{
	if (!m_CmdEchoQueue.bQueueFull)
	{
		m_CmdEchoQueue.CmdQueue  [m_CmdEchoQueue.wInput]   = pcmd->wCmd;
		m_CmdEchoQueue.IndexQueue[m_CmdEchoQueue.wInput]   = pcmd->wIndex;
		m_CmdEchoQueue.ValueQueue[m_CmdEchoQueue.wInput++] = pcmd->wValue;
		if (m_CmdEchoQueue.wInput >= m_CmdEchoQueue.wQueueEnd)
			m_CmdEchoQueue.wInput = 0;
		if (m_CmdEchoQueue.wInput == m_CmdEchoQueue.wOutput)
			m_CmdEchoQueue.bQueueFull = TRUE;
		return TRUE;
	}

	return FALSE;
}

/*!	\fn void	CtmDsp54All::DelEchoCmd(CMDDATA* pcmd)
 *	\brief
 *	\return
 */
BOOL	CtmDsp54All::DelEchoCmd(CMDDATA* pcmd)
{
	if ((m_CmdEchoQueue.wInput != m_CmdEchoQueue.wOutput) || (m_CmdEchoQueue.bQueueFull))
	{
		if (m_CmdEchoQueue.wInput > m_CmdEchoQueue.wOutput)
		{
			for (int i = m_CmdEchoQueue.wOutput; i < m_CmdEchoQueue.wInput; i++)
			{
				if (pcmd->wIndex == m_CmdEchoQueue.IndexQueue[i] &&
				        pcmd->wCmd 	== m_CmdEchoQueue.CmdQueue[i])
				{
					m_CmdEchoQueue.wOutput ++;
					if (m_CmdEchoQueue.wOutput >= m_CmdEchoQueue.wQueueEnd)
						m_CmdEchoQueue.wOutput = 0;
					m_CmdEchoQueue.bQueueFull = FALSE;
					return TRUE;
				}
			}
		}
		else
		{
			if (pcmd->wIndex == m_CmdEchoQueue.IndexQueue[m_CmdEchoQueue.wOutput] &&
			        pcmd->wCmd 	== m_CmdEchoQueue.CmdQueue[m_CmdEchoQueue.wOutput])
				m_CmdEchoQueue.wOutput ++;
		}
		if (m_CmdEchoQueue.wOutput >= m_CmdEchoQueue.wQueueEnd)
			m_CmdEchoQueue.wOutput = 0;
		m_CmdEchoQueue.bQueueFull = FALSE;
		return TRUE;
	}

	return FALSE;
}

/*!	\fn void	CtmDsp54All::PopEchoCmd(CMDDATA* pcmd)
 *	\brief
 *	\return
 */
BOOL	CtmDsp54All::PopEchoCmd(CMDDATA* pcmd)
{
	if ((m_CmdEchoQueue.wInput != m_CmdEchoQueue.wOutput) || (m_CmdEchoQueue.bQueueFull))
	{
		pcmd->wCmd   = m_CmdEchoQueue.CmdQueue  [m_CmdEchoQueue.wOutput];
		pcmd->wIndex = m_CmdEchoQueue.IndexQueue[m_CmdEchoQueue.wOutput];
		pcmd->wValue = m_CmdEchoQueue.ValueQueue[m_CmdEchoQueue.wOutput++];
		if (m_CmdEchoQueue.wOutput >= m_CmdEchoQueue.wQueueEnd) m_CmdEchoQueue.wOutput = 0;
		m_CmdEchoQueue.bQueueFull = FALSE;
		return TRUE;
	}

	return FALSE;
}

/*!	\fn void	CtmDsp54All::PushTransmitCmd(TRANSMITQUEUE* pTransmit)
 *	\brief		將轉發資料轉入buff
 *	\return
 */
BOOL	CtmDsp54All::PushTransmitCmd(TRANSMITDATA* pTransmit)
{
	//add by J.Wong 覃淕蛌楷勦蹈 2016/8/23 16:04:26
	pthread_mutex_lock(&m_cache_lock);
	if (m_Transmit_Cache.size() != m_Transmit.wQueueEnd)
	{
		TRANSMITDATA item;
		memcpy(item.acBuffer, pTransmit->acBuffer, COMM_MAXTRANSIMT_NET);
		m_Transmit_Cache.push(item);
		pthread_mutex_unlock(&m_cache_lock);
		return TRUE;
	}
	pthread_mutex_unlock(&m_cache_lock);
	return FALSE;

//	if (!m_Transmit.bQueueFull)
//	{
//		//printf("m_Transmit.bQueueFull=%d m_Transmit.wInput=%d out=%d \n", m_Transmit.bQueueFull, m_Transmit.wInput, m_Transmit.wOutput);
//		memcpy(m_Transmit.acBuffer[m_Transmit.wInput++], pTransmit->acBuffer, COMM_MAXTRANSIMT_NET);
//		m_lGetTransCount++;
//	    if (m_Transmit.wInput >= m_Transmit.wQueueEnd)
//	        m_Transmit.wInput = 0;
//	    if(m_lGetTransCount > m_lSendTransCount)
//	    {
//	    	if((m_lGetTransCount - m_lSendTransCount) >= m_Transmit.wQueueEnd )
//	    		m_Transmit.bQueueFull = TRUE;
//	    }
//	    //if (m_Transmit.wInput == m_Transmit.wOutput)
//	    //    m_Transmit.bQueueFull = TRUE;
//	    //printf("m_Transmit.bQueueFull=%d  end\n", m_Transmit.bQueueFull);
//	    return TRUE;
//	}
}

/*!	\fn void	CtmDsp54All::PopTransmitCmd (TRANSMITQUEUE* pTransmit)
 *	\brief		從buff中取出資料
 *	\return
 */
BOOL	CtmDsp54All::PopTransmitCmd (TRANSMITDATA* pTransmit)
{
	//add by J.Wong 覃淕蛌楷勦蹈 2016/8/23 16:04:26
	//printf("m_Transmit.wOutput=%d m_Transmit.wInput=%d m_Transmit.bQueueFull=%d \n", m_Transmit.wOutput, m_Transmit.wInput, m_Transmit.bQueueFull);
//	if ((m_Transmit.wInput != m_Transmit.wOutput) || (m_Transmit.bQueueFull))
//	{
//		if(m_lGetTransCount > m_lSendTransCount)
//		{
//			//James modify wOutPut++ --> wOutPut ; wati echo
//			memcpy(pTransmit->acBuffer, m_Transmit.acBuffer[m_Transmit.wOutput], COMM_MAXTRANSIMT_NET);
//		    if (m_Transmit.wOutput >= m_Transmit.wQueueEnd) m_Transmit.wOutput = 0;
//		    m_Transmit.bQueueFull = FALSE;
//		    return TRUE;
//		}
//		else if(m_lGetTransCount < m_lSendTransCount)
//		{
//			m_Transmit.wOutput  = 0;
//			m_Transmit.wInput	= 0;
//			printf("================Get more echo \n");
//			m_lGetTransCount = m_lSendTransCount;
//			return FALSE;
//		}
//		else
//			return FALSE;
//	}
	pthread_mutex_lock(&m_cache_lock);
	if (!m_Transmit_Cache.empty())
	{
		TRANSMITDATA item;
		item = m_Transmit_Cache.front();
		m_Transmit_Cache.pop();

		pthread_mutex_unlock(&m_cache_lock);
		memcpy(pTransmit->acBuffer, item.acBuffer, COMM_MAXTRANSIMT_NET);
		return TRUE;
	}
	pthread_mutex_unlock(&m_cache_lock);
	return FALSE;
}

/*!	\fn void	CtmDsp54All::PopTransmitCount (int	nCount)
 *	\brief		在transmit buff中移動下標個數
 *	\return
 */
BOOL	CtmDsp54All::PopTransmitCount (int	nCount)
{
	//printf("PopTransmitCount =%d \n", m_Transmit.wOutput);
	m_Transmit.wOutput += nCount;
	m_lSendTransCount++;
	if (m_Transmit.wOutput >= m_Transmit.wQueueEnd) m_Transmit.wOutput = 0;
	m_Transmit.bQueueFull = FALSE;
	return TRUE;
}

/*!	\fn void	CtmDsp54All::WriteToken(BYTE* pch, int nCount, int* pnWriteCount, WORD wFirstID)
 *	\brief		發送組包給dsp54
 *	\return
 */
void	CtmDsp54All::WriteToken(BYTE* pch, int nCount, int* pnWriteCount, WORD wFirstID)
{
	pthread_mutex_lock(&g_CommWriteMutex);
	WORD wTemp = 0, wType = 0;
	//獲得數據包WORD總數(協議頭+協議體+協議尾)
	wTemp = (m_SendHeader.GetSize() + nCount + m_SendFooter.GetSize()) >> 1;
	//設置數據包WORD總數
	m_SendHeader.SetPartValue(m_nPackSizePos, &wTemp, sizeof(WORD));
	wTemp = *(WORD *)pch;

	//設置包的序號 Type的高位為包序號
	if (m_bSendDataCounter >= 0xFF)
		m_bSendDataCounter = 0;
	m_bSendDataCounter++;
	wType |= m_bSendDataCounter << 8;
	m_SendHeader.SetPartValue(m_nTypePos, &wType, sizeof(WORD));
	//printf("WriteToken wTemp = %x, buffer=%x \n", wTemp, *(WORD*)m_SendHeader.GetBuffer());
	//設置CMD
	m_SendHeader.SetPartValue(m_nCMDPos, &wTemp, sizeof(WORD));
	wTemp = wFirstID;
	//設置起始ID
	m_SendHeader.SetPartValue(m_nIDPos, &wTemp, sizeof(WORD));
	wTemp = nCount >> 1;
	//設置資料長度(WORD數)
	m_SendHeader.SetPartValue(m_nDataLenPos, &wTemp, sizeof(WORD));
	//拷貝Head
	memcpy(m_pData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
	//拷貝DATA
	memcpy(m_pData + m_SendHeader.GetSize(), pch + 2, nCount);

	wTemp = 0;
	WORD * pwTemp = (WORD *)m_pData + 1;
	//CRC校驗
	for (int i = 0; i < (m_SendHeader.GetSize() - 2 + nCount) >> 1; i++)
	{
		wTemp ^= *pwTemp++;
	}
	//設置Foot
	m_SendFooter.SetPartValue(m_nCRCPos, &wTemp, sizeof(WORD));
	//拷貝Foot
	memcpy(m_pData + (m_SendHeader.GetSize() + nCount), m_SendFooter.GetBuffer(), m_SendFooter.GetSize());
	SendData(m_nDeviceID, (char*)m_pData, m_SendHeader.GetSize() + nCount + m_SendFooter.GetSize());
	pthread_mutex_unlock(&g_CommWriteMutex);
}

/*!	\fn void	CtmDsp54All::SelTempSendValue()
 *	\brief	雙色機溫度傳輸
 *	\return
 */
void     CtmDsp54All::SelTempSendValue()
{
	short       i;
	WORD		wIndex, wDataID, wTempControl;
	char*		pszID;

	wTempControl = GetDBValue("SYSX_TEMPER_HEATERON_NULL_DUALOPTION").lValue;
	if (!((wTempControl + 1) & 0x0001))
	{
		wDataID = g_pDatabase->GetDataID("MSET3_TMP1_HEATERON_CHL1_DEGREE");
		for (i = 0; i < 6; i++)
		{
			wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
		}
	}
	if (!((wTempControl + 1) & 0x0002))
	{
		wDataID = g_pDatabase->GetDataID("MSET3_TMP1_HEATERON_CHL11_DEGREE");
		for (i = 0; i < 6; i++)
		{
			wIndex = g_pDatabase->DataIDToIndex(wDataID + i);
			pszID = g_pDatabase->GetString(wIndex);
			g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, pszID, 0);
		}
	}
}

BOOL     CtmDsp54All::CheckOilVatID(char* pszData)
{
	short       i;

	for (i = 0; i < (int)(sizeof(g_apszVatTransMachID) / sizeof(g_apszVatTransMachID[0])); i++)
		if (strcmp(pszData, g_apszVatTransMachID[i]) == 0) return TRUE;

	for (i = 0; g_apszVatTransID[i] != NULL; i++)
	{
		if (strcmp(pszData, g_apszVatTransID[i]) == 0)
			return TRUE;
	}

	//for (i=0; i < (int)(sizeof(g_apszVatTransID) / sizeof(g_apszVatTransID[0])); i++)
	for (i = 0; g_apszVatTransMSET2ID[i] != NULL; i++)
	{
		if (strcmp(pszData, g_apszVatTransMSET2ID[i]) == 0)
		{
			if (!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, 吹氣擴展功能
			{
				char sz[128];
				if ((i >= 0) && (i < 6))
				{
					if (i < 3)
						sprintf(sz, "MSET2_BLST%d_NULL_MODE", i + 1);
					else
						sprintf(sz, "MSET2_MBLST%d_NULL_MODE", i + 1);
					if ((GetDBValue(sz).lValue != 0) && (GetDBValue(sz).lValue != 1) && (GetDBValue(sz).lValue != 2))
						continue;
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

void     CtmDsp54All::TransferOilVat()
{
	short       i;
	DWORD		dwOilVat;

	//for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
	for (i = 0; g_apszVatTransID[i] != NULL; i++)
	{
		g_pBlock->GetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, g_apszVatTransID[i], &dwOilVat);

		if (i == 3) /*wangli2015-5-20 18:34:40 關模低壓終止位置*/
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 1, FALSE); /*wangli2015-5-20 18:12:03 有推力座時（二板機 純二板沒有），關模低壓終止位置精度為2 的flag*/
		else
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 0, FALSE);

		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MOLDSET_ID, g_apszVatTransID[i], GetOilVat((int)dwOilVat));
	}
}

void     CtmDsp54All::TransferOilVat(BOOL bTransferMSET2, BOOL Flag)
{
	short       i;
	DWORD		dwOilVat;

	for (i = 0; g_apszVatTransMSET2ID[i] != NULL; i++)
	{
		if (!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, 吹氣擴展功能
		{
			char sz[128];
			if ((i >= 0) && (i < 6))
			{
				if (i < 3)
					sprintf(sz, "MSET2_BLST%d_NULL_MODE", i + 1);
				else
					sprintf(sz, "MSET2_MBLST%d_NULL_MODE", i + 1);
				if ((GetDBValue(sz).lValue != 0) && (GetDBValue(sz).lValue != 1) && (GetDBValue(sz).lValue != 2))
					continue;
			}
		}

		g_pBlock->GetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_NEWMOLDSET2_ID, g_apszVatTransMSET2ID[i], &dwOilVat);
		g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_NEWMOLDSET2_ID, g_apszVatTransMSET2ID[i], GetOilVat((int)dwOilVat));
	}


}

void     CtmDsp54All::TransferOilVat(BOOL bTransferMach)//Andy 20070520 add;
{
	short   i;
	DWORD		dwOilVat;

	if (bTransferMach)
	{
		for (i = 0; i < (int)(sizeof(g_apszVatTransMachID) / sizeof(g_apszVatTransMachID[0])); i++)
		{
			if (i == 0)
			{
				if (GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue != 0xFFFF)
				{
					if (g_pExTable->GetTableValue(0, "D_WSMVAT")) /*JOYCE2014-9-26 bug修正,應該為模板位置,mark MSET_MLD_OPEN_STEP5_POSITION*/
						g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID , g_apszVatTransMachID[i],
						                        GetOilVat((int)(GetDBValue("MSET_MLD_CLOSE_NULL_W04B7").lValue - GetDBValue("MACH_MLD_OPEN_NULL_W0216").lValue)));
					else
						g_pBlock->SetBlockValue(g_frameTx.commdata.acBuffer, BLOCK_MACHINE_ID , g_apszVatTransMachID[i],
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

void     CtmDsp54All::SendOilVat()
{
	short       i;

	//for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
	for (i = 0; g_apszVatTransMSET2ID[i] != NULL; i++)
	{
		if (!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, 吹氣擴展功能
		{
			char sz[128];
			if ((i >= 0) && (i < 6))
			{
				if (i < 3)
					sprintf(sz, "MSET2_BLST%d_NULL_MODE", i + 1);
				else
					sprintf(sz, "MSET2_MBLST%d_NULL_MODE", i + 1);
				if ((GetDBValue(sz).lValue != 0) && (GetDBValue(sz).lValue != 1) && (GetDBValue(sz).lValue != 2))
					continue;
			}
		}
		SendControlIndex(g_apszVatTransMSET2ID[i]);
	}

	for (i = 0; g_apszVatTransID[i] != NULL; i++)
	{
		if (i == 3) /*wangli2015-5-20 18:34:40 關模低壓終止位置*/
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 1, FALSE); /*wangli2015-5-20 18:12:03 有推力座時（二板機 純二板沒有），關模低壓終止位置精度為2 的flag*/
		else
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 0, FALSE);
		SendControlIndex(g_apszVatTransID[i]);
	}
}

void     CtmDsp54All::SetVatDefPosi()
{
	short       i;
	WORD		wValue;

	//for (i=0; i<(int)(sizeof(g_apszVatTransID)/sizeof(g_apszVatTransID[0])); i++)
	for (i = 0; g_apszVatTransMSET2ID[i] != NULL; i++)
	{
		if (!m_bDC && GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue)	//2012-12-11, 吹氣擴展功能
		{
			char sz[128];
			if ((i >= 0) && (i < 6))
			{
				if (i < 3)
					sprintf(sz, "MSET2_BLST%d_NULL_MODE", i + 1);
				else
					sprintf(sz, "MSET2_MBLST%d_NULL_MODE", i + 1);
				if ((GetDBValue(sz).lValue != 0) && (GetDBValue(sz).lValue != 1) && (GetDBValue(sz).lValue != 2))
					continue;
			}
		}
		wValue = GetDBValue(g_apszVatTransMSET2ID[i]).lValue;
		SetDBValue(g_apszVatTransMSET2ID[i], OilToPosi(wValue), FALSE);
	}

	for (i = 0; g_apszVatTransID[i] != NULL; i++)
	{
		wValue = GetDBValue(g_apszVatTransID[i]).lValue;
		if (i == 3) /*wangli2015-5-20 18:34:40 關模低壓終止位置*/
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 1, FALSE); /*wangli2015-5-20 18:12:03 有推力座時（二板機 純二板沒有），關模低壓終止位置精度為2 的flag*/
		else
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77", 0, FALSE);

		SetDBValue(g_apszVatTransID[i], OilToPosi(wValue), FALSE);
	}
	if (g_pExTable->GetTableValue(0, "D_WSMVAT"))
		SetDBValue("MSET_MLD_CLOSE_NULL_W04B7", OilToPosi(GetDBValue("MSET_MLD_OPEN_STEP5_POSITION").lValue), FALSE);
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

/*!	\fn void	CtmDsp54All::GetOPMeter()
 *	\brief	并入到CB AD /DA顯示值里面去了
 *	\return
 */
//void  CtmDsp54All::GetOPMeter()
//{
//	Echo(COMM_ECHO_SUCCESS);
//	char cszDBID[256];
//	g_pBlock->GetBlock(g_frameRx.commdata.acBuffer+2*sizeof(WORD), BLOCK_METER_ID, FALSE);
//
//	double dX = 0, dY1 = 0, dY2 = 0;
//	long lOPERSTEP1 =GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;
//	tmDB_CHARTSTROKE1     chartstroke1;
//	long long lChlValue = GetDBValue("MCONF_AD1_READ_CHANLE_OPTION").lValue;
//	if(lChlValue == 0)					//為了兼容沒有此筆資料的導致獲取OPMeter不對問題
//		lChlValue = 5;
//	sprintf(cszDBID, "METER_AD1_READ_CHL%d_RELATIVEPOSITION", lChlValue);	//James add 方便用戶更改位置尺
//	chartstroke1.nY1 = GetDBValue(cszDBID).lValue;//dbmetr.adPosi_Relative[4];
//	chartstroke1.nY2 = GetDBValue("MONI_SCRW_RCV_NULL_REALRPM").lValue;//dbmcur.cnChargeRPM;
//
//	if (g_WarmStartFlag||(lOPERSTEP1 &0x0008))
//	{
//
//    	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALBACKPRES);
//	 	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARGE_REALRPM);
//
//        ClearChartChrgCount();//ClearChartChargeCount();
//        if (g_WarmStartFlag) g_WarmStartFlag = FALSE;
//	}
//
//	if (lOPERSTEP1&0x0020)
//	{
//        chartstroke1.nX  = GetDBValue("METER_AD1_READ_CHL1_RELATIVEPOSITION").lValue;//dbmetr.adPosi_Relative[0];
//        AddChartCharge(&chartstroke1);
//
//        dX =chartstroke1.nX;
//		dY1=chartstroke1.nY1;
//		dY2=chartstroke1.nY2;
//		//dX /= 10;
//		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALBACKPRES, &dX, &dY1, 1);
//		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARGE_REALRPM, &dX, &dY2, 1);
//	}
//
//}


//void  CtmDsp54All::GetSProfile01I()
//{
//	ClearChartInjectCount();
//	ClearChartHoldCount();
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetDProfile01I()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetEProfile01I()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetSProfile01H()
//{
//	ClearChartHoldCount();
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetDProfile01H()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetEProfile01H()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetSBProfile01I()
//{
//	ClearChartInjectBCount();
//	ClearChartHoldBCount();
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetDBProfile01I()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetEBProfile01I()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetSBProfile01H()
//{
//	ClearChartHoldBCount();
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetDBProfile01H()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//
//void  CtmDsp54All::GetEBProfile01H()
//{
//	Echo(COMM_ECHO_SUCCESS);
//}
//

//BOOL	CtmDsp54All::AddChartInject(tmDB_CHARTSTROKE1* pdbchartstroke1)
//{
//	if (m_dbchartInject.nIndex < tmMAX_STROKE)
//    {
//    m_dbchartInject.nX [m_dbchartInject.nIndex]   = pdbchartstroke1->nX;
//    m_dbchartInject.nY1[m_dbchartInject.nIndex]   = pdbchartstroke1->nY1;
//    m_dbchartInject.nY2[m_dbchartInject.nIndex++] = pdbchartstroke1->nY2;
//    }
//
//	return TRUE;
//}
//
//BOOL	CtmDsp54All::AddChartHold  (tmDB_CHARTSTROKE1* pdbchartstroke1)
//{
//	if (m_dbchartHold.nIndex == 0)
//    	m_dbchartHold.nX0 = pdbchartstroke1->nX;
//
//	if (m_dbchartHold.nIndex < tmMAX_STROKE)
//   {
//    m_dbchartHold.nX [m_dbchartHold.nIndex]   = pdbchartstroke1->nX - m_dbchartHold.nX0;
//    m_dbchartHold.nY1[m_dbchartHold.nIndex]   = pdbchartstroke1->nY1;
//    m_dbchartHold.nY2[m_dbchartHold.nIndex++] = pdbchartstroke1->nY2;
//    }
//
//	return TRUE;
//}

//BOOL	CtmDsp54All::AddChartInjectB(tmDB_CHARTSTROKE1* pdbchartstroke1)
//{
//	if (m_dbchartInjectB.nIndex < tmMAX_STROKE)
//	    {
//	    m_dbchartInjectB.nX [m_dbchartInjectB.nIndex]   = pdbchartstroke1->nX;
//	    m_dbchartInjectB.nY1[m_dbchartInjectB.nIndex]   = pdbchartstroke1->nY1;
//	    m_dbchartInjectB.nY2[m_dbchartInjectB.nIndex++] = pdbchartstroke1->nY2;
//	    }
//
//	return TRUE;
//}

//BOOL	CtmDsp54All::AddChartHoldB  (tmDB_CHARTSTROKE1* pdbchartstroke1)
//{
//	if (m_dbchartHoldB.nIndex == 0)
//	    m_dbchartHoldB.nX0 = pdbchartstroke1->nX;
//
//	if (m_dbchartHoldB.nIndex < tmMAX_STROKE)
//	    {
//	    m_dbchartHoldB.nX [m_dbchartHoldB.nIndex]   = pdbchartstroke1->nX - m_dbchartHoldB.nX0;
//	    m_dbchartHoldB.nY1[m_dbchartHoldB.nIndex]   = pdbchartstroke1->nY1;
//	    m_dbchartHoldB.nY2[m_dbchartHoldB.nIndex++] = pdbchartstroke1->nY2;
//	    }
//
//	return TRUE;
//}

//BOOL   CtmDsp54All::AddChartCharge(tmDB_CHARTSTROKE1* pdbchartstroke1)		//Sunny<20070815>
//{
//	if (m_dbchartChrg.nIndex < tmMAX_STROKE)
//	    {
//	    m_dbchartChrg.nX [m_dbchartChrg.nIndex]   = pdbchartstroke1->nX;
//	    m_dbchartChrg.nY1[m_dbchartChrg.nIndex]   = pdbchartstroke1->nY1;
//	    m_dbchartChrg.nY2[m_dbchartChrg.nIndex++] = pdbchartstroke1->nY2;
//	    }
//	return TRUE;
//}

DWORD	CtmDsp54All::GetInjectTime(int nIndex)
{
	if (nIndex >= 6)
	{
		return	m_dwInjectTime[5];
	}
	else if (nIndex < 0)
	{
		return	m_dwInjectTime[0];
	}
	else
	{
		return 	m_dwInjectTime[nIndex];
	}
}

void	CtmDsp54All::SetDefaultEffectPos() //JOYCE2011-8-26 開模位置有效區初始值
{
	WORD  PrecisionDate = GetDBValue("CONF_AD1_WRITE_CHL1_LENGTHP1MM").lValue;
	WORD	temp = GetDBValue("CONF_AD1_WRITE_CHL2_PHYSICALLENGTH").lValue;
	WORD	wClampPos;
	WORD	lEffectPos;

	wClampPos = (HEX2BCD(temp)) * 10 + (PrecisionDate / (0x0001 << 4) % 0x0010);
	lEffectPos = wClampPos / 6; //主機合模位置尺長度的1/6
	if (wClampPos) //大于60,則以1/6值作為有效區,小于則以default--60作為有效區;讀不到主機合模位置尺長度，60作為有效區
	{
		if (lEffectPos > 600)
		{
			SetDBValue("MACH_COR_COR_NULL_EFFECTPOSITION", lEffectPos, FALSE);
		}
	}
}

WORD  		CtmDsp54All::GetCurrentMoldMedia()
{
	WORD wTemp = 0;
	WORD m_nCurrentIndex = 0;

	m_nCurrentIndex = g_pDatabase->GetIndex(CURRENT_MOLD_MEDIA);
	wTemp = g_pDatabase->Readw(m_nCurrentIndex);
// printf("%s dataIndex = %d, wTemp=%d\n", CURRENT_MOLD_MEDIA, m_nCurrentIndex, wTemp);

	return wTemp;
}

void		CtmDsp54All::ProcessLubricate()  //JOYCE2011-8-3 lubricate
{
	DWORD	dwLubrData = 0;
	BOOL	bProcessFlag = FALSE;
	BOOL	bShotCntLub1Flag = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue & 0x0001; //bit0
	BOOL	bShotCntLub2Flag = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue & 0x0002; //bit1
	BOOL	bShotCntLub3Flag = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue & 0x0004; //bit2
	long  lTemp = 0;
	long	lShotCntTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED27").lValue;/*實際運行模數*/
	long  lChangCntLub1 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED50").lValue; /*第1組潤滑切換模數*/
	long  lChangCntLub2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED51").lValue; /*第2組潤滑切換模數*/
	long  lChangCntLub3 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED52").lValue; /*第3組潤滑切換模數*/

	//printf("232 lChangCntLub1=%d,lChangCntLub2=%d,lChangCntLub3=%d\n",lChangCntLub1,lChangCntLub2,lChangCntLub3);
	if ((!bShotCntLub1Flag) && (lShotCntTemp > lChangCntLub1)) //only once; LUB1
	{
		bProcessFlag = TRUE;
		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue | 0x0001;
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19", lTemp, FALSE);
		for (int i = 0; i < 5 ; i++)
		{
			if (pHMILub1A[i] != NULL)	dwLubrData = GetDBValue(pHMILub1A[i]).lValue;
			if (pDSPLub1[i] != NULL)
			{
				SetDBValue(pDSPLub1[i], dwLubrData);

				if (i == 0) /*wangli2016-11-14 16:05:33 lub1自動潤滑模數 * 尾板支架潤滑參數係數 = 尾板支架潤滑模數*/
				{
					lTemp = dwLubrData * GetDBValue("SYSX_OTHERS_OTHERS_BYTE_RESERVED31").lValue / 100 ;
					SetDBValue("MACH_OTHR_OTHR_NULL_W01AF", lTemp);
				}

			}
		}
	}

	if ((!bShotCntLub2Flag) && (lShotCntTemp > lChangCntLub2)) //only once; LUB2
	{
		bProcessFlag = TRUE;
		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue | 0x0002;
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19", lTemp, FALSE);
		for (int i = 0; i < 5 ; i++)
		{
			if (pHMILub2A[i] != NULL)	dwLubrData = GetDBValue(pHMILub2A[i]).lValue;
			if (pDSPLub2[i] != NULL)	SetDBValue(pDSPLub2[i], dwLubrData);
		}
	}

	if ((!bShotCntLub3Flag) && (lShotCntTemp > lChangCntLub3) && m_b3LUB) //only one; LUB3
	{
		bProcessFlag = TRUE;
		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue | 0x0004;
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19", lTemp, FALSE);
		for (int i = 0; i < 5 ; i++)
		{
			if (pHMILub3A[i] != NULL)	dwLubrData = GetDBValue(pHMILub3A[i]).lValue;

			if (i == 4)
			{
				SetDBValue("MACH_MTR_ON_NULL_W01DD", dwLubrData >> 16); //自動潤滑保持計時
				SetDBValue("MACH_MTR_ON_NULL_W01DC", dwLubrData & 0xFFFF);
			}
			else
			{
				if (pDSPLub3[i] != NULL)	SetDBValue(pDSPLub3[i], dwLubrData);
			}
		}
	}

	if (bProcessFlag == TRUE)
		SaveLubricate();
}

void    CtmDsp54All::SaveLubricate()  //JOYCE2011-8-23 lubricate
{
	DWORD dwLubrData[55];
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
		"SYSX_OTHERS_OTHERS_INT_RESERVED56",      /*JOYCE2012-6-25 add 6個 潤滑次數上下限限定*/
		"SYSX_OTHERS_OTHERS_INT_RESERVED57",
		"SYSX_OTHERS_OTHERS_INT_RESERVED58",
		"SYSX_OTHERS_OTHERS_INT_RESERVED59",
		"SYSX_OTHERS_OTHERS_INT_RESERVED60",
		"SYSX_OTHERS_OTHERS_INT_RESERVED61",
		"SYSX_OTHERS_OTHERS_INT_RESERVED62",     /*JOYCE2012-9-14 配置版中增加標識碼*/
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED50",   /*JOYCE20130809 第1組潤滑切換模數*/
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED51",   /*JOYCE20130809 第2組潤滑切換模數*/
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED52",   /*JOYCE20130809 第3組潤滑切換模數*/
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED53",/*wangli2016-1-5 16:59:51  開機潤滑時數 lub1*/
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED54",/*wangli2016-1-5 16:59:51  開機潤滑時數 lub2*/
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
		for (int i = 0; i < 55; i++)
		{
			if (pDataID1[i] != NULL)	dwLubrData[i] = GetDBValue(pDataID1[i]).lValue;
			//printf("dwLubrData1[%d]:%d\n",i,dwLubrData[i]);
		}
	}
	write(nFd, &dwLubrData, sizeof(dwLubrData));
	close(nFd);

	//FILE2
	sprintf(m_szCopyFile, "%s/lubricate2.cdb", szPath);
	nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{
		for (int i = 0; i < 3 ; i++)
		{
			if (pDataID2[i] != NULL)	dwLubrData[i] = GetDBValue(pDataID2[i]).lValue;
			//printf("dwLubrData2[%d]:%d\n",i,dwLubrData[i]);
		}
	}
	write(nFd, &dwLubrData, sizeof(dwLubrData));
	close(nFd);
}

void	CtmDsp54All::SetPowerOnLubDefault()
{
	int nPowerOnLub = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED8").lValue;
	if (!nPowerOnLub)
		SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION", 0); /*開機自動潤滑天數為0,則送0到主機,代表開機時無需做潤滑*/
	else
	{
		/*開關機時間差*/
		STATEMODE	tmStateModeHead;
		STATEMODE	tmStateMode;
		if (CtmConfig::GetInstance()->GetRecordType() == 1)
		{
			tmFileRecordHeadInfo	HeadInfo;
			memset(&HeadInfo, 0, sizeof(HeadInfo));

			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));
			tmStateModeHead.nCount = HeadInfo.nCount;
			tmStateModeHead.nIndex = HeadInfo.nIndex;
		}
		else
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, 0, &tmStateModeHead);
		int		u_nPowerOnTime[3]	= {0};
		int		u_nPowerOffTime[3]	= {0};
		BOOL  u_bPowerOn = FALSE, u_bPowerOff = FALSE;
		//printf("nCount=%d\n",tmStateModeHead.nCount);
		if (tmStateModeHead.nCount > 1)
		{
			for (int i = tmStateModeHead.nCount; i > 0; i--)
			{
				if (CtmConfig::GetInstance()->GetRecordType() == 1)
				{
					CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPSTATE, i, &tmStateMode, sizeof(tmStateMode));
				}
				else
					CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, i, &tmStateMode);
				if (tmStateMode.nMode == DATAPROCESS_STATEMODE_POWERON) /*本次開機時間*/
				{
					if (!u_bPowerOn)
					{
						u_nPowerOnTime[0] = tmStateMode.date.year;
						u_nPowerOnTime[1] = tmStateMode.date.month;
						u_nPowerOnTime[2] = tmStateMode.date.day;
						u_bPowerOn = TRUE ;
						if (u_bPowerOff)
							break;
					}
				}
				if (tmStateMode.nMode == DATAPROCESS_STATEMODE_POWEROFF) /*上次關機時間*/
				{
					if (!u_bPowerOff)
					{
						u_nPowerOffTime[0] = tmStateMode.date.year;
						u_nPowerOffTime[1] = tmStateMode.date.month;
						u_nPowerOffTime[2] = tmStateMode.date.day;
						u_bPowerOff = TRUE ;
						if (u_bPowerOn)
							break;
					}
				}
			}

			if (u_bPowerOff) /*無關機時間則不做處理*/
			{
				if (u_nPowerOnTime[0] >= u_nPowerOffTime[0]) /*year*/
				{
					long long lDifferDay = 0;
					int			  Py = 360, Pm = 30; //系數

					lDifferDay = (u_nPowerOnTime[0] - u_nPowerOffTime[0]) * Py
					             + (u_nPowerOnTime[1] - u_nPowerOffTime[1]) * Pm
					             + (u_nPowerOnTime[2] - u_nPowerOffTime[2]);

					if (u_nPowerOnTime[0] > u_nPowerOffTime[0]) /*year*/
						lDifferDay += 1;
					//printf("lDifferDay=%d\n",lDifferDay);

					if (lDifferDay >= 0) /*開關機天數差*/
					{
						if (lDifferDay >= nPowerOnLub)
						{
							SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION", 1); /*開關機天數差 > 開機自動潤滑天數,則送1到主機,代表開機時需做潤滑*/
						}
						else
						{
							SetDBValue("MACH_LUB_ON_NULL_BYPASSOPTION", 0); /*開關機天數差 <= 開機自動潤滑天數,則送0到主機,代表開機時無需做潤滑*/
						}
					}
				}
			}
		}
	}
}

void	CtmDsp54All::ProsNotclmpTime()/*wangli2015-12-14 17:07:18  累計非開關模狀態 時間 =  關機時間 + 關機之前累計的時間*/
{
	DWORD   dwAllnotclmptime1 = 0;
	DWORD   dwAllnotclmptime2 = 0;
	DWORD   dwMachoffTime   = 0;
	DWORD	dwNotclmptime1	= 0;
	DWORD	dwNotclmptime2	= 0;

	dwNotclmptime1 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56").lValue;
	dwNotclmptime2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57").lValue;

	/*開關機時間差*/
	STATEMODE	tmStateModeHead;
	STATEMODE	tmStateMode;
	if (CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));

		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));
		tmStateModeHead.nCount = HeadInfo.nCount;
		tmStateModeHead.nIndex = HeadInfo.nIndex;
	}
	else
		CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, 0, &tmStateModeHead);
	BOOL  u_bPowerOn = FALSE, u_bPowerOff = FALSE;
	time_t 	t1, t2;
	struct tm timeprt1, timeprt2;

	memset(&timeprt1, 0, sizeof(tm));
	memset(&timeprt2, 0, sizeof(tm));
	//printf("nCount =%d\n",tmStateModeHead.nCount);
	if (tmStateModeHead.nCount > 1)
	{
		for (int i = tmStateModeHead.nCount; i > 0; i--)
		{
			if (CtmConfig::GetInstance()->GetRecordType() == 1)
			{
				CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPSTATE, i, &tmStateMode, sizeof(tmStateMode));
			}
			else
				CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, i, &tmStateMode);
			if (tmStateMode.nMode == DATAPROCESS_STATEMODE_POWERON) /*本次開機時間*/
			{
				if (!u_bPowerOn)
				{
					timeprt2.tm_year = tmStateMode.date.year - 1900;
					timeprt2.tm_mon  = tmStateMode.date.month - 1;
					timeprt2.tm_mday = tmStateMode.date.day;
					timeprt2.tm_hour = tmStateMode.time.hour;
					timeprt2.tm_min  = tmStateMode.time.minute;
					timeprt2.tm_sec  = 0;
					timeprt2.tm_wday = timeprt2.tm_yday = timeprt2.tm_isdst = 0;


					t2 = mktime(&timeprt2);

					u_bPowerOn = TRUE ;
					if (u_bPowerOff)
						break;
				}
			}
			if (tmStateMode.nMode == DATAPROCESS_STATEMODE_POWEROFF) /*上次關機時間*/
			{
				if (!u_bPowerOff)
				{
					timeprt1.tm_year = tmStateMode.date.year - 1900;
					timeprt1.tm_mon  = tmStateMode.date.month - 1;
					timeprt1.tm_mday = tmStateMode.date.day;
					timeprt1.tm_hour = tmStateMode.time.hour;
					timeprt1.tm_min  = tmStateMode.time.minute;
					timeprt1.tm_sec  = 0;
					timeprt1.tm_wday = timeprt1.tm_yday = timeprt1.tm_isdst = 0;


					t1 = mktime(&timeprt1);

					u_bPowerOff = TRUE ;
					if (u_bPowerOn)
						break;
				}
			}
		}

		if (u_bPowerOff) /*無關機時間則不做處理*/
		{

			dwMachoffTime = (DWORD) (difftime(t2, t1) / 60) ;

			dwAllnotclmptime1 = dwNotclmptime1 + dwMachoffTime;
			//cout << "dwAllnotclmptime1 = " << dwAllnotclmptime1 << endl;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56", dwAllnotclmptime1, FALSE);

			dwAllnotclmptime2 = dwNotclmptime2 + dwMachoffTime;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57", dwAllnotclmptime2, FALSE);


			SaveNotclmpTime();
		}
	}

	SetPowonLubHr();

}


void	CtmDsp54All::SetPowonLubHr()/*wangli2016-1-19 14:11:37 開機后  設定值 與 累計值進行比較*/
{
	DWORD		lTemp = 0, lTemp2 = 0;
	BOOL		b2LUB = FALSE ;

	lTemp2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED53").lValue;

	if (lTemp2 != 0)
	{
		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56").lValue;

		if (lTemp / 60 >= lTemp2)
		{
			SetDBValue("MACH_OTHR_OTHR_NULL_W01AB", 1);
		}
	}


	lTemp2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED54").lValue;
	if ((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 1) || (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 3))
		b2LUB = TRUE;
	else
		b2LUB = FALSE;
	if ((lTemp2 != 0) && b2LUB)
	{
		lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57").lValue;

		if (lTemp / 60 >= lTemp2)
		{
			SetDBValue("MACH_OTHR_OTHR_NULL_W01AC", 1);
		}
	}

}

void	CtmDsp54All::Check_PCTBL()//joyce2012-12-19
{
	char pDataID[256];
	int PCChannel = 0, PCDataChannel = 0;

	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED13", 0, FALSE);/*QMC*/

	for (int i = 0; i < MAX_PCTBL_NUM; i++)
	{
		sprintf(pDataID, "CONF_CODE_WRITE_CHL%d_OUTPUTDEFINITION", i + 1);

		for (int j = 0; j < (int)(sizeof(pChkPCValue) / sizeof(pChkPCValue[0])); j++)
		{
			if (!(GetDBValue(pDataID).lValue & 0x8000)   /*非空*/
			        && ((GetDBValue(pDataID).lValue & 0x03FF) == *(pChkPCValue + j))) /*高4位代表換點的點序;0x0400表示反邏輯*/
				m_nAirPCPoint[j] = i;

			//printf("pChkPCValue[%d]=%X,m_nAirPCPoint[%d]=%d\n",j,*(pChkPCValue+j),j,m_nAirPCPoint[j]);
		}

		if ((GetDBValue(pDataID).lValue & 0x0FFF) == 0x00EF)    /*高4位代表換點的點序  PC--MCS使能*/
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED13", 1, FALSE); /*QMC使用,PC自動搜索, O_MCS_ENA 0EFH ;MCS 使能*/
	}

	for (int i = 0; i < (int)(sizeof(pChkPCValue) / sizeof(pChkPCValue[0])); i++)
	{
		if (m_nAirPCPoint[i] != -1)
		{
			PCChannel     = (m_nAirPCPoint[i] / 16) / 2;       /*0~1*/           /*0~15,16~31,32~47,48~63,......*/
			PCDataChannel = (m_nAirPCPoint[i] % 32) / 16;       /*0~1*/          /*0~15,16~31,32~47,48~63,......*/

			sprintf(m_AirPCDataID[i], "STATE_IO%d_WRITE_NULL_REALDATA%d", PCChannel + 1, PCDataChannel + 1);
			//printf("m_AirPCDataID[%d]=%s\n",i,m_AirPCDataID[i]);
		}
	}
}

void	CtmDsp54All::Check_PBTBL()/*zhongjw 2015-8-29 14:03:15判斷相應PB點是否存在*/
{
	char pDataID[256];
	int PBChannel = 0, PBDataChannel = 0;
	for (int i = 0; i < MAX_PBTBL_NUM; i++)
	{
		sprintf(pDataID, "CONF_CODE_READ_CHL%d_INPUTDEFINITION", i + 1);
		if ((GetDBValue(pDataID).lValue & 0x0FFF) == 0x0255)   /*判斷PB點是否存在閥門動力模式  0x0255*/
		{
			PBChannel     = (i / 16) / 2;       /*0~1*/           /*0~15,16~31,32~47,48~63,......*/
			PBDataChannel = (i % 32) / 16;       /*0~1*/          /*0~15,16~31,32~47,48~63,......*/
			sprintf(m_ValPBDateID, "STATE_IO%d_READ_NULL_REALDATA%d", PBChannel + 1, PBDataChannel + 1);

			nValPBPoint = i;
			break;
		}

	}
}

void	CtmDsp54All::SetValMod()
{
	WORD	wTemp;
	int IODataBit;
	if (nValPBPoint != -1)
	{

		wTemp = GetDBValue(m_ValPBDateID).lValue;
		IODataBit  = (nValPBPoint % 32) % 16;      /*0~15*/          /*0~15,16~31,32~47,48~63,......*/

		if ((_TestBit(wTemp, IODataBit)) && (bPBSelected == FALSE))
		{
			SetDBValue("MSET_SCRW_HLDP_NULL_W068F", 1);/*閥門動力模式:氣動*/
			SetDBValue("MSET3_GATE_PUMP_NULL_PRESSURE", 0);
			SetDBValue("MSET3_GATE_PUMP_NULL_FLOW", 0);
			bPBSelected = TRUE;
		}
		else if ((!(_TestBit(wTemp, IODataBit))) && ((bPBSelected == TRUE) || (bPBInitFlag == FALSE)))
		{
			SetDBValue("MSET_SCRW_HLDP_NULL_W068F", 0);/*閥門動力模式：液壓*/
			SetDBValue("MSET3_GATE_PUMP_NULL_PRESSURE", 85);
			SetDBValue("MSET3_GATE_PUMP_NULL_FLOW", 50);
			bPBSelected = FALSE;

			if (bPBInitFlag == FALSE)
				bPBInitFlag = TRUE;
		}
	}

}


void	CtmDsp54All::ClearNotclmptime()
{
	WORD		 wState0;
	wState0 = (WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;
	if ((wState0 & 0x0005) != 0)/*wangli2016-1-15 14:31:04 判斷 是開關模狀態*/
	{
		if (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56").lValue)
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56", 0); /*wangli2016-1-15 14:31:08 馬達潤滑時數  清0*/

		if (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57").lValue)
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57", 0);

		//SaveNotclmpTime();  /*wangli2016-1-15 14:47:02 若一直 在做全自動，則 開關模 頻率較高，不執行存文件操作*/

	}

}


/*<<<<<<<<<<純二板------------------------------*/
void	 CtmDsp54All::SetClmpShutOffset()
{
	long lClmpShutOrig = GetDBValue("MACH_SHUT_SHUT_NULL_W0268").lValue;/*模座位置 抱閘基準點*/
	long lClmpShutDest = GetDBValue("MSET_SCRW_INJ_NULL_W0775").lValue; /*模座位置 抱閘目標位置*/
	long lGearDist     = GetDBValue("MACH_SHUT_SHUT_NULL_W026D").lValue;/*拉杆齒距*/
	long lTemp = 0;
	if (lGearDist)
	{
		if (lClmpShutDest > lClmpShutOrig)
			lTemp = (lClmpShutDest - lClmpShutOrig) % lGearDist;
		else
			lTemp = (lClmpShutOrig - lClmpShutDest) % lGearDist;

		if (lTemp > (lGearDist / 2)) /*JOYCE2014-5-28 模座位置抱閘偏差的絕對值如果大于拉杆齒距的1/2,則進行處理, 保証【模座位置抱閘偏差】<= (拉杆齒距/2)*/
		{
			lTemp = lGearDist - lTemp;
			bGearDist = TRUE;
		}
		else
			bGearDist = FALSE;
	}
	else
		lTemp = 0;

	SetDBValue("MSET_SCRW_INJ_NULL_W0774", lTemp);/*模座位置抱閘偏差*/
}

void	 CtmDsp54All::SetTBarShutDest()
{
	long long	lTemp1  = 0;
	long long	lTemp2 = GetDBValue("MSET_SCRW_INJ_NULL_W0774").lValue;/*模座位置抱閘偏差*/
	//long long lTemp3 = GetDBValue("SYS_HMI_NULL_NULL_RESERVED42").lValue;/*破模距離*/
	long lClmpShutOrig = GetDBValue("MACH_SHUT_SHUT_NULL_W0268").lValue;/*模座位置 抱閘基準點*/
	long lClmpShutDest = GetDBValue("MSET_SCRW_INJ_NULL_W0775").lValue; /*模座位置 抱閘目標位置*/

	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W0269").lValue;     /*拉杆1 抱閘基準點*/
	if (lClmpShutDest > lClmpShutOrig)
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2 * 10;
		else
			lTemp1  = lTemp1 + lTemp2 * 10;
	}
	else
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2 * 10;
		else
			lTemp1  = lTemp1 - lTemp2 * 10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0776", lTemp1);              /*拉杆1 抱閘目標位置*/
	//SetDBValue("MSET_SCRW_INJ_NULL_W077C", lTemp1 + lTemp3*10);  /*拉杆1 破模目標位置*/

	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W026A").lValue; /*拉杆2 抱閘基準點*/
	if (lClmpShutDest > lClmpShutOrig)
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2 * 10;
		else
			lTemp1  = lTemp1 + lTemp2 * 10;
	}
	else
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2 * 10;
		else
			lTemp1  = lTemp1 - lTemp2 * 10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0777", lTemp1);
	//SetDBValue("MSET_SCRW_INJ_NULL_W077D", lTemp1 + lTemp3*10);

	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W026B").lValue; /*拉杆3 抱閘基準點*/
	if (lClmpShutDest > lClmpShutOrig)
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2 * 10;
		else
			lTemp1  = lTemp1 + lTemp2 * 10;
	}
	else
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2 * 10;
		else
			lTemp1  = lTemp1 - lTemp2 * 10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0778", lTemp1);
	//SetDBValue("MSET_SCRW_INJ_NULL_W077E", lTemp1 + lTemp3*10);

	lTemp1 = GetDBValue("MACH_SHUT_SHUT_NULL_W026C").lValue; /*拉杆4 抱閘基準點*/
	if (lClmpShutDest > lClmpShutOrig)
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 - lTemp2 * 10;
		else
			lTemp1  = lTemp1 + lTemp2 * 10;
	}
	else
	{
		if (bGearDist == TRUE)
			lTemp1  = lTemp1 + lTemp2 * 10;
		else
			lTemp1  = lTemp1 - lTemp2 * 10;
	}
	SetDBValue("MSET_SCRW_INJ_NULL_W0779", lTemp1);
	//SetDBValue("MSET_SCRW_INJ_NULL_W077F", lTemp1 + lTemp3*10);
}

void	 CtmDsp54All::SetTBarShutDest2()
{
	long long	lTemp1  = 0;
	long long	lTemp2 = GetDBValue("MACH_SHUT_SHUT_NULL_W026D").lValue;   /*拉杆 齒距*/
	long long lTemp3 = GetDBValue("SYS_HMI_NULL_NULL_RESERVED42").lValue;/*破模距離*/

	lTemp1 = GetDBValue("METER_AD1_READ_CHL9_RELATIVEPOSITION").lValue;     /*拉杆1 現在位置*/
	if (lTemp1 > (lTemp2 * 10))
		lTemp1  = lTemp1 - lTemp2 * 10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0776", lTemp1);              /*拉杆1 抱閘目標位置*/
	SetDBValue("MSET_SCRW_INJ_NULL_W077C", lTemp1 + lTemp3 * 10); /*拉杆1 破模目標位置*/

	lTemp1 = GetDBValue("METER_AD1_READ_CHL10_RELATIVEPOSITION").lValue;     /*拉杆2 現在位置*/
	if (lTemp1 > (lTemp2 * 10))
		lTemp1  = lTemp1 - lTemp2 * 10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0777", lTemp1);
	SetDBValue("MSET_SCRW_INJ_NULL_W077D", lTemp1 + lTemp3 * 10);

	lTemp1 = GetDBValue("METER_AD1_READ_CHL11_RELATIVEPOSITION").lValue;     /*拉杆3 現在位置*/
	if (lTemp1 > (lTemp2 * 10))
		lTemp1  = lTemp1 - lTemp2 * 10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0778", lTemp1);
	SetDBValue("MSET_SCRW_INJ_NULL_W077E", lTemp1 + lTemp3 * 10);

	lTemp1 = GetDBValue("METER_AD1_READ_CHL12_RELATIVEPOSITION").lValue;     /*拉杆4 現在位置*/
	if (lTemp1 > (lTemp2 * 10))
		lTemp1  = lTemp1 - lTemp2 * 10;
	SetDBValue("MSET_SCRW_INJ_NULL_W0779", lTemp1);
	SetDBValue("MSET_SCRW_INJ_NULL_W077F", lTemp1 + lTemp3 * 10);
}

void	 CtmDsp54All::SaveTBARData()
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
		for (int i = 0; i < 20; i++)
		{
			if (pDataID[i] != NULL)	wTBARData[i] = GetDBValue(pDataID[i]).lValue;
			//printf("wTBARData22[%d]:%d\n",i,wTBARData[i]);
		}
	}
	write(nFd, &wTBARData, sizeof(wTBARData));
	close(nFd);
}

void	 CtmDsp54All::SaveOpnCntData()/*zhongjw 2015-11-13 8:51:12試車模式邊上的開模總數不可重置*/
{
	WORD wOpnCntData = 0;
	char*   pDataID = "SYSX_OTHERS_OTHERS_INT_RESERVED113";
	char	szPath[256];
	char	m_szCopyFile[256];

	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/OpnCntData");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/OpnCntData.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);

	if (nFd >= 0)
	{
		if (pDataID != NULL)
			wOpnCntData = GetDBValue(pDataID).lValue;
	}
	write(nFd, &wOpnCntData, sizeof(wOpnCntData));
	close(nFd);
}

void	 CtmDsp54All::SaveWarningCnt()/*zhongjw 2015-12-23 模座組態畫面，鎖模壓力監測燈警報次數不可重置*/
{
	WORD wWarningCntData = 0;
	char*   pDataID = "MSET_SCRW_HLDP_NULL_W068E";
	char	szPath[256];
	char	m_szCopyFile[256];

	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/WarningCntData");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/WarningCntData.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);

	if (nFd >= 0)
	{
		if (pDataID != NULL)
			wWarningCntData = GetDBValue(pDataID).lValue;
	}
	write(nFd, &wWarningCntData, sizeof(wWarningCntData));
	close(nFd);
}

void		CtmDsp54All::SaveParallelData()		/*zhongjw 2015-4-13 11:07:32 純二版機偏差計數和拉桿水平位置不可重置*/
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
		for (int i = 0; i < 11 ; i++)
		{
			if (pDataID[i] != NULL)	dwCntData[i] = GetDBValue(pDataID[i]).lValue;
		}
	}

	write(nFd, &dwCntData, sizeof(dwCntData));
	close(nFd);
}


void		CtmDsp54All::SaveTBarParalleltData()/*zhongjw 2015-11-17 10:10:13平行度記錄功能*/
{
	tmDATE  date;
	tmTIME  time;
	GetDate(&date);
	GetTime(&time);
	char	sz0[128], szPath[128], m_szCopyFile[128], pszTime[128];
	WORD  w_Offset[8], wIndex = 0, wCnt = 0; ;
	DWORD		dwValue = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == 7)
		{
			sprintf(sz0, "MSET_OTHR_OTHR_NULL_W084%c", 'A');
		}
		else
			sprintf(sz0, "MSET_OTHR_OTHR_NULL_W084%d", i + 3);

		w_Offset[i] = GetDBValue(sz0).lValue;
	}

	dwValue = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue + 1; /*開模總數*/
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/TBarParallel/");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/TBarParallel.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{
		lseek(nFd, 0, SEEK_SET);
		read(nFd, &wCnt, sizeof(wCnt));
		read(nFd, &wIndex, sizeof(wIndex));
		if (wIndex == 10000) wIndex = 0;
		lseek(nFd, 32 * wIndex + 32, SEEK_SET);
		write(nFd, &date, sizeof(date));
		write(nFd, &time, sizeof(time));
		write(nFd, &dwValue, sizeof(dwValue));
		write(nFd, w_Offset, sizeof(w_Offset));
		sprintf(pszTime, "%04u-%02u-%02u; %02u:%02u:%02u;%10ld:%6d %6d %6d %6d %6d %6d %6d %6d\n",
		        date.year, date.month, date.day, time.hour, time.minute, time.second, dwValue, w_Offset[0], w_Offset[1], w_Offset[2], w_Offset[3], w_Offset[4], w_Offset[5], w_Offset[6], w_Offset[7]);
		wIndex++;
		wCnt++;
		if (wCnt > 10000) wCnt = 10000;
		lseek(nFd, 0, SEEK_SET);
		write(nFd, &wCnt, sizeof(wCnt));
		write(nFd, &wIndex, sizeof(wIndex));
	}
	close(nFd);
}

long	CtmDsp54All::ChangeVoltage(char pDataID[256], int ChannelNO, int nValue)
{
	WORD	wPMathch2[16], wDAOut2[16];
	int   nDAOut = 0, nK = 0;
	char pDataID2[256];
	long long lMaxHydraulic = 0;

	sprintf(pDataID2, "CONF_PVL%d_READ_NULL_MAXDATA", ChannelNO);
	if (GetDBValue(pDataID2).lValue)
		lMaxHydraulic = GetDBValue(pDataID2).lValue;
	else
	{
		if (ChannelNO % 2)
			lMaxHydraulic = 140;
		else
			lMaxHydraulic = 99;
	}

	for (int i = 0; i < 16; i++)
	{
		sprintf(pDataID2, "PMATCH2_PVL%d_READ_CHL%d_DATA", ChannelNO, i + 1);
		wPMathch2[i] = GetDBValue(pDataID2).lValue;
		//printf("wPMathch2[%d]=%d\n", i, wPMathch2[i]);

		sprintf(pDataID2, "PMATCH_PVL%d_READ_CHL%d_DATA", ChannelNO, i + 1);
		wDAOut2[i] = GetDBValue(pDataID2).lValue;
		//printf("wDAOut2[%d]=%d\n", i, wDAOut2[i]);
	}

	for (int i = 0; i < 16; i++)
	{
		if (nValue == wPMathch2[i])
		{
			nDAOut = wDAOut2[i];
			break;
		}
		else	if (i == 0)
		{
			if (nValue < wPMathch2[i])
			{
				nK = wDAOut2[i] / wPMathch2[i];
				nDAOut = nK * nValue;
				break;
			}
		}
		else	if (i == 15)
		{
			if (nValue > wPMathch2[i])
			{
				if (lMaxHydraulic != wPMathch2[i])
				{
					nK = (4095 - wDAOut2[i]) / (lMaxHydraulic - wPMathch2[i]);
					nDAOut = nK * (nValue - wPMathch2[i]) + wDAOut2[i];
				}
				break;
			}
		}
		else	if ((nValue > wPMathch2[i]) && (nValue < wPMathch2[i + 1]))
		{
			if (wPMathch2[i + 1] != wPMathch2[i])
			{
				nK = (wDAOut2[i + 1] - wDAOut2[i]) / (wPMathch2[i + 1] - wPMathch2[i]);
				nDAOut = nK * (nValue - wPMathch2[i]) + wDAOut2[i];
			}
			//printf("%d, nDAOut=%d\n", i, nDAOut);
			break;
		}
	}
	return nDAOut;
}


void		CtmDsp54All::SaveNotclmpTime()/*wangli2015-12-10 17:04:14*/
{
	DWORD dwNotclmptime[2];
	char*   pDataID[] = {
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED56",
		"SYSX_OTHERS_OTHERS_DWORD_RESERVED57",
		NULL
	};

	memset(dwNotclmptime, 0, sizeof(dwNotclmptime));

	char	szPath[256];
	char	m_szCopyFile[256];

	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/notclmptime.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);

	if (nFd >= 0)
	{
		for (int i = 0; i < 2 ; i++)
		{
			if (pDataID[i] != NULL)	dwNotclmptime[i] = GetDBValue(pDataID[i]).lValue;
			//printf("54all dwNotclmptime22[%d]:%d\n",i,dwNotclmptime[i]);
		}
	}
	write(nFd, &dwNotclmptime, sizeof(dwNotclmptime));
	close(nFd);
}

int		CtmDsp54All::SetUnitChangetoBar(char pDataID[256])
{
	long  lTemp = 0, lValue = 0;

	long long lMaxClmpPres	= GetDBValue("CONF_CODE_WRITE_NULL_MAXCLAMPUPPRESSURE").lValue;			//關模高壓壓力上限
	WORD  	  nPVLTemp = HEX2BCD(lMaxClmpPres);
	if (nPVLTemp == 0)
		nPVLTemp = 140;

	if (pDataID != NULL)
	{
		lValue = GetDBValue(pDataID).lValue;
		if (GetDBValue("MACH_MLD_CLOSE_CLAMPUP_ALARMTONES").lValue)
			lTemp = (lValue * nPVLTemp * 10 / GetDBValue("MACH_MLD_CLOSE_CLAMPUP_ALARMTONES").lValue + 5) / 10; /*公式是X*nPVLTemp/機器噸位, 然后四舍五入*/
		else
			lTemp = 0;
		return 	lTemp;
	}

	return lTemp;
}

/*純二板------------------------------>>>>>>>>>>>>>>>>>*/

//void	 CtmDsp54All::ProcessAmmeterEnergy()
//{
//	if(g_ptaskAmmeter == NULL) return;
//
//	N2CAMMETERCONFIG	N2CAmmeterConf;
//	N2CAmmeterConf = *((N2CAMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));
//
//	COM2AMMETERCONFIG	COM2AmmeterConf;
//	COM2AmmeterConf = *((COM2AMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));
//
//	DWORD  	dwAmmeter1=0, dwAmmeter2=0, dwAmmeter3=0;
//
//	if((m_bAmmeter == 2) || (m_bAmmeter == 3))
//	{
//		//Ammeter1
//		dwAmmeter1 	= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;
//		printf("into auto dwAmmeter1=%d\n",dwAmmeter1);
//		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED69", dwAmmeter1);//Ammeter1
//
//		//Ammeter2
//		if(N2CAmmeterConf.wCount > 1)
//		{
//			dwAmmeter2 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue;
//			printf("into auto dwAmmeter2=%d\n",dwAmmeter2);
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED70", dwAmmeter2);//Ammeter2
//		}
//
//		//Ammeter3
//		if(N2CAmmeterConf.wCount > 2)
//		{
//			dwAmmeter3 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue;
//			printf("into auto dwAmmeter3=%d\n",dwAmmeter3);
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED71", dwAmmeter3);//Ammeter3
//		}
//	}
//	else
//	{
//		//Ammeter1
//		if(COM2AmmeterConf.AmmeterInfo[0].wCTPara != 0)
//			dwAmmeter1 	= (GetDBValue("AMMETER_HMI_REAL_POWER").lValue) *COM2AmmeterConf.AmmeterInfo[0].wCTPara/10;
//		else
//			dwAmmeter1 	= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;
//		printf("into auto dwAmmeter1=%d\n",dwAmmeter1);
//		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED69", dwAmmeter1);//Ammeter1
//
//		//Ammeter2
//		if(COM2AmmeterConf.wCount > 1)
//		{
//			if(COM2AmmeterConf.AmmeterInfo[1].wCTPara != 0)
//				dwAmmeter2 	= (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue) *COM2AmmeterConf.AmmeterInfo[1].wCTPara/10;
//			else
//				dwAmmeter2 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue;
//			printf("into auto dwAmmeter2=%d\n",dwAmmeter2);
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED70", dwAmmeter2);//Ammeter2
//		}
//
//		//Ammeter3
//		if(COM2AmmeterConf.wCount > 2)
//		{
//			if(COM2AmmeterConf.AmmeterInfo[2].wCTPara != 0)
//				dwAmmeter3 	= (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue) *COM2AmmeterConf.AmmeterInfo[2].wCTPara/10;
//			else
//				dwAmmeter3 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue;
//			printf("into auto dwAmmeter3=%d\n",dwAmmeter3);
//			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED71", dwAmmeter3);//Ammeter3
//		}
//	}
//}

void	     CtmDsp54All::BootTimeCntDown()
{
	if (GetDBValue("MACH_OTHR_OTHR_NULL_W01CE").lValue)
	{
		//printf("g_lCounterBootTime=%d\n",g_lCounterBootTime);
		if (g_lCounterBootTime <= 0)
			SetDBValue("MACH_OTHR_OTHR_NULL_W01CE", 0);
	}
}

BOOL     CtmDsp54All::CheckDataID(char* pszData, char* pszData2[], int Length) //2009-9-30
{
	short       i = 0;
	//printf("Length =%d\n",Length);
	for (i = 0; i < Length; i++)
	{
		if (strcmp(pszData, pszData2[i]) == 0) return TRUE;
	}
	return FALSE;
}

void     CtmDsp54All::TempAutRecog()
{
	char	szPath[256];
	char	m_szCopyFile[256];
	WORD wDspType = 0;
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/usr/Data/temp");
	ExistDir(szPath);

	sprintf(m_szCopyFile, "%s/TempOil.cdb", szPath);

	int nFd = 0;

	wDspType = (WORD)GetDBValue("CONF_CODE_WRITE_NULL_DATA14").lValue;
	//wDspType &= 0xFFF0;
	wDspType = wDspType >> 4;
	if (wDspType == 0x553) /*wangli2016-1-21 14:48:01 @9A ==553,表示 5530*/
	{
		nFd =  open(m_szCopyFile, O_RDWR);

		if (nFd < 0)
		{
			printf("m_bOld5580OrNew\n");
			m_bOld5580OrNew = TRUE;
			nFd =  open(m_szCopyFile,  O_CREAT);

		}

	}
	else  /*wangli2016-1-21 14:48:01 @9A ！=553,表示 5580*/
	{
		nFd =  open(m_szCopyFile, O_RDWR);
		if (nFd >= 0)
		{

			usleep(50 * 1000);
			system("rm -rf /usr/Data/temp/TempOil.cdb");
			m_bOld5530Flag = TRUE; /*wangli2016-1-21 15:15:27 表示 之前為5530的主機，改為5580的主機*/
			m_bOld5530Flag2 = TRUE; /*wangli2016-1-21 15:15:27 表示 之前為5530的主機，改為5580的主機*/
		}
	}
	close(nFd);
}

void     CtmDsp54All::TempAutProcess()
{
	char	szPath[256];
	char	m_szCopyFile[256];
	char*	psz28_DB[] = {
		"TEMPERCARD1_INTERFACE_WSEG9_USE",
		"TEMPERCARD1_INTERFACE_WSEG10_USE",
		"TEMPERCARD1_INTERFACE_WSEG11_USE",
		"TEMPERCARD1_INTERFACE_WSEG12_USE",
		"M3_CONF_TEMPERCARD_HEATE_TYPE1",

	};

	int nNum = sizeof(psz28_DB) / sizeof(psz28_DB[0]);

	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/usr/Data/temp");
	sprintf(m_szCopyFile, "%s/TempOil.cdb", szPath);


	int nFd = 0;

	nFd = open(m_szCopyFile, O_RDWR);
	if (nFd >= 0) /*wangli2016-1-21 15:41:49 文件存在，說明是5530*/
	{
		if (m_bOld5580OrNew)
		{
			m_bOld5580OrNew = FALSE;

			SetDBValue("MACH_OTHR_OTHR_NULL_W01CF", 0, FALSE);
			SetDBValue("SYSX_TEMPER_HEATERON_NULL_SHOWHEATWAY", 0, FALSE);
			//SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP11_DEGREE", 0);

			long long lTemp1 = 0;
			lTemp1 = GetDBValue("M3_CONF_TEMPERCARD_HEATE_TYPE1").lValue; /*wangli2016-1-25 13:12:35 加熱類型*/

			SetDBValue("M3_CONF_TEMPERCARD_HEATE_TYPE1", lTemp1 | 0x000003FF, FALSE);

		}

		SetDBValue("TEMPERCARD1_INTERFACE_WSEG11_USE", 0, FALSE);        /*溫度卡*/

		SetDBValue("TEMPERCARD1_INTERFACE_WSEG12_USE", 0, FALSE);        /*溫度卡 油溫*/
		SetDBValue("TEMPERCARD1_INTERFACE_WSEG10_USE", 3, FALSE);        /*溫度卡 油溫*/

		if (g_ptaskdsp28_M3 != NULL)
			g_ptaskdsp28_M3->WriteValue(9, nNum, psz28_DB);


	}
	else
	{
		if (m_bOld5530Flag) /*wangli2016-1-21 16:02:33 由 5530切成5580*/
		{
			m_bOld5530Flag = FALSE;
			SetDBValue("MACH_OTHR_OTHR_NULL_W01CF", 0, FALSE);
			SetDBValue("SYSX_TEMPER_HEATERON_NULL_SHOWHEATWAY", 0, FALSE);
			SetDBValue("M3_CONF_TEMPERCARD_HEATE_TYPE1", 0, FALSE);

			SetDBValue("TEMPERCARD1_INTERFACE_WSEG9_USE", 0, FALSE);        /*溫度卡*/

		}

		SetDBValue("TEMPERCARD1_INTERFACE_WSEG12_USE", 3, FALSE);        /*溫度卡 油溫*/

		if (!g_pExTable->GetTableValue(0, "D_T11PORT19"))
			SetDBValue("TEMPERCARD1_INTERFACE_WSEG10_USE", 0, FALSE);        /*溫度卡 清除5530油溫*/

		if (g_ptaskdsp28_M3 != NULL)
			g_ptaskdsp28_M3->WriteValue(9, nNum, psz28_DB);

	}
	close(nFd);


}

void     CtmDsp54All::TempAutProcess2()
{
	char	szPath[256];
	char	m_szCopyFile[256];

	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/usr/Data/temp");
	sprintf(m_szCopyFile, "%s/TempOil.cdb", szPath);


	int nFd = 0;

	nFd = open(m_szCopyFile, O_RDWR);
	if (nFd >= 0) /*wangli2016-1-21 15:41:49 文件存在，說明是5530*/
	{
		//if(m_bOld5580OrNew)
		SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP12_DEGREE", 0, FALSE);
		SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP11_DEGREE", 0, FALSE);
		SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP10_DEGREE", 0, FALSE);
	}
	else
	{
		if (m_bOld5530Flag2) /*wangli2016-1-21 16:02:33 由 5530切成5580*/
		{
			m_bOld5530Flag2 = FALSE;
			SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP9_DEGREE", 0, FALSE);
		}

		SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP12_DEGREE", 0, FALSE);
		if (!g_pExTable->GetTableValue(0, "D_T11PORT19"))
			SetDBValue("MACH3_TMP1_HEATERONOFF_RAMP10_DEGREE", 0, FALSE);        /*溫度卡 清除5530油溫*/
	}
	close(nFd);


}


#ifndef	D_BEOBJECT_CTMDSP54ALL
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
		(static_cast<CtmPackClass*>(pResult))->CreateSelf();
	return pResult;


}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmPackClass*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
