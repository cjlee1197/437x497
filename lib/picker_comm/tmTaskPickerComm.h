/*
 *  Name      : tmTaskPickerComm.h
 *  Funtion   : 
 *  Created on: 2015-9-2
 *      Author: @Wang.z.w
 */

#ifndef    __TMTASKPICKERCOMM_H
#define    __TMTASKPICKERCOMM_H

#include    "./tmHmi485cmd.h"
#include	"../../taskammeter.h"
#include	"../../oil54struct.h"
#include 	"../../common.h"

#define     STAT_CMD_SIZE		sizeof(HMI_485_CMD)
#define     FILE_CMD_SIZE		sizeof(HMI_485_FILE_CMD)
								
#define     ERR_STAT_SIZE       8
#define 	SYS_STAT_SIZE	    sizeof(SYS_STAT_FOR_485)
#define     FILE_NAME_SIZE      sizeof(FILE_NAME_FOR_485)
#define     ADD_OTHER_SIZE      6 //head+code+len+crc+tail

#define     MAX_SIZE 		    255 + ADD_OTHER_SIZE    

#define		DEVICE_CLOSED		0x01
//#define		DEVICE_SUCCESS		 0x00
#define		DEVICE_QUEUE_FULL	0x02
#define		DEVICE_QUEUE_EMPTY	0x03

	
// �R�O�ƾڴV
typedef	struct tagtmPickerCmdFrame{
	BYTE		header;
	BYTE		code;
	BYTE		len;
	HMI_485_CMD	structCmd;
	WORD		crc;
	BYTE		tail;	
}TMPICKERCMDFRAME;

// ���A�ƾڴV
typedef	struct tagtmPickerStatFrame{
	BYTE				header;
	BYTE				code;
	BYTE				len;
	SYS_STAT_FOR_485	structStat;
	WORD				crc;
	BYTE				tail;	
}TMPICKERSTATFRAME;



#define	DFL_ZQUEUE_SIZE			8192
#define	ZQUEUE_MAX_SIZE			256

#define	OPRTATOR_SUCCESS		0
#define	FULL_QUEUE			 	-1
#define	EMPTY_QUEUE				-2
#define	VALID_QUEUE				-3
#define	ERR_NO_EXIST_QUEUE		-4
#define	ERR_CREATE_QUEUE		-5
#define	ERR_DESTORY_QUEUE		-6
#define OPRTATOR_FAILURE		-7
typedef     struct	tagQueue
{
	int		m_id;
	int		m_rear;
	int 	m_front;
	int 	m_size;
	int 	m_counter;
	char* 	m_buf;
	pthread_mutex_t qlock;
	
}QUEUE;

class		CQueue
{
	public:
		CQueue();
		virtual ~CQueue();
	public:
		static	unsigned short AddID(){ return ++m_id; }
		static	unsigned short SubID() {return --m_id; }
		QUEUE*	GetQueue(int nID);
		int		CreateQueue( int nSize);
		int	    DestroyQueue(int nID);
		int		EnQueue(int nID,void* pData, int nCount);
		int		DeQueue(int nID,void* pData, int nCount);
		BOOL	IsEmptyQueue(int nID);
		BOOL	IsFullQueue(int nID);
		int		QueueSize(int nID);
		BOOL	GetQueueValue(int nID,void* pData, int nSize,int nPos);
		int		GetCurrentFrontPos(int nID);
		int		GetCurrentRearPos(int nID);
		void	ClearQueue(int nID);
//		int		TestEnQueue(int nCount);
//		int		TestDeQueue(int nCount);
	private:
		void	InitQueue();
		int		EnQueueSpace(QUEUE* queue);
		int		DeQueueSpace(QUEUE* queue);
		int		GetQueueIndex(int nID);
	private:
		QUEUE*	m_queue;
		static	unsigned	short   m_id;
	private:
		CQueue(const CQueue &) ;
		CQueue& operator=(const CQueue& )const;
};

// �D��ƥ\����
class CtmTaskPickerComm : public CTaskAmmeter
{
	DECLARE_DYNCREATE(CtmTaskPickerComm)
	
	public:
		CtmTaskPickerComm();
		virtual ~CtmTaskPickerComm();
		
		void    init();
		void    Reset();
		void    Run();
		int	    CreateSelf();
		int	    FreeSelf();
		static	CtmTaskPickerComm*	m_pSelf;
		static  void	TxRun(_QUEUE*	_queue);  	
		static  void  CheckRun(void *); //�ˬd�o�e���ƪ��u�{,�i�H����h���o�e
		static  void  CheckData(void *);//�ˬd�����ƾڴV�O�_���T 
	      
		void	ComposeProtocol(int nType, BYTE *pData, int nLen); // �ե]�R�O
		int     SendData(void *pData, int nLen); // �o�e�ƾ�
	  	int     Parse(void *pData, int nLen);    // �����ƾ�
		WORD    CalcCRC(BYTE pData, WORD wCrc);  // ����C�Ӧr�`��CRC
		WORD    GetCRC(BYTE *pData, int nLen, WORD wCrcOut); // ����ƾڪ�CRC
			
	public:	
		//*************** ����R�O���f *************************//

		int		Write(int nType,void *pData, int nLen);
		void 	hmiIssueCommand(int nType, int hmiCmdType);
		bool    HandleCmdData(int nType, void *pData, int nLen);
		
		//*************** Ū�����A�ƾڱ��f *************************//
		
		int		ParseData(void *pData, int nLen); 		// �ƥ�
		int		RequestData(int nType, void *pData, int nLen); // �ШD�ƾ�
		
		//*************** �B�z�������ƾ� ***************//
		
		bool    HandleFrame(void *pData, int nLen);    // �B�z��ӴV
		bool    HandleErrStat(void *pData, int nLen);  // �B�z���~�ƾ�
		bool    HandleSysStat(void *pData, int nLen);  // �B�z�t�Ϊ��A
		bool    HandleFileName(void *pData, int nLen); // �B�z��󪬺A
		int     UpdateFrame(void *pData, int nLen);    // handle frame err 
		
		//*************** �O�s��DB��ơ]�Ȯɤ��{�^ **********************************//
		
		void    SaveErrStat();  // �O�s���~�ƾ�
		void    SaveSysStat();  // �O�s�t�Ϊ��A
		void    SaveFileStat(); // �O�s��󪬺A
		
	private:
		inline	int 	PushQueue(BYTE ch);	
		inline	int 	PushQueue(void* pInputData, int nLen);
		inline	int 	PopQueue(void* pOutputData,  int nLen);
		inline	int		initSend();	
		inline	int		UpdateSend();
		
	private:
	
		int                 m_nErrCount;
		int                 m_nCount_Cmd_Data;
		int                 m_nCount_Stat_Data;
		int                 m_nCount_Stat_Frame;
		int					m_queueTx_data_counter;
		
		short               m_mouldNum;
		
		long 			    			m_lCheckTimer;
		long                m_lLineTimer;
		long                m_lSendTimer;
		bool                m_flag_start;
		bool                m_flag_recv_ok;
		bool                m_flag_set_mould_num_cmd;//�]�m�Ҹ��R�O
		bool                m_flag_send_file_cmd;    //�o�e���R�O�лx
		bool                m_flag_sending_file_cmd; //���b�o�e���R�O�лx�]�Ҽ{��u�{������D�^
		bool                m_flag_recv_file_data;   //������󦨥\�лx
		
		tmRS485AMMETER      m_picker;
			
		BYTE				m_cmd_code;
		HMI_485_CMD			m_cmd_data;
		BYTE				m_sendbuff[MAX_SIZE];
		BYTE                m_sendcmdbuff[MAX_SIZE];
		BYTE				m_snedcmd_len;
		
		
		BYTE				m_stat_code;
		BYTE 				m_readbuff[MAX_SIZE];
		BYTE 				m_parsebuff[MAX_SIZE];
		 
		
		//�O�s�����ƾڪ��w�s��
		BYTE                m_err_stat[ERR_STAT_SIZE];
		SYS_STAT_FOR_485    m_sys_stat;
		FILE_NAME_FOR_485   m_file_name;
		
		// �u�{�����ܶq
		_QUEUE				m_queueTx;
		BYTE				m_buffer[4096];
		pthread_t			m_sendThreadID;
		sem_t				m_TxWait;
		pthread_mutex_t	    m_Tx_lock;
		pthread_mutex_t		m_queueTx_lock;
		pthread_t           m_checkThreadID;
		int				    m_IDTxQueue;
		CQueue			 m_clsFrameQueue;

};

#ifndef	D_BEOBJECT_CTMTASKPICKCKERCOMM
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
