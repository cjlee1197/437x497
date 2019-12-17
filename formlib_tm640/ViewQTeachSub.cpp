/*===========================================================================+
|  Class    : ViewCore library                                         		 	 |
|  Task     : ViewCore library action source file                       		 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                            	         |
|  Version  : V1.00                                                          |
|  Creation : 			                                                 			   |
|  Revision :           			                                      			   |
+===========================================================================*/
#include	"ViewQTeachSub.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
// 瑈祘 跋办翴
#define		Origin			1 // 翴
#define		WaitP				2 // 诀翴
#define		DownWaitP		3 // 诀翴
#define		ReadyDownP	4	// 非称翴
#define		PickDownP		5	// 凹翴
#define		PickP				6	// 凹ン翴
#define		MoveOutP		7	// 绢翴
#define		PlaceDownP	8	// 竚凹翴
#define		PlaceP			9	// 竚凹竚翴
#define		Homing		 10	// 翴

// 禸计
#define		Axis_X1				1  // X1禸
#define		Axis_Y1				2  // Y1禸
#define		Axis_Z				3  // Z禸
#define		Axis_X2				4  // X2禸
#define		Axis_Y2				5  // Y2禸
#define		Axis_C				6  // C禸

#define		OK					1  //

#define		Color_White		0xFFFF
#define		Color_Red			0xF800
#define		Color_Yellow 	0xFF80
#define		Color_LBlue		0xDFBF
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
int		AxisXOld =0,AxisYOld =0,AxisZOld =0,AxisX2Old =0,AxisY2Old =0,AxisCOld=0,
AxisXNew =0,AxisYNew =0,AxisX2New =0,AxisY2New =0,AxisZNew =0;// 盎代HotKey禸
int GetAxisPos=0; //眔禸

BOOL b_BtnNextP_OK = 0; // ЧΘ毙旧 竚
BOOL b_PosSet_OK[10] = {0}; // ЧΘ毙旧 砞﹚竚
BOOL b_All_PosSet_OK = 0; //ЧΘ毙旧 场砞﹚竚 砞﹚ЧΘ
char* pAll_PosSet_OK_DB	 = "MACHINE_PROFILE_NUM7_EQUIPMENT2_ACTION_NUM"; // 竚砞竚ЧΘ DB

CtmWnd*		pwndBtnNextP	= NULL; // 竚秙

CtmWnd*		pwndBtn_GetPos[6]	= {NULL}; // 莉竚秙
char* GetPos_String[] = // 眔竚Btn 嘿
{
	"",
	"Btn_GetPos_X1",
	"Btn_GetPos_Y1",
	"Btn_GetPos_Z",
	"Btn_GetPos_X2",
	"Btn_GetPos_Y2",
};

char* AxisPosNow_DBString[] = // 魁瞷竚 DB嘿
{
	"",
	"MACHINE_INTERFACE_CONTROL_XAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_YAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_ZAXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_X2AXISPOSITION",
	"MACHINE_INTERFACE_CONTROL_Y2AXISPOSITION",
};


CtmWnd*   pwndQTeach_Pos[6] ={NULL}; // е硉毙旧 竚砞﹚
char* u_pszQTeach_PosString[] =
{
	"",
	"QTeach_Pos_X1",
	"QTeach_Pos_Y1",
	"QTeach_Pos_Z",
	"QTeach_Pos_X2",
	"QTeach_Pos_Y2",
};

CtmWnd*   pwndQTeach_Speed[6] ={NULL}; // е硉毙旧 硉砞﹚
char* u_pszQTeach_SpeedString[] =
{
	"",
	"QTeach_Speed_X1",
	"QTeach_Speed_Y1",
	"QTeach_Speed_Z",
	"QTeach_Speed_X2",
	"QTeach_Speed_Y2",
};

CtmWnd*   pwndQTeach_DT[6] ={NULL}; // е硉毙旧 ┑砞﹚ DT-DelatTime
char* u_pszQTeach_DTString[] =
{
	"",
	"QTeach_DT_X1",
	"QTeach_DT_Y1",
	"QTeach_DT_Z",
	"QTeach_DT_X2",
	"QTeach_DT_Y2",
};


/*============================竚  砞﹚============================*/

int QTeach_PGNo = 0; // е硉毙旧 ˙艼腹絏
char* SubStartNo_DB = "MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_TYPE"; // 竚 秨﹍腹 DB

long l_Position[6] = {0};
char* Sub_POSSET_DBString[] = // 竚翴 竚砞﹚ DB嘿
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER1", // Z
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER2", // X2
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER3", // Y2
};
long l_Speed[6] = {0};
char* Sub_Speed_DBString[] = // 竚翴 硉砞﹚ DB嘿
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER4", // Z
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER5", // X2
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER6", // Y2
};
long l_Delaytime[6] = {0};
char* Sub_DT_DBString[] = // 竚翴 ┑砞﹚ DB嘿
{
	"",
	"",
	"",
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER7", // Z
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER8", // X2
	"MACHINE_PROFILE_NUM36_EQUIPMENT2_ACTION_PARAMETER9", // Y2
};


char* P7_POSSET_DBString[] = // P7 凹绢翴 竚砞﹚ DB嘿
{
	"",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER1",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER2",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER3",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER4",
	"MACHINE_PROFILE_NUM46_EQUIPMENT2_ACTION_PARAMETER5",
};

/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	// 眔夹 竚秙
	pwndBtnNextP	= pwndSender->FindControlFromName("BtnNextP");	
	
	// 眔夹 莉竚秙
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ )
	{
		pwndBtn_GetPos[i] = pwndSender->FindControlFromName(GetPos_String[i]);
	} 
	
	// 眔夹 畒夹竚砞﹚
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		pwndQTeach_Pos[i] = pwndSender->FindControlFromName(u_pszQTeach_PosString[i]);
	} 
	// 眔夹 硉砞﹚
	for(int i = 0; i < sizeof(u_pszQTeach_SpeedString)/sizeof(u_pszQTeach_SpeedString[0]); i++ )
	{
		pwndQTeach_Speed[i] = pwndSender->FindControlFromName(u_pszQTeach_SpeedString[i]);
	}
	// 眔夹 ┑砞﹚
	for(int i = 0; i < sizeof(u_pszQTeach_DTString)/sizeof(u_pszQTeach_DTString[0]); i++ )
	{
		pwndQTeach_DT[i] = pwndSender->FindControlFromName(u_pszQTeach_DTString[i]);
	}	
	
	QTeach_PGNo = GetDBValue(SubStartNo_DB).lValue; // 眔 竚 秨﹍腹
	printf("SubPage Get QTeach_PGNo=%d\n",QTeach_PGNo); 
	
	// 魁ヘ玡禸篈 ノ 盎代HotKey禸
	AxisXOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0001;
	AxisYOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0002;
	AxisZOld = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0004;
	AxisX2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0001;
	AxisY2Old = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0002;
	
	UpdateDataValue();
	UpdateTeach_Pos();
	UpdateBtnNextP();
	
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{              
	printf("OnChange\n");
	//taborder 5-9 琌砞﹚竚
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl); // 莉taborderじン
	if(pwnd!=NULL)
	{
		switch(wIDControl)
		{
			case 5:
				GetAxisPos = Axis_Z;
				b_PosSet_OK[Axis_Z]=OK;
				break;
			case 8:
				GetAxisPos = Axis_X2;
				b_PosSet_OK[Axis_X2]=OK;
				break;
			case 9:
				GetAxisPos = Axis_Y2;
				b_PosSet_OK[Axis_Y2]=OK;
				break;
			default:
				break;
		}
		UpdateTeach_Pos();
		UpdateBtnNextP();
	}
	return wIDControl;
}
/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateA(CtmWnd* pwndSender)
{
	AxisXNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0001;
	AxisYNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0002;
	AxisZNew = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED43").lValue & 0x0004;
	AxisX2New = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0001;
	AxisY2New = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED44").lValue & 0x0002;
	
	if(AxisXNew != AxisXOld)	//X1禸 砆
	{
		printf("AxisXNew\n");
		if(AxisXNew)
		{
			GetAxisPos = Axis_X1;
			pwndQTeach_Pos[Axis_X1]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_X1]->Update();
		}
		AxisXOld = AxisXNew;
	}
	else if(AxisYNew != AxisYOld)	//Y1禸 砆
	{
		printf("AxisYNew\n");
		if(AxisYNew)
		{
			GetAxisPos = Axis_Y1;
			pwndQTeach_Pos[Axis_Y1]->SetPropValueT("bgc",0xFF80); // 独︹
			pwndQTeach_Pos[Axis_Y1]->Update();
		}
		AxisYOld = AxisYNew;
	}
	else if(AxisZNew != AxisZOld)	//Z禸 砆
	{
		printf("AxisZNew\n");
		if(AxisZNew)
		{
			GetAxisPos = Axis_Z;
			pwndQTeach_Pos[Axis_Z]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_Z]->Update();
		}
		AxisZOld = AxisZNew;
	}
	else if(AxisX2New != AxisX2Old)	//X2禸 砆
	{
		printf("AxisX2New\n");
		if(AxisX2New)
		{
			GetAxisPos = Axis_X2;
			pwndQTeach_Pos[Axis_X2]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_X2]->Update();
		}
		AxisX2Old = AxisX2New;
	}
	else if(AxisY2New != AxisY2Old)	//Y2禸 砆
	{
		printf("AxisY2New\n");
		if(AxisY2New)
		{
			GetAxisPos = Axis_Y2;
			pwndQTeach_Pos[Axis_Y2]->SetPropValueT("bgc",0xFF80);
			pwndQTeach_Pos[Axis_Y2]->Update();
		}
		AxisY2Old = AxisY2New;
	}
}

void    OnDestroyA(CtmWnd* pwndSender)
{
	
}

/*---------------------------------------------------------------------------+
|  Function : OnKeyA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA=%d\n",wKey);
	switch(wKey)
	{
		case MoveOutP:
			::PutCommand("QTeach_MoveOutP.txt");
			break;
		case PlaceDownP:
				if(b_BtnNextP_OK) // ┕ ┪琌 砞﹚ЧΘ┕
				{
					// ㄌ酚ㄏノ砞﹚糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
					QTeach_PGNo++;
					// Z 绢
					{
						if(pwndQTeach_Pos[Axis_Z]!=NULL) // 砞﹚竚 P1竚
						{
							pwndQTeach_Pos[Axis_Z]->GetPropValueT("value", &l_Position[Axis_Z],sizeof(l_Position[Axis_Z]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Z]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_Z]);
							SetDBValue(Sub_POSSET_DBString[Axis_Z], l_Position[Axis_Z]); // 糶 WaitP db
						}
						if(pwndQTeach_Speed[Axis_Z]!=NULL) // 砞﹚硉 P2硉
						{
							pwndQTeach_Speed[Axis_Z]->GetPropValueT("value", &l_Speed[Axis_Z],sizeof(l_Speed[Axis_Z]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Z]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_Speed_DBString[Axis_Z], l_Speed[Axis_Z]); // 糶 WaitP db
						}
						if(pwndQTeach_DT[Axis_Z]!=NULL) // 砞﹚┑ P5┑
						{
							pwndQTeach_DT[Axis_Z]->GetPropValueT("value", &l_Delaytime[Axis_Z],sizeof(l_Delaytime[Axis_Z]));
							if(l_Delaytime[Axis_Z]!=0) // 0ぃノ
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Z]*10; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_DT_DBString[Axis_Z], l_Delaytime[Axis_Z]); // 糶 WaitP db
						}	
					}
					QTeach_PGNo++;
					// X2 玡秈
					{
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 砞﹚竚 P1竚
						{
							pwndQTeach_Pos[Axis_X2]->GetPropValueT("value", &l_Position[Axis_X2],sizeof(l_Position[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_X2]);
							SetDBValue(Sub_POSSET_DBString[Axis_X2], l_Position[Axis_X2]); // 糶 WaitP db
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // 砞﹚硉 P2硉
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_Speed_DBString[Axis_X2], l_Speed[Axis_X2]); // 糶 WaitP db
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 砞﹚┑ P5┑
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0ぃノ
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_DT_DBString[Axis_X2], l_Delaytime[Axis_X2]); // 糶 WaitP db
						}	
					}
					QTeach_PGNo++;
					// Y2  (メ)
					{
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // 砞﹚竚 P1竚
						{
							pwndQTeach_Pos[Axis_Y2]->GetPropValueT("value", &l_Position[Axis_Y2],sizeof(l_Position[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_Y2]);
							SetDBValue(Sub_POSSET_DBString[Axis_Y2], l_Position[Axis_Y2]); // 糶 WaitP db
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // 砞﹚硉 P2硉
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_Speed_DBString[Axis_Y2], l_Speed[Axis_Y2]); // 糶 WaitP db
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // 砞﹚┑ P5┑
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0ぃノ
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							SetDBValue(Sub_DT_DBString[Axis_Y2], l_Delaytime[Axis_Y2]); // 糶 WaitP db
						}	
					}			
					QTeach_PGNo++;
					// Y2 ど 
					{
						if(pwndQTeach_Pos[Axis_Y2]!=NULL) // 砞﹚竚 P1竚
						{
							l_Position[Axis_Y2] = GetDBValue(P7_POSSET_DBString[Axis_Y2]).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_Y2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_Y2]);
						}
						if(pwndQTeach_Speed[Axis_Y2]!=NULL) // 砞﹚硉 P2硉
						{
							pwndQTeach_Speed[Axis_Y2]->GetPropValueT("value", &l_Speed[Axis_Y2],sizeof(l_Speed[Axis_Y2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_Y2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
						}
						if(pwndQTeach_DT[Axis_Y2]!=NULL) // 砞﹚┑ P5┑
						{
							pwndQTeach_DT[Axis_Y2]->GetPropValueT("value", &l_Delaytime[Axis_Y2],sizeof(l_Delaytime[Axis_Y2]));
							if(l_Delaytime[Axis_Y2]!=0) // 0ぃノ
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_Y2]*10; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
						}	
					}	
					QTeach_PGNo++;
					// X2 癶 
					{
						if(pwndQTeach_Pos[Axis_X2]!=NULL) // 砞﹚竚 P1竚
						{
							l_Position[Axis_X2] = GetDBValue(P7_POSSET_DBString[Axis_X2]).lValue;
							g_QTeach_Action_P[QTeach_PGNo-1].P1 = l_Position[Axis_X2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
							printf("Set g_QTeach_Action_P[%d].P1=%d\n",QTeach_PGNo-1,l_Position[Axis_X2]);
						}
						if(pwndQTeach_Speed[Axis_X2]!=NULL) // 砞﹚硉 P2硉
						{
							pwndQTeach_Speed[Axis_X2]->GetPropValueT("value", &l_Speed[Axis_X2],sizeof(l_Speed[Axis_X2]));
							g_QTeach_Action_P[QTeach_PGNo-1].P2 = l_Speed[Axis_X2]; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
						}
						if(pwndQTeach_DT[Axis_X2]!=NULL) // 砞﹚┑ P5┑
						{
							pwndQTeach_DT[Axis_X2]->GetPropValueT("value", &l_Delaytime[Axis_X2],sizeof(l_Delaytime[Axis_X2]));
							if(l_Delaytime[Axis_X2]!=0) // 0ぃノ
								g_QTeach_Action_P[QTeach_PGNo-1].P5 = l_Delaytime[Axis_X2]*10; // 糶把计计 g_QTeach_Action_P[QTeach_PGNo-1]
						}	
					}
					::PutCommand("QTeach_PlaceDownP.txt");
				}
				else
					MsgBox(g_MultiLanguage["TEACH_QTEACH_NOTYET"], tmFT_CODE_TECH); // 矗ボ
			break;
		default:
			break;
	}
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);	
	return wKey;
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
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 		
	for(int i = 0; i < sizeof(GetPos_String)/sizeof(GetPos_String[0]); i++ ) // 莉竚 Btn紆癬
	{
		if(pwnd == pwndBtn_GetPos[i])
		{
			GetPosNow(i);
			b_PosSet_OK[i]=OK;
			UpdateTeach_Pos();
			UpdateBtnNextP();
		}
	} 
	
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
|  Function : UpdateDataValue()                  						   				   		 |
|       		: 穝砞﹚竚砞﹚硉计																		 |
+---------------------------------------------------------------------------*/
void	UpdateDataValue()
{
	printf("Update data\n");
	long AxisSetPos[6] = {0};
	long AxisSetSpeed[6] = {0};
	long AxisSetDT[6] = {0};
	
	for(int WhichAxis=0; WhichAxis<6; WhichAxis++)
	{
		if(pwndQTeach_Pos[WhichAxis] != NULL) // Set Pos
		{
			// Get data Pos value from db
			char	SetPos_DBID[256];	memset(SetPos_DBID, 0, sizeof(SetPos_DBID));
			pwndQTeach_Pos[WhichAxis]->GetPropValueT("dbid1", SetPos_DBID, sizeof(SetPos_DBID));
			AxisSetPos[WhichAxis] = GetDBValue(SetPos_DBID).lValue;
			//printf("Get %s = %d\n",SetPos_DBID,AxisSetPos[WhichAxis]);
			// Update show data
			pwndQTeach_Pos[WhichAxis]->SetPropValueT("value",AxisSetPos[WhichAxis]);
			pwndQTeach_Pos[WhichAxis]->Update();
			pwndQTeach_Pos[WhichAxis]->UpdateAll();
			//printf("Set Axis%d = %d\n",WhichAxis,AxisSetPos[WhichAxis]);
		}
		
		if(pwndQTeach_Speed[WhichAxis] != NULL) // Set Speed
		{
			// Get data Speed value from db
			char	SetSpeed_DBID[256];	memset(SetSpeed_DBID, 0, sizeof(SetSpeed_DBID));
			pwndQTeach_Speed[WhichAxis]->GetPropValueT("dbid1", SetSpeed_DBID, sizeof(SetSpeed_DBID));
			AxisSetSpeed[WhichAxis] = GetDBValue(SetSpeed_DBID).lValue;
			//printf("Get %s = %d\n",SetSpeed_DBID,AxisSetSpeed[WhichAxis]);
			// Update show data
			pwndQTeach_Speed[WhichAxis]->SetPropValueT("value",AxisSetSpeed[WhichAxis]);
			pwndQTeach_Speed[WhichAxis]->Update();
			pwndQTeach_Speed[WhichAxis]->UpdateAll();
			//printf("Set Axis%d Speed = %d\n",WhichAxis,AxisSetSpeed[WhichAxis]);
		}
		
		if(pwndQTeach_DT[WhichAxis] != NULL) // Set DelayTime
		{
			// Get data DelayTime value from db
			char	SetDT_DBID[256];	memset(SetDT_DBID, 0, sizeof(SetDT_DBID));
			pwndQTeach_DT[WhichAxis]->GetPropValueT("dbid1", SetDT_DBID, sizeof(SetDT_DBID));
			AxisSetDT[WhichAxis] = GetDBValue(SetDT_DBID).lValue;
			// Update show data
			pwndQTeach_DT[WhichAxis]->SetPropValueT("value",AxisSetDT[WhichAxis]);
			pwndQTeach_DT[WhichAxis]->Update();
			pwndQTeach_DT[WhichAxis]->UpdateAll();
		}
		
	}
}

/*---------------------------------------------------------------------------+
|  Function : GetPosNow(int GetAxisPos)                  				   		       |
|       		: 莉讽玡 GetAxisPos 禸竚																		 |
+---------------------------------------------------------------------------*/
void	GetPosNow(int GetAxisPos)
{
	printf("Get Pos %d\n",GetAxisPos);

	if( (GetAxisPos==Axis_Z) || (GetAxisPos==Axis_X2) || (GetAxisPos==Axis_Y2) )
	{
		long AxisPosNow = GetDBValue(AxisPosNow_DBString[GetAxisPos]).lValue; // 眔计
		printf("Get %s = %d\n",u_pszQTeach_PosString[GetAxisPos],AxisPosNow);
		if(pwndQTeach_Pos[GetAxisPos] != NULL)
		{
			printf("Set Axis%d = %d\n",GetAxisPos,AxisPosNow);
			pwndQTeach_Pos[GetAxisPos]->OnLoseFocus();
			pwndQTeach_Pos[GetAxisPos]->SetPropValueT("value",AxisPosNow); // 糶计
			pwndQTeach_Pos[GetAxisPos]->SetPropValueT("bgc",0xFFFF);
			pwndQTeach_Pos[GetAxisPos]->CreateA();
			pwndQTeach_Pos[GetAxisPos]->Show();
		}
		UpdateTeach_Pos();
		UpdateBtnNextP();
	}	
}

/*---------------------------------------------------------------------------+
|  Function : UpdateTeach_Pos()     	             										  		 |
|       		: 穝矗ボ 竚砞﹚																						 |
+---------------------------------------------------------------------------*/
void	UpdateTeach_Pos()
{
	printf("UpdateTeach_Pos()\n");
	int u_All_PosSet_OK=0;
	for(int i = 0; i < sizeof(u_pszQTeach_PosString)/sizeof(u_pszQTeach_PosString[0]); i++ )
	{
		if(pwndQTeach_Pos[i]!=NULL)
		{
			if(b_All_PosSet_OK==1)
				b_PosSet_OK[i]=OK;
			if(b_PosSet_OK[i]==OK)
			{
				u_All_PosSet_OK++;
				pwndQTeach_Pos[i]->SetPropValueT("bgc",Color_LBlue); // フ︹
			}
			else
			{
				pwndQTeach_Pos[i]->SetPropValueT("bgc",Color_Yellow); // 独︹
			}
			//pwndQTeach_Pos[i]->CreateA();
			pwndQTeach_Pos[i]->Show();
			pwndQTeach_Pos[i]->OnLoseFocus();
		}
	} 
	if(u_All_PosSet_OK>=3)
		b_All_PosSet_OK =OK; //场竚砞﹚ЧΘ
}

/*---------------------------------------------------------------------------+
|  Function : UpdateBtnNextP()                  										  		   |
|       		: 穝矗ボ 竚																							 |
+---------------------------------------------------------------------------*/
void	UpdateBtnNextP()
{
	printf("UpdateBtnNextP()\n");
	if(pwndBtnNextP!=NULL)
	{
		if(b_All_PosSet_OK==OK) // 竚 && 獀ㄣ && 硉 砞﹚ЧΘ &&(b_All_SpdSet_OK==OK)
			{
				SetDBValue(pAll_PosSet_OK_DB, 1); // 糶 竚砞﹚ЧΘ db
				b_BtnNextP_OK = OK;
				pwndBtnNextP->SetPropValueT("upbitmap","res_tm640/pic/PGBtnSelect.bmp");
			}
		else
			{
				b_BtnNextP_OK = 0;
				pwndBtnNextP->SetPropValueT("upbitmap","res_tm640/pic/PGBtnUp.bmp");
			}
		pwndBtnNextP->CreateA();
		pwndBtnNextP->Update();
	}
}