/*==========================================================================+
|  Class    : I32 communication          									|
|  Task     : I32 communication header file                     			|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2006/2/21                                                    	|
|  Revision : 1.0                                                           |
+==========================================================================*/
#ifndef		D_DI32COMM
#define		D_DI32COMM

#include	"common.h"
#include	"taskcomm.h"
#include		"oil54struct.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		I32RX_FO		0x00000001			// FO讀
#define		I32RX_TEMP		0x00000002			// 溫度讀
#define		I32RX_STATE		0x00000003			// 机器狀態讀
#define		I32RX_ERROR		0x00000004			// 錯誤讀
#define		I32RX_MONI		0x00000005			// 品質讀
#define		I32RX_MAXID		0x00000008			// 最大資料讀ID
       		
#define		I32TX_FO		0x00000001			// FO寫
#define		I32TX_TEMP		0x00000002			// 溫度寫
#define		I32TX_REAL		0x00000003			// 實時資料寫
#define		I32TX_STATE		0x00000004			// 机器狀態讀
#define		I32TX_SET		0x00000005			// 設定資料寫
#define		I32TX_MONI		0x00000006			// 品質上下限
#define		I32TX_ERROR		0x00000007			// 錯誤信息
#define		I32TX_CORE		0x00000008			// core data
#define		I32TX_MAXID		0x00000008			// 最大資料寫ID

#define		TEMP_COUNT		21

#define		SETDATA_COUNT	sizeof(I32_SETDATA)/sizeof(DWORD)
#define		SETCORE_COUNT	sizeof(I32_COREDATA)/sizeof(double)
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void *(* PRSRUN)(void *);

typedef	struct tagFOLIST
{
	union
	{
		long	lState[SIMD_FO_COUNT];
		long	lCmd[SIMD_FO_COUNT];
	};
} I32_FOLIST;

/*typedef struct tagI32_MONI
{  
    DWORD      tmCycleTime;                     //  0 循環計時實際值
    DWORD      tmClampClose;                   	//  1 關模計時實際值
    DWORD      tmClampOpen;                    	//  2 開模計時實際值
    DWORD      tmInject;                       	//  4 射出計時實際值
    DWORD      tmCharge;                       	//  5 儲料計時實際值
    DWORD      tmSuckBack;                     	//  6 射退計時實際值
    DWORD      tmEject;                        	//  7 脫模計時實際值
    double     adPosi_ClampOpenEnd;          	//  8 開模位置實際值
    double     daPres_TurnToHold;               //  9 保壓轉換壓力實際值
    double     tmTurnToHold;                    // 10 保壓轉換時間實際值
    double     adPosi_TurnToHold;               // 11 保壓轉換位置實際值
    double     adPosi_InjectStart;          	// 12 射出起點位置實際值
    double     adPosi_InjectHoldEnd;            // 13 射出終點位置實際值
} I32_MONI;

typedef union tagMONITOR
{
	struct
   	{
   		int	nCount;
		int nIndex;
   	};
   	struct
   	{
		DWORD		dwShotCount;				// 開模序號
		I32_MONI	Moni;
	};
}	MONITOR;
*/
typedef	struct tagREALDATA
{
	double		dMoldPos;						// 開關模位置
	double		dEjectPos;						// 脫模位置		
	double		dInjectpos;						// 射出位置
	double		dInjUnitpos;					// 座台位置
	double		dRecoveryPos;					// 儲料位置
	double		dClampForceSensor;				// 鎖模壓力實際值 from sensor
	double		dClampForceTorque;				// 鎖模壓力實際值 from torque
	double		dPresTurnHold;					// 保壓轉換壓力實際值
	double		dTimeTurnHold;					// 保壓轉換時間實際值
	double		dPosiTurnHold;					// 保壓轉換位置實際值
}	I32_REALDATA;

typedef struct tagCOREDATA
{
	double		dwEject_TravelPosition;  		// 中途托模動作位置			//yang 2006/10/13 									
	double		dCoreA_FWDPosition;				// 中子A進動作位置								
	double		dCoreA_BWDPosition;				// 中子A退動作位置								
	double		dCoreB_FWDPosition;				// 中子B進動作位置									
	double		dCoreB_BWDPosition;				// 中子B退動作位置									
	double		dCoreC_FWDPosition;				// 中子C進動作位置									
	double		dCoreC_BWDPosition;				// 中子C退動作位置								
	double		dCoreD_FWDPosition;				// 中子D進動作位置								
	double		dCoreD_BWDPosition;				// 中子D退動作位置								
	double		dfBlast1Position;				// 吹氣1位置 									
	double		dfBlast2Position;				// 吹氣2位置									
	double		dfBlast3Position;				// 吹氣3位置									
	double		dfBlast4Position;				// 吹氣4位置
	double		dEjectWorkingArea;				// 脫模有效區					
	double		dMoldOpenEndPostion;			// 開模終設定位置	
	double		dCoreWorkingArea;				// 中子有效區		
} I32_COREDATA;

/*
typedef	struct tagREALDATA2
{
	short		nMoldState;						// 開關模PosiState
	short		nEjectorState;					// 脫模PosiState
	short		nInjectionState;				// 射出PosiState
	short		nInjUnitState;					// 座台PosiState
	short		nRecoveryState;					// 儲料PosiState
	short		nDecompression;					// 鬆退	PosiState
}	I32_REALDATA2;
*/
typedef struct tagI32_SETTEMP
{
	int		nIndex;
	short   nSetting;                   		// setting value
	short   nRamp;                      		// ramp area
//	short   nMax;                       		// max up limit
//	short   nMin;                       		// max dn limit
	short   nMaxOffset;               			// 溫度容許上限offset //<<WY2006/10/04 change to relative value, not absolute
    short   nMinOffset;               			// 溫度容許下限offset //<<WY2006/10/04 change to relative value, not absolute
	WORD 	wKP;                        		// pid KP
	WORD 	wKI;                        		// pid KI
	WORD	wKD;                        		// pid KD
	WORD	wDT;                        		// Sample Rate 
	WORD	wTD;			            		// time delay of system
	short   nAccSet;                    		// Acculumate Setting
} I32_SETTEMP;

typedef	struct tagI32_SETDATA
{
	// Lubration                                                            
	DWORD		dwAutoLubraShotCount1;			// 自動潤滑1模數			
	DWORD		dwtAutoLubrationMonitorTime1;  	// 自動潤滑1監測時間		
	DWORD		dwAutoLubrationCounter1;		// 自動潤滑1計數			
	DWORD		dwtAutoLub1Delay;				// 自動潤滑1延遲			
	DWORD		dwAutoLubShotCount2;			// 自動潤滑2模數			
	DWORD		dwtAutoLubrationMonitorTime2;  	// 自動潤滑2監測時間		
	DWORD		dwAutoLubrationCounter2;		// 自動潤滑2計數			
	DWORD		dwtAutoLub2Delay;				// 自動潤滑2延遲			
	// Mold Adjust                                                      	
	DWORD		dwRailingTon;					// 每柵單位頓數				
	DWORD		dwRailingDistance;				// 每柵單位距離				
	DWORD		dwRailingTimer;					// 每柵單位時間				
	DWORD		dwAdjustCounter;				// 調模計數
	
	DWORD       dwAdjMoldWidth;                 // 模板厚度 		//<<yang 2006/11/1 for I32
	DWORD       dwAdjObjectWidth;               // 模具厚度 
	DWORD       dwfAdjMode;                     // 調模方式 
					
	DWORD		dwClampForceSet;				// 鎖模力設定值         	
	// Production                                                       	
	DWORD		dwShotCountSetting;				// 設定開模總數		    	
	DWORD		dwtCycleDelay;					// 再循環延遲           	
	// Mold	                                                            	
	DWORD		dwfMoldCloseOption;				// 關模功能選擇				
	DWORD		dwfMoldOpenOption;				// 開模功能選擇				
	// Ejector                                                          	
//	DWORD		dwPosi_EjectTravel;  			// 中途托模位置         	
	DWORD		dwEjectorCounter;				// 脫模次數					
	DWORD		dwfEjectorOption; 				// 脫模功能選擇				
	// Charge                                                           	
	DWORD		dwtCOOLER;						// 冷卻時間	            	
	// Injection Unit                                                   	
	DWORD		dwtInjUnitBackward;				// 座退時間					
	DWORD		dwfInjUnitBackwardOption;		// 座退功能選擇				
	DWORD       dwtInjUnitBreakDelay;			// 座進剎車延遲時間     	
	DWORD       dwtInjUnitFwdOffDelay;			// 座進終延遲時間       	
	DWORD       dwtInjUnitReleaseBreakDelay;	// 座退放剎車延遲時間   
	DWORD		dwtInjUnitReleaseOnDelay;		// 座退終延遲時間   
	// Suck Back                                                        	
	DWORD		dwfSuckbackOption;				// 射退退功能選擇			
	// CoreA                                                            	
	DWORD		dwfCoreAOption;					// 中子A選用				
	DWORD		dsfCoreAMode;					// 中子A控制模式			
	DWORD		dwCoreA_FWDOption;				// 中子A使用模式			
	DWORD		dwtCoreA_FWDDelay;				// 中子A進動作延遲	    	
	DWORD		dwtCoreA_FWDActionTime;			// 中子A進動作時間	    	
//	DWORD		dwCoreA_FWDPosition;			// 中子A進動作位置      	
	DWORD		dwCoreA_InCouter;				// 絞牙A進動作計數	    	
	DWORD		dwCoreA_BWDOption;				// 中子A使用模式			
	DWORD		dwtCoreA_BWDDelay;				// 中子A退動作延遲	    	
	DWORD		dwtCoreA_BWDActionTime;			// 中子A退動作時間	    	
//	DWORD		dwCoreA_BWDPosition;			// 中子A退動作位置      	
	DWORD		dwCoreA_OutCouter;				// 絞牙A退動作計數	    	
	// CoreB                                                            	
	DWORD		dwfCoreBOption;					// 中子B選用				
	DWORD		dsfCoreBMode;					// 中子B控制模式			
	DWORD		dwCoreB_FWDOption;				// 中子B使用模式			
	DWORD		dwtCoreB_FWDDelay;				// 中子B進動作延遲	    	
	DWORD		dwtCoreB_FWDActionTime;			// 中子B進動作時間	    	
//	DWORD		dwCoreB_FWDPosition;			// 中子B進動作位置      	
	DWORD		dwCoreB_InCouter;				// 絞牙B進動作計數
	DWORD		dwCoreB_BWDOption;				// 中子B使用模式			
	DWORD		dwtCoreB_BWDDelay;				// 中子B退動作延遲	    	
	DWORD		dwtCoreB_BWDActionTime;			// 中子B退動作時間	    	
//	DWORD		dwCoreB_BWDPosition;			// 中子B退動作位置      	
	DWORD		dwCoreB_OutCouter;				// 絞牙B退動作計數
	// CoreC                                                            	
	DWORD		dwfCoreCOption;					// 中子C選用				
	DWORD		dsfCoreCMode;					// 中子C控制模式			
	DWORD		dwCoreC_FWDOption;				// 中子C使用模式			
	DWORD		dwtCoreC_FWDDelay;				// 中子C進動作延遲	    	
	DWORD		dwtCoreC_FWDActionTime;			// 中子C進動作時間	    	
//	DWORD		dwCoreC_FWDPosition;			// 中子C進動作位置      	
	DWORD		dwCoreC_InCouter;				// 絞牙C進動作計數	    	
	DWORD		dwCoreC_BWDOption;				// 中子C使用模式			
	DWORD		dwtCoreC_BWDDelay;				// 中子C退動作延遲	    	
	DWORD		dwtCoreC_BWDActionTime;			// 中子C退動作時間	    	
//	DWORD		dwCoreC_BWDPosition;			// 中子C退動作位置      	
	DWORD		dwCoreC_OutCouter;				// 絞牙C退動作計數	    	
	// CoreD                                                            	
	DWORD		dwfCoreDOption;					// 中子D選用				
	DWORD		dsfCoreDMode;					// 中子D控制模式			
	DWORD		dwCoreD_FWDOption;				// 中子D使用模式			
	DWORD		dwtCoreD_FWDDelay;				// 中子D進動作延遲	    	
	DWORD		dwtCoreD_FWDActionTime;			// 中子D進動作時間	    	
//	DWORD		dwCoreD_FWDPosition;			// 中子D進動作位置      	
	DWORD		dwCoreD_InCouter;				// 絞牙D進動作計數	    	
	DWORD		dwCoreD_BWDOption;				// 中子D使用模式			
	DWORD		dwtCoreD_BWDDelay;				// 中子D退動作延遲	    	
	DWORD		dwtCoreD_BWDActionTime;			// 中子D退動作時間	    	
//	DWORD		dwCoreD_BWDPosition;			// 中子D退動作位置      	
	DWORD		dwCoreD_OutCouter;				// 絞牙D退動作計數	    	
	// Blast1                                                           	
	DWORD		dwtBlast1ActionTime;			// 吹氣1時間				
	DWORD		dwtBlast1ActionDelay;			// 吹氣1延遲		    	
//	DWORD		dwfBlast1Position;				// 吹氣1位置				
	// Blast2                                                           	
	DWORD		dwtBlast2ActionTime;			// 吹氣2時間				
	DWORD		dwtBlast2ActionDelay;			// 吹氣2延遲		    	
//	DWORD		dwfBlast2Position;				// 吹氣2位置				
	// Blast3                                                           	
	DWORD		dwtBlast3ActionTime;			// 吹氣3時間				
	DWORD		dwtBlast3ActionDelay;			// 吹氣3延遲		    	
//	DWORD		dwfBlast3Position;				// 吹氣3位置				
	// Blast4                                                           	
	DWORD		dwtBlast4ActionTime;			// 吹氣4時間				
	DWORD		dwtBlast4ActionDelay;			// 吹氣4延遲				
//	DWORD		dwfBlast4Position;				// 吹氣4位置				
	// Auto Purge
	DWORD		dwAutoPurgeCouter;				// 清料次數
	// Monitor Alarm Status                                             	
	DWORD		dwAutoAlarmStatus;      		// 自動警報狀態
	// Defaut monitor value
    DWORD		dwtCycleDefaultMax;				// 全程計時警報上限內設值   
    DWORD		dwtChargeDefaultMax;         	// 儲料計時警報上限內設值   
    DWORD		dwtSuckBackDefaultMax;       	// 射退計時警報上限內設值   
    DWORD		dwtNozzleDefaultMax;         	// 座台計時警報上限內設值   
    DWORD		dwtEjectDefaultMax;          	// 脫模計時警報上限內設值   
    //yang 2006/10/18 
    DWORD		dwf24HourHeartor;				// 24小時定時加溫開		
    DWORD		dwfZero;						// 總數到				0x0001開模總數歸零 0x0002裝模總數歸零 0x0004自動潤滑
    DWORD		dwfPhotoSensor;					// 電眼自動使用				
    DWORD		dwfPowerDoor;					// 自動安全門使用			
    DWORD		dwfRobot;						// 機械手使用	
    DWORD		dwClampUpSelect;				// 0 = Sensor 1 = Torque
}	I32_SETDATA;

typedef struct tagSTATE                 		// 機器狀態
{                                       		
    DWORD       wOperationMode;         		// 操作狀態     
    DWORD       wOperationStep;         		// 動作狀態     
    DWORD       wOperationStep2;        		// 動作狀態2  
    DWORD       flMotorStatus;          		// 馬達狀態    1=ON  0=OFF 
    DWORD       flHeaterStatus;         		// 電熱狀態    1=ON  0=OFF 
    DWORD      	tmActing;            			// 動作時間
	DWORD		tCycleTime;						// 循環時間
	DWORD		tCurrentShotCount;				// 開模總數
	DWORD       dwHMIAdjCounter;        		// 畫面顯示調模計數值		//<<yang 2006/11/1 for I32
	DWORD		dwAutoLub1ShotCounter;			// 自動潤滑1模數		//Alan2006/10/8>>
	DWORD		dwAutoLub1Counter;      		// 自動潤滑1計數
	DWORD		dwAutoLub2ShotCounter;			// 自動潤滑2模數
	DWORD		dwAutoLub2Counter;				// 自動潤滑2計數		//Alan2006/10/8>>
	WORD		wAlarmStatus;					// 警報狀態
	WORD 		wInOperation;					// MotionKeyInUse	// 0 =no use 1= use
	WORD		awInPortValue[8];				// Input
	WORD		awOutPortValue[8];				// Output	
	DWORD		dwEthernetState;				//<<yang 2006/10/22 for I32 
	DWORD		dwTestPanel;
} I32_STATE;

typedef	struct tagMONILIMIT
{
	I32_MONI	MoniMin;
	I32_MONI	MoniMax;
} I32_MONILIMIT;


typedef struct tagI32_REALTEMP
{
	int		nIndex;
	WORD    wStatus;                    		// heater status
	short   nCurrent;                   		// current value
	short   nAccCurrent;                		// Acculumate current value
	short   nAccLast;			        		// Acculumate last value
	short	nSum;                       		// pid SUM
	short	nDE;                        		// pid DE
	short	nDDE;                       		// pid DDE
	WORD	wKPOut;                     		// pid KPOut                  
	WORD	wKIOut;                     		// pid KIOut
	WORD	wKDOut;                     		// pid KDOut
	short	nMaxUpDDE;                  		// pid max up DDE 
	short	nMaxDnDDE;                  		// pid MAX down dde
}	I32_REALTEMP;

typedef	struct tagTXDATA
{
	int			nType;							// 資料類型
	union
	{
		I32_REALDATA	RealData;				// 實時資料
		I32_STATE		State;
		I32_FOLIST		FOStateList;
		I32_SETTEMP		SetTemp;
		I32_SETDATA		SetData;				// 設定資料
		I32_MONILIMIT	MoniLimit;
		I32_COREDATA	CoreData;				//yang 2006/10/13 
	};	
} I32_TXDATA;

typedef struct tagRXDATA
{
	int			nType;							// 資料類型
	union
	{
		I32_FOLIST		FOCmdList;
		I32_REALTEMP	RealTemp;
		WORD			wErrorID;
		I32_MONI		Monitor;
		I32_STATE		State;
	};
} I32_RXDATA;

typedef struct tagSETTEMPID
{
	int		nSetting;                   		// setting value
	int		nRamp;                      		// ramp area
	int		nMax;                       		// max up limit
	int		nMin;                       		// max dn limit
	int		nKP;                        		// pid KP
	int		nKI;                        		// pid KI
	int		nKD;                        		// pid KD
	int		nDT;                        		// Sample Rate 
	int		nTD;			            		// time delay of system
	int		nAccSet;                    		// Acculumate Setting
} SETTEMPID;

typedef struct tagREALTEMPID
{
	int 	nStatus;                    		// heater status
	int 	nCurrent;                   		// current value
	int 	nAccCurrent;                		// Acculumate current value
	int 	nAccLast;			        		// Acculumate last value
	int 	nSum;                       		// pid SUM
	int 	nDE;                        		// pid DE
	int 	nDDE;                       		// pid DDE
	int 	nKPOut;                     		// pid KPOut                  
	int 	nKIOut;                     		// pid KIOut
	int 	nKDOut;                     		// pid KDOut
	int 	nMaxUpDDE;                  		// pid max up DDE 
	int 	nMaxDnDDE;                  		// pid MAX down dde
} REALTEMPID;

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class  CTaskI32: public CTaskComm
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskI32();
    virtual ~CTaskI32();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
	static int		m_nID;
	static int		m_nTempCount[TEMP_COUNT];
	static int		m_nTempIndex[TEMP_COUNT];
	static int		m_nSaveMonitor;				// 品質資料保存標誌
	static int		m_nStartAlarm;				// 開始進入自動警報統計
	static int		m_nSetDataIDList[SETDATA_COUNT];
	static int		m_nSetCoreIDList[SETCORE_COUNT];
	
	static SETTEMPID	m_SetTempID[TEMP_COUNT];
	static REALTEMPID	m_RealTempID[TEMP_COUNT];
	
	static pthread_t	m_I32ThreadID;
//	static MONITOR		m_Monitor;					// 當前品質資料位置
	static I32_REALTEMP	m_RealTemp[TEMP_COUNT][500];
	static I32_SETDATA	m_SetData;
	static I32_COREDATA	m_CoreData;
	long				m_lUpdateCounter;
	static WORD			m_wTestPanel;					//<<yang 2006/10/31 for test panel_key
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	void    	Run();
	int			GetStatus();
	static int	Write(int nType, int nLength, void *pData);
	static int	WriteValue(char *pszID);
	void		TestPanel(WORD wValue);						//<<yang 2006/10/31 for test panel
	static		I32_MONILIMIT GetMoniLimit();				//<<yang 2006/11/1 for monitor limit
//	int		GetMoniCount() {return m_Monitor.nCount;};
//	int		GetMoniIndex() {return m_Monitor.nIndex;};
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	static int		ReadFO(I32_FOLIST *pList, int nLength);
	static int		WriteFO(I32_FOLIST *pList);
	static int		ReadTempID();
	static int		ReadTemp(I32_REALTEMP *pTemp);
	static int		WriteTemp(I32_SETTEMP *pTemp);
	static int		WriteReal(I32_REALDATA *pReal);
//	static int		WriteReal2(I32_REALDATA2 *pReal);
	static int		WriteState();
	static int		WriteSet(I32_SETDATA *pSet);
	static int		WriteMoniLimit();
	static int		WriteError();
	static int		ReadError(WORD* pwErrorID);
	static int		ReadMoni(I32_MONI *pMoni);
	static int		SetCurve(WORD nStepIndex);
	static int		ReadState(I32_STATE *pState);
	static int		WriteCore();
	static int		Read(char *pData, int nLength);
	static int		SetTemp();
	static int		WriteCore(char* pszID);
};

#endif
