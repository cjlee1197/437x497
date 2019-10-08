#include	"tmTaskAmmeterModbus.h"
#include	"../../app.h"
#include	"../../main.h"
#include	"../../utils.h"
#include	"../libControl.h"
#include	"../../commonaction.h"

#define		MODBUS_FRAME_ADDR_POS		0
#define		MODBUS_FRAME_CODE_POS		1
#define		MODBUS_FRAME_STARTADDR_POS	2
#define		MODBUS_FRAME_VALUE_POS		4

AMMETERCONFIG	CtmTaskAmmeterModbus::m_AmmeterConf = 
{
	1, {1, 0}
};

IMPLEMENT_DYNCREATE(CtmTaskAmmeterModbus, CTaskAmmeter)

CtmTaskAmmeterModbus::CtmTaskAmmeterModbus(): CTaskAmmeter()
{
	m_nRegType = MODBUS_TYPE_NONE;
	memset(&m_para, 0, sizeof(m_para));
	memset(&m_RS485Ammeter, 0, sizeof(m_RS485Ammeter));
	
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
	m_RS485Ammeter.nBaudRate 				= B9600;
	m_RS485Ammeter.wChannel 				= _COM2;
	
	if(Open(&m_RS485Ammeter) < 0 )
	{
		printf("OpenAmmeter ModBus error \n");	
	}
	else
	{
		printf("OpenAmmeter ModBus ok \n");		
	}
	
	m_bReadConfFlag = FALSE;
	
	ReadConf();	//2012-11-22 
	CTaskAmmeter::CTaskAmmeter();
}

CtmTaskAmmeterModbus::~CtmTaskAmmeterModbus()
{
}

//	execute it atfer the contructor
int		CtmTaskAmmeterModbus::CreateSelf()
{
	CTaskAmmeter::CreateSelf();
	
	AddCounter(&m_lCheckTimer, m_idMe);
	RegisterMessage(MSG_AMMETER, m_idMe);
	
	return 0;
}

//	execute it before the destructor
int		CtmTaskAmmeterModbus::FreeSelf()
{
	DeleteCounter(&m_lCheckTimer, m_idMe);
	UnRegisterAllMessage(MSG_AMMETER);
	
	return CTaskAmmeter::FreeSelf();
}

void	CtmTaskAmmeterModbus::Run()
{	
	
}

int CtmTaskAmmeterModbus::ReadHoldRegister(BYTE slaveID, WORD wStartAddr, WORD wNum)
{
	WORD wCRC = 0xffff;
	//printf("read hold register:slave %d, startAddr %x, num %d\n", slaveID, wStartAddr, wNum);
	m_sendbuff[MODBUS_FRAME_ADDR_POS] = slaveID;						//01 
	m_sendbuff[MODBUS_FRAME_CODE_POS] = MODBUS_CODE_READ_HOLDREGISTER;	//03 
	
	m_sendbuff[MODBUS_FRAME_STARTADDR_POS] = (wStartAddr >> 8) & 0xFF;		// start address big-endian
	m_sendbuff[MODBUS_FRAME_STARTADDR_POS + 1] = wStartAddr & 0xFF;
	
	m_sendbuff[MODBUS_FRAME_VALUE_POS] = (wNum >> 8) & 0xFF;				//register number big-endian
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 1] = wNum & 0xFF;
	
	wCRC = CRC16(wCRC, (char *)m_sendbuff, 6);								//crc16-modbus calculation
	
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 2] = wCRC & 0xFF;					//crc-16 field little-endian
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 3] = (wCRC >> 8) & 0xFF;
	
	SendData(m_sendbuff, 8);
	
	return 0;
}

int CtmTaskAmmeterModbus::WriteSingleRegister(BYTE slaveID, WORD wStartAddr, WORD wValue)
{
	WORD wCRC = 0xffff;
	
	m_sendbuff[MODBUS_FRAME_ADDR_POS] = slaveID;
	m_sendbuff[MODBUS_FRAME_CODE_POS] = MODBUS_CODE_WRITE_SINGLE_REGISTER;
	
	m_sendbuff[MODBUS_FRAME_STARTADDR_POS] = (wStartAddr >> 8) & 0xFF;		// start address big-endian
	m_sendbuff[MODBUS_FRAME_STARTADDR_POS + 1] = wStartAddr & 0xFF;
	
	m_sendbuff[MODBUS_FRAME_VALUE_POS] = (wValue >> 8) & 0xFF;				//register number big-endian
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 1] = wValue & 0xFF;
	
	wCRC = CRC16(wCRC, (char *)m_sendbuff, 6);								//crc16-modbus calculation
	
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 2] = wCRC & 0xFF;					//crc-16 field little-endian
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 3] = (wCRC >> 8) & 0xFF;
	
	SendData(m_sendbuff, 8);
	
	return 0;
}

int CtmTaskAmmeterModbus::WriteRegisters(BYTE slaveID, WORD wStartAddr, int nNum, WORD* wValue)
{
	WORD wCRC = 0xffff, wTemp = 0;
	BYTE bSize = 0;
	int i = 0, j = 0;
	//printf("write register:slave %d, startAddr %x, num %d\n", slaveID, wStartAddr, nNum);
	
	m_sendbuff[MODBUS_FRAME_ADDR_POS] = slaveID;
	m_sendbuff[MODBUS_FRAME_CODE_POS] = MODBUS_CODE_WRITE_REGISTERS;
	
	m_sendbuff[MODBUS_FRAME_STARTADDR_POS] = (wStartAddr >> 8) & 0xFF;		// start address big-endian
	m_sendbuff[MODBUS_FRAME_STARTADDR_POS + 1] = wStartAddr & 0xFF;
	
	m_sendbuff[MODBUS_FRAME_VALUE_POS] = (nNum >> 8) & 0xFF;				//register number big-endian
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 1] = nNum & 0xFF;
	
	bSize = nNum * 2;
	m_sendbuff[MODBUS_FRAME_VALUE_POS + 2] = bSize;							//size of data big-endian
	
	for (i = 0, j = 0; i < nNum; i++)
	{
		wTemp = wValue[i];
		//printf("i:%d, %d, wTemp:%x \n", i, j, wTemp);
		m_sendbuff[MODBUS_FRAME_VALUE_POS + 3 + j] = (wTemp >> 8) & 0xFF;
		//printf(" %d, wTemp:%x \n", MODBUS_FRAME_VALUE_POS + 3 + j, m_sendbuff[MODBUS_FRAME_VALUE_POS + 3 + j]);
		j++;
		m_sendbuff[MODBUS_FRAME_VALUE_POS + 3 + j] = wTemp & 0xFF;
		//printf("2 %d wTemp:%x \n", MODBUS_FRAME_VALUE_POS + 3 + j, m_sendbuff[MODBUS_FRAME_VALUE_POS + 3 + j]);
		j++;
	}
	
	wCRC = CRC16(wCRC, (char *)m_sendbuff, 7 + nNum * 2);
	
	m_sendbuff[7 + nNum * 2] = wCRC & 0xFF;					//crc-16 field little-endian
	m_sendbuff[7 + nNum * 2 + 1] = (wCRC >> 8) & 0xFF;
	
	SendData(m_sendbuff, 9 + nNum * 2);
	
	return 0;
}

int	CtmTaskAmmeterModbus::Request(int nType, WORD wAmmeterNo)
{
	WORD  wTemp =0;
	WaitSend();
	
	switch (nType)
	{
		case AMMETER_ENERGY:
			m_nRegType = MODBUS_TYPE_ENERGY;
			//ReadHoldRegister(1, 0, 2);
			ReadHoldRegister(1, 0x64, 2);
			//wTemp = 0xffff;
			//WriteRegisters(1, 0x64, 2, &wTemp);
			break;
		case AMMETER_VOLTAGE:
			m_nRegType = MODBUS_TYPE_VOLTAGE;
			ReadHoldRegister(1, 2, 2);
			break;
		case AMMETER_CURRENT:
			m_nRegType = MODBUS_TYPE_CURRENT;
			ReadHoldRegister(1, 4, 2);
			break;
		case AMMETER_TOTAL_POWER:
			m_nRegType = MODBUS_TYPE_POWER;
			ReadHoldRegister(1, 6, 2);
			break;
		case AMMETER_READ_CT:
			m_nRegType = MODBUS_TYPE_CT_R;
			//ReadHoldRegister(1, 0x15, 1);
			ReadHoldRegister(1, 0x3C, 1);
			break;
		case AMMETER_WRITE_ENERGYRESET:
			WriteRegisters(1, 0x64, 2, &wTemp);  /*JOYCE2014-1-9 add 海天COM2松下電表  電表電能複位*/
			break;
		default:
			break;
	}
	
	StartRecv();
	
	return 0;
}

int	CtmTaskAmmeterModbus::Write(int nType, void *pData, int nLen)
{
	WORD wTemp = 0;
	
	if (nLen <= 0 || (nLen & 1) != 0)
		return -1;
	
	WaitSend();
	
	switch (nType)
	{
		case AMMETER_WRITE_CT:
			memcpy(&wTemp, pData, sizeof(wTemp));
			WriteSingleRegister(1, 0x15, wTemp);
			break;
		default:
			break;
	}
	
	StartRecv();
	
	return 0;
}

int	CtmTaskAmmeterModbus::Parse(void *pBuf, int nLen)
{	
	WORD wCRC = 0xffff;
	BYTE crc[2], bCode = 0;
	static int nPos = 0;
	BYTE bLen = 0;
	
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

		if (nLen > 0 && nPos + nLen < (int)MODBUS_RTU_FRAME_MAXLEN)		//check array overflow
		{
			//printf("%x \n", *(char *)pBuf);
			memcpy(m_readbuff + nPos, pBuf, nLen);
			
			nPos += nLen;
			m_nErrCount = 0;
			return nLen;
		}
		
		if (nPos == 0)
			break;
		
		g_bAmmeterStatus = TRUE;
		
		wCRC = CRC16(wCRC, (char *)m_readbuff, nPos - sizeof(wCRC));
		crc[0] = wCRC & 0xFF;
		crc[1] = (wCRC >> 8) & 0xFF;
		//printf("CRC: %x, %x\n", crc[0], crc[1]);
		if (memcmp(&crc, m_readbuff + nPos - sizeof(wCRC), 2) != 0)
			break;
		
		memcpy(&bCode, m_readbuff + 1, 1);
		//printf("bCode:%x\n", bCode);
		
		switch (bCode)
		{
			case MODBUS_CODE_READ_HOLDREGISTER:
				memcpy(&bLen, m_readbuff + 2, 1);
				ResReadHoldRegister((void *)(m_readbuff + 3), bLen);
				//printf("parse test0\n");
				break;
			case MODBUS_CODE_WRITE_SINGLE_REGISTER:
				//printf("res Write single register\n");
				break;
			default:
				break;
		}
		
	} while(0);
	
	nPos = 0;
	
	StartSend();
	WaitRecv();
	
	return nLen;
}

/*
 * first, we need 4-bytes data filled with 0xFE to wakeup the device.
 */
int CtmTaskAmmeterModbus::SendData(void *pData, int nLen)
{
	if (m_fdcom >= 0)
	{
		write(m_fdcom, pData, nLen);
	}
	return CTaskAmmeter::SendData(pData, nLen);	
}

void CtmTaskAmmeterModbus::ResReadHoldRegister(void *pData, int nLen)
{
	WORD wTemp = 0;
	DWORD dwTemp = 0;
	int i;
	
	for (i = 0; i < nLen / 2; i++)
	{
		memcpy(&wTemp, (char *)pData + i * 2, sizeof(wTemp));
		wTemp = (wTemp >> 8) | (wTemp << 8);
		memcpy((char *)pData + i * 2, &wTemp, sizeof(wTemp));
	}
	
	if (nLen == 4)
	{
		memcpy(&(dwTemp), pData, nLen);
		dwTemp = (dwTemp >> 16) | (dwTemp << 16);
		memcpy(pData, &dwTemp, nLen);
	}
	
	//printf("m_nRegType=%x nLen=%d \n", m_nRegType, nLen);
	switch (m_nRegType)
	{
		case MODBUS_TYPE_ENERGY:
			SaveEnergy(pData, nLen);
			break;
		case MODBUS_TYPE_VOLTAGE:
			SaveVoltage(pData, nLen);
			break;
		case MODBUS_TYPE_CURRENT:
			SaveCurrent(pData, nLen);
			break;
		case MODBUS_TYPE_POWER:
			SaveTotalPower(pData, nLen);
			break;
		case MODBUS_TYPE_CT_R:
			SaveCT(pData, nLen, 1);
			break;
		default:
			break;
	}
	
	m_nRegType = MODBUS_TYPE_NONE;
}

void  CtmTaskAmmeterModbus::SaveEnergy(void *pData, int nLen)
{	
	double dPower = 0;
	DWORD dwTemp = 0;
	if (nLen == 4)
	{
		memcpy(&dwTemp, pData, nLen);
		m_para.dwEnergy = ((dwTemp & 0xffff) <<16) | ((dwTemp >>16) & 0xffff);		// 
		//SetDBValue("AMMETER_HMI_REAL_POWER", m_para.dwEnergy);
		//printf("Energy:%x\n", m_para.dwEnergy);
	}
	dPower = (double)m_para.dwEnergy /100;  //for precision =2 
	//printf("dEnergy:%f\n", dPower);
	if (m_notifier)
		m_notifier(AMMETER_NOTIFY_TOTALPOWER, &dPower, sizeof(dPower), 0);
}

void  CtmTaskAmmeterModbus::SaveVoltage(void *pData, int nLen)
{
	if (nLen == 4)
		memcpy(&(m_para.nVoltage), pData, nLen);
	//printf("m_para.nVoltage=%d \n", m_para.nVoltage);
}

void  CtmTaskAmmeterModbus::SaveCurrent(void *pData, int nLen)
{
	if (nLen == 4)
		memcpy(&(m_para.nCurrent), pData, nLen);
	//printf("m_para.nCurrent=%d \n", m_para.nCurrent);
}

void  CtmTaskAmmeterModbus::SaveTotalPower(void *pData, int nLen)
{
	if (nLen == 4)
		memcpy(&(m_para.nTotalPower), pData, nLen);
		
	//printf("total power:%d \n", m_para.nTotalPower);
}

void  CtmTaskAmmeterModbus::SaveCT(void *pData, int nLen, int index)
{
	if (nLen == 2)
		memcpy(&wTemp, pData, nLen);
}

int  CtmTaskAmmeterModbus::ReadInt(int nType, WORD wAmmeterNo)
{
	int nValue = 0;
	
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			nValue = m_para.nTotalPower;
			break;
		case AMMETER_ENERGY:
			nValue = m_para.dwEnergy;
			break;
		case AMMETER_VOLTAGE:
			nValue = m_para.nVoltage;
			break;
		case AMMETER_CURRENT:
			nValue = m_para.nCurrent;
			break;
		case AMMETER_READ_CT:
			nValue = m_para.wCT;
			break;
		default:
			break;
	}
	
	return nValue;
}

double	CtmTaskAmmeterModbus::ReadDouble(int nType, WORD wAmmeterNo)
{
	double dValue = 0.0F;
	
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			dValue = (double)m_para.nTotalPower;
			break;
		case AMMETER_ENERGY:
			dValue = (double)m_para.dwEnergy;
			break;
		case AMMETER_VOLTAGE:
			dValue = (double)m_para.nVoltage;
			break;
		case AMMETER_CURRENT:
			dValue = (double)m_para.nCurrent;
			break;
		case AMMETER_READ_CT: 
			dValue = (double)m_para.wCT;
			break;
		default:
			break;
	}
	return dValue;
}

float	CtmTaskAmmeterModbus::ReadFloat(int nType, WORD wAmmeterNo)
{
	float fValue = 0.0F;
	
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			fValue = (float)m_para.nTotalPower;
			break;
		case AMMETER_ENERGY:
			fValue = (float)m_para.dwEnergy;
			break;
		case AMMETER_VOLTAGE:
			fValue = (float)m_para.nVoltage;
			break;
		case AMMETER_CURRENT:
			fValue = (float)m_para.nCurrent;
			break;
		case AMMETER_READ_CT:
			fValue = (float)m_para.wCT;
			break;
		default:
			break;
	}
	return fValue;
}


void* CtmTaskAmmeterModbus::ReadConf()
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
	m_bReadConfFlag = TRUE;
	return &m_AmmeterConf;
}

int 	CtmTaskAmmeterModbus::WriteConf(void* pConf)
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
