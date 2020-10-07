#include	"../../package.h"
#include	"../../timer.h"
#include	"../../main.h"
#include	"../../tmdebug.h"
#include	"../../init.h"
#include	"../../utils.h"
#include	"../../ethnet.h"
#include	"../libComm.h"
#include	"tmTaskPicker.h"
IMPLEMENT_DYNCREATE(CtmTaskPicker, CTaskHost)
typedef void *(* PRSRUN)(void *);

#define		CNT_INT_CHECK_ONLNE_TIME	3000

char*	u_pszDBStrID[] = 
{
	"MACHINE_PROFILE_STEPNUM",
	"MACHINE_PROFILE_EQUIPMENT1_STEPNUM",
};

char*	g_Dsp28RealID[] =
{
   //"MACHINE_INTERFACE_WOPERATINGSTATE",    		            //Operation status
	 //"MACHINE_INTERFACE_WSTEPNUMBER",         		          //Step num
	 //"MACHINE_INTERFACE_WMOTIONTYPE",         		          //Step type
	 //"MACHINE_INTERFACE_WTYPENUMBER",         		          //Step type num
	 //"MACHINE_INTERFACE_DWRESERVED1",                       //Step status
		//"AXIS1_INTERFACE_CONTROL_FACTUALVELOCITY",            //X axis actual speed
		//"AXIS1_INTERFACE_CONTROL_FACTUALPOSITION",            //X axis actual position
		//"AXIS2_INTERFACE_CONTROL_FACTUALVELOCITY",            //Y axis actual speed
		//"AXIS2_INTERFACE_CONTROL_FACTUALPOSITION",            //Y axis actual position
		//"AXIS3_INTERFACE_CONTROL_FACTUALVELOCITY",            //Z axis actual speed
		//"AXIS3_INTERFACE_CONTROL_FACTUALPOSITION",            //Z axis actual position
  	//"MACHINE_INTERFACE_WVALVESTATE",                      //Valve status
  	//"MACHINE_INTERFACE_WSUCKERSTATE",                     //Cupala status
  	//"MACHINE_INTERFACE_WFIXTURE",                         //Clamp status
  	//"MACHINE_INTERFACE_WPOSTURE",                         //Gesture status
  	//"MACHINE_INTERFACE_DWEXTERNALINPUTSTATE",             //Remote Input status     
  	//"MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE",            //Remote Output status
  	//"MACHINE_INTERFACE_DWINTERNALINPUTSTATE",             //Inner Input status
  	//"MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE",            //inner Output status   
  	//"MACHINE_INTERFACE_DWRESERVED1",                      //sigle states
  	"MACHINE_INTERFACE_MOTORENABLESTATE",                 //motor status
  	"MACHINE_INTERFACE_ERROR_ERROR1_DWALL",               //Controller error
  	//"AXIS1_INTERFACE_COMMU_ERROR",                        //Axis1 error
  	//"AXIS2_INTERFACE_COMMU_ERROR",                        //Axis2 error
  	//"AXIS3_INTERFACE_COMMU_ERROR",                        //Axis3 error
  	//"AXIS4_INTERFACE_COMMU_ERROR",                        //Axis4 error
		"MACHINE_INTERFACE_CONTROL_XAXISPOSITION",
		"MACHINE_INTERFACE_CONTROL_YAXISPOSITION",
		"MACHINE_INTERFACE_CONTROL_ZAXISPOSITION",
		"MACHINE_INTERFACE_CONTROL_X2AXISPOSITION",
		"MACHINE_INTERFACE_CONTROL_Y2AXISPOSITION",				
		"MACHINE_INTERFACE_WOPERATINGSTATE",
		"MACHINE_INTERFACE_WSTEPNUMBER",
		"MACHINE_INTERFACE_PART1_WSTEPNUMBER",
		"MACHINE_INTERFACE_PART2_WSTEPNUMBER",
		"MACHINE_INTERFACE_DWEXTERNALINPUTSTATE",
		"MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE",
		"MACHINE_INTERFACE_DWINTERNALINPUTSTATE",
		"MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE",
		"MACHINE_INTERFACE_MONITOR_RSV03",
		"MACHINE_INTERFACE_MONITOR_RSV04",
		"MACHINE_INTERFACE_MONITOR_RSV05",
		"MACHINE_INTERFACE_MONITOR_RSV06",
		"MACHINE_INTERFACE_MONITOR_RSV07",
		"MACHINE_INTERFACE_CONTROL_RSV00",
		"MACHINE_INTERFACE_CONTROL_RSV01",
		"MACHINE_INTERFACE_CONTROL_RSV02",
		"MACHINE_INTERFACE_CONTROL_RSV03",
		"MACHINE_INTERFACE_CONTROL_RSV04",
		//"MACHINE_INTERFACE_MONITOR_TAKE_TIME",
		//"MACHINE_INTERFACE_MONITOR_RSV08",
		//cjlee add ↓ 2018/12/28 上午 10:32:40
//		"MACHINE_INTERFACE_POSNOW_X1",
//		"MACHINE_INTERFACE_POSNOW_Y1",
//		"MACHINE_INTERFACE_POSNOW_Z",
//		"MACHINE_INTERFACE_POSNOW_X2",
//		"MACHINE_INTERFACE_POSNOW_Y2",
//		"MACHINE_INTERFACE_Ver",
//		"MACHINE_INTERFACE_VerTime",
		//cjlee add ↑
		//"MACHINE_INTERFACE_LASTMODETIME", // 上次週期時間
		//"MACHINE_INTERFACE_MONITORMODES", // 開模數
};

CtmTaskPicker*	CtmTaskPicker::m_pSelf = NULL;
	

CtmTaskPicker::CtmTaskPicker(): CTaskHost()
{
	m_pSelf				= this;
	m_nConnecting		= 1;
	m_nConnectStatus	= 2;	//
	
	m_nMaxElements		= 50;
	m_nMaxLen			= 1460;
	m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME;	//	5s
	m_lOnLineTime		= CNT_INT_CHECK_ONLNE_TIME;
	//m_lUpdateTimer		= 100;	//	update the form view within 100ms
		
	
	//AddCounter(&m_lUpdateTimer, 	m_idMe);
	AddCounter(&m_lCheckQuickTimer, m_idMe);
	AddCounter(&m_lOnLineTime, 		m_idMe);
	//AddCounter(&m_lTestTime,		m_idMe);
	
	memset(m_RxBuffer, 0, sizeof(m_RxBuffer));
	memset(m_TxBuffer, 0, sizeof(m_TxBuffer));
	
	RegisterMessage(MSG_DSP28, m_idMe);
}

CtmTaskPicker::~CtmTaskPicker()
{
	DeleteCounter(&m_lCheckQuickTimer, m_idMe);
	//DeleteCounter(&m_lUpdateTimer, m_idMe);
	DeleteCounter(&m_lOnLineTime, m_idMe);
	
	UnRegisterAllMessage(MSG_DSP28);
}

//	execute it atfer the contructor
int		CtmTaskPicker::CreateSelf()
{
	#ifdef D_ARM
	//usleep(50*1000);
	//system("ifconfig eth0 192.168.0.232");
  //printf(" IP config\n");
	//usleep(50*1000);
	//system("ifconfig eth0 up");
	#endif	
	CTaskHost::CreateSelf();
	m_pData			= new BYTE[m_nMaxLen];
	
	m_bDiagFlag		= FALSE;
	m_nDiagReqNum	= 0;
	
	//m_nWriteDiagFlag= 0;	
	//	create m_pHostProtocol
	m_pHostProtocol	= static_cast<CtmHostProtocol*>(g_pPackage->CreateClassInstance("HostProtocol_picker"));
	
	printf("Create dsp28 OK...\n");

	pthread_attr_init(&m_ThreadAttr);
	pthread_attr_setdetachstate(&m_ThreadAttr, PTHREAD_CREATE_DETACHED);
	pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		
	return 0;
}

//	execute it before the destructor
int		CtmTaskPicker::FreeSelf()
{
	//	destroy m_pHostProtocol
	g_pPackage->ReleaseClassInstance("HostProtocol_picker", m_pHostProtocol);
	
	if (m_nDeviceID	!= -1) 
		DelDevice(m_nDeviceID);
	delete []m_pData;
	return CTaskHost::FreeSelf();
}


void	CtmTaskPicker::_Open(void *pData)
{	
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		;
	}
	
	
	//	init the socket
	m_pSelf->m_nConnecting			= 1;
	m_pSelf->m_ethernet.srcAddr 		= 192 | 168 << 8 | 0 << 16 | 232 << 24;
	m_pSelf->m_ethernet.dstAddr 		= 192 | 168 << 8 | 0 << 16 | 123 << 24;
	//m_pSelf->m_ethernet.srcAddr 		= 192 | 168 << 8 | 3 << 16 | 211 << 24;			//	source ip
	//m_pSelf->m_ethernet.dstAddr 		= 192 | 168 << 8 | 3 << 16 | 123 << 24;			//	dest ip
	m_pSelf->m_ethernet.wsrcPort		= 2320;											//	source port
	m_pSelf->m_ethernet.wdstPort		= 2320;											//	dest port
	m_pSelf->m_ethernet.nDomain 		= AF_INET;
	m_pSelf->m_ethernet.nType   		= SOCK_DGRAM;
	m_pSelf->m_ethernet.wRxBufferSize 	= sizeof(m_pSelf->m_RxBuffer);
	m_pSelf->m_ethernet.fpRxBuffer	  	= m_pSelf->m_RxBuffer;
	m_pSelf->m_ethernet.wTxBufferSize 	= sizeof(m_pSelf->m_TxBuffer);
	m_pSelf->m_ethernet.fpTxBuffer	 	  = m_pSelf->m_TxBuffer;
	
	if (m_pSelf->m_nDeviceID	!= -1) 
		DelDevice(m_pSelf->m_nDeviceID);
		
	m_pSelf->m_nDeviceID	= -1;
	
	
	
	//	set the Rev Function
	while (TRUE)
	{
		if (m_pSelf->m_nDeviceID == -1)
		{
			m_pSelf->m_ethernet.NetFunc		= (NETFUNC)ResValueFunc;		//
			m_pSelf->m_nDeviceID			= AddDevice("CEthernet", &m_pSelf->m_ethernet);

			if(m_pSelf->m_nDeviceID != -1 )	break;
			else					usleep(CNT_INT_CHECK_ONLNE_TIME * 1000);
		}
	}
	
	int		nTime 	= 0;
	int		nNum 	= 0;
	char  szStrID[256];
	int 	nIndex = 0;
	int		nCount = 0;
	DWORD	wDataID = 0;
	DBVALUE	db;
	
	//printf("m_pSelf->m_nConnectStatus = %d\n", m_pSelf->m_nConnectStatus);
	while (m_pSelf->m_nConnectStatus != 0)
	{
		//	7 is LineStatus,	
		nTime = CNT_INT_CHECK_ONLNE_TIME;
		m_pSelf->ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);				//	
	
		//	1 is Actual value
		//printf("req actual \n");
		nTime = 50;	//	the unit is ms, 
		m_pSelf->ReqValues(CONST_REQ_ACTUAL, sizeof(g_Dsp28RealID) / sizeof(g_Dsp28RealID[0]), g_Dsp28RealID, &nTime);	
			
		usleep(500 * 1000);
		//m_pSelf->m_nConnectStatus = 0;
	}

	//	send the moldset and machine data to dsp28 when the network is connected
	//	9 is moldset, the moldset concept is abstract include the mold and machine data
	
//	{ // 開機送初始資料
//	nNum = m_pSelf->GetBlockToDsp28(BLOCK_PICKER_RECIPE_ID, CONST_REQ_MOLDSET);
//	printf("nNum = %d\n",nNum);
//	nNum += m_pSelf->GetBlockToDsp28(BLOCK_PICKER_MACHINE_ID, CONST_REQ_MOLDSET);		
//	m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);	//	tell the moldset & machine sum num to the dsp28 
//	printf("Send init data nNum = %d\n",nNum);
//	}
	m_pSelf->m_nConnecting	= 0;
	
	
	WORD u_PickerState = GetDBValue("MACHINE_INTERFACE_CONTROL_RSV15").lValue;
	printf("STATE=%d\n",u_PickerState);
	// send init data complete
	if(u_PickerState == 3) // 3=手動
		{
//			int init_data_done = 0xFF40;
//			m_pSelf->ReqValues(6, 1, &init_data_done, NULL);
			//usleep(100);
			int manual_state = 0xf300;
			m_pSelf->ReqValues(6, 1, &manual_state, NULL);
			printf("Manaul SwitchKwy\n");
//			int manual_update = 0xFF32;
//			m_pSelf->ReqValues(6, 1, &manual_update, NULL);
//			printf("send init_data_done manual_state\n");
		}
	else
		{
			printf("Not Manaul SwitchKey\n");
//			int init_data_done = 0xFF40;
//			m_pSelf->ReqValues(6, 1, &init_data_done, NULL);
//			printf("send init_data_done none_state\n");
		}
	
	// send 速度設定值
	long lSpeed = GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue;
	if(lSpeed > 0)
	{
		SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lSpeed-5);
		SetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED", lSpeed);
	}
	
	
//calculate and send main program step 
	nCount = 0;
	for(int i = 1; i <= 100; i++)
	{
		sprintf(szStrID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i);
		wDataID = g_pDatabase->GetDataID(szStrID);
		nIndex	=g_pDatabase->DataIDToIndex(wDataID);
		db = g_pDatabase->Read(nIndex);
		//printf("nIndex = %d, wDataID = %d, Value = %d\n", nIndex, wDataID, db.lData);
		if(db.lData != 0)  nCount++;
		else break;	
	}
	wDataID = g_pDatabase->GetDataID("MACHINE_PROFILE_STEPNUM");
	printf("MainCount =%d, wDataID = %d\n", nCount, wDataID);
	g_pDatabase->Write(g_pDatabase->DataIDToIndex(wDataID), &nCount, TRUE);
	
//calculate and send part1 program step 
/*
	nCount = 0;
	for(int i = 1; i <= 50; i++)
	{
		sprintf(szStrID, "MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_STEP", i);
		wDataID = g_pDatabase->GetDataID(szStrID);
		nIndex	=g_pDatabase->DataIDToIndex(wDataID);
		db = g_pDatabase->Read(nIndex);
		//printf("nIndex = %d, wDataID = %d, Value = %d\n", nIndex, wDataID, db.lData);
		if(db.lData != 0)  nCount++;
		else break;	
	}
	wDataID = g_pDatabase->GetDataID("MACHINE_PROFILE_EQUIPMENT1_STEPNUM");
	printf("Part1Count =%d, wDataID = %d\n", nCount, wDataID);
	g_pDatabase->Write(g_pDatabase->DataIDToIndex(wDataID), &nCount, TRUE);
*/
	
	//m_pSelf->WriteValue(CONST_REQ_MOLDSET, sizeof(u_pszDBStrID)/sizeof(char*) , u_pszDBStrID); /*cjlee 註解 2020/4/1 下午 03:01:01*/

}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
 *	\brief	向主機請求數據(發送請求的信息給主機)
 *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息(一般狀況下，是cycle Time)
 *	\return 錯誤數
 */
int		CtmTaskPicker::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
{
	int nResult = 0;
	
	if (m_pHostProtocol != NULL)
	{
		if (nElements > 0)
		{
			int*	pIndex = new int[nElements];
			char 	cszStrID[256];
			memset(pIndex, 0, sizeof(int) * nElements);
			
			for (int i = 0; i < nElements; i++)
			{
				if(pszIDs[i] != NULL)			//James add 2008/5/7 05:47下午
				{
					memset(cszStrID, 0, sizeof(cszStrID));
					Trim(cszStrID, pszIDs[i], sizeof(cszStrID));
					pIndex[i] = g_pDatabase->GetDataID(cszStrID);
				}
				//printf("Rcv[%d] = %d\n",i,pIndex[i]);
			}
			
			nResult = ReqValues(nType, nElements, pIndex, pEx);
			
			delete []pIndex;
		}
	}
		
	return nResult;
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	向主機請求數據(發送請求的信息給主機)(此請求信息有可能是寫、也有可能是讀)
 *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pIndexs		請求數據的ID(int),根據實際需求決定是Index還是DataID
 *	\param	[in]	pEx			附加信息(一般狀況下，是cycle Time)
 *	\return 錯誤數
 */
int		CtmTaskPicker::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
{
	//static	int nPackageNum = 0;
	//printf("nType = %d\n", nType);
	if (m_pHostProtocol != NULL)
	{
		//char	szName[128];
		int	nNum = nElements / m_nMaxElements ;
		int	nLen = 0;
		
		for (int i = 0; i < nNum; i++)
		{
			nLen 	= m_pHostProtocol->Compose(nType, m_pData, pIndexs, m_nMaxElements, pEx);
			pIndexs = pIndexs + m_nMaxElements;
			
			if (nLen > 0 && m_nDeviceID >= 0)
			{			
				SendData(m_nDeviceID, (char*)m_pData, nLen);
				usleep(30 * 1000);
			}		
		}
		
		int	nLast = nElements - nNum * m_nMaxElements;
		
		if (nLast > 0)
		{
			nLen = m_pHostProtocol->Compose(nType, m_pData,  pIndexs, nLast,pEx);
			//printf("nType=%d, nLen=%d, pIndexs[0]=%d\n",nType,nLen,pIndexs[0]);
			if (nLen > 0 && m_nDeviceID >= 0)
			{
			  //if(nType==CONST_REQ_COMMAND)
			  //{
    			//for(int i=0; i<nLen; i++)
    			//	printf(" %02x ", m_pData[i]);
    			//printf("\n");
    		  //}			
//			  if(nType==CONST_REQ_WRITEMOTOR)
//			  {
//    			for(int i=0; i<nLen; i++)
//    				printf(" %02x ", m_pData[i]);
//    			printf("\n");
//    		}		
				SendData(m_nDeviceID, (char*)m_pData, nLen);
				usleep(30 * 1000);
			}
			else
				g_tmDebugInfo->PrintDebugInfo("nLen =%d, Device id =%d\n", nLen, m_nDeviceID);
		}
	}
		
	return 0;
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, char* pszID, void* pEx)
 *	\brief	向主機請求數據(發送請求的信息給主機)(此請求信息有可能是寫、也有可能是讀)
 *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	pszID		數據ID
 *	\param	[in]	pEx			附加信息(一般狀況下，是cycle Time)
 *	\return 錯誤數
 */
int		CtmTaskPicker::ReqValues(int nType, char* pszID, void* pEx)
{

	if (m_pHostProtocol != NULL)
	{
		char 	cszStrID[256];
		
		memset(cszStrID, 0, sizeof(cszStrID));
		Trim(cszStrID, pszID, sizeof(cszStrID));		
		int nIndex 	= g_pDatabase->GetDataID(cszStrID);
		int	nLen 	= m_pHostProtocol->Compose(nType, m_pData, &nIndex, 1, pEx);
		if (nLen > 0 && m_nDeviceID >= 0)
		{
			//g_tmDebugInfo->LogData(szName, m_pData, nLen);
			SendData(m_nDeviceID, (char*)m_pData, nLen);
			usleep(20 * 1000);
		}
	}
	return 0;
}

int 	CtmTaskPicker::ReqValues(int nType, void* pOutData, int nSize)
{
    return m_pHostProtocol->Compose(nType, pOutData, NULL, nSize, NULL);
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
 *	\brief	停止向主機請求數據
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskPicker::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
{
	return 	StopReq(nType, NULL);	
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	停止向主機請求數據
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pIndexs		請求數據的ID
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
 
int		CtmTaskPicker::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
{
	return 	StopReq(nType, NULL);
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	停止向主機請求數據
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskPicker::StopReq(int nType, char* pszID, void* pEx)
{
	int nLen = m_pHostProtocol->Compose(nType, m_pData, NULL, 0, pEx);
	if (nLen > 0 && m_nDeviceID >= 0)
	{		
		SendData(m_nDeviceID, (char*)m_pData, nLen);
		usleep(20 * 1000);
	}
	return 0;
}

/*!	\fn int	CtmHmiPackage::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
 *	\brief	寫數據到主機(利用ReqValues是相同的道理)
  *	\param	[in]	nType		請求何種類型的數據
 *	\param	[in]	nElements	請求數據的數目
 *	\param	[in]	pszIDs		請求數據的ID(String類型)
 *	\param	[in]	pEx			附加信息
 *	\return 錯誤數
 */
int		CtmTaskPicker::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
{
	return ReqValues(nType, nElements, pszIDs, pEx);
}

int		CtmTaskPicker::WriteValue(int nType, int nElements, int pIndexs[], void* pEx)
{
	return ReqValues(nType, nElements, pIndexs, pEx);
}

int		CtmTaskPicker::WriteValue(int nType, char* pszID, void* pEx)
{
	if(nType == 9)
		{
			int	nNum = 0;
			nNum = m_pSelf->GetBlockToDsp28(BLOCK_PICKER_RECIPE_ID, CONST_REQ_MOLDSET);
			//printf("nNum =%d\n",nNum);
			nNum += m_pSelf->GetBlockToDsp28(BLOCK_PICKER_MACHINE_ID, CONST_REQ_MOLDSET);		
			m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);			
		}
	else
	return ReqValues(nType, pszID, pEx);
}

int		CtmTaskPicker::WriteValue(int nDataID)
{
	int	nTempID = nDataID;
	
	return WriteValue(3, 1, &nTempID);
}

int		CtmTaskPicker::WriteValue(char* pszID)
{
	return WriteValue(3, 1, &pszID);
}

int 	CtmTaskPicker::WriteValue(int nType, void* pInputData, int nSize)
{
	
	if(nType == COMM_ETH_DEVICEID)		//daniel add 2010/01/29 更新前傳入選擇的DEVICEID
	{
		nType = COMM_ETH_SELECT28;
		pInputData = (void *)&m_pSelf->m_nDeviceID;	
	}
    return m_pHostProtocol->Compose(nType, NULL, pInputData, nSize, NULL);
}

/*!	\fn int	CtmTaskPicker::GetData(void* pdata, int nLen)
 *	\brief	接收發送過來的數據
 *	\param	[in]	pdata	接收的數據
 *	\param	[in]	nLen	接收的數據長度
 *	\return CTaskHost::GetData(pdata, nLen)
 */
int		CtmTaskPicker::GetData(void* pdata, int nLen)
{
	int nGetType = m_pHostProtocol->GetType(pdata, nLen);
	int	nNum = 0;
	switch(nGetType)
	{
		case CONST_RES_MOLDSETSTATUS:	//接收dsp28要求發送模組數據的請求
			nNum = g_pBlock->GetBlockData(BLOCK_PICKER_RECIPE_ID)->dwFactorTotal;
			nNum += g_pBlock->GetBlockData(BLOCK_PICKER_MACHINE_ID)->dwFactorTotal;
			if (nNum != *(long*)((BYTE*)pdata + 12))
			{	
				g_tmDebugInfo->PrintDebugInfo("Mold Num Error! Send=%d, Rev=%d\n",
					nNum, 	*(long*)((BYTE*)pdata + 12));
				nNum = GetBlockToDsp28(BLOCK_PICKER_RECIPE_ID, 9);
				nNum += GetBlockToDsp28(BLOCK_PICKER_MACHINE_ID, 9);
					
				m_pSelf->ReqValues(8, 1, &nNum, NULL);	//	tell the moldset & machine sum num to the dsp28 
			}
			return 0;
			break;
		case CONST_RES_ACTUAL:			//接收dsp28及時數據 每50ms主機發送一次，由此來決定連線是否正常
			m_pSelf->m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME;
			m_pSelf->m_nConnectStatus 	= 0;
			m_pHostProtocol->Parse(pdata, nLen);
			//printf("get echo\n");
			break;
		default:
			if (m_bDiagFlag)
			{
				//	change the data type in the pdata Buffer			
				nGetType = nGetType | 0x8000;
				*(WORD*)((BYTE*)pdata + 4) = (WORD)nGetType;
			}
			
			m_pHostProtocol->Parse(pdata, nLen);
			break;	
	}
			//printf("111 get echo\n");
	
	return CTaskHost::GetData(pdata, nLen);
}

/*!	\fn int	CtmTaskPicker::ResValueFunc(char *pData, int nLength)
 *	\brief	接收主機發送過來的數據(采用輪回方式接收數據)
 *	\param	[in]	pdata	接收的數據
 *	\param	[in]	nLen	接收的數據長度
 *	\return 
 */
void	CtmTaskPicker::ResValueFunc(char *pData, int nLength)
{
	m_pSelf->GetData(pData, nLength);
	
	//James add 2008/5/7 01:05下午	
	//if (g_ptaskCmd->GetFocus() != NULL) 
	//	g_ptaskCmd->GetFocus()->Update();
	MSG		msg;
	msg.message	= MSG_DSP28;
	g_pApplication->QueueMessage(&msg);
}

int		CtmTaskPicker::SendZeroSet(int nZeroType)
{
	SetDBValue("MACH_OTHR_OTHR_NULL_W0339", GetDBValue("MACH_OTHR_OTHR_NULL_W0337").lValue, TRUE);
	return 0;
}

/*!	\fn int	CtmTaskPicker::GetOnLineStatus()
 *	\brief	判斷連線是否正常		
 *	\return 連線狀態
 */
int		CtmTaskPicker::GetOnLineStatus()
{
	if (m_lCheckQuickTimer <= 0)
	{
		//m_nConnectStatus = 1 表示重新鏈接  =2表示斷線狀態 =0表示連接上
		m_nConnectStatus = m_nConnectStatus | 1;
		BOOL	bConnect	= FALSE;
		//printf("m_nConnecting = %d GetState(m_nDeviceID) = %d\n",m_nConnecting,GetState(m_nDeviceID));
		
		if ((GetState(m_nDeviceID) != DEVICE_S_OPENED) &&
			(m_nConnecting != 1))
		{
			m_nConnecting	= 1;
			m_nConnectStatus = m_nConnectStatus | 2;
			bConnect		= TRUE;
			pthread_cancel(m_OpenThreadID);
		//	printf("1 open ethernet\n");
			pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		}
		
		if ((!bConnect) && (m_nConnecting != 1))
		{
			m_nConnecting	= 1;
			m_nConnectStatus = m_nConnectStatus | 2;
			pthread_cancel(m_OpenThreadID);
		//	printf("2 open ethernet\n");
			pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
		}
		m_lCheckQuickTimer = CNT_INT_CHECK_ONLNE_TIME;
		
	} 
	
	return m_nConnectStatus;
	
}


void	CtmTaskPicker::Run()
{
	//if (m_lUpdateTimer <= 0 && m_nConnectStatus == 0)
	//{
	//	if (g_ptaskCmd->GetFocus() != NULL) 	g_ptaskCmd->GetFocus()->Update();
	//	m_lUpdateTimer = 100;		
	//}
	//SetMeter();
	//判斷連線狀態： 如果5S沒有收到dsp28的及時數據，說明是斷線，就需要重新鏈接
	m_nConnectStatus = GetOnLineStatus();
	if(m_nConnectStatus != 0)
	{
		m_nConnecting	= 0;
	}
//	printf("m_nConnectStatus = %d\n",m_nConnectStatus);


	if (m_lOnLineTime <= 0)
	{
		if (0 == m_nConnectStatus)	//	僅在連線的狀態下，才會發送LinkStatus
		{
			int	nTime = CNT_INT_CHECK_ONLNE_TIME;
			ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);			//發送連線是否正常數據包
	//		printf(" req online status\n");
		}
		
		m_lOnLineTime = CNT_INT_CHECK_ONLNE_TIME;
	}
}
		
/*!	\fn int	CtmTaskPicker::GetBlockToDsp28(WORD wIndex, int nType)
 *	\brief	發送指定Block數據給主機		
 *	\param	[in]	wIndex	Block類型
 *	\param	[in]	nType	數據類型(協議定的)
 *	\return 連線狀態
 */
int		CtmTaskPicker::GetBlockToDsp28(WORD wIndex, int nType)
{
	BLOCKDATA*		pBlockData = NULL;
	if ((pBlockData	= g_pBlock->GetBlockData(wIndex)) == NULL)	return 0;
		
	if (pBlockData->dwFactorTotal == 0) 						return 0;
		
	int* pIndexs = new int[pBlockData->dwFactorTotal];
	char	szName[128];
	
	sprintf(szName, "Block%d.txt", wIndex);
	
	for (int i = 0; i < (int)pBlockData->dwFactorTotal; i++)
	{
		pIndexs[i] = g_pDatabase->IndexToDataID(pBlockData->pwDBIndex[i]);
		g_tmDebugInfo->LogDebugInfo(szName, "wIndex = %d, ReqType = %d, ID = %3x, Index = %3x", 
											wIndex, nType, pIndexs[i], pBlockData->pwDBIndex[i]);
	}
	
	g_tmDebugInfo->LogDebugInfo(szName, "total=%d\n", pBlockData->dwFactorTotal);
	
	WriteValue(nType, pBlockData->dwFactorTotal, pIndexs, NULL);
	int nResult = pBlockData->dwFactorTotal;
	delete []pIndexs;
	
	return nResult;
	
}

void	CtmTaskPicker::StartDiag()
{
	m_bDiagFlag = TRUE;
}

void	CtmTaskPicker::EndDiag()
{
	m_bDiagFlag = FALSE;
}




int		CtmTaskPicker::ReqValuesDiag(int pIDs[], int nElements, int nDiagType)
{
	int	nType	= nDiagType | 0x8000;
	int	nResult	= 0;
	if (nElements > 0)
	{
		nResult		= ReqValues(nType, nElements, pIDs, NULL);
	}
	return nResult;
}

int		CtmTaskPicker::ReqValuesDiag(int nStartID, int nElements, int nDiagType)
{
	int	nResult	= 0;
	if (nElements > 0 && nStartID >= 0)
	{
		int*	pIDs = new int[nElements];
		
		for (int i = 0; i < nElements; i++)
		{
			pIDs[i] = nStartID + i;
		}
		
		nResult = ReqValuesDiag(pIDs, nElements, nDiagType);
		delete []pIDs;
	}
	
	return nResult;
	
}


int		CtmTaskPicker::WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType)
{
	int	nType	= nDiagType | 0x8000;
	int	nResult	= 0;
	if (nElements > 0)
	{
		nResult		= ReqValues(nType, nElements, pIDs, pValues);
	}
	return nResult;
}
		
		
int		CtmTaskPicker::WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType)
{
	int	nResult	= 0;
	if (nElements > 0 && nStartID >= 0)
	{
		int*	pIDs = new int[nElements];
		
		for (int i = 0; i < nElements; i++)
		{
			pIDs[i] = nStartID + i;
		}
		
		WriteValuesDiag(pIDs, pValues, nElements, nDiagType);
		delete []pIDs;
	}
	
	return nResult;
}
		

#ifndef	D_BEOBJECT_CTMTASKPICKER
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskpicker == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskHost*>(pResult))->CreateSelf();
		g_ptaskpicker = static_cast<CTaskHost*>(pResult);
	}
	
	return g_ptaskpicker;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskHost*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
