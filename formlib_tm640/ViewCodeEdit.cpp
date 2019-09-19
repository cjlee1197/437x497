/*============================================================================+
|  Class    : ViewCodeEdit library                	                     			|
|  Task     : ViewCodeEdit library  header file                         			|
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
#include	"ViewCodeEdit.h"
#include	<sys/stat.h>
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../selectedit.h"
#include 	"../tmFileExplorer.h"
#include	"../wnd.h"
#include<stdio.h>
#include<fcntl.h> 

#include	"../tmshmsg.h"
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define		MSGBOX_TEACHPOINT						1
#define		MSGBOX_COMMANDEXPLANATION		2
#define 	SIGN(A)  ( (A) >= 0 ? 1: -1 )
#define		COMMANDLISTSTARTTABORDER	5	//指令的光標值

#define		MAXCOMMANDBUTTONCOL 3				//指令列表的按鈕列最大數量
#define		MAXCOMMANDBUTTONLINE 3			//指令列表的按鈕行最大數量
#define		MAXCOMMANDBUTTON 9//MAXCOMMANDBUTTONCOL*MAXCOMMANDBUTTONLINE			//指令列表的按鈕最大數量


#define		KEY_NUM_START 101
#define		KEY_NUM_END 111   //11
#define		KEY_ABC_START 201
#define		KEY_ABC_END  226   //26
#define		KEY_SYMBOL_START 301
#define		KEY_SYMBOL_END 316 //16
#define		KEY_OPER_START 401
#define		KEY_OPER_END 407   //7

/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
char*	CommandModeImageUp[] =			//指令分類標籤Up		排序 : 中文->英文->NULL
{
	"res_tm640/mode1_1.png",			
	"res_tm640/mode2_1.png",			
	"res_tm640/mode3_1.png",			
	"res_tm640/mode4_1.png",		
	"res_tm640/modeC1_1.png",		
	"res_tm640/modeC2_1.png",		
	"res_tm640/modeC3_1.png",	
	"res_tm640/modeC4_1.png",
	NULL
}; 

char*	CommandModeImageDown[] =		//指令分類標籤Down	排序 : 中文->英文->NULL
{
	"res_tm640/mode1_2.png",			
	"res_tm640/mode2_2.png",			
	"res_tm640/mode3_2.png",			
	"res_tm640/mode4_2.png",		
	"res_tm640/modeC1_2.png",		
	"res_tm640/modeC2_2.png",		
	"res_tm640/modeC3_2.png",	
	"res_tm640/modeC4_2.png",
	NULL
}; 


char*	CommandMove[] =								//移動指令字串列表
{
	"ROBOT_COMMAND_PTP",						//PTP
	"ROBOT_COMMAND_LIN",						//LINE
	"ROBOT_COMMAND_CIRC",						//CIRC
	//"ROBOT_COMMAND_CIRCF",					//CIRC_F
	"ROBOT_COMMAND_PTPREL",					//MOV_REL
	"ROBOT_COMMAND_LINREL",					//LIN_REL
	"ROBOT_COMMAND_CIRCREL",				//CIRC_REL
	//"ROBOT_COMMAND_CIRCF_REL",			//CIRC_F_REL
	"ROBOT_COMMAND_PTP_PALLET",			//PTP_PALLET
	"ROBOT_COMMAND_LINE_PALLET",		//LINE_PALLET
	"ROBOT_COMMAND_PATH_BLEND",
	NULL
}; 
char*	CommandDefin[] =						//定義指令字串列表
{
	"ROBOT_COMMAND_SETCOORD",				//SETCOORD
	"ROBOT_COMMAND_SETJOINT",				//SETJOINT
	"ROBOT_COMMAND_COORD_CAL",					//COORDCAL
	"ROBOT_COMMAND_SET_TABLE",			//SETPALLET
	NULL
}; 

char*	CommandLogic[] =						//邏輯指令字串列表
{
//	"ROBOT_COMMAND_IF",							//if
	"ROBOT_COMMAND_GOTO",						//GOTO
	"ROBOT_COMMAND_CALLSUB",				//CALLSUB
	"ROBOT_COMMAND_EDN",					//RETURN
	"ROBOT_COMMAND_RETURN",					//RETURN
	NULL
}; 

char*	CommandOther[] =						//其他指令字串列表
{
	"ROBOT_COMMAND_WAIT",						//WAIT
	"ROBOT_COMMAND_WORKCOORD",			//WORKCOORD
	"ROBOT_COMMAND_OM_SET",					//OM_SET
	"ROBOT_COMMAND_IM_GET",					//IM_GET
	"ROBOT_COMMAND_PAUSE",					//TEST
	"ROBOT_COMMAND_TOOL",						//TOOL
	"ROBOT_COMMAND_OPTION",				//OPT.STOP
	"ROBOT_COMMAND_PALLET_NEXT",			//NEXT_PALLET
	"ROBOT_COMMAND_PALLET_LAST",			//LAST_PALLET
	"ROBOT_COMMAND_PALLET_START",			//FRIST_PALLET
	"ROBOT_COMMAND_SHIFT_PALLET",			//SET_PT_table
	"ROBOT_COMMAND_GET_PALLET",			//GET_PT_table
	NULL
}; 
char** CommandStr[4] =
{
	CommandMove,
	CommandDefin,
	CommandLogic,
	CommandOther
};

//KeyNum
WORD	KeyNum[]=
{
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	48,
	46
};

//KeyABC
WORD	KeyABC[]=
{
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90
};

//Keyother
WORD	KeyOther[]=
{
	43,
	45,
	42,
	47,
	64,
	35,
	38,
	124,
	40,
	41,
	95,
	33,
	61,
	60,
	62,
	59
};

//KeyOper
WORD	KeyOper[]=
{
	26368,
	27648,
	26880,
	27136,
	32,
	7178,
	3592,
};
char*	MM[]=
{
"Jan",
"Feb",
"Mar",
"Apr",
"May",
"Jun",
"Jul",
"Aug",
"Sep",
"Oct",
"Nov",
"Dec",
};
//
CtmWnd*		u_pwndLTextMemoProg			= NULL;
CtmWnd*		u_pwndbtnListUp					= NULL;
CtmWnd*		u_pwndbtnListDown				= NULL;
CtmWnd*		u_pwndStaticEditFileName= NULL;
CtmWnd*		u_pwndFileExplorer					= NULL;
CtmWnd*		u_pwndFileExplorerD					= NULL;
CtmWnd*		u_pwndbtnModeCommand[10]		= {NULL};
CtmWnd*		u_pwndbtnCommand[15]			= {NULL};
CtmWnd*		u_pwndStaticMask					=NULL;
CtmWnd*		u_pwndStaticCntLength			=NULL;
//sel copy del
CtmWnd*		u_pwndEditSelStart			=NULL;
CtmWnd*		u_pwndEditSelEnd				=NULL;
CtmWnd*		u_pwndBtnCopy						=NULL;
CtmWnd*		u_pwndBtnPaste					=NULL;
CtmWnd*		u_pwndBtnDelLine				=NULL;
CtmWnd*		u_pwndStaticModifyDate	=NULL;
CtmWnd*		u_pwndStaticModifyTime	=NULL;


char		u_TitleFileName[64]			="\0";
int			u_nEditTableCnt					=0;
int 		u_nPointTableCnt				=0;
int 		u_nUnitTableCnt					=0;
int 		u_pwndbtnCommandCnt			=0;
int			u_pwndbtnModeCommandCnt	=0;
int 		u_nString_StartNo				=0;
WORD		u_wDBvalue_StartNo			=0;


//keyborad
CtmWnd*		u_pwndbtnKeyBorad[56]				= {NULL};
int 			u_pwndbtnKeyBoradCnt				=0;
CtmWnd*		u_pwndkeyboradMask					=NULL;
CtmWnd*		u_pwndkeyboradBackImage			=NULL;

BOOL		u_bEnterKey							= FALSE;
BOOL		u_bUnitConversion				= TRUE;		//TRUE =英制；FALSE =公制

int 		ComMode									=0;			//當前指令模式
int 		ListStart								=1;			//指令清單目前由第幾個開始
int 		ListCount								=0;			//指令清單內的指令總數
double	ListCntHeight						=0;				//ListCntLength
char 		u_szMem_Part_File[192]		= "\0";		//開啟的檔案路徑				

BOOL		WorkOne									=TRUE;		//進入畫面時自動執行，僅一次
BOOL 		EditIsWorkFile	= FALSE;
BOOL 		OnAuto	= FALSE;

int* sel_start=NULL;
int* sel_end=NULL;
int Memotaborder=-1;
//2015/6/15 下午 02:39:55
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{ 
	GetDBString("CNC_EDIT_PATH",u_szMem_Part_File, sizeof(u_szMem_Part_File)); 
	//strcpy(g_Now_Edit_FileName , u_szMem_Part_File);
	
	//抓取各元件的指針
	u_pwndLTextMemoProg 	= pwndSender->FindControlFromName("LTextMemoProg");
	u_pwndStaticEditFileName= pwndSender->FindControlFromName("StaticEditFileNmae");
	u_pwndStaticMask= pwndSender->FindControlFromName("mask");
	u_pwndStaticCntLength	= pwndSender->FindControlFromName("CntLength");
	u_pwndbtnListUp		= pwndSender->FindControlFromName("btnListUp");
	u_pwndbtnListDown	= pwndSender->FindControlFromName("btnListDown");
	
	
	u_pwndbtnModeCommandCnt = GetSpecialControlNum(pwndSender, "btnModeCommand", "CtmToolButton", u_pwndbtnModeCommand);
	u_pwndbtnCommandCnt = GetSpecialControlNum(pwndSender, "btnCommand", "CtmToolButton", u_pwndbtnCommand);
	
	//KeyBorad
	u_pwndbtnKeyBoradCnt = GetSpecialControlNum(pwndSender, "BtnKey", "CtmToolButton", u_pwndbtnKeyBorad);
	u_pwndkeyboradMask	= pwndSender->FindControlFromName("keymask");
	u_pwndkeyboradBackImage	= pwndSender->FindControlFromName("keybackimage");
	
	//sel copy del
	u_pwndEditSelStart			= pwndSender->FindControlFromName("SelStart");
	u_pwndEditSelEnd				= pwndSender->FindControlFromName("SelEnd");
	u_pwndBtnCopy						= pwndSender->FindControlFromName("btnCopy");
	u_pwndBtnPaste					= pwndSender->FindControlFromName("btnPatse");
	u_pwndBtnDelLine				= pwndSender->FindControlFromName("btnDelLine");
	
	u_pwndStaticModifyDate	= pwndSender->FindControlFromName("ModifyDate");
	u_pwndStaticModifyTime	= pwndSender->FindControlFromName("ModifyTime");
	
	sel_start = ((CtmMTextMemo*)u_pwndLTextMemoProg)->SetSelectStartPtr();
	sel_end = ((CtmMTextMemo*)u_pwndLTextMemoProg)->SetSelectEndPtr();
		
	if(file_exists("temp/temp"))
	{
		ShowProgram(0,FALSE);
		sprintf(u_TitleFileName,"%s*\0",u_szMem_Part_File);
	}
	else
	{
		ShowProgram(0,TRUE);
		sprintf(u_TitleFileName,"%s\0",u_szMem_Part_File);
	}
	SetFileInfo(u_TitleFileName);
	RobotStatus = OP_EDIT_MODE;
	((CtmMTextMemo*)u_pwndLTextMemoProg)->OnLoseFocus(); 	
	return	TRUE;            	
}


WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp = NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	/* //sel copy del
	u_pwndEditSelStart			= pwndSender->FindControlFromName("SelStart");
	u_pwndEditSelEnd				= pwndSender->FindControlFromName("SelEnd");
	u_pwndBtnCopy						= pwndSender->FindControlFromName("btnCopy")
	u_pwndBtnPaste					= pwndSender->FindControlFromName("btnPatse")
	u_pwndBtnDelLine				= pwndSender->FindControlFromName("btnDelLine")*/
	
	
	if(pwndTemp == u_pwndbtnListUp)		
	{
		if(ListStart-MAXCOMMANDBUTTONCOL > 0)
		{
			ListStart-=MAXCOMMANDBUTTONCOL;
			CommandListShow();
		}
	}
	else if(pwndTemp == u_pwndBtnCopy)	
	{
		if(Memotaborder!=-1 && ((CtmFormView*)pwndSender)->Get_TabStop()!=Memotaborder) 
			((CtmFormView*)pwndSender)->Goto(Memotaborder);
		((CtmMTextMemo*)u_pwndLTextMemoProg)->CopyLine();
	}
	else if(pwndTemp == u_pwndBtnPaste)	
	{
		if(Memotaborder!=-1 && ((CtmFormView*)pwndSender)->Get_TabStop()!=Memotaborder) 
			((CtmFormView*)pwndSender)->Goto(Memotaborder);
		((CtmMTextMemo*)u_pwndLTextMemoProg)->PatseLine();
	}
	else if(pwndTemp == u_pwndBtnDelLine)	
	{
		if(Memotaborder!=-1 && ((CtmFormView*)pwndSender)->Get_TabStop()!=Memotaborder) 
			((CtmFormView*)pwndSender)->Goto(Memotaborder);
		int sel[2];
		u_pwndEditSelStart->GetPropValueT("value", &(sel[0]) , sizeof(int));
		u_pwndEditSelEnd->GetPropValueT("value", &(sel[1]) , sizeof(int));
		((CtmMTextMemo*)u_pwndLTextMemoProg)->DelRow_New(sel[0]-1,sel[1]-1);
		((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();	
	}
	else if(pwndTemp == u_pwndbtnListDown)		
	{
		if(ListStart+MAXCOMMANDBUTTON-1 < ListCount)
		{
			ListStart+=MAXCOMMANDBUTTONCOL;
			CommandListShow();
		}
	}
	else if(KEY_NUM_START <= wIDControl && wIDControl <= KEY_NUM_END)
		u_pwndLTextMemoProg->OnKey(KeyNum[wIDControl-KEY_NUM_START]);
	else if(KEY_ABC_START <= wIDControl && wIDControl <= KEY_ABC_END)
		u_pwndLTextMemoProg->OnKey(KeyABC[wIDControl-KEY_ABC_START]);
	else if(KEY_SYMBOL_START <= wIDControl && wIDControl <= KEY_SYMBOL_END)
		u_pwndLTextMemoProg->OnKey(KeyOther[wIDControl-KEY_SYMBOL_START]);
	else if(KEY_OPER_START <= wIDControl && wIDControl <= KEY_OPER_END)
		u_pwndLTextMemoProg->OnKey(KeyOper[wIDControl-KEY_OPER_START]);
	else
	{
		for(int i=0 ; i<u_pwndbtnModeCommandCnt ; i++ )
		{
			if(pwndTemp == u_pwndbtnModeCommand[i] && ComMode != i)		
			{
				SetCommandMode(i);
			}
		}
		for(int i=0 ; i<u_pwndbtnCommandCnt ; i++ )
		{
			if(pwndTemp == u_pwndbtnCommand[i] )		
			{
				if(EditIsWorkFile && OnAuto)
					MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
				else
				{
					remove("./DialogHelp/buffer");
					CallCommandWork(ComMode , i+ListStart-1);
					AddNewCommand();
				}
			}
		}
	}

	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	if(wKey == _ENTER)
	{
		if(u_pwndEditSelStart == pwndTemp)
		{
			int sel;
			u_pwndEditSelStart->GetPropValueT("value", &sel , sizeof(int));
			int flag = ((CtmMTextMemo*)u_pwndLTextMemoProg)->SetSelectStartLine(sel-1);
			printf("flag = %d\n",flag);
			if(flag>0)
			{
				u_pwndEditSelStart->SetPropValueT("value",flag);
				u_pwndEditSelStart->Update();
			}
			if(Memotaborder!=-1 && ((CtmFormView*)pwndSender)->Get_TabStop()!=Memotaborder) 
				((CtmFormView*)pwndSender)->Goto(Memotaborder);
		}
		else if(u_pwndEditSelEnd == pwndTemp)
		{
			int sel;
			u_pwndEditSelEnd->GetPropValueT("value", &sel , sizeof(int));
			int flag = ((CtmMTextMemo*)u_pwndLTextMemoProg)->SetSelectEndLine(sel-1);
			printf("flag = %d\n",flag);
			if(flag>0)
			{
				u_pwndEditSelEnd->SetPropValueT("value",flag);
				u_pwndEditSelEnd->Update();
			}
			if(Memotaborder!=-1 && ((CtmFormView*)pwndSender)->Get_TabStop()!=Memotaborder) 
				((CtmFormView*)pwndSender)->Goto(Memotaborder);
		}
	}
}
void		OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
	{
		switch(wParam)
		{
			case	_F3:
				SaveProgram(FALSE);	
				if(FileCmy())
				{
					system("rm -r temp/temp");
					system("sync");
				}
				MsgBoxCall("msgboxFile.txt");		
				if(g_Now_FileName[0] != '\0')
					ChangeFile(pwndSender);
				else
					((CtmFormView*)pwndSender)->OnGetFocus();	
				memset(g_Now_FileName,0,sizeof(g_Now_FileName));
				Now_Push_Btn->CreateA();			
				Now_Push_Btn->UpdateAll();		
				Now_Push_Btn->Update();				
			break;
			case	_F4:
			{
				if(OnAuto && EditIsWorkFile)
					MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
				char	*str = NULL;
				int CommandMode,CommandNO;
				str= (char*) malloc(INT_BUFFER_SIZE*sizeof(char));
				memset(str,0,INT_BUFFER_SIZE*sizeof(char));
				RevertCommand = str;
				((CtmMTextMemo*)u_pwndLTextMemoProg)->GetStr(RevertCommand);
				char TempCommand[INT_BUFFER_SIZE];
				strcpy(TempCommand,RevertCommand);
				CommandMode = CallModifyCommand(TempCommand);
				CommandNO 	= CommandMode%1000;
				CommandMode = CommandMode/1000;
				if(CommandMode > 0 && CommandNO > 0)
				{
					CallCommandWork(CommandMode-1,CommandNO-1);
					ModifyCommand();
				}
				RevertCommand = NULL;
				free(str);
				Now_Push_Btn->CreateA();			
				Now_Push_Btn->UpdateAll();		
				Now_Push_Btn->Update();	
			}
			break;
			case	_F5://F2，教點檢視
					MsgBoxCall("msgboxTeachPointShow.txt");
					Now_Push_Btn->CreateA();				//20140930 add by Mario
					Now_Push_Btn->UpdateAll();			//20140930 add by Mario
					Now_Push_Btn->Update();					//20140930 add by Mario
			break;
			case	_F6://F3，指令說明
				MsgBoxCall("msgboxInputCommand.txt");
				Now_Push_Btn->CreateA();				//20140930 add by Mario
				Now_Push_Btn->UpdateAll();			//20140930 add by Mario
				Now_Push_Btn->Update();					//20140930 add by Mario
			break;
			case	_F7://F7，儲存
				SaveProgram(TRUE);				
				((CtmFormView*)pwndSender)->OnGetFocus();	
				//system("sleep 0.2");						//20140930 add by Mario
				Now_Push_Btn->CreateA();				//20140930 add by Mario
				Now_Push_Btn->UpdateAll();			//20140930 add by Mario
				Now_Push_Btn->Update();					//20140930 add by Mario
			break;
		}
	}
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	SaveProgram(FALSE);
	system("sync");
	if(FileCmy())
	{
		system("sync");
		system("rm -r temp/temp");
	}
}

void		OnUpdateA(CtmWnd*	pwndSender)
{
	static int old_sel_start=-1,old_sel_end=-1;
	if(Memotaborder == ((CtmFormView*)pwndSender)->Get_TabStop())	//光標在MTextmemo元件上
	{
		if(*sel_start!=old_sel_start)
		{
			old_sel_start = *sel_start;
			u_pwndEditSelStart->SetPropValueT("value",old_sel_start+1);
			u_pwndEditSelStart->Update();
		}
		if(*sel_end!=old_sel_end)
		{
			old_sel_end = *sel_end;
			u_pwndEditSelEnd->SetPropValueT("value",old_sel_end+1);
			u_pwndEditSelEnd->Update();
		}
	}
	char temp[INT_BUFFER_SIZE]="\0";
	sprintf(temp, "%s/%s", g_szProgDirectory, g_TxCNCData.shmCNCData.CNC_mem_part);
	if(WorkOne)
	{
		u_pwndLTextMemoProg->GetPropValueT("taborder",&Memotaborder,sizeof(Memotaborder));
		SetCommandMode(0);		//預設移動指令
		WorkOne =FALSE;
	}
	if(strcmp(temp,u_szMem_Part_File)==0 && !EditIsWorkFile)
		EditIsWorkFile = TRUE;
	else if(strcmp(temp,u_szMem_Part_File)!=0 && EditIsWorkFile)
		EditIsWorkFile= FALSE;
		
		
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE && !OnAuto)
		OnAuto = TRUE;
	else if(OnAuto && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE  )
		OnAuto=FALSE;
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/

void		ShowProgram(int nProgramStartNo , BOOL mode)		//顯示檔案內容，nProgramStartNo為印出的起始行(檔案裡)
{
	if(mode)
	{
		if(u_pwndLTextMemoProg != NULL)
		{
			((CtmMTextMemo*)u_pwndLTextMemoProg)->LoadFromFile(u_szMem_Part_File, nProgramStartNo,TRUE);
			((CtmMTextMemo*)u_pwndLTextMemoProg)->GotoLine(nProgramStartNo);
			((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
		}
	}
	else
	{

		((CtmMTextMemo*)u_pwndLTextMemoProg)->LoadFromFile("temp/temp", nProgramStartNo,TRUE);
		((CtmMTextMemo*)u_pwndLTextMemoProg)->GotoLine(nProgramStartNo);
		((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
	}
}

void		SaveProgram(BOOL mode)		//儲存檔案		0:暫存檔案		1:儲存檔案
{
	if(mode)
	{
		if(!EditIsWorkFile || !OnAuto)
		{
			if(u_pwndLTextMemoProg != NULL)
			{		
				//提示：檔案正在保存中，請勿切換畫面，請勿關機！
				Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("_AID_FILE_NOW_EDIT")], 1);
				((CtmMTextMemo *)u_pwndLTextMemoProg)->SaveToFile(u_szMem_Part_File);
				//usleep(5000*10);
				Etnernet_File(u_szMem_Part_File);
				//視窗：檔案已儲存！
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_FILESAVE_COMPLETE")], tmFT_CODE_TECH);
				if(EditIsWorkFile)	
				{					
					//printf("沒有我會出錯\n");	
					u_build_queue =1;	
				}											
			}
		}
		else 
			MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
	}
	else
	{
		if(u_pwndLTextMemoProg != NULL)
			((CtmMTextMemo *)u_pwndLTextMemoProg)->SaveToFile("temp/temp");
	}
	system("sync");
}

void    CodeChange(char *pDst, char *pSrc)		//輔助用函式，單字節轉化為雙字節處理
{
	//供StaticX2元件的text屬性使用(目前)
	WORD	*pwDst = (WORD *)pDst;
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}

void		AddNewCommand()		//將新指令插入光標所在的位置
{
	//開啟暫存新指令的檔案
	
	if(access("./DialogHelp/buffer",4) == -1)	//判斷檔案是否存在
		return;
		
	ifstream fCommandBuffer;
	fCommandBuffer.open("./DialogHelp/buffer");
	char	szCommand[512];
	fCommandBuffer.getline( szCommand, 512 );
	fCommandBuffer.close();
	
	int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
	//將新指令插入光標所在位置
	((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, szCommand);
	((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
}

void		ModifyCommand()
{
	if(access("./DialogHelp/buffer",4) == -1)	//判斷檔案是否存在
		return;
	ifstream fCommandBuffer;
	fCommandBuffer.open("./DialogHelp/buffer");
	char	szCommand[200];
	fCommandBuffer.getline( szCommand, 200 );
	fCommandBuffer.close();
	
	int nCurrentRow	= ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
	((CtmMTextMemo*)u_pwndLTextMemoProg)->DelRow_New(nCurrentRow,nCurrentRow);
	((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();	
	
	//int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
	//將新指令插入光標所在位置
	((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, szCommand);
	((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
}

void		ChangeFile(CtmWnd* pwndSender)		//20140923	by Mario  更新編輯檔案
{
	if(access("temp/temp",4)==0)
	{
		MsgBoxCall("msgboxFileSelect.txt");
		if(!g_bMsgBoxFlag)  return ;
	}
	((CtmFormView*)pwndSender)->OnLoseFocus();	
	SetDBString("CNC_EDIT_PATH", g_Now_FileName,sizeof(g_Now_FileName)); 
	GetDBString("CNC_EDIT_PATH",u_szMem_Part_File, sizeof(u_szMem_Part_File));
	ShowProgram(0,TRUE);
	SetFileInfo(u_szMem_Part_File);
	((CtmFormView*)pwndSender)->OnGetFocus();	
}

BOOL		FileCmy()													//該檔若與暫存檔相同則回傳TRUE
{
	BOOL FileSame = TRUE;
	ifstream fin[2];
  char s[200],s1[200];
  BOOL FinEOF1 = FALSE;
  BOOL FinEOF2 = FALSE;
  fin[0].open(u_szMem_Part_File);
  fin[1].open("temp/temp");
  if(fin[1] == NULL)	printf("error\n");
  //mario add 20150113

  while(1)
  {  
  	FinEOF1 = fin[0].eof();
  	FinEOF2 = fin[1].eof();
	  if(FinEOF1 != FinEOF2) return FALSE;
	  else if(!FinEOF1)
	  {
	  	fin[0].getline(s,200);
	  	fin[1].getline(s1,200);
	  	if(strcmp(s,s1) != 0)
				return FALSE;
	  }
	  else if(FinEOF1) return TRUE;
	}

	fin[0].close();
	fin[1].close();
		
	return TRUE;
}

void  SetCommandMode(int Mode)								//使元部的指令標籤重置成未被選取，所有的指令全部FALSE
{
	ComMode = Mode;
	ListStart = 1;
	for(int i=0 ; i<u_pwndbtnModeCommandCnt ; i++)
	{
		if(i!=Mode)
			u_pwndbtnModeCommand[i]->SetPropValueT("upbitmap", CommandModeImageUp[i]);
		else 
		{
			ListCount=0;
			while(CommandStr[ComMode][ListCount]!=NULL)
				ListCount++;
			u_pwndbtnModeCommand[i]->SetPropValueT("upbitmap", CommandModeImageDown[i]);
		}
		u_pwndbtnModeCommand[i]->CreateA();
		u_pwndbtnModeCommand[i]->Update(); 
	}
	CommandListShow();
}

void	CommandListShow()											//指令列表
{
	u_pwndStaticMask->CreateA();
	u_pwndStaticMask->Update();
	
	for(int i=0 ; i<u_pwndbtnCommandCnt ; i++)
	{
		if(ListStart+i <= ListCount)
		{
			u_pwndbtnCommand[i]->SetPropValueT("taborder",COMMANDLISTSTARTTABORDER + i);
			u_pwndbtnCommand[i]->SetPropValueT("captionID",CommandStr[ComMode][i+ListStart-1]);
			u_pwndbtnCommand[i]->CreateA();
			u_pwndbtnCommand[i]->Update();
		}
		else
			u_pwndbtnCommand[i]->SetPropValueT("taborder",-2);
	}
	SetListUpDown();
}

void	SetListUpDown()			//上下按鈕
{
	if(ListStart > 1)
	{
		u_pwndbtnListUp->SetPropValueT("upbitmap", 		"res_tm640/ListUp.png");
		u_pwndbtnListUp->SetPropValueT("downbitmap", 	"res_tm640/ListUp1.png");
		u_pwndbtnListUp->CreateA();
		u_pwndbtnListUp->Update();
	}
	else 
	{
		u_pwndbtnListUp->SetPropValueT("upbitmap", 		"res_tm640/ListNoUp.png");
		u_pwndbtnListUp->SetPropValueT("downbitmap", 	"res_tm640/ListNoUp.png");
		u_pwndbtnListUp->CreateA();
		u_pwndbtnListUp->Update();
	}
	
	if(ListStart+MAXCOMMANDBUTTON-1 < ListCount)
	{
		u_pwndbtnListDown->SetPropValueT("upbitmap", 		"res_tm640/ListDown.png");
		u_pwndbtnListDown->SetPropValueT("downbitmap", 	"res_tm640/ListDown1.png");
		u_pwndbtnListDown->CreateA();
		u_pwndbtnListDown->Update();
	}
	else 
	{
		u_pwndbtnListDown->SetPropValueT("upbitmap", 		"res_tm640/ListNoDown.png");
		u_pwndbtnListDown->SetPropValueT("downbitmap", 	"res_tm640/ListNoDown.png");
		u_pwndbtnListDown->CreateA();
		u_pwndbtnListDown->Update();
	}
}

void	CallCommandWork(int mode , int temp)			//呼叫指令工作函式或指令
{
	switch(mode)
	{
		case 0:				//移動指令
			if(temp>2) temp+=1;	//修正CIRCLE_F
			if(temp>6) temp+=1; //修正CIRCLE_REL_F
			switch(temp)
			{
				case 0:			//PTP_ABS	
					g_MoveCommandMode=0;
					MsgBoxCall("RobotCommand/msgboxMoveCommand.txt");
				break;
				case 1:			//LINE_ABS
					g_MoveCommandMode=1;
					MsgBoxCall("RobotCommand/msgboxMoveCommand.txt");
				break;
				case 2:			//CIRCILE_ABS	
					g_MoveCommandMode=2;		
					MsgBoxCall("RobotCommand/msgboxMoveCommand.txt");
				break;
				case 3:			//CIRCILE_ABS	
					g_MoveCommandMode=3;		
					MsgBoxCall("RobotCommand/msgboxMoveCommand.txt");
				break;
				case 4:			//PTP_REL	
					g_MoveCommandMode=0;		
					MsgBoxCall("RobotCommand/msgboxMoveCommandRel.txt");
				break;
				case 5:			//LINE_REL	
					g_MoveCommandMode=1;		
					MsgBoxCall("RobotCommand/msgboxMoveCommandRel.txt");
				break;
				case 6:			//CIRCILE_REL	
					g_MoveCommandMode=2;		
					MsgBoxCall("RobotCommand/msgboxMoveCommandRel.txt");
				break;
				case 7:			//CIRCILE_REL	
					g_MoveCommandMode=3;		
					MsgBoxCall("RobotCommand/msgboxMoveCommandRel.txt");
				break;
				case 8:			//PTP_PLT	
					g_MoveCommandMode=0;		
					MsgBoxCall("RobotCommand/msgboxMovePallet.txt");
				break;
				case 9:			//LINT_PLT	
					g_MoveCommandMode=1;		
					MsgBoxCall("RobotCommand/msgboxMovePallet.txt");
				break;
				case 10:			//PATH_BLEND	
					MsgBoxCall("RobotCommand/msgboxPathBlend.txt");
				break;
				default:
				break;
			}
		break;
		case 1:				//定義指令
			switch(temp)
			{
				//case 0:			//SETVAR
				//	MsgBoxCall("msgboxSetVar.txt");
				//break;
				case 0/*1*/:			//SETCOORD
					MsgBoxCall("RobotCommand/msgboxSetCoord.txt");
				break;
				case 1/*2*/:			//SETCOORD
					MsgBoxCall("RobotCommand/msgboxSetJoint.txt");
				break;
				case 2:			//COORDCAL
					MsgBoxCall("RobotCommand/msgboxCoordCal.txt");
				break;
				case 3:			//SETTABLE
					MsgBoxCall("RobotCommand/msgboxSetPallet.txt");
				break;
				default:
				break;
			}
		break;
		case 2:				//邏輯指令
			switch(temp)
			{
				/*case 0:			//if
				{
					int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
					//將新指令插入光標所在位置
					((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, "IF(  )");
					((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
				}
				break;*/
				case 0:			//GOTO
					MsgBoxCall("RobotCommand/msgboxGoTo.txt");
				break;
				case 1:			//CALLSUB
					MsgBoxCall("RobotCommand/msgboxCallSub.txt");
				break;
				case 2:			//END
				{
					int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
					//將新指令插入光標所在位置
					((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, "END;");
					((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
				}
				break;
				case 3:			//RETURN
				{
					int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
					//將新指令插入光標所在位置
					((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, "RETURN;");
					((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
				}
				break;
				default:
				break;
			}
		break;
		case 3:				//其他指令
			switch(temp)
			{
				case 0:			//WAIT
					MsgBoxCall("RobotCommand/msgboxWait.txt");
				break;
				case 1:			//WORKCOORD
					MsgBoxCall("RobotCommand/msgboxWorkCoord.txt");
				break;
				case 2:			//OM_SET
					MsgBoxCall("RobotCommand/msgboxOMSet.txt");
				break;
				case 3:			//IM_GET
					MsgBoxCall("RobotCommand/msgboxIMGet.txt");
				break;
				case 4:			//PAUSE
				{
					int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
					//將新指令插入光標所在位置
					((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, "PAUSE;");
					((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
				}
				break;
				//case 5:			//WAIT_UNTIL_IN
				//	MsgBoxCall("msgboxWaitUntilIn.txt");
				//break;
				case 5:			//TOOL
					MsgBoxCall("RobotCommand/msgboxTool.txt");
				break;
				case 6:			//OPT.STOP
				{
					int		nCurrentRow = ((CtmMTextMemo*)u_pwndLTextMemoProg)->GetCurrentRow();
					//將新指令插入光標所在位置
					((CtmMTextMemo*)u_pwndLTextMemoProg)->AddNewStr(nCurrentRow, "OPT_STOP;");
					((CtmMTextMemo*)u_pwndLTextMemoProg)->Update();
				}
				break;
				//case 8:			//ALL_SPEED
				//	MsgBoxCall("msgboxAllSpeed.txt");
				//break;
				case 7:			//NEXt_PALLET
					g_MoveCommandMode=0;
					MsgBoxCall("RobotCommand/msgboxNextPallet.txt");
				break;
				case 8:			//LAST_PALLET
					g_MoveCommandMode=1;
					MsgBoxCall("RobotCommand/msgboxLastPallet.txt");
				break;
				case 9:			//FRIST_PALLET
					g_MoveCommandMode=2;
					MsgBoxCall("RobotCommand/msgboxStartPallet.txt");
				break;
				case 10:			//SET_PT_PALLET
					g_MoveCommandMode=2;
					MsgBoxCall("RobotCommand/msgboxSetPTPallet.txt");
				break;
				case 11:			//GET_PT_PALLET
					g_MoveCommandMode=2;
					MsgBoxCall("RobotCommand/msgboxGetPTPallet.txt");
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
}

int	CallModifyCommand(char* f_Command)
{
	//取得指令
	char* ptr =NULL;
	if(strspn(f_Command,"C") == 1 && *(f_Command+1) != '0' && isdigit(*(f_Command+1)))//設定座標
	{
		char* temp=NULL;
		ptr = strstr(f_Command,"C");
		if(ptr == NULL) return -1;
		ptr+=1;
		if(!ISValue(ptr)) return -1; 
		ptr=PassInt(ptr);
		ptr=PassSpaceFindChar(ptr ,'=');
		if(ptr == NULL) return -1;
		ptr = PassSpace(ptr+1);
		if(*ptr == '(')		//定義自訂點C
		{
			return 2001;
		}
		else if(*ptr == 'C' || *ptr == 'P')	//自訂點C的+-*/
		{
			return 2003;
		}
	}
	else if(strspn(f_Command,"J") == 1 && *(f_Command+1) != '0' && isdigit(*(f_Command+1)))//設定座標
	{
			return 2002;
	}
	else if(strspn(f_Command,"#") == 1)//設定變數
	{
		return -1;
		//Get_IN處理
	}
	else if(strstr(f_Command,"PALLET") == f_Command)
	{
			return 2004;
	}
	else if(strstr(f_Command,"NEXT_PALLET")==f_Command)
	{
			return 4008;
	}
	else if(strstr(f_Command,"LAST_PALLET")==f_Command)
	{
			return 4009;
	}
	else if(strstr(f_Command,"FRIST_PALLET")==f_Command)
	{
		return 4010;
	}
	else if(strstr(f_Command,"SET_PT_PALLET") == f_Command)
	{
			return 4011;
	}
	else // 指令後強制空格
	{
		ptr = StrToke_TE(" =;(\n",f_Command);
		if(strcmp(ptr,"PTP_ABS")==0) //指令PTP_ABS解碼
		{
			return 1001;
		} 
		else if(strcmp(ptr,"LINE_ABS")==0)//指令LINE_ABS解碼 
		{ 
			return 1002;
		}
		else if(strcmp(ptr,"CIRCLE_ABS")==0)//指令CIRCLE_ABS解碼 
		{ 
		return 1003;
		}
		else if(strcmp(ptr,"PTP_REL")==0)//指令PTP_REL解碼 
		{
			return 1004;
		}
		else if(strcmp(ptr,"LINE_REL")==0)//指令LINE_REL解碼 
		{ 
			return 1005;
		}
		else if(strcmp(ptr,"CIRCLE_REL")==0)//指令CIRCLE_REL解碼 
		{ 
			return 1006;
		}
		else if(strcmp(ptr,"PTP_PLT")==0)
		{
			return 1007;
		}
		else if(strcmp(ptr,"LINE_PLT")==0)
		{
			return 1008;
		}
		else if(strstr(f_Command,"PATH_BLEND") == f_Command)
		{
				return 1009;
		}
		else if(strcmp(ptr,"GOTO")==0)
		{
			return 3001;
		}
		else if(strcmp(ptr,"RETURN")==0)
		{
			return -1;	
		}
		else if(strcmp(ptr,"CALLSUB")==0)
		{
			return 3002;
		}
		else if(strcmp(ptr,"PAUSE")==0)
		{
			return -1;
		}
		else if(strcmp(ptr,"END")==0)
		{
				return -1;
		}
		else if(strcmp(ptr,"OPT_STOP")==0)
		{
			return -1;	
		}
		else if(strcmp(ptr,"WORKCOORD")==0)
		{
			return 4002;
		}
		else if(strcmp(ptr,"TOOL")==0)
		{
			return 4006;
		}
		else if(strcmp(ptr,"WAIT")==0)
		{
			return 4001;	
		}
		else if(strcmp(ptr,"SET_OUT")==0)
		{
			return 4003;	
		}
		else if(strcmp(ptr,"IF")==0)
		{
			return 3001;	
		}
		else if(strcmp(ptr,"MARKPASS")==0)
		{
			return -1;				//被註解
		}
		else if(strlen(ptr) == 1)
		{
			return -1;				//被註解
		}
		else
		{
			return -1;
		}
	}
}


void 	SetFileInfo(char* Str)
{
	//printf("Str = %s\n",Str);
	//printf("Str = %d\n",strlen(Str));
	char	szTemp[64]		= "\0";
	if(*(Str+strlen(Str)-1)=='*')
	{
		CodeChange(szTemp, Str+strlen(Str)-6);
		*(Str+strlen(Str)-1) = '\0';
	}
	else
		CodeChange(szTemp, Str+strlen(Str)-5);
	u_pwndStaticEditFileName->SetPropValueT("text", szTemp);
	u_pwndStaticEditFileName->Update();
	
	struct stat buf;
	int result;
	//printf("Str = %s\n",Str);
	result =stat(Str, &buf );
	char CtimeStr[128];
	sprintf(CtimeStr,"%s\0",ctime(&buf.st_mtime));
	char Date[16];
	char Date2[32];
	char Time[16];
	char Time2[32];
	strncpy(Date,CtimeStr+20,4);
	int m;
	for(m=0;m<12;m++)
	{
		if(0==strncmp(CtimeStr+4,MM[m],3))
			break;
	}
	sprintf(Date+5,"%02d\0",m+1);
	strncpy(Date+8,CtimeStr+8,2);
	Date[4]=Date[7]='/';
	Date[10]='\0';
	
	strncpy(Time,CtimeStr+11,8);
	Time[8]='\0';
	
	
	CodeChange(Date2, Date);
	u_pwndStaticModifyDate->SetPropValueT("text", Date2);
	u_pwndStaticModifyDate->Update();
	CodeChange(Time2, Time);
	u_pwndStaticModifyTime->SetPropValueT("text", Time2);
	u_pwndStaticModifyTime->Update();
}