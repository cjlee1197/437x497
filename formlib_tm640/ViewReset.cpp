/*===========================================================================+
|  Class    : ViewReset library                                              |
|  Task     : ViewReset library  action source file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include    "ViewReset.h"
//#include	"../control.h"
#include	"../_kb.h"
#include    "../optionbox.h"
#include    "../utils.h"
#include	"../sram.h"
#include	"../_kb.h"
#include	"../sram.h"
#include 	"../qualitycontrol.h"
#include	"../freedata.h"
#include	"../iotable.h"
#include	"../dbid2.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     BUTTON_NAME_CONFIRM	   "buttonCONFIRM"             // 確認
#define     BUTTON_NAME_CANCEL	   "buttonCANCEL"              // 取消

#define		RESETALL_KEY			0x01

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*		pwndCheckRESETDSP		   	= NULL;
CtmWnd*		pwndCheckRESETALL		   	= NULL; 

CtmWnd*		pwndCheckZero				= NULL;
CtmWnd*		pwndCheckMoldset			= NULL;
CtmWnd*		pwndCheckMachine			= NULL;
CtmWnd*		pwndCheckUser				= NULL;

CtmWnd*		pwndDefaultPassWord			= NULL;

CtmWnd*		pwndBtnCONFIRM 		   		= NULL;
CtmWnd*		pwndBtnCANCEL 		   		= NULL;

int			m_iKey 		   	= 0;
BOOL        b_RESET =FALSE;
BOOL        bChecked =FALSE;
BOOL	    b_RstFalg=FALSE;

///*ZHBA 2013-9-13 */M3
char*			pszUserID[] = {		//for iNet, 2010-11-15  
	"USER_HMI_NULL_NULL_USER1ID",
	"USER_HMI_NULL_NULL_USER2ID",
	"USER_HMI_NULL_NULL_USER3ID",
	"USER_HMI_NULL_NULL_USER4ID"
};

char*			pszPasswdID[] = {
	"USER_HMI_NULL_NULL_USER1PASSWORD",
	"USER_HMI_NULL_NULL_USER2PASSWORD",
	"USER_HMI_NULL_NULL_USER3PASSWORD",
	"USER_HMI_NULL_NULL_USER4PASSWORD"
};

char*			pszPrivilegeID[] = {
	"USER_HMI_NULL_NULL_USER1PRIVILEGE",
	"USER_HMI_NULL_NULL_USER2PRIVILEGE",
	"USER_HMI_NULL_NULL_USER3PRIVILEGE",
	"USER_HMI_NULL_NULL_USER4PRIVILEGE"
};

char*			pszUserValue[4] = {
	"0001",
	"0002",
	"0003",
	"0004"
};

int			pszPasswdValue[4] = {
	714,
	718,
	716,
	1291
};

int			pszPrivilegeValue[4] = {
	0,
	1,
	5,
	8
};
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	g_iPrivilege=8;
	
	pwndCheckRESETDSP		= pwndSender->FindControlFromName("CheckBoxRESET_DSP");
	pwndCheckRESETALL 		= pwndSender->FindControlFromName("CheckBoxRESET_ALL");

	pwndBtnCONFIRM			= pwndSender->FindControlFromName(BUTTON_NAME_CONFIRM);
	pwndBtnCANCEL			= pwndSender->FindControlFromName(BUTTON_NAME_CANCEL);

	pwndCheckMoldset		= pwndSender->FindControlFromName("CheckBoxRESET_MOLD");
	pwndCheckMachine		= pwndSender->FindControlFromName("CheckBoxRESET_MACHINE");
	pwndCheckZero			= pwndSender->FindControlFromName("CheckBoxRESET_POSN");
	pwndCheckUser			= pwndSender->FindControlFromName("CheckBoxRESET_USER");

	pwndDefaultPassWord		= pwndSender->FindControlFromName("editDEFAULTPASSWORD");
	
	return TRUE;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//printf("m_iKey%d\n",m_iKey);
	switch(wKey)
	{
//		case 1:
//			::PutCommand("Syst.txt");
//			break;
//		case 3:
//			::PutCommand("Vers_IML.txt");
//			break;			
//		case 99:
//			//::PutCommand("Func.txt");
//			::PutCommand("Index.txt"); // cjlee 2019/4/6 下午 05:10:43
//			break;
		case 70: // Home
			::PutCommand("Over.txt"); // cjlee 2019/4/17 下午 06:56:41
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
		default:
			break;
	}
	m_iKey = wKey;
	if (wKey == _CTRL_C) 	//在此畫面按取消鍵,則取消對所有重置內容的選擇
	{
		BOOL 	bTemp = FALSE;
		if(pwndCheckRESETALL 	!= NULL)
		{
			pwndCheckRESETALL->SetPropValueT("checked", bTemp);
			pwndCheckRESETALL->Update();
			//printf("pwndCheckRESETALL\n");
		}
		if(pwndCheckMoldset	!= NULL)
		{
			pwndCheckMoldset->SetPropValueT("checked", bTemp);
			pwndCheckMoldset->SetPropValueT("enabled", (double)TRUE);
			pwndCheckMoldset->Update();
		}
		if(pwndCheckMachine != NULL)
		{
			pwndCheckMachine->SetPropValueT("checked", bTemp);
			pwndCheckMachine->SetPropValueT("enabled", (double)TRUE);
			pwndCheckMachine->Update();
		}
		if(pwndCheckZero != NULL)
		{
			pwndCheckZero->SetPropValueT("checked", bTemp);
			pwndCheckZero->SetPropValueT("enabled", (double)TRUE);
			pwndCheckZero->Update();
		}
		if(pwndCheckUser 	!= NULL)
		{
			pwndCheckUser->SetPropValueT("checked", bTemp);
			pwndCheckUser->SetPropValueT("enabled", (double)TRUE);
			pwndCheckUser->Update();
		}
		if(pwndCheckRESETDSP 	!= NULL)
		{
			pwndCheckRESETDSP->SetPropValueT("checked", bTemp);
		  	pwndCheckRESETDSP->SetPropValueT("enabled", (double)TRUE);
			pwndCheckRESETDSP->Update();
		}
	
	   	if(b_RESET)
	   		b_RESET=FALSE;
	}
	else if (wKey == RESETALL_KEY)
	{
		if(pwndCheckRESETALL != NULL)
		{   
			if(bChecked)
				bChecked = FALSE;
			else
				bChecked = TRUE;
			 	
			pwndCheckRESETALL->SetPropValueT("checked", bChecked);
			pwndCheckRESETALL->Update();
			
	  		if (bChecked == TRUE)
			{
    			SetChecked(pwndCheckMoldset,bChecked);
    			SetChecked(pwndCheckMachine,bChecked);
    			SetChecked(pwndCheckZero,bChecked);
    			SetChecked(pwndCheckUser,bChecked);
			}
			BOOL 	bEnable = FALSE;
			if(bChecked)
				bEnable=FALSE;
			else
				bEnable=TRUE;
    		SetEnabled(pwndCheckMoldset,bEnable);
    		SetEnabled(pwndCheckMachine,bEnable);
    		SetEnabled(pwndCheckZero,bEnable);
    		SetEnabled(pwndCheckUser,bEnable);
		}
	}
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwnd = NULL;
	BOOL	bInitialDSP = FALSE;
	BOOL	bInitialMMI = FALSE;
	BOOL 	bTemp = FALSE;
	BOOL 	bEnable = FALSE;
	pwnd = pwndSender->FindControlFromTab(wIDControl);
    if(pwnd == NULL)  return wIDControl;
    
	if(pwnd == pwndCheckRESETALL) //JOYCE2010-8-10 
	{    	
		pwndCheckRESETALL->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	  	if (bTemp == TRUE)
		{
			//BOOL 	bTemp = TRUE; 
    		SetChecked(pwndCheckMoldset,bTemp);
    		SetChecked(pwndCheckMachine,bTemp);
    		SetChecked(pwndCheckZero,bTemp);
    		SetChecked(pwndCheckUser,bTemp);
		}
		
		if(bTemp)
			bEnable=FALSE;
		else
			bEnable=TRUE;
    	SetEnabled(pwndCheckMoldset,bEnable);
    	SetEnabled(pwndCheckMachine,bEnable);
    	SetEnabled(pwndCheckZero,bEnable);
    	SetEnabled(pwndCheckUser,bEnable);
	}
	
	if(pwnd == pwndBtnCONFIRM)
	{
		Prompt(g_MultiLanguage["MSG_DUMMY"],0); //20160304,去掉之前元件的提示字串
		SaveShotCount(); //2012-8-28  累積運行模數
		/*SaveDefaultUser();*/
		SaveStringData(); /*ZHBA 2015/9/18 Replace SaveDefaultUser()*/ 
		SaveFuncConf();  //20150611， 配置畫面的設定
		if(CtmConfig::GetInstance()->GetAmmeterFlag())//JOYCE2011-5-13  for ammeter
		    SaveEnergyData();
		if(g_pExTable->GetTableValue(0,"D_2PLATEN"))	//2015-4-8
		    Save2PlatenPara();
		
		if(pwndCheckRESETDSP != NULL)
			  pwndCheckRESETDSP->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	  	if ((bTemp == TRUE) && (m_iKey == _ENTER))
	  	{
	  		b_RstFalg = TRUE;
		    bInitialDSP = TRUE;
		    ResetDSP();
		    Delay(2000); //2 second
		    //MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);//Andy 20071008
	  	}
	  	
	  	if(pwndCheckRESETALL != NULL)
	    	pwndCheckRESETALL->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	  	if((bTemp ==TRUE) && (m_iKey == _ENTER))
	  	{
	  		b_RstFalg = TRUE;
	    	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); //Andy 20071008
	    	Reset_DB(RESET_DB_ALL);
			CtmConfig::GetInstance()->SetSramInit(0);	//	fans add 2007/9/24 10:18上午
	    	
	    	#ifdef D_NOWRITE_FALSH
	    	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   			if(GetFileType(CHECKFILETYPE))
    			nStorageType = STORAGE_TYPE_FLASH;
	    	CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MOLDSET_SRAMFILE, nStorageType);
	    	usleep(50*1000);
	    	system("rm -rf /usr/Data/moldset/*.cdb");
	    	#else	
	    	CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MOLDSET_SRAMFILE);
	    	#endif
	    	bInitialMMI = TRUE;
	    	SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION", TRUE, TRUE);
	  	}
		if(pwndCheckZero != NULL)
		{
			pwndCheckZero->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	    	if((bTemp ==TRUE) && (m_iKey == _ENTER))	
	    	{
	  			b_RstFalg = TRUE;
	    	  	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); //Andy 20071008
	    		//Reset_DB(RESET_DB_DA);
	    		char sz[256];
	    		for(int k=0;k<16;k++) //Andy 20090805
	    		{
	    			sprintf(sz,"MACH_AD1_WRITE_CHL%d_HOMEPOSITIONOFFSET",k+1);
	    			SetDBValue(sz,0);
	    		}
	    		if(g_ptaskdsp != NULL)
					g_ptaskdsp->WriteValue(COMM_SENDZEROSET);	//2008-9-28
	    		bInitialMMI = TRUE;
	    	}			
		}	

		if(pwndCheckMoldset != NULL) 
		{
			pwndCheckMoldset->GetPropValueT("checked", &bTemp,sizeof(bTemp));
			if((bTemp ==TRUE) && (m_iKey == _ENTER))	
	    	{
	  			b_RstFalg = TRUE;
	    	  	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); //Andy 20071008
	    		Reset_DB(RESET_DB_ALL);
	    		CtmConfig::GetInstance()->SetSramInit(0);

		    	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   				if(GetFileType(CHECKFILETYPE))
    				nStorageType = STORAGE_TYPE_FLASH;
		    	CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MOLDSET_SRAMFILE, nStorageType);
		    	usleep(50*1000);
	    		system("rm -rf /usr/Data/moldset/*.cdb");
		    	SetDBValue("SYSX_HMI_NULL_NULL_SYSTEMRESETOPTION", TRUE, TRUE);
	    		bInitialMMI = TRUE;
	    	}
		}
		if(pwndCheckMachine != NULL)  //user date重置成 出廠時保存的值
		{
			pwndCheckMachine->GetPropValueT("checked", &bTemp,sizeof(bTemp));
			if((bTemp ==TRUE) && (m_iKey == _ENTER))	
	    	{
	  			b_RstFalg = TRUE;
	    	  	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); //Andy 20071008
	    		tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   				if(GetFileType(CHECKFILETYPE))
    				nStorageType = STORAGE_TYPE_FLASH;
		    	
   			    //Prompt(g_MultiLanguage["MSG_RESET_NOFILE"]);
    			usleep(50*1000);
    			system("rm -rf /usr/Data/machine/machine.cdb");
	    		usleep(50*1000);
    			system("rm -rf /usr/Data/machine/machine_mach.cdb");
	    		//usleep(50*1000);
    			//system("rm -rf /usr/Data/DA/da.cdb");
	    		//usleep(50*1000);
    			//system("rm -rf /usr/Data/DA/da_mach.cdb");
	    		usleep(50*1000);
    			system("rm -rf /usr/Data/systemx/systemx.cdb");
	    		usleep(50*1000);
    			system("rm -rf /usr/Data/systemx/systemx_mach.cdb");
    			//if(1 == CtmConfig::GetInstance()->GetAustoneFlag())		//新Austone通訊
    			//{
    			//	usleep(50*1000);
    			//	system("rm -rf /usr/Data/Aus_Driver_Para.cdb");		//James add 2010/8/3 for austone
    			//}
    			Reset_DB(RESET_DB_MACHINE);
//		    		Reset_DB(RESET_DB_MACHINEB);
    			if(g_pFreeData != NULL)
    				g_pFreeData->GetDefaultDataFromTable("./lib/hmi_group/SystemID");
    			bInitialMMI = TRUE;
	    	}
		}
		if(pwndCheckUser != NULL) //JOYCE2009-4-15 add //user date重置成 出廠時保存的值
		{
			pwndCheckUser->GetPropValueT("checked", &bTemp,sizeof(bTemp));
			if((bTemp ==TRUE) && (m_iKey == _ENTER))	
	    	{
	  			b_RstFalg = TRUE;
	    	  	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); //Andy 20071008
	    		tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   				if(GetFileType(CHECKFILETYPE))
    				nStorageType = STORAGE_TYPE_FLASH;

		   		//Prompt(g_MultiLanguage["MSG_RESET_NOFILE"]);
	    		usleep(50*1000);
	    		system("rm -rf /usr/Data/user/userm.cdb");
	    		usleep(50*1000);
    			system("rm -rf /usr/Data/user/userm_mach.cdb");
    			if(g_pFreeData != NULL)	
    				g_pFreeData->GetDefaultDataFromTable("./lib/hmi_group/UserID");
    			bInitialMMI = TRUE;
	    	}
		}
		
		if(b_RstFalg)
		{
			system("sync");   //20160324
			usleep(1000*1000);
			system("sync");
			usleep(1000*1000);
			system("sync");
			usleep(1000*1000);
			b_RstFalg = FALSE;
		}

		if(bInitialMMI == TRUE)	ResetMMI();
			
		if(bInitialMMI || bInitialDSP)
		{
			Prompt(g_MultiLanguage["MSG_DUMMY"],0);
			MsgBox(g_MultiLanguage["MSG_RESET_RESTART"],3);//Andy 20071008 
		}
			
	}
	else if (pwnd == pwndBtnCANCEL)
	{
		if (m_iKey == _ENTER)
		{
			BOOL 	bTemp = FALSE;
			if(pwndCheckRESETALL 	!= NULL)
			{
				pwndCheckRESETALL->SetPropValueT("checked", bTemp);
				pwndCheckRESETALL->Update();
			}
			if(pwndCheckMoldset != NULL)
			{
				pwndCheckMoldset->SetPropValueT("checked", bTemp);
				pwndCheckMoldset->SetPropValueT("enabled", (double)TRUE);
				pwndCheckMoldset->Update();
			}
			if(pwndCheckMachine != NULL)
			{
				pwndCheckMachine->SetPropValueT("checked", bTemp);
				pwndCheckMachine->SetPropValueT("enabled", (double)TRUE);
				pwndCheckMachine->Update();
			}
			if(pwndCheckZero!= NULL)
			{
				pwndCheckZero->SetPropValueT("checked", bTemp);
				pwndCheckZero->SetPropValueT("enabled", (double)TRUE);
				pwndCheckZero->Update();
			}
			if(pwndCheckUser 	!= NULL)
			{
				pwndCheckUser->SetPropValueT("checked", bTemp);
				pwndCheckUser->SetPropValueT("enabled", (double)TRUE);
				pwndCheckUser->Update();
			}
			if(pwndCheckRESETDSP 	!= NULL)
			{
				pwndCheckRESETDSP->SetPropValueT("checked", bTemp);
				pwndCheckRESETDSP->SetPropValueT("enabled", (double)TRUE);
				pwndCheckRESETDSP->Update();
			}
		}			
	}
    return wIDControl;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwnd = NULL;
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)  return wIDControl;
	    
		
	return wIDControl;	
}

void	OnUpdateA(CtmWnd* pwndSender)
{

}

void    ResetMMI()
{

}

void    ResetDSP()
{
	BOOL 	bTemp = TRUE;
	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1);
	SetDBValue("CONF_CODE_WRITE_NULL_DATA14",0);
	SetDBValue("CONF_CODE_WRITE_NULL_DATA7",1);
	if(g_ptaskdsp != NULL)
		g_ptaskdsp->WriteValue(COMM_SENDCONFIG);	//2008-9-28
	
	//DA 
	CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_NEWPOWERMATCH_ID);
	CtmBlockControl::GetInstance()->Read(STORAGE_TYPE_FLASH);
	
	if(pwndCheckZero != NULL)
		pwndCheckZero->GetPropValueT("checked", &bTemp,sizeof(bTemp));
	if(!bTemp)
	{
		CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_ZSET_ID);
		CtmBlockControl::GetInstance()->Read(STORAGE_TYPE_FLASH);
	}
}

void		SaveShotCount() //2012-8-28 累積運行模數
{
	WORD   wShotCount[2];
	char*   pDataID[] = {
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED25",
            NULL
            };
	
   memset(wShotCount, 0, sizeof(wShotCount));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/shotcount");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/shotcount.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT| O_SYNC);
    
    if (nFd >= 0)
		{
		for(int i=0; i<1 ;i++)
			{
			if(pDataID[i] != NULL)	wShotCount[i] = GetDBValue(pDataID[i]).lValue;
			}
		}
	write(nFd, &wShotCount, sizeof(wShotCount));
	close(nFd);
}

void		SaveActTime()	//2012-12-26 
{
	DWORD 	dwTimeData[4]={0};
    char	szPath[256];
	char	m_szCopyFile[256];
	
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/usr/Data");
	ExistDir(szPath);	
	sprintf(m_szCopyFile, "%s/Acttime.cdb", szPath);
	      	
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT| O_SYNC);
	if (nFd >= 0)
	{
		dwTimeData[0] =GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED21").lValue;
		dwTimeData[1] =GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED22").lValue;
		dwTimeData[2] =GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED23").lValue;
		dwTimeData[3] =GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED24").lValue;
	       
		write(nFd, &dwTimeData, sizeof(dwTimeData));
		close(nFd);
	}
}

/*void		SaveDefaultUser()	//2012-12-26 */
void	SaveStringData() /*ZHBA 2015/9/18 Replace SaveDefaultUser()*/ 
{
	char*   pDataID[] = {
						"SYS_HMI_NULL_NULL_USERDEFAULTID",
						"SYSX_OTHERS_OTHERS_STRING_RESERVED2",
            			NULL
            };
	char	m_pszNameID[2][128];
    char	szPath[256];
	char	m_szCopyFile[256];
	
	memset(m_pszNameID, 0, sizeof(m_pszNameID));
	memset(szPath, 		0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");
	ExistDir(szPath);	
	sprintf(m_szCopyFile, "%s/StringData.cdb", szPath);
	
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT| O_SYNC);
	
	if (nFd >= 0)
	{
		GetDBString(pDataID[0], m_pszNameID[0], 9);
		GetDBString(pDataID[1], m_pszNameID[1], 5);
		write(nFd, &m_pszNameID, sizeof(m_pszNameID));
		close(nFd);
	}
}

void		SaveFuncConf()	//20150611，配置畫面的設定
{
	if(GetDBValue("M3_CONF_TEMPERCARD1_NODE_ID").lValue !=11)  //20160301, 溫度卡1節點
		return;

	WORD   wFuncConf[28];
	char*   pDataID[] = {
						"M3_CONF_TEMPERCARD1_NODE_ID",
						"M3_CONF_TEMPERCARD2_NODE_ID",
						"M3_CONF_TEMPERCARD3_NODE_ID",
						"M3_CONF_TEMPERCARD4_NODE_ID",
						"M3_CONF_TEMPERCARD5_NODE_ID",
						"M3_CONF_TEMPERCARD6_NODE_ID",
						"M3_CONF_TEMPERCARD7_NODE_ID",
						"M3_CONF_TEMPERCARD8_NODE_ID",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED10",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED11",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED12",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED13",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED14",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED15",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED16",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED17",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED18",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED19",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED20",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED21",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED22",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED23",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED24",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED25",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED26",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED27",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED28",
						"MCONF_OTHERS_OTHERS_NULL_RESERVED29",
            			NULL
            			};
	
    memset(wFuncConf, 0, sizeof(wFuncConf));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/FuncConf.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT| O_SYNC);
    
    if (nFd >= 0)
	{
		for(int i=0; i<28 ;i++)
		{
			if(pDataID[i] != NULL)	wFuncConf[i] = GetDBValue(pDataID[i]).lValue;
		}
	}
	
	write(nFd, &wFuncConf, sizeof(wFuncConf));
	close(nFd);
}

void		SaveEnergyData() //2012-3-21  
{
	DWORD  	dwEnergyData[15];

	char*   pDataID[] = {
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED11",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED12",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED13",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED14",
						"SYSX_OTHERS_OTHERS_DWORD_RESERVED15",
						"SYSX_OTHERS_OTHERS_INT_RESERVED13",
						"SYSX_OTHERS_OTHERS_INT_RESERVED14",
						"SYSX_OTHERS_OTHERS_INT_RESERVED15",
						"SYSX_OTHERS_OTHERS_INT_RESERVED16",
						"SYSX_OTHERS_OTHERS_INT_RESERVED17",
						"SYSX_OTHERS_OTHERS_INT_RESERVED18",
						"SYSX_OTHERS_OTHERS_INT_RESERVED19",
						"SYSX_OTHERS_OTHERS_INT_RESERVED20",
						"SYSX_OTHERS_OTHERS_INT_RESERVED21",
						"SYSX_OTHERS_OTHERS_INT_RESERVED22",
            NULL
            };
	
   memset(dwEnergyData, 0, sizeof(dwEnergyData));

    char	szPath[256];
    char	m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf/energy");
    ExistDir(szPath);	
    sprintf(m_szCopyFile, "%s/energy.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT| O_SYNC);
    
    if (nFd >= 0)
		{
		for(int i=0; i<15 ;i++)
			{
			if(pDataID[i] != NULL)	dwEnergyData[i] = GetDBValue(pDataID[i]).lValue;
				//printf("reset dwEnergyData[%d]:%f\n",i,dwEnergyData[i]);
			}
		}
	write(nFd, &dwEnergyData, sizeof(dwEnergyData));
	close(nFd);
}

void    Save2PlatenPara()   //2015-4-8 
{
    WORD  w2PlatenPara[4];
    char*   pDataID[] = {
                    "MACH_OTHR_OTHR_NULL_SPECIALDATA16",
                    "MACH_OTHR_OTHR_NULL_SPECIALDATA19",
                    "MACH_OTHR_OTHR_NULL_SPECIALDATA21",
                    "MSET_ROTA_ROTA_SLOW_PRESSURE",
            NULL
            };
    
   memset(w2PlatenPara, 0, sizeof(w2PlatenPara));

    char    szPath[256];
    char    m_szCopyFile[256];
    
    memset(szPath, 0, sizeof(szPath));
    memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
    sprintf(szPath, "%s", "/conf");
    ExistDir(szPath);
    sprintf(m_szCopyFile, "%s/2platen.cdb", szPath);
    
    int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT| O_SYNC);
    
    if (nFd >= 0)
    {
        for(int i=0; i<4 ;i++)
        {
            if(pDataID[i] != NULL)  w2PlatenPara[i] = GetDBValue(pDataID[i]).lValue;
            //printf("reset w2PlatenPara[%d]:%d\n",i,w2PlatenPara[i]);
        }
    }
    write(nFd, &w2PlatenPara, sizeof(w2PlatenPara));
    close(nFd);
}

void		SaveAS2PLATADJPOSI() 
{	
	WORD 	wAS2PlatAdjPos[1];
   	char	szPath[256];
	char	m_szCopyFile[256];
	char*   pDataID[] = {
                    "MSET_OTHR_OTHR_NULL_MOLDSPECIAL14DATA",
            NULL
            };
	      
	memset(wAS2PlatAdjPos, 0, sizeof(wAS2PlatAdjPos));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf");  
	ExistDir(szPath);	
	sprintf(m_szCopyFile, "%s/AS2PlatAdjPos.cdb", szPath);
	      	      	
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{			
		for(int i=0; i<1 ;i++)
        {
            if(pDataID[i] != NULL)  wAS2PlatAdjPos[i] = GetDBValue(pDataID[i]).lValue;
        }
		write(nFd, &wAS2PlatAdjPos, sizeof(wAS2PlatAdjPos));
		close(nFd);
	}
}

/*===========================================================================+
|           Help Function                                                    |
+===========================================================================*/	
void    SetEnabled( CtmWnd* pwnd,BOOL  bEnabled)	
{
	if(pwnd != NULL)
		{
			pwnd->SetPropValueT("enabled",(double)bEnabled); 		
			pwnd->Update();
		}		
}

void    SetChecked( CtmWnd* pwnd,BOOL  bVisible)
{
	if(pwnd != NULL)
		{
			pwnd->SetPropValueT("checked",(double)bVisible); 		
			pwnd->Update();
		}		
}

void		SaveAutoRunTime()
{	
	long long lOpenTime[2];
	char	szPath[256];
	char	m_szCopyFile[256];
	
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/OpenTime");	      
	ExistDir(szPath);	
	sprintf(m_szCopyFile, "%s/opentime.cdb", szPath);
		      	
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{						
		lOpenTime[0] =(long long)GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED21").lValue;
		lOpenTime[1] =(long long)GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED22").lValue;
		write(nFd, &lOpenTime, sizeof(lOpenTime));
		close(nFd);
	}
}

void		SavePushKeyOilData() /*   2015-7-2  for ALL key oildata */
{
	WORD 	wOilData[2];
   	char	szPath[256];
	char	m_szCopyFile[256];

	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/OilData");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/OilData.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{
		wOilData[0] =(WORD)GetDBValue("SYSX_OTHERS_OTHERS_NULL_VALTON").lValue;
		write(nFd, &wOilData, sizeof(wOilData));
		close(nFd);
	}
}

void		SaveTieBarDate()
{
	int 	TieBarDate[2];
	char	szPath[256];
	char	m_szCopyFile[256];

	memset(TieBarDate, 0, sizeof(TieBarDate));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/conf/TieBardate");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/TieBardate.cdb", szPath);

	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	if (nFd >= 0)
	{
		TieBarDate[0] =(int)GetDBValue("MACH_MDH_MDH_NULL_W002C").lValue;
		TieBarDate[1] =(int)GetDBValue("MACH_MDH_MDH_NULL_W002D").lValue;

		write(nFd, &TieBarDate, sizeof(TieBarDate));
		close(nFd);
	}
}
