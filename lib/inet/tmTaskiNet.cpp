/*===========================================================================+
|  Class    : TaskiNet                                                       |
|  Task     : Communication Service Routine source file                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Caozz                                                          |
|  Version  : V5.00                                                          |
|  Creation : 11/22/2011                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"tmTaskiNet.h"
#include 	<arpa/inet.h>
#include 	<linux/sockios.h>
#include	"../../main.h"
#include    "../../utils.h"
#include	"../../storage.h"
#include	"../../manufacture.h"
#include	"../../wnd.h"
#include	"../../tmdebug.h"
#include	"../../tmshmsg.h"
#include	"../../commonaction.h"
#include	"../../tmoper.h"
#include	"../../_kb.h"
#include	"../libControl.h"

IMPLEMENT_DYNCREATE(CtmTaskiNet, CTaskHost)
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define 	INET_TIMEER				100
#define		INET_TIME_NETONLINE		1000

#define 	CRC_32  0xEDB88320

//******
//		Protocol Type
//******
#define		PROTOCOL_TYPE_STATIC	0
#define		PROTOCOL_TYPE_DYNAMIC	1

#define		DATA_HMI				0
#define		DATA_INET				1

//******
//		iNet Encoder Type
//******

#define		INET_ENCODER_TYPE_ANSI			 	1		/*表示ANSI						*/
#define		INET_ENCODER_TYPE_UNICODE        	2		/*表示UNICODE；                 */
#define		INET_ENCODER_TYPE_UTF8           	3		/*表示UTF8；                    */
#define		INET_ENCODER_TYPE_UTF16          	4		/*表示UTF16；                   */
#define		INET_ENCODER_TYPE_UTF32          	5		/*表示UTF32；                   */
#define		INET_ENCODER_TYPE_CUSTOM_128     	128		/*128开始直到255为用户自定义范围*/


//******
//		iNet Trigger Type
//******
#define		INET_TRIGGER_TYPE_PARAID			1		/*根据Parameter ID而触发	*/
#define		INET_TRIGGER_TYPE_TIMER				2		/*表示根据时间，循环		*/
#define		INET_TRIGGER_TYPE_KEYDOWN			3		/*表示按键按下				*/
#define		INET_TRIGGER_TYPE_KEYUP				4		/*表示按键松开				*/
#define		INET_TRIGGER_TYPE_TOUCHDOWN			5		/*表示鼠标或触摸屏按下		*/
#define		INET_TRIGGER_TYPE_TOUCHMOVE			6		/*表示鼠标或触摸屏拖动		*/
#define		INET_TRIGGER_TYPE_TOUCHUP			7		/*表示鼠标或触摸屏松开		*/
#define		INET_TRIGGER_TYPE_DEVICE_PULLIN		8		/*外部存储设备插入			*/
#define		INET_TRIGGER_TYPE_DEVICE_PULLOUT	9		/*外部存储设备拔出			*/
//******
//		iNet Trigger Condition
//******
#define		INET_TRIGGER_CONDITION_EQUAL		1		/*等于    */
#define		INET_TRIGGER_CONDITION_GREATER		2		/*大于    */
#define		INET_TRIGGER_CONDITION_LESS			4		/*小于    */
#define		INET_TRIGGER_CONDITION_RISING		8		/*上升沿  */
#define		INET_TRIGGER_CONDITION_FALLING		16		/*下降沿  */
#define		INET_TRIGGER_CONDITION_AND			32		/*逻辑与  */
#define		INET_TRIGGER_CONDITION_OR			64		/*逻辑或  */
#define		INET_TRIGGER_CONDITION_NOT			128		/*逻辑非  */
#define		INET_TRIGGER_CONDITION_XOR			256		/*逻辑异或*/

//******
//		iNet DataType
//******
#define		INET_DATA_TYPE_NULL          		0x0001
#define		INET_DATA_TYPE_VARIANT       		0x0002
#define		INET_DATA_TYPE_BYTEBOOL      		0x0003
#define		INET_DATA_TYPE_WORDBOOL      		0x0004
#define		INET_DATA_TYPE_BYTE          		0x0005
#define		INET_DATA_TYPE_WORD          		0x0006
#define		INET_DATA_TYPE_DWORD         		0x0007
#define		INET_DATA_TYPE_UINT64        		0x0008
#define		INET_DATA_TYPE_INT16         		0x0009
#define		INET_DATA_TYPE_INT32         		0x000A
#define		INET_DATA_TYPE_INT64         		0x000B
#define		INET_DATA_TYPE_FLOAT         		0x000C
#define		INET_DATA_TYPE_DOUBLE        		0x000D
#define		INET_DATA_TYPE_CURRENCY      		0x000E
#define		INET_DATA_TYPE_ANSICHAR      		0x000F
#define		INET_DATA_TYPE_UNICODECHAR   		0x0010
#define		INET_DATA_TYPE_ANSISTRING    		0x0011
#define		INET_DATA_TYPE_UNICODESTRING 		0x0012
#define		INET_DATA_TYPE_ARRAY         		0x2000
#define		INET_DATA_TYPE_DATE          		0x0200
#define		INET_DATA_TYPE_TIME          		0x0201
#define		INET_DATA_TYPE_DATETIME      		0x0202
#define		INET_DATA_TYPE_IPV4          		0x0203
#define		INET_DATA_TYPE_IPV6          		0x0204
#define		INET_DATA_TYPE_MAC           		0x0205

//******
//		iNet Error ID
//******
#define		INET_ERROR_ID_MAX_SIZE	          	1
#define		INET_ERROR_ID_LENGTH         		2
#define		INET_ERROR_ID_VERSION	          	3
#define		INET_ERROR_ID_CRC		          	4
#define		INET_ERROR_ID_STATICVERSION       	5
#define		INET_ERROR_ID_MACERROR		       	6


//******
//		iNet Msg
//******
#define		INET_MSG_FORM_CREATE				11001
#define		INET_MSG_RECORDSET_SAVE				11002
#define		INET_MSG_MONI_ADD					11003
#define		INET_MSG_ERROR_ADD					11004
#define		INET_MSG_ERROR_FIX					11005
#define		INET_MSG_PUSHKEY					11006
#define		INET_MSG_LOGIN_REQUEST				11007
#define		INET_MSG_INJECT_DATA				11008
#define		INET_MSG_INJECTB_DATA				11009
#define		INET_MSG_SENDVERSION				11010
#define		INET_MSG_ETHNET_INIT				11011
#define		INET_MSG_OFFLINE					11012
#define		INET_MSG_TIMER						11013
#define		INET_MSG_SHOTCOUNT					11014
#define		INET_MSG_ONLINE						11015
#define		INET_MSG_REQ_MOLD_COUNT				11016
#define		INET_MSG_REQ_MOLD_LIST				11017
#define		INET_MSG_REQ_MOLD_SET				11018
#define		INET_MSG_REQ_MOLD_DEL				11019
#define		INET_MSG_MANU_PRODUCE_STATUS		11020
#define		INET_MSG_MANU_SCHEDULE_STATUS		11021
#define		INET_MSG_END_SYNCH					11022
#define		INET_MSG_USER_LOGINOUT				11026


#define		INET_NETINFOFILE_PATH				"/conf/iNetConf"
#define		HMI_PNSN_HARDWARE_PATH				"PNSN_hardware.txt"
#define		HMI_PNSN_SOFTWARE_PATH				"PNSN_software.txt"


//******
//		NetMask Local
//******
pthread_mutex_t			CtmTaskiNet::m_SendMutex;

BYTE	CtmTaskiNet::m_acLocalNetMask[CONST_LENGTH_IP]	= {255, 255, 0, 0};
BYTE	CtmTaskiNet::m_acICCIP[CONST_LENGTH_IP]			= {0, 0, 0, 0};

static	IOETH		m_ioeth =
{
	{192, 168, 10, 245},
	{192, 168, 10, 45},
	AF_INET,
	UDP_SRCPORT,
	UDP_DSTPORT
};


tmDealToiNetMsg	CtmTaskiNet::m_iNetDealToiNetMsg[] =
{
	{MSG_TIMER,							&CtmTaskiNet::DealTimer				},
	//{MSG_SH_INET_PARADATAID,			&CtmTaskiNet::DealParaDataID		},
	{MSG_SH_INET_RESTART,				&CtmTaskiNet::DealToiNetRestart		},
	{0, NULL}
};

tmCustomData	CtmTaskiNet::m_CustomData[]=
{
	{310001		, INET_DATA_TYPE_BYTE		},	//是否需要分配IP
	{310002		, INET_DATA_TYPE_IPV4		},	//本地IP地址
	{310003		, INET_DATA_TYPE_BYTE		},	//字符编码方式
	{310004		, INET_DATA_TYPE_DWORD		},	//协议错误ID
	{310005		, INET_DATA_TYPE_DATETIME	},	//时间(收到时间与发送时间)
	{310006		, INET_DATA_TYPE_DWORD		},	//同步时间标记
	{310007		, INET_DATA_TYPE_DWORD		},	//连线时间
	{310008		, INET_DATA_TYPE_DWORD		},	//静态协议版本
	{310009		, INET_DATA_TYPE_DWORD		},	//动态协议版本
	{310015		, INET_DATA_TYPE_DWORD		},	//iNet静态协议版本
	{310016		, INET_DATA_TYPE_DWORD		},	//iNet动态协议版本
	{310018		, INET_DATA_TYPE_DWORD		},	//主机类型
	{310019		, INET_DATA_TYPE_ANSISTRING	},	//PNSN_Hardware
	{310020		, INET_DATA_TYPE_ANSISTRING	},	//PNSN_Software
};

tmGetData	CtmTaskiNet::m_SendToiNet[] =
{
	{310001	        ,   &CtmTaskiNet::ComposeReqIPFlag		},
	{310002         ,   &CtmTaskiNet::ComposeIPAddress		},
	{310003         ,   &CtmTaskiNet::ComposeEncoder		},
	{310004         ,   &CtmTaskiNet::ComposeProError		},
	{310005         ,   &CtmTaskiNet::ComposeTimer			},
	{310006         ,   &CtmTaskiNet::ComposeTimerFlag		},
	{310007         ,   &CtmTaskiNet::ComposeOnline			},
	{310008         ,   &CtmTaskiNet::ComposeStaticVer		},
	{310009         ,   &CtmTaskiNet::ComposeDynamicVer		},
	{310010         ,   &CtmTaskiNet::ComposeDynamicList	},
	{310012         ,   &CtmTaskiNet::ComposeReadData		},
	{310013         ,   &CtmTaskiNet::ComposeWriteData		},
	{310014         ,   &CtmTaskiNet::ComposeDataDic		},
	{310015         ,   &CtmTaskiNet::ComposeiNetStaticVer	},
	{310016         ,   &CtmTaskiNet::ComposeiNetDynamicVer	},
	{310017         ,   &CtmTaskiNet::ComposeDataMap		},
	{310018         ,   &CtmTaskiNet::ComposeHostType		},
	{310019         ,   &CtmTaskiNet::ComposePNSN_Hardware	},
	{310020         ,   &CtmTaskiNet::ComposePNSN_Software	},
	{0xFFFFFFFF		,	NULL								}
};

tmSetData	CtmTaskiNet::m_RevFromiNet[]=
{
	{310002         ,   &CtmTaskiNet::ParseIPAddress		},
	{310005         ,   &CtmTaskiNet::ParseTimer			},
	{310006         ,   &CtmTaskiNet::ParseTimerFlag		},
	{310007         ,   &CtmTaskiNet::ParseOnline			},
	{310008         ,   &CtmTaskiNet::ParseStaticVer		},
	{310009         ,   &CtmTaskiNet::ParseDynamicVer		},
	{310010         ,   &CtmTaskiNet::ParseDynamicList		},
	{310011         ,   &CtmTaskiNet::ParseDynamicData		},
	{310012         ,   &CtmTaskiNet::ParseReadData			},
	{310013         ,   &CtmTaskiNet::ParseWriteData		},
	{310014         ,   &CtmTaskiNet::ParseDataDic			},
	{310015         ,   &CtmTaskiNet::ParseiNetStaticVer	},
	{310016         ,   &CtmTaskiNet::ParseiNetDynamicVer	},
	{310017         ,   &CtmTaskiNet::ParseDataMap			},
	{0xFFFFFFFF		,	NULL								}
};

tmRevDataPro	CtmTaskiNet::m_RevDataPro[] =
{
	{0x02000005     ,   &CtmTaskiNet::RevDataProFunc		},
	{0xFFFFFFFF		,	NULL								}
};


tmDataTypeMap 		CtmTaskiNet::m_DataTypeMap[] =
{
	{INET_DATA_TYPE_NULL         	, TYPE_UNKNOWN				, 1 	},
	{INET_DATA_TYPE_WORDBOOL     	, TYPE_BOOL					, 2 	},
	{INET_DATA_TYPE_BYTE         	, TYPE_BYTE					, 1 	},
	{INET_DATA_TYPE_WORD         	, TYPE_WORD					, 2 	},
	{INET_DATA_TYPE_DWORD        	, TYPE_DWORD				, 4 	},
	{INET_DATA_TYPE_INT16        	, TYPE_SHORT				, 2 	},
	{INET_DATA_TYPE_INT32        	, TYPE_INT					, 4 	},
	{INET_DATA_TYPE_FLOAT        	, TYPE_FLOAT				, 4 	},
	{INET_DATA_TYPE_DOUBLE       	, TYPE_DOUBLE				, 8 	},
	{INET_DATA_TYPE_ANSICHAR     	, TYPE_CHAR					, 1 	},
	{INET_DATA_TYPE_UNICODECHAR  	, TYPE_WORD					, 2 	},
	{INET_DATA_TYPE_ANSISTRING   	, TYPE_STRING				, 0 	},
	{INET_DATA_TYPE_UNICODESTRING	, TYPE_STRING				, 0 	},
	{INET_DATA_TYPE_VARIANT      	, INET_DATA_TYPE_VARIANT	, 16	},
	{INET_DATA_TYPE_UINT64       	, INET_DATA_TYPE_UINT64		, 8	 	},
	{INET_DATA_TYPE_INT64        	, INET_DATA_TYPE_INT64		, 8 	},
	{INET_DATA_TYPE_CURRENCY     	, INET_DATA_TYPE_CURRENCY	, 8 	},
	{INET_DATA_TYPE_ARRAY        	, INET_DATA_TYPE_ARRAY    	, 0 	},
	{INET_DATA_TYPE_DATE         	, INET_DATA_TYPE_DATE     	, 4 	},
	{INET_DATA_TYPE_TIME         	, INET_DATA_TYPE_TIME     	, 4 	},
	{INET_DATA_TYPE_DATETIME     	, INET_DATA_TYPE_DATETIME 	, 8 	},
	{INET_DATA_TYPE_IPV4         	, INET_DATA_TYPE_IPV4     	, 4 	},
	{INET_DATA_TYPE_IPV6         	, INET_DATA_TYPE_IPV6     	, 16	},
	{INET_DATA_TYPE_MAC          	, INET_DATA_TYPE_MAC      	, 6 	},
};


char*		CtmTaskiNet::m_pErrorStr[]=
{
	NULL,
	"Packet too big",				// INET_ERROR_ID_MAX_SIZE
	"Length error",                 // INET_ERROR_ID_LENGTH
	"Version not matched",          // INET_ERROR_ID_VERSION
	"CRC error",                    // INET_ERROR_ID_CRC
	"Static version error",         // INET_ERROR_ID_STATICVERSIO
	"MacAddress error",             // INET_ERROR_ID_MACERROR
};

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
extern	BOOL	g_biNetStatus;
extern	WORD	g_wiNetStatus;

extern	DWORD	g_dwInetSendCMD;
extern	DWORD	g_dwInetRevCMD;

extern	WORD	g_wInetMsgDataIDCount;
extern	DWORD	g_dwInetMsgDataIDList[512];

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
typedef void * (* PCHECKNETWORK)(void *);
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
int 	cmp_inet_datamap (const void *a , const void *b)
{
	return (((INetDataMap*)a)->dwInetDBID - ((INetDataMap*)b)->dwInetDBID);
}
int 	cmp_hmi_datamap (const void *a , const void *b)
{
	return (((INetDataMap*)a)->dwHmiDBID - ((INetDataMap*)b)->dwHmiDBID);
}

CtmTaskiNet::CtmTaskiNet() : CTaskHost()
{
	pthread_mutex_init(&m_SendMutex, NULL);
	memset(m_pacEtherName, 0, sizeof(m_pacEtherName));
	memset(_CRC32_Talbe, 0, sizeof(_CRC32_Talbe));

	m_handle			= -1;
	m_nLineStatus		= 0;

	RestartNet(NET_SETINITPARA);
	m_bReStart = TRUE;
	m_dwiNetTimer 			= 0;
	m_bEndSynch				= FALSE;

	m_byReadDataFlag = 1;
	m_bDynamicUpdateFlag = FALSE;

	memset(&m_CheckNetworkThreadID, 0, sizeof(m_CheckNetworkThreadID));

	int i = 0;
	while(m_iNetDealToiNetMsg[i].nMsg != 0)
	{
		RegisterMessage(m_iNetDealToiNetMsg[i].nMsg, m_idMe);
		i++;
	}

	m_liNetTimer = INET_TIMEER;
	AddCounter(&m_liNetTimer, m_idMe);

	m_lCheckIPSCounter		= 0;
	AddCounter(&m_lCheckIPSCounter, m_idMe);

	m_lCheckNetLineCounter 	= 0;
	AddCounter(&m_lCheckNetLineCounter, m_idMe);

	m_lCheckNetStatusCounter	= INET_TIME_NETONLINE;
	AddCounter(&m_lCheckNetStatusCounter, m_idMe);

	m_lOfflineCounter = 0x7FFFFFFF;
	AddCounter(&m_lOfflineCounter, m_idMe);

	AddCounter(&m_lSendCMDTimer, m_idMe);
	AddCounter(&m_lRevCMDTimer, m_idMe);

	//********
	//	Get current physical address
	//********
	GetRealPhysicalAddr(m_acRealPhysicalAddress);

	//********
	//	open ether device, create socket
	//********
	//OpenDevice();
	CreateCRC32Table();
	Init();

	pthread_create(&m_CheckNetworkThreadID, NULL, (PCHECKNETWORK)_CheckNetwork, this);
}

CtmTaskiNet::~CtmTaskiNet()
{
	int i = 0;
	while(m_iNetDealToiNetMsg[i].nMsg != 0)
	{
		UnRegisterMessage(m_iNetDealToiNetMsg[i].nMsg, m_idMe);
		i++;
	}
	if (m_handle != -1)	DelDevice(m_handle);


	DeleteCounter(&m_liNetTimer, m_idMe);
	DeleteCounter(&m_lCheckIPSCounter, m_idMe);
	DeleteCounter(&m_lCheckNetLineCounter, m_idMe);
	DeleteCounter(&m_lCheckNetStatusCounter, m_idMe);

	DeleteCounter(&m_lOfflineCounter, m_idMe);

	DeleteCounter(&m_lSendCMDTimer, m_idMe);
	DeleteCounter(&m_lRevCMDTimer, m_idMe);

	if (m_CheckNetworkThreadID != 0) pthread_cancel(m_CheckNetworkThreadID);

	pthread_mutex_destroy(&m_SendMutex);
}
int		CtmTaskiNet::CreateSelf()
{
	CTaskHost::CreateSelf();
	return 0;
}
int		CtmTaskiNet::FreeSelf()
{
	return CTaskHost::FreeSelf();
}
void	CtmTaskiNet::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	CTask::WndProc(message, wParam, lParam);

	int i = 0;

	while (m_iNetDealToiNetMsg[i].nMsg != 0)
	{
		if (m_iNetDealToiNetMsg[i].pMsgFunc != NULL && message == m_iNetDealToiNetMsg[i].nMsg)
		{
			(this->*(m_iNetDealToiNetMsg[i].pMsgFunc))(wParam, lParam);
			return;
		}
		i++;
	}
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

int    CtmTaskiNet::CheckNetwork()
{
	if (m_bReStart)
	{
		if(OpenDevice() == -1)
		{
			m_bReStart = TRUE;
		}
		else m_bReStart = FALSE;
	}

	//=================================
	//		check physical device
	//=================================
	if (m_handle == -1)
	{
		m_bReStart = TRUE;
		OpenDevice();
		if (m_handle == -1)
		{
			m_bReStart = TRUE;
			m_nLineStatus |= INET_PHYSICAL_DEVICE_NO_READY;
			return m_nLineStatus;
		}
		else
		{
			m_nLineStatus &= (~INET_PHYSICAL_DEVICE_NO_READY);
			m_bReStart = FALSE;
		}
	}
	else	m_nLineStatus &= (~INET_PHYSICAL_DEVICE_NO_READY);
	//=================================
	//		check physical online(open and close eth0 for test)
	//=================================

	if(GetLinkStatus() == DEVICE_S_OPENED)
	{
		m_nLineStatus &= (~INET_PHYSICAL_ONLINE_NO_READY);
	}
	else
	{
		m_nLineStatus |= INET_PHYSICAL_ONLINE_NO_READY;
		return m_nLineStatus;
	}

	if (!m_bInitFlag)
	{
		OpenDevice();
		m_nLineStatus |= INET_INIT_NO_READY;
		return m_nLineStatus;
	}
	else							//	on line
		m_nLineStatus &= (~INET_INIT_NO_READY);

	//printf("CheckNetwork  m_bInitIP = %d\n", m_bInitIP);
	if (!m_bInitIP)
	{
		m_nLineStatus |= INET_INIT_IPADDRESS;
		return m_nLineStatus;
	}
	else							//	on line
		m_nLineStatus &= (~INET_INIT_IPADDRESS);

	//=================================
	//	inet server  on line status
	//=================================
	if (m_lCheckIPSCounter <= 0) 	//	not on line
	{
		OpenDevice();
		m_nLineStatus |= INET_INETSERVER_ONLINE_NO_READY;
	}
	else							//	on line
		m_nLineStatus &= (~INET_INETSERVER_ONLINE_NO_READY);

	return m_nLineStatus;
}

void _CheckNetwork(CtmTaskiNet* piNet)
{
	pthread_detach(pthread_self());
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{

	}

	while(1)
	{
		piNet->CheckNetwork();
		usleep(1000*1000);
	}
}

/*------------------------------------------------------------------+
|	Run()															|
+------------------------------------------------------------------*/
void    CtmTaskiNet::Run()
{
	if(m_lCheckNetLineCounter <= 0)
	{
		if (m_nLineStatus == 0)
		{
			g_biNetStatus = TRUE;
			g_wiNetStatus |= 1;

		}
		else
		{
			g_wiNetStatus = 0;
			g_biNetStatus = FALSE;
		}
		//printf("*********************************m_nLineStatus = %d\n", m_nLineStatus);
		if(m_nLineStatus == 0)	//netowrk is ok
		{
			//printf("Run  netowrk is ok\n");
			m_lCheckNetLineCounter 	= INET_TIME_NETONLINE;
		}
		//	can't open the device
		else if ((m_nLineStatus & INET_PHYSICAL_DEVICE_NO_READY) ||
				(m_nLineStatus & INET_PHYSICAL_ONLINE_NO_READY))
		{
			//printf("Run  can't open the device\n");
			m_lCheckNetLineCounter 	= INET_TIME_NETONLINE;
			return;
		}
		//	don't get the ip & time from the iNetServer
		else if (m_nLineStatus & INET_INIT_NO_READY  )
		{
			Send(INET_SEND_STATIC_VERSION);
			//printf("Run  send static version\n");
			m_lCheckNetLineCounter 	= INET_TIME_NETONLINE;
			return;
		}
		else if (m_nLineStatus & INET_INIT_IPADDRESS  )
		{
			Send(INET_SEND_STATIC_REQIP);
			//printf("Run  send ip address \n");
			m_lCheckNetLineCounter 	= INET_TIME_NETONLINE;
			return;
		}
		//	don't get the echo
		else if (m_nLineStatus & INET_INETSERVER_ONLINE_NO_READY)
		{
			m_lCheckNetLineCounter 	= INET_TIME_NETONLINE;
			RestartNet(NET_RESTARTNET);
			return;
		}
		else return;
	}
	if(m_lCheckNetStatusCounter <= 0)
	{
		SendMsg(MSG_SH_INET_SEND_STATUS, m_nLineStatus, 0, NULL);	//Send net status
		m_lCheckNetStatusCounter = INET_TIME_NETONLINE;
	}


	if(m_lSendCMDTimer < 0)
	{
		g_dwInetSendCMD = 0;
	}

	if(m_lRevCMDTimer < 0)
	{
		g_dwInetRevCMD = 0;
	}
	return;
}

void ParseiNetData(char *pdata, int nlength)
{
 	int  i = 0;
 	DWORD	dwCRC = 0;
 	//printf("ParseiNetData  nlength = %d\n", nlength);

 	if (g_ptaskinet == NULL) return;
 	CtmTaskiNet*	pNet 	= (CtmTaskiNet*)g_ptaskinet;

 	NETFRAMERX* 	pTemp 	= &pNet->m_frameRx;
 	memcpy(pTemp, pdata, nlength);

 	if(nlength >= MAX_ETHTOKEN_SIZE)
 	{
 		pNet->m_dwProErrorID = INET_ERROR_ID_MAX_SIZE;
 		pNet->Send(INET_SEND_STATIC_ERROR);
 		return;
 	}

 	if (pTemp->netPro.wVersion != pNet->m_wProtocolVer)
 	{
 		pNet->m_dwProErrorID = INET_ERROR_ID_VERSION;
 		pNet->Send(INET_SEND_STATIC_ERROR);
 		return;
 	}

 	if (memcmp(pTemp->netPro.byMAC, pNet->m_acRealPhysicalAddress, sizeof(pTemp->netPro.byMAC)) != 0)
 	{
 		pNet->m_dwProErrorID = INET_ERROR_ID_MACERROR;
 		pNet->Send(INET_SEND_STATIC_ERROR);
 		return;
 	}


 	memcpy(&dwCRC, pdata+nlength-4, sizeof(dwCRC));

 	//printf("iNetCRC = %08X, HMICRC = %08X\n", dwCRC, pNet->GetCRC32(pdata, nlength - sizeof(dwCRC)));

 	if(dwCRC !=  pNet->GetCRC32(pdata, nlength - sizeof(dwCRC)))
 	{
 		pNet->m_dwProErrorID = INET_ERROR_ID_CRC;
 		pNet->Send(INET_SEND_STATIC_ERROR);
 		return;
 	}


 	while (i < pNet->m_nStaticRevNum)
 	{
 		if (pNet->m_pStaticRevFile[i].dwProID == pTemp->netPro.dwProID)
 			break;
 		i++;
 	}

 	pNet->m_lRevCMDTimer = 3000;
 	g_dwInetRevCMD = pNet->m_pStaticRevFile[i].dwProID;

 	///printf("pNet->m_pStaticRevFile[%d].dwProID = %08x\n", i, pNet->m_pStaticRevFile[i].dwProID);

 	BYTE	byControlFLag = 0;
 	BYTE	byPackStamp = 0;
 	DWORD	dwProID = 0;
 	if (i < pNet->m_nStaticRevNum)
 	{
 		byControlFLag = pTemp->netPro.byFlag;

 		byPackStamp = pTemp->netPro.byStamp;

 		pNet->SetData(i, (char*)pTemp->netData.acBuffer, nlength - sizeof(pTemp->netPro), PROTOCOL_TYPE_STATIC);

 		int j = 0;
 		while (pNet->m_RevDataPro[j].dwProID != 0xFFFFFFFF)
		{
			if (pNet->m_pStaticRevFile[i].dwProID == pNet->m_RevDataPro[j].dwProID)
			{
				if (pNet->m_RevDataPro[j].pDealFunc != NULL)
				{
					(pNet->*(pNet->m_RevDataPro[j].pDealFunc))((char*)pTemp, i);
				}
			}
			j++;
		}
		//printf("byControlFLag = %x, byControlFLag >> 6 = %d\n", byControlFLag, byControlFLag >> 6);
		if(byControlFLag >> 6 == 1)
 		{
 			dwProID = (pNet->m_pStaticRevFile[i].dwProID | 0x01000000) & 0x01FFFFFF;
 			pNet->SetProtocolStamp(dwProID, byPackStamp);
 			pNet->Send(dwProID);
 		}

 	}

}

void	CtmTaskiNet::ParseThread_CleanupFunc(void *arg)
{
	 pthread_mutex_unlock(&m_SendMutex);
}

int		CtmTaskiNet::RestartNet(short nflag)
{
	BYTE			acIP[CONST_LENGTH_IP];
	int				nStatus;
	if ((m_handle != -1) && (nflag == NET_RESTARTNET))	CloseDevice();
	m_bInitFlag			= FALSE;
	m_bInitIP			= FALSE;
	//********
	//	Set local ip, ip[3] must be 250
	//********

	ReqValues(CONST_REQ_LOCALNETINFO, &m_iNetInfo, sizeof(m_iNetInfo));

	char  cEth0[20] = "eth0:0", cEth1[20] = "eth1:0";
	if(CtmConfig::GetInstance()->GetINETMacSwtich() ==0)		//Sunny<20080816>
		strcpy(m_pacEtherName,cEth0);
	else if(CtmConfig::GetInstance()->GetINETMacSwtich() ==1)
		strcpy(m_pacEtherName,cEth1);
	else strcpy(m_pacEtherName,cEth0);

	m_ioeth.acDstAddress[0] = m_iNetInfo.acServerIP[0];
	m_ioeth.acDstAddress[1] = m_iNetInfo.acServerIP[1];
	m_ioeth.acDstAddress[2] = m_iNetInfo.acServerIP[2];
	m_ioeth.acDstAddress[3] = m_iNetInfo.acServerIP[3];

	m_ioeth.acSrcAddress[0] = m_iNetInfo.acLocalIP[0];
	m_ioeth.acSrcAddress[1] = m_iNetInfo.acLocalIP[1];
	m_ioeth.acSrcAddress[2] = m_iNetInfo.acLocalIP[2];
	m_ioeth.acSrcAddress[3] = m_iNetInfo.acLocalIP[3];

	m_ioeth.wSrcPort        = m_iNetInfo.wsrcPort;
	m_ioeth.wDstPort		= m_iNetInfo.wdstPort;

	if(m_iNetInfo.wDHCPFlag == 1) // Static IP
	{
		m_acLocalNetMask[0] = m_iNetInfo.acNetMaskIP[0];
		m_acLocalNetMask[1] = m_iNetInfo.acNetMaskIP[1];
		m_acLocalNetMask[2] = m_iNetInfo.acNetMaskIP[2];
		m_acLocalNetMask[3] = m_iNetInfo.acNetMaskIP[3];
		SetLocalIP(m_ioeth.acSrcAddress);
	}
	else if(m_iNetInfo.wDHCPFlag == 0)   //Dhcp
	{
		m_acLocalNetMask[0] = 255;
		m_acLocalNetMask[1] = 255;
		m_acLocalNetMask[2] = 255;
		m_acLocalNetMask[3] = 0;
		nStatus = GetLocalIP(acIP);
		if ((nStatus < 0) || (acIP[3] != 250) ||
			(memcmp(acIP, m_ioeth.acDstAddress, 3) != 0))
		{
			memcpy(m_ioeth.acSrcAddress, m_ioeth.acDstAddress, 3);
			m_ioeth.acSrcAddress[3] = 250;
			SetLocalIP(m_ioeth.acSrcAddress);
		}
		else SetLocalIP(acIP);
	}

	m_bUpdateFlag		= FALSE;
	m_bInitFlag			= FALSE;
	m_bInitIP			= FALSE;

	m_nLineStatus	= 	INET_PHYSICAL_DEVICE_NO_READY	|
						INET_PHYSICAL_ONLINE_NO_READY	|
						INET_INIT_NO_READY			|
						INET_INIT_IPADDRESS			|
						INET_INETSERVER_ONLINE_NO_READY;


	return	0;
}
void	CtmTaskiNet::SendToiNet(WORD wDataLength, DWORD dwIDData)
{
	//pthread_mutex_lock(&m_SendMutex1);

   	//m_frameTx.netPro.dwDataID		= dwIDData;
    //SendData(m_handle, (char *)&m_frameTx, sizeof(NETPRO)+wDataLength);

	//pthread_mutex_unlock(&m_SendMutex1);
}

void	CtmTaskiNet::SendOneOffData()
{
	Send(INET_SEND_STATIC_DATAMAP);
}


/*---------------------------------------------------------------------------+
|  Function :  OpenDevice()								                     |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int 	CtmTaskiNet::OpenDevice()
{
	memset(&m_ethernet, 0, sizeof(m_ethernet));
	m_ethernet.srcAddr = m_ioeth.acSrcAddress[0] | m_ioeth.acSrcAddress[1] << 8 |
				   m_ioeth.acSrcAddress[2] << 16 | m_ioeth.acSrcAddress[3] << 24;
	m_ethernet.dstAddr = m_ioeth.acDstAddress[0] | m_ioeth.acDstAddress[1] << 8 |
				   m_ioeth.acDstAddress[2] << 16 | m_ioeth.acDstAddress[3] << 24;
	m_ethernet.wsrcPort			= m_ioeth.wSrcPort;
	m_ethernet.wdstPort			= m_ioeth.wDstPort;
	m_ethernet.nDomain 			= m_ioeth.cProtocol;
	m_ethernet.nType   			= SOCK_DGRAM;
	m_ethernet.wRxBufferSize 	= MAX_ETOKEN_SIZE;  			// Size of the receive buffer
	m_ethernet.fpRxBuffer	 	= m_acRxiNetBuffer;     		// Far pointer to the receive buffer
	m_ethernet.wTxBufferSize 	= MAX_ETOKEN_SIZE;  			// Size of the transmit buffer
	m_ethernet.fpTxBuffer	 	= m_acTxiNetBuffer;     		// Far pointer to the transmit buffer
	//********
	//	get ip from ips, then use instant style
	//********
	m_ethernet.NetFunc 		= (NETFUNC)ParseiNetData;

  	//m_ethernet.RxThread_CleanupFunc = ParseThread_CleanupFunc;
	//printf("inet RxThread_CleanupFunc = %x\n", m_ethernet.RxThread_CleanupFunc);

	//********
	//	Open device
	//********
	CloseDevice();
	m_handle = AddDevice("CEthernet", &m_ethernet);
	//printf("m_handle = %d\n", m_handle);

	if(m_handle != -1)
	{
		return	GetState(m_handle);
	}
	else	return	-1;
}
int		CtmTaskiNet::CloseDevice()
{
	pthread_mutex_lock(&m_SendMutex);
	if(m_handle != -1)	DelDevice(m_handle);
	m_handle = -1;
	pthread_mutex_unlock(&m_SendMutex);
	return	0;
}
int		CtmTaskiNet::GetLocalIP(BYTE *pIP)
{
	return	GetNetInformation(pIP, CONST_LENGTH_IP);
}
/*---------------------------------------------------------------------------+
|  Function :  GetNetInformation(BYTE *pdata, int nflag)                     |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskiNet::GetNetInformation(BYTE *pdata, int nflag)
{
	register 	int 	fd, intrface;
	struct 		ifreq 	buf[8];
	struct 		ifconf 	ifc;
	int					i;
	DWORD				dwIP;
	unsigned int		dwCommand;
	int					nResult = 0;
	struct sockaddr_in*	psocketaddr;
	if (pdata == NULL)			return -1;
	if (nflag == CONST_LENGTH_IP)		dwCommand = SIOCGIFADDR;
	else if(nflag == CONST_LENGTH_MAC)	dwCommand = SIOCGIFHWADDR;
	else						return -1;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
	{
		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = (caddr_t)buf;
		if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
		{
			intrface = ifc.ifc_len / sizeof(struct ifreq);
			while (intrface-- > 0)
			{
				if (strcmp(buf[intrface].ifr_name, m_pacEtherName) != 0)	continue;	//Sunny<20080816>
				//if (strcmp(buf[intrface].ifr_name, /*"lo"*/"eth1") !=/*==*/ 0)	continue;
				//if (strcmp(buf[intrface].ifr_name, "lo") == 0)	continue;
				if (!(ioctl(fd, dwCommand, (char *)&buf[intrface])))
				{
					// get current ip
					if (nflag == CONST_LENGTH_IP)
					{
						psocketaddr = (struct sockaddr_in*)(&buf[intrface].ifr_addr);
						memcpy((char *)&dwIP ,
								(char *)&(psocketaddr->sin_addr), CONST_LENGTH_IP);
						for(i=0; i<CONST_LENGTH_IP; i++)
							*(pdata+i) = (BYTE)(dwIP >> (i * 8));
					}
					// get current physical address
					if (nflag == CONST_LENGTH_MAC)
					{
						memcpy((char *)pdata,
							(char *)buf[intrface].ifr_hwaddr.sa_data, CONST_LENGTH_MAC);
					}
				}
				else
				{
					nResult = -1;
					if (nflag == CONST_LENGTH_IP);
						//printf ("get ip error: %s\n", strerror(errno));
					if(nflag == CONST_LENGTH_MAC);
						//printf ("get mac error: %s\n", strerror(errno));
				}
			}
		}
		close(fd);
		return	nResult;
	}
	else	return -1;
}
/*---------------------------------------------------------------------------+
|  Function : SetLocalIP(BYTE *pIP)                                          |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskiNet::SetLocalIP(BYTE *pIP)
{
	char	acNetIPMask[80];
	if (pIP == NULL)	return -1;
	memcpy(m_ioeth.acSrcAddress, pIP, CONST_LENGTH_IP);

	sprintf(acNetIPMask, "ifconfig %s %d.%d.%d.%d netmask %d.%d.%d.%d",
							m_pacEtherName,*pIP, *(pIP+1), *(pIP+2), *(pIP+3),
							m_acLocalNetMask[0], m_acLocalNetMask[1],
							m_acLocalNetMask[2], m_acLocalNetMask[3]);	//Sunny<20080816>
	usleep(200*1000);
	System(acNetIPMask);
	printf("iNet:acNetIPMask=%s \n", acNetIPMask);

	if(m_iNetInfo.wDHCPFlag == 1 && memcmp(pIP, m_iNetInfo.acServerIP, 3) != 0)
	{
		sprintf(acNetIPMask, "route add -net %d.%d.%d.0 netmask %d.%d.%d.%d gw %d.%d.%d.%d",
							m_iNetInfo.acServerIP[0], m_iNetInfo.acServerIP[1],
							m_iNetInfo.acServerIP[2],
							m_acLocalNetMask[0], m_acLocalNetMask[1],
							m_acLocalNetMask[2], m_acLocalNetMask[3],
							m_iNetInfo.acGatway[0], m_iNetInfo.acGatway[1],
							m_iNetInfo.acGatway[2], m_iNetInfo.acGatway[3]);
		usleep(200*1000);
		printf("iNet:acNetIPMask=%s \n", acNetIPMask);
		System(acNetIPMask);
	}

	return	0;
}
int    CtmTaskiNet::GetLinkStatus()
{
	struct ifreq ifrInfo;
	struct eth_link ethData;
	int	   ckfd = -1;

    if ((ckfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        //printf("socket error\n");
        return -1;
    }
	memset(&ifrInfo, 0, sizeof(ifrInfo));
	ethData.cmd = 0x0000000A;
	strcpy(ifrInfo.ifr_name, m_pacEtherName);	//Sunny<20080816>
	//strcpy(ifrInfo.ifr_name, "eth1");//eth0
	ifrInfo.ifr_data = (char *)&ethData;
	if (ioctl(ckfd, SIOCETHTOOL, &ifrInfo) == -1)
	{
	    //printf("Ehernet failed: %s\n", strerror(errno));
	    close(ckfd);
	    return -1;
	}
	close(ckfd);
	return (ethData.data ? DEVICE_S_OPENED : DEVICE_E_FAILURE);
}
int		CtmTaskiNet::ReadFile(char* pszFileName, char*  pszBuffer, int nLength, int nOffset)
{
	int nRealLength = 0;

	if(m_File.Open(pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
	{
		m_File.Seek(nOffset, SEEK_SET);
		nRealLength = m_File.Read(pszBuffer, nLength);
		m_File.Close();
		return nRealLength;
	}
	else
		return -1;
}
int		CtmTaskiNet::WriteFile(char* pszFileName, char*  pszBuffer, int nLength, int nOffset)
{
	int nRealLength = 0;

	if(m_File.Open(pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
	{
		m_File.Seek(nOffset, SEEK_SET);
		nRealLength = m_File.Write(pszBuffer, nLength);
		m_File.Close();
		return nRealLength ;
	}
	else
		return -1;
}

void	CtmTaskiNet::Init()
{
	InitDataMap();
	InitStaticProtocol();
	InitDynamicProtocol();
}

void	CtmTaskiNet::InitStaticProtocol()
{
	char*	pszFileName = NULL;
	char*	pszDir = NULL;
	CIniFile ReadIni;

	if(ReadIni.OpenFile("outerconfig.ini"))
	{
		ReadIni.ReadString("iNet","ProtocolConf", 	NULL, &pszFileName);
		if(pszFileName != NULL)
		{
			if(ReadIni.OpenFile(pszFileName))
			{
				m_wProtocolVer = ReadIni.ReadInteger("Protocol","Version",0);
				m_dwStaticVer = ReadIni.ReadInteger("Static","Version", 0);

				ReadIni.ReadString("Static","ProtocolListDir", 		NULL, &pszDir);
				//printf("InitStaticProtocol  m_wProtocolVer = %x, m_dwStaticVer = %x, pszDir = %s\n",
				//						m_wProtocolVer, m_dwStaticVer, pszDir);

				if(pszDir != NULL)
				{
					m_pStaticSendFile = GerProtocolDataInfo(pszDir, "SendList.ini", &m_nStaticSendNum);
					m_pStaticRevFile = GerProtocolDataInfo(pszDir, "RevList.ini", &m_nStaticRevNum);
					//printf("m_pStaticRevFile = %p, m_nStaticRevNum = %d\n", m_pStaticRevFile, m_nStaticRevNum);
				}

			}
		}
	}
}

void	CtmTaskiNet::InitDynamicProtocol()
{
	char*	pszFileName = NULL;
	char*	pszDir = NULL;
	CIniFile ReadIni;
	char	szFileName[64];
	memset(szFileName, 0, sizeof(szFileName));

	memset(m_piNetStartMsg , 0 ,sizeof(m_piNetStartMsg));
	m_niNetStartMsgCount =0 ;
	memset(m_piNetEndMsg , 0 ,sizeof(m_piNetEndMsg));
	m_niNetEndMsgCount =0 ;

	if(ReadIni.OpenFile("outerconfig.ini"))
	{
		ReadIni.ReadString("iNet","ProtocolConf", 	NULL, &pszFileName);
		//printf("InitDynamicProtocol  pszFileName = %s\n", pszFileName);
		if(pszFileName != NULL)
		{
			if(ReadIni.OpenFile(pszFileName))
			{
				m_dwDynamicVer = ReadIni.ReadInteger("Dynamic","Version", 0);
				ReadIni.ReadString("Dynamic", "ProtocolListDir", NULL, &pszDir);
				//printf("InitDynamicProtocol  m_wProtocolVer = %x, m_dwDynamicVer = %x, pszDir = %s\n",
				//						m_wProtocolVer, m_dwDynamicVer, pszDir);

				strcpy(m_szDynamicDir, pszDir);
				if(pszDir != NULL)
				{
					m_pDynamicProtocol = GerDynamicProtocolDataInfo(pszDir, "Pro.list", &m_wDynamicProNum);

					//printf("m_wDynamicProNum = %d, m_pDynamicProtocol = %p\n", m_wDynamicProNum, m_pDynamicProtocol);
					for(int i = 0; i < m_wDynamicProNum; i++)
					{
						//printf("InitDynamicProtocol  ID = %08x, Version = %x\n", m_pDynamicProtocol[i].dwProID, m_pDynamicProtocol[i].wProVer);
						sprintf(szFileName, "%s%08x.msg", pszDir, m_pDynamicProtocol[i].dwProID);
						//printf("InitDynamicProtocol  szFileName = %s\n", szFileName);
						GetMsgDataInfo(szFileName, &m_pDynamicProtocol[i]);
					}
				}
			}
		}
	}

	qsort(g_dwInetMsgDataIDList, g_wInetMsgDataIDCount, sizeof(DWORD),cmp_dword);
}

void	CtmTaskiNet::InitDataMap()
{
	int nOffset = 0;
	CIniFile ReadIni;
	char*	pszFileName = NULL;

	if(ReadIni.OpenFile("outerconfig.ini"))
	{
		ReadIni.ReadString("iNet","DataMap", 	NULL, &pszFileName);
		//printf("InitDataMap  pszFileName = %s\n", pszFileName);
		if(pszFileName != NULL)
		{
			ReadFile(pszFileName, (char*)&m_dwDataMapVer, sizeof(m_dwDataMapVer), nOffset);
			nOffset += sizeof(m_dwDataMapVer);

			ReadFile(pszFileName, (char*)&m_nInetDataCount, sizeof(m_nInetDataCount), nOffset);
			nOffset += sizeof(m_nInetDataCount);
			//printf("InitDataMap  m_dwDataMapVer = %08x, m_nInetDataCount = %d\n", m_dwDataMapVer, m_nInetDataCount);

			if(m_nInetDataCount > 0)
			{
				if(m_InetDataMap != NULL) delete [] m_InetDataMap;
				if(m_HmiDataMap != NULL) delete [] m_HmiDataMap;

				m_InetDataMap = new INetDataMap[m_nInetDataCount];
				m_HmiDataMap = new INetDataMap[m_nInetDataCount];


				ReadFile(pszFileName, (char*)m_InetDataMap, sizeof(INetDataMap)*m_nInetDataCount, nOffset);
				nOffset += sizeof(INetDataMap)*m_nInetDataCount;
				memcpy(m_HmiDataMap, m_InetDataMap, sizeof(INetDataMap)*m_nInetDataCount);

				qsort(m_InetDataMap, m_nInetDataCount, sizeof(INetDataMap), cmp_inet_datamap);
				qsort(m_HmiDataMap, m_nInetDataCount, sizeof(INetDataMap), cmp_hmi_datamap);

				for(int i = 0; i < m_nInetDataCount; i++)
				{
					;//printf("InitDataMap  m_InetDataMap[%d], iNetID = %08x, HMIID = %08x\n",
					//		m_InetDataMap[i].dwInetDBID, m_InetDataMap[i].dwHmiDBID);
				}
			}
		}
	}

}

tmiNetDataFile*		CtmTaskiNet::GerProtocolDataInfo(char* pszDir, char* pszFileName, int* pnNum)
{
	char 			szTemp[64];
	CIniFile  		ReadIni;
	DWORD*			pProID = NULL;
	tmiNetDataFile*	iNetDataFile = NULL;
	sprintf(szTemp, "%s%s", pszDir, pszFileName);

	int nNum = GetDataListNum(szTemp);
	//printf("szTemp = %s\n", szTemp);
	*pnNum = nNum;

	if(nNum != 0)
	{
		iNetDataFile = new tmiNetDataFile[nNum];
		memset(iNetDataFile, 0, sizeof(tmiNetDataFile) * nNum);

		pProID = new DWORD[nNum];
		memset(pProID, 0, sizeof(DWORD) * nNum);

		GetDataList(szTemp, pProID, nNum);

		for(int i = 0; i < nNum; i++)
		{
			iNetDataFile[i].dwProID = pProID[i];
			//printf("GerProtocolDataInfo   iNetDataFile[%d].dwProID = %08x\n", i, iNetDataFile[i].dwProID);

			sprintf(szTemp, "%s%08X.ini", pszDir, pProID[i]);

			iNetDataFile[i].wProVer = GetProtocolVersion(szTemp);
			iNetDataFile[i].byControlFlag = GetProtocolControlFlag(szTemp);

			iNetDataFile[i].wDataNum = GetDataListNum(szTemp);
			//printf("GerProtocolDataInfo  DBID file = %s, iNetDataFile[i].wDataNum = %d\n", szTemp, iNetDataFile[i].wDataNum);
			iNetDataFile[i].pDBID	 = new DWORD[iNetDataFile[i].wDataNum];

			GetDataList(szTemp, iNetDataFile[i].pDBID, iNetDataFile[i].wDataNum);

		}

		delete [] pProID;
	}

	return iNetDataFile;
}

int		CtmTaskiNet::GetDataListNum(char* pszFileName)
{
	CIniFile  	ReadIni;
	int			nNum = 0;


	if(ReadIni.OpenFile(pszFileName))
	{
		nNum = ReadIni.ReadInteger("List","Count", 0);
	}

	return nNum;
}

int		CtmTaskiNet::GetProtocolVersion(char* pszFileName)
{
	CIniFile  	ReadIni;
	int			nVersion = 0;


	if(ReadIni.OpenFile(pszFileName))
	{
		nVersion = ReadIni.ReadInteger("List","Version", 0);
	}

	return nVersion;
}

int		CtmTaskiNet::GetProtocolControlFlag(char* pszFileName)
{
	CIniFile  	ReadIni;
	int			nControlFlag = 0;


	if(ReadIni.OpenFile(pszFileName))
	{
		nControlFlag = ReadIni.ReadInteger("List","ControlFlag", 0);
	}

	return nControlFlag;
}

void	CtmTaskiNet::GetDataList(char* pszFileName, DWORD*	pData, int nNum)
{
	CIniFile  	ReadIni;
	char		szTemp[64];

	if(ReadIni.OpenFile(pszFileName))
	{
		for(int i = 0; i < nNum; i++)
		{
			sprintf(szTemp, "ID%d", i);
			pData[i] = ReadIni.ReadInteger("List",szTemp, 0);
			//printf("GetDataList  %s = %08x, %d\n", szTemp, pData[i], pData[i]);
		}
	}
}

tmiNetDataFile*		CtmTaskiNet::GerDynamicProtocolDataInfo(char* pszDir, char* pszFileName, WORD* pwNum)
{
	char 			szTemp[64];
	int 			nOffset = 0;
	DWORD*			pProID = NULL;
	tmiNetDataFile*	iNetDataFile = NULL;
	sprintf(szTemp, "%s%s", pszDir, pszFileName);

	WORD wNum = 0;

	ReadFile(szTemp, (char*)(&wNum), sizeof(wNum), nOffset);
	nOffset += sizeof(wNum);
	*pwNum = wNum;
	//printf("GerDynamicProtocolDataInfo nNum = %d, szTemp = %s\n", wNum, szTemp);
	if(wNum != 0)
	{
		iNetDataFile = new tmiNetDataFile[wNum];
		memset(iNetDataFile, 0, sizeof(tmiNetDataFile) * wNum);

		pProID = new DWORD[wNum];
		memset(pProID, 0, sizeof(DWORD) * wNum);

		ReadFile(szTemp, (char*)(pProID), sizeof(DWORD) * wNum, nOffset);
		nOffset += sizeof(DWORD) * wNum;

		for(int i = 0; i < wNum; i++)
		{
			iNetDataFile[i].dwProID = pProID[i];
			//printf("GerDynamicProtocolDataInfo   iNetDataFile[%d].dwProID = %08x\n", i, iNetDataFile[i].dwProID);

			sprintf(szTemp, "%s%08X.dbid", pszDir, pProID[i]);
			nOffset = 0;
			ReadFile(szTemp, (char*)(&iNetDataFile[i].wProVer), sizeof(iNetDataFile[i].wProVer), nOffset);
			nOffset += sizeof(iNetDataFile[i].wProVer);

			ReadFile(szTemp, (char*)(&iNetDataFile[i].wDataNum), sizeof(iNetDataFile[i].wDataNum), nOffset);
			nOffset += sizeof(iNetDataFile[i].wDataNum);

			//printf("GerDynamicProtocolDataInfo  DBID file = %s, iNetDataFile[i].wDataNum = %d\n", szTemp, iNetDataFile[i].wDataNum);
			iNetDataFile[i].pDBID	 = new DWORD[iNetDataFile[i].wDataNum];

			ReadFile(szTemp, (char*)(iNetDataFile[i].pDBID), sizeof(DWORD)*iNetDataFile[i].wDataNum, nOffset);
			nOffset += sizeof(DWORD)*iNetDataFile[i].wDataNum;
			for(int j = 0; j < iNetDataFile[i].wDataNum;j++)
				;//printf("GerDynamicProtocolDataInfo j = %d, ID = %d\n", j, iNetDataFile[i].pDBID[j]);

		}

		delete [] pProID;
	}

	return iNetDataFile;
}

void	CtmTaskiNet::SetDynamicProtocolList(tmiNetDynamicList* pDynamicList, WORD wCount)
{
	int		nOffset = 0;
	char	szFileName[128];
	memset(szFileName, 0, sizeof(szFileName));

	if(m_szDynamicDir != NULL && m_szDynamicDir[0] != '\0')
	{
		sprintf(szFileName, "%sPro.list", m_szDynamicDir);
		WriteFile(szFileName, (char*)&wCount, sizeof(wCount), nOffset);
		nOffset += sizeof(wCount);

		for(int i = 0; i < wCount; i++)
		{
			WriteFile(szFileName, (char*)(&pDynamicList[i].dwProID), sizeof(pDynamicList[i].dwProID), nOffset);
			nOffset += sizeof(pDynamicList[i].dwProID);
		}
	}
}

void	CtmTaskiNet::SetDynamicDBID(DWORD dwProID, WORD wVersion, WORD wDBCount, DWORD* pDBIDs)
{
	int		nOffset = 0;
	char	szFileName[128];
	memset(szFileName, 0, sizeof(szFileName));

	if(m_szDynamicDir != NULL && m_szDynamicDir[0] != '\0')
	{
		sprintf(szFileName, "%s%08X.dbid", m_szDynamicDir, dwProID);
		WriteFile(szFileName, (char*)(&wVersion), sizeof(wVersion), nOffset);
		nOffset += sizeof(wVersion);

		WriteFile(szFileName, (char*)(&wDBCount), sizeof(wDBCount), nOffset);
		nOffset += sizeof(wDBCount);

		WriteFile(szFileName, (char*)pDBIDs, sizeof(DWORD)*wDBCount, nOffset);
		nOffset += sizeof(DWORD)*wDBCount;

	}
}

void	CtmTaskiNet::SetDynamicMsg(DWORD dwProID, char* pMsg, int nOffset)
{
	//int		nOffset = 0;
	char	szFileName[128];
	memset(szFileName, 0, sizeof(szFileName));

	WORD	wStartMsgNum = 0;
	WORD	wEndMsgNum = 0;

	if(m_szDynamicDir != NULL && m_szDynamicDir[0] != '\0')
	{
		sprintf(szFileName, "%s%08X.msg", m_szDynamicDir, dwProID);

		memcpy(&wStartMsgNum, pMsg+nOffset, sizeof(wStartMsgNum));

		//printf("SetDynamicMsg  wStartMsgNum = %d, nOffset = %d\n", wStartMsgNum, nOffset);
		WriteFile(szFileName, (char*)(&wStartMsgNum), sizeof(wStartMsgNum), nOffset);
		nOffset += sizeof(wStartMsgNum);

		if(wStartMsgNum > 0)
		{
			nOffset += SetDynamicMsg(dwProID, pMsg, wStartMsgNum, nOffset);
		}

		memcpy(&wEndMsgNum, pMsg+nOffset, sizeof(wEndMsgNum));
		//printf("SetDynamicMsg  wEndMsgNum = %d, nOffset = %d\n", wEndMsgNum, nOffset);

		WriteFile(szFileName, (char*)(&wEndMsgNum), sizeof(wEndMsgNum), nOffset);
		nOffset += sizeof(wEndMsgNum);

		if(wEndMsgNum > 0)
		{
			nOffset += SetDynamicMsg(dwProID, pMsg, wEndMsgNum, nOffset);
		}
	}
}

int		CtmTaskiNet::SetDynamicMsg(DWORD dwProID, char* pMsg, WORD wMsgNum, int nOffset)
{
	//int		nOffset = 0;
	char	szFileName[128];
	memset(szFileName, 0, sizeof(szFileName));

	WORD	wMsgID = 0;
	WORD	wMsgParaNum = 0;
	DWORD	dwDBID = 0;
	DWORD	dwCondition = 0;
	WORD	wDataType = 0;
	WORD	wUOM = 0;
	WORD	wLength = 0;

	char	szValue[8];
	if(m_szDynamicDir != NULL && m_szDynamicDir[0] != '\0')
	{
		sprintf(szFileName, "%s%08X.msg", m_szDynamicDir, dwProID);

		for(int i = 0; i < wMsgNum; i++)
		{
			memcpy(&wMsgID, pMsg+nOffset, sizeof(wMsgID));
			WriteFile(szFileName, (char*)(&wMsgID), sizeof(wMsgID), nOffset);
			nOffset += sizeof(wMsgID);
			//printf("SetDynamicMsg   wMsgID = %x\n", wMsgID);
			memcpy(&wMsgParaNum, pMsg+nOffset, sizeof(wMsgParaNum));
			WriteFile(szFileName, (char*)(&wMsgParaNum), sizeof(wMsgParaNum), nOffset);
			nOffset += sizeof(wMsgParaNum);
			//printf("SetDynamicMsg   wMsgParaNum = %x\n", wMsgParaNum);
			if(wMsgParaNum > 0)
			{
				for(int j = 0; j < wMsgParaNum; j++)
				{
					memcpy(&dwDBID, pMsg+nOffset, sizeof(dwDBID));
					WriteFile(szFileName, (char*)(&dwDBID), sizeof(dwDBID), nOffset);
					nOffset += sizeof(dwDBID);
					//printf("SetDynamicMsg for  j = %d   dwDBID = %x\n", j, dwDBID);
					memcpy(&dwCondition, pMsg+nOffset, sizeof(dwCondition));
					WriteFile(szFileName, (char*)(&dwCondition), sizeof(dwCondition), nOffset);
					nOffset += sizeof(dwCondition);
					//printf("SetDynamicMsg for  j = %d   dwCondition = %x\n", j, dwCondition);

					memcpy(&wDataType, pMsg+nOffset, sizeof(wDataType));
					WriteFile(szFileName, (char*)(&wDataType), sizeof(wDataType), nOffset);
					nOffset += sizeof(wDataType);
					//printf("SetDynamicMsg for  j = %d   wDataType = %d\n", j, wDataType);

					memcpy(&wUOM, pMsg+nOffset, sizeof(wUOM));
					WriteFile(szFileName, (char*)(&wUOM), sizeof(wUOM), nOffset);
					nOffset += sizeof(wUOM);
					//printf("SetDynamicMsg for  j = %d   wUOM = %d\n", j, wUOM);

					if(wDataType == INET_DATA_TYPE_ANSISTRING || wDataType == INET_DATA_TYPE_UNICODESTRING)
					{
						memcpy(&wLength, pMsg+nOffset, sizeof(wLength));
						WriteFile(szFileName, (char*)(&wLength), sizeof(wLength), nOffset);
						nOffset += sizeof(wLength);
					}
					else
					{
						wLength = GetiNetDataSize(wDataType);
						WriteFile(szFileName, (char*)(&wLength), sizeof(wLength), nOffset);
					}

					//nOffset += sizeof(wLength);

					WriteFile(szFileName, pMsg+nOffset, wLength, nOffset+2);
					nOffset += wLength;
				}
			}
		}
	}

	return nOffset;
}

void	CtmTaskiNet::GetMsgDataInfo(char* pszFileName, tmiNetDataFile* piNetDataFile)
{
	int		nOffset = 0;

	//printf("pszFileName = %s\n", pszFileName);

	ReadFile(pszFileName, (char*)(&(piNetDataFile->wStartMSGNum)), sizeof(piNetDataFile->wStartMSGNum), nOffset);
	nOffset += sizeof(piNetDataFile->wStartMSGNum);

	//printf("GetMsgDataInfo  piNetDataFile->wStartMSGNum = %d, nOffset = %d\n", piNetDataFile->wStartMSGNum, nOffset);

	if(piNetDataFile->wStartMSGNum > 0)
	{
		piNetDataFile->pStartiNetMsg = new tmiNetMsg[piNetDataFile->wStartMSGNum];
		memset(piNetDataFile->pStartiNetMsg, 0, sizeof(tmiNetMsg)*piNetDataFile->wStartMSGNum);

		nOffset += GetMsgDataInfo(pszFileName, piNetDataFile->pStartiNetMsg, piNetDataFile->wStartMSGNum, nOffset);

		m_niNetStartMsgCount = AddMsgPro(piNetDataFile->pStartiNetMsg, piNetDataFile->wStartMSGNum, piNetDataFile->dwProID,
										 m_piNetStartMsg, m_niNetStartMsgCount);
	}

	ReadFile(pszFileName, (char*)(&(piNetDataFile->wEndMSGNum)), sizeof(piNetDataFile->wEndMSGNum), nOffset);
	nOffset += sizeof(piNetDataFile->wEndMSGNum);

	//printf("GetMsgDataInfo  piNetDataFile->wEndMSGNum = %d, nOffset = %d\n", piNetDataFile->wEndMSGNum, nOffset);
	if(piNetDataFile->wEndMSGNum > 0)
	{
		piNetDataFile->pEndiNetMsg = new tmiNetMsg[piNetDataFile->wEndMSGNum];
		memset(piNetDataFile->pEndiNetMsg, 0, sizeof(tmiNetMsg)*piNetDataFile->wEndMSGNum);

		nOffset += GetMsgDataInfo(pszFileName, piNetDataFile->pEndiNetMsg, piNetDataFile->wEndMSGNum, nOffset);

		m_niNetEndMsgCount = AddMsgPro(piNetDataFile->pEndiNetMsg, piNetDataFile->wEndMSGNum, piNetDataFile->dwProID,
										 m_piNetEndMsg, m_niNetEndMsgCount);

		//printf("GetMsgDataInfo  m_niNetEndMsgCount = %d\n", m_niNetEndMsgCount);
		for(int i = 0; i < m_niNetEndMsgCount; i++)
		{
			//printf("GetMsgDataInfo m_piNetEndMsg[%d].iNetMsg->wMsgID = %x, ProIDCount = %d\n",
			//		i, m_piNetEndMsg[i].iNetMsg->wMsgID, m_piNetEndMsg[i].iNetMsg->wParaCount);
			for(int j = 0; j < m_piNetEndMsg[i].iNetMsg->wParaCount; j++)
			{
				;//printf("GetMsgDataInfo m_piNetEndMsg[%d].pProIDs[%d] = %x\n", i, j, m_piNetEndMsg[i].pProIDs[j]);
			}
		}
	}
}

int		CtmTaskiNet::GetMsgDataInfo(char* pszFileName, tmiNetMsg* piNetMsg, WORD wiNetMsgNum, int nOffset)
{
	//int nOffset = 0;
	//printf("GetMsgDataInfo   wiNetMsgNum = %d\n", wiNetMsgNum);
	for(int i = 0; i < wiNetMsgNum; i++)
	{
		ReadFile(pszFileName, (char*)(&(piNetMsg[i].wMsgID)), sizeof(piNetMsg[i].wMsgID), nOffset);
		nOffset += sizeof(piNetMsg[i].wMsgID);
		//printf("GetMsgDataInfo   i = %d, piNetMsg[i].wMsgID = %x\n", i, piNetMsg[i].wMsgID);

		ReadFile(pszFileName, (char*)(&(piNetMsg[i].wParaCount)), sizeof(piNetMsg[i].wParaCount), nOffset);
		nOffset += sizeof(piNetMsg[i].wParaCount);
		//printf("GetMsgDataInfo   i = %d, piNetMsg[i].wParaCount = %x\n", i, piNetMsg[i].wParaCount);

		if(piNetMsg[i].wParaCount > 0)
		{
			piNetMsg[i].pMsgPara = new tmMsgPara[piNetMsg[i].wParaCount];
			memset(piNetMsg[i].pMsgPara, 0, sizeof(tmMsgPara)*piNetMsg[i].wParaCount);
			for(int j = 0; j < piNetMsg[i].wParaCount; j++)
			{
				//ReadFile(pszFileName, (char*)(&(piNetMsg[i].pMsgPara[j])), sizeof(piNetMsg[i].pMsgPara[j]), nOffset);
				//nOffset += sizeof(piNetMsg[i].pMsgPara[j]);

				ReadFile(pszFileName, (char*)(&(piNetMsg[i].pMsgPara[j].dwDataID)), sizeof(piNetMsg[i].pMsgPara[j].dwDataID), nOffset);
				nOffset += sizeof(piNetMsg[i].pMsgPara[j].dwDataID);
				//printf("	GetMsgDataInfo   i = %d, j = %d, piNetMsg[i].pMsgPara[j].dwDataID = %x\n", i, j, piNetMsg[i].pMsgPara[j].dwDataID);

				ReadFile(pszFileName, (char*)(&(piNetMsg[i].pMsgPara[j].dwCondition)), sizeof(piNetMsg[i].pMsgPara[j].dwCondition), nOffset);
				nOffset += sizeof(piNetMsg[i].pMsgPara[j].dwCondition);
				//printf("	GetMsgDataInfo   i = %d, j = %d, piNetMsg[i].pMsgPara[j].dwCondition = %x\n", i, j, piNetMsg[i].pMsgPara[j].dwCondition);

				ReadFile(pszFileName, (char*)(&(piNetMsg[i].pMsgPara[j].wDataType)), sizeof(piNetMsg[i].pMsgPara[j].wDataType), nOffset);
				nOffset += sizeof(piNetMsg[i].pMsgPara[j].wDataType);
				//printf("	GetMsgDataInfo   i = %d, j = %d, piNetMsg[i].pMsgPara[j].wDataType = %x\n", i, j, piNetMsg[i].pMsgPara[j].wDataType);

				ReadFile(pszFileName, (char*)(&(piNetMsg[i].pMsgPara[j].wUOM)), sizeof(piNetMsg[i].pMsgPara[j].wUOM), nOffset);
				nOffset += sizeof(piNetMsg[i].pMsgPara[j].wUOM);
				//printf("	GetMsgDataInfo   i = %d, j = %d, piNetMsg[i].pMsgPara[j].wUOM = %x\n", i, j, piNetMsg[i].pMsgPara[j].wUOM);

				ReadFile(pszFileName, (char*)(&(piNetMsg[i].pMsgPara[j].wLength)), sizeof(piNetMsg[i].pMsgPara[j].wLength), nOffset);
				nOffset += sizeof(piNetMsg[i].pMsgPara[j].wLength);
				//printf("	GetMsgDataInfo   i = %d, j = %d, piNetMsg[i].pMsgPara[j].wLength = %x\n", i, j, piNetMsg[i].pMsgPara[j].wLength);

				if(piNetMsg[i].pMsgPara[j].wDataType == INET_DATA_TYPE_ANSISTRING ||
				   piNetMsg[i].pMsgPara[j].wDataType == INET_DATA_TYPE_UNICODESTRING)
				{
					piNetMsg[i].pMsgPara[j].pcData = new char[piNetMsg[i].pMsgPara[j].wLength];
					memset(piNetMsg[i].pMsgPara[j].pcData, 0, piNetMsg[i].pMsgPara[j].wLength);

					ReadFile(pszFileName, piNetMsg[i].pMsgPara[j].pcData, piNetMsg[i].pMsgPara[j].wLength, nOffset);
					nOffset += piNetMsg[i].pMsgPara[j].wLength;
				}
				else
				{
					ReadFile(pszFileName, piNetMsg[i].pMsgPara[j].acData, piNetMsg[i].pMsgPara[j].wLength, nOffset);
					nOffset += piNetMsg[i].pMsgPara[j].wLength;
				}

			}
		}
	}

	return nOffset;
}

int		CtmTaskiNet::ComposeData(char* pOut, DWORD* pDBID, WORD wDataNum, BYTE byIDFlag)
{
	int nResult = 0;
	DBVALUE	dbValue;
	memset(&dbValue, 0, sizeof(dbValue));
	WORD 	wPriLen	= 0;
	BOOL	bBreak = FALSE;
	DWORD	dwDBID = 0;
	INetDataMap*	piNetDataMap = NULL;

	if(byIDFlag == DATA_INET)
	{
		memcpy(pOut + nResult, &m_byReadDataFlag, sizeof(m_byReadDataFlag));
		nResult += sizeof(m_byReadDataFlag);

		if(m_byReadDataFlag == 1)
		{
			nResult += ComposeSchema(pOut + nResult, pDBID, wDataNum, byIDFlag);
		}

		memcpy(pOut + nResult, &wDataNum, sizeof(wDataNum));
		nResult += sizeof(wDataNum);
	}

	for (int i = 0; i < wDataNum; i++)
	{
		if(byIDFlag == DATA_INET)
		{
			dwDBID = pDBID[i];
			piNetDataMap = GetiNetDBMapFromiNetDBID(pDBID[i]);
			if(piNetDataMap != NULL)
				dwDBID = piNetDataMap->dwHmiDBID;
		}
		else
			dwDBID = pDBID[i];

		int j = 0;
		bBreak = FALSE;
		//printf("pDBID[i] = %d, %x\n", dwDBID, dwDBID);
		while (m_SendToiNet[j].dwCmd != 0xFFFFFFFF)
		{
			if (dwDBID == m_SendToiNet[j].dwCmd)
			{
				if (m_SendToiNet[j].pDealFunc != NULL)
				{
					int nFuncResult = (this->*(m_SendToiNet[j].pDealFunc))(pOut + nResult, i);
					if (nFuncResult < 0) return -1;
					nResult += nFuncResult;
					bBreak = TRUE;
				}
				break;
			}
			j++;
		}

		if(!bBreak)
		{
			int 	nIndex = g_pDatabase->DataIDToIndex(dwDBID);
			//printf("nIndex = %d\n", nIndex);
			if (nIndex < 0)
			{
				wPriLen = 1;
				memcpy((pOut + nResult), dbValue.acData, wPriLen);
				nResult += wPriLen;
				continue;
			}

			dbValue = g_pDatabase->Read(nIndex);
			wPriLen =  dbValue.DataType.wLength;

			if (dbValue.DataType.wType != TYPE_STRING)
			{
				if(dbValue.DataType.wType == TYPE_DOUBLE)
				{
					double dTemp = 0.0;
					#ifdef	D_PXA270
						dTemp = Pxa270DBConvert(dbValue.dData);
					#else
						dTemp = dbValue.dData;
					#endif
					memcpy((pOut + nResult), &dTemp, wPriLen);
				}
				else
					memcpy((pOut + nResult), dbValue.acData, wPriLen);
			}
			else
			{
				wPriLen = strlen(dbValue.pcData);
				memcpy((pOut + nResult), &wPriLen, sizeof(wPriLen));
				nResult += sizeof(wPriLen);
				strncpy((char *)(pOut + nResult), dbValue.pcData, wPriLen);
			}

			nResult += wPriLen;
		}
	}

	return nResult;
}


int		CtmTaskiNet::ComposeSchema(char* pOut, DWORD* pDBID, WORD wDataNum, BYTE byIDFlag)
{
	int 	nResult = 0;

	DBVALUE	dbValue;
	DWORD	dwDBID = 0;
	int 	nDBIndex = -1;
	WORD	wDataType = INET_DATA_TYPE_NULL;
	WORD	wDataUOM = 0;
	BYTE	byDataPrecision = 0;
	BOOL	bCustomData = FALSE;
	INetDataMap*	piNetDataMap = NULL;

	memcpy(pOut + nResult, &wDataNum, sizeof(wDataNum));
	nResult += sizeof(wDataNum);

	for(int i = 0; i < wDataNum; i++)
	{
		memcpy(pOut + nResult, &pDBID[i], sizeof(DWORD));
		nResult += sizeof(DWORD);

		if(byIDFlag == DATA_INET)
		{
			dwDBID = pDBID[i];
			piNetDataMap = GetiNetDBMapFromiNetDBID(pDBID[i]);
			if(piNetDataMap != NULL)
				dwDBID = piNetDataMap->dwHmiDBID;
		}
		else
			dwDBID = pDBID[i];

		wDataType = GetCustomDataType(dwDBID);
		//printf("ComposeSchema dwDBID = %x, GetCustomDataType(dwDBID) = %d\n", dwDBID, GetCustomDataType(dwDBID));
		if(wDataType != 0xFFFF)
		{
			memcpy((pOut + nResult), &wDataType, sizeof(wDataType));
			nResult += sizeof(wDataType);

			wDataUOM = 0;
			memcpy((pOut + nResult), &wDataUOM, sizeof(wDataUOM));
			nResult += sizeof(wDataUOM);

			byDataPrecision = 0;
			memcpy((pOut + nResult), &byDataPrecision, sizeof(byDataPrecision));
			nResult += sizeof(byDataPrecision);
		}
		else
		{
			nDBIndex = g_pDatabase->DataIDToIndex(dwDBID);
			//printf("ComposeSchema nDBIndex = %d\n", nDBIndex);
			if (nDBIndex < 0)
			{
				wDataType = INET_DATA_TYPE_NULL;
				memcpy((pOut + nResult), &wDataType, sizeof(wDataType));
				nResult += sizeof(wDataType);

				wDataUOM = 0;
				memcpy((pOut + nResult), &wDataUOM, sizeof(wDataUOM));
				nResult += sizeof(wDataUOM);

				byDataPrecision = 0;
				memcpy((pOut + nResult), &byDataPrecision, sizeof(byDataPrecision));
				nResult += sizeof(byDataPrecision);

			}
			else
			{
				INetDBInfo iNetDBInfo;
				iNetDBInfo = GetiNetDBInfo(nDBIndex, piNetDataMap);

				memcpy((pOut + nResult), &iNetDBInfo.wType, sizeof(iNetDBInfo.wType));
				nResult += sizeof(iNetDBInfo.wType);

				memcpy((pOut + nResult), &iNetDBInfo.wUOM, sizeof(iNetDBInfo.wUOM));
				nResult += sizeof(iNetDBInfo.wUOM);

				memcpy((pOut + nResult), &iNetDBInfo.byPrecision, sizeof(iNetDBInfo.byPrecision));
				nResult += sizeof(iNetDBInfo.byPrecision);
			}
		}
	}

	return nResult;
}

int		CtmTaskiNet::SetData(int nID, char* pTemp, int nLen, BYTE	byProtocolType)
{
	int nResult = 0;
	int	nOffSet	= 0;
	BOOL	bBreak = FALSE;
	tmiNetDataFile* piNetDataFile = NULL;

	if(byProtocolType == PROTOCOL_TYPE_STATIC)
		piNetDataFile = m_pStaticRevFile;

	for (int i = 0; i < piNetDataFile[nID].wDataNum; i++)
	{
		int j = 0;
		bBreak = FALSE;
		while (m_RevFromiNet[j].dwCmd != 0xFFFFFFFF)
		{
			//printf("piNetDataFile[nID].pDBID[i] = %x\n", piNetDataFile[nID].pDBID[i]);
			if (piNetDataFile[nID].pDBID[i] == m_RevFromiNet[j].dwCmd)
			{
				if (m_RevFromiNet[j].pDealFunc != NULL)
				{
					int nFuncResult = (this->*(m_RevFromiNet[j].pDealFunc))(pTemp + nOffSet, nLen - nOffSet);
					if (nFuncResult < 0) return 0;
					nOffSet += nFuncResult;
					nResult++;
					bBreak = TRUE;
				}
				break;
			}
			j++;
		}
		if(!bBreak)
		{
			;
		}

		if (nOffSet >= nLen) 	break;

	}
	return nResult;
}

int		CtmTaskiNet::Send(DWORD dwProID, BYTE	byProtocolType)
{
	int nResult = 0;
	WORD wPackLen = 0;
	DWORD	dwCRC = 0;
	BYTE 	byIDFlag = DATA_HMI;

	pthread_mutex_lock(&m_SendMutex);

	int nID = -1;
	nID = GetProtocolIndex(dwProID, byProtocolType);
	//printf("###############Send   nID = %d, %08x\n", nID, dwProID);
	if(nID != -1)
	{
		memset(m_frameTx.netData.acBuffer, 0, sizeof(m_frameTx.netData.acBuffer));
		tmiNetDataFile*	piNetDataFile = NULL;
		if(byProtocolType == PROTOCOL_TYPE_STATIC)
		{
			piNetDataFile = m_pStaticSendFile;
		}
		else
		{
			byIDFlag = DATA_INET;
			piNetDataFile = m_pDynamicProtocol;
		}

		wPackLen = ComposeData((char*)m_frameTx.netData.acBuffer,
								piNetDataFile[nID].pDBID,
								piNetDataFile[nID].wDataNum, byIDFlag) +
							   sizeof(NETPRO) + sizeof(dwCRC);

		m_frameTx.netPro.wLength = wPackLen;
		m_frameTx.netPro.wVersion = m_wProtocolVer;
		m_frameTx.netPro.byStamp = piNetDataFile[nID].byPackStamp++;
		m_frameTx.netPro.byFlag = piNetDataFile[nID].byControlFlag;
		m_frameTx.netPro.dwProID = piNetDataFile[nID].dwProID;

		memcpy(m_frameTx.netPro.byMAC, m_acRealPhysicalAddress, sizeof(m_acRealPhysicalAddress));

		dwCRC = GetCRC32((char*)&m_frameTx, wPackLen - sizeof(dwCRC));

		memcpy(((char*)&m_frameTx)+wPackLen-sizeof(dwCRC), &dwCRC, sizeof(dwCRC));

		SendData(m_handle, (char *)&m_frameTx, wPackLen);

		m_lSendCMDTimer = 3000;
		g_dwInetSendCMD = piNetDataFile[nID].dwProID;
		//printf("Send id=%lx, nsize=%d\n", piNetDataFile[nID].dwProID, wPackLen);
	}
	pthread_mutex_unlock(&m_SendMutex);

	return wPackLen;
}

int		CtmTaskiNet::GetProtocolIndex(DWORD dwProID, BYTE byProtocolType)
{
	int nIndex = -1;

	tmiNetDataFile*	piNetDataFile = NULL;
	int				nNum = 0;

	if(byProtocolType == PROTOCOL_TYPE_STATIC)
	{
		piNetDataFile = m_pStaticSendFile;
		nNum = m_nStaticSendNum;
	}
	else
	{
		piNetDataFile = m_pDynamicProtocol;
		nNum = m_wDynamicProNum;
	}
	//printf("nNum = %d\n", nNum);
	for(int i = 0; i < nNum; i++)
	{
		//printf("piNetDataFile[%d].dwProID = %08x\n", i, piNetDataFile[i].dwProID);
		if(dwProID == piNetDataFile[i].dwProID)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

void	CtmTaskiNet::SetProtocolStamp(DWORD dwProID, BYTE byPackStamp, BYTE	byProtocolType)
{
	int nIndex = -1;

	tmiNetDataFile*	piNetDataFile = NULL;
	if(byProtocolType == PROTOCOL_TYPE_STATIC)
	{
		piNetDataFile = m_pStaticSendFile;
	}
	else
	{
		piNetDataFile = m_pDynamicProtocol;
	}

	nIndex = GetProtocolIndex(dwProID, byProtocolType);

	if(nIndex != -1)
		piNetDataFile[nIndex].byPackStamp = byPackStamp;
}

void	CtmTaskiNet::UnInit()
{
	UnStaticProtocol();
	UnDynamicProtocol();
	UnDataMap();
}

void	CtmTaskiNet::UnStaticProtocol()
{
	if(m_nStaticSendNum > 0)
	{
		for(int i = 0; i < m_nStaticSendNum; i++)
		{
			delete [] m_pStaticSendFile[i].pDBID;
			m_pStaticSendFile[i].pDBID = NULL;
		}

		delete []m_pStaticSendFile;
		m_pStaticSendFile = NULL;
	}

	if(m_nStaticSendNum > 0)
	{
		for(int i = 0; i < m_nStaticRevNum; i++)
		{
			delete [] m_pStaticRevFile[i].pDBID;
			m_pStaticRevFile[i].pDBID = NULL;
		}

		delete [] m_pStaticRevFile;
		m_pStaticRevFile = NULL;
	}
}

void	CtmTaskiNet::UnDynamicProtocol()
{
	if(m_wDynamicProNum > 0)
	{
		for(int i = 0; i < m_wDynamicProNum; i++)
		{
			if(m_pDynamicProtocol[i].wStartMSGNum > 0)
			{
				for(int j = 0; j < m_pDynamicProtocol[i].wStartMSGNum; j++)
				{
					if(m_pDynamicProtocol[i].pStartiNetMsg[j].wParaCount > 0)
					{
						for(int k = 0; k < m_pDynamicProtocol[i].pStartiNetMsg[j].wParaCount; k++)
						{
							if(m_pDynamicProtocol[i].pStartiNetMsg[j].pMsgPara[k].wDataType == INET_DATA_TYPE_ANSISTRING ||
				  			   m_pDynamicProtocol[i].pStartiNetMsg[j].pMsgPara[k].wDataType == INET_DATA_TYPE_UNICODESTRING)
				  			{
				  				delete [] m_pDynamicProtocol[i].pStartiNetMsg[j].pMsgPara[k].pcData;

				  				m_pDynamicProtocol[i].pStartiNetMsg[j].pMsgPara[k].pcData = NULL;
				  			}
						}

						delete [] m_pDynamicProtocol[i].pStartiNetMsg[j].pMsgPara;
						m_pDynamicProtocol[i].pStartiNetMsg[j].pMsgPara = NULL;
					}
				}

				delete [] m_pDynamicProtocol[i].pStartiNetMsg;
				m_pDynamicProtocol[i].pStartiNetMsg = NULL;
			}

			if(m_pDynamicProtocol[i].wEndMSGNum > 0)
			{
				for(int j = 0; j < m_pDynamicProtocol[i].wEndMSGNum; j++)
				{
					if(m_pDynamicProtocol[i].pEndiNetMsg[j].wParaCount > 0)
					{
						for(int k = 0; k < m_pDynamicProtocol[i].pEndiNetMsg[j].wParaCount; k++)
						{
							if(m_pDynamicProtocol[i].pEndiNetMsg[j].pMsgPara[k].wDataType == INET_DATA_TYPE_ANSISTRING ||
				  			   m_pDynamicProtocol[i].pEndiNetMsg[j].pMsgPara[k].wDataType == INET_DATA_TYPE_UNICODESTRING)
				  			{
				  				delete [] m_pDynamicProtocol[i].pEndiNetMsg[j].pMsgPara[k].pcData;

				  				m_pDynamicProtocol[i].pEndiNetMsg[j].pMsgPara[k].pcData = NULL;
				  			}
						}

						delete [] m_pDynamicProtocol[i].pEndiNetMsg[j].pMsgPara;
						m_pDynamicProtocol[i].pEndiNetMsg[j].pMsgPara = NULL;
					}
				}

				delete [] m_pDynamicProtocol[i].pStartiNetMsg;
				m_pDynamicProtocol[i].pStartiNetMsg = NULL;
			}

			delete [] m_pDynamicProtocol[i].pDBID;
			m_pDynamicProtocol[i].pDBID = NULL;
		}

		delete [] m_pDynamicProtocol;
	}

	memset(m_piNetStartMsg , 0 ,sizeof(m_piNetStartMsg));
	m_niNetStartMsgCount =0 ;
	memset(m_piNetEndMsg , 0 ,sizeof(m_piNetEndMsg));
	m_niNetEndMsgCount =0 ;
}

void	CtmTaskiNet::UnDataMap()
{
	if(m_nInetDataCount > 0)
	{
		delete [] m_InetDataMap;
		m_InetDataMap = NULL;

		delete [] m_HmiDataMap;
		m_HmiDataMap = NULL;
	}
}

/*---------------------------------------------------------------------------+
|                ComposeData			                                     |
+---------------------------------------------------------------------------*/
int		CtmTaskiNet::ComposeReqIPFlag(char* pOut, int nIndex)
{
	int nOffset = 0;
	BYTE byIPFlag = 0;

	if(m_iNetInfo.wDHCPFlag == 1)
		byIPFlag = 0;
	else
		byIPFlag = 1;

	memcpy(pOut + nOffset, &byIPFlag, sizeof(byIPFlag));
	nOffset += sizeof(byIPFlag);

	return nOffset;
}

int		CtmTaskiNet::ComposeIPAddress(char* pOut, int nIndex)
{
	int nOffset = 0;
	BYTE acIP[CONST_LENGTH_IP];

	GetLocalIP(acIP);

	memcpy(pOut + nOffset, acIP, CONST_LENGTH_IP);
	nOffset += CONST_LENGTH_IP;

	return nOffset;
}

int		CtmTaskiNet::ComposeEncoder(char* pOut, int nIndex)
{
	int nOffset = 0;
	BYTE byEncoder = INET_ENCODER_TYPE_ANSI;

	memcpy(pOut + nOffset, &byEncoder, sizeof(byEncoder));
	nOffset += sizeof(byEncoder);

	return nOffset;
}

int		CtmTaskiNet::ComposeProError(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwProErrorID, sizeof(m_dwProErrorID));
	nOffset += sizeof(m_dwProErrorID);

	if(m_dwProErrorID <= sizeof(m_pErrorStr)/sizeof(m_pErrorStr[0]))
	{
		m_wProErrorLength = strlen(m_pErrorStr[m_dwProErrorID]);
		memcpy(pOut + nOffset, &m_wProErrorLength, sizeof(m_wProErrorLength));
		nOffset += sizeof(m_wProErrorLength);

		memcpy(pOut + nOffset, m_pErrorStr[m_dwProErrorID], m_wProErrorLength);
		nOffset += m_wProErrorLength;
	}
	else
	{
		m_wProErrorLength = 0;
		memcpy(pOut + nOffset, &m_wProErrorLength, sizeof(m_wProErrorLength));
		nOffset += sizeof(m_wProErrorLength);
	}

	return nOffset;
}

int		CtmTaskiNet::ComposeTimer(char* pOut, int nIndex)
{
	int nOffset = 0;
	INetDateTime iNetDateTime;
	tmDATETIME  CurDateTime;

	GetDate(&CurDateTime.date);
	GetTime(&CurDateTime.time);

	iNetDateTime.wYear 			= CurDateTime.date.year;
	iNetDateTime.byMonth 		= CurDateTime.date.month;
	iNetDateTime.byDay			= CurDateTime.date.day;
	iNetDateTime.byHour			= CurDateTime.time.hour;
	iNetDateTime.byMinute 		= CurDateTime.time.minute;
	iNetDateTime.wMilliSecond 	= CurDateTime.time.second*1000;

	memcpy(pOut + nOffset, &iNetDateTime, sizeof(iNetDateTime));
	nOffset += sizeof(iNetDateTime);

	return nOffset;
}

int		CtmTaskiNet::ComposeTimerFlag(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_byiNetTimeSyncFlag, sizeof(m_byiNetTimeSyncFlag));
	nOffset += sizeof(m_byiNetTimeSyncFlag);

	return nOffset;
}

int		CtmTaskiNet::ComposeOnline(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwiNetOnLineTime, sizeof(m_dwiNetOnLineTime));
	nOffset += sizeof(m_dwiNetOnLineTime);

	return nOffset;
}

int		CtmTaskiNet::ComposeStaticVer(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwStaticVer, sizeof(m_dwStaticVer));
	nOffset += sizeof(m_dwStaticVer);

	return nOffset;
}

int		CtmTaskiNet::ComposeDynamicVer(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwDynamicVer, sizeof(m_dwDynamicVer));
	nOffset += sizeof(m_dwDynamicVer);

	return nOffset;
}

int		CtmTaskiNet::ComposeDynamicList(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwiNetDynamicVer, sizeof(m_dwiNetDynamicVer));
	nOffset += sizeof(m_dwiNetDynamicVer);

	memcpy(pOut + nOffset, &m_wiNetDynamicCount, sizeof(m_wiNetDynamicCount));
	nOffset += sizeof(m_wiNetDynamicCount);

	if(m_wiNetDynamicCount > 0 && m_iNetDynamicList != NULL)
	{
		memcpy(pOut + nOffset, m_iNetDynamicList, sizeof(tmiNetDynamicList)*m_wiNetDynamicCount);
		nOffset += sizeof(tmiNetDynamicList)*m_wiNetDynamicCount;

		delete [] m_iNetDynamicList;
		m_iNetDynamicList = NULL;
		m_wiNetDynamicCount = 0;
	}
	return nOffset;
}

int		CtmTaskiNet::ComposeReadData(char* pOut, int nIndex)
{
	int 	nOffset = 0;

	nOffset += ComposeData(pOut, m_ReadDataList, m_wReadDataCount, DATA_INET);

	delete [] m_ReadDataList;
	m_ReadDataList = NULL;
	m_wReadDataCount = 0;

	return nOffset;
}

int		CtmTaskiNet::ComposeWriteData(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_wWriteDataCount, sizeof(m_wWriteDataCount));
	nOffset += sizeof(m_wWriteDataCount);

	memcpy(pOut + nOffset, m_pWriteState, m_wWriteDataCount);
	nOffset += m_wWriteDataCount;

	return nOffset;
}

int		CtmTaskiNet::ComposeDataDic(char* pOut, int nIndex)
{
	int nOffset = 0;

	return nOffset;
}

int		CtmTaskiNet::ComposeiNetStaticVer(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwiNetStaticVer, sizeof(m_dwiNetStaticVer));
	nOffset += sizeof(m_dwiNetStaticVer);

	return nOffset;
}

int		CtmTaskiNet::ComposeiNetDynamicVer(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwiNetDynamicVer, sizeof(m_dwiNetDynamicVer));
	nOffset += sizeof(m_dwiNetDynamicVer);

	return nOffset;
}

int		CtmTaskiNet::ComposeDataMap(char* pOut, int nIndex)
{
	int nOffset = 0;

	memcpy(pOut + nOffset, &m_dwDataMapVer, sizeof(m_dwDataMapVer));
	nOffset += sizeof(m_dwDataMapVer);

	return nOffset;
}

int		CtmTaskiNet::ComposeHostType(char* pOut, int nIndex)
{
	int nOffset = 0;

	DWORD	dwHostType = 0;
	dwHostType = CtmConfig::GetInstance()->GetINETHostType();

	memcpy(pOut + nOffset, &dwHostType, sizeof(dwHostType));
	nOffset += sizeof(dwHostType);
	
	//printf("ComposeHostType  dwHostType = %d\n", dwHostType);

	return nOffset;
}

int		CtmTaskiNet::ComposePNSN_Hardware(char* pOut, int nIndex)
{
	int nOffset = 0;
	WORD	wLength = 0;
	char*	pBuffer = NULL;

	FILE *fp =NULL;

    if ((fp = fopen(HMI_PNSN_HARDWARE_PATH,"r")) == NULL)
    {
    	memcpy(pOut + nOffset, &wLength, sizeof(wLength));
		nOffset += sizeof(wLength);
    }
    else
    {

	    fseek(fp, 0, SEEK_END);
	    wLength = ftell(fp);
	
	    memcpy(pOut + nOffset, &wLength, sizeof(wLength));
		nOffset += sizeof(wLength);
	
		if(wLength > 0)
	    {
	    	pBuffer = new char[wLength];
	    	memset(pBuffer, 0, wLength);
	
	    	fseek(fp, 0, SEEK_SET);
	
	    	fread(pBuffer, wLength, 1, fp);
	
	    	memcpy(pOut + nOffset, pBuffer, wLength);
			nOffset += wLength;
	
			delete [] pBuffer;
	    }
	}

	return nOffset;
}


int		CtmTaskiNet::ComposePNSN_Software(char* pOut, int nIndex)
{
	int nOffset = 0;
	WORD	wLength = 0;
	char*	pBuffer = NULL;

	FILE *fp =NULL;

    if ((fp = fopen(HMI_PNSN_SOFTWARE_PATH, "r")) == NULL)
    {
    	memcpy(pOut + nOffset, &wLength, sizeof(wLength));
		nOffset += sizeof(wLength);
    }
    else
    {

	    fseek(fp, 0, SEEK_END);
	    wLength = ftell(fp);
	
	    memcpy(pOut + nOffset, &wLength, sizeof(wLength));
		nOffset += sizeof(wLength);
	
		if(wLength > 0)
	    {
	    	pBuffer = new char[wLength];
	    	memset(pBuffer, 0, wLength);
	
	    	fseek(fp, 0, SEEK_SET);
	
	    	fread(pBuffer, wLength, 1, fp);
	
	    	memcpy(pOut + nOffset, pBuffer, wLength);
			nOffset += wLength;
	
			delete [] pBuffer;
	    }
	}
	
	return nOffset;
}


/*---------------------------------------------------------------------------+
|           Parse                                                     		 |
+---------------------------------------------------------------------------*/
int		CtmTaskiNet::ParseIPAddress(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	nOffset = CONST_LENGTH_IP;

	m_bInitIP = TRUE;
	if(m_iNetInfo.wDHCPFlag == 0)
	{
		memcpy(m_iNetInfo.acLocalIP, pData+nOffset, sizeof(m_iNetInfo.acLocalIP));
		SetLocalNetInfo(&m_iNetInfo, sizeof(m_iNetInfo));
		SetLocalIP(m_iNetInfo.acLocalIP);
	}

	return nOffset;
}

int		CtmTaskiNet::ParseTimer(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	INetDateTime iNetDateTime;
	tmDATETIME  CurDateTime;

	memcpy(&iNetDateTime, pData+nOffset, sizeof(iNetDateTime));
	nOffset += sizeof(iNetDateTime);

	CurDateTime.date.year  		 = iNetDateTime.wYear;
	CurDateTime.date.month 		 = iNetDateTime.byMonth;
	CurDateTime.date.day   		 = iNetDateTime.byDay;
	CurDateTime.time.hour  		 = iNetDateTime.byHour;
	CurDateTime.time.minute		 = iNetDateTime.byMinute;
	CurDateTime.time.second		 = iNetDateTime.wMilliSecond/1000;

	SetDate(CurDateTime.date);
	SetTime(CurDateTime.time);

	return nOffset;
}

int		CtmTaskiNet::ParseTimerFlag(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	INetDateTime iNetDateTime;
	tmDATETIME  CurDateTime;

	memcpy(&iNetDateTime, pData+nOffset, sizeof(iNetDateTime));
	nOffset += sizeof(iNetDateTime);

	CurDateTime.date.year  		 = iNetDateTime.wYear;
	CurDateTime.date.month 		 = iNetDateTime.byMonth;
	CurDateTime.date.day   		 = iNetDateTime.byDay;
	CurDateTime.time.hour  		 = iNetDateTime.byHour;
	CurDateTime.time.minute		 = iNetDateTime.byMinute;
	CurDateTime.time.second		 = iNetDateTime.wMilliSecond/1000;

	SetDate(CurDateTime.date);
	SetTime(CurDateTime.time);

	m_byiNetTimeSyncFlag = 0;

	return nOffset;
}

int		CtmTaskiNet::ParseOnline(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	memcpy(&m_dwiNetOnLineTime, pData+nOffset, sizeof(m_dwiNetOnLineTime));
	nOffset += sizeof(m_dwiNetOnLineTime);

	if (m_lCheckIPSCounter <= 0)
			SendOneOffData();

	m_lCheckIPSCounter = m_dwiNetOnLineTime*3;
	m_lOfflineCounter = m_dwiNetOnLineTime*6;

	return nOffset;
}

int		CtmTaskiNet::ParseStaticVer(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	memcpy(&m_dwiNetStaticVer, pData+nOffset, sizeof(m_dwiNetStaticVer));
	nOffset += sizeof(m_dwiNetStaticVer);

	if(m_dwiNetStaticVer != m_dwStaticVer)
	{
		m_dwProErrorID = INET_ERROR_ID_STATICVERSION;
		Send(INET_SEND_STATIC_ERROR);
	}
	else
	{

	}
	m_bInitFlag = TRUE;

	return nOffset;
}

int		CtmTaskiNet::ParseDynamicVer(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	memcpy(&m_dwiNetDynamicVer, pData+nOffset, sizeof(m_dwiNetDynamicVer));
	nOffset += sizeof(m_dwiNetDynamicVer);

	if(m_dwiNetDynamicVer != m_dwDynamicVer)
		m_bDynamicUpdateFlag = TRUE;
	else
		m_bDynamicUpdateFlag = FALSE;

	return nOffset;
}

int		CtmTaskiNet::ParseDynamicList(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	DWORD	dwVersion = 0;
	WORD	wDynamicProCount = 0;
	tmiNetDynamicList* pDynamicList = NULL;
	m_wiNetDynamicCount = 0;

	memcpy(&dwVersion, pData+nOffset, sizeof(dwVersion));
	nOffset += sizeof(dwVersion);

	if(dwVersion == m_dwiNetDynamicVer)
	{
		memcpy(&wDynamicProCount, pData+nOffset, sizeof(wDynamicProCount));
		nOffset += sizeof(wDynamicProCount);

		pDynamicList = new tmiNetDynamicList[wDynamicProCount];
		memset(pDynamicList, 0, sizeof(tmiNetDynamicList)*wDynamicProCount);

		m_iNetDynamicList = new tmiNetDynamicList[wDynamicProCount];
		memset(m_iNetDynamicList, 0, sizeof(tmiNetDynamicList)*wDynamicProCount);
		m_wiNetDynamicCount = 0;

		memcpy(pDynamicList, pData+nOffset, sizeof(tmiNetDynamicList)*wDynamicProCount);
		nOffset += sizeof(tmiNetDynamicList)*wDynamicProCount;

		SetDynamicProtocolList(pDynamicList, wDynamicProCount);

		for(int i = 0; i < wDynamicProCount; i++)
		{
			int j = 0;
			for(j = 0; j < m_wDynamicProNum; j++)
			{
				if(pDynamicList[i].dwProID == m_pDynamicProtocol[j].dwProID)
				{
					if(pDynamicList[i].wProVer != m_pDynamicProtocol[j].wProVer)
					{
						m_iNetDynamicList[m_wiNetDynamicCount].dwProID = pDynamicList[i].dwProID;
						m_iNetDynamicList[m_wiNetDynamicCount].wProVer = pDynamicList[i].wProVer;
						m_wiNetDynamicCount++;
						break;
					}
				}
			}
			if(j >= m_wDynamicProNum)
			{
				//printf("ParseDynamicList  pDynamicList[%d].dwProID = %08X, %04X\n", i, pDynamicList[i].dwProID, pDynamicList[i].wProVer);
				m_iNetDynamicList[m_wiNetDynamicCount].dwProID = pDynamicList[i].dwProID;
				m_iNetDynamicList[m_wiNetDynamicCount].wProVer = pDynamicList[i].wProVer;
				m_wiNetDynamicCount++;
			}
		}

		delete []pDynamicList;
	}
	else
	{
		return -1;
	}

	return nOffset;
}

int		CtmTaskiNet::ParseDynamicData(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	WORD	wCount = 0;
	DWORD	dwProID = 0;
	WORD	wVersion = 0;
	WORD	wDBCount = 0;
	DWORD*	pDBIDs = NULL;

	memcpy(&wCount, pData+nOffset, sizeof(wCount));
	nOffset += sizeof(wCount);
	//printf("ParseDynamicData wCount = %d\n", wCount);

	for(int i = 0; i < wCount; i++)
	{
		memcpy(&dwProID, pData+nOffset, sizeof(dwProID));
		nOffset += sizeof(dwProID);
		//printf("ParseDynamicData i = %d, dwProID = %08x\n", i, dwProID);

		memcpy(&wVersion, pData+nOffset, sizeof(wVersion));
		nOffset += sizeof(wVersion);
		//printf("ParseDynamicData i = %d, wVersion = %04x\n",i, wVersion);

		memcpy(&wDBCount, pData+nOffset, sizeof(wDBCount));
		nOffset += sizeof(wDBCount);
		//printf("ParseDynamicData i = %d, wDBCount = %d\n",i, wDBCount);

		pDBIDs = new DWORD[wDBCount];
		memcpy(pDBIDs, pData+nOffset, sizeof(DWORD) * wDBCount);
		nOffset += sizeof(DWORD) * wDBCount;
		SetDynamicDBID(dwProID, wVersion, wDBCount, pDBIDs);

		SetDynamicMsg(dwProID, pData+nOffset, 0);

		delete []pDBIDs;
	}

	return nOffset;
}

int		CtmTaskiNet::ParseReadData(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	memcpy(&m_wReadDataCount, pData+nOffset, sizeof(m_wReadDataCount));
	nOffset += sizeof(m_wReadDataCount);

	m_ReadDataList = new DWORD[m_wReadDataCount];

	memcpy(m_ReadDataList, pData+nOffset, sizeof(DWORD)*m_wReadDataCount);
	nOffset += sizeof(DWORD)*m_wReadDataCount;

	return nOffset;
}

int		CtmTaskiNet::ParseWriteData(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	DWORD	dwHmiDBID = 0;
	DWORD	dwDBID = 0;
	WORD	wDataType = 0;
	WORD	wUOM = 0;
	WORD	wLength = 0;
	DBVALUE	dbValue;
	INetDataMap*	piNetDataMap = NULL;

	memcpy(&m_wWriteDataCount, pData+nOffset, sizeof(m_wWriteDataCount));
	nOffset += sizeof(m_wWriteDataCount);

	m_pWriteState = new BYTE[m_wWriteDataCount];
	memset(m_pWriteState, 0, m_wWriteDataCount);

	for(int i = 0; i < m_wWriteDataCount; i++)
	{
		memcpy(&dwDBID, pData+nOffset, sizeof(dwDBID));
		nOffset += sizeof(dwDBID);

		dwHmiDBID = dwDBID;
		piNetDataMap = GetiNetDBMapFromiNetDBID(dwDBID);

		if(piNetDataMap != NULL)
			dwHmiDBID = piNetDataMap->dwHmiDBID;

		int 	nIndex = g_pDatabase->DataIDToIndex(dwHmiDBID);

		//printf("ParseWriteData  dwHmiDBID = %x\n", dwHmiDBID);
		memcpy(&wDataType, pData+nOffset, sizeof(wDataType));
		nOffset += sizeof(wDataType);

		memcpy(&wUOM, pData+nOffset, sizeof(wUOM));
		nOffset += sizeof(wUOM);

		if(wDataType == INET_DATA_TYPE_ANSISTRING)
		{
			memcpy(&wLength, pData+nOffset, sizeof(wLength));
			nOffset += sizeof(wLength);

			char	szTemp[32];
			memset(szTemp, 0, 32);

			memcpy(szTemp, pData + nOffset, wLength);

			dbValue = g_pDatabase->Write(nIndex, szTemp, TRUE);
			nOffset += wLength;
		}
		else
		{
			double dValue = 0.0;
			char acData[8];
			memset(acData, 0, sizeof(acData));
			//printf("ParseWriteData  wDataType = %d, length = %d\n", wDataType, GetiNetDataSize(wDataType));
			memcpy(acData, pData + nOffset, GetiNetDataSize(wDataType));
			nOffset += GetiNetDataSize(wDataType);

			if(wDataType == INET_DATA_TYPE_DOUBLE)
			{
				#ifdef	D_PXA270
					memcpy(&dValue, acData, sizeof(dValue));
					dValue = Pxa270DBConvert(dValue);
					memcpy(acData, &dValue, sizeof(dValue));
				#endif
			}
			dbValue = g_pDatabase->Write(nIndex, acData, TRUE);
		}
		m_pWriteState[i] = dbValue.dwState;
	}

	return nOffset;
}

int		CtmTaskiNet::ParseiNetStaticVer(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	char	szTemp[16];

	memcpy(&m_dwiNetStaticVer, pData+nOffset, sizeof(m_dwiNetStaticVer));
	nOffset += sizeof(m_dwiNetStaticVer);

	if(m_dwiNetStaticVer != m_dwStaticVer)
	{
		m_dwProErrorID = INET_ERROR_ID_STATICVERSION;
		Send(INET_SEND_STATIC_ERROR);
	}

	return nOffset;
}

int		CtmTaskiNet::ParseiNetDynamicVer(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	char	szTemp[16];

	memcpy(&m_dwiNetDynamicVer, pData+nOffset, sizeof(m_dwiNetDynamicVer));
	nOffset += sizeof(m_dwiNetDynamicVer);

	if(m_dwiNetDynamicVer != m_dwDynamicVer)
	{
		char*	pszFileName = NULL;
		CIniFile ReadIni;

		if(ReadIni.OpenFile("outerconfig.ini"))
		{
			ReadIni.ReadString("iNet","ProtocolConf", 	NULL, &pszFileName);
			//printf("ParseiNetDynamicVer  pszFileName = %s, m_dwiNetDynamicVer = %08x\n", pszFileName, m_dwiNetDynamicVer);
			if(pszFileName != NULL)
			{
				if(ReadIni.OpenFile(pszFileName))
				{
					sprintf(szTemp, "0x%08x", m_dwiNetDynamicVer);
					ReadIni.WriteString("Dynamic", "Version", szTemp);
				}
			}
		}

		UnDynamicProtocol();
		InitDynamicProtocol();
	}
	m_bEndSynch = TRUE;
	m_bUpdateFlag = FALSE;
	m_bDynamicUpdateFlag = FALSE;

	return nOffset;
}


int		CtmTaskiNet::ParseDataDic(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;

	memcpy(&m_dwDicStartID, pData+nOffset, sizeof(m_dwDicStartID));
	nOffset += sizeof(m_dwDicStartID);

	memcpy(&m_dwDicEndID, pData+nOffset, sizeof(m_dwDicEndID));
	nOffset += sizeof(m_dwDicEndID);

	m_byReadDataFlag = 0;

	return nOffset;
}


int		CtmTaskiNet::ParseDataMap(char* pData, int nLen)
{
	if (nLen <=0)	return 0;

	int		nOffset = 0;
	int nDataCount = 0;

	memcpy(&m_dwDataMapVer, pData+nOffset, sizeof(m_dwDataMapVer));
	nOffset += sizeof(m_dwDataMapVer);

	memcpy(&nDataCount, pData+nOffset, sizeof(nDataCount));

	if(nDataCount <= 0) return -1;

	CIniFile ReadIni;
	char*	pszFileName = NULL;

	if(ReadIni.OpenFile("outerconfig.ini"))
	{
		ReadIni.ReadString("iNet","DataMap", 	NULL, &pszFileName);
		//printf("ParseDataMap  pszFileName = %s\n", pszFileName);
		if(pszFileName != NULL)
		{
			WriteFile(pszFileName, (char*)&m_dwDataMapVer, sizeof(m_dwDataMapVer), nOffset);

			m_nInetDataCount = nDataCount;
			WriteFile(pszFileName, (char*)&m_nInetDataCount, sizeof(m_nInetDataCount), nOffset);

			nOffset += sizeof(m_nInetDataCount);

			//printf("ParseDataMap  m_nInetDataCount = %d\n", m_nInetDataCount);

			if(m_nInetDataCount > 0)
			{
				if(m_InetDataMap != NULL) delete [] m_InetDataMap;
				if(m_HmiDataMap != NULL) delete [] m_HmiDataMap;

				m_InetDataMap = new INetDataMap[m_nInetDataCount];
				m_HmiDataMap = new INetDataMap[m_nInetDataCount];

				memcpy(m_InetDataMap, pData+nOffset, sizeof(INetDataMap)*m_nInetDataCount);
				memcpy(m_HmiDataMap, m_InetDataMap, sizeof(INetDataMap)*m_nInetDataCount);

				qsort(m_InetDataMap, m_nInetDataCount, sizeof(INetDataMap), cmp_inet_datamap);
				qsort(m_HmiDataMap, m_nInetDataCount, sizeof(INetDataMap), cmp_hmi_datamap);

				WriteFile(pszFileName, (char*)m_InetDataMap, sizeof(INetDataMap)*m_nInetDataCount, nOffset);
				nOffset += sizeof(INetDataMap)*m_nInetDataCount;

				for(int i = 0; i < m_nInetDataCount; i++)
				{
					;//printf("ParseDataMap  m_InetDataMap[%d], iNetID = %08x, HMIID = %08x\n",
					//		m_InetDataMap[i].dwInetDBID, m_InetDataMap[i].dwHmiDBID);
				}
			}
		}
	}

	return nOffset;
}

/*---------------------------------------------------------------------------+
|           MSG 矪瞶                                                       	 |
+---------------------------------------------------------------------------*/
void	CtmTaskiNet::DealTimer(WPARAM wParam, LPARAM lParam)
{
	//printf("m_bEndSynch = %d\n", m_bEndSynch);
	if (m_nLineStatus == 0 && m_bEndSynch && m_liNetTimer <= 0)
	{
		m_dwiNetTimer +=100 ;
		m_liNetTimer = INET_TIMEER ;
		int i, j, k;

		for(i = 0; i < m_niNetEndMsgCount ; i++)
		{
			if(((m_piNetEndMsg[i].iNetMsg->wMsgID & 0xFF00) >> 8) == INET_TRIGGER_TYPE_TIMER)
			{
				//printf("DealTimer m_piNetEndMsg[i].iNetMsg->wMsgID = %x, wParaCount = %d\n",
				//		m_piNetEndMsg[i].iNetMsg->wMsgID, m_piNetEndMsg[i].iNetMsg->wParaCount);
				for(j = 0; j < m_piNetEndMsg[i].iNetMsg->wParaCount; j++)
				{
					//printf("DealTimer m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwData = %d, m_dwiNetTimer = %d\n",
					//		m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwData, m_dwiNetTimer);
					if((m_dwiNetTimer % m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwData) != 0)
						break;
				}

				if(j >= m_piNetEndMsg[i].iNetMsg->wParaCount)
				{
					for(k = 0; k < m_piNetEndMsg[i].nProCount; k++)
					{
						//pthread_mutex_lock(&m_SendMutex);
						//printf("DealTimer m_piNetEndMsg[%d].pProIDs[%d] = %d\n", m_piNetEndMsg[i].pProIDs[k]);
						Send(m_piNetEndMsg[i].pProIDs[k], PROTOCOL_TYPE_DYNAMIC);
						//pthread_mutex_unlock(&m_SendMutex);
					}
				}
			}
		}
	}
}

void	CtmTaskiNet::ProcessParaIDMsg(tmMsgParaID* pMsgParaID)
{
	DWORD			dwDBID = pMsgParaID->dwDBID;
	DWORD			dwInetDBID = 0;
	DBVALUE			dbValue;
	INetDataMap*	piNetDataMap = GetiNetDBMapFromHmiDBID(dwDBID);

	dwInetDBID = dwDBID;
	if(piNetDataMap != NULL)
		dwInetDBID = piNetDataMap->dwInetDBID;

	dbValue= g_pDatabase->Read(g_pDatabase->DataIDToIndex(dwDBID));

	union
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
		char			*pcData;
	};
	memset(acData, 0, sizeof(acData));
	union
	{
		char 			acOldData[8];
		unsigned char 	ucOldData;
		unsigned short	wOldData;
		unsigned long	dwOldData;
		signed char 	cOldData;
		signed short	nOldData;
		signed long		lOldData;
		float 			fOldData;
		double 			dOldData;
		char			*pcOldData;
	};
	memset(acOldData, 0, sizeof(acOldData));

	//printf("DealParaDataID  wParam  ID = %08x\n", dwDBID);

	if (m_nLineStatus == 0 && m_bEndSynch)
	{
		int i, j, k;

		for(i = 0; i < m_niNetEndMsgCount ; i++)
		{
			if(((m_piNetEndMsg[i].iNetMsg->wMsgID & 0xFF00) >> 8) == INET_TRIGGER_TYPE_PARAID)
			{
				for(j = 0; j < m_piNetEndMsg[i].iNetMsg->wParaCount; j++)
				{
					if(dwInetDBID == m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwDataID)
					{
						int nResult = -1;
						memcpy(acData, dbValue.acData, sizeof(dbValue.acData));
						memcpy(acOldData, pMsgParaID->szOldValue, sizeof(dbValue.acData));
						switch(dbValue.DataType.wType)
						{
							case TYPE_SHORT:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, nData, nOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].nData);
								break;
							case TYPE_BYTE:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, ucData, ucOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].ucData);
								break;
							case TYPE_BOOL:
							case TYPE_WORD:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, wData, wOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].wData);
								break;
							case TYPE_DWORD:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, dwData, dwOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwData);
								break;
							case TYPE_CHAR:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, cData, cOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].cData);
								break;
							case TYPE_INT:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, lData, lOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].lData);
								break;
							case TYPE_FLOAT:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, fData, fOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].fData);
								break;
							case TYPE_DOUBLE:
								nResult = ProcessCondtion(m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dwCondition, dData, dOldData, m_piNetEndMsg[i].iNetMsg->pMsgPara[j].dData);
								break;
							case TYPE_STRING:
								break;
							default:
								break;
						}
						//printf("nResult = %d\n", nResult);
						if(nResult == -1)
							break;
					}
					else
						break;
				}
				//printf("j = %d, m_piNetEndMsg[i].iNetMsg->wParaCount = %d\n", j, m_piNetEndMsg[i].iNetMsg->wParaCount);
				if(j >= m_piNetEndMsg[i].iNetMsg->wParaCount)
				{
					for(k = 0; k < m_piNetEndMsg[i].nProCount; k++)
					{
						//printf("m_piNetEndMsg[i].pProIDs[k] = %x\n",m_piNetEndMsg[i].pProIDs[k]);
						//pthread_mutex_lock(&m_SendMutex);
						Send(m_piNetEndMsg[i].pProIDs[k], PROTOCOL_TYPE_DYNAMIC);
						//pthread_mutex_unlock(&m_SendMutex);
					}
				}
			}
		}
	}
}

void	CtmTaskiNet::DealToiNetRestart(WPARAM wParam, LPARAM lParam)
{
	RestartNet(NET_RESTARTNET);
}

int		CtmTaskiNet::RevDataProFunc(char* pData, int nIndex)
{
	m_bUpdateFlag = TRUE;
}
/*---------------------------------------------------------------------------+
|           Parse                                                     		 |
+---------------------------------------------------------------------------*/
int		CtmTaskiNet::ReqValues(int nType, void* pOutData, int nSize)
{
	if(nType == CONST_REQ_LOCALNETINFO)
	{
		return GetLocalNetInfo(pOutData, nSize);
	}
	else if(nType ==  CONST_MSG_PARA_ID)
	{
		ProcessParaIDMsg((tmMsgParaID*)pOutData);
	}
	else
		return 0;
}

int		CtmTaskiNet::WriteValue(int nType, void* pInputData, int nSize)
{
	if(nType == CONST_RES_LOCALNETINFO)
	{
		return SetLocalNetInfo(pInputData, nSize);
	}
	else
		return 0;
}

int		CtmTaskiNet::GetLocalNetInfo(void* pOutData, int nSize)
{
	CtmFile	u_File;
	tmiNetInfo	iNetInfo;
	int		nFd	= 0;
	u_File.Open(INET_NETINFOFILE_PATH, O_RDONLY | O_SYNC);

	nFd = u_File.GetFileHandle();
	if (nFd > 0)
	{
		if(u_File.Read(pOutData, nSize) == nSize)
		{
			u_File.Close();
			nFd = -1;
			return nSize;
		}
	}
	printf("GetLocalNetInfo  \n");
	iNetInfo.wDHCPFlag	 	= 1;
	iNetInfo.acLocalIP[0]	=192;
	iNetInfo.acLocalIP[1]	=168;
	iNetInfo.acLocalIP[2]	=8;
	iNetInfo.acLocalIP[3]	=123;

	iNetInfo.acNetMaskIP[0]	=255;
	iNetInfo.acNetMaskIP[1]	=255;
	iNetInfo.acNetMaskIP[2]	=255;
	iNetInfo.acNetMaskIP[3]	=0;

	iNetInfo.acGatway[0]	=192;
	iNetInfo.acGatway[1]	=168;
	iNetInfo.acGatway[2]	=8;
	iNetInfo.acGatway[3]	=150;

	iNetInfo.acServerIP[0]	=192;
	iNetInfo.acServerIP[1]	=168;
	iNetInfo.acServerIP[2]	=8;
	iNetInfo.acServerIP[3]	=150;

	iNetInfo.wsrcPort		= 12345;
	iNetInfo.wdstPort		= 12345;

	memcpy(pOutData, &iNetInfo, nSize);
	return 0;
}

int 	CtmTaskiNet::SetLocalNetInfo(void* pInputData, int nSize)
{
	CtmFile	u_File;
	int		nFd	= 0;
	u_File.Open(INET_NETINFOFILE_PATH, O_RDWR | O_CREAT | O_SYNC);

	nFd = u_File.GetFileHandle();
	if (nFd > 0)
	{
		if(u_File.Write(pInputData, nSize) == nSize)
		{
			u_File.Close();
			nFd = -1;
			return nSize;
		}
		else return 0;
	}
	else
		return 0;
}

WORD	CtmTaskiNet::GetCustomDataType(DWORD dwDBID)
{
	WORD	wDataType = 0xFFFF;
	int i = 0;
	for(i = 0; i < sizeof(m_CustomData)/sizeof(m_CustomData[0]); i++)
	{
		if(dwDBID == m_CustomData[i].dwDBID)
		{
			wDataType = m_CustomData[i].wType;
			break;
		}
	}

	return wDataType;
}

INetDataMap*	CtmTaskiNet::GetiNetDBMapFromiNetDBID(DWORD dwiNetDBID)
{
	INetDataMap		iNetDataMap;
	INetDataMap*	piNetDataMap = NULL;

	iNetDataMap.dwInetDBID = dwiNetDBID;
	piNetDataMap = (INetDataMap*)bsearch(&iNetDataMap, m_InetDataMap, m_nInetDataCount, sizeof(INetDataMap),cmp_inet_datamap);

	return piNetDataMap;
}

INetDataMap*	CtmTaskiNet::GetiNetDBMapFromHmiDBID(DWORD dwHmiDBID)
{
	INetDataMap		iNetDataMap;
	INetDataMap*	piNetDataMap = NULL;

	iNetDataMap.dwHmiDBID = dwHmiDBID;
	piNetDataMap = (INetDataMap*)bsearch(&iNetDataMap, m_HmiDataMap, m_nInetDataCount, sizeof(INetDataMap),cmp_hmi_datamap);

	return piNetDataMap;
}

INetDBInfo	CtmTaskiNet::GetiNetDBInfo(int nDBIndex, INetDataMap* piNetDataMap)
{
	DBVALUE	dbValue;

	INetDBInfo iNetDBInfo;
	memset(&iNetDBInfo, 0, sizeof(iNetDBInfo));

	dbValue = g_pDatabase->Read(nDBIndex);

	//printf("DataTypeHmiTOInet(%d) = %d\n", dbValue.DataType.wType, DataTypeHmiTOInet(dbValue.DataType.wType));
	iNetDBInfo.wType = DataTypeHmiTOInet(dbValue.DataType.wType);
	iNetDBInfo.byPrecision = dbValue.wPrecision;
	iNetDBInfo.wUOM = 0;
	if(piNetDataMap != NULL)
	{
		iNetDBInfo.byPrecision += piNetDataMap->byPrecision;
		iNetDBInfo.wUOM = piNetDataMap->wUOM;
	}

	return iNetDBInfo;
}

WORD 		CtmTaskiNet::GetiNetDataSize(WORD wDataType)
{
	WORD wSize = 0;

	for(int i = 0; i < sizeof(m_DataTypeMap)/sizeof(m_DataTypeMap[0]); i++)
	{
		if(wDataType == m_DataTypeMap[i].wiNetType)
		{
			wSize = m_DataTypeMap[i].wSize;
			break;
		}
	}

	return wSize;
}

WORD 		CtmTaskiNet::GetHMIDataSize(WORD wDataType)
{
	WORD wSize = 0;

	for(int i = 0; i < sizeof(m_DataTypeMap)/sizeof(m_DataTypeMap[0]); i++)
	{
		if(wDataType == m_DataTypeMap[i].wHMIType)
		{
			wSize = m_DataTypeMap[i].wSize;
			break;
		}
	}

	return wSize;
}

WORD 		CtmTaskiNet::DataTypeHmiTOInet(WORD wDataType)
{
	WORD wType;

	for(int i = 0; i < sizeof(m_DataTypeMap)/sizeof(m_DataTypeMap[0]); i++)
	{
		if(wDataType == m_DataTypeMap[i].wHMIType)
		{
			wType = m_DataTypeMap[i].wiNetType;
			break;
		}
	}

	return wType;
}

WORD 		CtmTaskiNet::DataTypeInetTOHmi(WORD wDataType)
{
	WORD wType;

	for(int i = 0; i < sizeof(m_DataTypeMap)/sizeof(m_DataTypeMap[0]); i++)
	{
		if(wDataType == m_DataTypeMap[i].wiNetType)
		{
			wType = m_DataTypeMap[i].wHMIType;
			break;
		}
	}

	return wType;
}

int 	CtmTaskiNet::AddMsgPro(tmiNetMsg* piNetMsg, WORD wMsgNum, DWORD dwProID, tmiNetMsgPRO* piNetMsgPro, int niNetMsgProCount)
{
	//printf("AddMsgPro  wMsgNum = %d, dwProID = %x, niNetMsgProCount = %d\n", wMsgNum, dwProID, niNetMsgProCount);
	for(int i = 0; i < wMsgNum; i++)
	{
		int j = 0;
		BOOL	bFlag = TRUE;
		for(j = 0; j < niNetMsgProCount; j++)
		{
			if(piNetMsgPro[j].iNetMsg->wMsgID == piNetMsg[i].wMsgID &&
				piNetMsgPro[j].iNetMsg->wParaCount == piNetMsg[i].wParaCount)
			{
				int k = 0;
				for(k = 0; k < piNetMsgPro[j].iNetMsg->wParaCount; k++)
				{
					if(piNetMsgPro[j].iNetMsg->pMsgPara[k].dwDataID == piNetMsg[i].pMsgPara[k].dwDataID &&
						piNetMsgPro[j].iNetMsg->pMsgPara[k].dwCondition == piNetMsg[i].pMsgPara[k].dwCondition &&
						piNetMsgPro[j].iNetMsg->pMsgPara[k].wLength == piNetMsg[i].pMsgPara[k].wLength)
					{
						if(piNetMsgPro[j].iNetMsg->pMsgPara[k].wDataType == INET_DATA_TYPE_ANSISTRING ||
							piNetMsgPro[j].iNetMsg->pMsgPara[k].wDataType == INET_DATA_TYPE_UNICODESTRING)
						{
							if(memcmp(piNetMsgPro[j].iNetMsg->pMsgPara[k].pcData, piNetMsg[i].pMsgPara[k].pcData, piNetMsgPro[j].iNetMsg->pMsgPara[k].wLength) != 0)
							{
								break;
							}
						}
						else
						{
							if(memcmp(piNetMsgPro[j].iNetMsg->pMsgPara[k].acData, piNetMsg[i].pMsgPara[k].acData, piNetMsgPro[j].iNetMsg->pMsgPara[k].wLength) != 0)
							{
								break;
							}
						}
					}
					else
						break;
				}

				if(k >= piNetMsgPro[j].iNetMsg->wParaCount)
				{
					piNetMsgPro[j].pProIDs[piNetMsgPro[j].nProCount++] = dwProID;
					bFlag = FALSE;
				}
				else
					break;
			}
			else
				break;
		}

		if(bFlag)
		{
			piNetMsgPro[niNetMsgProCount].iNetMsg = piNetMsg;
			piNetMsgPro[niNetMsgProCount].pProIDs[0] = dwProID;
			piNetMsgPro[niNetMsgProCount].nProCount = 1;
			niNetMsgProCount++;
			if((piNetMsg->wMsgID & 0xFF00) >> 8 == INET_TRIGGER_TYPE_PARAID)
				AddMsgDataID(piNetMsg, piNetMsg->wParaCount);
		}
	}

	return niNetMsgProCount;
}

void	CtmTaskiNet::AddMsgDataID(tmiNetMsg* piNetMsg, WORD wParaCount)
{
	DWORD			dwHmiDataID = 0;
	INetDataMap*	piNetDataMap = NULL;

	for(int i = 0; i < wParaCount; i++)
	{
		piNetDataMap = GetiNetDBMapFromiNetDBID(piNetMsg->pMsgPara[i].dwDataID);
		dwHmiDataID = piNetMsg->pMsgPara[i].dwDataID;
		if(piNetDataMap != NULL)
			dwHmiDataID = piNetDataMap->dwHmiDBID;

		int j = 0;
		for(j = 0; j < g_wInetMsgDataIDCount; j++)
		{
			if(g_dwInetMsgDataIDList[j] == dwHmiDataID)
			{
				break;
			}
		}
		if(j >= g_wInetMsgDataIDCount)
		{
			//printf("g_wInetMsgDataIDCount = %d, dwHmiDataID = %x\n", g_wInetMsgDataIDCount, dwHmiDataID);
			g_dwInetMsgDataIDList[g_wInetMsgDataIDCount++] = dwHmiDataID;
		}
	}
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, unsigned char Value, unsigned char OldValue, unsigned char ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_AND) != 0)
	{
		if((Value & ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_OR) != 0)
	{
		if((Value | ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_NOT) != 0)
	{
		if(!(Value) == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_XOR) != 0)
	{
		if((Value ^ ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, unsigned short Value, unsigned short OldValue, unsigned short ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_AND) != 0)
	{
		if((Value & ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_OR) != 0)
	{
		if((Value | ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_NOT) != 0)
	{
		if(!(Value) == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_XOR) != 0)
	{
		if((Value ^ ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, unsigned long Value, unsigned long OldValue, unsigned long ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_AND) != 0)
	{
		if((Value & ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_OR) != 0)
	{
		if((Value | ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_NOT) != 0)
	{
		if(!(Value) == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_XOR) != 0)
	{
		if((Value ^ ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, signed char Value, signed char OldValue, signed char ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_AND) != 0)
	{
		if((Value & ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_OR) != 0)
	{
		if((Value | ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_NOT) != 0)
	{
		if(!(Value) == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_XOR) != 0)
	{
		if((Value ^ ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, signed short Value, signed short OldValue, signed short ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_AND) != 0)
	{
		if((Value & ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_OR) != 0)
	{
		if((Value | ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_NOT) != 0)
	{
		if(!(Value) == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_XOR) != 0)
	{
		if((Value ^ ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, signed long Value, signed long OldValue, signed long ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_AND) != 0)
	{
		if((Value & ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_OR) != 0)
	{
		if((Value | ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_NOT) != 0)
	{
		if(!(Value) == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_XOR) != 0)
	{
		if((Value ^ ParaValue) != 0)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, float Value, float OldValue, float ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	return nResult;
}

int		CtmTaskiNet::ProcessCondtion(DWORD dwCondition, double Value, double	OldValue, double ParaValue)
{
	int nResult = -1;

	if((dwCondition & INET_TRIGGER_CONDITION_EQUAL) != 0)
	{
		if(Value == ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_GREATER) != 0)
	{
		if(Value > ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_LESS) != 0)
	{
		if(Value < ParaValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_RISING) != 0)
	{
		if(Value > OldValue)
		{
			nResult = 0;
		}
	}

	if((dwCondition & INET_TRIGGER_CONDITION_FALLING) != 0)
	{
		if(Value < OldValue)
		{
			nResult = 0;
		}
	}

	return nResult;
}


/*	生成CRC的表	*/
void 	CtmTaskiNet::CreateCRC32Table()
{
    unsigned int nTemp;
    unsigned int i, j;
    //printf("CreateCRC32Table  ::");
    for (i = 0; i < 256; i++)
    {
        nTemp =i;
        for (j = 0; j < 8; j++)
        {
            if (nTemp & 1)  nTemp = CRC_32 ^ (nTemp >> 1);
            else            nTemp >>= 1;
        }
        _CRC32_Talbe[i] = nTemp;
        //printf("%x, ", _CRC32_Talbe[i]);
    }
}

/*====================================================
// 函数说明：一次性计算32位CRC校验码
// 参数1：pData，表示需要计算CRC的数据
// 参数2：Length，表示此数据的长度
// 返回值：计算所得出的32位的CRC校验码
//==================================================*/
unsigned int CtmTaskiNet::GetCRC32(char* pData, unsigned  int Length)
{
    return GetCRC32B(0xFFFFFFFF, pData, Length);
}

/*====================================================
// 函数说明：多次性计算32位CRC校验码
// 函数1：上一次计算的32位CRC校验码，如果是第一次，此值为0xFFFFFFF
// 参数2：pData，表示需要计算CRC的数据
// 参数3：Length，表示此数据的长度
// 返回值：计算所得出的32位的CRC校验码
//==================================================*/

unsigned int 	CtmTaskiNet::GetCRC32B(unsigned int CRCCode, char* pData, unsigned int Length)
{
	unsigned int i;
	unsigned int nResult = CRCCode;
	for (i = 0; i < Length; i++)
	nResult = _CRC32_Talbe[(nResult ^ pData[i]) & 0xFF] ^ (nResult >> 8);

	return nResult;
}

#ifndef	D_BEOBJECT_CTMTASKINET
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskinet == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskHost*>(pResult))->CreateSelf();
		g_ptaskinet = static_cast<CTaskHost*>(pResult);
	}
	return g_ptaskinet;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskHost*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
