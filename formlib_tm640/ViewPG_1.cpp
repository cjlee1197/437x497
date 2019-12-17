#include	"ViewPG_1.h"
#include    "../database.h"
#include    "../init.h"
#include    "../selectedit.h"
#include    "../tmconfig.h"
//#include    "../lib/hmi_control_data/tmControl_CurveExData.h"
#include    "../dialog.h"

/*===========================================================================
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸

int     iEditNo=0,iCheckBoxAct=0,iEditACTION=0,iSelectEditACTION=0,iStaticACTION=0,iStaticEditNo;
int 	iStaticAct=0;
int 	No1=0,No=0;
int		Cleanhelp =0;
int		MaxDBNum = 100;
bool	m_bEnterKey = FALSE;
char 	pNo[256]	={NULL}; 
char 	pStaticNo[256]	={NULL}; 
int		StandbyStepNum=0; // 待機點步驟數量 3軸-5步 5軸-7步

CtmWnd*		pwndEditNo[32] 	={NULL}; 		//
CtmWnd*		pwndStaticEditNo[32] 	={NULL}; 
CtmWnd*		pwndCheckBoxAct[32] ={NULL}; 	//
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
CtmWnd*		pwndButtonAct[32] 	={NULL};
int			iNumButtonAct =0;
int			SelectNo	=0;		//絞ヶ祭紬
long		ActionType  =0;		//雄釬濬倰
CtmWnd*		pwndButtonReturn	= NULL;

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	//u_PickerType = (int)(GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	// 機型選擇 編碼器選擇
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	
	if(u_PickerType==MechType3)
		StandbyStepNum = 5;
	if(u_PickerType==MechType5)
		StandbyStepNum = 7;
		
	iEditNo 	 			= GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);					//
	iStaticEditNo 	 			= GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", pwndStaticEditNo);
	iCheckBoxAct				= GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);	//
	iStaticAct				= GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", pwndStaticAct);	//雄釬蹈杅講 動作列數量
	iEditACTION				= GetSpecialControlNum(pwndSender, "EditACTION", "CtmEditX1", pwndEditACTION);
	iSelectEditACTION				= GetSpecialControlNum(pwndSender, "SelectEditACTION", "CtmSelectEdit", pwndSelectEditACTION);
	iStaticACTION				= GetSpecialControlNum(pwndSender, "StaticACTION", "CtmStaticX2", pwndStaticACTION);
	//No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40").lValue-5;	
	No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39").lValue;	
	SelectNo= GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40").lValue;	
	if(No1<0)
	{
		No1=0;
	}	
	else if(No1>95)
	{
		No1=95;
	}											// 0
	//SetDBValue("MACHINE_PROFILE_STEPNUM", 0);		//
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
	pwndButtonReturn		= pwndSender->FindControlFromName("ButtonReturn");
	char temp[128];
	char szTemp[256];
	//iNumButtonAct 	 = GetSpecialControlNum(pwndSender, "ButtonAct", "CtmToolButton", pwndButtonAct);
	printf("iNumButtonAct=%d\n");
	char 	pDataID[256];
	memset(pDataID, 0 ,sizeof(pDataID));
	sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",SelectNo);
	ActionType = GetDBValue(pDataID).lValue;
	for (int i = 0; i < iEditNo; i++)	//
	{
		No = i+1+No1;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		if(pwndEditNo[i] != NULL)
		{
			pwndEditNo[i]->SetPropValueT("text",pNo);
			pwndEditNo[i]->Update();
		}
		if(No<=StandbyStepNum)
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",50712);
				pwndStaticAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
		}
		else
		{
			memset(pStaticNo, 0 ,sizeof(pStaticNo));
			sprintf(pStaticNo,"%d",No-StandbyStepNum);
			pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
			pwndStaticEditNo[i]->Update();
		}
		if(pwndEditNo[i] != NULL)			//
		{
			if(No==SelectNo)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",36256);
			}
		}
	}
	UpdateText();
	return TRUE;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwnd = pwndSender->FindControlFromTab(wIDControl);

	return wIDControl;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("wKey=%d\n",wKey);
	if(wKey>0)
	{
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
		printf("Set wKey=%d\n",wKey);
	}
	switch(wKey)
	{
		case 1:		// 軸動作
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_AxisAct.txt");
			Exit();
		break;
		case 2:		// 等待
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Wait.txt");
			Exit();
		break;
		case 3:		// 允許
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Permit.txt");
			Exit();
		break;
		case 4:		// 閥門
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Valve.txt");
			Exit();
		break;
		case 5:		// 標籤
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Tag.txt");
			Exit();
		break;
		case 6:		// 跳轉
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Goto.txt");
			Exit();
		break;
		case 8:		// 檢測
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Detect.txt");
			Exit();
		break;
		case 12:	// 堆疊
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Pile.txt");
			Exit();
		break;
		case 13:	// 副程式
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_Sub.txt");
			Exit();
		break;
		case 0x10:	// P2P
			//SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", wKey);
			MsgBoxCall("EditWindow_P2P.txt");
			Exit();
		break;
		default:
		break;
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("MouseDown\n");
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == pwndButtonReturn)
		{
			printf("Return\n");
			Exit();
		}
		
	if(pwnd == NULL)	return wIDControl;
	return TRUE;
}

//void	UpdateText()						//更新顯示字串
//{
//	//printf("UpdateText\n");
//	char 	pDataID[256];
//	char 	pDataID2[256];
//	char 	pDataID3[256];
//	char 	pDataID4[256];
//	char 	pDataID5[256];
//	char 	pDataID6[256];
//	char 	pDataID7[256];
//	char 	pDataID8[256];
//	char 	pDataID9[256];
//	char 	pDataID10[256];
//	char 	pDataID11[256];
//	char 	pDataID12[256];
//	char    szActPara1[256];
//	char    szActPara2[256];
//	char    szActPara3[256];
//	char    szActPara5[256];
//	int	ACTIONTYPE =0,ACTIONNUM =0;
//	DWORD   wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
//	// ↓ 字串合併顯示 cjlee add 	
//	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
//	char	str[1024]; // 顯示字串用
//	char	path[1024]; // 顯示字串用
//	char* str_1;
//	char* str_2;
//	char* str_3;
//	char* str_4;
//	char* str_5;
//	char* str_6;
//	char* str_7;
//	char* str_8;
//	char* str_9;
//	char* str_10;
//	char* str_11;
//	char* str_12;
//	
//	// ↑ 字串合併顯示 cjlee add 
//	for(int i=0;i<iCheckBoxAct;i++)
//	{
//		memset(str,0,sizeof(str));
//		memset(pDataID, 0 ,sizeof(pDataID));
//		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1+No1);
//		memset(pDataID2, 0 ,sizeof(pDataID2));
//		sprintf(pDataID2,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1+No1);
//		sprintf(szActPara1,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i+1+No1); 
//		sprintf(szActPara2,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i+1+No1); 
//		sprintf(szActPara3,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3", i+1+No1); 
//		sprintf(szActPara5,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1+No1); 
//		ACTIONTYPE =GetDBValue(pDataID).lValue;
//		ACTIONNUM =GetDBValue(pDataID2).lValue;
//		wActPara1  = GetDBValue(szActPara1).lValue;
//		wActPara2  = GetDBValue(szActPara2).lValue;
//		wActPara3  = GetDBValue(szActPara3).lValue;
//		wActPara5  = GetDBValue(szActPara5).lValue;
//		//printf("LINE%d, ACTIONTYPE:%d, ACTIONNUM:%d, Para1:%d, Para2:%d, Para3:%d, Para5:%d\n",i+1+No1,ACTIONTYPE,ACTIONNUM,wActPara1,wActPara2,wActPara3,wActPara5);
//		memset(pDataID, 0 ,sizeof(pDataID));
//		memset(pDataID2, 0 ,sizeof(pDataID2));
//		memset(pDataID3, 0 ,sizeof(pDataID3));
//		memset(pDataID4, 0 ,sizeof(pDataID4));
//		memset(pDataID5, 0 ,sizeof(pDataID5));
//		memset(pDataID6, 0 ,sizeof(pDataID6));
//		memset(pDataID7, 0 ,sizeof(pDataID7));
//		memset(pDataID8, 0 ,sizeof(pDataID8));
//		memset(pDataID9, 0 ,sizeof(pDataID9));
//		memset(pDataID10,0 ,sizeof(pDataID10));
//		memset(pDataID11,0 ,sizeof(pDataID11));
//		memset(pDataID12,0 ,sizeof(pDataID12));
//
//		//printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
//		switch(ACTIONTYPE)
//		{
//			case 0:		//
//				sprintf(pDataID,"VW_PICKER_NULL");
//				break;
//			case 1:		//軸動作
//				if(wActPara5) // 延時
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "延時"
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "秒"
//				}
//				switch(ACTIONNUM)
//				{
//					case 0:
//						sprintf(pDataID4,"VW_PICKER_NULL");
//						break;
//					case 1:
//						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X軸"
//						break;
//					case 2:
//						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y軸"
//						break;
//					case 3:
//						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z軸"
//						break;
//					default:
//						break;
//				}
//				if(ACTIONNUM <= 3)
//				{
//					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "以"
//					sprintf(pDataID6,"%3d",wActPara2);
//					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "的速度移動到"
//					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
//					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
//					if(wActPara3)
//					{
//						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
//						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
//						sprintf(pDataID12,"VW_CHARGE_MM");
//					}
//				}
//				else if(ACTIONNUM == 4)
//				{
//					if(wActPara2)
//						sprintf(pDataID4,"PICKER_REMOTE_O_14");
//					else
//						sprintf(pDataID4,"PICKER_REMOTE_O_13");
//					if(wActPara3)
//						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
//					else
//						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
//				}
//				break;
//			case 2:		//等待
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_WAIT");
//				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 3:		//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_INNER_O_0%d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 4:		//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_VALVE");
//				sprintf(pDataID5,"DLG_DATETIME_COLON");
//				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
//				if(wActPara2)
//				{
//					sprintf(pDataID8,"PICKER_ACTTIME");
//					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
//					sprintf(pDataID10,"VW_INJPRO_SECOND");
//				}
//				break;
//			case 5:		//標籤
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				if(ACTIONNUM == 1)
//				{
//					sprintf(pDataID4,"ACTIONPOINT_START");
//				}
//				else
//				{
//					sprintf(pDataID4,"PICKER_LABEL_%d",ACTIONNUM);
//				}
//				break;
//			case 6:		//跳轉
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				if(ACTIONNUM == 1)
//				{
//					sprintf(pDataID4,"ACTIONPOINT_END");
//				}
//				else
//				{
//					sprintf(pDataID4,"PICKER_JUMP");
//					sprintf(pDataID5,"PICKER_LABEL_%d",ACTIONNUM);
//				}
//				break;
//			case 8:		//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"VW_PUMP_TEST");
//				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",ACTIONNUM);
//				if(wActPara1)
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 12:	//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				if(ACTIONNUM)
//					sprintf(pDataID4,"VW_PID_GROUP%d",ACTIONNUM);
//				else
//					sprintf(pDataID4,"VW_PICKER_NULL");
//				sprintf(pDataID5,"VW_PICKER_PILE");
//				if(wActPara1)
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
//				else
//					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
//				break;
//			case 13:	//
//				if(wActPara5)
//				{
//					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
//					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
//					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
//				}
//				sprintf(pDataID4,"PICKER_SUBTECH_1");
//				switch(ACTIONNUM)
//				{
//					case 0:
//						sprintf(pDataID5,"VW_PICKER_NULL");
//						break;
//					case 1:
//						sprintf(pDataID5,"PICKER_PROD_BAD");
//						break;
//					case 2:
//						sprintf(pDataID5,"PICKER_PROD_SAMPLE");
//						break;
//					case 3:
//						sprintf(pDataID5,"PICKER_PROD_TEST");
//						break;
//					case 4:
//						sprintf(pDataID5,"VW_AD_RESERVED");
//						break;
//					case 5:
//						sprintf(pDataID5,"VW_AD_RESERVED");
//						break;
//					default:
//						break;
//				}
//				if(wActPara1)
//					sprintf(pDataID6,"ACTIONPOINT_START");
//				else
//					sprintf(pDataID6,"ACTIONPOINT_END");
//				sprintf(pDataID7,"PICKER_SUBTECH_2");
//				sprintf(pDataID8,"%2d",wActPara2);
//				break;
//			default: 
//				break;	
//		}
//		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
//		// ↓ 字串合併顯示 cjlee add 
//		index_1 = g_MultiLanguage.GetStrKeyIndex(pDataID);
//		index_2 = g_MultiLanguage.GetStrKeyIndex(pDataID2);	
//		index_3 = g_MultiLanguage.GetStrKeyIndex(pDataID3);	
//		index_4 = g_MultiLanguage.GetStrKeyIndex(pDataID4);
//		index_5 = g_MultiLanguage.GetStrKeyIndex(pDataID5);	
//		index_6 = g_MultiLanguage.GetStrKeyIndex(pDataID6);	
//		index_7 = g_MultiLanguage.GetStrKeyIndex(pDataID7);
//		index_8 = g_MultiLanguage.GetStrKeyIndex(pDataID8);	
//		index_9 = g_MultiLanguage.GetStrKeyIndex(pDataID9);	
//		index_10 = g_MultiLanguage.GetStrKeyIndex(pDataID10);
//		index_11 = g_MultiLanguage.GetStrKeyIndex(pDataID11);	
//		index_12 = g_MultiLanguage.GetStrKeyIndex(pDataID12);	
//		
//		if (index_1<0)
//			index_1 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_2<0)
//			str_2 = pDataID2;
//		else
//			str_2 = g_MultiLanguage[index_2];
//		if (index_3<0)
//			index_3 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_4<0)
//			index_4 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_5<0)
//			index_5 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_6<0)
//			str_6 = pDataID6;
//		else
//			str_6 = g_MultiLanguage[index_6];
//		if (index_7<0)
//			index_7 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_8<0)
//			str_8 = pDataID8;
//		else
//			str_8 = g_MultiLanguage[index_8];
//		if (index_9<0)
//			index_9 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_10<0)
//			index_10 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		if (index_11<0)
//			str_11= pDataID11;
//		else
//			str_11 = g_MultiLanguage[index_11];
//		if (index_12<0)
//			index_12 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
//		//printf("Line%d = %d  %d  %d %d  %d  %d %d  %d  %d %d  %d  %d\n",i+1+No1,index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12);
//		
//		str_1 = g_MultiLanguage[index_1];
//		//str_2 = g_MultiLanguage[index_2];
//		str_3 = g_MultiLanguage[index_3];
//		str_4 = g_MultiLanguage[index_4];
//		str_5 = g_MultiLanguage[index_5];
//		//str_6 = g_MultiLanguage[index_6];
//		str_7 = g_MultiLanguage[index_7];
//		//str_8 = g_MultiLanguage[index_8];
//		str_9 = g_MultiLanguage[index_9];
//		str_10 = g_MultiLanguage[index_10];
//		//str_11 = g_MultiLanguage[index_11];
//		str_12 = g_MultiLanguage[index_12];
//		
//		memset(str,0,sizeof(str));
//		//printf("sl_1=%d, sl_2=%d, sl_3=%d, sl_4=%d, sl_5=%d, sl_6=%d, sl_7=%d, sl_8=%d, sl_9=%d, sl_10=%d, sl_11=%d, sl_12=%d\n",strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
//		strncat(str, str_1, strlen(str_1));
//		strncat(str, str_2, strlen(str_2));
//		strncat(str, str_3, strlen(str_3));
//		strncat(str, str_4, strlen(str_4));
//		strncat(str, str_5, strlen(str_5));
//		strncat(str, str_6, strlen(str_6));
//		strncat(str, str_7, strlen(str_7));
//		strncat(str, str_8, strlen(str_8));
//		strncat(str, str_9, strlen(str_9));
//		strncat(str, str_10, strlen(str_10));
//		strncat(str, str_11, strlen(str_11));
//		strncat(str, str_12, strlen(str_12));
//		printf("strlen=%s,d=%d\n",str,str);
//		//printf("strlen=%d\n",strlen(str));
//		//strncpy(path, str, strlen(str)+1);
//		//path[strlen(str)]= '\0';
//		//printf("path=%d\n",path);
///*
//		memset(str,0,sizeof(str));
//		printf("strlen:%d\n",strlen(str));
//		memcpy(str,str_1,strlen(str_1));
//		memcpy(str+strlen(str_1),str_2,strlen(str_2));
//		memcpy(str+strlen(str_1)+strlen(str_2),str_3,strlen(str_3));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3),str_4,strlen(str_4));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4),str_5,strlen(str_5));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5),str_6,strlen(str_6));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6),str_7,strlen(str_7));	
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7),str_8,strlen(str_8));		
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8),str_9,strlen(str_9));	
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9),str_10,strlen(str_10));	
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10),str_11,strlen(str_11));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11),str_12,strlen(str_12));
//		memcpy(str+strlen(str_1)+strlen(str_2)+strlen(str_3)+strlen(str_4)+strlen(str_5)+strlen(str_6)+strlen(str_7)+strlen(str_8)+strlen(str_9)+strlen(str_10)+strlen(str_11)+strlen(str_12),"",strlen(""));
//		printf("%d str%d strlen: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",i+1+No1,strlen(str),strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
//*/
//		
//		if(pwndCheckBoxAct[i] != NULL)
//		{
//			pwndCheckBoxAct[i]->SetPropValueT("captionID",str);
//			/*pwndCheckBoxAct[i]->SetPropValueT("captionID1", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID2", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID3", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID4", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID5", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID6", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID7", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID8", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID9", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID10", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID11", "");
//			pwndCheckBoxAct[i]->SetPropValueT("captionID12", "");*/
//			pwndCheckBoxAct[i]->CreateA();
//			pwndCheckBoxAct[i]->Update();
//			pwndCheckBoxAct[i]->UpdateAll();
//		}
//		//pwndCheckBoxAct[i]->Update();
//		// ↑ 字串合併顯示 cjlee add 
//		/*
//		pwndCheckBoxAct[i]->SetPropValueT("captionID1", pDataID);printf("pDataID1:%s",pDataID);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID2", pDataID2);printf("pDataID2:%s",pDataID2);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID3", pDataID3);printf("pDataID3:%s",pDataID3);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID4", pDataID4);printf("pDataID4:%s",pDataID4);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID5", pDataID5);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID6", pDataID6);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID7", pDataID7);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID8", pDataID8);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID9", pDataID9);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID10", pDataID10);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID11", pDataID11);
//		pwndCheckBoxAct[i]->SetPropValueT("captionID12", pDataID12);
//		pwndCheckBoxAct[i]->CreateA();
//		pwndCheckBoxAct[i]->Update();
//		pwndCheckBoxAct[i]->UpdateAll();*/		
//	}
//}
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
					case 4:
						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2軸"
						break;
					case 5:
						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2軸"
						break;
					default:
						break;
				}
				if(ACTIONNUM <= 5)
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
				else if(ACTIONNUM == 6)
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
		printf("Line%d = %s\n",i+1+No1-StandbyStepNum,str);
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
			pwndCheckBoxAct[i]->SetPropValueT("captionID","");
			//pwndCheckBoxAct[i]->CreateA();
			//pwndCheckBoxAct[i]->Show();
			//pwndCheckBoxAct[i]->Update();
			//pwndCheckBoxAct[i]->UpdateAll();
		}
		if(pwndStaticAct[i] != NULL)
		{
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			//pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			//pwndStaticAct[i]->UpdateAll();
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