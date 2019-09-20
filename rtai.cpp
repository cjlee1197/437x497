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

//#include <rtai_shm.h>
//#include <rtai_nam2num.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "rtai.h"
#include "tmdebug.h"




/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define	DEVICE_BUFF		"mbuff"
#define	DEVICE_RBUFF	"SMEM10"
#define	DEVICE_WBUFF	"SMEM20"
/*
#ifdef	D_TEST
#define	DEVICE_RBUFF	"/dev/shm/myshm1"
#define	DEVICE_WBUFF	"/dev/shm/SMEM21"
#elif defined	TCP_HMI_NC
#define	DEVICE_RBUFF	"/dev/shm/SMEM11"
#define	DEVICE_WBUFF	"/dev/shm/SMEM21"
#elif defined	SHM_HMI_NC
#define	DEVICE_RBUFF	"/dev/shm/SMEM10"
#define	DEVICE_WBUFF	"/dev/shm/SMEM20"
#endif
*/

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void * (* PRSRUN)(void *);

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/
char*		pRtaiTxAddr = NULL;
/*==========================================================================+
|           Macro definition                                                |
+==========================================================================*/

/*==========================================================================+
|           Class declaration - Comm                                        |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CRTAI,CDevice)

CRTAI::CRTAI() : CDevice()
{
	m_pcTx		= NULL;
	m_pcRx		= NULL;

	//m_icomm.cWrited	  = 1;
	//m_icomm.cReaded	  = 0;
                      
	m_Rtai.nRxSize	  = 0;
	m_Rtai.nTxSize	  = 0;
	m_Rtai.RTAIFunc	  = NULL;
	                  
	//m_icomm.nRx		  = 0;
	//m_icomm.nTx		  = 0;
	//m_icomm.nHmiReady = 0;
	m_icomm.pRtai	  = &m_Rtai;
	m_nTxSize		  = 0;
	m_nRxSize		  = 0;
}

CRTAI::~CRTAI()
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
int		CRTAI::Create(void* pDevice)
{
	if(pDevice != NULL)	m_Rtai	= *(RTAI *)pDevice;
	
	m_nTxSize		= m_Rtai.nTxSize - 8;	//Sunny<20101115> Create TxSize - CNC_HEAD_OFFSET_LENGTH
	m_nRxSize		= m_Rtai.nRxSize;
	
	m_wOpStatus		= DEVICE_S_CLOSED;

	if(Open() == DEVICE_SUCCESS)
	{
		m_wOpStatus		= DEVICE_S_OPENED;
		return DEVICE_S_OPENED;
	}
	else
		return DEVICE_S_ERROR;
}

WORD    CRTAI::Open()
{
	//m_pcRx	= (char *)rtai_malloc(nam2num(DEVICE_RBUFF),m_nRxSize);
	//m_pcTx	= (char *)rtai_malloc(nam2num(DEVICE_WBUFF),m_nTxSize);
	m_pcRx	= create_share_memory (DEVICE_RBUFF,m_nRxSize);
	m_pcTx	= create_share_memory (DEVICE_WBUFF,m_nTxSize);
	pRtaiTxAddr = m_pcTx; //wilson lu added , 2010.12.20
	printf("m_pcRx=%x m_pcTx=%x ,DEVICE_WBUFF=%s\n", m_pcRx, m_pcTx,DEVICE_WBUFF);
	if(m_pcTx == NULL)
	{
		perror(": Tx rtai_malloc Error \n");
		return DEVICE_S_ERROR;
	}
	if(m_pcRx == NULL)
	{
		perror(": Rx rtai_malloc Error \n");
		return DEVICE_S_ERROR;
	}
	m_icomm.pcRx	= m_pcRx;
	memset(m_pcRx, 0 ,m_nRxSize);
	memset(m_pcTx, 0 ,m_nTxSize);
	printf("m_pcRx=%x m_pcTx=%x \n", m_pcRx, m_pcTx);
	//printf("pthread_create  Get  ShareMemory NC=>HMI \n");
	if(pthread_create(&m_RxThread, NULL, (PRSRUN)COMM_RxRun, &m_icomm) != 0)
	{
		perror("RTAI::_Open");
	}

	m_wOpStatus	= DEVICE_S_OPENED;
	
	return DEVICE_SUCCESS;
}

WORD    CRTAI::Close()
{
	if(m_wOpStatus	== DEVICE_S_CLOSED)
		return DEVICE_SUCCESS;
	m_wOpStatus	= DEVICE_S_CLOSED;
	pthread_cancel(m_RxThread);
	
	
	if(m_pcRx != NULL)
		delete_share_memory(m_pcRx);
	if(m_pcTx != NULL)
		delete_share_memory(m_pcTx);
		
	//if(m_pcRx != NULL)
	//	rtai_free(nam2num(DEVICE_RBUFF),m_pcRx);
	//if(m_pcTx != NULL)
	//	rtai_free(nam2num(DEVICE_WBUFF),m_pcTx);
	return DEVICE_SUCCESS;
}

WORD    CRTAI::Read (BYTE* pch, int nCount, int* pnReadCount)
{
	if(pch == NULL)	return DEVICE_S_ERROR;
	if(nCount > m_nRxSize)	nCount	= m_nRxSize;
	memcpy(pch, m_pcRx, nCount);
	*pnReadCount	= nCount;
	
	//=======
	// 3qa????μ|?§12|
	//=======
	//write(m_icomm.nTx, &m_icomm.cReaded,1);	
	
	return DEVICE_SUCCESS;
}

WORD    CRTAI::Read (BYTE* pch, BYTE cTerminator, int* pnReadCount)
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::Write(BYTE  ch)
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::Write(BYTE* pch, int nCount, int* pnWriteCount)
{
	long	nOffset = 0, nLength = 0; 		//james add offset and size
	if(pch == NULL)
		return DEVICE_S_ERROR;
	if(nCount < 0)
		return DEVICE_S_ERROR;
	
	nOffset = *(long*)pch;
	nLength = *(long*)(pch+sizeof(long));
	
	memcpy(m_pcTx + nOffset, pch+ 8 + nOffset, nLength);
	//printf("CRTAI--write m_pcTx=%x nOffset=%ld nLength:%ld\n", m_pcTx, nOffset,nLength);
	*pnWriteCount	= m_Rtai.nTxSize;
	return DEVICE_SUCCESS;
}

WORD    CRTAI::ReadToken (BYTE* pch, int nBufferSize, int* pnReadCount)
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::WriteToken(BYTE* pch, int nCount,      int* pnWriteCount)
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::GetRxState(WORD* pwState)
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::GetTxState(WORD* pwState)
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::AbortRx()
{
	return	DEVICE_S_ERROR;
}

WORD    CRTAI::AbortTx()
{
	return	DEVICE_S_ERROR;
}
/*--------------------------------------------------------------------------+
|           Helpers                                                         |
+--------------------------------------------------------------------------*/
void 	CRTAI::COMM_RxRun(_ICOMM *picomm)
{
	char	cData	= 0;
	RTAI	*pRtai	= picomm->pRtai;
	//=======
	// 3]﹐m?uμ{°h￥X??￥?Y?c??3﹐?
	// 3]﹐m?uμ{|?‥?ncel?H﹐1??￥??￥X
	//=======
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CRTAI::COMM_RxRun setcanceltype");
	}
	
	while (true)
	{	
		if(pRtai->RTAIFunc != NULL)
		{
			//printf("pRtai->nRxSize is  %d\n",pRtai->nRxSize);
			(pRtai->RTAIFunc)(picomm->pcRx, pRtai->nRxSize);		//將接收的資料存入資料庫(taskcnc.cpp -> ResValueFunc(char *pData, int nLength))
		}
		usleep(25*1000/*50*1000*/);	//Sunny<20101110> mark
	}
}

/*==========================================================================+
|           External                                                        |
+==========================================================================*/
int		CRTAI::GetOptSize(int nType)
{
	switch(nType)
	{
		case 0:
			return 	0;
		default:
			return 0;
	}
}
char*		CRTAI::create_share_memory(char* name, int size)
{
	key_t key;
	int shm_id, fd;
	char * p_map = NULL;
	
	//PrintfMarkNE_IDprintf("create share memory name =%s, size = %d\n", name, size);
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 00777);
	if(fd > 0)
	{
		write(fd , "" , 1);
		close(fd);	
	}
	key = ftok(name, 0);
	if(key == -1)
	{	
		perror("ftok error");
		return NULL;
	}
	shm_id = shmget(key, size, IPC_CREAT);
	if(shm_id == -1)
	{
		perror("shmget error");
		return NULL;	
	}
	p_map =(char*) shmat(shm_id, NULL, 0);
	printf("name is %s     \tsize is %d\n",name,size);
	return p_map;
}
		
char*		CRTAI::delete_share_memory(void* p_map)
{
	if(p_map != NULL) shmdt(p_map);
}
