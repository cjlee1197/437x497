/*===========================================================================+
|  Class    : ViewVers library                                               |
|  Task     : ViewVers library action source file                            |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)2.3                                                     |
|  Link     :G++(GCC)2.3                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                         |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewDelay_ADJ.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     CONST_REQ_WRITE         3

#define     CONTROLTYPENUM          13
#define     MAXSTEP               	100

// 流程 區域點
#define		Origin			1 // 原點
#define		WaitP				2 // 待機點
#define		DownWaitP		3 // 下待機點
#define		ReadyDownP	4	// 準備下降點
#define		PickDownP		5	// 取出側下降點
#define		PickP				6	// 取出側取件點
#define		MoveOutP		7	// 橫出點
#define		PlaceDownP	8	// 置物側下降點
#define		PlaceP			9	// 置物側置物點
#define		Homing		 10	// 回原點
// 動作類型
#define		Action_Axis				1  // 軸動作
#define		Action_Wait				2  // 等待
#define		Action_Permit			3  // 允許

#define		Action_Valve			4	 // 閥門
#define		Action_Tag				5  // 標籤
#define		Action_Goto				6  // 跳轉
#define		Action_Detect			8  // 檢測
#define		Action_Pile				12 // 堆疊
#define		Action_Sub				13 // 副程式
// 軸數
#define		Axis_X1				1  // X1軸
#define		Axis_Y1				2  // Y1軸
#define		Axis_Z				3  // Z軸
#define		Axis_X2				4  // X2軸
#define		Axis_Y2				5  // Y2軸
#define		Axis_C				6  // C軸
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
char*	FWD_DT_DB=NULL;
char*	BWD_DT_DB=NULL;


CtmWnd*		pwndMARM_FWD_DT	= NULL; // 主臂前進延時 數值設定
CtmWnd*		pwndMARM_BWD_DT	= NULL; // 主臂後退延時 數值設定


DT_ADJ DT_ADJ_DEF[]= // 各延時點 定義 列表
{
// Step ACTION_TYPE,	ACTION_NUM,		P1,	P2, P3, P4, 			P5,
	{0, 	Action_Axis,	Axis_X1,			0,	0,	0,	PickP,		0, }, // 主臂前進 延時
	{0, 	Action_Axis,	Axis_X1,			0,	0,	0,	MoveOutP,	0, }, // 主臂後退 延時
	{0, 	Action_Valve,				0,			0,	0,	0,	PickP,		0, }, // 產品夾具延時
//	{0, 	Action_Permit,	0,			0,	0,	0,	MoveOutP,	0, }, // 頂針延時
	{0, 	Action_Axis,	Axis_Y1,			0,	0,	0,	MoveOutP,	0, }, // 主臂上升延時
};

/*===========================================================================+
|           Function                                                         |
+===========================================================================*/
void	OnDestroyA(CtmWnd* pwndSender)
{
	if(FWD_DT_DB != NULL)
	    free(FWD_DT_DB);
	FWD_DT_DB = NULL;
	if(BWD_DT_DB != NULL)
	    free(BWD_DT_DB);
	BWD_DT_DB = NULL;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	
	return wIDControl;
}

BOOL	OnCreateA(CtmWnd* pwndSender)
{
	// 取得指標 設定完成 Btn
	pwndMARM_FWD_DT = pwndSender->FindControlFromName("MARM_FWD_DT");
	// 取得指標 設定完成 Btn
	pwndMARM_BWD_DT = pwndSender->FindControlFromName("MARM_BWD_DT");
	
	Get_DT_DB();
    
  return TRUE;
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA\n");
	long l_DT;
	
  if(pwndMARM_BWD_DT!=NULL)
  	{
  		pwndMARM_BWD_DT->GetPropValueT("value", &l_DT,sizeof(l_DT));
  		SetDBValue(BWD_DT_DB,l_DT*10); // 數值寫入DB
  		printf("Set %s =%d\n",BWD_DT_DB,l_DT);
  		g_ptaskpicker->WriteValue(CONST_REQ_WRITE, 1 ,&BWD_DT_DB);
  	}
  if(pwndMARM_FWD_DT!=NULL)
  	{
  		pwndMARM_FWD_DT->GetPropValueT("value", &l_DT,sizeof(l_DT));
  		SetDBValue(FWD_DT_DB,l_DT*10); // 數值寫入DB
  		printf("Set %s =%d\n",FWD_DT_DB,l_DT);
  		g_ptaskpicker->WriteValue(CONST_REQ_WRITE, 1 ,&FWD_DT_DB);
  	}
		
	      
	      

	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else                              return _NULL_KEY;
}


void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{

}

void	OnUpdateA(CtmWnd* pwndSender)
{

}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function : GetDB()                  																			 |
|       		: 取得延時的DB 名稱																							 |
+---------------------------------------------------------------------------*/
void	Get_DT_DB()
{
	char ACTION_TYPE_DBID[256]; // TYPE DB
	char ACTION_NUM_DBID[256];	// NUM DB
	char GROUP_DBID[256];			 	// PARAMETER 4 DB
	
	int Action_TYPE =0; // TYPE
	int Action_NUM =0;	// NUM
	int Group =0;			 	// PARAMETER 4
	int value=0;
	
	for(int i =0;i<100;i++)
	{
		memset(ACTION_TYPE_DBID, 0 ,sizeof(ACTION_TYPE_DBID));
		sprintf(ACTION_TYPE_DBID,"MACHINE_PROFILE_NUM%d_ACTION_TYPE",i+1);
		Action_TYPE = GetDBValue(ACTION_TYPE_DBID).lValue;
		
		memset(ACTION_NUM_DBID, 0 ,sizeof(ACTION_NUM_DBID));
		sprintf(ACTION_NUM_DBID,"MACHINE_PROFILE_NUM%d_ACTION_NUM",i+1);
		Action_NUM = GetDBValue(ACTION_NUM_DBID).lValue;
		
		memset(GROUP_DBID, 0 ,sizeof(GROUP_DBID));
		sprintf(GROUP_DBID,"MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4",i+1);
		Group = GetDBValue(GROUP_DBID).lValue;

		if( (Action_TYPE==1) && (Action_NUM==1) && (Group==6)) // ACTION_TYPE==軸動作 && ACTION_NUM==Axis_X1 && Group==6
		{
			printf("Get FWD MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4\n",i+1);
      FWD_DT_DB = (char*)malloc(256);
      sprintf(FWD_DT_DB, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1);
      value=GetDBValue(FWD_DT_DB).lValue; // 讀取DB數值
      printf("%s=%d\n",FWD_DT_DB,value);
      if(pwndMARM_FWD_DT!=NULL)
      	{
	      	pwndMARM_FWD_DT->SetPropValueT("value",value/10);
	      	pwndMARM_FWD_DT->CreateA();
	      	pwndMARM_FWD_DT->Show();
	      }
		}



		if( (Action_TYPE==1) && (Action_NUM==1) && (Group==7)) // ACTION_TYPE==軸動作 && ACTION_NUM==Axis_X1 && Group==7
		{
			printf("Get BWD MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4\n",i+1);
      BWD_DT_DB = (char*)malloc(256);
			sprintf(BWD_DT_DB, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1);
			value=GetDBValue(BWD_DT_DB).lValue; // 讀取DB數值
			printf("%s=%d\n",BWD_DT_DB,value);
      if(pwndMARM_BWD_DT!=NULL)
      	{
	      	pwndMARM_BWD_DT->SetPropValueT("value",value/10);
	      	pwndMARM_BWD_DT->CreateA();
	      	pwndMARM_BWD_DT->Show();
	      }
		}
		
	}	
}