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
#include	"ViewSUB_PG_0.h"
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
#define     MAXSTEP               20
#define     STATE_SINGLESTEP        2
#define		STATE_HAND				3
#define     STATE_FULLAUTO          4 
#define     PARA_NUM	            10 
long		u_SubGroup =0; // 副程式號碼
char*	SubName[] = // 副程式名稱 對照畫面腳本標題
{
	"",
	"SUB_PG_PROD_BAD",
	"SUB_PG_PROD_SAMPLE",
	"SUB_PG_PROD_TEST",
	"SUB_PG_PROD_RES1",
	"SUB_PG_PROD_RES1",
};
int     iEditNo=0,iCheckBoxAct=0,iEditACTION=0,iSelectEditACTION=0,iStaticACTION=0,iStaticEditNo=0;
int 	iStaticAct=0;
int 	No1=0,No=0;
int		SelectNo = 0;
int		Cleanhelp =0;
int		MaxDBNum = 20;
int		EditedStepNum = 0;
bool	m_bEnterKey = FALSE;
char 	pNo[256]	={NULL}; 
char 	pStaticNo[256]	={NULL}; 
char	pNoDataID[256] = {NULL};
WORD        u_wPickerOPSatus    = 0;
WORD        wStep  = 0,wStepOld;
WORD        wSingle_Step  = 0,wSingle_StepOld;
char*       u_pszDBString[MAXSTEP*CONTROLTYPENUM] = {NULL};

CtmWnd*		pwndEditNo[32] 	={NULL}; 		//唗瘍蹈 序號列
CtmWnd*		pwndStaticEditNo[32] 	={NULL}; 		//唗瘍蹈 序號列
CtmWnd*		pwndCheckBoxAct[32] ={NULL}; 	//雄釬蹈 動作列
CtmWnd*		pwndStaticAct[32] ={NULL}; 	//動作列文字
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

CtmWnd*		pwndButtonDownload	= NULL; // 下載按鈕
CtmWnd*		pwndLoadingStr	= NULL; // 下載進度文字
CtmWnd*		pwndLoadingBar	= NULL; // 下載進度條
CtmWnd*		pwndLoadingMask	= NULL; // 下載進度條遮罩
CtmWnd*		pwndLoadingMask2	= NULL; // 下載進度條遮罩2

CtmWnd* u_pwndSingle = NULL;
int     iNumText1=0,iNumText2=0,iNumText3=0,iNumText4=0,iNumText5=0;


long	NoColor[] =
{
	0x8627,
	0xFFFF,//0xFFDF,
};
long	BgColor[] =
{
	0x8627,
	0xFFFF,//0xFFDF,
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
	// 取得標題名稱
	char	m_pszTitleID[256];	memset(m_pszTitleID, 0, sizeof(m_pszTitleID));
	pwndSender->GetPropValueT("titleID", m_pszTitleID, sizeof(m_pszTitleID));
	printf("Now at %s\n",m_pszTitleID);
	// 取得副程式號碼 (SUB幾)
	for(int i=0; i<sizeof(SubName)/sizeof(SubName[0]); i++)
	{
		if(strcmp(m_pszTitleID, SubName[i]) == 0)
			u_SubGroup=i;
	}
	printf("SubGroup=%d\n",u_SubGroup);
	
	{	//初始定義
		iEditNo 	 			= GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);					//唗瘍蹈杅講 序號列數量
		iStaticEditNo 	 			= GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", pwndStaticEditNo);	//唗瘍蹈杅講 序號列數量
		iCheckBoxAct				= GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);	//雄釬蹈杅講 動作列數量
		iStaticAct				= GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", pwndStaticAct);	//雄釬蹈杅講 動作列數量
		iEditACTION				= GetSpecialControlNum(pwndSender, "EditACTION", "CtmEditX1", pwndEditACTION);
		iSelectEditACTION				= GetSpecialControlNum(pwndSender, "SelectEditACTION", "CtmSelectEdit", pwndSelectEditACTION);
		iStaticACTION				= GetSpecialControlNum(pwndSender, "StaticACTION", "CtmStaticX2", pwndStaticACTION);
		No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50").lValue;	
		SelectNo = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue;	
		//printf("No1=%d,SelectNo=%d\n ",No1,SelectNo);
		//SetDBValue("MACHINE_PROFILE_STEPNUM", 0);		// 數量清零
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
		
		pwndLoadingStr  = pwndSender->FindControlFromName("LoadingStr");
		pwndLoadingBar  = pwndSender->FindControlFromName("LoadingBar");
		pwndLoadingMask = pwndSender->FindControlFromName("LoadingMask");
		pwndLoadingMask2 = pwndSender->FindControlFromName("LoadingMask2");
		pwndButtonDownload  = pwndSender->FindControlFromName("ButtonDownload");
		
	}
	
	int nStep = 0;
	for(int i = 1; i <= MAXSTEP; i++ )
    {       
        u_pszDBString[nStep] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i);
        u_pszDBString[nStep+1] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+1], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup, i);
        u_pszDBString[nStep+2] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+2], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup, i);
        u_pszDBString[nStep+3] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+3], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup, i);  
        u_pszDBString[nStep+4] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+4], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup, i); 
        u_pszDBString[nStep+5] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+5], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup, i);  
        u_pszDBString[nStep+6] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+6], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER4",u_SubGroup, i);  
        u_pszDBString[nStep+7] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+7], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5",u_SubGroup, i);  
        u_pszDBString[nStep+8] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+8], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER6",u_SubGroup, i);  
        u_pszDBString[nStep+9] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+9], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER7",u_SubGroup, i);  
        u_pszDBString[nStep+10] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+10], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER8",u_SubGroup, i);  
        u_pszDBString[nStep+11] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+11], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER9",u_SubGroup, i);  
        u_pszDBString[nStep+12] = (char*)malloc(256);
        sprintf(u_pszDBString[nStep+12], "MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER10",u_SubGroup, i);  
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
	// ↓ 字串合併顯示 cjlee add 	
	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
	char	str[1024]; // 顯示字串用
	char	path[1024]; // 顯示字串用
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
	
	// ↑ 字串合併顯示 cjlee add 
	for(int i=0;i<iCheckBoxAct;i++)
	{
		memset(str,0,sizeof(str));
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1+No1);
		memset(pDataID2, 0 ,sizeof(pDataID2));
		sprintf(pDataID2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1+No1);
		sprintf(szActPara1,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup, i+1+No1); 
		sprintf(szActPara2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup, i+1+No1); 
		sprintf(szActPara3,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup, i+1+No1); 
		sprintf(szActPara5,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5",u_SubGroup, i+1+No1); 
		ACTIONTYPE =GetDBValue(pDataID).lValue;
		ACTIONNUM =GetDBValue(pDataID2).lValue;
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		
		sprintf(ACTION_STEP_ID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup, i+1+No1);
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
			case 1:		//軸動作
				if(wActPara5) // 延時
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "延時"
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "秒"
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X軸"
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y軸"
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z軸"
						break;
					case 4: // cjlee 2019/4/6 下午 05:44:41
						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2軸"
						break;
					case 5: // cjlee 2019/4/6 下午 05:44:41
						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2軸"
						break;
					default:
						break;
				}
				if(ACTIONNUM <= 5)//if(ACTIONNUM <= 3 || ACTIONNUM >= 5) // cjlee 2019/4/6 下午 05:44:41
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "以"
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "的速度移動到"
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");// "mm的位置"
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7"); // "提前"
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM"); // "mm"
					}
				}
				else if(ACTIONNUM == 6)//(ACTIONNUM == 4) // cjlee 2019/4/6 下午 05:44:41
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
			case 2:		//等待
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_WAIT");
				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5"); // "打開"
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6"); // "確認"
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
			case 4:		//閥門
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
				if(wActPara2) // "檢測"
					sprintf(pDataID8,"VW_PUMP_TEST");
//				if(wActPara2)
//				{
//					sprintf(pDataID8,"PICKER_ACTTIME");
//					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
//					sprintf(pDataID10,"VW_INJPRO_SECOND");
//				}
				break;
			case 5:		//標籤
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
			case 6:		//跳轉
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
			case 14: // 副程式結束
				sprintf(pDataID4,"STR_SUB_END");
				break;
			default: 
				break;	
		}
		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
		// ↓ 字串合併顯示 cjlee add 
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
			RegionUpdateStop_APP = TRUE; //停止元件獨立刷新，記錄未刷新範圍
			
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			pwndStaticAct[i]->UpdateAll();
			
			ChangePos(1);  //將未刷新的部分一次刷新
			

		}
		//pwndCheckBoxAct[i]->Update();
		// ↑ 字串合併顯示 cjlee add 
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
	for (int i = 0; i < iEditNo; i++)	//鳳��唗瘍 獲參序號
	{
		No = i+1+No1;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		if(pwndEditNo[i] != NULL)
		{
			pwndEditNo[i]->SetPropValueT("text",pNo);
			pwndEditNo[i]->Update();
		}

		{
			RegionUpdateStop_APP = TRUE; //停止元件獨立刷新，記錄未刷新範圍
			
			memset(pNoDataID, 0 ,sizeof(pNoDataID));
			sprintf(pNoDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,No);
			memset(pStaticNo, 0 ,sizeof(pStaticNo));
			int value = GetDBValue(pNoDataID).lValue;
			if(No>EditedStepNum)value=0; // 未編輯的動作
			sprintf(pStaticNo,"%d",value);
			
			//memset(pStaticNo, 0 ,sizeof(pStaticNo));
			//sprintf(pStaticNo,"%d",No-5);
			pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
			pwndStaticEditNo[i]->Show();
			ChangePos(1);  //將未刷新的部分一次刷新
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
	if(pwndEditNo[0] != NULL && !SelectFlag)			//嫖梓詢謠
	{
		pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
		sscanf(pNo,"%d",&SelectNo);
		pwndStaticAct[0]->SetPropValueT("bgc",36256);
		//pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
	}
	
	char 	pDataID[256],pDataID2[256];
	int		type = 0,num = 0;
	for(int i =0;i<MaxDBNum;i++) // 計數有編寫的教導有幾步驟
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1);
		type = GetDBValue(pDataID).lValue;
		memset(pDataID2, 0 ,sizeof(pDataID));
		sprintf(pDataID2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1);
		num = GetDBValue(pDataID2).lValue;

		if(type ==14 && num ==1) // "副程式結束"type=14 num=1
		{
			EditedStepNum=i+1;
		}
	}
	printf("EditedStepNum=%d\n",EditedStepNum);
	
	UpdateText();
	UpdateNo();
	Update_Download_Hint();
	
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
							for(int k =0; k < iCheckBoxAct; k++)		//詢謠珆尨
							{
								No = k+1+No1;
								{
									pwndStaticAct[k]->SetPropValueT("bgc",BgColor[1]);
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
	printf("OnKey = %d\n",wKey);
	int         nCMDValue       = 0xFF0D;
	int         nCMDLastStep    = 0xFF26;
	int         nCMDSingleCycle = 0xFF25;
	char 	pDataID[256];
	int		ActionType = 0, ACTIONNUM = 0;
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,SelectNo);
	ActionType =GetDBValue(pDataID).lValue;
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup, SelectNo); 
	ACTIONNUM =GetDBValue(pDataID).lValue;
	if(u_wPickerOPSatus != STATE_FULLAUTO) //赻雄狟癹秶
	{
		if (wKey == 0x000B)		// 下一頁
		{
			PageDown(pwndSender);
			UpdateText();
			UpdateNo();
			return _NULL_KEY;
		}
		else if (wKey == 0x000A)	// 上一頁
		{
			//printf("PageUp\n");
			PageUp(pwndSender);
			UpdateText();
			UpdateNo();
			return _NULL_KEY;
		}
		else if (wKey == 0x0002)	//插入��
		{
			if(EditedStepNum+1>19)//超過步驟數量
			{
				printf("Over Step Num\n");
			}
			else if(SelectNo <= EditedStepNum)
			{
					Insert(SelectNo);
					EditedStepNum++;
					//UpdateText();
					UpdateNo();
			}
		}
		else if (wKey == 0x0003)	//刪除
		{
			if(SelectNo <= EditedStepNum)
			{
				if(ActionType != 14) // 副程式結束(14) 不能刪除
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
		else if (wKey == 0x0007)	//ь清理
		{
			Cleanhelp =0;
			Clean();
			UpdateText();
		}
		else if (wKey == 0x0008)	//ь清除資料
		{
			MsgBox(g_MultiLanguage["PICKER_CONFIRMDELETE"], tmFT_CODE_TECH);
		    if(g_bMsgBoxFlag)
		    {
		    	Clear();
		    	UpdateText();
		    	UpdateNo();
		    }
		}
		else if (wKey == 0x0004)	//下載
		{
			if(u_wPickerOPSatus == STATE_HAND)
			{
				Cleanhelp =0;
				Clean();
				Prompt(g_MultiLanguage["PICKER_DATADOWNLODING"],1); 
				GetPosTag();
				Download();
				g_Hint_Download = 0; // 完成下載
				Update_Download_Hint();
				UpdateNo();
				UpdateText();
				Prompt(g_MultiLanguage["PICKER_DATADOWNLOADFINISH"],1); 
			}
			else
			{
				MsgBox(g_MultiLanguage["PICKER_M3_ALARM1042"], tmFT_CODE_TECH);
			}
		}
		else if(wKey == 0x0001) //下一步
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
    else if(wKey == 0x0009)// 上一步
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
    else if(wKey == 0x000C) // 合併
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
    else if(wKey == 0x000D) // 分解
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
//    else if(wKey == 0x000E) // 執行單步
//    	{
//    		::PutCommand("PG_0_SINGLE.txt");
//    	}   
//    else if(wKey == 0x0010) // 結束單步
//    	{
//    		::PutCommand("PG_SUB_0.txt");
//    	}  
	}

	switch(wKey) // 副程式選擇
	{
		case 0x0033:
    		::PutCommand("SUB_PG_0_1.txt");
			break;	
		case 0x0034:
    		::PutCommand("SUB_PG_0_2.txt");
			break;	
		case 0x0035:
			::PutCommand("SUB_PG_0_3.txt");
			break;
		case 0x0036:
			::PutCommand("SUB_PG_0_4.txt");
			break;
		case 0x0037:
			::PutCommand("SUB_PG_0_5.txt");
			break;
		default:
			break;
	}

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);

	if(u_wPickerOPSatus != STATE_FULLAUTO) //自動下限制
	{
		if(pwnd == pwndButtonStartEdit)
 		{
 			if(SelectNo < EditedStepNum)
			{
	 			printf("Start Edit\n");

	 			char 	pDataID[256];
				int		ActionType = 0, ACTIONNUM = 0;
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,SelectNo);
				ActionType =GetDBValue(pDataID).lValue;
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup, SelectNo); 
				ACTIONNUM =GetDBValue(pDataID).lValue;
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED47", u_SubGroup); // 紀錄副程式編號
	 			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51", SelectNo); // 紀錄選擇的序號
	 			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType); // cjlee 確保儲存動作類型 2019/2/20 下午 08:13:15
	 			printf("SUB_PG0 Set u_SubGroup%d SelectNo%d ActionType=%d\n", u_SubGroup,SelectNo,ActionType); 
				if(ActionType != 0)
				{
					switch(ActionType)
					{
						case 1:		//軸動作
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_PG_2_AXISACT.txt");
							usleep(100*1000);
						break;
						case 2:		//等待
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_PG_2_WAIT.txt");
							usleep(100*1000);
						break;
						case 3:		//允許
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_PG_2_PERMIT.txt");
							usleep(100*1000);
						break;
						case 4:		// 閥門
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_PG_2_VALVE.txt");
							usleep(100*1000);
						break;
						case 5:		// 標籤
							if(ACTIONNUM != 1)
							{
								SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
								::PutCommand("SUB_PG_2_LABEL.txt");
							usleep(100*1000);
							}
						break;
						case 6:		// 跳轉
							if(ACTIONNUM != 1)
							{
								SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
								::PutCommand("SUB_PG_2_LABEL.txt");
							usleep(100*1000);
							}
						break;
						case 8:		// 檢測
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_PG_2_TEST.txt");
							usleep(100*1000);
						break;
						case 12:	// 堆疊
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_SUB_PG_2_PILE.txt");
							usleep(100*1000);
						break;
						case 13:	// 副程式
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52", ActionType);
							::PutCommand("SUB_SUB_PG_2_SUBTECH.txt");
							usleep(100*1000);
						break;
						default:
							pwnd->SetPropValueT("formname","SUB_PG_1.txt");
							::PutCommand("SUB_PG_1.txt");
							usleep(100*1000);
						break;
					}
				}
				else
				{
					::PutCommand("SUB_PG_1.txt");
					usleep(100*1000);
				}
			}
 		}
	}
	
	for(int i =0; i < iCheckBoxAct; i++)			//絞ヶ齬詢謠珆尨ㄛ甜鳳��絞ヶ齬唗瘍
	{
		if(pwndCheckBoxAct[i] != NULL && pwnd == pwndCheckBoxAct[i])
		{			
			for(int k =0; k < iCheckBoxAct; k++)		//詢謠珆尨
			{
				No = k+1+No1;
					pwndStaticAct[k]->SetPropValueT("bgc",BgColor[1]);
				pwndStaticAct[k]->Update();
				//pwndCheckBoxAct[k]->Update();
			}
			((CtmFormView*)pwndSender)->Goto(pwndStaticAct[i]->GetTabOrder());
			//((CtmFormView*)pwndSender)->Goto(pwndCheckBoxAct[i]->GetTabOrder());
			pwndStaticAct[i]->SetPropValueT("bgc",36256);
			//pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
			
			if(pwndEditNo[i] != NULL)					//鳳��唗瘍
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
	printf("OnMouseUp\n");
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
 	

 	
	UpdateText();
	return wIDControl;
 	
}

void    OnDestroyA(CtmWnd* pwndSender)
{
	//MsgBox(g_MultiLanguage["PICKER_DATADOWNLODING"], tmFT_CODE_TECH); // 修改完確認是否下載 
    for(int i = 0; i < MAXSTEP*CONTROLTYPENUM; i++)
    {
        if(u_pszDBString[i] != NULL)
            free(u_pszDBString[i]);
        u_pszDBString[i] = NULL;
    }
    
}
void	PageDown(CtmWnd* pwndSender)		//狟珨珜
{
	int		iPageturn	=10;
	int		SelectNoHelp =0;
	SelectNoHelp = SelectNo-No1;
	for (int i = 0; i < iEditNo; i++)
	{
		if(No1 + iPageturn +10> 20)
		{
			No1 = 10;
			iPageturn =0;
		}
		No = i+1+No1+iPageturn;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		pwndEditNo[i]->SetPropValueT("text",pNo);
		pwndEditNo[i]->Update();
		//printf("No=%d,",No);
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",BgColor[1]);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",0xFFDF);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pNoDataID, 0 ,sizeof(pNoDataID));
				sprintf(pNoDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,No);
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				int value = GetDBValue(pNoDataID).lValue;
				if(No>EditedStepNum)value=0; // 未編輯的動作
				sprintf(pStaticNo,"%d",value);
				//memset(pStaticNo, 0 ,sizeof(pStaticNo));
				//sprintf(pStaticNo,"%d",No-5);
				//printf("pStaticNo=%s\n",pStaticNo);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Show();
			}
		}
		if(pwndEditNo[0] != NULL)			//嫖梓詢謠
		{
			pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
			sscanf(pNo,"%d",&SelectNo);
			pwndStaticAct[0]->SetPropValueT("bgc",36256);
			//pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
		}
	}
	No1 = No1 +iPageturn;
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50", No1);
	
	//((CtmFormView*)pwndSender)->Goto(pwndButtonPageDown->GetTabOrder());
}


void	PageUp(CtmWnd* pwndSender)			//奻珨珜
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
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",BgColor[1]);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",0xFFDF);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pNoDataID, 0 ,sizeof(pNoDataID));
				sprintf(pNoDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,No);
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				int value = GetDBValue(pNoDataID).lValue;
				if(No>EditedStepNum)value=0; // 未編輯的動作
				sprintf(pStaticNo,"%d",value);
				//memset(pStaticNo, 0 ,sizeof(pStaticNo));
				//sprintf(pStaticNo,"%d",No-5);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Show();
			}
		}
		if(pwndEditNo[0] != NULL)			//嫖梓詢謠
		{
			pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
			sscanf(pNo,"%d",&SelectNo);
			pwndStaticAct[0]->SetPropValueT("bgc",36256);
			//pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
		}
	}
	No1 = No1 -iPageturn;
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50", No1);
	
}

void	Insert(int SelectNo)				// 插入��
{
	printf("Insert %d\n",SelectNo);
	char 	pDataID[256];
	int		value = 0;
	//for(int i=MaxDBNum;i>SelectNo;i--)// 把每一步的資料 往下一步移 直到SelectNo這一步
	for(int i=EditedStepNum+1;i>SelectNo;i--)// 把每一步的資料 往下一步移 直到SelectNo這一步
	{
		// 步數 往下移 還要+1
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i-1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i);
		SetDBValue(pDataID, value+1);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i-1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i);
		SetDBValue(pDataID, value);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i-1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i);
		SetDBValue(pDataID, value);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,i-1,j+1);
			value = GetDBValue(pDataID).lValue;
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,i,j+1);
			SetDBValue(pDataID, value);
		}
	}
	// 新增的這步 初始化
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,SelectNo-1);
	value = GetDBValue(pDataID).lValue;
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,SelectNo);
	SetDBValue(pDataID, value+1);
	printf("Set %s=%d\n",pDataID,value+1);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,SelectNo);
	SetDBValue(pDataID, 0);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,SelectNo);
	SetDBValue(pDataID, 0);
	
	for(int j=0;j<PARA_NUM;j++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,SelectNo,j+1);
		SetDBValue(pDataID, 0);
	}
}

void	Delete(int SelectNo)				// 刪除
{
	char 	pDataID[256];
	int		value = 0;
	//for(int i=SelectNo;i<MaxDBNum;i++) // 從SelectNo開始 把每下一步資料 移到這一步
	for(int i=SelectNo;i<EditedStepNum+1;i++) // 從SelectNo開始 把每下一步資料 移到這一步
	{
		// 步數往上移 還要 -1
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i+1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i);
		SetDBValue(pDataID, value-1);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i);
		SetDBValue(pDataID, value);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1);
		value = GetDBValue(pDataID).lValue;
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i);
		SetDBValue(pDataID, value);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,i+1,j+1);
			value = GetDBValue(pDataID).lValue;
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,i,j+1);
			SetDBValue(pDataID, value);
		}
	}
	
	//最後一步 初始化
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,MaxDBNum);
	SetDBValue(pDataID, 0);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,MaxDBNum);
	SetDBValue(pDataID, 0);
	
	for(int j=0;j<PARA_NUM;j++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,MaxDBNum,j+1);
		SetDBValue(pDataID, 0);
	}
}

void	Clear()								// 清除資料
{
	printf("Clear\n");
	char 	pDataID[256];
	for(int i=0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i+1);
		SetDBValue(pDataID, 0);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1);
		SetDBValue(pDataID, 0);
		
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1);
		SetDBValue(pDataID, 0);
		
		for(int j=0;j<PARA_NUM;j++)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER%d",u_SubGroup,i+1,j+1);
			SetDBValue(pDataID, 0);
		}
	}
	// 設定副程式結束
	printf("Set Sub Clean init\n");
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM1_ACTION_TYPE",u_SubGroup);
	SetDBValue(pDataID, 14); // 副程式結束
	printf("Set %s=%d\n",pDataID,14);
	
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM1_ACTION_NUM",u_SubGroup);
	SetDBValue(pDataID, 1);
	printf("Set %s=%d\n",pDataID,1);
	
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM1_ACTION_STEP",u_SubGroup);
	SetDBValue(pDataID, 1); // 序號 = 1
	printf("Set %s=%d\n",pDataID,1);
	EditedStepNum = 1;
}

void	Clean()								//
{
	printf("Clean\n");
	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
	char 	pDataID[256];
	int value =0;
	int CleanFlag =0;
	for(int i =0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1);
		value = GetDBValue(pDataID).lValue;
		printf("%s=%d\n",pDataID,value);
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

void	Download()							//下載資料至主機
{
	printf("Download\n");
	char 	pDataID[256];
	int value =0;
	int Num =0;
	char  str_loading[256] = "\0"; // 下載進度
	int u_loading=0;
	printf("MaxDBNum=%d\n",MaxDBNum);
	
	pwndButtonDownload->SetPropValueT("taborder", -2); // "下載"按鍵上鎖
	pwndLoadingBar->SetPropValueT("right",24); // 重置進度條
	pwndLoadingBar->CreateA();
	pwndLoadingBar->Update();
	pwndLoadingMask->Update();
	pwndLoadingMask2->Update();
	pwndLoadingStr->SetPropValueT("bgc",0xF7BE); // 進度值
	pwndLoadingStr->SetPropValueT("fgc",0x0001);
	pwndLoadingStr->Update();
	
	for(int i =0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1);
		value = GetDBValue(pDataID).lValue;
		//printf("%s=%d\n",pDataID,value);
		if(value !=0) // ACTION_TYPE 有值
		{
			Num++; // 計數 教導有幾步驟
		}
		else // 數完步數
		{
			printf("Num=%d\n",Num);
			if(g_ptaskpicker != NULL) // 傳送教導至28
			{
				char 	pSubStepNum_DBID[256];
				memset(pSubStepNum_DBID, 0 ,sizeof(pSubStepNum_DBID));
				sprintf(pSubStepNum_DBID,"MACHINE_PROFILE_SUB%d_STEPNUM",u_SubGroup);
				SetDBValue(pSubStepNum_DBID, 0, TRUE);
				
				printf("Send cmd\n");
				char 	ptempDataID[256];
				for(int i =0;i<Num*CONTROLTYPENUM;i++)
				{
					if((i%13)<8) // 參數6以後不送
					{
						memset(ptempDataID, 0 ,sizeof(ptempDataID));
						sprintf(ptempDataID,u_pszDBString[i]);
						printf("%s=%d\n",u_pszDBString[i],GetDBValue(ptempDataID));
						g_ptaskpicker->WriteValue(CONST_REQ_WRITE, 1 ,&u_pszDBString[i]); // 寫到28
					}
					
					if(pwndLoadingStr!=NULL && pwndLoadingBar!=NULL)//顯示下載進度
					{
						pwndLoadingBar->SetPropValueT("right",152+u_loading*4);
						pwndLoadingBar->CreateA();
						pwndLoadingBar->Update();
						u_loading = 1+((100*i)/(Num*CONTROLTYPENUM));
						sprintf(str_loading ,"%d",u_loading+1);
						if(u_loading>50)
							{
								pwndLoadingStr->SetPropValueT("bgc",0x09A6);
								pwndLoadingStr->SetPropValueT("fgc",0xFFFF);
								pwndLoadingStr->CreateA();
							}
						pwndLoadingStr->SetPropValueT("text",str_loading);
						pwndLoadingStr->Update();
					}
				}
				//g_ptaskpicker->WriteValue(CONST_REQ_WRITE, Num*CONTROLTYPENUM ,u_pszDBString);
				
				memset(pSubStepNum_DBID, 0 ,sizeof(pSubStepNum_DBID));
				sprintf(pSubStepNum_DBID,"MACHINE_PROFILE_SUB%d_STEPNUM",u_SubGroup);
				SetDBValue(pSubStepNum_DBID, Num, TRUE);
			}
			pwndButtonDownload->SetPropValueT("taborder", 0xFFFFFFFF); // "下載"按鍵解鎖
	
			printf("Num=%d\n",Num);
			EditedStepNum = Num;
			return;
		}
	}	
}

void	GetPosTag()							//紀錄 位置標籤 參數4
{
	char 	pDBID[256];
	int type=0,group=0,axis=0,pos=0;
	for(int i =0;i<MaxDBNum;i++)	// 紀錄 取出、置放 位置
	{
		memset(pDBID, 0 ,sizeof(pDBID));
		sprintf(pDBID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1);
		type = GetDBValue(pDBID).lValue;

		if(type==1) // ACTION_TYPE = 軸動作
		{
			memset(pDBID, 0 ,sizeof(pDBID));
			sprintf(pDBID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER4",u_SubGroup,i+1); // 群組
			group = GetDBValue(pDBID).lValue;
			if(group==5) // 5=取物取件
			{
				printf("Set Group 5\n");
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1); // 哪一軸
				axis = GetDBValue(pDBID).lValue;
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,i+1); // 位置
				pos = GetDBValue(pDBID).lValue;
				switch(axis)
				{
					case 1:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION1", pos);
						break;
					case 2:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION2", pos);
						break;
					case 3:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION3", pos);
						break;
					case 4:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION4", pos);
						break;
					case 5:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION5", pos);
						break;
					default:
						break;
				}
			}
			else if(group==8) // 8=置物放置
			{
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1); // 哪一軸
				axis = GetDBValue(pDBID).lValue;
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup,i+1); // 位置
				pos = GetDBValue(pDBID).lValue;
				switch(axis)
				{
					case 1:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION6", pos);
						break;
					case 2:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION7", pos);
						break;
					case 3:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION8", pos);
						break;
					case 4:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION9", pos);
						break;
					case 5:
						SetDBValue("AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION10", pos);
						break;
					default:
						break;
				}
			}
		}				
		else if(type==0)// 數完步數
		{
			return;
		}
	}
}

void	UpdateText()						//更新顯示字串
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
	// ↓ 字串合併顯示 cjlee add 	
	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
	char	str[1024]; // 顯示字串用
	char	path[1024]; // 顯示字串用
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
	
	// ↑ 字串合併顯示 cjlee add 
	for(int i=0;i<iCheckBoxAct;i++)
	{
		memset(str,0,sizeof(str));
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1+No1);
		memset(pDataID2, 0 ,sizeof(pDataID2));
		sprintf(pDataID2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1+No1);
		sprintf(szActPara1,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup, i+1+No1); 
		sprintf(szActPara2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup, i+1+No1); 
		sprintf(szActPara3,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup, i+1+No1); 
		sprintf(szActPara5,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5",u_SubGroup, i+1+No1); 
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
			case 1:		//軸動作
				if(wActPara5) // 延時
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "延時"
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "秒"
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X軸"
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y軸"
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z軸"
						break;
					case 4: // cjlee 2019/4/6 下午 05:41:56
						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2軸"
						break;
					case 5: // cjlee 2019/4/6 下午 05:41:56
						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2軸"
						break;
					default:
						break;
				}
				if(ACTIONNUM <= 5) //(ACTIONNUM <= 3 || ACTIONNUM >= 5) // cjlee 2019/4/6 下午 05:41:56
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "以"
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "的速度移動到"
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				else if(ACTIONNUM == 6)//if(ACTIONNUM == 4)  // cjlee 2019/4/6 下午 05:41:56
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
			case 2:		//等待
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
			case 4:		//閥門
				if(wActPara5) // "延時"
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_VALVE"); // "閥門"
				sprintf(pDataID5,"DLG_DATETIME_COLON"); // ":"
				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
				if(wActPara1) // "打開/關閉"
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
				if(wActPara2) // "檢測"
					sprintf(pDataID8,"VW_PUMP_TEST");
//				if(wActPara2)
//				{
//					sprintf(pDataID8,"PICKER_ACTTIME");
//					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
//					sprintf(pDataID10,"VW_INJPRO_SECOND");
//				}
				break;
			case 5:		//標籤
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
			case 6:		//跳轉
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
			case 13:	//副程式
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
			case 14: // 副程式結束
				sprintf(pDataID4,"STR_SUB_END");
				break;
			default: 
				break;	
		}
		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
		// ↓ 字串合併顯示 cjlee add 
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
			RegionUpdateStop_APP = TRUE; //停止元件獨立刷新，記錄未刷新範圍
			
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			pwndStaticAct[i]->UpdateAll();
			
			ChangePos(1);  //將未刷新的部分一次刷新
			

		}
		//pwndCheckBoxAct[i]->Update();
		// ↑ 字串合併顯示 cjlee add 
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
/*---------------------------------------------------------------------------+
|  Function : Sync()	     		   	          												  		   |
|       		: 合併動作																											 |
+---------------------------------------------------------------------------*/
void	Sync(int SelectNo)
{
	printf("Sync SelectNo=%d\n",SelectNo);
		
	char 	pDataID[256];
	int 	ACTION_STEP = 0,ACTION_STEP_Pre = 0,value = 0;;
	
	// 取得上一步ACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,SelectNo-1);
	ACTION_STEP_Pre = GetDBValue(pDataID).lValue;
	// 取得這一步ACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,SelectNo);
	ACTION_STEP = GetDBValue(pDataID).lValue;
	if (ACTION_STEP !=ACTION_STEP_Pre)
		{
			SetDBValue(pDataID, ACTION_STEP_Pre);
			printf("Set No%d Step=%d\n",ACTION_STEP,ACTION_STEP_Pre);
			for(int i=MaxDBNum;i>SelectNo;i--) // 此步以下步驟皆上移一個ACTION_STEP
			{
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i);
				value = GetDBValue(pDataID).lValue;
				SetDBValue(pDataID, value-1);
			}
			UpdateNo();
		}
}
/*---------------------------------------------------------------------------+
|  Function : UnSync()	     		             												  		   |
|       		: 分解動作																											 |
+---------------------------------------------------------------------------*/
void	UnSync(int SelectNo)
{
	printf("UnSync SelectNo=%d\n",SelectNo);
	char 	pDataID[256];
	int 	ACTION_STEP = 0,ACTION_STEP_Pre = 0,value = 0;;
	
	// 取得上一步ACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,SelectNo-1);
	ACTION_STEP_Pre = GetDBValue(pDataID).lValue;
	// 取得這一步ACTION_STEP
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,SelectNo);
	ACTION_STEP = GetDBValue(pDataID).lValue;
	if (ACTION_STEP == ACTION_STEP_Pre)
		{
			SetDBValue(pDataID, ACTION_STEP+1);
			//printf("Set No%d Step=%d\n",ACTION_STEP,ACTION_STEP+1);
			for(int i=MaxDBNum;i>SelectNo;i--) // 此步以下步驟皆下移一個ACTION_STEP
			{
				memset(pDataID, 0 ,sizeof(pDataID));
				sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,i);
				value = GetDBValue(pDataID).lValue;
				SetDBValue(pDataID, value+1);
			}
			UpdateNo();
		}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateNo()	                  												  		   |
|       		: 更新序號																											 |
+---------------------------------------------------------------------------*/
void	UpdateNo()
{
	int value=0;
	for (int i = 0; i < iEditNo; i++)	//獲參序號
	{
		No = i+1+No1;
		{
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pNoDataID, 0 ,sizeof(pNoDataID));
				sprintf(pNoDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_STEP",u_SubGroup,No);
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				value = GetDBValue(pNoDataID).lValue;
				//printf("Get %s=%d\n",pNoDataID,value);
				if(No>EditedStepNum)value=0; // 未編輯的動作
				sprintf(pStaticNo,"%d",value);				
				pwndStaticEditNo[i]->SetPropValueT("bgc",NoColor[((value)%2)]);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Show();
			}
		}
	}
}

/*---------------------------------------------------------------------------+
|  Function : Update_Download_Hint()                  										   |
|       		: 刷新提示 是否需要下載																					 |
+---------------------------------------------------------------------------*/
void	Update_Download_Hint()
{
	printf("Update_Download_Hint()\n");
	if(pwndButtonDownload!=NULL)
	{
		if(g_Hint_Download==1) // 需要下載
		{
			pwndButtonDownload->SetPropValueT("upbitmap","res_tm640/pic/PGBtn_Download_Needed.bmp");
		}
		else
		{
			pwndButtonDownload->SetPropValueT("upbitmap","res_tm640/pic/PGBtn_Download.bmp");
		}
		pwndButtonDownload->CreateA();
		pwndButtonDownload->Update();
	}
}
