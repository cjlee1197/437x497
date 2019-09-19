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

//Mario write

#include	"ViewMsgBoxInputCommand.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include 	"../memo.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define COMMAND_MODE_NUM	4
#define COMMAND_LIST_NUM	7

/*========================================================+
|						Global Variable																|
+========================================================*/

char*	CommandMove[] =								//移動指令字串列表
{
	"ROBOT_COMMAND_PTP",						//MOV
	"ROBOT_COMMAND_LIN",						//LIN
	"ROBOT_COMMAND_CIRC",						//ARC
	"ROBOT_COMMAND_PTPREL",					//MOV_REL
	"ROBOT_COMMAND_LINREL",					//LIN_REL
	"ROBOT_COMMAND_CIRCREL",					//CIRC_REL
	"ROBOT_COMMAND_PTP_PALLET",					//CIRC_REL
	"ROBOT_COMMAND_LINE_PALLET",					//CIRC_REL
	NULL
}; 

char*	CommandMoveFile[] =					//移動指令字串說明文件
{
	"PTP_ABS_EX",						//MOV
	"LINE_ABS_EX",						//LIN
	"CIRCLE_ABS_EX",						//ARC
	"PTP_REL_EX",					//MOV_REL
	"LINE_REL_EX",					//LIN_REL
	"CIRCLE_REL_EX",					//CIRC_REL
	"PTP_PLT_EX",					//CIRC_REL
	"LINE_PLT_EX",					//CIRC_REL
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

char*	CommandDefinFile[] =						//定義指令字串說明文件
{
	"SET_COORD_EX",				//SETCOORD
	"SET_JOINT_EX",				//SETJOINT
	"COORD_CAL_EX",					//COORDCAL
	"SET_PALLET_EX",			//SETPALLET
	NULL
}; 

char*	CommandLogic[] =						//邏輯指令字串列表
{
	"ROBOT_COMMAND_IF",							//if
	"ROBOT_COMMAND_GOTO",						//GOTO
	"ROBOT_COMMAND_CALLSUB",				//CALLSUB
	"ROBOT_COMMAND_EDN",					//RETURN
	"ROBOT_COMMAND_RETURN",					//RETURN
	NULL
}; 

char*	CommandLogicFile[] =						//邏輯指令字串說明文件
{
	"IF_EX",							//if
	"GOTO_EX",						//GOTO
	"CALLSUB_EX",				//CALLSUB
	"EDN_EX",					//RETURN
	"RETURN_EX",					//RETURN
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

char*	CommandOtherFile[] =						//其他指令字串說明文件
{
	"WAIT_EX",						//WAIT
	"WORKCOORD_EX",			//WORKCOORD
	"SET_OUT_EX",					//OM_SET
	"GET_IN_EX",					//IM_GET
	"PAUSE_EX",					//TEST
	"TOOL_EX",						//TOOL
	"OPTION_EX",				//OPT.STOP
	"NEXT_PALLET_EX",			//NEXT_PALLET
	"LAST_PALLET_EX",			//LAST_PALLET
	"START_PALLET_EX",			//START_PALLET
	"SET_PT_PALLET_EX",			//SET_PT_PALLET
	"GET_PT_PALLET_EX",			//GET_PT_PALLET
	NULL
}; 


//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK							= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;
CtmWnd*		u_pwndBtnCommandMode[COMMAND_MODE_NUM]	= {NULL};
int				u_nCommandModeCnt 		= 0;
CtmWnd*		u_pwndBtnCommandList[COMMAND_LIST_NUM]	= {NULL};
int				u_nCommandListCnt 		= 0;
CtmWnd*		u_pwndBtnPageUp					= NULL;
CtmWnd*		u_pwndBtnPageDown				= NULL;
CtmWnd*		u_pwndStaticCommandName	=	NULL;
CtmWnd*		u_pwndStaticCommandSay	= NULL;
CtmWnd*		u_pwndMTextMemoEX				= NULL;


int 			u_CommandMode					=	0;		//當前的指令模式  預設為0:移動
int 			u_CommandListStartNo 	=	0;		//指令列表從第幾個指令開始顯示
int				u_CommandMax					=	0;		//當前模式的指令最大數量
char** 		CommandStr	 					=NULL;	//字串名稱指標陣列
char** 		CommandFileName				=NULL;	//說明檔名指標陣列
int				u_CommandSelect				=0;			//預設第一個	

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel			= pwndSender->FindControlFromName("btnCancel");
		
	u_nCommandModeCnt					= GetSpecialControlNum(pwndSender, "BtnCommandMode", "CtmToolButton", u_pwndBtnCommandMode);
	u_nCommandListCnt					= GetSpecialControlNum(pwndSender, "btnCommand", "CtmToolButton", u_pwndBtnCommandList);
	u_pwndBtnPageUp						= pwndSender->FindControlFromName("BtnPageUp");
	u_pwndBtnPageDown					= pwndSender->FindControlFromName("BtnPageDown");
	u_pwndStaticCommandName		= pwndSender->FindControlFromName("CommandName");
	u_pwndStaticCommandSay		= pwndSender->FindControlFromName("CommanExplanation");
	u_pwndMTextMemoEX					= pwndSender->FindControlFromName("MemoCommandEx");
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	

	return wIDControl;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	//((CtmMTextMemo*)u_pwndMTextMemoEX)->OnLoseFocus(); 	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp==u_pwndBtnCancel)
	{
		Exit();
	}
	else if(pwndTemp==u_pwndBtnPageDown)
	{
		if(u_CommandListStartNo < u_CommandMax - COMMAND_LIST_NUM)
		{
			u_CommandListStartNo++;
			u_CommandSelect--;
			ShowList();
		}
	}
	else if(pwndTemp==u_pwndBtnPageUp)
	{
		if(u_CommandListStartNo > 0)
		{
			u_CommandListStartNo--;
			u_CommandSelect++;
			ShowList();
		}
	}
	else 
	{
		for(int i = 0 ; i < u_nCommandModeCnt ; i++)
		{
			if(pwndTemp==u_pwndBtnCommandMode[i])
			{
				u_CommandMode = i;
				UpdateCommandModeAndList();
				return wIDControl;
			}
		}
		for(int i = 0 ; i < u_nCommandListCnt ; i++)
		{
			if(pwndTemp==u_pwndBtnCommandList[i])
			{
				u_CommandSelect = i;
				UpdateCommandDate();
				return wIDControl;
			}
		}
	}

	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	
	return wKey;	
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL FristOne = TRUE;
	if(FristOne)
	{
		FristOne = FALSE;
		((CtmFormView*)pwndSender)->OnLoseFocus(); 
		UpdateCommandModeAndList();
	}
}

/*========================================================+
|						Helper																				|
+========================================================*/

void	UpdateCommandModeAndList()
{
	SetBtnCommandModeColor();
	ListIni();
	ShowList();
}

void SetBtnCommandModeColor()
{
	static int old_CommandMode = -1;
	if(u_CommandMode != old_CommandMode)
	{
		if(old_CommandMode >= 0 && old_CommandMode < COMMAND_MODE_NUM )
		{
			u_pwndBtnCommandMode[old_CommandMode]->SetPropValueT("bgc",1046);
			u_pwndBtnCommandMode[old_CommandMode]->Update();
		}
		u_pwndBtnCommandMode[u_CommandMode]->SetPropValueT("bgc",0x001F);
		u_pwndBtnCommandMode[u_CommandMode]->Update();
		
		old_CommandMode = u_CommandMode;
	}
	
}

void ListIni()
{
	u_CommandSelect = 0;
	u_CommandListStartNo = 0;
	//char* temp[] = NULL;
	if(u_CommandMode == 0)
	{
		CommandStr = CommandMove;
		CommandFileName = CommandMoveFile;
		
	}
	else if(u_CommandMode == 1)
	{
		CommandStr = CommandDefin;
		CommandFileName = CommandDefinFile;
	}
	else if(u_CommandMode == 2)
	{
		CommandStr = CommandLogic;
		CommandFileName = CommandLogicFile;
	}
	else if(u_CommandMode == 3)
	{
		CommandStr = CommandOther;
		CommandFileName = CommandOtherFile;
	}
	
	for (int i = 0 ; i <= 999 ; i++)
	{
		if(CommandStr[i]==NULL)
		{
			u_CommandMax = i;
			break;
		}
	}
	
	
}

void ShowList()
{
	for(int i = 0; i < u_nCommandListCnt ; i++)
	{
		if((u_CommandListStartNo+i) < u_CommandMax)
		{
			u_pwndBtnCommandList[i]->SetPropValueT("captionID",CommandStr[u_CommandListStartNo+i]);
			u_pwndBtnCommandList[i]->SetPropValueT("enabled",(double)TRUE);
			u_pwndBtnCommandList[i]->CreateA();
			u_pwndBtnCommandList[i]->Update();
		}
		else
		{
			u_pwndBtnCommandList[i]->SetPropValueT("captionID","ROBOT_STR_DUMMY");
			u_pwndBtnCommandList[i]->SetPropValueT("enabled",(double)FALSE);
			u_pwndBtnCommandList[i]->CreateA();
			u_pwndBtnCommandList[i]->Update();
		}
	
	}
	UpdateCommandDate();
}

void	UpdateCommandDate()
{
	if(u_CommandSelect < 0) u_CommandSelect = 0;
	if(u_CommandSelect >= COMMAND_LIST_NUM) u_CommandSelect = COMMAND_LIST_NUM-1;
	
	if(u_CommandSelect + u_CommandListStartNo > u_CommandMax-1) u_CommandSelect = 0;
	
	u_pwndStaticCommandName->SetPropValueT("textID",CommandStr[u_CommandSelect+u_CommandListStartNo]);
	u_pwndStaticCommandName->Update();
	
	char	Str_Say[64];
	memset(Str_Say,0,64);
	sprintf(Str_Say,"%s_SAY",CommandStr[u_CommandSelect+u_CommandListStartNo]);
//	printf("Str_Say is %s\n",Str_Say);
	
	u_pwndStaticCommandSay->SetPropValueT("textID",Str_Say);
	u_pwndStaticCommandSay->Update();
	
	char	Str_EX[64];
	memset(Str_EX,0,64);
	sprintf(Str_EX,"CommandFile/%s",CommandFileName[u_CommandSelect+u_CommandListStartNo]);
	//printf("Str_EX is %s\n",Str_EX);
	
	
	((CtmMTextMemo*)u_pwndMTextMemoEX)->OnLoseFocus(); 	
	((CtmMTextMemo*)u_pwndMTextMemoEX)->LoadFromFile(Str_EX, 0);
	((CtmMTextMemo*)u_pwndMTextMemoEX)->GotoLine(0);
	((CtmMTextMemo*)u_pwndMTextMemoEX)->Update();
	
	SetBtnCommandListColor();
	
	
}

void	SetBtnCommandListColor()
{
	static int old_CommandListSelect = -1;
	if(old_CommandListSelect != u_CommandSelect )
	{
		if(old_CommandListSelect >=0 && old_CommandListSelect < COMMAND_LIST_NUM)
		{
			u_pwndBtnCommandList[old_CommandListSelect]->SetPropValueT("bgc",0xDEDB);
			u_pwndBtnCommandList[old_CommandListSelect]->Update();
		}
		u_pwndBtnCommandList[u_CommandSelect]->SetPropValueT("bgc",0x841F);
		u_pwndBtnCommandList[u_CommandSelect]->Update();
		
		old_CommandListSelect = u_CommandSelect;
	}
}