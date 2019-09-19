/*============================================================================+
|  Class    : ViewMessageBoxTeachPoint library                               	|
|  Task     : ViewMessageBoxTeachPoint library action source file							|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxUserManage.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../selectedit.h"
#include	"../tmcontrol.h" 

/*========================================================+
|						Constants																			|
+========================================================*/

#define MaxUserMum 11

enum {NO_ACTION=0,ADD_USER,MODIFY_PWD,MODIFY_LEVEL,DELETE_USER};
/*========================================================+
|						Global Variable																|
+========================================================*/
char * DataID_UserId[]=
{
	"USER_HMI_NULL_NULL_USERMASTERID",
	"USER_HMI_NULL_NULL_USER1ID",
	"USER_HMI_NULL_NULL_USER2ID",
	"USER_HMI_NULL_NULL_USER3ID",
	"USER_HMI_NULL_NULL_USER4ID",
	"USER_HMI_NULL_NULL_USER5ID",
	"USER_HMI_NULL_NULL_USER6ID",
	"USER_HMI_NULL_NULL_USER7ID",
	"USER_HMI_NULL_NULL_USER8ID",
	"USER_HMI_NULL_NULL_USER9ID",
	"USER_HMI_NULL_NULL_USER10ID",
	NULL
};

char* SelEditStr[] =
{
	"str0",
	"str1",
	"str2",
	"str3",
	"str4",
	"str5",
	"str6",
	"str7",
	"str8",
	"str9",
	"str10",
	NULL
};

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey							= FALSE;

CtmWnd*		u_pwndBtnInsert					= NULL; // 新增
CtmWnd*		u_pwndBtnEdit					  = NULL; // 修改
CtmWnd*		u_pwndBtnDel 				    = NULL; // 刪除
CtmWnd*		u_pwndBtnOK			    		= NULL; // 確定
CtmWnd*		u_pwndBtnCancel					= NULL; // 取消
CtmWnd*		u_pwndBtnModifyPswd			= NULL; // 修改-密碼
CtmWnd*		u_pwndBtnModifyAuth			= NULL; // 修改-權限
CtmWnd*		u_pwndStaticMessage1		= NULL; // 請輸入帳號
CtmWnd*		u_pwndStaticMessage2		= NULL; // 請輸入密碼 / 帳號
CtmWnd*		u_pwndStaticMessage3		= NULL; // 請輸入權限 / 請輸入新密碼.權限
//CtmWnd*		u_pwndStaticMessage4		= NULL; // 權限不夠，不可執行動作
CtmWnd*		u_pwndEditUserName      = NULL; // 新增-輸入帳號
CtmWnd*		u_pwndEditPswd	        = NULL; // 新增-輸入密碼
CtmWnd*		u_pwndEditAuthority	    = NULL; // 新增-輸入權限 / 修改-權限
CtmWnd*		u_pwndSelectEditUserName= NULL; // 選擇帳號
CtmWnd*		u_pwndStaticMessage_Mark= NULL; // 遮蓋
CtmWnd*		u_pwndEditModifyPswd		= NULL; // 修改-密碼

//各元件taborder值
#define		INDEX_BTN_Insert							3
#define		INDEX_BTN_Edit								4
#define		INDEX_BTN_Del		 							5
#define		INDEX_BTN_ModifyPswd		 			6
#define		INDEX_BTN_ModifyAuth					7
#define		INDEX_BTN_OK									1
#define		INDEX_BTN_CANCEL							2
//#define		INDEX_StaticMessage_1					8
//#define		INDEX_StaticMessage_2					9
//#define		INDEX_StaticMessage_3					10
//#define		INDEX_StaticMessage_4					11
#define		INDEX_EDIT_UserName						12
#define		INDEX_EDIT_Pswd								13
#define		INDEX_EDIT_Authority					14
#define		INDEX_SELECTEDIT_UserName			15
#define		INDEX_EDIT_ModifyPswd					16

#define BTN_NOW_USE       0xFC00
#define BTN_NOW_NO_USE      0x0416
#define BTN_NO_WORK 0xC618 

int screenflag = NO_ACTION;
//int now_user_AUTH = 13; // 現在登入的使用者的權限大小
int modify_user_AUTH = 5; // 要修改的使用者的權限大小

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{ 
	//取得各元件指針
	u_pwndBtnInsert					= pwndSender->FindControlFromName("btnInsert");
	u_pwndBtnEdit						= pwndSender->FindControlFromName("btnEdit");
	u_pwndBtnDel						= pwndSender->FindControlFromName("btnDel");
	u_pwndBtnOK							= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel					= pwndSender->FindControlFromName("btnCancel");
	u_pwndBtnModifyPswd			= pwndSender->FindControlFromName("btnModifyPswd");
	u_pwndBtnModifyAuth			= pwndSender->FindControlFromName("btnModifyAuth");
	u_pwndStaticMessage1 		= pwndSender->FindControlFromName("StaticMessage1");
	u_pwndStaticMessage2 		= pwndSender->FindControlFromName("StaticMessage2");
	u_pwndStaticMessage3 		= pwndSender->FindControlFromName("StaticMessage3");
	//u_pwndStaticMessage4 		= pwndSender->FindControlFromName("StaticMessage4");
	u_pwndEditUserName    	= pwndSender->FindControlFromName("EditUserName");
	u_pwndEditPswd        	= pwndSender->FindControlFromName("EditPswd");
	u_pwndEditAuthority	    = pwndSender->FindControlFromName("EditAuthority");
	u_pwndSelectEditUserName= pwndSender->FindControlFromName("SelectEditUserName");
	u_pwndStaticMessage_Mark= pwndSender->FindControlFromName("StaticMessage_Mark");
	u_pwndEditModifyPswd		= pwndSender->FindControlFromName("EditModifyPswd");
	
	//u_pwndStaticMessage 		= pwndSender->FindControlFromName("StaticMessage");
	//u_pwndEditFileName	= pwndSender->FindControlFromName("EditFileName");		// 輸入新的檔案名稱
	//u_pwndBtnOK							= pwndSender->FindControlFromName("btnOK");
	//u_pwndBtnCancel					= pwndSender->FindControlFromName("btnCancel");
	u_pwndEditAuthority->SetPropValueT("max",g_iPrivilege-1);
	//u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_RENAME_FILENAME");

	/*u_pwndEditFileName->SetPropValueT("text", "1oo561351");
	u_pwndEditFileName->Update();*/
	//u_pwndStaticMessage_Mark->Update();
	
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndSelectEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditModifyPswd->SetPropValueT("taborder", -2);
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	return wIDControl;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	//((CtmFormView*)pwndSender)->OnLoseFocus();
	
	if(pwndTemp == u_pwndBtnInsert) // 新增
	{					
		screenflag = ADD_USER;
		((CtmFormView*)pwndSender)->OnLoseFocus();
		
		u_pwndBtnInsert->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnInsert->Update();
		u_pwndBtnEdit->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnEdit->Update();
		u_pwndBtnDel->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnDel->Update();
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditModifyPswd->SetPropValueT("taborder", -2);
		u_pwndSelectEditUserName->SetPropValueT("taborder", -2);
		
		u_pwndStaticMessage_Mark->Update();	
			
		u_pwndStaticMessage1->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage1->Update();
		
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_PASSWORD");
		u_pwndStaticMessage2->Update();
		
		u_pwndStaticMessage3->SetPropValueT("textID", "ROBOT_STR_LEVEL");
		u_pwndStaticMessage3->Update();
		
		u_pwndEditUserName->SetPropValueT("taborder", INDEX_EDIT_UserName);
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		
		u_pwndEditPswd->SetPropValueT("taborder", INDEX_EDIT_Pswd);
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();		
		
		u_pwndEditAuthority->SetPropValueT("taborder", INDEX_EDIT_Authority);
		u_pwndEditAuthority->SetPropValueT("value", (double)0);
		u_pwndEditAuthority->Update();
		
		u_pwndBtnOK->SetPropValueT("taborder", INDEX_BTN_OK);
		u_pwndBtnOK->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnOK->Update();

	}
	else if(pwndTemp == u_pwndBtnEdit) // 修改
	{
		screenflag = NO_ACTION;
		((CtmFormView*)pwndSender)->OnLoseFocus();
		
		u_pwndBtnInsert->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnInsert->Update();
		u_pwndBtnEdit->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnEdit->Update();
		u_pwndBtnDel->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnDel->Update();
		
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->SetPropValueT("value",(double)0);
		u_pwndEditAuthority->Update();
		u_pwndEditModifyPswd->SetPropValueT("text", "");
		u_pwndEditModifyPswd->Update();
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditModifyPswd->SetPropValueT("taborder", -2);
				
		u_pwndStaticMessage_Mark->Update();	
			
		u_pwndBtnModifyPswd->SetPropValueT("taborder", INDEX_BTN_ModifyPswd);
		u_pwndBtnModifyPswd->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnModifyPswd->Update();
		
		u_pwndBtnModifyAuth->SetPropValueT("taborder", INDEX_BTN_ModifyAuth);
		u_pwndBtnModifyAuth->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnModifyAuth->Update();
		
		u_pwndBtnOK->SetPropValueT("taborder", INDEX_BTN_OK);
		u_pwndBtnOK->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnOK->Update();
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnDel) // 刪除
	{
		screenflag = DELETE_USER;
		((CtmFormView*)pwndSender)->OnLoseFocus();
		
		u_pwndBtnInsert->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnInsert->Update();
		u_pwndBtnEdit->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnEdit->Update();
		u_pwndBtnDel->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnDel->Update();
		
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->SetPropValueT("value",(double)0);
		u_pwndEditAuthority->Update();
		u_pwndEditModifyPswd->SetPropValueT("text", "");
		u_pwndEditModifyPswd->Update();
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditModifyPswd->SetPropValueT("taborder", -2);
		
		u_pwndStaticMessage_Mark->Update();
				 
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage2->Update();
			
		u_pwndSelectEditUserName->SetPropValueT("taborder", INDEX_SELECTEDIT_UserName);	
		((CtmFormView*)pwndSender)->Goto(INDEX_SELECTEDIT_UserName);
		SetSelectEditID();
		u_pwndSelectEditUserName->UpdateAll();
		
		u_pwndBtnOK->SetPropValueT("taborder", INDEX_BTN_OK);
		u_pwndBtnOK->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnOK->Update();
		
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnModifyPswd) // 修改-密碼
	{		
		screenflag = MODIFY_PWD;
		((CtmFormView*)pwndSender)->OnLoseFocus();
		
		u_pwndEditAuthority->SetPropValueT("value",(double)0);
		u_pwndEditAuthority->Update();
		u_pwndEditModifyPswd->SetPropValueT("text", "");
		u_pwndEditModifyPswd->Update();			
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditModifyPswd->SetPropValueT("taborder", -2);
		
		u_pwndSelectEditUserName->SetPropValueT("taborder", INDEX_SELECTEDIT_UserName);	
		SetSelectEditID();
		u_pwndStaticMessage_Mark->Update();
		((CtmFormView*)pwndSender)->Goto(INDEX_SELECTEDIT_UserName);
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", INDEX_BTN_ModifyPswd);
		u_pwndBtnModifyPswd->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnModifyPswd->Update();
		
		u_pwndBtnModifyAuth->SetPropValueT("taborder", INDEX_BTN_ModifyAuth);
		u_pwndBtnModifyAuth->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnModifyAuth->Update();
		
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage2->Update();
		
		u_pwndStaticMessage3->SetPropValueT("textID", "ROBOT_STR_PASSWORD");
		u_pwndStaticMessage3->Update();
		
		u_pwndEditModifyPswd->SetPropValueT("taborder", INDEX_EDIT_ModifyPswd);
		u_pwndEditModifyPswd->Update();
		
		u_pwndSelectEditUserName->UpdateAll();
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnModifyAuth) // 修改-權限
	{
		screenflag = MODIFY_LEVEL;
		((CtmFormView*)pwndSender)->OnLoseFocus();
		
		u_pwndEditAuthority->SetPropValueT("value",(double)0);
		u_pwndEditAuthority->Update();
		u_pwndEditModifyPswd->SetPropValueT("text", "");
		u_pwndEditModifyPswd->Update();				
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditModifyPswd->SetPropValueT("taborder", -2);
		
		u_pwndSelectEditUserName->SetPropValueT("taborder", INDEX_SELECTEDIT_UserName);	
		SetSelectEditID();
		u_pwndStaticMessage_Mark->Update();
		
		u_pwndBtnModifyPswd->SetPropValueT("taborder", INDEX_BTN_ModifyPswd);
		u_pwndBtnModifyPswd->SetPropValueT("bgc", BTN_NOW_NO_USE);
		u_pwndBtnModifyPswd->Update();
		
		u_pwndBtnModifyAuth->SetPropValueT("taborder", INDEX_BTN_ModifyAuth);
		u_pwndBtnModifyAuth->SetPropValueT("bgc", BTN_NOW_USE);
		u_pwndBtnModifyAuth->Update();
		
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage2->Update();
		
		u_pwndStaticMessage3->SetPropValueT("textID", "ROBOT_STR_LEVEL");
		u_pwndStaticMessage3->Update();
		
		u_pwndEditAuthority->SetPropValueT("taborder", INDEX_EDIT_Authority);
		u_pwndEditAuthority->Update();
		
		u_pwndSelectEditUserName->UpdateAll();
		//((CtmFormView*)pwndSender)->Goto(taborder[INDEX_SELECTEDIT_UserName]);
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnOK) // 確認
	{
		if(screenflag == NO_ACTION) return;
		if(screenflag == ADD_USER) // 新增
		{
			char user_ID[20];
			memset(user_ID, '\0', sizeof(user_ID));
			u_pwndEditUserName->GetPropValueT("text", user_ID, sizeof(user_ID));
			char pswd[20];
			memset(pswd, '\0', sizeof(pswd));
			u_pwndEditPswd->GetPropValueT("text", pswd, sizeof(pswd));
			int authority;
			u_pwndEditAuthority->GetPropValueT("value", &authority, sizeof(authority));
			if(user_ID[0]=='\0')	//視窗：不得為空！
				MsgBoxCall("msgboxConfirm.txt","ROBOT_ALARM_SET_USER_ID");
			else if(pswd[0]=='\0')
				MsgBoxCall("msgboxConfirm.txt","ROBOT_MSG_ENTER_PASSWORD");
			else if(authority <=0)
				MsgBoxCall("msgboxConfirm.txt","ROBOT_ALARM_SET_LEVEL");
			else if(authority >= g_iPrivilege)
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			else
			{
				if(g_ptmControlServer->UserControl()->AddUser(user_ID,pswd,authority))
				{
					MsgBoxCall("msgboxConfirm.txt","ROBOT_MSG_INSERT_SUCCESS");
					Exit();		//關閉MsgBox
				}
				else
					MsgBoxCall("msgboxConfirm.txt","ROBOT_MSG_USER_EXIST");
			}
		}
		if(screenflag == DELETE_USER) // 刪除
		{	
			char oper_user_id[20];
			memset(oper_user_id,'\0',sizeof(oper_user_id));
			
			int OperUserIndex = ((CtmSelectEdit*)u_pwndSelectEditUserName)->GetIndex();
		  u_pwndSelectEditUserName->GetPropValueT(SelEditStr[OperUserIndex],oper_user_id,sizeof(oper_user_id));
			
			if(oper_user_id[0]==' ' || oper_user_id[0]=='\0' )
				MsgBoxCall("msgboxConfirm.txt","ROBOT_ALARM_NO_USER");
			else if((g_ptmControlServer->UserControl()->GetPrivilefe(oper_user_id) < 0 ) 
					 || (g_ptmControlServer->UserControl()->GetPrivilefe(oper_user_id) >= g_iPrivilege ))
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			else
			{
				if(g_ptmControlServer->UserControl()->DeleteUser(oper_user_id))
				{
					MsgBoxCall("msgboxConfirm.txt","ROBOT_MSG_DEL_SUCCESS");
					Exit();		//關閉MsgBox									
				}
				else 
					MsgBoxCall("msgboxConfirm.txt","ROBOT_ALARM_NO_USER");
			}
		}
		if(screenflag == MODIFY_PWD || screenflag == MODIFY_LEVEL) // 修改-密碼	 修改-權限		
		{	
			char oper_user_id[20];
			char* pwd_str = NULL;
			char pswd[20];
			int oper_user_level;
			int authority=-1;
			
			memset(oper_user_id,'\0',sizeof(oper_user_id));
			memset(pswd, '\0', sizeof(pswd));
			
			int OperUserIndex = ((CtmSelectEdit*)u_pwndSelectEditUserName)->GetIndex();
		  u_pwndSelectEditUserName->GetPropValueT(SelEditStr[OperUserIndex],oper_user_id,sizeof(oper_user_id));
			printf("OperUserIndex=%d, oper_user_id=%s\n",OperUserIndex,oper_user_id);
			printf("g_iPrivilege=%d, now=%d\n",g_iPrivilege,g_ptmControlServer->UserControl()->GetPrivilefe(oper_user_id));
			if(oper_user_id[0]==' ' || oper_user_id[0]=='\0' )
				MsgBoxCall("msgboxConfirm.txt","ROBOT_ALARM_NO_USER");
			else if((g_ptmControlServer->UserControl()->GetPrivilefe(oper_user_id) < 0 ) 
					 || (g_ptmControlServer->UserControl()->GetPrivilefe(oper_user_id) >= g_iPrivilege ))
				MsgBoxCall("msgboxConfirm.txt","ROBOT_STR_AUTHNOENOUGH");
			else
			{	
				if(screenflag == MODIFY_PWD)
				{
					pwd_str = &pswd[0];
					u_pwndEditModifyPswd->GetPropValueT("text", pswd, sizeof(pswd));
					if(pswd[0]=='\0')
						MsgBoxCall("msgboxConfirm.txt","ROBOT_MSG_ENTER_PASSWORD");
				}
				else if(screenflag == MODIFY_LEVEL)
					u_pwndEditAuthority->GetPropValueT("value", &authority, sizeof(authority));
					//ModifyUser pwd_str=NULL 表示修改權限  authority=-1 表示修改密碼
				if(g_ptmControlServer->UserControl()->ModifyUser(oper_user_id,pwd_str,authority))
				{
					MsgBoxCall("msgboxConfirm.txt","ROBOT_MSG_MODIFY_SUCCESS");
					Exit();		//關閉MsgBox
				}
			}
		}
		
	}
	else if(pwndTemp == u_pwndBtnCancel) // 取消
	{
		Exit();
	}		
	
	return wIDControl;
		
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	/*
	if(wKey == _ENTER)
		u_bEnterKey	= TRUE;
	else
		u_bEnterKey = FALSE;
	*/
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	return wKey; 
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	//static BOOL StartRunOne = TRUE;
	//if(StartRunOne)
	//	SetSelectEditID();
	//StartRunOne=FALSE;
}


/*========================================================+
|						Helper																				|
+========================================================*/


void SetSelectEditID()
{
 //g_ptmControlServer->UserControl()->IniUser();
 int UserNum=0;
 //int MaxUserMum = 10;
 for(int i=1;i<MaxUserMum;i++)
 {
  char UserID[7];
  memset(UserID,0,sizeof(UserID));
  GetDBString(DataID_UserId[i],UserID, sizeof(UserID));
  if(UserID[0]=='\0' || UserID[0]==' ') continue;
  UserNum++;
  u_pwndSelectEditUserName->SetPropValueT(SelEditStr[UserNum-1],UserID);
 }
 if(UserNum-1 >=5)
  u_pwndSelectEditUserName->SetPropValueT("listnum",5);
 else 
  u_pwndSelectEditUserName->SetPropValueT("listnum",UserNum);
  
 ((CtmSelectEdit*)u_pwndSelectEditUserName)->SetIndex(0);
 ((CtmSelectEdit*)u_pwndSelectEditUserName)->UpdateAll();
}

int GetOperUserLevel()
{
	
	
}