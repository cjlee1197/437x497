/*==========================================================================+
|  Class    : KEB Motor communication          								|
|  Task     : KEB Motor communication file                     				|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : zholy		                                                    |
|  Version  : V1.00                                                         |
|  Creation : 2005/12/17                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#include	"main.h"
#include	"utils.h"
#include	"taskkeb.h"
#include	"curvedata.h"

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		TIME_KEBRUN			1000
#define		TIME_SENDQUEUE		20

const	SETDB		m_SetDB[]		=	
{
	{{'0','1'}, -1, -1}, {{'0','2'}, -1, -1}, {{'0','4'}, -1, -1},
	{{'0','8'}, -1, -1}, {{'1','0'},  1,  6}, {{'2','0'}, -1, -1},
	{{'4','0'}, 14, 15}, {{'8','0'}, -1, -1}, {{'F','F'}, -1, -1}
};
const	READLIST	m_ReadList[]	=
{
	{ADDR_CURPRES, IID_PRES},
	{ADDR_CURSPED, IID_SPD }
};
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
long			m_lTimeKEBRun		= TIME_KEBRUN;

BYTE			g_RxKEBBuffer[KEBBUFF_LENGTH];
BYTE			g_TxKEBBuffer[KEBBUFF_LENGTH];


ETHERNET		CTaskKeb::m_ethernet	=
{
	192 | 168 << 8 | 0 << 16 | 200 << 24	,		//LEO20070627
	192 | 168 << 8 | 0 << 16 | 201 << 24	,		// ip addr
	8000			,								// src port
	8000			,								// dst port
	AF_INET			,								// ipv4
	SOCK_DGRAM		,								// TCP SOCK_STREAM
	KEBBUFF_LENGTH	,
	g_RxKEBBuffer	,
	KEBBUFF_LENGTH	,
	g_TxKEBBuffer	,
	NULL
};

int				CTaskKeb::m_nValueID	= 	-1;
int				CTaskKeb::m_nReadOrder	= 	 0;
long			CTaskKeb::m_lStepRank	= 	 1;
BOOL			CTaskKeb::m_bNeedEOT	= TRUE;
BOOL			CTaskKeb::m_bOneOffSend	= TRUE;
BOOL			CTaskKeb::m_bKebOnline	= FALSE;
WORD			CTaskKeb::m_wLastKEBOP  = KEBOP_NULL;
pCURVEFunc		CTaskKeb::m_pCurveFunc	= NULL;
BYTE			CTaskKeb::m_cInvokID	= 0x31; 	// '1'
KEBQUEUE		CTaskKeb::m_KebQueue	= {0, 0, 0};
WRITEECHO		CTaskKeb::m_WriteEcho	= {0, FALSE};
TKEBW_S01PARA	m_ItemList[KEB_QUEUE_COUNT];
REALTIMEDATA	g_RealData;

//---------------
//	test
//---------------
long			m_lTimeSend			=    0;
long			m_lTestTime			=   -1;
long			m_lTest1			=    0;
long			m_lTest2			=    0;

long	long	g_lKEBPulse;
long	long	g_lKEBUM;
long	long	g_lKEBRPM;
long	long	g_lKEBMMPerSec;

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

       
/*===========================================================================+
|            Class implementation - TaskComm                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|            Constructor and destructor                                      |
+---------------------------------------------------------------------------*/
CTaskKeb::CTaskKeb() : CTaskComm()
{
	if (m_nValueID == -1)
	{
		m_ethernet.NetFunc	= (NETFUNC)AnalysisKEBData;
		m_ethernet.wdstPort	= 8000;
		m_nValueID			= AddDevice("CEthernet", &m_ethernet);
		//printf("AddKEBDevice: %d\n", m_nValueID);
	}
	
	RegisterMessage(MSG_KEB, m_idMe);
	AddCounter(&m_lTimeKEBRun, m_idMe);
	AddCounter(&m_lTestTime, m_idMe);
	AddRoutine(TIME_SENDQUEUE, (SENDFUNC)SendKEBQueue);
}

CTaskKeb::~CTaskKeb()
{
	if (m_nValueID != -1)	DelDevice(m_nValueID);
	
	DeleteRoutine();
	DeleteCounter(&m_lTimeKEBRun, m_idMe);
	DeleteCounter(&m_lTestTime, m_idMe);
	UnRegisterAllMessage(MSG_KEB);
}

void	CTaskKeb::Run()
{
	if (m_lTimeKEBRun <= 0)
	{
		SetKEBOnlineState(m_bKebOnline);
		m_bKebOnline	= FALSE;
		m_lTimeKEBRun	= TIME_KEBRUN;
	}
	
	if ((g_wiNetStatus) && (m_bOneOffSend))
	{
		m_bOneOffSend	= FALSE;
		Send_OneOffSendData();
	}
	
	if (g_wiNetStatus == 0)
	{
		m_bOneOffSend	= TRUE;
		m_bNeedEOT		= TRUE;
		if (m_pCurveFunc != NULL) m_pCurveFunc(KEBCURVE_OFF);
	}
}

//----------------------------------------------
//----------------------------------------------
BYTE    CTaskKeb::ExclusiveOR(BYTE *pData, int nLength)
{
    int     i       = 1;
    BYTE    cBCC    = 0;

    while(nLength >= i++)	cBCC 	^= (*pData++);

    if (cBCC <= 0x20)		cBCC	|= 0x20;
    return cBCC;
}

BOOL    CTaskKeb::CheckBCC(BYTE *pData, int nLength)
{
    BYTE    cBCC;

    cBCC	= ExclusiveOR(pData, nLength);

    if(*(pData+nLength) == cBCC) return TRUE;
    else return FALSE;
}

int		CTaskKeb::AddItem(TKEBW_S01PARA *pItem)
{
	KEBQUEUE		*pQueue		= &m_KebQueue;
	
	if ((pQueue->nIn == pQueue->nOut) && (pQueue->nFull == 1))	return -1;
	
	m_ItemList[pQueue->nIn++]	= *pItem;
	
	if (pQueue->nIn >= KEB_QUEUE_COUNT)	pQueue->nIn		= 0;
		
	if (pQueue->nIn	== pQueue->nOut)	pQueue->nFull	= 1;
	
	return pQueue->nIn;
}

int		CTaskKeb::DelItem(BYTE cIID)
{
	KEBQUEUE		*pQueue		= &m_KebQueue;
	
	if ((pQueue->nIn == pQueue->nOut) && (pQueue->nFull == 0))	return -1;
	if (m_ItemList[pQueue->nOut].cIID != cIID) 	return -1;
	
	pQueue->nOut++;
	
	if (pQueue->nOut >= KEB_QUEUE_COUNT)	pQueue->nOut	= 0;
		
	if (pQueue->nIn == pQueue->nOut)	pQueue->nFull		= 0;
	
	return pQueue->nOut;
}

int		CTaskKeb::ReadItem()
{
	KEBQUEUE		*pQueue		= &m_KebQueue;
	
	if ((pQueue->nIn == pQueue->nOut) && (pQueue->nFull == 0))	return -1;
	
	return	pQueue->nOut;
}

int		CTaskKeb::SendKEBQueue()
{
			int		nIndex;
	static	int		m_nReadIndex;
	//SetKEBOnlineState(m_bKebOnline);
	//m_bKebOnline	= FALSE;
	
	m_lTest1++;//test
	
	if (m_WriteEcho.bEcho)
	{
		DelItem(m_WriteEcho.cIID);
		m_WriteEcho.bEcho	= FALSE;
		m_WriteEcho.cIID	= 0;
	}
	
	//===========
	//  Send WriteValue
	//===========
	nIndex	= ReadItem();
	if (nIndex >= 0)
	{
		m_WriteEcho.cIID	= m_ItemList[nIndex].cIID;
		m_WriteEcho.bEcho	= FALSE;
		SendKEBDATA((BYTE *)&m_ItemList[nIndex], SIZEOF_TKEBW_S01PARA);//sizeof(TKEBW_S01PARA) zholy061229 snap to CPU
		return	1;
	}
	
	//===========
	//  Send ReadScopeData
	//===========
	if (m_nReadOrder == 0)
	{
		Read_KebProcessData(KEB_RW_SCOPE, IID_SCOPE);
//		m_nReadOrder	= 1;
		return 1;
	}
	
	//===========
	//  Send ReadValue-Others
	//===========
	if (m_nReadOrder)
	{
		Read_KebValue(m_ReadList[m_nReadIndex].wAddr, m_ReadList[m_nReadIndex].cIID, SET0_INIT);
		if (++m_nReadIndex == 2) m_nReadIndex = 0;//后续增加读其他资料时要将2换为MaxReadList
		m_nReadOrder	= 0;
	}
	return 1;
}

BOOL    CTaskKeb::Read_KebValue(WORD wAddr)
{
    int         nLength;
    char        csz[10];
    BYTE        cSendBuff[KEBDATA_LENGTH];
    
    TKEBR_PARA  *pSendBuff  = (TKEBR_PARA *)cSendBuff;
    nLength                 = sizeof(TKEBR_PARA);
    pSendBuff->cENQ         = CTRLCHAR_ENQ;
    sprintf(csz, "%04X", wAddr);
    strncpy(pSendBuff->cIndex, csz, KEB_LEN_CMD);
    
    SendKEBDATA(cSendBuff, nLength);
    
    return TRUE;
}

BOOL	CTaskKeb::Read_KebValue(WORD wAddr, BYTE cIID, WORD wSet)
{
	int                 nLength;
    char                csz[10];
    BYTE                cSendBuff[KEBDATA_LENGTH];
    
    TKEBR_MASKDATA		*pReadValue = (TKEBR_MASKDATA *)cSendBuff;
    
    nLength                         = SIZEOF_TKEBR_MASKDATA;//sizeof(TKEBR_MASKDATA) zholy061229 snap to CPU
    pReadValue->cSID                = KEB_RW_PARA;
    pReadValue->cIID                = cIID;
    sprintf(csz, "%04X", wAddr);
    strncpy(pReadValue->cIndex, csz, KEB_LEN_CMD);
	strncpy(pReadValue->cMask, m_SetDB[wSet].bSetASCII, KEB_LEN_MASK);

    pReadValue->cENQ    			= CTRLCHAR_ENQ;
    pReadValue->cBCC    			= ExclusiveOR((BYTE *)pReadValue, nLength-1);

    SendKEBDATA(cSendBuff, nLength);

    return TRUE;
}

BOOL	CTaskKeb::Read_KebProcessData(BYTE cSID, BYTE cIID)
{
	int         nLength;
    BYTE        cSendBuff[KEBDATA_LENGTH];
    
    TKEBR_PD	*pReadValue = (TKEBR_PD *)cSendBuff;
    
    nLength                 = sizeof(TKEBR_PD);
    pReadValue->cSID        = cSID;
    pReadValue->cIID        = cIID;
    pReadValue->cENQ    	= CTRLCHAR_ENQ;
    pReadValue->cBCC    	= ExclusiveOR((BYTE *)pReadValue, nLength-1);

    SendKEBDATA(cSendBuff, nLength);

    return TRUE;
}

BOOL    CTaskKeb::Write_KebValue(WORD wAddr, DWORD dwData, WORD wSet)
{
    char            csz[10];
    int             nLength;
    BYTE            cSendBuff[KEBDATA_LENGTH];
    
    TKEBW_S01PARA   *pWriteValue	= (TKEBW_S01PARA *)cSendBuff;
	nLength             			= SIZEOF_TKEBW_S01PARA;//sizeof(TKEBW_S01PARA) snap to CPU
		
    pWriteValue->cSTX				= CTRLCHAR_STX;
    pWriteValue->cSID				= KEB_RW_PARA;
    pWriteValue->cIID				= m_cInvokID;
    sprintf(csz, "%04X", wAddr);
    strncpy(pWriteValue->cIndex, csz, KEB_LEN_CMD);
	sprintf(csz, "%08lX", dwData);
	strncpy(pWriteValue->cData, csz, KEB_LEN_DWORD);
	pWriteValue->cETX				= CTRLCHAR_ETX;
	strncpy(pWriteValue->cMask, m_SetDB[wSet].bSetASCII, KEB_LEN_MASK);
    pWriteValue->cBCC    			= ExclusiveOR((BYTE *)&pWriteValue->cSID, nLength-2);
	
	AddItem(pWriteValue);
	
	if (++m_cInvokID >= 0x3B) m_cInvokID	= 0x31;//目前IID写服务占用0x31~0x3A共10个ID,预留0x41~0x46给读服务.
	
    return TRUE;
}

void	CTaskKeb::SetKEBOnlineState(BOOL bOnline)
{
	if (bOnline)
	{
		g_wiNetStatus	= 1;//g_wiNetStatus
		//printf("Online!\n");
	}
	else
	{
		g_wiNetStatus	= 0;//g_wiNetStatus
		//printf("Offline!\n");
	}
}

void	CTaskKeb::Send_OneOffSendData()
{
	short	i;
	long	lStepRank;
	
	Write_KebValue(ADDR_CONTROLCMD, KEBCMD_STOP, SET0_INIT);
	
	lStepRank	= GetDBValue("MCONF_SCREW_INJECT_NULL_RANK").lValue;
	m_lStepRank	= lStepRank;
	
	for(i = 0; i < lStepRank - 1; i++)
	{
		Write_KebValue(ADDR_SELTINDEX,  (DWORD)(i+1), SET0_INIT);
    	Write_KebValue(ADDR_INDEXMODE, 	           1, SET0_INIT);
	}
	Write_KebValue(ADDR_SELTINDEX,  (DWORD)lStepRank, SET0_INIT);
    Write_KebValue(ADDR_INDEXMODE, 	               0, SET0_INIT);
	
	Write_KebValue(ADDR_SCOPE1, ADDR_CURPOSI, SET0_INIT);
	Write_KebValue(ADDR_SCOPE1 + 1, SET_ALL, SET0_INIT);
	Write_KebValue(ADDR_SCOPE2, ADDR_CURSPED, SET0_INIT);
	Write_KebValue(ADDR_SCOPE2 + 1, SET_ALL, SET0_INIT);
	Write_KebValue(ADDR_SCOPE3, ADDR_CURPRES, SET0_INIT);
	Write_KebValue(ADDR_SCOPE3 + 1, SET_ALL, SET0_INIT);
	Write_KebValue(ADDR_SCOPE4, ADDR_INVENTERSTAT, SET0_INIT);
	Write_KebValue(ADDR_SCOPE4 + 1, SET_ALL, SET0_INIT);//setscope
}

BOOL    CTaskKeb::SendKEBDATA(BYTE *pData, int nDataLen)
{
    char    csz[10];
    BYTE    cSendBuff[KEBPACKET_LENGTH];
    int     nLength     =   0;

    //=======
    // fill EOT
    //=======
    if (m_bNeedEOT)
    {
        cSendBuff[0]		=  CTRLCHAR_EOT;
        sprintf(csz, "%02X", KEB_INVERTER_ADDR);
        strncpy((char *)&cSendBuff[1], csz, KEB_LEN_ADDR);
        nLength			   +=  (KEB_LEN_ADDR+1);
        m_bNeedEOT			=  FALSE;
    }
    memcpy(cSendBuff + nLength, pData, nDataLen);
    nLength     		   += nDataLen;

    SendData(m_nValueID, (char *)cSendBuff, nLength);
    
    return TRUE;
}

void	CTaskKeb::SetCurve(long lCurCount)
{
	static long	lCount;//test
	
	static long	lLastCnt, lSumCnt;
	
	if (g_RealData.lPosi  < 0)	g_RealData.lPosi	*= -1;
	if (g_RealData.lSpeed < 0)	g_RealData.lSpeed	*= -1;
	
	//James 既mark
	//if (m_lTestTime > 0)
	//{
	//	if ((lCount <= 500) && (lCount >= 1))
	//	{
	//		if (lCurCount > lLastCnt)	lSumCnt	+= lCurCount - lLastCnt;
	//		else	lSumCnt	+= 65535 - lLastCnt + lCurCount;
	//		lLastCnt	= lCurCount;
	//		
	//		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALSPEED, (DWORD *)&g_RealData.lPosi, (DWORD *)&g_RealData.lSpeed, 1);
	//		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALPRESS, (DWORD *)&g_RealData.lPosi, (DWORD *)&g_RealData.lPress, 1);
	//		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, (DWORD *)&lSumCnt, (DWORD *)&g_RealData.lPress, 1);
	//		CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARRG_REALPRESS, (DWORD *)&g_RealData.lPosi, (DWORD *)&g_RealData.lPress, 1);
	//		lCount++;
	//		//printf("%ld,%ld,%ld,%ld,%ld\n" ,g_RealData.lPosi,g_RealData.lPress,g_RealData.lSpeed, lSumCnt, lCount);
	//	}
	//	else if (lCount > 500)
	//	{
	//		lCount	= -1;
	//		CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALSPEED);
	//		CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALPRESS);
	//		CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALPRESS);
	//		CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARRG_REALPRESS);
	//		//printf("curve get finished ,please wait!\n");
	//	}
	//	
	//}
	//else
	//{
	//	lCount		= 1;
	//	lLastCnt	= lCurCount;
	//	lSumCnt		= 0;
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALSPEED, (DWORD *)&g_RealData.lPosi, (DWORD *)&g_RealData.lSpeed, 1);
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALPRESS, (DWORD *)&g_RealData.lPosi, (DWORD *)&g_RealData.lPress, 1);
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, (DWORD *)&lSumCnt, (DWORD *)&g_RealData.lPress, 1);
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARRG_REALPRESS, (DWORD *)&g_RealData.lPosi, (DWORD *)&g_RealData.lPress, 1);
	//	m_lTestTime	= 20000;
	//}
}

void	CTaskKeb::GetScopeData(char *pdata, int nlength)
{
	long    				lData;
    char    				csz[20];
    char    				*pOffset;
    FKEB_SCOPE				*pScopeData	= (FKEB_SCOPE	*)pdata;
    static BOOL				m_bSendPosiDSP = FALSE;
		
	if (!m_bSendPosiDSP)
	{
		m_bSendPosiDSP	= TRUE;
		SetDBValue("MACH_ECDR1_WRITE_CHL1_ZEROOFFSET", (WORD)(lData>>16));
		SetDBValue("MACH_ECDR1_WRITE_CHL2_ZEROOFFSET", (WORD)(lData & 0x0000FFFF));
	}
	
	memset(csz,0,20);
	strncpy(csz, pScopeData->cData2, KEB_LEN_DWORD);
	lData = (long)strtoul(csz, &pOffset, 16);
	if (g_lKEBRPM != 0)
		{			
		//printf("lData = %ld\n", lData);//test
		g_RealData.lSpeed = (long)(lData * g_lKEBMMPerSec);
		g_RealData.lSpeed = (long)(g_RealData.lSpeed / (long)g_lKEBRPM);
		//printf("g_RealData.lSpeed = %ld, g_lKEBMMPerSec = %lld, g_lKEBRPM = %lld\n", g_RealData.lSpeed, g_lKEBMMPerSec, g_lKEBRPM);
		}
	else
		g_RealData.lSpeed	= 0;
	
    memset(csz,0,20);
	strncpy(csz, pScopeData->cData3, KEB_LEN_WORD);
	g_RealData.lPress	= (long)strtoul(csz, &pOffset, 16);
	
	memset(csz,0,20);
	strncpy(csz, pScopeData->cData4, KEB_LEN_WORD);
	g_RealData.lStat	= (long)strtoul(csz, &pOffset, 16);
	
	memset(csz,0,20);
	strncpy(csz, pScopeData->cTimeStamp, 4);
	g_RealData.lTimerCnt	= (long)strtoul(csz, &pOffset, 16);
	
	if (m_pCurveFunc != NULL) m_pCurveFunc(KEBCURVE_OPING);
	
	memset(csz,0,20);
	strncpy(csz, pScopeData->cData1, KEB_LEN_DWORD);
	lData				= (long)strtoul(csz, &pOffset, 16);
	if (g_lKEBPulse != 0)
		{
		g_RealData.lPosi = (long)(lData * g_lKEBUM);
		g_RealData.lPosi = (long)(g_RealData.lPosi / g_lKEBPulse);
		g_RealData.lPosi = (long)(g_RealData.lPosi / 10);
		}
	else
		g_RealData.lPosi = 0;

}

void    CTaskKeb::AnalysisKEBData(char *pdata,int nlength)
{
    long    lData;
    char    csz[20];
    char    *pOffset;
    
    m_lTest2++;//test

    //printf("%ld, %ld\n", m_lTest1, m_lTest2);
    
    m_bKebOnline	= TRUE;
    
    if ((BYTE)pdata[nlength-1] == CTRLCHAR_EOT) m_bNeedEOT = TRUE;

    if (nlength <= 3)
    {
        switch(nlength)
        {
            case 1:	//ANSWINQUIRY
                	//if ((BYTE)pdata[0] == CTRLCHAR_ACK)  OK;
                	//else	ERROR;
                break;
            case 2://ERROR,IIDACK
            {
                FKEB_IIDACK *pEcho	= (FKEB_IIDACK *)pdata;
                if (pEcho->cIID == m_WriteEcho.cIID)
                {
                	if (pEcho->cACK == CTRLCHAR_ACK)
                		m_WriteEcho.bEcho	= TRUE;
                }
                break;
            }
            case 3:	//SERVERROR
                	//if ((BYTE)pdata[2] == CTRLCHAR_EOT ||(BYTE)pdata[2] == CTRLCHAR_NAK) ERROR;
                	//else	OK;
                break;
            default :
                break;
        }
    }
    else
    {
        if (!CheckBCC((BYTE*)&pdata[1], nlength-2)) return;	//check BCC
        
        switch(nlength)
        {
            case 11://ONEPARA
					//case 0x0215:
                	//if (wData > 0)
					//Write_KebValue(ADDR_CONTROLCMD, KEBCMD_STOP);
                break;
            case 13://SONEDATA
            {
                FKEB_SONEDATA	*pEcho	= (FKEB_SONEDATA	*)pdata;
                memset(csz,0,20);
                strncpy(csz, pEcho->cData, KEB_LEN_DWORD);
                lData	= (long)strtoul(csz, &pOffset, 16);
                switch(pEcho->cIID)
                {
                	case IID_POS:
                		g_RealData.lPosi     = lData;
                		//printf("%ld\n", g_RealData.lPosi);
                		break;
                	case IID_PRES:
                		g_RealData.lPress    = lData;
                		break;
                	case IID_SPD:
                		g_RealData.lSpeed    = lData;
                		break;
                	default :
                		break;
                }
                break;
			}
            case 21://STWODATA
                break;
			case 33://SPD_SCOPE
			{
				FKEB_SCOPE	*pEcho	= (FKEB_SCOPE	*)pdata;
				if (pEcho->cSID == KEB_RW_SCOPE) GetScopeData(pdata, nlength);
				break;
			}
            default :
                break;
        }
    }
}

void	CTaskKeb::ErrorReset()
{
	Write_KebValue(ADDR_SETERROR, 1, SET0_INIT);
	Write_KebValue(ADDR_SETERROR, 0, SET0_INIT);
}

void	CTaskKeb::ResetPosition()
{
	Write_KebValue(ADDR_RESETPOSI, 1, SET0_INIT);
	Write_KebValue(ADDR_RESETPOSI, 0, SET0_INIT);
}

void	CTaskKeb::SendInjectStartIndex()
{	
	short	i, wStartIndex, wStepRank;
	char	cStrID[60], cStrRank[10];
	long	lRelative, lRankEnd;
	
	wStepRank	= (short)GetDBValue("MCONF_SCREW_INJECT_NULL_RANK").lValue;
	lRelative	= GetDBValue("METER_AD1_READ_CHL1_RELATIVEPOSITION").lValue;
	
	for(i = 0; i < wStepRank; i++)
	{
		cStrID[0]	= '\0';
		cStrRank[0]	= '\0';
		sprintf(cStrRank, "%d", i+1);
		strcat(cStrID, "MSET_SCRW_INJ_STEP");
		strcat(cStrID, cStrRank);
		strcat(cStrID, "_POSITION");
		lRankEnd	= GetDBValue(cStrID).lValue;
		if (lRelative > lRankEnd)
		{
			wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart + i;
			break;
		}
	}
	
	if (i == wStepRank) wStartIndex	= 0;
	
	Write_KebValue(ADDR_STRTINDEX , (DWORD)wStartIndex, SET3_INJECTSPD);
	
	//	short	wStartIndex;
	//if	  (dbmetr.adPosi_Relative[0] > dbmset.adPosi_Inject1End)	wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart;
    //else if (dbmetr.adPosi_Relative[0] > dbmset.adPosi_Inject2End)	wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart + 1;
    //else if (dbmetr.adPosi_Relative[0] > dbmset.adPosi_Inject3End)	wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart + 2;
    //else if (dbmetr.adPosi_Relative[0] > dbmset.adPosi_Inject4End)	wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart + 3;
    //else if (dbmetr.adPosi_Relative[0] > dbmset.adPosi_Inject5End)	wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart + 4;
    //else if (dbmetr.adPosi_Relative[0] > dbmset.adPosi_Inject6End)	wStartIndex = m_SetDB[SET3_INJECTSPD].wIndexStart + 5;
    //else wStartIndex = 0;
    //
    //Write_KebValue(ADDR_STRTINDEX , (DWORD)wStartIndex, SET3_INJECTSPD);
}

void    CTaskKeb::SendPOS_Index(BYTE cIndex, DWORD dwData)
{
	DWORD	dwPulse;
	
	if (g_lKEBUM != 0)
		{
		dwPulse	= (DWORD)(dwData * 10 * g_lKEBPulse);
		dwPulse	= (DWORD)(dwPulse / (DWORD)g_lKEBUM);
		}
	else
		dwPulse	= 0;
    Write_KebValue(ADDR_SELTINDEX, (DWORD)cIndex, SET0_INIT);
    Write_KebValue(ADDR_INDEXPOSI, 		 dwPulse, SET0_INIT);
}

void    CTaskKeb::SendSPD_Index(BYTE cIndex, DWORD dwData)
{
	DWORD dwRPM;
	
	if (g_lKEBMMPerSec != 0)
		{
		dwRPM	= (DWORD)(dwData * g_lKEBRPM) / (g_lKEBMMPerSec);
		}
	else
		dwRPM	= 0;
    Write_KebValue(ADDR_SELTINDEX, (DWORD)cIndex, SET0_INIT);
    Write_KebValue(ADDR_INDEXSPED,         dwRPM, SET0_INIT);
}

void	CTaskKeb::SendRank_Inject()
{
	long	lStepRank;
	
	lStepRank	= GetDBValue("MCONF_SCREW_INJECT_NULL_RANK").lValue;
	
	if (m_lStepRank < MAXRANK_INJECT)
	{
		Write_KebValue(ADDR_SELTINDEX, (DWORD)m_lStepRank, SET0_INIT);
		Write_KebValue(ADDR_INDEXMODE,                  1, SET0_INIT);
	}
	
	if (lStepRank < MAXRANK_INJECT)
	{
		Write_KebValue(ADDR_SELTINDEX,  (DWORD)lStepRank, SET0_INIT);
		Write_KebValue(ADDR_INDEXMODE, 	               0, SET0_INIT);
	}
	
	m_lStepRank	= lStepRank;
}

void	CTaskKeb::SendAdjustInjectTorque(DWORD dwData)
{
    Write_KebValue(ADDR_SETTORQUE, dwData, SET4_LOWSPDINJ);
}

void	CTaskKeb::SendAdjustSuckBackTorque(DWORD dwData)
{
    Write_KebValue(ADDR_SETTORQUE, dwData, SET6_LOWSUCKBACK);
}

void	CTaskKeb::SendAdjustInjectSPD(DWORD dwData)
{
    Write_KebValue(ADDR_SETLOWSPD, (-1 * dwData), SET4_LOWSPDINJ);
}

void	CTaskKeb::SendAdjustSuckBackSPD(DWORD dwData)
{
    Write_KebValue(ADDR_SETLOWSPD, dwData, SET6_LOWSUCKBACK);
}

void    CTaskKeb::HookSetCurve(pCURVEFunc pFunc)
{
    m_pCurveFunc = pFunc;
}

void    CTaskKeb::UnHookSetCurve()
{
    m_pCurveFunc = NULL;
}

void	CTaskKeb::SetInjectCurve(WORD wSwitch)
{
	DWORD  dwposi  = abs(g_RealData.lPosi / 100);
	DWORD  dwSpeed = abs(g_RealData.lSpeed);
	DWORD  dwPress = abs(g_RealData.lPress);
	
	//James 既mark 2008/12/22 7:11PM
	//if (wSwitch & KEBCURVE_ON)
	//{
	//	HookSetCurve((pCURVEFunc)SetInjectCurve);
	//	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALSPEED);
	//	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALPRESS);
	//	wSwitch	|= KEBCURVE_OPING;
	//}
	//
	//if (wSwitch & KEBCURVE_OPING)
	//{
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALSPEED, &dwposi, &dwSpeed, 1);
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_INJECT_REALPRESS, &dwposi, &dwPress, 1);
	//	//printf("Speed = %d, posi = %d\n", dwSpeed, dwposi);
	//	return;
	//}
	
	if (wSwitch & KEBCURVE_OFF)
	{
		//CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALSPEED);
		//CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_INJECT_REALPRESS);
		//printf("End Inject Curve !");
		UnHookSetCurve();
		return;
	}
}

void	CTaskKeb::SetHoldCurve(WORD wSwitch)
{
	static long	lLastCnt, lSumCnt;
	DWORD  dwPress = abs(g_RealData.lPress);
	
	//James 既mark 2008/12/22 7:12PM
	//if (wSwitch & KEBCURVE_ON)
	//{
	//	HookSetCurve((pCURVEFunc)SetHoldCurve);
	//	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALPRESS);
	//	lLastCnt	= g_RealData.lTimerCnt;
	//	lSumCnt		= 0;
	//	wSwitch	|= KEBCURVE_OPING;
	//}
	//
	//if (wSwitch & KEBCURVE_OPING)
	//{
	//	if (g_RealData.lTimerCnt >= lLastCnt)	lSumCnt	+= g_RealData.lTimerCnt - lLastCnt;
	//	else	lSumCnt	+= 65535 - lLastCnt + g_RealData.lTimerCnt;
	//	lLastCnt	= g_RealData.lTimerCnt;
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_HOLD_REALPRESS, (DWORD *)&lSumCnt, &dwPress, 1);
	//	return;
	//}
	
	if (wSwitch & KEBCURVE_OFF)
	{
		lLastCnt	= 
		lSumCnt		= 0;
		//CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_HOLD_REALPRESS);
		UnHookSetCurve();
		return;
	}
}

void	CTaskKeb::SetBackCurve(WORD wSwitch)
{
	DWORD  dwposi  = abs(g_RealData.lPosi / 100);
	DWORD  dwPress = abs(g_RealData.lPress);
	
	//if (wSwitch & KEBCURVE_ON)
	//{
	//	HookSetCurve((pCURVEFunc)SetBackCurve);
	//	CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARRG_REALPRESS);
	//	wSwitch	|= KEBCURVE_OPING;
	//}
	//
	//if (wSwitch & KEBCURVE_OPING)
	//{
	//	CtmCurveDataControl::GetInstance()->SetData(CURVE_OIL_DATA_CHARRG_REALPRESS, &dwposi, &dwPress, 1);
	//	return;
	//}
	
	if (wSwitch & KEBCURVE_OFF)
	{
		//CtmCurveDataControl::GetInstance()->SetComplete(CURVE_OIL_DATA_CHARRG_REALPRESS);
		UnHookSetCurve();
		return;
	}
}

void    CTaskKeb::SendMessageToKEB(WORD wOPCode)
{
	if ((WORD)(GetDBValue("STATE_OTHR_OTHR_NULL_OPERSTEP1").lValue) & 0x0001)  // 关模时送射出开始段位
    {
        SendInjectStartIndex();
    }
    
	//End to Send KEB Curve
    if (!(m_wLastKEBOP & wOPCode) && (m_wLastKEBOP != KEBOP_NULL))
    {
        switch(m_wLastKEBOP)
        {
            case KEBOP_INJECT:
				SetInjectCurve(KEBCURVE_OFF);
                break;
            case KEBOP_CHARGE:
                SetBackCurve(KEBCURVE_OFF);
                break;
            case KEBOP_HOLD:
            	SetHoldCurve(KEBCURVE_OFF);
                break;
            default :
                break;
        }
        m_wLastKEBOP    &= KEBOP_NULL;
    }
    
    //Avoid to hold begin flag include inject flag then inject not to stop
    if ((m_wLastKEBOP == KEBOP_INJECT) && (wOPCode == KEBOP_HOLD))
    {
    	m_wLastKEBOP	&= KEBOP_NULL;
        SetInjectCurve(KEBCURVE_OFF);
   	}
    
    //Begin to Send KEB Curve
    if ((wOPCode & KEBOP) && (m_wLastKEBOP == KEBOP_NULL))
    {
        m_wLastKEBOP    |= wOPCode;
        switch(wOPCode)
        {
            case KEBOP_INJECT:
				SetInjectCurve(KEBCURVE_ON);
                break;
            case KEBOP_CHARGE:
                SetBackCurve(KEBCURVE_ON);
                break;
            case KEBOP_HOLD:
				SetHoldCurve(KEBCURVE_ON);
                break;
            default :
                break;
        }
    }
}
