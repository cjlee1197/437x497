/*===========================================================================+
|  Class    : View Qteach								                                     |
|  Task     : View Qteach								                                     |
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
#include	"ViewQTeach.h"
#include	"../commonaction.h"
#include	"../font.h"
#include	"../utils.h"

/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b
#define		EncType_Abs			0x00000000  // ����
#define		EncType_Res		  0x00010000  // �W�q
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
#define		GPIO_SAFEKEY_BIT	0x14 // �w�����s
#define		GPIO_SAFEKEY_0 0x4
#define		GPIO_SAFEKEY_1 0x10
#define		GPIO_SAFEKEY_2 0x14
#define		MANUAL_CONTINUE_MODE	0
#define		MANUAL_JOG_MODE	1
#define		MANUAL_STOP_MODE	2
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

#define     MaxDBNum	      100// �оɨB�J�ƶq�W��
#define     PARA_NUM	      5	 // �ѼƼƶq
// �ʧ@����
#define		Action_Axis				1  // �b�ʧ@
#define		Action_Wait				2  // ����
#define		Action_Permit			3  // ���\

#define		Action_Valve			4	 // �֪�
#define		Action_Tag				5  // ����
#define		Action_Goto				6  // ����
#define		Action_Detect			8  // �˴�
#define		Action_Pile				12 // ���|
#define		Action_Sub				13 // �Ƶ{��
// �b��
#define		Axis_X1				1  // X1�b
#define		Axis_Y1				2  // Y1�b
#define		Axis_Z				3  // Z�b
#define		Axis_X2				4  // X2�b
#define		Axis_Y2				5  // Y2�b
#define		Axis_C				6  // C�b
//����
#define		Wait_IMEStop		1  // �g�X���氱
#define		Wait_MoldOpen		2  // �}�ҧ���
#define		Wait_MoldClose	3  // ���ҧ���
#define		Wait_SDoorClose	4  // �w������
#define		Wait_IMAuto			5  // �g�X���۰�
#define		Wait_PushOut		6  // ���X����
#define		Wait_PullBack		7  // ���h����

// ���\
#define		Permit_MoldSafe				1  // �ҰϦw��
#define		Permit_MoldClose			2  // ���\����
#define		Permit_PushOut				3  // ���\���X
#define		Permit_PickerESTOP		4  // �����氱
#define		Permit_MoldOpen				5  // ���\�}��
#define		Permit_PullBack				6  // ���\���h

// �֪�
#define		Valve_AxisC_V				1  // �D�u����
#define		Valve_AxisC_H				2  // �D�u����
#define		Valve_Clamp1				3  // ��1
#define		Valve_Clamp2				4  // ��2
#define		Valve_Suck1					5  // �l1
#define		Valve_Suck2					6  // �l2
#define		Valve_Choose1				71 // ���1
#define		Valve_Choose2				72 // ���2	
#define		Valve_Choose3				73 // ���3
#define		Valve_Choose4				74 // ���4				

#define		ON					1  //
#define		OFF					0  //
#define		START				1  //
#define		END					1  //
#define		Selected		1  //
#define		NO					0  //
#define		OK					1  //

#define		Color_White		0xFFFF
#define		Color_Red			0xF800
#define		Color_Yellow 	0xFF80
#define		Color_LBlue		0xDFBF

#define		MIN				0  //
#define		MAX				1  //
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
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b

BOOL b_RunOnlyOne = FALSE; // �Q�� Update() �Ө�o�� �u����@��
BOOL b_SaveFlag = FALSE; // �O�_�x�s�ƭ�

int WhichAxis=0;
int WhichAxis_Old=-1;
char* pWhichAxisDB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_TYPE"; // �������@�b DB

int 	Manual_Type=MANUAL_STOP_MODE; // �I��w�����s

int			u_QTeach_Type=0,u_QTeach_Pos=0;

CtmWnd*		pwndBtn_PosHV	= NULL; // ���A��ܫ��s
CtmWnd*		pwndImg_NowMoveAxis	= NULL; // �{�b���ʶb ��� �Ϥ�
CtmWnd*		pwndStr_NowMoveAxis	= NULL; // �{�b���ʶb ��� ��r

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

CtmWnd*		pwndBar_PosHint[6] = {NULL}; // ��m���� bar
char* Bar_PosHint_String[] = // ��m���� bar �W��
{
	"",
	"PosHint_X1",
	"PosHint_Y1",
	"PosHint_Z",
	"PosHint_X2",
	"PosHint_Y2",
};

CtmWnd*		pwndImg_PosHint[6] = {NULL}; // ��m���� �Ϥ�
char* Img_PosHint_String[] = // ��m���� �Ϥ� �W��
{
	"",
	"Img_PosHint_X1",
	"Img_PosHint_Y1",
	"Img_PosHint_Z",
	"Img_PosHint_X2",
	"Img_PosHint_Y2",
};
CtmWnd*		pwndMask_PosHint[6] = {NULL}; // ��m���� �B�n
char* Mask_PosHint_String[] = // ��m���� �Ϥ� �W��
{
	"",
	"Mask_PosHint_X1",
	"Mask_PosHint_Y1",
	"Mask_PosHint_Z",
	"Mask_PosHint_X2",
	"Mask_PosHint_Y2",
};
long AxisPosNow_Old[6]={0};
long AxisPosNow[6]={0};

CtmWnd*		pwndBtnTeachStart	= NULL; // �}�l�оɫ��s
CtmWnd*		pwndBtnNextP	= NULL; // �U�@��m���s
CtmWnd*		pwndStr_ClampSelect	= NULL; // �v���Τ�r

CtmWnd*		pwndCheck_Move	= NULL; // �ǳƤU���I ��� ���ʫ�U�� �Ĥ�
CtmWnd*		pwndBtn_Check_Move	= NULL; // �ǳƤU���I ��� ���ʫ�U�� Btn
CtmWnd*		pwndImg_QTeach_ReadyDownP	= NULL; // �ǳƤU���I ��ܹϤ�

CtmWnd*		pwndCheck_SubUse	= NULL; // ���u�ϥ� �Ĥ�
CtmWnd*		pwndCheck_SubSync	= NULL; // ���u�P�D�u�P�� �Ĥ�
CtmWnd*		pwndBtn_SubUse		= NULL; // ���u�ϥ� Btn
CtmWnd*		pwndBtn_SubSync		= NULL; // ���u�P�D�u�P�� Btn
CtmWnd*		pwndMask_SubArm		= NULL; // ���u �B�n


CtmWnd*		pwndBtn_SLVL	= NULL; // �X�ʨϥ� Btn
CtmWnd*		pwndCheck_SLVL	= NULL; // �X�ʨϥ� �Ĥ�
CtmWnd*		pwndMask_SLVL	= NULL; // �X�ʨϥ� �B�n
CtmWnd*		pwndPick_SLVL			= NULL; // ����X�� �ƭȳ]�w
CtmWnd*		pwndPickOut_SLVL	= NULL; // ���X�X�� �ƭȳ]�w
CtmWnd*		pwndSLVL_Logo	= NULL; // �X�ʨϥ� Logo 
CtmWnd*		pwndPick_Dis			= NULL; // �U���Z�� �ƭ����
CtmWnd*		pwndPickOut_Dis		= NULL; // ��h�Z�� �ƭ����

CtmWnd*		pwndCheck_Ejector	= NULL; // �˴� ���h����(�}�l���X) �Ĥ�

CtmWnd*		pwndBtn_PileUse	= NULL; // ���|��� Btn
CtmWnd*		pwndCheck_Pile	= NULL; // ���|��� �Ĥ�
CtmWnd*		pwndMask_Pile		= NULL; // ���|�ռ� �B�n
CtmWnd*   pwndBtn_PileNum[10] = {NULL}; // ���|�ռ� Btn
CtmWnd*   pwndBtn_PileChoose[10] = {NULL}; // ���|�ռ� ��ܶs

char* PileNum_String[] = // ���|�ռ� �W��
{
	"",
	"Btn_PileUse1",
	"Btn_PileUse2",
	"Btn_PileUse3",
	"Btn_PileUse4",
};
char* PileChoose_String[] = // ���|�ռ� ��ܶs �W��
{
	"",
	"Choose_Pile1",
	"Choose_Pile2",
	"Choose_Pile3",
	"Choose_Pile4",
};




CtmWnd*		pwndStr_Place_Z	= NULL; // ��m/���| ��r����
CtmWnd*		pwndStr_Place_X	= NULL; // ��m/���| ��r����

BOOL b_BtnNextP_OK = 0; // �����о� �i�ܤU�@��m
BOOL b_PosSet_OK[10] = {0}; // �����о� �]�w��m
BOOL b_All_PosSet_OK = 0; //�����о� �����]�w��m �ҳ]�w����
char* pAll_PosSet_OK_DB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_NUM"; // ��m�]�m���� DB
BOOL b_SpdSet_OK[10] = {0}; // �����о� �]�w��m
BOOL b_All_SpdSet_OK = 0; //�����о� �����]�w��m �ҳ]�w����
BOOL b_ClampSet_OK = OK; // �����о� ����T�{

int Teach_SetNum[2][10]= // �U�I �һݳ]�w�ƶq [3,5�b][���@�I]
{
	{
		0, // 0
		0, // 1
		3, // 2
		0, // 3
		1, // 4
		1, // 5
		1, // 6
		2, // 7
		2, // 8	
		1, // 9	
	},
	{
		0, // 0
		0, // 1
		5, // 2
		0, // 3
		2, // 4
		2, // 5
		2, // 6
		4, // 7
		2, // 8	
		1, // 9	
	},
};


char* PosHV_ImgPath[] = // ���A��ܹϤ�
{
	"",
	"res_tm640/pic/picker/Clamp_Vertical.bmp", // ���� 1
	"res_tm640/pic/picker/Clamp_Horizontal.bmp", // ���� 2
	"res_tm640/pic/picker/Clamp_Vertical_Disable.bmp",
	"res_tm640/pic/picker/Clamp_Horizontal_Disable.bmp",
};

char* Axis_ImgPath[] = // �b ��ܹϤ�
{
	"res_tm640/pic/picker/Axis_NULL.bmp",
	"res_tm640/pic/picker/Axis_X1.bmp", // Axis_X1
	"res_tm640/pic/picker/Axis_Y1.bmp", // Axis_Y1
	"res_tm640/pic/picker/Axis_Z.bmp", // Axis_Z
	"res_tm640/pic/picker/Axis_X2.bmp", // Axis_X2
	"res_tm640/pic/picker/Axis_Y2.bmp", // Axis_Y2
};
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

/*=================================�v��]�w=================================*/
int u_status=0;
int u_clamp_onoff[10]={0}; // �v�㪬�A
int u_detect_onoff[10]={0}; // �v���˴�
int ClampSelect; // �v���� �l1 �l2 ��1 ��2
int ClampDetectSelect; // �v���˴���� �l1 �l2 ��1 ��2

char* ClampSelect_DB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_STEP"; // �v���� DB
char* ClampDetectSelect_DB = "MACHINE_PROFILE_NUM1_EQUIPMENT2_ACTION_NUM"; // �v���˴���� DB

BOOL b_Select[10] = {FALSE}; // �v���λP�_ ��� u_pszClamp_SelectString[]
BOOL b_DetectSelect[10] = {FALSE}; // �v���˴���λP�_ ��� u_pszDetect_SelectString[]

CtmWnd*   pwndClamp_Select[10] ={NULL}; // �ֳt�о� �v����
CtmWnd*   pwndCheckBox[10] ={NULL}; // �ֳt�о� �v���� CheckBox

CtmWnd*   pwndDetect_Select[10] ={NULL}; // �ֳt�о� �v���˴����

CtmWnd*		pwndClamp_Mask	= NULL; // �v�� �B�n
CtmWnd*		pwndStr_Clamp	= NULL; // �v�� ���� ��r
CtmWnd*		pwndStr_Suck	= NULL; // �v�� �l�L ��r
CtmWnd*		pwndStr_Clamp_Detect1	= NULL; // �v�� �˴� ��r1
CtmWnd*		pwndStr_Clamp_DT1	= NULL; // �v�� ���� ��r1
CtmWnd*		pwndStr_Clamp_Detect2	= NULL; // �v�� �˴� ��r2
CtmWnd*		pwndStr_Clamp_DT2 = NULL; // �v�� ���� ��r2

CtmWnd*		pwndDetect_Mask1	= NULL; // �v���˴���� �B�n1
CtmWnd*		pwndDetect_Mask2	= NULL; // �v���˴���� �B�n2
CtmWnd*		pwndStr_Detect1	= NULL; // �v���˴���� ��r1
CtmWnd*		pwndStr_Detect2	= NULL; // �v���˴���� ��r2


char* u_pszClamp_SelectString[] = // �v��������X
{
	"", // 0
	"",
	"",
	"Select_Clamp1", // 3
	"Select_Clamp2", // 4
	"Select_Suck1", // 5
	"Select_Suck2", // 6
	"",
	"",
	"",
	"",
};
char* u_pszCheckBoxString[] = // �T�{ �v���� 
{
	"",
	"",
	"",
	"Check_Clamp1",
	"Check_Clamp2",
	"Check_Suck1",
	"Check_Suck2",
	"",
	"",
};

char* u_pszDetect_SelectString[] = // �v���˴��������X
{
	"", // 0
	"",
	"",
	"DetectSelect_Clamp1", // 3
	"DetectSelect_Clamp2", // 4
	"DetectSelect_Suck1", // 7
	"DetectSelect_Suck2", // 8
	"",
	"",
	"",
	"",
};

char* CheckBox_ImgPath[] = //  �ĤĿ�� ��ܹϤ�
{
	"res_tm640/pic/picker/Check_No.bmp",
	"res_tm640/pic/picker/Check_OK.bmp",
};
char* ChooseBox_ImgPath[] = //  ��� ��ܹϤ�
{
	"res_tm640/pic/picker/Choose_No.bmp",
	"res_tm640/pic/picker/Choose_OK.bmp",
};


char* Clamp_ImgPath[] = // �v����ܹϤ�
{
	"",	// 0
	"",	// 1
	"",	// 2
	"res_tm640/pic/picker/Qclamp1_off.bmp", // 3
	"res_tm640/pic/picker/Qclamp2_off.bmp", //4
	"res_tm640/pic/picker/QCupulaClose1.bmp", // 5
	"res_tm640/pic/picker/QCupulaClose2.bmp",	// 6
	"",	// 7
	"",	// 8
	"", // 9
	"",	// 10
	"",	// 11
	"",	// 12
	"res_tm640/pic/picker/Qclamp1_on.bmp",	// 13
	"res_tm640/pic/picker/Qclamp2_on.bmp",	// 14
	"res_tm640/pic/picker/QCupulaOpen1.bmp", // 15
	"res_tm640/pic/picker/QCupulaOpen2.bmp",	// 16
	"",	// 15
	"",	// 16
	"", // 19
	"",	// 20
	"",	// 21
	"",	// 22
	"", // 23
	"", // 24
	"",	// 25
	"",	// 26
	"", // 27
	"",	// 28
	"", // 29
	"",	// 30
};
char* Clamp_Hint_ImgPath[] = // �v�㴣����ܹϤ�
{
	"",	// 0
	"",	// 1
	"",	// 2
	"res_tm640/pic/picker/Hintclamp1_off.bmp", // 3
	"res_tm640/pic/picker/Hintclamp2_off.bmp", //4
	"res_tm640/pic/picker/HintCupulaClose1.bmp", // 5
	"res_tm640/pic/picker/HintCupulaClose2.bmp",	// 6
	"",	// 7
	"",	// 8
	"", // 9
	"",	// 10
	"",	// 11
	"",	// 12
	"res_tm640/pic/picker/Hintclamp1_on.bmp",	// 13
	"res_tm640/pic/picker/Hintclamp2_on.bmp",	// 14
	"res_tm640/pic/picker/HintCupulaOpen1.bmp", // 15
	"res_tm640/pic/picker/HintCupulaOpen2.bmp",	// 16
	"", // 17
	"",	// 18
	"", // 19
	"",	// 20
	"",	// 21
	"",	// 22
	"", // 23
	"", // 24
	"",	// 25
	"",	// 26
	"", // 27
	"",	// 28
	"", // 29
	"",	// 30
};

CtmWnd*   pwndBtn_Clamp[10] ={NULL}; // �ֳt�о� �v�� Btn
char* u_pszBtn_Clamp_String[] =
{
	"",
	"",
	"",
	"Btn_Clamp1", // 3
	"Btn_Clamp2", // 4
	"Btn_Suck1", // 5
	"Btn_Suck2", // 6
	"",
	"",
};
char* P6_Clamp_onoff_DBString[] = // P6�����I �v�㪬�A DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_TYPE", // 3 ����1
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_NUM", // 4 ����2
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_TYPE", // 5 �l�L1
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_NUM", // 6 �l�L2
	"",
	"",
};
char* P9_Clamp_onoff_DBString[] = // P9��m�I �v�㪬�A DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_TYPE", // 3 ����1
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_NUM", // 4 ����2
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_TYPE", // 5 �l�L1
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_NUM", // 6 �l�L2
	"",
	"",
};


CtmWnd*   pwndDetectBox[10] ={NULL}; // �ֳt�о� �v���˴� CheckBox (���I�ߴ�)
char* u_pszDetectBoxString[] = // �T�{ �v���˴���� 
{
	"",
	"",
	"",
	"Detect_Clamp1",
	"Detect_Clamp2",
	"Detect_Suck1",
	"Detect_Suck2",
	"",
	"",
};

char* P6_Clamp_detect_DBString[] = // P6�����I �v���˴����A DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_TYPE", // 3 ����1
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_NUM", // 4 ����2
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_TYPE", // 5 �l�L1
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_NUM", // 6 �l�L2
	"",
	"",
};
char* P9_Clamp_detect_DBString[] = // P9��m�I �v���˴����A DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_TYPE", // 3 ����1
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_NUM", // 4 ����2
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_TYPE", // 5 �l�L1
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_NUM", // 6 �l�L2
	"",
	"",
};

CtmWnd*   pwndQTeach_Clamp_DT[10] ={NULL}; // �ֳt�о� �v�㩵�� �ƭȿ�J
char* u_pszQTeach_Clamp_DTString[] =
{
	"",
	"",
	"",
	"QTeach_DT_Clamp1", // 3
	"QTeach_DT_Clamp2", // 4
	"QTeach_DT_Suck1", // 5
	"QTeach_DT_Suck2", // 6
	"",
	"",
};
long l_Clamp_Delaytime[10] = {0};
char* P6_Clamp_DT_DBString[] = // P6�����I �v�㩵�� DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER1", // 3 ����1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER2", // 4 ����2
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER3", // 5 �l�L1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER4", // 6 �l�L2
	"",
	"",
};
char* P9_Clamp_DT_DBString[] = // P9�����I �v�㩵�� DB�W��
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER5", // 3 ����1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER6", // 4 ����2
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER7", // 5 �l�L1
	"MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_PARAMETER8", // 6 �l�L2
	"",
	"",
};

/*=================================���u �]�w=================================*/
BOOL b_SubArmUse 			 = FALSE; // ���u ��λP�_
BOOL b_SubArmSync			 = FALSE; // ���u ���u�P�D�u�P�� �P�_
int u_SubArmUse_WaitP 		 = 0; // ���u �W�[�B�� �����I
int u_SubArmUse_PickDownP  = 0; // ���u �W�[�B�� �U���I
int u_SubArmUse_PickP			 = 0; // ���u �W�[�B�� �����I
int u_SubArmUse_MoveOutP	 = 0; // ���u �W�[�B�� ��X�I
int u_SubArmUse_SubPlaceP  = 0; // ���u �W�[�B�� ��X�I (����f)
char* SubStartNo_DB = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_TYPE"; // ��m���f �}�l�Ǹ� DB
char* Check_SubUse_DB  = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_NUM"; // �T�{�O�_��� ���u�ϥ� DB
char* Check_SubSync_DB = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_STEP"; // �T�{�O�_��� ���u�P�D�u�P�� DB

/*=================================���ʫ�U�� �]�w=================================*/
BOOL b_Check_Move = FALSE; // ���ʫ�U�� ��λP�_
int u_Check_Move =0; // ���ʫ�U�� �W�[�B��
char* Check_Move_DB = "MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_STEP"; // �T�{�O�_��� ���ʫ�U�� DB

/*==============================�X�ʨϥ�(�b���e) �]�w==============================*/
BOOL b_Check_SLVL = FALSE; // �X�ʨϥ� ��λP�_
int	 u_Check_SLVL =0; // �X�ʨϥ� �ƭ� ��u_Group��bit
char* Check_SLVL_DB = "MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_STEP"; // �T�{�O�_��� �X�ʨϥ� DB
char* P_2_SLVL_Set_DB = "MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_TYPE"; // �q���@�I���� �X�ʳ]�m DB
char* Pick_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER1"; // �U������ ���e�Z�� DB
char* PickOut_SLVL_DB ="MACHINE_PROFILE_NUM3_EQUIPMENT2_ACTION_PARAMETER2"; // �W�ԥX�� ���e�Z�� DB


/*==========================���h����(�}�l���X) �]�w==============================*/
BOOL b_Check_Ejector = FALSE; // �}�l���X ��λP�_
int u_Check_Ejector =0; // �}�l���X �W�[�B��
char* Check_Ejector_DB = "MACHINE_PROFILE_NUM4_EQUIPMENT2_ACTION_STEP"; // �T�{�O�_��� �}�l���X DB

/*=================================���| �]�w=================================*/
int PileNum = 0; // ���| �ո��X
int u_PileNum =0; // ���| �ϥ� �W�[�B��
char* Check_Pile_DB = "MACHINE_PROFILE_NUM2_EQUIPMENT2_ACTION_NUM"; // �T�{�O�_��� ���|�ո� DB
char* PilePage[] =
{
	"",
	"Pile_1.txt", // 1
	"Pile_2.txt", // 2
	"Pile_3.txt", // 3
	"Pile_4.txt", // 4
};
/*=================================��m�]�w=================================*/
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
CtmWnd*   pwndQTeach_Pos_Max[6] ={NULL}; // �ֳt�о� ��m �W�U�� ��ܤ�r
CtmWnd*   pwndQTeach_Pos_Min[6] ={NULL};
char* u_pszQTeach_Pos_MaxString[] =
{
	"",
	"Pos_Max_X1",
	"Pos_Max_Y1",
	"Pos_Max_Z",
	"Pos_Max_X2",
	"Pos_Max_Y2",
};
char* u_pszQTeach_Pos_MinString[] =
{
	"",
	"Pos_Min_X1",
	"Pos_Min_Y1",
	"Pos_Min_Z",
	"Pos_Min_X2",
	"Pos_Min_Y2",
};

long l_Position[6] = {0};
char* P2_POSSET_DBString[] = // P2 �����I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P4_POSSET_DBString[] = // P4 �ǳƤU���I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER5",
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
char* P8_POSSET_DBString[] = // P8 �m�����U���I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P9_POSSET_DBString[] = // P9 �m������m�I ��m�]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER5",
};

/*=================================�t�׳]�w=================================*/
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
long l_Speed[5] = {0};
char* P2_Speed_DBString[] = // P2 �����I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P4_Speed_DBString[] = // P4 �ǳƤU���I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P5_Speed_DBString[] = // P5 ���X���U���I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P6_Speed_DBString[] = // P6 ���X�������I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P7_Speed_DBString[] = // P7 ���X����X�I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P8_Speed_DBString[] = // P8 �m�����U���I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_PARAMETER10",
};
char* P9_Speed_DBString[] = // P9 �m������m�I �t�׳]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER6",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER7",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER8",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER9",
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_PARAMETER10",
};

/*=================================���A�]�w=================================*/
char*	PosStatus_DBString[]= // ���A DB�W��
{
	"",
	"", // P1
	"MACHINE_PROFILE_NUM50_EQUIPMENT2_ACTION_STEP", // P2 �����I�����A
	"",
	"MACHINE_PROFILE_NUM49_EQUIPMENT2_ACTION_STEP", // P4
	"MACHINE_PROFILE_NUM48_EQUIPMENT2_ACTION_STEP", // P5
	"MACHINE_PROFILE_NUM47_EQUIPMENT2_ACTION_STEP", // P6 �����I�����A
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_STEP", // P7
	"MACHINE_PROFILE_NUM45_EQUIPMENT2_ACTION_STEP", // P8
	"MACHINE_PROFILE_NUM44_EQUIPMENT2_ACTION_STEP", // P9 ��m�I�����A
};

/*=================================���ɳ]�w=================================*/
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
long l_Delaytime[6] = {0};
char* P2_DT_DBString[] = // P2 �����I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM43_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P4_DT_DBString[] = // P4 �ǳƤU���I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM42_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P5_DT_DBString[] = // P5 ���X���U���I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM41_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P6_DT_DBString[] = // P6 ���X�������I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM40_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P7_DT_DBString[] = // P7 ���X����X�I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM39_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P8_DT_DBString[] = // P8 �m�����U���I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM38_EQUIPMENT2_ACTION_PARAMETER5",
};
char* P9_DT_DBString[] = // P9 �m������m�I ���ɳ]�w DB�W��
{
	"",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM37_EQUIPMENT2_ACTION_PARAMETER5",
};


int u_Group;
char*	GroupName[] = // �s�զW�� ��ӵe���}�����D
{
	"",
	"TEACH_QTEACH_Origin", 			// 1
	"TEACH_QTEACH_WAITP", 			// 2
	"",
	"TEACH_QTEACH_READYDOWNP", 	// 4
	"TEACH_QTEACH_PICKDOWNP", 	// 5 
	"TEACH_QTEACH_PICKP", 			// 6
	"TEACH_QTEACH_MOVEOUTP", 		// 7
	"TEACH_QTEACH_PLACEDOWNP", 	// 8
	"TEACH_QTEACH_PLACEP", 			// 9
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

int StartNo_R; // �C�@�I �}�l Ū �B�J�Ǹ�
int StartNo_W; // �C�@�I �}�l �g �B�J�Ǹ�
int Action_Step; // �ʧ@���Ǹ�
	
int StepNum[20]; // �C�@�I �B�J�ƶq[Group]

int u_SelectClamp=0; // ��Ϊv��ƶq
int SelectClamp[10]={0}; // ��Ϊ��v�㸹�X

int u_SelectDetect=0; // ��Ϊv���˴��ƶq 
int SelectDetect[10]={0}; // ��Ϊ��v���˴����X �v�T �ʧ@�R�O �˴� ��n ���}

int u_StepParamNum=8; // �C�@�B�ѼƼƶq 3+5

/*=================================�оɰʧ@�C��=================================*/
ACTION_P Action_P[]= // �оɰʧ@�C��
{	
// 			QTP,	Step  ACTION_TYPE,	ACTION_NUM, 			P1,	P2, P3, P4, P5,
/*=====================================P2�����I=====================================*/
/* 1*/	{	2,		0, 	Action_Axis,	Axis_Y1,					0,	50,	0,	2,	0, }, //	Y1�b
///* 1*/	{	2,		0,	Action_Axis,	Axis_Y2,					0,	50,	0,	2,	0, }, //	Y2�b (���u)
/* 2*/	{	2,		0,	Action_Permit,Permit_MoldClose,	ON,	0,	0,	2,	0, }, //	���\ �}�� �}
/* 3*/	{	2,		0, 	Action_Axis,	Axis_X1,					0,	50,	0,	2,	0, }, //	X1�b
///* 3*/	{	2,		0, 	Action_Axis,	Axis_X2,					0,	50,	0,	2,	0, }, //	X2�b (���u)
/* 4*/	{	2,		0, 	Action_Axis,	Axis_Z,						0,	50,	0,	6,	0, }, //	Z�b
/* 5*/	{	2,		0, 	Action_Valve,	Valve_AxisC_V,		ON,	ON,	0,	2,	0, }, //	�֪� �D�u���� �} �˴�
/* 6*/	{	2,		0, 	Action_Tag,		START,						0,	0,	0,	0,	0, }, //	�}�l
/* 7*/	{	2,		0, 	Action_Permit,Permit_PushOut,		OFF,0,	0,	2,	0, }, //	���\ ���X ��
/* 8*/	{	2,		0, 	Action_Permit,Permit_PullBack,	OFF,0,	0,	2,	0, }, //	���\ ���h ��
/* 9*/	{	2,		0, 	Action_Wait,	Wait_MoldOpen,		ON,	0,	0,	2,	0, }, //	���� �}�ҧ���
/*=====================================P5�U���I=====================================*/
/*10*/	{	5,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	6,	0, }, //	Y1�b (�U��)
///*10*/	{	5,		0, 	Action_Axis,	Axis_Y2,					0,	80,	0,	6,	0, }, //	Y2�b (�U��)
/*=====================================P6�����I=====================================*/
///**/	{	6,		0, 	Action_Valve,	Valve_Choose1,		ON,	0,	0,	6,	0, }, //	�֪� ��1 �}
///**/	{	6,		0, 	Action_Valve,	Valve_Choose2,		ON,	0,	0,	6,	0, }, //	�֪� ��2 �}
///**/	{	6,		0, 	Action_Valve,	Valve_Choose3,		ON,	0,	0,	6,	0, }, //	�֪� ��3 �}
///**/	{	6,		0, 	Action_Valve,	Valve_Choose4,		ON,	0,	0,	6,	0, }, //	�֪� ��4 �}
/*11*/	{	6,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	6,	0, }, //	X1�b (�e�i)
///*11*/	{	6,		0, 	Action_Axis,	Axis_X2,					0,	80,	0,	6,	0, }, //	X2�b (�e�i)
/*12*/	{	6,		0, 	Action_Permit,Permit_PushOut,		ON,	0,	0,	6,	0, }, //	���\ ���X �}
///*13*/	{	6,		0, 	Action_Wait,	Wait_PushOut,			ON,	0,	0,	6,	0, }, //	���� ���X���� �}
/*14*/	{	6,		0, 	Action_Permit,Permit_PullBack,	ON,	0,	0,	6,	0, }, //	���\ ���h �}
/*=====================================P7��X�I=====================================*/
/*15*/	{	7,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	7,	0, }, //	X1�b (��h)
///*15*/	{	7,		0, 	Action_Axis,	Axis_X2,					0,	80,	0,	7,	0, }, //	X2�b (��h)
/*16*/	{	7,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	7,	0, }, //	Y1�b (�W��)
///*16*/	{	7,		0, 	Action_Axis,	Axis_Y2,					0,	80,	0,	7,	0, }, //	Y2�b (�W��)
///**/	{	7,		0, 	Action_Detect,Valve_Choose1,		ON,	0,	0,	7,	0, }, //	�˴� ��1 ���}
///**/	{	7,		0, 	Action_Detect,Valve_Choose2,		ON,	0,	0,	7,	0, }, //	�˴� ��2 ���}
///**/	{	7,		0, 	Action_Detect,Valve_Choose3,		ON,	0,	0,	7,	0, }, //	�˴� ��3 ���}
///**/	{	7,		0, 	Action_Detect,Valve_Choose4,		ON,	0,	0,	7,	0, }, //	�˴� ��4 ���}
/*17*/	{	7,		0, 	Action_Permit,Permit_MoldClose,	ON,	0,	0,	7,	0, }, //	���\ ���� �}
/*18*/	{	7,		0, 	Action_Permit,Permit_PushOut,		OFF,0,	0,	7,	0, }, //	���\ ���X ��
/*19*/	{	7,		0, 	Action_Valve,	Valve_AxisC_H,		ON,	ON,	0,	7,	0, }, //	�֪� �D�u���� �} �˴�
/*20*/	{	7,		0, 	Action_Permit,Permit_MoldOpen,	ON,	0,	0,	7,	0, }, //	���\ �}�� �}
///**/	{	0,		0, 	����f,				AxisZ,X2,Y2,			0,	0,	0,	0,	0, }, //  ����f
/*=====================================P8�U���I=====================================*/
///**/	{	8,		0, 	Action_Pile,	Pile1,						ON,	0,	0,	8,	0, }, //  ���| ���|1 �}�l 
/*21*/	{	8,		0, 	Action_Axis,	Axis_Z,						0,	80,	0,	9,	0, }, //	Z�b  (��X)
/*22*/	{	8,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	9,	0, }, //	X1�b (��m/���|�I)
/*=====================================P9��m�I=====================================*/
/*23*/	{	9,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	9,	0, }, //	Y1�b (��m/���|�I)
///**/	{	9,		0, 	Action_Pile,	Pile1,						OFF,	0,	0,	9,	0, }, //  ���| ���|1 ����
///**/	{	9,		0, 	Action_Valve,	Valve_Choose1,		OFF,0,	0,	9,	0, }, //	�֪� ��1 ��
///**/	{	9,		0, 	Action_Valve,	Valve_Choose2,		OFF,0,	0,	9,	0, }, //	�֪� ��2 ��
///**/	{	9,		0, 	Action_Valve,	Valve_Choose3,		OFF,0,	0,	9,	0, }, //	�֪� ��3 ��
///**/	{	9,		0, 	Action_Valve,	Valve_Choose4,		OFF,0,	0,	9,	0, }, //	�֪� ��4 ��
/*=====================================P10�^�����I=====================================*/
/*24*/	{	10,		0, 	Action_Axis,	Axis_Y1,					0,	80,	0,	0,	0, }, //	Y1�b
/*25*/	{	10,		0, 	Action_Axis,	Axis_X1,					0,	80,	0,	0,	0, }, //	X1�b
///*25*/	{	10,		0, 	Action_Axis,	Axis_X2,					0,	80,	0,	0,	0, }, //	X2�b (���u)
/*26*/	{	10,		0, 	Action_Axis,	Axis_Z,						0,	80,	0,	0,	0, }, //	Z�b
/*27*/	{	10,		0, 	Action_Valve,	Valve_AxisC_V,		ON,	ON,	0,	0,	0, }, //	�֪� �D�u���� �} �˴�
/*28*/	{	10,		0, 	Action_Goto,		END,						0,	0,	0,	0,	0, }, //	����
};

ACTION_P Action_Axis_P[]= // �оɰʧ@ �b�ʧ@
{
	{	0,	0,	Action_Axis,	0,								0,	0,	0,	0,	0, }, //	0�b
	{	0,	0, 	Action_Axis,	Axis_X1,					0,	0,	0,	0,	0, }, //	X1�b
	{	0,	0, 	Action_Axis,	Axis_Y1,					0,	0,	0,	0,	0, }, //	Y1�b
	{	0,	0, 	Action_Axis,	Axis_Z ,					0,	0,	0,	0,	0, }, //	Z�b
	{	0,	0, 	Action_Axis,	Axis_X2,					0,	0,	0,	0,	0, }, //	X2�b
	{	0,	0, 	Action_Axis,	Axis_Y2,					0,	0,	0,	0,	0, }, //	Y2�b
};

ACTION_P Action_Valve_P[]= // �оɰʧ@ �֪�
{
	{	0,	0,	Action_Valve,	Valve_Choose1,		ON,	0,	0,	0,	0, }, //	�֪� ��1 �}
	{	0,	0,	Action_Valve,	Valve_Choose2,		ON,	0,	0,	0,	0, }, //	�֪� ��2 �}
	{	0,	0,	Action_Valve,	Valve_Choose3,		ON,	0,	0,	0,	0, }, //	�֪� ��3 �}
	{	0,	0,	Action_Valve,	Valve_Choose4,		ON,	0,	0,	0,	0, }, //	�֪� ��4 �}
};
ACTION_P Action_Detect_P[]= // �оɰʧ@ �˴�
{
	{	0,	0,	Action_Detect,Valve_Choose1,		ON,	0,	0,	0,	0, }, //	�˴� ��1 ���}
	{	0,	0,	Action_Detect,Valve_Choose2,		ON,	0,	0,	0,	0, }, //	�˴� ��2 ���}
	{	0,	0,	Action_Detect,Valve_Choose3,		ON,	0,	0,	0,	0, }, //	�˴� ��3 ���}
	{	0,	0,	Action_Detect,Valve_Choose4,		ON,	0,	0,	0,	0, }, //	�˴� ��4 ���}
};

ACTION_P Action_Wait_P[]= // �оɰʧ@ ����
{
	{	0,	0,	Action_Wait,	0,								ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_IMEStop,			ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_MoldOpen,		ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_MoldClose,		ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_SDoorClose,	ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_IMAuto,			ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_PushOut,			ON,	0,	0,	0,	0, }, //	����
	{	0,	0,	Action_Wait,	Wait_PullBack,		ON,	0,	0,	0,	0, }, //	����
};


ACTION_P Action_Pile_P[]= // �оɰʧ@ ���|
{
	{	0,	0,	Action_Pile,						0,		ON,	0,	0,	0,	0, }, //  ���| ���|0 �}�l
	{	0,	0,	Action_Pile,						1,		ON,	0,	0,	0,	0, }, //  ���| ���|1 �}�l
	{	0,	0,	Action_Pile,						2,		ON,	0,	0,	0,	0, }, //  ���| ���|2 �}�l
	{	0,	0,	Action_Pile,						3,		ON,	0,	0,	0,	0, }, //  ���| ���|3 �}�l
	{	0,	0,	Action_Pile,						4,		ON,	0,	0,	0,	0, }, //  ���| ���|4 �}�l
};


/*=================================�оɰʧ@�C��=================================*/

int Action_P_ListNum = ( sizeof(Action_P) / sizeof(Action_P[0]) ); // �оɰʧ@�C��ƶq

/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
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
	char	m_pszTitleID[256];	memset(m_pszTitleID, 0, sizeof(m_pszTitleID));
	pwndSender->GetPropValueT("titleID", m_pszTitleID, sizeof(m_pszTitleID));
	printf("Now at %s\n",m_pszTitleID);

	// ���o�s�ո��X (��m�N�X)
	for(int i=0; i<sizeof(GroupName)/sizeof(GroupName[0]); i++)
	{
		if(strcmp(m_pszTitleID, GroupName[i]) == 0)
			u_Group=i;
	}
	printf("Group=%d\n",u_Group);
	
	// ���o ���c�]�w 3 or 5�b
	//u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // Ū���]�w ������� �T���b
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);// Ū���]�w ������� �T���b
	u_PickerType = dw_MechType & MechWord;
	
	for(int i=0;i<7;i++)
	{
		printf("%s=%d\n",P6_Clamp_DT_DBString[i],(GetDBValue(P6_Clamp_DT_DBString[i]).lValue));
	}
	
	
	
	printf("u_PickerType=%d\n",u_PickerType);
	if(u_PickerType==MechType5)
		u_SubArmUse_WaitP 		 = 2; // ���u �W�[�B�� �����I
	if(u_PickerType==MechType3)
		SetDBValue(Check_SubUse_DB,FALSE);	
	
	// ���o���� �Ҧ��]�w �v����
	for(int i = 0; i < sizeof(u_pszClamp_SelectString)/sizeof(u_pszClamp_SelectString[0]); i++ )
	{
		pwndClamp_Select[i] = pwndSender->FindControlFromName(u_pszClamp_SelectString[i]); // �v����
		pwndCheckBox[i] = pwndSender->FindControlFromName(u_pszCheckBoxString[i]); // �v���� �Ĥ�
		pwndDetect_Select[i] = pwndSender->FindControlFromName(u_pszDetect_SelectString[i]); // �v���˴���� (�ʧ@�R�O)
		pwndDetectBox[i] = pwndSender->FindControlFromName(u_pszDetectBoxString[i]);	// �v��ߴ� �Ĥ� (���I)
		pwndQTeach_Clamp_DT[i] = pwndSender->FindControlFromName(u_pszQTeach_Clamp_DTString[i]);// �v�㩵��
	} 
	// ���o���� �Ҧ��]�w �v���� �B�n
	pwndClamp_Mask	= pwndSender->FindControlFromName("Clamp_Mask");
	
	pwndStr_Clamp	= pwndSender->FindControlFromName("Str_Clamp");
	pwndStr_Suck	= pwndSender->FindControlFromName("Str_Suck");
	pwndStr_Clamp_Detect1	= pwndSender->FindControlFromName("Str_Clamp_Detect1");
	pwndStr_Clamp_DT1	= pwndSender->FindControlFromName("Str_Clamp_DT1");
	pwndStr_Clamp_Detect2	= pwndSender->FindControlFromName("Str_Clamp_Detect2");
	pwndStr_Clamp_DT2	= pwndSender->FindControlFromName("Str_Clamp_DT2");

	
	// ���o���� �Ҧ��]�w �v���˴���� �B�n  
	pwndDetect_Mask1	= pwndSender->FindControlFromName("Detect_Mask1");
	pwndDetect_Mask2	= pwndSender->FindControlFromName("Detect_Mask2");
	// ���o���� �Ҧ��]�w �v���˴���� ��r  
	pwndStr_Detect1	= pwndSender->FindControlFromName("Str_Detect1");
	pwndStr_Detect2	= pwndSender->FindControlFromName("Str_Detect2");
	


	// ���o���� �y�Ц�m�]�w
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		pwndQTeach_Pos[i] = pwndSender->FindControlFromName(u_pszQTeach_PosString[i]);
	} 
	// ���o�y�Ц�m �W�U����ܤ�r
	for(int i = 0; i < sizeof(u_pszQTeach_Pos_MaxString)/sizeof(u_pszQTeach_Pos_MaxString[0]); i++ )
	{
		pwndQTeach_Pos_Max[i] = pwndSender->FindControlFromName(u_pszQTeach_Pos_MaxString[i]);
	} 	
	for(int i = 0; i < sizeof(u_pszQTeach_Pos_MinString)/sizeof(u_pszQTeach_Pos_MinString[0]); i++ )
	{
		pwndQTeach_Pos_Min[i] = pwndSender->FindControlFromName(u_pszQTeach_Pos_MinString[i]);
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
		
	// ���o���� ���A
	pwndBtn_PosHV = pwndSender->FindControlFromName("Btn_PosHV");
	
	// ���o���� ��m���� �Ϥ�
	for(int i = 0; i < sizeof(Bar_PosHint_String)/sizeof(Bar_PosHint_String[0]); i++ )
	{
		pwndBar_PosHint[i] = pwndSender->FindControlFromName(Bar_PosHint_String[i]);
	}
	// ���o���� ��m���� �Ϥ�
	for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
	{
		pwndImg_PosHint[i] = pwndSender->FindControlFromName(Img_PosHint_String[i]);
	}
	// ���o���� ��m���� �B�n
	for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
	{
		pwndMask_PosHint[i] = pwndSender->FindControlFromName(Mask_PosHint_String[i]);
	}
	
	// ���o���� �v�� ON/OFF Btn
	for(int i = 0; i < sizeof(u_pszBtn_Clamp_String)/sizeof(u_pszBtn_Clamp_String[0]); i++ )
	{
		pwndBtn_Clamp[i] = pwndSender->FindControlFromName(u_pszBtn_Clamp_String[i]);
	} 

	// ���o���� �����m���s
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ )
	{
		pwndBtn_GetPos[i] = pwndSender->FindControlFromName(GetPos_String[i]);
	} 

	
	// ���o���� �{�b���ʶb ��� �Ϥ�
	pwndImg_NowMoveAxis	= pwndSender->FindControlFromName("Img_NowMoveAxis");	
	// ���o���� �{�b���ʶb ��� ��r
	pwndStr_NowMoveAxis	= pwndSender->FindControlFromName("Str_NowMoveAxis");	
	
	// ���o���� �}�l�оɫ��s
	pwndBtnTeachStart	= pwndSender->FindControlFromName("BtnTeachStart");	
	// ���o���� �U�@��m���s
	pwndBtnNextP	= pwndSender->FindControlFromName("BtnNextP");	
	// ���o���� �v���Τ�r
	pwndStr_ClampSelect	= pwndSender->FindControlFromName("Str_ClampSelect");	
	
	// ���o���� ���ʫ�U�� �Ĥ�
	pwndCheck_Move	= pwndSender->FindControlFromName("Check_Move");	
	// ���o���� ���ʫ�U�� btn
	pwndBtn_Check_Move	= pwndSender->FindControlFromName("Btn_Check_Move");
	b_Check_Move = GetDBValue(Check_Move_DB).lValue;
	if(b_Check_Move)
		{
			if(u_PickerType==MechType5)
				u_Check_Move 		 = 2; // �h2�B�J
			else if(u_PickerType==MechType3)
				u_Check_Move 		 = 1; // �h1�B�J				
		}
	if(b_Check_Move==0 && u_Group==ReadyDownP)
		b_All_PosSet_OK=OK; // ��m�]�w����
		
	// ���o���� ���u�ϥ� �Ĥ�
	pwndCheck_SubUse	= pwndSender->FindControlFromName("Check_SubUse");	
	b_SubArmUse = GetDBValue(Check_SubUse_DB).lValue;	
	if(b_SubArmUse)
		{
			u_SubArmUse_PickDownP  = 1; // ���u �W�[�B�� �U���I
			u_SubArmUse_PickP			 = 1; // ���u �W�[�B�� �����I
			u_SubArmUse_MoveOutP	 = 2; // ���u �W�[�B�� ��X�I
			u_SubArmUse_SubPlaceP  = 6; // ���u �W�[�B�� ��X�I (����f)
		}
	// ���o���� ���u�P�D�u�P�� �Ĥ�
	pwndCheck_SubSync	= pwndSender->FindControlFromName("Check_SubSync");	
	b_SubArmSync = GetDBValue(Check_SubSync_DB).lValue;	
	// ���o���� ���u�ϥ� Btn
	pwndBtn_SubUse	= pwndSender->FindControlFromName("Btn_SubUse");
	// ���o���� ���u�P�D�u�P�� Btn
	pwndBtn_SubSync	= pwndSender->FindControlFromName("Btn_SubSync");
	// ���o���� ���u �B�n
	pwndMask_SubArm	= pwndSender->FindControlFromName("Mask_SubArm");
	 
	// ���o���� �X�ʨϥ� �Ĥ�
	pwndCheck_SLVL	= pwndSender->FindControlFromName("Check_SLVL");	
	// ���o���� �X�ʨϥ� btn
	pwndBtn_SLVL	= pwndSender->FindControlFromName("Btn_SLVL");	
	u_Check_SLVL = GetDBValue(Check_SLVL_DB).lValue; // ���odb�ƭ�
	b_Check_SLVL = (u_Check_SLVL>>u_Group) & 1; // �T�{��u_Group bit �O�_��1
	//printf("u_Check_SLVL=%x, b_Check_SLVL=%d\n",u_Check_SLVL,b_Check_SLVL);

	// ���o���� ����X��/���X�X�� �ƭȳ]�w 
	pwndPick_SLVL 	 = pwndSender->FindControlFromName("Pick_SLVL");
	pwndPickOut_SLVL = pwndSender->FindControlFromName("PickOut_SLVL");
	// ���o���� �U���Z��/��h�Z�� �ƭ���� 
	pwndPick_Dis 	 = pwndSender->FindControlFromName("Pick_Dis");
	pwndPickOut_Dis = pwndSender->FindControlFromName("PickOut_Dis");
	// ���o���� �X�ʨϥ� Logo
	pwndSLVL_Logo = pwndSender->FindControlFromName("SLVL_Logo");	
	// ���o���� �X�ʳ]�m �B�n
	pwndMask_SLVL = pwndSender->FindControlFromName("Mask_SLVL");	
	
	
	// ���o���� ���ʫ�U�� ��ܹϤ�
	pwndImg_QTeach_ReadyDownP	= pwndSender->FindControlFromName("Img_QTeach_ReadyDownP");
	
	// ���o���� �˴� ���h����(�}�l���X) ��ܤĤ�
	pwndCheck_Ejector 	 = pwndSender->FindControlFromName("Check_Ejector");
	u_Check_Ejector = GetDBValue(Check_Ejector_DB).lValue; // ���odb�ƭ�
	b_Check_Ejector = u_Check_Ejector & 1; // �T�{��u_Group bit �O�_��1
	printf("u_Check_Ejector=%x, b_Check_Ejector=%d\n",u_Check_Ejector,b_Check_Ejector);
	
	// ���o���� ���|���
	pwndCheck_Pile = pwndSender->FindControlFromName("Check_Pile"); // ���|���
	pwndBtn_PileUse	= pwndSender->FindControlFromName("Btn_PileUse"); // ���|��� Btn 
	// ���o���� ���|�ռ� ���
	for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ )
	{
		pwndBtn_PileNum[i] = pwndSender->FindControlFromName(PileNum_String[i]); // ���|�ռ�
	} 
	// ���o���� ���|�ռ� ��ܶs
	for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ )
	{
		pwndBtn_PileChoose[i] = pwndSender->FindControlFromName(PileChoose_String[i]); // ���|�ռ� ��ܶs
	} 
	// ���o���� ���|�ռ� �B�n
	pwndMask_Pile = pwndSender->FindControlFromName("Mask_Pile"); // ���|�ռ� �B�n
	
	PileNum  = GetDBValue(Check_Pile_DB).lValue;
	printf("PileNum=%d\n",PileNum);
	if(PileNum>0)
		u_PileNum=1; // �h�@�B�J
	
	// ��m/���| ��r����
	pwndStr_Place_Z = pwndSender->FindControlFromName("Str_Place_Z"); // ��mZ
	pwndStr_Place_X = pwndSender->FindControlFromName("Str_Place_X"); // ��mX
	
	
	int All_PosSet_OK = GetDBValue(pAll_PosSet_OK_DB).lValue; // ��^�e�@�� ���ݦA���T�{�]�w��
	if(All_PosSet_OK)
		b_All_PosSet_OK=OK;
		
	if(u_Group==PickDownP)
		{
			GetDownDis();	
			ShowData(pwndPick_SLVL); // ��� �]�w�ƭ�
		}
	if(u_Group==MoveOutP)
		{
			GetBackDis();
			ShowData(pwndPickOut_SLVL); // ��� �]�w�ƭ�
		}
		
	/*=====================================���init()==================================*/	
	// Update ���
	UpdateShow();
	
	// �p��U�I�B�J
	printf("Action_P[] = %d\n",Action_P_ListNum);
	for(int Group=0;Group<20;Group++)
	{
		StepNum[Group]=0;
		for(int i=0;i<Action_P_ListNum;i++)
		{
			if(Action_P[i].QTP == Group)
				{
					printf("Action_P[%d].QTP=%d\n",i,Action_P[i].QTP);
					StepNum[Group]++;
				}	
		}
		printf("StepNum[%d]=%d\n",Group,StepNum[Group]);
	}

	if(u_Group==2)
		{
			int getvalue = GetDBValue(P2_POSSET_DBString[Axis_Y2]).lValue;
			printf("OnCreateA %s=%d\n",P2_POSSET_DBString[Axis_Y2],getvalue);
		}
	
	// �]�w�t�׹w�]�� 80
	long ltemp=0;
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		if(pwndQTeach_Speed[i]!=NULL)
		{
			pwndQTeach_Speed[i]->GetPropValueT("value", &ltemp,sizeof(ltemp));
			if(ltemp==0)
			{
				pwndQTeach_Speed[i]->SetPropValueT("value",80); // �g�J�ƭ� 80
				pwndQTeach_Speed[i]->Update();
			}
		}
	}
	return TRUE;
}
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

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
			case 0:
				b_SpdSet_OK[Axis_Z]=OK;
				break;
			case 1:
				b_SpdSet_OK[Axis_X1]=OK;
				break;
			case 2:
				b_SpdSet_OK[Axis_Y1]=OK;
				break;
			case 3:
				b_SpdSet_OK[Axis_X2]=OK;
				break;
			case 4:
				b_SpdSet_OK[Axis_Y2]=OK;
				break;
			case 5:
				b_PosSet_OK[Axis_Z]=OK;
				break;
			case 6:
				b_PosSet_OK[Axis_X1]=OK;
				if(u_Group==MoveOutP)
					{
						int value=0;
						pwndQTeach_Pos[Axis_X1]->GetPropValueT("value", &value,sizeof(value));
						SetDBValue(P7_POSSET_DBString[Axis_X1], value); // �]�w��m
						if(b_Check_SLVL)
							GetBackDis();
					}
				break;
			case 7:
				b_PosSet_OK[Axis_Y1]=OK;
				if(u_Group==PickDownP)
					{
						int value=0;
						pwndQTeach_Pos[Axis_Y1]->GetPropValueT("value", &value,sizeof(value));
						SetDBValue(P5_POSSET_DBString[Axis_Y1], value); // �]�w��m
						if(b_Check_SLVL)
							GetDownDis();
					}
				break;
			case 8:
				b_PosSet_OK[Axis_X2]=OK;
				break;
			case 9:
				b_PosSet_OK[Axis_Y2]=OK;
				break;
			default:
				break;
		}
		UpdateHint();
	}
	return wIDControl;
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
	if(u_Group < wKey) // ���U�@��m �~�|�x�s
	{
		b_SaveFlag = TRUE;
		SetDBValue(pAll_PosSet_OK_DB, 0); // �~�|���� �]�w��m
	}
	else 
	{
		b_SaveFlag = FALSE;
		SetDBValue(pAll_PosSet_OK_DB, 1);
	}
	switch(wKey)
	{
		case 1:
			::PutCommand("QTeach_Mode.txt");
			break;
		/*=================================WaitP===================================*/
		case WaitP:
			::PutCommand(QTeach_P_Page[u_PickerType][WaitP]);
			break;
		/*===============================ReadyDownP===============================*/
		case ReadyDownP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
				::PutCommand(QTeach_P_Page[u_PickerType][ReadyDownP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			break;
		/*===============================PickDownP===============================*/
		case PickDownP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
				::PutCommand(QTeach_P_Page[u_PickerType][PickDownP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			break;
		/*===============================PickP===============================*/
		case PickP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
				::PutCommand(QTeach_P_Page[u_PickerType][PickP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			break;
		/*===============================MoveOutP===============================*/
		case MoveOutP:
			if( (u_Group>MoveOutP) && (b_SubArmUse==1) )
				::PutCommand("QTeach_SubPlaceP.txt");
			else
			{
				if( (u_Group > wKey) || b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
					::PutCommand(QTeach_P_Page[u_PickerType][MoveOutP]);
				else
					MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			}
			break;
		/*===============================PlaceDownP===============================*/
		case PlaceDownP:
			if( (u_Group<PlaceDownP) && (b_SubArmUse==1) )
				::PutCommand("QTeach_SubPlaceP.txt");
			else
			{
				if( (u_Group > wKey) || b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
					::PutCommand(QTeach_P_Page[u_PickerType][PlaceDownP]);
				else
					MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			}
			break;
		/*===============================PlaceP===============================*/
		case PlaceP:
			if( (u_Group > wKey) || b_BtnNextP_OK) // ���W�@�� �άO �]�w�������U�@��
				::PutCommand(QTeach_P_Page[u_PickerType][PlaceP]);
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			break;
		case 10: // ����
			if(b_BtnNextP_OK) // �]�w�������U�@��
			{
				if(PileNum>0) //�Y����ܰ��|
					{
						g_Pile_Select = PileNum; // ���| ��n��
						::PutCommand(PilePage[PileNum]); // ����ܰ��|��n�� �ѼƳ]�w��
					}
				else
					{
						::PutCommand("QTeach_Done.txt"); // ����� �����оɭ�
					}
			}
			else
				MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // ���ܪ�^
			break;
		case 98: // �T�{�Ҧ�
			Clean_QTeach();// �M���ֳt�оɵ{��
			SaveMode();
			::PutCommand(QTeach_P_Page[u_PickerType][WaitP]);
			break;
		case 99: // �����о�
			::PutCommand("Index.txt");
			break;
		case 7178: // �]�w�Ʀr�᪺�T�{��
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
	printf("OnMouseUp\n");	
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;  
 		
 	for(int i=0; i<10; i++) // �v���� Btn���U�u�_
 	{
	 	if((pwnd == pwndClamp_Select[i])||(pwnd == pwndCheckBox[i])) // �v����
	 	{
			b_Select[i] = !b_Select[i];
			pwndCheckBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Select[i]]);
			pwndCheckBox[i]->CreateA();
			pwndCheckBox[i]->Update();
			UpdateBtnTeachStart();
			b_DetectSelect[i]=0;
			UpdateBtnDetect_Select(); // ��s �v�� �˴����
	 	}
	 	//printf("b_Select[%d]=%d\n",i,b_Select[i]);
	 	if(pwnd == pwndDetect_Select[i])
 		{		
 			b_DetectSelect[i] = !b_DetectSelect[i];
			pwndDetect_Select[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_DetectSelect[i]]);
			pwndDetect_Select[i]->CreateA();
			pwndDetect_Select[i]->Update();
 		}
 	} 
 	 	
	if(pwnd == pwndBtn_PosHV) // ���A Btn���U�u�_
	{
		printf("Btn_PosHV\n");
		u_status++;
		if(u_status>2)
			u_status=1;
		printf("u_status=%d\n",u_status);
		switch(u_Group)
		{
			case WaitP: //�����I
				SetDBValue(PosStatus_DBString[WaitP],u_status); // �����I�����A
				SetDBValue(PosStatus_DBString[ReadyDownP],u_status+2); // �ǳƤU���I�����A
				SetDBValue(PosStatus_DBString[PickDownP],u_status+2); // ���󰼤U���I�����A
				SetDBValue(PosStatus_DBString[PickP],u_status+2); // �����I�����A
				break;// ���A
			case MoveOutP: // ��X�I
				SetDBValue(PosStatus_DBString[MoveOutP],u_status); // ��X�I�����A
				SetDBValue(PosStatus_DBString[PlaceDownP],u_status); // �m�����U���I�����A
				break;
			case PlaceP: //��m�I
				SetDBValue(PosStatus_DBString[PlaceP],u_status); // ��m�I�����A
				break;
			default:
				break;
		}
		if(pwndBtn_PosHV != NULL)
		{
		 pwndBtn_PosHV->SetPropValueT("upbitmap",PosHV_ImgPath[u_status]);
		 pwndBtn_PosHV->CreateA();
		 pwndBtn_PosHV->Update();
		}
	}
	
	for(int i=0;i<10;i++) // �v�� Btn���U�u�_
	{
		if(pwnd == pwndBtn_Clamp[i])
		{
			u_clamp_onoff[i]++;
			if(u_clamp_onoff[i]>1)
				u_clamp_onoff[i]=0;
			printf("%s=%d\n",u_pszBtn_Clamp_String[i],u_clamp_onoff[i]);
			switch(u_Group) // ���� ON/OFF���A
			{
				case 6: // �����I
					SetDBValue(P6_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					printf("Set %s = %d\n",P6_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					break;
				case 9: // ��m�I
					SetDBValue(P9_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					printf("Set %s = %d\n",P6_Clamp_onoff_DBString[i],u_clamp_onoff[i]);
					break;
				default:
					break;
			}
			if(pwndBtn_Clamp[i] != NULL) // ��� ����1 ON/OFF���A
			{
				pwndBtn_Clamp[i]->SetPropValueT("upbitmap",Clamp_ImgPath[(u_clamp_onoff[i]*10+i)]);
			 pwndBtn_Clamp[i]->CreateA();
			 pwndBtn_Clamp[i]->Update();
			}
			b_ClampSet_OK=OK;
			UpdateBtnNextP(); // ��s�������
		}		
	}
	
 	for(int i=0; i<10; i++) // �v���˴� Btn���U�u�_
 	{
		if(pwnd == pwndDetectBox[i]) // �v���˴����
		{
			u_detect_onoff[i]++;
			if(u_detect_onoff[i]>1)
				u_detect_onoff[i]=0;
			printf("%s=%d\n",u_pszDetectBoxString[i],u_detect_onoff[i]);
			switch(u_Group) // ���� ON/OFF���A
			{
				case 6: // �����I
					SetDBValue(P6_Clamp_detect_DBString[i],u_detect_onoff[i]);
					printf("Set %s = %d\n",P6_Clamp_detect_DBString[i],u_detect_onoff[i]);
					break;
				case 9: // ��m�I
					SetDBValue(P9_Clamp_detect_DBString[i],u_detect_onoff[i]);
					printf("Set %s = %d\n",P9_Clamp_detect_DBString[i],u_detect_onoff[i]);
					break;
				default:
					break;
			}
			if(pwndDetectBox[i] != NULL) // ��� ����1 ON/OFF���A
			{
			 pwndDetectBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[u_detect_onoff[i]]);
			 pwndDetectBox[i]->CreateA();
			 pwndDetectBox[i]->Update();
			}
		}	
	 	
 	}
	
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ ) // �����m Btn���U�u�_
	{
		if(pwnd == pwndBtn_GetPos[i])
		{
			GetPosNow(i);
			b_PosSet_OK[i]=OK;
			UpdateHint();
			
			if(u_Group==PickDownP)
				{
					int value=0;
					pwndQTeach_Pos[Axis_Y1]->GetPropValueT("value", &value,sizeof(value));
					SetDBValue(P5_POSSET_DBString[Axis_Y1], value); // �]�w��m
					if(b_Check_SLVL)
						GetDownDis();
				}
			if(u_Group==MoveOutP)
				{
					int value=0;
					pwndQTeach_Pos[Axis_X1]->GetPropValueT("value", &value,sizeof(value));
					SetDBValue(P7_POSSET_DBString[Axis_X1], value); // �]�w��m
					if(b_Check_SLVL)
						GetBackDis();
				}
		}
	} 
	
	if( (pwnd == pwndCheck_Move) || (pwnd == pwndBtn_Check_Move) ) // �e�i��U�� �Ĥ� || Btn ���U�u�_
	{
		b_Check_Move = !b_Check_Move;
		SetDBValue(Check_Move_DB,b_Check_Move);
		pwndCheck_Move->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Move]);
		pwndCheck_Move->CreateA();
		pwndCheck_Move->Update();
//		if(b_Check_Move)
//		{
//			b_All_PosSet_OK=0; // �ݳ]�w��m
//			UnLockBtn(pwndQTeach_Pos[Axis_X1]);
//			UnLockBtn(pwndQTeach_Speed[Axis_X1]);
//			UnLockBtn(pwndQTeach_DT[Axis_X1]);
//			UnLockBtn(pwndBtn_GetPos[Axis_X1]);
//			pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_ReadyDownP.bmp");
//			pwndImg_QTeach_ReadyDownP->Show();
//		}
//		else
//		{
//			b_All_PosSet_OK=OK; // ��m�]�w����
//			LockBtn(pwndQTeach_Pos[Axis_X1]);
//			LockBtn(pwndQTeach_Speed[Axis_X1]);
//			LockBtn(pwndQTeach_DT[Axis_X1]);
//			LockBtn(pwndBtn_GetPos[Axis_X1]);
//			pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_WaitP.bmp");
//			pwndImg_QTeach_ReadyDownP->Show();
//			SetFromWaitP(Axis_X1);// ���� WaitP X1���]�w
//		}
		UpdateHint();
		UpdateBtnNextP();
	}
	
	if( (pwnd == pwndCheck_SubUse) || (pwnd == pwndBtn_SubUse) ) // ���u�ϥ� Btn���U�u�_
	{
		b_SubArmUse = !b_SubArmUse;
		SetDBValue(Check_SubUse_DB,b_SubArmUse);
		if(!b_SubArmUse) // ����� ���u�ϥ� �� ���u�P�ʦ۰ʨ���
			{
				b_SubArmSync=FALSE;
				SetDBValue(Check_SubSync_DB,b_SubArmSync);
			}
		UpdateSubArm();
	}
	if(pwnd == pwndCheck_SubSync || (pwnd == pwndBtn_SubSync) ) // ���u�P�D�u�P�� Btn���U�u�_
	{
		b_SubArmSync = !b_SubArmSync;
		SetDBValue(Check_SubSync_DB,b_SubArmSync);
		UpdateSubArm();
	}
	
	if( (pwnd == pwndCheck_SLVL) || (pwnd == pwndBtn_SLVL) ) // �X�ʨϥ� �Ĥ� || Btn ���U�u�_
	{
		b_Check_SLVL = !b_Check_SLVL;
		
		u_Check_SLVL = u_Check_SLVL & ~(1<<u_Group); // �M�Ÿ�bit
		if(b_Check_SLVL)
			{
				u_Check_SLVL = u_Check_SLVL | (1<<u_Group); // �]�w �� bit �� b_Check_SLVL
			}
		//printf("u_Check_SLVL=%x\n",u_Check_SLVL);
		
		SetDBValue(Check_SLVL_DB,u_Check_SLVL);
		
		pwndCheck_SLVL->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_SLVL]);
		pwndCheck_SLVL->CreateA();
		pwndCheck_SLVL->Update();
		if(b_Check_SLVL)
		{
			if(u_Group==PickDownP)
			{
				GetDownDis();
				UnLockBtn(pwndPick_SLVL); // ���� ����X�� �ƭȳ]�w
			}
			if(u_Group==MoveOutP)
			{
				GetBackDis();
				UnLockBtn(pwndPickOut_SLVL); // ���� ���X�X�� �ƭȳ]�w
			}
		}
		else
		{
			LockBtn(pwndPick_SLVL); // �W�� ����X�� �ƭȳ]�w
			LockBtn(pwndPickOut_SLVL); // �W�� ���X�X�� �ƭȳ]�w
		}
		Update_SLVL();
	}
	
	if(pwnd ==pwndCheck_Ejector) // �Ĥ� �˴� ���h����(�}�l���X)
	{
		b_Check_Ejector = !b_Check_Ejector;
		u_Check_Ejector = b_Check_Ejector; // �]�w b_Check_Ejector
		SetDBValue(Check_Ejector_DB,u_Check_Ejector);
		printf("u_Check_Ejector=%d\n",u_Check_Ejector);

		pwndCheck_Ejector->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Ejector]);
		pwndCheck_Ejector->CreateA();
		pwndCheck_Ejector->Update();
	}
		

	if( (pwnd == pwndCheck_Pile) || (pwnd == pwndBtn_PileUse) ) // ���|�Ĥ� || ���|��� Btn���U�u�_
	{
		if(PileNum==0)
			PileNum=1;
		else
			PileNum=0;
		SetDBValue(Check_Pile_DB, PileNum); // �g�J DB
		UpdateCheck_Pile();
	}
	for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ )
	{
		if( (pwnd == pwndBtn_PileNum[i]) || (pwnd ==pwndBtn_PileChoose[i])) // ���|�ռ� || ���|�ռƿ�ܶs �Q���
		{
			PileNum=i;
			SetDBValue(Check_Pile_DB, PileNum); // �g�J DB
			UpdateCheck_Pile();
		}
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
	if(!b_RunOnlyOne)
		{
			UpdateSubArm();
			UpdateCheck_Pile();
			UpdateClamp();
			UpdateBtnDetect_Select();
			Update_ClampStr();
			Update_SLVL();
			for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
			{
				Update_PosHint(i);
			}
			
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43",0x0000);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44",0x0000);// �ʴ����ʶb �M0
			
			WhichAxis_Old = WhichAxis;
			
			((CtmFormView*)pwndSender)->OnLoseFocus(); // ��������
			
			b_RunOnlyOne = TRUE;// ���� ����@��
		}
		
	WhichAxis = GetDBValue(pWhichAxisDB).lValue; // �������@�b�Q���U
	
	if(WhichAxis<6 && WhichAxis!=WhichAxis_Old && Manual_Type != MANUAL_STOP_MODE)
	{
		b_PosSet_OK[WhichAxis]=0; // ��m�ݭ��s�T�{
		UpdateTeach_Pos(); // ��s ��m�]�w
		Update_AxisMoveNow(WhichAxis); // ��s �{�b���ʶb �Ϥ�
		//MsgBoxCall("MsgBox_QTeachAxisSet.txt"); // ��� ���ʶb����
		WhichAxis_Old =WhichAxis;
	}	
	else if(WhichAxis>=6 && WhichAxis!=WhichAxis_Old) // ���s��}
	{
		Update_AxisMoveNow(0); // ��s �{�b���ʶb �Ϥ� ��ܵL
		WhichAxis_Old =WhichAxis;
	}
	
	
	for(int i = 0; i < sizeof(Img_PosHint_String)/sizeof(Img_PosHint_String[0]); i++ )
	{
		AxisPosNow[i] = GetDBValue(AxisPosNow_DBString[i]).lValue; // ���o�ƭ�
		if(AxisPosNow_Old[i]!=AxisPosNow[i])
		{
			Update_PosHint(i);
			AxisPosNow_Old[i] = AxisPosNow[i];
		}
	}
	
}
/*---------------------------------------------------------------------------+
|  Function : OnMessage()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
	{
		case	MSG_GPIO_READ:
		{
			printf("MSG_GPIO_READ\n");
			// �I���w�����s
			{
			char gpio_safekey;
			gpio_safekey = wParam&GPIO_SAFEKEY_BIT;
			printf("gpio_safekey = %x\n",gpio_safekey);
			if(gpio_safekey==GPIO_SAFEKEY_0) // ��} ����
				{
					//printf("ACT=STOP\n");
					Manual_Type = MANUAL_STOP_MODE;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_1) // 1�q �I��
				{
					//printf("ACT=JOG\n");
					Manual_Type = MANUAL_JOG_MODE;
					WhichAxis_Old=WhichAxis;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_2) // 2�q �s���
				{
					//printf("ACT=CONTINUE\n");
					Manual_Type = MANUAL_CONTINUE_MODE;
					WhichAxis_Old=WhichAxis;
				}
			}
		}
		break;
		case	MSG_PANEL_KEY: //hotkey
		{
			
		}
		break;
		default:
			break;	
	}	
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
void    OnDestroyA(CtmWnd* pwndSender)
{ 
	printf("OnDestroyA %d\n",u_Group);
	printf("b_SaveFlag= %d\n",b_SaveFlag);
	int StartNum=0;
	for(int i=0;i<u_Group;i++)
	{
		StartNum=StartNum+StepNum[i];
	}
	StartNo_R=(1+StartNum); // �q Action_P[StartNo_R] �}�lŪ
	StartNo_W=(1+StartNum); // �q MACHINE_PROFILE_NUM[StartNo_W ]_EQUIPMENT1_ACTION_, �}�l�g
	switch(u_Group)
	{
		case 1:
			
			break;
		case WaitP: // ���}P2
			if(b_SaveFlag)
				Save();
			break;
		case ReadyDownP:
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP); // �W�[�e�@�B���W�[���B�J��
			Action_Step = StartNo_W-1;
			if(b_SaveFlag)
				Save();
			break;
		case PickDownP:
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move); // +�q�Y����W�[�� �� �e�@�B�W�[�� 
			Action_Step = StartNo_W-1;
			if(b_SaveFlag)
				Save();
			break;
		case PickP: // ���}P6
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP); // +�q�Y����W�[�� �� �e�@�B�W�[�� 
			Action_Step = StartNo_W-1 - b_SubArmSync;
			if(b_SaveFlag)
				Save();
			break;
		case MoveOutP: // ���}P7
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP + u_SubArmUse_PickP
			+ u_SelectClamp + b_Check_Ejector + b_SubArmUse/*���u�����*/);  // +�q�Y����W�[�� �� �e�@�B�W�[�� 
			Action_Step = StartNo_W-1 - b_SubArmSync - b_SubArmSync;
			if(b_SaveFlag)
				Save();
			break;
		case PlaceDownP: // ���}P8
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP + u_SubArmUse_PickP + u_SubArmUse_MoveOutP + u_SubArmUse_SubPlaceP
			+ u_SelectClamp + b_Check_Ejector + u_SelectDetect + b_SubArmUse/*���u�����*/); // +�q�Y����W�[�� �� �e�@�B�W�[�� 
			Action_Step = StartNo_W-1 - b_SubArmSync - b_SubArmSync - b_SubArmSync*2;
			if(b_SaveFlag)
				Save();
			break;
		case PlaceP: // ���}P9
			StartNo_W=(1+StartNum + u_SubArmUse_WaitP + u_Check_Move + u_SubArmUse_PickDownP + u_SubArmUse_PickP + u_SubArmUse_MoveOutP + u_SubArmUse_SubPlaceP
			+ u_PileNum + u_SelectClamp + b_Check_Ejector + u_SelectDetect + b_SubArmUse/*���u�����*/); // +�q�Y����W�[�� �� �e�@�B�W�[�� 
			Action_Step = StartNo_W-1 - b_SubArmSync - b_SubArmSync - b_SubArmSync*2;
			if(b_SaveFlag)
				{
					Save();
					Save2PG();
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", 1); // �]�w����{���Ĥ@�B
					SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", 6);
					g_Hint_Download = 1; // �����U��
				}
			break;
		default:
			break;
	} 
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : SaveMode()                      	     	           				 		 |
|       		: �����v����																									 |
+---------------------------------------------------------------------------*/
void	SaveMode()
{
	ClampSelect=0;// �v����
	for(int i=0;i<10;i++)
	{
		ClampSelect=ClampSelect+(b_Select[i]<<i);
	}
	printf("Set ClampSelect=%d\n",ClampSelect);
	SetDBValue(ClampSelect_DB,ClampSelect);
	if(ClampSelect==0)
		SetDBValue(ClampDetectSelect_DB,0);
	
	ClampDetectSelect=0; // �v���˴�
	for(int i=0;i<10;i++)
	{
		if(b_Select[i]==1)
			ClampDetectSelect=ClampDetectSelect+(b_DetectSelect[i]<<i);
	}
	printf("Set ClampDetectSelect=%d\n",ClampDetectSelect);
	SetDBValue(ClampDetectSelect_DB,ClampDetectSelect);
}
/*---------------------------------------------------------------------------+
|  Function : Save()                      	     	           				         |
|       		: �N�ֳt�оɽsĶ�����e�s�J�Ȧsg_QTeach_Action_P[]								 |
+---------------------------------------------------------------------------*/
void	Save()
{
	printf("====================Save====================\n");
	printf("StartNo_R=%d\n",StartNo_R); //
	printf("StartNo_W=%d\n",StartNo_W);
	printf("StepNum[%d]=%d\n",u_Group,StepNum[u_Group]);
	
	int QTeach_PGNo = StartNo_W-1; // �ֳt�о� ���B�J���X
	int Action_PNo  = StartNo_R-1; // �ֳt�о� �ʧ@�C���X	
	int Read_No = Action_PNo;

	printf("Action_Step=%d\n",Action_Step);
	
	switch(u_Group)
	{
		case Origin:
			
			break;
		/*=====================================P2�����I=====================================*/
		case WaitP: // �����I			
			for(int i =0;i<StepNum[u_Group];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{
				if(u_PickerType==MechType5) // �ϥ�5�b
				{
					if(i==1) // �[�b��2�B �]�w�ʧ@ �b�ʧ@ �s�W�ʧ@ 
					{
						printf("Set AxisY2\n");
						QTeach_PGNo++;
						Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // �g�J �b�ʧ@ Axis_Y2
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step-b_SubArmSync;// �]�wStep�� �Y���P�B(b_SubArmSync),Step�ƩM�W�@�B�ۦP
						printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,Action_Step-b_SubArmSync);
						
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0������
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_Y2]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
							SetDBValue(P2_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // �g�J WaitP db
						}	
					}
					if(i==3) // �[�b��4�B �]�w�ʧ@ �b�ʧ@ �s�W�ʧ@
					{
						printf("Set AxisX2\n");
						QTeach_PGNo++;
						Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						
						if(b_SubArmSync)
						{
							g_QTeach_Action_P[QTeach_PGNo-2].Step = 3;
							g_QTeach_Action_P[QTeach_PGNo-1].Step = 3;// �]�wStep�� �Y���P�B(b_SubArmSync),Step�ƩM�W�@�B�ۦP
							printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,3);
						}
						else
						{
							g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep�� �Y���P�B(b_SubArmSync),Step�ƩM�W�@�B�ۦP
							printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,Action_Step);
						}
						
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P2_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0������
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
							SetDBValue(P2_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // �g�J WaitP db
						}	
					}
				}
				QTeach_PGNo++;
				Action_Step++;
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]	
				Action_P[Action_PNo].Step = Action_Step;// �]�w�B�J��
					
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // �]�w��m P1����m
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P2_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J WaitP db
						if(b_Check_Move) // ���ʫ�U���Ŀ�
						{
							if(WhichAxis!=Axis_X1) // ���x�sX1�b�y��
								{
									SetDBValue(P4_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J ReadyDownP db
								}
						}
						else
						{
							SetDBValue(P4_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J ReadyDownP db
						}
						
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // �]�w�t�� P2���t��
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P2_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J WaitP db
						if(b_Check_Move) // ���ʫ�U���Ŀ�
						{
							if(WhichAxis!=Axis_X1) // ���x�sX1�b�t��
								SetDBValue(P4_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J ReadyDownP db
						}
						else
						{
							SetDBValue(P4_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J ReadyDownP db
						}
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P2_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // �g�J db
					}
				}
				if(Action_P[Action_PNo].Type == Action_Valve 
					&& Action_P[Action_PNo].Num == Valve_AxisC_V) // �֪� ���A Num�����A ����/����
				{
					Action_P[Action_PNo].Num = GetDBValue(PosStatus_DBString[WaitP]).lValue; // �g�J�ѼƼƭ� �ܦC��Action_P[]
					printf("P2 Get %s=%d\n",PosStatus_DBString[WaitP],GetDBValue(PosStatus_DBString[WaitP]).lValue);
				}
				
				// �qAction_P[]Ū���o�I�ݭn���B�J
				printf("SaveAction2Temp %d\n",Action_PNo);
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				
				if(i==1 && b_SubArmSync && u_PickerType==MechType5)
					{
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step-b_SubArmSync;// �]�wStep�� �Y���P�B(b_SubArmSync),Step�ƩM�W�@�B�ۦP
						printf("g_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,2);
					}
				
				
				Action_PNo++;
			}
			break;
		/*====================================P3�ǳƤU��====================================*/
		case ReadyDownP:
			// �NReadyDownP ��m �t�� ���]�w �g�J PickDownP	
			for(int i=0; i<6; i++)
			{
				if( (i!=Axis_Y1) && (i!=Axis_Y2) ) // Y1,Y2�b ���Q�]�w
				{
					int value=0;
					value= GetDBValue(P4_POSSET_DBString[i]).lValue; // ���o ReadyDownP DB��
					SetDBValue(P5_POSSET_DBString[i], value); // �g�J PickDownP db
					value= GetDBValue(P4_Speed_DBString[i]).lValue; // ���o ReadyDownP DB��
					SetDBValue(P5_Speed_DBString[i], value); // �g�J PickDownP db
				}
			}			
			if(b_Check_Move) // ���ʫ�U�� �Ŀ�
			{
				for(int j=0;j<u_Check_Move;j++) // �s�W�ʧ@
				{
					printf("Set Axis%d\n",j);
					QTeach_PGNo++;
					Action_Step++;
					if(j==0)
					{
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X1); // �g�J �b�ʧ@ Axis_X1
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
						if(pwndQTeach_Pos[Axis_X1]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_X1]->GetPropValueT("value", &l_Position[Axis_X1],sizeof(l_Position[Axis_X1]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X1]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_POSSET_DBString[Axis_X1], l_Position[Axis_X1]); // �g�J ReadyDownP db
							SetDBValue(P5_POSSET_DBString[Axis_X1], l_Position[Axis_X1]); // �g�J PickDownP db
						}
						if(pwndQTeach_Speed[Axis_X1]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X1]->GetPropValueT("value", &l_Speed[Axis_X1],sizeof(l_Speed[Axis_X1]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X1]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_Speed_DBString[Axis_X1], l_Speed[Axis_X1]); // �g�J ReadyDownP db
							SetDBValue(P5_Speed_DBString[Axis_X1], l_Speed[Axis_X1]); // �g�J PickDownP db
						}
						if(pwndQTeach_DT[Axis_X1]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X1]->GetPropValueT("value", &l_Delaytime[Axis_X1],sizeof(l_Delaytime[Axis_X1]));
							if(l_Delaytime[Axis_X1]!=0) // 0������
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X1]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
							SetDBValue(P4_DT_DBString[Axis_X1], l_Delaytime[Axis_X1]); // �g�J ReadyDownP db
						}
					}
					if(j==1)
					{
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // �g�J ReadyDownP db
							SetDBValue(P5_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // �g�J PickDownP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P4_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // �g�J ReadyDownP db
							SetDBValue(P5_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // �g�J PickDownP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0������
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
							SetDBValue(P4_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // �g�J ReadyDownP db
						}
					}
				}
				

			}
			break;
		/*=====================================P5�U���I=====================================*/
		case PickDownP: // �U���I
			for(int i =0;i<StepNum[u_Group];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{
				QTeach_PGNo++;
				Action_Step++;
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// �]�wStep��
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // �]�w��m P1����m
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P5_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J PickDownP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // �]�w�t�� P2���t��
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P5_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J PickDownP db
					}
					if( (b_Check_SLVL) && (Action_P[Action_PNo].Num==Axis_Y1)) // �X�ʨϥ� && Y1�b  �]�w���e P3�����e 
					{
						long l_dis =0;
						l_dis = GetDBValue(Pick_SLVL_DB).lValue;
						Action_P[Action_PNo].P3 = l_dis; // �g�J�ѼƼƭ� Action_P[Action_PNo]
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P5_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // �g�J PickDownP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				Action_PNo++;
			}
			if(b_SubArmUse) // ���u�ϥ�
			{
				printf("Set AxisY2\n");
				QTeach_PGNo++;
				if(!b_SubArmSync) // �Y���P�B �Ǹ���++
					Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // �g�J �b�ʧ@ Axis_Y2 �s�W�ʧ@
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				if(pwndQTeach_Pos[Axis_Y2]!=NULL) // �]�w��m P1����m
				{
					pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
					g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
					SetDBValue(P5_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // �g�J WaitP db
				}
				if(pwndQTeach_Speed[Axis_Y2]!=NULL) // �]�w�t�� P2���t��
				{
					pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
					g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
					SetDBValue(P5_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // �g�J WaitP db
				}
				if(b_Check_SLVL) // �X�ʨϥ� �]�w���e P3�����e 
				{
					long l_dis =0;
					l_dis = GetDBValue(Pick_SLVL_DB).lValue;
					g_QTeach_Action_P[QTeach_PGNo-1].P3 = l_dis; // �g�J�ѼƼƭ� g_QTeach_Action_P[QTeach_PGNo-1]
					printf("Setg_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,l_dis);
				}
				if(pwndQTeach_DT[Axis_Y2]!=NULL) // �]�w���� P5������
				{
					pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
					if(l_Delaytime[Axis_Y2]!=0) // 0������
						g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // �g�J�ѼƼƭ� g_QTeach_Action_P[QTeach_PGNo-1]
					SetDBValue(P5_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // �g�J WaitP db
				}	
			}
			SaveSLVLData();
			break;
		/*=====================================P6�����I=====================================*/
		case PickP: // �����I
			
			for(int i =0;i<StepNum[u_Group];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{
				if(i==0 && u_SelectClamp>0) // �]�w�ʧ@ �֪�  �s�W�ʧ@ �l
				{
					for(int j=0;j<u_SelectClamp;j++) // �ҿ� �v��ƶq
					{
						int choose=0;
						int on_off=0;
						int detect=0;
						int DelayTime=0;
						for(int k=0;k<10;k++) // �]�w (�v��j+1) 
						{
							if(b_Select[k]==1) // ��k�Ӫv�� �Q��� k��� u_pszClamp_SelectString[]
							{
								choose++;
								if(choose == j+1 && k>4)
								{
									printf("Set Valve%d\n",j);
									QTeach_PGNo++;
									Action_Step++;
									SaveAct_Valve2Temp(QTeach_PGNo,j);
									// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]		
									g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
									g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // ����k k���u_pszClamp_SelectString[]
									
									on_off = GetDBValue(P6_Clamp_onoff_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P1 = on_off; // �}�� P1
									detect = GetDBValue(P6_Clamp_detect_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // �˴� P2
									if(pwndQTeach_Clamp_DT[k]!=NULL) // �]�w���� P5������ 
									{
										pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
										if(l_Clamp_Delaytime[k]!=0) // 0������
											g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // �g�J�ѼƼƭ� g_QTeach_Action_P[]
										SetDBValue(P6_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // �g�J PickP db
									}
								}
							}
						}		
					}
				}	
				
				
				if(b_SubArmUse) // ���u�ϥ�
				{
					if(i==1) // �[�b��2�B �]�w�ʧ@ �b�ʧ@ �s�W�ʧ@ 
					{
						printf("Set AxisX2\n");
						QTeach_PGNo++;
						if(!b_SubArmSync) // �Y���P�B �Ǹ���++
							Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P6_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P6_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0������
								Action_P[Action_PNo].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
							SetDBValue(P6_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // �g�J WaitP db
						}	
					}
				}
				QTeach_PGNo++;
				Action_Step++;
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// �]�wStep��
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // �]�w��m P1����m
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P6_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J PickP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // �]�w�t�� P2���t��
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P6_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J PickP db
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P6_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // �g�J PickP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				
				if(i==1 && u_SelectClamp>0) // �]�w�ʧ@ �֪�  �s�W�ʧ@ ��
				{
					for(int j=0;j<u_SelectClamp;j++) // �ҿ� �v��ƶq
					{
						int choose=0;
						int on_off=0;
						int detect=0;
						int DelayTime=0;
						for(int k=0;k<10;k++) // �]�w (�v��j+1) 
						{
							if(b_Select[k]==1) // ��k�Ӫv�� �Q��� k��� u_pszClamp_SelectString[]
							{
								choose++;
								if(choose == j+1 && k<5)
								{
									printf("Set Valve%d\n",j);
									QTeach_PGNo++;
									Action_Step++;
									SaveAct_Valve2Temp(QTeach_PGNo,j);
									// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]		
									g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
												
									g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // ����k k���u_pszClamp_SelectString[]
									
									on_off = GetDBValue(P6_Clamp_onoff_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P1 = on_off; // �}�� P1
									detect = GetDBValue(P6_Clamp_detect_DBString[k]).lValue;
									g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // �˴� P2
									if(pwndQTeach_Clamp_DT[k]!=NULL) // �]�w���� P5������ 
									{
										pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
										if(l_Clamp_Delaytime[k]!=0) // 0������
											g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // �g�J�ѼƼƭ� g_QTeach_Action_P[]
										SetDBValue(P6_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // �g�J PickP db
									}
								}
							}
						}		
					}
				}
				if(i==1 && b_SubArmUse) // �]�w�ʧ@ �֪�  �s�W�ʧ@ ���u�֪�
				{
					QTeach_PGNo++;
					Action_Step++;
					SaveAct_Valve2Temp(QTeach_PGNo,0);
		
					g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
					g_QTeach_Action_P[QTeach_PGNo-1].Num = 14; // 
								
					//on_off = GetDBValue(P6_Clamp_onoff_DBString[k]).lValue;
					g_QTeach_Action_P[QTeach_PGNo-1].P1 = ON; // �}�� P1
					//detect = GetDBValue(P6_Clamp_detect_DBString[k]).lValue;
					//g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // �˴� P2
//								if(pwndQTeach_Clamp_DT[k]!=NULL) // �]�w���� P5������ 
//								{
//									pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
//									if(l_Clamp_Delaytime[k]!=0) // 0������
//										g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // �g�J�ѼƼƭ� g_QTeach_Action_P[]
//									SetDBValue(P6_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // �g�J PickP db
//								}
				}		
				
				if(i==2 && b_Check_Ejector) // �]�w�ʧ@ ���� 
				{
					printf("Check Ejector!!!\n");
					QTeach_PGNo++;
					Action_Step++;
					SaveAct_Wait2Temp(QTeach_PGNo,Wait_PullBack);
					
					// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
					g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
					g_QTeach_Action_P[QTeach_PGNo-1].P1 = OFF; // �}�� P1
				}
				Action_PNo++;
			}
			break;
		/*=====================================P7��X�I=====================================*/
		case MoveOutP: // ��X�I
			for(int i =0;i<StepNum[u_Group];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{
				if(b_SubArmUse) // ���u�ϥ�
				{
					if(i==1) // �[�b��1�B �]�w�ʧ@ �b�ʧ@ �s�W�ʧ@ 
					{
						QTeach_PGNo++;
						if(!b_SubArmSync) // �Y���P�B �Ǹ���++
							Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // �g�J WaitP db
						}
						if(b_Check_SLVL) // �X�ʨϥ�  �]�w���e P3�����e 
						{
							long l_dis =0;
							l_dis = GetDBValue(PickOut_SLVL_DB).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P3 = l_dis; // �g�J�ѼƼƭ� g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Setg_QTeach_Action_P[%d]=%d\n",QTeach_PGNo-1,l_dis);
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // �g�J WaitP db
						}	
					}
					if(i==2) // �[�b��2�B �]�w�ʧ@ �b�ʧ@ �s�W�ʧ@
					{
						QTeach_PGNo++;
						if(!b_SubArmSync) // �Y���P�B �Ǹ���++
							Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // �g�J �b�ʧ@ Axis_Y2
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // �]�w��m P1����m
						{
							pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // �g�J WaitP db
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // �]�w�t�� P2���t��
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // �g�J WaitP db
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // �]�w���� P5������
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(P7_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // �g�J WaitP db
						}	
					}
				}
				if( i==2 && u_SelectDetect>0) // �]�w�ʧ@ �˴�  �s�W�ʧ@
				{
					for(int j=0;j<u_SelectDetect;j++) // �ҿ� �v��ƶq
					{
						printf("Set Detect%d\n",j);
						QTeach_PGNo++;
						Action_Step++;
						SaveAct_Detect2Temp(QTeach_PGNo,j);
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]	
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��	
						int choose=0;
						int on_off=0;
						for(int k=0;k<10;k++) // �]�w (�v��j+1) 
						{
							if(b_DetectSelect[k]==1)
							{
								choose++;
								if(choose == j+1)
								{
									g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // ����k k���u_pszClamp_SelectString[]
								}
							}
						}	
					}
				}
				QTeach_PGNo++;
				Action_Step++;
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// �]�wStep��
				printf("\n\nAction_P[Action_PNo].Type=%d Action_P[Action_PNo].Num=%d\n\n",Action_P[Action_PNo].Type,Action_P[Action_PNo].Num);
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // �]�w��m P1����m
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P7_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J MoveOutP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // �]�w�t�� P2���t��
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P7_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J MoveOutP db
					}
					if( (b_Check_SLVL) && (Action_P[Action_PNo].Num==Axis_X1)) // �]�w���e P3�����e X1�b
					{
						long l_dis =0;
						l_dis = GetDBValue(PickOut_SLVL_DB).lValue;
						Action_P[Action_PNo].P3 = l_dis; // �g�J�ѼƼƭ� Action_P[Action_PNo]
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P7_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // �g�J MoveOutP db
					}
				}
				if(Action_P[Action_PNo].Type == Action_Valve 
					&& Action_P[Action_PNo].Num == Valve_AxisC_H) // �֪� ���A Num�����A ����/����
				{
					Action_P[Action_PNo].Num = GetDBValue(PosStatus_DBString[MoveOutP]).lValue; // �g�J�ѼƼƭ� �ܦC��Action_P[]
					printf("\n\n!!!!!!!!!!!!!MoveOutP =%d !!!!!!!!!!!!!!\n\n",Action_P[Action_PNo].Num);
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				Action_PNo++;
			}
			
			if(b_SubArmUse) // ���u�ϥ� (����f)
			{
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Z); // �g�J �b�ʧ@ Axis_Z (��X)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2 (�e�i)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // �g�J �b�ʧ@ Axis_Y2 (�U��)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Valve2Temp(QTeach_PGNo,0); // �g�J �֪� ���u�֪� OFF
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_Y2); // �g�J �b�ʧ@ Axis_Y2 (�W��) (�^�����I)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				
				QTeach_PGNo++;Action_Step++;
				SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2 (��h) (�^�����I)
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				
				SetDBValue(SubStartNo_DB, QTeach_PGNo-5); // �]�w ���f��Ƽg�J �}�l�Ǹ�
			}
			SaveSLVLData();
			break;
		/*=====================================P8�U���I=====================================*/
		case PlaceDownP: // �U���I
			for(int i =0;i<StepNum[u_Group];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{
				if(i==0 && PileNum>0) // �����|�ݨD  �s�W�ʧ@
				{
					// �s�W�ʧ@
					QTeach_PGNo++;
					Action_Step++;
					SaveAct_Pile2Temp(QTeach_PGNo,PileNum); // �g�J ���|(�ʧ@)
					
					// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
					g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
					g_QTeach_Action_P[QTeach_PGNo-1].Num = PileNum; // ���| ��
					g_QTeach_Action_P[QTeach_PGNo-1].P1	= ON; // ���| �}�l
				}
				
				QTeach_PGNo++;
				Action_Step++;
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// �]�wStep��
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // �]�w��m P1����m
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P8_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J PlaceDownP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // �]�w�t�� P2���t��
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P8_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J PlaceDownP db
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P8_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // �g�J PlaceDownP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				Action_PNo++;
			}
			break;
		/*=====================================P9��m�I=====================================*/
		case PlaceP: // ��m�I
			for(int i =0;i<StepNum[u_Group];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{				
				QTeach_PGNo++;
				Action_Step++;
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// �]�wStep��
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					if(pwndQTeach_Pos[WhichAxis]!=NULL) // �]�w��m P1����m
					{
						pwndQTeach_Pos[WhichAxis]->GetPropValueT("value", &l_Position[WhichAxis],sizeof(l_Position[WhichAxis]));
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P9_POSSET_DBString[WhichAxis], l_Position[WhichAxis]); // �g�J PlaceP db
					}
					if(pwndQTeach_Speed[WhichAxis]!=NULL) // �]�w�t�� P2���t��
					{
						pwndQTeach_Speed[WhichAxis]->GetPropValueT("value", &l_Speed[WhichAxis],sizeof(l_Speed[WhichAxis]));
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P9_Speed_DBString[WhichAxis], l_Speed[WhichAxis]); // �g�J PlaceP db
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						pwndQTeach_DT[WhichAxis]->GetPropValueT("value", &l_Delaytime[WhichAxis],sizeof(l_Delaytime[WhichAxis]));
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						SetDBValue(P9_DT_DBString[WhichAxis], l_Delaytime[WhichAxis]); // �g�J PlaceP db
					}
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				Action_PNo++;
			}	
			if(PileNum>0) // �����|�ݨD  �s�W�ʧ@
			{
				// �s�W�ʧ@
				QTeach_PGNo++;
				Action_Step++;
				SaveAct_Pile2Temp(QTeach_PGNo,PileNum); // �g�J ���|(�ʧ@)
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
				g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
				g_QTeach_Action_P[QTeach_PGNo-1].Num = PileNum; // ���| ��
				g_QTeach_Action_P[QTeach_PGNo-1].P1	= OFF; // ���| ����
			}
			
			if(u_SelectClamp>0) // �]�w�ʧ@ �֪�  �s�W�ʧ@ ��
			{
				for(int j=0;j<u_SelectClamp;j++) // �ҿ� �v��ƶq
				{
					int choose=0;
					int on_off=0;
					int detect=0;
					int DelayTime=0;
					for(int k=0;k<10;k++) // �]�w (�v��j+1) 
					{
						if(b_Select[k]==1) // ��k�Ӫv�� �Q��� k��� u_pszClamp_SelectString[]
						{
							choose++;
							if(choose == j+1)
							{
								printf("Set Valve%d\n",j);
								QTeach_PGNo++;
								Action_Step++;
								SaveAct_Valve2Temp(QTeach_PGNo,j);
								// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]		
								g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
											
								g_QTeach_Action_P[QTeach_PGNo-1].Num = k; // ����k k���u_pszClamp_SelectString[]
								
								on_off = GetDBValue(P9_Clamp_onoff_DBString[k]).lValue;
								g_QTeach_Action_P[QTeach_PGNo-1].P1 = on_off; // �}�� P1
								detect = GetDBValue(P9_Clamp_detect_DBString[k]).lValue;
								g_QTeach_Action_P[QTeach_PGNo-1].P2 = detect; // �˴� P2
								if(pwndQTeach_Clamp_DT[k]!=NULL) // �]�w���� P5������ 
								{
									pwndQTeach_Clamp_DT[k]->GetPropValueT("value", &l_Clamp_Delaytime[k],sizeof(l_Clamp_Delaytime[k]));
									if(l_Clamp_Delaytime[k]!=0) // 0������
										g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Clamp_Delaytime[k]*10; // �g�J�ѼƼƭ� g_QTeach_Action_P[]
									SetDBValue(P9_Clamp_DT_DBString[k], l_Clamp_Delaytime[k]); // �g�J PickP db
								}
							}
						}
					}		
				}
			}
		/*=====================================P10�^�����I=====================================*/
			// �^���I(�����I)
			for(int i =0;i<StepNum[Homing];i++) // �qAction_P[]Ū���o�I�ݭn���B�J
			{
				if(u_PickerType==MechType5) // �ϥ�5�b
				{			
					if(i==2) // �[�b��3�B �]�w�ʧ@ �b�ʧ@ �s�W�ʧ@
					{
						printf("Set AxisX\n");
						QTeach_PGNo++;
						//Action_Step++;
						SaveAct_Axis2Temp(QTeach_PGNo,Axis_X2); // �g�J �b�ʧ@ Axis_X2
						
						// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						
						g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Step;// �]�wStep��
						
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // �]�w��m P1����m
						{
							l_Position[Axis_X2] = GetDBValue(P2_POSSET_DBString[Axis_X2]).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��g_QTeach_Action_P[QTeach_PGNo-1]
						}
						// �]�w�t�� P2���t��
						{
							l_Speed[Axis_X2] = GetDBValue(P2_Speed_DBString[Axis_X2]).lValue; // ���o�����I�t��
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // �]�w���� P5������
						{
							l_Delaytime[Axis_X2] = GetDBValue(P2_DT_DBString[Axis_X2]).lValue; // ���o�����I����
							if(l_Delaytime[Axis_X2]!=0) // 0������
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
						}
					}
				}
				
				QTeach_PGNo++;
				Action_Step++;
				
				// �̷ӨϥΪ̳]�w�g�J�ѼƼƭ� �ܦC��Action_P[]		
				Action_P[Action_PNo].Step = Action_Step;// �]�wStep��
				if(Action_P[Action_PNo].Type == Action_Axis) //�b�ʧ@ 
				{
					int WhichAxis = Action_P[Action_PNo].Num; // ���@�b
					// �]�w��m P1����m
					{
						l_Position[WhichAxis] = GetDBValue(P2_POSSET_DBString[WhichAxis]).lValue; // ���o�����I�y��
						Action_P[Action_PNo].P1 = l_Position[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
					}
					// �]�w�t�� P2���t��
					{
						l_Speed[WhichAxis] = GetDBValue(P2_Speed_DBString[WhichAxis]).lValue; // ���o�����I�t��
						Action_P[Action_PNo].P2 = l_Speed[WhichAxis]; // �g�J�ѼƼƭ� �ܦC��Action_P[]
					}
					if(pwndQTeach_DT[WhichAxis]!=NULL) // �]�w���� P5������
					{
						l_Delaytime[WhichAxis] = GetDBValue(P2_DT_DBString[WhichAxis]).lValue; // ���o�����I����
						if(l_Delaytime[WhichAxis]!=0) // 0������
							Action_P[Action_PNo].P5 = l_Delaytime[WhichAxis]*10; // �g�J�ѼƼƭ� �ܦC��Action_P[]
					}
				}
				
				if(Action_P[Action_PNo].Type == Action_Valve 
					&& Action_P[Action_PNo].Num == Valve_AxisC_V) // �֪� ���A Num�����A ����/����
				{
					Action_P[Action_PNo].Num = GetDBValue(PosStatus_DBString[WaitP]).lValue; // �g�J�ѼƼƭ� �ܦC��Action_P[]
				}
				
				SaveAction2Temp(QTeach_PGNo,Action_PNo); // �N�ֳt�оɰʧ@�s�J�Ȧs��m
				Action_PNo++;
			}	
			break;
		default:
			break;
	} 
	
	b_SaveFlag=FALSE;



}
/*---------------------------------------------------------------------------+
|  Function : Save2PG()                      	     	           				       |
|       		: ��Ȧs�����e g_QTeach_Action_P[i]															 |
|							�g�J�оɵ{�� MACHINE_PROFILE_NUM%d_ACTION_STEP	, i+1					 |
+---------------------------------------------------------------------------*/
void	Save2PG()
{
	printf("===========================Save2PG===========================\n");
	CleanPG();
	char 	pDataID[256];
	int		value = 0;
	
	for(int i=0;i<MaxDBNum;i++) // 
	{
	/*======================================= �B�J�Ǹ� STEP =======================================*/
		value = g_QTeach_Action_P[i].Step;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
		SetDBValue(pDataID, value);
		printf("Set %s=%d\n",pDataID,value);
	/*======================================= �ʧ@���� TYPE =======================================*/
		value = g_QTeach_Action_P[i].Type;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		SetDBValue(pDataID, value);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
		value = g_QTeach_Action_P[i].Num;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		SetDBValue(pDataID, value);
	/*======================================= �Ѽ�1 =======================================*/
		value = g_QTeach_Action_P[i].P1;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1",i+1);
		SetDBValue(pDataID, value);
	/*======================================= �Ѽ�2 =======================================*/
		value = g_QTeach_Action_P[i].P2;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2",i+1);
		SetDBValue(pDataID, value);
	/*======================================= �Ѽ�3 =======================================*/
		value = g_QTeach_Action_P[i].P3;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3",i+1);
		SetDBValue(pDataID, value);
	/*======================================= �Ѽ�4 =======================================*/
		value = g_QTeach_Action_P[i].P4;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4",i+1);
		SetDBValue(pDataID, value);
	/*======================================= �Ѽ�5 =======================================*/
		value = g_QTeach_Action_P[i].P5;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5",i+1);
		SetDBValue(pDataID, value);
	}
}
/*---------------------------------------------------------------------------+
|  Function : CleanPG()                      	     	           				 			 |
|       		: �M���{���оɵ{��																							 |
+---------------------------------------------------------------------------*/
void	CleanPG()
{
	char 	pDataID[256];	
	for(int i=0;i<MaxDBNum;i++) //
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
		SetDBValue(pDataID, 0);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		SetDBValue(pDataID, 0);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		SetDBValue(pDataID, 0);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i+1,j+1);
			SetDBValue(pDataID, 0);
		}
	}
}
/*---------------------------------------------------------------------------+
|  Function : Clean_QTeach()                  						   				         |
|       		: �M���ֳt�оɽsĶ�����e																				 |
+---------------------------------------------------------------------------*/
void	Clean_QTeach()
{
	printf("Clean_QTeach\n");
	char 	pDataID[256];	
	for(int i=0;i<MaxDBNum;i++) //
	{
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_STEP",i+1);
//		SetDBValue(pDataID, 0);
		g_QTeach_Action_P[i].Step = 0;
		
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_TYPE",i+1);
//		SetDBValue(pDataID, 0);
		g_QTeach_Action_P[i].Type = 0;
		
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_NUM",i+1);
//		SetDBValue(pDataID, 0);
		g_QTeach_Action_P[i].Num = 0;
		
		g_QTeach_Action_P[i].P1 = 0;
		g_QTeach_Action_P[i].P2 = 0;
		g_QTeach_Action_P[i].P3 = 0;
		g_QTeach_Action_P[i].P4 = 0;
		g_QTeach_Action_P[i].P5 = 0;
//		for(int j=0;j<PARA_NUM;j++)
//		{
//			memset(pDataID, 0 ,sizeof(pDataID));
//			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_EQUIPMENT1_ACTION_PARAMETER%d",i+1,j+1);
//			SetDBValue(pDataID, 0);
//		}
	}
	
}
/*---------------------------------------------------------------------------+
|  Function : GetPosNow(int GetAxisPos)                  				   		       |
|       		: �����e GetAxisPos �b��m																		 |
+---------------------------------------------------------------------------*/
void	GetPosNow(int GetAxisPos)
{
	printf("Get Pos %d\n",GetAxisPos);

	if((u_Group==ReadyDownP) && (GetAxisPos==Axis_Y1) // P4�ǳƤU���I ��X1,X2�b�~���i���o
		|| ((u_Group==ReadyDownP) && (GetAxisPos==Axis_Z))
		|| ((u_Group==ReadyDownP) && (GetAxisPos==Axis_Y2)) )	
	{
	}
	else if( (u_Group==PickDownP) && (GetAxisPos==Axis_X1) // P5���X���U���I �I��Y1,Y2�b�~���i���o
		|| ((u_Group==PickDownP) && (GetAxisPos==Axis_Z))
		|| ((u_Group==PickDownP) && (GetAxisPos==Axis_X2)) )	
	{
	}
	else if( (u_Group==PickP) && (GetAxisPos==Axis_Y1) // P6�����I ��X1,X2�b�~���i���o
		|| ((u_Group==PickP) && (GetAxisPos==Axis_Z))
		|| ((u_Group==PickP) && (GetAxisPos==Axis_Y2)) )	
	{
	}
	else if( ((u_Group==MoveOutP) && (GetAxisPos==Axis_Z)) ) // P7��X�I �I��X1,X2,Y1,Y2�b�~���i���o
	{
	}
	else if((u_Group==PlaceDownP) && (GetAxisPos==Axis_Y1)
		|| ((u_Group==PlaceDownP) && (GetAxisPos==Axis_X2))
		|| ((u_Group==PlaceDownP) && (GetAxisPos==Axis_Y2)) ) // P8�m�����U���I �I��X,Z�b�~���i���o
	{
	}
	else if( (u_Group==PlaceP) && (GetAxisPos!=Axis_Y1) ) // P9�m�����m���I �I��Y�b�~���i���o
	{
	}
	else
	{
		long AxisPosNow = GetDBValue(AxisPosNow_DBString[GetAxisPos]).lValue; // ���o�ƭ�
		printf("Get %s = %d\n",u_pszQTeach_PosString[GetAxisPos],AxisPosNow);
		if(pwndQTeach_Pos[GetAxisPos] != NULL)
		{
			long Max_value=0,Min_value=0;
			char	MAX_DBID[256];	memset(MAX_DBID, 0, sizeof(MAX_DBID));
			pwndQTeach_Pos[GetAxisPos]->GetPropValueT("maxid", MAX_DBID, sizeof(MAX_DBID));
			Max_value = GetDBValue(MAX_DBID).lValue;
			printf("Max:%d\n",Max_value);
			char	MIN_DBID[256];	memset(MIN_DBID, 0, sizeof(MIN_DBID));
			pwndQTeach_Pos[GetAxisPos]->GetPropValueT("minid", MIN_DBID, sizeof(MIN_DBID));
			Min_value = GetDBValue(MIN_DBID).lValue;
			printf("Min:%d\n",Min_value);
			if( (AxisPosNow-Max_value)>0 )
			{
				printf("AxisPosNow>MAX_DBID\n");
				MsgBox(g_MultiLanguage["TEACH_QTEACH_POSMAX_ERROR"], tmFT_CODE_TECH); // �W�L�̤j��
			}
			else if((AxisPosNow-Min_value)<0)
			{
				printf("AxisPosNow<Min_value\n");
				MsgBox(g_MultiLanguage["TEACH_QTEACH_POSMIN_ERROR"], tmFT_CODE_TECH); // �p��̤p��
			}
			else
			{
				printf("Set Axis%d = %d\n",GetAxisPos,AxisPosNow);
				pwndQTeach_Pos[GetAxisPos]->OnLoseFocus();
				pwndQTeach_Pos[GetAxisPos]->SetPropValueT("value",AxisPosNow); // �g�J�ƭ�
				UpdateTeach_Pos();
			}
		}
	}	
}

/*---------------------------------------------------------------------------+
|  Function : UpdateImg()                  						   				   		       |
|       		: ��s��ܹϤ�																									 |
+---------------------------------------------------------------------------*/
void	UpdateImg()
{
	printf("Update Img\n");
	char 	pDataID[256];
	switch(u_Group) // ���o���A��
	{
		case WaitP: // �����I
			if(pwndBtn_PosHV !=NULL) // ���A��ܦs�b
			{
				if(GetDBValue("MACHINE_FUNCTION_OPTIONS_MOLD_POSE").lValue == 0)// �Y�Ҥ����A �S�w�q
				{
					pwndBtn_PosHV->SetPropValueT("enabled",double(TRUE)); // ���꫺�A���
					u_status = GetDBValue(PosStatus_DBString[WaitP]).lValue; // Ū�����A
				}
				else // �Ҥ����A �w�w�q
				{
					u_status = GetDBValue("MACHINE_FUNCTION_OPTIONS_MOLD_POSE").lValue; // Ū�����A
//					if(u_status==1) // �Ҥ����A����
//						u_status=4;
//					else if(u_status==2) // �Ҥ����A����
//						u_status=3;
					SetDBValue(PosStatus_DBString[WaitP],u_status); // �����I�����A
					SetDBValue(PosStatus_DBString[ReadyDownP],u_status+2); // �ǳƤU���I�����A
					SetDBValue(PosStatus_DBString[PickDownP],u_status+2); // ���󰼤U���I�����A
					SetDBValue(PosStatus_DBString[PickP],u_status+2); // �����I�����A
					pwndBtn_PosHV->SetPropValueT("enabled",double(FALSE)); // �꫺�A���
				}
			}
			if(u_status == 0)// �Y���A�L�w�q �h���w�w�]��
			{
				u_status=Valve_AxisC_V; // ����
				SetDBValue(PosStatus_DBString[WaitP],u_status); // �����I�����A
				SetDBValue(PosStatus_DBString[ReadyDownP],u_status+2); // �ǳƤU���I�����A
				SetDBValue(PosStatus_DBString[PickDownP],u_status+2); // ���󰼤U���I�����A
				SetDBValue(PosStatus_DBString[PickP],u_status+2); // �����I�����A
			}
			break;
		case ReadyDownP: // �ǳƤU���I
			u_status = GetDBValue(PosStatus_DBString[ReadyDownP]).lValue; // ���A
			if(pwndCheck_Move!=NULL) // �e�i��U��
			{
				pwndCheck_Move->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Move]);
				pwndCheck_Move->CreateA();
				pwndCheck_Move->Update();
				if(b_Check_Move)
				{
					UnLockBtn(pwndQTeach_Pos[Axis_X1]);
					UnLockBtn(pwndQTeach_Speed[Axis_X1]);
					UnLockBtn(pwndQTeach_DT[Axis_X1]);
					UnLockBtn(pwndBtn_GetPos[Axis_X1]);
					if(pwndImg_QTeach_ReadyDownP!=NULL)
					{
						pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_ReadyDownP.bmp");
						pwndImg_QTeach_ReadyDownP->Show();							
					}
				}
				else
				{
					LockBtn(pwndQTeach_Pos[Axis_X1]);
					LockBtn(pwndQTeach_Speed[Axis_X1]);
					LockBtn(pwndQTeach_DT[Axis_X1]);
					LockBtn(pwndBtn_GetPos[Axis_X1]);
					if(pwndImg_QTeach_ReadyDownP!=NULL)
					{
						pwndImg_QTeach_ReadyDownP->SetPropValueT("imagepath","res_tm640/pic/picker/QTeach_WaitP.bmp");
						pwndImg_QTeach_ReadyDownP->Show();							
					}
				}
			}
			break;
		case PickDownP: // ���X���U���I
			u_status = GetDBValue(PosStatus_DBString[PickDownP]).lValue; // ���A
			if(pwndCheck_SLVL!=NULL) // �X�ʨϥ�
			{
				pwndCheck_SLVL->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_SLVL]);
				pwndCheck_SLVL->CreateA();
				pwndCheck_SLVL->Update();
				if(b_Check_SLVL)
				{
					UnLockBtn(pwndPick_SLVL);
				}
				else
				{
					LockBtn(pwndPick_SLVL);
				}
			}
			break;
		case PickP: // �����I
			u_status = GetDBValue(PosStatus_DBString[PickP]).lValue; // ���A
			for(int i=0;i<10;i++)
			{
				if(pwndQTeach_Clamp_DT[i]!=NULL) // ��� �v�� ���� �ƭ�
					{
						l_Clamp_Delaytime[i] = GetDBValue(P6_Clamp_DT_DBString[i]).lValue; // Ū��DB�ƭ�
						pwndQTeach_Clamp_DT[i]->SetPropValueT("value",l_Clamp_Delaytime[i]); // �g�J�ƭ�
						pwndQTeach_Clamp_DT[i]->CreateA();
						pwndQTeach_Clamp_DT[i]->Update();
					}
				u_clamp_onoff[i] = GetDBValue(P6_Clamp_onoff_DBString[i]).lValue; // �v�� ON/OFF ���A
				u_detect_onoff[i] = GetDBValue(P6_Clamp_detect_DBString[i]).lValue; // �v�� �˴� ���A 
			}
			
			if(pwndCheck_Ejector!=NULL) // ��� �˴����h����(���X�}�l)
			{
				pwndCheck_Ejector->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Ejector]);
				pwndCheck_Ejector->CreateA();
				pwndCheck_Ejector->Update();
			}
			
			break;
		case MoveOutP: // ��X�I
			if(pwndBtn_PosHV !=NULL) // ���A��ܦs�b
			{
				if(GetDBValue("MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE").lValue == 0)// �Y��X���A �S�w�q
				{
					pwndBtn_PosHV->SetPropValueT("enabled",double(TRUE)); // ���꫺�A���
					u_status = GetDBValue(PosStatus_DBString[MoveOutP]).lValue; // Ū�����A
				}
				else // �Ҥ����A �w�w�q
				{
					u_status = GetDBValue("MACHINE_FUNCTION_OPTIONS_ZAXIS_POSE").lValue; // Ū�����A
//					if(u_status==1) // �Ҥ����A����
//						u_status=4;
//					else if(u_status==2) // �Ҥ����A����
//						u_status=3;
					SetDBValue(PosStatus_DBString[MoveOutP],u_status); // ���X����X�I�����A
					SetDBValue(PosStatus_DBString[PlaceDownP],u_status+2); // �m�����U���I�����A
					pwndBtn_PosHV->SetPropValueT("enabled",double(FALSE)); // �꫺�A���
				}
			}
			if(u_status == 0)// �Y���A�L�w�q �h���w�w�]��
			{
				u_status=Valve_AxisC_H; // ����
				SetDBValue(PosStatus_DBString[MoveOutP],u_status); // ��X�I�����A
			}
			if(pwndCheck_SLVL!=NULL) // �X�ʨϥ�
			{
				pwndCheck_SLVL->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_SLVL]);
				pwndCheck_SLVL->CreateA();
				pwndCheck_SLVL->Update();
				if(b_Check_SLVL)
				{
					UnLockBtn(pwndPickOut_SLVL);
				}
				else
				{
					LockBtn(pwndPickOut_SLVL);
				}
			}
			
			break;
		case PlaceDownP: // �U���I
			u_status = GetDBValue(PosStatus_DBString[PlaceDownP]).lValue; // ���A  
			break;
		case PlaceP: // ��m�I
			u_status = GetDBValue(PosStatus_DBString[PlaceP]).lValue; // ���A
			for(int i=0;i<10;i++)
			{
				if(pwndQTeach_Clamp_DT[i]!=NULL) // ��� �v�� ���� �ƭ�
					{
						l_Clamp_Delaytime[i] = GetDBValue(P9_Clamp_DT_DBString[i]).lValue; // Ū��DB�ƭ�
						pwndQTeach_Clamp_DT[i]->SetPropValueT("value",l_Clamp_Delaytime[i]); // �g�J�ƭ�
						pwndQTeach_Clamp_DT[i]->CreateA();
						pwndQTeach_Clamp_DT[i]->Update();
					}
				u_clamp_onoff[i] = GetDBValue(P9_Clamp_onoff_DBString[i]).lValue; // �v�� ON/OFF ���A
				u_detect_onoff[i] = GetDBValue(P9_Clamp_detect_DBString[i]).lValue; // �v�� �˴� ���A
			}
			break;
		default:
			break;
	}
	
	if(pwndBtn_PosHV != NULL) // ��ܫ��A���A
	{
	 printf("u_status=%d\n",u_status);
	 pwndBtn_PosHV->SetPropValueT("upbitmap",PosHV_ImgPath[u_status]);
	 pwndBtn_PosHV->CreateA();
	 pwndBtn_PosHV->Update();
	}
	
	for(int i=0;i<10;i++) // ��ܪv��̪��A
	{
		if(pwndBtn_Clamp[i] != NULL) // �v��̪��A
		{
		 printf("%s=%d\n",u_pszBtn_Clamp_String[i],u_clamp_onoff[i]);
		 pwndBtn_Clamp[i]->SetPropValueT("upbitmap",Clamp_ImgPath[ (u_clamp_onoff[i]*10+i)*b_Select[i] ]);
		 pwndBtn_Clamp[i]->CreateA();
		 pwndBtn_Clamp[i]->Update();
		 if(u_SelectClamp>0)
		 	b_ClampSet_OK=0; // �Y���v����s �ݽT�{
		}	
		if(pwndDetectBox[i] != NULL) // ��ܪv���˴�ON/OFF���A
		{
		 pwndDetectBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[u_detect_onoff[i]]);
		 pwndDetectBox[i]->CreateA();
		 pwndDetectBox[i]->Update();
		}
		Update_ClampStr(); // ��ܳQ��ܪv�㪺��r����
	}
	
	
	if(pwndStr_Place_Z!=NULL) // ��� ��m/���| ��r����
		{
			if(PileNum>0) //���|
				pwndStr_Place_Z->SetPropValueT("textID","TEACH_QTEACH_PILEZ");
			else
				pwndStr_Place_Z->SetPropValueT("textID","TEACH_QTEACH_PLACEZ");
			pwndStr_Place_Z->Update();
		}
	if(pwndStr_Place_X!=NULL) // ��� ��m/���| ��r����
		{
			if(PileNum>0) //���|
				pwndStr_Place_X->SetPropValueT("textID","TEACH_QTEACH_PILEX");
			else
				pwndStr_Place_X->SetPropValueT("textID","TEACH_QTEACH_PLACEX");
			pwndStr_Place_X->Update();
		}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateDataValue()                  						   				   		 |
|       		: ��s�]�w��m�B�]�w�t�׼ƭ�																		 |
+---------------------------------------------------------------------------*/
void	UpdateDataValue()
{
	//printf("Update data\n");
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
|  Function : UpdateClamp()                  						   				   		     |
|       		: �v�������																									 |
+---------------------------------------------------------------------------*/
void	UpdateClamp()
{
	printf("Update Clamp\n");
	ClampSelect = GetDBValue(ClampSelect_DB).lValue;
	ClampDetectSelect = GetDBValue(ClampDetectSelect_DB).lValue;
	
	//printf("ClampSelect=%d\n",ClampSelect);
	
	// �B�n
	if(pwndClamp_Mask!=NULL)
		{
			pwndClamp_Mask->Update();
		}
	for(int i=0; i<10; i++)
 	{
 		//if(((ClampSelect>>i) & 1)==1)
 		//	printf("Select[%d] = %d\n",i,((ClampSelect>>i) & 1)); // �v���ܽT�{
 		b_Select[i] = ((ClampSelect>>i) & 1);
	 	if((pwndClamp_Select[i]!=NULL)&&(pwndCheckBox[i]!=NULL))  //�v����
	 	{
			pwndCheckBox[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Select[i]]);
			pwndCheckBox[i]->CreateA();
			pwndCheckBox[i]->Update();
	 	}
 		b_DetectSelect[i] = ((ClampDetectSelect>>i) & 1);
	 	if(pwndDetect_Select[i]!=NULL)  //�v���˴����
	 	{
			pwndDetect_Select[i]->SetPropValueT("upbitmap",CheckBox_ImgPath[b_DetectSelect[i]]);
			pwndDetect_Select[i]->CreateA();
			pwndDetect_Select[i]->Update();
	 	}
	 	
	 	if(pwndBtn_Clamp[i]!=NULL) // �v�� ON/OFF Btn
 		{
 			if(b_Select[i]==1)
			{
				pwndBtn_Clamp[i]->SetPropValueT("enabled",double(TRUE));
				pwndBtn_Clamp[i]->SetPropValueT("upbitmap",Clamp_ImgPath[(u_clamp_onoff[i]*10+i)]); //Clamp_Hint_ImgPath //Clamp_ImgPath
				pwndBtn_Clamp[i]->CreateA();
				pwndBtn_Clamp[i]->Update();
			}
			else
			{
				pwndBtn_Clamp[i]->SetPropValueT("enabled",double(FALSE));
			}
 		}
 		if(pwndQTeach_Clamp_DT[i]!=NULL) // �v�� ���� �ƭȿ�J
		{
 			if(b_Select[i]==1)
			{
				pwndQTeach_Clamp_DT[i]->SetPropValueT("enabled",double(TRUE));
				pwndQTeach_Clamp_DT[i]->CreateA();
				pwndQTeach_Clamp_DT[i]->Update();
			}
			else
			{
				pwndQTeach_Clamp_DT[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
 		if(pwndDetectBox[i]!=NULL) // �v�� �˴� �Ĥ�
		{
 			if(b_Select[i]==1)
			{
				pwndDetectBox[i]->SetPropValueT("enabled",double(TRUE));
				pwndDetectBox[i]->CreateA();
				pwndDetectBox[i]->Update();
			}
			else
			{
				pwndDetectBox[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
 	}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateShow()                  						   				   		     |
|       		: �e����l���																									 |
+---------------------------------------------------------------------------*/
void	UpdateShow()
{
	UpdateDataValue();
	UpdateClamp();
	// �T�{�v����
	for(int i=0;i<10;i++)
	{
		if(b_Select[i]==Selected)
			{
				u_SelectClamp++;
				SelectClamp[u_SelectClamp]=i;
			}
	}
	printf("u_SelectClamp=%d\n",u_SelectClamp);
	
	// �T�{�v���˴����
	for(int i=0;i<10;i++)
	{
		if(b_DetectSelect[i]==Selected)
			{
				u_SelectDetect++;
				SelectDetect[u_SelectDetect]=i;
			}
	}
	printf("u_SelectDetect=%d\n",u_SelectDetect);
	UpdateBtnDetect_Select();
	UpdateImg();
	if((u_Group == 0)||(u_Group == ReadyDownP))
		Update_Check_Move();
	UpdateHint();
}
/*---------------------------------------------------------------------------+
|  Function : UpdateSubArm()                  													     |
|       		: ��s���� ���u��� �P�B�Ĥ�																		 |
+---------------------------------------------------------------------------*/
void UpdateSubArm() // ��s ���u��� ���s��
{
	if(pwndMask_SubArm!=NULL) // ���u �B�n
	{
		pwndMask_SubArm->Update();
	}
	
	if( pwndCheck_SubUse!=NULL && pwndBtn_SubUse!=NULL) // ���u�ϥ� Btn
	{
		if(u_PickerType==MechType3) // 3 �b�����
		{
			pwndCheck_SubUse->SetPropValueT("enabled",(double)(FALSE));
			pwndBtn_SubUse->SetPropValueT("enabled",(double)(FALSE));
		}
		else if(u_PickerType==MechType5) // ���b �~���
		{
			pwndCheck_SubUse->SetPropValueT("enabled",(double)(TRUE)); // �Ĥ�
			pwndCheck_SubUse->SetPropValueT("upbitmap",CheckBox_ImgPath[b_SubArmUse]);
			pwndCheck_SubUse->CreateA();
			pwndCheck_SubUse->Update();
			
			pwndBtn_SubUse->SetPropValueT("enabled",(double)(TRUE)); //Btn
			pwndBtn_SubUse->Update();
		}
	}
	if( pwndCheck_SubSync!=NULL && pwndBtn_SubSync!=NULL) // ���u�P�D�u�P�� Btn
	{
		if(b_SubArmUse==TRUE) // ���u�ϥ� ���u�P�D�u�P�ʤ~�i��
		{
			pwndCheck_SubSync->SetPropValueT("enabled",(double)(TRUE)); // �Ĥ�
			pwndCheck_SubSync->SetPropValueT("upbitmap",CheckBox_ImgPath[b_SubArmSync]);
			pwndCheck_SubSync->CreateA();
			pwndCheck_SubSync->Update();
			
			pwndBtn_SubSync->SetPropValueT("enabled",(double)(TRUE)); // Btn
			pwndBtn_SubSync->Update();
		}
		else
		{
			pwndCheck_SubSync->SetPropValueT("enabled",(double)(FALSE)); // �Ĥ�
			pwndBtn_SubSync->SetPropValueT("enabled",(double)(FALSE)); // Btn
		}
	}
	
	
}
/*---------------------------------------------------------------------------+
|  Function : UpdateBtnDetect_Select()                  								     |
|       		: ��s���� �v���� �˴��Ĥ�																		 |
+---------------------------------------------------------------------------*/
void UpdateBtnDetect_Select() // ��s �˴���ܫ���
{
	printf("UpdateBtnDetect_Select\n");
	if(pwndDetect_Mask1!=NULL) // �˴��B�n1
	{
		pwndDetect_Mask1->Update();
	}
	if(pwndDetect_Mask2!=NULL) // �˴��B�n2
	{
		pwndDetect_Mask2->Update();
	}
		
	for(int i=0;i<10;i++) // �˴� Btn
	{
		if(pwndDetect_Select[i]!=NULL)
		{
			if(b_Select[i])
			{
				pwndDetect_Select[i]->SetPropValueT("enabled",(double)(TRUE));
				pwndDetect_Select[i]->CreateA();
				pwndDetect_Select[i]->Update();	
				if(i==Valve_Clamp1 || i==Valve_Clamp2)
					{
						if(pwndStr_Detect1!=NULL)
							pwndStr_Detect1->Update();
					}
				if(i==Valve_Suck1 || i==Valve_Suck2)
					{
						if(pwndStr_Detect2!=NULL)
							pwndStr_Detect2->Update();
					}
			}
			else
			{
				pwndDetect_Select[i]->SetPropValueT("enabled",(double)(FALSE));
			}
		}
	}
}
/*---------------------------------------------------------------------------+
|  Function : Update_ClampStr()                  								  				   |
|       		: ��s���� �Q��ܪv�� ��r����																	 |
+---------------------------------------------------------------------------*/
void Update_ClampStr()
{
	printf("Update_ClampStr\n");
	for(int i=0;i<10;i++)
	{
		if(b_Select[i]) // �v��i�Q���
		{
			if(i<=Valve_Clamp2) // ����
				{
					if(pwndStr_Clamp!=NULL)
						pwndStr_Clamp->Update();
					if(pwndStr_Clamp_Detect1!=NULL)
						pwndStr_Clamp_Detect1->Update();
					if(pwndStr_Clamp_DT1!=NULL)
						pwndStr_Clamp_DT1->Update();
				}
			else if(i>=Valve_Suck1) // �l�L
				{
					if(pwndStr_Suck!=NULL)
						pwndStr_Suck->Update();
					if(pwndStr_Clamp_Detect2!=NULL)
						pwndStr_Clamp_Detect2->Update();
					if(pwndStr_Clamp_DT2!=NULL)
						pwndStr_Clamp_DT2->Update();
				}
		}
	}
}		
/*---------------------------------------------------------------------------+
|  Function : Update_SLVL()                  										  				   |
|       		: ��s���� �X�ʨϥ� �X�ʳ]�m																		 |
+---------------------------------------------------------------------------*/
void	Update_SLVL()
{
	printf("Update_SLVL\n");
	if(pwndMask_SLVL!=NULL) // �B�n
	{
		pwndMask_SLVL->Update();
	}
	
	if(b_Check_SLVL) // �Y ��ܨϥ�
	{
		if(pwndPick_SLVL!=NULL)
		{
			pwndPick_SLVL->Show(); // ���
		}
		if(pwndPick_Dis!=NULL)
		{
			pwndPick_Dis->Show(); // ���
		}
		if(pwndPickOut_SLVL!=NULL)
		{
			pwndPickOut_SLVL->Show(); // ���
		}
		if(pwndPickOut_Dis!=NULL)
		{
			pwndPickOut_Dis->Show(); // ���
		}
		if(pwndSLVL_Logo!=NULL)
		{
			pwndSLVL_Logo->Show(); // ���
		}
	}
	else
	{
	}
}

/*---------------------------------------------------------------------------+
|  Function : UpdateBtnTeachStart()                  										     |
|       		: ��s���� �}�l�о�																							 |
+---------------------------------------------------------------------------*/
void	UpdateBtnTeachStart()
{
	ClampSelect=0;
	for(int i=0;i<10;i++)
	{
		ClampSelect=ClampSelect+(b_Select[i]<<i);
	}

	if(pwndBtnTeachStart!=NULL) // �}�l�о�
	{
		if(ClampSelect>0)
			{
				pwndBtnTeachStart->SetPropValueT("upbitmap","res_tm640/pic/PGBtnSelect.bmp");
			}
		else
			{
				pwndBtnTeachStart->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
			}
		pwndBtnTeachStart->CreateA();
		pwndBtnTeachStart->Update();
	}
	if(pwndStr_ClampSelect!=NULL) // �v����
	{
		if(ClampSelect>0)
		{
			pwndStr_ClampSelect->SetPropValueT("bgc",0xF7BE); // �Ǧ�
		}
		else
		{
			pwndStr_ClampSelect->SetPropValueT("bgc",0xFF80); // ����
		}
		pwndStr_ClampSelect->CreateA();
		pwndStr_ClampSelect->Update();
	}
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
		printf("b_All_PosSet_OK==%d,b_ClampSet_OK=%d\n",b_All_PosSet_OK,b_ClampSet_OK);
		if(b_All_PosSet_OK==OK) // ��m && �v�� && �t�� �]�w���� &&(b_All_SpdSet_OK==OK) &&(b_ClampSet_OK==OK)
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
	printf("Teach_SetNum=%d,u_All_PosSet_OK=%d\n",Teach_SetNum[u_PickerType][u_Group],u_All_PosSet_OK);
	if(u_All_PosSet_OK>=Teach_SetNum[u_PickerType][u_Group])
		b_All_PosSet_OK =OK; //������m�ҳ]�w����
}
/*---------------------------------------------------------------------------+
|  Function : UpdateTeach_Speed()     	             										  	 |
|       		: ��s���� �t�׳]�w��																						 |
+---------------------------------------------------------------------------*/
void	UpdateTeach_Speed()
{
	printf("UpdateTeach_Speed()\n");
//	int u_All_SpdSet_OK=0;
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		if(pwndQTeach_Speed[i]!=NULL)
		{
//			if(b_SpdSet_OK[i]==OK)
//			{
//				u_All_SpdSet_OK++;
//				pwndQTeach_Speed[i]->SetPropValueT("bgc",Color_LBlue); // �զ�
//			}
//			else
//			{
//				pwndQTeach_Speed[i]->SetPropValueT("bgc",Color_Yellow); // ����
//			}
			pwndQTeach_Speed[i]->Show();
			pwndQTeach_Speed[i]->OnLoseFocus();
		}
	} 
//	if(u_All_SpdSet_OK>=Teach_SetNum[u_PickerType][u_Group])
//		b_All_SpdSet_OK =OK; //������m�ҳ]�w����
}


/*---------------------------------------------------------------------------+
|  Function : UpdateHint()                  												  		   |
|       		: ��s���� 																											 |
+---------------------------------------------------------------------------*/
void	UpdateHint()
{
	UpdateBtnTeachStart();
	UpdateTeach_Pos(); // ��s ��m�]�w
	UpdateTeach_Speed(); // ��s �t�׳]�w
	UpdateBtnNextP(); // �̫��s �U�@��m �������
}
/*---------------------------------------------------------------------------+
|  Function : UpdateCheck_Pile()           																	 |
|       		: ��s���� ���|���																							 |
+---------------------------------------------------------------------------*/
void	UpdateCheck_Pile()
{
	// ���|���
	if(PileNum>0)
	{
		if(pwndCheck_Pile!=NULL)
			pwndCheck_Pile->SetPropValueT("upbitmap",CheckBox_ImgPath[OK]);
	}
	else
	{
		if(pwndCheck_Pile!=NULL)
			pwndCheck_Pile->SetPropValueT("upbitmap",CheckBox_ImgPath[NO]);
	}
	if(pwndCheck_Pile!=NULL)
	{
		pwndCheck_Pile->CreateA();
		pwndCheck_Pile->Show();
	}
	
	// ���|�ռ� �B�n
	if(pwndMask_Pile!=NULL)
		pwndMask_Pile->Show();
	
	
	// ���|�ռ� ��ܶs
	if(PileNum>0)
	{
		// enable
		for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ ) // ���|�ռ� ��ܶs
		{
			if(pwndBtn_PileChoose[i]!=NULL)
			{
				pwndBtn_PileChoose[i]->SetPropValueT("enabled",double(TRUE));
				pwndBtn_PileChoose[i]->Show();
			}
		}
		for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ ) // ���|�ռ� Btn
		{
			if(pwndBtn_PileNum[i]!=NULL)
			{
				pwndBtn_PileNum[i]->SetPropValueT("enabled",double(TRUE));
				pwndBtn_PileNum[i]->Show();
			}
		} 
		// Update Img
		for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ )
		{
			if(pwndBtn_PileChoose[i]!=NULL)
			{
				if(i==PileNum)
					{
						pwndBtn_PileChoose[i]->SetPropValueT("upbitmap",ChooseBox_ImgPath[OK]);
					}
				else
					pwndBtn_PileChoose[i]->SetPropValueT("upbitmap",ChooseBox_ImgPath[NO]);
				pwndBtn_PileChoose[i]->CreateA();
				pwndBtn_PileChoose[i]->Show();
			}
		} 
	}
	else
	{
		// disable
		for(int i = 0; i < sizeof(PileChoose_String)/sizeof(PileChoose_String[0]); i++ ) // ���|�ռ� ��ܶs
		{
			if(pwndBtn_PileChoose[i]!=NULL)
			{
				pwndBtn_PileChoose[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
		for(int i = 0; i < sizeof(PileNum_String)/sizeof(PileNum_String[0]); i++ ) // ���|�ռ� Btn
		{
			if(pwndBtn_PileNum[i]!=NULL)
			{
				pwndBtn_PileNum[i]->SetPropValueT("enabled",double(FALSE));
			}
		} 
	}
	
	
}
/*---------------------------------------------------------------------------+
|  Function : Update_AxisMoveNow()           																 |
|       		: ��s �{�b���ʶb																								 |
+---------------------------------------------------------------------------*/
void	Update_AxisMoveNow(int Axis)
{
	if(pwndImg_NowMoveAxis!=NULL) // �Ϥ�
	{
		pwndImg_NowMoveAxis->SetPropValueT("imagepath",Axis_ImgPath[Axis]);
		pwndImg_NowMoveAxis->Show();
	}
	if(pwndStr_NowMoveAxis !=NULL) // ��r
	{
		if(Axis==0)
			pwndStr_NowMoveAxis->SetPropValueT("textID","PICKER_NULL");
		else
			pwndStr_NowMoveAxis->SetPropValueT("textID","TEACH_QTEACH_NOW_MOVE");
		//pwndStr_NowMoveAxis->CreateA();
		pwndStr_NowMoveAxis->Show();
	}
	
}
			

/*---------------------------------------------------------------------------+
|  Function : SaveAction2Temp()                  												  	 |
|       		: �N�ֳt�оɰʧ@�s�J�Ȧsg_QTeach_Action_P												 |
+---------------------------------------------------------------------------*/
void	SaveAction2Temp(int QTeach_PGNo,int j)
{
	/*======================================= �B�J�Ǹ� STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_P[j].Step);
	/*======================================= �ʧ@���� TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_P[j].Type);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_P[j].Num);
	/*======================================= �Ѽ�1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_P[j].P1);
	/*======================================= �Ѽ�2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_P[j].P2);
	/*======================================= �Ѽ�3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_P[j].P3);
	/*======================================= �Ѽ�4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_P[j].P4);
	/*======================================= �Ѽ�5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_P[j].P5);
}

/*---------------------------------------------------------------------------+
|  Function : SaveAct_Axis2Temp()                  													 |
|       		: �N�ֳt�оɰʧ@ �b�ʧ@ �s�J�Ȧsg_QTeach_Action_P								 |
+---------------------------------------------------------------------------*/
void	SaveAct_Axis2Temp(int QTeach_PGNo,int Axis)
{
	/*======================================= �B�J�Ǹ� STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[Axis].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].Step);
	/*======================================= �ʧ@���� TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Axis_P[Axis].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].Type);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Axis_P[Axis].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].Num);
	/*======================================= �Ѽ�1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Axis_P[Axis].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P1);
	/*======================================= �Ѽ�2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Axis_P[Axis].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P2);
	/*======================================= �Ѽ�3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Axis_P[Axis].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P3);
	/*======================================= �Ѽ�4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Axis_P[Axis].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P4);
	/*======================================= �Ѽ�5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Axis_P[Axis].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Axis_P[Axis].P5);
}
/*---------------------------------------------------------------------------+
|  Function : SaveAct_Valve2Temp()                  												 |
|       		: �N�ֳt�оɰʧ@ �֪� �s�J�Ȧsg_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Valve2Temp(int QTeach_PGNo,int j)
{
	/*======================================= �B�J�Ǹ� STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[j].Step);
	/*======================================= �ʧ@���� TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Valve_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Valve_P[j].Type);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Valve_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Valve_P[j].Num);
	/*======================================= �Ѽ�1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Valve_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P1);
	/*======================================= �Ѽ�2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Valve_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P2);
	/*======================================= �Ѽ�3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Valve_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P3);
	/*======================================= �Ѽ�4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Valve_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P4);
	/*======================================= �Ѽ�5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Valve_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Valve_P[j].P5);
}

/*---------------------------------------------------------------------------+
|  Function : SaveAct_Valve2Temp()                  												 |
|       		: �N�ֳt�оɰʧ@ �˴� �s�J�Ȧsg_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Detect2Temp(int QTeach_PGNo,int j)
{
	/*======================================= �B�J�Ǹ� STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[j].Step);
	/*======================================= �ʧ@���� TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Detect_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Detect_P[j].Type);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Detect_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Detect_P[j].Num);
	/*======================================= �Ѽ�1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Detect_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P1);
	/*======================================= �Ѽ�2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Detect_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P2);
	/*======================================= �Ѽ�3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Detect_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P3);
	/*======================================= �Ѽ�4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Detect_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P4);
	/*======================================= �Ѽ�5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Detect_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Detect_P[j].P5);
}
/*---------------------------------------------------------------------------+
|  Function : SaveAct_Wait2Temp()                  												 |
|       		: �N�ֳt�оɰʧ@ ���� �s�J�Ȧsg_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Wait2Temp(int QTeach_PGNo,int j)
{
	printf("SaveAct_Wait2Temp(%d,%d)\n",QTeach_PGNo,j);
	/*======================================= �B�J�Ǹ� STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Wait_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Wait_P[j].Step);
	/*======================================= �ʧ@���� TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Wait_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Wait_P[j].Type);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Wait_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Wait_P[j].Num);
	/*======================================= �Ѽ�1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Wait_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P1);
	/*======================================= �Ѽ�2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Wait_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P2);
	/*======================================= �Ѽ�3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Wait_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P3);
	/*======================================= �Ѽ�4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Wait_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P4);
	/*======================================= �Ѽ�5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Wait_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Wait_P[j].P5);
}


/*---------------------------------------------------------------------------+
|  Function : SaveAct_Pile2Temp()                  													 |
|       		: �N�ֳt�оɰʧ@ ���| �s�J�Ȧsg_QTeach_Action_P									 |
+---------------------------------------------------------------------------*/
void	SaveAct_Pile2Temp(int QTeach_PGNo,int j)
{
	/*======================================= �B�J�Ǹ� STEP =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Step = Action_Axis_P[j].Step;
	printf("Save g_QTeach_Action_P[%d].Step = %d\n",QTeach_PGNo-1,Action_Axis_P[j].Step);
	/*======================================= �ʧ@���� TYPE =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Type = Action_Pile_P[j].Type;
	printf("Save g_QTeach_Action_P[%d].Type = %d\n",QTeach_PGNo-1,Action_Pile_P[j].Type);
	/*======================================= �ʧ@�Φ� NUM =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].Num = Action_Pile_P[j].Num;
	printf("Save g_QTeach_Action_P[%d].Num = %d\n",QTeach_PGNo-1,Action_Pile_P[j].Num);
	/*======================================= �Ѽ�1 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P1 = Action_Pile_P[j].P1;
	//printf("Save g_QTeach_Action_P[%d].P1 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P1);
	/*======================================= �Ѽ�2 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P2 = Action_Pile_P[j].P2;
	//printf("Save g_QTeach_Action_P[%d].P2 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P2);
	/*======================================= �Ѽ�3 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P3 = Action_Pile_P[j].P3;
	//printf("Save g_QTeach_Action_P[%d].P3 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P3);
	/*======================================= �Ѽ�4 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P4 = Action_Pile_P[j].P4;
	//printf("Save g_QTeach_Action_P[%d].P4 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P4);
	/*======================================= �Ѽ�5 =======================================*/
	g_QTeach_Action_P[QTeach_PGNo-1].P5 = Action_Pile_P[j].P5;
	//printf("Save g_QTeach_Action_P[%d].P5 = %d\n",QTeach_PGNo-1,Action_Pile_P[j].P5);
}


/*---------------------------------------------------------------------------+
|  Function : UnLockBtn()                  																	 |
|       		: �N	pwnd �������																							 |
+---------------------------------------------------------------------------*/
void	UnLockBtn(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{
		((CtmFormView*)pwnd)->OnLoseFocus();
		pwnd->SetPropValueT("enabled",double(TRUE));
		pwnd->CreateA();
		pwnd->Update();
	}
}
/*---------------------------------------------------------------------------+
|  Function : LockBtn()                  																		 |
|       		: �N	pwnd ����W��																							 |
+---------------------------------------------------------------------------*/
void	LockBtn(CtmWnd* pwnd)
{
	if(pwnd != NULL)
	{		
		((CtmFormView*)pwnd)->OnLoseFocus();
		pwnd->SetPropValueT("enabled",double(FALSE));
		pwnd->CreateA();
		pwnd->Update();
	}
}
/*---------------------------------------------------------------------------+
|  Function : SetFromWaitP()                  															 |
|       		: ���WaitP�I ����m �t�� ���� �Ѽ�															 |
+---------------------------------------------------------------------------*/
void	SetFromWaitP(int WhichAxis)
{
	int value =0;
	// ��m
	if(pwndQTeach_Pos[WhichAxis]!=NULL)
	{
		value= GetDBValue(P2_POSSET_DBString[WhichAxis]).lValue; // ���o WaitP DB��
		SetDBValue(P4_POSSET_DBString[WhichAxis], value); // �g�J ReadyDownP db
	}
	// �t��
	if(pwndQTeach_Speed[WhichAxis]!=NULL)
	{
		value= GetDBValue(P2_Speed_DBString[WhichAxis]).lValue; // ���o WaitP DB��
		SetDBValue(P4_Speed_DBString[WhichAxis], value); // �g�J ReadyDownP db
	}
	// ����
	if(pwndQTeach_DT[WhichAxis]!=NULL)
	{
		value= GetDBValue(P2_DT_DBString[WhichAxis]).lValue; // ���o WaitP DB��
		SetDBValue(P4_DT_DBString[WhichAxis], value); // �g�J ReadyDownP db
	}
}
		
/*---------------------------------------------------------------------------+
|  Function : Update_PosHint(int Axis)               												 |
|       		: ��s ��m����																									 |
+---------------------------------------------------------------------------*/
void	Update_PosHint(int Axis)
{
	printf("Update_PosHint(%d)\n",Axis);
	if(pwndImg_PosHint[Axis]!=NULL)
	{
		long Max_value=0,Min_value=0;
		char	MAX_DBID[256];	memset(MAX_DBID, 0, sizeof(MAX_DBID));
		if(pwndQTeach_Pos[Axis]!=NULL)
			pwndQTeach_Pos[Axis]->GetPropValueT("maxid", MAX_DBID, sizeof(MAX_DBID));
		Max_value = GetDBValue(MAX_DBID).lValue;
		gQTeach_PosLimt[Axis][MAX]=Max_value; // ������m�]�w���̤j�ȤW��
		char	MIN_DBID[256];	memset(MIN_DBID, 0, sizeof(MIN_DBID));
		pwndQTeach_Pos[Axis]->GetPropValueT("minid", MIN_DBID, sizeof(MIN_DBID));
		Min_value = GetDBValue(MIN_DBID).lValue;			
		gQTeach_PosLimt[Axis][MIN]=Min_value; // ������m�]�w���̤p�ȤW��
		
		pwndMask_PosHint[Axis]->Show(); // �B�n
		if(AxisPosNow[Axis]>Max_value) // �{�b��m�W�L�̤j��
		{
			pwndImg_PosHint[Axis]->SetPropValueT("imagepath","res_tm640/pic/picker/PosHint_NO.bmp");
			pwndImg_PosHint[Axis]->SetPropValueT("left",317+(80)); // �Х� ��ܦ�m
			pwndImg_PosHint[Axis]->SetPropValueT("right",317+(80)+20); // �Х� ��ܦ�m
		}
		else if(AxisPosNow[Axis]<Min_value) // �{�b��m �p��̤p��
		{
			pwndImg_PosHint[Axis]->SetPropValueT("imagepath","res_tm640/pic/picker/PosHint_NO.bmp");
			pwndImg_PosHint[Axis]->SetPropValueT("left",317); // �Х� ��ܦ�m
			pwndImg_PosHint[Axis]->SetPropValueT("right",317+20); // �Х� ��ܦ�m
		}
		else // �X�z����m
		{
			if((Max_value-Min_value)!=0)
			{
				pwndImg_PosHint[Axis]->SetPropValueT("imagepath","res_tm640/pic/picker/PosHint_OK.bmp");
				pwndImg_PosHint[Axis]->SetPropValueT("left",317+((AxisPosNow[Axis]-Min_value)*80/(Max_value-Min_value))); // �Х� ��ܦ�m
				pwndImg_PosHint[Axis]->SetPropValueT("right",317+((AxisPosNow[Axis]-Min_value)*80/(Max_value-Min_value))+20); // �Х� ��ܦ�m
			}
		}
		pwndImg_PosHint[Axis]->CreateA();
		pwndImg_PosHint[Axis]->Show();
		
		if(pwndQTeach_Pos_Max[Axis]!=NULL) // ��m�W�U�� ��ܤ�r
		{
			char StrValue[10]="\0";
			memset(StrValue, 0, sizeof(StrValue));
			sprintf(StrValue,"%d"".""%d", (Max_value/100), (Max_value%100)); // 2��p��
			pwndQTeach_Pos_Max[Axis]->SetPropValueT("text",StrValue);
			pwndQTeach_Pos_Max[Axis]->CreateA();
			pwndQTeach_Pos_Max[Axis]->Show();
		}
		if(pwndQTeach_Pos_Min[Axis]!=NULL) // ��m�W�U�� ��ܤ�r
		{
			char StrValue[10]="\0";
			memset(StrValue, 0, sizeof(StrValue));
			sprintf(StrValue,"%d"".""%d", (Min_value/100), (Min_value%100)); // 2��p��
			pwndQTeach_Pos_Min[Axis]->SetPropValueT("text",StrValue);
			pwndQTeach_Pos_Min[Axis]->CreateA();
			pwndQTeach_Pos_Min[Axis]->Show();
		}
	}
	

}
/*---------------------------------------------------------------------------+
|  Function : Update_Check_Move()               														 |
|       		: ��s ���ʫ�U�� �]�w����																			 |
+---------------------------------------------------------------------------*/
void	Update_Check_Move() // ��s ���ʫ�U�� �]�w����	
{
	// Axis_X1
	if( (pwndQTeach_Pos[Axis_X1]!=NULL) && (pwndQTeach_Speed[Axis_X1]!=NULL) && (pwndQTeach_DT[Axis_X1]!=NULL) && (pwndBtn_GetPos[Axis_X1]!=NULL) )
	{
		if(b_Check_Move)
		{
			//b_All_PosSet_OK=0; // �ݳ]�w��m
			UnLockBtn(pwndQTeach_Pos[Axis_X1]);
			UnLockBtn(pwndQTeach_Speed[Axis_X1]);
			UnLockBtn(pwndQTeach_DT[Axis_X1]);
			UnLockBtn(pwndBtn_GetPos[Axis_X1]);
		}
		else
		{
			//b_All_PosSet_OK=OK; // ��m�]�w����
			LockBtn(pwndQTeach_Pos[Axis_X1]);
			LockBtn(pwndQTeach_Speed[Axis_X1]);
			LockBtn(pwndQTeach_DT[Axis_X1]);
			LockBtn(pwndBtn_GetPos[Axis_X1]);
			SetFromWaitP(Axis_X1);// ���� WaitP X1���]�w
		}
	}
	// Axis_X2
	if( (pwndQTeach_Pos[Axis_X2]!=NULL) && (pwndQTeach_Speed[Axis_X2]!=NULL) && (pwndQTeach_DT[Axis_X2]!=NULL) && (pwndBtn_GetPos[Axis_X2]!=NULL) )
	{
		if(b_Check_Move)
		{
			//b_All_PosSet_OK=0; // �ݳ]�w��m
			UnLockBtn(pwndQTeach_Pos[Axis_X2]);
			UnLockBtn(pwndQTeach_Speed[Axis_X2]);
			UnLockBtn(pwndQTeach_DT[Axis_X2]);
			UnLockBtn(pwndBtn_GetPos[Axis_X2]);
		}
		else
		{
			//b_All_PosSet_OK=OK; // ��m�]�w����
			LockBtn(pwndQTeach_Pos[Axis_X2]);
			LockBtn(pwndQTeach_Speed[Axis_X2]);
			LockBtn(pwndQTeach_DT[Axis_X2]);
			LockBtn(pwndBtn_GetPos[Axis_X2]);
			SetFromWaitP(Axis_X2);// ���� WaitP X2���]�w
		}
	}
	if(pwndCheck_Move!=NULL)
	{
		pwndCheck_Move->SetPropValueT("upbitmap",CheckBox_ImgPath[b_Check_Move]);
		pwndCheck_Move->CreateA();
		pwndCheck_Move->Update();
	}
}

/*---------------------------------------------------------------------------+
|  Function : GetDownDis()             							  											 |
|       		: �p�� �U���Z��																									 |
+---------------------------------------------------------------------------*/
void	GetDownDis() // �p�� �U���Z��
{
	int DownDis=0; //�U���Z��
	int u_WaitPos_Y1 		 = 0; // �����I Y1 �]�w��m
	int u_PickDownPos_Y1 = 0; // ���X�U���I Y1 �]�w��m

	u_WaitPos_Y1 = GetDBValue(P2_POSSET_DBString[Axis_Y1]).lValue;
	printf("u_WaitPos_Y1=%d\n",u_WaitPos_Y1);
	u_PickDownPos_Y1 = GetDBValue(P5_POSSET_DBString[Axis_Y1]).lValue;
	printf("u_PickDownPos_Y1=%d\n",u_PickDownPos_Y1);
	DownDis = u_PickDownPos_Y1-u_WaitPos_Y1;
	printf("DownDis=%d\n",DownDis);
	if(pwndPick_SLVL!=NULL)
		pwndPick_SLVL->SetPropValueT("max",DownDis);
	if(pwndPick_Dis!=NULL)
		{
			pwndPick_Dis->SetPropValueT("value",DownDis);
			printf("Set pwndPick_Dis=%d\n",DownDis);
			pwndPick_Dis->CreateA();
			pwndPick_Dis->Show();
		}
}


/*---------------------------------------------------------------------------+
|  Function : GetBackDis()             							  											 |
|       		: �p�� ��h�Z��																									 |
+---------------------------------------------------------------------------*/
void	GetBackDis() // �p�� ��h�Z��
{
	// �p�� ��h�Z��
	int u_BackDis				 = 0; // ��h�Z��
	int u_MoveOutPos_X1	 = 0; // ��X�I X1 �]�w��m
	int u_PickPos_X1 		 = 0; // ���X�����I X1 �]�w��m
	u_MoveOutPos_X1 = GetDBValue(P7_POSSET_DBString[Axis_X1]).lValue;

	u_PickPos_X1 = GetDBValue(P6_POSSET_DBString[Axis_X1]).lValue;

	u_BackDis = abs(u_MoveOutPos_X1-u_PickPos_X1);
	printf("u_BackDis=%d\n",u_BackDis);
	if(pwndPickOut_SLVL!=NULL)
		pwndPickOut_SLVL->SetPropValueT("max",u_BackDis);
	if(pwndPickOut_Dis!=NULL)
	{
		pwndPickOut_Dis->SetPropValueT("value",u_BackDis);
		pwndPickOut_Dis->CreateA();
		pwndPickOut_Dis->Show();
	}
}
/*---------------------------------------------------------------------------+
|  Function : SaveSLVLData()                  		   									       |
|       		: �����e �X�� �]�w�ƭ�	�s�JDB																 |
+---------------------------------------------------------------------------*/
void	SaveSLVLData() // �����e �X�� �]�w�ƭ�	�s�JDB	
{
	printf("SaveSLVLData!!!!!!!!!!!!!!!!!!!!!\n");
	long value = 0;
	// ����X��
	if(pwndPick_SLVL!=NULL)
	{
		pwndPick_SLVL->GetPropValueT("value", &value,sizeof(value));
		SetDBValue(Pick_SLVL_DB,value); // �ƭ� �g�Jdb
		printf("Set %s=%d\n",Pick_SLVL_DB,value);
	}
	
	// ���X�X��
	if(pwndPickOut_SLVL!=NULL)
	{
		pwndPickOut_SLVL->GetPropValueT("value", &value,sizeof(value));
		SetDBValue(PickOut_SLVL_DB,value); // �ƭ� �g�Jdb
		printf("Set %s=%d\n",PickOut_SLVL_DB,value);
	}
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