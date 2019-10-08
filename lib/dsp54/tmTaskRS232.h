/*===========================================================================+
|  Class    : TaskComm                                                       |
|  Task     : Communication Service Routine header file                      |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/13/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKRS232
#define     D_TASKRS232

#include    "../../taskcomm.h"
#include    "../../common.h"
#include    "../../rs232.h"
#include	"../../tmcontrol.h"
//#include	"control.h"
#include	"../../taskhost.h"
#include	"../libComm.h"
#include	"../../rs485.h"//JOYCE2010-12-16
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     COMM_RETRY                  3
#define     COMM_STATE_HALT             0x00
#define     COMM_STATE_RUN              0x01
#define     COMM_STATE_MACHINE          0x02
#define     COMM_STATE_RESET            0x10
#define     COMM_STATE_CONFIG           0x11
#define     COMM_STATE_ECHO             0x12
#define     COMM_STATE_WAIT00           0x01
#define     COMM_STATE_WAIT01           0x02
#define     COMM_STATE_WAIT02           0x04
#define     COMM_STATE_WAIT03           0x08
#define     COMM_STATE_WAIT04           0x10
#define     COMM_STATE_WAITB01          COMM_STATE_WAIT03    //Chuntzu 2004/11/18 add
#define     COMM_STATE_WAITB02          COMM_STATE_WAIT04
#define     COMM_STATE_WAIT05           0x20
#define     COMM_STATE_WAIT06           0x40
#define     COMM_STATE_WAIT07           0x80
#define     COMM_SIZEDIAGWM             8
#define     MAX_COMMAND_KEEP            100      // St0527'03 for Command send

#define     tmMAX_POINT_PB				192
#define     tmMAX_POINT_PC				192
#define     tmASSIGN_MASK 				0x01FF
#define     tmASSIGN_NEWIO				0x1000
#define     tmASSIGN_OLDIO				0x2000
#define     tmASSIGN_NODEF				0x8000

#define     tmMAX_GROUP_PC				3

//********************
//				BLOCK ID
//********************
#define		BLOCK_MACHINE_ID			1
#define		BLOCK_MOLDSET_ID			2
#define		BLOCK_POWERMATCH_ID     	3
#define		BLOCK_CONFIG_ID         	4
#define		BLOCK_STATE_ID          	5
#define		BLOCK_STATUS_ID         	6
#define		BLOCK_METER_ID          	7
#define		BLOCK_TEMP_ID           	8
#define		BLOCK_DAOUT_ID          	9
#define		BLOCK_MONIMAXMIN_ID     	10
#define		BLOCK_MACHINEB_ID       	11
#define		BLOCK_MOLDSETB_ID       	12
#define		BLOCK_MONIMAXMINB_ID    	13
#define		BLOCK_ZSET_ID           	14
#define		BLOCK_MONICURRENT_ID        15
#define		BLOCK_MONILAST_ID           16
#define		BLOCK_MONIBCURRENT_ID		17
#define		BLOCK_MONIBLAST_ID          18
#define		BLOCK_MOLDSETX_ID			19
#define		BLOCK_DAOUT2_ID				20
#define		BLOCK_POWERMATCH2_ID     	21
#define		BLOCK_NEWDAOUT_ID			22
#define		BLOCK_NEWPOWERMATCH_ID     	23
#define		BLOCK_OPERATION_ID			24

//COM2 used	James add 2008/9/28
#define		BLOCK_COM2TEMPSET_ID		63
#define		BLOCK_COM2TEMPRAMP_ID		77
#define		BLOCK_COM2TEMP_ID			78
#define		BLOCK_CONFIG2_ID			79
//Andy 20091208
#define		BLOCK_STATE2_ID          	168
#define		BLOCK_COM2TEMPSETPERCENT_ID 176
#define		BLOCK_COM2GATE_ID          	177
#define		BLOCK_COM2STATE_ID         	178
#define		BLOCK_COM2METER_ID          179

#define		BLOCK_COM2GATEB_ID          181
#define		BLOCK_COM2ZSET_ID          	182

#define     tmMAX_DIAGRM				16
#define     tmMAX_DIAGRI				1
#define     CURRENT_MOLD_MEDIA      "MHDR_MODULE_NULL_NULL_MODULEBYMEDIA"

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class  CtmTaskRS232: public CTaskHost
{
	DECLARE_DYNCREATE(CtmTaskRS232)
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmTaskRS232();
    virtual ~CtmTaskRS232();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		static 	void			_Run(void *pData);
		static	CtmTaskRS232*	m_pSelf;
		int		CreateSelf();
		int		FreeSelf();
private:
    CRS232*  m_prs232;
    CRS485*  m_prs485;//JOYCE2010-12-16
    int		m_nID;
    int		m_nID2;//JOYCE2010-12-16
    BYTE	m_cState;
    BYTE	m_cStateRx;
    BYTE	m_cStateTx;
   	BOOL	m_bCloseFlag;//JOYCE2011-2-11 
    //static	CtmTaskRS232*		m_pSelf;//JOYCE2011-2-11 
    sem_t	ThreadWait;     	
    long	m_lCounter;
    long	m_lCounterW0;
    long	m_lCounterW1;
    long	m_lCounterBW1;
    long	m_lCounterReqConfig;        //wy040600 wy113099
    long	m_lCounterShtCntEcho;//JOYCE 2011-8-22
    long	m_lCounter485State;//JOYCE2010-12-16 
    long 	m_lCounter485ReqS;//JOYCE2010-12-16 
    long 	m_lCounter485ReqC;
    BOOL	bGotConfig;                 //wy040600 wy113099
    BOOL	m_bGotDA;                  //St1114'01 for D/A
    BOOL	m_bNewDA;
    short	m_nReqConfigTimes;         //St041100
    BOOL	m_bSendRProfile01;
    BOOL	m_bSendRBProfile01;
		int		m_nCommandIndex;		//yangpeng 2007/4/7 02:13下午
		DWORD	m_dwInjectTime[6];		// James add 2008/3/14 04:21下午  射出計時

		TLED	 m_tled;
    WORD     m_wCmdState;
    WORD     m_wCmdSend;
    int      m_nCmdNull;
    CMDDATA  m_CmddData;
    CMDDATA  m_CmdPanelBreak;
    CMDQUEUE m_CmdQueue;
    BOOL     m_bMachineSend;
    BOOL     m_bSysReset;          //Steven20040202 change to standard
		
		BOOL    m_bUseSlow7GL;  	//JOYCE2008-9-24 ADD
		BOOL    m_bAdjustUse7GL;	//JOYCE2008-9-24 ADD
		BOOL		m_bAutoAdjust7GL; //JOYCE2008-9-24 ADD
		BOOL		m_bKeySuck7GL; 		//JOYCE2008-9-24 ADD
		BOOL		m_bMUCELL;				//JOYCE2010-12-16 
		BOOL		m_bMUCELL2;				//JOYCE2012-6-29  
		BOOL		m_b3LUB;
		BOOL		m_bOLDLUB;
		BOOL  	m_bSDOOR;
		BOOL  	m_b2SDOOR;
		BOOL  	m_bENERGY;
		BOOL  	m_bDC;
		BOOL  	m_bALogin;
		BOOL  	m_bStepLub;
		BOOL		m_bInjPurgeMsg;//JOYCE2011-4-26 for mucell
		BOOL		m_bMucellRun;		//fuxy, 2012-3-15 
		BOOL    m_bWarmStartFlag;
		BOOL    m_bColdStartFlag;
		BOOL    m_bCOM2Vlv;
    BOOL		m_bAustoneState;	//JOYCE2011-2-11 
		BOOL    m_bAustoneCfgFlag;
		BOOL    m_bN2CFlag;
		BOOL		m_bSVRT;
		BOOL		m_bInjSpdUnit;
		BOOL	bUnitChangFlag;
		int    nPresUnitChge;
		int	  		bTempUnitFlag;//wangli2014-4-21 14:53:41
	    int     nTempUnitChge;
		

		int   	m_nAirPCPoint[7];
		char  	m_AirPCDataID[256][256];
		long		lWaitCount; 		//Andy 20090604
		WORD		wSendMoldStatus;//Andy 20090604
		WORD		wSendConfStatus;//Andy 20090604
		WORD		wSendBlockStatus;//
		WORD		wOldCMD;
		WORD		m_wLubrCnt;//JOYCE2011-1-8 
		WORD 		wShotCntFlag;//JOYCE 2011-8-22
		DWORD	  dwOldCmd;//JOYCE 2011-8-22
		
    tmDB_CHART1		m_dbchartInject;
    tmDB_CHART1		m_dbchartHold;
    tmDB_CHART1		m_dbchartInjectB;
    tmDB_CHART1		m_dbchartHoldB;
    tmDB_CHART1		m_dbchartChrg;		   //Sunny<20070815>
    tmDB_CHART1		m_dbchartChrgB;
    tmDB_CHART1		m_dbchartInjectLmt;
    tmDB_CHART1		m_dbchartHoldLmt;
    
    tmDB_DIAGRM		m_dbdiagrm;
    tmDB_DIAGRI		m_dbdiagri;
    
    TRANSMITQUEUE	m_Transmit;				//轉發資料緩存 50         //JOYCE2011-2-11 
    TRANSMITDATA  m_TransmitBuffer;		//轉發資料臨時緩存      //JOYCE2011-2-11 
    BOOL	 		    m_bTransmitCmdEcho;		//是否受到dsp54 echo  //JOYCE2011-2-11 
    tmDB_AUSTONDRIVEINFO	m_dbAustonDriveInfo; //JOYCE2011-2-11 
    //	fans add 2008/6/16 11:18上午
	int				m_nSendRM;
	int				m_nRevRM;
	pthread_t 		m_RunThreadID;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    //==============
    //	override
    //==============
    int		ReqValues(int nType);
    int		ReqValues(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
    int		ReqValues(int nType, void* pOutData, int nSize);
    
    int 	ReqValues(int nType, int nValue1, int nPrecision1, int nValue2, int nPrecision2);//JOYCE2010-12-16 

    int		WriteValue(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
    int		WriteValue(int nType, void* pInputData, int nSize);
    int		WriteValue(int nDataID);
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void	Run();
    static	void	Run_Pthread(void* pClass, char *pData, int nLength);
    void	SetStatusWindow(CtmWnd* pwndStatus);
        	
    void	SendConfig()                                        ;
    void	SendZeroSet()                                       ;
    void	SendRequestConfig()                                 ;   //wy040600 wy113099
    void	SendPanel(BYTE cKey)                                ;
    void	SendControl()                                       ;
    void	SendBlock61()											                  ;  //Andy 20090619
    void	SendControlIndex(char* pszData)                     ;
    void	SendControlIndex(int nIndex)												;
    void	SendCommand()                                       ;
    BOOL	PushCmd(CMDDATA* pcmd)															;
    BOOL	PushTransmitCmd(TRANSMITDATA* pTransmit)           	;	//JOYCE2011-2-11  for austone
    void	SendTransmit(void*	pInData, int nInputLen)				  ;	//JOYCE2011-2-11  for austone 
    void	SendTransmitQueue(void*	pInData, int nInputLen)		  ;	//JOYCE2011-2-11  for austone 
    void	SendControlMachine()                                ;
    void	SendControlMoldset()                                ;
    void	SendControlMonitor()                                ;
    void	SendControlMachineB()                               ;
    void	SendControlMoldsetB()                               ;
    void	SendControlMonitorB()                               ;
    void	SendPowerMatch    ()                                ;   // St1108'01 for D/A
    int		SendDiagRM(DIAGRM diagrm)                           ;
    void	SendDiagWM(DIAGWM diagwm)                           ;
    void	SendDiagSM()                                        ;
    void	SendDiagRI(DIAGRI diagri)                           ;
    void	SendDiagWO(DIAGWO diagwo)                           ;
    void	SendDiagWO(WORD wAddress, WORD wData)               ;
    void	SendDiagSI()                                        ;
    void	SendAssignInput (WORD wOldAddress, WORD wNewAddress);
    void	SendAssignOutput(WORD wOldAddress, WORD wNewAddress);
    void	SendStatus(WORD wStatus)                            ;
    void	SendRProfile01()                                    ;
    void	SendSProfile01()                                    ;
    void	SendRBProfile01()                                    ;
    void	SendSBProfile01()                                    ;
    DWORD	GetInjectTime(int nIndex)							;  //James add 2008/3/14 04:24下午
    void	ProcessShotCnt()																		; //JOYCE2011-8-22 DSP54計開模數
    void	ProcessAmmeterEnergy();
    	
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
	
	void	ClearChartChrgBCount()       {m_dbchartChrgB.nIndex = 0;}	//Sunny<20070815>
	short	GetChartChrgBCount()         {return	m_dbchartChrgB.nIndex;           }
	short*	GetChartChrgBX()             {return	m_dbchartChrgB.nX;               }
	short*	GetChartChrgBY1()            {return	m_dbchartChrgB.nY1;              }
	short*	GetChartChrgBY2()            {return	m_dbchartChrgB.nY2;              }

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
	
	tmDB_AUSTONDRIVEINFO*	GetDBAustonDrive()		{return	&m_dbAustonDriveInfo;} //JOYCE2011-2-11 
	
	tmDB_DIAGRM*	GetDBDiagRM()		{return	&m_dbdiagrm;}
	tmDB_DIAGRI*	GetDBDiagRI()		{return	&m_dbdiagri;}
	
	//	fans add 2008/6/16 03:23下午
	void	InitMessage()				{m_nRevRM = m_nSendRM = 0;}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
public:
    void	GetColdStart();
    void	GetWarmStart();
    void	GetConfig();
    void	GetOPState();
    void	GetOPState2();//Andy 20091208
    void	GetOPStatus();
    void	GetOPMeter();
    void	GetOPMeterZeroAgain();   //wy042099 for run-time zero setting
    void	GetOPTemperature();
    void	GetOPSummary();          //St020101 for update quickly 
    void	GetCurveData();          //Chuntzu 2005/1/20 add for new curve
    void	GetControlIndex();
//	void	GetCommand();
    void	GetProduction();
    void	GetDAOutput();           // St1108'01 for D/A
    void	GetDAOutput2();			//Andy 20061212 for New D/A
    void	GetMoniCurrent();
    void	GetMoniLast();
    void	GetMoniMaxMin();
    void	GetMoniCurrentB();
    void	GetMoniLastB();
    void	GetMoniMaxMinB();
    void	GetDiagRM();
    void	GetDiagRI();
    void	GetSProfile01I();
    void	GetDProfile01I();
    void	GetEProfile01I();
    void	GetSProfile01H();
    void	GetDProfile01H();
    void	GetEProfile01H();
    void	GetSBProfile01I();
    void	GetDBProfile01I();
    void	GetEBProfile01I();
    void	GetSBProfile01H();
    void	GetDBProfile01H();
    void	GetEBProfile01H();
    void	GetEcho();
    BOOL	GetDAFlag()				{ return	m_bNewDA; };
		void	GetAustonDriveInfo(); //JOYCE2011-2-11
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
	
private:
    void	Echo(WORD wCode);
    void	Reset();
    void	Rs485Reset();//JOYCE2010-12-16 
    void	SelTempSendValue();
    BOOL	CheckOilVatID(char* pszData);
    void	TransferOilVat();
    void    TransferOilVat(BOOL bTransferMach);//Andy 20070520 add;
    void	SendOilVat();
    void	SetVatDefPosi();
    void	LED();
    WORD	WriteToken(BYTE* pch, int nCount, int* pnWriteCount);
    BOOL	PopCmd (CMDDATA* pcmd);
    BOOL	AddChartInject(tmDB_CHARTSTROKE1* pdbchartstroke1);
	BOOL	AddChartHold  (tmDB_CHARTSTROKE1* pdbchartstroke1);
	BOOL  AddChartCharge (tmDB_CHARTSTROKE1* pdbchartstroke1);		//Sunny<20070815>
	BOOL	AddChartInjectB(tmDB_CHARTSTROKE1* pdbchartstroke1);
	BOOL	AddChartHoldB  (tmDB_CHARTSTROKE1* pdbchartstroke1);
	BOOL  AddChartChargeB(tmDB_CHARTSTROKE1* pdbchartstroke1);
    BOOL	CheckDataID(char* pszData, char* pszData2[],int Length);//wangli2014-8-20 16:32:50
	
	BOOL	PopTransmitCmd (TRANSMITDATA* pTransmit);	//JOYCE2011-2-11  for austone
    BOOL	PopTransmitCount (int	nCount);						//JOYCE2011-2-11  for austone
	static 	void	ResValueFunc(void* pClass, char *pData, int nLength);//JOYCE2011-2-11 

	WORD	test2USB(WORD wOp,void* pOutData=NULL, int nSize=0);
	void		SaveLubricate();  //JOYCE2011-8-22 lubricate
	void		ProcessLubricate();  //JOYCE2011-8-22 lubricate
	void		SetDefaultEffectPos(); //JOYCE2011-8-26 開模位置有效區初始值
	void		SetPowerOnLubDefault();
	void		Check_PCTBL();
	void		ReadADConfig();
	void		SaveADConfig();
	void		ProcessADConfig();
	void		SaveTBARData();
	void    SaveParallelData();
	int     SetUnitChangetoBar(char pDataID[256]);
	void		SetClmpShutOffset();
	void		SetTBarShutDest();
	void		SetTBarShutDest2();
	WORD		GetCurrentMoldMedia();

    //MuCell; //JOYCE2010-12-16 
public:
	void	CreaMuCell();
	float	Get_B(BYTE*	psz, DWORD	dwLen);
	float	Get_C(BYTE*	psz, DWORD	dwLen);		//fuxy, 2012-3-13, add 
	float	Get_E(BYTE*	psz, DWORD	dwLen);
	float	Get_G(BYTE*	psz, DWORD	dwLen);
	float	Get_H(BYTE*	psz, DWORD	dwLen);
	float	Get_J(BYTE*	psz, DWORD	dwLen);
//	float	Get_L(BYTE*	psz, DWORD	dwLen);		//fuxy, 2011-4-21, mark 
	float	Get_P(BYTE*	psz, DWORD	dwLen);		  //fuxy, 2011-4-21, add 
	float	Get_R(BYTE*	psz, DWORD	dwLen);
	float	Get_S(BYTE*	psz, DWORD	dwLen);
	float	Get_T(BYTE*	psz, DWORD	dwLen);
	
	void	Request_B(int	nValue, int nPrecision);
	void	Request_C(int	nValue, int nPrecision);	//fuxy, 2012-3-13, add 
	void	Request_E(int	nValue, int nPrecision);
	void	Request_G(int	nValue, int nPrecision);
	void	Request_H(int	nValue, int nPrecision);
	void	Request_J(int	nValue, int nPrecision);
//	void	Request_L(int	nValue1, int nValue2, int nPrecision1, int nPrecision2);	//fuxy, 2011-4-21, mark 
	void	Request_P(int	nValue, int nPrecision);	//fuxy, 2011-4-21, add 
	void	Request_R(int	nValue, int nPrecision);
	void	Request_S(int	nValue, int nPrecision);
	void	Request_T(int	nValue, int nPrecision);
	
	int		RS485ReadToken();
	void	SetPonit(BYTE*	psz, WORD wPrecision, WORD wLen);

  /*JOYCE2012-6-29 for mucell_2--------*/
	float	Get_C2(BYTE*	psz, DWORD	dwLen);
	float	Get_D2(BYTE*	psz, DWORD	dwLen);
	float	Get_F2(BYTE*	psz, DWORD	dwLen);
	float	Get_M2(BYTE*	psz, DWORD	dwLen);
	float	Get_S2(BYTE*	psz, DWORD	dwLen);
	float	Get_U2(BYTE*	psz, DWORD	dwLen);

	void	Request_C2(int	nValue, int nPrecision);
	void	Request_D2(int	nValue, int nPrecision);
	void	Request_F2(int	nValue, int nPrecision);
	void	Request_M2(int	nValue, int nPrecision);
	void	Request_S2(int	nValue, int nPrecision);
	void	Request_U2(int	nValue, int nPrecision);
  /*JOYCE2012-6-29 for mucell_2--------*/
};

typedef struct tagCOMMMAP
{
    WORD	wCmd;
    void	(CtmTaskRS232::*npFunc)();
} COMMMAP;
    
typedef struct tagDB_ADCONFIG
{
    WORD	    wPhysicalLength;
    WORD	    wTravel;
} DB_ADCONFIG;

#ifndef	D_BEOBJECT_CTMTASKRS232	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

