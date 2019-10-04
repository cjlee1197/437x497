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
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b
#define		TransType_D			0	// �`�ꪽ�|
#define		TransType_Tooth	1 // ���ƼҼ�
#define		EncWord 				0xFFFF0000 // High Word
#define 	MechWord 				0x0000FFFF // Low Word

#define   MANAGE					5 // �޲z
#define   MACHINE					7 // ����
#define   ENGINEER				10 // �u�{
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
/*--------------------------------------------------+
| dw_MechType �������  														|
|	Low WORD 0-�T�b 1-���b 	High WORD 0-���� 1-�W�q		|
| EX: ����/���b 0x0000 0001;  0x0001 0000 �W�q/�T�b |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //���� pMechTypeDB ���ƭ�
int u_PickerType = 0; // ������� 0-�T�b 1-���b
int u_TransType  = 0; // �ǰʤ覡  0-�`�ꪽ�| 1-���ƼҼ�
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b
char* pTransTypeDB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_TYPE"; // �ǰʤ覡DB
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("g_iPrivilege=%d\n",g_iPrivilege); // �v������
	//u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // Ū���]�w ������� �T���b
	// ������� �s�X�����
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	
	u_TransType  = (int)(GetDBValue(pTransTypeDB).lValue); // Ū���]�w �ǰʤ覡 ���ƼҼ�
	
	g_Pile_Select = 0; // ���� �ֳt�оɤ� ���|�ѼƳ]�w
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
		case 1: // ���c�Ѽ�
			//MsgBoxCall("msgboxUserLogin.txt");
			if(g_iPrivilege>=MACHINE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
				{
					if( (u_PickerType==MechType3) && (u_TransType==TransType_D) )			 		 // �T�b �`�ꪽ�|
					{
						strcpy(g_szCurrentFormName, "Mech_Axis3.txt");
						::PutCommand("Mech_Axis3.txt");
					}
					else if( (u_PickerType==MechType3) && (u_TransType==TransType_Tooth) ) // �T�b ���ƼҼ�
					{
						strcpy(g_szCurrentFormName, "Mech_Axis3_TN.txt");
						::PutCommand("Mech_Axis3_TN.txt");
					}
					else if( (u_PickerType==MechType5) && (u_TransType==TransType_D) ) 		 // ���b �`�ꪽ�|
					{
						strcpy(g_szCurrentFormName, "Mech_Axis.txt");
						::PutCommand("Mech_Axis.txt");
					}
					else if( (u_PickerType==MechType5) && (u_TransType==TransType_Tooth) ) // ���b ���ƼҼ�
					{
						strcpy(g_szCurrentFormName, "Mech_Axis_TN.txt");
						::PutCommand("Mech_Axis_TN.txt");
					}
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;	
		case 2: // �t�װѼ�
			if(g_iPrivilege>=MACHINE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
				if(u_PickerType == MechType5) // ���b
				{
					strcpy(g_szCurrentFormName, "SpeedSet.txt");
					::PutCommand("SpeedSet.txt");
				}
				else if(u_PickerType == MechType3) // �T�b
				{
					strcpy(g_szCurrentFormName, "SpeedSet_Axis3.txt");
					::PutCommand("SpeedSet_Axis3.txt");
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 3: // �k�s
			if(g_iPrivilege>=MACHINE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
				if(u_PickerType == MechType5) // ���b
				{
					strcpy(g_szCurrentFormName, "Zero.txt");
					::PutCommand("Zero.txt");
				}
				else if(u_PickerType == MechType3) // �T�b
				{
					strcpy(g_szCurrentFormName, "Zero_Axis3.txt");
					::PutCommand("Zero_Axis3.txt");
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;	
		case 4: // �ϰ�]�w
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
				if(u_PickerType == MechType5) // ���b
				{
					strcpy(g_szCurrentFormName, "ZoneLimit.txt");
					::PutCommand("ZoneLimit.txt");
				}
				else if(u_PickerType == MechType3) // �T�b
				{
					strcpy(g_szCurrentFormName, "ZoneLimit_Axis3.txt");
					::PutCommand("ZoneLimit_Axis3.txt");
				}
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 5: // �w������
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
			strcpy(g_szLastFormName, "Index.txt");
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
			if(u_PickerType == MechType5) // ���b
			{
				strcpy(g_szCurrentFormName, "LimitSet.txt");
				::PutCommand("LimitSet.txt");
			}
			else if(u_PickerType == MechType3) // �T�b
			{
				strcpy(g_szCurrentFormName, "LimitSet_Axis3.txt");
				::PutCommand("LimitSet_Axis3.txt");
			}				
			break;	
		case 6: // �T�ϳ]�w
			//::PutCommand(".txt");
			break;
		case 7: // �ɮ׼Ҳ�
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
		case 8: // ��ʾާ@
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, "Hand_1.txt");
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, "Index.txt");
				::PutCommand("Hand_1.txt");
			break;
		case 11: // �ֳt�о�
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
		case 12: // �i���о�
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
		case 13: // ���|�Ѽ�
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
		case 14: // �Ƶ{��
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
		case 15: // �Ҧ����
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
		case 16: // �˴��]�w
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
		case 17: // ���ɭץ� // ��m�ץ�
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
		case 22: // ĵ�����v
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
			strcpy(g_szCurrentFormName, "Alarm.txt");
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
			strcpy(g_szLastFormName, "Index.txt");
			::PutCommand("Alarm.txt");
			break;
		case 23: // �ާ@�i��
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
		case 24: // �X�ʸ�T		
			if(g_iPrivilege>=MANAGE)
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
				strcpy(g_szLastFormName, "Index.txt");
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
				if(u_PickerType == MechType5) // ���b
				{
					strcpy(g_szCurrentFormName, "DriveInfo.txt");
					::PutCommand("DriveInfo.txt");
				}
				else if(u_PickerType == MechType3) // �T�b
				{
					strcpy(g_szCurrentFormName, "DriveInfo_Axis3.txt");
					::PutCommand("DriveInfo_Axis3.txt");
				}		
			}
			else
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			break;
		case 31:// �Ͳ��޲z
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
		case 32:// �t�γ]�m
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
		case 33:// �t�έ��m
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
		case 34: //������T
			memset(g_szLastFormName, 0, sizeof(g_szLastFormName)); // �����W�@��
			strcpy(g_szLastFormName, "Index.txt");
			memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName)); // �e���U�@��
			if(u_PickerType == MechType5) // ���b
			{
				strcpy(g_szCurrentFormName, "Vers_IML.txt");
				::PutCommand("Vers_IML.txt");
			}
			else if(u_PickerType == MechType3) // �T�b
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
//				case _F1: // �D��
//					::PutCommand("Over.txt");
//					break;	
//				case _F2: // �D���
//					::PutCommand("Index.txt");
//					break;
//				case _F3: // IO
//					::PutCommand("Internal_IO.txt");
//					break;	
//				case _F4: // ��L
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

