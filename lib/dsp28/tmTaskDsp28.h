#ifndef		D__TMTASKDSP28_V01
#define		D__TMTASKDSP28_V01
#include	"../../taskhost.h"
#include	"../../main.h"
#include	"../../ethnet.h"
//	0: CTask
//	1: CTaskComm, CtmPackClass
//	2: CtmTaskHost
const	int		CONST_REQ_ACTUAL 		= 1;		//�Y�ɸ�ƽШD
const	int		CONST_REQ_READMOTOR 	= 2;		//���F�ѼƸ��Ū�ШD
const	int		CONST_REQ_WRITEMOTOR 	= 3;		//���F�ѼƸ�Ƽg�ШD
const	int		CONST_REQ_ALARM 		= 4;		//ĵ����� �ȮɬO�S���ШD�A�u������
const	int		CONST_REQ_CURVE 		= 5;		//���u��ƽШD
const	int		CONST_REQ_COMMAND		= 6;		//�ʧ@�R�O�ШD
const	int		CONST_REQ_LINESTATUS	= 7;		//�p�u���A�ШD
const	int		CONST_REQ_MOLDSETSTATUS = 8;		//�Ҳէ�s�����T�{
const	int		CONST_REQ_MOLDSET	 	= 9;		//�Ҳէ�s�ШD
const	int		CONST_REQ_TEMPER 		= 10;		//�ū׼ƾڽШD
const	int		CONST_REQ_ADJUST		= 11;		//AD�ե��R�O�ƾڽШD
const	int		CONST_REQ_ADZERO		= 12;		//AD�k�s�R�O�ƾڽШD

const	int		CONST_REQ_SIEMREAD_DIAG	= 13;		//����l��ƶE�_Ū�Х�
const	int		CONST_REQ_SIEMWRITE_DIAG= 14;		//����l��ƶE�_�g�Х�

const	int		CONST_REQ_PWMTRDATA		= 15;		//N2C��o�M�q�O�p�q�T�o�e Caozz add 2012/3/21 �U�� 06:58:58

const	int		CONST_RES_ACTUAL 		= 1001;		//�Y�ɸ�Ʀ^�X
const	int		CONST_RES_READMOTOR 	= 1002;		//���F�ѼƸ��Ū�^�X
const	int		CONST_RES_WRITEMOTOR 	= 1003;		//���F�ѼƸ�Ƽg�^�X
const	int		CONST_RES_ALARM 		= 1004;		//ĵ�����
const	int		CONST_RES_CURVE 		= 1005;		//���u��Ʀ^�X
const	int		CONST_RES_COMMAND		= 1006;		//�ʧ@�R�O�^�X
const	int		CONST_RES_LINESTATUS	= 1007;		//�p�t���A�^�X  �ȮɨS���ϥΡC
const	int		CONST_RES_MOLDSETSTATUS = 1008;		//�Ҳո�Ƨ�s���A�^�X
const	int		CONST_RES_MOLDSET	 	= 1009;		//�Ҳո�Ʀ^�X
const	int		CONST_RES_TEMPER 		= 1010;		//�ū׸�Ʀ^�X
const	int		CONST_RES_ADJUST		= 1011;		//AD�ե��ƾڦ^�X
const	int		CONST_RES_ADZERO		= 1012;		//AD�k�s�R�O�ƾڦ^�X

const	int		CONST_RES_SIEMREAD_DIAG	= 1013;		//����l��ƶE�_Ū�^�X
const	int		CONST_RES_SIEMWRITE_DIAG= 1014;		//����l��ƶE�_�g�^�X

const	int		CONST_RES_PWMTRDATA		= 1015;		//����N2C��o�M�q�O�p�q�T Caozz add 2012/3/21 �U�� 06:58:58

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
		static 	void	ResValueFunc(char *pData, int nLength);									// 	�����ƾ�  
		
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
	    static	CtmTaskDsp28*		m_pSelf;				//	���V���������w  
		static	WORD	m_wSourceIPPort[5];
	    
	    //====================
		//	the common variants
		//====================
		ETHERNET		m_ethernet;		
		int 			m_nConnecting;					//�s���ʧ@���A  
		pthread_t 		m_OpenThreadID;
		pthread_mutex_t	mutex_Msg;						//�����i�{��
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

