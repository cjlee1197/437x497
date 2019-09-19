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

#include	"ViewMsgBoxEditTeachPoint.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"

/*========================================================+
|						Constants																			|
+========================================================*/
#define		REVERT		0
#define		ABSCOORD	1

#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
/*========================================================+
|						Global Variable																|
+========================================================*/

//判斷是否按下ENTER鍵
BOOL			u_bEnterKey						= FALSE;

CtmWnd*		u_pwndStaticTitle			= NULL;

CtmWnd*		u_pwndBtnOK						= NULL;
CtmWnd*		u_pwndBtnCancel				= NULL;

CtmWnd*		u_pwndBtnNowPos				= NULL;
CtmWnd*		u_pwndBtnRevertValue	= NULL;

CtmWnd*			u_pwndEditTable[7]				={NULL};	//輸入卡氏座標元件指針
int					u_pwndEditTableCnt				=0;				//輸入卡氏座標元件指針總數
CtmWnd*			u_pwndStaticUnit[3]				={NULL};	//卡氏座標單位(mm <-> in)元件指針
int					u_pwndStaticUnitCnt				=0;				//卡氏座標單位(mm <-> in)元件指針總數

int		TeachPointNO = 0;
LONG_AXIS	TeachPointValue;
int		TeachPointPosture = 0;
BOOL	RunOnlyOne = FALSE;
int		Unitindex = 0;

WORD		u_wDBvalueC_StartNo					=0;		//控制參數編號：卡氏座標的X座標
/*========================================================+
|						Function																			|
+========================================================*/


BOOL		OnCreateA(CtmWnd* pwndSender)
{
	u_pwndStaticTitle	= pwndSender->FindControlFromName("TeachPointText");
	
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	
	
	u_pwndBtnNowPos				= pwndSender->FindControlFromName("btnNowPos");
	u_pwndBtnRevertValue	= pwndSender->FindControlFromName("btnRevertValue");
	
	u_pwndEditTableCnt= GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwndEditTable);
	u_pwndStaticUnitCnt= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnit);
	
	u_wDBvalueC_StartNo		   	= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	TeachPointNO = g_wTeachPointNo;
	g_wTeachPointNo = 0;
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if(u_bEnterKey)
	{
		if(pwndTemp == u_pwndBtnOK)
		{
			GetTeachPointValue();
			if(EditTeachPoint(TeachPointNO,TeachPointValue,TeachPointPosture))
				Exit();	
			else
				MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NULL_TEACHPOINT"], tmFT_CODE_TECH);
		}
		else if(pwndTemp == u_pwndBtnCancel)
			Exit();	
		else if(pwndTemp == u_pwndBtnNowPos)
		{
			((CtmFormView*)pwndSender)->OnLoseFocus();
			GetTableValue(ABSCOORD);
			((CtmFormView*)pwndSender)->OnGetFocus();
		}
	}
	return wIDControl;
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnOK)
	{
		GetTeachPointValue();
		if(EditTeachPoint(TeachPointNO,TeachPointValue,TeachPointPosture))
			Exit();
		else
			MsgBoxConfirm(g_MultiLanguage["ROBOT_MSG_NULL_TEACHPOINT"], tmFT_CODE_TECH);
	}
	else if(pwndTemp == u_pwndBtnCancel)
	{
		Exit();	
	}
	else if(pwndTemp == u_pwndBtnNowPos)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();
		GetTableValue(ABSCOORD);
		((CtmFormView*)pwndSender)->OnGetFocus();
	}
	return wIDControl;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	if(!RunOnlyOne)
	{
		SetText();
		SetUnit();
		((CtmFormView*)pwndSender)->OnLoseFocus();
		GetTableValue(REVERT);
		((CtmFormView*)pwndSender)->OnGetFocus();
		RunOnlyOne = TRUE;
	}
}

/*========================================================+
|											Helper															|
+========================================================*/

void	GetTableValue(int Mode)
{
	switch(Mode)
	{
		case ABSCOORD :
		{
			for(int i =0; i<u_pwndEditTableCnt; i++)		//卡氏座標(Cartesian)		POS另外處理
			{
				if(u_pwndEditTable[i] != NULL)
				{
		      char*	pDataID 	= NULL;
		      pDataID 	= g_pDatabase->GetString(u_wDBvalueC_StartNo+i);
		      double	lDBvalue	=0;
		      lDBvalue	= GetDBValue(pDataID).lValue;
		      if(i < 3)	
		      {
						lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
						lDBvalue += 0.5*SIGN(lDBvalue);
					}
		      u_pwndEditTable[i]->SetPropValueT("value", lDBvalue);
		      u_pwndEditTable[i]->Update();
				}
			}
		}
		break;
		case REVERT :
		{
			LONG_AXIS TPValue;
			int TPPos=0;
			if(GetTechPointValue(&TPValue,&TPPos,TeachPointNO))
			{
				for(int j = 0 ; j < u_pwndEditTableCnt; j++)
				{
					if(u_pwndEditTable[j] != NULL)
					{
						long	CoordValue=0;
						memcpy(&CoordValue,(&TPValue.x)+1*j,sizeof(CoordValue));
						double lDBvalue = CoordValue;
						if(j < 3)
						{
							lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	
							lDBvalue += 0.5*SIGN(lDBvalue);	
						}
						u_pwndEditTable[j]->SetPropValueT("value",lDBvalue);
						u_pwndEditTable[j]->Update();
					}
				}
			}
		}
		break;
	}
}

void	GetTeachPointValue()
{
	switch(cfg.cn)
	{
		case 6:
		u_pwndEditTable[5]->GetPropValueT("value",&TeachPointValue.c,sizeof(long));
		case 5:
		u_pwndEditTable[4]->GetPropValueT("value",&TeachPointValue.b,sizeof(long));
		case 4:
		u_pwndEditTable[3]->GetPropValueT("value",&TeachPointValue.a,sizeof(long));
		case 3:
		u_pwndEditTable[2]->GetPropValueT("value",&TeachPointValue.z,sizeof(long));
		case 2:
		u_pwndEditTable[1]->GetPropValueT("value",&TeachPointValue.y,sizeof(long));
		case 1:
		u_pwndEditTable[0]->GetPropValueT("value",&TeachPointValue.x,sizeof(long));
		break;
	}
	
	TeachPointValue.x	=	TeachPointValue.x*(!Unitindex) + TeachPointValue.x*Unitindex*g_Metric_Imperial_Unit/10;	 
	TeachPointValue.x += 0.5*SIGN(TeachPointValue.x);
	TeachPointValue.y	=	TeachPointValue.y*(!Unitindex) + TeachPointValue.y*Unitindex*g_Metric_Imperial_Unit/10;	 
	TeachPointValue.y += 0.5*SIGN(TeachPointValue.y);
	TeachPointValue.z	=	TeachPointValue.z*(!Unitindex) + TeachPointValue.z*Unitindex*g_Metric_Imperial_Unit/10;	 
	TeachPointValue.z += 0.5*SIGN(TeachPointValue.z);

	//u_pwndEditTable[cfg.cn]->GetPropValueT("value",&TeachPointPosture,sizeof(int));

}

void	SetUnit()	
{
	if(Unitindex == 0)
	{
		for(int i = 0 ; i < u_pwndStaticUnitCnt ; i++)
		{
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
			u_pwndStaticUnit[i]->Update();
		}

		for(int j = 0 ; j < 3 ; j++)
		{
			u_pwndEditTable[j]->SetPropValueT("precision",3);
			u_pwndEditTable[j]->Update();
		}

	}
	else
	{
		for(int i = 0 ; i < u_pwndStaticUnitCnt ; i++)
		{
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
			u_pwndStaticUnit[i]->Update();
		}

		for(int j = 0 ; j < 3 ; j++)
		{
			u_pwndEditTable[j]->SetPropValueT("precision",4);
			u_pwndEditTable[j]->Update();
		}
	}
}

void	SetText()
{
	char	temp[30] 		= "\0";
	char	szTemp[30]		= "\0";
	memset(temp, 0, sizeof(temp));
	memset(szTemp, 0, sizeof(szTemp));
	sprintf(temp, "TeachPoint NO.%d",TeachPointNO);
	CodeChange(szTemp, temp);
	u_pwndStaticTitle->SetPropValueT("text", szTemp);
	u_pwndStaticTitle->Update();
}