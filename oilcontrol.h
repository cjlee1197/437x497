/*===========================================================================+
|  Function : Control                                                        |
|  Task     : Machine Control Utilities header file                          |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52                                                     |
|  Link     : MSVC V1.52                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/20/1998                                                     |
|  Revision : 06/20/1998                                                     |
+===========================================================================*/

#ifndef     D_CONTROL
#define     D_CONTROL

#include    "common.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     tmMAX_OP_ERRORSAVE			8      
#define     tmERROR_NULL				(tmMAX_OP_ERRORSAVE * 16)
#define     tmMODE_MANUAL				-1
#define     tmMODE_SEMIAUTO				1
#define     tmMODE_AUTOADJMOLD			11

#define     tmMAX_OP_STEP				5       // 3*16 st092099
#define     tmMAX_OP_SUBSTEP			2       // 2*16
#define     tmMAX_OP_ERRORFLAG			16       // 6*16




/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef struct tagtmDB_BUFFER
    {
    WORD    wOperationMode;
    WORD    wOperationStep;
    WORD    wOperationSubStep;
    char    cIndexMode;                 // Operation mode index
    BYTE    cIndexError;                // Error code index
    BOOL    bSwitchToAuto;              // Change operation mode from manual to auto
    WORD    wSwitchToAutoShutCount;     // Shut count after changing operation mode from manual to auto

    _FLAG   flMotorStatus;
    _FLAG   flHeaterStatus;

    BOOL    bDelayedSpeech;
    long    lDelayedCount;
    BOOL    bRepeatSpeech;
    long    lRepeatCount;
    } tmDB_BUFFER;

typedef struct tagtmDB_EFLAG
    {
    WORD        wErrorFlag[tmMAX_OP_ERRORFLAG];
    WORD        wCoreAlarmFlag;                     // 中子警報組別狀態 //St20031128 add
    } tmDB_EFLAG;
/*===========================================================================+
|           Variabal definition                                              |
+===========================================================================*/

/*===========================================================================+
|           Macro definition                                                 |
+===========================================================================*/
#define     dbbuff              m_dbbuffer
#define     dbeflg              m_dberrorflg


/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
void        Error_Control();
WORD        GetErrorLogCount();
//tmDB_ERROR*	GetErrorLog(WORD wIndex);   // wIndex = 1 is the first (nearest) record
BOOL        AddErrorLog(WORD wID);
BOOL        FixErrorLog(WORD wID);
BOOL        ClearErrorLog();
/*===========================================================================+
|           External                                                         |
+===========================================================================*/
extern      BYTE        HoldcIndex;         ////wy120999 add dor delay speech 100ms
extern		tmDB_BUFFER	m_dbbuffer;
extern		tmDB_EFLAG	m_dberrorflg;
#endif
