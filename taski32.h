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
#define		I32RX_FO		0x00000001			// FOŪ
#define		I32RX_TEMP		0x00000002			// �ū�Ū
#define		I32RX_STATE		0x00000003			// �󾹪��AŪ
#define		I32RX_ERROR		0x00000004			// ���~Ū
#define		I32RX_MONI		0x00000005			// �~��Ū
#define		I32RX_MAXID		0x00000008			// �̤j���ŪID
       		
#define		I32TX_FO		0x00000001			// FO�g
#define		I32TX_TEMP		0x00000002			// �ū׼g
#define		I32TX_REAL		0x00000003			// ��ɸ�Ƽg
#define		I32TX_STATE		0x00000004			// �󾹪��AŪ
#define		I32TX_SET		0x00000005			// �]�w��Ƽg
#define		I32TX_MONI		0x00000006			// �~��W�U��
#define		I32TX_ERROR		0x00000007			// ���~�H��
#define		I32TX_CORE		0x00000008			// core data
#define		I32TX_MAXID		0x00000008			// �̤j��ƼgID

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
    DWORD      tmCycleTime;                     //  0 �`���p�ɹ�ڭ�
    DWORD      tmClampClose;                   	//  1 ���ҭp�ɹ�ڭ�
    DWORD      tmClampOpen;                    	//  2 �}�ҭp�ɹ�ڭ�
    DWORD      tmInject;                       	//  4 �g�X�p�ɹ�ڭ�
    DWORD      tmCharge;                       	//  5 �x�ƭp�ɹ�ڭ�
    DWORD      tmSuckBack;                     	//  6 �g�h�p�ɹ�ڭ�
    DWORD      tmEject;                        	//  7 ��ҭp�ɹ�ڭ�
    double     adPosi_ClampOpenEnd;          	//  8 �}�Ҧ�m��ڭ�
    double     daPres_TurnToHold;               //  9 �O���ഫ���O��ڭ�
    double     tmTurnToHold;                    // 10 �O���ഫ�ɶ���ڭ�
    double     adPosi_TurnToHold;               // 11 �O���ഫ��m��ڭ�
    double     adPosi_InjectStart;          	// 12 �g�X�_�I��m��ڭ�
    double     adPosi_InjectHoldEnd;            // 13 �g�X���I��m��ڭ�
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
		DWORD		dwShotCount;				// �}�ҧǸ�
		I32_MONI	Moni;
	};
}	MONITOR;
*/
typedef	struct tagREALDATA
{
	double		dMoldPos;						// �}���Ҧ�m
	double		dEjectPos;						// ��Ҧ�m		
	double		dInjectpos;						// �g�X��m
	double		dInjUnitpos;					// �y�x��m
	double		dRecoveryPos;					// �x�Ʀ�m
	double		dClampForceSensor;				// ������O��ڭ� from sensor
	double		dClampForceTorque;				// ������O��ڭ� from torque
	double		dPresTurnHold;					// �O���ഫ���O��ڭ�
	double		dTimeTurnHold;					// �O���ഫ�ɶ���ڭ�
	double		dPosiTurnHold;					// �O���ഫ��m��ڭ�
}	I32_REALDATA;

typedef struct tagCOREDATA
{
	double		dwEject_TravelPosition;  		// ���~���Ұʧ@��m			//yang 2006/10/13 									
	double		dCoreA_FWDPosition;				// ���lA�i�ʧ@��m								
	double		dCoreA_BWDPosition;				// ���lA�h�ʧ@��m								
	double		dCoreB_FWDPosition;				// ���lB�i�ʧ@��m									
	double		dCoreB_BWDPosition;				// ���lB�h�ʧ@��m									
	double		dCoreC_FWDPosition;				// ���lC�i�ʧ@��m									
	double		dCoreC_BWDPosition;				// ���lC�h�ʧ@��m								
	double		dCoreD_FWDPosition;				// ���lD�i�ʧ@��m								
	double		dCoreD_BWDPosition;				// ���lD�h�ʧ@��m								
	double		dfBlast1Position;				// �j��1��m 									
	double		dfBlast2Position;				// �j��2��m									
	double		dfBlast3Position;				// �j��3��m									
	double		dfBlast4Position;				// �j��4��m
	double		dEjectWorkingArea;				// ��Ҧ��İ�					
	double		dMoldOpenEndPostion;			// �}�Ҳ׳]�w��m	
	double		dCoreWorkingArea;				// ���l���İ�		
} I32_COREDATA;

/*
typedef	struct tagREALDATA2
{
	short		nMoldState;						// �}����PosiState
	short		nEjectorState;					// ���PosiState
	short		nInjectionState;				// �g�XPosiState
	short		nInjUnitState;					// �y�xPosiState
	short		nRecoveryState;					// �x��PosiState
	short		nDecompression;					// �P�h	PosiState
}	I32_REALDATA2;
*/
typedef struct tagI32_SETTEMP
{
	int		nIndex;
	short   nSetting;                   		// setting value
	short   nRamp;                      		// ramp area
//	short   nMax;                       		// max up limit
//	short   nMin;                       		// max dn limit
	short   nMaxOffset;               			// �ū׮e�\�W��offset //<<WY2006/10/04 change to relative value, not absolute
    short   nMinOffset;               			// �ū׮e�\�U��offset //<<WY2006/10/04 change to relative value, not absolute
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
	DWORD		dwAutoLubraShotCount1;			// �۰ʼ��1�Ҽ�			
	DWORD		dwtAutoLubrationMonitorTime1;  	// �۰ʼ��1�ʴ��ɶ�		
	DWORD		dwAutoLubrationCounter1;		// �۰ʼ��1�p��			
	DWORD		dwtAutoLub1Delay;				// �۰ʼ��1����			
	DWORD		dwAutoLubShotCount2;			// �۰ʼ��2�Ҽ�			
	DWORD		dwtAutoLubrationMonitorTime2;  	// �۰ʼ��2�ʴ��ɶ�		
	DWORD		dwAutoLubrationCounter2;		// �۰ʼ��2�p��			
	DWORD		dwtAutoLub2Delay;				// �۰ʼ��2����			
	// Mold Adjust                                                      	
	DWORD		dwRailingTon;					// �C�]���y��				
	DWORD		dwRailingDistance;				// �C�]���Z��				
	DWORD		dwRailingTimer;					// �C�]���ɶ�				
	DWORD		dwAdjustCounter;				// �ռҭp��
	
	DWORD       dwAdjMoldWidth;                 // �ҪO�p�� 		//<<yang 2006/11/1 for I32
	DWORD       dwAdjObjectWidth;               // �Ҩ�p�� 
	DWORD       dwfAdjMode;                     // �ռҤ覡 
					
	DWORD		dwClampForceSet;				// ��ҤO�]�w��         	
	// Production                                                       	
	DWORD		dwShotCountSetting;				// �]�w�}���`��		    	
	DWORD		dwtCycleDelay;					// �A�`������           	
	// Mold	                                                            	
	DWORD		dwfMoldCloseOption;				// ���ҥ\����				
	DWORD		dwfMoldOpenOption;				// �}�ҥ\����				
	// Ejector                                                          	
//	DWORD		dwPosi_EjectTravel;  			// ���~���Ҧ�m         	
	DWORD		dwEjectorCounter;				// ��Ҧ���					
	DWORD		dwfEjectorOption; 				// ��ҥ\����				
	// Charge                                                           	
	DWORD		dwtCOOLER;						// �N�o�ɶ�	            	
	// Injection Unit                                                   	
	DWORD		dwtInjUnitBackward;				// �y�h�ɶ�					
	DWORD		dwfInjUnitBackwardOption;		// �y�h�\����				
	DWORD       dwtInjUnitBreakDelay;			// �y�i�b������ɶ�     	
	DWORD       dwtInjUnitFwdOffDelay;			// �y�i�ש���ɶ�       	
	DWORD       dwtInjUnitReleaseBreakDelay;	// �y�h��b������ɶ�   
	DWORD		dwtInjUnitReleaseOnDelay;		// �y�h�ש���ɶ�   
	// Suck Back                                                        	
	DWORD		dwfSuckbackOption;				// �g�h�h�\����			
	// CoreA                                                            	
	DWORD		dwfCoreAOption;					// ���lA���				
	DWORD		dsfCoreAMode;					// ���lA����Ҧ�			
	DWORD		dwCoreA_FWDOption;				// ���lA�ϥμҦ�			
	DWORD		dwtCoreA_FWDDelay;				// ���lA�i�ʧ@����	    	
	DWORD		dwtCoreA_FWDActionTime;			// ���lA�i�ʧ@�ɶ�	    	
//	DWORD		dwCoreA_FWDPosition;			// ���lA�i�ʧ@��m      	
	DWORD		dwCoreA_InCouter;				// ����A�i�ʧ@�p��	    	
	DWORD		dwCoreA_BWDOption;				// ���lA�ϥμҦ�			
	DWORD		dwtCoreA_BWDDelay;				// ���lA�h�ʧ@����	    	
	DWORD		dwtCoreA_BWDActionTime;			// ���lA�h�ʧ@�ɶ�	    	
//	DWORD		dwCoreA_BWDPosition;			// ���lA�h�ʧ@��m      	
	DWORD		dwCoreA_OutCouter;				// ����A�h�ʧ@�p��	    	
	// CoreB                                                            	
	DWORD		dwfCoreBOption;					// ���lB���				
	DWORD		dsfCoreBMode;					// ���lB����Ҧ�			
	DWORD		dwCoreB_FWDOption;				// ���lB�ϥμҦ�			
	DWORD		dwtCoreB_FWDDelay;				// ���lB�i�ʧ@����	    	
	DWORD		dwtCoreB_FWDActionTime;			// ���lB�i�ʧ@�ɶ�	    	
//	DWORD		dwCoreB_FWDPosition;			// ���lB�i�ʧ@��m      	
	DWORD		dwCoreB_InCouter;				// ����B�i�ʧ@�p��
	DWORD		dwCoreB_BWDOption;				// ���lB�ϥμҦ�			
	DWORD		dwtCoreB_BWDDelay;				// ���lB�h�ʧ@����	    	
	DWORD		dwtCoreB_BWDActionTime;			// ���lB�h�ʧ@�ɶ�	    	
//	DWORD		dwCoreB_BWDPosition;			// ���lB�h�ʧ@��m      	
	DWORD		dwCoreB_OutCouter;				// ����B�h�ʧ@�p��
	// CoreC                                                            	
	DWORD		dwfCoreCOption;					// ���lC���				
	DWORD		dsfCoreCMode;					// ���lC����Ҧ�			
	DWORD		dwCoreC_FWDOption;				// ���lC�ϥμҦ�			
	DWORD		dwtCoreC_FWDDelay;				// ���lC�i�ʧ@����	    	
	DWORD		dwtCoreC_FWDActionTime;			// ���lC�i�ʧ@�ɶ�	    	
//	DWORD		dwCoreC_FWDPosition;			// ���lC�i�ʧ@��m      	
	DWORD		dwCoreC_InCouter;				// ����C�i�ʧ@�p��	    	
	DWORD		dwCoreC_BWDOption;				// ���lC�ϥμҦ�			
	DWORD		dwtCoreC_BWDDelay;				// ���lC�h�ʧ@����	    	
	DWORD		dwtCoreC_BWDActionTime;			// ���lC�h�ʧ@�ɶ�	    	
//	DWORD		dwCoreC_BWDPosition;			// ���lC�h�ʧ@��m      	
	DWORD		dwCoreC_OutCouter;				// ����C�h�ʧ@�p��	    	
	// CoreD                                                            	
	DWORD		dwfCoreDOption;					// ���lD���				
	DWORD		dsfCoreDMode;					// ���lD����Ҧ�			
	DWORD		dwCoreD_FWDOption;				// ���lD�ϥμҦ�			
	DWORD		dwtCoreD_FWDDelay;				// ���lD�i�ʧ@����	    	
	DWORD		dwtCoreD_FWDActionTime;			// ���lD�i�ʧ@�ɶ�	    	
//	DWORD		dwCoreD_FWDPosition;			// ���lD�i�ʧ@��m      	
	DWORD		dwCoreD_InCouter;				// ����D�i�ʧ@�p��	    	
	DWORD		dwCoreD_BWDOption;				// ���lD�ϥμҦ�			
	DWORD		dwtCoreD_BWDDelay;				// ���lD�h�ʧ@����	    	
	DWORD		dwtCoreD_BWDActionTime;			// ���lD�h�ʧ@�ɶ�	    	
//	DWORD		dwCoreD_BWDPosition;			// ���lD�h�ʧ@��m      	
	DWORD		dwCoreD_OutCouter;				// ����D�h�ʧ@�p��	    	
	// Blast1                                                           	
	DWORD		dwtBlast1ActionTime;			// �j��1�ɶ�				
	DWORD		dwtBlast1ActionDelay;			// �j��1����		    	
//	DWORD		dwfBlast1Position;				// �j��1��m				
	// Blast2                                                           	
	DWORD		dwtBlast2ActionTime;			// �j��2�ɶ�				
	DWORD		dwtBlast2ActionDelay;			// �j��2����		    	
//	DWORD		dwfBlast2Position;				// �j��2��m				
	// Blast3                                                           	
	DWORD		dwtBlast3ActionTime;			// �j��3�ɶ�				
	DWORD		dwtBlast3ActionDelay;			// �j��3����		    	
//	DWORD		dwfBlast3Position;				// �j��3��m				
	// Blast4                                                           	
	DWORD		dwtBlast4ActionTime;			// �j��4�ɶ�				
	DWORD		dwtBlast4ActionDelay;			// �j��4����				
//	DWORD		dwfBlast4Position;				// �j��4��m				
	// Auto Purge
	DWORD		dwAutoPurgeCouter;				// �M�Ʀ���
	// Monitor Alarm Status                                             	
	DWORD		dwAutoAlarmStatus;      		// �۰�ĵ�����A
	// Defaut monitor value
    DWORD		dwtCycleDefaultMax;				// ���{�p��ĵ���W�����]��   
    DWORD		dwtChargeDefaultMax;         	// �x�ƭp��ĵ���W�����]��   
    DWORD		dwtSuckBackDefaultMax;       	// �g�h�p��ĵ���W�����]��   
    DWORD		dwtNozzleDefaultMax;         	// �y�x�p��ĵ���W�����]��   
    DWORD		dwtEjectDefaultMax;          	// ��ҭp��ĵ���W�����]��   
    //yang 2006/10/18 
    DWORD		dwf24HourHeartor;				// 24�p�ɩw�ɥ[�Ŷ}		
    DWORD		dwfZero;						// �`�ƨ�				0x0001�}���`���k�s 0x0002�˼��`���k�s 0x0004�۰ʼ��
    DWORD		dwfPhotoSensor;					// �q���۰ʨϥ�				
    DWORD		dwfPowerDoor;					// �۰ʦw�����ϥ�			
    DWORD		dwfRobot;						// �����ϥ�	
    DWORD		dwClampUpSelect;				// 0 = Sensor 1 = Torque
}	I32_SETDATA;

typedef struct tagSTATE                 		// �������A
{                                       		
    DWORD       wOperationMode;         		// �ާ@���A     
    DWORD       wOperationStep;         		// �ʧ@���A     
    DWORD       wOperationStep2;        		// �ʧ@���A2  
    DWORD       flMotorStatus;          		// ���F���A    1=ON  0=OFF 
    DWORD       flHeaterStatus;         		// �q�����A    1=ON  0=OFF 
    DWORD      	tmActing;            			// �ʧ@�ɶ�
	DWORD		tCycleTime;						// �`���ɶ�
	DWORD		tCurrentShotCount;				// �}���`��
	DWORD       dwHMIAdjCounter;        		// �e����ܽռҭp�ƭ�		//<<yang 2006/11/1 for I32
	DWORD		dwAutoLub1ShotCounter;			// �۰ʼ��1�Ҽ�		//Alan2006/10/8>>
	DWORD		dwAutoLub1Counter;      		// �۰ʼ��1�p��
	DWORD		dwAutoLub2ShotCounter;			// �۰ʼ��2�Ҽ�
	DWORD		dwAutoLub2Counter;				// �۰ʼ��2�p��		//Alan2006/10/8>>
	WORD		wAlarmStatus;					// ĵ�����A
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
	int			nType;							// �������
	union
	{
		I32_REALDATA	RealData;				// ��ɸ��
		I32_STATE		State;
		I32_FOLIST		FOStateList;
		I32_SETTEMP		SetTemp;
		I32_SETDATA		SetData;				// �]�w���
		I32_MONILIMIT	MoniLimit;
		I32_COREDATA	CoreData;				//yang 2006/10/13 
	};	
} I32_TXDATA;

typedef struct tagRXDATA
{
	int			nType;							// �������
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
	static int		m_nSaveMonitor;				// �~���ƫO�s�лx
	static int		m_nStartAlarm;				// �}�l�i�J�۰�ĵ���έp
	static int		m_nSetDataIDList[SETDATA_COUNT];
	static int		m_nSetCoreIDList[SETCORE_COUNT];
	
	static SETTEMPID	m_SetTempID[TEMP_COUNT];
	static REALTEMPID	m_RealTempID[TEMP_COUNT];
	
	static pthread_t	m_I32ThreadID;
//	static MONITOR		m_Monitor;					// ��e�~���Ʀ�m
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
