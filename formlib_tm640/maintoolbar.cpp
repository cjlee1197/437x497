/*===========================================================================+
|  Class    : maintoolbar                                                    |
|  Task     : Main Toolbar                                                   |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans                                                           |
|  Version  : V1.00                                                          |
|  Creation : 12/23/2006                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include	"maintoolbar.h"
#include	"../main.h"
#include	"../commonaction.h"
#include	"../timer.h"
#include	"../utils.h"
#include	"../lib/libiNetComm.h"
#include	"../button.h"
#include	"../selectedit.h"
#include	"../tmshmsg.h"
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
const	WORD	_TOOL_NULL		=0x0000;    
const	WORD	_TOOL_TOOLBAR	=0x0001;
const	WORD	_TOOL_VIEW		=0x0002;

//The extag of the Button on the toolbar
const	int		BUTTON_TIMEAUTOCHECK_TEXT	=0x01;	//�D��ʪ��A�h����X�ݪ��e��/Toolbar
const	int		BUTTON_ONLINEHELP_TEXT		=0x0F;	//���U�e��(�S��B:�ݭn�P�_�쩳�եέ������U�e��)
const	char*   INSTALL_SET_NAME			="Install2.txt";
const	char*   PRIVE_SET_NAME				="Priv.txt";
const	char*	AXISCARD_SET_NAME			="AxisCard.txt";
const	char*	AXISCARD2_SET_NAME			="AxisCard2.txt";
const	char*   FUNCTION_SET_NAME	  	    ="Function.txt";
const	char*   DINGDAN_SET_NAME            ="DingDan.txt";
const	char*   VIEW_INJE_NAME            ="Inje_7FT612.txt";
const	char*   VIEW_CHRG_NAME            ="Chrg_7FT612.txt";
const	char*   VIEW_TEMP_NAME            ="Temp_7FT612.txt";
//const	int		BUTTON_MANUFACTUREFUNC		=0x02;	//�ͺ޵e��

CtmWnd*		pwndOverTemp     	= NULL;		//2013-5-16 
CtmWnd*	    pwnd7LUBar12Polo    = NULL;
CtmWnd*	    pwnd7KEBar14BMC    = NULL;

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

//extern	int     g_nDemo;

//int		u_nWndExTag					= -1;				//Button Extag
WORD	u_wAction						= -1;

WORD	u_wOPStatus=0;    			//������e�����ާ@���A
WORD	u_wArryPrivilege=0;			//�{�ɬ����������v��,�̫�n�٭�(�������v��)
WORD	u_wExKey=0;

char	u_szHelpPageTextName[256];	//������ܪ����U�e�����
char	u_szLastPageTextName[256];	//�������������U�e���e���̫�@�ӵe��

int		u_nWndExTag=-1;				//Button Extag
char	u_szExFormName[256];		//����Button�D��ʪ��A�N�e�����m�ūe����ڤ��W
char	u_szExBarName[256];			//����Button�D��ʪ��A�NToolbar���m�ūe����ڤ��W

char	u_szFormName[256];
char	u_szBarName[256];
extern	WORD	g_wiNetStatus;
extern 	USERPRODUCTS	g_UserProductInfo;
extern	WORD		g_wControl;
BOOL    b_7SX = FALSE,b_7SX2 = FALSE;
BOOL	b_Selsample = FALSE;

FGC		u_ExFgc =0;					//�O��Button��ܫe�~���]�m���r���C��
int		nFTLoginFlag=0;
int 	nTransTemp	= 0;

// cjlee add 2019/4/30 �U�� 03:13:30
char	u_szLastFormName[256];		//�����W�@������
char	u_szCurrentFormName[256];		//�����W�@������
char	u_szTempFormName[256];
char	u_szJumpFormName[256];		//�u�������e��
BOOL 	b_JumpKeyB = FALSE;
BOOL 	b_JumpIO = FALSE;
/*===========================================================================+
|           Class implementation - maintoolbar                               |
+===========================================================================*/
WORD	OnKeyA(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex)
{
	printf("OnKeyA\n");
	pButton->GetPropValueT("extag", &u_nWndExTag, sizeof(u_nWndExTag));
	pButton->GetPropValueT("action", &u_wAction, sizeof(u_wAction));
	
	if(u_nWndExTag == 174)
		g_Pass = FALSE;
	Now_Push_Btn = pButton;
	
	if(u_wAction != 4096 && g_TxCNCData.shmCNCData.CNC_machine_lock == 1)
	{
		MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_MACHLOCK_OUT")],tmFT_CODE_TECH);
		return 0;
	}
	else if(u_wAction != 4096 && g_TxCNCData.shmCNCData.CNC_jog_status != RUN_STATUS_STOP)
	{
		MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_JOG_RUNING_ALARM")],tmFT_CODE_TECH);
		return 0;
	}
	else if(u_nWndExTag != 4 && g_TxCNCData.shmCNCData.CNC_servo_on == 1 && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE)
	{
		MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_MAKE_SERVO_OFF")],tmFT_CODE_TECH);
		return 0;
	}
	/*else if(u_nWndExTag == 1001)
	{
		MsgBoxCall("msgboxMenu.txt");		
		if(Toolbar_MenuIndex[0]!=0)
		{
			char  toolfilename[32];
			memset(toolfilename,0,sizeof(toolfilename));
			sprintf(toolfilename,"toolbar_%02d",Toolbar_MenuIndex[0]);
			if(Toolbar_MenuIndex[1]!=0)
			{
				sprintf(toolfilename,"%s_%02d",toolfilename,Toolbar_MenuIndex[1]);
				if(Toolbar_MenuIndex[2]!=0)
				{
					sprintf(toolfilename,"%s_%02d",toolfilename,Toolbar_MenuIndex[2]);
				}
			}
			sprintf(toolfilename,"%s.txt",toolfilename);
			pButton->SetPropValueT("barname",toolfilename);
			pButton->SetPropValueT("action",512);
			Toolbar_MenuIndex[0]=0;
			Toolbar_MenuIndex[1]=0;
			Toolbar_MenuIndex[2]=0;
		}
	}*/
	
	int	wPrivilege =0; //YANGY2009-6-13 16:
	pButton->GetPropValueT("privilege", &wPrivilege,sizeof(wPrivilege));//YANGY2009-6-13 16:

//�P�_�v��,���show��Toolbar�Ϊ�View���e 
//	printf("g_iPrivilege:%d\n", g_iPrivilege);
	if((CtmConfig::GetInstance()->GetKeyLockFlag()==2) &&(g_iPrivilege ==0))
	{
		;
	}
	else
	{
		if((u_wExKey != wKey)||(!g_WndLoginFlag))
		{ 
			if(CtmConfig::GetInstance()->GetKeyLockFlag()==1 || CtmConfig::GetInstance()->GetKeyLockFlag()==2) g_WndLoginPrivildge = 0;   //�n���v�� //JOYCE2009-9-4 add
			
			if (u_szBarName[0] != '\0')
			{
				//	printf("%d, %d \n", wPrivilege, g_iPrivilege);
				if(wPrivilege > g_iPrivilege)
				{
					g_WndLoginPrivildge = wPrivilege; //������e�n�X�ݪ��e���n�D���v��
					if(CtmConfig::GetInstance()->GetTouchPanlType() == 0)
						Login();
					else
						Login("password_touch.txt");	//2013-6-21 
					
					if(!g_WndLoginFlag)					
					{
						pButton->GetPropValueT("barname", u_szExBarName,sizeof(u_szExBarName));
						pButton->SetPropValueT("barname", (char*)NULL);
						pButton->GetPropValueT("barname", u_szBarName,sizeof(u_szBarName));
					}
				}
			}
			if (u_szFormName[0] != '\0')
			{
			  	//printf("wPrivilege=%d,g_iPrivilege=%d \n", wPrivilege, g_iPrivilege);
				if(wPrivilege > g_iPrivilege)
				{
					g_WndLoginPrivildge = wPrivilege;//������e�n�X�ݪ��e���n�D���v��
					if(CtmConfig::GetInstance()->GetTouchPanlType() == 0)
						Login();
					else
						Login("password_touch.txt");	//2013-6-21 
					
					if(!g_WndLoginFlag)
					{
						pButton->GetPropValueT("formname", u_szExFormName,sizeof(u_szExFormName));
						pButton->SetPropValueT("formname", (char*)NULL);
						pButton->GetPropValueT("formname", u_szFormName,sizeof(u_szFormName));
					}
					//printf("g_WndLoginPrivildge=%d u_szFormName=%s g_iPrivilege=%d \n", g_WndLoginPrivildge, u_szFormName, g_iPrivilege);
					if(strcmp(u_szFormName, AXISCARD_SET_NAME) == 0 ||
						strcmp(u_szFormName, AXISCARD2_SET_NAME) == 0 )
					{
						if(g_iPrivilege == 9)
						{
							strcpy(u_szFormName, AXISCARD2_SET_NAME);
						}
						else
						{
							strcpy(u_szFormName, AXISCARD_SET_NAME);
						}
						pButton->SetPropValueT("formname", u_szFormName);
						//printf("2 g_WndLoginPrivildge=%d u_szFormName=%s g_iPrivilege=%d \n", g_WndLoginPrivildge, u_szFormName, g_iPrivilege);
					}
				}
				/*wangli2013-6-25 14:46:11  7FT*/
				if((wPrivilege > g_iPrivilege) && (strcmp(u_szFormName, FUNCTION_SET_NAME) == 0))
				{
					g_WndLoginPrivildge =wPrivilege;//������e�n�X�ݪ��e���n�D���v��
					if(CtmConfig::GetInstance()->GetTouchPanlType() == 0)
						Login();
					else
					  Login("password_touch.txt");	//2013-6-21
					if(!g_WndLoginFlag)
					{
						pButton->GetPropValueT("formname", u_szExFormName,sizeof(u_szExFormName));
						pButton->SetPropValueT("formname", (char*)NULL);
						pButton->GetPropValueT("formname", u_szFormName,sizeof(u_szFormName));
					}
				}

				if((wPrivilege > g_iPrivilege) && (strcmp(u_szFormName, DINGDAN_SET_NAME) == 0))
				{
					g_WndLoginPrivildge =wPrivilege;//������e�n�X�ݪ��e���n�D���v��
					if(CtmConfig::GetInstance()->GetTouchPanlType() == 0)
						Login();
					else
						Login("password_touch.txt");	//2013-6-21
					if(!g_WndLoginFlag)
					{
						pButton->GetPropValueT("formname", u_szExFormName,sizeof(u_szExFormName));
						pButton->SetPropValueT("formname", (char*)NULL);
						pButton->GetPropValueT("formname", u_szFormName,sizeof(u_szFormName));
					}
				}
			}
		}
	}	
//ZHBA 2009-8-7 add
	
	if(u_nWndExTag == 12)	
	{
		//�̧����ϥμƶq�ӿ��toolbar�e�����
		if(g_TxCNCData.shmCNCData.PARAM.maker_param[33]<2)
			pButton->SetPropValueT("barname", "toolbar12_1.txt");
		else
			pButton->SetPropValueT("barname", "toolbar12_2.txt");
	}
	Now_Push_Btn = pButton;
	
	//cjlee add 2019/4/30 �U�� 04:48:15
	if(u_wAction == 777) //�u������IO
	{
		b_JumpIO=!b_JumpIO; b_JumpKeyB=FALSE;
		if(b_JumpIO) //�u����
			{
				pButton->GetPropValueT("formname", &u_szTempFormName, sizeof(u_szTempFormName));
				printf("Pop to %s\n",u_szTempFormName);
				::PutCommand(u_szTempFormName);
			}
		else //��^��e��
			{
				printf("Go back to %s \n",g_szCurrentFormName);
				pButton->Press(tmBUTTON_UP);
				::PutCommand(g_szCurrentFormName);
			}
	}
	if(u_wAction == 778) //�u������KeyBoard
	{
		b_JumpKeyB=!b_JumpKeyB; b_JumpIO=FALSE;
		if(b_JumpKeyB) //�u����
			{
				pButton->GetPropValueT("formname", &u_szTempFormName, sizeof(u_szTempFormName));
				printf("Pop to %s\n",u_szTempFormName);
				::PutCommand(u_szTempFormName);
			}
		else //��^��e��
			{
				printf("Go back to %s \n",g_szCurrentFormName);
				pButton->Press(tmBUTTON_UP);
				::PutCommand(g_szCurrentFormName);
			}
	}
	// cjlee add
	return wKey;
}

WORD	OnKeyAEnd(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex)
{
	printf("Maintollbar OnKeyAEnd\n");
	pButton->GetPropValueT("extag", &u_nWndExTag, sizeof(u_nWndExTag));
	pButton->GetPropValueT("action", &u_wAction, sizeof(u_wAction));
	if(u_wAction == 4096 && u_nWndExTag != 4) //extag = 4 �Oreset �����e�X�T����view
	{
			SendMsg(MSG_SH_APP_TOOLBAR_ONKEY,wKey,u_nWndExTag,NULL);  //�e�Xmsg�t�Xview��F1~F10���ʧ@
	}
	
	if(u_nWndExTag == 4)
	{
		char temp2 = RUN_STATUS_STOP;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_auto_status,&temp2,sizeof(g_TxCNCData.shmCNCData.CNC_auto_status));
		int temp = ID_RESET;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_action_id,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_action_id));
	}
	 if(u_nWndExTag == 1000)
		pButton->SetPropValueT("extag",1001);
		

	// cjlee add �� 2019/4/30 �U�� 03:24:11
	if(u_wAction == 99 && u_nWndExTag == 99) //extag = 99 �O��^
	{
		printf("Press GO Back");
		printf("Last form %s\n",g_szLastFormName);
		printf("Now form %s\n",g_szCurrentFormName);
		if (strcmp(g_szLastFormName, g_szCurrentFormName) != 0)
			{
				printf(" to %s\n",g_szLastFormName);
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName	, g_szLastFormName);
				::PutCommand(g_szCurrentFormName);
			}
	}
	
	if(u_wAction != 99 && u_wAction != 777) // ���O��^ �]���O�u��
	{
		pButton->GetPropValueT("formname", &u_szTempFormName, sizeof(u_szTempFormName));
		printf("Press %s\n",u_szTempFormName);
		if(strcmp(u_szTempFormName, g_szCurrentFormName))
			{
				memset(g_szLastFormName, 0, sizeof(g_szLastFormName));
				strcpy(g_szLastFormName, g_szCurrentFormName);
				memset(g_szCurrentFormName, 0, sizeof(g_szCurrentFormName));
				strcpy(g_szCurrentFormName, u_szTempFormName);// �������୶��
				printf("Last form %s\n",g_szLastFormName);
				printf("Now form %s\n",g_szCurrentFormName);
				pButton->Press(tmBUTTON_UP);
				printf("Go to %s\n",g_szCurrentFormName);
				::PutCommand(g_szCurrentFormName);	
			}
	}
		
	//cjlee add ��
		
	return 0;
}
