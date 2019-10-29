/*===========================================================================+
|  Class    : ViewZoneLimit library                                          |
|  Task     : ViewZoneLimit library action source file                       |
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
#include 	"ViewZoneLimit.h"
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
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		NULL										0

#define		REQ_READMOTOR    				2  
#define		REQ_WRITEMOTOR    			3
#define		CONST_REQ_COMMAND   6
#define		SET_SAFE_DIST		0xFF46

#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//�Q��update�Ȱ���@��

int u_Axis_Num = 1;
CtmWnd*		pwdImg_ZoneLimit  = NULL; // ��ܹϤ�

CtmWnd*		pwndBtn_Axis[6] 	={NULL}; // �U�bBtn
int iBtn_Axis = 0;

CtmWnd*		pwndStr_ZoneLimit[3] 	={NULL}; // �U�b��� ��r
int uZoneLimit_Str = 0;
CtmWnd*		pwndData_ZoneLimit[3] 	={NULL}; // �U�b �Ѽ�
int uZoneLimit_Data = 0;

CtmWnd*			pwndBtnSafeDist = NULL; // �w���Z�� Btn
CtmWnd*			pwndEdit_SafeDist = NULL; // �w���Z�� Edit
CtmWnd*			pwndUnit_SafeDist = NULL; // �w���Z�� Unit
CtmWnd*			pwndMask_SafeDist = NULL; // �w���Z�� Mask

int	 nEditNum 	= 0;

char* Str_ZoneLimit[6][3] = // ��ܤ�r
{
	{"VW_MAXPOSLIMIT","PICKER_SAFEDIST_BUFFER","POS_TOLERANCE"},	// �@�P
	{"ZONELIMIT_Zone","ZONELIMIT_X1_MIN","ZONELIMIT_X1_MAX"},			// X1�b
	{"ZONELIMIT_Zone","ZONELIMIT_Y1_MIN","ZONELIMIT_Y1_MAX"},			// Y1�b
	{"ZONELIMIT_Zone","ZONELIMIT_Z_MIN","ZONELIMIT_Z_MAX"},				// Z�b
	{"ZONELIMIT_Zone","ZONELIMIT_X2_MIN","ZONELIMIT_X2_MAX"},			// X2�b
	{"ZONELIMIT_Zone","ZONELIMIT_Y2_MIN","ZONELIMIT_Y2_MAX"}			// Y2�b
};

char* dbid0_ZoneLimit[6][3] = // �ѼƼƭ� db
{
	{"SYSX_OTHERS_OTHERS_DWORD_RESERVED4","MACHINE_LIMITS_AXIS1_CONTROLPRECISION","MACHINE_LIMITS_AXIS2_CONTROLPRECISION"},	// �@�P
	{"MACHINE_LIMITS_AXIS1_MAXPOSTION","MACHINE_LIMITS_AXIS_X1_MINPOSTION","MACHINE_LIMITS_AXIS_X1_MAXPOSTION"},						// X1�b
	{"MACHINE_LIMITS_AXIS2_MAXPOSTION","MACHINE_LIMITS_AXIS_Y1_MINPOSTION","MACHINE_LIMITS_AXIS_Y1_MAXPOSTION"},						// Y1�b
	{"MACHINE_LIMITS_AXIS3_MAXPOSTION","MACHINE_LIMITS_AXIS_Z_MINPOSTION","MACHINE_LIMITS_AXIS_Z_MAXPOSTION"},							// Z�b
	{"MACHINE_LIMITS_AXIS4_MAXPOSTION","MACHINE_LIMITS_AXIS_X2_MINPOSTION","MACHINE_LIMITS_AXIS_X2_MAXPOSTION"},						// X2�b
	{"MACHINE_LIMITS_AXIS5_MAXPOSTION","MACHINE_LIMITS_AXIS_Y2_MINPOSTION","MACHINE_LIMITS_AXIS_Y2_MAXPOSTION"}							// Y2�b
};

char* maxid_ZoneLimit[6][3] = // �Ѽ� maxid db
{
	{NULL,NULL,NULL},	// �@�P
	{NULL,"MACHINE_LIMITS_AXIS_X1_MAXPOSTION","MACHINE_LIMITS_AXIS1_MAXPOSTION"},				// X1�b
	{NULL,"MACHINE_LIMITS_AXIS2_MAXPOSTION","SYSX_OTHERS_OTHERS_DWORD_RESERVED4"},			// Y1�b
	{NULL,"MACHINE_LIMITS_AXIS_Z_MAXPOSTION","MACHINE_LIMITS_AXIS3_MAXPOSTION"},				// Z�b
	{NULL,"MACHINE_LIMITS_AXIS_X2_MAXPOSTION","MACHINE_LIMITS_AXIS4_MAXPOSTION"},				// X2�b
	{NULL,"MACHINE_LIMITS_AXIS_Y2_MAXPOSTION","MACHINE_LIMITS_AXIS5_MAXPOSTION"}				// Y2�b
};
int max_ZoneLimit[]
{
	20000,60000,200,
};
int min_ZoneLimit[]
{
	0,0,0,
};

char* minid_ZoneLimit[6][3] = // �Ѽ� minid db
{
	{NULL,NULL,NULL},	// �@�P
	{NULL,NULL,"MACHINE_LIMITS_AXIS_X1_MINPOSTION"},						// X1�b
	{NULL,NULL,"MACHINE_LIMITS_AXIS_Y1_MINPOSTION"},						// Y1�b
	{NULL,NULL,"MACHINE_LIMITS_AXIS_Z_MINPOSTION"},							// Z�b
	{NULL,NULL,"MACHINE_LIMITS_AXIS_X2_MINPOSTION"},						// X2�b
	{NULL,NULL,"MACHINE_LIMITS_AXIS_X2_MINPOSTION"}							// Y2�b
};

/*--------------------------------------------------+
| dw_MechType �������  														|
|	Low WORD 0-�T�b 1-���b 	High WORD 0-���� 1-�W�q		|
| EX: ����/���b 0x0000 0001;  0x0001 0000 �W�q/�T�b |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //���� pMechTypeDB ���ƭ�
int u_PickerType = 0; // ������� 0-�T�b 1-���b
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // �������DB �T�b ���b
char* u_pszImgZoneLimit[] = // �U�b�ܷN��
{
	"res_tm640/pic/picker/ZoneLimit_Axis3_Axis",   // 
	"res_tm640/pic/picker/ZoneLimit_Axis5_Axis",	  // 
};


char* u_pszStrID[] = 
{
	"MACHINE_INTERFACE_ARM_SAFE_DIS",        // �D���u�w���Z��
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
	pwndBtnSafeDist = pwndSender->FindControlFromName("BtnSafeDist");
	pwndEdit_SafeDist = pwndSender->FindControlFromName("Edit_SafeDist");
	pwndUnit_SafeDist = pwndSender->FindControlFromName("Unit_SafeDist");
	pwndMask_SafeDist = pwndSender->FindControlFromName("Mask_SafeDist");
	
	pwdImg_ZoneLimit = pwndSender->FindControlFromName("Img_ZoneLimit");
	
	iBtn_Axis	 	= GetSpecialControlNum(pwndSender, "Btn_Axis_", "CtmToolButton", pwndBtn_Axis);
	printf("iBtn_Axis=%d\n",iBtn_Axis);
	
	uZoneLimit_Str	 	= GetSpecialControlNum(pwndSender, "ZoneLimit_Str", "CtmStaticX2", pwndStr_ZoneLimit);
	printf("uZoneLimit_Str=%d\n",uZoneLimit_Str);
	
	uZoneLimit_Data	 	= GetSpecialControlNum(pwndSender, "ZoneLimit_Data", "CtmEditX1", pwndData_ZoneLimit);
	printf("uZoneLimit_Data=%d\n",uZoneLimit_Data);
	
	// Ū���]�w ������� �T���b
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;

	// ��s��r
	for(int i=0;i<uZoneLimit_Str;i++) // Update String
	{
		if(pwndStr_ZoneLimit[i]!=NULL) // Update String
		{
			pwndStr_ZoneLimit[i]->SetPropValueT("textID",Str_ZoneLimit[u_Axis_Num][i]);
			pwndStr_ZoneLimit[i]->Update();
		}
	}
	UpdateData();
	DisableSaftDist();
	ShowSafeDist();
	
	// �U�b�ܷN��
	char szImgPath[256];
	sprintf(szImgPath, "%s%d.bmp",u_pszImgZoneLimit[u_PickerType],u_Axis_Num);
	printf("Set Img= %s\n",szImgPath);
	pwdImg_ZoneLimit->SetPropValueT("imagepath",szImgPath);
	pwdImg_ZoneLimit->Show();
	
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
	printf("OnChangeA\n");
	CtmWnd* pWnd = NULL;
	DWORD		dwTempValue = 0;

	pWnd = pwndSender->FindControlFromTab(wIDControl);
	if(pWnd == NULL) return wIDControl;		

	if(wIDControl==5)// �D���u�w���Z��
	{
		printf("Set SafeDist\n");
		if(pwndEdit_SafeDist!=NULL)
		{
			long value=0,max_value=0;
			pwndEdit_SafeDist->GetPropValueT("value", &value,sizeof(value));
			max_value = GetDBValue("MACHINE_LIMITS_AXIS1_MAXPOSTION").lValue;
			value = max_value-value;
			SetDBValue("MACHINE_INTERFACE_ARM_SAFE_DIS", value);
		}
		
		long data=0;
		data = GetDBValue("MACHINE_INTERFACE_ARM_SAFE_DIS").lValue;
		printf("data =%d\n",data);
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
WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA\n");
	((CtmFormView*)pwndSender)->OnLoseFocus(); // ��������
	BOOL Saftdist_Show=FALSE;
	for(int i=0;i<iBtn_Axis;i++) // �b�����ܦ�
	{
		pwndBtn_Axis[i]->SetPropValueT("upbitmap","res_tm640/pic/PileBtnUp.bmp");
		pwndBtn_Axis[i]->CreateA();
		pwndBtn_Axis[i]->Update();
	}
	
	switch(wKey)
	{
		case 1:
    	u_Axis_Num=1;Saftdist_Show=TRUE;
			break;	
		case 2:
    	u_Axis_Num=2;Saftdist_Show=TRUE;
			break;	
		case 3:
			u_Axis_Num=3;Saftdist_Show=TRUE;
			break;
		case 4:
			u_Axis_Num=4;Saftdist_Show=TRUE;
			break;
		case 5:
			u_Axis_Num=5;Saftdist_Show=TRUE;
			break;
		case 77:
			u_Axis_Num=0;Saftdist_Show=TRUE;
			break;
		default:
			break;
	}
	
	if(pwndBtn_Axis[u_Axis_Num]) // ������b����
	{	
		pwndBtn_Axis[u_Axis_Num]->SetPropValueT("upbitmap","res_tm640/pic/PileBtnSelect.bmp");
		pwndBtn_Axis[u_Axis_Num]->CreateA();
		pwndBtn_Axis[u_Axis_Num]->Update();
	}
	
	// ��s��r
	for(int i=0;i<uZoneLimit_Str;i++) // Update String
	{
		if(pwndStr_ZoneLimit[i]!=NULL) // Update String
		{
			pwndStr_ZoneLimit[i]->SetPropValueT("textID",Str_ZoneLimit[u_Axis_Num][i]);
			pwndStr_ZoneLimit[i]->Update();
		}
	}
	
	UpdateData(); // ��s��� �w�q
	
	// �D���u�w���Z�� ��s
	if(Saftdist_Show)
	{
		if(u_Axis_Num==0)
			EnableSaftDist();
		else if(u_Axis_Num>0 && u_Axis_Num<=5)
			DisableSaftDist();
	}
		
	// �U�b�ܷN��
	char szImgPath[256];
	sprintf(szImgPath, "%s%d.bmp",u_pszImgZoneLimit[u_PickerType],u_Axis_Num);
	printf("Set Img= %s\n",szImgPath);
	pwdImg_ZoneLimit->SetPropValueT("imagepath",szImgPath);
	pwdImg_ZoneLimit->Show();
	

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
	printf("OnMouseUp\n");
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
	
	if(pwnd == pwndBtnSafeDist)
	{
		SendCommand(SET_SAFE_DIST);
		
		if(g_ptaskpicker != NULL)
		{
			WORD		wNum = 0;
			wNum = sizeof(u_pszStrID)/sizeof(char*);
			g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID); // Update Data from 28
		}
		
		if(pwndEdit_SafeDist!=NULL)
			{
				ShowSafeDist();
				((CtmFormView*)pwndEdit_SafeDist)->OnLoseFocus(); // ��������
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
			g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
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
	for(int i=0;i<uZoneLimit_Data;i++) // Update Data def
	{
		if(pwndStr_ZoneLimit[i]!=NULL) // Update Data def
		{
			pwndData_ZoneLimit[i]->SetPropValueT("dbid0",dbid0_ZoneLimit[u_Axis_Num][i]);
			if (u_Axis_Num>0)
			{
				if(i!=0)
					pwndData_ZoneLimit[i]->SetPropValueT("maxid",maxid_ZoneLimit[u_Axis_Num][i]);
				else
					pwndData_ZoneLimit[i]->SetPropValueT("max",300000);
					
				if(i==2)
					pwndData_ZoneLimit[i]->SetPropValueT("minid",minid_ZoneLimit[u_Axis_Num][i]);
			}
			else // �@�P
			{
				pwndData_ZoneLimit[i]->SetPropValueT("maxid",maxid_ZoneLimit[u_Axis_Num][i]);
				pwndData_ZoneLimit[i]->SetPropValueT("max",max_ZoneLimit[i]); // �̤j��
				pwndData_ZoneLimit[i]->SetPropValueT("minid",maxid_ZoneLimit[u_Axis_Num][i]);
				pwndData_ZoneLimit[i]->SetPropValueT("min",min_ZoneLimit[i]); // �̤p��
			}
			pwndData_ZoneLimit[i]->CreateA();
			pwndData_ZoneLimit[i]->Update();
		}
	}
}

void EnableSaftDist()
{
	if(pwndMask_SafeDist!=NULL)
	{
		pwndMask_SafeDist->Update();
	}
	if(pwndBtnSafeDist!=NULL)
	{
		pwndBtnSafeDist->SetPropValueT("enabled",double(TRUE));
		pwndBtnSafeDist->Update();
	}
	if(pwndEdit_SafeDist!=NULL)
	{
		pwndEdit_SafeDist->SetPropValueT("enabled",double(TRUE));
		pwndEdit_SafeDist->Update();
	}
	if(pwndUnit_SafeDist!=NULL)
	{
		pwndUnit_SafeDist->SetPropValueT("enabled",double(TRUE));
		pwndUnit_SafeDist->Update();
	}
}

void DisableSaftDist()
{
	if(pwndBtnSafeDist!=NULL)
	{
		pwndBtnSafeDist->SetPropValueT("enabled",double(FALSE));
	}
	if(pwndEdit_SafeDist!=NULL)
	{
		pwndEdit_SafeDist->SetPropValueT("enabled",double(FALSE));
	}
	if(pwndUnit_SafeDist!=NULL)
	{
		pwndUnit_SafeDist->SetPropValueT("enabled",double(FALSE));
	}
	if(pwndMask_SafeDist!=NULL)
	{
		pwndMask_SafeDist->Update();
	}
}

void ShowSafeDist()
{
	if(pwndEdit_SafeDist!=NULL)
	{
		long value=0,max_value=0;
		value = GetDBValue("MACHINE_INTERFACE_ARM_SAFE_DIS").lValue;
		max_value = GetDBValue("MACHINE_LIMITS_AXIS1_MAXPOSTION").lValue;
		value = max_value-value;
		pwndEdit_SafeDist->SetPropValueT("value",value);
		pwndEdit_SafeDist->Update();
	}
}