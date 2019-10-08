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
#ifndef	D_LIBCOMM
#define	D_LIBCOMM


//=================================================================
//taskhost
//=================================================================
#include "../oil54struct.h"


//#define		COMM_GETCOMMAND

#define		COMM_UPDATEDEVICE_DSP28				0
#define		COMM_UPDATEDEVICE_DSP54				1
#define		COMM_UPDATEDEVICE_DSP54TRANSMIT		2

#define		AUSTONE_MINDATAID		50000

#define		BLOCK_DB_DSP28MACHINE1		160
#define		BLOCK_DB_DSP28MACHINE2		161
#define		BLOCK_DB_DSP28MACHINE3		162
#define		BLOCK_DB_DSP28MACHINE4		163
#define		BLOCK_DB_DSP28MACHINE5		164
#define		BLOCK_DB_DSP28MACHINE6		165

#define		BLOCK_DB_AUSTON_ACTUAL		183
#define		BLOCK_DB_AUSTON_PARA		184
#define		BLOCK_DB_AUSTON_DRIVER		189
#define		BLOCK_DB_AUSTON_DRIVER_S	190
#define		BLOCK_DB_AUSTON_MOTOR		191
#define		BLOCK_DB_AUSTON_MOTOR_S		192


#define     COMM_S_REACHSHOTCOUNT       0x0001
#define     COMM_S_REACHPACKCOUNT       0x0002
#define     COMM_S_POSNSETERROR         0x0004      //Check of Posi_end    Anny052000
#define     COMM_S_COM2MOLDTEMPERROR    0x0008      //Check of Posi_end    Anny052000
#define     COMM_S_INETMSGALARM2S       0x0010      //<<LEO20060724 add
#define     COMM_S_INETMSGLIGHT2S       0x0020      //<<LEO20060724 add 

#define     COMM_S_MOTORPHASEERR        0x0080 

#define     COMM_ECHO_SUCCESS           0x0000
#define     COMM_ECHO_FAILURE           0x0001

#define     COMM_STATE_HALT             0x00
#define     COMM_STATE_RUN              0x01
#define     COMM_STATE_MACHINE          0x02
#define     COMM_STATE_RESET            0x10
#define     COMM_STATE_CONFIG           0x11
#define     COMM_STATE_ECHO             0x12
#define     COMM_STATE_WAIT00           0x01
#define     COMM_STATE_WAIT01           0x02
#define     COMM_STATE_WAIT02           0x04
#define     COMM_STATE_WAIT03           0x08
#define     COMM_STATE_WAIT04           0x10
#define     COMM_STATE_WAITB01          COMM_STATE_WAIT03    //Chuntzu 2004/11/18 add
#define     COMM_STATE_WAITB02          COMM_STATE_WAIT04
#define     COMM_STATE_WAIT05           0x20
#define     COMM_STATE_WAIT06           0x40
#define     COMM_STATE_WAIT07           0x80
#define     COMM_SIZEDIAGWM             8
#define     MAX_COMMAND_KEEP            100      // St0527'03 for Command send

#define     tmMAX_POINT_PB				192
#define     tmMAX_POINT_PC				192
#define     tmASSIGN_MASK 				0x01FF
#define     tmASSIGN_NEWIO				0x1000
#define     tmASSIGN_OLDIO				0x2000
#define     tmASSIGN_NODEF				0x8000

#define     tmMAX_GROUP_PC				3
#define     tmMAX_OP_STEP				5       // 3*16 st092099
#define     tmMAX_OP_SUBSTEP			2       // 2*16

#define     tmMAX_DIAGRM				16
#define     tmMAX_DIAGRI				1

#define		MAX_TRANSMIT_KEEP			100
#define		COMM_MAXTRANSIMT			512//160//512
#define		COMM_MAXTRANSIMT_RS232		160//160//512
#define		COMM_MAXTRANSIMT_NET		512//160//512

const		int	AUSTONE_CMD_SAVEDATA 	=  0x00010004L; //0x00000005;
const		int	AUSTONE_CMD_LOADDATA	= 0x00000006;
const		int	AUSTONE_CMD_AUTOPID 	= 0x00000009;
const		int	AUSTONE_CMD_MOTORZERO 	= 0x00000004;
const		int	AUSTONE_GETCMD_SAVEDATA = 0x80010004;//0x80000005;
const		int	AUSTONE_GETCMD_LOADDATA	= 0x80000006;
const		int	AUSTONE_GETCMD_AUTOPID 	= 0x80000009;
const		int	AUSTONE_GETCMD_MOTORZERO= 0x80000004;


#define		MUCELL_COMM_TASKCODE_SEND_B			0x42	//Set SCF Percent Set Point
#define		MUCELL_COMM_TASKCODE_SEND_C			0x43	//Hydraulic Calibration 	2012-3-13        //mucell_2 Toggle Delivery Pressure Control 
#define		MUCELL_COMM_TASKCODE_SEND_D			0x44  //Set Automatic Delivery Pressure Drop Setpoint	//JOYCE2012-6-29 for mucell_2
#define		MUCELL_COMM_TASKCODE_SEND_E			0x45	//Set Shot Weight
#define		MUCELL_COMM_TASKCODE_SEND_F			0x46  //Set SCF Flowrate Setpoint	//JOYCE2012-6-29 for mucell_2
#define		MUCELL_COMM_TASKCODE_SEND_G			0x47	//Set Specific Back Pressure
#define		MUCELL_COMM_TASKCODE_SEND_H			0x48	//Clear Faults
#define		MUCELL_COMM_TASKCODE_SEND_J			0x4A	//Set Screw Rotation Time
#define		MUCELL_COMM_TASKCODE_SEND_L			0x4C	//Start Learn Mode
#define		MUCELL_COMM_TASKCODE_SEND_M			0x4D  //Set Manual Delivery Pressure Setpoint	//JOYCE2012-6-29 for mucell_2
#define		MUCELL_COMM_TASKCODE_SEND_P			0x50	//Injector Purge 
#define		MUCELL_COMM_TASKCODE_SEND_R			0x52	//Safety Relay Re
#define		MUCELL_COMM_TASKCODE_SEND_S			0x53	//Request System Status
#define		MUCELL_COMM_TASKCODE_SEND_T			0x54	//Toggle Run/Stop
#define		MUCELL_COMM_TASKCODE_SEND_U			0x55  //Set Units	//JOYCE2012-6-29 for mucell_2

#define		MUCELL_COMM_TASKCODE_REV_B			0x62
#define		MUCELL_COMM_TASKCODE_REV_C			0x63
#define		MUCELL_COMM_TASKCODE_REV_D			0x64
#define		MUCELL_COMM_TASKCODE_REV_E			0x65
#define		MUCELL_COMM_TASKCODE_REV_F			0x66
#define		MUCELL_COMM_TASKCODE_REV_G			0x67
#define		MUCELL_COMM_TASKCODE_REV_H			0x68
#define		MUCELL_COMM_TASKCODE_REV_J			0x6A
#define		MUCELL_COMM_TASKCODE_REV_L			0x6C
#define		MUCELL_COMM_TASKCODE_REV_M			0x6D
#define		MUCELL_COMM_TASKCODE_REV_P			0x70
#define		MUCELL_COMM_TASKCODE_REV_R			0x72
#define		MUCELL_COMM_TASKCODE_REV_S			0x73
#define		MUCELL_COMM_TASKCODE_REV_T			0x74
#define		MUCELL_COMM_TASKCODE_REV_U			0x75

#define		AUSTONE_PARAMDATA_PATH			"/usr/Data/Aus_Driver_Para.cdb"

#define     COMM_GETAIRPCPOINT			0x6600
#define     COMM_GETAIRPCDATA				0x6601
#define     COMM_BOOTTIMECNTDOWN  	0x6602

enum CMDSTATE {
            CMD_NONE,
            CMD_SEND,
            CMD_ECHO
            };       

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

typedef struct tagTLED
{
    long    lCount;
    int     nIndex;
} TLED;
    
typedef struct tagCONTROLINDEX
{
    WORD    wIndex;
    WORD    wValue;
} CONTROLINDEX;
    
typedef struct tagCMDDATA
{
    WORD        wCmd;
    WORD        wIndex;
    WORD        wValue;
    WORD        wEcho;
} CMDDATA;

typedef struct tagCMDNETDATA
{
    WORD        wCmd;
    WORD        wValue;
} CMDNETDATA;

typedef struct tagCMDQUEUE
{
    WORD    wInput;
    WORD    wOutput;
    WORD    wQueueEnd;
    BOOL    bQueueFull;
    WORD    CmdQueue  [MAX_COMMAND_KEEP];    //Steven 2003/12/12 add
    WORD    IndexQueue[MAX_COMMAND_KEEP];
    WORD    ValueQueue[MAX_COMMAND_KEEP];
} CMDQUEUE;
/*
typedef struct tagMONIMAXMIN
    {
    WORD    flAutoAlarmStatus;
    tmDB_MONI moniMax;
    tmDB_MONI moniMin;
    } MONIMAXMIN;

typedef struct tagMONIMAXMINB
    {
    DB_MONIB moniMaxB;
    DB_MONIB moniMinB;
    } MONIMAXMINB;
*/
typedef struct tagDIAGRMRI
{
    WORD    wAddressROM;
    DWORD   dwAddressRAM;
    WORD    wAddressRI;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DIAGRMRI;
#else 
#ifdef	D_X86
DIAGRMRI;
#endif
#endif

typedef struct tagDIAGRM
{
    WORD    wAddressROM;
    DWORD   dwAddressRAM;// WORD   wAddressRAM;
} DIAGRM;

typedef struct tagDIAGWM
{
    DWORD   dwAddress;//WORD   wAddress;
    WORD    wData;
} DIAGWM;

typedef struct tagDIAGRI
{
    WORD    wAddress;
} 
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DIAGRI;
#else 
#ifdef	D_X86
DIAGRI;
#endif
#endif

//////////Andy 20090310
typedef struct tagtmDB_STATE
    {
    WORD        wOperationMode;                     // 510 操作狀態
    WORD        wOperationStep[5];       					  // 511 動作狀態1
    WORD        wOperationSubStep[2];  							// 514 動作級別狀態1
    WORD        wErrorFlag[16];       							// 516 警報狀態1
    WORD        wCmd;                       				// 51C Command type
    WORD        wIndex;                     				// 51D Index address
    WORD        wValue;                     				// 51E Update value       
    WORD        wEcho;                              // 51F Command Echo     //WeiJun 0609'03
    long	      tmlActing[4];                       // 525 動作時間1
    WORD        wInputData[12];                     // 529 輸入通訊1
    WORD        wOutputData[12];                    // 52C 輸出通訊1
    }
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_STATE;
#else 
#ifdef	D_X86
tmDB_STATE;
#endif
#endif

//////////Andy 20091208
typedef struct tagtmDB_STATE2
    {
    WORD        wOperationStep[4];       					  // 511 動作狀態1
    WORD        wOperationSubStep[2];  							// 514 動作級別狀態1
    WORD        wErrorFlag[8];       								// 516 警報狀態1
    WORD        wActStep[8];       					  			// 511 動作狀態1
    WORD        wOperationReserved[2];              // 52C 輸出通訊1
    }
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_STATE2;
#else 
#ifdef	D_X86
tmDB_STATE2;
#endif
#endif


typedef struct tagtmDB_BLOCK61
    {
    WORD        wDataCount;                   // 資料組數
    WORD        wIndexData[60];  					 		//  
    }
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_BLOCK61;
#else 
#ifdef	D_X86
tmDB_BLOCK61;
#endif
#endif

typedef struct tagDIAGWO
{
    WORD    wAddress;
    WORD    wData;
} DIAGWO;

typedef struct tagASSIGNIO
{
    WORD    wOldAddress;
    WORD    wNewAddress;
} ASSIGNIO;

typedef struct tagDB_OPSUM                     //St020101
{
    WORD        wOperationMode;                     // 510 操作狀態
    WORD        wOperationStep[5];      // 511 動作狀態1
    WORD        wOperationSubStep[tmMAX_OP_SUBSTEP];  // 514 動作級別狀態1
    long		tmlCycle;                            // 600 循環計時實際值
    long		tmlActing[2];                       // 525 動作時間1
    WORD         daOutput[4];                        // 540 DA輸出CH1
    WORD         adPosi_Relative[4];                 // 550 位置尺相對位置CH1
} 
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DB_OPSUM;
#else 
#ifdef	D_X86
DB_OPSUM;
#endif
#endif

typedef struct tagtmDB_CURVEDATA
{
    _FLAG       flCurveKind;                        // 曲線種類 0-Inj 1-Hold 2-Inj2 3-Hold2
    _COUNT      cnPoint;                            // 曲線點數
    _AD         nX1;                                // 曲線位置
    _DA         nY1;                                // 曲線壓力 
    _DA         nY2;                                // 曲線速度 
    _TIMEL      nX2;                                // 曲線時間
    WORD        wCurveReserved;                     // 保留
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_CURVEDATA;
#else 
#ifdef	D_X86
tmDB_CURVEDATA;
#endif
#endif

typedef	struct	tagtmNewCurveData
{
	_FLAG       flCurveKind; 						// 0-Inj 1-Hold 2-Inj2 3-Hold2
	_AD         nX1;                                // 曲線位置
    _DA         nY1;                                // 曲線壓力 
    _DA         nY2;                                // 曲線速度 
    _DA			nY3;
    _DA			nY4;
    _TIMEL      nX2;                                // 曲線時間
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmNEWCURVEDATA;
#else
#ifdef	D_X86
tmNEWCURVEDATA;
#endif
#endif

typedef struct tagtmDB_ETHCURVEDATA
{
	WORD		wID;
	WORD		wLength;
	_COUNT      cnPoint;
	tmNEWCURVEDATA	tmNewCurveData[10];				// CurveData
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_ETHCURVEDATA;
#else
#ifdef	D_X86
tmDB_ETHCURVEDATA;
#endif
#endif

typedef struct tagtmDB_AUSTONDRIVEINFO
{
	WORD	wOptState;			//驅動器操作狀態
	WORD	wAlarmNub;			//驅動器警報序號
	float	fMotorSpeed;		//馬達速度
	float	fMotorTorqueForce;	//馬達扭力
	float	fDriverFlow;		//驅動器電流
	float	fDriveBusVoltage;	//驅動器bus voltage
	float	fDriveTemper;		//驅動器溫度
	float	fMotorTemper;		//馬達溫度
	WORD	wAustonReserved[14];//Auston driver reserved 14 WORD
	WORD	wDsp28Reserved[100];//dsp28 reserved 100 word		
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_AUSTONDRIVEINFO;
#else 
#ifdef	D_X86
tmDB_AUSTONDRIVEINFO;
#endif
#endif

typedef struct tagtmDB_CHARTSTROKE0                   // For communication
{
    short       nX1;                                // Position
    short       nX2;                                // Time
    short       nY1;                                // Pressure
    short       nY2;                                // Speed
} tmDB_CHARTSTROKE0;

typedef struct tagtmDB_CHARTSTROKE1                   // For database
{
    short       nX;                                 // Position or Time
    short       nY1;                                // Pressure
    short       nY2;                                // Speed
    short       nX2;                                // Time    
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_CHARTSTROKE1;
#else 
#ifdef	D_X86
tmDB_CHARTSTROKE1;
#endif
#endif

typedef struct tagtmDB_CHART1
{
    short       nIndex;
    short       nX0;                                // Keep the first X point -- 02/17/98 by C.C.
    short       nX [tmMAX_STROKE];
    short       nY1[tmMAX_STROKE];
    short       nY2[tmMAX_STROKE];
} tmDB_CHART1;

typedef struct tagtmDB_DIAGRMRI
{
    WORD    awRMData[tmMAX_DIAGRM];
    WORD    awRIData[tmMAX_DIAGRI];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_DIAGRMRI;
#else 
#ifdef	D_X86
tmDB_DIAGRMRI;
#endif
#endif

typedef struct tagtmDB_DIAGRM
{
    WORD    awData[tmMAX_DIAGRM];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_DIAGRM;
#else 
#ifdef	D_X86
tmDB_DIAGRM;
#endif
#endif

typedef struct tagtmDB_DIAGRI
{
    WORD    awData[tmMAX_DIAGRI];
} 
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_DIAGRI;
#else 
#ifdef	D_X86
tmDB_DIAGRI;
#endif
#endif

typedef	struct tagtmDB_STATUS
{
	WORD	wID;
	WORD	wLen;
	WORD	wOperMode;
	WORD	wOperStep[5];
	WORD	wOperSubStep[2];
	WORD	wStepErrorData[16];
	WORD	wCmd;
	WORD	wIndex;
	WORD	wEcho;
	DWORD	dwActingTime[4];
	WORD	wInputData[16];
	WORD	wOutPutData[16];
	WORD	wActStep[5];
	WORD	wMotorState;
	WORD	wHeaterState;
	WORD	wShotState;
	WORD	wMoldTempHeaterState;
	WORD	wRsvOper[11];
	WORD	wRsvCofg[16];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmDB_STATUS;
#else 
#ifdef	D_X86
tmDB_STATUS;
#endif
#endif

typedef union tagCOMMRX
{
    BYTE            acBuffer[MAX_TOKEN_SIZE];
//    tmDB_CONFIG       config;
    CONTROLINDEX    controlindex;
    DB_OPSUM        OPSummary;                //St020101
    tmDB_CURVEDATA    curvedata;
    tmDB_STATE        state; //Andy 20090310 //ZHBA  2009-7-30 add
    tmDB_STATE2       state2; //Andy 20090310
/*    tmDB_STATE        state;
    tmDB_STATUS       status;
    tmDB_METER        meter;
    tmDB_TEMP         temp;
    
    tmDB_DAOUT        DAoutput;               // St1108'01  for D/A
    WORD            wShotCount;
    tmDB_MONI         moniCurrent;
    tmDB_MONI         moniLast;
	
    tmDB_MONIB        moniCurrentB;
    tmDB_MONIB        moniLastB;
    MONIMAXMINB     monimaxminB;
    tmDB_CHARTSTROKE0 chartstroke0;
	tmDB_COMMAND      command;*/
	tmDB_DIAGRM       diagrm;
    tmDB_DIAGRI       diagri;
    WORD            wResult;
} COMMRX;

typedef union tagCOMMTX
{
    BYTE            acBuffer[MAX_TOKEN_SIZE];
/*    tmDB_CONFIG       config;
    tmDB_MACHINE      machine;
    tmDB_MOLDSET      moldset;
    MONIMAXMIN      monimaxmin;
    tmDB_MACHINEB     machineB;
    tmDB_MOLDSETB     moldsetB;
    MONIMAXMINB     monimaxminB;
    tmZSET            zeroset;*/
    CONTROLINDEX    controlindex;
    CMDDATA         cmddata;                // St0522'03
//    tmDB_POWERMATCH   powermatch;             // St1108'01  for D/A 
    DIAGRM          diagrm;
    DIAGWM          diagwm;
    DIAGRI          diagri;
    DIAGWO          diagwo;
    ASSIGNIO        assignio;
    tmDB_BLOCK61		block61; 
    WORD            wStatus;

    WORD            wResult;
} COMMTX;


typedef struct tagCOMM485FRAMERX
{
	DWORD		dwLen;
	BYTE		bTaskCmd;
    COMMRX      commdata;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	COMM485FRAMERX;
#else 
#ifdef	D_X86
COMM485FRAMERX;
#endif
#endif

typedef struct tagCOMMFRAMERX
{
    WORD        wCmd;
    COMMRX      commdata;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	COMMFRAMERX;
#else 
#ifdef	D_X86
COMMFRAMERX;
#endif
#endif

typedef struct tagCOMMFRAMETX
{
    WORD        wCmd;
    COMMTX      commdata;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	COMMFRAMETX;
#else 
#ifdef	D_X86
COMMFRAMETX;
#endif
#endif

typedef struct tagETHNETFUNCTX
{
	WORD	wCmd;
	WORD	wID;
	WORD	wLength;
	BYTE    acData[1512];	
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	ETHNETFUNCTX;
#else 
#ifdef	D_X86
ETHNETFUNCTX;
#endif
#endif

typedef union tagETHNETRX
{
    BYTE            acBuffer[MAX_TOKEN_SIZE];
    CONTROLINDEX    controlindex;
    DB_OPSUM        OPSummary;
    tmDB_ETHCURVEDATA   curvedata;
    tmDB_STATUS       	status;			//James add 2010/4/21 for cycle time and step time
	tmDB_DIAGRMRI       diagrmri;
    //tmDB_DIAGRI       diagri;
    WORD            	wResult;
} ETHNETRX;

typedef union tagETHNETTX
{
    BYTE            acBuffer[MAX_TOKEN_SIZE];
    CONTROLINDEX    controlindex;
    CMDDATA         cmddata;
    CMDNETDATA		cmdnetdata;
    DIAGRMRI        diagrmri;
    DIAGWM          diagwm;
    //DIAGRI          diagri;
    DIAGWO          diagwo;
    ASSIGNIO        assignio;
    WORD            wStatus;
    WORD            wResult;
} ETHNETTX;

typedef struct tagETHNETFRAMERX
{
    WORD        wCmd;
    ETHNETRX    commdata;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	ETHNETFRAMERX;
#else 
#ifdef	D_X86
ETHNETFRAMERX;
#endif
#endif

typedef struct tagETHNETFRAMETX
{
    WORD        wCmd;
    ETHNETTX    commdata;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	ETHNETFRAMETX;
#else 
#ifdef	D_X86
ETHNETFRAMETX;
#endif
#endif

typedef 	struct	tagETHNETRESENDTX
{
	WORD			wFlag;				//0表示已經收到Echo， 否則沒有
	ETHNETFUNCTX	ethnetFuncTx;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	ETHNETRESENDTX;
#else 
#ifdef	D_X86
ETHNETRESENDTX;
#endif
#endif

typedef struct tagTRANSMITQUEUE
{
    WORD    wInput;
    WORD    wOutput;
    WORD    wQueueEnd;
    BOOL    bQueueFull;
    BYTE*    acBuffer[MAX_TRANSMIT_KEEP];
    //BYTE    acBuffer[MAX_TRANSMIT_KEEP][COMM_MAXTRANSIMT];
} TRANSMITQUEUE;

typedef	struct	tagTRANSMITDATA
{
	BYTE    acBuffer[COMM_MAXTRANSIMT];	
} TRANSMITDATA;

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

#endif
