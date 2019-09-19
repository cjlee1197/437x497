/*===========================================================================+
|  Class    : ViewSyst library                                               |
|  Task     : ViewSyst library action source file                            |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include "ViewRemoteSet.h"
#include "../utils.h"
#include "../commonaction.h"
#include "../language.h"
#include "../selectedit.h"
#include "../main.h"
#include "../tmcontrol.h"
#include	"../lib/libiNetComm.h"
#include	"../ethnet.h"
#include	"../tmshmsg.h"
//#include	"../lib/remote/tmTaskRemote.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>  
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     EDIT_MAC        "editMAC_"
#define     EDIT_IP         "editLOCALDST"

#define 	EDITLOCAL		"editLOCAL"				//Caozz 2010/10/18 12:40下午

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*     m_pwndEditConfirmNET 	=NULL;
CtmWnd*     m_pwndEditConfirmMac 	= NULL;
CtmWnd* 	m_pwndControlType 		= NULL;

//CtmWnd*     m_pwndButtonReqView 	= NULL;
//CtmWnd*     m_pwndButtonReqControl 	= NULL;
//CtmWnd* 	m_pwndButtonCancelRemote 		= NULL;

CtmWnd* 	m_pwndLocalMac[6]		= {NULL};
CtmWnd* 	m_pwndRemoteIP[4]		= {NULL};
CtmWnd*    	m_pwndLOCAL[20]			= {NULL};
CtmWnd*		m_pwndeditPASSWORD  		= NULL;

CtmWnd*     m_pwndMACSwitch 	= NULL;
CtmWnd* 	m_pwndRealIP[4]		= {NULL};

BOOL  	 	biNetControlsVisible =FALSE;

BYTE		m_acPhysicalAddr[6], m_acPhysicalAddr0[6];
int		acRemoteIP[4];
BYTE		acRealIP[4];
BYTE		acRealIP_old[4];
char		szEth[20];

int 		nLOCALNum = 0;

int 		u_nRemoteMac = 0;

BOOL		m_bEnterKey = FALSE;
BOOL		m_bUpdateRealIPFlag = FALSE;
	
int ip_4[16]={192,168,8,130,255,255,255,0,192,168,8,1};
int servo_ip[4]={192,168,8,188};

extern		WORD	g_wControl;

BOOL		UpdateRealIPAddr();
char* hostname_to_ip(char * ifaName );
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	
	
	
	m_pwndEditConfirmNET  = pwndSender->FindControlFromName("editCONFIRMNET");
    m_pwndEditConfirmMac  = pwndSender->FindControlFromName("editCONFIRMMAC");
    m_pwndControlType 	  = pwndSender->FindControlFromName("editCONTROLTYPE");
    
    //m_pwndButtonReqView 	  = pwndSender->FindControlFromName("buttonReqView");
    //m_pwndButtonReqControl 	  = pwndSender->FindControlFromName("buttonReqControl");
    //m_pwndButtonCancelRemote 	  = pwndSender->FindControlFromName("buttonCancelRemote");
		m_pwndeditPASSWORD  = pwndSender->FindControlFromName("editPassword");			//密碼
    
    GetSpecialControlNum(pwndSender, EDIT_IP, "CtmEditX1", m_pwndRemoteIP);
    GetSpecialControlNum(pwndSender, EDIT_MAC, "CtmEditX2", m_pwndLocalMac);
   	nLOCALNum = GetSpecialControlNum(pwndSender, EDITLOCAL, "CtmEditX1", m_pwndLOCAL);
	
		m_pwndMACSwitch 	  = pwndSender->FindControlFromName("editMACSwitch");
		if(g_TxCNCData.shmCNCData.iNet_LinkState == 1)
		{
	    GetSpecialControlNum(pwndSender, "editRealIP", "CtmEditX1", m_pwndRealIP);
	   	((CtmSelectEdit*)m_pwndEditConfirmNET)->SetIndex(g_TxCNCData.shmCNCData.iNet_LinkState);
   		
	  	char ipaddr[50]; 
	  	char *ipaddr1;
	  	strcpy(ipaddr,g_TxCNCData.shmCNCData.iNet_Server_IP);
   		servo_ip[0]=atoi(ipaddr);
			ipaddr1=strstr(ipaddr,".")+1;
			servo_ip[1]=atoi(ipaddr1);
			ipaddr1=strstr(ipaddr1,".")+1;
			servo_ip[2]=atoi(ipaddr1);
			ipaddr1=strstr(ipaddr1,".")+1;
			servo_ip[3]=atoi(ipaddr1);
			
	   	for(int i=0; i<4; i++)
	  	{
	    	if(m_pwndRemoteIP[i] !=NULL)
				m_pwndRemoteIP[i]->SetPropValueT("value",servo_ip[i]);
	   		m_pwndRemoteIP[i]->Update();
			}
   	}
   	
   	
		m_pwndEditConfirmNET->Update();
    CreateSet(pwndSender, FALSE); 
    SetIPAddr();
    return TRUE;
} 


WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*         pwnd      = NULL;
	pwnd	= pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL) return wIDControl;
		
	BYTE       		Temp0 = 0;
	BOOL			bModifyMac = FALSE;
	long long       lTemp = 0, lCount0 = 0;
		
	WORD 	wTemp = 0;
    long long TempValue = 0;

    if(m_pwndEditConfirmNET == pwnd)	//確認Net重新連線
    {
    	if(pwnd->Is("CtmEditX1"))
    		pwnd->GetPropValueT("value", &lTemp,sizeof(lTemp));
    	else if(pwnd->Is("CtmSelectEdit"))
    		lTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
      	
      if((lTemp == 1))//表示有Remote功能
				GetIPAddr();
			else
			{
				char temp=0;
				g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.iNet_LinkState,&temp,sizeof(char));
			}
    }
	return wIDControl;
}


WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
    return wIDControl;	
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	WORD 	wTemp = 0;
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
 		
    return wIDControl;	
}

void	OnShowA(CtmWnd* pwndSender)
{
	return;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
	{
		m_bEnterKey = TRUE;
	}
	else
	{
		m_bEnterKey = FALSE;
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else                              return _NULL_KEY;
}
/*void  	OnUpdateA(CtmWnd* pwndSender)
{
	if(m_bUpdateRealIPFlag)	//2012/6/28 下午 03:17:51
	{
		if(UpdateRealIPAddr())	m_bUpdateRealIPFlag = FALSE;
	}
	return;
}*/
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void        CreateSet(CtmWnd* pwndSender, BOOL bEnable)
{
		int sock_get_ip;  
	  char ipaddr[50]; 
	  char *ipaddr1;
		char	szTemp[50]="\0";
		struct   sockaddr_in *sin;  
	  struct   ifreq ifr_ip;     
	  if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) != -1)  
	  {   
		  memset(&ifr_ip, 0, sizeof(ifr_ip));     
		  strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);
		  if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) >= 0 )
		  {
			  sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
			  strcpy(ipaddr,inet_ntoa(sin->sin_addr));
			  //printf("ipaddr = %s\n",ipaddr);
			}
		}
		close(sock_get_ip);
	ip_4[0]=atoi(ipaddr);
	ipaddr1=strstr(ipaddr,".")+1;
	ip_4[1]=atoi(ipaddr1);
	ipaddr1=strstr(ipaddr1,".")+1;
	ip_4[2]=atoi(ipaddr1);
	ipaddr1=strstr(ipaddr1,".")+1;
	ip_4[3]=atoi(ipaddr1);
	
	
	//printf("ip_4[0] = %d\n",ip_4[0]);
	//printf("ip_4[0] = %d\n",ip_4[1]);
	//printf("ip_4[0] = %d\n",ip_4[2]);
	//printf("ip_4[0] = %d\n",ip_4[3]);
	for(int i = 0; i < nLOCALNum; i++) 
  {
   	//printf("localIP = %d\n",localIP[i]);
   	m_pwndLOCAL[i]->SetPropValueT("value", ip_4[i]);
   	m_pwndLOCAL[i]->Update();
  }
    
	
    u_nRemoteMac = CtmConfig::GetInstance()->GetRemoteMacSwtich();
	
	//2012/6/20 上午 09:39:50 Test<<<<< ,so give a const mac in any case
	if(CtmConfig::GetInstance()->GetINETFlag() == 0)
		sprintf(szEth, "eth%d", u_nRemoteMac);		
	else if(CtmConfig::GetInstance()->GetINETMacSwtich() != u_nRemoteMac)
		sprintf(szEth, "eth%d", u_nRemoteMac);	
	else 
		sprintf(szEth, "eth%d:0", u_nRemoteMac);
	//2012/6/20 上午 09:39:50 Test>>>>>>
	
	//printf("szEth=%s \n",szEth);
	GetRealPhysicalAddr(m_acPhysicalAddr, szEth);		//2009-5-27
	for(int i=0;i<6;i++)
	{	
		if(m_pwndLocalMac[i] != NULL)
		{
			//printf("i=%d m_acPhysicalAddr=%d \n",i,m_acPhysicalAddr[i]);
			m_pwndLocalMac[i] ->SetPropValueT("value", m_acPhysicalAddr[i]);	//2011-3-3, for Mac 
			m_pwndLocalMac[i] ->Update();
		}
	}
}

void		GetIPAddr()		//fuxy, 2011-1-20, add 
{
		long long TempValue = 0;
    	for(int i=0; i<4; i++)
    	{
    		if(m_pwndRemoteIP[i] !=NULL)
				m_pwndRemoteIP[i] ->GetPropValueT("value", &TempValue,sizeof(TempValue));
		
			acRemoteIP[i] = (BYTE)TempValue;
		//printf("acRemoteIP[i]=%d  \n",acRemoteIP[i]);
		}
		char server_IP_tmp[17];
		memset(server_IP_tmp,0,sizeof(server_IP_tmp));
		sprintf(server_IP_tmp,"%d.%d.%d.%d",acRemoteIP[0],acRemoteIP[1],acRemoteIP[2],acRemoteIP[3]);
		//printf("server_IP_tmp=%s  \n",server_IP_tmp);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.iNet_Server_IP,server_IP_tmp,sizeof(server_IP_tmp));
		//printf("g_TxCNCData.shmCNCData.iNet_Server_IP=%s  \n",g_TxCNCData.shmCNCData.iNet_Server_IP);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.iNet_Server_IP,server_IP_tmp,sizeof(server_IP_tmp));
		char temp=1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.iNet_LinkState,&temp,sizeof(char));

		//printf("acRemoteIP[i]=%d  \n",acRemoteIP[i]);
}

void		SetIPAddr()
{
  for(int i=0; i<4; i++)
  	{
    	if(m_pwndRemoteIP[i] !=NULL)
			m_pwndRemoteIP[i]->SetPropValueT("value",servo_ip[i]);
   		m_pwndRemoteIP[i]->Update();
		}
		
}
