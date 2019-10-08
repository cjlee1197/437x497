#ifndef		D__TMTASKDSP28_V01
#define		D__TMTASKDSP28_V01
#include	"../../taskhost.h"
#include	"../../main.h"
#include	"../../ethnet.h"
//	0: CTask
//	1: CTaskComm, CtmPackClass
//	2: CtmTaskHost
const	int		CONST_REQ_ACTUAL 		= 1;		//即時資料請求
const	int		CONST_REQ_READMOTOR 	= 2;		//馬達參數資料讀請求
const	int		CONST_REQ_WRITEMOTOR 	= 3;		//馬達參數資料寫請求
const	int		CONST_REQ_ALARM 		= 4;		//警報資料 暫時是沒有請求，只有接收
const	int		CONST_REQ_CURVE 		= 5;		//曲線資料請求
const	int		CONST_REQ_COMMAND		= 6;		//動作命令請求
const	int		CONST_REQ_LINESTATUS	= 7;		//聯線狀態請求
const	int		CONST_REQ_MOLDSETSTATUS = 8;		//模組更新完成確認
const	int		CONST_REQ_MOLDSET	 	= 9;		//模組更新請求
const	int		CONST_REQ_TEMPER 		= 10;		//溫度數據請求
const	int		CONST_REQ_ADJUST		= 11;		//AD校正命令數據請求
const	int		CONST_REQ_ADZERO		= 12;		//AD歸零命令數據請求

const	int		CONST_REQ_SIEMREAD_DIAG	= 13;		//西門子資料診斷讀請示
const	int		CONST_REQ_SIEMWRITE_DIAG= 14;		//西門子資料診斷寫請示

const	int		CONST_REQ_PWMTRDATA		= 15;		//N2C轉發和電力計通訊發送 Caozz add 2012/3/21 下午 06:58:58

const	int		CONST_RES_ACTUAL 		= 1001;		//即時資料回饋
const	int		CONST_RES_READMOTOR 	= 1002;		//馬達參數資料讀回饋
const	int		CONST_RES_WRITEMOTOR 	= 1003;		//馬達參數資料寫回饋
const	int		CONST_RES_ALARM 		= 1004;		//警報資料
const	int		CONST_RES_CURVE 		= 1005;		//曲線資料回饋
const	int		CONST_RES_COMMAND		= 1006;		//動作命令回饋
const	int		CONST_RES_LINESTATUS	= 1007;		//聯系狀態回饋  暫時沒有使用。
const	int		CONST_RES_MOLDSETSTATUS = 1008;		//模組資料更新狀態回饋
const	int		CONST_RES_MOLDSET	 	= 1009;		//模組資料回饋
const	int		CONST_RES_TEMPER 		= 1010;		//溫度資料回饋
const	int		CONST_RES_ADJUST		= 1011;		//AD校正數據回饋
const	int		CONST_RES_ADZERO		= 1012;		//AD歸零命令數據回饋

const	int		CONST_RES_SIEMREAD_DIAG	= 1013;		//西門子資料診斷讀回饋
const	int		CONST_RES_SIEMWRITE_DIAG= 1014;		//西門子資料診斷寫回饋

const	int		CONST_RES_PWMTRDATA		= 1015;		//接收N2C轉發和電力計通訊 Caozz add 2012/3/21 下午 06:58:58

#define		DSP28PROTOCOL_HEADER_MAXSIZE	12
#define		DSP28PROTOCOL_FOOTER_MAXSIZE	2
#define		DSP28PROTOCOL_HEADER_ELEMENTNUM_POS	10
#define		DSP28PROTOCOL_HEADER_CONTEXT	0x8899
#define		DSP28PROTOCOL_FOOTER_CONTEXT	0x6688

class	CtmDsp28Protocol:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmDsp28Protocol)
	public:
		CtmDsp28Protocol()
		{
			m_nHeaderPos	= 0;       
			m_nFooterPos	= 0;
			m_nDataSizePos	= 2;
			m_nTypePos		= 4;
			m_nDataCountPos	= 6; 
			m_nCyclePos		= 8; 
			m_nElementNumPos= 10;
		}
		~CtmDsp28Protocol()
		{
		}
	public:
		void	SetSendType(int nType)
		{
			m_nSendType	= nType;
			WORD wTemp	= m_nSendType;
			m_SendHeader.SetPartValue(m_nTypePos, 	&wTemp, sizeof(wTemp));
		}
		void	SetRevType(int nType)
		{
			m_nRevType	= nType;	
			WORD wTemp	= m_nRevType;
			m_RevHeader.SetPartValue(m_nTypePos, 	&wTemp, sizeof(wTemp));
		}
	    int		InitSend()
	    {
	    	m_SendHeader.SetMaxSize(12);		//	the Rev header
			m_SendHeader.SetSize(12);		//	the Rev header
			
			m_SendFooter.SetMaxSize(2);		
			m_SendFooter.SetSize(2);
			
			WORD	wTemp;
			wTemp	= 0x8899;
			m_SendHeader.SetPartValue(m_nHeaderPos, &wTemp, sizeof(wTemp));
			SetSendType(m_nSendType);	
			wTemp	= 0x6688;
			m_SendFooter.SetPartValue(m_nFooterPos, &wTemp, sizeof(wTemp));
			return 0;
		}
		int		InitRev()
		{
			m_RevHeader.SetMaxSize(12);		//	the Rev header
			m_RevHeader.SetSize(12);		//	the Rev header
			
			m_RevFooter.SetMaxSize(2);		
			m_RevFooter.SetSize(2);
			
			SetRevType(m_nRevType);
			return 0;
		}
	protected:
		int		m_nHeaderPos;
		int		m_nFooterPos;
		int		m_nDataSizePos;
		int		m_nTypePos;
		int		m_nDataCountPos;
		int		m_nCyclePos;
		int		m_nElementNumPos;
};

IMPLEMENT_DYNCREATE(CtmDsp28Protocol, CtmProtocol)
//CtmDsp28Protocol::CtmDsp28Protocol()
//{
//	m_nHeaderPos	= 0;       
//	m_nFooterPos	= 0;
//	m_nDataSizePos	= 2;
//	m_nTypePos		= 4;
//	m_nDataCountPos	= 6; 
//	m_nCyclePos		= 8; 
//	m_nElementNumPos= 10;
//}
//
//
//CtmDsp28Protocol::~CtmDsp28Protocol()
//{
//	
//}
//
//int		CtmDsp28Protocol::InitSend()
//{
//	m_SendHeader.SetMaxSize(12);	//	the Send header
//	m_SendHeader.SetSize(12);		//	the Send header
//	m_SendFooter.SetMaxSize(2);		
//	m_SendFooter.SetSize(2);
//	
//	WORD	wTemp;
//	wTemp	= 0x8899;
//	m_SendHeader.SetPartValue(m_nHeaderPos, &wTemp, sizeof(wTemp));
//	SetSendType(m_nSendType);	
//	wTemp	= 0x6688;
//	m_SendFooter.SetPartValue(m_nFooterPos, &wTemp, sizeof(wTemp));
//	return 0;
//}
//
//int		CtmDsp28Protocol::InitRev()
//{
//	m_RevHeader.SetMaxSize(12);		//	the Rev header
//	m_RevHeader.SetSize(12);		//	the Rev header
//	
//	m_RevFooter.SetMaxSize(2);		
//	m_RevFooter.SetSize(2);
//	
//	SetRevType(m_nRevType);
//	return 0;
//}
//
class  CtmTaskDsp28: public CTaskHost
{
	DECLARE_DYNCREATE(CtmTaskDsp28)
	public:
		CtmTaskDsp28();
		~CtmTaskDsp28();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
		//====================
		//	override the function that are defined in the CtmTaskHost
		//====================
		int		SendZeroSet(int nZeroType = 0);		
		//	request value
		int		ReqValues(int nType, int nElements, char* pszIDs[], void* pEx = NULL);
		int		ReqValues(int nType, int nElements, int pIndexs[], void* pEx = NULL);
		int		ReqValues(int nType, char* pszID, void* pEx = NULL);
		int		ReqValues(int nType, void* pOutData, int nSize);		
		//	stop request
		int		StopReq(int nType, int nElements, char* pIDs[], void* pEx = NULL);
		int		StopReq(int nType, int nElements, int pIndexs[], void* pEx = NULL);
		int		StopReq(int nType, char* pszID, void* pEx = NULL);		
		//	write the value to the motor & host; the values are from database
		int		WriteValue(int nType, int nElements, char* pszIDs[], void* pEx = NULL);
		int		WriteValue(int nType, int nElements, int pIndexs[], void* pEx = NULL);
		int		WriteValue(int nType, char* pszID, void* pEx = NULL);	
		int		WriteValue(int nType, void* pInputData, int nSize);	
		int		WriteValue(int nDataID);
		int		WriteValue(char* pszID);
		//	0 is on line
		//int		CheckOnLine();
		int		GetOnLineStatus();		
		//void			SetHostProtocol(CtmHostProtocol* pHostProtocol);
		//CtmHostProtocol* GetHostProtocol();
		
		
		
		//	diagnose
		void	StartDiag();
		void	EndDiag();
		int		ReqValuesDiag(int pIDs[], int nElements, int nDiagType = 2);
		int		ReqValuesDiag(int nStartID, int nElements, int nDiagType = 2);
		int		WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType = 3);
		int		WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType = 3);
		
		
		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
			
	protected:
		int		GetData(void* pdata, int nLen);
		//virtual	void	InitDsp();
		//virtual	void	UnInitDevice();
	private:
		//====================
		//	the static functions
		//====================
		//	the thread function must be a staic function
		static 	void	_Open(void *pData);
		static 	void	ResValueFunc(char *pData, int nLength);									// 	接收數據  
		
		BOOL	RestartNet(BOOL bFlag = TRUE);
		int		GetBlockToDsp28(WORD wIndex, int nType);
		WORD	OpenICMPDevice();
		WORD	SendICMPDate(int sockfd, char* pIcmp_data, char* pDateName, sockaddr_in* dstaddr);
		char*	RecvEchoReply(int sockfd, char* pszRecvBuf);
	protected:
		BOOL		m_bDiagFlag;
		int			m_nDiagReqNum;
		//int		m_nWriteDiagFlag;
	private:
	    //====================
		//	the static variants
		//====================
	    static	CtmTaskDsp28*		m_pSelf;				//	指向本身的指針  
		static	WORD	m_wSourceIPPort[5];
	    
	    //====================
		//	the common variants
		//====================
		ETHERNET		m_ethernet;		
		int 			m_nConnecting;					//連接動作狀態  
		pthread_t 		m_OpenThreadID;
		pthread_mutex_t	mutex_Msg;						//消息進程鎖
		pthread_attr_t  m_ThreadAttr;
		long	m_lFirstTime;	
		long	m_lCheckQuickTimer;
		long	m_lUpdateTimer;
		
		
		long	m_lOnLineTime;
		long	m_lTestTime;
		
		
		BYTE	m_RxBuffer[4096];						//	rev buffer
		BYTE	m_TxBuffer[4096];						//	send buffer
		
	
};




#ifndef	D_BEOBJECT_CTMTASKDSP28	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

