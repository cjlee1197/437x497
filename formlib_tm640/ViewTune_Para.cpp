/*===========================================================================+
|  Class    : ViewTune_Para library                                          |
|  Task     : ViewTune_Para library action source file                       |
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
#include 	"ViewTune_Para.h"
#include 	"../commonaction.h"
#include	"../font.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     SERVP_ROWS              10
#define			GROUP_NUM								8

#define			OPER_SEL								0xFC00
#define			OPER_NO_SEL							0x0416
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
char*	u_szTuneP_Common_Index[] 	={
	 	"S19",		
	 	"S27",	
	 	"S93",		
	 	"S94",			
	 	"S20",		
	 	"S78",		
	 	"S79",		
	 	"S80",		
	 	"NULL",
	 	"NULL",
		"NULL"
};
char*	u_szTuneP_Common_DBID[] 	={
	 	"SERVO_PARAM19",	
	 	"SERVO_PARAM27",
	 	"SERVO_PARAM93",	
	 	"SERVO_PARAM94",		
	 	"SERVO_PARAM20",	
	 	"SERVO_PARAM78",	
	 	"SERVO_PARAM79",	
	 	"SERVO_PARAM80",	
	 	"NULL",
		"NULL",
	 	"NULL"  
};
char*	u_szTuneP_Common_Description[] 	={
	 	"SERVO_PARAM_STR_19",
	 	"SERVO_PARAM_STR_27",
	 	"SERVO_PARAM_STR_93",
	 	"SERVO_PARAM_STR_94",
	 	"SERVO_PARAM_STR_20",
	 	"SERVO_PARAM_STR_78",
	 	"SERVO_PARAM_STR_79",
	 	"SERVO_PARAM_STR_80",
	 	"NULL",
	 	"NULL",
	 	"NULL" 
};
char*	u_szTuneP_Common_Avail[] 	={
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"ROBOT_STR_NOW_UPDATE",		
	 	"NULL",
	 	"NULL",
		"NULL"
};

char*		u_szTuneP_X_Index[] 	={
		"S13",	
		"S28",	
		"S81",
		"S87",
		"S66",	
		"S72",	
		"S60",	
		"S42",
		"S35",	
		"S48",	 
		"NULL"
};
char*		u_szTuneP_X_DBID[] 	={
		"SERVO_PARAM13",	
		"SERVO_PARAM28",	
		"SERVO_PARAM81",	
		"SERVO_PARAM87",		
		"SERVO_PARAM66",
		"SERVO_PARAM72",
		"SERVO_PARAM60",			
		"SERVO_PARAM42",
		"SERVO_PARAM35",	
		"SERVO_PARAM48",	
		"NULL"		
};
char*		u_szTuneP_X_Description[] 	={
		"SERVO_PARAM_STR_13",	
		"SERVO_PARAM_STR_28",	
		"SERVO_PARAM_STR_81",	
		"SERVO_PARAM_STR_87",	
		"SERVO_PARAM_STR_66",	
		"SERVO_PARAM_STR_72",	
		"SERVO_PARAM_STR_60",	
		"SERVO_PARAM_STR_42",	
		"SERVO_PARAM_STR_35",	
		"SERVO_PARAM_STR_48",	
		"NULL"					
};
char*		u_szTuneP_X_Avail[] 	={
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"NULL"
}; 

char*		u_szTuneP_Y_Index[] 	={
		"S14",	
		"S29",	
		"S82",
		"S88",
		"S67",	
		"S73",	
		"S61",
		"S43",	
		"S36",	
		"S49",	
		"NULL"
};
char*		u_szTuneP_Y_DBID[] 	={
		"SERVO_PARAM14",	
		"SERVO_PARAM29",	
		"SERVO_PARAM82",	
		"SERVO_PARAM88",
		"SERVO_PARAM67",	
		"SERVO_PARAM73",	
		"SERVO_PARAM61",	
		"SERVO_PARAM43",	
		"SERVO_PARAM36",	
		"SERVO_PARAM49",		
		"NULL"		
};
char*		u_szTuneP_Y_Description[] 	={
		"SERVO_PARAM_STR_14",	
		"SERVO_PARAM_STR_29",	
		"SERVO_PARAM_STR_82",	
		"SERVO_PARAM_STR_88",	
		"SERVO_PARAM_STR_67",	
		"SERVO_PARAM_STR_73",	
		"SERVO_PARAM_STR_61",	
		"SERVO_PARAM_STR_43",	
		"SERVO_PARAM_STR_36",	
		"SERVO_PARAM_STR_49",	
		"NULL"					
};
char*		u_szTuneP_Y_Avail[] 	={
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"NULL"
};

char*		u_szTuneP_Z_Index[] 	={
		"S15",	
		"S30",
		"S83",
		"S89",	
		"S68",	
		"S74",	
		"S62",
		"S44",	
		"S37",	
		"S50",	
		"NULL"
};
char*		u_szTuneP_Z_DBID[] 	={
		"SERVO_PARAM15",	
		"SERVO_PARAM30",	
		"SERVO_PARAM83",	
		"SERVO_PARAM89",
		"SERVO_PARAM68",	
		"SERVO_PARAM74",	
		"SERVO_PARAM62",	
		"SERVO_PARAM44",		
		"SERVO_PARAM37",	
		"SERVO_PARAM50",	
		"NULL"		
};
char*		u_szTuneP_Z_Description[] 	={
		"SERVO_PARAM_STR_15",	
		"SERVO_PARAM_STR_30",	
		"SERVO_PARAM_STR_83",	
		"SERVO_PARAM_STR_89",	
		"SERVO_PARAM_STR_68",	
		"SERVO_PARAM_STR_74",	
		"SERVO_PARAM_STR_62",	
		"SERVO_PARAM_STR_44",	
		"SERVO_PARAM_STR_37",	
		"SERVO_PARAM_STR_50",	
		"NULL"					
};
char*		u_szTuneP_Z_Avail[] 	={
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"NULL"
};

char*		u_szTuneP_A_Index[] 	={
		"S16",	
		"S31",	
		"S84",
		"S90",
		"S69",	
		"S75",	
		"S63",
		"S45",	
		"S38",	
		"S51",	
		"NULL"
};
char*		u_szTuneP_A_DBID[] 	={
		"SERVO_PARAM16",	
		"SERVO_PARAM31",	
		"SERVO_PARAM84",	
		"SERVO_PARAM90",
		"SERVO_PARAM69",	
		"SERVO_PARAM75",	
		"SERVO_PARAM63",	
		"SERVO_PARAM45",		
		"SERVO_PARAM38",	
		"SERVO_PARAM51",	
		"NULL"		
};
char*		u_szTuneP_A_Description[] 	={
		"SERVO_PARAM_STR_16",	
		"SERVO_PARAM_STR_31",	
		"SERVO_PARAM_STR_84",	
		"SERVO_PARAM_STR_90",	
		"SERVO_PARAM_STR_69",	
		"SERVO_PARAM_STR_75",	
		"SERVO_PARAM_STR_63",	
		"SERVO_PARAM_STR_45",	
		"SERVO_PARAM_STR_38",	
		"SERVO_PARAM_STR_51",	
		"NULL"					
};
char*		u_szTuneP_A_Avail[] 	={
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"NULL"
};

char*		u_szTuneP_B_Index[] 	={
		"S17",	
		"S32",	
		"S85",
		"S91",
		"S70",	
		"S76",	
		"S64",	
		"S46",
		"S39",	
		"S52",	
		"NULL"
};
char*		u_szTuneP_B_DBID[] 	={
		"SERVO_PARAM17",	
		"SERVO_PARAM32",	
		"SERVO_PARAM85",	
		"SERVO_PARAM91",	
		"SERVO_PARAM70",	
		"SERVO_PARAM76",	
		"SERVO_PARAM64",	
		"SERVO_PARAM46",
		"SERVO_PARAM39",		
		"SERVO_PARAM52",	
		"NULL"		
};
char*		u_szTuneP_B_Description[] 	={
		"SERVO_PARAM_STR_17",	
		"SERVO_PARAM_STR_32",	
		"SERVO_PARAM_STR_85",	
		"SERVO_PARAM_STR_91",	
		"SERVO_PARAM_STR_70",	
		"SERVO_PARAM_STR_76",	
		"SERVO_PARAM_STR_64",	
		"SERVO_PARAM_STR_46",	
		"SERVO_PARAM_STR_39",	
		"SERVO_PARAM_STR_52",	
		"NULL"					
};
char*		u_szTuneP_B_Avail[] 	={
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"NULL"
};         

char*		u_szTuneP_C_Index[] 	={
		"S18",	
		"S33",
		"S86",
		"S92",	
		"S71",	
		"S77",	
		"S65",
		"S47",	
		"S40",	
		"S53",	
		"NULL"
};
char*		u_szTuneP_C_DBID[] 	={
		"SERVO_PARAM18",	
		"SERVO_PARAM33",	
		"SERVO_PARAM86",	
		"SERVO_PARAM92",	
		"SERVO_PARAM71",	
		"SERVO_PARAM77",	
		"SERVO_PARAM65",	
		"SERVO_PARAM47",	
		"SERVO_PARAM40",	
		"SERVO_PARAM53",	
		"NULL"		
};
char*		u_szTuneP_C_Description[] 	={
		"SERVO_PARAM_STR_18",	
		"SERVO_PARAM_STR_33",	
		"SERVO_PARAM_STR_86",	
		"SERVO_PARAM_STR_92",	
		"SERVO_PARAM_STR_71",	
		"SERVO_PARAM_STR_77",	
		"SERVO_PARAM_STR_65",	
		"SERVO_PARAM_STR_47",	
		"SERVO_PARAM_STR_40",	
		"SERVO_PARAM_STR_53",	
		"NULL"					
};
char*		u_szTuneP_C_Avail[] 	={
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"ROBOT_STR_NOW_UPDATE",	
		"NULL"
};

char**		Tune_Para_Index[] 	={
	u_szTuneP_Common_Index,
	u_szTuneP_X_Index,
	u_szTuneP_Y_Index,
	u_szTuneP_Z_Index,
	u_szTuneP_A_Index,
	u_szTuneP_B_Index,
	u_szTuneP_C_Index,
	NULL
};

char**		Tune_Para_DBID[] 	={
	u_szTuneP_Common_DBID,
	u_szTuneP_X_DBID,
	u_szTuneP_Y_DBID,
	u_szTuneP_Z_DBID,
	u_szTuneP_A_DBID,
	u_szTuneP_B_DBID,
	u_szTuneP_C_DBID,
	NULL
};

char** 		Tune_Para_Description[] 	={
	u_szTuneP_Common_Description,
	u_szTuneP_X_Description,
	u_szTuneP_Y_Description,
	u_szTuneP_Z_Description,
	u_szTuneP_A_Description,
	u_szTuneP_B_Description,
	u_szTuneP_C_Description,
	NULL
};

char** 		Tune_Para_Avail[] 	={
	u_szTuneP_Common_Avail,
	u_szTuneP_X_Avail,
	u_szTuneP_Y_Avail,
	u_szTuneP_Z_Avail,
	u_szTuneP_A_Avail,
	u_szTuneP_B_Avail,
	u_szTuneP_C_Avail,
	NULL
};

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*		pwndStaticIndexTable[SERVP_ROWS] 	= {NULL};
CtmWnd*		pwndEditValueTable	[SERVP_ROWS] 	= {NULL};
CtmWnd*		pwndStaticTextTable	[SERVP_ROWS] 	= {NULL};
CtmWnd*		pwndStaticAvailTable[SERVP_ROWS] 	= {NULL};

CtmWnd*		pwndBtnGroupTable[GROUP_NUM] 	= {NULL};

CtmWnd*		u_pwndImageBoxLock = NULL;
CtmWnd*		u_pwndStaticTitle	 = NULL;

int	u_nStaticIndexTableCnt	=	0;
int	u_nEditValueTableCnt		=	0;
int	u_nStaticTextTableCnt		=	0;
int	u_nStaticAvailTableCnt	=	0;
int	u_nBtnGroupTableCnt			=	0;

WORD		u_wPara_Type 				= 0;
BOOL	AutoFalg = FALSE;
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	u_nStaticIndexTableCnt	= GetSpecialControlNum(pwndSender,"StaticTableIndex"	, "CtmStaticX2"	, pwndStaticIndexTable);
	u_nEditValueTableCnt		= GetSpecialControlNum(pwndSender,"EditTableValue"		, "CtmEditX1"		, pwndEditValueTable);
	u_nStaticTextTableCnt		= GetSpecialControlNum(pwndSender,"StaticTableText"		, "CtmStaticX2"	, pwndStaticTextTable);
	u_nStaticAvailTableCnt	= GetSpecialControlNum(pwndSender,"StaticTableAvail"	, "CtmStaticX2"	, pwndStaticAvailTable);
	
	u_nBtnGroupTableCnt			= GetSpecialControlNum(pwndSender,"BtnParamGroup"			, "CtmToolButton"	, pwndBtnGroupTable);
	
	u_pwndImageBoxLock			= pwndSender->FindControlFromName("ImageBoxLock");	
	u_pwndStaticTitle				= pwndSender->FindControlFromName("StaticWORK_DATA");	
	
	pwndBtnGroupTable[u_wPara_Type]->SetPropValueT("bgc",OPER_SEL);
	pwndBtnGroupTable[u_wPara_Type]->CreateA();
	pwndBtnGroupTable[u_wPara_Type]->Update();
	UpdateTunePTable();
	return TRUE;
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	for(int i = 0 ; i < GROUP_NUM ; i++ )
	{
		if(pwndBtnGroupTable[i] == pwndTemp)
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			pwndBtnGroupTable[u_wPara_Type]->SetPropValueT("bgc",OPER_NO_SEL);
			pwndBtnGroupTable[u_wPara_Type]->CreateA();
			pwndBtnGroupTable[u_wPara_Type]->Update();
			u_wPara_Type = i;
			pwndBtnGroupTable[u_wPara_Type]->SetPropValueT("bgc",OPER_SEL);
			pwndBtnGroupTable[u_wPara_Type]->CreateA();
			pwndBtnGroupTable[u_wPara_Type]->Update();
			UpdateTunePTable();
			return wIDControl;
		}
	}
	for(int i = 0 ; i < SERVP_ROWS ; i++ )
	{
		if(pwndEditValueTable[i] == pwndTemp)
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

void		OnUpdateA(CtmWnd*	pwndSender)
{
	static BOOL FristOne = TRUE;
	static BOOL old_EditLock = FALSE;
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
		for (int i=0; i<SERVP_ROWS; i++)
    {
      if(pwndEditValueTable[i] != NULL)
      {
				if(old_EditLock)
        	pwndEditValueTable[i]->SetPropValueT("enabled",(double)FALSE);
				else
        	pwndEditValueTable[i]->SetPropValueT("enabled",(double)TRUE);
        pwndEditValueTable[i]->Update();
      } 
    }
  }
}


/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void    UpdateTunePTable()
{
  WORD    *pwDst, *pwSrc;
	char	temp[128];
	char	szTemp[256];
	BOOL	bEnable =TRUE;
	
	for (int i=0; i<SERVP_ROWS; i++)
  {
  	memset(temp,0,128);
  	memset(szTemp,0,256);
  	if(Tune_Para_Index[u_wPara_Type][i] == "NULL")
  	{
  		sprintf(temp ," ");
	    CodeChange(szTemp, temp);
	    pwndStaticIndexTable[i] -> SetPropValueT("text",szTemp);
	    
			pwndEditValueTable[i]->SetPropValueT("dbid0","");
			pwndEditValueTable[i]->SetPropValueT("enabled",(double)FALSE);
  		
	    pwndStaticTextTable[i] -> SetPropValueT("textID","ROBOT_STR_DUMMY");
  		
	    pwndStaticAvailTable[i] -> SetPropValueT("textID","ROBOT_STR_DUMMY");
  	}
  	else
  	{
  		//Index
	    sprintf(temp ,"%s",Tune_Para_Index[u_wPara_Type][i]);
	    CodeChange(szTemp, temp);
	    pwndStaticIndexTable[i] -> SetPropValueT("text",szTemp);
	    
	    //VALUE
	    if(AutoFalg)
				pwndEditValueTable[i]->SetPropValueT("enabled",(double)FALSE);
	    else 
				pwndEditValueTable[i]->SetPropValueT("enabled",(double)TRUE);
	  	pwndEditValueTable[i] -> Update();
			pwndEditValueTable[i]->SetPropValueT("dbid0",Tune_Para_DBID[u_wPara_Type][i]);
      
      //Text
	    pwndStaticTextTable[i] -> SetPropValueT("textID",Tune_Para_Description[u_wPara_Type][i]);
	    
	    //Avail
	    pwndStaticAvailTable[i] -> SetPropValueT("textID",Tune_Para_Avail[u_wPara_Type][i]);
    } 
	  pwndStaticIndexTable[i] -> Update();
	  pwndEditValueTable[i] -> Update();
	  pwndStaticTextTable[i] -> Update();
	  pwndStaticAvailTable[i] -> Update();
  }
}

void    CodeChange(char *pDst, char *pSrc)
{
    WORD    *pwDst  = (WORD *)pDst;
    
    while(*pSrc != 0)      *pwDst ++ = *pSrc ++;
    
    *pwDst  = 0;
    
}