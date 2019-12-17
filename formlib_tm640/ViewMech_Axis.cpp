/*===========================================================================+
|  Class    : ViewGuideSet library                                           |
|  Task     : ViewGuideSet library action source file                        |
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
#include 	"ViewMech_Axis.h"
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
#include 	"../selectedit.h"
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		TransType_D			0	// 節圓直徑
#define		TransType_Tooth	1 // 齒數模數
#define		EncType_Abs			0x00000000  // 絕對
#define		EncType_Res		  0x00010000  // 增量
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次

int u_Axis_Num = 0;
BOOL 				b_Dir[5] ={FALSE};
BOOL 				b_Double[5] ={FALSE};

CtmWnd*	u_pwndEdit_DOUBLE[5] = {NULL}; // 倍數機構
CtmWnd*	u_pwndDOUBLE_POS_INV[5] = {NULL}; // 位置反向

/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
CtmWnd*			pwndBtn_MechType3 = NULL; // 機型選擇 3軸 Btn
CtmWnd*			pwndBtn_MechType5 = NULL; // 機型選擇 5軸 Btn
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
CtmWnd*			pwndBtn_Gear_D 		= NULL; // 節圓直徑 Btn
CtmWnd*			pwndBtn_Tooth_M 	= NULL; // 齒數模數 Btn
int u_TransType = 0; // 傳動方式  0-節圓直徑 1-齒數模數
CtmWnd*			pwndBtn_EncType_Abs 		= NULL; // 編碼器選擇 絕對 Btn
CtmWnd*			pwndBtn_EncType_Res 		= NULL; // 編碼器選擇 增量 Btn
int u_EncType = 0; // 編碼器選擇  0-絕對 1-增量

CtmWnd*		pwndBtn_Axis[6] 	={NULL}; // 各軸Btn
int iBtn_Axis = 0;

CtmWnd*		pwndMask_MechData	= NULL; // 機構參數遮罩

CtmWnd*		pwndMech_Data[6] 	={NULL}; // 各軸 參數
char* Mech_Data_String[] = // 參數 元件 名稱
{
	"edit_ENC_REZ", 				// 編碼器解析度
	"edit_Motor_Ratio", 	  // 減速比
	"edit_Motor_Diameter",	// 節圓直徑
	"edit_Tooth_Num",				// 齒數
	"edit_M_Num",						// 模數
};

CtmWnd*		pwndDOUBLE 	={NULL}; // 各軸 參數 倍數 
CtmWnd*		pwndPOS_INV ={NULL}; // 各軸 參數 位置反向 


char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸
char* pTransTypeDB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE"; // 傳動方式DB

long MechPara = 0; // 傳動方式 數值

char*	u_pszEditDoubleName[] = // 倍數機構 輸入元件 名稱
{
	"Btn_DOUBLE_X",
	"Btn_DOUBLE_Y",
	"Btn_DOUBLE_Z",
	"Btn_DOUBLE_X2",
	"Btn_DOUBLE_Y2"
}; 

char*	u_pszEditPOSINVName[] = // 位置反向 輸入元件 名稱
{
	"Btn_POS_INV_X",
	"Btn_POS_INV_Y",
	"Btn_POS_INV_Z",
	"Btn_POS_INV_X2",
	"Btn_POS_INV_Y2"
};

char*	Str_Double[] = // 倍數機構使用與否 文字
{
	"MECH_UNUSE", // 使用
	"MECH_USE", // 不用
};
char*	Str_POSINV[] = // 位置反向 文字
{
	"VW_ROTATELOC_ANGELREVERSED_OPT1", // 正向
	"VW_ROTATELOC_ANGELREVERSED_OPT2", // 反向
};
char*	Img_Btn[] =
{
	"res_tm640/pic/ButtonUp.bmp",
	"res_tm640/pic/ButtonDown.bmp",
};
char*	Str_PICKER_Type[] = // 機構選擇 文字
{
	"TYPE_3AXIS", // 3軸
	"TYPE_5AXIS", // 5軸
};
long	Color_Btn[] = // 按鍵選擇 顏色
{
	0xD6BA,
	0xEF32,//0x8DA0,
};

BOOL TF[] =
{
	FALSE,
	TRUE
};

char*	u_pszEditDoubleDB[] = // 紀錄倍數機構是否使用DB名稱
{
	"PICKER_PARAMETER_DOUBLE_X1",
	"PICKER_PARAMETER_DOUBLE_Y1",
	"PICKER_PARAMETER_DOUBLE_Z",
	"PICKER_PARAMETER_DOUBLE_X2",
	"PICKER_PARAMETER_DOUBLE_Y2"
}; 

char*	u_pszEditPOSINVDB[] = // 紀錄位置反向是否使用DB名稱
{
	"PICKER_PARAMETER_POS_INV_X1",
	"PICKER_PARAMETER_POS_INV_Y1",
	"PICKER_PARAMETER_POS_INV_Z",
	"PICKER_PARAMETER_POS_INV_X2",
	"PICKER_PARAMETER_POS_INV_Y2"
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

char* dbid0_Mech[6][5] = // 參數數值 db
{	//  編碼器解析度									,減速比														,節圓直徑													,齒數														 ,模數
	{NULL,NULL,NULL,NULL,NULL},	// 共同
	{"MACHINE_INTERFACE_CONTROL_RSV05","MACHINE_INTERFACE_CONTROL_RSV10","MACHINE_FUNCTION_OPTIONS_RSV22","MACHINE_FUNCTION_OPTIONS_RSV12","MACHINE_FUNCTION_OPTIONS_RSV17"},		// X1軸
	{"MACHINE_INTERFACE_CONTROL_RSV06","MACHINE_INTERFACE_CONTROL_RSV11","MACHINE_FUNCTION_OPTIONS_RSV23","MACHINE_FUNCTION_OPTIONS_RSV13","MACHINE_FUNCTION_OPTIONS_RSV18"},		// Y1軸
	{"MACHINE_INTERFACE_CONTROL_RSV07","MACHINE_INTERFACE_CONTROL_RSV12","MACHINE_FUNCTION_OPTIONS_RSV24","MACHINE_FUNCTION_OPTIONS_RSV14","MACHINE_FUNCTION_OPTIONS_RSV19"},		// Z軸
	{"MACHINE_INTERFACE_CONTROL_RSV08","MACHINE_INTERFACE_CONTROL_RSV13","MACHINE_FUNCTION_OPTIONS_RSV25","MACHINE_FUNCTION_OPTIONS_RSV15","MACHINE_FUNCTION_OPTIONS_RSV20"},		// X2軸
	{"MACHINE_INTERFACE_CONTROL_RSV09","MACHINE_INTERFACE_CONTROL_RSV14","MACHINE_FUNCTION_OPTIONS_RSV26","MACHINE_FUNCTION_OPTIONS_RSV16","MACHINE_FUNCTION_OPTIONS_RSV21"}			// Y2軸
};

CtmWnd*	pwndStrAxisData[6] 	 = {NULL}; // 各軸 參數 文字
char* 	StrAxisData_String[] = 				 // 參數 文字 名稱
{
	"Str_ENC_REZ", 				// 編碼器解析度
	"Str_Motor_Ratio", 	  // 減速比
	"Str_Motor_Diameter",	// 每轉距離
	"Str_Tooth_N",	// 每轉距離
	"Str_DOUBLE",					// 倍數機構
	"Str_POS_INV"					// 位置反向
};

CtmWnd*	pwndStrMechData[3] 	 = {NULL}; // 機構 共同參數 文字
char* 	StrMechData_String[] = 				 // 共同參數 文字 名稱
{
	"Str_MechType", // 機型選擇
	"Str_Trans", // 傳動方式
	"Str_EncType", // 編碼器選擇

};

/*---------------------------------------------------------------------------+
|           View Content - GuideSet                                          |
+---------------------------------------------------------------------------*/ 

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
	
	// 取得指標 機構選擇：3軸 5軸
	pwndBtn_MechType3     = pwndSender->FindControlFromName("Btn_MechType3");
	pwndBtn_MechType5     = pwndSender->FindControlFromName("Btn_MechType5");
	// 取得指標 傳動方式：節圓直徑 齒數模數
	pwndBtn_Gear_D     		= pwndSender->FindControlFromName("Btn_Gear_D");
	pwndBtn_Tooth_M    		= pwndSender->FindControlFromName("Btn_Tooth_M");
	// 取得指標 編碼器選擇：絕對 增量
	pwndBtn_EncType_Abs     = pwndSender->FindControlFromName("Btn_EncType_Abs");
	pwndBtn_EncType_Res	    = pwndSender->FindControlFromName("Btn_EncType_Res");
	
	pwndMask_MechData 		  = pwndSender->FindControlFromName("Mask_MechData"); // 機構參數遮罩

	for(int i = 0; i < 5; i++) // 取得指標 
	{
		u_pwndEdit_DOUBLE[i] = pwndSender->FindControlFromName(u_pszEditDoubleName[i]);
		u_pwndDOUBLE_POS_INV[i] = pwndSender->FindControlFromName(u_pszEditPOSINVName[i]);
	}
	 
	iBtn_Axis	 	= GetSpecialControlNum(pwndSender, "Btn_Axis_", "CtmToolButton", pwndBtn_Axis);
	printf("iBtn_Axis=%d\n",iBtn_Axis);
	// 取得指標 機構參數 輸入元件
	for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
	{
		pwndMech_Data[i] = pwndSender->FindControlFromName(Mech_Data_String[i]);// 參數 元件
		printf("pwndMech_Data[%d] = %s\n",i,Mech_Data_String[i]);
	} 
	pwndDOUBLE				= pwndSender->FindControlFromName("Btn_DOUBLE");
	pwndPOS_INV				= pwndSender->FindControlFromName("Btn_POS_INV");
	
	// 取得指標 各軸參數 文字
	for(int i = 0; i < sizeof(StrAxisData_String)/sizeof(StrAxisData_String[0]); i++ )
	{
		pwndStrAxisData[i] = pwndSender->FindControlFromName(StrAxisData_String[i]);// 參數 元件
	} 	
	// 取得指標 共同參數 文字
	for(int i = 0; i < sizeof(StrMechData_String)/sizeof(StrMechData_String[0]); i++ )
	{
		pwndStrMechData[i] = pwndSender->FindControlFromName(StrMechData_String[i]);// 參數 元件
	} 		
	
	// Update Data def
	UpdateData();
	UpdateBtnData();
	//============================Update string============================
	
	printf("Update string\n");
	for(int i = 0; i < 5; i++)
	{
		if(u_pwndEdit_DOUBLE[i] != NULL) // 倍數機構
			{
				int u_Double = (int)(GetDBValue(u_pszEditDoubleDB[i]).lValue);
				//printf("%s=%d\n",u_pszEditDoubleDB[i],u_Double);
				b_Double[i]=TF[u_Double];
				u_pwndEdit_DOUBLE[i]->SetPropValueT("captionID",Str_Double[u_Double]);
				u_pwndEdit_DOUBLE[i]->SetPropValueT("bgc",Color_Btn[u_Double]);		
				u_pwndEdit_DOUBLE[i]->CreateA();
				u_pwndEdit_DOUBLE[i]->Update();
				if(u_Double == 1)
					((CtmButton*)u_pwndEdit_DOUBLE[i])->Press(tmBUTTON_DOWN);
			}
		if(u_pwndDOUBLE_POS_INV[i] != NULL) // 位置反向
			{
				int u_POSINV = (int)(GetDBValue(u_pszEditPOSINVDB[i]).lValue);
				b_Dir[i]=TF[u_POSINV];
				u_pwndDOUBLE_POS_INV[i]->SetPropValueT("captionID",Str_POSINV[u_POSINV]);
				u_pwndDOUBLE_POS_INV[i]->SetPropValueT("bgc",Color_Btn[u_POSINV]);	
				u_pwndDOUBLE_POS_INV[i]->CreateA();
				u_pwndDOUBLE_POS_INV[i]->Update();	
				if(u_POSINV == 1)
					((CtmButton*)u_pwndDOUBLE_POS_INV[i])->Press(tmBUTTON_DOWN);
			}
	}
	
	// 機型選擇 編碼器選擇
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	u_EncType		 = dw_MechType & EncWord;
	UpdateMechType();
	UpdateEncType();
	// 傳動方式
	u_TransType = (int)(GetDBValue(pTransTypeDB).lValue);
	UpdateTransType();
	
	return TRUE;
}

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
//WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
//{
//	CtmWnd* pWnd = NULL;
//	DWORD		dwTempValue = 0;
//
//	pWnd = pwndSender->FindControlFromTab(wIDControl);
//	if(pWnd == NULL) return wIDControl;
//		
//	return wIDControl;
//}

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
	printf("key = %d\n", wKey);
	((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標
	
	for(int i=0;i<iBtn_Axis;i++) // 軸按鍵變灰
	{
		pwndBtn_Axis[i]->SetPropValueT("upbitmap","res_tm640/pic/ButtonUp1.bmp");
		pwndBtn_Axis[i]->CreateA();
		pwndBtn_Axis[i]->Update();
	}
	
	switch(wKey)
	{
		case 1:
    	u_Axis_Num=1;
    	ShowMechData(FALSE); // 遮蓋 機構參數
    	ShowAxisData(TRUE);  // 顯示 各軸參數
			break;
		case 2:
    	u_Axis_Num=2;
    	ShowMechData(FALSE); // 遮蓋 機構參數
    	ShowAxisData(TRUE);  // 顯示 各軸參數
			break;	
		case 3:
			u_Axis_Num=3;
			ShowMechData(FALSE); // 遮蓋 機構參數
			ShowAxisData(TRUE);  // 顯示 各軸參數
			break;
		case 4:
			u_Axis_Num=4;
			ShowMechData(FALSE); // 遮蓋 機構參數
			ShowAxisData(TRUE);  // 顯示 各軸參數
			break;
		case 5:
			u_Axis_Num=5;
			ShowMechData(FALSE); // 遮蓋 機構參數
			ShowAxisData(TRUE);  // 顯示 各軸參數
			break;
		case 46:
			SendCommand(0xFF32); 
			break;
		case 77:
			u_Axis_Num=0;
			ShowAxisData(FALSE); // 遮蓋 各軸參數
			ShowMechData(TRUE);  // 顯示 機構參數
			break;
		case 7178:
			SendCommand(0xFF32); 
			break;
		default:
			break;
	}

	if(pwndBtn_Axis[u_Axis_Num]) // 選取的軸按鍵
	{	
		pwndBtn_Axis[u_Axis_Num]->SetPropValueT("upbitmap","res_tm640/pic/ButtonDown1.bmp");
		pwndBtn_Axis[u_Axis_Num]->CreateA();
		pwndBtn_Axis[u_Axis_Num]->Update();
	}
	
	if(u_Axis_Num!=0)
	{
		UpdateData(); // 更新資料 定義
		UpdateBtnData(); // 更新資料 倍數機構 位置反向
	}

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}
/*---------------------------------------------------------------------------+
|  Function : OnMessage()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY) // Toolbar Btn
		{
			printf("Toolbar =%d\n",wParam);
			switch(wParam)
			{
				case _F1: // 主頁
						::PutCommand("Over.txt");
					break;	
				case _F2: // 主選單
					::PutCommand("Index.txt");
					break;
				case _F5: // 返回
					::PutCommand("Index.txt");
					break;	
				default:
					break;
			}
		}
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
	printf("OnMouseUp\n");	
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;


	if((pwnd == pwndDOUBLE) || (pwnd == pwndPOS_INV) || (pwnd == pwndBtn_Gear_D) || (pwnd == pwndBtn_Tooth_M) )
	{
		if(pwnd == pwndPOS_INV) // 位置反向 Btn
			{
				int nTemp =0;
				b_Dir[u_Axis_Num-1] = !b_Dir[u_Axis_Num-1];
				nTemp = b_Dir[u_Axis_Num-1];
				SetDBValue(u_pszEditPOSINVDB[u_Axis_Num-1],nTemp);
				int u_POSINV = (int)(GetDBValue(u_pszEditPOSINVDB[u_Axis_Num-1]).lValue);

				//Update string
				pwndPOS_INV->SetPropValueT("captionID",Str_POSINV[u_POSINV]);
				pwndPOS_INV->SetPropValueT("bgc",Color_Btn[u_POSINV]);
				pwndPOS_INV->CreateA();
				pwndPOS_INV->Update();
				if(u_POSINV == 1)
					((CtmButton*)pwndPOS_INV)->Press(tmBUTTON_DOWN);
			}
		if(pwnd == pwndDOUBLE) // 倍數機構 Btn
			{
				printf("Press pwndDOUBLE\n");
				printf("u_Axis_Num=%d\n",u_Axis_Num);
				int nTemp =0;
				b_Double[u_Axis_Num-1] = !b_Double[u_Axis_Num-1];
				nTemp = b_Double[u_Axis_Num-1];
				SetDBValue(u_pszEditDoubleDB[u_Axis_Num-1],nTemp);
				int u_Double = (int)(GetDBValue(u_pszEditDoubleDB[u_Axis_Num-1]).lValue);
				printf("Set %s =%d\n",u_pszEditDoubleDB[u_Axis_Num-1],u_Double);
				//Update string
				pwndDOUBLE->SetPropValueT("captionID",Str_Double[u_Double]);
				pwndDOUBLE->SetPropValueT("bgc",Color_Btn[u_Double]);
				pwndDOUBLE->CreateA();
				pwndDOUBLE->Update();
				if(u_Double == 1)
					((CtmButton*)pwndDOUBLE)->Press(tmBUTTON_DOWN);
			}
		if(pwnd == pwndBtn_Gear_D) // 傳動方式 節圓直徑 Btn
			{
				u_TransType = TransType_D;
				SetDBValue(pTransTypeDB,TransType_D);
			}
		if(pwnd == pwndBtn_Tooth_M) // 傳動方式 齒數模數 Btn
			{
				u_TransType = TransType_Tooth;
				SetDBValue(pTransTypeDB,TransType_Tooth);
			}
			
		for(int i = 0; i < 5; i++) // 偵測按下放開 Btn
		{
			printf("%d", (int)GetDBValue(pTransTypeDB).lValue); 					// bit 0 傳動方式
			printf("%d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue); 	// bit 1 倍數機構
			printf("%d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue); // bit 2 位置反向
			
			printf(" <<1 = %d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue <<1); 
			printf(" <<1 <<1= %d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue <<2); 
			
			MechPara = (int)GetDBValue(pTransTypeDB).lValue + ((int)GetDBValue(u_pszEditDoubleDB[i]).lValue<<1) + ((int)GetDBValue(u_pszEditPOSINVDB[i]).lValue<<2);
			printf("MechPara = %x\n",MechPara);
			SetDBValue(u_pszMechPara[i],MechPara);
			SendCommand(0xFF32);
		}
	}

	if(pwnd == pwndBtn_MechType3) // 機型選擇 3軸 Btn
	{
		u_PickerType = MechType3;
		dw_MechType = u_PickerType+u_EncType;
		SetDBValue(pMechTypeDB,dw_MechType);
		printf("dw_MechType=%x\n",dw_MechType);
		UpdateMechType();
	}
	if(pwnd == pwndBtn_MechType5) // 機型選擇 5軸 Btn
	{
		u_PickerType = MechType5;
		dw_MechType = u_PickerType+u_EncType;
		SetDBValue(pMechTypeDB,dw_MechType);
		printf("dw_MechType=%x\n",dw_MechType);
		UpdateMechType();
	}
	if(pwnd == pwndBtn_EncType_Abs) // 
	{
		u_EncType = EncType_Abs;
		dw_MechType = u_PickerType+u_EncType;
		SetDBValue(pMechTypeDB,dw_MechType);
		printf("dw_MechType=%x\n",dw_MechType);
		UpdateEncType();
	}
	if(pwnd == pwndBtn_EncType_Res) // 
	{
		u_EncType = EncType_Res;
		dw_MechType = u_PickerType+u_EncType;
		SetDBValue(pMechTypeDB,dw_MechType);
		printf("dw_MechType=%x\n",dw_MechType);
		UpdateEncType();
	}

	if( (pwnd == pwndBtn_Gear_D) || (pwnd == pwndBtn_Tooth_M) 
		|| (pwnd == pwndBtn_MechType3) || (pwnd == pwndBtn_MechType5) ) // 傳動方式 || 機型選擇 Btn 被按下
	{
		if( (u_PickerType==MechType3) && (u_TransType==TransType_D) )			 		 // 三軸 節圓直徑
		{
			::PutCommand("Mech_Axis3.txt");
		}
		else if( (u_PickerType==MechType3) && (u_TransType==TransType_Tooth) ) // 三軸 齒數模數
		{
			::PutCommand("Mech_Axis3_TN.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_D) ) 		 // 五軸 節圓直徑
		{
			::PutCommand("Mech_Axis.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_Tooth) ) // 五軸 齒數模數
		{
			::PutCommand("Mech_Axis_TN.txt");
		}
	}

  
	
  return wIDControl;	
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
	if(!RunOnlyOne)
	{
		RunOnlyOne=TRUE;
		((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標

		ShowAxisData(FALSE); // 遮蓋 各軸參數
		ShowMechData(TRUE);  // 顯示 機構參數
	}
}

/*---------------------------------------------------------------------------+
|  Function : SendCommand()                      	     	                     |
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
			g_ptaskpicker->ReqValues(6, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
		}
}

/*---------------------------------------------------------------------------+
|  Function : UpdateMechType()                  										  		   |
|       		: 刷新	機型選擇顯示																						 |
+---------------------------------------------------------------------------*/
void	UpdateMechType()
{
	if(pwndBtn_MechType3 != NULL)
			{			
				if(u_PickerType==MechType3)
					{
						pwndBtn_MechType3->SetPropValueT("bgc",Color_Btn[1]);	
						((CtmButton*)pwndBtn_MechType3)->Press(tmBUTTON_DOWN);
					}
				else
					{
						pwndBtn_MechType3->SetPropValueT("bgc",Color_Btn[0]);	
						pwndBtn_MechType3->CreateA();
					}
				pwndBtn_MechType3->Update();
			}
	if(pwndBtn_MechType5 != NULL)
			{			
				if(u_PickerType==MechType5)
					{
						pwndBtn_MechType5->SetPropValueT("bgc",Color_Btn[1]);	
						((CtmButton*)pwndBtn_MechType5)->Press(tmBUTTON_DOWN);
					}
				else
					{
						pwndBtn_MechType5->SetPropValueT("bgc",Color_Btn[0]);	
						pwndBtn_MechType5->CreateA();
					}
				pwndBtn_MechType5->Update();
			}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateTransType()                  										  		   |
|       		: 刷新	傳動方式顯示																						 |
+---------------------------------------------------------------------------*/
void	UpdateTransType()
{
	if(pwndBtn_Gear_D != NULL)
			{			
				if(u_TransType==TransType_D) // 節圓直徑
					{
						pwndBtn_Gear_D->SetPropValueT("bgc",Color_Btn[1]);	
						((CtmButton*)pwndBtn_Gear_D)->Press(tmBUTTON_DOWN);
					}
				else
					{
						pwndBtn_Gear_D->SetPropValueT("bgc",Color_Btn[0]);	
						pwndBtn_Gear_D->CreateA();
					}
				pwndBtn_Gear_D->Update();
			}
	if(pwndBtn_Tooth_M != NULL)
			{			
				if(u_TransType==TransType_Tooth) // 齒數模數
					{
						pwndBtn_Tooth_M->SetPropValueT("bgc",Color_Btn[1]);	
						((CtmButton*)pwndBtn_Tooth_M)->Press(tmBUTTON_DOWN);
					}
				else
					{
						pwndBtn_Tooth_M->SetPropValueT("bgc",Color_Btn[0]);	
						pwndBtn_Tooth_M->CreateA();
					}
				pwndBtn_Tooth_M->Update();
			}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateEncType()                  											  		   |
|       		: 刷新	編碼器選擇顯示																					 |
+---------------------------------------------------------------------------*/
void	UpdateEncType()
{	
	if(pwndBtn_EncType_Abs != NULL)
			{			
				if(u_EncType==EncType_Abs)
					{
						pwndBtn_EncType_Abs->SetPropValueT("bgc",Color_Btn[1]);	
						((CtmButton*)pwndBtn_EncType_Abs)->Press(tmBUTTON_DOWN);
					}
				else
					{
						pwndBtn_EncType_Abs->SetPropValueT("bgc",Color_Btn[0]);	
						pwndBtn_EncType_Abs->CreateA();
					}
				pwndBtn_EncType_Abs->Update();
			}
	if(pwndBtn_EncType_Res != NULL)
			{			
				if(u_EncType==EncType_Res)
					{
						pwndBtn_EncType_Res->SetPropValueT("bgc",Color_Btn[1]);	
						((CtmButton*)pwndBtn_EncType_Res)->Press(tmBUTTON_DOWN);
					}
				else
					{
						pwndBtn_EncType_Res->SetPropValueT("bgc",Color_Btn[0]);	
						pwndBtn_EncType_Res->CreateA();
					}
				pwndBtn_EncType_Res->Update();
			}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateData()                      	     	                     |
|  Task     : 更新資料 定義	     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	UpdateData()
{
	//printf("UpdateData\n");
	for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ ) // Update Data def
	{
		if(pwndMech_Data[i]!=NULL) // Update Data def
		{
			pwndMech_Data[i]->SetPropValueT("dbid0",dbid0_Mech[u_Axis_Num][i]);
			//printf("Set pwndMech_Data[%d] = %s\n",i,dbid0_Mech[u_Axis_Num][i]);
			pwndMech_Data[i]->CreateA();
			pwndMech_Data[i]->Update();
		}
	}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateBtnData()                         	                     |
|  Task     : 更新資料 定義	     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	UpdateBtnData()
{
	if(pwndDOUBLE!=NULL)
	{
		int u_Double = (int)(GetDBValue(u_pszEditDoubleDB[u_Axis_Num-1]).lValue);
		b_Double[u_Axis_Num-1]=TF[u_Double];
		pwndDOUBLE->SetPropValueT("captionID",Str_Double[u_Double]);
		pwndDOUBLE->SetPropValueT("bgc",Color_Btn[u_Double]);	
		pwndDOUBLE->CreateA();
		pwndDOUBLE->Update();
		if(u_Double == 1)
			((CtmButton*)pwndDOUBLE)->Press(tmBUTTON_DOWN);
	}
	if(pwndPOS_INV!=NULL)
	{
		int u_POSINV = (int)(GetDBValue(u_pszEditPOSINVDB[u_Axis_Num-1]).lValue);
		b_Dir[u_Axis_Num-1]=TF[u_POSINV];
		pwndPOS_INV->SetPropValueT("captionID",Str_POSINV[u_POSINV]);
		pwndPOS_INV->SetPropValueT("bgc",Color_Btn[u_POSINV]);	
		pwndPOS_INV->CreateA();
		pwndPOS_INV->Update();
		if(u_POSINV == 1)
			((CtmButton*)pwndPOS_INV)->Press(tmBUTTON_DOWN);
	}
}

/*---------------------------------------------------------------------------+
|  Function : ShowE()                         	            				         |
|  Task     : 顯示遮住元件	     	                                           |
+----------------------------------------------------------------------------+
|  Parameter: pwnd 元件, enabled TRUE-顯示 FALSE-遮住						             |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ShowE(CtmWnd* pwnd,BOOL enabled)
{
	printf("ShowE\n");
	if(pwnd!=NULL)
	{
		printf("E enable=%d\n",enabled);
		pwnd->SetPropValueT("enabled",double(enabled));
		//pwnd->SetPropValueT("visible",double(enabled));
		if(enabled)
		{
			printf("TRUE\n");
			pwnd->Show();
		}
	}
}
/*---------------------------------------------------------------------------+
|  Function : ShowMechData(BOOL enabled)                           		       |
|  Task     : 顯示機構參數	     	                                           |
+----------------------------------------------------------------------------+
|  Parameter:             enabled TRUE-顯示 FALSE                            |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ShowMechData(BOOL enabled) // 顯示機構參數
{
	printf("ShowMechData\n");
	if(pwndMask_MechData!=NULL) // 遮罩
		pwndMask_MechData->Update();
		
	ShowE(pwndBtn_MechType3,enabled); 	// 顯示 3軸Btn
	ShowE(pwndBtn_MechType5,enabled); 	// 顯示 5軸Btn
	ShowE(pwndBtn_Gear_D,enabled); 			// 顯示 每轉距離Btn
	ShowE(pwndBtn_Tooth_M,enabled); 		// 顯示 齒數模數Btn
	ShowE(pwndBtn_EncType_Abs,enabled); // 顯示 絕對Btn
	ShowE(pwndBtn_EncType_Res,enabled); // 顯示 增量Btn
	
	for(int i = 0; i < sizeof(StrMechData_String)/sizeof(StrMechData_String[0]); i++ )
	{
		ShowE(pwndStrMechData[i],enabled); // 顯示 共同參數 文字
	} 
}

/*---------------------------------------------------------------------------+
|  Function : ShowAxisData(BOOL enabled)                           		       |
|  Task     : 顯示機構參數	     	                                           |
+----------------------------------------------------------------------------+
|  Parameter:             enabled TRUE-顯示 FALSE                            |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ShowAxisData(BOOL enabled) // 顯示各軸參數
{
	printf("ShowAxisData\n");
	if(pwndMask_MechData!=NULL) // 遮罩
		pwndMask_MechData->Update();
	for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
	{
		ShowE(pwndMech_Data[i],enabled); 	 // 顯示 各軸參數 元件
	} 

	ShowE(pwndDOUBLE,enabled); 	// 顯示 倍數 Btn
	ShowE(pwndPOS_INV,enabled); // 顯示 反向 Btn
	
	for(int i = 0; i < sizeof(StrAxisData_String)/sizeof(StrAxisData_String[0]); i++ )
	{
		ShowE(pwndStrAxisData[i],enabled); 	 // 顯示 各軸參數 文字
	} 	
	
}
