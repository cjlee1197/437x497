/*===========================================================================+
|  Class    : ViewCore library                                         		 	 |
|  Task     : ViewCore library action source file                       		 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                            	         |
|  Version  : V1.00                                                          |
|  Creation : 			                                                 			   |
|  Revision :           			                                      			   |
+===========================================================================*/
#include	"ViewQTeachSLVLSet.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸

// 軸數
#define		Axis_X1				1  // X1軸
#define		Axis_Y1				2  // Y1軸
#define		Axis_Z				3  // Z軸
#define		Axis_X2				4  // X2軸
#define		Axis_Y2				5  // Y2軸
#define		Axis_C				6  // C軸
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸

CtmWnd*		pwndBtnSetDone	= NULL; // 設定完成 按鈕

CtmWnd*		pwndPick_SLVL			= NULL; // 取件柔性 數值設定
CtmWnd*		pwndPickOut_SLVL	= NULL; // 取出柔性 數值設定

int u_Group=0;
char* QTeach_P_Page[2][20] = // 快速教導點 頁面
{
	{
		"",
		"QTeach_Mode.txt", 							// 1 模式選擇 
		"QTeach_WaitP_Axis3.txt", 			// 2 待機點 
		"", 														// 3 下待機點
		"QTeach_ReadyDownP_Axis3.txt", 	// 4 準備下降點 
		"QTeach_PickDownP_Axis3.txt", 	// 5 取出側下降點
		"QTeach_PickP_Axis3.txt", 			// 6 取出側取件點
		"QTeach_MoveOutP_Axis3.txt", 		// 7 橫出點
		"QTeach_PlaceDownP_Axis3.txt", 	// 8 置物側下降點
		"QTeach_PlaceP_Axis3.txt",			// 9 置物側置物點
	},
	{
		"",
		"QTeach_Mode.txt", // 模式選擇
		"QTeach_WaitP.txt", // 待機點
		"", // 下待機點
		"QTeach_ReadyDownP.txt", // 準備下降點 
		"QTeach_PickDownP.txt", // 取出側下降點
		"QTeach_PickP.txt", // 取出側取件點
		"QTeach_MoveOutP.txt", // 橫出點
		"QTeach_PlaceDownP.txt", // 置物側下降點
		"QTeach_PlaceP.txt", // 置物側置物點
	}
};

char* P_2_SLVL_Set_DB = "MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_TYPE"; // 從哪一點跳至 柔性設置 DB
char* Pick_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER1"; // 下降取物 提前距離 DB
char* PickOut_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER2"; // 上拉出模 提前距離 DB

int u_DownDis				 = 0; // 下降距離
int u_WaitPos_Y1 		 = 0; // 等待點 Y1 設定位置
int u_PickDownPos_Y1 = 0; // 取出下降點 Y1 設定位置
CtmWnd*		pwndPick_Dis			= NULL; // 下降距離 數值顯示

int u_BackDis				 = 0; // 後退距離
int u_MoveOutPos_X1	 = 0; // 橫出點 X1 設定位置
int u_PickPos_X1 		 = 0; // 取出取件點 X1 設定位置
CtmWnd*		pwndPickOut_Dis		= NULL; // 後退距離 數值顯示

char* P2_POSSET_DBString[] = // P2 等待點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P5_POSSET_DBString[] = // P5 取出側下降點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P6_POSSET_DBString[] = // P6 取出測取件點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P7_POSSET_DBString[] = // P7 取出側橫出點 位置設定 DB名稱
{
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER5",
};
/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	// 取得 機構設定 3 or 5軸
	u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	
	
	// 取得指標 設定完成 Btn
	pwndBtnSetDone = pwndSender->FindControlFromName("BtnSetDone");
	
	// 取得指標 取件柔性/取出柔性 數值設定 
	pwndPick_SLVL 	 = pwndSender->FindControlFromName("Pick_SLVL");
	pwndPickOut_SLVL = pwndSender->FindControlFromName("PickOut_SLVL");
	
	// 取得指標 下降距離/後退距離 數值顯示 
	pwndPick_Dis 	 = pwndSender->FindControlFromName("Pick_Dis");
	pwndPickOut_Dis = pwndSender->FindControlFromName("PickOut_Dis");
	
	// 計算 下降距離
	u_WaitPos_Y1 = GetDBValue(P2_POSSET_DBString[Axis_Y1]).lValue;
	printf("u_WaitPos_Y1=%d\n",u_WaitPos_Y1);
	u_PickDownPos_Y1 = GetDBValue(P5_POSSET_DBString[Axis_Y1]).lValue;
	printf("u_PickDownPos_Y1=%d\n",u_PickDownPos_Y1);
	u_DownDis = u_PickDownPos_Y1-u_WaitPos_Y1;
	printf("u_DownDis=%d\n",u_DownDis);
	pwndPick_SLVL->SetPropValueT("max",u_DownDis);
	if(pwndPick_Dis!=NULL)
		{
			pwndPick_Dis->SetPropValueT("value",u_DownDis);
			printf("Set pwndPick_Dis=%d\n",u_DownDis);
			pwndPick_Dis->CreateA();
			pwndPick_Dis->Show();
		}
	// 計算 後退距離
	u_MoveOutPos_X1 = GetDBValue(P7_POSSET_DBString[Axis_X1]).lValue;

	u_PickPos_X1 = GetDBValue(P6_POSSET_DBString[Axis_X1]).lValue;

	u_BackDis = abs(u_MoveOutPos_X1-u_PickPos_X1);
	printf("u_BackDis=%d\n",u_BackDis);
	pwndPickOut_SLVL->SetPropValueT("max",u_BackDis);
	if(pwndPickOut_Dis!=NULL)
		{
			pwndPickOut_Dis->SetPropValueT("value",u_BackDis);
			pwndPickOut_Dis->CreateA();
			pwndPickOut_Dis->Show();
		}


	// 顯示 設定數值
	ShowData(pwndPick_SLVL);
	ShowData(pwndPickOut_SLVL);
	
	u_Group = GetDBValue(P_2_SLVL_Set_DB).lValue; // 取得從哪點 跳至 柔性設置頁
	
	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = NULL;	
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	
}

void    OnDestroyA(CtmWnd* pwndSender)
{
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
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
 	
 	if(pwnd == pwndBtnSetDone) // 設定完成 Btn按下彈起
	{	
		SaveData();
		::PutCommand(QTeach_P_Page[u_PickerType][u_Group]);
	}
	

}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : SaveData()                  				   									       |
|       		: 獲取當前 設定數值	存入DB																			 |
+---------------------------------------------------------------------------*/
void	SaveData()
{
	long value = 0;
	// 取件柔性
	pwndPick_SLVL->GetPropValueT("value", &value,sizeof(value));
	SetDBValue(Pick_SLVL_DB,value); // 數值 寫入db
	printf("Set %s=%d\n",Pick_SLVL_DB,value);
	
	// 取出柔性
	pwndPickOut_SLVL->GetPropValueT("value", &value,sizeof(value));
	SetDBValue(PickOut_SLVL_DB,value); // 數值 寫入db
	printf("Set %s=%d\n",PickOut_SLVL_DB,value);
}
/*---------------------------------------------------------------------------+
|  Function : SaveData(CtmWnd* pwnd, char* DB_ID)	   									       |
|       		: pwnd 元件指標									 																 |
|       		: 獲取當前 設定數值	存入DB																			 |
+---------------------------------------------------------------------------*/
void	ShowData(CtmWnd* pwnd)
{
	char	DB_ID[256];	memset(DB_ID, 0, sizeof(DB_ID));
	pwnd->GetPropValueT("dbid1", DB_ID, sizeof(DB_ID)); // 取得資料庫ID
			
	long value = 0;
	if(pwnd != NULL)
	{
		value = GetDBValue(DB_ID).lValue; // 讀取DB數值
		pwnd->SetPropValueT("value",value); // 寫入數值
		printf("Show %s=%d\n",DB_ID,value);
		pwnd->CreateA();
		pwnd->Update();
	}
}