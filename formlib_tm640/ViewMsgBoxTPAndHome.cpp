/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

#include	"ViewMsgBoxTPAndHome.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define 	STOP					0
#define 	CYCLE_START		1
#define 	FEED_DOWN			2


#define		STATE_INITIAL				0
#define		STATE_PLUS					1		//按下Button"+"
#define		STATE_MINUS					2		//按下Button"-"

/*========================================================+
|						Global Variable																|
+========================================================*/



BOOL 	 		u_bEnterKey 					= FALSE;

CtmWnd*		u_pwndEditTPNum				= NULL;
CtmWnd*		u_pwndStaticAlarm			= NULL;

CtmWnd*		u_pwndBtnSTOP					= NULL;
CtmWnd*		u_pwndBtnRUN					= NULL;
CtmWnd*		u_pwndBtnPAUSE				= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;

//速度控制↓---------------------------------------
CtmWnd*		u_pwndStaticSpeedShow		= NULL;
CtmWnd*		u_pwndBtnPlus						= NULL;
CtmWnd*		u_pwndBtnMinus					= NULL;


int			u_lSpeed							=0;
int				ViewMode							=-1;			//0:TP   1:home
int				RunStatus							=0;		//目前工作模式 (0=停止 / 1=執行 / 2=暫停)	
int 			TeachPointCnt					=-1;
BOOL			TeachPointError				=FALSE;		//TRUE:正確  FALSE:錯誤
BOOL			FristUpdate						=TRUE;
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy↓


BOOL		OnCreateA_GotoTP(CtmWnd* pwndSender)
{
	ViewMode = 0;
	u_pwndEditTPNum		= pwndSender->FindControlFromName("TeathPointNum");
	TeachPointCnt			= GetTeachPointCnt();
	OnCreateA(pwndSender);
	RobotStatus = OP_JOG_MODE;
	return TRUE;
}
BOOL		OnCreateA_ReturnHmoe(CtmWnd* pwndSender)
{
	TeachPointError = TRUE;
	ViewMode = 1;;
	RobotStatus = OP_HOME_MODE;
	OnCreateA(pwndSender);
	return TRUE;
}

BOOL		OnCreateA(CtmWnd* pwndSender)
{	
	u_pwndStaticAlarm	= pwndSender->FindControlFromName("StaticAlarm");
	u_pwndBtnSTOP			= pwndSender->FindControlFromName("BtnStop");
	u_pwndBtnRUN			= pwndSender->FindControlFromName("BtnRun");
	u_pwndBtnPAUSE		= pwndSender->FindControlFromName("BtnPause");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("BtnCancel");
	
	//=====速度控制=====
	u_pwndStaticSpeedShow	= pwndSender->FindControlFromName("StaticSpeedShow");
	u_pwndBtnPlus					= pwndSender->FindControlFromName("btnPlus");
	u_pwndBtnMinus				= pwndSender->FindControlFromName("btnMinus");	
	
	return TRUE;
}

//20140624↑
WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	SetAlarmStatic(0);
	if(pwndTemp == u_pwndBtnCancel)
	{
		if(RunStatus == STOP)
			Exit();
		else
			SetAlarmStatic(2);
	}
	else if(pwndTemp == u_pwndBtnPlus)
		SetAndShowSpeed(STATE_PLUS);
	else if(pwndTemp == u_pwndBtnMinus)
		SetAndShowSpeed(STATE_MINUS);
	else	//開始 暫停 停止	
	{ 
		if(pwndTemp == u_pwndBtnRUN && RunStatus != CYCLE_START)//開始
		{
			if(u_pwndEditTPNum != NULL)
				ReadEditNum();
			
			if(TeachPointError == TRUE)	//回原點在create時就先給TRUE
			{
				RunStatus = CYCLE_START;
				SetViewStatus(RunStatus);
			}
		}
		else if(pwndTemp == u_pwndBtnPAUSE && RunStatus == CYCLE_START)//暫停
		{
			RunStatus = FEED_DOWN;
			SetViewStatus(RunStatus);
		}
		else if(pwndTemp == u_pwndBtnSTOP && RunStatus != STOP)//停止
		{
			RunStatus = STOP;
			SetViewStatus(RunStatus);
		}
		char temp = RunStatus;
		//sprintf(temp,"%d",RunStatus);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_status));
		SetCycleBtnImage(RunStatus);
	}

	return wIDControl;
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;	
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	if(FristUpdate && ViewMode == 0)
	{
		FristUpdate = FALSE;
		SetAndShowSpeed(STATE_INITIAL);
		u_pwndEditTPNum->SetPropValueT("max",TeachPointCnt);
		u_pwndEditTPNum->SetPropValueT("min",1);
		u_pwndEditTPNum->Update();
		SetCycleBtnImage(RunStatus);
	}
	else if(FristUpdate && ViewMode == 1)
	{
		FristUpdate = FALSE;
		SetAndShowSpeed(STATE_INITIAL);
		((CtmFormView*)pwndSender)->OnLoseFocus();	
		SetCycleBtnImage(RunStatus);
	}
	
	if(RunStatus != STOP && g_pRxCNCData->CNC_prog_end == 1)
	{
		RunStatus = STOP;
		SetViewStatus(RunStatus);
		SetCycleBtnImage(RunStatus);
	}
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	int temp = 0;
	char temp2 = 0;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_goto_tp_no,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_goto_tp_no));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_status,&temp2,sizeof(g_TxCNCData.shmCNCData.CNC_jog_status));
}

void	ReadEditNum()
{
	int InputTPNum=0;
	u_pwndEditTPNum->GetPropValueT("value", &InputTPNum, sizeof(InputTPNum));
	if(InputTPNum > TeachPointCnt || InputTPNum < 1)
	{
		TeachPointError = FALSE;
		SetAlarmStatic(1);
	}
	else
	{
		TeachPointError = TRUE;
		SetAlarmStatic(0);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_goto_tp_no,&InputTPNum,sizeof(g_TxCNCData.shmCNCData.CNC_goto_tp_no));
	}
	
}


void	SetAlarmStatic(int mode)
{
	if(mode == 0)
		u_pwndStaticAlarm->SetPropValueT("textID","ROBOT_STR_DUMMY");
	else if(mode == 1)
		u_pwndStaticAlarm->SetPropValueT("textID","ROBOT_MSG_NULL_TEACHPOINT");
	else	if(mode == 2)
		u_pwndStaticAlarm->SetPropValueT("textID","ROBOT_CANT_EXIT");
		
	u_pwndStaticAlarm->Update();
	
}


void	SetViewStatus(int f_RunStatus)
{
	if(f_RunStatus == 1 || f_RunStatus == 2)		//start  pause2015/3/16 上午 11:26:21
	{
		if(u_pwndEditTPNum != NULL)
		{
			u_pwndEditTPNum->SetPropValueT("taborder",-2);
			u_pwndEditTPNum->Update();
		}
	}
	else if(f_RunStatus == 0)
	{
		if(u_pwndEditTPNum != NULL)
		{
			u_pwndEditTPNum->SetPropValueT("taborder",double(0));
			u_pwndEditTPNum->Update();
		}
	}
}

void	SetCycleBtnImage(int f_RunStatus) // 0stop 1run 2pause
{
	if(f_RunStatus == 0)
	{
		((CtmButton*)u_pwndBtnSTOP)->Press(tmBUTTON_DOWN);
		((CtmButton*)u_pwndBtnRUN)->Press(tmBUTTON_UP);
		((CtmButton*)u_pwndBtnPAUSE)->Press(tmBUTTON_UP);
	}
	else if(f_RunStatus == 1)
	{
		((CtmButton*)u_pwndBtnRUN)->Press(tmBUTTON_DOWN);
		((CtmButton*)u_pwndBtnSTOP)->Press(tmBUTTON_UP);
		((CtmButton*)u_pwndBtnPAUSE)->Press(tmBUTTON_UP);
	}
	else if(f_RunStatus == 2)
	{
		((CtmButton*)u_pwndBtnPAUSE)->Press(tmBUTTON_DOWN);
		((CtmButton*)u_pwndBtnRUN)->Press(tmBUTTON_UP);
		((CtmButton*)u_pwndBtnSTOP)->Press(tmBUTTON_UP);
	}
}

void		SetAndShowSpeed(int nType)		//設定機械手臂的移動速度
{
	if(u_pwndStaticSpeedShow != NULL)
	{
		char	temp[15] 		= "\0";
		char	szTemp[15]		= "\0";
		memset(temp, 0, sizeof(temp));
		memset(szTemp, 0, sizeof(szTemp));
		
		//控制參數列表完成後，"MAKER_PARAM1"->移動速度的控制參數(機械手臂)
		u_lSpeed = GetDBValue("CNC_JOG_OVER_CMD").lValue;
		
		//nType等於SPEED_INITIAL時u_lSpeed不須處理
		if(nType == STATE_PLUS)		//+
		{
			u_lSpeed+=10;
			if(u_lSpeed>100)		//MAX =100%
			{
				u_lSpeed = 100;
				return;
			}
		}
		else if(nType == STATE_MINUS)		//-
		{
			u_lSpeed-=10;
			if((u_lSpeed<0)||(u_lSpeed>100))		//MIN =0%
			{
				u_lSpeed =0;
				return;
			}
		}
		//控制參數列表完成後，"MAKER_PARAM1"->移動速度的控制參數(機械手臂)
		SetDBValue("CNC_JOG_OVER_CMD", u_lSpeed);
		sprintf(temp, "%d%s", u_lSpeed, "%");
		CodeChange(szTemp, temp);
		u_pwndStaticSpeedShow->SetPropValueT("text", szTemp);
		u_pwndStaticSpeedShow->Update();
	}
}