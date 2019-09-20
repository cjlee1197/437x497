/*==========================================================================+
|  Class    : CNC communication          									|
|  Task     : CNC communication file                     					|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James		                                                    |
|  Version  : V1.00                                                         |
|  Creation : 2009/12/23                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#define __USE_GNU
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <linux/unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include	"rtai.h"
#include	"taskcnc.h"
#include	"main.h"
#include	"utils.h"
#include	"tmdebug.h"
#include	"libCNCComm.h"
#include	"tmdebug.h"
#include	"commonaction.h"
#include 	"multilanguage.h"
#include 	"tmshmsg.h"
#include	"tmconfig.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		CNC_COUNTER					20
#define		CNC_UPDATE_COUNTER			25
#define		CNC_HEAD_OFFSET_LENGTH		8
#define		CNC_SERVO_PARAM_NAME_0		"SERVO_PARAM0"
#define		CNC_SERVO_PARAM_NAME_MAX	"SERVO_PARAM134"
#define		CNC_TUNE_PARAM_NAME_0		"TUNE_PARAM0"
#define		CNC_TUNE_PARAM_NAME_MAX		"TUNE_PARAM69"
#define		CNC_MAKER_PARAM_NAME_0		"MAKER_PARAM0"
#define		CNC_MAKER_PARAM_NAME_MAX	"MAKER_PARAM50"
#define		CNC_USER_PARAM_NAME_0		"USER_PARAM0"
#define		CNC_USER_PARAM_NAME_MAX		"USER_PARAM13"
#define		CNC_COMMON_PARAM_NAME_0		"COMMON_PARAM0"
#define		CNC_COMMON_PARAM_NAME_MAX	"COMMON_PARAM10"

#define		CNC_MACRO_LOCAL_VAR_0		"CNC_MACRO_LOCAL_VAR0"
#define		CNC_MACRO_LOCAL_VAR_MAX		"CNC_MACRO_LOCAL_VAR33"
#define		CNC_MACRO_GLOBAL_VAR_0		"CNC_MACRO_GLOBAL_VAR0"
#define		CNC_MACRO_GLOBAL_VAR_MAX	"CNC_MACRO_GLOBAL_VAR100"
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
#define		CNC_READ_STARTID_SAVETODB	"S_MLC_I_BIT0"
#define		CNC_READ_ENDID_SAVETODB		"CNC_DIS_TO_GO_X"
#define		CNC_TASK_CYCLE_TIME			10000		//unit:us
#define 	TIMER_RELTIME				0
#define 	USEC_PER_SEC				1000000
#define 	NSEC_PER_SEC				1000000000
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
//pthread_t		CTaskCNC::m_CNCThreadID;		 mario mark 2015/12/28 下午 03:14:56
int				CTaskCNC::m_nID = 0;
int				CTaskCNC::m_nParamID[PARAMTYPE_NUB][PARAMTYPE_COUNT] = {0};
int				CTaskCNC::m_nInitSendID[INITSEND_NUB][INITSEND_COUNT] = {0};
int				CTaskCNC::m_RxSize = 0;
int				CTaskCNC::m_TxSize = 0;
int				CTaskCNC::m_ReadStartOffset = 0;
int				CTaskCNC::m_ReadEndOffset = 0;	
int 			iniok = 0;
CNC_TXDATA		g_TxCNCData;
CNC_RXDATA*		g_pRxCNCData=NULL;
CNC_TXSHMDATA 	*g_pTxCNCData;
TP_TABLE			g_TeachPoint;

TRACKPOINT*	ptmTrackPoint_Head = NULL;
TRACKPOINT*	ptmTrackPoint_Temp = NULL;
TRACKPOINT	g_TrackPoint_EndPoint;

FILE *fpt = NULL;

LONG_AXIS	tmTrackPoint_Max = {0,0,0,0,0}, tmTrackPoint_Min = {0,0,0,0,0};

CtmStringList	g_OpFileStringList;
WORD			g_wCurve_Horizontal_Axis =0, g_wCurve_Vertical_Axis =1;
WORD			g_wCurve_Point_Save_Cnt  =0;
BOOL			g_bCurve_Point_Save_Flag =0;
long			g_lCNC_Current_block_no_Old =0;
BOOL 			u_bFirstPointFlag = TRUE;

char last_filename[INT_FILE_LEN]="";
long u_queue_block_no;
char u_build_queue; //wilson lu, 2010.12.21 

long u_MDI_queue_block_no;	//20110510 addMDI queue
char u_MDI_build_queue;		//20110510 addMDI queue
CtmStringList	g_TempStringList;

char current_filename[INT_FN_LEN];

/*--------------------------------------------------------------------------+
|      :                                                      				|
+--------------------------------------------------------------------------*/

#ifdef __UCLIBC__
#define MAKE_PROCESS_CPUCLOCK(pid, clock) \
	((~(clockid_t) (pid) << 3) | (clockid_t) (clock))
#define CPUCLOCK_SCHED          2

static int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *req,
		struct timespec *rem)
{
	if (clock_id == CLOCK_THREAD_CPUTIME_ID)
		return -EINVAL;
	if (clock_id == CLOCK_PROCESS_CPUTIME_ID)
		clock_id = MAKE_PROCESS_CPUCLOCK (0, CPUCLOCK_SCHED);

	return syscall(__NR_clock_nanosleep, clock_id, flags, req, rem);
}

static int sched_setaffinity(pid_t pid, unsigned int cpusetsize,
		cpu_set_t *mask)
{
	return -EINVAL;
}

static void CPU_SET(int cpu, cpu_set_t *set) { }
static void CPU_ZERO(cpu_set_t *set) { }
#else
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
			   __const struct timespec *__req,
			   struct timespec *__rem);
#endif 			

/*--------------------------------------------------------------------------+
|  Task     :  Auto queue                                                   |
+--------------------------------------------------------------------------*/
void CTaskCNC::ResetQueue(void)
{
	//printf("===== ResetQueue =====\n");
	strcpy(last_filename,"");
	u_queue_block_no=(long)GetDBValue("CNC_START_BLK_NO").lValue;
	if(g_pTxCNCData != NULL)
	{
		g_pTxCNCData->CNC_queue1_status=EMPTY_STATUS;
		g_pTxCNCData->CNC_queue2_status=EMPTY_STATUS;
		g_pTxCNCData->CNC_queue1_total_block=0;
		g_pTxCNCData->CNC_queue2_total_block=0;
	}
	return;
}
  	       
void CTaskCNC::BuildQueue(void)
{
	//printf("BuildQueue\n");
#ifndef	D_TEST
	int i;
	//static fpos_t filepos;
	static long filepos;
	//static FILE *fpt = NULL;
	char full_filename[50];
	int read_block_no;
	char block_buffer[INT_BUFFER_SIZE];
	long start_block_no;
	//char current_filename[INT_FN_LEN];
	memset(&current_filename,0,sizeof(current_filename));
	
	if(g_pRxCNCData->CNC_rebuild_all_queue==0) start_block_no=(long)GetDBValue("CNC_START_BLK_NO").lValue;
	else start_block_no=g_pRxCNCData->CNC_read_block_no;
	   	
	//printf(" CNC_rebuild_all_queue=%d,start_block_no=%ld\n",g_pRxCNCData->CNC_rebuild_all_queue,start_block_no);
	if(g_pRxCNCData->CNC_rebuild_all_queue==1 || (g_pRxCNCData->CNC_rebuild_all_queue==0 && u_queue_block_no==start_block_no))
	{ 
		if(g_pRxCNCData->CNC_rebuild_all_queue==0)
		{
			GetDBString("CNC_MEM_PART",(char *)&g_TxCNCData.shmCNCData.CNC_mem_part, sizeof(g_TxCNCData.shmCNCData.CNC_mem_part));  
			strcpy(current_filename,(char *)&g_TxCNCData.shmCNCData.CNC_mem_part);
		}
		else
		{
			strcpy(current_filename,(char *)&g_pRxCNCData->CNC_read_block_filename);
		}
		//if(fpt!=NULL) fclose(fpt);
		
		//sprintf(full_filename, "./Prog/%s", current_filename);//Sunny<20110609>
		sprintf(full_filename, "%s/%s", g_szProgDirectory, current_filename);
		//printf("Auto-full_filename=<%s>,u_build_queue=%d current_filename:%s \n",full_filename,u_build_queue,g_pRxCNCData->CNC_read_block_filename);
		if(fpt!=NULL)
		{
			fclose(fpt);
			fpt=NULL;
		} 
		fpt=fopen(full_filename,"rt");
		if(fpt==NULL) 
		{
			
			//printf("BuildQueue---000000 full_filename = %s is NULL\n",full_filename);
			ResetQueue();
			g_pTxCNCData->CNC_queue1_status=END_OF_FILE;
			g_pTxCNCData->CNC_queue2_status=END_OF_FILE;
			//printf("!!!!!!!!!!!!!!!fpt=NULL!,CNC_rebuild_all_queue=%d\n",g_pRxCNCData->CNC_rebuild_all_queue);
			return;
		}
		fseek(fpt,0,SEEK_SET);
		u_queue_block_no=0;
		while(u_queue_block_no<start_block_no)
		{
	   		if(fgets((char *)&block_buffer[0], INT_BUFFER_SIZE, fpt)!=NULL)
	   		{
	   			u_queue_block_no++;
	   		}
	   		else
	   		{
	   			//printf(" Wrong start block number!\n");
	   			if(fpt!=NULL)
	   			{
	   				//printf("fclose \n");
	   				fclose(fpt);
	   				fpt = NULL;//Sunny<20110308>add
	   			}
	   			break;
	   		}
	   	}
		filepos=ftell(fpt);
	}
	//printf("BuildQueue-----------filepos=%ld current_filename:%s\n",filepos,current_filename);
	if(fpt!=NULL)//Sunny<20110310>add
	{
		//printf("Error setting file pointer.fpt=%d filepos =%ld \n",fpt,filepos); 
		if(fseek(fpt,filepos,SEEK_SET)!=0)	printf("Error setting file pointer.\n"); 
	}

	//================ Build first queue ==================
	if(g_pRxCNCData->CNC_request_queue1==ID_BUILD_QUEUE || u_build_queue==1 || u_build_queue==3)
	{
		read_block_no=0;
		g_pTxCNCData->CNC_queue1_total_block=0;
    	memset(&g_pTxCNCData->CNC_queue1,0,sizeof(g_pTxCNCData->CNC_queue1));
		while(read_block_no<NUM_OF_BLOCK)
		{
			if(fpt!=NULL)//Sunny<20110310>add
			{
				if(fgets((char *)&block_buffer[0], INT_BUFFER_SIZE, fpt)!=NULL)
				{	
						//printf("block_buffer = %s\n",block_buffer);
						char temp[INT_BUFFER_SIZE];
	    			//strcpy(temp,g_pTxCNCData->CNC_queue1[read_block_no]);
	    			//strcpy((char *)&g_pTxCNCData->CNC_queue1[read_block_no],CommandChang(temp,block_buffer));
	    			WriteValue((long)&g_TxCNCData.shmCNCData.CNC_queue1[read_block_no],CommandChang(temp,block_buffer),sizeof(temp));
	    		  //printf("g_pTxCNCData->CNC_queue1[%d] is [ %s ]\n",read_block_no,g_pTxCNCData->CNC_queue1[read_block_no]);
	    			
	    			filepos=ftell(fpt);
	    			//printf(" 1st queue : read_block_no=%d,u_queue_block_no=%ld,current_filename=%s,block_buffer=%s\n",read_block_no,u_queue_block_no,current_filename,block_buffer);
	    			u_queue_block_no++;
	    			read_block_no++;
	    			g_pTxCNCData->CNC_queue1_total_block++;
				}
				else break;
			}
			else break;
	}
	if(u_build_queue==1 || u_build_queue==3) g_pTxCNCData->CNC_queue1_status=FULL_STATUS;
	else g_pTxCNCData->CNC_queue1_status=READ_OK_STATUS;
	//if(g_pTxCNCData->CNC_queue1_status==READ_OK_STATUS) ;//printf("CNC_queue1_status=READ_OK_STATUS\n");
	//else if(g_pTxCNCData->CNC_queue1_status==FULL_STATUS) ;//printf("CNC_queue1_status=FULL_STATUS\n");
	}
	
	//================ Build second queue ==================
	if(g_pRxCNCData->CNC_request_queue2==ID_BUILD_QUEUE || u_build_queue==1 || u_build_queue==3)
	{
		read_block_no=0;
		g_pTxCNCData->CNC_queue2_total_block=0;
		memset(&g_pTxCNCData->CNC_queue2,0,sizeof(g_pTxCNCData->CNC_queue2));
		while(read_block_no<NUM_OF_BLOCK)
		{
	    	if(fpt!=NULL)//Sunny<20110310>add
	    	{
	    		if(fgets((char *)&block_buffer[0], INT_BUFFER_SIZE, fpt)!=NULL)
	    		{
						//printf("block_buffer = %s\n",block_buffer);
	    			char temp[INT_BUFFER_SIZE];
	    			//strcpy((char *)&g_pTxCNCData->CNC_queue2[read_block_no],CommandChang(temp,block_buffer));
	    			WriteValue((long)&g_TxCNCData.shmCNCData.CNC_queue2[read_block_no],CommandChang(temp,block_buffer),sizeof(temp));
	    			//printf("g_pTxCNCData->CNC_queue2[%d] is [ %s ]\n",read_block_no,g_pTxCNCData->CNC_queue2[read_block_no]);
	    			
	    			
	    			//strcpy((char *)&g_pTxCNCData->CNC_queue2[read_block_no],block_buffer);
	    			filepos=ftell(fpt);
	    			//printf(" 2rd queue : read_block_no=%d,u_queue_block_no=%ld,current_filename=%s,block_buffer=%s\n",read_block_no,u_queue_block_no,current_filename,block_buffer);
	    			u_queue_block_no++;
	    			read_block_no++;
	    			g_pTxCNCData->CNC_queue2_total_block++;
	    		}
	    		else break;
	    	}
	    	else break;
		}
		if(u_build_queue==1 || u_build_queue==3) g_pTxCNCData->CNC_queue2_status=FULL_STATUS;
		else g_pTxCNCData->CNC_queue2_status=READ_OK_STATUS;
		//if(g_pTxCNCData->CNC_queue2_status==READ_OK_STATUS) ;//printf("CNC_queue2_status=READ_OK_STATUS\n");
		//else if(g_pTxCNCData->CNC_queue2_status==FULL_STATUS) ;//printf("CNC_queue2_status=FULL_STATUS\n");

	}
	
	if(fpt != NULL)//Sunny<20110308>add
	{
		if(feof(fpt) || g_pRxCNCData->CNC_reset_flag==1)	//20110914 add g_pRxCNCData->CNC_reset_flag==1
		{
			//printf("======= end of part program file =======\n");
			//while(1)
				//if(fclose(fpt)==0) break;
				fclose(fpt);
			fpt = NULL;//Sunny<20110308>add
		}
	}
	
	if(u_build_queue==1 || u_build_queue==3) u_build_queue=2;
	return;
#endif
}	

void	CTaskCNC::ReadQueue(void)
{
	//printf("ReadQueue\n");
#ifndef	D_TEST
	//2012/8/10 下午 02:09:54 Test<<<<<<<<<<<<
	static char	pszFile[64] = "\0", pszFile_old[64] = "\0";
	static int	nCnt = 0;
	if(strcmp(pszFile_old,g_pRxCNCData->CNC_curr_blk.modal.fn))
	{
		strcpy(pszFile_old,g_pRxCNCData->CNC_curr_blk.modal.fn);
		if(strcmp("O010",g_pRxCNCData->CNC_curr_blk.modal.fn)==0)
		{
			nCnt++;
			//printf("nCnt:%d \n",nCnt);
		}
	}
	//2012/8/10 下午 02:09:54 Test>>>>>>>>>>
	
	static char build_queue_again=0;
	if((g_pRxCNCData->CNC_reset_flag==1 || g_pRxCNCData->CNC_prog_end==1) && u_build_queue==2 && build_queue_again==0)
	{
		//printf("g_pRxCNCData->CNC_reset_flag==1 || g_pRxCNCData->CNC_prog_end==1) && u_build_queue==2 && build_queue_again==0\n");
		
		if(g_pRxCNCData->CNC_prog_end == 1 && g_TxCNCData.shmCNCData.CNC_cycle_status == 0)
		{
			if(g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP )
			{
				char temp = RUN_STATUS_STOP;
				g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_auto_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_auto_status));
			}
			EtherNet_WriteValue(HMI2NC_RE_PEOG_END,0,1,0);	//與 NC mode.c配合 
		}
		//printf("reset:CNC_prog_end=%d,u_build_queue=%d\n",g_pRxCNCData->CNC_prog_end,u_build_queue);
		ResetQueue();
		u_build_queue=3;
		build_queue_again=1;
	}
	else if(g_pRxCNCData->CNC_prog_end==0 || u_build_queue==1  || u_build_queue==3)
	{
		if(g_pRxCNCData->CNC_prog_end==0) build_queue_again=0;
		if((g_pRxCNCData->CNC_request_queue1==ID_BUILD_SUB_QUEUE || g_pRxCNCData->CNC_request_queue2==ID_BUILD_SUB_QUEUE))	//ETHERNET才需要此處理 sharememory不需要
		{
			if(strcmp(current_filename,g_pRxCNCData->CNC_read_block_filename)!=0)
			{
				g_pRxCNCData->CNC_request_queue1=g_pRxCNCData->CNC_request_queue2=ID_BUILD_QUEUE;
			}
		}
		if(g_pRxCNCData->CNC_request_queue1==ID_BUILD_QUEUE || g_pRxCNCData->CNC_request_queue2==ID_BUILD_QUEUE || \
		   u_build_queue==1 || u_build_queue==3)
		{
			if(u_build_queue==1)
			{
				 ResetQueue();
			}
			if(g_pRxCNCData->CNC_request_queue1==ID_BUILD_QUEUE && g_pTxCNCData->CNC_queue1_status==FULL_STATUS &&
			   g_pTxCNCData->CNC_queue1_status!=END_OF_FILE)
			{
				g_pTxCNCData->CNC_queue1_status=EMPTY_STATUS;
				g_pTxCNCData->CNC_queue1_total_block=0;
				//printf(" ==> begin to build queue1\n");
			}
			if(g_pRxCNCData->CNC_request_queue2==ID_BUILD_QUEUE && g_pTxCNCData->CNC_queue2_status==FULL_STATUS &&
			   g_pTxCNCData->CNC_queue2_status!=END_OF_FILE)
			{
				g_pTxCNCData->CNC_queue2_status=EMPTY_STATUS; 
				g_pTxCNCData->CNC_queue2_total_block=0;
				//printf(" ==> begin to build queue2\n");
			}
			if(g_pTxCNCData != NULL)
			{
				if(g_pTxCNCData->CNC_queue1_status==EMPTY_STATUS ||  g_pTxCNCData->CNC_queue2_status==EMPTY_STATUS)
				{
					//printf(" CNC_rebuild_all_queue=%d\n",g_pRxCNCData->CNC_rebuild_all_queue);
					BuildQueue();
				}
			}
		}
		if(g_pRxCNCData->CNC_request_queue1==ID_SET_FULL && g_pTxCNCData->CNC_queue1_status==READ_OK_STATUS)
		{
			g_pTxCNCData->CNC_queue1_status=FULL_STATUS;
			//printf(" set g_pTxCNCData->CNC_queue1_status=FULL_STATUS ! \n");
		}
		if(g_pRxCNCData->CNC_request_queue2==ID_SET_FULL && g_pTxCNCData->CNC_queue2_status==READ_OK_STATUS)
		{
			g_pTxCNCData->CNC_queue2_status=FULL_STATUS;
			//printf(" set g_pTxCNCData->CNC_queue1_status=FULL_STATUS ! \n");
		}
		if(g_pTxCNCData != NULL)
		{
			if(g_pRxCNCData->CNC_request_queue1==ID_NO_ACT && g_pTxCNCData->CNC_queue1_status==END_OF_FILE)
			{
				g_pTxCNCData->CNC_queue1_status=EMPTY_STATUS;
				u_build_queue=1;
				//printf(" set g_pTxCNCData->CNC_queue1_status=END_OF_FILE ! \n");
			}
			if(g_pRxCNCData->CNC_request_queue2==ID_NO_ACT && g_pTxCNCData->CNC_queue2_status==END_OF_FILE)
			{
				g_pTxCNCData->CNC_queue2_status=EMPTY_STATUS;
				u_build_queue=1;
				//printf(" set g_pTxCNCData->CNC_queue2_status=END_OF_FILE ! \n");
			}
		}
	}
#endif
}


/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskCNC::CTaskCNC() : CTaskComm()
{
	//printf("CTaskCNC 	\n");
	RTAI	rtai;
	m_nID	= -1;
	m_lUpdateCounter= CNC_UPDATE_COUNTER;
	m_RxSize = rtai.nRxSize	= sizeof(CNC_RXDATA);
	m_TxSize = rtai.nTxSize	= sizeof(CNC_TXDATA);
	
	rtai.RTAIFunc	= (RTAIFUNC)ResValueFunc; //將motion 傳來的數據寫到data base!
	m_nID			= AddDevice("CRTAI", &rtai);
	g_pTxCNCData = (CNC_TXSHMDATA *)pRtaiTxAddr;
	/*if(g_pDatabase != NULL)			//Mario mark maybe Error!!!!!!!!!!!!!!!!!!!!
		g_pDatabase->LoadDB();*/
	
	//Init();
	RegisterMessage(MSG_RTAI, m_idMe);

	//printf("pthread_create  Process ShareMemory Data Form NC\n");
	if(pthread_create(&m_RxEndPointThread, NULL, (PRSRUN)SaveTrackEndPoint, NULL) != 0)
	{
		perror("CTaskCNC::CTaskCNC()");
	}
		
	AddCounter(&m_lUpdateCounter, m_idMe);
	//printf("CTaskCNC over\n");
}

CTaskCNC::~CTaskCNC()
{
	if(m_nID >= 0)	DelDevice(m_nID);
	DeleteCounter(&m_lUpdateCounter, m_idMe);
	UnRegisterAllMessage(MSG_RTAI);
	if(m_RxEndPointThread!= 0)	pthread_cancel(m_RxEndPointThread);
}

void    CTaskCNC::Run()
{
	static BOOL bFirstRun = TRUE;
	int result=0;
	int i;
	if(bFirstRun)
	{
		//printf("Run bFirstRun\n");
		Init();
		bFirstRun =FALSE;
		LoadTeachPoint();				//Mario add 2015/4/7
	}
	if (m_lUpdateCounter < 0 && g_ptaskCmd->GetFocus() != NULL)	//20120222 mark
	{
		//g_ptaskCmd->GetFocus()->Update();
		m_lUpdateCounter = CNC_UPDATE_COUNTER;
	}
#ifndef	D_TEST	
	if(g_pRxCNCData != NULL)
	{
		Motion_DB_Modify();					//Sunny<20100812>
		Alarm_Control();					//Sunny<20100819>
		if(g_pRxCNCData->CNC_reset_flag==1 && g_TxCNCData.shmCNCData.CNC_action_id == ID_RESET/*4*/) //HMI使用RESET
		{
			Now_Push_Btn->CreateA();			
			Now_Push_Btn->UpdateAll();		
			Now_Push_Btn->Update();		
			SendMsg(MSG_SH_APP_CNC_RESET_FLAG,0,0,NULL);
			g_pTxCNCData->CNC_action_id = ID_NO_ACTION;
			g_TxCNCData.shmCNCData.CNC_action_id = ID_NO_ACTION/*0*/;
			EtherNet_WriteValue(HMI2NC_UPDATE,(int)&g_pTxCNCData->CNC_action_id-(int)&g_pTxCNCData->param_update_id,sizeof(g_pTxCNCData->CNC_action_id),(void*)&g_pTxCNCData->CNC_action_id);
		}
		else	if(g_pRxCNCData->CNC_reset_flag==1 && g_pTxCNCData->CNC_action_id == ID_RESET/*4*/)		//CNC內部自動RESET
		{
			SendMsg(MSG_SH_APP_CNC_RESET_FLAG,0,0,NULL);
			g_pTxCNCData->CNC_action_id = ID_NO_ACTION;
			EtherNet_WriteValue(HMI2NC_UPDATE,(int)&g_pTxCNCData->CNC_action_id-(int)&g_pTxCNCData->param_update_id,sizeof(g_pTxCNCData->CNC_action_id),(void*)&g_pTxCNCData->CNC_action_id);
			//g_TxCNCData.shmCNCData.CNC_action_id = ID_NO_ACTION/*0*/;
		}
	}
	
	//MetricOrInch_System_Control();		//20110908
#endif

}

/*--------------------------------------------------------------------------+
|  Function :  ResValueFunc(char *pData, int nLength)            			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CTaskCNC::ResValueFunc(char *pData, int nLength)//Read the Data from g_pRxCNCData
{
	CNC_RXDATA	*pRx		= (CNC_RXDATA *)pData;
  
	if(pData != NULL && g_pRxCNCData==NULL) 
	{
		 g_pRxCNCData = (CNC_RXDATA* )pData;//memcpy(&g_pRxCNCData, pData, nLength); 20110830 modify
	}

	#ifndef	D_TEST
		nLength = m_ReadEndOffset-m_ReadStartOffset+3;	//Sunny<20100414> add 不是整個struct都存到資料庫		
		//printf("CTaskCNC nLength is %d\n",nLength);
		//printf("CTaskCNC Memory Len is %d\n",(int)&g_pRxCNCData->CNC_dis_to_go.c-(int)&g_pRxCNCData->s_mlc_i_bit[0]+3);
		
  		//讀到數據后，批量寫入到資料庫中去
		if(g_pDatabase != NULL)
		{
    	 	g_pDatabase->WriteBatch(pRx, m_ReadStartOffset, nLength);
		}
	#endif
}

/*--------------------------------------------------------------------------+
|  Function :  Read(char *pData, int nLength)            					|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::Read(int nType,	int nLength, void *pData)
{
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  Write(int nType, int nLength, void *pData)					|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::Write(int nType, int nLength, void *pData)
{	
	switch(nType)
	{
		default:
			break;
	}
	return 0;
}


/*--------------------------------------------------------------------------+
|  Function :  WriteValue(char *pszID)										|
|  Task     :  單筆資料寫                                             		|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::WriteValue(char *pszID)
{
	int 	nResult = 0, nDataID = 0;
	char	cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pszID, sizeof(cszStrID));

	if(g_pDatabase != NULL)
	{
		nDataID = g_pDatabase->GetDataID(cszStrID);
		WriteValue(nDataID);
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteValue(int nDataID)										|
|  Task     :  單筆資料寫                                                   |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::WriteValue(int nDataID)
{
	int 	nResult = 0, nType = 0, nLength = 0, nOffset= 0;
	WORD	wDataType = 0;
	BOOL	bParaUpdate = FALSE;
	DBVALUE		data;
	DATATYPE	DataType;
	void*	pInputData = NULL;
	
	memset(&data, 0, sizeof(DBVALUE));
	memset(&DataType, 0, sizeof(DATATYPE));
	nType = FindParamType(nDataID);
	//如果是PARAM資料的話，先設置type Sunny<20110311>修改成先寫下參數，再設置type，否則會出現讀到舊值的問題
	if(nType > 0)
	{
		g_TxCNCData.shmCNCData.param_update_id = nType;//Sunny<20110311> move behind
		bParaUpdate =  TRUE;
	}
	//查找資料數據類型和offset，將其拷貝到share memory 中去。
	memcpy(&DataType, &g_pDatabase->DataIDGetDataType(nDataID), sizeof(DATATYPE));
	switch(DataType.wType)
	{
		case TYPE_BOOL:
		case TYPE_BYTE:
		case TYPE_WORD:
		case TYPE_DWORD:
		case TYPE_CHAR:
		case TYPE_SHORT:
		case TYPE_INT:
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
			data = g_pDatabase->Read(g_pDatabase->DataIDToIndex(nDataID));
			pInputData = data.acData;
			break;
		case TYPE_STRING:
			pInputData = g_pDatabase->Reads(g_pDatabase->DataIDToIndex(nDataID));
			break;
		case TYPE_UNKNOWN:
		default:
			break;
	}
	if(pInputData != NULL)
	{
		g_TxCNCData.lOffset = nOffset = g_pDatabase->DataIDGetOffset(nDataID);
		g_TxCNCData.lLength = nLength = DataType.wLength;
	}
	else
	{
		g_TxCNCData.lOffset = nOffset = 0;
		g_TxCNCData.lLength = nLength = 0;	
	}
	//printf("nOffset is %d\n",nOffset); 
	memcpy(((char*)&g_TxCNCData)+CNC_HEAD_OFFSET_LENGTH + nOffset, pInputData,nLength);
	SendData(m_nID, (char *)&g_TxCNCData, nLength);
	EtherNet_WriteValue(HMI2NC_UPDATE,nOffset,nLength,(char*)&g_TxCNCData.shmCNCData.param_update_id+nOffset);		//ethernet
	
	//if it's Para data , need set type
	if(bParaUpdate)
	{
		g_TxCNCData.lOffset = nOffset = 0;
		g_TxCNCData.lLength = nLength = 4;
		SendData(m_nID, (char *)&g_TxCNCData, nLength);
		EtherNet_WriteValue(HMI2NC_UPDATE,g_TxCNCData.lOffset,g_TxCNCData.lLength,(char*)&g_TxCNCData.shmCNCData.param_update_id);		//ethernet
	}
	//SaveParaToFile();
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteValue(int nType, void* pInputData, int nSize)			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::WriteValue(int nType, void* pInputData, int nSize)
{
	long nResult = 0, nOffset = 0, nLength = 0;
	
	//printf("Len1=%d Len2=%d \n",(char*)&g_TxCNCData.shmCNCData.CNC_machine -(char*)&g_TxCNCData.shmCNCData,sizeof(CNC_TXDATA)-8);
	if(nSize == 0)
		nLength = (char*)&g_TxCNCData.shmCNCData.CNC_goto_tp_no -(char*)&g_TxCNCData.shmCNCData;//sizeof(CNC_TXDATA)-8; 2012/7/25 modify
	else
		nLength = nSize;
	
	//printf("nType=%d \n", nType);
	switch(nType)
	{
		case 	0:
			if(pInputData == NULL)
			{
				g_TxCNCData.lOffset = nOffset;
				g_TxCNCData.lLength = nLength;
			}
			else
			{
				if(nSize > 0)
					memcpy(((char*)&g_TxCNCData)+CNC_HEAD_OFFSET_LENGTH, pInputData,nLength);	
			}
			SendData(m_nID, (char *)&g_TxCNCData, nLength);
			break;
		case 	ID_SERVO:
		case 	ID_TUNE:
		case	ID_USER:
		case	ID_MAKER:
		case	ID_COMMON:
			SendData(m_nID, (char *)&g_TxCNCData, nLength);
			break;
		default:
			break;
	}
	nResult = nLength;
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteValue(long shmDataAddr, void* setDataAddr, int nLength)
|  Task     :  單筆資料寫入g_TxCNCData，未存入資料庫                                                        |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+-------------------------------------------------------------F-------------*/
void	CTaskCNC::WriteValue(long shmDataAddr, void* setDataAddr, int nLength)			//Add by Mario 
{
	int Offset = shmDataAddr - (long)&g_TxCNCData.shmCNCData;
	//printf("Offset = %d  nLength = %d\n",Offset,nLength);
	WriteValue(setDataAddr,Offset,nLength);
}


/*--------------------------------------------------------------------------+
|  Function : WriteValue(int nType, int nElements, int pIndexs[], void* pEx)|
|  Task     : 多筆資料寫                                                    |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::WriteValue(int nType, int nElements, int pIndexs[], void* pEx)
{
	int nResult = 0, i = 0;
	if(nType == 0)
	{
		for(i = 0; i < nElements; i++)
		{
			;
		}
	}
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)|
|  Task     : 多筆資料寫                                                    |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
{
	int nResult = 0;
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :WriteValue(void* pInputData, int nOffset, int nLength)			|
|  Task     : 資料通過offset寫                                              |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                  |
+--------------------------------------------------------------------------*/
int		CTaskCNC::WriteValue(void* pInputData, int nOffset, int nLength)
{
	int	nResult = 0;
	memcpy(&g_TxCNCData, &nOffset, sizeof(nOffset));
	memcpy(((char*)&g_TxCNCData)+sizeof(nOffset), &nLength, sizeof(nLength));
	memcpy(((char*)&g_TxCNCData)+CNC_HEAD_OFFSET_LENGTH + nOffset, pInputData, nLength);
	if(pInputData != NULL && nLength > 0)
	{
		SendData(m_nID, (char *)&g_TxCNCData, 0);
		EtherNet_WriteValue(HMI2NC_UPDATE,nOffset,nLength,(void*)pInputData);		//ethernet
		nResult = nLength;
	}
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :ReadValue(void* pInputData, int nOffset, int nLength)			|
|  Task     : 資料通過offset讀                                              |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::ReadValue(void* pInputData, int nOffset, int nLength)
{
	int	nResult = 0;
	if(pInputData != NULL && nLength > 0)
	{
		memcpy(pInputData, ((char*)&g_pRxCNCData) + nOffset, nLength);
		nResult = nLength;
	}
	return nResult;
}

/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::FindParamType(char *pszID)							|
|  Task     : 查找資料屬于哪種類型                                          |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::FindParamType(char *pszID)
{
	int			nType 	= 0, nDataID = 0;
	WORD		wIndex	= 0;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	if (pszID == NULL)
		return nType;
	
	Trim(cszStrID, pszID, sizeof(cszStrID));
	if(g_pDatabase != NULL)
	{
		nDataID = g_pDatabase->GetDataID(cszStrID);
	}
	nType = FindParamType(nDataID);
	return nType;
}

/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::FindParamType(int nDataID)							|
|  Task     : 查找param資料屬于哪種類型                                     |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskCNC::FindParamType(int nDataID)
{
	int			nType = 0, i = 0;
	
	if(nDataID < m_nParamID[0][0] || nDataID > m_nParamID[ID_COMMON-1][PARAMTYPE_COUNT-1])
	{
		return nType;
	}
	
	for(i = 0; i < PARAMTYPE_NUB; i++)
	{
		//如果查找到對應的type，則返回
		if(nDataID >= m_nParamID[i][0] && nDataID <= m_nParamID[i][1])
		{
			nType = ID_SERVO + i;
			break;
		}
	}
	return nType;
}

/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::Init()												|
|  Task     : Init					                                        |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CTaskCNC::Init()
{
	int	nLocalCount = 0, nGlobalCount = 0;
	DB_DATAATTRIBUTE*	pDataAttr = NULL;
	DWORD	dwOffset = 0;
	char	szCNC_mem_part[INT_FILE_LEN];
	szCNC_mem_part[0]='\0';
	
	
	
	ReadRobotData();
	if(cfg.Machine==SIXROBOT)
	{
	  CtmConfig::GetInstance()->SetFormPath("./res_tm640/form_sixjoint/");
	  CtmConfig::GetInstance()->SetToolBarPath("./res_tm640/toolbar_sixjoint/");
	}
	else
	{
	  CtmConfig::GetInstance()->SetFormPath("./res_tm640/form/");
	  CtmConfig::GetInstance()->SetToolBarPath("./res_tm640/toolbar/");
	}
	
#ifdef    TCP_HMI_NC
	while(Ethernet_Break==-1)
	{
		printf("wait ethernet start\n");
		usleep(50*1000);
	}
#endif
	memset(&g_TrackPoint_EndPoint, 0, sizeof(TRACKPOINT) );	//20110329 add
	
	long start_encoder_value[6];
	start_encoder_value[0] = GetDBValue("CNC_DIS_TO_GO_X").lValue;
	start_encoder_value[1] = GetDBValue("CNC_DIS_TO_GO_Y").lValue;
	start_encoder_value[2] = GetDBValue("CNC_DIS_TO_GO_Z").lValue;
	start_encoder_value[3] = GetDBValue("CNC_DIS_TO_GO_A").lValue;
	start_encoder_value[4] = GetDBValue("CNC_DIS_TO_GO_B").lValue;
	start_encoder_value[5] = GetDBValue("CNC_DIS_TO_GO_C").lValue;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_start_encoder_value,&start_encoder_value,sizeof(long)*6);
	
	SetDBValue("CNC_ACTION_ID", ID_NO_ACTION);
	
	SetDBValue("READ_FINISH_FLAG", 0);
	
	SetDBValue("CNC_START_BLK_NO", 0);
	
	GetDBString("CNC_MEM_PART", szCNC_mem_part, sizeof(g_TxCNCData.shmCNCData.CNC_mem_part));
	
	GetDBString("CNC_BLOCK_BUFFER", g_szProgDirectory, sizeof(g_szProgDirectory));
	if( (g_szProgDirectory[0]== '.' && g_szProgDirectory[1]== '/') || 
		(g_szProgDirectory[0]== '/' && g_szProgDirectory[1]== '/'))
		;
	else	//20120502 reuse if, open last ProgDirectory File when init
	{
		strcpy(g_szProgDirectory,"./Prog");
		SetDBString("CNC_BLOCK_BUFFER", g_szProgDirectory,sizeof(g_szProgDirectory));
	}
	
	ReadOperationFile(szCNC_mem_part);
	//SetDBString("CNC_MDI_PART", "o-mdi",sizeof("o-mdi"));//20110510 MDI file name
	
	if(g_pDatabase != NULL)
	{
		//初始化Param Type數組
		m_nParamID[ID_SERVO-1][0] 	= g_pDatabase->GetDataID(CNC_SERVO_PARAM_NAME_0);
		m_nParamID[ID_SERVO-1][1] 	= g_pDatabase->GetDataID(CNC_SERVO_PARAM_NAME_MAX);
		
		m_nParamID[ID_TUNE-1][0] 	= g_pDatabase->GetDataID(CNC_TUNE_PARAM_NAME_0);
		m_nParamID[ID_TUNE-1][1] 	= g_pDatabase->GetDataID(CNC_TUNE_PARAM_NAME_MAX);
		
		m_nParamID[ID_MAKER-1][0] 	= g_pDatabase->GetDataID(CNC_MAKER_PARAM_NAME_0);
		m_nParamID[ID_MAKER-1][1] 	= g_pDatabase->GetDataID(CNC_MAKER_PARAM_NAME_MAX);
		
		m_nParamID[ID_USER-1][0] 	= g_pDatabase->GetDataID(CNC_USER_PARAM_NAME_0);
		m_nParamID[ID_USER-1][1] 	= g_pDatabase->GetDataID(CNC_USER_PARAM_NAME_MAX);
		
		m_nParamID[ID_COMMON-1][0] 	= g_pDatabase->GetDataID(CNC_COMMON_PARAM_NAME_0);
		m_nParamID[ID_COMMON-1][1] 	= g_pDatabase->GetDataID(CNC_COMMON_PARAM_NAME_MAX);
		
		//Init macro_local_var macro_global_var data send
		m_nInitSendID[0][0]	=	g_pDatabase->GetDataID(CNC_MACRO_LOCAL_VAR_0);
		m_nInitSendID[0][1]	=	g_pDatabase->GetDataID(CNC_MACRO_LOCAL_VAR_MAX);
		
		//printf("ID1=%d, ID2=%d ", m_nInitSendID[0][0], m_nInitSendID[0][1]);
		m_nInitSendID[1][0]	=	g_pDatabase->GetDataID(CNC_MACRO_GLOBAL_VAR_0);
		m_nInitSendID[1][1]	=	g_pDatabase->GetDataID(CNC_MACRO_GLOBAL_VAR_MAX);
		
		nLocalCount = m_nInitSendID[0][1] - m_nInitSendID[0][0];
		nGlobalCount = m_nInitSendID[1][1] - m_nInitSendID[1][0];
				
		//Init g_TxCNCData
		memset(&g_TxCNCData, 0, sizeof(CNC_TXDATA));	//2012/7/25 add
		
		//2012/7/25 modify <<<<<
		//Init Send All TxData
		//g_pDatabase->ReadBatch(((char*)&g_TxCNCData)+CNC_HEAD_OFFSET_LENGTH, 0, m_TxSize - CNC_HEAD_OFFSET_LENGTH -sizeof(LONG_AXIS)*3\
		//                        -sizeof(int)*2-sizeof(char)*INT_BUFFER_SIZE*NUM_OF_BLOCK*2-sizeof(int)*4);
		g_pDatabase->ReadBatch(((char*)&g_TxCNCData)+CNC_HEAD_OFFSET_LENGTH, 0, (char*)&g_TxCNCData.shmCNCData.CNC_goto_tp_no -(char*)&g_TxCNCData.shmCNCData);
		//2012/7/25 modify >>>>>
		
		WriteValue((int)0, (void*)NULL, (int)0);
		EtherNet_WriteValue(HMI2NC_UPDATE,0,sizeof(g_TxCNCData.shmCNCData),(void*)&g_TxCNCData.shmCNCData);		//ethernet
		#ifdef    TCP_HMI_NC
			UpdateProgToNC(NULL);
			UpdateFileToNC("./Data/TP/TEACHPOINT");
			UpdateFileToNC("./Data/DHParam.dat");
		#endif
		//讀取Rx的起始Offset
		pDataAttr = g_pDatabase->GetAttr();
		if(pDataAttr != NULL)
		{
			int nIndex= g_pDatabase->GetIndex(CNC_READ_STARTID_SAVETODB);
			int nIndexEnd =g_pDatabase->GetIndex(CNC_READ_ENDID_SAVETODB);
			if(nIndex >=0)
				m_ReadStartOffset = pDataAttr[nIndex].dwOffset;
			else
				m_ReadStartOffset = m_TxSize;
			if (nIndexEnd >=0)	m_ReadEndOffset = pDataAttr[nIndexEnd].dwOffset;
		}
	} 
	SetDBValue("PARAM_UPDATE_ID", ID_ALL_PARAM); 
	
	//2015/5/13 上午 10:44:06
	char temp = 0;
	//temp[0] = '0';
	
	//初始化
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_status));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_auto_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_auto_status));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_pos_calibrate,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_pos_calibrate));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_pos_zero,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_pos_zero));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_machine_lock,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_machine_lock));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_stop_action_OK,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_stop_action_OK));
	
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_cycle_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_cycle_status));
	WORD ans = SetDBString("CNC_CYCLE_STATUS",&temp,1);
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_single_block,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_single_block));
	SetDBString("CNC_SINGLE_BLOCK",&temp,1);
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_option_stop,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_option_stop));
	SetDBString("CNC_OPTION_STOP",&temp,1);
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_block_skip,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_block_skip));
	SetDBString("CNC_READ_BLOCK_SKIP_FLAG",&temp,1);
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_mode,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
	SetDBString("CNC_JOG_MODE",&temp,1);
	if(g_TxCNCData.shmCNCData.CNC_work_coord_no==0)
		SetDBValue("CNC_WORK_COORD_NO", 1);	
	if(g_TxCNCData.shmCNCData.CNC_tool_coord_no==0)
		SetDBValue("CNC_TOOL_COORD_NO", 1);
	//if(g_pTxCNCData != NULL) g_pTxCNCData->CNC_offset_tool_no = g_TxCNCData.shmCNCData.CNC_offset_tool_no = 0;	//20110513 modify
	//g_TxCNCData.shmCNCData.CNC_offset_tool_no =0;
	u_build_queue=1;
	EtherNet_WriteValue(HMI2NC_INI_OK,0,0,(void*)NULL);		//初始化完成
	//Init Send All TxData
	iniok = 1;  
	return 1;
}

/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::ReadOperationFile()									                |
|  Task     : ReadOperationFile		                                          |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
BOOL/*void*/	CTaskCNC::ReadOperationFile(char *pszFileName, BOOL bFileContentModify, BOOL bWholeDirectory)	//Sunny<20100428>
{
#ifndef	D_TEST
	FILE *fp;
 	char szFile[6]					="\0";
 	char szFullFile[128/*40*/]		="\0";//20120411 modify buffer
 	char szCommand[128/*32*/]		="\0";//20120411 modify buffer
 	static char szFile_Last[128/*40*/]="\0";//20120411 modify buffer
 	//static char szFile_Last_MDI[6]	="\0";
	static WORD wOperMode_old 		= 0; 
	//printf("ReadOperationFile--%s\n",pszFileName);


 	if(bWholeDirectory == FALSE)
 	{
 		if((pszFileName == NULL) || (strcmp(pszFileName,"0")==0))	sprintf(szFullFile, "./Prog/O0001");
 		else sprintf(szFullFile, "%s/%s", g_szProgDirectory,pszFileName);
 	}
 	else	strcpy(szFullFile,pszFileName);
	//printf("szFullFile:%s  szFile_Last:%s \n",szFullFile,szFile_Last);
 	if((strcmp(szFile_Last,szFullFile)==0) && (bFileContentModify == FALSE) && (wOperMode_old == g_TxCNCData.shmCNCData.CNC_mode_lgt))	return FALSE;	//20111205 add && (wOperMode_old == g_TxCNCData.shmCNCData.CNC_mode_lgt)
 	else	strcpy(szFile_Last,szFullFile);
 			
		
 	g_OpFileStringList.LoadFromFile(szFullFile);
 	g_nblock_no = g_OpFileStringList.GetNum();
 		
 	static FILE *fpt = NULL;
 	sprintf(szCommand, "basename %s >TempFile.lis", szFullFile);
	system(szCommand);
	usleep(50*1000);
	g_TempStringList.LoadFromFile("TempFile.lis");
	strcpy(g_TxCNCData.shmCNCData.CNC_mem_part,g_TempStringList.IndexOf(0));
		
 	SetDBString("CNC_MEM_PART", g_TxCNCData.shmCNCData.CNC_mem_part,sizeof(g_TxCNCData.shmCNCData.CNC_mem_part));
	WriteValue("CNC_MEM_PART");//Sunny<20101123> add
	SetDBValue("CNC_READ_BLOCK_RESULT", -999);//Read File Finish
	SetDBValue("CNC_START_BLK_NO", 0);//Sunny<20101123> add
	u_build_queue=1;//When File Changed, Rebuid the queue.wilson lu, 2010.12.21
		
	//20111122 add<<<<<
	sprintf(szCommand, "dirname %s >TempFile.lis", szFullFile);
	system(szCommand);
	usleep(50*1000);
	g_TempStringList.LoadFromFile("TempFile.lis");
	strcpy(g_szProgDirectory,g_TempStringList.IndexOf(0));
	//printf("3ReadOperationFile--%s g_szProgDirectory:%s \n",szFullFile,g_szProgDirectory);
	SetDBString("CNC_BLOCK_BUFFER", g_szProgDirectory,sizeof(g_szProgDirectory));
	//20111122 add>>>>>
	if(g_pRxCNCData!= NULL) wOperMode_old = g_TxCNCData.shmCNCData.CNC_mode_lgt;
	return TRUE;
#else
	return FALSE;
#endif
}

/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::SaveParaToFile()										|
|  Task     : SaveParaToFile                                        		|
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:   	                 	-                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CTaskCNC::SaveParaToFile()//Sunny<20100630>
{
	FILE 		*fp;
	fp = fopen("TxCNCPara.log","w");
	WORD		wParaNo =0;	

	for(int i = m_nParamID[ID_SERVO-1][0]; i <= m_nParamID[ID_SERVO-1][1] ; i++ )
	{
		wParaNo =i-m_nParamID[ID_SERVO-1][0];
		fprintf(fp, " SERVO_%03d Value=%d\n",wParaNo,GetDBValue(i).lValue);	
	}
	for(int i = m_nParamID[ID_TUNE-1][0]; i <= m_nParamID[ID_TUNE-1][1] ; i++ )
	{
		wParaNo =i-m_nParamID[ID_TUNE-1][0];
        fprintf(fp, "  TUNE_%03d Value=%d\n",wParaNo,GetDBValue(i).lValue);	
	}
	for(int i = m_nParamID[ID_MAKER-1][0]; i <= m_nParamID[ID_MAKER-1][1] ; i++ )
	{
		wParaNo =i-m_nParamID[ID_MAKER-1][0];
        fprintf(fp, " MAKER_%03d Value=%d\n",wParaNo,GetDBValue(i).lValue);	
	}
	for(int i = m_nParamID[ID_USER-1][0]; i <= m_nParamID[ID_USER-1][1] ; i++ )
	{
		wParaNo =i-m_nParamID[ID_USER-1][0];
        fprintf(fp, "  USER_%03d Value=%d\n",wParaNo,GetDBValue(i).lValue);	
	}
	for(int i = m_nParamID[ID_COMMON-1][0]; i <= m_nParamID[ID_COMMON-1][1] ; i++ )
	{
		wParaNo =i-m_nParamID[ID_COMMON-1][0];
        fprintf(fp, "COMMON_%03d Value=%d\n",wParaNo,GetDBValue(i).lValue);	
	}
	fclose(fp);
}
	
void	CTaskCNC::Alarm_Control()//Sunny<20100714>
{
#ifndef	D_TEST
	static int iCNC_alarm_id_old =-1,iCNC_alarm_group_old =-1;
	
	if((g_pRxCNCData->CNC_alarm_id != 0 )&&(g_pRxCNCData->CNC_alarm_group != 0 ))
	{
		if((iCNC_alarm_group_old != g_pRxCNCData->CNC_alarm_group)||(iCNC_alarm_id_old != g_pRxCNCData->CNC_alarm_id))
		{
			//printf("-----------------ALARM-----------------\n");
			g_ptmControlServer->ErrorControl()->Robot_AddErrorLog(g_pRxCNCData->CNC_alarm_id,g_pRxCNCData->CNC_alarm_group);
			iCNC_alarm_id_old 	= g_pRxCNCData->CNC_alarm_id;
			iCNC_alarm_group_old= g_pRxCNCData->CNC_alarm_group;
		}
		SendMsg(MSG_USER_SH_ERROR_ADD, 0xFFFFFFFF, 0xFFFFFFFF, NULL);
	}
	
	if((g_pRxCNCData->CNC_alarm_id == 0)&&(iCNC_alarm_id_old != -1))	//Clear every Alarm only once
	{
		SendMsg(MSG_USER_SH_ERROR_FIX, 0xFFFFFFFF, 0xFFFFFFFF , NULL);
		iCNC_alarm_id_old 		=-1;
		iCNC_alarm_group_old 	=-1;
		int temp = 0;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
	}
#endif
}

void	CTaskCNC::Motion_DB_Modify()//Sunny<20100813><<<<<
{	
#ifndef	D_TEST
	DB_DATAATTRIBUTE*	pDataAttr = NULL;
	pDataAttr = g_pDatabase->GetAttr();
	static WORD	wStartDBIndex =0;
	static long CNC_machineX=-1,CNC_machineY=-1,CNC_machineZ=-1,CNC_machineC=-1,CNC_machineA=-1; //wilson lu,2010.1210
	static long CNC_absoluteX=-1,CNC_absoluteY=-1,CNC_absoluteZ=-1,CNC_absoluteC=-1,CNC_absoluteA=-1; //wilson lu,2010.1210
	
	g_TxCNCData.shmCNCData.CNC_auto_status = g_pTxCNCData->CNC_auto_status;
	
	
	if(pDataAttr != NULL)
	{
		static DWORD dwOffset1 = pDataAttr[g_pDatabase->GetIndex("CNC_MACHINE_X")].dwOffset;
		g_pDatabase->WriteBatch(&(g_pRxCNCData->CNC_machine), dwOffset1, (sizeof(LONG_AXIS))*3+sizeof(int),TRUE);
		
		LONG_AXIS	Encoder_Value;
		static DWORD dwOffset2 = pDataAttr[g_pDatabase->GetIndex("CNC_DIS_TO_GO_X")].dwOffset;		//暫時以此紀錄encoder值

		Encoder_Value.x = g_pRxCNCData->CNC_encoder_value[0];
		Encoder_Value.y = g_pRxCNCData->CNC_encoder_value[1];
		Encoder_Value.z = g_pRxCNCData->CNC_encoder_value[2];
		Encoder_Value.a = g_pRxCNCData->CNC_encoder_value[3];
		Encoder_Value.b = g_pRxCNCData->CNC_encoder_value[4];
		Encoder_Value.c = g_pRxCNCData->CNC_encoder_value[5];
		if(Encoder_Value.x != 0 || Encoder_Value.y != 0 || Encoder_Value.z != 0 || Encoder_Value.a != 0 || Encoder_Value.b != 0 || Encoder_Value.c != 0 )
		{
			if(iniok==1)
			{
				g_pDatabase->WriteBatch(&(Encoder_Value),dwOffset2,sizeof(LONG_AXIS),TRUE);
			}
		}
		static DWORD 	dwMacro_Sys_Var0_Offset = pDataAttr[g_pDatabase->GetIndex("MACRO_SYSTEM_VAR0")].dwOffset;
		if(g_pRxCNCData->macro_system_var_set_no != 0)
		{
			g_pDatabase->WriteBatch(&g_pRxCNCData->macro_system_var[0], dwMacro_Sys_Var0_Offset, sizeof(double)*INT_MACRO_SYS_PARA_NO,TRUE);
			g_pRxCNCData->macro_system_var_set_no = 0;
		}
		//2012/7/23 Read Macro_Sys_Param Form NC and write to DB >>>>>
	}
	if(g_pRxCNCData->CNC_set_work_shift_flag==1) 
	{
		g_pTxCNCData->CNC_work_shift_flag=g_TxCNCData.shmCNCData.CNC_work_shift_flag=g_pRxCNCData->CNC_work_shift_flag;
		SetDBValue("WORK_SHIFT_FLAG",g_pRxCNCData->CNC_work_shift_flag);
	}

	if(g_pRxCNCData->CNC_inform_stop == 1 && g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_RUN && g_TxCNCData.shmCNCData.CNC_stop_action_OK == 0)
	{
		char temp = RUN_STATUS_PAUSE;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_auto_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_auto_status));
		temp = 1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_stop_action_OK,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_stop_action_OK));
	}
	
	if(g_pRxCNCData->CNC_inform_stop == 0 && g_TxCNCData.shmCNCData.CNC_stop_action_OK == 1)
	{
		char temp = 0;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_stop_action_OK,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_stop_action_OK));
	}

//20110829 add<<<<<<<<
	if(g_pRxCNCData->CNC_absolute_reset.x==1 && g_TxCNCData.shmCNCData.CNC_absolute_reset.x==1)	SetDBValue("CNC_ABSOLUTE_RESET_X", 0);
	if(g_pRxCNCData->CNC_absolute_reset.y==1 && g_TxCNCData.shmCNCData.CNC_absolute_reset.y==1)	SetDBValue("CNC_ABSOLUTE_RESET_Y", 0);
	if(g_pRxCNCData->CNC_absolute_reset.z==1 && g_TxCNCData.shmCNCData.CNC_absolute_reset.z==1)	SetDBValue("CNC_ABSOLUTE_RESET_Z", 0);
	if(g_pRxCNCData->CNC_absolute_reset.c==1 && g_TxCNCData.shmCNCData.CNC_absolute_reset.c==1)	SetDBValue("CNC_ABSOLUTE_RESET_C", 0);
	if(g_pRxCNCData->CNC_absolute_reset.a==1 && g_TxCNCData.shmCNCData.CNC_absolute_reset.a==1)	SetDBValue("CNC_ABSOLUTE_RESET_A", 0);
	
//20110829 add>>>>>>>>
#endif
}//Sunny<20100812>>>>>>

/*--------------------------------------------------------------------------+
運動軌跡功能相關函數
1、SaveCurveScreen()
	保存當前曲線畫面；
2、AddTrackPoint(TRACKPOINT*	pTrackPoint, int nIndex)
	保存當前軌跡點；
3、GetAndDrawTrackPoint(CtmWnd* pwndSender, WORD wCoordinateMode)
	獲取之前保存的歷史點，并重新畫軌跡曲線，主要用於曲線放大功能；
4、FreeTrackPoint()
	釋放保存軌跡曲線點的鏈錶；
5、DrawTrackPoint(CtmWnd* pwndSender,double dPoint_X,double dPoint_Y,unsigned short Color,WORD wCurveStyle,WORD wCoordinateMode)
	最終實現畫軌跡曲線的地方
+--------------------------------------------------------------------------*/
/*int		CTaskCNC::SaveCurveScreen(WORD wViewTop,WORD ViewBottom)
{
	int			nLength, i, j;
	char		*pBMP;
	BYTE		*pRed, *pGreen, *pBlue;
	BITMAPFILEHEADER	*pHeader;
	BITMAPINFOHEADER	*pInfo;
	WORD				Pixel;
	WORD				*pwPixel;	
	char				cPixel;
	WORD				wCurveViewHeight = ViewBottom - wViewTop;
	
	nLength	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
				+ wCurveViewHeight * g_infoDrv.nX * 3;
	
	pBMP	= (char *)malloc(nLength);
	
	pHeader	= (BITMAPFILEHEADER	*)pBMP;
	pInfo	= (BITMAPINFOHEADER	*)(pBMP + sizeof(BITMAPFILEHEADER));
	
	pHeader->bfType			= 0x4D42;
	pHeader->bfSize			= nLength; 				// 位圖文件的大小，以字節為單位 
	pHeader->bfReserved1	= 0; 					// 位圖文件保留字，必須為0 
	pHeader->bfReserved2	= 0; 					// 位圖文件保留字，必須為0
	pHeader->bfOffBits		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 				// 位圖數据的起始位置，以相對于位圖
	
	pInfo->biSize			= sizeof(BITMAPINFOHEADER); 				// 本結构所占用字節數 
	pInfo->biWidth			= g_infoDrv.nX; 				// 位圖的寬度，以像素為單位 
	pInfo->biHeight			= wCurveViewHeight; 				// 位圖的高度，以像素為單位 
	pInfo->biPlanes			= 1; 				// 目標設備的級別，必須為1 
	pInfo->biBitCount		= 24;				// 每個像素所需的位數，必須是1(雙色), 4(16色)，8(256色)或24(真彩色)之一 
	pInfo->biCompression	= 0; 			// 位圖壓縮類型，必須是 0(不壓縮), 1(BI_RLE8壓縮型)或2(BI_RLE4壓縮型)之一 
	pInfo->biSizeImage		= wCurveViewHeight * g_infoDrv.nX * 3; 			// 位圖的大小，以字節為單位 
	pInfo->biXPelsPerMeter	= 0x00; 		// 位圖水平分辨率，每米像素數 
	pInfo->biYPelsPerMeter	= 0x00; 		// 位圖垂直分辨率，每米像素數 
	pInfo->biClrUsed		= 0x00;				// 位圖實際使用的顏色表中的顏色數	
	pInfo->biClrImportant	= 0x00;			// 位圖顯示過程中重要的顏色數
	
	pRed	= (BYTE *)(pBMP + pHeader->bfOffBits + 2);
	pGreen	= (BYTE *)(pBMP + pHeader->bfOffBits + 1);
	pBlue	= (BYTE *)(pBMP + pHeader->bfOffBits );
	pwPixel		= (WORD *)g_infoDrv.pcMem;
	
	//for(j = g_infoDrv.nY - 1; j >= 0; j --)
	for(j = ViewBottom-1; j >= wViewTop; j --)
	{
		for(i = 0; i < g_infoDrv.nX; i ++)
		{
			Pixel	= *(pwPixel + j * g_infoDrv.nX + i);//g_Rows[j][i];
			
			*pRed	= (Pixel & 0xF800)	>> 8;
			*pGreen	= (Pixel & 0x7E0)	>> 3;
			*pBlue	= (Pixel & 0x1F) 	<< 3;
			
			pRed 	+= 3;
			pGreen 	+= 3;
			pBlue 	+= 3;
		}
	}
	
	int fd; 
	if ((fd = open("./res_tm640/CurveScreen.bmp", O_RDWR|O_CREAT)) < 0)
	{
		//return 0;
	}
	if(lseek(fd, 0, 0) < 0)		
		;
	if(write(fd, pBMP, nLength))
		;
	close(fd);
	free(pBMP);
	//g_pInit -> Init_Bmp();
	return 0;
}
*/
int		CTaskCNC::AddTrackPoint(TRACKPOINT*	pTrackPoint, int nIndex)
{
	//static char pszFileName[28] ="/work/DebugLogCurveData.log";
	//g_tmDebugInfo->PrintLogInfo(pszFileName,"X=%ld  Y=%ld  Z=%ld \n", pTrackPoint->lAxis.x, pTrackPoint->lAxis.y, pTrackPoint->lAxis.z);

	if(ptmTrackPoint_Head == NULL)
	{
		ptmTrackPoint_Head	= new TRACKPOINT;
		if(pTrackPoint != NULL)
		{
			ptmTrackPoint_Head->G_group1	= pTrackPoint->G_group1;
			ptmTrackPoint_Head->lAxis		= pTrackPoint->lAxis;
			ptmTrackPoint_Head->pNext		= NULL;
		}
		ptmTrackPoint_Temp = ptmTrackPoint_Head;
	}
	else
	{
		if(nIndex == 0)		//順序插入
		{
			if(ptmTrackPoint_Temp != NULL)
			{
				while(ptmTrackPoint_Temp->pNext != NULL)
				{
					ptmTrackPoint_Temp = ptmTrackPoint_Temp->pNext;
				}
				ptmTrackPoint_Temp->pNext 			= new TRACKPOINT;
				ptmTrackPoint_Temp 					= ptmTrackPoint_Temp->pNext;
				ptmTrackPoint_Temp->G_group1		= pTrackPoint->G_group1;
				ptmTrackPoint_Temp->lAxis			= pTrackPoint->lAxis;
				ptmTrackPoint_Temp->pNext			= NULL;
			}
		}
		else
		{
				
		}
	}
	tmTrackPoint_Max.x = (tmTrackPoint_Max.x > pTrackPoint->lAxis.x) ? tmTrackPoint_Max.x : pTrackPoint->lAxis.x;
	tmTrackPoint_Max.y = (tmTrackPoint_Max.y > pTrackPoint->lAxis.y) ? tmTrackPoint_Max.y : pTrackPoint->lAxis.y;
	tmTrackPoint_Max.z = (tmTrackPoint_Max.z > pTrackPoint->lAxis.z) ? tmTrackPoint_Max.z : pTrackPoint->lAxis.z;
	
	tmTrackPoint_Min.x = (tmTrackPoint_Min.x < pTrackPoint->lAxis.x) ? tmTrackPoint_Min.x : pTrackPoint->lAxis.x;
	tmTrackPoint_Min.y = (tmTrackPoint_Min.y < pTrackPoint->lAxis.y) ? tmTrackPoint_Min.y : pTrackPoint->lAxis.y;
	tmTrackPoint_Min.z = (tmTrackPoint_Min.z < pTrackPoint->lAxis.z) ? tmTrackPoint_Min.z : pTrackPoint->lAxis.z;
	g_wCurve_Point_Save_Cnt++;
	return 0;
}
/*
void	CTaskCNC::GetAndDrawTrackPoint(CtmWnd* pwndSender, WORD wXStart, WORD wYStart, WORD wLength, WORD wHeight, WORD wCoordinateMode)
{
	TRACKPOINT* pTemp = ptmTrackPoint_Head;
	unsigned	short Color =0;						  	
	double		dZoomScale_X =0, dZoomScale_Y =0, dZoomScale =0;
	double		dPoint_X=0, dPoint_Y =0;
	long		lCurve_Max_X=0, lCurve_Max_Y=0,lCurve_Min_X=0, lCurve_Min_Y=0;
	
	if(g_wCurve_Horizontal_Axis == g_wCurve_Vertical_Axis)	return;
	
	if(wCoordinateMode & TRACK_CURVE_ZOOMIN)//Zoom In
	{
		if((g_wCurve_Horizontal_Axis ==0)&&(g_wCurve_Vertical_Axis ==2))	//XZ
		{
			dZoomScale_X = (double)wLength /(tmTrackPoint_Max.x - tmTrackPoint_Min.x);
			dZoomScale_Y = (double)wHeight /(tmTrackPoint_Max.z - tmTrackPoint_Min.z);
			lCurve_Min_X = tmTrackPoint_Min.x;
			lCurve_Max_Y = tmTrackPoint_Max.z;
		}
		else if((g_wCurve_Horizontal_Axis ==1)&&(g_wCurve_Vertical_Axis ==2))	//YZ
		{
			dZoomScale_X = (double)wLength /(tmTrackPoint_Max.y - tmTrackPoint_Min.y);
			dZoomScale_Y = (double)wHeight /(tmTrackPoint_Max.z - tmTrackPoint_Min.z);
			lCurve_Min_X = tmTrackPoint_Min.y;
			lCurve_Max_Y = tmTrackPoint_Max.z;
		}
		else	//XY
		{
			dZoomScale_X = (double)wLength /(tmTrackPoint_Max.x - tmTrackPoint_Min.x);
			dZoomScale_Y = (double)wHeight /(tmTrackPoint_Max.y - tmTrackPoint_Min.y);
			lCurve_Min_X = tmTrackPoint_Min.x;
			lCurve_Max_Y = tmTrackPoint_Max.y;
		}
		
	}
	else if(wCoordinateMode & TRACK_CURVE_NORMAL)//正常顯示
	{
		if((g_wCurve_Horizontal_Axis ==0)&&(g_wCurve_Vertical_Axis ==2))	//XZ
		{
			dZoomScale_X = (double)wLength/(GetDBValue("TUNE_PARAM106").lValue - GetDBValue("TUNE_PARAM107").lValue);
			dZoomScale_Y = (double)wHeight/(GetDBValue("TUNE_PARAM110").lValue - GetDBValue("TUNE_PARAM111").lValue);
			lCurve_Min_X = GetDBValue("TUNE_PARAM107").lValue;
			lCurve_Max_Y = GetDBValue("TUNE_PARAM110").lValue;
		}
		else if((g_wCurve_Horizontal_Axis ==1)&&(g_wCurve_Vertical_Axis ==2))	//YZ
		{
			dZoomScale_X = (double)wLength/(GetDBValue("TUNE_PARAM108").lValue - GetDBValue("TUNE_PARAM109").lValue);
			dZoomScale_Y = (double)wHeight/(GetDBValue("TUNE_PARAM110").lValue - GetDBValue("TUNE_PARAM111").lValue);
			lCurve_Min_X = GetDBValue("TUNE_PARAM109").lValue;
			lCurve_Max_Y = GetDBValue("TUNE_PARAM110").lValue;
		}
		else	//XY
		{
			dZoomScale_X = (double)wLength/(GetDBValue("TUNE_PARAM106").lValue - GetDBValue("TUNE_PARAM107").lValue);
			dZoomScale_Y = (double)wHeight/(GetDBValue("TUNE_PARAM108").lValue - GetDBValue("TUNE_PARAM109").lValue);
			lCurve_Min_X = GetDBValue("TUNE_PARAM107").lValue;
			lCurve_Max_Y = GetDBValue("TUNE_PARAM108").lValue;
		}
	}
	dZoomScale  =(dZoomScale_X < dZoomScale_Y) ? dZoomScale_X : dZoomScale_Y;
	
	//static BOOL u_bFirstPointFlag = TRUE;
	
	long lHorizontal_Value =0, lVertical_Value =0;
	
	static BOOL bEndPoint = FALSE;
	while(pTemp != NULL)
	{
		if(pTemp->G_group1== 0 )Color = 0;//65503;//WHITE;//G00 空跑
		else					Color = MAGENTA;//BLUE;//MAGENTA;
			
		if((g_wCurve_Horizontal_Axis ==0)&&(g_wCurve_Vertical_Axis ==2))	//XZ
		{
			lHorizontal_Value	= pTemp->lAxis.x;
			lVertical_Value		= pTemp->lAxis.z;
		}
		else if((g_wCurve_Horizontal_Axis ==1)&&(g_wCurve_Vertical_Axis ==2))	//YZ
		{
			lHorizontal_Value	= pTemp->lAxis.y;
			lVertical_Value		= pTemp->lAxis.z;
		}
		else 	//XY
		{
			lHorizontal_Value	= pTemp->lAxis.x;
			lVertical_Value		= pTemp->lAxis.y;
		}

		dPoint_X = wXStart +(lHorizontal_Value - lCurve_Min_X)*dZoomScale;
		dPoint_Y = wYStart +(lCurve_Max_Y - lVertical_Value)*dZoomScale;//Y軸正方向朝上，剛好與座標大小相反，so
		//printf("EndPoint--X:%ld Y:%ld \n",pTemp->lAxis.x,pTemp->lAxis.y);
		DrawTrackPoint(pwndSender, dPoint_X, dPoint_Y, Color,u_bFirstPointFlag);
		
		u_bFirstPointFlag = FALSE;
		pTemp = pTemp->pNext;
		
		bEndPoint = TRUE;
	};
	if(bEndPoint == TRUE)
	{
		DrawTrackPoint(pwndSender, dPoint_X, dPoint_Y, YELLOW,u_bFirstPointFlag);
		bEndPoint = FALSE;
	}
}
*/
int		CTaskCNC::FreeTrackPoint()
{
	TRACKPOINT* pTemp = ptmTrackPoint_Head;
	while(pTemp != NULL)
	{
		ptmTrackPoint_Head = ptmTrackPoint_Head->pNext;
		delete pTemp;
		//printf("free i=%d \n", i++);
		pTemp = ptmTrackPoint_Head;
	}
	memset(&tmTrackPoint_Max, 0, sizeof(tmTrackPoint_Max));
	memset(&tmTrackPoint_Min, 0, sizeof(tmTrackPoint_Min));
	g_wCurve_Point_Save_Cnt =0;
}

void	CTaskCNC::DrawTrackPoint(CtmWnd* pwndSender,double dPoint_X,double dPoint_Y,unsigned short Color,BOOL bFirstPointFlag,WORD wCoordinateMode)
{
	static	double	dPoint_X_Old = 0, dPoint_Y_Old = 0; 
	static	BOOL	bCurveStart = FALSE;
	
	_SetViewPort(pwndSender->GetWindowRECT().left, pwndSender->GetWindowRECT().top, pwndSender->GetWindowRECT().right, pwndSender->GetWindowRECT().bottom);
	_SetColor(Color);
	//printf("DrawTrackPoint----bFirstPointFlag:%d x:%f Y:%f\n",bFirstPointFlag,dPoint_X,dPoint_Y);
	
    if(!bFirstPointFlag)
    {
    	if(bCurveStart)
    		bCurveStart = FALSE;
    	else 
    	{
    		_MoveTo( dPoint_X_Old, dPoint_Y_Old);
    		//_MoveTo( dPoint_X, dPoint_Y);
    		_LineTo( dPoint_X, dPoint_Y);
    		
    		_SetColor(YELLOW);
    		_MoveTo( dPoint_X, dPoint_Y);
    		_LineTo( dPoint_X, dPoint_Y);
    	}
    	dPoint_X_Old = dPoint_X;
    	dPoint_Y_Old = dPoint_Y;
    }
	else bCurveStart = TRUE;
}


static inline void tsnorm(struct timespec *ts)
{
	while (ts->tv_nsec >= NSEC_PER_SEC) {
		ts->tv_nsec -= NSEC_PER_SEC;
		ts->tv_sec++;
	}
}

/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::SaveTrackEndPoint()									|
|  Task     : 單獨線程存儲各Block終點位置                                   |
|             處理Block的隊列部份移置此(20110831)                           |
+--------------------------------------------------------------------------*/
//creat thread使用OSADL的realtime task  20110831
void	CTaskCNC::SaveTrackEndPoint()
{
	//Mario mark  2015/8/5 下午 03:02:51
	//if(g_pRxCNCData == NULL)	return; 
	
	//Mario add 2015/8/5 下午 03:02:56 
	while(g_pRxCNCData == NULL) 
	{
		printf("------------Delay until g_pRxCNCData != NULL----------------------\n");
		Delay(500);
	}
	
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		perror("CRTAI::COMM_RxRun setcanceltype");
	}
	static 	char* ethernet_send_check=NULL;
	static	int				lCNC_Current_block_no_Old = -1;
	static	CNC_BLOCK_DATA	block; 		
	static	BOOL			bPointEnd_Old_Clear = FALSE; 
	static	INT_COORD5		Point_Zero ={0,0,0,0,0};
	static	LONG_AXIS		Machine_Old;
	//yangpeng 20110830 add
	cpu_set_t mask;
	struct sched_param schedp;
	int policy = SCHED_FIFO;
	struct timespec now, next, interval;
	int timermode = TIMER_ABSTIME;
	memset(&Machine_Old, 0, sizeof(LONG_AXIS));
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if(sched_setaffinity(0, sizeof(mask), &mask) == -1)
		fprintf(stderr,	"WARNING: Could not set CPU affinity " "to CPU #%d\n", 0);
	memset(&schedp, 0, sizeof(schedp));
	schedp.sched_priority = sched_get_priority_max(90);
	sched_setscheduler(0, policy, &schedp);
	interval.tv_sec = CNC_TASK_CYCLE_TIME / USEC_PER_SEC;
	interval.tv_nsec = (CNC_TASK_CYCLE_TIME % USEC_PER_SEC) * 1000;
	/* Get current time */
	clock_gettime(CLOCK_MONOTONIC, &now);
	next = now;
	next.tv_sec+=1;
	//printf("start readqueue task\n");

	while (true/*GetDBValue("CNC_ACTION_ID").lValue != ID_CNC_EXIT*/)
	{
		//yangpeng modify 20110830
		if(timermode == TIMER_ABSTIME)
		{
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
		}
		else
		{
			clock_gettime(CLOCK_MONOTONIC, &now);
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_RELTIME, &interval, NULL);
			next.tv_sec = now.tv_sec + interval.tv_sec;
			next.tv_nsec = now.tv_nsec + interval.tv_nsec;
			tsnorm(&next);
		}
		next.tv_sec += interval.tv_sec;
		next.tv_nsec += interval.tv_nsec;
		tsnorm(&next);
		//printf("next.tv_sec:%ld  next.tv_nsec:%ld \n",next.tv_sec,next.tv_nsec);
	#ifndef	D_TEST

		g_ptaskCNC->ReadQueue();
		if(ethernet_send_check == NULL) 
		{
			ethernet_send_check = (char*)malloc(sizeof(int)*4);
			memset(ethernet_send_check,0,sizeof(int)*4);
		}
		if(memcmp(ethernet_send_check,(char *)&g_pTxCNCData->CNC_queue1_status,sizeof(int)*4)!=0)
		{
			EtherNet_WriteValue(HMI2NC_UPDATE,(int)&g_pTxCNCData->CNC_queue1_status-(int)&g_pTxCNCData->param_update_id,sizeof(int)*4,(void*)&g_pTxCNCData->CNC_queue1_status);
			memcpy(ethernet_send_check,(char *)&g_pTxCNCData->CNC_queue1_status,sizeof(int)*4);
		}
	#endif
	}
	
	schedp.sched_priority = 0;
 	sched_setscheduler(0, SCHED_OTHER, &schedp);
}

/*void CTaskCNC::QueryCurrentCommand(int offset)
{
#ifndef	D_TEST
	if(g_pRxCNCData==NULL)	return;
		
	if(offset==1 && g_pRxCNCData->CNC_curr_blk_ptr==1)
	{
        memcpy( &g_CNC_block_data[offset].mdl, &g_pRxCNCData->CNC_curr_blk.modal, sizeof(INT_MODAL));
		memcpy( &g_CNC_block_data[offset].sgl, &g_pRxCNCData->CNC_curr_blk.single, sizeof(INT_SINGLE));
    }
    else if(offset==2 && g_pRxCNCData->CNC_next_blk_ptr==1)
	{
        memcpy( &g_CNC_block_data[offset].mdl, &g_pRxCNCData->CNC_next_blk.modal, sizeof(INT_MODAL));
		memcpy( &g_CNC_block_data[offset].sgl, &g_pRxCNCData->CNC_next_blk.single, sizeof(INT_SINGLE));
	}
    if(offset==3 && g_pRxCNCData->CNC_next_next_blk_ptr==1)
	{
        memcpy( &g_CNC_block_data[offset].mdl, &g_pRxCNCData->CNC_next_next_blk.modal, sizeof(INT_MODAL));
		memcpy( &g_CNC_block_data[offset].sgl, &g_pRxCNCData->CNC_next_next_blk.single, sizeof(INT_SINGLE));
	}
	return;
#endif
}*/


/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::MetricOrInch_System_Control()						|
|  Task     : 公英制轉換                                   					|
|             												                |
+--------------------------------------------------------------------------*/
void	CTaskCNC::MetricOrInch_System_Control()//20110908<<<<<
{/*
#ifndef	D_TEST
	static	int		g_nMetricOrInch_System_Old=-1,	nG_6=-1,	nG_6_old=-1;
	static	WORD	wDBIndex_Start	= g_pDatabase->GetIndex("CNC_OFFSET0_TYPE");
	//static	WORD	wDBIndex_End	= g_pDatabase->GetIndex("CNC_OFFSET99_WRADIUS_WEAR");//CNC_WORK_COORD25_A 2012/7/10 modify
	static	WORD	wDBIndex_End	= g_pDatabase->GetIndex("CNC_WORK_COORD25_A");// 2012/7/24 mark
	if(g_pRxCNCData != NULL)
	{
		//if((g_TxCNCData.shmCNCData.CNC_single_block ==1) || (g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MDI_MODE) || (g_pRxCNCData->CNC_status == 2 || g_pRxCNCData->CNC_status == 3)) //OperMDI or ActionStart/ActionFeedHold
		{	
			nG_6 = g_pRxCNCData->CNC_table2.nonmotion.last_inch_mm;//g_CNC_block_data[1].mdl.group[6]; 20111013 modify
    		if(nG_6 == 20 || nG_6==21)	//21-Metric  20-Inch
			{
				if(nG_6 != nG_6_old)
				{
					if(nG_6 == 20)	SetDBValue("TUNE_PARAM37",1);
					else			SetDBValue("TUNE_PARAM37",0);
						
					nG_6_old = nG_6;
				}
			}
		}
	}
	
	g_nMetricOrInch_System = GetDBValue("TUNE_PARAM37").lValue;
	
    if(g_nMetricOrInch_System != g_nMetricOrInch_System_Old)  //0-Metric 1-Inch   1.0000英寸=25.400毫米
	{
		//printf("nG_6:%d  g_nMetricOrInch_System:%d \n",nG_6,g_nMetricOrInch_System);
		if(g_nMetricOrInch_System_Old != -1)	//Don't need transform unit when the system startup
		{
			for(int i = wDBIndex_Start; i <= wDBIndex_End; i++)
			{
				if(GetDBValue(i).wPrecision == 3)	//Position DB
				{
					if(g_nMetricOrInch_System ==1)		//0->1  Metric->Inch
					{
						SetDBValue(i,(long)(GetDBValue(i).lValue / 2.54 + 0.5));
					}
					else if(g_nMetricOrInch_System ==0)	//1->0  Inch->Metric
					{
						SetDBValue(i,(GetDBValue(i).lValue * 2.54));
					}
				}
			}
			*/
			/*	2012/7/24 mark
			//20120504 when System_Unit changed, Update all Work_Coor<<<<< 
			DB_DATAATTRIBUTE*	pDataAttr = NULL;
			pDataAttr = g_pDatabase->GetAttr();
			WORD wStartDBIndex = g_pDatabase->GetIndex("CNC_WORK_COORD0_X");
			//printf("pRx-0-X:%ld Y:%ld Z:%ld \n",g_pRxCNCData->CNC_work_coord[0].x,g_pRxCNCData->CNC_work_coord[0].y,g_pRxCNCData->CNC_work_coord[0].z);
			memcpy(&g_pTxCNCData->CNC_work_coord[0],&(g_pRxCNCData->CNC_work_coord[0]),sizeof(LONG_AXIS)*NUM_OF_COOR);
			if(pDataAttr != NULL)	g_pDatabase->WriteBatch(&(g_pRxCNCData->CNC_work_coord[0]), pDataAttr[wStartDBIndex].dwOffset, sizeof(LONG_AXIS)*NUM_OF_COOR,TRUE);
			//20120504 >>>>>*/
		/*}
		g_nMetricOrInch_System_Old = g_nMetricOrInch_System;
	}
#endif*/
}//20110908>>>>>>


/*--------------------------------------------------------------------------+
|  Function :CTaskCNC::LoadTeachPoint()						|
|  Task     : 教導點讀取                                   					|
|             												                |
+--------------------------------------------------------------------------*/
void	LoadTeachPoint()	//Mario add  2015/4/7 上午 09:04:17
{
	FILE *fpt = NULL;
	fpt = fopen("./Data/TP/TEACHPOINT","r");
	if(fpt==NULL)
		CreateTeachPoint();
	fread(&g_TeachPoint, sizeof(g_TeachPoint) , 1, fpt);
	fclose( fpt );
}

void	CreateTeachPoint()	//Mario add  2016/1/4 下午 04:35:03
{
	memset(&g_TeachPoint,0,sizeof(g_TeachPoint));
	FILE *fpt = NULL;
	fpt = fopen("./Data/TP/TEACHPOINT","w");
	fwrite(&g_TeachPoint, sizeof(g_TeachPoint) , 1, fpt);
	fclose( fpt );
}


void	UpdateTeachPoint()
{
	FILE *fpt = NULL;
	fpt = fopen("./Data/TP/TEACHPOINT","w");
	fwrite(&g_TeachPoint, sizeof(g_TeachPoint) , 1, fpt);
	fclose( fpt );
	UpdateFileToNC("./Data/TP/TEACHPOINT");
}

int		GetTeachPointCnt(int	f_CooreSys)
{ 
	if(f_CooreSys == 0)
		f_CooreSys = g_TxCNCData.shmCNCData.CNC_work_coord_no;
	if(f_CooreSys == 0) f_CooreSys=1;
	return	g_TeachPoint.CoordSysMaxTP[f_CooreSys-1];
}

BOOL	GetTechPointValue(LONG_AXIS *f_GetTeachPointValue , int *f_GetTeachPointPos ,int	f_TeachPointNum ,int	f_CooreSys)
{
	if(f_CooreSys == 0)
		f_CooreSys = g_TxCNCData.shmCNCData.CNC_work_coord_no;
	if(f_CooreSys == 0) f_CooreSys=1;
	if (f_TeachPointNum <= GetTeachPointCnt(f_CooreSys) && f_TeachPointNum>0)
	{
		memcpy(f_GetTeachPointValue ,&g_TeachPoint.CoordValue[f_CooreSys-1][f_TeachPointNum-1] ,sizeof(LONG_AXIS));
		memcpy(f_GetTeachPointPos ,&g_TeachPoint.CoordPos[f_CooreSys-1][f_TeachPointNum-1] ,sizeof(int));
		return	TRUE;
	}
	else	
		return	FALSE;	
}


BOOL	EditTeachPoint(int	f_TeachPointNum ,LONG_AXIS f_SetTeachPointValue ,int f_GetTeachPointPos ,int	f_CooreSys)
{
	if(f_CooreSys == 0)
		f_CooreSys = g_TxCNCData.shmCNCData.CNC_work_coord_no;
	if(f_CooreSys == 0) f_CooreSys=1;
	if (f_TeachPointNum <= GetTeachPointCnt(f_CooreSys) && f_TeachPointNum > 0)
	{
		memcpy(&g_TeachPoint.CoordValue[f_CooreSys-1][f_TeachPointNum-1] ,&f_SetTeachPointValue ,sizeof(LONG_AXIS));
		g_TeachPoint.CoordPos[f_CooreSys-1][f_TeachPointNum-1] = f_GetTeachPointPos;
		UpdateTeachPoint();
		return	TRUE;
	}
	else	
		return	FALSE;	
}



BOOL	AddTeachPoint(LONG_AXIS f_SetTeachPointValue ,int f_GetTeachPointPos ,int	f_CooreSys)
{
	if(f_CooreSys == 0)
		f_CooreSys = g_TxCNCData.shmCNCData.CNC_work_coord_no;
	if(f_CooreSys == 0) f_CooreSys=1;
	if (GetTeachPointCnt(f_CooreSys) < 24)
	{
		memcpy(&g_TeachPoint.CoordValue[f_CooreSys-1][GetTeachPointCnt(f_CooreSys)] ,&f_SetTeachPointValue ,sizeof(LONG_AXIS));
		g_TeachPoint.CoordPos[f_CooreSys-1][GetTeachPointCnt(f_CooreSys)] = f_GetTeachPointPos;
		g_TeachPoint.CoordSysMaxTP[f_CooreSys-1]++;
		UpdateTeachPoint();
		return	TRUE;
	}
	else	
		return	FALSE;	
}


BOOL	DeleteTeachPoint(int	f_TeachPointNum ,int	f_CooreSys)
{
	if(f_CooreSys == 0)
		f_CooreSys = g_TxCNCData.shmCNCData.CNC_work_coord_no;
	if(f_CooreSys == 0) f_CooreSys=1;
	if (f_TeachPointNum <= GetTeachPointCnt(f_CooreSys) && f_TeachPointNum > 0)
	{
		for(int	i = f_TeachPointNum ; i < GetTeachPointCnt(f_CooreSys) ; i++)
		{
			memcpy(&g_TeachPoint.CoordValue[f_CooreSys-1][i-1] ,&g_TeachPoint.CoordValue[f_CooreSys-1][i] ,sizeof(LONG_AXIS));
			g_TeachPoint.CoordPos[f_CooreSys-1][i-1] = g_TeachPoint.CoordPos[f_CooreSys-1][i];
		}
		g_TeachPoint.CoordSysMaxTP[f_CooreSys-1]--;
		UpdateTeachPoint();
		return TRUE;
	}
	else
		return	FALSE;
}

BOOL	DelAllTeachPoint(int	f_CooreSys)
{
	if(f_CooreSys == 0)
		f_CooreSys = g_TxCNCData.shmCNCData.CNC_work_coord_no;
	if(f_CooreSys == 0) f_CooreSys=1;
	if (f_CooreSys>0 && f_CooreSys < 25)
	{
		g_TeachPoint.CoordSysMaxTP[f_CooreSys-1]=0;
		UpdateTeachPoint();
		return TRUE;
	}
	else
		return	FALSE;
}

int	EtherNet_WriteValue(int type ,int s_arr ,int len ,void* data)
{
	if(g_ptaskCncComm==NULL) return -1;
	return g_ptaskCncComm->WriteValue(type,s_arr,len,data);
}

#define D_TITLE_NUM 7

char* title[D_TITLE_NUM] =
{
"[MACHINE DATA]",
"[MACHINE JOINT STR]",
"[MACHINE JOINT STRID]",
"[MACHINE JOINT UNIT]",
"[MACHINE COORD STR]",
"[MACHINE COORD STRID]",	
"[MACHINE COORD UNIT]",
};
char* strid[D_TITLE_NUM][7] =
{
	{"MACHINEKINDS","JOINT_NUM","COORD_NUM",NULL},
	{"JOINT_STR1","JOINT_STR2","JOINT_STR3","JOINT_STR4","JOINT_STR5","JOINT_STR6",NULL},
	{"JOINT_STRID1","JOINT_STRID2","JOINT_STRID3","JOINT_STRID4","JOINT_STRID5","JOINT_STRID6",NULL},
	{"JOINT_UNIT_STRID1","JOINT_UNIT_STRID2","JOINT_UNIT_STRID3","JOINT_UNIT_STRID4","JOINT_UNIT_STRID5","JOINT_UNIT_STRID6",NULL},
	{"COORD_STR1","COORD_STR2","COORD_STR3","COORD_STR4","COORD_STR5","COORD_STR6",NULL},
	{"COORD_STRID1","COORD_STRID2","COORD_STRID3","COORD_STRID4","COORD_STRID5","COORD_STRID6",NULL},
	{"COORD_UNIT_STRID1","COORD_UNIT_STRID2","COORD_UNIT_STRID3","COORD_UNIT_STRID4","COORD_UNIT_STRID5","COORD_UNIT_STRID6",NULL},
};
enum ConfigTitle{Data=0,JointStr,JointStrId,JointUnit,CoordStr,CoordStrId,CoordUnit};
int	ReadRobotData()
{	
	FILE* robot_ini_file=NULL;
  char str[512];
  char* cp_str_ptr;
  memset(str,'\0',sizeof(str));
  //memset(New,0,sizeof(str));
  
  int error;
  int str_index;
  char* str_ptr;
	memset(&cfg,0,sizeof(Robot_Config));
  for(int i=0 ; i<D_TITLE_NUM ; i++)
  {
  	if(robot_ini_file!=NULL) fclose(robot_ini_file);
		robot_ini_file = fopen("./robotconfig.ini","r");
		if(robot_ini_file==NULL) printf("!!!!!!!!!!!!!!\n");
		error=i;
		str_index=0;
  	while(fgets(str, sizeof(str), robot_ini_file) != NULL)
  	{
  		if(strstr(str,title[i])==NULL) continue;
  		//printf("str= %s\n",str);
  		error=-1;
  		
  		while(strid[i][str_index]!=NULL)
  		{
  			while(fgets(str, sizeof(str), robot_ini_file) != NULL)
  			{
  				if(str[0]=='#') continue;
  				if(str[0]=='[') break;
  				if(strstr(str,strid[i][str_index])==NULL) continue;
  				str_ptr = str+strlen(strid[i][str_index]);
  				while(*str_ptr==' ')str_ptr++;
  				//printf("str_ptr = %s\n",str_ptr);
  				switch(i)
  				{
	  				case Data:
	  					if(str_index==0)
	  						cfg.Machine=atoi(str_ptr);
	  					if(str_index==1)
	  						cfg.jn=atoi(str_ptr);
	  					if(str_index==2)
	  						cfg.cn=atoi(str_ptr);
	  				break;
	  				case JointStr:
	  					cfg.j_s[str_index]=malloc(strlen(str_ptr));
	  					sprintf(cfg.j_s[str_index],"%s",str_ptr);
	  					cfg.j_s[str_index][strlen(str_ptr)-2]='\0';
	  				break;
	  				case JointStrId:
	  					cfg.j_sid[str_index]=malloc(strlen(str_ptr));
	  					sprintf(cfg.j_sid[str_index],"%s",str_ptr);
	  					cfg.j_sid[str_index][strlen(str_ptr)-2]='\0';
	  				break;
	  				case JointUnit:
	  					cfg.j_unit_sid[str_index]=malloc(strlen(str_ptr));
	  					sprintf(cfg.j_unit_sid[str_index],"%s",str_ptr);
	  					cfg.j_unit_sid[str_index][strlen(str_ptr)-2]='\0';
	  				break;
	  				case CoordStr:
	  					cfg.c_s[str_index]=malloc(strlen(str_ptr));
	  					sprintf(cfg.c_s[str_index],"%s",str_ptr);
	  					cfg.c_s[str_index][strlen(str_ptr)-2]='\0';
	  				break;
	  				case CoordStrId:
	  					cfg.c_sid[str_index]=malloc(strlen(str_ptr));
	  					sprintf(cfg.c_sid[str_index],"%s",str_ptr);
	  					cfg.c_sid[str_index][strlen(str_ptr)-2]='\0';
	  				break;
	  				case CoordUnit:
	  					cfg.c_unit_sid[str_index]=malloc(strlen(str_ptr));
	  					sprintf(cfg.c_unit_sid[str_index],"%s",str_ptr);
	  					cfg.c_unit_sid[str_index][strlen(str_ptr)-2]='\0';
	  				break;
	  			} 
	  			break;
  			}
  			str_index++;
  		}
  		break;
  	}
  	if(error!=-1) return error;
  }
	  /*printf("Machine = %d\n",cfg.Machine);
	  printf("JointNum = %d\n",cfg.jn);
	  printf("CoordNum = %d\n",cfg.cn);
	  for(int j = 0; j<cfg.jn ; j++)
	  {
		 	printf("Joint Str_%d = %s\n",j+1,cfg.j_s[j]);
		 	printf("Joint StrID_%d = %s\n",j+1,cfg.j_sid[j]);
		 	printf("Joint Unit_%d = %s\n",j+1,cfg.j_unit_sid[j]);
		 }
	  for(int j = 0; j<cfg.cn ; j++)
	  {
		 	printf("Coord Str_%d = %s\n",j+1,cfg.c_s[j]);
		 	printf("Coord StrID_%d = %s\n",j+1,cfg.c_sid[j]);
		 	printf("Coord Unit_%d = %s\n",j+1,cfg.c_unit_sid[j]);
		}*/
	return -1;
}