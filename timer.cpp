/* Copyright (C), 1996-2001, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Function : Utilities                                                      |
|  Task     : Utilities                                                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 01/14/2005                                                     |
|  Revision : 01/14/2005                                                     |
+===========================================================================*/

#include		"timer.h"
#include		"main.h"
#include 		"tmconfig.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		TIME_USEC		10000			// 定時器時間間隔 10ms	//10000-->1000: zholy061220 1ms

#define		TIME_SUCCESS	0
#define		TIME_E_MAX		1				// 定時器到達最大數目

#define		TIME_SIGNAL		SIGALRM

/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
extern	pthread_t	g_threadMain;			// 主線程ID，main.cpp;

struct 	itimerval	m_oldtimer;
struct 	sigaction	m_oldalarm;
TIMER	m_timers[TIME_MAX];
BOOL    m_first[TIME_MAX];			//標記消息是否發送過

pthread_t		m_threadTimer;

Routine		m_Routine	= {0, 0, NULL, 0};//zholy061220

int		m_iCount	= 0;
sem_t 	m_wait;
int		m_nCloseTimer	= 0;
void	Run(int signum);
void	Run_Msg(int signum);

/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
int	init_Timer()
{
	struct itimerval timerv;
    struct sigaction siga;
    
    siga.sa_handler = Run;
    siga.sa_flags 	= 0;
    
    m_iCount	= 0;
    
    sigemptyset(&siga.sa_mask);
   // memset(&siga.sa_mask, 0, sizeof(sigset_t));

    sigaction(TIME_SIGNAL, &siga, &m_oldalarm);

    timerv.it_interval.tv_sec 	= 0;
    timerv.it_interval.tv_usec 	= TIME_USEC;
    timerv.it_value 			= timerv.it_interval;

    if (setitimer(ITIMER_REAL, &timerv, &m_oldtimer))
    {
        fprintf(stderr, "TIMER: setitimer call failed!\n");
    }
    
    return TIME_SUCCESS;
}

int	Destroy_Timer()
{
	m_nCloseTimer	= 1;
	//sem_post(&m_wait);
	
	timerclear(&m_oldtimer.it_value);
	if(setitimer(ITIMER_REAL, &m_oldtimer, NULL) == -1)
	{
        fprintf( stderr, "TIMER: setitimer call failed!\n");
    }	
	
	pthread_cancel(m_threadTimer);
    if(sigaction (TIME_SIGNAL, &m_oldalarm, NULL) == -1)
    {
        fprintf( stderr, "TIMER: sigaction call failed!\n");
    }
    return TIME_SUCCESS;
}


void	Run_Msg(int signum)
{
	static WORD		i = 0;
	if( i > 60000)
		i = 0;
	SendMsg(MSG_TIMER, 0, i, NULL);		//20120222 modify Update View
	//printf("====test ====Run_Msg \n");
	//for(i = 0; i < TIME_MAX; i++)
	//{
	//	if(m_timers[i].plCounter != NULL)
	//	{	
	//		if(*m_timers[i].plCounter > 0)
	//		{
	//			m_first[i] = TRUE;
	//		}
	//		else
	//		{
	//			if(m_first[i])
	//			{
	//				SendMsg(MSG_TIMER, 0, (LPARAM)m_timers[i].plCounter, NULL);
	//				//msg.message	= MSG_TIMER;
	//				//msg.wParam	= 0;
	//				//msg.lParam	= (LPARAM)m_timers[i].plCounter;
	//				//g_pApplication->QueueMessage(&msg);
	//				m_first[i] = FALSE;
	//			}
	//		}
	//	}
	//}
}

void	Run(int signum)
{
	int		i;
	MSG		msg;
	
	if(signum != TIME_SIGNAL) return;
	
	for(i = 0; i < TIME_MAX; i++)
	{
		if(m_timers[i].plCounter != NULL)
		{	
			*m_timers[i].plCounter	-= 10;//10-->1: zholy061220
			//if(*m_timers[i].plCounter > 0)
			//{
			//	m_first[i] = TRUE;
			//}
			//else
			//{
			//	if(m_first[i])
			//	{
			//		//SendMsg(MSG_TIMER, 0, (LPARAM)m_timers[i].plCounter, NULL);
			//		//msg.message	= MSG_TIMER;
			//		//msg.wParam	= 0;
			//		//msg.lParam	= (LPARAM)m_timers[i].plCounter;
			//		//g_pApplication->QueueMessage(&msg);
			//		m_first[i] = FALSE;
			//	}
			//}
		}
	}
/*	
	if (m_Routine.nFlag == 1)//>>>zholy061220
	{
		m_Routine.lCounter	-=10;
		if (m_Routine.lCounter <= 0)
		{
			m_Routine.lCounter	= m_Routine.nCount;
			(*m_Routine.pFunc)();
		}
	}*/
}

int	AddTimer(long* plCounter)
{
	int			i;
	sigset_t 	sa_mask;

	sigemptyset(&sa_mask);
    sigaddset(&sa_mask, TIME_SIGNAL);
    pthread_sigmask(SIG_BLOCK, &sa_mask, NULL);
    //printf("AddTimer m_iCount=%d\n", m_iCount);
    if(m_iCount == TIME_MAX)
	{
		pthread_sigmask(SIG_UNBLOCK, &sa_mask, NULL);
		return TIME_E_MAX;
	}
    
    for(i = 0; i < TIME_MAX; i++)
	{
		if(m_timers[i].plCounter == plCounter)
		{
			pthread_sigmask(SIG_UNBLOCK, &sa_mask, NULL);
			return TIME_E_EXIST;
		}
	}
	for(i = 0; i < TIME_MAX; i++)
	{
		if(m_timers[i].plCounter == NULL)
		{
			m_timers[i].plCounter		= plCounter;
			m_iCount++;
			break;
		}
	}
    pthread_sigmask(SIG_UNBLOCK, &sa_mask, NULL);
   
    return TIME_SUCCESS;
}

int	DeleteTimer(long* plCounter)
{
	int			i;
	sigset_t 	sa_mask;
	
	sigemptyset(&sa_mask);
    sigaddset(&sa_mask, TIME_SIGNAL);
    pthread_sigmask(SIG_BLOCK, &sa_mask, NULL);
    
    for(i = 0; i < TIME_MAX; i++)
	{
		if(m_timers[i].plCounter == plCounter)
		{
			m_timers[i].plCounter	= NULL;
			m_iCount--;
			
			pthread_sigmask(SIG_UNBLOCK, &sa_mask, NULL);
    
			return TIME_SUCCESS;
		}		
	}
	  
    pthread_sigmask(SIG_UNBLOCK, &sa_mask, NULL);
    
    return TIME_E_NOTIMER;
}

void* TimerRun (void* data)
{
	int	nCount = 0;
    //init_Timer();		//James mark 2012/2/8 18:46:36
    
    //=======
	// 設置線程退出後，立即釋放所有資源
	// 設置線程收到cancel信號後，立刻退出
	//=======
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("TimerRun");
		return NULL;
	}
	
	while(1)
	{
		if(nCount >= 10)
		{
			Run_Msg(TIME_SIGNAL);
			nCount = 0;
		}
		Run(TIME_SIGNAL);
		nCount++;
		if(CtmConfig::GetInstance()->GetTimerTaskType()==1)
		{
			if(!(nCount%5))
				Run_Msg(TIME_SIGNAL);
		}
		usleep(TIME_USEC);
	};
    /*
    while(m_nCloseTimer != 1)
    {
    	sem_wait((sem_t*)data);
    }
	*/
    return NULL;
}

int Init_Timer()
{	
	sem_init(&m_wait, 0, 0);
	
	pthread_create(&m_threadTimer, NULL, TimerRun, &m_wait);
    return 0;
}

//>>>zholy061220
int	AddRoutine(int nCount, SENDFUNC	pFunc)
{
	if (m_Routine.nFlag == 1)
	{
		perror("AddRoutine Error");
		return 0;
	}
	m_Routine.nFlag		= 1;
	m_Routine.nCount	= nCount;
	m_Routine.pFunc		= pFunc;
	return	1;
}

int	DeleteRoutine()
{
	m_Routine.nFlag		= 0;
	m_Routine.nCount	= 0;
	m_Routine.pFunc		= NULL;
	return	1;
}
//<<<zholy061220
