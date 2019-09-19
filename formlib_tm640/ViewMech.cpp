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
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b
#define		TransType_D			0	// �`�ꪽ�|
#define		TransType_Tooth	1 // ���ƼҼ�
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL 				b_Dir[5] ={FALSE};
BOOL 				b_Double[5] ={FALSE};

CtmWnd*	u_pwndEdit_ENC[5] = {NULL}; // �s�X���ѪR��
CtmWnd*	u_pwndEdit_MotorD[5] = {NULL}; // �`�ꪽ�|
CtmWnd*	u_pwndEdit_MotorR[5] = {NULL}; // ��t��
CtmWnd*	u_pwndEdit_DOUBLE[5] = {NULL}; // ���ƾ��c
CtmWnd*	u_pwndDOUBLE_POS_INV[5] = {NULL}; // ��m�ϦV

CtmWnd*			pwndBtn_MechType3 = NULL; // ������� 3�b Btn
CtmWnd*			pwndBtn_MechType5 = NULL; // ������� 5�b Btn
int u_PickerType = 0; // ������� 0-�T�b 1-���b
CtmWnd*			pwndBtn_Gear_D 		= NULL; // �`�ꪽ�| Btn
CtmWnd*			pwndBtn_Tooth_M 	= NULL; // ���ƼҼ� Btn
int u_TransType = 0; // �ǰʤ覡  0-�`�ꪽ�| 1-���ƼҼ�

char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b
char* pTransTypeDB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE"; // �ǰʤ覡DB

long MechPara = 0; // �ǰʤ覡 �ƭ�

char*	u_pszEditENCName[] = // �s�X���ѪR�� Btn �W��
{
	"edit_ENC_REZ_X",
	"edit_ENC_REZ_Y",
	"edit_ENC_REZ_Z",
	"edit_ENC_REZ_X2",
	"edit_ENC_REZ_Y2"
};
char*	u_pszEditMotorDName[] = // �`�ꪽ�| ��J���� �W��
{
	"edit_Motor_Diameter_X",
	"edit_Motor_Diameter_Y",
	"edit_Motor_Diameter_Z",
	"edit_Motor_Diameter_X2",
	"edit_Motor_Diameter_Y2"
};
char*	u_pszEditMotorRName[] = // ��t�� ��J���� �W��
{
	"edit_Motor_Ratio_X",
	"edit_Motor_Ratio_Y",
	"edit_Motor_Ratio_Z",
	"edit_Motor_Ratio_X2",
	"edit_Motor_Ratio_Y2"
};

char*	u_pszEditDoubleName[] = // ���ƾ��c ��J���� �W��
{
	"Btn_DOUBLE_X",
	"Btn_DOUBLE_Y",
	"Btn_DOUBLE_Z",
	"Btn_DOUBLE_X2",
	"Btn_DOUBLE_Y2"
}; 

char*	u_pszEditPOSINVName[] = // ��m�ϦV ��J���� �W��
{
	"Btn_POS_INV_X",
	"Btn_POS_INV_Y",
	"Btn_POS_INV_Z",
	"Btn_POS_INV_X2",
	"Btn_POS_INV_Y2"
};

char*	Str_Double[] = // ���ƾ��c�ϥλP�_ ��r
{
	"MECH_UNUSE", // �ϥ�
	"MECH_USE", // ����
};
char*	Str_POSINV[] = // ��m�ϦV ��r
{
	"VW_ROTATELOC_ANGELREVERSED_OPT1", // ���V
	"VW_ROTATELOC_ANGELREVERSED_OPT2", // �ϦV
};
char*	Img_Btn[] =
{
	"res_tm640/pic/ButtonUp.bmp",
	"res_tm640/pic/ButtonDown.bmp",
};
char*	Str_PICKER_Type[] = // ���c��� ��r
{
	"TYPE_3AXIS", // 3�b
	"TYPE_5AXIS", // 5�b
};
long	Color_Btn[] = // ������ �C��
{
	0xD6BA,
	0xEF32,//0x8DA0,
};

BOOL TF[] =
{
	FALSE,
	TRUE
};

char*	u_pszEditDoubleDB[] = // �������ƾ��c�O�_�ϥ�DB�W��
{
	"PICKER_PARAMETER_DOUBLE_X1",
	"PICKER_PARAMETER_DOUBLE_Y1",
	"PICKER_PARAMETER_DOUBLE_Z",
	"PICKER_PARAMETER_DOUBLE_X2",
	"PICKER_PARAMETER_DOUBLE_Y2"
}; 

char*	u_pszEditPOSINVDB[] = // ������m�ϦV�O�_�ϥ�DB�W��
{
	"PICKER_PARAMETER_POS_INV_X1",
	"PICKER_PARAMETER_POS_INV_Y1",
	"PICKER_PARAMETER_POS_INV_Z",
	"PICKER_PARAMETER_POS_INV_X2",
	"PICKER_PARAMETER_POS_INV_Y2"
}; 

/*---------------------------------------------------------------------------+
| u_pszMechPara �ǰʤ覡  																									 |
|							   _____________________________________        							 |	
|   						|	Bit |				0				|				1				| 										 |		
|								|	 0	|	�ϥθ`�ꪽ�|	|	�ϥξ��ƼҼ�  |       							 |
|								|	 1	|			���ϥ� 		|	�ϥέ��ƾ��c  |       							 |
|								|__2__|_____���ϥ�____|_�ϥΦ�m�ϦV__|       							 |
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
	
	// ���o���� ���c��ܡG3�b 5�b
	pwndBtn_MechType3     = pwndSender->FindControlFromName("Btn_MechType3");
	pwndBtn_MechType5     = pwndSender->FindControlFromName("Btn_MechType5");
	// ���o���� �ǰʤ覡�G�`�ꪽ�| ���ƼҼ�
	pwndBtn_Gear_D     		= pwndSender->FindControlFromName("Btn_Gear_D");
	pwndBtn_Tooth_M    		= pwndSender->FindControlFromName("Btn_Tooth_M");


	for(int i = 0; i < 5; i++) // ���o���� 
	{
		u_pwndEdit_ENC[i] = pwndSender->FindControlFromName(u_pszEditENCName[i]);
		u_pwndEdit_MotorD[i] = pwndSender->FindControlFromName(u_pszEditMotorDName[i]);
		u_pwndEdit_MotorR[i] = pwndSender->FindControlFromName(u_pszEditMotorRName[i]);
		
		u_pwndEdit_DOUBLE[i] = pwndSender->FindControlFromName(u_pszEditDoubleName[i]);
		u_pwndDOUBLE_POS_INV[i] = pwndSender->FindControlFromName(u_pszEditPOSINVName[i]);
	}
	 
	// Ū���ѼƼƭ�
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
		if(u_pwndEdit_DOUBLE[i] != NULL) // ���ƾ��c
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
		if(u_pwndDOUBLE_POS_INV[i] != NULL) // ��m�ϦV
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
	
	// �������
	u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue);
	UpdateMechType();
	// �ǰʤ覡
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
				case _F1: // �D��
						::PutCommand("Over.txt");
					break;	
				case _F2: // �D���
					::PutCommand("Index.txt");
					break;
				case _F5: // ��^
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

	for(int i = 0; i < 5; i++) // �������U��} Btn
	{
		if((pwnd == u_pwndDOUBLE_POS_INV[i]) || (pwnd == u_pwndEdit_DOUBLE[i]) 
			|| (pwnd == pwndBtn_Gear_D) || (pwnd == pwndBtn_Tooth_M) )
		{
			if(pwnd == u_pwndDOUBLE_POS_INV[i]) // ��m�ϦV Btn
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
			if(pwnd == u_pwndEdit_DOUBLE[i]) // ���ƾ��c Btn
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
			if(pwnd == pwndBtn_Gear_D) // �ǰʤ覡 �`�ꪽ�| Btn
				{
					u_TransType = TransType_D;
					SetDBValue(pTransTypeDB,TransType_D);
				}
			if(pwnd == pwndBtn_Tooth_M) // �ǰʤ覡 ���ƼҼ� Btn
				{
					u_TransType = TransType_Tooth;
					SetDBValue(pTransTypeDB,TransType_Tooth);
				}
			//printf("%d", (int)GetDBValue(pTransTypeDB).lValue); 					// bit 0 �ǰʤ覡
			//printf("%d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue); 	// bit 1 ���ƾ��c
			//printf("%d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue); // bit 2 ��m�ϦV
			
			//printf(" <<1 = %d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue <<1); 
			//printf(" <<1 <<1= %d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue <<2); 

			MechPara = (int)GetDBValue(pTransTypeDB).lValue + ((int)GetDBValue(u_pszEditDoubleDB[i]).lValue<<1) + ((int)GetDBValue(u_pszEditPOSINVDB[i]).lValue<<2);
			//printf("MechPara = %x\n",MechPara);
			SetDBValue(u_pszMechPara[i],MechPara);
			SendCommand(0xFF32);
			
		}
	}

	if(pwnd == pwndBtn_MechType3) // ������� 3�b Btn
	{
		u_PickerType = MechType3;
		SetDBValue(pMechTypeDB,u_PickerType);
		UpdateMechType();
	}
	if(pwnd == pwndBtn_MechType5) // ������� 5�b Btn
	{
		u_PickerType = MechType5;
		SetDBValue(pMechTypeDB,u_PickerType);
		UpdateMechType();
	}

	if( (pwnd == pwndBtn_Gear_D) || (pwnd == pwndBtn_Tooth_M) 
		|| (pwnd == pwndBtn_MechType3) || (pwnd == pwndBtn_MechType5) ) // �ǰʤ覡 || ������� Btn �Q���U
	{
		if( (u_PickerType==MechType3) && (u_TransType==TransType_D) )			 		 // �T�b �`�ꪽ�|
		{
			::PutCommand("Mech_Axis3.txt");
		}
		else if( (u_PickerType==MechType3) && (u_TransType==TransType_Tooth) ) // �T�b ���ƼҼ�
		{
			::PutCommand("Mech_Tooth_Axis3.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_D) ) 		 // ���b �`�ꪽ�|
		{
			::PutCommand("Mech.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_Tooth) ) // ���b ���ƼҼ�
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
|       		: ��s	����������																						 |
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
|       		: ��s	�ǰʤ覡���																						 |
+---------------------------------------------------------------------------*/
void	UpdateTransType()
{
	if(pwndBtn_Gear_D != NULL)
			{			
				if(u_TransType==TransType_D) // �`�ꪽ�|
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
				if(u_TransType==TransType_Tooth) // ���ƼҼ�
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