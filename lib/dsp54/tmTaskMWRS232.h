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

#ifndef     D_TASKMWRS232
#define     D_TASKMWRS232

#include    "../../taskcomm.h"
#include    "../../common.h"
#include    "../../rs232.h"
#include	"../../tmcontrol.h"
#include	"../../taskhost.h"
#include	"tmTaskRS232.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
//********************
//COM2 BLOCK ID
//********************
#define		BLOCK_COM2DAOUT_ID          	325
#define		BLOCK_COM2POWERMATCH_ID     	326
#define		BLOCK_COM2DAOUT2_ID				    327
#define		BLOCK_COM2POWERMATCH2_ID     	328
#define		BLOCK_COM2NEWPOWERMATCH_ID    329
#define		BLOCK_COM2NEWDAOUT_ID					330
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class  CtmTaskMWRS232: public CTaskHost
{
	DECLARE_DYNCREATE(CtmTaskMWRS232)
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CtmTaskMWRS232(WORD wChannel = _COM2);
    virtual ~CtmTaskMWRS232();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		static	CtmTaskMWRS232*		m_pSelf;
private:
    CRS232*  m_prs232;
    tmRS232       rs232;	
    int		m_nID;
    BYTE	m_cState;
    BYTE	m_cStateRx;
    BYTE	m_cStateTx;
        	
    long	m_lCounter;
    long	m_lCounterW0;
    long	m_lCounterW1;
    long	m_lCounterBW1;
    long	m_lCounterReqConfig;        //wy040600 wy113099
    long	m_lCounterClsOpn;           //JOYCE2011-1-27 
    BOOL	bGotConfig;                 //wy040600 wy113099
    BOOL	m_bGotDA;                  //St1114'01 for D/A
    BOOL	m_bNewDA;
    BOOL    m_bNewDA2;   				//Andy 20070314
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
    BOOL     m_bCOM2;			   //James add 2008/7/3 COM2使用的Flag
    BOOL     m_bResetClsOpn;	//JOYCE2011-1-27
    BOOL     m_bDACOM2;
    BOOL     m_bDC;
    BOOL     m_bMold40;
    
    BYTE		g_acRxBuffer[4096];		//James move from gloab var
	BYTE		g_acTxBuffer[4096];
	//COMMFRAMERX	g_frameRx;
	//COMMFRAMETX	g_frameTx;

    tmDB_CHART1		m_dbchartInject;
    tmDB_CHART1		m_dbchartHold;
    tmDB_CHART1		m_dbchartInjectB;
    tmDB_CHART1		m_dbchartHoldB;
    tmDB_CHART1		m_dbchartChrg;		//Sunny<20070815>
    tmDB_CHART1		m_dbchartInjectLmt;
    tmDB_CHART1		m_dbchartHoldLmt;
    tmDB_DIAGRM		m_dbdiagrm;
    tmDB_DIAGRI		m_dbdiagri;


	//	fans add 2008/6/16 11:18上午
	int				m_nSendRM;
	int				m_nRevRM;
	pthread_t 			m_RunThreadID;
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
    static 		void		_Run(void *pData);
    
    int		WriteValue(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
    int		WriteValue(int nType, void* pInputData, int nSize);
    int		WriteValue(int nDataID);
  
public:
    void	Run();
    static	void	Run_Pthread(void* pClass, char *pData, int nLength);
    void	SetStatusWindow(CtmWnd* pwndStatus);
        	
    void	SendConfig()                                        ;
    void	SendZeroSet()                                       ;
    void	SendRequestConfig()                                 ;   //wy040600 wy113099
    void	SendPanel(BYTE cKey)                                ;
    void	SendControl()                                       ;
    void	SendControlIndex(char* pszData)                     ;
    void	SendControlIndex(int nIndex)						;
    void	SendCommand()                                       ;
    BOOL	PushCmd(CMDDATA* pcmd);
    void	SendControlMachine()                                ;
    void	SendControlMoldset()                                ;
    void	SendControlMonitor()                                ;
    void	SendControlMachineB()                               ;
    void	SendControlMoldsetB()                               ;
    void	SendControlMonitorB()                               ;
    void	SendPowerMatch    ()                                ;   // St1108'01 for D/A
    int		SendDiagRM(DIAGRM diagrm)                           ;	// fans modify 2008/6/16 11:17上午
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
    BOOL	GetDAFlag()				{ return	m_bNewDA2; };

	
private:
    void	Echo(WORD wCode);
    void	Reset();
    void	ResetClose();//JOYCE2011-1-27
    void	ResetOpen(); //JOYCE2011-1-27
    void	SelTempSendValue();
    static	void		ResValueFunc(void* pClass, char *pData, int nLength);
        	
    BOOL	CheckOilVatID(char* pszData);
    void	TransferOilVat();
    void    TransferOilVat(BOOL bTransferMach);//Andy 20070520 add;
    void	SendOilVat();
    void	SetVatDefPosi();
    void	LED();
    void	WriteToken(BYTE* pch, int nCount, int* pnWriteCount);
    BOOL	PopCmd (CMDDATA* pcmd);
    BOOL	AddChartInject(tmDB_CHARTSTROKE1* pdbchartstroke1);
		BOOL	AddChartHold  (tmDB_CHARTSTROKE1* pdbchartstroke1);
		BOOL	AddChartInjectB(tmDB_CHARTSTROKE1* pdbchartstroke1);
		BOOL	AddChartHoldB  (tmDB_CHARTSTROKE1* pdbchartstroke1);
		BOOL  AddChartCharge (tmDB_CHARTSTROKE1* pdbchartstroke1);		//Sunny<20070815>
};

int		DspBufferChange(BYTE* pcData, char* cszDBID, DBVALUE SetdbValue, WORD wBlockID);

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagCOMMMAP2
    {
    WORD	wCmd;
    void	(CtmTaskMWRS232::*npFunc)();
    } COMMMAP2;

#ifndef	D_BEOBJECT_CTMTASKMWRS232	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif
   
#endif

