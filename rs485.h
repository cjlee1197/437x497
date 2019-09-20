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
#ifndef     D__RS485
#define     D__RS485
#include	<pthread.h>
#include	<termios.h>
#include	<semaphore.h>

#include    "device.h"
#include	"oil54struct.h"

typedef void (*PRS485FUNC)(void *pClass, char *, int);

enum TOKEN485STATE 
{
	TOKEN_485_NONE,
	TOKEN_485_ADDR,
	TOKEN_485_PKL,
	TOKEN_485_FRAME,
	TOKEN_485_CRC,
	TOKEN_485_DONE,
	TOKEN_485_STATE_COUNT
};

typedef	struct	tagRSMsgLen
{
	union
	{
		DWORD	dwPKL;
		BYTE	bLen[3];
		BYTE	bLen1;
		BYTE	bLen2;
		BYTE	bLen3;	
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmRSMsgLen;
#else 
#ifdef	D_X86
tmRSMsgLen;
#endif
#endif

typedef	struct	tagRSMuCell
{
	BYTE	bAddress;
	BYTE	bMsgLen[3];				//All message len
	BYTE	bTaskCode;
	BYTE	bInfoMation[200];
	BYTE	bCheckSum[2];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmRSMuCell;
#else 
#ifdef	D_X86
tmRSMuCell;
#endif
#endif

typedef struct tagRS485
{
	WORD    wChannel;                   // Channel of rs485 - COM1/COM2/COM3/COM4
	_CONFIG config;
	WORD    wRxBufferSize;  			// Size of the receive buffer
	BYTE * 	fpRxBuffer;    				// Far pointer to the receive buffer
	WORD    wTxBufferSize;  			// Size of the transmit buffer
	BYTE * 	fpTxBuffer;    				// Far pointer to the transmit buffer
	PRS485FUNC	pRSFunc;        		//netdigger 2007/4/17
    void    *pClass;            		//netdigger 2007/4/17
    WORD	wMode;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmRS485;
#else 
#ifdef	D_X86
tmRS485;
#endif
#endif
/*
 * =====================================================================================
 *  Class:  CRSMuCell
 *  Description:  Haitian MuCell
 * =====================================================================================
 */
class CRS485: public CDevice
{
	friend		void	RS485_RxRun(CRS485*	pirs485);
    friend		void	RS485_TxRun(CRS485*	pirs485);
	//friend void * RecvFunc(void *pClass);
	DECLARE_DYNCREATE(CRS485)

public:
	CRS485();
    CRS485(tmRS232* prs232);
    ~CRS485();

private:
	//tmRS485 m_rs485;
	
    tmRS232 m_rs232;
    WORD    m_wOpStatus;
    WORD    m_wRTimeOut;                // Read  time out count in 1/1000 second
    WORD    m_wWTimeOut;                // Write time out count in 1/1000 second
    BYTE    m_STX;
    BYTE    m_ETX;
    BYTE	m_SlaveAddr;
    BYTE	m_MasterAddr;

    TOKEN   m_token;
    int		m_TokenState;

	sem_t	ThreadWait;				// 創建線程信號
	sem_t	TxWait;					// 發送線程信號
    _IRS232	m_irs232;				
//    WORD    m_wOpStatus;
//	WORD    m_wRTimeOut;                // Read  time out count in 1/1000 second
//    WORD    m_wWTimeOut;                // Write time out count in 1/1000 second
//
//	int 	m_errcount;
//	int 	m_fdcom;
//	char 	m_devicename[64];
//	struct termios m_oldtermios;
//	
//	BYTE	m_SlaveAddr;
//	BYTE	m_readstate;
//	int		m_ReadLen;
//	int		m_WriteLen;
//	BYTE	m_ReadTaskCode;
//	BYTE	m_WriteTaskCode;
//	WORD	m_wCheckSum;
//
//	_QUEUE  queueRx;
//    _QUEUE  queueTx;
//    
//	pthread_t m_rxthreadid;
//	
//	int		Create(void* pDevice);
//
//    WORD    Open();
//    WORD    Close();
//    WORD	WriteToken(BYTE* pch, int nCount, int* pnWriteCount);
//    WORD    ReadToken(BYTE* pch, int nBufferSize, int* pnReadCount);
//    void	ReadData (BYTE *pData, int len );
//
//private:
//	sem_t 	send_sem;
//	sem_t 	read_sem;
public:
    int		Create(void* pDevice);

    WORD    Open();
    WORD    Open(tmRS232* prs232);
    WORD    Close();
    WORD    Read(BYTE* pch, int  nCount,      int* pnReadCount);
    WORD    Read(BYTE* pch, BYTE cTerminator, int* pnReadCount);
    WORD    Write(BYTE  ch);
    WORD    Write(BYTE* pch, int  nCount,      int* pnWriteCount);
    WORD    ReadToken(BYTE* pch, int nBufferSize, int* pnReadCount);
    WORD    WriteToken(BYTE* pch, int nCount,      int* pnWriteCount);
	WORD	SetSlaveAddr(BYTE	bAddr);
	WORD	SetMasterAddr(BYTE	bAddr);
    WORD    GetOpStatus()                       {return m_wOpStatus;};

	void	RS232_RxNormal();
	void	RS232_TxNormal();
    void    SetRTimeOut(WORD wRTimeOut)         {m_wRTimeOut = wRTimeOut;};
    void    SetWTimeOut(WORD wWTimeOut)         {m_wWTimeOut = wWTimeOut;};
    
    WORD    GetRxSize(WORD* pwSize);
    WORD    GetTxRoom(WORD* pwRoom);

    WORD    GetRxState(WORD* pwState);
    WORD    GetTxState(WORD* pwState);

    WORD    AbortRx();
    WORD    AbortTx();

    WORD    GetErrorCount(WORD* pwErrorCount);
    WORD    ClrErrorCount();
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:	    
	WORD	Op(BYTE cFunction, tmRS232* prs232);
    WORD	_Open(tmRS232* prs232);
    WORD	_Close(tmRS232* prs232);
    WORD	_Read(tmRS232* prs232);
    WORD	_Write(tmRS232* prs232);
    WORD	_GetRxState(tmRS232* prs232);
    WORD	_GetTxState(tmRS232* prs232);
    WORD	_GetRxSize(tmRS232* prs232);
    WORD	_GetTxRoom(tmRS232* prs232);
    WORD	_AbortRx(tmRS232* prs232);
    WORD	_AbortTx(tmRS232* prs232);
    WORD	_GetErrorCount(tmRS232* prs232);
    WORD	_ClrErrorCount(tmRS232* prs232);
    void	_XOn()			{};
    void	_TxDisable()		{};
};

#endif

