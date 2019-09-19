/*===========================================================================+
|  Class    : ViewNetSet library                                             |
|  Task     : ViewNetSet library action source file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Caozz		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include "ViewNetSet.h"
#include "../utils.h"
#include "../commonaction.h"
#include "../language.h"
#include "../selectedit.h"
#include "../main.h"
#include	"../tmcontrol.h"
#include	"../lib/libiNetComm.h"
#include	"../ethnet.h"
#include	"../tmshmsg.h"
#include	"../iotable.h"
#include	"../tmnbmsg.h"
#include 	"../drawtext.h"
#include	"../lib/inet/tmTaskiNet.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     EDIT_MAC        "editMAC_"
#define     EDIT_SERVER_IP  "editServerIP_"

#define		IP_V4_LENGTH	4
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

CtmWnd*		pwndEditIP0		 		= NULL;
CtmWnd*		pwndEditIP1        		= NULL;
CtmWnd*     pwndEditIP2        		= NULL;
CtmWnd*     pwndEditIP3        		= NULL;
CtmWnd*     pwndEditConfirmNet 	    = NULL;
CtmWnd*     pwndEditConfirmMac 	    = NULL;


CtmWnd*		m_pwndEditDHCP			= NULL;
CtmWnd*		m_pwndEditNetCardSelect	= NULL;
CtmWnd*		m_pwndEditLocalIP[IP_V4_LENGTH] 	= {NULL};
CtmWnd*		m_pwndEditNetMask[IP_V4_LENGTH] 	= {NULL};
CtmWnd*		m_pwndEditGetWay[IP_V4_LENGTH] 		= {NULL};
CtmWnd*		m_pwndEditServerIP[IP_V4_LENGTH] 	= {NULL};

CtmWnd* 	pwndLocalMac[6];

int 		languageindex = 0, LogoIndex = 0, toolbarindex	= 0;;
bool  	 	biNetControlsVisible 	= FALSE;
BYTE		m_acPhysicalAddr[6], m_acPhysicalAddr0[6];
BYTE		acRemoteIP[4], acLocalIP[16];

tmDATE      u_OldDate;               // Wain 2007/04/17 Add <<
tmTIME      u_OldTime;               // Wain 2007/04/17 Add >>
BOOL		u_biNet = FALSE;		//	fans add 2009/5/21 03:08下午
WORD  		wEditType1 =0; //txt中元件的屬性, 確定元件顯示/隱藏時是否需要邊框

tmiNetInfo	u_iNetInfo;
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
    int       PartYear =0;
	u_biNet = CtmConfig::GetInstance()->GetINETFlag();
   
    pwndEditIP0     = pwndSender->FindControlFromName("editIP_0");
    pwndEditIP1     = pwndSender->FindControlFromName("editIP_1");
    pwndEditIP2     = pwndSender->FindControlFromName("editIP_2");
    pwndEditIP3     = pwndSender->FindControlFromName("editIP_3");
    pwndEditConfirmNet  = pwndSender->FindControlFromName("editCONFIRMNET");
    pwndEditConfirmMac  = pwndSender->FindControlFromName("editCONFIRMMAC");
    
    
    
    m_pwndEditDHCP  = pwndSender->FindControlFromName("editDHCPUse");
    m_pwndEditNetCardSelect  = pwndSender->FindControlFromName("editSelectMac");

    GetSpecialControlNum(pwndSender, "editLocalIP", "CtmEditX1", m_pwndEditLocalIP);
    GetSpecialControlNum(pwndSender, "editNetMask", "CtmEditX1", m_pwndEditNetMask);
    GetSpecialControlNum(pwndSender, "editGetway", "CtmEditX1", m_pwndEditGetWay);
    
    GetSpecialControlNum(pwndSender, EDIT_SERVER_IP, "CtmEditX1", m_pwndEditServerIP);
    
    
    
	if(pwndEditIP0 != NULL)
		pwndEditIP0->GetPropValueT("wtype", &wEditType1,sizeof(wEditType1));
	
	GetSpecialControlNum(pwndSender, EDIT_MAC, "CtmEditX2", pwndLocalMac);
   	
    CreateSet(pwndSender); 
    SetVisible(pwndEditConfirmMac, FALSE, 0x30);
    
	return TRUE;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	tmDATE     	date;
	tmTIME     	time;
	int      	PartYear =0;
    int 		nTemp = 0;
    CtmWnd*     pwnd      = NULL;
	pwnd	= pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)		return wIDControl;
    
	if((pwnd == m_pwndEditServerIP[0]) ||(pwnd == m_pwndEditServerIP[1]) ||(pwnd == m_pwndEditServerIP[2]) ||(pwnd == m_pwndEditServerIP[3]))
    {
		GetIPAddr();
		
		if(pwndEditConfirmNet != NULL)
		{
			if((acRemoteIP[0] == 0xFF) &&(acRemoteIP[1] == 0) &&(acRemoteIP[2] == 0) &&(acRemoteIP[3] == 0xFF))
			{
				pwndEditConfirmNet->SetPropValueT("index",3);		
				pwndEditConfirmNet->SetPropValueT("maxindex",3);			
			}
			else
			{
				pwndEditConfirmNet->SetPropValueT("index",2);
				pwndEditConfirmNet->SetPropValueT("maxindex",2);
			}
			pwndEditConfirmNet->UpdateAll();
			pwndEditConfirmNet->Update();
		}
  	}	
	
	return wIDControl;
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwnd      = NULL;
    int 		nTemp = 0;
	BOOL		bModifyMac = FALSE;
	BYTE       	Temp0 = 0;
	pwnd	= pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)		return wIDControl;
    
    if(pwndEditConfirmNet == pwnd)//確認iNet重新連線
    {
    	if(pwnd->Is("CtmEditX1"))
    		pwnd->GetPropValueT("value", &nTemp,sizeof(nTemp));
    	else if(pwnd->Is("CtmSelectEdit"))
    		nTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
    	
        long long lConst0 = 0;
        
        if(nTemp ==1 && u_biNet)//表示有iNet功能
        {
            
            for(int i=0;i<4;i++)
			{
				if(m_pwndEditLocalIP[i] !=NULL)
					m_pwndEditLocalIP[i] ->GetPropValueT("value", &u_iNetInfo.acLocalIP[i],sizeof(u_iNetInfo.acLocalIP[i]));
				
				if(m_pwndEditNetMask[i] !=NULL)
					m_pwndEditNetMask[i] ->GetPropValueT("value", &u_iNetInfo.acNetMaskIP[i],sizeof(u_iNetInfo.acNetMaskIP[i]));
				
				
				if(m_pwndEditGetWay[i] !=NULL)
					m_pwndEditGetWay[i] ->GetPropValueT("value", &u_iNetInfo.acGatway[i],sizeof(u_iNetInfo.acGatway[i]));
				
				
				if(m_pwndEditServerIP[i] !=NULL)
					m_pwndEditServerIP[i] ->GetPropValueT("value", &u_iNetInfo.acServerIP[i],sizeof(u_iNetInfo.acServerIP[i]));
			
			}
			if(m_pwndEditDHCP != NULL)
			{
				u_iNetInfo.wDHCPFlag = ((CtmSelectEdit*)m_pwndEditDHCP)->GetIndex();
			}
			
			if(m_pwndEditNetCardSelect != NULL)
			{
				CtmConfig::GetInstance()->SetINETMacSwtich(((CtmSelectEdit*)m_pwndEditNetCardSelect)->GetIndex());
			}
			
			g_ptaskinet->WriteValue(CONST_RES_LOCALNETINFO, (void*)&u_iNetInfo, sizeof(u_iNetInfo));
			
			SendMsg(MSG_SH_INET_RESTART, 0, 0, NULL);		//2009-5-27

    		if(pwnd->Is("CtmEditX1"))
    			pwnd->SetPropValueT("value",lConst0);
    		else if(pwnd->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwnd)->SetIndex(lConst0);
            pwnd->Update();
    	}
    	else if(nTemp ==2 && u_biNet)
    	{
    	    SetVisible(pwndEditConfirmMac, TRUE, wEditType1);
    	    
    	    GetIPAddr();	//fuxy, 2011-1-20 
    	    
			if((acRemoteIP[0] == 0xFF) &&(acRemoteIP[1] == 0) &&(acRemoteIP[2] == 0) &&(acRemoteIP[3] == 0xFF))		//判斷IP地址 
			{
    			for(int i=2; i<6; i++)
	    		{
	    			if(pwndLocalMac[i] != NULL)
	    		    {
	    				pwndLocalMac[i] ->SetPropValueT("enabled", TRUE);
	    				pwndLocalMac[i] ->Update();
	    			}
	    		}
	    	}
	    	
    		if(pwnd->Is("CtmEditX1"))
    			pwnd->SetPropValueT("value",lConst0);
    		else if(pwnd->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwnd)->SetIndex(lConst0);
            pwnd->Update();
    	}	
    }    
    else  if(pwndEditConfirmMac == pwnd)		//fuxy, 2011-1-21，修改Mac地址後，確認修改  
    {
        long long lConst0 = 0;
    	if(pwnd->Is("CtmEditX1"))
    		pwnd->GetPropValueT("value", &nTemp,sizeof(nTemp));
    	else if(pwnd->Is("CtmSelectEdit"))
    		nTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
    	
    	if(nTemp == 1)
    	{
	    	for(int i=0; i<6; i++)		//取修改後的Mac地址 
    		{
    			pwndLocalMac[i] ->GetPropValueT("value", &Temp0, sizeof(Temp0));
    			m_acPhysicalAddr0[i] = Temp0;
    		}
    		
    		for(int i=2; i<6; i++)
    		{
    			//printf("m_acPhysicalAddr0[%d]:%4X \n", i, m_acPhysicalAddr0[i]);
    			if(m_acPhysicalAddr0[i] != m_acPhysicalAddr[i])
    			{
    				bModifyMac = TRUE;		//Mac地址有變化 
    				break;
    			}
    			else
    				bModifyMac = FALSE;		//Mac地址無變化
    		}
    		//printf("bModifyMac:%d \n", bModifyMac);
    		if(bModifyMac)
    		{
    			for(int i=2;i<6;i++)
	    		{
	    			if(pwndLocalMac[i] != NULL)
	    		    {
	    				pwndLocalMac[i] ->SetPropValueT("enabled", (double)FALSE);
	    				pwndLocalMac[i] ->Update();
	    			}
	    		}
    			//SetPhysicalAddr(m_acPhysicalAddr0);		//實際把Mac地址，寫入網卡硬件 
    			MsgBox(g_MultiLanguage["MSG_RESET_RESTART"], tmFT_CODE_TECH);
    		}
    	
    		if(pwnd->Is("CtmEditX1"))
    			pwnd->SetPropValueT("value",lConst0);
    		else if(pwnd->Is("CtmSelectEdit"))
    			((CtmSelectEdit*)pwnd)->SetIndex(lConst0);
            pwnd->Update();
    	}
    }
    else if(m_pwndEditDHCP == pwnd)
    {
    	if(pwnd->Is("CtmEditX1"))
    		pwnd->GetPropValueT("value", &nTemp,sizeof(nTemp));
    	else if(pwnd->Is("CtmSelectEdit"))
    		nTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
    	if((nTemp == 0)&&(g_ptaskinet != NULL))
        {
        	for(int i=0;i<4;i++)
			{
				SetVisible(m_pwndEditLocalIP[i], biNetControlsVisible, wEditType1);
				SetVisible(m_pwndEditNetMask[i], biNetControlsVisible, wEditType1);
				SetVisible(m_pwndEditGetWay[i], biNetControlsVisible, wEditType1);
				
			}
        }
    	
    }


	return wIDControl;
}

void	OnShowA(CtmWnd* pwndSender)
{
	return;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else                              return _NULL_KEY;
}

void  	OnUpdateA(CtmWnd* pwndSender)
{
	
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void        CreateSet(CtmWnd* pwndSender)
{
	if(!u_biNet)
	{
		biNetControlsVisible =FALSE;
		for(int i=0;i<4;i++)
		{
			SetVisible(m_pwndEditServerIP[i], biNetControlsVisible, wEditType1);
			SetVisible(m_pwndEditLocalIP[i], biNetControlsVisible, wEditType1);
			SetVisible(m_pwndEditNetMask[i], biNetControlsVisible, wEditType1);
			SetVisible(m_pwndEditGetWay[i], biNetControlsVisible, wEditType1);
			
		}
	    SetVisible(pwndEditConfirmNet, biNetControlsVisible, wEditType1);
	    SetVisible(m_pwndEditDHCP, biNetControlsVisible, wEditType1);
	    SetVisible(m_pwndEditNetCardSelect, biNetControlsVisible, wEditType1);
	    
	}
	if(g_ptaskinet != NULL)
	{
		g_ptaskinet->ReqValues(CONST_REQ_LOCALNETINFO, (void*)&u_iNetInfo, sizeof(u_iNetInfo));
		if(u_iNetInfo.wDHCPFlag == 0)
	 	{
	 		for(int i=0;i<4;i++)
			{
				if(m_pwndEditLocalIP[i] != NULL)
				{
					m_pwndEditLocalIP[i]->SetPropValueT("wtype",0x31); 		
				  	m_pwndEditLocalIP[i]->Update();
					m_pwndEditLocalIP[i]->SetPropValueT("visible", biNetControlsVisible);
					m_pwndEditLocalIP[i]->Update();
				}
				if(m_pwndEditNetMask[i] != NULL)
				{
					m_pwndEditNetMask[i]->SetPropValueT("wtype",0x31); 		
				  	m_pwndEditNetMask[i]->Update();
					m_pwndEditNetMask[i]->SetPropValueT("visible", biNetControlsVisible);
					m_pwndEditNetMask[i]->Update();
				}
				if(m_pwndEditGetWay[i] != NULL)
				{
					m_pwndEditGetWay[i]->SetPropValueT("wtype",0x31); 		
				  	m_pwndEditGetWay[i]->Update();
					m_pwndEditGetWay[i]->SetPropValueT("visible", biNetControlsVisible);
					m_pwndEditGetWay[i]->Update();
				}
			}
	 	}
	 	SetIPInfo();
	 	
		GetRealPhysicalAddr(m_acPhysicalAddr);		//2009-5-27
		for(int i=0;i<6;i++)
		{	
			if(pwndLocalMac[i] != NULL)
			{
				pwndLocalMac[i] ->SetPropValueT("enabled", (double)FALSE);
				pwndLocalMac[i] ->SetPropValueT("value", m_acPhysicalAddr[i]);	//2011-3-3, for Mac 
				pwndLocalMac[i] ->Update();
			}
		}
	}
}

void		GetIPAddr() 
{
	long long TempValue = 0;
	for(int i=0; i<4; i++)
	{
		if(m_pwndEditServerIP[i] !=NULL)
			m_pwndEditServerIP[i] ->GetPropValueT("value", &TempValue,sizeof(TempValue));
	
		acRemoteIP[i] = (BYTE)TempValue;
	}
}

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType)
{
	if(pwnd != NULL)
	{
        if(wType)
		{
			if(bVisible)
			    pwnd->SetPropValueT("wtype",wType);
			else
				pwnd->SetPropValueT("wtype",wType|0x30); 
		}
		else
		{
			if(bVisible)
				pwnd->SetPropValueT("wtype",0x23);
			else
        		pwnd->SetPropValueT("wtype",0x30);
        }
		pwnd->Update();

		pwnd->SetPropValueT("visible",(double)bVisible); 	
		pwnd->Update();
	}		
}	


void	SetIPInfo()
{
	if(m_pwndEditDHCP != NULL)
	{
		((CtmSelectEdit*)m_pwndEditDHCP)->SetIndex(u_iNetInfo.wDHCPFlag);
		((CtmSelectEdit*)m_pwndEditDHCP)->Update();
	}
	
	if(m_pwndEditNetCardSelect != NULL)
	{
		((CtmSelectEdit*)m_pwndEditNetCardSelect)->SetIndex(CtmConfig::GetInstance()->GetINETMacSwtich());
		((CtmSelectEdit*)m_pwndEditNetCardSelect)->Update();
	}
	
	printf("ip  :: %d.%d.%d.%d\n", u_iNetInfo.acLocalIP[0], u_iNetInfo.acLocalIP[1], 
								   u_iNetInfo.acLocalIP[2], u_iNetInfo.acLocalIP[3]);
	
	printf("netmask  :: %d.%d.%d.%d\n", u_iNetInfo.acNetMaskIP[0], u_iNetInfo.acNetMaskIP[1], 
								   		u_iNetInfo.acNetMaskIP[2], u_iNetInfo.acNetMaskIP[3]);
	
	printf("getway   :: %d.%d.%d.%d\n", u_iNetInfo.acGatway[0], u_iNetInfo.acGatway[1], 
								   		u_iNetInfo.acGatway[2], u_iNetInfo.acGatway[3]);
	
	printf("Server   :: %d.%d.%d.%d\n", u_iNetInfo.acServerIP[0], u_iNetInfo.acServerIP[1], 
								   		u_iNetInfo.acServerIP[2], u_iNetInfo.acGatway[3]);
	
	for(int i=0;i<4;i++)
	{
		if(m_pwndEditLocalIP[i] != NULL)
		{
			printf("m_pwndEditLocalIP  %s, %d\n", m_pwndEditLocalIP[i]->Name, u_iNetInfo.acLocalIP[i]);
			m_pwndEditLocalIP[i]->SetPropValueT("value",u_iNetInfo.acLocalIP[i]); 		
		  	m_pwndEditLocalIP[i]->UpdateAll();
		}
		if(m_pwndEditNetMask[i] != NULL)
		{
			m_pwndEditNetMask[i]->SetPropValueT("value",u_iNetInfo.acNetMaskIP[i]);
		  	m_pwndEditNetMask[i]->UpdateAll();
		}
		if(m_pwndEditGetWay[i] != NULL)
		{
			m_pwndEditGetWay[i]->SetPropValueT("value",u_iNetInfo.acGatway[i]);
		  	m_pwndEditGetWay[i]->UpdateAll();
		}
		if(m_pwndEditServerIP[i] != NULL)
		{
			m_pwndEditServerIP[i]->SetPropValueT("value",u_iNetInfo.acServerIP[i]);
		  	m_pwndEditServerIP[i]->UpdateAll();
		}
	}
	
}