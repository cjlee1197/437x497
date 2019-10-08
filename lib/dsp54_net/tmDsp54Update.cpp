
#include	"tmDsp54Update.h"
#include	"../../utils.h"
#include	"../../main.h"
#include	"../../tmconfig.h"
#include	"../../commonaction.h"
#include	"../../curvedata.h" 
#include	"../../qualitycontrol.h"
#include	"../../manufacture.h"
#include    "../../tmdebug.h"
#include	"../../oil54struct.h"
#include	"../../selvat_ancestor.h"
#include	"../../default.h"
#include	"../libControl.h"
#include	"../libComm.h"
#include	"../../tmshmsg.h"
#include 	"../../3des.h"

//#define		COMM_TIME_CONNECT			(3 * 1024)
#define		COMM_TIME_ECHO				500

#define		COMM_STATE_NONE				0
#define		COMM_STATE_SEND				1
#define		COMM_STATE_GETECHO			2

#ifdef	D_3354
#define		FILE_USBPATH_DSPLASTBAK		"/media/sda1/DspLastVerBak/"
#define		FILE_USBPATH_UPDATEFILE		"/media/sda1/updatefile/"
#else
#define		FILE_USBPATH_DSPLASTBAK		"/mnt/usb/DspLastVerBak/"
#define		FILE_USBPATH_UPDATEFILE		"/mnt/usb/updatefile/"
#endif

IMPLEMENT_DYNCREATE(CtmDsp54Update, CtmDsp54UpdateProtocol)
IMPLEMENT_DYNCREATE(CtmDsp54UpdateProtocol, CtmProtocol)

static      ETHNETFRAMETX	g_frameTx;
static      ETHNETFRAMERX	g_frameRx;
static		int nUpdateDeviceId;
static		int nUpdateDevice = 0;							//琌秨币蔼锣传 0=dsp28 1=dsp54
long		u_lEchoTime		= COMM_TIME_ECHO;
		
const		WORD	g_wSendCmd[] = {COMM_ETH_SENDDLSTART,	//James add for Check echo
									COMM_ETH_SENDDLDATA,
									COMM_ETH_SENDDLEND,
									COMM_ETH_SENDDLREADY,
									COMM_ETH_SENDDLSMALL};

const       COMMMAP g_commmap[] = {
              {COMM_ETH_SENDDLSTART				, &CtmDsp54Update::GetEcho},
        	{COMM_ETH_SENDDLDATA				, &CtmDsp54Update::GetEcho},
        	{COMM_ETH_SENDDLEND					, &CtmDsp54Update::GetEcho},
        	{COMM_ETH_SENDDLREADY				, &CtmDsp54Update::GetEcho},
        	{COMM_ETH_SENDDLSMALL				, &CtmDsp54Update::GetEcho},
        	{COMM_ETH_MODE_TRANSFORM		,	&CtmDsp54Update::GetEcho},
        	{COMM_ETH_MODE_NORMAL				,	&CtmDsp54Update::GetEcho},
        	
        	{COMM_ETH_GETDLSTART				, &CtmDsp54Update::GetDLStart},
        	{COMM_ETH_GETDLDATA					, &CtmDsp54Update::GetDLData},
        	{COMM_ETH_GETDLEND					, &CtmDsp54Update::GetDLEnd},
        	{COMM_ETH_GETDLREADY				, &CtmDsp54Update::GetDLReady},
        	{COMM_ETH_GETDLSMALL				, &CtmDsp54Update::GetDLSmall},
            };

/*!	\fn  CtmDsp54UpdateProtocol::CtmDsp54UpdateProtocol()
 *	\brief	篶硑ㄧ计
 *	\return 0
 */
CtmDsp54UpdateProtocol::CtmDsp54UpdateProtocol()
{
	m_nStartPos	    = 0;
	m_nPackSizePos	= 2;
	m_nTypePos		= 4;
	m_nCMDPos	    = 6; 
	m_nIDPos		= 8;
	m_nDataLenPos   = 10;
	
	m_nCRCPos       = 0;
	m_nEndPos	    = 2;
}

/*!	\fn int	CtmDsp54All::CtmDsp54UpdateProtocol::~CtmDsp54UpdateProtocol()
 *	\brief	猂篶ㄧ计
 *	\return 0
 */
CtmDsp54UpdateProtocol::~CtmDsp54UpdateProtocol()
{
	
}

/*!	\fn int	CtmDsp54UpdateProtocol::InitSend()
 *	\brief	﹍て祇癳buffer
 *	\return 0
 */
int		CtmDsp54UpdateProtocol::InitSend()
{
	m_SendHeader.SetMaxSize(12);
	m_SendHeader.SetSize(12);
	m_SendFooter.SetMaxSize(4);
	m_SendFooter.SetSize(4);
	
	WORD	wTemp;
	wTemp	= 0x0002;
	m_SendHeader.SetPartValue(m_nStartPos, &wTemp, sizeof(wTemp));
	wTemp   = m_nSendType;
	m_SendHeader.SetPartValue(m_nTypePos, &wTemp, sizeof(wTemp));
	wTemp	= 0x0003;
	m_SendFooter.SetPartValue(m_nEndPos, &wTemp, sizeof(wTemp));
	return 0;
}

/*!	\fn int	CtmDsp54UpdateProtocol::InitRev()
 *	\brief	﹍て钡Μbuffer
 *	\return 0
 */
int		CtmDsp54UpdateProtocol::InitRev()
{
	m_RevHeader.SetMaxSize(12);		//	the Rev header
	m_RevHeader.SetSize(12);		//	the Rev header
	
	m_RevFooter.SetMaxSize(4);		
	m_RevFooter.SetSize(4);
	
	WORD wTemp   = m_nRevType;
	m_SendHeader.SetPartValue(m_nTypePos, &wTemp, sizeof(wTemp));
	return 0;
}

CtmDsp54Update::CtmDsp54Update()
{
    m_nMaxLen			= 1490*2;
    m_nConnetSate		= COMM_STATE_NONE;		//硄癟篈	0=None 1=send 2=get echo 
    m_wCommandIndex		= 0;		//㏑絯
	m_wCmdState 		= CMD_NONE; //讽玡㏑篈
	m_wDLState			= 0;
	m_wGetCmdState		= 0;
	m_wSetCmdState		= 0;
	m_wBurnDeviceType	=0;
	m_wCheckDSP55Type	=0xFFFF;
	m_nFileNameLen		=0;
	m_nRevNewFileNameLen =0;
	m_wRecvPacketCount	=0;
	m_bTransferEcho		=FALSE;
	
	//memset(m_cRevFileName, 0, sizeof(m_cRevFileName));
	//memset(m_cSendFileName, 0, sizeof(m_cSendFileName));
	memset(&m_SendFileInfo,  0, sizeof(m_SendFileInfo));
	memset(&m_sysFileInfo, 	 0,	sizeof(m_sysFileInfo));
	memset(&m_sysRevFileInfo,0,	sizeof(m_sysRevFileInfo));
	memset(m_cszSuffixName,	 0, sizeof(m_cszSuffixName));
	memset(m_cszSendFileName,0, sizeof(m_cszSendFileName));
	memset(m_cszRevFileName, 0,	sizeof(m_cszRevFileName));
	memset(m_cszRevNewFileName,0, sizeof(m_cszRevNewFileName));
	
	m_nMaxPackSize	= COMM_UPDATE_PACKSIZE_54;				//James add (dsp28 穝size 0x100, dsp54size 0x400)
	m_SendFileInfo.lPackageSize = m_nMaxPackSize;
	m_CmdQueue.wInput     = m_CmdQueue.wOutput = 0;
	m_CmdQueue.wQueueEnd  = MAX_COMMAND_KEEP;
	m_CmdQueue.bQueueFull = FALSE;
	AddTimer(&u_lEchoTime);
}

CtmDsp54Update::~CtmDsp54Update()
{
	DeleteTimer(&u_lEchoTime);
}


int		CtmDsp54Update::CreateSelf()
{
    CtmProtocol::CreateSelf();
    m_pData		= new BYTE[m_nMaxLen];
	m_nSendType	= 10;
	m_nRevType	= 1010;
	m_wUpdateType = 0;
//	m_wUploadFlag	= 0;
	InitSend();
	InitRev();
	
	return 0;
}

int		CtmDsp54Update::FreeSelf()
{
    CtmProtocol::FreeSelf();
    delete [] m_pData;
    return 0;
}

/*!	\fn int	CtmDsp54Update::SaveDataToDB(void* buf, int nLen, WORD wFirstID)
 *	\brief	玂诀祇癳筁ㄓ穝计沮
 *	\param	[void*]	buf			计沮ず甧
 *	\param	[in]	nLen		计沮
 *	\param	[WORD]	wFirstID	计沮ID
 *	\return 0
 */
BOOL     CtmDsp54Update::SaveDataToDB(void* buf, int nLen, WORD wFirstID)
{
	int			nResult 	= 0;

	return (nResult == nLen);
}

BOOL    CtmDsp54Update::DataIsValid(void* pInData, int nInputLen)
{
    WORD wTemp = 0;
    WORD wCRC = 0;
    WORD * pwTemp = NULL;
    
    memcpy(m_RevHeader.GetBuffer(), pInData, m_RevHeader.GetSize());
    memcpy(m_RevFooter.GetBuffer(), 
        (char *)pInData+(nInputLen-m_RevFooter.GetSize()), m_RevFooter.GetSize());
    //check 计沮琌タ絋
    m_RevHeader.GetPartValue(m_nPackSizePos, &wTemp, sizeof(WORD));
    if(wTemp<<1 != nInputLen)
    {
        cout<< "PackSize is wrong, right is : "<< nInputLen/2<< endl;
        goto ERROR;
    }
    //check 肚癳戈WORD羆计琌タ絋
    m_RevHeader.GetPartValue(m_nDataLenPos, &wTemp, sizeof(WORD));
    if(wTemp<<1 != nInputLen-m_RevHeader.GetSize()-m_RevFooter.GetSize())
    {
        cout<< "DataSize is wrong, right is : "<< (nInputLen-m_RevHeader.GetSize()-m_RevFooter.GetSize())/2<< endl;
        goto ERROR;
    }
    //check CRC喷琌タ絋
    m_RevFooter.GetPartValue(m_nCRCPos, &wTemp, sizeof(WORD));
    pwTemp = (WORD *)pInData+1;
    for(int i=0; i<(nInputLen-6)>>1; i++)
    {
        wCRC ^= *pwTemp++;
    }
    if(wCRC != wTemp)
    {
        cout<< "CRC is wrong, right is : "<< wCRC<< endl;
        goto ERROR;
    }
    return TRUE;
ERROR:
    cout<< "Data is not valid"<< endl;
    return FALSE;
}

int		CtmDsp54Update::ComposeProtocol	(void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
	int nCmd = *(WORD*)((BYTE*)pOutData+4);
	return 0;
}

int		CtmDsp54Update::ComposeProtocol(int nCmd, void* pOutData,	void* pInData, int nInputLen, void* pEx)
{
    int nResult = 0;
//    WORD wTemp   = m_nRevType;
   // if(nCmd != 0x9005 && nCmd != 0x9009 && nCmd != 0x9007)
    //	printf("CtmDsp54Update::ComposeProtocol nCmd=%x \n", nCmd);
    //if(nCmd != 0x9005 && nCmd != 0x9007)
//   	printf("CtmDsp54Update ComposeProtocol nCmd=%x \n", nCmd);
    switch(nCmd)
	{
		case COMM_ETH_MODE_TRANSFORM:
			//printf("Send Transfor...\n");
			m_bTransferEcho=FALSE;
			TransferToDsp28();
			break;
		case COMM_ETH_SENDDLSTART:		//秨﹍穝
			//1.先發送版本信息
			//printf("Send DL Start ...\n");
			m_nFileNameLen=nInputLen;
			SendDLStart(pInData);
			break;
		case COMM_ETH_SENDDLDATA:		//祇癳穝计沮
			SendDLData();
			break;
		case COMM_ETH_SENDDLEND:		//穝挡
			SendDLEnd();
			break;
		case COMM_ETH_SENDDLEXIT:
			//printf("dsp28 send exit...\n");
			SendExit();
			break;
		case COMM_ETH_SENDDLREADY:		//非称秨﹍穝
			SendDLReady();
			break;
		case COMM_ETH_SENDDLSMALL:		//穝祘Α
			SendDLSmall(pInData);
			break;
		
		case COMM_ETH_GETDLSTATE:	
			GetDLStepState(pOutData);
			break;
		case COMM_ETH_SETDLSTATE:
			m_wDLState = *(WORD*)pInData;
			break;
		case COMM_ETH_GETDLOLDVERS:
			GetDLOldVers(pOutData);
			break;
		case COMM_ETH_SETDLOLDVERS:
			m_nRevNewFileNameLen=nInputLen;
			SetDLOldVers(pInData);
			break;
		case COMM_ETH_GETDLCMDSTATE:
			if( pOutData != NULL)
			{
				memcpy(pOutData , &m_wCmdState, sizeof(m_wCmdState));	
			}
			break;
		case COMM_ETH_SETBURNTYPE:
			if(pInData != NULL)
			{
				m_wBurnDeviceType=*(WORD*)pInData;
			}
			//printf("m_wBurnDeviceType=%04X\n",m_wBurnDeviceType);
			break;
		case COMM_ETH_SELECT28:
			if(pInData != NULL)
			{	
				//包村更新設備的包大小
				nUpdateDeviceId = *(int *)pInData;
				nUpdateDevice = COMM_UPDATEDEVICE_DSP28;
				m_nMaxPackSize = COMM_UPDATE_PACKSIZE_28;			//James add 2010/4/29 dsp28 data size 100H
				m_SendFileInfo.lPackageSize = m_nMaxPackSize;
			}
			break;
		case COMM_ETH_SELECT54:
			if(pInData != NULL)
			{
				nUpdateDeviceId = *(int *)pInData;
				nUpdateDevice = COMM_UPDATEDEVICE_DSP54;
				m_nMaxPackSize = COMM_UPDATE_PACKSIZE_54;			//James add 2010/4/29 dsp54 data size 400H
				m_SendFileInfo.lPackageSize = m_nMaxPackSize;
			}
			break;
		case COMM_ETH_SELECT54TRANSMIT:								//James modify 2010/5/13 for add dsp28 transmit
			if(pInData != NULL)
			{
				nUpdateDeviceId = *(int *)pInData;
				nUpdateDevice = COMM_UPDATEDEVICE_DSP54TRANSMIT;
				m_nMaxPackSize = COMM_UPDATE_PACKSIZE_28;			//James add 2010/4/29 dsp54 data size 400H
				m_SendFileInfo.lPackageSize = m_nMaxPackSize;
			}
			break;
		case COMM_ETH_SENDRUNTIME:
			Update_Run();
			break;
		 //the next all for test
		case COMM_ETH_GETDLSTART:
			GetDLStart();
			break;
			
		case COMM_ETH_GETDLDATA:
			GetDLData();
			break;
		
		case COMM_ETH_GETDLEND:
			GetDLEnd();
			break;
		
		case COMM_ETH_GETDLREADY:
			GetDLReady();
			break;
			
		case COMM_ETH_GETDLSMALL:
			GetDLSmall();
			break;
		case COMM_ETH_GETPACKCOUNT:
			if (m_wDLState&0x80)
				memcpy(pOutData , &m_SendFileInfo.lCurrentNub, sizeof(m_SendFileInfo.lCurrentNub));
			else if(m_wDLState&0x10)
				memcpy(pOutData , &m_wRecvPacketCount, sizeof(m_wRecvPacketCount));
			break;
		case COMM_ETH_SETSUFFIXNAME:
			memset(m_cszSuffixName,0,sizeof(m_cszSuffixName));
			memcpy(m_cszSuffixName,pInData,nInputLen);
			break;
		case COMM_ETH_MODE_NORMAL:
				TurnToNormal();
			break;
		case COMM_ETH_GETTRANSFORM_ECHO:
			 usleep(500*1000);
			 //printf("1:m_bTransferEcho=%d\n",m_bTransferEcho);
			 memcpy(pOutData,&m_bTransferEcho,sizeof(m_bTransferEcho));
			 break;
	    default:
			break;
	}
    
	return nResult;
}

BOOL CtmDsp54Update::DoDealFunc(WORD wCmd)
{
    int i = 0;
  //  g_tmDebugInfo->PrintDebugInfo("g_commmap[i].wCmd=%x \n", wCmd);
    while ((g_commmap[i].wCmd != wCmd) && (g_commmap[i].wCmd != COMM_NULL)) i++;
	if (g_commmap[i].wCmd != COMM_NULL)
	{
		m_nCommandIndex = g_commmap[i].wCmd;
	    (this->*(g_commmap[i].npFunc))();
	    return TRUE;
	}
	return FALSE;
}

/*!	\fn int	CtmDsp54Update::ParseProtocol	(void* pInData,		int nInputLen)
 *	\brief	秆猂诀祇筁ㄓ计沮
 *	\param	[void*]	pInData		计沮ず甧
 *	\param	[in]	nInputLen	计沮
 *	\return 0
 */
int		CtmDsp54Update::ParseProtocol	(void* pInData,		int nInputLen)
{
    memcpy(&g_frameRx, (char*)pInData+m_nCMDPos, nInputLen-m_nCMDPos);
    DoDealFunc(g_frameRx.wCmd);
	return 0;
}
/*****************************************************************************/
/*                          HELP FUNCTION                                    */
/*****************************************************************************/

void    CtmDsp54Update::GetReadData()
{
	Echo(COMM_ECHO_SUCCESS);
    WORD wTempCmd = *(WORD *)(g_frameRx.commdata.acBuffer+2*sizeof(WORD));      
    ComposeProtocol(wTempCmd, NULL,	NULL, 0, NULL);
}

void	CtmDsp54Update::GetEcho()
{
    WORD 			wResult = 0xFFFF, wLength = 0, wPackNumber = 0, wPackCount = 0;
    wResult 	= *(WORD*)(g_frameRx.commdata.acBuffer+2*sizeof(WORD));
    wLength 	= *(WORD*)(g_frameRx.commdata.acBuffer+sizeof(WORD));
    wPackNumber	= *(WORD*)(g_frameRx.commdata.acBuffer);
	for(int i = 0; i < sizeof(g_wSendCmd)/sizeof(WORD); i++)		//James add for check echo
    {
    	if(g_wSendCmd[i] == g_frameRx.wCmd)
    	{
    		m_nConnetSate 	= COMM_STATE_GETECHO;
    		m_wCommandIndex = 0;
    	}
    }
	//printf("wResult=%d\n",wResult);
	if(wResult == 0)
	{
		//printf("wLength=%d\n",wLength);
		if(wLength == 1)
		{
			m_wGetCmdState 	= g_frameRx.wCmd;
			m_wCmdState 	= CMD_ECHO;
			switch(g_frameRx.wCmd)
			{
				case COMM_ETH_MODE_TRANSFORM:
					 m_bTransferEcho=TRUE;
					// printf("2:m_bTransferEcho=%d\n",m_bTransferEcho);
					 break;
				case COMM_ETH_SENDDLSTART:
					m_wDLState 	|= 0x02;
					if(m_nMaxPackSize == COMM_UPDATE_PACKSIZE_28)
						m_SendFileInfo.lCurrentPos += m_SendFileInfo.lCurrentSize;
					break;
				case COMM_ETH_SENDDLDATA:
					m_wDLState 	|= 0x80;
					//g_tmDebugInfo->PrintDebugInfo("wPackNumber=%d curnub=%d packagecount=%d \n", wPackNumber, m_SendFileInfo.lCurrentNub, m_SendFileInfo.lPackageCount);
					if(wPackNumber == m_SendFileInfo.lCurrentNub)
					{
						m_SendFileInfo.lCurrentNub ++;
						m_SendFileInfo.lCurrentPos += m_SendFileInfo.lCurrentSize;
						m_SendFileInfo.lCurrentSize = 0;
					}
					if(m_nMaxPackSize == COMM_UPDATE_PACKSIZE_28)
						wPackCount = m_SendFileInfo.lPackageCount-2;
					else
						wPackCount = m_SendFileInfo.lPackageCount-1;
					//printf("current=%d,wPackCount=%d\n",m_SendFileInfo.lCurrentNub,wPackCount);
					if(m_SendFileInfo.lCurrentNub >wPackCount)
					{
						m_wDLState 	|= 0x100;
						SendDLEnd();
					}
					break;
				case COMM_ETH_SENDDLEND:
					m_wDLState 	|= 0x200;
					memset(&m_SendFileInfo, 0, sizeof(m_SendFileInfo));	//James add 2009/9/23
					memset(&m_sysFileInfo,0,sizeof(m_sysFileInfo));
					m_SendFileInfo.lPackageSize = m_nMaxPackSize;		//James add 2009/9/23
					
					if(m_nMaxPackSize == COMM_UPDATE_PACKSIZE_28)
					{
						SendExit();
						usleep(200*1000);
						TurnToNormal();
					}
					//printf("echo send end...\n");
					SendMsg(MSG_SH_DSP54UPDATE_DLEND, 0, 0, NULL);
					m_nConnetSate = COMM_STATE_NONE;
					break;
				case COMM_ETH_SENDDLREADY:
					m_wDLState 	|= 0x08;
					if(m_wBurnDeviceType&0x01 == 1)		//no upload 
						m_wDLState	|= 0x20;
					break;
				case COMM_ETH_SENDDLSMALL:
					m_wDLState 	|= 0x02;			//James ADD 2013/4/7 16:17:47
					break;
				default:
					break;
			};
		}
		return ; 
	}
	else if(wResult == 1)
	{
		switch(g_frameRx.wCmd)
		{
			case COMM_ETH_SENDDLSTART:
				if(m_nMaxPackSize == COMM_UPDATE_PACKSIZE_28)
				{
					SendExit();
					//printf("echo send end...\n");
					SendMsg(MSG_SH_DSP54UPDATE_DLFILEERR, 0, 0, NULL);
				}
				else
				{
					char szTemp[32];
					strcpy(szTemp,m_cszSendFileName);
					SendDLStart(szTemp);
				}
				break;
			case COMM_ETH_SENDDLDATA:
				SendDLData();
				break;
			case COMM_ETH_SENDDLEND:
				SendDLEnd();
				break;
			case COMM_ETH_SENDDLREADY:
				SendDLReady();
				break;
			case COMM_ETH_SENDDLSMALL:
				SendDLSmall();
				break;
			case COMM_ETH_MODE_TRANSFORM:
				TransferToDsp28();
				break;
			case COMM_ETH_MODE_NORMAL:
				TurnToNormal();
				break;
			default:
				break;
		};
	}
	else if(wResult == 2)
	{
		SendMsg(MSG_SH_DSP54UPDATE_DLFILEERR, 0, 0, NULL);
	}
}

void	CtmDsp54Update::Echo(WORD wCode)
{
	int         nWriteCount;
	WORD        wFirstID = 0;
	
	wFirstID = g_frameRx.commdata.wResult;             
	g_frameRx.commdata.wResult = wCode;                
	
	WriteToken((BYTE*)&g_frameRx, sizeof(WORD), &nWriteCount, wFirstID);
}

void	CtmDsp54Update::WriteToken(BYTE* pch, int nCount, int* pnWriteCount, WORD wFirstID)
{
    WORD wTemp = 0;
 	//包長度
    wTemp = (m_SendHeader.GetSize() + nCount + m_SendFooter.GetSize()) >>1;

    m_SendHeader.SetPartValue(m_nPackSizePos, &wTemp, sizeof(WORD));
   // wTemp=0x0001;
   // m_SendHeader.SetPartValue(m_nTypePos, &wTemp, sizeof(WORD));
    //CMD 
    wTemp = *(WORD *)pch;
  
    for(int i = 0; i < sizeof(g_wSendCmd)/sizeof(WORD); i++)
    {
    	if(g_wSendCmd[i] == wTemp)
    	{
    		m_nConnetSate 	= COMM_STATE_SEND;
    		u_lEchoTime		= COMM_TIME_ECHO;
    		m_wCommandIndex = wTemp;
    		break;
    	}
    }
 
    m_SendHeader.SetPartValue(m_nCMDPos, &wTemp, sizeof(WORD));
    //保留ID
    wTemp = wFirstID;
    
    m_SendHeader.SetPartValue(m_nIDPos, &wTemp, sizeof(WORD));
    
    wTemp = nCount>>1;
    m_SendHeader.SetPartValue(m_nDataLenPos, &wTemp, sizeof(WORD));
    //m_SendHeader數據拷貝到m_pData 中
    memcpy(m_pData, m_SendHeader.GetBuffer(), m_SendHeader.GetSize());
   //數據長度
    memcpy(m_pData+m_SendHeader.GetSize(), pch+2, nCount);
    //更新類型
    memcpy(m_pData+4,&m_wUpdateType,sizeof(m_wUpdateType));
    
    wTemp = 0;
    WORD * pwTemp = (WORD *)m_pData+1;
    
    for(int i=0; i<(m_SendHeader.GetSize()-2+nCount)>>1; i++)
    {
        wTemp ^= *pwTemp++;
    }
    
    m_SendFooter.SetPartValue(m_nCRCPos, &wTemp, sizeof(WORD));
    memcpy(m_pData+(m_SendHeader.GetSize()+nCount), m_SendFooter.GetBuffer(), m_SendFooter.GetSize());
   	if(nUpdateDevice == COMM_UPDATEDEVICE_DSP54TRANSMIT)		
   	{
   		if(g_ptaskdsp != NULL)
   			g_ptaskdsp->WriteValue(COMM_SENDTRANSMITDSP28, (void*)m_pData, m_SendHeader.GetSize()+nCount+m_SendFooter.GetSize());
   	}
   	else
   	{
   		if((m_wBurnDeviceType>>8)&0xFF)		usleep(10*1000);    //当烧录设备为CAN设备时，每个数据包之间需要一定的延迟时间
    	SendData(nUpdateDeviceId, (char*)m_pData, m_SendHeader.GetSize()+nCount+m_SendFooter.GetSize());
    }
}

void 	CtmDsp54Update::SendDLStart(void* pInData)
{
	int nWriteCount = 0, nStorageFlag = 0;
	int	nResult = 0;
	m_wDLState  = 0;
	//if(pInData != NULL )
		//拷貝版本信息
	//memcpy(m_wSendFileName, pInData, sizeof(m_wSendFileName));
	memset(m_cszSendFileName,0,sizeof(m_cszSendFileName));
	memcpy(m_cszSendFileName,pInData,m_nFileNameLen);
	if(m_nMaxPackSize==COMM_UPDATE_PACKSIZE_54)
	{
		m_wSendFileName[0]=0x1111;
		m_wSendFileName[1]=0x2222;
		m_wSendFileName[2]=0x3333;
		memcpy(g_frameTx.commdata.acBuffer,		m_wSendFileName,	sizeof(m_wSendFileName));
	}	
	//printf("m_cSendFileName=%04x-%04x-%04x,burntype=%x\n", m_wSendFileName[0], m_wSendFileName[1], m_wSendFileName[2],m_wBurnDeviceType);
	//g_tmDebugInfo->PrintDebugInfo("m_cSendFileName=%04x-%04x-%4x\n", m_wSendFileName[0], m_wSendFileName[1], m_wSendFileName[2]);
	g_frameTx.wCmd 	= COMM_ETH_SENDDLSTART;//發送請求命令:0x60
	m_wSetCmdState	= COMM_ETH_SENDDLSTART;
	m_wCmdState		= CMD_SEND;//命令狀態為 1
	
	nStorageFlag = TOStorageType(1);
	
	//掛載好USB設備
	if(1 == nStorageFlag)
	{
		memset(&m_SendFileInfo, 0, sizeof(m_SendFileInfo));	//James add 2009/9/23
		memset(&m_sysFileInfo, 0,sizeof(m_sysFileInfo));
		m_SendFileInfo.lPackageSize = m_nMaxPackSize;	//James add 2009/9/23
		nResult = SendDatatoStorage((BYTE*)m_SendFileInfo.pData, m_SendFileInfo.lCurrentPos, m_SendFileInfo.lPackageSize, STORAGE_TYPE_USB);
		//update file name not exist
		//g_tmDebugInfo->PrintDebugInfo("SendDLStart nResult=%d \n", nResult);
		if(-1 == nResult)
		{
			return;
			//send file not exist msg
		}
	}
	else
	{
		return;	
	}
	//m_wDLState 	|= 0x01;
	m_wDLState 	= 0x01;
	
	WORD wFileLen = m_SendFileInfo.lFileLength/1024;
	
//	printf("wFileLen=%d\n",wFileLen);
	
	if(m_nMaxPackSize==COMM_UPDATE_PACKSIZE_28)//dsp28
	{
		memcpy(&m_sysFileInfo,m_SendFileInfo.pData,sizeof(m_sysFileInfo));
		memcpy(g_frameTx.commdata.acBuffer,&m_sysFileInfo,sizeof(m_sysFileInfo));
		//printf("send start wait ...\n");
		WriteToken((BYTE*)&g_frameTx,sizeof(m_sysFileInfo),&nWriteCount,m_wBurnDeviceType);
		//printf("send start ok ...\n");
	}
	else //dsp55&dsp54
	{
		//printf("packCurrent:%d\n",m_SendFileInfo.lCurrentNub);
		memcpy(&m_wCheckDSP55Type,m_SendFileInfo.pData,sizeof(WORD));
		//printf("m_wCheckDSP55Type=%04x\n",m_wCheckDSP55Type);
		HighToLow((BYTE*)&m_wCheckDSP55Type,sizeof(WORD));
		//printf("m_wChekcDSP55Type=%x,%x\n",m_wCheckDSP55Type,COMM_DSP55_FIFLETYPE);
		if(m_wCheckDSP55Type==COMM_DSP55_FIFLETYPE)
		{
			memcpy(g_frameTx.commdata.acBuffer+sizeof(m_wSendFileName), &wFileLen, sizeof(wFileLen));
			WriteToken((BYTE*)&g_frameTx, sizeof(m_wSendFileName)+sizeof(wFileLen), &nWriteCount, m_wBurnDeviceType);
		}
		else
		{
			SendMsg(MSG_SH_DSP54UPDATE_DLFILEERR, 0, 0, NULL);	
		}
	}
	u_lEchoTime		= COMM_TIME_ECHO;
}

void		CtmDsp54Update::SendDLData()
{
	int nResult 	= 0;
	int nWriteCount = 0;
	m_wSetCmdState	= COMM_ETH_SENDDLDATA;
	m_wCmdState		= CMD_SEND;
	g_frameTx.wCmd 	= COMM_ETH_SENDDLDATA;
	nResult = SendDatatoStorage((BYTE*)m_SendFileInfo.pData, m_SendFileInfo.lCurrentPos, m_SendFileInfo.lPackageSize, STORAGE_TYPE_USB);
	//g_tmDebugInfo->PrintDebugInfo("SendDLData nResult=%d size=%d pos=%d \n", nResult, m_SendFileInfo.lPackageSize, m_SendFileInfo.lCurrentPos);
	if(-1 == nResult)
	{
		//send file not exist msg
	}
	else if(nResult > 0)
	{
		memcpy(g_frameTx.commdata.acBuffer, m_SendFileInfo.pData,nResult);
		WriteToken((BYTE*)&g_frameTx, nResult, &nWriteCount, m_SendFileInfo.lCurrentNub);
	}
}

void		CtmDsp54Update::SendDLEnd()
{
	int nWriteCount = 0;
	g_frameTx.wCmd 	= COMM_ETH_SENDDLEND;
	m_wSetCmdState	= COMM_ETH_SENDDLEND;
	m_wCmdState		= CMD_SEND;
	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);
	m_wCommandIndex=0;
}

void		CtmDsp54Update::SendDLReady()
{
	int nWriteCount = 0;
	g_frameTx.wCmd 	= COMM_ETH_SENDDLREADY;
	m_wSetCmdState	= COMM_ETH_SENDDLREADY;
	m_wCmdState		= CMD_SEND;
	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);
}

void		CtmDsp54Update::SendDLSmall(void* pInData)
{
//	int nWriteCount = 0;
//	g_frameTx.wCmd 	= COMM_ETH_SENDDLSMALL;
//	m_wSetCmdState	= COMM_ETH_SENDDLSMALL;
//	m_wCmdState		= CMD_SEND;
//	m_wDLState 		|= 0x400;
//	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);
	
	int nWriteCount = 0, nStorageFlag = 0;
	int	nResult = 0;
	m_wDLState  = 0;
	if(pInData != NULL )
		memcpy(m_wSendFileName, pInData, sizeof(m_wSendFileName));
	
	//printf("SendDLStart m_wSendFileName[0]=%x\n", m_wSendFileName[0]);
	g_tmDebugInfo->PrintDebugInfo("m_cSendFileName=%x-%x-%x \n", m_wSendFileName[0], m_wSendFileName[1], m_wSendFileName[2]);
	g_frameTx.wCmd 	= COMM_ETH_SENDDLSMALL;
	m_wSetCmdState	= COMM_ETH_SENDDLSMALL;
	m_wCmdState		= CMD_SEND;
	memcpy(g_frameTx.commdata.acBuffer,		m_wSendFileName,	sizeof(m_wSendFileName));
	
	
	nStorageFlag = TOStorageType(1);
	//printf("nStorageFlag=%d \n", nStorageFlag);
	if(1 == nStorageFlag)
	{
		memset(&m_SendFileInfo, 0, sizeof(m_SendFileInfo));	//James add 2009/9/23
		m_SendFileInfo.lPackageSize = m_nMaxPackSize;	//James add 2009/9/23
		nResult = SendDatatoStorage((BYTE*)m_SendFileInfo.pData, m_SendFileInfo.lCurrentPos, m_SendFileInfo.lPackageSize, STORAGE_TYPE_USB);
		//update file name not exist
		g_tmDebugInfo->PrintDebugInfo("SendDLStart nResult=%d \n", nResult);
		//printf("nResult=%d \n", nResult);
		if(-1 == nResult)
		{
			return;
			//send file not exist msg
		}
	}
	else
	{
		return;	
	}
	m_wDLState 	= 0x01;
	
//	WORD wFileLen = m_SendFileInfo.lFileLength/1024;
//	memcpy(g_frameTx.commdata.acBuffer+sizeof(m_wSendFileName), &wFileLen, sizeof(wFileLen));
//	WriteToken((BYTE*)&g_frameTx, sizeof(m_wSendFileName)+sizeof(wFileLen), &nWriteCount);
	u_lEchoTime		= COMM_TIME_ECHO;						//James add 2010/4/27
}

void	CtmDsp54Update::GetDLStart()
{
	char cszRevFileName[3][5];
	memset(cszRevFileName, 0, sizeof(cszRevFileName));
	m_wGetCmdState	= COMM_ETH_GETDLSTART;
	m_wDLState 		|= 0x04;
	if(m_nMaxPackSize==COMM_UPDATE_PACKSIZE_54)
	{
		memset(m_cszRevFileName,0,sizeof(m_cszRevFileName));
		memcpy(m_wRevFileName, g_frameRx.commdata.acBuffer+2*sizeof(WORD), sizeof(m_wRevFileName));
		sprintf(m_cszRevFileName,"%04X-%04X-%04X",m_wRevFileName[0],m_wRevFileName[1],m_wRevFileName[2]);
		//printf("%s\n",m_cszRevFileName);
	}
	else if(m_nMaxPackSize==COMM_UPDATE_PACKSIZE_28)	//add by Wong 2014/11/13 17:10:34
	{
		char 	cszDateTime[8];
		DWORD	dwSwVersion=0;
		memset(&m_sysParseFileInfo,0,sizeof(m_sysParseFileInfo));
		memset(cszDateTime,0,sizeof(cszDateTime));
		memcpy(&m_sysRevFileInfo, g_frameRx.commdata.acBuffer+2*sizeof(WORD), sizeof(m_sysRevFileInfo));	
		memcpy(&m_sysParseFileInfo,&m_sysRevFileInfo,sizeof(m_sysRevFileInfo));
		//if(!(m_wBurnDeviceType&0xFE))
		{
			if(!(m_wBurnDeviceType&0xFFFE))
				DES3_DisEncrypt_BurnAll((LONG*)&m_sysParseFileInfo,224); 
			memcpy(cszDateTime,&m_sysParseFileInfo.adwSwVersion[0],sizeof(m_sysParseFileInfo.adwSwVersion[0]));
			dwSwVersion=m_sysParseFileInfo.adwSwVersion[1];
			//printf("%08X-%08x\n",m_sysParseFileInfo.adwSwVersion[0],m_sysParseFileInfo.adwSwVersion[1]);
			Revstr(cszDateTime,strlen(cszDateTime));
			memset(m_cszRevFileName,0,sizeof(m_cszRevFileName));
			if(strcmp(cszDateTime," "))
				sprintf(m_cszRevFileName,"%s-%04X-%04X",cszDateTime,(dwSwVersion>>16&0xFFFF),(dwSwVersion&0xFFFF));
			else
				sprintf(m_cszRevFileName,"%04X-%04X-%04X",0,(dwSwVersion>>16&0xFFFF),(dwSwVersion&0xFFFF));	
		} 
//		else
//		{
//			memset(m_cszRevFileName,0,sizeof(m_cszRevFileName));
//			sprintf(m_cszRevFileName,"%04X-%04X-%04X",(m_sysParseFileInfo.adwSwVersion[0]&0xFFFF),(dwSwVersion>>16&0xFFFF),(dwSwVersion&0xFFFF));	
//		}
//		//printf("recv1====%s\n",m_cszRevFileName);
	} 
	if(m_cszRevNewFileName[0] != '\0')		//Jamea add 2009/10/19 эゅン嘿
	{
		;
	}
	else
	{
		memset(m_cszRevNewFileName,0,sizeof(m_cszRevNewFileName));
		memcpy(m_cszRevNewFileName, m_cszRevFileName, strlen(m_cszRevFileName));
	}
	//printf("GetDLStart 1=%x-%x-%x \n", m_wRevFileName[0], m_wRevFileName[1],m_wRevFileName[2]);
	//g_tmDebugInfo->PrintDebugInfo("GetDLStart 1=%x-%x-%x \n", m_wRevFileName[0], m_wRevFileName[1],m_wRevFileName[2]);

	
	Echo(COMM_ECHO_SUCCESS);
	SendDLReady();
}

void	CtmDsp54Update::GetDLData()
{
	WORD	wID	= 0, wLength = 0;
	long	lStartAddr = 0;
	if(m_wSetCmdState == COMM_ETH_SENDDLREADY)
	{
		wID				= *(WORD*)(g_frameRx.commdata.acBuffer);
		wLength			= *(WORD*)(g_frameRx.commdata.acBuffer+sizeof(WORD))*2;
		m_wGetCmdState	= COMM_ETH_GETDLDATA;
		m_wDLState 		|= 0x10;
		
		if(wID <= COMM_UPDATE_PACKMAX)
		{
			lStartAddr = wID * m_nMaxPackSize;
			//printf("recv data id: %d\n",wID);
			Echo(COMM_ECHO_SUCCESS);
			m_wRecvPacketCount++;
		}
		else
		{
			Echo(COMM_ECHO_FAILURE);
			return ;
		}
		SaveDatatoStorage(g_frameRx.commdata.acBuffer+2*sizeof(WORD), lStartAddr, wLength, STORAGE_TYPE_USB);
		SendDLReady();
	}
	
}

void	CtmDsp54Update::GetDLEnd()
{
	m_wGetCmdState	= COMM_ETH_GETDLEND;
	m_wRecvPacketCount=0;
	Echo(COMM_ECHO_SUCCESS);
	
	if(m_wSetCmdState == COMM_ETH_SENDDLREADY)
	{
		m_wDLState |= 0x20;
	}
}

void	CtmDsp54Update::GetDLReady()
{
	m_wGetCmdState	= COMM_ETH_GETDLREADY;
	Echo(COMM_ECHO_SUCCESS);
	m_wDLState 	|= 0x40;
	m_wCmdState=CMD_ECHO;
	//printf("m_wDLState=%x,%x\n",m_wDLState,m_wDLState & 0x20);
	if(m_wDLState & 0x20)
	{
		SendDLData();
	}
}

void	CtmDsp54Update::GetDLSmall()
{
//	int nWriteCount = 0;
//	Echo(COMM_ECHO_SUCCESS);
//	m_wGetCmdState	= COMM_ETH_GETDLSMALL;
//	g_frameTx.wCmd 	= COMM_ETH_GETDLSMALL;
//	m_wDLState 		|= 0x800;
//	SendMsg(MSG_SH_DSP54UPDATE_DLSMALLEND, 0, 0, NULL);
//	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);
	char cszRevFileName[3][5];
	memset(cszRevFileName, 0, sizeof(cszRevFileName));
	m_wGetCmdState	= COMM_ETH_GETDLSMALL;
	g_frameTx.wCmd 	= COMM_ETH_GETDLSMALL;
	m_wDLState 		|= 0x04;
	//áNAME(VVVV-YMDD-TIME)
	memcpy(m_wRevFileName, g_frameRx.commdata.acBuffer+2*sizeof(WORD), sizeof(m_wRevFileName));
	if(m_wRevNewFileName[0] != 0 && m_wRevNewFileName[1] != 0)		//Jamea add 2009/10/19
	{
		//check
		;
	}
	else
	{
		memcpy(m_wRevNewFileName, m_cszRevFileName, strlen(m_cszRevFileName));
	}
	//printf("GetDLStart 1=%x-%x-%x \n", m_wRevFileName[0], m_wRevFileName[1],m_wRevFileName[2]);
	g_tmDebugInfo->PrintDebugInfo("GetDLStart 1=%x-%x-%x \n", m_wRevFileName[0], m_wRevFileName[1],m_wRevFileName[2]);
	
	Echo(COMM_ECHO_SUCCESS);
	
	SendDLReady();
}

void	CtmDsp54Update::GetDLStepState(void* pOutData)
{	
	if(pOutData != NULL)
	{
		memcpy(pOutData, &m_wDLState, sizeof(m_wDLState));
	}
}

void	CtmDsp54Update::GetDLOldVers(void* pOutData)
{
	
	if(pOutData != NULL)
	{
		memcpy(pOutData, m_cszRevFileName, strlen(m_cszRevFileName));
	}
}

void	CtmDsp54Update::SetDLOldVers(void* pInData)
{
	if(pInData != NULL)
	{
		//memcpy(m_wRevNewFileName, pInData, sizeof(m_wRevNewFileName));
		memset(m_cszRevNewFileName,0,sizeof(m_cszRevNewFileName));
		memcpy(m_cszRevNewFileName,pInData,m_nRevNewFileNameLen);
	}
}

int CtmDsp54Update::SaveDatatoStorage(BYTE* pData, long lStartAddr, long lSize, tmStorageType StorageType)
{
	int nResult 	= 0, nStorageFlag = 0;
	int	nFd			= 0;
	CtmFile			u_File;
	char cszFileName[256], cszStoragepath[256];
	
	memset(cszFileName, 0, sizeof(cszFileName));
	memset(cszStoragepath, 0, sizeof(cszStoragepath));
	nStorageFlag = TOStorageType(1);
	if(1 == nStorageFlag)
	{
		DIR*		dir;
		dir = opendir(FILE_USBPATH_DSPLASTBAK);
		if (!dir) 
		{
			#ifdef D_PXA270
			mkdir(FILE_USBPATH_DSPLASTBAK, 777);
			#else
			mkdir("/mnt/mmc/DspLastVerBak/", 777);
			#endif
		//printf("OK!\n");
		}
		closedir(dir);

		#ifdef D_PXA270
		strcpy(cszStoragepath, FILE_USBPATH_DSPLASTBAK);
		#else
		strcpy(cszStoragepath, "/mnt/mmc/DspLastVerBak/");
		#endif
		//Get Download FileName
		//sprintf(cszFileName, "%04x-%04x-%04x", m_wRevNewFileName[0], m_wRevNewFileName[1], m_wRevNewFileName[2]); //James modify 2009/10/19 m_wRevFileName->m_wRevNewFileName
		sprintf(cszStoragepath, "%s%s%s", cszStoragepath, m_cszRevNewFileName,m_cszSuffixName);
		u_File.Open(cszStoragepath	, O_RDWR | O_CREAT | O_SYNC);
		g_tmDebugInfo->PrintDebugInfo("cszFileName=%s cszStoragepath=%s \n", cszFileName, cszStoragepath);
		nFd = u_File.GetFileHandle();
		if (nFd > 0)
		{
			//Write to storage
			u_File.Seek(lStartAddr, SEEK_SET);
			HighToLow(pData, lSize);				//James add 2009/10/19 と 01:00:02
			nResult = u_File.Write(pData, lSize);
		
			u_File.Close();
			nFd = -1;
		}
		else
		{
			SendMsg(MSG_SH_DSP54UPDATE_NOWRITE, 0, 0, NULL);
			return	-1;
		}
	}
	return nResult;
}

int	CtmDsp54Update::SendDatatoStorage(BYTE* pData, long lStartAddr, long lSize, tmStorageType StorageType)
{
	int nResult 	= 0,	nStorageFlag = 0;
	int	nFd			= 0;
	CtmFile			u_File;
	char cszFileName[256], cszStoragepath[256];
	
	memset(cszFileName, 0, sizeof(cszFileName));
	memset(cszStoragepath,0,sizeof(cszStoragepath));
	nStorageFlag = TOStorageType(1);
	if(1 == nStorageFlag)
	{
		#ifdef D_PXA270
		strcpy(cszStoragepath, FILE_USBPATH_UPDATEFILE);
		#else
		strcpy(cszStoragepath, "/mnt/mmc/updatefile/");
		#endif
		//Get Download FileName  YYYY-DDDD-TIME
		//sprintf(cszFileName, "%04x-%04x-%04x", m_wSendFileName[0], m_wSendFileName[1], m_wSendFileName[2]);
		//printf("SendFileName=%s\n",m_cszSendFileName);
		sprintf(cszStoragepath, "%s%s%s", cszStoragepath, m_cszSendFileName,m_cszSuffixName);
		//printf("cszStoragepath=%s\n",cszStoragepath);
		u_File.Open(cszStoragepath	, O_RDWR | O_SYNC);
		//g_tmDebugInfo->PrintDebugInfo("cszFileName=%s cszStoragepath=%s\n", cszFileName, cszStoragepath);
		nFd = u_File.GetFileHandle();
		if (nFd > 0)
		{
			if(m_SendFileInfo.lFileLength <= 0)
			{  
				//Get download file length
				m_SendFileInfo.lFileLength 	= u_File.Seek(0, SEEK_END);
				//set download package cout
				if(m_SendFileInfo.lPackageSize != 0)
				{
					if(m_SendFileInfo.lFileLength % m_SendFileInfo.lPackageSize)
						m_SendFileInfo.lPackageCount= m_SendFileInfo.lFileLength / m_SendFileInfo.lPackageSize +1 ;
					else
						m_SendFileInfo.lPackageCount= m_SendFileInfo.lFileLength / m_SendFileInfo.lPackageSize;
				}
			}
			//Read data
			//g_tmDebugInfo->PrintDebugInfo("lStartAddr=%x \n", lStartAddr);
			u_File.Seek(lStartAddr, SEEK_SET);
			nResult = u_File.Read(pData, lSize);
			
			if(m_nMaxPackSize == COMM_UPDATE_PACKSIZE_54)
			{
				if(nResult < lSize)			//James add 2013/4/11 16:24:54 set 0xffff the end package
				{
					memset((BYTE*)(pData+nResult), 0xFFFF, lSize-nResult);	
				}
				nResult = lSize;
			}
			
			if(m_nMaxPackSize == COMM_UPDATE_PACKSIZE_54)
				HighToLow(pData, lSize);				//James add 2009/10/19 ¤U¤è 12:59:14
			m_SendFileInfo.lCurrentSize = lSize;//COMM_UPDATE_PACKSIZE_54;//nResult;	//James modify 2013/4/11 16:28:01
			g_tmDebugInfo->PrintDebugInfo("2 nResult=%d m_SendFileInfo.lCurrentNub=%ld \n", nResult, m_SendFileInfo.lCurrentNub);
			
			u_File.Close();
			nFd = -1;
		}
		else
		{
			SendMsg(MSG_SH_DSP54UPDATE_FILENOEXIST, 0, 0, NULL);
			return -1;
		}
	}
	return nResult;
}

int		CtmDsp54Update::TOStorageType(int type)
{
	int	 nStatus = 0;
	switch(type)		//choose storage type
   	{
   		case 1:
   			#ifdef	D_ARM
   			#ifdef	D_PXA270		//James add for pxa270 sd 纔тusb礛тsd 2009/3/19 10:56と
				//掛載USB
				if(!g_wUSBStatus)
				{
					nStatus = CtmUsbFile::GetInstance()->Mount(1);
				}
				else
				{
					nStatus = 1;	
				}
			#else
				if(nStatus < 1)
				{
					if (!g_wMMCStatus)
						nStatus = CtmMMCardFile::GetInstance()->Mount(1);
				}
	   			if(g_wMMCStatus || g_wUSBStatus)
	   				Prompt(g_MultiLanguage["VM_DATA_READING"]);
	   		#endif
			#else
   				if(!g_wUSBStatus)
				{
					nStatus = CtmUsbFile::GetInstance()->Mount(1);
				}
				else
				{
					nStatus = 1;	
				}
			#endif
   			break;
   		default:
   			break;
   	}
   	return nStatus;
}

int		CtmDsp54Update::HighToLow(BYTE* pData, long lSize)
{
	
	if(nUpdateDevice == COMM_UPDATEDEVICE_DSP28 || nUpdateDevice == COMM_UPDATEDEVICE_DSP54TRANSMIT)	//James modify 2010/5/13 for add dsp28 transmit
	{
		int i = lSize;
		return i;	
	}
	else
	{
		WORD	wTempOldValue = 0, wTempNewValue = 0;
		int i = lSize;
		WORD *	pTempdata = (WORD *)pData;
		while (i > 1)
		{
			*pTempdata++ = (*pTempdata) << 8 | (*pTempdata) >> 8;
			i -=2;
		}
	}
}

BOOL	CtmDsp54Update::PushCmd(CMDDATA* pcmd)
{
	if (!m_CmdQueue.bQueueFull)
	{
	    m_CmdQueue.CmdQueue  [m_CmdQueue.wInput]   = pcmd->wCmd;
	    m_CmdQueue.IndexQueue[m_CmdQueue.wInput]   = pcmd->wIndex;
	    m_CmdQueue.ValueQueue[m_CmdQueue.wInput++] = pcmd->wValue;
	    if (m_CmdQueue.wInput >= m_CmdQueue.wQueueEnd)
	        m_CmdQueue.wInput = 0;
	    if (m_CmdQueue.wInput == m_CmdQueue.wOutput)
	        m_CmdQueue.bQueueFull = TRUE;
	    return TRUE;
	}
	
	return FALSE;
}

void	CtmDsp54Update::Update_Run()
{
	if(m_nConnetSate == COMM_STATE_SEND)
	{
		//if(u_lEchoTime <= 0)   
		{
			switch(m_wCommandIndex)
			{
				//case COMM_ETH_SENDDLSTART:
				//	SendDLStart();
				//	break;
				case COMM_ETH_SENDDLDATA:
					SendDLData();
					break;
				case COMM_ETH_SENDDLEND:
					SendDLEnd();
					break;
				case COMM_ETH_SENDDLREADY:
					//SendDLReady();
					break;
				case COMM_ETH_SENDDLSMALL:
					SendDLSmall();
					break;
				default:
					break;
			}
		//m_wCommandIndex=0;
		//	u_lEchoTime = COMM_TIME_ECHO;
		}
	}
}

BOOL	CtmDsp54Update::PopCmd(CMDDATA* pcmd)
{
	if ((m_CmdQueue.wInput != m_CmdQueue.wOutput) || (m_CmdQueue.bQueueFull))
	{
	    pcmd->wCmd   = m_CmdQueue.CmdQueue  [m_CmdQueue.wOutput];
	    pcmd->wIndex = m_CmdQueue.IndexQueue[m_CmdQueue.wOutput];
	    pcmd->wValue = m_CmdQueue.ValueQueue[m_CmdQueue.wOutput++];
	    if (m_CmdQueue.wOutput >= m_CmdQueue.wQueueEnd) m_CmdQueue.wOutput = 0;
	    m_CmdQueue.bQueueFull = FALSE;
	    return TRUE;
	}
	
	return FALSE;
}

void CtmDsp54Update::SendExit()
{
	int nWriteCount = 0;
	g_frameTx.wCmd 	= COMM_ETH_SENDDLEXIT;
	m_wSetCmdState	= COMM_ETH_SENDDLEXIT;
	m_wCmdState		= CMD_SEND;
	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);
}

void 	CtmDsp54Update::TransferToDsp28()
{
	int nWriteCount = 0;
	g_frameTx.wCmd 	= COMM_ETH_MODE_TRANSFORM;
	m_wSetCmdState	= COMM_ETH_MODE_TRANSFORM;
	m_wCmdState		= CMD_SEND;
	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);	
}


void	CtmDsp54Update::TurnToNormal()
{
	int nWriteCount = 0;
	g_frameTx.wCmd 	= COMM_ETH_MODE_NORMAL;
	m_wSetCmdState	= COMM_ETH_MODE_NORMAL;
	m_wCmdState		= CMD_SEND;
	WriteToken((BYTE*)&g_frameTx, (2), &nWriteCount);	
}

#ifndef	D_BEOBJECT_CTMDSP54UPDATE
CObject*	CreateInstance(char* Name)
{
	static		CObject*	pResult = NULL;		//daniel add 2010/01/29 ňゎCreate 2Ω
	if(pResult == NULL)
	{	
		pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CtmPackClass*>(pResult))->CreateSelf();
	}
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmPackClass*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
