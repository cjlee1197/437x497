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
#include	"ViewManualMode.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h" 
#include	"../selectedit.h" 
#include	"../tmshmsg.h"
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define		STATE_INITIAL				0  
#define		STATE_PLUS					1		//按下Button"+"
#define		STATE_MINUS					2		//按下Button"-"

//JogLock
#define		COLOR_GRAY					50712	//灰色
#define		COLOR_BLUE					1046	//藍色
#define		JOGLOCK_ON_EXTAG		20002	//上鎖
#define		JOGLOCK_OFF_EXTAG		0			//開鎖
	
#define		COORDINATETYPE_LOCK	  -1		//卡氏座標
#define		COORDINATETYPE_J			0			//關節座標
#define		COORDINATETYPE_C			1			//卡氏座標
#define		COORDINATETYPE_T			2			//卡氏座標
//20140915↓		by Mario
#define		MSGBOX_TEACHPOINT		1
#define		ONE_PAGE_MAX_TEACHPOINT			12
#define		MAX_TEACHPOINT			24
#define		MAX_TEACHPOINTPAGE	2				//設置教導點最大頁數
#define		SAVE_SWITCH_DELAY		15
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )

#define	JOGMODE_N_LOCK				16
#define	JOGMODE_LOCK					63488

#define	PTP_LINE_SEL							0xFC00
#define	PTP_LINE_NO_SEL						0x0416

#define	TP_SEL1 0xEDAF
#define	TP_SEL2 0xFF79

#define	TP_NO_SEL1 0xADD9
#define	TP_NO_SEL2 0xDF5E
#define EDITJOGTAB 		19


#define		LINE_MODE				0  
#define		PTP_MODE				1	

/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
//開關
char*	SwitchStr[] =			
{
	"ROBOT_BTN_CYCLE_CLOSE",	//關			
	"ROBOT_BTN_CYCLE_OPEN",	//開
	NULL
}; 

char*	ImageRun[]=
{
	"res_tm640/run0.png",
	"res_tm640/run1.png",
	"res_tm640/run2.png",
	"res_tm640/run3.png",
	"res_tm640/run4.png",
	NULL
};

//色彩 20141008 add by Mario
int color[] ={0x0023,0x0022,NULL};


//左上當前座標位置↓-------------------------------
CtmWnd*		u_pwndEditTable[13]		= {NULL};
CtmWnd*		u_pwndLimitFlag[6]					= {NULL};

//左下教導點資訊↓---------------------------------
CtmWnd*		u_pwndStaticTPCTable[12]	= {NULL};
CtmWnd*		u_pwndNoTable[24]					= {NULL};
CtmWnd*		u_pwndTPTable[256]				= {NULL};


//左下教導點頁數控制↓-----------------------------
CtmWnd*		u_TP_Page_Up	= NULL;
CtmWnd*		u_TP_Page_Down	= NULL;

//操作模式按鈕

//操作模式1
	//右方移動控制↓-----------------------------------
	CtmWnd*		u_pwndBtnCoordinateMM	= NULL;
	CtmWnd*		u_pwndBtnPlusCTable[12]				= {NULL};		//右方移動控制區的Button"+"
	CtmWnd*		u_pwndBtnMinusCTable[12]			= {NULL};		//右方移動控制區的Button"-"
	CtmWnd*		u_pwndStaticCTable[12]				= {NULL};		//右方移動控制區的座標
	//其他控制設定↓-----------------------------------
	CtmWnd*		u_pwndBtnMoveMode							= NULL;
	CtmWnd*		u_pwndBtnMoveModeStr					= NULL;
	CtmWnd*		u_pwndSelectEditJog						= NULL;
	CtmWnd*		u_pwndSelectEditJogStr				= NULL;
	
//操作模式2
//	CtmWnd*		u_pwndOper2StaticStr[35]			= {NULL};
//	int				u_dOper2StaticStr							=0;
//	CtmWnd*		u_pwndOper2EditStr[35]			= {NULL};
//	int				u_dOper2EditStr							=0;
	CtmWnd*		u_pwndEditTPNo								= NULL;
	CtmWnd*		u_pwndCheckBoxTP							= NULL;
	CtmWnd*		u_pwndCheckBoxHome						= NULL;
	CtmWnd*		u_pwndBtnPressRun						= NULL;
	CtmWnd*		u_pwndImageBoxRun						= NULL;
	CtmWnd*		u_pwndBtnRun								= NULL;
	CtmWnd*		u_pwndBtnSelPTP							= NULL;
	CtmWnd*		u_pwndBtnSelLINE						= NULL;
	
//共用速度控制↓---------------------------------------
//CtmWnd*		u_pwndStaticSpeedStr		= NULL;
CtmWnd*		u_pwndStaticSpeedShow		= NULL;
CtmWnd*		u_pwndBtnPlus						= NULL;
CtmWnd*		u_pwndBtnMinus					= NULL;
CtmWnd*		u_pwndStaticAlarm				= NULL;

//POSTURE相關控制設定
//CtmWnd*		u_pwndStaticPosture				=NULL;
//CtmWnd*		u_pwndBtnPlusPTable			= NULL;	
//CtmWnd*		u_pwndBtnMinusPTable			= NULL;	
//20140922↑			by Mario
//------------------------------------------------
int		u_nBtnPlusCTableCnt		=0;
int		u_nBtnMinusCTableCnt	=0;
int		u_nStaticCTableCnt		=0;
int		u_nEditTableCnt				=0;
int		u_nLimitFlagCnt				=0;
int		u_nStaticTPCTableCnt	=0;			//TPC = TeachPoint Coordinate
int		u_nNoTableCnt					=0;
int		u_nTPTableCnt					=0;

BOOL		u_bEnterKey						= FALSE;

int			u_lSpeed							=0;
int			u_nMoveControlType		=COORDINATETYPE_J;
int			u_nTPTableType				=0;

WORD		u_wDBvalueJ_StartNo		=0;		//控制參數編號：關節座標的J1軸
WORD		u_wDBvalueC_StartNo		=0;		//控制參數編號：卡氏座標的X座標
WORD		u_wDBvalueHomeAngle		=0;		//控制參數編號：原點角度


//add		by		Mario
int			u_nTeachPointCnt = 0;
int			Unitindex							=0;		//表示目前單位(0=公制,1=英制)，預設0	
CtmWnd*		u_pwndTPUnit[3]		= {NULL};		//指向卡氏座標xyz的顯示單位元件以配合公英制做改變
int			u_TeachPoint_Page		=	0;				//目前教導點的頁數(目前僅2頁)預設為0(第一頁)
//90140915↑
int			u_Move_Mode					=0;			//移動模式 0=連續  1=吋動
//20140924
BOOL			RunOnlyOne				=	FALSE;	//利用update僅執行一次
//20140929
BOOL				Press_Move_btn		=FALSE;
//2015/3/2 下午 02:59:00
BOOL	Start_Lock = FALSE;
BOOL	PressRunFlag = FALSE;
int 	TPAndHomeFlag = 0;
int 	LINE_PTP_Flag = 0;
BOOL	TPAndHomeRun	=FALSE;
int		StopTPTable = 0;
BOOL 	JogMoving = FALSE;
//2015/6/11 上午 10:19:59
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//抓取各元件的指針↓
	//=====速度控制=====
//	u_pwndStaticSpeedStr	= pwndSender->FindControlFromName("StaticSpeed");
	u_pwndStaticSpeedShow	= pwndSender->FindControlFromName("StaticSpeedShow");
	u_pwndBtnPlus					= pwndSender->FindControlFromName("btnPlus");
	u_pwndBtnMinus				= pwndSender->FindControlFromName("btnMinus");	
	
	//=====右方移動控制=====
	u_pwndBtnCoordinateMM	= pwndSender->FindControlFromName("BtnCoordinateMM");
	u_nBtnPlusCTableCnt		= GetSpecialControlNum(pwndSender, "btnPlusC", "CtmToolButton", u_pwndBtnPlusCTable);
	u_nBtnMinusCTableCnt	= GetSpecialControlNum(pwndSender, "btnMinusC", "CtmToolButton", u_pwndBtnMinusCTable);
	u_nStaticCTableCnt		= GetSpecialControlNum(pwndSender, "StaticC", "CtmStaticX2", u_pwndStaticCTable);
	
	//=====左上當前座標位置=====
	u_nEditTableCnt		= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwndEditTable);
	u_nLimitFlagCnt		= GetSpecialControlNum(pwndSender, "LimitFlag", "CtmImageBox", u_pwndLimitFlag);
	
	//=====左下教導點資訊=====
	
	u_nStaticTPCTableCnt	=	GetSpecialControlNum(pwndSender, "StaticTPC", "CtmStaticX2", u_pwndStaticTPCTable);
	u_nNoTableCnt		= GetSpecialControlNum(pwndSender, "NoTABLE", "CtmEditX1", u_pwndNoTable);
	u_nTPTableCnt		= GetSpecialControlNum(pwndSender, "TPTABLE", "CtmEditX1", u_pwndTPTable);
	
	//20140919↑				by Mario
	//=====其他控制設定=====
	//u_pwndBtnCoordinateTP	= pwndSender->FindControlFromName("BtnCoordinateTP");
	u_pwndBtnMoveMode				= pwndSender->FindControlFromName("BtnMoveMode");
	u_pwndBtnMoveModeStr		= pwndSender->FindControlFromName("Mode1StaticMoveMode");
	u_pwndSelectEditJog			= pwndSender->FindControlFromName("SelectEditJog");
	u_pwndSelectEditJogStr	= pwndSender->FindControlFromName("Mode1StaticJog");
	
	u_wDBvalueJ_StartNo			= g_pDatabase->GetIndex(ANGULAR_COORD_ID);
	u_wDBvalueC_StartNo			= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	u_wDBvalueHomeAngle			= g_pDatabase->GetIndex("MAKER_PARAM7");
	
	//讀取目前單位型態 20140915 by Mario
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	//抓取卡氏座標單位元件
	u_pwndTPUnit[0]	= pwndSender->FindControlFromName("StaticUnit04");
	u_pwndTPUnit[1] = pwndSender->FindControlFromName("StaticUnit05");
	u_pwndTPUnit[2] = pwndSender->FindControlFromName("StaticUnit06");
			
		
//=====左下教導點頁數控制=====			20140915↓		by	Mario		
	u_TP_Page_Up		= pwndSender->FindControlFromName("PageUp");
	u_TP_Page_Down	= pwndSender->FindControlFromName("PageDown");
	

	//u_dOper2StaticStr		= GetSpecialControlNum(pwndSender, "Mode2Str", "CtmStaticX2", u_pwndOper2StaticStr);
	//u_dOper2EditStr			= GetSpecialControlNum(pwndSender, "Mode2HomeAngle", "CtmEditX1", u_pwndOper2EditStr);
	u_pwndEditTPNo			= pwndSender->FindControlFromName("EditTeachPointNO");
	u_pwndCheckBoxTP		= pwndSender->FindControlFromName("CheckBoxTP");
	u_pwndCheckBoxHome	= pwndSender->FindControlFromName("CheckBoxHome");
	u_pwndBtnPressRun		= pwndSender->FindControlFromName("BtnPressRun");	
	u_pwndImageBoxRun		= pwndSender->FindControlFromName("ImageBoxRuning");
	u_pwndBtnRun				= pwndSender->FindControlFromName("BtnRun");
	u_pwndBtnSelPTP			= pwndSender->FindControlFromName("ButtonSelPTP");
	u_pwndBtnSelLINE		= pwndSender->FindControlFromName("ButtonSelLine");
	
	//u_pwndStaticAlarm		= pwndSender->FindControlFromName("StaticAlarm");
	
	//設定單位	
	if(GetTeachPointCnt()>0) 
	{
		StopTPTable=1;
	}
	u_pwndEditTPNo->SetPropValueT("value",StopTPTable);
	u_pwndEditTPNo->Update();
	SetUnit(Unitindex);
	
	
	if(g_TxCNCData.shmCNCData.CNC_jog_mode == 0)
		RobotStatus = OP_JOG_MODE;
	else if(g_TxCNCData.shmCNCData.CNC_jog_mode == 1)
		RobotStatus = OP_INCJOG_MODE;
		
	u_nTeachPointCnt = GetTeachPointCnt();
	return	TRUE;
}  

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndSelectEditJog)
	{
		int temp = ((CtmSelectEdit*)u_pwndSelectEditJog)->GetIndex()+1;
		SetDBValue("CNC_JOG_INC", temp);
	}
	return	wIDControl;
}

WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	Start_Lock = FALSE;
	
		if(pwndTemp == u_pwndBtnRun && g_TxCNCData.shmCNCData.CNC_servo_on == 1)
		{
			if(TPAndHomeRun)
				SetTPAndHomeRun(FALSE);
			else if(Press_Move_btn==0)
			{
				if(TPAndHomeFlag == 0)//Home
				{
					if(g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
					{
						int InputTPNum=0;
						g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_goto_tp_no,&InputTPNum,sizeof(g_TxCNCData.shmCNCData.CNC_goto_tp_no));
						SetTPAndHomeRun(TRUE);
						Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex(" ")],0);
					}
					else
						MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
				}
				else if(TPAndHomeFlag == 1)//TP
				{
					if(StopTPTable > u_nTeachPointCnt || StopTPTable < 1)
						SetAlarmStatic(1);
					else
					{
						if(g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
						{
							int temp = LINE_PTP_Flag*10000+StopTPTable;
							SetAlarmStatic(0);
							g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_goto_tp_no,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_goto_tp_no));
							SetTPAndHomeRun(TRUE);
							Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex(" ")],0);
						}							
						else
							MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
					}
				}							
			}
			//else
			//	MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		}
	return wIDControl;							
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{							
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	
	if(wKey == _KEY_TEXTINPUT && u_pwndCheckBoxHome == pwndTemp)
	{
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	0;
		}	
		if(TPAndHomeFlag != 0)
		{
			TPAndHomeFlag=0;
			((CtmFormView*)pwndSender)->OnKey1(wKey);
			u_pwndCheckBoxTP->SetPropValueT("checked", (double)FALSE);	
			u_pwndCheckBoxTP->Update();
			return wKey; 
		}
		return 0; 
	}
	else if(wKey == _KEY_TEXTINPUT && u_pwndCheckBoxTP == pwndTemp)
	{
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	0;
		}
		if(TPAndHomeFlag != 1)
		{
			TPAndHomeFlag=1;
			((CtmFormView*)pwndSender)->OnKey1(wKey);
			u_pwndCheckBoxHome->SetPropValueT("checked", (double)FALSE);	
			u_pwndCheckBoxHome->Update(); 
			return wKey; 
		}
		return 0;
	}
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp ==  u_TP_Page_Down)		//page+
	{
		if(MAX_TEACHPOINTPAGE > u_TeachPoint_Page && (u_nTeachPointCnt - (u_TeachPoint_Page + 1) *12 ) > 0)
		{
			//printf("UP\n");
			u_TeachPoint_Page+=1;
			UpdateTeachPointTable();
		}
		return wIDControl;
	}
	else if(pwndTemp == u_TP_Page_Up)		//page-
	{
		if( 0 < u_TeachPoint_Page)
		{
			u_TeachPoint_Page-=1;
			UpdateTeachPointTable();
		}
		return wIDControl;
	}
	else if(pwndTemp == u_pwndBtnPlus)		//Speed↑
	{
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_NOT_READY)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		SetAndShowSpeed(STATE_PLUS);
	}
	else if(pwndTemp == u_pwndBtnMinus)		//Speed↓
	{
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_NOT_READY)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_MACHINE_NOT_READY"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		SetAndShowSpeed(STATE_MINUS);
	}
	else if(pwndTemp == u_pwndBtnMoveMode)
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
	else if(pwndTemp == u_pwndBtnCoordinateMM)
	{
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		SwitchJogMode();
		SetJogSelectEditMode();
	}
	else if(pwndTemp == u_pwndBtnPressRun)
	{
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
		{
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			return	wIDControl;
		}
		PressRunFlag=!PressRunFlag;
		u_pwndBtnPressRun->SetPropValueT("captionID",SwitchStr[PressRunFlag*1]);
		u_pwndBtnPressRun->CreateA();
		u_pwndBtnPressRun->Update();
		if(!PressRunFlag && TPAndHomeRun)
		{
			SetTPAndHomeRun(FALSE);
		}
	}
	else if(pwndTemp == u_pwndBtnSelLINE)
	{
		LINE_PTP_Flag=LINE_MODE;
		u_pwndBtnSelLINE->SetPropValueT("bgc",PTP_LINE_SEL);
		u_pwndBtnSelLINE->Update();
		u_pwndBtnSelPTP->SetPropValueT("bgc",PTP_LINE_NO_SEL);
		u_pwndBtnSelPTP->Update();
	}
	else if(pwndTemp == u_pwndBtnSelPTP)
	{
		LINE_PTP_Flag=PTP_MODE;
		u_pwndBtnSelPTP->SetPropValueT("bgc",PTP_LINE_SEL);
		u_pwndBtnSelPTP->Update();
		u_pwndBtnSelLINE->SetPropValueT("bgc",PTP_LINE_NO_SEL);
		u_pwndBtnSelLINE->Update();
	}
	

	return wIDControl;
}


void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY && g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
	{
		if(wParam == _F1)	
		{	
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();					//20140930 add by Mario
			//((CtmFormView*)pwndSender)->OnLoseFocus();		
		}
		/*else if(wParam == _F5 )		//F5，夾爪1控制
		{
			int tmp = g_TxCNCData.shmCNCData.CNC_jaws_control;
			if(tmp%2 == 1)
				tmp-=1;
			else
				tmp+=1;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
			system("sleep 0.2");						//20140930 add by Mario
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();					//20140930 add by Mario
			//printf("g_TxCNCData.shmCNCData.CNC_jaws_control = %d\n",g_TxCNCData.shmCNCData.CNC_jaws_control);
		}
		else if(wParam == _F6 )		//F6，夾爪2控制
		{
			int tmp = g_TxCNCData.shmCNCData.CNC_jaws_control;
			if(tmp%4 >= 2)
				tmp-=2;
			else
				tmp+=2;
			
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
			system("sleep 0.2");						//20140930 add by Mario
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();					//20140930 add by Mario
			//printf("g_TxCNCData.shmCNCData.CNC_jaws_control = %d\n",g_TxCNCData.shmCNCData.CNC_jaws_control);
		}*/
		else if(wParam == _F5 )		//F7，教點設置
		{
			if(AddTeachPoint(g_pRxCNCData->CNC_absolute,g_pRxCNCData->CNC_posture))
			{
				if(StopTPTable==0) 
				{
					StopTPTable=1;
					u_pwndEditTPNo->SetPropValueT("value",StopTPTable);
					u_pwndEditTPNo->Update();
				}
				if(u_nTeachPointCnt < 12)
					u_TeachPoint_Page=0;
				else if(u_nTeachPointCnt < 24) 
					u_TeachPoint_Page=1;
				UpdateTeachPointTable();
				usleep(1000*100);
				Etnernet_File("./Data/TP/TeachPoint");
				//SetSelTPColor();//因為enable還沒有true，所以UpdateTeachPointTable()裡的SetSelTPColor會執行失敗，故額外再使用一次
				//TeachPointUpdate(TP_ADD,GetTeachPointCnt(),&g_pRxCNCData->CNC_absolute,g_pRxCNCData->CNC_posture);
			}
			else
			{
				MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_MAX_TEACHPOINT"], tmFT_CODE_TECH);
				//((CtmFormView*)pwndSender)->OnLoseFocus();
			}
			system("sleep 0.2");						//20140930 add by Mario
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();					//20140930 add by Mario
		}
		else if(wParam == _F6)		//F8，教點刪除
		{
			//g_nTeachPointType = 1;
			if(u_nTeachPointCnt <=0 )
			{
				MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NON_TEACHPOINT"], tmFT_CODE_TECH);
				//((CtmFormView*)pwndSender)->OnLoseFocus();
			}
			else
			{
				if((StopTPTable-1)/12 != u_TeachPoint_Page)
				{
					u_TeachPoint_Page = (StopTPTable-1)/12;
					UpdateTeachPointTable();
				}
				MsgBox(g_MultiLanguage["ROBOT_STR_TP_DELETE_CONFIRM"], tmFT_CODE_TECH);
				if(!g_bMsgBoxFlag) return;
				g_bMsgBoxFlag = FALSE;
				if(DeleteTeachPoint(StopTPTable))
				{
					if(StopTPTable > GetTeachPointCnt())	
					{
						StopTPTable=GetTeachPointCnt();
						u_pwndEditTPNo->SetPropValueT("value",StopTPTable);
						u_pwndEditTPNo->Update();
					}
					UpdateTeachPointTable();
					usleep(1000*100);
					Etnernet_File("./Data/TP/TeachPoint");
					//TeachPointUpdate(TP_DELETE,g_wTeachPointNo,NULL,0);
				}
				else
				{
					MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_TP_SEL"], tmFT_CODE_TECH);
					//((CtmFormView*)pwndSender)->OnLoseFocus();
				}
			}
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();					//20140930 add by Mario
		}
		else if(wParam == _F7)		//F9，教點編輯
		{
			if(u_nTeachPointCnt <=0 )
			{
				MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NON_TEACHPOINT"], tmFT_CODE_TECH);
				//((CtmFormView*)pwndSender)->OnLoseFocus();
			}
			else
			{
				g_wTeachPointNo = StopTPTable;
				if(g_wTeachPointNo > 0 && g_wTeachPointNo <= GetTeachPointCnt())
				{
					MsgBoxCall("msgboxEditTeachPoint.txt");
				}
				else
				{
					MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_TP_SEL"], tmFT_CODE_TECH);
				}
				UpdateTeachPointTable();
				usleep(1000*100);
				Etnernet_File("./Data/TP/TeachPoint");
			}
			system("sleep 0.2");						//20140930 add by Mario
			Now_Push_Btn->CreateA();				//20140930 add by Mario
			Now_Push_Btn->UpdateAll();			//20140930 add by Mario
			Now_Push_Btn->Update();					//20140930 add by Mario
		}
		else if(wParam == _F10)	
		{	
			SetTPAndHomeRun(FALSE);
			system("sleep 0.2");	
			Now_Push_Btn->CreateA();			
			Now_Push_Btn->UpdateAll();		
			Now_Push_Btn->Update();				
		}
	}
	else if(message==MSG_SH_APP_TOOLBAR_ONKEY && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		if(wParam != _F10)
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		system("sleep 0.2");						//20140930 add by Mario
		Now_Push_Btn->CreateA();				//20140930 add by Mario
		Now_Push_Btn->UpdateAll();			//20140930 add by Mario
		Now_Push_Btn->Update();					//20140930 add by Mario
	}
	else if(message==MSG_MOUSELUP)
	{
		if(!PressRunFlag && TPAndHomeRun)
			SetTPAndHomeRun(FALSE);
	}
	else if(message==MSG_SH_APP_FORMVIEW)
	{
		int temp_StopTPTable=(int)wParam+u_TeachPoint_Page*12;
		if(temp_StopTPTable <= GetTeachPointCnt() && temp_StopTPTable>0) StopTPTable=temp_StopTPTable; 
		u_pwndEditTPNo->SetPropValueT("value",StopTPTable);
		u_pwndEditTPNo->Update();
		SetSelTPColor();
	}
	else if(message==MSG_PANEL_KEY)
	{
		char keycode;
		keycode = wParam;
		//printf("panel key = %x\n",keycode);
		if(keycode>0x80 && Press_Move_btn ==0 && g_TxCNCData.shmCNCData.CNC_servo_on == 1)
		{
			if(TPAndHomeRun) return;
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
	else	if(message==MSG_GPIO_READ)
	{
		//printf("CtmGpio437Hmi ReadGpio=%x\n",wParam);
		char temp;
		if(wParam&0x4 || (g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_JOG_MODE && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_INCJOG_MODE && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_HOME_MODE))
		{
			RestJogMoveDate();
			Press_Move_btn = 0;
			temp = 0;
		}
		else if(!(wParam&0x4)/* && g_TxCNCData.shmCNCData.CNC_servo_on==0*/)
			temp = 1;
		else return;
		//printf("temp = %d\n",temp);
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
	}
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static int u_time			= 0;
	static int RunImageNO = 0;
	static BOOL JogLock = (g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE);
	if(g_pRxCNCData->CNC_inform_stop == 1 && (g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_JOG_MODE || g_TxCNCData.shmCNCData.CNC_mode_lgt ==OP_HOME_MODE))
	{
		SetTPAndHomeRun(FALSE);
	}
	
	if(JogLock && (g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE))
	{
		JogLock =FALSE;
		SetBtn_JogMove(JogLock);
	}
	else if(!JogLock && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		JogLock =TRUE;
		SetBtn_JogMove(JogLock);
	} 
	if(!RunOnlyOne)
	{
		RunOnlyOne = TRUE;
		SetBtn_JogMove(JogLock);
		ini(pwndSender);
	
		LINE_PTP_Flag=LINE_MODE;
		u_pwndBtnSelLINE->SetPropValueT("bgc",PTP_LINE_SEL);
		u_pwndBtnSelLINE->Update();
		
		((CtmFormView*)pwndSender)->Goto(25);
		((CtmFormView*)pwndSender)->OnLoseFocus();
	}
	ShowCurrentCoordinate();
	
	SetCycleBtnImage();
	
	if(!TPAndHomeRun && RunImageNO != 0)
	{
		RunImageNO = 0;
		u_pwndImageBoxRun->SetPropValueT("imagepath",ImageRun[RunImageNO]);
			u_pwndImageBoxRun->Show();
	}
	else if(TPAndHomeRun)
	{
		if(RunImageNO == 0) 
		{
			u_time = 0;
			RunImageNO = 1;
			u_pwndImageBoxRun->SetPropValueT("imagepath",ImageRun[RunImageNO]);
			u_pwndImageBoxRun->Show();
		}
		else 
		{
			u_time +=1;
			if(u_time == 20)
			{
				RunImageNO+=1;
				if(RunImageNO > 4) RunImageNO = 1;
				u_pwndImageBoxRun->SetPropValueT("imagepath",ImageRun[RunImageNO]);
				u_pwndImageBoxRun->Show();
				u_time = 0;
			}
		}	
	}	
	
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	RobotStatus = OP_EDIT_MODE;
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/

void		ShowCurrentCoordinate()		//顯示機械手臂目前的座標位置
{
	for(int i =0; i<cfg.jn; i++)		//關節座標(Joint)
	{
		if(u_pwndEditTable[i] != NULL)
		{
			//取得控制參數的資料ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueJ_StartNo+i);
      //printf("pDataID :  %s %d %d\n", pDataID, u_wDBvalueJ_StartNo, i);
      
      //根據資料ID，取得控制參數的值
      long	lDBvalue	=0;
      lDBvalue	= GetDBValue(pDataID).lValue;

      u_pwndEditTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndEditTable[i]->Update();
			SetLimitFlag(i,(int)lDBvalue);
		}
	}
	for(int i =cfg.jn; i<cfg.cn+cfg.jn; i++)		//卡氏座標(Cartesian)
	{
		if(u_pwndEditTable[i] != NULL)
		{
			//取得控制參數的資料ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueC_StartNo+i-cfg.jn);
      //printf("pDataID :  %s %d %d\n", pDataID, u_wDBvalueC_StartNo, i);
      
      //根據資料ID，取得控制參數的值
      //long	lDBvalue	=0;																						
      double	lDBvalue	=0;																						
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < cfg.jn+3)																									    	
      {																																
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
				lDBvalue += 0.5*SIGN(lDBvalue);																								
				u_pwndEditTable[i]->SetPropValueT("precision",3+Unitindex);		
			}																																
      u_pwndEditTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndEditTable[i]->Update();
		}
	}
}

void	SetLimitFlag(int Axis,int Value)
{
	int edit_left=0,edit_right=0,edit_Len=0;
	int	Limit_Plus=0,LiMit_Minus=0,LiMit_Range=0,Now_Pos=0;
	u_pwndEditTable[Axis]->GetPropValueT("left",&edit_left,sizeof(edit_left));
	u_pwndEditTable[Axis]->GetPropValueT("right",&edit_right,sizeof(edit_right));
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
	u_pwndLimitFlag[Axis]->Clear();
	u_pwndLimitFlag[Axis]->SetPropValueT("left",edit_left+offset-1);
	u_pwndLimitFlag[Axis]->SetPropValueT("right",edit_left+offset+5);
	u_pwndLimitFlag[Axis]->Update();
	//printf("Axis is %d\nLeft is %d ,right is %d\nLimit is %d add to %d\n",Axis+1,edit_left,edit_right,LiMit_Minus,Limit_Plus);
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
		SetDBValue("CNC_JOG_OVER_CMD", u_lSpeed);
		//g_TxCNCData.shmCNCData.CNC_jog_over_cmd = u_lSpeed;
		sprintf(temp, "%d%s", u_lSpeed, "%");
		CodeChange(szTemp, temp);
		u_pwndStaticSpeedShow->SetPropValueT("text", szTemp);
		u_pwndStaticSpeedShow->Update();
	}
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
	else if(nCoordinateType == COORDINATETYPE_C)		//卡氏座標
	{
		for(int i =0; i<u_nStaticCTableCnt; i++)
		{
			if(u_pwndStaticCTable[i] != NULL)
			{
				u_pwndStaticCTable[i]->SetPropValueT("textID",cfg.c_sid[i]);
				u_pwndStaticCTable[i]->Update();
			}
		}
	}
	else if(nCoordinateType == COORDINATETYPE_LOCK)		//無
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
	else if(nCoordinateType == COORDINATETYPE_T)		//卡氏座標
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
}

void		UpdateTeachPointTable()		//顯示教導點表格
{
	SetSelTPColor();
	u_nTeachPointCnt = GetTeachPointCnt();
	for(int i = 0 ; i < 12 ; i++)
	{
		LONG_AXIS TPValue;
		int TPPos=0;
		int TPNum;
		TPNum = i + 1 + u_TeachPoint_Page*12;
		if(GetTechPointValue(&TPValue,&TPPos,TPNum))
		{
			u_pwndNoTable[i]->SetPropValueT("visible", (double)TRUE);
			u_pwndNoTable[i]->SetPropValueT("value",TPNum);
			u_pwndNoTable[i]->Update();
			
			int	tableNum = 0;
			for(int j = 0 ; j < cfg.cn ; j++)
			{
				tableNum = i*cfg.cn+j;
				if(u_pwndTPTable[tableNum] != NULL)
				{
					long	CoordValue=0;
					memcpy(&CoordValue,(&TPValue.x)+1*j,sizeof(CoordValue));
					u_pwndTPTable[tableNum]->SetPropValueT("visible", (double)TRUE);
					double lDBvalue = CoordValue;
					if(j < 3)
					{
						lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
						lDBvalue += 0.5*SIGN(lDBvalue);	
					}
					u_pwndTPTable[tableNum]->SetPropValueT("value",lDBvalue);
					u_pwndTPTable[tableNum]->Update();
				}
			}
		}
		else
			break;
	}	
	NonTeachPoint(u_nTeachPointCnt);
}



void		NonTeachPoint(int nStartNo)		//將教導點表格沒使用到的部分隱藏
{
	for(int i =nStartNo - u_TeachPoint_Page * 12 ; i<u_nNoTableCnt  ; i++)
	{
		if(i < 0)
			continue;
		u_pwndNoTable[i]->SetPropValueT("visible", (double)FALSE);
		u_pwndNoTable[i]->Update();
	}
	
	for(int i =(nStartNo - u_TeachPoint_Page * 12)*cfg.cn; i<u_nTPTableCnt; i++)
	{
		if(i < 0)
		{
			i += cfg.cn; 
			continue;
		}
		u_pwndTPTable[i]->SetPropValueT("visible", (double)FALSE);
		u_pwndTPTable[i]->Update();
	}
}

void		SetUnit(int UnitType)	//設定單位(mm or in)  20140915		by	Mario
{
	printf("cfg.cn = %d\n",cfg.cn);
	int temp = 0;
	if(UnitType == 0)
	{
		while(temp < 3)
		{
			u_pwndTPUnit[temp]->SetPropValueT("textID","ROBOT_UNIT_MM");
			temp+=1;
		}
		for(int i = 0 ; i < ONE_PAGE_MAX_TEACHPOINT ; i ++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				//printf("i*(cfg.cn)+j = %d\n",i*(cfg.cn)+j);
				u_pwndTPTable[i*(cfg.cn)+j]->SetPropValueT("precision",3);
				u_pwndTPTable[i*(cfg.cn)+j]->Update();
			}
		}
	}
	else
	{
		while(temp < 3)
		{
			u_pwndTPUnit[temp]->SetPropValueT("textID","ROBOT_UNIT_INCH");
			temp+=1;
		}
		for(int i = 0 ; i < ONE_PAGE_MAX_TEACHPOINT ; i ++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				u_pwndTPTable[i*(cfg.cn)+j]->SetPropValueT("precision",4);
				u_pwndTPTable[i*(cfg.cn)+j]->Update();
			}
		}
	}
}


void 		SwitchJogMode()
{
	//u_nMoveControlType = (!u_nMoveControlType);		//0與1之間做轉換
	u_nMoveControlType += 1;
	if(u_nMoveControlType > 2)  u_nMoveControlType = 0;
	
	UpdateMoveControlType(u_nMoveControlType);
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
	else if (u_nMoveControlType == COORDINATETYPE_LOCK)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
	}
	else if (u_nMoveControlType == COORDINATETYPE_T)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_TOOL");
	}
	u_pwndBtnCoordinateMM->CreateA();
	u_pwndBtnCoordinateMM->Update();
}


void		SetJogMoveDate(int nCoordinateType, int nBtnType, int nAxisNum)
{
	if(nCoordinateType == COORDINATETYPE_LOCK)
	return ;
	char temp = nCoordinateType+1;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
	if(nBtnType == STATE_PLUS)
	{
		int temp = nAxisNum+1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
	}
	else if(nBtnType == STATE_MINUS)
	{
		int temp = -nAxisNum-1;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
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
	else if(u_nMoveControlType == COORDINATETYPE_LOCK)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_SELECT_MODE");
	}
	else if(u_nMoveControlType == COORDINATETYPE_T)
	{
		u_pwndBtnCoordinateMM->SetPropValueT("bgc",JOGMODE_N_LOCK);
		u_pwndBtnCoordinateMM->SetPropValueT("captionID","ROBOT_STR_COORDINATE_TOOL");
	}

	u_pwndBtnCoordinateMM->CreateA();
	u_pwndBtnCoordinateMM->Update();
	UpdateMoveControlType(u_nMoveControlType);
/*----------------教導點顯示卡式/關節---------------------*/
	UpdateTeachPointTable();
/*--------Check Box----------*/
	u_pwndCheckBoxHome->SetPropValueT("checked", (double)TRUE);	
	u_pwndCheckBoxHome->Update(); 
	u_pwndCheckBoxTP->SetPropValueT("checked", (double)FALSE);			
	u_pwndCheckBoxTP->Update(); 
}

void SetBtn_JogMove(BOOL f_JogLock)
{
	if(f_JogLock)		//Jog模式上鎖
	{
	  u_pwndBtnRun->SetPropValueT("upbitmap","res_tm640/RUNUP_N.png");
		u_pwndBtnRun->CreateA();
		u_pwndBtnRun->Update();
	}
	else					//Jog模式解鎖
	{
		u_pwndBtnRun->SetPropValueT("upbitmap","res_tm640/RUNUP.png");
			u_pwndBtnRun->CreateA();
			u_pwndBtnRun->Update();
	}
	
}

/*void TeachPointUpdate(int f_OprMode ,int f_TeachPointNo ,LONG_AXIS * f_TeachPointVlaue ,int f_TeachPointPos)	//沒用到就填0或NULL
{
	int temp = f_OprMode;//TP_FULL_LOAD;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_tp_opr_mode,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_tp_opr_mode));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_tp_coord_sys,&g_TxCNCData.shmCNCData.CNC_work_coord_no,sizeof(g_TxCNCData.shmCNCData.CNC_tp_coord_sys));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_tp_num,&f_TeachPointNo,sizeof(g_TxCNCData.shmCNCData.CNC_tp_num));
	if(f_TeachPointVlaue != NULL)
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_tp_coord,f_TeachPointVlaue,sizeof(g_TxCNCData.shmCNCData.CNC_tp_coord));
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_tp_posture,&f_TeachPointPos,sizeof(g_TxCNCData.shmCNCData.CNC_tp_posture));			
}*/

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
			if(u_nMoveControlType == COORDINATETYPE_C || u_nMoveControlType ==COORDINATETYPE_T)
				sprintf(textID,"ROBOT_STR_INC_POS_%d",i+1);
			else if(u_nMoveControlType == COORDINATETYPE_J)
				sprintf(textID,"ROBOT_STR_INC_THETA_%d",i+1);
			u_pwndSelectEditJog->SetPropValueT(strID,textID);
		}
		u_pwndSelectEditJog->Update();
		
		u_pwndSelectEditJog->UpdateAll();
	}
}


void	SetTPAndHomeRun(BOOL	RunStatus)
{
	TPAndHomeRun = RunStatus;
	g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_status,&RunStatus,sizeof(g_TxCNCData.shmCNCData.CNC_jog_status));
}

void	SetCycleBtnImage() 
{
	static BOOL	old_RunStatus = FALSE;
	if(old_RunStatus == TPAndHomeRun ) return;
	old_RunStatus = TPAndHomeRun;
	if(!old_RunStatus)
	{
		((CtmButton*)u_pwndBtnRun)->Press(tmBUTTON_UP);
	}
	else
	{
		((CtmButton*)u_pwndBtnRun)->Press(tmBUTTON_DOWN);
	}
}

void	SetAlarmStatic(int mode)
{
	/*if(mode == 0)
		u_pwndStaticAlarm->SetPropValueT("textID","ROBOT_STR_DUMMY");
	else if(mode == 1)
		u_pwndStaticAlarm->SetPropValueT("textID","ROBOT_MSG_NULL_TEACHPOINT");
		
	u_pwndStaticAlarm->Update();*/
	
}

void	SetSelTPColor()
{
	//printf("StopTPTable = %d\n",StopTPTable);
	static int StopTPTable_old=0;
	if((StopTPTable_old != 0) && ((StopTPTable_old<=u_TeachPoint_Page*12 || StopTPTable_old>(u_TeachPoint_Page+1)*12) || StopTPTable_old!=StopTPTable))
	{
		int temp_TPNP = StopTPTable_old;
		while(temp_TPNP > 12) temp_TPNP-=12;
		u_pwndNoTable[temp_TPNP-1]->SetPropValueT("bgc",TP_NO_SEL1);
		u_pwndNoTable[temp_TPNP-1]->SetPropValueT("destcolor",TP_NO_SEL2);
		u_pwndNoTable[temp_TPNP-1]->Update();
		//u_pwndNoTable[temp_TPNP-1]->CreateA();
		for(int i=0 ; i<cfg.cn ; i++)
		{
			u_pwndTPTable[(temp_TPNP-1)*(cfg.cn)+i]->SetPropValueT("bgc",TP_NO_SEL1);
			u_pwndTPTable[(temp_TPNP-1)*(cfg.cn)+i]->SetPropValueT("destcolor",TP_NO_SEL2);
			u_pwndTPTable[(temp_TPNP-1)*(cfg.cn)+i]->Update();
			//u_pwndTPTable[(temp_TPNP-1)*7+i]->CreateA();
			
		}
		StopTPTable_old=0;
	}
	
	if(StopTPTable == 0) return;
	if(StopTPTable>u_TeachPoint_Page*12 && StopTPTable<=(u_TeachPoint_Page+1)*12)
	{
		int temp_TPNP = StopTPTable;
		while(temp_TPNP > 12) temp_TPNP-=12;
		u_pwndNoTable[temp_TPNP-1]->SetPropValueT("visible",(double)TRUE);
		u_pwndNoTable[temp_TPNP-1]->SetPropValueT("bgc",TP_SEL1);
		u_pwndNoTable[temp_TPNP-1]->SetPropValueT("bgc",TP_SEL1);
		u_pwndNoTable[temp_TPNP-1]->SetPropValueT("destcolor",TP_SEL2);
		u_pwndNoTable[temp_TPNP-1]->Update();
		u_pwndNoTable[temp_TPNP-1]->CreateA();
		for(int i=0 ; i<cfg.cn ; i++)
		{
			u_pwndTPTable[(temp_TPNP-1)*cfg.cn+i]->SetPropValueT("visible",(double)TRUE);
			u_pwndTPTable[(temp_TPNP-1)*cfg.cn+i]->SetPropValueT("bgc",TP_SEL1);
			u_pwndTPTable[(temp_TPNP-1)*cfg.cn+i]->SetPropValueT("destcolor",TP_SEL2);
			u_pwndTPTable[(temp_TPNP-1)*cfg.cn+i]->Update();
			u_pwndTPTable[(temp_TPNP-1)*cfg.cn+i]->CreateA();
		}
		StopTPTable_old=StopTPTable;
	}
}