/*===========================================================================+
|  Class    : ViewMldR library                                               |
|  Task     : ViewMldR library action source file                            |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans		                                                   	 	 |
|  Version  : V1.00                                                          |
|  Creation : 			                                                  		   |
|  Revision :           			                                      			   |
+===========================================================================*/
#include 	"ViewMldR.h"
#include	"../database.h"
#include 	"../commonaction.h"
#include 	"../dialog.h"
#include 	"../utils.h"
#include 	"../moldControl.h"
#include 	"../selectedit.h"
#include 	"../main.h"
#include	"../lib/libiNetComm.h"
#include 	"../tmshmsg.h"
#include 	"../tmconfig.h"
#include	"../iotable.h"

/*===========================================================================+
|           Named rule Help                                                  |
+===========================================================================*/
// 模具頭表         --  editSourceTable
//     序號             --  editSourceTable__1: example -- editSourceTable001, editSourceTable011, ... ...
//     模具名稱         --  editSourceTable__2: example -- editSourceTable002, editSourceTable012, ... ...
//     存儲日期         --  editSourceTable__3: example -- editSourceTable003, editSourceTable013, ... ...
//     顏色             --  editSourceTable__4: example -- editSourceTable004, editSourceTable014, ... ...
//     材料             --  editSourceTable__5: example -- editSourceTable005, editSourceTable015, ... ...
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     MAX_EDITTABLE_ROWS     21
#define     MAX_EDITTABLE_COLS     6
#define     STR_EDITTABLE_SNAME    "editSourceTable"
#define     MAX_EDITTABLE_SIZE     50
#define     MAX_MOLDNAME_LENGTH    32+1		//fuxy, 2011-11-18 for Mold 32

#define		MOLD_INDEX				1	//模具的列序號
#define		ATERIAL_INDEX			3	//模具材料的列序號
#define		COLOR_INDEX				4	//模具顏色的列序號

typedef     WORD                   tmMEDIATYPE;

#define		PAGEUP_KEY			0x01
#define		PAGEDOWN_KEY		0x02

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*     pwndEditSource		= NULL;
CtmWnd*		pwndButtonPageDown	= NULL;
CtmWnd*		pwndButtonPageUp	= NULL;
CtmWnd*     pwndEditConfirm		= NULL;
CtmWnd*     pwndEditSourceMax	= NULL;
CtmWnd*     pwndEditSourceRemain = NULL;
CtmWnd*     pwndEditReadMoldNO	= NULL;
CtmWnd*     pwndEditSortType	= NULL;
CtmWnd*     pwndEditCurrentMold		= NULL;

CtmWnd*     pwndButton_Save		= NULL;
CtmWnd*     pwndButton_Del		= NULL;

CtmWnd*     pwndEditSourceTable[MAX_EDITTABLE_ROWS * MAX_EDITTABLE_COLS];

short       nSourceTableRows = 0;
short       nSourceTableEditCount[MAX_EDITTABLE_ROWS];
char        m_acEditNameTemp[MAX_EDITTABLE_SIZE];
char        m_szSourceTable[MAX_MOLD_CNT * MAX_EDITTABLE_COLS][MAX_MOLDNAME_LENGTH];

int      	nMaxCount 	= 0, nSourceCount = 0;	//最大儲存量,   已儲存的模具數
int			nStartValue = 1, nPageSourceCount 	= 1;	//在本頁顯示的模具（起始NO和個數）
int	 		nReadMoldNo	= 0; 					//讀取操作的模具NO.
int			nIndex		= 0, nIndexOld 	= -1;
int     bTempUnitFlag = 0, nTempUnitChge = 0, nTempUnitChgeOld = 0;

BOOL		m_bEnterKey 	= FALSE;
BOOL		m_bCheckKey 	= FALSE;
BOOL		bGetNetMold 	= FALSE;
BOOL 		bPriv = FALSE;
BOOL		m_biNet = 0;

STROAGETYPE m_nSourceMedia		= (STROAGETYPE)0;
STROAGETYPE m_nSourceMedia_old 	= (STROAGETYPE)0 ;//yuc<080118>

enum    tagtmMEDIATYPE
{
	tmMEDIA_MMI,
	tmMEDIA_SERVER,
	tmMEDIA_DISK
};

enum    tagtmMOLDSORTTYPE
{
	tmMOLD_SORT_NAME,
	tmMOLD_SORT_DATE,
	tmMOLD_SORT_MATER,
	tmMOLD_SORT_COLOR
};
static	char*	g_MACH_SetTempID[] = {      //zsq 2015-2-2 for 7HX TempUnitExchange
	"MACH_ALARM_ON_NULL_MAXOILTMPOFFSET",
	"MACH_ALARM_ON_NULL_MINOILTMPOFFSET",
	"MACH_ALARM_ON_NULL_TMPCTCHECKDEGREE",//電熱連續加溫檢查度數
	"MACH_TMPOIL_COOLERON_NULL_DEGREEOFFSET",//油溫冷卻器
	"MACH_TMPOIL_COOLEROFF_NULL_DEGREEOFFSET",
	"MACH_TMP1_HEATEROFF_WARM_DEGREE",//保溫溫度
};
static	char*	g_MACH_TempUpLowID[] = {       //zsq 2015-2-2 for 7HX TempUnitExchange
	"MACH_ALARM_ON_NULL_MAXTMPOFFSET",
	"MACH_ALARM_ON_NULL_MINTMPOFFSET",
	"MACH_TMP1_COOLERON_NULL_DEGREEOFFSET",
	"MACH_TMP1_COOLEROFF_NULL_DEGREEOFFSET",
};

extern	BOOL	g_biNetStatus;
STROAGETYPE		TOStorageType(int type);
//  << Down  lines for sort only :yuc20080114

#define 	MAX_MOLD_PER_INET_PAGE 		10		//fuxy, 2011-11-18, add for Mold32
#define 	MAX_MOLD_PER_PAGE 		10 // 20

DB_MOLDHDR 	SortMoldData[MAX_MOLD_CNT];			//本地臨時緩存，用于排序
DB_MOLDHDR 	TempMoldData[MAX_MOLD_CNT];			//本地臨時緩存，用于排序 存儲iNet模組
int 	m_SortType		= SORT_BY_DATE;			//默認排序類型
int		m_SortType_old	= SORT_BY_DATE;
int		nMoldCnt_Per_Page = 0;
int		nMoldReadPri = 0, g_iPriOld = 0;
int     lTimeData[2] = {0};
// >>
BOOL 	m_b7TR = FALSE;

// cjlee add ↓ 2019/2/23 下午 03:02:36

CtmWnd*   pwndBtn_SortType	= NULL;
BOOL 			b_SortType = 0;
char*	Str_Sort[] =
{
	"VW_MOLD_DATE",
	"VW_MOLD_NAME",
};

CtmWnd*   pwndBtn_SourceMedia	= NULL;
BOOL 			b_SourceMedia = 0;
int				n_SourceMedia = 0;
char*	Str_SourceMedia[] =
{
	"VW_MOLD_MMISAVE",
	"VW_MOLD_MEMORYCARD",
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
	int		ntemp = 0;
	m_biNet = CtmConfig::GetInstance()->GetINETFlag();//wangli2015-3-4 15:07:27

	pwndEditSource			= pwndSender->FindControlFromName("editSOURCEMEDIA");
	pwndEditSourceMax     	= pwndSender->FindControlFromName("editSOURCEMAX");
	pwndEditSourceRemain  	= pwndSender->FindControlFromName("editSOURCEREMAIN");
	pwndButtonPageDown		= pwndSender->FindControlFromName("buttonDOWNTWENTY");
	pwndButtonPageUp	 	= pwndSender->FindControlFromName("buttonUPTWENTY");
	pwndEditReadMoldNO		= pwndSender->FindControlFromName("editREADMOLDNO");
	pwndEditConfirm       	= pwndSender->FindControlFromName("editCONFIRM");
	pwndEditSortType      	= pwndSender->FindControlFromName("editMOlDSORTTYPE");
	pwndEditCurrentMold    	 = pwndSender->FindControlFromName("editCURRENTMOLD");

	pwndButton_Save	 = pwndSender->FindControlFromName("Button_Save");
	pwndButton_Del  = pwndSender->FindControlFromName("Button_Del");

	bTempUnitFlag = g_pExTable->GetTableValue(0, "D_TEMPUNITEX"); //zsq 2015-2-2 for 7HX 溫度單位轉換 華氏
	nTempUnitChgeOld = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue;  //zsq 2015-2-2 for 7HX 溫度單位轉換 0-攝氏 1-華氏

	m_b7TR 		= g_pExTable->GetTableValue(0, "D_7TR");

	pwndBtn_SortType  = pwndSender->FindControlFromName("BtnSortType"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtn_SourceMedia  = pwndSender->FindControlFromName("BtnSourceMedia"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtnConfirm  = pwndSender->FindControlFromName("BtnConfirm"); // cjlee add 2019/2/23 上午 08:43:25
	



///	if(CtmConfig::GetInstance()->GetINETFlag() == 1)		//fuxy, 2011-11-21, for Mold 32
///		nMoldCnt_Per_Page = MAX_MOLD_PER_INET_PAGE;
///	else
	nMoldCnt_Per_Page = MAX_MOLD_PER_PAGE;

	if (!g_pExTable->GetTableValue(0, "D_7LL"))
	{
		if (m_biNet)
		{
			ReadMldCrlData();
			g_iPriOld = g_iPrivilege;
			nMoldReadPri = lTimeData[1];
			if (nMoldReadPri)
			{
				if (g_iPrivilege < nMoldReadPri)
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
		else
		{

			if (g_iPrivilege == 0)
			{
				g_iPrivilege = 1;
				bPriv = TRUE;
			}
		}
	}


	for (i = 0; i < MAX_EDITTABLE_ROWS; i++)
	{
		for (j = 0; j < MAX_EDITTABLE_COLS; j++)
		{
			pwndEditSourceTable[i * MAX_EDITTABLE_ROWS + j] = NULL;
		}
	}

	for (i = 0; i < (MAX_EDITTABLE_ROWS - 1); i++)
	{
		sprintf(msz, "%02d", i);
		strcpy(m_acEditNameTemp, STR_EDITTABLE_SNAME);
		strcat(m_acEditNameTemp, msz);
		//printf("m_acEditNameTemp = %s\n",m_acEditNameTemp);
		nSourceTableEditCount[i] = GetSpecialControlNum(pwndSender, m_acEditNameTemp, "CtmEdit", &pwndEditSourceTable[MAX_EDITTABLE_COLS * i]);
		//printf("nSourceTableEditCount[%d]=%d\n",i,nSourceTableEditCount[i]);
		if (nSourceTableEditCount[i] > 0)    nSourceTableRows++;
	}
	//printf("nSourceTableRows = %d\n",nSourceTableRows);

//	if (pwndEditSource != NULL) // 來源
//	{
//		if (pwndEditSource->Is("CtmEditX1"))	//Sunny<20070530>
//			pwndEditSource->GetPropValueT("value", &ntemp, sizeof(ntemp));
//		else if (pwndEditSource->Is("CtmSelectEdit"))
//			ntemp = ((CtmSelectEdit*)pwndEditSource)->GetIndex();
//		m_nSourceMedia_old = m_nSourceMedia = TOStorageType(ntemp);		//yuc<080118>
//		printf("Source from %d\n",m_nSourceMedia);
//	}

// cjlee 2019/2/23 下午 03:29:17
	if (pwndBtn_SourceMedia != NULL) // 來源
	{
		ntemp = n_SourceMedia;
		m_nSourceMedia_old = m_nSourceMedia = TOStorageType(ntemp);		//yuc<080118>
		printf("Source from %d\n",m_nSourceMedia);
	}

	if(pwndBtn_SortType != NULL)
		{
			m_SortType = b_SortType;
			//Update string
			pwndBtn_SortType->SetPropValueT("captionID",Str_Sort[m_SortType]);	
			pwndBtn_SortType->CreateA();
			pwndBtn_SortType->Update();
		}
	if(pwndBtn_SourceMedia != NULL)
		{
			n_SourceMedia = b_SourceMedia;
			m_nSourceMedia = TOStorageType(n_SourceMedia);
			//Update string
			pwndBtn_SourceMedia->SetPropValueT("captionID",Str_SourceMedia[n_SourceMedia]);	
			pwndBtn_SourceMedia->CreateA();
			pwndBtn_SourceMedia->Update();
		}	
// cjlee 2019/2/23 下午 03:29:17

	if (pwndEditCurrentMold != NULL)		//fuxy, 2011-11-17, add for mold32
	{
		pwndEditCurrentMold->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
		pwndEditCurrentMold->Update();
	}

	//GetNetMoldList();
	CreateSet(pwndSender);
	SetServiceView();									//設置伺服器是否可用

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
	if (!g_pExTable->GetTableValue(0, "D_7LL"))
	{
		if (bPriv && g_iPrivilege != 8) /*Dongxl2015-11-17 添加8權級判斷避免進機器和組態時權級被更改  */
			g_iPrivilege = g_iPriOld;
		//g_iPrivilege = 0;
	}
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKeyA\n");
//	switch(wKey)
//	{
//		case 97:
//			::PutCommand("Hand.txt");
//			break;
//		case 98:
//			::PutCommand("MldS.txt");
//			break;
//		case 99:
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

	if (pwndSender->Is("CtmFormView")) 	return ((CtmFormView*)pwndSender)->OnKey1(wKey);
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
	int 		nSourceMedia = 0;
	int 		iTemp = 0, nTemp = 0;
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_Save)
	{
		::PutCommand("MldS.txt");
	}
	if(pwnd ==pwndButton_Del)
	{
		::PutCommand("MldD.txt");
	}

// cjlee add 2019/2/23 下午 04:05:57
	if (pwnd == pwndBtn_SourceMedia) // 來源
	{
		b_SourceMedia = !b_SourceMedia;
		n_SourceMedia = b_SourceMedia;
		m_nSourceMedia = TOStorageType(n_SourceMedia);
		//Update string
		pwndBtn_SourceMedia->SetPropValueT("captionID",Str_SourceMedia[n_SourceMedia]);
		pwndBtn_SourceMedia->CreateA();
		pwndBtn_SourceMedia->Update();
		
		if (m_nSourceMedia != m_nSourceMedia_old)
		{
			nStartValue = 1;
#ifdef	D_PXA270
			if ( m_nSourceMedia == STORAGE_TYPE_USB )
#else
			if ( m_nSourceMedia == STORAGE_TYPE_MMCARD )
#endif
			{
				Prompt(g_MultiLanguage["VM_DATA_READING"]);
			}

			GetMoldCount();		//2009-5-21
			if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList();			//James modify 2008/7/22 選擇目標為iNet的時候，從iNet讀取模組
			else
				UpdateSource(pwnd);
			m_nSourceMedia_old = m_nSourceMedia;	//yuc<080118>
		}
	}

	else  if (pwnd == pwndBtn_SortType) // 排序方式
	{
		b_SortType = !b_SortType;
		m_SortType = b_SortType;
		//Update string
		pwndBtn_SortType->SetPropValueT("captionID",Str_Sort[m_SortType]);
		pwndBtn_SortType->CreateA();
		pwndBtn_SortType->Update();
			
		if (m_SortType != m_SortType_old)	//if Changed . yuc<080118>
		{
			nStartValue = 1;
			GetMoldCount();			//2009-5-21
			if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList();
			else
				UpdateSource(pwnd);
			m_SortType_old = m_SortType;
		}
	}
	else  if (pwnd == pwndBtnConfirm) // 確認
	{
		{
			//Sunny<20080214><<<<< 增加判斷Msgbox跳出時是否key下確認鍵，否則不做以下讀取操作
			MsgBox(g_MultiLanguage["PICKER_FILE_LOADFILE"], tmFT_CODE_TECH);
			if (!g_bMsgBoxFlag)	return wIDControl;
			//Sunny<20080214>>>>>>
			Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"], 1); //Andy 20071008
			ReadMold(n_SourceMedia);

//        	    if(g_pExTable->GetTableValue(0,"D_OPN1PERCENT"))//tyl 2014.09.22 for 7LK
//				{
//		  	 		 long Opn1spd = GetDBValue("MSET_MLD_OPEN_STEP1_FLOW").lValue;
//		  	 		 SetDBValue("MACH_OTHR_OTHR_NULL_W03B9",Opn1spd);
//				}

			SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 0);		//fuxy, 2010-6-3, 自動監測清為不用
			if (CtmConfig::GetInstance()->GetDSP28Flag() == 1)
			{
				char* pszID;
				if (g_ptaskdsp28 != NULL)
					g_ptaskdsp28->WriteValue(9, pszID); 	//Andy 20090512 Send to DSP28
			}

			//if(g_biNetStatus)
			//	SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT", 0);	//iNet, 讀取模具後，預計產量清0;
			if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList();
			else
				UpdateSource(pwndSender);
			//James modify 2008/4/16 07:05下午   多國語言轉換
			char	szTitle[256];
			//pwndSender->GetPropValueT("title", szTitle, sizeof(szTitle));
			//printf("szTitle=%s\n",szTitle);
			//printf("CurrentMold=%s\n",CtmMoldSet::GetInstance()->GetCurrentMold());
			strcpy(szTitle, CtmMoldSet::GetInstance()->GetCurrentMold());
			g_pMainFrame->SetTitle2(szTitle);				

			if (pwndEditCurrentMold != NULL)		//fuxy, 2011-11-17, add for mold32
			{
				pwndEditCurrentMold->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
				pwndEditCurrentMold->Update();
			}
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
  return wIDControl;	
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)		//ycfan 20080129
{
	printf("OnchangeBM\n");
	int 		nSourceMedia = 0;
	int 		iTemp = 0, nTemp = 0;
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if (pwnd == NULL)	return wIDControl;

	if (pwnd == pwndEditSource) // 來源
	{
		if (pwnd->Is("CtmEditX1"))
			pwnd->GetPropValueT("value", &iTemp, sizeof(iTemp));
		else if (pwnd->Is("CtmSelectEdit"))
			iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
		m_nSourceMedia = TOStorageType(iTemp); //if Changed . yuc<080118>
		printf("Source from %d\n",m_nSourceMedia);
		
		if (m_nSourceMedia != m_nSourceMedia_old)
		{
			nStartValue = 1;
#ifdef	D_PXA270
			if ( m_nSourceMedia == STORAGE_TYPE_USB )
#else
			if ( m_nSourceMedia == STORAGE_TYPE_MMCARD )
#endif
			{
				Prompt(g_MultiLanguage["VM_DATA_READING"]);
			}

			GetMoldCount();		//2009-5-21
			if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList();			//James modify 2008/7/22 選擇目標為iNet的時候，從iNet讀取模組
			else
				UpdateSource(pwnd);
			m_nSourceMedia_old = m_nSourceMedia;	//yuc<080118>
		}
	}
	else  if (pwnd == pwndEditSortType) // 排序方式
	{
		if (pwnd->Is("CtmSelectEdit"))
			m_SortType = ((CtmSelectEdit*)pwnd)->GetIndex();
		if (m_SortType != m_SortType_old)	//if Changed . yuc<080118>
		{
			nStartValue = 1;
			GetMoldCount();			//2009-5-21
			if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
				GetNetMoldList();
			else
				UpdateSource(pwnd);
			m_SortType_old = m_SortType;
		}
	}
	else  if (pwnd == pwndEditConfirm) // 確認
	{
		//if(m_bEnterKey)	//2013-6-21, mark for touch
		{
			if (pwndEditSource->Is("CtmEditX1"))
				pwndEditSource->GetPropValueT("value", &nSourceMedia, sizeof(nSourceMedia));
			else if (pwndEditSource->Is("CtmSelectEdit"))
				nSourceMedia = ((CtmSelectEdit*)pwndEditSource)->GetIndex();

			if (pwnd->Is("CtmEditX1"))						//Sunny<20070530>
				pwnd->GetPropValueT("value", &iTemp, sizeof(iTemp));
			else if (pwnd->Is("CtmSelectEdit"))
				iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();

			if (iTemp == 1)
			{
				nTemp = 0;
				if (pwnd->Is("CtmEditX1"))						//Sunny<20070530>
					pwnd->SetPropValueT("value", nTemp);
				else if (pwnd->Is("CtmSelectEdit"))
					((CtmSelectEdit*)pwnd)->SetIndex(nTemp);
				pwnd->Update();
				//Sunny<20080214><<<<< 增加判斷Msgbox跳出時是否key下確認鍵，否則不做以下讀取操作
				MsgBox(g_MultiLanguage["PICKER_FILE_LOADFILE"], tmFT_CODE_TECH);
				if (!g_bMsgBoxFlag)	return wIDControl;
				//Sunny<20080214>>>>>>
				Prompt(g_MultiLanguage["MSG_DISK_PROCESSING"], 1); //Andy 20071008
				ReadMold(nSourceMedia);

//        	    if(g_pExTable->GetTableValue(0,"D_OPN1PERCENT"))//tyl 2014.09.22 for 7LK
//				{
//		  	 		 long Opn1spd = GetDBValue("MSET_MLD_OPEN_STEP1_FLOW").lValue;
//		  	 		 SetDBValue("MACH_OTHR_OTHR_NULL_W03B9",Opn1spd);
//				}

				SetDBValue("STATE_ALARM_ONOFF_NULL_STATUS", 0);		//fuxy, 2010-6-3, 自動監測清為不用
				if (CtmConfig::GetInstance()->GetDSP28Flag() == 1)
				{
					char* pszID;
					if (g_ptaskdsp28 != NULL)
						g_ptaskdsp28->WriteValue(9, pszID); 	//Andy 20090512 Send to DSP28
				}

				//if(g_biNetStatus)
				//	SetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT", 0);	//iNet, 讀取模具後，預計產量清0;
				if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
					GetNetMoldList();
				else
					UpdateSource(pwndSender);
				//James modify 2008/4/16 07:05下午   多國語言轉換
				char	szTitle[256];
				//pwndSender->GetPropValueT("title", szTitle, sizeof(szTitle));
				//printf("szTitle=%s\n",szTitle);
				//printf("CurrentMold=%s\n",CtmMoldSet::GetInstance()->GetCurrentMold());
				strcpy(szTitle, CtmMoldSet::GetInstance()->GetCurrentMold());
				g_pMainFrame->SetTitle2(szTitle);				

				if (pwndEditCurrentMold != NULL)		//fuxy, 2011-11-17, add for mold32
				{
					pwndEditCurrentMold->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
					pwndEditCurrentMold->Update();
				}
			}
		}
	}
	else  if (pwnd == pwndEditReadMoldNO) // 讀取模具
	{
		pwnd ->GetPropValueT("value", &nReadMoldNo, sizeof(nReadMoldNo));
		UpdateSourceBar(pwnd);
	}
	nTempUnitChge = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue;  //zsq 2015-1-26  溫度單位轉換 0-攝氏 1-華氏
	if (bTempUnitFlag && (nTempUnitChge != nTempUnitChgeOld))
	{
		if (nTempUnitChge == 1)
		{
			g_MultiLanguage.SetMulStr("VW_AUSTONE_C", "VW_AUSTONE_FAHRTEMP");
			SetTempValueFahr();
		}
		else if (nTempUnitChge == 0)
		{
			g_MultiLanguage.SetMulStr("VW_AUSTONE_FAHRTEMP", "VW_AUSTONE_C");
			SetTempValueCels();
		}
		nTempUnitChgeOld = nTempUnitChge;
		SetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED9", nTempUnitChge);
		usleep(1000 * 1000);
		Prompt(g_MultiLanguage["MSG_DUMMY"]);
	}

	return wIDControl;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if (pwnd == NULL)	return wIDControl;

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
			PageUp(pwndSender);	//2013-5-28
		}
	}
	/*else  if (pwnd == pwndEditConfirm) //20150603 觸摸屏讀取模具完成后提示欄仍顯示讀取中，請稍後
	{
		((CtmFormView*)pwndSender)->Goto(pwndEditSource->GetTabOrder());
	}*/

	return wIDControl;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void       CreateSet(CtmWnd* pwndSender)
{
	printf("CreateSet\n");
	GetMoldCount();
	SetSource(pwndSender);
}

void	GetMoldCount()
{
	printf("GetMoldCount\n");
	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
		SendMsg(MSG_SH_REQUEST_INET_MOLD_COUNT, 0, 0, NULL);
	else
	{
		nMaxCount     = CtmMoldSet::GetInstance()->GetMaxMoldSetNum();
		nSourceCount  = CtmMoldSet::GetInstance()->GetMoldSetNum(m_nSourceMedia);
		printf("nMaxCount=%d, nSourceCount=%d\n",nMaxCount,nSourceCount);
		nPageSourceCount = (nMoldCnt_Per_Page < nSourceCount - nStartValue + 1) ? nMoldCnt_Per_Page : nSourceCount - nStartValue + 1;
		SetReadNoMax();
	}
}

void  SetSource(CtmWnd* pwndSender)
{
	printf("SetSource\n");
	int		i = 0, j = 0;
	int     StartIndex = 0;
	int		ntempStartIndex = 0;
	StartIndex 	= nStartValue;

	//設置最大存儲容量和剩余容量
	if (pwndEditSourceMax   != NULL)
	{
		pwndEditSourceMax->SetPropValueT("value", nMaxCount);
		pwndEditSourceMax->Update();
	}

	if (pwndEditSourceRemain != NULL)
	{
		pwndEditSourceRemain->SetPropValueT("value", nMaxCount - nSourceCount);
		pwndEditSourceRemain->Update();
	}

	// <<	 yuc080114
	//加載MODL到本地緩存，用于排序,同時使用排序類型和排序媒介
	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
	{
		memset(SortMoldData, 0, sizeof(SortMoldData));
		memcpy(SortMoldData, TempMoldData, sizeof(TempMoldData));
		for (i = 0; SortMoldData[i].acID[0] != '\0'; i++)
		{}
		//nSourceCount = i;
	}
	else		//fuxy, 2011-4-20, add
		LoadData(SortMoldData, m_nSourceMedia, nSourceCount, m_SortType);

	if (m_nSourceMedia != STORAGE_TYPE_INET)
		ntempStartIndex = StartIndex - 1;
	else
		ntempStartIndex = 0;
	//已使用臨時緩沖區代替直接從設備讀入
	for (i = 0; i < nSourceCount; i++)
	{
		//printf("SortMoldData[i + ntempStartIndex].acID[0] = %s\n",SortMoldData[i + ntempStartIndex].acID[0]);
		// 此循環內的代碼需要在每一個模具相關的代碼里修改
		if (SortMoldData[i + ntempStartIndex].acID[0] == '\0' ) //是否有資料
			break;
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 0], "%3d"           , i + StartIndex);
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 1], "%s"            , SortMoldData[i + ntempStartIndex].acID);
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 2], "%02d/%02d/%02d", SortMoldData[i + ntempStartIndex].DateSave.month, SortMoldData[i + ntempStartIndex].DateSave.day, SortMoldData[i + ntempStartIndex].DateSave.year % 100);
		SortMoldData[i].szMaterial[5] = '\0';
		SortMoldData[i].szColor[5] = '\0';
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 3], "%s"            , SortMoldData[i + ntempStartIndex].szMaterial);
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 4], "%s"            , SortMoldData[i + ntempStartIndex].szColor);
	}
	// >>
	for (i = nPageSourceCount; i < nSourceTableRows; i++)		//fuxy, 2010-5-20,  for (i = i; i < nSourceTableRows; i++)
	{
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 0], "%3d"           , i + StartIndex);
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 1], " ");
		sprintf(m_szSourceTable[i * MAX_EDITTABLE_COLS + 2], " ");
		strcpy (m_szSourceTable[i * MAX_EDITTABLE_COLS + 3], " ");
		strcpy (m_szSourceTable[i * MAX_EDITTABLE_COLS + 4], " ");
	}

	for (i = 0; i < nSourceTableRows; i++)
	{
		for (j = 0; j < nSourceTableEditCount[i]; j++)
		{
			if (pwndEditSourceTable[i * MAX_EDITTABLE_COLS + j] != NULL)
			{
				pwndEditSourceTable[i * MAX_EDITTABLE_COLS + j]->SetPropValueT("text", m_szSourceTable[i * MAX_EDITTABLE_COLS + j]);
			}
		}
	}
	strcpy(m_szSourceTable[MAX_EDITTABLE_COLS * (MAX_EDITTABLE_ROWS - 1)], g_MultiLanguage["VW_MOLD_NEWFILE"]);
	for (i = 1; i < nSourceTableEditCount[MAX_EDITTABLE_ROWS - 1]; i++)
		strcpy (m_szSourceTable[MAX_EDITTABLE_COLS * (MAX_EDITTABLE_ROWS - 1) + i], " ");

	for (i = 0; i < nSourceTableEditCount[MAX_EDITTABLE_ROWS - 1]; i++)
	{
		if (pwndEditSourceTable[(MAX_EDITTABLE_ROWS - 1)*MAX_EDITTABLE_COLS + i] != NULL)
		{
			pwndEditSourceTable[(MAX_EDITTABLE_ROWS - 1)*MAX_EDITTABLE_COLS + i]->SetPropValueT("text", m_szSourceTable[MAX_EDITTABLE_COLS * (MAX_EDITTABLE_ROWS - 1) + i]);
		}
	}
}

void  UpdateSource(CtmWnd* pwndSender)
{
	short       i = 0, j = 0;

	SetSource(pwndSender);
	if (pwndEditSourceMax    != NULL) pwndEditSourceMax->Update();
	if (pwndEditSourceRemain != NULL) pwndEditSourceRemain->Update();
	for (i = 0; i < nSourceTableRows; i++)
	{
		for (j = 0; j < nSourceTableEditCount[i]; j++)
		{
			if (pwndEditSourceTable[i * MAX_EDITTABLE_COLS + j] != NULL)
			{
				pwndEditSourceTable[i * MAX_EDITTABLE_COLS + j]->Update();
			}
		}
	}
}

void  UpdateSourceBar(CtmWnd* pwndSender)
{
	int         i = 0;
	FGC         FgcTemp;
	BGC         BgcTemp;
	FGC		  	fgc ;
	BGC 	  	bgc ;
	nIndex = nReadMoldNo - nStartValue;
	for (i = 0; i < MAX_EDITTABLE_COLS; i++)
	{
		if (pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndexOld + i] != NULL && nIndexOld >= 0)
		{
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndexOld + i]->Update();
		}
		if (pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i] != NULL && nIndex >= 0)
		{
			FgcTemp = LIGHTBLUE;
			BgcTemp = YELLOW;
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->GetPropValueT("fgc", &fgc, sizeof(fgc));
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->GetPropValueT("bgc", &bgc, sizeof(bgc));
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->SetPropValueT("fgc", FgcTemp);
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->SetPropValueT("bgc", BgcTemp);
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->Update();
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->SetPropValueT("fgc", fgc);
			pwndEditSourceTable[MAX_EDITTABLE_COLS * nIndex + i]->SetPropValueT("bgc", bgc);
		}
	}
	nIndexOld = nIndex;
}

void	ReadMold(int nSourceMedia)
{
	DB_MOLDHDR tempMoldHead;
	memset(&tempMoldHead, 0, sizeof(tempMoldHead));		//fuxy, 2012-2-22, add 初始化資料
	m_nSourceMedia = TOStorageType(nSourceMedia);
	printf("ReadMold(%d)\n",m_nSourceMedia);
	if (pwndEditReadMoldNO != NULL)
		pwndEditReadMoldNO -> GetPropValueT("value", &nReadMoldNo, sizeof(nReadMoldNo));

	nReadMoldNo -= 1; 	//yuc(load the right modle)last step
	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
		nReadMoldNo = nReadMoldNo % nMoldCnt_Per_Page;			//fuxy, 2010-7-6，每翻一頁，往後移20個(否則第二頁及以後已審核的模具，也不能讀取)

	strncpy(tempMoldHead.acID, SortMoldData[nReadMoldNo].acID, sizeof(tempMoldHead.acID) - 1);
	strncpy(tempMoldHead.szMaterial , SortMoldData[nReadMoldNo].szMaterial, sizeof(tempMoldHead.szMaterial) - 1);
	strncpy(tempMoldHead.szColor, SortMoldData[nReadMoldNo].szColor, sizeof(tempMoldHead.szColor) - 1);
	printf("tempMoldHead.acID=%s,tempMoldHead.szMaterial=%s,tempMoldHead.szColor=%s\n",tempMoldHead.acID,tempMoldHead.szMaterial,tempMoldHead.szColor);
	tmDATE			dtDate;
	GetDate(&dtDate);
	tempMoldHead.cUnitCount = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
	tempMoldHead.DateSave	= tmDATE2DATE(dtDate);
	printf("read mold:%d\n", nReadMoldNo);
	//printf("read mold:%d\n", SortMoldData[nReadMoldNo].Media);

	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus )//zholy070416
	{
		memcpy(g_MoldHead.acID, tempMoldHead.acID, sizeof(tempMoldHead.acID));		//2009-5-21
		memcpy(g_MoldHead.szMaterial, tempMoldHead.szMaterial, sizeof(tempMoldHead.szMaterial));
		memcpy(g_MoldHead.szColor, tempMoldHead.szColor, sizeof(tempMoldHead.szColor));
		SendMsg(MSG_SH_REQUEST_INET_MOLD, -1, (int)STORAGE_TYPE_SRAM, NULL);	//2009-12-30, Modify

		//SetDBValue("LCD_HMI_NULL_NULL_LCDBRIGHTERMIN", 1); 			//記錄模具資料為iNet模具
		SendMsg(MSG_SH_REV_INET_MOLD_LIMIT, 0, 0, NULL);	//2013-4-11, 申請模具上下限
		g_biNetLimit  = FALSE;	//Caozz add 2010/8/17   無模具上下限
	}
	else
	{
		CtmMoldSet::GetInstance()->SaveToCurrentMold(tempMoldHead, m_nSourceMedia);
		printf("CtmMoldSet::GetInstance()->SaveToCurrentMold()\n");

		if (g_biNetStatus)
			SendMsg(MSG_SH_INET_DEAL_MOLD, 0, 0, NULL);		//2009-12-30, Modify,INET_SEND_MOLDSET_RELATIVE_ID
		//SetDBValue("LCD_HMI_NULL_NULL_LCDBRIGHTERMIN", 0);
		
		if(m_b7TR)
		{
			if (((SortMoldData[nReadMoldNo].Media & 0x0001) != GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED89").lValue)
			    ||(((SortMoldData[nReadMoldNo].Media >> 1) & 0x0001) != GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED38").lValue)
			    ||(((SortMoldData[nReadMoldNo].Media >> 2) & 0x0001) != GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED88").lValue)) //位置/温度/压力
			{
				MsgBox(g_MultiLanguage["VW_MEASUREINCONSISTENT"], tmFT_CODE_TECH);
			}
		}
	}
	//g_biNetLimit  = FALSE;	//Caozz add 2010/8/17 10:54上午
	printf("ReadMold End\n");
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
			Prompt(g_MultiLanguage["VM_DATA_READING"]);
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


//================================
//設置伺服器上限
//==================================
void	SetServiceView()
{
	if (!g_biNetStatus)
	{
		if (pwndEditSource != NULL)
		{
			if (pwndEditSource->Is("CtmEditX1"))
			{
				long long nMaxLimit = 1;
				pwndEditSource->SetProperty("max", &nMaxLimit);
			}
			else if (pwndEditSource->Is("CtmSelectEdit"))
			{
				int nMaxIndex = 2;
				pwndEditSource->SetProperty("maxindex", &nMaxIndex);
			}
			pwndEditSource->Update();
		}
	}
}

void  	OnUpdateA(CtmWnd* pwndSender)
{
	if (bGetNetMold)
	{
		char	szTitle[256];
		pwndSender->GetPropValueT("title", szTitle, sizeof(szTitle));
		g_pMainFrame->SetTitle(szTitle);
		bGetNetMold = FALSE;

		if (pwndEditCurrentMold != NULL)		//fuxy, 2012-2-22, add for mold32 讀模具后, 刷新畫面中的當前模具
		{
			pwndEditCurrentMold->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
			pwndEditCurrentMold->Update();
		}
	}
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	char*	pData = NULL;
	switch (message)
	{
	case MSG_INET_NETHDRLIST:
		pData = (char*)lParam;
		nPageSourceCount	= wParam;		//2010-5-12, add,當前頁顯示的模具數
		OnUpdateB(pwnd, pData);
		SetReadNoMax();
		break;
	case MSG_INET_NETMLDCNT:
		nMaxCount		= 999;
		nSourceCount	= wParam;
		break;
	case MSG_SH_REV_INET_MOLD_EXIST:
		switch (wParam)
		{
		case	0:
			bGetNetMold	= TRUE;
			break;
		case	1:
			MsgBox(g_MultiLanguage["VW_MOLD_READERROR1"], tmFT_CODE_TECH);
			break;
		case	2:
			MsgBox(g_MultiLanguage["VW_MOLD_READERROR2"], tmFT_CODE_TECH);
			break;
		case	0x10000:
			MsgBox(g_MultiLanguage["DLG_MOLD_NOTMATCH"], tmFT_CODE_TECH);
			break;
		case	0x10001:
			break;
		case	0x10002:
			break;
		default:
			break;
		}
		break;
	default:
		break;
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
		UpdateSource(pwndSender);
	}
}


//=====================================================
//	只在目標選擇Inet的時候調用一次，將Inet所有的模組請求過來
//=======================================================
void	GetNetMoldList()
{
	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)		//2009-5-21
		SendMsg(MSG_SH_REQUEST_INET_MOLD_LIST, nStartValue, nStartValue + nMoldCnt_Per_Page - 1, NULL);	//iNet返回時，調用MSG_INET_NETHDRLIST,獲取模具數
}

void	SetReadNoMax()
{
	if (pwndEditReadMoldNO != NULL)
	{
		pwndEditReadMoldNO ->SetPropValueT("value", nStartValue);
		pwndEditReadMoldNO ->SetPropValueT("min", nStartValue);
		pwndEditReadMoldNO ->SetPropValueT("max", nPageSourceCount + nStartValue - 1);
		pwndEditReadMoldNO -> SetPropValueT("maxchar", 3);

		pwndEditReadMoldNO ->Update();
	}
}

void 	PageUp(CtmWnd* pwndSender)
{
	if ( ((nStartValue - nMoldCnt_Per_Page) <= 0) && nStartValue > 1)
	{
		nStartValue = 1;
	}
	else if (nStartValue == 1)
	{
		return ;
	}
	else
	{
		nStartValue = nStartValue - nMoldCnt_Per_Page;
	}

	GetMoldCount();
	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
		GetNetMoldList();
	else
		UpdateSource(pwndButtonPageUp);
	((CtmFormView*)pwndSender)->Goto(pwndButtonPageUp->GetTabOrder());
}

void	PageDown(CtmWnd* pwndSender)
{
	printf("PageDown\n");
	int i = 0;
	if (m_nSourceMedia == STORAGE_TYPE_INET)
	{
		for (i = 0; SortMoldData[i].acID[0] != '\0'; i++)
			;
		if ( i < nMoldCnt_Per_Page)		//不足一頁
		{
			return ;
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
		else if ((nStartValue + nMoldCnt_Per_Page) > nSourceCount)
		{
			printf("((nStartValue + nMoldCnt_Per_Page) > nSourceCount)\n");
			return;
		}
		else
		{
			nStartValue = nStartValue + nMoldCnt_Per_Page;
			printf("nStartValue = nStartValue + nMoldCnt_Per_Page\n");
			printf("%d = %d + %d\n",nStartValue,nStartValue,nMoldCnt_Per_Page);
		}
	}

	GetMoldCount();
	if (m_nSourceMedia == STORAGE_TYPE_INET && g_biNetStatus)
		GetNetMoldList();
	else
		UpdateSource(pwndButtonPageDown);
	((CtmFormView*)pwndSender)->Goto(pwndButtonPageDown->GetTabOrder());
}

void	SetTempValueFahr()
{
	long lTemp = 0;
	for (unsigned int i = 0; i < sizeof(g_MACH_SetTempID) / sizeof(g_MACH_SetTempID[0]); i++)
	{
		lTemp = GetDBValue(g_MACH_SetTempID[i]).lValue;
		if (lTemp != 0xFFFF)
			lTemp = (((lTemp * 90) / 5 + 320) + 5) / 10;
		SetDBValue(g_MACH_SetTempID[i], lTemp, FALSE);
	}
	for (unsigned int i = 0; i < sizeof(g_MACH_TempUpLowID) / sizeof(g_MACH_TempUpLowID[0]); i++)
	{
		lTemp = GetDBValue(g_MACH_TempUpLowID[i]).lValue;
		if (lTemp != 0xFFFF)
			lTemp = ((lTemp * 90 / 5) + 5) / 10;
		SetDBValue(g_MACH_TempUpLowID[i], lTemp, FALSE);
	}
}
void	SetTempValueCels()
{
	long lTemp = 0;
	for (unsigned int i = 0; i < sizeof(g_MACH_SetTempID) / sizeof(g_MACH_SetTempID[0]); i++)
	{
		lTemp = GetDBValue(g_MACH_SetTempID[i]).lValue;
		if (lTemp != 0xFFFF)
		{
			if (lTemp > 31)
				lTemp = (((lTemp - 32) * 50 / 9) + 5) / 10;
			else
				lTemp = 0;
		}
		SetDBValue(g_MACH_SetTempID[i], lTemp, FALSE);
	}
	for (unsigned int i = 0; i < sizeof(g_MACH_TempUpLowID) / sizeof(g_MACH_TempUpLowID[0]); i++)
	{
		lTemp = GetDBValue(g_MACH_TempUpLowID[i]).lValue;
		if (lTemp != 0xFFFF)
			lTemp = ((lTemp * 50 / 9) + 5) / 10;
		SetDBValue(g_MACH_TempUpLowID[i], lTemp, FALSE);
	}
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
