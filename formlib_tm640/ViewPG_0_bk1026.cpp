#include	"ViewPG_0.h"
#include    "../database.h"
#include    "../init.h"
#include    "../selectedit.h"
#include    "../tmconfig.h"
//#include    "../lib/hmi_control_data/tmControl_CurveExData.h"
#include    "../dialog.h" 

#define     CONST_REQ_COMMAND       6
#define     CONST_REQ_WRITE         3
#define     CONTROLTYPENUM          13
#define     MAXSTEP               100
#define     STATE_SINGLESTEP        2
#define		STATE_HAND				3
#define     STATE_FULLAUTO          4 
#define     PARA_NUM	            10 
int     iEditNo=0,iCheckBoxAct=0,iEditACTION=0,iSelectEditACTION=0,iStaticACTION=0,iStaticEditNo=0;
int 	No1=0,No=0;
int		SelectNo = 0;
int		Cleanhelp =0;
int		MaxDBNum = 100;
bool	m_bEnterKey = FALSE;
char 	pNo[256]	={NULL}; 
char 	pStaticNo[256]	={NULL}; 
WORD        u_wPickerOPSatus    = 0;
WORD        wStep  = 0,wStepOld;
char*       u_pszDBString[MAXSTEP*CONTROLTYPENUM] = {NULL};

CtmWnd*		pwndEditNo[32] 	={NULL}; 		//序号列 腹
CtmWnd*		pwndStaticEditNo[32] 	={NULL}; 		//序号列 腹
CtmWnd*		pwndCheckBoxAct[32] ={NULL}; 	//动作列 笆
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

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	
	
	{	//初始定义 ﹍﹚竡
		iEditNo 	 			= GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);					//序号列数量 腹计秖
		iStaticEditNo 	 			= GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", pwndStaticEditNo);	//序号列数量 腹计秖
		iCheckBoxAct				= GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);	//动作列数量 笆计秖
		iEditACTION				= GetSpecialControlNum(pwndSender, "EditACTION", "CtmEditX1", pwndEditACTION);
		iSelectEditACTION				= GetSpecialControlNum(pwndSender, "SelectEditACTION", "CtmSelectEdit", pwndSelectEditACTION);
		iStaticACTION				= GetSpecialControlNum(pwndSender, "StaticACTION", "CtmStaticX2", pwndStaticACTION);
		No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39").lValue;	
		SelectNo = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40").lValue;	
		//SetDBValue("MACHINE_PROFILE_STEPNUM", 0);		//数量清零 计秖睲箂
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
	
	int SelectFlag =0;
	for (int i = 0; i < iEditNo; i++)	//获叁序号 莉把腹
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
			if(pwndEditNo[i] != NULL)
			{
				pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				pwndCheckBoxAct[i]->Update();
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
			sprintf(pStaticNo,"%d",No-5);
			pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
			pwndStaticEditNo[i]->Update();
		}
		if(pwndEditNo[i] != NULL)			//
		{
			if(No==SelectNo)
			{
				pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
				SelectFlag =1;
			}
		}
	}
	if(pwndEditNo[0] != NULL && !SelectFlag)			//光标高亮
	{
		pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
		sscanf(pNo,"%d",&SelectNo);
		pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
	}
	
	UpdateText();
	
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
        int StepNumMax = 15+No1;
        if(wStep >0 && wStep < StepNumMin)
        {
        	PageUp(pwndSender);
			UpdateText();
        }
        else if(wStep <=100 && wStep > StepNumMax)
        {
        	PageDown(pwndSender);
			UpdateText();
        }
        else if(wStep >0 && wStep != wStepOld)
        {
        	for(int i =0; i < iCheckBoxAct; i++)
			{
				int StepNum = i+1+No1;
				if(wStep == StepNum && pwndCheckBoxAct[i] != NULL)
				{
					for(int k =0; k < iCheckBoxAct; k++)		//高亮显示
					{
						No = k+1+No1;
						if(No <6)
						{
							pwndCheckBoxAct[k]->SetPropValueT("bgc",50712);
						}
						else
						{
							pwndCheckBoxAct[k]->SetPropValueT("bgc",0xFFDF);
						}
					}
					((CtmFormView*)pwndSender)->Goto(pwndCheckBoxAct[i]->GetTabOrder());
					pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
					pwndCheckBoxAct[i]->Update();
				}
			}
        	wStepOld = wStep;
        }
    }
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
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
	if(u_wPickerOPSatus != STATE_FULLAUTO) //自动下限制
	{
		if (wKey == 0x000B)		//下一页
		{
			PageDown(pwndSender);
			UpdateText();
			return _NULL_KEY;
		}
		else if (wKey == 0x000A)	//上一页
		{
			PageUp(pwndSender);
			UpdateText();
			return _NULL_KEY;
		}
		/*else if(wKey == 0x0005)		//开始编辑
		{
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", SelectNo);
			if(ActionType != 0)
			{
				switch(ActionType)
				{
					case 1:		//轴动作
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_AXISACT.txt");
						usleep(100*1000);
					break;
					case 2:		//等待
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_WAIT.txt");
						usleep(100*1000);
					break;
					case 3:		//允许
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_PERMIT.txt");
						usleep(100*1000);
					break;
					case 4:		//阀门
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_VALVE.txt");
						usleep(100*1000);
					break;
					case 5:		//标签
						if(ACTIONNUM != 1)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							::PutCommand("PG_2_LABEL.txt");
							usleep(100*1000);
						}
					break;
					case 6:		//跳转
						if(ACTIONNUM != 1)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							::PutCommand("PG_2_LABEL.txt");
							usleep(100*1000);
						}
					break;
					case 8:		//检测
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_TEST.txt");
						usleep(100*1000);
					break;
					case 12:	//堆叠
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						::PutCommand("PG_2_PILE.txt");
						usleep(100*1000);
					break;
					case 13:	//子程序
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
		else if (wKey == 0x0002)	//插叁
		{
			if(SelectNo > 5)
			{
				if(ActionType != 5)
				{
					Insert(SelectNo);
					UpdateText();
				}
				else if(ACTIONNUM != 1)
				{
					Insert(SelectNo);
					UpdateText();
				}
			}
		}
		else if (wKey == 0x0003)	//删除
		{
			if(SelectNo > 5)
			{
				if(ActionType != 5 && ActionType != 6)
				{
					Delete(SelectNo);
					UpdateText();
				}
				else if(ACTIONNUM != 1)
				{
					Delete(SelectNo);
					UpdateText();
				}
			}
		}
		else if (wKey == 0x0007)	//清理
		{
			Cleanhelp =0;
			Clean();
			UpdateText();
		}
		else if (wKey == 0x0008)	//清除
		{
			MsgBox(g_MultiLanguage["PICKER_CONFIRMDELETE"], tmFT_CODE_TECH);
		    if(g_bMsgBoxFlag)
		    {
		    	Clear();
		    	UpdateText();
		    }
		}
		else if (wKey == 0x0004)	//下载
		{
			if(u_wPickerOPSatus == STATE_HAND)
			{
				Cleanhelp =0;
				Clean();
				Download();
				UpdateText();
				Prompt(g_MultiLanguage["PICKER_DATADOWNLOADFINISH"],1); 
			}
			else
			{
				MsgBox(g_MultiLanguage["PICKER_M3_ALARM1042"], tmFT_CODE_TECH);
			}
		}
		else if(wKey == 0x0001)
    	{
    	    if(u_wPickerOPSatus == STATE_SINGLESTEP)
    	    {
    	        if(g_ptaskpicker != NULL)
    	        {
    	            g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDValue, NULL);
    	        }
    	        if(u_pwndSingle != NULL)
    	        {
    	            u_pwndSingle->SetPropValueT("upbitmap", "res_7ZTQ8/pic/ButtonDown.bmp");
    	            u_pwndSingle->Update();
    	        }
    	    }
    	    else
    	    {
    	         MsgBox(g_MultiLanguage["PICKER_NOTSINGLESTEP"], tmFT_CODE_TECH);
    	    }
    	    return _NULL_KEY;
    	}
    	else if(wKey == 0x0009)
    	{
    	    if(u_wPickerOPSatus == STATE_SINGLESTEP)
    	    {
    	        if(g_ptaskpicker != NULL)
    	        {
    	            g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &nCMDLastStep, NULL);
    	            printf("nCMDLastStep:%X\n",nCMDLastStep);
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
	}
	
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_wPickerOPSatus != STATE_FULLAUTO) //自动下限制 笆
	{
		if(pwnd == pwndButtonStartEdit)
 		{
 			printf("Start Edit\n");
 			char 	pDataID[256];
			int		ActionType = 0, ACTIONNUM = 0;
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",SelectNo);
			ActionType =GetDBValue(pDataID).lValue;
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_NUM", SelectNo); 
			ACTIONNUM =GetDBValue(pDataID).lValue;
 			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40", SelectNo);
			if(ActionType != 0)
			{
				switch(ActionType)
				{
					case 1:		//轴动作 禸笆
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						//pwnd->SetPropValueT("formname","PG_2_AXISACT.txt");
						::PutCommand("PG_2_AXISACT.txt");
						usleep(100*1000);
					break;
					case 2:		//等待 单
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						//pwnd->SetPropValueT("formname","PG_2_WAIT.txt");
						::PutCommand("PG_2_WAIT.txt");
						usleep(100*1000);
					break;
					case 3:		//允许 す砛
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						//pwnd->SetPropValueT("formname","PG_2_PERMIT.txt");
						::PutCommand("PG_2_PERMIT.txt");
						usleep(100*1000);
					break;
					case 4:		//阀门 恢
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						//pwnd->SetPropValueT("formname","PG_2_VALVE.txt");
						::PutCommand("PG_2_VALVE.txt");
						usleep(100*1000);
					break;
					case 5:		//标签夹乓
						if(ACTIONNUM != 1)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_LABEL.txt");
							::PutCommand("PG_2_LABEL.txt");
						usleep(100*1000);
						}
					break;
					case 6:		//跳转 诫锣
						if(ACTIONNUM != 1)
						{
							SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
							//pwnd->SetPropValueT("formname","PG_2_LABEL.txt");
							::PutCommand("PG_2_LABEL.txt");
						usleep(100*1000);
						}
					break;
					case 8:		//检测 浪代
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						//pwnd->SetPropValueT("formname","PG_2_TEST.txt");
						::PutCommand("PG_2_TEST.txt");
						usleep(100*1000);
					break;
					case 12:	//堆叠 帮舼
						SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41", ActionType);
						//pwnd->SetPropValueT("formname","PG_2_PILE.txt");
						::PutCommand("PG_2_PILE.txt");
						usleep(100*1000);
					break;
					case 13:	//子程序 祘
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
	
	for(int i =0; i < iCheckBoxAct; i++)			//当前排高亮显示，并获叁当前排序号
	{
		if(pwndCheckBoxAct[i] != NULL && pwnd == pwndCheckBoxAct[i])
		{
			for(int k =0; k < iCheckBoxAct; k++)		//高亮显示
			{
				No = k+1+No1;
				if(No <6)
				{
					pwndCheckBoxAct[k]->SetPropValueT("bgc",50712);
				}
				else
				{
					pwndCheckBoxAct[k]->SetPropValueT("bgc",0xFFDF);
				}
				pwndCheckBoxAct[k]->Update();
			}
			((CtmFormView*)pwndSender)->Goto(pwndCheckBoxAct[i]->GetTabOrder());
			pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
			
			if(pwndEditNo[i] != NULL)					//获叁序号
			{
				pwndEditNo[i]->GetPropValueT("text", pNo, sizeof(pNo));
				sscanf(pNo,"%d",&SelectNo);
			}
			pwndCheckBoxAct[i]->Update();
		}
	}
	UpdateText();
	
	if(pwnd == NULL)	return wIDControl;
	return TRUE;
}
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
}

void    OnDestroyA(CtmWnd* pwndSender)
{
    for(int i = 0; i < MAXSTEP*CONTROLTYPENUM; i++)
    {
        if(u_pszDBString[i] != NULL)
            free(u_pszDBString[i]);
        u_pszDBString[i] = NULL;
    }
    
}
void	PageDown(CtmWnd* pwndSender)		//下一页
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
		printf("No=%d,",No);
		if(No<6)
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				pwndCheckBoxAct[i]->Update();
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
				pwndCheckBoxAct[i]->SetPropValueT("bgc",0xFFDF);
				pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				sprintf(pStaticNo,"%d",No-5);
				printf("pStaticNo=%s\n",pStaticNo);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Update();
			}
		}
		if(pwndEditNo[0] != NULL)			//光标高亮
		{
			pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
			sscanf(pNo,"%d",&SelectNo);
			pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
		}
	}
	No1 = No1 +iPageturn;
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", No1);
	
	//((CtmFormView*)pwndSender)->Goto(pwndButtonPageDown->GetTabOrder());
}


void	PageUp(CtmWnd* pwndSender)			//上一页
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
				pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				pwndCheckBoxAct[i]->Update();
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
				pwndCheckBoxAct[i]->SetPropValueT("bgc",0xFFDF);
				pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				memset(pStaticNo, 0 ,sizeof(pStaticNo));
				sprintf(pStaticNo,"%d",No-5);
				pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
				pwndStaticEditNo[i]->Update();
			}
		}
		if(pwndEditNo[0] != NULL)			//光标高亮
		{
			pwndEditNo[0]->GetPropValueT("text", pNo, sizeof(pNo));
			sscanf(pNo,"%d",&SelectNo);
			pwndCheckBoxAct[0]->SetPropValueT("bgc",36256);
		}
	}
	No1 = No1 -iPageturn;
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED39", No1);
	
}

void	Insert(int SelectNo)				//插叁
{
	char 	pDataID[256];
	int		value = 0;
	for(int i=MaxDBNum;i>SelectNo;i--)
	{
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

void	Delete(int SelectNo)				//删除
{
	char 	pDataID[256];
	int		value = 0;
	for(int i=SelectNo;i<MaxDBNum;i++)
	{
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

void	Clear()								//清空所有数据
{
	char 	pDataID[256];
	for(int i=6;i<MaxDBNum;i++)
	{
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
	SetDBValue("MACHINE_PROFILE_NUM7_ACTION_TYPE", 6);
	SetDBValue("MACHINE_PROFILE_NUM7_ACTION_NUM", 1);
}

void	Clean()								//清理空行
{
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
			Delete(Cleanhelp+1);
			Clean();
		}
	}
	Prompt(g_MultiLanguage["MSG_DUMMY"]);
}

void	Download()							//下载资料至主机
{
	char 	pDataID[256];
	int value =0;
	int Num =0;
	for(int i =0;i<MaxDBNum;i++)
	{
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		value = GetDBValue(pDataID).lValue;
		if(value !=0)
		{
			memset(pDataID, 0 ,sizeof(pDataID));
			sprintf(pDataID,"MACHINE_PROFILE_NUM%d_ACTION_STEP",i+1);
			SetDBValue(pDataID, i+1);
			Num++;
		}
		else
		{
			if(g_ptaskpicker != NULL)
			{
				SetDBValue("MACHINE_PROFILE_STEPNUM", 0, TRUE);
				g_ptaskpicker->WriteValue(CONST_REQ_WRITE, Num*CONTROLTYPENUM ,u_pszDBString);
				SetDBValue("MACHINE_PROFILE_STEPNUM", Num, TRUE);
			}
			printf("Num=%d\n",Num);
			return;
		}
	}
}

void	UpdateText()						//
{
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
	for(int i=0;i<iCheckBoxAct;i++)
	{
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
		printf("Para1:%d,Para2:%d,Para3:%d,Para5:%d\n",wActPara1,wActPara2,wActPara3,wActPara5);
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
		printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
		switch(ACTIONTYPE)
		{
			case 0:		//
				sprintf(pDataID,"VW_PICKER_NULL");
				break;
			case 1:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS");
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS");
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS");
						break;
					default:
						break;
				}
				if(ACTIONNUM <= 3)
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1");
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2");
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				else if(ACTIONNUM == 4)
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
			case 2:		//
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
			case 5:		//
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
			case 6:		//秨﹍ 挡? cjlee
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
		pwndCheckBoxAct[i]->UpdateAll();
		printf("\n");
	}
}