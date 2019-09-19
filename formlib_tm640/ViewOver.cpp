/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	| 
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewOver.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include	"../tmshmsg.h"
#include 	"../taskcnc.h"
#include 	"../gui.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define		STATE_INITIAL				0		//初始化速度
#define		STATE_PLUS					1		//按下Button"+"
#define		STATE_MINUS					2		//按下Button"-"

#define		COORDINATETYPE_LOCK	  -1		//NO座標
#define		COORDINATETYPE_J			0			//關節座標
#define		COORDINATETYPE_C			1			//卡氏座標
#define		COORDINATETYPE_T			2			//工作座標
//20140915↓		by Mario
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )


//JogLock
#define		COLOR_GRAY					50712	//灰色
#define		COLOR_BLUE					1046	//藍色蓋
#define		JOGLOCK_ON_EXTAG		20002	//上鎖
#define		JOGLOCK_OFF_EXTAG		0			//開鎖


#define		SAVE_SWITCH_DELAY		30

#define	JOGMODE_N_LOCK				16
#define	JOGMODE_LOCK					63488

#define COORDINATEMMTAB 	0
#define MOVEMODETAB 	18
#define EDITJOGTAB 		19
#define SPEEDPLUSTAB 	16
#define SPEEDMINUSTAB 17
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/

//色彩 20141008 add by Mario
int color[] ={0x0023,0x0022,NULL};
//速度控制↓---------------------------------------
CtmWnd*		u_pwndStaticSpeedShow		= NULL;
CtmWnd*		u_pwndBtnPlus						= NULL;
CtmWnd*		u_pwndBtnMinus					= NULL;

//右方移動控制↓-----------------------------------
CtmWnd*		u_pwndBtnCoordinateMM	= NULL;
CtmWnd*		u_pwndBtnPlusCTable[12]				= {NULL};		//右方移動控制區的Button"+"
CtmWnd*		u_pwndBtnMinusCTable[12]			= {NULL};		//右方移動控制區的Button"-"
CtmWnd*		u_pwndStaticCTable[12]				= {NULL};		//右方移動控制區的座標

//中間座標資訊↓---------------------------------
CtmWnd*		u_pwndCoordTable[7]					= {NULL};
int				u_nCoordTableCnt						=	0;
CtmWnd*		u_pwndOriginTable[7]				= {NULL};
int				u_nOriginTableCnt						=	0;

CtmWnd*		u_pwndJointTable[6]					= {NULL};
CtmWnd*		u_pwndJointLimitFlag[6]					= {NULL};
CtmWnd*		u_pwndCoordLimitFlag[6]					= {NULL};
int				u_nJointTableCnt						=	0;
int				u_nJointLimitFlagCnt						=	0;
int				u_nCoordLimitFlagCnt						=	0;

CtmWnd*		u_pwndStaticUnut[64]				= {NULL};
int				u_nStaticUnitCnt						=	0;

//其他控制設定↓-----------------------------------
CtmWnd*		u_pwndBtnMoveMode			= NULL;
CtmWnd*		u_pwndSelectEditJog		= NULL;
//CtmWnd*		u_pwndReturnHome			= NULL;
CtmWnd*		u_pwndBtnServoSwitch	= NULL;
//CtmWnd*		u_pwndStaticServoCmd	= NULL;
//CtmWnd*		u_pwndStaticServoCmdStatus	= NULL;

//時間元件-----------
CtmWnd*		u_pwndeditCYCLE_TIME_Hour			= NULL;
CtmWnd*		u_pwndeditCYCLE_TIME_Minut		= NULL;
CtmWnd*		u_pwndeditCYCLE_TIME_Second		= NULL;
	
CtmWnd*		u_pwndeditCNC_RUN_TIME_Hour		= NULL;
CtmWnd*		u_pwndeditCNC_RUN_TIME_Minut	= NULL;
CtmWnd*		u_pwndeditCNC_RUN_TIME_Second	= NULL;



//------------------------------------------------
int		u_nBtnPlusCTableCnt		=0;
int		u_nBtnMinusCTableCnt	=0;
int		u_nStaticCTableCnt		=0;


BOOL		u_bEnterKey						= FALSE;

int			u_lSpeed							=0;
int			u_nMoveControlType		=COORDINATETYPE_J;//COORDINATETYPE_LOCK;
int			u_nTPTableType				=0;

WORD		u_wDBvalueC_StartNo					=0;		//控制參數編號：卡氏座標的X座標
WORD		u_wDBvalueJ_StartNo					=0;		//控制參數編號：關節座標的J1軸
WORD		u_wDBvalueOrigin_StartNo		=0;		//控制參數編號：機械座標的X座標


//add		by		Mario
int			Unitindex						=0;		//表示目前單位(0=公制,1=英制)
CtmWnd*		u_pwndTPUnit[3]		= {NULL};		//指向卡氏座標xyz的顯示單位元件以配合公英制做改變
//90140915↑
int			u_Move_Mode					=0;			//移動模式 0=連續  1=吋動
//20140924
WORD				RangeStartC			=	0;			//手臂範圍參數
WORD				RangeStartJ			=	0;			//手臂範圍參數
//20140926	
BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次
//20140929
short				Press_Move_btn		=0;
BOOL	Start_Lock = FALSE;
//int Save_Switch_time = -1;

//2015/6/11 上午 10:19:56
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//TouchScreenAdjustForm();
	//g_TxCNCData.shmCNCData.CNC_jog_inc = 1;
	//抓取各元件的指針↓
	//=====速度控制=====	
	u_pwndStaticSpeedShow	= pwndSender->FindControlFromName("StaticSpeedShow");
	u_pwndBtnPlus					= pwndSender->FindControlFromName("btnPlus");
	u_pwndBtnMinus				= pwndSender->FindControlFromName("btnMinus");	
	//=====右方移動控制=====
	u_pwndBtnCoordinateMM	= pwndSender->FindControlFromName("BtnCoordinateMM");
	u_nBtnPlusCTableCnt		= GetSpecialControlNum(pwndSender, "btnPlusC", "CtmToolButton", u_pwndBtnPlusCTable);
	u_nBtnMinusCTableCnt	= GetSpecialControlNum(pwndSender, "btnMinusC", "CtmToolButton", u_pwndBtnMinusCTable);
	u_nStaticCTableCnt		= GetSpecialControlNum(pwndSender, "StaticC", "CtmStaticX2", u_pwndStaticCTable);
	
	
	//u_pwndBtnMinusPTable	=	pwndSender->FindControlFromName("btnMinusPos");		//Pos-
	//u_pwndBtnPlusPTable	 = pwndSender->FindControlFromName("btnPlusPos");			//Pos+
	//u_pwndStaticPosture =	pwndSender->FindControlFromName("StaticPos");				//Pos按鈕說明
	
	//=====中間當前座標位置=====
	u_nCoordTableCnt		= GetSpecialControlNum(pwndSender, "CTable", "CtmEditX1", u_pwndCoordTable);
	u_nOriginTableCnt		= GetSpecialControlNum(pwndSender, "OTable", "CtmEditX1", u_pwndOriginTable);
	
	u_nJointTableCnt		= GetSpecialControlNum(pwndSender, "JTable", "CtmEditX1", u_pwndJointTable);
	u_nCoordLimitFlagCnt		= GetSpecialControlNum(pwndSender, "CoordLimitFlag", "CtmImageBox", u_pwndCoordLimitFlag);
	u_nJointLimitFlagCnt		= GetSpecialControlNum(pwndSender, "JointLimitFlag", "CtmImageBox", u_pwndJointLimitFlag);
	
	u_nStaticUnitCnt		= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnut);
	
	//時間設定元件                        
	u_pwndeditCYCLE_TIME_Hour			= pwndSender->FindControlFromName("EditCycleTimeH");
	u_pwndeditCYCLE_TIME_Minut		= pwndSender->FindControlFromName("EditCycleTimeM");
	u_pwndeditCYCLE_TIME_Second		= pwndSender->FindControlFromName("EditCycleTimeS");
	                              
	u_pwndeditCNC_RUN_TIME_Hour		= pwndSender->FindControlFromName("EditRunTimeH");
	u_pwndeditCNC_RUN_TIME_Minut	= pwndSender->FindControlFromName("EditRunTimeM");
	u_pwndeditCNC_RUN_TIME_Second	= pwndSender->FindControlFromName("EditRunTimeS");                        
	                        
	                        
	
	//20140919↑				by Mario
	//=====其他控制設定=====
	u_pwndBtnMoveMode			= pwndSender->FindControlFromName("BtnMoveMode");
	u_pwndSelectEditJog		= pwndSender->FindControlFromName("SelectEditJog");
	u_pwndBtnServoSwitch	= pwndSender->FindControlFromName("BtnServoSwitch");
	//u_pwndStaticServoCmd	= pwndSender->FindControlFromName("StaticServoCmd2");
	//u_pwndStaticServoCmdStatus	= pwndSender->FindControlFromName("StaticServoSetStatus");
	
	u_wDBvalueOrigin_StartNo	= g_pDatabase->GetIndex(MACHINE_COORD_ID);
	u_wDBvalueJ_StartNo		   	= g_pDatabase->GetIndex(ANGULAR_COORD_ID);
	u_wDBvalueC_StartNo		   	= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	
	//SetDBValue("SERVO_PARAM6", 281);
	
	//讀取目前單位型態 20140915 by Mario
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	
	//抓取卡氏座標單位元件
	u_pwndTPUnit[0]	= pwndSender->FindControlFromName("StaticUnit04");
	u_pwndTPUnit[1] = pwndSender->FindControlFromName("StaticUnit05");
	u_pwndTPUnit[2] = pwndSender->FindControlFromName("StaticUnit06");
	//設定單位	
	//SetUnit(Unitindex);
	
	RangeStartC			= g_pDatabase->GetIndex("PLACE_RANGE_X_P");//為關節運動範圍的座標值
	RangeStartJ			= g_pDatabase->GetIndex("PLACE_RANGE_1_P");//為卡氏運動範圍的座標值	
	SetUnit();
	
	if(g_TxCNCData.shmCNCData.CNC_jog_mode == 0)
		RobotStatus = OP_JOG_MODE;
	else if(g_TxCNCData.shmCNCData.CNC_jog_mode == 1)
		RobotStatus = OP_INCJOG_MODE;
	return	TRUE;
}  

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	Start_Lock = FALSE;
	//Save_Switch_time=-1;
	/*
	for(int i =0; i<u_nBtnPlusCTableCnt; i++)
	{
		if(pwndTemp == u_pwndBtnPlusCTable[i])		//"+"
		{	
			if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_JOG_MODE || g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_INCJOG_MODE)
			{
				Press_Move_btn = i+1;
				SetJogMoveDate(u_nMoveControlType, STATE_PLUS, i);			//修改to CNC資料
			}
			else if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);
			return wIDControl;
		}
	}
	for(int i =0; i<u_nBtnMinusCTableCnt; i++)
	{
		if(pwndTemp == u_pwndBtnMinusCTable[i])		//"-"
		{	
			if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_JOG_MODE || g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_INCJOG_MODE)
			{
				Press_Move_btn = i+1;
				SetJogMoveDate(u_nMoveControlType, STATE_MINUS, i);			//修改to CNC資料
			}
			else if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);	
			return wIDControl;
		}
	}*/
	return wIDControl;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		return	wIDControl;
	}
	/*if(pwndTemp == u_pwndBtnServoSwitch)
	{
		char temp;
		//if(g_TxCNCData.shmCNCData.CNC_servo_on == 1)
		//{
			if(g_pRxCNCData->CNC_status!=M_READY || g_TxCNCData.shmCNCData.CNC_jog_coor!=0 || g_TxCNCData.shmCNCData.CNC_jog_status!=0)//手動/機構動作中都要禁止
			{
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
				return wIDControl;
			}
			else
			{
				u_pwndBtnServoSwitch->SetPropValueT("captionID","ROBOT_STR_SERVO_OFF");
				temp = 0;
			}
		//}
		//else if(g_TxCNCData.shmCNCData.CNC_servo_on == 0)
		//{
		//	u_pwndBtnServoSwitch->SetPropValueT("captionID","ROBOT_STR_SERVO_ON");
		//	temp = 1;
		//}
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
		u_pwndBtnServoSwitch->CreateA();
		u_pwndBtnServoSwitch->Update();
		return	wIDControl;
	}*/
	if(pwndTemp == u_pwndBtnMoveMode)
	{
		u_Move_Mode =(!u_Move_Mode);		//0與1之間做轉換
		char temp=u_Move_Mode;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_mode,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
		SetDBString("CNC_JOG_MODE",&temp,1);
		if(g_TxCNCData.shmCNCData.CNC_jog_mode == 0)
			RobotStatus = OP_JOG_MODE;
		else if(g_TxCNCData.shmCNCData.CNC_jog_mode == 1)
			RobotStatus = OP_INCJOG_MODE;
		if(u_Move_Mode == 0)
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_CONTINUE");
		}
		
		u_pwndSelectEditJog->SetPropValueT("wtype",color[u_Move_Mode]);
		u_pwndSelectEditJog->SetPropValueT("taborder", EDITJOGTAB*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
		u_pwndSelectEditJog->UpdateAll();
		if(u_Move_Mode == 1)	
		{
			((CtmFormView*)pwndSender)->Goto(EDITJOGTAB);		//吋動量下拉選單
			u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_JOG");
		}	
		u_pwndBtnMoveMode->CreateA();
		u_pwndBtnMoveMode->Update();
		
	}
	else if(pwndTemp == u_pwndBtnPlus)		//Speed↑
	{
		SetAndShowSpeed(STATE_PLUS);
	}
	else if(pwndTemp == u_pwndBtnMinus)		//Speed↓
	{
		SetAndShowSpeed(STATE_MINUS);
	}
	else if(pwndTemp == u_pwndBtnCoordinateMM)
	{
		//u_nMoveControlType = (!u_nMoveControlType); 
		u_nMoveControlType += 1;
		if(u_nMoveControlType > COORDINATETYPE_T) u_nMoveControlType = COORDINATETYPE_J;
		
		UpdateMoveControlType(u_nMoveControlType);
		SetJogSelectEditMode();
		
		if(u_nMoveControlType == COORDINATETYPE_J)
			u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_JOINT");
		else if (u_nMoveControlType == COORDINATETYPE_C)
			u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_CARTESIAN");
		else if (u_nMoveControlType == COORDINATETYPE_T)
			u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_TOOL");
		
		if (u_nMoveControlType == COORDINATETYPE_LOCK)
		{
			u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
			u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
		}
		else
			u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
		u_pwndBtnCoordinateMM->CreateA();
		u_pwndBtnCoordinateMM->Update();
	}
	return wIDControl;
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{	/*if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_NOT_READY)
	{
		MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);
		return	wIDControl;
	}		*/
	if(message==MSG_MOUSELUP)
	{
		//if(Press_Move_btn>0)
			//RestJogMoveDate();
	}
	else if(message==MSG_PANEL_KEY)
	{
		char keycode;
		keycode = wParam;
		//printf("panel key = %x\n",keycode);
		if(keycode>0x80 && Press_Move_btn ==0 && g_TxCNCData.shmCNCData.CNC_servo_on==1)
		{
			keycode = keycode-0x80;
			if(keycode>= 0x40) return ;	//避開Start鍵
			keycode = keycode-1;	//避開Pause / Stop鍵
			int Joint = keycode&0xF;
			if(Joint==0) return; 
			int N = (keycode & 0x10)/16;
			if(N==0) N=2;
			if(Press_Move_btn!=0) return;
			Press_Move_btn = Joint;
			//printf("key down , joint is %d , N = %d\n",Joint,N);
			SetJogMoveDate(u_nMoveControlType, N, Joint-1);			//修改to CNC資料
		}
		else if(keycode<0x80/* && Press_Move_btn*/)
		{
			keycode = keycode;
			if(keycode>= 0x40) return ;	//避開Start鍵
			keycode = keycode-1;	//避開Pause / Stop鍵
			int Joint = keycode&0xF;
			if(Joint==0) return; 
			int N = (keycode & 0x10)/16;
			if(N==0) N=2;
			if(Press_Move_btn!=Joint) return;
			Press_Move_btn = 0;
			//printf("key up , joint is %d , N = %d\n",Joint,N);
			RestJogMoveDate();
		}
	}	
	/*else if(message==MSG_SH_APP_TOOLBAR_ONKEY)
	{

	}*/
	else	if(message==MSG_GPIO_READ)
	{
		//printf("CtmGpio437Hmi ReadGpio=0x%x  g_TxCNCData.shmCNCData.CNC_mode_lgt=%d\n",wParam,g_TxCNCData.shmCNCData.CNC_mode_lgt);
		char temp;
		if(wParam&0x4 || (g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_JOG_MODE && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_INCJOG_MODE && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_HOME_MODE))
		{
			//printf("in up\n");
			RestJogMoveDate();
			Press_Move_btn = 0;
			temp = 0;
		}
		else if(!(wParam&0x4))
			temp = 1;
		else return;
		//printf("temp = %d\n",temp);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
	}
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL JogLock = (g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE);
	static int  old_ServoStatus=-1;
	int  ServoStatus;
	if(JogLock && (g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE))
	{
		JogLock =FALSE;
	}
	else if(!JogLock && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		JogLock =TRUE;
	}
	
	/*if(Save_Switch_time != -1)
	{
  	time(&loc_now);
  	ptrnow=localtime(&loc_now);
  	if(Save_Switch_time == ptrnow->tm_sec)
  	{
  		Save_Switch_time = -1;
  		if(u_nMoveControlType != COORDINATETYPE_LOCK)
  		{
  			u_nMoveControlType = COORDINATETYPE_LOCK;
				UpdateMoveControlType(u_nMoveControlType);
				u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
				u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
				u_pwndBtnCoordinateMM->CreateA();
				u_pwndBtnCoordinateMM->Update();	
  		}
  	}
	}
	*/
	if(!RunOnlyOne)
	{
		RunOnlyOne=TRUE;
		ini(pwndSender);	
	}
	if(g_TxCNCData.shmCNCData.CNC_jog_inc != ((CtmSelectEdit*)u_pwndSelectEditJog)->GetIndex()+1)
	{
		int temp = ((CtmSelectEdit*)u_pwndSelectEditJog)->GetIndex()+1;
		SetDBValue("CNC_JOG_INC", temp);
	}
	
	UpdateCoordTable();
	UpdateJointInfo();
	UpdateRunAndCycleTime();
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void		UpdateCoordTable()		//顯示機械手臂目前的座標位置
{
	for(int i =0; i<u_nCoordTableCnt; i++)		//卡氏座標(Cartesian)		POS另外處理
	{
		if(u_pwndCoordTable[i] != NULL)
		{
			//取得控制參數的資料ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueC_StartNo+i);
      
      //根據資料ID，取得控制參數的值
      //long	lDBvalue	=0;																							
      double	lDBvalue	=0;																							
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < 3)																									   
      {
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
				lDBvalue += 0.5*SIGN(lDBvalue);																								
				u_pwndCoordTable[i]->SetPropValueT("precision",3+Unitindex);		
			}
      u_pwndCoordTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndCoordTable[i]->Update();
		}
		if(u_pwndOriginTable[i] != NULL)
		{
			//取得控制參數的資料ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueOrigin_StartNo+i);
      
      //根據資料ID，取得控制參數的值
      double	lDBvalue	=0;
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < 3)
      {
      	SetCoordLimitFlag(i,lDBvalue);
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;
				lDBvalue += 0.5*SIGN(lDBvalue);
				u_pwndOriginTable[i]->SetPropValueT("precision",3+Unitindex);
			}
      u_pwndOriginTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndOriginTable[i]->Update();
		}
	}
}

void	SetCoordLimitFlag(int Coord,int Value)
{
	int edit_left=0,edit_right=0,edit_Len=0;
	int	Limit_Plus=0,LiMit_Minus=0,LiMit_Range=0,Now_Pos=0;
	u_pwndOriginTable[Coord]->GetPropValueT("left",&edit_left,sizeof(edit_left));
	u_pwndOriginTable[Coord]->GetPropValueT("right",&edit_right,sizeof(edit_right));
	edit_Len = edit_right-edit_left-2;
	Limit_Plus = g_TxCNCData.shmCNCData.PARAM.tune_param[53+Coord*2];
	LiMit_Minus = g_TxCNCData.shmCNCData.PARAM.tune_param[54+Coord*2];
	LiMit_Range = Limit_Plus - LiMit_Minus;
	if(LiMit_Range==0) return;
	if(LiMit_Range < 0) LiMit_Range=-LiMit_Range;	//其實不應該會有此情況  但是怕後續處理錯誤因此修正
	int offset=0;
	Now_Pos = Value-LiMit_Minus;
	if(Now_Pos<0 || Now_Pos>LiMit_Range)
		offset=edit_Len/2;
	else
		offset=edit_Len*Now_Pos/LiMit_Range;
	u_pwndCoordLimitFlag[Coord]->Clear();
	u_pwndCoordLimitFlag[Coord]->SetPropValueT("left",edit_left+offset-1);
	u_pwndCoordLimitFlag[Coord]->SetPropValueT("right",edit_left+offset+5);
	u_pwndCoordLimitFlag[Coord]->Update();
}

void		UpdateJointInfo()		//更新六軸資訊
{
	char*	pDataID 	= NULL;
	int	lDBvalue	=0;
	for(int i =0; i<u_nJointTableCnt; i++)
	{
		if(u_pwndJointTable[i] != NULL)
		{
      pDataID 	= g_pDatabase->GetString(u_wDBvalueJ_StartNo+i);
      lDBvalue	= GetDBValue(pDataID).lValue;
			u_pwndJointTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndJointTable[i]->Update();
      SetJointLimitFlag(i,lDBvalue);
		}
	}
}

void	SetJointLimitFlag(int Axis,int Value)
{
	int edit_left=0,edit_right=0,edit_Len=0;
	int	Limit_Plus=0,LiMit_Minus=0,LiMit_Range=0,Now_Pos=0;
	u_pwndJointTable[Axis]->GetPropValueT("left",&edit_left,sizeof(edit_left));
	u_pwndJointTable[Axis]->GetPropValueT("right",&edit_right,sizeof(edit_right));
	edit_Len = edit_right-edit_left-2;
	Limit_Plus = g_TxCNCData.shmCNCData.PARAM.tune_param[41+Axis*2];
	LiMit_Minus = g_TxCNCData.shmCNCData.PARAM.tune_param[42+Axis*2];
	LiMit_Range = Limit_Plus - LiMit_Minus;
	if(LiMit_Range==0) return;
	if(LiMit_Range < 0) LiMit_Range+=360000;
	Now_Pos = Value-LiMit_Minus;
	if(Now_Pos < 0) Now_Pos+=360000;
	int offset=0;
	offset=edit_Len*Now_Pos/LiMit_Range;
	if(offset > edit_Len) offset=edit_Len/2;
	u_pwndJointLimitFlag[Axis]->Clear();
	u_pwndJointLimitFlag[Axis]->SetPropValueT("left",edit_left+offset-1);
	u_pwndJointLimitFlag[Axis]->SetPropValueT("right",edit_left+offset+5);
	u_pwndJointLimitFlag[Axis]->Update();
}

void		UpdateMoveControlType(int nCoordinateType)		//更換右方移動控制區的座標模式
{
	if(nCoordinateType == COORDINATETYPE_J)		//關節座標
	{
		for(int i =0; i<u_nStaticCTableCnt; i++)
		{
			if(u_pwndStaticCTable[i] != NULL)
			{
				u_pwndStaticCTable[i]->SetPropValueT("textID", cfg.j_sid[i]);
				u_pwndStaticCTable[i]->Update();
			}
		}
	}
	else if(nCoordinateType == COORDINATETYPE_C || nCoordinateType == COORDINATETYPE_T)		//卡氏座標
	{
		for(int i =0; i<u_nStaticCTableCnt; i++)
		{
			if(u_pwndStaticCTable[i] != NULL)
			{
				u_pwndStaticCTable[i]->SetPropValueT("textID", cfg.c_sid[i]);
				u_pwndStaticCTable[i]->Update();
			}
		}
	}
	else if(nCoordinateType == COORDINATETYPE_LOCK)		//NO
	{
		for(int i =0; i<u_nStaticCTableCnt; i++)
		{
			if(u_pwndStaticCTable[i] != NULL)
			{
				u_pwndStaticCTable[i]->SetPropValueT("textID", "ROBOT_STR_DUMMY");
				u_pwndStaticCTable[i]->Update();
			}
		}
	}
}



void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum)
{
	//Press_Move_btn = TRUE;
	char temp = nCoordinateType +1;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
	if(nBtnType == STATE_PLUS)
	{
		int temp2 = nAxisNum+1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp2,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
	}
	else if(nBtnType == STATE_MINUS)
	{
		int temp2 = -nAxisNum-1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp2,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
	}	
}

void		RestJogMoveDate()
{
	char temp_CHAT = 0;
	int temp_INT = 0;
	//g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_return_home,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_return_home));
	Press_Move_btn = 0;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp_INT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
}

void		ini(CtmWnd* pwndSender)
{
/*---------------速度初始化----------------*/
	SetAndShowSpeed(STATE_INITIAL);
/*---------------連續/吋動 與 吋動量-------------*/
	u_Move_Mode = (int)g_TxCNCData.shmCNCData.CNC_jog_mode;
	
	if(u_Move_Mode == 0)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_CONTINUE");
	}
	
	u_pwndSelectEditJog->SetPropValueT("wtype",color[u_Move_Mode]);
	u_pwndSelectEditJog->SetPropValueT("taborder", EDITJOGTAB*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
	u_pwndSelectEditJog->UpdateAll();
	if(u_Move_Mode == 1)
	{
		((CtmFormView*)pwndSender)->Goto(EDITJOGTAB);		//吋動量下拉選單
		u_pwndBtnMoveMode->SetPropValueT("captionID","ROBOT_STR_MOVE_JOG");
	}	
	u_pwndBtnMoveMode->CreateA();
	u_pwndBtnMoveMode->Update();
	
	int temp = GetDBValue("CNC_JOG_INC").lValue-1;
	((CtmSelectEdit*)u_pwndSelectEditJog)->SetIndex(temp);
	u_pwndSelectEditJog->Update();
		
/*-----------------控制手臂卡式/關節----------------------*/
	if(u_nMoveControlType == COORDINATETYPE_J)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_JOINT");
	}
	else if(u_nMoveControlType == COORDINATETYPE_C)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_CARTESIAN");
	}
	else if(u_nMoveControlType == COORDINATETYPE_T)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_TOOL");
	}
	else if(u_nMoveControlType == COORDINATETYPE_LOCK)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
	}
	u_pwndBtnCoordinateMM->CreateA();
	u_pwndBtnCoordinateMM->Update();
	UpdateMoveControlType(u_nMoveControlType);
}

/*void SetBtn_JogMove(BOOL f_JogLock)
{
	if(f_JogLock)		//Jog模式上鎖
	{
		for(int JointNum = 0 ; JointNum < u_nBtnPlusCTableCnt ; JointNum++)
		{
			if(u_pwndBtnPlusCTable[JointNum] != NULL)
				u_pwndBtnPlusCTable[JointNum]->SetPropValueT("taborder",-2);
			if(u_pwndBtnMinusCTable[JointNum] != NULL)
				u_pwndBtnMinusCTable[JointNum]->SetPropValueT("taborder",-2);
		}
		u_pwndBtnCoordinateMM->SetPropValueT("taborder",-2);
		u_pwndBtnMoveMode->SetPropValueT("taborder",-2);
		u_pwndSelectEditJog->SetPropValueT("taborder",-2);
		u_pwndBtnPlus->SetPropValueT("taborder",-2);
		u_pwndBtnMinus->SetPropValueT("taborder",-2);
	}
	else					//Jog模式解鎖
	{
		for(int JointNum = 0 ; JointNum < u_nBtnPlusCTableCnt ; JointNum++)
		{
			if(u_pwndBtnPlusCTable[JointNum] != NULL)
			{
				u_pwndBtnPlusCTable[JointNum]->SetPropValueT("taborder",JointNum*2+1);
			}
			if(u_pwndBtnMinusCTable[JointNum] != NULL)
			{
				u_pwndBtnMinusCTable[JointNum]->SetPropValueT("taborder",JointNum*2+2);
			}
		}
		u_pwndBtnCoordinateMM->SetPropValueT("taborder",(double)COORDINATEMMTAB);
		u_pwndBtnMoveMode->SetPropValueT("taborder",-2);
		u_pwndSelectEditJog->SetPropValueT("taborder",EDITJOGTAB*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
		u_pwndBtnPlus->SetPropValueT("taborder",SPEEDPLUSTAB);
		u_pwndBtnMinus->SetPropValueT("taborder",SPEEDMINUSTAB);
	}
}*/

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

void		SetUnit()	//設定單位(mm or in)  20140915		by	Mario
{
	for (int i = 0 ;i < u_nStaticUnitCnt ; i++)
	{
		if(Unitindex == 0 )
			u_pwndStaticUnut[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
		else if(Unitindex == 1)
			u_pwndStaticUnut[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
		
		u_pwndStaticUnut[i]->Update();
	}
}

void	UpdateRunAndCycleTime()
{
	long  lCycleTime =g_pRxCNCData->CNC_cycle_time;//GetDBValue("CNC_CYCLE_TIME").lValue;
	long	lCycleTimeSec = (long)((lCycleTime/1000) % 60);
	long	lCycleTimeMin = (long)(lCycleTime/60000);
	long	lCycleTimeHour= (long)(lCycleTimeMin/60);
	lCycleTimeMin = (long)(lCycleTimeMin%60);
	if(u_pwndeditCYCLE_TIME_Hour != NULL)	
	{
		u_pwndeditCYCLE_TIME_Hour -> SetPropValueT("value", lCycleTimeHour);
		u_pwndeditCYCLE_TIME_Hour -> Update();
	}
	if(u_pwndeditCYCLE_TIME_Minut != NULL)	
	{
		u_pwndeditCYCLE_TIME_Minut -> SetPropValueT("value", lCycleTimeMin);
		u_pwndeditCYCLE_TIME_Minut -> Update();
	}
	if(u_pwndeditCYCLE_TIME_Second != NULL)	
	{
		u_pwndeditCYCLE_TIME_Second -> SetPropValueT("value", lCycleTimeSec);
		u_pwndeditCYCLE_TIME_Second -> Update();
	}
	
	//RunTime
	long 	lRunTime =g_pRxCNCData->CNC_run_time;//GetDBValue("CNC_RUN_TIME").lValue;
	long	lRunTimeSec = (long)((lRunTime/1000) % 60);
	long	lRunTimeMin = (long)(lRunTime/60000);
	long	lRunTimeHour= (long)(lRunTimeMin/60);
	lRunTimeMin = (long)(lRunTimeMin%60);
	if(u_pwndeditCNC_RUN_TIME_Hour != NULL)	
	{
		u_pwndeditCNC_RUN_TIME_Hour -> SetPropValueT("value", lRunTimeHour);
		u_pwndeditCNC_RUN_TIME_Hour -> Update();
	}
	if(u_pwndeditCNC_RUN_TIME_Minut != NULL)	
	{
		u_pwndeditCNC_RUN_TIME_Minut -> SetPropValueT("value", lRunTimeMin);
		u_pwndeditCNC_RUN_TIME_Minut -> Update();
	}
	if(u_pwndeditCNC_RUN_TIME_Second != NULL)	
	{
		u_pwndeditCNC_RUN_TIME_Second -> SetPropValueT("value", lRunTimeSec);
		u_pwndeditCNC_RUN_TIME_Second -> Update();
	}
}

void	SetJogSelectEditMode()
{
	static int f_mode = -1;
	if(f_mode != u_nMoveControlType &&  u_nMoveControlType != COORDINATETYPE_LOCK)
	{
		f_mode = u_nMoveControlType;
		for(int i = 0 ; i < 3 ; i++)
		{
			char strID[8];
			char textID[64];
			sprintf(strID,"strID%d",i);
			if(u_nMoveControlType == COORDINATETYPE_C || u_nMoveControlType == COORDINATETYPE_T)
				sprintf(textID,"ROBOT_STR_INC_POS_%d",i+1);
			else if(u_nMoveControlType == COORDINATETYPE_J)
				sprintf(textID,"ROBOT_STR_INC_THETA_%d",i+1);
			u_pwndSelectEditJog->SetPropValueT(strID,textID);
		}
		u_pwndSelectEditJog->Update();
		u_pwndSelectEditJog->UpdateAll();
	}
}