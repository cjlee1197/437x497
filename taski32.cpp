/*==========================================================================+
|  Class    : I32 communication          									|
|  Task     : I32 communication file                     					|
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
#include	"i32.h"
#include	"taski32.h"
#include	"main.h"
#include	"utils.h"
#include	"tasksiem.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
const	int	MAX_MONITOR		=		100;

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
extern _FRAMEBUFFERINFO		_fbData;
extern	WORD				g_wTaskCommStat;
#define		I32_UPDATE_COUNTER		200;
WORD		g_wOldEhternetStat = 0;
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
extern	WORD	g_wI32;
//CtmRecordFile	g_MoniFile;
//I32_MONILIMIT	g_MoniLimit;				// 品質上下限
I32_MONI		g_Moni;						// 當前品質資料
I32_MONI		g_LastMoni;					// Last Monitor record	//<<yang 2006/11/6 
I32_MONI		g_monisig;	
	
char*		m_pszSetDataIDList[] =
{
	TECH_LUB_GENER_AUTOLUBACOUNT          ,            //自動潤滑1模數       
	TECH_LUB_GENER_ACTTIME				  ,			   //自動潤滑1監測時間                             
	TECH_LUB_GENER_REQUESTIOCOUNT         ,            //自動潤滑1計數                                 
	//TECH_LUB_INTERFACE_RESPONSEIOCOUNT    ,            //自動潤滑1實際計數                             
	TECH_LUB_GENER_DELAYTIME              ,            //自動潤滑1延遲                                                              
	TECH_LUB_GENER_AUTOLUBACOUNT2         ,            //自動潤滑2模數 
	TECH_LUB_GENER_ACTTIME2               ,            //自動潤滑2監測時間                                
	TECH_LUB_GENER_REQUESTIOCOUNT2        ,            //自動潤滑2計數                                 
	//TECH_LUB_INTERFACE_RESPONSEIOCOUNT2   ,            //自動潤滑2實際計數                             
	TECH_LUB_GENER_DELAYTIME2             ,            //自動潤滑2延遲                                 
	TECH_MDH_GENER_RAILINGTON             ,            //每柵單位頓數                                  
	TECH_MDH_GENER_RAILINGDISTANCE        ,            //每柵單位距離                                  
	TECH_MDH_GENER_RAILINGTIME            ,            //每柵單位時間                                  
	TECH_MDH_GENER_ADJUSTCOUNTER          ,            //調模計數 
	TECH_MDH_GENER_MOLDWIDTH			  ,      		// 模板厚度 		//<<yang 2006/11/1 for I32
	TECH_MDH_GENER_OBJWIDTH 			  ,      		// 模具厚度 
	TECH_MDH_GENER_MODE     			  ,      		// 調模方式                                      
	TECH_MLD_GENER_CLAMPFORCESET          ,            //鎖模力設定值                                  
	TECH_GENER_SETSHOTCOUNT               ,            //設定開模總數                                  
	TECH_GENER_CYCLEDELAY                 ,            //再循環延遲                                    
	TECH_MLD_GENER_CLOSELINK              ,            //關模功能選擇                                  
	TECH_MLD_GENER_OPENLINK               ,            //開模功能選擇                                  
//	TECH_EJT_GENER_POSNOPENLINK           ,            //中途托模位置                                  
	EJT_GENER_CNNOSTROKES                 ,            //脫模次數
	TECH_EJT_GENER_MODE                   ,            //脫模控制: 0=停留 1=定次 2=震動                
	TECH_MLD_GENER_TIMECOOLER             ,            //冷卻時間                                      
	TECH_IJU_GENER_BACKWARDTTIME          ,            //座退時間                                      
	TECH_IJU_GENER_BACKWARDMODE           ,            //座退功能選擇:0=不退 1=儲料後 2=冷卻後 3=射出後
	TECH_IJU_GENER_FORWARDDELAYBRAKEON    ,            //座進剎車延遲時間                              
	TECH_IJU_GENER_FORWARDDELAYCLAMPUP    ,            //座進終延遲時間                                
	TECH_IJU_GENER_BACKWARDDELAYBRAKEOFF  ,            //座退放剎車延遲時間    
	TECH_IJU_GENER_BACKWARDDELAYON		  ,			   //座退終延遲時間                    
	TECH_PSTDECOM_GENER_SUCKBACKMODE      ,            //射退退功能選擇: 0=儲料前 1=儲料後 2=冷卻後    
	TECH_COREA_GENER_FUNCTION             ,            //中子A選用: 0=不用 1=中子 2=絞牙               
	TECH_COREA_GENER_MODE                 ,            //中子A動作控制: 0=限位器 1=時間                
	TECH_COREA_GENER_INPOINT              ,            //中子A使用模式                                 
	TECH_COREA_GENER_INDELAY              ,            //中子A進延遲                                   
	TECH_COREA_GENER_INACTTIME            ,            //中子A進動作時間                               
//	TECH_COREA_GENER_INACTPOSN            ,            //中子A進動作位置                               
	TECH_COREA_GENER_INCOUNT              ,            //絞牙A進齒數                                   
	TECH_COREA_GENER_OUTPOINT             ,            //中子A使用模式                                 
	TECH_COREA_GENER_OUTDELAY             ,            //中子A退延遲                                   
	TECH_COREA_GENER_OUTACTTIME           ,            //中子A動作時間                                 
//	TECH_COREA_GENER_OUTACTPOSN           ,            //中子A動作位置                                 
	TECH_COREA_GENER_OUTCOUNT             ,            //絞牙A齒數                                     
	TECH_COREB_GENER_FUNCTION             ,            //中子B選用: 0=不用 1=中子 2=絞牙               
	TECH_COREB_GENER_MODE                 ,            //中子B動作控制: 0=限位器 1=時間                
	TECH_COREB_GENER_INPOINT              ,            //中子B使用模式                                 
	TECH_COREB_GENER_INDELAY              ,            //中子B進延遲                                   
	TECH_COREB_GENER_INACTTIME            ,            //中子B進動作時間                               
//	TECH_COREB_GENER_INACTPOSN            ,            //中子B進動作位置                               
	TECH_COREB_GENER_INCOUNT              ,            //絞牙B進齒數                                   
	TECH_COREB_GENER_OUTPOINT             ,            //中子B使用模式                                 
	TECH_COREB_GENER_OUTDELAY             ,            //中子B退延遲                                   
	TECH_COREB_GENER_OUTACTTIME           ,            //中子B動作時間                                 
//	TECH_COREB_GENER_OUTACTPOSN           ,            //中子B動作位置                                 
	TECH_COREB_GENER_OUTCOUNT             ,            //絞牙B齒數                                     
	TECH_COREC_GENER_FUNCTION             ,            //中子C選用: 0=不用 1=中子 2=絞牙               
	TECH_COREC_GENER_MODE                 ,            //中子C動作控制: 0=限位器 1=時間                
	TECH_COREC_GENER_INPOINT              ,            //中子C使用模式                                 
	TECH_COREC_GENER_INDELAY              ,            //中子C進延遲                                   
	TECH_COREC_GENER_INACTTIME            ,            //中子C進動作時間                               
//	TECH_COREC_GENER_INACTPOSN            ,            //中子C進動作位置                               
	TECH_COREC_GENER_INCOUNT              ,            //絞牙C進齒數                                   
	TECH_COREC_GENER_OUTPOINT             ,            //中子C使用模式                                 
	TECH_COREC_GENER_OUTDELAY             ,            //中子C退延遲                                   
	TECH_COREC_GENER_OUTACTTIME           ,            //中子C動作時間                                 
//	TECH_COREC_GENER_OUTACTPOSN           ,            //中子C動作位置                                 
	TECH_COREC_GENER_OUTCOUNT             ,            //絞牙C齒數                                     
	TECH_CORED_GENER_FUNCTION             ,            //中子D選用: 0=不用 1=中子 2=絞牙               
	TECH_CORED_GENER_MODE                 ,            //中子D動作控制: 0=限位器 1=時間                
	TECH_CORED_GENER_INPOINT              ,            //中子D使用模式                                 
	TECH_CORED_GENER_INDELAY              ,            //中子D進延遲                                   
	TECH_CORED_GENER_INACTTIME            ,            //中子D進動作時間                               
//	TECH_CORED_GENER_INACTPOSN            ,            //中子D進動作位置                               
	TECH_CORED_GENER_INCOUNT              ,            //絞牙D進齒數                                   
	TECH_COREC_GENER_OUTPOINT             ,            //中子D使用模式                                 
	TECH_CORED_GENER_OUTDELAY             ,            //中子D退延遲                                   
	TECH_CORED_GENER_OUTACTTIME           ,            //中子D動作時間                                 
//	TECH_CORED_GENER_OUTACTPOSN           ,            //中子D動作位置                                 
	TECH_CORED_GENER_OUTCOUNT             ,            //絞牙D齒數                                     
	TECH_BLASTA_GENER_ACTTIME             ,            //吹氣1時間                                     
	TECH_BLASTA_GENER_DELAY               ,            //吹氣1延遲                                     
//	TECH_BLASTA_GENER_ACTPOSN             ,            //吹氣1動作位置                                 
	TECH_BLASTB_GENER_ACTTIME             ,            //吹氣2時間                                     
	TECH_BLASTB_GENER_DELAY               ,            //吹氣2延遲                                     
//	TECH_BLASTB_GENER_ACTPOSN             ,            //吹氣2動作位置                                 
	TECH_BLASTC_GENER_ACTTIME             ,            //吹氣3時間                                     
	TECH_BLASTC_GENER_DELAY               ,            //吹氣3延遲                                     
//	TECH_BLASTC_GENER_ACTPOSN             ,            //吹氣3動作位置                                 
	TECH_BLASTD_GENER_ACTTIME             ,            //吹氣4時間                                     
	TECH_BLASTD_GENER_DELAY               ,            //吹氣4延遲                                     
//	TECH_BLASTD_GENER_ACTPOSN             ,            //吹氣4動作位置                                 
	TECH_RCV_GENER_PURGECOUNT             ,            //清料次數                                      
	TECH_ALM_GENER_AUTOALARMRESET         ,            //自動警報狀態                                  
	TECH_GENER_CYCLEDEFAULTMAX            ,            //全程計時警報上限內設值                        
	TECH_RCV_GENER_CHARGEDEFAULTMAX       ,            //儲料計時警報上限內設值                        
	TECH_PSTDECOM_GENER_SUCKBACKDEFAULTMAX,            //射退計時警報上限內設值                        
	TECH_IJU_GENER_NOZZLEDEFAULTMAX       ,            //座台計時警報上限內設值                        
	TECH_EJT_GENER_DEFAULTMAX,                         //脫模計時警報上限內設值   
	TECH_TEMP_24HOUR_FLAGHEATER,						//24小時定時加溫	
	TECH_MLD_GENER_OPEN_TOTALSETZERO,					//總數到				0x0001開模總數歸零 0x0002裝模總數歸零
	TECH_EJT_GENER_PHOTOSENSOR,							//電眼檢測
	TECH_GENER_FLAGPOWERDOOR,							//自動安全門
	TECH_GENER_FLAGROTBOT,								//機械手選用
	TECH_MDH_GENER_CLAMPUP_SELECT						//鎖模力				//<<yang 2006/11/3 
};

char*		m_pszCoreIDList[] =			//yang 2006/10/13 
{
	TECH_EJT_GENER_POSNOPENLINK,            //中途托模位置
	TECH_COREA_GENER_INACTPOSN ,            //中子A進動作位置 
	TECH_COREA_GENER_OUTACTPOSN,            //中子A動作位置 
	TECH_COREB_GENER_INACTPOSN ,            //中子B進動作位置
	TECH_COREB_GENER_OUTACTPOSN,            //中子B動作位置
	TECH_COREC_GENER_INACTPOSN ,            //中子C進動作位置
	TECH_COREC_GENER_OUTACTPOSN,            //中子C動作位置 
	TECH_CORED_GENER_INACTPOSN ,            //中子D進動作位置
	TECH_CORED_GENER_OUTACTPOSN,            //中子D動作位置
	TECH_BLASTA_GENER_ACTPOSN  ,            //吹氣1動作位置
	TECH_BLASTB_GENER_ACTPOSN  ,            //吹氣2動作位置
	TECH_BLASTC_GENER_ACTPOSN  ,            //吹氣3動作位置
	TECH_BLASTD_GENER_ACTPOSN ,             //吹氣4動作位置
	TECH_EJT_GENER_POSNWORKAREA,			//脫模有效區	
	MLD_GENER_POSNMOLDOPENPOSITION,			//開模終設定位置
	TECH_CORE_GENER_POSNWORKAREA			//中子有效區
};

char*	m_pszMoldCloseCurve[] =
{
	MLD_TRACE_VELOCITYSETPOINT_POSNRECORDINGDATA,	//60310002
	MLD_TRACE_ACTUALVELOCITY_POSNRECORDINGDATA,		//60300002
	MLD_TRACE_ACTUALFORCE_POSNRECORDINGDATA,		//60320002
	MLD_TRACE_FORCELIMIT_POSNRECORDINGDATA			//60330002
};

char*	m_pszInjectCurve[] =
{
	INJ_TRACE_ACTUALHOLDPRESSURE_TMRECORDINGDATA,				//50350002
	INJ_TRACE_ACTUALINJPRESSURE_POSNRECORDINGDATA,				//50330002
	INJ_TRACE_ACTUALINJVELOCITY_POSNRECORDINGDATA,				//50310002
	INJ_TRACE_ACTUALTRANSITIONPOSITION_TMRECORDINGDATA,			//50380002
	INJ_TRACE_ACTUALTRANSITIONPRESSURE_TMRECORDINGDATA,			//50360002
	INJ_TRACE_ACTUALTRANSITIONVELOCITY_TMRECORDINGDATA,			//50370002
	INJ_TRACE_HOLDPRESSURESETPOINT_TMRECORDINGDATA,				//50340002
	INJ_TRACE_INJPRESSURELIMITSETPOINT_POSNRECORDINGDATA,		//50320002
	INJ_TRACE_INTERPOLATEDVELOCITYSETPOINT_POSNRECORDINGDATA	//50300002
};

char*	m_pszRecoveryCurve[] =
{
	RCV_TRACE_ACTUALPRESSURE_POSNRECORDINGDATA,		//30320002
	RCV_TRACE_ACTUALSPEED_POSNRECORDINGDATA,		//30300002
	RCV_TRACE_PRESSURESETPOINT_POSNRECORDINGDATA,	//30330002
	RCV_TRACE_SPEEDSETPOINT_POSNRECORDINGDATA		//30310002
};

int				CTaskI32::m_nID = 0;
int				CTaskI32::m_nTempCount[TEMP_COUNT];
int				CTaskI32::m_nTempIndex[TEMP_COUNT];
int				CTaskI32::m_nSaveMonitor = 0;				// 品質資料保存標誌
int				CTaskI32::m_nStartAlarm = 0;				// 開始進入自動警報統計
WORD			CTaskI32::m_wTestPanel = FALSE;				//<<yang 2006/10/31 for test panel_key
int				CTaskI32::m_nSetDataIDList[SETDATA_COUNT];
int				CTaskI32::m_nSetCoreIDList[SETCORE_COUNT];
//MONITOR			CTaskI32::m_Monitor;
SETTEMPID		CTaskI32::m_SetTempID[TEMP_COUNT];
REALTEMPID		CTaskI32::m_RealTempID[TEMP_COUNT];
pthread_t		CTaskI32::m_I32ThreadID;
I32_REALTEMP	CTaskI32::m_RealTemp[TEMP_COUNT][500];
I32_SETDATA		CTaskI32::m_SetData;
I32_COREDATA	CTaskI32::m_CoreData;
	
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - TaskComm                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskI32::CTaskI32() : CTaskComm()
{
	I32		i32;

	m_lUpdateCounter= I32_UPDATE_COUNTER;
	m_nStartAlarm	= 0;				// 開始進入自動警報統計
	m_nSaveMonitor	= 0;				// 品質資料保存標誌
	m_nID	= -1;
	memset(&g_Moni, 0, sizeof(I32_MONI));
	memset(&g_LastMoni, 0, sizeof(I32_MONI));
	//=======
	// 初始化溫度ID表
	//=======
	for(int i= 0; i < TEMP_COUNT; i ++)
	{
		m_nTempCount[i]	= 0;
		m_nTempIndex[i]	= 0;
	}
	ReadTempID();
	//=======
	//初始化設定資料ID表
	//=======
	DBVALUE	Data;
	DWORD	*pData	= &m_SetData.dwAutoLubraShotCount1;
	for(int i = 0; i < SETDATA_COUNT; i ++)
	{
		Data	= g_pDatabase->Read(m_pszSetDataIDList[i]);
		m_nSetDataIDList[i]	= Data.dwIndex;
		*(pData + i)		= Data.dwData;
		//printf("index = %d, data = %d\n", i, Data.dwData);
	}
	double	*pfData = &m_CoreData.dwEject_TravelPosition;
	for (int i = 0; i < SETCORE_COUNT; i ++)
		{
		Data	= g_pDatabase->Read(m_pszCoreIDList[i]);
		m_nSetCoreIDList[i]	= Data.dwIndex;
		*(pfData + i)		= Data.dData;
		}
	
	i32.nRxSize		= sizeof(I32_RXDATA);
	i32.nTxSize		= sizeof(I32_TXDATA);
	i32.nRxCount	= I32RX_MAXID;
	i32.nTxCount	= I32TX_MAXID;
	//printf("nRxSize=%d, nTxSize=%d, nRxCount=%d, nTxCount=%d\n", i32.nRxSize, i32.nTxSize, i32.nRxCount, i32.nTxCount);
	
	i32.I32Func	= (I32FUNC)Read;
	m_nID			= AddDevice("CI32", &i32);
	
	pthread_create(&m_I32ThreadID, NULL, (PRSRUN)SetTemp, NULL);
	RegisterMessage(MSG_I32, m_idMe);
	AddCounter(&m_lUpdateCounter, m_idMe);
}

CTaskI32::~CTaskI32()
{
	if(m_nID >= 0)	DelDevice(m_nID);
	m_nID	= -1;
	for(int i= 0; i < TEMP_COUNT; i ++)
	{
		m_nTempCount[i]	= 0;
		m_nTempIndex[i]	= 0;
	}
	DeleteCounter(&m_lUpdateCounter, m_idMe);
	UnRegisterAllMessage(MSG_I32);
}

void    CTaskI32::Run()
{
	if (m_lUpdateCounter < 0 && g_ptaskCmd->GetFocus() != NULL)
		{
		/*			
		if (g_wOldEhternetStat != g_wTaskCommStat)				//<<yang 2006/10/22 
			{
			g_wOldEhternetStat = g_wTaskCommStat;
			WriteState();
			}
			*/
		m_lUpdateCounter = I32_UPDATE_COUNTER;
		if (GetStatus() >= 0)	g_wI32 = 1;
		else					g_wI32 = 0;
		g_ptaskCmd->GetFocus()->Update();
		}
}

int		CTaskI32::ReadFO(I32_FOLIST *pList, int nLength)
{
	//=======
	// 為了測試方便
	//=======
//	MSG msg;
	static int	nTTY	= 0;
	if(pList->lCmd[19] == 80)
	{
/*		msg.message		= MSG_KEYDOWN;
		msg.wParam		= (WPARAM)0x6300;
		msg.lParam		= 0;
		g_pApplication->QueueMessage(&msg);
		g_pApplication->PostQuitMessage(HTSK_MAIN);		*///退出程序		
		return 0;
	}
	else if(pList->lCmd[19] == 81)
	{
		//退出圖形模式
		if(_fbData.ntty >= 0)
		{
			if(nTTY == 0)
			{
				ioctl(_fbData.ntty, KDSETMODE, KD_TEXT);
				//close(_fbData.ntty);
				nTTY	= 1;
			}
			else
			{
				ioctl(_fbData.ntty, KDSETMODE, KD_GRAPHICS);
				nTTY	= 0;
			}			
		}
		return 0;
	}
	
	for(int i = 0; i <= 5; i ++)
	{
		//printf("CI32::ReadFO %08x FO=%d, cmd=%08x\n", pList, i, pList->lCmd[i]);
	}
	
	//g_ptasksiem->ExeFO(SIMD_FO_COUNT, SIMD_FOCMD, pList);
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteFO(I32_FOList *pList, int nLength)     				|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteFO(I32_FOLIST *pList) 
{
	I32_TXDATA	TxData;
	
	int i;
	static long	nState[SIMD_FO_COUNT];
	for(i = 0; i < SIMD_FO_COUNT; i ++)
	{			
		if(pList->lState[i] != nState[i])
		{
//			//printf("WriteFO  FO: %2d,", i);
//			//printf("changed state: %ld\n", pList->lState[i]);
			nState[i]	= pList->lState[i];
		}
	}
	
	for(i = 0; i <= 5; i ++)
	{			
			//printf("CI32::WriteFO  FO=%2d,", i);
			//printf("state=%08x\n", pList->lState[i]);
	}
	
	TxData.nType	= I32TX_FO;
	memcpy(&TxData.FOStateList, pList, sizeof(I32_FOLIST));
	SendData(m_nID, (char *)&TxData, TxData.nType);
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadTempID()     										|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::ReadTempID()
{
	int		i;
	char	szID[200];
	char	szIndex[10];
	for(i = 0; i < TEMP_COUNT; i ++)
	{
		sprintf(szIndex, "%d", i + 1);
		
		//設定值
		strncpy(szID,TECH_TEMP_PROF_NSETTING_, 200); 
		strcat(szID, szIndex);
		m_SetTempID[i].nSetting	= g_pDatabase->GetIndex(szID);
//		//printf("%s=%d\n", szID, m_SetTempID[i].nSetting);
		
		strncpy(szID,TECH_TEMP_PROF_NRAMP_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nRamp	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_PROFLMT_NMAX_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nMax	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_PROFLMT_NMIN_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nMin	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_CONFIG_WKP_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nKP	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_CONFIG_WKI_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nKI	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_CONFIG_WKD_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nKD	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_CONFIG_WDT_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nDT	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_CONFIG_WTD_, 200);
		strcat(szID, szIndex);
		m_SetTempID[i].nTD	= g_pDatabase->GetIndex(szID);
		
		// 實際值
		strncpy(szID,TECH_TEMP_INTERFACE_WSTATUS_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nStatus	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NCURRENT_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nCurrent	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NACCCURRENT_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nAccCurrent	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NACCLAST_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nAccLast	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NSUM_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nSum	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NDE_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nDE	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NDDE_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nDDE	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_WKPOUT_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nKPOut	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_WKIOUT_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nKIOut	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_WKDOUT_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nKDOut	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NMAXUPDDE_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nMaxUpDDE	= g_pDatabase->GetIndex(szID);
		
		strncpy(szID,TECH_TEMP_INTERFACE_NMAXDNDDE_, 200);
		strcat(szID, szIndex);
		m_RealTempID[i].nMaxDnDDE	= g_pDatabase->GetIndex(szID);
		
		//=======
		// 發送溫度設定值
		//=======
	}
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadTemp(I32_TEMP *pTemp, int nIndex)     				|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::ReadTemp(I32_REALTEMP *pTemp)
{
	MSG		msg;
	int nIndex	= pTemp->nIndex;
//	//printf("ReadTemp nIndex=%d nCurrent=%d\n", nIndex, pTemp->nCurrent);
	//=======
	// 當前資料存入資料庫
	//=======	
	g_pDatabase->Write(m_RealTempID[nIndex].nStatus,	&pTemp->wStatus);
	g_pDatabase->Write(m_RealTempID[nIndex].nCurrent,	&pTemp->nCurrent);
	g_pDatabase->Write(m_RealTempID[nIndex].nAccCurrent,&pTemp->nAccCurrent);
	g_pDatabase->Write(m_RealTempID[nIndex].nAccLast,	&pTemp->nAccLast);
	g_pDatabase->Write(m_RealTempID[nIndex].nSum,		&pTemp->nSum);
	g_pDatabase->Write(m_RealTempID[nIndex].nDE,		&pTemp->nDE);
	g_pDatabase->Write(m_RealTempID[nIndex].nDDE,		&pTemp->nDDE);
	g_pDatabase->Write(m_RealTempID[nIndex].nKPOut,		&pTemp->wKPOut);
	g_pDatabase->Write(m_RealTempID[nIndex].nKIOut,		&pTemp->wKIOut);
	g_pDatabase->Write(m_RealTempID[nIndex].nKDOut,		&pTemp->wKDOut);
	g_pDatabase->Write(m_RealTempID[nIndex].nMaxUpDDE,	&pTemp->nMaxUpDDE);
	g_pDatabase->Write(m_RealTempID[nIndex].nMaxDnDDE,	&pTemp->nMaxDnDDE);
	
	//=======
	// 存到內存，為畫圖表做準備
	//=======
	m_RealTemp[nIndex][m_nTempIndex[nIndex] ++]	= *pTemp;
	if(m_nTempIndex[nIndex] >= 500)	m_nTempIndex[nIndex]	= 0;
	if(m_nTempCount[nIndex] < 500)	m_nTempCount[nIndex] ++;
	msg.message	= MSG_I32;
	g_pApplication->QueueMessage(&msg);
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteTemp(I32_TEMP *pTemp)     							|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteTemp(I32_SETTEMP *pTemp)
{
	I32_TXDATA	TxData;
	TxData.nType		= I32TX_TEMP;
//	TxData.nTempIndex	= nIndex;
	memcpy(&TxData.SetTemp, pTemp, sizeof(I32_SETTEMP));
//	//printf("WriteTemp Index=%ld Setting=%ld\n", TxData.SetTemp.nIndex, TxData.SetTemp.nSetting);
	SendData(m_nID, (char *)&TxData, TxData.nType);	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteReal(I32_TEMP *pReal)     							|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteReal(I32_REALDATA *pReal)
{
	I32_TXDATA	TxData;
	TxData.nType		= I32TX_REAL;
	
	
	memcpy(&TxData.RealData, pReal, sizeof(I32_REALDATA));
	//#ifdef	D_PXA270
	//TxData.RealData.dClampForceTorque = TorqueToForce(abs((long int)pReal->dClampForceTorque));		//<<yang 2006/11/3 for I32 Torque to Force
	//#else
	//TxData.RealData.dClampForceTorque = TorqueToForce(abs(pReal->dClampForceTorque));		//<<yang 2006/11/3 for I32 Torque to Force
	//#endif
	SendData(m_nID, (char *)&TxData, TxData.nType);	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteReal2(I32_REAL2 *pReal)     						|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
/*int		CTaskI32::WriteReal2(I32_REALDATA2 *pReal)
{
	I32_TXDATA	TxData;
	static	I32_REALDATA2 Data;
	TxData.nType		= I32TX_REAL2;
//	TxData.nTempIndex	= nIndex;
	memcpy(&TxData.RealData2, pReal, sizeof(I32_REALDATA2));
	if(Data.nMoldState != TxData.RealData2.nMoldState)
	{
		Data.nMoldState	= TxData.RealData2.nMoldState;
//		//printf("WriteReal nMoldState=%d \n", Data.nMoldState);
	}
	if(Data.nEjectorState != TxData.RealData2.nEjectorState)
	{
		Data.nEjectorState	= TxData.RealData2.nEjectorState;
//		//printf("WriteReal nEjectorState=%d \n", Data.nEjectorState);
	}
	if(Data.nInjectionState != TxData.RealData2.nInjectionState)
	{
		Data.nInjectionState	= TxData.RealData2.nInjectionState;
//		//printf("WriteReal nInjectionState=%d \n", Data.nInjectionState);
	}
	if(Data.nInjUnitState != TxData.RealData2.nInjUnitState)
	{
		Data.nInjUnitState	= TxData.RealData2.nInjUnitState;
//		//printf("WriteReal nInjUnitState=%d \n", Data.nInjUnitState);
	}
	if(Data.nRecoveryState != TxData.RealData2.nRecoveryState)
	{
		Data.nRecoveryState	= TxData.RealData2.nRecoveryState;
//		//printf("WriteReal nRecoveryState=%d \n", Data.nRecoveryState);
	}
	
//	//printf("WriteTemp \n");
	SendData(m_nID, (char *)&TxData, TxData.nType);	
	return 0;
}*/

/*--------------------------------------------------------------------------+
|  Function :  WriteReal(I32_SETDATA *pSet)    								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteSet(I32_SETDATA *pSet)
{
	I32_TXDATA	TxData;
	TxData.nType		= I32TX_SET;
	memcpy(&TxData.SetData, pSet, sizeof(I32_SETDATA));
	//printf("WriteSet dwAutoAlarmStatus=%u\n",TxData.SetData.dwAutoAlarmStatus);
	SendData(m_nID, (char *)&TxData, TxData.nType);	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteReal(I32_SETDATA *pSet)    								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteMoniLimit()
{
	I32_TXDATA	TxData;
	TxData.nType		= I32TX_MONI;
	I32_MONILIMIT	MoniLimit;
	
	MoniLimit = GetMoniLimit();
	memcpy(&TxData.SetData, &MoniLimit, sizeof(I32_MONILIMIT));
	SendData(m_nID, (char *)&TxData, TxData.nType);	
//	printf("Send MoniLimit!\n");
	return 0;
}

I32_MONILIMIT		CTaskI32::GetMoniLimit()						//<<yang 2006/11/1 for monitor limit
{
	I32_MONI		MoniPct, MoniVal;
	I32_MONILIMIT	MoniLimit;
	//=======
	// 計算上下限值
	//=======
	MoniPct.tmCycleTime			= g_pDatabase->Read(TECH_GENER_MONITOR_CYCLETIMEPCT).dwData;
	MoniPct.tmClampClose		= g_pDatabase->Read(TECH_MLD_GENER_MONITOR_CLAMPCLOSEPCT).dwData;
	MoniPct.tmClampOpen			= g_pDatabase->Read(TECH_MLD_GENER_MONITOR_CLAMPOPENPCT).dwData;
	MoniPct.tmInject			= g_pDatabase->Read(TECH_INJ_GENER_MONITOR_INJECTPCT).dwData;
	MoniPct.tmCharge			= g_pDatabase->Read(TECH_RCV_GENER_MONITOR_CHARGEPCT).dwData;
	MoniPct.tmSuckBack			= g_pDatabase->Read(TECH_PSTDECOM_GENER_MONITOR_SUCKBACKPCT).dwData;
	MoniPct.tmEject				= g_pDatabase->Read(TECH_EJT_GENER_MONITOR_EJECTPCT).dwData;
	MoniPct.adPosi_ClampOpenEnd	= g_pDatabase->Read(TECH_MLD_GENER_MONITOR_CLAMPOPENENDPCT).dData;
	MoniPct.daPres_TurnToHold	= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_PRESTURNTOHOLDPCT).dData;
	MoniPct.tmTurnToHold		= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_TURNTOHOLDPCT).dData;
	MoniPct.adPosi_TurnToHold	= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_POSITURNTOHOLDPCT).dData;
	MoniPct.adPosi_InjectStart	= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_POSIINJECTSTARTPCT).dData;
	MoniPct.adPosi_InjectHoldEnd= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_POSIINJECTHOLDENDPCT).dData;
	
	MoniVal.tmCycleTime			= g_pDatabase->Read(TECH_GENER_MONITOR_CYCLETIMEVAL).dwData;
	MoniVal.tmClampClose		= g_pDatabase->Read(TECH_MLD_GENER_MONITOR_CLAMPCLOSEVAL).dwData;
	MoniVal.tmClampOpen			= g_pDatabase->Read(TECH_MLD_GENER_MONITOR_CLAMPOPENVAL).dwData;
	MoniVal.tmInject			= g_pDatabase->Read(TECH_INJ_GENER_MONITOR_INJECTVAL).dwData;
	MoniVal.tmCharge			= g_pDatabase->Read(TECH_RCV_GENER_MONITOR_CHARGEVAL).dwData;
	MoniVal.tmSuckBack			= g_pDatabase->Read(TECH_PSTDECOM_GENER_MONITOR_SUCKBACKVAL).dwData;
	MoniVal.tmEject				= g_pDatabase->Read(TECH_EJT_GENER_MONITOR_EJECTVAL).dwData;
	MoniVal.adPosi_ClampOpenEnd	= g_pDatabase->Read(TECH_MLD_GENER_MONITOR_CLAMPOPENENDVAL).dData;
	MoniVal.daPres_TurnToHold	= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_PRESTURNTOHOLDVAL).dData;
	MoniVal.tmTurnToHold		= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_TURNTOHOLDVAL).dData;
	MoniVal.adPosi_TurnToHold	= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_POSITURNTOHOLDVAL).dData;
	MoniVal.adPosi_InjectStart	= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_POSIINJECTSTARTVAL).dData;
	MoniVal.adPosi_InjectHoldEnd= g_pDatabase->Read(TECH_TRANSITION_GENER_MONITOR_POSIINJECTHOLDENDVAL).dData;
	
	MoniLimit.MoniMax.tmCycleTime	= (DWORD)(g_LastMoni.tmCycleTime * (1 + MoniPct.tmCycleTime / 100.0) + MoniVal.tmCycleTime);
	MoniLimit.MoniMax.tmClampClose= (DWORD)(g_LastMoni.tmClampClose * (1 + MoniPct.tmClampClose / 100.0) + MoniVal.tmClampClose);
	MoniLimit.MoniMax.tmClampOpen	= (DWORD)(g_LastMoni.tmClampOpen * (1 + MoniPct.tmClampOpen / 100.0) + MoniVal.tmClampOpen);
	MoniLimit.MoniMax.tmInject	= (DWORD)(g_LastMoni.tmInject * (1 + MoniPct.tmInject / 100.0) + MoniVal.tmInject);
	MoniLimit.MoniMax.tmCharge	= (DWORD)(g_LastMoni.tmCharge * (1 + MoniPct.tmCharge / 100.0) + MoniVal.tmCharge);
	MoniLimit.MoniMax.tmSuckBack	= (DWORD)(g_LastMoni.tmSuckBack * (1 + MoniPct.tmSuckBack / 100.0) + MoniVal.tmSuckBack);
	MoniLimit.MoniMax.tmEject		= (DWORD)(g_LastMoni.tmEject * (1 + MoniPct.tmEject / 100.0) + MoniVal.tmEject);
	MoniLimit.MoniMax.adPosi_ClampOpenEnd	= g_LastMoni.adPosi_ClampOpenEnd * (1 + MoniPct.adPosi_ClampOpenEnd / 100.0) + MoniVal.adPosi_ClampOpenEnd;
	MoniLimit.MoniMax.daPres_TurnToHold	= g_LastMoni.daPres_TurnToHold * (1 + MoniPct.daPres_TurnToHold / 100.0) + MoniVal.daPres_TurnToHold;
	MoniLimit.MoniMax.tmTurnToHold= g_LastMoni.tmTurnToHold * (1 + MoniPct.tmTurnToHold / 100.0) + MoniVal.tmTurnToHold;
	MoniLimit.MoniMax.adPosi_TurnToHold	= g_LastMoni.adPosi_TurnToHold * (1 + MoniPct.adPosi_TurnToHold / 100.0) + MoniVal.adPosi_TurnToHold;
	MoniLimit.MoniMax.adPosi_InjectStart	= g_LastMoni.adPosi_InjectStart * (1 + MoniPct.adPosi_InjectStart / 100.0) + MoniVal.adPosi_InjectStart;
	MoniLimit.MoniMax.adPosi_InjectHoldEnd= g_LastMoni.adPosi_InjectHoldEnd * (1 + MoniPct.adPosi_InjectHoldEnd / 100.0) + MoniVal.adPosi_InjectHoldEnd;
//	printf("adPosi_InjectHoldEnd Max=%f Real=%f Percent=%f Delta=%f\n", MoniLimit.MoniMax.adPosi_InjectHoldEnd, 
//		g_LastMoni.adPosi_InjectHoldEnd, MoniPct.adPosi_InjectHoldEnd, MoniVal.adPosi_InjectHoldEnd);
	if((g_LastMoni.tmCycleTime * (1 - MoniPct.tmCycleTime / 100.0) - MoniVal.tmCycleTime) >= 0)
		MoniLimit.MoniMin.tmCycleTime	= (DWORD)(g_LastMoni.tmCycleTime * (1 - MoniPct.tmCycleTime / 100.0) - MoniVal.tmCycleTime);
	else
		MoniLimit.MoniMin.tmCycleTime	= 0;
	if((g_LastMoni.tmClampClose * (1 - MoniPct.tmClampClose / 100.0) - MoniVal.tmClampClose) >= 0)
		MoniLimit.MoniMin.tmClampClose= (DWORD)(g_LastMoni.tmClampClose * (1 - MoniPct.tmClampClose / 100.0) - MoniVal.tmClampClose);
	else
		MoniLimit.MoniMin.tmClampClose= 0;
	if((g_LastMoni.tmClampOpen * (1 - MoniPct.tmClampOpen / 100.0) - MoniVal.tmClampOpen) >= 0)
		MoniLimit.MoniMin.tmClampOpen	= (DWORD)(g_LastMoni.tmClampOpen * (1 - MoniPct.tmClampOpen / 100.0) - MoniVal.tmClampOpen);
	else
		MoniLimit.MoniMin.tmClampOpen	= 0;
	if((g_LastMoni.tmInject * (1 - MoniPct.tmInject / 100.0) - MoniVal.tmInject) >= 0)
		MoniLimit.MoniMin.tmInject	= (DWORD)(g_LastMoni.tmInject * (1 - MoniPct.tmInject / 100.0) - MoniVal.tmInject);
	else
		MoniLimit.MoniMin.tmInject	= 0;
	if((g_LastMoni.tmCharge * (1 - MoniPct.tmCharge / 100.0) - MoniVal.tmCharge) >= 0)
		MoniLimit.MoniMin.tmCharge	= (DWORD)(g_LastMoni.tmCharge * (1 - MoniPct.tmCharge / 100.0) - MoniVal.tmCharge);
	else
		MoniLimit.MoniMin.tmCharge	= 0;
	if((g_LastMoni.tmSuckBack * (1 - MoniPct.tmSuckBack / 100.0) - MoniVal.tmSuckBack) >= 0)
		MoniLimit.MoniMin.tmSuckBack	= (DWORD)(g_LastMoni.tmSuckBack * (1 - MoniPct.tmSuckBack / 100.0) - MoniVal.tmSuckBack);
	else
		MoniLimit.MoniMin.tmSuckBack	= 0;
	if((g_LastMoni.tmEject * (1 - MoniPct.tmEject / 100.0) - MoniVal.tmEject) >= 0)
		MoniLimit.MoniMin.tmEject		= (DWORD)(g_LastMoni.tmEject * (1 - MoniPct.tmEject / 100.0) - MoniVal.tmEject);
	else
		MoniLimit.MoniMin.tmEject		= 0;
	if(g_LastMoni.adPosi_ClampOpenEnd * (1 - MoniPct.adPosi_ClampOpenEnd / 100.0) - MoniVal.adPosi_ClampOpenEnd >= 0)
		MoniLimit.MoniMin.adPosi_ClampOpenEnd	= g_LastMoni.adPosi_ClampOpenEnd * (1 - MoniPct.adPosi_ClampOpenEnd / 100.0) - MoniVal.adPosi_ClampOpenEnd;
	else
		MoniLimit.MoniMin.adPosi_ClampOpenEnd	= 0;
	if(g_LastMoni.daPres_TurnToHold * (1 - MoniPct.daPres_TurnToHold / 100.0) - MoniVal.daPres_TurnToHold >= 0)
		MoniLimit.MoniMin.daPres_TurnToHold	= g_LastMoni.daPres_TurnToHold * (1 - MoniPct.daPres_TurnToHold / 100.0) - MoniVal.daPres_TurnToHold;
	else
		MoniLimit.MoniMin.daPres_TurnToHold	= 0;
	if(g_LastMoni.tmTurnToHold * (1 - MoniPct.tmTurnToHold / 100.0) - MoniVal.tmTurnToHold >= 0)
		MoniLimit.MoniMin.tmTurnToHold= g_LastMoni.tmTurnToHold * (1 - MoniPct.tmTurnToHold / 100.0) - MoniVal.tmTurnToHold;
	else
		MoniLimit.MoniMin.tmTurnToHold= 0;
	if(g_LastMoni.adPosi_TurnToHold * (1 - MoniPct.adPosi_TurnToHold / 100.0) - MoniVal.adPosi_TurnToHold >= 0)
		MoniLimit.MoniMin.adPosi_TurnToHold	= g_LastMoni.adPosi_TurnToHold * (1 - MoniPct.adPosi_TurnToHold / 100.0) - MoniVal.adPosi_TurnToHold;
	else
		MoniLimit.MoniMin.adPosi_TurnToHold	= 0;
	if(g_LastMoni.adPosi_InjectStart * (1 - MoniPct.adPosi_InjectStart / 100.0) - MoniVal.adPosi_InjectStart >= 0)
		MoniLimit.MoniMin.adPosi_InjectStart	= g_LastMoni.adPosi_InjectStart * (1 - MoniPct.adPosi_InjectStart / 100.0) - MoniVal.adPosi_InjectStart;
	else
		MoniLimit.MoniMin.adPosi_InjectStart	= 0;
	if(g_LastMoni.adPosi_InjectHoldEnd * (1 - MoniPct.adPosi_InjectHoldEnd / 100.0) - MoniVal.adPosi_InjectHoldEnd >= 0)
		MoniLimit.MoniMin.adPosi_InjectHoldEnd= g_LastMoni.adPosi_InjectHoldEnd * (1 - MoniPct.adPosi_InjectHoldEnd / 100.0) - MoniVal.adPosi_InjectHoldEnd;
	else
		MoniLimit.MoniMin.adPosi_InjectHoldEnd= 0;
	
	return	MoniLimit;
}

/*--------------------------------------------------------------------------+
|  Function :  WriteError()    												|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteError()
{
	I32_TXDATA	TxData;
	TxData.nType		= I32TX_ERROR;
	SendData(m_nID, (char *)&TxData, TxData.nType);	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadError(WORD wErrorID)    								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::ReadError(WORD* pwErrorID)
{
	SIMD_MSG	msg;
	time_t		now;
	struct tm 	tmDate;	
	DWORD		dwDay, dwTime;
	int			nLeapCount;
	//=======
	// 計算時間
	//=======
	if(pwErrorID == 0)	return 0;
	time(&now);
	tmDate	= *localtime(&now);		// 轉換時間格式
	
	nLeapCount	= (tmDate.tm_year - 92) / 4;
	dwDay	= (tmDate.tm_year - 92) * 365 + tmDate.tm_yday + nLeapCount + 1;
	dwTime	= ((tmDate.tm_hour * 60 + tmDate.tm_min) * 60 + tmDate.tm_sec) * 1000;

	msg.bMsgType	= 0x10;
	msg.bState		= 0x00;
	msg.dwDate		= dwDay;
	msg.dwTime		= dwTime;
	msg.wObjNumber	= 0;
	msg.wErrorID	= *pwErrorID;
	
	//SaveMsg(1, &msg);
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  I32_ReadEMoni(I32_MONI *pMoni)    							|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::ReadMoni(I32_MONI *pMoni)
{	
	g_Moni	= *pMoni;
	
	if(m_nSaveMonitor == 0)		return -1;		
	m_nSaveMonitor	= 0;
	g_LastMoni = g_Moni;
	//=======
	// 把品質資料存入資料庫
	//=======
	g_pDatabase->Write(TECH_INTERFACE_MONITOR_CYCLETIME,						&pMoni->tmCycleTime);
	g_pDatabase->Write(TECH_MLD_INTERFACE_MONITOR_CLAMPCLOSE,					&pMoni->tmClampClose);
	g_pDatabase->Write(TECH_MLD_INTERFACE_MONITOR_CLAMPOPEN,					&pMoni->tmClampOpen);
	g_pDatabase->Write(TECH_INJ_INTERFACE_MONITOR_INJECT,						&pMoni->tmInject);
	g_pDatabase->Write(TECH_RCV_INTERFACE_MONITOR_CHARGE,						&pMoni->tmCharge);
	g_pDatabase->Write(TECH_PSTDECOM_INTERFACE_MONITOR_SUCKBACK,				&pMoni->tmSuckBack);
	g_pDatabase->Write(TECH_EJT_INTERFACE_MONITOR_EJECT,						&pMoni->tmEject);
	g_pDatabase->Write(TECH_MLD_INTERFACE_MONITOR_CLAMPOPENEND,					&pMoni->adPosi_ClampOpenEnd);
	g_pDatabase->Write(TECH_TRANSITION_INTERFACE_MONITOR_PRESTURNTOHOLD,		&pMoni->daPres_TurnToHold);
	g_pDatabase->Write(TECH_TRANSITION_IINTERFACE_MONITOR_TURNTOHOLD,			&pMoni->tmTurnToHold);
	g_pDatabase->Write(TECH_TRANSITION_IINTERFACE_MONITOR_POSITURNTOHOLD,		&pMoni->adPosi_TurnToHold);
	g_pDatabase->Write(TECH_INJ_INTERFACE_MONITOR_POSIINJECTSTART,				&pMoni->adPosi_InjectStart);
	g_pDatabase->Write(TECH_CUSHION_INTERFACE_MONITOR_POSIINJECTHOLDEND,		&pMoni->adPosi_InjectHoldEnd);	

	//=======
	// 把當前品質資料存入文件
	//=======
	I32_MONI2	Monitor;	
	Monitor.dwShotCount	= g_pDatabase->Read(TECH_INTERFACE_SHOTCOUNT).dwData;
	Monitor.Moni		= g_LastMoni;
//	SaveMoni2(&Monitor);
	//=======
	// 自動警報開關模計數
	//=======
	int	nAutoAlarmUnit;
	
	if(m_nStartAlarm > 0)
	{
		nAutoAlarmUnit	= g_pDatabase->Read(TECH_ALM_GENER_AUTOALARMUNIT).lData;
//		printf("nAutoAlarmUnit=%d, m_nStartAlarm=%d\n", nAutoAlarmUnit, m_nStartAlarm);
		if(nAutoAlarmUnit > 0 && m_nStartAlarm ++ >= nAutoAlarmUnit)
			{
			//=======
			// 更改自動警報狀態狀態
			//=======
			m_SetData.dwAutoAlarmStatus	= 1;
			g_pDatabase->Write(TECH_ALM_GENER_AUTOALARMRESET, &m_SetData.dwAutoAlarmStatus);
			WriteValue(TECH_ALM_GENER_AUTOALARMRESET);
			m_nStartAlarm			= 0;
			Write(I32TX_MONI, 0, NULL);
			}
	}
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  SetCurve(WORD nStepIndex) 								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::SetCurve(WORD nStepIndex)
{
	char	sz[0xFF];
	double	dMaxData;
	double 	dMax, dMin;	
	DBVALUE	data;
	//=======
	// 設置脫模曲線最大長度
	//=======
	if(nStepIndex == 0)
	{
		strncpy(sz, "EJT_PROFLMT_VELOCITYFWD_POSN", 0xFF);
		dMax	= g_pDatabase->Read(sz).dData;
		//printf("eject curve max is %3.2f\n", dMax);
		
		strncpy(sz, "EJT_PROFLMT_VELOCITYBWD_POSN", 0xFF);
		dMin	= g_pDatabase->Read(sz).dData;
		//printf("eject curve max is %3.2f\n", dMin);
		
		dMaxData	= dMax -dMin;
		
		strncpy(sz, "CURVES1_TRACE_POSNPOSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES2_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES3_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES4_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES5_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES6_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
	}
	else
	{
		dMaxData	= 3;
		
		strncpy(sz, "CURVES1_TRACE_POSNPOSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES2_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES3_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES4_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES5_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
		
		strncpy(sz, "CURVES6_TRACE_POSNRECORDINGDATA", 0xFF);
		data		= g_pDatabase->Read(sz);
		data.dData	= dMaxData;
		//g_ptasksiem->WriteValue(1, &data);
	}
/*	int	nCurveList[9];
	
	switch(nStepIndex)
	{
		case 1:		//關模
			nCurveList[0]	= CURVE_CLOSEMOLD_SPEEDSET;
			nCurveList[1]	= CURVE_CLOSEMOLD_SPEEDREAL;
			nCurveList[2]	= CURVE_CLOSEMOLD_PRESSSET;
			nCurveList[3]	= CURVE_CLOSEMOLD_PRESSREAL;
			g_ptasksiem->ReqCurve(4, 1, nCurveList);
			break;
		case 51:	//射出
			nCurveList[0]	= CURVE_INJECT_SPEEDSET;
			nCurveList[1]	= CURVE_INJECT_SPEEDREAL;
			nCurveList[2]	= CURVE_INJECT_PRESSSET;
			nCurveList[3]	= CURVE_INJECT_PRESSREAL;
			nCurveList[4]	= CURVE_CONVER_SPEED;
			nCurveList[5]	= CURVE_CONVER_PRESS;
			nCurveList[6]	= CURVE_CONVER_POSITOIN;
			nCurveList[7]	= CURVE_HOLDPRESS_SET;
			nCurveList[8]	= CURVE_HOLDPRESS_REAL;
			g_ptasksiem->ReqCurve(9, 1, nCurveList);
			break;
		case 52:
			nCurveList[0]	= CURVE_CHARGE_SPEEDSET;
			nCurveList[1]	= CURVE_CHARGE_SPEEDREAL;
			nCurveList[2]	= CURVE_CHARGE_PRESSSET;
			nCurveList[3]	= CURVE_CHARGE_PRESSREAL;
			g_ptasksiem->ReqCurve(4, 1, nCurveList);
			break;	//保壓
		default:;			
	}
*/
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadState(I32_STATE *pState) 								|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::ReadState(I32_STATE *pState)
{
	static 	I32_STATE	OldState = {0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF, 0xFF};
	char	szID[100];
	int		nIndex;
	
	if(pState == NULL)	return -1;
	
	if(OldState.wOperationMode != pState->wOperationMode)
	{
		SetCurve(pState->wOperationMode);
		OldState.wOperationMode	= pState->wOperationMode;
		g_pDatabase->Write(TECH_INTERFACE_OP_MODE,		&pState->wOperationMode);
		if((pState->wOperationMode == 2 || pState->wOperationMode == 1) && m_SetData.dwAutoAlarmStatus == 0)
			m_nStartAlarm	= 1;
		SetDBValue(COMMAND__IPC_OPERATINGMODE, OldState.wOperationMode);
		
	}
	if(OldState.wOperationStep != pState->wOperationStep)
	{
//		SetCurve(OldState.wOperationStep);
		OldState.wOperationStep	= pState->wOperationStep;
		g_pDatabase->Write(TECH_INTERFACE_OP_STEP,		&pState->wOperationStep);
		if (pState->wOperationStep == 0x50) 
			{
			PutKeyTail(0x6300);
//			printf("SaveCurrentScreen!\n");
			}
	}
	if(OldState.wOperationStep2 != pState->wOperationStep2)						//yang 2006/10/13 
	{
		OldState.wOperationStep2	= pState->wOperationStep2;
		g_pDatabase->Write(TECH_INTERFACE_OP_STEP2,		&pState->wOperationStep2);
	}
	if(OldState.flMotorStatus != pState->flMotorStatus)
	{
		OldState.flMotorStatus = pState->flMotorStatus;
		g_pDatabase->Write(TECH_INTERFACE_MOTOR_STATUS,	&pState->flMotorStatus);
	}
	if(OldState.flHeaterStatus != pState->flHeaterStatus)
	{
		OldState.flHeaterStatus = pState->flHeaterStatus;
		g_pDatabase->Write(TECH_TEMP_INTERFACE_HEATER_STATUS,&pState->flHeaterStatus);
	}
	if(OldState.tmActing != pState->tmActing)
	{
		OldState.tmActing = pState->tmActing;
		g_pDatabase->Write(TECH_INTERFACE_ACTION_TIME,	&pState->tmActing);
	}
	if(OldState.tCycleTime != pState->tCycleTime)
	{
		OldState.tCycleTime = pState->tCycleTime;
		g_pDatabase->Write(TECH_INTERFACE_CYCLE_TIME,	&pState->tCycleTime);
	}
	if(OldState.tCurrentShotCount != pState->tCurrentShotCount)
	{
//		//printf("CurrentShotCount=%u\n",pState->tCurrentShotCount);
		OldState.tCurrentShotCount = pState->tCurrentShotCount;
		g_pDatabase->Write(TECH_INTERFACE_SHOTCOUNT,	&pState->tCurrentShotCount);
		m_nSaveMonitor	= 1;		
	}		
	if(OldState.dwHMIAdjCounter != pState->dwHMIAdjCounter)						// 畫面顯示調模計數值
	{
		OldState.dwHMIAdjCounter = pState->dwHMIAdjCounter;
		g_pDatabase->Write(TECH_MDH_INTERFACE_COUNTER,	&pState->dwHMIAdjCounter);
	}
	if(OldState.wAlarmStatus != pState->wAlarmStatus)
	{
		OldState.wAlarmStatus	= pState->wAlarmStatus;
		g_pDatabase->Write(TECH_ALM_GENER_ALARMSTATUS,	&pState->wAlarmStatus);
	}
	
	if(OldState.wInOperation != pState->wInOperation)
	{
		OldState.wInOperation	= pState->wInOperation;
		g_pDatabase->Write(TECH_INTERFACE_INOPERATION,	&pState->wInOperation);
		_PutKeyTail(0xFFFF);
	}
	if(OldState.dwAutoLub1Counter != pState->dwAutoLub1Counter)					//yang 2006/10/13 
	{
		OldState.dwAutoLub1Counter	= pState->dwAutoLub1Counter;
		g_pDatabase->Write(TECH_LUB_INTERFACE_RESPONSEIOCOUNT,	&pState->dwAutoLub1Counter);
	}
	
	if(OldState.dwAutoLub2Counter != pState->dwAutoLub2Counter)		//yang 2006/10/13 
	{
		OldState.dwAutoLub2Counter	= pState->dwAutoLub2Counter;
		g_pDatabase->Write(TECH_LUB_INTERFACE_RESPONSEIOCOUNT2,	&pState->dwAutoLub2Counter);
	}
	if(OldState.dwAutoLub1ShotCounter != pState->dwAutoLub1ShotCounter)		//yang 2006/10/13 
	{
		OldState.dwAutoLub1ShotCounter	= pState->dwAutoLub1ShotCounter;
		g_pDatabase->Write(TECH_LUB_INTERFACE_RESPONSEIOSHOTCOUNT,	&pState->dwAutoLub1ShotCounter);
	}
	if(OldState.dwAutoLub2ShotCounter != pState->dwAutoLub2ShotCounter)		//yang 2006/10/13 
	{
		OldState.dwAutoLub2ShotCounter	= pState->dwAutoLub2ShotCounter;
		g_pDatabase->Write(TECH_LUB_INTERFACE_RESPONSEIOSHOTCOUNT2,	&pState->dwAutoLub2ShotCounter);
	}
	
	for(int i = 1; i <= 8; i ++)
	{
		if(OldState.awInPortValue[i - 1] != pState->awInPortValue[i - 1])
		{
			OldState.awInPortValue[i - 1]	= pState->awInPortValue[i - 1];
			sprintf(szID, TECH_INTERFACE_INPUT_, i);
			g_pDatabase->Write(szID, &OldState.awInPortValue[i - 1]);
			if (pState->awInPortValue[i - 1] > 0 && m_wTestPanel == 1 && i > 4)
				{
				nIndex = 16 * (i - 5) + _ScanBitR(pState->awInPortValue[i - 1]) + 1;
				//PutPanelKey(nIndex);
				}
		}
		if(OldState.awOutPortValue[i - 1] != pState->awOutPortValue[i - 1])
		{
			OldState.awOutPortValue[i - 1]	= pState->awOutPortValue[i - 1];
			sprintf(szID, TECH_INTERFACE_OUTPUT_, i);
			g_pDatabase->Write(szID, &OldState.awOutPortValue[i - 1]);
		}
	}
	OnTimeHeater();
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  Read(char *pData, int nLength)            				|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::Read(char *pData, int nLength)
{
	I32_RXDATA	*pRx	= (I32_RXDATA *)pData;
	
	for(int i = 0; i < I32RX_MAXID; i ++)
	{	
		switch(pRx->nType)
		{
			case I32RX_FO:
				ReadFO(&pRx->FOCmdList, sizeof(I32_FOLIST));
				break;
			case I32RX_TEMP:
				ReadTemp(&pRx->RealTemp);
				break;
			case I32RX_ERROR:
				ReadError(&pRx->wErrorID);
				break;
			case I32RX_MONI:
				ReadMoni(&pRx->Monitor);
				break;
			case I32RX_STATE:
				ReadState(&pRx->State);
				break;
			default:;
		}
		pRx->nType = 0;
		pRx ++;
	}		
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  Write(int nType, int nLength, void *pData)					|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::Write(int nType, int nLength, void *pData)
{
	if(m_nID < 0)	return -1;		
	switch(nType)
	{
		case I32TX_FO:
			WriteFO((I32_FOLIST *)pData);
			break;
		case I32TX_TEMP:
			WriteTemp((I32_SETTEMP *)pData);
			break;
		case I32TX_REAL:
			WriteReal((I32_REALDATA *)pData);
			break;
		case I32TX_STATE:
			WriteState();
			break;
		case I32TX_SET:
			WriteSet((I32_SETDATA *)pData);
			break;
		case I32TX_MONI:
			WriteMoniLimit();
			break;	
		case I32TX_ERROR:
			WriteError();
			break;
		case I32TX_CORE:
			WriteCore();
			break;
		default:;
	}
	return 0;

}

/*--------------------------------------------------------------------------+
|  Function :  SetTemp()                                  				|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::WriteValue(char *pszID)
{
	int		nID, i;
	DWORD 	*pData;
	
	if(pszID == NULL)	return -1;
	nID	= g_pDatabase->GetIndex(Trim(pszID));
	if(nID < 0)	return -1;
		
	for(i = 0; i < SETDATA_COUNT; i ++)
		if(nID == m_nSetDataIDList[i]) break;
	if(i == SETDATA_COUNT)	return WriteCore(Trim(pszID));
	
	pData	= &m_SetData.dwAutoLubraShotCount1;
	
	*(pData + i)	= g_pDatabase->Read(nID).dwData;
//	printf("pID=%s i=%d value=%ld\n", pszID, i, *(pData + i));
	return Write(I32TX_SET, sizeof(I32_SETDATA), &m_SetData);
}

/*--------------------------------------------------------------------------+
|  Function :  SetTemp()                                  				|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::SetTemp()
{
	int		i;
	I32_SETTEMP SetTemp;
	
	Sleep(5);
	for(i = 0; i < TEMP_COUNT-11; i ++)
	{
		SetTemp.nIndex		= i;
		SetTemp.nSetting	= g_pDatabase->Read(m_SetTempID[i].nSetting).lData;
		SetTemp.nRamp		= g_pDatabase->Read(m_SetTempID[i].nRamp).lData;
//		SetTemp.nMax		= g_pDatabase->Read(m_SetTempID[i].nMax).lData;
//		SetTemp.nMin		= g_pDatabase->Read(m_SetTempID[i].nMin).lData;
		SetTemp.nMaxOffset	= g_pDatabase->Read(m_SetTempID[i].nMax).lData;
		SetTemp.nMinOffset	= g_pDatabase->Read(m_SetTempID[i].nMin).lData;
		SetTemp.wKP			= g_pDatabase->Read(m_SetTempID[i].nKP).wData;
		SetTemp.wKI			= g_pDatabase->Read(m_SetTempID[i].nKI).wData;
		SetTemp.wKD			= g_pDatabase->Read(m_SetTempID[i].nKD).wData;
		SetTemp.wDT			= g_pDatabase->Read(m_SetTempID[i].nDT).wData;
		SetTemp.wTD			= g_pDatabase->Read(m_SetTempID[i].nTD).wData;
		SetTemp.nAccSet		= g_pDatabase->Read(m_SetTempID[i].nAccSet).lData;
		////printf("SetTemp i=%d, nSetting=%d\n", i, SetTemp.nSetting);
		Sleep(1);
		Write(I32TX_TEMP, sizeof(SetTemp), &SetTemp);
	}
	SetDBValue(TECH_TEMP_24HOUR_FLAGHEATER, 0);
	DBVALUE	Data;
	DWORD	*pData	= &m_SetData.dwAutoLubraShotCount1;
	for(int i = 0; i < SETDATA_COUNT; i ++)
	{
		Data	= g_pDatabase->Read(m_pszSetDataIDList[i]);
		m_nSetDataIDList[i]	= Data.dwIndex;
		*(pData + i)		= Data.dwData;
		//printf("index = %d, data = %d\n", i, Data.dwData);
	}
	Write(I32TX_SET, sizeof(m_SetData), &m_SetData);
	Write(I32TX_STATE, 0, NULL);
	WriteCore();				//<<yang 2006/10/20 
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  I32_Status()                                  				|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskI32::GetStatus()
{	
	return m_nID;
}

int		CTaskI32::WriteState()				//yang 2006/10/12 
{
	I32_TXDATA	TxData;
	char	szID[100];
	
	memset(&TxData, 0, sizeof(I32_TXDATA));
	TxData.nType					= I32TX_STATE;
	TxData.State.wOperationMode		= g_pDatabase->Read(TECH_INTERFACE_OP_MODE).dwData;
	TxData.State.wOperationStep		= g_pDatabase->Read(TECH_INTERFACE_OP_STEP).dwData;
	TxData.State.wOperationStep2	= g_pDatabase->Read(TECH_INTERFACE_OP_STEP2).dwData;					//yang 2006/10/13 
	TxData.State.flMotorStatus		= g_pDatabase->Read(TECH_INTERFACE_MOTOR_STATUS).dwData;
	TxData.State.flHeaterStatus		= g_pDatabase->Read(TECH_TEMP_INTERFACE_HEATER_STATUS).dwData;
	TxData.State.tmActing			= g_pDatabase->Read(TECH_INTERFACE_ACTION_TIME).dwData;
	TxData.State.tCycleTime			= g_pDatabase->Read(TECH_INTERFACE_CYCLE_TIME).dwData;
	TxData.State.tCurrentShotCount	= g_pDatabase->Read(TECH_INTERFACE_SHOTCOUNT).dwData;
	TxData.State.wAlarmStatus		= g_pDatabase->Read(TECH_ALM_GENER_ALARMSTATUS).wData;
	TxData.State.wInOperation		= g_pDatabase->Read(TECH_INTERFACE_INOPERATION).wData;
	TxData.State.dwAutoLub1Counter	= g_pDatabase->Read(TECH_LUB_INTERFACE_RESPONSEIOCOUNT).dwData;			// 自動潤滑1計數//yang 2006/10/13 
	TxData.State.dwAutoLub2Counter	= g_pDatabase->Read(TECH_LUB_INTERFACE_RESPONSEIOCOUNT2).dwData;		// 自動潤滑2計數//yang 2006/10/13 
	TxData.State.dwAutoLub1ShotCounter	= g_pDatabase->Read(TECH_LUB_INTERFACE_RESPONSEIOSHOTCOUNT).dwData;
	TxData.State.dwAutoLub2ShotCounter	= g_pDatabase->Read(TECH_LUB_INTERFACE_RESPONSEIOSHOTCOUNT2).dwData;
	TxData.State.dwHMIAdjCounter	= g_pDatabase->Read(TECH_MDH_INTERFACE_COUNTER).dwData;
	for(int i = 1; i <= 8; i ++)
	{
		sprintf(szID, TECH_INTERFACE_INPUT_, i);
		TxData.State.awInPortValue[i - 1] = g_pDatabase->Read(szID).wData;
		sprintf(szID, TECH_INTERFACE_OUTPUT_, i);
		TxData.State.awOutPortValue[i - 1]= g_pDatabase->Read(szID).wData;
	}
	TxData.State.dwEthernetState = 1;
	TxData.State.dwTestPanel = m_wTestPanel;
	SendData(m_nID, (char *)&TxData, TxData.nType);
	
	return 0;
}

int		CTaskI32::WriteCore()
{
	I32_TXDATA	TxData;
	double*	pData = &TxData.CoreData.dwEject_TravelPosition;
	
	memset(&TxData, 0, sizeof(I32_COREDATA));
	TxData.nType	= I32TX_CORE;
	for (int i=0; i < SETCORE_COUNT; i++)
		{
		*pData = g_pDatabase->Read(m_nSetCoreIDList[i]).dData;
		//printf("Value[%d]=%f \n", i, *pData);
		pData++;
		}
	SendData(m_nID, (char *)&TxData, TxData.nType);
	return 0;
}

int		CTaskI32::WriteCore(char* pszID)
{
	int		nID, i;
	double 	*pData;
	
	if(pszID == NULL)	return -1;
	nID	= g_pDatabase->GetIndex(Trim(pszID));
	//nID	= g_pDatabase->GetIndex(pszID);
	if(nID < 0)	return -1;
		
	for(i = 0; i < SETCORE_COUNT; i ++)
		if(nID == m_nSetCoreIDList[i]) break;
	if(i == SETCORE_COUNT)	return -1;
	
	pData	= &m_CoreData.dwEject_TravelPosition;
	
	*(pData + i)	= g_pDatabase->Read(nID).dData;
	//printf("pID=%s i=%d value=%lf\n", pszID, i, *(pData + i));
	return Write(I32TX_CORE, sizeof(I32_COREDATA), &m_CoreData);
	//return 0;
}

void	CTaskI32::TestPanel(WORD wValue)						//<<yang 2006/10/31 for test panel
{
	m_wTestPanel = wValue;
	WriteState();
}
