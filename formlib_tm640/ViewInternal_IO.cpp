/*===========================================================================+
|  Class    : ViewInternal_IO library                                        |
|  Task     : ViewInternal_IO library action source file                     |
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
#include    "ViewInternal_IO.h"
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
#define			ID_MAX_SELECT				48
#define     STR_MAX_COUNT       256
#define			tmTIME_DEMO					1200
#define			IO_CHANGE						5
#define     PB_MAXNUM						8
#define     PC_MAXNUM						8
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

CtmWnd*     u_pwndSelectBox[ID_MAX_SELECT];

char        u_acSelBoxString[ID_MAX_SELECT][STR_MAX_COUNT];
int 				u_nSelectBoxCount 	= 0;

DBVIEWVALUE u_dbInputValue;
DWORD        u_wInputValue;

DBVIEWVALUE u_dbOutputValue;
DWORD        u_wOutputValue;

CtmWnd*			pwndButton_IO2       = NULL;    
CtmWnd*			pwndButton_Alarm     = NULL;    
CtmWnd*			pwndButton_Moni 	 = NULL;
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	//printf("ViewInternal_IO\n");
	u_nSelectBoxCount = GetSpecialControlNum(pwndSender, (char*)NULL, "CtmSelectBox", &u_pwndSelectBox[0]);
	printf("u_nSelectBoxCount = %d\n", u_nSelectBoxCount);
	pwndButton_IO2     = pwndSender->FindControlFromName("Button_IO2");  
	pwndButton_Alarm   = pwndSender->FindControlFromName("Button_Alarm");  
	pwndButton_Moni    = pwndSender->FindControlFromName("Button_Moni");
	
	CreateLEDSet();
	CreateStringSet();
	return TRUE;
}

void	OnUpdateA(CtmWnd* pwndSender)
{

		  CreateLEDSet();
}

void  	OnDestroyA(CtmWnd* pwndSender)
{
}
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void       CreateLEDSet()
{
	BOOL 	bSelected = FALSE;
		
	u_dbInputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALINPUTSTATE");	
	u_wInputValue = static_cast<DWORD>(u_dbInputValue.lValue);

	u_dbOutputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE");	
	u_wOutputValue = static_cast<DWORD>(u_dbOutputValue.lValue);	
	
	//printf("u_wInputValue = %X\n",u_wInputValue);
	//printf("u_wOutputValue = %X\n",u_wOutputValue);
 
	for(int i = 0; i < u_nSelectBoxCount; i++)
	{
		if(u_pwndSelectBox[i] != NULL)
		{
			if(i < PB_MAXNUM)     // PB
			{
	   		if( (u_wInputValue>>(i))&1 ) bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
		 		u_pwndSelectBox[i]->Update();
			}
			else                 //PC
			{
				if( u_wOutputValue>>(i-PB_MAXNUM)&1 ) bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
		 		u_pwndSelectBox[i]->Update();
			}
		}		
	}
}

void	CreateStringSet()
{
	char	pszString[128];
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
     	 sprintf(szTemp, "DI-%02d", i+25);
   	  }
   	  else                //PC
   	  {
   	  	sprintf(szTemp, "DO-%02d", i+25-PB_MAXNUM);
   	  }
      
      int		j = 0;
      while(szTemp[j] != 0)
	    {
	     *pwDst ++   = szTemp[j ++];
	    }
	    *pwDst++  = ' ';	    
	    if(i < PB_MAXNUM)   //PB
	    {
	   		sprintf(pszString, "PCIKER_INNER_I_%02d", i+1);
	    }
	    else                //PC
	    {
	    	sprintf(pszString, "PICKER_INNER_O_%02d", i+1-PB_MAXNUM);
	    }
	    
	    pwSrc = (WORD *)g_MultiLanguage[pszString];
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
	printf("OnKeyA\n");
	switch(wKey)
	{
//		case 0:
//			break;
//		case 1:
//			::PutCommand("Remote_IO.txt");
//			break;
//		case 2:
//			::PutCommand("Alarm.txt");
//			break;
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
	;
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
	//printf("OnMouseUp\n");
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_IO2)
	{
		printf("pwndButton_IO2\n");
		::PutCommand("Remote_IO.txt");
	}
	
  return wIDControl;	
}
