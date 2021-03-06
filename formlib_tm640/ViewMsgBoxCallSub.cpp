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

#include	"ViewMsgBoxCallSub.h"
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
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndEditTable				= NULL;
CtmWnd*		u_pwndLoopTime				= NULL;


/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy↓
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditTable		= pwndSender->FindControlFromName("EditTable01");
	u_pwndLoopTime		= pwndSender->FindControlFromName("EditCycleTime");
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{	
		if(pwndTemp == u_pwndBtnCancel)
		{
			remove("./DialogHelp/buffer");
			Exit();
		}
		else if(pwndTemp == u_pwndBtnOK)
		{
			char temp[64]="\0";
			int temp2=0;
			int temp3=0;
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			u_pwndLoopTime->GetPropValueT("value", &temp3, sizeof(temp3));
			if(temp3<1)
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
			sprintf(temp,"CALLSUB %d L%d;",temp2,temp3);
			AddCommand(temp);
			Exit();
		}
	}
	
	return wIDControl;
}
//20140624↑

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
			int temp2=0;
			int temp3=0;
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			u_pwndLoopTime->GetPropValueT("value", &temp3, sizeof(temp3));
			sprintf(temp,"CALLSUB %d L%d;",temp2,temp3);
			AddCommand(temp);
			Exit();
		}
	
		((CtmFormView*)pwndSender)->Goto(wIDControl);
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
	static BOOL StartRunOne = TRUE;
	if(StartRunOne)
	{
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
		StartRunOne = FALSE;
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

BOOL		RevertOldCommand(char *Command)
{
	/*==========所需參數======================*/
	/**/int  FileName =0;//4位數字
	/**/int  LoopNum = 0;
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//移除 「/*註解*/」
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(ptr != strstr(ptr,"CALLSUB")) return FALSE;
	ptr = PassSpace(ptr+7);
	if(!isdigit(*ptr)) return FALSE;
	FileName = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	temp = strstr(ptr,"L");
	if(temp == NULL) return FALSE;
	ptr = temp+1;
	LoopNum = atoi(ptr);
	FileName = FileName%10000;
	u_pwndEditTable	->SetPropValueT("value",FileName);
	u_pwndLoopTime	->SetPropValueT("value",LoopNum);
	u_pwndEditTable	->Update();
  u_pwndLoopTime	->Update();
  
	return TRUE;
}