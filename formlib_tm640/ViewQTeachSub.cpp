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
#include	"ViewQTeachSub.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
// �y�{ �ϰ��I
#define		Origin			1 // ���I
#define		WaitP				2 // �ݾ��I
#define		DownWaitP		3 // �U�ݾ��I
#define		ReadyDownP	4	// �ǳƤU���I
#define		PickDownP		5	// ���X���U���I
#define		PickP				6	// ���X�������I
#define		MoveOutP		7	// ��X�I
#define		PlaceDownP	8	// �m�����U���I
#define		PlaceP			9	// �m�����m���I
#define		Homing		 10	// �^���I

// �b��
#define		Axis_X1				1  // X1�b
#define		Axis_Y1				2  // Y1�b
#define		Axis_Z				3  // Z�b
#define		Axis_X2				4  // X2�b
#define		Axis_Y2				5  // Y2�b
#define		Axis_C				6  // C�b

#define		OK					1  //

#define		Color_White		0xFFFF
#define		Color_Red			0xF800
#define		Color_Yellow 	0xFF80
#define		Color_LBlue		0xDFBF
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
int		AxisXOld =0,AxisYOld =0,AxisZOld =0,AxisX2Old =0,AxisY2Old =0,AxisCOld=0,
AxisXNew =0,AxisYNew =0,AxisX2New =0,AxisY2New =0,AxisZNew =0;// ����HotKey���U���b
int GetAxisPos=0; //���o���@�b

BOOL b_BtnNextP_OK = 0; // �����о� �i�ܤU�@��m
BOOL b_PosSet_OK[10] = {0}; // �����о� �]�w��m
BOOL b_All_PosSet_OK = 0; //�����о� �����]�w��m �ҳ]�w����
char* pAll_PosSet_OK_DB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_NUM"; // ��m�]�m���� DB

CtmWnd*		pwndBtnNextP	= NULL; // �U�@��m���s

CtmWnd*		pwndBtn_GetPos[6]	= {NULL}; // �����m���s
char* GetPos_String[] = // ���o��mBtn �W��
{
	"",
	"Btn_GetPos_X1",
	"Btn_GetPos_Y1",
	"Btn_GetPos_Z",
	"Btn_GetPos_X2",
	"Btn_GetPos_Y2",
};

char* AxisPosNow_DBString[] = // �����{�b��m DB�W��
{
	"",
	"MACHINE_INTERFACE_CONTROL_XAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_YAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_ZAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_X2AXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_Y2AXISPOSITION",
};


CtmWnd*   pwndQTeach_Pos[6] ={NULL}; // �ֳt�о� ��m�]�w
char* u_pszQTeach_PosString[] =
{
	"",
	"QTeach_Pos_X1",
	"QTeach_Pos_Y1",
	"QTeach_Pos_Z",
	"QTeach_Pos_X2",
	"QTeach_Pos_Y2",
};

CtmWnd*   pwndQTeach_Speed[6] ={NULL}; // �ֳt�о� �t�׳]�w
char* u_pszQTeach_SpeedString[] =
{
	"",
	"QTeach_Speed_X1",
	"QTeach_Speed_Y1",
	"QTeach_Speed_Z",
	"QTeach_Speed_X2",
	"QTeach_Speed_Y2",
};

CtmWnd*   pwndQTeach_DT[6] ={NULL}; // �ֳt�о� ���ɳ]�w DT-DelatTime
char* u_pszQTeach_DTString[] =
{
	"",
	"QTeach_DT_X1",
	"QTeach_DT_Y1",
	"QTeach_DT_Z",
	"QTeach_DT_X2",
	"QTeach_DT_Y2",
};


/*============================���f��m  �]�w============================*/

int QTeach_PGNo = 0; // �ֳt�о� ���B�J���X
char* SubStartNo_DB = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_TYPE"; // ��m���f �}�l�Ǹ� DB

long l_Position[6] = {0};
char* Sub_POSSET_DBString[] = // ���f��m�I ��m�]�w DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER1", // Z
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER2", // X2
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER3", // Y2
};
long l_Speed[6] = {0};
char* Sub_Speed_DBString[] = // ���f��m�I �t�׳]�w DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER4", // Z
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER5", // X2
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER6", // Y2
};
long l_Delaytime[6] = {0};
char* Sub_DT_DBString[] = // ���f��m�I ���ɳ]�w DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER7", // Z
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER8", // X2
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER9", // Y2
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
	// ���o���� �U�@��m���s
	pwndBtnNextP	= pwndSender->FindControlFromName("BtnNextP");	
	
	// ���o���� �����m���s
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ )
	{
		pwndBtn_GetPos[i] = pwndSender->FindControlFromName(GetPos_String[i]);
	} 
	
	// ���o���� �y�Ц�m�]�w
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		pwndQTeach_Pos[i] = pwndSender->FindControlFromName(u_pszQTeach_PosString[i]);
	} 
	// ���o���� �t�׳]�w
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		pwndQTeach_Speed[i] = pwndSender->FindControlFromName(u_pszQTeach_SpeedString[i]);
	}
	// ���o���� ���ɳ]�w
	for(int i = 0; i < sizeof(u_pszQTeach_DTString)/sizeof(u_pszQTeach_DTString[0]); i++ )
	{
		pwndQTeach_DT[i] = pwndSender->FindControlFromName(u_pszQTeach_DTString[i]);
	}	
	
	QTeach_PGNo = GetDBValue(SubStartNo_DB).lValue; // ���o ��m���f �}�l�Ǹ�
	printf("SubPage Get QTeach_PGNo=%d\n",QTeach_PGNo); 
	
	// �����ثe�b�����A �Χ@ ����HotKey���U���b
	AxisXOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0001;
	AxisYOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0002;
	AxisZOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0004;
	AxisX2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0001;
	AxisY2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0002;
	
	UpdateDataValue();
	UpdateTeach_Pos();
	UpdateBtnNextP();
	
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
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{              
	printf("OnChange\n");
	//taborder 5-9 �O�]�w��m
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl); // �����taborder������
	if(pwnd!=NULL)
	{
		switch(wIDControl)
		{
			case 5:
				GetAxisPos = Axis_Z;
				b_PosSet_OK[Axis_Z]=OK;
				break;
			case 8:
				GetAxisPos = Axis_X2;
				b_PosSet_OK[Axis_X2]=OK;
				break;
			case 9:
				GetAxisPos = Axis_Y2;
				b_PosSet_OK[Axis_Y2]=OK;
				break;
			default:
				break;
		}
		UpdateTeach_Pos();
		UpdateBtnNextP();
	}
	return wIDControl;
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
	AxisXNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0001;
	AxisYNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0002;
	AxisZNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0004;
	AxisX2New = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0001;
	AxisY2New = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0002;
	
	if(AxisXNew != AxisXOld)	//X1�b �Q���U
	{
		printf("AxisXNew\n");
		if(AxisXNew)
		{
			GetAxisPos = Axis_X1;
			pwndQTeach_Pos[Axis_X1]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_X1]->Update();
		}
		AxisXOld = AxisXNew;
	}
	else if(AxisYNew != AxisYOld)	//Y1�b �Q���U
	{
		printf("AxisYNew\n");
		if(AxisYNew)
		{
			GetAxisPos = Axis_Y1;
			pwndQTeach_Pos[Axis_Y1]->SetPropValueT("bgc",0xFF80); // ����
			pwndQTeach_Pos[Axis_Y1]->Update();
		}
		AxisYOld = AxisYNew;
	}
	else if(AxisZNew != AxisZOld)	//Z�b �Q���U
	{
		printf("AxisZNew\n");
		if(AxisZNew)
		{
			GetAxisPos = Axis_Z;
			pwndQTeach_Pos[Axis_Z]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_Z]->Update();
		}
		AxisZOld = AxisZNew;
	}
	else if(AxisX2New != AxisX2Old)	//X2�b �Q���U
	{
		printf("AxisX2New\n");
		if(AxisX2New)
		{
			GetAxisPos = Axis_X2;
			pwndQTeach_Pos[Axis_X2]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_X2]->Update();
		}
		AxisX2Old = AxisX2New;
	}
	else if(AxisY2New != AxisY2Old)	//Y2�b �Q���U
	{
		printf("AxisY2New\n");
		if(AxisY2New)
		{
			GetAxisPos = Axis_Y2;
			pwndQTeach_Pos[Axis_Y2]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_Y2]->Update();
		}
		AxisY2Old = AxisY2New;
	}
}

void    OnDestroyA(CtmWnd* pwndSender)
{
	
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
WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA=%d\n",wKey);
	switch(wKey)
	{
		case MoveOutP:
			::PutCommand("QTeach_MoveOutP.txt");
			break;
		case PlaceDownP:
				if(b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
				{
					// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
					QTeach_PGNo++;
					// Z ��X
					{
						if(pwndQTeach_Pos[Axis_Z]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_Z]->GetPropValueT("value", &l_Position[Axis_Z],sizeof(l_Position[Axis_Z]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Z]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_Z]);
							SetDBValue(Sub_POSSET_DBString[Axis_Z], l_Position[Axis_Z]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_Z]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_Z]->GetPropValueT("value", &l_Speed[Axis_Z],sizeof(l_Speed[Axis_Z]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Z]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_Speed_DBString[Axis_Z], l_Speed[Axis_Z]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_Z]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_Z]->GetPropValueT("value", &l_Delaytime[Axis_Z],sizeof(l_Delaytime[Axis_Z]));
							if(l_Delaytime[Axis_Z]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Z]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_DT_DBString[Axis_Z], l_Delaytime[Axis_Z]); // �g�J WaitP db
						}	
					}
					QTeach_PGNo++;
					// X2 �e�i
					{
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_X2]);
							SetDBValue(Sub_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // �g�J WaitP db
						}	
					}
					QTeach_PGNo++;
					// Y2 �U�� (����f)
					{
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_Y2]);
							SetDBValue(Sub_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // �g�J WaitP db
						}	
					}			
					QTeach_PGNo++;
					// Y2 �W�� �^��
					{
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // �]�w��m P1����m
						{
							l_Position[Axis_Y2] = GetDBValue(P7_POSSET_DBString[Axis_Y2]).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_Y2]);
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						}	
					}	
					QTeach_PGNo++;
					// X2 ��h �^��
					{
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							l_Position[Axis_X2] = GetDBValue(P7_POSSET_DBString[Axis_X2]).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_X2]);
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						}	
					}
					::PutCommand("QTeach_PlaceDownP.txt");
				}
				else
					MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			break;
		default:
			break;
	}
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
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
 		
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ ) // �����m Btn���U�u�_
	{
		if(pwnd == pwndBtn_GetPos[i])
		{
			GetPosNow(i);
			b_PosSet_OK[i]=OK;
			UpdateTeach_Pos();
			UpdateBtnNextP();
		}
	} 
	
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : UpdateDataValue()                  						   				   		 |
|       		: ��s�]�w��m�B�]�w�t�׼ƭ�																		 |
+---------------------------------------------------------------------------*/
void	UpdateDataValue()
{
	printf("Update data\n");
	long AxisSetPos[6] = {0};
	long AxisSetSpeed[6] = {0};
	long AxisSetDT[6] = {0};
	
	for(int WhichAxis=0; WhichAxis<6; WhichAxis++)
	{
		if(pwndQTeach_Pos[WhichAxis] != NULL) // Set Pos
		{
			// Get data Pos value from db
			char	SetPos_DBID[256];	memset(SetPos_DBID, 0, sizeof(SetPos_DBID));
			pwndQTeach_Pos[WhichAxis]->GetPropValueT("dbid1", SetPos_DBID, sizeof(SetPos_DBID));
			AxisSetPos[WhichAxis] = GetDBValue(SetPos_DBID).lValue;
			//printf("Get %s = %d\n",SetPos_DBID,AxisSetPos[WhichAxis]);
			// Update show data
			pwndQTeach_Pos[WhichAxis]->SetPropValueT("value",AxisSetPos[WhichAxis]);
			pwndQTeach_Pos[WhichAxis]->Update();
			pwndQTeach_Pos[WhichAxis]->UpdateAll();
			//printf("Set Axis%d = %d\n",WhichAxis,AxisSetPos[WhichAxis]);
		}
		
		if(pwndQTeach_Speed[WhichAxis] != NULL) // Set Speed
		{
			// Get data Speed value from db
			char	SetSpeed_DBID[256];	memset(SetSpeed_DBID, 0, sizeof(SetSpeed_DBID));
			pwndQTeach_Speed[WhichAxis]->GetPropValueT("dbid1", SetSpeed_DBID, sizeof(SetSpeed_DBID));
			AxisSetSpeed[WhichAxis] = GetDBValue(SetSpeed_DBID).lValue;
			//printf("Get %s = %d\n",SetSpeed_DBID,AxisSetSpeed[WhichAxis]);
			// Update show data
			pwndQTeach_Speed[WhichAxis]->SetPropValueT("value",AxisSetSpeed[WhichAxis]);
			pwndQTeach_Speed[WhichAxis]->Update();
			pwndQTeach_Speed[WhichAxis]->UpdateAll();
			//printf("Set Axis%d Speed = %d\n",WhichAxis,AxisSetSpeed[WhichAxis]);
		}
		
		if(pwndQTeach_DT[WhichAxis] != NULL) // Set DelayTime
		{
			// Get data DelayTime value from db
			char	SetDT_DBID[256];	memset(SetDT_DBID, 0, sizeof(SetDT_DBID));
			pwndQTeach_DT[WhichAxis]->GetPropValueT("dbid1", SetDT_DBID, sizeof(SetDT_DBID));
			AxisSetDT[WhichAxis] = GetDBValue(SetDT_DBID).lValue;
			// Update show data
			pwndQTeach_DT[WhichAxis]->SetPropValueT("value",AxisSetDT[WhichAxis]);
			pwndQTeach_DT[WhichAxis]->Update();
			pwndQTeach_DT[WhichAxis]->UpdateAll();
		}
		
	}
}

/*---------------------------------------------------------------------------+
|  Function : GetPosNow(int GetAxisPos)                  				   		       |
|       		: �����e GetAxisPos �b��m																		 |
+---------------------------------------------------------------------------*/
void	GetPosNow(int GetAxisPos)
{
	printf("Get Pos %d\n",GetAxisPos);

	if( (GetAxisPos==Axis_Z) || (GetAxisPos==Axis_X2) || (GetAxisPos==Axis_Y2) )
	{
		long AxisPosNow = GetDBValue(AxisPosNow_DBString[GetAxisPos]).lValue; // ���o�ƭ�
		printf("Get %s = %d\n",u_pszQTeach_PosString[GetAxisPos],AxisPosNow);
		if(pwndQTeach_Pos[GetAxisPos] != NULL)
		{
			printf("Set Axis%d = %d\n",GetAxisPos,AxisPosNow);
			pwndQTeach_Pos[GetAxisPos]->OnLoseFocus();
			pwndQTeach_Pos[GetAxisPos]->SetPropValueT("value",AxisPosNow); // �g�J�ƭ�
			pwndQTeach_Pos[GetAxisPos]->SetPropValueT("bgc",0xFFFF);
			pwndQTeach_Pos[GetAxisPos]->CreateA();
			pwndQTeach_Pos[GetAxisPos]->Show();
		}
		UpdateTeach_Pos();
		UpdateBtnNextP();
	}	
}

/*---------------------------------------------------------------------------+
|  Function : UpdateTeach_Pos()     	             										  		 |
|       		: ��s���� ��m�]�w��																						 |
+---------------------------------------------------------------------------*/
void	UpdateTeach_Pos()
{
	printf("UpdateTeach_Pos()\n");
	int u_All_PosSet_OK=0;
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		if(pwndQTeach_Pos[i]!=NULL)
		{
			if(b_All_PosSet_OK==1)
				b_PosSet_OK[i]=OK;
			if(b_PosSet_OK[i]==OK)
			{
				u_All_PosSet_OK++;
				pwndQTeach_Pos[i]->SetPropValueT("bgc",Color_LBlue); // �զ�
			}
			else
			{
				pwndQTeach_Pos[i]->SetPropValueT("bgc",Color_Yellow); // ����
			}
			//pwndQTeach_Pos[i]->CreateA();
			pwndQTeach_Pos[i]->Show();
			pwndQTeach_Pos[i]->OnLoseFocus();
		}
	} 
	if(u_All_PosSet_OK>=3)
		b_All_PosSet_OK =OK; //������m�ҳ]�w����
}

/*---------------------------------------------------------------------------+
|  Function : UpdateBtnNextP()                  										  		   |
|       		: ��s���� �U�@��m																							 |
+---------------------------------------------------------------------------*/
void	UpdateBtnNextP()
{
	printf("UpdateBtnNextP()\n");
	if(pwndBtnNextP!=NULL)
	{
		if(b_All_PosSet_OK==OK) // ��m && �v�� && �t�� �]�w���� &&(b_All_SpdSet_OK==OK)
			{
				SetDBValue(pAll_PosSet_OK_DB, 1); // �g�J ��m�]�w���� db
				b_BtnNextP_OK = OK;
				pwndBtnNextP->SetPropValueT("upbitmap","res_tm640/pic/PGBtnSelect.bmp");
			}
		else
			{
				b_BtnNextP_OK = 0;
				pwndBtnNextP->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
			}
		pwndBtnNextP->CreateA();
		pwndBtnNextP->Update();
	}
}