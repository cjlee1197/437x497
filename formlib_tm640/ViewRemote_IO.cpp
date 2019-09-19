/*===========================================================================+
|  Class    : ViewRemote_IO library                                          |
|  Task     : ViewRemote_IO library action source file                       |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 									                                             |
|  Version  : V1.00                  					                               |
|  Creation : 			                   		                                   |
|  Revision :           			         			                                 |
+===========================================================================*/
#include    "ViewRemote_IO.h"
#include    "../commonaction.h"
#include		"../utils.h"
#include    "../static.h"
#include		"../font.h"
#include		"../tmshmsg.h"

/*===========================================================================+
|           Named rule Help                                                  |
+===========================================================================*/
// PB             --  SelectBoxPB__: example -- SelectBoxPB10, SelectBoxPB11, ... ...
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     WORDBITS            16
#define			ID_MAX_SELECT				50
#define     STR_MAX_COUNT       256
#define			tmTIME_DEMO					1200
#define			IO_CHANGE							5
#define     PB_MAXNUM						25
#define     PC_MAXNUM						25
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

CtmWnd*     u_pwndSelectBox[ID_MAX_SELECT];
char        u_acSelBoxString[ID_MAX_SELECT][STR_MAX_COUNT];
int 				u_nSelectBoxCount 	= 0;


DBVIEWVALUE u_dbInputValue;
WORD        u_wInputValue,u_wInputValue_OLD;

DBVIEWVALUE u_dbOutputValue;
WORD        u_wOutputValue,u_wOutputValue_OLD;

DBVIEWVALUE u_dbEX_InputValue;
DWORD        u_wEX_InputValue,u_wEX_InputValue_OLD;

DBVIEWVALUE u_dbEX_OutputValue;
DWORD        u_wEX_OutputValue,u_wEX_OutputValue_OLD;

CtmWnd*		pwndButton_IO1       = NULL;
CtmWnd*		pwndButton_Alarm     = NULL; 
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndButton_IO1     = pwndSender->FindControlFromName("Button_IO1");  
	pwndButton_Alarm   = pwndSender->FindControlFromName("Button_Alarm"); 
	//u_nSelectBoxCount = GetSpecialControlNum(pwndSender, (char*)NULL, "CtmSelectBox", &u_pwndSelectBox[0]);
	u_nSelectBoxCount = GetSpecialControlNum(pwndSender, "SelectBox", "CtmSelectBox", &u_pwndSelectBox[0]);
	printf("u_nSelectBoxCount = %d\n", u_nSelectBoxCount);

	CreateLEDSet();
	CreateStringSet();
	
//	//check IO state
//	{
//		u_dbEX_InputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE");	
//		u_wEX_InputValue = static_cast<WORD>(u_dbEX_InputValue.lValue);
//		u_dbEX_OutputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE");	
//		u_wEX_OutputValue = static_cast<WORD>(u_dbEX_OutputValue.lValue);	
//
//	  
//		u_dbInputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALINPUTSTATE");	
//		u_wInputValue = static_cast<WORD>(u_dbInputValue.lValue);
//		u_dbOutputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE");	
//		u_wOutputValue = static_cast<WORD>(u_dbOutputValue.lValue);	
//
//	  
//	  for(int i = 0; i < 16 ; i++)
//	  {
//	  	printf("IN-IN%d = %d\n",i+1,_TestBit(u_wInputValue, i));
//	  }
//	  for(int i = 0; i < 16 ; i++)
//	  {
//	  	printf("EX-IN%d = %d\n",i+1,_TestBit(u_wEX_InputValue, i));
//	  }
//	  
//	  
//	  for(int i = 0; i < 16 ; i++)
//	  {
//	  	printf("IN-OUT%d = %d\n",i+1,_TestBit(u_wOutputValue, i));
//	  }
//	  for(int i = 0; i < 16 ; i++)
//	  {
//	  	printf("EX-OUT%d = %d\n",i+1,_TestBit(u_wEX_OutputValue, i));
//	  }
//	}
	
	return TRUE;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
		u_dbEX_InputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE");	
		u_wEX_InputValue = static_cast<DWORD>(u_dbEX_InputValue.lValue);
		u_dbEX_OutputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE");	
		u_wEX_OutputValue = static_cast<DWORD>(u_dbEX_OutputValue.lValue);	
		
		CreateLEDSet();
//		 if((u_wEX_InputValue!=u_wEX_InputValue_OLD) || (u_wEX_OutputValue!=u_wEX_OutputValue_OLD) || (u_wInputValue!=u_wInputValue_OLD) || (u_wOutputValue!=u_wOutputValue_OLD))
//		 	{
//		 		CreateLEDSet();
//		 		u_wEX_InputValue_OLD=u_wEX_InputValue;
//		 		u_wEX_OutputValue_OLD=u_wEX_OutputValue;
//		 		u_wInputValue_OLD=u_wInputValue;
//		 		u_wOutputValue_OLD=u_wOutputValue;
//		 	}

}

void  	OnDestroyA(CtmWnd* pwndSender)
{

}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void       CreateLEDSet()
{
	//printf("CreateLEDSet()\n");
	BOOL 	bSelected = FALSE;
		
	/*-----------------------------------External IO--------------------------------------*/
	u_dbEX_InputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE");	
	u_wEX_InputValue = static_cast<DWORD>(u_dbEX_InputValue.lValue);
	//printf("u_wEX_InputValue=%x\n",u_wEX_InputValue);
	u_dbEX_OutputValue = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE");	
	u_wEX_OutputValue = static_cast<DWORD>(u_dbEX_OutputValue.lValue);	
	//printf("u_wEX_OutputValue=%x\n",u_wEX_OutputValue);
	/*-----------------------------------External IO--------------------------------------*/
	if((u_wEX_OutputValue!=u_wEX_OutputValue_OLD)||(u_wEX_InputValue!=u_wEX_InputValue_OLD))
	{
		for(int i = 0; i < u_nSelectBoxCount; i++)
		{
			if(u_pwndSelectBox[i] != NULL)
			{
				if(i < PB_MAXNUM)     // PB
				{
					printf("%d=%d\n",i,(u_wEX_InputValue>>i)&1);
		   		if((u_wEX_InputValue>>i)&1)	bSelected = TRUE;
			 		else 							bSelected = FALSE;
			 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
			 		u_pwndSelectBox[i]->Update();
				}
				else                 //PC
				{
					printf("%d=%d\n",i,(u_wEX_OutputValue>>(i- PB_MAXNUM))&1);
					if((u_wEX_OutputValue>>(i- PB_MAXNUM))&1)	bSelected = TRUE;
			 		else 							bSelected = FALSE;
			 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
			 		u_pwndSelectBox[i]->Update();
				}
			}		
		}
		u_wEX_InputValue_OLD  = u_wEX_InputValue;
		u_wEX_OutputValue_OLD = u_wEX_OutputValue;
		printf("u_wEX_InputValue=%x\n",u_wEX_InputValue);
		printf("u_wEX_OutputValue=%x\n",u_wEX_OutputValue);
	}
}


void	CreateStringSet()
{
	//printf("CreateStringSet()\n");
	char	pszPBString[32];
	int 	j;
		
	for( int i = 0; i < u_nSelectBoxCount; i++)
	{
		memset(u_acSelBoxString[i], 0, sizeof(u_acSelBoxString[i]));
		if(g_pFont->GetDefaultFont()->GetFontCode() == tmFT_CODE_TECH)
		{
		  WORD *pwSrc;
      char  szTemp[32];
      WORD *pwDst;
      
      pwDst   = (WORD *)(u_acSelBoxString[i]);
      if(i < PB_MAXNUM)   //PB
     	{
     	 sprintf(szTemp, "DI-%02d", i+1);
     	 //printf("%s\n",szTemp);
   	  }
   	  else                //PC
   	  {
   	  	sprintf(szTemp, "DO-%02d", i+1-PB_MAXNUM);
   	  	//printf("%s\n",szTemp);
   	  }
      
      int		j = 0;
      while(szTemp[j] != 0)
	    {
	     *pwDst ++   = szTemp[j ++];
	    }
	    *pwDst++  = ' ';
	       
	    if(i < PB_MAXNUM)   //PB
	    {
	   		sprintf(pszPBString, "PICKER_REMOTE_I_%02d", i+1);
	   		//printf("%s\n",pszPBString);
	    }
	    else                //PC
	    {
	    	sprintf(pszPBString, "PICKER_REMOTE_O_%02d", i+1-PB_MAXNUM);
	    	//printf("%s\n",pszPBString);
	    }
	    
	    pwSrc = (WORD *)g_MultiLanguage[pszPBString];
      if(pwSrc != NULL)	
      {
    	  j = 0;
    	  while(*pwSrc != 0)
    	     *pwDst ++   = *pwSrc ++;
    	}
		  *pwDst  = 0;
		}
	}
	
		for(int i = 0; i < u_nSelectBoxCount; i++)	
		{
	    if(u_pwndSelectBox[i] != NULL)
	    {
	    	u_pwndSelectBox[i]->SetPropValueT("text", u_acSelBoxString[i]);
	    }
	  }
	
}
WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
//		case 0:
//			::PutCommand("Internal_IO.txt");
//			break;
//		case 1:
//			break;
//		case 2:
//			::PutCommand("Alarm.txt");
//			break;
		case 70: // Home
			::PutCommand("Over.txt");
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
		default:
			break;
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}
/*---------------------------------------------------------------------------+
|  Function : OnMessage()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	if(message==MSG_SH_APP_TOOLBAR_ONKEY)
		{
			printf("Toolbar =%d\n",wParam);
			switch(wParam)
			{
				case _F1: // 主頁
						::PutCommand("Over.txt");
					break;	
				case _F2: // 主選單
					::PutCommand("Index.txt");
					break;
				case _F3: // IO
					::PutCommand("Over.txt");
					break;	
				case _F4: // 鍵盤
					;
					break;
				case _F5: // 返回
					::PutCommand("Over.txt");
					break;	
				default:
					break;
			}
		}
}
/*---------------------------------------------------------------------------+
|  Function : OnMouseDown()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL) 		return wIDControl;		
  return wIDControl;	
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

	if(pwnd ==pwndButton_IO1)
	{
		::PutCommand("Internal_IO.txt");
	}
	if(pwnd ==pwndButton_Alarm)
	{
		::PutCommand("Alarm.txt");
	}
  return wIDControl;	
}