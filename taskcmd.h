/*===========================================================================+
|  Class    : TaskCmd                                                        |
|  Task     : Command Service Routine header file                            |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 05/07/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKCMD
#define     D_TASKCMD

#include    "wnd.h"
#include    "task.h"
#include	"tmpackclass.h"
#include    "taskdemo.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     MAX_WINDOWS_FOCUS   8      // MAX_WINDOWS
#define		CMD_COUNTER		2000

//	fans add 2007/11/19 02:42¤U¤È for backup sram
const	long	CNT_LONG_BACKUP_SRAM_TIME	= 1000 * 1800;	//	backup sram with per 30'
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskCmd                                      |
+===========================================================================*/
class CTaskCmd : public CTask, public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskCmd)	
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskCmd();
    ~CTaskCmd();

/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
protected:
	static	WORD    m_wTasksCount;
	static	BOOL    m_bIdle;            // Multiple CTaskCmd share the same flag
	static	long    m_lCounter;         // Counter for idle
	static	long	m_lBackCounter;		// counter for the backup time
    static	WORD    m_wSP;
	static	WORD    m_wSP2;
    static	CtmWnd*	m_pwndStack[MAX_WINDOWS_FOCUS];
    static	CtmWnd*	m_pwndStack2[MAX_WINDOWS_FOCUS];
    static	CtmWnd*	m_pwndFocus;
    static	CtmWnd*	m_pwndFocus2;
			long	m_lTestCounter;
            BOOL    m_bBypass;          // By pass the key code preprocess
            BOOL    m_bHexadecimal;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    //void    Run();
    void    OnChange(WORD wIDControl);
    CtmWnd*	GetFocus()				{return m_pwndFocus;};
    CtmWnd*	GetFocus2()				{return m_pwndFocus2;};
    void    PushFocus();
    void    PushFocus2();
    void    PopFocus();
    void    PopFocus2();
    void    SetFocus (CtmWnd* pwnd);
    void    SetFocus2 (CtmWnd* pwnd);
    void    KillFocus(CtmWnd* pwnd);
    void    KillFocus2(CtmWnd* pwnd);
    BOOL    IsIdle()                    {return m_bIdle;};
    BOOL	SetIdle(BOOL bIdle);
    
    BOOL    GetBypass()                 {return m_bBypass;};
    void    SetBypass(BOOL bBypass)     {m_bBypass = bBypass;};
    WORD	GetTaskCmdCount()			{ return m_wTasksCount; };
	static	BOOL	BackUpSram();	//	fans add 2007/11/19 02:42¤U¤È for backup sram
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	
	
    void  	Push(CtmWnd* pwnd);
    CtmWnd* Pop();
    void  	Push2(CtmWnd* pwnd);
    CtmWnd* Pop2();
};

extern	CTaskDemo*  g_ptaskdemo;

#endif

