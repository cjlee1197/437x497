#include	"tmTaskAmmeterDLT.h"
#include	"../../app.h"
#include	"../../main.h"
#include	"../../utils.h"
#include	"../../tmshmsg.h"
#include	"../libControl.h"
#include	"../../commonaction.h"
#include	"../../iotable.h"

#define     MINUTE                           60
#define     HOUR                             (60*MINUTE)
#define     DAY                              (24*HOUR)
#define     YEAR                             (365*DAY)

#define		RUN_CHECK_TIMER					1000


//BOOL	bAmmeterMulti = FALSE;


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

AMMETERCONFIG	CtmTaskAmmeterDLT::m_AmmeterConf = 
{
	1, {{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},0}
};

IMPLEMENT_DYNCREATE(CtmTaskAmmeterDLT, CTaskAmmeter)

CtmTaskAmmeterDLT::CtmTaskAmmeterDLT(): CTaskAmmeter()
{
	m_nErrCount = 0;
	
	memset(m_nSendCount, 0, sizeof(m_nSendCount));
	
	memset(m_address, 0xAA, AMMETER_FRAME_ADDR_LENGTH);
	memset(m_wakeup, AMMETER_FRAME_WAKEUP_FLAG, sizeof(m_wakeup));
	
	memset(&m_para, 0, sizeof(m_para));
	memset(&m_RS485Ammeter, 0, sizeof(m_RS485Ammeter));
	m_SendAmmeter = 0;
	m_ParseAmmeter = AMMETER_MAX_NUM;
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
	m_RS485Ammeter.nBaudRate 				= B1200;
	m_RS485Ammeter.wChannel 				= _COM2;
	
	if(Open(&m_RS485Ammeter) < 0 )
	{
		printf("OpenAmmeter DLT error \n");	
	}
	else
	{
		printf("OpenAmmeter DLT ok \n");		
	}
	m_bReadConfFlag = FALSE;
	m_bAmmeterConfFlag = FALSE;
	m_lCheckTimer = RUN_CHECK_TIMER;
	
	//bAmmeterMulti  = g_pExTable->GetTableValue(0,"D_AMTMULTI");
	
	ReadConf();
}

CtmTaskAmmeterDLT::~CtmTaskAmmeterDLT()
{
}

//	execute it atfer the contructor
int		CtmTaskAmmeterDLT::CreateSelf()
{
	CTaskAmmeter::CreateSelf();
	
	AddCounter(&m_lCheckTimer, m_idMe);
	
	RegisterMessage(MSG_AMMETER, m_idMe);
	RegisterMessage(MSG_SH_AMMETER_CONFIG, 		m_idMe);
	
	return 0;
}

//	execute it before the destructor
int		CtmTaskAmmeterDLT::FreeSelf()
{
	DeleteCounter(&m_lCheckTimer, m_idMe);
	UnRegisterAllMessage(MSG_AMMETER);
	UnRegisterAllMessage(MSG_SH_AMMETER_CONFIG);
	
	Close();
	return CTaskAmmeter::FreeSelf();
}


void	CtmTaskAmmeterDLT::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_SH_AMMETER_CONFIG:
			if(wParam)
				m_bAmmeterConfFlag = TRUE;
			else
			{
				m_bAmmeterConfFlag = FALSE;
				memset(&m_para[AMMETER_MAX_NUM], 0, sizeof(m_para[AMMETER_MAX_NUM]));
			}
			break;
		default:
			break;
	}
	CTask::WndProc(message, wParam, lParam);
}

void	CtmTaskAmmeterDLT::Run()
{
	if(m_lCheckTimer < 0)
	{
		for(int i = 0; i < m_AmmeterConf.wCount; i++)
		{
			if(m_nSendCount[i] >= 5)
				g_bAmmeterStatus = g_bAmmeterStatus & (~(1 << i));
		}
		m_lCheckTimer = RUN_CHECK_TIMER;
	}
}

/*
 * first, we need 4-bytes data filled with 0xFE to wakeup the device.
 */
int CtmTaskAmmeterDLT::SendData(void *pData, int nLen)
{
	if (m_fdcom >= 0)
	{
		write(m_fdcom, m_wakeup, sizeof(m_wakeup));
		write(m_fdcom, pData, nLen);
	}
	return CTaskAmmeter::SendData(pData, nLen);	
}

/*
 * parse the date we received from device.
 * when the length of data we received is zero or the buffer is full,
 * we start parsing the data.
 * if received zero length data more than 4 times, then reopen the device.
 *
 */
int		CtmTaskAmmeterDLT::Parse(void *pBuf, int nLen)
{
	int i = 0;
	static int nPos = 0;
	FRAMEHEADER *pHeader = NULL;
	BYTE *pData	= NULL;
	//MSG		msg;
//	printf("nLen=%d\n", nLen);
	do
	{
		if (nLen <= 0)
		{
			m_nErrCount++;
			
			if (m_nErrCount > 6)
			{
				g_bAmmeterStatus = FALSE;
				//printf("reopen device\n");
				Close();
				sleep(3);
				Open();
				m_nErrCount = 0;
				
				break;
			}
		}
		//if (nLen > 0)
		//	printf("%x\n", *(BYTE *)pBuf);
			
		if (nLen > 0 && nPos + nLen < (int)BUFF_SIZE)	//check array overflow
		{
			memcpy(m_readbuff + nPos, pBuf, nLen);
			
			nPos += nLen;
			m_nErrCount = 0;
			return nLen;
		}
		
		if (nPos == 0 || m_readbuff[0] != AMMETER_FRAME_WAKEUP_FLAG)
		{
			//printf("recv data error: no wake up flag! nPos=%d\n", nPos);
			break;
		}
		
		//g_bAmmeterStatus = TRUE;
		i = 0;
		while(m_readbuff[i] == 0xFE)i++;
		pHeader 	= (FRAMEHEADER *)(m_readbuff + i);				//the first byte is wakeup flag 0xFE
		pData = (BYTE *)(pHeader + 1); 
		
		
		if (CheckFrame((BYTE *)pHeader) != 0)
			break;
			
		for (i = 0; i < pHeader->len; i++)
		{
			pData[i] -= AMMETER_FRAME_DATAADD;
		}
		
		//msg.message	= MSG_AMMETER;
		//g_pApplication->QueueMessage(&msg);
		for(int i = 0; i < m_AmmeterConf.wCount; i++)
		{
			if(memcmp(m_AmmeterConf.AmmeterInfo[i].address, pHeader->address, AMMETER_FRAME_ADDR_LENGTH) == 0)	//match 
			{
				m_ParseAmmeter = i;
				break;
			}
		}
		
		if(m_bAmmeterConfFlag)
			m_ParseAmmeter = AMMETER_MAX_NUM;
		//printf("m_bAmmeterConfFlag:%d, m_ParseAmmeter:%d \n", m_bAmmeterConfFlag, m_ParseAmmeter);
		
		if(memcmp(m_AmmeterConf.AmmeterInfo[0].address, m_address, AMMETER_FRAME_ADDR_LENGTH) == 0)
   			m_ParseAmmeter = 0;
   
		g_bAmmeterStatus = g_bAmmeterStatus | (1 << m_ParseAmmeter);
		m_nSendCount[m_ParseAmmeter] = 0;
		memcpy(m_para[m_ParseAmmeter].address, pHeader->address, AMMETER_FRAME_ADDR_LENGTH);
		
		switch (pHeader->control & 0x7F)
		{
			case AMMETER_CONTROL_READ:
				ResRead(pData, pHeader->len, m_ParseAmmeter);
				break;
			default:
				break;
		}
		
	} while(0);
	
	nPos = 0;
	memset(m_readbuff, 0, sizeof(m_readbuff));
	
	StartSend();
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
int		CtmTaskAmmeterDLT::CheckFrame(BYTE *pFrame)
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
 * set the frame header
 *
 * @bControl - the frame control field
 * @nLen - the len of the data field
 * @pHeader - the frame header
 */
void	CtmTaskAmmeterDLT::SetHeader(BYTE bControl, int nLen, FRAMEHEADER *pHeader)
{
	pHeader->starta 	= AMMETER_FRAME_START_FLAG;
	if(!m_bAmmeterConfFlag)
		memcpy(pHeader->address, m_AmmeterConf.AmmeterInfo[m_SendAmmeter].address, AMMETER_FRAME_ADDR_LENGTH);
	else
		memcpy(pHeader->address, m_address, AMMETER_FRAME_ADDR_LENGTH);
	pHeader->startb 	= AMMETER_FRAME_START_FLAG;
	pHeader->control 	= bControl;
	pHeader->len		= nLen;
}

/*
 * set the frame footer
 * 
 * @pHeader - the frame footer
 */
void	CtmTaskAmmeterDLT::SetFooter(FRAMEHEADER *pHeader)
{
	unsigned i = 0;
	BYTE *pData = (BYTE *)pHeader + sizeof(FRAMEHEADER);
	BYTE checksum = 0;
	FRAMEFOOTER *pFooter = (FRAMEFOOTER *)((BYTE *)pHeader + sizeof(FRAMEHEADER) + pHeader->len);
	
	for (i = pHeader->len; i > 0 && pHeader->control == 0x01; i--)
	{
		*pData += AMMETER_FRAME_DATAADD;
		pData++;
	}
	for (i = 0, pData = (BYTE *)pHeader; i < sizeof(FRAMEHEADER) + pHeader->len; i++, pData++)
	{
		checksum += *pData;
	}
	
	pFooter->cs = checksum;
	pFooter->end = AMMETER_FRAME_END_FLAG;
}

int		CtmTaskAmmeterDLT::Request(int nType, WORD wAmmeterNo)
{
	WaitSend();
	m_SendAmmeter = wAmmeterNo;
	m_nSendCount[wAmmeterNo]++;
	//printf("wAmmeterNo:%d\n", wAmmeterNo);
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			//WaitSend();
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_TOTAL_POWER, 0, NULL);
			//StartRecv();
			break;
		case AMMETER_ENERGY:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_ENERGY, 0, NULL);
			break;
/*		case AMMETER_VOLTAGE_A:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_VOLTAGE_A, 0, NULL);
			break;
		case AMMETER_VOLTAGE_B:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_VOLTAGE_B, 0, NULL);
			break;
		case AMMETER_VOLTAGE_C:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_VOLTAGE_C, 0, NULL);
			break;
		case AMMETER_CURRENT_A:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_CURRENT_A, 0, NULL);
			break;
		case AMMETER_CURRENT_B:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_CURRENT_B, 0, NULL);
			break;
		case AMMETER_CURRENT_C:
			Request(AMMETER_CONTROL_READ, AMMETER_REQ_CURRENT_C, 0, NULL);
			break;*/
		default:
			break;
	}
	StartRecv();
	return 0;
}

int 	CtmTaskAmmeterDLT::Request(BYTE bCode, WORD wID, int nLen, BYTE *pData, WORD wAmmeterNo)
{	
	switch(bCode)
	{
		case AMMETER_CONTROL_READ:
			ReqRead(wID);
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

int		CtmTaskAmmeterDLT::ReadInt(int nType, WORD wAmmeterNo)
{
	int nValue = 0;
	
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			nValue = (int)m_para[wAmmeterNo].dTotalPower;
			break;
		case AMMETER_ENERGY:
			nValue = (int)m_para[wAmmeterNo].dEnergy;
			break;
		case AMMETER_VOLTAGE_A:
			nValue = m_para[wAmmeterNo].nVoltageA;
			break;
		case AMMETER_VOLTAGE_B:
			nValue = m_para[wAmmeterNo].nVoltageB;
			break;
		case AMMETER_VOLTAGE_C:
			nValue = m_para[wAmmeterNo].nVoltageC;
			break;
		case AMMETER_CURRENT_A:
			nValue = (int)m_para[wAmmeterNo].dCurrentA;
			break;
		case AMMETER_CURRENT_B:
			nValue = (int)m_para[wAmmeterNo].dCurrentB;
			break;
		case AMMETER_CURRENT_C:
			nValue = (int)m_para[wAmmeterNo].dCurrentC;
			break;
		case AMMETER_ADDRESS:
			nValue = (int)m_para[wAmmeterNo].address;
			break;
		default:
			break;
	}
	return nValue;
}

double	CtmTaskAmmeterDLT::ReadDouble(int nType, WORD wAmmeterNo)
{
	double dValue = 0;
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			dValue = m_para[wAmmeterNo].dTotalPower;
			//printf("wAmmeterNo:%d, %f \n", wAmmeterNo, m_para[wAmmeterNo].dTotalPower);
			break;
		case AMMETER_ENERGY:
			dValue = m_para[wAmmeterNo].dEnergy;
			break;
		case AMMETER_VOLTAGE_A:
			dValue = m_para[wAmmeterNo].nVoltageA;
			break;
		case AMMETER_VOLTAGE_B:
			dValue = m_para[wAmmeterNo].nVoltageB;
			break;
		case AMMETER_VOLTAGE_C:
			dValue = m_para[wAmmeterNo].nVoltageC;
			break;
		case AMMETER_CURRENT_A:
			dValue = m_para[wAmmeterNo].dCurrentA;
			break;
		case AMMETER_CURRENT_B:
			dValue = m_para[wAmmeterNo].dCurrentB;
			break;
		case AMMETER_CURRENT_C:
			dValue = m_para[wAmmeterNo].dCurrentC;
			break;
		default:
			break;
	}
	return dValue;
}

void* CtmTaskAmmeterDLT::ReadConf()
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
	
	/*多個電表、單個電表,配置版統一，由畫面直接設定電表個數;多個電表直接設定地址,單個電表廣播地址0xAA */
	//if(bAmmeterMulti)		//多個電表, 目前默認為2個   
	//	m_AmmeterConf.wCount = 2;
	//else              	//單個電表, 要從配置中讀取CTPara, 其他均為默認值  
	//{
	//	m_AmmeterConf.wCount = 1;
	//	memset(m_AmmeterConf.AmmeterInfo[0].address, 0xAA, sizeof(m_AmmeterConf.AmmeterInfo[0].address));	//廣播地址0xAA 
	//}
	if(m_AmmeterConf.wCount < 2)
	{
		m_AmmeterConf.wCount = 1;
		memset(m_AmmeterConf.AmmeterInfo[0].address, 0xAA, sizeof(m_AmmeterConf.AmmeterInfo[0].address));	//廣播地址0xAA 
	}
	
	m_bReadConfFlag = TRUE;
	return &m_AmmeterConf;
}

int 	CtmTaskAmmeterDLT::WriteConf(void* pConf)
{
	CtmFile	u_File;
	int		nFd	= 0;
	DWORD	dwVersion = AMMETER_CONFIG_VERSION;
	u_File.Open(AMMETER_CONFIG_PATH, O_RDWR | O_CREAT | O_SYNC);
	
	m_AmmeterConf = *((AMMETERCONFIG*)pConf);
	nFd = u_File.GetFileHandle();
	
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
int		CtmTaskAmmeterDLT::ReqRead(WORD wID)
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

void	CtmTaskAmmeterDLT::ResRead(void *pData, int nLen, WORD	wAmmeterNo)
{
	WORD	wID		= 0;
	
	memcpy(&wID, pData, sizeof(wID));
	//printf("Res Read:%d\n", nLen);
	switch (wID)
	{
		case AMMETER_REQ_TOTAL_POWER:
			SaveTotalPower((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_ENERGY:
			SaveEnergy((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_VOLTAGE_A:
			SaveVoltageA((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_VOLTAGE_B:
			SaveVoltageB((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_VOLTAGE_C:
			SaveVoltageC((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_CURRENT_A:
			SaveCurrentA((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_CURRENT_B:
			SaveCurrentB((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		case AMMETER_REQ_CURRENT_C:
			SaveCurrentC((char *)pData + sizeof(wID), nLen - sizeof(wID), wAmmeterNo);
			break;
		default:
			break;
	}
}

void	CtmTaskAmmeterDLT::SaveTotalPower(void *pData, int nLen, WORD wAmmeterNo)
{
	double dPower = 0;
	m_para[wAmmeterNo].dEnergy = BCD2Double(pData, nLen, 2);
	dPower = m_para[wAmmeterNo].dEnergy;
	if (m_notifier)
		m_notifier(AMMETER_NOTIFY_TOTALPOWER, &dPower, sizeof(dPower), wAmmeterNo);
	//printf("total power:%f\n", dValue);
}

void	CtmTaskAmmeterDLT::SaveEnergy(void *pData, int nLen, WORD wAmmeterNo)
{
	double dValue = BCD2Double(pData, nLen, 2);
	
//	if(m_AmmeterConf.AmmeterInfo[wAmmeterNo].wCTPara != 0) //not zero
//			dValue *= m_AmmeterConf.AmmeterInfo[wAmmeterNo].wCTPara ;//乘上電表系數
	m_para[wAmmeterNo].dTotalPower = dValue;
	//printf("SaveEnergy: %f \n", dValue);
	//SetDBValue("AMMETER_HMI_REAL_POWER", (long)(dValue * 100));
	if (m_notifier)
		m_notifier(AMMETER_NOTIFY_TOTALPOWER, &dValue, sizeof(dValue), wAmmeterNo);
}

void	CtmTaskAmmeterDLT::SaveVoltageA(void *pData, int nLen, WORD wAmmeterNo)
{
	m_para[wAmmeterNo].nVoltageA = BCD2Int(pData, nLen);
	//printf("voltage A:%d\n", nValue);
}

void	CtmTaskAmmeterDLT::SaveVoltageB(void *pData, int nLen, WORD wAmmeterNo)
{
	m_para[wAmmeterNo].nVoltageB = BCD2Int(pData, nLen);
	//printf("voltage B:%d\n", nValue);
}

void  	CtmTaskAmmeterDLT::SaveVoltageC(void *pData, int nLen, WORD wAmmeterNo)
{
	m_para[wAmmeterNo].nVoltageC = BCD2Int(pData, nLen);
	//printf("voltage C:%d\n", nValue);
}

void	CtmTaskAmmeterDLT::SaveCurrentA(void *pData, int nLen, WORD wAmmeterNo)
{
	m_para[wAmmeterNo].dCurrentA = BCD2Double(pData, nLen, 2);
	//printf("Current A:%f\n", dValue);
}

void  CtmTaskAmmeterDLT::SaveCurrentB(void *pData, int nLen, WORD wAmmeterNo)
{
	m_para[wAmmeterNo].dCurrentB = BCD2Double(pData, nLen, 2);
	//printf("Current B:%f\n", dValue);
}

void  CtmTaskAmmeterDLT::SaveCurrentC(void *pData, int nLen, WORD wAmmeterNo)
{
	m_para[wAmmeterNo].dCurrentC = BCD2Double(pData, nLen, 2);
	//printf("Current C:%f\n", dValue);
}

double  CtmTaskAmmeterDLT::BCD2Double(void *pData, int nLen, int nPrecision)
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

int		CtmTaskAmmeterDLT::BCD2Int(void *pData, int nLen)
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

UTIME	CtmTaskAmmeterDLT::GetNowTime()
{
	tmDATETIME      Time;
    UTIME           now;
	GetDate(&Time.date);
	GetTime(&Time.time);  
    now = MakeTime(&Time);
	return now;
}

UTIME 	CtmTaskAmmeterDLT::MakeTime(tmDATETIME *tm)
{
	UTIME res;
	int   year;

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
int		CtmTaskAmmeterDLT::ReqFollowData(WORD wID)
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
int		CtmTaskAmmeterDLT::ReqReRead()
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
int 	CtmTaskAmmeterDLT::ReqWrite(WORD wID, void *pBuf, int nDataLen, WORD wAmmeterNo)
{
	int nLen = 0;
	BYTE *pData = NULL;
	
	SetHeader(AMMETER_CONTROL_WRITE, sizeof(wID) + nDataLen, (FRAMEHEADER *)m_sendbuff);
	pData = m_sendbuff + sizeof(FRAMEHEADER);
	*pData++ = wID & 0x00FF;
	*pData = wID & 0xFF00;
	memcpy(pData, pBuf, nDataLen);
	SetFooter((FRAMEHEADER *)m_sendbuff);
	
	nLen = sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER) + ((FRAMEHEADER *)m_sendbuff)->len;
	return SendData(m_sendbuff, nLen);
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
int 	CtmTaskAmmeterDLT::ReqAdjustTime()
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
int 	CtmTaskAmmeterDLT::ReqWriteAddr(WORD wID, void *pAddr)
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
int 	CtmTaskAmmeterDLT::ReqChangeRate(WORD wID, BYTE bRate)
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

#ifndef	D_BEOBJECT_CTMTASKAMMETER
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
