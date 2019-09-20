/*==========================================================================+
|  Class    : Ethernet                                                      |
|  Task     : Ethernet service routine file                            		|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2005/10/27                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
typedef unsigned short u16;
typedef unsigned int u32;
#ifdef	D_ARM
typedef long long u64;
#endif
typedef unsigned char u8;
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include "ethnet.h"
#include "tmconfig.h"
#include <sys/types.h>
#include <sys/socket.h>

//#include <linux/tcp.h>
#include <netinet/tcp.h>
#include	"tmdebug.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		ETH_QUEUE_FULL			0x0001
#define		ETH_QUEUE_EMPTY			0x0002

#ifdef	D_PXA270
#define		FLASH_PHYADDRESS			"/dev/mtdblock4"
#else
#define		FLASH_PHYADDRESS			"/dev/mtdblock1"
#endif

#define		LEN_MAC					6

#define		IS_TCP					(this->m_ethernet.nType == SOCK_STREAM)
#define		IS_TCP0					(peth->m_ethernet.nType == SOCK_STREAM)
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void *(* PRSRUN)(void *);

typedef struct tagtmNETDATA
{
	int		iLength;	// 當前數據包長度
	BYTE*	pNext;		// 下一個數據包起始位置
	BYTE	cData[];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmNETDATA;
#else 
#ifdef	D_X86
tmNETDATA;
#endif
#endif

sem_t	CEthernet::m_TxWait[MAX_ETH_COUNT];
int		CEthernet::m_nUsed[MAX_ETH_COUNT]	= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int	CEthernet::m_nCount	= 0;
/*==========================================================================+
|           Class implementation - Ethernet                                 |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CEthernet,CDevice)

CEthernet::CEthernet() : CDevice()
{
	int		nIndex = 0;
	
	if (m_nCount == 0)	signal(SIGPIPE, SIG_IGN);
	if(m_nCount >= MAX_ETH_COUNT)
	{
		m_nIndex = -1;
		return;
	}
	
	while(m_nUsed[nIndex] != 0 && nIndex < MAX_ETH_COUNT) nIndex ++;
	if(nIndex >= MAX_ETH_COUNT)
	{	
		m_nIndex = -1;
		return;
	}
	m_nIndex	= nIndex;
	m_nUsed[m_nIndex]	= 1;
	m_nCount	++;
}

CEthernet::~CEthernet()
{
	if(m_nIndex < 0)	return;
	Close();
	
	m_nUsed[m_nIndex]	= 0;
	m_nIndex			= -1;
	m_nCount --;
}

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :                                                               |
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CEthernet::Create(void* pDevice)
{
	if(m_nIndex == -1)
	{
		m_wOpStatus			= DEVICE_S_ERROR;
		return m_wOpStatus;
	};	
	m_ethernet 			= *(ETHERNET *)pDevice;
	m_ethData.iHandle	= -1;
	m_wOpStatus			= DEVICE_S_CLOSED;
	m_wStatus			= DEVICE_SUCCESS;
	
	//=======
	// 設置本機的傳輸信息
	//=======
	m_srcAddr.sin_family		= m_ethernet.nDomain;
	m_srcAddr.sin_port			= htons(m_ethernet.wsrcPort);
	m_srcAddr.sin_addr.s_addr 	= m_ethernet.srcAddr;//INADDR_ANY; //LEO20070627
	bzero(&(m_srcAddr.sin_zero),8);
	m_ethData.srcAddr			= m_srcAddr;
	//=======
	// 設置目標機的傳輸信息
	//=======
	m_dstAddr.sin_family		= m_ethernet.nDomain; //AF_INET;
	m_dstAddr.sin_port			= htons(m_ethernet.wdstPort);
	m_dstAddr.sin_addr.s_addr 	= m_ethernet.dstAddr;
	bzero(&(m_dstAddr.sin_zero),8);
	m_ethData.dstAddr			= m_dstAddr;
	m_wOpStatus = Open();
	return m_wOpStatus;
}

WORD    CEthernet::Open()
{	
	_NETQUEUE*	pQueue;
	int			nReuse = 1;
	
	if(m_wOpStatus == DEVICE_S_OPENED)
		goto SOCKETOPENED;
		
	//=======
	// 創建新的套接字，UDP
	//=======
	m_ethData.iHandle	= socket(m_ethernet.nDomain, m_ethernet.nType, 0);
	           
//	fcntl(m_ethData.iHandle,F_SETFL,O_NONBLOCK);
	if(m_ethData.iHandle == -1)
		goto SOCKETERROR;
	setsockopt(m_ethData.iHandle, SOL_SOCKET, SO_REUSEADDR, &nReuse, sizeof(nReuse));  
	
	//if (m_ethernet.nType == SOCK_STREAM)
	//{
	//	int on=1;
	//	setsockopt(m_ethData.iHandle, IPPROTO_TCP, TCP_NODELAY,&on,sizeof(on));  
	//}
	//=======
	//　向系統申請使用本地端口
	//=======
	if(m_ethernet.wsrcPort != 0)		// 如果是0，由系統自動分配
	{
		if(bind(m_ethData.iHandle, (struct sockaddr *)&m_srcAddr,
			  sizeof(struct sockaddr)) == -1)
			goto BINDERROR;
	}
	
	//=======
	// 說明套接字目標機的配置
	//=======	
	if (IS_TCP)
	{
		printf("Open\n");
		if(connect(m_ethData.iHandle, (struct sockaddr *)&m_dstAddr,sizeof(struct sockaddr))== -1)
			{
				perror("connect");
				goto BINDERROR;
			}
	}
	if (IS_TCP)
	{
		if(m_ethernet.KeepAlive == 1)
		{
			setsockopt(m_ethData.iHandle,SOL_SOCKET,SO_KEEPALIVE,
							(void*)&(m_ethernet.KeepAlive),sizeof(m_ethernet.KeepAlive));
			setsockopt(m_ethData.iHandle,SOL_TCP,	TCP_KEEPIDLE,
							(void *)&(m_ethernet.KeepIdle),sizeof(m_ethernet.KeepIdle));
			setsockopt(m_ethData.iHandle,SOL_TCP,	TCP_KEEPINTVL,
							(void *)&(m_ethernet.KeepInterval),sizeof(m_ethernet.KeepInterval));
			setsockopt(m_ethData.iHandle,SOL_TCP,	TCP_KEEPCNT,
							(void *)&(m_ethernet.KeepCount),sizeof(m_ethernet.KeepCount)) ;
       	}                                                                                                                                                                                      	
	}
	//=======
	// 創建接收和發送的緩沖區
	//=======
	pQueue			= &m_ethData.queueRx;
	
	pQueue->bFull	= FALSE;	
	pQueue->pStart	=
	pQueue->pInput	=
	pQueue->pOutput	= m_ethernet.fpRxBuffer;
	pQueue->pEnd	= m_ethernet.fpRxBuffer  + m_ethernet.wRxBufferSize;
	pQueue->pLast	= NULL;	
	pthread_mutex_init(&pQueue->mutex, NULL);
	
	pQueue			= &m_ethData.queueTx;
	
	pQueue->bFull	= FALSE;	
	pQueue->pStart	=
	pQueue->pInput	=
	pQueue->pOutput	= m_ethernet.fpTxBuffer;
	pQueue->pEnd	= m_ethernet.fpTxBuffer  + m_ethernet.wTxBufferSize; 
	pQueue->pLast	= NULL;
	pthread_mutex_init(&pQueue->mutex, NULL);
	
	m_ethData.NetFunc	= m_ethernet.NetFunc;
	
	m_ethData.nIndex	= m_nIndex;
	//=======
	// 創建數據發送線程
	//=======
	sem_init(&m_ethData.ThreadWait, 0, 0);
	sem_init(&m_TxWait[m_nIndex], 0, 0);

	if(pthread_create(&m_ethData.RxThread, NULL, 
		(PRSRUN)Ethernet_RxRun, this) != 0)
	{
		//perror("CEthernet::Open Ethernet_RxRun:");
	}
	sem_wait(&m_ethData.ThreadWait);
	
	//=======
	// 創建數據接收線程
	//=======
	

	if(pthread_create(&m_ethData.TxThread, NULL, 
		(PRSRUN)Ethernet_TxRun, this) != 0)
	{
		//perror("CEthernet::Open Ethernet_TxRun:");
	}	
	sem_wait(&m_ethData.ThreadWait);
	
	m_wOpStatus	= DEVICE_S_OPENED;
	goto SOCKETOPENED;

BINDERROR:
	//perror("bind");
	close(m_ethData.iHandle);
	m_ethData.iHandle	= -1;
SOCKETERROR:
	//perror("socket");
	m_wOpStatus	= DEVICE_S_CLOSED;
SOCKETOPENED:
	return m_wOpStatus;
}

WORD    CEthernet::Close()
{	
	//printf("CEthernet::Close() ::m_nIndex = %d, m_wOpStatus = %d\n", m_nIndex, m_wOpStatus);
	if(m_nIndex < 0)	return DEVICE_S_CLOSED;
	if(m_ethData.iHandle == -1)  return DEVICE_S_CLOSED;
	//if(m_wOpStatus == DEVICE_S_CLOSED)	return DEVICE_S_CLOSED;
		
	//=======
	// 關閉收發進程
	//=======
	pthread_cancel(m_ethData.RxThread);
	pthread_cancel(m_ethData.TxThread);
	
	//=======
	// 釋放信號量
	//=======
	sem_destroy(&m_ethData.ThreadWait);
	sem_destroy(&m_TxWait[m_nIndex]);
	
	//=======
	// 釋放互斥鎖
	//=======
	pthread_mutex_destroy(&m_ethData.queueRx.mutex);
	pthread_mutex_destroy(&m_ethData.queueTx.mutex);

	//=======
	// 關閉socket
	//=======
	close(m_ethData.iHandle);
	m_ethData.iHandle	= -1;
	m_wOpStatus	= DEVICE_S_CLOSED;
	return m_wOpStatus;
}

WORD    CEthernet::Read (BYTE* pch, int nCount, int* pnReadCount)
{
	BYTE	*pData = NULL;
	if(pch == NULL || pnReadCount == NULL || nCount <= 0)
		return DEVICE_E_FAILURE;

	if((*pnReadCount = ReadQueue(&pData, nCount, &m_ethData.queueRx)) == -1)
		return DEVICE_E_RX_EMPTY;
	memcpy(pch, pData, *pnReadCount);
	
	return DEVICE_SUCCESS;
}

WORD    CEthernet::Read (BYTE* pch, BYTE cTerminator, int* pnReadCount)
{
	return DEVICE_ERR_FUNCTION;
}

WORD    CEthernet::Write(BYTE  ch)
{
	return DEVICE_ERR_FUNCTION;
}

WORD    CEthernet::Write(BYTE* pch, int nCount, int* pnWriteCount)
{
	int	Value;
	
	if(pch == NULL || pnWriteCount == NULL || nCount <= 0)
		return DEVICE_E_FAILURE;

	if(QueueState(&m_ethData.queueTx) == ETH_QUEUE_EMPTY)
	{
		if (IS_TCP)
			*pnWriteCount	= send(m_ethData.iHandle, pch, nCount, MSG_NOSIGNAL);
		else
			*pnWriteCount = sendto(m_ethData.iHandle, pch, nCount, MSG_NOSIGNAL, 
       							 (const sockaddr*)&m_ethData.dstAddr, sizeof(sockaddr));
			
		//printf("CEthernet::Write1 state=%d\n", *pnWriteCount);
		if (*pnWriteCount <= 0)
		{
			m_wOpStatus	= DEVICE_S_CLOSED;
			return DEVICE_ERR_DEVICE;
		}
		return DEVICE_SUCCESS;
	}
	
	if((*pnWriteCount = WriteQueue(pch, nCount, &m_ethData.queueTx)) == -1)
		return DEVICE_E_TX_FULL;
//	//printf("CEthernet::Write\n");
	sem_getvalue(&m_TxWait[m_nIndex], &Value);
	if(Value == 0)
		sem_post(&m_TxWait[m_nIndex]);
	
	return DEVICE_SUCCESS;
}

WORD    CEthernet::ReadToken (BYTE* pch, int nBufferSize, int* pnReadCount)
{
	return Read(pch, nBufferSize, pnReadCount);
}
WORD    CEthernet::WriteToken(BYTE* pch, int nCount, int* pnWriteCount)
{
	return Write(pch, nCount, pnWriteCount);
}

WORD    CEthernet::GetRxState(WORD* pwState)
{
	return DEVICE_ERR_FUNCTION;
}
WORD    CEthernet::GetTxState(WORD* pwState)
{
	return DEVICE_ERR_FUNCTION;
}

WORD    CEthernet::AbortRx()
{
	return DEVICE_ERR_FUNCTION;
}
WORD    CEthernet::AbortTx()
{
	return DEVICE_ERR_FUNCTION;
}

WORD    CEthernet::GetOpStatus()
{
	struct ifreq ifrInfo;
	struct ethtool_value ethData;
	
	if(m_wOpStatus != DEVICE_S_OPENED)	return DEVICE_E_FAILURE;
	if(m_ethData.wState != DEVICE_S_OPENED)	return DEVICE_E_FAILURE;
	
	memset(&ifrInfo, 0, sizeof(ifrInfo));
	ethData.cmd = ETHTOOL_GLINK;

	if(CtmConfig::GetInstance()->GetINETMacSwtich() ==0)		//Sunny<20080816>
		strncpy(ifrInfo.ifr_name, "eth0", sizeof(ifrInfo.ifr_name));//eth0
	else if(CtmConfig::GetInstance()->GetINETMacSwtich() ==1)		
		strncpy(ifrInfo.ifr_name, "eth1", sizeof(ifrInfo.ifr_name));
	
	//strncpy(ifrInfo.ifr_name, "eth1", sizeof(ifrInfo.ifr_name));//eth0
	ifrInfo.ifr_data = (char *) &ethData;

	if (ioctl(m_ethData.iHandle, SIOCETHTOOL, &ifrInfo) == -1)
	{
//	    //printf("Ehernet failed: %s\n", strerror(errno));
	    return DEVICE_E_FAILURE;
	}
	
	return (ethData.data ? DEVICE_S_OPENED : DEVICE_E_FAILURE);//incorrect
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CEthernet::Ethernet_RxRun(CEthernet* peth)
{
	int			iLength;
	
	BYTE		tmpBuff[MAX_ETHTOKEN_SIZE];
	
	sem_post(&peth->m_ethData.ThreadWait);
	
	//=======
	// 設置線程退出後，立即釋放所有資源
	// 設置線程收到cancel信號後，立刻退出
	//=======
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		//perror("CEthernet::Ethernet_RxRun setcanceltype");
		return;
	}
	peth->m_ethData.wState	= DEVICE_S_OPENED;
	
	//=======
	// 數據接收過程
	//=======
	//printf("Before Handle=%ld buff=%ld Domain=%ld\n", peth->m_ethData.iHandle, tmpBuff, AF_INET);
	while(TRUE)
	{
		if((iLength = recv(peth->m_ethData.iHandle, tmpBuff, MAX_ETHTOKEN_SIZE, MSG_NOSIGNAL)) <= 0)
		{			
			//printf("Handle=%ld buff=%ld Domain=%ld,iLength = %d\n", peth->m_ethData.iHandle, tmpBuff, PF_INET, iLength);
			
			if(iLength == 0)
			{
				peth->m_ethData.wState	= DEVICE_S_CLOSED;
				sleep(1);
				//printf("Server connection error!");
				struct sockaddr_in addr;
				
				addr			= peth->m_ethData.dstAddr;
				addr.sin_family	= AF_UNSPEC;
				
				if (IS_TCP0)
				{
					while(connect(peth->m_ethData.iHandle, (struct sockaddr *)&addr,
				  		sizeof(struct sockaddr)) == -1)
				  	{
				  		sleep(1);	
				  		perror("Ethernet_RxRun unconnect");
				  	}
				}
			  	peth->m_ethData.wState	= DEVICE_S_OPENED;
			  	continue;
			}
			else
			{
				//printf("Handle=%ld buff=%ld Domain=%ld\n", pethData->iHandle, tmpBuff, PF_INET);
				peth->m_ethData.wState	= DEVICE_S_CLOSED;
				//perror("Ethernet_RxRun");
				//pethData->iHandle = -1;
				continue;
				
			}
		}
		//printf("Handle=%ld buff=%ld Domain=%ld,iLength = %d\n", peth->m_ethData.iHandle, tmpBuff, PF_INET, iLength);	
		if(peth->m_ethData.NetFunc == NULL)			
			peth->WriteQueue(tmpBuff, iLength, &peth->m_ethData.queueRx);
		else
			(peth->m_ethData.NetFunc)((char *)tmpBuff, iLength);
	}
}


void*	CEthernet::Ethernet_TxRun(CEthernet* peth)
{		
	int			iLength;
	BYTE		*pch;
	_NETQUEUE 	*pQueue;
	
	sem_post(&peth->m_ethData.ThreadWait);
	
	pQueue	= &peth->m_ethData.queueTx;;
	//=======
	// 設置線程退出後，立即釋放所有資源
	// 設置線程收到cancel信號後，立刻退出
	//=======
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		//perror("CEthernet::Ethernet_TxRun setcanceltype");
	}
	while(TRUE)
	{	
		// 緩沖區為空，則等待
		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == FALSE)
		{
			//sem_wait(&(pethData->TxWait));
			sem_wait(&m_TxWait[peth->m_ethData.nIndex]);
			////printf("Ethernet_TxRun\n");
			continue;
		}
		iLength	= peth->ReadQueue(&pch, MAX_ETHTOKEN_SIZE, &peth->m_ethData.queueTx);
		if(iLength > 0)
		{       							 
       		if (IS_TCP0)
       		{
				if (send(peth->m_ethData.iHandle, pch, iLength, MSG_NOSIGNAL) == -1)
				{
					peth->m_ethData.wState	= DEVICE_S_CLOSED;
					perror("Ethernet_TxRun");
				}
			}
			else
			{
				if (sendto(peth->m_ethData.iHandle, pch, iLength, MSG_NOSIGNAL, 
       						(const sockaddr*)&peth->m_ethData.dstAddr, sizeof(sockaddr)) == -1)
				{
					peth->m_ethData.wState	= DEVICE_S_CLOSED;
					perror("Ethernet_TxRun");
				}
			}
				
		}
	}
}

int	CEthernet::WriteQueue(BYTE *pch, int iLength, _NETQUEUE *pQueue)
{
	tmNETDATA*	pData;
	
	if(pQueue->bFull == TRUE)	
		{
//		//printf("the queue is full!\n");
		return -1;
		}
		
	if(pQueue->pInput >= pQueue->pOutput)
	{
		if((unsigned int)(pQueue->pEnd - pQueue->pInput) >= 
			sizeof(tmNETDATA) + iLength)
		{
			pData			= (tmNETDATA *)pQueue->pInput;
			pData->iLength	= iLength	+ sizeof(tmNETDATA);
			memcpy(pData->cData, pch, iLength);
			
			pQueue->pInput += pData->iLength;
		} 
		else if((unsigned int)(pQueue->pStart - pQueue->pOutput) >= 
			sizeof(tmNETDATA) + iLength)
		{
			pData			= (tmNETDATA *)pQueue->pStart;
			pData->iLength	= iLength	+ sizeof(tmNETDATA);
			memcpy(pData->cData, pch, iLength);
			
			pQueue->pInput	= pQueue->pStart + pData->iLength;
			
		}
		else
			return -1;
	}
	else if((unsigned int)(pQueue->pOutput - pQueue->pInput) >= 
		sizeof(tmNETDATA) + iLength)
	{
		pData			= (tmNETDATA *)pQueue->pInput;
		pData->iLength	= iLength	+ sizeof(tmNETDATA);
		memcpy(pData->cData, pch, iLength);
		
		pQueue->pInput += pData->iLength;
	}
	else
		return -1;
	
	pthread_mutex_lock(&pQueue->mutex);
	
	if(pQueue->pLast != NULL)
	{
		((tmNETDATA *)pQueue->pLast)->pNext	= (BYTE *)pData;		
		
		if(pQueue->pOutput == pQueue->pInput)
			pQueue->bFull = TRUE;
	}
	else	// 表示所有的數據已經被讀走
		pQueue->pOutput	= (BYTE *)pData;
		
	pData->pNext	= NULL;
	pQueue->pLast	= (BYTE *)pData;
	pthread_mutex_unlock(&pQueue->mutex);
	
	return pData->iLength;
}


int	CEthernet::ReadQueue(BYTE **pch, int iLength, _NETQUEUE *pQueue)
{
	tmNETDATA*	pData;
	
	if( pQueue->bFull == FALSE && 
		(pQueue->pOutput == pQueue->pInput) )	return -1;
	
	pData	= (tmNETDATA *)pQueue->pOutput;
	iLength	= ((unsigned int)iLength < pData->iLength - sizeof(tmNETDATA)) ?
				iLength : (pData->iLength - sizeof(tmNETDATA));	
	
	*pch	= pData->cData;
	pthread_mutex_lock(&pQueue->mutex);
	if(pData->pNext != NULL)
		pQueue->pOutput		= pData->pNext;
	else
	{
		pQueue->pOutput	   += pData->iLength;
		pQueue->pLast		= NULL;
	}	
	pQueue->bFull = FALSE;
	
	pthread_mutex_unlock(&pQueue->mutex);
	
	return iLength;
}

int	CEthernet::QueueState(_NETQUEUE *pQueue)
{
	if(pQueue->bFull == FALSE && (pQueue->pOutput == pQueue->pInput))
		return ETH_QUEUE_EMPTY;
	if(pQueue->bFull == TRUE)
		return ETH_QUEUE_FULL;
	return 0;
}

int		GetRealPhysicalAddr(BYTE *pMAC, char* pszEth)
{
	register 	int 	fd, intrface;
	struct 		ifreq 	buf[8];
	struct 		ifconf 	ifc;
	char		szEth[20];
	
	if (pszEth == NULL)
	{
		int	nTemp = CtmConfig::GetInstance()->GetINETMacSwtich();
		sprintf(szEth, "%s%d", "eth", nTemp);
	}
	else 
	{
		sprintf(szEth, "%s", pszEth);	
	}
	
	if (pMAC == NULL)	return -1;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
	{
		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = (caddr_t)buf;
		if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)) 
		{
			intrface = ifc.ifc_len / sizeof(struct ifreq);
			while (intrface-- > 0)
			{
				//if (strcmp(buf[intrface].ifr_name, "lo") == 0)	continue;
				if (strcmp(buf[intrface].ifr_name, szEth) != 0)	continue;	//Sunny<20080816>
				if (!(ioctl(fd, SIOCGIFHWADDR, (char *)&buf[intrface])))
				{
					memcpy((char *)pMAC, (char *)buf[intrface].ifr_hwaddr.sa_data, 6);
				}
				else 	
				{
					close(fd);
					return -1;
				}
			}
		}
		close(fd);
		return	0;
	}
	else	return -1;
}


BOOL	SetPhysicalAddr(BYTE *pMAC)
{
	int 	fd; 
	BOOL 	bResult = TRUE;
	
	if(pMAC == NULL)					return FALSE;
	
	if ((fd = open(FLASH_PHYADDRESS, O_RDWR)) < 0)	
	{
		printf("flash open error, %s\n", strerror(errno));
		return FALSE;
	}
	if(lseek(fd, 0, 0) < 0)		
		printf("could not seek to device, %s\n", strerror(errno));
	// write net physical address from flash
	if(write(fd, pMAC, LEN_MAC) != LEN_MAC)
	{	
		bResult = FALSE;
		printf("could not write to device, %s\n", 
													strerror(errno));
	}
	close(fd);
	
	return bResult;
}
