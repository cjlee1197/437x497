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
#include	"tmKeyBoardRobot.h"
#include	"../../utils.h"
#include	"../../tmconfig.h"
#include	"../../device.h"
#include    "../../common.h"
#include	"../../main.h"

#include	"../../tmshmsg.h"
#ifdef	D_CANMASTER
#include	"../../canMaster.h"
#endif

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		KB_DEVNAME			"/dev/tty0"

typedef void * (* PKBRUN)(void *);

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/

    	int			m_nKB;					// 鍵盤設備的文件句柄	
/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/
IMPLEMENT_DYNCREATE(CtmKeyBoardRobot, CtmKeyBoard)
CtmKeyBoardRobot::CtmKeyBoardRobot() : CtmKeyBoard()
{
	
	
}

CtmKeyBoardRobot::~CtmKeyBoardRobot()
{
		
}

int		CtmKeyBoardRobot::CreateSelf()
{
	printf("!!!!!!!!!!CtmKeyBoardRobot::CreateSelf\n");
	CtmKeyBoard::CreateSelf();
		
	m_nKB			= 0;
	Init_KB();
	return 0;
}


int		CtmKeyBoardRobot::FreeSelf()
{
	CtmKeyBoard::FreeSelf();
	Destroy_KB();
	return 0;
}
/*--------------------------------------------------------------------------+
| Function : void Init_KB()                                              	|
| Task     : Init keyboard input                                   			|
+---------------------------------------------------------------------------+
| Call     : Init_KB()                                                		|
|                                                                           |
| Parameter: None                      -                                    |
|                                                                           |
| Return   : None                      -                         	 		|
+--------------------------------------------------------------------------*/

void CtmKeyBoardRobot::Init_KB()
{
	printf("!!!!!!!!!!!!!!!Init_KB\n");
	m_nKB	= open("/dev/panel",O_RDWR);
	
	if(m_nKB < 0)
		printf("open panel 	error\n");
	else
	{
		pthread_create(&m_ScanDownThread, NULL, (PKBRUN)KBRun, this);
	}
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
void	CtmKeyBoardRobot::Destroy_KB()
{
	if(m_ScanDownThread != 0)		pthread_cancel(m_ScanDownThread);
	
	if(m_nKB > 0)
	{
		close(m_nKB);
	}
	m_nKB 		= -1;
}

/*---------------------------------------------------------------------------+
;| Function :                                                                |
;| Task     :                                                                |
;+---------------------------------------------------------------------------+
;| Call     :                                                                |
;|                                                                           |
;| Parameter:                                                                |
;|                                                                           |
;| Return   :                                                                |
;+--------------------------------------------------------------------------*/
void	CtmKeyBoardRobot::PutKeyTail(WORD /*KEYCODE*/ keycode)
{
	MSG				msg;
	KEYCODE			Key;	

	if(m_Queue.bFull == TRUE)
	{		
		return;
	}
	Key.wCode	= keycode;
	m_KeyBuff[m_Queue.wInput++]	= Key;
	
	if(m_Queue.wInput >= m_Queue.wEnd)		m_Queue.wInput	= 0;
	if(m_Queue.wInput == m_Queue.wOutput)	m_Queue.bFull	= TRUE;

	msg.message		= MSG_KEYDOWN;
	msg.wParam		= (WPARAM)Key.wCode;
	msg.lParam		= 0;
	//printf("CtmKeyBoardG MSG_KEYDOWN Key.wCode=%d\n",Key.wCode);
	g_pApplication->QueueMessage(&msg);	
}

void 	KBRun(CtmKeyBoardRobot* pKey)
{
	printf("!!!!!!!!!!!!!!!!!!KBRun\n");
	KEYCODE			Key;
	unsigned short data;
	static WORD		wKeyLastScan = 0;
	static BOOL		bFlag = TRUE;
	while (TRUE)
	{
		int rst = read(m_nKB, &data, 1);
		printf("data = %04x\n",data);
	}
}

