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

#ifndef     D_TASKCOMM
#define     D_TASKCOMM

#include    "task.h"
#include    "rs232.h"
#include    "database.h"
#include    "wnd.h"
//#include    "dbstat.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     COMM_RETRY                  3
#define     COMM_NULL                   0x0000

#define     COMM_SENDCONFIG             0x3130
#define     COMM_SENDZEROSET            0x3131
#define     COMM_SENDREQUESTCONFIG      0x3132      //wy040600 WY113099
#define     COMM_SENDPANEL              0x3200
#define     COMM_SENDPANELBREAK         0x32FF
#define     COMM_SENDCONTROL            0x3330
#define     COMM_SENDCONTROLINDEX       0x3331
#define     COMM_SENDCONTROLMACHINE     0x3332
#define     COMM_SENDCONTROLMOLDSET     0x3333
#define     COMM_SENDCONTROLMONITOR     0x3334
#define     COMM_SENDPOWERMATCH         0x3335      // St1108'01 for D/A 
#define     COMM_SENDCOMMAND            0x3336      // St0502'03  
#define     COMM_SENDCONTROLMACHINEB    0x3337
#define     COMM_SENDCONTROLMOLDSETB    0x3338
#define     COMM_SENDCONTROLMONITORB    0x3339
#define     COMM_SENDDIAGRM             0x3430
#define     COMM_SENDDIAGWM             0x3431
#define     COMM_SENDDIAGSM             0x3432
#define     COMM_SENDDIAGRI             0x3433
#define     COMM_SENDDIAGWO             0x3434
#define     COMM_SENDDIAGSI             0x3435
#define     COMM_SENDASSIGNINPUT        0x3436
#define     COMM_SENDASSIGNOUTPUT       0x3437
#define     COMM_SENDSTATUS             0x3530
#define     COMM_SENDR_PROFILE01        0x3630      // Request of profile 01
#define     COMM_SENDS_PROFILE01        0x3631      // Stop    of profile 01
#define     COMM_SENDR_BPROFILE01       0x3632      // B Request of profile 01
#define     COMM_SENDS_BPROFILE01       0x3633      // B Stop    of profile 01

#define     COMM_GETCOLDSTART           0x4141
#define     COMM_GETWARMSTART           0x4142
#define     COMM_GETCONFIG              0x4241
#define     COMM_GETDAOUTPUT            0x4242      // St1108'01 for D/A
#define     COMM_GETOPSTATE             0x4341
#define     COMM_GETOPSTATUS            0x4342
#define     COMM_GETOPMETER             0x4343
#define     COMM_GETOPMETERZEROAGAIN    0x4841      //"HA" ZERO SETTING AGAIN WY042099
#define     COMM_GETOPTEMPERATURE       0x4344
//#ifdef    D_FAST
#define     COMM_GETOPSUMMARY           0x4345      // St020101 for update quickly
//#endif
//#ifdef   D_54CPU
#define     COMM_GETCURVEDATA           0x4346      // WJ0610'03 add for command =>54
//#else
// #if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
 #define    COMM_GETCOMMAND             0x4346      // WJ0610'03 add for command =>52
// #endif
//#endif

#define     COMM_GETCONTROLINDEX        0x4441
#define     COMM_GETPRODUCTION          0x4541
#define     COMM_GETMONICURRENT         0x4542
#define     COMM_GETMONILAST            0x4543
//efine     COMM_GETMONIMAXMIN          0x4544
#define     COMM_GETMONICURRENTB        0x4545
#define     COMM_GETMONILASTB           0x4546
#define     COMM_GETMONIMAXMINB         0x4547
#define     COMM_GETDIAGRM              0x4641
#define     COMM_GETDIAGRI              0x4642
#define     COMM_GETS_PROFILE01_I       0x4741      // Start of profile 01 -- Inject
#define     COMM_GETD_PROFILE01_I       0x4742      // Data  of profile 01 -- Inject
#define     COMM_GETE_PROFILE01_I       0x4743      // End   of profile 01 -- Inject
#define     COMM_GETS_PROFILE01_H       0x4744      // Start of profile 01 -- Hold
#define     COMM_GETD_PROFILE01_H       0x4745      // Data  of profile 01 -- Hold
#define     COMM_GETE_PROFILE01_H       0x4746      // End   of profile 01 -- Hold

#define     COMM_GETS_BPROFILE01_I      0x4747      // Start of profile 01 -- Inject
#define     COMM_GETD_BPROFILE01_I      0x4748      // Data  of profile 01 -- Inject
#define     COMM_GETE_BPROFILE01_I      0x4749      // End   of profile 01 -- Inject
#define     COMM_GETS_BPROFILE01_H      0x474A      // Start of profile 01 -- Hold
#define     COMM_GETD_BPROFILE01_H      0x474B      // Data  of profile 01 -- Hold
#define     COMM_GETE_BPROFILE01_H      0x474C      // End   of profile 01 -- Hold

#define     COMM_S_REACHSHOTCOUNT       0x0001
#define     COMM_S_REACHPACKCOUNT       0x0002
#define     COMM_S_POSNSETERROR         0x0004      //Check of Posi_end    Anny052000
#define     COMM_S_COM2MOLDTEMPERROR    0x0008      //Check 模溫偏差

/*
#define     COMM_S_SUCCESS              0x0000
#define     COMM_S_FAILURE              0x0001

#define     COMM_SUCCESS                0x0000
#define     COMM_E_FAILURE              0x0010
*/
#define     COMM_ECHO_SUCCESS           0x0000
#define     COMM_ECHO_FAILURE           0x0001

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

//#if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
#define     MAX_COMMAND_KEEP            100      // St0527'03 for Command send
enum CMDSTATE {
            CMD_NONE,
            CMD_SEND,
            CMD_ECHO
            };
//#endif
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
#ifndef   D_TEST
typedef struct tagTLED
    {
    long    lCount;
    int     nIndex;
    } TLED;
#endif
typedef struct tagCONTROLINDEX
    {
    WORD    wIndex;
    WORD    wValue;
    } CONTROLINDEX;
//#if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
typedef struct tagCMDDATA
    {
    WORD        wCmd;
    WORD        wIndex;
    WORD        wValue;
    WORD        wEcho;
    } CMDDATA;

typedef struct tagCMDQUEUE
    {
    WORD    wInput;
    WORD    wOutput;
    WORD    wQueueEnd;
    BOOL    bQueueFull;
    WORD    CmdQueue  [MAX_COMMAND_KEEP];    //Steven 2003/12/12 add
    WORD    IndexQueue[MAX_COMMAND_KEEP];
    WORD    ValueQueue[MAX_COMMAND_KEEP];
    } CMDQUEUE;
//#endif
typedef struct tagMONIMAXMIN
    {
    WORD    flAutoAlarmStatus;
    DB_MONI moniMax;
    DB_MONI moniMin;
#ifndef   D_54CPU
 #ifdef  D_DC
    DB_MONIB moniMaxB;
    DB_MONIB moniMinB;
 #endif
#endif
    } MONIMAXMIN;
#ifdef   D_54CPU
 #ifdef    D_DC    //Chuntzu 2004/7/30 add
typedef struct tagMONIMAXMINB
    {
    DB_MONIB moniMaxB;
    DB_MONIB moniMinB;
    } MONIMAXMINB;
 #endif
#endif

typedef struct tagDIAGRM
    {
    WORD    wAddressROM;
    WORD    wAddressRAM;
    } DIAGRM;

typedef struct tagDIAGWM
    {
    WORD    wAddress;
    WORD    wData;
    } DIAGWM;

typedef struct tagDIAGRI
    {
    WORD    wAddress;
    } DIAGRI;

typedef struct tagDIAGWO
    {
    WORD    wAddress;
    WORD    wData;
    } DIAGWO;

typedef struct tagASSIGNIO
    {
    WORD    wOldAddress;
    WORD    wNewAddress;
    } ASSIGNIO;

#ifdef   D_54CPU
typedef struct tagDB_OPSUM                     //St020101
    {
    WORD        wOperationMode;                     // 510 操作狀態
    WORD        wOperationStep[5];      // 511 動作狀態1
    WORD        wOperationSubStep[MAX_OP_SUBSTEP];  // 514 動作級別狀態1
    _TIMEL      tmlCycle;                            // 600 循環計時實際值
    _TIMEL      tmlActing[2];                       // 525 動作時間1
    _DA         daOutput[4];                        // 540 DA輸出CH1
    _AD         adPosi_Relative[4];                 // 550 位置尺相對位置CH1
    } DB_OPSUM;
#else
typedef struct tagDB_OPSUM                     //St020101
    {
    WORD        wOperationMode;                     // 510 操作狀態
    WORD        wOperationStep[MAX_OP_STEP+1];      // 511 動作狀態1
    WORD        wOperationSubStep[MAX_OP_SUBSTEP];  // 514 動作級別狀態1
    _TIMER      tmActing[4];                        // 525 動作時間1
    _TIMER      tmCycle;                            // 600 循環計時實際值
    _DA         daOutput[4];                        // 540 DA輸出CH1
    _AD         adPosi_Relative[4];                 // 550 位置尺相對位置CH1
    } DB_OPSUM;
#endif
typedef union tagCOMMRX
    {
    BYTE            acBuffer[MAX_TOKEN_SIZE]; //2048
    DB_CONFIG       config;
    CONTROLINDEX    controlindex;
    DB_STATE        state;
    DB_STATUS       status;
    DB_METER        meter;
    DB_TEMP         temp;
    DB_OPSUM        OPSummary;                //St020101
    DB_DAOUT        DAoutput;               // St1108'01  for D/A
    WORD            wShotCount;
    DB_MONI         moniCurrent;
    DB_MONI         moniLast;
    DB_MONIB        moniCurrentB;
    DB_MONIB        moniLastB;
    MONIMAXMINB     monimaxminB;
    DB_CURVEDATA    curvedata;

    DB_CHARTSTROKE0 chartstroke0;
    DB_DIAGRM       diagrm;
    DB_DIAGRI       diagri;
    DB_COMMAND      command;
    WORD            wResult;
    } COMMRX;

typedef union tagCOMMTX
    {
    BYTE            acBuffer[MAX_TOKEN_SIZE];
    DB_CONFIG       config;
    DB_MACHINE      machine;
    DB_MOLDSET      moldset;
    DB_MACHINEB     machineB;
    DB_MOLDSETB     moldsetB;
    MONIMAXMINB     monimaxminB;

    MONIMAXMIN      monimaxmin;
    ZSET            zeroset;
    CONTROLINDEX    controlindex;
    CMDDATA         cmddata;                // St0522'03
    DB_POWERMATCH   powermatch;             // St1108'01  for D/A
    DIAGRM          diagrm;
    DIAGWM          diagwm;
    DIAGRI          diagri;
    DIAGWO          diagwo;
    ASSIGNIO        assignio;
    WORD            wStatus;

    WORD            wResult;
    } COMMTX;

typedef struct tagCOMMFRAMERX
    {
    WORD        wCmd;
    COMMRX      commdata;
    } COMMFRAMERX;

typedef struct tagCOMMFRAMETX
    {
    WORD        wCmd;
    COMMTX      commdata;
    } COMMFRAMETX;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
//atic void OnOffLED_Motor();

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class CTaskComm : public CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
#ifdef   D_2COM
    CTaskComm(WORD wChannel);
#else
    CTaskComm();
#endif
    ~CTaskComm();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/

private:
//  WORD    m_wChannelMap;
    CRS232  m_rs232;
    CWnd*   m_pwndStatus;
//#if    defined(D_NEWV)||(D_CMD)           //Weichun0903,03
//    CWnd*   m_pwndBaudrate;
//#endif
    BYTE    m_cState;
    BYTE    m_cStateRx;
    BYTE    m_cStateTx;

    long    m_lCounter;
    long    m_lCounterW0;
    long    m_lCounterW1;
    long    m_lCounterBW1;
    long    m_lCounterReqConfig;        //wy040600 wy113099
    BOOL    bGotConfig;                 //wy040600 wy113099
//    BOOL    m_bSendOilVat;                 //wy040600 wy113099
    BOOL    m_bGotDA;                  //St1114'01 for D/A
    short   m_nReqConfigTimes;         //St041100
//  BOOL    m_bInjectStart;
//  BOOL    m_bHoldStart;
    
//  BOOL    m_bSendDiagRM;
//  DIAGRM  m_diagrm;
//  BOOL    m_bSendDiagRI;
//  DIAGRI  m_diagri;
    BOOL    m_bSendRProfile01;
    BOOL    m_bSendRBProfile01;

#ifdef      D_NETS
    BOOL    m_bPassThrough;
#endif
#ifndef     D_TEST
            TLED        m_tled;
#endif
//#if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
    WORD     m_wCmdState;
    WORD     m_wCmdSend;
    int      m_nCmdNull;
    CMDDATA  m_CmddData;
//#ifdef   D_NPANL
//    CMDDATA  m_CmdPanelKey;
    CMDDATA  m_CmdPanelBreak;
//#endif
    CMDQUEUE m_CmdQueue;
//    BOOL     m_bSysReset;
    BOOL     m_bMachineSend;
//#endif
    BOOL     m_bSysReset;          //Steven20040202 change to standard
#ifdef    D_2COM             //Chuntzu 2004/8/10 add for 2 com port
    BOOL     m_bCOM2;          //Steven20040202 change to standard
#endif
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Run();
    void    SetStatusWindow(CWnd* pwndStatus);
    
    void    SendConfig()                                        ;
    void    SendZeroSet()                                       ;
    void    SendRequestConfig()                                 ;   //wy040600 wy113099
    void    SendPanel(BYTE cKey)                                ;
    void    SendControl()                                       ;
    void    SendControlIndex(WORD* pwData)                      ;
    void    SendControlIndexO(WORD* pwData)                     ;   //Chuntzu 2004/11/10 add 
//#if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
    void    SendCommand()                                       ;
    BOOL    PushCmd(CMDDATA* pcmd);
 #ifdef   D_MS3110
    void    SendCmdData(CMDDATA cmddata)                        ;               //St0512'03
 #endif
//#endif
    void    SendControlMachine()                                ;
    void    SendControlMoldset()                                ;
    void    SendControlMonitor()                                ;
#ifdef   D_54CPU
 #ifdef   D_DC
    void    SendControlMachineB()                               ;
    void    SendControlMoldsetB()                               ;
    void    SendControlMonitorB()                               ;
 #endif
#endif
    void    SendPowerMatch    ()                                ;   // St1108'01 for D/A
    void    SendDiagRM(DIAGRM diagrm)                           ;
    void    SendDiagWM(DIAGWM diagwm)                           ;
    void    SendDiagSM()                                        ;
    void    SendDiagRI(DIAGRI diagri)                           ;
    void    SendDiagWO(DIAGWO diagwo)                           ;
    void    SendDiagWO(WORD wAddress, WORD wData)               ;
    void    SendDiagSI()                                        ;
    void    SendAssignInput (WORD wOldAddress, WORD wNewAddress);
    void    SendAssignOutput(WORD wOldAddress, WORD wNewAddress);
    void    SendStatus(WORD wStatus)                            ;
    void    SendRProfile01()                                    ;
    void    SendSProfile01()                                    ;
#ifdef   D_54CPU
 #ifdef   D_DC
    void    SendRBProfile01()                                    ;
    void    SendSBProfile01()                                    ;
 #endif
#endif
#ifdef  D_INJCC
    void    SendOilVat();
#endif

#ifdef      D_NETS
    void    UPassThrough(BYTE* pch, int nCount);
#endif
#ifdef      D_TKDP
void        SendTaskNow(WORD wData);
#endif

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
public:
    void    __near GetColdStart();
    void    __near GetWarmStart();
    void    __near GetConfig();
    void    __near GetOPState();
    void    __near GetOPStatus();
    void    __near GetOPMeter();
    void    __near GetOPMeterZeroAgain();   //wy042099 for run-time zero setting
    void    __near GetOPTemperature();
    void    __near GetOPSummary();          //St020101 for update quickly 
    void    __near GetControlIndex();
//#if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
    void    __near GetCommand();
//#endif
    void    __near GetProduction();
    void    __near GetDAOutput();           // St1108'01 for D/A
    void    __near GetMoniCurrent();
    void    __near GetMoniLast();
    void    __near GetMoniMaxMin();
#ifdef   D_54CPU
    void    __near GetCurveData();          //Chuntzu 2005/1/20 add for new curve
 #ifdef   D_DC    //Chuntzu 2004/7/30 add
    void    __near GetMoniCurrentB();
    void    __near GetMoniLastB();
    void    __near GetMoniMaxMinB();
 #endif
#endif

    void    __near GetDiagRM();
    void    __near GetDiagRI();
    void    __near GetSProfile01I();
    void    __near GetDProfile01I();
    void    __near GetEProfile01I();
    void    __near GetSProfile01H();
    void    __near GetDProfile01H();
    void    __near GetEProfile01H();
#ifdef   D_54CPU
 #ifdef   D_DC
    void    __near GetSBProfile01I();
    void    __near GetDBProfile01I();
    void    __near GetEBProfile01I();
    void    __near GetSBProfile01H();
    void    __near GetDBProfile01H();
    void    __near GetEBProfile01H();
 #endif
#endif
    void    __near GetEcho();

private:
    void    __near Echo(WORD wCode);
    void    __near Reset();
#ifdef  D_DC
    void    __near SelTempSendValue();
#endif
#if    defined(D_MLDT)||defined(D_MDT4)||defined(D_MDT6)||defined(D_MDT12)||defined(D_MDT8)||defined(D_6V4MT)||defined(D_3T6MT)||defined(D_T14M4)||defined(D_MDT10)  //Weichun2003/10/23 Gwei 2005/8/18 D_MDT10          
    void    __near SelMoldTempSendValue();
#endif
#if     defined(D_SELVAT)||defined(D_WSMVAT)
    BOOL    __near CheckOilVatID(WORD* pwData);
    void    __near TransferOilVat();
    void    __near SendOilVat();
    void    __near SetVatDefPosi();
#endif
    void    __near LED();
//#if    defined(D_CMD)||(D_JW)           //Chuntzu 2004/10/26 add D_JW
//    BOOL    __near PushCmd(CMDDATA* pcmd);
    BOOL    __near PopCmd (CMDDATA* pcmd);
//#endif
#ifdef  D_INJCC
    BOOL    __near CheckOilVatID(WORD* pwData);
    void    __near TransferOilVat();
    void    __near SetVatDefPosi();
#endif

};

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagCOMMMAP
    {
    WORD    wCmd;
    void    (__near CTaskComm::*npFunc)();
    } COMMMAP;
extern      BOOL        g_bOPSumPass;   //st031701
#ifdef  D_PRO
extern      BOOL        g_bUpInjPro;    //St051401
extern      BOOL        g_bUpHldPro;    //St051401
#endif

#endif

