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

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

//extern	int     g_nDemo;

int		u_nWndExTag					= -1;				//Button Extag
WORD	u_wAction						= -1;
/*===========================================================================+
|           Class implementation - maintoolbar                               |
+===========================================================================*/
WORD	OnKeyA(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex)
{
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
	else if(u_nWndExTag == 1001)
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
	}
	 
	
	
	if(u_nWndExTag == 12)	
	{
		//依夾爪使用數量來選擇toolbar畫面文件
		if(g_TxCNCData.shmCNCData.PARAM.maker_param[33]<2)
			pButton->SetPropValueT("barname", "toolbar12_1.txt");
		else
			pButton->SetPropValueT("barname", "toolbar12_2.txt");
	}
	Now_Push_Btn = pButton;
	return wKey;
}

WORD	OnKeyAEnd(CtmToolBar* pToolBar, CtmButton* pButton, WORD wKey, int iButtonIndex)
{
	pButton->GetPropValueT("extag", &u_nWndExTag, sizeof(u_nWndExTag));
	pButton->GetPropValueT("action", &u_wAction, sizeof(u_wAction));
	if(u_wAction == 4096 && u_nWndExTag != 4) //extag = 4 是reset 不須送出訊號給view
	{
			SendMsg(MSG_SH_APP_TOOLBAR_ONKEY,wKey,u_nWndExTag,NULL);  //送出msg配合view做F1~F10的動作
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
	return 0;
}
