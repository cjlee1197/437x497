/*==========================================================================+
|  Class    : Task                                                          |
|  Task     : Task                                                          |
|---------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                  |
|  Link     : MSVC V1.50 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                     |
|  Version  : V1.00                                                         |
|  Creation : 04/20/1995                                                    |
|  Revision : V1.10                                                         |
|             06/20/1995 by C.C. Chen                                       |
|           : V2.00                                                         |
|             04/27/1996 by C.C. Chen                                       |
|			: V3.00 多線程系統改寫											|
|			  2005/01/19 by netdigger										|
+==========================================================================*/
#include	"task.h"
#include	"main.h"
#include	"utils.h"
#include 	"drawtext.h"
/*==========================================================================+
|           External                                                        |
+==========================================================================*/
extern      tmWNDATTR  		g_wndattrDefault;   // Defined in "ddefault.cpp"
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
//=======
//      Initialize static class member
//=======
WORD        CTask::m_wTasksCount 			= 0;
TASKID      CTask::m_idCurrentTask 			= 0;
CTask*      CTask::m_ptaskList[MAX_TASKS + 1] 	= {NULL};
BOOL        CTask::m_bSystemIdle 			= FALSE;
ImageBackground	g_imgbgDate;
ImageBackground g_imgbgTime;
/*==========================================================================+
|           Class implementation - Task                                     |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
CTask::CTask()
{
	Create(NULL);
}

CTask::CTask(TASK* ptask)
{
	Create(ptask);
}

CTask::~CTask()
{
	m_wTasksCount--;
	m_ptaskList[m_idMe] = NULL;	
}
    
/*--------------------------------------------------------------------------+
|           Operations                                                  	|
+--------------------------------------------------------------------------*/
void        CTask::Create(TASK* ptask)
{
	if (m_wTasksCount >= MAX_TASKS)
    {
	    m_idMe = NULL_TASKID;
	    return;
    }

	m_wTasksCount++;
	m_ptask 	= ptask;
	m_idMe 		= 1;                    	// Find task ID of myself
	while (m_ptaskList[m_idMe] != NULL)
		m_idMe++;
	
	//=======
	// Save this CTask pointer in global list
	//=======
	m_ptaskList[m_idMe] = this;
	//printf("m_idMe = %d  this = %d\n",m_idMe,this);
	m_wStatus 			= TASK_NORMAL;	
}

CTask*      CTask::GetNextTask()
{
	CTask*      ptask;
	
	do
	{
	    if ((++m_idCurrentTask) > MAX_TASKS) m_idCurrentTask = 1;
	}
	while ((ptask = m_ptaskList[m_idCurrentTask]) == NULL);
	
	return ptask;
}

void	CTask::OnIdle()                
{
	RegisterMessage(MSG_KEY, m_idMe);
	m_bSystemIdle = TRUE;
};

void	CTask::OnActive()
{
	UnRegisterMessage(MSG_KEY, m_idMe);
	m_bSystemIdle = FALSE;
};

/*--------------------------------------------------------------------------+
|  Function : void*		TaskRun(void* TaskID)	                          	|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     : 線程專用函數                    								|
|                                                                           |
|  Parameter: TaskID        - taskID	                          			|
|                                                                         	|
|  Return   :               -                   							|
+--------------------------------------------------------------------------*/

/*===========================================================================+
|           Class implementation - CTaskDateTime                             |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|  Class    : CTaskDateTime                                                  |
|  Task     : Show system date and time on status bar                        |
+----------------------------------------------------------------------------+
|  Call     : CTaskDateTime::Run()                                           |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskDateTime::CTaskDateTime() : CTask()
{
	m_pwndDate =
	m_pwndTime = NULL;
	m_lCounter =
	m_nDay     = 0;
	::AddCounter((long *)&m_lCounter, m_idMe);
}

CTaskDateTime::~CTaskDateTime()
{
	g_imgbgDate.FreeImage();
	g_imgbgTime.FreeImage();
	::DeleteCounter((long *)&m_lCounter, m_idMe);  // The destructor may be called after the "main" is exit
                                            // The RTCDSR is removed at that time
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CTaskDateTime::Run()
{
	MSG         msg;
	tmDATE        date;
	tmTIME        time;
	char        sz[11];                     // Maximum character : YYYY/MM/DD

	m_lTaskCnt++;                          //Bw2003/4/2 for RTC_ISR enable
	
	if (g_pApplication->GetMessage(CMD_SYSTEM, &msg))
    {
    switch (msg.wMessage)
        {
        case _SYS_RESETTIME:
        	//printf("TaskDataTime():\n");
            m_lCounter = 0;
            m_nDay = 0;
            m_nMin = 0;             //Weichun 2003/10/13 add for 7HTNew time update
            break;
        default:
            g_pApplication->SetMessage(CMD_SYSTEM, &msg);
            break;
        }
    }

	if (m_lCounter <= 0)                    // The counter will be decremented in RTCISR
    {
    GetTime(&time);                     //St1213'01 move to here for check day
 	//Yang 2007/1/8 m_bSystemIdle	= g_ptaskCmd->IsIdle();
    if (!m_bSystemIdle)
        {
        if (m_pwndDate != NULL)
            {
            GetDate(&date);
            if (m_nMin != (short)time.minute)
                {
                m_nMin = (short)time.minute;
                sprintf(sz, "%04u.%02u.%02u/%02u:%02u", date.year, date.month, date.day, time.hour, time.minute);
                g_imgbgDate.ShowImage( m_pwndDate->GetmClientLeft(), m_pwndDate->GetmClientTop(), 
					m_pwndDate->GetmClientRight(), m_pwndDate->GetmClientBottom());
                m_pwndDate->ShowExtent(0, 1, sz, 0);
                }
            }
        }
    else
        m_nDay = 0;
    m_lCounter = 1024;                  // The RTCISR will be activated every 1/1024 second  

    }
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CTaskDateTime::SetDateTimeWindow(CtmWnd* pwndDate, CtmWnd* pwndTime)
{
	m_pwndDate = pwndDate;
	m_pwndTime = pwndTime;
	
	if (m_pwndDate != NULL)
		g_imgbgDate.CopyImage(m_pwndDate->GetmClientLeft(), m_pwndDate->GetmClientTop(), 
			m_pwndDate->GetmClientRight(), m_pwndDate->GetmClientBottom());
	if (m_pwndTime != NULL)
		g_imgbgTime.CopyImage(m_pwndTime->GetmClientLeft(), m_pwndTime->GetmClientTop(), 
			m_pwndTime->GetmClientRight(), m_pwndTime->GetmClientBottom());
}
/*===========================================================================+
|           Class implementation - CTaskCursor                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|  Class    : CTaskCursor                                                    |
|  Task     : Blink cursor in the specified window                           |
+----------------------------------------------------------------------------+
|  Call     : CTaskCursor::Run()                                             |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskCursor::CTaskCursor(TASK* ptask, TASKCURSOR* ptaskcursor) : CTask(ptask)
{
	m_pwnd = ptaskcursor->pwnd;
	m_rect = m_pwnd->GetClientRECT();
#ifndef  D_I31
	m_rect.left--;                          // Make room for vertical cursor
	m_rect.right++;                         // Make room for vertical cursor
#endif
	m_bOn = FALSE;
	m_bEnable = FALSE;
	m_bWait   = FALSE;                      // St1116'02 for ABC input

	m_bEcho   = FALSE;
	m_szEcho[1] = '\0';
	m_ftSize.width =0;
	m_ftSize.height =0;
	m_nWidth=0;
	m_nHeight=0;

	::AddCounter((long *)&m_lCounter, m_idMe);
}

CTaskCursor::~CTaskCursor()
{
	::DeleteCounter((long *)&m_lCounter, m_idMe);
	if (m_bOn) 
	{
//		printf("%s cancle Cursor!!\n", m_pwnd->Name);
		WriteCursor();
	}

	if (m_bEcho) m_pwnd->ShowExtent(m_nXEcho, m_nYEcho, m_szEcho);
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CTaskCursor::Run()
{
//Yang 2007/1/8 	if(g_ptaskCmd->IsIdle()) return;
	
	if ((m_bEnable) && (m_lCounter <= 0))
    {
	    m_bOn = !m_bOn;
	    WriteCursor();
	    m_bWait = FALSE;                    // St1116'02 for ABC input
    	if ((m_bEcho) &&(!m_bOn))
      {
	      m_pwnd->ShowExtent(m_nXEcho, m_nYEcho, m_szEcho);
	      m_bEcho   = FALSE;
      }
			ChangePosNow(m_rect);
    	m_lCounter = 512;                   // The RTCISR will be activated every 1/1024 second
    }
}

void	CTaskCursor::SetCursorPosition(char* szText,int nLen,char cKey,SIZE ftSize,CRect rect,WORD wAlign,int offset_bytes)
{
	m_ftSize.width=ftSize.width;
	m_ftSize.height=ftSize.height;
	if(g_pDrawText)
	{
		m_fgc = m_pwnd->GetFGColor();
		m_bgc = m_pwnd->GetBGColor();
//		m_nWidth = (g_wndattrDefault.wWidthCursor > ((ftSize.width>>1)+1)) ? 
//						((ftSize.width>>1)+1) : g_wndattrDefault.wWidthCursor;
		if(cKey!=0)
		{
			m_nWidth=g_pDrawText->GetFontWidth((unsigned char*)&cKey,ftSize);
		}
		else
		{
			m_nWidth=g_pDrawText->GetFontWidth((unsigned char*)szText,ftSize);	
		}
		m_nWidth = (g_wndattrDefault.wWidthCursor > m_nWidth) ? 
						m_nWidth : g_wndattrDefault.wWidthCursor;
		m_nHeight=ftSize.height;
		if (m_bOn)
    	{
	    	m_bOn = FALSE;
	   	 	WriteCursor();
    	}
    	CPoint pt=g_pDrawText->GetCharPosition((unsigned char*)szText,nLen,(unsigned char)cKey,ftSize,rect,wAlign,offset_bytes);
		m_nX = pt.x()+1;
		m_nY = pt.y()+1;
		m_lCounter = 0;	
	}
		
}

void        CTaskCursor::SetCursorPosition(short nX, short nY)
{	
	m_cf  = m_pwnd->GetFont();
	m_fgc = m_pwnd->GetFGColor();
	m_bgc = m_pwnd->GetBGColor();

	m_nHeight=m_cf->m_wHeight;
	m_nWidth = (g_wndattrDefault.wWidthCursor > ((m_cf->m_wWidth>>1)+1)) ? 
					((m_cf->m_wWidth>>1)+1) : g_wndattrDefault.wWidthCursor;	//netdigger 20050214
	if (m_bOn)
    {
	    m_bOn = FALSE;
	    WriteCursor();
    }

	m_nX = nX;
	m_nY = nY;
	m_lCounter = 0;
}

void        CTaskCursor::DisplayCursor(BOOL bOn)
{
	if ((!bOn) && (m_bOn))
    {
	    m_bOn = FALSE;
	    WriteCursor();
    }
	
	m_bEnable = bOn;
	if (m_bWait)                            // St1116'02 for ABC input
    	m_lCounter = 1024;
	else
		m_lCounter = 0;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void __near CTaskCursor::WriteCursor()
{

	m_rect = m_pwnd->GetClientRECT();
#ifndef  D_I31
	m_rect.left--;                          // Make room for vertical cursor
	m_rect.right++;                         // Make room for vertical cursor
#endif

	short       nOldWriteMode;
	
	nOldWriteMode = _SetWriteMode(_GXOR);
	_SetViewPort(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	_SetColor(m_bgc);
	for (short i=0; i<m_nWidth; i++)
    {

	    _MoveTo(m_nX+i, m_nY);
	    _LineTo(m_nX+i, m_nY+m_nHeight-1);
    }
  RECT m_rect_cursor;
  MKRECT(m_rect_cursor,m_rect.left+m_nX,m_rect.top+m_nY,m_rect.left+m_nX+m_nWidth+1,m_rect.top+m_nY+m_nHeight);
	ChangePosNow(m_rect_cursor);
	_SetWriteMode(nOldWriteMode);
}

void       CTaskCursor::SetEcho(short nX, short nY, char cEcho)
{
	m_szEcho[0] = cEcho;
	if ((m_bEcho) && (m_nXEcho < nX)) m_pwnd->ShowExtent(m_nXEcho, m_nYEcho, m_szEcho);
	m_bEcho   = TRUE;
	m_nXEcho  = nX;
	m_nYEcho  = nY;
}

/*===========================================================================+
|           Class implementation - CTaskAnimator                             |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|  Class    : CTaskAnimator                                                  |
|  Task     : Show animator                                                  |
+----------------------------------------------------------------------------+
|  Call     : CTaskAnimator::Run()                                           |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskAnimator::CTaskAnimator(TASK* ptask, TASKANIMATOR* ptaskanimator) : CTask(ptask)
{
	m_pwnd = ptaskanimator->pwnd;
	m_nSlice = ptaskanimator->nSlice;
	m_lCounter = 0;
	::AddCounter((long *)&m_lCounter, m_idMe);
}

CTaskAnimator::~CTaskAnimator()
{
	::DeleteCounter((long *)&m_lCounter, m_idMe);
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CTaskAnimator::Run()
{
if (m_lCounter <= 0)                    // The counter will be decremented in RTCISR
    {
    if (!m_bSystemIdle) m_pwnd->Show();
    m_lCounter = (long)m_nSlice;        // Preset the counter
    }
}
