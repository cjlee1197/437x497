/*===========================================================================+
|  Class    : ViewDBChoose library                                     		   |
|  Task     : ViewDBChoose library action source file              		       |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 										                                           |
|  Version  : V1.00      			                                               |
|  Creation : 			                                            		         |
|  Revision :   																														 |
+===========================================================================*/
#include 	"ViewDBChoose.h"
#include	"../main.h"
#include	"../_kb.h"
#include 	"../common.h"
#include 	"../commonaction.h"
#include	"../timer.h"
#include	"../utils.h"
#include	"../manufacture.h"
#include	"../tmdebug.h"
#include	"../selvat_ancestor.h"
#include	"../tmconfig.h"
#include	"../lib/libiNetComm.h"
#include	"../tmshmsg.h"
#include	"../lib/libControl.h"
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次

CtmWnd*		pwndStaticText			= NULL; // Static 顯示文字
CtmWnd*		pwndBtnFlwTP				= NULL; // Btn 以示教器為主
CtmWnd*		pwndBtnFlwCON				= NULL; // Btn 以控制器為主
CtmWnd*		pwndValue_TP				= NULL; // Edit 示教器數值
CtmWnd*		pwndValue_CON				= NULL; // Edit 控制器數值
CtmWnd*		pwndBtnAllFlwTP			= NULL; // Btn 全部以示教器為主
CtmWnd*		pwndBtnAllFlwCON		= NULL; // Btn 全部以控制器為主

CtmWnd*		pwndValue_497				= NULL; // 示教器數值 顯示文字
CtmWnd*		pwndValue_28				= NULL; // 示教器數值 顯示文字
/*---------------------------------------------------------------------------+
|           View Content - GuideSet                                          |
+---------------------------------------------------------------------------*/ 

/*---------------------------------------------------------------------------+
|  Function : OnCreateA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndStaticText			= pwndSender->FindControlFromName("StaticText");
	pwndBtnFlwTP				= pwndSender->FindControlFromName("BtnFlwTP");
	pwndBtnFlwCON				= pwndSender->FindControlFromName("BtnFlwCON");
	pwndBtnAllFlwTP			= pwndSender->FindControlFromName("BtnAllFlwTP");
	pwndBtnAllFlwCON		= pwndSender->FindControlFromName("BtnAllFlwCON");
	
	pwndValue_TP				= pwndSender->FindControlFromName("Value_TP");
	pwndValue_CON				= pwndSender->FindControlFromName("Value_CON");
	
	pwndValue_497				= pwndSender->FindControlFromName("Value_497");
	pwndValue_28				= pwndSender->FindControlFromName("Value_28");
	
	// 若以選擇 全部套用 1:示教器 或 2:控制器 參數
	int iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72").lValue;
	if(iDBSelect > 0) // 全部套用1:示教器 或 2:控制器
	{
  	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", iDBSelect); // 1:以示教器為主 2:以控制器為主
  	Exit();
	}
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pWnd = NULL;

	pWnd = pwndSender->FindControlFromTab(wIDControl);
	if(pWnd == NULL) return wIDControl;
		
	return wIDControl;
}

/*---------------------------------------------------------------------------+
|  Function : OnKeyA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標
	
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

 	if(pwnd == pwndBtnFlwTP) // 以示教器為主
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 1); // 1:以示教器為主 2:以控制器為主
    	Exit();
    }
	}
 	if(pwnd == pwndBtnFlwCON) // 以控制器為主
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 2); // 1:以示教器為主 2:以控制器為主
    	Exit();
    }
	}
 	if(pwnd == pwndBtnAllFlwTP) // 全部以示教器為主
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 1); // 1:以示教器為主 2:以控制器為主
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72", 1); // 1:全部以示教器為主 2:全部以控制器為主
    	Exit();
    }
	}
 	if(pwnd == pwndBtnAllFlwCON) // 全部以控制器為主
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 2); // 1:以示教器為主 2:以控制器為主
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72", 2); // 1:全部以示教器為主 2:全部以控制器為主
    	Exit();
    }
	}
 	
  return wIDControl;	
}




/*---------------------------------------------------------------------------+
|  Function : OnDestroyA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnDestroyA(CtmWnd* pwndSender)
{

}

/*---------------------------------------------------------------------------+
|  Function : OnShowA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnShowA(CtmWnd* pwndSender)
{

}

/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
	{
		RunOnlyOne=TRUE;
		((CtmFormView*)pwndSender)->OnLoseFocus(); // 取消光標
		
		pwndValue_497->SetPropValueT("text", gstr_DBValue_497);
		pwndValue_497->Update();
		pwndValue_28->SetPropValueT("text", gstr_DBValue_28);
		pwndValue_28->Update();
	}
}
