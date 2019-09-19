/*===========================================================================+
|  Class    : ViewPG_0 library                           		                 |
|  Task     : ViewPG_0 library action source file      		                   |
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
#include	"ViewPG_0.h"
#include    "../database.h"
#include    "../init.h"
#include    "../selectedit.h"
#include    "../tmconfig.h"
//#include    "../lib/hmi_control_data/tmControl_CurveExData.h"
#include    "../dialog.h" 

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
#define     CONST_REQ_COMMAND       6
#define     CONST_REQ_WRITE         3
#define     CONTROLTYPENUM          13
#define     MAXSTEP               100
#define     STATE_SINGLESTEP        2
#define		STATE_HAND				3
#define     STATE_FULLAUTO          4 
#define     PARA_NUM	            10 
int     iEditNo=0,iCheckBoxAct=0,iEditACTION=0,iSelectEditACTION=0,iStaticACTION=0,iStaticEditNo=0;
int 	iStaticAct=0;
int 	No1=0,No=0;
int		SelectNo = 0;
int		Cleanhelp =0;
int		MaxDBNum = 100;
int		EditedStepNum = 0;
bool	m_bEnterKey = FALSE;
char 	pNo[256]	={NULL}; 
char 	pStaticNo[256]	={NULL}; 
char	pNoDataID[256] = {NULL};
WORD        u_wPickerOPSatus    = 0;
WORD        wStep  = 0,wStepOld;
WORD        wSingle_Step  = 0,wSingle_StepOld;
char*       u_pszDBString[MAXSTEP*CONTROLTYPENUM] = {NULL};

CtmWnd*		pwndEditNo[32] 	={NULL}; 		//����� �Ǹ��C
CtmWnd*		pwndStaticEditNo[32] 	={NULL}; 		//����� �Ǹ��C
CtmWnd*		pwndCheckBoxAct[32] ={NULL}; 	//������ �ʧ@�C
CtmWnd*		pwndStaticAct[32] ={NULL}; 	//�ʧ@�C��r
CtmWnd*		pwndButtonPageDown	= NULL;
CtmWnd*		pwndButtonPageUp	= NULL;
CtmWnd*		pwndSelectEditACTIONTYPE	= NULL;
CtmWnd*		pwndSelectEditACTIONNUM	= NULL;
CtmWnd*		pwndEditACTIONNUM	= NULL;
CtmWnd*		pwndEditSelectNo	= NULL;
CtmWnd*		pwndEditACTION[32] ={NULL};
CtmWnd*		pwndSelectEditACTION[32] ={NULL};
CtmWnd*		pwndStaticACTION[32] ={NULL};
CtmWnd*		pwndButtonSAVE	= NULL;
CtmWnd*		pwndButtonCANEL	= NULL;
CtmWnd*		pwndButtonInsert	= NULL;
CtmWnd*		pwndButtonDelete	= NULL;
CtmWnd*		pwndButtonStartEdit	= NULL;

CtmWnd*		pwndText1[32] ={NULL};
CtmWnd*		pwndText2[32] ={NULL};
CtmWnd*		pwndText3[32] ={NULL};
CtmWnd*		pwndText4[32] ={NULL};
CtmWnd*		pwndText5[32] ={NULL};

CtmWnd* u_pwndSingle = NULL;
int     iNumText1=0,iNumText2=0,iNumText3=0,iNumText4=0,iNumText5=0;

CtmWnd*			pwndButton_Back       = NULL;  
CtmWnd*			pwndBtnHome       = NULL;  
CtmWnd*			pwndBtnMenu       = NULL;  
CtmWnd*			pwndBtnBack       = NULL;  

long	NoColor[] =
{
	0x8627,
	0xFFDF,
};

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
	
	
	{	//��ʼ���� ��l�w�q
		iEditNo 	 			= GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);					//��������� �Ǹ��C�ƶq
		iStaticEditNo 	 			= GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", pwndStaticEditNo);	//��������� �Ǹ��C�ƶq
		iCheckBoxAct				= GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);	//���������� �ʧ@�C�ƶq
		iStaticAct				= GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", pwndStaticAct);	//���������� �ʧ@�C�ƶq
		iEditACTION				= GetSpecialControlNum(pwndSender, "EditACTION", "CtmEditX1", pwndEditACTION);
		iSelectEditACTION				= GetSpecialControlNum(pwndSender, "SelectEditACTION", "CtmSelectEdit", pwndSelectEditACTION);
		iStaticACTION				= GetSpecialControlNum(pwndSender, "StaticACTION", "CtmStaticX2", pwndStaticACTION);
		No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39").lValue;	
		SelectNo = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40").lValue;	
		//SetDBValue("MACHINE_PROFILE_STEPNUM", 0);		//�������� �ƶq�M�s
		pwndButtonPageDown	 = pwndSender->FindControlFromName("buttonDOWNTWENTY");
		pwndButtonPageUp	 = pwndSender->FindControlFromName("buttonUPTWENTY");
		pwndEditSelectNo	= pwndSender->FindControlFromName("EditSelectNo");
		pwndSelectEditACTIONTYPE	= pwndSender->FindControlFromName("SelectEditTYPE");
		pwndSelectEditACTIONNUM	= pwndSender->FindControlFromName("SelectEditNUM");
		pwndEditACTIONNUM	= pwndSender->FindControlFromName("Edit1NUM");
		pwndButtonSAVE		= pwndSender->FindControlFromName("ButtonSAVE");
		pwndButtonCANEL		= pwndSender->FindControlFromName("ButtonCANEL");
		pwndButtonInsert		= pwndSender->FindControlFromName("ButtonInsert");
		pwndButtonDelete		= pwndSender->FindControlFromName("ButtonDelete");
		pwndButtonStartEdit		= pwndSender->FindControlFromName("ButtonStartEdit"); //2018-8-1
		
		iNumText1 	 			= GetSpecialControlNum(pwndSender, "Text1", "CtmStaticX2", pwndText1);
		iNumText2 	 			= GetSpecialControlNum(pwndSender, "Text2", "CtmStaticX2", pwndText2);
		iNumText3 	 			= GetSpecialControlNum(pwndSender, "Text3", "CtmStaticX2", pwndText3);
		iNumText4 	 			= GetSpecialControlNum(pwndSender, "Text4", "CtmStaticX2", pwndText4);
		iNumText5 	 			= GetSpecialControlNum(pwndSender, "Text5", "CtmStaticX2", pwndText5);
		
		u_pwndSingle = pwndSender->FindControlFromName("ButtonSingle");
		u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
		char temp[128];
		char szTemp[256];
		
		pwndButton_Back     = pwndSender->FindControlFromName("LinuxCtmToolButton5");
		pwndBtnHome     = pwndSender->FindControlFromName("BtnHome");
		pwndBtnMenu     = pwndSender->FindControlFromName("BtnMenu");
		pwndBtnBack     = pwndSender->FindControlFromName("BtnBack");
	}
	
	int nStep = 0;
	for(int i = 1; i <= MAXSTEP; i++ )
    {       
        u_pszDBString[nStep] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep], "MACHINE_PROFILE_NUM%d_ACTION_STEP", i);
        u_pszDBString[nStep+1] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+1], "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i);
        u_pszDBString[nStep+2] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+2], "MACHINE_PROFILE_NUM%d_ACTION_NUM", i);
        u_pszDBString[nStep+3] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+3], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i);  
        u_pszDBString[nStep+4] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+4], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i); 
        u_pszDBString[nStep+5] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+5], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3", i);  
        u_pszDBString[nStep+6] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+6], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4", i);  
        u_pszDBString[nStep+7] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+7], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i);  
        u_pszDBString[nStep+8] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+8], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER6", i);  
        u_pszDBString[nStep+9] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+9], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER7", i);  
        u_pszDBString[nStep+10] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+10], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER8", i);  
        u_pszDBString[nStep+11] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+11], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER9", i);  
        u_pszDBString[nStep+12] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+12], "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER10", i);  
        nStep += 13;     
    }
    nStep = 0;
    {
    /*=========================TEST============================*/
    printf("No1=%d\n",No1);
    {
	//printf("UpdateText\n");
	char 	pDataID[256];
	char 	pDataID2[256];
	char 	pDataID3[256];
	char 	pDataID4[256];
	char 	pDataID5[256];
	char 	pDataID6[256];
	char 	pDataID7[256];
	char 	pDataID8[256];
	char 	pDataID9[256];
	char 	pDataID10[256];
	char 	pDataID11[256];
	char 	pDataID12[256];
	char  ACTION_STEP_ID[256];
	char    szActPara1[256];
	char    szActPara2[256];
	char    szActPara3[256];
	char    szActPara5[256];
	int	ACTION_STEP =0,ACTIONTYPE =0,ACTIONNUM =0;
	DWORD   wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
	// �� �r��X����� cjlee add 	
	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
	char	str[1024]; // ��ܦr���
	char	path[1024]; // ��ܦr���
	char* str_1;
	char* str_2;
	char* str_3;
	char* str_4;
	char* str_5;
	char* str_6;
	char* str_7;
	char* str_8;
	char* str_9;
	char* str_10;
	char* str_11;
	char* str_12;
	
	// �� �r��X����� cjlee add 
	for(int i=0;i<iCheckBoxAct;i++)
	{
		memset(str,0,sizeof(str));
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1+No1);
		memset(pDataID2, 0 ,sizeof(pDataID2));
		sprintf(pDataID2,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1+No1);
		sprintf(szActPara1,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i+1+No1); 
		sprintf(szActPara2,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i+1+No1); 
		sprintf(szActPara3,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3", i+1+No1); 
		sprintf(szActPara5,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1+No1); 
		ACTIONTYPE =GetDBValue(pDataID).lValue;
		ACTIONNUM =GetDBValue(pDataID2).lValue;
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		
		sprintf(ACTION_STEP_ID,"MACHINE_PROFILE_NUM%d_ACTION_STEP", i+1+No1);
		ACTION_STEP = GetDBValue(ACTION_STEP_ID).lValue;
		//printf("LINE%d, ACTION_STEP:%d, ACTIONTYPE:%d, ACTIONNUM:%d, Para1:%d, Para2:%d, Para3:%d, Para5:%d\n",i+1+No1,ACTION_STEP,ACTIONTYPE,ACTIONNUM,wActPara1,wActPara2,wActPara3,wActPara5);
		memset(pDataID, 0 ,sizeof(pDataID));
		memset(pDataID2, 0 ,sizeof(pDataID2));
		memset(pDataID3, 0 ,sizeof(pDataID3));
		memset(pDataID4, 0 ,sizeof(pDataID4));
		memset(pDataID5, 0 ,sizeof(pDataID5));
		memset(pDataID6, 0 ,sizeof(pDataID6));
		memset(pDataID7, 0 ,sizeof(pDataID7));
		memset(pDataID8, 0 ,sizeof(pDataID8));
		memset(pDataID9, 0 ,sizeof(pDataID9));
		memset(pDataID10,0 ,sizeof(pDataID10));
		memset(pDataID11,0 ,sizeof(pDataID11));
		memset(pDataID12,0 ,sizeof(pDataID12));

		//printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
		switch(ACTIONTYPE)
		{
			case 0:		//
				sprintf(pDataID,"VW_PICKER_NULL");
				break;
			case 1:		//�b�ʧ@
				if(wActPara5) // ����
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "����"
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "��"
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X�b"
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y�b"
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z�b"
						break;
//					case 4: // cjlee 2019/4/6 �U�� 05:44:41
//						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2�b"
//						break;
//					case 5: // cjlee 2019/4/6 �U�� 05:44:41
//						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2�b"
//						break;
					default:
						break;
				}
				//if(ACTIONNUM <= 5) // cjlee ���b 2019/4/6 �U�� 05:44:41
				if(ACTIONNUM <= 3 || ACTIONNUM >= 5) // cjlee �T�b2019/4/6 �U�� 05:44:41
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "�H"
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "���t�ײ��ʨ�"
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				//else if(ACTIONNUM == 6) // cjlee ���b
				else if(ACTIONNUM == 4) // cjlee �T�b 2019/4/6 �U�� 05:44:41
				{
					if(wActPara2)
						sprintf(pDataID4,"PICKER_REMOTE_O_14");
					else
						sprintf(pDataID4,"PICKER_REMOTE_O_13");
					if(wActPara3)
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
					else
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				}
				break;
			case 2:		//����
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_WAIT");
				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 3:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_INNER_O_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 4:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_VALVE");
				sprintf(pDataID5,"DLG_DATETIME_COLON");
				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
				if(wActPara2)
				{
					sprintf(pDataID8,"PICKER_ACTTIME");
					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
					sprintf(pDataID10,"VW_INJPRO_SECOND");
				}
				break;
			case 5:		//����
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_START");
				}
				else
				{
					sprintf(pDataID4,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 6:		//����
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_END");
				}
				else
				{
					sprintf(pDataID4,"PICKER_JUMP");
					sprintf(pDataID5,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 8:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"VW_PUMP_TEST");
				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 12:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM)
					sprintf(pDataID4,"VW_PID_GROUP%d",ACTIONNUM);
				else
					sprintf(pDataID4,"VW_PICKER_NULL");
				sprintf(pDataID5,"VW_PICKER_PILE");
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 13:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_SUBTECH_1");
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID5,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID5,"PICKER_PROD_BAD");
						break;
					case 2:
						sprintf(pDataID5,"PICKER_PROD_SAMPLE");
						break;
					case 3:
						sprintf(pDataID5,"PICKER_PROD_TEST");
						break;
					case 4:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					case 5:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					default:
						break;
				}
				if(wActPara1)
					sprintf(pDataID6,"ACTIONPOINT_START");
				else
					sprintf(pDataID6,"ACTIONPOINT_END");
				sprintf(pDataID7,"PICKER_SUBTECH_2");
				sprintf(pDataID8,"%2d",wActPara2);
				break;
			default: 
				break;	
		}
		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
		// �� �r��X����� cjlee add 
		index_1 = g_MultiLanguage.GetStrKeyIndex(pDataID);
		index_2 = g_MultiLanguage.GetStrKeyIndex(pDataID2);	
		index_3 = g_MultiLanguage.GetStrKeyIndex(pDataID3);	
		index_4 = g_MultiLanguage.GetStrKeyIndex(pDataID4);
		index_5 = g_MultiLanguage.GetStrKeyIndex(pDataID5);	
		index_6 = g_MultiLanguage.GetStrKeyIndex(pDataID6);	
		index_7 = g_MultiLanguage.GetStrKeyIndex(pDataID7);
		index_8 = g_MultiLanguage.GetStrKeyIndex(pDataID8);	
		index_9 = g_MultiLanguage.GetStrKeyIndex(pDataID9);	
		index_10 = g_MultiLanguage.GetStrKeyIndex(pDataID10);
		index_11 = g_MultiLanguage.GetStrKeyIndex(pDataID11);	
		index_12 = g_MultiLanguage.GetStrKeyIndex(pDataID12);	
		
		if (index_1<0)
			index_1 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_2<0)
			str_2 = pDataID2;
		else
			str_2 = g_MultiLanguage[index_2];
		if (index_3<0)
			index_3 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_4<0)
			index_4 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_5<0)
			index_5 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_6<0)
			str_6 = pDataID6;
		else
			str_6 = g_MultiLanguage[index_6];
		if (index_7<0)
			index_7 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_8<0)
			str_8 = pDataID8;
		else
			str_8 = g_MultiLanguage[index_8];
		if (index_9<0)
			index_9 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_10<0)
			index_10 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_11<0)
			str_11= pDataID11;
		else
			str_11 = g_MultiLanguage[index_11];
		if (index_12<0)
			index_12 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		//printf("Line%d = %d  %d  %d %d  %d  %d %d  %d  %d %d  %d  %d\n",i+1+No1,index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12);
		
		str_1 = g_MultiLanguage[index_1];
		//str_2 = g_MultiLanguage[index_2];
		str_3 = g_MultiLanguage[index_3];
		str_4 = g_MultiLanguage[index_4];
		str_5 = g_MultiLanguage[index_5];
		//str_6 = g_MultiLanguage[index_6];
		str_7 = g_MultiLanguage[index_7];
		//str_8 = g_MultiLanguage[index_8];
		str_9 = g_MultiLanguage[index_9];
		str_10 = g_MultiLanguage[index_10];
		//str_11 = g_MultiLanguage[index_11];
		str_12 = g_MultiLanguage[index_12];
		
		memset(str,0,sizeof(str));
		//printf("sl_1=%d, sl_2=%d, sl_3=%d, sl_4=%d, sl_5=%d, sl_6=%d, sl_7=%d, sl_8=%d, sl_9=%d, sl_10=%d, sl_11=%d, sl_12=%d\n",strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
		strncat(str, str_1, strlen(str_1));
		strncat(str, str_2, strlen(str_2));
		strncat(str, str_3, strlen(str_3));
		strncat(str, str_4, strlen(str_4));
		strncat(str, str_5, strlen(str_5));
		strncat(str, str_6, strlen(str_6));
		strncat(str, str_7, strlen(str_7));
		strncat(str, str_8, strlen(str_8));
		strncat(str, str_9, strlen(str_9));
		strncat(str, str_10, strlen(str_10));
		strncat(str, str_11, strlen(str_11));
		strncat(str, str_12, strlen(str_12));
		//printf("Line%d = %s\n",i+1+No1-5,str);
		//printf("strlen=%d\n",strlen(str));
		//strncpy(path, str, strlen(str)+1);
		//path[strlen(str)]= '\0';
		//printf("path=%d\n",path);
/*
		memset(str,0,sizeof(str));
		printf("strlen:%d\n",strlen(str));
		memcpy(str,str_1,strlen(str_1));
		memcpy(str+strlen(str_1),str_2,strlen(str_2));
		memcpy(str+strlen(str_1)+strlen(str_2),str_3,strlen(str_3));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3),str_4,strlen(str_4));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4),str_5,strlen(str_5));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5),str_6,strlen(str_6));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6),str_7,strlen(str_7));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7),str_8,strlen(str_8));		
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8),str_9,strlen(str_9));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9),str_10,strlen(str_10));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10),str_11,strlen(str_11));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11),str_12,strlen(str_12));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11)+strlen(str_12),"",strlen(""));
		printf("%d str%d strlen: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",i+1+No1,strlen(str),strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
*/
		if(pwndCheckBoxAct[i] != NULL)
		{
			//pwndCheckBoxAct[i]->SetPropValueT("captionID","");
			//pwndCheckBoxAct[i]->CreateA();
			//pwndCheckBoxAct[i]->Show();
			//pwndCheckBoxAct[i]->Update();
			//pwndCheckBoxAct[i]->UpdateAll();
		}
		if(pwndStaticAct[i] != NULL)
		{
			RegionUpdateStop_APP = TRUE; //�����W�ߨ�s�A�O������s�d��
			
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			pwndStaticAct[i]->UpdateAll();
			
			ChangePos(1);  //�N����s�������@����s
			

		}
		//pwndCheckBoxAct[i]->Update();
		// �� �r��X����� cjlee add 
		/*
		pwndCheckBoxAct[i]->SetPropValueT("captionID1", pDataID);printf("pDataID1:%s",pDataID);
		pwndCheckBoxAct[i]->SetPropValueT("captionID2", pDataID2);printf("pDataID2:%s",pDataID2);
		pwndCheckBoxAct[i]->SetPropValueT("captionID3", pDataID3);printf("pDataID3:%s",pDataID3);
		pwndCheckBoxAct[i]->SetPropValueT("captionID4", pDataID4);printf("pDataID4:%s",pDataID4);
		pwndCheckBoxAct[i]->SetPropValueT("captionID5", pDataID5);
		pwndCheckBoxAct[i]->SetPropValueT("captionID6", pDataID6);
		pwndCheckBoxAct[i]->SetPropValueT("captionID7", pDataID7);
		pwndCheckBoxAct[i]->SetPropValueT("captionID8", pDataID8);
		pwndCheckBoxAct[i]->SetPropValueT("captionID9", pDataID9);
		pwndCheckBoxAct[i]->SetPropValueT("captionID10", pDataID10);
		pwndCheckBoxAct[i]->SetPropValueT("captionID11", pDataID11);
		pwndCheckBoxAct[i]->SetPropValueT("captionID12", pDataID12);
		pwndCheckBoxAct[i]->CreateA();
		pwndCheckBoxAct[i]->Update();
		pwndCheckBoxAct[i]->UpdateAll();*/		
	}
		}
    /*=========================TEST============================*/
  	}

	int SelectFlag =0;
	for (int i = 0; i < iEditNo; i++)	//������� ��ѧǸ�
	{
		No = i+1+No1;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		if(pwndEditNo[i] != NULL)
		{
			pwndEditNo[i]->SetPropValueT("text",pNo);
			pwndEditNo[i]->Update();
		}

		if(No<6)
		{
			RegionUpdateStop_APP = TRUE; //�����W�ߨ�s�A�O������s�d��
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",50712);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
			ChangePos(1);  //�N����s�������@����s
		}

		else
		{
			RegionUpdateStop_APP = TRUE; //�����W�ߨ�s�A�O������s�d��
			
			memset(pNoDataID, 0 ,sizeof(pNoDataID));
			sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",No);
			memset(pStaticNo, 0 ,sizeof(pStaticNo));
			int value = GetDBValue(pNoDataID).lValue;
			if(No>EditedStepNum)value=5; // ���s�誺�ʧ@
			sprintf(pStaticNo,"%d",value-5);
			
			//memset(pStaticNo, 0 ,sizeof(pStaticNo));
			//sprintf(pStaticNo,"%d",No-5);
			pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
			pwndStaticEditNo[i]->Update();
			ChangePos(1);  //�N����s�������@����s
		}
		if(pwndEditNo[i] != NULL)			//
		{
			if(No==SelectNo)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",36256);
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
				SelectFlag =1;
			}
		}
	}
	if(pwndEditNo[0] != NULL && !SelectFlag)			//������
	{
		pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
		sscanf(pNo,"%d",&SelectNo);
		pwndStaticAct[0]->SetPropValueT("bgc",36256);
		//pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
	}
	
	char 	pDataID[256],pDataID2[256];
	int		type = 0,num = 0;
	for(int i =0;i<MaxDBNum;i++) // �p�Ʀ��s�g���оɦ��X�B�J
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		type = GetDBValue(pDataID).lValue;
		memset(pDataID2, 0 ,sizeof(pDataID));
		sprintf(pDataID2,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		num = GetDBValue(pDataID2).lValue;

		if(type ==6 && num ==1) // "����"type=6 num=1
		{
			EditedStepNum=i+1;
		}
	}
	printf("EditedStepNum=%d\n",EditedStepNum);
	
	UpdateText();
	UpdateNo();
	
	return TRUE;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED42").lValue)
	{
		UpdateText();
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED42", 0);
	}
	if(u_wPickerOPSatus == STATE_SINGLESTEP || u_wPickerOPSatus == STATE_FULLAUTO)
    {
        wStep = (WORD)GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
        int StepNumMin = 1+No1;
        int StepNumMax = 10+No1;
        if(wStep >0 && wStep < StepNumMin)
        {
        	PageUp(pwndSender);
        	UpdateText();
        	UpdateNo();
        }
        else if(wStep <=100 && wStep > StepNumMax)
        {
        	PageDown(pwndSender);
        	UpdateText();
        	UpdateNo();
        }
        else if(wStep >0 && wStep != wStepOld)
        {
        	printf("wStep=%d\n",wStep);
        	for(int i =0; i < iCheckBoxAct; i++)
					{
						int StepNum = i+1+No1;
						if(wStep == StepNum && pwndCheckBoxAct[i] != NULL && pwndStaticAct[i] != NULL)
						{
							for(int k =0; k < iCheckBoxAct; k++)		//������ʾ
							{
								No = k+1+No1;
								if(No <6)
								{
									pwndStaticAct[k]->SetPropValueT("bgc",50712);
									//pwndCheckBoxAct[k]->SetPropValueT("bgc",50712);
								}
								else
								{
									pwndStaticAct[k]->SetPropValueT("bgc",0xFFDF);
									//pwndCheckBoxAct[k]->SetPropValueT("bgc",0xFFDF);
								}
							}
							((CtmFormView*)pwndSender)->Goto(pwndStaticAct[i]->GetTabOrder());
							//((CtmFormView*)pwndSender)->Goto(pwndCheckBoxAct[i]->GetTabOrder());
							pwndStaticAct[i]->SetPropValueT("bgc",36256);
							pwndStaticAct[i]->CreateA();
							pwndStaticAct[i]->Show();
							pwndStaticAct[i]->Update();
							//pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
							//pwndCheckBoxAct[i]->Update();
						}
					}
        	wStepOld = wStep;
        }
    }
  UpdateText();
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("OnChangeA\n");
	CtmWnd*	pwnd = pwndSender->FindControlFromTab(wIDControl);

	return wIDControl;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	int         nCMDValue       = 0xFF0D;
	int         nCMDLastStep    = 0xFF26;
	int         nCMDSingleCycle = 0xFF25;
	char 	pDataID[256];
	int		ActionType = 0, ACTIONNUM = 0;
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",SelectNo);
	ActionType =GetDBValue(pDataID).lValue;
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM", SelectNo); 
	ACTIONNUM =GetDBValue(pDataID).lValue;
	if(u_wPickerOPSatus != STATE_FULLAUTO) //�Զ�������
	{
		if (wKey == 0x000B)		//��һҳ
		{
			PageDown(pwndSender);
			UpdateText();
			UpdateNo();
			return _NULL_KEY;
		}
		else if (wKey == 0x000A)	//��һҳ
		{
			//printf("PageUp\n");
			PageUp(pwndSender);
			UpdateText();
			UpdateNo();
			return _NULL_KEY;
		}
		/*else if(wKey == 0x0005)		//��ʼ�༭
		{
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", SelectNo);
			if(ActionType != 0)
			{
				switch(ActionType)
				{
					case 1:		//�ᶯ��
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_AXISACT.txt");
						usleep(100*1000);
					break;
					case 2:		//�ȴ�
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_WAIT.txt");
						usleep(100*1000);
					break;
					case 3:		//����
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_PERMIT.txt");
						usleep(100*1000);
					break;
					case 4:		//����
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_VALVE.txt");
						usleep(100*1000);
					break;
					case 5:		//��ǩ
						if(ACTIONNUM != 1)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							::PutCommand("PG_2_LABEL.txt");
							usleep(100*1000);
						}
					break;
					case 6:		//��ת
						if(ACTIONNUM != 1)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							::PutCommand("PG_2_LABEL.txt");
							usleep(100*1000);
						}
					break;
					case 8:		//���
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_TEST.txt");
						usleep(100*1000);
					break;
					case 12:	//�ѵ�
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_PILE.txt");
						usleep(100*1000);
					break;
					case 13:	//�ӳ���
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_SUBTECH.txt");
						usleep(100*1000);
					break;
					default:
						::PutCommand("PG_1.txt");
						usleep(100*1000);
					break;
				}
			}
			else
			{
				::PutCommand("PG_1.txt");
				usleep(100*1000);
			}
		}*/
		else if (wKey == 0x0002)	//���J��
		{
			if(SelectNo > 5 && SelectNo <= EditedStepNum)
			{
				if(ActionType != 5)
				{
					Insert(SelectNo);
					EditedStepNum++;
					UpdateText();
					UpdateNo();
				}
				else if(ACTIONNUM != 1)
				{
					Insert(SelectNo);
					EditedStepNum++;
					UpdateText();
					UpdateNo();
				}
			}
		}
		else if (wKey == 0x0003)	//�R��
		{
			if(SelectNo > 5 && SelectNo <= EditedStepNum)
			{
				if(ActionType != 5 && ActionType != 6)
				{
					UnSync(SelectNo);
					UnSync(SelectNo+1);
					Delete(SelectNo);
					EditedStepNum--;
					UpdateText();
					UpdateNo();
				}
				else if(ACTIONNUM != 1)
				{
					UnSync(SelectNo);
					UnSync(SelectNo+1);
					Delete(SelectNo);
					EditedStepNum--;
					UpdateText();
					UpdateNo();
				}
			}
		}
		else if (wKey == 0x0007)	//���� �M�z
		{
			Cleanhelp =0;
			Clean();
			UpdateText();
		}
		else if (wKey == 0x0008)	//��M�����
		{
			MsgBox(g_MultiLanguage["PICKER_CONFIRMDELETE"], tmFT_CODE_TECH);
		    if(g_bMsgBoxFlag)
		    {
		    	Clear();
		    	UpdateText();
		    	UpdateNo();
		    }
		}
		else if (wKey == 0x0004)	//�U��
		{
			if(u_wPickerOPSatus == STATE_HAND)
			{
				Cleanhelp =0;
				Clean();
				Prompt(g_MultiLanguage["PICKER_DATADOWNLODING"],1); 
				Download();
				UpdateNo();
				UpdateText();
				Prompt(g_MultiLanguage["PICKER_DATADOWNLOADFINISH"],1); 
			}
			else
			{
				MsgBox(g_MultiLanguage["PICKER_M3_ALARM1042"], tmFT_CODE_TECH);
			}
		}
		else if(wKey == 0x0001) //�U�@�B
    	{
    	    if(u_wPickerOPSatus == STATE_SINGLESTEP)
    	    {
    	    		wSingle_Step = (WORD)GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
    	        if(g_ptaskpicker != NULL && wSingle_Step != wSingle_StepOld)
    	        {
    	        		printf("Now step=%d\n",wSingle_Step);
    	            g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDValue, NULL);
    	            printf("SendCMD%d:%x=1\n",CONST_REQ_COMMAND,nCMDValue);
    	            wSingle_StepOld = wSingle_Step;    	            
    	        }
    	        if(u_pwndSingle != NULL)
    	        {
    	            u_pwndSingle->SetPropValueT("upbitmap", "res_tm640/pic/ButtonDown.bmp");
    	            u_pwndSingle->Update();
    	        }
    	    }
    	    else
    	    {
    	         MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
    	    }
    	    return _NULL_KEY;
    	}
    else if(wKey == 0x0009)// �W�@�B
    	{
    	    if(u_wPickerOPSatus == STATE_SINGLESTEP)
    	    {
    	    		//wSingle_Step = (WORD)GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
    	        if(g_ptaskpicker != NULL )//&& wSingle_Step != wSingle_StepOld)
    	        {
  	        		//printf("Now step=%d\n",wSingle_Step);
  	            g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDLastStep, NULL);
  	            printf("nCMDLastStep:%X\n",nCMDLastStep);
  	            //wSingle_StepOld = wSingle_Step;
    	        }
    	    }
    	    else
    	    {
    	         MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
    	    }
    	    return _NULL_KEY;
    	}
    else if(wKey == 0x000F)
    	{
    		if(u_wPickerOPSatus == STATE_SINGLESTEP)
    	    {
    	    	if(g_ptaskpicker != NULL)
    	    	{
    	       		g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDSingleCycle, NULL);
    	      	 	printf("nCMDSingleCycle:%X\n",nCMDSingleCycle);
    	    	}   
    	    }
    	    else
    	    {
    	         MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
    	    }
    	    return _NULL_KEY;
    	}
    else if(wKey == 0x000C) // �X��
    	{
				if(SelectNo > 5)
				{
					if(ActionType != 5)
					{
						Sync(SelectNo);
						UpdateText();
					}
					else if(ACTIONNUM != 1)
					{
						Sync(SelectNo);
						UpdateText();
					}
				}
    	}
    else if(wKey == 0x000D) // ����
    	{
    		if(SelectNo > 6)
				{
					if(ActionType != 5)
					{
						UnSync(SelectNo);
						UpdateText();
					}
					else if(ACTIONNUM != 1)
					{
						UnSync(SelectNo);
						UpdateText();
					}
				}
    	}
    else if(wKey == 0x000E) // ��B�Ҧ�
    	{
    		;
    	}
	}

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	//�ݸ� cjlee 2019/4/9 �U�� 05:37:54
//	char 	pDataID[256];
//	int 	ACTION_STEP = 0;
//				// ���o���@�BACTION_STEP
//			memset(pDataID, 0 ,sizeof(pDataID));
//			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo);
//			ACTION_STEP = GetDBValue(pDataID).lValue;
//			printf("ACTION_STEP_Pre=%d\n",ACTION_STEP);

	if(u_wPickerOPSatus != STATE_FULLAUTO) //�Զ������� �۰ʤU����
	{
		if(pwnd == pwndButtonStartEdit)
 		{
 			if(SelectNo < EditedStepNum)
			{
	 			//printf("Start Edit\n");
	 			char 	pDataID[256];
				int		ActionType = 0, ACTIONNUM = 0;
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",SelectNo);
				ActionType =GetDBValue(pDataID).lValue;
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM", SelectNo); 
				ACTIONNUM =GetDBValue(pDataID).lValue;
	 			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", SelectNo);
	 			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType); // cjlee �T�O�x�s�ʧ@���� 2019/2/20 �U�� 08:13:15
	 			printf("PG0 Set SelectNo%d ActionType=%d\n",SelectNo,ActionType); 
				if(ActionType != 0)
				{
					switch(ActionType)
					{
						case 1:		//�ᶯ�� �b�ʧ@
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_AXISACT.txt");
							::PutCommand("PG_2_AXISACT.txt");
							usleep(100*1000);
						break;
						case 2:		//�ȴ� ����
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_WAIT.txt");
							::PutCommand("PG_2_WAIT.txt");
							usleep(100*1000);
						break;
						case 3:		//���� ���\
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_PERMIT.txt");
							::PutCommand("PG_2_PERMIT.txt");
							usleep(100*1000);
						break;
						case 4:		//���� �֪�
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_VALVE.txt");
							::PutCommand("PG_2_VALVE.txt");
							usleep(100*1000);
						break;
						case 5:		//��ǩ����
							if(ACTIONNUM != 1)
							{
								SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
								//pwnd->SetPropValueT("formname","PG_2_LABEL.txt");
								::PutCommand("PG_2_LABEL.txt");
							usleep(100*1000);
							}
						break;
						case 6:		//��ת ����
							if(ACTIONNUM != 1)
							{
								SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
								//pwnd->SetPropValueT("formname","PG_2_LABEL.txt");
								::PutCommand("PG_2_LABEL.txt");
							usleep(100*1000);
							}
						break;
						case 8:		//��� �˴�
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_TEST.txt");
							::PutCommand("PG_2_TEST.txt");
							usleep(100*1000);
						break;
						case 12:	//�ѵ� ���|
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_PILE.txt");
							::PutCommand("PG_2_PILE.txt");
							usleep(100*1000);
						break;
						case 13:	//�ӳ��� �l�{��
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_SUBTECH.txt");
							::PutCommand("PG_2_SUBTECH.txt");
							usleep(100*1000);
						break;
						default:
							pwnd->SetPropValueT("formname","PG_1.txt");
							::PutCommand("PG_1.txt");
							usleep(100*1000);
						break;
					}
				}
				else
				{
					//pwnd->SetPropValueT("formname","PG_1.txt");
					::PutCommand("PG_1.txt");
					usleep(100*1000);
				}
			}
 		}
	}
	
	for(int i =0; i < iCheckBoxAct; i++)			//��ǰ�Ÿ�����ʾ����������ǰ�����
	{
		if(pwndCheckBoxAct[i] != NULL && pwnd == pwndCheckBoxAct[i])
		{			
			for(int k =0; k < iCheckBoxAct; k++)		//������ʾ
			{
				No = k+1+No1;
				if(No <6)
				{
					pwndStaticAct[k]->SetPropValueT("bgc",50712);
					//pwndCheckBoxAct[k]->SetPropValueT("bgc",50712);
				}
				else
				{
					pwndStaticAct[k]->SetPropValueT("bgc",0xFFDF);
					//pwndCheckBoxAct[k]->SetPropValueT("bgc",0xFFDF);
				}
				pwndStaticAct[k]->Update();
				//pwndCheckBoxAct[k]->Update();
			}
			((CtmFormView*)pwndSender)->Goto(pwndStaticAct[i]->GetTabOrder());
			//((CtmFormView*)pwndSender)->Goto(pwndCheckBoxAct[i]->GetTabOrder());
			pwndStaticAct[i]->SetPropValueT("bgc",36256);
			//pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
			
			if(pwndEditNo[i] != NULL)					//�������
			{
				pwndEditNo[i]->GetPropValueT("text", pNo, sizeof(pNo));
				sscanf(pNo,"%d",&SelectNo);
			}
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Update();
			//pwndCheckBoxAct[i]->Update();
		}
	}
	//UpdateText();
	
	if(pwnd == NULL)	return wIDControl;
	return TRUE;
}
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
 	
 	else if(pwnd ==pwndButton_Back)
	{
		::PutCommand("TeachOption.txt");
	}  
 	else if(pwnd == pwndBtnHome) // Home // cjlee 2019/4/17 �U�� 07:16:53
	{
		::PutCommand("Over.txt");
	}  
 	else if(pwnd == pwndBtnMenu) // Menu
	{
		::PutCommand("Index.txt");
	}  
 	else if(pwnd == pwndBtnBack) // Back
	{
		::PutCommand("Index.txt");
	}  

	UpdateText();
	return wIDControl;
 	
}

void    OnDestroyA(CtmWnd* pwndSender)
{
	//MsgBox(g_MultiLanguage["PICKER_DATADOWNLODING"], tmFT_CODE_TECH); // �ק粒�T�{�O�_�U�� 
    for(int i = 0; i < MAXSTEP*CONTROLTYPENUM; i++)
    {
        if(u_pszDBString[i] != NULL)
            free(u_pszDBString[i]);
        u_pszDBString[i] = NULL;
    }
    
}
void	PageDown(CtmWnd* pwndSender)		//��һҳ
{
	int		iPageturn	=10;
	int		SelectNoHelp =0;
	SelectNoHelp = SelectNo-No1;
	for (int i = 0; i < iEditNo; i++)
	{
		if(No1 + iPageturn +10> 100)
		{
			No1 = 90;
			iPageturn =0;
		}
		No = i+1+No1+iPageturn;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		pwndEditNo[i]->SetPropValueT("text",pNo);
		pwndEditNo[i]->Update();
		//printf("No=%d,",No);
		if(No<6)
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",50712);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
		}
		else
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",0xFFDF);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",0xFFDF);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pNoDataID, 0 ,sizeof(pNoDataID));
				sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",No);
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				int value = GetDBValue(pNoDataID).lValue;
				if(No>EditedStepNum)value=5; // ���s�誺�ʧ@
				sprintf(pStaticNo,"%d",value-5);
				//memset(pStaticNo, 0 ,sizeof(pStaticNo));
				//sprintf(pStaticNo,"%d",No-5);
				//printf("pStaticNo=%s\n",pStaticNo);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Update();
			}
		}
		if(pwndEditNo[0] != NULL)			//������
		{
			pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
			sscanf(pNo,"%d",&SelectNo);
			pwndStaticAct[0]->SetPropValueT("bgc",36256);
			//pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
		}
	}
	No1 = No1 +iPageturn;
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", No1);
	
	//((CtmFormView*)pwndSender)->Goto(pwndButtonPageDown->GetTabOrder());
}


void	PageUp(CtmWnd* pwndSender)			//��һҳ
{
	int		iPageturn	=10;
	int		SelectNoHelp =0;
	SelectNoHelp = SelectNo-No1;
	for (int i = 0; i < iEditNo; i++)
	{
		if(No1 -iPageturn  < 0)
		{
			No1 = 0;
			iPageturn =0;
		}
		No = i+1+No1-iPageturn;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		pwndEditNo[i]->SetPropValueT("text",pNo);
		pwndEditNo[i]->Update();
		if(No<6)
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",50712);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
		}
		else
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",0xFFDF);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",0xFFDF);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pNoDataID, 0 ,sizeof(pNoDataID));
				sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",No);
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				int value = GetDBValue(pNoDataID).lValue;
				if(No>EditedStepNum)value=5; // ���s�誺�ʧ@
				sprintf(pStaticNo,"%d",value-5);
				//memset(pStaticNo, 0 ,sizeof(pStaticNo));
				//sprintf(pStaticNo,"%d",No-5);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Update();
			}
		}
		if(pwndEditNo[0] != NULL)			//������
		{
			pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
			sscanf(pNo,"%d",&SelectNo);
			pwndStaticAct[0]->SetPropValueT("bgc",36256);
			//pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
		}
	}
	No1 = No1 -iPageturn;
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", No1);
	
}

void	Insert(int SelectNo)				//����
{
	char 	pDataID[256];
	int		value = 0;
	//for(int i=MaxDBNum;i>SelectNo;i--)// ��C�@�B����� ���U�@�B�� ����SelectNo�o�@�B
	for(int i=EditedStepNum+1;i>SelectNo;i--)// ��C�@�B����� ���U�@�B�� ����SelectNo�o�@�B
	{
		// �B�� ���U�� �٭n+1
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i-1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i);
		SetDBValue(pDataID, value+1);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i-1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i);
		SetDBValue(pDataID, value);

		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i-1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i);
		SetDBValue(pDataID, value);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i-1,j+1);
			value = GetDBValue(pDataID).lValue;
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i,j+1);
			SetDBValue(pDataID, value);
		}
	}
	// �s�W���o�B ��l��
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo-1);
	value = GetDBValue(pDataID).lValue;
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo);
	SetDBValue(pDataID, value+1);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",SelectNo);
	SetDBValue(pDataID, 0);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",SelectNo);
	SetDBValue(pDataID, 0);
	
	for(int j=0;j<PARA_NUM;j++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",SelectNo,j+1);
		SetDBValue(pDataID, 0);
	}
}

void	Delete(int SelectNo)				//ɾ��
{
	char 	pDataID[256];
	int		value = 0;
	//for(int i=SelectNo;i<MaxDBNum;i++) // �qSelectNo�}�l ��C�U�@�B��� ����o�@�B
	for(int i=SelectNo;i<EditedStepNum+1;i++) // �qSelectNo�}�l ��C�U�@�B��� ����o�@�B
	{
		// �B�Ʃ��W�� �٭n -1
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i);
		SetDBValue(pDataID, value-1);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i);
		SetDBValue(pDataID, value);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i);
		SetDBValue(pDataID, value);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i+1,j+1);
			value = GetDBValue(pDataID).lValue;
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i,j+1);
			SetDBValue(pDataID, value);
		}
	}
	
	//�̫�@�B ��l��
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",MaxDBNum);
	SetDBValue(pDataID, 0);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",MaxDBNum);
	SetDBValue(pDataID, 0);
	
	for(int j=0;j<PARA_NUM;j++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",MaxDBNum,j+1);
		SetDBValue(pDataID, 0);
	}
}

void	Clear()								//�����������
{
	printf("Clear\n");
	char 	pDataID[256];
	for(int i=6;i<MaxDBNum;i++)
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
	SetDBValue("MACHINE_PROFILE_NUM6_ACTION_TYPE", 5);
	SetDBValue("MACHINE_PROFILE_NUM6_ACTION_NUM", 1);
	SetDBValue("MACHINE_PROFILE_NUM7_ACTION_TYPE", 6);
	SetDBValue("MACHINE_PROFILE_NUM7_ACTION_NUM", 1);
	
	for(int i=0;i<7;i++) // Set ACTION_STEP �e7�B
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
		SetDBValue(pDataID, i+1);
	}
	EditedStepNum = 7;
}

void	Clean()								//�������
{
	printf("Clean\n");
	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
	char 	pDataID[256];
	int value =0;
	int CleanFlag =0;
	for(int i =0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		value = GetDBValue(pDataID).lValue;
		if(value ==0 && CleanFlag !=1)
		{
			Cleanhelp = i;
			CleanFlag = 1;
		}
		if(i>Cleanhelp && value !=0 && CleanFlag ==1)
		{
			UnSync(Cleanhelp+1);
			Delete(Cleanhelp+1);
			Clean();
		}
	}
	Prompt(g_MultiLanguage["MSG_DUMMY"]);
}

void	Download()							//�������������� �U����ƦܥD��
{
	printf("Download\n");
	char 	pDataID[256];
	int value =0;
	int Num =0;
	printf("MaxDBNum=%d\n",MaxDBNum);
	for(int i =0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		value = GetDBValue(pDataID).lValue;
		//printf("%s=%d\n",pDataID,value);
		if(value !=0) // ACTION_TYPE ����
		{
			//memset(pDataID, 0 ,sizeof(pDataID));
			//sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
			//SetDBValue(pDataID, i+1);
			//printf("Set mem %s=%d\n",pDataID,i+1);
			Num++; // �p�� �оɦ��X�B�J
		}
		else // �Ƨ��B��
		{
			if(g_ptaskpicker != NULL)
			{
				SetDBValue("MACHINE_PROFILE_STEPNUM", 0, TRUE);
				
				printf("Send cmd\n");
				char 	ptempDataID[256];
				for(int i =0;i<Num*CONTROLTYPENUM;i++)
				{
					memset(ptempDataID, 0 ,sizeof(ptempDataID));
					sprintf(ptempDataID,u_pszDBString[i]);
					printf("%s=%d\n",u_pszDBString[i],GetDBValue(ptempDataID));
					g_ptaskpicker->WriteValue(CONST_REQ_WRITE, 1 ,&u_pszDBString[i]);
				}
				//g_ptaskpicker->WriteValue(CONST_REQ_WRITE, Num*CONTROLTYPENUM ,u_pszDBString);
				
				SetDBValue("MACHINE_PROFILE_STEPNUM", Num, TRUE);
			}
			printf("Num=%d\n",Num);
			EditedStepNum = Num;
			return;
		}
	}
}

void	UpdateText()						//��s��ܦr��
{
	//printf("UpdateText\n");
	char 	pDataID[256];
	char 	pDataID2[256];
	char 	pDataID3[256];
	char 	pDataID4[256];
	char 	pDataID5[256];
	char 	pDataID6[256];
	char 	pDataID7[256];
	char 	pDataID8[256];
	char 	pDataID9[256];
	char 	pDataID10[256];
	char 	pDataID11[256];
	char 	pDataID12[256];
	char    szActPara1[256];
	char    szActPara2[256];
	char    szActPara3[256];
	char    szActPara5[256];
	int	ACTIONTYPE =0,ACTIONNUM =0;
	DWORD   wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
	// �� �r��X����� cjlee add 	
	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
	char	str[1024]; // ��ܦr���
	char	path[1024]; // ��ܦr���
	char* str_1;
	char* str_2;
	char* str_3;
	char* str_4;
	char* str_5;
	char* str_6;
	char* str_7;
	char* str_8;
	char* str_9;
	char* str_10;
	char* str_11;
	char* str_12;
	
	// �� �r��X����� cjlee add 
	for(int i=0;i<iCheckBoxAct;i++)
	{
		memset(str,0,sizeof(str));
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1+No1);
		memset(pDataID2, 0 ,sizeof(pDataID2));
		sprintf(pDataID2,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1+No1);
		sprintf(szActPara1,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i+1+No1); 
		sprintf(szActPara2,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i+1+No1); 
		sprintf(szActPara3,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3", i+1+No1); 
		sprintf(szActPara5,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1+No1); 
		ACTIONTYPE =GetDBValue(pDataID).lValue;
		ACTIONNUM =GetDBValue(pDataID2).lValue;
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		//printf("LINE%d, ACTIONTYPE:%d, ACTIONNUM:%d, Para1:%d, Para2:%d, Para3:%d, Para5:%d\n",i+1+No1,ACTIONTYPE,ACTIONNUM,wActPara1,wActPara2,wActPara3,wActPara5);
		memset(pDataID, 0 ,sizeof(pDataID));
		memset(pDataID2, 0 ,sizeof(pDataID2));
		memset(pDataID3, 0 ,sizeof(pDataID3));
		memset(pDataID4, 0 ,sizeof(pDataID4));
		memset(pDataID5, 0 ,sizeof(pDataID5));
		memset(pDataID6, 0 ,sizeof(pDataID6));
		memset(pDataID7, 0 ,sizeof(pDataID7));
		memset(pDataID8, 0 ,sizeof(pDataID8));
		memset(pDataID9, 0 ,sizeof(pDataID9));
		memset(pDataID10,0 ,sizeof(pDataID10));
		memset(pDataID11,0 ,sizeof(pDataID11));
		memset(pDataID12,0 ,sizeof(pDataID12));

		//printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
		switch(ACTIONTYPE)
		{
			case 0:		//
				sprintf(pDataID,"VW_PICKER_NULL");
				break;
			case 1:		//�b�ʧ@
				if(wActPara5) // ����
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "����"
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "��"
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X�b"
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y�b"
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z�b"
						break;
//					case 4: // cjlee 2019/4/6 �U�� 05:41:56
//						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2�b"
//						break;
//					case 5: // cjlee 2019/4/6 �U�� 05:41:56
//						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2�b"
//						break;
					default:
						break;
				}
				//if(ACTIONNUM <= 5) // ���b
				if(ACTIONNUM <= 3 || ACTIONNUM >= 5) // �T�b cjlee 2019/4/6 �U�� 05:41:56
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "�H"
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "���t�ײ��ʨ�"
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				//else if(ACTIONNUM == 6) // ���b
				else if(ACTIONNUM == 4)  // �T�bcjlee 2019/4/6 �U�� 05:41:56
				{
					if(wActPara2)
						sprintf(pDataID4,"PICKER_REMOTE_O_14");
					else
						sprintf(pDataID4,"PICKER_REMOTE_O_13");
					if(wActPara3)
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
					else
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				}
				break;
			case 2:		//����
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_WAIT");
				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 3:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_INNER_O_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 4:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_VALVE");
				sprintf(pDataID5,"DLG_DATETIME_COLON");
				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
				if(wActPara2)
				{
					sprintf(pDataID8,"PICKER_ACTTIME");
					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
					sprintf(pDataID10,"VW_INJPRO_SECOND");
				}
				break;
			case 5:		//����
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_START");
				}
				else
				{
					sprintf(pDataID4,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 6:		//����
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_END");
				}
				else
				{
					sprintf(pDataID4,"PICKER_JUMP");
					sprintf(pDataID5,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 8:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"VW_PUMP_TEST");
				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 12:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM)
					sprintf(pDataID4,"VW_PID_GROUP%d",ACTIONNUM);
				else
					sprintf(pDataID4,"VW_PICKER_NULL");
				sprintf(pDataID5,"VW_PICKER_PILE");
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 13:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_SUBTECH_1");
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID5,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID5,"PICKER_PROD_BAD");
						break;
					case 2:
						sprintf(pDataID5,"PICKER_PROD_SAMPLE");
						break;
					case 3:
						sprintf(pDataID5,"PICKER_PROD_TEST");
						break;
					case 4:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					case 5:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					default:
						break;
				}
				if(wActPara1)
					sprintf(pDataID6,"ACTIONPOINT_START");
				else
					sprintf(pDataID6,"ACTIONPOINT_END");
				sprintf(pDataID7,"PICKER_SUBTECH_2");
				sprintf(pDataID8,"%2d",wActPara2);
				break;
			default: 
				break;	
		}
		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
		// �� �r��X����� cjlee add 
		index_1 = g_MultiLanguage.GetStrKeyIndex(pDataID);
		index_2 = g_MultiLanguage.GetStrKeyIndex(pDataID2);	
		index_3 = g_MultiLanguage.GetStrKeyIndex(pDataID3);	
		index_4 = g_MultiLanguage.GetStrKeyIndex(pDataID4);
		index_5 = g_MultiLanguage.GetStrKeyIndex(pDataID5);	
		index_6 = g_MultiLanguage.GetStrKeyIndex(pDataID6);	
		index_7 = g_MultiLanguage.GetStrKeyIndex(pDataID7);
		index_8 = g_MultiLanguage.GetStrKeyIndex(pDataID8);	
		index_9 = g_MultiLanguage.GetStrKeyIndex(pDataID9);	
		index_10 = g_MultiLanguage.GetStrKeyIndex(pDataID10);
		index_11 = g_MultiLanguage.GetStrKeyIndex(pDataID11);	
		index_12 = g_MultiLanguage.GetStrKeyIndex(pDataID12);	
		
		if (index_1<0)
			index_1 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_2<0)
			str_2 = pDataID2;
		else
			str_2 = g_MultiLanguage[index_2];
		if (index_3<0)
			index_3 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_4<0)
			index_4 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_5<0)
			index_5 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_6<0)
			str_6 = pDataID6;
		else
			str_6 = g_MultiLanguage[index_6];
		if (index_7<0)
			index_7 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_8<0)
			str_8 = pDataID8;
		else
			str_8 = g_MultiLanguage[index_8];
		if (index_9<0)
			index_9 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_10<0)
			index_10 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_11<0)
			str_11= pDataID11;
		else
			str_11 = g_MultiLanguage[index_11];
		if (index_12<0)
			index_12 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		//printf("Line%d = %d  %d  %d %d  %d  %d %d  %d  %d %d  %d  %d\n",i+1+No1,index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12);
		
		str_1 = g_MultiLanguage[index_1];
		//str_2 = g_MultiLanguage[index_2];
		str_3 = g_MultiLanguage[index_3];
		str_4 = g_MultiLanguage[index_4];
		str_5 = g_MultiLanguage[index_5];
		//str_6 = g_MultiLanguage[index_6];
		str_7 = g_MultiLanguage[index_7];
		//str_8 = g_MultiLanguage[index_8];
		str_9 = g_MultiLanguage[index_9];
		str_10 = g_MultiLanguage[index_10];
		//str_11 = g_MultiLanguage[index_11];
		str_12 = g_MultiLanguage[index_12];
		
		memset(str,0,sizeof(str));
		//printf("sl_1=%d, sl_2=%d, sl_3=%d, sl_4=%d, sl_5=%d, sl_6=%d, sl_7=%d, sl_8=%d, sl_9=%d, sl_10=%d, sl_11=%d, sl_12=%d\n",strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
		strncat(str, str_1, strlen(str_1));
		strncat(str, str_2, strlen(str_2));
		strncat(str, str_3, strlen(str_3));
		strncat(str, str_4, strlen(str_4));
		strncat(str, str_5, strlen(str_5));
		strncat(str, str_6, strlen(str_6));
		strncat(str, str_7, strlen(str_7));
		strncat(str, str_8, strlen(str_8));
		strncat(str, str_9, strlen(str_9));
		strncat(str, str_10, strlen(str_10));
		strncat(str, str_11, strlen(str_11));
		strncat(str, str_12, strlen(str_12));
		//printf("Line%d = %s\n",i+1+No1-5,str);
		//printf("strlen=%d\n",strlen(str));
		//strncpy(path, str, strlen(str)+1);
		//path[strlen(str)]= '\0';
		//printf("path=%d\n",path);
/*
		memset(str,0,sizeof(str));
		printf("strlen:%d\n",strlen(str));
		memcpy(str,str_1,strlen(str_1));
		memcpy(str+strlen(str_1),str_2,strlen(str_2));
		memcpy(str+strlen(str_1)+strlen(str_2),str_3,strlen(str_3));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3),str_4,strlen(str_4));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4),str_5,strlen(str_5));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5),str_6,strlen(str_6));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6),str_7,strlen(str_7));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7),str_8,strlen(str_8));		
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8),str_9,strlen(str_9));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9),str_10,strlen(str_10));	
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10),str_11,strlen(str_11));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11),str_12,strlen(str_12));
		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11)+strlen(str_12),"",strlen(""));
		printf("%d str%d strlen: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",i+1+No1,strlen(str),strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
*/
		if(pwndCheckBoxAct[i] != NULL)
		{
			//pwndCheckBoxAct[i]->SetPropValueT("captionID","");
			//pwndCheckBoxAct[i]->CreateA();
			//pwndCheckBoxAct[i]->Show();
			//pwndCheckBoxAct[i]->Update();
			//pwndCheckBoxAct[i]->UpdateAll();
		}
		if(pwndStaticAct[i] != NULL)
		{
			RegionUpdateStop_APP = TRUE; //�����W�ߨ�s�A�O������s�d��
			
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			pwndStaticAct[i]->UpdateAll();
			
			ChangePos(1);  //�N����s�������@����s
			

		}
		//pwndCheckBoxAct[i]->Update();
		// �� �r��X����� cjlee add 
		/*
		pwndCheckBoxAct[i]->SetPropValueT("captionID1", pDataID);printf("pDataID1:%s",pDataID);
		pwndCheckBoxAct[i]->SetPropValueT("captionID2", pDataID2);printf("pDataID2:%s",pDataID2);
		pwndCheckBoxAct[i]->SetPropValueT("captionID3", pDataID3);printf("pDataID3:%s",pDataID3);
		pwndCheckBoxAct[i]->SetPropValueT("captionID4", pDataID4);printf("pDataID4:%s",pDataID4);
		pwndCheckBoxAct[i]->SetPropValueT("captionID5", pDataID5);
		pwndCheckBoxAct[i]->SetPropValueT("captionID6", pDataID6);
		pwndCheckBoxAct[i]->SetPropValueT("captionID7", pDataID7);
		pwndCheckBoxAct[i]->SetPropValueT("captionID8", pDataID8);
		pwndCheckBoxAct[i]->SetPropValueT("captionID9", pDataID9);
		pwndCheckBoxAct[i]->SetPropValueT("captionID10", pDataID10);
		pwndCheckBoxAct[i]->SetPropValueT("captionID11", pDataID11);
		pwndCheckBoxAct[i]->SetPropValueT("captionID12", pDataID12);
		pwndCheckBoxAct[i]->CreateA();
		pwndCheckBoxAct[i]->Update();
		pwndCheckBoxAct[i]->UpdateAll();*/		
	}
}

void	Sync(int SelectNo) // �X�ְʧ@
{
	printf("SelectNo=%d\n",SelectNo);
		
	char 	pDataID[256];
	int 	ACTION_STEP = 0,ACTION_STEP_Pre = 0,value = 0;;
	
	// ���o�W�@�BACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo-1);
	ACTION_STEP_Pre = GetDBValue(pDataID).lValue;
	// ���o�o�@�BACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo);
	ACTION_STEP = GetDBValue(pDataID).lValue;
	if (ACTION_STEP !=ACTION_STEP_Pre)
		{
			SetDBValue(pDataID, ACTION_STEP_Pre);
			printf("Set No%d Step=%d\n",ACTION_STEP,ACTION_STEP_Pre);
			for(int i=MaxDBNum;i>SelectNo;i--) // ���B�H�U�B�J�ҤW���@��ACTION_STEP
			{
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i);
				value = GetDBValue(pDataID).lValue;
				SetDBValue(pDataID, value-1);
			}
			UpdateNo();
		}
}

void	UnSync(int SelectNo) // ���Ѱʧ@
{
	printf("SelectNo=%d\n",SelectNo);
	char 	pDataID[256];
	int 	ACTION_STEP = 0,ACTION_STEP_Pre = 0,value = 0;;
	
	// ���o�W�@�BACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo-1);
	ACTION_STEP_Pre = GetDBValue(pDataID).lValue;
	// ���o�o�@�BACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",SelectNo);
	ACTION_STEP = GetDBValue(pDataID).lValue;
	if (ACTION_STEP == ACTION_STEP_Pre)
		{
			SetDBValue(pDataID, ACTION_STEP+1);
			//printf("Set No%d Step=%d\n",ACTION_STEP,ACTION_STEP+1);
			for(int i=MaxDBNum;i>SelectNo;i--) // ���B�H�U�B�J�ҤU���@��ACTION_STEP
			{
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i);
				value = GetDBValue(pDataID).lValue;
				SetDBValue(pDataID, value+1);
			}
			UpdateNo();
		}
}
void	UpdateNo()
{
	int value=0;
	for (int i = 0; i < iEditNo; i++)	//��ѧǸ�
	{
		No = i+1+No1;
		if(No<6)
		{
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("bgc",NoColor[1]);
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
		}
		else
		{
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pNoDataID, 0 ,sizeof(pNoDataID));
				sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",No);
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				value = GetDBValue(pNoDataID).lValue;
				//printf("Get %s=%d\n",pNoDataID,value);
				if(No>EditedStepNum)value=5; // ���s�誺�ʧ@
				sprintf(pStaticNo,"%d",value-5);				
				pwndStaticEditNo[i]->SetPropValueT("bgc",NoColor[((value)%2)]);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Update();
			}
		}
	}
}
