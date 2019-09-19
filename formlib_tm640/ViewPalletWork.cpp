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
#include	"ViewPalletWork.h"
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
#define	SEL_COLOR							0xFC00
#define	NO_SEL_COLOR						0x0416
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
CtmWnd*		u_pwndStaticCTable[12]				= {NULL};		//右方移動控制區的座標

//中間座標資訊↓---------------------------------
CtmWnd*		u_pwndCoordTable[7]					= {NULL};
int				u_nCoordTableCnt						=	0;

CtmWnd*		u_pwndStaticUnut[64]				= {NULL};
int				u_nStaticUnitCnt						=	0;

//其他控制設定↓-----------------------------------
CtmWnd*		u_pwndBtnMoveMode			= NULL;
CtmWnd*		u_pwndSelectEditJog		= NULL;

//PALLET SET
CtmWnd*		BtnSetWorkCoord		= NULL;
CtmWnd*		BtnSetReadCoord		= NULL;
CtmWnd*		BtnSetPaelletO		= NULL;
CtmWnd*		BtnSetPaelletR		= NULL;
CtmWnd*		BtnSetPaelletC		= NULL;
CtmWnd*		EditFileName			= NULL;
CtmWnd*		EditWorkNo				= NULL;
CtmWnd*		EditToolNo				= NULL;
CtmWnd*		EditToolOutput		= NULL;
CtmWnd*		EditInput					= NULL;
CtmWnd*		EditRowNum				= NULL;
CtmWnd*		EditColNum				= NULL;
CtmWnd*		EditLevelNum			= NULL;
CtmWnd*		EditShiftZ				= NULL;
CtmWnd*		EditPalletCatchZ	= NULL;
CtmWnd*		EditWorkCatchZ		= NULL;
CtmWnd*		EditOutput1				= NULL;
CtmWnd*		EditOutput2				= NULL;
CtmWnd*		EditDelay1				= NULL;
CtmWnd*		EditDelay2				= NULL;
CtmWnd*		EditEndNum				= NULL;
CtmWnd*		BtnZMode					= NULL;
CtmWnd*		BtnSMode					= NULL;
CtmWnd*		BtnOK							= NULL;

enum PALLETMODE{PALLET_Z_MODE,PALLET_S_MODE};
int PalletMode=PALLET_Z_MODE;
int CoordSetFlag=0;
double WorkCoord[6];
double OCoord[6];
double RCoord[6];
double CCoord[6];
double ReadyCoord[6];

//------------------------------------------------
int		u_nStaticCTableCnt		=0;

BOOL		u_bEnterKey						= FALSE;

int			u_lSpeed							=0;
int			u_nMoveControlType		=COORDINATETYPE_J;//COORDINATETYPE_LOCK;
int			u_nTPTableType				=0;

WORD		u_wDBvalueC_StartNo					=0;		//控制參數編號：卡氏座標的X座標


//add		by		Mario
int			Unitindex						=0;		//表示目前單位(0=公制,1=英制)
CtmWnd*		u_pwndTPUnit[3]		= {NULL};		//指向卡氏座標xyz的顯示單位元件以配合公英制做改變
//90140915↑
int			u_Move_Mode					=0;			//移動模式 0=連續  1=吋動
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
	u_nStaticCTableCnt		= GetSpecialControlNum(pwndSender, "StaticC", "CtmStaticX2", u_pwndStaticCTable);
	
	
	//=====中間當前座標位置=====
	u_nCoordTableCnt		= GetSpecialControlNum(pwndSender, "CTable", "CtmEditX1", u_pwndCoordTable);
	u_nStaticUnitCnt		= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnut);

	
	//20140919↑				by Mario
	//=====其他控制設定=====
	u_pwndBtnMoveMode			= pwndSender->FindControlFromName("BtnMoveMode");
	u_pwndSelectEditJog		= pwndSender->FindControlFromName("SelectEditJog");
	
	u_wDBvalueC_StartNo		   	= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	
	//讀取目前單位型態 20140915 by Mario
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	
	//抓取卡氏座標單位元件
	u_pwndTPUnit[0]	= pwndSender->FindControlFromName("StaticUnit04");
	u_pwndTPUnit[1] = pwndSender->FindControlFromName("StaticUnit05");
	u_pwndTPUnit[2] = pwndSender->FindControlFromName("StaticUnit06");
	
	BtnSetWorkCoord		= pwndSender->FindControlFromName("BtnWorkCoordSet");
	BtnSetReadCoord		= pwndSender->FindControlFromName("btnReadyCoordSet");
	BtnSetPaelletO		= pwndSender->FindControlFromName("btnOriginCoordSet");
	BtnSetPaelletR		= pwndSender->FindControlFromName("btnRowCoordSet");
	BtnSetPaelletC		= pwndSender->FindControlFromName("btnColCoordSet");
	EditFileName			= pwndSender->FindControlFromName("EditFileName");
	EditWorkNo				= pwndSender->FindControlFromName("EditWork");
	EditToolNo				= pwndSender->FindControlFromName("EditTool");
	EditToolOutput		= pwndSender->FindControlFromName("ToolOutPut");
	EditInput					= pwndSender->FindControlFromName("EditWorkInput");
	EditRowNum				= pwndSender->FindControlFromName("EditPalletRow");
	EditColNum				= pwndSender->FindControlFromName("EditPalletCol");
	EditLevelNum			= pwndSender->FindControlFromName("EditLevel");
	EditShiftZ				= pwndSender->FindControlFromName("EditZ");
	EditPalletCatchZ	= pwndSender->FindControlFromName("EditPalletZ");
	EditWorkCatchZ		= pwndSender->FindControlFromName("EditWorkZ");
	EditOutput1				= pwndSender->FindControlFromName("Editoutput1");
	EditOutput2				= pwndSender->FindControlFromName("Editoutput2");
	EditDelay1				= pwndSender->FindControlFromName("EditDelay1");
	EditDelay2				= pwndSender->FindControlFromName("EditDelay2");
	EditEndNum				= pwndSender->FindControlFromName("EditTotalNum");
	BtnZMode					= pwndSender->FindControlFromName("btnZmode");
	BtnSMode					= pwndSender->FindControlFromName("btnSmode");
	BtnOK							= pwndSender->FindControlFromName("btnOK");
			
	//設定單位	
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

	if(pwndTemp == u_pwndBtnMoveMode)
	{
		MsgBoxCall("msgboxUserManage.txt",0);
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
	else if(pwndTemp == BtnZMode)
	{
		PalletMode=PALLET_Z_MODE;
		BtnZMode->SetPropValueT("bgc",SEL_COLOR);
		BtnZMode->Update();
		BtnSMode->SetPropValueT("bgc",NO_SEL_COLOR);
		BtnSMode->Update();
	}
	else if(pwndTemp == BtnSMode)
	{
		PalletMode=PALLET_S_MODE;
		BtnZMode->SetPropValueT("bgc",NO_SEL_COLOR);
		BtnZMode->Update();
		BtnSMode->SetPropValueT("bgc",SEL_COLOR);
		BtnSMode->Update();
	}
	else if(pwndTemp == BtnOK)
	{
		int i =CreateProg();
		printf("Create %d\n",i);
		if(i!=0)
			MsgBoxConfirm(g_MultiLanguage["ROBOT_BTN_CREATE_PROG_ERROR"], tmFT_CODE_TECH);
		else
			MsgBoxConfirm(g_MultiLanguage["ROBOT_BTN_CREATE_PROG_OK"], tmFT_CODE_TECH);
	}
	else if(pwndTemp == BtnSetWorkCoord)
	{
		CoordSetFlag |= 0x01;
		WorkCoord[0]=(double)g_pRxCNCData->CNC_absolute.x/1000;
		WorkCoord[1]=(double)g_pRxCNCData->CNC_absolute.y/1000;
		WorkCoord[2]=(double)g_pRxCNCData->CNC_absolute.z/1000;
		WorkCoord[3]=(double)g_pRxCNCData->CNC_absolute.a/1000;
		WorkCoord[4]=(double)g_pRxCNCData->CNC_absolute.b/1000;
		WorkCoord[5]=(double)g_pRxCNCData->CNC_absolute.c/1000;
	}
	else if(pwndTemp == BtnSetPaelletO)
	{
		CoordSetFlag |= 0x02;
		OCoord[0]=(double)g_pRxCNCData->CNC_absolute.x/1000;
		OCoord[1]=(double)g_pRxCNCData->CNC_absolute.y/1000;
		OCoord[2]=(double)g_pRxCNCData->CNC_absolute.z/1000;
		OCoord[3]=(double)g_pRxCNCData->CNC_absolute.a/1000;
		OCoord[4]=(double)g_pRxCNCData->CNC_absolute.b/1000;
		OCoord[5]=(double)g_pRxCNCData->CNC_absolute.c/1000;
	}
	else if(pwndTemp == BtnSetPaelletR)
	{
		CoordSetFlag |= 0x04;
		CCoord[0]=(double)g_pRxCNCData->CNC_absolute.x/1000;
		CCoord[1]=(double)g_pRxCNCData->CNC_absolute.y/1000;
		CCoord[2]=(double)g_pRxCNCData->CNC_absolute.z/1000;
		CCoord[3]=(double)g_pRxCNCData->CNC_absolute.a/1000;
		CCoord[4]=(double)g_pRxCNCData->CNC_absolute.b/1000;
		CCoord[5]=(double)g_pRxCNCData->CNC_absolute.c/1000;
	}
	else if(pwndTemp == BtnSetPaelletC)
	{
		CoordSetFlag |= 0x08;
		RCoord[0]=(double)g_pRxCNCData->CNC_absolute.x/1000;
		RCoord[1]=(double)g_pRxCNCData->CNC_absolute.y/1000;
		RCoord[2]=(double)g_pRxCNCData->CNC_absolute.z/1000;
		RCoord[3]=(double)g_pRxCNCData->CNC_absolute.a/1000;
		RCoord[4]=(double)g_pRxCNCData->CNC_absolute.b/1000;
		RCoord[5]=(double)g_pRxCNCData->CNC_absolute.c/1000;
	}
	else if(pwndTemp == BtnSetReadCoord)
	{
		CoordSetFlag |= 0x016;
		ReadyCoord[0]=(double)g_pRxCNCData->CNC_absolute.x/1000;
		ReadyCoord[1]=(double)g_pRxCNCData->CNC_absolute.y/1000;
		ReadyCoord[2]=(double)g_pRxCNCData->CNC_absolute.z/1000;
		ReadyCoord[3]=(double)g_pRxCNCData->CNC_absolute.a/1000;
		ReadyCoord[4]=(double)g_pRxCNCData->CNC_absolute.b/1000;
		ReadyCoord[5]=(double)g_pRxCNCData->CNC_absolute.c/1000;
	}
	return wIDControl;
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{	
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
	//UpdateJointInfo();
	//UpdateRunAndCycleTime();
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
				u_pwndStaticCTable[i]->SetPropValueT("textID", cfg.j_sid[i]/*u_pszStrCoordinateJ[i]*/);
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

enum ERROR_VALUE{OK,SET_COORD,ROW_COL_NUM,TOOL_OUTPUT_NO,PALLET_LEVEL_TALL,CATCH_Z,PALLET_Z,PALLET_OUTPUT1,PALLET_OUTPUT2,END_NUM};

int		CreateProg()
{
	int error_num;
	FILE *ProgTEXT;
	
	int filename;
	EditFileName->GetPropValueT("value",&filename, sizeof(filename));
	char ProgName[16];
	sprintf(ProgName,"Prog/O%04d\0",filename);
	
	if(CoordSetFlag != 31) return SET_COORD;
		
	int Tooloutput;
	EditToolOutput->GetPropValueT("value",&Tooloutput, sizeof(Tooloutput));
	if(Tooloutput <=0) return TOOL_OUTPUT_NO;
		
	int c,r;
	EditColNum->GetPropValueT("value",&c, sizeof(c));
	EditRowNum->GetPropValueT("value",&r, sizeof(r));
	if(c <=0 || r <=0) return ROW_COL_NUM;
		
	int l,lz;
	EditLevelNum->GetPropValueT("value",&l, sizeof(l));
	EditShiftZ->GetPropValueT("value",&lz, sizeof(lz));
	if(l <=0 || (l>1 && lz<=0)) return PALLET_LEVEL_TALL;

	int wz,pz;
	EditWorkCatchZ->GetPropValueT("value",&wz, sizeof(wz));
	EditPalletCatchZ->GetPropValueT("value",&pz, sizeof(pz));
	if(wz <=0) return CATCH_Z;
	if(pz<=0 || pz < ((l-1)*lz))	return PALLET_Z;
		
	int output1,delay1;
	EditOutput1->GetPropValueT("value",&output1, sizeof(output1));
	EditDelay1->GetPropValueT("value",&delay1, sizeof(delay1));
	if(output1<=0 || output1==Tooloutput || delay1<=0) return PALLET_OUTPUT1;
		
	int output2,delay2;
	EditOutput2->GetPropValueT("value",&output2, sizeof(output2));
	EditDelay2->GetPropValueT("value",&delay2, sizeof(delay2));
	if(output2<=0 || output2==Tooloutput || output2==output1 || delay2<=0) return PALLET_OUTPUT2;
		
	int end_num;
	EditEndNum->GetPropValueT("value",&end_num, sizeof(end_num));
	//if(end_num <=0) return END_NUM;
		
	printf("ProgName = %s\n",ProgName);
	ProgTEXT = fopen(ProgName,"w");
	
	//fprintf(ProgTEXT,"/*PALLET PROG*/\n\n");
	
	int w,t;
	EditWorkNo->GetPropValueT("value",&w, sizeof(w));
	EditToolNo->GetPropValueT("value",&t, sizeof(t));
	if(w!=0) fprintf(ProgTEXT,"WORKCOORD W%d;\n",w);
	if(t!=0) fprintf(ProgTEXT,"TOOL T%d;\n",t);
	fprintf(ProgTEXT,"C1 = (X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f POS=0);\n",WorkCoord[0],WorkCoord[1],WorkCoord[2]+wz,WorkCoord[3],WorkCoord[4],WorkCoord[5]);
	fprintf(ProgTEXT,"C2 = (X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f POS=0);\n",WorkCoord[0],WorkCoord[1],WorkCoord[2],WorkCoord[3],WorkCoord[4],WorkCoord[5]);
	fprintf(ProgTEXT,"C3 = (X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f POS=0);\n",ReadyCoord[0],ReadyCoord[1],ReadyCoord[2],ReadyCoord[3],ReadyCoord[4],ReadyCoord[5]);
	fprintf(ProgTEXT,"C4 = (X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f POS=0);\n",OCoord[0],OCoord[1],OCoord[2],OCoord[3],OCoord[4],OCoord[5]);
	fprintf(ProgTEXT,"C5 = (X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f POS=0);\n",RCoord[0],RCoord[1],RCoord[2],RCoord[3],RCoord[4],RCoord[5]);
	fprintf(ProgTEXT,"C6 = (X=%.3f Y=%.3f Z=%.3f RX=%.3f RY=%.3f RZ=%.3f POS=0);\n",CCoord[0],CCoord[1],CCoord[2],CCoord[3],CCoord[4],CCoord[5]);
	
	fprintf(ProgTEXT,"#1=0;\n");
	fprintf(ProgTEXT,"#2=%d;\n",pz);
	fprintf(ProgTEXT,"#3=0;\n");
	fprintf(ProgTEXT,"#4=0;\n");
	fprintf(ProgTEXT,"#5=0;\n");
	fprintf(ProgTEXT,"#6=0;\n");
		
	if(PalletMode==PALLET_Z_MODE)
		fprintf(ProgTEXT,"PALLET1 = (C4 C5 C6 R=%d C=%d Z);\n",c,r);
	else 
		fprintf(ProgTEXT,"PALLET1 = (C4 C5 C6 R=%d C=%d S);\n",c,r);
	fprintf(ProgTEXT,"FRIST_PALLET1;\n");
	fprintf(ProgTEXT,"LINE_ABS C3 S100 F5;\n");
	fprintf(ProgTEXT,"N1000\n");
	int in;
	EditInput->GetPropValueT("value",&in, sizeof(in));
	if(in >0)
	{
		fprintf(ProgTEXT,"WAIT 0.5S;\n",in);
		fprintf(ProgTEXT,"#1 = GET_IN(%d);\n",in);
		fprintf(ProgTEXT,"IF(#1 != 1) GOTO 1000;\n");
	}
	
	fprintf(ProgTEXT,"LINE_ABS C1 S100 F5;\n");
	fprintf(ProgTEXT,"LINE_ABS C2 S100 F0;\n");
	fprintf(ProgTEXT,"SET_OUT %d = 1;\n",Tooloutput);
	fprintf(ProgTEXT,"WAIT 0.3S;\n");
	fprintf(ProgTEXT,"LINE_ABS C1 S100 F5;\n");
	
	
	fprintf(ProgTEXT,"//PUT WORK\n");
	fprintf(ProgTEXT,"LINE_ABS C3 S100 F5;\n");
	fprintf(ProgTEXT,"LINE_PLT PALLET1 Z#2 S100 F5;\n");
	fprintf(ProgTEXT,"LINE_PLT PALLET1 Z#3 S100 F0;\n");
	fprintf(ProgTEXT,"SET_OUT %d = 0;\n",Tooloutput);
	fprintf(ProgTEXT,"WAIT 0.3S;\n");
	fprintf(ProgTEXT,"LINE_PLT PALLET1 Z#2 S100 F5;\n");
	fprintf(ProgTEXT,"LINE_ABS C3 S100 F5;\n");
	fprintf(ProgTEXT,"#4 = GET_PT_PALLET1;\n");
	fprintf(ProgTEXT,"IF(#4==%d) GOTO 2000;\n",r*c);
	fprintf(ProgTEXT,"NEXT_PALLET1;\n");
	fprintf(ProgTEXT,"GOTO 1000;\n\n");
	
	fprintf(ProgTEXT,"N2000\n");
	fprintf(ProgTEXT,"FRIST_PALLET1;\n");
	fprintf(ProgTEXT,"#5=#5+1;\n");
	fprintf(ProgTEXT,"#3=#3+%d;\n",lz);
	//fprintf(ProgTEXT,"#2=#3+%d;\n",pz);
	fprintf(ProgTEXT,"IF(#5==%d) GOTO 3000;\n",l);
	fprintf(ProgTEXT,"SET_OUT %d = 1;\n",output1);
	fprintf(ProgTEXT,"WAIT %dMS;\n",delay1);
	fprintf(ProgTEXT,"SET_OUT %d = 0;\n",output1);
	fprintf(ProgTEXT,"GOTO 1000;\n\n");
	
	
	fprintf(ProgTEXT,"N3000\n");
	fprintf(ProgTEXT,"#5=0;\n");
	fprintf(ProgTEXT,"#3=0;\n");
	//fprintf(ProgTEXT,"#2=#3+%d;\n",pz);
	fprintf(ProgTEXT,"SET_OUT %d = 1;\n",output2);
	fprintf(ProgTEXT,"WAIT %dMS;\n",delay2);
	fprintf(ProgTEXT,"SET_OUT %d = 0;\n",output2);
	fprintf(ProgTEXT,"#6=#6+1;\n");
	if(end_num!=0)
		fprintf(ProgTEXT,"IF(#6==%d) GOTO 4000;\n",end_num);
	fprintf(ProgTEXT,"GOTO 1000;\n\n");

	fprintf(ProgTEXT,"N4000\n");
	fprintf(ProgTEXT,"LINE_ABS C3 S100 F5;\n");
	fprintf(ProgTEXT,"END;\n");
	fclose(ProgTEXT);
	Etnernet_File(ProgName);
	return 0;
}