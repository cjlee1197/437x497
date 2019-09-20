/*===========================================================================+
|  Class    : Application                                                    |
|  Task     : Application Processor header file                              |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 04/02/1995                                                     |
|  Revision : 04/24/1996                                                     |
+===========================================================================*/

#ifndef     D_APP
#define     D_APP

#include    "wnd.h"
#include    "task.h"
#include	"timer.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     CMD_MAX_CHANNEL     4
#define     CMD_MAX_MESSAGE     8

#define     CMD_SYSTEM          0x0001
#define     CMD_KEY             0x0002
#define     CMD_232             0x0003

#define     _SYS_COMMAND        0x0001
#define     _SYS_CHANGE         0x0002
#define     _SYS_UPDATE         0x0003
#define     _SYS_PUSHFOCUS      0x0011
#define     _SYS_POPFOCUS       0x0012
#define     _SYS_SETFOCUS       0x0013
#define     _SYS_KILLFOCUS      0x0014

#define     _SYS_COMMAND_VIEW2  0x0015
#define     _SYS_PUSHFOCUS2     0x0016
#define     _SYS_POPFOCUS2      0x0017
#define     _SYS_SETFOCUS2      0x0018
#define     _SYS_KILLFOCUS2     0x0019

#define     _SYS_RESETTIME      0x0101
#define     _SYS_NEWSESSION     0x8001
#define     _SYS_WAIT           0x8002
#define     _SYS_RESET          0xFFFD
#define     _SYS_SHELL          0xFFFE
#define     _SYS_EXIT           0xFFFF

#define     _KEY_GETBYPASS      0x0001
#define     _KEY_SETBYPASS      0x0002
#define     _KEY_ADDHEAD        0x0003
#define     _KEY_ADDTAIL        0x0004

#define 	QS_NOTIFYMSG        0x10000000
#define 	QS_SYNCMSG        	0x20000000
#define 	QS_POSTMSG          0x40000000
#define 	QS_QUIT             0x80000000
#define 	QS_INPUT            0x01000000
#define 	QS_PAINT            0x02000000
#define		QS_NET				0x04000000
#define		QS_COM				0x08000000
#define 	QS_TIMER            0x0000FFFF
#define 	QS_EMPTY            0x00000000


#define 	DEF_MSGQUEUE_LEN    16

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

typedef struct tagMSG
{
	// the handle to the window which receives this message.
    HWND            hwnd;
    // the handle to the task which receives this message.
//    TASKID			TaskID;
	// the message identifier. 
    int             message;
    // The first parameter of the message (32-bit integer).
    WPARAM          wParam;
    // The second parameter of the message (32-bit integer). 
    LPARAM          lParam;
    struct timeval  time;
    void*           pAdd;
    //=======
    // 兼容原386代碼
    //=======
    
    WORD    wMessage;
    union
    {
        DWORD   dwParam;
        CtmWnd* pwnd;
        WORD    wKey;
        WORD    wCmd;
        WORD    wIDControl;
        long    lCounter;
        BOOL    bBypass;
    } ;

} MSG;

typedef	struct tagMSGQUEUE
{
    DWORD dwState;              	// message queue states
    pthread_mutex_t lock;       	// lock
    sem_t wait;                 	// wait semaphores

    MSG* msg;                   	// post message buffer
    int len;                    	// buffer len
    int readpos, writepos;      	// positions for reading and writing
    
    //=======
    // 兼容原先的386程序
    //=======
    WORD    wInput;
    WORD    wOutput;
    WORD    wQueueEnd;
    BOOL    bQueueFull;
    MSG     msgQueue[CMD_MAX_MESSAGE];
    
}	MSGQUEUE;

typedef MSG* 		PMSG;
typedef MSGQUEUE* 	PMSGQUEUE;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - Application                                  |
+===========================================================================*/
class CApp
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CApp();
    virtual ~CApp();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
	static CTask*   m_ptaskCurrent;
	static	MSGQUEUE		m_MsgQueue;
	static	MSGQUEUE 		m_msg[CMD_MAX_CHANNEL];
	static	BOOL			m_bReset;
	int		m_nRunCount;
	
	int		MainProc(int message, WPARAM wParam, LPARAM lParam);
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:    
    void    Run();
    void    Run2();
    
    BOOL    SetMessage(WORD wChannel, MSG* pmsg);
    BOOL    GetMessage(WORD wChannel, MSG* pmsg);
    BOOL    GetMessage2(WORD wChannel, MSG* pmsg);
    
    BOOL 	InitMsgQueue(PMSGQUEUE pMsgQueue, int iBufferLen);
    void 	DestroyMsgQueue (PMSGQUEUE pMsgQueue);
    int		GetMessage(PMSG pMsg, HWND hWnd);
    int		GetMessage2(PMSG pMsg, HWND hWnd);
    int		PostMessage(HWND hWnd, int iMsg, WPARAM wParam, LPARAM lParam);
    int		PostQuitMessage(HWND hWnd);
    BOOL	QueueMessage(PMSG msg);
};

void	AddCounter(long* plCounter, TASKID TaskID);
void	DeleteCounter(long* plCounter, TASKID TaskID);
void	RegisterMessage(int message, TASKID TaskID);
void 	UnRegisterMessage(int message, TASKID TaskID);
void 	UnRegisterAllMessage(TASKID TaskID);

void	OpenMainForm();//	fans	2006/12/6 05:54下午


//	in fact, this function is Post, it needn't revcive dealed result;
BOOL	SendMsg(int nMsg, WPARAM wParam, LPARAM lParam, void* pAdd);

//	fans 2009-7-22 19:35:29
//	int fact, this function is Send, it must wait for the taskCMD dealed result;
BOOL	SendMsgW(int nMsg, WPARAM wParam, LPARAM lParam, void* pAdd);


#endif
