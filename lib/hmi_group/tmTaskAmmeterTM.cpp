#include	"tmTaskAmmeterTM.h"
#include	"../../app.h"
#include	"../../main.h"
#include	"../../utils.h"
#include	"../libControl.h"
#include	"../../commonaction.h"

#define     MINUTE                           60
#define     HOUR                             (60*MINUTE)
#define     DAY                              (24*HOUR)
#define     YEAR                             (365*DAY)

#define		AMMETER_TIMEOUT		5000
#define		AMMETER_TIME		140			//5s

#ifdef		D_X86
	char*	cszSavePath	= "./Ammeter";
#else
	char*	cszSavePath	= "/conf/Ammeter";
#endif

WORD		wCurVol[]		=
{
	AMMETER_VOLTAGE_A,
	AMMETER_CURRENT_A,
	AMMETER_VOLTAGE_B,
	AMMETER_CURRENT_B,
	AMMETER_VOLTAGE_C,
	AMMETER_CURRENT_C,
};

WORD		wVotage[] =
{
	AMMETER_RS485ID_AVRMS,
	AMMETER_RS485ID_BVRMS,
	AMMETER_RS485ID_CVRMS,
};


AMMETERCONFIG	CtmTaskAmmeterTM::m_AmmeterConf = 
{
	1, {32, 0}
};


long	lTimer1 = AMMETER_TIMEOUT;
IMPLEMENT_DYNCREATE(CtmTaskAmmeterTM, CTaskAmmeter)

CtmTaskAmmeterTM::CtmTaskAmmeterTM(): CTaskAmmeter()
{
	m_nErrCount = 0;
	m_cCurIndex		= 0;
	m_cOldIndex		= 0;
	m_dwAmtCount	= 0;
	m_wAmtRate		= 0;
	m_bCheckFlag	= FALSE;
	m_bAutoCheckAmt = FALSE;
	m_dwSetAmtCount	= 0;
	
	memset(m_address, 0xAA, AMMETER_FRAME_ADDR_LENGTH);
	memset(m_wakeup, AMMETER_FRAME_WAKEUP_FLAG, sizeof(m_wakeup));
	memset(m_para, 0, sizeof(m_para));
	memset(m_dwAmtImpedance, 0, sizeof(m_dwAmtImpedance));
	memset(m_AMTError, 0, sizeof(m_AMTError));
	memset(m_wLineTime, 0, sizeof(m_wLineTime));
	memset(m_wNode, 0, sizeof(m_wNode));
	memset(m_dwConfigV, 0, sizeof(m_dwConfigV));
	memset(m_dwCurImpedance, 0, sizeof(m_dwCurImpedance));
	memset(m_dwOldImpedance, 0, sizeof(m_dwOldImpedance));
	memset(m_dwCheckImpedance, 0, sizeof(m_dwCheckImpedance));
	memset(m_wVoltageV, 0, sizeof(m_wVoltageV));
	memset(m_wAmtCTRate, 0, sizeof(m_wAmtCTRate));
	
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_cCurIndex].wCTPara;
	m_RS485Ammeter.newtermios.c_cflag 		= CLOCAL | CREAD;
	m_RS485Ammeter.newtermios.c_cflag		&= ~CSIZE;
	m_RS485Ammeter.newtermios.c_cflag      	|= CS8;
	m_RS485Ammeter.newtermios.c_cflag      	&= ~CSTOPB;
	m_RS485Ammeter.newtermios.c_iflag 		= INPCK; 
	m_RS485Ammeter.newtermios.c_cflag      	|= PARENB;
	m_RS485Ammeter.newtermios.c_cflag      	&= ~PARODD;
	m_RS485Ammeter.newtermios.c_oflag 		= 0;
	m_RS485Ammeter.newtermios.c_lflag 		= 0;
	m_RS485Ammeter.newtermios.c_cc[VTIME] 	= 1;//5;	//500ms
	m_RS485Ammeter.newtermios.c_cc[VMIN] 	= 0;
	m_RS485Ammeter.nBaudRate 				= B38400;
	m_RS485Ammeter.wChannel 				= _COM2;
	
	if(Open(&m_RS485Ammeter) < 0 )
	{
		printf("OpenAmmeter TM error \n");	
	}
	else
	{
		printf("OpenAmmeter TM ok \n");	
	}
	
	m_bReadConfFlag = FALSE;
	//m_bAmmeterConfFlag = FALSE;
	
	AddTimer(&lTimer1);
}

CtmTaskAmmeterTM::~CtmTaskAmmeterTM()
{
	DeleteTimer(&lTimer1);
}

//	execute it atfer the contructor
int	CtmTaskAmmeterTM::CreateSelf()
{
	//printf("int	CtmTaskAmmeterTM::CreateSelf() \n");
	CTaskAmmeter::CreateSelf();
		
	//AddCounter(&m_lCheckTimer, m_idMe);
	RegisterMessage(MSG_AMMETER, m_idMe);
	
	return 0;
}

//	execute it before the destructor
int	CtmTaskAmmeterTM::FreeSelf()
{
	//DeleteCounter(&m_lCheckTimer, m_idMe);
	UnRegisterAllMessage(MSG_AMMETER);
	
	return CTaskAmmeter::FreeSelf();
}

void	CtmTaskAmmeterTM::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	CTask::WndProc(message, wParam, lParam);
	for(int i = 0; i < AMMETER_RS485ID_COUNT; i++)
	{
		if(m_wLineTime[i] > 0)
			m_wLineTime[i]--;
	}
}

void CtmTaskAmmeterTM::Run()
{
	
}

/*
 * first, we need 4-bytes data filled with 0xFE to wakeup the device.
 */
int CtmTaskAmmeterTM::SendData(void *pData, int nLen)
{
	//printf("2 SendData nLen=%d m_fdcom=%d \n", nLen, m_fdcom);
	if (m_fdcom >= 0)
	{
		//for (int i = 0; i < nLen; i++)
		//	printf("%2x  ", *((BYTE *)pData + i));
		//printf("\n");
		write(m_fdcom, m_wakeup, sizeof(m_wakeup));
		write(m_fdcom, pData, nLen);
		//CTaskAmmeter::SendData(m_wakeup, sizeof(m_wakeup));
		//return CTaskAmmeter::SendData(pData, nLen);	
	}
	return CTaskAmmeter::SendData(m_wakeup, sizeof(m_wakeup));
}

/*
 * parse the date we received from device.
 * when the length of data we received is zero or the buffer is full,
 * we start parsing the data.
 * if received zero length data more than 4 times, then reopen the device.
 *
 */
int	CtmTaskAmmeterTM::Parse(void *pData, int nLen)
{
//	int i = 0;
	static int nPos = 0;
	FRAMEHEADER *pHeader = NULL;
	BYTE *pTempData	= NULL;
	//MSG		msg;
	//printf("Parse nLen=%d\n", nLen);
	do
	{
		if (nLen <= 0)
		{
			m_nErrCount++;
			
			if (lTimer1 < 0)
			{
				g_bAmmeterStatus = FALSE;
				//printf("reopen device\n");
				Close();
				sleep(3);
				Open();
				m_nErrCount = 0;
				lTimer1 = AMMETER_TIMEOUT;		//James add 2011/11/10 ¤W¤È 12:13:38
				
				break;
			}
		}
		//lTimer1 = AMMETER_TIMEOUT;		//James add 2011/11/10 ¤W¤È 12:13:38
		//if (nLen > 0)
		//	printf("%x\n", *(BYTE *)pData);
		//if(nLen > 0)
		//	printf("nPos=%d nLen=%d pData=%x \n", nPos, nLen, *(BYTE*)pData);
		if (nLen > 0 && nPos + nLen < (int)BUFF_SIZE)	//check array overflow
		{
			memcpy(m_readbuff + nPos, pData, nLen);
			
			nPos += nLen;
			m_nErrCount = 0;
			//printf("return nLen=%d nPos=%d pData=%x \n", nLen, nPos, *(BYTE*)pData);
			return nLen;
		}
		
		if (nPos == 0 || m_readbuff[0] != AMMETER_FRAME_WAKEUP_FLAG)
		{
			//printf("nPos=%d m_readbuff[0]=%x \n", nPos, m_readbuff[0]);
			//printf("recv data error: no wake up flag! nPos=%d\n", nPos);
			//printf("return break \n");
			usleep(20*1000);
			break;
		}
		
		//printf("ok nPos=%d m_readbuff[0]=%x \n", nPos, m_readbuff[0]);
		g_bAmmeterStatus = TRUE;
		
		pHeader 	= (FRAMEHEADER *)(m_readbuff + 1);				//the first byte is wakeup flag 0xFE
		pTempData = (BYTE *)(pHeader + 1); 
		memcpy(&m_cCurIndex, m_readbuff+2, sizeof(m_cCurIndex));
		m_cCurIndex = GetAmtCurNum(m_cCurIndex);
		if(m_cCurIndex < 0 || m_cCurIndex > AMMETER_RS485ID_COUNT)
			m_cCurIndex = 0;
		
		if (CheckFrame((BYTE *)pHeader) != 0)
		{
			//printf("return CheckFrame \n");
			usleep(20*1000);
			break;
		}
		
		m_wLineTime[m_cCurIndex] = AMMETER_TIME;
		//printf("GetData pHeader->len =%d  lTimer1 =%d pHeader->control=%x\n",  
		//pHeader->len, lTimer1, pHeader->control);
		//for (i = 0; i < pHeader->len; i++)
		//{
		//	pTempData[i] -= AMMETER_FRAME_DATAADD;
		//}
		
		//msg.message	= MSG_AMMETER;
		//g_pApplication->QueueMessage(&msg);
		
		//printf("pHeader->contro=%x len=%d  \n", pHeader->control, pHeader->len);
		switch (pHeader->control & 0x1F)
		{
			case AMMETER_CONTROL_READ:
				ResRead(pTempData, pHeader->len);
				lTimer1 = AMMETER_TIMEOUT;
				usleep(20*1000);
				break;
			default:
				break;
		}
		
		usleep(1);
	} while(0);
	
	nPos = 0;
	memset(m_readbuff, 0, sizeof(m_readbuff));
	
	//printf("StartSend 1\n");
	StartSend();
	//printf("WaitRecv 1 \n");
	WaitRecv();
	
	return nLen;
}

/*
 * checking the frame we received.
 * the frame format(10 bytes header, 2 bytes footer):
 * [68H A0 A1 A2 A3 A4 A5 68H C L	 D0 D1 D2 ... DN 	CS 16H]
 * -68H: 	starta flag
 * -A0-A5: 	addr fields
 * -68H: 	startb flag
 * -C: 		control code
 * -L: 		data length
 * -D0-DN: 	the data fields
 * -CS: 	checksum
 * -16H:	end flag 
 */
int CtmTaskAmmeterTM::CheckFrame(BYTE *pFrame)
{
	unsigned i = 0;
	int bResult = -1;
	BYTE checksum = 0;
	BYTE *pData = NULL;
	FRAMEHEADER *pHeader = (FRAMEHEADER *)pFrame;
	FRAMEFOOTER *pFooter = (FRAMEFOOTER *)(pFrame + sizeof(FRAMEHEADER) + pHeader->len);
	
	do
	{
		if (pHeader->starta != AMMETER_FRAME_START_FLAG || pHeader->startb != AMMETER_FRAME_START_FLAG
			|| pFooter->end != AMMETER_FRAME_END_FLAG)
		{
			bResult = -1;
			break;
		}
		
		if (!(pHeader->control & AMMETER_CONTROL_RESPONSE))
		{
			bResult = -1;
			break;
		}
		
		if (pHeader->control & AMMETER_CONTROL_NEXTFRAME)
		{
			printf("contiguous data\n");
		}
		
		for (i = 0, pData = pFrame; i < sizeof(FRAMEHEADER) + pHeader->len; i++, pData++)
		{
			checksum += *pData;
		}
		
		if (pFooter->cs != checksum)
		{
			printf("checksum error!\n");
			bResult = -1;
			break;
		}
		
		bResult = 0;
		
	} while(0);
	
	return bResult;
}

/*
 * checking the frame we received.
 * the frame format(10 bytes header, 2 bytes footer):
 * [68H A0 A1 A2 A3 A4 A5 68H C L	 D0 D1 D2 ... DN 	CS 16H]
 * -68H: 	starta flag
 * -A0-A5: 	addr fields  A0 is Ammeter Node
 * -68H: 	startb flag
 * -C: 		control code
 * -L: 		data length
 * -D0-DN: 	the data fields
 * -CS: 	checksum
 * -16H:	end flag 
 */
void CtmTaskAmmeterTM::SetHeader(BYTE bControl, int nLen, FRAMEHEADER *pHeader, WORD wIndex)
{
	if(wIndex >= 0 && wIndex < AMMETER_RS485ID_COUNT)
	{
		BYTE	bNode	= m_wNode[wIndex];
		pHeader->starta 	= AMMETER_FRAME_START_FLAG;
		memcpy(pHeader->address, m_address, AMMETER_FRAME_ADDR_LENGTH);
		///memcpy(pHeader->address, &bNode, sizeof(bNode));
		pHeader->startb 	= AMMETER_FRAME_START_FLAG;
		pHeader->control 	= bControl;
		pHeader->len		= nLen;
	}
}

/*
 * set the frame footer
 * 
 * @pHeader - the frame footer
 */
void CtmTaskAmmeterTM::SetFooter(FRAMEHEADER *pHeader)
{
	unsigned i = 0;
	BYTE *pData = (BYTE *)pHeader + sizeof(FRAMEHEADER);
	BYTE checksum = 0;
	FRAMEFOOTER *pFooter = (FRAMEFOOTER *)((BYTE *)pHeader + sizeof(FRAMEHEADER) + pHeader->len);
	
	for (i = pHeader->len; i > 0; i--)
	{
		//*pData += AMMETER_FRAME_DATAADD;
		pData++;
	}
	for (i = 0, pData = (BYTE *)pHeader; i < sizeof(FRAMEHEADER) + pHeader->len; i++, pData++)
	{
		checksum += *pData;
	}
	
	pFooter->cs = checksum;
	pFooter->end = AMMETER_FRAME_END_FLAG;
}

int	CtmTaskAmmeterTM::Request(int nType, WORD wAmmeterNo)
{
	//printf("WaitSend 2\n");
	WaitSend();
	
	//printf("Request nType=%d  lTimer1=%d \n", nType, lTimer1);
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_POSI_WATTHR, 0, NULL);
//printf("Request nType=%d  lTimer1=%d \n", nType, lTimer1);
			break;
		case AMMETER_ENERGY:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_WATT, 0, NULL);
			break;
		case AMMETER_VOLTAGE_A:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_AVRMS, 0, NULL);
			break;
		case AMMETER_VOLTAGE_B:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_BVRMS, 0, NULL);
			break;
		case AMMETER_VOLTAGE_C:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_CVRMS, 0, NULL);
			break;
		case AMMETER_CURRENT_A:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_AIRMS, 0, NULL);
			break;
		case AMMETER_CURRENT_B:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_BIRMS, 0, NULL);
			break;
		case AMMETER_CURRENT_C:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_CIRMS, 0, NULL);
			break;
		case AMMETER_VOLTAGE_AB:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_ABVRMS, 0, NULL);
			break;
		case AMMETER_VOLTAGE_AC:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_ACVRMS, 0, NULL);
			break;
		case AMMETER_VOLTAGE_BC:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_BCVRMS, 0, NULL);
			break;
		case AMMETER_WRITE_TEST:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_TEST, 0, NULL);
			break;
		default:
			break;
	}
	//printf("StartRecv 2 \n");
	StartRecv();
	return 0;
}

int CtmTaskAmmeterTM::Request(BYTE bCode, WORD wID, int nLen, BYTE *pData)
{	
	//printf("bCode=%d\n", bCode);
	switch(bCode)
	{
		case AMMETER_CONTROL_READ:
			ReqRead(wID);
			//printf("wID=%d\n", wID);
			break;
		case AMMETER_CONTROL_FOLLOW:
			ReqFollowData(wID);
			break;
		case AMMETER_CONTROL_REREAD:
			ReqReRead();
			break;
		case AMMETER_CONTROL_WRITE:
			ReqWrite(wID, pData, nLen);
			break;
		case AMMETER_CONTROL_TIME:
			ReqAdjustTime();
			break;
		case AMMETER_CONTROL_ADDR:
			ReqWriteAddr(wID, pData);
			break;
		case AMMETER_CONTROL_BITRATE:
			ReqChangeRate(wID, *pData);
			break;
		case AMMETER_CONTROL_PASSWORD:
		case AMMETER_CONTROL_ZERO:
			break;
		default:
			break;
	}
	return 0;
}

int	CtmTaskAmmeterTM::Request(BYTE bCode, WORD* pwID, WORD wElement, WORD wIndex)
{
	//printf("WaitSend 1\n");
	WaitSend();
	
	//printf("Request bCode=%d  lTimer1=%d wElement=%d \n", bCode, lTimer1, wElement);
	switch(bCode)
	{
		case AMMETER_CONTROL_READ:
			ReqRead(pwID, wElement, wIndex);
			break;
	}
	//printf("StartRecv 1\n");
	StartRecv();
	return 0;
}

int	CtmTaskAmmeterTM::ReadInt(int nType, WORD wAmmeterNo)
{
	int nValue = 0;
	WORD 	wNode = 0;
	if(wAmmeterNo < 0 || wAmmeterNo > AMMETER_RS485ID_COUNT)
		return nValue;
	
	wNode = (nType & 0xFF00) >> 8;
	nType = nType & 0x00FF;

//printf("nType =%d\n",nType);
	switch (nType)
	{
		case AMMETER_DATAVER:
			nValue = m_para[wAmmeterNo].dwDataVer;
			//printf("nValue ... is %x \n", nValue);
			break;
		default:
			break;
	}
	
	//printf("Read nType=%d \n", nType);
	//switch (nType)
	//{
	//	case AMMETER_TOTAL_POWER:
	//		nValue = (int)m_para.dTotalPower;
	//		break;
	//	case AMMETER_ENERGY:
	//		nValue = (int)m_para.dEnergy;
	//		break;
	//	case AMMETER_VOLTAGE_A:
	//		nValue = m_para.nVoltageA;
	//		break;
	//	case AMMETER_VOLTAGE_B:
	//		nValue = m_para.nVoltageB;
	//		break;
	//	case AMMETER_VOLTAGE_C:
	//		nValue = m_para.nVoltageC;
	//		break;
	//	case AMMETER_CURRENT_A:
	//		nValue = (int)m_para.dCurrentA;
	//		break;
	//	case AMMETER_CURRENT_B:
	//		nValue = (int)m_para.dCurrentB;
	//		break;
	//	case AMMETER_CURRENT_C:
	//		nValue = (int)m_para.dCurrentC;
	//		break;
	//	default:
	//		break;
	//}
	return nValue;
}

double CtmTaskAmmeterTM::ReadDouble(int nType, WORD wAmmeterNo)
{
	double dValue = 0;
	//DWORD 	nValue =0;
	
	//switch (nType)
	//{
	//	case AMMETER_DATAVER:
	//		nValue = m_para[wAmmeterNo].dwDataVer;
	//		//printf("nValue ... is %x \n", nValue);
	//		break;
	//	case AMMETER_DEVIATION:
	//		nValue = m_para[wAmmeterNo].dwPhaseDeviation;
	//		//printf("nValue ... is %x \n", nValue);
	//		break;
	//	default:
	//		break;
	//}
	
	return dValue;
}

float CtmTaskAmmeterTM::ReadFloat(int nType, WORD wAmmeterNo)
{
	float 	fValue = 0;
	WORD 	wNode = 0;
	if(wAmmeterNo < 0 || wAmmeterNo > AMMETER_RS485ID_COUNT)
		return fValue;
	
	wNode = (nType & 0xFF00) >> 8;
	nType = nType & 0x00FF;
	
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			fValue = m_para[wAmmeterNo].fTotalPower;
			break;
		case AMMETER_ENERGY:
			fValue = m_para[wAmmeterNo].fEnergy;
			//cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!fValue ="<<fValue <<endl;
			break;
		case AMMETER_VOLTAGE_A:
			fValue = m_para[wAmmeterNo].fVoltageA;
			break;
		case AMMETER_VOLTAGE_B:
			fValue = m_para[wAmmeterNo].fVoltageB;
			break;
		case AMMETER_VOLTAGE_C:
			fValue = m_para[wAmmeterNo].fVoltageC;
			break;
		case AMMETER_CURRENT_A:
			fValue = m_para[wAmmeterNo].fCurrentA;
			break;
		case AMMETER_CURRENT_B:
			fValue = m_para[wAmmeterNo].fCurrentB;
			break;
		case AMMETER_CURRENT_C:
			fValue = m_para[wAmmeterNo].fCurrentC;
			break;
		case AMMETER_VOLTAGE_AB:
			fValue = m_para[wAmmeterNo].fVoltageAB;
			break;
		case AMMETER_VOLTAGE_BC:
			fValue = m_para[wAmmeterNo].fVoltageBC;
			break;
		case AMMETER_VOLTAGE_AC:
			fValue = m_para[wAmmeterNo].fVoltageAC;
			break;
		case AMMETER_POWERFACTOR:
			fValue = m_para[wAmmeterNo].fPowerFactor;
			break; 
		default:
			break;
	}
	return fValue;
}


void* CtmTaskAmmeterTM::ReadConf()
{
	if(m_bReadConfFlag) return &m_AmmeterConf;
	
	CtmFile	u_File;
	int		nFd	= 0;
	DWORD	dwVersion = 0;
	u_File.Open(AMMETER_CONFIG_PATH, O_RDONLY | O_SYNC);
	
	nFd = u_File.GetFileHandle();
	
	if (nFd > 0)
	{
		u_File.Read(&dwVersion, sizeof(dwVersion));
		if(dwVersion == AMMETER_CONFIG_VERSION)
			u_File.Read(&m_AmmeterConf, sizeof(m_AmmeterConf));
	}
	//for(int i = 0; i < m_AmmeterConf.wCount; i++)
	//	printf("Read2  %d, wNode = %d, wCTPara = %d\n", i, m_AmmeterConf.AmmeterInfo[i].wNode, m_AmmeterConf.AmmeterInfo[i].wCTPara);
	
	m_bReadConfFlag = TRUE;
	return &m_AmmeterConf;
}

int CtmTaskAmmeterTM::WriteConf(void* pConf)
{
	CtmFile	u_File;
	int		nFd	= 0;
	DWORD	dwVersion = AMMETER_CONFIG_VERSION;
	u_File.Open(AMMETER_CONFIG_PATH, O_RDWR | O_CREAT | O_SYNC);
	
	m_AmmeterConf = *((AMMETERCONFIG*)pConf);
	nFd = u_File.GetFileHandle();
	//printf("test 21\n");
	
	if (nFd > 0)
	{
		u_File.Write(&dwVersion, sizeof(dwVersion));
		u_File.Write(&m_AmmeterConf, sizeof(m_AmmeterConf));
	}
	else
		return -1;
	m_bReadConfFlag = TRUE;
	return 0;
}

/*
 * request reading data
 * control code: 0x01
 * data length: 2
 * data filed: the ID
 *
 * eg: wID=0x9010
 * [68 AA AA AA AA AA AA 68 01 02 10 90 CS 16]
 * Note: the data filed is little-endian.
 *
 * @wID - the data ID
 */
int CtmTaskAmmeterTM::ReqRead(WORD wID)
{
	int nLen = 0;
	BYTE *pData = NULL;

	SetHeader(AMMETER_CONTROL_READ, 2, (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	*pData++ = wID & 0x00FF;
	*pData++ = (wID >> 8) & 0x00FF;
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	
	return SendData(m_sendbuff, nLen);
}

int CtmTaskAmmeterTM::ReqRead(WORD* pwID, WORD wElement, WORD wIndex)
{
	int 	nLen = 0;
	WORD	wTempID = 0;
	BYTE *pData = NULL;

	SetHeader(AMMETER_CONTROL_READ, wElement*2, (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	
	for(int i = 0; i < wElement; i++)
	{
		//wTempID = *(WORD*)((BYTE*)pwID + i*sizeof(WORD));
		memcpy(&wTempID, ((BYTE*)pwID) + i*sizeof(WORD), sizeof(WORD));
		*pData++ = wTempID & 0x00FF;
		*pData++ = (wTempID >> 8) & 0x00FF;
		
		//printf("ReqRead i =%d wTempID=%d \n", i, wTempID);
	}
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	
	return SendData(m_sendbuff, nLen);
}

void CtmTaskAmmeterTM::ResRead(void *pData, int nLen)
{
	WORD	wID		= 0, wElement = 0, wOffset = 0;;
	
	wElement = nLen / SINGLE_DATA_LEN;
	for(int i = 0; i < wElement; i++)
	{
		memcpy(&wID, (BYTE*)pData+wOffset, sizeof(wID));
		wOffset += sizeof(wID);
		//printf("CtmTaskAmmeterTM::ResRead   i=%d ReswID=%d Read:%d wOffset=%d \n", i, wID, nLen, wOffset);
		switch (wID)
		{
			case AMMETER_RS485ID_POSI_WATTHR:	//AMMETER_REQ_TOTAL_POWER:
				SaveTotalPower((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_WATT: 			//AMMETER_REQ_ENERGY:
				SaveEnergy((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_AVRMS:			//AMMETER_REQ_VOLTAGE_A:
				SaveVoltageA((char *)pData + wOffset, nLen - sizeof(wID));
				//printf("SaveVoltageA \n");
				DearlErrorVoltage(wID, m_cCurIndex,(char*)pData+wOffset);
				break;
			case AMMETER_RS485ID_BVRMS:			//AMMETER_REQ_VOLTAGE_B:
				SaveVoltageB((char *)pData + wOffset, nLen - sizeof(wID));
				DearlErrorVoltage(wID, m_cCurIndex,(char*)pData+wOffset);
				break;
			case AMMETER_RS485ID_CVRMS:			//AMMETER_REQ_VOLTAGE_C:
				SaveVoltageC((char *)pData + wOffset, nLen - sizeof(wID));
				DearlErrorVoltage(wID, m_cCurIndex,(char*)pData+wOffset);
				break;
			case AMMETER_RS485ID_AIRMS:			//AMMETER_REQ_CURRENT_A:
				SaveCurrentA((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_BIRMS:			//AMMETER_REQ_CURRENT_B:
				SaveCurrentB((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_CIRMS:			//AMMETER_REQ_CURRENT_C:
				SaveCurrentC((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_ABVRMS:
				SaveVoltageAB((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_BCVRMS:
				SaveVoltageBC((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_ACVRMS:
				SaveVoltageAC((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_NEGA_WATTHR:
				printf("dataversion \n");
				SaveDataVersion((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_POSI_VAR:
				break;
			case AMMETER_RS485ID_NEGA_VAR:
				SavePhaseDeviation((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_PF:
				SavePowerFactor((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			default:
				break;
		}
		wOffset += sizeof(float);
	}
}

void CtmTaskAmmeterTM::SaveTotalPower(void *pData, int nLen)
{
	double dPower = 0;
	//m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	//m_CTPara = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED19").lValue;
	//m_CTPara = m_wAmtCTRate[m_cCurIndex];
	AMMETERCONFIG	AmmeterConf;
	memset(&AmmeterConf, 0, sizeof(AmmeterConf)); //2013-4-13, add 
	AmmeterConf = *((AMMETERCONFIG*)(g_ptaskAmmeter->ReadConf()));
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_cCurIndex].wCTPara;
	long lValue = 0;

	if(m_CTPara > 0)
		m_para[m_cCurIndex].fEnergy = DATA2FLOAT(pData, nLen)*m_CTPara;
	else
		m_para[m_cCurIndex].fEnergy = DATA2FLOAT(pData, nLen);
	
	lValue = (long)(m_para[m_cCurIndex].fEnergy / 10000.0);  // mw  --> kw * 100(2 precision)
	SetDBValue("AMMETER_HMI_REAL_POWER", lValue);
	dPower = (double)lValue;
	if (m_notifier)
		m_notifier(AMMETER_NOTIFY_TOTALPOWER, &dPower, sizeof(dPower), 0);
	//printf("total power:%f, %d\n", m_para[m_cCurIndex].fEnergy, lValue);
}

void CtmTaskAmmeterTM::SaveEnergy(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_cCurIndex].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_cCurIndex].fTotalPower = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_cCurIndex].fTotalPower = DATA2FLOAT(pData, nLen);
	
	//printf("energy:%f\n", m_para.fTotalPower);
}

void CtmTaskAmmeterTM::SaveVoltageA(void *pData, int nLen)
{
	//m_para.nVoltageA = BCD2Int(pData, nLen);
	m_para[m_cCurIndex].fVoltageA = DATA2FLOAT(pData, nLen);
	//printf("voltage A:%d\n", m_para.fVoltageA);
}

void CtmTaskAmmeterTM::SaveVoltageB(void *pData, int nLen)
{
	//m_para.nVoltageB = BCD2Int(pData, nLen);
	m_para[m_cCurIndex].fVoltageB = DATA2FLOAT(pData, nLen);
	//printf("voltage B:%d\n", m_para.fVoltageB);
}

void CtmTaskAmmeterTM::SaveVoltageC(void *pData, int nLen)
{
	//m_para.nVoltageC = BCD2Int(pData, nLen);
	m_para[m_cCurIndex].fVoltageC = DATA2FLOAT(pData, nLen);
	//printf("voltage C:%d\n", m_para.fVoltageC);
}

void CtmTaskAmmeterTM::SaveCurrentA(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_cCurIndex].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_cCurIndex].fCurrentA = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_cCurIndex].fCurrentA = DATA2FLOAT(pData, nLen);
	//printf("Current A:%f\n", m_para.fCurrentA);
}

void CtmTaskAmmeterTM::SaveCurrentB(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_cCurIndex].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_cCurIndex].fCurrentB = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_cCurIndex].fCurrentB = DATA2FLOAT(pData, nLen);
	//printf("Current B:%f\n", m_para.fCurrentB);
}

void CtmTaskAmmeterTM::SaveCurrentC(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_cCurIndex].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_cCurIndex].fCurrentC = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_cCurIndex].fCurrentC = DATA2FLOAT(pData, nLen);
	//printf("Current C:%f\n", m_para.fCurrentC);
}

void CtmTaskAmmeterTM::SaveVoltageAB(void *pData, int nLen)
{
	m_para[m_cCurIndex].fVoltageAB = DATA2FLOAT(pData, nLen);
	//printf("Voltage AB:%f\n", m_para.fVoltageAB);
}

void CtmTaskAmmeterTM::SaveVoltageBC(void *pData, int nLen)
{
	m_para[m_cCurIndex].fVoltageBC = DATA2FLOAT(pData, nLen);
	//printf("Voltage BC:%f\n", m_para.fVoltageBC);
}

void CtmTaskAmmeterTM::SaveVoltageAC(void *pData, int nLen)
{
	m_para[m_cCurIndex].fVoltageAC = DATA2FLOAT(pData, nLen);
	//printf("Voltage AC:%f\n", m_para.fVoltageAC);
}

void CtmTaskAmmeterTM::SaveDataVersion(void *pData, int nLen)
{
	DWORD	dwTemp = 0;
	memcpy(&dwTemp, pData, sizeof(DWORD));
	m_para[m_cCurIndex].dwDataVer = dwTemp;
	printf("dwDataVer:%x\n", m_para[m_cCurIndex].dwDataVer);
}

void CtmTaskAmmeterTM::SavePhaseDeviation(void *pData, int nLen)
{
	DWORD	dwTemp = 0;
	memcpy(&dwTemp, pData, sizeof(DWORD));
	m_para[m_cCurIndex].dwPhaseDeviation = dwTemp;
	DearlErrorDeviation(m_cCurIndex, dwTemp);
	//printf("Voltage AC:%d\n", m_para[m_cCurIndex].dwPhaseDeviation);
}

void CtmTaskAmmeterTM::SavePowerFactor(void *pData, int nLen)
{
	m_para[m_cCurIndex].fPowerFactor = DATA2FLOAT(pData, nLen)*100;
	//printf("Voltage AC:%f\n", m_para[m_cCurIndex].fPowerFactor);
}

double CtmTaskAmmeterTM::BCD2Double(void *pData, int nLen, int nPrecision)
{
	int 	i 		= 0;
	double dValue 	= 0;
	char *pValue = (char *)pData;
	double dTemp 	= pow(10.0F, -nPrecision);
	
	for (i = 0; i < nLen; i++)
	{
		dValue += (pValue[i] & 0xF) * dTemp;
		dTemp  *= 10;
		dValue += ((pValue[i] >> 4) & 0xF) * dTemp;
		dTemp  *= 10;
	}
	
	return dValue;
}

int CtmTaskAmmeterTM::BCD2Int(void *pData, int nLen)
{
	int i = 0;
	char *pValue = (char *)pData; 
	int nValue = 0, nTmp = 1;
	
	for (i = 0; i < nLen; i++)
	{
		nValue += (pValue[i] & 0xF) * nTmp;
		nTmp  *= 10;
		nValue += ((pValue[i] >> 4) & 0xF) * nTmp;
		nTmp  *= 10;
	}
	
	return nValue;
}

float CtmTaskAmmeterTM::DATA2FLOAT(void *pData, int nLen)
{
	float fTemp = 0;
	memcpy(&fTemp, pData, sizeof(float));
	return fTemp;	
}

/*
 * request reading the following data
 * control code: 0x02
 * data length: 2
 * data filed: the ID
 *
 * eg: wID=0x9010
 * [68 AA AA AA AA AA AA 68 02 02 10 90 CS 16]
 * Note: the data filed is little-endian.
 *
 * @wID - the data ID
 */
int CtmTaskAmmeterTM::ReqFollowData(WORD wID)
{
	int nLen = 0;
	BYTE *pData = NULL;

	SetHeader(AMMETER_CONTROL_FOLLOW, sizeof(wID), (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	*pData++ = wID & 0x00FF;
	*pData++ = wID & 0xFF00;
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	return SendData(m_sendbuff, nLen);
}

/*
 * request rereading the previous frame
 * control code: 0x03
 * data length: 0
 * data filed: none
 *
 * eg:
 * [68 AA AA AA AA AA AA 68 03 00 CS 16]
 * Note: the data filed is little-endian.
 */
int CtmTaskAmmeterTM::ReqReRead()
{
	int nLen = 0;

	SetHeader(AMMETER_CONTROL_REREAD, 0, (FRAMEHEADER *)m_sendbuff);
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	return SendData(m_sendbuff, nLen);
}

/*
 * request writing data
 * control code: 0x04
 * data length: 2+m (include ID length 2 bytes)
 * data filed: ID + data
 *
 * eg:
 * [68 AA AA AA AA AA AA 68 04 L D0 D1 D2 ... DN CS 16]
 * Note: the data filed is little-endian.
 */
int CtmTaskAmmeterTM::ReqWrite(WORD wID, void *pBuf, int nDataLen)
{
	WaitSend();
	int nLen = 0;
	BYTE *pData = NULL;
	
	//printf("ReqWrite wID=%d \n", wID);
	SetHeader(AMMETER_CONTROL_WRITE, sizeof(wID) + nDataLen, (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	//*pData++ = wID & 0x00FF;
	//*pData = wID & 0xFF00;
	*pData++ = wID & 0x00FF;
	*pData++ = (wID >> 8) & 0x00FF;
	memcpy(pData, pBuf, nDataLen);
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	
	SendData(m_sendbuff, nLen);
	StartRecv();
	return nLen;
}

int	CtmTaskAmmeterTM::Write(int nType, void *pData, int nLen)
{
	WaitSend();
	int nLength = 0;
	BYTE *pDataBuff = NULL;
	WORD wID = nType;
	
	//printf("ReqWrite wID=%d \n", wID);
	SetHeader(AMMETER_CONTROL_WRITE, sizeof(wID) + nLen, (FRAMEHEADER *)m_sendbuff);
	pDataBuff = m_sendbuff + sizeof(FRAMEHEADER);
	//*pData++ = wID & 0x00FF;
	//*pData = wID & 0xFF00;
	*pDataBuff++ = wID & 0x00FF;
	*pDataBuff++ = (wID >> 8) & 0x00FF;
	memcpy(pDataBuff, pData, nLen);
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLength = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	
	SendData(m_sendbuff, nLength);
	StartRecv();
	return nLength;	
}

/*
 * broadcast adjusting time using current datetime
 * control code: 0x08
 * data length: 6
 *
 * eg:
 * [68 AA AA AA AA AA AA 68 08 06 ss mm hh dd mm yy CS 16]
 * 
 */
int CtmTaskAmmeterTM::ReqAdjustTime()
{
	
	time_t time;
	struct tm *ptm;
	
	int nLen = 0;
	BYTE *pData = NULL;
	
	SetHeader(AMMETER_CONTROL_TIME, 6, (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	
	ctime(&time);
	ptm = gmtime(&time);
	pData[0] = ptm->tm_sec;
	pData[1] = ptm->tm_min;
	pData[2] = ptm->tm_hour;
	pData[3] = ptm->tm_mday;
	pData[4] = ptm->tm_mon + 1;
	pData[5] = ptm->tm_year;
	
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	return SendData(m_sendbuff, nLen);
}

/*
 * set the device's address
 * control code: 0x0A
 * address: 0x99 0x99 0x99 0x99 0x99 0x99
 * data length: 6
 * data field: A0 ... A5 (the new device address)
 * 
 * eg:
 * [68 99 99 99 99 99 99 68 0A 06 A0 A1 A2 A3 A4 A5 CS 16]
 *
 * @pAddr - the address setting to device
 */
int CtmTaskAmmeterTM::ReqWriteAddr(WORD wID, void *pAddr)
{
	int nLen = 0;
	BYTE *pData = NULL;
	BYTE oldAddr[AMMETER_FRAME_ADDR_LENGTH];
	
	memcpy(oldAddr, m_address, sizeof(m_address)); //backup the address
	memset(m_address, AMMETER_FRAME_BROADCAST, sizeof(m_address));	//set the address to broadcast
	
	SetHeader(AMMETER_CONTROL_ADDR, AMMETER_FRAME_ADDR_LENGTH, (FRAMEHEADER *)m_sendbuff);
	memcpy(m_address, oldAddr, sizeof(m_address)); //recover the address
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	memcpy(pData, pAddr, AMMETER_FRAME_ADDR_LENGTH);
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	return SendData(m_sendbuff, nLen);
}

/*
 * change the bit-rate
 * control code: 0x0C
 * data length: 1
 * 
 * eg:
 * [68 AA AA AA AA AA AA 68 0C 01 Z CS 16]
 *
 * @bRate - the bit-rate sending to device
 */
int CtmTaskAmmeterTM::ReqChangeRate(WORD wID, BYTE bRate)
{
	int nLen = 0;
	BYTE *pData = NULL;
	
	SetHeader(AMMETER_CONTROL_BITRATE, 1, (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	*pData = bRate;
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	return SendData(m_sendbuff, nLen);
}

/*
 * the save file struct
 * 4 byte for Amt count - - - - - 4
 * 8*4 byte for Amt config- - - - 36
 * 2 byte for Impedance flag- - - 38
 * 2 byte for Impedance rate- - - 40
 * 8*4 byte for old Impedance - - 72
 * 18*2 byte for voltage up down- 108
 * 8*2 byte for CT rate - - - - - 124
 */
void	CtmTaskAmmeterTM::SetAMTNode()
{	
	memset(m_wNode, 0, sizeof(m_wNode));
	memset(m_dwConfigV, 0, sizeof(m_dwConfigV));
	memset(m_wVoltageV, 0, sizeof(m_wVoltageV));
	memset(m_wAmtCTRate, 0, sizeof(m_wAmtCTRate));
		printf("SetAMTNode  comein\n");
	
	int		Ammeter_fd 	= 0;
	DWORD	dwValue 	= 0;
	WORD	wFalg		= 0;
	Ammeter_fd = open(cszSavePath, O_RDONLY);
	if(Ammeter_fd == 0)
		return;
	
	read(Ammeter_fd, &m_dwSetAmtCount, sizeof(m_dwSetAmtCount));
	for(int i = 0; i < m_dwSetAmtCount; i++)
	{
		read(Ammeter_fd, &dwValue, sizeof(dwValue));
		m_dwConfigV[i] = dwValue;
		m_wNode[i] = ((dwValue&0xFF000000)>>24);
	}
	
	lseek(Ammeter_fd, 9*sizeof(DWORD), SEEK_SET);
	read(Ammeter_fd, &wFalg, sizeof(wFalg));
	read(Ammeter_fd, &m_wAmtRate, sizeof(m_wAmtRate));
	if(wFalg == 1)
		m_bCheckFlag = TRUE;
	
	lseek(Ammeter_fd, 10*sizeof(DWORD), SEEK_SET);
	for(int i = 0; i < AMMETER_SEGMENT; i++)
		read(Ammeter_fd, &m_dwOldImpedance[i], sizeof(m_dwOldImpedance[i]));
	
	lseek(Ammeter_fd, 18*sizeof(DWORD), SEEK_SET);
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < AMMETER_SEGMENT; j++)
		{
			read(Ammeter_fd, &m_wVoltageV[i][j], sizeof(m_wVoltageV[i][j]));
		}
	}
	
	lseek(Ammeter_fd, 27*sizeof(DWORD), SEEK_SET);
	for(int i = 0; i < AMMETER_RS485ID_COUNT; i++)
	{
		read(Ammeter_fd, &m_wAmtCTRate[i], sizeof(m_wAmtCTRate[i]));
		printf("SetAMTNode  m_wAmtCTRate[%d]=%d\n",i,m_wAmtCTRate[i]);
	}
	
	close(Ammeter_fd);
	memset(m_para, 0, sizeof(m_para));
}

int		CtmTaskAmmeterTM::GetAmtCurNum(int nNode)
{
	int		nResult	= 0;
	for(int i = 0; i < AMMETER_RS485ID_COUNT; i++)
	{
		if(m_wNode[i] == nNode)
		{
			nResult = i;
			break;
		}
	}
	return 	nResult;
}

void	CtmTaskAmmeterTM::DealAmtErrlog(int nTmpIndex, int nIndx)
{
	if(nTmpIndex<0 || nTmpIndex>=AMMETER_SEGMENT)
		return;

	DWORD	dwAmtErr = 0;
	
	if(m_dwCurImpedance[nTmpIndex] > m_dwOldImpedance[nTmpIndex]*(100+m_wAmtRate)/100 ||
		m_dwCurImpedance[nTmpIndex] < m_dwOldImpedance[nTmpIndex]*(100-m_wAmtRate)/100)
	{
		m_AMTError[nTmpIndex].wTotalNum++;
		m_AMTError[nTmpIndex].wCount++;
	}
	else
	{
		m_AMTError[nTmpIndex].wTotalNum++;
		if(m_AMTError[nTmpIndex].wCount > 0)
			m_AMTError[nTmpIndex].wCount--;
	}
	
	if(m_AMTError[nTmpIndex].wTotalNum == AMMETER_ERRORCNT) //3 Times Check !if error>=2:Add else :Fix
	{
		dwAmtErr |= ERROR_AMMETER_TM<<24;
		dwAmtErr |= nIndx<<16;
		dwAmtErr |= nTmpIndex;
		if(m_AMTError[nTmpIndex].wFlag == 0 && m_AMTError[nTmpIndex].wCount >= 2)
		{
			if(g_ptmControlServer != NULL)
					g_ptmControlServer->ErrorControl()->AddErrorLog(dwAmtErr);
			m_AMTError[nTmpIndex].wFlag = 1;
		}
		else if(m_AMTError[nTmpIndex].wFlag == 1 && m_AMTError[nTmpIndex].wCount < 2)
		{
			if(g_ptmControlServer != NULL)
					g_ptmControlServer->ErrorControl()->FixErrorLog(dwAmtErr);
			m_AMTError[nTmpIndex].wFlag = 0;
		}
		m_AMTError[nTmpIndex].wTotalNum = 0;
		m_AMTError[nTmpIndex].wCount 	= 0;
	}
}

void	CtmTaskAmmeterTM::SetAmtAutoCheck(DWORD	dwValue, int nSegment)
{
	if(nSegment<0 || nSegment>=AMMETER_SEGMENT || m_dwAmtCount<0 || m_dwAmtCount>AMMETER_COUNT)
		return;
	m_dwAmtImpedance[nSegment][m_dwAmtCount] = dwValue;
	//printf("m_dwAmtImpedance[%d][%d]  %d \n",nSegment, m_dwAmtCount, m_dwAmtImpedance[nSegment][m_dwAmtCount]);
}

void	CtmTaskAmmeterTM::SaveAmtCheckV()
{
	DWORD	dwTotal = 0;
	int		nCount	= 0;
	for(int	i = 0; i < AMMETER_SEGMENT; i++)
	{
		nCount  = 0;
		dwTotal = 0;
		
		for(int j = 0; j < AMMETER_COUNT; j++)
		{
			if(m_dwAmtImpedance[i][j] != 0)
			{
				dwTotal += m_dwAmtImpedance[i][j];
				nCount++;
			}
		}
		if(nCount != 0)
			dwTotal /= nCount;
		m_dwCheckImpedance[i] = dwTotal;
	}
}

DWORD	CtmTaskAmmeterTM::GetAmtOnLineS()
{
	DWORD	dwResult = 0;
	for(int i = 0; i < AMMETER_RS485ID_COUNT; i++)
	{
		dwResult |= (m_wLineTime[i]>0) << (i*4);
	}
	return dwResult;
}

void	CtmTaskAmmeterTM::DearlErrorVoltage(int	nDataType, int nAmtIndex, void* pData)
{	
	static	WORD	wErrorFlag[AMMETER_SEGMENT] = {0,0,0,0,0,0,0,0,0};
	DWORD	dwAmtErr	= 0;
	DWORD	dwFFValue	= 0xFF0000;
	float	fValue		= 0;
	int		nSegment 	= 0;
	memcpy(&fValue, pData, sizeof(float));
	dwAmtErr |= ERROR_AMMETER_TM<<24;
	dwAmtErr |= nAmtIndex<<16;
	for(int i = 0; i < 3; i++)
	{
		if(wVotage[i] == nDataType)
		{
			dwAmtErr |= i | 0x100;
			nSegment = ((m_dwConfigV[nAmtIndex]&(dwFFValue>>(i*8)))>>((2-i)*8))-1;
			if(nSegment < 0 || nSegment > AMMETER_SEGMENT)
				return ;
			if((m_wVoltageV[1][nSegment]>fValue || m_wVoltageV[0][nSegment]<fValue) && (wErrorFlag[nSegment]==0))
			{
				g_ptmControlServer->ErrorControl()->AddErrorLog(dwAmtErr);
				wErrorFlag[nSegment] = 1;
				printf("AddErrorLog nSegment is %d dwAmtErr:%x, wErrorFlag[nSegment]:%d\n", nSegment, dwAmtErr,wErrorFlag[nSegment]);
			}
			else if((m_wVoltageV[1][nSegment]<=fValue && m_wVoltageV[0][nSegment]>=fValue) && wErrorFlag[nSegment] == 1)
			{
				g_ptmControlServer->ErrorControl()->FixErrorLog(dwAmtErr);
				wErrorFlag[nSegment] = 0;
				//printf("FixErrorLog nSegment is %d dwAmtErr:%x, wErrorFlag[nSegment]:%d\n", nSegment, dwAmtErr,wErrorFlag[nSegment]);
			}
		}
	}
}

void	CtmTaskAmmeterTM::DearlErrorDeviation(int  nAmtIndex, int nValue)
{
	static	WORD	wErrorFlag[AMMETER_RS485ID_COUNT] = {0,0,0,0,0,0,0,0};
	DWORD	dwAmtErr	= 0;
	dwAmtErr |= ERROR_AMMETER_TM<<24;
	dwAmtErr |= nAmtIndex<<16;
	dwAmtErr |= 0x103;
	if(nValue == 1 && wErrorFlag[nAmtIndex] == 0)
	{
		wErrorFlag[nAmtIndex] =1;
		g_ptmControlServer->ErrorControl()->AddErrorLog(dwAmtErr);
	}
	else if(nValue == 0 && wErrorFlag[nAmtIndex] == 1)
	{
		wErrorFlag[nAmtIndex] =0;
		g_ptmControlServer->ErrorControl()->FixErrorLog(dwAmtErr);
	}
}

#ifndef	D_BEOBJECT_CTMTASKAMMETERTM
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	
	if (pResult != NULL)
	{
		(static_cast<CTaskAmmeter*>(pResult))->CreateSelf();
	}
	
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskAmmeter*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
