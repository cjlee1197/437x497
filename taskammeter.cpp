/*===========================================================================+
|  Class    : TaskAmmeter                                                    |
|  Task     : Ammeter Service Routine                                 		 |
|----------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                     |
|  Link     : Gcc V3.2.2                                                   	 |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Gerry                                                          |
|  Version  : V1.00                                                          |
|  Creation : 2011/6/2 15:03:36                                              |
|  Revision :                                                                |
+===========================================================================*/
#include    "utils.h"
#include    "taskammeter.h"
#include    "main.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
IMPLEMENT_DYNCREATE(CTaskAmmeter, CtmPackClass)

typedef void * (* PRSRUN)(void *);

CTaskAmmeter*          g_ptaskAmmeter		= NULL;
CTaskAmmeter*          g_ptaskPickerComm	= NULL;
CTaskAmmeter*		   g_pTaskHostRS232		= NULL;
BOOL				   g_bPickerLineState	= FALSE;
BOOL				   g_bTecnoLineState	= FALSE;

#define	AMMETER_DEVICE_PATHNAME			"/dev/ttyS1"
/*===========================================================================+
|           Class implementation - CTaskMoni                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CTaskAmmeter::CTaskAmmeter() : CTask(), CtmPackClass()
{
	m_fdcom = -1;
	memset(&m_termios, 0, sizeof(termios));
	
	m_nBaudRate = B9600;
	//m_notifyer = NULL;
	sem_init(&send_sem, 0, 0);
	sem_init(&read_sem, 0, 0);
}

CTaskAmmeter::~CTaskAmmeter()
{
	sem_destroy(&send_sem);
	sem_destroy(&read_sem);
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

int	CTaskAmmeter::CreateSelf()
{
	//Open();

	pthread_attr_init(&m_ThreadAttr);
	pthread_attr_setdetachstate(&m_ThreadAttr, PTHREAD_CREATE_DETACHED);
	pthread_create(&m_RxThread, NULL, (PRSRUN)RecvFunc, this);
	
	return 0;
}

int	CTaskAmmeter::FreeSelf()
{
	//Close();
	
	return 0;
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

int CTaskAmmeter::Open(tmRS485AMMETER* pRS485Ammeter)
{
	struct termios newtermios;
	char	pszPath[256];
	WORD	wChannel = 1;
	
	memset(pszPath, 0, sizeof(pszPath));
	memset(&newtermios, 0, sizeof(termios));
	int nResult = -1;
	
	Close();
	
	if(pRS485Ammeter == NULL)
	{
		sprintf(pszPath, "%s%d", COM_PATH, wChannel);
	}
	else
	{
		sprintf(pszPath, "%s%d", COM_PATH, pRS485Ammeter->wChannel);
		memcpy(&newtermios, &(pRS485Ammeter->newtermios), sizeof(termios));
		m_nBaudRate = pRS485Ammeter->nBaudRate;
	}
	do
	{
		if((m_fdcom = open(pszPath, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0)
		{
			perror("CtmTaskAmmeterModbus open");
			break;
		}
		
		if (fcntl(m_fdcom, F_SETFL, 0/*FASYNC*/) != 0)
		{
			perror("CtmTaskAmmeterModbus fcntl");
			break;
		}
		
		if(!isatty(m_fdcom))
		{
			perror("CtmTaskAmmeterModbus isatty");
			break;
		}
		
		if (tcgetattr(m_fdcom, &m_termios) != 0)
		{
			perror("CtmTaskAmmeterModbus tcgetattr");
			break;
		}
		if(pRS485Ammeter == NULL)
		{
			newtermios.c_cflag 		= CLOCAL | CREAD;
	
			newtermios.c_cflag      &= ~CSIZE;
			newtermios.c_cflag      |= CS8;
			
			newtermios.c_cflag      &= ~CSTOPB;
		
			newtermios.c_iflag 		= INPCK; 
			newtermios.c_cflag      |= PARENB;
			newtermios.c_cflag      &= ~PARODD;
		
			newtermios.c_oflag 		= 0;
			newtermios.c_lflag 		= 0;
			newtermios.c_cc[VTIME] 	= 1;//5;	//500ms
			newtermios.c_cc[VMIN] 	= 0;
		}
		cfsetispeed(&newtermios, m_nBaudRate);
		cfsetospeed(&newtermios, m_nBaudRate);
		fcntl(m_fdcom, F_SETFL, FASYNC);
		tcflush(m_fdcom, TCIOFLUSH);
		
		if (tcsetattr(m_fdcom, TCSANOW, &newtermios) != 0)
		{
			perror("CtmTaskAmmeterModbus tcsetattr");
			break;
		}
		nResult = 0;
		
	} while(0);
	
	if (nResult != 0)
	{
		printf("Open failed!\n");
		Close();
	}
	
	return nResult;
}

int CTaskAmmeter::Close()
{
	int nResult = 0;
	
	do
	{
		if (m_fdcom < 0)
		{
			nResult = 0;
			break;
		}
		
		tcsetattr(m_fdcom, TCSANOW,&m_termios);
	
		if (close(m_fdcom) == -1)
		{
			perror("Close");
			nResult = -1;
			break;
		}
		
		m_fdcom = -1;
		
	} while(0);
	return nResult;
}

void CTaskAmmeter::SetBaudRate(int nRate)
{
	m_nBaudRate = nRate;
	//Close();
	Open();
}

int CTaskAmmeter::SendData(void *pData, int nLen)
{
	//printf("SendData nLen=%d m_fdcom=%d m_nSendFlag=%d \n", nLen, m_fdcom, m_nSendFlag);
	//if (m_fdcom >= 0)
	//{
	//	write(m_fdcom, pData, nLen);
	//}
	
	return 0;
}

void	CTaskAmmeter::WaitSend()
{
	sem_wait(&send_sem);
}

void	CTaskAmmeter::StartSend()
{
	sem_post(&send_sem);
	
}

void	CTaskAmmeter::WaitRecv()
{
	sem_wait(&read_sem);
}

void	CTaskAmmeter::StartRecv()
{
	sem_post(&read_sem);
}


void * CTaskAmmeter::RecvFunc(void *pClass)
{
	int nLen = 0;
	BYTE buf = 0;
	
	pthread_detach(pthread_self());
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("RecvFunc setcanceltype");
	}
	
	((CTaskAmmeter *)pClass)->StartSend();
	((CTaskAmmeter *)pClass)->WaitRecv();
	
	while (1)
	{	
		nLen = read(((CTaskAmmeter *)pClass)->m_fdcom, &buf, 1);
		((CTaskAmmeter *)pClass)->Parse(&buf, nLen);	//don't check nLen, we need to use zero length data.
	}
	return 0;
}
