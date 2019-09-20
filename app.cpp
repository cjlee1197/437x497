/*==========================================================================+
|  Class    : Application                                                   |
|  Task     : Application Processor                                         |
|---------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                  |
|  Link     : MSVC V1.50 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                    	|
|  Version  : V1.00                                                         |
|  Creation : 04/02/1995                                                    |
|  Revision : 04/24/1996                                                    |
|			: 2005/01/18	netdigger										|
+==========================================================================*/

#include    "utils.h"
#include    "app.h"
#include    "cmddef.h"
#include	"main.h"
#include	"_kb.h"
#include	"_video.h"
#include	"cbitmap.h"
#include	"tmconfig.h"
#include	"package.h"
#include	"tmoper.h"
//#include	"memwatch.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define 		MAX_MSGLIST			64
#define			MAX_TIMELIST		20
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagTIMELIST
{
	TASKID		TaskID;
	int			Counter;
}	TIMELIST;

typedef struct tagMsgList
{
	TASKID		TaskID;
	int			Message;
	int			Counter;
}	MsgList;
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
//=======
//      Initialize static class member
//=======
CTask*      	CApp::m_ptaskCurrent = NULL;
MsgList			g_MsgList[MAX_MSGLIST];
TIMELIST		g_TimerList[MAX_TIMELIST];

int				g_iTempLanguage = 0;
MSGQUEUE		CApp::m_MsgQueue;
MSGQUEUE 		CApp::m_msg[CMD_MAX_CHANNEL];
BOOL			CApp::m_bReset = FALSE;
BOOL			g_bRestartSystem	=FALSE;
extern			CParseObject* 	ParseView1;
extern	sem_t 	g_SemGUI;
extern int g_FbIotcol;
/*==========================================================================+
|           Class implementation - Application                              |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
CApp::CApp()
{
	int 	i;
	m_nRunCount = 0;
	if(!InitMsgQueue(&m_MsgQueue, 256))			//	fans 2009/9/1 11:28上午 modify 0->256
	{
		fprintf(stderr, "CAPP: Init MSGQUEUE failed!\n");
		exit (1);
	}
	
	for(i = 0; i < MAX_TIMELIST; i ++)
		g_TimerList[i].TaskID	= HTSK_INVALID;
}

CApp::~CApp()
{
	DestroyMsgQueue(&m_MsgQueue);
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : CApp::Run()                                                    |
|  Task     : Application processor and task scheduler                       |
+----------------------------------------------------------------------------+
|  Call     : CApp::Run()                                                    |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void        CApp::Run()
{
	
	int		i;
	MSG		msg;
	
	while(GetMessage(&msg, HWND_DESKTOP))
	{
		if(1 == m_nRunCount)
			continue;
/*		
		if(msg.message == MSG_KEY)
		{
			//printf("CApp::Run key = %04x\n", msg.wParam);
		}
*/		
		if(msg.message == MSG_KEY && msg.wParam == 0x6300)
		{
			SaveScreen();
			continue;
		}
		if(msg.message == MSG_KEY && msg.wParam == 0x5800)
		{
			SetDBValue("CNC_ACTION_ID", ID_CNC_EXIT);	//Sunny<20100408> F12 Exit,Tell P3 also.		//Mario debug
			PostQuitMessage(HTSK_MAIN);
			continue;
		}
		//	fans add 2008/6/16 11:42上午
		if (g_ptaskCmd != NULL && g_ptaskCmd->GetFocus() != NULL)
		{
			g_ptaskCmd->GetFocus()->WndProc(msg.message, msg.wParam, msg.lParam);
		}
		
		switch(msg.message)
		{
		case MSG_TIMER:
			//ChangePosRegion();
			for(i = 0; i < MAX_TIMELIST; i++)
			{
				//=======
				// APP的消息處理函數是另外的
				//=======
				if(g_TimerList[i].TaskID == HTSK_MAIN)
				{
					continue;
				}
				else if(g_TimerList[i].TaskID != HTSK_INVALID)
				{
					if((m_ptaskCurrent = CTask::GetTask(g_TimerList[i].TaskID)) != NULL)
					{
						//CTask::GetTask(g_TimerList[i].TaskID)->Run();
						if (m_ptaskCurrent->GetStatus() == TASK_NORMAL)
						{
							//	fans modiy 2009-4-7 12:45:58
							//m_ptaskCurrent->Run();
							//ioctl(g_FbIotcol, 0xFFFF11, NULL);
							m_ptaskCurrent->WndProc(msg.message, msg.wParam, msg.lParam);
							//ioctl(g_FbIotcol, 0xFFFF12, NULL);							
						}
					}
				}
			}
			break;
		default:
			for(i = 0; i < MAX_MSGLIST; i ++)
			{
				if(g_MsgList[i].Message == msg.message)
				{
					if((m_ptaskCurrent = CTask::GetTask(g_MsgList[i].TaskID)) != NULL)
					{
						if (m_ptaskCurrent->GetStatus() == TASK_NORMAL)
						{
							//	fans modiy 2009-4-7 12:45:58
							//m_ptaskCurrent->Run();
							m_ptaskCurrent->WndProc(msg.message, msg.wParam, msg.lParam);
						}
						
					}
				}
			}
			break;
		}
		
		char*	pszForm = NULL;//[256];
		if (GetMessage(CMD_SYSTEM, &msg))
        {
        switch (msg.wMessage)	
            {
            case _SYS_RESET:
            	if (g_ptaskCmd->GetTaskCmdCount() > 1)
            		{
            		m_bReset = TRUE;
            		Exit();
            		}
            	else
            		{
		            if(g_pInit != NULL)
		            	g_pInit->Init_System();
		            else
		            	perror("g_pInit Error \n");
		            ReleaseCommandTarget(g_pMainFrame);
		            g_pMainFrame->GetCurrentView()->Clear();
					//add by Wong 2014/12/8 20:05:34
					if(ParseView1!=NULL)
					{
						ParseView1->ReInitData();	
					}
		            g_bRestartSystem=TRUE;
		            if(g_ParseMainFrame!=NULL)
		            {
		            	delete g_ParseMainFrame;
		            	g_ParseMainFrame = NULL;
		            	
		            }
					OpenMainForm();
					pszForm = *(char**)g_pMainFrame->GetCurrentView()->GetPropValue("overviewname");
					g_pMainFrame->GetCurrentView()->OnCommand(pszForm);
					ResetTime();
					//PutKeyTail(_F1);	//Sunny<20090917>mark 避免demo時自動Key F1
					//printf("App-Run \n");
					m_bReset = FALSE;
					}
                break;
            case _SYS_SHELL:
                break;
            case _SYS_EXIT:
                PostQuitMessage(HTSK_MAIN);
                break;
            default:
                SetMessage(CMD_SYSTEM, &msg);
                break;
            }
        }
	}
}


void        CApp::Run2()
{
	int		i;
	MSG		msg;
	
	m_nRunCount = 1;
	
	while(GetMessage2(&msg, HWND_DESKTOP))
	{
/*		
		if(msg.message == MSG_KEY)
		{
			//printf("CApp::Run key = %04x\n", msg.wParam);
		}
*/		
		if(msg.message == MSG_KEY && msg.wParam == 0x6300)
		{
			SaveScreen();
			continue;
		}
		if(msg.message == MSG_KEY && msg.wParam == 0x5800)
		{
			PostQuitMessage(HTSK_MAIN);
			continue;
		}
		//	fans add 2008/6/16 11:42上午
		if (g_ptaskCmd != NULL && g_ptaskCmd->GetFocus() != NULL)
		{
			g_ptaskCmd->GetFocus()->WndProc(msg.message, msg.wParam, msg.lParam);
		}
		
		switch(msg.message)
		{
		case MSG_TIMER:
			//ChangePosRegion();
			for(i = 0; i < MAX_TIMELIST; i++)
			{
				//=======
				// APP的消息處理函數是另外的
				//=======
				if(g_TimerList[i].TaskID == HTSK_MAIN)
				{
					continue;
				}
				else if(g_TimerList[i].TaskID != HTSK_INVALID)
				{
					if((m_ptaskCurrent = CTask::GetTask(g_TimerList[i].TaskID)) != NULL)
					{
						if (m_ptaskCurrent->GetStatus() == TASK_NORMAL)
						{
							//	fans modiy 2009-4-7 12:45:58
							//m_ptaskCurrent->Run();
							m_ptaskCurrent->WndProc(msg.message, msg.wParam, msg.lParam);
						}
					}
				}
			}
			break;
		default:
			for(i = 0; i < MAX_MSGLIST; i ++)
			{
				if(g_MsgList[i].Message == msg.message)
				{
					if((m_ptaskCurrent = CTask::GetTask(g_MsgList[i].TaskID)) != NULL)
					{
						//CTask::GetTask(g_MsgList[i].TaskID)->Run();
						if (m_ptaskCurrent->GetStatus() == TASK_NORMAL)
						{
							//	fans modiy 2009-4-7 12:45:58
							//m_ptaskCurrent->Run();
							m_ptaskCurrent->WndProc(msg.message, msg.wParam, msg.lParam);
						}
					}
				}
			}
			break;
		}
		
		char*	pszForm = NULL;//[256];
		if (GetMessage2(CMD_SYSTEM, &msg))
        {
        switch (msg.wMessage)
            {
            case _SYS_RESET:
            	if (g_ptaskCmd->GetTaskCmdCount() > 1)
            		{
            		m_bReset = TRUE;
            		Exit();
            		}
            	else
            		{
		            if(g_pInit != NULL)
		            	g_pInit->Init_System();
		            else
		            	perror("g_pInit Error \n");
		            ReleaseCommandTarget(g_pMainFrame);
		            g_pMainFrame->GetCurrentView()->Clear();
		            //add by Wong 2014/12/8 20:05:34
		            if(ParseView1!=NULL)
					{
						ParseView1->ReInitData();	
					}
		            g_bRestartSystem=TRUE;
		            if(g_ParseMainFrame!=NULL)
		            {
		            	delete g_ParseMainFrame;
		            	g_ParseMainFrame = NULL;
		            }
					OpenMainForm();
					pszForm = *(char**)g_pMainFrame->GetCurrentView()->GetPropValue("overviewname");
					g_pMainFrame->GetCurrentView()->OnCommand(pszForm);
					ResetTime();
					PutKeyTail(_F1);
					//printf("App-Run2 \n");
					m_bReset = FALSE;
					}
                break;
            case _SYS_SHELL:
                break;
            case _SYS_EXIT:
                PostQuitMessage(HTSK_MAIN);
                break;
            default:
                SetMessage(CMD_SYSTEM, &msg);
                break;
            }
        }
	}
}

BOOL        CApp::SetMessage(WORD wChannel, MSG* pmsg)
{
	WORD        wTaskStatus;
	CTaskCmd*   ptaskCmd;
	CTask*      ptaskCurrent;
	long        lCounter;

	switch (wChannel)
    {
    	case CMD_SYSTEM:
        switch (pmsg->wMessage)
        {
	        case _SYS_CHANGE:
	            g_ptaskCmd->OnChange(pmsg->wIDControl);
	            return TRUE;
	        case _SYS_PUSHFOCUS:
	            g_ptaskCmd->PushFocus();
	            return TRUE;
	        case _SYS_PUSHFOCUS2:
	            g_ptaskCmd->PushFocus2();
	            return TRUE;
	        case _SYS_POPFOCUS:
	            g_ptaskCmd->PopFocus();
	            return TRUE;
	        case _SYS_POPFOCUS2:
	            g_ptaskCmd->PopFocus2();
	            return TRUE;
	        case _SYS_SETFOCUS:
	            g_ptaskCmd->SetFocus(pmsg->pwnd);           // ??? The CView will be OnLoseFocus() before execute any command
	            return TRUE;                                // ??? So if CDialog is called from command then the CView is lose focus before CDialog is displayed
	        case _SYS_SETFOCUS2:
	            g_ptaskCmd->SetFocus2(pmsg->pwnd);           // ??? The CView will be OnLoseFocus() before execute any command
	            return TRUE; 
			case _SYS_KILLFOCUS:
	            g_ptaskCmd->KillFocus(pmsg->pwnd);
	            return TRUE;
			case _SYS_KILLFOCUS2:
	            g_ptaskCmd->KillFocus2(pmsg->pwnd);
	            return TRUE;
	        case _SYS_NEWSESSION:
	        			RegionUpdateStop = TRUE;
	        			g_nCommand = 2;
                 if (g_wIDView == ID_VIEW_OVERVIEW) g_wIDView = 0;
                wTaskStatus = g_ptaskCmd->GetStatus();      // Save current task status
                g_ptaskCmd->SetStatus(TASK_SUSPEND);        // Suspend it
                ptaskCmd = g_ptaskCmd;                      // Save current command processor
                g_ptaskCmd = static_cast<CTaskCmd*>(g_pPackage->CreateClassInstance("HostCmd"));	//James Modify 配合外放so修改 2008/6/10 10:13上午
                g_ptaskCmd->SetFocus(pmsg->pwnd);           // Redirect the command target
                ChangePos();
								Run2();										// 配合另外一個線程調用msgbox Run()-> Run2()
	        			RegionUpdateStop = TRUE;
                g_pPackage->ReleaseClassInstance("HostCmd", g_ptaskCmd);	//James modify 配合外放so修改 2008/6/10 10:13上午
                g_ptaskCmd = ptaskCmd;                      // Restore the old command processor
                g_ptaskCmd->PopFocus();
                g_ptaskCmd->SetStatus(wTaskStatus);         // Restore the old task status
              	if (m_bReset) 
              		Reset();
              	ChangePos();
	        			g_nCommand = 1;
	            return TRUE;
	        case _SYS_WAIT:
	            wTaskStatus = m_ptaskCurrent->GetStatus();  // Save current task status
	            m_ptaskCurrent->SetStatus(TASK_IDLE);       // Suspend it
	            lCounter = pmsg->lCounter;
	            m_ptaskCurrent = ptaskCurrent;              // Restore the current task pointer
	            m_ptaskCurrent->SetStatus(wTaskStatus);     // Restore the old task status
	            return TRUE;
	        default: 
	        	break;
        }
        break;
    	case CMD_KEY:
        switch (pmsg->wMessage)
        {
	        case _KEY_GETBYPASS:
	            pmsg->bBypass = g_ptaskCmd->GetBypass();
	            return TRUE;
	        case _KEY_SETBYPASS:
	            g_ptaskCmd->SetBypass(pmsg->bBypass);
	            return TRUE;
	        case _KEY_ADDHEAD:
//	            _PutKeyHead(pmsg->wKey);
	            return TRUE;
	        case _KEY_ADDTAIL:
	            _PutKeyTail(pmsg->wKey);
	            return TRUE;
	        default:
	            break;
        }
        break;
    	default:
        break;
	}

	MSGQUEUE*   pMSG = &m_msg[wChannel];
	
	if (!pMSG->bQueueFull)
	{
	    pMSG->msgQueue[pMSG->wInput++] = *pmsg;
	    if (pMSG->wInput >= pMSG->wQueueEnd)
	        pMSG->wInput = 0;
	    if (pMSG->wInput == pMSG->wOutput)
	        pMSG->bQueueFull = TRUE;
	    pmsg->message = MSG_MMI;
	    QueueMessage(pmsg);
	    return TRUE;
	}
	
	return FALSE;
}

BOOL	CApp::GetMessage(WORD wChannel, MSG* pmsg)
{
	
	MSGQUEUE*   pMSG = &m_msg[wChannel];
	
	if ((pMSG->wInput != pMSG->wOutput) || (pMSG->bQueueFull))
	{
	    *(pmsg) = pMSG->msgQueue[pMSG->wOutput++];
	    if (pMSG->wOutput >= pMSG->wQueueEnd) pMSG->wOutput = 0;
	    pMSG->bQueueFull = FALSE;
	    return TRUE;
	}
	
	return FALSE;
}

BOOL	CApp::GetMessage2(WORD wChannel, MSG* pmsg)
{
	
	MSGQUEUE*   pMSG = &m_msg[wChannel];
	
	if ((pMSG->wInput != pMSG->wOutput) || (pMSG->bQueueFull))
	{
	    *(pmsg) = pMSG->msgQueue[pMSG->wOutput++];
	    if (pMSG->wOutput >= pMSG->wQueueEnd) pMSG->wOutput = 0;
	    pMSG->bQueueFull = FALSE;
	    return TRUE;
	}
	
	return FALSE;
}
BOOL CApp::InitMsgQueue (PMSGQUEUE pMsgQueue, int iBufferLen)
{
    pMsgQueue->dwState = QS_EMPTY;

    pthread_mutex_init(&pMsgQueue->lock, NULL);
    sem_init(&pMsgQueue->wait, 0, 0);
	pMsgQueue->readpos 			= 0;
    pMsgQueue->writepos 		= 0;
    
    if (iBufferLen <= 0)
        iBufferLen 	= DEF_MSGQUEUE_LEN;
        
    pMsgQueue->msg 	= (MSG *)malloc(sizeof(MSG) * iBufferLen);
    pMsgQueue->len 	= iBufferLen;
    return pMsgQueue->msg	   != NULL;
}

void CApp::DestroyMsgQueue (PMSGQUEUE pMsgQueue)
{
    free (pMsgQueue->msg);
    pMsgQueue->msg 	= NULL;
}

/*--------------------------------------------------------------------------+
|  Function : int	GetMessage (PMSG pMsg, HWND hWnd)                      	|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     : 線程專用函數                    								|
|                                                                           |
|  Parameter: imMe          - task句柄                          			|
|                                                                         	|
|  Return   :               -                   							|
+--------------------------------------------------------------------------*/
int	CApp::GetMessage(PMSG pMsg, HWND hWnd)
{
	if( 1 == m_nRunCount)
		return 1;
	
    PMSGQUEUE pMsgQueue  = &m_MsgQueue;

    memset(pMsg, 0, sizeof(MSG));

checkagain:

    if(pMsgQueue->dwState & QS_QUIT)
    {
        pMsg->hwnd 		= hWnd;
        pMsg->message 	= MSG_QUIT;
        pMsg->wParam 	= 0;
        pMsg->lParam 	= 0;
        pMsg->pAdd 		= NULL;
		
        pMsgQueue->dwState &= ~QS_QUIT;   
   
        return 0;
    }
    if(pMsgQueue->dwState & QS_POSTMSG)
    {
        pthread_mutex_lock(&pMsgQueue->lock);
        if(pMsgQueue->readpos != pMsgQueue->writepos)
        {            
            *pMsg 		= pMsgQueue->msg[pMsgQueue->readpos];            
            //pMsg->pAdd 	= NULL;
            pMsgQueue->readpos++;
            if (pMsgQueue->readpos >= pMsgQueue->len)
            	pMsgQueue->readpos = 0;
            pthread_mutex_unlock (&pMsgQueue->lock);

            return 0x00000001;
        }
        else
            pMsgQueue->dwState &= ~QS_POSTMSG;

        pthread_mutex_unlock (&pMsgQueue->lock);
    }
    
    // no message, wait again.
    sem_wait (&pMsgQueue->wait);
    goto checkagain;
    return 1;
}

int	CApp::GetMessage2(PMSG pMsg, HWND hWnd)
{
    PMSGQUEUE pMsgQueue  = &m_MsgQueue;

    memset(pMsg, 0, sizeof(MSG));

checkagain:

    if(pMsgQueue->dwState & QS_QUIT)
    {
	        pMsg->hwnd 		= hWnd;
	        pMsg->message 	= MSG_QUIT;
	        pMsg->wParam 	= 0;
	        pMsg->lParam 	= 0;
	        pMsg->pAdd 		= NULL;
			
	        pMsgQueue->dwState &= ~QS_QUIT;   
	        
	        m_nRunCount = 0;
	        //printf("return break hWnd=%x\n", &pMsg->hwnd );     
	        return 0;
    }
    if(pMsgQueue->dwState & QS_POSTMSG)
    {
        pthread_mutex_lock(&pMsgQueue->lock);
        if(pMsgQueue->readpos != pMsgQueue->writepos)
        {            
            *pMsg 		= pMsgQueue->msg[pMsgQueue->readpos];            
            //pMsg->pAdd 	= NULL;
            pMsgQueue->readpos++;
            if (pMsgQueue->readpos >= pMsgQueue->len)
            	pMsgQueue->readpos = 0;
            pthread_mutex_unlock (&pMsgQueue->lock);

            return 0x00000001;
        }
        else
            pMsgQueue->dwState &= ~QS_POSTMSG;

        pthread_mutex_unlock (&pMsgQueue->lock);
    }
    
    // no message, wait again.
    sem_wait (&pMsgQueue->wait);
    goto checkagain;
    return 1;
}

int	CApp::PostMessage(HWND hWnd, int iMsg, WPARAM wParam, LPARAM lParam)
{
    PMSGQUEUE pMsgQueue;
    PMSG pMsg;
    int sem_value;
  
    pMsgQueue	= &m_MsgQueue;
//    if( !(pMsgQueue = GetMsgQueue(hWnd)) ) return ERR_INV_HWND;

    pthread_mutex_lock (&pMsgQueue->lock);
   
    if((pMsgQueue->writepos + 1) % pMsgQueue->len == pMsgQueue->readpos)
    {
        pthread_mutex_unlock (&pMsgQueue->lock);
//        //printf("CApp::PostMessage message Queue FULL\n");
        return ERR_QUEUE_FULL;
    }
    
    // Write the data and advance write pointer
    pMsg 	= &(pMsgQueue->msg[pMsgQueue->writepos]);
    pMsg->hwnd 		= hWnd;
    pMsg->message 	= iMsg;
    pMsg->wParam 	= wParam;
    pMsg->lParam 	= lParam;

    pMsgQueue->writepos++;
    if(pMsgQueue->writepos >= pMsgQueue->len) 
    	pMsgQueue->writepos = 0;

    pMsgQueue->dwState 	   |= QS_POSTMSG;

    pthread_mutex_unlock (&pMsgQueue->lock);
    // Signal that the msg queue contains one more element for reading
    sem_getvalue (&pMsgQueue->wait, &sem_value);
    if (sem_value == 0)
        sem_post(&pMsgQueue->wait);

    return ERR_OK;
}

int	CApp::PostQuitMessage(HWND hWnd)
{
    PMSGQUEUE pMsgQueue;
    int sem_value;
    
    pMsgQueue	= &m_MsgQueue;

//    if(!(pMsgQueue = GetMsgQueue(hWnd))) 	return ERR_INV_HWND;

    pMsgQueue->dwState |= QS_QUIT;

    // Signal that the msg queue contains one more element for reading
    sem_getvalue (&pMsgQueue->wait, &sem_value);
    if(sem_value == 0)
        sem_post(&pMsgQueue->wait);

    return ERR_OK;
}

BOOL CApp::QueueMessage(PMSG msg)
{
    int 	sem_value;
    int		readpos, writepos;
    MSG		Msg;
    
    PMSGQUEUE	pMsgQueue	= &m_MsgQueue;

    pthread_mutex_lock(&pMsgQueue->lock);

    if((pMsgQueue->writepos + 1) % pMsgQueue->len == pMsgQueue->readpos)
    {
        pthread_mutex_unlock(&pMsgQueue->lock);
		sem_getvalue(&pMsgQueue->wait, &sem_value);
		if(sem_value == 0)
			sem_post(&pMsgQueue->wait);
        return FALSE;
    }
    
    readpos		= pMsgQueue->readpos;
    writepos	= pMsgQueue->writepos;
    
    //=======
    // 檢查是否有相同的消息存在
    //=======
    while(readpos != writepos)
    {            
        Msg 		= pMsgQueue->msg[readpos];
        
        if (Msg.message == msg->message	&& msg->message == MSG_RS232)
        {
        	pthread_mutex_unlock(&pMsgQueue->lock);
	        return FALSE;
        }

        
        if( Msg.message == msg->message	&&
        	Msg.wParam  == msg->wParam	&&
        	Msg.lParam  == msg->lParam	&&
        	Msg.pAdd	  == msg->pAdd)
        {
        	pthread_mutex_unlock(&pMsgQueue->lock);
	        return FALSE;
        }

        readpos++;
        if(readpos >= pMsgQueue->len)
        	readpos = 0;
    }

    // Write the data and advance write pointer
    pMsgQueue->msg[pMsgQueue->writepos] 	= *msg;

    pMsgQueue->writepos++;
    if(pMsgQueue->writepos >= pMsgQueue->len) pMsgQueue->writepos = 0;

    pMsgQueue->dwState |= QS_POSTMSG;

    pthread_mutex_unlock(&pMsgQueue->lock);
   
    // Signal that the msg queue contains one more element for reading
    sem_getvalue(&pMsgQueue->wait, &sem_value);
    if (sem_value == 0)
       sem_post(&pMsgQueue->wait);

    return TRUE;
}

int	CApp::MainProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case MSG_TIMER:
//			//printf("timer Finished\n");
			PostQuitMessage(HTSK_MAIN);	
			return 0;		
			break;
		default:
			break;
	}
	return 1;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	AddCounter(long* plCounter, TASKID TaskID)
{
	int	i;
	
	//首先安裝定時器，不成功直接退出
	if(AddTimer(plCounter) != TIME_SUCCESS)
	{
//		//printf("::AddCounter Add Fail,TASKID=%d\n", TaskID);
		return;
	}
	
//	//printf("::AddCounter,TASKID=%d\n", TaskID);
	
	//=======
	// 安裝成功後，向MSG_TIMER的列表注冊
	// 一個Task隻需要一個MSG_TIMER就能工作，禁止重復的定時器
	//=======	
	for(i = 0; i < MAX_TIMELIST; i++)
	{
		// TASK已經注冊過MSG_TIMER，不需要再次注冊
		if(g_TimerList[i].TaskID == TaskID)
		{
			g_TimerList[i].Counter++;
			return;
		}
	}
	
	// 如果沒有注冊過，則寫入列表
	for(i = 0; i < MAX_TIMELIST; i++)
	{
		if(g_TimerList[i].Counter == 0)
		{
			g_TimerList[i].Counter		= 1;
			g_TimerList[i].TaskID		= TaskID;
			break;
		}
	}
}

void	DeleteCounter(long* plCounter, TASKID TaskID)
{
	int	i;
	
	//首先刪除定時器，不成功直接退出
	if(DeleteTimer(plCounter) != TIME_SUCCESS)
	{
		return;
	}
	
	for(i = 0; i < MAX_TIMELIST; i++)
	{
		if(g_TimerList[i].TaskID == TaskID)
		{
			g_TimerList[i].Counter--;
			
			if(g_TimerList[i].Counter == 0)
				g_TimerList[i].TaskID	= HTSK_INVALID;
			return ;
		}
	}
}

void	RegisterMessage(int message, TASKID TaskID)
{
	int i;
	
	//=======
	// 不允許重復注冊消息
	//=======
	for(i = 0; i < MAX_MSGLIST; i ++)
	{
		if(g_MsgList[i].Message == message && g_MsgList[i].TaskID == TaskID)
		{
			g_MsgList[i].Counter ++;
			return;
		}
	}
	
	for(i = 0; i < MAX_MSGLIST; i ++)
	{
		if(g_MsgList[i].Message == MSG_NULLMSG)
		{
			g_MsgList[i].Message	= message;
			g_MsgList[i].TaskID		= TaskID;
			g_MsgList[i].Counter	= 1;
			break;
		}
	}	
}

void 	UnRegisterMessage(int message, TASKID TaskID)
{
	int i;
	for(i = 0; i < MAX_MSGLIST; i ++)
	{
		if(g_MsgList[i].Message == message && g_MsgList[i].TaskID == TaskID)
		{
			g_MsgList[i].Counter--;
			if(g_MsgList[i].Counter == 0)
			{
				g_MsgList[i].Message	= MSG_NULLMSG;
				g_MsgList[i].TaskID		= HTSK_INVALID;
			}
			break;
		}
	}
}

void 	UnRegisterAllMessage(TASKID TaskID)
{
	int i;
	for(i = 0; i < MAX_MSGLIST; i ++)
	{
		if(g_MsgList[i].TaskID == TaskID)
		{
			g_MsgList[i].Message	= MSG_NULLMSG;
			g_MsgList[i].TaskID		= HTSK_INVALID;
			g_MsgList[i].Counter	= 0;
		}
	}


}

extern int g_FbIotcol;

//	fans	2006/12/6 05:54下午
void	OpenMainForm()
{
	const int DATETIM_INDEX = 0;
	//	fans add
	if (g_ParseMainFrame == NULL)
		g_ParseMainFrame = new CParseObject;
	char	szCmd[256];
	sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), CtmConfig::GetInstance()->GetMainFormName());
				
	//g_ParseMainFrame->OpenFile("res/form/mainform.TXT");	// <FANS> 2006/11/21 10:47上午
	g_ParseMainFrame->OpenFile(szCmd);
	g_ParseMainFrame->CloseFile();

//	usleep(100*1000);
	//ioctl(g_FbIotcol, 0xFFFF11, NULL);
	static int counter=0;
	if(CtmConfig::GetInstance()->GetLogoFlag() == 1&&counter!=0)
	{
		counter=1;
		usleep(40*1000);
	}
	
	g_ParseMainFrame->GetMainControl()->CreateA();
	g_pMainFrame = (CtmMainFrame*)g_ParseMainFrame->GetMainControl();
	g_pMainFrame->Show();
	//getchar();
	usleep(100*1000);
	ioctl(g_FbIotcol, 0xFFFF12, NULL);
	//usleep(100*1000);
	//getchar();
	
	//	fans add
	if (g_pMainFrame->GetPromptBar() != NULL)
	{
		//g_ptaskDateTime->SetDateTimeWindow(g_pMainFrame->GetPromptBar()->GetControlA(DATETIM_INDEX), NULL);
	}
	//getchar();
	AssignCommandTarget(g_pMainFrame);
	
}

BOOL	SendMsg(int nMsg, WPARAM wParam, LPARAM lParam, void* pAdd)
{
	MSG			msg;
	memset(&msg, 0, sizeof(msg));
	msg.message = nMsg;
	msg.wParam	= wParam;
	msg.lParam	= lParam;
	msg.pAdd	= pAdd;
	return g_pApplication->QueueMessage(&msg);
}

//	fans 2009-7-22 19:44:44
BOOL	SendMsgW(int nMsg, WPARAM wParam, LPARAM lParam, void* pAdd)
{
	AUTOCHECK_CLASS(g_ptaskCmd)->GetFocus()->WndProc(nMsg, wParam, lParam);
	return TRUE;
}

