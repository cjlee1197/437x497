/*===========================================================================+
|  Class    : ViewPG_2 library                           		                 |
|  Task     : ViewPG_2 library action source file      		                   |
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
#include		"ViewSUB_EditWindow.h"
#include    "../database.h"
#include    "../init.h"
#include    "../selectedit.h"
#include    "../tmconfig.h"
//#include    "../lib/hmi_control_data/tmControl_CurveExData.h"
#include    "../dialog.h"
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word

// 動作類型
#define		Action_Axis				1  // 軸動作
#define		Action_Wait				2  // 等待
#define		Action_Permit			3  // 允許

#define		Action_Valve			4	 // 閥門
#define		Action_Tag				5  // 標籤
#define		Action_Goto				6  // 跳轉
#define		Action_Detect			8  // 檢測
#define		Action_Pile				12 // 堆疊
#define		Action_Sub				13 // 副程式
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸

BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次

CtmWnd*		pwndimageBackGround1	= NULL;
CtmWnd*		pwndStaticTITLE2	= NULL;
CtmWnd*		pwndSelectEditNUM	= NULL;
CtmWnd*		pwndSelectGroupNum	= NULL;
CtmWnd*		pwndDetectMode			= NULL;
CtmWnd*		pwndSTRT_STOP				= NULL;
CtmWnd*		pwndON_OFF					= NULL;
CtmWnd*		pwndValveCheck			= NULL;
CtmWnd*		pwndOPEN_MODE				= NULL;
CtmWnd*		pwndEND_MODE				= NULL; // 軸動作 結束模式
CtmWnd*		pwndEditEnd1				= NULL; // 軸動作 結束模式 輸入1
CtmWnd*		pwndEditEnd2				= NULL; // 軸動作 結束模式 輸入2
CtmWnd*		pwndStaticEnd1			= NULL; // 軸動作 結束模式 文字1
CtmWnd*		pwndStaticEnd2			= NULL; // 軸動作 結束模式 文字2
CtmWnd*		pwndEditPos_X				= NULL; // 軸動作 點到點 X座標
CtmWnd*		pwndEditPos_Y				= NULL; // 軸動作 點到點 Y座標
CtmWnd*		pwndEditPos_Z				= NULL; // 軸動作 點到點 Z座標
CtmWnd*		pwndPerAdv					= NULL; // 軸動作 點到點 提前百分比
CtmWnd*		pwndPerEnd					= NULL; // 軸動作 點到點 末段百分比
CtmWnd*		pwndEditACTIONPARA1	= NULL;
CtmWnd*		pwndEditACTIONPARA2	= NULL;
CtmWnd*		pwndEditACTIONPARA3	= NULL;
CtmWnd*		pwndEditACTIONPARA5	= NULL;
CtmWnd*		pwndEditACTIONPARA6	= NULL;
CtmWnd*		pwndStaticACTIONPARA2	= NULL;
CtmWnd*		pwndStaticACTIONPARA3	= NULL;
CtmWnd*		pwndBtnCancel	= NULL;

CtmWnd*		pwndStaticPreDistance	= NULL;
CtmWnd*		pwndEditSet_PreDistance	= NULL;
CtmWnd*		pwndSelectEditManualType	= NULL;
CtmWnd*		pwndButton_GetPosi	= NULL;

CtmWnd*		pwndBtnWait	= NULL;
CtmWnd*		pwndBtnPermit	= NULL;
CtmWnd*		pwndBtnTag	= NULL;
CtmWnd*		pwndBtnGoto	= NULL;
CtmWnd*		pwndBtnValve	= NULL;
CtmWnd*		pwndBtnDetect	= NULL;


int			iNumButtonAct =0;
int			SelectNo	=0;
long		ActionType  =0;
int			EditNUM=0,GroupNUM=0,ACTIONPARA1=0,ACTIONPARA2=0,ACTIONPARA3=0,ACTIONPARA5=0,ACTIONPARA6=0;
int			ACTIONPARA7=0,ACTIONPARA8=0,ACTIONPARA67=0;
int			iDetectMode=0,iSTRTSTOP=0,iON_OFF=0,iValveCheck=0,iOpenMode=0,iEndMode=0;
int		AxisXOld =0,AxisYOld =0,AxisZOld =0,AxisX2Old =0,AxisY2Old =0,AxisCOld=0,iSelectIndexOld=0xFFFF,AxisXNew =0,AxisYNew =0,AxisX2New =0,AxisY2New =0,AxisZNew =0,AxisCNew =0,iSelectIndex=0xFFFF;
long		u_SubGroup =0; // 副程式號碼

int     iEditNo=0,iCheckBoxAct=0,iEditACTION=0,iSelectEditACTION=0,iStaticACTION=0,iStaticEditNo;
int 	iStaticAct=0;
int 	No1=0,No=0;
int		Cleanhelp =0;
int		MaxDBNum = 100;
bool	m_bEnterKey = FALSE;
char 	pNo[256]	={NULL}; 
char 	pStaticNo[256]	={NULL}; 
int		StandbyStepNum=0; // 待機點步驟數量 3軸-5步 5軸-7步

CtmWnd*		pwndEditNo[32] 	={NULL}; 		//
CtmWnd*		pwndStaticEditNo[32] 	={NULL}; 
CtmWnd*		pwndCheckBoxAct[32] ={NULL}; 	//
CtmWnd*		pwndStaticAct[32] ={NULL}; 	//動作列文字
CtmWnd*		pwndButtonPageDown	= NULL;
CtmWnd*		pwndButtonPageUp	= NULL;
CtmWnd*		pwndSelectEditACTIONTYPE	= NULL;
CtmWnd*		pwndSelectEditACTIONNUM	= NULL;
CtmWnd*		pwndEditACTIONNUM	= NULL;
CtmWnd*		pwndEditSelectNo	= NULL;
CtmWnd*		pwndEditACTION[32] ={NULL};
CtmWnd*		pwndSelectEditACTION[32] ={NULL};
CtmWnd*		pwndStaticACTION[32] ={NULL};
CtmWnd*		pwndBtnSave	= NULL;
CtmWnd*		pwndButtonCANEL	= NULL;
CtmWnd*		pwndButtonInsert	= NULL;
CtmWnd*		pwndButtonDelete	= NULL;
CtmWnd*		pwndButtonEditNUM	= NULL;
CtmWnd*		pwndStaticACTIONPARA1	= NULL;

CtmWnd*		pwndButtonAct[32] 	={NULL};

BOOL 			b_ManualMode = FALSE;
BOOL TF[] =
{
	FALSE,
	TRUE
};

char*	u_pszAxisName[] =
{
	"VW_PICKER_NULL",
	"VW_HAP5_ROBOT_XAXIS",
	"VW_HAP5_ROBOT_YAXIS",
	"VW_HAP5_ROBOT_ZAXIS",
	"VW_HAP5_ROBOT_CAXIS",
	"VW_HAP5_ROBOT_X2AXIS",
	"VW_HAP5_ROBOT_Y2AXIS",
};

char*	Str_ManualMode[] =
{
	"PICKER_CONTINUE",
	"PICKER_POINTMOVE",
};

char*	Str_Axis[] =
{
	"VW_PICKER_NULL",
	"VW_HAP5_ROBOT_XAXIS",
	"VW_HAP5_ROBOT_YAXIS",
	"VW_HAP5_ROBOT_ZAXIS",
	"VW_HAP5_ROBOT_X2AXIS",
	"VW_HAP5_ROBOT_Y2AXIS",
	"VW_HAP5_ROBOT_CAXIS",
};
char*	Str_Pile[] =
{
	"VW_PICKER_NULL",
	"VW_PID_GROUP1",
	"VW_PID_GROUP2",
	"VW_PID_GROUP3",
	"VW_PID_GROUP4",
};
char*	Str_Sub[] =
{
	"VW_PICKER_NULL",
	"PICKER_PROD_BAD",
	"PICKER_PROD_SAMPLE",
	"PICKER_PROD_TEST",
	"PICKER_FUNC_SUBTECH",
	"PICKER_FUNC_SUBTECH2",
};



char* Str_InnerI[] =
{
	"VW_PICKER_NULL",
	"PCIKER_INNER_I_01",
	"PCIKER_INNER_I_02",
	"PCIKER_INNER_I_03",
	"PCIKER_INNER_I_04",
	"PCIKER_INNER_I_05",
	"PCIKER_INNER_I_06",
	"PCIKER_INNER_I_07",
};
char* Str_InnerO[] =
{
	"VW_PICKER_NULL",
	"PICKER_INNER_O_01",
	"PICKER_INNER_O_02",
	"PICKER_INNER_O_03",
	"PICKER_INNER_O_04",
	"PICKER_INNER_O_05",
	"PICKER_INNER_O_06",
	"PICKER_INNER_O_07",
};

char*	Str_RemoteI[] =
{
	"VW_PICKER_NULL",
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
	"PICKER_REMOTE_I_13",
	"PICKER_REMOTE_I_14",
	"PICKER_REMOTE_I_15",
	"PICKER_REMOTE_I_16",
};
char*	Str_RemoteO[] =
{
	"VW_PICKER_NULL",
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
};
char*	Str_Label[] =
{
	"VW_PICKER_NULL",
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
};

char*	Str_Group[] =
{
	"PICKER_WITHOUT",
	"PICKER_GROUP_1",
	"PICKER_GROUP_2",
	"PICKER_GROUP_3",
	"PICKER_GROUP_4",
	"PICKER_GROUP_5",
	"PICKER_GROUP_6",
	"PICKER_GROUP_7",
	"PICKER_GROUP_8",
};

char*	Str_Detect[] =
{
	"PICKER_SGL_DETC", // 區域檢測
	"PICKER_PER_DETC", // 單點撿測
};
char*	Img_Btn[] =
{
	"res_tm640/pic/EditBtnDown.bmp", // 灰按鈕
	"res_tm640/pic/EditBtnUp.bmp", // 亮按鈕
};

char*	Str_STRTSTOP[] =
{
	"PICKER_STOP", // 停止
	"PICKER_STRT", // 開始
};
char*	Str_ON_OFF[] =
{
	"PICKER_DESCRIBE_AXIS_6", // 關閉
	"PICKER_DESCRIBE_AXIS_5", // 打開
};

char*	Str_STRT_END[] =
{
	"ACTIONPOINT_END", 	 // 結束
	"ACTIONPOINT_START", // 開始
};

char*	Str_ValveCheck[] =
{
	"PICKER_NOCHECK", // 不檢測
	"PICKER_CHECK", 	// 檢測
};

char*	Str_OpenMode[] =
{
	"PICKER_STANDARD", 	// 標準
	"PICKER_OPNFOLLOW", // 開模追隨
	"PICKER_OPNMIDDWN", // 中途下降
};
char*	Str_EndMode[] =
{
	"PICKER_ADV_END", // 不檢測
	"PICKER_SEC_SPD", // 檢測
};
char*	Str_StaticEnd1[] =
{
	"PICKER_ADV_DIST", // 提前
	"PICKER_CHG_DIST", // 變速
};
char*	Str_StaticEnd2[] =
{
	"PICKER_END_DIST", // 末段
	"PICKER_SPD_SEC", // 二段
};
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
	//u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	// 機型選擇 編碼器選擇
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	if(u_PickerType==MechType3)
		StandbyStepNum = 5;
	if(u_PickerType==MechType5)
		StandbyStepNum = 7;
	
	iEditNo 	 			= GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);					//
	iStaticEditNo 	 			= GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", pwndStaticEditNo);	//序��列��量
	iCheckBoxAct				= GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);	//
	iStaticAct				= GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", pwndStaticAct);	//雄釬蹈杅講 動作列數量
	iEditACTION				= GetSpecialControlNum(pwndSender, "EditACTION", "CtmEditX1", pwndEditACTION);
	iSelectEditACTION				= GetSpecialControlNum(pwndSender, "SelectEditACTION", "CtmSelectEdit", pwndSelectEditACTION);
	iStaticACTION				= GetSpecialControlNum(pwndSender, "StaticACTION", "CtmStaticX2", pwndStaticACTION);
	//No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40").lValue-5;	
	No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50").lValue;	
	SelectNo		=GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue;		//祭紬 步驟
	ActionType = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52").lValue;			//濬倰 類型
	u_SubGroup = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED47").lValue; // 讀取副程式編號
	printf("1 u_SubGroup%d SelectNo%d ActionType=%d\n",u_SubGroup,SelectNo,ActionType);
	
	if(No1<0)
	{
		No1=0;
	}	
	else if(No1>10)
	{
		No1=10;
	}											// 0
		
	//SetDBValue("MACHINE_PROFILE_STEPNUM", 0);		//
	pwndButtonPageDown	 = pwndSender->FindControlFromName("buttonDOWNTWENTY");
	pwndButtonPageUp	 = pwndSender->FindControlFromName("buttonUPTWENTY");
	pwndEditSelectNo	= pwndSender->FindControlFromName("EditSelectNo");
	pwndSelectEditACTIONTYPE	= pwndSender->FindControlFromName("SelectEditTYPE");
	pwndEditACTIONNUM	= pwndSender->FindControlFromName("Edit1NUM");
	pwndButtonInsert		= pwndSender->FindControlFromName("ButtonInsert");
	pwndButtonDelete		= pwndSender->FindControlFromName("ButtonDelete");
	
	pwndButtonEditNUM		= pwndSender->FindControlFromName("ButtonEditNUM");
	pwndStaticACTIONPARA1	= pwndSender->FindControlFromName("StaticACTIONPARA1");
	char temp[128];
	char szTemp[256];
	iNumButtonAct 	 = GetSpecialControlNum(pwndSender, "ButtonAct", "CtmToolButton", pwndButtonAct);
	
	for (int i = 0; i < iEditNo; i++)	//
	{
		No = i+1+No1;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		if(pwndEditNo[i] != NULL)
		{
			pwndEditNo[i]->SetPropValueT("text",pNo);
			pwndEditNo[i]->Update();
		}
		if(No<=StandbyStepNum)
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",50712);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
		}
		else
		{
			memset(pStaticNo, 0 ,sizeof(pStaticNo));
			sprintf(pStaticNo,"%d",No-StandbyStepNum);
			pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
			pwndStaticEditNo[i]->Update();
		}
		if(pwndEditNo[i] != NULL)			//
		{
			if(No==SelectNo)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",36256);
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
			}
		}
	}
	UpdateText();
	
	
	pwndimageBackGround1= pwndSender->FindControlFromName("imageBackGround1");
	pwndStaticTITLE2	= pwndSender->FindControlFromName("StaticTITLE2");
	pwndSelectEditNUM	= pwndSender->FindControlFromName("SelectEditNUM");
	pwndSelectGroupNum	= pwndSender->FindControlFromName("SelectGroupNum");
	pwndDetectMode			= pwndSender->FindControlFromName("DetectMode");
	pwndSTRT_STOP				= pwndSender->FindControlFromName("STRT_STOP");
	pwndON_OFF					= pwndSender->FindControlFromName("ON_OFF");
	pwndValveCheck			= pwndSender->FindControlFromName("ValveCheck");
	pwndOPEN_MODE				= pwndSender->FindControlFromName("OPEN_MODE");
	pwndEND_MODE				= pwndSender->FindControlFromName("END_MODE");
	pwndEditEnd1				= pwndSender->FindControlFromName("EditEnd1");
	pwndEditEnd2				= pwndSender->FindControlFromName("EditEnd2");
	pwndStaticEnd1			= pwndSender->FindControlFromName("StaticEnd1");
	pwndStaticEnd2			= pwndSender->FindControlFromName("StaticEnd2");
	pwndEditPos_X				= pwndSender->FindControlFromName("EditPos_X");
	pwndEditPos_Y				= pwndSender->FindControlFromName("EditPos_Y");
	pwndEditPos_Z				= pwndSender->FindControlFromName("EditPos_Z");
	pwndPerAdv					= pwndSender->FindControlFromName("PerAdv");
	pwndPerEnd					= pwndSender->FindControlFromName("PerEnd");
	pwndEditACTIONPARA1	= pwndSender->FindControlFromName("EditACTIONPARA1");
	pwndEditACTIONPARA2	= pwndSender->FindControlFromName("EditACTIONPARA2");
	pwndEditACTIONPARA3	= pwndSender->FindControlFromName("EditACTIONPARA3");
	pwndEditACTIONPARA5	= pwndSender->FindControlFromName("EditACTIONPARA5");
	pwndEditACTIONPARA6	= pwndSender->FindControlFromName("EditACTIONPARA6");
	pwndStaticACTIONPARA2	= pwndSender->FindControlFromName("StaticACTIONPARA2");
	pwndStaticACTIONPARA3	= pwndSender->FindControlFromName("StaticACTIONPARA3");
	
	pwndBtnSave		= pwndSender->FindControlFromName("BtnSave");
	pwndBtnCancel		= pwndSender->FindControlFromName("BtnCancel");
	
	pwndStaticPreDistance	= pwndSender->FindControlFromName("StaticPreDistance");
	pwndEditSet_PreDistance	= pwndSender->FindControlFromName("EditSet_PreDistance");
	pwndSelectEditManualType= pwndSender->FindControlFromName("SelectEditManualType");
	pwndButton_GetPosi		= pwndSender->FindControlFromName("Button_GetPosi");
	
	pwndBtnWait			= pwndSender->FindControlFromName("BtnWait");
	pwndBtnPermit		= pwndSender->FindControlFromName("BtnPermit");
	pwndBtnTag			= pwndSender->FindControlFromName("BtnTag");
	pwndBtnGoto			= pwndSender->FindControlFromName("BtnGoto");
	pwndBtnValve		= pwndSender->FindControlFromName("BtnValve");
	pwndBtnDetect		= pwndSender->FindControlFromName("BtnDetect");

	AxisXOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0001;
	AxisYOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0002;
	AxisZOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0004;
	/* // cjlee2019/4/6 下午 05:53:50
	AxisCOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0008;
	AxisX2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0016;
	AxisY2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0032;*/
	AxisX2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0001;
	AxisY2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0002;
	AxisCOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0008;
	
	//Update string 
	printf("Update string\n");

	if(pwndSelectEditManualType != NULL) // 手動類型
		{
			int u_ManualMode = (int)(GetDBValue("MACHINE_FUNCTION_OPTIONS_RSV05").lValue);
			b_ManualMode=TF[u_ManualMode];
			pwndSelectEditManualType->SetPropValueT("captionID",Str_ManualMode[u_ManualMode]);
			//pwndSelectEditManualType->SetPropValueT("bgc",Color_Btn[u_Double]);		
			pwndSelectEditManualType->CreateA();
			pwndSelectEditManualType->Update();
		}

	return TRUE;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
	{
		RunOnlyOne=TRUE;
		((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標
			
		SelectNo		=GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue;		//祭紬 步驟
		ActionType = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52").lValue;			//濬倰 類型
		u_SubGroup = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED47").lValue; // 讀取副程式編號
		printf("SelectNo%d ActionType=%d\n",SelectNo,ActionType);
		char 	pDataID[256];
		
		SetVisible( pwndimageBackGround1, 1,0x0031);
		if(pwndSelectEditNUM != NULL)			// 編號
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,SelectNo);
			EditNUM = GetDBValue(pDataID).lValue;
			if(pwndSelectEditNUM->Is("CtmSelectEdit"))
			{
				((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(EditNUM);
				if(ActionType==1)
				{
					SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 1);//進入編程畫面送1 離開送0 主機判斷
					if(pwndStaticPreDistance != NULL)
					{
						if(EditNUM ==1)	//X1軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_XAXIS");
						}
						else if(EditNUM ==2)	//Y1軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_YAXIS");
						}
						else if(EditNUM ==3)	//Z軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_ZAXIS");
						}
						else if(EditNUM ==6)	//C嵞
						{
							pwndStaticPreDistance->SetPropValueT("textID","MSG_DUMMY");
							SetEnabled( pwndEditSet_PreDistance, 0);	
							SetEnabled( pwndSelectEditManualType, 0);	
							SetVisible( pwndStaticACTIONPARA1, 0,0x0030);
							if(pwndButton_GetPosi!=NULL)
							{
								pwndButton_GetPosi->SetPropValueT("taborder",(double)-3);
								pwndButton_GetPosi->UpdateAll();
							}
							if(pwndStaticACTIONPARA2!=NULL)
							{
								pwndStaticACTIONPARA2->SetPropValueT("textID","MOTION_GATE");
								pwndStaticACTIONPARA2->UpdateAll();
							}
							if(pwndStaticACTIONPARA3!=NULL)
							{
								pwndStaticACTIONPARA3->SetPropValueT("textID","MOTION_ONOFF");
								pwndStaticACTIONPARA3->UpdateAll();
							}
							if(pwndEditACTIONPARA2 != NULL)
							{
								pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
								pwndEditACTIONPARA2->SetPropValueT("promptID","PICKER_AXISC_VALVE");
								pwndEditACTIONPARA2	->UpdateAll();
							}
							if(pwndEditACTIONPARA3 != NULL)
							{
								pwndEditACTIONPARA3->SetPropValueT("max",(double)1);
								pwndEditACTIONPARA3->SetPropValueT("precision",(double)0);
								pwndEditACTIONPARA3->SetPropValueT("promptID","PICKER_AXISC_ONOFF");
								pwndEditACTIONPARA3	->UpdateAll();
							}
						}
						else if(EditNUM ==4)	//X2軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_X2AXIS");
						}
						else if(EditNUM ==5)	//Y2軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_Y2AXIS");
						}
						pwndStaticPreDistance->Update();
					}
				}
				((CtmSelectEdit*)pwndSelectEditNUM)->Update();
			}
			else if(pwndSelectEditNUM->Is("CtmToolButton"))
			{
				iSelectIndex = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54").lValue;
				printf("iSelectIndex=%d EditNUM=%d\n",iSelectIndex,EditNUM);
				if(iSelectIndex==0xFFFF)
					iSelectIndex=EditNUM;
				if(ActionType==1) //
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);	
					SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 1);//進入編程畫面送1 離開送0 主機判斷
					if(pwndStaticPreDistance != NULL) // 文字:"n軸" 點動距離
					{
						if(EditNUM ==1)	//X1軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_XAXIS");
						}
						else if(EditNUM ==2)	//Y1軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_YAXIS");
						}
						else if(EditNUM ==3)	//Z軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_ZAXIS");
						}
						else if(EditNUM ==6)	//C嵞
						{
							pwndStaticPreDistance->SetPropValueT("textID","MSG_DUMMY");
							SetEnabled( pwndEditSet_PreDistance, 0);	
							SetEnabled( pwndSelectEditManualType, 0);	
							SetVisible( pwndStaticACTIONPARA1, 0,0x0030);
							if(pwndButton_GetPosi!=NULL)
							{
								pwndButton_GetPosi->SetPropValueT("taborder",(double)-3);
								pwndButton_GetPosi->UpdateAll();
							}
							if(pwndStaticACTIONPARA2!=NULL)
							{
								pwndStaticACTIONPARA2->SetPropValueT("textID","MOTION_GATE");
								pwndStaticACTIONPARA2->UpdateAll();
							}
							if(pwndStaticACTIONPARA3!=NULL)
							{
								pwndStaticACTIONPARA3->SetPropValueT("textID","MOTION_ONOFF");
								pwndStaticACTIONPARA3->UpdateAll();
							}
							if(pwndEditACTIONPARA2 != NULL)
							{
								pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
								pwndEditACTIONPARA2->SetPropValueT("promptID","PICKER_AXISC_VALVE");
								pwndEditACTIONPARA2	->UpdateAll();
							}
							if(pwndEditACTIONPARA3 != NULL)
							{
								pwndEditACTIONPARA3->SetPropValueT("max",(double)1);
								pwndEditACTIONPARA3->SetPropValueT("precision",(double)0);
								pwndEditACTIONPARA3->SetPropValueT("promptID","PICKER_AXISC_ONOFF");
								pwndEditACTIONPARA3	->UpdateAll();
							}
						}
						else if(EditNUM ==4)	//X2軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_X2AXIS");
						}
						else if(EditNUM ==5)	//Y2軸
						{
							pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_Y2AXIS");
						}
						pwndStaticPreDistance->Update();
					}
					if(pwndEditACTIONPARA1 != NULL) // 位置 上限值
					{
						//printf("Set Position Maxdb\n");
						if(EditNUM ==1)	//X1軸
						{
							pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS1_MAXPOSTION");
						}
						else if(EditNUM ==2)	//Y1軸
						{
							pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS2_MAXPOSTION");
						}
						else if(EditNUM ==3)	//Z軸
						{
							pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS3_MAXPOSTION");
						}
						else if(EditNUM ==4)	//X2軸
						{
							pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS4_MAXPOSTION");
						}
						else if(EditNUM ==5)	//Y2軸
						{
							pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS5_MAXPOSTION");
						}
						pwndEditACTIONPARA1->Update();
					}
				}
				else if(ActionType==Action_Wait) // Wait
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_InnerI[iSelectIndex]);	
					}
				else if(ActionType==Action_Permit) // Permit
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_InnerO[iSelectIndex]);	
					}
				else if(ActionType==Action_Valve) // Valve
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_RemoteO[iSelectIndex]);	
					}
				else if(ActionType==Action_Detect) // Detect
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_RemoteI[iSelectIndex]);	
					}
				else if(ActionType==Action_Tag) // Tag
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_Label[iSelectIndex]);	
					}
				else if(ActionType==Action_Goto) // Goto
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_Label[iSelectIndex]);	
					}
				else if(ActionType==Action_Pile) // Pile
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_Pile[EditNUM]);	
					}
				else if(ActionType==Action_Sub) // Sub
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_Sub[EditNUM]);	
					}
			}
			else
			{
				pwndSelectEditNUM->SetPropValueT("value",EditNUM);
			}
			pwndSelectEditNUM->CreateA();
			pwndSelectEditNUM	->Update();
			pwndSelectEditNUM	->UpdateAll();
		}	
		
		if(pwndSelectGroupNum != NULL)			// 群組
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER4",u_SubGroup,SelectNo);
			GroupNUM = GetDBValue(pDataID).lValue;
			printf("Get GroupNum =%d\n",GroupNUM);

			if(pwndSelectGroupNum->Is("CtmToolButton"))
			{
				pwndSelectGroupNum->SetPropValueT("captionID",Str_Group[GroupNUM]);	
			}
			pwndSelectGroupNum	->CreateA();
			pwndSelectGroupNum	->Update();
		}	
		
		if(pwndDetectMode != NULL) // 檢測模式：單點/區間
			{
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
				iDetectMode = GetDBValue(pDataID).lValue;
				if(iDetectMode == 1) // 區間檢測
				{
					pwndDetectMode->SetPropValueT("captionID",Str_Detect[iDetectMode]);
				}
				else // 單點撿測
				{
					pwndDetectMode->SetPropValueT("captionID",Str_Detect[0]);
				}
				pwndDetectMode	->CreateA();
				pwndDetectMode	->Update();
			}
			
		if(pwndSTRT_STOP != NULL) // 檢測模式：單點/區間
			{
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
				iSTRTSTOP = GetDBValue(pDataID).lValue;
				if(iDetectMode == 1) // 區間檢測
					SetEnabled(pwndSTRT_STOP, 1);
				else
					SetEnabled(pwndSTRT_STOP, 0);
				if(iSTRTSTOP == 1) // 開始
				{
					pwndSTRT_STOP->SetPropValueT("captionID",Str_STRTSTOP[iSTRTSTOP]);
				}
				else // 停止
				{
					pwndSTRT_STOP->SetPropValueT("captionID",Str_STRTSTOP[0]);
				}
				pwndSTRT_STOP	->CreateA();
				pwndSTRT_STOP	->Update();
			}
		if(pwndON_OFF != NULL) // 開關狀態 ON/OFF
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,SelectNo);
			iON_OFF = GetDBValue(pDataID).lValue;
			if(iON_OFF == 1) // ON
			{
				if(ActionType==Action_Sub) // Sub
					pwndON_OFF->SetPropValueT("captionID",Str_STRT_END[iON_OFF]);
				else
					pwndON_OFF->SetPropValueT("captionID",Str_ON_OFF[iON_OFF]);
			}
			else // 單點撿測
			{
				if(ActionType==Action_Sub) // Sub
					pwndON_OFF->SetPropValueT("captionID",Str_STRT_END[0]);
				else
					pwndON_OFF->SetPropValueT("captionID",Str_ON_OFF[0]);
			}
			pwndON_OFF	->CreateA();
			pwndON_OFF	->Update();
		}

		if(pwndValveCheck != NULL) // 閥門 檢測
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
			iValveCheck = GetDBValue(pDataID).lValue;
			if(iValveCheck == 1) // ON
			{
				pwndValveCheck->SetPropValueT("captionID",Str_ValveCheck[iValveCheck]);
			}
			else // 單點撿測
			{
				pwndValveCheck->SetPropValueT("captionID",Str_ValveCheck[0]);
			}
			pwndValveCheck	->CreateA();
			pwndValveCheck	->Update();
		}
				
		if(pwndOPEN_MODE != NULL) // 開模 模式 0標準 1開模隨動 2開模中途
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
			iOpenMode = GetDBValue(pDataID).lValue;
			if(iOpenMode == 1) // 開模隨動
			{
				pwndOPEN_MODE->SetPropValueT("captionID",Str_OpenMode[iOpenMode]);
			}
			else if(iOpenMode == 2) // 開模中途
			{
				pwndOPEN_MODE->SetPropValueT("captionID",Str_OpenMode[iOpenMode]);
			}
			else // 單點撿測
			{
				pwndOPEN_MODE->SetPropValueT("captionID",Str_OpenMode[0]);
			}
			pwndOPEN_MODE	->CreateA();
			pwndOPEN_MODE	->Update();
		}
		
		if(pwndEND_MODE != NULL && pwndStaticEnd1!=NULL && pwndStaticEnd2!=NULL) // 停止 模式 0提前完成 1兩段變速
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER8",u_SubGroup,SelectNo);
			iEndMode = GetDBValue(pDataID).lValue;

			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
			ACTIONPARA3 = GetDBValue(pDataID).lValue;
			pwndEditEnd1->SetPropValueT("value",ACTIONPARA3);
				
			if(iEndMode == 1) // 兩段變速 
			{
				pwndEND_MODE->SetPropValueT("captionID",Str_EndMode[iEndMode]);
				pwndStaticEnd1->SetPropValueT("textID",Str_StaticEnd1[iEndMode]);
				pwndStaticEnd2->SetPropValueT("textID",Str_StaticEnd2[iEndMode]);
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER7",u_SubGroup,SelectNo);
				ACTIONPARA7 = GetDBValue(pDataID).lValue;
				pwndEditEnd2->SetPropValueT("value",ACTIONPARA7);
				printf("Set pwndEditEnd2 =%s\n",pDataID);
				pwndEditEnd2->SetPropValueT("precision",(double)0);
			}
			else // 提前完成
			{
				pwndEND_MODE->SetPropValueT("captionID",Str_EndMode[0]);
				pwndStaticEnd1->SetPropValueT("textID",Str_StaticEnd1[0]);
				pwndStaticEnd2->SetPropValueT("textID",Str_StaticEnd2[0]);
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
				ACTIONPARA6 = GetDBValue(pDataID).lValue;
				pwndEditEnd2->SetPropValueT("value",ACTIONPARA6);
				printf("Set pwndEditEnd2 =%s\n",pDataID);
				pwndEditEnd2->SetPropValueT("precision",(double)2);
			}
			pwndEND_MODE->CreateA();
			pwndEND_MODE->Update();
			pwndStaticEnd1->CreateA();
			pwndStaticEnd1->Update();
			pwndStaticEnd2->CreateA();
			pwndStaticEnd2->Update();
			pwndEditEnd1->CreateA();
			pwndEditEnd1->Update();
			pwndEditEnd2->CreateA();
			pwndEditEnd2->Update();
		}
		
		if(pwndEditPos_X != NULL) // 軸動作 點到點 X座標 PARA1
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,SelectNo);
			ACTIONPARA1 = GetDBValue(pDataID).lValue;
			pwndEditPos_X->SetPropValueT("value",ACTIONPARA1);
			printf("Set pwndEditPos_X =%s\n",pDataID);
			pwndEditPos_X->SetPropValueT("maxid","MACHINE_LIMITS_AXIS1_MAXPOSTION");
			pwndEditPos_X	->CreateA();
			pwndEditPos_X	->Update();
		}
		if(pwndEditPos_Y != NULL) // 軸動作 點到點 Y座標 PARA7
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER7",u_SubGroup,SelectNo);
			ACTIONPARA7 = GetDBValue(pDataID).lValue;
			pwndEditPos_Y->SetPropValueT("value",ACTIONPARA7);
			printf("Set pwndEditPos_Y =%s\n",pDataID);
			pwndEditPos_Y->SetPropValueT("maxid","MACHINE_LIMITS_AXIS2_MAXPOSTION");
			pwndEditPos_Y	->CreateA();
			pwndEditPos_Y	->Update();
		}
		if(pwndEditPos_Z != NULL) // 軸動作 點到點 Z座標 PARA8
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER8",u_SubGroup,SelectNo);
			ACTIONPARA8 = GetDBValue(pDataID).lValue;
			pwndEditPos_Z->SetPropValueT("value",ACTIONPARA8);
			printf("Set pwndEditPos_Z =%s\n",pDataID);
			pwndEditPos_Z->SetPropValueT("maxid","MACHINE_LIMITS_AXIS3_MAXPOSTION");
			pwndEditPos_Z	->CreateA();
			pwndEditPos_Z	->Update();
		}
		if(pwndPerAdv != NULL) // 軸動作 點到點 提前百分比 PARA3
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
			ACTIONPARA3 = GetDBValue(pDataID).lValue;
			pwndPerAdv->SetPropValueT("value",ACTIONPARA3);
			pwndPerAdv->CreateA();
			pwndPerAdv->Update();
		}
		if(pwndPerEnd != NULL) // 軸動作 點到點 末段百分比 PARA6
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
			ACTIONPARA6 = GetDBValue(pDataID).lValue;
			pwndPerEnd->SetPropValueT("value",ACTIONPARA6);
			pwndPerEnd->CreateA();
			pwndPerEnd->Update();
		}
				
		if(pwndEditACTIONPARA1 != NULL) // 參數1
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,SelectNo);
			ACTIONPARA1 = GetDBValue(pDataID).lValue;
			if(pwndEditACTIONPARA1->Is("CtmSelectEdit"))
			{
				((CtmSelectEdit*)pwndEditACTIONPARA1)->SetIndex(ACTIONPARA1);
			}
			else
			{
				pwndEditACTIONPARA1->SetPropValueT("value",ACTIONPARA1);
			}
			pwndEditACTIONPARA1	->Update();
			pwndEditACTIONPARA1	->UpdateAll();
		}
		
		if(pwndEditACTIONPARA2 != NULL)			//參數2
		{
			//if(SelectNo==1) SetEnabled( pwndEditACTIONPARA2, 0);	 //菴珨祭嘐趙
			if(ActionType == Action_Wait) // 等待
			{
				if(EditNUM ==2)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)2);
				}
				else if(EditNUM ==3)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
				}
				else
					SetEnabled( pwndEditACTIONPARA2, 0);
			}
			if(ActionType == Action_Valve) // 閥門
			{
	//			if(EditNUM !=10) // cjlee changed 2019/4/26 上午 10:23:49
	//			{
	//				SetEnabled( pwndEditACTIONPARA2, 0);
	//			}	
			}
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
			ACTIONPARA2 = GetDBValue(pDataID).lValue;
			if(pwndEditACTIONPARA2->Is("CtmSelectEdit"))
			{
				((CtmSelectEdit*)pwndEditACTIONPARA2)->SetIndex(ACTIONPARA2);
			}
			else
			{
				pwndEditACTIONPARA2->SetPropValueT("value",ACTIONPARA2);
			}
			if(pwndStaticACTIONPARA2!=NULL)
			{
				pwndStaticACTIONPARA2	->Update();
				pwndStaticACTIONPARA2	->UpdateAll();
			}
			pwndEditACTIONPARA2	->CreateA();
			pwndEditACTIONPARA2	->Update();
			//pwndEditACTIONPARA2	->UpdateAll();
		}
		
		if(pwndEditACTIONPARA3 != NULL)			//統杅3
		{
			if(SelectNo==1) SetEnabled( pwndEditACTIONPARA3, 0);	 //菴珨祭嘐趙
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
			ACTIONPARA3 = GetDBValue(pDataID).lValue;
			pwndEditACTIONPARA3->SetPropValueT("value",ACTIONPARA3);
			pwndEditACTIONPARA3	->Update();
		}
		
		if(pwndEditACTIONPARA5 != NULL)			//參數5
		{
			//if(SelectNo==1) SetEnabled( pwndEditACTIONPARA5, 0);	 //第一步固化
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5",u_SubGroup,SelectNo);
			ACTIONPARA5 = GetDBValue(pDataID).lValue;
			pwndEditACTIONPARA5->SetPropValueT("value",ACTIONPARA5/10);
			pwndEditACTIONPARA5	->Update();
		}
		
		if(pwndEditACTIONPARA6 != NULL)			//參數6
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
			ACTIONPARA6 = GetDBValue(pDataID).lValue;
			pwndEditACTIONPARA6->SetPropValueT("value",ACTIONPARA6);
			pwndEditACTIONPARA6->Update();
		}
		
		if(pwndStaticTITLE2 != NULL)
		{
			if(ActionType == 5)
			{
				SetVisible( pwndimageBackGround1, 1,0x0031);
				SetVisible( pwndStaticTITLE2, 0,0x0031);
			}
		}
		
	}
	
	iSelectIndex = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54").lValue;
	AxisXNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0001;
	AxisYNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0002;
	AxisZNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0004;
	AxisX2New = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0001;
	AxisY2New = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0002;
	AxisCNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0008;
	if(pwndStaticPreDistance != NULL && (ActionType==1)&&(SelectNo>StandbyStepNum)) // hotkey 移動軸
	{
		if(AxisXNew != AxisXOld)	//X1軸
		{
			//printf("AxisXNew\n");
			if(AxisXNew)
			{
				//((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(1);
				EditNUM = 1;
				if(pwndSelectEditNUM!=NULL)
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);
				}
				pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_XAXIS");
				SetEnabled( pwndEditSet_PreDistance, 1);	
				SetEnabled( pwndSelectEditManualType, 1);	
				SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
				if(pwndButton_GetPosi!=NULL)
				{
					pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
					pwndButton_GetPosi->UpdateAll();
				}
				if(pwndStaticACTIONPARA2!=NULL)
				{
					pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
					pwndStaticACTIONPARA2->UpdateAll();
				}
				if(pwndStaticACTIONPARA3!=NULL)
				{
					pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
					pwndStaticACTIONPARA3->UpdateAll();
				}
				if(pwndEditACTIONPARA2 != NULL)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
					pwndEditACTIONPARA2->SetPropValueT("promptID","");
					pwndEditACTIONPARA2	->Update();
					pwndEditACTIONPARA2	->UpdateAll();
				}
				if(pwndEditACTIONPARA3 != NULL)
				{
					pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
					pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
					pwndEditACTIONPARA3->SetPropValueT("promptID","");
					pwndEditACTIONPARA3	->Update();
					pwndEditACTIONPARA3	->UpdateAll();
				}
			}
			AxisXOld = AxisXNew;
		}
		if(AxisYNew != AxisYOld)	//Y1軸
		{
			printf("AxisYNew\n");
			if(AxisYNew)
			{
				//((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(2);
				EditNUM = 2;
				if(pwndSelectEditNUM!=NULL)
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);
				}
				pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_YAXIS");
				SetEnabled( pwndEditSet_PreDistance, 1);	
				SetEnabled( pwndSelectEditManualType, 1);	
				SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
				if(pwndButton_GetPosi!=NULL)
				{
					pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
					pwndButton_GetPosi->UpdateAll();
				}
				if(pwndStaticACTIONPARA2!=NULL)
				{
					pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
					pwndStaticACTIONPARA2->UpdateAll();
				}
				if(pwndStaticACTIONPARA3!=NULL)
				{
					pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
					pwndStaticACTIONPARA3->UpdateAll();
				}
				if(pwndEditACTIONPARA2 != NULL)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
					pwndEditACTIONPARA2->SetPropValueT("promptID","");
					pwndEditACTIONPARA2	->Update();
					pwndEditACTIONPARA2	->UpdateAll();
				}
				if(pwndEditACTIONPARA3 != NULL)
				{
					pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
					pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
					pwndEditACTIONPARA3->SetPropValueT("promptID","");
					pwndEditACTIONPARA3	->Update();
					pwndEditACTIONPARA3	->UpdateAll();
				}
			}
			AxisYOld = AxisYNew;
		}
		if(AxisZNew != AxisZOld)	//Z軸
		{
			printf("AxisZNew\n");
			if(AxisZNew)
			{
				//((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(3);
				EditNUM = 3;
				if(pwndSelectEditNUM!=NULL)
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);
				}
				pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_ZAXIS");
				SetEnabled( pwndEditSet_PreDistance, 1);	
				SetEnabled( pwndSelectEditManualType, 1);	
				SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
				if(pwndButton_GetPosi!=NULL)
				{
					pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
					pwndButton_GetPosi->UpdateAll();
				}
				if(pwndStaticACTIONPARA2!=NULL)
				{
					pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
					pwndStaticACTIONPARA2->UpdateAll();
				}
				if(pwndStaticACTIONPARA3!=NULL)
				{
					pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
					pwndStaticACTIONPARA3->UpdateAll();
				}
				if(pwndEditACTIONPARA2 != NULL)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
					pwndEditACTIONPARA2->SetPropValueT("promptID","");
					pwndEditACTIONPARA2	->Update();
					pwndEditACTIONPARA2	->UpdateAll();
				}
				if(pwndEditACTIONPARA3 != NULL)
				{
					pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
					pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
					pwndEditACTIONPARA3->SetPropValueT("promptID","");
					pwndEditACTIONPARA3	->Update();
					pwndEditACTIONPARA3	->UpdateAll();
				}
			}
			AxisZOld = AxisZNew;
		}
		if(AxisX2New != AxisX2Old)	//X2軸
		{
			printf("AxisX2New\n");
			if(AxisX2New)
			{
				//((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(5);
				EditNUM = 4;
				if(pwndSelectEditNUM!=NULL)
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);
				}
				pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_X2AXIS");
				SetEnabled( pwndEditSet_PreDistance, 1);	
				SetEnabled( pwndSelectEditManualType, 1);	
				SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
				if(pwndButton_GetPosi!=NULL)
				{
					pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
					pwndButton_GetPosi->UpdateAll();
				}
				if(pwndStaticACTIONPARA2!=NULL)
				{
					pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
					pwndStaticACTIONPARA2->UpdateAll();
				}
				if(pwndStaticACTIONPARA3!=NULL)
				{
					pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
					pwndStaticACTIONPARA3->UpdateAll();
				}
				if(pwndEditACTIONPARA2 != NULL)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
					pwndEditACTIONPARA2->SetPropValueT("promptID","");
					pwndEditACTIONPARA2	->Update();
					pwndEditACTIONPARA2	->UpdateAll();
				}
				if(pwndEditACTIONPARA3 != NULL)
				{
					pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
					pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
					pwndEditACTIONPARA3->SetPropValueT("promptID","");
					pwndEditACTIONPARA3	->Update();
					pwndEditACTIONPARA3	->UpdateAll();
				}
			}
			AxisX2Old = AxisX2New;
		}
		if(AxisY2New != AxisY2Old)	//Y2軸
		{
			printf("AxisY2New\n");
			if(AxisY2New)
			{
				//((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(6);
				EditNUM = 5;
				if(pwndSelectEditNUM!=NULL)
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);
				}
				pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_Y2AXIS");
				SetEnabled( pwndEditSet_PreDistance, 1);	
				SetEnabled( pwndSelectEditManualType, 1);	
				SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
				if(pwndButton_GetPosi!=NULL)
				{
					pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
					pwndButton_GetPosi->UpdateAll();
				}
				if(pwndStaticACTIONPARA2!=NULL)
				{
					pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
					pwndStaticACTIONPARA2->UpdateAll();
				}
				if(pwndStaticACTIONPARA3!=NULL)
				{
					pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
					pwndStaticACTIONPARA3->UpdateAll();
				}
				if(pwndEditACTIONPARA2 != NULL)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
					pwndEditACTIONPARA2->SetPropValueT("promptID","");
					pwndEditACTIONPARA2	->Update();
					pwndEditACTIONPARA2	->UpdateAll();
				}
				if(pwndEditACTIONPARA3 != NULL)
				{
					pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
					pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
					pwndEditACTIONPARA3->SetPropValueT("promptID","");
					pwndEditACTIONPARA3	->Update();
					pwndEditACTIONPARA3	->UpdateAll();
				}
			}
			AxisY2Old = AxisY2New;
		}
		if(AxisCNew != AxisCOld)	//C嵞
		{
			printf("AxisCNew\n");
			if(AxisCNew)
			{
				//((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(4);
				EditNUM = 6;
				if(pwndSelectEditNUM!=NULL)
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);
				}
				pwndStaticPreDistance->SetPropValueT("textID","MSG_DUMMY");
				SetEnabled( pwndEditSet_PreDistance, 0);	
				SetEnabled( pwndSelectEditManualType, 0);	
				SetVisible( pwndStaticACTIONPARA1, 0,0x0030);
				if(pwndButton_GetPosi!=NULL)
				{
					pwndButton_GetPosi->SetPropValueT("taborder",(double)-3);
					pwndButton_GetPosi->UpdateAll();
				}
				if(pwndStaticACTIONPARA2!=NULL)
				{
					pwndStaticACTIONPARA2->SetPropValueT("textID","MOTION_GATE");
					pwndStaticACTIONPARA2->UpdateAll();
				}
				if(pwndStaticACTIONPARA3!=NULL)
				{
					pwndStaticACTIONPARA3->SetPropValueT("textID","MOTION_ONOFF");
					pwndStaticACTIONPARA3->UpdateAll();
				}
				if(pwndEditACTIONPARA2 != NULL)
				{
					pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
					pwndEditACTIONPARA2->SetPropValueT("promptID","PICKER_AXISC_VALVE");
					pwndEditACTIONPARA2	->Update();
					pwndEditACTIONPARA2	->UpdateAll();
				}
				if(pwndEditACTIONPARA3 != NULL)
				{
					pwndEditACTIONPARA3->SetPropValueT("max",(double)1);
					pwndEditACTIONPARA3->SetPropValueT("precision",(double)0);
					pwndEditACTIONPARA3->SetPropValueT("promptID","PICKER_AXISC_ONOFF");
					pwndEditACTIONPARA3	->Update();
					pwndEditACTIONPARA3	->UpdateAll();
				}
			}
			AxisCOld = AxisCNew;
		}
		//((CtmSelectEdit*)pwndSelectEditNUM)->Update();
		pwndSelectEditNUM->CreateA();
		pwndSelectEditNUM->Update();	
		pwndStaticPreDistance->Update();
	}
	if(iSelectIndex!=iSelectIndexOld) // 更新標籤
	{
		printf("Update Tag\n");
		iSelectIndexOld=iSelectIndex;
//		if(pwndSelectEditNUM!=NULL)
//		{
//			((CtmSelectEdit*)pwndSelectEditNUM)->SetIndex(iSelectIndex);
//			((CtmSelectEdit*)pwndSelectEditNUM)->Update();
//		}
		if(pwndSelectEditNUM->Is("CtmToolButton"))
		{
			iSelectIndex = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54").lValue;
			if(iSelectIndex<0)
				iSelectIndex=EditNUM;
			printf("iSelectIndex=%d\n",iSelectIndex);
			if(ActionType==1) // 軸動作
			{
				pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);	
				SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 1);//進入編程畫面送1 離開送0 主機判斷
				if(pwndStaticPreDistance != NULL) // 文字:"n軸" 點動距離
				{
					if(EditNUM ==1)	//X1軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_XAXIS");
					}
					else if(EditNUM ==2)	//Y1軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_YAXIS");
					}
					else if(EditNUM ==3)	//Z軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_ZAXIS");
					}
					else if(EditNUM ==6)	//C嵞
					{
						pwndStaticPreDistance->SetPropValueT("textID","MSG_DUMMY");
						SetEnabled( pwndEditSet_PreDistance, 0);	
						SetEnabled( pwndSelectEditManualType, 0);	
						SetVisible( pwndStaticACTIONPARA1, 0,0x0030);
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-3);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","MOTION_GATE");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","MOTION_ONOFF");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
							pwndEditACTIONPARA2->SetPropValueT("promptID","PICKER_AXISC_VALVE");
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)1);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)0);
							pwndEditACTIONPARA3->SetPropValueT("promptID","PICKER_AXISC_ONOFF");
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					else if(EditNUM ==4)	//X2軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_X2AXIS");
					}
					else if(EditNUM ==5)	//Y2軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_Y2AXIS");
					}
					pwndStaticPreDistance->Update();
				}
				if(pwndEditACTIONPARA1 != NULL) // 位置 上限值
				{
					//printf("Set Position Maxdb\n");
					if(EditNUM ==1)	//X1軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS1_MAXPOSTION");
					}
					else if(EditNUM ==2)	//Y1軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS2_MAXPOSTION");
					}
					else if(EditNUM ==3)	//Z軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS3_MAXPOSTION");
					}
					else if(EditNUM ==4)	//X2軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS4_MAXPOSTION");
					}
					else if(EditNUM ==5)	//Y2軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS5_MAXPOSTION");
					}
					pwndEditACTIONPARA1->Update();
				}
			}
			else if(ActionType==Action_Wait) // Wait
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_InnerI[iSelectIndex]);	
				}
			else if(ActionType==Action_Permit) // Permit
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_InnerO[iSelectIndex]);	
				}
			else if(ActionType==Action_Valve) // Valve
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_RemoteO[iSelectIndex]);	
				}
			else if(ActionType==Action_Detect) // Detect
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_RemoteI[iSelectIndex]);	
				}
				else if(ActionType==Action_Tag) // Tag
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_Label[iSelectIndex]);	
					}
				else if(ActionType==Action_Goto) // Goto
					{
						pwndSelectEditNUM->SetPropValueT("captionID",Str_Label[iSelectIndex]);	
					}
			else if(ActionType==Action_Pile) // Pile
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Pile[EditNUM]);	
				}
			else if(ActionType==Action_Sub) // Sub
				{
					pwndSelectEditNUM->SetPropValueT("captionID",Str_Sub[EditNUM]);	
				}
			pwndSelectEditNUM->CreateA();	
			pwndSelectEditNUM->Update();
		}
//		if(ActionType==4) // cjlee changed 2019/4/26 上午 10:25:53
//		{
//			if(iSelectIndex ==10)
//			{
//				SetEnabled( pwndEditACTIONPARA2, 1);
//			}
//			else
//			{
//				SetEnabled( pwndEditACTIONPARA2, 0);
//			}
//		}
	}
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("OnChangeA\n");
	
	CtmWnd*	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == pwndSelectEditNUM && pwndEditACTIONPARA2 != NULL)
	{
		if(pwndSelectEditNUM != NULL)
		{
			if(pwndSelectEditNUM->Is("CtmSelectEdit"))
			{
				EditNUM = ((CtmSelectEdit*)pwndSelectEditNUM)->GetIndex();
			}
//			else
//			{
//				pwndSelectEditNUM->GetPropValueT("value", &EditNUM,sizeof(EditNUM));
//			}
		}
		if(ActionType == 1)
		{
			if(pwndStaticPreDistance != NULL)
			{
				if(EditNUM ==1)	//X1嵞
				{
					pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_XAXIS");
					SetEnabled( pwndEditSet_PreDistance, 1);	
					SetEnabled( pwndSelectEditManualType, 1);	
					SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
					if(pwndButton_GetPosi!=NULL)
					{
						pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
						pwndButton_GetPosi->UpdateAll();
					}
					if(pwndStaticACTIONPARA2!=NULL)
					{
						pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
						pwndStaticACTIONPARA2->UpdateAll();
					}
					if(pwndStaticACTIONPARA3!=NULL)
					{
						pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
						pwndStaticACTIONPARA3->UpdateAll();
					}
					if(pwndEditACTIONPARA2 != NULL)
					{
						pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
						pwndEditACTIONPARA2->SetPropValueT("promptID","");
						pwndEditACTIONPARA2	->Update();
						pwndEditACTIONPARA2	->UpdateAll();
					}
					if(pwndEditACTIONPARA3 != NULL)
					{
						pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
						pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
						pwndEditACTIONPARA3->SetPropValueT("promptID","");
						pwndEditACTIONPARA3	->Update();
						pwndEditACTIONPARA3	->UpdateAll();
					}
				}
				else if(EditNUM ==2)	//Y1嵞
				{
					pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_YAXIS");
					SetEnabled( pwndEditSet_PreDistance, 1);	
					SetEnabled( pwndSelectEditManualType, 1);	
					SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
					if(pwndButton_GetPosi!=NULL)
					{
						pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
						pwndButton_GetPosi->UpdateAll();
					}
					if(pwndStaticACTIONPARA2!=NULL)
					{
						pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
						pwndStaticACTIONPARA2->UpdateAll();
					}
					if(pwndStaticACTIONPARA3!=NULL)
					{
						pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
						pwndStaticACTIONPARA3->UpdateAll();
					}
					if(pwndEditACTIONPARA2 != NULL)
					{
						pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
						pwndEditACTIONPARA2->SetPropValueT("promptID","");
						pwndEditACTIONPARA2	->Update();
						pwndEditACTIONPARA2	->UpdateAll();
					}
					if(pwndEditACTIONPARA3 != NULL)
					{
						pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
						pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
						pwndEditACTIONPARA3->SetPropValueT("promptID","");
						pwndEditACTIONPARA3	->Update();
						pwndEditACTIONPARA3	->UpdateAll();
					}
				}
				else if(EditNUM ==3)	//Z嵞
				{
					pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_ZAXIS");
					SetEnabled( pwndEditSet_PreDistance, 1);	
					SetEnabled( pwndSelectEditManualType, 1);	
					SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
					if(pwndButton_GetPosi!=NULL)
					{
						pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
						pwndButton_GetPosi->UpdateAll();
					}
					if(pwndStaticACTIONPARA2!=NULL)
					{
						pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
						pwndStaticACTIONPARA2->UpdateAll();
					}
					if(pwndStaticACTIONPARA3!=NULL)
					{
						pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
						pwndStaticACTIONPARA3->UpdateAll();
					}
					if(pwndEditACTIONPARA2 != NULL)
					{
						pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
						pwndEditACTIONPARA2->SetPropValueT("promptID","");
						pwndEditACTIONPARA2	->Update();
						pwndEditACTIONPARA2	->UpdateAll();
					}
					if(pwndEditACTIONPARA3 != NULL)
					{
						pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
						pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
						pwndEditACTIONPARA3->SetPropValueT("promptID","");
						pwndEditACTIONPARA3	->Update();
						pwndEditACTIONPARA3	->UpdateAll();
					}
				}
				else if(EditNUM ==4)
				{
					pwndStaticPreDistance->SetPropValueT("textID","MSG_DUMMY");
					SetEnabled( pwndEditSet_PreDistance, 0);	
					SetEnabled( pwndSelectEditManualType, 0);	
					SetVisible( pwndStaticACTIONPARA1, 0,0x0030);
					if(pwndButton_GetPosi!=NULL)
					{
						pwndButton_GetPosi->SetPropValueT("taborder",(double)-3);
						pwndButton_GetPosi->UpdateAll();
					}
					if(pwndStaticACTIONPARA2!=NULL)
					{
						pwndStaticACTIONPARA2->SetPropValueT("textID","MOTION_GATE");
						pwndStaticACTIONPARA2->UpdateAll();
					}
					if(pwndStaticACTIONPARA3!=NULL)
					{
						pwndStaticACTIONPARA3->SetPropValueT("textID","MOTION_ONOFF");
						pwndStaticACTIONPARA3->UpdateAll();
					}
					if(pwndEditACTIONPARA2 != NULL)
					{
						pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
						pwndEditACTIONPARA2->SetPropValueT("promptID","PICKER_AXISC_VALVE");
						pwndEditACTIONPARA2	->Update();
						pwndEditACTIONPARA2	->UpdateAll();
					}
					if(pwndEditACTIONPARA3 != NULL)
					{
						pwndEditACTIONPARA3->SetPropValueT("max",(double)1);
						pwndEditACTIONPARA3->SetPropValueT("precision",(double)0);
						pwndEditACTIONPARA3->SetPropValueT("promptID","PICKER_AXISC_ONOFF");
						pwndEditACTIONPARA3	->Update();
						pwndEditACTIONPARA3	->UpdateAll();
					}
				}
				pwndStaticPreDistance->Update();
			}
		}
		if(ActionType == 2)
		{
			if(EditNUM ==2)
			{
				SetEnabled( pwndEditACTIONPARA2, 1);
				pwndEditACTIONPARA2->SetPropValueT("max",(double)2);
			}
			else if(EditNUM ==3)
			{
				SetEnabled( pwndEditACTIONPARA2, 1);
				pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
			}
			else
			{
				SetEnabled( pwndEditACTIONPARA2, 0);;
			}
		}
		pwndStaticACTIONPARA2	->Update();
		pwndStaticACTIONPARA2	->UpdateAll();
		pwndEditACTIONPARA2	->Update();
		pwndEditACTIONPARA2	->UpdateAll();
	}
	
	pwnd->OnLoseFocus(); // 取消光標
	
	
	return wIDControl;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA\n");
	//printf("wKey=%d\n",wKey);
	//char 	pDataID[256];
	//memset(pDataID, 0 ,sizeof(pDataID));
	//sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",SelectNo);
	//EditNUM =GetDBValue(pDataID).lValue;	
	long AXISACTPos =0;	
	
	((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標

	if (wKey == 0x0004)	// 獲取位置
	{
		printf("Get Pos\n");
		if(pwndSelectEditNUM != NULL)
		{
			printf("pwndSelectEditNUM exsit\n");
			if(pwndSelectEditNUM->Is("CtmSelectEdit"))
			{
				EditNUM = ((CtmSelectEdit*)pwndSelectEditNUM)->GetIndex();
			}
//			else
//			{
//				pwndSelectEditNUM->GetPropValueT("value", &EditNUM,sizeof(EditNUM));
//			}
			printf("EditNUM=%d\n",EditNUM);
		}
		switch(EditNUM)
		{
			case 1:
				AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_XAXISPOSITION").lValue;	
				printf("AXISACTPos=%d\n",AXISACTPos);
				break;
			case 2:
				AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_YAXISPOSITION").lValue;	
				break;
			case 3:
				AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_ZAXISPOSITION").lValue;	
				break;
			case 4:
				AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_X2AXISPOSITION").lValue;	
				break;
			case 5:
				AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_Y2AXISPOSITION").lValue;	
				break;
			default:
				break;
		}
		if(EditNUM!=0)
		{
			if(pwndEditACTIONPARA1 != NULL)
			{
				printf("Set Axis%d=%d\n",EditNUM,AXISACTPos);
				pwndEditACTIONPARA1->SetPropValueT("value",AXISACTPos);
				pwndEditACTIONPARA1->Update();
			}
		}
		
		if(pwndEditPos_X!=NULL && pwndEditPos_Y!=NULL && pwndEditPos_Z!=NULL ) // P2P 三軸座標
		{
			AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_XAXISPOSITION").lValue;	
			pwndEditPos_X->SetPropValueT("value",AXISACTPos);
			AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_YAXISPOSITION").lValue;	
			pwndEditPos_Y->SetPropValueT("value",AXISACTPos);
			AXISACTPos =GetDBValue("MACHINE_INTERFACE_CONTROL_ZAXISPOSITION").lValue;	
			pwndEditPos_Z->SetPropValueT("value",AXISACTPos);
			pwndEditPos_X->Update();
			pwndEditPos_Y->Update();
			pwndEditPos_Z->Update();
		}			
		return _NULL_KEY;
	}

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{ 	
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	return TRUE;
}
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("MousUp\n");
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == NULL)	return wIDControl;
 			
 	if(pwnd == pwndSelectEditManualType) // 手動類型
	{
		int nTemp =0;
		b_ManualMode = !b_ManualMode;
		nTemp = b_ManualMode;		
		//Update string
		pwndSelectEditManualType->SetPropValueT("captionID",Str_ManualMode[nTemp]);
		pwndSelectEditManualType->CreateA();
		pwndSelectEditManualType->Update();
		// Set Value to DB
		SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV05",nTemp);
	}
 		
	else if(pwnd == pwndSelectEditNUM) // 軸動作選擇
	{
		if(ActionType==1) // 軸動作
		{
			EditNUM++;
			if(EditNUM>6)
				EditNUM=0;
			printf("EditNUM=%d\n",EditNUM);
			
			pwndSelectEditNUM->SetPropValueT("captionID",Str_Axis[EditNUM]);	
			if(ActionType==1)
			{
				SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 1);//進入編程畫面送1 離開送0 主機判斷
				if(pwndStaticPreDistance != NULL)
				{
					if(EditNUM ==1)	//X1軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_XAXIS");
						SetEnabled( pwndEditSet_PreDistance, 1);	
						SetEnabled( pwndSelectEditManualType, 1);	
						SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
						SetEnabled( pwndEditACTIONPARA1, 1);
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
							pwndEditACTIONPARA2->SetPropValueT("promptID","");
							pwndEditACTIONPARA2	->Update();
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
							pwndEditACTIONPARA3->SetPropValueT("promptID","");
							pwndEditACTIONPARA3	->Update();
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					else if(EditNUM ==2)	//Y1軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_YAXIS");
						SetEnabled( pwndEditSet_PreDistance, 1);	
						SetEnabled( pwndSelectEditManualType, 1);	
						SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
						SetEnabled( pwndEditACTIONPARA1, 1);
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
							pwndEditACTIONPARA2->SetPropValueT("promptID","");
							pwndEditACTIONPARA2	->Update();
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
							pwndEditACTIONPARA3->SetPropValueT("promptID","");
							pwndEditACTIONPARA3	->Update();
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					else if(EditNUM ==3)	//Z軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_ZAXIS");
						SetEnabled( pwndEditSet_PreDistance, 1);	
						SetEnabled( pwndSelectEditManualType, 1);	
						SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
						SetEnabled( pwndEditACTIONPARA1, 1);
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
							pwndEditACTIONPARA2->SetPropValueT("promptID","");
							pwndEditACTIONPARA2	->Update();
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
							pwndEditACTIONPARA3->SetPropValueT("promptID","");
							pwndEditACTIONPARA3	->Update();
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					else if(EditNUM ==6)	//C嵞
					{
						pwndStaticPreDistance->SetPropValueT("textID","MSG_DUMMY");
						SetEnabled( pwndEditSet_PreDistance, 0);	
						SetEnabled( pwndSelectEditManualType, 0);	
						SetVisible( pwndStaticACTIONPARA1, 0,0x0030);
						SetEnabled( pwndEditACTIONPARA1, 0);	
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-3);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","MOTION_GATE");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","MOTION_ONOFF");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)1);
							pwndEditACTIONPARA2->SetPropValueT("promptID","PICKER_AXISC_VALVE");
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)1);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)0);
							pwndEditACTIONPARA3->SetPropValueT("promptID","PICKER_AXISC_ONOFF");
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					else if(EditNUM ==4)	//X2軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_X2AXIS");
						SetEnabled( pwndEditSet_PreDistance, 1);	
						SetEnabled( pwndSelectEditManualType, 1);	
						SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
						SetEnabled( pwndEditACTIONPARA1, 1);
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
							pwndEditACTIONPARA2->SetPropValueT("promptID","");
							pwndEditACTIONPARA2	->Update();
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
							pwndEditACTIONPARA3->SetPropValueT("promptID","");
							pwndEditACTIONPARA3	->Update();
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					else if(EditNUM ==5)	//Y2軸
					{
						pwndStaticPreDistance->SetPropValueT("textID","VW_HAP5_ROBOT_Y2AXIS");
						SetEnabled( pwndEditSet_PreDistance, 1);	
						SetEnabled( pwndSelectEditManualType, 1);	
						SetVisible( pwndStaticACTIONPARA1, 1,0x0030);
						SetEnabled( pwndEditACTIONPARA1, 1);
						if(pwndButton_GetPosi!=NULL)
						{
							pwndButton_GetPosi->SetPropValueT("taborder",(double)-1);
							pwndButton_GetPosi->UpdateAll();
						}
						if(pwndStaticACTIONPARA2!=NULL)
						{
							pwndStaticACTIONPARA2->SetPropValueT("textID","VW_EJECT_RCUT_SPED");
							pwndStaticACTIONPARA2->UpdateAll();
						}
						if(pwndStaticACTIONPARA3!=NULL)
						{
							pwndStaticACTIONPARA3->SetPropValueT("textID","VW_PICKER_ADVANCE");
							pwndStaticACTIONPARA3->UpdateAll();
						}
						if(pwndEditACTIONPARA2 != NULL)
						{
							pwndEditACTIONPARA2->SetPropValueT("max",(double)100);
							pwndEditACTIONPARA2->SetPropValueT("promptID","");
							pwndEditACTIONPARA2	->Update();
							pwndEditACTIONPARA2	->UpdateAll();
						}
						if(pwndEditACTIONPARA3 != NULL)
						{
							pwndEditACTIONPARA3->SetPropValueT("max",(double)99999);
							pwndEditACTIONPARA3->SetPropValueT("precision",(double)2);
							pwndEditACTIONPARA3->SetPropValueT("promptID","");
							pwndEditACTIONPARA3	->Update();
							pwndEditACTIONPARA3	->UpdateAll();
						}
					}
					pwndStaticPreDistance->Update();
				}
				if(pwndEditACTIONPARA1 != NULL)//依照軸改變位置最大值
				{
					//printf("Set Position Maxdb\n");
					if(EditNUM ==1)	//X1軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS1_MAXPOSTION");
					}
					else if(EditNUM ==2)	//Y1軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS2_MAXPOSTION");
					}
					else if(EditNUM ==3)	//Z軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS3_MAXPOSTION");
					}
					else if(EditNUM ==4)	//X2軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS4_MAXPOSTION");
					}
					else if(EditNUM ==5)	//Y2軸
					{
						pwndEditACTIONPARA1->SetPropValueT("maxid","MACHINE_LIMITS_AXIS5_MAXPOSTION");
					}
					pwndEditACTIONPARA1->Update();
				}
			}
			pwndSelectEditNUM->CreateA();
			pwndSelectEditNUM->Update();
		}
		else if(ActionType==Action_Pile) // Pile
		{
			EditNUM++;
			if(EditNUM>4)
				EditNUM=0;
			pwndSelectEditNUM->SetPropValueT("captionID",Str_Pile[EditNUM]);
			pwndSelectEditNUM->CreateA();
			pwndSelectEditNUM->Update();
		}
		else if(ActionType==Action_Sub) // Sub
		{
			EditNUM++;
			if(EditNUM>5)
				EditNUM=0;
			pwndSelectEditNUM->SetPropValueT("captionID",Str_Sub[EditNUM]);
			pwndSelectEditNUM->CreateA();
			pwndSelectEditNUM->Update();
		}
		else if(ActionType == Action_Wait) // Wait
		{
			MsgBoxCall("EditWindow_SUB_Wait2.txt");
		}
		else if(ActionType == Action_Permit) // Permit
		{
			MsgBoxCall("EditWindow_SUB_Permit2.txt");
		}
		else if (ActionType == Action_Tag) // 標籤
		{
			MsgBoxCall("EditWindow_SUB_Tag2.txt");
		}
		else if (ActionType == Action_Goto) // 跳轉
		{
			MsgBoxCall("EditWindow_SUB_Goto2.txt");
		}
		else if (ActionType == Action_Valve) // 閥門
		{
			MsgBoxCall("EditWindow_SUB_Valve2.txt");
		}
		else if (ActionType == Action_Detect) // 檢測
		{
			MsgBoxCall("EditWindow_SUB_Detect2.txt");
		}
	}
	
	else if(pwnd == pwndSelectGroupNum) // 群組選擇
	{
		GroupNUM++;
		if(GroupNUM>8)
			GroupNUM=0;
		printf("GroupNUM=%d\n",GroupNUM);
		pwndSelectGroupNum->SetPropValueT("captionID",Str_Group[GroupNUM]);	
		pwndSelectGroupNum->CreateA();
		pwndSelectGroupNum->Update();
	}
	
	else if(pwnd == pwndDetectMode) // 檢測模式：單點/區間
	{
		iDetectMode++;
		if(iDetectMode>1)
			iDetectMode=0;
		pwndDetectMode->SetPropValueT("captionID",Str_Detect[iDetectMode]);	
		pwndDetectMode->CreateA();
		pwndDetectMode->Update();
		if(iDetectMode)
		{
			SetEnabled(pwndSTRT_STOP, 1);
		}
		else
		{
			SetEnabled(pwndSTRT_STOP, 0);
		}
		pwndSTRT_STOP->SetPropValueT("upbitmap",Img_Btn[iDetectMode]);
		pwndSTRT_STOP->CreateA();
		pwndSTRT_STOP->Update();
	}
	else if(pwnd == pwndSTRT_STOP) // 檢測：開始/停止
	{
		iSTRTSTOP++;
		if(iSTRTSTOP>1)
			iSTRTSTOP=0;
		pwndSTRT_STOP->SetPropValueT("captionID",Str_STRTSTOP[iSTRTSTOP]);	
		pwndSTRT_STOP->CreateA();
		pwndSTRT_STOP->Update();
	}
	else if(pwnd == pwndON_OFF)
	{
		iON_OFF++;
		if(iON_OFF>1)
			iON_OFF=0;
		if(ActionType==Action_Sub) // Sub
			pwndON_OFF->SetPropValueT("captionID",Str_STRT_END[iON_OFF]);
		else
			pwndON_OFF->SetPropValueT("captionID",Str_ON_OFF[iON_OFF]);	
		pwndON_OFF->CreateA();
		pwndON_OFF->Update();
	}
	
	else if(pwnd == pwndValveCheck)
	{
		iValveCheck++;
		if(iValveCheck>1)
			iValveCheck=0;
		pwndValveCheck->SetPropValueT("captionID",Str_ValveCheck[iValveCheck]);	
		pwndValveCheck->CreateA();
		pwndValveCheck->Update();
	}
	
	else if(pwnd == pwndOPEN_MODE)
	{
		iOpenMode++;
		if(iOpenMode>2)
			iOpenMode=0;
		pwndOPEN_MODE->SetPropValueT("captionID",Str_OpenMode[iOpenMode]);	
		pwndOPEN_MODE->CreateA();
		pwndOPEN_MODE->Update();
	}
	else if(pwnd == pwndEND_MODE)
	{
		iEndMode++;
		if(iEndMode>1)
			iEndMode=0;
		pwndEND_MODE->SetPropValueT("captionID",Str_EndMode[iEndMode]);	
		pwndEND_MODE->CreateA();
		pwndEND_MODE->Update();
		pwndStaticEnd1->SetPropValueT("textID",Str_StaticEnd1[iEndMode]);
		pwndStaticEnd1->CreateA();
		pwndStaticEnd1->Update();
		pwndStaticEnd2->SetPropValueT("textID",Str_StaticEnd2[iEndMode]);
		pwndStaticEnd2->CreateA();
		pwndStaticEnd2->Update();
		((CtmFormView*)pwndEditEnd1)->OnLoseFocus(); // 取消光標
		((CtmFormView*)pwndEditEnd2)->OnLoseFocus(); // 取消光標
		
		char 	pDataID[256];
		if(iEndMode)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER7",u_SubGroup,SelectNo);
			ACTIONPARA7 = GetDBValue(pDataID).lValue;
			pwndEditEnd2->SetPropValueT("value",ACTIONPARA7);
			pwndEditEnd2->SetPropValueT("max",100);
			pwndEditEnd2->SetPropValueT("precision",(double)0);
		}
		else
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
			ACTIONPARA6 = GetDBValue(pDataID).lValue;
			pwndEditEnd2->SetPropValueT("value",ACTIONPARA6);
			pwndEditEnd2->SetPropValueT("max",99999);
			pwndEditEnd2->SetPropValueT("precision",(double)2);
		}
		pwndEditEnd2->CreateA();
		pwndEditEnd2->Update();
	}
	
	else if(pwnd == pwndBtnSave) // 儲存
	{
		Save();
		g_Hint_Download = 1; // 提醒下載
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54", 0xFFFF); // Clear iSelectIndex
		Exit();
	}
	else if(pwnd == pwndBtnCancel) // 取消
	{
		if(SelectNo <= StandbyStepNum)
		{
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54", 0xFFFF); // Clear iSelectIndex
			Exit();
		}
		else
		{
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54", 0xFFFF); // Clear iSelectIndex
			Exit();
		}
	}
	else if (pwnd == pwndBtnWait) // 等待
	{
		MsgBoxCall("EditWindow_SUB_Wait2.txt");
	}
	else if( pwnd == pwndBtnPermit)
	{
		MsgBoxCall("EditWindow_SUB_Permit2.txt");
	}
	else if (pwnd == pwndBtnTag) // 標籤
	{
		MsgBoxCall("EditWindow_SUB_Tag2.txt");
	}
	else if (pwnd == pwndBtnGoto) // 跳轉
	{
		MsgBoxCall("EditWindow_SUB_Goto2.txt");
	}
	else if (pwnd == pwndBtnValve) // 閥門
	{
		MsgBoxCall("EditWindow_SUB_Valve2.txt");
	}
	else if (pwnd == pwndBtnDetect) // 檢測
	{
		MsgBoxCall("EditWindow_SUB_Detect2.txt");
	}
  return wIDControl;	
}

void  OnDestroyA(CtmWnd* pwndSender)
{
	SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 0);
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54", 0xFFFF);
}

/*===========================================================================
|          						help                                         |
+===========================================================================*/
void	Save()
{
	printf("Save\n");
	printf("SelectNo%d\n",SelectNo);
	char 	pDataID[256];
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,SelectNo);
	//ActionType =GetDBValue(pDataID).lValue;
	SetDBValue(pDataID, ActionType);
	printf("Set %s=%d\n",pDataID,ActionType);
	
	if(pwndSelectEditNUM != NULL)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,SelectNo);
		if(pwndSelectEditNUM->Is("CtmSelectEdit"))
		{
			EditNUM = ((CtmSelectEdit*)pwndSelectEditNUM)->GetIndex();
		}
//		else
//		{
//			pwndSelectEditNUM->GetPropValueT("value", &EditNUM,sizeof(EditNUM));
//		}
		if(pwndSelectEditNUM->Is("CtmToolButton"))
		{
			if(ActionType!=Action_Axis && ActionType!=Action_Pile && ActionType!=Action_Sub && iSelectIndex!=0xFFFF)
				EditNUM=iSelectIndex;
		}
		printf("Set %s=%d\n",pDataID,EditNUM);
		SetDBValue(pDataID, EditNUM);
	}
	if(pwndSelectGroupNum != NULL) // 群組
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER4",u_SubGroup,SelectNo);
		printf("Set %s=%d\n",pDataID,GroupNUM);
		SetDBValue(pDataID, GroupNUM);
	}
	if(pwndDetectMode != NULL) // 檢測模式：單點/區間
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
		SetDBValue(pDataID, iDetectMode);
	}
	if(pwndSTRT_STOP != NULL) // 檢測：開始/停止
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
		SetDBValue(pDataID, iSTRTSTOP);
	}
	if(pwndON_OFF != NULL) // 開關狀態 ON/OFF
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,SelectNo);
		printf("Save %s=%d\n",pDataID,iON_OFF);
		SetDBValue(pDataID, iON_OFF);
	}
	if(pwndValveCheck != NULL) // 閥門 檢測
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
		SetDBValue(pDataID, iValveCheck);
	}
	if(pwndOPEN_MODE != NULL) // 開模 模式
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
		SetDBValue(pDataID, iOpenMode);
	}
	if(pwndEND_MODE != NULL) // 結束 模式
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER8",u_SubGroup,SelectNo);
		printf("Set %s=%d\n",pDataID,iEndMode);
		SetDBValue(pDataID, iEndMode);
	}
	if(pwndEditEnd1 != NULL) // 提前距離 變速距離
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
		pwndEditEnd1->GetPropValueT("value", &ACTIONPARA3,sizeof(ACTIONPARA3));
		printf("Set %s=%d\n",pDataID,ACTIONPARA3);
		SetDBValue(pDataID,ACTIONPARA3);
	}
	if(pwndEditEnd2 != NULL) // 末段距離P6 / 二段速度P7
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		if(iEndMode)
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER7",u_SubGroup,SelectNo);
		else
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
		pwndEditEnd2->GetPropValueT("value", &ACTIONPARA67,sizeof(ACTIONPARA67));
		printf("Set %s=%d\n",pDataID,ACTIONPARA67);
		SetDBValue(pDataID,ACTIONPARA67);
	}
	if(pwndEditPos_X != NULL) // // 軸動作 點到點 X座標
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,SelectNo);
		pwndEditPos_X->GetPropValueT("value", &ACTIONPARA1,sizeof(ACTIONPARA1));
		printf("Set %s=%d\n",pDataID,ACTIONPARA1);
		SetDBValue(pDataID,ACTIONPARA1);
	}
	if(pwndEditPos_Y != NULL) // // 軸動作 點到點 Y座標
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER7",u_SubGroup,SelectNo);
		pwndEditPos_Y->GetPropValueT("value", &ACTIONPARA7,sizeof(ACTIONPARA7));
		printf("Set %s=%d\n",pDataID,ACTIONPARA7);
		SetDBValue(pDataID,ACTIONPARA7);
	}
	if(pwndEditPos_Z != NULL) // // 軸動作 點到點 Z座標
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER8",u_SubGroup,SelectNo);
		pwndEditPos_Z->GetPropValueT("value", &ACTIONPARA8,sizeof(ACTIONPARA8));
		printf("Set %s=%d\n",pDataID,ACTIONPARA8);
		SetDBValue(pDataID,ACTIONPARA8);
	}
	if(pwndPerAdv != NULL) // 軸動作 點到點 提前百分比 PARA3
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
		pwndPerAdv->GetPropValueT("value", &ACTIONPARA3,sizeof(ACTIONPARA3));
		printf("Set %s=%d\n",pDataID,ACTIONPARA3);
		SetDBValue(pDataID,ACTIONPARA3);
	}
	if(pwndPerEnd != NULL) // 軸動作 點到點 末段百分比 PARA6
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
		pwndPerEnd->GetPropValueT("value", &ACTIONPARA6,sizeof(ACTIONPARA6));
		printf("Set %s=%d\n",pDataID,ACTIONPARA6);
		SetDBValue(pDataID,ACTIONPARA6);
	}
	
	if(pwndEditACTIONPARA1 != NULL)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,SelectNo);
		if(pwndEditACTIONPARA1->Is("CtmSelectEdit"))
		{
			ACTIONPARA1 = ((CtmSelectEdit*)pwndEditACTIONPARA1)->GetIndex();
		}
		else
		{
			pwndEditACTIONPARA1->GetPropValueT("value", &ACTIONPARA1,sizeof(ACTIONPARA1));
		}
		printf("Set %s=%d\n",pDataID,ACTIONPARA1);
		SetDBValue(pDataID, ACTIONPARA1);
	}
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup,SelectNo);
	if(pwndEditACTIONPARA2 != NULL) // 參數2
	{
		if(pwndEditACTIONPARA2->Is("CtmSelectEdit"))
		{
			ACTIONPARA2 = ((CtmSelectEdit*)pwndEditACTIONPARA2)->GetIndex();
		}
		else
		{
			pwndEditACTIONPARA2->GetPropValueT("value", &ACTIONPARA2,sizeof(ACTIONPARA2));
		}
		if((ActionType==1||ActionType==0x10) && (EditNUM <= 5) && ACTIONPARA2<=1)// 速度最小值為1
			{ACTIONPARA2=80;}		
		printf("Set %s=%d\n",pDataID,ACTIONPARA2);
		SetDBValue(pDataID, ACTIONPARA2);
	}

	if(pwndEditACTIONPARA3 != NULL)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup,SelectNo);
		pwndEditACTIONPARA3->GetPropValueT("value", &ACTIONPARA3,sizeof(ACTIONPARA3));
		printf("Set %s=%d\n",pDataID,ACTIONPARA3);
		SetDBValue(pDataID, ACTIONPARA3);
	}	
	
	if(pwndEditACTIONPARA5 != NULL)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5",u_SubGroup,SelectNo);
		pwndEditACTIONPARA5->GetPropValueT("value", &ACTIONPARA5,sizeof(ACTIONPARA5));
		printf("Set %s=%d\n",pDataID,ACTIONPARA5*10);
		SetDBValue(pDataID, ACTIONPARA5*10);
	}
	if(pwndEditACTIONPARA6 != NULL)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup,SelectNo);
		pwndEditACTIONPARA6->GetPropValueT("value", &ACTIONPARA6,sizeof(ACTIONPARA6));
		printf("Set %s=%d\n",pDataID,ACTIONPARA6);
		SetDBValue(pDataID, ACTIONPARA6);
	}
	
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED42", 1);
}

void		SetVisible(CtmWnd* pwnd, BOOL  bVisible, WORD  wType)
{
	if(pwnd != NULL)
	{
		if(wType)
			pwnd->SetPropValueT("wtype",wType); 
		else
		{
			if(bVisible)
				pwnd->SetPropValueT("wtype",0x23);
		   	else
        		pwnd->SetPropValueT("wtype",0x30);
        }

			pwnd->SetPropValueT("visible",(double)bVisible);
			pwnd->Update();
			pwnd->UpdateAll();
	}		
}

void		SetEnabled(CtmWnd* pwnd, BOOL  bVisible)
{
	if(pwnd != NULL)
	{
		pwnd->SetPropValueT("enabled",(double)bVisible);
		pwnd->UpdateAll();
		pwnd->UpdateAll();
	}
}

//void	UpdateText()						//更新顯示字串
//{
//	//printf("UpdateText\n");
//	char 	pDataID[256];
//	char 	pDataID2[256];
//	char 	pDataID3[256];
//	char 	pDataID4[256];
//	char 	pDataID5[256];
//	char 	pDataID6[256];
//	char 	pDataID7[256];
//	char 	pDataID8[256];
//	char 	pDataID9[256];
//	char 	pDataID10[256];
//	char 	pDataID11[256];
//	char 	pDataID12[256];
//	char    szActPara1[256];
//	char    szActPara2[256];
//	char    szActPara3[256];
//	char    szActPara5[256];
//	int	ACTIONTYPE =0,ACTIONNUM =0;
//	DWORD   wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
//	// ↓ 字串合併顯示 cjlee add 	
//	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
//	char	str[1024]; // 顯示字串用
//	char	path[1024]; // 顯示字串用
//	char* str_1;
//	char* str_2;
//	char* str_3;
//	char* str_4;
//	char* str_5;
//	char* str_6;
//	char* str_7;
//	char* str_8;
//	char* str_9;
//	char* str_10;
//	char* str_11;
//	char* str_12;
//	
//	// ↑ 字串合併顯示 cjlee add 
//	for(int i=0;i<iCheckBoxAct;i++)
//	{
//		memset(str,0,sizeof(str));
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",i+1+No1);
//		memset(pDataID2, 0 ,sizeof(pDataID2));
//		sprintf(pDataID2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",i+1+No1);
//		sprintf(szActPara1,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1", i+1+No1); 
//		sprintf(szActPara2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2", i+1+No1); 
//		sprintf(szActPara3,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3", i+1+No1); 
//		sprintf(szActPara5,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5", i+1+No1); 
//		ACTIONTYPE =GetDBValue(pDataID).lValue;
//		ACTIONNUM =GetDBValue(pDataID2).lValue;
//		wActPara1  = GetDBValue(szActPara1).lValue;
//		wActPara2  = GetDBValue(szActPara2).lValue;
//		wActPara3  = GetDBValue(szActPara3).lValue;
//		wActPara5  = GetDBValue(szActPara5).lValue;
//		//printf("LINE%d, ACTIONTYPE:%d, ACTIONNUM:%d, Para1:%d, Para2:%d, Para3:%d, Para5:%d\n",i+1+No1,ACTIONTYPE,ACTIONNUM,wActPara1,wActPara2,wActPara3,wActPara5);
//		memset(pDataID, 0 ,sizeof(pDataID));
//		memset(pDataID2, 0 ,sizeof(pDataID2));
//		memset(pDataID3, 0 ,sizeof(pDataID3));
//		memset(pDataID4, 0 ,sizeof(pDataID4));
//		memset(pDataID5, 0 ,sizeof(pDataID5));
//		memset(pDataID6, 0 ,sizeof(pDataID6));
//		memset(pDataID7, 0 ,sizeof(pDataID7));
//		memset(pDataID8, 0 ,sizeof(pDataID8));
//		memset(pDataID9, 0 ,sizeof(pDataID9));
//		memset(pDataID10,0 ,sizeof(pDataID10));
//		memset(pDataID11,0 ,sizeof(pDataID11));
//		memset(pDataID12,0 ,sizeof(pDataID12));
//
//		//printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
//		switch(ACTIONTYPE)
//		{
//			case 0:		//
//				sprintf(pDataID,"VW_PICKER_NULL");
//				break;
//			case 1:		//軸動作
//				if(wActPara5) // 延時
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "延時"
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "秒"
//				}
//				switch(ACTIONNUM)
//				{
//					case 0:
//						sprintf(pDataID4,"VW_PICKER_NULL");
//						break;
//					case 1:
//						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X軸"
//						break;
//					case 2:
//						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y軸"
//						break;
//					case 3:
//						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z軸"
//						break;
//					default:
//						break;
//				}
//				if(ACTIONNUM <= 3)
//				{
//					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "以"
//					sprintf(pDataID6,"%3d",wActPara2);
//					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "的速度移動到"
//					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
//					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
//					if(wActPara3)
//					{
//						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
//						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
//						sprintf(pDataID12,"VW_CHARGE_MM");
//					}
//				}
//				else if(ACTIONNUM == 4)
//				{
//					if(wActPara2)
//						sprintf(pDataID4,"PICKER_REMOTE_O_14");
//					else
//						sprintf(pDataID4,"PICKER_REMOTE_O_13");
//					if(wActPara3)
//						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
//					else
//						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
//				}
//				break;
//			case 2:		//等待
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_WAIT");
//				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 3:		//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_INNER_O_0%d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 4:		//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_VALVE");
//				sprintf(pDataID5,"DLG_DATETIME_COLON");
//				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
//				if(wActPara2)
//				{
//					sprintf(pDataID8,"PICKER_ACTTIME");
//					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
//					sprintf(pDataID10,"VW_INJPRO_SECOND");
//				}
//				break;
//			case 5:		//標籤
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				if(ACTIONNUM == 1)
//				{
//					sprintf(pDataID4,"ACTIONPOINT_START");
//				}
//				else
//				{
//					sprintf(pDataID4,"PICKER_LABEL_%d",ACTIONNUM);
//				}
//				break;
//			case 6:		//跳轉
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				if(ACTIONNUM == 1)
//				{
//					sprintf(pDataID4,"ACTIONPOINT_END");
//				}
//				else
//				{
//					sprintf(pDataID4,"PICKER_JUMP");
//					sprintf(pDataID5,"PICKER_LABEL_%d",ACTIONNUM);
//				}
//				break;
//			case 8:		//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"VW_PUMP_TEST");
//				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 12:	//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				if(ACTIONNUM)
//					sprintf(pDataID4,"VW_PID_GROUP%d",ACTIONNUM);
//				else
//					sprintf(pDataID4,"VW_PICKER_NULL");
//				sprintf(pDataID5,"VW_PICKER_PILE");
//				if(wActPara1)
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 13:	//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_SUBTECH_1");
//				switch(ACTIONNUM)
//				{
//					case 0:
//						sprintf(pDataID5,"VW_PICKER_NULL");
//						break;
//					case 1:
//						sprintf(pDataID5,"PICKER_PROD_BAD");
//						break;
//					case 2:
//						sprintf(pDataID5,"PICKER_PROD_SAMPLE");
//						break;
//					case 3:
//						sprintf(pDataID5,"PICKER_PROD_TEST");
//						break;
//					case 4:
//						sprintf(pDataID5,"VW_AD_RESERVED");
//						break;
//					case 5:
//						sprintf(pDataID5,"VW_AD_RESERVED");
//						break;
//					default:
//						break;
//				}
//				if(wActPara1)
//					sprintf(pDataID6,"ACTIONPOINT_START");
//				else
//					sprintf(pDataID6,"ACTIONPOINT_END");
//				sprintf(pDataID7,"PICKER_SUBTECH_2");
//				sprintf(pDataID8,"%2d",wActPara2);
//				break;
//			default: 
//				break;	
//		}
//		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
//		// ↓ 字串合併顯示 cjlee add 
//		index_1 = g_MultiLanguage.GetStrKeyIndex(pDataID);
//		index_2 = g_MultiLanguage.GetStrKeyIndex(pDataID2);	
//		index_3 = g_MultiLanguage.GetStrKeyIndex(pDataID3);	
//		index_4 = g_MultiLanguage.GetStrKeyIndex(pDataID4);
//		index_5 = g_MultiLanguage.GetStrKeyIndex(pDataID5);	
//		index_6 = g_MultiLanguage.GetStrKeyIndex(pDataID6);	
//		index_7 = g_MultiLanguage.GetStrKeyIndex(pDataID7);
//		index_8 = g_MultiLanguage.GetStrKeyIndex(pDataID8);	
//		index_9 = g_MultiLanguage.GetStrKeyIndex(pDataID9);	
//		index_10 = g_MultiLanguage.GetStrKeyIndex(pDataID10);
//		index_11 = g_MultiLanguage.GetStrKeyIndex(pDataID11);	
//		index_12 = g_MultiLanguage.GetStrKeyIndex(pDataID12);	
//		
//		if (index_1<0)
//			index_1 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_2<0)
//			str_2 = pDataID2;
//		else
//			str_2 = g_MultiLanguage[index_2];
//		if (index_3<0)
//			index_3 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_4<0)
//			index_4 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_5<0)
//			index_5 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_6<0)
//			str_6 = pDataID6;
//		else
//			str_6 = g_MultiLanguage[index_6];
//		if (index_7<0)
//			index_7 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_8<0)
//			str_8 = pDataID8;
//		else
//			str_8 = g_MultiLanguage[index_8];
//		if (index_9<0)
//			index_9 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_10<0)
//			index_10 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_11<0)
//			str_11= pDataID11;
//		else
//			str_11 = g_MultiLanguage[index_11];
//		if (index_12<0)
//			index_12 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		//printf("Line%d = %d  %d  %d %d  %d  %d %d  %d  %d %d  %d  %d\n",i+1+No1,index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12);
//		
//		str_1 = g_MultiLanguage[index_1];
//		//str_2 = g_MultiLanguage[index_2];
//		str_3 = g_MultiLanguage[index_3];
//		str_4 = g_MultiLanguage[index_4];
//		str_5 = g_MultiLanguage[index_5];
//		//str_6 = g_MultiLanguage[index_6];
//		str_7 = g_MultiLanguage[index_7];
//		//str_8 = g_MultiLanguage[index_8];
//		str_9 = g_MultiLanguage[index_9];
//		str_10 = g_MultiLanguage[index_10];
//		//str_11 = g_MultiLanguage[index_11];
//		str_12 = g_MultiLanguage[index_12];
//		
//		memset(str,0,sizeof(str));
//		//printf("sl_1=%d, sl_2=%d, sl_3=%d, sl_4=%d, sl_5=%d, sl_6=%d, sl_7=%d, sl_8=%d, sl_9=%d, sl_10=%d, sl_11=%d, sl_12=%d\n",strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
//		strncat(str, str_1, strlen(str_1));
//		strncat(str, str_2, strlen(str_2));
//		strncat(str, str_3, strlen(str_3));
//		strncat(str, str_4, strlen(str_4));
//		strncat(str, str_5, strlen(str_5));
//		strncat(str, str_6, strlen(str_6));
//		strncat(str, str_7, strlen(str_7));
//		strncat(str, str_8, strlen(str_8));
//		strncat(str, str_9, strlen(str_9));
//		strncat(str, str_10, strlen(str_10));
//		strncat(str, str_11, strlen(str_11));
//		strncat(str, str_12, strlen(str_12));
//		printf("strlen=%s,d=%d\n",str,str);
//		//printf("strlen=%d\n",strlen(str));
//		//strncpy(path, str, strlen(str)+1);
//		//path[strlen(str)]= '\0';
//		//printf("path=%d\n",path);
///*
//		memset(str,0,sizeof(str));
//		printf("strlen:%d\n",strlen(str));
//		memcpy(str,str_1,strlen(str_1));
//		memcpy(str+strlen(str_1),str_2,strlen(str_2));
//		memcpy(str+strlen(str_1)+strlen(str_2),str_3,strlen(str_3));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3),str_4,strlen(str_4));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4),str_5,strlen(str_5));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5),str_6,strlen(str_6));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6),str_7,strlen(str_7));	
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7),str_8,strlen(str_8));		
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8),str_9,strlen(str_9));	
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9),str_10,strlen(str_10));	
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10),str_11,strlen(str_11));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11),str_12,strlen(str_12));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11)+strlen(str_12),"",strlen(""));
//		printf("%d str%d strlen: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",i+1+No1,strlen(str),strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
//*/
//		
//		if(pwndCheckBoxAct[i] != NULL)
//		{
//			pwndCheckBoxAct[i]->SetPropValueT("captionID",str);
//			/*pwndCheckBoxAct[i]->SetPropValueT("captionID1", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID2", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID3", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID4", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID5", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID6", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID7", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID8", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID9", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID10", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID11", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID12", "");*/
//			pwndCheckBoxAct[i]->CreateA();
//			pwndCheckBoxAct[i]->Update();
//			pwndCheckBoxAct[i]->UpdateAll();
//		}
//		//pwndCheckBoxAct[i]->Update();
//		// ↑ 字串合併顯示 cjlee add 
//		/*
//		pwndCheckBoxAct[i]->SetPropValueT("captionID1", pDataID);printf("pDataID1:%s",pDataID);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID2", pDataID2);printf("pDataID2:%s",pDataID2);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID3", pDataID3);printf("pDataID3:%s",pDataID3);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID4", pDataID4);printf("pDataID4:%s",pDataID4);
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
//		pwndCheckBoxAct[i]->UpdateAll();*/		
//	}
//}
void	UpdateText()						//更新顯示字串
{
	//printf("UpdateText\n");
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
	char    szActPara1[256];
	char    szActPara2[256];
	char    szActPara3[256];
	char    szActPara5[256];
	int	ACTIONTYPE =0,ACTIONNUM =0;
	DWORD   wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
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
	for(int i=0;i<iCheckBoxAct;i++)
	{
		memset(str,0,sizeof(str));
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",i+1+No1);
		memset(pDataID2, 0 ,sizeof(pDataID2));
		sprintf(pDataID2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",i+1+No1);
		sprintf(szActPara1,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1", i+1+No1); 
		sprintf(szActPara2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2", i+1+No1); 
		sprintf(szActPara3,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3", i+1+No1); 
		sprintf(szActPara5,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5", i+1+No1); 
		ACTIONTYPE =GetDBValue(pDataID).lValue;
		ACTIONNUM =GetDBValue(pDataID2).lValue;
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		//printf("LINE%d, ACTIONTYPE:%d, ACTIONNUM:%d, Para1:%d, Para2:%d, Para3:%d, Para5:%d\n",i+1+No1,ACTIONTYPE,ACTIONNUM,wActPara1,wActPara2,wActPara3,wActPara5);
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

		//printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
		switch(ACTIONTYPE)
		{
			case 0:		//
				sprintf(pDataID,"VW_PICKER_NULL");
				break;
			case 1:		//軸動作
				if(wActPara5) // 延時
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "延時"
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "秒"
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X軸"
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y軸"
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z軸"
						break;
					case 4:
						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2軸"
						break;
					case 5:
						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2軸"
						break;
					default:
						break;
				}
				if(ACTIONNUM <= 5)
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "以"
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "的速度移動到"
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				else if(ACTIONNUM == 6)
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
			case 2:		//等待
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_WAIT");
				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 3:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_INNER_O_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 4:		//閥門
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_VALVE");
				sprintf(pDataID5,"DLG_DATETIME_COLON");
				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
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
			case 5:		//標籤
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_START");
				}
				else
				{
					sprintf(pDataID4,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 6:		//跳轉
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_END");
				}
				else
				{
					sprintf(pDataID4,"PICKER_JUMP");
					sprintf(pDataID5,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 8:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"VW_PUMP_TEST");
				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 12:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM)
					sprintf(pDataID4,"VW_PID_GROUP%d",ACTIONNUM);
				else
					sprintf(pDataID4,"VW_PICKER_NULL");
				sprintf(pDataID5,"VW_PICKER_PILE");
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 13:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_SUBTECH_1");
				switch(ACTIONNUM)
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
		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
		// ↓ 字串合併顯示 cjlee add 
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
		printf("Line%d = %s\n",i+1+No1-StandbyStepNum,str);
		//printf("strlen=%d\n",strlen(str));
		//strncpy(path, str, strlen(str)+1);
		//path[strlen(str)]= '\0';
		//printf("path=%d\n",path);
/*
		memset(str,0,sizeof(str));
		printf("strlen:%d\n",strlen(str));
		memcpy(str,str_1,strlen(str_1));
		memcpy(str+strlen(str_1),str_2,strlen(str_2));
		memcpy(str+strlen(str_1)+strlen(str_2),str_3,strlen(str_3));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3),str_4,strlen(str_4));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4),str_5,strlen(str_5));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5),str_6,strlen(str_6));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6),str_7,strlen(str_7));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7),str_8,strlen(str_8));		
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8),str_9,strlen(str_9));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9),str_10,strlen(str_10));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10),str_11,strlen(str_11));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11),str_12,strlen(str_12));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11)+strlen(str_12),"",strlen(""));
		printf("%d str%d strlen: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",i+1+No1,strlen(str),strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
*/
		if(pwndCheckBoxAct[i] != NULL)
		{
			pwndCheckBoxAct[i]->SetPropValueT("captionID","");
			//pwndCheckBoxAct[i]->CreateA();
			//pwndCheckBoxAct[i]->Show();
			//pwndCheckBoxAct[i]->Update();
			//pwndCheckBoxAct[i]->UpdateAll();
		}
		if(pwndStaticAct[i] != NULL)
		{
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			//pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			//pwndStaticAct[i]->UpdateAll();
		}
		//pwndCheckBoxAct[i]->Update();
		// ↑ 字串合併顯示 cjlee add 
	}
}