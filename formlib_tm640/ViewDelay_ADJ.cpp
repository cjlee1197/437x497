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

// �y�{ �ϰ��I
#define		Origin			1 // ���I
#define		WaitP				2 // �ݾ��I
#define		DownWaitP		3 // �U�ݾ��I
#define		ReadyDownP	4	// �ǳƤU���I
#define		PickDownP		5	// ���X���U���I
#define		PickP				6	// ���X�������I
#define		MoveOutP		7	// ��X�I
#define		PlaceDownP	8	// �m�����U���I
#define		PlaceP			9	// �m�����m���I
#define		Homing		 10	// �^���I
// �ʧ@����
#define		Action_Axis				1  // �b�ʧ@
#define		Action_Wait				2  // ����
#define		Action_Permit			3  // ���\

#define		Action_Valve			4	 // �֪�
#define		Action_Tag				5  // ����
#define		Action_Goto				6  // ����
#define		Action_Detect			8  // �˴�
#define		Action_Pile				12 // ���|
#define		Action_Sub				13 // �Ƶ{��
// �b��
#define		Axis_X1				1  // X1�b
#define		Axis_Y1				2  // Y1�b
#define		Axis_Z				3  // Z�b
#define		Axis_X2				4  // X2�b
#define		Axis_Y2				5  // Y2�b
#define		Axis_C				6  // C�b
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
char*	FWD_DT_DB=NULL;
char*	BWD_DT_DB=NULL;


CtmWnd*		pwndMARM_FWD_DT	= NULL; // �D�u�e�i���� �ƭȳ]�w
CtmWnd*		pwndMARM_BWD_DT	= NULL; // �D�u��h���� �ƭȳ]�w


DT_ADJ DT_ADJ_DEF[]= // �U�����I �w�q �C��
{
// Step ACTION_TYPE,	ACTION_NUM,		P1,	P2, P3, P4, 			P5,
	{0, 	Action_Axis,	Axis_X1,			0,	0,	0,	PickP,		0, }, // �D�u�e�i ����
	{0, 	Action_Axis,	Axis_X1,			0,	0,	0,	MoveOutP,	0, }, // �D�u��h ����
	{0, 	Action_Valve,				0,			0,	0,	0,	PickP,		0, }, // ���~���㩵��
//	{0, 	Action_Permit,	0,			0,	0,	0,	MoveOutP,	0, }, // ���w����
	{0, 	Action_Axis,	Axis_Y1,			0,	0,	0,	MoveOutP,	0, }, // �D�u�W�ɩ���
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
	// ���o���� �]�w���� Btn
	pwndMARM_FWD_DT = pwndSender->FindControlFromName("MARM_FWD_DT");
	// ���o���� �]�w���� Btn
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
  		SetDBValue(BWD_DT_DB,l_DT*10); // �ƭȼg�JDB
  		printf("Set %s =%d\n",BWD_DT_DB,l_DT);
  		g_ptaskpicker->WriteValue(CONST_REQ_WRITE, 1 ,&BWD_DT_DB);
  	}
  if(pwndMARM_FWD_DT!=NULL)
  	{
  		pwndMARM_FWD_DT->GetPropValueT("value", &l_DT,sizeof(l_DT));
  		SetDBValue(FWD_DT_DB,l_DT*10); // �ƭȼg�JDB
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
|       		: ���o���ɪ�DB �W��																							 |
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

		if( (Action_TYPE==1) && (Action_NUM==1) && (Group==6)) // ACTION_TYPE==�b�ʧ@ && ACTION_NUM==Axis_X1 && Group==6
		{
			printf("Get FWD MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4\n",i+1);
      FWD_DT_DB = (char*)malloc(256);
      sprintf(FWD_DT_DB, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1);
      value=GetDBValue(FWD_DT_DB).lValue; // Ū��DB�ƭ�
      printf("%s=%d\n",FWD_DT_DB,value);
      if(pwndMARM_FWD_DT!=NULL)
      	{
	      	pwndMARM_FWD_DT->SetPropValueT("value",value/10);
	      	pwndMARM_FWD_DT->CreateA();
	      	pwndMARM_FWD_DT->Show();
	      }
		}



		if( (Action_TYPE==1) && (Action_NUM==1) && (Group==7)) // ACTION_TYPE==�b�ʧ@ && ACTION_NUM==Axis_X1 && Group==7
		{
			printf("Get BWD MACHINE_PROFILE_NUM%d_ACTION_PARAMETER4\n",i+1);
      BWD_DT_DB = (char*)malloc(256);
			sprintf(BWD_DT_DB, "MACHINE_PROFILE_NUM%d_ACTION_PARAMETER5", i+1);
			value=GetDBValue(BWD_DT_DB).lValue; // Ū��DB�ƭ�
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