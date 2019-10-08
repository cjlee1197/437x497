#ifndef	D__TMDSP54ALL_V01
#define	D__TMDSP54ALL_V01

#include	"../../tmprotocol.h"
#include	"../../oil54struct.h"
#include	"../libComm.h"
#include	<queue>

class	CtmDsp54Protocol:  public CtmProtocol
{
	DECLARE_DYNCREATE(CtmDsp54Protocol)
	public:
		CtmDsp54Protocol();
		~CtmDsp54Protocol();
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


class	CtmDsp54All:  public CtmDsp54Protocol
{
	DECLARE_DYNCREATE(CtmDsp54All)
	public:
		CtmDsp54All();
		~CtmDsp54All();
	
	public:
		int		ComposeProtocol	(int nCmd, void* pOutData,	void* pInData, int nInputLen, void* pEx = NULL);
		int		ParseProtocol	(void* pInData,		int nInputLen);
		int		CreateSelf()	;
		int     FreeSelf()      ;
	private:
	    int      m_nMaxLen;
	    BYTE*	 m_pData;
	    BYTE	 m_cState;
	    BYTE	 m_cStateRx;
        BYTE	 m_cStateTx;
        BOOL     m_bGotConfig;
        BOOL	 m_bGotDA;
        BOOL	 m_bNewDA;
        WORD     m_wCmdState;					//當前CMD的狀態  CMD_SEND or CMD_ECHO
        WORD     m_wCmdSend;					//發送CMD時候的緩存
        int      m_nCmdNull;
        int      m_nEchoCmdNull;
   	int 	m_nNum;//20140613
        BOOL     m_bSysReset;
        BOOL    m_bAutoKey;
        BOOL    m_bSendAutoKey;
		BOOL		m_bOLDLUB;
		BOOL		m_b3LUB;
		BOOL 		m_bPowerOnLub;
		BOOL		m_bPowerOnLubHr;
		BOOL  	m_bSDOOR;
		BOOL  	m_b2SDOOR;
		BOOL  	m_bDC;
		BOOL  	m_bALogin;
		BOOL  	m_bStepLub;
		BOOL    m_bColdStartFlag;
		BOOL    m_bCOM2Vlv;
		BOOL    m_bInjSpdUnit;
		BOOL		m_bSVRT,bINJBlow, m_b28Clmp;
		BOOL		bGearDist; 
		BOOL	bUnitChangFlag;
		BOOL  bPBSelected;
		BOOL  bPBInitFlag;
		int    nPresUnitChge;
		int	  		bTempUnitFlag;//wangli2014-4-21 14:53:41
	    int     nTempUnitChge;
        int		m_bAmmeter;
        int		 m_nCommandIndex;
        int		 m_nSendRM;
	    int		 m_nRevRM;
		  int   	m_nAirPCPoint[7];
		  char  	m_AirPCDataID[256][256];
		  char    m_ValPBDateID[256];
	    WORD	wOperationModeOld;
	    BOOL	 m_bSendRProfile01;
        BOOL	 m_bSendRBProfile01;
        BOOL	 m_bGetPanlEcho;
        DWORD	 m_dwInjectTime[6];
        BYTE	 m_bSendDataCounter;
        WORD	 m_wGetDataCounter;
        CMDDATA  m_CmdPanelBreak;
        CMDQUEUE m_CmdQueue;
        CMDQUEUE m_CmdEchoQueue;
        CMDDATA  m_CmddData;					//單筆發送數據緩存
        CMDDATA  m_CmdEchoData;					//單筆發送數據緩存
        CMDDATA  m_CmdDSPUpData;					//DSP XPLC上傳資料緩存
        TRANSMITQUEUE	m_Transmit;				//轉發資料緩存 50
        TRANSMITDATA    m_TransmitBuffer;		//轉發資料臨時緩存
        BOOL	 m_bTransmitCmdEcho;			//是否受到dsp54 echo
        BOOL	 m_bTransmitGetDataFlag;		//收到dsp54轉發資料
        BOOL	 m_bOld5530Flag;
		BOOL	m_bOld5530Flag2;
        BOOL 	m_bOld5580OrNew;
        tmDB_AUSTONDRIVEINFO	m_dbAustonDriveInfo;
        
        tmDB_CHART1		m_dbchartInject;
        tmDB_CHART1		m_dbchartHold;
        tmDB_CHART1		m_dbchartInjectB;
        tmDB_CHART1		m_dbchartHoldB;
        tmDB_CHART1		m_dbchartChrg;
        tmDB_CHART1		m_dbchartInjectLmt;
        tmDB_CHART1		m_dbchartHoldLmt;
        tmDB_DIAGRMRI	m_dbdiagrmri;
        
        long long		m_lSendTransCount;
        long long		m_lGetTransCount;
        WORD	wMoldHeaterStatusOld;
        
         //add by J.Wong 2016/8/23 16:00:46
        queue<TRANSMITDATA>	m_Transmit_Cache;
       	pthread_mutex_t		m_cache_lock;       
        
    private:
    	void    SendRequestRead(WORD wCommand)						;		//MMI請求讀取數據
    	void    SendUpdateData(ETHNETFUNCTX ethnetFuncTX)			;		//MMI請求寫數據
    	
        void	SendConfig()                                        ;
        void	SendZeroSet()                                       ;
        void	SendConnect()										;
        void	SendRequestConfig()                                 ;
        void	SendPanel(BYTE cKey)                                ;
        void	SendControl()                                       ;
        void	SendControlIndex(char* pszData)                     ;
        void	SendControlIndex(int nIndex)						;
        void	SendCommand(CMDDATA* pcmd)                          ;
        void	SendPanlCommand()									;
        BOOL	PushCmd(CMDDATA* pcmd)                              ;
        BOOL	PushEchoCmd(CMDDATA* pcmd)                          ;
        BOOL	PushTransmitCmd(TRANSMITDATA* pTransmit)           ;
        void	SendControlMachine(WORD	wType=1)                    ;
        void	SendControlMoldset(WORD	wType=1)                    ;
        void	SendControlNewMachine2(WORD wType=1)				;
        void	SendControlNewMoldset2(WORD	wType=1)				;
        void	SendControlNewMachine3(WORD wType=1)				;
        void	SendControlNewMoldset3(WORD	wType=1)				;
        void	SendControlMonitor()                                ;
        void	SendControlMachineB()                               ;
        void	SendControlMoldsetB(WORD	wType=1)                ;
        void	SendControlMonitorB()                               ;
        void	SendPowerMatch    ()                                ;
        //int		SendDiagRM(DIAGRM diagrm)                       ;
        void	SendDiagWM(DIAGWM diagwm)                           ;
        //void	SendDiagSM()                                        ;
        int     SendDiagRMRI(DIAGRMRI diagrmri)						;
        //void	SendDiagRI(DIAGRI diagri)                           ;
        void	SendDiagWO(DIAGWO diagwo)                           ;
        void	SendDiagWO(WORD wAddress, WORD wData)               ;
        //void	SendDiagSI()                                        ;
        void	SendAssignInput (WORD wOldAddress, WORD wNewAddress);
        void	SendAssignOutput(WORD wOldAddress, WORD wNewAddress);
        void	SendStatus(WORD wStatus)                            ;
        void	SendTransmit(void*	pInData, int nInputLen)			;
        void	SendTransmitQueue(void*	pInData, int nInputLen)		;
        //void	SendTransmit(char* pszData)                     	;
        void	SendTransmitCmd(long	lCmd)						;
        void	SendTransmitBlock(WORD	wBlockID)					;
        //void	SendRProfile01()                                    ;
        //void	SendSProfile01()                                    ;
        //void	SendRBProfile01()                                   ;
        //void	SendSBProfile01()                                   ;
        
        DWORD	GetInjectTime(int nIndex)							; 
        void	ClearChartInjectCount()     {m_dbchartInject.nIndex = 0;}
        short	GetChartInjectCount()       {return	m_dbchartInject.nIndex;         }
        short*	GetChartInjectX()           {return	m_dbchartInject.nX;             }
        short*	GetChartInjectY1()          {return	m_dbchartInject.nY1;            }
        short*	GetChartInjectY2()          {return	m_dbchartInject.nY2;            }
        
        void	ClearChartHoldCount()       {m_dbchartHold.nIndex = 0;}
        short	GetChartHoldCount()         {return	m_dbchartHold.nIndex;           }
        short*	GetChartHoldX()             {return	m_dbchartHold.nX;               }
        short*	GetChartHoldY1()            {return	m_dbchartHold.nY1;              }
        short*	GetChartHoldY2()            {return	m_dbchartHold.nY2;              }
        
        void	ClearChartInjectBCount()    {m_dbchartInjectB.nIndex = 0;}
        short	GetChartInjectBCount()      {return	m_dbchartInjectB.nIndex;        }
        short*	GetChartInjectBX()          {return	m_dbchartInjectB.nX;            }
        short*	GetChartInjectBY1()         {return	m_dbchartInjectB.nY1;           }
        short*	GetChartInjectBY2()         {return	m_dbchartInjectB.nY2;           }
        
        void	ClearChartHoldBCount()      {m_dbchartHoldB.nIndex = 0;}
        short	GetChartHoldBCount()        {return	m_dbchartHoldB.nIndex;          }
        short*	GetChartHoldBX()            {return	m_dbchartHoldB.nX;              }
        short*	GetChartHoldBY1()           {return	m_dbchartHoldB.nY1;             }
        short*	GetChartHoldBY2()           {return	m_dbchartHoldB.nY2;             }
        
        void	ClearChartChrgCount()       {m_dbchartChrg.nIndex = 0;}	//Sunny<20070815>
        short	GetChartChrgCount()         {return	m_dbchartChrg.nIndex;           }
        short*	GetChartChrgX()             {return	m_dbchartChrg.nX;               }
        short*	GetChartChrgY1()            {return	m_dbchartChrg.nY1;              }
        short*	GetChartChrgY2()            {return	m_dbchartChrg.nY2;              }
        
        void	ClearChartInjectLmtCount()  {m_dbchartInjectLmt.nIndex = 0;}
        short	GetChartInjectLmtCount()    {return	m_dbchartInjectLmt.nIndex;      }
        short*	GetChartInjectLmtX()        {return	m_dbchartInjectLmt.nX;          }
        short*	GetChartInjectLmtY1()       {return	m_dbchartInjectLmt.nY1;         }
        short*	GetChartInjectLmtY2()       {return	m_dbchartInjectLmt.nY2;         }
        
        void	ClearChartHoldLmtCount()    {m_dbchartHoldLmt.nIndex = 0;}
        short	GetChartHoldLmtCount()      {return	m_dbchartHoldLmt.nIndex;        }
        short*	GetChartHoldLmtX()          {return	m_dbchartHoldLmt.nX;            }
        short*	GetChartHoldLmtY1()         {return	m_dbchartHoldLmt.nY1;           }
        short*	GetChartHoldLmtY2()         {return	m_dbchartHoldLmt.nY2;           }
        int		GetCommand()				{return m_nCommandIndex;				}
        
        tmDB_DIAGRMRI*	GetDBDiagRMRI()		{return	&m_dbdiagrmri;}
        //tmDB_DIAGRI*	GetDBDiagRI()		{return	&m_dbdiagri;}
        
        void	InitMessage()				{m_nRevRM = m_nSendRM = 0;}
    public:
    	void	Run();
    	void	GetUpdateData();
    	void	GetReadData();
        void	GetColdStart();
        void	GetWarmStart();
        void	GetConfig();
        void	GetConfigLimit();
        void	GetOPState();
        void	GetOPStatus();
        //void	GetOPMeter();
        //void	GetOPMeterZeroAgain();
        //void	GetOPTemperature();
        //void	GetOPSummary();
        void	GetCurveData();
       	//void	GetControlIndex();
        //void	GetProduction();      
        void	GetDAOutput2();			
        void	GetMoniCurrent();
        void	GetMoniLast();
        void	GetMoniMaxMin();
        void	GetMoniCurrentB();
        void	GetMoniLastB();
        void	GetMoniMaxMinB();
        void	GetDiagRMRI();
        void  	GetAustonDriveInfo();
        void	GetTransmit();
        //void	GetDiagRM();
        //void	GetDiagRI();
        //void	GetSProfile01I();
        //void	GetDProfile01I();
        //void	GetEProfile01I();
        //void	GetSProfile01H();
        //void	GetDProfile01H();
        //void	GetEProfile01H();
        //void	GetSBProfile01I();
        //void	GetDBProfile01I();
        //void	GetEBProfile01I();
        //void	GetSBProfile01H();
        //void	GetDBProfile01H();
        //void	GetEBProfile01H();
        void	GetEcho();
        BOOL	GetDAFlag()				{ return	m_bNewDA; };
    private:
        void	Echo(WORD wCode);
        void	SelTempSendValue();
        BOOL    DataIsValid(void* pInData, int nInputLen);    	
        BOOL	CheckOilVatID(char* pszData);
        void	TransferOilVat();
        void  TransferOilVat(BOOL bTransferMach);
        void  TransferOilVat(BOOL bTransferMSET2,BOOL Flag);
        void	SendOilVat();
        void	SetVatDefPosi();
        void	WriteToken(BYTE* pch, int nCount, int* pnWriteCount, WORD wFirstID = 0);
        BOOL    SaveDataToDB(void* buf, int nLen, WORD wFirstID);
        BOOL    DoDealFunc(WORD wCmd);
        BOOL	PopCmd (CMDDATA* pcmd);
        BOOL	PopEchoCmd (CMDDATA* pcmd);
        BOOL	DelEchoCmd (CMDDATA* pcmd);
        BOOL	PopTransmitCmd (TRANSMITDATA* pTransmit);
        BOOL	PopTransmitCount (int	nCount);
        //BOOL	AddChartInject(tmDB_CHARTSTROKE1* pdbchartstroke1);
        //BOOL	AddChartHold  (tmDB_CHARTSTROKE1* pdbchartstroke1);
        //BOOL	AddChartInjectB(tmDB_CHARTSTROKE1* pdbchartstroke1);
        //BOOL	AddChartHoldB  (tmDB_CHARTSTROKE1* pdbchartstroke1);
        //BOOL  AddChartCharge (tmDB_CHARTSTROKE1* pdbchartstroke1);
				void		SetDefaultEffectPos(); //JOYCE2011-8-26 開模位置有效區初始值
       	WORD		GetCurrentMoldMedia();
				void		SaveLubricate();
				void		ProcessLubricate();
				void		SetPowerOnLubDefault();
    	  void		Check_PCTBL();
    	  void    Check_PBTBL();
				void    SetValMod();
				void	  ClearNotclmptime();
				void		SaveTBARData();
				void    SaveOpnCntData();
				void    SaveParallelData();
				void		SaveTBarParalleltData();
				void		SaveNotclmpTime();/*wangli2015-12-10 17:04:14*/
				void	  SaveWarningCnt();
				int     SetUnitChangetoBar(char pDataID[256]);
				long		ChangeVoltage(char pDataID[256], int ChannelNO, int nValue);
				void		ProsNotclmpTime();
				void	  SetPowonLubHr();/*wangli2016-1-19 14:11:37 開機后  設定值 與 累計值進行比較*/


				void		SetClmpShutOffset();
				void		SetTBarShutDest();
				void		SetTBarShutDest2();
    		void		BootTimeCntDown();
    		//void	  ProcessAmmeterEnergy();
		    BOOL	CheckDataID(char* pszData, char* pszData2[],int Length);
			void    TempAutRecog();
			void    TempAutProcess();
			void    TempAutProcess2();

				
};

typedef struct tagCOMMMAP
{
    WORD	wCmd;
    void	(CtmDsp54All::*npFunc)();
} COMMMAP;

typedef struct tagDB_ADCONFIG
{
    WORD	    wPhysicalLength;
    WORD	    wTravel;
} DB_ADCONFIG;

#ifndef	D_BEOBJECT_CTMDSP54ALL
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif
