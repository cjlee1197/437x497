/*===========================================================================+
|  Class    : TaskiNet                                                       |
|  Task     : Communication Service Routine header file                      |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Caozz                                                          |
|  Version  : V5.00                                                          |
|  Creation : 11/22/2011                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKINET
#define     D_TASKINET
#include	<fstream>   
#include	"../../taskhost.h"
#include	"../libiNetComm.h"
#include    "../../ethnet.h"
#include    "../../database.h"
#include	"../../taskcomm.h"
#include 	"../../common.h"
#include	"../../recordset.h"
#include	"../../oil54struct.h"
#include	"../../readdata.h"
#include	"../../files.h"
#include 	"../libControl.h"
/*===========================================================================+
|           Class declaration - TaskINet                                     |
+===========================================================================*/

#define		INET_LINE_STATUS_OK				0
#define		INET_PHYSICAL_DEVICE_NO_READY	0x0001
#define		INET_PHYSICAL_ONLINE_NO_READY	0x0002
#define		INET_INIT_NO_READY				0x0004
#define		INET_INIT_IPADDRESS				0x0008
#define		INET_INETSERVER_ONLINE_NO_READY	0x0010

#define 	INET_PROTOCOLLIST_FILENAME		"./lib/inet/PROList.lst"
#define		INET_MAXMSG						128

#define		INET_MSG_HMI					0
#define		INET_MSG_INET					1

const	int		CONST_REQ_LOCALNETINFO 		= 1;		//Request Local set net info
const	int		CONST_MSG_PARA_ID	 		= 2;		//Request Local set net info

const	int		CONST_RES_LOCALNETINFO 		= 101;		//Write Local set net info

class	CtmTaskiNet;

typedef struct tagINetDateTime
{
	WORD		wYear;
	BYTE		byMonth;
	BYTE		byDay;
	BYTE		byHour;
	BYTE		byMinute;
	WORD		wMilliSecond;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INetDateTime;
#else 
#ifdef	D_X86
INetDateTime;
#endif
#endif

typedef struct tagINetDataMap
{
	DWORD		dwInetDBID;
	DWORD		dwHmiDBID;
	WORD		wUOM;
	BYTE		byPrecision;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INetDataMap;
#else 
#ifdef	D_X86
INetDataMap;
#endif
#endif

typedef struct tagINetDBInfo
{
	WORD		wType;
	WORD		wUOM;
	BYTE		byPrecision;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	INetDBInfo;
#else 
#ifdef	D_X86
INetDBInfo;
#endif
#endif


typedef struct tagMsgPara
{
	DWORD		dwDataID;
	DWORD		dwCondition;
	WORD		wDataType;
	WORD		wUOM;
	WORD		wLength;
	union
	{
		char 			acData[16];
		unsigned char 	ucData;
		unsigned short	wData;
		unsigned long	dwData;
		signed char 	cData;
		signed short	nData;
		signed long		lData;
		float 			fData;
		double 			dData;
		char			*pcData;
	};
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmMsgPara;
#else 
#ifdef	D_X86
tmMsgPara;
#endif
#endif


typedef struct tagiNetMsg
{
	WORD		wMsgID;
	WORD		wParaCount;
	tmMsgPara*	pMsgPara;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmiNetMsg;
#else 
#ifdef	D_X86
tmiNetMsg;
#endif
#endif

typedef struct tagtmiNetDataFile
{
	WORD		wProVer;
	DWORD		dwProID;
	BYTE		byControlFlag;
	BYTE		byPackStamp;
	int			nSize;
	WORD		wDataNum;
	DWORD*		pDBID;
	WORD		wStartMSGNum;
	tmiNetMsg* 	pStartiNetMsg;
	WORD		wEndMSGNum;
	tmiNetMsg* 	pEndiNetMsg;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmiNetDataFile;
#else 
#ifdef	D_X86
tmiNetDataFile;
#endif
#endif

typedef struct tagtmiNetDynamicList
{
	DWORD		dwProID;
	WORD		wProVer;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmiNetDynamicList;
#else 
#ifdef	D_X86
tmiNetDynamicList;
#endif
#endif

typedef struct tagtmiNetMsgPRO
{
	tmiNetMsg* 	iNetMsg;
	int 		nProCount;
	DWORD		pProIDs[20];
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmiNetMsgPRO;
#else 
#ifdef	D_X86
tmiNetMsgPRO;
#endif
#endif

typedef int 	(CtmTaskiNet::*pDealData)(char *, int);
typedef	void	(CtmTaskiNet::*pDealToiNetMsg)(WPARAM, LPARAM );
typedef struct TagDealiNetData
{
    DWORD		dwCmd;
    pDealData	pDealFunc;
} tmGetData, tmSetData;

typedef struct TagDealDataPro
{
    DWORD		dwProID;
    pDealData	pDealFunc;
} tmSendDataPro, tmRevDataPro;

typedef	struct	TagNetDealToiNetMsg
{
	int 		nMsg;
	pDealToiNetMsg	pMsgFunc;
}tmDealToiNetMsg;

typedef struct tagiNetInfo
{
	WORD	wDHCPFlag;
	BYTE	acLocalIP[4];
	BYTE	acNetMaskIP[4];
	BYTE	acGatway[4];
	BYTE	acServerIP[4];
	WORD	wsrcPort;
	WORD	wdstPort;
} tmiNetInfo;

typedef struct tagDataTypeMap
{
	WORD	wiNetType;
	WORD	wHMIType;
	WORD	wSize;
} tmDataTypeMap;
typedef	struct tagCustomData
{
	DWORD		dwDBID;
	WORD		wType;
	
}tmCustomData;

typedef struct tagMsgParaID
{
	DWORD	dwDBID;
	char 	szOldValue[8];
	char*	pszOldValue;
}tmMsgParaID;


class CtmTaskiNet: public CTaskHost
{
	friend	void ParseiNetData(char *pdata, int nlength);
	
	friend	void _CheckNetwork(CtmTaskiNet* pRomote);
	DECLARE_DYNCREATE(CtmTaskiNet)
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
	CtmTaskiNet();
	virtual ~CtmTaskiNet();
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:
	//====================
	//	override the function that are defined in the CtmPackClass
	//====================
	int						CreateSelf();
	int						FreeSelf();
private:
	char					m_pacEtherName[20];	
	ETHERNET				m_ethernet;
	unsigned int 			_CRC32_Talbe[256];
	
	long					m_lCheckIPSCounter;
	long					m_lCheckNetLineCounter;
	long 					m_liNetTimer;
	long	 				m_lCheckNetStatusCounter;
	long					m_lOfflineCounter;
	
	long					m_lSendCMDTimer;
	long					m_lRevCMDTimer;
	
	
	WORD					m_wProtocolVer;
	DWORD					m_dwStaticVer;
	DWORD					m_dwDynamicVer;
	
	CtmFile 				m_File;
	
	tmiNetDataFile*			m_pStaticSendFile;
	tmiNetDataFile*			m_pStaticRevFile;
	
	tmiNetDataFile*			m_pDynamicProtocol;
	char					m_szDynamicDir[128];
	
	int						m_nStaticSendNum;
	int						m_nStaticRevNum;
	
	WORD					m_wDynamicProNum;
	
	DWORD					m_dwDataMapVer;
	int						m_nInetDataCount;
	INetDataMap*			m_InetDataMap;
	INetDataMap*			m_HmiDataMap;
	
	BYTE					m_acRxiNetBuffer[MAX_ETOKEN_SIZE];
	BYTE					m_acTxiNetBuffer[MAX_ETOKEN_SIZE];
	BYTE					m_acRealPhysicalAddress[CONST_LENGTH_MAC];
	
	
	tmiNetMsgPRO			m_piNetStartMsg[INET_MAXMSG];
	int 					m_niNetStartMsgCount;
	
	tmiNetMsgPRO			m_piNetEndMsg[INET_MAXMSG];
	int 					m_niNetEndMsgCount;
	
	static pthread_mutex_t	m_SendMutex;
	static		BYTE		m_acICCIP[CONST_LENGTH_IP];
	static		BYTE		m_acLocalNetMask[CONST_LENGTH_IP];
	
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    
	void					WndProc(int message, WPARAM wParam, LPARAM lParam);
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	void    				Run();
	int						GetNetInformation(BYTE *pdata, int nflag);
	int						GetLocalIP(BYTE *pIP);
	int						SetLocalIP(BYTE *pIP);
	int						RestartNet(short nflag);
	int     				GetLinkStatus();
	        				
	int						ReqValues(int nType, void* pOutData, int nSize);
	int						WriteValue(int nType, void* pInputData, int nSize);	
	        				
	int						GetLocalNetInfo(void* pOutData, int nSize);
	int 					SetLocalNetInfo(void* pInputData, int nSize);
	
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	int						CheckNetwork();
	int						OpenDevice();
	int						CloseDevice();
	        				
	void					SendOneOffData();
	void					SendToiNet(WORD wDataLength, DWORD dwIDData);
	        				
	void					CreateCRC32Table();
	unsigned int 			GetCRC32(char* pData, unsigned  int Length);
	unsigned int 			GetCRC32B(unsigned int CRCCode, char* pData, unsigned int Length);

private:
	
	int						m_nLineStatus;
	DWORD					m_dwiNetTimer;
	BOOL					m_bEndSynch	;
	
	
	NETFRAMERX  			m_frameRx;
	NETFRAMETX  			m_frameTx;
	
	BOOL					m_bUpdateFlag;
	BOOL					m_bInitFlag;
	BOOL					m_bInitIP;
	int						m_handle;
	static	tmGetData		m_SendToiNet[];
	static	tmSetData		m_RevFromiNet[];
	static	tmDealToiNetMsg	m_iNetDealToiNetMsg[];
	
	static	tmDataTypeMap	m_DataTypeMap[];
	static	tmCustomData	m_CustomData[];
	static	tmRevDataPro	m_RevDataPro[];
	static	char*			m_pErrorStr[];
	
	
	tmiNetInfo				m_iNetInfo;
	
	
	pthread_t				m_CheckNetworkThreadID;
	
	BOOL					m_bReStart;		
	int 					m_iNetPROCount;
	
	
	DWORD					m_dwProErrorID;
	WORD					m_wProErrorLength;
	
	BYTE					m_byiNetTimeSyncFlag;
	DWORD					m_dwiNetOnLineTime;
	
	DWORD					m_dwiNetStaticVer;
	DWORD					m_dwiNetDynamicVer;
	
	DWORD					m_dwDicStartID;
	DWORD					m_dwDicEndID;
	
	BOOL					m_bDynamicUpdateFlag;
	WORD					m_wiNetDynamicCount;
	tmiNetDynamicList*		m_iNetDynamicList;
	
	BYTE					m_byReadDataFlag;
	WORD					m_wReadDataCount;
	DWORD*					m_ReadDataList;
	
	WORD					m_wWriteDataCount; 
	BYTE*					m_pWriteState;
	
private:
	
	void					Init();
	void					InitStaticProtocol();
	void					InitDynamicProtocol();
	void					InitDataMap();
	void					UnInit();
	void					UnStaticProtocol();
	void					UnDynamicProtocol();
	void					UnDataMap();
	                		
	int 					GetProtocolIndex(DWORD dwProID, BYTE byProtocolType = 0);
	void					SetProtocolStamp(DWORD dwProID, BYTE byPackStamp, BYTE byProtocolType = 0);
	
	tmiNetDataFile*			GerDynamicProtocolDataInfo(char* pszDir, char* pszFileName, WORD* pwNum);
	tmiNetDataFile*			GerProtocolDataInfo(char* pszDir, char* pszFileName, int* pnNum);
	int						GetDataListNum(char* pszFileName);
	int						GetProtocolVersion(char* pszFileName);
	int						GetProtocolControlFlag(char* pszFileName);
	void					GetDataList(char* pszFileName, DWORD*	pData, int nNum);
	void					GetMsgDataInfo(char* pszFileName, tmiNetDataFile* piNetDataFile);
	int						GetMsgDataInfo(char* pszFileName, tmiNetMsg* piNetMsg, WORD wiNetMsgNum, int nOffset);
	
	WORD					GetCustomDataType(DWORD dwDBID);
	INetDataMap*			GetiNetDBMapFromiNetDBID(DWORD dwiNetDBID);
	INetDataMap*			GetiNetDBMapFromHmiDBID(DWORD dwiNetDBID);
	WORD					GetiNetDataSize(WORD wDataType);
	WORD					GetHMIDataSize(WORD wDataType);
	WORD					DataTypeHmiTOInet(WORD wDataType);
	WORD					DataTypeInetTOHmi(WORD wDataType);
	
	int 					AddMsgPro(tmiNetMsg* piNetMsg, WORD wMsgNum, DWORD dwProID, tmiNetMsgPRO* piNetMsgPro, int niNetMsgProCount);
	void					AddMsgDataID(tmiNetMsg* piNetMsg, WORD wParaCount);
	
	INetDBInfo				GetiNetDBInfo(int nDBIndex, INetDataMap* piNetDataMap);
	                		
	void					SetDynamicProtocolList(tmiNetDynamicList* pDynamicList, WORD wCount);
	void					SetDynamicDBID(DWORD dwProID, WORD wVersion, WORD wDBCount, DWORD* pDBIDs);
	
	void					SetDynamicMsg(DWORD dwProID, char* pMsg, int nOffset);
	int						SetDynamicMsg(DWORD dwProID, char* pMsg, WORD wMsgNum, int nOffset);
	
	int 					ReadFile			(char* pszFileName, char*  pszBuffer, int nLength, int nOffset = 0);
	int 					WriteFile			(char* pszFileName, char*  pszBuffer, int nLength, int nOffset = 0);
	int						ComposeData(char* pOut, DWORD* pDBID, WORD wDataNum, BYTE byIDFlag);
	int						ComposeSchema(char* pOut, DWORD* pDBID, WORD wDataNum, BYTE byIDFlag);
	
	
	int						SetData(int nID, char* pTemp, int nLen, BYTE	byProtocolType);
	int						Send(DWORD dwProID, BYTE byProtocolType = 0);
	//===============================//
	//	compose the special data	 //
	//===============================//
	int						ComposeReqIPFlag		(char* pOut, int nIndex);
	int						ComposeIPAddress		(char* pOut, int nIndex);
	int						ComposeEncoder			(char* pOut, int nIndex);
	int						ComposeProError			(char* pOut, int nIndex);
	int						ComposeTimer			(char* pOut, int nIndex);
	int						ComposeTimerFlag		(char* pOut, int nIndex);
	int						ComposeOnline			(char* pOut, int nIndex);
	int						ComposeStaticVer		(char* pOut, int nIndex);
	int						ComposeDynamicVer		(char* pOut, int nIndex);
	int						ComposeDynamicList		(char* pOut, int nIndex);
	int						ComposeReadData			(char* pOut, int nIndex);
	int						ComposeWriteData		(char* pOut, int nIndex);
	int						ComposeDataDic			(char* pOut, int nIndex);
	int						ComposeiNetStaticVer	(char* pOut, int nIndex);
	int						ComposeiNetDynamicVer	(char* pOut, int nIndex);
	int						ComposeDataMap			(char* pOut, int nIndex);
	int						ComposeHostType			(char* pOut, int nIndex);
	int						ComposePNSN_Hardware	(char* pOut, int nIndex);
	int						ComposePNSN_Software	(char* pOut, int nIndex);
	
	//============================================//
	//	parse the special data that inet send	  //
	//	the parse function's result is the data's offset that is parsed
	//============================================//
	int 					ParseIPAddress			(char* pData, int nLen);
	int 					ParseTimer				(char* pData, int nLen);
	int 					ParseTimerFlag			(char* pData, int nLen);
	int 					ParseOnline				(char* pData, int nLen);
	int 					ParseStaticVer			(char* pData, int nLen);
	int 					ParseDynamicVer			(char* pData, int nLen);
	int 					ParseDynamicList		(char* pData, int nLen);
	int 					ParseDynamicData			(char* pData, int nLen);
	int 					ParseReadData			(char* pData, int nLen);
	int 					ParseWriteData			(char* pData, int nLen);
	int 					ParseiNetStaticVer		(char* pData, int nLen);
	int 					ParseiNetDynamicVer		(char* pData, int nLen);
	int 					ParseDataDic			(char* pData, int nLen);
	int 					ParseDataMap			(char* pData, int nLen);
	//============================================//
	//	Deal the hmi message					  //
	//============================================//
	void					DealTimer				(WPARAM wParam, LPARAM lParam);
	//void					DealParaDataID			(WPARAM wParam, LPARAM lParam);
	void					DealToiNetRestart		(WPARAM wParam, LPARAM lParam);
	
	int						RevDataProFunc			(char* pData, int nIndex);
	
	
	void					ProcessParaIDMsg(tmMsgParaID* pMsgParaID);
	
	int						ProcessCondtion(DWORD dwCondition, unsigned char Value, unsigned char OldValue, unsigned char ParaValue);
	int						ProcessCondtion(DWORD dwCondition, unsigned short Value, unsigned short OldValue, unsigned short ParaValue);
	int						ProcessCondtion(DWORD dwCondition, unsigned long Value, unsigned long OldValue, unsigned long ParaValue);
	int						ProcessCondtion(DWORD dwCondition, signed char Value, signed char OldValue, signed char ParaValue);
	int						ProcessCondtion(DWORD dwCondition, signed short Value, signed short OldValue, signed short ParaValue);
	int						ProcessCondtion(DWORD dwCondition, signed long Value, signed long OldValue, signed long ParaValue);
	int						ProcessCondtion(DWORD dwCondition, float Value, float OldValue, float ParaValue);
	int						ProcessCondtion(DWORD dwCondition, double Value, double	OldValue, double ParaValue);
	int						ProcessCondtion(DWORD dwCondition, char* Value, char*	OldValue, char* ParaValue);
	
		
	static void				ParseThread_CleanupFunc(void *arg);
};
/*===========================================================================+
|           Variable and Function declaration                                |
+===========================================================================*/
void	ParseiNetData(char *pdata, int  nlength);

void 	_CheckNetwork(CtmTaskiNet* piNet);

#ifndef	D_BEOBJECT_CTMTASKINET

extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
#endif
