#ifndef		D__TMTASKMONI_V01
#define		D__TMTASKMONI_V01
#include	"../../taskmoni.h"


class  CtmTaskMoni: public CTaskMoni
{
	DECLARE_DYNCREATE(CtmTaskMoni)
	public:
		CtmTaskMoni();
		~CtmTaskMoni();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();

		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
		void    ActivateScreenSaver(BOOL bActivate);
		void		SaveEnergyData(); //JOYCE2010-7-22
		void		SaveShotCount();//JOYCE2010-10-12 

	private:
		void	ProcessAutoRun();
		void	ProcessAutoRunTime();
		void	ProcessMotorRun();
		void	ProcessStopMachine();
		void	ProcessSaveOperation();
		void	ProcessEnergydata(); //JOYCE2010-7-22 
		void	ProcessNotclmpTime();/*wangli2015-12-10 17:29:18*/
		void	SaveLubricate();  //JOYCE2011-8-23 lubricate
		void	SaveAutoRunTime();
		void	SaveNotclmpTime();/*wangli2015-12-10 17:04:14*/
		void	CheckPowerNub();//JOYCE2011-5-19 
		void	ProcessCoolFan();  //電機冷卻風扇計時
		long long  CheckMachineDate(long long  year,long long month,long long day,
								    long long  hore,long long min,  long long sec);
		void 	ShowStopMachinePassword();
		void 	ShowStopMachineAlarm();
		void 	SetDsp54Manual();
		void  	TestBatPower(); 
		void  	ReadLastRTCTime();   
		void	TestInstBatPower();
		void	 		WndProc(int message, WPARAM wParam, LPARAM lParam); //Andy 20100622
		
		void	ShowOperLed();
		void	ShowModeLed(int nLedIndex);
		void    ShowActStepLed();

	private:
		static		BOOL		m_bCaton;
		long	m_lCounterLoginIn;
		long 	m_lCounterUpdate;
   		long	m_lCounterNotclmpTime;			/*wangli2015-12-10 18:02:08*/

		BOOL    m_bIdle;

	    long    m_lCounter;                 // Counter for sleeping
	    long    m_lCounterRun;              // Counter for time out
	    long	m_lHeatCounter;
	    CScreen*    m_pwndScreen;
	    //BOOL        m_bBypassOld;
	   
	    long	m_lCounterRunAuto;			//LEO20070404
	    long	m_lCounterRunAutoTime;
	    long	m_lCounterRunMotor;			//LEO20070404
	    long	m_lCounterAlarm;			//LEO20070404
	    long	m_lCounterSaveOpt;			//LEO20070404
	    
	    //long	m_lCounterOperate;    // 2010-6-2
	    //long	m_lCounterSemiAuto;		// 2010-6-2
	    //long	m_lCounterTimeAuto;		// 2010-6-2
	    //long	m_lCounterWritedata;
		long	m_lCounterEnergydata;//JOYCE2010-7-22 
		long	m_lCounterCoolFan;   //JOYCE2011-5-28 
		//long  	m_lCounterMaintain;    
		//long  	u_lRecoverTime;

		int		m_dwShotCountCurrent;
		
};

extern	void		LCDPowerOn_3354(int nCount);



#ifndef	D_BEOBJECT_CTMTASKMONI	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

