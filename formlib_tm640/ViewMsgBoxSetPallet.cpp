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

#include	"ViewMsgBoxSetPallet.h"
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


CtmWnd*		u_pwndEditTableNO					= NULL;
CtmWnd*		u_pwndEditStartPointNO		= NULL;
CtmWnd*		u_pwndEditRowPointNO			= NULL;
CtmWnd*		u_pwndEditColPointNO			= NULL;
CtmWnd*		u_pwndEditRowNum					= NULL;
CtmWnd*		u_pwndEditColNum					= NULL;
CtmWnd*		u_pwndSelectEditStartPoint= NULL;
CtmWnd*		u_pwndSelectEditRowPoint	= NULL;
CtmWnd*		u_pwndSelectEditColPoint	= NULL;
CtmWnd*		u_pwndSelectEditTableMode	= NULL;

/*========================================================+
|						Function																			|
+========================================================*/


BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	
	u_pwndEditTableNO					= pwndSender->FindControlFromName("EditTableNum");
	u_pwndEditStartPointNO		= pwndSender->FindControlFromName("EditStartPointNO");
	u_pwndEditRowPointNO			= pwndSender->FindControlFromName("EditRowPointNO");
	u_pwndEditColPointNO			= pwndSender->FindControlFromName("EditColPointNO");
	u_pwndEditRowNum					= pwndSender->FindControlFromName("EditRowNum");
	u_pwndEditColNum					= pwndSender->FindControlFromName("EditColNum");
	u_pwndSelectEditStartPoint= pwndSender->FindControlFromName("SelectEditStartPoint");
	u_pwndSelectEditRowPoint	= pwndSender->FindControlFromName("SelectEditRowPoint");
	u_pwndSelectEditColPoint	= pwndSender->FindControlFromName("SelectEditColPoint");
	u_pwndSelectEditTableMode	= pwndSender->FindControlFromName("SelectEditTableMode");

	
	
	return TRUE;
}

/*WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
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
		char temp[64]="\0";
		char temp2[4]="\0";
		int TableNO = 0;
		int	StartPointNO = 0;
		int	RowPointNO = 0;
		int ColPointNO = 0;
		int RowNum = 0;
		int ColNum = 0;
		
		u_pwndEditTableNO				->GetPropValueT("value", &TableNO, sizeof(TableNO));
		u_pwndEditStartPointNO	->GetPropValueT("value", &StartPointNO, sizeof(StartPointNO));
		u_pwndEditRowPointNO		->GetPropValueT("value", &RowPointNO, sizeof(RowPointNO));
		u_pwndEditColPointNO		->GetPropValueT("value", &ColPointNO, sizeof(ColPointNO));
		u_pwndEditRowNum				->GetPropValueT("value", &RowNum, sizeof(RowNum));
		u_pwndEditColNum				->GetPropValueT("value", &ColNum, sizeof(ColNum));
		
		if(TableNO<1  || StartPointNO<1 || RowPointNO<1 || ColPointNO<1 || RowNum<1 || ColNum<1)
		{
			MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
			return wIDControl;
		}
		
		if(((CtmSelectEdit*)u_pwndSelectEditStartPoint)->GetIndex() == 0)
			temp2[0]='C';
		else if(((CtmSelectEdit*)u_pwndSelectEditStartPoint)->GetIndex() == 1)
			temp2[0]='P';
			
			if(((CtmSelectEdit*)u_pwndSelectEditRowPoint)->GetIndex() == 0)
			temp2[1]='C';
		else if(((CtmSelectEdit*)u_pwndSelectEditRowPoint)->GetIndex() == 1)
			temp2[1]='P';
			
		if(((CtmSelectEdit*)u_pwndSelectEditColPoint)->GetIndex() == 0)
			temp2[2]='C';
		else if(((CtmSelectEdit*)u_pwndSelectEditColPoint)->GetIndex() == 1)
			temp2[2]='P';
			
		if(((CtmSelectEdit*)u_pwndSelectEditTableMode)->GetIndex() == 0)
			temp2[3]='S';
		else if(((CtmSelectEdit*)u_pwndSelectEditTableMode)->GetIndex() == 1)
			temp2[3]='Z';
		
		
		sprintf(temp,"PALLET%d = (%c%d %c%d %c%d R=%d C=%d %c);",TableNO,temp2[0],StartPointNO,temp2[1],RowPointNO,temp2[2],ColPointNO,RowNum,ColNum,temp2[3]);
		AddCommand(temp);
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
	/**/int PalletNO=0;											/**/
	/**/int Point1[2]={0,0};								/**/
	/**/int Point2[2]={0,0};								/**/
	/**/int Point3[2]={0,0};								/**/
	/**/int Row=0;													/**/
	/**/int Col=0;													/**/
	/**/int mode=0;													/**/
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//移除 「/*註解*/」
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(strstr(ptr,"PALLET") != ptr) return FALSE;
	ptr+=6;
	if(!isdigit(*ptr))	return FALSE;
	PalletNO = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	temp = strstr(ptr,"(");
	if(temp == NULL)	return FALSE;
	ptr = PassSpace(temp+1);
	
	if(*ptr == 'C')
		Point1[0] = 0;
	else if(*ptr == 'P')
		Point1[0] = 1;
	else return FALSE;
	ptr+=1;
	if(!isdigit(*ptr))	return FALSE;
	Point1[1] = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	ptr = PassSpace(ptr+1);
	
	if(*ptr == 'C')
		Point2[0] = 0;
	else if(*ptr == 'P')
		Point2[0] = 1;
	else return FALSE;
	ptr+=1;
	if(!isdigit(*ptr))	return FALSE;
	Point2[1] = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	ptr = PassSpace(ptr+1);
	
	if(*ptr == 'C')
		Point3[0] = 0;
	else if(*ptr == 'P')
		Point3[0] = 1;
	else return FALSE;
	ptr+=1;
	if(!isdigit(*ptr))	return FALSE;
	Point3[1] = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	ptr = PassSpace(ptr+1);
	
	if(*ptr != 'R') return FALSE;
	temp = strstr(ptr,"=");
	if(temp == NULL)	return FALSE;
	ptr = PassSpace(temp+1);
	if(!isdigit(*ptr))	return FALSE;
	Row = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	ptr = PassSpace(ptr+1);
	
	if(*ptr != 'C') return FALSE;
	temp = strstr(ptr,"=");
	if(temp == NULL)	return FALSE;
	ptr = PassSpace(temp+1);
	if(!isdigit(*ptr))	return FALSE;
	Col = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	ptr = PassSpace(ptr+1);
	
	if(*ptr == 'S') mode=0;
	else if(*ptr == 'Z') mode=1;
	else return FALSE;
	
	
	u_pwndEditTableNO														->SetPropValueT("value",PalletNO);
	u_pwndEditTableNO														->Update();
	((CtmSelectEdit*)u_pwndSelectEditStartPoint)->SetIndex(Point1[0]);
	u_pwndSelectEditStartPoint									->UpdateAll();
	u_pwndEditStartPointNO											->SetPropValueT("value",Point1[1]);
	u_pwndEditStartPointNO											->Update();
	((CtmSelectEdit*)u_pwndSelectEditRowPoint)	->SetIndex(Point2[0]);
	u_pwndSelectEditRowPoint										->UpdateAll();
	u_pwndEditRowPointNO												->SetPropValueT("value",Point2[1]);
	u_pwndEditRowPointNO												->Update();
	((CtmSelectEdit*)u_pwndSelectEditColPoint)	->SetIndex(Point3[0]);
	u_pwndSelectEditColPoint										->UpdateAll();
	u_pwndEditColPointNO												->SetPropValueT("value",Point3[1]);
	u_pwndEditColPointNO												->Update();
	u_pwndEditRowNum														->SetPropValueT("value",Row);
	u_pwndEditRowNum														->Update();
	u_pwndEditColNum														->SetPropValueT("value",Col);
	u_pwndEditColNum														->Update();
	((CtmSelectEdit*)u_pwndSelectEditTableMode)	->SetIndex(mode);
	u_pwndSelectEditTableMode										->UpdateAll();
	

	return TRUE;
}