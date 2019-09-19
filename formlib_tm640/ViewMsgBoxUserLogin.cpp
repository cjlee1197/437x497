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


#include	"ViewMsgBoxUserLogin.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../selectedit.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define MaxUserMum 11
#define COLOR_GRAY			   	0xC618
/*========================================================+
|						Global Variable																|
+========================================================*/
char*			g_apszPasswdID[] = {
	"USER_HMI_NULL_NULL_USERMASTERPASSWORD",
	"USER_HMI_NULL_NULL_USER1PASSWORD",
	"USER_HMI_NULL_NULL_USER2PASSWORD",
	"USER_HMI_NULL_NULL_USER3PASSWORD",
	"USER_HMI_NULL_NULL_USER4PASSWORD",
	"USER_HMI_NULL_NULL_USER5PASSWORD",
	"USER_HMI_NULL_NULL_USER6PASSWORD",
	"USER_HMI_NULL_NULL_USER7PASSWORD",
	"USER_HMI_NULL_NULL_USER8PASSWORD",
	"USER_HMI_NULL_NULL_USER9PASSWORD",
	"USER_HMI_NULL_NULL_USER10PASSWORD"
};
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
enum TabOrderNum{Tab_SELECTEDITUSERID=0,Tab_EDITPWD,Tab_BTNMANAGE,Tab_END};
enum BGCNum{BGC_SELECTEDITUSERID=0,BGC_EDITPWD,BGC_BTNMANAGE,BGC_END};

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndStaticPassword					= NULL;
CtmWnd*		u_pwndBtnLogin				= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndBtnUseManage		= NULL;
CtmWnd*		u_pwndSelectEditUserId= NULL;
CtmWnd*		u_pwndEditPWD					= NULL;

int taborder[Tab_END];
int bgc[BGC_END];
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy↓
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndStaticPassword		= pwndSender->FindControlFromName("StaticUserPassword");
	u_pwndBtnLogin 					= pwndSender->FindControlFromName("BtnLogin");
	u_pwndBtnCancel					= pwndSender->FindControlFromName("BtnCancel");
	u_pwndSelectEditUserId	= pwndSender->FindControlFromName("SelectEditUserId");
	u_pwndEditPWD						= pwndSender->FindControlFromName("EditPassword");
	u_pwndBtnUseManage			= pwndSender->FindControlFromName("btnUserManager");
	if(g_iPrivilege>0)
	{
		
	}
	long value =0;
	for(int i=0;i<10;i++)
	{
		value = GetDBValue(g_apszPasswdID[i]).lValue;
		printf("%s=%d\n",g_apszPasswdID[i],value);
	}
	
	return TRUE;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

		if(pwndTemp == u_pwndBtnCancel)
		{
			Exit();
		}
		else if(pwndTemp == u_pwndBtnLogin) // 登入 或 登出
		{
			BOOL logincheck;
			printf("g_iPrivilege=%d\n",g_iPrivilege);
			if(g_iPrivilege==0) // 登入
				logincheck = LoginUser();
			else // 登出
				g_iPrivilege=0;
			LoginSwitch(pwndSender,g_iPrivilege); // 轉換使用者
			
			char UserID[7];	// 顯示當前使用者
			memset(UserID,0,sizeof(UserID));
			GetDBString(DataID_UserId[((CtmSelectEdit*)u_pwndSelectEditUserId)->GetIndex()],UserID, sizeof(UserID));
			g_pMainFrame->SetUserID(UserID);
			printf("UserID=%s\n",UserID);

//			if(logincheck) // 登入正確離開
//				Exit();
		}
		else if(pwndTemp == u_pwndBtnUseManage) // 用戶管理
		{
			MsgBoxCall("msgboxUserManage.txt",1,NULL);
			SetSelectEditID();
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
		u_pwndSelectEditUserId->GetPropValueT("taborder",&(taborder[Tab_SELECTEDITUSERID]),sizeof(taborder[Tab_SELECTEDITUSERID]));
		u_pwndEditPWD->GetPropValueT("taborder",&(taborder[Tab_EDITPWD]),sizeof(taborder[Tab_EDITPWD]));
		u_pwndBtnUseManage->GetPropValueT("taborder",&(taborder[Tab_BTNMANAGE]),sizeof(taborder[Tab_BTNMANAGE]));
		u_pwndSelectEditUserId->GetPropValueT("bgc",&(bgc[BGC_SELECTEDITUSERID]),sizeof(bgc[BGC_SELECTEDITUSERID]));
		u_pwndEditPWD->GetPropValueT("bgc",&(bgc[BGC_EDITPWD]),sizeof(bgc[BGC_EDITPWD]));
		u_pwndBtnUseManage->GetPropValueT("bgc",&(bgc[BGC_BTNMANAGE]),sizeof(bgc[BGC_BTNMANAGE]));
		
		SetSelectEditID();
		StartRunOne=FALSE;
		
		LoginSwitch(pwndSender,g_iPrivilege);
	}
}
/*========================================================+
|						Helper																				|
+========================================================*/

void SetSelectEditID()
{
	//g_ptmControlServer->UserControl()->IniUser();
	int UserNum=0;
	for(int i=1;i<MaxUserMum;i++)
	{
		char UserID[7];
		memset(UserID,0,sizeof(UserID));
		GetDBString(DataID_UserId[i],UserID, sizeof(UserID));
		if(UserID[0]=='\0' || UserID[0]==' ')	continue;
		UserNum++;
		((CtmSelectEdit*)u_pwndSelectEditUserId)->SetPropValueT(SelEditStr[UserNum-1],UserID);
	}
	if(UserNum-1 >=5)
		u_pwndSelectEditUserId->SetPropValueT("listnum",5);
	else 
		u_pwndSelectEditUserId->SetPropValueT("listnum",UserNum);
	((CtmSelectEdit*)u_pwndSelectEditUserId)->SetIndex(0);
	((CtmSelectEdit*)u_pwndSelectEditUserId)->UpdateAll();
}

BOOL LoginUser()
{
	printf("LoginUser\n");
	int passwd;
	char UserID[12];
	char passwdstr[12];
	memset(UserID,0,sizeof(UserID));
	memset(passwdstr,0,sizeof(passwdstr));
	u_pwndSelectEditUserId->GetPropValueT(
	                        SelEditStr[((CtmSelectEdit*)u_pwndSelectEditUserId)->GetIndex()],
	                        UserID,sizeof(UserID));
	u_pwndEditPWD->GetPropValueT("text", passwdstr, sizeof(passwdstr));
	printf("User=%s,PS=%s\n",UserID,passwdstr);
	int level = g_ptmControlServer->UserControl()->LoginLocal(UserID,passwdstr);
	printf("level=%d\n",level);
	if(level>0 && level <= 16)
	{
		g_iPrivilege = level;
		return TRUE;
	}
	else level=0;
	return FALSE;
}


void LoginSwitch(CtmWnd* pwndSender,int level) // 轉換使用者
{
	if(level>0)
	{
		int userindex;
		userindex = g_ptmControlServer->UserControl()->GetUserNum();
		((CtmSelectEdit*)u_pwndSelectEditUserId)->SetIndex(userindex);
		((CtmFormView*)pwndSender)->OnLoseFocus();
		u_pwndBtnLogin->SetPropValueT("captionID","ROBOT_STR_LOGIN_OUT");
		u_pwndBtnCancel->SetPropValueT("captionID","ROBOT_BTN_OK");
		u_pwndSelectEditUserId->SetPropValueT("taborder",-2);
		u_pwndSelectEditUserId->SetPropValueT("bgc",COLOR_GRAY);
		u_pwndEditPWD->SetPropValueT("taborder",-2);
		char level_str[8];
		sprintf(level_str,"%d",level);
		u_pwndEditPWD->SetPropValueT("text",level_str);
		u_pwndEditPWD->SetPropValueT("bgc",COLOR_GRAY);
		u_pwndEditPWD->SetPropValueT("type",4); //level
		u_pwndStaticPassword->SetPropValueT("textID","ROBOT_STR_LEVEL");
	}
	else
	{
		((CtmSelectEdit*)u_pwndSelectEditUserId)->SetIndex(0);
		u_pwndBtnLogin->SetPropValueT("captionID","ROBOT_STR_LOGIN");
		u_pwndBtnCancel->SetPropValueT("captionID","ROBOT_BTN_CANCEL");
		u_pwndSelectEditUserId->SetPropValueT("taborder",taborder[Tab_SELECTEDITUSERID]);
		((CtmFormView*)pwndSender)->Goto(taborder[Tab_SELECTEDITUSERID]);
		u_pwndSelectEditUserId->SetPropValueT("bgc",bgc[BGC_SELECTEDITUSERID]);
		u_pwndEditPWD->SetPropValueT("taborder",taborder[Tab_EDITPWD]);
		u_pwndEditPWD->SetPropValueT("text","");
		u_pwndEditPWD->SetPropValueT("bgc",bgc[BGC_EDITPWD]);
		u_pwndEditPWD->SetPropValueT("type",132);	//password
		u_pwndStaticPassword->SetPropValueT("textID","ROBOT_STR_PASSWORD");
		
	}
	u_pwndBtnLogin->CreateA();
	u_pwndBtnLogin->Update();
	u_pwndBtnCancel->CreateA();
	u_pwndBtnCancel->Update();
	//u_pwndSelectEditUserId->CreateA();
	u_pwndSelectEditUserId->UpdateAll();
	u_pwndStaticPassword->Update();
	u_pwndEditPWD->Update();
	
	
	if(level>10) // 權級10以上 可修改用戶管理
	{
		u_pwndBtnUseManage->SetPropValueT("taborder",taborder[Tab_BTNMANAGE]);
		u_pwndBtnUseManage->SetPropValueT("bgc",bgc[BGC_BTNMANAGE]);
	}
	else
	{
		u_pwndBtnUseManage->SetPropValueT("taborder",-2);
		u_pwndBtnUseManage->SetPropValueT("bgc",COLOR_GRAY);
	}
	u_pwndBtnUseManage->Update();
}