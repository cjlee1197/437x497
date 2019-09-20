/*==========================================================================+
|  Class    : rtai                                                          |
|  Task     : rtai service routine file                              		|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James                                                         |
|  Version  : V1.00                                                         |
|  Creation : 2009/12/22                                                    |
|  Revision : V1.00                                                         |
+==========================================================================*/

#ifndef	D__DRTAI
#define	D__DRTAI

#include	"device.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void * (* RTAIFUNC)(char *, int);
typedef struct tagRTAI
{
	int			nRxSize;
	int			nTxSize;
	RTAIFUNC	RTAIFunc;	
}RTAI;

typedef	struct tag_RTAICOMM
{
	char			*pcRx;
	RTAI			*pRtai;
}_ICOMM;

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Macro definition                                                |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - Comm                                        |
+==========================================================================*/
class CRTAI : public CDevice
{
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
DECLARE_DYNCREATE(CRTAI)
public:
	CRTAI();
	~CRTAI();
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
    
    WORD    GetOpStatus()                       {return m_wOpStatus;};
	int		GetOptSize(int nType);				
    void    SetRTimeOut(WORD wRTimeOut)         {m_wRTimeOut = wRTimeOut;};
    void    SetWTimeOut(WORD wWTimeOut)         {m_wWTimeOut = wWTimeOut;};
    
    static 	void	COMM_RxRun(_ICOMM *picomm);
private:
	char* 	create_share_memory(char* name, int size);
	char* 	delete_share_memory(void* p_map);
 
private:
    WORD    	m_wOpStatus;
    WORD    	m_wRTimeOut;      // Read  time out count in 1/1000 second
    WORD    	m_wWTimeOut;      // Write time out count in 1/1000 second
    
    int			m_nRxSize;
    int			m_nTxSize;
    
		char		*m_pcTx;
		char		*m_pcRx;
	
	RTAI		m_Rtai;
	_ICOMM		m_icomm;
	
	pthread_t	m_RxThread;				// 接收線程ID

/*--------------------------------------------------------------------------+
|           Helpers                                                         |
+--------------------------------------------------------------------------*/

/*==========================================================================+
|           External                                                        |
+==========================================================================*/
};
extern	char*	pRtaiTxAddr;
#endif
