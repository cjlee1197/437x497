/*
 *  Name      : tmTaskPickerComm.cpp
 *  Funtion   : 籔诀瘪も㎝礶秈︽ユがㄧ计
 *  Created on: 2015-9-2
 *      Author: @Wang.z.w
 */

#include    "tmTaskPickerComm.h"

#include	"../../app.h"
#include	"../../main.h"
#include	"../../utils.h"
#include  "../../tmshmsg.h"
#include	"../libControl.h"
#include	"../../commonaction.h"
#include  "./tmHmi485cmd.cpp"


#define		PICKER_TIMEOUT		5000
#define   PICKER_SEND_TIME  100

static int m_nSendCount =0;

long	    lTimer1 = PICKER_TIMEOUT;
CtmTaskPickerComm*	CtmTaskPickerComm::m_pSelf=NULL;

typedef void * (* PRSRUN)(void *);
static	QUEUE * 					queues[ZQUEUE_MAX_SIZE];   						
unsigned	short   CQueue::m_id=0;

bool flag_axis_move_cmd = false;
	
CQueue::CQueue()
{
	InitQueue();
}

CQueue::~CQueue()
{
	for(int i=0;i<=m_id;i++)
	{
		DestroyQueue(i);
	}
}

void	CQueue::InitQueue()
{
	m_queue=NULL;
}

int		CQueue::CreateQueue( int nSize)
{
	int nIndex=0;
	if(nSize<=0)
		nSize=DFL_ZQUEUE_SIZE;
	do{
		for( nIndex=0; nIndex<ZQUEUE_MAX_SIZE; ++nIndex)
			if(queues[nIndex]==NULL)
				break;
	}while(0);
	if(nIndex>=ZQUEUE_MAX_SIZE)	return ERR_CREATE_QUEUE;
	m_queue=new QUEUE;
	m_queue->m_buf=new char[nSize];
	if(m_queue->m_buf==NULL)
	{
		delete m_queue;
		m_queue=NULL;
		return -ERR_CREATE_QUEUE;	
	}
	pthread_mutex_init(&m_queue->qlock,NULL);
	pthread_mutex_lock(&m_queue->qlock);
	
	m_queue->m_id=AddID();
	m_queue->m_size=nSize;
	queues[nIndex]=m_queue;
	m_queue->m_front=
	m_queue->m_rear=0;
	
	pthread_mutex_unlock(&m_queue->qlock);
	return m_queue->m_id;
}

int	CQueue::DestroyQueue(int nID)
{
	int nIndex=GetQueueIndex(nID);
	if(nIndex>=0)
	{
		delete[]queues[nIndex]->m_buf;
		queues[nIndex]->m_buf=NULL;
		pthread_mutex_destroy(&queues[nIndex]->qlock);
		delete queues[nIndex];
		queues[nIndex]=NULL;
		return OPRTATOR_SUCCESS;		
	}
	return  ERR_NO_EXIST_QUEUE;
}

int	CQueue::GetQueueIndex(int nID)
{
	int nIndex;
	if(nID<0)	return -1;
	for(nIndex=0; nIndex<ZQUEUE_MAX_SIZE; ++nIndex)
	{
		if(queues[nIndex]!=NULL&&queues[nIndex]->m_id==nID)
			break;	
	}
	if(nIndex>=ZQUEUE_MAX_SIZE)	return ERR_NO_EXIST_QUEUE;
	return 	nIndex;

}

QUEUE*	CQueue::GetQueue(int nID)
{	
	int nIndex;
	if(nID<0)	return NULL;
	for(nIndex=0;nIndex<ZQUEUE_MAX_SIZE;++nIndex)
	{
		if(queues[nIndex]!=NULL&&queues[nIndex]->m_id==nID)
			break;	
	}
	if(nIndex>=ZQUEUE_MAX_SIZE)	return NULL;
	return queues[nIndex];
}

int		CQueue::EnQueue(int nID,void* pData, int nCount)
{
	QUEUE* queue=GetQueue(nID);
	if(nCount<=0||queue==NULL)		return ERR_NO_EXIST_QUEUE;
	pthread_mutex_lock(&queue->qlock);
	int nEnSpaceCount=EnQueueSpace(queue);
	if(nEnSpaceCount<=0)		{	pthread_mutex_unlock(&queue->qlock); 	return FULL_QUEUE; 	}
	if(nCount>nEnSpaceCount)	{	pthread_mutex_unlock(&queue->qlock);	return OPRTATOR_FAILURE;	}
	if(queue->m_rear==queue->m_size)
		queue->m_rear=0;
	if(queue->m_rear+nCount<=queue->m_size)
	{
		memcpy(&queue->m_buf[queue->m_rear],pData,nCount);
		if(queue->m_rear+nCount<=queue->m_size)
			queue->m_rear+=nCount;
	
	}	
	else
	{
		memcpy(&queue->m_buf[queue->m_rear],pData,queue->m_size-queue->m_rear);
		int remain=(queue->m_rear+nCount)%queue->m_size;
		memcpy(queue->m_buf,(char*)pData+queue->m_size-queue->m_rear,remain);
		queue->m_rear=remain;
			
	}
	queue->m_counter+=nCount;
	pthread_mutex_unlock(&queue->qlock);
	return nCount;
}

int		CQueue::DeQueue(int nID,void* pData, int nCount)
{
		QUEUE* queue=GetQueue(nID);
		if(nCount<0||queue==NULL)	return ERR_NO_EXIST_QUEUE;
		pthread_mutex_lock(&queue->qlock);
		int nDeSpaceCount=DeQueueSpace(queue);
		if(nDeSpaceCount<=0)		{	pthread_mutex_unlock(&queue->qlock);	return EMPTY_QUEUE;	}
		if(nCount>nDeSpaceCount)	{	pthread_mutex_unlock(&queue->qlock);	return OPRTATOR_FAILURE;	}
		if(queue->m_front==queue->m_size)
			queue->m_front=0;
		if(queue->m_front+nCount<=queue->m_size)
		{
			memcpy(pData,&queue->m_buf[queue->m_front],nCount);
			if(queue->m_front+nCount<=queue->m_size)
				queue->m_front+=nCount;	
		}
		else
		{
			memcpy(pData,&queue->m_buf[queue->m_front],queue->m_size-queue->m_front);
			int remain=(nCount+queue->m_front)%queue->m_size;
			memcpy((char*)pData+queue->m_size-queue->m_front,queue->m_buf,remain);	
			queue->m_front=remain;
		}
		queue->m_counter-=nCount;
		pthread_mutex_unlock(&queue->qlock);
		return nCount;
}

BOOL	CQueue::IsEmptyQueue(int nID)
{
	
	QUEUE* queue=GetQueue(nID);
	if(queue ==NULL)	return 0;
	return queue->m_counter ? 0 : 1;
}

BOOL	CQueue::IsFullQueue(int nID)
{
	QUEUE* queue=GetQueue(nID);
	if(queue==NULL)	return 0;
	return (queue->m_size==queue->m_counter);
}

int		CQueue::QueueSize(int nID)
{
	QUEUE* queue=GetQueue(nID);
	if(queue==NULL)	return ERR_NO_EXIST_QUEUE;
	return queue->m_counter;
}

int		CQueue::EnQueueSpace(QUEUE* queue)
{
	return queue->m_rear<queue->m_front? (queue->m_front-queue->m_rear) : (queue->m_size-(queue->m_rear-queue->m_front));
}

int		CQueue::DeQueueSpace(QUEUE* queue)
{
	return queue->m_front<=queue->m_rear?(queue->m_rear-queue->m_front) : (queue->m_size-(queue->m_front-queue->m_rear));	
}

BOOL	CQueue::GetQueueValue(int nID,void* pData, int nSize,int nPos)
{
		QUEUE* queue=GetQueue(nID);
		if(queue==NULL)	return 0;
		if((nPos+nSize)>queue->m_size)
		{
			nPos=0;
		}
		memcpy(pData,queue->m_buf+nPos,nSize);
		return nPos+nSize;
}
int		CQueue::GetCurrentFrontPos(int nID)
{
		QUEUE* queue=GetQueue(nID);
		if(queue==NULL)		return -1;
		return queue->m_front;			
}
int		CQueue::GetCurrentRearPos(int nID)
{
		QUEUE* queue=GetQueue(nID);
		if(queue==NULL)		return -1;
		return queue->m_rear;		
}

void	CQueue::ClearQueue(int nID)
{
	QUEUE* queue=GetQueue(nID);
	if(queue==NULL)		return ;
	 queue->m_front=
	 queue->m_rear=0;
}

IMPLEMENT_DYNCREATE(CtmTaskPickerComm, CTaskAmmeter)

CtmTaskPickerComm::CtmTaskPickerComm():CTaskAmmeter()
{
	init();
	
	memset(&m_picker, 0, sizeof(m_picker));
	
	m_picker.newtermios.c_cflag 		= CLOCAL | CREAD;
	m_picker.newtermios.c_cflag		    &= ~CSIZE;
	m_picker.newtermios.c_cflag      	|= CS8;
	m_picker.newtermios.c_cflag      	&= ~CSTOPB;
	// 喷
//	m_picker.newtermios.c_iflag 		= INPCK; 
//	m_picker.newtermios.c_cflag      	|= PARENB;
//  m_picker.newtermios.c_cflag      	&= ~PARODD;
	// 案喷
// 	m_picker.newtermios.c_iflag 		|= INPCK; 
//	m_picker.newtermios.c_cflag         &= ~PARENB;
	//
	m_picker.newtermios.c_iflag 		&= ~INPCK; 
	m_picker.newtermios.c_cflag         &= ~PARENB;
	
	m_picker.newtermios.c_oflag 		= 0;
	m_picker.newtermios.c_lflag 		= 0;
	m_picker.newtermios.c_cc[VTIME] 	= 1;//5;	//500ms
	m_picker.newtermios.c_cc[VMIN] 	    = 0;
	m_picker.wChannel 				    = _COM2;
	m_picker.nBaudRate 			        = B115200;
}

CtmTaskPickerComm::~CtmTaskPickerComm()
{
}

void    CtmTaskPickerComm::init()
{
	m_cmd_code  = 0x00;
	m_stat_code = 0x00;
	
	m_mouldNum			= 0;
	m_nErrCount			= 0;	
	m_nCount_Cmd_Data	= 0;
	m_nCount_Stat_Frame	= 0;
	m_nCount_Stat_Data	= 0;
	m_snedcmd_len		= 0;				

	m_flag_start		     = false; 
	m_flag_recv_ok       = false;
	m_flag_recv_file_data = false;
	m_flag_send_file_cmd = false;
	m_flag_sending_file_cmd = false;
	m_flag_set_mould_num_cmd = false;
	
	memset(m_sendbuff, 	  0, MAX_SIZE);
	memset(m_readbuff, 	  0, MAX_SIZE);
	memset(m_parsebuff,   0, MAX_SIZE);
	memset(m_sendcmdbuff,  0, MAX_SIZE);

	memset(m_err_stat,    0, ERR_STAT_SIZE);
	memset(&m_sys_stat,   0, SYS_STAT_SIZE);
	memset(&m_file_name,  0, FILE_NAME_SIZE);
	memset(&m_cmd_data,   0, STAT_CMD_SIZE);

}

// cmd : 挡篶砰㏑Α
void 	CtmTaskPickerComm::hmiIssueCommand(int nType, int hmiCmdType)
{
	//printf("IssueCommand(): hmiCmdType =%d...\n", hmiCmdType);
	char state = 0x00;
	HMI_485_FILE_CMD fileCmd;
	memset(&fileCmd, 0, FILE_CMD_SIZE);
	HMI_485_CMD sysCmd;
	memset(&sysCmd, 0 , STAT_CMD_SIZE);
	
	if(nType == FILE_CMD)
	{
			fileCmd = hmiFileCmd(4);
			Write(FILE_CMD, &fileCmd, FILE_CMD_SIZE);	
	}
	else
	{
		nType = MOT_CMD;
		switch(hmiCmdType)
		{
			case HMI_485_SYS_IDLE_TYPE:
				nType = IDLE_CMD;
				break;
			case HMI_485_SYS_RESET_TYPE:
				nType = RESET_CMD;
				sysCmd = hmiSysReset(state);
				break;
			case HMI_485_SYS_ABORT_TYPE:
				sysCmd = hmiSysAbort(state);
				break;
			case HMI_485_SET_MODE_TYPE:
				sysCmd = hmiSetMode(1);
				break;
			case HMI_485_SET_HOME_TYPE:
				sysCmd = hmiSetHome(2);
				break;
			case HMI_485_AXIS_POS_MOVE_TYPE:
				sysCmd = hmiAxisPosMove(1, 1, 1);
				break;
			case HMI_485_AXIS_NEG_MOVE_TYPE:
				sysCmd = hmiAxisNegMove(2, 2, 2);	
				break;
			case HMI_485_INTERP_RUN_TYPE:
				sysCmd = hmiInterpRun(3);
				break;
			case HMI_485_INTERP_PAUSE_TYPE:
				sysCmd = hmiInterpPause(state);
				break;
			case HMI_485_INTERP_RESUME_TYPE:
				sysCmd = hmiInterpResume(state);
				break;
			case HMI_485_INTERP_STEP_TYPE:
				sysCmd = hmiInterpStep(state);
				break;
			case HMI_485_SET_MOULD_NUM_TYPE:
				fileCmd = hmiFileCmd(4);
				sysCmd = hmiSetMouldNum(4);
				break;
			case HMI_485_CONTROL_ROBOT_TYPE:
				sysCmd = hmiControlRobot(1); //控制权 0机械手 1塑机
				break;
			case HMI_485_SET_CHAMFER_FLAG_TYPE:
				sysCmd = hmiSetChamferFlag(1); //倒角 0垂直  1水平
				break;
			case HMI_485_SET_FIXTURE_FLAG_TYPE:
				sysCmd = hmiSetFixtureFlag(1); //吸具 0关闭  1打开
				break;
			default:
				break;
		}	
		int nLen = STAT_CMD_SIZE;
		if(hmiCmdType == HMI_485_SYS_IDLE_TYPE)
			nLen = 0;
		Write(nType, &sysCmd, nLen);
		if(hmiCmdType == HMI_485_SET_MOULD_NUM_TYPE)
		{
			//usleep(80*1000); //wait 80ms
			Write(FILE_CMD, &fileCmd, FILE_CMD_SIZE);	
			m_flag_recv_file_data = true;
		}
	}
	
}

// check cmdData and handle fileCmd
bool  CtmTaskPickerComm::HandleCmdData(int nType, void *pData, int nLen)
{
	//if(m_flag_send_file_cmd) return false;
		
	if(FILE_CMD == nType)
	{
			if(nLen == FILE_CMD_SIZE)
			{
				//printf("require file data cmd.\n");
				if(m_flag_set_mould_num_cmd)
					m_flag_send_file_cmd = true;	
			}
			else return false;
	}
	else if(MOT_CMD==nType)
	{
			if(nLen == STAT_CMD_SIZE)
			{
				HMI_485_CMD motCmd;
				memset(&motCmd, 0, STAT_CMD_SIZE);
				memcpy(&motCmd, pData, STAT_CMD_SIZE);
				// check statu of "set mould in step"
				
				if(motCmd.hmiCmdType == HMI_485_SET_MOULD_NUM_TYPE)
				{
					//printf("hmiCmdType : set mould in step>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
					m_flag_set_mould_num_cmd = true;
					m_mouldNum = motCmd.hmiCmdData.hmiSetMouldNum.mouldNum;
				}
				else if(motCmd.hmiCmdType == HMI_485_AXIS_POS_MOVE_TYPE || motCmd.hmiCmdType == HMI_485_AXIS_NEG_MOVE_TYPE)
				{
					flag_axis_move_cmd = true;	
				}	
			}
			else return false;
	}
	else if(IDLE_CMD==nType || RESET_CMD==nType)
	{
			;
	}
	else 
			return false;
	
	return true;
}

/*****************************************************************
** NAME     :  Write()
** FUNCTION :  倒礶矗ㄑ钡ノㄓ糶㏑
** INPUT PARA   :  
**			 nType ㏑摸 code
**			 pData ㏑计沮
**			 nLen  计沮
** RETURN VALUE :
**			 nResult 龟悔祇癳竊计
** 			 タ祇癳Θ璽┪箂祇癳ア毖
** AUTHOR   :  @Wang.z.w     2015/9/5 8:52:13   
******************************************************************/
int		CtmTaskPickerComm::Write(int nType, void *pData, int nLen)
{
	int nResult = -1;
	//HandleCmdData(nType, pData, nLen)
	if(!HandleCmdData(nType, pData, nLen)) return -1;
//	if(m_lCheckTimer > 0) usleep(m_lCheckTimer*1000);
//	m_lCheckTimer = 100; // 100ms
	
	::pthread_mutex_lock(&m_Tx_lock);
	ComposeProtocol(nType, (BYTE *)pData, nLen);
	if(m_flag_sending_file_cmd)
	{
		nResult = SendData(m_sendbuff, m_nCount_Cmd_Data);
	}
	else
	{
		if(nType == FILE_CMD && m_flag_set_mould_num_cmd)
		{
				m_flag_sending_file_cmd = true;	
				memcpy(m_sendcmdbuff, pData, nLen);
				m_snedcmd_len = nLen;
		}
		else 
		{
				nResult = SendData(m_sendbuff, m_nCount_Cmd_Data);
		}
	}
	::pthread_mutex_unlock(&m_Tx_lock);
		
	return nResult;
}

// 代刚ノ
int		CtmTaskPickerComm::ParseData(void *pData, int nLen) 
{
	if(nLen > m_nCount_Stat_Frame)
		nLen = m_nCount_Stat_Frame;
		
	//printf("nLen = %x\n", nLen);
	memcpy(pData, m_parsebuff, nLen);
	
	return nLen;
}

/*****************************************************************
** NAME     :  RequestData()
** FUNCTION :  倒礶矗ㄑ钡ノㄓ弄3贺篈计沮
** INPUT PARA   :  
**			 nType 篈计沮摸0x01 0x02 0x03
**			 pData 癸莱篈计沮皐
**			 nLen  叫―计沮
** RETURN VALUE :
**			 nResult ボ琂竊计
** 			 タ弄 Θ璽┪箂弄ア毖
** AUTHOR       :  @Wang.z.w     2015/9/11 8:53:33
******************************************************************/
int		CtmTaskPickerComm::RequestData(int nType, void *pData, int nLen)
{
	int  nResult = 0;
	bool flag_ok = true;
	
	// RBOT_SYS_STAT		0x01     //诀瘪も北竟盾
  // RBOT_FILE_NAME   0x02     //ゅン篈絏
  // RBOT_COM_ERR	    0x03     //岿粇絏
	switch(nType)
	{
		case RBOT_SYS_STAT:
			if(nLen > (int)SYS_STAT_SIZE)
				nLen = SYS_STAT_SIZE;
						
			memcpy(pData, &m_sys_stat, nLen);
			break;
		case RBOT_FILE_NAME:
			if(nLen > (int)FILE_NAME_SIZE)
				nLen = FILE_NAME_SIZE;
			memcpy(pData, &m_file_name, nLen);
			break;
		case RBOT_COM_ERR:
			if(nLen > ERR_STAT_SIZE)
				nLen = ERR_STAT_SIZE;
			memcpy(pData, &m_err_stat, nLen);
			break;
		default:
			flag_ok = false;
			//printf("RequestData :nType is err\n ");
			break;
	}
	
	if(!flag_ok) return -1;
		
	nResult = nLen;
	return nResult;
} 

void	CtmTaskPickerComm::ComposeProtocol(int nType, BYTE *pData, int nLen) 
{
	//printf("Compos here: nType = %x, nLen = %x.\n", nType, nLen);
	
	if(nLen < 0) return ;
	
	int npos = 0;
	// @header
	m_sendbuff[npos++] = FRAME_HEAD;
	// @code
	m_sendbuff[npos++] = nType&0xFF;
	// @len
	m_sendbuff[npos++] = nLen&0xFF;
	// @data
	if(nLen > 0)
	{
		memcpy(m_sendbuff+npos, pData, nLen);
		npos += nLen;
	}
	// @crc
	WORD  crc =0;
	crc = GetCRC(pData, nLen, crc);
	memcpy(m_sendbuff+npos, &crc, sizeof(crc)); 
	npos += sizeof(crc);
	// @tail
	m_sendbuff[npos] = FRAME_TAIL;
	m_nCount_Cmd_Data = npos + 1;
}

WORD    CtmTaskPickerComm::CalcCRC(BYTE pData, WORD wCrc)
{
	WORD wCheck = 0;
	wCrc = wCrc^pData;
	
	for(int i=0; i<8; i++)
	{
		wCheck = wCrc & 1;
		wCrc   = wCrc >> 1;	
		wCrc   = wCrc & 0x7fff;
		
		if(wCheck == 1)
		{
			wCrc = wCrc ^ 0xa001;	
		}
		wCrc = wCrc & 0xffff;
	}
	
	return wCrc;
}

WORD    CtmTaskPickerComm::GetCRC(BYTE *pData, int nLen, WORD wCrcOut)
{
	WORD wH = 0;
	WORD wL = 0;
	WORD wCrc;
	wCrc = 0xFFFF;
	
	for(int i=0; i< nLen; i++)
	{
		wCrc = CalcCRC(*pData, wCrc);
		pData++;	
	}
	
	wH = wCrc / 256;
	wL = wCrc % 256;
	wCrc = (wH << 8) | wL;
	
	wCrcOut = wCrc;
	return wCrcOut;
}

void    CtmTaskPickerComm::Reset()
{
	m_flag_start= FALSE;
	//m_nErrCount = 0;
	memset(m_readbuff, 0, MAX_SIZE);
	
}

void    CtmTaskPickerComm::Run()
{
//	static int nCount = 0;
//	// cmd : read stat
//	if((m_lCheckTimer<=0) && !m_flag_send_file_cmd)
//	{
//		//m_lCheckTimer = 100; // 100ms
//		Write(IDLE_CMD, NULL, 0);	
//		
//		
//		//hmiIssueCommand(0, nCount%15);
//		//hmiIssueCommand(3, 11);
//		nCount++;
//		
//	}	
	if(m_lCheckTimer<=0)
	{
		m_lCheckTimer=100;
		if(m_clsFrameQueue.IsEmptyQueue(m_IDTxQueue))
		{
			initSend();
		}
	}
}

// 秅戳┦钡Μ计沮
int		CtmTaskPickerComm::Parse(void *pData, int nLen)
{
	/**********/
	static int nPos =0;
//	if(nPos > 74)
//	{
//		printf("Parse nLen=%d, nPos =%d, pData = %x, m_readbuff[2] = %x\n", nLen, nPos, *(BYTE *)pData, m_readbuff[2]);
//	}
	
	// 礚计沮
		if (nLen <= 0)
		{
			UpdateSend();
			m_lCheckTimer=100;
			nPos = 0;
			Reset();
			// 100毫秒记录一次接收失败
			if(m_lLineTimer <= 0)
			{
				//printf("LineLine m_nErrCount=%d\n",m_nErrCount);
				m_nErrCount++;
				m_lLineTimer = 400;
			}
			// 5次接收失败就报错
			if(m_nErrCount == 5)
			{
				m_nErrCount = 0;
				g_bPickerLineState = false;
				//printf("CtmTaskPickerComm::Parse() : recv err.\n");
			}

			if (lTimer1 < 0)
			{
				nPos = 0;
				Reset();
 				//g_bAmmeterStatus = FALSE;
				//printf("reopen device\n");
				Close();
				sleep(1);
				if(Open(&m_picker) < 0)
				{
					//perror("open error:\n");
				}
				m_clsFrameQueue.ClearQueue(m_IDTxQueue);
				m_lCheckTimer=100;
				m_nErrCount = 0;
				lTimer1 =PICKER_TIMEOUT;
			}	
			return -1;
		}
	
		// Τ计沮瞶
		if(nLen > 0)
		{
			//printf("-------------------------------recv start\n");
			m_nErrCount = 0;
			lTimer1 =PICKER_TIMEOUT; // 5s
			
			// 喷狤繷
			if(nPos==0) 
			{
				//printf("-------------------------------recv start\n");
				if(!m_flag_start && ((*(BYTE *)pData) == FRAME_HEAD))
				{
					m_flag_start = TRUE;
				}
				else
				{
					nPos =0;
					Reset();
					//UpdateSend();
					//m_lCheckTimer=100;
					return -1;
				}
			}
			
			// 越界限制
			if(nPos + nLen > MAX_SIZE)
			{
				nPos =0;
				Reset();
				return -1;	
			}
					
			memcpy(m_readbuff + nPos, pData, nLen);
			//printf("recv ok! nPos=%d m_readbuff[nPos]=%x \n", nPos, m_readbuff[nPos]);
			nPos += nLen;
			
			// record the lenth of data
			if(nPos == 3)
			{
				m_nCount_Stat_Data = *(BYTE *)pData;
				if(m_nCount_Stat_Data + ADD_OTHER_SIZE > MAX_SIZE) m_nCount_Stat_Data = MAX_SIZE - ADD_OTHER_SIZE;
				//printf("m_nCount_Stat_Data=%d<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n",m_nCount_Stat_Data);
			}
			
			// 喷痷鞍
			if((nPos > 3) && (nPos == (m_nCount_Stat_Data + ADD_OTHER_SIZE)))
			{
				//printf("nPos=%d,%d\n",nPos,m_nCount_Stat_Data + ADD_OTHER_SIZE);
				// recv data sucess
				if((*(BYTE *)pData) == FRAME_TAIL)
				{	
					if(HandleFrame(m_readbuff, nPos))// recv ok
					{
						nPos =0;
						Reset();
						UpdateSend();
						m_lCheckTimer=100;
						g_bPickerLineState = true;
					}
					else //接收数据有错，丢掉nMove个字节,找到下一个head
					{
						int nMove = UpdateFrame(m_readbuff, nPos);
						//printf("recv data has err : nMove = %d..................\n", nMove);
						nPos -= nMove;
						if(nPos == 0) Reset();
					}
				}
				else
				{
					int nMove = UpdateFrame(m_readbuff, nPos);
					//printf("the tail is err : nMove = %d....................\n", nMove);
					nPos -= nMove;
					//nPos =0;
					//Reset();
				}
				
				//UpdateSend();
				//m_lCheckTimer=100;
			}
						
			return nLen;
		}
		
/*************/
	return -1;
}

// handle frame err 
int   CtmTaskPickerComm::UpdateFrame(void *pData, int nLen)
{
	int i=1;
	BYTE bTemp[MAX_SIZE];
	memset(bTemp, 0, MAX_SIZE);
	memcpy(bTemp, pData, nLen);
	memset(m_readbuff, 0, MAX_SIZE);
	
	for(; i<nLen; i++)
	{
		if(bTemp[i] == FRAME_HEAD) // find next head
			break;
	}
	
	if(i <= nLen-1) // find
		memcpy(m_readbuff, bTemp+i, nLen-i);
		
	if(i <= nLen-3) // find
		m_nCount_Stat_Data = bTemp[i+2];
		
	if(i == nLen) // not find
		m_flag_start = false;
		
	return i;
}    

// 矪瞶钡Μ计沮喷矗Τノ獺
bool  CtmTaskPickerComm::HandleFrame(void *pData, int nLen)
{
	bool flag_ok = false;
	//printf("HandleFrame()...\n");

	// 莉篈计沮琿
	int npos =3;
	BYTE bTemp[MAX_SIZE];
	memset(bTemp, 0, MAX_SIZE);
	
	// 1. 喷计沮
	BYTE code     = m_readbuff[1]; // 数据状态码
	int  len_old  = m_readbuff[2]; // 理论数据长度
	int  len_new  = nLen - ADD_OTHER_SIZE; // 实际获得数据长度
	if(len_old != len_new)
	{
		//printf("HandleFrame: Check len err.(old =%x ,new = %x)\n", len_old, len_new);
		return false;	
	}

	// 2. 喷CRC
	WORD crc_new  = 0xFFFF;
	WORD crc_old  = 0xFFFF;
	memcpy(bTemp, m_readbuff+npos, len_new);

	crc_new  = GetCRC(bTemp, len_new, crc_new);
	crc_old  = ((m_readbuff[4 + len_new])<<8)|(m_readbuff[3 + len_new]);
	//printf("HandleFrame: Check crc(old =%x ,new = %x)\n", crc_old, crc_new);
	
	if(crc_old != crc_new)
	{
		printf("HandleFrame: Check crc err.(old =%x ,new = %x)\n", crc_old, crc_new);
		return false;
	}
	
	// 3. 秆弄篈
	//printf("HandleFrame(): code = %0x...\n", code);
	if(code == RBOT_COM_ERR)// 错误状态 0x03
	{	
		flag_ok = HandleErrStat(bTemp, len_new);
	}
	else if(code == RBOT_SYS_STAT)// 系统状态 0x01
	{
		flag_ok = HandleSysStat(bTemp, len_new);
	}
	else if(code == RBOT_FILE_NAME)// 文件状态 0x02
	{
		flag_ok = HandleFileName(bTemp, len_new);
	}
	else                          // 状态错误
	{
		//printf("the code of recv stat is err.\n");
		return false;	
	}
	
	return flag_ok;
}

// 矪瞶钡Μ岿粇篈计沮喷矗Τノ獺
bool  CtmTaskPickerComm::HandleErrStat(void *pData, int nLen)
{
	if(nLen != ERR_STAT_SIZE)
	{
		//printf("HandleErr() : nLen is over the range");
		return false;	
	}
	
	//printf("HandleErrStat is begin...");
	memset(m_err_stat, 0, ERR_STAT_SIZE);	
	memcpy(m_err_stat, pData, ERR_STAT_SIZE);
	
	// 玂DB戈
	//SaveErrStat();
	
	return true;	
}
	
// 矪瞶钡Μ家腹ゅン计沮喷矗Τノ獺
bool  CtmTaskPickerComm::HandleFileName(void *pData, int nLen)
{
	if(nLen != FILE_NAME_SIZE)
	{
		//printf("HandleFileName() : nLen is over the range");
		return false;	
	}
	
	//printf("HandleFileName is begin...\n");
	memset(&m_file_name, 0, FILE_NAME_SIZE);
	memcpy(&m_file_name, pData, FILE_NAME_SIZE);
	
	// @check echoMouldNum
	//WORD wClsprotPos = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED40").lValue;
	//WORD wMouldNumInStep = GetDBValue("").lValue;
	//if((m_file_name.echoMouldNum != m_mouldNum) /*&& (wMouldNumInStep != 0x12)*/) // failure
	if(m_flag_set_mould_num_cmd)
	{
			if(m_file_name.echoMouldNum != m_mouldNum) // check failure
			{
					m_flag_recv_file_data = true; 
					return false;
			}
			else // check sucess
			{
					m_nSendCount = 0;
					m_flag_recv_file_data = false; 
					m_flag_send_file_cmd = false;
					m_flag_sending_file_cmd = false;
					m_flag_set_mould_num_cmd = false;
					SendMsg(MSG_SH_PICKER_CHECK_MOULDNUM_OK, 0, 0, NULL);
			}
	}
	
		
	// 计沮玂DB戈
	//SaveFileStat();
	
	return true;
}

// 矪瞶钡Μ╰参篈计沮喷矗Τノ獺
bool  CtmTaskPickerComm::HandleSysStat(void *pData, int nLen)
{
	if(nLen != SYS_STAT_SIZE)
	{
		//printf("HandleStat() : nLen is over the range");
		return false;	
	}
	
	// 测试用，发完移轴命令后收到状态数据
	if(flag_axis_move_cmd)
	{
		flag_axis_move_cmd = false;
		//printf("HandleSysStat: recv sys ok after runing axis_move_cmd.****************************\n");
	}
		
	//printf("HandleSysStat is begin...");
	//printf("size::%d,nLen=%d\n",SYS_STAT_SIZE,nLen);
	memset(&m_sys_stat, 0, SYS_STAT_SIZE);
	memcpy(&m_sys_stat, pData, SYS_STAT_SIZE);

	/****************开始检验数据的正确性*******************/
	
	// 运动轴位置
	//if(!HandleAxisPos())
		//return false;
		
	// 控制器错误状态
	// 系统运动状态及模式
	// 系统运动轴状态 
		//if(!HandleSysAxis())
			//return false;	
	// 系统输出状态
		//if(!HandleOutStat())
			//return false;	
	// 系统输入状态	
		//if(!CheckStatData())
		//	return false;	
	/********* @end ***********/		
	
	// 各种数据无误后，保存到DB资料
	//SaveSysStat();
	
	// 告诉画面更新数据
	//sendMsg();
	
	return true;
}  

/******************************************/
int		CtmTaskPickerComm::CreateSelf()
{
	
	CTaskAmmeter::CreateSelf();
	m_pSelf=this;	
	m_lSendTimer =0;
	m_lLineTimer =400;
	m_lCheckTimer=100;
	memset(m_buffer,0,sizeof(m_buffer));
	
	m_queueTx.bFull	= FALSE;
	m_queueTx.pQueue=
	m_queueTx.pInput=
	m_queueTx.pOutput=m_buffer;
	m_queueTx.pQueueEnd=m_buffer+sizeof(m_buffer);
	m_queueTx_data_counter=0;
	m_IDTxQueue=0;
	if(Open(&m_picker) < 0)
	{
		perror("open error:\n");
	}
	else
	{
		StartRecv();	
	}
	sem_init(&m_TxWait, 0, 0);	
	::pthread_mutex_init(&m_Tx_lock,NULL);
	::pthread_mutex_init(&m_queueTx_lock,NULL);
	//pthread_create(&m_sendThreadID,NULL,(PRSRUN)TxRun,&m_queueTx);
	pthread_create(&m_checkThreadID,NULL,(PRSRUN)CheckRun,NULL);
	m_IDTxQueue=m_clsFrameQueue.CreateQueue(sizeof(m_buffer));
	AddTimer(&lTimer1);
	AddCounter(&m_lCheckTimer, m_idMe);
	AddCounter(&m_lLineTimer, m_idMe);
	AddCounter(&m_lSendTimer, m_idMe);
	
	return 0;
}
		
int		CtmTaskPickerComm::FreeSelf()
{	
	sem_destroy(&m_TxWait);
	::pthread_mutex_destroy(&m_Tx_lock);
	::pthread_mutex_destroy(&m_queueTx_lock);
	pthread_cancel(m_sendThreadID);
	pthread_cancel(m_checkThreadID);
	Close();
	DeleteTimer(&lTimer1);
	DeleteCounter(&m_lCheckTimer, m_idMe);
	DeleteCounter(&m_lLineTimer, m_idMe);
	DeleteCounter(&m_lSendTimer, m_idMe);
	
	return CTaskAmmeter::FreeSelf();
}

// 祇癳计沮
int 	CtmTaskPickerComm::SendData(void *pData, int nLen)
{
		PushQueue(pData,nLen);
		//printf("sendData end....\n");
	//push queue
//	int nStatus;
//	int	TimeCount=0;
//	BYTE*	pTemp=(BYTE*)pData;
//	for(int i=0; i<nLen; i++)
//	{
//		TimeCount=300;
//		while(((nStatus=PushQueue(*pTemp))==DEVICE_QUEUE_FULL)&&(--TimeCount>0));
//		pTemp++;	
//		if(nStatus!=DEVICE_SUCCESS)
//		{
//			printf("nStatus!=DEVICE_SUCCESS : err nStatus =%d--------------------\n",nStatus);
//			break;
//		}
//	}
//	
//	//m_flag_recv_false = false;
//	usleep(100*1000); //10ms等待收发切换
	return 0;
}

// 玂岿粇篈波疌
void    CtmTaskPickerComm::SaveErrStat()
{
}
  
// 玂╰参篈计沮
void    CtmTaskPickerComm::SaveSysStat()
{	
}
	 
// 玂ゅン篈计沮
void    CtmTaskPickerComm::SaveFileStat()
{
}

inline	int 	CtmTaskPickerComm::PushQueue(BYTE ch)
{
	
	int			Value;
	if(m_fdcom<0)		return  DEVICE_CLOSED;
	if(m_queueTx.bFull)	return  DEVICE_QUEUE_FULL;
	*m_queueTx.pInput++ =ch;
	if(m_queueTx.pInput == m_queueTx.pQueueEnd)
		m_queueTx.pInput	= m_queueTx.pQueue;
	//if(m_queueTx.pInput == m_queueTx.pOutput)
	::pthread_mutex_lock(&m_queueTx_lock);
	if(++m_queueTx_data_counter==sizeof(m_buffer))
	{
		m_queueTx.bFull	= TRUE;	
	}
	::pthread_mutex_unlock(&m_queueTx_lock);
	sem_getvalue(&m_TxWait, &Value);
	if(Value==0)
	{
		sem_post(&m_TxWait);	
	}
	return DEVICE_SUCCESS;
}

inline	int 	CtmTaskPickerComm::PushQueue(void* pInputData, int nLen)
{
	if(m_clsFrameQueue.IsFullQueue(m_IDTxQueue))
	{
		usleep(5*1000);	
	}
	
	if(m_clsFrameQueue.IsFullQueue(m_IDTxQueue))		return DEVICE_QUEUE_FULL;
	BYTE count=nLen&0xFF;
	m_clsFrameQueue.EnQueue(m_IDTxQueue,&count,1);
	if(m_clsFrameQueue.IsFullQueue(m_IDTxQueue))		return DEVICE_QUEUE_FULL;
	BYTE*	pTemp=(BYTE*)pInputData;
	m_clsFrameQueue.EnQueue(m_IDTxQueue,pTemp,count);
	return DEVICE_SUCCESS;
}

inline	int 	CtmTaskPickerComm::PopQueue(void* pOutputData,  int nLen)
{
	if(m_clsFrameQueue.IsEmptyQueue(m_IDTxQueue))
	{
		usleep(1000);//1ms等待重新检查
	}

	if(m_clsFrameQueue.IsEmptyQueue(m_IDTxQueue)) return DEVICE_QUEUE_EMPTY;
	int nResult = m_clsFrameQueue.DeQueue(m_IDTxQueue,pOutputData,nLen);	
	return nResult>0 ? 0:DEVICE_QUEUE_EMPTY;
}


inline	int		CtmTaskPickerComm::initSend()
{
	Write(IDLE_CMD, NULL, 0);
	::pthread_mutex_lock(&m_Tx_lock);
	char  buffer[256];
	char  count=0;
	//printf("initSend ..........\n");
	if((m_pSelf->PopQueue(&count,1))==DEVICE_QUEUE_EMPTY)
	{
		usleep(5*1000);
		if((m_pSelf->PopQueue(&count,1))==DEVICE_QUEUE_EMPTY)
		{
			::pthread_mutex_unlock(&m_Tx_lock);
			return DEVICE_QUEUE_EMPTY;
		}
		::pthread_mutex_unlock(&m_Tx_lock);
	}
	else
	{
		memset(buffer,0,sizeof(buffer));
		if(m_pSelf->PopQueue(buffer,count)==DEVICE_QUEUE_EMPTY)
	  {
	  	::pthread_mutex_unlock(&m_Tx_lock);
			return DEVICE_QUEUE_EMPTY;;
		}
		int nWriteCount=write(m_pSelf->m_fdcom, buffer, count);
		::pthread_mutex_unlock(&m_Tx_lock);
		return	nWriteCount;
	}	
}

inline	int		CtmTaskPickerComm::UpdateSend()
{
	::pthread_mutex_lock(&m_Tx_lock);
	usleep(5*1000);
	char  buffer[256];
	char  count=0;
	if((m_pSelf->PopQueue(&count,1))==DEVICE_QUEUE_EMPTY)
	{
		usleep(5*1000);
		if((m_pSelf->PopQueue(&count,1))==DEVICE_QUEUE_EMPTY)
		{
			::pthread_mutex_unlock(&m_Tx_lock);
			return DEVICE_QUEUE_EMPTY;
		}	
		else
		{
				//printf("2:nCount=%d\n",count);
				memset(buffer,0,sizeof(buffer));
				if(m_pSelf->PopQueue(buffer,count)==DEVICE_QUEUE_EMPTY) 	
				{
					::pthread_mutex_unlock(&m_Tx_lock);
					return DEVICE_QUEUE_EMPTY;
				}
				//for(int i=0; i<count; i++)
				//{
				//	printf("buffer[%d]=%x\n",i,buffer[i]);	
				//}
				int nWriteCount=write(m_pSelf->m_fdcom, buffer, count);
				::pthread_mutex_unlock(&m_Tx_lock);
				return nWriteCount;
		}
	}
	else
	{
		//printf("1:nCount=%d\n",count);
		memset(buffer,0,sizeof(buffer));
		if(m_pSelf->PopQueue(buffer,count)==DEVICE_QUEUE_EMPTY) 	
		{
			::pthread_mutex_unlock(&m_Tx_lock);
			return DEVICE_QUEUE_EMPTY;
		}
		//for(int i=0; i<count; i++)
		//{
		//	printf("buffer[%d]=%x\n",i,buffer[i]);	
		//}
		int nWriteCount=write(m_pSelf->m_fdcom, buffer, count);
		::pthread_mutex_unlock(&m_Tx_lock);
		return nWriteCount;
	}
		
}

// 对文件命令进行特殊处理
void  CtmTaskPickerComm::CheckRun(void *ptr)
{
	pthread_detach(pthread_self());
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CtmTaskPickerComm::CheckRun setcanceltype");
	}
	
	//int nCount  = 0;
	m_pSelf->m_lSendTimer = 0;
	
	while(1)
	{	
			// 控制文件命令重复发送条件
			if((m_pSelf->m_lSendTimer>0) || !m_pSelf->m_flag_sending_file_cmd) // 不用发
			{
				usleep(100*1000);//100ms
				continue;
			}
		
			// 发送了3次，仍没接收到文件状态数据，报错
			if(20 == m_nSendCount) //not recv fileStat
			{
				// 参数重置
				m_nSendCount = 0;
				memset(m_pSelf->m_sendcmdbuff,  0, MAX_SIZE);
				m_pSelf->m_snedcmd_len=0;
				m_pSelf->m_flag_set_mould_num_cmd = false;
				m_pSelf->m_flag_send_file_cmd = false;
				m_pSelf->m_flag_sending_file_cmd = false;
				if(m_pSelf->m_flag_recv_file_data)
				{
					//printf("CheckRun(): has received file data ,but its mouldnum is failed in step.\n");
					m_pSelf->m_flag_recv_file_data = false;
					SendMsg(MSG_SH_PICKER_CHECK_MOULDNUM_ERR, 0, 0, NULL); // 校验失败
				}
				else
				{
					//printf("CheckRun(): send file cmd with %d times ,but not receive file data.\n", m_nSendCount);
					SendMsg(MSG_SH_PICKER_RECV_FILE_ERR, 0, 0, NULL); // 同步失败
				}
				usleep(50*1000);//100ms
				continue;
			}
			
			// 发送命令数据
			int nResult =0;
			tmTIME  time;
			GetTime(&time);
			//printf("Send file cmd, m_nSendCount = %d    %2d:%2d\n", m_nSendCount,  time.minute, time.second);
			//for(int i=0; i<m_pSelf->m_snedcmd_len; i++)
			//printf("send[%d]=%x\n",i,m_pSelf->m_sendcmdbuff[i] );
			if(m_pSelf->m_snedcmd_len)
				nResult = m_pSelf->Write(FILE_CMD,m_pSelf->m_sendcmdbuff,m_pSelf->m_snedcmd_len);
			m_nSendCount++; // 记录发送命令次数
			m_pSelf->m_lSendTimer = PICKER_SEND_TIME; //100ms PICKER_TIMEOUT; //wait 500ms
			usleep(50*1000);//100ms
	}
	
}

void	CtmTaskPickerComm::TxRun(_QUEUE* _queue)
{
	pthread_detach(pthread_self());
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CtmTaskPickerComm::TxRun setcanceltype");
	}
	while(TRUE)
	{
		if(_queue->pInput == _queue->pOutput && _queue->bFull == FALSE)
		{
			sem_wait(&(m_pSelf->m_TxWait));
			continue;
		}
		
		int ncount=write(m_pSelf->m_fdcom, _queue->pOutput, 1);
		if(ncount<=0)	continue;
		*_queue->pOutput++;
		::pthread_mutex_lock(&m_pSelf->m_queueTx_lock);
		if(--m_pSelf->m_queueTx_data_counter<0)
			m_pSelf->m_queueTx_data_counter=0;
		::pthread_mutex_unlock(&m_pSelf->m_queueTx_lock);
		if(_queue->pOutput == _queue->pQueueEnd)
		{
			_queue->pOutput	= _queue->pQueue;	
			_queue->bFull	= FALSE;
		}	
	}	
}

#ifndef	 D_BEOBJECT_CTMTASKPICKCKERCOMM
CObject*	CreateInstance(char* Name)
{	
	CObject*	pResult = Create(Name);
	if (pResult != NULL)
	{
		(static_cast<CtmTaskPickerComm*>(pResult))->CreateSelf();
	}
	
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmTaskPickerComm*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

