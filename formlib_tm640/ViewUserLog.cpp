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
#define	    STR_ACTION			"UserLog_ACT"			//	描述字串 
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL RunOnlyOne = FALSE;
WORD LogCount; // 履歷總數
tmOper_log* Log; //履歷內容
int iPage=0; // 顯示頁數

CtmWnd* pwndBtn_PageUp 	 = NULL; // 上一頁
CtmWnd* pwndBtn_PageDown = NULL; // 下一頁

CtmWnd* pwndStr_No[11] 	={NULL}; // 序號	
int iStr_No = 0;
CtmWnd* pwndStr_Act[11] 	={NULL}; // 描述	
int iStr_Act = 0;
CtmWnd* pwndStr_Value_Now[11] 	={NULL}; // 現有值
int iStr_Value_Now = 0;
CtmWnd* pwndStr_Value_Old[11] 	={NULL}; // 原有值
int iStr_Value_Old = 0;
CtmWnd* pwndStr_Date[11] 	={NULL}; // 修改日期
int iStr_Date = 0;
CtmWnd* pwndStr_Time[11] 	={NULL}; // 修改時間	
int iStr_Time = 0;

int *a;

/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{	
	/*獲取指標*/
	pwndBtn_PageUp	 = pwndSender->FindControlFromName("Btn_PageUp");
	pwndBtn_PageDown = pwndSender->FindControlFromName("Btn_PageDown");
	
	iStr_No	 			 = GetSpecialControlNum(pwndSender, "Str_No", "CtmStaticX2", pwndStr_No);
	iStr_Act	 		 = GetSpecialControlNum(pwndSender, "Str_Act", "CtmStaticX2", pwndStr_Act);
	iStr_Value_Now = GetSpecialControlNum(pwndSender, "Str_Value_Now", "CtmStaticX2", pwndStr_Value_Now);
	iStr_Value_Old = GetSpecialControlNum(pwndSender, "Str_Value_Old", "CtmStaticX2", pwndStr_Value_Old);
	iStr_Date	 		 = GetSpecialControlNum(pwndSender, "Str_Date", "CtmStaticX2", pwndStr_Date);
	iStr_Time	 		 = GetSpecialControlNum(pwndSender, "Str_Time", "CtmStaticX2", pwndStr_Time);
	
	Update_UserLog(); // 更新履歷內容
	
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
    	Update_UserLog(); // 更新履歷內容
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

	else if(pwnd ==pwndBtn_PageDown) // 下一頁
	{
		if(iPage<19)
			iPage++;
		Update_UserLog(); // 更新履歷內容
	}
	else if(pwnd ==pwndBtn_PageUp) // 上一頁
	{
		if(iPage>0)
			iPage--;
		Update_UserLog(); // 更新履歷內容
	}
 	
}

void	OnDestroyA(CtmWnd* pwndSender)
{
}
/*---------------------------------------------------------------------------+
|  Function : Update_UserLog()                         	                     |
|  Task     : 更新資料 履歷內容  	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void Update_UserLog() // 更新履歷內容
{
	printf("Update_UserLog\n");
	char  str_Temp[256] = "\0";
	char  str_Date[256] = "\0";
	char  str_Time[256] = "\0";
	char  str_Action[256] = ""; // 描述文字
	
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
		if( (iPage*10+i+1) <= LogCount) // 需小於總紀錄數目
		{
			if(pwndStr_No[i]!=NULL) // 序號
			{
				sprintf(str_Temp, "%4d", iPage*10+i+1);
				pwndStr_No[i]->SetPropValueT("text",str_Temp);
				pwndStr_No[i]->Update();
			}
			
			if(pwndStr_Act[i]!=NULL) // 描述
			{
				if(Log->wDBIndex!=0)
					sprintf(str_Action, "%s%d", STR_ACTION, Log->wDBIndex);
				else
					sprintf(str_Action,"");
				pwndStr_Act[i]->SetPropValueT("text",str_Action);
				pwndStr_Act[i]->Update();
			}
			
			if(pwndStr_Value_Now[i]!=NULL) // 現有值
			{
				sprintf(str_Temp, "%4d", Log->dNewValue);
				pwndStr_Value_Now[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Now[i]->Update();
			}
			if(pwndStr_Value_Old[i]!=NULL) // 原有值
			{
				sprintf(str_Temp, "%4d", Log->dOldValue);
				pwndStr_Value_Old[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Old[i]->Update();
			}
			
			if(pwndStr_Date[i]!=NULL) // 修改日期
			{
				sprintf(str_Date, "%0d/%02d/%02d"	, Log->dateStart.month, Log->dateStart.day, Log->dateStart.year%100);
				pwndStr_Date[i]->SetPropValueT("text",str_Date);
				pwndStr_Date[i]->Update();
			}
			if(pwndStr_Time[i]!=NULL) // 修改時間
			{
				sprintf(str_Time, "%0d:%02d:%02d"	, Log->timeStart.hour, Log->timeStart.minute, Log->timeStart.second);
				pwndStr_Time[i]->SetPropValueT("text",str_Time);
				pwndStr_Time[i]->Update();
			}
		}
		else
		{
			sprintf(str_Temp,"");
			if(pwndStr_No[i]!=NULL) // 序號
			{
				pwndStr_No[i]->SetPropValueT("text",str_Temp);
				pwndStr_No[i]->Update();
			}
			
			if(pwndStr_Act[i]!=NULL) // 描述
			{
				pwndStr_Act[i]->SetPropValueT("text",str_Temp);
				pwndStr_Act[i]->Update();
			}
			
			if(pwndStr_Value_Now[i]!=NULL) // 現有值
			{
				pwndStr_Value_Now[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Now[i]->Update();
			}
			if(pwndStr_Value_Old[i]!=NULL) // 原有值
			{
				pwndStr_Value_Old[i]->SetPropValueT("text",str_Temp);
				pwndStr_Value_Old[i]->Update();
			}
			
			if(pwndStr_Date[i]!=NULL) // 修改日期
			{
				pwndStr_Date[i]->SetPropValueT("text",str_Temp);
				pwndStr_Date[i]->Update();
			}
			if(pwndStr_Time[i]!=NULL) // 修改時間
			{
				pwndStr_Time[i]->SetPropValueT("text",str_Temp);
				pwndStr_Time[i]->Update();
			}
		}
	}
}