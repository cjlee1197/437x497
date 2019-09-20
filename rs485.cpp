/*===========================================================================+
|  Class    : RS485                                                          |
|  Task     : RS485 service routine                                          |
|----------------------------------------------------------------------------|
|  Compile  : gcc 3.4.5  -                                                   |
|  Link     : gcc 3.4.5  -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 11/23/2010                                                     |
|  Revision : V1.0                                                           |
|                                                      						 |
+===========================================================================*/
/*
//Task:
Code B:  
	設置:SCF百分比 從0%-0.5% 
	返回:設置點、最小點、最大點
	Master->SCF

Code E:
	設置:注射重量-克
	返回:注射重量設置點

Code G:
	設置:特定熔體壓力 單位IMM控制器在傳輸到SCF系統前已經轉換為psi(磅)了
	返回:熔體壓力設置值
	
Code H:
	設置:設置故障復位，但active故障將不被復位
	返回:無
	
Code J:
	設置:螺桿轉速時間(seconds)
	返回:螺桿轉速時間

Code L:
	設置:學習模式(射出重量、熔體壓力)
	返回:

Code R:
	設置:安全繼電器復位
	
Code S:
	設置:請求系統狀態

Code T:
	設置:切換停止/運行(Run=1, Stop=0)
	
Code Acknowledgment 位置錯誤或超時錯誤。 0.5s沒有回應，則表示超時。
*/




#include	<string.h>
#include	<stdio.h>
#include	<termios.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<errno.h>
#include	"rs485.h"
#include	"main.h"
#include    "utils.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		MUCELL_COMM_SLAVE_ADDR				0xB1
#define		MUCELL_COMM_MASTER_ADDR				0x31

#define 	RS485_COM_READ_NONE					0		/* none */
#define		RS485_COM_READ_READY				1		/* ready */
#define		RS485_COM_READ_WAITHEADA			2		/* wait heada */
#define		RS485_COM_READ_WAITADDR				3		/* wait addr */
#define		RS485_COM_READ_WAITLEN				4		/* wait length */
#define		RS485_COM_READ_WAITDATA				5		/* wait data */
#define		RS485_COM_READ_WAITCRC				6		/* wait crc */
#define		RS485_COM_READ_WAITFOOT				7		/* wait foot */
#define 	RS485_COM_READ_ERROR				8		/* error */
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

IMPLEMENT_DYNCREATE(CRS485,CDevice)
/*===========================================================================+
|           Class implementation - RS232                                     |
+===========================================================================*/

//CRS485::CRS485() : CDevice()
//{
//		
//}
//
//CRS485::CRS485(tmRS485* prs485) : CDevice()
//{
//	memset(&m_rs485, 0, sizeof(m_rs485));
//	m_errcount				= 0;
//	m_readstate				= AMMETER_COM_READ_READY;
//	m_SlaveAddr				= MUCELL_COMM_SLAVE_ADDR;
//	
//	queueRx.pQueue = 
//	queueRx.pInput = 
//	queueRx.pOutput = g_ReadDataBuff;
//	queueRx.pQueueEnd = g_ReadDataBuff +  sizeof(g_ReadDataBuff);
//	
//	queueTx.pQueue = 
//	queueTx.pInput = 
//	queueTx.pOutput = g_WriteDataBuff;
//	queueTx.pQueueEnd = g_WriteDataBuff +  sizeof(g_WriteDataBuff);
//	
//	sem_init(&send_sem, 0, 0);
//	sem_init(&read_sem, 0, 0);
//	
//	Create(prs485);
//	
//	if(pthread_create(&m_rxthreadid, NULL, RecvFunc, this) != 0)
//	{
//		perror("RS485 create error\n");	
//	}
//
//	sem_post(&send_sem);
//}
//
//
//CRS485::~CRS485()
//{
//	sem_destroy(&send_sem);
//	sem_destroy(&read_sem);
//	pthread_cancel(m_rxthreadid);
//	Close();
//}
//
//int		CRS485::Create(void* pDevice)
//{
//	memcpy(&m_rs485, prs485, sizeof(m_rs485));
//	m_wRTimeOut = 500;
//	m_wWTimeOut = 500;
//	
//	m_wOpStatus = Open();
//	return	m_wOpStatus;
//}
//
//WORD	CRS485::Open()
//{
//	struct	termios		Termios;
//	int		iDeviceLen;
//	
//	// 數字轉成ascii,放入路徑 存成如 /dev/ttyS0
//	strcpy(m_devicename, COM_PATH);
//	iDeviceLen						= strlen(cComDevice);
//	m_devicename[iDeviceLen]		= m_rs485.wChannel + 0x30;	
//	m_devicename[iDeviceLen + 1]	= '\0';
//	
//	m_fdcom = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
//	
//	if(m_fdcom == -1)
//	{
//		printf("CRS485 open %s:%s\n", m_devicename, strerror(errno));
//		return -1;
//	}
//	ret = fcntl(m_fdcom, F_SETFL, 0/*FASYNC*/);
//	if ( ret != 0 ) {
//		printf("CRS485 fcntl %s\n", strerror(errno));
//		return ret;
//	}
//	ret = isatty(m_fdcom);
//	if( ret == 0 ) {
//		printf("CRS485 isatty %d %s\n", m_fdcom, strerror(errno));
//		return ret;
//	}
//	
//	ret = tcgetattr(m_fdcom, &m_oldtermios);
//	if ( ret != 0) {
//		printf("CRS485 tcgetattr %s\n", strerror(errno));
//		return ret;
//	}
//	
//	newtermios.c_cflag 		= CLOCAL | CREAD;
//
//	newtermios.c_cflag      &= ~CSIZE;
//	newtermios.c_cflag      |= CS8;
//	
//	newtermios.c_cflag      &= ~CSTOPB;
//
//	newtermios.c_iflag 		= INPCK; 
//	newtermios.c_cflag      |= PARENB;
//	newtermios.c_cflag      &= ~PARODD;
//
//	
//	newtermios.c_cc[VTIME] 	= 5;	//500ms
//	newtermios.c_cc[VMIN] 	= 0;
//
//	cfsetispeed(&newtermios, m_rs485.config.BaudRate);
//	cfsetospeed(&newtermios, m_rs485.config.BaudRate);
//	tcflush(m_fdcom, TCIOFLUSH);
//    ret = tcsetattr(m_fdcom, TCSANOW, &newtermios);
//	if ( ret != 0)
//	{
//		printf("RS485 open tcsetattr %s\n", strerror(errno));
//		return ret;
//	}
//	
//	return DEVICE_S_OPENED;
//}
//
//WORD	CRS485::Close()
//{
//	if ( m_fdcom == -1 )
//	{
//		return 0;
//	}
//	tcsetattr(m_fdcom, TCSANOW,&m_oldtermios);
//	
//	if ( close(m_fdcom) == -1 )
//	{
//		perror("CloseDevice error \n");
//		return -1;
//	}
//	m_fdcom = -1;
//	return 0;
//}
//
//void	CRS485::ReadData (BYTE *pData, int len )
//{
//	if( len < 1 )
//	{
//		printf("Cann't get data from device !\n");
//		m_errcount++;
//		if(m_errcount > 4)
//		{
//			Close();
//			Open();
//			m_errcount = 0;
//		}
//		TransOver();
//		return;
//	}
//	m_errcount = 0;
//	while(len > 0)
//	{
//		switch(m_readstate)
//		{
//			case RS485_COM_READ_WAITHEADA:
//				
//				break;
//			
//			case RS485_COM_READ_WAITADDR:
//				
//				break;
//			
//			case RS485_COM_READ_WAITLEN:
//				
//				break;
//			
//			case RS485_COM_READ_WAITDATA:
//				
//				break;
//			
//			case RS485_COM_READ_WAITCRC:
//				
//				break;
//		}
//		pData++;
//		len --;	
//	}
//}
//
//void	CRS485::TransOver ( )
//{
//	m_readstate = AMMETER_COM_READ_READY;
//	m_waitlen = 0;
//	m_readcs = 0;
//	sem_post(&send_sem);
//	sem_wait(&read_sem);
//	return ;
//}
//
////void 	*RecvFunc ( void *pClass)
////{
////	BYTE Buff;
////	int len = 0;
////	
////	pthread_detach(pthread_self());
////	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
////	{
////		perror("RecvFunc setcanceltype");
////	}
////
////	while( 1 )
////	{
////		len = read(((CRS485 *)pClass)->m_fdcom, &Buff, 1);
////		
////		if(queueRx.pInput == queueRx.pOutput && queueRx.bFull == TRUE)
////			continue;
////			
////		*queueRx.pInput++	= Buff;
////		if(queueRx.pInput == queueRx.pQueueEnd)
////			queueRx.pInput	= queueRx.pQueue;
////		
////		if(queueRx.pInput == queueRx.pOutput)
////			queueRx.bFull		= TRUE;
////		
////		((CRS485 *)pClass)->ReadData(&buff, len);
////	}
////
////	return NULL;
////}
//
//void 	CRS485::RS485_RxRun(CRS485*	pirs485)
//{
//	BYTE 		Buff;	
//	tmRS232*	prs232 		= pirs485->m_irs232.prs232;
//	_QUEUE*		pQueue		= &(pirs485->m_irs232.queueRx);
//	MSG			msg;
//	
//	
//	sem_post(&pirs232->ThreadWait);	
//	
//	//pirs232->ThreadWait = ThreadWait;	
//
//	//=======
//	// 設置線程退出後，立即釋放所有資源
//	// 設置線程收到cancel信號後，立刻退出
//	//=======
//	pthread_detach(pthread_self());	
//	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
//	{
//		perror("CRS232::RS232_RxRun setcanceltype");
//	}
//	
//	while (TRUE)
//	{	
//		if(read(pirs232->m_irs232.hCom, &Buff, 1) == -1)
//		{
//			perror("CRS232::RS232_RxRun read:");
//			continue;
//		}
//		
//		if(pirs232->m_irs232.prs232->pRSFunc != NULL)    //netdigger 2007/4/17
//		{
//		    prs232->pRSFunc(pirs232->m_irs232.prs232->pClass, (char *)&Buff, 1);
//		    continue;
//		}
//		
//		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == TRUE)
//			continue;
//			
//		*pQueue->pInput++	= Buff;
//		if(pQueue->pInput == pQueue->pQueueEnd)
//			pQueue->pInput	= pQueue->pQueue;
//		
//		if(pQueue->pInput == pQueue->pOutput)
//			pQueue->bFull		= TRUE;
//		msg.message	= MSG_RS232;
//		msg.wParam	= (WPARAM)prs232->wChannel;
//		msg.lParam	= Buff;
//		g_pApplication->QueueMessage(&msg);
//	}
//	
//	sem_post(&pirs232->ThreadWait);
//}
//
//void	CRS485::RS485_TxRun(CRS485*	pirs485)
//{
//	_QUEUE*		pQueue		= &(pirs485->m_irs232.queueTx);
//	
//	sem_post(&(pirs232->ThreadWait));
//	
//	//=======
//	// 設置線程退出後，立即釋放所有資源
//	// 設置線程收到cancel信號後，立刻退出
//	//=======
//	pthread_detach(pthread_self());
//	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
//	{
//		perror("CRS232::RS232_RxRun setcanceltype");
//	}
//
//	while (TRUE)
//	{	
//		// 緩沖區為空，則等待
//		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == FALSE)
//		{
//			sem_wait(&(pirs232->TxWait));
//			continue;
//		}
//		
//		if(write(pirs232->m_irs232.hCom, pQueue->pOutput, 1) == -1)
//			continue;		
//			
//		*pQueue->pOutput++;
//		
//		if(pQueue->pOutput == pQueue->pQueueEnd)
//			pQueue->pOutput	= pQueue->pQueue;
//				
//		pQueue->bFull	= FALSE;
//	}
//	
//	sem_post(&(pirs232->ThreadWait));
//}
typedef void * (* PFUN)(void *);
typedef void * (* PRSRUN)(void *);

#define     RSLEEP              100     // Sleep for 50/1024 second
#define     WSLEEP              100     // Sleep for 50/1024 second

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
WORD        g_w485TokenStatus[TOKEN_STATE_COUNT] = 
{
	TOKEN_485_NONE,		
	TOKEN_485_ADDR,	
	TOKEN_485_PKL,	
	TOKEN_485_FRAME,
	TOKEN_485_CRC,	
	TOKEN_485_DONE,	
	TOKEN_485_STATE_COUNT
};

/*===========================================================================+
|           Class implementation - RS232                                     |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CRS485::CRS485() : CDevice()
{
	memset(&m_irs232, 0, sizeof(m_irs232));
}

CRS485::CRS485(tmRS232* prs232) : CDevice()
{
	memset(&m_irs232, 0, sizeof(m_irs232));
	Create(prs232);
}

CRS485::~CRS485()
{
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
int		CRS485::Create(void* prs232)
{
	//memset(&m_irs232, 0, sizeof(m_irs232));
	m_rs232 	= *(tmRS232*)prs232;
	m_wRTimeOut = COM_DEF_RTIMEOUT;
	m_wWTimeOut = COM_DEF_WTIMEOUT;
	m_STX 		= COM_DEF_STX;
	m_ETX 		= COM_DEF_ETX;
	m_SlaveAddr = MUCELL_COMM_SLAVE_ADDR;
	m_MasterAddr= MUCELL_COMM_MASTER_ADDR;
	m_wOpStatus = Open();
	return	m_wOpStatus;
}

WORD	CRS485::SetSlaveAddr(BYTE	bAddr)
{
	m_SlaveAddr = 	bAddr;
	return TRUE;
}

WORD	CRS485::SetMasterAddr(BYTE	bAddr)
{
	m_MasterAddr = 	bAddr;
	return TRUE;	
}

WORD	CRS485::Open()
{
	m_TokenState = RS485_COM_READ_NONE;
	m_token.wState = TOKEN_485_NONE;
	return Op(COM_OPEN, &m_rs232);
}

WORD	CRS485::Open(tmRS232* prs232)
{
	m_rs232 = *prs232;
	return Open();
}

WORD	CRS485::Close()
{
	return Op(COM_CLOSE, &m_rs232);
}

WORD	CRS485::Read(BYTE* pch, int nCount, int* pnReadCount)
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

WORD	CRS485::Read(BYTE* pch, BYTE cTerminator, int* pnReadCount)
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

WORD	CRS485::Write(BYTE ch)
{
	tmRS232     rs232;
	int         nTimeOutCount;
	
	rs232.wChannel = m_rs232.wChannel;
	rs232.cChar = ch;
	nTimeOutCount = m_wWTimeOut / WSLEEP;
	while ((Op(COM_WRITE, &rs232) == DEVICE_E_TX_FULL) && (--nTimeOutCount > 0));
	
	return m_wOpStatus;
}

WORD        CRS485::Write(BYTE* pch, int nCount, int* pnWriteCount)
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

WORD        CRS485::ReadToken(BYTE* pch, int nBufferSize, int* pnReadCount)
{
	tmRS232   		rs232;
	tmRSMsgLen		RsMsgLen;
	static DWORD	dwPKL = 0;
	BYTE			bCRC[3];
	static BYTE		bCRCValue = 0;
	int         	i = 0; 
	static int		nOffset = 0 , nCount = 0;
	memset(&RsMsgLen, 0, sizeof(RsMsgLen));
	memset(bCRC, 0, sizeof(bCRC));
	rs232.wChannel = m_rs232.wChannel;
	if (m_token.wState == TOKEN_485_NONE || m_token.wState == TOKEN_485_DONE)                       // Set up initial parsing condition
	{
	    m_token.wState = TOKEN_485_ADDR;
	    m_token.wCurrentCRC = 0;
	}
	while (m_token.wState != TOKEN_485_NONE && m_token.wState != TOKEN_485_DONE)
    {
	    switch (m_token.wState)
	    {
	        case TOKEN_485_ADDR:
	        	
	           	if(Op(COM_GETRXSIZE, &rs232) != DEVICE_SUCCESS)		goto _READTOKEN_ERROR;  // Get Rx data size
	           
	            if(rs232.wSize < 2)									return g_w485TokenStatus[m_token.wState];
	            if(Op(COM_READ, &rs232)!= DEVICE_SUCCESS)			goto _READTOKEN_ERROR;
	            
	        	
	        	bCRCValue += rs232.cChar;
	        	m_token.wState = TOKEN_485_PKL;
	       		break;
	       case TOKEN_485_PKL:
	       		if (Op(COM_GETRXSIZE, &rs232) != DEVICE_SUCCESS)	goto _READTOKEN_ERROR;  // Get Rx data size
	            if (rs232.wSize < 3)		  						return g_w485TokenStatus[m_token.wState];
	            if(Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <PKL>.c.cH
	            	RsMsgLen.bLen[0] = rs232.cChar;
	            bCRCValue += rs232.cChar;
	            if(Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <PKL>.c.cH
	            	RsMsgLen.bLen[1] = rs232.cChar;
	            bCRCValue += rs232.cChar;
	            if(Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <PKL>.c.cH
	            	RsMsgLen.bLen[2] = rs232.cChar;
	            bCRCValue += rs232.cChar;
	            //need crc  change long
	            dwPKL = strtol((char*)RsMsgLen.bLen, NULL, 16);
	            if(dwPKL > nBufferSize)					goto _READTOKEN_ERROR;
	            
	            
	            nOffset += 4;
	            m_token.wState = TOKEN_485_FRAME;
	            *(DWORD*)pch	= dwPKL;
	            break;
	        case TOKEN_485_FRAME:
	            if (Op(COM_GETRXSIZE, &rs232) != DEVICE_SUCCESS)	goto _READTOKEN_ERROR;  // Get Rx data size
	            if (rs232.wSize < dwPKL-6)		  					return g_w485TokenStatus[m_token.wState];
	            for(int i = 0; i < dwPKL-6; i++)
	            {
	            	if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)		goto _READTOKEN_ERROR;  // cH
	                *(pch + nOffset) = rs232.cChar;
	            	nOffset++;
	                bCRCValue += rs232.cChar;
	            }
	            m_token.wState = TOKEN_485_CRC;
	            break;
	        case TOKEN_485_CRC:
	        	if (Op(COM_GETRXSIZE, &rs232) != DEVICE_SUCCESS)	goto _READTOKEN_ERROR;  // Get Rx data size
	            if (rs232.wSize < 2)		  						return g_w485TokenStatus[m_token.wState];
	            if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <CRC>.c.cH
	            bCRC[0]= rs232.cChar;
	            if (Op(COM_READ, &rs232) != DEVICE_SUCCESS)			goto _READTOKEN_ERROR;  // Read <CRC>.c.cL
	            bCRC[1]= rs232.cChar;
	            if(bCRCValue != strtol((char*)bCRC, NULL, 16))
	            {
	            	m_token.wState = TOKEN_485_DONE;
	            	bCRCValue = 0;
	            	nOffset = 0;
	            	nCount++;
	            	return TOKEN_485_DONE;
	            }
	            else 
	            {	
	            	m_token.wState = TOKEN_485_NONE;
	            	bCRCValue = 0;
	            	nOffset = 0;
	            	nCount++;
	            	return TOKEN_485_NONE;
	            }
	            break;
	        default :
	        	break;
	    }
    }
	
	*pnReadCount = m_token.wPKL;
	
_READTOKEN_ERROR:
	m_wOpStatus = g_w485TokenStatus[m_token.wState];
	m_token.wState = TOKEN_485_NONE;
	return m_wOpStatus;
}

WORD        CRS485::WriteToken(BYTE* pch, int nCount, int* pnWriteCount)
{
	int         i;
	BYTE		bCRC[2];
	BYTE        bCRCValue =0;
	BYTE		bValue;
	
	//memset(&RsMsgLen, 0, sizeof(RsMsgLen));
	*pnWriteCount = 0;
	//RsMsgLen.dwPKL = (DWORD)nCount+6;
	//sprintf((char*)RsMsgLen.bLen, "%03x", nCount+6);
	//printf("RsMsgLen.bLen=%s \n", RsMsgLen.bLen);
	//if (*pch == 0) 						return COM_E_TOKEN_NONE;
	bCRCValue  += m_SlaveAddr;
	if (Write(m_SlaveAddr)        != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send Slave addr
	bCRCValue  += *pch;
	if (Write(*pch++)     != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send PKL 1
	bCRCValue  += *pch;
	if (Write(*pch++)     != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send PKL 2
	bCRCValue  += *pch;
	if (Write(*pch++)     != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send PKL 3
	
	//printf("Write ");
	for(i = 0; i < nCount-3; i++)
	{
		bCRCValue += *pch;
		//printf("%x ", *pch);
		if (Write(*pch++)         != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send value
	}
	//printf("\n");
	memset(bCRC, 0, sizeof(bCRC));
	sprintf((char*)bCRC, "%02X", bCRCValue);
	//printf("bCRC=%s wCRC=%X \n", bCRC, bCRCValue); 
	if (Write(bCRC[0])       != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send crc
	if (Write(bCRC[1])       != DEVICE_SUCCESS) goto _WRITETOKEN_ERROR; // Send crc
	
	
_WRITETOKEN_ERROR:
	return m_wOpStatus;
	return DEVICE_SUCCESS;
	return DEVICE_SUCCESS;
}

void 	RS485_RxRun(CRS485*	pirs485)
{
	BYTE 		Buff;	
	tmRS232*	prs232 		= pirs485->m_irs232.prs232;
	_QUEUE*		pQueue		= &(pirs485->m_irs232.queueRx);
	MSG			msg;
	
	
	sem_post(&pirs485->ThreadWait);	

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
		if(read(pirs485->m_irs232.hCom, &Buff, 1) == -1)
		{
			//perror("CRS485::RS485_RxRun read:");
			continue;
		}
		
		if(pirs485->m_irs232.prs232->pRSFunc != NULL)
		{
		    prs232->pRSFunc(pirs485->m_irs232.prs232->pClass, (char *)&Buff, 1);
		    continue;
		}
		
		if(pQueue->pInput == pQueue->pOutput && pQueue->bFull == TRUE)
			continue;
		
		//printf("Buff=%x \n", Buff);
		*pQueue->pInput++	= Buff;
		if(pQueue->pInput == pQueue->pQueueEnd)
			pQueue->pInput	= pQueue->pQueue;
		
		if(pQueue->pInput == pQueue->pOutput)
			pQueue->bFull		= TRUE;
	}
	
	sem_post(&pirs485->ThreadWait);
}

void	RS485_TxRun(CRS485*	pirs485)
{
	_QUEUE*		pQueue		= &(pirs485->m_irs232.queueTx);
	
	sem_post(&(pirs485->ThreadWait));
	
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
			sem_wait(&(pirs485->TxWait));
			continue;
		}
		
		if(write(pirs485->m_irs232.hCom, pQueue->pOutput, 1) == -1)
		{
			//printf("485 write error \n");
			continue;
		}		
		//printf("pQueue->pOutput=%x \n", *pQueue->pOutput);
		*pQueue->pOutput++;
		
		if(pQueue->pOutput == pQueue->pQueueEnd)
			pQueue->pOutput	= pQueue->pQueue;
				
		pQueue->bFull	= FALSE;
	}
	
	sem_post(&(pirs485->ThreadWait));
}

WORD        CRS485::GetRxSize(WORD* pwSize)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETRXSIZE, &rs232);
	*pwSize = rs232.wSize;
	
	return m_wOpStatus;
}

WORD        CRS485::GetTxRoom(WORD* pwRoom)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETTXROOM, &rs232);
	*pwRoom = rs232.wRoom;
	
	return m_wOpStatus;
}

WORD        CRS485::GetRxState(WORD* pwState)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETRXSTATE, &rs232);
	*pwState = rs232.wState;
	
	return m_wOpStatus;
}

WORD        CRS485::GetTxState(WORD* pwState)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETTXSTATE, &rs232);
	*pwState = rs232.wState;
	
	return m_wOpStatus;
}

WORD        CRS485::AbortRx()
{
	return Op(COM_ABORTRX, &m_rs232);
}

WORD        CRS485::AbortTx()
{
	return Op(COM_ABORTTX, &m_rs232);
}

WORD        CRS485::GetErrorCount(WORD* pwErrorCount)
{
	tmRS232       rs232;
	
	rs232.wChannel = m_rs232.wChannel;
	Op(COM_GETERRORCOUNT, &rs232);
	*pwErrorCount = rs232.wErrorCount;
	
	return m_wOpStatus;
}

WORD        CRS485::ClrErrorCount()
{
	return Op(COM_CLRERRORCOUNT, &m_rs232);
}



/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
WORD	CRS485::Op(BYTE cFunction, tmRS232 * fprs232)
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

WORD	CRS485::_Open(tmRS232 * fprs232)
{
	struct	termios		Termios;
	_QUEUE*		pQueue;
	char	cComDevice[20];
	int		iDeviceLen;
	
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
	fcntl(m_irs232.hCom, F_SETFL, FASYNC);
	tcgetattr(m_irs232.hCom, &m_irs232.Termios);
	tcflush(m_irs232.hCom, TCIOFLUSH);
    tcsetattr(m_irs232.hCom, TCSANOW, &Termios);
	//=======
	// 運行數據接收線程
	//=======
	sem_init(&ThreadWait, 0, 0);
	sem_init(&TxWait, 0, 0);
	
	//printf("Printf rs485 open \n");
	//printf("m_rs232.wMode=%d \n", m_rs232.wMode);
	if(m_rs232.wMode == 0)
	{
		if(pthread_create(&m_irs232.RxThread, NULL, (PRSRUN)RS485_RxRun, this) != 0)
		{
			perror("CRS232::_Open");
		}
		
		sem_wait(&ThreadWait);
		
		pthread_create(&m_irs232.TxThread, NULL, (PRSRUN)RS485_TxRun, this);	
		sem_wait(&ThreadWait);
	}
	
	m_irs232.wState		= COM_S_OPENED;
	return DEVICE_S_OPENED;
}

WORD	CRS485::_Close(tmRS232 * fprs232)
{
	
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	// 關閉接收線程
	pthread_cancel(m_irs232.RxThread);
	pthread_cancel(m_irs232.TxThread);

	sem_destroy(&ThreadWait);
	sem_destroy(&TxWait);
	
	// 恢復原先的com口設置
	tcsetattr(m_irs232.hCom,TCSANOW,&m_irs232.Termios);
	// 關閉文件句柄
	if(close(m_irs232.hCom) == -1)
	{
//		//printf("RS232 Close com%d:%s\n", m_rs232.wChannel, strerror(errno));
		return DEVICE_S_CLOSED;
	};
	// 設置相關狀態
	m_irs232.hCom 		= 0;
	m_irs232.wState		= COM_S_CLOSED;
	return DEVICE_S_CLOSED;
}

WORD	CRS485::_Read(tmRS232* prs232)
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

WORD	CRS485::_Write(tmRS232* prs232)
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
	
	//printf("prs232->cChar=%x \n", prs232->cChar);
	*pQueue->pInput++	= prs232->cChar;
	
	if(pQueue->pInput == pQueue->pQueueEnd)
		pQueue->pInput	= pQueue->pQueue;
		
	if(pQueue->pInput == pQueue->pOutput)
		pQueue->bFull		= TRUE;
	
	if(m_rs232.wMode == 0)
	{
		sem_getvalue(&TxWait, &Value);
		if(Value == 0)
			sem_post(&TxWait);
	}
	
	return DEVICE_SUCCESS;
}


WORD	CRS485::_GetRxState(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	prs232->wState	= m_irs232.wRxState;
	
	return DEVICE_SUCCESS;
}

WORD	CRS485::_GetTxState(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	prs232->wState	= m_irs232.wTxState;

	return DEVICE_SUCCESS;
}

WORD	CRS485::_GetRxSize(tmRS232* prs232)
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

WORD	CRS485::_GetTxRoom(tmRS232* prs232)
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

WORD	CRS485::_AbortRx(tmRS232* prs232)
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

WORD	CRS485::_AbortTx(tmRS232* prs232)
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

WORD	CRS485::_GetErrorCount(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
	
	prs232->wErrorCount	= m_irs232.wErrorCount;	
	
	return DEVICE_SUCCESS;
}

WORD	CRS485::_ClrErrorCount(tmRS232* prs232)
{
	// 如果COM口已經關閉，則退出
	if(m_irs232.wState == COM_S_CLOSED || m_irs232.hCom == 0)
		return DEVICE_S_CLOSED;
		
	m_irs232.wErrorCount	= 0;
	
	return DEVICE_SUCCESS;
}

