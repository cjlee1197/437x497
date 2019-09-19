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

#include	"ViewMsgBoxGoTo.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../selectedit.h" 
#include	"../checkbox.h"

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

CtmWnd*		u_pwndCheckBoxUseIf		= NULL;
CtmWnd*		u_pwndSelectEditInt1	= NULL;
CtmWnd*		u_pwndEditInt1				= NULL;
CtmWnd*		u_pwndSelectEditCondition	= NULL;
CtmWnd*		u_pwndSelectEditInt2	= NULL;
CtmWnd*		u_pwndEditInt2				= NULL;

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditTable		= pwndSender->FindControlFromName("EditTable01");
	
	u_pwndCheckBoxUseIf					= pwndSender->FindControlFromName("CheckBoxUseIf");
	u_pwndSelectEditInt1				= pwndSender->FindControlFromName("SelectEditInt1");
	u_pwndEditInt1							= pwndSender->FindControlFromName("EditInt1");
	u_pwndSelectEditCondition		= pwndSender->FindControlFromName("SelectEditCondition");
	u_pwndSelectEditInt2				= pwndSender->FindControlFromName("SelectEditInt2");
	u_pwndEditInt2							= pwndSender->FindControlFromName("EditInt2");
	
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
			
			BOOL	temp3 = FALSE;
			int	int1mode = ((CtmSelectEdit*)u_pwndSelectEditInt1)->GetIndex()+1;
			int int1 = 0;
			int	Condition = ((CtmSelectEdit*)u_pwndSelectEditCondition)->GetIndex()+1;;
			int	int2mode = ((CtmSelectEdit*)u_pwndSelectEditInt2)->GetIndex()+1;
			int int2 = 0;
			
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			u_pwndCheckBoxUseIf->GetPropValueT("checked", &temp3,sizeof(temp3));
			u_pwndEditInt1->GetPropValueT("value", &int1, sizeof(int1));
			u_pwndEditInt2->GetPropValueT("value", &int2, sizeof(int2));
			
			if(temp3 == TRUE)
			{
				sprintf(temp,"if(");
				if(int1mode == 1)	sprintf(temp,"%s #%d",temp,int1);
				else sprintf(temp,"%s %d",temp,int1);
				
				if(Condition == 1)	sprintf(temp,"%s >",temp);
				else if(Condition == 2)	sprintf(temp,"%s <",temp);
				else if(Condition == 3)	sprintf(temp,"%s ==",temp);
				else if(Condition == 4)	sprintf(temp,"%s !=",temp);
				
				if(int2mode == 1)	sprintf(temp,"%s #%d",temp,int2);
				else sprintf(temp,"%s %d",temp,int1);
				
				sprintf(temp,"%s ) GOTO %d;",temp,temp2);
			}	
			else
				sprintf(temp,"GOTO %d;",temp2);
			AddCommand(temp);
			Exit();
		}
	}
	
	return wIDControl;
}

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
			
			BOOL	temp3 = FALSE;
			int	int1mode = ((CtmSelectEdit*)u_pwndSelectEditInt1)->GetIndex()+1;
			int int1 = 0;
			int	Condition = ((CtmSelectEdit*)u_pwndSelectEditCondition)->GetIndex()+1;;
			int	int2mode = ((CtmSelectEdit*)u_pwndSelectEditInt2)->GetIndex()+1;
			int int2 = 0;
			
			u_pwndEditTable->GetPropValueT("value", &temp2, sizeof(temp2));
			u_pwndCheckBoxUseIf->GetPropValueT("checked", &temp3,sizeof(temp3));
			u_pwndEditInt1->GetPropValueT("value", &int1, sizeof(int1));
			u_pwndEditInt2->GetPropValueT("value", &int2, sizeof(int2));
			
			
			if(temp2<1 || (temp3 == TRUE && (int1<=0 || int2<=0)))
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
			if(temp3 == TRUE)
			{
				sprintf(temp,"IF(");
				if(int1mode == 1)	sprintf(temp,"%s#%d",temp,int1);
				else sprintf(temp,"%s%d",temp,int1);
				
				if(Condition == 1)	sprintf(temp,"%s>" ,temp);
				else if(Condition == 2)	sprintf(temp,"%s<",temp);
				else if(Condition == 3)	sprintf(temp,"%s==",temp);
				else if(Condition == 4)	sprintf(temp,"%s!=",temp);
				
				if(int2mode == 1)	sprintf(temp,"%s#%d",temp,int2);
				else sprintf(temp,"%s%d",temp,int2);
				
				sprintf(temp,"%s) GOTO %d;",temp,temp2);
			}	
			else
				sprintf(temp,"GOTO %d;",temp2);
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
	/**/BOOL IF_Flag = FALSE;
	/**/int  Var1[2]={0,0};
	/**/int  Type=0;
	/**/int  Var2[2]={0,0};
	/**/int  LineNo=0;
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//移除 「/*註解*/」
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	if(ptr == strstr(ptr,"IF"))
	{
		IF_Flag = TRUE;
		temp = strstr(ptr,"(");
		if(temp == NULL) return FALSE;
		ptr = PassSpace(temp+1);
		if(*ptr == '#')
		{
			Var1[0]=0;
			ptr+=1;
			if(!isdigit(*ptr)) return FALSE;
			Var1[1] = atoi(ptr);
			while(isdigit(*ptr)) ptr++;
			ptr = PassSpace(ptr);
		}
		else if(isdigit(*ptr))
		{
			Var1[0]=1;
			if(!isdigit(*ptr) && *ptr != '-')  return FALSE;
			float  Value = atof(ptr);
			Var1[1]=(int)(Value*1/*000*/);
			if(*ptr == '-') ptr++;
			while(isdigit(*ptr)) ptr++;
			ptr = PassSpace(ptr);
		}
		else return FALSE;
		
		if(ptr == strstr(ptr,">"))
		{
			Type=0;
			ptr = PassSpace(ptr+1);
		}
		else if(ptr == strstr(ptr,"<"))
		{
			Type=1;
			ptr = PassSpace(ptr+1);
		}
		else if(ptr == strstr(ptr,"=="))
		{
			Type=2;
			ptr = PassSpace(ptr+2);
		}
		else if(ptr == strstr(ptr,"!="))
		{
			Type=3;
			ptr = PassSpace(ptr+2);
		}
		else return FALSE;
		
		if(*ptr == '#')
		{
			Var2[0]=0;
			ptr+=1;
			if(!isdigit(*ptr)) return FALSE;
			Var2[1] = atoi(ptr);
			while(isdigit(*ptr)) ptr++;
			ptr = PassSpace(ptr);
		}
		else if(isdigit(*ptr))
		{
			Var2[0]=1;
			if(!isdigit(*ptr) && *ptr != '-')  return FALSE;
			float  Value = atof(ptr);
			Var2[1]=(int)(Value*1/*000*/);
			if(*ptr == '-') ptr++;
			while(isdigit(*ptr)) ptr++;
			ptr = PassSpace(ptr);
		}
		else return FALSE;
	}
	
	temp = strstr(ptr,"GOTO");
	if(temp == NULL) return FALSE;
	ptr = PassSpace(temp+4);
	if(!isdigit(*ptr)) return FALSE;
	LineNo= atoi(ptr);
	
	u_pwndEditTable	->SetPropValueT("value",LineNo);
	u_pwndEditTable	->Update();
	
	if(IF_Flag)
	{
		u_pwndCheckBoxUseIf->SetPropValueT("checked",(double)TRUE);
		u_pwndCheckBoxUseIf->Update();
		((CtmSelectEdit*)u_pwndSelectEditInt1)	->SetIndex(Var1[0]);
		u_pwndSelectEditInt1										->UpdateAll();
		u_pwndEditInt1	->SetPropValueT("value",Var1[1]);
		u_pwndEditInt1	->Update();
		((CtmSelectEdit*)u_pwndSelectEditCondition)	->SetIndex(Type);
		u_pwndSelectEditCondition										->UpdateAll();
		((CtmSelectEdit*)u_pwndSelectEditInt2)	->SetIndex(Var2[0]);
		u_pwndSelectEditInt2										->UpdateAll();
		u_pwndEditInt2	->SetPropValueT("value",Var2[1]);
		u_pwndEditInt2	->Update();
	}
	
	return TRUE;
}