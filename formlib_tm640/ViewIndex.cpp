/*===========================================================================+
|  Class    : View Index								                                     |
|  Task     : View Index								                                     |
|----------------------------------------------------------------------------|
|  Compile  : Arm-linux-g++                                                  |
|  Link     : Arm-linux-g++                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 04/04/2007                                                     |
|  Revision : V2.00                                                          |
+===========================================================================*/
#include	"ViewIndex.h"
#include	"../font.h"
#include	"../utils.h"
#include	"../tmshmsg.h"
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		TransType_D			0	// 節圓直徑
#define		TransType_Tooth	1 // 齒數模數
#define		EncWord 				0xFFFF0000 // High Word
#define 	MechWord 				0x0000FFFF // Low Word

#define   MANAGE					5 // 管理
#define   MACHINE					7 // 機械
#define   ENGINEER				10 // 工程
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
int u_TransType  = 0; // 傳動方式  0-節圓直徑 1-齒數模數
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸
char* pTransTypeDB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE"; // 傳動方式DB
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("g_iPrivilege=%d\n",g_iPrivilege); // 權限等級
	//u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	// 機型選擇 編碼器選擇
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	
	u_TransType  = (int)(GetDBValue(pTransTypeDB).lValue); // 讀取設定 傳動方式 齒數模數
	
	g_Pile_Select = 0; // 關閉 快速教導中 堆疊參數設定
	return TRUE;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{              
	return wIDControl;
}

WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
		case 1: // 機構參數
			//MsgBoxCall("msgboxUserLogin.txt");
			if(g_iPrivilege>=MACHINE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
				{
					if( (u_PickerType==MechType3) && (u_TransType==TransType_D) )			 		 // 三軸 節圓直徑
					{
						strcpy(g_szCurrentFormName, "Mech_Axis3.txt");
						::PutCommand("Mech_Axis3.txt");
					}
					else if( (u_PickerType==MechType3) && (u_TransType==TransType_Tooth) ) // 三軸 齒數模數
					{
						strcpy(g_szCurrentFormName, "Mech_Axis3_TN.txt");
						::PutCommand("Mech_Axis3_TN.txt");
					}
					else if( (u_PickerType==MechType5) && (u_TransType==TransType_D) ) 		 // 五軸 節圓直徑
					{
						strcpy(g_szCurrentFormName, "Mech_Axis.txt");
						::PutCommand("Mech_Axis.txt");
					}
					else if( (u_PickerType==MechType5) && (u_TransType==TransType_Tooth) ) // 五軸 齒數模數
					{
						strcpy(g_szCurrentFormName, "Mech_Axis_TN.txt");
						::PutCommand("Mech_Axis_TN.txt");
					}
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;	
		case 2: // 速度參數
			if(g_iPrivilege>=MACHINE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
				if(u_PickerType == MechType5) // 五軸
				{
					strcpy(g_szCurrentFormName, "SpeedSet.txt");
					::PutCommand("SpeedSet.txt");
				}
				else if(u_PickerType == MechType3) // 三軸
				{
					strcpy(g_szCurrentFormName, "SpeedSet_Axis3.txt");
					::PutCommand("SpeedSet_Axis3.txt");
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 3: // 歸零
			if(g_iPrivilege>=MACHINE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
				if(u_PickerType == MechType5) // 五軸
				{
					strcpy(g_szCurrentFormName, "Zero.txt");
					::PutCommand("Zero.txt");
				}
				else if(u_PickerType == MechType3) // 三軸
				{
					strcpy(g_szCurrentFormName, "Zero_Axis3.txt");
					::PutCommand("Zero_Axis3.txt");
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;	
		case 4: // 區域設定
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
				if(u_PickerType == MechType5) // 五軸
				{
					strcpy(g_szCurrentFormName, "ZoneLimit.txt");
					::PutCommand("ZoneLimit.txt");
				}
				else if(u_PickerType == MechType3) // 三軸
				{
					strcpy(g_szCurrentFormName, "ZoneLimit_Axis3.txt");
					::PutCommand("ZoneLimit_Axis3.txt");
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 5: // 安全限制
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
			strcpy(g_szLastFormName, "Index.txt");
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
			if(u_PickerType == MechType5) // 五軸
			{
				strcpy(g_szCurrentFormName, "LimitSet.txt");
				::PutCommand("LimitSet.txt");
			}
			else if(u_PickerType == MechType3) // 三軸
			{
				strcpy(g_szCurrentFormName, "LimitSet_Axis3.txt");
				::PutCommand("LimitSet_Axis3.txt");
			}				
			break;	
		case 6: // 禁區設定
			//::PutCommand(".txt");
			break;
		case 7: // 檔案模組
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "MldS.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("MldS.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 8: // 手動操作
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Hand_1.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Hand_1.txt");
			break;
		case 11: // 快速教導
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "QTeach_Mode.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("QTeach_Mode.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 12: // 進階教導
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "PG_0.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("PG_0.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 13: // 堆疊參數
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Pile_1.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Pile_1.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 14: // 副程式
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "SUB_PG_0_1.txt");
				::PutCommand("SUB_PG_0_1.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 15: // 模式選擇
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Funcopt.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Funcopt.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 16: // 檢測設定
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Param.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Param.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 17: // 延時修正 // 位置修正
			if(g_iPrivilege>=MANAGE)
			{			
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Delay_ADJ.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Delay_ADJ.txt"); //POS_ADJ
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 21: // IO
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
			strcpy(g_szCurrentFormName, "Internal_IO.txt");
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
			strcpy(g_szLastFormName, "Index.txt");
			::PutCommand("Internal_IO.txt");
			break;	
		case 22: // 警報歷史
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
			strcpy(g_szCurrentFormName, "Alarm.txt");
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
			strcpy(g_szLastFormName, "Index.txt");
			::PutCommand("Alarm.txt");
			break;
		case 23: // 操作履歷
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "UserLog.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("UserLog.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;	
		case 24: // 驅動資訊		
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
				if(u_PickerType == MechType5) // 五軸
				{
					strcpy(g_szCurrentFormName, "DriveInfo.txt");
					::PutCommand("DriveInfo.txt");
				}
				else if(u_PickerType == MechType3) // 三軸
				{
					strcpy(g_szCurrentFormName, "DriveInfo_Axis3.txt");
					::PutCommand("DriveInfo_Axis3.txt");
				}		
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 31:// 生產管理
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Prod.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Prod.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 32:// 系統設置
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Syst.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Syst.txt");
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;	
		case 33:// 系統重置
//			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
//			strcpy(g_szCurrentFormName, "Reset.txt");
//			memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
//			strcpy(g_szLastFormName, "Index.txt");
//			::PutCommand("Reset.txt");
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "VersionManage.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("VersionManage.txt");	
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");			
			break;
		case 34: //版本資訊
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // 紀錄上一頁
			strcpy(g_szLastFormName, "Index.txt");
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // 前往下一頁
			if(u_PickerType == MechType5) // 五軸
			{
				strcpy(g_szCurrentFormName, "Vers_IML.txt");
				::PutCommand("Vers_IML.txt");
			}
			else if(u_PickerType == MechType3) // 三軸
			{
				strcpy(g_szCurrentFormName, "Vers_IML_Axis3.txt");
				::PutCommand("Vers_IML_Axis3.txt");
			}		
			break;
		default:
			break;
	}
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
		{
			printf("Toolbar =%d\n",wParam);
//			switch(wParam)
//			{
//				case _F1: // 主頁
//					::PutCommand("Over.txt");
//					break;	
//				case _F2: // 主選單
//					::PutCommand("Index.txt");
//					break;
//				case _F3: // IO
//					::PutCommand("Internal_IO.txt");
//					break;	
//				case _F4: // 鍵盤
//					::PutCommand("Hand_1.txt");
//					break;
//				default:
//					break;
//			}
		}
}

void	OnUpdateA(CtmWnd* pwndSender)
{
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

