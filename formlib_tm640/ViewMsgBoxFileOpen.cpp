/*============================================================================+
|  Class    : ViewMagBox library                                            	|
|  Task     : ViewMagBox library action source file                          	|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : 	                                                     		|
|  Version  :                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

#include	"ViewMsgBoxFileOpen.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../tmshmsg.h"

/*========================================================+
|						Constants																			|
+========================================================*/

/*========================================================+
|						Global Variable																|
+========================================================*/

//�P�_�O�_���UENTER��
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnYes					= NULL;
CtmWnd*		u_pwndBtnNo						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndFileNameStatic	= NULL;


BOOL		WorkOne									=TRUE;		//�i�J�e���ɦ۰ʰ���A�Ȥ@��

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnYes			= pwndSender->FindControlFromName("btnYes");	//�s��
	u_pwndBtnNo 			= pwndSender->FindControlFromName("btnNo");		//�x�s
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");//����
	u_pwndFileNameStatic		= pwndSender->FindControlFromName("FileName");
	
	g_bMsgBoxFlag = FALSE;
	
	return TRUE;
}


WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	

		if(pwndTemp == u_pwndBtnYes)
		{
			g_bMsgBoxFlag = TRUE;
			system("rm -r temp/temp");
			SendMsg(MSG_SH_APP_CREATE_ONKEY,15616,0,NULL);
		}
		else if(pwndTemp == u_pwndBtnNo)
		{ 
			if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE)
			{
				g_bMsgBoxFlag = TRUE;
				SendMsg(MSG_SH_APP_CREATE_ONKEY,15872,0,NULL);
			}
			else
			{
				g_bMsgBoxFlag = FALSE;
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
			}
		}
		else if(pwndTemp == u_pwndBtnCancel)
			g_bMsgBoxFlag = FALSE;
			
		Exit();
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnYes)
		{
			SendMsg(MSG_SH_APP_CREATE_ONKEY,15616,0,NULL);
		}
		else if(pwndTemp == u_pwndBtnNo)
		{
			SendMsg(MSG_SH_APP_CREATE_ONKEY,15872,0,NULL);
		}
		else if(pwndTemp == u_pwndBtnCancel)
			g_bMsgBoxFlag = FALSE;
			
		Exit();
	}
	
	return wIDControl;
}
//20140624��

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;	
}


void		OnUpdateA(CtmWnd*	pwndSender)
{
	if(WorkOne)
	{
		char	szTemp[64]		= "\0";
		char	szTemp1[64]		= "\0";
		sprintf(szTemp,"%s", g_Now_FileName);
		CodeChange(szTemp1,szTemp );
		u_pwndFileNameStatic->SetPropValueT("text", szTemp1);
		u_pwndFileNameStatic->Update();
		WorkOne =FALSE;
	((CtmFormView*)pwndSender)->OnLoseFocus();			
	}
}

void    CodeChange(char *pDst, char *pSrc)		//���U�Ψ禡�A��r�`��Ƭ����r�`�B�z
{
	//��StaticX2����text�ݩʨϥ�(�ثe)
	WORD	*pwDst = (WORD *)pDst;
    
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}