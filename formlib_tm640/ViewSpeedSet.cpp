/*===========================================================================+
|  Class    : ViewSpeedSet library                                           |
|  Task     : ViewSpeedSet library action source file                        |
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
#include 	"ViewSpeedSet.h"
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

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//�Q��update�Ȱ���@��

int u_Axis_Num = 1;
CtmWnd*		pwndBtn_Axis[6] 	={NULL}; // �U�bBtn
int iBtn_Axis = 0;

CtmWnd*		pwndSpeed_Data[7] 	={NULL}; // �U�b �Ѽ� ����
char* Speed_Data_String[] = // �Ѽ� ���� �W��
{
	"edit_MaxSpeed", 	// �̰���t
	"edit_ACC_T", 	  // �[�t�ɶ�
	"edit_DCC_T",			// ��t�ɶ�
	"edit_ACC_Ratio", // �[�t���Ƥ��
	"edit_DCC_Ratio",	// ��t���Ƥ��
	"edit_POS_KP",	// ����T�� ��mKp
	"edit_SPD_KP",			// �l�H�T�� �t��Kp
};

char* dbid0_Speed[6][7] = // �ѼƼƭ� db [�b][�Ѽ�]
{	//
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL},	
	{ "MACHINE_INTERFACE_MAXSPEED_X1",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_X1",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_X1", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV1", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV6", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_X1", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_X1"	// �l�H�T�� �t��KP,
		},		// X1�b                     
	{ "MACHINE_INTERFACE_MAXSPEED_Y1",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_Y1",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_Y1", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV2", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV7", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_Y1", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Y1"	// �l�H�T�� �t��KP,
		},		// Y1�b 
	{ "MACHINE_INTERFACE_MAXSPEED_Z",				// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_Z",			// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_Z", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV3", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV8", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_Z", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Z"	// �l�H�T�� �t��KP,
		},		// Z�b  
	{ "MACHINE_INTERFACE_MAXSPEED_X2",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_X2",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_X2", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV4", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV9", 		// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_X2", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_X2"	// �l�H�T�� �t��KP,
		},		// X2�b                     
	{ "MACHINE_INTERFACE_MAXSPEED_Y2",			// �̰���t
		"MACHINE_INTERFACE_SPEED_ACCT_Y2",		// �[�t�ɶ�
		"MACHINE_INTERFACE_SPEED_DCCT_Y2", 		// ��t�ɶ�
		"MACHINE_INTERFACE_CONTROL_RESV5", 		// �[�t���Ƥ��
		"MACHINE_INTERFACE_CONTROL_RESV10", 	// ��t���Ƥ��
		"MACHINE_INTERFACE_SPEED_POS_KP_Y2", 	// ����T�� ��mKP
		"MACHINE_INTERFACE_SPEED_SPEED_KP_Y2"	// �l�H�T�� �t��KP,
		},		// Y2�b 
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
	iBtn_Axis	 	= GetSpecialControlNum(pwndSender, "Btn_Axis_", "CtmToolButton", pwndBtn_Axis);
	printf("iBtn_Axis=%d\n",iBtn_Axis);
	
	// ���o���� �Ѽ� ����
	for(int i = 0; i < sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]); i++ )
	{
		pwndSpeed_Data[i] = pwndSender->FindControlFromName(Speed_Data_String[i]);// �Ѽ� ����
		printf("pwndSpeed_Data[%d] = %s\n",i,Speed_Data_String[i]);
	} 
	
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
	CtmWnd* pWnd = NULL;
	DWORD		dwTempValue = 0;

	pWnd = pwndSender->FindControlFromTab(wIDControl);
	if(pWnd == NULL) return wIDControl;
		
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
		case 46:
			SendCommand(0xFF32); 
			break;
		case 7178:
			SendCommand(0xFF32); 
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
	
	UpdateData(); // ��s��� �w�q
	
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
			g_ptaskpicker->ReqValues(6, 1, &CommandID, NULL);
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
	for(int i = 0; i < sizeof(Speed_Data_String)/sizeof(Speed_Data_String[0]); i++ ) // Update Data def
	{
		if(pwndSpeed_Data[i]!=NULL) // Update Data def
		{
			pwndSpeed_Data[i]->SetPropValueT("dbid0",dbid0_Speed[u_Axis_Num][i]);
			pwndSpeed_Data[i]->CreateA();
			pwndSpeed_Data[i]->Update();
		}
	}
}