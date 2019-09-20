/*==========================================================================+
|  Class    : EthNet                                                        |
|  Task     : EthNet service routine header file                            |
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
#ifndef	D_DETHERNET
#define	D_DETHERNET

#include "device.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		ETH_PACTETSIZE_MAX		1514			// packet包最大值
#define		ETH_PACTETSIZE_MIN		62				// packet包最小值
#define		MAX_ETHTOKEN_SIZE		8192

#define		ETH_TCP					0x0001
#define		MAX_ETH_COUNT			10
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void * (* NETFUNC)(char *, int);

typedef struct tag_NETQUEUE
{
    WORD    wSize;
    WORD    wGuardBand;
    BYTE* 	pStart;
    BYTE* 	pEnd;
    BYTE* 	pInput;
    BYTE* 	pOutput;
    BOOL    bFull;
    BYTE*	pLast;		// 指向最後一次寫入的包的包頭位置
    pthread_mutex_t	mutex;
} 	_NETQUEUE;

typedef struct tagtmETHERNET
{
	DWORD	srcAddr;			//LEO20070627
	DWORD	dstAddr;
	WORD	wsrcPort;
	WORD	wdstPort;
	int		nDomain;
	int		nType;
	
	int 	KeepAlive; 			//TCP KeepAlive
	int 	KeepIdle; 			
	int 	KeepInterval;
	int 	KeepCount;	
	
	WORD        wRxBufferSize;  // Size of the receive buffer
	BYTE*		fpRxBuffer;     // Far pointer to the receive buffer
	WORD        wTxBufferSize;  // Size of the transmit buffer
	BYTE*		fpTxBuffer;     // Far pointer to the transmit buffer
	
	NETFUNC		NetFunc;
} 	ETHERNET;

typedef struct tag_IETHERNET
{
    ETHERNET* 	pethernet;

    WORD    wState;
    WORD    wRxState;
    WORD    wTxState;
    WORD    wErrorCount;
    WORD	wAction;
    
    _NETQUEUE	queueRx;
    _NETQUEUE	queueTx;
        
	int			iHandle;				// handle;
	pthread_t	RxThread;				// 接收線程ID
	pthread_t	TxThread;				// 發送線程ID
	sem_t		ThreadWait;				// 創建線程信號
//	sem_t		TxWait;					// 發送線程信號
//	sem_t		RxWait;
	NETFUNC		NetFunc;
	
	struct sockaddr_in 		dstAddr;
	struct sockaddr_in		srcAddr;
	
	int			nIndex;
} 	_IETHERNET;
/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - ETHNET                                      |
+==========================================================================*/
class CEthernet : public CDevice
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CEthernet)
public:
    CEthernet();
    ~CEthernet();

/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/
private:
	ETHERNET		m_ethernet;
	_IETHERNET		m_ethData;
	
	static	int		m_nCount;
	static	sem_t	m_TxWait[MAX_ETH_COUNT];
	static  int		m_nUsed[MAX_ETH_COUNT];	
	int				m_nIndex;
	
	struct sockaddr_in 		m_srcAddr;
	struct sockaddr_in 		m_dstAddr;
	
	WORD    m_wOpStatus;			// 狀態
	WORD	m_wStatus;				// 錯誤信息
	WORD    m_wRTimeOut;            // Read  time out count in 1/1000 second
    WORD    m_wWTimeOut;            // Write time out count in 1/1000 second
    
    static 	void	Ethernet_RxRun(CEthernet* peth);
    static	void*	Ethernet_TxRun(CEthernet* peth);
    
    static int	WriteQueue(BYTE *pch, int iLength, _NETQUEUE *pQueue);
    static int	ReadQueue(BYTE **pch, int iLength, _NETQUEUE *pQueue);
    static int	QueueState(_NETQUEUE *pQueue);

    void	_XOn()				{};
    void	_TxDisable()		{};
	
public:
	
	int		Create(void* pDevice);
	WORD    Open();
    WORD    Close();
    WORD    Read (BYTE* pch, int nCount,       int* pnReadCount);
    WORD    Read (BYTE* pch, BYTE cTerminator, int* pnReadCount);
    WORD    Write(BYTE  ch);
    WORD    Write(BYTE* pch, int nCount,       int* pnWriteCount);
   	WORD    ReadToken (BYTE* pch, int nBufferSize, int* pnReadCount);
    WORD    WriteToken(BYTE* pch, int nCount,      int* pnWriteCount);
    
    WORD    GetRxState(WORD* pwState);
    WORD    GetTxState(WORD* pwState);

    WORD    AbortRx();
    WORD    AbortTx();

    WORD    GetOpStatus();

    void    SetRTimeOut(WORD nRTimeOut)		{m_wRTimeOut = nRTimeOut;};
    void    SetWTimeOut(WORD nWTimeOut)		{m_wWTimeOut = nWTimeOut;};
};


int		GetRealPhysicalAddr(BYTE *pMAC, char* pszEth = NULL);
BOOL 	SetPhysicalAddr(BYTE *pMAC);
#endif
