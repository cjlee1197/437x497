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
BOOL b_RunOnlyOne = FALSE; // 利用 Update() 來到這頁 只執行一次

CtmWnd* pwndMask_QTeach_Pile	= NULL; // 遮罩 快速教導 堆疊參數設置
CtmWnd* pwndBtnNextP	= NULL; // 遮罩 快速教導 堆疊參數設置 Btn 下一位置
CtmWnd* pwndBtnLastP	= NULL; // 遮罩 快速教導 堆疊參數設置 Btn 上一位置
CtmWnd* pwndStr_QTeach_Pile	= NULL; // 遮罩 快速教導 堆疊參數設置 文字說明

CtmWnd*	u_pwndBtn_Pile_Group[PileNum] = {NULL}; // 第n組 堆疊 按鈕

CtmWnd*	u_pwndBtn_Pile_Dir[PileNum][3] = {NULL}; // 各軸方向 按鈕
CtmWnd*	u_pwndBtn_Pile_Order[PileNum] = {NULL}; // 堆疊順序 按鈕
CtmWnd*	u_pwndImg_Pile[PileNum] = {NULL}; // 顯示圖片

BOOL 		b_Dir[PileNum][3] = {FALSE}; // 各軸方向 [第n組][第n軸]

int			u_Oder[PileNum] = {0}; // 堆疊順序

char*	u_pszBtnPileGroupName[PileNum] =  // 第n組 堆疊 Btn名稱
{
	"Pile_1", // 第一組
	"Pile_2", // 第二組
	"Pile_3", // 第三組
	"Pile_4", // 第四組
};

char*	u_pszBtnPileDirName[PileNum][3] =  // 各軸方向 Btn名稱
{
	{"Btn_Pile1_Dir_X","","Btn_Pile1_Dir_Z",}, //第一組
	{"Btn_Pile2_Dir_X","","Btn_Pile2_Dir_Z",}, //第二組
	{"Btn_Pile3_Dir_X","","Btn_Pile3_Dir_Z",}, //第三組
	{"Btn_Pile4_Dir_X","","Btn_Pile4_Dir_Z",}	//第四組
}; 

char*	u_pszPileDirDB[] =  // 各軸方向DB
{
 "MACHINE_FUNCTION_PILE1_DIR", //第一組
 "MACHINE_FUNCTION_PILE2_DIR", //第二組
 "MACHINE_FUNCTION_PILE3_DIR", //第三組
 "MACHINE_FUNCTION_PILE4_DIR", //第四組
};

char*	Str_PileDir[] = // 各軸方向 字串
{
	"VW_ROTATELOC_ANGELREVERSED_OPT1",
	"VW_ROTATELOC_ANGELREVERSED_OPT2",
};

char*	u_pszBtnPileOrderName[] =  // 各組順序 Btn名稱
{
	"Btn_Pile1_Order",
	"Btn_Pile2_Order",
	"Btn_Pile3_Order",
	"Btn_Pile4_Order",
};
char*	u_pszBtnPileOrderDB[] =  // 各組順序 DB
{
	"MACHINE_FUNCTION_PILE1_ORDER", // 堆疊順序_第1組
	"MACHINE_FUNCTION_PILE2_ORDER", // 堆疊順序_第2組
	"MACHINE_FUNCTION_PILE3_ORDER", // 堆疊順序_第3組
	"MACHINE_FUNCTION_PILE4_ORDER", // 堆疊順序_第4組
};

char*	Str_PileOrder[] = // 各組順序 字串
{
	"PICKER_PILE_ORDER_1",
	"PICKER_PILE_ORDER_2",
	"PICKER_PILE_ORDER_3",
	"PICKER_PILE_ORDER_4",
	"PICKER_PILE_ORDER_5",
	"PICKER_PILE_ORDER_6",
};


char*	u_pszImgPile[] =  // 各組 顯示圖片 名稱
{
	"Img_Pile1",
	"Img_Pile2",
	"Img_Pile3",
	"Img_Pile4",
};
char* PileImgpath[] = // 顯示圖片
{																				// 	(bit0 X, bit1 Y, bit3 Z) 0=正向 1=反向
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
		
	// 取得指標 快速教導 堆疊參數設置 遮罩
	pwndMask_QTeach_Pile = pwndSender->FindControlFromName("Mask_QTeach_Pile");
	pwndBtnNextP = pwndSender->FindControlFromName("BtnNextP");
	pwndBtnLastP = pwndSender->FindControlFromName("BtnLastP");
	pwndStr_QTeach_Pile = pwndSender->FindControlFromName("Str_QTeach_Pile");
	
	
	for(int i = 0; i < (PileNum); i++) // 第n組堆疊 抓取Btn指標
	{
		u_pwndBtn_Pile_Group[i] = pwndSender->FindControlFromName(u_pszBtnPileGroupName[i]);
	}
	
	for(int i = 0; i < (PileNum); i++) // 各軸方向 抓取Btn指標
	{
		for(int j=0;j<3;j++)
		{
			u_pwndBtn_Pile_Dir[i][j] = pwndSender->FindControlFromName(u_pszBtnPileDirName[i][j]);
		}
	}
	for(int i = 0; i < (PileNum); i++) // 各組順序 抓取Btn指標
	{
		u_pwndBtn_Pile_Order[i] = pwndSender->FindControlFromName(u_pszBtnPileOrderName[i]);
	}
	for(int i = 0; i < (PileNum); i++) // 抓取各組 顯示圖片指標
	{
		u_pwndImg_Pile[i] = pwndSender->FindControlFromName(u_pszImgPile[i]);
	}
	
	//Update string
	for(int i = 0; i < (PileNum); i++) // 各軸方向 顯示文字
	{
 		for(int j=0;j<3;j++)
 		{
			if(u_pwndBtn_Pile_Dir[i][j] != NULL)
			{
				int u_PileDir = (int)(GetDBValue(u_pszPileDirDB[i]).lValue);
				printf("u_PileDir[%d][%d]=%d\n",i,j,u_PileDir);
				u_PileDir = ((u_PileDir>>j) & 1); // 讀取第j個bit

				b_Dir[i][j]=TF[u_PileDir]; // 讀取方向紀錄
				u_pwndBtn_Pile_Dir[i][j]->SetPropValueT("captionID",Str_PileDir[u_PileDir]);
				u_pwndBtn_Pile_Dir[i][j]->CreateA();
				u_pwndBtn_Pile_Dir[i][j]->Update();
			}
		}
	}
	
	for(int i = 0; i < (PileNum); i++) // 各組順序 顯示文字
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
			b_RunOnlyOne = TRUE;// 完成 執行一次
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
 			if(pwnd == u_pwndBtn_Pile_Dir[i][j]) // 堆疊方向 [第n組][XYZ]
 			{
				b_Dir[i][j] = !b_Dir[i][j];
				// (bit0 X, bit1 Y, bit3 Z) 0=正向 1=反向
				int nTemp=0;
				for(int k=0;k<3;k++)
				{
					nTemp = nTemp + (b_Dir[i][k]<<k);
				}
				printf("Pile%d Dir%d=%d\n",i,j,nTemp);
				SetDBValue(u_pszPileDirDB[i],nTemp); // 寫入 DB
				printf("Set u_pszPileDirDB[%d]=%d\n",i,nTemp);
				
				//Update string
				
				int u_PileDir = (int)(GetDBValue(u_pszPileDirDB[i]).lValue);
				u_PileDir = ((u_PileDir>>j) & 1); // 讀取第j個bit
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
	
	if(pwnd == pwndBtnNextP) // 下一頁 Btn 按下彈起
	{
		::PutCommand("QTeach_Done.txt"); // 跳轉至 教導完成頁
	}
	else if(pwnd == pwndBtnLastP) // 上一頁 Btn 按下彈起
	{
		::PutCommand("QTeach_PlaceP.txt"); // 返回上一頁
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
	pwndMask_QTeach_Pile->Update(); // 遮罩
	if(g_Pile_Select>0)
		{
			pwndBtnNextP->SetPropValueT("enabled",double(TRUE));
			pwndBtnLastP->SetPropValueT("enabled",double(TRUE));
			pwndStr_QTeach_Pile->SetPropValueT("enabled",double(TRUE));
			pwndBtnNextP->Update();
			pwndBtnLastP->Update();
			pwndStr_QTeach_Pile->Update();
			for(int i=0;i<PileNum;i++) // 鎖住所有 堆疊組 按鈕
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