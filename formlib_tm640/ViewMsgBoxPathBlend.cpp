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


#include	"ViewMsgBoxPathBlend.h"
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
CtmWnd*		u_pwndSelectEditONOFF					= NULL;

int				u_UnitTime						=	0;
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy��
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	u_pwndSelectEditONOFF	= pwndSender->FindControlFromName("SelectONOFF");
	
	return TRUE;
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
			temp2 = ((CtmSelectEdit*)u_pwndSelectEditONOFF)->GetIndex();

			if(temp2 == 0)
				sprintf(temp,"PATH_BLEND ON;\0");
			else
				sprintf(temp,"PATH_BLEND OFF;\0");
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


BOOL		RevertOldCommand(char *Command)
{
	/*==========�һݰѼ�======================*/
	/**/int  	Index =1;
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//���� �u/*����*/�v
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(ptr != strstr(ptr,"PATH_BLEND")) return FALSE;
	ptr = PassSpace(ptr+10);
	temp = strstr(ptr,"ON");
	if(temp!=NULL) Index=0;
	((CtmSelectEdit*)u_pwndSelectEditONOFF)->SetIndex(Index);
	u_pwndSelectEditONOFF->Update();
	
	return TRUE;
}