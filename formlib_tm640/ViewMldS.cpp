/*===========================================================================+
|  Class    : ViewMldS library                                               |
|  Task     : ViewMldS library action source file                            |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include 	"ViewMldS.h"
#include 	"../commonaction.h"
#include 	"../dialog.h"
#include 	"../utils.h"
#include 	"../moldControl.h"
#include 	"../selectedit.h"
#include	"../lib/libiNetComm.h"
#include 	"../main.h"//zholy070416
#include 	"../tmshmsg.h"
#include	"../iotable.h"//tyl 2013.10.23

/*===========================================================================+
|           Named rule Help                                                  |
+===========================================================================*/
// 模具頭表         --  editDESTITable
//     序號             --  editDESTITable__1: example -- editDESTITable001, editDESTITable011, ... ...
//     模具名稱         --  editDESTITable__2: example -- editDESTITable002, editDESTITable012, ... ...
//     存儲日期         --  editDESTITable__3: example -- editDESTITable003, editDESTITable013, ... ...
//     顏色             --  editDESTITable__4: example -- editDESTITable004, editDESTITable014, ... ...
//     材料             --  editDESTITable__5: example -- editDESTITable005, editDESTITable015, ... ...
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     MAX_EDITTABLE_ROWS     21
#define     MAX_EDITTABLE_COLS     6
#define     STR_EDITTABLE_DNAME    "editDESTITable"
#define     STR_EDITTABLE_NEW      "editNEW"
#define     MAX_EDITTABLE_SIZE     50
#define     MAX_MOLDNAME_LENGTH    32+1		//fuxy, 2011-11-18 for Mold 32

typedef     WORD                   tmMEDIATYPE;

#define		PAGEUP_KEY			0x01
#define		PAGEDOWN_KEY		0x02

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*     pwndEditDesti		= NULL;
CtmWnd*     pwndEditSaveMode	= NULL;
CtmWnd*     pwndEditConfirm		= NULL;
CtmWnd*     pwndEditSourceMax	= NULL;
CtmWnd*     pwndEditSourceRemain = NULL;
CtmWnd*     pwndEditNewName		= NULL;
CtmWnd*		pwndEditNewMaterial	= NULL;
CtmWnd*		pwndEditNewColor	= NULL;
CtmWnd*		pwndButtonPageDown	= NULL;
CtmWnd*		pwndButtonPageUp	= NULL;
CtmWnd*		pwndEditNewNumber	= NULL;
CtmWnd*		pwndEditNewDate		= NULL;
CtmWnd*		pwndEditSortType	= NULL;
CtmWnd*   pwndEditCurrentMold	= NULL;

CtmWnd*   pwndButton_Read	= NULL;
CtmWnd*   pwndButton_Del	= NULL;


CtmWnd*     pwndEditDestiTable[MAX_EDITTABLE_ROWS * MAX_EDITTABLE_COLS];

//CtmWnd*		pwndUpdate[256] 	= {NULL};
int			iUpdateNum 			= 0;
FGC		  	m_destinationfgc;
BGC 	  	m_destinationbgc;

short       nDestiTableRows	= 0;
short       nDestiTableEditCount[MAX_EDITTABLE_ROWS];
char        m_acEditNameTemp[MAX_EDITTABLE_SIZE];
char        m_szDestiTable[MAX_MOLD_CNT * MAX_EDITTABLE_COLS][MAX_MOLDNAME_LENGTH];

int 	    nMaxCount 	= 0, nDestiCount	= 0;		//最大儲存量,   已儲存的模具數
int			nStartValue = 1, nPageSourceCount = 1;	//在本頁顯示的模具（起始NO和個數）
int			nIndex 		= 0, nIndexOld 	= 0;

STROAGETYPE m_nDestiMedia = (STROAGETYPE)0;
STROAGETYPE m_nDestiMedia_old = (STROAGETYPE)0;		// Check if Media changed. yuc<080118>

// <<  For Sort New MOLDHDRs yuc080114
#define 	MAX_MOLD_PER_INET_PAGE 		10		//fuxy, 2011-11-18, add for Mold32
#define 	MAX_MOLD_PER_PAGE 			10//20
DB_MOLDHDR 	SortMoldData[MAX_MOLD_CNT];			//本地臨時緩存，用于排序
DB_MOLDHDR 	TempMoldData[MAX_MOLD_CNT];			//本地臨時緩存，用于排序 存儲iNet模組
int m_SortType		= SORT_BY_DATE;				//排序類型
int m_SortType_old	= SORT_BY_DATE;				//排序類型
int			nMoldCnt_Per_Page = 0;
int		nMoldSavePri = 0, g_iPriOld = 0;
int     lTimeData[2] = {0};
// >>

bool		m_bEnterKey = FALSE;
bool		m_bCheckKey = FALSE;
BOOL		m_biNet = 0;
BOOL 		bPriv = FALSE;
BOOL 		bConfirm = FALSE;

enum    tagtmMEDIATYPE
{
	tmMEDIA_MMI,
	tmMEDIA_SERVER,
	tmMEDIA_DISK
};

extern	BOOL	g_biNetStatus;
void 		SaveMold();
void		SaveCurrentMold();
STROAGETYPE		TOStorageType(int type);
DATE	tmDATE2DATE(tmDATE tmdate);
CtmFile	tmfile;
BOOL 	m_b7TR = FALSE;

// cjlee add ↓ 2019/2/23 上午 08:35:11

CtmWnd*   pwndBtn_SortType	= NULL;
BOOL 			b_SortType = 0;
char*	Str_Sort[] =
{
	"VW_MOLD_DATE",
	"VW_MOLD_NAME",
};

CtmWnd*   pwndBtn_DestMedia	= NULL;
BOOL 			b_DestMedia = 0;
int				n_DestMedia = 0;
char*	Str_DestMedia[] =
{
	"VW_MOLD_MMISAVE",
	"VW_MOLD_MEMORYCARD",
};

CtmWnd*   pwndBtn_SaveType	= NULL;
BOOL 			b_SaveType = 0;
int				n_SaveType = 0;
char*	Str_SaveType[] =
{
	"VW_MOLD_MODEHELPSAVEAS",
	"VW_MOLD_MODEHELPREPLACE",
};
CtmWnd*   pwndBtnConfirm	= NULL;

// cjlee add ↑ 2019/2/23 上午 08:35:11

/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	short   i = 0, j = 0;
	char    msz[4];
	int   nViewID = ID_VIEW_MOLDSAVE;
	g_pDatabase->Write("SYS_HMI_NULL_NULL_VIEWID", &nViewID, FALSE);
	m_biNet = CtmConfig::GetInstance()->GetINETFlag();

	pwndEditSortType     = pwndSender->FindControlFromName("editTAXISMEDIA");
	pwndEditDesti        = pwndSender->FindControlFromName("editDESTIMEDIA");
	pwndEditSaveMode     = pwndSender->FindControlFromName("editSAVEMODESEL");
	pwndEditConfirm      = pwndSender->FindControlFromName("editCONFIRM");
	pwndEditSourceMax    = pwndSender->FindControlFromName("editSOURCEMAX");
	pwndEditSourceRemain = pwndSender->FindControlFromName("editSOURCEREMAIN");
	pwndEditNewNumber	 = pwndSender->FindControlFromName("editNEWNUMBER");
	pwndEditNewName      = pwndSender->FindControlFromName("editNEWNAME");
	pwndEditNewMaterial  = pwndSender->FindControlFromName("editNEWMATERIAL");
	pwndEditNewColor	 = pwndSender->FindControlFromName("editNEWCOLOR");
	pwndEditNewDate		 = pwndSender->FindControlFromName("editNEWDATE");
	pwndButtonPageDown	 = pwndSender->FindControlFromName("buttonDOWNTWENTY");
	pwndButtonPageUp	 = pwndSender->FindControlFromName("buttonUPTWENTY");
	pwndEditCurrentMold  = pwndSender->FindControlFromName("editCURRENTMOLD");

	pwndButton_Read	 = pwndSender->FindControlFromName("Button_Read");
	pwndButton_Del  = pwndSender->FindControlFromName("Button_Del");
	
	pwndBtn_SortType  = pwndSender->FindControlFromName("BtnSortType"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtn_DestMedia  = pwndSender->FindControlFromName("BtnDestMedia"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtn_SaveType  = pwndSender->FindControlFromName("BtnSaveType"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtnConfirm  = pwndSender->FindControlFromName("BtnConfirm"); // cjlee add 2019/2/23 上午 08:43:25
	
	
	m_b7TR 		= g_pExTable->GetTableValue(0, "D_7TR");

	if (!g_pExTable->GetTableValue(0, "D_7LL"))
	{

		if (m_biNet)
		{
			ReadMldCrlData();
			g_iPriOld = g_iPrivilege;
			nMoldSavePri = lTimeData[0];
			if (nMoldSavePri)
			{
				if (g_iPrivilege < nMoldSavePri)
				{
					g_iPrivilege = 0;
					bPriv = TRUE;
				}
			}
			else
			{
				if (g_iPrivilege == 0)
				{
					g_iPrivilege = 1;
					bPriv = TRUE;
				}
			}

		}
	}
///	if(CtmConfig::GetInstance()->GetINETFlag() == 1)		//fuxy, 2011-11-21, for Mold 32
///		nMoldCnt_Per_Page = MAX_MOLD_PER_INET_PAGE;
///	else
	nMoldCnt_Per_Page = MAX_MOLD_PER_PAGE;

//	iUpdateNum 		= GetSpecialControlNum(pwndSender, -1, "CtmEditX1", pwndUpdate);


	for (i = 0; i < MAX_EDITTABLE_ROWS; i++)
	{
		for (j = 0; j < MAX_EDITTABLE_COLS; j++)
		{
			pwndEditDestiTable[i * MAX_EDITTABLE_ROWS + j]  = NULL;
		}
	}

	for (i = 0; i < MAX_EDITTABLE_ROWS; i++)
	{
		sprintf(msz, "%02d", i);
		strcpy(m_acEditNameTemp, STR_EDITTABLE_DNAME);
		strcat(m_acEditNameTemp, msz);
		nDestiTableEditCount[i]  = GetSpecialControlNum(pwndSender, m_acEditNameTemp, "CtmEdit", &pwndEditDestiTable[MAX_EDITTABLE_COLS * i]);
		if (nDestiTableEditCount[i]  > 0)    nDestiTableRows++;
	}

	OnCreateCommon(pwndSender);
	CreateSet(pwndSender);
	SetServiceView();									//設置伺服器是否可用

	if (pwndEditNewNumber != NULL)
	{
		pwndEditNewNumber->SetPropValueT("text", "");		//fuxy, 2011-4-19, add
		pwndEditNewNumber->Update();
	}


	if (pwndEditNewName != NULL)
	{
		pwndEditNewName->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
		pwndEditNewName->Update();
	}

	if (pwndEditCurrentMold != NULL)		//fuxy, 2011-11-17, add for mold32
	{
		pwndEditCurrentMold->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
		pwndEditCurrentMold->Update();
	}

	if (g_pExTable->GetTableValue(0, "D_SOFTLOCK")) //TYL 2013.10.21 for 7LU SOFTLOCK
	{
		if (pwndEditDesti != NULL)
			pwndEditDesti->UpdateAll();
	}

	// cjlee add ↓ 2019/2/23 上午 08:37:52
		if(pwndBtn_SortType != NULL)
			{
				m_SortType = b_SortType;
				//Update string
				pwndBtn_SortType->SetPropValueT("captionID",Str_Sort[m_SortType]);	
				pwndBtn_SortType->CreateA();
				pwndBtn_SortType->Update();
			}
		if(pwndBtn_DestMedia != NULL)
			{
				n_DestMedia = b_DestMedia;
				m_nDestiMedia = TOStorageType(n_DestMedia);
				//Update string
				pwndBtn_DestMedia->SetPropValueT("captionID",Str_DestMedia[n_DestMedia]);	
				pwndBtn_DestMedia->CreateA();
				pwndBtn_DestMedia->Update();
			}
		if(pwndBtn_SaveType != NULL)
			{
				n_SaveType = b_SaveType;
				//Update string
				pwndBtn_SaveType->SetPropValueT("captionID",Str_SaveType[n_SaveType]);	
				pwndBtn_SaveType->CreateA();
				pwndBtn_SaveType->Update();
			}
			
	// cjlee add ↑ 2019/2/23 上午 08:37:52
	
	return TRUE;
}

void  OnDestroyA(CtmWnd* pwndSender)
{
#ifdef D_ARM
	if (g_wMMCStatus)
	{
		CtmMMCardFile::GetInstance()->UMount();
	}
	if (g_wUSBStatus)
	{
		CtmUsbFile::GetInstance()->UMount();
	}
#endif
	SetDBValue("SYS_HMI_NULL_NULL_VIEWID", 0);

	if (!g_pExTable->GetTableValue(0, "D_7LL"))
	{
		if (bPriv && g_iPrivilege != 8) /*Dongxl2015-11-17 添加8權級判斷避免進機器和組態時權級被更改  */
			g_iPrivilege = g_iPriOld;
	}

}
/*
void	OnUpdateA(CtmWnd* pwndSender)
{
	OnFastUpdateCommon(pwndUpdate, iUpdateNum);
}
*/
void	OnShowA(CtmWnd* pwndSender)
{
	if (pwndEditNewName != NULL)
		pwndEditNewName->Update();
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA\n");
	printf("wKey=%d\n",wKey);
//	switch(wKey)
//	{
//		case 97:
//			::PutCommand("Hand.txt");
//			break;
//		case 98:
//			break;
//		case 99:
//			::PutCommand("MldR.txt");
//			break;
//		case 100:
//			::PutCommand("MldD.txt");
//			break;
//		default:
//			break;
//	}
	switch(wKey)
	{
		case 70: // Home
			::PutCommand("Over.txt"); // cjlee 2019/4/17 下午 06:56:41
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
	}
	if (wKey == _ENTER)
	{
		m_bEnterKey = TRUE;
	}
	else
	{
		m_bEnterKey = FALSE;
	}

	if (wKey == 46)
	{
		m_bCheckKey = TRUE;
	}
	else
	{
		m_bCheckKey = FALSE;
	}
	if (wKey == PAGEUP_KEY && pwndButtonPageUp)
	{
		PageUp(pwndSender);
	}
	if (wKey == PAGEDOWN_KEY && pwndButtonPageDown)
	{
		PageDown(pwndSender);
	}

	if (pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
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
	printf("OnMouseUp\n");
	int 		iTemp = 0, nTemp = 0;
	BOOL        bTemp = 0;
	long long   lTemp = 0;
	char		sztempMold[MAX_MOLDNAME_LENGTH];
	memset(sztempMold, 0, sizeof(sztempMold));
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_Read)
	{
		::PutCommand("MldR.txt");
	}
	if(pwnd ==pwndButton_Del)
	{
		::PutCommand("MldD.txt");
	}
	
	// cjlee add ↓ 2019/2/23 上午 09:04:07

	if (pwnd == pwndBtn_DestMedia) //存儲目標
	{
		b_DestMedia = !b_DestMedia;
		n_DestMedia = b_DestMedia;
		m_nDestiMedia = TOStorageType(n_DestMedia);
		//Update string
		pwndBtn_DestMedia->SetPropValueT("captionID",Str_DestMedia[n_DestMedia]);
		pwndBtn_DestMedia->CreateA();
		pwndBtn_DestMedia->Update();

		printf("Source from %d\n",m_nDestiMedia);

		if (m_nDestiMedia != m_nDestiMedia_old)	//yuc<080118>
		{
			nStartValue = 1;

			GetMoldCount();		//2009-5-21
			if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList(nStartValue);			//James modify 2008/7/22 選擇目標為iNet的時候，從iNet讀取模組
			else
				UpdateDestination(pwndSender);		//2009-5-21, 注意不要重復調用，會導致gui刷新不正常

			m_nDestiMedia_old = m_nDestiMedia;	//yuc<080118>
		}
	}
	
	else if(pwnd == pwndBtn_SortType) //從界面獲得排序類型,name,date
		{
			b_SortType = !b_SortType;
			m_SortType = b_SortType;
			//Update string
			pwndBtn_SortType->SetPropValueT("captionID",Str_Sort[m_SortType]);
			pwndBtn_SortType->CreateA();
			pwndBtn_SortType->Update();
			
			if (m_SortType != m_SortType_old)	//If changed
			{
				nStartValue = 1;
				GetMoldCount();
				if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
					GetNetMoldList(nStartValue);
				else
					UpdateDestination(pwnd);
				m_SortType_old = m_SortType;
			}
		}
		
	else if (pwnd == pwndBtn_SaveType) // (Cover/Save as...)
	{
			b_SaveType = !b_SaveType;
			n_SaveType = b_SaveType;
			//Update string
			pwndBtn_SaveType->SetPropValueT("captionID",Str_SaveType[n_SaveType]);
			pwndBtn_SaveType->CreateA();
			pwndBtn_SaveType->Update();
		
		bTemp = FALSE;
		if (pwndEditNewNumber != NULL)
		{
			pwndEditNewNumber->SetPropValueT("enabled", bTemp);
			pwndEditNewNumber->Update();
		}
		if (pwndEditNewDate != NULL)
		{
			pwndEditNewDate->SetPropValueT("enabled", bTemp);
			pwndEditNewDate->Update();
		}

		if (lTemp == 0)
			bTemp = TRUE;
		else if (lTemp == 1)
			bTemp = FALSE;

		if (pwndEditNewName != NULL)
		{
			pwndEditNewName->SetPropValueT("enabled", bTemp);
			pwndEditNewName->Update();
		}
		if (pwndEditNewMaterial != NULL)
		{
			pwndEditNewMaterial->SetPropValueT("enabled", bTemp);
			pwndEditNewMaterial->Update();
		}
		if (pwndEditNewColor != NULL)
		{
			pwndEditNewColor->SetPropValueT("enabled", bTemp);
			pwndEditNewColor->Update();
		}
	}
	
	else if (pwnd == pwndBtnConfirm) // 確認
	{
		{
			bConfirm =	1;
				if (pwndEditNewName != NULL)
					pwndEditNewName->GetPropValueT("text", sztempMold,	sizeof(sztempMold));
				if (strcmp(sztempMold, "IMCS") == 0) 							//不能為IMCS
				{
					MsgBox(g_MultiLanguage["VW_MOLD_DEFAULTMOLD"], tmFT_CODE_TECH);
					return wIDControl;
				}
				//  must not the same name when use "SAVE AS"
				if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
				{
					// if the mold is exist in inet server, promp the message is overview?
					for (int i = 0; SortMoldData[i].acID[0] != '\0'; i++)
					{
						if (!strcmp(sztempMold, SortMoldData[i].acID) && !strcmp)
						{
							MsgBox(g_MultiLanguage["PICKER_FILE_OVERWRITE"], tmFT_CODE_TECH);
							if (!g_bMsgBoxFlag)												//James add 2008/7/22 獲得msgbox的值，決定是否覆蓋模組
								return wIDControl;
							break;
						}
					}
				}
				else
				{
					if (n_SaveType == 0) // "SAVE AS"
					{
						for(int i=0; i<nDestiCount; i++) // check if same name
						{
							//printf("Moldname=%s\n",SortMoldData[i].acID);
							if (strcmp(sztempMold,SortMoldData[i].acID) == 0)
								{
									MsgBox(g_MultiLanguage["PICKER_FILE_CHANGENAME"], tmFT_CODE_TECH);
									return wIDControl;
								}
						}
					}
//					if (strcmp(sztempMold, CtmMoldSet::GetInstance()->GetCurrentMold()) == 0)
//					{
//						if (n_SaveType == 0)
//						{
//							MsgBox(g_MultiLanguage["PICKER_FILE_CHANGENAME"], tmFT_CODE_TECH);
//							return wIDControl;
//						}
//					}
				}
				MsgBox(g_MultiLanguage["PICKER_FILE_SAVE"], tmFT_CODE_TECH);
				if (!g_bMsgBoxFlag)	return wIDControl;
				SaveMold();
				////James modify 2008/4/16 07:05下午   多國語言轉換
				char*	szTitle = NULL, m_pszTitleID[256];
				pwndSender->GetPropValueT("titleID", m_pszTitleID, sizeof(m_pszTitleID));
				StrIDToStr2(m_pszTitleID, &szTitle);
				g_pMainFrame->SetTitle(szTitle);

				GetMoldCount();		//2009-5-21
				if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
					GetNetMoldList(nStartValue);
				else
					UpdateDestination(pwndSender);
				//<<<JOYCE2008-10-24 add
#ifdef D_ARM
				if (g_wMMCStatus)
				{
					CtmMMCardFile::GetInstance()->UMount(0, 0);
					CtmMMCardFile::GetInstance()->Mount(0, 0);
				}
				if (g_wUSBStatus)
				{
					CtmUsbFile::GetInstance()->UMount(0, 0); //參數給0,是指不需要提示字串
					CtmUsbFile::GetInstance()->Mount(0, 0);
				}
#endif
				//>>>JOYCE2008-10-24 add
			
		}
	}
		
	if (pwnd == pwndButtonPageUp) // 上一頁
	{
		PageUp(pwndSender);
	}
	if (pwnd == pwndButtonPageDown) // 下一頁
	{
		PageDown(pwndSender);
	}
	// cjlee add ↑ 2019/2/23 上午 09:52:28
	
  return wIDControl;	
}
//<<<<ycfan 20080129
WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	printf("OnchangeBM\n");
	int 		iTemp = 0, nTemp = 0;
	BOOL        bTemp = 0;
	long long   lTemp = 0; 	//nMedia =0, lEditValue =0;
	char		sztempMold[MAX_MOLDNAME_LENGTH];
	memset(sztempMold, 0, sizeof(sztempMold));
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if (pwnd == NULL)	return wIDControl;

	//存儲目標
	if (pwnd == pwndEditDesti)
	{
		if (pwnd->Is("CtmEditX1"))
			pwnd->GetPropValueT("value", &iTemp, sizeof(iTemp)); //zholy070416
		else  if (pwnd->Is("CtmSelectEdit"))
			iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
		m_nDestiMedia = TOStorageType(iTemp);
		printf("Source from %d\n",m_nDestiMedia);

		if (m_nDestiMedia != m_nDestiMedia_old)	//yuc<080118>
		{
			nStartValue = 1;

			GetMoldCount();		//2009-5-21
			if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList(nStartValue);			//James modify 2008/7/22 選擇目標為iNet的時候，從iNet讀取模組
			else
				UpdateDestination(pwndSender);		//2009-5-21, 注意不要重復調用，會導致gui刷新不正常

			m_nDestiMedia_old = m_nDestiMedia;	//yuc<080118>
		}
	}
	else if (pwnd == pwndEditSaveMode) // (Cover/Save as...)
	{
		if (pwnd->Is("CtmEditX1"))								 //Sunny<20070530>
			pwnd->GetPropValueT("value", &iTemp, sizeof(iTemp)); //zholy070416
		else if (pwnd->Is("CtmSelectEdit"))
			iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();

		bTemp = FALSE;
		if (pwndEditNewNumber != NULL)
		{
			pwndEditNewNumber->SetPropValueT("enabled", bTemp);
			pwndEditNewNumber->Update();
		}
		if (pwndEditNewDate != NULL)
		{
			pwndEditNewDate->SetPropValueT("enabled", bTemp);
			pwndEditNewDate->Update();
		}

		if (lTemp == 0)
			bTemp = TRUE;
		else if (lTemp == 1)
			bTemp = FALSE;

		if (pwndEditNewName != NULL)
		{
			pwndEditNewName->SetPropValueT("enabled", bTemp);
			pwndEditNewName->Update();
		}
		if (pwndEditNewMaterial != NULL)
		{
			pwndEditNewMaterial->SetPropValueT("enabled", bTemp);
			pwndEditNewMaterial->Update();
		}
		if (pwndEditNewColor != NULL)
		{
			pwndEditNewColor->SetPropValueT("enabled", bTemp);
			pwndEditNewColor->Update();
		}
	}
	else if (pwnd == pwndEditConfirm)
	{
		//if(m_bEnterKey)	//2013-6-21, mark for touch
		{
			if (pwnd->Is("CtmEditX1"))						//Sunny<20070530>
				pwnd->GetPropValueT("value", &iTemp, sizeof(iTemp));
			else if (pwnd->Is("CtmSelectEdit"))
				iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
			bConfirm =	iTemp;
			if (iTemp == 1)
			{
				nTemp = 0;
				if (pwnd->Is("CtmEditX1"))						//Sunny<20070530>
					pwnd->SetPropValueT("value", nTemp);
				else if (pwnd->Is("CtmSelectEdit"))
					((CtmSelectEdit*)pwnd)->SetIndex(nTemp);
				pwnd->Update();

				if (pwndEditNewName != NULL)
					pwndEditNewName->GetPropValueT("text", sztempMold,	sizeof(sztempMold));
				if (strcmp(sztempMold, "IMCS") == 0) 							//不能為IMCS
				{
					MsgBox(g_MultiLanguage["VW_MOLD_DEFAULTMOLD"], tmFT_CODE_TECH);
					return wIDControl;
				}
				//  must not the same name when use "SAVE AS"
				if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
				{
					// if the mold is exist in inet server, promp the message is overview?
					for (int i = 0; SortMoldData[i].acID[0] != '\0'; i++)
					{
						if (!strcmp(sztempMold, SortMoldData[i].acID) && !strcmp)
						{
							MsgBox(g_MultiLanguage["PICKER_FILE_OVERWRITE"], tmFT_CODE_TECH);
							if (!g_bMsgBoxFlag)												//James add 2008/7/22 獲得msgbox的值，決定是否覆蓋模組
								return wIDControl;
							break;
						}
					}
				}
				else
				{
					if (strcmp(sztempMold, CtmMoldSet::GetInstance()->GetCurrentMold()) == 0)
					{
						if (pwndEditSaveMode->Is("CtmEditX1"))								//Sunny<20070530>
							pwndEditSaveMode->GetPropValueT("value", &iTemp, sizeof(iTemp));	//zholy070416
						else if (pwndEditSaveMode->Is("CtmSelectEdit"))
							iTemp = ((CtmSelectEdit*)pwndEditSaveMode)->GetIndex();
						if (iTemp == 0)
						{
							MsgBox(g_MultiLanguage["PICKER_FILE_CHANGENAME"], tmFT_CODE_TECH);
							return wIDControl;
						}
					}
				}
				MsgBox(g_MultiLanguage["PICKER_FILE_SAVE"], tmFT_CODE_TECH);
				if (!g_bMsgBoxFlag)	return wIDControl;
				SaveMold();
				////James modify 2008/4/16 07:05下午   多國語言轉換
				char*	szTitle = NULL, m_pszTitleID[256];
				pwndSender->GetPropValueT("titleID", m_pszTitleID, sizeof(m_pszTitleID));
				StrIDToStr2(m_pszTitleID, &szTitle);
				g_pMainFrame->SetTitle(szTitle);

				GetMoldCount();		//2009-5-21
				if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
					GetNetMoldList(nStartValue);
				else
					UpdateDestination(pwndSender);
				//<<<JOYCE2008-10-24 add
#ifdef D_ARM
				if (g_wMMCStatus)
				{
					CtmMMCardFile::GetInstance()->UMount(0, 0);
					CtmMMCardFile::GetInstance()->Mount(0, 0);
				}
				if (g_wUSBStatus)
				{
					CtmUsbFile::GetInstance()->UMount(0, 0); //參數給0,是指不需要提示字串
					CtmUsbFile::GetInstance()->Mount(0, 0);
				}
#endif
				//>>>JOYCE2008-10-24 add
			}
		}
	}
	// << new yuc080114
	else if (pwnd == pwndEditSortType) //從界面獲得排序類型,name,date
	{
		//Get the SortType,if changed, Reload the data and save.
		if (pwnd->Is("CtmSelectEdit"))
			m_SortType = ((CtmSelectEdit*)pwnd)->GetIndex();
		if (m_SortType != m_SortType_old)	//If changed.		yuc<080118>;
		{
			nStartValue = 1;
			GetMoldCount();
			if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList(nStartValue);
			else
				UpdateDestination(pwnd);
			m_SortType_old = m_SortType;
		}
	}

	return wIDControl;
}
//>>>>

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if (pwnd == NULL)		return wIDControl;

	if (pwnd == pwndButtonPageDown)	//下一頁
	{
		if (m_bEnterKey)
		{
			PageDown(pwndSender);	//2013-5-28
		}
	}
	else if (pwnd == pwndButtonPageUp) 	//上一頁
	{
		if (m_bEnterKey)
		{
			PageUp(pwndSender);
		}
	}
//    SetServiceView();									//設置伺服器是否可用

	return wIDControl;
}

/*---------------------------------------------------------------------------+
|       Helper Functions                                                     |
+---------------------------------------------------------------------------*/
void       CreateSet(CtmWnd* pwndSender)
{
	int 	ntemp 	= 0;
	if (pwndEditDesti != NULL)
	{
		if (pwndEditDesti->Is("CtmEditX1"))
			pwndEditDesti->GetPropValueT("value", &ntemp, sizeof(ntemp)); //zholy070416
		else  if (pwndEditDesti->Is("CtmSelectEdit"))
			ntemp = ((CtmSelectEdit*)pwndEditDesti)->GetIndex();

		m_nDestiMedia = m_nDestiMedia_old = TOStorageType(ntemp); 	//yuc<080118>
		if ( m_nDestiMedia == STORAGE_TYPE_USB || m_nDestiMedia == STORAGE_TYPE_MMCARD)
		{
			Prompt(g_MultiLanguage["VM_DATA_READING"]);
		}
	}
	// cjlee 2019/2/23 下午 03:15:00
	
	if (pwndBtn_DestMedia != NULL)
	{
		ntemp = n_DestMedia;
		m_nDestiMedia = m_nDestiMedia_old = TOStorageType(ntemp); 	//yuc<080118>
		if ( m_nDestiMedia == STORAGE_TYPE_USB || m_nDestiMedia == STORAGE_TYPE_MMCARD)
		{
			Prompt(g_MultiLanguage["VM_DATA_READING"]);
		}
	}
	// cjlee 2019/2/23 下午 03:15:05
	GetMoldCount();		//2009-5-21
	SetDestination(pwndSender);
}

void	GetMoldCount()
{
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
		SendMsg(MSG_SH_REQUEST_INET_MOLD_COUNT, 0, 0, NULL);
	else
	{
		//獲取 nMaxCount,nDestiCount ,前者指可存儲最大數,後者指獲得的模組數
		nMaxCount     = CtmMoldSet::GetInstance()->GetMaxMoldSetNum ();
		nDestiCount   = CtmMoldSet::GetInstance()->GetMoldSetNum (m_nDestiMedia);
		nPageSourceCount = (nMoldCnt_Per_Page < nDestiCount - nStartValue + 1) ? nMoldCnt_Per_Page : nDestiCount - nStartValue + 1;
	}
}

void  SetDestination(CtmWnd* pwndSender)
{
	int         i = 0 , j = 0;
	int         StartIndex = 0;
	int			ntempStartIndex = 0;
	StartIndex 	= nStartValue;

	//設置最大存儲容量和剩余容量
	if (pwndEditSourceMax   != NULL)
	{
		pwndEditSourceMax->SetPropValueT("value", nMaxCount);
		pwndEditSourceMax->Update();
	}
	if (pwndEditSourceRemain != NULL)
	{
		pwndEditSourceRemain->SetPropValueT("value", nMaxCount - nDestiCount);
		pwndEditSourceRemain->Update();
	}
	// <<	 yuc080114
	//加載MOLD到本地緩存，用于排序,同時使用排序類型和排序媒介
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
	{
		memset(SortMoldData, 0, sizeof(SortMoldData));
		memcpy(SortMoldData, TempMoldData, sizeof(TempMoldData));
		//使用循環記錄模組數量，不用nDestiCount 是應為剛剛發送給INet的模組，數量沒有及時增加
		int i = 0;
		for (i = 0; SortMoldData[i].acID[0] != '\0'; i++) {}
//	    nDestiCount = i;
	}
	else		//fuxy, 2011-4-20, add
	{
		LoadData(SortMoldData, m_nDestiMedia, nDestiCount, m_SortType);
//    	printf("%s\n",SortMoldData[0].acID);//zjh
	}
	//已使用臨時緩沖區代替直接從設備讀入

	if (m_nDestiMedia != STORAGE_TYPE_INET)
		ntempStartIndex = StartIndex - 1;
	else
		ntempStartIndex = 0;
	for (i = 0; i < nDestiCount; i++)		//nStartValue
	{
		// 此循環內的代碼需要在每一個模具相關的代碼里修改
		if (SortMoldData[i + ntempStartIndex].acID[0] == '\0' ) //是否有資料
			break;
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 0], "%3d"           , i + StartIndex);
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 1], "%s"            , SortMoldData[i + ntempStartIndex].acID);
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 2], "%02d/%02d/%02d", SortMoldData[i + ntempStartIndex].DateSave.month, SortMoldData[i + ntempStartIndex].DateSave.day, SortMoldData[i + ntempStartIndex].DateSave.year % 100);
		SortMoldData[i].szMaterial[5] = '\0';
		SortMoldData[i].szColor[5] = '\0';
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 3], "%s"            , SortMoldData[i + ntempStartIndex].szMaterial);
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 4], "%s"            , SortMoldData[i + ntempStartIndex].szColor);
	}
	// >>
	for (i = nPageSourceCount; i < nDestiTableRows; i++)		//fuxy, 2010-5-20, for (i = i; i < nDestiTableRows; i++)
	{
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 0], "%3d"           , i + StartIndex);
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 1], " ");
		sprintf(m_szDestiTable[i * MAX_EDITTABLE_COLS + 2], " ");
		strcpy (m_szDestiTable[i * MAX_EDITTABLE_COLS + 3], " ");
		strcpy (m_szDestiTable[i * MAX_EDITTABLE_COLS + 4], " ");
	}
	for (i = 0; i < nDestiTableRows; i++)
	{
		for (j = 0; j < nDestiTableEditCount[i]; j++)
		{
			if (pwndEditDestiTable[i * MAX_EDITTABLE_COLS + j] != NULL)
			{
				pwndEditDestiTable[i * MAX_EDITTABLE_COLS + j]->SetPropValueT("text", m_szDestiTable[i * MAX_EDITTABLE_COLS + j]);
				//printf("pwndEditDestiTable[%d]=%s\n",i * MAX_EDITTABLE_COLS + j,m_szDestiTable[i * MAX_EDITTABLE_COLS + j]);
			}
		}
	}
	strcpy(m_szDestiTable[MAX_EDITTABLE_COLS * (MAX_EDITTABLE_ROWS - 1)], g_MultiLanguage["VW_MOLD_NEWFILE"]);
	for (i = 1; i < nDestiTableEditCount[MAX_EDITTABLE_ROWS - 1]; i++)
		strcpy (m_szDestiTable[MAX_EDITTABLE_COLS * (MAX_EDITTABLE_ROWS - 1) + i], " ");
	for (i = 0; i < nDestiTableEditCount[MAX_EDITTABLE_ROWS - 1]; i++)
	{
		if (pwndEditDestiTable[(MAX_EDITTABLE_ROWS - 1)*MAX_EDITTABLE_COLS + i] != NULL)
		{
			pwndEditDestiTable[(MAX_EDITTABLE_ROWS - 1)*MAX_EDITTABLE_COLS + i]->SetPropValueT("text", m_szDestiTable[MAX_EDITTABLE_COLS * (MAX_EDITTABLE_ROWS - 1) + i]);
		}
	}
}

void  UpdateDestination(CtmWnd* pwndSender)
{
	short       i = 0, j = 0;

	SetDestination(pwndSender);
	if (pwndEditSourceMax    != NULL) 	pwndEditSourceMax->Update();
	if (pwndEditSourceRemain != NULL) 	pwndEditSourceRemain->Update();
	for (i = 0; i < nDestiTableRows; i++)
	{
		for (j = 0; j < nDestiTableEditCount[i]; j++)
		{
			if (pwndEditDestiTable[i * MAX_EDITTABLE_COLS + j] != NULL)
			{
				pwndEditDestiTable[i * MAX_EDITTABLE_COLS + j]->Update();
			}
		}
	}
}

void  UpdateDestinationBar(CtmWnd* pwndSender)
{
	int       i = 0;
	FGC       FgcTemp 	= YELLOW;
	BGC       BgcTemp 	= LIGHTBLUE;
	nIndex	= nStartValue;
	if (nIndexOld == nIndex && nIndexOld != 0) return;
	if ((nIndexOld >= 0) && (nIndexOld < nDestiTableRows))
	{
		for (i = 0; i < nDestiTableEditCount[0]; i++)
		{
			if (pwndEditDestiTable[nIndexOld * MAX_EDITTABLE_COLS + i] != NULL && nIndexOld >= 0)
				pwndEditDestiTable[nIndexOld * MAX_EDITTABLE_COLS + i]->Update();
		}
		for (i = 0; i < nDestiTableEditCount[0]; i++)
		{
			if (pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i] != NULL && nIndex >= 0)
			{
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->GetPropValueT("fgc", &m_destinationfgc, sizeof(m_destinationfgc));
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->GetPropValueT("bgc", &m_destinationbgc, sizeof(m_destinationbgc));
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->SetPropValueT("fgc", BgcTemp);
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->SetPropValueT("bgc", FgcTemp);
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->Update();
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->SetPropValueT("fgc", m_destinationfgc);
				pwndEditDestiTable[nIndex * MAX_EDITTABLE_COLS + i]->SetPropValueT("bgc", m_destinationbgc);

			}
		}
	}
	nIndexOld = nIndex;
}

void 	SaveMold()
{
	printf("SaveMold\n");
	char	sztempMold[MAX_MOLDNAME_LENGTH], sztempMaterial[20], sztempColor[20];
	int		temp = 0, nMoldIndex = 0,  nSaveCurrent = 0;
	BOOL 	bSaveCurrent = FALSE;
	DB_MOLDHDR	SourceHead;
	DB_MOLDHDR tempMoldHead;
	tmDATE			dtDate;
	memset(&tempMoldHead, 0 , sizeof(tempMoldHead));
	//存儲目標
	// cjlee 2019/2/23 下午 03:26:21
//	if (pwndEditDesti != NULL)
//	{
//		if (pwndEditDesti->Is("CtmEditX1"))
//			pwndEditDesti->GetPropValueT("value", &temp, sizeof(temp));
//		else if (pwndEditDesti->Is("CtmSelectEdit"))
//			temp = ((CtmSelectEdit*)pwndEditDesti)->GetIndex();
//		m_nDestiMedia = TOStorageType(temp);
//	}
		if (pwndBtn_DestMedia != NULL)
	{
		temp = n_DestMedia;
		m_nDestiMedia = TOStorageType(temp);
	}
	// cjlee 2019/2/23 下午 03:26:21
	
	//Get Mold Information
	if (pwndEditNewName != NULL)
		pwndEditNewName	->GetPropValueT("text", sztempMold,	sizeof(sztempMold));
	if (pwndEditNewMaterial != NULL)
		pwndEditNewMaterial ->GetPropValueT("text", sztempMaterial, sizeof(sztempMaterial));
	if (pwndEditNewColor != NULL)
		pwndEditNewColor	->GetPropValueT("text", sztempColor, sizeof(sztempColor));

	strncpy(tempMoldHead.acID, 			sztempMold, 		sizeof(tempMoldHead.acID) - 1);
	strncpy(tempMoldHead.szMaterial,	sztempMaterial,		sizeof(tempMoldHead.szMaterial) - 1);
	strncpy(tempMoldHead.szColor, 		sztempColor, 			sizeof(tempMoldHead.szColor) - 1);

	GetDate(&dtDate);
	tempMoldHead.cUnitCount = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
	tempMoldHead.DateSave	= tmDATE2DATE(dtDate);
	if(m_b7TR)
		tempMoldHead.Media	= ((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue & 0x0001) << 2) | ((GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue & 0x0001) << 1)
	                              | (GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue & 0x0001);

	memset(&SourceHead, 0 , sizeof(SourceHead));
	if (pwndEditSaveMode != NULL)
	{
		if (pwndEditSaveMode->Is("CtmOptionBox"))
			pwndEditSaveMode->GetPropValueT("selectindex", &nSaveCurrent, sizeof(nSaveCurrent));
		else if (pwndEditSaveMode->Is("CtmSelectEdit"))
			nSaveCurrent = ((CtmSelectEdit*)pwndEditSaveMode)->GetIndex();

		if (nSaveCurrent == 1) 	bSaveCurrent = TRUE;
	}
	if (n_SaveType == 1) 	bSaveCurrent = TRUE; // cjlee add 2019/2/23 上午 10:09:47
			
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
	{
		memcpy(&g_MoldHead, &tempMoldHead, sizeof(tempMoldHead));	//Sunny<20091119>

		SendMsg(MSG_SH_INET_DEAL_MOLD, 0, (int)STORAGE_TYPE_INET, NULL);
		//James add 將發送給iNet的模組立即存到本地的buffer，為了及時顯示，同時對應的模組總數要增加
		int i = 0;
		for (i = 0; TempMoldData[i].acID[0] != '\0'; i++)
		{}
		memcpy(&TempMoldData[i], &tempMoldHead, sizeof(DB_MOLDHDR));
		GetNetMoldList(nStartValue);	//2009-5-21
	}

	if (bSaveCurrent)
	{
		strncpy(SourceHead.acID, CtmMoldSet::GetInstance()->GetCurrentMold(), sizeof(SourceHead.acID));
		CtmMoldSet::GetInstance()->SaveCurrentDB(tempMoldHead, m_nDestiMedia);
//		if (m_nDestiMedia == STORAGE_TYPE_INET)
		SendMsg(MSG_SH_INET_DEAL_MOLD, 0, (int)STORAGE_TYPE_SRAM, NULL);	//2009-5-21
	}
	else
	{
		CtmMoldSet::GetInstance()->SaveCurrentDB(tempMoldHead, m_nDestiMedia);	//Sunny<20080228>另存時存儲最新的資料
		nMoldIndex = CtmMoldSet::GetInstance()->Find(tempMoldHead, m_nDestiMedia);	//2009-5-21
		// if (m_nDestiMedia == STORAGE_TYPE_INET)
		SendMsg(MSG_SH_INET_DEAL_MOLD, nMoldIndex, (int)STORAGE_TYPE_FLASH, NULL);
		MsgBox(g_MultiLanguage["VW_COMPLETED_DATA_SAVE"], tmFT_CODE_TECH);
	}

	if (m_nDestiMedia == STORAGE_TYPE_USB || m_nDestiMedia == STORAGE_TYPE_MMCARD)	//2012-3-27
	{
#ifdef	D_3354
		if (CheckFileExist("moldset.ini"))
		{
			CopyFile("moldset.ini", "/media/sda1/moldset/moldset.ini");
		}
		else
		{
			WriteMoldSetIni("/media/sda1/moldset/");
		}
#else
		if (CheckFileExist("moldset.ini"))
		{
			CopyFile("moldset.ini", "/mnt/usb/moldset/moldset.ini");
		}
		else
		{
			WriteMoldSetIni("/mnt/usb/moldset/");
		}
#endif
	}

	sztempMaterial[0] = sztempColor[0] = '\0';
	if (pwndEditNewName != NULL)
		pwndEditNewName	->SetPropValueT("text", "IMCS");
	if (pwndEditNewMaterial != NULL)
		pwndEditNewMaterial->SetPropValueT("text", sztempMaterial);
	if (pwndEditNewColor != NULL)
		pwndEditNewColor	->SetPropValueT("text", sztempColor);

}

STROAGETYPE		TOStorageType(int type)
{
	STROAGETYPE Result;
	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
	int	nStatus = 0;
	switch (type)		//choose storage type
	{
	case 0:
#ifdef D_NOWRITE_FALSH
		if (GetFileType(CHECKFILETYPE))
			nStorageType = STORAGE_TYPE_FLASH;
		Result = nStorageType;
#else
		Result = STORAGE_TYPE_SRAM;
#endif

		break;
	case 1:
#ifdef	D_ARM
#ifdef	D_PXA270		//James add for pxa270 sd 優先找usb，然后再找sd 2009/3/19 10:56上午
		if (!g_wUSBStatus)
		{
			nStatus = CtmUsbFile::GetInstance()->Mount(0);
		}
		else
		{
			nStatus = 1;
		}
#endif
		if (nStatus < 1)
		{
			Result = STORAGE_TYPE_MMCARD;
			if (!g_wMMCStatus)
				nStatus = CtmMMCardFile::GetInstance()->Mount();
		}
		else
		{
			Result = STORAGE_TYPE_USB;
		}
		if (g_wMMCStatus || g_wUSBStatus)
		{
			if (bConfirm)
				Prompt(g_MultiLanguage["VM_DATASAVING"]);
			else
				Prompt(g_MultiLanguage["VM_DATA_READING"]);
		}
#else
		Result = STORAGE_TYPE_USB;
#endif
		break;
	case 2:
		Result = STORAGE_TYPE_INET;
		break;
	default:
		break;
	}
	return Result;
}

DATE	tmDATE2DATE(tmDATE tmdate)
{
	DATE  	date;
	date.day		= tmdate.day;
	date.month		= tmdate.month;
	date.year		= tmdate.year;
	date.dayofweek	= tmdate.dayofweek;
	date.reserved	= 0;
	return date;
}

void	SaveCurrentMold()
{
	DB_MOLDHDR tempMoldHead;
	memset(&tempMoldHead, 0, sizeof(tempMoldHead));
	tmDATE			dtDate;
	char* 		ctemp;

	char	sztempMold[MAX_MOLDNAME_LENGTH], sztempMaterial[20], sztempColor[20];

	if (pwndEditNewName != NULL)
		pwndEditNewName->GetPropValueT("text", sztempMold, sizeof(sztempMold));
	if (sztempMold == NULL || sztempMold[0] == '\0')	return;
	if (pwndEditNewMaterial != NULL)
		pwndEditNewMaterial->GetPropValueT("text", sztempMaterial, sizeof(sztempMaterial));
	if (pwndEditNewColor != NULL)
		pwndEditNewColor->GetPropValueT("text", sztempColor, sizeof(sztempColor));

	ctemp = CtmMoldSet::GetInstance()->GetCurrentMold();

	GetDate(&dtDate);//獲取日期
	tempMoldHead.cUnitCount = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
	tempMoldHead.DateSave	= tmDATE2DATE(dtDate);
	strncpy(tempMoldHead.acID, ctemp, sizeof(tempMoldHead.acID));

#ifdef D_NOWRITE_FALSH
	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
	if (GetFileType(CHECKFILETYPE))
		nStorageType = STORAGE_TYPE_FLASH;
	if (CtmMoldSet::GetInstance()->Read(tempMoldHead, nStorageType))
#else
	if (CtmMoldSet::GetInstance()->Read(tempMoldHead, STORAGE_TYPE_SRAM));
#endif
	{
		strncpy(tempMoldHead.acID, sztempMold, sizeof(tempMoldHead.acID) - 1);
		strncpy(tempMoldHead.szMaterial , sztempMaterial, sizeof(tempMoldHead.szMaterial) - 1);
		strncpy(tempMoldHead.szColor, sztempColor, sizeof(tempMoldHead.szColor) - 1);
		tmDATE			dtDate;
		GetDate(&dtDate);
		tempMoldHead.cUnitCount = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
		tempMoldHead.DateSave	= tmDATE2DATE(dtDate);
#ifdef D_NOWRITE_FALSH
		tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
		if (GetFileType(CHECKFILETYPE))
			nStorageType = STORAGE_TYPE_FLASH;
		CtmMoldSet::GetInstance()->Save(tempMoldHead, nStorageType);
#else
		CtmMoldSet::GetInstance()->Save(tempMoldHead, STORAGE_TYPE_SRAM);
#endif
	}
}

//================================
//設置伺服器上限
//==================================
void	SetServiceView()
{
	if (!g_biNetStatus)
	{
		if (pwndEditDesti != NULL)
		{
			if (pwndEditDesti->Is("CtmEditX1"))
			{
				long long nMaxLimit = 1;
				pwndEditDesti->SetProperty("max", &nMaxLimit);
			}
			else if (pwndEditDesti->Is("CtmSelectEdit"))
			{
				int nMaxIndex = 2;
				pwndEditDesti->SetProperty("maxindex", &nMaxIndex);
			}
			pwndEditDesti->Update();
		}
	}
}


void	OnUpdateB(CtmWnd* pwndSender, void *pData)
{
	//接收iNet傳過來的模組資料
	FNET_MOLDHDRLIST* Fnet_moldhdrlist = (FNET_MOLDHDRLIST*)pData;
	//存到本地的SortMoldData區
	if (Fnet_moldhdrlist != NULL)
	{
		//if(Fnet_moldhdrlist->moldhdrlist[0].acID[0] !='\0')
		memset(TempMoldData, 0, sizeof(TempMoldData));		//James add 2008/7/22 將原來的buffer清空準備接收iNet傳過來的模組
		for (int i = 0; Fnet_moldhdrlist->moldhdrlist[i].acID[0] != '\0' ; i++)
		{
			if (i < MAX_MOLD_CNT)
				memcpy(&TempMoldData[i], &Fnet_moldhdrlist->moldhdrlist[i], sizeof(DB_MOLDHDR));	//轉存到緩存
		}

		//進行刷新排序
		//if(Fnet_moldhdrlist->moldhdrlist[0].acID[0] !='\0')
		UpdateDestination(pwndSender);
	}
}

//=====================================================
//	只在目標選擇Inet的時候調用一次，將Inet所有的模組請求過來
//=======================================================
void	GetNetMoldList(int StartNum)
{
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
		SendMsg(MSG_SH_REQUEST_INET_MOLD_LIST, StartNum, StartNum + nMoldCnt_Per_Page - 1, NULL);
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)		//2009-5-21
{
	char* pData = NULL;
	switch (message)
	{
	case MSG_MOLDSET_ADD:
//			MsgBox(g_MultiLanguage["VW_COMPLETED_DATA_SAVE"], tmFT_CODE_TECH);		//2010-7-16, mark for iNet Update
		break;
	case MSG_MOLDSET_OVERWRITE:
		MsgBox(g_MultiLanguage["PICKER_FILE_OVERWRITE"], tmFT_CODE_TECH);
		break;
	case MSG_INET_NETMLDCNT:	//	inet return the mold count
		nMaxCount		= 999;
		nDestiCount		= wParam;
		UpdateDestination(pwnd);
		break;
	case MSG_INET_NETHDRLIST:
		pData = (char*)lParam;
		nPageSourceCount	= wParam;	//2010-5-12, add, 當前頁顯示的模具數
		OnUpdateB(pwnd, pData);
		break;
	case MSG_SH_INET_DEAL_MOLD:		//fuxy, 2010-5-20, add, 在iNet儲存模具後，畫面上立即刷新顯示
		GetMoldCount();
		UpdateDestination(pwnd);
		break;
	default:
		break;
	}
}


//2012-3-27
/*---------------------------*/
int		WriteMoldSetIni(char*	pszPath)
{
	//CtmFile	tmfile;
	char	pszWritePath[256];
	int nFlag = 0;
	int	nIndex = 0, nMoldLength = 0, nMaterialLength = 0, nColorLength = 0, nMoldHDRLength = 0,
	    nMMIConfigLength = 0, nCom2TempSetLength = 0, nMoldSetLength = 0, nMoldSetBLength = 0,
	    nMonitorLength = 0, nMoldSetXLength = 0;
	char	pFileString[][256] =
	{
		"[ParameterSize]",			// 0
		"MoldLength=",				// 1
		"MaterialLength=",			// 2
		"ColorLength=",				// 3
		"MOLDHDR=",					// 4
		"MMICONFIG=",				// 5
		"COM2TEMPSET=",				// 6
		"MOLDSET=",					// 7
		"MOLDSETB=",				// 8
		"MONITOR=",					// 9
		"MOLDSETLENGTH="			// 10
	};

	if (g_pDatabase != NULL)
	{
		DB_DATAATTRIBUTE* pDataAttr;
		pDataAttr = g_pDatabase->GetAttr();

		if (pDataAttr != NULL)
		{
			//Mold Name Length
			nIndex = g_pDatabase->GetIndex("MHDR_MODULE_NULL_NULL_MODULENAME");
			if (nIndex > 0)
			{
				nMoldLength = pDataAttr[nIndex].DataType.wLength;
				//printf("nMoldLength=%d \n", nMoldLength);
			}
			//Material Length
			nIndex = g_pDatabase->GetIndex("MHDR_MODULE_NULL_NULL_MODULEMATERIAL");
			if (nIndex > 0)
			{
				nMaterialLength = pDataAttr[nIndex].DataType.wLength;
				//printf("nMaterialLength=%d \n", nMaterialLength);
			}
			//Color Length
			nIndex = g_pDatabase->GetIndex("MHDR_MODULE_NULL_NULL_MODULECOLOR");
			if (nIndex > 0)
			{
				nColorLength = pDataAttr[nIndex].DataType.wLength;
				//printf("nColorLength=%d \n", nColorLength);
			}

			//BLOCKDATA*	pBlockData = NULL;
			//char*		pDBDataID = 0;
			//MoldHDR Length
			nMoldHDRLength = g_pBlock->GetBlockSize(BLOCK_MOLDHDR_ID);

			//pBlockData	= g_pBlock->GetBlockData(BLOCK_MOLDHDR_ID);
			//for(int i = 0; i < pBlockData->dwFactorTotal; i++)
			//{
			//	pDBDataID = g_pDatabase->GetString(pBlockData->pwDBIndex[i]);
			//	printf(" BLOCK_MOLDHDR_ID pDBDataID=%s \n", pDBDataID);
			//}
			//printf("nMoldHDRLength=%d \n", nMoldHDRLength);
			//MMIConfigLength Length
			nMMIConfigLength = g_pBlock->GetBlockSize(BLOCK_MMICONFIG_ID);

			//pBlockData	= g_pBlock->GetBlockData(BLOCK_MMICONFIG_ID);
			//for(int i = 0; i < pBlockData->dwFactorTotal; i++)
			//{
			//	pDBDataID = g_pDatabase->GetString(pBlockData->pwDBIndex[i]);
			//	printf(" BLOCK_MMICONFIG_ID pDBDataID=%s \n", pDBDataID);
			//}
			//printf("nMMIConfigLength=%d \n", nMMIConfigLength);
			//nCom2TempSetLength Length
			nCom2TempSetLength = g_pBlock->GetBlockSize(63);	///Com2TempSet
			nCom2TempSetLength += g_pBlock->GetBlockSize(176);	//DB_COM2TEMPSETPERCENT
			nCom2TempSetLength += g_pBlock->GetBlockSize(177);	//DB_COM2GATE
			nCom2TempSetLength += g_pBlock->GetBlockSize(181);	//DB_COM2GATE2
			nCom2TempSetLength += g_pBlock->GetBlockSize(216);	//DB_COM2TEMPDEGREEOFFSET

			//pBlockData	= g_pBlock->GetBlockData(63);
			//for(int i = 0; i < pBlockData->dwFactorTotal; i++)
			//{
			//	pDBDataID = g_pDatabase->GetString(pBlockData->pwDBIndex[i]);
			//	printf(" 63 pDBDataID=%s value=%ld\n", pDBDataID, GetDBValue(pDBDataID).lValue);
			//}
			//printf("nMMIConfigLength=%d \n", nCom2TempSetLength);
			//nMoldSetLength Length
			nMoldSetLength = g_pBlock->GetBlockSize(BLOCK_MOLDSET_ID);

			//pBlockData	= g_pBlock->GetBlockData(BLOCK_MOLDSET_ID);
			//for(int i = 0; i < pBlockData->dwFactorTotal; i++)
			//{
			//	pDBDataID = g_pDatabase->GetString(pBlockData->pwDBIndex[i]);
			//	printf(" BLOCK_MOLDSET_ID pDBDataID=%s value=%ld\n", pDBDataID, GetDBValue(pDBDataID).lValue);
			//}
			//printf("nMoldSetLength=%d \n", nMoldSetLength);
			//nMoldSetBLength Length
			nMoldSetBLength = g_pBlock->GetBlockSize(BLOCK_MOLDSETB_ID);

			//pBlockData	= g_pBlock->GetBlockData(BLOCK_MOLDSETB_ID);
			//for(int i = 0; i < pBlockData->dwFactorTotal; i++)
			//{
			//	pDBDataID = g_pDatabase->GetString(pBlockData->pwDBIndex[i]);
			//	printf(" BLOCK_MOLDSETB_ID pDBDataID=%s value=%ld\n", pDBDataID, GetDBValue(pDBDataID).lValue);
			//}
			//printf("nMoldSetBLength=%d \n", nMoldSetBLength);
			//nMonitorLength Length
			nMonitorLength = g_pBlock->GetBlockSize(BLOCK_MONITOR_ID);

			//pBlockData	= g_pBlock->GetBlockData(BLOCK_MONITOR_ID);
			//for(int i = 0; i < pBlockData->dwFactorTotal; i++)
			//{
			//	pDBDataID = g_pDatabase->GetString(pBlockData->pwDBIndex[i]);
			//	printf(" BLOCK_MONITOR_ID pDBDataID=%s value=%ld\n", pDBDataID, GetDBValue(pDBDataID).lValue);
			//}
			//printf("nMonitorLength=%d \n", nMonitorLength);
			//nMoldSetXLength Length
			nMoldSetXLength = g_pBlock->GetBlockSize(BLOCK_MOLDSETX_ID);
			//printf("nMoldSetXLength=%d \n", nMoldSetXLength);
		}
	}

	memset(pszWritePath, 0, sizeof(pszWritePath));
	sprintf(pszWritePath, "%s%s", pszPath, "moldset.ini");
	nFlag = tmfile.Open(pszWritePath, O_RDWR | O_CREAT | O_SYNC | O_TRUNC);
	if (nFlag == tmFILE_GOOD)
	{
		sprintf(pFileString[0], "%s\r\n", pFileString[0]);
		tmfile.Write(pFileString[0], strlen(pFileString[0]));
		sprintf(pFileString[1], "%s%d\r\n", pFileString[1], nMoldLength);
		tmfile.Write(pFileString[1], strlen(pFileString[1]));
		sprintf(pFileString[2], "%s%d\r\n", pFileString[2], nMaterialLength);
		tmfile.Write(pFileString[2], strlen(pFileString[2]));
		sprintf(pFileString[3], "%s%d\r\n", pFileString[3], nColorLength);
		tmfile.Write(pFileString[3], strlen(pFileString[3]));
		sprintf(pFileString[4], "%s%d\r\n", pFileString[4], nMoldHDRLength);
		tmfile.Write(pFileString[4], strlen(pFileString[4]));
		sprintf(pFileString[5], "%s%d\r\n", pFileString[5], nMMIConfigLength);
		tmfile.Write(pFileString[5], strlen(pFileString[5]));
		sprintf(pFileString[6], "%s%d\r\n", pFileString[6], nCom2TempSetLength);
		tmfile.Write(pFileString[6], strlen(pFileString[6]));
		sprintf(pFileString[7], "%s%d\r\n", pFileString[7], nMoldSetLength);
		tmfile.Write(pFileString[7], strlen(pFileString[7]));
		sprintf(pFileString[8], "%s%d\r\n", pFileString[8], nMoldSetBLength);
		tmfile.Write(pFileString[8], strlen(pFileString[8]));
		sprintf(pFileString[9], "%s%d\r\n", pFileString[9], nMonitorLength);
		tmfile.Write(pFileString[9], strlen(pFileString[9]));
		sprintf(pFileString[10], "%s%d\r\n", pFileString[10], nMoldSetXLength);
		tmfile.Write(pFileString[10], strlen(pFileString[10]));
		tmfile.Close();
		return 0;
	}
	else
	{
		printf("save error \n");
		return -1;
	}
}


void PageUp(CtmWnd* pwndSender)
{
	if (((nStartValue - nMoldCnt_Per_Page) <= 0) && nStartValue > 1)		//iNet也適用
	{
		nStartValue = 1;
	}
	else if (nStartValue == 1)
	{
		return;
	}
	else
	{
		nStartValue = nStartValue - nMoldCnt_Per_Page;
	}

	GetMoldCount();		//2009-5-21
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
		GetNetMoldList(nStartValue);
	else
		UpdateDestination(pwndButtonPageUp);

	((CtmFormView*)pwndSender)->Goto(pwndButtonPageUp->GetTabOrder());
}

void	PageDown(CtmWnd* pwndSender)
{
	int i = 0;
	if (m_nDestiMedia == STORAGE_TYPE_INET)
	{
		for (i = 0; SortMoldData[i].acID[0] != '\0'; i++);
		if ( i < nMoldCnt_Per_Page)		//不足一頁
		{
			return;
		}
		else
		{
			nStartValue += nMoldCnt_Per_Page;
		}
	}
	else
	{
		if ( (nStartValue + nMoldCnt_Per_Page) > CtmMoldSet::GetInstance()->GetMaxMoldSetNum ())
		{
			nStartValue = CtmMoldSet::GetInstance()->GetMaxMoldSetNum () - nMoldCnt_Per_Page;
		}
		else if ((nStartValue + nMoldCnt_Per_Page) > nDestiCount)
		{
			return;
		}
		else
		{
			nStartValue = nStartValue + nMoldCnt_Per_Page;
		}
	}

	GetMoldCount();		//2009-5-21
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_biNetStatus)
		GetNetMoldList(nStartValue);
	else
		UpdateDestination(pwndButtonPageDown);

	((CtmFormView*)pwndSender)->Goto(pwndButtonPageDown->GetTabOrder());
}

int		CheckFileExist(char*	pszName)
{
	int nFlag = 0;
	nFlag = tmfile.Open(pszName, O_RDONLY);
	if (nFlag == tmFILE_GOOD)
	{
		tmfile.Close();
		return 1;
	}
	else
	{
		return 0;
	}
}

int		CopyFile(char*	pszSrc,	char*	pszDst)
{
	char	pszCmd[512];

	memset(pszCmd, 0, sizeof(pszCmd));
	sprintf(pszCmd, "cp %s %s ", pszSrc, pszDst);
	//printf("pszCmd=%s \n", pszCmd);
	usleep(10 * 1000);
	system(pszCmd);
	usleep(10 * 1000);
	return 1;
}

void	ReadMldCrlData()
{
	char	szPath[256];
	char	m_szCopyFile[256];
	char	pszDB[256];

	memset(pszDB, 0, sizeof(szPath));
	memset(szPath, 0, sizeof(szPath));
	memset(m_szCopyFile, 0, sizeof(m_szCopyFile));
	sprintf(szPath, "%s", "/usr/Data/MldCrl");
	ExistDir(szPath);
	sprintf(m_szCopyFile, "%s/mldcrl.cdb", szPath);
	int nFd =  open(m_szCopyFile, O_RDWR);

	if (nFd >= 0)
	{
		int nSize = lseek(nFd, 0, SEEK_END);

		if (nSize > 0)
		{
			lseek(nFd, 0, SEEK_SET);
			for (int i = 0; i < nSize; i++)
				read(nFd, &lTimeData, sizeof(lTimeData));
		}
	}
	else
	{
		lTimeData[0] = 1;
		lTimeData[1] = 0;
	}
	close(nFd);
}
