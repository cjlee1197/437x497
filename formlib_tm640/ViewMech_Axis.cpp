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
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b
#define		TransType_D			0	// �`�ꪽ�|
#define		TransType_Tooth	1 // ���ƼҼ�
#define		EncType_Abs			0x00000000  // ����
#define		EncType_Res		  0x00010000  // �W�q
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//�Q��update�Ȱ���@��

int u_Axis_Num = 0;
BOOL 				b_Dir[5] ={FALSE};
BOOL 				b_Double[5] ={FALSE};

CtmWnd*	u_pwndEdit_DOUBLE[5] = {NULL}; // ���ƾ��c
CtmWnd*	u_pwndDOUBLE_POS_INV[5] = {NULL}; // ��m�ϦV

/*--------------------------------------------------+
| dw_MechType �������  														|
|	Low WORD 0-�T�b 1-���b 	High WORD 0-���� 1-�W�q		|
| EX: ����/���b 0x0000 0001;  0x0001 0000 �W�q/�T�b |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //���� pMechTypeDB ���ƭ�
CtmWnd*			pwndBtn_MechType3 = NULL; // ������� 3�b Btn
CtmWnd*			pwndBtn_MechType5 = NULL; // ������� 5�b Btn
int u_PickerType = 0; // ������� 0-�T�b 1-���b
CtmWnd*			pwndBtn_Gear_D 		= NULL; // �`�ꪽ�| Btn
CtmWnd*			pwndBtn_Tooth_M 	= NULL; // ���ƼҼ� Btn
int u_TransType = 0; // �ǰʤ覡  0-�`�ꪽ�| 1-���ƼҼ�
CtmWnd*			pwndBtn_EncType_Abs 		= NULL; // �s�X����� ���� Btn
CtmWnd*			pwndBtn_EncType_Res 		= NULL; // �s�X����� �W�q Btn
int u_EncType = 0; // �s�X�����  0-���� 1-�W�q

CtmWnd*		pwndBtn_Axis[6] 	={NULL}; // �U�bBtn
int iBtn_Axis = 0;

CtmWnd*		pwndMask_MechData	= NULL; // ���c�ѼƾB�n

CtmWnd*		pwndMech_Data[6] 	={NULL}; // �U�b �Ѽ�
char* Mech_Data_String[] = // �Ѽ� ���� �W��
{
	"edit_ENC_REZ", 				// �s�X���ѪR��
	"edit_Motor_Ratio", 	  // ��t��
	"edit_Motor_Diameter",	// �`�ꪽ�|
	"edit_Tooth_Num",				// ����
	"edit_M_Num",						// �Ҽ�
};

CtmWnd*		pwndDOUBLE 	={NULL}; // �U�b �Ѽ� ���� 
CtmWnd*		pwndPOS_INV ={NULL}; // �U�b �Ѽ� ��m�ϦV 


char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b
char* pTransTypeDB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE"; // �ǰʤ覡DB

long MechPara = 0; // �ǰʤ覡 �ƭ�

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

char* dbid0_Mech[6][5] = // �ѼƼƭ� db
{	//  �s�X���ѪR��									,��t��														,�`�ꪽ�|													,����														 ,�Ҽ�
	{NULL,NULL,NULL,NULL,NULL},	// �@�P
	{"MACHINE_INTERFACE_CONTROL_RSV05","MACHINE_INTERFACE_CONTROL_RSV10","MACHINE_FUNCTION_OPTIONS_RSV22","MACHINE_FUNCTION_OPTIONS_RSV12","MACHINE_FUNCTION_OPTIONS_RSV17"},		// X1�b
	{"MACHINE_INTERFACE_CONTROL_RSV06","MACHINE_INTERFACE_CONTROL_RSV11","MACHINE_FUNCTION_OPTIONS_RSV23","MACHINE_FUNCTION_OPTIONS_RSV13","MACHINE_FUNCTION_OPTIONS_RSV18"},		// Y1�b
	{"MACHINE_INTERFACE_CONTROL_RSV07","MACHINE_INTERFACE_CONTROL_RSV12","MACHINE_FUNCTION_OPTIONS_RSV24","MACHINE_FUNCTION_OPTIONS_RSV14","MACHINE_FUNCTION_OPTIONS_RSV19"},		// Z�b
	{"MACHINE_INTERFACE_CONTROL_RSV08","MACHINE_INTERFACE_CONTROL_RSV13","MACHINE_FUNCTION_OPTIONS_RSV25","MACHINE_FUNCTION_OPTIONS_RSV15","MACHINE_FUNCTION_OPTIONS_RSV20"},		// X2�b
	{"MACHINE_INTERFACE_CONTROL_RSV09","MACHINE_INTERFACE_CONTROL_RSV14","MACHINE_FUNCTION_OPTIONS_RSV26","MACHINE_FUNCTION_OPTIONS_RSV16","MACHINE_FUNCTION_OPTIONS_RSV21"}			// Y2�b
};

CtmWnd*	pwndStrAxisData[6] 	 = {NULL}; // �U�b �Ѽ� ��r
char* 	StrAxisData_String[] = 				 // �Ѽ� ��r �W��
{
	"Str_ENC_REZ", 				// �s�X���ѪR��
	"Str_Motor_Ratio", 	  // ��t��
	"Str_Motor_Diameter",	// �C��Z��
	"Str_Tooth_N",	// �C��Z��
	"Str_DOUBLE",					// ���ƾ��c
	"Str_POS_INV"					// ��m�ϦV
};

CtmWnd*	pwndStrMechData[3] 	 = {NULL}; // ���c �@�P�Ѽ� ��r
char* 	StrMechData_String[] = 				 // �@�P�Ѽ� ��r �W��
{
	"Str_MechType", // �������
	"Str_Trans", // �ǰʤ覡
	"Str_EncType", // �s�X�����

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
	// ���o���� �s�X����ܡG���� �W�q
	pwndBtn_EncType_Abs     = pwndSender->FindControlFromName("Btn_EncType_Abs");
	pwndBtn_EncType_Res	    = pwndSender->FindControlFromName("Btn_EncType_Res");
	
	pwndMask_MechData 		  = pwndSender->FindControlFromName("Mask_MechData"); // ���c�ѼƾB�n

	for(int i = 0; i < 5; i++) // ���o���� 
	{
		u_pwndEdit_DOUBLE[i] = pwndSender->FindControlFromName(u_pszEditDoubleName[i]);
		u_pwndDOUBLE_POS_INV[i] = pwndSender->FindControlFromName(u_pszEditPOSINVName[i]);
	}
	 
	iBtn_Axis	 	= GetSpecialControlNum(pwndSender, "Btn_Axis_", "CtmToolButton", pwndBtn_Axis);
	printf("iBtn_Axis=%d\n",iBtn_Axis);
	// ���o���� ���c�Ѽ� ��J����
	for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
	{
		pwndMech_Data[i] = pwndSender->FindControlFromName(Mech_Data_String[i]);// �Ѽ� ����
		printf("pwndMech_Data[%d] = %s\n",i,Mech_Data_String[i]);
	} 
	pwndDOUBLE				= pwndSender->FindControlFromName("Btn_DOUBLE");
	pwndPOS_INV				= pwndSender->FindControlFromName("Btn_POS_INV");
	
	// ���o���� �U�b�Ѽ� ��r
	for(int i = 0; i < sizeof(StrAxisData_String)/sizeof(StrAxisData_String[0]); i++ )
	{
		pwndStrAxisData[i] = pwndSender->FindControlFromName(StrAxisData_String[i]);// �Ѽ� ����
	} 	
	// ���o���� �@�P�Ѽ� ��r
	for(int i = 0; i < sizeof(StrMechData_String)/sizeof(StrMechData_String[0]); i++ )
	{
		pwndStrMechData[i] = pwndSender->FindControlFromName(StrMechData_String[i]);// �Ѽ� ����
	} 		
	
	// Update Data def
	UpdateData();
	UpdateBtnData();
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
				u_pwndEdit_DOUBLE[i]->SetPropValueT("bgc",Color_Btn[u_Double]);		
				u_pwndEdit_DOUBLE[i]->CreateA();
				u_pwndEdit_DOUBLE[i]->Update();
				if(u_Double == 1)
					((CtmButton*)u_pwndEdit_DOUBLE[i])->Press(tmBUTTON_DOWN);
			}
		if(u_pwndDOUBLE_POS_INV[i] != NULL) // ��m�ϦV
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
	
	// ������� �s�X�����
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	u_EncType		 = dw_MechType & EncWord;
	UpdateMechType();
	UpdateEncType();
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
	((CtmFormView*)pwndSender)->OnLoseFocus(); // ��������
	
	for(int i=0;i<iBtn_Axis;i++) // �b�����ܦ�
	{
		pwndBtn_Axis[i]->SetPropValueT("upbitmap","res_tm640/pic/ButtonUp1.bmp");
		pwndBtn_Axis[i]->CreateA();
		pwndBtn_Axis[i]->Update();
	}
	
	switch(wKey)
	{
		case 1:
    	u_Axis_Num=1;
    	ShowMechData(FALSE); // �B�\ ���c�Ѽ�
    	ShowAxisData(TRUE);  // ��� �U�b�Ѽ�
			break;
		case 2:
    	u_Axis_Num=2;
    	ShowMechData(FALSE); // �B�\ ���c�Ѽ�
    	ShowAxisData(TRUE);  // ��� �U�b�Ѽ�
			break;	
		case 3:
			u_Axis_Num=3;
			ShowMechData(FALSE); // �B�\ ���c�Ѽ�
			ShowAxisData(TRUE);  // ��� �U�b�Ѽ�
			break;
		case 4:
			u_Axis_Num=4;
			ShowMechData(FALSE); // �B�\ ���c�Ѽ�
			ShowAxisData(TRUE);  // ��� �U�b�Ѽ�
			break;
		case 5:
			u_Axis_Num=5;
			ShowMechData(FALSE); // �B�\ ���c�Ѽ�
			ShowAxisData(TRUE);  // ��� �U�b�Ѽ�
			break;
		case 46:
			SendCommand(0xFF32); 
			break;
		case 77:
			u_Axis_Num=0;
			ShowAxisData(FALSE); // �B�\ �U�b�Ѽ�
			ShowMechData(TRUE);  // ��� ���c�Ѽ�
			break;
		case 7178:
			SendCommand(0xFF32); 
			break;
		default:
			break;
	}

	if(pwndBtn_Axis[u_Axis_Num]) // ������b����
	{	
		pwndBtn_Axis[u_Axis_Num]->SetPropValueT("upbitmap","res_tm640/pic/ButtonDown1.bmp");
		pwndBtn_Axis[u_Axis_Num]->CreateA();
		pwndBtn_Axis[u_Axis_Num]->Update();
	}
	
	if(u_Axis_Num!=0)
	{
		UpdateData(); // ��s��� �w�q
		UpdateBtnData(); // ��s��� ���ƾ��c ��m�ϦV
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


	if((pwnd == pwndDOUBLE) || (pwnd == pwndPOS_INV) || (pwnd == pwndBtn_Gear_D) || (pwnd == pwndBtn_Tooth_M) )
	{
		if(pwnd == pwndPOS_INV) // ��m�ϦV Btn
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
		if(pwnd == pwndDOUBLE) // ���ƾ��c Btn
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
			
		for(int i = 0; i < 5; i++) // �������U��} Btn
		{
			printf("%d", (int)GetDBValue(pTransTypeDB).lValue); 					// bit 0 �ǰʤ覡
			printf("%d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue); 	// bit 1 ���ƾ��c
			printf("%d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue); // bit 2 ��m�ϦV
			
			printf(" <<1 = %d", (int)GetDBValue(u_pszEditDoubleDB[i]).lValue <<1); 
			printf(" <<1 <<1= %d\n", (int)GetDBValue(u_pszEditPOSINVDB[i]).lValue <<2); 
			
			MechPara = (int)GetDBValue(pTransTypeDB).lValue + ((int)GetDBValue(u_pszEditDoubleDB[i]).lValue<<1) + ((int)GetDBValue(u_pszEditPOSINVDB[i]).lValue<<2);
			printf("MechPara = %x\n",MechPara);
			SetDBValue(u_pszMechPara[i],MechPara);
			SendCommand(0xFF32);
		}
	}

	if(pwnd == pwndBtn_MechType3) // ������� 3�b Btn
	{
		u_PickerType = MechType3;
		dw_MechType = u_PickerType+u_EncType;
		SetDBValue(pMechTypeDB,dw_MechType);
		printf("dw_MechType=%x\n",dw_MechType);
		UpdateMechType();
	}
	if(pwnd == pwndBtn_MechType5) // ������� 5�b Btn
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
		|| (pwnd == pwndBtn_MechType3) || (pwnd == pwndBtn_MechType5) ) // �ǰʤ覡 || ������� Btn �Q���U
	{
		if( (u_PickerType==MechType3) && (u_TransType==TransType_D) )			 		 // �T�b �`�ꪽ�|
		{
			::PutCommand("Mech_Axis3.txt");
		}
		else if( (u_PickerType==MechType3) && (u_TransType==TransType_Tooth) ) // �T�b ���ƼҼ�
		{
			::PutCommand("Mech_Axis3_TN.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_D) ) 		 // ���b �`�ꪽ�|
		{
			::PutCommand("Mech_Axis.txt");
		}
		else if( (u_PickerType==MechType5) && (u_TransType==TransType_Tooth) ) // ���b ���ƼҼ�
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
		((CtmFormView*)pwndSender)->OnLoseFocus(); // ��������

		ShowAxisData(FALSE); // �B�\ �U�b�Ѽ�
		ShowMechData(TRUE);  // ��� ���c�Ѽ�
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
/*---------------------------------------------------------------------------+
|  Function : UpdateEncType()                  											  		   |
|       		: ��s	�s�X��������																					 |
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
|  Task     : ��s��� �w�q	     	                                           |
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
|  Task     : ��s��� �w�q	     	                                           |
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
|  Task     : ��ܾB����	     	                                           |
+----------------------------------------------------------------------------+
|  Parameter: pwnd ����, enabled TRUE-��� FALSE-�B��						             |
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
|  Task     : ��ܾ��c�Ѽ�	     	                                           |
+----------------------------------------------------------------------------+
|  Parameter:             enabled TRUE-��� FALSE                            |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ShowMechData(BOOL enabled) // ��ܾ��c�Ѽ�
{
	printf("ShowMechData\n");
	if(pwndMask_MechData!=NULL) // �B�n
		pwndMask_MechData->Update();
		
	ShowE(pwndBtn_MechType3,enabled); 	// ��� 3�bBtn
	ShowE(pwndBtn_MechType5,enabled); 	// ��� 5�bBtn
	ShowE(pwndBtn_Gear_D,enabled); 			// ��� �C��Z��Btn
	ShowE(pwndBtn_Tooth_M,enabled); 		// ��� ���ƼҼ�Btn
	ShowE(pwndBtn_EncType_Abs,enabled); // ��� ����Btn
	ShowE(pwndBtn_EncType_Res,enabled); // ��� �W�qBtn
	
	for(int i = 0; i < sizeof(StrMechData_String)/sizeof(StrMechData_String[0]); i++ )
	{
		ShowE(pwndStrMechData[i],enabled); // ��� �@�P�Ѽ� ��r
	} 
}

/*---------------------------------------------------------------------------+
|  Function : ShowAxisData(BOOL enabled)                           		       |
|  Task     : ��ܾ��c�Ѽ�	     	                                           |
+----------------------------------------------------------------------------+
|  Parameter:             enabled TRUE-��� FALSE                            |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ShowAxisData(BOOL enabled) // ��ܦU�b�Ѽ�
{
	printf("ShowAxisData\n");
	if(pwndMask_MechData!=NULL) // �B�n
		pwndMask_MechData->Update();
	for(int i = 0; i < sizeof(Mech_Data_String)/sizeof(Mech_Data_String[0]); i++ )
	{
		ShowE(pwndMech_Data[i],enabled); 	 // ��� �U�b�Ѽ� ����
	} 

	ShowE(pwndDOUBLE,enabled); 	// ��� ���� Btn
	ShowE(pwndPOS_INV,enabled); // ��� �ϦV Btn
	
	for(int i = 0; i < sizeof(StrAxisData_String)/sizeof(StrAxisData_String[0]); i++ )
	{
		ShowE(pwndStrAxisData[i],enabled); 	 // ��� �U�b�Ѽ� ��r
	} 	
	
}
