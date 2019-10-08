#ifndef		D__TMTASKDSP28_M3_V01
#define		D__TMTASKDSP28_M3_V01
#include	"../../taskhost.h"
#include	"../../main.h"
#include	"../../ethnet.h"
#include	"../dsp28/tmTaskDsp28.h"
//	0: CTask
//	1: CTaskComm, CtmPackClass
//	2: CtmTaskHost
#define		m_nBlockNum    20
class  CtmTaskDsp28_M3: public CTaskHost
{
	DECLARE_DYNCREATE(CtmTaskDsp28_M3)
	public:
		CtmTaskDsp28_M3();
		~CtmTaskDsp28_M3();
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
		//	diagnose
		void	StartDiag();
		void	EndDiag();
		int		ReqValuesDiag(int pIDs[], int nElements, int nDiagType = 2);
		int		ReqValuesDiag(int nStartID, int nElements, int nDiagType = 2);
		int		WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType = 3);
		int		WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType = 3);
		long  	SetTimeToJ6();

		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
		
		void	InitData();
		int		SendMoldsetData(int nIndex);
		int		CalMoldsetCount();
		int		SetMoldsetID();
		void	SetPowerAmmeter();
	protected:
		int		GetData(void* pdata, int nLen);
	private:
		//====================
		//	the static functions
		//====================
		//	the thread function must be a staic function
		static 	void	_Open(void *pData);
		static 	void	ResValueFunc(char *pData, int nLength);									// 	接收數據  
		
		BOOL	RestartNet(BOOL bFlag = TRUE);
		int		GetBlockToDsp28(WORD wIndex, int nType);
		int		ReqReadData();
		//int		SendOneOffData();
	protected:
		BOOL		m_bDiagFlag;
		int			m_nDiagReqNum;
	private:
	    //====================
		//	the static variants
		//====================
	    static	CtmTaskDsp28_M3*		m_pSelf;				//	指向本身的指針  
		static	WORD	m_wSourceIPPort[5];
	    
	    //====================
		//	the common variants
		//====================
		ETHERNET		m_ethernet;		
		int 			m_nConnecting;					//連接動作狀態  
		pthread_t 		m_OpenThreadID;
		pthread_mutex_t	mutex_Msg;						//消息進程鎖
		pthread_attr_t  m_ThreadAttr;
		long	m_lCheckQuickTimer;
		long	m_lReqReadDataTime;
		//long	m_lUpdateTimer;
		long	m_lMoldSendNum;							//開機發送請求資料總數
		long	m_lMoldGetNum;							//讀取到資料總數
		BOOL	m_bReqOnLineData;						//開機請求一次，是否受到
		BOOL	m_bCheckData;							//檢查資料
		long	m_lCheckResult;							//檢查結果
		BOOL	m_bFlashSaved;							//flash上是否有資料
		BOOL	m_bDC;
		BOOL	m_bT11Port19;
		BOOL	m_bMold36;
		BOOL  	m_bEtherCat;
		BOOL 	m_bJ6Redy;
		BOOL	m_bJUIII;
		long	m_lOnLineTime;  
		long	m_lTempReqTime; /*ZHBA 2013-4-18 */
		//long	m_lTestTime;
		//long	m_lSendDataState;	
		
		WORD	m_wRealReadNum;
		WORD	m_wReadIDIndex;
		int*	m_pReadIDs;
		WORD	m_wReadIDNum;
		int		m_nSyncDBIndex;
		int		m_nSyncState;
		int		m_nBlockIndex[m_nBlockNum];
		int		m_nMoldsetCount;
		int* 	m_pMoldSetIndexs;
		WORD	m_OldHeadCount;
		
};


#ifndef	D_BEOBJECT_CTMTASKDSP28_M3	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

