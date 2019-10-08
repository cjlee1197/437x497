#ifndef	D__TMDSP54UPDATE_V01
#define	D__TMDSP54UPDATE_V01

#include	"../../tmprotocol.h"
#include	"../../oil54struct.h"
#include	"../libComm.h"

#define		COMM_UPDATE_PACKSIZE_54		0x400			//dsp54 穝程计沮
#define		COMM_UPDATE_PACKSIZE_28		0x1C0			//dsp28 穝程计沮
#define		COMM_UPDATE_PACKMAX			0x400			//穝程计
#define		COMM_DSP55_FIFLETYPE		0x3A00
typedef struct tagSendFileInfo
{
	long 	lFileLength;		//计沮羆
	long	lPackageCount;		//计沮计
	long	lPackageSize;		//–计沮
	long	lCurrentNub;		//计沮讽玡腹
	long	lCurrentPos;		//计沮讽玡竚
	long	lCurrentSize;		//讽玡计沮
	BYTE	pData[COMM_UPDATE_PACKSIZE_54];
}SENDFILEINFO;

typedef		struct		tagSYSTEM
{
		DWORD		dwFlag; 
		WORD	    wDevice;  //ARM设备类型范围(0x1000~0x1FFF)
		WORD	    wFileType;        
		DWORD	    adwHwVersion[2];
		DWORD	    adwSwVersion[2];
		DWORD		dwReserved[106];   //Data pading
} SYSTEM;


class	CtmDsp54UpdateProtocol:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmDsp54UpdateProtocol)
	public:
		CtmDsp54UpdateProtocol();
		~CtmDsp54UpdateProtocol();
	public:
	    int		InitSend();
		int		InitRev();
	protected:
	    int     m_nStartPos	    ;
	    int     m_nPackSizePos	;
	    int     m_nTypePos		;
	    int     m_nCMDPos	    ;
	    int     m_nIDPos		;
	    int     m_nDataLenPos   ;
	    int     m_nCRCPos       ;
	    int     m_nEndPos	    ;
};


class	CtmDsp54Update:  public CtmDsp54UpdateProtocol
{
	DECLARE_DYNCREATE(CtmDsp54Update)
	public:
		CtmDsp54Update();
		~CtmDsp54Update();
	
	public:
		int		ComposeProtocol	(int nCmd, void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);
		int		CreateSelf()	;
		int     FreeSelf()      ;
		void	GetEcho()		;
		void	GetReadData();
	private:
		int		m_nConnetSate;
		WORD	m_wCommandIndex;
		WORD	m_wCmdState;
		 int    m_nMaxLen;					//ビ叫ず
		 int	m_nMaxPackSize;				//程Data跋办计沮(Ω穝计沮程data)
	    BYTE*	m_pData;
	    BYTE	m_cState;
	    int		m_nCommandIndex;
	    WORD	m_wDLState;					//DownLoad and UpLoad State use bit show state
	    SENDFILEINFO	m_SendFileInfo;
	    CMDQUEUE m_CmdQueue;
	    
	    WORD	m_wGetCmdState;
	    WORD	m_wSetCmdState;
	    //char 	m_cRevFileName[3][5];		//NAME(VVVV-YMDD-TIME)
	    //char 	m_cSendFileName[3][5];		//NAME(VVVV-YMDD-TIME)
	    WORD	m_wRevFileName[3];			//NAME(VVVV-YMDD-TIME)
	    WORD	m_wRevNewFileName[3];		//NAME(VVVV-YMDD-TIME)
	    WORD	m_wSendFileName[3];			//NAME(VVVV-YMDD-TIME)
	    WORD	m_wUpdateType;				//0:main 1:burn
	    SYSTEM  m_sysFileInfo;
	    SYSTEM  m_sysRevFileInfo;
	    SYSTEM	m_sysParseFileInfo;
	    WORD	m_wUploadFlag;
	    WORD	m_wBurnDeviceType;
	    WORD	m_wCheckDSP55Type;
	    char	m_cszSuffixName[8];
	    char	m_cszSendFileName[32];
	    char	m_cszRevFileName[32];
	    char	m_cszRevNewFileName[32];
	    int		m_nRevNewFileNameLen;
	    int		m_nFileNameLen;
	    WORD	m_wRecvPacketCount;
	    BOOL	m_bTransferEcho;
	public:
		void 	SendDLStart(void* pInData = NULL);
		void	SendDLData();
		void	SendDLEnd();
		void	SendExit();
		void	TransferToDsp28();
		void	TurnToNormal();
		void	SendDLReady();
		void	SendDLSmall(void* pInData = NULL);
		void	Update_Run();				//James add 2010/4/27 for run timer
		
		void	GetDLStart();
		void	GetDLData();
		void	GetDLEnd();
		void	GetDLReady();
		void	GetDLSmall();
		
		void	GetDLStepState(void* pOutData);
		void	GetDLOldVers(void* pOutData);
		void	SetDLOldVers(void* pInData);
		int		TOStorageType(int type);
		int		HighToLow(BYTE* pData, long lSize);
		
		int 	SaveDatatoStorage(BYTE* pData, long lStartAddr, long lSize, tmStorageType StorageType);
		int		SendDatatoStorage(BYTE* pData, long lStartAddr, long lSize, tmStorageType StorageType);
	 	void	Echo(WORD wCode);
    private:
        BOOL    DataIsValid(void* pInData, int nInputLen);   
        void	WriteToken(BYTE* pch, int nCount, int* pnWriteCount, WORD wFirstID = 0);
        BOOL    SaveDataToDB(void* buf, int nLen, WORD wFirstID);
        BOOL    DoDealFunc(WORD wCmd);
        BOOL	PopCmd(CMDDATA* pcmd);
        BOOL	PushCmd(CMDDATA* pcmd);
};

typedef struct tagCOMMMAP
{
    WORD	wCmd;
    void	(CtmDsp54Update::*npFunc)();
} COMMMAP;

#ifndef	D_BEOBJECT_CTMDSP54UPDATE
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
