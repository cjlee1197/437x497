/*===========================================================================+
|  Class    : TaskMoni                                                       |
|  Task     : Communication Service Routine                                  |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/13/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include	"../../package.h"
#include	"../../timer.h"
#include	"../../main.h"
#include	"../../tmdebug.h"
#include	"../../utils.h"
#include	"../../_kb.h"
#include	"../../commonaction.h"	//Sunny<20070405>
#include	"tmTaskMoni.h"
#include	"../libComm.h"
#include	"../../iotable.h"
#include	"../../tmconfig.h"
#include	"../../manufacture.h"
#include	"../../tmshmsg.h"
#include	"../../tmnbmsg.h"
#include    "../../ammeter.h"
#include	"../libControl.h"
#include	"../../taskammeter.h"
#include	"../hmi_group/tmTaskAmmeterDLT.h"
#include	"../../font.h"
#include	"../../dbid2.h"
#include	"../../filerecord.h"
#include	"../../cursor.h"

IMPLEMENT_DYNCREATE(CtmTaskMoni, CTaskMoni)
typedef void *(* PRSRUN)(void *);
	
//******
//      Initialize static class member
//******
#define		TIME_MONI_SAVEOPERATION			3600*1024	//LEO20070404
#define		PER_MINUTE	60 * 1000 //1000*2
#define		TIME_MONI_SAVEENERGY			3600*1024	//JOYCE2010-7-22 
#define		AMMETER_REQ_TIME			3000
#define 	MAX_OPERLED_NUM			    64  //6*7-9+n

static		tmDATE	u_Currentdate;
long			l_ALUB = 0;
BOOL		CtmTaskMoni::m_bCaton		= FALSE;
BOOL	bBatPower = TRUE, bTimePast = FALSE, bMsgPower = FALSE;	//JOYCE2011-1-19 
OPERATION  Operation01;			//JOYCE2011-1-19  
BOOL  bEnergy = FALSE;//JOYCE2011-7-21 
BOOL    u_bOLDLUB = 0;
BOOL		m_bStepLub = 0, m_b7HTKEYLOCK=FALSE;
int		m_bAmmeter = 0;
int k =0, m =0;

BOOL		m_bAmmeterConfFlag = TRUE; 

void	ProcessPPHData();
DWORD		dwOldShtCnt = 0;

int		u_ActLedMode[18];  //led 
WORD	u_wOPSatusOld	= 0xFFFF;
WORD	u_wOperLedNum	= 0;
WORD	wLedStatusOld[64];

int     u_nOperConf[MAX_OPERLED_NUM];
int     u_nOperLed[MAX_OPERLED_NUM];
WORD    wLedSame[16];
WORD    wCnt =0;

/*joyce20130817 bug:國光液晶問題:在屏保黑屏后再喚醒的時候,會出現橫條白色閃屏,然后慢慢的恢複到正常的畫面
  軟體進行改善:點亮LCD前加大延遲時間,由標準10ms改為200ms*/
extern	int			g_nRTCFd;
#ifdef	D_PXA270
#define     LCD_CMD_BACKONOFF       0x4015
#define     LCD_CMD_SIGNONOFF       0x4012
#define     LCD_CMD_DISPONOFF       0x4013
#define     LCD_CMD_POWRONOFF       0x4014
#else
#define     LCD_CMD_BACKONOFF       1
#define     LCD_CMD_SIGNONOFF       2
#define     LCD_CMD_DISPONOFF       3
#define     LCD_CMD_POWRONOFF       4
#endif
void	SPC_LCDON()
{
  ioctl(g_nRTCFd, LCD_CMD_POWRONOFF, 1);
  usleep(200*1000);
  ioctl(g_nRTCFd, LCD_CMD_SIGNONOFF, 1);
  usleep(200*1000);
  ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 1);
}
//=======================================================================
//Power Ameter start  //JOYCE2011-5-10 add
//=======================================================================
#define   LOWORD(l)           ((WORD)((DWORD)(l) & 0xFFFF)) //JOYCE2010-7-21 
#define   HIWORD(l)           ((WORD)((DWORD)(l) >> 16))    //JOYCE2010-7-21 
#define   UNWORD(hw,lw)       (((DWORD)(hw) << 16) | lw) 		//JOYCE2010-7-21 	

//CAmmeter * pAmmeter = NULL;
pthread_t	PowerThread;
BOOL		bReadAmeter = FALSE;
bool		bTaskAmeter = FALSE;
DWORD	  	dwOldPower  =0, dwOldPower2 =0, dwOldPower3 =0;
int			nAmmeterNo =0;
int			m_nInstallFlag = 0;

static int month[12] = {
	0,
	DAY*(31), 
	DAY*(31+28),
	DAY*(31+28+31),
	DAY*(31+28+31+30),
	DAY*(31+28+31+30+31),
	DAY*(31+28+31+30+31+30),
	DAY*(31+28+31+30+31+30+31),
	DAY*(31+28+31+30+31+30+31+31),
	DAY*(31+28+31+30+31+30+31+31+30),
	DAY*(31+28+31+30+31+30+31+31+30+31),
	DAY*(31+28+31+30+31+30+31+31+30+31+30)
};

void PrintTime(UTIME Time, char *string)
{
	int     year = 0, nMonth = 0, nDay = 0;
    int     nHour = 0, nMinute = 0, nSecond = 0;     
    struct	tm		tmTime;
    tmTime	=*gmtime(&Time);
    nDay	= tmTime.tm_mday;
    nMonth  = tmTime.tm_mon + 1;
    year    = tmTime.tm_year+1900;
    nHour   = tmTime.tm_hour;
    nMinute	= tmTime.tm_min;
    nSecond	= tmTime.tm_sec;
	sprintf(string, "%02u%c%02u%c%02u%c%02u%c%02u%c%02u\n", 
	            nMonth, '/', nDay, '/', year, ' ', nHour, ':',nMinute, ':', nSecond);
}

UTIME MakeTime(tmDATETIME *tm)
{
	UTIME res;
	int   year;
	
#define     MINUTE                           60
#define     HOUR                             (60*MINUTE)
#define     DAY                              (24*HOUR)
#define     YEAR                             (365*DAY)

	year = tm->date.year - 1970;
	res = YEAR*year + DAY*((year+1)/4);									//滿百年的非閏年問題已經無法考慮了
	res += month[tm->date.month - 1];
	
	if ((tm->date.month > 2 ) && (tm->date.year%4 == 0))
		res += DAY;
			
	res += DAY*(tm->date.day-1);
	res += HOUR*tm->time.hour;
	res += MINUTE*tm->time.minute;
	res += tm->time.second;
	
	return res;
}

UTIME GetNowTime()
{
	tmDATETIME      Time;
    UTIME           now;
	GetDate(&Time.date);
	GetTime(&Time.time);  
    now = MakeTime(&Time);
	return now;
}

void 	PowerNotify(int type, void *pdate, int len, int wAmmeterNo)	//m_notifier 
{
	switch ( type ) {
		case AMMETER_NOTIFY_TOTALPOWER:
			if(len == sizeof(double))
			{
				double power = *(double*)pdate;
				long savepower = 0;
				
				if((m_bAmmeter == 2) || (m_bAmmeter == 3))/*CAN 電力計*/
					savepower = (long)power;
				else
					savepower = (long)(power *100); 	 //for precision =2 
				
				//printf("wAmmeterNo:%d, savepower:%d \n", wAmmeterNo, savepower);
				if(wAmmeterNo == 0)
				{
					SetDBValue("AMMETER_HMI_REAL_POWER", savepower);
					SetDBValue("AMMETER_HMI_ACCUMULATE_POWER", 
							savepower - GetDBValue("AMMETER_HMI_ACCUMULATE_POWER_REF").lValue);
					SetDBValue("AMMETER_HMI_DISPOSABLE_POWER", 
							savepower - GetDBValue("AMMETER_HMI_DISPOSABLE_POWER_REF").lValue);
				}
				else  if(wAmmeterNo == 1)
				{
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30", savepower);
					//printf("savepower:%d \n", savepower);
				}
				else  if(wAmmeterNo == 2)
				{
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38", savepower);
					//printf("savepower:%d \n", savepower);
				}
				
				bReadAmeter = TRUE;		//James add 2011/5/12 PM 05:00:49
				bTaskAmeter = TRUE;   //JOYCE2011-5-17
			}
			break;
		default:	
			break;
	}				/* -----  end switch  ----- */
}

void 	FirstNotify(int type, void *pdate, int len, int wAmmeterNo)	//SetNotifier(FirstNotify) 
{
	switch ( type ) {
		case AMMETER_NOTIFY_TOTALPOWER:
			if(len == sizeof(double))
			{
				double power = *(double*)pdate;
				long savepower = 0;
				
				if((m_bAmmeter == 2) || (m_bAmmeter == 3))/*CAN 電力計*/
					savepower = (long)power;
				else
					savepower = (long)(power *100);	 //for precision =2 
				
				if(wAmmeterNo == 0)
				{
					dwOldPower = GetDBValue("AMMETER_HMI_REAL_POWER").lValue;	//last 
					SetDBValue("AMMETER_HMI_REAL_POWER", savepower);
					
					if(GetDBValue("AMMETER_HMI_ACCUMULATE_POWER_RECORDTIME").lValue == 0)
					{
						SetDBValue("AMMETER_HMI_ACCUMULATE_POWER_RECORDTIME", GetNowTime());
						SetDBValue("AMMETER_HMI_ACCUMULATE_POWER_REF", savepower);
						SetDBValue("AMMETER_HMI_ACCUMULATE_POWER", 0);
					}
					else
					{
						SetDBValue("AMMETER_HMI_ACCUMULATE_POWER",
								savepower - GetDBValue("AMMETER_HMI_ACCUMULATE_POWER_REF").lValue);
					}
                	
					if(GetNowTime() - GetDBValue("AMMETER_HMI_DISPOSABLE_POWER_RECORDTIME").lValue > 2*60*60)
					{
						SetDBValue("AMMETER_HMI_DISPOSABLE_POWER_RECORDTIME", GetNowTime());
						SetDBValue("AMMETER_HMI_DISPOSABLE_POWER_REF", savepower);
						SetDBValue("AMMETER_HMI_DISPOSABLE_POWER", 0);
					}
					else
					{
						SetDBValue("AMMETER_HMI_DISPOSABLE_POWER", 
								savepower - GetDBValue("AMMETER_HMI_DISPOSABLE_POWER_REF").lValue);
					}
				}
				else if(wAmmeterNo == 1)
				{
					dwOldPower2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue;
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30", savepower);
				}
				else if(wAmmeterNo == 2)
				{
					dwOldPower3 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue;
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38", savepower);
				}
				
				bReadAmeter = TRUE;		//James add 2011/5/12 PM 05:00:49
				bTaskAmeter = TRUE;   //JOYCE2011-5-17
				//pAmmeter->SetNotifyer(PowerNotify);
				if(g_ptaskAmmeter!= NULL)
				{
					if(m_bAmmeter == 1)
					{
						if(wAmmeterNo == 1)
							g_ptaskAmmeter->SetNotifier(PowerNotify);
					}
					else
						g_ptaskAmmeter->SetNotifier(PowerNotify);
				}
			}
			break;
		default:	
			break;
	}				/* -----  end switch  ----- */
}

void	CheckPowerNub()
{
	DWORD  	dwCurrentAuto =0, dwTotalAutoAmmeter1 =0, dwCurrentProduct =0, dwTotalAutoAmmeter2 =0, dwTotalAutoAmmeter3 =0;
	DWORD  	dwEnergyPerUnit =0;
	
	if(nAmmeterNo ==0)
	{
		dwCurrentAuto 			= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED19").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED18").lValue);
		dwCurrentProduct 		= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED21").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED20").lValue);
		WORD wWeightPerUnit 	= GetDBValue("SYS_HMI_NULL_NULL_RESERVED24").lValue;
		
		if(dwCurrentProduct * wWeightPerUnit)
			dwEnergyPerUnit = (dwCurrentAuto*1000*10)/(dwCurrentProduct * wWeightPerUnit);//0.00度/公斤
		else
			dwEnergyPerUnit = 0;
		SetDBValue("SYS_HMI_NULL_NULL_RESERVED22",LOWORD(dwEnergyPerUnit),FALSE);
		SetDBValue("SYS_HMI_NULL_NULL_RESERVED23",HIWORD(dwEnergyPerUnit),FALSE);
		
		//dwTotalAll 		= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED36").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED35").lValue);	//fuxy, 2012-7-5, SYS_HMI_NULL_NULL_RESERVED31, SYS_HMI_NULL_NULL_RESERVED30 
		dwTotalAutoAmmeter1 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED33").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED32").lValue);
		if(dwCurrentAuto	>= 99999999)
		{
			dwCurrentAuto 	=0;		//2008-9-9, fuxy, 清零
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED18", dwCurrentAuto, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED19", dwCurrentAuto, FALSE);
		}
		if(dwTotalAutoAmmeter1 	>= 99999999)
		{
			dwTotalAutoAmmeter1 	=0;
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED32", dwTotalAutoAmmeter1, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED33", dwTotalAutoAmmeter1, FALSE);
		}
		if(dwCurrentProduct	>= 99999999)//JOYCE2010-7-27
		{
			dwCurrentProduct 	=0;
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED20", dwCurrentProduct, FALSE);
			SetDBValue("SYS_HMI_NULL_NULL_RESERVED21", dwCurrentProduct, FALSE);
		}
	}
	else if(nAmmeterNo ==1) 
	{
		dwTotalAutoAmmeter2 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED31").lValue;

		if(dwTotalAutoAmmeter2 	>= 99999999)
		{
			dwTotalAutoAmmeter2	=0;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED31", dwTotalAutoAmmeter2, FALSE);
		}
	}
	else if(nAmmeterNo ==2) 
	{
		dwTotalAutoAmmeter3 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED35").lValue;

		if(dwTotalAutoAmmeter3 	>= 99999999)
		{
			dwTotalAutoAmmeter3	=0;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED35", dwTotalAutoAmmeter3, FALSE);
		}
	}
}

void *GetPower(void *)
{
	int	nCount = 0;
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
	}
	
	static 	int nSendCount = 0;
	static 	WORD wAmmeterIndex = 0;
	
	WORD  	u_wOPStatus  = 0;
	DWORD	 dwCurrentPower = 0;
	long	lOffset = 0, lAmMeterPlus = 0;
	DWORD  	dwCurrentAuto = 0, dwTotalAuto = 0;
	COM2AMMETERCONFIG	COM2AmmeterConf;
	N2CAMMETERCONFIG	N2CAmmeterConf;

  /*CAN 電力計*/ 
	WORD	AmmeterID[30];
	WORD	wN2CAmmeter[] =
	{
		AMMETER_RS485ID_POSI_WATTHR,
		AMMETER_RS485ID_AVRMS,
		AMMETER_RS485ID_BVRMS,
		AMMETER_RS485ID_CVRMS,
		//AMMETER_RS485ID_AWATT,// Active power of phase A
		//AMMETER_RS485ID_BWATT,// Active power of phase B
		//AMMETER_RS485ID_CWATT,// Active power of phase C
		AMMETER_RS485ID_NEGA_VAR
	};
  /*CAN 電力計*/
   
	if (g_ptaskAmmeter!= NULL)
		g_ptaskAmmeter->SetNotifier(FirstNotify);
	
	while(1)
	{
		if(m_bAmmeter == 2)/*CAN 電力計*/
		{
			if(g_ptaskdsp28_N2C!=NULL && g_ptaskdsp28_N2C->GetOnLineStatus())
			{
				usleep(500*1000);
				continue;
			}
		}
		
		if (g_ptaskAmmeter != NULL)
		{
			COM2AmmeterConf = *((COM2AMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));
			N2CAmmeterConf = *((N2CAMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));

			if(m_bAmmeter == 2)/*CAN 電力計*/
			{
				int nRequestCnt =sizeof(wN2CAmmeter)/sizeof(wN2CAmmeter[0]);
				if(!g_bAmmeterStatus || m_bAmmeterConfFlag) 
				{
					//printf("N2CAmmeterConf.wCount = %d,nRequestCnt=%d\n", N2CAmmeterConf.wCount,nRequestCnt);
					for(int i = 0; i < N2CAmmeterConf.wCount; i++)
					{
						for(int j = 0; j < nRequestCnt; j++)
						{
							AmmeterID[i*nRequestCnt+j] = wN2CAmmeter[j] | ((N2CAmmeterConf.AmmeterInfo[i].wNode) << 8);
							//printf("AmmeterID[%d] = %04X, N2CAmmeterConf.AmmeterInfo[i].wNode = %d\n", i*nRequestCnt+j,AmmeterID[i*nRequestCnt+j], N2CAmmeterConf.AmmeterInfo[i].wNode);
						}
					}
					AmmeterID[N2CAmmeterConf.wCount*nRequestCnt] = AMMETER_REQ_TIME;
			
					g_ptaskAmmeter->Request(AMMETER_CONTROL_ACTUALVALUE, AmmeterID, N2CAmmeterConf.wCount*nRequestCnt+1);
					m_bAmmeterConfFlag = FALSE;
				}
			}

			else  if(m_bAmmeter == 3)
			{
				g_ptaskAmmeter->Request(AMMETER_TOTAL_POWER);
				
				g_ptaskAmmeter->Request(AMMETER_VOLTAGE_A);
				g_ptaskAmmeter->Request(AMMETER_VOLTAGE_B);
				g_ptaskAmmeter->Request(AMMETER_VOLTAGE_C);
				
				g_ptaskAmmeter->Request(AMMETER_CURRENT_A);
				g_ptaskAmmeter->Request(AMMETER_CURRENT_B);
				g_ptaskAmmeter->Request(AMMETER_CURRENT_C);
				
				g_ptaskAmmeter->Request(AMMETER_VOLTAGE_AB);
				g_ptaskAmmeter->Request(AMMETER_VOLTAGE_AC);
				g_ptaskAmmeter->Request(AMMETER_VOLTAGE_BC);
				
				g_ptaskAmmeter->Request(AMMETER_RS485ID_NEGA_WATTHR);
				
				g_ptaskAmmeter->Request(AMMETER_ENERGY);
			}
			
			
//				if(m_bAmmeter == 1)
//				{
			if((m_bAmmeter == 2) || (m_bAmmeter == 3))/*CAN 電力計*/
			{
				if(wAmmeterIndex >= N2CAmmeterConf.wCount)
					wAmmeterIndex = 0;
			}
			else	
			{
				if(wAmmeterIndex >= COM2AmmeterConf.wCount)
					wAmmeterIndex = 0;
			}
					nAmmeterNo = wAmmeterIndex;
					wAmmeterIndex++;
//				}
//				else
//					nAmmeterNo = 0;
				
//				nSendCount++;
				
				g_ptaskAmmeter->Request(AMMETER_ENERGY, nAmmeterNo); 
			
			if(bReadAmeter)			//while read ameter return value ,then begain compress value,
			{
				//printf("taskmoni:nAmmeterNo=%d\n",nAmmeterNo);
				if(nAmmeterNo == 0)
				{
					dwCurrentPower 	= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;
					
					if(dwOldPower != 0)
					{
						lOffset = dwCurrentPower - dwOldPower;
						if(lOffset >= 0)			//if current power >= old power the add the offset
						{
							lAmMeterPlus = lOffset;
						}
						else						//else offset = 0, not need add
						{
							lAmMeterPlus = 0;
						}
						dwOldPower = dwCurrentPower;	
					}
					else
					{
						lAmMeterPlus = dwCurrentPower;
						dwOldPower = dwCurrentPower;
					}
					
					//if(lAmMeterPlus != 0)
        			//	printf("lAmMeterPlus:%d,  current: %d, nAmmeterNo:%d \n", lAmMeterPlus,dwCurrentPower, nAmmeterNo);
        			
					if((m_bAmmeter != 2) && (m_bAmmeter != 3))/*CAN 電力計 COM2弘訊電力計 已經在獲取時乘上CT比了,這裡不需要再乘*/
					{	
					if(COM2AmmeterConf.AmmeterInfo[nAmmeterNo].wCTPara != 0) //not zero	//AMT TM not need 
						lAmMeterPlus *= COM2AmmeterConf.AmmeterInfo[nAmmeterNo].wCTPara/10;	//乘上電表系數
					}
        			
					u_wOPStatus 	= _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) + 1;/*JOYCE2012-6-21 過濾高4個bit-非動作狀態*/
					dwCurrentAuto	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED19").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED18").lValue);
					//dwTotalAll 		= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED36").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED35").lValue);		//SYS_HMI_NULL_NULL_RESERVED31, SYS_HMI_NULL_NULL_RESERVED30 
					dwTotalAuto 	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED33").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED32").lValue);
					
					if(u_wOPStatus)
					{
						dwCurrentAuto += lAmMeterPlus;
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED18",LOWORD(dwCurrentAuto),FALSE);
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED19",HIWORD(dwCurrentAuto),FALSE);
	    		
						dwTotalAuto += lAmMeterPlus;
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED32",LOWORD(dwTotalAuto),FALSE);
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED33",HIWORD(dwTotalAuto),FALSE);
					}
					
					//dwTotalAll += lAmMeterPlus;
					//SetDBValue("SYS_HMI_NULL_NULL_RESERVED35",LOWORD(dwTotalAll),FALSE);		//SYS_HMI_NULL_NULL_RESERVED30 
					//SetDBValue("SYS_HMI_NULL_NULL_RESERVED36",HIWORD(dwTotalAll),FALSE);		//SYS_HMI_NULL_NULL_RESERVED31 
				}
				else if(nAmmeterNo ==1)
				{
					dwCurrentPower 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED30").lValue;
					if(dwOldPower2 != 0)
					{
						lOffset = dwCurrentPower - dwOldPower2;
						if(lOffset >= 0)			//if current power >= old power the add the offset
						{
							lAmMeterPlus = lOffset;
						}
						else						//else offset = 0, not need add
						{
							lAmMeterPlus = 0;
						}
						dwOldPower2 = dwCurrentPower;	
					}
					else
					{
						lAmMeterPlus = dwCurrentPower;
						dwOldPower2 = dwCurrentPower;
					}
					
					//if(lAmMeterPlus != 0)
        			//	printf("1 lAmMeterPlus:%d,  nAmmeterNo:%d \n", lAmMeterPlus, nAmmeterNo);
					if((m_bAmmeter != 2) && (m_bAmmeter != 3))/*CAN 電力計 已經在獲取時乘上CT比了,這裡不需要再乘*/
					{	
					if(COM2AmmeterConf.AmmeterInfo[nAmmeterNo].wCTPara != 0) //not zero	//AMT TM not need 
						lAmMeterPlus *= COM2AmmeterConf.AmmeterInfo[nAmmeterNo].wCTPara/10;	//乘上電表系數
					}
        			
					u_wOPStatus 	= _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) + 1;/*JOYCE2012-6-21 過濾高4個bit-非動作狀態*/
					dwCurrentAuto	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED19").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED18").lValue);
					dwTotalAuto 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED31").lValue;
					
					if(u_wOPStatus)
					{
						dwCurrentAuto += lAmMeterPlus;
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED18",LOWORD(dwCurrentAuto),FALSE);
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED19",HIWORD(dwCurrentAuto),FALSE);

						dwTotalAuto += lAmMeterPlus;
						SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED31", dwTotalAuto,FALSE);
					}
					//dwTotalAll += lAmMeterPlus;
					//SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED", dwTotalAll, FALSE);
				}
				else if(nAmmeterNo ==2)
				{
					dwCurrentPower 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED38").lValue;
					if(dwOldPower3 != 0)
					{
						lOffset = dwCurrentPower - dwOldPower3;
						if(lOffset >= 0)			//if current power >= old power the add the offset
						{
							lAmMeterPlus = lOffset;
						}
						else						//else offset = 0, not need add
						{
							lAmMeterPlus = 0;
						}
						dwOldPower3 = dwCurrentPower;	
					}
					else
					{
						lAmMeterPlus = dwCurrentPower;
						dwOldPower3 = dwCurrentPower;
					}
					
					if((m_bAmmeter != 2) && (m_bAmmeter != 3))/*CAN 電力計 已經在獲取時乘上CT比了,這裡不需要再乘*/
					{	
					if(COM2AmmeterConf.AmmeterInfo[nAmmeterNo].wCTPara != 0) //not zero	//AMT TM not need 
						lAmMeterPlus *= COM2AmmeterConf.AmmeterInfo[nAmmeterNo].wCTPara/10;	//乘上電表系數
					}
        			
					u_wOPStatus 	= _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) + 1;/*JOYCE2012-6-21 過濾高4個bit-非動作狀態*/
					dwCurrentAuto	= UNWORD(GetDBValue("SYS_HMI_NULL_NULL_RESERVED19").lValue,GetDBValue("SYS_HMI_NULL_NULL_RESERVED18").lValue);
					dwTotalAuto 	= GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED35").lValue;
					
					if(u_wOPStatus)
					{
						dwCurrentAuto += lAmMeterPlus;
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED18",LOWORD(dwCurrentAuto),FALSE);
						SetDBValue("SYS_HMI_NULL_NULL_RESERVED19",HIWORD(dwCurrentAuto),FALSE);

						dwTotalAuto += lAmMeterPlus;
						SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED35", dwTotalAuto,FALSE);
					}
				}
				
				CheckPowerNub();
			}
			
			if(nSendCount >= 10 && !g_bAmmeterStatus) 
			{
				//sleep(60);
				nSendCount = 0;
			}
			if(g_bAmmeterStatus) nSendCount = 0;
		
			//printf("bTaskAmeter=%d nCount=%d \n", bTaskAmeter, nCount);
			
			if(bTaskAmeter)	//JOYCE2011-5-17 flag 圖標
			{
				//g_wDsp2Status = TRUE;	//flag 圖標
				bTaskAmeter = FALSE;
				nCount = 0;		//2012-2-6  
			}
			else
			{
				nCount++;
				if(nCount > 2)	//連續3次沒有連上, 才改變狀態圖標的顯示, 2012-2-6  
				{
					nCount = 0;
					//g_wDsp2Status = FALSE;// flag 圖標
				}
			}
			
		}	

		if(m_bAmmeter == 2)/*CAN 電力計*/
			sleep(3); 
		else	
			sleep(2); 
	}
}
//=======================================================================
//Power Ameter end
//=======================================================================

CtmTaskMoni::CtmTaskMoni(): CTaskMoni()
{
	
}

CtmTaskMoni::~CtmTaskMoni()
{

}

//	execute it atfer the contructor
int		CtmTaskMoni::CreateSelf()
{
	CTaskMoni::CreateSelf();
	m_dwShotCountCurrent = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	dwOldShtCnt = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	//printf("CreateSelf  dwOldShtCnt=%d\n",dwOldShtCnt);
	
	m_bIdle = FALSE;
	m_lCounter = GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERTIME").lValue * PER_MINUTE -100;
	m_lHeatCounter = 1000;
	m_lCounterUpdate	= 50;
	m_bSaver = FALSE;
	//m_bCaton = FALSE;
	m_pwndScreen = NULL;
	//LEO20070404
	m_lCounterRun 		= 
	m_lCounterRunAuto 	= 
	m_lCounterRunAutoTime 	= 
	m_lCounterRunMotor	= (long)60*1024;
	m_lCounterNotclmpTime 	= (long)1024;
  m_lCounterEnergydata = TIME_MONI_SAVEENERGY;
	m_lCounterAlarm		= GetDBValue("OPER_HMI_NULL_NULL_ALARMINTERVAL").lValue;
	m_lCounterSaveOpt	= TIME_MONI_SAVEOPERATION;
	m_lCounterCoolFan	  = (long)60*1024;
	m_lCounterLoginIn = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED4").lValue * 60 *1024;

	TestBatPower();    //JOYCE2011-1-19 
	ReadLastRTCTime(); //JOYCE2011-1-19 
	TestInstBatPower();//JOYCE2011-1-19 

	GetDate(&u_Currentdate);
	
	l_ALUB = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED83").lValue;  //JOYCE2013-10-30 g_pExTable->GetTableValue(0,"D_ALUB");	
	bEnergy= g_pExTable->GetTableValue(0,"D_ENERGY");//JOYCE2011-7-21 
	u_bOLDLUB = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70").lValue;//0-NEW LUB 1-OLD LUB
	m_bStepLub = g_pExTable->GetTableValue(0,"D_STEPLUB"); /*JOYCE2012-7-12 No.12-028 遞進式潤滑*/
	m_b7HTKEYLOCK = g_pExTable->GetTableValue(0,"D_7HTKEYLOCK");
	m_bAmmeter 	= CtmConfig::GetInstance()->GetAmmeterFlag();
	
	int	nFd = open("/conf/installFlag", O_RDWR|O_CREAT);
	if (nFd >= 0)
	{
		read(nFd, &m_nInstallFlag, sizeof(m_nInstallFlag));
	}
 	close(nFd);


	if(m_bAmmeter)
	{
	    if(m_bAmmeter == 1)
	    	g_ptaskAmmeter	= static_cast<CTaskAmmeter *>(g_pPackage->CreateClassInstance("HostAmmeter1"));
	    else if(m_bAmmeter == 2)
	    	g_ptaskAmmeter	= static_cast<CTaskAmmeter *>(g_pPackage->CreateClassInstance("HostN2CAmmeter"));/*CAN 電力計*/
	    else if(m_bAmmeter == 3)
	    	g_ptaskAmmeter	= static_cast<CTaskAmmeter *>(g_pPackage->CreateClassInstance("HostAmmeter3"));	
	    else if(m_bAmmeter == 4)
	    	g_ptaskAmmeter	= static_cast<CTaskAmmeter *>(g_pPackage->CreateClassInstance("HostAmmeter4"));		/*JOYCE2013-12-12 松下電表moldbus*/ 

	    pthread_create(&PowerThread, NULL, GetPower, NULL);	
	}


    for(int i = 0; i < 18; i++)
	  u_ActLedMode[i] = g_pExTable->GetTableValue(10,(int*)&i);

	for(int i = 0; i < MAX_OPERLED_NUM; i++)
		u_nOperConf[i] = g_pExTable->GetTableValue(11,(int*)&i);
	for(int i = 0; i < MAX_OPERLED_NUM; i++)
		u_nOperLed[i] = g_pExTable->GetTableValue(12,(int*)&i);
	
	memset(wLedSame, 0, sizeof(wLedSame));
	memset(wLedStatusOld, 0, sizeof(wLedStatusOld));
	for(int i=0; i <MAX_OPERLED_NUM; i++)  //用於不同的狀態，點亮同一個Led
	{
	    if(u_nOperLed[i] != 0xFF)
	    {
	        u_wOperLedNum++;
	        
	        for(int j=i+1; j <MAX_OPERLED_NUM; j++)
	        {
	            if(u_nOperLed[i] == u_nOperLed[j])
	            {
	                wLedSame[wCnt] = u_nOperLed[i];  //記錄有多個狀態均可點亮的Led
	                wCnt++;
	            }
	        }
	    }
	}

	/*#ifdef	D_X86
	m_lCounterRun = (long)1000;
	::AddCounter((long __far*)&m_lCounterRun, m_idMe);
	#endif*/

	RegisterMessage(MSG_NB_HMI_LOWBATPOWER, 		m_idMe);
	RegisterMessage(MSG_DSP54_GET_SHOTCOUNT, m_idMe);
	RegisterMessage(MSG_NB_HMI_SEND_LOGININ, 		m_idMe);
	RegisterMessage(MSG_SH_AMMETER_CONFIG, 		m_idMe);
	
	//LEO20070404
	::AddCounter((long __far*)&m_lCounterRun, m_idMe);
	::AddCounter((long __far*)&m_lCounterRunAuto, m_idMe);
	::AddCounter((long __far*)&m_lCounterRunAutoTime, m_idMe);
	::AddCounter((long __far*)&m_lCounterNotclmpTime, m_idMe);
	::AddCounter((long __far*)&m_lCounterRunMotor, m_idMe);
	::AddCounter((long __far*)&m_lCounterSaveOpt, m_idMe);
	
	::AddCounter((long __far*)&m_lCounter, m_idMe);
	::AddCounter((long __far*)&m_lHeatCounter, m_idMe);
	::AddCounter((long __far*)&m_lCounterEnergydata, m_idMe);
	::AddCounter((long __far*)&m_lCounterCoolFan, m_idMe);
	::AddCounter((long __far*)&m_lCounterLoginIn, m_idMe);
	::AddCounter((long __far*)&m_lCounterUpdate, m_idMe);

	return 0;
}

//	execute it before the destructor
int		CtmTaskMoni::FreeSelf()
{
		//LEO20070404
	::DeleteCounter((long __far*)&m_lCounterSaveOpt, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterRunAuto, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterRunAutoTime, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterNotclmpTime, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterRunMotor, m_idMe);
	//#ifdef	D_X86
	::DeleteCounter((long __far*)&m_lCounterRun, m_idMe);
	//#endif
	::DeleteCounter((long __far*)&m_lCounter, m_idMe);
	::DeleteCounter((long __far*)&m_lHeatCounter, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterEnergydata, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterCoolFan, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterLoginIn, m_idMe);
	::DeleteCounter((long __far*)&m_lCounterUpdate, m_idMe);

	UnRegisterMessage(MSG_NB_HMI_LOWBATPOWER, 		m_idMe); 
	UnRegisterMessage(MSG_DSP54_GET_SHOTCOUNT, m_idMe);
	UnRegisterMessage(MSG_NB_HMI_SEND_LOGININ, 		m_idMe);
	UnRegisterMessage(MSG_SH_AMMETER_CONFIG, 		m_idMe);

	return CTaskMoni::FreeSelf();
}

void	CtmTaskMoni::Run()
{
	int nShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	static	int	nCount = nShotCount;//dbbuff.wSwitchToAutoShutCount ;	// 目標產品總數		James modify 2007/9/21 12:35上午
	static  int nPackCount = nShotCount;								// 保存上一次的目標包裝總數
	static 	BOOL bMsgFlag 	= FALSE;

#ifdef	D_3354	
	if(g_SramWriteFlag > 0) //20150414 
	{
		if(g_SramWriteFlag == 2)  //20150602
        {        //;//fsync(g_SramWriteFd);
            if(g_SramWriteFd > 0)
            {
                ;//printf("sync \n");
                fdatasync(g_SramWriteFd);
            }
            g_SramWriteFlag = 0;
        }
        else
       	{
            ; //printf("===========no sync\n ");       
        }
	}
#endif

	if (/*dbbuff.wSwitchToAutoShutCount*/ nShotCount == 0)				// James modify 2007/9/21 12:35上午
	{
		nCount = 0;
	}

	if(CtmConfig::GetInstance()->GetINETFlag() == 1) //JOYCE2011-2-21 add judgement
	{
		if(GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue == 0)
		{
			bMsgFlag 	= FALSE;	
		}
	}

		
	//BOOL	bScreenSaverLogoutOption=GetDBValue("SYSX_HMI_NULL_NULL_SCREENSAVERLOGOUTOPTION").lValue;//屏保後自動登出
	BOOL	bScreenSaverLCDOFFOption=GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERLCDPOWEROFF").lValue;
	//printf("CtmTaskMoni::Run() m_bIdle=%d m_bSaver=%d m_lCounter=%d\n", m_bIdle, m_bSaver, m_lCounter);
	m_bIdle = g_ptaskCmd->IsIdle();
	if(m_bIdle == TRUE)
	{
		//printf("m_bIdle == TRUE\n");
		//printf("m_lCounter=%d\n",m_lCounter);
		if (m_lCounter <= 0 || 
			(nShotCount - nCount) >= GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERSHOTCOUNT").lValue &&
			GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERSHOTCOUNT").lValue >0 )
		{
			if(m_bSaver == FALSE )	
			{
				m_bSaver 	   = TRUE;
				ScreenSaverForm();
				if(bScreenSaverLCDOFFOption)
					system("echo 0 > /sys/class/backlight/backlight/brightness");
				else
					ScreenSaverForm();
			}
		}	
	}
	else if(m_bIdle == FALSE)
	{
		//printf("m_bIdle == FALSE\n");
		if (m_bSaver == TRUE)	
		{
			if(bScreenSaverLCDOFFOption)
				system("./SetBL");
			m_bSaver = FALSE;
		}
		m_lCounter = GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERTIME").lValue * PER_MINUTE - 100;
		nCount = nShotCount/*dbbuff.wSwitchToAutoShutCount*/;
		
		//printf("Idle FALSE Counter=%ld\n", m_lCounter);
	}

	DWORD	dwShotCountSetting = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT").lValue;
	DWORD	dwShotCountCurrent = 0; 
	
	if(CtmConfig::GetInstance()->GetINETFlag() == 1)
	{
		DB_PRODMANAGE 	*pdbplan 	= (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetCurrentPlan();
		DB_PRODMANAGE 	*pdbplaninfo= (DB_PRODMANAGE *)CtmManufacturePlan::GetInstance()->GetPlanInfo();
		if(pdbplan!=NULL)
		{
			dwShotCountCurrent = pdbplan->dwGoodProductCount;
		}
		else if (pdbplaninfo != NULL && pdbplaninfo->nCurrentIndex == 0)
		{
			dwShotCountCurrent = pdbplaninfo->dwGoodProductCount;
		}
	}
	else
	{
		dwShotCountCurrent = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	}	

	if ((dwShotCountSetting != 0) 							&&
		(m_dwShotCountCurrent != dwShotCountCurrent)		&&
		(dwShotCountCurrent >= dwShotCountSetting)			&&
		!(dwShotCountCurrent & 0x80000000))
	{
		g_ptaskdsp->WriteValue(COMM_SENDSTATUS, COMM_S_REACHSHOTCOUNT);
		//g_ptaskrs232->SendStatus(COMM_S_REACHSHOTCOUNT);
		if(CtmConfig::GetInstance()->GetINETFlag() == 1) //JOYCE2011-2-21 add judgement
		{
			if(!bMsgFlag)
			{
				if(m_bCaton) //JOYCE2011-7-6 bug:屏保動畫,跳出結束計劃的msgbox框,睡眠后當機問題
				{
					Exit();
					m_bCaton = FALSE;
				}
				SendMsg(MSG_SH_MANUFACTURE_SHOTCOUNT, 0, 0, NULL);
				bMsgFlag = TRUE;
			}
			else
			{
				bMsgFlag = FALSE;
			}
		}
	}
	
	DWORD	dwPackCountSetting = GetDBValue("PROD_PRODUCT_PACK_NULL_SETSHOTCOUNT").lValue;
	if ((dwPackCountSetting != 0) 						&&
		(m_dwShotCountCurrent != dwShotCountCurrent)	&&
		((dwShotCountCurrent - nPackCount) >= dwPackCountSetting ) )			//解決中途更改包裝數第一次會不對 James add 2007/9/22 11:11上午
	{
		nPackCount = dwShotCountCurrent;
		if (dwShotCountCurrent != 0) g_ptaskdsp->WriteValue(COMM_SENDSTATUS,COMM_S_REACHPACKCOUNT);
		SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", 0);
	}
    
    if(m_dwShotCountCurrent != dwShotCountCurrent)  //Gwei 2005/5/16加入以解決同一目標產品總數,第二次會多一模才送COMM_S_REACHSHOTCOUNT的問題
    m_dwShotCountCurrent = dwShotCountCurrent;
	
	if((dwOldShtCnt > 0) && (GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue == 0))
	{
		dwOldShtCnt = 0;  /*開模數歸零時的處理*/
		//printf("run  dwOldShtCnt=%d\n",dwOldShtCnt);
	}
		
	/*******
	|       Process save operation
	*******///LEO20070404
	ProcessSaveOperation();
	/*******
	|       Process machine timing
	*******///LEO20070404
	ProcessAutoRun();

	ProcessAutoRunTime();
	ProcessNotclmpTime();
	/*******
	|       Process motor working time
	*******///LEO20070404
	ProcessMotorRun();
	/*******
	|       process stop operation
	*******///LEO20070404
	ProcessStopMachine();
	if(g_CursorChange) 
	{
		//cout<<"==="<<g_CursorX<<"---------"<<g_CursorY<<endl;
		if( g_CursorX >= 770 )
			g_CursorX = 599;
		g_pCursor->Draw(g_CursorX, g_CursorY); 
		g_CursorChange = 0;
	} 
//#ifdef	D_X86
/*2007/4/11 03:59下午 yangpeng
if(m_bSaver == TRUE && m_lCounterRun <= 0)
	{
	m_lCounterRun	= 50;
	}

*/
	if((bEnergy) || m_bAmmeter) //JOYCE2010-7-22 
  {
		ProcessEnergydata(); 
  }

	ProcessCoolFan();  //電機冷卻風扇計時
  
  // #ifdef  D_ALUB
  if(l_ALUB)
  	{
			tmDATE  date;
			long		countDay=0,countDel=0;
			int			Py=360,Pm=30; //系數
			GetDate(&date);
  		countDay	= Py*(date.year%2000)+Pm*(date.month-1)+date.day;
  		countDel	= countDay - GetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION").lValue;
  		//printf("%d,%d,%d\n",countDel,countDay,GetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION").lValue);
			if(GetDBValue("MACH_MDH_MDH_NULL_W0028").lValue != countDel)
  		  {
  		    if((countDel < 0)||(GetDBValue("MACH_MDH_MDH_NULL_W0028").lValue > countDel))
  		      SetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION",countDay - GetDBValue("MACH_MDH_MDH_NULL_W0028").lValue);
  		    else    
  		    	{  		    		
  		    	SetDBValue("MACH_MDH_MDH_NULL_W0028",countDay - GetDBValue("SYSX_MOLDHEGHT_MOLDHEGHT_NULL_KEEPOPTION").lValue);
						if((!u_bOLDLUB) && (!m_bStepLub)) /*JOYCE2012-3-9 新潤滑功能資料,調模潤滑累計天數 處理*/
							{
           			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54",countDel);
								SaveLubricate();
           		}
  		    	}
					//printf("countDay :%ld,countDel:%ld\n",countDay,countDel);  
  		  }   
  	}
   // #endif   

	if(m_b7HTKEYLOCK)  //JOYCE20130318
	{
		if(m_lCounterLoginIn < 0)
		{
			g_iPrivilege = 0;
			if (g_pszUserID != NULL) //取消用戶登入名
			{
				delete []g_pszUserID;
				g_pszUserID = NULL;
			}
			g_wLoginFlag = 0; //用戶登入登出狀態置零
			//printf("m_lCounterLoginIn=%d,g_iPrivilege=%d\n",m_lCounterLoginIn,g_iPrivilege);
			m_lCounterLoginIn = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED4").lValue * 60 *1024;
		}
	}
    
//if (m_lHeatCounter < 0)
//	{
//	CountZero();
//	m_lHeatCounter = 5000;
//	}

//#elif	D_ARM
 if (m_lHeatCounter < 0)
	{
	ClearZeroOil();
	OnTimeHeater();
	TestBatPower();			//yang 2007/7/31 for test battery power
	m_lHeatCounter = 1000;
	//if (g_ptaskCmd->GetFocus() != NULL && !g_wTaskDSP54) 
	//	g_ptaskCmd->GetFocus()->Update();
	}    

	#ifdef	D_CANMASTER  //20160115
	if(GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue != ID_VIEW_PANEL)  //3354 LED
	{
		ShowOperLed();
		ShowActStepLed(); //20150520
	}
	#else
	if((GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue != ID_VIEW_PANEL) &&(CtmConfig::GetInstance()->GetKeyFlag() ==0))     //20150603, MMI270 LED
	{
		ShowOperLed();
		ShowActStepLed(); //20150520
	}
	#endif

  if(m_lCounterUpdate <=0 )/*畫面定時100ms刷新*/
	{
		m_lCounterUpdate = 50;/*zhongjw 2015-7-29 畫面定時50ms刷新*/
		if (g_ptaskCmd->GetFocus() != NULL) 
			g_ptaskCmd->GetFocus()->Update();	
	}

}

void   CtmTaskMoni::ActivateScreenSaver(BOOL bActivate)
{
	static 	BOOL	bLCDPower = TRUE;	//LCD On/Off Flag Caozz add 2011/5/25 下午 06:57:10 
	
	m_bSaver = bActivate;
	if (bActivate)
    {
     	if(bLCDPower)
		   	{
		   	OnIdle();
    		LCDPowerOff();
				bLCDPower = FALSE;
    		}
    }
	else
    {
      if(!bLCDPower)
     		{
	   		OnActive();

				#ifdef	D_3354
		   		int	nValue = GetDB2Value(DBID2_DATA_1080).lValue;
		   		//printf("nValue=%d \n", nValue);
		   		LCDPowerOn_3354(nValue);
		   		#else
				SPC_LCDON();//joyce20130817  LCDPowerOn();
				#endif

				bLCDPower = TRUE;
				}
    }
}


struct lcd_backlight
{
	unsigned int pre_scaler;
	unsigned int period;
	unsigned int count;
};

void		LCDPowerOn_3354(int nCount)
{
	
	struct lcd_backlight	param;
    param.pre_scaler 	= 0;
    param.period 		= 100;
    if(nCount <=50)
    	param.count 		= 50;
    else
    	param.count 		= nCount;
    	
	if (_fbData.nfb == -1)
		_fbData.nfb	= open("/dev/fb0", O_RDWR, 0);
		
    if (ioctl(_fbData.nfb, 0x40044632, &param) == -1) 
    {
		;//perror("Error Setting backlight param");
    }
}

void	 CtmTaskMoni::WndProc(int message, WPARAM wParam, LPARAM lParam) //Andy 20100622
{
	CTask::WndProc(message,wParam,lParam);
	switch (message)
	{
		case MSG_NB_HMI_LOWBATPOWER:
			if(g_ptaskdsp != NULL)                                
	    		g_ptaskdsp->WriteValue(COMM_SENDPANEL,_KEY_PANEL_MANUAL);		//給主機送手動按鍵
	    	SendMsg(MSG_NB_HMI_KEYNOCE, 0, 0, NULL);							//OK和C按鍵無效
			//MsgBox(g_MultiLanguage["MSG_BATTRYLOWER"], tmFT_CODE_TECH);
	  MsgBoxCall("msgboxconfirm1.txt","MSG_BATTRYLOWER");
			break;
		case MSG_DSP54_GET_SHOTCOUNT:
			ProcessPPHData();
			break;
		case MSG_NB_HMI_SEND_LOGININ:	/*joyce20130318自動登出時間*/
			m_lCounterLoginIn = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED4").lValue * 60 *1024;
			//printf("m_lCounterLoginIn=%d\n",m_lCounterLoginIn);
			break;
		case MSG_SH_AMMETER_CONFIG:
			//printf("—————MSG_SH_AMMETER_CONFIG\n");
			m_bAmmeterConfFlag = TRUE;
			break;
		default:
			break;
	}
}

void	CtmTaskMoni::ShowOperLed()
{
    WORD	u_wOPSatus = 0;
  	u_wOPSatus 	= _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue) + 1;
	if(u_wOPSatus == 1)
	{
		for(int j = u_wOPSatus; j < 16; j++) //忽略bit0，從低到高位尋找為1的bit
		{
			if( _TestBit((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue,j))	
			{
				u_wOPSatus = j+1;
				break;
			}
		}
	}
	else if(u_wOPSatus == 0xFFFF) 	u_wOPSatus = 0;

	if((u_wOPSatus < 16) &&(u_wOPSatus != u_wOPSatusOld))
	{
		ShowModeLed(u_wOPSatus);			//James add 2013/5/9 13:30:53 for M3 Keyboard
	}
	u_wOPSatusOld =u_wOPSatus;
}

void	CtmTaskMoni::ShowModeLed(int nLedIndex)
{
	int nOnLed = -1;
	for(int i = 0; i < 18; i++)
	{
	   // printf("%d=%d \n", i,u_ActLedMode[i]);
		if((u_ActLedMode[i] != -1)&&(u_ActLedMode[i] != 65535))
		{
			//printf("nLedIndex =%d\n",nLedIndex);
			if((nLedIndex != 0xFFFF) && (nLedIndex == i))
			{
				_PowerOnLED(u_ActLedMode[i]);
				nOnLed = u_ActLedMode[i];
				//printf("On Mode Led i=%d, %d, %d \n", i, nLedIndex, u_ActLedMode[i]);
				//break;
			}
			else
			{
				if((nOnLed != -1) && (nOnLed != 0xFFFF) &&(u_ActLedMode[i] == nOnLed))
				{
					;
				}
				else
				{
					_PowerOffLED(u_ActLedMode[i]);
				}
				//printf("Off Mode Led i=%d, %d \n", u_ActLedMode[i], nOnLed);
			}
		}	
	}
	if(g_wDspStatus)
	{
		if(nLedIndex == 0)
		{
			/*#ifdef	D_CANMASTER
			if(g_dwM3Key[1] !=0)
			{
				_PowerOnLED(u_ActLedMode[0]);
				_PowerOnLED(61); //20160229, 點亮手動燈的同時也點亮一個面板上不存在的LED，防止開機有時手動燈不能點亮
			}
			#else*/
			_PowerOnLED(u_ActLedMode[0]);
			//#endif
		}
		else
		{
			_PowerOffLED(u_ActLedMode[0]);
		}
	}
	else
	{
		_PowerOffLED(u_ActLedMode[0]);
	}
}

void    CtmTaskMoni::ShowActStepLed()
{
    WORD	wTemp1 =0, wTemp2 =0, wTemp4 =0, wTemp5 =0;
	BOOL    bLedSame =FALSE;
	WORD    wSubStep1   = (WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERSUBSTEP1").lValue;
    WORD    nAirStep =GetDBValue("STATE_OTHR_OTHR_NULL_RSVSELE2").lValue; //OPER.SELE+2
    WORD    nLubStep =GetDBValue("STATE_OTHR_OTHR_NULL_RSVSELE1").lValue;  //OPER.SELE+1
    
    wTemp1 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;  //step +0;
    wTemp2 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP2").lValue;  //step +1;
    wTemp4 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP4").lValue;  //step +3;
    wTemp5 = GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP5").lValue;  //step +4;
    
	for(int i=0; i <u_wOperLedNum; i++)
	{
	    bLedSame =FALSE;
        //printf("i:%d, wTemp:%x,%x,%x,%x u_nOperConf[i]:%x \n", i,  wTemp1, wTemp2, wTemp4, wTemp5, u_nOperConf[i]);
        if((_TestBit(wTemp1, u_nOperConf[i]&0x0F)) &&((u_nOperConf[i] &0x00F0) ==0x0000))
        {
            switch(u_nOperConf[i]&0x0F)
            {
                case 0x0A:  //中子進
                    //printf("i:%d, wSubStep1:%x, u_nOperConf[i]:%x, u_nOperLed[i]:%d\n", i, wSubStep1, u_nOperConf[i], u_nOperLed[i]);
                    if((wSubStep1&0x000F) == (u_nOperConf[i] &0x0F00)>>8)
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    break;
                case 0x0B:  //中子退
                    if((wSubStep1&0x000F) == (u_nOperConf[i] &0x0F00)>>8)
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    break;
                default:
                    _PowerOnLED(u_nOperLed[i]);
                    wLedStatusOld[u_nOperLed[i]] =4;  //記錄上一次亮的狀態
                    //printf("11 i:%d, u_nOperConf[i]:%x, u_nOperLed[i]:%d\n", i, u_nOperConf[i], u_nOperLed[i], wLedStatusOld[u_nOperLed[i]]);
                    continue;
                    break;
            }
        }
        else
        {
            for(int k=0; k<wCnt ;k++)
            {
                if(u_nOperLed[i] == wLedSame[k])
                {
                    bLedSame =TRUE;
                    break;
                }
            }
            //printf("off 11 i:%d, u_nOperLed[i]:%d, bLedSame:%d,wLedStatusOld[i]:%d\n", i, u_nOperLed[i], bLedSame, wLedStatusOld[u_nOperLed[i]]);
            if((!bLedSame) ||(bLedSame && (wLedStatusOld[u_nOperLed[i]] ==0)))  
            {
                _PowerOffLED(u_nOperLed[i]);
            }
            if(bLedSame && (wLedStatusOld[u_nOperLed[i]]>0))
                wLedStatusOld[u_nOperLed[i]]--;   
        }
        
        if((_TestBit(wTemp2, u_nOperConf[i]&0x0F)) &&((u_nOperConf[i] &0x00F0) ==0x0010))
        {
            switch(u_nOperConf[i]&0x0F)
            {
                case 0x02:  //0x12，潤滑
                    //printf("i:%d, nLubStep:%x, u_nOperConf[i]:%x, u_nOperLed[i]:%d\n", i, nLubStep, u_nOperConf[i], u_nOperLed[i]);
                    if(((nLubStep&0x0040) == 0x0040) &&((u_nOperConf[i] &0x0F00) ==0x0100))
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    if(((nLubStep&0x0800) == 0x0800) &&((u_nOperConf[i] &0x0F00) ==0x0200))
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    break;
                case 0x04:  //0x14，公模吹氣
                    printf("i:%d, nAirStep:%x, u_nOperConf[i]:%x, u_nOperLed[i]:%d\n", i, nAirStep, u_nOperConf[i], u_nOperLed[i]);
                    if(((nAirStep &0x0001) == 0x0001) &&((u_nOperConf[i] &0x0F00) ==0x0100))
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    if(((nAirStep &0x0002) == 0x0002) &&((u_nOperConf[i] &0x0F00) ==0x0200))
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    break;
                case 0x05:  //0x15，母模吹氣
                    if(((nAirStep &0x0100) == 0x0100) &&((u_nOperConf[i] &0x0F00) ==0x0100))
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    if(((nAirStep &0x0200) == 0x0200) &&((u_nOperConf[i] &0x0F00) ==0x0200))
                    {
                        _PowerOnLED(u_nOperLed[i]);
                        continue;
                    }
                    else
                        _PowerOffLED(u_nOperLed[i]);
                    break;
                default:
                    //printf("22 i:%d, u_nOperConf[i]:%x, u_nOperLed[i]:%d\n", i, u_nOperConf[i], u_nOperLed[i]&0x0F);
                    _PowerOnLED(u_nOperLed[i]);
                    wLedStatusOld[u_nOperLed[i]] =4; 
                    continue;
                    break;
            }
        }
        else
        {
            for(int k=0; k<wCnt ;k++)
            {
                if(u_nOperLed[i] == wLedSame[k])
                {
                    bLedSame =TRUE;
                    break;
                }
            }
            //if(i<2)
            //    printf("off 22 i:%d, u_nOperConf[i]:%x, u_nOperLed[i]:%d, bLedSame:%d,wLedStatusOld[i]:%d\n", i, u_nOperConf[i], u_nOperLed[i], bLedSame, wLedStatusOld[u_nOperLed[i]]);
            if((!bLedSame) ||(bLedSame && (wLedStatusOld[u_nOperLed[i]] ==0)))
                _PowerOffLED(u_nOperLed[i]);
            if(bLedSame && (wLedStatusOld[u_nOperLed[i]]>0))
                wLedStatusOld[u_nOperLed[i]]--;
        }
        

	}		
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void	CtmTaskMoni::ProcessAutoRun()
{
	long		lTemp;
  WORD    wAutoMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x000F;//JOYCE2010-10-12 
	
	//JOYCE2010-10-12 if (GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue != MODE_MANUAL)   // If the system is in auto mode
	if((g_flMotorStatus == 0xFFFF) && ((wAutoMode == 3) || (wAutoMode == 5) || (wAutoMode == 9))) //JOYCE2010-10-12 
	{
		if (m_lCounterRunAuto <= 0)
		{
			m_lCounterRunAuto = (long)60*1024;
			lTemp = GetDBValue("OPER_HMI_NULL_NULL_MINUTEAUTO").lValue;
			SetDBValue("OPER_HMI_NULL_NULL_MINUTEAUTO", lTemp + 1);
			SetDBValue("OPER_HMI_NULL_NULL_CRC", CountOperationCRC());
		}
	}
	else 	m_lCounterRunAuto = (long)60*1024;
}

void	CtmTaskMoni::ProcessAutoRunTime()
{
	long		lTemp;
  	WORD    wAutoMode = GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x000F;  //JOYCE2010-10-12 //2012-7-10  過濾高4個bit -非動作狀態 
	
	//JOYCE2010-10-12 if (GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue != MODE_MANUAL)   // If the system is in auto mode
	
	if((g_flMotorStatus == 0xFFFF) && ((wAutoMode == 3) || (wAutoMode == 5) || (wAutoMode == 9))) //JOYCE2010-10-12 
	{
		//printf("m_lCounterRunAutoTime =%d\n",m_lCounterRunAutoTime);
		if (m_lCounterRunAutoTime <= 0)
		{
			m_lCounterRunAutoTime = (long)60*1024;
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED10").lValue;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED10", lTemp + 1);
			
			//printf(" taskmoni:  SYSX_OTHERS_OTHERS_DWORD_RESERVED10 =%d\n",GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED10").lValue);
		}
	}
	else 	m_lCounterRunAutoTime = (long)60*1024;
		
}

void	CtmTaskMoni::ProcessNotclmpTime()/*wangli2015-12-10 17:29:18 非開關模狀態時間*/
{
	DWORD		lTemp=0, lTemp2=0;
	WORD		 wState0;
	BOOL		 b2LUB = FALSE ;
	static		WORD	wTime = 0;
	wState0 = (WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue;
	
	//printf("wState0 =%x\n",wState0);
	if ((wState0 & 0x0005) == 0)/*wangli2015-12-29 16:49:06 判斷 不是開關模狀態*/
	{
		if (m_lCounterNotclmpTime <= 0)/*wangli2015-12-30 9:25:12 非開關模狀態按秒累加*/
		{
			m_lCounterNotclmpTime = (long)1024;
			
			wTime ++ ;
			
			if( wTime >= 60 ) /*wangli2015-12-30 9:26:07 非開關模狀態累計時間 以分鐘存入資料*/
			{
			wTime = 0;
			

			lTemp2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED53").lValue;
			
			if(lTemp2 != 0)
			{
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56").lValue;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56", lTemp + 1);
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56").lValue;
			
			
			if(lTemp/60 >= lTemp2)
				{
					SetDBValue("MACH_OTHR_OTHR_NULL_W01AB", 1);
				}
			if(lTemp >= 0xFFFFFFFF)
				{
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED56", 0);
					SaveNotclmpTime();
				}
			}
			

			lTemp2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED54").lValue;
			if((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 1) || (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED84").lValue == 3))
				b2LUB = TRUE;
			else
				b2LUB = FALSE;
			if((lTemp2 != 0) && b2LUB)
			{			
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57").lValue;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57", lTemp + 1);
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57").lValue;
			
			if(lTemp/60 >= lTemp2)
				{
					SetDBValue("MACH_OTHR_OTHR_NULL_W01AC", 1);
				}
			if(lTemp >= 0xFFFFFFFF)
				{
					SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED57", 0);
					SaveNotclmpTime();
				}
			}
			
			}
			
		}
	}
	else	m_lCounterNotclmpTime = (long)1024;
}

void	CtmTaskMoni::ProcessMotorRun()
{
	long		lTemp=0, lTemp2=0;
	
	if (g_flMotorStatus)
	{
		if (m_lCounterRunMotor <= 0)
		{
			m_lCounterRunMotor = (long)60*1024;
			lTemp = GetDBValue("OPER_HMI_NULL_NULL_MINUTEMOTOR").lValue;
			SetDBValue("OPER_HMI_NULL_NULL_MINUTEMOTOR", lTemp + 1);
			SetDBValue("OPER_HMI_NULL_NULL_CRC", CountOperationCRC());

			lTemp2 = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED1").lValue;//JOYCE2011-5-28 馬達計時,可重置
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED1", lTemp2 + 1);    //JOYCE2011-5-28 馬達計時,可重置
		}
	}
	else	m_lCounterRunMotor = (long)60*1024;
}

void	CtmTaskMoni::ProcessStopMachine()
{
	long		lTemp;
	long		lStopHour,lAlarmHour;
	BOOL		bTimeOut;
	char		smessage[100];
	static		int 	nCount = 0;
	
	lTemp		= GetDBValue("OPER_HMI_NULL_NULL_STATE").lValue;
	bTimeOut	= GetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG").lValue;
	if ((lTemp == 0xA5) && (!bTimeOut))
	{
		if (m_lCounterRun <= 0)
		{
			m_lCounterRun = (long)60*1024;
			lTemp = GetDBValue("OPER_HMI_NULL_NULL_MINUTE").lValue;
			SetDBValue("OPER_HMI_NULL_NULL_MINUTE", lTemp + 1);
			m_lCounterAlarm--;
			GetDate(&u_Currentdate);
		}
			lStopHour		= GetDBValue("OPER_HMI_NULL_NULL_STAGES").lValue*65536+GetDBValue("OPER_HMI_NULL_NULL_DAYSTIMEOUTPERIOD").lValue;
			lAlarmHour	= GetDBValue("OPER_HMI_NULL_NULL_STAGENOW").lValue*65536+GetDBValue("OPER_HMI_NULL_NULL_DAYSALARMPERIOD").lValue;
			//printf("lStopHour : %d,lAlarmHour: %d\n",lStopHour,lAlarmHour);
	    if (((lStopHour < GetDBValue("OPER_HMI_NULL_NULL_MINUTE").lValue)&&(lStopHour)&&(m_nInstallFlag == 0))||
	    		 (u_Currentdate.year >  GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue) ||
	        ((u_Currentdate.year == GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue) &&
	         (u_Currentdate.month > GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH").lValue)) ||
	        ((u_Currentdate.year == GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTYEAR").lValue) &&
	         (u_Currentdate.month== GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTMONTH").lValue) &&
	         (u_Currentdate.day  >= GetDBValue("OPER_HMI_NULL_NULL_DATETIMEOUTDAY").lValue)))
	    {	//LEO20070409
		    //if(0 == JudgeFactoryOnWork_Install())           
	        {
	        	SetDBValue("OPER_HMI_NULL_NULL_TIMEOUTFLAG", 1);
	            SaveOperation();
	            
	            if(nCount == 0)
	            {
	            	nCount++;
								if((!m_bCaton) && (!bMsgPower)) //增加判斷,以防屏保動畫時跳出
		            	MsgBox(g_MultiLanguage["VW_TIMEOUTALARM2"],3);	//Sunny<20080828><<<<<
		            g_bCancelInstall =TRUE;
		            InstallLogin();//Sunny<20080828>>>>>>
		            nCount--;
		        }
				if(g_ptaskdsp != NULL) 
	            	g_ptaskdsp->WriteValue(COMM_SENDPANEL,_KEY_PANEL_MANUAL);      
	            //if(g_ptaskrs232 != NULL) 
	            //	g_ptaskrs232->SendPanel((BYTE)_KEY_PANEL_MANUAL);                        
	        }
	    }
	/*******
	|       Process time out alarm
	*******/
	    else if (((lAlarmHour < GetDBValue("OPER_HMI_NULL_NULL_MINUTE").lValue)&&(lAlarmHour)&&(m_nInstallFlag == 0))||
	    					(u_Currentdate.year >  GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue) ||
	             ((u_Currentdate.year == GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue) &&
	              (u_Currentdate.month > GetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH").lValue)) ||
	             ((u_Currentdate.year == GetDBValue("OPER_HMI_NULL_NULL_DATEALARMYEAR").lValue) &&
	              (u_Currentdate.month== GetDBValue("OPER_HMI_NULL_NULL_DATEALARMMONTH").lValue) &&
	              (u_Currentdate.day  >= GetDBValue("OPER_HMI_NULL_NULL_DATEALARMDAY").lValue)))
	    {	//LEO20070409
	        if (m_lCounterAlarm <= 0)
	        {
	            m_lCounterAlarm = GetDBValue("OPER_HMI_NULL_NULL_ALARMINTERVAL").lValue;
	            sprintf(smessage,"%s\n%s",g_MultiLanguage["VW_TIMEOUTALARM"]
	            								,g_MultiLanguage["VW_TIMEOUTALARM2"]);
	            //char		smessageDst[100];
	            //CodeChange(smessageDst, smessage);
	            //MsgBox(smessage);             // If the screen is in sleeping,the 1st PutKey_KB()

	            if(nCount == 0)
	            {
	            	nCount++;
								if((!m_bCaton) && (!bMsgPower)) //增加判斷,以防屏保動畫時跳出
								{
									long lTemp = GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
									lTemp = _SetBit(lTemp, 10);/*mach 1C8 Bit10 送1，輸出不正常燈*/
									SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", lTemp);	
	            		MsgBox(g_MultiLanguage["VW_TIMEOUTALARM"],3);
	            		lTemp = _ClearBit(lTemp, 10);/*mach 1C8 Bit10 送0，關閉不正常燈*/
	            		SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", lTemp);		
	            	}
	            	nCount--;
	            }
	        }                                                   // wake up the screen, the 2nd show the message
	    }
	}
	else
	{
	    m_lCounterRun = (long)60*1024;
	}
}

void	CtmTaskMoni::ProcessSaveOperation()
{
	if (m_lCounterSaveOpt <= 0)
	{
		m_lCounterSaveOpt	= TIME_MONI_SAVEOPERATION;
		SaveOperation();
		SaveShotCount();//JOYCE2010-10-12 累積運行模數
		SaveAutoRunTime();/*JOYCE2014-6-5 自動運行時間*/
		SaveNotclmpTime();/*wangli2016-1-12 9:47:36 累計非開關模狀態時間*/
		if((!u_bOLDLUB) && (!m_bStepLub)) 
			SaveLubricate();  //JOYCE2011-8-3 lubricate
	}
}

void	CtmTaskMoni::ProcessEnergydata() //JOYCE2010-7-22 1小時存一次
{
	if (m_lCounterEnergydata <= 0)
	{
		m_lCounterEnergydata	= TIME_MONI_SAVEOPERATION;
		SaveEnergyData();
	}
}

void	CtmTaskMoni::ProcessCoolFan()  //電機冷卻風扇計時
{
	long		lTemp=0;
	WORD 		wValue = GetDBValue("STATE_IO1_WRITE_NULL_REALDATA2").lValue;

	if ((g_flMotorStatus)&&(wValue & 0x0100)) //motor on&&PC25 ON
	{
		if (m_lCounterCoolFan <= 0)
		{
			m_lCounterCoolFan = (long)60*1024;
			lTemp = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED2").lValue;
			SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED2", lTemp + 1);
		}
	}
	else	m_lCounterCoolFan = (long)60*1024;
}

void		CtmTaskMoni::SaveEnergyData() //JOYCE2010-7-22
{
	DWORD  dwEnergyData[22];
	char*   pDataID[] = {
						"SYS_HMI_NULL_NULL_RESERVED18",
						"SYS_HMI_NULL_NULL_RESERVED19",
						"SYS_HMI_NULL_NULL_RESERVED20",
						"SYS_HMI_NULL_NULL_RESERVED21",
						"SYS_HMI_NULL_NULL_RESERVED22",
						"SYS_HMI_NULL_NULL_RESERVED23",
						"SYS_HMI_NULL_NULL_RESERVED24",
						"SYS_HMI_NULL_NULL_RESERVED25",
						"SYS_HMI_NULL_NULL_RESERVED26",
						"SYS_HMI_NULL_NULL_RESERVED27",
						"SYS_HMI_NULL_NULL_RESERVED28",
						"SYS_HMI_NULL_NULL_RESERVED29",
						"SYS_HMI_NULL_NULL_RESERVED35",  //"SYS_HMI_NULL_NULL_RESERVED30",	//fuxy, 2012-7-5, austone 已使用用作驅動器溫度,換用資料保留35
						"SYS_HMI_NULL_NULL_RESERVED36",	 //"SYS_HMI_NULL_NULL_RESERVED31",	//fuxy, 2012-7-5, austone 已使用用作馬達溫度,換用資料保留36
						"SYS_HMI_NULL_NULL_RESERVED32",
						"SYS_HMI_NULL_NULL_RESERVED33",
						"SYS_HMI_NULL_NULL_RESERVED34",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED31",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED32",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED33",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED34",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED35",
            NULL
            };
	
   memset(dwEnergyData, 0, sizeof(dwEnergyData));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/energy");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/energy.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
    
    if (nFd >= 0)
		{
		for(int i=0; i < 22 ;i++)
			{
			if(pDataID[i] != NULL)	dwEnergyData[i] = GetDBValue(pDataID[i]).lValue;
					//printf("dwEnergyData22[%d]:%d\n",i,dwEnergyData[i]);
			}
		}
	write(nFd, &dwEnergyData, sizeof(dwEnergyData));
	close(nFd);
}

void		CtmTaskMoni::SaveShotCount() //JOYCE2010-10-12 累積運行模數
{
	WORD wShotCount[2];
	char*   pDataID[] = {
						"SYS_HMI_NULL_NULL_RESERVED35",
						"SYS_HMI_NULL_NULL_RESERVED36",
            NULL
            };
	
   memset(wShotCount, 0, sizeof(wShotCount));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/shotcount");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/shotcount.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
    
    if (nFd >= 0)
		{
		for(int i=0; i<2 ;i++)
			{
			if(pDataID[i] != NULL)	wShotCount[i] = GetDBValue(pDataID[i]).lValue;
					//printf("wShotCount22[%d]:%d\n",i,wShotCount[i]);
			}
		}
	write(nFd, &wShotCount, sizeof(wShotCount));
	close(nFd);
}

void		CtmTaskMoni::SaveAutoRunTime() //wangli2014-6-5 11:29:11 AutoRunTime
{
	DWORD dwAutoRunTime[1];
	char*   pDataID[] = {
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED10",
            NULL
            };
	
   memset(dwAutoRunTime, 0, sizeof(dwAutoRunTime));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/autoruntime");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/autoruntime.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
    
    if (nFd >= 0)
		{
		for(int i=0; i<1 ;i++)
			{
			if(pDataID[i] != NULL)	dwAutoRunTime[i] = GetDBValue(pDataID[i]).lValue;
					//printf("dwAutoRunTime22[%d]:%d\n",i,dwAutoRunTime[i]);
			}
		}
	write(nFd, &dwAutoRunTime, sizeof(dwAutoRunTime));
	close(nFd);
}

void    CtmTaskMoni::SaveLubricate()  //JOYCE2011-8-23 lubricate
{
	DWORD dwLubrData[55];
	char*   pDataID1[] = {
						"SYSX_OTHERS_OTHERS_INT_RESERVED20",
						"SYSX_OTHERS_OTHERS_INT_RESERVED21",
						"SYSX_OTHERS_OTHERS_INT_RESERVED22",
						"SYSX_OTHERS_OTHERS_INT_RESERVED23",
						"SYSX_OTHERS_OTHERS_INT_RESERVED24",
						"SYSX_OTHERS_OTHERS_INT_RESERVED25",
						"SYSX_OTHERS_OTHERS_INT_RESERVED26",
						"SYSX_OTHERS_OTHERS_INT_RESERVED27",
						"SYSX_OTHERS_OTHERS_INT_RESERVED28",
						"SYSX_OTHERS_OTHERS_INT_RESERVED29",
						"SYSX_OTHERS_OTHERS_INT_RESERVED30",
						"SYSX_OTHERS_OTHERS_INT_RESERVED31",
						"SYSX_OTHERS_OTHERS_INT_RESERVED32",
						"SYSX_OTHERS_OTHERS_INT_RESERVED33",
						"SYSX_OTHERS_OTHERS_INT_RESERVED34",
						"SYSX_OTHERS_OTHERS_INT_RESERVED35",
						"SYSX_OTHERS_OTHERS_INT_RESERVED36",
						"SYSX_OTHERS_OTHERS_INT_RESERVED37",
						"SYSX_OTHERS_OTHERS_INT_RESERVED38",
						"SYSX_OTHERS_OTHERS_INT_RESERVED39",
						"SYSX_OTHERS_OTHERS_INT_RESERVED40",
						"SYSX_OTHERS_OTHERS_INT_RESERVED41",
						"SYSX_OTHERS_OTHERS_INT_RESERVED42",
						"SYSX_OTHERS_OTHERS_INT_RESERVED43",
						"SYSX_OTHERS_OTHERS_INT_RESERVED44",
						"SYSX_OTHERS_OTHERS_INT_RESERVED45",
						"SYSX_OTHERS_OTHERS_INT_RESERVED46",
						"SYSX_OTHERS_OTHERS_INT_RESERVED47",
						"SYSX_OTHERS_OTHERS_INT_RESERVED48",
						"SYSX_OTHERS_OTHERS_INT_RESERVED49",
						"SYSX_OTHERS_OTHERS_INT_RESERVED50",
						"SYSX_OTHERS_OTHERS_INT_RESERVED51",
						"SYSX_OTHERS_OTHERS_INT_RESERVED52",
						"SYSX_OTHERS_OTHERS_INT_RESERVED53",
						"SYSX_OTHERS_OTHERS_INT_RESERVED54",
						"SYSX_OTHERS_OTHERS_INT_RESERVED55",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED20",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED21",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED22",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED23",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED24",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED26",
						"SYSX_OTHERS_OTHERS_INT_RESERVED56",      /*JOYCE2012-6-25 add 6個 潤滑次數上下限限定*/
						"SYSX_OTHERS_OTHERS_INT_RESERVED57",
						"SYSX_OTHERS_OTHERS_INT_RESERVED58",
						"SYSX_OTHERS_OTHERS_INT_RESERVED59",
						"SYSX_OTHERS_OTHERS_INT_RESERVED60",
						"SYSX_OTHERS_OTHERS_INT_RESERVED61",
						"SYSX_OTHERS_OTHERS_INT_RESERVED62",     /*JOYCE2012-9-14 配置版中增加標識碼*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED50",   /*JOYCE20130809 第1組潤滑切換模數*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED51",   /*JOYCE20130809 第2組潤滑切換模數*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED52",   /*JOYCE20130809 第3組潤滑切換模數*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED53",/*wangli2016-1-5 16:59:51  開機潤滑時數 lub1*/
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED54",/*wangli2016-1-5 16:59:51  開機潤滑時數 lub2*/
            NULL
            };
	
	char*   pDataID2[] = {
            "MACH_LUB_ON_NULL_BYPASSOPTION",
						"SYSX_OTHERS_OTHERS_INT_RESERVED19",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED27",
            NULL
            };

   memset(dwLubrData, 0, sizeof(dwLubrData));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/lubricate1.cdb", szPath);    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);    
    if (nFd >= 0)
		{
		for(int i=0; i<55; i++)
			{
			if(pDataID1[i] != NULL)	dwLubrData[i] = GetDBValue(pDataID1[i]).lValue;
					//printf("dwLubrData1[%d]:%d\n",i,dwLubrData[i]);
			}
		}
	write(nFd, &dwLubrData, sizeof(dwLubrData));
	close(nFd);

		//FILE2
    sprintf(m_szCopyFile, "%s/lubricate2.cdb", szPath);
    nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);    
    if (nFd >= 0)
		{
		for(int i=0; i<3 ;i++)
			{
			if(pDataID2[i] != NULL)	dwLubrData[i] = GetDBValue(pDataID2[i]).lValue;
					//printf("dwLubrData2[%d]:%d\n",i,dwLubrData[i]);
			}
		}
	write(nFd, &dwLubrData, sizeof(dwLubrData));
	close(nFd);
}

void		CtmTaskMoni::SaveNotclmpTime()/*wangli2015-12-10 17:04:14*/
{
	DWORD dwNotclmptime[2];
	char*   pDataID[] = {
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED56",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED57",
            NULL
            };
	
   memset(dwNotclmptime, 0, sizeof(dwNotclmptime));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/notclmptime.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
    
    if (nFd >= 0)
		{
		for(int i=0; i<2 ;i++)
			{
			if(pDataID[i] != NULL)	dwNotclmptime[i] = GetDBValue(pDataID[i]).lValue;
					//printf("taskmoni dwNotclmptime22[%d]:%d\n",i,dwNotclmptime[i]);
			}
		}
	write(nFd, &dwNotclmptime, sizeof(dwNotclmptime));
	close(nFd);
}

void  CtmTaskMoni::TestBatPower()   //JOYCE2011-1-19  
{
#ifdef  D_ARM
	static int nCount = 60;
	static int nMinuteCount = 0;
	int nStat = 0;
	if (++nCount >= 60)  //Sunny<20080218> 每分鐘檢查，連續5分鐘低電壓，再顯示低電壓提示 
	{
	  	#ifdef D_PXA270
   		bBatPower = nStat = ioctl(g_nBatteryFd, 0xC084, 0);
  		#else
	  	bBatPower = nStat = ioctl(g_nBatteryFd, 0x1000, 0);  	//電池電量低  
		#endif
		
	  	if (!nStat)
	  	{
	   		if (++nMinuteCount >= 5)
	   		{
	    		Prompt(g_MultiLanguage["MSG_BATTRYLOWER"],1);  //Sunny<20080213>
	    		nMinuteCount  = 0;
	   		}   
	  	}  
	  	nCount = 0;
	}
#endif
}


void	CtmTaskMoni::ReadLastRTCTime()		//JOYCE2011-1-19 , 讀取上一次保存的時間 
{
	int 		fd =-1; 
	#ifdef D_PXA270
	if ((fd = open("/etc/init.d/install", O_RDWR|O_CREAT)) < 0)
	#else
	if ((fd = open("/conf/install", O_RDWR|O_CREAT)) < 0)
	#endif
	{
		return;
	}
	if(lseek(fd, 0, 0) < 0)		
		;
	if(read(fd, &Operation01, sizeof(OPERATION)) != sizeof(OPERATION))	
		;
	close(fd);
}

void	CtmTaskMoni::TestInstBatPower()		//JOYCE2011-1-19  
{
	tmDATE     	date;
	tmTIME		time;
	
	GetDate(&date);
	GetTime(&time);
	
//	printf("currentdate y=%4d,m=%2d,d=%2d, h=%2d,m=%2d,s=%2d\n", Operation01.CurrentData.year,
//	Operation01.CurrentData.month, Operation01.CurrentData.day, Operation01.CurrentTime.hour,
//	Operation01.CurrentTime.minute, Operation01.CurrentTime.second);
	
//	printf("date y=%4d,m=%2d,d=%2d, h=%2d,m=%2d,s=%2d\n", date.year,
//		date.month, date.day, time.hour, time.minute, time.second);

	if(GetDBValue("OPER_HMI_NULL_NULL_STATE").lValue == 0xA5)
	{
		//當前時間早於上次保存的時間 
		if((Operation01.CurrentData.year >  date.year) || 
	  	((Operation01.CurrentData.year == date.year) && (Operation01.CurrentData.month > date.month)) || 
	  	((Operation01.CurrentData.year == date.year) && (Operation01.CurrentData.month == date.month) && (Operation01.CurrentData.day > date.day)) ||
	  	((Operation01.CurrentData.year == date.year) && (Operation01.CurrentData.month == date.month) && (Operation01.CurrentData.day == date.day) && (Operation01.CurrentTime.hour > time.hour)))
		{
	   		bTimePast = TRUE;
	   		
	   		SetDate(Operation01.CurrentData);
			SetTime(Operation01.CurrentTime);
		}
		
		if((!bBatPower) && bTimePast)
		{
			SendMsg(MSG_NB_HMI_LOWBATPOWER, 0, 0, NULL);
			bMsgPower = TRUE;
			bTimePast = FALSE;
		}
	}
	
}


TMPPHDATA	tmPPHData;
TMPPHDATA	tmPPHData_head;
void	ProcessPPHData()
{
	static	BOOL	bRead = FALSE;
	
	tmDATETIME  datetime;
	BYTE	bMonth=0, bDay = 0, bHour = 0;
//	DWORD	dwStartData = 0;
//	DWORD	dwEndData = 0;
	
	if(!bRead)
	{
		memset(&tmPPHData, 0, sizeof(tmPPHData));
		memset(&tmPPHData_head, 0, sizeof(tmPPHData_head));
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{

		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_PPH))
		{
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_PPH, 0,  &tmPPHData_head, sizeof(tmPPHData_head)); 
		}
		else
		{
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_PPH, 0,  &tmPPHData_head, sizeof(tmPPHData_head));
		}
			
			
	}
	else
	{
		if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DATAPPH_SRAMFILE))
		{
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPPH_SRAMFILE, 0, &tmPPHData_head); 
		}
		else
		{
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPPH_SRAMFILE, 0, &tmPPHData_head);
		}
		bRead = TRUE;
	}
	}
	
	GetDate(&datetime.date);
	GetTime(&datetime.time);
	
	bMonth	= datetime.date.month;
	bDay	= datetime.date.day;
	bHour	= datetime.time.hour;
	
	//find current month & day
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
		CManageFile::GetInstance()->ReadRecord(FILE_RECORD_PPH,0, &tmPPHData_head, sizeof(tmPPHData_head));
	else		
	CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPPH_SRAMFILE, 0, &tmPPHData_head);
		
	int i = 0;	
	int nIndex  = tmPPHData_head.wIndex;
	int wCount  = tmPPHData_head.wCount;
	for(i = 0; i < wCount; i++)
	{
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_PPH,i+1, &tmPPHData, sizeof(tmPPHData));
		else		
		CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPPH_SRAMFILE, i+1, &tmPPHData);
			
		if(tmPPHData.bMonth == bMonth && tmPPHData.bDay == bDay)
		{
			nIndex = i + 1;
			break;
		}
	}
	if(i >= wCount) // no find record
	{
		nIndex++;
		memset(&tmPPHData, 0, sizeof(TMPPHDATA));           
		tmPPHData_head.wCount ++;
	}
	
	if(nIndex > CtmConfig::GetInstance()->GetDataEnergy2MaxNum())
	{
		nIndex = 1;
	}
	
	tmPPHData_head.wIndex = nIndex;
	
	if(tmPPHData_head.wCount > CtmConfig::GetInstance()->GetDataEnergy2MaxNum())
		tmPPHData_head.wCount = CtmConfig::GetInstance()->GetDataEnergy2MaxNum();
	
	tmPPHData.bMonth = bMonth;
	tmPPHData.bDay = bDay;
	
	if(bMonth > 0  && bMonth <=12 && bDay > 0 && bDay <= 31 && bHour >= 0 && bHour < 24)
	{
		DWORD		dwCurrentShtCnt = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;		
		if(dwCurrentShtCnt >= dwOldShtCnt)
		{
			tmPPHData.dwHour[bHour] += (dwCurrentShtCnt - dwOldShtCnt) * GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
			dwOldShtCnt = dwCurrentShtCnt;
		}

	if(CtmConfig::GetInstance()->GetRecordType() == 1)
		{
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_PPH, nIndex, &tmPPHData, sizeof(tmPPHData));
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_PPH, 0, &tmPPHData_head, sizeof(tmPPHData_head));
		}
	else	
		{	
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPPH_SRAMFILE, nIndex, &tmPPHData);
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPPH_SRAMFILE, 0, &tmPPHData_head);
		}
	}
}


#ifndef	D_BEOBJECT_CTMTASKMONI
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskMoni == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskMoni*>(pResult))->CreateSelf();
		g_ptaskMoni = static_cast<CTaskMoni*>(pResult);
	}
	return g_ptaskMoni;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskMoni*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
