

#ifndef D_OIL54STRUCT
#define	D_OIL54STRUCT

#include	"database.h"
#include	"cmdswitch.h"

//=================================================================
//rs232
//=================================================================
/*---------------------------------------------------------------------------+
|           RS232 device driver                                              |
+---------------------------------------------------------------------------*/
#define     MAX_COM             10			// 請不要>=10

#define     _COM1               0
#define     _COM2               1
#define     _COM3               2
#define     _COM4               3
#define		_COM5				4
#define		_COM6				5

#ifdef D_3354
#define		COM_PATH			"/dev/ttyO"
#else
#define		COM_PATH			"/dev/ttyS"	//路徑長度不要超過18個字符
#endif

//=======
//      New RS232 device service routine
//=======
#define     INT_COM_DSR         0x86
#define     COM_SIGNATURE       0xCC86
#define     COM_DEVICECODE      0x03    // Device code

//=======
//      RS232 function code information
//=======
#define     COM_OPEN            0x00    // Function 0 : Open
#define     COM_CLOSE           0x01    // Function 1 : Close
#define     COM_READ            0x02    // Function 2 : Read character
#define     COM_WRITE           0x03    // Function 3 : Write character
#define     COM_GETRXSTATE      0x04    // Function 4 : Get Rx state
#define     COM_GETTXSTATE      0x05    // Function 5 : Get Tx state
#define     COM_GETRXSIZE       0x06    // Function 6 : Get Rx data size
#define     COM_GETTXROOM       0x07    // Function 7 : Get Tx buffer room
#define     COM_ABORTRX         0x08    // Function 8 : Abort Rx data
#define     COM_ABORTTX         0x09    // Function 9 : Abort Tx data
#define     COM_GETERRORCOUNT   0x0A    // Function 10: Get Error count
#define     COM_CLRERRORCOUNT   0x0B    // Function 11: Clr Error count

//=======
//      RS232 handshake
//=======
#define     COM_HS_NONE         0x00
#define     COM_HS_DTRDSR       0x01
#define     COM_HS_RTSCTS       0x02
#define     COM_HS_XONXOFF      0x03

//=======
//      RS232 baud rate
//=======
#define     COM_BR_110          B110
#define     COM_BR_150          B150
#define     COM_BR_300          B300
#define     COM_BR_600          B600
#define     COM_BR_1200         B1200
#define     COM_BR_1800         B1800
#define     COM_BR_2000         B2000
#define     COM_BR_2400         B2400
#define     COM_BR_3600         B3600
#define     COM_BR_4800         B4800
#define     COM_BR_7200         B7200
#define		COM_BR_9600			B9600
#define     COM_BR_19200        B19200
#define     COM_BR_38400        B38400
#define     COM_BR_57600        B57600
#define     COM_BR_115200       B115200

//=======
//      RS232 parity
//=======
#define     COM_PR_NONE         0x00
#define     COM_PR_ODD          0x01
#define     COM_PR_EVEN         0x02

//=======
//      RS232 stop bit
//=======
#define     COM_SB_1            0x0
#define     COM_SB_2            0x1

//=======
//      RS232 data length
//=======
#define     COM_DL_5            0x00
#define     COM_DL_6            0x01
#define     COM_DL_7            0x02
#define     COM_DL_8            0x03

//=======
//      RS232 state code
//=======
#define     COM_S_CLOSED        0
#define     COM_S_OPENED        1
#define     COM_S_IDLE          2
#define     COM_S_BUSY          3
#define     COM_S_HOLD          4
#define     COM_S_ERROR         5

#define		ERROR_OBJECT_N2C		1					//caozz add 
#define		ERROR_OBJECT_E2C		2					//caozz add 
#define		ERROR_OBJECT_RS485		3					//JOYCE2011-5-4  add 
#define		ERROR_OBJECT_COM2		4         			//1HMI2DSP
#define		ERROR_OBJECT_TMIOT		11

#define 	ERROR_OBJECT_AUSTONE	100					//caozz add 
#define		ERROR_OBJECT_SIEMENS	101					//caozz add 
#define		ERROR_OBJECT_TM			102					//caozz add 2011/8/8 上午 11:30:43
#define		ERROR_AMMETER_TM		103					//Rw.ying	2013/1/10 10:29:05
//#define		ERROR_AMMETER_POWER		104					//Dongxl.2013-6-8 14:33:20
#define		ERROR_ROBOT_RS485	    104					//JOYCE2015-10-29  add 
#define		ERROR_OBJECT_ESR		105					/*ZHBA 2013-9-11 */  //20150106

//=======
//      RS232 result code of the most recent method(operation)
//=======
#define     COM_OK              0x0000  // Operation status : OK
#define     COM_ERR_DEVICE      0x0001  // Operation status : Illegal device code
#define     COM_ERR_FUNCTION    0x0002  // Operation status : Illegal function code

#define     COM_SUCCESS         0x0000
#define     COM_E_FAILURE       0x0010
#define     COM_E_NOPORT        0x0020
#define     COM_E_OPENED        0x0030
#define     COM_E_CLOSED        0x0040
#define     COM_E_BUSY          0x0050
#define     COM_E_HOLD          0x0060

#define     COM_E_DSR_NOTREADY  0x0080
#define     COM_E_CTS_NOTREADY  0x0090
#define     COM_E_RX_EMPTY      0x00A0
#define     COM_E_TX_FULL       0x00B0

#define     COM_E_TOKEN_NONE    0x10C0
#define     COM_E_TOKEN_STX     0x20C1
#define     COM_E_TOKEN_PKL     0x30C2
#define     COM_E_TOKEN_DATA    0x40C3
#define     COM_E_TOKEN_CRC     0x50C4
#define     COM_E_TOKEN_ETX     0x60C5
#define     COM_E_BUFFER_SMALL  0x70C6

#define     COM_E_TOKEN_WAIT    0x80C8

//=======
//      RS232 default parameter value
//=======
#define     COM_DEF_HANDSHAKE   COM_HS_DTRDSR
#define     COM_DEF_BAUDRATE    COM_BR_9600
#define     COM_DEF_PARITY      COM_PR_NONE
#define     COM_DEF_STOPBIT     COM_SB_1
#define     COM_DEF_DATALENGTH  COM_DL_8
#define     COM_DEF_RTIMEOUT    3000    // 8000
#define     COM_DEF_WTIMEOUT    3000    // 8000
#define     COM_DEF_STX         0x02    // Default Start of TeXt
#define     COM_DEF_ETX         0x03    // Default End of TeXt
#define     MAX_TOKEN_SIZE      4096    // 1024 -- 03/03/97

#define		MOLDNAME_LENGTH		40

enum TOKENSTATE
{
	TOKEN_NONE,
	TOKEN_STX,
	TOKEN_PKL0,
	TOKEN_PKL1,
	TOKEN_DATAFRAME0,
	TOKEN_DATAFRAME1,
	TOKEN_CRC0,
	TOKEN_CRC1,
	TOKEN_ETX,
	TOKEN_DONE,
	TOKEN_STATE_COUNT
};

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef void (*PRS232FUNC)(void *pClass, char *, int);      //netdigger 2007/4/17

typedef struct tag_CONFIG
{
	WORD    DataLength  : 2;
	WORD    StopBit     : 1;
	WORD    Parity      : 2;
	WORD                : 5;
	//WORD    BaudRate    : 4;		// netdigger 20050131
	WORD				: 4;
	WORD    Handshake   : 2;
	WORD	BaudRate;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	_CONFIG;
#else
#ifdef	D_X86
_CONFIG;
#endif
#endif

typedef struct tag_OPEN
{
	_CONFIG     wConfig;
	WORD        wRxBufferSize;          // Size of the receive buffer
	BYTE *		fpRxBuffer;             // Far pointer to the receive buffer
	WORD        wTxBufferSize;          // Size of the transmit buffer
	BYTE *		fpTxBuffer;             // Far pointer to the transmit buffer
} _OPEN;

typedef struct tagRS232
{
	WORD    wChannel;                   // Channel of RS232 - COM1/COM2/COM3/COM4
	union
	{
		_CONFIG config;
		WORD    wState;                 // Rx/Tx state
		WORD    wSize;                  // Rx data size
		WORD    wRoom;                  // Tx buffer room
		WORD    wErrorCount;            // Error count
		BYTE    cChar;                  // Character to be read/write
	};
	WORD        wRxBufferSize;  // Size of the receive buffer
	BYTE * fpRxBuffer;     // Far pointer to the receive buffer
	WORD        wTxBufferSize;  // Size of the transmit buffer
	BYTE * fpTxBuffer;     // Far pointer to the transmit buffer
	PRS232FUNC	pRSFunc;        //netdigger 2007/4/17
	void        *pClass;            //netdigger 2007/4/17
	WORD	wMode;
//	PRS232FUNC	pRSFunc;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmRS232;
#else
#ifdef	D_X86
tmRS232;
#endif
#endif

typedef struct tagTOKEN
{
	union
	{
		struct
		{
			CW      cwPKL;
			CW      cwCRC;
		};
		struct
		{
			WORD    wPKL;
			WORD    wCRC;
		};
	};
	WORD    wCurrentLength;             // Accumulated length of <Data Frame> in byte
	WORD    wCurrentCRC;                // Accumulated CRC
	WORD    wState;                     // Token state
	CW      cwData;                     // Data to be packed from byte to word
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TOKEN;
#else
#ifdef	D_X86
TOKEN;
#endif
#endif

typedef struct tag_QUEUE
{
	WORD    wSize;
	WORD    wGuardBand;
	BYTE* 	pQueue;
	BYTE* 	pQueueEnd;
	BYTE* 	pInput;
	BYTE* 	pOutput;
	BOOL    bFull;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	_QUEUE;
#else
#ifdef	D_X86
_QUEUE;
#endif
#endif

typedef struct tag_IRS232
{
	tmRS232* 	prs232;

	WORD    wDataLength;
	WORD    wParity;
	WORD    wHandshake;

	WORD    wState;
	WORD    wRxState;
	WORD    wTxState;
	WORD    wErrorCount;
	WORD	wAction;
	_CONFIG	config;
	_QUEUE  queueRx;
	_QUEUE  queueTx;

	int			hCom;					// Com's handle;
	pthread_t	RxThread;				// 接收線程ID
	pthread_t	TxThread;				// 發送線程ID
//	sem_t		ThreadWait;				// 創建線程信號
//	sem_t		TxWait;					// 發送線程信號

	struct termios		Termios;		// 原先的RS232口的設置
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	_IRS232;
#else
#ifdef	D_X86
_IRS232;
#endif
#endif


//=================================================================
//tmcontrol
//=================================================================
#define		CNT_INT_MOTION_CONTROL_NUM  32	//服務最大個數

/*===========================================================================+
|           Constant  ---CurveData                                          |
+===========================================================================*/
#define		MAX_CURVE_NUM								60      ////Sunny<20070815> amend 12;
#define		MAX_CURVE_TEMPNUM							10 		//5;	//James modify 2008/5/23 09:03上午
#define		MAX_POINT_NUM								600
#define		NEW_TO_LASTTIME								5

//	oil data id
#define		CURVE_OIL_DATA_INJECT_REALPRESS						0			//	inject pressure
#define		CURVE_OIL_DATA_INJECT_REALSPEED						1			//	inject speed
#define		CURVE_OIL_DATA_INJECT_REALPOS						2			//	inject postion
#define		CURVE_OIL_DATA_HOLD_REALPRESS						3			//	hold press
#define		CURVE_OIL_DATA_HOLD_REALSPEED						4			//	hold speed
#define		CURVE_OIL_DATA_CHARRG_REALPRESS						5			//	charge pressure
#define		CURVE_OIL_DATA_LASTTIME_INJECT_REALPRESS			6			//	temporarily inject pressure
#define		CURVE_OIL_DATA_LASTTIME_INJECT_REALSPEED			7       	//	temporarily inject speed   
#define		CURVE_OIL_DATA_LASTTIME_INJECT_REALPOS				8       	//	temporarily inject postion 
#define		CURVE_OIL_DATA_LASTTIME_HOLD_REALPRESS				9       	//	temporarily hold press     
#define		CURVE_OIL_DATA_LASTTIME_HOLD_REALSPEED				10      	//	temporarily hold speed     
#define		CURVE_OIL_DATA_LASTTIME_CHARRG_REALPRESS			11      	//	charge pressure
#define		CURVE_OIL_DATA_INJECT_SETPRESS						12			//	inject pressure setting
#define		CURVE_OIL_DATA_INJECT_SETSPEED						13			//	inject	speed settting
#define		CURVE_OIL_DATA_INJECT_SETPOS						14			//	inject pos seeting
#define		CURVE_OIL_DATA_HOLD_SETPRESS						15			//	hold press	setting
#define		CURVE_OIL_DATA_HOLD_SETSPEED						16      	//  hold speed  setting
#define		CURVE_OIL_DATA_CHARGE_SETPRESS						17			//	charge press setting
#define		CURVE_OIL_DATA_CHARGE_SETBACKPRES					18			//  charge backpres setting //Sunny<20070815>
#define		CURVE_OIL_DATA_CHARGE_REALBACKPRES					19      	//  charge backpres real
#define		CURVE_OIL_DATA_CHARGE_SETRPM						20      	//  charge RPM setting
#define		CURVE_OIL_DATA_CHARGE_REALRPM						21     		//  charge RPM real
#define		CURVE_OIL_DATA_INJECT_DXPOS							22			//	inject pos  x=pos,y=timer
#define		CURVE_OIL_DATA_HOLD_DXPOS							23			//	Hold Pos
//const	int CURVE_OIL_DATA_LASTTIME_CHARGE_REALRPM		= 22;       //  temporarily charge RPM
//const	int CURVE_OIL_DATA_LASTTIME_CHARGE_REALBACKPRES	= 23;       //  temporarily charge backpres
#define		CURVE_OIL_DATA_LASTTIME_HOLD_REALPOS				25

//temporarily data id
#define		CURVE_OIL_DATA_INJECT_TEMPORARILY_REALPRESS		3000		//	inject pressure temporarily data
#define		CURVE_OIL_DATA_INJECT_TEMPORARILY_REALSPEED		3001		//	inject speed temporarily data
#define		CURVE_OIL_DATA_INJECT_TEMPORARILY_REALPOS		3002		//	inject postion temporarily data
#define		CURVE_OIL_DATA_HOLD_TEMPORARILY_REALPRESS		3003		//	hold press temporarily data
#define		CURVE_OIL_DATA_HOLD_TEMPORARILY_REALSPEED		3004		//	hold speed temporarily data
#define		CURVE_OIL_DATA_CHARRG_TEMPRARILY_REALPRESS		3005		//	charge pressure temporarily data
#define		CURVE_OIL_DATA_INJECT_TEMPORARILY_DXPOS			3022		//	inject  pos temporarily data 
#define		CURVE_OIL_DATA_HOLD_TEMPORARILY_DXPOS			3023		//	3000 +HOLD_DXPOS 

//#define		CURVE_OIL_DATA_LASTTIME_INJECT_REALPRESS	6				//	temporarily inject pressure
//#define		CURVE_OIL_DATA_LASTTIME_INJECT_REALSPEED	7       //	temporarily inject speed
//#define		CURVE_OIL_DATA_LASTTIME_INJECT_REALPOS		8       //	temporarily inject postion
//#define		CURVE_OIL_DATA_LASTTIME_HOLD_REALPRESS		9       //	temporarily hold press
//#define		CURVE_OIL_DATA_LASTTIME_HOLD_REALSPEED		10      //	temporarily hold speed
//#define		CURVE_OIL_DATA_LASTTIME_CHARRG_REALPRESS	11      //	charge pressure

#define		TEMPORARILY_TO_NEW							3000

/*===========================================================================+
|           Constant  ---QualityControl                                      |
+===========================================================================*/
#define		MONI_DATATYPE_REAL_ID								0				//	實際值
#define     MONI_DATATYPE_LIMITUP_ID					1				//	上限值(根據誤差量和誤差百分比)
#define     MONI_DATATYPE_LIMITLOWER_ID				2				//	下限值(根據誤差量和誤差百分比)
#define     MONI_DATATYPE_FILEREAL_ID					3				//	文件中的實際值
#define     MONI_DATATYPE_STANDARDAVG_ID			4				//	標準差
#define     MONI_DATATYPE_SIG_ID							5				//	sig
#define     MONI_DATATYPE_AVG_ID							6				//	平均值
#define     MONI_DATATYPE_AVGLIMITUP_ID				7				//	平均值的上限(平均值加上標準差)
#define     MONI_DATATYPE_AVGLIMITLOWER_ID		8				//	平均值的下限(平均值減去標準差)
#define     MONI_DATATYPE_SUM_ID							9				//	和
#define     MONI_DATATYPE_SQRSUM_ID						10			//	平方和

/*===========================================================================+
|           Constant  ---Error                                             |
+===========================================================================*/
#define     tmMAX_OP_ERRORSAVE			14
#define     tmERROR_NULL				(tmMAX_OP_ERRORSAVE * 16)
#define     tmMODE_MANUAL				-1
#define     tmMODE_SEMIAUTO				1
#define     tmMODE_AUTOADJMOLD			11
#define     tmMAX_MONITOR         		100

#define		tmUNKNOWN_ERRORFLAG			0
#define		tmOIL_ERRORFLAG				1
#define		tmSIM_ERRORFLAG				2
#define		tmAUSTONE_ERRORFLAG			3
#define		tmRS485_ERRORFLAG			4
#define		tmN2C_ERRORFLAG				5
#define		tmMNT_ERRORFLAG				6
#define		tmCOM2_ERRORFLAG			7   //1HMI2DSP
#define		tmM3_ERRORFLAG				8
#define		tmE2C_ERRORFLAG				9
#define		tmRobotRS485_ERRORFLAG		10

#define     tmMAX_OP_ERRORFLAG			16       // 6*16

#define		tmOIL_MONI2FLAG				1
#define		tmI32_MONI2FLAG				2

#define		tmMAX_USER					11
#define		tmMAX_SIMD_MSGCOUNT			1000
#define		LOGIN_INVALIDID				0x8001
#define		LOGIN_INVALIDPASSWORD		0x8002

#define		SHOTCOUNT_SETZERO			0x0001			//開模總數歸零 	//<<yang 2006/10/22 
#define		PACKCOUNT_SETZERO			0x0002			//裝模總數歸零 
#define		AUTOLUBA_SETZERO			0x0004			//自動潤滑

#define     SPEECH_REPEATCOUNT  		10000
#define     SPEECH_DELAYEDCOUNT 		20000

#define		I32_TEST_PANEL				1				//test panel_key flag
#define		I32_OUTPUT_KEY				2				//Output key flag
#define		SIMD_FO_COUNT				20				// FO command count
#define		CONST_INST_FLAG				0x1234ABCD		//LEO20070404
#define		MAX_PRIVILEGE				10				//highest Privilege

typedef struct tagtmDB_EFLAG
{
	WORD        wErrorFlag[tmMAX_OP_ERRORFLAG];
	WORD        wCoreAlarmFlag;                     // 中子警報組別狀態 //St20031128 add
	WORD				wErrorFlag2[8];
} tmDB_EFLAG;

typedef struct tagtmDB_BUFFER
{
	WORD    wOperationMode;
	WORD    wOperationStep;
	WORD    wOperationSubStep;
	char    cIndexMode;                 // Operation mode index
	WORD    cIndexError;                // Error code index
	WORD    cIndexError2;                // Error code index
	BOOL    bSwitchToAuto;              // Change operation mode from manual to auto
	WORD    wSwitchToAutoShutCount;     // Shut count after changing operation mode from manual to auto

	_FLAG   flMotorStatus;
	_FLAG   flHeaterStatus;

	BOOL    bDelayedSpeech;
	long    lDelayedCount;
	BOOL    bRepeatSpeech;
	long    lRepeatCount;
} tmDB_BUFFER;

typedef struct tagOPERATION
{	//LEO20070404
	DWORD	dwFlag;						// flag						0x1234ABCD
	char    cMachineID[8 + 1];          // 機台編號                 // 0-7 (unit=BYTE)
	tmDATE  dateMachineOut;             // 機台出廠日期             // 8,9,10,12
	tmDATE  dateMachineSell;            // 機台交易日期             // 8,9,10,12
	BYTE    cState;                     // 運轉狀態                 // 14
	BYTE    cStages;                    // 期數                     // 15
	BYTE    cStageNow;                  // 當前期數                 // 16
	WORD    cDaysTimeoutPeriod;         // 單期停車天數             // 17
	tmDATE  dateTimeout;                // 下期停車日期             // 18,19,20,22
	WORD    cDaysTimeoutAlarmPeriod;    // 警告天數                 // 24
	tmDATE  dateTimeoutAlarm;           // 下次警告日期             // 26,27,28,30
	tmTIME	timeStop;					// 停車時間LEO20050119 Judge time on work
	short   nMinuteTimeoutAlarmInterval;// 警告間隔分鐘             // 32-33
	long    lMinute;                    // 運轉時數                 // 34-37
	long    lMinuteMotor;               // Motor on  minute
	long    lMinuteAuto;                // Automode  minute
	BOOL    bTimeout;                   // 警告旗標                 // 38,39
	WORD	wCRC;
	char   cMachineType[9];               // 機台型號                 // //YANGY2010-1-13
	tmDATE	CurrentData;				//上一次存儲日期
	tmTIME	CurrentTime;				//上一次存儲時間
	WORD	wBatNotExit;				//檢測電池不存在3次則停機
} OPERATION;

extern		WORD	g_wiNetStatus;
extern		WORD	g_wDspStatus;
extern		WORD	g_wDsp2Status;
extern		WORD	g_wErrorStatus;
extern		WORD	g_wErrorStatus_Dsp28;
/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
extern		tmDB_BUFFER		m_dbbuffer;
extern		tmDB_EFLAG		m_dberrorflg;
extern		tmDB_BUFFER		m_dbbuffer2;
extern		tmDB_EFLAG		m_dberrorflg2;

#define     dbbuff              m_dbbuffer
#define     dbeflg              m_dberrorflg
#define     dbbuff2              m_dbbuffer2
#define     dbeflg2              m_dberrorflg2

//=================================================================
//taskinet
//=================================================================
#define		MAX_ETOKEN_SIZE					32768				//原8192  Caozz 2010/8/3 05:53下午

#define     UDP_SRCPORT                     0x3039      		// Source machine port number
#define     UDP_DSTPORT                     0x3039      		// Destination machine port number

#define     PROINET_VERSION					2           		// inet protocol version
#define     PROINET_FLOW_MMITOIPS			0           		// inet data flow
#define     PROINET_FLOW_IPSTOMMI			1           		// inet data flow
#define		INET_COUNT_IPSONLINE			3					// not use

#define		CONST_MAX_PLANCOUNT				10

#define		CONST_LENGTH_MAC				6
#define		CONST_LENGTH_IP					4

#define		MAX_CH_TEMP						20

#define		NET_RESTARTNET					1
#define		NET_SETINITPARA					2

#define     CONST_FLAG_OPSUCCESS            1
#define     CONST_FLAG_OPFALSE              0
#define     CONST_FLAG_EXIST                1

// Update Type
#define     UPDATE_TYPE_WORD                0
#define     UPDATE_TYPE_DWORD               1
#define     UPDATE_TYPE_PCHAR               2
#define     UPDATE_TYPE_DATE                3
#define     UPDATE_TYPE_TIME                4
#define     UPDATE_TYPE_DATETIME            5

// From inet mold information flag
#define		FINET_MOLD_NULL					0x0001
#define		FINET_MOLD_COUNT				0x0002
#define		FINET_MOLD_LIST					0x0004

// content
#define     PROINET_CONT_DATA                          0x0000FFFF			// Content: Shield Code                     屏蔽碼

#define     PROINET_CONT_STAT_LINE_ONLINE              0x00000000           // Content: Status   --> line Status        --> online              連線狀態
#define     PROINET_CONT_STAT_PARA_TEMPER              0x00010000           // Content: Status   --> parameter Status   --> Temperature         溫度資料
#define     PROINET_CONT_STAT_PARA_MONITOR             0x00010001           // Content: Status   --> parameter Status   --> Monitor             監測資料
#define     PROINET_CONT_STAT_PARA_INJECT              0x00010002           // Content: Status   --> parameter Status   --> Inject              射出資料
#define     PROINET_CONT_STAT_PARA_UPDATE              0x00010003           // Content: Status   --> parameter Status   --> Update              資料修改
#define     PROINET_CONT_STAT_PARA_MOLDCOUNT           0x00010004           // Content: Status   --> parameter Status   --> Mold Count          模具數量
#define     PROINET_CONT_STAT_PARA_MOLDLIST            0x00010005           // Content: Status   --> parameter Status   --> Mold List           模具列表
#define     PROINET_CONT_STAT_PARA_MOLDEXIST           0x00010006           // Content: Status   --> parameter Status   --> Mold Exist          模具是否存在
#define     PROINET_CONT_STAT_CTRL_MOTION              0x00020000           // Content: Status   --> control Status     --> Motion              機器動作
#define     PROINET_CONT_STAT_CTRL_ERROR               0x00020001           // Content: Status   --> control Status     --> Errorlog            異常資料
#define     PROINET_CONT_STAT_OPER_MODIMOLD            0x00030000           // Content: Status   --> operate Status     --> Modify Mold         模具修改反饋
#define     PROINET_CONT_STAT_OPER_COPYMOLD            0x00030001           // Content: Status   --> operate Status     --> Copy Mold           模具拷貝反饋
#define     PROINET_CONT_STAT_OPER_CTRLMOTOR           0x00030002           // Content: Status   --> operate Status     --> Control Motor       馬達操作反饋
#define     PROINET_CONT_STAT_OPER_CTRLHEATER          0x00030003           // Content: Status   --> operate Status     --> Control Heater      電熱操作反饋
#define     PROINET_CONT_STAT_WORK_MACHSTATUS          0x00040000           // Content: Status   --> produce Status     --> Machine Status      機器狀態
#define     PROINET_CONT_STAT_WORK_PRODSTATUS          0x00040001           // Content: Status   --> produce Status     --> Produce Status      生產狀態
#define     PROINET_CONT_STAT_WORK_PLANSTATUS          0x00040002           // Content: Status   --> produce Status     --> Plan schedule Status計畫單狀態
#define     PROINET_CONT_STAT_WORK_PRODCOUNT           0x00040003           // Content: Status   --> produce Status     --> Produce Count       生產數量

#define     PROINET_CONT_PARA_LINE_IPTIME              0x01000000           // Content: Parameter--> line parameter     --> IP & Time           MMI IP和時間
#define     PROINET_CONT_PARA_LINE_PRODUCEFLAG         0x01000001           // Content: Parameter--> line parameter     --> Produce Flag        生管標誌 //<<LEO20060801
#define     PROINET_CONT_PARA_CONF_VERSION             0x01010000           // Content: Parameter--> config parameter   --> Version             版本參數
#define     PROINET_CONT_PARA_CONF_CONFIGURE           0x01010001           // Content: Parameter--> config parameter   --> Configure           配置參數
#define     PROINET_CONT_PARA_CONF_CONFIG              0x01010002           // Content: Parameter--> config parameter   --> Config              配置參數
#define     PROINET_CONT_PARA_CTRL_MACHINE             0x01020000           // Content: Parameter--> control parameter  --> Machine             機器參數
#define     PROINET_CONT_PARA_CTRL_MOLDSET             0x01020001           // Content: Parameter--> control parameter  --> Moldset             模具參數
#define     PROINET_CONT_PARA_OPER_USER                0x01030000           // Content: Parameter--> operate parameter  --> User Information    操作者信息

#define     PROINET_CONT_COMD_GETD_ONLINE              0x42000000           // Content: Command  --> get value command  --> Get online status   取得連線信息
#define     PROINET_CONT_COMD_GETD_IPTIME              0x42000001           // Content: Command  --> get value command  --> Get ip and time     取得IP和時間
#define     PROINET_CONT_COMD_GETD_USER                0x42000002           // Content: Command  --> get value command  --> Get user information取得用戶信息
#define     PROINET_CONT_COMD_GETD_MACHSTATUS          0x42000003           // Content: Command  --> get value command  --> Get machine status  取得機器狀態信息
#define     PROINET_CONT_COMD_GETD_MOTION              0x42000004           // Content: Command  --> get value command  --> Get motion          取得機器動作信息
#define     PROINET_CONT_COMD_GETD_MOLDCOUNT           0x42000005           // Content: Command  --> get value command  --> Get mold count      取得伺服器模具數量
#define     PROINET_CONT_COMD_GETD_MOLDLIST            0x42000006           // Content: Command  --> get value command  --> Get mold list       取得伺服器模具列表
#define     PROINET_CONT_COMD_GETD_MOLDEXIST           0x42000007           // Content: Command  --> get value command  --> Get mold exist      取得模具是否存在信息
#define     PROINET_CONT_COMD_GETD_MOLDSET             0x42000008           // Content: Command  --> get value command  --> Get moldset         取得模具設定信息
#define     PROINET_CONT_COMD_GETD_TEMPER              0x42000009           // Content: Command  --> get value command  --> Get Temperature     取得溫度信息
#define     PROINET_CONT_COMD_GETD_MONITOR             0x4200000A           // Content: Command  --> get value command  --> Get Monitor         取得監測信息
#define     PROINET_CONT_COMD_GETD_INJECT              0x4200000B           // Content: Command  --> get value command  --> Get Inject          取得射出信息
#define     PROINET_CONT_COMD_GETD_MACHCONFIG          0x4200000C           // Content: Command  --> get value command  --> Get Machine         取得機器參數信息
#define     PROINET_CONT_COMD_GETD_PLANSCHEDULE        0x4200000D           // Content: Command  --> get value command  --> Get Plan Schedule   取得計畫單
#define     PROINET_CONT_COMD_GETD_PRODSTATUS          0x4200000E           // Content: Command  --> get value command  --> Get Produce         取得生產狀態
#define     PROINET_CONT_COMD_GETD_PLANSTATUS          0x4200000F           // Content: Command  --> get value command  --> Get Plan Schedule   取得計畫單狀態
#define     PROINET_CONT_COMD_CTRL_MODIFYMOLD          0x42010000           // Content: Command  --> control command    --> Modify mold         修改模具資料
#define     PROINET_CONT_COMD_CTRL_COPYMOLD            0x42010001           // Content: Command  --> control command    --> Copy mold           拷貝模具資料
#define     PROINET_CONT_COMD_CTRL_DELMOLD             0x02010002           // Content: Command  --> control command    --> Delete mold         刪除模具資料
#define     PROINET_CONT_COMD_CTRL_SETMOTOR            0x02010003           // Content: Command  --> control command    --> Set motor           控制馬達
#define     PROINET_CONT_COMD_CTRL_SETHERTER           0x02010004           // Content: Command  --> control command    --> Set Heater          控制電熱
#define     PROINET_CONT_COMD_CTRL_DELALLMOLD          0x02010005           // Content: Command  --> control command    --> Del server all mold 刪除伺服器所有模具
#define     PROINET_CONT_COMD_CTRL_DELPLAN             0x02010006           // Content: Command  --> control command    --> Del plan Schedule   刪除計畫單
#define     PROINET_CONT_COMD_ORDR_PRODSCHEDULE        0x42020000           // Content: Command  --> control order      --> Produce Command     下達生產計劃單
#define     PROINET_CONT_COMD_ORDR_PRODMESSAGE         0x02020001           // Content: Command  --> control order      --> Produce Message     發送生產簡訊
#define		PROINET_NULL							   0xFFFFFFFFu

#define  	CIDS_MONITOR_SHUTOFF 						0x00010008
#define  	CIDS_ERROR_SHUTOFF  						0x00020002
#define  	CIDS_UPDATE_SHUTOFF     					0x00010009

//Write and Request
#define		COMM_INET_GETONLINESTAT						0X8024
#define		COMM_INET_GETMENUFACTUREFUNC				0x8025
#define		COMM_INET_GETDBCHANGE						0x8026

#define		COMM_INET_SENDMACHINE						0X8201
#define		COMM_INET_SENDMACHINEPRODUCESTATUS			0X8203
#define		COMM_INET_SENDPLANSCHEDULESTATUS			0X8204
#define		COMM_INET_SENDPRODUCECOUNT					0X8207
#define		COMM_INET_SENDMOLDSET						0X8213
#define		COMM_INET_SETMODIFYMOLDSET					0X8217
#define		COMM_INET_SETDBCHANGE						0x8218

//#ifdef	D_PXA270
//#pragma pack(2)
//typedef		DWORD	 _COUNTL;
//#pragma pack()
//#else
//typedef		DWORD	_COUNTL __attribute__((packed, aligned(2)));
//#endif

// add by J.Wang 2015/7/24 9:45:29
typedef struct tagINETPROTOCOL
{
	WORD		wLength;		/*衪祜軞酗僅ㄛ婦嬤芛迵杅擂⑹*/
	WORD		wVersion;		/*衪祜唳掛*/
	BYTE		byStamp;		/*肮衪祜痋唗瘍*/
	BYTE		byFlag;			/*杅擂痋杻梗諷秶恁砐*/
	DWORD		dwProID;		/*衪祜ID*/
	BYTE		byMAC[6];		/*諷秶んID*/
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	NETPRO;
#else
#ifdef	D_X86
NETPRO;
#endif
#endif

//typedef struct tagINETPROTOCOL
//{
//    DWORD      dwProVersion;                                 //協議版本
//    DWORD      dwReserve;                                    //保留
//    DWORD      dwDataSource;                                 //數據來源
//    DWORD      dwDataID;                                     //數據編號
//}
//#ifdef	D_ARM
//__attribute__((packed, aligned(2)))	NETPRO;
//#else
//#ifdef	D_X86
//NETPRO;
//#endif
//#endif

typedef union tagNETTX
{
	BYTE	acBuffer[MAX_ETOKEN_SIZE];
}	NETTX;

typedef union tagNETRX
{
	BYTE	acBuffer[MAX_ETOKEN_SIZE];
}	NETRX;

typedef struct tagNETFRAMERX
{
	NETPRO	netPro;
	NETRX  	netData;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	NETFRAMERX;
#else
#ifdef	D_X86
NETFRAMERX;
#endif
#endif

typedef	struct tagDATE
{
	BYTE	day;
	BYTE	month;
	WORD	year;
	BYTE	dayofweek;
	BYTE	reserved;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DATE;
#else
#ifdef	D_X86
DATE;
#endif
#endif

typedef struct tagNETFRAMETX
{
	NETPRO	netPro;
	NETTX  	netData;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	NETFRAMETX;
#else
#ifdef	D_X86
NETFRAMETX;
#endif
#endif

typedef struct tagREMOTEPROTOCOL
{
	DWORD		dwDataLength;		//數據長度
	DWORD      	dwProVersion;   	//協議版本
	DWORD      	dwDataID;       	//數據編號
	DWORD		dwEcho;				//回復標志
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	REMOTEPRO;
#else
#ifdef	D_X86
REMOTEPRO;
#endif
#endif

typedef struct tagREMOTEFRAME
{
	REMOTEPRO	netPro;
	NETTX  		netData;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	REMOTEFRAME;
#else
#ifdef	D_X86
REMOTEFRAME;
#endif
#endif


struct eth_link {
	DWORD	cmd;
	DWORD	data;
};

typedef struct tagTNet_Temper
{
	_COUNTL     dwShotCountCurrent;
	_DA         daTemp_Real[MAX_CH_TEMP];                   //實際溫度
	_DA         daTemp_Setting[MAX_CH_TEMP];                //設定溫度
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNet_Temper;
#else
#ifdef	D_X86
TNet_Temper;
#endif
#endif

typedef union tagTNet_Moni
{
	struct
	{
		WORD        wall[50];
	};
	struct
	{
		DWORD       dw[5];
		WORD        w[40];
	};
	struct
	{
		_COUNTL		dwShotCount;
		_TIMEL      tmlCycle;                       // 循環計時實際值
		_TIMEL      tmlInject;                      // 射出計時實際值
		_TIMEL      tmlTurnToHold;                  // 保壓轉換時間實際值
		_TIMEL      tmlCharge;                      // 儲料計時實際值
		_TIMER      tmClampClose;                   // 關模計時實際值
		_TIMER      tmClampCloseProtect;            // 關模低壓計時實際值
		_TIMER      tmClampCloseHigh;               // 關模高壓計時實際值
		_AD         adPosi_ClampOpenEnd;            // 開模位置實際值
		_TIMER      tmClampOpen;                    // 開模計時實際值
		_DA         daPres_TurnToHold;              // 保壓轉換壓力實際值
		_AD         adPosi_InjectStart;             // 射出起點位置實際值
		_AD         adPosi_TurnToHold;              // 保壓轉換位置實際值
		_AD         adPosi_InjectHoldEnd;           // 射出終點位置實際值
		_AD         adPosi_InjectCushion;           // 射出監控位置實際值
		_COUNT      cnChargeRPM;                    // 儲料RPM實際值
		_TIMER      tmSuckBack;                     // 射退計時實際值
		_TIMER      tmEject;                        // 脫模計時實際值
		_DA         daPres_ClampCloseHigh;          // 關模高壓力實際值
		_DA         daPres_InjectMiddle;            // 射出尖峰壓力實際值
		_DA         daPres_ChargeMiddle;            // 儲料尖峰壓力實際值
		_TIMER      tmEjectAdv;                     // 脫進計時實際值
		_TIMER      tmEjectRet;                     // 脫退計時實際值
		_TIMER      tmCoreAIn;                      // 中子A進計時實際值
		_TIMER      tmCoreAOut;                     // 中子A退計時實際值
		_TIMER      tmCoreBIn;                      // 中子B進計時實際值
		_TIMER      tmCoreBOut;                     // 中子B退計時實際值
		_TIMER      tmCoreCIn;                      // 中子C進計時實際值
		_TIMER      tmCoreCOut;                     // 中子C退計時實際值
		_TIMER      tmCoreDIn;                      // 中子D進計時實際值
		_TIMER      tmCoreDOut;                     // 中子D退計時實際值
		_TIMER      tmCoreEIn;                      // 中子E進計時實際值
		_TIMER      tmCoreEOut;                     // 中子E退計時實際值
		_TIMER      tmCoreFIn;                      // 中子F進計時實際值
		_TIMER      tmCoreFOut;                     // 中子F退計時實際值
		WORD        wReservedMG[10];                // 保留
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNet_Moni;
#else
#ifdef	D_X86
TNet_Moni;
#endif
#endif

typedef struct tagRNETIPTIME
{
	BYTE        acLocalIP[CONST_LENGTH_IP];
	WORD        wCurrYear;
	WORD        wCurrMonth;
	WORD        wCurrWeek;
	WORD        wCurrDay;
	WORD        wCurrHour;
	WORD        wCurrMinute;
	WORD        wCurrSecond;
	WORD        wCurrHsecond;
} RNETIPTIME;

typedef struct tagTNET_MOLDSETHDR
{
	WORD		wRecordNo;
	WORD		awDataLength[3];
} TNET_MOLDSETHDR;

enum MLDPOSI
{
	MLDHDR_POSI,
	MLDA_POSI,
	MLDB_POSI,
	MLD_MMICONFIG
};

enum MEDIATYPE
{
	MEDIA_MMI,
	MEDIA_DISK,
	MEDIA_SERVER,
	MEDIA_COUNT
};

typedef struct tagIOETH
{
	BYTE    acSrcAddress[CONST_LENGTH_IP];	// Source IP address
	BYTE    acDstAddress[CONST_LENGTH_IP];	// Destination IP address
	BYTE    cProtocol;                      // Internet Protocols (as defined by RFC 1700)
	WORD    wSrcPort;                       // Source port number
	WORD    wDstPort;                       // Destination port number
} IOETH;

typedef struct tagTNET_MACHINEVERSION
{
	WORD                wIDProduct;         // 產品版本: 0 = 386; 1 = S7; 2 = LINUX
	WORD                wMMIVersion;        // 面板版本
	WORD                wMMICodeDate;       // 面板程式日期
	WORD                wDSPVersion;        // 主機版本
	WORD                wDSPCodeDate;       // 主機程式日期
	WORD                wMachineType;       // 機器類型: 雙色機等
	WORD                wStructType;        // 結構類型: 0 = 52;1 = 54資料
	WORD                wMaxMoldCount;      // 最大模具數量
	WORD                wTemperNumber;      // 溫度段數
	WORD                wSpeedPrecision;    // 速度精度
	WORD                wTimePrecision;     // 時間精度
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNET_MACHVERSION;
#else
#ifdef	D_X86
TNET_MACHVERSION;
#endif
#endif

typedef struct tagTNET_OPSTATUSBACK
{
	unsigned char       anICCIPAddr[CONST_LENGTH_IP];
	WORD                wOperateResult;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNET_OPSTATUSBACK;
#else
#ifdef	D_X86
TNET_OPSTATUSBACK;
#endif
#endif

typedef struct tagTNET_DBUPDATA1
{
	WORD                wDataType;
	char                CurUserID[8];       //修改人員ID
	WORD                DataAddress;        //參數地址
	WORD                OldValue;           //參數原值
	WORD                NewValue;           //參數新值
} TNET_DBUPDATA1;

typedef struct tagTNET_DBUPDATA2
{
	WORD                wDataType;
	char                CurUserID[8];       //修改人員ID
	WORD                DataAddress;        //參數地址
	DWORD               OldValue;           //參數原值
	DWORD               NewValue;           //參數新值
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNET_DBUPDATA2;
#else
#ifdef	D_X86
TNET_DBUPDATA2;
#endif
#endif

typedef struct tagTNET_DBUPDATA3
{
	WORD                wDataType;
	char                CurUserID[8];         //修改人員ID
	WORD                DataAddress;          //參數地址
	WORD                wOldStrLength;
	char                OldValue[16];         //參數原值
	WORD                wNewStrLength;
	char                NewValue[16];         //參數新值
} TNET_DBUPDATA3;

typedef struct tagTNET_DBUPDATA4
{
	WORD                wDataType;
	char                CurUserID[8]; 		   //修改人員ID
	WORD                DataAddress;  		   //參數地址
	unsigned char       OldValue[4];  		   //參數原值
	unsigned char       NewValue[4];  		   //參數新值
} TNET_DBUPDATA4;

typedef struct tagTNET_DBUPDATA5
{
	WORD                wDataType;
	char                CurUserID[8];          //修改人員ID
	WORD                DataAddress;           //參數地址
	unsigned char       OldValue[6];           //參數原值
	unsigned char       NewValue[6];           //參數新值
} TNET_DBUPDATA5;

typedef struct tagTNET_DBUPDATA6
{
	WORD                wDataType;
	char                CurUserID[8];          //修改人員ID
	WORD                DataAddress;           //參數地址
	unsigned char       OldValue[8];           //參數原值
	unsigned char       NewValue[8];           //參數新值
} TNET_DBUPDATA6;

typedef struct tagTNET_CONFIGURE
{
	WORD	awConfigure[240];
} TNET_CONFIGURE;

typedef struct tagTNet_User
{
	char	UserID[8];
} TNet_User;

typedef struct tagFNet_User
{
	WORD        wFlg_UserExist;					//操作員存在標志
	WORD        wPrivilage;						//權級
	char        acID[8];						//用戶名，20150730
	char        acPassword[8];					//密碼
	char        acEGName[8];					//英文名稱
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	FNet_User;
#else
#ifdef	D_X86
FNet_User;
#endif
#endif

//======
//		Manufacture data structure
//======
typedef union tagDB_PRODUCEMANAGE
{
	struct
	{
		WORD       wCount;
		WORD       wPlanProduceStatus;
		short      nCurrentIndex;
	};// HeadInformation;
	struct	//	fans modify 2009/5/19 09:17上午
	{
		char		acPlanScheduleNumber[18];
		char		acMoldName[18];
		char		acMoldID[MOLDNAME_LENGTH];
		char		acMoldColor[18];
		char    	acMoldMaterial[18];
		struct
		{
			_COUNTL		dwPlanProduceCount;
		} __attribute__((packed, aligned(2))) ;
		WORD		wUnitCount;
		DATE		dtPlanBeginDate;
		tmTIME		tmPlanBeginTime;
		DATE		dtPlanEndDate;
		tmTIME		tmPlanEndTime;
		WORD		wPlanScheduleStatus;

		struct
		{
			_COUNTL		dwGoodProductCount;
		} __attribute__((packed, aligned(2)))  ;

		struct
		{
			_COUNTL		dwBadProductCount;
		} __attribute__((packed, aligned(2))) ;
		DATE		dtFactBeginDate;
		tmTIME		tmFactBeginTime;
		DATE		dtFactEndDate;
		tmTIME		tmFactEndTime;
		DATE		dtLastPauseDate;
		tmTIME		tmLastPauseTime;
		DWORD		dwSortNo;
	} __attribute__((packed, aligned(2))); // ContentStruct;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))		DB_PRODMANAGE;
#else
#ifdef	D_X86
DB_PRODMANAGE;
#endif
#endif

typedef struct tagUSERPRODUCTS
{
	char     	acUserID[18];
	DWORD		dwUserProdCount;
	DWORD     	dwUserBadCount;
} USERPRODUCTS;						//Caozz 2010/8/17 11:14上午  生管中班別

typedef struct tagDB_MOLDHDR
{
	char       acID[MOLDNAME_LENGTH];			// 模具名稱
	DATE	   DateSave;			// datatime
	char       szMaterial[6];		// 材料
	char       szColor[6];			// 顏色
	WORD       cUnitCount;			// 單模產數
	WORD	   Media;				// 存取媒體
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DB_MOLDHDR;
#else
#ifdef	D_X86
DB_MOLDHDR;
#endif
#endif

typedef struct tagDB_MOLDHDR14
{
	char       acID[14];			// 模具名稱
	DATE	   DateSave;			// datatime
	char       szMaterial[6];		// 材料
	char       szColor[6];			// 顏色
	WORD       cUnitCount;			// 單模產數
	WORD	   Media;				// 存取媒體
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DB_MOLDHDR14;
#else
#ifdef	D_X86
DB_MOLDHDR14;
#endif
#endif

typedef struct tagTNET_MACHPRODUCESTATUS
{
	char     acPlanScheduleNumber[18];
	WORD     wProduceStatus;
} TNET_PRODUCESTATUS;

typedef struct tagTNET_PLANSCHEDULESTATUS
{
	char          acPlanScheduleNumber[18];
	WORD          wPlanScheduleStatus;
	struct
	{
		_COUNTL       dwGoodProductCount;
	} __attribute__((packed, aligned(2)))  ;

	struct
	{
		_COUNTL		  dwBadProductCount;
	} __attribute__((packed, aligned(2)))  ;

	DATE          dtFactBeginDate;
	tmTIME        tmFactBeginTime;
	DATE          dtFactEndDate;
	tmTIME        tmFactEndTime;
	DATE          dtLastPauseDate;
	tmTIME        tmLastPauseTime;
	DWORD		dwSortNo;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNET_PLANSTATUS;
#else
#ifdef	D_X86
TNET_PLANSTATUS;
#endif
#endif

typedef struct tagTNET_PLANCONTENT
{
	BYTE                anICCIPAddr[CONST_LENGTH_IP];
	WORD                wScheduleCount;
	WORD                wLength;
	TNET_PLANSTATUS     stPlanSchedule[CONST_MAX_PLANCOUNT];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNET_PLANCONTENT;
#else
#ifdef	D_X86
TNET_PLANCONTENT;
#endif
#endif

typedef struct tagTNET_REQUESTCMD
{
	WORD      wRequestFlag;
} TNET_REQUESTCMD;

typedef struct tagTNET_DELETECMD
{
	BYTE      anICCIPAddr[CONST_LENGTH_IP];
	char      acPlanScheduleNumber[18];
} TNET_DELETECMD;

typedef struct tagTNET_SENDPRODUCECOUNT
{
	char      acPlanScheduleNumber[18];
	struct
	{
		_COUNTL   dwGoodProductCount;
	} __attribute__((packed, aligned(2))) ;
	struct
	{
		_COUNTL   dwBadProductCount;
	} __attribute__((packed, aligned(2))) ;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TNET_PRODUCECOUNT;
#else
#ifdef	D_X86
TNET_PRODUCECOUNT;
#endif
#endif

typedef struct tagFNET_MACHPRODUCESTATUS
{
	NETPRO    ProBuff;
	WORD      wRequestFlag;
} FNET_PRODUCESTATUS;

typedef struct tagFNET_PLANSCHEDULESTATUS
{
	NETPRO    ProBuff;
	WORD      wRequestFlag;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	FNET_PLANSTATUS;
#else
#ifdef	D_X86
FNET_PLANSTATUS;
#endif
#endif

//	fans modify 2009/5/19 09:30上午
typedef struct tagFNET_PLANPRODUCECOMMAND
{
	char		acPlanScheduleNumber[18];
	char		acMoldName[18];
	char		acMoldID[MOLDNAME_LENGTH];			//	fans add
	char		acMoldColor[18];
	char		acMoldMaterial[18];

	struct
	{
		_COUNTL		dwPlanProduceCount;
	}	__attribute__((packed, aligned(2))) ;

	WORD		wUnitCount;
	DATE		dtPlanBeginDate;
	tmTIME		tmPlanBeginTime;
	DATE		dtPlanEndDate;
	tmTIME		tmPlanEndTime;
	WORD		wPlanScheduleStatus;
	struct
	{
		_COUNTL		dwGoodProductCount;
	} __attribute__((packed, aligned(2)));

	struct
	{
		_COUNTL		dwBadProductCount;
	} __attribute__((packed, aligned(2)));

	DATE		dtFactBeginDate;
	tmTIME		tmFactBeginTime;
	DWORD		dwSortNo;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	FNET_PLANCOMMAND;
#else
#ifdef	D_X86
FNET_PLANCOMMAND;
#endif
#endif

typedef struct tagFNET_PLANPRODUCECONTENT
{
	BYTE                anICCIPAddr[CONST_LENGTH_IP];
	WORD                wCmdFlag;
	WORD                wPlanCount;
	WORD                wContentLength;
	FNET_PLANCOMMAND    stPlanCommand[CONST_MAX_PLANCOUNT];
} FNET_PLANCONTENT;

typedef struct tagFNet_MoldList
{
	WORD                StartIndex;                                 //起始序號
	WORD                EndIndex;                                   //結束序號
	WORD                MoldCount;                                  //本次模具資料筆數
	char                acMoldHdrList[64 * 200];  						//模具列表
} FNet_MoldList;

typedef struct tagFNET_MOLDHDRLIST
{
	WORD			wTypeFlag;
	int				nCount;
	int				nStartIndex;
	int				nReceiveCnt;
	DB_MOLDHDR		moldhdrlist[200];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	FNET_MOLDHDRLIST;
#else
#ifdef	D_X86
FNET_MOLDHDRLIST;
#endif
#endif

typedef struct tagtmOIL_ERROR
{
	DWORD		dwID;                                // 故障代碼
#ifdef	D_X86
	WORD		wR1;
#endif
	DWORD		dwShotCount;                        // 開模序號
	tmDATE		dateStart;                          // 故障起始日期
	tmTIME		timeStart;                          // 故障起始時間
	tmDATE		dateFixed;                          // 故障排除日期
	tmTIME		timeFixed;                          // 故障排除時間
} tmOIL_ERROR;

typedef union tagtmDB_ERROR
{
	struct
	{
		WORD    wIndex;                             // Index
		WORD    wCount;                             // Count
	};
	struct
	{
		union
		{
			tmOIL_ERROR		OilError;
			//SIMD_MSG		SimMsg;
		};
		WORD	wSource;
#ifdef	D_X86
		WORD	wR1;
#endif
	};
} PACKED tmDB_ERROR;

typedef struct tagSIMD_MESSAGE
{
	BYTE		bMsgType;
	BYTE		bState;
	DWORD		dwTime;
	DWORD		dwDate;
	WORD		wObjNumber;
	union
	{
		char	cData[16];
		struct
		{
			long	lFO_Error;
			long	lFO_ErrorID1;
			long	lFO_ErrorID2;
			long	lFO_ErrorID3;
		};
		long	lAlarmNo;
		DWORD	dwInitEvent;
		DWORD	dwExecFaultType;
		struct
		{
			DWORD	dwPH_InitEvent;
			long	lPH_LogBaseAdrIn;
			long	lPH_LogBaseAdrOut;
			long	lPH_LogDiagAdr;
		};
		struct
		{
			long	lErrorCode;
			short	nSizeRet;
			short	nTypeRet;
			short	nCounterRet;
			short	nCycleRet;
			short	nNoElementsRet;
		};
		WORD		wErrorID;
		struct
		{
			long	lDrvErrorID;
			long	lDrvError;
		};
		struct
		{
			long	lAppError;
			long	lAppErrorID1;
			long	lAppErrorID2;
		};
	};
} PACKED SIMD_MSG;

typedef struct tagI32_MONI
{
	DWORD      tmCycleTime;                     //  0 循環計時實際值
	DWORD      tmClampClose;                   	//  1 關模計時實際值
	DWORD      tmClampOpen;                    	//  2 開模計時實際值
	DWORD      tmInject;                       	//  4 射出計時實際值
	DWORD      tmCharge;                       	//  5 儲料計時實際值
	DWORD      tmSuckBack;                     	//  6 射退計時實際值
	DWORD      tmEject;                        	//  7 脫模計時實際值
	double     adPosi_ClampOpenEnd;          	//  8 開模位置實際值
	double     daPres_TurnToHold;               //  9 保壓轉換壓力實際值
	double     tmTurnToHold;                    // 10 保壓轉換時間實際值
	double     adPosi_TurnToHold;               // 11 保壓轉換位置實際值
	double     adPosi_InjectStart;          	// 12 射出起點位置實際值
	double     adPosi_InjectHoldEnd;            // 13 射出終點位置實際值
} I32_MONI;

typedef struct tagI32_MONI2
{
	DWORD		dwShotCount;		// 開模序號
	I32_MONI	Moni;
} I32_MONI2;


typedef struct tagINETDBLIMIT		//從iNet得到的上下限資料 Caozz 2010/8/4 05:31下午
{
	DWORD		dwDBID;
	DWORD		dwLowerLimit;
	DWORD		dwUpperLimit;
} INETDBLIMIT;
typedef struct tagINETPUMPINFO
{
	WORD	wType;
	char 	szDate[12];
	char 	szNumber[12];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INET_PUMPINFO;
#else
#ifdef	D_X86
INET_PUMPINFO;
#endif
#endif

typedef struct tagPRODUCTSTATUS
{
	WORD	wID;
	WORD	wStatus;
	WORD 	wNameLen;
	char*	pszData;
} PRODUCTSTATUS;

typedef struct tagBADPRODUCTINFO
{
	WORD	wID;
	WORD	wStatus;
	WORD 	wNameLen;
	char*	pszData;
	DWORD	dwBadCount;
} BADPRODUCTINFO;

extern		WORD		g_wTaskDSP54;

#endif
