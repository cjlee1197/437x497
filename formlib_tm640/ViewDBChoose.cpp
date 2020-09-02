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
BOOL				RunOnlyOne				=	FALSE;	//�Q��update�Ȱ���@��

CtmWnd*		pwndStaticText			= NULL; // Static ��ܤ�r
CtmWnd*		pwndBtnFlwTP				= NULL; // Btn �H�ܱо����D
CtmWnd*		pwndBtnFlwCON				= NULL; // Btn �H������D
CtmWnd*		pwndValue_TP				= NULL; // Edit �ܱо��ƭ�
CtmWnd*		pwndValue_CON				= NULL; // Edit ����ƭ�
CtmWnd*		pwndBtnAllFlwTP			= NULL; // Btn �����H�ܱо����D
CtmWnd*		pwndBtnAllFlwCON		= NULL; // Btn �����H������D

CtmWnd*		pwndValue_497				= NULL; // �ܱо��ƭ� ��ܤ�r
CtmWnd*		pwndValue_28				= NULL; // �ܱо��ƭ� ��ܤ�r
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
	
	// �Y�H��� �����M�� 1:�ܱо� �� 2:��� �Ѽ�
	int iDBSelect = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72").lValue;
	if(iDBSelect > 0) // �����M��1:�ܱо� �� 2:���
	{
  	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", iDBSelect); // 1:�H�ܱо����D 2:�H������D
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
	((CtmFormView*)pwndSender)->OnLoseFocus(); // ��������
	
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

 	if(pwnd == pwndBtnFlwTP) // �H�ܱо����D
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 1); // 1:�H�ܱо����D 2:�H������D
    	Exit();
    }
	}
 	if(pwnd == pwndBtnFlwCON) // �H������D
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 2); // 1:�H�ܱо����D 2:�H������D
    	Exit();
    }
	}
 	if(pwnd == pwndBtnAllFlwTP) // �����H�ܱо����D
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 1); // 1:�H�ܱо����D 2:�H������D
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72", 1); // 1:�����H�ܱо����D 2:�����H������D
    	Exit();
    }
	}
 	if(pwnd == pwndBtnAllFlwCON) // �����H������D
	{
		MsgBox(g_MultiLanguage["SELECT_CONFIRM"], tmFT_CODE_TECH);
    if(g_bMsgBoxFlag)
    {
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED71", 2); // 1:�H�ܱо����D 2:�H������D
    	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED72", 2); // 1:�����H�ܱо����D 2:�����H������D
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
		((CtmFormView*)pwndSender)->OnLoseFocus(); // ��������
		
		pwndValue_497->SetPropValueT("text", gstr_DBValue_497);
		pwndValue_497->Update();
		pwndValue_28->SetPropValueT("text", gstr_DBValue_28);
		pwndValue_28->Update();
	}
}
