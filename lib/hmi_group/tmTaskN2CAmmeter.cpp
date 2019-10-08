#include	"tmTaskN2CAmmeter.h"
#include	"../../app.h"
#include	"../../main.h"
#include	"../../utils.h"
#include	"../libControl.h"
#include	"../libComm.h"
#include	"../../commonaction.h"
#include 	"../../tmshmsg.h"
#include	"../../iotable.h"

#define     MINUTE                           60
#define     HOUR                             (60*MINUTE)
#define     DAY                              (24*HOUR)
#define     YEAR                             (365*DAY)

#define		AMMETER_TIMEOUT		15000
#define		AMMETER_VOLTIME		(30 * 1024)	
#define		RUN_CHECK_TIMER		1000
#define		MAX_NUM				0
#define		MIN_NUM				1


BOOL	bAmmeterStatusOld = FALSE;
BOOL	bAmtNoDeviatError = FALSE;
BOOL	bAmtVolA      = FALSE;
BOOL	bAmtVolB      = FALSE;
BOOL	bAmtVolC      = FALSE;
BOOL	bAmtVolErrorA      = FALSE;
BOOL	bAmtVolTimeOutA    = FALSE;
BOOL	bAmtVolErrorB      = FALSE;
BOOL	bAmtVolTimeOutB    = FALSE;
BOOL	bAmtVolErrorC      = FALSE;
BOOL	bAmtVolTimeOutC    = FALSE;
BOOL	bAmtConnectTimeOut= FALSE;

AMMETERCONFIG	CtmTaskN2CAmmeter::m_AmmeterConf = 
{
	1, {32, 0}
};

long	lTimer1 = AMMETER_TIMEOUT,lTimerA = AMMETER_VOLTIME,lTimerB = AMMETER_VOLTIME,lTimerC = AMMETER_VOLTIME;
long	lErrNum = 0;
IMPLEMENT_DYNCREATE(CtmTaskN2CAmmeter, CTaskAmmeter)

CtmTaskN2CAmmeter::CtmTaskN2CAmmeter(): CTaskAmmeter()
{
	m_nErrCount = 0;
	
	memset(m_para, 0, sizeof(m_para));
	memset(&m_Head, 0, sizeof(m_Head));
	m_nDataState = 0; //0=normal, 1= comm error , 2= id error
	AddCounter(&lTimer1, m_idMe);
	m_bReadConfFlag = FALSE;
	ReadConf();
	//AddTimer(&lTimer1);
	
}

CtmTaskN2CAmmeter::~CtmTaskN2CAmmeter()
{
	//DeleteTimer(&lTimer1);
	DeleteCounter(&lTimer1, m_idMe);
}

//	execute it atfer the contructor
int		CtmTaskN2CAmmeter::CreateSelf()
{
	CTaskAmmeter::CreateSelf();
		
	return 0;
}

//	execute it before the destructor
int		CtmTaskN2CAmmeter::FreeSelf()
{
	return CTaskAmmeter::FreeSelf();
}

/*
void	CtmTaskN2CAmmeter::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	CTask::WndProc(message, wParam, lParam);
}
*/
void	CtmTaskN2CAmmeter::Run()
{
	WORD  wValue = 0;
	if(lTimer1 < 0) 
	{
		//if(g_bAmmeterStatus)	//2013-5-8 
		//{
		//	printf("testamt 007\n");
		//	wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
		//    wValue &= ~COMM_S_MOTORPHASEERR;
		//    SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8 
		
		//    bAmmeterStatusOld = FALSE;
		//}
		//printf("testamt 009\n");
		g_bAmmeterStatus = FALSE;
		
	}
	if(bAmtVolErrorA)
	{
		AddCounter((long *)&lTimerA, m_idMe);
	    if(lTimerA < 0)
         {
           DeleteCounter((long *)&lTimerA, m_idMe);
           lTimerA =AMMETER_VOLTIME;
           bAmtVolTimeOutA=TRUE;
         }	
	}
	else
	{
		if(lTimerA !=AMMETER_VOLTIME)
		{
		DeleteCounter((long *)&lTimerA, m_idMe);
        lTimerA =AMMETER_VOLTIME;
        bAmtVolTimeOutA=FALSE;	
        }
	}
	if(bAmtVolErrorB)
	{
		AddCounter((long *)&lTimerB, m_idMe);
	    if(lTimerB < 0)
         {
           DeleteCounter((long *)&lTimerB, m_idMe);
           lTimerB =AMMETER_VOLTIME;
           bAmtVolTimeOutB=TRUE;
         }	
	}
	else
	{
		if(lTimerB !=AMMETER_VOLTIME)
		{
		DeleteCounter((long *)&lTimerB, m_idMe);
        lTimerB =AMMETER_VOLTIME;
        bAmtVolTimeOutB=FALSE;	
        }
	}
	if(bAmtVolErrorC)
	{
		AddCounter((long *)&lTimerC, m_idMe);
	    if(lTimerC < 0)
         {
           DeleteCounter((long *)&lTimerC, m_idMe);
           lTimerC =AMMETER_VOLTIME;
           bAmtVolTimeOutC=TRUE;
         }	
	}
	else
	{
		if(lTimerC !=AMMETER_VOLTIME)
		{
		DeleteCounter((long *)&lTimerC, m_idMe);
        lTimerC =AMMETER_VOLTIME;
        bAmtVolTimeOutC=FALSE;	
        }
	}
	if((g_ptaskdsp28_N2C!=NULL && g_ptaskdsp28_N2C->GetOnLineStatus()) || (g_ptaskdsp28_Austone!=NULL && g_ptaskdsp28_Austone->GetOnLineStatus()))
	{
		if(g_bAmmeterStatus)	//2013-5-8 
		{
			wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
		    wValue &= ~COMM_S_MOTORPHASEERR;
		    SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8 
		    bAmmeterStatusOld = FALSE;
		}
		g_bAmmeterStatus = FALSE;
	}
}


/*
 * parse the date we received from device.
 * when the length of data we received is zero or the buffer is full,
 * we start parsing the data.
 * if received zero length data more than 4 times, then reopen the device.
 *
 */
int		CtmTaskN2CAmmeter::ParseData(void *pData, int nLen)
{
	lTimer1 = AMMETER_TIMEOUT;
	WORD	wCycle = 0, wCmd = 0, wElement = 0, wID = 0, wOffset = 0, wValue = 0;
	BYTE 	bType = 0, bNode = 0;
	//static	int	nDataState = 0;   //0=normal, 1= comm error , 2= id error
	//m_bMachineID = -1; /*JOYCE2014-4-22 MARK,挪到循環體中,解決g_bAmmeterStatus無故被改變的問題*/
	memcpy(&wCycle, (BYTE*)pData+wOffset, sizeof(wCycle));
	wOffset += sizeof(wCycle);
	
	memcpy(&wCmd, (BYTE*)pData+wOffset, sizeof(wCmd));
	wOffset += sizeof(wCmd);
	
	memcpy(&wElement, (BYTE*)pData+wOffset, sizeof(wElement));
	wOffset += sizeof(wElement);
	
	for(int i = 0; i < wElement; i++)
	{
		m_bMachineID = 0xFF;/*JOYCE2014-4-22 add 無符號類型,給-1有問題*/
		memcpy(&bNode, (BYTE*)pData+wOffset, sizeof(bNode));
		wOffset += sizeof(bNode);
		/*if(m_bMachineID == 0)	//0 is comm error, 0xFF is id error
		{
			g_bAmmeterStatus = FALSE;
			if(m_nDataState == 0)
			{
				SendMsg(MSG_SH_AMMETER_COMMERROR,0, 0, NULL);
			}
			m_nDataState = 1;	
			return nLen;
		}
		else if(m_bMachineID == 0xFF)
		{
			if(m_nDataState == 0)
			{
				SendMsg(MSG_SH_AMMETER_IDERROR,0, 0, NULL);
			}
			m_nDataState = 2;	
			return nLen;	
		}
		else
			m_nDataState = 0;*/
			
		for(int i = 0; i < m_AmmeterConf.wCount; i++)
		{
			if(bNode == m_AmmeterConf.AmmeterInfo[i].wNode) 
			{
				m_bMachineID = i;	
				break;
			}
		}
		if(m_bMachineID == 0xFF) return nLen;/*當電力計上傳的節點號和HMI申請的不一致時, 直接返回不做處理*/
		//printf("222 %d:m_bMachineID=%d\n",i,m_bMachineID);
		
		memcpy(&bType, (BYTE*)pData+wOffset, sizeof(bType));
		wOffset += sizeof(bType);
		//printf("i=%d,bNode=%d, bType = %d, %d\n", i,bNode, bType, m_nDataState);
		if(bType == 1)
		{
			g_bAmmeterStatus = g_bAmmeterStatus & (~(1 << m_bMachineID));
			//g_bAmmeterStatus = FALSE;
			if(m_nDataState == 0)
			{
				SendMsg(MSG_SH_AMMETER_COMMERROR,bNode, 0, NULL);
				bAmtConnectTimeOut =TRUE;
			}
			m_nDataState = 1;	
			//return nLen;	
			wOffset += sizeof(wID);
			wOffset += sizeof(float);
			continue;
		}
		else if(bType == 2)
		{
			g_bAmmeterStatus = g_bAmmeterStatus & (~(1 << m_bMachineID));
			if(m_nDataState == 0)
			{
				SendMsg(MSG_SH_AMMETER_IDERROR,bNode, 0, NULL);
			}
			m_nDataState = 2;	
			//return nLen;	
			wOffset += sizeof(wID);
			wOffset += sizeof(float);
			continue;
		}
		else
			{
			if(bAmtConnectTimeOut)
			  {
			     SendMsg(MSG_USER_SH_PROMPTNULL, 0, 0, NULL); 
			     bAmtConnectTimeOut =FALSE;
			  }
			m_nDataState = 0;
		    }
		
		memcpy(&wID, (BYTE*)pData+wOffset, sizeof(wID));
		wOffset += sizeof(wID);
		float fTemp = 0;
		memcpy(&fTemp, pData, sizeof(float));
		//printf("i=%d m_bMachineID=%d , wID = %04x, Value = %f\n", i, m_bMachineID, wID,fTemp);
		wID = wID & 0x0FFF;
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
				bAmtVolA =TRUE;
				DearlErrorVoltage(0, m_bMachineID,(char*)pData+wOffset);
				bAmtVolA =FALSE;
				break;
			case AMMETER_RS485ID_BVRMS:			//AMMETER_REQ_VOLTAGE_B:
				SaveVoltageB((char *)pData + wOffset, nLen - sizeof(wID));
				bAmtVolB =TRUE;
				DearlErrorVoltage(1, m_bMachineID,(char*)pData+wOffset);
				bAmtVolB =FALSE;
				break;
			case AMMETER_RS485ID_CVRMS:			//AMMETER_REQ_VOLTAGE_C:
				SaveVoltageC((char *)pData + wOffset, nLen - sizeof(wID));
				bAmtVolC =TRUE;
				DearlErrorVoltage(2, m_bMachineID,(char*)pData+wOffset);
				bAmtVolC =FALSE;
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
			case AMMETER_RS485ID_NEGA_VAR:
				SavePhaseDeviation((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_PF:
				SavePowerFactor((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_NEGA_WATTHR:
				SaveDataVersion((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_AWATT:	
				SavePowerA((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_BWATT:	
				SavePowerB((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			case AMMETER_RS485ID_CWATT:	
				SavePowerC((char *)pData + wOffset, nLen - sizeof(wID));
				break;
			default:
				break;
		}
		wOffset += sizeof(float);
		g_bAmmeterStatus = g_bAmmeterStatus | (1 << m_bMachineID);
	}
	
	if(g_bAmmeterStatus && bAmtNoDeviatError)	//2013-5-8 
	{
		bAmtNoDeviatError = FALSE;
	}
	
	if((!bAmtNoDeviatError) &&(!g_bAmmeterStatus))	//2013-5-8 
	{
		wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
		wValue &= ~COMM_S_MOTORPHASEERR;
		SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8 
		
		bAmtNoDeviatError = TRUE;	
	}
	
	//g_bAmmeterStatus = TRUE;
	lTimer1 = AMMETER_TIMEOUT;
	return nLen;
}


int		CtmTaskN2CAmmeter::ReqRead(WORD wID)
{
	int nLen = 0;
	BYTE bType = 0;
	WORD wDataID = 0;
	m_Head.wCycleTime = 0;
	m_Head.wCmd = 1;			//1:read, 2:write
	m_Head.wElementCount = 1;
	
	memcpy(m_sendbuff + nLen, &m_Head, sizeof(m_Head));
	nLen += sizeof(m_Head);
	
	m_bMachineID = (wID & 0xFF00) >> 8; 
	memcpy(m_sendbuff + nLen, &m_bMachineID, sizeof(m_bMachineID));
	nLen += sizeof(m_bMachineID);
	
	memcpy(m_sendbuff + nLen, &bType, sizeof(bType));
	nLen += sizeof(bType);
	
	wDataID = wID & 0x00FF | N2C_AMMETER_DATA;
	memcpy(m_sendbuff + nLen, &wDataID, sizeof(wDataID));
	nLen += sizeof(wDataID);
	
	//printf("CtmTaskN2CAmmeter ReqRead wDataID = %04x\n", wDataID);
	
	if(g_ptaskdsp28_N2C != NULL)
		g_ptaskdsp28_N2C->ReqValues(15, (void*)m_sendbuff, nLen);
	if(g_ptaskdsp28_Austone != NULL)
		g_ptaskdsp28_Austone->ReqValues(15, (void*)m_sendbuff, nLen);
		
	return 0;
}

int		CtmTaskN2CAmmeter::ReqRead(BYTE bCode, WORD* pwID, WORD wElement)
{
	int nLen = 0;
	
	BYTE bType = TYPE_FLOAT;
	WORD wDataID = 0;
	m_Head.wCmd = bCode;			//1:read, 2:write
	m_Head.wCycleTime = 0;
	m_Head.wElementCount = wElement;
	
	if(bCode == AMMETER_CONTROL_ACTUALVALUE)
	{
		m_Head.wCycleTime = *(pwID + wElement - 1);
		m_Head.wElementCount = wElement - 1;
	}
	
	memcpy(m_sendbuff + nLen, &m_Head, sizeof(m_Head));
	nLen += sizeof(m_Head);
//	printf("wCmd =%d, wCycleTime = %d, wElementCount = %d\n", 
//			m_Head.wCmd, m_Head.wCycleTime, m_Head.wElementCount);
	for(int i = 0; i < m_Head.wElementCount; i++)
	{
		m_bMachineID = (pwID[i] & 0xFF00) >> 8;
		memcpy(m_sendbuff + nLen, &m_bMachineID, sizeof(m_bMachineID));
		nLen += sizeof(m_bMachineID);
		
		memcpy(m_sendbuff + nLen, &bType, sizeof(bType));
		nLen += sizeof(bType);
		
		wDataID = pwID[i] & 0x00FF | N2C_AMMETER_DATA;
		memcpy(m_sendbuff + nLen, &wDataID, sizeof(wDataID));
		nLen += sizeof(wDataID);
//		printf("CtmTaskN2CAmmeter ReqRead m_bMachineID =%d, wDataID = %04x, sizeof(AmmeterHeader) = %d\n",
//				 m_bMachineID, wDataID, sizeof(AmmeterHeader));
	}
	
	if(g_ptaskdsp28_N2C != NULL)
		g_ptaskdsp28_N2C->ReqValues(15, (void*)m_sendbuff, nLen);
	if(g_ptaskdsp28_Austone != NULL)
		g_ptaskdsp28_Austone->ReqValues(15, (void*)m_sendbuff, nLen);
			
	return 0;
}

int CtmTaskN2CAmmeter::Write(int nType, void *pData, int nLen)
{
	return ReqWrite(nType, pData, nLen);	
}

int CtmTaskN2CAmmeter::ReqWrite(WORD wID, void *pBuf, int nDataLen)
{
	int nLen = 0;
	
	BYTE bType = TYPE_FLOAT;
	WORD wDataID = 0;
	m_Head.wCycleTime = 0;
	m_Head.wCmd = 2;			//1:read, 2:write
	m_Head.wElementCount = 1;
	
	memcpy(m_sendbuff + nLen, &m_Head, sizeof(m_Head));
	nLen += sizeof(m_Head);
	
	m_bMachineID = (wID & 0xFF00) >> 8; 
	memcpy(m_sendbuff + nLen, &m_bMachineID, sizeof(m_bMachineID));
	nLen += sizeof(m_bMachineID);
	
	memcpy(m_sendbuff + nLen, &bType, sizeof(bType));
	nLen += sizeof(bType);
	
	wDataID = wID & 0x00FF | N2C_AMMETER_DATA;
	memcpy(m_sendbuff + nLen, &wDataID, sizeof(wDataID));
	nLen += sizeof(wDataID);
	
	memcpy(m_sendbuff + nLen, pBuf, nDataLen);
	nLen += nDataLen;
	//printf("CtmTaskN2CAmmeter ReqWrite wDataID = %04x\n", wDataID);
	
	if(g_ptaskdsp28_N2C != NULL)
		g_ptaskdsp28_N2C->ReqValues(15, (void*)m_sendbuff, nLen);
	if(g_ptaskdsp28_Austone != NULL)
		g_ptaskdsp28_Austone->ReqValues(15, (void*)m_sendbuff, nLen);
			
	return 0;
}

int		CtmTaskN2CAmmeter::Request(int nType, WORD wAmmeterNo)
{
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_POSI_WATTHR, 0, NULL);
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
		case AMMETER_POWER_A:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_AWATT, 0, NULL);
			break;
		case AMMETER_POWER_B:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_BWATT, 0, NULL);
			break;
		case AMMETER_POWER_C:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_CWATT, 0, NULL);
			break;
		case AMMETER_WRITE_TEST:
			Request(AMMETER_CONTROL_READ, AMMETER_RS485ID_TEST, 0, NULL);
			break;
		case 0xFF01:			//James add 2012/4/5 下午 02:19:03
			return m_nDataState;
			break;
		default:
			break;
	}
	return 0;
}

int 	CtmTaskN2CAmmeter::Request(BYTE bCode, WORD wID, int nLen, BYTE *pData, WORD wAmmeterNo)
{	
	switch(bCode)
	{
		case AMMETER_CONTROL_READ:
			ReqRead(wID);
			break;
		case AMMETER_CONTROL_WRITE:
			ReqWrite(wID, pData, nLen);
			break;
		default:
			break;
	}
	return 0;
}



int		CtmTaskN2CAmmeter::Request(BYTE bCode, WORD* pwID, WORD wElement, WORD wAmmeterNo)
{
	switch(bCode)
	{
		case AMMETER_CONTROL_READ:
			ReqRead(1, pwID, wElement);
			break;
		case AMMETER_CONTROL_ACTUALVALUE:
			ReqRead(3, pwID, wElement);
			break;
	}
	return 0;
}

int		CtmTaskN2CAmmeter::ReadInt(int nType, WORD wAmmeterNo)
{
	int nValue = 0;
	WORD 	wNode = 0;
	m_bMachineID = -1;
	
	wNode = (nType & 0xFF00) >> 8;
	if(wNode == 0) wNode = 32;
	
	for(int i = 0; i < m_AmmeterConf.wCount; i++)
	{
		if(wNode == m_AmmeterConf.AmmeterInfo[i].wNode) 
		{
			m_bMachineID = i;	
			break;
		}
	}
	if(m_bMachineID == -1) return 0.0;
	nType = nType & 0x00FF;
	//printf("Read nType=%d \n", nType);
	switch (nType)
	{
		case AMMETER_DATAVER:
			nValue = m_para[m_bMachineID].dwDataVer;
			//printf("nValue ... is %x \n", nValue);
			break;
		case AMMETER_DEVIATION:
			nValue = m_para[m_bMachineID].dwPhaseDeviation;
			//printf("nValue ... is %x \n", nValue);
			break;
		default:
			break;
	}
	return nValue;
}

double	CtmTaskN2CAmmeter::ReadDouble(int nType, WORD wAmmeterNo)
{
	double dValue = 0;
	return dValue;
}

float	CtmTaskN2CAmmeter::ReadFloat(int nType, WORD wAmmeterNo)
{
	float 	fValue = 0;
	WORD 	wNode = 0;
	m_bMachineID = -1;
	
	wNode = (nType & 0xFF00) >> 8;
	if(wNode == 0) wNode = 32;
	
	for(int i = 0; i < m_AmmeterConf.wCount; i++)
	{
		if(wNode == m_AmmeterConf.AmmeterInfo[i].wNode) 
		{
			m_bMachineID = i;	
			break;
		}
	}
	if(m_bMachineID == -1) return 0.0;
	
	nType = nType & 0x00FF;
	switch (nType)
	{
		case AMMETER_TOTAL_POWER:
			fValue = m_para[m_bMachineID].fTotalPower;
			break;
		case AMMETER_ENERGY:
			fValue = m_para[m_bMachineID].fEnergy;
			break;
		case AMMETER_VOLTAGE_A:
			fValue = m_para[m_bMachineID].fVoltageA;
			break;
		case AMMETER_VOLTAGE_B:
			fValue = m_para[m_bMachineID].fVoltageB;
			break;
		case AMMETER_VOLTAGE_C:
			fValue = m_para[m_bMachineID].fVoltageC;
			break;
		case AMMETER_CURRENT_A:
			fValue = m_para[m_bMachineID].fCurrentA;
			break;
		case AMMETER_CURRENT_B:
			fValue = m_para[m_bMachineID].fCurrentB;
			break;
		case AMMETER_CURRENT_C:
			fValue = m_para[m_bMachineID].fCurrentC;
			break;
		case AMMETER_VOLTAGE_AB:
			fValue = m_para[m_bMachineID].fVoltageAB;
			break;
		case AMMETER_VOLTAGE_BC:
			fValue = m_para[m_bMachineID].fVoltageBC;
			break;
		case AMMETER_VOLTAGE_AC:
			fValue = m_para[m_bMachineID].fVoltageAC;
			break;
		case AMMETER_POWERFACTOR:
			fValue = m_para[m_bMachineID].fPowerFactor;
			break;
		case AMMETER_POWER_A:		//2013-4-17 
			fValue = m_para[m_bMachineID].fPowerA;
			break;
		case AMMETER_POWER_B:
			fValue = m_para[m_bMachineID].fPowerB;
			break;
		case AMMETER_POWER_C:
			fValue = m_para[m_bMachineID].fPowerC;
			break; 
		default:
			break;
	}
	return fValue;
}

void	CtmTaskN2CAmmeter::SaveTotalPower(void *pData, int nLen)
{
	double dPower = 0;
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	long lValue = 0;
	if(m_CTPara > 0)
		m_para[m_bMachineID].fEnergy = DATA2FLOAT(pData, nLen)*m_CTPara;
	else
		m_para[m_bMachineID].fEnergy = DATA2FLOAT(pData, nLen);
	
	lValue = (long)(m_para[m_bMachineID].fEnergy / 10000.0);  // mw  --> kw * 100(2 precision)
	//SetDBValue("AMMETER_HMI_REAL_POWER", lValue);  /*JOYCE2014-3-29 mark 7HT海天特殊情況處理,多個電力計時需區分統計*/
	
	dPower = (double)lValue;
	if (m_notifier)
		m_notifier(AMMETER_NOTIFY_TOTALPOWER, &dPower, sizeof(dPower), m_bMachineID);
	//printf("total power:%f, node = %d\n", m_para[m_bMachineID].fEnergy, m_AmmeterConf.AmmeterInfo[m_bMachineID].wNode);
}

void	CtmTaskN2CAmmeter::SaveEnergy(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fTotalPower = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fTotalPower = DATA2FLOAT(pData, nLen);
	
	//printf("energy:%f\n", m_para[m_bMachineID].fTotalPower);
}

void	CtmTaskN2CAmmeter::SaveVoltageA(void *pData, int nLen)
{
	//m_para[m_bMachineID].nVoltageA = BCD2Int(pData, nLen);
	m_para[m_bMachineID].fVoltageA = DATA2FLOAT(pData, nLen);
	//printf("voltage A:%f, %d\n", m_para[m_bMachineID].fVoltageA, m_para[m_bMachineID].fVoltageA);
}

void	CtmTaskN2CAmmeter::SaveVoltageB(void *pData, int nLen)
{
	//m_para[m_bMachineID].nVoltageB = BCD2Int(pData, nLen);
	m_para[m_bMachineID].fVoltageB = DATA2FLOAT(pData, nLen);
	//printf("voltage B:%f, %d\n", m_para[m_bMachineID].fVoltageB, m_para[m_bMachineID].fVoltageB);
}

void	CtmTaskN2CAmmeter::SaveVoltageC(void *pData, int nLen)
{
	//m_para[m_bMachineID].nVoltageC = BCD2Int(pData, nLen);
	m_para[m_bMachineID].fVoltageC = DATA2FLOAT(pData, nLen);
	//printf("voltage C:%f, %d\n", m_para[m_bMachineID].fVoltageC, m_para[m_bMachineID].fVoltageC);
}

void	CtmTaskN2CAmmeter::SaveCurrentA(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fCurrentA = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fCurrentA = DATA2FLOAT(pData, nLen);
	//printf("Current A:%f\n", m_para[m_bMachineID].fCurrentA);
}

void	CtmTaskN2CAmmeter::SaveCurrentB(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fCurrentB = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fCurrentB = DATA2FLOAT(pData, nLen);
	//printf("Current B:%f\n", m_para[m_bMachineID].fCurrentB);
}

void	CtmTaskN2CAmmeter::SaveCurrentC(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fCurrentC = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fCurrentC = DATA2FLOAT(pData, nLen);
	//printf("Current m_CTPara = %d, C:%f\n",m_CTPara,  m_para[m_bMachineID].fCurrentC);
}

void	CtmTaskN2CAmmeter::SaveVoltageAB(void *pData, int nLen)
{
	m_para[m_bMachineID].fVoltageAB = DATA2FLOAT(pData, nLen);
	//printf("Voltage AB:%f\n", m_para[m_bMachineID].fVoltageAB);
}

void	CtmTaskN2CAmmeter::SaveVoltageBC(void *pData, int nLen)
{
	m_para[m_bMachineID].fVoltageBC = DATA2FLOAT(pData, nLen);
	//printf("Voltage BC:%f\n", m_para[m_bMachineID].fVoltageBC);
}

void	CtmTaskN2CAmmeter::SaveVoltageAC(void *pData, int nLen)
{
	m_para[m_bMachineID].fVoltageAC = DATA2FLOAT(pData, nLen);
	//printf("Voltage AC:%f\n", m_para[m_bMachineID].fVoltageAC);
}

void	CtmTaskN2CAmmeter::SavePowerA(void *pData, int nLen)//2013-4-17 
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fPowerA = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fPowerA = DATA2FLOAT(pData, nLen);
//	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!SavePowerA:%f\n", m_para[m_bMachineID].fCurrentA);
}

void	CtmTaskN2CAmmeter::SavePowerB(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fPowerB = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fPowerB = DATA2FLOAT(pData, nLen);
	//printf("Current B:%f\n", m_para[m_bMachineID].fCurrentB);
}

void	CtmTaskN2CAmmeter::SavePowerC(void *pData, int nLen)
{
	m_CTPara = m_AmmeterConf.AmmeterInfo[m_bMachineID].wCTPara;
	
	if(m_CTPara > 0)
		m_para[m_bMachineID].fPowerC = DATA2FLOAT(pData, nLen) * m_CTPara;
	else
		m_para[m_bMachineID].fPowerC = DATA2FLOAT(pData, nLen);
	//printf("Current m_CTPara = %d, C:%f\n",m_CTPara,  m_para[m_bMachineID].fCurrentC);
}

void CtmTaskN2CAmmeter::SavePhaseDeviation(void *pData, int nLen)
{
	DWORD	dwTemp = 0;
	memcpy(&dwTemp, pData, sizeof(DWORD));
	m_para[m_bMachineID].dwPhaseDeviation = dwTemp;
	DearlErrorDeviation(m_bMachineID, dwTemp);
	//printf("Voltage AC:%d\n", m_para[m_cCurIndex].dwPhaseDeviation);
}

void CtmTaskN2CAmmeter::SavePowerFactor(void *pData, int nLen)
{
	m_para[m_bMachineID].fPowerFactor = DATA2FLOAT(pData, nLen)*100;
	//printf("Voltage AC:%f\n", m_para[m_cCurIndex].fPowerFactor);
}

void CtmTaskN2CAmmeter::SaveDataVersion(void *pData, int nLen)
{
	DWORD	dwTemp = 0;
	memcpy(&dwTemp, pData, sizeof(DWORD));
	m_para[m_bMachineID].dwDataVer = dwTemp;
	//printf("Voltage AC:%x\n", m_para[m_cCurIndex].dwDataVer);
}

double	CtmTaskN2CAmmeter::BCD2Double(void *pData, int nLen, int nPrecision)
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

int		CtmTaskN2CAmmeter::BCD2Int(void *pData, int nLen)
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

float	CtmTaskN2CAmmeter::DATA2FLOAT(void *pData, int nLen)
{
	float fTemp = 0;
	memcpy(&fTemp, pData, sizeof(float));
	return fTemp;	
}

void* CtmTaskN2CAmmeter::ReadConf()
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
	else	//2013-4-12 default 
	{
		if(g_pExTable->GetTableValue(0,"D_7HD"))
		   m_AmmeterConf.wCount =0;
		
		m_AmmeterConf.N2CVoltage[MAX_NUM][0] = 450;
		m_AmmeterConf.N2CVoltage[MAX_NUM][1] = 450;
		m_AmmeterConf.N2CVoltage[MAX_NUM][2] = 450;
		
		if(g_ptaskAmmeter != NULL)
			g_ptaskAmmeter->WriteConf(&m_AmmeterConf);
	}
	
	m_bReadConfFlag = TRUE;
	return &m_AmmeterConf;
}

int CtmTaskN2CAmmeter::WriteConf(void* pConf)
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
/*============================================================================+
|	function:	DearlErrorVoltage(int	nDataType, int nAmtIndex, void* pData)|
|	Effect	:	To handle voltage error function	 	   					  |
+-----------------------------------------------------------------------------+
|																			  |
|	parameter:	nDataType	A,B,C voltage index								  |
|				nAmtIndex	Ammeter	index									  |
|				pData		Every ammeter A,B,C voltage value				  |
|																			  |
|	return	:	NULL														  |
|	Author	:	SH	Renwei.Ying		2013.1.7								  |
+============================================================================*/
void	CtmTaskN2CAmmeter::DearlErrorVoltage(int nDataType, int nAmtIndex, void* pData)
{
	if(nAmtIndex < 0 || nAmtIndex > 2)	return;
	static	WORD	wErrorFlag[2][9] = {0,0,0,0,0,0,0,0,0};		//[2]:0 for too low error;1 for too high error
	static	DWORD	dwErrorAmt[2][9] = {0,0,0,0,0,0,0,0,0};
	static	BYTE	bOldAmtState  = 0;
	if(bOldAmtState != g_bAmmeterStatus)	//Clearly error when after the break
	{
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0;j < 9; j++)
				if(dwErrorAmt[i][j] != 0)
					g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[i][j]);
		}
		memset(wErrorFlag, 0, sizeof(wErrorFlag));
		memset(dwErrorAmt, 0, sizeof(dwErrorAmt));
		memset(m_para, 0, sizeof(m_para));
		bOldAmtState = g_bAmmeterStatus;
	}
	DWORD	dwAmtErr	= 0;
	float	fValue		= 0;
	WORD	wSegment 	= 0;
	memcpy((char*)&fValue, (char*)pData, sizeof(float));
	dwAmtErr |= ERROR_AMMETER_TM<<24;
	dwAmtErr |= nAmtIndex<<16;	
	dwAmtErr |= nDataType | 0x100;			//0x100:for Voltage Err!  nDataType(0:A 1:B 2:C(V))
	wSegment = nAmtIndex*3+nDataType;
	if(wSegment >= 9)	return ;
	
	if(m_AmmeterConf.N2CVoltage[MIN_NUM][wSegment]>fValue && wErrorFlag[MIN_NUM][wSegment]==0)
	{
	  if(bAmtVolA)
	     bAmtVolErrorA =TRUE;
	  if(bAmtVolB)
	     bAmtVolErrorB =TRUE;
	  if(bAmtVolC)
	     bAmtVolErrorC =TRUE;
	     
	  if((bAmtVolTimeOutA) || (bAmtVolTimeOutB) || (bAmtVolTimeOutC))
	  {
		wErrorFlag[MIN_NUM][wSegment] = 1;
		dwErrorAmt[MIN_NUM][wSegment] = dwAmtErr;			//Err for voltage too low(MIN>fValue)
		g_ptmControlServer->ErrorControl()->AddErrorLog(dwErrorAmt[MIN_NUM][wSegment]);
		if(dwErrorAmt[MAX_NUM][wSegment] != 0 && wErrorFlag[MAX_NUM][wSegment] ==1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MAX_NUM][wSegment]);
			dwErrorAmt[MAX_NUM][wSegment] = 0;
			wErrorFlag[MAX_NUM][wSegment] = 0;
		}
	  }
	}
	else if(m_AmmeterConf.N2CVoltage[MAX_NUM][wSegment]<fValue && wErrorFlag[MAX_NUM][wSegment]==0)
	{	
	  if(bAmtVolA)
	     bAmtVolErrorA =TRUE;
	  if(bAmtVolB)
	     bAmtVolErrorB =TRUE;
	  if(bAmtVolC)
	     bAmtVolErrorC =TRUE;
	  if((bAmtVolTimeOutA) || (bAmtVolTimeOutB) || (bAmtVolTimeOutC))
	  {
		wErrorFlag[MAX_NUM][wSegment] = 1;
		dwErrorAmt[MAX_NUM][wSegment] = dwAmtErr+4;			//Err for voltage too high(MAX<fValue)
		g_ptmControlServer->ErrorControl()->AddErrorLog(dwErrorAmt[MAX_NUM][wSegment]);
		if(dwErrorAmt[MIN_NUM][wSegment] != 0 && wErrorFlag[MIN_NUM][wSegment] == 1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MIN_NUM][wSegment]);
			dwErrorAmt[MIN_NUM][wSegment] = 0;
			wErrorFlag[MIN_NUM][wSegment] = 0;
		}
	  }
    }
	else if((m_AmmeterConf.N2CVoltage[MIN_NUM][wSegment]<=fValue && m_AmmeterConf.N2CVoltage[MAX_NUM][wSegment]>=fValue) && 
		(wErrorFlag[MIN_NUM][wSegment] == 1 || wErrorFlag[MAX_NUM][wSegment] == 1))
	{	
		    if(bAmtVolA)
	     		bAmtVolErrorA =FALSE;
	 		if(bAmtVolB)
	    		bAmtVolErrorB =FALSE;
	  		if(bAmtVolC)
	    	    bAmtVolErrorC =FALSE;
		if(dwErrorAmt[MIN_NUM][wSegment] != 0 && wErrorFlag[MIN_NUM][wSegment] == 1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MIN_NUM][wSegment]);
			dwErrorAmt[MIN_NUM][wSegment] = 0;
			wErrorFlag[MIN_NUM][wSegment] = 0;
		}
		if(dwErrorAmt[MAX_NUM][wSegment] != 0 && wErrorFlag[MAX_NUM][wSegment] ==1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MAX_NUM][wSegment]);
			dwErrorAmt[MAX_NUM][wSegment] = 0;
			wErrorFlag[MAX_NUM][wSegment] = 0;
		}
	}
	else if((m_AmmeterConf.N2CVoltage[MIN_NUM][wSegment]<=fValue && m_AmmeterConf.N2CVoltage[MAX_NUM][wSegment]>=fValue) && 
		(wErrorFlag[MIN_NUM][wSegment] == 0 && wErrorFlag[MAX_NUM][wSegment] == 0))
		{
			if(bAmtVolA)
	     		bAmtVolErrorA =FALSE;
	 		if(bAmtVolB)
	    		bAmtVolErrorB =FALSE;
	  		if(bAmtVolC)
	    	    bAmtVolErrorC =FALSE;
		}
}

/*void	CtmTaskN2CAmmeter::DearlErrorPower(int nDataType, int nAmtIndex, void* pData)
{
	if(nAmtIndex < 0 || nAmtIndex > 2)	return;
	static	WORD	wErrorFlag[2][9] = {0,0,0,0,0,0,0,0,0};		//[2]:0 for too low error;1 for too high error
	static	DWORD	dwErrorAmt[2][9] = {0,0,0,0,0,0,0,0,0};
	static	BYTE	bOldAmtState  = 0;
	if(bOldAmtState != g_bAmmeterStatus)	//Clearly error when after the break
	{
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0;j < 9; j++)
				if(dwErrorAmt[i][j] != 0)
					g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[i][j]);
		}
		memset(wErrorFlag, 0, sizeof(wErrorFlag));
		memset(dwErrorAmt, 0, sizeof(dwErrorAmt));
		memset(m_para, 0, sizeof(m_para));
		bOldAmtState = g_bAmmeterStatus;
	}
	DWORD	dwAmtErr	= 0;
	float	fValue		= 0;
	WORD	wSegment 	= 0;
	memcpy((char*)&fValue, (char*)pData, sizeof(float));
	dwAmtErr |= ERROR_AMMETER_TM<<24;
	dwAmtErr |= nAmtIndex<<16;	
	dwAmtErr |= nDataType | 0x100;			//0x100:for Voltage Err!  nDataType(0:A 1:B 2:C(V))
	wSegment = nAmtIndex*3+nDataType;
	if(wSegment >= 9)	return ;
	
	if(m_AmmeterConf.N2CVoltage[MIN_NUM][wSegment]>fValue && wErrorFlag[MIN_NUM][wSegment]==0)
	{
		wErrorFlag[MIN_NUM][wSegment] = 1;
		dwErrorAmt[MIN_NUM][wSegment] = dwAmtErr;			//Err for voltage too low(MIN>fValue)
		g_ptmControlServer->ErrorControl()->AddErrorLog(dwErrorAmt[MIN_NUM][wSegment]);
		if(dwErrorAmt[MAX_NUM][wSegment] != 0 && wErrorFlag[MAX_NUM][wSegment] ==1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MAX_NUM][wSegment]);
			dwErrorAmt[MAX_NUM][wSegment] = 0;
			wErrorFlag[MAX_NUM][wSegment] = 0;
		}
	}
	else if(m_AmmeterConf.N2CVoltage[MAX_NUM][wSegment]<fValue && wErrorFlag[MAX_NUM][wSegment]==0)
	{
		wErrorFlag[MAX_NUM][wSegment] = 1;
		dwErrorAmt[MAX_NUM][wSegment] = dwAmtErr+4;			//Err for voltage too high(MAX<fValue)
		g_ptmControlServer->ErrorControl()->AddErrorLog(dwErrorAmt[MAX_NUM][wSegment]);
		if(dwErrorAmt[MIN_NUM][wSegment] != 0 && wErrorFlag[MIN_NUM][wSegment] == 1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MIN_NUM][wSegment]);
			dwErrorAmt[MIN_NUM][wSegment] = 0;
			wErrorFlag[MIN_NUM][wSegment] = 0;
		}
	}
	else if((m_AmmeterConf.N2CVoltage[MIN_NUM][wSegment]<=fValue && m_AmmeterConf.N2CVoltage[MAX_NUM][wSegment]>=fValue) && 
		(wErrorFlag[MIN_NUM][wSegment] == 1 || wErrorFlag[MAX_NUM][wSegment] == 1))
	{	
		if(dwErrorAmt[MIN_NUM][wSegment] != 0 && wErrorFlag[MIN_NUM][wSegment] == 1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MIN_NUM][wSegment]);
			dwErrorAmt[MIN_NUM][wSegment] = 0;
			wErrorFlag[MIN_NUM][wSegment] = 0;
		}
		if(dwErrorAmt[MAX_NUM][wSegment] != 0 && wErrorFlag[MAX_NUM][wSegment] ==1)
		{
			g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[MAX_NUM][wSegment]);
			dwErrorAmt[MAX_NUM][wSegment] = 0;
			wErrorFlag[MAX_NUM][wSegment] = 0;
		}
	}
}*/

void	CtmTaskN2CAmmeter::DearlErrorDeviation(int  nAmtIndex, int nValue)		//Rw.Ying
{
	static	WORD	wErrorFlag[AMMETER_N2CNUM] = {0,0,0};
	static	DWORD	dwErrorAmt[AMMETER_N2CNUM] = {0,0,0};
	static	BYTE	bOldAmtState  = 0;
	WORD	wValue = 0;
	//printf("bOldAmtState:%d, %d, g_bAmmeterStatus:%d\n", bOldAmtState, bAmmeterStatusOld, g_bAmmeterStatus);
	if((bOldAmtState != g_bAmmeterStatus)||(bAmmeterStatusOld != g_bAmmeterStatus))	//Clearly error when after the break
	{
		for(int i = 0; i < AMMETER_N2CNUM; i++)
		{
			if(dwErrorAmt[i] != 0)
				g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[i]);
		}
		memset(wErrorFlag, 0, sizeof(wErrorFlag));
		memset(dwErrorAmt, 0, sizeof(dwErrorAmt));
		bOldAmtState = bAmmeterStatusOld = g_bAmmeterStatus;
	}
	
	DWORD	dwAmtErr	= 0;
	dwAmtErr |= ERROR_AMMETER_TM<<24;
	dwAmtErr |= nAmtIndex<<16;
	dwAmtErr |= 0x100 | 3;		//0x100:for Voltage Err!  0:A 1:B 2:C(V) 3 is for Deviation Err
//	printf("nValue:%d, dwAmtErr:%x, %d, %d\n", nValue, dwAmtErr, nAmtIndex, wErrorFlag[nAmtIndex]);
	
/*JOYCE2014-3-31 7HT海天默認使用相位偏差功能:檢查三相接線是否OK*/	
//	if(GetDBValue("SYS_HMI_NULL_NULL_RESERVED25").lValue == 1)		//2013-5-8 
//	{
		if(nValue == 1 && wErrorFlag[nAmtIndex] == 0)
		{
			dwErrorAmt[nAmtIndex] = dwAmtErr;
			wErrorFlag[nAmtIndex] = 1;
			g_ptmControlServer->ErrorControl()->AddErrorLog(dwErrorAmt[nAmtIndex]);
			if(g_ptaskdsp != NULL)
				g_ptaskdsp->WriteValue(COMM_SENDSTATUS, COMM_S_MOTORPHASEERR);	//2013-5-8 
		}
		else if(nValue == 0 && wErrorFlag[nAmtIndex] == 1)
		{
			  wErrorFlag[nAmtIndex] = 0;
			  g_ptmControlServer->ErrorControl()->FixErrorLog(dwErrorAmt[nAmtIndex]);
			wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
		    wValue &= ~COMM_S_MOTORPHASEERR;
		    SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8 
			
			dwErrorAmt[nAmtIndex] = 0;
		}
		else if(nValue == 0 && wErrorFlag[nAmtIndex] == 0)
		{
			wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
		    wValue &= ~COMM_S_MOTORPHASEERR;
		    SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8 
		}
//	}
//	else
//	{
//		if(nValue == 1 && wErrorFlag[nAmtIndex] == 0)
//		{
//			wErrorFlag[nAmtIndex] = 1;
//			
//			wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
//			wValue &= ~COMM_S_MOTORPHASEERR;
//			SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8  
//		}
//		else if(nValue == 0 && wErrorFlag[nAmtIndex] == 1)
//		{
//			wErrorFlag[nAmtIndex] = 0;
//			
//			wValue	= GetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1").lValue;
//		    wValue &= ~COMM_S_MOTORPHASEERR;
//		    SetDBValue("MACH_OTHR_OTHR_NULL_WERRORSTATUS1", wValue);    //2013-5-8 
//		}
//	}
}

#ifndef	D_BEOBJECT_CTMTASKN2CAMMETER
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	//printf("Name=%s \n", Name);
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
