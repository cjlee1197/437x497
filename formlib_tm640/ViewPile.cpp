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
#include	"ViewPile.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		PileNum 4
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL b_RunOnlyOne = FALSE; // �Q�� Update() �Ө�o�� �u����@��

CtmWnd* pwndMask_QTeach_Pile	= NULL; // �B�n �ֳt�о� ���|�ѼƳ]�m
CtmWnd* pwndBtnNextP	= NULL; // �B�n �ֳt�о� ���|�ѼƳ]�m Btn �U�@��m
CtmWnd* pwndBtnLastP	= NULL; // �B�n �ֳt�о� ���|�ѼƳ]�m Btn �W�@��m
CtmWnd* pwndStr_QTeach_Pile	= NULL; // �B�n �ֳt�о� ���|�ѼƳ]�m ��r����

CtmWnd*	u_pwndBtn_Pile_Group[PileNum] = {NULL}; // ��n�� ���| ���s

CtmWnd*	u_pwndBtn_Pile_Dir[PileNum][3] = {NULL}; // �U�b��V ���s
CtmWnd*	u_pwndBtn_Pile_Order[PileNum] = {NULL}; // ���|���� ���s
CtmWnd*	u_pwndImg_Pile[PileNum] = {NULL}; // ��ܹϤ�

BOOL 		b_Dir[PileNum][3] = {FALSE}; // �U�b��V [��n��][��n�b]

int			u_Oder[PileNum] = {0}; // ���|����

char*	u_pszBtnPileGroupName[PileNum] =  // ��n�� ���| Btn�W��
{
	"Pile_1", // �Ĥ@��
	"Pile_2", // �ĤG��
	"Pile_3", // �ĤT��
	"Pile_4", // �ĥ|��
};

char*	u_pszBtnPileDirName[PileNum][3] =  // �U�b��V Btn�W��
{
	{"Btn_Pile1_Dir_X","","Btn_Pile1_Dir_Z",}, //�Ĥ@��
	{"Btn_Pile2_Dir_X","","Btn_Pile2_Dir_Z",}, //�ĤG��
	{"Btn_Pile3_Dir_X","","Btn_Pile3_Dir_Z",}, //�ĤT��
	{"Btn_Pile4_Dir_X","","Btn_Pile4_Dir_Z",}	//�ĥ|��
}; 

char*	u_pszPileDirDB[] =  // �U�b��VDB
{
 "MACHINE_FUNCTION_PILE1_DIR", //�Ĥ@��
 "MACHINE_FUNCTION_PILE2_DIR", //�ĤG��
 "MACHINE_FUNCTION_PILE3_DIR", //�ĤT��
 "MACHINE_FUNCTION_PILE4_DIR", //�ĥ|��
};

char*	Str_PileDir[] = // �U�b��V �r��
{
	"VW_ROTATELOC_ANGELREVERSED_OPT1",
	"VW_ROTATELOC_ANGELREVERSED_OPT2",
};

char*	u_pszBtnPileOrderName[] =  // �U�ն��� Btn�W��
{
	"Btn_Pile1_Order",
	"Btn_Pile2_Order",
	"Btn_Pile3_Order",
	"Btn_Pile4_Order",
};
char*	u_pszBtnPileOrderDB[] =  // �U�ն��� DB
{
	"MACHINE_FUNCTION_PILE1_ORDER", // ���|����_��1��
	"MACHINE_FUNCTION_PILE2_ORDER", // ���|����_��2��
	"MACHINE_FUNCTION_PILE3_ORDER", // ���|����_��3��
	"MACHINE_FUNCTION_PILE4_ORDER", // ���|����_��4��
};

char*	Str_PileOrder[] = // �U�ն��� �r��
{
	"PICKER_PILE_ORDER_1",
	"PICKER_PILE_ORDER_2",
	"PICKER_PILE_ORDER_3",
	"PICKER_PILE_ORDER_4",
	"PICKER_PILE_ORDER_5",
	"PICKER_PILE_ORDER_6",
};


char*	u_pszImgPile[] =  // �U�� ��ܹϤ� �W��
{
	"Img_Pile1",
	"Img_Pile2",
	"Img_Pile3",
	"Img_Pile4",
};
char* PileImgpath[] = // ��ܹϤ�
{																				// 	(bit0 X, bit1 Y, bit3 Z) 0=���V 1=�ϦV
	"res_tm640/pic/picker/Pile_XP_ZP.bmp",// 0( 		0,			0,			0)
	"res_tm640/pic/picker/Pile_XN_ZP.bmp",// 1(			1,			0,			0)
	"",
	"",
	"res_tm640/pic/picker/Pile_XP_ZN.bmp",// 4( 		0,			0,			1)
	"res_tm640/pic/picker/Pile_XN_ZN.bmp",// 5(			1,			0,			1)
};

BOOL TF[] =
{
	FALSE,
	TRUE
};

/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("OnCreateA\n");
		
	// ���o���� �ֳt�о� ���|�ѼƳ]�m �B�n
	pwndMask_QTeach_Pile = pwndSender->FindControlFromName("Mask_QTeach_Pile");
	pwndBtnNextP = pwndSender->FindControlFromName("BtnNextP");
	pwndBtnLastP = pwndSender->FindControlFromName("BtnLastP");
	pwndStr_QTeach_Pile = pwndSender->FindControlFromName("Str_QTeach_Pile");
	
	
	for(int i = 0; i < (PileNum); i++) // ��n�հ��| ���Btn����
	{
		u_pwndBtn_Pile_Group[i] = pwndSender->FindControlFromName(u_pszBtnPileGroupName[i]);
	}
	
	for(int i = 0; i < (PileNum); i++) // �U�b��V ���Btn����
	{
		for(int j=0;j<3;j++)
		{
			u_pwndBtn_Pile_Dir[i][j] = pwndSender->FindControlFromName(u_pszBtnPileDirName[i][j]);
		}
	}
	for(int i = 0; i < (PileNum); i++) // �U�ն��� ���Btn����
	{
		u_pwndBtn_Pile_Order[i] = pwndSender->FindControlFromName(u_pszBtnPileOrderName[i]);
	}
	for(int i = 0; i < (PileNum); i++) // ����U�� ��ܹϤ�����
	{
		u_pwndImg_Pile[i] = pwndSender->FindControlFromName(u_pszImgPile[i]);
	}
	
	//Update string
	for(int i = 0; i < (PileNum); i++) // �U�b��V ��ܤ�r
	{
 		for(int j=0;j<3;j++)
 		{
			if(u_pwndBtn_Pile_Dir[i][j] != NULL)
			{
				int u_PileDir = (int)(GetDBValue(u_pszPileDirDB[i]).lValue);
				printf("u_PileDir[%d][%d]=%d\n",i,j,u_PileDir);
				u_PileDir = ((u_PileDir>>j) & 1); // Ū����j��bit

				b_Dir[i][j]=TF[u_PileDir]; // Ū����V����
				u_pwndBtn_Pile_Dir[i][j]->SetPropValueT("captionID",Str_PileDir[u_PileDir]);
				u_pwndBtn_Pile_Dir[i][j]->CreateA();
				u_pwndBtn_Pile_Dir[i][j]->Update();
			}
		}
	}
	
	for(int i = 0; i < (PileNum); i++) // �U�ն��� ��ܤ�r
	{
		if(u_pwndBtn_Pile_Order[i] != NULL)
		{
			u_Oder[i] = (int)(GetDBValue(u_pszBtnPileOrderDB[i]).lValue);
			u_pwndBtn_Pile_Order[i]->SetPropValueT("captionID",Str_PileOrder[u_Oder[i]]);
			u_pwndBtn_Pile_Order[i]->CreateA();
			u_pwndBtn_Pile_Order[i]->Update();
		}
	}
	// Update Img
	UpdateImg();
	
	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = NULL;	
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(!b_RunOnlyOne)
		{
			Update_QTeach_Pile();			
			b_RunOnlyOne = TRUE;// ���� ����@��
		}
	
}

void    OnDestroyA(CtmWnd* pwndSender)
{
	g_Pile_Select = 0;
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	switch(wKey)
	{
		case 1:
    		::PutCommand("Pile_1.txt");
			break;	
		case 2:
    		::PutCommand("Pile_2.txt");
			break;	
		case 3:
			::PutCommand("Pile_3.txt");
			break;
		case 4:
			::PutCommand("Pile_4.txt");
			break;
		case 99:
			::PutCommand("Func.txt");
			break;
		default:
			break;
	}
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
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
 	
 	for(int i = 0; i < (PileNum); i++)
 	{
 		for(int j=0;j<3;j++)
 		{
 			if(pwnd == u_pwndBtn_Pile_Dir[i][j]) // ���|��V [��n��][XYZ]
 			{
				b_Dir[i][j] = !b_Dir[i][j];
				// (bit0 X, bit1 Y, bit3 Z) 0=���V 1=�ϦV
				int nTemp=0;
				for(int k=0;k<3;k++)
				{
					nTemp = nTemp + (b_Dir[i][k]<<k);
				}
				printf("Pile%d Dir%d=%d\n",i,j,nTemp);
				SetDBValue(u_pszPileDirDB[i],nTemp); // �g�J DB
				printf("Set u_pszPileDirDB[%d]=%d\n",i,nTemp);
				
				//Update string
				
				int u_PileDir = (int)(GetDBValue(u_pszPileDirDB[i]).lValue);
				u_PileDir = ((u_PileDir>>j) & 1); // Ū����j��bit
				u_pwndBtn_Pile_Dir[i][j]->SetPropValueT("captionID",Str_PileDir[u_PileDir]);
				u_pwndBtn_Pile_Dir[i][j]->CreateA();
				u_pwndBtn_Pile_Dir[i][j]->Update();
				
				//Update Img
				UpdateImg();
			}
 		}
 	}
	
	for(int i = 0; i < (PileNum); i++)
	{
		if(pwnd == u_pwndBtn_Pile_Order[i])
			{
				u_Oder[i]++;
				if(u_Oder[i]>5) u_Oder[i]=0;
				SetDBValue(u_pszBtnPileOrderDB[i],u_Oder[i]);
				
				//Update string
				if(u_pwndBtn_Pile_Order[i] != NULL)
				{
					u_Oder[i] = (int)(GetDBValue(u_pszBtnPileOrderDB[i]).lValue);
					u_pwndBtn_Pile_Order[i]->SetPropValueT("captionID",Str_PileOrder[u_Oder[i]]);
					u_pwndBtn_Pile_Order[i]->CreateA();
					u_pwndBtn_Pile_Order[i]->Update();
				}
			}
	}
	
	if(pwnd == pwndBtnNextP) // �U�@�� Btn ���U�u�_
	{
		::PutCommand("QTeach_Done.txt"); // ����� �оɧ�����
	}
	else if(pwnd == pwndBtnLastP) // �W�@�� Btn ���U�u�_
	{
		::PutCommand("QTeach_PlaceP.txt"); // ��^�W�@��
	}
}


void UpdateImg()
{
	printf("UpdateImg\n");
	for(int i = 0; i < (PileNum); i++)
	{
		// Update Img
		if(u_pwndBtn_Pile_Order[i] != NULL)
		{
			int Pile_Dir = (int)(GetDBValue(u_pszPileDirDB[i]).lValue);
			printf("u_pszPileDirDB[%d]=Pile_Dir=%d\n",i,Pile_Dir);
			u_pwndImg_Pile[i]->SetPropValueT("imagepath",PileImgpath[Pile_Dir]);
			u_pwndImg_Pile[i]->Show();
			printf("imagepath=%s\n",PileImgpath[Pile_Dir]);
		}
	}
}

void Update_QTeach_Pile()
{
	pwndMask_QTeach_Pile->Update(); // �B�n
	if(g_Pile_Select>0)
		{
			pwndBtnNextP->SetPropValueT("enabled",double(TRUE));
			pwndBtnLastP->SetPropValueT("enabled",double(TRUE));
			pwndStr_QTeach_Pile->SetPropValueT("enabled",double(TRUE));
			pwndBtnNextP->Update();
			pwndBtnLastP->Update();
			pwndStr_QTeach_Pile->Update();
			for(int i=0;i<PileNum;i++) // ���Ҧ� ���|�� ���s
			{
				if(u_pwndBtn_Pile_Group[i]!=0)
					u_pwndBtn_Pile_Group[i]->SetPropValueT("enabled",double(FALSE));
			}
		}
	else
		{
			pwndBtnNextP->SetPropValueT("enabled",double(FALSE));
			pwndBtnLastP->SetPropValueT("enabled",double(FALSE));
			pwndStr_QTeach_Pile->SetPropValueT("enabled",double(FALSE));
		}	
}