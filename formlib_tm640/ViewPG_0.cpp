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
#include "omp.h"
#include    "../dialog.h" 
#include "time.h"

DWORD dw_MechType = 0; 
int u_PickerType = 0; // 機型選擇 0-三軸 1-五軸
char* pMechTypeDB = "MACHINE_CONFIGURATION_MACHINETYPE";
int	StandbyStepNum = 0;
MachineState u_wPickerOPSatus = STATE_IDLE;
MachineState u_wPickerOPSatus_Old = STATE_IDLE;

PG_0 *actionTable = new PG_0;
char *u_pszDBString[MAXSTEP*CONTROLTYPENUM] = {NULL};
WORD wSingle_Step, wSingle_StepOld;
WORD wStep  = 0,wStepOld;
char pNoDataID[1024];
static int showFlag = -1;

long	NoColor[] =
{
	LBLUE_,
	WHITE_,	// 白
};
long	BgColor[] =
{
	LBLUE_,
	WHITE_,	// 白
};

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	if(u_PickerType==MechType3)
		StandbyStepNum = 5;
	if(u_PickerType==MechType5)
		StandbyStepNum = 7;
	
	actionTable->num_of_action = GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", actionTable->pwndStaticAct);
	GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", actionTable->pwndCheckBoxAct);
	GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", actionTable->pwndStaticEditNo);
	actionTable->pwndBtnPageUp = pwndSender->FindControlFromName("buttonUPTWENTY");
	actionTable->pwndBtnPageDown = pwndSender->FindControlFromName("buttonDOWNTWENTY");
	actionTable->pwndImgBar = pwndSender->FindControlFromName("ImgBarPlace");
	actionTable->pwndImgBarMask	= pwndSender->FindControlFromName("ImgBarMask");
	actionTable->pwndBtnFollow = pwndSender->FindControlFromName("BtnFollow");
	
	actionTable->pwndBtnExecute = pwndSender->FindControlFromName("LinuxCtmToolButton2");
	actionTable->pwndBtnEdit = pwndSender->FindControlFromName("ButtonStartEdit");
	actionTable->pwndBtnClear = pwndSender->FindControlFromName("ButtonClear");
	actionTable->pwndBtnDownload = pwndSender->FindControlFromName("ButtonDownload");
	actionTable->pwndBtnInsert = pwndSender->FindControlFromName("ButtonInsert");
	actionTable->pwndBtnDelete = pwndSender->FindControlFromName("ButtonDelete");
	actionTable->pwndBtnCombine = pwndSender->FindControlFromName("ButtonSync");
	actionTable->pwndBtnSplit = pwndSender->FindControlFromName("ButtonUnSync");
	actionTable->pwndBtnSingle = pwndSender->FindControlFromName("ButtonSingle");
	
	//actionTable->headNo = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39").lValue;
	//actionTable->page = actionTable->headNo / 15;
	//cout << "actionTable->headNo  = " << actionTable->headNo  << endl;
	actionTable->headNo = 0;
	actionTable->page = 0;
	
	actionTable->iPageturn = actionTable->num_of_action;
	
	if(actionTable->b_Follow)
	{
		actionTable->pwndBtnFollow->SetPropValueT("upbitmap", "res_tm640/pic/PGBtnSelect.bmp");
		actionTable->pwndBtnFollow->SetPropValueT("captionID", "PICKER_FOLLOW");
	}
	else
	{
		actionTable->pwndBtnFollow->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
		actionTable->pwndBtnFollow->SetPropValueT("captionID","PICKER_UNFOLLOW");
	}
 	actionTable->pwndBtnFollow->CreateA();
 	actionTable->pwndBtnFollow->Update();
 	
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
 	
	
	int actionType;
	int actionNum;
	char tmp[256];
	for(int i =0 ; i < MAXDBNUM ; i++) // 計數有編寫的教導有幾步驟
	{
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i + 1);
		actionType = GetDBValue(tmp).lValue;
		
		//memset(tmp, '\0' ,sizeof(tmp));
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i + 1);
		actionNum = GetDBValue(tmp).lValue;

		if(actionType == 6 && actionNum == 1) // "結束"type=6 num=1
			actionTable->editedStepNum= i + 1;
	}
	actionTable->ShowText();
	actionTable->UpdatePageBar();

	return TRUE;
}
ListNode::ListNode()
{
	memset(reference,0,sizeof(reference));
	readFlag = false;
}
PG_0::PG_0():lastSelect(-1), b_Follow(false), finishPoint(100)
{
	for(int i = 0 ; i < 20 ; i++)
		strList.push_back(new ListNode);
}
PG_0::~PG_0()
{
	for(int i = 0 ; i < 20 ; i++)
		delete strList[i];
}
inline void PG_0::AddStr(char *str, int &k)
{
	strList[k]->readFlag = true;
	sprintf(strList[k]->reference, str);
	k++;
}
void PG_0::ShowText()
{
	cout << "ShowText"<< endl;
	char tmp[256];
	int actionType;
	int actionNum;
	
	DWORD   wActPara1=0, wActPara2=0, wActPara3=0, wActPara5=0, wActPara6 = 0, wActPara7 = 0, wActPara8 = 0;
	char    szActPara1[256], szActPara2[256], szActPara3[256], szActPara5[256], szActPara6[256], szActPara7[256], szActPara8[256];
	
	//#pragma omp parallel
	for(int i = 0, k = 0 ; i < num_of_action ; i++)
	{
		for(int j = 0 ; j < 20 ; j++)
			strList[j]->readFlag = false;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i + 1 + headNo);
		actionType =GetDBValue(tmp).lValue;
		
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i + 1 + headNo);
		actionNum =GetDBValue(tmp).lValue;
		
		sprintf(szActPara1,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i + 1 + headNo); 
		sprintf(szActPara2,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER2", i + 1 + headNo); 
		sprintf(szActPara3,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER3", i + 1 + headNo); 
		sprintf(szActPara5,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i + 1 + headNo); 
		sprintf(szActPara6,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER6", i + 1 + headNo); 
		sprintf(szActPara7,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER7", i + 1 + headNo); 
		sprintf(szActPara8,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER8", i + 1 + headNo); 
		
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		wActPara6  = GetDBValue(szActPara6).lValue;
		wActPara7  = GetDBValue(szActPara7).lValue;
		wActPara8  = GetDBValue(szActPara8).lValue;
		
		char str[256];

		switch(actionType)
		{
			case 0:	
				AddStr("VW_PICKER_NULL", k);
				break;
			case 1: // 軸動作
				if(wActPara5) // 延時
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k); // "延時"
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k); // "秒"
					
				}
				switch(actionNum)
				{
					case 0:
						AddStr("VW_PICKER_NULL", k);
						break;
					case 1:
						AddStr("VW_HAP5_ROBOT_XAXIS", k); // "X軸"
						break;
					case 2:
						AddStr("VW_HAP5_ROBOT_YAXIS", k); // "Y軸"
						break;
					case 3:
						AddStr("VW_HAP5_ROBOT_ZAXIS", k); // "Z軸"
						break;
					case 4: // cjlee 2019/4/6 下午 05:44:41
						AddStr("VW_HAP5_ROBOT_X2AXIS", k); // "X2軸"
						break;
					case 5: // cjlee 2019/4/6 下午 05:44:41
						AddStr("VW_HAP5_ROBOT_Y2AXIS", k); // "Y2軸"
						break;
					default:
						break;
				}
				
				if(actionNum <= 5)//if(ACTIONNUM <= 3 || ACTIONNUM >= 5) // cjlee 2019/4/6 下午 05:44:41
				{
					AddStr("PICKER_DESCRIBE_AXIS_1", k); // "以"
					sprintf(str,"%3d",wActPara2);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_2", k); // "的速度移動到"
					sprintf(str,"%3d.%02d",wActPara1/100,wActPara1%100);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_3", k);
					if(wActPara3)
					{
						if(wActPara8) // 1-兩段變速 0-提前完成
							AddStr("PICKER_CHG_DIST", k); // "變速"
						else
							AddStr("PICKER_DESCRIBE_AXIS_7", k); // "提前"
						
						sprintf(str,"%3d.%02d",wActPara3/100,wActPara3%100);
						AddStr(str, k);
						AddStr("PICKER_DESCRIBE_AXIS_3", k); // "mm"
					}
				}
				else if(actionNum == 6)//(ACTIONNUM == 4) // cjlee 2019/4/6 下午 05:44:41
				{
					if(wActPara2)
						AddStr("PICKER_REMOTE_O_14", k);
					else
						AddStr("PICKER_REMOTE_O_13", k);
					if(wActPara3)
						AddStr("PICKER_DESCRIBE_AXIS_5", k);
					else
						AddStr("PICKER_DESCRIBE_AXIS_6", k);
				}
				break;
			case 2:		//等待
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);	
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);	
				}
				AddStr("PICKER_WAIT", k);
				sprintf(str,"PCIKER_INNER_I_0%d",actionNum);
				AddStr(str, k);
				if(wActPara1)
					AddStr("PICKER_DESCRIBE_AXIS_5", k); // "打開"
				else
					AddStr("PICKER_DESCRIBE_AXIS_6", k); // "確認"
				break;
			case 3:		
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				sprintf(str,"PICKER_INNER_O_0%d",actionNum);
				AddStr(str, k);
				if(wActPara1)
					AddStr("PICKER_DESCRIBE_AXIS_5", k);
				else
					AddStr("PICKER_DESCRIBE_AXIS_6", k);
				break;
			case 4:		//閥門
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				AddStr("PICKER_VALVE", k);
				AddStr("DLG_DATETIME_COLON", k);
				sprintf(str,"PICKER_REMOTE_O_%02d",actionNum);
				AddStr(str, k);
				
				if(wActPara1)
					AddStr("PICKER_DESCRIBE_AXIS_5", k);
				else
					AddStr("PICKER_DESCRIBE_AXIS_6", k);
				if(wActPara2) // "檢測"
					AddStr("VW_PUMP_TEST", k);
				break;
			case 5:	//標籤	
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				if(actionNum == 1)
					AddStr("ACTIONPOINT_START", k);
				else
				{
					sprintf(str,"PICKER_LABEL_%d",actionNum);
					AddStr(str, k);
				}
				break;
			case 6: //跳轉	
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				if(actionNum == 1)
				{
					AddStr("ACTIONPOINT_END", k);
					finishPoint = i + 1 + headNo;
				}
				else
				{
					AddStr("PICKER_JUMP", k);
					sprintf(str,"PICKER_LABEL_%d",actionNum);
					AddStr(str, k);
				}
				break;
			case 8:			//檢測
				if(wActPara5) // 延時
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k); // 延時
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k); // 秒
				}
				if(wActPara2) // 檢測模式:區域
				{
					AddStr("PICKER_PER", k); // 持續
					if(wActPara1) // 開始結束
						AddStr("PICKER_STRT", k); // 開始
					else
						AddStr("PICKER_STOP", k); // 結束
				}
				else // 檢測模式:單點
					AddStr("PICKER_SGL", k); // 單點
				AddStr("VW_PUMP_TEST", k); // 檢測
				sprintf(str,"PICKER_REMOTE_I_0%d",actionNum); // 檢測點
				AddStr(str, k);

				if(wActPara1) // On/Off
					AddStr("PICKER_DESCRIBE_AXIS_5", k); // 打開

				else
					AddStr("PICKER_DESCRIBE_AXIS_6", k); // 關閉
				break;
			case 12:	
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				if(actionNum)
				{
					sprintf(str,"VW_PID_GROUP%d",actionNum);
					AddStr(str, k);
				}
				else
					AddStr("VW_PICKER_NULL", k);
				AddStr("VW_PICKER_PILE", k);
				if(wActPara1)
					AddStr("PICKER_DESCRIBE_AXIS_5", k);
				else
					AddStr("PICKER_DESCRIBE_AXIS_6", k);
				break;
			case 13:	
				if(wActPara5)
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				AddStr("PICKER_SUBTECH_1", k);
				switch(actionNum)
				{
					case 0:
						AddStr("VW_PICKER_NULL", k);
						break;
					case 1:
						AddStr("PICKER_PROD_BAD", k);
						break;
					case 2:
						AddStr("PICKER_PROD_SAMPLE", k);
						break;
					case 3:
						AddStr("PICKER_PROD_TEST", k);
						break;
					case 4:
						AddStr("PICKER_FUNC_SUBTECH", k);
						break;
					case 5:
						AddStr("PICKER_FUNC_SUBTECH2", k);
						break;
					default:
						break;
				}
				if(wActPara1)
					AddStr("ACTIONPOINT_START", k);
				else
				{
					AddStr("ACTIONPOINT_END", k);
					finishPoint = i + 1 + headNo;
				}
				AddStr("PICKER_SUBTECH_2", k);
				sprintf(str,"%2d",wActPara2);
				AddStr(str, k);
				break;
			case 0x10: // P2P
				if(wActPara5) // 延時
				{
					AddStr("PICKER_DESCRIBE_AXIS_4", k);
					sprintf(str,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10); // 
					AddStr(str, k);
					AddStr("PICKER_DESCRIBE_AXIS_8", k);
				}
				AddStr("PICKER_P2P", k);
				if(wActPara2) //速度
				{
					AddStr("PICKER_DESCRIBE_AXIS_1", k);
					sprintf(str,"%3d",wActPara2);
					AddStr(str, k);
				}
				AddStr("PICKER_DESCRIBE_AXIS_2", k);
				sprintf(str,"%3d.%02d",wActPara1/100,wActPara1%100); // "X"
				AddStr(str, k);
				sprintf(str,"%3d.%02d",wActPara7/100,wActPara1%100); // "Y"
				AddStr(str, k);
				sprintf(str,"%3d.%02d",wActPara8/100,wActPara1%100); // "Z"
				AddStr(str, k);
			default: 
				break;	
		}
		int index[13];
		char str_[1024];
		
		memset(str_,0,sizeof(str_));
		for(int j = 0 ; j < 13 ; j++)
		{
			if(!strList[j]->readFlag)
				break;
			index[j] = g_MultiLanguage.GetStrKeyIndex(strList[j]->reference);
			
			if(index[j] < 0)
				strncat(str_, strList[j]->reference, strlen(strList[j]->reference));
			else
				strncat(str_, g_MultiLanguage[index[j]], strlen(g_MultiLanguage[index[j]]));
		}
		
		if(pwndStaticAct[i] != NULL)
		{	
			char tmp[256];
			int temp=0;
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP",headNo + i +1);
			temp = GetDBValue(tmp).lValue;		
												
			pwndStaticAct[i]->SetPropValueT("text",str_);
			
			if((headNo + i ) < StandbyStepNum)
				pwndStaticAct[i]->SetPropValueT("bgc",GRAY_);
			else
				pwndStaticAct[i]->SetPropValueT("bgc",BgColor[((temp)%2)]);
//			else
//			{
//				if((i % 2) && (headNo + i + 1) <= finishPoint)
//					pwndStaticAct[i]->SetPropValueT("bgc", LBLUE_);
//				else
//					pwndStaticAct[i]->SetPropValueT("bgc", WHITE_);
//			}		
			pwndStaticAct[i]->Show();
		}
		if(pwndStaticEditNo[i] != NULL)
		{
			char tmp[256];
			int value;
			int preValue;
			int nextValue;
			sprintf(tmp, "MACHINE_PROFILE_NUM%d_ACTION_STEP", headNo + i +1);
			value = GetDBValue(tmp).lValue;

			sprintf(tmp, "MACHINE_PROFILE_NUM%d_ACTION_STEP", headNo + i );
			preValue = GetDBValue(tmp).lValue;

			sprintf(tmp, "MACHINE_PROFILE_NUM%d_ACTION_STEP",  headNo + i + 2 );
			nextValue = GetDBValue(tmp).lValue;
			if((headNo + i ) < StandbyStepNum)
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
			else
			{
				sprintf(tmp, "%d", value - StandbyStepNum);		
				if(headNo + i +1 > actionTable->editedStepNum)
					pwndStaticEditNo[i]->SetPropValueT("text","0");
				else
					pwndStaticEditNo[i]->SetPropValueT("text",tmp);
			}

//			if(( nextValue == value || preValue == value) && (headNo + i ) > StandbyStepNum && (headNo + i + 1 ) <= finishPoint)
//				pwndStaticEditNo[i]->SetPropValueT("bgc", 0xFFE0);
//			else if((i % 2) && (headNo + i + 1) <= finishPoint)
//				pwndStaticEditNo[i]->SetPropValueT("bgc",BLUE_);
//			else
//				pwndStaticEditNo[i]->SetPropValueT("bgc",WHITE_);
			
			pwndStaticEditNo[i]->SetPropValueT("bgc",NoColor[((value)%2)]); // 動作背景顏色
			pwndStaticEditNo[i]->Show();
		}
		
		k = 0;
	}

	ChangePos(1);
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	u_wPickerOPSatus = (MachineState)GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue; // 機械手狀態

	if( (u_wPickerOPSatus_Old != u_wPickerOPSatus) && (u_wPickerOPSatus != STATE_SINGLESTEP) ) // 跳出單步 狀態更新
  		::PutCommand("PG_0.txt");

	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED42").lValue)
	{
		printf("Get SYSX_OTHERS_OTHERS_INT_RESERVED42\n");
		actionTable->ShowText();
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED42", 0);
	}
	if((u_wPickerOPSatus == STATE_SINGLESTEP || u_wPickerOPSatus == STATE_FULLAUTO) &&  actionTable->b_Follow) // 顯示當前執行步驟 && 跟隨
    {	
		wStep = (WORD)GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue; // 執行步驟
        int StepNumMin = 1 + actionTable->headNo;
        int StepNumMax = actionTable->num_of_action + actionTable->headNo;
		if(wStep >0 && wStep < StepNumMin)
        	actionTable->PageUp();
		else if(wStep <= 100 && wStep > StepNumMax)
        	actionTable->PageDown();
		else if(wStep >0 && wStep != wStepOld) // 執行步驟變化
        {
			int temp;
        	actionTable->ShowText(); 
			for(int i =0; i < actionTable->num_of_action; i++)
			{
				int StepNum = i + 1 + actionTable->headNo;
				if(wStep == StepNum && actionTable->pwndCheckBoxAct[i] != NULL && actionTable->pwndStaticAct[i] != NULL)
				{
					for(int k = 0; k < actionTable->num_of_action ; k++)
					{
						if(k + 1 + actionTable->headNo <(StandbyStepNum + 1))
							 actionTable->pwndStaticAct[k]->SetPropValueT("bgc",50712);
						else
						{
							sprintf(pNoDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",k + 1 + actionTable->headNo);
							temp = GetDBValue(pNoDataID).lValue;		
							if(k + 1 + actionTable->headNo > actionTable->editedStepNum) temp = StandbyStepNum; // 未編輯的動作	
								{
									actionTable->pwndStaticAct[k]->SetPropValueT("bgc",BgColor[((temp)%2)]); // 動作背景顏色
								}
//								if(temp % 2)
//									actionTable->pwndStaticAct[k]->SetPropValueT("bgc",WHITE_);
//								else
//									actionTable->pwndStaticAct[k]->SetPropValueT("bgc",LBLUE_);
						}								
					}
					((CtmFormView*)pwndSender)->Goto(actionTable->pwndStaticAct[i]->GetTabOrder());
					actionTable->pwndStaticAct[i]->SetPropValueT("bgc",GREEN_);
					actionTable->pwndStaticAct[i]->CreateA();
					actionTable->pwndStaticAct[i]->Show();
					actionTable->pwndStaticAct[i]->Update();
					actionTable->pwndStaticEditNo[i]->SetPropValueT("bgc",GREEN_);
					actionTable->pwndStaticEditNo[i]->CreateA();
					actionTable->pwndStaticEditNo[i]->Show();
					actionTable->pwndStaticEditNo[i]->Update();
				}
			}
			wStepOld = wStep;
		}
	}

	u_wPickerOPSatus_Old = u_wPickerOPSatus;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{	return wIDControl;	}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	int nCMDValue = 0xFF0D;
	int nCMDSingleCycle = 0xFF25;
	int actionType;
	int actionTypePre;
	int actionNum;
	char pDataID[256];
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", actionTable->selectNo );
	actionType =GetDBValue(pDataID).lValue;
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", actionTable->selectNo); 
	actionNum = GetDBValue(pDataID).lValue;
	if(u_wPickerOPSatus != STATE_FULLAUTO)
	{
		switch(wKey)
		{
			case 0x0001:
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
					if(actionTable->pwndBtnSingle != NULL)
					{
						actionTable->pwndBtnSingle->SetPropValueT("upbitmap", "res_tm640/pic/ButtonDown.bmp");
						actionTable->pwndBtnSingle->Update();
					}
				}
				else
					MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
				return _NULL_KEY;
				break;
			case 0x0002:
				if(actionTable->selectNo > StandbyStepNum && actionTable->selectNo <= actionTable->editedStepNum) // (選擇步驟 > 等待點步驟) && (選擇步驟 <= 編輯步驟數)
					if(actionType != 5)
						actionTable->Insert();
					else if(actionNum != 1)
						actionTable->Insert();
				break;
			case 0x0003:
				MsgBox(g_MultiLanguage["PICKER_CONFIRMDELETE"], tmFT_CODE_TECH);
				if(g_bMsgBoxFlag)
					if(actionTable->selectNo > StandbyStepNum && actionTable->selectNo <= actionTable->editedStepNum)
						if(actionType != 5 && actionType != 6)
						{
							actionTable->UnSync(actionTable->selectNo);
							actionTable->UnSync(actionTable->selectNo + 1);
							actionTable->Delete(actionTable->selectNo);
						}
						else if(actionNum != 1)
						{
							actionTable->UnSync(actionTable->selectNo);
							actionTable->UnSync(actionTable->selectNo + 1);
							actionTable->Delete(actionTable->selectNo);
						}
				break;
			case 0x0004:
				if(u_wPickerOPSatus != STATE_FULLAUTO) // 非自動模式下 可下載
				{
					MsgBox(g_MultiLanguage["CHECK_DOWNLOAD_CONFIRM"], tmFT_CODE_TECH);
					if(g_bMsgBoxFlag)
					{
						actionTable->Clean();
						Prompt(g_MultiLanguage["PICKER_DATADOWNLODING"],1); 
						actionTable->GetPosTag();
						actionTable->SendCommand(0xF600); // Servo Off
						actionTable->Download();
						g_Hint_Download = 0; // 完成下載
						actionTable->Update_Download_Hint();
						actionTable->ShowText();
						Prompt(g_MultiLanguage["PICKER_DATADOWNLOADFINISH"],1); 
					}
				}
				else
					MsgBox(g_MultiLanguage["INDEX_AUTO_DENIED"], tmFT_CODE_TECH);
				break;
			case 0x0008:
				MsgBox(g_MultiLanguage["PICKER_CONFIRMDELETE"], tmFT_CODE_TECH);
				if(g_bMsgBoxFlag)
					actionTable->Clear();
				break;
			case 0x000C: // 合併
				if(actionTable->selectNo > StandbyStepNum) // (SelectNo > 5)
				{
					sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", actionTable->selectNo - 1);
					actionTypePre = GetDBValue(pDataID).lValue;
					if( ((actionType == Action_Axis) || (actionType == Action_Permit)) && ((actionTypePre == Action_Axis) || (actionTypePre == Action_Permit))) // 所選動作及合併動作，皆須是軸動作或是允許動作
						actionTable->Sync();
				}
				break;
			case 0x000D: // 分解
				if(actionTable->selectNo > StandbyStepNum) //(SelectNo > 6)
				{
					if(actionType != StandbyStepNum)
						actionTable->UnSync(actionTable->selectNo);
					else if(actionNum != 1)
						actionTable->UnSync(actionTable->selectNo);
				}
				break;
			case 0x0010:
				actionTable->SendCommand(MODE_MANUAL);
				::PutCommand("PG_0.txt");
				break;
			case 0x000E:
				if(u_wPickerOPSatus == STATE_HAND)
				{
					actionTable->SendCommand(MODE_SINGLESTEP);	
					::PutCommand("PG_0_SINGLE.txt");
				}
				break;
			case 0x000F:
				if(u_wPickerOPSatus == STATE_SINGLESTEP)
				{
					if(g_ptaskpicker != NULL)
					{
						g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDSingleCycle, NULL);
						printf("nCMDSingleCycle:%X\n",nCMDSingleCycle);
					}   
				}
				else
					MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
				return _NULL_KEY;
				break;
		}
	}
	
	if (wKey == 0x000B)		// 下一頁
	{
		actionTable->PageDown();
		
		return _NULL_KEY;
	}
	else if (wKey == 0x000A)	// 上一頁
	{
		actionTable->PageUp();
	
		return _NULL_KEY;
	}
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	char tmp[256];
	if( (u_wPickerOPSatus != STATE_FULLAUTO) || (!actionTable->b_Follow) ) // 自動下限制 || 不跟隨
	{
		for(int i =0; i < actionTable->num_of_action ; i++)
		{
			if(actionTable->pwndCheckBoxAct[i] != NULL && pwnd == actionTable->pwndCheckBoxAct[i]) // press selected btn
			{
				if(actionTable->lastSelect >= 0 && actionTable->lastSelect != i )
				{
					actionTable->pwndStaticAct[actionTable->lastSelect]->SetPropValueT("bgc",actionTable->lastSelectColor_1);
					actionTable->pwndStaticEditNo[actionTable->lastSelect]->SetPropValueT("bgc",actionTable->lastSelectColor_2);
					actionTable->pwndStaticEditNo[actionTable->lastSelect]->Show();
					actionTable->pwndStaticAct[actionTable->lastSelect]->Show();
				
				}
				if(actionTable->lastSelect != i)
				{
					actionTable->pwndStaticAct[i]->GetPropValueT("bgc", &actionTable->lastSelectColor_1, sizeof(DWORD));
					actionTable->pwndStaticEditNo[i]->GetPropValueT("bgc", &actionTable->lastSelectColor_2, sizeof(DWORD));
				}
				
				actionTable->pwndStaticAct[i]->SetPropValueT("bgc",GREEN_);
				actionTable->pwndStaticEditNo[i]->SetPropValueT("bgc",GREEN_);
				
				actionTable->pwndStaticEditNo[i]->Show();
				actionTable->pwndStaticAct[i]->Show();
				actionTable->lastSelect = i;
				actionTable->selectNo = actionTable->headNo + i + 1;
				//actionTable->pwndStaticAct[i]->GetPropValueT("text", tmp, sizeof(tmp));
				//cout << "actionTable->selectNo = " << actionTable->selectNo << endl;
				//cout << "tmp = " << tmp << endl;
				ChangePos(1);
				showFlag = i;
				break;
			}
		}
	}	
	if(pwnd == actionTable->pwndBtnEdit) // edit
 	{
		if(actionTable->selectNo < actionTable->editedStepNum)
		{
			int actionType;
			int actionNum;
			char tmp[256];
			
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE", actionTable->selectNo);
			actionType =GetDBValue(tmp).lValue;
			//memset(tmp, '\0' ,sizeof(tmp));
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM", actionTable->selectNo); 
			actionNum =GetDBValue(tmp).lValue;
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", actionTable->selectNo);
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
			if(actionType != 0)
			{
					switch(actionType)
					{
						case 1:		// 軸動作
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_AxisAct.txt");
							usleep(100*1000);
							break;
						case 2:		// 等待
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_Wait.txt");
							usleep(100*1000);
							break;
						case 3:		// 允許
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_Permit.txt");
							usleep(100*1000);
							break;
						case 4:		// 閥門
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_Valve.txt");
							usleep(100*1000);
							break;
						case 5:		// 標籤
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							if(actionNum != 1)
							{
								MsgBoxCall("EditWindow_Tag.txt");
								usleep(100*1000);
							}
							break;
						case 6:		// 趟轉
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							if(actionNum != 1)
							{
								MsgBoxCall("EditWindow_Goto.txt");
								usleep(100*1000);
							}
							break;
						case 8:		// 檢測
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_Detect.txt");
							usleep(100*1000);
							break;
						case 12:	// 堆疊
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_Pile.txt");
							usleep(100*1000);
							break;
						case 13:	// 副程式
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_Sub.txt");
							usleep(100*1000);
							break;
						case 0x10:	// P2P
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", actionType);
							MsgBoxCall("EditWindow_P2P.txt");
							usleep(100*1000);
							break;
						default:
							MsgBoxCall("EditWindow_ActSelect.txt");
							usleep(100*1000);
							break;
					}
			}
			else
			{
				MsgBoxCall("EditWindow_ActSelect.txt");
				usleep(100*1000);
			}
			actionTable->ShowText();
		}
	}
}
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{	
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	char tmp[256];
	if(showFlag >= 0 )
		actionTable->pwndStaticAct[showFlag]->Show();
	if(pwnd == actionTable->pwndBtnFollow)
 	{
 		if(actionTable->b_Follow) // 跟隨
		{
			actionTable->pwndBtnFollow->SetPropValueT("upbitmap","res_tm640/pic/PGBtnSelect.bmp");
			actionTable->pwndBtnFollow->SetPropValueT("captionID","PICKER_FOLLOW");
		}
		else // 不跟隨
		{
			actionTable->pwndBtnFollow->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
			actionTable->pwndBtnFollow->SetPropValueT("captionID","PICKER_UNFOLLOW");
		}
		
		actionTable->b_Follow = !actionTable->b_Follow;
 		actionTable->pwndBtnFollow->CreateA();
 		actionTable->pwndBtnFollow->Update();
 	}

	return wIDControl;	
}

void    OnDestroyA(CtmWnd* pwndSender)
{
	for(int i = 0; i < MAXSTEP * CONTROLTYPENUM ; i++)
    {
        if(u_pszDBString[i] != NULL)
            free(u_pszDBString[i]);
        u_pszDBString[i] = NULL;
    }
}
void PG_0::PageDown()		// 下一頁
{
	cout << "Page Down" << endl;
	if(headNo + iPageturn > 100)
		headNo = 90;
	else
	{
		headNo += iPageturn;
		page += 1;
	}


	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", headNo);
	UpdatePageBar();
	
	ShowText();
	lastSelect = -1;
}
void PG_0::PageUp()	
{
	cout << "Page Up" << endl;
	if(headNo - iPageturn  < 0)
		headNo = 0;
	else
	{
		headNo -= iPageturn;
		page -= 1;
	}

	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", headNo);
	UpdatePageBar();
	ShowText();
	lastSelect = -1;
}

void PG_0::UpdatePageBar()
{
	pwndImgBarMask->CreateA();
	pwndImgBarMask->Show();
	pwndImgBar->SetPropValueT("top",(Bar_Position + page * Bar_Step));
	pwndImgBar->SetPropValueT("bottom",(Bar_Position + (page * Bar_Step) + Bar_Heigh));
	pwndImgBar->CreateA();
	pwndImgBar->Show();
}

void PG_0::Insert()	
{
	char tmp[256];
	int	value;
	for(int i = editedStepNum + 1; i > selectNo ; i--)
	{
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i - 1);
		value = GetDBValue(tmp).lValue;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i);
		SetDBValue(tmp, value + 1);
		
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i - 1);
		value = GetDBValue(tmp).lValue;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i);
		SetDBValue(tmp, value);
		
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i - 1);
		value = GetDBValue(tmp).lValue;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i);
		SetDBValue(tmp, value);
		
		for(int j = 0 ; j < PARA_NUM ; j++)
		{
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i - 1, j + 1);
			value = GetDBValue(tmp).lValue;
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d",i, j + 1);
			SetDBValue(tmp, value);
		}
	}
	sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP", selectNo - 1);
	value = GetDBValue(tmp).lValue;
	sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP", selectNo);
	SetDBValue(tmp, value+1);
	
	sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE", selectNo);
	SetDBValue(tmp, 0);
	
	sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM", selectNo);
	SetDBValue(tmp, 0);
	
	for(int j = 0;j < PARA_NUM ; j++)
	{
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d", selectNo, j + 1);
		SetDBValue(tmp, 0);
	}
	
	editedStepNum++;
	ShowText();
}

void PG_0::Delete(int selectNo)				
{
	char tmp[256];
	int value;

	for(int i = selectNo ; i < editedStepNum + 1 ; i++) 
	{
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP", i + 1);
		value = GetDBValue(tmp).lValue;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_STEP", i);
		SetDBValue(tmp, value - 1);
		
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE", i + 1);
		value = GetDBValue(tmp).lValue;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE", i);
		SetDBValue(tmp, value);
		
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM", i + 1);
		value = GetDBValue(tmp).lValue;
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM", i);
		SetDBValue(tmp, value);
		
		for(int j = 0 ; j < PARA_NUM ; j++)
		{
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d", i + 1, j + 1);
			value = GetDBValue(tmp).lValue;
			sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d", i, j + 1);
			SetDBValue(tmp, value);
		}
	}
	
	sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_TYPE", MAXDBNUM);
	SetDBValue(tmp, 0);
	
	sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_NUM", MAXDBNUM);
	SetDBValue(tmp, 0);
	
	for(int j = 0;  j < PARA_NUM ; j++)
	{
		sprintf(tmp,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d", MAXDBNUM, j + 1);
		SetDBValue(tmp, 0);
	}
	
	editedStepNum--;
	lastSelect = -1;
	ShowText();
}

void PG_0::Clear()								//ь清除資料
{
	char 	pDataID[256];
	for(int i = StandbyStepNum ; i <= editedStepNum ; i++) // 從等待點步驟 之後開始清空
	{
		sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", i + 1);
		SetDBValue(pDataID, 0);
		
		sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i + 1);
		SetDBValue(pDataID, 0);
		
		sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", i + 1);
		SetDBValue(pDataID, 0);
		
		for(int j = 0 ; j < PARA_NUM ; j++)
		{
			sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER%d", i + 1, j + 1);
			SetDBValue(pDataID, 0);
		}
	}
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", StandbyStepNum + 1);
	SetDBValue(pDataID, 5); // 開始
	
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", StandbyStepNum + 1);
	SetDBValue(pDataID, 1);

	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", StandbyStepNum + 2);
	SetDBValue(pDataID, 6); // 結束

	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", StandbyStepNum + 2);
	SetDBValue(pDataID, 1);
	
	for(int i = 0 ; i < (StandbyStepNum + 2) ; i++) 
	{
		sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", i + 1);
		SetDBValue(pDataID, i + 1);
	}
	editedStepNum = (StandbyStepNum + 2);
	ShowText();
}

void PG_0::Clean()								//ь情除"空"動作
{
	Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"],1); 
	char pDataID[256];
	int value ;
	int cleanFlag = 0;
	
	cleanhelp = 0;
	for(int i = 0 ; i < MAXDBNUM ; i++)
	{
		sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i + 1);
		value = GetDBValue(pDataID).lValue;

		if(value == 0 && cleanFlag != 1)
		{
			cleanhelp = i;
			cleanFlag = 1;
		}
		if(i > cleanhelp && value != 0 && cleanFlag == 1)
		{
			UnSync(cleanhelp + 1);
			Delete(cleanhelp + 1);
			Clean();
		}
	}
	Prompt(g_MultiLanguage["MSG_DUMMY"]);

}

void PG_0::Download()							//下載資料至主機
{					
	char 	pDataID[256];
	int value ;
	int value1, value2;
	int Check_MoldOpen = 0;
	int Num = 0;

	printf("MaxDBNum=%d\n",MAXDBNUM);
	pwndBtnDownload->SetPropValueT("taborder", -2);

	for(int i =0 ; i < MAXDBNUM ; i++)
	{
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		value = GetDBValue(pDataID).lValue;
		if(value != 0) // ACTION_TYPE 有值
			Num++; // 計數 教導有幾步驟

		else // 數完步數
		{
			for(int i = 0 ; i < Num ; i++) // 檢查是否有『等待開模完成』
			{
				sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_TYPE", i + 1);
				value1 = GetDBValue(pDataID).lValue;
				sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", i + 1);
				value2 = GetDBValue(pDataID).lValue;
				if(value1 == 2 && value2 == 2) // 等待 // 開模完成
					Check_MoldOpen = TRUE;
			}
			if(!Check_MoldOpen) // 未編寫檢測 『等待開模完成』
			{
				MsgBox(g_MultiLanguage["CHECK_MOLDOPEN_CONFIRM"], tmFT_CODE_TECH);//未檢測『等待開模完成』，確認下載?
				if(g_bMsgBoxFlag)
				{
					if(g_ptaskpicker != NULL) // 傳送教導至28
					{
						SetDBValue("MACHINE_PROFILE_STEPNUM", 0, TRUE);
						printf("Send cmd\n");
						for(int step=0; step<Num * CONTROLTYPENUM; step++)
						{
							printf("u_pszDBString[%d]=%d\n",step,u_pszDBString[step]);
						}
						g_ptaskpicker->WriteValue(CONST_REQ_WRITE, Num * CONTROLTYPENUM ,u_pszDBString);
						SetDBValue("MACHINE_PROFILE_STEPNUM", Num, TRUE);
					}
					MsgBoxCall("msgboxConfirm.txt","PICKER_DATADOWNLOADFINISH"); // 下載完成
				}
			}
			else
			{
				if(g_ptaskpicker != NULL) // 傳送教導至28
				{
					SetDBValue("MACHINE_PROFILE_STEPNUM", 0, TRUE);
					printf("Send cmd\n");
				
					g_ptaskpicker->WriteValue(CONST_REQ_WRITE, Num * CONTROLTYPENUM ,u_pszDBString);
					SetDBValue("MACHINE_PROFILE_STEPNUM", Num, TRUE);
					MsgBoxCall("msgboxConfirm.txt","PICKER_DATADOWNLOADFINISH"); // 下載完成
				}
			}
			pwndBtnDownload->SetPropValueT("taborder", 0xFFFFFFFF); // "下載"按鍵解鎖
		
			printf("Num=%d\n",Num);
			editedStepNum = Num;
				
			return;
		}
	}
}

void PG_0::GetPosTag()							//紀錄 位置標籤 參數4 提供首頁位置補正用
{
	char 	pDBID[256];
	int type, group, axis, pos;

	for(int i = 0 ; i < 10 ; i++)
	{
		sprintf(pDBID, "AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION%d", i + 1);
		SetDBValue(pDBID, 0);
	}

	for(int i = 0 ; i< MAXDBNUM ; i++)	// 紀錄 取出、置放 位置
	{
		memset(pDBID, 0 ,sizeof(pDBID));
		sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		type = GetDBValue(pDBID).lValue;

		if(type == 1) // ACTION_TYPE = 軸動作
		{
			sprintf(pDBID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4", i + 1); // 群組
			group = GetDBValue(pDBID).lValue;

			if(group == 6) // 6=取物取件
			{
				sprintf(pDBID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", i + 1); // 哪一軸
				axis = GetDBValue(pDBID).lValue; 
				sprintf(pDBID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i + 1); // 位置
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
			else if(group == 9) // 9=置物放置
			{
				sprintf(pDBID, "MACHINE_PROFILE_NUM%d_ACTION_NUM", i + 1); // 哪一軸
				axis = GetDBValue(pDBID).lValue;
				sprintf(pDBID, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1", i+ 1); // 位置
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
		else if(type == 0)// 數完步數
		{
			return;
		}
	}
}

void PG_0::Sync() // 合併動作
{
	char 	pDataID[256];
	int 	actionStep, actionStepPre, value;

	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", selectNo - 1);
	actionStepPre = GetDBValue(pDataID).lValue;
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", selectNo);
	actionStep = GetDBValue(pDataID).lValue;
	cout << "Sync" << endl;
	cout << actionStep << endl;
	cout << actionStepPre << endl;
	if (actionStep != actionStepPre)
	{
		SetDBValue(pDataID, actionStepPre);
		for(int i = MAXDBNUM ; i > selectNo ; i--) // 此步以下步驟皆上移一個ACTION_STEP
		{
			sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", i);
			value = GetDBValue(pDataID).lValue;
			SetDBValue(pDataID, value - 1);
		}
		ShowText();
	}
}

void PG_0::UnSync(int selectNo) // 分解動作
{
	char 	pDataID[256];
	int 	actionStep, actionStepPre, value;
	
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", selectNo - 1);
	actionStepPre = GetDBValue(pDataID).lValue;
	sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", selectNo);
	actionStep = GetDBValue(pDataID).lValue;
	
	if (actionStep == actionStepPre)
	{
		SetDBValue(pDataID, actionStep + 1);
		for(int i = MAXDBNUM ; i > selectNo ; i--) // 此步以下步驟皆下移一個ACTION_STEP
		{
			sprintf(pDataID, "MACHINE_PROFILE_NUM%d_ACTION_STEP", i);
			value = GetDBValue(pDataID).lValue;
			SetDBValue(pDataID, value + 1);
		}
		ShowText();
	}
}

/*---------------------------------------------------------------------------+
|  Function : Update_Download_Hint()                  										   |
|       		: 刷新提示 是否需要下載																					 |
+---------------------------------------------------------------------------*/
void PG_0::Update_Download_Hint()
{
	printf("Update_Download_Hint()\n");
	if(pwndBtnDownload != NULL)
	{
		if(g_Hint_Download == 1) // 需要下載
			pwndBtnDownload->SetPropValueT("upbitmap", "res_tm640/pic/PGBtn_Download_Needed.bmp");
		else
		{
			pwndBtnDownload->SetPropValueT("upbitmap", "res_tm640/pic/PGBtn_Download.bmp");
		}
		pwndBtnDownload->CreateA();
		pwndBtnDownload->Update();
	}
}

void PG_0::SendCommand(int	CommandID)
{
	if(g_ptaskpicker != NULL)
 	{			
		g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
	}
}
