/*===========================================================================+
|  Class    : ViewCore library                                          	 |
|  Task     : ViewCore library action source file                       	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewUserLog.h"
//#include	"../control.h"
#include	"../tmdebug.h"
#include	"../tmcontrol.h"
#include 	"../oper_log.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define	    STR_ACTION			"UserLog_ACT"			//	�y�z�r�� 
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL RunOnlyOne = FALSE;
WORD LogCount; // �i���`��
tmOper_log* Log; //�i�����e
int iPage=0; // ��ܭ���

CtmWnd* pwndBtn_PageUp 	 = NULL; // �W�@��
CtmWnd* pwndBtn_PageDown = NULL; // �U�@��

CtmWnd* pwndStr_No[11] 	={NULL}; // �Ǹ�	
int iStr_No = 0;
CtmWnd* pwndStr_Act[11] 	={NULL}; // �y�z	
int iStr_Act = 0;
CtmWnd* pwndStr_Value_Now[11] 	={NULL}; // �{����
int iStr_Value_Now = 0;
CtmWnd* pwndStr_Value_Old[11] 	={NULL}; // �즳��
int iStr_Value_Old = 0;
CtmWnd* pwndStr_Date[11] 	={NULL}; // �ק���
int iStr_Date = 0;
CtmWnd* pwndStr_Time[11] 	={NULL}; // �ק�ɶ�	
int iStr_Time = 0;

int *a;

/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{	
	/*�������*/
	pwndBtn_PageUp	 = pwndSender->FindControlFromName("Btn_PageUp");
	pwndBtn_PageDown = pwndSender->FindControlFromName("Btn_PageDown");
	
	iStr_No	 			 = GetSpecialControlNum(pwndSender, "Str_No", "CtmStaticX2", pwndStr_No);
	iStr_Act	 		 = GetSpecialControlNum(pwndSender, "Str_Act", "CtmStaticX2", pwndStr_Act);
	iStr_Value_Now = GetSpecialControlNum(pwndSender, "Str_Value_Now", "CtmStaticX2", pwndStr_Value_Now);
	iStr_Value_Old = GetSpecialControlNum(pwndSender, "Str_Value_Old", "CtmStaticX2", pwndStr_Value_Old);
	iStr_Date	 		 = GetSpecialControlNum(pwndSender, "Str_Date", "CtmStaticX2", pwndStr_Date);
	iStr_Time	 		 = GetSpecialControlNum(pwndSender, "Str_Time", "CtmStaticX2", pwndStr_Time);
	
	Update_UserLog(); // ��s�i�����e
	
	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = NULL;	
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
		{
			RunOnlyOne = TRUE;
		}

}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
		case 1: // Clear
			printf("Clear\n");
    	g_ptmControlServer->Oper_Log()->ClearOperLog();
    	Update_UserLog(); // ��s�i�����e
			break;
		default:
			break;
	}
	
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|  Function : OnMouseUp()                       	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("OnMouseUp\n");
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;

	else if(pwnd ==pwndBtn_PageDown) // �U�@��
	{
		if(iPage<19)
			iPage++;
		Update_UserLog(); // ��s�i�����e
	}
	else if(pwnd ==pwndBtn_PageUp) // �W�@��
	{
		if(iPage>0)
			iPage--;
		Update_UserLog(); // ��s�i�����e
	}
 	
}

void	OnDestroyA(CtmWnd* pwndSender)
{
}
/*---------------------------------------------------------------------------+
|  Function : Update_UserLog()                         	                     |
|  Task     : ��s��� �i�����e  	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void Update_UserLog() // ��s�i�����e
{
	printf("Update_UserLog\n");
	char  str_Temp[256] = "\0";
	char  str_Date[256] = "\0";
	char  str_Time[256] = "\0";
	char  str_Action[256] = ""; // �y�z��r
	
	LogCount = g_ptmControlServer->Oper_Log()->GetOperLogCount();
	printf("LogCount=%d\n",LogCount);
	for(int i=0;i<iStr_No;i++)
	{
		printf("No%d\n",iPage*10+i+1);
		Log = g_ptmControlServer->Oper_Log()->GetOperLog(iPage*10+i+1);
		printf("wDBIndex=%d\n",Log->wDBIndex);
		sprintf(str_Action, "%s%d", STR_ACTION, Log->wDBIndex);
		printf("wDBIndex=%s\n",str_Action);
		printf("dNewValue=%d\n",Log->dNewValue);
		printf("dOldValue=%d\n",Log->dOldValue);
		printf("%02d:%02d:%02d\n"	, Log->timeStart.hour, Log->timeStart.minute, Log->timeStart.second);
		if( (iPage*10+i+1) <= LogCount) // �ݤp���`�����ƥ�
		{
			if(pwndStr_No[i]!=NULL) // �Ǹ�
			{
				sprintf(str_Temp, "%4d", iPage*10+i+1);
				pwndStr_No[i]->SetPropValueT("text",str_Temp);
				pwndStr_No[i]->Update();
			}
			
			if(pwndStr_Act[i]!=NULL) // �y�z
			{
				if(Log->wDBIndex!=0)
					sprintf(str_Action, "%s%d", STR_ACTION, Log->wDBIndex);
				else
					sprintf(str_Action,"");
				pwndStr_Act[i]->SetPropValueT("text",str_Action);
				pwndStr_Act[i]->Update();
			}
			
			if(pwndStr_Value_Now[i]!=NULL) // �{����
			{
				sprintf(str_Temp, "%4d", Log->dNewValue);
				pwndStr_Value_Now[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Now[i]->Update();
			}
			if(pwndStr_Value_Old[i]!=NULL) // �즳��
			{
				sprintf(str_Temp, "%4d", Log->dOldValue);
				pwndStr_Value_Old[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Old[i]->Update();
			}
			
			if(pwndStr_Date[i]!=NULL) // �ק���
			{
				sprintf(str_Date, "%0d/%02d/%02d"	, Log->dateStart.month, Log->dateStart.day, Log->dateStart.year%100);
				pwndStr_Date[i]->SetPropValueT("text",str_Date);
				pwndStr_Date[i]->Update();
			}
			if(pwndStr_Time[i]!=NULL) // �ק�ɶ�
			{
				sprintf(str_Time, "%0d:%02d:%02d"	, Log->timeStart.hour, Log->timeStart.minute, Log->timeStart.second);
				pwndStr_Time[i]->SetPropValueT("text",str_Time);
				pwndStr_Time[i]->Update();
			}
		}
		else
		{
			sprintf(str_Temp,"");
			if(pwndStr_No[i]!=NULL) // �Ǹ�
			{
				pwndStr_No[i]->SetPropValueT("text",str_Temp);
				pwndStr_No[i]->Update();
			}
			
			if(pwndStr_Act[i]!=NULL) // �y�z
			{
				pwndStr_Act[i]->SetPropValueT("text",str_Temp);
				pwndStr_Act[i]->Update();
			}
			
			if(pwndStr_Value_Now[i]!=NULL) // �{����
			{
				pwndStr_Value_Now[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Now[i]->Update();
			}
			if(pwndStr_Value_Old[i]!=NULL) // �즳��
			{
				pwndStr_Value_Old[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Old[i]->Update();
			}
			
			if(pwndStr_Date[i]!=NULL) // �ק���
			{
				pwndStr_Date[i]->SetPropValueT("text",str_Temp);
				pwndStr_Date[i]->Update();
			}
			if(pwndStr_Time[i]!=NULL) // �ק�ɶ�
			{
				pwndStr_Time[i]->SetPropValueT("text",str_Temp);
				pwndStr_Time[i]->Update();
			}
		}
	}
}