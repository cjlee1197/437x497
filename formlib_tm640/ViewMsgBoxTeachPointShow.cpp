/*============================================================================+
|  Class    : ViewMessageBoxCommandExplanation library                        |
|  Task     : ViewMessageBoxCommandExplanation library action source file			|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxTeachPointShow.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../memo.h"
#include	"../tmshmsg.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
#define	MAX_TEACNPOINT_NUM  24
#define START_WORK_COORD_SYS 1
#define START_TEACH_POINT 1

/*========================================================+
|						Global Variable																|
+========================================================*/

//CtmWnd*		u_pwndLTextMemoCommandExplanation		= NULL;
//CtmWnd*		u_pwndBtnPageDown										= NULL;
//CtmWnd*		u_pwndBtnPageUp											= NULL;

CtmWnd*		u_pwndBtnSearch				= NULL;
CtmWnd*		u_pwndEditTPNO				= NULL;
CtmWnd*		u_pwndEditCoordSysNo	= NULL;

CtmWnd*		u_pwndBtnPageDown			= NULL;
CtmWnd*		u_pwndBtnPageUp				= NULL;
CtmWnd*		u_pwndBtnclose				= NULL;
CtmWnd*		u_pwndPointTable[4]		= {NULL};
CtmWnd*		u_pwndEditTable[28]		= {NULL};
CtmWnd*		u_pwndUnitTable[12]		= {NULL};
//CtmWnd*		u_pwndPosTable[3]			={NULL};

int 		u_nPointTableCnt 			=0;
int			u_nEditTableCnt				=0;
int 		u_nUnitTableCnt   		=0;
int 		u_nPosTable						=0;

int			u_nTableShowNum					= 3;		//Table一次印出的教導點個數

int			u_nWorkCoordSys					=START_WORK_COORD_SYS;		//目前指定的座標系
int			u_nTeachPointCnt				=0;		//目前存在的教導點個數
WORD		u_wTableStartNo					=START_TEACH_POINT-1;		//Table印出的起始編號
int			Unitindex								=0;

BOOL			u_bEnterKey													= FALSE;		//判斷ENTER鍵是否被按下
BOOL			RunOnlyOne						=FALSE;
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{	
	u_pwndBtnSearch				= pwndSender->FindControlFromName("btnSearch");
	u_pwndEditTPNO				= pwndSender->FindControlFromName("EditTableTPNo");
	u_pwndEditCoordSysNo	= pwndSender->FindControlFromName("EditCoordSysNo");
	
	u_pwndBtnPageDown			= pwndSender->FindControlFromName("PageDown");
	u_pwndBtnPageUp				= pwndSender->FindControlFromName("PageUp");
	
	u_pwndBtnclose		  	= pwndSender->FindControlFromName("btnClose");
	//計算CtmStaticX2元件中名稱含有"StaticTeachPoint"的個數，並將指針存放於u_pwndPointTable裡
	u_nPointTableCnt			= GetSpecialControlNum(pwndSender, "StaticTeachPoint", "CtmStaticX2", u_pwndPointTable);
	//計算CtmEditX1元件中名稱含有"EditTABLE"的個數，並將指針存放於u_pwndEditTable裡
	u_nEditTableCnt				= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwndEditTable);
	//計算CtmStaticX2元件中名稱含有"StaticUnit"的個數，並將指針存放於u_pwndUnitTable裡
	u_nUnitTableCnt				= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndUnitTable);

	
	u_nTeachPointCnt = GetTeachPointCnt(u_nWorkCoordSys);
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
		
	
	return TRUE;
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp		= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_pwndBtnclose)
	{
		Exit();
	}
	else if(pwndTemp == u_pwndBtnPageDown)		//教導點列表PageDown
	{
		u_wTableStartNo += u_nTableShowNum;
		if(u_wTableStartNo < u_nTeachPointCnt)		//判斷教導點編號是否超出總個數
		{
			if(u_wTableStartNo >= u_nTeachPointCnt-u_nTableShowNum)
				u_wTableStartNo = u_nTeachPointCnt-u_nTableShowNum;
			TeachPointCheck(u_wTableStartNo);
		}
		else
			u_wTableStartNo -=u_nTableShowNum;
	}
	else if(pwndTemp == u_pwndBtnPageUp)		//教導點列表PageUp
	{
		u_wTableStartNo -= u_nTableShowNum;		
		if((u_wTableStartNo < 0)||(u_wTableStartNo > u_nTeachPointCnt-u_nTableShowNum))
			u_wTableStartNo =0;
		TeachPointCheck(u_wTableStartNo);
	}
	else if(pwndTemp == u_pwndBtnSearch)
	{
		int temp = 0;
		u_pwndEditCoordSysNo->GetPropValueT("value", &temp, sizeof(temp));
		if(temp < 1 || temp > MAX_TEACNPOINT_NUM)  
		{
			Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_ALARM_TEACHPOINTSHOW")], 0);	
			return wIDControl;
		}
		u_nWorkCoordSys = temp;
		u_nTeachPointCnt = GetTeachPointCnt(u_nWorkCoordSys);
		temp = 0;
		u_pwndEditTPNO->GetPropValueT("value", &temp, sizeof(temp));
		if(temp < 1 || temp > MAX_TEACNPOINT_NUM)  
		{
			Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_ALARM_TEACHPOINTSHOW")], 0);	
			return wIDControl;
		}
		if(temp-1 < u_nTeachPointCnt && temp > 0 )
		{
			u_wTableStartNo=temp-1;
			TeachPointCheck(u_wTableStartNo);
		}
		else
		{
			Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NULL_TEACHPOINT")], 0);	
		}
	}
	return wIDControl;
}
	

void		OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		u_pwndEditTPNO				->SetPropValueT("value", START_TEACH_POINT);
		u_pwndEditTPNO				->Update();
		u_pwndEditCoordSysNo	->SetPropValueT("value", START_WORK_COORD_SYS);
		u_pwndEditCoordSysNo	->Update();
		((CtmFormView*)pwndSender)->OnGetFocus();
		TeachPointCheck(u_wTableStartNo);
		SetUnit();
		RunOnlyOne = TRUE;
	}
}

/*========================================================+
|						Helper																				|
+========================================================*/

void		TeachPointCheck(WORD wStartNo)		//判斷會顯示幾筆教導點資料
{	
	if(wStartNo > u_nTeachPointCnt-1)
	{
		NoUseTablePart(1);
	}
	else if(wStartNo == u_nTeachPointCnt-1)		//一筆		
	{
		UpdateTable(wStartNo);
		NoUseTablePart(2);
	}
	else if(wStartNo == u_nTeachPointCnt-2)		//兩筆
	{
		UpdateTable(wStartNo);
		NoUseTablePart(3);
	}
	else if(wStartNo == u_nTeachPointCnt-3)		//三筆
	{
		UpdateTable(wStartNo);
		NoUseTablePart(4);
	}
	else		//三筆 M:(或以上) 
	{
		UpdateTable(wStartNo);
	}
}


void		UpdateTable(WORD wStartNo)		//顯示教導點列表
{	
	//顯示教導點編號
	for(int i =0; i<u_nPointTableCnt; i++)
	{
		LONG_AXIS TPValue;
		int TPPos=0;
		int TPNum;
		TPNum = i + wStartNo +1;
    if(GetTechPointValue(&TPValue,&TPPos,TPNum,u_nWorkCoordSys))
		{
			char	temp[15] 		= "\0";
			char	szTemp[15]		= "\0";
			memset(temp, 0, sizeof(temp));
			memset(szTemp, 0, sizeof(szTemp));
			sprintf(temp,"P%d",i + wStartNo +1);
			CodeChange(szTemp, temp);
	    u_pwndPointTable[i]->SetPropValueT("text", szTemp);
	    u_pwndPointTable[i]->Update();
			int tableNum = 0;
			for(int j = 0 ; j < cfg.cn ; j ++)
	    {
	    	tableNum = i*(cfg.cn+1)+j;
	    	
	    	if(u_pwndEditTable[tableNum] != NULL)
				{
					long	CoordValue=0;
					memcpy(&CoordValue,(&TPValue.x)+1*j,sizeof(CoordValue));
					u_pwndEditTable[tableNum]->SetPropValueT("visible", (double)TRUE);
					double lDBvalue = CoordValue;
					if(j < 3)
					{
						lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
						lDBvalue += 0.5*SIGN(lDBvalue);	
					}
					u_pwndEditTable[tableNum]->SetPropValueT("value",lDBvalue);
					u_pwndEditTable[tableNum]->Update();
				}  	
	    }
		}  
	}
}

void		NoUseTablePart(WORD wPartNo)		//將沒有使用的區塊清空
{
	wPartNo-=1;
	
	for(int i = wPartNo; i<u_nPointTableCnt; i++)
	{
		char	temp[15] 		= "\0";
		char	szTemp[15]		= "\0";
		memset(temp, 0, sizeof(temp));
		memset(szTemp, 0, sizeof(szTemp));
		sprintf(temp,"No teach point");
		CodeChange(szTemp, temp);
		u_pwndPointTable[i]->SetPropValueT("text",szTemp);
		u_pwndPointTable[i]->Update();
	}
	
	for(int i = wPartNo*cfg.cn; i<u_nEditTableCnt; i++)
	{
		int a = 0;
		u_pwndEditTable[i]->SetPropValueT("value", a);
		u_pwndEditTable[i]->Update();
	}
}

void		SetUnit()	//設定單位(mm or in)  20140915		by	Mario
{
	if(Unitindex == 0)
	{
		for(int i = 0 ; i < u_nUnitTableCnt ; i++)
		{
			u_pwndUnitTable[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
			u_pwndUnitTable[i]->Update();
		}
		for(int i = 0 ; i < u_nTableShowNum ; i ++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				u_pwndEditTable[i*(cfg.cn)+j]->SetPropValueT("precision",3);
				u_pwndEditTable[i*(cfg.cn)+j]->Update();
			}
		}
	}
	else
	{
		for(int i = 0 ; i < u_nUnitTableCnt ; i++)
		{
			u_pwndUnitTable[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
			u_pwndUnitTable[i]->Update();
		}
		for(int i = 0 ; i < u_nTableShowNum ; i ++)
		{
			for(int j = 0 ; j < 3 ; j++)
			{
				u_pwndEditTable[i*(cfg.cn)+j]->SetPropValueT("precision",4);
				u_pwndEditTable[i*(cfg.cn)+j]->Update();
			}
		}
	}
}