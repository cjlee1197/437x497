/*===========================================================================+
|  Class    : ViewVers library                                               |
|  Task     : ViewVers library action source file                            |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)2.3                                                     |
|  Link     :G++(GCC)2.3                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                       		 |
|  Version  : V1.00                                                          |
|  Creation : 			                                                   		 	 |
|  Revision :           			                                       		 		 |
+===========================================================================*/
#include	"ViewVers.h"
#include	"../commonaction.h"
#include	"../wnd.h"
#include	"../tmcontrol.h"
#include	"../_kb.h"
#include	"../utils.h"
#include	"../main.h"
#include	"../font.h"
#include	"../lib/libComm.h"
#include	"../iotable.h"

//  add for debug 9/6/2007 1:52PM
#include    "../tmdebug.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		EncType_Abs			0x00000000  // 絕對
#define		EncType_Res		  0x00010000  // 增量
#define		EncWord 				0xFFFF0000 // High Word
#define		DriveImgPath_Austone	"res_tm640/pic/Drive_Austone.bmp"
#define		DriveImgPath_HDT			"res_tm640/pic/Drive_HDT.bmp"

const		int MAXCONTROLS =8;

#define		NAME_EDIT01E0	  	"edit01E0_"
#define		NAME_EDIT01F0	  	"edit01F0_"
#define		NAME_EDIT0200  		"edit0200_"
#define		NAME_EDIT0220	  	"edit0220_"
#define		NAME_EDIT0230	  	"edit0230_"
#define		NAME_EDIT0240	  	"edit0240_"
#define		NAME_EDIT02F0	  	"edit02F0_"
#define		NAME_EDIT0600	  	"edit0600_"

#define		REQ_READMOTOR    	 2   
#define		DB_TP						1 // 選擇示教器參數資料
#define		DB_CON					2 // 選擇控制器參數資料
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次
/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_EncType 		= 0; // 編碼器選擇  0-絕對 1-增量
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸

//short   	FlowStep;
//BOOL    	SendFlag = FALSE;
long long 	MaxCount = 0;

int			iNumedit01E0 = 0;
int			iNumedit01F0 = 0;
int			iNumedit0200 = 0;
int			iNumedit0220 = 0;
int			iNumedit0230 = 0;
int			iNumedit0240 = 0;
int			iNumedit02F0 = 0;
int			iNumedit0600 = 0;

int 		iComIndex = -1;


CtmWnd*   m_pwndeditMOLDSETMAX	= NULL;
CtmWnd*   pwndTransmitCom 		= NULL;
CtmWnd*   pwnsMachineID 		= NULL;
CtmWnd*	  pwndInstallVers  		= NULL;

CtmWnd*   pwndedit01E0[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit01F0[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit0200[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit0220[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit0230[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit0240[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit02F0[MAXCONTROLS]		= {NULL};
CtmWnd*   pwndedit0600[MAXCONTROLS]		= {NULL};


CtmWnd*	  pwnsEditMMIVersion2     = NULL;
CtmWnd*	  pwndeditKernelVersion   = NULL;
CtmWnd*	  pwndeditM3KeyboardVersion   = NULL;

CtmWnd*	  pwndStr_Controller_Date   = NULL; //cjlee 2019/5/12 下午 09:48:37
CtmWnd*   pwndStr_Drive_Ver[5]		= {NULL}; // 驅動器版本
int iDrive_Ver_Count=0;
CtmWnd*   pwndImg_Drive[5]		= {NULL}; // 驅動器圖片
int iDrive_Img_Count=0;
CtmWnd*   u_pwnd_State[5] ={NULL};
long lStateValue = 0;
long lStateValue_Old = -1;
CtmWnd*	  pwndBmpController   = NULL;

//	fans add

int		u_n1E0 = 0;
int		u_n1F0 = 0;
int		u_n200 = 0;
int		u_n220 = 0;
int		u_n230 = 0;
int		u_n240 = 0;
int		u_n2F0 = 0;
int		u_n600 = 0;


CtmWnd*		pwnsEditMMIVersion     	= NULL;
CtmWnd*		pwnsEditMMIDate        	= NULL;
CtmWnd*		pwnsEditFORMLIBVersion 	= NULL;
CtmWnd*		pwnsEditFORMLIBDate    	= NULL;
CtmWnd*		pwnsEditRESVersion     	= NULL;
CtmWnd*		pwnsEditRESDate        	= NULL;
CtmWnd*		pwnsEditN2C28sign       = NULL;
CtmWnd*		pwndEditN2C28Vers       = NULL;
CtmWnd*		pwndEditN2C28Date       = NULL;
CtmWnd*		pwndEditAustoneVers	   	= NULL;
CtmWnd*		pwndEditAustoneDate	   	= NULL;
CtmWnd*  	m_pwndEditMachineType	= NULL;// YANGJX 2010-1-12 

CtmWnd*		pwnsEditDSPCH		 	= NULL;
CtmWnd*		pwnseditDSPVERS1		= NULL;
CtmWnd*		pwnseditOilCODE		 	= NULL;
CtmWnd*		pwndHmiViersion = NULL;

CtmWnd*		pwnsEditFuncCode		= NULL; 
CtmWnd*		pwndeditdingdan1		= NULL;
CtmWnd*		pwndeditdingdan2		= NULL;
CtmWnd*		pwndeditdingdan3		= NULL;
CtmWnd*		pwndeditdingdan4		= NULL;
CtmWnd*		pwndeditdingdan5		= NULL;
CtmWnd*		pwndeditdingdan6		= NULL;

//	fans add 2007/9/21 02:02下午
//CtmWnd*		pwndEditX2WithDB2[256];

//int			  nEditX2WithDB2 	= 0;
long		  lTimer = 1000 * 5;
BOOL		  bUpdate = TRUE;

BOOL		  b2F0Code =FALSE;
BOOL		  b_7AKFY 	=FALSE;

char		u_cszAustoneVers[10], u_cszAustoneDate[10];
int				iDSPCH	= 0;
char 		StringRead[6][64];

CTaskHost*	g_ptaskdspCH = NULL;

#ifdef	D_CANMASTER
extern	DWORD	g_dwM3KeyboardVers;
#endif

char* ParamStr[] = // 參數文字 
{
	/*機構參數*/
	"MECH_MECHTYPE", // 機型選擇
	"TRANS_TYPE",		 // 傳動方式
	"MECH_ENCTYPE",	 // 編碼器選擇
	"MOTOR_ENC_REZ", // 編碼器解析度
	"MECH_GEAR_D",	 // 每轉距離
	"MOTOR_RATIO",	 // 減速比 
	"MECH_DOUB_MECH",// 倍數機構 
	"MECH_POS_INV",	 // 位置反向
	/*速度參數*/
	"SPEED_MAX",	 // 最高轉速
	"SPEED_ACC_T",	 // 加速時間
	"SPEED_DCC_T",	 // 減速時間
	"SPEED_RATIO",	 // 平滑比例
	"SPEED_POS_KP",	 // 到位響應
	"SPEED_SPEED_KP",	 // 追隨響應
	/*區域設定*/
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
	"",	 // 
};

char* u_pszStrID[] = 
{
	"MACHINE_INTERFACE_Ver",
	"MACHINE_INTERFACE_VerTime",
	"MACHINE_CONFSYS_ELECNAMEPLATE_HARDWARE_PN",
	"MACHINE_CONFSYS_ELECNAMEPLATE_HARDWARE_SN",
	"MACHINE_CONFSYS_ELECNAMEPLATE_SOFTWARE_PN",
	"MACHINE_CONFSYS_ELECNAMEPLATE_SOFTWARE_VN",
	"MACHINE_INTERFACE_CMDMONITO",
	//"MACHINE_INTERFACE_POSNOW_X1",
	NULL,
};

char* Drive_Ver_DB[] = // 驅動器版本編號DB
{		
	"MACHINE_INTERFACE_CONTROL_RSV00",
	"MACHINE_INTERFACE_CONTROL_RSV01",
	"MACHINE_INTERFACE_CONTROL_RSV02",
	"MACHINE_INTERFACE_CONTROL_RSV03",
	"MACHINE_INTERFACE_CONTROL_RSV04",
};
char* u_pszImageBoxString[] =
{
	"BmpDrive1",
	"BmpDrive2",
	"BmpDrive3",
	"BmpDrive4",
	"BmpDrive5",
};

/*============================比對參數============================*/
int NumofMechType[] = {3,5}; // 三五軸機型對應軸數

char* Mech_Data_String[] = // 參數 元件 名稱
{
	"edit_ENC_REZ", 				// 編碼器解析度
	"edit_Motor_Ratio", 	  // 減速比
	"edit_Motor_Diameter",	// 節圓直徑
	"edit_Tooth_Num",				// 齒數
	"edit_M_Num",						// 模數
};
char* dbid_MechType[] = 
{
	"MACHINE_CONFIGURATION_MACHINETYPE", // 機型選擇DB 三軸 五軸
	"MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE", // 傳動方式DB
};

/*---------------------------------------------------------------------------+
| u_pszMechPara 傳動方式  																									 |
|							   _____________________________________        							 |	
|   						|	Bit |				0				|				1				| 										 |		
|								|	 0	|	使用節圓直徑	|	使用齒數模數  |       							 |
|								|	 1	|			不使用 		|	使用倍數機構  |       							 |
|								|__2__|_____不使用____|_使用位置反向__|       							 |
|													        																					 |
+---------------------------------------------------------------------------*/
char*	u_pszMechPara[] =
{
	"MACHINE_FUNCTION_OPTIONS_RSV07", // X1
	"MACHINE_FUNCTION_OPTIONS_RSV08", // Y1
	"MACHINE_FUNCTION_OPTIONS_RSV09", // Z
	"MACHINE_FUNCTION_OPTIONS_RSV10", // X2
	"MACHINE_FUNCTION_OPTIONS_RSV11"  // Y2
}; 

char* dbid_Mech[6][5] = // 參數數值 db
{	//  編碼器解析度									,減速比														,節圓直徑													,齒數														 ,模數
	{NULL,NULL,NULL,NULL,NULL},	// 共同
	{"MACHINE_INTERFACE_CONTROL_RSV05","MACHINE_INTERFACE_CONTROL_RSV10","MACHINE_FUNCTION_OPTIONS_RSV22","MACHINE_FUNCTION_OPTIONS_RSV12","MACHINE_FUNCTION_OPTIONS_RSV17"},		// X1軸
	{"MACHINE_INTERFACE_CONTROL_RSV06","MACHINE_INTERFACE_CONTROL_RSV11","MACHINE_FUNCTION_OPTIONS_RSV23","MACHINE_FUNCTION_OPTIONS_RSV13","MACHINE_FUNCTION_OPTIONS_RSV18"},		// Y1軸
	{"MACHINE_INTERFACE_CONTROL_RSV07","MACHINE_INTERFACE_CONTROL_RSV12","MACHINE_FUNCTION_OPTIONS_RSV24","MACHINE_FUNCTION_OPTIONS_RSV14","MACHINE_FUNCTION_OPTIONS_RSV19"},		// Z軸
	{"MACHINE_INTERFACE_CONTROL_RSV08","MACHINE_INTERFACE_CONTROL_RSV13","MACHINE_FUNCTION_OPTIONS_RSV25","MACHINE_FUNCTION_OPTIONS_RSV15","MACHINE_FUNCTION_OPTIONS_RSV20"},		// X2軸
	{"MACHINE_INTERFACE_CONTROL_RSV09","MACHINE_INTERFACE_CONTROL_RSV14","MACHINE_FUNCTION_OPTIONS_RSV26","MACHINE_FUNCTION_OPTIONS_RSV16","MACHINE_FUNCTION_OPTIONS_RSV21"}			// Y2軸
};

/*===========================================================================+
|           Function                                                         |
+===========================================================================*/
void	OnDestroyA(CtmWnd* pwndSender)
{
	DeleteTimer(&lTimer);
	//g_ptaskdsp->WriteValue(INITMESSAGE);
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	
	return wIDControl;
}

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	// 讀取設定 機型選擇 三五軸
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_EncType		 = dw_MechType & EncWord;
	
	//printf("MACHINE_INTERFACE_Ver ID=%d\n",g_pDatabase->GetIndex("MACHINE_CONFSYS_ELECNAMEPLATE_SOFTWARE_PN"));
	//printf("MACHINE_INTERFACE_POSNOW_X1 ID=%d\n",g_pDatabase->GetIndex("MACHINE_CONFSYS_ELECNAMEPLATE_SOFTWARE_VN"));
	pwndStr_Controller_Date = pwndSender->FindControlFromName("Str_Controller_Date");
  iDrive_Ver_Count = GetSpecialControlNum(pwndSender, "Str_Drive_Ver", "CtmStaticX2", pwndStr_Drive_Ver);
  iDrive_Img_Count = GetSpecialControlNum(pwndSender, "Img_Drive_", "CtmImageBox", pwndImg_Drive);
	for(int i = 0; i < sizeof(u_pszImageBoxString)/sizeof(u_pszImageBoxString[0]); i++ )
	{
		u_pwnd_State[i] = pwndSender->FindControlFromName(u_pszImageBoxString[i]);
	}  
	pwndBmpController = pwndSender->FindControlFromName("BmpController");
  
	b_7AKFY	= g_pExTable->GetTableValue(0,"D_7AKFY");
	AddTimer(&lTimer);
	char    szMachineID[10];

  	m_pwndeditMOLDSETMAX     = pwndSender->FindControlFromName("editMOLDSETMAX");
  	pwndTransmitCom			 = pwndSender->FindControlFromName("StaticCommand");
  	pwnsMachineID			 = pwndSender->FindControlFromName("editMACHINEID");
  	pwndInstallVers		     = pwndSender->FindControlFromName("StaticInstallVers");
  	
	pwnsEditMMIVersion		 = pwndSender->FindControlFromName("editMMIVersion"); 
	pwnsEditMMIDate          = pwndSender->FindControlFromName("editMMIDate");
	pwnsEditFORMLIBVersion	 = pwndSender->FindControlFromName("editFORMLIBVersion"); 
	pwnsEditFORMLIBDate      = pwndSender->FindControlFromName("editFORMLIBDate");
	pwnsEditRESVersion		 = pwndSender->FindControlFromName("editRESVersion"); 
	pwnsEditRESDate          = pwndSender->FindControlFromName("editRESDate");
	pwnsEditN2C28sign        = pwndSender->FindControlFromName("editN2C28sign");
	pwndEditN2C28Vers        = pwndSender->FindControlFromName("editN2C28Vers");
	pwndEditN2C28Date        = pwndSender->FindControlFromName("editN2C28Date");
	pwndEditAustoneVers		 = pwndSender->FindControlFromName("editAustoneVers");
	pwndEditAustoneDate		 = pwndSender->FindControlFromName("editAustoneDate");
  	m_pwndEditMachineType	 = pwndSender->FindControlFromName("editMACHINETYPE");
    
    pwnsEditFuncCode		= pwndSender->FindControlFromName("editFunctionCode");
    pwnseditDSPVERS1		= pwndSender->FindControlFromName("editDSPVERS1");
    pwnseditOilCODE		    = pwndSender->FindControlFromName("editOilCODE");
  	
    iNumedit01E0 = GetSpecialControlNum(pwndSender, NAME_EDIT01E0, "CtmEditX2", pwndedit01E0);
    iNumedit01F0 = GetSpecialControlNum(pwndSender, NAME_EDIT01F0, "CtmEditX2", pwndedit01F0);
    iNumedit0200 = GetSpecialControlNum(pwndSender, NAME_EDIT0200, "CtmEditX2", pwndedit0200);
    iNumedit0220 = GetSpecialControlNum(pwndSender, NAME_EDIT0220, "CtmEditX2", pwndedit0220);
    iNumedit0230 = GetSpecialControlNum(pwndSender, NAME_EDIT0230, "CtmEditX2", pwndedit0230);
    iNumedit0240 = GetSpecialControlNum(pwndSender, NAME_EDIT0240, "CtmEditX2", pwndedit0240);
    iNumedit02F0 = GetSpecialControlNum(pwndSender, NAME_EDIT02F0, "CtmEditX2", pwndedit02F0);
    iNumedit0600 = GetSpecialControlNum(pwndSender, NAME_EDIT0600, "CtmEditX2", pwndedit0600);
    
    pwndeditKernelVersion	 = pwndSender->FindControlFromName("editKernelVersion");
    pwndeditM3KeyboardVersion	 = pwndSender->FindControlFromName("editM3KeyboardVersion");
    
  	if(pwndeditKernelVersion !=NULL)       //zhouy 2017.2.23 for mmi版本日期
  	{
		pwndeditKernelVersion->SetPropValueT("value", GetKernelVers2());
		pwndeditKernelVersion->UpdateAll();
  	}
  	
#ifdef	D_CANMASTER
  	if(pwndeditM3KeyboardVersion !=NULL) //zhouy 2017.2.23  M3Key版本顯示
  	{
		pwndeditM3KeyboardVersion->SetPropValueT("value", (g_dwM3KeyboardVers+0x20000000)); /* 加0x20000000為了在版本前增加20 150410->20150410  */
		pwndeditM3KeyboardVersion->Update();
  	}
#else
	if(pwndeditM3KeyboardVersion != NULL)
  	{
  		pwndeditM3KeyboardVersion->SetPropValueT("wtype", 0x30); 
		pwndeditM3KeyboardVersion->SetPropValueT("visible", (double)FALSE); 
		pwndeditM3KeyboardVersion->Update();
  	}
#endif
    
    if(g_pExTable->GetTableValue(0,"D_SZFTVRT")||b_7AKFY||g_pExTable->GetTableValue(0,"D_VRTODR")) 
   	{
   		memset(StringRead, 0, sizeof(StringRead));
		  	
		pwndeditdingdan1	= pwndSender->FindControlFromName("editVW_DINGDAN1");/*wangli2013-6-26 11:09:10*/
		pwndeditdingdan2	= pwndSender->FindControlFromName("editVW_DINGDAN2");
		pwndeditdingdan3	= pwndSender->FindControlFromName("editVW_DINGDAN3");
		pwndeditdingdan4	= pwndSender->FindControlFromName("editVW_DINGDAN4");
		pwndeditdingdan5	= pwndSender->FindControlFromName("editVW_DINGDAN5");
		pwndeditdingdan6	= pwndSender->FindControlFromName("editVW_DINGDAN6");
		 
		char szTest[64];
		memset(szTest, 0, 64);
		
		ReadDingDanFrom();
		if(pwndeditdingdan1 != NULL)
		{
			pwndeditdingdan1->SetPropValueT("text", StringRead[0]); 
			pwndeditdingdan1->Update(); 
		}
		
		if(pwndeditdingdan2 != NULL)
		{
			pwndeditdingdan2->SetPropValueT("text", StringRead[1]); 
			pwndeditdingdan2->Update(); 
		}
		
		if(pwndeditdingdan3 != NULL)
		{
			pwndeditdingdan3->SetPropValueT("text", StringRead[2]); 
			pwndeditdingdan3->Update(); 
		}
		
		if(pwndeditdingdan4 != NULL)
		{
			pwndeditdingdan4->SetPropValueT("text", StringRead[3]); 
			pwndeditdingdan4->Update(); 
		}
		
		if(pwndeditdingdan5 != NULL)
		{
			pwndeditdingdan5->SetPropValueT("text", StringRead[4]); 
			pwndeditdingdan5->Update(); 
		}
		
		if(pwndeditdingdan6 != NULL)
		{
			pwndeditdingdan6->SetPropValueT("text", StringRead[5]); 
			pwndeditdingdan6->Update(); 
		}
   	}

	if(g_pExTable->GetTableValue(0,"D_0HTVERS"))
		b2F0Code =TRUE;

  	pwnsEditDSPCH	       	= pwndSender->FindControlFromName("editDSP_CHANNEL");  
  	if(pwnsEditDSPCH != NULL)
  		pwnsEditDSPCH->GetPropValueT("value",&iDSPCH,sizeof(iDSPCH));
  	if(iDSPCH) 	g_ptaskdspCH = g_ptaskCom2dsp;
  	else		g_ptaskdspCH	= g_ptaskdsp;

    pwndHmiViersion = pwndSender->FindControlFromName("editMMIVERSION1");
    if(g_ptaskdspCH != NULL)
    {
    	g_ptaskdspCH->ReqValues(COMM_GETCOMMAND, (void*)&iComIndex, sizeof(iComIndex));
    	ShowTransmitCom(pwndSender);
    }
    CreateSet(pwndSender);
    GetDBString("OPER_HMI_NULL_NULL_MACHINEID", szMachineID, 9);
	if(pwnsMachineID 	  != NULL) 
		pwnsMachineID->SetPropValueT("text", szMachineID);
		
		if(g_pExTable->GetTableValue(0,"D_7LUNEW"))
		{
				if(pwnseditDSPVERS1!=NULL)
				{
					pwnseditDSPVERS1->SetPropValueT("text", "7LUA");
					pwnseditDSPVERS1->Update();
				}
				if(pwnseditOilCODE!=NULL)
				{
					char		szDBID[256];
					long long 	llValue = 0;
					szDBID[0] = '\0';
					if(GetDBValue("MCONF_MOLDHIGHT_MOLDHIGHT_NULL_OILTONOPTION").lValue)
					{
						llValue = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED89").lValue;
						switch(llValue)
						{
							case 1:
        	 					llValue=98;//60EK
       						break;
       						
       						case 2:
        	 					llValue=146;//90EK
       						break;
       						
       						case 3:
        	 					llValue=290;//120EK
       						break;
       						
       						case 4:
        	 					llValue=354;//160EK
       						break;
       						
       						case 5:
        	 					llValue=514;//200EK
       						break;
       						
       						case 6:
        	 					llValue=610;//260EK
       						break;
       						
       						case 7:
        	 					llValue=802;//320EK
       						break;
       						
       						case 8:
        	 					llValue=898;//400EK
       						break;
       						
       						case 9:
        	 					llValue=1154;//500EK
       						break;
       						
       						case 10:
        	 					llValue=1410;//600EK
       						break;
       						
       						case 11:
        	 					llValue=1666;//700EK
       						break;
       						
       						case 12:
        	 					llValue=2050;//800EK
       						break;
       						
       						case 13:
        	 					llValue=2178;//900EK
       						break;
       						
       						case 14:
        	 					llValue=41090;//1100EK
       						break;
       						
       						case 15:
        	 					llValue=41602;//1300EK
       						break;
       						
       						case 16:
        	 					llValue=42626;//1700EK
       						break;
       						
       						default:
        					break;
						}
					}
					else
					{	pwnseditOilCODE->GetPropValueT("dbid2", szDBID, sizeof(szDBID));
						llValue = GetDBValue(szDBID).lValue;	
					}		
					pwnseditOilCODE->SetPropValueT("value", llValue);
					pwnseditOilCODE->Update();
				}
					
		}
		
		char    szMachineType[10];
	  memset(szMachineType, 0, 10);
	  GetDBString("SYS_HMI_NULL_NULL_CUSTOMERID", szMachineType, 9);
	  if(m_pwndEditMachineType 	  != NULL) 
		m_pwndEditMachineType->SetPropValueT("text", szMachineType);
    		
    char    szMMIVersion[10];
    strncpy(szMMIVersion, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMMMIVERS").pcData, 8 );
	if(pwnsEditMMIVersion 	  != NULL) 
		pwnsEditMMIVersion->SetPropValueT("text", szMMIVersion);
    
    char    szMMIDate[10];
    strncpy(szMMIDate, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMMMIDATE").pcData, 8 );
	if(pwnsEditMMIDate 	  != NULL) //mark 2008-7-3
		pwnsEditMMIDate->SetPropValueT("text", szMMIDate);
		
    char    szFORMLIBVersion[10];
    strncpy(szFORMLIBVersion, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMFORMLIBVERS").pcData, 8 );
	if(pwnsEditFORMLIBVersion 	  != NULL) 
		pwnsEditFORMLIBVersion->SetPropValueT("text", szFORMLIBVersion);
		
    char    szFORMLIBDate[10];
    strncpy(szFORMLIBDate, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMFORMLIBDATE").pcData, 8 );
	if(pwnsEditFORMLIBDate 	  != NULL) 
		pwnsEditFORMLIBDate->SetPropValueT("text", szFORMLIBDate);
		
    char    szRESVersion[10];
	strncpy(szRESVersion, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMRESVERS").pcData, 8 );
	if(pwnsEditRESVersion 	  != NULL) 
		pwnsEditRESVersion->SetPropValueT("text", szRESVersion);
    char    szRESDate[10];
	strncpy(szRESDate, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMRESDATE").pcData, 8 );
	if(pwnsEditRESDate 	  != NULL) 
		pwnsEditRESDate->SetPropValueT("text", szRESDate);
		
	memset(u_cszAustoneVers, 0, sizeof(u_cszAustoneVers));
	DWORD dwValue = GetDBValue("AUSTON_ACTUALDATA_PROGRAM_VERSION").lValue;
	u_cszAustoneVers[3] = *(BYTE*)&dwValue;
	u_cszAustoneVers[2] = *((BYTE*)&dwValue+1);
	u_cszAustoneVers[1] = *((BYTE*)&dwValue+2);
	u_cszAustoneVers[0] = *((BYTE*)&dwValue+3);	
	
	
	if(pwndEditAustoneVers 	  != NULL)
	{
		pwndEditAustoneVers->SetPropValueT("text", u_cszAustoneVers);
		pwndEditAustoneVers->Update();
	}

	if(pwnsEditFuncCode 	  != NULL) 
		pwnsEditFuncCode->SetPropValueT("text", szFORMLIBVersion);
	//memset(u_cszAustoneVers, 0, sizeof(u_cszAustoneVers));
	//u_cszAustoneVers[3] = *(BYTE*)&dwValue;
	//u_cszAustoneVers[2] = *((BYTE*)&dwValue+1);
	//u_cszAustoneVers[1] = *((BYTE*)&dwValue+2);
	//u_cszAustoneVers[0] = *((BYTE*)&dwValue+3);	
	//if(pwndEditAustoneVers 	  != NULL)
	//{
	//	pwndEditAustoneVers->SetPropValueT("text", u_cszAustoneVers);
	//	pwndEditAustoneVers->Update();
	//}
	
	//SetDBValue("AUSTON_ACTUALDATA_PROGRAM_DATETIME", 0x000C0002);
	//memset(u_cszAustoneDate, 0, sizeof(u_cszAustoneDate));
	//memcpy(u_cszAustoneDate, &GetDBValue("AUSTON_ACTUALDATA_PROGRAM_DATETIME").lValue, sizeof(DWORD));
	//if(pwndEditAustoneDate 	  != NULL) 
	//{
	//	pwndEditAustoneDate->SetPropValueT("text", u_cszAustoneDate);
	//	pwndEditAustoneDate->Update();
	//}
	//FlowStep = 0;
	//SendFlag = FALSE;
	//nEditX2WithDB2 = GetSpecialControlNum(pwndSender, "dbid1", pwndEditX2WithDB2);
	pwnsEditMMIVersion2		 = pwndSender->FindControlFromName("editMMIVERSION2");   
	char    szMMIVersion2[10];  
	strncpy(szMMIVersion2, g_pDatabase->Read("VERS_HMI_NULL_NULL_PROGRAMMMIVERS").pcData, 8 );
	if(pwnsEditMMIVersion2 	  != NULL)
	{
		pwnsEditMMIVersion2->SetPropValueT("text", szMMIVersion2);
		pwnsEditMMIVersion2->Update();
	}
	
	char szVers[10]="\0";
    int nVers = (char)CtmConfig::GetInstance()->GetStopPWDType();
    sprintf(szVers,"%d",nVers);
    if(pwndInstallVers != NULL)
    {
 		pwndInstallVers->SetPropValueT("text", szVers);
 		pwndInstallVers->Update();
    }
	    
	DIAGRM      	 diagrm;
	memset(&diagrm, 0, sizeof(diagrm));

	if(g_ptaskdspCH != NULL)
   		g_ptaskdspCH->WriteValue(COMM_INIT_MESSAGE);     //2008-9-28   
	
	diagrm.wAddressROM = 0x00F0;//1E0
	if(g_ptaskdspCH != NULL)
		u_n1E0 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
		
	/*編碼器液壓  N2C28 液壓轉盤*/
	if(g_pExTable->GetTableValue(0,"D_N2CROT"))
	{
		;
	}
	else
	{
		SetVisible(pwnsEditN2C28sign, FALSE,0);	
		SetVisible(pwndEditN2C28Vers, FALSE,0);	
		SetVisible(pwndEditN2C28Date, FALSE,0);	
	}	
	
	// 驅動器圖片
	for(int i=0;i<iDrive_Img_Count;i++)
	{
		if(pwndImg_Drive[i]!=NULL)
		{
			if(u_EncType==EncType_Abs) // 絕對型 ENC for AUSTONE
			{
				pwndImg_Drive[i]->SetPropValueT("imagepath",DriveImgPath_Austone);
			}
			else if(u_EncType==EncType_Res) // 增量式 for HDT
			{
				pwndImg_Drive[i]->SetPropValueT("imagepath",DriveImgPath_HDT);
			}
			pwndImg_Drive[i]->CreateA();
			pwndImg_Drive[i]->Update();
		}
	}
	
    return TRUE;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
//		case 1:
//			::PutCommand("Syst.txt");
//			break;
//		case 2:
//			::PutCommand("Reset.txt");
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

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else                              return _NULL_KEY;
}



void		SetDBRM(CtmWnd* pwnd[], int nCount)
{
	tmDB_DIAGRM    	db_diagrm ;
	long long 		TempValue = 0;
	
	if(g_ptaskdspCH != NULL)
		g_ptaskdspCH->ReqValues(COMM_GETDIAGRM, &db_diagrm, sizeof(tmDB_DIAGRM));
	
	for (int i = 0; i < nCount; i++)
	{
		if (pwnd[i] != NULL)
		{
			TempValue =db_diagrm.awData[i];
			pwnd[i] ->SetPropValueT("value",TempValue);
			pwnd[i] ->Update();
		}
	}
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	///tmDB_DIAGRM*	db_diagrm;
	////long long 		TempValue;
	DIAGRM      	 diagrm;
	memset(&diagrm, 0, sizeof(diagrm));
	switch(message)
	{
		case MSG_DSP54_REVRM:
			//FlowStep++;
			if(lParam == (DWORD)u_n1E0)	
			{
				SetDBRM(pwndedit01E0, MAXCONTROLS);
				diagrm.wAddressROM = 0x00F8;//1F0
				if(g_ptaskdspCH != NULL)
					u_n1F0 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
			}
			else if(lParam == (DWORD)u_n1F0)
			{
				SetDBRM(pwndedit01F0, MAXCONTROLS);
				diagrm.wAddressROM = 0x0100;//200
				if(g_ptaskdspCH != NULL)
					u_n200 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
			}
			else if(lParam == (DWORD)u_n200)
			{
				SetDBRM(pwndedit0200, MAXCONTROLS);
				diagrm.wAddressROM = 0x0110;  //220
				if(g_ptaskdspCH != NULL)
					u_n220 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
			}
			else if(lParam == (DWORD)u_n220)
			{
				SetDBRM(pwndedit0220, MAXCONTROLS);
				diagrm.wAddressROM = 0x0118;  //230
				if(g_ptaskdspCH != NULL)
					u_n230 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
			}
			else if(lParam == (DWORD)u_n230)
			{
				SetDBRM(pwndedit0230, MAXCONTROLS);
				diagrm.wAddressROM = 0x0120;  //240
				if(g_ptaskdspCH != NULL)
					u_n240 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
			}
			else if(lParam == (DWORD)u_n240)
			{
				SetDBRM(pwndedit0240, MAXCONTROLS);
				if(b2F0Code)
				{
					diagrm.wAddressROM = 0x0178;  //2F0
					if(g_ptaskdspCH != NULL)
						u_n2F0 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
				}
				else
				{
					diagrm.wAddressROM = 0x0300;  //600
					if(g_ptaskdspCH != NULL)
						u_n600 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
				}
			}
			else if(lParam == (DWORD)u_n2F0)
			{
				SetDBRM(pwndedit02F0, MAXCONTROLS);
				diagrm.wAddressROM = 0x0300;  //600
				if(g_ptaskdspCH != NULL)
					u_n600 = g_ptaskdspCH->WriteValue(COMM_SENDDIAGRM, (void*)&diagrm, sizeof(DIAGRM));
			}
			else if(lParam == (DWORD)u_n600)
			{
				SetDBRM(pwndedit0600, MAXCONTROLS);
			}
			break;
		default:
			break;
	}
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
	{
		printf("RunOnlyOne\n");
		RunOnlyOne=TRUE;
		GetValueFrom28(); // 參數比較
	}
	
	
	if(g_ptaskdspCH != NULL)
	{
		ShowTransmitCom(pwndSender);
	}
	
	if(g_ptaskpicker != NULL)
	{
		WORD		wNum = 0;
		wNum = sizeof(u_pszStrID)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID); // Update Data from 28
		
		// 顯示 ASCII 值  主機日期 各軸版本
		DWORD dw_dbvalue = 0;//825832760;
		//SetDBValue("MACHINE_CONFSYS_ELECNAMEPLATE_HARDWARE_SN", 825832760); //測試用
		dw_dbvalue = GetDBValue("MACHINE_CONFSYS_ELECNAMEPLATE_HARDWARE_SN").lValue;
		if(pwndStr_Controller_Date!=NULL)
		{
			pwndStr_Controller_Date->SetPropValueT("text",DW2Str_ASCII(dw_dbvalue));
			pwndStr_Controller_Date->Update();
		}

		// 驅動器版本號
		for(int i=0;i<iDrive_Ver_Count;i++)
		{
			//SetDBValue(Drive_Ver_DB[i], 825770807); //測試用
			dw_dbvalue = GetDBValue(Drive_Ver_DB[i]).lValue;
			if(pwndStr_Drive_Ver[i]!=NULL)
			{
				if(u_EncType==EncType_Abs) // 絕對型 ENC for AUSTONE
				{
					pwndStr_Drive_Ver[i]->SetPropValueT("text",DW2Str_ASCII(dw_dbvalue));
				}
				else if(u_EncType==EncType_Res) // 增量式 for HDT
				{
					char  str_temp[256] = "\0";
					sprintf(str_temp ,"%d",dw_dbvalue);
					pwndStr_Drive_Ver[i]->SetPropValueT("text",str_temp);
				}
				pwndStr_Drive_Ver[i]->Update();
			}
		}
		
	char  pszPath[256] = "\0";
	lStateValue   = GetDBValue("MACHINE_INTERFACE_CMDMONITO").lValue;	
	if(lStateValue != lStateValue_Old)	// 驅動器 通訊燈號
	{
		for(int i = 0; i < sizeof(u_pszImageBoxString)/sizeof(u_pszImageBoxString[0]); i++)
		{
			if(_TestBit(lStateValue, i)) sprintf(pszPath,"res_tm640/pic/picker/connected.bmp");
			else sprintf(pszPath, "res_tm640/pic/picker/unselected.bmp");
			 if(u_pwnd_State[i] != NULL)
			 {
				 u_pwnd_State[i]->SetPropValueT("imagepath", pszPath);
				 u_pwnd_State[i]->Update();
			 }
		}	
		lStateValue_Old = lStateValue;
	}	
	
	if(pwndBmpController!=NULL) // 主機連線 通訊燈號
	{
		int npickerStatus = g_ptaskpicker->GetOnLineStatus();
		if(!npickerStatus)  
			{
				npickerStatus = 1;
				sprintf(pszPath,"res_tm640/pic/picker/connected.bmp");
			}
		else
			{
				npickerStatus = 0;
				sprintf(pszPath, "res_tm640/pic/picker/unselected.bmp");
			}
		pwndBmpController->SetPropValueT("imagepath", pszPath);
		pwndBmpController->Update();
	}
	

		//printf("MACHINE_INTERFACE_Ver ID=%d\n",g_pDatabase->GetIndex("MACHINE_INTERFACE_Ver"));
		//printf("MACHINE_INTERFACE_POSNOW_X1 ID=%d\n",g_pDatabase->GetIndex("MACHINE_INTERFACE_POSNOW_Y1"));

		//g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,u_pszStrID);
//		int index =8388613;
//		g_ptaskpicker->WriteValue(REQ_READMOTOR, 1 ,&index);
//		int index2 =8390513;
//		g_ptaskpicker->WriteValue(REQ_READMOTOR, 1 ,&index2);
	}	
	//if (bUpdate)
	//{
	//	if (lTimer <= 0)
	//	{
	//		long long 	llValue = 0;
	//		char		szDBID[256];
	//		for (int i = 0; i < nEditX2WithDB2; i++)
	//		{
	//			llValue = 0;
	//			szDBID[0] = '\0';
	//			if (pwndEditX2WithDB2[i] != NULL && pwndHmiViersion != pwndEditX2WithDB2[i])
	//			{
	//				pwndEditX2WithDB2[i]->GetPropValueT("dbid1", szDBID, sizeof(szDBID));
	//				llValue = GetDBValue(szDBID).lValue;
	//				
	//				pwndEditX2WithDB2[i]->SetPropValueT("value", llValue);
	//				pwndEditX2WithDB2[i]->Update();
	//				
	//				
	//			}
	//		}
	//		bUpdate = FALSE;
	//	}
	//}
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

void       CreateSet(CtmWnd* pwndSender)
{
    MaxCount     = CtmMoldSet::GetInstance()->GetMaxMoldSetNum ();
    if(m_pwndeditMOLDSETMAX   != NULL) 
    {
    	m_pwndeditMOLDSETMAX->SetPropValueT("value", MaxCount);
    	m_pwndeditMOLDSETMAX->Update();
    }
}

void	ShowTransmitCom(CtmWnd* pwndSender)
{
	char TransmitCom[20];
	char sz[20];    //Sunny<20070429>
	WORD    *pwDst; //Sunny<20070429>
	int j = 0;
	int iTempComIndex= -1;

	if(g_ptaskdspCH != NULL)
		g_ptaskdspCH->ReqValues(COMM_GETCOMMAND, (void*)&iTempComIndex, sizeof(iTempComIndex));	//2008-9-28

	if(iComIndex != iTempComIndex && g_wDspStatus != 0)
	{
		if(g_pFont->GetDefaultFont()->GetFontCode() == tmFT_CODE_TECH)//Sunny<20070429>
		{
			sprintf(TransmitCom,"%4x",iComIndex);
			pwDst  = (WORD *)sz;
	    	while(TransmitCom[j] != 0)
	        	*pwDst ++   = TransmitCom[j ++];
	    	*pwDst  = 0;
	    }
	    else sprintf(sz,"%4x",iComIndex);
		
		pwndTransmitCom->SetPropValueT("text",sz);
		pwndTransmitCom->Clear();
		pwndTransmitCom->Update();
		if(g_ptaskdspCH != NULL)
			g_ptaskdspCH->ReqValues(COMM_GETCOMMAND, (void*)&iComIndex, sizeof(iComIndex));
	}
	else if(g_wDspStatus == 0)
	{
		pwDst  = (WORD *)sz;
		if(g_pFont->GetDefaultFont()->GetFontCode() == tmFT_CODE_TECH)//Sunny<20070429>
		{
            *pwDst ++   = ' ';
            *pwDst      = 0;
        }
        else	strcpy(sz," ");
		
		if(pwndTransmitCom != NULL)
		{
			pwndTransmitCom->SetPropValueT("text",sz);
			pwndTransmitCom->Clear();
			pwndTransmitCom->Update();
		}
		
		iComIndex = -1;
	}
}

void		SetVisible(CtmWnd* pwnd,BOOL  bVisible,WORD  wType)
{
	if(pwnd != NULL)
	{
		if(bVisible)
		{
			if(wType)
				pwnd->SetPropValueT("wtype",wType); 
			else
				pwnd->SetPropValueT("wtype",0x21);
		}
		else
			pwnd->SetPropValueT("wtype",0x30);
		//if(bUpdate)
			pwnd->Update();
		
		pwnd->SetPropValueT("visible",(double)bVisible); 	
		//if(bUpdate)
			pwnd->Update();
	}		
}

void   ReadDingDanFrom()
{
 	char	szPath[256];
	char	m_szCopyFile[256];
    char	pszDB[256];

	memset(pszDB, 0, sizeof(szPath));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/usr/Dingdan");
	ExistDir(szPath);	
	sprintf(m_szCopyFile, "%s/dingdan.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR | O_CREAT);
	
	if (nFd >= 0)
	{
		int nSize = lseek(nFd, 0, SEEK_END);
		if (nSize > 0)
		{
	    	lseek(nFd, 0, SEEK_SET);
		   	for (int i = 0; i < nSize; i++)
		      read(nFd, &StringRead, sizeof(StringRead));					 
	  	}
	}
	close(nFd);
}

char*	DW2Str_ASCII(DWORD dw_dbvalue)
{
	char *strp=NULL;
	char ACII_value[256] = {NULL};
	for(int i=0;i<4;i++) // 4位數
	{
		DWORD dw_value = (dw_dbvalue>>((3-i)*8))&0x000000FF;
		char s_temp[256];
		//sprintf(s_temp,"%x",dw_value-'0'); // 轉ASCII
		sprintf(s_temp,"%c",dw_value); // 轉ASCII
		strncat(ACII_value, s_temp, strlen(s_temp)); // 合併字串
		strp=ACII_value;
		//printf("ACII_value=%s\n",ACII_value);
	}
	return strp;
}


/*---------------------------------------------------------------------------+
|  Function : GetValueFrom28()                       					    		       |
|  Task     : 取得28設定值 比對參數                                          |
+----------------------------------------------------------------------------+
|  Parameter:   																	                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	GetValueFrom28() // 取得28設定值 比對參數
{
	if(g_ptaskpicker != NULL)
	{
		printf("Get 28 data value\n");
		int i_dbvalue_497[6][5] = {0};// 參數數值 db// 示教器暫存值
		int i_dbvalue_different = 0; // 參數數值差異個數
		int iDBSelect=0; // 參數依據選擇  1:以示教器為主 2:以控制器為主
		
		WORD wNum = 0;
		int  itemp=0;
		tmParam Param497; // 497 示教器上參數
		tmParam Param28;
		
		// 機型選擇
		// 紀錄497數值
		Param497.iMechType = (int)(GetDBValue(dbid_MechType[0]).lValue);
		
		wNum = sizeof(dbid_MechType)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,dbid_MechType); // Update Data from 28
		
		printf("Get = %s\n",dbid_MechType[0]);
		Param28.iMechType = (int)(GetDBValue(dbid_MechType[0]).lValue); // 機型選擇
		printf("%s=%d\n",dbid_MechType[0],Param28.iMechType); // 28的值
		printf("497=%d\n",Param497.iMechType); // 497的值

		if(Param497.iMechType != Param28.iMechType) // 497和28三五軸選擇不同 機型選擇
		{
			MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[0]]);
			iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
			printf("Choose %d\n",iDBSelect);
			SetChosenDB(dbid_MechType[0],Param497.iMechType,Param28.iMechType,iDBSelect);
		}
		
		// 紀錄497數值 傳動方式 倍數機構 位置反向
//		Param497.iTransType = ((int)(GetDBValue(u_pszMechPara[0]).lValue) & 0x001); // 傳動方式
//		Param497.iDouble = ((int)(GetDBValue(u_pszMechPara[0]).lValue) & 0x002)>>1; // 倍數機構
//		Param497.iPosInv = ((int)(GetDBValue(u_pszMechPara[0]).lValue) & 0x004)>>2; // 位置反向
		for(int NumofAxis=0; NumofAxis<NumofMechType[dw_MechType]; NumofAxis++)
		{
			printf("NumofAxis=%d\n",NumofAxis);
			// 紀錄497數值 傳動方式 + 倍數機構 + 位置反向
			Param497.iTransType = (int)(GetDBValue(u_pszMechPara[NumofAxis]).lValue); // 傳動方式 + 倍數機構 + 位置反向
			// 向 28 請求資料 傳動方式 + 倍數機構 + 位置反向
			wNum = 1;
			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,&u_pszMechPara[NumofAxis]);
			
		
			printf("Get = %s\n",u_pszMechPara[NumofAxis]);
			Param28.iTransType = (int)(GetDBValue(u_pszMechPara[NumofAxis]).lValue);
			
			if (itemp != i_dbvalue_497[NumofAxis] )
				i_dbvalue_different++;
			
			printf("%s=%d\n",u_pszMechPara[NumofAxis],Param28.iTransType); // 28的值
			printf("497=%d\n",Param497.iTransType); // 497的值
			
			if(Param497.iTransType != Param28.iTransType) // 497和28 傳動方式 + 倍數機構 + 位置反向 不同
			{
				MsgBoxCall("DB_Choose.txt",g_MultiLanguage[ParamStr[1]]);
				iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
				printf("Choose %d\n",iDBSelect);
				SetChosenDB(u_pszMechPara[NumofAxis],Param497.iTransType,Param28.iTransType,iDBSelect);
			}
		}

		
//		printf("dw_MechType=%d\n",NumofMechType[dw_MechType]);
//		// 依據 機構選擇dw_MechType 決定比對軸數NumofMechType[dw_MechType]
//		for(int NumofAxis=0; NumofAxis<NumofMechType[dw_MechType]; NumofAxis++)
//		{
//			// 紀錄497數值
//			for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
//			{			
//				itemp = (int)(GetDBValue(dbid_Mech[NumofAxis+1][i]).lValue);
//				i_dbvalue_497[NumofAxis+1][i] = itemp;
//			}
//			
//			// 向 28 請求資料 Mech 機械參數
//			wNum = sizeof(dbid_Mech[NumofAxis+1])/sizeof(char*);
//			g_ptaskpicker->ReqValues(REQ_READMOTOR, wNum ,dbid_Mech[NumofAxis+1]);
//
//			for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
//			{			
//				printf("Get = %s\n",dbid_Mech[NumofAxis+1][i]);
//				itemp = (int)(GetDBValue(dbid_Mech[NumofAxis+1][i]).lValue);
//				
//				if (itemp != i_dbvalue_497[NumofAxis+1][i] )
//					i_dbvalue_different++;
//				
//				printf("%s=%d\n",dbid_Mech[NumofAxis+1][i],itemp); // 28的值
//				printf("497=%d\n",i_dbvalue_497[NumofAxis+1][i]); // 497的值
//			}
//			printf("i_dbvalue_different=%d\n",i_dbvalue_different);
//			// 示教器和控制器參數數值不同
//			if(i_dbvalue_different>0)
//			{
//				MsgBoxCall("DB_Choose.txt");
//				
//				iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71").lValue;
//				printf("Choose %d\n",iDBSelect);
//				if(iDBSelect == DB_TP) // 497為主
//				{
//					for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
//					{			
//						printf("Set %s = %d\n",dbid_Mech[NumofAxis+1][i],i_dbvalue_497[NumofAxis+1][i]);
//						SetDBValue(dbid_Mech[NumofAxis+1][i],i_dbvalue_497[NumofAxis+1][i]);
//					}
//				}
//				else if(iDBSelect == DB_CON) // 28控制器為主
//				{
//					for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
//					{			
//						itemp = (int)(GetDBValue(dbid_Mech[NumofAxis+1][i]).lValue);
//						SetDBValue(dbid_Mech[NumofAxis+1][i],0);
//						printf("Set %s = %d\n",dbid_Mech[NumofAxis+1][i],itemp);
//						SetDBValue(dbid_Mech[NumofAxis+1][i],itemp);
//					}
//				}
//				i_dbvalue_different=0;
//			}
//		}
	}
}

/*---------------------------------------------------------------------------+
|  Function : SetChosenDB()                       						    		       |
|  Task     : 設定選擇DB數值				                                         |
+----------------------------------------------------------------------------+
|  Parameter: dbIDName:DB名稱, Param497:示教器數值, Param28:28數值           |
|							iDBSelect: 選擇來源																						 |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetChosenDB(char* dbIDName, int Param497, int Param28, int iDBSelect)
{
	if(iDBSelect == DB_TP) // 497為主
	{
		SetDBValue(dbIDName,Param497);
	}
	else if(iDBSelect == DB_CON) // 28控制器為主
	{
		SetDBValue(dbIDName,0);
		SetDBValue(dbIDName,Param28);
	}
}