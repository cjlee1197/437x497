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

#include	"ViewMsgBoxSetPTPallet.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
//BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;

CtmWnd*		u_pwndEditPalletNO		= NULL;
CtmWnd*		u_pwndChechBoxSetMode1= NULL;
CtmWnd*		u_pwndChechBoxSetMode2= NULL;
CtmWnd*		u_pwndEditSetRow			= NULL;
CtmWnd*		u_pwndEditSetCol			= NULL;
CtmWnd*		u_pwndEditSetSeatNum	= NULL;

int	      SetMode 							= 1;		//1為位置編號  2為行列  預設為1
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 					= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel				= pwndSender->FindControlFromName("btnCancel");
	
	u_pwndEditPalletNO		= pwndSender->FindControlFromName("EditPalletNO");
	u_pwndChechBoxSetMode1= pwndSender->FindControlFromName("CheckBoxSetMode1");
	u_pwndChechBoxSetMode2= pwndSender->FindControlFromName("CheckBoxSetMode2");
	u_pwndEditSetRow			= pwndSender->FindControlFromName("EditRow");
	u_pwndEditSetCol			= pwndSender->FindControlFromName("EditCol");
	u_pwndEditSetSeatNum	= pwndSender->FindControlFromName("EditSeatNum");
	
	
	
	return TRUE;
}
/*
WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	
	return wIDControl;
}*/

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	

		if(pwndTemp == u_pwndBtnCancel)
		{
			remove("./DialogHelp/buffer");
			Exit();
		}
		else if(pwndTemp == u_pwndBtnOK)
		{
			char temp[64]="\0";
			int PalletNo = 0;
			
			u_pwndEditPalletNO->GetPropValueT("value", &PalletNo, sizeof(PalletNo));

			if(PalletNo <= 0 )
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
				
			if(SetMode == 1 )
			{
				int SeatNum	=0;
				u_pwndEditSetSeatNum->GetPropValueT("value", &SeatNum, sizeof(SeatNum));
				if(SeatNum <= 0 )
				{
					MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
					return wIDControl;
				}
				sprintf(temp,"SET_PT_PALLET%d = %d;",PalletNo,SeatNum);
			}
			else
			{
				int Row = 0;
				int Col = 0;
				u_pwndEditSetRow			->GetPropValueT("value", &Row, sizeof(Row));
				u_pwndEditSetCol			->GetPropValueT("value", &Col, sizeof(Col));
				if(Row <= 0 || Col <= 0)
				{
					MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
					return wIDControl;
				}
				sprintf(temp,"SET_PT_PALLET%d = (R=%d C=%d);",PalletNo,Row,Col);
			}	
			AddCommand(temp);
			Exit();
		}
	
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	
	if(wKey == _KEY_TEXTINPUT && u_pwndChechBoxSetMode1 == pwndTemp)
	{
		if(SetMode == 2 )
		{
			SetMode = 1;
			((CtmFormView*)pwndSender)->OnKey1(wKey);
		
			u_pwndChechBoxSetMode2->SetPropValueT("checked", (double)FALSE);	
			u_pwndChechBoxSetMode2->Update(); 
		}
	}
	else if(wKey == _KEY_TEXTINPUT && u_pwndChechBoxSetMode2 == pwndTemp)
	{
		if(SetMode == 1 )
		{
			SetMode = 2;
			((CtmFormView*)pwndSender)->OnKey1(wKey);
		
			u_pwndChechBoxSetMode1->SetPropValueT("checked", (double)FALSE);	
			u_pwndChechBoxSetMode1->Update(); 
		}
		
	}
	
	
	return wKey;	
}

void		OnUpdateA(CtmWnd*	pwndSender)
{
	static	BOOL	FristOne = TRUE;
	if(FristOne)
	{
		FristOne = FALSE;
		if(SetMode == 1)
		{
			u_pwndChechBoxSetMode1->SetPropValueT("checked", (double)TRUE);	
			u_pwndChechBoxSetMode1->Update(); 
		}
		else if(SetMode == 2)
		{
			u_pwndChechBoxSetMode2->SetPropValueT("checked", (double)TRUE);	
			u_pwndChechBoxSetMode2->Update(); 
		}
		
		if(RevertCommand != NULL)
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			if(!RevertOldCommand(RevertCommand))
			{
				remove("./DialogHelp/buffer");
				Exit();
			}
			((CtmFormView*)pwndSender)->OnGetFocus();
		}
	}
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

void ModeChang(int SetPTMode)
{
	if(SetPTMode == 0)
	{
		
		
	}
	else if(SetPTMode == 1)
	{
		
		
	}
}

BOOL		RevertOldCommand(char *Command)
{
	/*==========所需參數======================*/
	/**/int Pallet_No = 0;
	/**/int SetMode		=	0;
	/**/int Pallet_R	=	0;
	/**/int Pallet_C	=	0;
	/**/int Pallet_S	=	0;
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//移除 「/*註解*/」
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(ptr != strstr(ptr,"SET_PT_PALLET")) return FALSE;
	ptr = PassSpace(ptr+13);
	if(!isdigit(*ptr)) 	return FALSE;
	Pallet_No = atoi(ptr);
	temp = strstr(ptr,"=");
	if(temp == NULL)  	return FALSE;
	ptr = PassSpace(temp+1);
	if(*ptr == '(')
	{
		SetMode = 0;
		temp = PassSpaceFindChar(ptr+1,'R');
		if(temp == NULL) return FALSE;
		temp = PassSpaceFindChar(temp+1,'=');
		if(temp == NULL) return FALSE;
		ptr = PassSpace(temp+1);
		if(!isdigit(*ptr)) return FALSE;
		Pallet_R = atoi(ptr);
		while(isdigit(*ptr))	ptr++;
		
		temp = PassSpaceFindChar(ptr,'C');
		if(temp == NULL) return FALSE;
		temp = PassSpaceFindChar(temp+1,'=');
		if(temp == NULL) return FALSE;
		ptr = PassSpace(temp+1);
		if(!isdigit(*ptr)) return FALSE;
		Pallet_C = atoi(ptr);
	}
	else if(isdigit(*ptr))
	{
		SetMode = 1;
		Pallet_S = atoi(ptr);
	}
	else return FALSE;
	
	
	
	u_pwndEditPalletNO->SetPropValueT("value",Pallet_No);
	u_pwndEditPalletNO->Update();
	
	if(SetMode == 0)
	{
		u_pwndChechBoxSetMode1->SetPropValueT("checked", (double)FALSE);	
		u_pwndChechBoxSetMode2->SetPropValueT("checked", (double)TRUE);	
		
		u_pwndEditSetRow			->SetPropValueT("value",Pallet_R);
		u_pwndEditSetCol			->SetPropValueT("value",Pallet_C);
		u_pwndEditSetRow->Update();
		u_pwndEditSetCol->Update();
	}
	else if(SetMode == 1)
	{
		u_pwndChechBoxSetMode1->SetPropValueT("checked", (double)TRUE);	
		u_pwndChechBoxSetMode2->SetPropValueT("checked", (double)FALSE);	
		
		u_pwndEditSetSeatNum->SetPropValueT("value",Pallet_S);
		u_pwndEditSetSeatNum->Update();
	}
	u_pwndChechBoxSetMode1->Update(); 
	u_pwndChechBoxSetMode2->Update(); 
	
	return TRUE;
}