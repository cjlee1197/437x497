/*===========================================================================+
|  Class    : RS232                                                          |
|  Task     : RS232 service routine                                          |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 08/12/1996                                                     |
|  Revision : V1.10                                                          |
|             03/02/1997 by C.C. Chen                                        |
|             - Send DataFrame in normal byte sequence                       |
|             03/03/1997 by C.C. Chen                                        |
|             - Change MAX_TOKEN_SIZE form 1024 to 2048                      |
+===========================================================================*/
#include    "utils.h"
#include    "rs232.h"
#include	"main.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
typedef void * (* PFUN)(void *);
typedef void * (* PRSRUN)(void *);

void 	RS232_RxRun(CRS232*	pirs232);
void	RS232_TxRun(CRS232*	pirs232);


#define     RSLEEP              100     // Sleep for 50/1024 second
#define     WSLEEP              100     // Sleep for 50/1024 second

#define		BAUDRATE_NUM		0x02	// the Number of Baund Rate			//
pthread_mutex_t					writeLock=PTHREAD_MUTEX_INITIALIZER;
//BYTE*		temp=NULL;				
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
WORD        g_wTokenStatus[TOKEN_STATE_COUNT] = 
{
	COM_E_TOKEN_NONE,           // TOKEN_NONE,
	COM_E_TOKEN_STX,            // TOKEN_STX,
	COM_E_TOKEN_PKL,            // TOKEN_PKL0,
	COM_E_TOKEN_PKL,            // TOKEN_PKL1,
	COM_E_TOKEN_DATA,           // TOKEN_DATAFRAME0,
	COM_E_TOKEN_DATA,           // TOKEN_DATAFRAME1,
	COM_E_TOKEN_CRC,            // TOKEN_CRC0,
	COM_E_TOKEN_CRC,            // TOKEN_CRC1,
	COM_E_TOKEN_ETX,            // TOKEN_ETX,
	DEVICE_SUCCESS				// TOKEN_DONE,
};

BYTE	g_wBaudRate[BAUDRATE_NUM]	=	// 目前產品中隻用到三種			//
{
	COM_BR_19200,
	COM_BR_38400,
};
/*===========================================================================+
|           Class implementation - RS232                                     |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CRS232,CDevice)

CRS232::CRS232() : CDevice()
{
	memset(&m_irs232, 0, sizeof(m_irs232));
}

CRS232::CRS232(tmRS232* prs232) : CDevice()
{
	memset(&m_irs232, 0, sizeof(m_irs232));
	Create(prs232);
}

CRS232::~CRS232()
{
	// 關閉接收線程
	pthread_cancel(m_irs232.RxThread);
	pthread_cancel(m_irs232.TxThread);
	sem_destroy(&ThreadWait);
	sem_destroy(&TxWait);
	pthread_mutex_destroy(&m_queue_lock);
	Close();
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CRS232::Create(void* prs232)
{
	//memset(&m_irs232, 0, sizeof(m_irs232));
	m_rs232 	= *(tmRS232*)prs232;
	m_wRTimeOut = COM_DEF_RTIMEOUT;
	m_wWTimeOut = COM_DEF_WTIMEOUT;
	m_STX 		= COM_DEF_STX;
	m_ETX 		= COM_DEF_ETX;
	pthread_mutex_init(&m_queue_lock,NULL);
	sem_init(&ThreadWait, 0, 0);
	sem_init(&TxWait, 0, 0);
	m_wOpStatus = Open();
	if(m_rs232.wMode == 0)
	{
		if(pthread_create(&m_irs232.RxThread, NULL, (PRSRUN)RS232_RxRun, this) != 0)
		{
			perror("CRS232::_Open");
		}
		
		sem_wait(&ThreadWait);
		
		pthread_create(&m_irs232.TxThread, NULL, (PRSRUN)RS232_TxRun, this);	
		sem_wait(&ThreadWait);
	}
	return	m_wOpStatus;
}

WORD	CRS232::Open()
{
	m_token.wState = TOKEN_NONE;
	return Op(COM_OPEN, &m_rs232);
}

WORD	CRS232::Open(tmRS232* prs232)
{
	m_rs232 = *prs232;
	return Open();
}

WORD	CRS232::Close()
{
	return Op(COM_CLOSE, &m_rs232);
}

WORD	CRS232::Read(BYTE* pch, int nCount, int* pnReadCount)
{
	tmRS232     rs232;
	int         i;
	int         nTimeOutCount;
	
	rs232.wChannel = m_rs232.wChannel;
	for (i=0; i<nCount; i++)
	{
	    nTimeOutCount = m_wRTimeOut / RSLEEP;
	    while ((Op(COM_READ, &rs232) == DEVICE_E_RX_EMPTY) && (--nTimeOutCount > 0));
	    if (m_wOpStatus != DEVICE_SUCCESS) break;
	    *pch++ = rs232.cChar;
	}
	
	*pnReadCount = i;
	return m_wOpStatus;
}

WORD	CRS232::Read(BYTE* pch, BYTE cTerminator, int* pnReadCount)
{
	tmRS232     rs232;
	int         i;
	int         nTimeOutCount;
	
	rs232.wChannel = m_rs232.wChannel;
	i = 0;
	do
	{
	    nTimeOutCount = m_wRTimeOut / RSLEEP;
	    while ((Op(COM_READ, &rs232) == DEVICE_E_RX_EMPTY) && (--nTimeOutCount > 0));
	    if (m_wOpStatus != DEVICE_SUCCESS) break;
	    *pch++ = rs232.cChar;
	    i++;
	}
	while (rs232.cChar != cTerminator);
	
	*pnReadCount = i;
	return m_wOpStatus;
}

WORD	CRS232::Write(BYTE ch)
{
	tmRS232     rs232;
	int         nTimeOutCount;
	
	rs232.wChannel = m_rs232.wChannel;
	rs232.cChar = ch;
	nTimeOutCount = m_wWTimeOut / WSLEEP;
	{
		while ((Op(COM_WRITE, &rs232) == DEVICE_E_TX_FULL) && (--nTimeOutCount > 0));
	}
//	if(temp!=NULL)
//	{
//		if((*temp == 0x30) && (*(temp+1) == 0x31))
//		{
//			//printf("this=%x,U1:TXNormal:WriteToken   pch=%x %x\n", this,*(temp+1),  *temp);
//			//printf("2:rs232.cChar=%x\n",rs232.cChar);
//		}
//	}
	return m_wOpStatus;
}

WORD        CRS232::Write(BYTE* pch, int nCount, int* pnWriteCount)
{
	tmRS232     rs232;
	int         i;
	int         nTimeOutCount;
	
	rs232.wChannel = m_rs232.wChannel;
	for (i=0; i<nCount; i++)
	{
	    rs232.cChar = *pch++;
	    nTimeOutCount = m_wWTimeOut / WSLEEP;
	    while ((Op(COM_WRITE, &rs232) == DEVICE_E_TX_FULL) && (--nTimeOutCount > 0));
	    if (m_wOpStatus != DEVICE_SUCCESS) break;
	}
	
	*pnWriteCount = i;
	return m_wOpStatus;
}

/*---------------------------------------------------------------------------+
|  Function : ReadToken(BYTE* psBuffer, int nBufferSize, int* pnReadCount)   |
|  Task     : Read a token though RS232 interface                            |
|             A complete token from the transmitter is:                      |
|               <STX><PKL><Data Frame><CRC><ETX>                             |
|               where                                                        |
|                 <STX> - 1 Byte,  Start of TeXt indicator                   |
|                 <PKL> - 2 Bytes, Length of <Data Frame> in word            |
|                 <CRC> - 2 Bytes, Exclusive OR from <PKL> to <Data Frame>   |
|                 <ETX> - 1 Byte,  End of TeXt indicator                     |
|             The token application got from the API is:                     |
|               <Data frame>                                                 |
+----------------------------------------------------------------------------+
|  Call     : nStatus = CRS232::ReadToken(psBuffer,                          |
|                                         nBuffersize,                       |
|                                         pnReadCount)                       |
|                                                                            |
|  Parameter: psBuffer                  - Pointer to caller buffer           |
|             nBufferSize               - Size of caller buffer              |
|             pnReadCount               - Pointer to number of byte read     |
|                                                                            |
|  Return   : nStatus                   - RS232 I/O status                   |
|             *pnReadCount              - Number of byte read                |
+---------------------------------------------------------------------------*/
WORD        CRS232::ReadToken(BYTE* pch, int nBufferSize, int* pnReadCount)
{
	tmRS232   	rs232;
	int         i;
	
	if(m_rs232.wMode == 1)
		RS232_RxNormal();
	rs232.wChannel = m_rs232.wChannel;
	if (m_token.wState == TOKEN_NONE)                       // Set up initial parsing condition
	{
	    m_token.wState = TOKEN_STX;
	    m_token.wCurrentCRC = 0;
	}
	while (m_token.wState != TOKEN_DONE)
    {
	    switch (m_token.wState)
	    {
	        case TOKEN_STX:
	            if(Op(COM_GETRXSIZE, &rs232) != DEVICE_SUCCESS)		goto _READTOKEN_ERROR;  // Get Rx data size
	            if(rs232.wSize < (1 + 2))						  return g_wTokenStatus[m_token.wState];
	            if(Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <STX>
	            if(rs232.cChar != m_STX)							goto _READTOKEN_ERROR;
	            if(Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <PKL>.c.cH
	            m_token.cwPKL.c.cH	= rs232.cChar;
	            if(Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <PKL>.c.cL
	            m_token.cwPKL.c.cL 	= rs232.cChar;
	            m_token.wCurrentCRC ^= m_token.wPKL;
	            m_token.wPKL <<= 1;                                                    // Change the unit of PKL from word to byte
	            if(m_token.wPKL > (WORD)nBufferSize)				goto _READTOKEN_ERROR;
	            m_token.wState = TOKEN_DATAFRAME0;
	            break;
	        case TOKEN_DATAFRAME0:
	            if (Op(COM_GETRXSIZE, &rs232) != DEVICE_SUCCESS)	goto _READTOKEN_ERROR;  // Get Rx data size
	            if (rs232.wSize < (m_token.wPKL + 2 + 1))		  return g_wTokenStatus[m_token.wState];
	            for (i=0; i<(int)m_token.wPKL; i+=2)                                    // Read <Data Frame>
	            {
	                if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)		goto _READTOKEN_ERROR;  // cH
	                *(pch + i + 1)      =
	                m_token.cwData.c.cH = rs232.cChar;
	                if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)		goto _READTOKEN_ERROR;  // cL
	                *(pch + i)          =
	                m_token.cwData.c.cL = rs232.cChar;
	                m_token.wCurrentCRC ^= m_token.cwData.w;
	             //if (/*i == 0&&*/m_token.cwData.w==COMM_GETCONFIG) printf("Recieve m_token.cwData.w=%04X\n", m_token.cwData.w);
	            }
	            if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <CRC>.c.cH
	            m_token.cwCRC.c.cH = rs232.cChar;
	            if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <CRC>.c.cL
	            m_token.cwCRC.c.cL = rs232.cChar;
	            m_token.wCRC = m_token.cwCRC.w;
	            if (m_token.wCRC != m_token.wCurrentCRC)			goto _READTOKEN_ERROR;
	            if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <ETX>
	            if (rs232.cChar != m_ETX)							goto _READTOKEN_ERROR;
	            m_token.wState = TOKEN_DONE;
	            break;
	    }
    }

	*pnReadCount = m_token.wPKL;
	
_READTOKEN_ERROR:
	m_wOpStatus = g_wTokenStatus[m_token.wState];
	m_token.wState = TOKEN_NONE;
	return m_wOpStatus;
}

/*---------------------------------------------------------------------------+
|  Function : WriteToken(BYTE* psBuffer, int nCount, int* pnWriteCount)      |
|  Task     : Write a token though RS232 interface                           |
|             The token application provided is:                             |
|               <Data frame>                                                 |
|             The token API encapsulated to driver is:                       |
|               <STX><PKL><Data Frame><CRC><ETX>                             |
|               where                                                        |
|                 <STX> - 1 Byte,  Start of TeXt indicator                   |
|                 <PKL> - 2 Bytes, Length of <Data Frame> in word            |
|                 <CRC> - 2 Bytes, Exclusive OR from <PKL> to <Data Frame>   |
|                 <ETX> - 1 Byte,  End of TeXt indicator                     |
+----------------------------------------------------------------------------+
|  Call     : nStatus = CRS232::WriteToken(psBuffer,                         |
|                                          nCount,                           |
|                                          pnReadCount)                      |
|                                                                            |
|  Parameter: psBuffer                  - Pointer to caller buffer           |
|             nCount                    - Number of byte to be written       |
|             pnWriteCount              - Pointer to number of byte written  |
|                                                                            |
|  Return   : nStatus                   - RS232 I/O status                   |
|             *pnWriteCount             - Number of byte written             |
+---------------------------------------------------------------------------*/
WORD        CRS232::WriteToken(BYTE* pch, int nCount, int* pnWriteCount)
{
	pthread_mutex_lock(&writeLock);
	int         i;
	WORD        wPKL = nCount >> 1;
	WORD        wCRC = 0 ^ wPKL;
	CW          cw;

	*pnWriteCount = 0;
	if (wPKL == 0) 
	{
		//printf("U3:TXNormal:WriteToken   pch=%x %x\n", *(pch+1),  *pch);
		pthread_mutex_unlock(&writeLock);
		return COM_E_TOKEN_NONE;
	}
	if (Write(m_STX)        != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send <STX>
	if (Write(HIBYTE(wPKL)) != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send <PKL>.c.cH
	if (Write(LOBYTE(wPKL)) != DEVICE_SUCCESS)
	{
		goto _WRITETOKEN_ERROR; // Send <PKL>.c.cL
	}
	nCount = nCount >> 1;                                           // Truncate to word
	for (i=0; i<nCount; i++)                                        // Send <Data frame>
	{
	    cw.c.cL = *pch++;
	    cw.c.cH = *pch++;
		//if (i == 0) printf("Send command=%04X\n", cw.w);
	    wCRC ^= cw.w;
	    if (Write(cw.c.cH) != DEVICE_SUCCESS) break;                   // cH
	    if (Write(cw.c.cL) != DEVICE_SUCCESS) break;                   // cL
	}
	*pnWriteCount = i << 1;
	if (m_wOpStatus         != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR;
	
	if (Write(HIBYTE(wCRC)) != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send <CRC>.c.cH
	if (Write(LOBYTE(wCRC)) != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send <CRC>.c.cL
	if (Write(m_ETX)        != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send <ETX>
	if(m_rs232.wMode == 1)
	{
		RS232_TxNormal();
	}
_WRITETOKEN_ERROR:
	pthread_mutex_unlock(&writeLock);
	
	return m_wOpStatus;
	return DEVICE_SUCCESS;
	return DEVICE_SUCCESS;
}

WORD        CRS232::GetRxSize(WORD* pwSize)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETRXSIZE, &rs232);
	*pwSize = rs232.wSize;
	
	return m_wOpStatus;
}

WORD        CRS232::GetTxRoom(WORD* pwRoom)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETTXROOM, &rs232);
	*pwRoom = rs232.wRoom;
	
	return m_wOpStatus;
}

WORD        CRS232::GetRxState(WORD* pwState)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETRXSTATE, &rs232);
	*pwState = rs232.wState;
	
	return m_wOpStatus;
}

WORD        CRS232::GetTxState(WORD* pwState)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETTXSTATE, &rs232);
	*pwState = rs232.wState;
	
	return m_wOpStatus;
}

WORD        CRS232::AbortRx()
{
	return Op(COM_ABORTRX, &m_rs232);
}

WORD        CRS232::AbortTx()
{
	return Op(COM_ABORTTX, &m_rs232);
}

WORD        CRS232::GetErrorCount(WORD* pwErrorCount)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETERRORCOUNT, &rs232);
	*pwErrorCount = rs232.wErrorCount;
	
	return m_wOpStatus;
}

WORD        CRS232::ClrErrorCount()
{
	return Op(COM_CLRERRORCOUNT, &m_rs232);
}

WORD	CRS232::Change_BaudRate()		//St1101'01 for baudrate change	//
{
	//#ifndef	D_9600
	//static	WORD	wBaundRateID	= 0x00;	
	//#endif
	//
	//#ifdef  D_9600
	//m_rs232.config.BaudRate == COM_BR_9600;
	//#else	
	//m_rs232.config.BaudRate	= g_wBaudRate[wBaundRateID];
	//wBaundRateID	++;
	//if(wBaundRateID >= BAUDRATE_NUM)	wBaundRateID	= 0;
	//#endif
	//return m_rs232.config.BaudRate;
	
	#ifdef  D_9600
		m_rs232.config.BaudRate == COM_BR_9600;
	#else
		//if (m_rs232.config.BaudRate == COM_BR_19200)
   		m_rs232.config.BaudRate = COM_BR_38400;
	//else
   		//m_rs232.config.BaudRate = COM_BR_19200;
	#endif
	return m_rs232.config.BaudRate;
}

void	CRS232::RS232_RxNormal()
{
	BYTE 		Buff;
	int			nCount = 0;
	tmRS232*	prs232 		= m_irs232.prs232;
	_QUEUE*		pQueue		= &(m_irs232.queueRx);
	MSG			msg;
	
	while (TRUE)
	{	
		nCount = read(m_irs232.hCom, &Buff, 1);
		if(nCount == -1)
		{
			//perror("CRS232::RS232_RxRun read:");
			break;
		}
		else if(nCount == 0)
		{
			break;	
		}
		
		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == TRUE)
			break;
			
		*pQueue->pInput++	= Buff;
		if(pQueue->pInput == pQueue->pQueueEnd)
			pQueue->pInput	= pQueue->pQueue;
		
		if(pQueue->pInput == pQueue->pOutput)
			pQueue->bFull		= TRUE;
		msg.message	= MSG_RS232;
		msg.wParam	= (WPARAM)prs232->wChannel;
		msg.lParam	= Buff;
		g_pApplication->QueueMessage(&msg);
	}
}

void	CRS232::RS232_TxNormal()
{
	_QUEUE*		pQueue		= &(m_irs232.queueTx);

	while (TRUE)
	{	
		// 緩沖區為空，則等待
		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == FALSE)
		{
			break;
		}

		if(write(m_irs232.hCom, pQueue->pOutput, 1) == -1)
			break;		
			
		*pQueue->pOutput++;
		
		if(pQueue->pOutput == pQueue->pQueueEnd)
			pQueue->pOutput	= pQueue->pQueue;
				
		pQueue->bFull	= FALSE;
	}	
}

void 	RS232_RxRun(CRS232*	pirs232)
{
	BYTE 		Buff;	
	tmRS232*	prs232 		= pirs232->m_irs232.prs232;
	_QUEUE*		pQueue		= &(pirs232->m_irs232.queueRx);
	MSG			msg;
	
	
	sem_post(&pirs232->ThreadWait);	
	
	//pirs232->ThreadWait = ThreadWait;	

	//=======
	// 設置線程退出後，立即釋放所有資源
	// 設置線程收到cancel信號後，立刻退出
	//=======
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CRS232::RS232_RxRun setcanceltype");
	}
	
	while (TRUE)
	{	
		if(pirs232->m_irs232.hCom==-1)
		{ 
			usleep(100*1000);
			continue;
		}
		if(read(pirs232->m_irs232.hCom, &Buff, 1) == -1)
		{
			perror("CRS232::RS232_RxRun read:");
			continue;
		}
		
		if(pirs232->m_irs232.prs232->pRSFunc != NULL)    //netdigger 2007/4/17
		{
		    prs232->pRSFunc(pirs232->m_irs232.prs232->pClass, (char *)&Buff, 1);
		    continue;
		}
		
		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == TRUE)
			continue;
			
		*pQueue->pInput++	= Buff;
		if(pQueue->pInput == pQueue->pQueueEnd)
			pQueue->pInput	= pQueue->pQueue;
		
		if(pQueue->pInput == pQueue->pOutput)
			pQueue->bFull		= TRUE;
		msg.message	= MSG_RS232;
		msg.wParam	= (WPARAM)prs232->wChannel;
		msg.lParam	= Buff;
		g_pApplication->QueueMessage(&msg);
	}
	
	sem_post(&pirs232->ThreadWait);
}

void	RS232_TxRun(CRS232*	pirs232)
{
//	tmRS232*	prs232 		= pirs232->prs232;
	_QUEUE*		pQueue		= &(pirs232->m_irs232.queueTx);
	
	sem_post(&(pirs232->ThreadWait));
	
	//=======
	// 設置線程退出後，立即釋放所有資源
	// 設置線程收到cancel信號後，立刻退出
	//=======
	pthread_detach(pthread_self());
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CRS232::RS232_RxRun setcanceltype");
	}

	while (TRUE)
	{	
		// 緩沖區為空，則等待
		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == FALSE)
		{
			sem_wait(&(pirs232->TxWait));
			continue;
		}
		if(pirs232->m_irs232.hCom==-1) 
		{
			usleep(100*1000);
			continue;
		}
		int ncount=write(pirs232->m_irs232.hCom, pQueue->pOutput, 1);

		if(ncount == -1)
			continue;		
		*pQueue->pOutput++;
		//printf("this=%x,%p,lock before....\n",pirs232,&pirs232->m_queue_lock);
		pthread_mutex_lock(&pirs232->m_queue_lock);
		//printf("this=%x,%p,lock after....\n",pirs232,&pirs232->m_queue_lock);
		pirs232->m_nCounter--;
		if(pirs232->m_nCounter<0)	pirs232->m_nCounter=0;
		//printf("this=%x,%p,unlock befor 0....\n",pirs232,&pirs232->m_queue_lock);	
		pthread_mutex_unlock(&pirs232->m_queue_lock);
		//printf("this=%x,%p,unlock after 0....\n",pirs232,&pirs232->m_queue_lock);	
		if(pQueue->pOutput == pQueue->pQueueEnd)
		{
			pQueue->pOutput	= pQueue->pQueue;	
			pQueue->bFull	= FALSE;
		}
		//printf("this=%x,%p,unlock after ....\n",pirs232,&pirs232->m_queue_lock);
	}
	
	sem_post(&(pirs232->ThreadWait));
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
WORD	CRS232::Op(BYTE cFunction, tmRS232 * fprs232)
{
	WORD        wStatus;
	
	switch(cFunction)
	{
		case COM_OPEN:
			wStatus	= _Open(fprs232);
			break;
		case COM_CLOSE:
			wStatus	= _Close(fprs232);
			break;
		case COM_READ:
			//if( m_rs232.config.BaudRate == COM_BR_19200 )
			//	usleep(1);
			wStatus	= _Read(fprs232);
			break;
		case COM_WRITE:
			wStatus	= _Write(fprs232);
			break;
		case COM_GETRXSTATE:
			wStatus = _GetRxState(fprs232);
			break;
		case COM_GETTXSTATE:
			wStatus	= _GetTxState(fprs232);
			break;
		case COM_GETRXSIZE:
			wStatus	= _GetRxSize(fprs232);
			break;
		case COM_GETTXROOM:
			wStatus = _GetTxRoom(fprs232);
			break;
		default:
			wStatus	= COM_ERR_FUNCTION;
			break;
	}
	
	m_wOpStatus = wStatus;
	return wStatus;
}

void	signal_handler_IO(int status);

void	signal_handler_IO(int status)
{
	//printf("received SIGIO signal. \n");
	//wait_flag	= false;	
}

WORD	CRS232::_Open(tmRS232 * fprs232)
{
	struct	termios		Termios;
	_QUEUE*		pQueue;
	char	cComDevice[20];
	int		iDeviceLen;
	
	struct	sigaction		saio;			//James add 2013/4/3 16:31:31
	// 模具是否已經打開
	if(m_irs232.wState == COM_S_OPENED)
		return COM_S_OPENED;
	
	// 不能超過最大設備數	
	if(m_rs232.wChannel >= MAX_COM)	
		return DEVICE_S_CLOSED;

	
	// 數字轉成ascii,放入路徑 存成如 /dev/ttyS0
	strcpy(cComDevice, COM_PATH);
	iDeviceLen					= strlen(cComDevice);
	cComDevice[iDeviceLen]		= m_rs232.wChannel + 0x30;	
	cComDevice[iDeviceLen + 1]	= '\0';
	
	//Tx
	pQueue		= &m_irs232.queueTx;
	pQueue->bFull	= FALSE;
	
	pQueue->pQueue	=
	pQueue->pInput	=
	pQueue->pOutput	= m_rs232.fpTxBuffer;
	
	pQueue->pQueueEnd	= m_rs232.fpTxBuffer  + m_rs232.wTxBufferSize; 
	
	pQueue = &m_irs232.queueRx;
	
	//Rx
	pQueue->bFull	= FALSE;
	m_nCounter			= 0;
	pQueue->pQueue	=
	pQueue->pInput	=
	pQueue->pOutput	= m_rs232.fpRxBuffer;
	
	pQueue->pQueueEnd	= m_rs232.fpRxBuffer  + m_rs232.wRxBufferSize; 
	
	// 打開COM口
	m_irs232.prs232		= &m_rs232;
//	strcpy(cComDevice, "/dev/ttyS1");
	m_irs232.hCom		= open(cComDevice, O_RDWR | O_NOCTTY);
	if(m_irs232.hCom == -1)
	{ 
		return DEVICE_S_CLOSED;
	}
	
	//James add 2013/4/3 16:31:45
	saio.sa_handler =SIG_IGN; //signal_handler_IO;
	//saio.sa_mask = 0;
	saio.sa_flags	= 	0;
	saio.sa_restorer=	NULL;
	sigaction(SIGIO, &saio, NULL);
	// 設備com的傳輸狀態
	Termios.c_cflag 		= CS8 | CLOCAL | CREAD;
	Termios.c_iflag 		= IGNPAR;
	Termios.c_oflag 		= 0;
	Termios.c_lflag 		= 0;
	Termios.c_cc[VTIME] 	= 0;
	if(m_rs232.wMode == 0)							// James add 2010/9/8 上午 08:37:54
		Termios.c_cc[VMIN] 		= 1;		// 至少讀到一個字節再返回
	else
		Termios.c_cc[VMIN] 		= 0;		// 輪詢讀取	
	cfsetispeed(&Termios,	fprs232->config.BaudRate); 	// set in speed 
	cfsetospeed(&Termios,	fprs232->config.BaudRate); 	// set out speed
	fcntl(m_irs232.hCom, F_SETOWN, getpid());
	fcntl(m_irs232.hCom, F_SETFL, FASYNC);
	tcgetattr(m_irs232.hCom, &m_irs232.Termios);
	tcflush(m_irs232.hCom, TCIOFLUSH);
    tcsetattr(m_irs232.hCom, TCSANOW, &Termios);
	//=======
	// 運行數據接收線程
	//=======
	//sem_init(&ThreadWait, 0, 0);
	//sem_init(&TxWait, 0, 0);
	
	
	//printf("m_rs232.wMode=%d \n", m_rs232.wMode);
	//if(m_rs232.wMode == 0)
	//{
	//	if(pthread_create(&m_irs232.RxThread, NULL, (PRSRUN)RS232_RxRun, this) != 0)
	//	{
	//		perror("CRS232::_Open");
	//	}
	//	
	//	sem_wait(&ThreadWait);
	//	
	//	pthread_create(&m_irs232.TxThread, NULL, (PRSRUN)RS232_TxRun, this);	
	//	sem_wait(&ThreadWait);
	//}
	
	m_irs232.wState		= COM_S_OPENED;
	m_irs232.wRxState	= COM_S_IDLE;
	m_irs232.wTxState	= COM_S_IDLE;
	
	return DEVICE_S_OPENED;
}

WORD	CRS232::_Close(tmRS232 * fprs232)
{
	
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == -1)
		return DEVICE_S_CLOSED;
	
	// 恢復原先的com口設置
	tcsetattr(m_irs232.hCom,TCSANOW,&m_irs232.Termios);
	// 關閉文件句柄
	if(close(m_irs232.hCom) == -1)
	{
//		//printf("RS232 Close com%d:%s\n", m_rs232.wChannel, strerror(errno));
		m_irs232.hCom=-1;
		return DEVICE_S_CLOSED;
	};
	// 設置相關狀態
	m_irs232.hCom 		= -1;
	m_irs232.wState		= COM_S_CLOSED;
	m_irs232.wRxState	= COM_S_IDLE;
	m_irs232.wTxState	= COM_S_IDLE;	
	
	return DEVICE_S_CLOSED;
}

WORD	CRS232::_Read(tmRS232* prs232)
{
	_QUEUE*		pQueue		= &m_irs232.queueRx;
	
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
		
	if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == FALSE)
		return DEVICE_E_RX_EMPTY;
		
	prs232->cChar	= *pQueue->pOutput++;
	
	if(pQueue->pOutput == pQueue->pQueueEnd)
		pQueue->pOutput	= pQueue->pQueue;
	pQueue->bFull	= FALSE;
	
	return DEVICE_SUCCESS;
}

WORD	CRS232::_Write(tmRS232* prs232)
{
	int			Value;
	_QUEUE*		pQueue		= &m_irs232.queueTx;
	
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	// 緩沖區已經滿，退出
	if(pQueue->bFull == TRUE)
	{
		//printf("rs232_FULL =%d\n", pQueue->bFull);
		return DEVICE_E_TX_FULL;	
	}
	
	*pQueue->pInput++	= prs232->cChar;
	
	if(pQueue->pInput == pQueue->pQueueEnd)
		pQueue->pInput	= pQueue->pQueue;
	//printf("pQueue->pInput=%x,%x\n",pQueue->pInput,pQueue->pOutput);	
	//if(pQueue->pInput == pQueue->pOutput)
	pthread_mutex_lock(&m_queue_lock);
	m_nCounter++;
	if(m_nCounter==m_rs232.wTxBufferSize)
	{
		printf("m_nCounter=%d\n",m_nCounter);
		pQueue->bFull		= TRUE;
	
	}
	pthread_mutex_unlock(&m_queue_lock);
	if(m_rs232.wMode == 0)
	{
			
		sem_getvalue(&TxWait, &Value);
		if(Value == 0)
		{
			sem_post(&TxWait);
		}
	}
	
	return DEVICE_SUCCESS;
}

WORD	CRS232::_GetRxState(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	prs232->wState	= m_irs232.wRxState;
	
	return DEVICE_SUCCESS;
}

WORD	CRS232::_GetTxState(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	prs232->wState	= m_irs232.wTxState;

	return DEVICE_SUCCESS;
}

WORD	CRS232::_GetRxSize(tmRS232* prs232)
{
	int			RxSize		= 0;
	_QUEUE*		pQueue		= &m_irs232.queueRx;
	
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
		
	if(pQueue->pInput > pQueue->pOutput)
		RxSize	= pQueue->pInput - pQueue->pOutput;
	else if(pQueue->pInput < pQueue->pOutput)
		RxSize	= pQueue->pInput - pQueue->pOutput
				  + m_irs232.prs232->wRxBufferSize;
	else if(pQueue->bFull == TRUE)
		RxSize	= m_irs232.prs232->wRxBufferSize;
	
	prs232->wSize	= RxSize;
	
	return DEVICE_SUCCESS;	
}

WORD	CRS232::_GetTxRoom(tmRS232* prs232)
{
	int			TxSize		= 0;
	_QUEUE*		pQueue		= &m_irs232.queueTx;
	
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
		
	if(pQueue->pOutput > pQueue->pInput)
		TxSize	= pQueue->pOutput - pQueue->pInput;
	else if(pQueue->pInput > pQueue->pOutput)
		TxSize	= pQueue->pOutput - pQueue->pInput
				  + m_irs232.prs232->wTxBufferSize;
	else if(pQueue->bFull == FALSE)
		TxSize	= m_irs232.prs232->wTxBufferSize;
	
	prs232->wSize	= TxSize;
	
	return DEVICE_SUCCESS;
}
WORD	CRS232::_AbortRx(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	_XOn();
	
	m_irs232.wRxState			= COM_S_IDLE;
	m_irs232.queueRx.pInput		=
	m_irs232.queueRx.pOutput	= m_irs232.queueRx.pQueue;
	m_irs232.queueRx.bFull		= FALSE;
	
	return DEVICE_SUCCESS;
}
WORD	CRS232::_AbortTx(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	_TxDisable();
	
	m_irs232.wTxState			= COM_S_IDLE;
	m_irs232.queueTx.pInput		=
	m_irs232.queueTx.pOutput	= m_irs232.queueTx.pQueue;
	m_irs232.queueTx.bFull		= FALSE;
	
	return DEVICE_SUCCESS;
}
WORD	CRS232::_GetErrorCount(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	prs232->wErrorCount	= m_irs232.wErrorCount;	
	
	return DEVICE_SUCCESS;
}
WORD	CRS232::_ClrErrorCount(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
		
	m_irs232.wErrorCount	= 0;
	
	return DEVICE_SUCCESS;
}
