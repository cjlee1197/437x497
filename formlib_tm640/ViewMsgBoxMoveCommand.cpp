/*============================================================================+
|  Class    : ViewMessageBoxMoveCommand library                         	    |
|  Task     : ViewMessageBoxMoveCommand library action source file						|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxMoveCommand.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h" 
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
#define	POINTSEL 		0x07FF
#define	POINTNOSEL 0xC618

#define	EditTRUE	 0xD71C
#define	EditFALSE	 0xC618

#define BTN_NOW_USE		0xFC00
#define BTN_NOW_NO_USE		0x0416
/*========================================================+
|						Global Variable																|
+========================================================*/
char*	ImgPath_1[] =					//Point_1圖片路徑
{
	"res_tm640/PTP.png",			
	"res_tm640/PTP2.png",	
	"res_tm640/LIN.png",			
	"res_tm640/LIN2.png",	
	"res_tm640/CIRC.png",			
	"res_tm640/CIRC2.png",	
	"res_tm640/CIRCF.png",			
	"res_tm640/CIRCF2.png",					
	NULL
}; 

char*	ImgPath_2[] =						//Point_2圖片路徑
{		
	"res_tm640/PTP.png",			
	"res_tm640/PTP2.png",	
	"res_tm640/LIN.png",			
	"res_tm640/LIN2.png",	
	"res_tm640/CIRC.png",			
	"res_tm640/CIRC3.png",		
	"res_tm640/CIRCF.png",			
	"res_tm640/CIRCF3.png",				
	NULL	
}; 

//元件指針
CtmWnd*			u_pwndSelectMoveType			= NULL;		//選擇移動方式元件指針
CtmWnd*			u_pwndShowMoveType				= NULL;		//移動方式示意圖元件指針

CtmWnd*			u_pwndTitleSetOne					= NULL;		//選擇設定Point_1元件指針
CtmWnd*			u_pwndTitleSetTwo					= NULL;		//選擇設定Point_2元件指針
CtmWnd*			u_pwndTitleSetTwoMark					= NULL;		//選擇設定Point_2Mark元件指針

CtmWnd*			u_pwndSelectPointCoord		= NULL;		//設定座標輸入方式元件指針

//CtmWnd*			u_pwndCheckBoxPointType01	= NULL;
//CtmWnd*			u_pwndCheckBoxPointType02	= NULL;
CtmWnd*			u_pwndBtnPointType01	= NULL;
CtmWnd*			u_pwndBtnPointType02	= NULL;

CtmWnd*			u_pwndSelectEditDefine		= NULL;

CtmWnd*			u_pwndInputTypeStatic			= NULL;		//輸入座標的提示訊息
CtmWnd*			u_pwndInputCoordNo				= NULL;		//輸入教導點編號或自訂點編號的元件指針
CtmWnd*			u_pwndEditTable[7]				={NULL};	//輸入卡氏座標元件指針
int					u_pwndEditTableCnt				=0;				//輸入卡氏座標元件指針總數
CtmWnd*			u_pwndStaticUnit[3]				={NULL};	//卡氏座標單位(mm <-> in)元件指針
int					u_pwndStaticUnitCnt				=0;				//卡氏座標單位(mm <-> in)元件指針總數
CtmWnd*			u_pwndEditStatic[7]				={NULL};	//座標標頭元件指針
int					u_pwndEditStaticCnt				=0;				//座標標頭元件指針總數


CtmWnd*			u_pwndBtnSpeedP						= NULL;		//速度+
CtmWnd*			u_pwndBtnSpeedM						= NULL;		//速度-
CtmWnd*			u_pwndShowSpeed						= NULL;		//速度
//CtmWnd*			u_pwndBtnNearPlace				= NULL;		//接近到位
CtmWnd*			u_pwndBtnSoftNessP				= NULL;
CtmWnd*			u_pwndBtnSoftNessM				= NULL;
CtmWnd*			u_pwndShowSoftNess				= NULL;	


CtmWnd*			u_pwndBtnOK								= NULL;		//OK
CtmWnd*			u_pwndBtnCancel						= NULL;		//Cancel


CtmWnd*			this_pwn									=	NULL;

CtmWnd*			u_pwndLinuxCtmImageBox1		=	NULL;

//指令設定值

char				CommandStr[256] 					="\0"; 		//最後整合指令字串

int					PointNum									=1;				//目前編輯的點

POINTCOORD 	u_Point1;
POINTCOORD 	u_Point2;
POINTCOORD * temp_POINT = &u_Point1;
//int					PointType_1[2]						={-1,-1};			//紀錄Point1的儲存狀態  -1:未存  0:座標   1:自訂   2:教導點
//int					PointType_2[2]						={-1,-1};			//紀錄Point2的儲存狀態  -1:未存  0:座標   1:自訂   2:教導點
//紀錄Point1的卡氏座標，若儲存狀態是1或2則用Point_1[0]紀錄點編號
//int					Point_1[7]								={0,0,0,0,0,0,0};		
//紀錄Point2的卡氏座標，若儲存狀態是1或2則用Point_2[0]紀錄點編號
//int 				Point_2[7]								={0,0,0,0,0,0,0};		

int 				speed 										= 10;			//目前速度
int					nearplace									=3;		//FALSE=OFF TRUE=ON
	

//系統參數和邏輯參數
int					UnitMode									=0;				//表示目前單位(0=公制,1=英制)，預設0
BOOL				u_bEnterKey								= FALSE;	//判斷是否按下ENTER鍵
BOOL				StartRunOne 							= TRUE;		//在Update中控制某些動作僅執行一次(OnCreateA中執行會發生元件背景消失的錯誤)
int 				u_time 										= 0;			//計時器	控制圖片變化
int					MoveMode									=	0;			//目前指定的移動模式  0:PTP		1:LIN		2:CIRC
int					u_DefineMode							= 0;
char 				ImagePath1[32] 						= "\0";		//圖片路徑
char 				ImagePath2[32] 						= "\0";		//圖片路徑	


/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	this_pwn = pwndSender;
	u_pwndSelectMoveType= pwndSender->FindControlFromName("SelectMoveType");
	u_pwndShowMoveType= pwndSender->FindControlFromName("ShowMoveType");
	
	u_pwndTitleSetOne= pwndSender->FindControlFromName("TitleSetOne");
	u_pwndTitleSetTwo= pwndSender->FindControlFromName("TitleSetTwo");
	u_pwndTitleSetTwoMark= pwndSender->FindControlFromName("TitleSetTwoMark");
	
	
	u_pwndSelectPointCoord= pwndSender->FindControlFromName("SelectPointCoord");
	//u_pwndCheckBoxPointType01= pwndSender->FindControlFromName("CheckBoxPointType01");
	//u_pwndCheckBoxPointType02= pwndSender->FindControlFromName("CheckBoxPointType02");
	u_pwndSelectEditDefine		= pwndSender->FindControlFromName("SelectEditDefineModeC");
	
	u_pwndBtnPointType01= pwndSender->FindControlFromName("BtnPointType01");
	u_pwndBtnPointType02= pwndSender->FindControlFromName("BtnPointType02");
												
	//u_pwndInputTypeStatic= pwndSender->FindControlFromName("InputTypeStatic");
	u_pwndInputCoordNo= pwndSender->FindControlFromName("InputCoordNo");
			
	u_pwndEditStaticCnt= GetSpecialControlNum(pwndSender, "TableStatic", "CtmStaticX2", u_pwndEditStatic);
	u_pwndEditTableCnt= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwndEditTable);
	u_pwndStaticUnitCnt= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnit);
	u_pwndBtnSpeedP= pwndSender->FindControlFromName("btnSpeedP");
	u_pwndBtnSpeedM= pwndSender->FindControlFromName("btnSpeedM");
	u_pwndShowSpeed= pwndSender->FindControlFromName("ShowSpeed");
	//u_pwndBtnNearPlace= pwndSender->FindControlFromName("btnNearPlace");
	u_pwndBtnSoftNessP= pwndSender->FindControlFromName("btnSoftNessP");
	u_pwndBtnSoftNessM= pwndSender->FindControlFromName("btnSoftNessM");
	u_pwndShowSoftNess= pwndSender->FindControlFromName("ShowSoftNess");
	
	u_pwndBtnOK= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel= pwndSender->FindControlFromName("btnCancel");
	
	u_pwndLinuxCtmImageBox1= pwndSender->FindControlFromName("LinuxCtmImageBox1");
	
	UnitMode									= GetDBValue("TUNE_PARAM37").lValue;		//取得公英制
	//u_pwndLinuxCtmImageBox1->SetPropValueT("imagepath","res_tm640/msgBG_776X535.png");
	
	return TRUE;
}


//WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
//{
	/*
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnOK)
		{
			SaveInputValue();
			if(PointType_1!=-1 && ( MoveMode==0 || MoveMode==1))
			{
				SortOutCommand();
				AddCommand(CommandStr);
				Exit();
			}
			else if(PointType_1!=-1 && PointType_2!=-1 && MoveMode==2 )
			{
				SortOutCommand();
				AddCommand(CommandStr);
				Exit();
			}
			else
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
			}
		}
	}
	return	wIDControl;
	*/
//}


WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnCancel)
	{
		remove("./DialogHelp/buffer");
		Exit();
	}
	else if(pwndTemp == u_pwndBtnSpeedP)
	{
		SetSpeedAndNearPlace(TRUE,TRUE);
	}
	else if(pwndTemp == u_pwndBtnSpeedM)
	{
		SetSpeedAndNearPlace(TRUE,FALSE);
	}
	else if(pwndTemp == u_pwndBtnSoftNessP)
	{ 
		SetSpeedAndNearPlace(FALSE,TRUE);
	}
	else if(pwndTemp == u_pwndBtnSoftNessM)
	{ 
		SetSpeedAndNearPlace(FALSE,FALSE);
	}
	/*else if(pwndTemp == u_pwndBtnNearPlace)
	{ 
		nearplace=!nearplace;
		if(nearplace)
			u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_OPEN");
		else
			u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_CLOSE");
		u_pwndBtnNearPlace->CreateA();
		u_pwndBtnNearPlace->Update();
	}*/
	else if(pwndTemp == u_pwndTitleSetOne & PointNum != 1)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		SaveInputValue();
		PointNum = 1;
		strcpy(ImagePath2,ImgPath_1[MoveMode*2+1]);
		SetTable();
	}
	else if(pwndTemp == u_pwndTitleSetTwo & PointNum != 2)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		SaveInputValue();
		PointNum = 2;
		strcpy(ImagePath2,ImgPath_2[MoveMode*2+1]);
		SetTable();
	}		
	else if(pwndTemp == u_pwndBtnPointType01)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		if(u_DefineMode == 1 )
		{
			u_DefineMode = 0;
			temp_POINT->DefineMode = u_DefineMode*10;
			//SetCheckBox();
			SetBtnType();
		}		
	}
	else if(pwndTemp == u_pwndBtnPointType02)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		if(u_DefineMode == 0 )
		{
			u_DefineMode = 1;
			temp_POINT->DefineMode = u_DefineMode*10;
			//SetCheckBox();
			SetBtnType();
		}
	}
	else if(pwndTemp == u_pwndBtnOK)
	{
		SaveInputValue();	
		if(( MoveMode==0 || MoveMode==1))
		{
			if((u_Point1.DefineMode == 0 && u_Point1.x == 0))
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
			}
			else
			{
				SortOutCommand();
				AddCommand(CommandStr);
				Exit();
			}
		}
		else if(MoveMode == 2 || MoveMode == 3)
		{
			if((u_Point1.DefineMode == 0 && u_Point1.x == 0) || (u_Point2.DefineMode == 0 && u_Point2.x == 0))
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
			}
			else
			{
				SortOutCommand();
				AddCommand(CommandStr);
				Exit();
			}
		}
	}
	
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
		if(pwndTemp == u_pwndSelectMoveType)
		{
			SetMoveMode(TRUE);
		}
		else if(pwndTemp == u_pwndSelectPointCoord)
		{
			temp_POINT->CoordinateMode = ((CtmSelectEdit*)u_pwndSelectPointCoord)->GetIndex();
			SetCoordMode();
			u_DefineMode = 0;
			temp_POINT->DefineMode = u_DefineMode*10;
			//SetCheckBox();
			SetBtnType();
		}
		else if(pwndTemp == u_pwndSelectEditDefine)
		{
			temp_POINT->DefineMode = u_DefineMode*10 + ((CtmSelectEdit*)u_pwndSelectEditDefine)->GetIndex();
		}
		
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	/*
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	  
	
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	
	if(wKey == _KEY_TEXTINPUT && u_pwndCheckBoxPointType01 == pwndTemp)
	{
		if(u_DefineMode == 1 )
		{
			u_DefineMode = 0;
			temp_POINT->DefineMode = u_DefineMode*10;
			SetCheckBox();
		}
		return wKey;
	}
	else if(wKey == _KEY_TEXTINPUT && u_pwndCheckBoxPointType02 == pwndTemp)
	{
		if(u_DefineMode == 0 )
		{
			u_DefineMode = 1;
			temp_POINT->DefineMode = u_DefineMode*10;
			SetCheckBox();
		}
		return wKey;
	}
	
	*/
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	return wKey;
	
	
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	printf("1\n");
	if(StartRunOne)
	{
		StartRunOne =FALSE;
		((CtmFormView*)pwndSender)->OnLoseFocus();
	printf("2\n");
		SetMoveMode(FALSE);
	printf("3\n");
		Initialize();
	printf("4\n");
		if(RevertCommand != NULL)
		{
	printf("5\n");
			if(!RevertMoveCommand(RevertCommand))
			{
	printf("6\n");
				remove("./DialogHelp/buffer");
				Exit();
	printf("7\n");
			}
	printf("8\n");
		}
		((CtmFormView*)pwndSender)->OnGetFocus();
	}

	printf("9\n");
	u_time = u_time+1;
	if(u_time == 10)
	{
		u_pwndShowMoveType->SetPropValueT("imagepath",ImagePath1);
		//u_pwndShowMoveType->CreateA();
		u_pwndShowMoveType->Update();
	}
	else if(u_time == 25)
	{
		u_pwndShowMoveType->SetPropValueT("imagepath",ImagePath2);
		//u_pwndShowMoveType->CreateA();
		u_pwndShowMoveType->Update();
		u_time = -5;
	}
	printf("10\n");
}
/*========================================================+
|						Helper																				|
+========================================================*/

void		AddCommand(char* pszCommand)		//插入指令
{
	//藉由額外產生的檔案來暫存要插入的指令
	FILE*		fCommandBuffer;
	fCommandBuffer = fopen("./DialogHelp/buffer", "w");
	fprintf(fCommandBuffer, "%s", pszCommand);
	fclose(fCommandBuffer);
}

void		SetSpeedAndNearPlace(BOOL Mode ,BOOL Type)
{
	char	temp[15] 		= "\0";
	char	szTemp[15]		= "\0";
	if(Mode)	//速度
	{
		if(Type)//+
		{
			if(speed<=90)
				speed+=10;
		}
		else//-
		{
			if(speed>=20)
				speed-=10;
		}
		sprintf(temp, "%d%s", speed, "%");
		CodeChange(szTemp, temp);
		u_pwndShowSpeed->SetPropValueT("text", szTemp);
		u_pwndShowSpeed->Update();
	}
	else	if(!Mode)	//SoftNess
	{
		if(Type)//+
		{
			if(nearplace<5)
				nearplace++;
		}
		else//-
		{
			if(nearplace>0)
				nearplace--;
		}
		sprintf(temp, "%d", nearplace);
		CodeChange(szTemp, temp);
		u_pwndShowSoftNess->SetPropValueT("text", szTemp);
		u_pwndShowSoftNess->Update();
	}
}

void		UpdateSpeed()
{
	char	temp[15] 		= "\0";
	char	szTemp[15]		= "\0";
	sprintf(temp, "%d%s", speed, "%");
	CodeChange(szTemp, temp);
	u_pwndShowSpeed->SetPropValueT("text", szTemp);
	u_pwndShowSpeed->Update();
	
	
	memset(temp,0,sizeof(temp));
	memset(szTemp,0,sizeof(szTemp));
	sprintf(temp, "%d\0", nearplace);
	CodeChange(szTemp, temp);
	u_pwndShowSoftNess->SetPropValueT("text", szTemp);
	u_pwndShowSoftNess->Update();
}

void		SetMoveMode(BOOL f_mode)		//TRUE ->依照畫面元件  FALSE->依照g_
{
	if(f_mode)
		MoveMode = ((CtmSelectEdit*)u_pwndSelectMoveType)->GetIndex();	//取得目前指定的移動模式
	else
	{
		MoveMode = g_MoveCommandMode;
		((CtmSelectEdit*)u_pwndSelectMoveType)->SetIndex(MoveMode);
	}
	
	if(MoveMode == 0)
	{
		u_pwndTitleSetOne->SetPropValueT("captionID","ROBOT_MSG_COMMAND_MOVE_GOTO");
		u_pwndTitleSetOne->CreateA();
		u_pwndTitleSetOne->Update();
		
		u_pwndTitleSetTwo->SetPropValueT("taborder",-3);
		u_pwndTitleSetTwo->SetPropValueT("captionID"," ");
		u_pwndTitleSetTwo->CreateA();
		u_pwndTitleSetTwo->Update();
		u_pwndTitleSetTwoMark->Update();
		
		u_pwndSelectPointCoord->SetPropValueT("taborder",3);
		u_pwndSelectPointCoord->SetPropValueT("bgc",EditTRUE);
		u_pwndSelectPointCoord->UpdateAll();
	}
	else if(MoveMode == 1)
	{
		u_pwndTitleSetOne->SetPropValueT("captionID","ROBOT_MSG_COMMAND_MOVE_GOTO");
		u_pwndTitleSetOne->CreateA();
		u_pwndTitleSetOne->Update();
		
		u_pwndTitleSetTwo->SetPropValueT("taborder",-3);
		u_pwndTitleSetTwo->SetPropValueT("captionID"," ");
		u_pwndTitleSetTwo->CreateA();
		u_pwndTitleSetTwo->Update();
		u_pwndTitleSetTwoMark->Update();
		
		u_pwndSelectPointCoord->SetPropValueT("taborder",-3);
		u_pwndSelectPointCoord->SetPropValueT("bgc",EditFALSE);
		u_pwndSelectPointCoord->UpdateAll();
	}
	else if(MoveMode == 2)
	{
		u_pwndTitleSetOne->SetPropValueT("captionID","ROBOT_COMMAND_STR_POS1");
		u_pwndTitleSetOne->CreateA();
		u_pwndTitleSetOne->Update();
		
		u_pwndTitleSetTwo->SetPropValueT("taborder",2);
		u_pwndTitleSetTwo->SetPropValueT("captionID","ROBOT_MSG_COMMAND_MOVE_GOTO");
		u_pwndTitleSetTwo->CreateA();
		u_pwndTitleSetTwo->Update();
		
		u_pwndSelectPointCoord->SetPropValueT("taborder",-3);
		u_pwndSelectPointCoord->SetPropValueT("bgc",EditFALSE);
		u_pwndSelectPointCoord->UpdateAll();
	}
	else if(MoveMode == 3)
	{
		u_pwndTitleSetOne->SetPropValueT("captionID","ROBOT_COMMAND_STR_POS1");
		u_pwndTitleSetOne->CreateA();
		u_pwndTitleSetOne->Update();
		
		u_pwndTitleSetTwo->SetPropValueT("taborder",2);
		u_pwndTitleSetTwo->SetPropValueT("captionID","ROBOT_COMMAND_STR_POS2");
		u_pwndTitleSetTwo->CreateA();
		u_pwndTitleSetTwo->Update();
		
		u_pwndSelectPointCoord->SetPropValueT("taborder",-3);
		u_pwndSelectPointCoord->SetPropValueT("bgc",EditFALSE);
		u_pwndSelectPointCoord->UpdateAll();
	}
	
	ClearValue(0);
	PointNum = 1;
	u_DefineMode = 0;
	//SetCheckBox();	
	SetBtnType();
	SetTable();
	
	strcpy(ImagePath1,ImgPath_1[MoveMode*2]);												//設定圖片路徑
	strcpy(ImagePath2,ImgPath_1[MoveMode*2+1]);
	u_time=9;
}

void 	ClearValue(int NO)	//0 :ALL
{
	if(NO == 0)
	{
		u_Point1.CoordinateMode =0;
		u_Point1.DefineMode=0;
		u_Point1.x=0;
		u_Point1.y=0;
		u_Point1.z=0;
		u_Point1.a=0;
		u_Point1.b=0;
		u_Point1.c=0;
		
		u_Point2.CoordinateMode =0;
		u_Point2.DefineMode=0;
		u_Point2.x=0;
		u_Point2.y=0;
		u_Point2.z=0;
		u_Point2.a=0;
		u_Point2.b=0;
		u_Point2.c=0;
	}
	else if(NO == 1)
	{
		u_Point1.CoordinateMode =0;
		u_Point1.DefineMode=0;
		u_Point1.x=0;
		u_Point1.y=0;
		u_Point1.z=0;
		u_Point1.a=0;
		u_Point1.b=0;
		u_Point1.c=0;
	}
	else if(NO == 2)
	{
		u_Point2.CoordinateMode =0;
		u_Point2.DefineMode=0;
		u_Point2.x=0;
		u_Point2.y=0;
		u_Point2.z=0;
		u_Point2.a=0;
		u_Point2.b=0;
		u_Point2.c=0;
	}
}

void	SetTable()
{
	if(PointNum == 1)
	{
		u_pwndTitleSetOne->SetPropValueT("bgc",POINTSEL);
		u_pwndTitleSetOne->Update();
		if(MoveMode!=0 && MoveMode!=1)
		{
			u_pwndTitleSetTwo->SetPropValueT("bgc",POINTNOSEL);
			u_pwndTitleSetTwo->Update();
		}
		temp_POINT = &u_Point1;
	}
	else if(PointNum == 2)
	{
		u_pwndTitleSetOne->SetPropValueT("bgc",POINTNOSEL);
		u_pwndTitleSetOne->Update();
		if(MoveMode!=0 && MoveMode!=1)
		{
			u_pwndTitleSetTwo->SetPropValueT("bgc",POINTSEL);
			u_pwndTitleSetTwo->Update();
		}
		temp_POINT = &u_Point2;
	}
	((CtmSelectEdit*)u_pwndSelectPointCoord)->SetIndex(temp_POINT->CoordinateMode);
	u_pwndSelectPointCoord->UpdateAll();
	SetCoordMode();

	//SetCheckBox();
	SetBtnType();
	ShowValue(temp_POINT->DefineMode/10);
}


void	ClearEdit()
{
	for(int i = 0 ; i < u_pwndEditTableCnt ; i++)
	{
		u_pwndEditTable[i]->SetPropValueT("value",(double)0);
		u_pwndEditTable[i]->Update();
	}
	u_pwndInputCoordNo->SetPropValueT("value",(double)0);
	u_pwndInputCoordNo->Update();
}

void	ShowValue(int f_DefineMode)	//1:自定  2:座標
{
	if(f_DefineMode == 0)
	{
		u_pwndInputCoordNo->SetPropValueT("value",(double)temp_POINT->x);
		u_pwndInputCoordNo->Update();
	}
	else if(f_DefineMode == 1)
	{
		int value_num=cfg.cn;
		if(temp_POINT->CoordinateMode==0) value_num=cfg.cn;
		else if(temp_POINT->CoordinateMode==1) value_num=cfg.jn;
		switch(value_num)
		{
			case 6:
			u_pwndEditTable[5]->SetPropValueT("value",(double)temp_POINT->c);
			u_pwndEditTable[5]->Update();
			case 5:
			u_pwndEditTable[4]->SetPropValueT("value",(double)temp_POINT->b);
			u_pwndEditTable[4]->Update();
			case 4:
			u_pwndEditTable[3]->SetPropValueT("value",(double)temp_POINT->a);
			u_pwndEditTable[3]->Update();
			case 3:
			u_pwndEditTable[2]->SetPropValueT("value",(double)temp_POINT->z);
			u_pwndEditTable[2]->Update();
			case 2:
			u_pwndEditTable[1]->SetPropValueT("value",(double)temp_POINT->y);
			u_pwndEditTable[1]->Update();
			case 1:
			u_pwndEditTable[0]->SetPropValueT("value",(double)temp_POINT->x);
			u_pwndEditTable[0]->Update();
		  break;
		}
		//u_pwndEditTable[cfg.jn]->SetPropValueT("value",(double)temp_POINT->p);
		//u_pwndEditTable[cfg.jn]->Update();
	}
}

void		SaveInputValue()		//儲存Input元件內的值
{
	//temp_POINT->CoordinateMode = ((CtmSelectEdit*)u_pwndSelectPointCoord)->GetIndex();
	
	if(u_DefineMode == 0)
	{
		u_pwndInputCoordNo->GetPropValueT("value",&(temp_POINT->x),sizeof(int));
	}
	else if(u_DefineMode == 1)
	{
		int value_num=cfg.cn;
		if(temp_POINT->CoordinateMode==0) value_num=cfg.cn;
		else if(temp_POINT->CoordinateMode==1) value_num=cfg.jn;
		switch(value_num)
		{
			case 6:
			u_pwndEditTable[5]->GetPropValueT("value",&(temp_POINT->c),sizeof(int));
			case 5:
			u_pwndEditTable[4]->GetPropValueT("value",&(temp_POINT->b),sizeof(int));
			case 4:
			u_pwndEditTable[3]->GetPropValueT("value",&(temp_POINT->a),sizeof(int));
			case 3:
			u_pwndEditTable[2]->GetPropValueT("value",&(temp_POINT->z),sizeof(int));
			case 2:
			u_pwndEditTable[1]->GetPropValueT("value",&(temp_POINT->y),sizeof(int));
			case 1:
			u_pwndEditTable[0]->GetPropValueT("value",&(temp_POINT->x),sizeof(int));
			break;
		}
		//u_pwndEditTable[value_num]->GetPropValueT("value",&(temp_POINT->p),sizeof(int));
	}
	
}

//void	SetCheckBox()
//{
	/*
	ClearEdit();
	
	if(temp_POINT->DefineMode/10 == 0)
	{
		u_DefineMode = 0;
		BOOL	temp_TRUE = TRUE;
		u_pwndCheckBoxPointType01->SetPropValueT("checked",temp_TRUE);
		u_pwndCheckBoxPointType02->SetPropValueT("checked",!temp_TRUE);
		u_pwndCheckBoxPointType01->Update();
		u_pwndCheckBoxPointType02->Update();
		
		((CtmSelectEdit*)u_pwndSelectEditDefine)->SetIndex(temp_POINT->DefineMode);
		u_pwndSelectEditDefine->UpdateAll();
		u_pwndSelectEditDefine->SetPropValueT("taborder",6);
		u_pwndSelectEditDefine->SetPropValueT("bgc",EditTRUE);
		u_pwndSelectEditDefine->UpdateAll();
		
		u_pwndInputCoordNo->SetPropValueT("taborder",7);
		u_pwndInputCoordNo->SetPropValueT("bgc",EditTRUE);
		u_pwndInputCoordNo->Update();
		
		for(int i = 0 ; i < u_pwndEditTableCnt ; i ++)
		{
			u_pwndEditTable[i]->SetPropValueT("taborder",-3);
			u_pwndEditTable[i]->SetPropValueT("bgc",EditFALSE);
			u_pwndEditTable[i]->Update();
		}
		
	}
	else if(temp_POINT->DefineMode/10 == 1)
	{
		u_DefineMode = 1;
		BOOL	temp_TRUE = TRUE;
		u_pwndCheckBoxPointType01->SetPropValueT("checked",!temp_TRUE);
		u_pwndCheckBoxPointType02->SetPropValueT("checked",temp_TRUE);
		u_pwndCheckBoxPointType01->Update();
		u_pwndCheckBoxPointType02->Update();

		((CtmSelectEdit*)u_pwndSelectEditDefine)->SetIndex(0);
		u_pwndSelectEditDefine->UpdateAll();
		u_pwndSelectEditDefine->SetPropValueT("taborder",-3);
		u_pwndSelectEditDefine->SetPropValueT("bgc",EditFALSE);
		u_pwndSelectEditDefine->UpdateAll();
		
		u_pwndInputCoordNo->SetPropValueT("taborder",-3);
		u_pwndInputCoordNo->SetPropValueT("bgc",EditFALSE);
		u_pwndInputCoordNo->Update();
		
		for(int i = 0 ; i < u_pwndEditTableCnt-1 ; i ++)
		{
			u_pwndEditTable[i]->SetPropValueT("taborder",8+i);
			u_pwndEditTable[i]->SetPropValueT("bgc",EditTRUE);
			u_pwndEditTable[i]->Update();
		}
		if(temp_POINT->CoordinateMode == 1)
		{
			u_pwndEditTable[cfg.cn]->SetPropValueT("taborder",-3);
			u_pwndEditTable[cfg.cn]->SetPropValueT("bgc",EditFALSE);
			u_pwndEditTable[cfg.cn]->Update();
		}
	}
	*/
//}

void	SetBtnType()
{
	ClearEdit();
		
	if(temp_POINT->DefineMode/10 == 0)
	{
		u_DefineMode = 0;
		//BOOL	temp_TRUE = TRUE;
		u_pwndBtnPointType01->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnPointType02->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnPointType01->Update();
		u_pwndBtnPointType02->Update();
		
		((CtmSelectEdit*)u_pwndSelectEditDefine)->SetIndex(temp_POINT->DefineMode);
		u_pwndSelectEditDefine->UpdateAll();
		u_pwndSelectEditDefine->SetPropValueT("taborder",6);
		u_pwndSelectEditDefine->SetPropValueT("bgc",EditTRUE);
		u_pwndSelectEditDefine->UpdateAll();
		
		u_pwndInputCoordNo->SetPropValueT("taborder",7);
		u_pwndInputCoordNo->SetPropValueT("bgc",EditTRUE);
		u_pwndInputCoordNo->Update();
		
		for(int i = 0 ; i < u_pwndEditTableCnt ; i ++)
		{
			u_pwndEditTable[i]->SetPropValueT("taborder",-3);
			u_pwndEditTable[i]->SetPropValueT("bgc",EditFALSE);
			u_pwndEditTable[i]->Update();
		}
		
	}
	else if(temp_POINT->DefineMode/10 == 1)
	{
		u_DefineMode = 1;
		//BOOL	temp_TRUE = TRUE;
		u_pwndBtnPointType01->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnPointType02->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnPointType01->Update();
		u_pwndBtnPointType02->Update();
		
		((CtmSelectEdit*)u_pwndSelectEditDefine)->SetIndex(0);
		u_pwndSelectEditDefine->UpdateAll();
		u_pwndSelectEditDefine->SetPropValueT("taborder",-3);
		u_pwndSelectEditDefine->SetPropValueT("bgc",EditFALSE);
		u_pwndSelectEditDefine->UpdateAll();
		
		u_pwndInputCoordNo->SetPropValueT("taborder",-3);
		u_pwndInputCoordNo->SetPropValueT("bgc",EditFALSE);
		u_pwndInputCoordNo->Update();
		
		int edit_num;
		if(temp_POINT->CoordinateMode == 1)
			edit_num=cfg.cn;
		else
			edit_num=cfg.jn;
		for(int i = 0 ; i < edit_num ; i ++)
		{
			u_pwndEditTable[i]->SetPropValueT("taborder",8+i);
			u_pwndEditTable[i]->SetPropValueT("bgc",EditTRUE);
			u_pwndEditTable[i]->Update();
		}
		
	}
	
}

void 		SetCoordMode()
{
//
	if(temp_POINT->CoordinateMode == 0)
	{
		u_pwndSelectEditDefine->SetPropValueT("taborder",-3);
		u_pwndSelectEditDefine = NULL;
		u_pwndSelectEditDefine= this_pwn->FindControlFromName("SelectEditDefineModeC");
		u_pwndSelectEditDefine->SetPropValueT("taborder",6);
		u_pwndSelectEditDefine->SetPropValueT("bgc",EditTRUE);
		u_pwndSelectEditDefine->CreateA();
		u_pwndSelectEditDefine->UpdateAll();
	}
	else if(temp_POINT->CoordinateMode == 1) 	
	{
		u_pwndSelectEditDefine->SetPropValueT("taborder",-3);
		u_pwndSelectEditDefine = NULL;
		u_pwndSelectEditDefine= this_pwn->FindControlFromName("SelectEditDefineModeJ");
		u_pwndSelectEditDefine->SetPropValueT("taborder",6);
		u_pwndSelectEditDefine->SetPropValueT("bgc",EditTRUE);
		u_pwndSelectEditDefine->CreateA();
		u_pwndSelectEditDefine->UpdateAll();
	}

//Edit 的屬性
	for(int i=3 ; i<u_pwndEditTableCnt ; i++)
	{
		//if(i < 3)
		//{
			if(temp_POINT->CoordinateMode == 0)
			{
				u_pwndEditTable[i]->SetPropValueT("precision",3+UnitMode);
				u_pwndEditTable[i]->SetPropValueT("max",180000);
				u_pwndEditTable[i]->SetPropValueT("min",-180000);
			}
			else if (temp_POINT->CoordinateMode == 1)
			{
				u_pwndEditTable[i]->SetPropValueT("precision",3);
				u_pwndEditTable[i]->SetPropValueT("max",9999999);
				u_pwndEditTable[i]->SetPropValueT("min",-9999999);
			}
			u_pwndEditTable[i]->Update();
		//}
	}

//Edit 前的標示
	for(int i = 0 ; i < u_pwndEditStaticCnt ; i++ )
	{
		if(temp_POINT->CoordinateMode == 0)
			u_pwndEditStatic[i]->SetPropValueT("textID",cfg.c_sid[i]);
		else if (temp_POINT->CoordinateMode == 1)
			u_pwndEditStatic[i]->SetPropValueT("textID",cfg.j_sid[i]);	
		
		u_pwndEditStatic[i]->Update();
	}
//Edit後的單位
	if(temp_POINT->CoordinateMode == 0)
	{
		for(int i = 0 ; i < 3 ; i++ )
		{
			if(UnitMode)
				u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
			else
				u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
			u_pwndStaticUnit[i]->Update();
		}
	}
	else if (temp_POINT->CoordinateMode == 1)
	{
		for(int i = 0 ; i < 3 ; i++ )
		{
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_DEG");
			u_pwndStaticUnit[i]->Update();
		}
	}
}

void 		Initialize()				//初始化
{
	SetSpeedValue();
	//SetMoveMode();
}

void		SetSpeedValue()			//設定速度的初始值
{
	speed =10;
	char	temp[15] 		= "\0";
	char	szTemp[15]		= "\0";
	sprintf(temp, "%d%s", speed, "%");
	CodeChange(szTemp, temp);
	u_pwndShowSpeed->SetPropValueT("text", szTemp);
	u_pwndShowSpeed->Update();
	
	nearplace=3;
	memset(temp,0,sizeof(temp));
	memset(szTemp,0,sizeof(szTemp));
	sprintf(temp, "%d\0", nearplace);
	CodeChange(szTemp, temp);
	u_pwndShowSoftNess->SetPropValueT("text", szTemp);
	u_pwndShowSoftNess->Update();
	/*u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_CLOSE");
	u_pwndBtnNearPlace->CreateA();
	u_pwndBtnNearPlace->Update();*/
}


void	SortOutCommand()
{

	double temp_in = 10000/g_Metric_Imperial_Unit;
	double temp_mm = 1000;
	char temp[16] ="\0";
	if(MoveMode == 0)
		sprintf(CommandStr,"PTP_ABS");
	else	if(MoveMode == 1)
		sprintf(CommandStr,"LINE_ABS");
	else	if(MoveMode == 2)
		sprintf(CommandStr,"CIRCLE_ABS");
	else	if(MoveMode == 3)
		sprintf(CommandStr,"CIRC_F_ABS");

	if(u_Point1.CoordinateMode == 0)	//C
	{
		if(u_Point1.DefineMode / 10 == 0)	//
		{
			if(u_Point1.DefineMode == 0)
				sprintf(CommandStr,"%s P%d",CommandStr,u_Point1.x);
			else if(u_Point1.DefineMode == 1)
				sprintf(CommandStr,"%s C%d",CommandStr,u_Point1.x);
		}
		else if(u_Point1.DefineMode / 10 == 1)
		{
			sprintf(CommandStr,"%s (",CommandStr);
			for(int i = 0 ; i < (cfg.cn) ;i++)
			{
				int temp_value=0; //= (int)temp;
				memcpy(&temp_value,(&u_Point1.x)+1*i,sizeof(temp_value));
				if(i<3 && UnitMode)//coord
					temp_value = temp_value/temp_in;
				else if(i<3 && !UnitMode)//coord
					temp_value = temp_value/temp_mm;
				else
					temp_value = temp_value/1000;
				sprintf(temp,"%.3f",(double)temp_value);
				ClearCoordZeor(temp,strlen(temp));
				if(i==0)
					sprintf(CommandStr,"%s%s=%s",CommandStr,cfg.c_s[i],temp);
				else
					sprintf(CommandStr,"%s %s=%s",CommandStr,cfg.c_s[i],temp);
			}
			sprintf(CommandStr,"%s)",CommandStr);
		}
	}
	else if(u_Point1.CoordinateMode == 1)
	{
		if(u_Point1.DefineMode / 10 == 0)	//
		{
			sprintf(CommandStr,"%s J%d",CommandStr,u_Point1.x);
		}
		else if(u_Point1.DefineMode / 10 == 1)
		{
			sprintf(CommandStr,"%s (",CommandStr);
			
			for(int i = 0 ; i < (cfg.jn) ;i++)
			{
				int temp_value=0; //= (int)temp;
				memcpy(&temp_value,(&u_Point1.x)+1*i,sizeof(temp_value));
				sprintf(temp,"%.3f",(double)temp_value/1000);
				ClearCoordZeor(temp,strlen(temp));
				if(i==0)
					sprintf(CommandStr,"%s%s=%s",CommandStr,cfg.j_s[i],temp);
				else
					sprintf(CommandStr,"%s %s=%s",CommandStr,cfg.j_s[i],temp);
			}
			sprintf(CommandStr,"%s)",CommandStr);
		}
	}
	//}
	
	if(MoveMode == 2 || MoveMode == 3)
	{
		if(u_Point2.CoordinateMode == 0)	//C
		{
			if(u_Point2.DefineMode / 10 == 0)	//
			{
				if(u_Point2.DefineMode == 0)
					sprintf(CommandStr,"%s P%d",CommandStr,u_Point2.x);
				else if(u_Point2.DefineMode == 1)
					sprintf(CommandStr,"%s C%d",CommandStr,u_Point2.x);
			}
			else if(u_Point2.DefineMode / 10 == 1)
			{
				sprintf(CommandStr,"%s (",CommandStr);
				
				for(int i = 0 ; i < (cfg.cn) ;i++)
				{
					int temp_value=0; //= (int)temp;
					memcpy(&temp_value,(&u_Point2.x)+1*i,sizeof(temp_value));
					if(i<3 && UnitMode)//coord
						temp_value = temp_value/temp_in;
					else if(i<3 && !UnitMode)//coord
						temp_value = temp_value/temp_mm;
					else
						temp_value = temp_value/1000;
					sprintf(temp,"%.3f",(double)temp_value);
					ClearCoordZeor(temp,strlen(temp));
					if(i==0)
						sprintf(CommandStr,"%s%s=%s",CommandStr,cfg.c_s[i],temp);
					else
						sprintf(CommandStr,"%s %s=%s",CommandStr,cfg.c_s[i],temp);
				}
				sprintf(CommandStr,"%s)",CommandStr);
			}
		}
		else if(u_Point2.CoordinateMode == 1)
		{
			if(u_Point2.DefineMode / 10 == 0)	//
			{
				sprintf(CommandStr,"%s J%d",CommandStr,u_Point2.x);
			}
			else if(u_Point2.DefineMode / 10 == 1)
			{
				sprintf(CommandStr,"%s (",CommandStr);
				
				for(int i = 0 ; i < (cfg.jn) ;i++)
				{
					int temp_value=0; //= (int)temp;
					memcpy(&temp_value,(&u_Point2.x)+1*i,sizeof(temp_value));
					sprintf(temp,"%.3f",(double)temp_value/1000);
					ClearCoordZeor(temp,strlen(temp));
					if(i==0)
						sprintf(CommandStr,"%s%s=%s",CommandStr,cfg.j_s[i],temp);
					else
						sprintf(CommandStr,"%s %s=%s",CommandStr,cfg.j_s[i],temp);
				}
				sprintf(CommandStr,"%s)",CommandStr);
			}
		}
	}
	//if(nearplace)
		sprintf(CommandStr,"%s S%d F%d;",CommandStr,speed,nearplace);
	//else
	//	sprintf(CommandStr,"%s S%d F0;",CommandStr,speed);
}

void		ClearCoordZeor(char *Coord,int Num)					//整理座標 去除小數點後多餘0
{
	for(int i =Num-1 ; i > 0 ; i--)
	{
		if( *(Coord+i) == '0')
			*(Coord+i)=NULL;
		else if(*(Coord+i) == '.' )
		{
			*(Coord+i)=NULL;
			break;
		}
		else
			break;
	}
}

BOOL	RevertMoveCommand(char *Command)
{
	//==================還原所需參數==================================================//
	/**/int  R_CoordType = 0;  //0:C 1:J                														/**/
	/**/long R_CoordValue[7] = {0,0,0,0,0,0,0};  //座標值，[0],[1]亦可當NO類型與值  /**/
	/**/int  R_CoordInputMode = 0; //輸入座標模式       														/**/
	/**/int  R_Speed = 0; //速度                																		/**/
	/**/int  R_NearPlace = 0; //接近到位        														    /**/
	//================================================================================//	
	
	//============circ需要第二組還原所需參數============================================//
	/**/int  R2_CoordType = 0;  //0:C 1:J                															/**/
	/**/long R2_CoordValue[7] = {0,0,0,0,0,0,0};  //座標值，[0],[1]亦可當NO類型與值  	/**/
	/**/int  R2_CoordInputMode = 0; //輸入座標模式       															/**/
	/**///int  R2_Speed = 0; //速度                																		/**/
	/**///BOOL R2_NearPlace = FALSE; //接近到位        														    /**/
	//==================================================================================//
	
	char *ptr = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	while(*ptr != ' '&& *ptr !='(') ptr++;//前進到參數開始的地方('('或'P'或'C'或'J')
	if(*ptr !='(')
		ptr = PassSpace(ptr);	
	if(MoveMode == 0)
	{
		char *temp = NULL;
		if(*ptr == '(')
		{
			R_CoordInputMode = 1;
			if(strstr(ptr,"X")!=NULL)
			{
				R_CoordType = 0;
				for(int i=0 ; i<(cfg.jn+1) ; i++)
				{
					temp = strstr(ptr,cfg.c_s[i]);
					if(temp == NULL) return FALSE;
					temp = PassSpaceFindChar(temp+strlen(cfg.c_s[i]),'=');
					if(temp == NULL) return FALSE;
					temp++;
					if(ISValue(temp) && *(temp) != '+' )
					{
						if(i < cfg.jn)
						{
							float CoordValue = atof(temp);
							R_CoordValue[i] = (long)(CoordValue*1000);
						}
						//else
						//	R_CoordValue[6] = atoi(temp);
					}
					else
						return FALSE;
				}
			}
			else if(strstr(ptr,"J1")!=NULL)
			{
				R_CoordType = 1;
				for(int i=0 ; i<cfg.jn ; i++)
				{
					temp = strstr(ptr,cfg.j_s[i]);
					if(temp == NULL) return FALSE;
					temp = PassSpaceFindChar(temp+strlen(cfg.j_s[i]),'=');
					if(temp == NULL) return FALSE;
					temp++;
					if(ISValue(temp) && *(temp) != '+' )
					{
							float CoordValue = atof(temp);
							R_CoordValue[i] = (long)(CoordValue*1000);
					}
					else
						return FALSE;
				}
			}
			else return FALSE;
			
			temp = strstr(ptr,")");
			if(temp==NULL)  return FALSE;
		}
		else if(*ptr == 'P' || *ptr == 'C' || *ptr == 'J')
		{
			R_CoordInputMode = 0;
			if(*ptr == 'P')
			{
				R_CoordType=0;
				R_CoordValue[0]=0;
			}
			else if(*ptr == 'C')
			{
				R_CoordType=0;
				R_CoordValue[0]=1;
			}
			else if(*ptr == 'J')
			{
				R_CoordType=1;
				R_CoordValue[0]=0;
			}
			ptr++;
			if(isdigit(*ptr))
				R_CoordValue[1] = atoi(ptr);
			else return FALSE;
			
			while(isdigit(*ptr)) ptr++;
			temp = ptr;
		}
		else return FALSE;
			
		
		ptr = temp+1;
		temp= strstr(ptr,"S");
		if(temp == NULL) return FALSE;
		temp +=1;
		if(!ISValue(temp)) return FALSE;
		R_Speed = atoi(temp); 
		temp =strstr(ptr,"F"); 
		if(temp == NULL) return FALSE;
		temp +=1;
		if(!ISValue(temp)) return FALSE;
		R_NearPlace=atoi(temp);
		
		u_Point1.CoordinateMode = R_CoordType;
		if(R_CoordInputMode != 0)
		{
			u_Point1.DefineMode=R_CoordInputMode*10;
			u_Point1.x=R_CoordValue[0];
			u_Point1.y=R_CoordValue[1];
			u_Point1.z=R_CoordValue[2];
			u_Point1.a=R_CoordValue[3];
			u_Point1.b=R_CoordValue[4];
			u_Point1.c=R_CoordValue[5];
			//u_Point1.p=R_CoordValue[6];
		}
		else 
		{
			u_Point1.DefineMode = R_CoordValue[0];
			u_Point1.x=R_CoordValue[1];
			
		}
		speed =R_Speed;
		nearplace = R_NearPlace;
	}
	else if(MoveMode == 1)
	{
		char *temp = NULL;
		if(*ptr == '(')
		{
			R_CoordInputMode = 1;
			if(strstr(ptr,"X")!=NULL)
			{
				R_CoordType = 0;
				for(int i=0 ; i<(cfg.jn) ; i++)
				{
					temp = strstr(ptr,cfg.c_s[i]);
					if(temp == NULL) return FALSE;
					temp = PassSpaceFindChar(temp+strlen(cfg.c_s[i]),'=');
					if(temp == NULL) return FALSE;
					temp++;
					if(ISValue(temp) && *(temp) != '+' )
					{
						if(i < cfg.jn)
						{
							float CoordValue = atof(temp);
							R_CoordValue[i] = (long)(CoordValue*1000);
						}
						//else
						//	R_CoordValue[6] = atoi(temp);
					}
					else
						return FALSE;
				}
			}
			else return FALSE;
			
			temp = strstr(ptr,")");
			if(temp==NULL)  return FALSE;
		}
		//else if(*ptr == 'P' || *ptr == 'C' || *ptr == 'J')
		else if(*ptr == 'P' || *ptr == 'C')
		{
			R_CoordInputMode = 0;
			if(*ptr == 'P')
			{
				R_CoordType=0;
				R_CoordValue[0]=0;
			}
			else if(*ptr == 'C')
			{
				R_CoordType=0;
				R_CoordValue[0]=1;
			}
			//else if(*ptr == 'J')
			//{
			//	R_CoordType=1;
			//	R_CoordValue[0]=0;
			//}
			ptr++;
			if(isdigit(*ptr))
				R_CoordValue[1] = atoi(ptr);
			else return FALSE;
			
			while(isdigit(*ptr)) ptr++;
			temp = ptr;
		}
		else return FALSE;
			
		
		ptr = temp+1;
		temp= strstr(ptr,"S");
		if(temp == NULL) return FALSE;
		temp +=1;
		if(!ISValue(temp)) return FALSE;
		R_Speed = atoi(temp); 
		temp =strstr(ptr,"F"); 
		if(temp == NULL) return FALSE;
		temp +=1;
		if(!ISValue(temp)) return FALSE;
		R_NearPlace=atoi(temp);
		
		
		
		u_Point1.CoordinateMode = R_CoordType;
		if(R_CoordInputMode != 0)
		{
			u_Point1.DefineMode=R_CoordInputMode*10;
			u_Point1.x=R_CoordValue[0];
			u_Point1.y=R_CoordValue[1];
			u_Point1.z=R_CoordValue[2];
			u_Point1.a=R_CoordValue[3];
			u_Point1.b=R_CoordValue[4];
			u_Point1.c=R_CoordValue[5];
			//u_Point1.p=R_CoordValue[6];
		}
		else 
		{
			u_Point1.DefineMode = R_CoordValue[0];
			u_Point1.x=R_CoordValue[1];
			
		}
		speed =R_Speed;
		nearplace = R_NearPlace;
	}
	else if(MoveMode == 2)
	{
		char *temp = NULL;
		//Point1
		if(*ptr == '(')
		{
			R_CoordInputMode = 1;
			if(strstr(ptr,"X")!=NULL)
			{
				R_CoordType = 0;
				for(int i=0 ; i<(cfg.jn) ; i++)
				{
					temp = strstr(ptr,cfg.c_s[i]);
					if(temp == NULL) return FALSE;
					temp = PassSpaceFindChar(temp+strlen(cfg.c_s[i]),'=');
					if(temp == NULL) return FALSE;
					temp++;
					if(ISValue(temp) && *(temp) != '+' )
					{
						if(i < cfg.jn)
						{
							float CoordValue = atof(temp);
							R_CoordValue[i] = (long)(CoordValue*1000);
						}
						//else
						//	R_CoordValue[6] = atoi(temp);
					}
					else
						return FALSE;
				}
			}
			else return FALSE;
			
			temp = strstr(ptr,")");
			if(temp==NULL)  return FALSE;
		}
		//else if(*ptr == 'P' || *ptr == 'C' || *ptr == 'J')
		else if(*ptr == 'P' || *ptr == 'C')
		{
			R_CoordInputMode = 0;
			if(*ptr == 'P')
			{
				R_CoordType=0;
				R_CoordValue[0]=0;
			}
			else if(*ptr == 'C')
			{
				R_CoordType=0;
				R_CoordValue[0]=1;
			}
			//else if(*ptr == 'J')
			//{
			//	R_CoordType=1;
			//	R_CoordValue[0]=0;
			//}
			ptr++;
			if(isdigit(*ptr))
				R_CoordValue[1] = atoi(ptr);
			else return FALSE;
			
			while(isdigit(*ptr)) ptr++;
			temp = ptr;
		}
		else return FALSE;
		
		ptr = PassSpace(temp+1);	
		
		//char *temp = NULL;
		//Point1
		if(*ptr == '(')
		{
			R2_CoordInputMode = 1;
			if(strstr(ptr,"X")!=NULL)
			{
				for(int i=0 ; i<(cfg.jn) ; i++)
				{
					R2_CoordType = 0;
					temp = strstr(ptr,cfg.c_s[i]);
					if(temp == NULL) return FALSE;
					temp = PassSpaceFindChar(temp+strlen(cfg.c_s[i]),'=');
					if(temp == NULL) return FALSE;
					temp++;
					if(ISValue(temp) && *(temp) != '+' )
					{
						if(i < cfg.jn)
						{
							float CoordValue = atof(temp);
							R2_CoordValue[i] = (long)(CoordValue*1000);
						}
						else
							R2_CoordValue[6] = atoi(temp);
					}
					else
						return FALSE;
				}
			}
			else return FALSE;
			
			temp = strstr(ptr,")");
			if(temp==NULL)  return FALSE;
		}
		//else if(*ptr == 'P' || *ptr == 'C' || *ptr == 'J')
		else if(*ptr == 'P' || *ptr == 'C')
		{
			R2_CoordInputMode = 0;
			if(*ptr == 'P')
			{
				R2_CoordType=0;
				R2_CoordValue[0]=0;
			}
			else if(*ptr == 'C')
			{
				R2_CoordType=0;
				R2_CoordValue[0]=1;
			}
			//else if(*ptr == 'J')
			//{
			//	R2_CoordType=1;
			//	R2_CoordValue[0]=0;
			//}
			ptr++;
			if(isdigit(*ptr))
				R2_CoordValue[1] = atoi(ptr);
			else return FALSE;
			
			while(isdigit(*ptr)) ptr++;
			temp = ptr;
		}
		else return FALSE;
		
		ptr = temp+1;
		temp= strstr(ptr,"S");
		if(temp == NULL) return FALSE;
		temp +=1;
		if(!ISValue(temp)) return FALSE;
		R_Speed = atoi(temp); 
		temp =strstr(ptr,"F"); 
		if(temp == NULL) return FALSE;
		temp +=1;
		if(!ISValue(temp)) return FALSE;
		R_NearPlace=atoi(temp);
		
		
		
		u_Point1.CoordinateMode = R_CoordType;
		if(R_CoordInputMode != 0)
		{
			u_Point1.DefineMode=R_CoordInputMode*10;
			u_Point1.x=R_CoordValue[0];
			u_Point1.y=R_CoordValue[1];
			u_Point1.z=R_CoordValue[2];
			u_Point1.a=R_CoordValue[3];
			u_Point1.b=R_CoordValue[4];
			u_Point1.c=R_CoordValue[5];
			//u_Point1.p=R_CoordValue[6];
		}
		else 
		{
			u_Point1.DefineMode = R_CoordValue[0];
			u_Point1.x=R_CoordValue[1];
			
		}
		
		u_Point2.CoordinateMode = R2_CoordType;
		if(R2_CoordInputMode != 0)
		{
			u_Point2.DefineMode=R2_CoordInputMode*10;
			u_Point2.x=R2_CoordValue[0];
			u_Point2.y=R2_CoordValue[1];
			u_Point2.z=R2_CoordValue[2];
			u_Point2.a=R2_CoordValue[3];
			u_Point2.b=R2_CoordValue[4];
			u_Point2.c=R2_CoordValue[5];
			//u_Point2.p=R2_CoordValue[6];
		}
		else 
		{
			u_Point2.DefineMode = R2_CoordValue[0];
			u_Point2.x=R2_CoordValue[1];
		}
		
		speed =R_Speed;
		nearplace = R_NearPlace;
	}
	UpdateSpeed();
	PointNum = 1;
	strcpy(ImagePath2,ImgPath_1[MoveMode*2+1]);
	SetTable();
	return TRUE;
}