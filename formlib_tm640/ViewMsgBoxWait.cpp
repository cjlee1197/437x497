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


#include	"ViewMsgBoxWait.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../selectedit.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/
char* Unit_Time_Str[]=
{
	"ROBOT_STR_SEC",
	"ROBOT_STR_MINISEC",
	NULL
};


//�P�_�O�_���UENTER��
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndEditTable				= NULL;
CtmWnd*		u_pwndSelectEditUnit					= NULL;

int				u_UnitTime						=	0;
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy��
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditTable		= pwndSender->FindControlFromName("EditTABLE01");
	u_pwndSelectEditUnit			= pwndSender->FindControlFromName("SelectEditTimeUnit");
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_pwndSelectEditUnit)
	{
		u_UnitTime = ((CtmSelectEdit*)u_pwndSelectEditUnit)->GetIndex();
		int	temp;
		u_pwndEditTable->GetPropValueT("value",&temp,sizeof(temp));
		u_pwndEditTable->SetPropValueT("max", 999000*!u_UnitTime+999);
		u_pwndEditTable->SetPropValueT("precision", (double)!u_UnitTime*3);
		if(u_UnitTime==1)
			temp=round((double)temp/1000.0);
		else	if(u_UnitTime==0)
			temp=temp*1000;
		u_pwndEditTable->SetPropValueT("value", temp);
		u_pwndEditTable->Update();
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
			if(temp2<1)
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				return wIDControl;
			}
			if(u_UnitTime == 1)
				sprintf(temp,"WAIT %dMS;",temp2);
			else if(u_UnitTime == 0)
			{
				char temp3[10];
				memset(temp3,0,10);
				sprintf(temp3,"%f",(double)temp2/1000);
				ClearCoordZeor(temp3,strlen(temp3));
				//temp2 /= 1000;
				sprintf(temp,"WAIT %sS;",temp3);
			}
			AddCommand(temp);
			Exit();
		}
		
		//((CtmFormView*)pwndSender)->Goto(wIDControl);
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
		((CtmFormView*)pwndSender)->OnLoseFocus();
		u_pwndEditTable->SetPropValueT("max", 999000*!u_UnitTime+999);
		u_pwndEditTable->SetPropValueT("precision", (double)!u_UnitTime*3);
		u_pwndEditTable->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
		if(RevertCommand != NULL)
		{
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

void		AddCommand(char* pszCommand)		//���J���O
{
	//�ǥ��B�~���ͪ��ɮרӼȦs�n���J�����O
	FILE*		fCommandBuffer;
	fCommandBuffer = fopen("./DialogHelp/buffer", "w");
	fprintf(fCommandBuffer, "%s", pszCommand);
	fclose(fCommandBuffer);
}


void		ClearCoordZeor(char *Coord,int Num)					//��z�y�� �h���p���I��h�l0
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

BOOL		RevertOldCommand(char *Command)
{
	/*==========�һݰѼ�======================*/
	/**/float	WaitTime =0;//4��Ʀr
	/**/int  	TimeUnit =0;
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//���� �u/*����*/�v
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(ptr != strstr(ptr,"WAIT")) return FALSE;
	ptr = PassSpace(ptr+4);
	if(!isdigit(*ptr)) return FALSE;
	WaitTime = atof(ptr);
	if(strstr(ptr,"MS") != NULL)
		TimeUnit = 1;
	else if(strstr(ptr,"S") != NULL)
	{
		WaitTime*=1000;
		TimeUnit = 0;
	}
	else return FALSE;
	
	u_UnitTime = TimeUnit;
	
	u_pwndEditTable->SetPropValueT("max", 999000*!u_UnitTime+999);
	u_pwndEditTable->SetPropValueT("precision", (double)!u_UnitTime*3);
	u_pwndEditTable->SetPropValueT("value",WaitTime);
	u_pwndEditTable->Update();
	((CtmSelectEdit*)u_pwndSelectEditUnit)->SetIndex(TimeUnit);
	u_pwndSelectEditUnit->Update();
	
	return TRUE;
}