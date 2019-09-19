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
#include 	"ViewMech.h"
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
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL 				b_Dir[5] ={FALSE};
BOOL 				b_Double[5] ={FALSE};

CtmWnd*	u_pwndEdit_ENC[5] = {NULL}; // 編碼器解析度
CtmWnd*	u_pwndEdit_MotorD[5] = {NULL}; // 節圓直徑
CtmWnd*	u_pwndEdit_MotorR[5] = {NULL}; // 減速比
CtmWnd*	u_pwndEdit_DOUBLE[5] = {NULL}; // 倍數機構
CtmWnd*	u_pwndDOUBLE_POS_INV[5] = {NULL}; // 位置反向

CtmWnd*			pwndBtn_MechType3 = NULL; // 機型選擇 3軸 Btn
CtmWnd*			pwndBtn_MechType5 = NULL; // 機型選擇 5軸 Btn
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
CtmWnd*			pwndBtn_Gear_D 		= NULL; // 節圓直徑 Btn
CtmWnd*			pwndBtn_Tooth_M 	= NULL; // 齒數模數 Btn
int u_TransType = 0; // 傳動方式  0-節圓直徑 1-齒數模數

char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸
char* pTransTypeDB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE"; // 傳動方式DB

long MechPara = 0; // 傳動方式 數值

char*	u_pszEditENCName[] = // 編碼器解析度 Btn 名稱
{
	"edit_ENC_REZ_X",
	"edit_ENC_REZ_Y",
	"edit_ENC_REZ_Z",
	"edit_ENC_REZ_X2",
	"edit_ENC_REZ_Y2"
};
char*	u_pszEditMotorDName[] = // 節圓直徑 輸入元件 名稱
{
	"edit_Motor_Diameter_X",
	"edit_Motor_Diameter_Y",
	"edit_Motor_Diameter_Z",
	"edit_Motor_Diameter_X2",
	"edit_Motor_Diameter_Y2"
};
char*	u_pszEditMotorRName[] = // 減速比 輸入元件 名稱
{
	"edit_Motor_Ratio_X",
	"edit_Motor_Ratio_Y",
	"edit_Motor_Ratio_Z",
	"edit_Motor_Ratio_X2",
	"edit_Motor_Ratio_Y2"
};

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


	for(int i = 0; i < 5; i++) // 取得指標 
	{
		u_pwndEdit_ENC[i] = pwndSender->FindControlFromName(u_pszEditENCName[i]);
		u_pwndEdit_MotorD[i] = pwndSender->FindControlFromName(u_pszEditMotorDName[i]);
		u_pwndEdit_MotorR[i] = pwndSender->FindControlFromName(u_pszEditMotorRName[i]);
		
		u_pwndEdit_DOUBLE[i] = pwndSender->FindControlFromName(u_pszEditDoubleName[i]);
		u_pwndDOUBLE_POS_INV[i] = pwndSender->FindControlFromName(u_pszEditPOSINVName[i]);
	}
	 
	// 讀取參數數值
	/*
	long long	lTemp = 0;
	for(int i = 0; i < 5; i++)
	{	
		if(u_pwndEdit_ENC[i] != NULL)
			{
				u_pwndEdit_ENC[i]->GetPropValueT("value", &lTemp,sizeof(lTemp));
				printf("%s=%d\n",u_pszEditENCName[i],lTemp);
			}
		if(u_pwndEdit_MotorD[i] != NULL)
			{
				u_pwndEdit_MotorD[i]->GetPropValueT("value", &lTemp,sizeof(lTemp));
				printf("%s=%d\n",u_pszEditMotorDName[i],lTemp);
			}
		if(u_pwndEdit_MotorR[i] != NULL)
			{
				u_pwndEdit_MotorR[i]->GetPropValueT("value", &lTemp,sizeof(lTemp));
				printf("%s=%d\n",u_pszEditMotorRName[i],lTemp);
			}

	}
	*/
	
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
				//u_pwndEdit_DOUBLE[i]->SetPropValueT("upbitmap",Img_Btn[u_Double]);
				//u_pwndEdit_DOUBLE[i]->SetPropValueT("downbitmap",Img_Btn[u_Double]);
				u_pwndEdit_DOUBLE[i]->SetPropValueT("bgc",Color_Btn[u_Double]);		
				//printf("Set IMG %s\n",Color_Btn[u_Double]);
				u_pwndEdit_DOUBLE[i]->CreateA();
				u_pwndEdit_DOUBLE[i]->Update();
				if(u_Double == 1)
					((CtmButton*)u_pwndEdit_DOUBLE[i])->Press(tmBUTTON_DOWN);
			}
		if(u_pwndDOUBLE_POS_INV[i] != NULL) // 位置反向
			{
				int u_POSINV = (int)(GetDBValue(u_pszEditPOSINVDB[i]).lValue);
				//printf("%s=%d\n",u_pszEditPOSINVDB[i],u_POSINV);
				b_Dir[i]=TF[u_POSINV];
				u_pwndDOUBLE_POS_INV[i]->SetPropValueT("captionID",Str_POSINV[u_POSINV]);
				//u_pwndDOUBLE_POS_INV[i]->SetPropValueT("upbitmap",Img_Btn[u_POSINV]);
				//u_pwndDOUBLE_POS_INV[i]->SetPropValueT("downbitmap",Img_Btn[u_POSINV]);
				u_pwndDOUBLE_POS_INV[i]->SetPropValueT("bgc",Color_Btn[u_POSINV]);	
				//printf("Set IMG %s\n",Img_Btn[u_POSINV]);
				u_pwndDOUBLE_POS_INV[i]->CreateA();
				u_pwndDOUBLE_POS_INV[i]->Update();	
				if(u_POSINV == 1)
					((CtmButton*)u_pwndDOUBLE_POS_INV[i])->Press(tmBUTTON_DOWN);
			}
	}
	
	// 機型選擇
	u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue);
	UpdateMechType();
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
	
//	int nTemp = 0;
//	for(int i = 0; i < 5; i++)
//	{
//		if(u_pwndEdit_DOUBLE[i] != NULL)
//			{
//				nTemp = ((CtmSelectEdit *)u_pwndEdit_DOUBLE[i])->GetIndex();
//				//printf("%s=%d\n",u_pszEditDoubleName[i],nTemp);
//				SetDBValue(u_pszEditDoubleDB[i],nTemp);
//			}
//		if(u_pwndDOUBLE_POS_INV[i] != NULL)
//			{
//				nTemp = ((CtmSelectEdit *)u_pwndDOUBLE_POS_INV[i])->GetIndex();
//				SetDBValue(u_pszEditPOSINVDB[i],nTemp);
//			}
//		if(pwndSender == u_pwndEdit_DOUBLE[i])
//			{
//				printf("touch %s\n",u_pszEditDoubleName[i]);
//			}
//		printf("%d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue); 
//		printf("%d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue); 
//		
//		//printf(" <<1 = %d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue <<1); 
//		//printf(" <<1 <<1= %d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue <<2); 
//
//		MechPara = 0 + ((int)GetDBValue(u_pszEditDoubleDB[i]).lValue<<1) + ((int)GetDBValue(u_pszEditPOSINVDB[i]).lValue<<2);
//		//printf("MechPara d=%d\n",MechPara);
//		SetDBValue(u_pszMechPara[i],MechPara);
//	}
	
	switch(wKey)
	{
//		case 33:
//    		::PutCommand("Mech2.txt");
//			break;
//		case 34:
//    		::PutCommand("Mech.txt");
//			break;
		case 46:
			SendCommand(0xFF32); 
			break;
		case 7178:
			SendCommand(0xFF32); 
			break;
		case 70: // Home
			::PutCommand("Over.txt");
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

	for(int i = 0; i < 5; i++) // 偵測按下放開 Btn
	{
		if((pwnd == u_pwndDOUBLE_POS_INV[i]) || (pwnd == u_pwndEdit_DOUBLE[i]) 
			|| (pwnd == pwndBtn_Gear_D) || (pwnd == pwndBtn_Tooth_M) )
		{
			if(pwnd == u_pwndDOUBLE_POS_INV[i]) // 位置反向 Btn
				{
					int nTemp =0;
					b_Dir[i] = !b_Dir[i];
					//printf("%s=%d\n",u_pszEditPOSINVName[i],b_Dir[i]);
					nTemp = b_Dir[i];
					SetDBValue(u_pszEditPOSINVDB[i],nTemp);
					int u_POSINV = (int)(GetDBValue(u_pszEditPOSINVDB[i]).lValue);
					//printf("%d\n", u_POSINV); 
					//printf("%s\n", Str_POSINV[u_POSINV]);
					
					//Update string
					u_pwndDOUBLE_POS_INV[i]->SetPropValueT("captionID",Str_POSINV[u_POSINV]);
					//u_pwndDOUBLE_POS_INV[i]->SetPropValueT("upbitmap",Img_Btn[u_POSINV]);
					//u_pwndDOUBLE_POS_INV[i]->SetPropValueT("downbitmap",Img_Btn[u_POSINV]);
					u_pwndDOUBLE_POS_INV[i]->SetPropValueT("bgc",Color_Btn[u_POSINV]);
					u_pwndDOUBLE_POS_INV[i]->CreateA();
					u_pwndDOUBLE_POS_INV[i]->Update();
					if(u_POSINV == 1)
						((CtmButton*)u_pwndDOUBLE_POS_INV[i])->Press(tmBUTTON_DOWN);
				}
			if(pwnd == u_pwndEdit_DOUBLE[i]) // 倍數機構 Btn
				{
					int nTemp =0;
					b_Double[i] = !b_Double[i];
					//printf("%s=%d\n",u_pszEditDoubleName[i],b_Double[i]);
					nTemp = b_Double[i];
					SetDBValue(u_pszEditDoubleDB[i],nTemp);

					int u_Double = (int)(GetDBValue(u_pszEditDoubleDB[i]).lValue);
					//printf("%d\n", u_Double); 
					//printf("%s\n", Str_Double[u_Double]);
					
					//Update string
					u_pwndEdit_DOUBLE[i]->SetPropValueT("captionID",Str_Double[u_Double]);
					//u_pwndEdit_DOUBLE[i]->SetPropValueT("upbitmap",Img_Btn[u_Double]);
					//u_pwndEdit_DOUBLE[i]->SetPropValueT("downbitmap",Img_Btn[u_Double]);
					u_pwndEdit_DOUBLE[i]->SetPropValueT("bgc",Color_Btn[u_Double]);
					u_pwndEdit_DOUBLE[i]->CreateA();
					u_pwndEdit_DOUBLE[i]->Update();
					if(u_Double == 1)
						((CtmButton*)u_pwndEdit_DOUBLE[i])->Press(tmBUTTON_DOWN);
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
			//printf("%d", (int)GetDBValue(pTransTypeDB).lValue); 					// bit 0 傳動方式
			//printf("%d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue); 	// bit 1 倍數機構
			//printf("%d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue); // bit 2 位置反向
			
			//printf(" <<1 = %d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue <<1); 
			//printf(" <<1 <<1= %d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue <<2); 

			MechPara = (int)GetDBValue(pTransTypeDB).lValue + ((int)GetDBValue(u_pszEditDoubleDB[i]).lValue<<1) + ((int)GetDBValue(u_pszEditPOSINVDB[i]).lValue<<2);
			//printf("MechPara = %x\n",MechPara);
			SetDBValue(u_pszMechPara[i],MechPara);
			SendCommand(0xFF32);
			
		}
	}

	if(pwnd == pwndBtn_MechType3) // 機型選擇 3軸 Btn
	{
		u_PickerType = MechType3;
		SetDBValue(pMechTypeDB,u_PickerType);
		UpdateMechType();
	}
	if(pwnd == pwndBtn_MechType5) // 機型選擇 5軸 Btn
	{
		u_PickerType = MechType5;
		SetDBValue(pMechTypeDB,u_PickerType);
		UpdateMechType();
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
			::PutCommand("Mech_Tooth_Axis3.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_D) ) 		 // 五軸 節圓直徑
		{
			::PutCommand("Mech.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_Tooth) ) // 五軸 齒數模數
		{
			::PutCommand("Mech_Tooth.txt");
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
//	for(int i = 0; i < 5; i++)
//	{
//		printf("%d", 0); 
//		printf("%d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue); 
//		printf("%d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue); 
//		MechPara = 0 + ((int)GetDBValue(u_pszEditDoubleDB[i]).lValue<<1) + ((int)GetDBValue(u_pszEditPOSINVDB[i]).lValue<<2);
//		SetDBValue(u_pszMechPara[i],MechPara); 
//		SendCommand(0xFF32); 
//	}

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
	;
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