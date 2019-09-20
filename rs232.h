/*===========================================================================+
|  Class    : RS232                                                          |
|  Task     : RS232 service routine header file                              |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 08/12/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     D__RS232
#define     D__RS232

#include    "device.h"
#include	"oil54struct.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - RS232                                        |
+===========================================================================*/
class CRS232 : public CDevice
{
	friend		void	RS232_RxRun(CRS232*	pirs232);
    friend		void	RS232_TxRun(CRS232*	pirs232);
DECLARE_DYNCREATE(CRS232)
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CRS232();
    CRS232(tmRS232* prs232);
    ~CRS232();

/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmRS232 m_rs232;
    WORD    m_wOpStatus;
    WORD    m_wRTimeOut;                // Read  time out count in 1/1000 second
    WORD    m_wWTimeOut;                // Write time out count in 1/1000 second
    BYTE    m_STX;
    BYTE    m_ETX;

    TOKEN   m_token;

	sem_t	ThreadWait;				// 創建線程信號
	sem_t	TxWait;					// 發送線程信號
    _IRS232	m_irs232;				
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	pthread_mutex_t		m_queue_lock;
	int					m_nCounter;				
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
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

    WORD    GetRxSize(WORD* pwSize);
    WORD    GetTxRoom(WORD* pwRoom);

    WORD    GetRxState(WORD* pwState);
    WORD    GetTxState(WORD* pwState);

    WORD    AbortRx();
    WORD    AbortTx();

    WORD    GetErrorCount(WORD* pwErrorCount);
    WORD    ClrErrorCount();

    WORD    GetOpStatus()                       {return m_wOpStatus;};
    
    WORD    Change_BaudRate();                            //St1101'01 for baudrate change

	void	RS232_RxNormal();
	void	RS232_TxNormal();
    void    SetRTimeOut(WORD wRTimeOut)         {m_wRTimeOut = wRTimeOut;};
    void    SetWTimeOut(WORD wWTimeOut)         {m_wWTimeOut = wWTimeOut;};
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
