#include	"../../package.h"
#include	"../../timer.h"
#include	"../../main.h"
#include	"../../tmdebug.h"
#include	"../../init.h"
#include	"../../utils.h"
#include	"../../ethnet.h"
#include	"tmTaskDsp54.h"
#include  "../../tmnbmsg.h"
IMPLEMENT_DYNCREATE(CtmTaskDsp54, CTaskHost)

typedef void *(* PRSRUN)(void *);
#define		CNT_INT_CHECK_ONLNE_TIME	5000
#define     TLED_SLICE					500
#define     COMM_TIME_REQCONFIG         (3 * 1024)

#define		DB_MACH_CMD_ID				0x0020
#define		DB_MOLDSET_CMD_ID			0x0021
#define		DB_MONIMAX_CMD_ID			0x0022
#define		DB_POWERMATCH_CMD_ID		0x0023
#define		DB_MACHB_CMD_ID				0x0024
#define		DB_MOLDSETB_CMD_ID			0x0025
#define		DB_MONIMAXB_CMD_ID			0x0026
#define		DB_NEWPOWERMATCH_CMD_ID		0x0027
#define		DB_OTHER_CMD_ID				0x0030

#define		DB_MACH_MAXINDEX_ID			0x400
#define		DB_MOLDSET_MAXINDEX_ID		0x900
#define		DB_POWERMATCH_MAXINDEX_ID	0xA00
#define		DB_MONIMAX_MAXINDEX_ID		0xA01
#define		DB_MACHB_MAXINDEX_ID		0x1100
#define		DB_MOLDSETB_MAXINDEX_ID		0x1400
#define		DB_MONIMAXB_MAXINDEX_ID		0x1460

BOOL        bSpeechDelayOnOff;    
long        m_lCounterSpeechDelayOnOff;
WORD		dbgppc[tmMAX_GROUP_PC];
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
static      long        g_lCounterLEDMotor;
static      BOOL        g_bLEDMotor = FALSE;
static      long        g_lCounterLEDAdjMold;
static      BOOL        g_bLEDAdjMold = FALSE;
static 		BOOL	  	u_wThreadStartRunFlag=FALSE;
static		BOOL		g_bUpdateFlag = FALSE;
static		WORD		g_wSentReqState = 0;
extern		pthread_mutex_t		g_setFlagLock;		
CtmTaskDsp54*	CtmTaskDsp54::m_pSelf = NULL;

CtmTaskDsp54::CtmTaskDsp54()
{
	m_pSelf				= this;
	m_nConnecting		= 1;
	m_nConnectStatus	= 2;
	
	m_nMaxElements		= 50;
	
	m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME;
	m_lOnLineTime		= CNT_INT_CHECK_ONLNE_TIME;
	m_lUpdateTimer		= 100;

	m_lCounterReqConfig = COMM_TIME_REQCONFIG;
	AddCounter((long *)&m_lCounterReqConfig, m_idMe);
	
	bSpeechDelayOnOff = FALSE;
	m_lCounterSpeechDelayOnOff = 100;

	dbbuff.flMotorStatus = 0xFF;
	dbbuff.flHeaterStatus= 0xFF;
	m_tled.lCount    = TLED_SLICE;
	m_tled.nIndex    = 0;

	//AddCounter((long *)&m_lCounterW0, m_idMe);
	//AddCounter((long *)&m_lCounterW1, m_idMe);
	//AddCounter((long *)&m_lCounterBW1, m_idMe);
	AddCounter((long *)&m_tled.lCount, m_idMe);
	AddCounter((long *)&g_lCounterLEDMotor, m_idMe);
	AddCounter((long *)&g_lCounterLEDAdjMold, m_idMe);

	//AddCounter(&m_lUpdateTimer, 	m_idMe);
	AddCounter(&m_lCheckQuickTimer, m_idMe);
	AddCounter(&m_lOnLineTime, 		m_idMe);

	memset(m_RxBuffer, 0, sizeof(m_RxBuffer));
	memset(m_TxBuffer, 0, sizeof(m_TxBuffer));
	RegisterMessage(MSG_DSP54, m_idMe);
	RegisterMessage(MSG_NB_HMI_SEND_SELVATGEND,m_idMe);
}

CtmTaskDsp54::~CtmTaskDsp54()
{
	DeleteCounter(&m_lCheckQuickTimer, m_idMe);
	//DeleteCounter(&m_lUpdateTimer, m_idMe);
	DeleteCounter(&m_lOnLineTime, m_idMe);
	DeleteCounter((long *)&g_lCounterLEDAdjMold, m_idMe);
	DeleteCounter((long *)&g_lCounterLEDMotor, m_idMe);
	DeleteCounter((long *)&m_tled.lCount, m_idMe);
	//DeleteCounter((long *)&m_lCounterW1, m_idMe);
	//DeleteCounter((long *)&m_lCounterBW1, m_idMe);
	//DeleteCounter((long *)&m_lCounterW0, m_idMe);
	//DeleteCounter((long *)&m_lCounterSpeechDelayOnOff, m_idMe);
	UnRegisterAllMessage(MSG_DSP54);
	UnRegisterAllMessage(MSG_NB_HMI_SEND_SELVATGEND); 
}

int		CtmTaskDsp54::CreateSelf()
{
	CTaskHost::CreateSelf();
	
	#ifdef D_ARM
	usleep(50*1000);
	system("ifconfig eth0 192.168.3.211");
	usleep(50*1000);
	system("ifconfig eth0 up");
	#endif
	m_pHostProtocol	= static_cast<CtmHostProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_54net"));
	pthread_attr_init(&m_ThreadAttr);
	pthread_attr_setdetachstate(&m_ThreadAttr, PTHREAD_CREATE_DETACHED);
	pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
	pthread_create(&m_RunThreadID, NULL, (PRSRUN)_Run, NULL);
		
	return 0;
}

int		CtmTaskDsp54::FreeSelf()
{
	g_pPackage->ReleaseClassInstance("HostProtocol_54net", m_pHostProtocol);
	
	if (m_nDeviceID	!= -1)
		DelDevice(m_nDeviceID);
	
	return CTaskHost::FreeSelf();
}


void	CtmTaskDsp54::_Open(void *pData)
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	    ;
	
	m_pSelf->m_nConnecting				= 1;
	m_pSelf->m_ethernet.srcAddr 		= 192 | 168 << 8 | 3 << 16 | 211 << 24;  //HMI
	m_pSelf->m_ethernet.dstAddr 		= 192 | 168 << 8 | 3 << 16 | 124 << 24;  //DSP55
	//m_pSelf->m_ethernet.srcAddr 		= 192 | 168 << 8 | 0 << 16 | 185 << 24;
	//m_pSelf->m_ethernet.dstAddr 		= 192 | 168 << 8 | 0 << 16 | 50 << 24;
	m_pSelf->m_ethernet.wsrcPort		= 2311;
	m_pSelf->m_ethernet.wdstPort		= 2311;
	m_pSelf->m_ethernet.nDomain 		= AF_INET;
	m_pSelf->m_ethernet.nType   		= SOCK_DGRAM;
	m_pSelf->m_ethernet.wRxBufferSize 	= sizeof(m_pSelf->m_RxBuffer);
	m_pSelf->m_ethernet.fpRxBuffer	 	= m_pSelf->m_RxBuffer;
	m_pSelf->m_ethernet.wTxBufferSize 	= sizeof(m_pSelf->m_TxBuffer);
	m_pSelf->m_ethernet.fpTxBuffer	 	= m_pSelf->m_TxBuffer;
	
	if (m_pSelf->m_nDeviceID	!= -1) 
		DelDevice(m_pSelf->m_nDeviceID);
		
	m_pSelf->m_nDeviceID	= -1;
	m_pSelf->m_pHostProtocol->SetDeviceID(m_pSelf->m_nDeviceID);

	while (TRUE)
	{
		if (m_pSelf->m_nDeviceID == -1)
		{
			m_pSelf->m_ethernet.NetFunc		= (NETFUNC)ResValueFunc;
			m_pSelf->m_nDeviceID			= AddDevice("CEthernet", &m_pSelf->m_ethernet);
			
			//printf("test--------------m_pSelf->m_nDeviceID:%d\n",m_pSelf->m_nDeviceID);
			m_pSelf->WriteValue(0xFF7771, NULL, 0);		//Clear TransBuff
			if(m_pSelf->m_nDeviceID != -1 )	
			{
			    m_pSelf->m_pHostProtocol->SetDeviceID(m_pSelf->m_nDeviceID);
			    break;
			}
			else
			    usleep(CNT_INT_CHECK_ONLNE_TIME * 1000);
		}
	}
	m_pSelf->m_nConnecting	= 0;
}

int		CtmTaskDsp54::ReqValues(int nType)
{
    return m_pHostProtocol->Compose(nType, NULL, NULL, 0, NULL);
}

int		CtmTaskDsp54::ReqValues(int nType, int nHostAdreess1, int nHostAdreess2)
{
    int nHostAdreess[2];
    nHostAdreess[0] = nHostAdreess1;
    nHostAdreess[1] = nHostAdreess2;
    return m_pHostProtocol->Compose(nType, NULL, nHostAdreess, 2, NULL);
}

int 	CtmTaskDsp54::ReqValues(int nType, void* pOutData, int nSize)
{
    return m_pHostProtocol->Compose(nType, pOutData, NULL, nSize, NULL);
}

int		CtmTaskDsp54::WriteValue(int nDataID)
{
    if(nDataID == COMM_BPROFILE01_S)
        m_lCounterW1 = 512;
    else if(nDataID == COMM_SENDS_BPROFILE01)
        m_lCounterBW1 = 512;
    return m_pHostProtocol->Compose(nDataID, NULL, NULL, 0, NULL);
}

int		CtmTaskDsp54::WriteValue(int nType, int nHostAdreess1, int nHostAdreess2)
{
    if(nType == COMM_ETH_SENDPANEL && LOBYTE(nHostAdreess1) == 0xFF)
        m_lCounterW0 = 512;
    int nHostAdreess[2];
    nHostAdreess[0] = nHostAdreess1;
    nHostAdreess[1] = nHostAdreess2;
    return m_pHostProtocol->Compose(nType, NULL, nHostAdreess, 2, NULL);
}

/*!	\fn int	CtmTaskDsp54::WriteValue(int nType, void* pInputData, int nSize)
 *	\brief	發送數據
 *	\param	[in]	nType		發送數據類型
 *	\param	[void*]	pInputData	發送數據內容
 *	\param	[int]	nSize		發送數據長度
 *	\return m_pHostProtocol->Compose(nType, NULL, pInputData, nSize, NULL);
 */
int 	CtmTaskDsp54::WriteValue(int nType, void* pInputData, int nSize)
{	
	if(nType == COMM_ETH_DEVICEID)		//daniel add 2010/01/29 更新前傳入選擇的DEVICEID
	{
		nType = COMM_ETH_SELECT54;
		pInputData = (void*)&m_pSelf->m_nDeviceID;	
	}
	else if(nType==COMM_ETH_SELECT28)
	{
		pInputData = (void*)&m_pSelf->m_nDeviceID;	
	}
    return m_pHostProtocol->Compose(nType, NULL, pInputData, nSize, NULL);
}

void	CtmTaskDsp54::WndProc(int message, WPARAM wParam, LPARAM lParam) 
{
	CTask::WndProc(message, wParam, lParam);

	switch (message)
	{
		case MSG_NB_HMI_SEND_SELVATGEND:
			{
   				printf("message =%x\n",message);
		    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED77",0,FALSE);/*wangli2015-5-20 18:29:51 清除 關模低壓終止位置精度為2 的flag*/
			}
		break;
		default:
			break;
	}
}

/*!	\fn int	CtmTaskDsp54::GetData(void* pdata, int nLen)
 *	\brief	接收發送過來的數據
 *	\param	[in]	pdata	接收的數據
 *	\param	[in]	nLen	接收的數據長度
 *	\return CTaskHost::GetData(pdata, nLen)
 */
int		CtmTaskDsp54::GetData(void* pdata, int nLen)
{
	g_bUpdateFlag	= TRUE;
	g_wSentReqState = 1;			//James add 2009/9/1 上午 11:45:17  如果有收到數據，就不能發送探測數據包了。
    m_lCounterReqConfig = COMM_TIME_REQCONFIG;
    m_lCheckQuickTimer = CNT_INT_CHECK_ONLNE_TIME;
	m_pHostProtocol->Parse(pdata, nLen);
	int nResult=CTaskHost::GetData(pdata, nLen);
	g_wSentReqState    = 0;
	return nResult;
}

/*!	\fn int	CtmTaskDsp54::ResValueFunc(char *pData, int nLength)
 *	\brief	接收主機發送過來的數據(采用輪回方式接收數據)
 *	\param	[in]	pdata	接收的數據
 *	\param	[in]	nLen	接收的數據長度
 *	\return 
 */
void	CtmTaskDsp54::ResValueFunc(char *pData, int nLength)
{
	pthread_mutex_lock(&g_setFlagLock);
	m_pSelf->GetData(pData, nLength);
	MSG		msg;
	memset(&msg,0,sizeof(MSG));
	msg.message	= MSG_DSP54;
	g_pApplication->QueueMessage(&msg);
	pthread_mutex_unlock(&g_setFlagLock);
}

/*!	\fn int	CtmTaskDsp54::GetOnLineStatus()
 *	\brief	判斷連線是否正常		
 *	\return 連線狀態
 */
int		CtmTaskDsp54::GetOnLineStatus()
{
	if (m_lCheckQuickTimer <= 0)
	{
		//m_nConnectStatus = 1 表示重新鏈接  =2表示斷線狀態 =0表示連接上
		m_nConnectStatus = m_nConnectStatus | 1;
		BOOL	bConnect	= FALSE;
		
		if ((GetState(m_nDeviceID) != DEVICE_S_OPENED) &&
			(m_nConnecting != 1))
		{
			m_nConnecting	= 1;
			m_nConnectStatus = m_nConnectStatus | 2;
			bConnect		= TRUE;
			usleep(50*1000);//pthread_cancel(m_OpenThreadID);
			pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		}
		
		if ((!bConnect) && (m_nConnecting != 1))
		{
			m_nConnecting	= 1;
			m_nConnectStatus = m_nConnectStatus | 2;
			usleep(50*1000);//pthread_cancel(m_OpenThreadID);
			pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		}
		m_lCheckQuickTimer = CNT_INT_CHECK_ONLNE_TIME;
		g_wDspStatus	   = FALSE;			//當重新連線，則顯示掉線狀態 James add 
	} 
	
	return m_nConnectStatus;
	
}

void	CtmTaskDsp54::Run()
{
	u_wThreadStartRunFlag = TRUE;
}
void	CtmTaskDsp54::_Run()
{
	while(1)
	{
		if(u_wThreadStartRunFlag)
		{
			pthread_mutex_lock(&g_setFlagLock);
			Run_Pthread();
			u_wThreadStartRunFlag=FALSE;
			pthread_mutex_unlock(&g_setFlagLock);
		}
		usleep(10*1000);
	}
}

void	CtmTaskDsp54::Run_Pthread()
{
	//long		lValue;
	//BYTE        cStateRx;
	//BOOL        bGotConfig;
	
	if ((g_lCounterLEDMotor <= 0) && (g_flMotorStatus == 1)) //ChunTzu 2004/3/19 mark for motor led//wy030599 add for check 0,1,-1
    {
	    g_lCounterLEDMotor = 250;
	    if ((g_bLEDMotor = !g_bLEDMotor))	
	    {
	    	PanelMotorOn();
	    }
	    else
	    {
	    	_UnActLed(LED_PNL_TM_MOTOR_ON);
	    }
    }
    if ((dbbuff.cIndexMode >= 11) && 
    	(dbbuff.cIndexMode <= 14) && 
    	(g_lCounterLEDAdjMold <= 0))  
    {
	    g_lCounterLEDAdjMold = 250;	    
	    
	    if((g_bLEDAdjMold = !g_bLEDAdjMold))
	    	PanelAutoAdjOn();
	    else
	    	_UnActLed(LED_PNL_TM_ADJUSTO_ATUO_OFF);
    }
    
	if(!g_wSentReqState)
	{
		if(m_pSelf->m_lCounterReqConfig <= 0)	
		{
			m_pSelf-> m_lCounterReqConfig = COMM_TIME_REQCONFIG;
			 WORD		wTempCmd = 0;
			m_pSelf->WriteValue(COMM_ETH_SENDCONNET, (void*)&wTempCmd, sizeof(wTempCmd));
		}
	}
	
	m_pSelf->WriteValue(0xFF01, NULL, 0);/*JOYCE2014-10-17 tmDsp54All.cpp增加run(), 方便功能處理*/

	if (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue ==ID_VIEW_PANEL)     //KV2003/2/12 add for LED test
	{
		if (m_pSelf->m_tled.lCount <= 0) m_pSelf->LED();       // Service LED test
	}
	
	//判斷連線狀態： 如果5S沒有收到dsp54的及時數據，說明是斷線，就需要重新鏈接
	m_pSelf->m_nConnectStatus = m_pSelf->GetOnLineStatus();
	if ((m_pSelf->m_nConnectStatus == 0) && (bSpeechDelayOnOff) && (m_lCounterSpeechDelayOnOff < 0))
    {
        bSpeechDelayOnOff = FALSE;
	    //::DeleteCounter((long *)&m_lCounterSpeechDelayOnOff, m_idMe);
	    m_lCounterSpeechDelayOnOff = 100;
    }
}

void  CtmTaskDsp54::LED()
{
	m_tled.lCount = TLED_SLICE;
	_Save_Restore_LED();
}

#ifndef	D_BEOBJECT_CTMTASKDSP54
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
