/*==========================================================================+
|  Class    : Simotion communication          								|
|  Task     : Simotion communication file                     				|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2005/12/23                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#include	"main.h"
#include	"tasksiem.h"
#include	"utils.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/

const 	BYTE	HEADER_REQ[] 		= {0xA0, 0xB0}; 	// header request
const 	BYTE	HEADER_RES[] 		= {0xA1, 0xB1}; 	// header response
const 	BYTE	FOOTER_REQ[] 		= {0xC0, 0xD0}; 	// footer request
const	BYTE	FOOTER_RES[] 		= {0xC1, 0xD1}; 	// footer response

#define 	_UNKNOWN 	0
#define 	_BOOL 		1
#define 	_BYTE 		2
#define 	_WORD 		3
#define 	_DWORD 		4
#define 	_SINT 		11
#define 	_USINT 		12
#define 	_INT 		13
#define 	_UINT 		14
#define 	_DINT 		15
#define 	_UDINT 		16
#define 	_REAL 		21
#define 	_LREAL 		22
#define 	_TIME 		31
#define 	_DATE 		32
#define 	_TOD 		33
#define 	_DT 		34

#define		PACKAGE_LENGTH			1514
#define		SIEM_UPDATE_COUNTER		100
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
BYTE	g_RxBuffer[4096];
BYTE	g_TxBuffer[4096];

extern	WORD	g_wTaskCommStat;
int				g_nFOState[SIMD_FO_COUNT];
int				g_nFOCommand[SIMD_FO_COUNT];
ETHERNET	CTaskSiem::m_ethernet	=
{
	169 | 254  << 8 | 11 << 16 | 21 << 24,			//LEO20070627
	169 | 254 << 8 | 11 << 16 | 22 << 24	,		// ip addr
	//60 | 248  << 8 | 175 << 16 | 38 << 24,
	0				,		// src port
	2301			,		// dst port
	AF_INET			,		// ipv4
	SOCK_STREAM		,		// TCP SOCK_STREAM
	4096			,
	g_RxBuffer		,
	4096			,
	g_TxBuffer		,
	NULL
};

char*		g_apcActualID[] =
{
	EJT_INTERFACE_POSNACTUAL,						//72000020,
	EJT_INTERFACE_VELACTUAL,						//72000021,
	RCV_INTERFACE_POSNACTUALSCREW,					//32000020,
	RCV_INTERFACE_RPMACTUAL,					 	//32000021,
	RCV_INTERFACE_PRESACTUAL,						//32000025,
	RCV_INTERFACE_POSNACTUALSHOTSIZE,				//32000027,
	RCV_INTERFACE_TORQACTUAL,						//32000026,
	RCV_INTERFACE_FORCEACTUAL,						//32000022,
	RCV_INTERFACE_PRESMAX,							//32000071,
	RCV_INTERFACE_WORK,								//32000070,
	INJ_INTERFACE_POSNACTUAL,						//52000020,
	INJ_INTERFACE_VELACTUAL,						//52000021,
	INJ_INTERFACE_WORK,								//52000070,
	INJ_INTERFACE_FORCEACTUAL,						//52000022,
	INJ_INTERFACE_PRESMAXPRESSURE,					//52000071,
	INJ_INTERFACE_POSNTRANSITION,					//52000074,
	INJ_INTERFACE_VELTRANSITION,					//52000073,
	INJ_INTERFACE_PRESTRANSITION,					//52000072,
	INJ_INTERFACE_TMTRANSITION,						//52000075,
	INJ_INTERFACE_FLAGCUSHIONRESULT,				//52000078,
	INJ_INTERFACE_POSNCUSHION,						//52000076,
	INJ_INTERFACE_POSNCUSHIONCORRECTIONVALUE,		//52000077,    
	MLD_INTERFACE_AXIS_POSITIONINGSTATE_POSNACTUAL,	//62000023
	MLD_INTERFACE_POSNACTUAL,						//62000020
	MLD_INTERFACE_VELACTUAL,						//62000021
	MLD_INTERFACE_FORCEACTUALSENSOR,				//62000028
	MLD_INTERFACE_FORCEACTUALDRIVE,					//62000022
	MDH_INTERFACE_POSNSETPOINT,						//82000040
	IJU_INTERFACE_POSNACTUAL,						//12000020,
	IJU_INTERFACE_VELACTUAL,						//12000021,
	IJU_INTERFACE_FORCEACTUAL,						//12000022
	INJ_AXIS_FORCESTATEDATA_FORCECOMMANDVALUE,			
	INJ_AXIS_FORCECONTROLLERDATA_FORCETOTALCOMMANDVALUE,
	INJ_AXIS_DATASETMONITORING_FORCEACTUALDATASET,
	MLD_INTERFACE_TORQUEMAXDRIVECLAMPUP,
	MLD_INTERFACE_TORQUEACTUALDRIVE
};
	
char *	g_asRealDataID[] =
{
	MLD_INTERFACE_POSNACTUAL,						// 開關模位置
	EJT_INTERFACE_POSNACTUAL,						// 脫模位置
	INJ_INTERFACE_POSNACTUAL,						// 射出位置
	IJU_INTERFACE_POSNACTUAL,						// 座台位置
	RCV_INTERFACE_POSNACTUALSCREW,					// 儲料位置
	MLD_INTERFACE_FORCEACTUALSENSOR,				// 鎖模力 from Senser
	MLD_INTERFACE_TORQUEMAXDRIVECLAMPUP,			// 鎖模力 from Torque	//<<yang 2006/11/3 
	INJ_INTERFACE_PRESTRANSITION,					// 保壓轉換壓力,
	INJ_INTERFACE_TMTRANSITION,						// 保壓轉換時間
	INJ_INTERFACE_POSNTRANSITION,					// 保壓轉換位置
/*
	MLD_INTERFACE_FLAGPOSITIONINGSTATE,				// 開關模位置狀態
	EJT_INTERFACE_FLAGPOSITIONINGSTATE,				// 脫模位置狀態
	INJ_INTERFACE_FLAGPOSITIONINGSTATE,				// 射出位置狀態
	IJU_INTERFACE_FLAGPOSITIONINGSTATE,				// 座台位置狀態
	RCV_INTERFACE_FLAGPOSITIONINGSTATE,				// 儲料位置狀態
	PSTDCP_INTERFACE_FLAGPOSITIONINGSTATE*/
};

CONFIGDATA	g_aConfigData[] = {
	{MLD_PROF_VELOCITYOPEN_CNNOPROFILEPOINTS,		5},	//60200002//mold.profilepoints.velocityprofileopen.numberofprofilepoints 5
	{MLD_PROF_VELOCITYCLOSE_CNNOPROFILEPOINTS,	5},		//60210002//mold.profilepoints.velocityprofileclose.numberofprofilepoints 5
	{RCV_PROF_SPEED_CNNOPROFILEPOINTS,			3},		//30200002//recovery.profilepoints.speedprofile.numberofprofilepoints 3
	{RCV_PROF_PRESSURE_CNNOPROFILEPOINTS,			3},
	{INJ_PROF_INJVELOCITY_CNNOPROFILEPOINTS,		6},	//50200002//injection.profilepoints.injectionvelocityprofile.numberofprofilepoints 6
	{INJ_PROF_INJPRESSURELIMIT_CNNOPROFILEPOINTS,	6},	//50210002
	{INJ_PROF_HOLDPRESSURE_CNNOPROFILEPOINTS,		5},	//50220002//injection.profilepoints.holdpressureprofile.numberofprofilepoints 5
	{EJT_PROF_VELOCITYFWD_CNNOPROFILEPOINTS,		2},	//70100002//ejector.velocityprofile.forward.numberofprofilepoints 2
	{EJT_PROF_VELOCITYBWD_CNNOPROFILEPOINTS,		2},	//70110002//ejector.velocityprofile.backward.numberofprofilepoints 2
	{NULL}
	};
	
CONFIGDATA g_aCurveData[] =
{
	{MLD_TRACE_ACTUALFORCE_POSNPRETRIGGERDELAY,					0},//60320005
	{MLD_TRACE_ACTUALFORCE_FLAGRECORDINGMODE,					1},//60320001
	{MLD_TRACE_ACTUALVELOCITY_POSNPRETRIGGERDELAY,				0},//60300005
	{MLD_TRACE_ACTUALVELOCITY_FLAGRECORDINGMODE,				1},//60300001
	{MLD_TRACE_FORCELIMIT_POSNPRETRIGGERDELAY,					0},//60330005
	{MLD_TRACE_FORCELIMIT_FLAGRECORDINGMODE,					1},//60330001
	{MLD_TRACE_VELOCITYSETPOINT_POSNPRETRIGGERDELAY,			0},//60310005
	{MLD_TRACE_VELOCITYSETPOINT_FLAGRECORDINGMODE,				1},//60310001
	{RCV_TRACE_ACTUALPRESSURE_POSNPRETRIGGERDELAY,				0},//30320005
	{RCV_TRACE_ACTUALPRESSURE_FLAGRECORDINGMODE,				1},//30320001
	{RCV_TRACE_ACTUALSPEED_POSNPRETRIGGERDELAY,					0},//30300005
	{RCV_TRACE_ACTUALSPEED_FLAGRECORDINGMODE,					1},//30300001
	{RCV_TRACE_PRESSURESETPOINT_POSNPRETRIGGERDELAY,			0},//30330005
	{RCV_TRACE_PRESSURESETPOINT_FLAGRECORDINGMODE,				1},//30330001
	{RCV_TRACE_SPEEDSETPOINT_POSNPRETRIGGERDELAY,				0},//30310005
	{RCV_TRACE_SPEEDSETPOINT_FLAGRECORDINGMODE,					1},//30310001
	{INJ_TRACE_ACTUALHOLDPRESSURE_TMPRETRIGGERDELAY,			0},//50350005
	{INJ_TRACE_ACTUALHOLDPRESSURE_FLAGRECORDINGMODE,			1},//50350001
	{INJ_TRACE_ACTUALINJPRESSURE_POSNPRETRIGGERDELAY,			0},//50330005
	{INJ_TRACE_ACTUALINJPRESSURE_FLAGRECORDINGMODE,				1},//50330001
	{INJ_TRACE_ACTUALINJVELOCITY_POSNPRETRIGGERDELAY,			0},//50310005
	{INJ_TRACE_ACTUALINJVELOCITY_FLAGRECORDINGMODE,				1},//50310001
	{INJ_TRACE_ACTUALTRANSITIONPOSITION_TMPRETRIGGERDELAY,		0},//50380005
	{INJ_TRACE_ACTUALTRANSITIONPOSITION_FLAGRECORDINGMODE,		1},//50380001
	{INJ_TRACE_ACTUALTRANSITIONPRESSURE_TMPRETRIGGERDELAY,		0},//50360005
	{INJ_TRACE_ACTUALTRANSITIONPRESSURE_FLAGRECORDINGMODE,		1},//50360001
	{INJ_TRACE_ACTUALTRANSITIONVELOCITY_TMPRETRIGGERDELAY,		0},//50370005
	{INJ_TRACE_ACTUALTRANSITIONVELOCITY_FLAGRECORDINGMODE,		1},//50370001
	{INJ_TRACE_HOLDPRESSURESETPOINT_TMPRETRIGGERDELAY,			0},//50340005
	{INJ_TRACE_HOLDPRESSURESETPOINT_FLAGRECORDINGMODE,			1},//50340001
	{INJ_TRACE_INJPRESSURELIMITSETPOINT_POSNPRETRIGGERDELAY,	0},//50320005
	{INJ_TRACE_INJPRESSURELIMITSETPOINT_FLAGRECORDINGMODE,		1},//50320001
	{INJ_TRACE_INTERPOLATEDVELOCITYSETPOINT_POSNPRETRIGGERDELAY,0},//50300005
	{INJ_TRACE_INTERPOLATEDVELOCITYSETPOINT_FLAGRECORDINGMODE,	1},//50300001
	{NULL, 0}
};

char*	g_pszFOStartupID[] = {
		DIAGNOSTIC_STARTUPTASKDONE,	//400010003
		DIAGNOSTIC_DEFAULTTASKDONE,	//400010008
		DIAGNOSTIC_RESTARTTASKDONE	//400010013
	};
//CtmRecordFile		g_ErrorFile;
//=======
//       Initialize static class member
//======= 
int			CTaskSiem::m_nValueID	= -1;
int			CTaskSiem::m_nFOID		= -1;
int			CTaskSiem::m_nMsgID		= -1;
int			CTaskSiem::m_nCurveID	= -1;
int			CTaskSiem::m_nParaID	= -1;
int			CTaskSiem::m_nRecipeID	= -1;
int			CTaskSiem::m_nConnecting= 0;					// 正在連接
pthread_t	CTaskSiem::m_ThreadID	= 0;
DBCURVE		CTaskSiem::m_Curve[CURVE_NUMBER+10];
SIMD_VALUE	CTaskSiem::m_asReqValue[MAX_SIMD_REQCOUNT];              /* storage for user input */
int			CTaskSiem::m_nCurveState = 0;					// 表格接收狀態
long		CTaskSiem::m_lTimerCount = 0;					// 實際值定時
//SIMD_MSG	CTaskSiem::m_Msg;
SIMD_PARA	CTaskSiem::m_Para;
//pthread_mutex_t	CTaskSiem::mutex_Msg;					// 消息進程鎖
/*==== =======================================================================+
|            Function implementation                                          |
+===== ======================================================================*/
       
/*==== =======================================================================+
|            Class implementation - TaskComm                                  |
+===== ======================================================================*/
/*---- -----------------------------------------------------------------------+
|            Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskSiem::CTaskSiem() : CTaskComm()
{	
	m_lUpdateCounter	= SIEM_UPDATE_COUNTER;
	m_lTimerCount		= 1000;
	m_nValueID			= 
	m_nFOID				= 
	m_nMsgID			= 
	m_nCurveID			= 
	m_nRecipeID			=
	m_nParaID			= -1;
	
	m_nConnecting		= 					// 正在連接
	m_nClosedError		= 0;				// 關閉錯誤線程標識
	RegisterMessage(MSG_SIEM, m_idMe);
	AddCounter(&m_lUpdateCounter, m_idMe);
	AddCounter(&m_lTimerCount, m_idMe);
//	pthread_mutex_init(&mutex_Msg, NULL);
	
	//========
	// 打開錯誤保存文件
	//=======
/*	g_ErrorFile.Open("error.cdb", sizeof(SIMD_MSG));
	if(g_ErrorFile.ReadRecord(1, &m_Msg) == tmFILE_BAD)
	{		
		memset(&m_Msg, 0, sizeof(SIMD_MSG));
		g_ErrorFile.WriteRecord(1, &m_Msg);
	}
		*/
	//=======
	// 初始化曲線列表
	//=======
	CtmRecordFile	CurveFile;
	CurveFile.Open("curve.cdb", sizeof(DBCURVE));
	for(int i = 0; i < CURVE_NUMBER + 10; i ++)
	{
		if(CurveFile.ReadRecord(i + 1, &m_Curve[i]) != tmFILE_BAD &&
			m_Curve[i].nIndex == i + 1)
			continue;
			
		m_Curve[i].nIndex	 	= i + 1;
		m_Curve[i].nCount		= 0;
		CurveFile.WriteRecord(i + 1, &m_Curve[i]);		
	}	
	CurveFile.Close();

	pthread_create(&m_ThreadID, NULL, (PRSRUN)_Open, NULL);
}

CTaskSiem::~CTaskSiem()
{
	StopActualData();																		//<<yang 2006/2/27 
	DelDevice(m_nMsgID);
	DelDevice(m_nCurveID);
	DelDevice(m_nFOID);
	DelDevice(m_nValueID);
	DelDevice(m_nParaID);
	DelDevice(m_nRecipeID);
//	pthread_mutex_destroy(&mutex_Msg);
//	g_ErrorFile.Close();
	
	DeleteCounter(&m_lTimerCount, m_idMe);
	DeleteCounter(&m_lUpdateCounter, m_idMe);
	UnRegisterAllMessage(MSG_SIEM);
}

void	CTaskSiem::Run()
{
	if (m_lUpdateCounter < 0)
		{
		if(	GetState(m_nMsgID) 		!= DEVICE_S_OPENED	||
			GetState(m_nCurveID) 	!= DEVICE_S_OPENED	||
			GetState(m_nFOID) 		!= DEVICE_S_OPENED	||
			GetState(m_nValueID) 	!= DEVICE_S_OPENED	||
			GetState(m_nParaID) 	!= DEVICE_S_OPENED	||
			GetState(m_nRecipeID)	!= DEVICE_S_OPENED)
				g_wTaskCommStat = 0;
		else	g_wTaskCommStat = 1;
		m_lUpdateCounter = SIEM_UPDATE_COUNTER;
		if(m_nConnecting == 2)
		{
			if(	GetState(m_nMsgID) 		!= DEVICE_S_OPENED	||
				GetState(m_nCurveID) 	!= DEVICE_S_OPENED	||
				GetState(m_nFOID) 		!= DEVICE_S_OPENED	||
				GetState(m_nValueID) 	!= DEVICE_S_OPENED	||
				GetState(m_nParaID) 	!= DEVICE_S_OPENED	||
				GetState(m_nRecipeID)	!= DEVICE_S_OPENED)
			{
				//printf("Restart\n");
				m_nConnecting = 0;
				pthread_create(&m_ThreadID, NULL, (PRSRUN)_Open, NULL);
			}
		}
		////printf("value=%d FO=%d Curve=%d Msg=%d Para=%d Recipe=%d Connect=%d\n", 
		//	m_nValueID, m_nFOID, m_nCurveID, m_nMsgID, m_nParaID, m_nRecipeID, m_nConnecting);
			
		if (g_ptaskCmd->GetFocus() != NULL)
			g_ptaskCmd->GetFocus()->Update();
		}
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_Reset(int nIndex)										|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::Reset(int *pnIndex)
{
//	//printf("Reset \n");
	int	nCount = 0;
	while(ResetDevice(*pnIndex) == -1 && nCount ++ < 10);
	
	if(nCount >= 10) return -1;
	
	if(*pnIndex == m_nValueID)
		GetActualData();
		
	return *pnIndex;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_Req(int nCount, int nType, int nCycle, UINT32 *pIDList)	|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::ReqValues(int nCount, int nType, int nCycle, UINT32 *pIDList)
{
	int 		i;
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)acSendBuffer;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + nCount * sizeof(SIMD_VALUE)];
	
	if(m_nValueID == -1)	return -1;
	if(nCount > MAX_SIMD_REQCOUNT || nCount <= 0)
		return -1;
	if(pIDList	== NULL)	return -1;
		
	memset(&m_asReqValue, 0, MAX_SIMD_REQCOUNT * sizeof(SIMD_VALUE));
		
	for(i = 0; i < nCount; i ++)
		//m_asReqValue[i].dwID 	= IndexToID(pIDList[i] + 1);
		m_asReqValue[i].dwID 	= pIDList[i];
		
    memcpy(&acSendBuffer[sizeof(SIMDHEADER)], m_asReqValue, nCount * sizeof(SIMD_VALUE));
    
	//=======
	// fill telegram header
	//======= 
	pHeader->acHeader[0]	= HEADER_REQ[0];
	pHeader->acHeader[1]	= HEADER_REQ[1];
	pHeader->wSize 			= sizeof(SIMDHEADER) + nCount * sizeof(SIMD_VALUE) + sizeof(SIMDFOOTER);
	pHeader->wType 			= nType;
	pHeader->wCounter 		= ++ nSingleCount;
	pHeader->wCycle 		= nCycle;
	pHeader->wNoElements 	= nCount;

	//=======
	// fill and copy telegram footer
	//=======
	pFooter->acFooter[0]	= FOOTER_REQ[0];
	pFooter->acFooter[1]	= FOOTER_REQ[1];
	
	SendData(m_nValueID, acSendBuffer, pHeader->wSize);
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_Req(int nCount, int nType, int nCycle, char **pszIDList)|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::ReqValues(int nCount, int nType, int nCycle, char **pszIDList)
{
	int		i, j;
	int		nIndex;
	UINT32 *pIDList	= (UINT32 *)malloc(nCount * sizeof(UINT32));
	
	j = 0;
	for(i = 0; i < nCount; i ++)
	{
		//printf("str[%d] = %s\n", i, pszIDList[i]);
		nIndex	= g_pDatabase->GetIndex(Trim(pszIDList[i]));
		if (g_pDatabase->Read(nIndex).dwDataPath != DATA_PATH_ELECTRIC)
			continue;
		pIDList[j++]	= (UINT32)g_pDatabase->IndexToDataID(nIndex);
		//printf("ID=%ld\n", pIDList[j-1]);
	}
	
	if (j > 0) ReqValues(j, nType, nCycle, pIDList);
	
	free(pIDList);
	return 0;
}

int	CTaskSiem::QuickValue(SIMD_VALUE	*pValue, int nCount)
{
	int		i;
//	I32_REALDATA	RealData;
//
//	double			*pDouble	= &RealData.dMoldPos;
///*	
//	static int naCount = 0;
//	if(naCount ++ > 100)
//	{
//		//printf("SIMD_QuickValue nCount =%d\n", nCount);
//		naCount = 0;
//	}*/
//	if(pValue == NULL || nCount != sizeof(g_asRealDataID)/sizeof(char *)) return -1;
//		
//	
//	for(i = 0; i < sizeof(g_asRealDataID)/sizeof(char *); i ++)
//	{
//		*pDouble	= pValue->dData;
//		pValue	++;
//		pDouble	++;
//	}
//	return	g_ptaski32->Write(I32TX_REAL, sizeof(I32_REALDATA), &RealData);
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_ResValueFunc(char *pData, int nLength)					|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CTaskSiem::ResValueFunc(char *pData, int nLength)
{
	int		i;
	MSG		msg;
//	I32_REALDATA	RealData;
//	//printf("TaskSiem::ResValueFunc() \n");
	static int nValueCount = 0;
	if(pData == NULL || (unsigned int)nLength < sizeof(SIMDHEADER) +sizeof(SIMDFOOTER))
	{
		if(nValueCount >= 0 && nValueCount ++ > 1000)
		{
			pthread_create(&m_ThreadID, NULL, (PRSRUN)Reset, &m_nValueID);
			nValueCount = -40000;
		}
		return;
	}
	nValueCount 	= 0;
	
	if(m_lTimerCount <= 0)
		GetActualData();
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)pData;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&pData[nLength - 2];
	SIMD_VALUE	*pValue		= (SIMD_VALUE *)(pData + sizeof(SIMDHEADER));
	DBVALUE		data;

	//=======
	// 檢驗數據正確性
	//=======
	if(memcmp(pHeader, HEADER_RES, 2) != 0)	return;
	if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;	
	if(pHeader->wSize != nLength)			return;		
	if((sizeof(SIMDHEADER) + pHeader->wNoElements * sizeof(SIMD_VALUE) + sizeof(SIMDFOOTER)) != (unsigned int)nLength) return;

	switch(pHeader->wType)
	{
		case SIMD_RESBASE + SIMD_ACTUAL:
			QuickValue(pValue, pHeader->wNoElements);	
			return;
		case SIMD_RESBASE + SIMD_REFRENCE:
			m_lTimerCount	= 1000;
		case SIMD_RESBASE + SIMD_SINGLE:
		case SIMD_RESBASE + SIMD_WRITE:
			break;
		default:
			return;
			break;
	}
	
	memset(&data, 0, sizeof(DBVALUE));
	
//	//printf("pHeader->wNoElements=%d \n", pHeader->wNoElements);
	for(i = 0; i < pHeader->wNoElements; i ++)
	{
		data.dwIndex	= (unsigned int)g_pDatabase->DataIDToIndex(pValue->dwID);
		if(pHeader->wType == 104)
		{
//			//printf("ucsate=%02x ", pValue->ucState);
			ReqValues(1, 3, 0, &pValue->dwID);
			return ;
		}

		switch(pValue->ucDataType)
		{
		case _BOOL:
//			//printf("type is BOOL , value: %u\n", pValue->ucData);
			data.DataType.wType		= TYPE_BOOL;
			data.DataType.wLength	= sizeof(BOOL);
			data.wData				= pValue->wData;
			break;
		case _BYTE:
		case _USINT:
			data.DataType.wType		= TYPE_BYTE;
			data.DataType.wLength	= sizeof(BYTE);
			data.ucData				= pValue->ucData;
//			//printf("type is BYTE, value: %u\n", pValue->ucData);
			break;
		case _WORD:
			data.DataType.wType		= TYPE_WORD;
			data.DataType.wLength	= sizeof(WORD);
			data.wData				= pValue->wData;
//			//printf("type is WORD, value: %u\n", pValue->wData);
			break;
		case _DWORD:
		case _UDINT:
			data.DataType.wType		= TYPE_DWORD;
			data.DataType.wLength	= sizeof(DWORD);
			data.dwData				= pValue->dwData;
			break;
		case _UINT:
			data.DataType.wType		= TYPE_WORD;
			data.DataType.wLength	= sizeof(WORD);
			data.wData				= pValue->wData;
//			//printf("type is DWORD, value: %lu\n", pValue->dwData);
			break;
		case _SINT:
			data.DataType.wType		= TYPE_CHAR;
			data.DataType.wLength	= sizeof(char);
			data.cData				= pValue->cData;
//			//printf("type is char, value: %d\n", pValue->cData);
			break;
		case _INT:
			data.DataType.wType		= TYPE_SHORT;
			data.DataType.wLength	= sizeof(short);
			data.nData				= pValue->nData;
//			//printf("type is Short, value=%ld, sim=%d, tech=%d\n", pValue->lData, pValue->dwID, data.dwIndex);
			break;
		case _DINT:
			data.DataType.wType		= TYPE_INT;
			data.DataType.wLength	= sizeof(int);
			data.lData				= pValue->lData;
//			//printf("type is INT, value=%ld, sim=%d, tech=%d\n", pValue->lData, pValue->dwID, data.dwIndex);
			break;
		case _REAL:
			data.DataType.wType		= TYPE_FLOAT;
			data.DataType.wLength	= sizeof(float);
			data.fData				= pValue->fData;
//			//printf("type is REAL, value: %f\n", pValue->fData);
			break;
		case _LREAL:
			data.DataType.wType		= TYPE_DOUBLE;
			data.DataType.wLength	= sizeof(double);
			data.dData				= pValue->dData;
//			//printf("type is DOUBLE, value: %lf index=%ld\n", pValue->dData, data.dwIndex);
			break;
		case _TIME:
		case _DATE:
		case _TOD:
		case _DT:
//			//printf(" data type not implemeted yet\n");
			break;
		case _UNKNOWN:
		default:
			data.DataType.wType		= TYPE_UNKNOWN;
//			//printf("type is error index=%d\n", i + 1);
			break; 
		}
//		//printf("Before db write\n");
		g_pDatabase->Write(&data);
		pValue ++;
	}
	msg.message	= MSG_SIEM;
	g_pApplication->QueueMessage(&msg);
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_WriteValue(int nCount, DBVALUE *pData)					|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::WriteValue(int nCount, DBVALUE *pData)
{
	int			i;
	char 		acSendBuffer[1414];
	SIMD_VALUE *pSimData	= (SIMD_VALUE *)acSendBuffer;
	DBVALUE		*pTechData	= pData;
	
	if(nCount > 100)		return -1;	
	for(i = 0; i < nCount; i ++)
	{
		pSimData->dwID	= g_pDatabase->IndexToDataID(pTechData->dwIndex);
		//printf("Count=%d pSimData->ID=%ld\n", nCount, pSimData->dwID);
		memcpy(pSimData->acData, pTechData->acData, 8);
		switch(pTechData->DataType.wType)
		{
			case TYPE_BOOL:
				pSimData->ucDataType	= _BOOL;
				break;
			case TYPE_BYTE:
				pSimData->ucDataType	= _USINT;
				break;
			case TYPE_WORD:
				pSimData->ucDataType	= _UINT;
				break;
			case TYPE_DWORD:
				pSimData->ucDataType	= _UDINT;
//				//printf("nCount=%d pSimData->dwID=%ld pSimData->dwData=%ld\n", nCount, pSimData->dwID, pSimData->dwData);
				break;
			case TYPE_CHAR:
				pSimData->ucDataType	= _SINT;
				break;
			case TYPE_SHORT:
				pSimData->ucDataType	= _INT;
				//printf("nCount=%d pSimData->dwID=%ld pSimData->nData=%d\n", nCount, pSimData->dwID, pSimData->nData);
				break;
			case TYPE_INT:
				pSimData->ucDataType	= _DINT;
				//printf("nCount=%d pSimData->dwID=%ld pSimData->lData=%ld\n", nCount, pSimData->dwID, pSimData->lData);
				break;
			case TYPE_FLOAT:
				pSimData->ucDataType	= _REAL;
				break;
			case TYPE_DOUBLE:
				pSimData->ucDataType	= _LREAL;
//				//printf("nCount=%d pSimData->dwID=%ld pSimData->dData=%f\n", nCount, pSimData->dwID, pSimData->dData);
				break;
			case TYPE_STRING:
				break;
			case TYPE_UNKNOWN:
			default:
				pSimData->ucDataType	= _UNKNOWN;
				break;
		};
		
		pSimData ++;
		pTechData ++;
	}

	return WriteValue(nCount, 4, (SIMD_VALUE *)acSendBuffer);
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_WriteValue(int nCount, int nType, SIMD_VALUE *pData)	|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::WriteValue(int nCount, int nType, SIMD_VALUE *pData)
{
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)acSendBuffer;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + nCount * sizeof(SIMD_VALUE)];
	SIMD_VALUE	*pValue		= (SIMD_VALUE *)(acSendBuffer + sizeof(SIMDHEADER));
	
	if(m_nValueID == -1)	return -1;
	if(nCount > MAX_SIMD_REQCOUNT || nCount <= 0)
		return -1;
	if(pData	== NULL)	return -1;
		
	memset(&acSendBuffer, 0, PACKAGE_LENGTH);
		
    memcpy(&acSendBuffer[sizeof(SIMDHEADER)], pData, nCount * sizeof(SIMD_VALUE));
    for(int i = 0; i < nCount; i ++)
    {
    	pValue->ucMode	= 3;
    	pValue ++;
    }
    
	//=======
	// fill telegram header
	//======= 
	pHeader->acHeader[0]	= HEADER_REQ[0];
	pHeader->acHeader[1]	= HEADER_REQ[1];
	pHeader->wSize 			= sizeof(SIMDHEADER) + nCount * sizeof(SIMD_VALUE) + sizeof(SIMDFOOTER);
	pHeader->wType 			= nType;
	pHeader->wCounter 		= ++ nSingleCount;
	pHeader->wCycle 		= 0;
	pHeader->wNoElements 	= nCount;

	//=======
	// fill and copy telegram footer
	//=======
	pFooter->acFooter[0]	= FOOTER_REQ[0];
	pFooter->acFooter[1]	= FOOTER_REQ[1];
	
	SendData(m_nValueID, acSendBuffer, pHeader->wSize);
	
	return nSingleCount;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_ExeFO(int nCount, int nType, UINT32 *pIDList)			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::ExeFO(int nCount, int nType, void *pData)
{
	int 		i;
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;
	
	SIMD_FO		*pValue		= (SIMD_FO *)pData;
	
	static int	nCommand[SIMD_FO_COUNT];
	for(i = 0; i < SIMD_FO_COUNT; i ++)
	{
		
		g_nFOCommand[i]	= pValue->nCommand;
		pValue ++;
	}
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)acSendBuffer;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + nCount * sizeof(SIMD_FO)];
	
	if(m_nFOID == -1)		return -1;
	if(nCount > MAX_SIMD_REQCOUNT || nCount <= 0)
		return -1;
	if(pData	== NULL)	return -1;
	
	memset(&m_asFO, 0, MAX_SIMD_REQCOUNT * sizeof(SIMD_FO));
		
	for(i = 0; i < nCount; i ++)
		m_asFO[i].nCommand 	= ((long *)pData)[i];
	
    memcpy(&acSendBuffer[sizeof(SIMDHEADER)], m_asFO, nCount * sizeof(SIMD_FO));
    
	//=======
	// fill telegram header
	//======= 
	pHeader->acHeader[0]	= HEADER_REQ[0];
	pHeader->acHeader[1]	= HEADER_REQ[1];
	pHeader->wSize 			= sizeof(SIMDHEADER) + nCount * sizeof(SIMD_FO) + sizeof(SIMDFOOTER);
	pHeader->wType 			= SIMD_FOCMD;
	pHeader->wCounter 		= ++ nSingleCount;
	pHeader->wCycle 		= 0;
	pHeader->wNoElements 	= SIMD_FO_COUNT;

	//=======
	// fill and copy telegram footer
	//=======
	pFooter->acFooter[0]	= FOOTER_REQ[0];
	pFooter->acFooter[1]	= FOOTER_REQ[1];
	
//	//printf("Send FO %d %u\n", nCount, pHeader->wSize);
	SendData(m_nFOID, acSendBuffer, pHeader->wSize);
	
	return nSingleCount;	
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_FOFunc(char *pData, int nLength)						|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void		CTaskSiem::FOFunc(char *pData, int nLength)
{
	static int nFOCount = 0;
	if(pData == NULL || (unsigned int)nLength < sizeof(SIMDHEADER) + sizeof(SIMDFOOTER))
	{
		if(nFOCount >= 0 && nFOCount ++ > 1000)
		{
//			//printf("SIMD_FOFunc nlength=%d nFOCount =%d\n", nLength, nFOCount);
			//pthread_create(&m_ThreadID, NULL, (PRSRUN)_Open, NULL);
			pthread_create(&m_ThreadID, NULL, (PRSRUN)Reset, &m_nFOID);
			nFOCount = -40000;
		}
		return;
	}
	nFOCount 	= 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)pData;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&pData[nLength - 2];
	SIMD_FO		*pValue		= (SIMD_FO *)(pData + sizeof(SIMDHEADER));

	//=======
	// 檢驗數據正確性
	//=======
	if(memcmp(pHeader, HEADER_RES, 2) != 0)	return;
	if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;	
	if(pHeader->wSize != nLength)			return;		
	if((sizeof(SIMDHEADER) + pHeader->wNoElements * sizeof(SIMD_FO) + sizeof(SIMDFOOTER)) != (unsigned int)nLength) return;
	
	SIMD_FO		*pValue1	= pValue;
	for (int i = 0; i <= 5; i ++)
	{
		//printf("receive FO g_nFOState[%d] = %08x\n", i, pValue1->nState);
		pValue1 ++;
	}
	
//	g_ptaski32->Write(I32TX_FO, sizeof(I32_FOLIST), pValue);
	
	switch(pHeader->wType)
	{
		
		case SIMD_RESBASE + SIMD_FOCMD:
			/* go through every entry and print it */
			
//			//printf("telegram type %u; telegram counter %u\n", pHeader->wType, pHeader->wCounter);
			memset(g_nFOState, 0, sizeof(g_nFOState)/sizeof(int));
			for(int i = 0; i < pHeader->wNoElements; i ++)
				{
				g_nFOState[i]	= pValue->nState;
				pValue ++;
				}
				
			break;
		default:
//			//printf("type fault; type is %u", pHeader->wType);
			return;
			break;
	}
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_Write(int nDataType, int nLength, void *pData)			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:	nDataType	- 資料類型										|
|				pData		- 指向資料結构
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::Write(int nType, int nLength, void *pData)
{
	SIMD_ACTVALUE	*pValue;
	
	switch(nType)
	{
		case DATA_READVALUE:
			pValue	= (SIMD_ACTVALUE *)pData;
			ReqValues(pValue->nCount, pValue->nCommand, pValue->nCycle, (UINT32 *)pValue->pData);
			break;
		case DATA_FO:
			break;
		default:
			return 1;
	}
	
	return 0;
	
}

/*--------------------------------------------------------------------------+
|  Function :  g_ptasksiem->ReqCurve(int nCount, BYTE nMode, int *pIndexList)		|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:				- 												|
|							- 												|
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
DBCURVE*	CTaskSiem::ReqCurve(int nCount, BYTE nMode, int *pIndexList)
{
	int			i;
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;

	if(m_nCurveID == -1)	return m_Curve;
	if(nCount > CURVE_NUMBER)			return NULL;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)acSendBuffer;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + nCount * sizeof(SIMD_VALUE)];
	SIMD_VALUE	*pCurve		= (SIMD_VALUE *)&acSendBuffer[sizeof(SIMDHEADER)];
	
	for(i = 0; i < nCount; i ++)
	{
		pCurve->dwID			= pIndexList[i];
		pCurve->ucMode			= nMode;
		pCurve->ucDataType		= _LREAL;
		pCurve ++;
		
//		m_Curve[i].dwIndex		= pIndexList[i];
//		m_Curve[i].wCount		= 0;
//		//printf("g_ptasksiem->ReqCurve m_Curve[%d].dwIndex=%d\n", i, m_Curve[i].dwIndex);
	}
//	for(; i < 4; i ++)	m_Curve[i].wCount = 0;
	
	//=======
	// fill telegram header
	//======= 
	pHeader->acHeader[0]	= HEADER_REQ[0];
	pHeader->acHeader[1]	= HEADER_REQ[1];
	pHeader->wSize 			= sizeof(SIMDHEADER) + nCount * sizeof(SIMD_VALUE) + sizeof(SIMDFOOTER);
	pHeader->wType 			= 21;
	pHeader->wCounter 		= ++ nSingleCount;
	pHeader->wCycle 		= 0;
	pHeader->wNoElements 	= nCount;

	//=======
	// fill and copy telegram footer
	//=======
	pFooter->acFooter[0]	= FOOTER_REQ[0];
	pFooter->acFooter[1]	= FOOTER_REQ[1];
	
	SendData(m_nCurveID, acSendBuffer, pHeader->wSize);	
//		//printf("g_ptasksiem->ReqCurve\n");
	return m_Curve;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_CurveFunc(char *pData, int nLength)						|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:				- 												|
|							- 												|
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CTaskSiem::CurveFunc(char *pData, int nLength)
{
	static int	nCurveIndex		= 0;
	static int	nCopyedCount	= 0;
	static int	nCurveSize		= 0;
	static int	nCurveID		= 0;
	static int	nCurveReceived	= 0;
	static int	nPointCount		= 0;		// 點的數目	
//	double		*pValue;
	int			nDataLength;
//	//printf("TaskSiem::CurveFunc()\n");
	static int nCurveCount = 0;
	if(pData == NULL || (unsigned int)nLength < sizeof(SIMDHEADER) +sizeof(SIMDFOOTER))
	{
		if(nCurveCount >= 0 && nCurveCount ++ > 1000)
		{
			pthread_create(&m_ThreadID, NULL, (PRSRUN)Reset, &m_nCurveID);
			nCurveCount = -40000;
		}
		return;
	}
	nCurveCount 	= 0;
/*	
	BYTE *pchar	= (BYTE *)pData;
	for(i = 0; i < nLength; i ++)
	{
		//printf("%02x ", *pchar ++);
		if(((i+1) % 16) == 0) //printf("\n");
	}
	//printf("\n");
*/	
//	//printf("CurveFunc, nlength is %u, nCurveIndex=%d\n", nLength, nCurveIndex);
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)pData;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)(pData + nLength - 2);
	SIMD_CURVE	*pCurve		= (SIMD_CURVE *)(pData + sizeof(SIMDHEADER));

DEALDATA:
	
	if(nCurveIndex == 0)
	{
		//printf("CurveFunc, size is %u,NoElements is %u, Index=%lu, dwDistance =%d, startX =%f\n", 
//				pHeader->wSize,pHeader->wNoElements, pCurve->dwIndex, pCurve->dwDistance, pCurve->dStartX);
		if(memcmp(pHeader, HEADER_RES, 2) != 0)	return;
		if(pHeader->wNoElements == 0)			return;
		if(pHeader->wNoElements > 500)			return;
		if(pCurve->dwIndex < 1 || pCurve->dwIndex >= (DWORD)CURVE_NUMBER)	return;
//		for(i = 0; i < 4; i ++)
//			if(pCurve->dwIndex == m_Curve[i].dwIndex)	break;
//		if(i >= 4)	return;
			
		nCopyedCount	= 0;
		nCurveReceived	= 0;
		nCurveID		= pCurve->dwIndex - 1;
		nPointCount		= pHeader->wNoElements;
//		pValue	= (double *)(pCurve + 1);
		
		m_Curve[nCurveID].nIndex			= pCurve->dwIndex;
		m_Curve[nCurveID].nCount			= 0;
		m_Curve[nCurveID].dDistance			= pCurve->dwDistance / 1000.0;
//		m_Curve[nCurveID].wState			= pCurve->bState;
		m_Curve[nCurveID].DataType.wType	= TYPE_DOUBLE;
		m_Curve[nCurveID].dStartX			= pCurve->dStartX;
		m_Curve[nCurveID].DataType.wLength	= sizeof(double);
		
		nCurveSize		= pHeader->wSize;
		if(pHeader->wSize > nLength)
		{			
			nDataLength		= nLength - sizeof(SIMDHEADER) - sizeof(SIMD_CURVE);
//			//printf("SIMD_CurveFunc nDataLength=%d,%d %u, %u\n", nDataLength, nLength, sizeof(SIMDHEADER), sizeof(SIMD_CURVE));
			nCurveIndex ++;
			memcpy(m_Curve[nCurveID].Value, pCurve + 1, nDataLength);
		}
		else
		{
			if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;			
			nDataLength		= nLength - sizeof(SIMDHEADER) - sizeof(SIMD_CURVE) - sizeof(SIMDFOOTER);
//			//printf("SIMD_CurveFunc nDataLength=%d,%d %u, %u, %u\n", nDataLength, nLength, sizeof(SIMDHEADER), sizeof(SIMD_CURVE), sizeof(SIMDFOOTER));
			memcpy(m_Curve[nCurveID].Value, pCurve + 1, nDataLength);
			m_Curve[nCurveID].nCount		= nPointCount;
			nCurveIndex		= 0;
			m_nCurveState	= 1;		// 已經接收到Curve;
//			//printf("%u,%u, %lf, %lf\n", m_Curve[nCurveID].nIndex, m_Curve[nCurveID].nCount, m_Curve[nCurveID].dDistance, m_Curve[nCurveID].dStartX);
			if(nPointCount > 0)
			{
				CtmRecordFile	CurveFile;
				CurveFile.Open("curve.cdb", sizeof(DBCURVE));
				CurveFile.WriteRecord(m_Curve[nCurveID].nIndex, &m_Curve[nCurveID]);
				CurveFile.Close();
			}
//			//printf("SIMD_CurveFunc CopyedCount=%d\n", nCopyedCount + nDataLength);
//			//printf("SIMD_CurveFunc curveID=%d, wCount=%d, dwIndex=%lu\n", nCurveID, m_Curve[nCurveID].wCount, m_Curve[nCurveID].dwIndex);
		}
//		//printf("SIMD_CurveFunc CopyedCount=%d\n", nCopyedCount + nDataLength);		
	}
	else
	{
//		//printf("SIMD_CurveFunc nLength=%d, nCurveReceived=%d, nCurveSize =%d\n", nLength, nCurveReceived, nCurveSize);
		if(nLength + nCurveReceived > nCurveSize)
		{
			//printf("SIMD_CurveFunc Length Error , Reset\n");
			nCurveIndex		= 0;
			goto DEALDATA;
		}		
		
		if(nLength + nCurveReceived == nCurveSize)
		{
			if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;			
			nDataLength		= nLength	- sizeof(SIMDFOOTER);
//			//printf("SIMD_CurveFunc nDataLength=%d,%d %u\n", nDataLength, nLength, sizeof(SIMDFOOTER));
			memcpy((char *)m_Curve[nCurveID].Value + nCopyedCount, pData, nDataLength);				
			m_Curve[nCurveID].nCount		= nPointCount;
			m_nCurveState	= 1;		// 已經接收到Curve;
			nCurveIndex		= 0;
//			//printf("%u,%u, %lf, %lf\n", m_Curve[nCurveID].nIndex, m_Curve[nCurveID].nCount, m_Curve[nCurveID].dDistance, m_Curve[nCurveID].dStartX);
			if(nPointCount > 0)
			{
				CtmRecordFile	CurveFile;
				CurveFile.Open("curve.cdb", sizeof(DBCURVE));
				CurveFile.WriteRecord(m_Curve[nCurveID].nIndex, &m_Curve[nCurveID]);
				CurveFile.Close();
			}
//			//printf("SIMD_CurveFunc CopyedCount=%d\n", nCopyedCount + nDataLength);
//			//printf("SIMD_CurveFunc curveID=%d, wCount=%d, dwIndex=%lu\n", nCurveID, m_Curve[nCurveID].wCount, m_Curve[nCurveID].dwIndex);
		}
		else
		{
			nDataLength	= nLength;
			nCurveIndex	++;
			memcpy((char *)m_Curve[nCurveID].Value + nCopyedCount, pData, nDataLength);		
		}
//		//printf("SIMD_CurveFunc CopyedCount=%d\n", nCopyedCount + nDataLength);
	}
	nCurveReceived += nLength;
	nCopyedCount   += nDataLength;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_SetTime()												|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:				- 												|
|							- 												|
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::SetTime()
{
	int			nLeapCount;
	time_t		now;
	struct tm 	tmDate;	
	DWORD		dwDay, dwTime;	
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)acSendBuffer;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + sizeof(SIMD_DAYTIME)];
	SIMD_DAYTIME	*pValue	= (SIMD_DAYTIME *)(acSendBuffer + sizeof(SIMDHEADER));
	
	if(m_nMsgID == -1)	return -1;

	//=======
	// 計算時間
	//=======
	time(&now);
	tmDate	= *localtime(&now);		// 轉換時間格式
	
//	//printf("%s ", asctime(&tmDate));
	
	nLeapCount	= (tmDate.tm_year - 92) / 4;
//	//printf("leep is %d ", nLeapCount);
	dwDay	= (tmDate.tm_year - 92) * 365 + tmDate.tm_yday + nLeapCount + 1;
	dwTime	= ((tmDate.tm_hour * 60 + tmDate.tm_min) * 60 + tmDate.tm_sec) * 1000;
	
//	//printf("dwDay =%08x, dwTime = %08x\n",(unsigned)dwDay, (unsigned)dwTime);
	pValue->dwDay	= dwDay;
	pValue->dwTime	= dwTime;
	
	//=======
	// fill telegram header
	//======= 
	pHeader->acHeader[0]	= HEADER_REQ[0];
	pHeader->acHeader[1]	= HEADER_REQ[1];
	pHeader->wSize 			= sizeof(SIMDHEADER) + sizeof(SIMD_DAYTIME) + sizeof(SIMDFOOTER);
	pHeader->wType 			= 41;
	pHeader->wCounter 		= ++ nSingleCount;
	pHeader->wCycle 		= 0;
	pHeader->wNoElements 	= 0;

	//=======
	// fill and copy telegram footer
	//=======
	pFooter->acFooter[0]	= FOOTER_REQ[0];
	pFooter->acFooter[1]	= FOOTER_REQ[1];

	SendData(m_nMsgID, acSendBuffer, pHeader->wSize);
	
	return 0;
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_SaveMsg(int nCount, SIMD_MSG *pMsg)						|
|  Task     :  把消息保存到文件中                                           |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
/*int		CTaskSiem::SaveMsg(int nCount, SIMD_MSG *pMsg)
{
	int	i;
	
	pthread_mutex_lock(&mutex_Msg);
	g_ErrorFile.ReadRecord(1, &m_Msg);
	for(i = 0; i < nCount; i ++)
	{
		if(m_Msg.nCount < MAX_SIMD_MSGCOUNT)	m_Msg.nCount ++;
		
		g_ErrorFile.WriteRecord(m_Msg.nIndex + 2, pMsg);

		m_Msg.nIndex ++;
		if(m_Msg.nIndex >= MAX_SIMD_MSGCOUNT)	m_Msg.nIndex = 0;
		
		pMsg ++;
	}
	
	g_ErrorFile.WriteRecord(1, &m_Msg);
	pthread_mutex_unlock(&mutex_Msg);
	return 0;
}	*/
/*--------------------------------------------------------------------------+
|  Function :  SIMD_FOFunc(char *pData, int nLength)						|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CTaskSiem::MsgFunc(char *pData, int nLength)
{
	static int nMsgCount = 0;
	if(pData == NULL || (unsigned int)nLength < sizeof(SIMDHEADER) +sizeof(SIMDFOOTER))
	{		
		
		if(nMsgCount >= 0 && nMsgCount ++ > 1000)
		{
//			//printf("SIMD_CurveFunc nlength=%d nMsgCount =%d\n", nLength, nMsgCount);
			//pthread_create(&m_ThreadID, NULL, (PRSRUN)_Open, NULL);
			pthread_create(&m_ThreadID, NULL, (PRSRUN)Reset, &m_nMsgID);
			nMsgCount = -40000;
		}
		return;
	}
	nMsgCount 	= 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)pData;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&pData[nLength - 2];
	SIMD_MSG	*pMsg 		= (SIMD_MSG *)(pData + sizeof(SIMDHEADER));

	if(memcmp(pHeader, HEADER_RES, 2) != 0)	return;
	if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;	
	if(pHeader->wSize != nLength)			return;		
	if(pHeader->wType != 141) 				return;

//	SaveMsg(pHeader->wNoElements, pMsg);
//	g_ptaski32->Write(I32TX_ERROR, 0, NULL);
/*	for(int i = 0; i < pHeader->wNoElements; i ++)
	{
//		//printf("MsgType is %u, ojbnumber is %u\n", pMsg->bMsgType, pMsg->wObjNumber);
//		//printf("Data is %lu, Time is %lu\n", pMsg->dwData, pMsg->dwTime);
//		//printf("hour=%lu\n", pMsg->dwTime / 3600000);
		switch(pMsg->bMsgType)
		{
			case 1:
				//printf("FO-Fault:");
				//printf("Error=%ld, ErrorID1=%ld, ErrorID2=%ld, TOErroID=%ld\n", pMsg->lFO_Error, pMsg->lFO_ErrorID1, pMsg->lFO_ErrorID2, pMsg->lFO_ErrorID3);
				break;
			case 2:
				//printf("TO-Fault: AlarmNO=%ld\n", pMsg->lAlarmNo);
				break;
			case 3:
				//printf("Time Falut: InitEvent =%lu\n", pMsg->dwInitEvent);
				break;
			case 4:
				//printf("ExecutionFault: ExecFaluttype=%lu\n", pMsg->dwExecFaultType);
				break;
			case 5:
				//printf("PeripheralFault:");
				//printf("InitEvent=%lu, LogBaseAdrIn=%ld, LogBaseAdrOut=%ld, LogDiagAdr=%ld\n", pMsg->dwPH_InitEvent, pMsg->lPH_LogBaseAdrIn, pMsg->lPH_LogBaseAdrOut, pMsg->lPH_LogDiagAdr);
				break;
			case 6:
				//printf("TelegramFault:");
				//printf("Errorcode=%ld, SizeRet=%u, TypeRet=%u, CounterRec=%u, CycleRec=%u, NoElementsRet=%u\n", pMsg->lErrorCode, pMsg->nSizeRet, pMsg->nTypeRet, pMsg->nCounterRet, pMsg->nCycleRet, pMsg->nNoElementsRet);
				
				break;
			default:
				//printf("unknown message\n");
		}
		pMsg ++;
	}*/

}


/*--------------------------------------------------------------------------+
|	function	: SyncFrom()												|
|	remark		: 把資料庫從主机同步上來									|
|---------------------------------------------------------------------------|
|	call		: SyncFrom()												|
|	return 		:				: 											|
|				:				: 											|
+--------------------------------------------------------------------------*/
int		CTaskSiem::SyncFrom()
{
	int	i, j;
	int	nCount	= g_pDatabase->GetIDCount();
	UINT32	IDList[100];
	UINT32	ID;
	DBVALUE	dbValue;

	i = 0;
	j = 0;
//	//printf("SyncFrom(): nCount=%d \n", nCount);
	while (i < nCount)
		{
		ID = g_pDatabase->IndexToDataID(i);
		dbValue = g_pDatabase->Read(i);
		if (dbValue.dwDataPath == DATA_PATH_ELECTRIC)
			{
			IDList[j]	= ID;
			j++;
			}
		if (j == 100) 
			{
			ReqValues(100, 3, 0, IDList);
			Delay(50);
			j = 0;
			}
		i++;
		}
	if (j > 0) ReqValues(j, 3, 0, IDList);
	return 0;
}

void	CTaskSiem::GetActualData()								//<<yang 2006/2/20 
{
	m_lTimerCount	= 1000;
	
	ReqValues(sizeof(g_apcActualID)/sizeof(char*), 2, 100, g_apcActualID);
	Delay(20);
	ReqValues(sizeof(g_asRealDataID)/sizeof(char*), 1, 15, g_asRealDataID);
}

void	CTaskSiem::StopActualData()							//<<yang 2006/2/20 
{
	ReqValues(sizeof(g_apcActualID)/sizeof(char*), 2, 0, g_apcActualID);
	Delay(20);
	ReqValues(sizeof(g_asRealDataID)/sizeof(char*), 1, 0, g_asRealDataID);
}

void	CTaskSiem::SetConfigData()
{
	short	i=0;
	DBVALUE	dbTemp;
	
	while (g_aConfigData[i].pcID != NULL)
		{
		dbTemp	= g_pDatabase->Read(g_aConfigData[i].pcID);
		
		if (dbTemp.wData < 1 || dbTemp.wData > g_aConfigData[i].wConfigValue)	//<<yang 2006/4/14 
			{
			SetDBValue(g_aConfigData[i].pcID, g_aConfigData[i].wConfigValue);
			printf("%s=%d g_aConfigData[%d].Value=%d\n", g_aConfigData[i].pcID, dbTemp.wData, i, g_aConfigData[i].wConfigValue);
			}
		i++;
		}
	
	if (GetDBValue(RCV_PROF_SPEED_CNNOPROFILEPOINTS).lValue != GetDBValue(RCV_PROF_PRESSURE_CNNOPROFILEPOINTS).lValue)
		SetDBValue(RCV_PROF_PRESSURE_CNNOPROFILEPOINTS, GetDBValue(RCV_PROF_SPEED_CNNOPROFILEPOINTS).lValue);
	if (GetDBValue(INJ_PROF_INJVELOCITY_CNNOPROFILEPOINTS).lValue != GetDBValue(INJ_PROF_INJPRESSURELIMIT_CNNOPROFILEPOINTS).lValue)
		SetDBValue(INJ_PROF_INJPRESSURELIMIT_CNNOPROFILEPOINTS, GetDBValue(INJ_PROF_INJVELOCITY_CNNOPROFILEPOINTS).lValue);
}

void	CTaskSiem::SetCurve()
{
	short	i=0;
	DBVALUE CurveData[sizeof(g_aCurveData)/sizeof(CONFIGDATA)];
	
	while (g_aCurveData[i].pcID != NULL)
		{
		CurveData[i]		= g_pDatabase->Read(g_aCurveData[i].pcID);
		CurveData[i].lData	= g_aCurveData[i].wConfigValue;
		i++;
		}
	if (i > 0)	WriteValue(i, CurveData);
//	//printf("config count=%d  number=%d\n", i, sizeof(g_aConfigData)/sizeof(CONFIGDATA));

	//========
	// 設置關模曲線最大長度
	//========
	double	dMaxData;
	DBVALUE	data;
	
	dMaxData	= g_pDatabase->Read(MLD_CONFIG_POSNMAXMOLDHEIGTH).dData;
	////printf("open mold curve max is %3.2f\n", dMaxData);
	
	data		= g_pDatabase->Read(MLD_TRACE_ACTUALVELOCITY_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(MLD_TRACE_VELOCITYSETPOINT_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(MLD_TRACE_ACTUALFORCE_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(MLD_TRACE_FORCELIMIT_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	//=======
	// 模保曲線長度
	//=======
	char	sz[100];
	strncpy(sz, "MLD_PROF_PROTECTION_POSNRANGE", 0xFF);
	dMaxData	= g_pDatabase->Read(sz).dData;
	//printf("open mold curve max is %3.2f\n", dMaxData);
	
	strncpy(sz, "CURVES10_TRACE_POSNRECORDINGDATA", 0xFF);
	data		= g_pDatabase->Read(sz);
	data.dData	= dMaxData;
	WriteValue(1, &data);

	//========
	// 設置射出曲線最大長度
	//========
	double 	dMax, dMin;
	dMax	= g_pDatabase->Read(INJ_PROFLMT_INJVELOCITY_POSNMAX).dData;
	////printf("inject curve max is %3.2f\n", dMax);
	
	dMin	= g_pDatabase->Read(INJ_PROFLMT_INJVELOCITY_POSNMIN).dData;
	////printf("inject curve max is %3.2f\n", dMin);
	
	dMaxData	= dMax -dMin;
	
	data		= g_pDatabase->Read(INJ_TRACE_INTERPOLATEDVELOCITYSETPOINT_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(INJ_TRACE_ACTUALINJVELOCITY_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(INJ_TRACE_INJPRESSURELIMITSETPOINT_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(INJ_TRACE_ACTUALINJPRESSURE_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	
	//=======
	// 設置儲料曲線最大長度
	//=======
	data		= g_pDatabase->Read(RCV_TRACE_ACTUALSPEED_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(RCV_TRACE_SPEEDSETPOINT_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(RCV_TRACE_ACTUALPRESSURE_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	data		= g_pDatabase->Read(RCV_TRACE_PRESSURESETPOINT_POSNRECORDINGDATA);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	//=======
	// 設置脫模曲線最大長度
	//=======
	
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
	WriteValue(1, &data);
	
	strncpy(sz, "CURVES2_TRACE_POSNRECORDINGDATA", 0xFF);
	data		= g_pDatabase->Read(sz);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	strncpy(sz, "CURVES3_TRACE_POSNRECORDINGDATA", 0xFF);
	data		= g_pDatabase->Read(sz);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	strncpy(sz, "CURVES4_TRACE_POSNRECORDINGDATA", 0xFF);
	data		= g_pDatabase->Read(sz);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	strncpy(sz, "CURVES5_TRACE_POSNRECORDINGDATA", 0xFF);
	data		= g_pDatabase->Read(sz);
	data.dData	= dMaxData;
	WriteValue(1, &data);
	
	strncpy(sz, "CURVES6_TRACE_POSNRECORDINGDATA", 0xFF);
	data		= g_pDatabase->Read(sz);
	data.dData	= dMaxData;
	WriteValue(1, &data);	
	
	
}

void	CTaskSiem::_Open(void *pData)
{	
//	//printf("_Open Connectiong = %d\n", m_nConnecting);
	InitCore();
	if(m_nConnecting == 1 || m_nConnecting == 2)	return;
	m_nConnecting	= 1;
	if (m_nValueID	!= -1) DelDevice(m_nValueID);
	if (m_nFOID		!= -1) DelDevice(m_nFOID);
	if (m_nCurveID	!= -1) DelDevice(m_nCurveID);
	if (m_nMsgID	!= -1) DelDevice(m_nMsgID);
	if (m_nParaID	!= -1) DelDevice(m_nParaID);
	if (m_nRecipeID != -1) DelDevice(m_nRecipeID);
		
	m_nValueID	=
	m_nFOID		=
	m_nMsgID	=
	m_nCurveID	=
	m_nRecipeID =
	m_nParaID	= -1;
	//printf("before Connecting\n");
	//SyncSystemData();													//<<yang 2006/11/2 
	while (TRUE)
		{
		if (m_nValueID == -1)
			{
			m_ethernet.NetFunc	= (NETFUNC)ResValueFunc;	
			m_ethernet.wdstPort	= 2301;
			m_nValueID			= AddDevice("CEthernet", &m_ethernet);
			//if(m_nValueID == -1)//printf("Create value Communication error!\n");
			//else				//printf("Create Value Communication success!\n");
			}
		if (m_nFOID == -1)
			{
			m_ethernet.NetFunc	= (NETFUNC)FOFunc;
			m_ethernet.wdstPort	= 2302;
			m_nFOID				= AddDevice("CEthernet", &m_ethernet);
			//if(m_nFOID == -1)	//printf("Create FO Communication error!\n");
			//else				//printf("Create FO Communication success!\n");
			}
		if (m_nRecipeID == -1)
			{
			m_ethernet.NetFunc	= (NETFUNC)RecipeFunc;
			m_ethernet.wdstPort	= 2303;
			m_nRecipeID			= AddDevice("CEthernet", &m_ethernet);
			//if(m_nRecipeID == -1)//printf("Create Recipe Communication error!\n");
			//else				//printf("Create Recipe Communication success!\n");
			}	
		if (m_nCurveID == -1)
			{
			m_ethernet.NetFunc	= (NETFUNC)CurveFunc;
			m_ethernet.wdstPort	= 2304;
			m_nCurveID			= AddDevice("CEthernet", &m_ethernet);
			//if(m_nCurveID == -1) //printf("Create Curve Commnication error!\n");
			//else				 //printf("Create Curve Commnication Success!\n");
			}
		if (m_nMsgID == -1)
			{
			m_ethernet.NetFunc	= (NETFUNC)MsgFunc;
			m_ethernet.wdstPort	= 2305;
			m_nMsgID			= AddDevice("CEthernet", &m_ethernet);
			//if(m_nMsgID == -1)	//printf("Create Message Communication error!\n");
			//else				//printf("Create Message Communication success!\n");
			}
		if (m_nParaID == -1)
			{
			m_ethernet.NetFunc	= (NETFUNC)ParaFunc;
			m_ethernet.wdstPort	= 2306;
			m_nParaID			= AddDevice("CEthernet", &m_ethernet);
			//if(m_nParaID == -1)	//printf("Create Parameter Communication error!\n");
			//else				//printf("Create Parameter Communication success!\n");
			}
		
		if (m_nValueID != -1 && m_nFOID != -1 && m_nCurveID != -1 && m_nMsgID != -1 && m_nParaID != -1 && m_nRecipeID != -1)
			break;
		else
			Sleep(5);
		}
	SyncFrom();
	Delay(20);
	if(FOReady())
		{
		GetActualData();
		Delay(20);
		SetTime();
		Delay(20);
		SetCurve();
		Delay(20);
		SetConfigData();
//		g_ptaski32->WriteValue(EJT_GENER_CNNOSTROKES);					//<<yang 2006/10/21 for I32 and SimotionD use the same ID
		}
	InitCore();
//	SyncSystemData();													//<<yang 2006/11/2 
	//========
	// 發送所有曲線的請求
	//========
	int	nCurveList[CURVE_NUMBER];
	for(int i = 0; i < CURVE_NUMBER; i ++)		nCurveList[i] = i + 1;
	//	g_ptasksiem->ReqCurve(CURVE_NUMBER, 0, nCurveList);
	
	m_nConnecting	= 2;
	
	return ;
}



void 	CTaskSiem::InitCore()
{
	
	
}

/*--------------------------------------------------------------------------+
|  Function :  SIMD_STATUS(int nIndex)                             			|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CTaskSiem::GetStatus(int nIndex)
{
	switch(nIndex)
	{
		case COM_VALUE_ID:
			return m_nValueID;
			break;
		case COM_FO_ID:
			return m_nFOID;
			break;
		case COM_CURVE_ID:
			return m_nRecipeID;
			break;
		case COM_MSG_ID:
			return m_nCurveID;
			break;
		case COM_PARA_ID:
			return m_nMsgID;
			break;
		case COM_RECIPE_ID:
			return m_nParaID;
			break;
		default:
			break;
	}
	return -1;
}

void	CTaskSiem::ParaFunc(char *pData, int nLength)
{
	static int nValueCount = 0;
	
	if(pData == NULL || (unsigned int)nLength < sizeof(SIMDHEADER) +sizeof(SIMDFOOTER))
	{
		if(nValueCount >= 0 && nValueCount ++ > 1000)
		{
			pthread_create(&m_ThreadID, NULL, (PRSRUN)Reset, &m_nParaID);
			nValueCount = -40000;
		}
		return;
	}
	nValueCount 	= 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)pData;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&pData[nLength - 2];
	char*		pValue		= pData + sizeof(SIMDHEADER);
	//printf("Receive Type=%d Element=%d ParaNo=%d Index=%d Stat=%d DataType=%d Size=%d Length=%d \n", pHeader->wType, pHeader->wNoElements, 
	//	((SIMD_PARADATA *)pValue)->wParaNo, ((SIMD_PARADATA *)pValue)->wSubIndex, ((SIMD_PARADATA *)pValue)->bState, 
	//	((SIMD_PARADATA *)pValue)->bDataType, pHeader->wSize, nLength);
	//=======
	// 檢驗數據正確性
	//=======
	if(memcmp(pHeader, HEADER_RES, 2) != 0)	return;
	if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;	
	if(pHeader->wSize != nLength || pHeader->wNoElements > 20)	return;		
	if((sizeof(SIMDHEADER) + pHeader->wNoElements * sizeof(SIMD_PARADATA) + sizeof(SIMDFOOTER)) != (unsigned int)nLength) return;
	
	m_Para.wType = pHeader->wType;
	memcpy((char* )m_Para.ParaData, pValue, pHeader->wNoElements * sizeof(SIMD_PARADATA));
}

int		CTaskSiem::ReqPara(int nCount, int nType, int nCycle, SIMD_PARA Para)
{
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)acSendBuffer;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + nCount * sizeof(SIMD_PARADATA)];
	
	if(m_nParaID == -1)	return -1;
	if(nCount > MAX_PARA_REQCOUNT || nCount <= 0)
		return -1;
	
    memcpy(&acSendBuffer[sizeof(SIMDHEADER)], (char *)Para.ParaData, nCount * sizeof(SIMD_PARADATA));
    
	//=======
	// fill telegram header
	//======= 
	pHeader->acHeader[0]	= HEADER_REQ[0];
	pHeader->acHeader[1]	= HEADER_REQ[1];
	pHeader->wSize 			= sizeof(SIMDHEADER) + nCount * sizeof(SIMD_PARADATA) + sizeof(SIMDFOOTER);
	pHeader->wType 			= nType;
	pHeader->wCounter 		= ++ nSingleCount;
	pHeader->wCycle 		= nCycle;
	pHeader->wNoElements 	= nCount;
	char*		pValue		= acSendBuffer + sizeof(SIMDHEADER);
	//printf("Size=%d Type=%d Elements=%d ParaNo=%d Sunindex=%d DataType=%d\n", 
	//	pHeader->wSize, pHeader->wType, pHeader->wNoElements, ((SIMD_PARADATA *)pValue)->wParaNo, 
	//	((SIMD_PARADATA *)pValue)->wSubIndex, ((SIMD_PARADATA *)pValue)->bDataType);
	//=======
	// fill and copy telegram footer
	//=======
	pFooter->acFooter[0]	= FOOTER_REQ[0];
	pFooter->acFooter[1]	= FOOTER_REQ[1];
	SendData(m_nParaID, acSendBuffer, pHeader->wSize);
	
	return 0;
}

SIMD_PARADATA		CTaskSiem::GetParaValue(WORD wParaNo, WORD wIndex)
{
	int		i;
	SIMD_PARADATA	smParaData;
	
	memset(&smParaData, 0, sizeof(SIMD_PARADATA));
//	for (i=0; i<MAX_PARA_REQCOUNT; i++)
//		{
//		if (m_Para.ParaData[i].wParaNo == wParaNo && m_Para.ParaData[i].wSubIndex == wIndex)
//			{
//			//printf("GetParaValue: wParaNo=%d wIndex=%d \n", wParaNo, wIndex);
			return m_Para.ParaData[0];
/*			switch (m_Para.ParaData[i].bDataType)
				{
				case _BOOL:
					nValue = *(WORD* )m_Para.ParaData[i].abValue;
					break;
				case _BYTE:
				case _USINT:
					nValue = *(BYTE* )m_Para.ParaData[i].abValue;
					break;
				case _WORD:
					nValue = *(WORD* )m_Para.ParaData[i].abValue;
					break;
				case _DWORD:
				case _UDINT:
					nValue = *(DWORD* )m_Para.ParaData[i].abValue;
					break;
				case _UINT:
					nValue = *(WORD* )m_Para.ParaData[i].abValue;
					break;
				case _SINT:
					nValue = *(BYTE* )m_Para.ParaData[i].abValue;
					break;
				case _INT:
					nValue = *(short* )m_Para.ParaData[i].abValue;
					break;
				case _DINT:
					nValue = *(int* )m_Para.ParaData[i].abValue;
					break;
				case _REAL:
					nValue = (int)round(*(float* )m_Para.ParaData[i].abValue);
					break;
				case _LREAL:
					nValue = (int)round(*(double* )m_Para.ParaData[i].abValue);
					break;
				case _TIME:
				case _DATE:
				case _TOD:
				case _DT:
					
					break;
				case _UNKNOWN:
				default:
					
					break; 
				}
			return	nValue;*/
//			}
//		}
//	return smParaData;
}

BOOL			CTaskSiem::FOReady()
{
	long		lFOFlag = FALSE;
	UINT32 IDList[sizeof(g_pszFOStartupID)/sizeof(char *)];
	int i=0;
	
	for (int i=0; i<sizeof(g_pszFOStartupID)/sizeof(char *); i++)
		{
		IDList[i] = g_pDatabase->GetDataID(g_pszFOStartupID[i]);
		g_pDatabase->Write(g_pszFOStartupID[i], &lFOFlag);
		}
	while (i < 5)
		{
		ReqValues(sizeof(g_pszFOStartupID)/sizeof(char *), 3, 0, IDList);
		Sleep(1);
		if (GetDBValue(g_pszFOStartupID[0]).lValue && 
			(GetDBValue(g_pszFOStartupID[1]).lValue || GetDBValue(g_pszFOStartupID[2]).lValue))
			break;
		i++;
		if (i == 5)
			{
			//printf("FO Not Ready!\n");
			return FALSE;
			}
		}
	//printf("FO Ready!\n");
	return	TRUE;
}

int				CTaskSiem::ReqRecipe(int nCount, SIMD_RECIPEDATA* pRecipeList)
{
	char 		acSendBuffer[PACKAGE_LENGTH];
	static	int	nSingleCount = 0;
	int			nSequence = 1; 
	int			i;
	int			nCurrentCount;
	int			nCycle = nCount/100 + (nCount%100 > 0) + 1;
	SIMDHEADER	*pHeader;
	SIMDFOOTER	*pFooter;
	
	if(m_nRecipeID == -1 || nCount <= 0)	return -1;
	
	for (i=0; i<nCycle; i++)
		{
		if (i == nCycle - 1)	
			{
			nSequence		= 0;
			nCurrentCount	= 0;
			}
		else if (nCount-100*i > 100) nCurrentCount = 100;
		else nCurrentCount = nCount-100*i;
		
		pHeader	= (SIMDHEADER *)acSendBuffer;
		pFooter	= (SIMDFOOTER *)&acSendBuffer[sizeof(SIMDHEADER) + nCurrentCount * sizeof(SIMD_RECIPEDATA)];

	    memcpy(&acSendBuffer[sizeof(SIMDHEADER)], (char *)&pRecipeList[i*100], nCurrentCount * sizeof(SIMD_RECIPEDATA));
	    
		//=======
		// fill telegram header
		//======= 
		pHeader->acHeader[0]	= HEADER_REQ[0];
		pHeader->acHeader[1]	= HEADER_REQ[1];
		pHeader->wSize 			= sizeof(SIMDHEADER) + nCurrentCount * sizeof(SIMD_RECIPEDATA) + sizeof(SIMDFOOTER);
		pHeader->wType 			= 11;
		pHeader->wCounter 		= ++ nSingleCount;
		pHeader->wSequence 		= nSequence++;
		pHeader->wNoElements 	= nCurrentCount;
		//char*		pValue		= acSendBuffer + sizeof(SIMDHEADER);
		//printf("Recipe Size=%d Type=%d Elements=%d \n", 
		//	pHeader->wSize, pHeader->wType, pHeader->wNoElements);
		//=======
		// fill and copy telegram footer
		//=======
		pFooter->acFooter[0]	= FOOTER_REQ[0];
		pFooter->acFooter[1]	= FOOTER_REQ[1];
		SendData(m_nRecipeID, acSendBuffer, pHeader->wSize);
		}
	return 0;
}

void			CTaskSiem::RecipeFunc(char *pData, int nLength)
{
	static int	nValueCount = 0;
	int			nIndex;
	if(pData == NULL || (unsigned int)nLength < sizeof(SIMDHEADER) +sizeof(SIMDFOOTER))
	{
		if(nValueCount >= 0 && nValueCount ++ > 1000)
		{
			pthread_create(&m_ThreadID, NULL, (PRSRUN)Reset, &m_nRecipeID);
			nValueCount = -40000;
		}
		return;
	}
	nValueCount = 0;
	
	SIMDHEADER	*pHeader	= (SIMDHEADER *)pData;
	SIMDFOOTER	*pFooter	= (SIMDFOOTER *)&pData[nLength - 2];
	SIMD_RECIPEDATA* pValue	= (SIMD_RECIPEDATA *)(pData + sizeof(SIMDHEADER));
	//printf("Recipe Receive Type=%d Element=%d Size=%d Length=%d \n", 
		//pHeader->wType, pHeader->wNoElements, pHeader->wSize, nLength);
	//=======
	// 檢驗數據正確性
	//=======
	if(memcmp(pHeader, HEADER_RES, 2) != 0)	return;
	if(memcmp(pFooter, FOOTER_RES, 2) != 0)	return;	
	if(pHeader->wSize != nLength || pHeader->wNoElements > 100 || pHeader->wType != 111 || pHeader->wSequence == 0)	return;		
	if((sizeof(SIMDHEADER) + pHeader->wNoElements * sizeof(SIMD_RECIPEDATA) + sizeof(SIMDFOOTER)) != (unsigned int)nLength) return;
	
	for (int i=0; i<pHeader->wNoElements; i++)
		{
		nIndex = g_pDatabase->DataIDToIndex(pValue[i].dwID);
		g_pDatabase->Write(nIndex, pValue[i].abData);		
		}
}
