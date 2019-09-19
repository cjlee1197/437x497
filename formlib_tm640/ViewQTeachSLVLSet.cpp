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
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b

// �b��
#define		Axis_X1				1  // X1�b
#define		Axis_Y1				2  // Y1�b
#define		Axis_Z				3  // Z�b
#define		Axis_X2				4  // X2�b
#define		Axis_Y2				5  // Y2�b
#define		Axis_C				6  // C�b
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
int u_PickerType = 0; // ������� 0-�T�b 1-���b
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b

CtmWnd*		pwndBtnSetDone	= NULL; // �]�w���� ���s

CtmWnd*		pwndPick_SLVL			= NULL; // ����X�� �ƭȳ]�w
CtmWnd*		pwndPickOut_SLVL	= NULL; // ���X�X�� �ƭȳ]�w

int u_Group=0;
char* QTeach_P_Page[2][20] = // �ֳt�о��I ����
{
	{
		"",
		"QTeach_Mode.txt", 							// 1 �Ҧ���� 
		"QTeach_WaitP_Axis3.txt", 			// 2 �ݾ��I 
		"", 														// 3 �U�ݾ��I
		"QTeach_ReadyDownP_Axis3.txt", 	// 4 �ǳƤU���I 
		"QTeach_PickDownP_Axis3.txt", 	// 5 ���X���U���I
		"QTeach_PickP_Axis3.txt", 			// 6 ���X�������I
		"QTeach_MoveOutP_Axis3.txt", 		// 7 ��X�I
		"QTeach_PlaceDownP_Axis3.txt", 	// 8 �m�����U���I
		"QTeach_PlaceP_Axis3.txt",			// 9 �m�����m���I
	},
	{
		"",
		"QTeach_Mode.txt", // �Ҧ����
		"QTeach_WaitP.txt", // �ݾ��I
		"", // �U�ݾ��I
		"QTeach_ReadyDownP.txt", // �ǳƤU���I 
		"QTeach_PickDownP.txt", // ���X���U���I
		"QTeach_PickP.txt", // ���X�������I
		"QTeach_MoveOutP.txt", // ��X�I
		"QTeach_PlaceDownP.txt", // �m�����U���I
		"QTeach_PlaceP.txt", // �m�����m���I
	}
};

char* P_2_SLVL_Set_DB = "MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_TYPE"; // �q���@�I���� �X�ʳ]�m DB
char* Pick_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER1"; // �U������ ���e�Z�� DB
char* PickOut_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER2"; // �W�ԥX�� ���e�Z�� DB

int u_DownDis				 = 0; // �U���Z��
int u_WaitPos_Y1 		 = 0; // �����I Y1 �]�w��m
int u_PickDownPos_Y1 = 0; // ���X�U���I Y1 �]�w��m
CtmWnd*		pwndPick_Dis			= NULL; // �U���Z�� �ƭ����

int u_BackDis				 = 0; // ��h�Z��
int u_MoveOutPos_X1	 = 0; // ��X�I X1 �]�w��m
int u_PickPos_X1 		 = 0; // ���X�����I X1 �]�w��m
CtmWnd*		pwndPickOut_Dis		= NULL; // ��h�Z�� �ƭ����

char* P2_POSSET_DBString[] = // P2 �����I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P5_POSSET_DBString[] = // P5 ���X���U���I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P6_POSSET_DBString[] = // P6 ���X�������I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P7_POSSET_DBString[] = // P7 ���X����X�I ��m�]�w DB�W��
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
	// ���o ���c�]�w 3 or 5�b
	u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // Ū���]�w ������� �T���b
	
	
	// ���o���� �]�w���� Btn
	pwndBtnSetDone = pwndSender->FindControlFromName("BtnSetDone");
	
	// ���o���� ����X��/���X�X�� �ƭȳ]�w 
	pwndPick_SLVL 	 = pwndSender->FindControlFromName("Pick_SLVL");
	pwndPickOut_SLVL = pwndSender->FindControlFromName("PickOut_SLVL");
	
	// ���o���� �U���Z��/��h�Z�� �ƭ���� 
	pwndPick_Dis 	 = pwndSender->FindControlFromName("Pick_Dis");
	pwndPickOut_Dis = pwndSender->FindControlFromName("PickOut_Dis");
	
	// �p�� �U���Z��
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
	// �p�� ��h�Z��
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


	// ��� �]�w�ƭ�
	ShowData(pwndPick_SLVL);
	ShowData(pwndPickOut_SLVL);
	
	u_Group = GetDBValue(P_2_SLVL_Set_DB).lValue; // ���o�q���I ���� �X�ʳ]�m��
	
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
 	
 	if(pwnd == pwndBtnSetDone) // �]�w���� Btn���U�u�_
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
|       		: �����e �]�w�ƭ�	�s�JDB																			 |
+---------------------------------------------------------------------------*/
void	SaveData()
{
	long value = 0;
	// ����X��
	pwndPick_SLVL->GetPropValueT("value", &value,sizeof(value));
	SetDBValue(Pick_SLVL_DB,value); // �ƭ� �g�Jdb
	printf("Set %s=%d\n",Pick_SLVL_DB,value);
	
	// ���X�X��
	pwndPickOut_SLVL->GetPropValueT("value", &value,sizeof(value));
	SetDBValue(PickOut_SLVL_DB,value); // �ƭ� �g�Jdb
	printf("Set %s=%d\n",PickOut_SLVL_DB,value);
}
/*---------------------------------------------------------------------------+
|  Function : SaveData(CtmWnd* pwnd, char* DB_ID)	   									       |
|       		: pwnd �������									 																 |
|       		: �����e �]�w�ƭ�	�s�JDB																			 |
+---------------------------------------------------------------------------*/
void	ShowData(CtmWnd* pwnd)
{
	char	DB_ID[256];	memset(DB_ID, 0, sizeof(DB_ID));
	pwnd->GetPropValueT("dbid1", DB_ID, sizeof(DB_ID)); // ���o��ƮwID
			
	long value = 0;
	if(pwnd != NULL)
	{
		value = GetDBValue(DB_ID).lValue; // Ū��DB�ƭ�
		pwnd->SetPropValueT("value",value); // �g�J�ƭ�
		printf("Show %s=%d\n",DB_ID,value);
		pwnd->CreateA();
		pwnd->Update();
	}
}