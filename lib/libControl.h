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
|  Creation : 12/22/2008	                                                 |
|  Revision : 							                                     |
+===========================================================================*/
#ifndef	D_LIBCONTROL
#define	D_LIBCONTROL
#include	"../curve.h"
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
#define		MAX_POINT_NUM_NEW							1000
#define		MAX_CURVE_NUM_NEW							40//35
#define		MAX_CURVEEX_NUM_NEW						40//35
#define		MAX_POINT_NUM_2000							2000
#define		MAX_POINT_NUM_1000							1000
#define		MAX_POINT_NUM_200							200

#define		CURVE_OIL_DATA_INJECTB_REALPRESS					30		//	inject pressure
#define		CURVE_OIL_DATA_INJECTB_REALSPEED					31		//	inject speed
#define		CURVE_OIL_DATA_INJECTB_REALPOS						32		//	inject postion
#define		CURVE_OIL_DATA_HOLDB_REALPRESS						33		//	hold press
#define		CURVE_OIL_DATA_HOLDB_REALSPEED						34		//	hold speed
#define		CURVE_OIL_DATA_CHARRGB_REALPRESS					35		//	charge pressure
#define		CURVE_OIL_DATA_LASTTIME_INJECTB_REALPRESS	36		//	temporarily inject pressure
#define		CURVE_OIL_DATA_LASTTIME_INJECTB_REALSPEED	37    //	temporarily inject speed   
#define		CURVE_OIL_DATA_LASTTIME_INJECTB_REALPOS		38    //	temporarily inject postion 
#define		CURVE_OIL_DATA_LASTTIME_HOLDB_REALPRESS		39    //	temporarily hold press     
#define		CURVE_OIL_DATA_LASTTIME_HOLDB_REALSPEED		40    //	temporarily hold speed     
#define		CURVE_OIL_DATA_LASTTIME_CHARRGB_REALPRESS	41    //	charge pressure
#define		CURVE_OIL_DATA_INJECTB_SETPRESS						42		//	inject pressure setting
#define		CURVE_OIL_DATA_INJECTB_SETSPEED						43		//	inject	speed settting
#define		CURVE_OIL_DATA_INJECTB_SETPOS							44		//	inject pos seeting
#define		CURVE_OIL_DATA_HOLDB_SETPRESS							45		//	hold press	setting
#define		CURVE_OIL_DATA_HOLDB_SETSPEED							46    //  hold speed  setting
#define		CURVE_OIL_DATA_CHARGEB_SETPRESS						47		//	charge press setting
#define		CURVE_OIL_DATA_CHARGEB_SETBACKPRES				48		//  charge backpres setting //Sunny<20070815>
#define		CURVE_OIL_DATA_CHARGEB_REALBACKPRES				49    //  charge backpres real
#define		CURVE_OIL_DATA_CHARGEB_SETRPM							50    //  charge RPM setting
#define		CURVE_OIL_DATA_CHARGEB_REALRPM						51    //  charge RPM real
#define		CURVE_OIL_DATA_INJECTB_DXPOS							52		//	inject pos  x=pos,y=timer
#define		CURVE_OIL_DATA_HOLDB_DXPOS								53
//const	int CURVE_OIL_DATA_LASTTIME_CHARGE_REALRPM		= 22;       //  temporarily charge RPM
//const	int CURVE_OIL_DATA_LASTTIME_CHARGE_REALBACKPRES	= 23;       //  temporarily charge backpres

//temporarily data id
#define		CURVE_OIL_DATA_INJECTB_TEMPORARILY_REALPRESS	3030		//	inject pressure temporarily data
#define		CURVE_OIL_DATA_INJECTB_TEMPORARILY_REALSPEED	3031		//	inject speed temporarily data
#define		CURVE_OIL_DATA_INJECTB_TEMPORARILY_REALPOS		3032		//	inject postion temporarily data
#define		CURVE_OIL_DATA_HOLDB_TEMPORARILY_REALPRESS		3033		//	hold press temporarily data
#define		CURVE_OIL_DATA_HOLDB_TEMPORARILY_REALSPEED		3034		//	hold speed temporarily data
#define		CURVE_OIL_DATA_CHARRGB_TEMPRARILY_REALPRESS		3035		//	charge pressure temporarily data
#define		CURVE_OIL_DATA_INJECTB_TEMPORARILY_DXPOS				3052		//inject  pos temporarily data 

//Ammeter Parameter
#define		AMMETER_ENERGY			0x0001		// 當前電能
#define		AMMETER_VOLTAGE_A		0x0002		// A相電壓
#define		AMMETER_VOLTAGE_B		0x0003		// B相電壓
#define		AMMETER_VOLTAGE_C		0x0004		// C相電壓
#define		AMMETER_VOLTAGE			0x0005		// 電壓
#define		AMMETER_CURRENT_A		0x0006		// A相電流
#define		AMMETER_CURRENT_B		0x0007		// B相電流
#define		AMMETER_CURRENT_C		0x0008		// C相電流
#define		AMMETER_CURRENT			0x0009		// 電流
#define		AMMETER_POWER_A			0x000C		// A相功率
#define		AMMETER_POWER_B			0x000D		// B相功率
#define		AMMETER_POWER_C			0x000E		// C相功率

#define		AMMETER_TOTAL_POWER		0x0010		// 總功率
#define		AMMETER_VOLTAGE_AB		0x0011		// AB相電壓
#define		AMMETER_VOLTAGE_BC		0x0012		// BC相電壓
#define		AMMETER_VOLTAGE_AC		0x0013		// AC相電壓
#define		AMMETER_READ_CT			0x0014		// 讀CT值
#define		AMMETER_WRITE_CT		0x0015		// 寫CT值
#define		AMMETER_DATAVER			0x0016		//Ver
#define		AMMETER_DEVIATION		0x0017		//相位偏差   
#define		AMMETER_POWERFACTOR		0x0018		//功率因素 
#define		AMMETER_WRITE_ENERGYRESET		0x0019		//電表電能複位   /*JOYCE2014-1-9 add 海天COM2松下電表*/

#define		AMMETER_ADDRESS			0x00FE		// 請求電錶地址
#define		AMMETER_WRITE_TEST		0x00FF		// test


#define     AMMETER_RS485ID_POSI_WATTHR             0x0001      // Positive active power of three phase
#define     AMMETER_RS485ID_NEGA_WATTHR             0x0002      // Negative active power of three phase
#define     AMMETER_RS485ID_POSI_VAR                0x0003      // Positive reactive power of three phase
#define     AMMETER_RS485ID_NEGA_VAR                0x0004      // Negative reactive power of three phase
#define     AMMETER_RS485ID_AVRMS                   0x0005      // Voltage rms value of phase A
#define     AMMETER_RS485ID_BVRMS                   0x0006      // Voltage rms value of phase B
#define     AMMETER_RS485ID_CVRMS                   0x0007      // Voltage rms value of phase C
#define     AMMETER_RS485ID_AIRMS                   0x0008      // Current rms value of phase A
#define     AMMETER_RS485ID_BIRMS                   0x0009      // Current rms value of phase B
#define     AMMETER_RS485ID_CIRMS                   0x000A      // Current rms value of phase C
#define     AMMETER_RS485ID_WATT                    0x000B      // Active power of three phase //總功率
#define     AMMETER_RS485ID_AWATT                   0x000C      // Active power of phase A 
#define     AMMETER_RS485ID_BWATT                   0x000D      // Active power of phase B 
#define     AMMETER_RS485ID_CWATT                   0x000E      // Active power of phase C 
#define     AMMETER_RS485ID_VAR                     0x000F      // Reactive power of three phase
#define     AMMETER_RS485ID_AVAR                    0x0010      // Reactive power of phase A
#define     AMMETER_RS485ID_BVAR                    0x0011      // Reactive power of phase B
#define     AMMETER_RS485ID_CVAR                    0x0012      // Reactive power of phase C
#define     AMMETER_RS485ID_PF                      0x0013      // Power factor of three phase
#define     AMMETER_RS485ID_APF                     0x0014      // Power factor of phase A
#define     AMMETER_RS485ID_BPF                     0x0015      // Power factor of phase B
#define     AMMETER_RS485ID_CPF                     0x0016      // Power factor of phase C
#define     AMMETER_RS485ID_ABVRMS					0x0017		//Voltage A-B
#define     AMMETER_RS485ID_BCVRMS					0x0018		//Voltage B-C
#define     AMMETER_RS485ID_ACVRMS					0x0019		//Voltage A-C

#define     AMMETER_RS485ID_ERROR                  	0x001A      //Error				Caozz add 2012/3/27 上午 09:19:10
#define     AMMETER_RS485ID_VAR_RESET              	0x001B      //Reset Var to 0                    
#define     AMMETER_RS485ID_IRMS_RESET             	0x001C      //Set IRMS offset

#define		AMMETER_RS485ID_COUNT					8			//RW.Ying  for Ammeter count

#define     AMMETER_RS485ID_TEST					0x00FF		//TEST 

#define	AMMETER_CONTROL_ACTUALVALUE		0x03		/* control code ammeter ActualValue */
#define	AMMETER_FRAME_ADDR_LENGTH   	6			/* address len 6 */
#define	AMMETER_MAX_NUM_COMMON  				8			/* ammeter max num 8 */

/*===========================================================================+
|           Constant  ---QualityControl                                      |
+===========================================================================*/
#define     MONI_OIL_NOTHING_ID                                 -1              //  
#define     MONI_OIL_SHOTCOUNT_ID                               0                   //  開模序號                (OIL)
#define     MONI_OIL_CYCLE_ID                                   1                   //  循環計時值           (OIL)
#define     MONI_OIL_INJECT_ID                                  2                   //  射出計時值           (OIL)
#define     MONI_OIL_TURNTOHOLD_ID                          3                   //  保壓轉換時間值 (OIL)
#define     MONI_OIL_CHARGE_ID                                  4                   //  儲料計時值           (OIL)
#define     MONI_OIL_PRES_TURNTOHOLD_ID                 5                   //  保壓轉換壓力值     (OIL)
#define     MONI_OIL_POSI_INJECTSTART_ID                6                   //  射出起點                (OIL)
#define     MONI_OIL_POSI_TURNTOHOLD_ID                 7                   //  保壓起點                (OIL)
#define     MONI_OIL_POSI_INJECTEND_ID                  8                   //  射出終點        (OIL)
#define     MONI_OIL_POSI_INJECTCUSHION_ID          9                   //  射出監控位置    (OIL)
#define     MONI_OIL_CHARGERPM_ID                               10              //  儲料RPM 值      (OIL)
#define     MONI_OIL_SUCKBACK_ID                                11              //  射退計時值      (OIL)
#define     MONI_OIL_EJECT_ID                                       12              //  脫模計時值      (OIL)
#define     MONI_OIL_EJECTADVANCE_ID                        13              //  脫進計時值      (OIL)
#define     MONI_OIL_EJECTRETURN_ID                         14              //  脫退計時值      (OIL)
#define     MONI_OIL_PRES_INJECTPEAK_ID                 15              //  射出尖峰壓力    (OIL)
#define     MONI_OIL_PRES_CHARGEPEAK_ID                 16              //  儲料尖峰壓力    (OIL)
#define     MONI_OIL_INJECTB_ID                                 17              //  B組射出計時值   (OIL)
#define     MONI_OIL_CHARGEB_ID                                 18              //  B組儲料計時值   (OIL)
#define     MONI_OIL_POSI_INJECTSTARTB_ID               19              //  B組射出起點     (OIL)
#define     MONI_OIL_POSI_TURNTOHOLDB_ID                20              //  B組保壓起點     (OIL)
#define     MONI_OIL_POSI_INJECTCUSHIONB_ID         21              //  B組射出終點     (OIL)
#define     MONI_OIL_PRES_INJECTERB_ID                  22              //  射出壓力值      (OIL)
#define     MONI_OIL_PRES_CHARGEBACKB_ID                23              //  儲料背壓值      (OIL)
#define     MONI_OIL_TIME_TAKE_ID                               24              //  取件時間            (OIL)//Sunny<20070809>
#define     MONI_OIL_POSI_OPENMOLDEND_ID                25              //  開模終點            (OIL)//Sunny<20080804>
#define     MONI_OIL_TURNTOHOLDB_ID                         26              //  B組保壓轉換時間實際值
#define     MONI_OIL_PRES_TURNTOHOLDB_ID                27              //  B組保壓轉換壓力實際值
#define     MONI_OIL_POSI_INJECTENDB_ID                 28              //  B組射出終點
#define     MONI_OIL_PRES_INJECTPEAKB_ID                29              //  B組射出尖峰壓力 (OIL)
#define     MONI_OIL_PRES_CHARGEPEAKB_ID                30              //  B組儲料尖峰壓力 (OIL)
#define     MONI_OIL_INJMAXSPEED_ID                         31              //  最大射出速率      (OIL)//Sunny<20080804>
#define     MONI_OIL_INJMAXSPEEDB_ID                        32              //  B組最大射出速率 (OIL)//Sunny<20080804>
//#define       MONI_OIL_OPENTIME_ID                            33              //  開模計時 (OIL)//ZHBA 2010-5-10
//#define       MONI_OIL_CLOSETIME_ID                           34              //  關模計時 (OIL)//ZHBA 2010-5-10
//#define       MONI_OIL_CLOSEPROTECTTIME_ID              35                //  關模低壓計時 (OIL)//ZHBA 2010-5-10
//#define       MONI_OIL_CLOSECLAMPUPTIME_ID                36              //  關模高壓計時 (OIL)//ZHBA 2010-5-10
//#define       MONI_OIL_RESERVED_ID                                37              //  wReservedMG[3]      //JOYCE2011-8-17 FOR 1HMI2DSP FLAG
#define     MONI_OIL_MUCELL_AVERAGEPERCENT_ID           33              //  average percent (MUCELL) //JOYCE2010-12-16 <<<
#define     MONI_OIL_MUCELL_MAXPERCENT_ID               34              //  max percent (MUCELL) 
#define     MONI_OIL_MUCELL_MINPERCENT_ID               35              //  min percent (MUCELL) 
#define     MONI_OIL_MUCELL_SETNPERCENT_ID              36              //  set percent (MUCELL) 

#define     MONI_OIL_RESERVED0_ID                           37              //  wReservedMG[0] //ZHBA 2010-5-10 
#define     MONI_OIL_RESERVED1_ID                           38              //  wReservedMG[1] //ZHBA 2010-5-10 
#define     MONI_OIL_RESERVED2_ID                           39              //  wReservedMG[2] //ZHBA 2010-5-10 
#define     MONI_OIL_RESERVED3_ID                           40              //  wReservedMG[3] //ZHBA 2010-5-10 
#define     MONI_OIL_RESERVED4_ID                           41              //  wReservedMG[4]      //JOYCE2011-8-17 FOR 1HMI2DSP FLAG
#define     MONI_OIL_RESERVED5_ID                           42              //  wReservedMG[5]
#define     MONI_OIL_RESERVED6_ID                           43              //  wReservedMG[6]
#define     MONI_OIL_RESERVED7_ID                           44              //  wReservedMG[7]
#define     MONI_OIL_RESERVED8_ID                           45              //  wReservedMG[8]
#define     MONI_OIL_RESERVED9_ID                           46              //  wReservedMG[9]  //  開模計時
#define     MONI_OIL_RESERVED10_ID                          47              //  wReservedMG[10] //  關模計時
#define     MONI_OIL_RESERVED11_ID                          48              //  wReservedMG[11]
#define     MONI_OIL_RESERVED12_ID                          49              //  wReservedMG[12]
#define     MONI_OIL_RESERVED13_ID                          50              //  wReservedMG[13]
#define     MONI_OIL_RESERVED14_ID                          51              //  wReservedMG[14]
#define     MONI_OIL_RESERVED15_ID                          52              //  wReservedMG[15]
#define     MONI_OIL_RESERVED16_ID                          53              //  wReservedMG[16]
#define     MONI_OIL_RESERVED17_ID                          54              //  wReservedMG[17]
#define     MONI_OIL_RESERVED18_ID                          55              //  wReservedMG[18]
#define     MONI_OIL_RESERVED19_ID                          56              //  wReservedMG[19]
#define     MONI_OIL_RESERVED20_ID                          57              //  wReservedMG[20]
#define     MONI_OIL_RESERVED21_ID                          58              //  wReservedMG[21]
#define     MONI_OIL_RESERVED22_ID                          59              //  wReservedMG[22]
#define     MONI_OIL_RESERVED23_ID                          60              //  wReservedMG[23]
#define     MONI_OIL_RESERVED24_ID                          61              //  wReservedMG[24]
#define     MONI_OIL_RESERVED25_ID                          62              //  wReservedMG[25]
#define     MONI_OIL_RESERVED26_ID                          63              //  wReservedMG[26]
#define     MONI_OIL_RESERVED27_ID                          64              //  wReservedMG[27]
#define     MONI_OIL_RESERVED28_ID                          65              //  wReservedMG[28]
#define     MONI_OIL_RESERVED29_ID                          66              //  wReservedMG[29]

#define     MONI_OIL_DWRESERVED0_ID                         67              //  dwReservedMG[0]
#define     MONI_OIL_DWRESERVED1_ID                         68              //  dwReservedMG[1]
#define     MONI_OIL_DWRESERVED2_ID                         69              //  dwReservedMG[2]
#define     MONI_OIL_DWRESERVED3_ID                         70              //  dwReservedMG[3]
#define     MONI_OIL_DWRESERVED4_ID                         71              //  dwReservedMG[4]
#define     MONI_OIL_DWRESERVED5_ID                         72              //  dwReservedMG[5]
#define     MONI_OIL_DWRESERVED6_ID                         73              //  dwReservedMG[6]
#define     MONI_OIL_DWRESERVED7_ID                         74              //  dwReservedMG[7]
#define     MONI_OIL_DWRESERVED8_ID                         75              //  dwReservedMG[8]
#define     MONI_OIL_DWRESERVED9_ID                         76              //  dwReservedMG[9]
#define     MONI_OIL_DWRESERVED10_ID                        77              //  dwReservedMG[10]
#define     MONI_OIL_DWRESERVED11_ID                        78              //  dwReservedMG[11]
#define     MONI_OIL_DWRESERVED12_ID                        79              //  dwReservedMG[12]
#define     MONI_OIL_DWRESERVED13_ID                        80              //  dwReservedMG[13]
#define     MONI_OIL_DWRESERVED14_ID                        81              //  dwReservedMG[14]
#define     MONI_OIL_DWRESERVED15_ID                        82              //  dwReservedMG[15]
#define     MONI_OIL_DWRESERVED16_ID                        83              //  dwReservedMG[16]
#define     MONI_OIL_DWRESERVED17_ID                        84              //  dwReservedMG[17]
#define     MONI_OIL_DWRESERVED18_ID                        85              //  dwReservedMG[18]
#define     MONI_OIL_DWRESERVED19_ID                        86              //  dwReservedMG[19]

/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
typedef	CURVEPOINT	tmCurveOilPoint;
typedef	struct	tagtmOilCurveFileData
{
	int	nID;			//	id for curveline
	int	nCount;			//	count for line
	tmCurveOilPoint*	Points/*[MAX_POINT_NUM]*/;
}	tmOilCurveFileData;

typedef struct tagtmOIL_MONI2
{
        DWORD   dwShotCount;
        _TIMEL  tmlCycle;                           // 1 循環計時實際值    
        _TIMEL  tmlInject;                          // 2 射出計時實際值 
        _TIMEL  tmlTurnToHold;                      // 3 保壓轉換時間實際值
        _TIMEL  tmlCharge;                          // 4 儲料計時實際值 
        LONG     adPosi_InjectStart;                // 5 射出起點
        LONG     adPosi_TurnToHold;                 // 6 保壓起點
        LONG     adPosi_InjectEnd;                  // 7 射出終點
        LONG     adPosi_InjectCushion;              // 8 射出監控位置
        LONG    adPosi_InjectPeak;               	  // 9  射出尖峰位置
        _DA     daPres_TurnToHold;                  // 10保壓轉換壓力實際值
        _COUNT  cnChargeRPM;                        // 11 儲料RPM 實際值
        _TIMER  tmSuckBack;                         // 12 射退計時實際值
        _TIMER  tmEject;                            // 13 脫模計時實際值
        _TIMER  tmEjectAdvance;                     // 14 脫進計時實際值
        _TIMER  tmEjectReturn;                      // 15 脫退計時實際值
        _DA     daPres_InjectPeak;                  // 16 射出尖峰壓力
        _DA     daPres_ChargePeak;                  // 17 儲料尖峰壓力 
        _TIMEL  tmlInjectB;                         // 18 B組射出計時實際值
        _TIMEL  tmlChargeB;                         // 19 B組儲料計時實際值
        LONG    adPosi_InjectStartB;                // 20 B組射出起點
        LONG    adPosi_TurnToHoldB;                 // 21 B組保壓起點
        LONG    adPosi_InjectCushionB;              // 22 B組射出終點
        _DA     daPres_InjecterB;              			// 23 B組射出壓力實際值   //gwei 2003/7/23
        _DA     daPres_ChargeBackB;            			// 24 B組儲料背壓實際值   //gwei 2003/7/23
        _TIMEL  tmTake;                 						// 25	   取件時間		  	//Sunny<20070809>
        LONG    adPosi_OpenMoldEnd;                 // 26    開模終點			//Sunny<20080804>
        _DA     daPres_TurnToHoldB;              		// 27 B組保壓轉換壓力實際值
        _TIMEL  tmlTurnToHoldB;                  		// 28 B組保壓轉換時間實際值
        LONG    adPosi_InjectHoldEndB;           		// 29 B組射出終點位置實際值
        _DA     daPres_InjectMiddleB;            		// 30 B組射出尖峰壓力實際值   
        _DA     daPres_ChargeMiddleB;            		// 31 B組儲料尖峰壓力實際值          
        WORD    InjectMaxSpeed;		                  // 32 最大射速			//Sunny<20080804>
        WORD    InjectMaxSpeedB;	                  // 33 B組最大射速			//Sunny<20080804>
        WORD		MuCellAveragePercent;               // 34 average percent (MUCELL) //JOYCE2010-12-16 <<<
        WORD		MuCellMaxPercent;                   // 35 max percent (MUCELL)
        WORD		MuCellMinPercent;                   // 36 min percent (MUCELL)
        WORD		MuCellSetPercent;                   // 37 set percent (MUCELL)
//        WORD		MuCellShotWeight;                   // 37 shot weight (MUCELL)
//        WORD		MuCellBackPress;                    // 38 back press (MUCELL)
        WORD    wReservedMG[30];                     // 37 保留00F-017
        DWORD   dwReserved[20];
        #ifdef	D_X86
        WORD	wR1;
        WORD	wR2;
        WORD	wR3;
        #endif
} tmOIL_MONI2;
    

typedef union tagtmDB_MONI2
{
	struct
	{
		int	nCount;
		int nIndex;
	};
	struct
	{
		union
		{
			tmOIL_MONI2		OilMoni2;
			I32_MONI2		I32Moni;
		};
		WORD	wSource;
		#ifdef	D_X86
		WORD	wR1;
		#endif
	};
} tmDB_MONI2;

typedef struct tagtmOIL_MONI2EX
    {
        long  dwShotCount;
        long  tmlCycle;                           // 1 循環計時實際值    
        long  tmlInject;                          // 2 射出計時實際值 
        long  tmlTurnToHold;                      // 3 保壓轉換時間實際值
        long  tmlCharge;                          // 4 儲料計時實際值 
        long  adPosi_InjectStart;                 // 5 射出起點
        long  adPosi_TurnToHold;                  // 6 保壓起點
        long  adPosi_InjectEnd;                   // 7 射出終點
        long  adPosi_InjectCushion;               // 8 射出監控位置
        long  adPosi_InjectPeak;               	  // 9  射出尖峰位置
        long  daPres_TurnToHold;                  // 10保壓轉換壓力實際值
        long  cnChargeRPM;                        // 11 儲料RPM 實際值
        long  tmSuckBack;                         // 12 射退計時實際值
        long  tmEject;                            // 13 脫模計時實際值
        long  tmEjectAdvance;                     // 14 脫進計時實際值
        long  tmEjectReturn;                      // 15 脫退計時實際值
        long  daPres_InjectPeak;                  // 16 射出尖峰壓力
        long  daPres_ChargePeak;                  // 17 儲料尖峰壓力 
        long  tmlInjectB;                         // 18 B組射出計時實際值
        long  tmlChargeB;                         // 19 B組儲料計時實際值
        long  adPosi_InjectStartB;                // 20 B組射出起點
        long  adPosi_TurnToHoldB;                 // 21 B組保壓起點
        long  adPosi_InjectCushionB;              // 22 B組射出終點
        long  daPres_InjecterB;                   // 23 B組射出壓力實際值   //gwei 2003/7/23
        long  daPres_ChargeBackB;                 // 24 B組儲料背壓實際值   //gwei 2003/7/23
        long  tmTake;                 			  		// 25 取件時間		  //Sunny<20070809>
        long  adPosi_OpenMoldEnd;                 // 26 開模終點		  //Sunny<20080804>
        long  daPres_TurnToHoldB;              		// 27 B組保壓轉換壓力實際值
        long  tmlTurnToHoldB;                  		// 28 B組保壓轉換時間實際值
        long  adPosi_InjectHoldEndB;           		// 29 B組射出終點位置實際值
        long  daPres_InjectMiddleB;            		// 30 B組射出尖峰壓力實際值   
        long  daPres_ChargeMiddleB;            		// 31 B組儲料尖峰壓力實際值          
        long  InjectMaxSpeed;		                  // 32 最大射速			//Sunny<20080804>
        long  InjectMaxSpeedB;	                  // 33 B組最大射速			//Sunny<20080804>        
        long	MuCellAveragePercent;               // 34 average percent (MUCELL) //JOYCE2010-12-16 <<<
        long  MuCellMaxPercent;                   // 35 max percent (MUCELL)
        long  MuCellMinPercent;                   // 36 min percent (MUCELL)
        long  MuCellSetPercent;                   // 37 set percent (MUCELL)
//        long  MuCellShotWeight;                   // 37 shot weight (MUCELL)
//        long	MuCellBackPress;                    // 38 back press (MUCELL)
        long  wReservedMG[15];                     // 00F 保留00F-017
        
        //	fans	2006/12/13 08:29下午
        tagtmOIL_MONI2EX operator+(tagtmOIL_MONI2 moni1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= moni1.tmlCycle 							+ this->tmlCycle;
        	Result.tmlInject 						= moni1.tmlInject 						+ this->tmlInject;
        	Result.tmlTurnToHold				= moni1.tmlTurnToHold 				+ this->tmlTurnToHold;
        	Result.tmlCharge						= moni1.tmlCharge 						+ this->tmlCharge;
        	Result.adPosi_InjectStart		= moni1.adPosi_InjectStart 		+ this->adPosi_InjectStart;
        	Result.adPosi_TurnToHold		= moni1.adPosi_TurnToHold 		+ this->adPosi_TurnToHold;
        	Result.adPosi_InjectEnd			= moni1.adPosi_InjectEnd 			+ this->adPosi_InjectEnd;
        	Result.adPosi_InjectCushion	= moni1.adPosi_InjectCushion	+ this->adPosi_InjectCushion;
        	Result.adPosi_InjectPeak  	= moni1.adPosi_InjectPeak   	+ this->adPosi_InjectPeak;
        	Result.daPres_TurnToHold		= moni1.daPres_TurnToHold 		+ this->daPres_TurnToHold;
        	Result.cnChargeRPM					= moni1.cnChargeRPM 					+ this->cnChargeRPM;
        	Result.tmSuckBack						= moni1.tmSuckBack 						+ this->tmSuckBack;
        	Result.tmEject							= moni1.tmEject 							+ this->tmEject;
        	Result.tmEjectAdvance				= moni1.tmEjectAdvance 				+ this->tmEjectAdvance;
        	Result.tmEjectReturn				= moni1.tmEjectReturn 				+ this->tmEjectReturn;
        	Result.daPres_InjectPeak		= moni1.daPres_InjectPeak 		+ this->daPres_InjectPeak;
        	Result.daPres_ChargePeak		= moni1.daPres_ChargePeak 		+ this->daPres_ChargePeak;
        	Result.tmlInjectB						= moni1.tmlInjectB 						+ this->tmlInjectB;
        	Result.tmlChargeB						= moni1.tmlChargeB 						+ this->tmlChargeB;
        	Result.adPosi_InjectStartB	= moni1.adPosi_InjectStartB 	+ this->adPosi_InjectStartB;
        	Result.adPosi_TurnToHoldB		= moni1.adPosi_TurnToHoldB 		+ this->adPosi_TurnToHoldB;
        	Result.adPosi_InjectCushionB= moni1.adPosi_InjectCushionB + this->adPosi_InjectCushionB;
        	Result.daPres_InjecterB			= moni1.daPres_InjecterB 			+ this->daPres_InjecterB;
        	Result.daPres_ChargeBackB		= moni1.daPres_ChargeBackB 		+ this->daPres_ChargeBackB;
        	Result.tmTake								= moni1.tmTake			 					+ this->tmTake;
        	Result.adPosi_OpenMoldEnd		= moni1.adPosi_OpenMoldEnd 		+ this->adPosi_OpenMoldEnd;
        	Result.daPres_TurnToHoldB		= moni1.daPres_TurnToHoldB 		+ this->daPres_TurnToHoldB;
        	Result.tmlTurnToHoldB				= moni1.tmlTurnToHoldB 				+ this->tmlTurnToHoldB;
        	Result.adPosi_InjectHoldEndB= moni1.adPosi_InjectHoldEndB	+ this->adPosi_InjectHoldEndB;
        	Result.daPres_InjectMiddleB	= moni1.daPres_InjectMiddleB 	+ this->daPres_InjectMiddleB;
        	Result.daPres_ChargeMiddleB	= moni1.daPres_ChargeMiddleB 	+ this->daPres_ChargeMiddleB;
        	Result.InjectMaxSpeed				= moni1.InjectMaxSpeed 				+ this->InjectMaxSpeed;
        	Result.InjectMaxSpeedB			= moni1.InjectMaxSpeedB 			+ this->InjectMaxSpeedB;
        	Result.MuCellAveragePercent	= moni1.MuCellAveragePercent	+ this->MuCellAveragePercent;
        	Result.MuCellMaxPercent   	= moni1.MuCellMaxPercent    	+ this->MuCellMaxPercent;
        	Result.MuCellMinPercent   	= moni1.MuCellMinPercent    	+ this->MuCellMinPercent;
        	Result.MuCellSetPercent   	= moni1.MuCellSetPercent    	+ this->MuCellSetPercent;
//        	Result.MuCellShotWeight   	= moni1.MuCellShotWeight    	+ this->MuCellShotWeight;
//        	Result.MuCellBackPress     	= moni1.MuCellBackPress     	+ this->MuCellBackPress;
        	Result.wReservedMG[0]			  = moni1.wReservedMG[0] 			  + this->wReservedMG[0]; 		//ZHBA 2010-5-10 
        	Result.wReservedMG[1]			  = moni1.wReservedMG[1] 			  + this->wReservedMG[1]; 		//ZHBA 2010-5-10 
        	Result.wReservedMG[2]			  = moni1.wReservedMG[2] 			  + this->wReservedMG[2]; 		//ZHBA 2010-5-10 
        	Result.wReservedMG[3]			  = moni1.wReservedMG[3] 			  + this->wReservedMG[3]; 		//ZHBA 2010-5-10 
        	Result.wReservedMG[4]			  = moni1.wReservedMG[4] 			  + this->wReservedMG[4]; 
        	Result.wReservedMG[5]			  = moni1.wReservedMG[5] 			  + this->wReservedMG[5]; 
        	Result.wReservedMG[6]			  = moni1.wReservedMG[6] 			  + this->wReservedMG[6]; 
        	Result.wReservedMG[7]			  = moni1.wReservedMG[7] 			  + this->wReservedMG[7];
        	Result.wReservedMG[8]			  = moni1.wReservedMG[8] 			  + this->wReservedMG[8]; 
        	Result.wReservedMG[9]			  = moni1.wReservedMG[9] 			  + this->wReservedMG[9]; 
        	Result.wReservedMG[10]			  = moni1.wReservedMG[10]  			  + this->wReservedMG[10]; 
        	Result.wReservedMG[11]			  = moni1.wReservedMG[11]  			  + this->wReservedMG[11];
        	Result.wReservedMG[12]			  = moni1.wReservedMG[12]  			  + this->wReservedMG[12]; 
        	Result.wReservedMG[13]			  = moni1.wReservedMG[13]  			  + this->wReservedMG[13]; 
        	Result.wReservedMG[14]			  = moni1.wReservedMG[14]  			  + this->wReservedMG[14]; 
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator+(tagtmOIL_MONI2EX moni1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= moni1.tmlCycle 							+ this->tmlCycle;
        	Result.tmlInject 						= moni1.tmlInject 						+ this->tmlInject;
        	Result.tmlTurnToHold				= moni1.tmlTurnToHold 				+ this->tmlTurnToHold;
        	Result.tmlCharge						= moni1.tmlCharge 						+ this->tmlCharge;
        	Result.adPosi_InjectStart		= moni1.adPosi_InjectStart 		+ this->adPosi_InjectStart;
        	Result.adPosi_TurnToHold		= moni1.adPosi_TurnToHold 		+ this->adPosi_TurnToHold;
        	Result.adPosi_InjectEnd			= moni1.adPosi_InjectEnd 			+ this->adPosi_InjectEnd;
        	Result.adPosi_InjectCushion	= moni1.adPosi_InjectCushion	+ this->adPosi_InjectCushion;
        	Result.adPosi_InjectPeak  	= moni1.adPosi_InjectPeak   	+ this->adPosi_InjectPeak;
        	Result.daPres_TurnToHold		= moni1.daPres_TurnToHold 		+ this->daPres_TurnToHold;
        	Result.cnChargeRPM					= moni1.cnChargeRPM 					+ this->cnChargeRPM;
        	Result.tmSuckBack						= moni1.tmSuckBack 						+ this->tmSuckBack;
        	Result.tmEject							= moni1.tmEject 							+ this->tmEject;
        	Result.tmEjectAdvance				= moni1.tmEjectAdvance 				+ this->tmEjectAdvance;
        	Result.tmEjectReturn				= moni1.tmEjectReturn 				+ this->tmEjectReturn;
        	Result.daPres_InjectPeak		= moni1.daPres_InjectPeak 		+ this->daPres_InjectPeak;
        	Result.daPres_ChargePeak		= moni1.daPres_ChargePeak 		+ this->daPres_ChargePeak;
        	Result.tmlInjectB						= moni1.tmlInjectB 						+ this->tmlInjectB;
        	Result.tmlChargeB						= moni1.tmlChargeB 						+ this->tmlChargeB;
        	Result.adPosi_InjectStartB	= moni1.adPosi_InjectStartB 	+ this->adPosi_InjectStartB;
        	Result.adPosi_TurnToHoldB		= moni1.adPosi_TurnToHoldB 		+ this->adPosi_TurnToHoldB;
        	Result.adPosi_InjectCushionB= moni1.adPosi_InjectCushionB + this->adPosi_InjectCushionB;
        	Result.daPres_InjecterB			= moni1.daPres_InjecterB 			+ this->daPres_InjecterB;
        	Result.daPres_ChargeBackB		= moni1.daPres_ChargeBackB 		+ this->daPres_ChargeBackB;
        	Result.tmTake								= moni1.tmTake			 					+ this->tmTake;
        	Result.adPosi_OpenMoldEnd		= moni1.adPosi_OpenMoldEnd 		+ this->adPosi_OpenMoldEnd;
        	Result.daPres_TurnToHoldB		= moni1.daPres_TurnToHoldB 		+ this->daPres_TurnToHoldB;
        	Result.tmlTurnToHoldB				= moni1.tmlTurnToHoldB 				+ this->tmlTurnToHoldB;
        	Result.adPosi_InjectHoldEndB= moni1.adPosi_InjectHoldEndB	+ this->adPosi_InjectHoldEndB;
        	Result.daPres_InjectMiddleB	= moni1.daPres_InjectMiddleB 	+ this->daPres_InjectMiddleB;
        	Result.daPres_ChargeMiddleB	= moni1.daPres_ChargeMiddleB 	+ this->daPres_ChargeMiddleB;
        	Result.InjectMaxSpeed				= moni1.InjectMaxSpeed 				+ this->InjectMaxSpeed;
        	Result.InjectMaxSpeedB			= moni1.InjectMaxSpeedB 			+ this->InjectMaxSpeedB;
        	Result.MuCellAveragePercent	= moni1.MuCellAveragePercent	+ this->MuCellAveragePercent;
        	Result.MuCellMaxPercent   	= moni1.MuCellMaxPercent    	+ this->MuCellMaxPercent;
        	Result.MuCellMinPercent   	= moni1.MuCellMinPercent    	+ this->MuCellMinPercent;
        	Result.MuCellSetPercent   	= moni1.MuCellSetPercent    	+ this->MuCellSetPercent;
//        	Result.MuCellShotWeight   	= moni1.MuCellShotWeight    	+ this->MuCellShotWeight;
//        	Result.MuCellBackPress    	= moni1.MuCellAveragePercent	+ this->MuCellBackPress;
        	Result.wReservedMG[0]			= moni1.wReservedMG[0] 			  + this->wReservedMG[0]; //ZHBA 2010-5-10 
        	Result.wReservedMG[1]			= moni1.wReservedMG[1] 			  + this->wReservedMG[1]; //ZHBA 2010-5-10 
        	Result.wReservedMG[2]			= moni1.wReservedMG[2] 			  + this->wReservedMG[2]; //ZHBA 2010-5-10 
        	Result.wReservedMG[3]			= moni1.wReservedMG[3] 			  + this->wReservedMG[3]; //ZHBA 2010-5-10 
        	Result.wReservedMG[4]			= moni1.wReservedMG[4] 			  + this->wReservedMG[4]; 
        	Result.wReservedMG[5]			= moni1.wReservedMG[5] 			  + this->wReservedMG[5]; 
        	Result.wReservedMG[6]			= moni1.wReservedMG[6] 			  + this->wReservedMG[6]; 
        	Result.wReservedMG[7]			= moni1.wReservedMG[7] 			  + this->wReservedMG[7];
        	Result.wReservedMG[8]			= moni1.wReservedMG[8] 			  + this->wReservedMG[8]; 
        	Result.wReservedMG[9]			= moni1.wReservedMG[9] 			  + this->wReservedMG[9]; 
        	Result.wReservedMG[10]			= moni1.wReservedMG[10]  		  + this->wReservedMG[10]; 
        	Result.wReservedMG[11]			= moni1.wReservedMG[11]  		  + this->wReservedMG[11];
        	Result.wReservedMG[12]			= moni1.wReservedMG[12]  		  + this->wReservedMG[12]; 
        	Result.wReservedMG[13]			= moni1.wReservedMG[13]  		  + this->wReservedMG[13]; 
        	Result.wReservedMG[14]			= moni1.wReservedMG[14]  		  + this->wReservedMG[14]; 
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator-(tagtmOIL_MONI2EX moni1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= (this->tmlCycle              	- moni1.tmlCycle				 			> 0) ? this->tmlCycle							- moni1.tmlCycle 							: 0;
        	Result.tmlInject 						= (this->tmlInject             	- moni1.tmlInject				 			> 0) ? this->tmlInject						- moni1.tmlInject 						: 0;
        	Result.tmlTurnToHold				= (this->tmlTurnToHold        	- moni1.tmlTurnToHold			 		> 0) ? this->tmlTurnToHold				- moni1.tmlTurnToHold					: 0;
        	Result.tmlCharge						= (this->tmlCharge             	- moni1.tmlCharge				 			> 0) ? this->tmlCharge						- moni1.tmlCharge 						: 0;
        	Result.adPosi_InjectStart		= (this->adPosi_InjectStart    	- moni1.adPosi_InjectStart		> 0) ? this->adPosi_InjectStart		- moni1.adPosi_InjectStart 		: 0;
        	Result.adPosi_TurnToHold		= (this->adPosi_TurnToHold     	- moni1.adPosi_TurnToHold		 	> 0) ? this->adPosi_TurnToHold		- moni1.adPosi_TurnToHold			: 0;
        	Result.adPosi_InjectEnd			= (this->adPosi_InjectEnd      	- moni1.adPosi_InjectEnd		 	> 0) ? this->adPosi_InjectEnd			- moni1.adPosi_InjectEnd			: 0;
        	Result.adPosi_InjectCushion	= (this->adPosi_InjectCushion  	- moni1.adPosi_InjectCushion	> 0) ? this->adPosi_InjectCushion	- moni1.adPosi_InjectCushion	: 0;
        	Result.adPosi_InjectPeak  	= (this->adPosi_InjectPeak  	  - moni1.adPosi_InjectPeak	    > 0) ? this->adPosi_InjectPeak   	- moni1.adPosi_InjectPeak   	: 0;
        	Result.daPres_TurnToHold		= (this->daPres_TurnToHold     	- moni1.daPres_TurnToHold		 	> 0) ? this->daPres_TurnToHold		- moni1.daPres_TurnToHold 		: 0;
        	Result.cnChargeRPM					= (this->cnChargeRPM           	- moni1.cnChargeRPM				 		> 0) ? this->cnChargeRPM					- moni1.cnChargeRPM 					: 0;
        	Result.tmSuckBack						= (this->tmSuckBack            	- moni1.tmSuckBack				 		> 0) ? this->tmSuckBack						- moni1.tmSuckBack						: 0;
        	Result.tmEject							= (this->tmEject               	- moni1.tmEject					 			> 0) ? this->tmEject							- moni1.tmEject 							: 0;
        	Result.tmEjectAdvance				= (this->tmEjectAdvance        	- moni1.tmEjectAdvance			 	> 0) ? this->tmEjectAdvance				- moni1.tmEjectAdvance 				: 0;
        	Result.tmEjectReturn				= (this->tmEjectReturn         	- moni1.tmEjectReturn			 		> 0) ? this->tmEjectReturn				- moni1.tmEjectReturn 				: 0;
        	Result.daPres_InjectPeak		= (this->daPres_InjectPeak     	- moni1.daPres_InjectPeak			> 0) ? this->daPres_InjectPeak		- moni1.daPres_InjectPeak 		: 0;
        	Result.daPres_ChargePeak		= (this->daPres_ChargePeak     	- moni1.daPres_ChargePeak			> 0) ? this->daPres_ChargePeak		- moni1.daPres_ChargePeak 		: 0;
        	Result.tmlInjectB						= (this->tmlInjectB            	- moni1.tmlInjectB				 		> 0) ? this->tmlInjectB           - moni1.tmlInjectB 						: 0;
        	Result.tmlChargeB						= (this->tmlChargeB            	- moni1.tmlChargeB				 		> 0) ? this->tmlChargeB           - moni1.tmlChargeB 						: 0;
        	Result.adPosi_InjectStartB	= (this->adPosi_InjectStartB   	- moni1.adPosi_InjectStartB		> 0) ? this->adPosi_InjectStartB  - moni1.adPosi_InjectStartB 	: 0;
        	Result.adPosi_TurnToHoldB		= (this->adPosi_TurnToHoldB    	- moni1.adPosi_TurnToHoldB		> 0) ? this->adPosi_TurnToHoldB   - moni1.adPosi_TurnToHoldB 		: 0;
        	Result.adPosi_InjectCushionB= (this->adPosi_InjectCushionB 	- moni1.adPosi_InjectCushionB	> 0) ? this->adPosi_InjectCushionB- moni1.adPosi_InjectCushionB : 0;
        	Result.daPres_InjecterB			= (this->daPres_InjecterB      	- moni1.daPres_InjecterB		 	> 0) ? this->daPres_InjecterB     - moni1.daPres_InjecterB 			: 0;
        	Result.daPres_ChargeBackB		= (this->daPres_ChargeBackB    	- moni1.daPres_ChargeBackB		> 0) ? this->daPres_ChargeBackB   - moni1.daPres_ChargeBackB 		: 0;
        	Result.tmTake								= (this->tmTake   							- moni1.tmTake			 		 			> 0) ? this->tmTake    						- moni1.tmTake 								: 0;
        	Result.adPosi_OpenMoldEnd		= (this->adPosi_OpenMoldEnd			- moni1.adPosi_OpenMoldEnd 		> 0) ? this->adPosi_OpenMoldEnd		- moni1.adPosi_OpenMoldEnd 		: 0;
        	Result.daPres_TurnToHoldB		= (this->daPres_TurnToHoldB			- moni1.daPres_TurnToHoldB		> 0) ? this->daPres_TurnToHoldB		- moni1.daPres_TurnToHoldB		: 0;
        	Result.tmlTurnToHoldB				= (this->tmlTurnToHoldB					- moni1.tmlTurnToHoldB				> 0) ? this->tmlTurnToHoldB				- moni1.tmlTurnToHoldB				: 0;
        	Result.adPosi_InjectHoldEndB= (this->adPosi_InjectHoldEndB	- moni1.adPosi_InjectHoldEndB > 0) ? this->adPosi_InjectHoldEndB- moni1.adPosi_InjectHoldEndB : 0;
        	Result.daPres_InjectMiddleB	= (this->daPres_InjectMiddleB		- moni1.daPres_InjectMiddleB	> 0) ? this->daPres_InjectMiddleB	- moni1.daPres_InjectMiddleB	: 0;
        	Result.daPres_ChargeMiddleB	= (this->daPres_ChargeMiddleB		- moni1.daPres_ChargeMiddleB	> 0) ? this->daPres_ChargeMiddleB	- moni1.daPres_ChargeMiddleB	: 0;
        	Result.InjectMaxSpeed				= (this->InjectMaxSpeed					- moni1.InjectMaxSpeed				> 0) ? this->InjectMaxSpeed				- moni1.InjectMaxSpeed				: 0;
        	Result.InjectMaxSpeedB			= (this->InjectMaxSpeedB				- moni1.InjectMaxSpeedB			  > 0) ? this->InjectMaxSpeedB			- moni1.InjectMaxSpeedB			  : 0;
        	Result.MuCellAveragePercent	= (this->MuCellAveragePercent		- moni1.MuCellAveragePercent  > 0) ? this->MuCellAveragePercent	- moni1.MuCellAveragePercent  : 0;
        	Result.MuCellMaxPercent			= (this->MuCellMaxPercent				- moni1.MuCellMaxPercent  		> 0) ? this->MuCellMaxPercent			- moni1.MuCellMaxPercent  		: 0;
        	Result.MuCellMinPercent			= (this->MuCellMinPercent				- moni1.MuCellMinPercent  		> 0) ? this->MuCellMinPercent			- moni1.MuCellMinPercent  		: 0;
        	Result.MuCellSetPercent			= (this->MuCellSetPercent				- moni1.MuCellSetPercent  		> 0) ? this->MuCellSetPercent			- moni1.MuCellSetPercent  		: 0;
//        	Result.MuCellShotWeight			= (this->MuCellShotWeight				- moni1.MuCellShotWeight  		> 0) ? this->MuCellShotWeight			- moni1.MuCellShotWeight  		: 0;
//        	Result.MuCellBackPress 			= (this->MuCellBackPress 				- moni1.MuCellBackPress   		> 0) ? this->MuCellBackPress 			- moni1.MuCellBackPress   		: 0;
        	Result.wReservedMG[0]			  = (this->wReservedMG[0]					  - moni1.wReservedMG[0]				  > 0) ? this->wReservedMG[0]				  - moni1.wReservedMG[0]				  : 0;  //ZHBA 2010-5-10 
        	Result.wReservedMG[1]			  = (this->wReservedMG[1]					  - moni1.wReservedMG[1]				  > 0) ? this->wReservedMG[1]				  - moni1.wReservedMG[1]				  : 0;  //ZHBA 2010-5-10 
        	Result.wReservedMG[2]			  = (this->wReservedMG[2]					  - moni1.wReservedMG[2]				  > 0) ? this->wReservedMG[2]				  - moni1.wReservedMG[2]				  : 0;  //ZHBA 2010-5-10 
        	Result.wReservedMG[3]			  = (this->wReservedMG[3]					  - moni1.wReservedMG[3]				  > 0) ? this->wReservedMG[3]				  - moni1.wReservedMG[3]				  : 0;  //ZHBA 2010-5-10 
        	Result.wReservedMG[4]			  = (this->wReservedMG[4]					  - moni1.wReservedMG[4]				  > 0) ? this->wReservedMG[4]				  - moni1.wReservedMG[4]				  : 0;  
        	Result.wReservedMG[5]			  = (this->wReservedMG[5]					  - moni1.wReservedMG[5]				  > 0) ? this->wReservedMG[5]				  - moni1.wReservedMG[5]				  : 0;  
        	Result.wReservedMG[6]			  = (this->wReservedMG[6]					  - moni1.wReservedMG[6]				  > 0) ? this->wReservedMG[6]				  - moni1.wReservedMG[6]				  : 0;  
        	Result.wReservedMG[7]			  = (this->wReservedMG[7]					  - moni1.wReservedMG[7]				  > 0) ? this->wReservedMG[7]				  - moni1.wReservedMG[7]				  : 0;  
        	Result.wReservedMG[8]			  = (this->wReservedMG[8]					  - moni1.wReservedMG[8]				  > 0) ? this->wReservedMG[8]				  - moni1.wReservedMG[8]				  : 0;  
        	Result.wReservedMG[9]			  = (this->wReservedMG[9]					  - moni1.wReservedMG[9]				  > 0) ? this->wReservedMG[9]				  - moni1.wReservedMG[9]				  : 0;  
        	Result.wReservedMG[10]			  = (this->wReservedMG[10]  				  - moni1.wReservedMG[10]   			  > 0) ? this->wReservedMG[10]  			  - moni1.wReservedMG[10]   			  : 0;  
        	Result.wReservedMG[11]			  = (this->wReservedMG[11]  				  - moni1.wReservedMG[11]   			  > 0) ? this->wReservedMG[11]  			  - moni1.wReservedMG[11]   			  : 0;  
        	Result.wReservedMG[12]			  = (this->wReservedMG[12]  				  - moni1.wReservedMG[12]   			  > 0) ? this->wReservedMG[12]  			  - moni1.wReservedMG[12]   			  : 0;  
        	Result.wReservedMG[13]			  = (this->wReservedMG[13]  				  - moni1.wReservedMG[13]   			  > 0) ? this->wReservedMG[13]  			  - moni1.wReservedMG[13]   			  : 0;  
        	Result.wReservedMG[14]			  = (this->wReservedMG[14]  				  - moni1.wReservedMG[14]   			  > 0) ? this->wReservedMG[14]  			  - moni1.wReservedMG[14]   			  : 0;  
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator-(tagtmOIL_MONI2 moni1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= (this->tmlCycle              	- moni1.tmlCycle				 			> 0) ? this->tmlCycle             - moni1.tmlCycle							: 0;
        	Result.tmlInject 						= (this->tmlInject             	- moni1.tmlInject				 			> 0) ? this->tmlInject            - moni1.tmlInject							: 0;
        	Result.tmlTurnToHold				= (this->tmlTurnToHold        	- moni1.tmlTurnToHold			 		> 0) ? this->tmlTurnToHold        - moni1.tmlTurnToHold					: 0;
        	Result.tmlCharge						= (this->tmlCharge             	- moni1.tmlCharge				 			> 0) ? this->tmlCharge            - moni1.tmlCharge							: 0;
        	Result.adPosi_InjectStart		= (this->adPosi_InjectStart    	- moni1.adPosi_InjectStart		> 0) ? this->adPosi_InjectStart   - moni1.adPosi_InjectStart		: 0;
        	Result.adPosi_TurnToHold		= (this->adPosi_TurnToHold     	- moni1.adPosi_TurnToHold		 	> 0) ? this->adPosi_TurnToHold    - moni1.adPosi_TurnToHold			: 0;
        	Result.adPosi_InjectEnd			= (this->adPosi_InjectEnd      	- moni1.adPosi_InjectEnd		 	> 0) ? this->adPosi_InjectEnd     - moni1.adPosi_InjectEnd			: 0;
        	Result.adPosi_InjectCushion	= (this->adPosi_InjectCushion  	- moni1.adPosi_InjectCushion	> 0) ? this->adPosi_InjectCushion - moni1.adPosi_InjectCushion	: 0;
        	Result.adPosi_InjectPeak		= (this->adPosi_InjectPeak     	- moni1.adPosi_InjectPeak		 	> 0) ? this->adPosi_InjectPeak    - moni1.adPosi_InjectPeak			: 0;
        	Result.daPres_TurnToHold		= (this->daPres_TurnToHold     	- moni1.daPres_TurnToHold		 	> 0) ? this->daPres_TurnToHold    - moni1.daPres_TurnToHold			: 0;
        	Result.cnChargeRPM					= (this->cnChargeRPM           	- moni1.cnChargeRPM				 		> 0) ? this->cnChargeRPM          - moni1.cnChargeRPM						: 0;
        	Result.tmSuckBack						= (this->tmSuckBack            	- moni1.tmSuckBack				 		> 0) ? this->tmSuckBack           - moni1.tmSuckBack						: 0;
        	Result.tmEject							= (this->tmEject               	- moni1.tmEject					 			> 0) ? this->tmEject              - moni1.tmEject								: 0;
        	Result.tmEjectAdvance				= (this->tmEjectAdvance        	- moni1.tmEjectAdvance				> 0) ? this->tmEjectAdvance       - moni1.tmEjectAdvance				: 0;
        	Result.tmEjectReturn				= (this->tmEjectReturn         	- moni1.tmEjectReturn			 		> 0) ? this->tmEjectReturn        - moni1.tmEjectReturn					: 0;
        	Result.daPres_InjectPeak		= (this->daPres_InjectPeak     	- moni1.daPres_InjectPeak		 	> 0) ? this->daPres_InjectPeak    - moni1.daPres_InjectPeak			: 0;
        	Result.daPres_ChargePeak		= (this->daPres_ChargePeak     	- moni1.daPres_ChargePeak			> 0) ? this->daPres_ChargePeak    - moni1.daPres_ChargePeak			: 0;
        	Result.tmlInjectB						= (this->tmlInjectB            	- moni1.tmlInjectB				 		> 0) ? this->tmlInjectB           - moni1.tmlInjectB						: 0;
        	Result.tmlChargeB						= (this->tmlChargeB            	- moni1.tmlChargeB				 		> 0) ? this->tmlChargeB           - moni1.tmlChargeB						: 0;
        	Result.adPosi_InjectStartB	= (this->adPosi_InjectStartB   	- moni1.adPosi_InjectStartB		> 0) ? this->adPosi_InjectStartB  - moni1.adPosi_InjectStartB		: 0;
        	Result.adPosi_TurnToHoldB		= (this->adPosi_TurnToHoldB    	- moni1.adPosi_TurnToHoldB		> 0) ? this->adPosi_TurnToHoldB   - moni1.adPosi_TurnToHoldB		: 0;
        	Result.adPosi_InjectCushionB= (this->adPosi_InjectCushionB 	- moni1.adPosi_InjectCushionB	> 0) ? this->adPosi_InjectCushionB- moni1.adPosi_InjectCushionB	: 0;
        	Result.daPres_InjecterB			= (this->daPres_InjecterB      	- moni1.daPres_InjecterB		 	> 0) ? this->daPres_InjecterB     - moni1.daPres_InjecterB			: 0;
        	Result.daPres_ChargeBackB		= (this->daPres_ChargeBackB    	- moni1.daPres_ChargeBackB		> 0) ? this->daPres_ChargeBackB   - moni1.daPres_ChargeBackB		: 0;
        	Result.tmTake								= (this->tmTake    							- moni1.tmTake		 			 			> 0) ? this->tmTake    						- moni1.tmTake								: 0;
        	Result.adPosi_OpenMoldEnd		= (this->adPosi_OpenMoldEnd    	- moni1.adPosi_OpenMoldEnd		> 0) ? this->adPosi_OpenMoldEnd		- moni1.adPosi_OpenMoldEnd		: 0;
        	Result.daPres_TurnToHoldB		= (this->daPres_TurnToHoldB			- moni1.daPres_TurnToHoldB		> 0) ? this->daPres_TurnToHoldB		- moni1.daPres_TurnToHoldB		: 0;
        	Result.tmlTurnToHoldB				= (this->tmlTurnToHoldB					- moni1.tmlTurnToHoldB				> 0) ? this->tmlTurnToHoldB				- moni1.tmlTurnToHoldB				: 0;
        	Result.adPosi_InjectHoldEndB= (this->adPosi_InjectHoldEndB	- moni1.adPosi_InjectHoldEndB > 0) ? this->adPosi_InjectHoldEndB- moni1.adPosi_InjectHoldEndB : 0;
        	Result.daPres_InjectMiddleB	= (this->daPres_InjectMiddleB		- moni1.daPres_InjectMiddleB	> 0) ? this->daPres_InjectMiddleB	- moni1.daPres_InjectMiddleB	: 0;
        	Result.daPres_ChargeMiddleB	= (this->daPres_ChargeMiddleB		- moni1.daPres_ChargeMiddleB	> 0) ? this->daPres_ChargeMiddleB	- moni1.daPres_ChargeMiddleB	: 0;
        	Result.InjectMaxSpeed				= (this->InjectMaxSpeed					- moni1.InjectMaxSpeed				> 0) ? this->InjectMaxSpeed				- moni1.InjectMaxSpeed				: 0;
        	Result.InjectMaxSpeedB			= (this->InjectMaxSpeedB				- moni1.InjectMaxSpeedB			  > 0) ? this->InjectMaxSpeedB			- moni1.InjectMaxSpeedB			  : 0;
        	Result.MuCellAveragePercent	= (this->MuCellAveragePercent		- moni1.MuCellAveragePercent  > 0) ? this->MuCellAveragePercent	- moni1.MuCellAveragePercent  : 0;
        	Result.MuCellMaxPercent			= (this->MuCellMaxPercent				- moni1.MuCellMaxPercent  		> 0) ? this->MuCellMaxPercent			- moni1.MuCellMaxPercent  		: 0;
        	Result.MuCellMinPercent			= (this->MuCellMinPercent				- moni1.MuCellMinPercent  		> 0) ? this->MuCellMinPercent			- moni1.MuCellMinPercent  		: 0;
        	Result.MuCellSetPercent			= (this->MuCellSetPercent				- moni1.MuCellSetPercent  		> 0) ? this->MuCellSetPercent			- moni1.MuCellSetPercent  		: 0;
 //       	Result.MuCellShotWeight			= (this->MuCellShotWeight				- moni1.MuCellShotWeight  		> 0) ? this->MuCellShotWeight			- moni1.MuCellShotWeight  		: 0;
 //       	Result.MuCellBackPress 			= (this->MuCellBackPress 				- moni1.MuCellBackPress   		> 0) ? this->MuCellBackPress 			- moni1.MuCellBackPress   		: 0;
        	Result.wReservedMG[0]			  = (this->wReservedMG[0]				  - moni1.wReservedMG[0]			  > 0) ? this->wReservedMG[0]			  - moni1.wReservedMG[0]			  : 0;//ZHBA 2010-5-10
        	Result.wReservedMG[1]			  = (this->wReservedMG[1]				  - moni1.wReservedMG[1]			  > 0) ? this->wReservedMG[1]			  - moni1.wReservedMG[1]			  : 0;//ZHBA 2010-5-10
        	Result.wReservedMG[2]			  = (this->wReservedMG[2]				  - moni1.wReservedMG[2]			  > 0) ? this->wReservedMG[2]			  - moni1.wReservedMG[2]			  : 0;//ZHBA 2010-5-10
        	Result.wReservedMG[3]			  = (this->wReservedMG[3]				  - moni1.wReservedMG[3]			  > 0) ? this->wReservedMG[3]			  - moni1.wReservedMG[3]			  : 0;//ZHBA 2010-5-10
        	Result.wReservedMG[4]			  = (this->wReservedMG[4]				  - moni1.wReservedMG[4]			  > 0) ? this->wReservedMG[4]			  - moni1.wReservedMG[4]			  : 0;  
        	Result.wReservedMG[5]			  = (this->wReservedMG[5]				  - moni1.wReservedMG[5]			  > 0) ? this->wReservedMG[5]			  - moni1.wReservedMG[5]			  : 0;  
        	Result.wReservedMG[6]			  = (this->wReservedMG[6]				  - moni1.wReservedMG[6]			  > 0) ? this->wReservedMG[6]			  - moni1.wReservedMG[6]			  : 0;  
        	Result.wReservedMG[7]			  = (this->wReservedMG[7]				  - moni1.wReservedMG[7]			  > 0) ? this->wReservedMG[7]			  - moni1.wReservedMG[7]			  : 0;  
        	Result.wReservedMG[8]			  = (this->wReservedMG[8]				  - moni1.wReservedMG[8]			  > 0) ? this->wReservedMG[8]			  - moni1.wReservedMG[8]			  : 0;  
        	Result.wReservedMG[9]			  = (this->wReservedMG[9]				  - moni1.wReservedMG[9]			  > 0) ? this->wReservedMG[9]			  - moni1.wReservedMG[9]			  : 0;  
        	Result.wReservedMG[10]			  = (this->wReservedMG[10]  			  - moni1.wReservedMG[10]   		  > 0) ? this->wReservedMG[10]  		  - moni1.wReservedMG[10]   		  : 0;  
        	Result.wReservedMG[11]			  = (this->wReservedMG[11]  			  - moni1.wReservedMG[11]   		  > 0) ? this->wReservedMG[11]  		  - moni1.wReservedMG[11]   		  : 0;  
        	Result.wReservedMG[12]			  = (this->wReservedMG[12]  			  - moni1.wReservedMG[12]   		  > 0) ? this->wReservedMG[12]  		  - moni1.wReservedMG[12]   		  : 0;  
        	Result.wReservedMG[13]			  = (this->wReservedMG[13]  			  - moni1.wReservedMG[13]   		  > 0) ? this->wReservedMG[13]  		  - moni1.wReservedMG[13]   		  : 0;  
        	Result.wReservedMG[14]			  = (this->wReservedMG[14]  			  - moni1.wReservedMG[14]   		  > 0) ? this->wReservedMG[14]  		  - moni1.wReservedMG[14]   		  : 0;  
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator*(tagtmOIL_MONI2EX moni1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= moni1.tmlCycle 							* this->tmlCycle;
        	Result.tmlInject 						= moni1.tmlInject 						* this->tmlInject;
        	Result.tmlTurnToHold				= moni1.tmlTurnToHold 				* this->tmlTurnToHold;
        	Result.tmlCharge						= moni1.tmlCharge 						* this->tmlCharge;
        	Result.adPosi_InjectStart		= moni1.adPosi_InjectStart 		* this->adPosi_InjectStart;
        	Result.adPosi_TurnToHold		= moni1.adPosi_TurnToHold 		* this->adPosi_TurnToHold;
        	Result.adPosi_InjectEnd			= moni1.adPosi_InjectEnd 			* this->adPosi_InjectEnd;
        	Result.adPosi_InjectCushion	= moni1.adPosi_InjectCushion	* this->adPosi_InjectCushion;
        	Result.adPosi_InjectPeak		= moni1.adPosi_InjectPeak 		* this->adPosi_InjectPeak;
        	Result.daPres_TurnToHold		= moni1.daPres_TurnToHold 		* this->daPres_TurnToHold;
        	Result.cnChargeRPM					= moni1.cnChargeRPM 					* this->cnChargeRPM;
        	Result.tmSuckBack						= moni1.tmSuckBack 						* this->tmSuckBack;
        	Result.tmEject							= moni1.tmEject 							* this->tmEject;
        	Result.tmEjectAdvance				= moni1.tmEjectAdvance 				* this->tmEjectAdvance;
        	Result.tmEjectReturn				= moni1.tmEjectReturn 				* this->tmEjectReturn;
        	Result.daPres_InjectPeak		= moni1.daPres_InjectPeak 		* this->daPres_InjectPeak;
        	Result.daPres_ChargePeak		= moni1.daPres_ChargePeak 		* this->daPres_ChargePeak;
        	Result.tmlInjectB						= moni1.tmlInjectB 						* this->tmlInjectB;
        	Result.tmlChargeB						= moni1.tmlChargeB 						* this->tmlChargeB;
        	Result.adPosi_InjectStartB	= moni1.adPosi_InjectStartB 	* this->adPosi_InjectStartB;
        	Result.adPosi_TurnToHoldB		= moni1.adPosi_TurnToHoldB 		* this->adPosi_TurnToHoldB;
        	Result.adPosi_InjectCushionB= moni1.adPosi_InjectCushionB * this->adPosi_InjectCushionB;
        	Result.daPres_InjecterB			= moni1.daPres_InjecterB 			* this->daPres_InjecterB;
        	Result.daPres_ChargeBackB		= moni1.daPres_ChargeBackB 		* this->daPres_ChargeBackB;
        	Result.tmTake								= moni1.tmTake 								* this->tmTake;
        	Result.adPosi_OpenMoldEnd		= moni1.adPosi_OpenMoldEnd		* this->adPosi_OpenMoldEnd;
        	Result.daPres_TurnToHoldB		= moni1.daPres_TurnToHoldB		* this->daPres_TurnToHoldB		;
        	Result.tmlTurnToHoldB				= moni1.tmlTurnToHoldB				* this->tmlTurnToHoldB				;
        	Result.adPosi_InjectHoldEndB= moni1.adPosi_InjectHoldEndB * this->adPosi_InjectHoldEndB ;
        	Result.daPres_InjectMiddleB	= moni1.daPres_InjectMiddleB	* this->daPres_InjectMiddleB	;
        	Result.daPres_ChargeMiddleB	= moni1.daPres_ChargeMiddleB	* this->daPres_ChargeMiddleB	;
        	Result.InjectMaxSpeed				= moni1.InjectMaxSpeed				* this->InjectMaxSpeed				;
        	Result.InjectMaxSpeedB			= moni1.InjectMaxSpeedB			  * this->InjectMaxSpeedB			  ;
        	Result.MuCellAveragePercent	= moni1.MuCellAveragePercent  * this->MuCellAveragePercent  ;
        	Result.MuCellMaxPercent			= moni1.MuCellMaxPercent  		* this->MuCellMaxPercent  ;
        	Result.MuCellMinPercent			= moni1.MuCellMinPercent  		* this->MuCellMinPercent  ;
        	Result.MuCellSetPercent			= moni1.MuCellSetPercent  		* this->MuCellSetPercent  ;
//        	Result.MuCellShotWeight			= moni1.MuCellShotWeight  		* this->MuCellShotWeight  ;
//        	Result.MuCellBackPress 			= moni1.MuCellBackPress   		* this->MuCellBackPress   ;
        	Result.wReservedMG[0]			  = moni1.wReservedMG[0]			  * this->wReservedMG[0]			  ;//ZHBA 2010-5-10
        	Result.wReservedMG[1]			  = moni1.wReservedMG[1]			  * this->wReservedMG[1]			  ;//ZHBA 2010-5-10
        	Result.wReservedMG[2]			  = moni1.wReservedMG[2]			  * this->wReservedMG[2]			  ;//ZHBA 2010-5-10
        	Result.wReservedMG[3]			  = moni1.wReservedMG[3]			  * this->wReservedMG[3]			  ;//ZHBA 2010-5-10 
        	Result.wReservedMG[4]			  = moni1.wReservedMG[4]			  * this->wReservedMG[4]			  ;
        	Result.wReservedMG[5]			  = moni1.wReservedMG[5]			  * this->wReservedMG[5]			  ;
        	Result.wReservedMG[6]			  = moni1.wReservedMG[6]			  * this->wReservedMG[6]			  ;
        	Result.wReservedMG[7]			  = moni1.wReservedMG[7]			  * this->wReservedMG[7]			  ;
        	Result.wReservedMG[8]			  = moni1.wReservedMG[8]			  * this->wReservedMG[8]			  ;
        	Result.wReservedMG[9]			  = moni1.wReservedMG[9]			  * this->wReservedMG[9]			  ;
        	Result.wReservedMG[10]			  = moni1.wReservedMG[10]			  * this->wReservedMG[10]			  ;
        	Result.wReservedMG[11]			  = moni1.wReservedMG[11]			  * this->wReservedMG[11]			  ;
        	Result.wReservedMG[12]			  = moni1.wReservedMG[12]			  * this->wReservedMG[12]			  ;
        	Result.wReservedMG[13]			  = moni1.wReservedMG[13]			  * this->wReservedMG[13]			  ;
        	Result.wReservedMG[14]			  = moni1.wReservedMG[14]			  * this->wReservedMG[14]			  ;
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator*(tagtmOIL_MONI2 moni1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= moni1.tmlCycle 							* this->tmlCycle;
        	Result.tmlInject 						= moni1.tmlInject 						* this->tmlInject;
        	Result.tmlTurnToHold				= moni1.tmlTurnToHold 				* this->tmlTurnToHold;
        	Result.tmlCharge						= moni1.tmlCharge 						* this->tmlCharge;
        	Result.adPosi_InjectStart		= moni1.adPosi_InjectStart 		* this->adPosi_InjectStart;
        	Result.adPosi_TurnToHold		= moni1.adPosi_TurnToHold 		* this->adPosi_TurnToHold;
        	Result.adPosi_InjectEnd			= moni1.adPosi_InjectEnd 			* this->adPosi_InjectEnd;
        	Result.adPosi_InjectCushion	= moni1.adPosi_InjectCushion	* this->adPosi_InjectCushion;
        	Result.adPosi_InjectPeak		= moni1.adPosi_InjectPeak 		* this->adPosi_InjectPeak;
        	Result.daPres_TurnToHold		= moni1.daPres_TurnToHold 		* this->daPres_TurnToHold;
        	Result.cnChargeRPM					= moni1.cnChargeRPM 					* this->cnChargeRPM;
        	Result.tmSuckBack						= moni1.tmSuckBack 						* this->tmSuckBack;
        	Result.tmEject							= moni1.tmEject 							* this->tmEject;
        	Result.tmEjectAdvance				= moni1.tmEjectAdvance 				* this->tmEjectAdvance;
        	Result.tmEjectReturn				= moni1.tmEjectReturn 				* this->tmEjectReturn;
        	Result.daPres_InjectPeak		= moni1.daPres_InjectPeak 		* this->daPres_InjectPeak;
        	Result.daPres_ChargePeak		= moni1.daPres_ChargePeak 		* this->daPres_ChargePeak;
        	Result.tmlInjectB						= moni1.tmlInjectB 						* this->tmlInjectB;
        	Result.tmlChargeB						= moni1.tmlChargeB 						* this->tmlChargeB;
        	Result.adPosi_InjectStartB	= moni1.adPosi_InjectStartB 	* this->adPosi_InjectStartB;
        	Result.adPosi_TurnToHoldB		= moni1.adPosi_TurnToHoldB 		* this->adPosi_TurnToHoldB;
        	Result.adPosi_InjectCushionB= moni1.adPosi_InjectCushionB * this->adPosi_InjectCushionB;
        	Result.daPres_InjecterB			= moni1.daPres_InjecterB 			* this->daPres_InjecterB;
        	Result.daPres_ChargeBackB		= moni1.daPres_ChargeBackB 		* this->daPres_ChargeBackB;
        	Result.tmTake								= moni1.tmTake 								* this->tmTake;
        	Result.adPosi_OpenMoldEnd		= moni1.adPosi_OpenMoldEnd		* this->adPosi_OpenMoldEnd;
        	Result.daPres_TurnToHoldB		= moni1.daPres_TurnToHoldB		* this->daPres_TurnToHoldB		;
        	Result.tmlTurnToHoldB				= moni1.tmlTurnToHoldB				* this->tmlTurnToHoldB				;
        	Result.adPosi_InjectHoldEndB= moni1.adPosi_InjectHoldEndB * this->adPosi_InjectHoldEndB ;
        	Result.daPres_InjectMiddleB	= moni1.daPres_InjectMiddleB	* this->daPres_InjectMiddleB	;
        	Result.daPres_ChargeMiddleB	= moni1.daPres_ChargeMiddleB	* this->daPres_ChargeMiddleB	;
        	Result.InjectMaxSpeed				= moni1.InjectMaxSpeed				* this->InjectMaxSpeed				;
        	Result.InjectMaxSpeedB			= moni1.InjectMaxSpeedB			  * this->InjectMaxSpeedB			  ;
        	Result.MuCellAveragePercent	= moni1.MuCellAveragePercent  * this->MuCellAveragePercent  ;
        	Result.MuCellMaxPercent			= moni1.MuCellMaxPercent  		* this->MuCellMaxPercent  ;
        	Result.MuCellMinPercent			= moni1.MuCellMinPercent  		* this->MuCellMinPercent  ;
        	Result.MuCellSetPercent			= moni1.MuCellSetPercent  		* this->MuCellSetPercent  ;
//        	Result.MuCellShotWeight			= moni1.MuCellShotWeight  		* this->MuCellShotWeight  ;
//        	Result.MuCellBackPress 			= moni1.MuCellBackPress   		* this->MuCellBackPress   ;
        	Result.wReservedMG[0]			  = moni1.wReservedMG[0]			  * this->wReservedMG[0]			  ;//ZHBA 2010-5-10
        	Result.wReservedMG[1]			  = moni1.wReservedMG[1]			  * this->wReservedMG[1]			  ;//ZHBA 2010-5-10
        	Result.wReservedMG[2]			  = moni1.wReservedMG[2]			  * this->wReservedMG[2]			  ;//ZHBA 2010-5-10
        	Result.wReservedMG[3]			  = moni1.wReservedMG[3]			  * this->wReservedMG[3]			  ;//ZHBA 2010-5-10 
        	Result.wReservedMG[4]			  = moni1.wReservedMG[4]			  * this->wReservedMG[4]			  ;
        	Result.wReservedMG[5]			  = moni1.wReservedMG[5]			  * this->wReservedMG[5]			  ;
        	Result.wReservedMG[6]			  = moni1.wReservedMG[6]			  * this->wReservedMG[6]			  ;
        	Result.wReservedMG[7]			  = moni1.wReservedMG[7]			  * this->wReservedMG[7]			  ;
        	Result.wReservedMG[8]			  = moni1.wReservedMG[8]			  * this->wReservedMG[8]			  ;
        	Result.wReservedMG[9]			  = moni1.wReservedMG[9]			  * this->wReservedMG[9]			  ;
        	Result.wReservedMG[10]			  = moni1.wReservedMG[10]			  * this->wReservedMG[10]			  ;
        	Result.wReservedMG[11]			  = moni1.wReservedMG[11]			  * this->wReservedMG[11]			  ;
        	Result.wReservedMG[12]			  = moni1.wReservedMG[12]			  * this->wReservedMG[12]			  ;
        	Result.wReservedMG[13]			  = moni1.wReservedMG[13]			  * this->wReservedMG[13]			  ;
        	Result.wReservedMG[14]			  = moni1.wReservedMG[14]			  * this->wReservedMG[14]			  ;
        	return Result;
        }
        
        
        
        tagtmOIL_MONI2EX operator*(int n1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= (this->tmlCycle              * n1);
        	Result.tmlInject 						= (this->tmlInject             * n1);
        	Result.tmlTurnToHold				= (this->tmlTurnToHold         * n1);
        	Result.tmlCharge						= (this->tmlCharge             * n1);
        	Result.adPosi_InjectStart		= (this->adPosi_InjectStart    * n1);
        	Result.adPosi_TurnToHold		= (this->adPosi_TurnToHold     * n1);
        	Result.adPosi_InjectEnd			= (this->adPosi_InjectEnd      * n1);
        	Result.adPosi_InjectCushion	= (this->adPosi_InjectCushion  * n1);
        	Result.adPosi_InjectPeak		= (this->adPosi_InjectPeak     * n1);
        	Result.daPres_TurnToHold		= (this->daPres_TurnToHold     * n1);
        	Result.cnChargeRPM					= (this->cnChargeRPM           * n1);
        	Result.tmSuckBack						= (this->tmSuckBack            * n1);
        	Result.tmEject							= (this->tmEject               * n1);
        	Result.tmEjectAdvance				= (this->tmEjectAdvance        * n1);
        	Result.tmEjectReturn				= (this->tmEjectReturn         * n1);
        	Result.daPres_InjectPeak		= (this->daPres_InjectPeak     * n1);
        	Result.daPres_ChargePeak		= (this->daPres_ChargePeak     * n1);
        	Result.tmlInjectB						= (this->tmlInjectB            * n1);
        	Result.tmlChargeB						= (this->tmlChargeB            * n1);
        	Result.adPosi_InjectStartB	= (this->adPosi_InjectStartB   * n1);
        	Result.adPosi_TurnToHoldB		= (this->adPosi_TurnToHoldB    * n1);
        	Result.adPosi_InjectCushionB= (this->adPosi_InjectCushionB * n1);
        	Result.daPres_InjecterB			= (this->daPres_InjecterB      * n1);
        	Result.daPres_ChargeBackB		= (this->daPres_ChargeBackB    * n1);
        	Result.tmTake								= (this->tmTake  		  			   * n1);
        	Result.adPosi_OpenMoldEnd		= (this->adPosi_OpenMoldEnd	   * n1);
        	Result.daPres_TurnToHoldB		= (this->daPres_TurnToHoldB		 * n1);
        	Result.tmlTurnToHoldB				= (this->tmlTurnToHoldB				 * n1);
        	Result.adPosi_InjectHoldEndB= (this->adPosi_InjectHoldEndB * n1);
        	Result.daPres_InjectMiddleB	= (this->daPres_InjectMiddleB	 * n1);
        	Result.daPres_ChargeMiddleB	= (this->daPres_ChargeMiddleB	 * n1);
        	Result.InjectMaxSpeed				= (this->InjectMaxSpeed				 * n1);
        	Result.InjectMaxSpeedB			= (this->InjectMaxSpeedB			 * n1);
        	Result.MuCellAveragePercent	= (this->MuCellAveragePercent	 * n1);
        	Result.MuCellMaxPercent			= (this->MuCellMaxPercent	 		 * n1);
        	Result.MuCellMinPercent			= (this->MuCellMinPercent	 		 * n1);
        	Result.MuCellSetPercent			= (this->MuCellSetPercent	 		 * n1);
//        	Result.MuCellShotWeight			= (this->MuCellShotWeight	 		 * n1);
//        	Result.MuCellBackPress 			= (this->MuCellBackPress 	 		 * n1);
        	Result.wReservedMG[0]			  = (this->wReservedMG[0]			  * n1); //ZHBA 2010-5-10
        	Result.wReservedMG[1]			  = (this->wReservedMG[1]			  * n1); //ZHBA 2010-5-10
        	Result.wReservedMG[2]			  = (this->wReservedMG[2]			  * n1); //ZHBA 2010-5-10
        	Result.wReservedMG[3]			  = (this->wReservedMG[3]			  * n1); //ZHBA 2010-5-10 
        	Result.wReservedMG[4]			  = (this->wReservedMG[4]			  * n1);
        	Result.wReservedMG[5]			  = (this->wReservedMG[5]			  * n1);
        	Result.wReservedMG[6]			  = (this->wReservedMG[6]			  * n1);
        	Result.wReservedMG[7]			  = (this->wReservedMG[7]			  * n1);
        	Result.wReservedMG[8]			  = (this->wReservedMG[8]			  * n1);
        	Result.wReservedMG[9]			  = (this->wReservedMG[9]			  * n1);
        	Result.wReservedMG[10]			  = (this->wReservedMG[10]			  * n1);
        	Result.wReservedMG[11]			  = (this->wReservedMG[11]			  * n1);
        	Result.wReservedMG[12]			  = (this->wReservedMG[12]			  * n1);
        	Result.wReservedMG[13]			  = (this->wReservedMG[13]			  * n1);
        	Result.wReservedMG[14]			  = (this->wReservedMG[14]			  * n1);
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator/(double d1)
        {
        	tagtmOIL_MONI2EX Result;
        	Result.tmlCycle 						= (DWORD)(this->tmlCycle              / d1);
        	Result.tmlInject 						= (DWORD)(this->tmlInject             / d1);
        	Result.tmlTurnToHold				= (DWORD)(this->tmlTurnToHold        	/ d1);
        	Result.tmlCharge						= (DWORD)(this->tmlCharge             / d1);
        	Result.adPosi_InjectStart		= (DWORD)(this->adPosi_InjectStart    / d1);
        	Result.adPosi_TurnToHold		= (DWORD)(this->adPosi_TurnToHold     / d1);
        	Result.adPosi_InjectEnd			= (DWORD)(this->adPosi_InjectEnd      / d1);
        	Result.adPosi_InjectCushion	= (DWORD)(this->adPosi_InjectCushion  / d1);
        	Result.adPosi_InjectPeak		= (DWORD)(this->adPosi_InjectPeak     / d1);
        	Result.daPres_TurnToHold		= (DWORD)(this->daPres_TurnToHold     / d1);
        	Result.cnChargeRPM					= (DWORD)(this->cnChargeRPM           / d1);
        	Result.tmSuckBack						= (DWORD)(this->tmSuckBack            / d1);
        	Result.tmEject							= (DWORD)(this->tmEject               / d1);
        	Result.tmEjectAdvance				= (DWORD)(this->tmEjectAdvance        / d1);
        	Result.tmEjectReturn				= (DWORD)(this->tmEjectReturn         / d1);
        	Result.daPres_InjectPeak		= (DWORD)(this->daPres_InjectPeak     / d1);
        	Result.daPres_ChargePeak		= (DWORD)(this->daPres_ChargePeak     / d1);
        	Result.tmlInjectB						= (DWORD)(this->tmlInjectB            / d1);
        	Result.tmlChargeB						= (DWORD)(this->tmlChargeB            / d1);
        	Result.adPosi_InjectStartB	= (DWORD)(this->adPosi_InjectStartB   / d1);
        	Result.adPosi_TurnToHoldB		= (DWORD)(this->adPosi_TurnToHoldB    / d1);
        	Result.adPosi_InjectCushionB= (DWORD)(this->adPosi_InjectCushionB / d1);
        	Result.daPres_InjecterB			= (DWORD)(this->daPres_InjecterB      / d1);
        	Result.daPres_ChargeBackB		= (DWORD)(this->daPres_ChargeBackB    / d1);
        	Result.tmTake								= (DWORD)(this->tmTake    			  		/ d1);
        	Result.adPosi_OpenMoldEnd		= (DWORD)(this->adPosi_OpenMoldEnd	  / d1);
        	Result.daPres_TurnToHoldB		= (DWORD)(this->daPres_TurnToHoldB		/ d1);
        	Result.tmlTurnToHoldB				= (DWORD)(this->tmlTurnToHoldB				/ d1);
        	Result.adPosi_InjectHoldEndB= (DWORD)(this->adPosi_InjectHoldEndB / d1);
        	Result.daPres_InjectMiddleB	= (DWORD)(this->daPres_InjectMiddleB	/ d1);
        	Result.daPres_ChargeMiddleB	= (DWORD)(this->daPres_ChargeMiddleB	/ d1);
        	Result.InjectMaxSpeed				= (DWORD)(this->InjectMaxSpeed				/ d1);
        	Result.InjectMaxSpeedB			= (DWORD)(this->InjectMaxSpeedB			  / d1);
        	Result.MuCellAveragePercent	= (DWORD)(this->MuCellAveragePercent  / d1);
        	Result.MuCellMaxPercent			= (DWORD)(this->MuCellMaxPercent  		/ d1);
        	Result.MuCellMinPercent			= (DWORD)(this->MuCellMinPercent  		/ d1);
        	Result.MuCellSetPercent			= (DWORD)(this->MuCellSetPercent  		/ d1);
//        	Result.MuCellShotWeight			= (DWORD)(this->MuCellShotWeight  		/ d1);
//        	Result.MuCellBackPress 			= (DWORD)(this->MuCellBackPress   		/ d1);
        	Result.wReservedMG[0] 			= (DWORD)(this->wReservedMG[0]			  / d1); //ZHBA 2010-5-10
        	Result.wReservedMG[1] 			= (DWORD)(this->wReservedMG[1]			  / d1); //ZHBA 2010-5-10
        	Result.wReservedMG[2] 			= (DWORD)(this->wReservedMG[2]			  / d1); //ZHBA 2010-5-10
        	Result.wReservedMG[3] 			= (DWORD)(this->wReservedMG[3]			  / d1); //ZHBA 2010-5-10 
        	Result.wReservedMG[4] 			= (DWORD)(this->wReservedMG[4]			  / d1);
        	Result.wReservedMG[5] 			= (DWORD)(this->wReservedMG[5]			  / d1);
        	Result.wReservedMG[6] 			= (DWORD)(this->wReservedMG[6]			  / d1);
        	Result.wReservedMG[7] 			= (DWORD)(this->wReservedMG[7]			  / d1);
        	Result.wReservedMG[8] 			= (DWORD)(this->wReservedMG[8]			  / d1);
        	Result.wReservedMG[9] 			= (DWORD)(this->wReservedMG[9]			  / d1);
        	Result.wReservedMG[10] 			= (DWORD)(this->wReservedMG[10]			  / d1);
        	Result.wReservedMG[11] 			= (DWORD)(this->wReservedMG[11]			  / d1);
        	Result.wReservedMG[12] 			= (DWORD)(this->wReservedMG[12]			  / d1);
        	Result.wReservedMG[13] 			= (DWORD)(this->wReservedMG[13]			  / d1);
        	Result.wReservedMG[14] 			= (DWORD)(this->wReservedMG[14]			  / d1);
        	return Result;
        }
        
        tagtmOIL_MONI2EX operator=(tagtmOIL_MONI2 moni1)
        {
        		this->dwShotCount						= (DWORD)(moni1.dwShotCount           );
            this->tmlCycle 				    	= (DWORD)(moni1.tmlCycle              );
            this->tmlInject 						= (DWORD)(moni1.tmlInject             );
            this->tmlTurnToHold			    = (DWORD)(moni1.tmlTurnToHold		  		);
            this->tmlCharge				    	= (DWORD)(moni1.tmlCharge             );
            this->adPosi_InjectStart	  = (DWORD)(moni1.adPosi_InjectStart    );
            this->adPosi_TurnToHold		  = (DWORD)(moni1.adPosi_TurnToHold     );
            this->adPosi_InjectEnd			= (DWORD)(moni1.adPosi_InjectEnd      );
            this->adPosi_InjectCushion	= (DWORD)(moni1.adPosi_InjectCushion  );
            this->adPosi_InjectPeak		  = (DWORD)(moni1.adPosi_InjectPeak     );
            this->daPres_TurnToHold		  = (DWORD)(moni1.daPres_TurnToHold     );
            this->cnChargeRPM						= (DWORD)(moni1.cnChargeRPM           );
            this->tmSuckBack						= (DWORD)(moni1.tmSuckBack            );
            this->tmEject								= (DWORD)(moni1.tmEject               );
            this->tmEjectAdvance				= (DWORD)(moni1.tmEjectAdvance        );
            this->tmEjectReturn			    = (DWORD)(moni1.tmEjectReturn         );
            this->daPres_InjectPeak		  = (DWORD)(moni1.daPres_InjectPeak     );
            this->daPres_ChargePeak		  = (DWORD)(moni1.daPres_ChargePeak     );
            this->tmlInjectB						= (DWORD)(moni1.tmlInjectB            );
            this->tmlChargeB						= (DWORD)(moni1.tmlChargeB            );
            this->adPosi_InjectStartB		= (DWORD)(moni1.adPosi_InjectStartB   );
            this->adPosi_TurnToHoldB		= (DWORD)(moni1.adPosi_TurnToHoldB    );
            this->adPosi_InjectCushionB = (DWORD)(moni1.adPosi_InjectCushionB );
            this->daPres_InjecterB			= (DWORD)(moni1.daPres_InjecterB      );
            this->daPres_ChargeBackB		= (DWORD)(moni1.daPres_ChargeBackB    );
            this->tmTake								= (DWORD)(moni1.tmTake    			  		);
            this->adPosi_OpenMoldEnd		= (DWORD)(moni1.adPosi_OpenMoldEnd	  );
            this->daPres_TurnToHoldB		= (DWORD)(moni1.daPres_TurnToHoldB		);
            this->tmlTurnToHoldB				= (DWORD)(moni1.tmlTurnToHoldB				);
            this->adPosi_InjectHoldEndB = (DWORD)(moni1.adPosi_InjectHoldEndB );
            this->daPres_InjectMiddleB	= (DWORD)(moni1.daPres_InjectMiddleB	);
            this->daPres_ChargeMiddleB	= (DWORD)(moni1.daPres_ChargeMiddleB	);
            this->InjectMaxSpeed				= (DWORD)(moni1.InjectMaxSpeed				);
            this->InjectMaxSpeedB			  = (DWORD)(moni1.InjectMaxSpeedB			  );
            this->MuCellAveragePercent  = (DWORD)(moni1.MuCellAveragePercent  );
            this->MuCellMaxPercent  		= (DWORD)(moni1.MuCellMaxPercent  );
            this->MuCellMinPercent  		= (DWORD)(moni1.MuCellMinPercent  );
            this->MuCellSetPercent  		= (DWORD)(moni1.MuCellSetPercent  );
//            this->MuCellShotWeight  		= (DWORD)(moni1.MuCellShotWeight  );
//            this->MuCellBackPress   		= (DWORD)(moni1.MuCellBackPress   );
          this->wReservedMG[0]			  = (DWORD)(moni1.wReservedMG[0]			  ); //ZHBA 2010-5-10
          this->wReservedMG[1]			  = (DWORD)(moni1.wReservedMG[1]			  ); //ZHBA 2010-5-10
          this->wReservedMG[2]			  = (DWORD)(moni1.wReservedMG[2]			  ); //ZHBA 2010-5-10
          this->wReservedMG[3]			  = (DWORD)(moni1.wReservedMG[3]			  ); //ZHBA 2010-5-10 
          this->wReservedMG[4]			  = (DWORD)(moni1.wReservedMG[4]			  ); 
          this->wReservedMG[5]			  = (DWORD)(moni1.wReservedMG[5]			  ); 
          this->wReservedMG[6]			  = (DWORD)(moni1.wReservedMG[6]			  ); 
          this->wReservedMG[7]			  = (DWORD)(moni1.wReservedMG[7]			  ); 
          this->wReservedMG[8]			  = (DWORD)(moni1.wReservedMG[8]			  ); 
          this->wReservedMG[9]			  = (DWORD)(moni1.wReservedMG[9]			  ); 
          this->wReservedMG[10]			  = (DWORD)(moni1.wReservedMG[10]			  ); 
          this->wReservedMG[11]			  = (DWORD)(moni1.wReservedMG[11]			  ); 
          this->wReservedMG[12]			  = (DWORD)(moni1.wReservedMG[12]			  ); 
          this->wReservedMG[13]			  = (DWORD)(moni1.wReservedMG[13]			  ); 
          this->wReservedMG[14]			  = (DWORD)(moni1.wReservedMG[14]			  ); 
            return  *this;
        }
        
        tagtmOIL_MONI2 Covert(tagtmOIL_MONI2EX moni1)
        {
        
            tagtmOIL_MONI2 Result;                                                        
            Result.tmlCycle 						= (_TIMEL)(moni1.tmlCycle              );
            Result.tmlInject 						= (_TIMEL)(moni1.tmlInject             );
            Result.tmlTurnToHold				= (_TIMEL)(moni1.tmlTurnToHold         );
            Result.tmlCharge						= (_TIMEL)(moni1.tmlCharge             );
            Result.adPosi_InjectStart		= (_COUNTL)(moni1.adPosi_InjectStart    );
            Result.adPosi_TurnToHold		= (_COUNTL)(moni1.adPosi_TurnToHold     );
            Result.adPosi_InjectEnd			= (_COUNTL)(moni1.adPosi_InjectEnd      );
            Result.adPosi_InjectCushion	= (_COUNTL)(moni1.adPosi_InjectCushion  );
            Result.adPosi_InjectPeak		= (_COUNTL)(moni1.adPosi_InjectPeak     );
            Result.daPres_TurnToHold		= (_DA   )(moni1.daPres_TurnToHold     );
            Result.cnChargeRPM					= (_COUNT)(moni1.cnChargeRPM           );
            Result.tmSuckBack						= (_TIMER)(moni1.tmSuckBack            );
            Result.tmEject							= (_TIMER)(moni1.tmEject               );
            Result.tmEjectAdvance				= (_TIMER)(moni1.tmEjectAdvance        );
            Result.tmEjectReturn				= (_TIMER)(moni1.tmEjectReturn         );
            Result.daPres_InjectPeak		= (_DA   )(moni1.daPres_InjectPeak     );
            Result.daPres_ChargePeak		= (_DA   )(moni1.daPres_ChargePeak     );
            Result.tmlInjectB						= (_TIMEL)(moni1.tmlInjectB            );
            Result.tmlChargeB						= (_TIMEL)(moni1.tmlChargeB            );
            Result.adPosi_InjectStartB	= (_COUNTL)(moni1.adPosi_InjectStartB   );
            Result.adPosi_TurnToHoldB		= (_COUNTL)(moni1.adPosi_TurnToHoldB    );
            Result.adPosi_InjectCushionB= (_COUNTL)(moni1.adPosi_InjectCushionB );
            Result.daPres_InjecterB			= (_DA   )(moni1.daPres_InjecterB      );
            Result.daPres_ChargeBackB		= (_DA   )(moni1.daPres_ChargeBackB    );
            Result.tmTake								= (_TIMEL)(moni1.tmTake    			   		 );
            Result.adPosi_OpenMoldEnd		= (_COUNTL)(moni1.adPosi_OpenMoldEnd	   );
            Result.daPres_TurnToHoldB		= (_AD   )(moni1.daPres_TurnToHoldB		 );
            Result.tmlTurnToHoldB				= (_TIMEL)(moni1.tmlTurnToHoldB				 );
            Result.adPosi_InjectHoldEndB= (_COUNTL)(moni1.adPosi_InjectHoldEndB );
            Result.daPres_InjectMiddleB	= (_AD   )(moni1.daPres_InjectMiddleB	 );
            Result.daPres_ChargeMiddleB	= (_AD   )(moni1.daPres_ChargeMiddleB	 );
            Result.InjectMaxSpeed				= (_AD   )(moni1.InjectMaxSpeed				 );
            Result.InjectMaxSpeedB			= (_AD   )(moni1.InjectMaxSpeedB			 );
            Result.MuCellAveragePercent	= (_AD   )(moni1.MuCellAveragePercent	 );
            Result.MuCellMaxPercent			= (_AD   )(moni1.MuCellMaxPercent	 );
            Result.MuCellMinPercent			= (_AD   )(moni1.MuCellMinPercent	 );
            Result.MuCellSetPercent			= (_AD   )(moni1.MuCellSetPercent	 );
//            Result.MuCellShotWeight			= (_AD   )(moni1.MuCellShotWeight	 );
//            Result.MuCellBackPress 			= (_AD   )(moni1.MuCellBackPress 	 );
          Result.wReservedMG[0]  			= (_AD   )(moni1.wReservedMG[0] 			 ); //ZHBA 2010-5-10
          Result.wReservedMG[1]  			= (_AD   )(moni1.wReservedMG[1] 			 ); //ZHBA 2010-5-10
          Result.wReservedMG[2]  			= (_AD   )(moni1.wReservedMG[2] 			 ); //ZHBA 2010-5-10
          Result.wReservedMG[3]  			= (_AD   )(moni1.wReservedMG[3] 			 ); //ZHBA 2010-5-10 
          Result.wReservedMG[4]  			= (_AD   )(moni1.wReservedMG[4] 			 ); 
          Result.wReservedMG[5]  			= (_AD   )(moni1.wReservedMG[5] 			 ); 
          Result.wReservedMG[6]  			= (_AD   )(moni1.wReservedMG[6] 			 ); 
          Result.wReservedMG[7]  			= (_AD   )(moni1.wReservedMG[7] 			 ); 
          Result.wReservedMG[8]  			= (_AD   )(moni1.wReservedMG[8] 			 ); 
          Result.wReservedMG[9]  			= (_AD   )(moni1.wReservedMG[9] 			 ); 
          Result.wReservedMG[10]  			= (_AD   )(moni1.wReservedMG[10] 			 ); 
          Result.wReservedMG[11]  			= (_AD   )(moni1.wReservedMG[11] 			 ); 
          Result.wReservedMG[12]  			= (_AD   )(moni1.wReservedMG[12] 			 ); 
          Result.wReservedMG[13]  			= (_AD   )(moni1.wReservedMG[13] 			 ); 
          Result.wReservedMG[14]  			= (_AD   )(moni1.wReservedMG[14] 			 ); 
            return Result;
        }
        
   
        void	OilSqrt()
        {        
        	this->tmlCycle              = (DWORD)sqrt((double)this->tmlCycle             	);
        	this->tmlInject             = (DWORD)sqrt((double)this->tmlInject            	);
        	this->tmlTurnToHold         = (DWORD)sqrt((double)this->tmlTurnToHold        	);
        	this->tmlCharge             = (DWORD)sqrt((double)this->tmlCharge            	);
        	this->adPosi_InjectStart    = (DWORD)sqrt((double)this->adPosi_InjectStart   	);
        	this->adPosi_TurnToHold     = (DWORD)sqrt((double)this->adPosi_TurnToHold    	);
        	this->adPosi_InjectEnd      = (DWORD)sqrt((double)this->adPosi_InjectEnd     	);
        	this->adPosi_InjectCushion  = (DWORD)sqrt((double)this->adPosi_InjectCushion 	);
        	this->adPosi_InjectPeak     = (DWORD)sqrt((double)this->adPosi_InjectPeak    	);
        	this->daPres_TurnToHold     = (DWORD)sqrt((double)this->daPres_TurnToHold    	);
        	this->cnChargeRPM           = (DWORD)sqrt((double)this->cnChargeRPM          	);
        	this->tmSuckBack            = (DWORD)sqrt((double)this->tmSuckBack           	);
        	this->tmEject               = (DWORD)sqrt((double)this->tmEject              	);
        	this->tmEjectAdvance        = (DWORD)sqrt((double)this->tmEjectAdvance       	);
        	this->tmEjectReturn         = (DWORD)sqrt((double)this->tmEjectReturn        	);
        	this->daPres_InjectPeak     = (DWORD)sqrt((double)this->daPres_InjectPeak    	);
        	this->daPres_ChargePeak     = (DWORD)sqrt((double)this->daPres_ChargePeak    	);
        	this->tmlInjectB            = (DWORD)sqrt((double)this->tmlInjectB           	);
        	this->tmlChargeB            = (DWORD)sqrt((double)this->tmlChargeB           	);
        	this->adPosi_InjectStartB   = (DWORD)sqrt((double)this->adPosi_InjectStartB  	);
        	this->adPosi_TurnToHoldB    = (DWORD)sqrt((double)this->adPosi_TurnToHoldB   	);
        	this->adPosi_InjectCushionB = (DWORD)sqrt((double)this->adPosi_InjectCushionB	);
        	this->daPres_InjecterB      = (DWORD)sqrt((double)this->daPres_InjecterB     	);
        	this->daPres_ChargeBackB    = (DWORD)sqrt((double)this->daPres_ChargeBackB   	);
        	this->tmTake    						= (DWORD)sqrt((double)this->tmTake   			 			 	);
        	this->adPosi_OpenMoldEnd		= (DWORD)sqrt((double)this->adPosi_OpenMoldEnd	 	);
        	this->daPres_TurnToHoldB		= (DWORD)sqrt((double)this->daPres_TurnToHoldB		);
        	this->tmlTurnToHoldB				= (DWORD)sqrt((double)this->tmlTurnToHoldB				);
        	this->adPosi_InjectHoldEndB = (DWORD)sqrt((double)this->adPosi_InjectHoldEndB );
        	this->daPres_InjectMiddleB	= (DWORD)sqrt((double)this->daPres_InjectMiddleB	);
        	this->daPres_ChargeMiddleB	= (DWORD)sqrt((double)this->daPres_ChargeMiddleB	);
        	this->InjectMaxSpeed				= (DWORD)sqrt((double)this->InjectMaxSpeed				);
        	this->InjectMaxSpeedB			  = (DWORD)sqrt((double)this->InjectMaxSpeedB			  );
        	this->MuCellAveragePercent  = (DWORD)sqrt((double)this->MuCellAveragePercent  );
        	this->MuCellMaxPercent  		= (DWORD)sqrt((double)this->MuCellMaxPercent  );
        	this->MuCellMinPercent  		= (DWORD)sqrt((double)this->MuCellMinPercent  );
        	this->MuCellSetPercent  		= (DWORD)sqrt((double)this->MuCellSetPercent  );
//        	this->MuCellShotWeight  		= (DWORD)sqrt((double)this->MuCellShotWeight  );
//        	this->MuCellBackPress   		= (DWORD)sqrt((double)this->MuCellBackPress   );
        	this->wReservedMG[0]			  = (DWORD)sqrt((double)this->wReservedMG[0]); //ZHBA 2010-5-10
        	this->wReservedMG[1]			  = (DWORD)sqrt((double)this->wReservedMG[1]); //ZHBA 2010-5-10
        	this->wReservedMG[2]			  = (DWORD)sqrt((double)this->wReservedMG[2]); //ZHBA 2010-5-10
        	this->wReservedMG[3]			  = (DWORD)sqrt((double)this->wReservedMG[3]); //ZHBA 2010-5-10 
        	this->wReservedMG[4]			  = (DWORD)sqrt((double)this->wReservedMG[4]); 
        	this->wReservedMG[5]			  = (DWORD)sqrt((double)this->wReservedMG[5]); 
        	this->wReservedMG[6]			  = (DWORD)sqrt((double)this->wReservedMG[6]); 
        	this->wReservedMG[7]			  = (DWORD)sqrt((double)this->wReservedMG[7]); 
        	this->wReservedMG[8]			  = (DWORD)sqrt((double)this->wReservedMG[8]); 
        	this->wReservedMG[9]			  = (DWORD)sqrt((double)this->wReservedMG[9]); 
        	this->wReservedMG[10]			  = (DWORD)sqrt((double)this->wReservedMG[10]); 
        	this->wReservedMG[11]			  = (DWORD)sqrt((double)this->wReservedMG[11]); 
        	this->wReservedMG[12]			  = (DWORD)sqrt((double)this->wReservedMG[12]); 
        	this->wReservedMG[13]			  = (DWORD)sqrt((double)this->wReservedMG[13]); 
        	this->wReservedMG[14]			  = (DWORD)sqrt((double)this->wReservedMG[14]); 
        	
        }
    } tmOIL_MONI2EX;

typedef struct	tagtmBlockInfo
{
	int			nBlockIndex;
	tmFileIDType FileID;
}	tmBlockInfo;

typedef struct tagHourData
{
	DWORD			dwStartData;
	DWORD			dwEndData;
}HOURDATA;

typedef struct tagDayData
{
	HOURDATA hourdata[25];
}DAYDATA;

typedef struct tagAmmeterData
{
	struct {
	WORD		wCount;
	WORD		wIndex;
	};
	struct {
		BYTE		bMonth;
		BYTE		bDay;
		DAYDATA		daydata;
	}__attribute__((packed, aligned(2)));
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AMMETERDATA;
#else 
#ifdef	D_X86
AMMETERDATA;
#endif
#endif

typedef struct tagtmPPHData
{
	struct {
	WORD		wCount;
	WORD		wIndex;
	};
	struct {
		BYTE		bMonth;
		BYTE		bDay;
		DWORD		dwHour[25];
	}__attribute__((packed, aligned(2)));
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TMPPHDATA;
#else 
#ifdef	D_X86
TMPPHDATA;
#endif
#endif

typedef struct tagCOM2AMMETERINFO
{
	BYTE 	address[AMMETER_FRAME_ADDR_LENGTH];
	WORD 	wCTPara;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	COM2AMMETERINFO;
#else 
#ifdef	D_X86
COM2AMMETERINFO;
#endif
#endif


typedef struct tagCOM2AMMETERCONFIG
{
	WORD 		wCount;
	COM2AMMETERINFO AmmeterInfo[AMMETER_MAX_NUM_COMMON];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	COM2AMMETERCONFIG;
#else 
#ifdef	D_X86
COM2AMMETERCONFIG;
#endif
#endif

typedef struct tagN2CAMMETERINFO
{
	WORD 	wNode;
	WORD 	wCTPara;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	N2CAMMETERINFO;
#else 
#ifdef	D_X86
N2CAMMETERINFO;
#endif
#endif


typedef struct tagN2CAMMETERCONFIG
{
	WORD 		wCount;
	N2CAMMETERINFO AmmeterInfo[AMMETER_MAX_NUM_COMMON];
	WORD		N2CVoltage[2][9];			//[2]0:MaxV 1:MinV; [9]Amt1:A,B,C(V);Amt2:A,B,C(V);Amt3:A,B,C(V)
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	N2CAMMETERCONFIG;
#else 
#ifdef	D_X86
N2CAMMETERCONFIG;
#endif
#endif

typedef struct tagHYB_VALUE
{
	DWORD	dwID;				/* data ID */
	BYTE	ucDataType;			/* data type */
	BYTE	ucState;			/* data state */
	union						/* data value */
	{
		char 			acData[8];
		unsigned char 	ucData;
		unsigned short	wData;
		unsigned long	dwData;
		signed char 	cData;
		signed short	nData;
		signed long		lData;
		float 			fData;
		double 			dData;
	};
} 
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	HYB_VALUE;
#else 
#ifdef	D_X86
HYB_VALUE;
#endif
#endif

typedef	struct tagHYB5MOLDSETDATA
{
	DB_MOLDHDR 	MoldSetHead;
	DWORD		dwTotal;
	HYB_VALUE	pValue[];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	HYB5MOLDSETDATA;
#else 
#ifdef	D_X86
HYB5MOLDSETDATA;
#endif
#endif

#endif
