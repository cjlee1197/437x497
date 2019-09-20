/* Copyright (C), 1997-2002, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Function : CmdDef                                                         |
|  Task     : Command Definition                                             |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 02/13/1997                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_CMDDEF
#define     D_CMDDEF

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define    _KEY_PANEL_AIR5             0xFF68          //			  // 吹氣
#define    _KEY_PANEL_AIR6             0xFF69 		   //			  // 吹氣
#define    _KEY_PANEL_COREEIN          0xFF6A          // _ALT_M      // 中子D 進
#define    _KEY_PANEL_COREEOUT         0xFF6B          // _ALT_N      // 中子D 退 
#define    _KEY_PANEL_COREFIN          0xFF6C          // _ALT_M      // 中子D 進
#define    _KEY_PANEL_COREFOUT         0xFF6D          // _ALT_N      // 中子D 退 
#define    _KEY_PANEL_NOZZLEFWD        0xFF6E          //             // 座台移進  0xFFA3
#define    _KEY_PANEL_NOZZLEBWD        0xFF6F          //             // 座台移出  0xFFA4
#define    _KEY_PANEL_EMPTY            0xFF70          // St031401 for SafeDoor    0xFFA2
#define    _KEY_PANEL_MOTOROFF         0xFF71          //             // 馬達關    //Anny2002/12/17
#define    _KEY_PANEL_HEATEROFF        0xFF72          //             // 電熱關    //Anny2002/12/17
#define    _KEY_PANEL_SHUTFWD          0xFF73          //             // 截流閥進  //Anny2002/11/5
#define    _KEY_PANEL_SHUTBWD          0xFF74          //             // 截流閥退  //Anny2002/11/5
#define    _KEY_PANEL_ROTARYOUT        0xFF75          //             //             0xFFA7
#define    _KEY_PANEL_SLIDERET         0xFF75                  // 滑台退
#define    _KEY_PANEL_CORECIN          0xFF76          // _ALT_4      // 中子C 進  0xFFA0
#define    _KEY_PANEL_CORECOUT         0xFF77          // _ALT_5      // 中子C 退  0xFFA1
#define    _KEY_PANEL_COREDIN          0xFF78          // _ALT_M      // 中子D 進  0xFFA8
#define    _KEY_PANEL_COREDOUT         0xFF79          // _ALT_N      // 中子D 退  0xFFA9
#define    _KEY_PANEL_SHUTOFF          0xFF7B          //             // 封閉噴嘴
#define    _KEY_PANEL_AIR3             0xFF7C          //             // 吹氣3
#define    _KEY_PANEL_AIR4             0xFF7D          //             // 吹氣4
#define    _KEY_PANEL_REARDOOROPEN     0xFF7E          //             // 後安全門開  0xFFA5
#define    _KEY_PANEL_REARDOORCLOSE    0xFF7F          //             // 後安全門關  0xFFA6
#define    _KEY_PANEL_MOLDCLOSE        0xFF80          // _ALT_A      // 關模    
#define    _KEY_PANEL_AUTOPURGE        0xFF81          // _ALT_B      // 自動清料
#define    _KEY_PANEL_MOLDOPEN         0xFF82          // _ALT_C      // 開模    
#define    _KEY_PANEL_INJECT           0xFF83          // _ALT_D      // 射出    
#define    _KEY_PANEL_SUCKBACK         0xFF84          // _ALT_E      // 射退    
#define    _KEY_PANEL_CHARGE           0xFF85          // _ALT_F      // 儲料    
#define    _KEY_PANEL_NOZZLEADV        0xFF86          // _ALT_G      // 座進    
#define    _KEY_PANEL_NOZZLERET        0xFF87          // _ALT_H      // 座退    
#define    _KEY_PANEL_EJECTADV         0xFF88          // _ALT_I      // 托模進  
#define    _KEY_PANEL_EJECTRET         0xFF89          // _ALT_J      // 托模退  
#define    _KEY_PANEL_COREAIN          0xFF8A          // _ALT_K      // 中子A 進
#define    _KEY_PANEL_COREAOUT         0xFF8B          // _ALT_L      // 中子A 退
#define    _KEY_PANEL_MOLDTHIN         0xFF8C          // _ALT_M      // 調模進  
#define    _KEY_PANEL_MOLDTHICK        0xFF8D          // _ALT_N      // 調模退  
#define    _KEY_PANEL_MOLDADJ          0xFF8E          // _ALT_O      // 調模    
#define    _KEY_PANEL_SEMIAUTO         0xFF8F          // _ALT_P      // 半自動  
#define    _KEY_PANEL_MANUAL           0xF090          //             // 手動
#define    _KEY_PANEL_RESET            0xF091          //             //
#define    _KEY_PANEL_KEYLOCK          0xF092          //             //
#define    _KEY_PANEL_TIMEAUTO         0xFF91          // _ALT_Q      // 全程自動
#define    _KEY_PANEL_SENSORAUTO       0xFF92          // _ALT_R      // 電眼自動
#define    _KEY_PANEL_DOOROPEN         0xFF93          // _ALT_S      // 安全門開
#define    _KEY_PANEL_DOORCLOSE        0xFF94          // _ALT_T      // 安全門關
#define    _KEY_PANEL_HEATER           0xFF95          // _ALT_U      // 電熱開關
#define    _KEY_PANEL_LUBRICATION      0xFF96          // _ALT_V      // 潤滑    
#define    _KEY_PANEL_AUTOADJ          0xFF97          // _ALT_P      // 自動調模  
#define    _KEY_PANEL_AIRBLASTMOV      0xFF98          // _ALT_W      // 公模吹氣
#define    _KEY_PANEL_AIRBLASTSTAT     0xFF99          // _ALT_X      // 母模吹氣
#define    _KEY_PANEL_MOTIONSTART      0xFF9A          // _ALT_Y      // 自動調模
#define    _KEY_PANEL_EMPTY2           0xFF9B          //FOR SPLACE KEY 2  //anny092601
#define    _KEY_PANEL_COREBIN          0xFF9C          // _ALT_Z      // 中子B 進
#define    _KEY_PANEL_COREBOUT         0xFF9D          // _ALT_1      // 中子B 退
#define    _KEY_PANEL_ROTARYIN         0xFF9E          //             // 
#define    _KEY_PANEL_MOTOR            0xFF9F          // _ALT_3      // 馬達開關

// #ifdef      D_DC     //Sunny<20080812>add DC Key<<<<<
#define    _KEY_PANEL_ROTATEMOLD       0xFFA1          // 轉模
#define    _KEY_PANEL_MIDEJTADV        0xFFA2          //中托進    //   0xFF7E
#define    _KEY_PANEL_MIDEJTRET        0xFFA3          //中托退    //   0xFF7F
#define    _KEY_PANEL_EJECTADVB        0XFFA4          // B托模進  //頂針B進      
#define    _KEY_PANEL_EJECTRETB        0XFFA5          // B托模退  //頂針B退      0XFF77
#define    _KEY_PANEL_NOZZLEADVB       0xFFA6          // B座進  0xFF78
#define    _KEY_PANEL_NOZZLERETB       0xFFA7          // B座退  0xFF79
#define    _KEY_PANEL_BOLTADV          0xFFA8          // 定位進   //Anny2002/9/4 ADD
#define    _KEY_PANEL_BOLTRET          0xFFA9          // 定位退   //Anny2002/9/4 ADD
#define    _KEY_PANEL_ROTATEADV        0xFFAA          // 轉盤正轉 ChunTzu2004/5/11 for HTNew
#define    _KEY_PANEL_ROTATERET        0xFFAB          // 轉盤反轉 ChunTzu2004/5/11 for HTNew  
#define    _KEY_PANEL_INJECTB          0xFFAC          // B射出
#define    _KEY_PANEL_SUCKBACKB        0xFFAD          // B射退
#define    _KEY_PANEL_CHARGEB          0xFFAE          // B儲料
#define    _KEY_PANEL_AUTOPURGEB       0xFFAF          // B清料  0xFF75
// #endif   //D_DC
//Sunny<20080812>add DC Key >>>>>

#define	   _KEY_PANEL_JOG				0xFFA1			//吋動
#define	   _KEY_PANEL_HOMING			0xFFA2			//歸零
#define	   _KEY_PANEL_ROBOT				0xFFA3			//機械手
#define	   _KEY_PANEL_START				0xFFA4			//啟動
#define	   _KEY_PANEL_STOP				0xFFA5          //停止
#define	   _KEY_PANEL_SPARE1			0xFF7B//0xFFA6          //spare key1 Fans TM版要求修改 2008/3/17 10:29上午
#define	   _KEY_PANEL_SPARE2			0xFF9B//0xFFA7          //spare key2 Fans TM版要求修改 2008/3/17 10:29上午
#define	   _KEY_PANEL_SPARE3			0xFFA8          //spare key3
#define	   _KEY_PANEL_SPARE4			0xFFA9          //spare key4
#define	   _KEY_PANEL_SPARE5			0xFFAA          //spare key5
#define	   _KEY_PANEL_SPARE6			0xFFAB          //spare key6
#define	   _KEY_PANEL_SPARE7			0xFFAC          //spare key7
#define	   _KEY_PANEL_SPARE8			0xFFAD          //spare key8
#define	   _KEY_PANEL_SPARE9			0xFFAE          //spare key9
#define	   _KEY_PANEL_SPARE10			0xFFAF          //spare key10
#define	   _KEY_PANEL_SPARE11			0xFFB0          //spare key11
#define	   _KEY_PANEL_SPARE12			0xFFB1          //spare key12
#define	   _KEY_PANEL_SPARE13			0xFFB2          //spare key13
#define	   _KEY_PANEL_SPARE14			0xFFB3          //spare key14
                                        
#define	   _KEY_PANEL_SLOWUSE			0xFFF0          //慢速使用key  //JOYCE2008-9-24 add for 7GL

#define	   _KEY_PANEL_BARRELHEATER			0xFFF1    //料管加熱key    //JOYCE2008-12-26 ADD for 注吹機
#define	   _KEY_PANEL_FLOWWAYHEATER			0xFFF2    //熱流道加熱key  //JOYCE2008-12-26 ADD for 注吹機
#define    _KEY_PANEL_SP40      			 0xFF63            // 保留 #4    //JOYCE2008-12-26 ADD for 注吹機
#define    _KEY_PANEL_FEEDIN           0xFF64            // 加料進     //JOYCE2008-12-26 ADD for 注吹機
#define    _KEY_PANEL_FEEDOUT   	     0xFF65            // 加料退     //JOYCE2008-12-26 ADD for 注吹機
#define    _KEY_PANEL_FEEDNOZZLEFWD    0xFF66            // 加料座進   //JOYCE2008-12-26 ADD for 注吹機
#define    _KEY_PANEL_FEEDNOZZLEBWD    0xFF67            // 加料座退   //JOYCE2008-12-26 ADD for 注吹機

#define		 _KEY_PANEL_7JMAUTO       			_KEY_PANEL_TIMEAUTO // 自動    		//JOYCE2009-5-8  FOR 7JM
#define		 _KEY_PANEL_7JMRESET       			_KEY_PANEL_SEMIAUTO // 複位  //JOYCE2009-5-8  FOR 7JM
#define		 _KEY_PANEL_LEFTCARRYAUTO       _KEY_PANEL_AIR3  		// 左架自動  LeftCarryAuto  		//JOYCE2009-3-6 FOR 7JM
#define		 _KEY_PANEL_RIGHTCARRYAUTO      _KEY_PANEL_AIR4  		// 右架自動  RightCarryAuto 		//JOYCE2009-3-6 FOR 7JM
#define		 _KEY_PANEL_CHARGEMOTOR         _KEY_PANEL_SUCKBACK // 擠料電機  ChargeMotorOn/Off  //JOYCE2009-3-6 FOR 7JM
#define		 _KEY_PANEL_THICKCTRLMOTOR      _KEY_PANEL_EMPTY    // 壁厚控製電機 //JOYCE2009-3-24 

#define     _KEY_PANEL_BREAK            0xFFFF
#define     _KEY_PANEL_                 0xFF


/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
enum CMDID  {
            ID_MOLD,
                ID_MOLD_READ,
                ID_MOLD_SAVE,
                ID_MOLD_SAVEAS,
                ID_MOLD_DELETE,
                ID_MOLD_PRINT,
                ID_MOLD_NET,
                ID_MOLD_INET,
                ID_MOLD_DISK,
                ID_MOLD_EXIT,
            ID_VIEW,
                ID_VIEW_OVERVIEW,
                ID_VIEW_CLAMP,
                ID_VIEW_CLAMP2,
                ID_VIEW_INJECT,
                ID_VIEW_VALVE,     //Steven 2004/3/23 add
                ID_VIEW_CHARGE,
                ID_VIEW_PURGE,
                ID_VIEW_HOLD,
                ID_VIEW_GASINJECT,
                ID_VIEW_INJECTPROFILE,
                ID_VIEW_INJECTSPECIAL,
                ID_VIEW_ROTATE,
                ID_VIEW_EJECT,
                ID_VIEW_INJECTPROFILEB,
                ID_VIEW_INJECTSPECIALB,
                ID_VIEW_INJECTFUN, 
                ID_VIEW_EJECTB,
                ID_VIEW_INJECTB,
                ID_VIEW_CHARGEB,
                ID_VIEW_EJECT2,
                ID_VIEW_BINJECT,             //ST081099 ADD FOR Vrt24 B Group
                ID_VIEW_BCHARGE,             //...
                ID_VIEW_BINJECTSPECIAL,      //...
                ID_VIEW_BINJECTPROFILE,      //...
                ID_VIEW_CORE,
                ID_VIEW_COREC,
                ID_VIEW_COREF,      //Anny2003/06/06 FOR 6 CORE
                ID_VIEW_CORESEQUENCE,
                ID_VIEW_CORE_E,
                ID_VIEW_OTHER,
                ID_VIEW_OTHER2,
                ID_VIEW_NOZZLE,
                ID_VIEW_TEMPERATURE,
                ID_VIEW_TEMPERATURE1,
                ID_VIEW_TEMPERATURE2,        //ST081099 ADD FOR Vrt24 B Group
                ID_VIEW_SEQUENCE,       //WY022499 ADD FOR FONT24 VERSION
                ID_VIEW_MOLDACT,
                ID_VIEW_MOLDSAVE,
                ID_VIEW_MOLDREAD,
                ID_VIEW_MOLDDEL,
                ID_VIEW_MACHINE,            // Bw032603 for save machine data
                ID_VIEW_EXHAUST,
                ID_VIEW_PROD,
                ID_VIEW_MONITOR1,
                ID_VIEW_MONITOR2,
                ID_VIEW_MONITOR3,
                ID_VIEW_MONITOR4,
                ID_VIEW_LIMIT,
                ID_VIEW_OUTPUTVALVE,
                ID_VIEW_LIMIT2,             //ChunTzu
                ID_VIEW_OUTPUTVALVE2,
                ID_VIEW_PARAMETER1,
                ID_VIEW_PARAMETER2,
                ID_VIEW_PARAMETER3,
                ID_VIEW_PARAMETERS,
                ID_VIEW_PARAFASTSET,        //BW
                ID_VIEW_PARAFASTSETB,       //Weichun
                ID_VIEW_SYSTEM,             //wy100999
                ID_VIEW_TIME8,              //st052803
                ID_VIEW_COMPRESS,           //WJ090103
                ID_VIEW_DATABASE0,
                ID_VIEW_DATABASE1,
                ID_VIEW_DATABASE2,
                ID_VIEW_DATABASE3,
                ID_VIEW_DATABASE4,
                ID_VIEW_DATABASE5,
                ID_VIEW_DATABASE6,
                ID_VIEW_DATABASE7,
                ID_VIEW_ADJUST,
                ID_VIEW_FUNCTION,
                ID_VIEW_CONFIG,
                ID_VIEW_CONFIG2,
                ID_VIEW_CONFIG3,
                ID_VIEW_CONFIG4,
                ID_VIEW_CODE,
                ID_VIEW_ALARM,
                ID_VIEW_MODIFY,     //Anny2002/12/5
                ID_VIEW_ZEROSET,
                ID_VIEW_VERSION,
                ID_VIEW_CONFIGURE,
                ID_VIEW_TIMEOUT,
                ID_VIEW_TIMEOUT_ALARM,
                ID_VIEW_PANEL,
                ID_VIEW_SHUTTLE,          // St042701
                ID_VIEW_INETFUNC,             //Bw 020614
                ID_VIEW_CLAMPPARA,
                ID_VIEW_CLAMPRAMPPARA,
                ID_VIEW_INJECTPARA,
                ID_VIEW_CHARGEPARA,
                ID_VIEW_EJECTPARA,
                ID_VIEW_COREPARA,
                ID_VIEW_OTHERPARA,
                ID_VIEW_INJECTPARAB,
                ID_VIEW_CHARGEPARAB,
                ID_VIEW_EJECTPARAB,
                ID_VIEW_OTHERPARAB,
                ID_VIEW_OTHERPARA1,
                ID_VIEW_OTHERPARA2,
                ID_VIEW_OTHERPARA3,
                ID_VIEW_SYSTEMPARA,
                ID_VIEW_TEMPPARA,
                ID_VIEW_MOLDTEMPPARA,
                ID_VIEW_ALARMPARA,
                ID_VIEW_CLAMPFUNC,
                ID_VIEW_INJECTFUNC,
                ID_VIEW_INJECTBFUNC,
                ID_VIEW_CHARGEFUNC,
                ID_VIEW_CHARGEBFUNC,
                ID_VIEW_EJECTFUNC,
                ID_VIEW_EJECTBFUNC,
                ID_VIEW_COREFUNC,
                ID_VIEW_OTHERFUNC,
                ID_VIEW_OTHERB,             //Weichun 2003/9/17
                ID_VIEW_OTHERBFUNC,         //Weichun 2003/9/17
                ID_VIEW_PRODFUNC,
                ID_VIEW_TEMPFUNC,           //Bw052702
                ID_VIEW_STRING,             //Weichun 2003/9/23
                ID_VIEW_TEMPCURVE,
                ID_VIEW_QUALITYCURVE,		//Jeff	2005/08/30
                ID_VIEW_ERRORCURVE,			//Jeff	2005/09/01
                ID_VIEW_MOLDCURVE,			//netdigger 2006/3/11 
                ID_VIEW_INJCURVE,
                ID_VIEW_CHGCURVE,			//netdigger 2006/3/12
                ID_VIEW_HOLDPRES,			//netdigger 2006/3/13
                ID_VIEW_POINTER,			//netdigger 2006/3/15

				ID_VIEW_CLAMPCONFIG,    //Anny2006/2/19
                ID_VIEW_INJECTCONFIG,   //Anny2006/2/19
                ID_VIEW_CHARGECONFIG,   //Anny2006/2/19
                ID_VIEW_EJECTCONFIG,    //Anny2006/2/19
                ID_VIEW_NOZZLECONFIG,   //Anny2006/2/19
            ID_SETUP,
                ID_SETUP_SYSTEM,
                ID_SETUP_DATETIME,
                ID_SETUP_PRODUCTION,
                ID_SETUP_USER,
                ID_SETUP_RESET,
                ID_SETUP_INSTALL,
                ID_SETUP_LOGOUT,                                     //Bw 020612
                ID_SETUP_LOGIN,                                      //Bw 020612
            ID_OTHER,
                ID_OTHER_ERRORLOG,
                ID_OTHER_MACHINEDIAG,
                ID_OTHER_MACHINEDIAG2,
                ID_OTHER_ASSIGNINPUT,
                ID_OTHER_ASSIGNOUTPUT,
                ID_OTHER_ABOUT,
                ID_OTHER_DIALOG,
            ID_HELP,
                ID_TOOL_DUPL,
                ID_TOOL_NEXT,
                ID_TOOL_SELECT,
                ID_TOOL_RETURN,
                ID_TOOL_RETURNHOME,
                ID_TOOL_LOGIN,
                ID_TOOL_MANUALLOGIN,                                //BW0105'02
                ID_TOOL_RESET,
                ID_TOOL_NULL,
                ID_VIEW_AUSTONE,
                ID_EXIT,
            ID_NULL = 0xFFFF,
            NULL_ID = 0xFFFF
            };

/*====================================================================+
|           Function and Class prototype                              |
+====================================================================*/

#endif
