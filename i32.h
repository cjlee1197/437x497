/*==========================================================================+
|  Class    : COMM                                                          |
|  Task     : COMM service routine header file                              |
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2005/11/24                                                    |
|  Revision :                                                               |
+==========================================================================*/

#ifndef	D__DSWAP
#define	D__DSWAP

#include	"device.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
/*
#define	COMM_SUCCESS		0
#define	COMM_EMPTY			1
#define	COMM_FULL			2
#define	COMM_ERROR			0xFFFF
*/
//#define	SWAP_SIZE			1024
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void * (* I32FUNC)(char *, int);
typedef struct tagI32
{
	int			nRxSize;
	int			nTxSize;
	int			nRxCount;		// 讀緩沖區數目
	int			nTxCount;		// 寫緩沖區數目
	I32FUNC		I32Func;	
} I32;

typedef	struct tag_ICOMM
{
	char			cWrited;
	char			cReaded;
	
	int				nRx;
	int				nTx;
	char			*pcRx;		
	
	I32				*pI32;
}	_ICOMM;

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Macro definition                                                |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - Comm                                        |
+==========================================================================*/
class CI32 : public CDevice
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CI32)
public:
	CI32();
	~CI32();
/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
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
    WORD	StartComm();

    WORD    GetOpStatus()                       {return m_wOpStatus;};

    void    SetRTimeOut(WORD wRTimeOut)         {m_wRTimeOut = wRTimeOut;};
    void    SetWTimeOut(WORD wWTimeOut)         {m_wWTimeOut = wWTimeOut;};
    
    static 	void	COMM_RxRun(_ICOMM *picomm);
    
private:
    WORD    m_wOpStatus;
    WORD    m_wRTimeOut;                // Read  time out count in 1/1000 second
    WORD    m_wWTimeOut;                // Write time out count in 1/1000 second
    
    int		m_nRxSize;
    int		m_nTxSize;
    
    char			*m_pcBuff;
	char			*m_pcTx;
	char			*m_pcRx;
	
	I32			m_I32;
	_ICOMM		m_icomm;
	
	pthread_t	m_RxThread;				// 接收線程ID

/*--------------------------------------------------------------------------+
|           Helpers                                                         |
+--------------------------------------------------------------------------*/

/*==========================================================================+
|           External                                                        |
+==========================================================================*/
};
#endif
