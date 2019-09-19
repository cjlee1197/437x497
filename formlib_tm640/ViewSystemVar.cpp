/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewSystemVar.h"
//#include	"../tmdebug.h"
#include	"../taskcnc.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     PARAMETER_COUNT_LOCAL		33
#define     PARAMETER_COUNT_GLOBAL  100
#define     PARAMETER_COUNT_SYSTEM  30

#define     MACRO_TYPE_LOCAL		0
#define     MACRO_TYPE_GLOBAL		1
#define     MACRO_TYPE_SYSTEM		2

#define     MAX_TABLE_NUM		60
#define     MAX_SHOW_DATA_TYPE	2
/*============================================================================+
|           Constant                                                     			|
+============================================================================*/
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
char*	ShowShareMemoryVarType[] =			
{
	"ROBOT_STR_PULSE_DATA",			
	"ROBOT_STR_SERVO_LAG",
	NULL
}; 

char*	PulseDataStatic[] =			
{
	"Isr pulse X :",			
	"Isr pulse Y :",
	"Isr pulse Z :",
	"Isr pulse A :",
	"Isr pulse B :",
	"Isr pulse C :",
	"Cmd pulse X :",			
	"Cmd pulse Y :",
	"Cmd pulse Z :",
	"Cmd pulse A :",
	"Cmd pulse B :",
	"Cmd pulse C :",
	"Total pulse X :",			
	"Total pulse Y :",
	"Total pulse Z :",
	"Total pulse A :",
	"Total pulse B :",
	"Total pulse C :",
	NULL
}; 

char*	ServoLagDataStatic[] =			
{
	"Isr ServoLag X :",			
	"Isr ServoLag Y :",
	"Isr ServoLag Z :",
	"Isr ServoLag A :",
	"Isr ServoLag B :",
	"Isr ServoLag C :",
	"Max ServoLag X :",			
	"Max ServoLag Y :",
	"Max ServoLag Z :",
	"Max ServoLag A :",
	"Max ServoLag B :",
	"Max ServoLag C :",
	" ",			
	" ",
	" ",
	" ",
	" ",
	" ",
	NULL
}; 


CtmWnd*		pwndStaticTable[100]= {NULL};
int				u_nStatic_TableCnt	=0;
CtmWnd*		pwndEditTable[100] 	= {NULL};
int				u_nEdit_TableCnt		=0;

CtmWnd*		pwndEditSTARTNO			= NULL;
CtmWnd*		pwndBtnUp						= NULL;
CtmWnd*		pwndBtnDown					= NULL;

CtmWnd*		pwndBtnLocal				= NULL;
CtmWnd*		pwndBtnGlobal				= NULL;
CtmWnd*		pwndBtnSystemVar		= NULL;
CtmWnd*		pwndBtnIsrtime		= NULL;



CtmWnd*		pwndEditDataValue[18] 	= {NULL};
int				u_nEditDataValueCnt		=0;
CtmWnd*		pwndStaticDataName[18] 	= {NULL};
int				u_nStaticDataNameCnt		=0;
CtmWnd*		pwndStaticDataType			= NULL;
CtmWnd*		pwndBtnDataUp						= NULL;
CtmWnd*		pwndBtnDataDown					= NULL;


int				LeftVarMode 		= MACRO_TYPE_LOCAL;
int				StartNO					=1;
int 			u_wStartDBIndex	=g_pDatabase->GetIndex("CNC_MACRO_LOCAL_VAR0");
int				MaxNO 					= PARAMETER_COUNT_LOCAL;
int				ShareMemoryDataMode	=0;
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	pwndEditSTARTNO 		= pwndSender->FindControlFromName("EditStartNo");
	pwndBtnUp 					= pwndSender->FindControlFromName("BtnUp");
	pwndBtnDown 				= pwndSender->FindControlFromName("BtnDown");
	pwndBtnLocal 				= pwndSender->FindControlFromName("BtnLocal");
	pwndBtnGlobal 			= pwndSender->FindControlFromName("BtnGlobal");
	pwndBtnSystemVar 		= pwndSender->FindControlFromName("BtnSystemVar");
	
	
	pwndStaticDataType 			= pwndSender->FindControlFromName("StaticDataType");
	u_nStaticDataNameCnt	= GetSpecialControlNum(pwndSender, "StaticDataName", "CtmStaticX2", pwndStaticDataName);
	u_nEditDataValueCnt		= GetSpecialControlNum(pwndSender, "EditDataValue", "CtmEditX1", pwndEditDataValue);
	pwndBtnDataUp 				= pwndSender->FindControlFromName("BtnShowDataPageUp");
	pwndBtnDataDown 			= pwndSender->FindControlFromName("BtnShowDataPageDown");
	
	u_nStatic_TableCnt	= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmStaticX2", pwndStaticTable);
	u_nEdit_TableCnt		= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", pwndEditTable);
	MaxNO = PARAMETER_COUNT_LOCAL-MAX_TABLE_NUM+1;
	if(MaxNO<1) MaxNO=1;
	SetButtonBgc(pwndBtnLocal);

	RobotStatus = OP_EDIT_MODE;	

	return	TRUE;
}	

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	
	if(pwndTemp == pwndBtnLocal && LeftVarMode != MACRO_TYPE_LOCAL)
	{
		LeftVarMode = MACRO_TYPE_LOCAL;
		u_wStartDBIndex 		= g_pDatabase->GetIndex("CNC_MACRO_LOCAL_VAR0");
		MaxNO = PARAMETER_COUNT_LOCAL-MAX_TABLE_NUM+1;
		if(MaxNO<1) MaxNO=1;
		StartNO = 1;
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		pwndEditSTARTNO->SetPropValueT("value",StartNO);
		pwndEditSTARTNO->SetPropValueT("max",MaxNO);
		pwndEditSTARTNO->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
		SetButtonBgc(pwndTemp);
	}
	else if(pwndTemp == pwndBtnGlobal && LeftVarMode != MACRO_TYPE_GLOBAL)
	{
		LeftVarMode = MACRO_TYPE_GLOBAL; 
		u_wStartDBIndex 		= g_pDatabase->GetIndex("CNC_MACRO_GLOBAL_VAR0");
		MaxNO = PARAMETER_COUNT_GLOBAL-MAX_TABLE_NUM+1;
		if(MaxNO<1) MaxNO=1;
		StartNO = 1;
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		pwndEditSTARTNO->SetPropValueT("value",StartNO);
		pwndEditSTARTNO->SetPropValueT("max",MaxNO);
		pwndEditSTARTNO->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
		SetButtonBgc(pwndTemp);
	}
	else if(pwndTemp == pwndBtnSystemVar && LeftVarMode != MACRO_TYPE_SYSTEM)
	{
		LeftVarMode = MACRO_TYPE_SYSTEM;
		u_wStartDBIndex 	= g_pDatabase->GetIndex("MACRO_SYSTEM_VAR0");
		MaxNO = PARAMETER_COUNT_SYSTEM-MAX_TABLE_NUM+1;
		if(MaxNO<1) MaxNO=1;
		StartNO = 1;
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		pwndEditSTARTNO->SetPropValueT("value",StartNO);
		pwndEditSTARTNO->SetPropValueT("max",MaxNO);
		pwndEditSTARTNO->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
		SetButtonBgc(pwndTemp);
	}
	else if(pwndTemp == pwndBtnUp)
	{
		StartNO-=MAX_TABLE_NUM;
		if(StartNO < 1) StartNO = 1;
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		pwndEditSTARTNO->SetPropValueT("value",StartNO);
		pwndEditSTARTNO->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
	}
	else if(pwndTemp == pwndBtnDown)
	{
		StartNO+=MAX_TABLE_NUM;
		if(StartNO > MaxNO) StartNO = MaxNO;
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		pwndEditSTARTNO->SetPropValueT("value",StartNO);
		pwndEditSTARTNO->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
	}
	else if(pwndTemp == pwndBtnDataUp)
	{
		if(ShareMemoryDataMode < MAX_SHOW_DATA_TYPE-1)
			ShareMemoryDataMode++;
		UpdateShareMemoryStatic();
	}
	else if(pwndTemp == pwndBtnDataDown)
	{
		if(ShareMemoryDataMode>0)
			ShareMemoryDataMode--;
		UpdateShareMemoryStatic();
	}
	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	if(wKey == _ENTER)
	{
		if(pwndTemp == pwndEditSTARTNO)
		{
			pwndEditSTARTNO->GetPropValueT("value", &StartNO,sizeof(StartNO));
			if(StartNO < 1) StartNO = 1;
			if(StartNO > MaxNO-MAX_TABLE_NUM+1) StartNO = MaxNO-MAX_TABLE_NUM+1;
			((CtmFormView*)pwndSender)->OnLoseFocus();		
			pwndEditSTARTNO->SetPropValueT("value",StartNO);
			pwndEditSTARTNO->Update();
			((CtmFormView*)pwndSender)->OnGetFocus();
			return	wKey;
		}
		for(int i = 0 ; i < u_nEdit_TableCnt ; i++)
		{
			if(pwndEditTable[i] == pwndTemp)
			{
				int	temp = 370+i+1;
				g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.macro_system_var_set_no,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_single_block));
				return	wKey;
			}	
		}
	}
	return	wKey;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL	LOCK = FALSE;
	static BOOL FristOne = TRUE;
	static int  old_IsrTimer=-1;
	if(FristOne) 
	{
		FristOne =FALSE;
		((CtmFormView*)pwndSender)->Goto(65);
		UpdateShareMemoryStatic();
	}
	UpdateShareMemoryValue();
	//if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_RUN || g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_PAUSE)
	//{
	UpdateMacroData(pwndSender,StartNO);
	//	LOCK = FALSE;
	//}
	//else if(!LOCK)
	//{
	//	LOCK=TRUE;
	//	SetLocalMacroDB_NULL();
	//}
	

}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void	UpdateMacroData(CtmWnd* pwndSender,int f_StartNO)
{
	static int 	Old_StartNO = -1;
	static int	Old_Macro_Mode = -1;
	
	char TextNO[20];
	char TextNO2[40];
	if(f_StartNO != Old_StartNO || Old_Macro_Mode != LeftVarMode)
	{
		BOOL	This_Enabled = TRUE;
		char  Sign;
		int 	NO_offset = 0;
		int 	MaxCount = 0;
		if(LeftVarMode == MACRO_TYPE_LOCAL)
		{
			MaxCount = PARAMETER_COUNT_LOCAL;
			Sign = '#';
			NO_offset = 0;
			This_Enabled = FALSE;
			//This_Enabled = TRUE;
		}
		else if(LeftVarMode == MACRO_TYPE_GLOBAL)
		{
			MaxCount = PARAMETER_COUNT_GLOBAL;
			Sign = '@';
			NO_offset = 0;
			This_Enabled = FALSE;
		}
		else if(LeftVarMode == MACRO_TYPE_SYSTEM)
		{
			MaxCount = PARAMETER_COUNT_SYSTEM;
			Sign = '$';
			NO_offset = 370;
			This_Enabled = TRUE;
		}
	
		for (int i=0; i<u_nStatic_TableCnt; i++)
		{
			if(pwndStaticTable[i] != NULL)
			{	
				memset(TextNO,0,10);
				if(i+f_StartNO<=MaxCount)
					sprintf(TextNO,"%c%d",Sign,i+f_StartNO+NO_offset);
				CodeChange(TextNO2, TextNO);
			  pwndStaticTable[i] -> SetPropValueT("text",TextNO2);
			  pwndStaticTable[i] -> Update();
		  }
		  if(pwndEditTable[i] != NULL)
		  {
			  char*	pDataID = NULL;
		  	if(i+f_StartNO<=MaxCount)
		  	{
	    	  pDataID = g_pDatabase->GetString(u_wStartDBIndex+f_StartNO-1+i);
	    	  if(g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE)
						pwndEditTable[i]->SetPropValueT("enabled",(double)This_Enabled);
					else
						pwndEditTable[i]->SetPropValueT("enabled",(double)FALSE);
	    	}
	    	else
					pwndEditTable[i]->SetPropValueT("enabled",(double)FALSE);
			  pwndEditTable[i] ->SetPropValueT("dbid0",pDataID);
				pwndEditTable[i] ->Update();
		  }
		}		
				
	}
	else
	{
		for (int i=0; i<u_nStatic_TableCnt; i++)
		{
			if(pwndEditTable[i] != NULL)
				pwndEditTable[i] ->Update();
		}
	}
	Old_Macro_Mode=LeftVarMode;
	Old_StartNO = f_StartNO;
}

void    CodeChange(char *pDst, char *pSrc)		//輔助用函式，單字節轉化為雙字節處理
{
	//供StaticX2元件的text屬性使用(目前)
	WORD	*pwDst = (WORD *)pDst;
    
  while(*pSrc != 0)      
  	*pwDst++ = *pSrc++;
    
  *pwDst =0;  
}

void	SetLocalMacroDB_NULL()
{
	for (int i=0; i<u_nEdit_TableCnt; i++)
	{
		if(pwndEditTable[i] != NULL)
		{
			char*	pDataID = NULL;
			pwndEditTable[i]->SetPropValueT("enabled",(double)FALSE);
			pwndEditTable[i]->SetPropValueT("dbid0",pDataID);
			pwndEditTable[i]->Update();
		}
	}
}


void	UpdateShareMemoryValue()
{
	char Name[25];
	char Name2[50];
	static int ShareMemoryDataMode_old = -1;
	if(ShareMemoryDataMode==0)
	{
		switch(cfg.jn)
		{
			case 6:
			pwndEditDataValue[5]	->SetPropValueT("value",g_pRxCNCData->CNC_IsrMachPos.c);
			pwndEditDataValue[11]	->SetPropValueT("value",g_pRxCNCData->CNC_MachProPos.c);
			pwndEditDataValue[17]	->SetPropValueT("value",g_pRxCNCData->CNC_NoAceMachPos.c);
			case 5:
			pwndEditDataValue[4]	->SetPropValueT("value",g_pRxCNCData->CNC_IsrMachPos.b);
			pwndEditDataValue[10]	->SetPropValueT("value",g_pRxCNCData->CNC_MachProPos.b);
			pwndEditDataValue[16]	->SetPropValueT("value",g_pRxCNCData->CNC_NoAceMachPos.b);
			case 4:
			pwndEditDataValue[3]	->SetPropValueT("value",g_pRxCNCData->CNC_IsrMachPos.a);
			pwndEditDataValue[9]	->SetPropValueT("value",g_pRxCNCData->CNC_MachProPos.a);
			pwndEditDataValue[15]	->SetPropValueT("value",g_pRxCNCData->CNC_NoAceMachPos.a);
			case 3:
			pwndEditDataValue[2]	->SetPropValueT("value",g_pRxCNCData->CNC_IsrMachPos.z);
			pwndEditDataValue[8]	->SetPropValueT("value",g_pRxCNCData->CNC_MachProPos.z);
			pwndEditDataValue[14]	->SetPropValueT("value",g_pRxCNCData->CNC_NoAceMachPos.z);
			case 2:
			pwndEditDataValue[1]	->SetPropValueT("value",g_pRxCNCData->CNC_IsrMachPos.y);
			pwndEditDataValue[7]	->SetPropValueT("value",g_pRxCNCData->CNC_MachProPos.y);
			pwndEditDataValue[13]	->SetPropValueT("value",g_pRxCNCData->CNC_NoAceMachPos.y);
			case 1:
			pwndEditDataValue[0]	->SetPropValueT("value",g_pRxCNCData->CNC_IsrMachPos.x);
			pwndEditDataValue[6]	->SetPropValueT("value",g_pRxCNCData->CNC_MachProPos.x);
			pwndEditDataValue[12]	->SetPropValueT("value",g_pRxCNCData->CNC_NoAceMachPos.x);
			break;
		}
		for(int i = 0 ; i < u_nEditDataValueCnt ; i++)
			pwndEditDataValue[i]		->Update();
	}
	else if(ShareMemoryDataMode==1)
	{
		switch(cfg.jn)
		{
			case 6:
			pwndEditDataValue[5]	->SetPropValueT("value",g_pRxCNCData->CNC_servo_lag.c);
			case 5:
			pwndEditDataValue[4]	->SetPropValueT("value",g_pRxCNCData->CNC_servo_lag.b);
			case 4:
			pwndEditDataValue[3]	->SetPropValueT("value",g_pRxCNCData->CNC_servo_lag.a);
			case 3:
			pwndEditDataValue[2]	->SetPropValueT("value",g_pRxCNCData->CNC_servo_lag.z);
			case 2:
			pwndEditDataValue[1]	->SetPropValueT("value",g_pRxCNCData->CNC_servo_lag.y);
			case 1:
			pwndEditDataValue[0]	->SetPropValueT("value",g_pRxCNCData->CNC_servo_lag.x);
			break;
		}
		if(ShareMemoryDataMode_old != ShareMemoryDataMode)	//此數值不會變化  無須重複更新
		{
			switch(cfg.jn)
			{
				case 6:
				pwndEditDataValue[11]	->SetPropValueT("value",(double)GetDBValue("SERVO_PARAM12").lValue*GetDBValue("TUNE_PARAM18").lValue*GetDBValue("TUNE_PARAM64").lValue/3600000000*Pow_Mario(10,GetDBValue("SERVO_PARAM26").lValue));
				case 5:
				pwndEditDataValue[10]	->SetPropValueT("value",(double)GetDBValue("SERVO_PARAM11").lValue*GetDBValue("TUNE_PARAM17").lValue*GetDBValue("TUNE_PARAM63").lValue/3600000000*Pow_Mario(10,GetDBValue("SERVO_PARAM25").lValue));
				case 4:
				pwndEditDataValue[9]	->SetPropValueT("value",(double)GetDBValue("SERVO_PARAM10").lValue*GetDBValue("TUNE_PARAM16").lValue*GetDBValue("TUNE_PARAM62").lValue/3600000000*Pow_Mario(10,GetDBValue("SERVO_PARAM24").lValue));
				case 3:
				pwndEditDataValue[8]	->SetPropValueT("value",(double)GetDBValue("SERVO_PARAM9").lValue*GetDBValue("TUNE_PARAM15").lValue*GetDBValue("TUNE_PARAM61").lValue/3600000000*Pow_Mario(10,GetDBValue("SERVO_PARAM23").lValue));
				case 2:
				pwndEditDataValue[7]	->SetPropValueT("value",(double)GetDBValue("SERVO_PARAM8").lValue*GetDBValue("TUNE_PARAM14").lValue*GetDBValue("TUNE_PARAM60").lValue/3600000000*Pow_Mario(10,GetDBValue("SERVO_PARAM22").lValue));
				case 1:
				pwndEditDataValue[6]	->SetPropValueT("value",(double)GetDBValue("SERVO_PARAM7").lValue*GetDBValue("TUNE_PARAM13").lValue*GetDBValue("TUNE_PARAM59").lValue/3600000000*Pow_Mario(10,GetDBValue("SERVO_PARAM21").lValue));
				break;
			}
		}
		
		pwndEditDataValue[12]	->SetPropValueT("value",(double)0);
		pwndEditDataValue[13]	->SetPropValueT("value",(double)0);  
		pwndEditDataValue[14]	->SetPropValueT("value",(double)0);  
		pwndEditDataValue[15]	->SetPropValueT("value",(double)0);  
		pwndEditDataValue[16]	->SetPropValueT("value",(double)0);  
		pwndEditDataValue[17]	->SetPropValueT("value",(double)0);  

		for(int i = 0 ; i < u_nEditDataValueCnt ; i++)
			pwndEditDataValue[i]		->Update();
	}
	ShareMemoryDataMode_old=ShareMemoryDataMode;
}


void	UpdateShareMemoryStatic()
{
	char Name[25];
	char Name2[50];
	
	if(ShareMemoryDataMode==0)
	{
		pwndStaticDataType	->SetPropValueT("textID",ShowShareMemoryVarType[ShareMemoryDataMode]);
		pwndStaticDataType	->Update();
		
		for(int i = 0 ; i < u_nEditDataValueCnt ; i++)
		{
			if(PulseDataStatic[i] == "NULL")	break;
			strcpy(Name,PulseDataStatic[i]);
			CodeChange(Name2, Name);
			pwndStaticDataName[i]		->SetPropValueT("text",Name2);
			pwndStaticDataName[i]		->Update();
		}
	}
	else if(ShareMemoryDataMode==1)
	{
		pwndStaticDataType	->SetPropValueT("textID",ShowShareMemoryVarType[ShareMemoryDataMode]);
		pwndStaticDataType	->Update();
		for(int i = 0 ; i < u_nEditDataValueCnt ; i++)
		{
			if(ServoLagDataStatic[i] == "NULL")	break;
			strcpy(Name,ServoLagDataStatic[i]);
			CodeChange(Name2, Name);
			pwndStaticDataName[i]		->SetPropValueT("text",Name2);
			pwndStaticDataName[i]		->Update();		
		}
	}
	
	
	
	

	
}

int Pow_Mario(int a , int b)
{
	int tmp=1;
	for(int i = 0 ; i < b ; i++)
		tmp*=a;
	//printf("%d\n",tmp);	
	return tmp;
	
}

void	SetButtonBgc(CtmWnd* f_pwndButton)
{
	static CtmWnd* Last_pwndButton=NULL;
	if(Last_pwndButton != NULL)
	{
		Last_pwndButton->SetPropValueT("bgc",0x0416);
		Last_pwndButton->Update();
	}
	Last_pwndButton=f_pwndButton;
	if(f_pwndButton!=NULL)
	{
		f_pwndButton->SetPropValueT("bgc",0xFC00);
		f_pwndButton->Update();
	}	
}