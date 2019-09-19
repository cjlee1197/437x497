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

#include	"ViewMsgBoxSetWorkCoordSeat.h"
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

CtmWnd*		u_pwndCoordTable[6]					= {NULL};
int				u_nCoordTableCnt						=	0;
CtmWnd*		u_pwndOriginTable[6]				= {NULL};
int				u_nOriginTableCnt						=	0;

CtmWnd*		u_pwndOffsetTable[6]				= {NULL};
int				u_nOffsetTableCnt						=	0;
CtmWnd*		u_pwndWorkCoordTable[6]			= {NULL};
int				u_nWorkCoordTableCnt				=	0;
CtmWnd*		u_pwndStaticUnit[12]				= {NULL};
int				u_nStaticUnitCnt						=	0;

CtmWnd*		u_pwndEditWorkCoordNO				= NULL;


int			Unitindex							=0;		//表示目前單位(0=公制,1=英制)，預設0

WORD		u_wDBvalueC_StartNo					=0;		//控制參數編號：卡氏座標的X座標
WORD		u_wDBvalueOrigin_StartNo		=0;		//控制參數編號：機械座標的X座標

/*========================================================+
|						Function																			|
+========================================================*/

//New add by Roy↓
BOOL		OnCreateA(CtmWnd* pwndSender)
{
	//當前座標
	u_nCoordTableCnt		= GetSpecialControlNum(pwndSender, "CTable", 				"CtmEditX1", u_pwndCoordTable);
	u_nOriginTableCnt		= GetSpecialControlNum(pwndSender, "OTable", 				"CtmEditX1", u_pwndOriginTable);
	
	//定義座標位置
	u_nOffsetTableCnt		= GetSpecialControlNum(pwndSender, "EditOffset", 		"CtmEditX1", u_pwndOffsetTable);
	
	//工作座標值
	u_nWorkCoordTableCnt= GetSpecialControlNum(pwndSender, "EditWorkValue", "CtmEditX1", u_pwndWorkCoordTable);
	
	//工作座標編號
	u_pwndEditWorkCoordNO	=	pwndSender->FindControlFromName("EditSetWorkCoordNo");
	
	//單位
	u_nStaticUnitCnt= GetSpecialControlNum(pwndSender, "StaticUnit", "CtmStaticX2", u_pwndStaticUnit);
	
	u_pwndBtnOK 			= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel		= pwndSender->FindControlFromName("btnCancel");
	
	
	u_wDBvalueOrigin_StartNo	= g_pDatabase->GetIndex(MACHINE_COORD_ID);
	u_wDBvalueC_StartNo		   	= g_pDatabase->GetIndex(ABSOLUTE_COORD_ID);
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	for(int i = 0 ; i < u_nOffsetTableCnt ; i++ )
	{
		if(pwndTemp == u_pwndOffsetTable[i])
		{
			ShowWorkCoordValue();
			return wIDControl;
		}
	}
	return wIDControl;
}
//20140624↑
WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);

	if(pwndTemp == u_pwndBtnCancel)
	{
		Exit();
	}
	else if(pwndTemp == u_pwndBtnOK)
	{
		int u_WorkCoordNO = 0;
		u_pwndEditWorkCoordNO->GetPropValueT("value",&u_WorkCoordNO,sizeof(u_WorkCoordNO));
		if(u_WorkCoordNO > 0 && u_WorkCoordNO < 25)
		{
			SetDB_WorkCoordValue(u_WorkCoordNO);
			Exit();
		}
		else
			;
	}
	return wIDControl;
}




void		OnUpdateA(CtmWnd* pwndSender)
{
	static BOOL FristOne = TRUE;
	
	ShowCurrentCoordinate(); 
	
	if(FristOne)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus();		
		u_pwndEditWorkCoordNO->Update();
		FristOne = FALSE;
		SetUnit();
		ShowWorkCoordValue();
		((CtmFormView*)pwndSender)->OnGetFocus();		
	}
}


/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/


void		ShowCurrentCoordinate()		//顯示機械手臂目前的座標位置
{
	for(int i =0; i<u_nCoordTableCnt; i++)		//卡氏座標(Cartesian)		POS另外處理
	{
		if(u_pwndCoordTable[i] != NULL)
		{
			//取得控制參數的資料ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueC_StartNo+i);
      //printf("pDataID :  %s %d %d\n", pDataID, u_wDBvalueC_StartNo, i);
      
      //根據資料ID，取得控制參數的值
      //long	lDBvalue	=0;																							
      double	lDBvalue	=0;																							
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < 3)																									   
      {																																
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
				lDBvalue += 0.5*SIGN(lDBvalue);																								
				u_pwndCoordTable[i]->SetPropValueT("precision",3+Unitindex);		
			}																												
      u_pwndCoordTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndCoordTable[i]->Update();
		}
		if(u_pwndOriginTable[i] != NULL)
		{
			//取得控制參數的資料ID
      char*	pDataID 	= NULL;
      pDataID 	= g_pDatabase->GetString(u_wDBvalueOrigin_StartNo+i);
      //printf("pDataID :  %s %d %d\n", pDataID, u_wDBvalueC_StartNo, i);
      
      //根據資料ID，取得控制參數的值
      //long	lDBvalue	=0;																							
      double	lDBvalue	=0;																							
      lDBvalue	= GetDBValue(pDataID).lValue;
      
      if(i < 3)																									   
      {																																
				lDBvalue	=	lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
				lDBvalue += 0.5*SIGN(lDBvalue);																								
				u_pwndOriginTable[i]->SetPropValueT("precision",3+Unitindex);		
			}																												
      u_pwndOriginTable[i]->SetPropValueT("value", lDBvalue);
      u_pwndOriginTable[i]->Update();
		}	
	}
}

void 		SetUnit()
{
	for(int i = 0 ; i < 3 ; i++)
	{
		if(u_pwndCoordTable[i] != NULL)
		{
			u_pwndCoordTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndCoordTable[i]->Update();
		}
		if(u_pwndOriginTable[i] != NULL)
		{
			u_pwndOriginTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndOriginTable[i]->Update();
		}
		if(u_pwndOffsetTable[i] != NULL)
		{
			u_pwndOffsetTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndOffsetTable[i]->Update();
		}
		if(u_pwndWorkCoordTable[i] != NULL)
		{
			u_pwndWorkCoordTable[i]->SetPropValueT("precision",3+Unitindex);
			u_pwndWorkCoordTable[i]->Update();
		}
	}
	for(int i = 0 ; i < u_nStaticUnitCnt ; i++ )
	{
		if(Unitindex)
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_INCH");
		else
			u_pwndStaticUnit[i]->SetPropValueT("textID","ROBOT_UNIT_MM");
		u_pwndStaticUnit[i]->Update();
	}
}

void		ShowWorkCoordValue()
{
	int u_EXOFS_StartNo		= g_pDatabase->GetIndex("CNC_WORK_COORD0_X");
	for(int i = 0 ; i < u_nOffsetTableCnt ; i++)
	{
		char*	pDataMachineID	= NULL;
		char*	pDataEXOFSID 		= NULL;
		int 	Machine_Value 	= 0;
		int 	EXOFS_Value 		= 0;
		int 	Offset_Value 		= 0;
		int		temp 						= 0;
		//取得機器座標
		u_pwndOriginTable[i]->GetPropValueT("value",&Machine_Value,sizeof(Machine_Value));
		
		//取得EXOFS值
		pDataEXOFSID 			= g_pDatabase->GetString(u_EXOFS_StartNo+i);
		double lDBvalue 	= 0;
		lDBvalue	=  GetDBValue(pDataEXOFSID).lValue;
		lDBvalue	=	 lDBvalue*(!Unitindex) + lDBvalue*Unitindex/g_Metric_Imperial_Unit*10;	 
		lDBvalue	+= 0.5*SIGN(lDBvalue);	
		EXOFS_Value = lDBvalue;
		
		//取的定義座標位置
		u_pwndOffsetTable[i]->GetPropValueT("value",&Offset_Value,sizeof(Offset_Value));
		
		//計算工作座標值
		temp = Machine_Value - EXOFS_Value - Offset_Value ;
		u_pwndWorkCoordTable[i]->SetPropValueT("value",temp);
		u_pwndWorkCoordTable[i]->Update();
	}
}

void		SetDB_WorkCoordValue(int NO)
{	
	int u_WorkCoord_StartNo = g_pDatabase->GetIndex("CNC_WORK_COORD1_X");
	for(int i = 0 ; i < u_nWorkCoordTableCnt ; i++)
	{
		double lDBvalue 	= 0; 
		int 	temp = 0;
		char*	pDataID	= NULL;
		u_pwndWorkCoordTable[i]->GetPropValueT("value",&temp,sizeof(temp));
		lDBvalue = temp;
		if(Unitindex && i < 3)
			lDBvalue = lDBvalue*g_Metric_Imperial_Unit/10;
		//printf("lDBvalue is %f\n",lDBvalue);
		pDataID = g_pDatabase->GetString(u_WorkCoord_StartNo + (NO-1)*6 + i );
		SetDBValue(pDataID,(int)lDBvalue);
	}
}