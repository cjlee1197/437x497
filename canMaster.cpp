/*==========================================================================+
|  Class    : CanOpen Master          										|
|  Task     : CanOpen Master file                     						|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 		                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 2012/07/27                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<unistd.h>
#include 	<string.h>
#include 	<signal.h>
#include 	<libgen.h>
#define 	__USE_GNU
#include 	<fcntl.h>
#include 	<getopt.h>
#include 	<pthread.h>
#include 	<signal.h>
#include 	<sched.h>
#include 	<sys/types.h>
#include 	<sys/socket.h>
#include 	<sys/ioctl.h>
#include 	<sys/uio.h>
#include 	<linux/can.h>
#include 	<linux/can/raw.h>
#include	<linux/can/error.h>
#include 	<net/if.h>
#include 	<linux/unistd.h>
#include 	<time.h>
#include 	"canMaster.h"
#include	"timer.h"
#include	"tmconfig.h"
#include	"tmshmsg.h"
#include	"main.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define 		TIMER_RELTIME					0
#define 		MODE_CYCLIC						0
#define 		MODE_CLOCK_NANOSLEEP			1
#define 		MODE_SYS_ITIMER					2
#define 		MODE_SYS_NANOSLEEP				3
#define 		USEC_PER_SEC					1000000
#define 		NSEC_PER_SEC					1000000000
        		
#define			SDOCMD_INITDOMAINDOWN			0x01
#define			SDOCMD_INITDOMAINUP				0x02
#define			SDOC_SENDFUNCID					0x600
        		
#define			MAX_PDO_COUNT					4
        		
#define 		PF_CAN 							29
#define			CAN_ID							6
#define 		COBID_TXPDO1        			0x180	// TXPDO1
// CAN statesm
#define			CAN_STATES_CONFIG				0x01
#define			CAN_STATES_RUNNING				0x0F
// M3 CAN Node ID
#define			M3_CAN_NODE_ID					6
#define			DSP28_CAN_NODE_ID				28

#define			M3_ONLINE_TIME					3000

#ifdef	D_ARM
	#ifdef	D_SOCKETCAN
	#define			CAN_RECV_BUFF					1
	#else
	#define			CAN_RECV_BUFF					2
	#endif
#else
	#ifdef	D_SOCKETCAN
	#define			CAN_RECV_BUFF					1
	#else
	#define			CAN_RECV_BUFF					10
	#endif
#endif
#define 		gettid() 						syscall(__NR_gettid)
//type define
typedef     	unsigned char       			BYTE;   /* 8-Bit */
typedef     	unsigned short 					WORD;   /* 16-Bit */
typedef     	unsigned long       			DWORD;  /* 32-Bit */

#ifdef  D_ARM
#define			CAN_OFF_CHECKLOOP				5
#else
#define			CAN_OFF_CHECKLOOP				50
#endif

pthread_t 		m_CanRxThreadID;
pthread_t 		m_CanTxThreadID;
pthread_t 		m_CanCHThreadID;
pthread_attr_t  m_ThreadRxAttr;
pthread_attr_t  m_ThreadTxAttr;

pthread_mutex_t			g_CANTxMutex;

void 	*TX_TestTask(void *param);
void 	*RX_TestTask(void *param);
void 	*Check_TestTask(void *param);
/*==========================================================================+
|                        struct                                             |
+==========================================================================*/
struct thread_param {
	int 				prio;
	int 				mode;
	int 				timermode;
	int 				signal;
	int 				clock;
	unsigned long 		max_cycles;
	struct 	thread_stat *stats;
	int 				bufmsk;
	unsigned long 		interval;
	int 				cpu;
	int 				sockfd;
};

// Struct for statistics
struct thread_stat {
	unsigned 	long 	cycles;
	unsigned 	long 	cyclesread;
	long 				min;
	long 				max;
	long 				act;
	double 				avg;
	long 				*values;
	long 				*hist_array;
	pthread_t 			thread;
	int 				threadstarted;
	int 				tid;
	long 				reduce;
	long 				redmax;
	long 				cycleofmax;
};

typedef union tagSDOCMD
{
	char				cdata;
	struct
	{
		char			s:1;
		char			e:1;
		char			n:2;
		char			rsvd:1;
		char			cmd:3;
	} bits;
} SDOCMD;

typedef union tagSDODATA
{
	char				cData[4];
	WORD				awData[2];
	DWORD				dwData;
} SDODATA;

typedef struct tagPDOMAP
{
	WORD   				wIndex;
	WORD				wSubIndex;
	//BYTE				wSubIndex;
	WORD				wDataSetSize;		
	DWORD   			dwData;			//PDO Data
} PDOMAP;

typedef struct tagSDODATABASE
{
	WORD   				wIndex;
	WORD				wSubIndex;
	//BYTE				wSubIndex;
	WORD				wDataSetSize;		
	char*   			pDataAddr;			//PDO Data
} SDODATABASE;

typedef struct tagPDO_BUFFER
{
	WORD				wCOBID;
	WORD				wDataLength;
	char				acData[MAX_CANDATALEN];
} PDO_BUFFER;

typedef struct tagMESSAGEPARSE_TABLE
{
	WORD				wMinID;
	WORD				wMaxID;
	void (*pParseFunc)(WORD *pw, WORD wLength);
} MESSAGEPARSE_TABLE;

DWORD	g_dwM3Key[MAX_KEYINDEX] = {0};
DWORD	g_dwTempM3Key[MAX_KEYINDEX] = {0};
DWORD	g_dwM3KeyCount[MAX_KEYINDEX] = {0};
DWORD	g_dwM3OldKey[MAX_KEYINDEX] = {0};
DWORD	g_dwM3LED[MAX_LEDINDEX] = {0};
DWORD	g_dwM3LEDOld[MAX_LEDINDEX] = {1};
int		g_nErrorFlag = 0;
int		g_nRestart	 = 0;
int		g_GetDataFlag = 0;
WORD	u_wRegRS = 0;
WORD	u_wRegER = 0;
#ifdef	D_SOCKETCAN
int		g_CanbusState = 2;
#else
int		g_CanbusState = 0;
#endif


/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
char	u_acOutPutData_Old[MAX_CANDATALEN]= "\0";

static int u_nskt 			= -1;
static int u_nskt_read 		= -1;
static int shutdowntask 	= 0; //James modify  0->1  for test 2013/11/22 16:06:03
static int clocksources[]	= {
	CLOCK_MONOTONIC,
	CLOCK_REALTIME,
};
static int 	u_nCANstate 	= CAN_STATES_CONFIG;// CAN_STATES_RUNNING;//
struct thread_stat 			u_stat[2];
struct thread_param 		u_param[2];
const PDOMAP SlaveM3PDOMAP[]=
{
	{0x1400,	0x01,	0x00,	0x08000206	},
	{0x1400,	0x02,	0x03,	0x000000FF	},
	{0x1600,	0x00,	0x03,	0x00000000	},
	{0x1600,	0x01,	0x00,	0x63200120	},
	{0x1600,	0x02,	0x00,	0x63200220	},
	{0x1600,	0x00,	0x03,	0x00000002	},
	
	{0x1400,	0x01,	0x00,	0x00000206	},
	{0x1800,	0x01,	0x00,	0x80000186	},
	{0x1800,	0x02,	0x03,	0x000000FF	},
	{0x1800,	0x02,	0x03,	0x000000FF	},
	{0x1800,	0x03,	0x02,	0x00000000	},
	{0x1800,	0x03,	0x02,	0x00000000	},
	{0x1800,	0x05,	0x02,	0x00000000	},
	{0x1A00,	0x00,	0x03,	0x00000000	},
	{0x1A00,	0x01,	0x00,	0x61200120	},
	{0x1A00,	0x02,	0x00,	0x61200220	},
	{0x1A00,	0x00,	0x03,	0x00000002	},
	{0x1800,	0x01,	0x00,	0x00000186	},
	
	{0x1801,	0x01,	0x00,	0x80000286	},
	{0x1801,	0x02,	0x03,	0x000000FF	},
	{0x1801,	0x03,	0x02,	0x00000000	},
	{0x1801,	0x05,	0x02,	0x00000000	},
	{0x1A01,	0x00,	0x00,	0x00000000	},
	{0x1A01,	0x01,	0x00,	0x61210120	},
	{0x1A01,	0x02,	0x00,	0x61210220	},
	{0x1A01,	0x00,	0x03,	0x00000002	},
	{0x1801,	0x01,	0x00,	0x00000286	},
//	{0x1800,	0X01,	0x00,	0x08000186	},
//	{0X1A00,	0X00,	0x03,	0		  	}, 
//	{0X1A00,	0X01,	0x00,	0x61200120	},
//	{0X1A00,	0X02,	0x00,	0x61200220	},
//	{0X1A00,	0X00,	0x03,	2		  	}, 
//	{0X1A01,	0X00,	0x03,	0		  	}, 
//	{0X1A01,	0X01,	0x00,	0x61210120	},
//	{0X1A01,	0X02,	0x00,	0x61210220	},
//	{0X1A01,	0X00,	0x03,	2		  	}, 
////	{0X1A00,	0X03,	0x00,	0x61200308	},
////	{0X1A00,	0X04,	0x00,	0x61200408	},
////	{0X1A00,	0X05,	0x00,	0x61200508	},
////	{0X1A00,	0X06,	0x00,	0x61200608	},
////	{0X1A00,	0X07,	0x00,	0x61200708	},
//	{0x1400,	0X01,	0x00,	0x08000206	},
//	{0X1600,	0X00,	0x03,	0			}, 
//	{0X1600,	0X01,	0x00,	0x63200120	},
//	{0X1600,	0X02,	0x00,	0x63200220	},
//	{0X1600,	0X00,	0x03,	2			}, 
	{0x1017,    0x00,   0x02,   1000		},
	{0xFFFF,	0xFF, 	0xFF, 	0xFFFFFFFF	}
};

PDO_BUFFER g_TxPDOBuffer[] = {
	{ 0x200+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x300+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x400+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x500+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x200+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x300+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x400+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0x500+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0} },
};

PDO_BUFFER g_RxPDOBuffer[] = {
	{ 0x180+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x280+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x380+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x480+M3_CAN_NODE_ID,		8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x180+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x280+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x380+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0}  },
	{ 0x480+DSP28_CAN_NODE_ID,	8, {0, 0, 0, 0, 0, 0, 0, 0}  },
};

DWORD	g_dwM3KeyboardVers = 0;

#define		CAN_SDO_M3KEYBOARD_VERS		0xBA00
SDODATABASE	g_CanDatabase[] = 
{
	{CAN_SDO_M3KEYBOARD_VERS, 0x00, 0x00, (char*)&g_dwM3KeyboardVers},	
};

PDOMAPADDR		g_TxPDOAddr[] = {
	{2, 	{4,4}, 	{NULL,NULL} },
};

PDOMAPADDR		g_RxPDOAddr[] = {
	{2, 	{4,4}, 	{NULL,NULL} },
	{2, 	{4,4}, 	{NULL,NULL} },
};

int 			u_nSlaveState 				= 0;
static	int 	u_nCommandId 				= CAN_COMMAND_TEST_AUTO;
static	int 	u_nCommandIdChange			= 0;
static	int 	u_nCanSendErrorCount		= 0;
static	int 	u_nCanHeartbeatErrorCount	= 0;
static	unsigned long	u_nSendAutoTestData 		= 0;
static	unsigned long	u_nSendAutoTestData2 		= 0;
static	long	u_nRecvAutoTestData 		= 0;
static	long	u_lnHeartbeatCount 			= 0;
static	int		u_nCanConfigIndex 			= 0;
static	int		u_nDelay 					= 10;
static	long	u_lnHeartbeatCountBak 		= 0;
static	long	u_lnHeartbeatTime			= M3_ONLINE_TIME;			//3S
static	char	u_acSendNormalData[MAX_CANDATALEN] = {0, 0, 0, 0,0, 0, 0, 0};
static	int		u_nStepBak					= -1;
static	int		u_nCheckCount				= 0;
static	int		u_nStep						= 0;

/*===================================================================
|																	|
===================================================================*/
int		SetCANDatabase(WORD wIndex,  WORD wSubIndex, WORD wDataType, char*	pData);
/*===================================================================
|						HELP										|
===================================================================*/
void	PrintFrameData(char *pdata, int size)
{
	//for(int i = 0; i < size; i++)
	//{
	//	printf("Data[%d] : %02X\n",i ,pdata[i]);
	//}

}
static inline long calcdiff(struct timespec t1, struct timespec t2)
{
	long diff = 0;
	diff = USEC_PER_SEC * ((int) t1.tv_sec - (int) t2.tv_sec);
	diff += ((int) t1.tv_nsec - (int) t2.tv_nsec) / 1000;
	return diff;
}
//---------------------------------------------------------------------
static inline void tsnorm(struct timespec *ts)
{
	while (ts->tv_nsec >= NSEC_PER_SEC) {
		ts->tv_nsec -= NSEC_PER_SEC;
		ts->tv_sec++;
	}
}
//---------------------------------------------------------------------
int		CreatSocketCan()
{
	int 	nfamily 		= PF_CAN;//, type = SOCK_RAW, proto = CAN_RAW;
	struct	sockaddr_can	addr;
	struct	ifreq			ifr;
	struct	can_filter		rfilter;
	memset(&addr,0,sizeof(addr));
	memset(&rfilter,0,sizeof(rfilter));
	can_err_mask_t	err_mask = 0xFFFFFFFF;//(CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF);
	
	
	if ((u_nskt = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("socket");
        return -1;
    }
    //fcntl(u_nskt, F_SETFL, O_NONBLOCK);
    rfilter.can_id   		= 0;
    rfilter.can_mask 		= 0;
    
	setsockopt(u_nskt, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
	setsockopt(u_nskt, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));
	addr.can_family 		= nfamily;
	if(1 == CtmConfig::GetInstance()->GetCANIndex())
	{
		strcpy(ifr.ifr_name, "can1");
	}
	else
	{
		strcpy(ifr.ifr_name, "can0");
	}
	ioctl(u_nskt, SIOCGIFINDEX, &ifr);
	addr.can_ifindex 		= ifr.ifr_ifindex;
	
	if (bind(u_nskt, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
	{
		perror("bind");
		return -1;
	}
	#ifdef	D_SOCKETCAN
	u_nskt_read	= u_nskt;
	#else
	u_nskt_read = open("/dev/tmcan0", O_RDWR | O_NOCTTY);
	#endif
	//printf("CreatSocketCan  u_nskt_read=%d \n", u_nskt_read);
	AddTimer(&u_lnHeartbeatTime);
	return u_nskt;
}

int		ReOpenSocketCan()
{
	int 	nfamily 		= PF_CAN;//, type = SOCK_RAW, proto = CAN_RAW;
	struct	sockaddr_can	addr;
	struct	ifreq			ifr;
	struct	can_filter		rfilter;
	can_err_mask_t	err_mask = 0xFFFFFFFF;//(CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF);
	g_nRestart = 1;
	//ioctl(u_nskt_read, 0x1004, 0);
	//usleep(100*1000);
	//ioctl(u_nskt_read, 0x1003, 0);
	//printf("ReOpenSocketCan 1 \n");
	
	#ifdef  D_ARM
		#ifdef	D_SOCKETCAN
		
		if(1 == CtmConfig::GetInstance()->GetCANIndex())
		{
			system("ip link set can1 down");
			usleep(500*1000);
			//system("ip link set can0 type can restart-ms 100");
			//usleep(200*1000);
			system("ip link set can1 up type can bitrate 500000");
			usleep(200*1000);
		}
		else
		{
			system("ip link set can0 down");
			usleep(500*1000);
			//system("ip link set can0 type can restart-ms 100");
			//usleep(200*1000);
			system("ip link set can0 up type can bitrate 500000");
			usleep(200*1000);
		}
		//sleep(1);
		#endif
	#else
	system("ip link set can0 down");
	usleep(500*1000);
	//system("ip link set can0 type can restart-ms 100");
	//usleep(200*1000);
	system("ip link set can0 up type can bitrate 500000");
	usleep(200*1000);
	sleep(1);
	#endif
	//printf("ReOpenSocketCan 2 u_nskt=%d \n", u_nskt);
	
//	if(u_nskt >=0 )
//	{
//		close(u_nskt);
//		u_nskt = -1;
//	}
//	if ((u_nskt = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
//    {
//        perror("socket");
//        return -1;
//    }
//    //fcntl(u_nskt, F_SETFL, O_NONBLOCK);
//	printf("ReOpenSocketCan 3 \n");
//    rfilter.can_id   		= 0;
//    rfilter.can_mask 		= 0;
//
//	setsockopt(u_nskt, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
//	setsockopt(u_nskt, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));
//	addr.can_family 		= nfamily;
//	strcpy(ifr.ifr_name, "can0");
//	ioctl(u_nskt, SIOCGIFINDEX, &ifr);
//	addr.can_ifindex 		= ifr.ifr_ifindex;
//
//	if (bind(u_nskt, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
//	{
//		perror("bind");
//		return -1;
//	}
	//printf("ReOpenSocketCan 4 u_nskt=%d\n", u_nskt);
	//system("ip link set can0 type can restart");
	u_lnHeartbeatTime = M3_ONLINE_TIME;
	//pthread_cancel(m_CanTxThreadID);
	//pthread_cancel(m_CanRxThreadID);
	//pthread_create(&m_CanTxThreadID, NULL, TX_TestTask, NULL/*&u_param[0]*/);
	//pthread_create(&m_CanRxThreadID, NULL, RX_TestTask, NULL/*&u_param[1]*/);
	g_nRestart = 0;
}
//---------------------------------------------------------------------

void	CanConfigTask()
{
	//printf("CanConfigTask u_nStepBak=%d u_nStep=%d \n", u_nStepBak, u_nStep);
	//2012/8/2 add <<<<<<
	if(u_nskt < 0 || u_nStepBak == u_nStep) 
	{
		u_nCheckCount++;
		if(u_nCheckCount > CAN_OFF_CHECKLOOP/2)
		{
			processTransmitSDOCInitDomainDownload(M3_CAN_NODE_ID,SlaveM3PDOMAP[u_nCanConfigIndex].wIndex,\
			SlaveM3PDOMAP[u_nCanConfigIndex].wSubIndex, SlaveM3PDOMAP[u_nCanConfigIndex].dwData, \
			SlaveM3PDOMAP[u_nCanConfigIndex].wDataSetSize); 	
		}
		else if(u_nCheckCount > CAN_OFF_CHECKLOOP)
		{
			u_nCanConfigIndex = 0;
			u_nStepBak = -1;
			u_nStep = 0;
			//printf("repeat config\n");
		}
		return ;
	}
	u_nCheckCount = 0;
	//2012/8/2 add >>>>>
 	if(u_nCanConfigIndex >= sizeof(SlaveM3PDOMAP)/sizeof(PDOMAP))
 	{
 		//printf("====set config 0 \n");
 		u_nCanConfigIndex = 0;	
 	}
 	//printf("u_nCanConfigIndex=%d u_nDelay=%d size=%x \n", u_nCanConfigIndex, u_nDelay, SlaveM3PDOMAP[u_nCanConfigIndex].wDataSetSize );
	if(u_nskt < 0 || g_nRestart == 1) return ;
	if(SlaveM3PDOMAP[u_nCanConfigIndex].wDataSetSize == 0xFF && u_nDelay >= CAN_OFF_CHECKLOOP)
	{
		//Read Vers 
		processTransmitSDOCInitDomainUpload(M3_CAN_NODE_ID, CAN_SDO_M3KEYBOARD_VERS, 0);
		usleep(5000);
		processTransmitNMT(M3_CAN_NODE_ID, 1);
		u_nCANstate 		= CAN_STATES_RUNNING;
		u_nCanConfigIndex	= 0;
		u_nDelay = 0;
	}
	else if(u_nDelay >= CAN_OFF_CHECKLOOP && SlaveM3PDOMAP[u_nCanConfigIndex].wDataSetSize < 0x4)
	{
		processTransmitSDOCInitDomainDownload(M3_CAN_NODE_ID,SlaveM3PDOMAP[u_nCanConfigIndex].wIndex,\
			SlaveM3PDOMAP[u_nCanConfigIndex].wSubIndex, SlaveM3PDOMAP[u_nCanConfigIndex].dwData, \
			SlaveM3PDOMAP[u_nCanConfigIndex].wDataSetSize); 
		u_nDelay			= 0;
		u_nCanConfigIndex++;
		u_nStepBak++;//2012/8/3 add
	}
	else u_nDelay++;
}


void	CANDataCheck()
{
	int		nbytes			= -1;
	long 	Tmp				= 0;
	struct	can_frame frame[CAN_RECV_BUFF];
	static	int  nRecvCount = 0;
	SDODATA	sdodata[2];
	int		nIndex=0, nSubIndex = 0, nDataState = 0;
	DWORD	dwData = 0;
	if(g_nErrorFlag!=0)
	{
		g_GetDataFlag=0;
	}
	if(g_nErrorFlag == 0 && u_nskt >= 0  && g_CanbusState == 2)
	{
		if(u_nskt_read >= 0)
		{
			nbytes = read(u_nskt_read, frame, sizeof(frame));
		}
		//if ((nbytes = read(u_nskt, &frame, sizeof(frame))) < 0) 
		if(nbytes <= 0)
		{
			//perror("read");
			return ;
		}
		else
		{
			//printf("nRecvCount=%d \n", nRecvCount);
			//printf("frame[0].can_id=%x \n", frame[0].can_id);
			#ifdef	D_SOCKETCAN
			for(int i = 0; i <= CAN_RECV_BUFF; i++)
			#else
			for(int i = 0; i < CAN_RECV_BUFF; i++)
			#endif
			{
				if(frame[i].can_id == 0 || frame[i].can_dlc == 0)
					continue;
				//printf("Rx FrameID=%x \n", frame[i].can_id&0x7FF);
				if(frame[i].can_id & CAN_ERR_FLAG)
				{
					g_nErrorFlag = frame[i].can_id & CAN_ERR_FLAG;
					continue;
				}
				//James mark for test 2013/11/22 16:05:19
				if(u_nCANstate != CAN_STATES_RUNNING)
				{
					if(frame[i].can_id>0x581 && frame[i].can_id < 0x5FF)//if(frame.can_id>0x601 && frame.can_id < 0x67F)	2012/8/3 modify
						u_nStep++;
					//return ;	
				}
				g_GetDataFlag |= 0x01;
				u_lnHeartbeatTime = M3_ONLINE_TIME;
				switch(frame[i].can_id&0x7FF)
				{
					case 0x180+M3_CAN_NODE_ID:
						memcpy(g_RxPDOBuffer[0].acData, frame[i].data, MAX_CANDATALEN);
						//memcpy(&g_dwTempM3Key[2], g_RxPDOBuffer[0].acData, 8);
						//if(memcmp(&g_dwM3OldKey[2], &g_dwTempM3Key[2], 8) != 0)
						//{
						//	//printf("===================g_dwM3Key[2]=%x g_dwM3Key[3]=%x\n",  g_dwTempM3Key[2], g_dwTempM3Key[3]);
						//	g_dwM3OldKey[2] = g_dwTempM3Key[2];
						//	g_dwM3OldKey[3] = g_dwTempM3Key[3];
						//	g_dwM3KeyCount[0] = 0;
						//}
						//else
						//{
						//	g_dwM3KeyCount[0]++;
						//	if(g_dwM3KeyCount[0] >= 1)
						//	{
						//		//memcpy(&g_dwM3Key[2], g_dwTempM3Key[2], 8);
						//		g_dwM3Key[2] = g_dwTempM3Key[2];
						//		g_dwM3Key[3] = g_dwTempM3Key[3];
						//	}
						//}
						
						//if(g_dwM3OldKey[3] != g_dwM3Key[3])
						//{
						//	printf("g_dwM3Key[3]=%x \n",  g_dwM3Key[3]);
						//	g_dwM3OldKey[3] = g_dwM3Key[3];
						//}
						memcpy(&g_dwM3Key[2], g_RxPDOBuffer[0].acData, 8);
						//printf("frame[%d].can_id=%x 2=%04x 3=%04x\n", i,frame[i].can_id, g_dwM3Key[2], g_dwM3Key[3]);
						//g_dwM3LED[0] = g_dwM3Key[2];
						//g_dwM3LED[1] = g_dwM3Key[3];
						//nRecvCount++;
						break;
					case 0x280+M3_CAN_NODE_ID:
						memcpy(g_RxPDOBuffer[1].acData, frame[i].data, MAX_CANDATALEN);
						//memcpy(&g_dwTempM3Key[0], g_RxPDOBuffer[1].acData, 8);
						//if(memcmp(&g_dwM3OldKey[0], &g_dwTempM3Key[0], 8) != 0)
						//{
						//	//printf("==========================g_dwM3Key[0]=%x g_dwM3Key[1]=%x\n",  g_dwTempM3Key[0], g_dwTempM3Key[1]);
						//	g_dwM3OldKey[0] = g_dwTempM3Key[0];
						//	g_dwM3OldKey[1] = g_dwTempM3Key[1];
						//	g_dwM3KeyCount[1] = 0;
						//}
						//else
						//{
						//	g_dwM3KeyCount[1]++;
						//	if(g_dwM3KeyCount[1] >= 1)
						//	{
						//		//memcpy(&g_dwM3Key[2], g_dwTempM3Key[2], 8);
						//		g_dwM3Key[0] = g_dwTempM3Key[0];
						//		g_dwM3Key[1] = g_dwTempM3Key[1];
						//	}
						//}
						//nRecvCount++;
						memcpy(&g_dwM3Key[0], g_RxPDOBuffer[1].acData, 8);
						//printf("frame[%d].can_id=%x 0=%04x 1=%04x\n", i,frame[i].can_id, g_dwM3Key[0], g_dwM3Key[1]);
						/*
						if(g_dwM3OldKey[0] != g_dwM3Key[0])
						{
							printf("g_dwM3Key[0]=%x \n",  g_dwM3Key[0]);
							g_dwM3OldKey[0] = g_dwM3Key[0];
						}
						if(g_dwM3OldKey[1] != g_dwM3Key[1])
						{
							printf("g_dwM3Key[1]=%x \n",  g_dwM3Key[1]);
							g_dwM3OldKey[1] = g_dwM3Key[1];
						}*/
						break;
					case 0x380+M3_CAN_NODE_ID:
						memcpy(g_RxPDOBuffer[2].acData, frame[i].data, MAX_CANDATALEN);
						break;
					case 0x480+M3_CAN_NODE_ID:
						memcpy(g_RxPDOBuffer[3].acData, frame[i].data, MAX_CANDATALEN);
						break;
					case 0x580+M3_CAN_NODE_ID:		//SDO Read
						memcpy(sdodata, frame[i].data, MAX_CANDATALEN);
						nIndex 		= ((sdodata[0].awData[1] & 0xFF) << 8) + ((sdodata[0].awData[0] & 0xFF00) >> 8);
						nSubIndex 	= ((sdodata[0].awData[1] & 0xFF00) >> 8);
						nDataState 	= sdodata[0].awData[0] & 0xFF;
						dwData		= sdodata[1].dwData;
						//Write to database 
						SetCANDatabase(nIndex, nSubIndex, nDataState&0xF, (char*)&dwData);
						break;
					case 0x700+M3_CAN_NODE_ID://heart beat
						u_nSlaveState = frame[i].data[0];
						u_lnHeartbeatCount++;
						break;
					default:
						break;
				}
		//		printf("frame.can_id=%x,  0=%d, 1=%d, 2=%d, 3=%d, 4=%d, 5=%d, 6=%d, 7=%d, 0=%d, 1=%d, 2=%d, 3=%d, 4=%d, 5=%d, 6=%d, 7=%d \n", frame.can_id, 
		//		g_RxPDOBuffer[0].acData[0], g_RxPDOBuffer[0].acData[1], g_RxPDOBuffer[0].acData[2], g_RxPDOBuffer[0].acData[3],
		//		g_RxPDOBuffer[0].acData[4], g_RxPDOBuffer[0].acData[5], g_RxPDOBuffer[0].acData[6], g_RxPDOBuffer[0].acData[7],
		//		g_RxPDOBuffer[1].acData[0], g_RxPDOBuffer[1].acData[1], g_RxPDOBuffer[1].acData[2], g_RxPDOBuffer[1].acData[3],
		//		g_RxPDOBuffer[1].acData[4], g_RxPDOBuffer[1].acData[5], g_RxPDOBuffer[1].acData[6], g_RxPDOBuffer[1].acData[7]);
				switch(u_nCommandId)
				{
					case CAN_COMMAND_TEST_AUTO:
						if(frame[i].can_id == (COBID_TXPDO1+CAN_ID))
						{
							memcpy(&Tmp, g_RxPDOBuffer[0].acData, sizeof(Tmp));
		
							u_nRecvAutoTestData++;
						}
						break;
					case CAN_COMMAND_TEST_NORMALL:
						break;
				}
			}
		}
	}
}

int		Can_Send()
{
	if(g_GetDataFlag==0x01)
	{
		g_GetDataFlag|=0x02;
		for(int i = 0 ; i < MAX_LEDINDEX; i++)
		{
				memcpy(g_TxPDOBuffer[0].acData, g_dwM3LED, sizeof(g_dwM3LED));
				g_dwM3LEDOld[i] = g_dwM3LED[i];	
				return 1;
		}
	}
	else
	{
		for(int i = 0 ; i < MAX_LEDINDEX; i++)
		{
			if(g_dwM3LED[i] != g_dwM3LEDOld[i])
			{
				memcpy(g_TxPDOBuffer[0].acData, g_dwM3LED, sizeof(g_dwM3LED));
				g_dwM3LEDOld[i] = g_dwM3LED[i];	
				return 1;
			}
		}
	}
	return 0;
}

void	CanTest()
{
	static	int i = 0;
	switch(u_nCommandId)
	{
		case CAN_COMMAND_TEST_AUTO :
			g_TxPDOBuffer[0].acData[6] = 0x00;	//2012/8/2
			memcpy(g_TxPDOBuffer[0].acData, &u_nSendAutoTestData, sizeof(u_nSendAutoTestData));
			memcpy(&g_TxPDOBuffer[0].acData[4], &u_nSendAutoTestData2, sizeof(u_nSendAutoTestData2));
			
			if(u_nCommandIdChange)
			{
				g_TxPDOBuffer[0].acData[6]	= 0xF1;
				u_nCommandIdChange			= 0;
			}
			if(i >= 64)
				i = 0;
			if( i >=32)
			{
				u_nSendAutoTestData = 0;
				u_nSendAutoTestData2 = (1 << (i-32));
			}
			else
			{
				u_nSendAutoTestData = (1 << i);
				u_nSendAutoTestData2 = 0;
			}
			//printf("u_nSendAutoTestData=%x 2=%x  i=%d \n", u_nSendAutoTestData, u_nSendAutoTestData2, i);
			i++;
			break;
		case CAN_COMMAND_TEST_NORMALL :
			memcpy(g_TxPDOBuffer[0].acData, &u_acSendNormalData, sizeof(u_acSendNormalData));
			if(u_nCommandIdChange)
			{
				g_TxPDOBuffer[0].acData[6]	= 0xF2;
				u_nCommandIdChange			= 0;
			}
			//printf("CanTest--NORMALL--Send data g_TxPDOBuffer[0].acData[6]=%X u_acSendNormalData:%d %d %d\n", (unsigned char)g_TxPDOBuffer[0].acData[6],u_acSendNormalData[0],u_acSendNormalData[1],u_acSendNormalData[2]);
				
			break;
	}
}

void 	*TX_TestTask(void *param)
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	    ;
	while(!shutdowntask)
	{
		if(u_lnHeartbeatTime < 0)
		{
			u_lnHeartbeatTime = M3_ONLINE_TIME;
			ReSetConfig();
		}
		
		switch(u_nCANstate) 
		{
			case CAN_STATES_CONFIG:
				CanConfigTask();
				break;
			case CAN_STATES_RUNNING:
				if(Can_Send())
				{
					processTransmitPDO(6, 1);
				}
				break;
			default :
				break;
		}      
		#ifdef  D_ARM
		usleep(20000);
		#else
		usleep(1000);
		#endif
	}
	return NULL;
}
//---------------------------------------------------------------------
void 	*RX_TestTask(void *param)
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	    ;
	while(!shutdowntask)
	{
		CANDataCheck();
		#ifdef  D_ARM
		usleep(20000);
		#else
		usleep(100);
		#endif
	}
	return NULL;
}

void 	*Check_TestTask(void *param)
{
	
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	    ;
	while(1)
	{
		#ifdef	D_SOCKETCAN
		#else
		if(u_nskt_read >=0)
		{
			u_wRegRS = ioctl(u_nskt_read, 0x1001, 0);
			u_wRegER = ioctl(u_nskt_read, 0x1002, 0);
			if((u_wRegRS & 0x40) || (u_wRegRS & 0x80))
			{
				g_CanbusState = 1;	
			}
			else
			{
				g_CanbusState = 2;	
			}
		}
		#endif
		if(g_nErrorFlag != 0)
		{
			if(g_nRestart == 0)
			{
				ReOpenSocketCan();
				SendMsg(MSG_SH_HMI_CAN_OFFLINE, 0, 0, NULL);
				printf("can off line \n");
			}
			usleep(500*1000);
			g_nErrorFlag = 0;
		}
		processTransmitNMT(M3_CAN_NODE_ID, 1);
		//sleep(1);
		usleep(500*1000);
	}
}

static void sighand(int sig)
{
	//printf("set shutdowntask = 1 !\n");
	shutdowntask = 1;
}

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
int		TaskCanMaster()
{
	#ifdef  D_ARM
	#else
	system("ip link set can0 down");
	usleep(500*1000);
	//system("ip link set can0 type can restart-ms 100");
	//usleep(500*1000);
	system("ip link set can0 up type can bitrate 500000");	//2012/8/1
	usleep(50*1000);										//2012/8/1
	#endif
	//sigset_t intmask;
	//int signum 				= SIGALRM;
	//sigemptyset(&intmask); 		/*將信號集合設置為空 */
	//sigaddset(&intmask,signum); /*加入中斷 Ctrl+C信號*/
	//sigprocmask(SIG_BLOCK, &intmask, NULL);
	
	pthread_mutex_init(&g_CANTxMutex, NULL);
	//signal(SIGINT, sighand);		2012/8/1 mark
	//signal(SIGTERM, sighand);		2012/8/1 mark
	
	return CreatSocketTask();
	//printf("CreatScketTask\n");
}

void	TaskCanMasterClose()
{
	DeleteTimer(&u_lnHeartbeatTime);
	//if(u_stat[0].thread!= 0)	pthread_cancel(u_stat[0].thread);
	//if(u_stat[1].thread!= 0)	pthread_cancel(u_stat[1].thread);
	if(u_nskt_read >= 0)
		close(u_nskt_read);
	if(u_nskt >= 0)
		close(u_nskt);
	pthread_mutex_destroy(&g_CANTxMutex);
	if(m_CanRxThreadID != 0)	pthread_cancel(m_CanRxThreadID);
	if(m_CanTxThreadID != 0)	pthread_cancel(m_CanRxThreadID);
	if(m_CanCHThreadID != 0)	pthread_cancel(m_CanCHThreadID);
}

int		CreatSocketTask()
{
	CreatSocketCan();
	
	pthread_attr_init(&m_ThreadRxAttr);
	pthread_attr_setdetachstate(&m_ThreadRxAttr, PTHREAD_CREATE_DETACHED);
	
	pthread_create(&m_CanTxThreadID, NULL, TX_TestTask, NULL/*&u_param[0]*/);
	pthread_create(&m_CanRxThreadID, NULL, RX_TestTask, NULL/*&u_param[1]*/);
	pthread_create(&m_CanCHThreadID, NULL, Check_TestTask, NULL/*&u_param[1]*/);
	return u_nskt;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void	SetSendData(int nIndex, char data)
{
	if(nIndex >= 0 && nIndex < 8)
	{
		u_acSendNormalData[nIndex]	= data;
		u_acOutPutData_Old[nIndex]	= data;//2012/8/2
	}
}

char	GetRecvData(int nIndex)
{
	if(nIndex >= 0 && nIndex < 8)
		return g_RxPDOBuffer[0].acData[nIndex];
	else 
		return 0;
}

int		GetErrorCount(int nType)
{
	switch(nType)
	{
		case CAN_GET_SEND_NO_COUNT:
			return u_nSendAutoTestData;
		case CAN_GET_HEARTBEAT_ERROR_COUNT:
			return u_nCanHeartbeatErrorCount;
		case CAN_GET_SLAVE_STATES:
			return u_nSlaveState;
		case CAN_GET_RECV_NO_COUNT:
			return u_nRecvAutoTestData;
		default:
			return 0;
	}
	return 0;
}

void	ReSetConfig()
{
	u_nSlaveState	= 0;
	u_nCANstate 	= CAN_STATES_CONFIG;
	if(g_nRestart == 0)
	{	
		g_nErrorFlag = CAN_ERR_FLAG; //ReOpenSocketCan();
	}
}

void	SetCommand(int nCommandId)
{
	u_nCommandId 		= nCommandId;
	u_nCommandIdChange	= 1; 
}

int		GetCommand()
{
	return u_nCommandId; 
}

int			Can_Write(struct can_frame* frame, int nsize)
{
	int	nResult = -1;
	pthread_mutex_lock(&g_CANTxMutex);
	#ifdef	D_SOCKETCAN
	if(frame != NULL && u_nskt >=0  && g_CanbusState == 2)
	#else
	if(frame != NULL && u_nskt >=0 && u_nskt_read >=0 && g_CanbusState == 2)
	#endif
		nResult = write(u_nskt_read, frame, nsize);
	pthread_mutex_unlock(&g_CANTxMutex);
	return nResult;	
}

// SDO client and server
void	processTransmitSDOCInitDomainUpload(WORD wNodeID,  WORD wIndex, BYTE cSubIndex)
{
	struct can_frame frame;
	SDOCMD *pSDOCMD 	= (SDOCMD *)(&frame.data[0]);
	
	memset(frame.data, 0, sizeof(frame.data));
	frame.can_id 		= SDOC_SENDFUNCID + wNodeID;
	pSDOCMD->bits.cmd	= SDOCMD_INITDOMAINUP;
	pSDOCMD->bits.n		= 2;
	frame.data[1]		= wIndex&0x00FF;
	frame.data[2]		= (wIndex&0xFF00)>>8;
	frame.data[3]		= cSubIndex;
	frame.can_dlc 		= 8;
	
	pthread_mutex_lock(&g_CANTxMutex);
	
	#ifdef	D_SOCKETCAN
	if(u_nskt >=0 && g_CanbusState == 2)
	#else
	if(u_nskt >=0 && u_nskt_read >=0 && g_CanbusState == 2)
	#endif
	{
		write(u_nskt_read, &frame, sizeof(frame));
	}
	pthread_mutex_unlock(&g_CANTxMutex);
}

void	processTransmitSDOCInitDomainDownload(WORD wNodeID, WORD wIndex, BYTE cSubIndex, DWORD dwValue, WORD wSize)
{
	struct can_frame frame;
	ssize_t		sError = 0;
	SDOCMD *pSDOCMD 	= (SDOCMD *)(&frame.data[0]);
	memset(frame.data, 0, sizeof(frame.data));
	
	switch(wSize)
	{
		case 3:
			pSDOCMD->bits.n	= 3;
			memcpy(&frame.data[4], &dwValue, 1);
			break;
		case 2:
			pSDOCMD->bits.n	= 2;
			memcpy(&frame.data[4], &dwValue, 2);
			break;
		case 1:
			pSDOCMD->bits.n	= 1;
			memcpy(&frame.data[4], &dwValue, 3);
			break;
		case 0:
			pSDOCMD->bits.n	= 0;
			memcpy(&frame.data[4], &dwValue, 4);
			break;
		default:
			return ;
	}
	
	frame.can_id		= SDOC_SENDFUNCID + wNodeID;
	pSDOCMD->bits.cmd	= SDOCMD_INITDOMAINDOWN;
	pSDOCMD->bits.e		= 1;
	pSDOCMD->bits.s		= 1;
	frame.data[1]		= wIndex&0x00FF;
	frame.data[2]		= (wIndex&0xFF00)>>8;
	frame.data[3]		= cSubIndex;
	frame.can_dlc 		= 8;
	
	pthread_mutex_lock(&g_CANTxMutex);
	if(u_nskt >=0 && u_nskt_read >=0 && g_CanbusState == 2)
		write(u_nskt_read, &frame, sizeof(frame));
	pthread_mutex_unlock(&g_CANTxMutex);
}

void	processTransmitPDO(WORD wNodeID, WORD wPDONum)
{
	if (wPDONum > MAX_PDO_COUNT) return;
	
	struct can_frame frame;
	memset(frame.data, 0, sizeof(frame.data));
	frame.can_id 		= g_TxPDOBuffer[wPDONum-1].wCOBID;
	memcpy(frame.data, g_TxPDOBuffer[wPDONum-1].acData, MAX_CANDATALEN);
	frame.can_dlc 		= g_TxPDOBuffer[wPDONum-1].wDataLength;
	
	pthread_mutex_lock(&g_CANTxMutex);
	if(u_nskt >=0  && u_nskt_read >=0 && g_CanbusState == 2)
		write(u_nskt_read, &frame, sizeof(frame));
	pthread_mutex_unlock(&g_CANTxMutex);
}


void	processTransmitNMT(WORD wNodeID, WORD wNMTCS)
{
	struct can_frame frame;
	ssize_t		sError = 0;
	memset(frame.data, 0, sizeof(frame.data));

	frame.can_id		= 0;
	frame.data[0]		= wNMTCS;
	frame.data[1]		= wNodeID;
	frame.can_dlc 		= 2;

	pthread_mutex_lock(&g_CANTxMutex);
	if(u_nskt >=0  && u_nskt_read >=0 && g_CanbusState == 2)
		sError = write(u_nskt_read, &frame, sizeof(frame));
	pthread_mutex_unlock(&g_CANTxMutex);
}

int		SetCANDatabase(WORD wIndex,  WORD wSubIndex, WORD wDataType, char*	pData)
{
	for(int i = 0; i < sizeof(g_CanDatabase)/sizeof(SDODATABASE); i++)
	{
		if(g_CanDatabase[i].wIndex == wIndex && 
			g_CanDatabase[i].wSubIndex == wSubIndex)
		{
			if(pData != NULL && g_CanDatabase[i].pDataAddr != NULL)
			{
				switch(g_CanDatabase[i].wDataSetSize)
				{
					case 0x3:			//byte 4
						memcpy(g_CanDatabase[i].pDataAddr, pData, 1);
						break;
					case 0x2:			//byte 3
						memcpy(g_CanDatabase[i].pDataAddr, pData, 2);
						break;
					case 0x1:			//byte 2
						memcpy(g_CanDatabase[i].pDataAddr, pData, 3);
						break;
					case 0x0:			//byte 1
						memcpy(g_CanDatabase[i].pDataAddr, pData, 4);
						break;	
					default:
						break;
				}	
			}
			return 1;
		}
	}
	return 0;
}
