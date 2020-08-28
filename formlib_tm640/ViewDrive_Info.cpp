/*===========================================================================+
|  Class    : ViewCore library                                          	 |
|  Task     : ViewCore library action source file                       	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewDrive_Info.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		REQ_READMOTOR    				2   
#define		MechType3				0	 // �T�b
#define		MechType5				1  // ���b
#define		EncType_Abs			0x00000000  // ����
#define		EncType_Res		  0x00010000  // �W�q
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//�Q��update�Ȱ���@��

int u_Axis_Num = 1;
CtmWnd*		pwndDriveInfoSTR[11] 	={NULL}; 	
int iDriveInfoSTR = 0;

CtmWnd*		pwndIMG_Axis  		= NULL;

CtmWnd*		pwndBtn_Axis[5] 	={NULL}; 
int iBtn_Axis = 0;


DWORD dw_InfoValue[11] = {0};

char* u_pszStrID_X1[] = 
{
	"MACHINE_INTERFACE_X1_DRIVE_ID",        // X1_�X�ʾ�ID
	"MACHINE_INTERFACE_X1_LAST_ALARMID",	  // X1_�̪�@��ĵ���N�X
	"MACHINE_INTERFACE_X1_CURRENTLOOP_KP",  // X1_�q�y��KP
	"MACHINE_INTERFACE_X1_CURRENTLOOP_KI",  // X1_�q�y��KI
	"MACHINE_INTERFACE_X1_SPEEDLOOP_KP",    // X1_�t����KP
	"MACHINE_INTERFACE_X1_SPEEDLOOP_KI",    // X1_�t����KI
	"MACHINE_INTERFACE_X1_BUS_V",           // X1_���u�q��
	"MACHINE_INTERFACE_X1_DRIVE_TMP",       // X1_�X�ʾ��ū�
	"MACHINE_INTERFACE_X1_MOTOR_TMP",       // X1_���F�ū�
	"MACHINE_INTERFACE_X1_DRIVE_I",         // X1_�X�ʾ��q�y
	"MACHINE_INTERFACE_X1_MOTOR_RPM",       // X1_���F��t
};
char* u_pszStrID_Y1[] = 
{
	"MACHINE_INTERFACE_Y1_DRIVE_ID",        // Y1_�X�ʾ�ID
	"MACHINE_INTERFACE_Y1_LAST_ALARMID",	  // Y1_�̪�@��ĵ���N�X
	"MACHINE_INTERFACE_Y1_CURRENTLOOP_KP",  // Y1_�q�y��KP
	"MACHINE_INTERFACE_Y1_CURRENTLOOP_KI",  // Y1_�q�y��KI
	"MACHINE_INTERFACE_Y1_SPEEDLOOP_KP",    // Y1_�t����KP
	"MACHINE_INTERFACE_Y1_SPEEDLOOP_KI",    // Y1_�t����KI
	"MACHINE_INTERFACE_Y1_BUS_V",           // Y1_���u�q��
	"MACHINE_INTERFACE_Y1_DRIVE_TMP",       // Y1_�X�ʾ��ū�
	"MACHINE_INTERFACE_Y1_MOTOR_TMP",       // Y1_���F�ū�
	"MACHINE_INTERFACE_Y1_DRIVE_I",         // Y1_�X�ʾ��q�y
	"MACHINE_INTERFACE_Y1_MOTOR_RPM",       // Y1_���F��t
};
char* u_pszStrID_Z[] = 
{
	"MACHINE_INTERFACE_Z_DRIVE_ID",        // Z_�X�ʾ�ID
	"MACHINE_INTERFACE_Z_LAST_ALARMID",	 	 // Z_�̪�@��ĵ���N�X
	"MACHINE_INTERFACE_Z_CURRENTLOOP_KP",  // Z_�q�y��KP
	"MACHINE_INTERFACE_Z_CURRENTLOOP_KI",  // Z_�q�y��KI
	"MACHINE_INTERFACE_Z_SPEEDLOOP_KP",    // Z_�t����KP
	"MACHINE_INTERFACE_Z_SPEEDLOOP_KI",    // Z_�t����KI
	"MACHINE_INTERFACE_Z_BUS_V",           // Z_���u�q��
	"MACHINE_INTERFACE_Z_DRIVE_TMP",       // Z_�X�ʾ��ū�
	"MACHINE_INTERFACE_Z_MOTOR_TMP",       // Z_���F�ū�
	"MACHINE_INTERFACE_Z_DRIVE_I",         // Z_�X�ʾ��q�y
	"MACHINE_INTERFACE_Z_MOTOR_RPM",       // Z_���F��t
};
char* u_pszStrID_X2[] = 
{
	"MACHINE_INTERFACE_X2_DRIVE_ID",        // X2_�X�ʾ�ID
	"MACHINE_INTERFACE_X2_LAST_ALARMID",	  // X2_�̪�@��ĵ���N�X
	"MACHINE_INTERFACE_X2_CURRENTLOOP_KP",  // X2_�q�y��KP
	"MACHINE_INTERFACE_X2_CURRENTLOOP_KI",  // X2_�q�y��KI
	"MACHINE_INTERFACE_X2_SPEEDLOOP_KP",    // X2_�t����KP
	"MACHINE_INTERFACE_X2_SPEEDLOOP_KI",    // X2_�t����KI
	"MACHINE_INTERFACE_X2_BUS_V",           // X2_���u�q��
	"MACHINE_INTERFACE_X2_DRIVE_TMP",       // X2_�X�ʾ��ū�
	"MACHINE_INTERFACE_X2_MOTOR_TMP",       // X2_���F�ū�
	"MACHINE_INTERFACE_X2_DRIVE_I",         // X2_�X�ʾ��q�y
	"MACHINE_INTERFACE_X2_MOTOR_RPM",       // X2_���F��t
};
char* u_pszStrID_Y2[] = 
{
	"MACHINE_INTERFACE_Y2_DRIVE_ID",        // Y2_�X�ʾ�ID
	"MACHINE_INTERFACE_Y2_LAST_ALARMID",	  // Y2_�̪�@��ĵ���N�X
	"MACHINE_INTERFACE_Y2_CURRENTLOOP_KP",  // Y2_�q�y��KP
	"MACHINE_INTERFACE_Y2_CURRENTLOOP_KI",  // Y2_�q�y��KI
	"MACHINE_INTERFACE_Y2_SPEEDLOOP_KP",    // Y2_�t����KP
	"MACHINE_INTERFACE_Y2_SPEEDLOOP_KI",    // Y2_�t����KI
	"MACHINE_INTERFACE_Y2_BUS_V",           // Y2_���u�q��
	"MACHINE_INTERFACE_Y2_DRIVE_TMP",       // Y2_�X�ʾ��ū�
	"MACHINE_INTERFACE_Y2_MOTOR_TMP",       // Y2_���F�ū�
	"MACHINE_INTERFACE_Y2_DRIVE_I",         // Y2_�X�ʾ��q�y
	"MACHINE_INTERFACE_Y2_MOTOR_RPM",       // Y2_���F��t
};

/*--------------------------------------------------+
| dw_MechType �������  														|
|	Low WORD 0-�T�b 1-���b 	High WORD 0-���� 1-�W�q		|
| EX: ����/���b 0x0000 0001;  0x0001 0000 �W�q/�T�b |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //���� pMechTypeDB ���ƭ�
int u_PickerType 	= 0; // ������� 0-�T�b 1-���b
int u_EncType 		= 0; // �s�X�����  0-���� 1-�W�q
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b
char* u_pszImgDriveInfo[] = 
{
	"res_tm640/pic/DriveInfo_Axis3_Axis",   // 
	"res_tm640/pic/DriveInfo_Axis5_Axis",	  // 
};
/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("Drive_Info\n");
	SetDBValue("MACHINE_CONFIGURATION_IO_REFERENCE_POINT", 0x01); // �e28�R�O �}�l���X�ʾ��n���
	
	iDriveInfoSTR	 	= GetSpecialControlNum(pwndSender, "STR", "CtmStaticX2", pwndDriveInfoSTR);
	
	pwndIMG_Axis    = pwndSender->FindControlFromName("IMG_Axis");
	
	iBtn_Axis	 	= GetSpecialControlNum(pwndSender, "Btn_Axis_", "CtmToolButton", pwndBtn_Axis);
	printf("iBtn_Axis=%d\n",iBtn_Axis);
	
	// Ū���]�w ������� �T���b
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	u_EncType		 = dw_MechType & EncWord;
	
	// �U�b�ܷN��
	char szImgPath[256];
	sprintf(szImgPath, "%s%d.bmp",u_pszImgDriveInfo[u_PickerType],u_Axis_Num);
	pwndIMG_Axis->SetPropValueT("imagepath",szImgPath);
	pwndIMG_Axis->Show();
	
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
//	struct timeval tv;
//	static struct timeval tv_old;
//	static int msg_last;
//	long lagtime=0;
//	
//	gettimeofday(&tv,NULL);
//  lagtime = (tv.tv_usec - tv_old.tv_usec)/1000;
//  printf("Update time = %d\n",lagtime);
//  
//	gettimeofday(&tv_old,NULL);
	
	if(!RunOnlyOne)
	{
		RunOnlyOne=TRUE;
		
		int		nTime 	= 0;
		
	  if(g_ptaskpicker != NULL)
		{
			WORD		wNum = 0;
			switch(u_Axis_Num)
			{
				case 1: // X1
					wNum = sizeof(u_pszStrID_X1)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X1, &nTime); // Update Data from 28
					break;
				case 2: // Y1
					wNum = sizeof(u_pszStrID_Y1)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y1, &nTime); // Update Data from 28
					break;
				case 3: // Z
					wNum = sizeof(u_pszStrID_Z)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Z, &nTime); // Update Data from 28
					break;
				case 4: // X2
					wNum = sizeof(u_pszStrID_X2)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X2, &nTime); // Update Data from 28
					break;
				case 5: // Y2
					wNum = sizeof(u_pszStrID_Y2)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y2, &nTime); // Update Data from 28
					break;
				default:
					break;
			}		
		}	
	}
	


	for(int i=0;i<iDriveInfoSTR;i++) // Update String value
	{
		DWORD dw;
		float f_value;
		switch(u_Axis_Num) // Get Data from DB
		{
			case 1: // X1
				dw = GetDBValue(u_pszStrID_X1[i]).lValue;
				break;
			case 2: // Y1
				dw = GetDBValue(u_pszStrID_Y1[i]).lValue;
				break;
			case 3: // Z
				dw = GetDBValue(u_pszStrID_Z[i]).lValue;
				break;
			case 4: // X2
				dw = GetDBValue(u_pszStrID_X2[i]).lValue;
				break;
			case 5: // Y2
				dw = GetDBValue(u_pszStrID_Y2[i]).lValue;
				break;
			default:
				break;
		}	
		
		char  str_temp[256] = "\0";
		if(i>=2)
		{
			if(u_EncType==EncType_Abs) // ���﫬 ENC for AUSTONE
			{
				if(i!=10)
				{
					f_value = Conv2Float(dw);
					if(f_value >= 65535)
						sprintf(str_temp ,"NULL");
					else
						sprintf(str_temp ,"%.02f",f_value);
				}
				else
					sprintf(str_temp ,"%d",dw);	
			}
			else if(u_EncType==EncType_Res) // �W�q�� for HDT
			{
				if(i==6) // ���u�q��
					dw = dw/1000;
				sprintf(str_temp ,"%d",dw);						
			}
		}
		else
		{
			sprintf(str_temp ,"%d",dw);
			if(u_EncType==EncType_Res && (i==1)) // �W�q�� for HDT // �̪�@��ĵ��
				sprintf(str_temp ,"%x",dw);		
		}
		
		if(pwndDriveInfoSTR[i]!=NULL) // Update String
		{
			pwndDriveInfoSTR[i]->SetPropValueT("text",str_temp);
			pwndDriveInfoSTR[i]->Update();
		}
	}
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//printf("OnKey\n");
	
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
			break;	
		case 2:
    	u_Axis_Num=2;
			break;	
		case 3:
			u_Axis_Num=3;
			break;
		case 4:
			u_Axis_Num=4;
			break;
		case 5:
			u_Axis_Num=5;
			break;
		default:
			break;
	}
	
	if(pwndBtn_Axis[u_Axis_Num-1]) // ������b����
	{	
		pwndBtn_Axis[u_Axis_Num-1]->SetPropValueT("upbitmap","res_tm640/pic/ButtonDown1.bmp");
		pwndBtn_Axis[u_Axis_Num-1]->CreateA();
		pwndBtn_Axis[u_Axis_Num-1]->Update();
	}
	
	char szImagePath[256]; // �U�b�ܷN��
	sprintf(szImagePath, "%s%d.bmp",u_pszImgDriveInfo[u_PickerType],u_Axis_Num);
	pwndIMG_Axis->SetPropValueT("imagepath",szImagePath);
	pwndIMG_Axis->Show();

	 if(g_ptaskpicker != NULL)
	{
		WORD		wNum = 0;
		int		nTime 	= 0;
		switch(u_Axis_Num)
		{
			case 1: // X1
				wNum = sizeof(u_pszStrID_X1)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X1, &nTime); // Update Data from 28
				break;
			case 2: // Y1
				wNum = sizeof(u_pszStrID_Y1)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y1, &nTime); // Update Data from 28
				break;
			case 3: // Z
				wNum = sizeof(u_pszStrID_Z)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Z, &nTime); // Update Data from 28
				break;
			case 4: // X2
				wNum = sizeof(u_pszStrID_X2)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X2, &nTime); // Update Data from 28
				break;
			case 5: // Y2
				wNum = sizeof(u_pszStrID_Y2)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y2, &nTime); // Update Data from 28
				break;
			default:
				break;
		}		
	}
	
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
 	
//	DWORD dw = GetDBValue("MACHINE_INTERFACE_X1_CURRENTLOOP_KP").lValue;
//	printf("dw=%d\n",dw);
//	float f_CL_KP = Conv2Float(dw);
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	SetDBValue("MACHINE_CONFIGURATION_IO_REFERENCE_POINT", 0);// ���}�e0 �D���P�_
}

float	Conv2Float(DWORD dw)
{
	float fd = 0;
	::memcpy((void*)&fd, (void*)&dw, 4);
	//printf("dw=%d\n",dw);
	//printf("fd=%f\n",fd);
	return fd;
}
