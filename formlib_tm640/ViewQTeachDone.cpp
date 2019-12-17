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
#include	"ViewQTeachDone.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word

#define     CONST_REQ_COMMAND       6
#define     CONST_REQ_WRITE         3
#define     CONTROLTYPENUM          13
#define     STATE_SINGLESTEP        2
#define			STATE_MANUAL						3
#define     STATE_FULLAUTO          4 
#define     MAXSTEP               	100

#define		MODE_NULL						0xF000
#define		MODE_HOMING					0xF100
#define		MODE_MOTORENABLE		0xF200
#define		MODE_MANUAL					0xF300
#define		MODE_SINGLESTEP			0xF400
#define		MODE_AUTO						0xF500
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

int				MaxDBNum = 100;
char*     u_pszDBString[MAXSTEP*CONTROLTYPENUM] = {NULL};

CtmWnd*		pwndImg_QTeach_Done	= NULL; // 顯示圖片

CtmWnd*		pwndButtonDownload	= NULL; // 下載按鈕
CtmWnd*		pwndLoadingStr	= NULL; // 下載進度文字
CtmWnd*		pwndLoadingBar	= NULL; // 下載進度條
CtmWnd*		pwndLoadingMask	= NULL; // 下載進度條遮罩
CtmWnd*		pwndLoadingMask2	= NULL; // 下載進度條遮罩2

CtmWnd*		pwndBtnLastP	= NULL; // 返回上一頁 返回修正

CtmWnd*		pwndBtn_SingleStep	= NULL; // 單步 Btn
CtmWnd*		pwndBtn_SingleCycle	= NULL; // 單循環 Btn
CtmWnd*		pwndBtn_SingleNextStep	= NULL; // 下一步 Btn
CtmWnd*		pwndMask_SingleStep	= NULL; // 單步 遮罩

int Count=0; // 計數
int Point=0; // 點位置
char* QTeach_Img[] = // 各點圖片
{
	"",
	"res_tm640/pic/picker/QTeach_WaitP.bmp", 				 // 等待
	"res_tm640/pic/picker/QTeach_ReadyDownP.bmp",    // 準備下降
	"res_tm640/pic/picker/QTeach_PickDownP.bmp",     // 下降
	"res_tm640/pic/picker/QTeach_PickP.bmp",         // 取物
	"res_tm640/pic/picker/QTeach_PickDownP.bmp",     // 後退
	"res_tm640/pic/picker/QTeach_ReadyDownP.bmp",    // 上升
	"res_tm640/pic/picker/QTeach_MoveOutP.bmp",      // 橫出
	"res_tm640/pic/picker/QTeach_PlaceDownP.bmp",    // 下降
	"res_tm640/pic/picker/QTeach_PlaceP.bmp",        // 放置
	"res_tm640/pic/picker/QTeach_PlaceDownP.bmp",    // 上升
};

WORD        u_wPickerOPSatus    = 0;
WORD        wSingle_Step  = 0,wSingle_StepOld;
BOOL b_Single_Step_Process =0; // 1-正在執行 0-執行完成
WORD        wSingle_Step_Process=0;
char* ProcessImgpath[] = // 各點圖片
{
	"res_tm640/pic/PGBtn_NextStep.bmp",
	"res_tm640/pic/PGBtn_NextStep_Process.bmp"
};
/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	// 取得指標 顯示圖片
	pwndImg_QTeach_Done = pwndSender->FindControlFromName("Img_QTeach_Done");
	
	// 取得指標 下載 及 下載顯示進度條
	pwndButtonDownload = pwndSender->FindControlFromName("BtnDownload");
	pwndLoadingStr  = pwndSender->FindControlFromName("LoadingStr");
	pwndLoadingBar  = pwndSender->FindControlFromName("LoadingBar");
	pwndLoadingMask = pwndSender->FindControlFromName("LoadingMask");
	pwndLoadingMask2 = pwndSender->FindControlFromName("LoadingMask2");
	// 取得指標 返回上一頁
	pwndBtnLastP = pwndSender->FindControlFromName("BtnLastP");
	
	// 取得指標 單步 Btn
	pwndBtn_SingleStep = pwndSender->FindControlFromName("Btn_SingleStep");
	// 取得指標 單循環 Btn
	pwndBtn_SingleCycle = pwndSender->FindControlFromName("Btn_SingleCycle");
	// 取得指標 下一步 Btn
	pwndBtn_SingleNextStep = pwndSender->FindControlFromName("Btn_SingleNextStep");
	// 取得指標 單步 遮罩
	pwndMask_SingleStep = pwndSender->FindControlFromName("Mask_SingleStep");
	
	
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
	
	// 取得 機構設定 3 or 5軸
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue); // 讀取設定 機型選擇 三五軸
	u_PickerType = dw_MechType & MechWord;
		
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
	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue; // 機械手狀態
	if(u_wPickerOPSatus==STATE_MANUAL) //手動模式
	{		
		Count++;
		if(Count>10) // 定時換照片
		{
			Count=0;
			if(pwndImg_QTeach_Done!=NULL)
				{
					Point++;
					if(Point>10)
						Point=0;
					pwndImg_QTeach_Done->SetPropValueT("imagepath",QTeach_Img[Point]);
					pwndImg_QTeach_Done->Show();	
				}
		}
	}
	else 	if(u_wPickerOPSatus==STATE_SINGLESTEP) //單步模式
	{
		wSingle_Step = (WORD)GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
		//printf("wSingle_Step=%d\n",wSingle_Step);
		if(wSingle_Step!=wSingle_Step_Process)
			{
				wSingle_Step_Process=wSingle_Step;
				b_Single_Step_Process=0; //執行完成
				printf("Process Down!!!!!\n");
				UpdateNextStep();
			}
	}
}

void    OnDestroyA(CtmWnd* pwndSender)
{
    for(int i = 0; i < MAXSTEP*CONTROLTYPENUM; i++)
    {
        if(u_pszDBString[i] != NULL)
            free(u_pszDBString[i]);
        u_pszDBString[i] = NULL;
    }
    
   if(u_wPickerOPSatus==STATE_SINGLESTEP) 
   	SendCommand(MODE_MANUAL); // 切回手動模式
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{

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
 	
	int         nCMDValue       = 0xFF0D;
	int         nCMDLastStep    = 0xFF26;
	int         nCMDSingleCycle = 0xFF25;
 	
 	if(pwnd == pwndButtonDownload) // 下載 Btn按下彈起
	{	
		printf("Pess Download\n");
		//if(u_wPickerOPSatus == STATE_MANUAL)
		if(1)
		{
			Prompt(g_MultiLanguage["PICKER_DATADOWNLODING"],1); 
			GetPosTag();
			SendCommand(0xF600); // Servo Off
			Download();
			Prompt(g_MultiLanguage["PICKER_DATADOWNLOADFINISH"],1); 
			MsgBox(g_MultiLanguage["TEACH_QTEACH_STEP_TEST"], tmFT_CODE_TECH); // 提示單步測試
	    if(g_bMsgBoxFlag)
	    {
	    	SendCommand(MODE_SINGLESTEP);
	    	ShowSingleStep();
	    }
	    else
	    {
				MsgBox(g_MultiLanguage["TEACH_QTEACH_DOWNLOAD_DONE"], tmFT_CODE_TECH); // 提示返回
		    if(g_bMsgBoxFlag)
		    {
		    	::PutCommand("Over.txt");
		    }
		    else
	    	{
					if(pwndMask_SingleStep!=NULL)
						pwndMask_SingleStep->Update();
					if(pwndBtn_SingleStep!=NULL)
						{
							pwndBtn_SingleStep->SetPropValueT("enabled",double(TRUE));
							pwndBtn_SingleStep->Update();
						}
	    	}
	    }
		}
		else
		{
			MsgBox(g_MultiLanguage["PICKER_M3_ALARM5025"], tmFT_CODE_TECH); // 請在手動下
		}
	}
 	if(pwnd == pwndBtnLastP) // 返回上一頁 Btn按下彈起
	{	
		printf("pwndBtnLastP\n");
		printf("u_PickerType=%d\n",u_PickerType);
		if(u_PickerType==MechType5)
			::PutCommand("QTeach_PlaceP.txt");
		else if(u_PickerType==MechType3)
			::PutCommand("QTeach_PlaceP_Axis3.txt");
	}
	
	if(pwnd == pwndBtn_SingleStep) // 單步 Btn按下彈起
	{	
		if(u_wPickerOPSatus == STATE_MANUAL)
		{
			SendCommand(MODE_SINGLESTEP); // 切換至單步模式
			ShowSingleStep();
		}
		if(u_wPickerOPSatus == STATE_SINGLESTEP)
		{
			SendCommand(MODE_MANUAL); // 切換至手動模式
			ShowSingleStep();
		}
	}
	if(pwnd == pwndBtn_SingleCycle) // 單循環 Btn按下彈起
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
	}
	
	if(pwnd == pwndBtn_SingleNextStep) // 下一步 Btn按下彈起
	{	
		printf("pwndBtn_SingleNextStep\n");
    if(u_wPickerOPSatus == STATE_SINGLESTEP)
    {
			wSingle_Step = (WORD)GetDBValue("MACHINE_INTERFACE_WSTEPNUMBER").lValue;
	    if(g_ptaskpicker != NULL && wSingle_Step != wSingle_StepOld)
	    {
	    		printf("Now step=%d\n",wSingle_Step);
	        g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDValue, NULL);
	        printf("SendCMD%d:%x=1\n",CONST_REQ_COMMAND,nCMDValue);
	        wSingle_StepOld = wSingle_Step;    
	        b_Single_Step_Process = 1;   
	        UpdateNextStep();        
	    }
	  }
    else
    {
    	 MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
    }
	}
	
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : GetPosTag()                  																	 |
|       		: 紀錄 位置標籤 參數4 提供首頁位置補正用												 |
+---------------------------------------------------------------------------*/
void	GetPosTag()
{
	char 	pDBID[256];
	int type=0,group=0,axis=0,pos=0;
	
	// 清空原有 紀錄 取出、置放 位置
	for(int i=0;i<10;i++)
	{
		memset(pDBID, 0 ,sizeof(pDBID));
		sprintf(pDBID,"AUSTON3_PARAMETER_MOTION_PROFILE_TARGET_POSITION%d",i+1);
		SetDBValue(pDBID, 0);
	}
	
	for(int i =0;i<MaxDBNum;i++)	// 紀錄 取出、置放 位置
	{
		memset(pDBID, 0 ,sizeof(pDBID));
		sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		type = GetDBValue(pDBID).lValue;

		if(type==1) // ACTION_TYPE = 軸動作
		{
			memset(pDBID, 0 ,sizeof(pDBID));
			sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4",i+1); // 群組
			group = GetDBValue(pDBID).lValue;

			if(group==6) // 6=取物取件
			{
				printf("Set Group 6\n");
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1); // 哪一軸
				axis = GetDBValue(pDBID).lValue;
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1",i+1); // 位置
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
			else if(group==9) // 9=置物放置
			{
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1); // 哪一軸
				axis = GetDBValue(pDBID).lValue;
				memset(pDBID, 0 ,sizeof(pDBID));
				sprintf(pDBID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER1",i+1); // 位置
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
/*---------------------------------------------------------------------------+
|  Function : Download()                  																	 |
|       		: 下載資料至主機																								 |
+---------------------------------------------------------------------------*/
void	Download()
{
	printf("Download\n");
	char 	pDataID[256];
	int value =0;
	int Num =0;
	char  str_loading[256] = "\0"; // 下載進度
	int u_loading=0;
	printf("MaxDBNum=%d\n",MaxDBNum);
	
	pwndButtonDownload->SetPropValueT("enabled", double(FALSE)); // "下載"按鍵上鎖
//	pwndLoadingBar->SetPropValueT("right",24); // 重置進度條
//	pwndLoadingBar->CreateA();
//	pwndLoadingBar->Update();
//	pwndLoadingMask->Update();
//	pwndLoadingMask2->Update();
//	pwndLoadingStr->SetPropValueT("bgc",0xF7BE); // 進度值
//	pwndLoadingStr->SetPropValueT("fgc",0x0001);
//	pwndLoadingStr->Update();
	
	for(int i =0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		value = GetDBValue(pDataID).lValue;
		//printf("%s=%d\n",pDataID,value);
		if(value !=0) // ACTION_TYPE 有值
		{
			//memset(pDataID, 0 ,sizeof(pDataID));
			//sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
			//SetDBValue(pDataID, i+1);
			//printf("Set mem %s=%d\n",pDataID,i+1);
			Num++; // 計數 教導有幾步驟
		}
		else // 數完步數
		{
			if(g_ptaskpicker != NULL) // 傳送教導至28
			{
				SetDBValue("MACHINE_PROFILE_STEPNUM", 0, TRUE);
				
				printf("Send cmd\n");
				char 	ptempDataID[256];
//				for(int i =0;i<Num*CONTROLTYPENUM;i++)
//				{
//					if((i%13)<8) // 參數6以後不送
//					{
//						memset(ptempDataID, 0 ,sizeof(ptempDataID));
//						sprintf(ptempDataID,u_pszDBString[i]);
//						printf("%s=%d\n",u_pszDBString[i],GetDBValue(ptempDataID));
//						g_ptaskpicker->WriteValue(CONST_REQ_WRITE, 1 ,&u_pszDBString[i]);
//					}
//					
//					if(pwndLoadingStr!=NULL && pwndLoadingBar!=NULL)//顯示下載進度
//					{
//						pwndLoadingBar->SetPropValueT("right",96+u_loading*4);
//						pwndLoadingBar->CreateA();
//						pwndLoadingBar->Update();
//						u_loading = 1+((100*i)/(Num*CONTROLTYPENUM));
//						sprintf(str_loading ,"%d",u_loading);
//						if(u_loading>50)
//							{
//								pwndLoadingStr->SetPropValueT("bgc",0x09A6);
//								pwndLoadingStr->SetPropValueT("fgc",0xFFFF);
//								pwndLoadingStr->CreateA();
//							}
//						pwndLoadingStr->SetPropValueT("text",str_loading);
//						pwndLoadingStr->Update();
//					}
//				}
				g_ptaskpicker->WriteValue(CONST_REQ_WRITE, Num*CONTROLTYPENUM ,u_pszDBString);
				SetDBValue("MACHINE_PROFILE_STEPNUM", Num, TRUE);
			}
			pwndButtonDownload->SetPropValueT("enabled", double(TRUE)); // "下載"按鍵解鎖
	
			return;
		}
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
|  Function : ShowSingleStep()	   																		       |
|       		: 顯示 單步測試按鈕們																						 |
+---------------------------------------------------------------------------*/
void	ShowSingleStep()
{
	if(pwndMask_SingleStep!=NULL)
		pwndMask_SingleStep->Update();
	if(pwndBtn_SingleStep!=NULL)
		{
			pwndBtn_SingleStep->SetPropValueT("enabled",double(TRUE));
			if(u_wPickerOPSatus == STATE_SINGLESTEP)
			{
				pwndBtn_SingleStep->SetPropValueT("upbitmap","res_tm640/pic/PGBtn_SingleRun.bmp");
			}
			else if(u_wPickerOPSatus == STATE_MANUAL)
			{
				pwndBtn_SingleStep->SetPropValueT("upbitmap","res_tm640/pic/PGBtn_SingleEND.bmp");
			}
			pwndBtn_SingleStep->CreateA();
			pwndBtn_SingleStep->Show();
		}
	if(pwndBtn_SingleCycle!=NULL)
		{
			if(u_wPickerOPSatus == STATE_MANUAL)
			{
				pwndBtn_SingleCycle->SetPropValueT("enabled",double(TRUE));
				pwndBtn_SingleCycle->Update();
			}
			else if(u_wPickerOPSatus == STATE_SINGLESTEP)
			{
				pwndBtn_SingleCycle->SetPropValueT("enabled",double(FALSE));
			}
		}
	if(pwndBtn_SingleNextStep!=NULL)
		{
			if(u_wPickerOPSatus == STATE_MANUAL)
			{
				pwndBtn_SingleNextStep->SetPropValueT("enabled",double(TRUE));
				pwndBtn_SingleNextStep->Update();
			}
			else if(u_wPickerOPSatus == STATE_SINGLESTEP)
			{
				pwndBtn_SingleNextStep->SetPropValueT("enabled",double(FALSE));
			}
		}
}
/*---------------------------------------------------------------------------+
|  Function : UpdateNextStep()	   																		       |
|       		: 更新 下一步按鈕																						 |
+---------------------------------------------------------------------------*/
void UpdateNextStep()
{
	if(pwndBtn_SingleNextStep!=NULL)
		{
			pwndBtn_SingleNextStep->SetPropValueT("upbitmap",ProcessImgpath[b_Single_Step_Process]);
			printf("Set pwndBtn_SingleNextStep =%s\n",ProcessImgpath[b_Single_Step_Process]);
			pwndBtn_SingleNextStep->CreateA();
			pwndBtn_SingleNextStep->Show();
		}
}