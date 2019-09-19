/*===========================================================================+
|  Class    : ViewDiag_ServP library                                         |
|  Task     : ViewDiag_ServP library action source file                      |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |  
+===========================================================================*/
#include 	"ViewDiag_ServP1.h"
#include 	"../commonaction.h"
#include	"../font.h"
#include	"../tmshmsg.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define	    EDIT_STARTNO			"EditStartNo"	//	ServP Start NO.    
#define	    EDIT_TOTALCOUNT 		"editTOTALCOUNT"		//	ServP Total Count  

#define	    SERVEP_TABLE			"StaticTABLE"           //  The Controls in Table
#define     SERV_PARAMETER_COUNT    MAX_SERVO_NO
#define     TUNE_PARAMETER_COUNT    MAX_TUNE_NO
#define     MAKE_PARAMETER_COUNT    MAX_MAKER_NO 
#define     USER_PARAMETER_COUNT    MAX_USER_NO
#define     COMN_PARAMETER_COUNT    MAX_COMMON_NO

#define     SERVP_ROWS              26
#define	    SERVP_COLS              3
#define     MAX_STATICTABLE_SIZE    50

#define     TYPE_SERV_PARA			1
#define     TYPE_TUNE_PARA			2
#define     TYPE_MAKER_PARA			3
#define     TYPE_USER_PARA			4
#define     TYPE_COMMON_PARA		5
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
int			u_nStatic_TableCnt =0;
int			u_nEdit_TableCnt =0;
int			u_wStartNo =0;
int	 		u_nServeP_String_Begin =0;
WORD		u_wStartDBIndex =0;
WORD		u_wParamType =0;
WORD		u_wPageChangeFlag =0;
int 		MaxStartNo		=0;
int			MaxParamCnt		=0;
BOOL		EditLock = FALSE; 
BOOL		PassLock = FALSE; 
CtmWnd*		pwndServPStaticTable[256] = {NULL};
CtmWnd*		pwndServPEditTable[256] = {NULL};
CtmWnd*		pwndSTARTNO = NULL;
CtmWnd*		pwndTOTALCOUNT = NULL;
CtmWnd*		pwndEditPassWord	= NULL;
CtmWnd*		w_pwndSender = NULL;

CtmWnd*		u_pwndImageBoxLock = NULL;
CtmWnd*		u_pwndStaticTitle	 = NULL;

/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateServP(CtmWnd* pwndSender)
{
	u_wParamType = TYPE_SERV_PARA;
	u_nServeP_String_Begin = g_MultiLanguage.GetStrKeyIndex("SERVO_PARAM_STR_0");//_PROMPT_SERVO_PARA0");
	u_wStartDBIndex = g_pDatabase->GetIndex("SERVO_PARAM0");
	MaxStartNo	= SERV_PARAMETER_COUNT-SERVP_ROWS;
	MaxParamCnt = SERV_PARAMETER_COUNT;
	if(MaxStartNo<0) MaxStartNo=0;
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreateTuneP(CtmWnd* pwndSender)
{
	u_wParamType = TYPE_TUNE_PARA;
	u_nServeP_String_Begin = g_MultiLanguage.GetStrKeyIndex("TUNE_PARAM_STR_0");//_PROMPT_SERVO_PARA0");
	u_wStartDBIndex = g_pDatabase->GetIndex("TUNE_PARAM0");
	MaxStartNo	= TUNE_PARAMETER_COUNT-SERVP_ROWS;
	MaxParamCnt = TUNE_PARAMETER_COUNT;
	if(MaxStartNo<0) MaxStartNo=0;
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreateMakerP(CtmWnd* pwndSender)
{
	u_wParamType = TYPE_MAKER_PARA;
	u_nServeP_String_Begin = g_MultiLanguage.GetStrKeyIndex("MAKER_PARAM_STR_0");//_PROMPT_SERVO_PARA0");
	u_wStartDBIndex = g_pDatabase->GetIndex("MAKER_PARAM0");
	MaxStartNo	= MAKE_PARAMETER_COUNT-SERVP_ROWS;
	MaxParamCnt = MAKE_PARAMETER_COUNT;
	if(MaxStartNo<0) MaxStartNo=0;
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreateUserP(CtmWnd* pwndSender)
{
	u_wParamType = TYPE_USER_PARA;
	u_nServeP_String_Begin = g_MultiLanguage.GetStrKeyIndex("USER_PARAM_STR_0");//_PROMPT_SERVO_PARA0");
	u_wStartDBIndex = g_pDatabase->GetIndex("USER_PARAM0");
	MaxStartNo	= USER_PARAMETER_COUNT-SERVP_ROWS;
	MaxParamCnt = USER_PARAMETER_COUNT;
	if(MaxStartNo<0) MaxStartNo=0;
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreateCommonP(CtmWnd* pwndSender)
{
	u_wParamType = TYPE_COMMON_PARA;
	u_nServeP_String_Begin = g_MultiLanguage.GetStrKeyIndex("COMMON_PARAM_STR_0");//_PROMPT_SERVO_PARA0");
	u_wStartDBIndex = g_pDatabase->GetIndex("COMMON_PARAM0");
	MaxStartNo	= COMN_PARAMETER_COUNT-SERVP_ROWS;
	MaxParamCnt = COMN_PARAMETER_COUNT;
	if(MaxStartNo<0) MaxStartNo=0;
	CreatePublic(pwndSender);	
	return TRUE;
}

BOOL	CreatePublic(CtmWnd* pwndSender)
{
	w_pwndSender 				= pwndSender;
	pwndSTARTNO 				= pwndSender->FindControlFromName(EDIT_STARTNO);
	pwndTOTALCOUNT 			= pwndSender->FindControlFromName(EDIT_TOTALCOUNT);	
	//pwndEditPassWord 		= pwndSender->FindControlFromName("EditPassWord");	
	u_pwndImageBoxLock	= pwndSender->FindControlFromName("ImageBoxLock");	
	u_pwndStaticTitle		= pwndSender->FindControlFromName("StaticWORK_DATA");	


	u_nStatic_TableCnt	= GetSpecialControlNum(pwndSender, SERVEP_TABLE, "CtmStaticX2", pwndServPStaticTable);
	u_nEdit_TableCnt	= GetSpecialControlNum(pwndSender, SERVEP_TABLE, "CtmEditX1", pwndServPEditTable);
		
	if (pwndSTARTNO != NULL)
	{
		pwndSTARTNO->SetPropValueT("max",MaxStartNo);
		UpdateServPTable(u_wStartNo);
	}
	RobotStatus = OP_EDIT_MODE;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	for(int i = 0 ; i < u_nStatic_TableCnt ; i++ )
	{
		if(pwndServPEditTable[i] == pwndTemp)
		{
			if(!g_Pass)
			{
				MsgBoxSpecial(5);
			}
			if(!g_Pass)
				return 65535;  //WORD ¤£¤ä´©­t¼Æ
			else
			{
				u_pwndImageBoxLock->SetPropValueT("imagepath","res_tm640/LockOFF.png");
				u_pwndStaticTitle->Show();
				u_pwndImageBoxLock->Show();
			}
			return	wIDControl;
		}
	}
	return	wIDControl;
}

void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
	{
		if(wParam == _F6)  //Up
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			//((CtmFormView*)w_pwndSender)->Goto(1);
			u_wStartNo -= SERVP_ROWS;
			if(u_wStartNo < 0) u_wStartNo = 0;
			UpdateServPTable(u_wStartNo);
			pwndSTARTNO->SetPropValueT("value",u_wStartNo);
      pwndSTARTNO->Update();
			((CtmFormView*)pwndSender)->OnGetFocus();
			Now_Push_Btn->CreateA();			
			Now_Push_Btn->UpdateAll();		
			Now_Push_Btn->Update();
		}
		else if(wParam == _F7)//Down
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			u_wStartNo += SERVP_ROWS;
			if(u_wStartNo > MaxStartNo) u_wStartNo = MaxStartNo;
			UpdateServPTable(u_wStartNo);
			pwndSTARTNO->SetPropValueT("value",u_wStartNo);
      pwndSTARTNO->Update();
			((CtmFormView*)pwndSender)->OnGetFocus();
			Now_Push_Btn->CreateA();			
			Now_Push_Btn->UpdateAll();		
			Now_Push_Btn->Update();
		}
	}
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp = NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == pwndSTARTNO)	//	Amend StartNO of the table
	{
		if(u_wPageChangeFlag)
		{
			pwndSTARTNO -> SetPropValueT("value",u_wStartNo);
      pwndSTARTNO -> Update();
      u_wPageChangeFlag =0;
		}
		else
		{
			pwndSTARTNO -> GetPropValueT("value", &u_wStartNo,sizeof(u_wStartNo));
			UpdateServPTable(u_wStartNo);
		}
	}
	return wIDControl;
}
/*
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{	
	CtmWnd* pwndTemp = NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndEditPassWord == pwndTemp)
	{
		pwndEditPassWord -> GetPropValueT("text", &g_Pass_Word,sizeof(g_Pass_Word));
		if(strcmp(g_Pass_Word,"1111") == 0)
			PassLock = TRUE;
		else
			PassLock = FALSE;
	}
}*/
WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
		if((wKey == 0x6D00)||(wKey == 0x6800))//Page Change
		{
			if(wKey == 0x6D00)	//PageDown
			{
				u_wStartNo += SERVP_ROWS;
				if((u_wParamType == TYPE_SERV_PARA)&&(u_wStartNo >= (SERV_PARAMETER_COUNT-SERVP_ROWS)))	u_wStartNo = SERV_PARAMETER_COUNT-SERVP_ROWS;
				else if((u_wParamType == TYPE_TUNE_PARA)&&(u_wStartNo >= (TUNE_PARAMETER_COUNT-SERVP_ROWS)))	u_wStartNo = TUNE_PARAMETER_COUNT-SERVP_ROWS;
				else if((u_wParamType == TYPE_MAKER_PARA)||(u_wParamType == TYPE_USER_PARA)||(u_wParamType == TYPE_COMMON_PARA))
				{
					//printf("u_wParamType:%d  u_wStartNo:%d \n",u_wParamType,u_wStartNo);
					if(u_wStartNo >= (MAX_MAKER_NO-SERVP_ROWS))	u_wStartNo = MAX_MAKER_NO-SERVP_ROWS;
				}
			}
			else if(wKey == 0x6800)	//PageUp
			{
				u_wStartNo -= SERVP_ROWS;
				if((u_wStartNo >= (TUNE_PARAMETER_COUNT-SERVP_ROWS)) ||(u_wStartNo <0))	u_wStartNo =0;
			}
			u_wPageChangeFlag =1;
			UpdateServPTable(u_wStartNo);
		}
		else if(wKey == _KEY_DOWNFIELD || wKey == _ENTER)	//20120518 add <<<<<
		{
			if(((CtmFormView *)pwndSender)->Get_TabStop() == SERVP_ROWS )
			{
				++u_wStartNo;
				if((u_wParamType == TYPE_SERV_PARA)&&(u_wStartNo >= (SERV_PARAMETER_COUNT-SERVP_ROWS)))	u_wStartNo = SERV_PARAMETER_COUNT-SERVP_ROWS;
				else if((u_wParamType == TYPE_TUNE_PARA)&&(u_wStartNo >= (TUNE_PARAMETER_COUNT-SERVP_ROWS)))	u_wStartNo = TUNE_PARAMETER_COUNT-SERVP_ROWS;
				else if((u_wParamType == TYPE_MAKER_PARA)||(u_wParamType == TYPE_USER_PARA)||(u_wParamType == TYPE_COMMON_PARA))
				{
					if(u_wStartNo >= (MAX_MAKER_NO-SERVP_ROWS))	u_wStartNo = MAX_MAKER_NO-SERVP_ROWS;
				}
				//UpdateServPTable(u_wStartNo);
				
				if(pwndServPEditTable[u_nEdit_TableCnt-1] != NULL)
				{
					pwndServPEditTable[u_nEdit_TableCnt-1]->OnLoseFocus();
					UpdateServPTable(u_wStartNo);
					pwndServPEditTable[u_nEdit_TableCnt-1]->OnGetFocus();
					pwndServPEditTable[u_nEdit_TableCnt-1]->UpdateAll();
				}

				if(pwndSTARTNO != NULL)
				{
					pwndSTARTNO -> SetPropValueT("value",u_wStartNo);
        			pwndSTARTNO -> Update();
        		}
			}
			else ((CtmFormView*)pwndSender)->OnKey1(wKey);
		}
		else if(wKey == _KEY_UPFIELD)
		{
			if(((CtmFormView *)pwndSender)->Get_TabStop() == 1 )	//Table Fisrt Edit
			{
				--u_wStartNo;
				if((u_wStartNo >= (TUNE_PARAMETER_COUNT-SERVP_ROWS)) ||(u_wStartNo <0))	u_wStartNo =0;
				//UpdateServPTable(u_wStartNo);
				
				if(pwndServPEditTable[0] != NULL)
				{
					pwndServPEditTable[0]->OnLoseFocus();
					UpdateServPTable(u_wStartNo);
					pwndServPEditTable[0]->OnGetFocus();
					pwndServPEditTable[0]->UpdateAll();
				}

				if(pwndSTARTNO != NULL)
				{
					pwndSTARTNO -> SetPropValueT("value",u_wStartNo);
        			pwndSTARTNO -> Update();
        		}
			}
			else ((CtmFormView*)pwndSender)->OnKey1(wKey);
		}
		else ((CtmFormView*)pwndSender)->OnKey1(wKey);
}

void		OnUpdateA(CtmWnd*	pwndSender)
{
	static BOOL FristOne = TRUE;
	static BOOL old_EditLock = FALSE;
	static BOOL	AutoFalg = FALSE;
	if(FristOne)
	{
		FristOne = FALSE;
		if(g_Pass)
		{
			u_pwndImageBoxLock->SetPropValueT("imagepath","res_tm640/LockOFF.png");
			u_pwndStaticTitle->Show();
			u_pwndImageBoxLock->Show();
		}
	}	
			
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE && !AutoFalg) 
		AutoFalg = TRUE;
	else if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE && AutoFalg)
		AutoFalg = FALSE;
	

		
	if(old_EditLock != AutoFalg)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		old_EditLock = AutoFalg;
		for (int i=0; i<u_nEdit_TableCnt; i++)
    {
      if(pwndServPEditTable[i] != NULL)
      {
				if(old_EditLock)
        	pwndServPEditTable[i]->SetPropValueT("enabled",(double)FALSE);
				else
        	pwndServPEditTable[i]->SetPropValueT("enabled",(double)TRUE);
        pwndServPEditTable[i]->Update();
      } 
    }
  }
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void    UpdateServPTable(WORD	wStartNo)
{
	static	WORD	wStartNo_Old =0;
	if((wStartNo_Old == wStartNo)&& wStartNo_Old != 0) return;
    
    WORD    *pwDst, *pwSrc;
	char temp[128];
	char szTemp[256];
	
	for (int i=0; i<u_nStatic_TableCnt; i+=2)
    {
        if(pwndServPStaticTable[i] != NULL)
        {
        	long lServPNo = i/2+wStartNo;
        	sprintf(temp ,"%d",lServPNo);
        	CodeChange(szTemp, temp);
        	pwndServPStaticTable[i] -> SetPropValueT("text",szTemp);
        	pwndServPStaticTable[i] -> Update();
        }
    }
    for (int i=1; i<u_nStatic_TableCnt; i+=2)
    {
        if(pwndServPStaticTable[i] != NULL)
        {
        	WORD *pwSrc;
    	    char  szTemp[128];
    	    int   j;
    	    WORD *pwDst;
    	    pwDst   = (WORD *)(szTemp);
        	pwSrc = (WORD *)g_MultiLanguage[u_nServeP_String_Begin + wStartNo +i/2];
        	
        	if(/*((u_wParamType == TYPE_MAKER_PARA)&&(i>45))||*/
        	   ((u_wParamType == TYPE_USER_PARA)&&(i>((USER_PARAMETER_COUNT - u_wStartNo)*2-1)))||
        	   ((u_wParamType == TYPE_MAKER_PARA)&&(i>((MAKE_PARAMETER_COUNT - u_wStartNo)*2-1)))||
        	   ((u_wParamType == TYPE_COMMON_PARA)&&(i>((COMN_PARAMETER_COUNT- u_wStartNo)*2-1))))	//20111231 add (-u_wStartNo)	
        	{
        		//printf("u_wParamType:%d ",u_wParamType);
        		pwSrc = (WORD *)g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("_VW_PARA_RESERVE")];	
        	}
        	if(pwSrc != NULL)
        	{        	        
                j = 0;
                while(*pwSrc != 0)
                {
                    *pwDst ++   = *pwSrc ++;
                }
            }
            *pwDst  = 0;
        	pwndServPStaticTable[i] -> SetPropValueT("text",szTemp);
        	pwndServPStaticTable[i]->Update();
        }
        
    }
    for (int i=0; i<u_nEdit_TableCnt && i < MaxParamCnt; i++)
    {
        if(pwndServPEditTable[i] != NULL)
        {
        	char*	pDataID = NULL;
        	pDataID = g_pDatabase->GetString(u_wStartDBIndex + wStartNo +i);
        	//printf("pDataID:%s %d %d %d\n",pDataID,u_wStartDBIndex, wStartNo,i);
        	if(pDataID != NULL && pDataID[0] != '\0')
        	{
        		static WORD wMaxchar =12;
        		pwndServPEditTable[i]->SetPropValueT("dbid0",pDataID);
        		pwndServPEditTable[i]->SetPropValueT("maxchar",wMaxchar);
        		//pwndServPEditTable[i]->SetPropValueT("Value",GetDBValue(pDataID).lValue);
        		//printf("Value %d\n",GetDBValue(pDataID).lValue);
        		pwndServPEditTable[i]->Update();
        	}
        }
        
    }
}


void    CodeChange(char *pDst, char *pSrc)
{
    WORD    *pwDst  = (WORD *)pDst;
    
    while(*pSrc != 0)      *pwDst ++ = *pSrc ++;
    
    *pwDst  = 0;
    
}

