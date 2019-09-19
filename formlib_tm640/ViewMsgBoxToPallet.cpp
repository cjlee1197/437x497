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


#include	"ViewMsgBoxToPallet.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndEditTable				= NULL;
CtmWnd*		u_pwndStaticTitle			= NULL;

int				Mode = 1;
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy��

BOOL		OnCreate_NEXT(CtmWnd* pwndSender)
{
	Mode = 1;
	OnCreateA(pwndSender);
	return TRUE;
}

BOOL		OnCreate_LAST(CtmWnd* pwndSender)
{
	Mode = -1;
	OnCreateA(pwndSender);
	return TRUE;
}

BOOL		OnCreate_START(CtmWnd* pwndSender)
{
	Mode = 0;
	OnCreateA(pwndSender);
	return TRUE;
}
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditTable		= pwndSender->FindControlFromName("EditTABLE01");
	u_pwndStaticTitle	= pwndSender->FindControlFromName("StaticMessage");
	
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
			int temp2 = 0;
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			
			if(temp2 < 1)
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
			
			if(Mode == 1) 
				sprintf(temp,"NEXT_PALLET%d;",temp2);
			else if(Mode == -1)
				sprintf(temp,"LAST_PALLET%d;",temp2);
			else if(Mode == 0)
				sprintf(temp,"FRIST_PALLET%d;",temp2);
			AddCommand(temp);
			Exit();
		}
	}
	
	return wIDControl;
}
//20140624��

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
			int temp2 = 0;
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			
			if(temp2 < 1)
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
			
			if(Mode == 1)
				sprintf(temp,"NEXT_PALLET%d;",temp2);
			else if(Mode == -1)
				sprintf(temp,"LAST_PALLET%d;",temp2);
			else if(Mode == 0)
				sprintf(temp,"FRIST_PALLET%d;",temp2);
			
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
		StartRunOne = FALSE;
		if(Mode == 1)
			u_pwndStaticTitle->SetPropValueT("textID","ROBOT_COMMAND_PALLET_NEXT");
		else if(Mode == -1)
			u_pwndStaticTitle->SetPropValueT("textID","ROBOT_COMMAND_PALLET_LAST");
		else if(Mode == 0)
			u_pwndStaticTitle->SetPropValueT("textID","ROBOT_COMMAND_PALLET_START");
		u_pwndStaticTitle->Update();
		
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

void		AddCommand(char* pszCommand)		//���J���O
{
	//�ǥ��B�~���ͪ��ɮרӼȦs�n���J�����O
	FILE*		fCommandBuffer;
	fCommandBuffer = fopen("./DialogHelp/buffer", "w");
	fprintf(fCommandBuffer, "%s", pszCommand);
	fclose(fCommandBuffer);
}

BOOL		RevertOldCommand(char *Command)
{
	/*==========�һݰѼ�======================*/
	/**/int  PalletNo =0;//4��Ʀr
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//���� �u/*����*/�v
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	char CommandName[15];
	if(Mode == 1) sprintf(CommandName,"NEXT_PALLET");
	else if(Mode == -1) sprintf(CommandName,"LAST_PALLET");
	else if(Mode == 0) sprintf(CommandName,"FRIST_PALLET");
	
	if(ptr != strstr(ptr,CommandName)) return FALSE;;
	ptr = PassSpace(ptr+strlen(CommandName));
	if(!isdigit(*ptr)) return FALSE;
	PalletNo = atoi(ptr);
	u_pwndEditTable->SetPropValueT("value",PalletNo);
	u_pwndEditTable->Update();
	return TRUE;
}