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

/*========================================================+
|						Constants																			|
+========================================================*/


/*========================================================+
|						Global Variable																|
+========================================================*/

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey							= FALSE;

CtmWnd*		u_pwndBtnInsert					= NULL; // �s�W
CtmWnd*		u_pwndBtnEdit					  = NULL; // �ק�
CtmWnd*		u_pwndBtnDel 				    = NULL; // �R��
CtmWnd*		u_pwndBtnOK			    		= NULL; // �T�w
CtmWnd*		u_pwndBtnCancel					= NULL; // ����
CtmWnd*		u_pwndBtnModifyPswd			= NULL; // �ק�-�K�X
CtmWnd*		u_pwndBtnModifyAuth			= NULL; // �ק�-�v��
CtmWnd*		u_pwndStaticMessage1		= NULL; // �п�J�b��
CtmWnd*		u_pwndStaticMessage2		= NULL; // �п�J�K�X / �b��
CtmWnd*		u_pwndStaticMessage3		= NULL; // �п�J�v�� / �п�J�s�K�X.�v��
CtmWnd*		u_pwndStaticMessage4		= NULL; // �v�������A���i����ʧ@
CtmWnd*		u_pwndEditUserName      = NULL; // �s�W-��J�b��
CtmWnd*		u_pwndEditPswd	        = NULL; // �s�W-��J�K�X
CtmWnd*		u_pwndEditAuthority	    = NULL; // �s�W-��J�v�� / �ק�-�K�X.�v��
CtmWnd*		u_pwndSelectEditUserName= NULL; // ��ܱb��
CtmWnd*		u_pwndStaticMessage_Mark= NULL; // �B�\

//�U����taborder��
#define		INDEX_BTN_Insert							3
#define		INDEX_BTN_Edit								4
#define		INDEX_BTN_Del		 							5
#define		INDEX_BTN_ModifyPswd		 			6
#define		INDEX_BTN_ModifyAuth					7
#define		INDEX_BTN_OK									1
#define		INDEX_BTN_CANCEL							2
#define		INDEX_StaticMessage_1					8
#define		INDEX_StaticMessage_2					9
#define		INDEX_StaticMessage_3					10
#define		INDEX_StaticMessage_4					11
#define		INDEX_EDIT_EditUserName				12
#define		INDEX_EDIT_EditPswd						13
#define		INDEX_EDIT_EditAuthority			14
#define		INDEX_SELECTEDIT_SelectEditUserName			15

#define PTP_LINE_SEL       0xFC00
#define PTP_LINE_NO_SEL      0x0416

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{ 
	//���o�U������w
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
	u_pwndStaticMessage4 		= pwndSender->FindControlFromName("StaticMessage4");
	u_pwndEditUserName    	= pwndSender->FindControlFromName("EditUserName");
	u_pwndEditPswd        	= pwndSender->FindControlFromName("EditPswd");
	u_pwndEditAuthority	    = pwndSender->FindControlFromName("EditAuthority");
	u_pwndSelectEditUserName= pwndSender->FindControlFromName("SelectEditUserName");
	u_pwndStaticMessage_Mark= pwndSender->FindControlFromName("StaticMessage_Mark");

	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_pwndBtnOK)
	{
		char user_ID[20] = "\0";
		memset(user_ID, '\0', sizeof(user_ID));
		u_pwndEditUserName->GetPropValueT("text", user_ID, sizeof(user_ID));
	}
	if(pwndTemp == u_pwndBtnCancel)
	{
		Exit();
	}
	//Exit();
	return wIDControl;
}



WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == u_pwndBtnInsert)
	{					
		u_pwndBtnInsert->SetPropValueT("bgc", PTP_LINE_SEL);
		u_pwndBtnInsert->Update();
		u_pwndBtnEdit->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnEdit->Update();
		u_pwndBtnDel->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnDel->Update();
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyPswd->Update();
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->Update();
		u_pwndSelectEditUserName->SetPropValueT("taborder", -2);
		u_pwndSelectEditUserName->Update();
		//u_pwndStaticMessage_Mark->Update();
		u_pwndStaticMessage_Mark->Update();		
		u_pwndStaticMessage1->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_PASSWORD");
		u_pwndStaticMessage3->SetPropValueT("textID", "ROBOT_STR_LEVEL");
		u_pwndEditUserName->SetPropValueT("taborder", INDEX_EDIT_EditUserName);
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditPswd->SetPropValueT("taborder", INDEX_EDIT_EditPswd);
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditAuthority->SetPropValueT("taborder", INDEX_EDIT_EditAuthority);
		u_pwndEditAuthority->SetPropValueT("text", "");
		u_pwndStaticMessage1->Update();
		u_pwndStaticMessage2->Update();
		u_pwndStaticMessage3->Update();
		u_pwndEditUserName->Update();
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->Update();		
		
		u_pwndBtnOK->SetPropValueT("taborder", INDEX_BTN_OK);
		u_pwndBtnOK->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnOK->Update();
		u_pwndBtnCancel->Update();
		//Exit();
	}/*
	else if(pwndTemp == u_pwndBtnEdit)
	{
		//printf("u_pwndBtnEdit");
		u_pwndBtnInsert->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnInsert->Update();
		u_pwndBtnEdit->SetPropValueT("bgc", PTP_LINE_SEL);
		u_pwndBtnEdit->Update();
		u_pwndBtnDel->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnDel->Update();
		u_pwndBtnModifyPswd->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnModifyPswd->Update();
		u_pwndBtnModifyAuth->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnModifyAuth->Update();
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("text", "");
		u_pwndEditAuthority->Update();
		
		u_pwndStaticMessage_Mark->Update();
		u_pwndBtnModifyPswd->SetPropValueT("taborder", INDEX_BTN_ModifyPswd);
		u_pwndBtnModifyPswd->SetPropValueT("caption", "ROBOT_STR_EDITPSD");
		u_pwndBtnModifyAuth->SetPropValueT("taborder", INDEX_BTN_ModifyAuth);
		u_pwndBtnModifyAuth->SetPropValueT("caption", "ROBOT_STR_EDITLEV");
		u_pwndBtnModifyPswd->Update();
		u_pwndBtnModifyAuth->Update();
		u_pwndBtnOK->Update();
		u_pwndBtnCancel->Update();
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnDel)
	{
		u_pwndBtnInsert->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnInsert->Update();
		u_pwndBtnEdit->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnEdit->Update();
		u_pwndBtnDel->SetPropValueT("bgc", PTP_LINE_SEL);
		u_pwndBtnDel->Update();
		//printf("u_pwndBtnDel");
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		u_pwndEditPswd->SetPropValueT("taborder", -2);
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->SetPropValueT("taborder", -2);
		u_pwndEditAuthority->SetPropValueT("text", "");
		u_pwndEditAuthority->Update();
		u_pwndBtnModifyPswd->SetPropValueT("taborder", -2);
		u_pwndBtnModifyPswd->Update();
		u_pwndBtnModifyAuth->SetPropValueT("taborder", -2);
		u_pwndBtnModifyAuth->Update();
		
		u_pwndStaticMessage_Mark->Update();
				
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage2->Update();
		
		u_pwndSelectEditUserName->Update();
		
		u_pwndBtnOK->Update();
		u_pwndBtnCancel->Update();
		
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnModifyPswd)
	{		
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		u_pwndEditPswd->SetPropValueT("taborder", INDEX_EDIT_EditPswd);
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->SetPropValueT("taborder", INDEX_EDIT_EditAuthority);
		u_pwndEditAuthority->SetPropValueT("text", "");
		u_pwndEditAuthority->Update();
		
		u_pwndBtnModifyPswd->SetPropValueT("bgc", PTP_LINE_SEL);
		u_pwndBtnModifyPswd->Update();
		u_pwndBtnModifyAuth->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnModifyAuth->Update();
		//u_pwndStaticMessage_Mark->Update();
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage3->SetPropValueT("textID", "ROBOT_STR_PASSWORD");
		//printf("u_pwndBtnModifyPswd");
		u_pwndStaticMessage2->Update();
		u_pwndStaticMessage3->Update();
		//u_pwndSelectEditUserName->UpdateALL();
		//u_pwndSelectEditUserName->CreateA();
		u_pwndSelectEditUserName->Update();
		u_pwndEditAuthority->Update();
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnModifyAuth)
	{
		u_pwndEditUserName->SetPropValueT("taborder", -2);
		u_pwndEditUserName->SetPropValueT("text", "");
		u_pwndEditUserName->Update();
		u_pwndEditPswd->SetPropValueT("taborder", INDEX_EDIT_EditPswd);
		u_pwndEditPswd->SetPropValueT("text", "");
		u_pwndEditPswd->Update();
		u_pwndEditAuthority->SetPropValueT("taborder", INDEX_EDIT_EditAuthority);
		u_pwndEditAuthority->SetPropValueT("text", "");
		u_pwndEditAuthority->Update();
		
		u_pwndBtnModifyPswd->SetPropValueT("bgc", PTP_LINE_NO_SEL);
		u_pwndBtnModifyPswd->Update();
		u_pwndBtnModifyAuth->SetPropValueT("bgc", PTP_LINE_SEL);
		u_pwndBtnModifyAuth->Update();
		//printf("u_pwndBtnModifyAuth");
		u_pwndStaticMessage2->SetPropValueT("textID", "ROBOT_STR_USER_ID");
		u_pwndStaticMessage2->Update();
		u_pwndStaticMessage3->SetPropValueT("textID", "ROBOT_STR_LEVEL");
		u_pwndStaticMessage3->Update();
		u_pwndSelectEditUserName->Update();
		u_pwndEditAuthority->Update();
		//Exit();
	}
	else if(pwndTemp == u_pwndBtnOK)
	{
		printf("u_pwndBtnOK");
		//char GetPassword[20] = "\0";
		//memset(GetPassword, 0, sizeof(GetPassword));
		//u_pwndEditTeachPointNo->GetPropValueT("text", GetPassword, sizeof(GetPassword));
		//if(!strcmp(GetPassword,"1111"))
		//{
		//	g_Pass = TRUE;
		//	Exit();
		//}
		//else
		//{
		// 	u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_PASSWORD_ERROR");
		// 	u_pwndStaticMessage->Update();
		//}
		Exit();
	}*/
	else if(pwndTemp == u_pwndBtnCancel)
	{
		Exit();
	}
		
	/*
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnOK)
	{
		//u_pwndEditFileName->GetPropValueT("text", g_sOperate_FileName, sizeof(g_sOperate_FileName));	
		u_pwndEditFileName->GetPropValueT("text", &g_Now_FileName, sizeof(g_Now_FileName));
		printf("111.g_Now_FileName = %s\n",g_Now_FileName);
		u_pwndEditFileName->SetPropValueT("text", "sgdsgrsrsg");
		u_pwndEditFileName->Update();
		u_pwndEditFileName->GetPropValueT("text", &g_Now_FileName, sizeof(g_Now_FileName));
		printf("222.g_Now_FileName = %s\n",g_Now_FileName);
		if(g_Now_FileName==0)
		{
			//�����G��J���ɦW���o���šI
			MsgBoxCall("msgboxConfirm1.txt","ROBOT_MSG_EDIT_FILENAME_NULL");
			((CtmFormView*)pwndSender)->Goto(INDEX_EDIT_FILENAME);		//����Taborder�Ȩ̾ڲ��^���w������
		}
		else
			Exit();		//����MsgBox
	}
	else if(pwndTemp == u_pwndBtnCancel)
	{
		memset(g_Now_FileName, '\0', sizeof(g_Now_FileName));
		//g_Now_FileName='\0';
		Exit();		//����MsgBox
	}
	*/
	
	return wIDControl;
		
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
/*	if(wKey == _ENTER)
		u_bEnterKey	= TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	*/
	return wKey; 
}

/*========================================================+
|						Helper																				|
+========================================================*/

