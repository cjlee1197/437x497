/*==========================================================================+
|  Class    : COMM                                                          |
|  Task     : COMM service routine file                              		 |
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

//#include <mbuff.h>
#include "i32.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define	DEVICE_BUFF		"mbuff"
#define	RX_DEVICE	"/dev/rtf1"
#define	TX_DEVICE	"/dev/rtf0"

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void * (* PRSRUN)(void *);

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/

/*==========================================================================+
|           Macro definition                                                |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - Comm                                        |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CI32,CDevice)

CI32::CI32() : CDevice()
{
	m_pcBuff	= NULL;
	m_pcTx		= NULL;
	m_pcRx		= NULL;

	m_icomm.cWrited	= 1;
	m_icomm.cReaded	= 0;

	m_I32.nRxSize	= 0;
	m_I32.nTxSize	= 0;
	m_I32.I32Func	= NULL;
	
	m_icomm.nRx		= 0;
	m_icomm.nTx		= 0;
	m_icomm.pI32	= &m_I32;
	m_nTxSize		= 0;
	m_nRxSize		= 0;

}

CI32::~CI32()
{
	Close();
}
/*--------------------------------------------------------------------------+
|           Attributes                                                      |
+--------------------------------------------------------------------------*/

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
int		CI32::Create(void* pDevice)
{
	if(pDevice != NULL)
		m_I32	= *(I32 *)pDevice;
	m_nTxSize		= m_I32.nTxSize * m_I32.nTxCount;
	m_nRxSize		= m_I32.nRxSize * m_I32.nRxCount;
	m_wOpStatus		= DEVICE_S_CLOSED;
	if(Open() == DEVICE_SUCCESS)
	{
		m_wOpStatus		= DEVICE_S_OPENED;
		return DEVICE_S_OPENED;
	}
	else
		return DEVICE_S_ERROR;
}

WORD    CI32::Open()
{
	//m_pcBuff	= (char *)mbuff_alloc(DEVICE_BUFF,m_nRxSize + m_nTxSize);
	if(m_pcBuff == NULL)
	{
		perror("CI32::Open mbuff_alloc failed");
		return	DEVICE_S_ERROR;
	}
	
	m_pcRx	= m_pcBuff;
	m_pcTx	= m_pcBuff + m_nRxSize;
//	//printf("m_pcRx=%08x, m_pcTx=%08x\n", m_pcRx, m_pcTx);
	m_icomm.pcRx	= m_pcRx;
	
	//printf("Open %s \n", RX_DEVICE);
	m_icomm.nRx	= open(RX_DEVICE, O_RDONLY);
	if(m_icomm.nRx <= 0)
	{
//		//printf("Open %s error:%s\n", RX_DEVICE, strerror(errno));
		return	DEVICE_S_ERROR;
	}	
	
//	//printf("Open %s \n", TX_DEVICE);
	m_icomm.nTx	= open(TX_DEVICE, O_WRONLY);
	if(m_icomm.nTx <= 0)
	{
//		//printf("Open %s error\n", TX_DEVICE);
		return	DEVICE_S_ERROR;
	}
	
	if(pthread_create(&m_RxThread, NULL, (PRSRUN)COMM_RxRun, &m_icomm) != 0)
	{
		perror("CRS232::_Open");
	}
	
	m_wOpStatus	= DEVICE_S_OPENED;
	
	return DEVICE_SUCCESS;
	
}

WORD    CI32::Close()
{
	if(m_wOpStatus	== DEVICE_S_CLOSED)	return DEVICE_SUCCESS;
	m_wOpStatus	= DEVICE_S_CLOSED;
	pthread_cancel(m_RxThread);
	if(m_icomm.nRx > 0)	close(m_icomm.nRx);
	if(m_icomm.nTx > 0)	close(m_icomm.nTx);
	//if(m_pcBuff != NULL)
	//	mbuff_free(DEVICE_BUFF,m_pcBuff);
	m_pcBuff	= NULL;
	return DEVICE_SUCCESS;

}

WORD    CI32::Read (BYTE* pch, int nCount, int* pnReadCount)
{
	if(pch == NULL)	return DEVICE_S_ERROR;
	if(nCount > m_nRxSize)	nCount	= m_nRxSize;
	if(m_icomm.cReaded == 0)	return DEVICE_E_RX_EMPTY;
	memcpy(pch, m_pcRx, nCount);
	m_icomm.cReaded	= 0;
	*pnReadCount	= nCount;
	
	//=======
	// 通知數據接收完畢
	//=======
	write(m_icomm.nTx, &m_icomm.cReaded,1);	
	
	return DEVICE_SUCCESS;
}
WORD    CI32::Read (BYTE* pch, BYTE cTerminator, int* pnReadCount)
{
	return	DEVICE_S_ERROR;
}
WORD    CI32::Write(BYTE  ch)
{
	return	DEVICE_S_ERROR;
}
WORD    CI32::Write(BYTE* pch, int nCount, int* pnWriteCount)
{
//	//printf("I32 Write nCount =%d, cWrite state =%d\n", nCount, m_icomm.cWrited);
	if(pch == NULL)	return DEVICE_S_ERROR;
	if(nCount < 0)	return DEVICE_S_ERROR;
	if(nCount > m_I32.nTxCount)	return DEVICE_S_ERROR;
	memcpy(m_pcTx + m_I32.nTxSize * (nCount - 1), pch, m_I32.nTxSize);
	*pnWriteCount	= m_I32.nTxSize;
	
	//=======
	// 通知有數據發送
	//=======
	if(m_icomm.cWrited == 1)	return DEVICE_E_TX_FULL;
	m_icomm.cWrited		= 1;
	write(m_icomm.nTx, &m_icomm.cWrited, 1);
	
	return DEVICE_SUCCESS;
}
WORD    CI32::ReadToken (BYTE* pch, int nBufferSize, int* pnReadCount)
{
	return	DEVICE_S_ERROR;
}
WORD    CI32::WriteToken(BYTE* pch, int nCount,      int* pnWriteCount)
{
	return	DEVICE_S_ERROR;
}

WORD    CI32::GetRxState(WORD* pwState)
{
	return	DEVICE_S_ERROR;
}
WORD    CI32::GetTxState(WORD* pwState)
{
	return	DEVICE_S_ERROR;
}

WORD    CI32::AbortRx()
{
	return	DEVICE_S_ERROR;
}
WORD    CI32::AbortTx()
{
	return	DEVICE_S_ERROR;
}


WORD    CI32::StartComm()
{
	char	cData	= 0;
	write(m_icomm.nTx, &cData, 1);
	
	return	DEVICE_SUCCESS;
}
/*--------------------------------------------------------------------------+
|           Helpers                                                         |
+--------------------------------------------------------------------------*/
void 	CI32::COMM_RxRun(_ICOMM *picomm)
{
	char	cData	= 0;
	I32		*pI32	= picomm->pI32;
//	MSG		msg;
	
	//=======
	// 設置線程退出後，立即釋放所有資源
	// 設置線程收到cancel信號後，立刻退出
	//=======
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CI32::COMM_RxRun setcanceltype");
	}
	
	cData	= 0;
	write(picomm->nTx, &cData, 1);
	picomm->cWrited	= 0;
	
	while (true)
	{	
		if(read(picomm->nRx, &cData, 1) <= 0)
		{
			perror("CI32::COMM_RxRun read:");
			continue;
		}
//		//printf("receive data from %s, Data is %d ", RX_DEVICE, cData);
		if(cData == 0)	picomm->cWrited	= 0;		
		else			picomm->cReaded	= 1;
		
		if(picomm->cReaded == 1 && pI32->I32Func != NULL)
		{
//			//printf("deal data:");
			(pI32->I32Func)(picomm->pcRx, pI32->nRxSize);
			picomm->cReaded	= 0;
			write(picomm->nTx, &picomm->cReaded,1);
		}
		
/*			
		msg.message	= MSG_FIFO;
		msg.wParam	= 0;
		msg.lParam	= 0;
		g_pApplication->QueueMessage(&msg);
*/
	}
}
/*==========================================================================+
|           External                                                        |
+==========================================================================*/

