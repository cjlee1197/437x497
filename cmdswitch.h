/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    :  		                                             			 |
|  Task     :  			                         							 |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 01/08/2009	                                                 |
|  Revision : 							                                     |
+===========================================================================*/
#ifndef	D_CMDSWITCH
#define	D_CMDSWITCH

#include 	"oil54struct.h"
#include	"tmconfig.h"

#define			MAX_COMM_COUNT			76

extern	int			g_DSP54CMD[MAX_COMM_COUNT][2];

extern	int			GetCmdSwitch(int nCmd);


#define     COMM_NULL                   0x0000
#define     COMM_SENDCONFIG             0x3130
#define     COMM_SENDZEROSET            0x3131
#define     COMM_SENDREQUESTCONFIG      0x3132      	//wy040600 WY113099
#define     COMM_SENDPOWERMATCH2        0x3135     		//Andy 20061212 for New D/A
#define     COMM_SENDPANEL              0x3200
#define     COMM_SENDPANELBREAK         0x32FF
#define     COMM_SENDCONTROL            0x3330
#define     COMM_SENDCONTROLINDEX       0x3331
#define     COMM_SENDCONTROLMACHINE     0x3332
#define     COMM_SENDCONTROLMOLDSET     0x3333
#define     COMM_SENDCONTROLMONITOR     0x3334
#define     COMM_SENDPOWERMATCH         0x3335      	// St1108'01 for D/A 
#define     COMM_SENDCOMMAND            0x3336      	// St0502'03  
#define     COMM_SENDCONTROLMACHINEB    0x3337
#define     COMM_SENDCONTROLMOLDSETB    0x3338
#define     COMM_SENDCONTROLMONITORB    0x3339
#define     COMM_SENDDIAGRM             0x3430
#define     COMM_SENDDIAGWM             0x3431
#define     COMM_SENDDIAGSM             0x3432
#define     COMM_SENDDIAGRI             0x3433
#define     COMM_SENDDIAGWO             0x3434
#define     COMM_SENDDIAGSI             0x3435
#define     COMM_SENDASSIGNINPUT        0x3436
#define     COMM_SENDASSIGNOUTPUT       0x3437
#define     COMM_SENDSTATUS             0x3530
#define     COMM_SENDR_PROFILE01        0x3630      	// Request of profile 01
#define     COMM_SENDS_PROFILE01        0x3631      	// Stop    of profile 01
#define     COMM_SENDR_BPROFILE01       0x3632      	// B Request of profile 01
#define     COMM_SENDS_BPROFILE01       0x3633      	// B Stop    of profile 01

#define		COMM_SENDTRANSMITDSP28		0x3730			//james add 2010/3/2 上午 09:08:38 轉發協議
#define     COMM_GETCOLDSTART           0x4141
#define     COMM_GETWARMSTART           0x4142
#define     COMM_GETCONFIG              0x4241
#define     COMM_GETDAOUTPUT            0x4242      	// St1108'01 for D/A
#define     COMM_GETDAOUTPUT2           0x4243     		//Andy 20061212 for New D/A
#define     COMM_GETOPSTATE             0x4341
#define     COMM_GETOPSTATUS            0x4342
#define     COMM_GETOPMETER             0x4343
#define     COMM_GETOPMETERZEROAGAIN    0x4841      	//"HA" ZERO SETTING AGAIN WY042099
#define     COMM_GETOPTEMPERATURE       0x4344
#define     COMM_GETOPSUMMARY           0x4345      	// St020101 for update quickly
#define     COMM_GETCURVEDATA           0x4346      	// WJ0610'03 add for command =>54
#define     COMM_GETAUSTIONDRIVEINFO    0x4347			//"CG" Get auston drive motor infomation
#define     COMM_GETOPSTATE2            0x4348 		//Andy 20091208
#define     COMM_GETCONTROLINDEX        0x4441
#define     COMM_GETPRODUCTION          0x4541
#define     COMM_GETMONICURRENT         0x4542
#define     COMM_GETMONILAST            0x4543
#define     COMM_GETMONICURRENTB        0x4545
#define     COMM_GETMONILASTB           0x4546
#define     COMM_GETMONIMAXMINB         0x4547
#define     COMM_GETDIAGRM              0x4641
#define     COMM_GETDIAGRI              0x4642
#define     COMM_GETS_PROFILE01_I       0x4741      	// Start of profile 01 -- Inject
#define     COMM_GETD_PROFILE01_I       0x4742      	// Data  of profile 01 -- Inject
#define     COMM_GETE_PROFILE01_I       0x4743      	// End   of profile 01 -- Inject
#define     COMM_GETS_PROFILE01_H       0x4744      	// Start of profile 01 -- Hold
#define     COMM_GETD_PROFILE01_H       0x4745      	// Data  of profile 01 -- Hold
#define     COMM_GETE_PROFILE01_H       0x4746      	// End   of profile 01 -- Hold
#define     COMM_GETS_BPROFILE01_I      0x4747      	// Start of profile 01 -- Inject
#define     COMM_GETD_BPROFILE01_I      0x4748      	// Data  of profile 01 -- Inject
#define     COMM_GETE_BPROFILE01_I      0x4749      	// End   of profile 01 -- Inject
#define     COMM_GETS_BPROFILE01_H      0x474A      	// Start of profile 01 -- Hold
#define     COMM_GETD_BPROFILE01_H      0x474B      	// Data  of profile 01 -- Hold
#define     COMM_GETE_BPROFILE01_H      0x474C      	// End   of profile 01 -- Hold

#define		COMM_BPROFILE01_R			0x6000		//James add new type 2008/6/6 01:35下午
#define		COMM_BPROFILE01_S			0x6001		//James add new type
#define		COMM_GETDAFLAG				0x6500		//James add new type
#define		COMM_INIT_MESSAGE			0x6501		//James add new type
#define		COMM_INJECT_TIME			0x6502		//	fans add 2008/10/17 10:54上午
#define     COMM_GETSTATERX             0x6503
#define     COMM_BGOTCONFIG             0x6504
#define    	COMM_GETCOMMAND             0x6505		//James add 2010/2/25 下午 03:22:36
#define		COMM_GETSTATE				0x7000
#define		COMM_SETDSP28IP				0x7200		//James add 2010/5/10 for set dsp28 IP

/*===========================================================================+
|           NewEthernet update HMI->DSP54 CMD define                         |
+===========================================================================*/  
#define		COMM_ETH_SENDDLSTART		0x3630		//  60 更新程式命令請求
#define		COMM_ETH_SENDDLDATA			0x3631		//  61 開始更新程式(HMI->Dsp54)
#define		COMM_ETH_SENDDLEND			0x3632		//  62 更新程式結束(HMI->Dsp54)
#define		COMM_ETH_SENDDLREADY		0x3633		//  63 更新準備好以及更新完成(DSP54->HMI 階段確認)
#define		COMM_ETH_SENDDLSMALL		0x3634		//	64 更新DL SMALL程式命令(HMI->Dsp54)
#define		COMM_ETH_SENDDLEXIT			0x3635		//	65 更新結束束
#define		COMM_ETH_MODE_TRANSFORM		0x3830		//	80
#define		COMM_ETH_MODE_NORMAL		0x3832		//	82

#define		COMM_ETHNEW_SENDDLSTART		0xFF3630		//  60 更新程式命令請求
#define		COMM_ETHNEW_SENDDLDATA		0xFF3631		//  61 開始更新程式(HMI->Dsp54)
#define		COMM_ETHNEW_SENDDLEND		0xFF3632		//  62 更新程式結束(HMI->Dsp54)
#define		COMM_ETHNEW_SENDDLREADY		0xFF3633		//  63 更新準備好以及更新完成(DSP54->HMI 階段確認)
#define		COMM_ETHNEW_SENDDLSMALL		0xFF3634		//	64 更新DL SMALL程式命令(HMI->Dsp54)

/*===========================================================================+
|           NewEthernet update DSP54->HMI CMD define                         |
+===========================================================================*/
#define		COMM_ETH_GETDLSTART			0x4641		// FA 上傳程式命令請求
#define		COMM_ETH_GETDLDATA 			0x4642		// FB 開始更新程式(DSP54->HMI)
#define		COMM_ETH_GETDLEND			0x4643		// FC 更新程式結束(DSP54->HMI)
#define		COMM_ETH_GETDLREADY			0x4644		// FD 更新準備好以及更新完成(HMI->DSP54 階段確認)
#define		COMM_ETH_GETDLSMALL			0x4645		// FE 回復DL SMALL程式更新完成
#define		COMM_ETH_GETDLEXIT			0x4646		// FF DSP55 echo,HMI download exit

#define		COMM_ETHNEW_GETDLSTART		0xFF4641		// FA 上傳程式命令請求
#define		COMM_ETHNEW_GETDLDATA 		0xFF4642		// FB 開始更新程式(DSP54->HMI)
#define		COMM_ETHNEW_GETDLEND		0xFF4643		// FC 更新程式結束(DSP54->HMI)
#define		COMM_ETHNEW_GETDLREADY		0xFF4644		// FD 更新準備好以及更新完成(HMI->DSP54 階段確認)
#define		COMM_ETHNEW_GETDLSMALL		0xFF4645		// FE 回復DL SMALL程式更新完成
/*===========================================================================+
|           NewEthernet HMI CMD define                                       |
+===========================================================================*/  
#define		COMM_ETH_SENDZEROSET			0x9001			//  ZeroSet
#define		COMM_ETH_SENDCONFIG				0x9002			//  Config
#define		COMM_ETH_SENDCONNET				0x9003			//  connet
#define		COMM_ETH_SENDSTATUS				0x9004			//	SendState
                                        	
#define		COMM_ETH_GETDLSTATE				0x9005			//  GetDLState
#define		COMM_ETH_SETDLSTATE				0x9006			// 	SetDLState
#define		COMM_ETH_GETDLOLDVERS			0x9007			//  GetDLOldVers
#define		COMM_ETH_SETDLOLDVERS			0x9008			//  SetDLOldVers
#define		COMM_ETH_GETDLCMDSTATE			0x9009			//	GetDLCMDState
#define		COMM_ETH_SENDTYPE				0x900A			//	SendType
#define		COMM_ETH_DEVICEID				0x900B			//	Selcet 28 or 54
#define		COMM_ETH_SELECT28				0x900C			//	28CMD
#define		COMM_ETH_SELECT54				0x900D			//	54CMD

#define		COMM_ETH_SENDCONTROLMACHINE		0x900E			//  30 上段參數資料
#define		COMM_ETH_SENDCONTROLMOLDSET		0x900F			//	32 上段模組資料
#define		COMM_ETH_SENDCONTROLMOLDSETB	0x9010			//	35 B座上段模組資料

#define		COMM_SENDTRANSMITCMD			0x9011			//  send transmit cmd
#define		COMM_SENDTRANSMITBLOCK			0x9012			//	send transmit block cmd
#define		COMM_SENDTRANSMITSTATE			0x9013			//  send transmit cmd State

#define		COMM_ETH_SENDRUNTIME			0x9014			//	run cmd
#define		COMM_ETH_SELECT54TRANSMIT		0x9015			//	54TransmitCMD
#define		COMM_ETH_GETPACKCOUNT			0x9016			//	get current package count
#define		COMM_ETH_SETSUFFIXNAME			0x9017			//	set suffix name for update	
#define		COMM_ETH_SETBURNTYPE			0x9018			//  upload or download
#define		COMM_ETH_GETTRANSFORM_ECHO		0x9019
#define		COMM_ETH_SENDCONTROLNEWMOLDSET2	0x9020			//	38 上段模組資料
#define		COMM_ETH_SENDCONTROLNEWMOLDSET3	0x9021			//	41 上段模組資料
#define		COMM_AUSTONE_CHECKDATA			0xA000			//  Austone CheckData

#define		COMM_ETHNEW_SENDZEROSET			0xFF9001		//  ZeroSet
#define		COMM_ETHNEW_SENDCONFIG			0xFF9002		//  Config
#define		COMM_ETHNEW_SENDCONNET			0xFF9003		//  connet
#define		COMM_ETHNEW_SENDSTATUS			0xFF9004		//	SendState

#define		COMM_ETHNEW_GETDLSTATE			0xFF9005		//  GetDLState
#define		COMM_ETHNEW_SETDLSTATE			0xFF9006		// 	SetDLState
#define		COMM_ETHNEW_GETDLOLDVERS		0xFF9007		//  GetDLOldVers
#define		COMM_ETHNEW_SETDLOLDVERS		0xFF9008		//  SetDLOldVers
#define		COMM_ETHNEW_GETDLCMDSTATE		0xFF9009		//	GetDLCMDState
#define		COMM_ETHNEW_SENDTYPE			0xFF900A		//	SendType
#define		COMM_ETHNEW_DEVICEID			0xFF900B		//	Selcet 28 or 54
#define		COMM_ETHNEW_SELECT28			0xFF900C		//	28CMD
#define		COMM_ETHNEW_SELECT54			0xFF900D		//	54CMD

#define		COMM_ETHNEW_SENDCONTROLMACHINE		0xFF900E	//  30 上段參數資料
#define		COMM_ETHNEW_SENDCONTROLMOLDSET		0xFF900F	//	32 上段模組資料
#define		COMM_ETHNEW_SENDCONTROLMOLDSETB		0xFF9010	//	35 B座上段模組資料
/*===========================================================================+
|           NewEthernet HMI->DSP54 CMD define                                |
+===========================================================================*/ 
#define		COMM_ETH_SENDREQUESTREAD			0x3132		//	12 請求讀
#define		COMM_ETH_SENDUPDATEDATA		 		0x3133		//	13 請求寫
#define     COMM_ETH_SENDPANEL              	0x3230		//  20 按鍵傳輸
#define     COMM_ETH_SENDCOMMAND             	0x3231		//  21 DSP XPLC上傳資料使用
#define     COMM_ETH_SENDPANELBREAK         	0x32FF		//	   松開按鍵
#define		COMM_ETH_SENDCONTROLMACHINE1		0x3330		//  30 上段參數資料
#define		COMM_ETH_SENDCONTROLMACHINE2		0x3331		//	31 下段參數資料
#define		COMM_ETH_SENDCONTROLMOLDSET1		0x3332		//	32 上段模組資料
#define		COMM_ETH_SENDCONTROLMOLDSET2		0x3333		//	33 下段模組資料
#define		COMM_ETH_SENDCONTROLMACHINEB		0x3334		//	34 B座參數資料
#define		COMM_ETH_SENDCONTROLMOLDSETB1		0x3335		//	35 B座上段模組資料
#define		COMM_ETH_SENDCONTROLMOLDSETB2		0x3336		//	36 B座下段模組資
#define		COMM_ETH_SENDCONTROLNEWMACHINE2		0x3337		//	37	MACHINE2
#define		COMM_ETH_SENDCONTROLNEWMOLDSET2_1	0x3338		//	38	MOLDSET2_1
#define		COMM_ETH_SENDCONTROLNEWMOLDSET2_2	0x3339		//	39	MOLDSET2_1
#define		COMM_ETH_SENDCONTROLNEWMACHINE3		0x3430		//	40 	MACHINE3
#define		COMM_ETH_SENDCONTROLNEWMOLDSET3_1	0x3431		//	41 	MOLDSET3_1
#define		COMM_ETH_SENDCONTROLNEWMOLDSET3_2	0x3432		//	42 	MOLDSET3_2
#define		COMM_ETH_SENDDIAGRMRI				0x3435		//0x3430	modify new gate 20141008	//	40 讀取DSP ROM&RAM DSP INPUT
#define		COMM_ETH_SENDDIAGWM					0x3436		//0x3431	modify new gate 20141008	//	41 寫入DSP RAM
#define		COMM_ETH_SENDDIAGWO					0x3437		//0x3432	modify new gate 20141008	//	42 寫入DSP OUTPUT
#define		COMM_ETH_SENDASSIGNINPUT			0x3438		//0x3433	modify new gate 20141008	//	43 對換 DSP INPUT
#define		COMM_ETH_SENDASSIGNOUTPUT			0x3439		//0x3434	modify new gate 20141013    //	44 對換 DSP OUTPUT
#define		COMM_ETH_SENDMONILIMIT				0x3530		//	50 監測值上下限
#define		COMM_ETH_SENDMONBILIMIT				0x3531		//	51 B座監測值上下限
//#define		COMM_ETH_SENDCONFIG				0x3130			// 	10 組態資料
//#define		COMM_ETH_SENDZEROSET			0x3131			//	11 A/D Offset歸零資料
//#define		COMM_ETH_SENDPOWERMATCH2		0x3132			//	12 比例閥輸出曲線
//#define		COMM_ETH_SENDREQUESTCONFIG 		0x3136			//	16 請求組態
//#define		COMM_ETH_SENDDIAGSI				0x3435			//	45 結束讀取DSP INPUT
//#define		COMM_ETH_SENDDIAGSM				0x3432			//	42 結束讀取DSP ROM&RAM
//#define		COMM_ETH_SENDDIAGRI				0x3433			//	43 讀取DSP INPUT
//#define		COMM_ETH_SENDCONTROLINDEX		0x3532			//	52 單筆資料握手傳輸

#define		COMM_ETHNEW_SENDREQUESTREAD			0xFF3132		//	12 請求讀
#define		COMM_ETHNEW_SENDUPDATEDATA		 	0xFF3133		//	13 請求寫
#define     COMM_ETHNEW_SENDPANEL              	0xFF3230		//  20 按鍵傳輸
#define     COMM_ETHNEW_SENDPANELBREAK         	0xFF32FF		//	   松開按鍵
#define		COMM_ETHNEW_SENDCONTROLMACHINE1		0xFF3330		//  30 上段參數資料
#define		COMM_ETHNEW_SENDCONTROLMACHINE2		0xFF3331		//	31 下段參數資料
#define		COMM_ETHNEW_SENDCONTROLMOLDSET1		0xFF3332		//	32 上段模組資料
#define		COMM_ETHNEW_SENDCONTROLMOLDSET2		0xFF3333		//	33 下段模組資料
#define		COMM_ETHNEW_SENDCONTROLMACHINEB		0xFF3334		//	34 B座參數資料
#define		COMM_ETHNEW_SENDCONTROLMOLDSETB1	0xFF3335		//	35 B座上段模組資料
#define		COMM_ETHNEW_SENDCONTROLMOLDSETB2	0xFF3336		//	36 B座下段模組資料

#define		COMM_ETHNEW_SENDCONTROLNEWMACHINE2		0xFF3337		//	37	MACHINE2
#define		COMM_ETHNEW_SENDCONTROLNEWMOLDSET2_1	0xFF3338		//	38	MOLDSET2_1
#define		COMM_ETHNEW_SENDCONTROLNEWMOLDSET2_2	0xFF3339		//	39	MOLDSET2_1
#define		COMM_ETHNEW_SENDCONTROLNEWMACHINE3		0xFF3430		//	40 	MACHINE3
#define		COMM_ETHNEW_SENDCONTROLNEWMOLDSET3_1	0xFF3431		//	41 	MOLDSET3_1
#define		COMM_ETHNEW_SENDCONTROLNEWMOLDSET3_2	0xFF3432		//	42 	MOLDSET3_2
#define		COMM_ETHNEW_SENDDIAGRMRI			0xFF3435		//0xFF3430	 modify new gate 20141008   //	40 讀取DSP ROM&RAM DSP INPUT
#define		COMM_ETHNEW_SENDDIAGWM				0xFF3436		//0xFF3431	 modify new gate 20141008 	//	41 寫入DSP RAM
#define		COMM_ETHNEW_SENDDIAGWO				0xFF3437		//0xFF3432	 modify new gate 20141008 	//	42 寫入DSP OUTPUT
#define		COMM_ETHNEW_SENDASSIGNINPUT			0xFF3438		//0xFF3433	 modify new gate 20141008 	//	43 對換 DSP INPUT
#define		COMM_ETHNEW_SENDASSIGNOUTPUT		0xFF3439		//0xFF3434   modify new gate 20141008    //	44 對換 DSP OUTPUT
#define		COMM_ETHNEW_SENDMONILIMIT			0xFF3530		//	50 監測值上下限
#define		COMM_ETHNEW_SENDMONBILIMIT			0xFF3531		//	51 B座監測值上下限
/*===========================================================================+
|           NewEthernet DSP54->HMI CMD define                                |
+===========================================================================*/ 
#define		COMM_ETH_GETCOLDSTART				0x4141			// 	AA 冷開機
#define		COMM_ETH_GETWARMSTART				0x4142			// 	AB 暖開機
#define		COMM_ETH_GETREQUESTREAD				0x4143			// 	AC 請求讀
#define		COMM_ETH_GETUPDATEDATA				0x4144			// 	AD 請求寫
#define		COMM_ETH_GETCONFIG              	0x4241			//	BA 組態資料
#define		COMM_ETH_GETCONFIGLIMIT         	0x4242			//	BB A/D&D/A上限值
#define		COMM_ETH_GETDAOUTPUT				0x4243			//	BC DA比例閥輸出曲線
#define		COMM_ETH_GETOPSTATE             	0x4341			//  CA 動作狀態
#define		COMM_ETH_GETOPSTATUS            	0x4342			//  CB DA&AD顯示值
#define		COMM_ETH_GETOPMETER            		0x4343			//  CC 射出曲線
#define		COMM_ETH_GETAUSTIONDRIVEINFO    	0x4344			//  CD AustionDriver	100H WORD
#define     COMM_ETH_GETMONICURRENT         	0x4541			//	EA 現模監測值
#define     COMM_ETH_GETMONILAST            	0x4542			//	EB 上模監測值
#define     COMM_ETH_GETMONIMAXMIN          	0x4543			//	EC 監測上下限
#define     COMM_ETH_GETMONICURRENTB        	0x4544			//	ED B座現模監測值
#define     COMM_ETH_GETMONILASTB 	       		0x4545			//	EE B座上模監測值
#define     COMM_ETH_GETMONIMAXMINB         	0x4546			//	EF B座監測上下限
#define		COMM_ETH_GETDIAGRMRI				0x4441			//  DA DSP ROM&RAM INPUT資料

#define		COMM_ETHNEW_GETCOLDSTART			0xFF4141		// 	AA 冷開機
#define		COMM_ETHNEW_GETWARMSTART			0xFF4142		// 	AB 暖開機
#define		COMM_ETHNEW_GETREQUESTREAD			0xFF4143		// 	AC 請求讀
#define		COMM_ETHNEW_GETUPDATEDATA			0xFF4144		// 	AD 請求寫
#define		COMM_ETHNEW_GETCONFIG              	0xFF4241		//	BA 組態資料
#define		COMM_ETHNEW_GETCONFIGLIMIT         	0xFF4242		//	BB A/D&D/A上限值
#define		COMM_ETHNEW_GETDAOUTPUT				0xFF4243		//	BC DA比例閥輸出曲線
#define		COMM_ETHNEW_GETOPSTATE             	0xFF4341		//  CA 動作狀態
#define		COMM_ETHNEW_GETOPSTATUS            	0xFF4342		//  CB DA&AD顯示值
#define		COMM_ETHNEW_GETOPMETER            	0xFF4343		//  CC 射出曲線
#define		COMM_ETHNEW_GETAUSTIONDRIVEINFO    	0xFF4344		//  CD AustionDriver
#define     COMM_ETHNEW_GETMONICURRENT         	0xFF4541		//	EA 現模監測值
#define     COMM_ETHNEW_GETMONILAST            	0xFF4542		//	EB 上模監測值
#define     COMM_ETHNEW_GETMONIMAXMIN          	0xFF4543		//	EC 監測上下限
#define     COMM_ETHNEW_GETMONICURRENTB        	0xFF4544		//	ED B座現模監測值
#define     COMM_ETHNEW_GETMONILASTB 	       	0xFF4545		//	EE B座上模監測值
#define     COMM_ETHNEW_GETMONIMAXMINB         	0xFF4546		//	EF B座監測上下限
#define		COMM_ETHNEW_GETDIAGRMRI				0xFF4441		//  DA DSP ROM&RAM INPUT資料
#endif
