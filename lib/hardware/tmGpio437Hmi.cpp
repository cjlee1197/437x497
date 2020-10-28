/*==========================================================================+
|  Class    : kb.cpp														|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               | 
|---------------------------------------------------------------------------|
|  Author   : yang                                                       	|
|  Version  : V1.00                                                         |
|  Creation : 2005/01/18                                                    |
|			: 2005/01/31	netdigger										|
|			: 2006/06/06	James											|
|  Remark	: 																|
+==========================================================================*/
#include	"tmGpio437Hmi.h"
#include	"../../utils.h"
#include	"../../tmconfig.h"
#include	"../../device.h"
#include    "../../common.h"
#include	"../../main.h"
#include	"../../tmshmsg.h"
#include    "../../taskmoni.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
typedef void * (* PGPIORUN)(void *);
int			m_nGpio;					// 鍵盤設備的文件句柄	
pthread_mutex_t m_mutex;
pthread_mutexattr_t attr;
/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/
IMPLEMENT_DYNCREATE(CtmGpio437Hmi, CtmGpio)
CtmGpio437Hmi::CtmGpio437Hmi() : CtmGpio()
{
	
	
}

CtmGpio437Hmi::~CtmGpio437Hmi()
{
		
}

int		CtmGpio437Hmi::CreateSelf()
{
	CtmGpio::CreateSelf();
		
	pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK_NP);
  pthread_mutex_init(&m_mutex, &attr);
	m_nGpio			= 0;
	Init_Gpio();
	return 0;
}


int		CtmGpio437Hmi::FreeSelf()
{
	CtmGpio::FreeSelf();
	Destroy_Gpio();
	return 0;
}

/*--------------------------------------------------------------------------+
| Function : void Init_Gpio()                                              	|
| Task     : Init Gpio input                                   			|
+---------------------------------------------------------------------------+
| Call     : Init_Gpio()                                                		|
|                                                                           |
| Parameter: None                      -                                    |
|                                                                           |
| Return   : None                      -                         	 		|
+--------------------------------------------------------------------------*/


void CtmGpio437Hmi::Init_Gpio()
{
	m_nGpio = open("/dev/kutio", O_RDWR);
	if(m_nGpio <= 0) 
	{
		printf("gpio error~~~~~\n");
		return ;
	}
	pthread_create(&m_GpioReadInput, NULL, (PGPIORUN)Gpio_InputRun, NULL);
}

/*--------------------------------------------------------------------------+
| Function : void Destroy_KB()                                           	|
| Task     : Restore keyboard input                                			|
+---------------------------------------------------------------------------+
| Call     : Destroy_KB()                                             		|
|                                                                           |
| Parameter: None                      -                                    |
|                                                                           |
| Return   : None                      -                         	 		|
+--------------------------------------------------------------------------*/
void	CtmGpio437Hmi::Destroy_Gpio()
{
	if(m_GpioReadInput != 0)		pthread_cancel(m_GpioReadInput);

	if(m_nGpio > 0)
	{
		close(m_nGpio);
	}
	m_nGpio 		= -1;
}

void 	Gpio_InputRun()
{
	char data;
	while (TRUE)
	{
		int rst = read(m_nGpio, &data, 1);
		if(rst > 0)
		{
			if(Gpio_Input != data || KeyMode==-1)
			{
				g_ptaskCmd->SetIdle(FALSE);
				if(g_ptaskMoni!=NULL && g_ptaskMoni->IsSleep()) continue;
				Gpio_Input = data;
				ReadGpio(Gpio_Input);
			}
		}
		else printf("read gpio fail!\n");
		usleep(500);
	}
}

void	ReadGpio(char /*KEYCODE*/ io)
{
	MSG				msg;
	static int count = 0;
	if(KeyMode!=-1) count++;
	if(count>1048576) count=1;
	msg.message		= MSG_GPIO_READ;
	msg.wParam		= (WPARAM)io;
	msg.lParam		= count;
	printf("CtmGpio437Hmi ReadGpio=%x cnt=%d\n",(char)msg.wParam,msg.lParam);
	SetDBValue("MACHINE_INTERFACE_CONTROL_RSV15",(char)msg.wParam&0xb);
	g_pApplication->QueueMessage(&msg);	
}

char old_status=0;
void	CtmGpio437Hmi::Gpio_OutputWrite(char* io_status)
{
	pthread_mutex_lock(&m_mutex);
	char tmp = (0x08&old_status);
	old_status = *io_status;
	old_status |= (0x08&tmp);
	int	size = write(m_nGpio, &old_status, 1);
	//printf("Gpio_OutputWrite = %d\n",old_status);
	pthread_mutex_unlock(&m_mutex); 
}


void	CtmGpio437Hmi::Gpio_buzzer(bool buzzer_enable)
{
	pthread_mutex_lock(&m_mutex);
	//printf("0Gpio_buzzer = %d\n",old_status);
	if(buzzer_enable)
		old_status |= 0x08;
	else
		old_status &= 0x07;
	//printf("1Gpio_buzzer = %d\n",old_status);
	int	size = write(m_nGpio, &old_status, 1);
	pthread_mutex_unlock(&m_mutex); 
}


#ifndef	D_BEOBJECT_CtmGpio437Hmi
CObject*	CreateInstance(char* Name)
{
	//printf("gpio CreateInstance\n");
	if (g_pgpio == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CtmGpio437Hmi*>(pResult))->CreateSelf();
		g_pgpio = static_cast<CtmGpio437Hmi*>(pResult);
		if(pResult == NULL) printf("pResult=NULL");
	}
	return g_pgpio;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmGpio437Hmi*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif