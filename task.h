/*===========================================================================+
|  Class    : Task                                                           |
|  Task     : Task header file                                               |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/20/1995                                                     |
|  Revision : V1.10                                                          |
|             06/20/1995 by C.C. Chen                                        |
|           : V2.00                                                          |
|             04/27/1996 by C.C. Chen                                        |
+===========================================================================*/
#ifndef     D_TASK
#define     D_TASK

#include	"common.h"
#include	"wnd.h"
#include 	<guilite/IGui>
#include	"fontcache.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     NULL_TASKID         0

#define     TASK_NORMAL         0
#define     TASK_IDLE           1
#define     TASK_WAITIO         2
#define     TASK_SUSPEND        3

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef     DWORD                TASKID;

typedef struct tagTASK
{
    void*   pParam;
}	TASK;

typedef struct tagTASKCURSOR
    {
    CtmWnd*   pwnd;
    } TASKCURSOR;

typedef struct tagTASKANIMATOR
    {
    CtmWnd*   pwnd;
    short   nSlice;
    } TASKANIMATOR;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - CTask                                        |
+===========================================================================*/
class CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTask();
    CTask(TASK* ptask);
    virtual ~CTask();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    static WORD     m_wTasksCount;
    static TASKID   m_idCurrentTask;

protected:
    static BOOL     m_bSystemIdle;
    static void*	TaskRun(void*	TaskID);
    static CTask*   m_ptaskList[MAX_TASKS+1];
    
    TASK*           m_ptask;
    TASKID          m_idMe;
    WORD            m_wStatus;
        
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    static CTask*	GetTask(TASKID id)	{return m_ptaskList[id];};
    void            Create(TASK* ptask);
    virtual void    Run()                   {}; // Must be virtual
    
    void			OnIdle();
    void			OnActive();
    
    CTask*          GetNextTask();
    WORD            GetStatus()             {return m_wStatus;};
    void            SetStatus(WORD wStatus) {m_wStatus = wStatus;};
    //	fans add
    virtual void	WndProc(int message, WPARAM wParam, LPARAM lParam) { Run(); };
};

typedef		CTask*	PCTASK;

/*===========================================================================+
|           Class declaration - CTaskDateTime                                |
+===========================================================================*/
class CTaskDateTime : public CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskDateTime();
    ~CTaskDateTime();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    CtmWnd*   m_pwndDate;
    CtmWnd*   m_pwndTime;

    long    m_lCounter;
    long    m_lOldCntr;                  //Bw2003/4/2 for RTC_ISR enable
    long    m_lTaskCnt;                  //Bw2003/4/2 for RTC_ISR enable
    short   m_nDay;
    short   m_nMin;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Run();
    void    SetDateTimeWindow(CtmWnd* pwndDate, CtmWnd* pwndTime);
            
    void    newtime() {m_nMin = 0;};
};

/*===========================================================================+
|           Class declaration - CTaskCursor                                  |
+===========================================================================*/


class CTaskCursor : public CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskCursor(TASK* ptask, TASKCURSOR* ptaskcursor);
    ~CTaskCursor();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    CtmWnd*   m_pwnd;
    RECT    m_rect;
    CtmFont*	m_cf;
    SIZE 	m_ftSize;
    FGC     m_fgc;
    BGC     m_bgc;
    short   m_nWidth;
    short 	m_nHeight;
    short   m_nX;
    short   m_nY;
    BOOL    m_bOn;
    BOOL    m_bEnable;
    BOOL    m_bWait;                         //St1116'02 for ABC input
    BOOL    m_bEcho;                         //St1116'02 for ABC input
    short   m_nXEcho;
    short   m_nYEcho;
    char    m_szEcho[2];
    long    m_lCounter;

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Run();
    void    SetCursorPosition(short nX, short nY);
    void	SetCursorPosition(char* szText,int nLen,char cKey,SIZE ftSize,CRect rect,WORD wAlign,int offset_bytes=0);
    void    DisplayCursor(BOOL bOn);
    void    SetWaitStatus(BOOL bCurWait)         {m_bWait = bCurWait;}; //St1116'02 for ABC input
    BOOL    IsWaiting()                          {return m_bWait;};  //St1116'02 for ABC input
    void    SetEcho(short nX, short nY, char cEcho);
	void	SetOnCursor(BOOL bStatus) {m_bOn = bStatus;};
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
    void    __near WriteCursor();
};

/*===========================================================================+
|           Class declaration - CTaskAnimator                                |
+===========================================================================*/
class CTaskAnimator : public CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskAnimator(TASK* ptask, TASKANIMATOR* ptaskanimator);
    ~CTaskAnimator();
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    CtmWnd*   m_pwnd;
    short   m_nSlice;
    long    m_lCounter;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Run();
};

#endif
