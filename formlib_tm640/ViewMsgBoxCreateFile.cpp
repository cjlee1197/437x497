/*============================================================================+
|  Class    : ViewMagBox library                                             	|
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

#include	"ViewMsgBoxCreateFile.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../memo.h"

/*========================================================+
|						Constants																			|
+========================================================*/

//20140704↓
/*//ROY：判斷光標停留在哪個元件上
#define		INDEX_EDIT_CREATE_FILENAME	0
#define		INDEX_BTN_OK								1
#define		INDEX_BTN_CANCEL						2*/
//20140704↑

/*========================================================+
|						Global Variable																|
+========================================================*/

CtmWnd*	u_pwndTextMemoFile		= NULL;
CtmWnd*	u_pwndBtnPageDown			= NULL;
CtmWnd*	u_pwndBtnPageUp				= NULL;

//20140704↓
/*BOOL			u_bEnterKey									= FALSE;	//ROY：判斷Enter鍵是否按下

char			u_sCreate_NewFileName[64]		= "\0";		//ROY：存放使用者輸入的檔案名稱

CtmWnd*		u_pwndStaticText						= NULL;
CtmWnd*		u_pwndEditCreateFileName		= NULL;
CtmWnd*		u_pwndBtnOK									= NULL;
CtmWnd*		u_pwndBtnCancel							= NULL;*/
//20140704↑

/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndTextMemoFile	= pwndSender->FindControlFromName("TextMemoFile");
	u_pwndBtnPageDown		= pwndSender->FindControlFromName("btnPageDown");
	u_pwndBtnPageUp			= pwndSender->FindControlFromName("btnPageUp");
	
	ShowDocument(0);
	
	return	TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	return	wIDControl;
}

WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	return	wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == 0x3D00)
	{
		Exit();
	}
	else
		((CtmFormView*)pwndSender)->OnKey1(wKey);
		
	return	wKey;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	printf("MsgBox is Running！\n");
}

//20140704↓
/*BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//ROY：獲取各元件指針
	u_pwndStaticText						= pwndSender->FindControlFromName("StaticText");
	u_pwndEditCreateFileName		= pwndSender->FindControlFromName("EditCreateFileName");
	u_pwndBtnOK									= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel							= pwndSender->FindControlFromName("btnCancel");
	
	//ROY："_WORK_REFDOC"為字串"建立檔案說明文件"
	u_pwndStaticText->SetPropValueT("textID", "_WORK_REFDOC");
	
	//printf("CreateA!\n");
	
	return true;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp = NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndEditCreateFileName)
		{
			u_pwndEditCreateFileName->GetPropValueT("text", g_sOperate_FileName, sizeof(g_sOperate_FileName));
			if(g_sOperate_FileName[0] == '\0')
			{
				MsgBox(g_MultiLanguage["_WORK_FILE_COPY_NULL"], tmFT_CODE_TECH);
				//printf("New FileName = NULL!\n");
				((CtmFormView*)pwndSender)->Goto(INDEX_EDIT_CREATE_FILENAME);
			}
			//printf("New FileName = %s\n", g_sOperate_FileName);
		}
		else if(pwndTemp == u_pwndBtnOK)
		{
			Exit();
		}
		else if(pwndTemp == u_pwndBtnCancel)
		{
			memset(g_sOperate_FileName, 0, sizeof(g_sOperate_FileName));
			//printf("Cancel! New FileName = %s\n", g_sOperate_FileName);
			Exit();
		}
	}
	return wIDControl;
}

WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp = NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	//printf("Old FileName: %s\n", g_sOperate_FileName);
	
	//if(u_bEnterKey)
	//{
		//if(pwndTemp == u_pwndEditCreateFileName)
		//{
			//u_pwndEditCreateFileName->GetPropValueT("text", g_sOperate_FileName, sizeof(g_sOperate_FileName));
			//if(g_sOperate_FileName[0] == '\0')
			//{
				//MsgBox(g_MultiLanguage["_WORK_FILE_COPY_NULL"], tmFT_CODE_TECH);
				//((CtmFormView*)pwndSender)->Goto(1);
				//u_pwndEditCreateFileName->Update();
				//printf("Update Over!\n");
				//return wIDControl;
			//}
			//printf("New FileName: %s\n", g_sOperate_FileName);
			
		//}
		//else if(pwndTemp == u_pwndBtnOK)
		//{
			//printf("OK!\n");
			//Exit();
		//}
		//else if(pwndTemp == u_pwndBtnCancel)
		//{
			//printf("Cancel!\n");
			//Exit();
		//}
	//}
	
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
		
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
		
	//20140616↓ ROY：原先取自ViewMsgBox.cpp
	//g_bMsgBoxFlag =FALSE;
	//if(wKey == _ENTER)
	//{
		//g_bMsgBoxFlag =TRUE;
		//Exit();
	//}
	//else if(wKey == 0x0E08)
	//{
		//g_bMsgBoxFlag =FALSE;
		//Exit();
	//}
	//else
		//((CtmFormView*)pwndSender)->OnKey1(wKey);		
	//20140616↑
	
	return wKey;
}*/
//20140704↑

/*========================================================+
|						Helper																				|
+========================================================*/

void		ShowDocument(int nStartNo)		//顯示說明文件
{
	if(u_pwndTextMemoFile != NULL)
	{
		((CtmTextMemo*)u_pwndTextMemoFile)->LoadFromFile("./CommandExplanation/English", nStartNo);
		((CtmTextMemo*)u_pwndTextMemoFile)->Update();
	}
}

void		SaveDocument()		//儲存說明文件，暫時沒用到
{
	if(u_pwndTextMemoFile != NULL)
		((CtmTextMemo*)u_pwndTextMemoFile)->SaveToFile("./CommandExplanation/English");
}
