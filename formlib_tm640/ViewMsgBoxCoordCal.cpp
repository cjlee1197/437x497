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

#include	"ViewMsgBoxCoordCal.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../selectedit.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey							= FALSE;

CtmWnd*		u_pwndBtnOK							= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;
CtmWnd*		u_pwndEditTable01				= NULL;
CtmWnd*		u_pwndEditPoint01				= NULL;
CtmWnd*		u_pwndEditPoint02				= NULL;
CtmWnd*		u_pwndSelectEditPoint01	= NULL;
CtmWnd*		u_pwndSelectEditPoint02	= NULL;
CtmWnd*		u_pwndSelectEditCal			= NULL;

/*========================================================+
|						Function																			|
+========================================================*/


BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditTable01	= pwndSender->FindControlFromName("PointTABLE01");
	
	u_pwndEditPoint01	= pwndSender->FindControlFromName("Point01");
	u_pwndEditPoint02	= pwndSender->FindControlFromName("Point02");
	
	u_pwndSelectEditPoint01	= pwndSender->FindControlFromName("SelectEditPoint01");
	u_pwndSelectEditPoint02	= pwndSender->FindControlFromName("SelectEditPoint02");
	u_pwndSelectEditCal			= pwndSender->FindControlFromName("SelectEditCal");
	
	return TRUE;
}
/*
WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{

	
	return wIDControl;
}*/
//20140624↑

WORD OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
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
			int Point = 0;
			int Point1 = 0;
			int Point2 = 0;
			char temp[3]="\0";
			char temp2[64]="\0";
				 
			u_pwndEditTable01->GetPropValueT("value", &Point, sizeof(Point1));
			u_pwndEditPoint01->GetPropValueT("value", &Point1, sizeof(Point1));
			u_pwndEditPoint02->GetPropValueT("value", &Point2, sizeof(Point1));
			
			if(Point<1  || Point1<1 || Point2<1 )
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
			if(((CtmSelectEdit*)u_pwndSelectEditPoint01)->GetIndex() == 0)
			{
				temp[0]='C';
			}
			else if(((CtmSelectEdit*)u_pwndSelectEditPoint01)->GetIndex() == 1)
				temp[0]='P';
			
			if(((CtmSelectEdit*)u_pwndSelectEditPoint02)->GetIndex() == 0)
				temp[2]='C';
			else if(((CtmSelectEdit*)u_pwndSelectEditPoint02)->GetIndex() == 1)
				temp[2]='P';
			
			
			if(((CtmSelectEdit*)u_pwndSelectEditCal)->GetIndex() == 0)
				temp[1]='+';
			else if(((CtmSelectEdit*)u_pwndSelectEditCal)->GetIndex() == 1)
				temp[1]='-';
			else if(((CtmSelectEdit*)u_pwndSelectEditCal)->GetIndex() == 2)
				temp[1]='*';
			else if(((CtmSelectEdit*)u_pwndSelectEditCal)->GetIndex() == 3)
				temp[1]='/';
			
			sprintf(temp2,"C%d = %c%d %c %c%d;",Point,temp[0],Point1,temp[1],temp[2],Point2);
			AddCommand(temp2);
			Exit();
		}
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
	/**/int CoordNO1=0;											/**/
	/**/int ModeNO2=0;											/**/
	/**/int CoordNO2=0;											/**/
	/**/int ModeNO3=0;											/**/
	/**/int CoordNO3=0;											/**/
	/**/int CalMode=0;											/**/
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//移除 「/*註解*/」
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(*ptr != 'C') return FALSE;
	ptr++;
	if(!isdigit(*ptr))	return FALSE;
	CoordNO1 = atoi(ptr);
	//while(isdigit(*ptr)) ptr++;
	temp = strstr(ptr,"=");
	if(temp == NULL)	return FALSE;
	ptr = PassSpace(temp+1);
	
	if(*ptr == 'C')
		ModeNO2 = 0;
	else if(*ptr == 'P')
		ModeNO2 = 1;
	else return FALSE;
	
	ptr+=1;// = PassSpace(ptr+1);
	if(!isdigit(*ptr))	return FALSE;
	CoordNO2 = atoi(ptr);
	
	while(isdigit(*ptr)) ptr++;
	ptr = PassSpace(ptr);
	
	if(*ptr == '+')
		CalMode=0;
	else if(*ptr == '-')
		CalMode=1;
	else if(*ptr == '*')
		CalMode=2;
	else if(*ptr == '/')
		CalMode=3;
	ptr = PassSpace(ptr+1);
	
	if(*ptr == 'C')
		ModeNO3 = 0;
	else if(*ptr == 'P')
		ModeNO3 = 1;
	else return FALSE;
	
	ptr+=1;// = PassSpace(ptr+1);
	if(!isdigit(*ptr))	return FALSE;
	CoordNO3 = atoi(ptr);
	
	u_pwndEditTable01	->SetPropValueT("value",CoordNO1);
	u_pwndEditPoint01	->SetPropValueT("value",CoordNO2);
	u_pwndEditPoint02	->SetPropValueT("value",CoordNO3);
	u_pwndEditTable01	->Update();
	u_pwndEditPoint01	->Update();
	u_pwndEditPoint02	->Update();
	
	((CtmSelectEdit*)u_pwndSelectEditPoint01)	->SetIndex(ModeNO2);
	u_pwndSelectEditPoint01										->UpdateAll();
	((CtmSelectEdit*)u_pwndSelectEditPoint02)	->SetIndex(ModeNO3);
	u_pwndSelectEditPoint02										->UpdateAll();
	((CtmSelectEdit*)u_pwndSelectEditCal)			->SetIndex(CalMode);
	u_pwndSelectEditCal												->UpdateAll();
	
	return TRUE;
}