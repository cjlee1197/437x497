/*============================================================================+
|  Class    : ViewMagBox library                                            	|
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


#include	"ViewMsgBoxSetCoord.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
/*========================================================+
|						Global Variable																|
+========================================================*/
//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;
CtmWnd*		u_pwndPointEditTABLE	= NULL;

CtmWnd*		u_pwndEditTable[7]		={NULL};
CtmWnd*			u_pwndStaticUnit[3]				={NULL};	//卡氏座標單位(mm <-> in)元件指針

int					u_pwndStaticUnitCnt				=0;
int				u_pwndEditTableCnt =0;

BOOL			StartRunOne = TRUE;
int				UnitMode										=0;
/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy↓
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndBtnOK 					= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel				= pwndSender->FindControlFromName("btnCancel");
	u_pwndPointEditTABLE	= pwndSender->FindControlFromName("PointTABLE");
	u_pwndEditTableCnt		= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwndEditTable);
	u_pwndStaticUnitCnt= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnit);
		
	UnitMode									= GetDBValue("TUNE_PARAM37").lValue;		//取得公英制
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{	
		if(pwndTemp == u_pwndBtnCancel)
		{
			remove("./DialogHelp/buffer");
			Exit();
		}
	}
	
	return wIDControl;
}
//20140624↑

WORD OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
		if(pwndTemp == u_pwndBtnOK)
		{
			
			int temp=0;
			char temp2[16] ="\0";
			char CommandStr[200] ="\0";
				u_pwndPointEditTABLE->GetPropValueT("value", &temp, sizeof(temp2));
			if(temp > 0 && temp < 1000)
			{
				sprintf(CommandStr,"C%d =(",temp);
				double temp_in = 10000/g_Metric_Imperial_Unit;
				double temp_mm = 1000;
				for(int i=0 ; i<cfg.cn ; i++)
				{
					u_pwndEditTable[i]->GetPropValueT("value", &temp, sizeof(temp));
					if(i<3 && UnitMode)
						sprintf(temp2,"%.3f",(double)temp/temp_in);
					else if(i<3 && !UnitMode)
						sprintf(temp2,"%.3f",(double)temp/temp_mm);
					else
						sprintf(temp2,"%.3f",(double)temp/1000);
					ClearCoordZeor(temp2,strlen(temp2));
					if(i==0)
						sprintf(CommandStr,"%s%s=%s",CommandStr,cfg.c_s[i],temp2);
					else 
						sprintf(CommandStr,"%s %s=%s",CommandStr,cfg.c_s[i],temp2);
				}
				sprintf(CommandStr,"%s);",CommandStr);
				AddCommand(CommandStr);
				Exit();
			}
			else
			{
				MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
				remove("./DialogHelp/buffer");
			}
		}
		else if(pwndTemp == u_pwndBtnCancel)
		{
			remove("./DialogHelp/buffer");
			Exit();
		}
		((CtmFormView*)pwndSender)->Goto(wIDControl);
	return wIDControl;
}


WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;	
}





void		OnUpdateA(CtmWnd* pwndSender)
{
	if(StartRunOne)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		SetUnit();
		for(int i=0 ; i<u_pwndEditTableCnt-1 ; i++)
		{
			if(i < 3)
			u_pwndEditTable[i]->SetPropValueT("precision",3+UnitMode);
			u_pwndEditTable[i]->Update();
		}
		
		if(RevertCommand != NULL)
		{
			if(!RevertOldCommand(RevertCommand))
			{
				remove("./DialogHelp/buffer");
				Exit();
			}
		}
		
		((CtmFormView*)pwndSender)->OnGetFocus();
		
		StartRunOne = FALSE;
	}
}

/*========================================================+
|						Helper																				|
+========================================================*/

void		AddCommand(char* pszCommand)		//插入指令
{
	//藉由額外產生的檔案來暫存要插入的指令
	FILE*		fCommandBuffer;
	fCommandBuffer = fopen("./DialogHelp/buffer", "w");
	fprintf(fCommandBuffer, "%s", pszCommand);
	fclose(fCommandBuffer);
}


void 		SetUnit()
{
	for(int i = 0 ; i < 3 ; i++ )
	{
		if(UnitMode)
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
		else
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
		u_pwndStaticUnit[i]->Update();
		}
}


void		ClearCoordZeor(char *Coord,int Num)					//整理座標 去除小數點後多餘0
{
	for(int i =Num-1 ; i > 0 ; i--)
	{
		if( *(Coord+i) == '0')
			*(Coord+i)=NULL;
		else if(*(Coord+i) == '.' )
		{
			*(Coord+i)=NULL;
			break;
		}
		else
			break;
	}
}

BOOL		RevertOldCommand(char *Command)
{
	/*==========所需參數======================*/
	/**/int CoordNO=0;											/**/
	/**/long CoordValue[7]={0,0,0,0,0,0,0};	/**/
	/**/																		/**/
	/*========================================*/
	char *ptr = NULL;
	char *temp = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	//移除 「/*註解*/」
	if(!PassMark2(ptr,Command)) return FALSE;
	ptr = PassSpace(ptr);
	
	if(*ptr != 'C') return FALSE;
	ptr++;
	if(!isdigit(*ptr))	return FALSE;
	CoordNO = atoi(ptr);
	while(isdigit(*ptr)) ptr++;
	temp = strstr(ptr,"(");
	if (temp == NULL) return FALSE;
	temp = PassSpace(temp+1);
	for(int i=0 ; i<(cfg.cn) ; i++)
	{
		temp = strstr(temp,cfg.c_s[i]);
		if(temp == NULL) return FALSE;
		temp = PassSpaceFindChar(temp+strlen(cfg.c_s[i]),'=');
		if(temp == NULL)	return FALSE;
		temp++;
		if(ISValue(temp) && *(temp) != '+' )
		{
			if(i < cfg.cn)
			{
				float d_CoordValue = atof(temp);
				CoordValue[i] = (long)(d_CoordValue*1000);
			}
			else
				CoordValue[6] = atoi(temp);
		}
		else return FALSE;		
	}
	u_pwndPointEditTABLE->SetPropValueT("value",CoordNO);
	u_pwndPointEditTABLE->Update();
	for(int i=-0 ; i<(cfg.cn) ; i++)
	{
		u_pwndEditTable[i]->SetPropValueT("value",CoordValue[i]);
		u_pwndEditTable[i]->Update();
	}
	return TRUE;
}