#include	"../../package.h"
#include	"../../timer.h"
#include	"../../main.h"
#include	"../../tmdebug.h"
#include	"../../init.h"
#include	"../../utils.h"
#include	"../../ethnet.h"
#include	"../libComm.h"
#include	"tmTaskDsp28.h"

#include 	<netinet/ip_icmp.h> 
#include 	<sys/socket.h>
#include 	<arpa/inet.h>

IMPLEMENT_DYNCREATE(CtmTaskDsp28, CTaskHost)
typedef void *(* PRSRUN)(void *);

long	CNT_INT_CHECK_ONLNE_TIME =10000;

char*	g_Dsp28RealID[] =
{
	"MACH_OTHR_OTHR_NULL_W0337",
	"MACH_OTHR_OTHR_NULL_W0338",
	
	"MACH_OTHR_OTHR_NULL_W0339",
	"MACH_OTHR_OTHR_NULL_W0377",		//HTK530 Mold Real Pos
	"MACH_OTHR_OTHR_NULL_W037E"			//HTK530 Mold Real Speed
	//"MACH_OTHR_OTHR_NULL_W034E",
	//"MACH_OTHR_OTHR_NULL_W034F"
};


CtmTaskDsp28*	CtmTaskDsp28::m_pSelf = NULL;
WORD			CtmTaskDsp28::m_wSourceIPPort[] = {169,254,11,10,2310};


CtmTaskDsp28::CtmTaskDsp28(): CTaskHost()
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

CtmTaskDsp28::~CtmTaskDsp28()
{
	DeleteCounter(&m_lCheckQuickTimer, m_idMe);
	//DeleteCounter(&m_lUpdateTimer, m_idMe);
	DeleteCounter(&m_lOnLineTime, m_idMe);
	
	UnRegisterAllMessage(MSG_DSP28);
}

//	execute it atfer the contructor
int		CtmTaskDsp28::CreateSelf()
{
	#ifdef D_ARM
	usleep(50*1000);
	char pszEthCmd[64];
	memset(&pszEthCmd,0,sizeof(pszEthCmd));
	if(CtmConfig::GetInstance()->GetDSP28Flag())
		system("ifconfig eth1:0 169.254.11.18");
	else
		system("ifconfig eth0:0 169.254.11.18");
		usleep(50*1000);
	system("ifconfig eth0 up");
	#endif
	CTaskHost::CreateSelf();
	m_pData			= new BYTE[m_nMaxLen];
	
	m_bDiagFlag		= FALSE;
	m_nDiagReqNum	= 0;
	//m_nWriteDiagFlag= 0;	
	//	create m_pHostProtocol
	m_pHostProtocol	= static_cast<CtmHostProtocol*>(g_pPackage->CreateClassInstance("HostProtocol"));
	pthread_attr_init(&m_ThreadAttr);
	pthread_attr_setdetachstate(&m_ThreadAttr, PTHREAD_CREATE_DETACHED);
	pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
	return 0;
}

//	execute it before the destructor
int		CtmTaskDsp28::FreeSelf()
{
	//	destroy m_pHostProtocol
	if(1==CtmConfig::GetInstance()->GetDSP28Flag())
		g_pPackage->ReleaseClassInstance("HostProtocol", m_pHostProtocol);
	else
		system("ifconfig eth0:0 down");
	usleep(50*1000);
	if (m_nDeviceID	!= -1) 
		DelDevice(m_nDeviceID);
	delete []m_pData;
	return CTaskHost::FreeSelf();
}


void	CtmTaskDsp28::_Open(void *pData)
{	
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		
	}
	
	//printf("1=%d, 2=%d, 3=%d, 4=%d, 5=%d \n", m_wSourceIPPort[0], m_wSourceIPPort[1],
				//m_wSourceIPPort[2], m_wSourceIPPort[3], m_wSourceIPPort[4]);
	//	init the socket
	m_pSelf->m_nConnecting				= 1;
	// m_pSelf->m_ethernet.srcAddr 		= 192 | 168 << 8 | 0 << 16 | 210 << 24;
	//m_pSelf->m_ethernet.dstAddr 		= 192 | 168 << 8 | 0 << 16 | 123 << 24;
	m_pSelf->m_ethernet.srcAddr 		       = 169 | 254 << 8 | 11 << 16 | 18 << 24;					//	source ip
	m_pSelf->m_ethernet.dstAddr 		= m_wSourceIPPort[0] | m_wSourceIPPort[1] << 8 | 		
										m_wSourceIPPort[2] << 16 | m_wSourceIPPort[3] << 24;	//	dest ip James modify 2010/5/10 
	m_pSelf->m_ethernet.wsrcPort		= 2310;											//	source port
	m_pSelf->m_ethernet.wdstPort		= m_wSourceIPPort[4];											//	dest port
	m_pSelf->m_ethernet.nDomain 		= AF_INET;
	m_pSelf->m_ethernet.nType   		= SOCK_DGRAM;
	m_pSelf->m_ethernet.wRxBufferSize 	= sizeof(m_pSelf->m_RxBuffer);
	m_pSelf->m_ethernet.fpRxBuffer	 	= m_pSelf->m_RxBuffer;
	m_pSelf->m_ethernet.wTxBufferSize 	= sizeof(m_pSelf->m_TxBuffer);
	m_pSelf->m_ethernet.fpTxBuffer	 	= m_pSelf->m_TxBuffer;
	
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
			
			//printf("m_pSelf->m_nDeviceID=%d \n", m_pSelf->m_nDeviceID);
			if(m_pSelf->m_nDeviceID != -1 )	break;
			else		usleep(CNT_INT_CHECK_ONLNE_TIME * 1000);
		}
	}
	
	//Send ICMP
	m_pSelf->OpenICMPDevice();
	
//	printf("m_pSelf->m_nConnectStatus=%d \n", m_pSelf->m_nConnectStatus);
//	g_nUpdateDevice = m_pSelf->m_nDeviceID;
	int	nTime 	= 0;
	int	nNum 	= 0;
	if(CtmConfig::GetInstance()->GetDSP28Flag())
	{
		while (m_pSelf->m_nConnectStatus != 0)
		{
			//	7 is LineStatus,	
			nTime = CNT_INT_CHECK_ONLNE_TIME;
			m_pSelf->ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);				//	
		
			// Actual value
			nTime = 1000;	//	the unit is ms, 
			m_pSelf->ReqValues(CONST_REQ_ACTUAL, sizeof(g_Dsp28RealID) / sizeof(g_Dsp28RealID[0]), g_Dsp28RealID, &nTime);	
			usleep(500 * 1000);
			//m_pSelf->m_nConnectStatus = 0;
		}
		//	send the moldset and machine data to dsp28 when the network is connected
		//	9 is moldset, the moldset concept is abstract include the mold and machine data
			nNum = m_pSelf->GetBlockToDsp28(BLOCK_MOLDSET_ID, CONST_REQ_MOLDSET);
	    	nNum += m_pSelf->GetBlockToDsp28(BLOCK_MACHINE_ID, CONST_REQ_MOLDSET);
			m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);	//	tell the moldset & machine sum num to the dsp28 
			m_pSelf->m_nConnecting	= 0;
	}
	//m_pSelf->ReqValues(COMM_ETH_SENDDLSTART, 1, &nNum, NULL);
	
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
 *	\brief	�V�D���ШD�ƾ�(�o�e�ШD���H�����D��)
 *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	nElements	�ШD�ƾڪ��ƥ�
 *	\param	[in]	pszIDs		�ШD�ƾڪ�ID(String����)
 *	\param	[in]	pEx			���[�H��(�@�몬�p�U�A�Ocycle Time)
 *	\return ���~��
 */
int		CtmTaskDsp28::ReqValues(int nType, int nElements, char* pszIDs[], void* pEx)
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
				if(pszIDs[i] != NULL)			//James add 2008/5/7 05:47�U��
				{
					memset(cszStrID, 0, sizeof(cszStrID));
					Trim(cszStrID, pszIDs[i], sizeof(cszStrID));
					pIndex[i] = g_pDatabase->GetDataID(cszStrID);
				}
			}
			
			nResult = ReqValues(nType, nElements, pIndex, pEx);
			
			delete []pIndex;
		}
	}
		
	return nResult;
}

/*!	\fn int	CtmHmiPackage::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	�V�D���ШD�ƾ�(�o�e�ШD���H�����D��)(���ШD�H�����i��O�g�B�]���i��OŪ)
 *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	nElements	�ШD�ƾڪ��ƥ�
 *	\param	[in]	pIndexs		�ШD�ƾڪ�ID(int),�ھڹ�ڻݨD�M�w�OIndex�٬ODataID
 *	\param	[in]	pEx			���[�H��(�@�몬�p�U�A�Ocycle Time)
 *	\return ���~��
 */
int		CtmTaskDsp28::ReqValues(int nType, int nElements, int pIndexs[], void* pEx)
{
	//printf("pIndex[i]=%d\n",pIndexs[0]);
	//static	int nPackageNum = 0;
	//printf("ReqValues nType=%d m_nDeviceID=%d \n", nType, m_nDeviceID);
	//m_pHostProtocol, nElements, m_nMaxElements);
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
			if (nLen > 0 && m_nDeviceID >= 0)
			{
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
 *	\brief	�V�D���ШD�ƾ�(�o�e�ШD���H�����D��)(���ШD�H�����i��O�g�B�]���i��OŪ)
 *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	pszID		�ƾ�ID
 *	\param	[in]	pEx			���[�H��(�@�몬�p�U�A�Ocycle Time)
 *	\return ���~��
 */
int		CtmTaskDsp28::ReqValues(int nType, char* pszID, void* pEx)
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

int 	CtmTaskDsp28::ReqValues(int nType, void* pOutData, int nSize)
{
    return m_pHostProtocol->Compose(nType, pOutData, NULL, nSize, NULL);
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
 *	\brief	����V�D���ШD�ƾ�
  *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	nElements	�ШD�ƾڪ��ƥ�
 *	\param	[in]	pszIDs		�ШD�ƾڪ�ID(String����)
 *	\param	[in]	pEx			���[�H��
 *	\return ���~��
 */
int		CtmTaskDsp28::StopReq(int nType, int nElements, char* pIDs[], void* pEx)
{
	return 	StopReq(nType, NULL);
	
}

/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	����V�D���ШD�ƾ�
  *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	nElements	�ШD�ƾڪ��ƥ�
 *	\param	[in]	pIndexs		�ШD�ƾڪ�ID
 *	\param	[in]	pEx			���[�H��
 *	\return ���~��
 */
int		CtmTaskDsp28::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
{
	return 	StopReq(nType, NULL);
}


/*!	\fn int	CtmHmiPackage::StopReq(int nType, int nElements, int pIndexs[], void* pEx)
 *	\brief	����V�D���ШD�ƾ�
  *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	pszIDs		�ШD�ƾڪ�ID(String����)
 *	\param	[in]	pEx			���[�H��
 *	\return ���~��
 */
int		CtmTaskDsp28::StopReq(int nType, char* pszID, void* pEx)
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
 *	\brief	�g�ƾڨ�D��(�Q��ReqValues�O�ۦP���D�z)
  *	\param	[in]	nType		�ШD����������ƾ�
 *	\param	[in]	nElements	�ШD�ƾڪ��ƥ�
 *	\param	[in]	pszIDs		�ШD�ƾڪ�ID(String����)
 *	\param	[in]	pEx			���[�H��
 *	\return ���~��
 */
int		CtmTaskDsp28::WriteValue(int nType, int nElements, char* pszIDs[], void* pEx)
{
	return ReqValues(nType, nElements, pszIDs, pEx);
}

int		CtmTaskDsp28::WriteValue(int nType, int nElements, int pIndexs[], void* pEx)
{
	return ReqValues(nType, nElements, pIndexs, pEx);
}

int		CtmTaskDsp28::WriteValue(int nType, char* pszID, void* pEx)
{ 
if(nType == 9)
		{
			int	nNum = 0;
			nNum = m_pSelf->GetBlockToDsp28(BLOCK_MOLDSET_ID, CONST_REQ_MOLDSET);
			nNum += m_pSelf->GetBlockToDsp28(BLOCK_MACHINE_ID, CONST_REQ_MOLDSET);		
			m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);			
		}
	else
	return ReqValues(nType, pszID, pEx);
}

int		CtmTaskDsp28::WriteValue(int nDataID)
{
	int	nTempID = nDataID;
	
	return WriteValue(3, 1, &nTempID);
}

int		CtmTaskDsp28::WriteValue(char* pszID)
{
	return WriteValue(3, 1, &pszID);
}

int 	CtmTaskDsp28::WriteValue(int nType, void* pInputData, int nSize)
{
	WORD	wCmdType = 0;
	if(nType == COMM_ETH_DEVICEID)		//daniel add 2010/01/29 ��s�e�ǤJ��ܪ�DEVICEID
	{
		wCmdType = *(WORD*)pInputData;
		//printf("CtmTaskDsp28::WriteValue()   wCmdType=%d\n", wCmdType);
		if(wCmdType == 0)
			nType = COMM_ETH_SELECT28;
		else if(wCmdType == 1)
			nType = COMM_UPDATEDEVICE_DSP54TRANSMIT;
		else
			nType = COMM_ETH_SELECT28;
		
		pInputData = (void *)&m_pSelf->m_nDeviceID;	
	}
	else if(nType == COMM_SETDSP28IP)	//James add 2010/5/10 for reset dsp28 IP and Port
	{
		if(nSize == 0)
		{
			memcpy(&m_wSourceIPPort, pInputData, sizeof(m_wSourceIPPort));
			RestartNet(TRUE);
		}
		else
		{
			if(nSize <= sizeof(m_wSourceIPPort))
			{
				memcpy(&m_wSourceIPPort, pInputData, nSize);
				RestartNet(TRUE);
			}
		}
	}
	else if(nType == COMM_ETH_SENDDLSTART)		//James add ����s���A�A�N�s�u�ɶ����� 2010/5/18
	{
		CNT_INT_CHECK_ONLNE_TIME = 15000;		
	}
    return m_pHostProtocol->Compose(nType, NULL, pInputData, nSize, NULL);
}

/*!	\fn int	CtmTaskDsp28::GetData(void* pdata, int nLen)
 *	\brief	�����o�e�L�Ӫ��ƾ�
 *	\param	[in]	pdata	�������ƾ�
 *	\param	[in]	nLen	�������ƾڪ���
 *	\return CTaskHost::GetData(pdata, nLen)
 */
int		CtmTaskDsp28::GetData(void* pdata, int nLen)
{
	int nGetType = m_pHostProtocol->GetType(pdata, nLen);
	int	nNum = 0;
	switch(nGetType)
	{
		case CONST_RES_MOLDSETSTATUS:	//����dsp28�n�D�o�e�Ҳռƾڪ��ШD
			nNum = g_pBlock->GetBlockData(BLOCK_MOLDSET_ID)->dwFactorTotal;
			nNum += g_pBlock->GetBlockData(BLOCK_MACHINE_ID)->dwFactorTotal;
			if (nNum != *(long*)((BYTE*)pdata + 12))
			{	
				g_tmDebugInfo->PrintDebugInfo("Mold Num Error! Send=%d, Rev=%d\n",
					nNum, 	*(long*)((BYTE*)pdata + 12));
				nNum = GetBlockToDsp28(BLOCK_MOLDSET_ID, 9);
				nNum += GetBlockToDsp28(BLOCK_MACHINE_ID, 9);
					
				m_pSelf->ReqValues(CONST_REQ_MOLDSETSTATUS, 1, &nNum, NULL);	//	tell the moldset & machine sum num to the dsp28 
			}
			return 0;
			break;
		case CONST_RES_ACTUAL:			//����dsp28�ήɼƾ� �C50ms�D���o�e�@���A�Ѧ��ӨM�w�s�u�O�_���`
			m_pSelf->m_lCheckQuickTimer	= CNT_INT_CHECK_ONLNE_TIME;
			m_pSelf->m_nConnectStatus 	= 0;
			m_pHostProtocol->Parse(pdata, nLen);
			break;
		case CONST_RES_LINESTATUS:
			//printf("lineStatus...\n");
			m_pSelf->m_nConnectStatus=0;
			break;
		case COMM_ECHO_SUCCESS:
			m_pHostProtocol->Parse(pdata, nLen);
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

	
	return CTaskHost::GetData(pdata, nLen);
}

/*!	\fn int	CtmTaskDsp28::ResValueFunc(char *pData, int nLength)
 *	\brief	�����D���o�e�L�Ӫ��ƾ�(���ν��^�覡�����ƾ�)
 *	\param	[in]	pdata	�������ƾ�
 *	\param	[in]	nLen	�������ƾڪ���
 *	\return 
 */
void	CtmTaskDsp28::ResValueFunc(char *pData, int nLength)
{
	//printf("ResValueFunc nLength=%d \n", nLength);
	m_pSelf->GetData(pData, nLength);
	
	//James add 2008/5/7 01:05�U��	
	//if (g_ptaskCmd->GetFocus() != NULL) 
	//	g_ptaskCmd->GetFocus()->Update();
	MSG		msg;
	msg.message	= MSG_DSP28;
	g_pApplication->QueueMessage(&msg);
}

int		CtmTaskDsp28::SendZeroSet(int nZeroType)
{
	SetDBValue("MACH_OTHR_OTHR_NULL_W0339", GetDBValue("MACH_OTHR_OTHR_NULL_W0337").lValue, TRUE);
	
	
	
	
	return 0;
}

/*!	\fn int	CtmTaskDsp28::GetOnLineStatus()
 *	\brief	�P�_�s�u�O�_���`		
 *	\return �s�u���A
 */
int		CtmTaskDsp28::GetOnLineStatus()
{
	if(1==CtmConfig::GetInstance()->GetDSP28Flag())
	{
		if (m_lCheckQuickTimer <= 0)
		{
			//m_nConnectStatus = 1 ��ܭ��s�챵  =2����_�u���A =0��ܳs���W
			//printf("Status=%d\n",m_nConnectStatus);
			m_nConnectStatus = m_nConnectStatus | 1;
			BOOL	bConnect	= FALSE;
			//printf("GetState(m_nDeviceID)=%d,%d\n",GetState(m_nDeviceID),DEVICE_S_OPENED);
			if ((GetState(m_nDeviceID) != DEVICE_S_OPENED) &&
				(m_nConnecting != 1))
			{
				m_nConnecting	= 1;
				m_nConnectStatus = m_nConnectStatus | 2;
				bConnect		= TRUE;
				pthread_cancel(m_OpenThreadID);
				pthread_join(m_OpenThreadID,NULL);
				//printf("checkonline1...\n");
				pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
			}
			//printf("bConnect=%d,m_nConnecting=%d\n",bConnect,m_nConnecting);
			if ((!bConnect) && (m_nConnecting != 1))
			{
				m_nConnecting	= 1;
				m_nConnectStatus = m_nConnectStatus | 2;
				pthread_cancel(m_OpenThreadID);
				pthread_join(m_OpenThreadID,NULL);
				//printf("checkonline2...\n");
				pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
			}
			m_lCheckQuickTimer = CNT_INT_CHECK_ONLNE_TIME;
			
		} 
	}
	//printf("m_nConnectStatus=%d\n",m_nConnectStatus);
	return m_nConnectStatus;
}

/*!	\fn int	CtmTaskDsp28::RestartNet(BOOL bFlag)
 *	\brief	���s�Ұʺ���		
 *	\return �s�u���A
 */
BOOL	CtmTaskDsp28::RestartNet(BOOL bFlag)
{
	if(bFlag)
	{
		//printf("restart...\n");
		m_nConnectStatus	= 2;
		pthread_cancel(m_OpenThreadID);
		pthread_join(m_OpenThreadID,NULL);
		pthread_create(&m_OpenThreadID, NULL, (PRSRUN)_Open, NULL);
	}
	else
	{
		;	
	}
}


void	CtmTaskDsp28::Run()
{
	//if (m_lUpdateTimer <= 0 && m_nConnectStatus == 0)
	//{
	//	if (g_ptaskCmd->GetFocus() != NULL) 	g_ptaskCmd->GetFocus()->Update();
	//	m_lUpdateTimer = 100;		
	//}
	//printf("Run...\n");
	//�P�_�s�u���A�G �p�G5S�S������dsp28���ήɼƾڡA�����O�_�u�A�N�ݭn���s�챵
	m_nConnectStatus = GetOnLineStatus();
	
	if (m_lOnLineTime <= 0)
	{
		if (0 == m_nConnectStatus)	//	�Ȧb�s�u�����A�U�A�~�|�o�eLinkStatus
		{
			int	nTime = CNT_INT_CHECK_ONLNE_TIME;
			ReqValues(CONST_REQ_LINESTATUS, 1, &nTime, NULL);			//�o�e�s�u�O�_���`�ƾڥ]
		}
		
		m_lOnLineTime = CNT_INT_CHECK_ONLNE_TIME;
	}
}
		
/*!	\fn int	CtmTaskDsp28::GetBlockToDsp28(WORD wIndex, int nType)
 *	\brief	�o�e���wBlock�ƾڵ��D��		
 *	\param	[in]	wIndex	Block����
 *	\param	[in]	nType	�ƾ�����(��ĳ�w��)
 *	\return �s�u���A
 */
int		CtmTaskDsp28::GetBlockToDsp28(WORD wIndex, int nType)
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

void	CtmTaskDsp28::StartDiag()
{
	m_bDiagFlag = TRUE;
}

void	CtmTaskDsp28::EndDiag()
{
	m_bDiagFlag = FALSE;
}




int		CtmTaskDsp28::ReqValuesDiag(int pIDs[], int nElements, int nDiagType)
{
	int	nType	= nDiagType | 0x8000;
	int	nResult	= 0;
	if (nElements > 0)
	{
		nResult		= ReqValues(nType, nElements, pIDs, NULL);
	}
	return nResult;
}

int		CtmTaskDsp28::ReqValuesDiag(int nStartID, int nElements, int nDiagType)
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


int		CtmTaskDsp28::WriteValuesDiag(int	pIDs[], int pValues[], int nElements, int nDiagType)
{
	int	nType	= nDiagType | 0x8000;
	int	nResult	= 0;
	if (nElements > 0)
	{
		nResult		= ReqValues(nType, nElements, pIDs, pValues);
	}
	return nResult;
}
		
		
int		CtmTaskDsp28::WriteValuesDiag(int nStartID, int pValues[], int nElements, int nDiagType)
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


WORD	CtmTaskDsp28::OpenICMPDevice()	
{
	int		nResult		= -1;
	int		nCount 		= 3;
	int		sockICMPfd	= -1;
	struct	sockaddr_in		dstaddr;
	char*	pICMPbuf 		= NULL;
	char	pRecvBuf[20];
	char	pDataName[20] 	=	"TECHMATION";
	pICMPbuf 	= (char*)malloc(200);
	memset(pICMPbuf, 0, sizeof(pICMPbuf));
	memset(pRecvBuf, 0, sizeof(pRecvBuf));
	bzero(&dstaddr, sizeof(dstaddr));

	if((sockICMPfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		printf("socket ICMP is open error.\n");
	dstaddr.sin_family 	= AF_INET;
	dstaddr.sin_port	= htons(2310);
	if(inet_pton(AF_INET, "169.254.11.10", &dstaddr.sin_addr) <= 0)
		printf("inet_pton func is error.\n");
	
	while(TRUE)
	{
		SendICMPDate(sockICMPfd, pICMPbuf, pDataName, &dstaddr);
		RecvEchoReply(sockICMPfd, pRecvBuf);
		if(strcmp(pRecvBuf, pDataName) == 0)
		{
			m_nConnectStatus=0;
			m_lCheckQuickTimer=0;
			nResult = 0;
			printf("send icmp ok...\n");
			break;
		}
		else
		{
			m_nConnectStatus=1;
		//	printf("send icmp failed...\n");
			if(nCount==0)	break;	
			else	continue;
		}
		nCount--;
	}
	m_lCheckQuickTimer=CNT_INT_CHECK_ONLNE_TIME;
	free(pICMPbuf);
	close(sockICMPfd);
	return nResult;
}

WORD	CtmTaskDsp28::SendICMPDate(int sockfd, char* pIcmp_data, char* pDateName, sockaddr_in* dstaddr)
{
	if(pIcmp_data == NULL || pDateName == NULL)
		return -1;
	int		nIcmpFlag = -1;	
	int		nDateLen   = strlen(pDateName);
	
	socklen_t dstlen = sizeof(struct sockaddr_in);
	struct icmp* Icmp_hdr = NULL;
	char*	pIcmpDate = NULL;
	
	Icmp_hdr = (struct icmp*)pIcmp_data;
	Icmp_hdr->icmp_type 	= ICMP_ECHO;
	Icmp_hdr->icmp_code 	= 0;
	Icmp_hdr->icmp_id 		= getpid();
	Icmp_hdr->icmp_seq 		= 1;
	Icmp_hdr->icmp_cksum	= 0;
	pIcmpDate = (char*)Icmp_hdr + 8;
	strcpy(pIcmpDate, pDateName);
	nIcmpFlag = sendto(sockfd, Icmp_hdr, 18, 0,
						(struct sockaddr*)dstaddr, dstlen);
	return 0;
}		

char*	CtmTaskDsp28::RecvEchoReply(int sockfd, char* pszRecvBuf)
{
	char	cszBuf[100];
	struct 	icmp*	icmp	= NULL;
	struct 	ip*		ip		= NULL;
	int		nLen	= 0;
	memset(cszBuf, 0, sizeof(cszBuf));

	alarm(5);
	if((nLen = read(sockfd, cszBuf, sizeof(cszBuf))) == -1)
	{
		printf("read ICMP data is error.\n");
		return 	pszRecvBuf;
	}
	ip = (struct ip*)cszBuf;
	if(ip->ip_p != IPPROTO_ICMP)
	{
		printf("ICMP protocol error \n");
		return 	pszRecvBuf;
	}
	icmp = (struct icmp*)(cszBuf + sizeof(struct ip));
	if(icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if(icmp->icmp_id != getpid())
		{
			printf("not is the this process.\n");
			return pszRecvBuf;
		}
		else
		{
			strncpy(pszRecvBuf, (char*)icmp+8, 10);
			return pszRecvBuf;
		}
	}
}
#ifndef	D_BEOBJECT_CTMTASKDSP28
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskdsp28 == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CTaskHost*>(pResult))->CreateSelf();
		g_ptaskdsp28 = static_cast<CTaskHost*>(pResult);
	}
	return g_ptaskdsp28;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
	{
		(static_cast<CTaskHost*>(pInstance))->FreeSelf();
		delete pInstance;
		pInstance = NULL;
	}
}
#endif
