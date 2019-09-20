/*===========================================================================+
|  Class    : TaskMoni                                                       |
|  Task     : System Monitor Service Routine header file                     |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 07/01/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKMONI
#define     D_TASKMONI

#include    "task.h"
#include 	"cbitmap.h"
#include	"statbar.h"
#include	"tmpackclass.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     MODE_MANUAL				0x0000		//¤â°Ê
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef	struct tagSCREENBMPINFO
{
	int	nXMax;
	int	nYMax;
	int	nXState;
	int	nYState;
	RECT	rect;
	CtmBitMap*	pBMP;
} SCREENBMPINFO;
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
class       CScreen;

/*===========================================================================+
|           Class declaration - TaskMoni                                     |
+===========================================================================*/
class CTaskMoni : public CTask, public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskMoni);
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskMoni();
    ~CTaskMoni();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
protected:
    BOOL    m_bIdle;

    long    m_lCounter;                 // Counter for sleeping
    long    m_lCounterRun;              // Counter for time out
    long	m_lHeatCounter;
    BOOL    m_bSaver;                   //BW1225'05 for not awake
    CScreen*    m_pwndScreen;
    BOOL        m_bBypassOld;
   
    long	m_lCounterRunAuto;			//LEO20070404
    long	m_lCounterRunAutoTime;
    long	m_lCounterRunMotor;			//LEO20070404
    long	m_lCounterAlarm;			//LEO20070404
    long	m_lCounterSaveOpt;			//LEO20070404
    
    
	long  	m_lCounterRunMotor7HX;
    long  	m_lCounterRunMotor2;
    long	m_lCounterOperate;    // 2010-6-2
    long	m_lCounterSemiAuto;		// 2010-6-2
    long	m_lCounterTimeAuto;		// 2010-6-2
    long	m_lCounterWritedata;
	long	m_lCounterEnergydata;//JOYCE2010-7-22 
	long	m_lCounterCoolFan;   //JOYCE2011-5-28 
	long  	m_lCounterMaintain;    
	long  	u_lRecoverTime;

        
	int		m_dwShotCountCurrent;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    //void    Run();
    virtual	void    ActivateScreenSaver(BOOL bActivate)					{};
    virtual	void    IdleScreenSaver()                 {m_bSaver = FALSE;};
    virtual	BOOL    IsSleep()                         {return m_bSaver;};
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
//private:
//	void	ProcessAutoRun();
//	void	ProcessMotorRun();
//	void	ProcessStopMachine();
//	void	ProcessSaveOperation();
};

/*===========================================================================+
|           Class declaration - Screen                                       |
+===========================================================================*/
class CScreen : public CtmWnd
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CScreen(tmWND* pwnd);
    ~CScreen();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
    tmSIZE    m_size;
    short   m_nX;
    short   m_nY;
	int		m_nBMPCount;
	
	SCREENBMPINFO	m_BmpInfo[10];
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
    void    Show();                     // Virtual function
    WORD    OnKey(WORD wKey);           // Virtual function
    
    int		AddBMP(char *pszBMP);
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	
};

extern		CTaskMoni*          g_ptaskMoni;

#endif
