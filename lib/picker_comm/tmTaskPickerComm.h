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

	
// 命令數據幀
typedef	struct tagtmPickerCmdFrame{
	BYTE		header;
	BYTE		code;
	BYTE		len;
	HMI_485_CMD	structCmd;
	WORD		crc;
	BYTE		tail;	
}TMPICKERCMDFRAME;

// 狀態數據幀
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

// 主函數功能類
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
		static  void  CheckRun(void *); //檢查發送次數的線程,可以執行多次發送
		static  void  CheckData(void *);//檢查接收數據幀是否正確 
	      
		void	ComposeProtocol(int nType, BYTE *pData, int nLen); // 組包命令
		int     SendData(void *pData, int nLen); // 發送數據
	  	int     Parse(void *pData, int nLen);    // 接收數據
		WORD    CalcCRC(BYTE pData, WORD wCrc);  // 校驗每個字節的CRC
		WORD    GetCRC(BYTE *pData, int nLen, WORD wCrcOut); // 獲取數據的CRC
			
	public:	
		//*************** 執行命令接口 *************************//

		int		Write(int nType,void *pData, int nLen);
		void 	hmiIssueCommand(int nType, int hmiCmdType);
		bool    HandleCmdData(int nType, void *pData, int nLen);
		
		//*************** 讀取狀態數據接口 *************************//
		
		int		ParseData(void *pData, int nLen); 		// 備用
		int		RequestData(int nType, void *pData, int nLen); // 請求數據
		
		//*************** 處理接收的數據 ***************//
		
		bool    HandleFrame(void *pData, int nLen);    // 處理整個幀
		bool    HandleErrStat(void *pData, int nLen);  // 處理錯誤數據
		bool    HandleSysStat(void *pData, int nLen);  // 處理系統狀態
		bool    HandleFileName(void *pData, int nLen); // 處理文件狀態
		int     UpdateFrame(void *pData, int nLen);    // handle frame err 
		
		//*************** 保存到DB資料（暫時文實現） **********************************//
		
		void    SaveErrStat();  // 保存錯誤數據
		void    SaveSysStat();  // 保存系統狀態
		void    SaveFileStat(); // 保存文件狀態
		
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
		bool                m_flag_set_mould_num_cmd;//設置模號命令
		bool                m_flag_send_file_cmd;    //發送文件命令標誌
		bool                m_flag_sending_file_cmd; //正在發送文件命令標誌（考慮到線程阻塞問題）
		bool                m_flag_recv_file_data;   //接收文件成功標誌
		
		tmRS485AMMETER      m_picker;
			
		BYTE				m_cmd_code;
		HMI_485_CMD			m_cmd_data;
		BYTE				m_sendbuff[MAX_SIZE];
		BYTE                m_sendcmdbuff[MAX_SIZE];
		BYTE				m_snedcmd_len;
		
		
		BYTE				m_stat_code;
		BYTE 				m_readbuff[MAX_SIZE];
		BYTE 				m_parsebuff[MAX_SIZE];
		 
		
		//保存接收數據的緩存區
		BYTE                m_err_stat[ERR_STAT_SIZE];
		SYS_STAT_FOR_485    m_sys_stat;
		FILE_NAME_FOR_485   m_file_name;
		
		// 線程相關變量
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
