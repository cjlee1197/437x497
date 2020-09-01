/*===========================================================================+
|  Class    : ViewOver library                                               |
|  Task     : ViewOver library action source file                            |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 										                                           |
|  Version  : V1.00      			                                               |
|  Creation : 			                                            		         |
|  Revision :   																														 |
+===========================================================================*/
#include 	"ViewOver_IML.h"
#include	"../main.h"
#include	"../_kb.h"
#include 	"../common.h"
#include 	"../commonaction.h"
#include	"../timer.h"
#include	"../utils.h"
#include	"../manufacture.h"
#include	"../tmdebug.h"
#include	"../selvat_ancestor.h"
#include	"../tmconfig.h"
#include	"../lib/libiNetComm.h"
#include	"../tmshmsg.h"
#include	"../lib/libControl.h"
#include	"../font.h"

/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word

#define   MANAGE					2 // 管理
#define   ENGINEER				3 // 工程

#define		CONST_REQ_COMMAND				6

#define		ENABLE							-1
#define		DISABLE							-2

#define		ACT_NULL						0
#define		ACT_AXIS						1
#define		ACT_VALVE						2
#define		ACT_PILE						3
#define		ACT_WAIT						4
#define		ACT_SIGN						5
#define		ACT_ASSIT						6
#define		ACT_JUMP						7

#define     PB_MAXNUM						3
#define     PC_MAXNUM						3
#define		ID_MAX_SELECT					48
#define     STR_MAX_COUNT       			256
#define		REQ_READMOTOR    				2   
#define		STEPMAX    						100

#define		StaticBgc		0xFFFF

#define		LGreen					0xCF90 // 0x8627 // 青綠
#define		White						0xFFFF // 白
#define		Gray						0xFFDF // 灰
#define		Yellow					0xFFE0 // 黃
#define		LBlue						0xD73D // 淺藍
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_PickerType 	= 0; // 機型選擇 0-三軸 1-五軸
int u_EncType 		= 0; // 編碼器選擇  0-絕對 1-增量
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸
int		StandbyStepNum=0; // 待機點步驟數量 3軸-5步 5軸-7步

extern	int         g_nDemo;		
int					u_nClickNum	= 0;
		
long        lSpeedValueOld=0; //2018-2-9 
WORD   		wStepNum=0,wStepNumOld=-1;
int     	numStaticSpeed=0,u_nSelectBoxCount=0,iEditNo=0,iCheckBoxAct=0 ,iJumpBtn=0;
int		u_nDetectBoxCount=0;
int 	iStaticAct=0;
long		u_lTimer = 0;
char        u_acSelBoxString[ID_MAX_SELECT][STR_MAX_COUNT];


DBVIEWVALUE u_dbInputValue;
WORD        u_wInputValue,u_wInputValue_OLD;

DBVIEWVALUE u_dbOutputValue;
WORD        u_wOutputValue;

DBVIEWVALUE u_dbEX_InputValue;
DWORD        u_wEX_InputValue,u_wEX_InputValue_OLD;

DBVIEWVALUE u_dbEX_OutputValue;
WORD        u_wEX_OutputValue;


BOOL 		u_Jump =TRUE;

typedef struct tagACTIONSTATUS
{
	WORD	wActionType;
	char*	pszStr[10];
}ACTIONSTATUS;

CtmWnd*			pwndImg_Over_Picker				 = NULL; // 主畫面 Picker 圖片

CtmWnd*			pwndStaticStepType				 = NULL;
CtmWnd*			pwndStaticActionType			 = NULL;
CtmWnd*			pwndTotalCount      			 = NULL;
CtmWnd*			pwndTotalGoodCount  			 = NULL;
CtmWnd*			pwndTotalbadCount   			 = NULL;
CtmWnd*	    pwndTotalSampleCount			 = NULL;

CtmWnd*		  pwndStaticSpeed[32] = {NULL};

CtmWnd*			pwndEditNo[32] 	={NULL}; 	
CtmWnd*			pwndCheckBoxAct[32] ={NULL};
CtmWnd*			pwndiJumpBtn[32] ={NULL};

CtmWnd*			pwndStaticAct[32] ={NULL}; 	//動作列文字

CtmWnd*			pwndEditPostionX = NULL; //各軸座標
CtmWnd*			pwndEditPostionY = NULL;
CtmWnd*			pwndEditPostionZ = NULL;
CtmWnd*			pwndEditPostionX2 = NULL;
CtmWnd*			pwndEditPostionY2 = NULL;

CtmWnd*			pwndEJECTPOSITION = NULL;

CtmWnd*			pwndButtonStartEdit = NULL; //開始編輯
CtmWnd*			pwndBtnJump = NULL; //彈跳視窗
CtmWnd*			pwndBtnClose = NULL;
BOOL b_BtnJump = FALSE;
int	u_BtnjumpCount=0;
char* BtnJumpImg[] = 
{
	"res_tm640/pic/BtnJump.bmp",
	"res_tm640/pic/BtnJump2.bmp",
};
char* BtnCloseImg[] = 
{
	"res_tm640/pic/BtnClose.bmp",
	"res_tm640/pic/BtnClose2.bmp",
};

CtmWnd*			pwndBtnRight = NULL; // 彈跳視窗 右
CtmWnd*			pwndBtnLeft  = NULL; // 彈跳視窗 左

CtmWnd*		pwndPickP_ADJ[6]	= {NULL}; // 取出補正 數值輸入
char* PickP_ADJ_String[] = // 取出補正 數值輸入 名稱
{
	"",
	"PickP_ADJ_X1",
	"PickP_ADJ_Y1",
	"PickP_ADJ_Z",
	"PickP_ADJ_X2",
	"PickP_ADJ_Y2",
};
CtmWnd*		pwndPlaceP_ADJ[6]	= {NULL}; // 置物補正 數值輸入
char* PlaceP_ADJ_String[] = // 置物補正 數值輸入 名稱
{
	"",
	"PlaceP_ADJ_X1",
	"PlaceP_ADJ_Y1",
	"PlaceP_ADJ_Z",
	"PlaceP_ADJ_X2",
	"PlaceP_ADJ_Y2",
};


//char*				u_pszOperationStateID    = "MACHINE_INTERFACE_WOPERATINGSTATE";

CtmWnd*     u_pwndSelectBox[ID_MAX_SELECT];
CtmWnd*     u_pwndDetectBox[ID_MAX_SELECT];

BGC		bgc_Enable    = YELLOW;
BGC		bgc_Disenable = 0x95FB;
FGC		fgc           = 0;
char	szPath[128];
char 	u_szPath[256] = "\0";

char* u_pszStrID[] = 
{
	"MACHINE_INTERFACE_MONITORMODES", // 開模數
	"MACHINE_INTERFACE_MONITOR_TAKE_TIME",
	"MACHINE_INTERFACE_MONITOR_RSV08",
	"MACHINE_INTERFACE_MONITOR_RSV01", // 取樣模數
	"MACHINE_INTERFACE_MONITOR_RSV02", // 總不良品
	"MACHINE_INTERFACE_MONITOR_RSV09", // 總良品
	"MACHINE_INTERFACE_LASTMODETIME", // 上次週期時間
	"MACHINE_COMMUNICATION_IMM_OPENCLOSEMOLDPOSITION", //開模位置
	"MACHINE_COMMUNICATION_IMM_EJECTPOSITION", //頂出位置
};
char* u_pszStr_TakeTime[] = 
{
	"MACHINE_INTERFACE_MONITOR_TAKE_TIME",
	NULL,
};
char* u_pszStr_CycTime[] = 
{
	"MACHINE_INTERFACE_MONITOR_RSV08",
	NULL,
};

char*	u_pszSelectImagePath[] = 
{
	"unselected.bmp",
	"selected.bmp"
};

char*	u_pszPicker_ImagePath[] = 
{
	"res_tm640/pic/picker/Picker_Axis3.bmp",
	"res_tm640/pic/picker/Picker.bmp"
};

char*		u_ActionStepString[] =
{
	"PICKER_WITHOUT",
	"VM_AXISACT",
	"PICKER_WAIT",
	"PICKER_PERMIT",
	"PICKER_VALVE",
	"PICKER_LABEL",
	"PICKER_JUMP",
	"PICKER_DESCRIBE_AXIS_4",
	"VW_PUMP_TEST ",
	"MOTION_WRITE ",
	"VW_PARA_READ",
	"VW_PICKER_ADVANCE",
	"VW_PICKER_PILE",
	"ACTIONPOINT_START",
	"ACTIONPOINT_END",
	NULL
};
char*		u_ActionTypeString1[] =
{
	"PICKER_WITHOUT",
	"VW_HAP5_ROBOT_XAXIS",
	"VW_HAP5_ROBOT_YAXIS",
	"VW_HAP5_ROBOT_ZAXIS",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	NULL
};
char*		u_ActionTypeString2[] =
{
	"PCIKER_INNER_I_01",
	"PCIKER_INNER_I_02",
	"PCIKER_INNER_I_03",
	"PCIKER_INNER_I_04",
	"PCIKER_INNER_I_05",
	"PCIKER_INNER_I_06",
	"PCIKER_INNER_I_07",
	"PICKER_WITHOUT",
	NULL
};
char*		u_ActionTypeString3[] =
{
	"PICKER_INNER_O_01",
	"PICKER_INNER_O_02",
	"PICKER_INNER_O_03",
	"PICKER_INNER_O_04",
	"PICKER_INNER_O_05",
	"PICKER_INNER_O_06",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	NULL
};
char*		u_ActionTypeString4[] =
{
	"PICKER_WITHOUT",
	"PICKER_REMOTE_O_01",
	"PICKER_REMOTE_O_02",
	"PICKER_REMOTE_O_03",
	"PICKER_REMOTE_O_04",
	"PICKER_REMOTE_O_05",
	"PICKER_REMOTE_O_06",
	"PICKER_REMOTE_O_07",
	"PICKER_REMOTE_O_08",
	"PICKER_REMOTE_O_09",
	"PICKER_REMOTE_O_10",
	"PICKER_REMOTE_O_11",
	"PICKER_REMOTE_O_12",
	"PICKER_REMOTE_O_13",
	"PICKER_REMOTE_O_14",
	"PICKER_REMOTE_O_15",
	"PICKER_REMOTE_O_16",
	NULL
};
char*		u_ActionTypeString5[] =
{
	"PICKER_WITHOUT",
	"PICKER_LABEL_1",
	"PICKER_LABEL_2",
	"PICKER_LABEL_3",
	"PICKER_LABEL_4",
	"PICKER_LABEL_5",
	"PICKER_LABEL_6",
	"PICKER_LABEL_7",
	"PICKER_LABEL_8",
	"PICKER_LABEL_9",
	"PICKER_LABEL_10",
	NULL
};
char*		u_ActionTypeString6[] =
{
	"PICKER_WITHOUT",
	"PICKER_TIMING",
	"PICKER_COUNT",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	NULL
};
char*		u_ActionTypeString7[] =
{
	"PICKER_WITHOUT",
	"PICKER_REMOTE_I_01",
	"PICKER_REMOTE_I_02",
	"PICKER_REMOTE_I_03",
	"PICKER_REMOTE_I_04",
	"PICKER_REMOTE_I_05",
	"PICKER_REMOTE_I_06",
	"PICKER_REMOTE_I_07",
	"PICKER_REMOTE_I_08",
	"PICKER_REMOTE_I_09",
	"PICKER_REMOTE_I_10",
	"PICKER_REMOTE_I_11",
	"PICKER_REMOTE_I_12",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	"PICKER_WITHOUT",
	NULL
};
char*		u_ActionTypeString8[] =
{
	"PICKER_WITHOUT",
	"ACTIONPOINT_CHANNEL1",
	"ACTIONPOINT_CHANNEL2",
	"ACTIONPOINT_CHANNEL3",
	"ACTIONPOINT_CHANNEL4",
	"ACTIONPOINT_CHANNEL5",
	"ACTIONPOINT_CHANNEL6",
	NULL
};

long	NoColor[] =
{
	LBlue, // 青綠
	White,	// 白
};
/*---------------------------------------------------------------------------+
|           View Content - OverView                                          |
+---------------------------------------------------------------------------*/ 
/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	char	  pDataID[256];
	printf("OnChangeA\n");
	int value=0;
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl); // 獲取此taborder的元件
	if(pwnd!=NULL)
	{
		memset(pDataID, 0, sizeof(pDataID));
		pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
		printf("Get dbid1 =%s\n",pDataID);
		pwnd->GetPropValueT("value", &value, sizeof(value));
		printf("Get value=%d\n",value);
		SetDBValue(pDataID, value); // 數值寫入DB
		printf("Set %s=%d\n",pDataID,value);
	}
	

	return wIDControl;
}

/*---------------------------------------------------------------------------+
|  Function : OnCreateA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
  strncpy(u_szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
	// 取得 機構設定 3 or 5軸
	dw_MechType = (GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	u_PickerType = dw_MechType & MechWord;
	if(u_PickerType==MechType3)
		StandbyStepNum = 5;
	if(u_PickerType==MechType5)
		StandbyStepNum = 7;
	
	pwndStaticStepType        = pwndSender->FindControlFromName("StaticActionnum1");
	pwndStaticActionType      = pwndSender->FindControlFromName("StaticActionnum2");
	
	pwndTotalCount      	  = pwndSender->FindControlFromName("TotalCount");
	pwndTotalGoodCount        = pwndSender->FindControlFromName("TotalGoodCount");
	pwndTotalbadCount         = pwndSender->FindControlFromName("TotalbadCount");
	pwndTotalSampleCount      = pwndSender->FindControlFromName("TotalSampleCount");

	numStaticSpeed  		  = GetSpecialControlNum(pwndSender,	"StaticSpeed_"	,	"CtmStaticX2",	pwndStaticSpeed);
	
	iEditNo 	 			  = GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);
	iCheckBoxAct			  = GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);
	iStaticAct				= GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", pwndStaticAct);	//動作列文字 數量
	iJumpBtn			 	  = GetSpecialControlNum(pwndSender, "Button_", "CtmToolButton", pwndiJumpBtn);
	
	// 各軸座標 cjlee add 2019/5/1 下午 03:04:05
	pwndEditPostionX = pwndSender->FindControlFromName("EditXposition");
	pwndEditPostionY = pwndSender->FindControlFromName("EditYposition");
	pwndEditPostionZ = pwndSender->FindControlFromName("EditZposition");
	pwndEditPostionX2= pwndSender->FindControlFromName("EditX2position");
	pwndEditPostionY2= pwndSender->FindControlFromName("EditY2position");
	
	// 脫模位置
	pwndEJECTPOSITION= pwndSender->FindControlFromName("EJECTPOSITION");
	
	// 開始編輯
	pwndButtonStartEdit = pwndSender->FindControlFromName("ButtonStartEdit");
	
	// 彈跳視窗
	pwndBtnJump = pwndSender->FindControlFromName("BtnJump");
	pwndBtnClose = pwndSender->FindControlFromName("BtnClose");
	pwndBtnRight = pwndSender->FindControlFromName("BtnRight");
	pwndBtnLeft = pwndSender->FindControlFromName("BtnLeft");
	
	// 取得指標 取出補正 數值輸入
	for(int i = 0; i < sizeof(PickP_ADJ_String)/sizeof(PickP_ADJ_String[0]); i++ )
	{
		pwndPickP_ADJ[i] = pwndSender->FindControlFromName(PickP_ADJ_String[i]);
	} 
	// 取得指標 置物補正 數值輸入
	for(int i = 0; i < sizeof(PlaceP_ADJ_String)/sizeof(PlaceP_ADJ_String[0]); i++ )
	{
		pwndPlaceP_ADJ[i] = pwndSender->FindControlFromName(PlaceP_ADJ_String[i]);
	} 
	
	strncpy(szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
		
	long lSpeedValue=GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue/10;
	for(int i=0;i<lSpeedValue;i++)
	{
		if(pwndStaticSpeed[i]!=NULL)
		{
			pwndStaticSpeed[i]->SetPropValueT("bgc", 36256);
			pwndStaticSpeed[i]->UpdateAll();
			pwndStaticSpeed[i]->UpdateAll();
		}
	}
	for(int i=lSpeedValue;i<numStaticSpeed;i++)
	{
		if(pwndStaticSpeed[i]!=NULL)
		{
			pwndStaticSpeed[i]->SetPropValueT("bgc", 57051);
			pwndStaticSpeed[i]->UpdateAll();
			pwndStaticSpeed[i]->UpdateAll();
		}
	}
	//u_nSelectBoxCount = GetSpecialControlNum(pwndSender, (char*)NULL, "CtmSelectBox", &u_pwndSelectBox[0]);//2017-8-2 16:26:19
	u_nSelectBoxCount = GetSpecialControlNum(pwndSender, "SelectBox", "CtmSelectBox", &u_pwndSelectBox[0]);
	u_nDetectBoxCount = GetSpecialControlNum(pwndSender, "Detect", "CtmSelectBox", &u_pwndDetectBox[0]);
	printf("u_nSelectBoxCount=%d\n",u_nSelectBoxCount);
	printf("u_nDetectBoxCount=%d\n",u_nDetectBoxCount);
	CreateLEDSet();
	CreateDetectLEDSet();
	wStepNum =GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
	ActionStep();
	//CreateStringSet();
	AddTimer(&u_lTimer);	
	
	memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
	strcpy(g_szCurrentFormName, "Over.txt"); // cjlee 2019/4/30 下午 05:48:15
	printf("Over: NOW FORM = %s\n",g_szCurrentFormName);
	
	// 依據 機型選擇 3,5軸 顯示相對應Picker圖片
	pwndImg_Over_Picker = pwndSender->FindControlFromName("Img_Over_Picker");
	if(pwndImg_Over_Picker!=NULL)
		{
			pwndImg_Over_Picker->SetPropValueT("imagepath", u_pszPicker_ImagePath[u_PickerType]);
			pwndImg_Over_Picker->Show();
		}
	// 顯示 取物補正 及 置物補正 數值
	for(int i = 0; i < sizeof(PickP_ADJ_String)/sizeof(PickP_ADJ_String[0]); i++ )
	{
		if(pwndPickP_ADJ[i]!=NULL)
			ShowValue(pwndPickP_ADJ[i]);
	} 
	for(int i = 0; i < sizeof(PlaceP_ADJ_String)/sizeof(PlaceP_ADJ_String[0]); i++ )
	{
		if(pwndPlaceP_ADJ[i]!=NULL)
			ShowValue(pwndPlaceP_ADJ[i]);
	} 

	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnDestroyA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnDestroyA(CtmWnd* pwndSender)
{
	DeleteTimer(&u_lTimer);
}

/*---------------------------------------------------------------------------+
|  Function : OnKeyA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA wKey=%d\n",wKey);

	if(CtmConfig::GetInstance()->GetDemoFlag() == 1)
	{	
		if (wKey == _F1)	u_nClickNum ++;
		else 				u_nClickNum = 0;	
		
		if (wKey == _KEY_LANGUAGE) 	
		{
			;//printf("%d\n",g_nDemo);			
		}

		if (u_nClickNum >= 3)
		{
			if (g_nDemo == 0) 	g_nDemo = 1;
			else	g_nDemo = 0;			
		}
	}	

	if(wKey == _KEY_RIGHTFIELD)
	{
		g_WndLoginPrivildge = 0;
		Login();

		return	_NULL_KEY;
	}
	else if(wKey == _KEY_LEFTFIELD)
	{
		switch(g_wLoginFlag)
		{
			case 0://無用戶登入
				MsgBox(g_MultiLanguage["VW_NET_LOGININFIRST"],3);
				break;
			case 1://本地用戶登出
				MsgBox(g_MultiLanguage["VW_INET_SYSLOGOUT"],3);
				if(g_bMsgBoxFlag ==TRUE)
				{
					g_iPrivilege = 0; 		// 權限歸零

					if (g_pszUserID != NULL) //取消用戶登入名
					{
						delete []g_pszUserID;
						g_pszUserID = NULL;
					}
					
					g_wLoginFlag = 0; //用戶登入登出狀態置零
				}
				break;
			case 2://網絡用戶登出
				MsgBox(g_MultiLanguage["VW_INET_SYSLOGOUT"],3);
				if(g_bMsgBoxFlag ==TRUE)
				{
					g_iPrivilege = 0; 		// 權限歸零
					
					if (g_pszUserID != NULL) //取消用戶登入名
					{
						memset(g_szLogoutName, 0, sizeof(g_szLogoutName));
						strcpy(g_szLogoutName, g_pszUserID);
						
						delete []g_pszUserID;
						g_pszUserID = NULL;
					}
					
					g_wLoginFlag = 0; //用戶登入登出狀態置零
					SendMsg(MSG_SH_HMI_LOGOUT_REQUEST,0,0,NULL);	//登出人員信息發送
				}
				break;
			default:
				return _NULL_KEY;
		}
		return _NULL_KEY;
	}

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|  Function : OnShowA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnShowA(CtmWnd* pwndSender)
{

}

/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateA(CtmWnd* pwndSender)
{
	CreateLEDSet();
	CreateDetectLEDSet();
	char		szTemp[256] = "\0";
	WORD		wMotorStatus = 0;
	WORD		wFlage = TRUE;;
	

  	long lSpeedValue=GetDBValue("MACHINE_CONFIGURATION_PERCENTAGEOFSPEED").lValue/10;//2017/10/12 
  	if(lSpeedValue!=lSpeedValueOld) // 若數度值有改變
  	{
 		for(int i=0;i<lSpeedValue;i++)
 		{
 			if(pwndStaticSpeed[i]!=NULL)
 			{
		  		pwndStaticSpeed[i]->SetPropValueT("bgc", 36256);
		  		pwndStaticSpeed[i]->Update();
			}
  		}
		for(int i=lSpeedValue;i<numStaticSpeed;i++)
		{
			if(pwndStaticSpeed[i]!=NULL)
 			{
		  		pwndStaticSpeed[i]->SetPropValueT("bgc", 57051);
		  		pwndStaticSpeed[i]->Update();
		  	}
		}
			lSpeedValueOld=lSpeedValue;
		}
	
    if(g_ptaskpicker != NULL) // Picker
	{
		WORD		wNum = 0;
		wNum = sizeof(u_pszStrID)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID); // Update Data from 28	
		
//		static long Moldcount;
//		Moldcount = GetDBValue("MACHINE_INTERFACE_MONITORMODES").lValue;
//		printf("Moldcount=%d\n",Moldcount);
/*
		WORD		wNum = 0;
		wNum = sizeof(u_pszStr_TakeTime)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStr_TakeTime); // Update Data from 28	
		
		wNum = sizeof(u_pszStr_CycTime)/sizeof(char*);
		g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStr_CycTime); // Update Data from 28	
		*/		
	}	
	ActionStep();
	SetEditValue(pwndTotalCount);
	SetEditValue(pwndTotalGoodCount);
	SetEditValue(pwndTotalbadCount);
	SetEditValue(pwndTotalSampleCount);
	
{	/*-----------------------------------機械手座標----------------------------------*/
	SetEditValue(pwndEditPostionX);
	SetEditValue(pwndEditPostionY);
	SetEditValue(pwndEditPostionZ);
	SetEditValue(pwndEditPostionX2);
	SetEditValue(pwndEditPostionY2);	
	// 開模位置 脫模位置
	long OpenClosePos=GetDBValue("MACHINE_COMMUNICATION_IMM_OPENCLOSEMOLDPOSITION").lValue;
	//printf("OpenClosePos=%d\n",OpenClosePos);
	long EJECTPOSITION=GetDBValue("MACHINE_COMMUNICATION_IMM_EJECTPOSITION").lValue;
	//printf("EJECTPOSITION=%d\n",EJECTPOSITION);
	pwndEJECTPOSITION->SetPropValueT("value", EJECTPOSITION);
	pwndEJECTPOSITION->Update();
}	/*-----------------------------------機械手座標----------------------------------*/
{	/*-----------------------------------彈跳視窗鍵----------------------------------*/
	if(pwndBtnJump != NULL) // 上彈 Btn
		{
			u_BtnjumpCount++;
			if(u_BtnjumpCount>=3) // 跳動週期
				{
					//printf("Jump!\n");
					u_BtnjumpCount=0;
					b_BtnJump =!b_BtnJump;
					pwndBtnJump->SetPropValueT("upbitmap", BtnJumpImg[b_BtnJump]);
					pwndBtnJump->CreateA();
					pwndBtnJump->Update();	
				}
		}
	if(pwndBtnClose != NULL) // 下收 Btn
		{
			u_BtnjumpCount++;
			if(u_BtnjumpCount>=5) // 跳動週期
				{
					//printf("Jump!\n");
					u_BtnjumpCount=0;
					b_BtnJump =!b_BtnJump;
					pwndBtnClose->SetPropValueT("upbitmap", BtnCloseImg[b_BtnJump]);
					pwndBtnClose->CreateA();
					pwndBtnClose->Update();	
				}
		}	
}	/*-----------------------------------彈跳視窗鍵----------------------------------*/
	// check get_time & cyc_time update value // cjlee add 2019/1/24 下午 01:02:50
	/*
	WORD wGetTime =GetDBValue("MACHINE_INTERFACE_MONITOR_TAKE_TIME").lValue;
	if(wGetTime !=0)
		printf("GetTime=%d\n",wGetTime);
	WORD wCycTime =GetDBValue("MACHINE_INTERFACE_MONITOR_RSV08").lValue;
	if(wCycTime !=0)
		printf("CycTime=%d\n",wCycTime);
	*/
}
/*---------------------------------------------------------------------------+
|  Function : OnMouseDown()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL) 		return wIDControl;		
    return wIDControl;	
}

/*---------------------------------------------------------------------------+
|  Function : OnMouseUp()                       	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{

 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == NULL)	return wIDControl;
	
	if(pwnd == pwndBtnJump) // 彈跳視窗
		{
			::PutCommand("Over2.txt");
		}
	else if(pwnd == pwndBtnClose) // 關閉彈跳視窗
		{
			::PutCommand("Over.txt");
		}
	if(pwnd == pwndBtnRight) // 彈跳視窗 右開
		{
			if(u_PickerType==MechType5)
				::PutCommand("Over3.txt");
			else if(u_PickerType==MechType3)
				::PutCommand("Over3_Axis3.txt");
		}
	else if(pwnd == pwndBtnLeft) // 彈跳視窗 左關
		{
			::PutCommand("Over.txt");
		}
		
	else if(pwnd == pwndButtonStartEdit) // 開始編輯
		{
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "PG_0.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Over.txt");
				::PutCommand("PG_0.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
		}



  return wIDControl;	
}

/*---------------------------------------------------------------------------+
|  Function : SendCommand()                    	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SendCommand(int	CommandID)
{
	if(g_ptaskpicker != NULL)
 	{			
		g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	//printf("Send Command = %x\n", CommandID);
	}
}

/*---------------------------------------------------------------------------+
|  Function : ChangeImage()                  	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

void	ChangeImage(CtmWnd* pwnd, char* pszPath)
{
	char	szTempPath[256] = "\0";
	char		szTemp[256] = "\0";
	if(pwnd != NULL)
	{
		if(pszPath == NULL)
		{
			pwnd->SetPropValueT("imagepath", pszPath);
			pwnd->Update();	
			return;	
		}
		sprintf(szTemp,"%spicker/%s",u_szPath, pszPath);	
		pwnd->GetPropValueT("imagepath", szTempPath, sizeof(szTempPath));
		//printf("szTemp:%s, %s\n", szTemp, szTempPath);
		if(strcmp(szTempPath, szTemp) != 0)
		{
			pwnd->SetPropValueT("imagepath", szTemp);
			pwnd->Update();	
	  }
	}
}
void		SetEditValue(CtmWnd* pwnd)
{
	char	  pDataID[256];
	long long lTemp =0;
	if(pwnd != NULL)
	{
//		pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
		WORD wMoldNum =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED2").lValue;
		pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
//		if(pDataID != NULL && pDataID[0] != '\0')
//		{
//			lTemp = GetDBValue(pDataID).lValue * wMoldNum;
//		}
//		else
//		{
//			lTemp = (GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED6").lValue - GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED8").lValue)* wMoldNum;
//		}


		if( strcmp(pDataID,"MACHINE_INTERFACE_MONITORMODES")==0 ) // 總產量
			{
				lTemp = GetDBValue(pDataID).lValue * wMoldNum; // 單模產品數*開模數
			}
		else if( strcmp(pDataID,"MACHINE_INTERFACE_MONITOR_RSV02")==0 ) // 總不良品
			{
				lTemp = GetDBValue(pDataID).lValue * wMoldNum; // 單模產品數*開模數
			} 
		else if( strcmp(pDataID,"MACHINE_INTERFACE_MONITOR_RSV09")==0 ) // 總良品
			{
				lTemp = ( GetDBValue("MACHINE_INTERFACE_MONITORMODES").lValue - GetDBValue("MACHINE_INTERFACE_MONITOR_RSV02").lValue )* wMoldNum; // (總產量-總不良品)*開模數
			} 
		pwnd->SetPropValueT("value",lTemp);
		pwnd->UpdateAll();
	}	
}
void       CreateLEDSet()
{
	BOOL 	bSelected = FALSE;
//	u_dbEX_InputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE");	//cjlee changed 2019/2/19 下午 02:01:33
//	u_wEX_InputValue = static_cast<WORD>(u_dbEX_InputValue.lValue);
	u_dbEX_InputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALINPUTSTATE");	//cjlee changed 2019/10/29 上午 09:03:42
	u_wEX_InputValue = static_cast<DWORD>(u_dbEX_InputValue.lValue);

	u_dbEX_OutputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE");	//cjlee changed 2019/2/19 下午 02:01:33
	u_wEX_OutputValue = static_cast<WORD>(u_dbEX_OutputValue.lValue);	

	for(int i = 0; i < u_nSelectBoxCount; i++)
	{
		if(u_pwndSelectBox[i] != NULL)
		{
			if(i < PB_MAXNUM)     // PB
			{
//	   		if(_TestBit(u_wEX_InputValue, i+10))	bSelected = TRUE;	//cjlee changed 2019/2/19 下午 02:01:33
//		 		else 							bSelected = FALSE;
	   		if( (u_wEX_InputValue>>(i+1))&1 ) bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
		 		u_pwndSelectBox[i]->CreateA();
		 		u_pwndSelectBox[i]->Update();
		 		//cout<<"PB-"<<i<<"=="<<bSelected<<endl;
			}
			else                 //PC
			{
				if(_TestBit(u_wEX_OutputValue, i - PB_MAXNUM+10))	bSelected = TRUE;	//cjlee changed 2019/2/19 下午 02:01:33
		 		else 							bSelected = FALSE;
		 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
		 		u_pwndSelectBox[i]->CreateA();
		 		u_pwndSelectBox[i]->Update();
		 		//cout<<"PC-"<<i<<"=="<<bSelected<<endl;
			}
		}		
	}
	
//	if(u_pwndDetectBox[4]!=NULL)
//	{
//		if(_TestBit(u_wInputValue, 4))	// 機械手急停
//		{bSelected = TRUE;	}
//		else 							
//		{bSelected = FALSE;}
//		u_pwndDetectBox[4]->SetPropValueT("selected",bSelected);
//		u_pwndDetectBox[4]->CreateA();
//		u_pwndDetectBox[4]->Update();
//	}
}
void	CreateStringSet()
{
	char	pszString[128];
	int 	j;
	
	for( int i = 0; i < u_nSelectBoxCount; i++)
	{
		memset(u_acSelBoxString[i], 0, sizeof(u_acSelBoxString[i]));
		if(g_pFont->GetDefaultFont()->GetFontCode() == tmFT_CODE_TECH)
		{
		  	WORD *pwSrc;
      		char  szTemp[32];
      		WORD *pwDst;
      
      		pwDst   = (WORD *)(u_acSelBoxString[i]);
      		if(i < PB_MAXNUM)   //PB
     		{
     	 		sprintf(szTemp, "", i+1);
   	  		}
   	  		else                //PC
   	 		{
   	  			sprintf(szTemp, "", i+1-PB_MAXNUM);
   	  		}
      
     		int	j = 0;
      		while(szTemp[j] != 0)
	    	{
	     		*pwDst ++   = szTemp[j ++];
	    	}
	    	*pwDst++  = ' ';	    
	    	if(i < PB_MAXNUM)   //PB
	    	{
	   			sprintf(pszString, "PCIKER_INNER_I_%02d", i+1);
	    	}
	    	else                //PC
	    	{
	    		sprintf(pszString, "PICKER_INNER_O_%02d", i+1-PB_MAXNUM);
	    	}
	    
	    	pwSrc = (WORD *)g_MultiLanguage[pszString];
      		if(pwSrc != NULL)	
      		{
    	  		j = 0;
    	  		while(*pwSrc != 0)
    	     	*pwDst ++   = *pwSrc ++;
    		}
		  	*pwDst  = 0;
		}
	}
	
		for(int i = 0; i < u_nSelectBoxCount; i++)	
		{
	    	if(u_pwndSelectBox[i] != NULL)
	   	 	{
	    		u_pwndSelectBox[i]->SetPropValueT("text", u_acSelBoxString[i]);
	    	}
	  	}
}
void       CreateDetectLEDSet()
{
	BOOL 	bSelected = FALSE;
	u_dbEX_InputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE");	//cjlee changed 2019/2/19 下午 02:01:33
	u_wEX_InputValue = static_cast<DWORD>(u_dbEX_InputValue.lValue);
	
	u_dbOutputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE");	
	u_wOutputValue = static_cast<DWORD>(u_dbOutputValue.lValue);	

	if(u_wEX_InputValue!=u_wEX_InputValue_OLD)
	{
		if(u_pwndDetectBox[0] != NULL) // 夾具1
			{
				printf("%d=%d\n",1,(u_wEX_InputValue>>2)&1);
	   		if((u_wEX_InputValue>>2)&1)	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndDetectBox[0]->SetPropValueT("selected", bSelected);
		 		u_pwndDetectBox[0]->Update();
		 	}	
		if(u_pwndDetectBox[1] != NULL) // 夾具2
			{
				printf("%d=%d\n",2,(u_wEX_InputValue>>3)&1);
	   		if((u_wEX_InputValue>>3)&1)	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndDetectBox[1]->SetPropValueT("selected", bSelected);
		 		u_pwndDetectBox[1]->Update();
		 	}	
		if(u_pwndDetectBox[2] != NULL) // 吸盤1
			{
				printf("%d=%d\n",3,(u_wEX_InputValue>>4)&1);
	   		if((u_wEX_InputValue>>4)&1)	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndDetectBox[2]->SetPropValueT("selected", bSelected);
		 		u_pwndDetectBox[2]->Update();
		 	}	
		if(u_pwndDetectBox[3] != NULL) // 吸盤2
			{
				printf("%d=%d\n",4,(u_wEX_InputValue>>5)&1);
	   		if((u_wEX_InputValue>>5)&1)	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndDetectBox[3]->SetPropValueT("selected", bSelected);
		 		u_pwndDetectBox[3]->Update();
		 	}	
		if(u_pwndDetectBox[4] != NULL) // 機械手急停
			{
				printf("%d=%d\n",5,(u_wEX_InputValue>>7)&1);
	   		if((u_wEX_InputValue>>7)&1)	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndDetectBox[4]->SetPropValueT("selected", bSelected);
		 		u_pwndDetectBox[4]->Update();
		 	}	
		u_wEX_InputValue_OLD = u_wEX_InputValue;
	}
}

void	StepString()
{
	long  nStepType=GetDBValue("MACHINE_INTERFACE_PART1_WSTEPNUMBER").lValue;
	long  nActionType=GetDBValue("MACHINE_INTERFACE_PART2_WSTEPNUMBER").lValue; 
	if((pwndStaticStepType!=NULL)&&(pwndStaticActionType!=NULL))
	{
		if(nStepType < sizeof(u_ActionStepString) / sizeof(char*))
		{
			pwndStaticStepType->SetPropValueT("textID", u_ActionStepString[nStepType]);
			pwndStaticStepType->Update();
		}
		switch(nStepType)
		{
				case   1:
				if(nActionType < sizeof(u_ActionTypeString1) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString1[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   2:
				if(nActionType < sizeof(u_ActionTypeString2) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString2[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   3:
				if(nActionType < sizeof(u_ActionTypeString3) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString3[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   4:
				if(nActionType < sizeof(u_ActionTypeString4) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString4[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   5:
				if(nActionType < sizeof(u_ActionTypeString5) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString5[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
				
				case   6:
				if(nActionType < sizeof(u_ActionTypeString5) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString5[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   7:
				if(nActionType < sizeof(u_ActionTypeString6) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString6[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   8:
				if(nActionType < sizeof(u_ActionTypeString7) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString7[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
				
				case   9:
				if(nActionType < sizeof(u_ActionTypeString8) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString8[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   10:
				if(nActionType < sizeof(u_ActionTypeString8) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString8[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
				
				case   11:
				if(nActionType < sizeof(u_ActionTypeString1) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString1[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
					
				case   12:
				if(nActionType < sizeof(u_ActionTypeString1) / sizeof(char*))
				{
					pwndStaticActionType->SetPropValueT("textID", u_ActionTypeString1[nActionType]);
					pwndStaticActionType->Update();
				}
					break;
		}
	}
}

void	ActionStep()
{
	WORD 	wStepNum =GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
	WORD   wProgNum =GetDBValue("MACHINE_PROFILE_STEPNUM").lValue;
	if((wStepNum>0)&&(wStepNum<=wProgNum))
	{
		if(wStepNum!=wStepNumOld)
		{
			//printf("wStepNum=%d\n",wStepNum);
			if(wStepNum<=iCheckBoxAct/2+1) // 現在StepNum < 動作列表列數/2
			{
				ActionNum(iCheckBoxAct/2+1); //序號不變
				ActionName(iCheckBoxAct/2+1); //序號不變
//				for(int i=0;i<iCheckBoxAct;i++)
//				{
//					if(i==wStepNum-1)
//					{ 
//						//SetTempColor(pwndCheckBoxAct[i], 0, 0xAE69);
//						SetTempColor(pwndStaticAct[i], 0, 0xAE69);
//						SetTempColor(pwndEditNo[i], 0, 0xAE69);
//					}
//					else
//					{
//						//SetTempColor(pwndCheckBoxAct[i], 0x4BAE, StaticBgc);//blue
//						SetTempColor(pwndStaticAct[i], 0, StaticBgc);//blue
//						SetTempColor(pwndEditNo[i], 0, StaticBgc);//blue
//						// cjlee changed 0x4BAE -> 0 2019/4/30 上午 08:58:24
//					}
//				}
				/*if(wStepNumOld<=iCheckBoxAct/2+1)
				{
					if((wStepNumOld-1)%2==1)
					{
						SetTempColor(pwndCheckBoxAct[wStepNumOld-1], 0x4BAE, StaticBgc);//blue
						SetTempColor(pwndEditNo[wStepNumOld-1], 0x4BAE, StaticBgc);//blue
					}
					else
					{
						SetTempColor(pwndCheckBoxAct[wStepNumOld-1], 0x4BAE, StaticBgc); //grey
						SetTempColor(pwndCheckBoxAct[0], 0x4BAE, StaticBgc); //第一行重新刷新
						SetTempColor(pwndEditNo[wStepNumOld-1], 0x4BAE, StaticBgc); //grey
						SetTempColor(pwndEditNo[0], 0x4BAE, StaticBgc); //第一行重新刷新
					}
				}
				else
				{
					SetTempColor(pwndCheckBoxAct[3], 0x4BAE, StaticBgc); 
					SetTempColor(pwndCheckBoxAct[4], 0x4BAE, StaticBgc); 
					SetTempColor(pwndEditNo[3], 0x4BAE, StaticBgc); 
					SetTempColor(pwndEditNo[4], 0x4BAE, StaticBgc); 
				}*/
			}
			else if(wStepNum>wProgNum-iCheckBoxAct+iCheckBoxAct/2+1)
			{
				ActionNum(wProgNum-iCheckBoxAct+iCheckBoxAct/2+1); //序號不變
				ActionName(wProgNum-iCheckBoxAct+iCheckBoxAct/2+1); //序號不變
//				for(int i=0;i<iCheckBoxAct;i++)
//				{
//					if(i==iCheckBoxAct+wStepNum-wProgNum-1)
//					{ 
//						//SetTempColor(pwndCheckBoxAct[i], 0, 0xAE69);
//						SetTempColor(pwndStaticAct[i], 0, 0xAE69);	
//						SetTempColor(pwndEditNo[i], 0, 0xAE69);
//					}
//					else
//					{
//						//SetTempColor(pwndCheckBoxAct[i], 0x4BAE, StaticBgc);//blue
//						SetTempColor(pwndStaticAct[i], 0, StaticBgc);//blue
//						SetTempColor(pwndEditNo[i], 0, StaticBgc);//blue
//						// cjlee changed 0x4BAE -> 0 2019/4/30 上午 08:58:24
//					}
//				}
				/*if(wStepNumOld>=wProgNum-iCheckBoxAct+iCheckBoxAct/2+1)
				{
					if((wStepNumOld-1)%2==1)
					{
						if(wProgNum%2==0)	 //此步是為了總步數不一樣時，底色會變
						{
							SetTempColor(pwndCheckBoxAct[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
							SetTempColor(pwndEditNo[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
						}
						else
						{
							SetTempColor(pwndCheckBoxAct[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
							SetTempColor(pwndEditNo[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
						}
					}
					else
					{
						if(wProgNum%2==0)
						{	
							SetTempColor(pwndCheckBoxAct[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
							SetTempColor(pwndEditNo[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
						}
						else
						{
							SetTempColor(pwndCheckBoxAct[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
							SetTempColor(pwndEditNo[iCheckBoxAct+wStepNumOld-wProgNum-1], 0x4BAE, StaticBgc);
						}
					}
				}*/
			}
			else 
			{
				ActionNum(wStepNum); //序號變化
				ActionName(wStepNum); //序號變化
//				for(int i=0;i<iCheckBoxAct;i++)
//				{
//					if(i==iCheckBoxAct/2)
//					{ 
//						//SetTempColor(pwndCheckBoxAct[i], 0, 0xAE69);
//						SetTempColor(pwndStaticAct[i], 0, 0xAE69);
//						SetTempColor(pwndEditNo[i], 0, 0xAE69);
//					}
//					else
//					{
//						//SetTempColor(pwndCheckBoxAct[i], 0x4BAE, StaticBgc);//blue
//						SetTempColor(pwndStaticAct[i], 0, StaticBgc);//blue
//						SetTempColor(pwndEditNo[i], 0, StaticBgc);//blue
//						// cjlee changed 0x4BAE -> 0 2019/4/30 上午 08:58:24
//					}
//				}
				/*if(wStepNumOld==wProgNum-iCheckBoxAct+iCheckBoxAct/2+2)
				{
					SetTempColor(pwndCheckBoxAct[iCheckBoxAct/2+1], 0x4BAE, StaticBgc);
					SetTempColor(pwndEditNo[iCheckBoxAct/2+1], 0x4BAE, StaticBgc);
				}*/
			}
			wStepNumOld=wStepNum;
		}
	}
	if(wStepNum==0)
	{
		if(wStepNum!=wStepNumOld)
		{
			ActionNum(iCheckBoxAct/2+1);
			ActionName(iCheckBoxAct/2+1);
//			for(int i=0;i<iCheckBoxAct;i++)
//			{
//				if(i==wStepNum-1)
//				{ 
//					SetTempColor(pwndStaticAct[i], 0, 0xAE69);
//					SetTempColor(pwndEditNo[i], 0, 0xAE69);
//				}
//				else
//				{
//					SetTempColor(pwndStaticAct[i], 0, StaticBgc);//blue
//					SetTempColor(pwndEditNo[i], 0, StaticBgc);//blue
//					// cjlee changed 0x4BAE -> 0 2019/4/30 上午 08:58:24
//				}
//			}
			wStepNumOld=wStepNum;
		}
	}
}
void   ActionNum(WORD wStepNo)
{
	char	pNoDataID[256] = {NULL};
	char 	pNo[256]			={NULL}; 
	memset(pNo, 0 ,sizeof(pNo));
	int value=0;
	for(int i=0;i<iEditNo;i++)	//獲取序號
	{
		if(pwndEditNo[i]!=NULL)
		{		
			memset(pNoDataID, 0 ,sizeof(pNoDataID));
			sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+wStepNo-iCheckBoxAct/2);
			value = GetDBValue(pNoDataID).lValue;
			if(i+wStepNo-iCheckBoxAct/2 >StandbyStepNum)
			{
				memset(pNo, 0 ,sizeof(pNo));
				if(value-StandbyStepNum>0)
					sprintf(pNo,"%d",value-StandbyStepNum);
				else
					sprintf(pNo,"");
				pwndEditNo[i]->SetPropValueT("text", pNo);
			}
			else
			{
				pwndEditNo[i]->SetPropValueT("text", "WAIT");
			}
			pwndEditNo[i]->SetPropValueT("bgc",NoColor[((value)%2)]);
			pwndEditNo[i]->UpdateAll();
		}
	}	
}

void    ActionName(WORD wStepNo)
{
	printf("AcrionName wStepNo=%d\n",wStepNo);
	WORD 	wStepNum =GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
	WORD   wProgNum =GetDBValue("MACHINE_PROFILE_STEPNUM").lValue;
	
	char    szActTypeName[256];
	char    szActNumName[256];
	char    szActPara1[256];
	char    szActPara2[256];
	char    szActPara3[256];
	char    szActPara5[256];
	
	char	pNoDataID[256] = {NULL};
	int value;
	
	char 	pDataID[256];
	char 	pDataID2[256];
	char 	pDataID3[256];
	char 	pDataID4[256];
	char 	pDataID5[256];
	char 	pDataID6[256];
	char 	pDataID7[256];
	char 	pDataID8[256];
	char 	pDataID9[256];
	char 	pDataID10[256];
	char 	pDataID11[256];
	char 	pDataID12[256];
	// ↓ 字串合併顯示 cjlee add 	
	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
	char	str[1024]; // 顯示字串用
	char	path[1024]; // 顯示字串用
	char* str_1;
	char* str_2;
	char* str_3;
	char* str_4;
	char* str_5;
	char* str_6;
	char* str_7;
	char* str_8;
	char* str_9;
	char* str_10;
	char* str_11;
	char* str_12;
	// ↑ 字串合併顯示 cjlee add 
	WORD   wActType=0,wActNum=0;
	DWORD  wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
	for(int i=0;i<iCheckBoxAct;i++)
	{
		long ltemp = i+wStepNo-iCheckBoxAct/2;
		sprintf(szActTypeName,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",   ltemp);
		sprintf(szActNumName,"MACHINE_PROFILE_NUM%d_ACTION_NUM", 	 ltemp); 
		sprintf(szActPara1,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", ltemp); 
		sprintf(szActPara2,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", ltemp); 
		sprintf(szActPara3,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3", ltemp);
		sprintf(szActPara5,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", ltemp); 
		wActType   = GetDBValue(szActTypeName).lValue; 
		wActNum    = GetDBValue(szActNumName).lValue;
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		//pwndCheckBoxAct[i]->SetPropValueT("captionID", "");
		//pwndCheckBoxAct[i]->CreateA();   
		//pwndCheckBoxAct[i]->Update();
		//pwndCheckBoxAct[i]->UpdateAll(); 
		
		memset(pNoDataID, 0 ,sizeof(pNoDataID));
		sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",ltemp);
		value = GetDBValue(pNoDataID).lValue;
		if(pwndStaticAct[i] != NULL)
		{
			pwndStaticAct[i]->SetPropValueT("bgc",NoColor[((value)%2)]);
		}
		
		if(wStepNum<=iCheckBoxAct/2+1)
		{
			if( i == wStepNo-1)
			{
				SetTempColor(pwndStaticAct[i], 0, LGreen);	
				SetTempColor(pwndEditNo[i], 0, LGreen);
			}
		}
		else if(wStepNum>wProgNum-iCheckBoxAct+iCheckBoxAct/2+1)
		{
			if(i==iCheckBoxAct+wStepNum-wProgNum-1)
			{ 
				SetTempColor(pwndStaticAct[i], 0, LGreen);	
				SetTempColor(pwndEditNo[i], 0, LGreen);
			}
		}
		else
		{
			if(i==iCheckBoxAct/2)
				{ 
					//SetTempColor(pwndCheckBoxAct[i], 0, 0xAE69);
					SetTempColor(pwndStaticAct[i], 0, LGreen);
					SetTempColor(pwndEditNo[i], 0, LGreen);
				}
		}
//		if(pwndCheckBoxAct[i]!=NULL)
//		{
//			pwndCheckBoxAct[i]->SetPropValueT("captionID", u_ActionStepString[wActType]);
//			pwndCheckBoxAct[i]->CreateA();
//			pwndCheckBoxAct[i]->Update();
//		}
		memset(pDataID, 0 ,sizeof(pDataID));
		memset(pDataID2, 0 ,sizeof(pDataID2));
		memset(pDataID3, 0 ,sizeof(pDataID3));
		memset(pDataID4, 0 ,sizeof(pDataID4));
		memset(pDataID5, 0 ,sizeof(pDataID5));
		memset(pDataID6, 0 ,sizeof(pDataID6));
		memset(pDataID7, 0 ,sizeof(pDataID7));
		memset(pDataID8, 0 ,sizeof(pDataID8));
		memset(pDataID9, 0 ,sizeof(pDataID9));
		memset(pDataID10,0 ,sizeof(pDataID10));
		memset(pDataID11,0 ,sizeof(pDataID11));
		memset(pDataID12,0 ,sizeof(pDataID12));
		switch(wActType)
		{
			case 1:		//軸動作
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				switch(wActNum)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS");
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS");
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS");
						break;
					case 4:
						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS");
						break;
					case 5:
						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS");
						break;
					default:
						break;
				}
				if(wActNum <= 5)
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1");
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2");
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				else if(wActNum == 6)
				{
					if(wActPara2)
						sprintf(pDataID4,"PICKER_REMOTE_O_14");
					else
						sprintf(pDataID4,"PICKER_REMOTE_O_13");
					if(wActPara3)
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
					else
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				}
				break;
			case 2:		//等待
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_WAIT");
				sprintf(pDataID5,"PCIKER_INNER_I_0%d",wActNum);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 3:		//允許
				
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_INNER_O_0%d",wActNum);
				if(wActPara1)
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 4:		//閥門
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_VALVE");
				sprintf(pDataID5,"DLG_DATETIME_COLON");
				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",wActNum);
				if(wActPara1)
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
				if(wActPara2) // "檢測"
					sprintf(pDataID8,"VW_PUMP_TEST");
//				if(wActPara2)
//				{
//					sprintf(pDataID8,"PICKER_ACTTIME");
//					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
//					sprintf(pDataID10,"VW_INJPRO_SECOND");
//				}
				
				break;
			case 5:		//標籤
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(wActNum == 1)
					sprintf(pDataID4,"ACTIONPOINT_START");
				else
				{
					sprintf(pDataID4,"PICKER_LABEL_%d",wActNum);
				}
				break;
			case 6:		//跳轉
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(wActNum == 1)
					sprintf(pDataID4,"ACTIONPOINT_END");
				else
				{
					sprintf(pDataID4,"PICKER_JUMP");
					sprintf(pDataID5,"PICKER_LABEL_%d",wActNum);
				}
				break;
			case 8:		//測試
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"VW_PUMP_TEST");
				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",wActNum);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 12:	//堆疊
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(wActNum)
					sprintf(pDataID4,"VW_PID_GROUP%d",wActNum);
				else
					sprintf(pDataID4,"VW_PICKER_NULL");
				sprintf(pDataID5,"VW_PICKER_PILE");
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 13:	//子程序
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_SUBTECH_1");
				switch(wActNum)
				{
					case 0:
						sprintf(pDataID5,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID5,"PICKER_PROD_BAD");
						break;
					case 2:
						sprintf(pDataID5,"PICKER_PROD_SAMPLE");
						break;
					case 3:
						sprintf(pDataID5,"PICKER_PROD_TEST");
						break;
					case 4:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					case 5:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					default:
						break;
				}
				if(wActPara1)
					sprintf(pDataID6,"ACTIONPOINT_START");
				else
					sprintf(pDataID6,"ACTIONPOINT_END");
				sprintf(pDataID7,"PICKER_SUBTECH_2");
				sprintf(pDataID8,"%2d",wActPara2);
				break;
			default: 
				break;	
		}

		// ↓ 字串合併顯示 cjlee add 
	{
		index_1 = g_MultiLanguage.GetStrKeyIndex(pDataID);
		index_2 = g_MultiLanguage.GetStrKeyIndex(pDataID2);	
		index_3 = g_MultiLanguage.GetStrKeyIndex(pDataID3);	
		index_4 = g_MultiLanguage.GetStrKeyIndex(pDataID4);
		index_5 = g_MultiLanguage.GetStrKeyIndex(pDataID5);	
		index_6 = g_MultiLanguage.GetStrKeyIndex(pDataID6);	
		index_7 = g_MultiLanguage.GetStrKeyIndex(pDataID7);
		index_8 = g_MultiLanguage.GetStrKeyIndex(pDataID8);	
		index_9 = g_MultiLanguage.GetStrKeyIndex(pDataID9);	
		index_10 = g_MultiLanguage.GetStrKeyIndex(pDataID10);
		index_11 = g_MultiLanguage.GetStrKeyIndex(pDataID11);	
		index_12 = g_MultiLanguage.GetStrKeyIndex(pDataID12);	
		
		if (index_1<0)
			index_1 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_2<0)
			str_2 = pDataID2;
		else
			str_2 = g_MultiLanguage[index_2];
		if (index_3<0)
			index_3 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_4<0)
			index_4 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_5<0)
			index_5 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_6<0)
			str_6 = pDataID6;
		else
			str_6 = g_MultiLanguage[index_6];
		if (index_7<0)
			index_7 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_8<0)
			str_8 = pDataID8;
		else
			str_8 = g_MultiLanguage[index_8];
		if (index_9<0)
			index_9 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_10<0)
			index_10 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_11<0)
			str_11= pDataID11;
		else
			str_11 = g_MultiLanguage[index_11];
		if (index_12<0)
			index_12 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		//printf("Line%d = %d  %d  %d %d  %d  %d %d  %d  %d %d  %d  %d\n",i+1+No1,index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12);
		
		str_1 = g_MultiLanguage[index_1];
		//str_2 = g_MultiLanguage[index_2];
		str_3 = g_MultiLanguage[index_3];
		str_4 = g_MultiLanguage[index_4];
		str_5 = g_MultiLanguage[index_5];
		//str_6 = g_MultiLanguage[index_6];
		str_7 = g_MultiLanguage[index_7];
		//str_8 = g_MultiLanguage[index_8];
		str_9 = g_MultiLanguage[index_9];
		str_10 = g_MultiLanguage[index_10];
		//str_11 = g_MultiLanguage[index_11];
		str_12 = g_MultiLanguage[index_12];
		
		memset(str,0,sizeof(str));
		//printf("sl_1=%d, sl_2=%d, sl_3=%d, sl_4=%d, sl_5=%d, sl_6=%d, sl_7=%d, sl_8=%d, sl_9=%d, sl_10=%d, sl_11=%d, sl_12=%d\n",strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
		strncat(str, str_1, strlen(str_1));
		strncat(str, str_2, strlen(str_2));
		strncat(str, str_3, strlen(str_3));
		strncat(str, str_4, strlen(str_4));
		strncat(str, str_5, strlen(str_5));
		strncat(str, str_6, strlen(str_6));
		strncat(str, str_7, strlen(str_7));
		strncat(str, str_8, strlen(str_8));
		strncat(str, str_9, strlen(str_9));
		strncat(str, str_10, strlen(str_10));
		strncat(str, str_11, strlen(str_11));
		strncat(str, str_12, strlen(str_12));
		//printf("Line%d = %s\n",i+1+No1-5,str);
		//printf("strlen=%d\n",strlen(str));
		//strncpy(path, str, strlen(str)+1);
		//path[strlen(str)]= '\0';
		//printf("path=%d\n",path);
		
		//printf("pwndStaticAct[%d]=%s\n",i,str);

		if(pwndStaticAct[i] != NULL)
		{
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Show();
			//pwndStaticAct[i]->Update();
			pwndStaticAct[i]->UpdateAll();
		}
		//pwndCheckBoxAct[i]->Update();
	}
		// ↑ 字串合併顯示 cjlee add 
		
//		pwndCheckBoxAct[i]->SetPropValueT("captionID1", pDataID);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID2", pDataID2);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID3", pDataID3);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID4", pDataID4);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID5", pDataID5);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID6", pDataID6);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID7", pDataID7);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID8", pDataID8);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID9", pDataID9);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID10", pDataID10);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID11", pDataID11);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID12", pDataID12);
//		pwndCheckBoxAct[i]->CreateA();
//		pwndCheckBoxAct[i]->Update();
//		pwndCheckBoxAct[i]->UpdateAll();
		
		/*switch(wActType)
		{
			case ACT_NULL:
				if(pwndCheckBoxAct[i]!=NULL)
				{
					pwndCheckBoxAct[i]->SetPropValueT("captionID", "VW_PICKER_NULL");
					pwndCheckBoxAct[i]->CreateA();
					pwndCheckBoxAct[i]->Update();
				}
				break;	
			case ACT_AXIS:
				ActionAXIS(pwndCheckBoxAct[i],wActNum);
				break;	
			case ACT_VALVE:
				ActionVALVE(pwndCheckBoxAct[i],wActNum);
				break;
			case ACT_PILE:
				ActionPILE(pwndCheckBoxAct[i],wActNum);
				break;
			case ACT_WAIT:
				ActionWAIT(pwndCheckBoxAct[i],wActNum);
				break;
			case ACT_SIGN:
				ActionSIGN(pwndCheckBoxAct[i],wActNum);
				break;
			case ACT_ASSIT:
				ActionASSIT(pwndCheckBoxAct[i],wActNum);
				break;
			case ACT_JUMP:
				ActionJUMP(pwndCheckBoxAct[i],wActNum);
				break;
			default:
				break;
		}*/
	}
}


void    SetTempColor(CtmWnd* pwnd,FGC fgcTemp,BGC bgcTemp)
{
    if(pwnd != NULL)
    {
        pwnd->SetFGColor(fgcTemp);
        pwnd->SetBGColor(bgcTemp);
        pwnd->Update();
    }
}

/*-------------------軸動作---------------------------*/
void    ActionAXIS(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID1", "ACTIONPOINT_CHANNEL1");
		pwnd->CreateA();
		pwnd->Update();
	}
}

/*-------------------吸夾---------------------------*/
void    ActionVALVE(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID", "ACTIONPOINT_CHANNEL2");
		pwnd->CreateA();
		pwnd->Update();
	}
}
/*-------------------堆疊---------------------------*/
void    ActionPILE(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID", "ACTIONPOINT_CHANNEL3");
		pwnd->CreateA();
		pwnd->Update();
	}
}

/*-------------------等待---------------------------*/
void    ActionWAIT(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID", "ACTIONPOINT_CHANNEL4");
		pwnd->CreateA();
		pwnd->Update();
	}
}

/*-------------------信號---------------------------*/
void    ActionSIGN(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID", "ACTIONPOINT_CHANNEL5");
		pwnd->CreateA();
		pwnd->Update();
	}
}

/*-------------------輔助---------------------------*/
void    ActionASSIT(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID", "ACTIONPOINT_CHANNEL6");
		pwnd->CreateA();
		pwnd->Update();
	}
}

/*-------------------跳轉---------------------------*/
void    ActionJUMP(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("captionID", "ACTIONPOINT_CHANNEL7");
		pwnd->CreateA();
		pwnd->Update();
	}
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
		{
			printf("Toolbar =%d\n",wParam);
		}
}
//void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
//{
//	char sz[256];
//	WORD	x_pos = 0, y_pos = 0;
//	switch(message)
//	{
//		case MSG_MOUSELUP:
//			x_pos = wParam >> 16;
//			y_pos = wParam & 0x0000FFFF;
//			for(int i=0 ;i<iJumpBtn;i++)
//			{
//				TouchButtonEvent(pwndiJumpBtn[i], x_pos, y_pos); 
//			}
//       		break;
//       case MSG_MOUSELDOWN: 
//        	break;
//		default:
//			break;
//	}
//}
//static inline	void	TouchButtonEvent(CtmWnd* pwnd, int nPosX, int nPosY)
//{
//	RECT    rect;
//	char    szImagePath[256];
//	if(pwnd != NULL)
//	{
//		rect = pwnd->GetWindowRECT();
//		if((nPosX < rect.right && nPosX > rect.left)&& (nPosY < rect.bottom && nPosY > rect.top))
//		{
//			if(u_Jump)
//			{
//				sprintf(szImagePath, "%sButtonDown.bmp", szPath);
//				pwnd->SetPropValueT("upbitmap", szImagePath);
//				pwnd->CreateA();
//				pwnd->UpdateAll();
//				u_Jump =FALSE;
//			}
//		}
//	}	
//}

/*---------------------------------------------------------------------------+
|  Function : ShowValue(CtmWnd* pwnd)             													 |
|       		: 顯示元件 pwnd 輸入數值																				 |
+---------------------------------------------------------------------------*/
void    ShowValue(CtmWnd* pwnd) // 顯示元件 pwnd 輸入數值	
{
	char	  pDataID[256];
	int value=0;
	
	if(pwnd!=NULL)
	{
		memset(pDataID, 0, sizeof(pDataID));
		pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
		value = (GetDBValue(pDataID).lValue);
		
		pwnd->SetPropValueT("value", value);
		pwnd->Update();
	}
}