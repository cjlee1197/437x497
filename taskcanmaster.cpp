/*===========================================================================+
|  Class    : TaskCANMaster                                                  |
|  Task     :													             |
|----------------------------------------------------------------------------|
|  Compile  : Gcc 4.3    -                                                   |
|  Link     : Gcc 4.3    -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 2013/04/08                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include    "utils.h"
#include    "taskcanmaster.h"
#include    "main.h"
#include	"_kb.h"
#include	"tmdebug.h"
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<unistd.h>
#include 	<string.h>
#include 	<signal.h>
#include 	<libgen.h>
#define 	__USE_GNU
#include 	<fcntl.h>
#include 	<getopt.h>
#include 	<pthread.h>
#include 	<signal.h>
#include 	<sched.h>
#include 	<sys/types.h>
#include 	<sys/socket.h>
#include 	<sys/ioctl.h>
#include 	<sys/uio.h>
#include 	<linux/can.h>
#include 	<linux/can/raw.h>
#include 	<net/if.h>
#include 	<linux/unistd.h>
#include 	<time.h>
#include	"canMaster.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
IMPLEMENT_DYNCREATE(CTaskCanMaster, CtmPackClass)

#define 		PF_CAN 							29
CTaskCanMaster*          g_ptaskCanMaster		= NULL;

//DWORD	g_dwM3Key[MAX_KEYINDEX] = {0};
//DWORD	g_dwM3LED[MAX_LEDINDEX] = {0};
//DWORD	g_dwM3LEDOld[MAX_LEDINDEX] = {0};

/*===========================================================================+
|           Class implementation - TaskMoni                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskCanMaster::CTaskCanMaster() : CTask()
{
	m_nskt = -1;
}

CTaskCanMaster::~CTaskCanMaster()
{

}

    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
int		CTaskCanMaster::OpenMaster()
{
	//m_nskt = TaskCanMaster();
	
	//struct can_frame frame;
	//
	//memset(frame.data, 0, sizeof(frame.data));
	//frame.can_id		= 0;
	//frame.data[0]		= 1;
	//frame.data[1]		= 6;
	//frame.can_dlc 		= 2;
	//printf("OpenMaster m_nskt=%d \n", m_nskt);
	//printf("processTransmitNMT--->frame.can_id:%d data:%x %x\n", frame.can_id, frame.data[0], frame.data[1]);
	//write(m_nskt, &frame, sizeof(frame));	
	//Can_Write(&frame, sizeof(frame));
	return  -1;	
}

int		CTaskCanMaster::CloseMaster()
{
	return  -1;	
}
	
void	CTaskCanMaster::ReSetConfig()
{
	;	
}
	
void	CTaskCanMaster::Process_NMT(WORD wNodeID, WORD wNMTCS)
{
	;
}

void	CTaskCanMaster::Process_PDO(WORD wNodeID, WORD wPDONum)
{
	;	
}
	
void	CTaskCanMaster::Process_SDORead(WORD wNodeID,  WORD wIndex, BYTE cSubIndex)
{
	;	
}

void	CTaskCanMaster::Process_SDOWrite(WORD wNodeID, WORD wIndex, BYTE cSubIndex, DWORD dwValue, WORD wSize)
{
	;	
}

void	CTaskCanMaster::Can_Read()
{
	;	
}

void	CTaskCanMaster::CanInitConfig()
{
	;	
}