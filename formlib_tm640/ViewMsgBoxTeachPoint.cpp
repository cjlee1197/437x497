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
#include	"ViewMsgBoxTeachPoint.h"
#include	"../utils.h"
#include	"../commonaction.h"

/*========================================================+
|						Constants																			|
+========================================================*/

#define		INDEX_EDIT_TEACHPOINT_NO		0

//�P�_�r�����
#define		TeachPoint_TYPE_Delete				1
#define		TeachPoint_TYPE_Update				2
#define		TeachPoint_TYPE_Move					3

/*========================================================+
|						Global Variable																|
+========================================================*/

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey							= FALSE;

CtmWnd*		u_pwndStaticMessage			= NULL;
CtmWnd*		u_pwndEditTeachPointNo	= NULL;
CtmWnd*		u_pwndBtnOK							= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//���o�U������w
	u_pwndStaticMessage 		= pwndSender->FindControlFromName("StaticMessage");
	u_pwndEditTeachPointNo	= pwndSender->FindControlFromName("EditTeachPointNo");
	u_pwndBtnOK							= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel					= pwndSender->FindControlFromName("btnCancel");
	

	
	//20140910��  by�@Mario    
	switch(g_nTeachPointType)		//����msg��ܪ��r��
	{
		case	TeachPoint_TYPE_Delete:
			//�R���G
			u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_ENTER_DELETE_TEACHPOINT");
			break;
		case	TeachPoint_TYPE_Update:
			//�s��
			u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_ENTER_UPDATE_TEACHPOINT");
			break;
		case	TeachPoint_TYPE_Move:
			//���ܱ��I
			u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_ENTER_MOVE_TEACHPOINT");
			break;
		default:
			break;
	}
	//�˵����I���w�]
	//u_pwndStaticMessage->SetPropValueT("textID", "ROBOT_MSG_ENTER_TEACHPOINT");
	
	g_wTeachPointNo =-1;		//�M�ŨϥΪ̿�J���о��I�s��
	
		u_pwndEditTeachPointNo->SetPropValueT("max", GetTeachPointCnt());
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{/*
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnOK)
		{
			//���o�ϥΪ̿�J���о��I�s��
			u_pwndEditTeachPointNo->GetPropValueT("value", &g_wTeachPointNo, sizeof(g_wTeachPointNo));
			
			//����"MAKER_PARAM0"->�x�s�о��I�ƶq������Ѽ�
			int	nTeachPointCnt = GetDBValue("MAKER_PARAM0").lValue;
			
			//���J���s��������A�H�KŪ������~����T
			if((g_wTeachPointNo <= 0)||(g_wTeachPointNo > nTeachPointCnt))
			{
				//�����G���о��I���s�b�I
				MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NULL_TEACHPOINT"], tmFT_CODE_TECH);
				((CtmFormView*)pwndSender)->Goto(INDEX_EDIT_TEACHPOINT_NO);
			}
			else
				Exit();
		}
		if(pwndTemp == u_pwndBtnCancel)
		{
			g_wTeachPointNo =0;
			Exit();
		}
	}*/
	return wIDControl;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
		((CtmFormView*)pwndSender)->Goto(wIDControl);
		if(pwndTemp == u_pwndBtnOK)
		{
			//���o�ϥΪ̿�J���о��I�s��
			u_pwndEditTeachPointNo->GetPropValueT("value", &g_wTeachPointNo, sizeof(g_wTeachPointNo));
				Exit();
		}
		else if(pwndTemp == u_pwndBtnCancel)
		{
			g_wTeachPointNo =-1;
			Exit();
		}
}
WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey	= TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey; 
}

/*========================================================+
|						Helper																				|
+========================================================*/

