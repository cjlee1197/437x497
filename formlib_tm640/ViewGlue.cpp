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
#include	"ViewGlue.h"
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

#define SIGN(A)  ( (A) >= 0 ? 1: -1 )

#define	JOGMODE_N_LOCK				16
#define	JOGMODE_LOCK					63488

#define	PTP_LINE_SEL							0xFC00
#define	PTP_LINE_NO_SEL						0x0416

#define	TP_SEL1 0xEDAF
#define	TP_SEL2 0xFF79

#define	TP_NO_SEL1 0xADD9
#define	TP_NO_SEL2 0xDF5E


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


//色彩 20141008 add by Mario
int color[] ={0x0023,0x0022,NULL};


//左上當前座標位置↓-------------------------------
CtmWnd*		u_pwndEditTable[13]		= {NULL};
//CtmWnd*		u_pwndLimitFlag[6]					= {NULL};


//左下教導點頁數控制↓-----------------------------
CtmWnd*		u_TP_Page_Up	= NULL;
CtmWnd*		u_TP_Page_Down	= NULL;

//操作模式按鈕

//操作模式1
	//右方移動控制↓-----------------------------------
	CtmWnd*		u_pwndBtnCoordinateMM	= NULL;
	CtmWnd*		u_pwndStaticCTable[12]				= {NULL};		//右方移動控制區的座標
	//其他控制設定↓-----------------------------------
	CtmWnd*		u_pwndBtnMoveMode							= NULL;
	CtmWnd*		u_pwndBtnMoveModeStr					= NULL;
	CtmWnd*		u_pwndSelectEditJog						= NULL;
	
//共用速度控制↓---------------------------------------
//CtmWnd*		u_pwndStaticSpeedStr		= NULL;
CtmWnd*		u_pwndStaticSpeedShow		= NULL;
CtmWnd*		u_pwndBtnPlus						= NULL;
CtmWnd*		u_pwndBtnMinus					= NULL;


CtmWnd*		EditFileName		=NULL;
CtmWnd*		TextMemoPos		=NULL;
CtmWnd*		BtnTop		=NULL;
CtmWnd*		BtnBottom		=NULL;
CtmWnd*		BtnMoveUp		=NULL;
CtmWnd*		BtnMoveDown		=NULL;
CtmWnd*		BtnDeletePos		=NULL;
CtmWnd*		BtnInsterPos		=NULL;
CtmWnd*		BtnCreateLog		=NULL;
CtmWnd*		BtnRevertLog		=NULL;
CtmWnd*		BtnCreateProg		=NULL;
//20140922↑			by Mario
//------------------------------------------------
int		u_nStaticCTableCnt		=0;
int		u_nEditTableCnt				=0;
int		u_nLimitFlagCnt				=0;

BOOL		u_bEnterKey						= FALSE;

int			u_lSpeed							=0;
int			u_nMoveControlType		=COORDINATETYPE_J;

WORD		u_wDBvalueJ_StartNo		=0;		//控制參數編號：關節座標的J1軸
WORD		u_wDBvalueC_StartNo		=0;		//控制參數編號：卡氏座標的X座標


//add		by		Mario
int			Unitindex							=0;		//表示目前單位(0=公制,1=英制)，預設0	
CtmWnd*		u_pwndTPUnit[3]		= {NULL};		//指向卡氏座標xyz的顯示單位元件以配合公英制做改變
//90140915↑
int			u_Move_Mode					=0;			//移動模式 0=連續  1=吋動
//20140924
BOOL			RunOnlyOne				=	FALSE;	//利用update僅執行一次
//20140929
BOOL				Press_Move_btn		=FALSE;
int sel_line=1;

	PATHDATA_T sPathdata_t;
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
	u_nStaticCTableCnt		= GetSpecialControlNum(pwndSender, "StaticC", "CtmStaticX2", u_pwndStaticCTable);
	
	//=====左上當前座標位置=====
	u_nEditTableCnt		= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwndEditTable);
	//u_nLimitFlagCnt		= GetSpecialControlNum(pwndSender, "LimitFlag", "CtmImageBox", u_pwndLimitFlag);
	
	//20140919↑				by Mario
	//=====其他控制設定=====
	//u_pwndBtnCoordinateTP	= pwndSender->FindControlFromName("BtnCoordinateTP");
	u_pwndBtnMoveMode				= pwndSender->FindControlFromName("BtnMoveMode");
	u_pwndBtnMoveModeStr		= pwndSender->FindControlFromName("Mode1StaticMoveMode");
	u_pwndSelectEditJog			= pwndSender->FindControlFromName("SelectEditJog");
	
	
	
	
	EditFileName		= pwndSender->FindControlFromName("EditFileName");
	TextMemoPos			= pwndSender->FindControlFromName("LTextMemoProg");
	BtnTop					= pwndSender->FindControlFromName("BtnLineUp");
	BtnBottom				= pwndSender->FindControlFromName("BtnLineDown");
	BtnMoveUp				= pwndSender->FindControlFromName("BtnCommandMoveUp");
	BtnMoveDown			= pwndSender->FindControlFromName("BtnCommandMoveDown");
	BtnDeletePos		= pwndSender->FindControlFromName("BtnCommandDelete");
	BtnInsterPos		= pwndSender->FindControlFromName("BtnCommandInsert");
	BtnCreateLog		= pwndSender->FindControlFromName("BtnCreateLog");
	BtnRevertLog		= pwndSender->FindControlFromName("BtnReverLog");
	BtnCreateProg		= pwndSender->FindControlFromName("BtnOutputGenerator");
	
	
	
	//讀取目前單位型態 20140915 by Mario
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	//抓取卡氏座標單位元件
	u_pwndTPUnit[0]	= pwndSender->FindControlFromName("StaticUnit04");
	u_pwndTPUnit[1] = pwndSender->FindControlFromName("StaticUnit05");
	u_pwndTPUnit[2] = pwndSender->FindControlFromName("StaticUnit06");
		
	u_wDBvalueJ_StartNo			= g_pDatabase->GetIndex(ANGULAR_COORD_ID);
	u_wDBvalueC_StartNo			= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	
	SetUnit(Unitindex);
	

	RobotStatus = OP_JOG_MODE;
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


WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{							
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_pwndBtnPlus)		//Speed↑
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
		u_pwndSelectEditJog->SetPropValueT("taborder", 19*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
		u_pwndSelectEditJog->UpdateAll();
	
		if(u_Move_Mode == 1)	
		{
			((CtmFormView*)pwndSender)->Goto(19);		//吋動量下拉選單
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
	else if(pwndTemp == BtnTop)
	{
		if(sel_line>1)
			sel_line--;
		if(sel_line>((CtmMTextMemo*)TextMemoPos)->GetStrNum())	sel_line=((CtmMTextMemo*)TextMemoPos)->GetStrNum();
		if(sel_line < 0 ) sel_line=1;
		((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10)	;
	}
	else if(pwndTemp == BtnBottom)
	{
		sel_line++;
		if(sel_line>((CtmMTextMemo*)TextMemoPos)->GetStrNum())	sel_line=((CtmMTextMemo*)TextMemoPos)->GetStrNum();
		((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10)	;
	}
	else if(pwndTemp == BtnMoveUp)
	{
		char tmp[512];
		char *ptr=tmp;
		if(sel_line>1 && sel_line<=((CtmMTextMemo*)TextMemoPos)->GetStrNum()-1)
		{
			((CtmMTextMemo*)TextMemoPos)->GetStr(sel_line-1, ptr,FALSE);
			if(ptr!=NULL)
			{
				((CtmMTextMemo*)TextMemoPos)->DelRow_New(sel_line-1,sel_line-1);
				sel_line--;
				((CtmMTextMemo*)TextMemoPos)->AddNewStr(sel_line-1,ptr,FALSE);
				((CtmMTextMemo*)TextMemoPos)->Update();
				((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10)	;
			}
		}	 
	}
	else if(pwndTemp == BtnMoveDown)
	{
		char tmp[512];
		char *ptr=tmp;
		if(sel_line>0 && sel_line<((CtmMTextMemo*)TextMemoPos)->GetStrNum()-1)
		{
			((CtmMTextMemo*)TextMemoPos)->GetStr(sel_line-1, ptr,FALSE);
			if(ptr!=NULL)
			{
				((CtmMTextMemo*)TextMemoPos)->DelRow_New(sel_line-1,sel_line-1);
				sel_line++;
				((CtmMTextMemo*)TextMemoPos)->AddNewStr(sel_line-1,ptr,FALSE);
				((CtmMTextMemo*)TextMemoPos)->Update();
				((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10)	;
			}
		}
	}
	else if(pwndTemp == BtnDeletePos)
	{
		if(sel_line<((CtmMTextMemo*)TextMemoPos)->GetStrNum()-1)
			((CtmMTextMemo*)TextMemoPos)->DelRow_New(sel_line-1,sel_line-1);
		if(sel_line>=((CtmMTextMemo*)TextMemoPos)->GetStrNum())
		{
			sel_line=((CtmMTextMemo*)TextMemoPos)->GetStrNum()-1;
			((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10);
		}
		((CtmMTextMemo*)TextMemoPos)->Update();
	}
	else if(pwndTemp == BtnInsterPos)
	{
		char a[128];
		sprintf(a,"X=%d.%03d Y=%d.%03d Z=%d.%03d RX=%d.%03d RY=%d.%03d RZ=%d.%03d",
							g_pRxCNCData->CNC_absolute.x/1000,abs(g_pRxCNCData->CNC_absolute.x%1000),
							g_pRxCNCData->CNC_absolute.y/1000,abs(g_pRxCNCData->CNC_absolute.y%1000),
							g_pRxCNCData->CNC_absolute.z/1000,abs(g_pRxCNCData->CNC_absolute.z%1000),
							g_pRxCNCData->CNC_absolute.a/1000,abs(g_pRxCNCData->CNC_absolute.a%1000),
							g_pRxCNCData->CNC_absolute.b/1000,abs(g_pRxCNCData->CNC_absolute.b%1000),
							g_pRxCNCData->CNC_absolute.c/1000,abs(g_pRxCNCData->CNC_absolute.c%1000));
		((CtmMTextMemo*)TextMemoPos)->AddNewStr(sel_line-1,a,FALSE);
		sel_line++;
		((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10)	;
		((CtmMTextMemo*)TextMemoPos)->Update();
	}
	else if(pwndTemp == BtnCreateLog)
	{
		((CtmMTextMemo *)TextMemoPos)->SaveToFile("Data/Glue.tmp");
	}
	else if(pwndTemp == BtnRevertLog)
	{
		sel_line=1;
		((CtmMTextMemo*)TextMemoPos)->LoadFromFile("Data/Glue.tmp", 0,TRUE);
		((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(1,1046 ,10)	;
		((CtmMTextMemo*)TextMemoPos)->Update();
	}
	else if(pwndTemp == BtnCreateProg)
	{
			int i =CreateProg();
			if(i!=1)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_BTN_CREATE_PROG_ERROR"], tmFT_CODE_TECH);
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_BTN_CREATE_PROG_OK"], tmFT_CODE_TECH);
				
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
		}
	}
	else if(message==MSG_PANEL_KEY)
	{
		char keycode;
		keycode = wParam;
		//printf("panel key = %x\n",keycode);
		if(keycode>0x80 && Press_Move_btn ==0 && g_TxCNCData.shmCNCData.CNC_servo_on == 1)
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

	if(JogLock && (g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE))
	{
		JogLock =FALSE;
	}
	else if(!JogLock && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		JogLock =TRUE;
	} 
	if(!RunOnlyOne)
	{
		RunOnlyOne = TRUE;
		ini(pwndSender);
		sel_line=1;
		((CtmMTextMemo*)TextMemoPos)->SetSelectWithFileLine(sel_line,1046 ,10);
	}
	ShowCurrentCoordinate();
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
	/*for(int i =0; i<cfg.jn; i++)		//關節座標(Joint)
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
	}*/
	for(int i =0/*cfg.jn*/; i<cfg.cn/*+cfg.jn*/; i++)		//卡氏座標(Cartesian)
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

/*void	SetLimitFlag(int Axis,int Value)
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
				u_pwndStaticCTable[i]->SetPropValueT("textID", cfg.c_sid[i]);
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

void		SetUnit(int UnitType)	//設定單位(mm or in)  20140915		by	Mario
{
	int temp = 0;
	if(UnitType == 0)
	{
		while(temp < 3)
		{
			u_pwndTPUnit[temp]->SetPropValueT("textID","ROBOT_UNIT_MM");
			temp+=1;
		}
	}
	else
	{
		while(temp < 3)
		{
			u_pwndTPUnit[temp]->SetPropValueT("textID","ROBOT_UNIT_INCH");
			temp+=1;
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
	u_pwndSelectEditJog->SetPropValueT("taborder", 19*(u_Move_Mode==1)-1*!(u_Move_Mode==1));
	u_pwndSelectEditJog->UpdateAll();
	if(u_Move_Mode == 1)	
	{
		((CtmFormView*)pwndSender)->Goto(19);		//吋動量下拉選單
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

POINTDATA_T setPointdata(double x, double y, double z, double rx, double ry, double rz){
	POINTDATA_T p;
	p.x = x;
	p.y = y;
	p.z = z;
	p.rx = rx;
	p.ry = ry;
	p.rz = rz;
	return p;
}

int Push_pointdata(PATHDATA_T *v,POINTDATA_T p){
	if (v->iPoint_top >= MAX_TEACH_POINT){
		#ifdef PATH_T_DBG
		printf("point data full\n");
		#endif
		return 0;
	}
	v->aPoint_t[v->iPoint_top] = p;
	v->iPoint_top++;
	return 1;
}

int Pop_pointdata(PATHDATA_T *v){
	if (v->iPoint_top <= 0){
		#ifdef PATH_T_DBG
		printf("point data empty\n");
		#endif
		return 0;
	}
	v->iPoint_top--;
	return 1;
}

int Push_pathdata(PATHDATA_T *v,POINTDATA_T p){
	if (v->iPath_top >= MAX_TEACH_POINT){
		#ifdef PATH_T_DBG
		printf("path data full\n");
		#endif
		return 0;
	}
	v->aPath_t[v->iPath_top] = p;
	v->iPath_top++;
	return 1;
}

int Pop_pathdata(PATHDATA_T *v){
	if (v->iPath_top <= 0){
		#ifdef PATH_T_DBG
		printf("path data empty\n");
		#endif
		return 0;
	}
	v->iPath_top--;
	return 1;
}

int Print_point(POINTDATA_T p){
	printf("(%.03f,%.03f,%.03f,%.03f,%.03f,%.03f)->%d\n",p.x,p.y,p.z,p.rx,p.ry,p.rz,p.type);
	return 1;
}

int Show_point(PATHDATA_T *v){
	int i;
	for (i=0;i<v->iPoint_top;i++){
		Print_point(v->aPoint_t[i]);
	}
	return 1;
}

int Show_path(PATHDATA_T *v){
	int i,j;
	for (i=0;i<v->iPath_top;i++){
		Print_point(v->aPath_t[i]);
	}
	return 1;
}

// 原理就是直線轉折處都用圓弧連起來
// 找轉折處夾角的內切圓，定好半徑範圍
// 會找到適合的半徑，和切線到轉折點的長度，和圓心連起來會是一個兩個直角三角形組合的鳶形
// 把斜邊扣掉半徑就是轉折點到圓弧的最短距離，得到通過點
// 可以看圖
int Path_generate(PATHDATA_T *v){
	int i = 0,j;
	POINTDATA_T p_tmp_0,p_tmp_1,p_tmp_2;
	double u[3],t[3],w[3],dot,theta,L_u,L_v,L,R,L_w;
	// start point
	p_tmp_0 = v->aPoint_t[i];
	p_tmp_0.type = POINT_t;
	Push_pathdata(v,p_tmp_0);
	// mid point
	for (i=1;i<v->iPoint_top-1;i++){
		p_tmp_0 = v->aPoint_t[i-1];
		p_tmp_1 = v->aPoint_t[i];
		p_tmp_2 = v->aPoint_t[i+1];
		// 作內切圓，找切線向量算長度
		u[0] = - p_tmp_1.x + p_tmp_0.x;
		u[1] = - p_tmp_1.y + p_tmp_0.y;
		u[2] = - p_tmp_1.z + p_tmp_0.z;
		L_u = sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
		// 原本要用u,v,w的，但是不小心給資料的指標用掉了
		t[0] = - p_tmp_1.x + p_tmp_2.x;
		t[1] = - p_tmp_1.y + p_tmp_2.y;
		t[2] = - p_tmp_1.z + p_tmp_2.z;
		L_v = sqrt(t[0]*t[0]+t[1]*t[1]+t[2]*t[2]);
		
		dot = u[0]*t[0]+u[1]*t[1]+u[2]*t[2];
		#ifdef PATH_T_DBG
		printf("Point %d: L_u = %.03f , L_v = %.03f ,dot = %.03f\n",i,L_u,L_v,dot);
		#endif
		// 算出角度
		theta = acos(dot/L_u/L_v);
		// 用設定的最大半徑反推
		R = ARC_R;
		L = L_u>L_v?0.5*L_v:0.5*L_u;
		L = L>R/tan(0.5*theta)?R/tan(0.5*theta):L;
		R = L*tan(0.5*theta);
		
		#ifdef PATH_T_DBG
		printf("Point %d: R = %.03f ,Theta = %.03f\n",i,R,theta*180.0/3.1415926);
		#endif
		
		if (R<ARC_R_MIN){
			#ifdef PATH_T_DBG
			printf("R < ARC_R_MIN, Path generate fail\n");
			#endif
			return 0;
		}
		// 變成單位向量，相加除以二就是分角線，拿來找圓弧第三點

		for (j=0;j<3;j++){
			u[j] = u[j]/L_u;
			t[j] = t[j]/L_v;
			w[j] = 0.5*(u[j]+t[j]);
		}
		L_w = sqrt(w[0]*w[0]+w[1]*w[1]+w[2]*w[2]);
		for (j=0;j<3;j++){
			w[j] = w[j]/L_w;
		}
		// 整段圓弧的姿態用教導點的姿態
		p_tmp_0 = p_tmp_1;
		p_tmp_2 = p_tmp_1;
		// 起點和終點
		p_tmp_0.x += u[0]*L;
		p_tmp_0.y += u[1]*L;
		p_tmp_0.z += u[2]*L;
		p_tmp_0.type = POINT_t;
		
		p_tmp_2.x += t[0]*L;
		p_tmp_2.y += t[1]*L;
		p_tmp_2.z += t[2]*L;
		p_tmp_2.type = END_t;
		// 找圓弧通過點
		p_tmp_1.x += w[0]*(sqrt(L*L+R*R)-R);
		p_tmp_1.y += w[1]*(sqrt(L*L+R*R)-R);
		p_tmp_1.z += w[2]*(sqrt(L*L+R*R)-R);
		p_tmp_1.type = ARC_t;
		// 照通過的順序寫進資料
		Push_pathdata(v,p_tmp_0);
		Push_pathdata(v,p_tmp_1);
		Push_pathdata(v,p_tmp_2);
		
		#ifdef PATH_T_DBG
		circle[i].x = p_tmp_1.x+w[0]*R;
		circle[i].y = p_tmp_1.y+w[1]*R;
		circle[i].z = p_tmp_1.z+w[2]*R;
		circle[i].r = R;
		printf("Point %d: ( %.03f ,%.03f ,%.03f ), R = %.03f ,Theta = %.03f\n",i,\
			   circle[i].x,circle[i].y,circle[i].z,circle[i].r,theta*180.0/3.1415926);
		#endif
	}
	//end point
	p_tmp_0 = v->aPoint_t[i];
	p_tmp_0.type = POINT_t;
	Push_pathdata(v,p_tmp_0);
	
	#ifdef PATH_T_DBG
	printf("Path gen end\n");
	#endif
	
	return 1;
}

char  coordstr_x[30];
char  coordstr_y[30];
char  coordstr_z[30];
char  coordstr_rx[30];
char  coordstr_ry[30];
char  coordstr_rz[30];
char  coordstr_x1[30];
char  coordstr_y1[30];
char  coordstr_z1[30];
char  coordstr_rx1[30];
char  coordstr_ry1[30];
char  coordstr_rz1[30];
char* get_coord(double vlaue , int num , char* str)//小數點後幾位
{
	//printf("vlaue = %f\n",vlaue);
	if(num<0) return NULL;
	memset(str,'\0',30);
	sprintf(str,"%.03f",vlaue);
	//printf("str = %s\n",str);
	int a=strlen(str);
	char * ptr = str+a-1;
	while(*ptr!='\0')
	{
		if(*ptr=='.' && *(ptr+1)=='\0')
		{
			*ptr = '\0';
			break;
		} 
		if(*ptr=='0')
			*ptr = '\0';
		else if(*ptr!='0') 
			break;
		ptr--;
	}
	return str;
}

int CreateProg()
{
	char tmp[512];
	char* ptr = tmp;
	double x,y,z,rx,ry,rz;
	sPathdata_t.iPath_top=0;
	sPathdata_t.iPoint_top=0;
	for(int i=0 ; i<((CtmMTextMemo*)TextMemoPos)->GetStrNum() ; i++)
	{
		ptr=tmp;
		memset(tmp,0,sizeof(tmp));
		((CtmMTextMemo*)TextMemoPos)->GetStr(i, ptr,FALSE);
		while(*ptr != '=') ptr++;
		ptr++;
		x = atof(ptr);
		while(*ptr != '=') ptr++;
		ptr++;
		y = atof(ptr);
		while(*ptr != '=') ptr++;
		ptr++;
		z = atof(ptr);
		while(*ptr != '=') ptr++;
		ptr++;
		rx = atof(ptr);
		while(*ptr != '=') ptr++;
		ptr++;
		ry = atof(ptr);
		while(*ptr != '=') ptr++;
		ptr++;
		rz = atof(ptr);
		if(Push_pointdata(&sPathdata_t,setPointdata(x,y,z,rx,ry,rz))!=1) return 0;
	}
	Show_point(&sPathdata_t);		
	int filename;
	EditFileName->GetPropValueT("value",&filename, sizeof(filename));
	char ProgName[32];
	sprintf(ProgName,"Prog/O%04d\0",filename);
	FILE *ProgTEXT;
	ProgTEXT = fopen(ProgName,"w");
	
	fprintf(ProgTEXT,"/*GLUE SUB PROG*/\n\n");
	if(Path_generate(&sPathdata_t)!=1) return 0;
	
	for(int i=0; i<sPathdata_t.iPath_top ;i++)
	{
		if(sPathdata_t.aPath_t[i].type==POINT_t)
		{
			get_coord(sPathdata_t.aPath_t[i].x,3,coordstr_x);
			get_coord(sPathdata_t.aPath_t[i].y,3,coordstr_y);
			get_coord(sPathdata_t.aPath_t[i].z,3,coordstr_z);
			get_coord(sPathdata_t.aPath_t[i].rx,3,coordstr_rx);
			get_coord(sPathdata_t.aPath_t[i].ry,3,coordstr_ry);
			get_coord(sPathdata_t.aPath_t[i].rz,3,coordstr_rz);
			if(i==0 || i==sPathdata_t.iPath_top-1)
			{
				fprintf(ProgTEXT,"LINE_ABS (X=%s Y=%s Z=%s RX=%s RY=%s RZ=%s POS=0) S100 F0;\n",	\
													coordstr_x,coordstr_y,coordstr_z,coordstr_rx,coordstr_ry,coordstr_rz);
			}
			else
			{
				fprintf(ProgTEXT,"LINE_ABS (X=%s Y=%s Z=%s RX=%s RY=%s RZ=%s POS=0) S100 F5;\n",	\
													coordstr_x,coordstr_y,coordstr_z,coordstr_rx,coordstr_ry,coordstr_rz);
			}
		}
		else if(sPathdata_t.aPath_t[i].type == ARC_t && sPathdata_t.aPath_t[i+1].type == END_t)
		{
			get_coord(sPathdata_t.aPath_t[i].x,3,coordstr_x);
			get_coord(sPathdata_t.aPath_t[i].y,3,coordstr_y);
			get_coord(sPathdata_t.aPath_t[i].z,3,coordstr_z);
			get_coord(sPathdata_t.aPath_t[i].rx,3,coordstr_rx);
			get_coord(sPathdata_t.aPath_t[i].ry,3,coordstr_ry);
			get_coord(sPathdata_t.aPath_t[i].rz,3,coordstr_rz);
			get_coord(sPathdata_t.aPath_t[i+1].x,3,coordstr_x1);
			get_coord(sPathdata_t.aPath_t[i+1].y,3,coordstr_y1);
			get_coord(sPathdata_t.aPath_t[i+1].z,3,coordstr_z1);
			get_coord(sPathdata_t.aPath_t[i+1].rx,3,coordstr_rx1);
			get_coord(sPathdata_t.aPath_t[i+1].ry,3,coordstr_ry1);
			get_coord(sPathdata_t.aPath_t[i+1].rz,3,coordstr_rz1);
			//fprintf(ProgTEXT,"CIRCLE_ABS");
			fprintf(ProgTEXT,"CIRCLE_ABS (X=%s Y=%s Z=%s RX=%s RY=%s RZ=%s POS=0) (X=%s Y=%s Z=%s RX=%s RY=%s RZ=%s POS=0) S100 F5;\n",	\
												coordstr_x,coordstr_y,coordstr_z,coordstr_rx,coordstr_ry,coordstr_rz,
												coordstr_x,coordstr_y1,coordstr_z1,coordstr_rx1,coordstr_ry1,coordstr_rz1);
			i++;
		}
		else	
		{
			fclose(ProgTEXT);
			return 0;
		}
	}
	fprintf(ProgTEXT,"\nRETURN;\n");
	fclose(ProgTEXT);
	Etnernet_File(ProgName);
	return 1;
}
