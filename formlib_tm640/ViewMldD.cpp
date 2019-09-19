/*===========================================================================+
|  Class    : ViewMldD library                                               |
|  Task     : ViewMldD library action source file                            |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include 	"ViewMldD.h"
#include 	"../commonaction.h"
#include 	"../dialog.h"
#include 	"../utils.h"
#include 	"../moldControl.h"
#include 	"../selectedit.h"
#include 	"../main.h"
#include	"../lib/libiNetComm.h"
#include 	"../tmshmsg.h"

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
CtmWnd*     pwndEditSource			= NULL;
CtmWnd*		pwndButtonPageUp		= NULL;
CtmWnd*		pwndButtonPageDown		= NULL;
CtmWnd*     pwndEditConfirm			= NULL;
CtmWnd*     pwndEditSourceMax		= NULL;
CtmWnd*     pwndEditSourceRemain	= NULL;
CtmWnd*     pwndEditDeleteMoldNO	= NULL;
CtmWnd*     pwndEditSortType		= NULL; 	//ycfan 20080115
CtmWnd*     pwndEditCurrentMold		= NULL;

CtmWnd*     pwndButton_Save		= NULL;
CtmWnd*     pwndButton_Read		= NULL;

CtmWnd*     pwndEditSourceTable[MAX_EDITTABLE_ROWS*MAX_EDITTABLE_COLS];

short       nSourceTableRows = 0;
short       nSourceTableEditCount[MAX_EDITTABLE_ROWS];
char        m_acEditNameTemp[MAX_EDITTABLE_SIZE];
char        m_szSourceTable[MAX_MOLD_CNT*MAX_EDITTABLE_COLS][MAX_MOLDNAME_LENGTH];

int		 	nMaxCount 	= 0, nSourceCount	= 0;      //最大儲存量,   已儲存的模具數
int			nStartValue = 1, nPageSourceCount 	= 1;	//在本頁顯示的模具（起始NO和個數）
int	 		nDeleteMoldNo	= 0;
int			nIndexOld    	= -1;
int			nMoldCnt_Per_Page = 0;	

STROAGETYPE m_nSourceMedia 	= (STROAGETYPE)0;
STROAGETYPE m_nSourceMedia_old = (STROAGETYPE)0;			// Check If Media Changed yuc<080118>

BOOL		m_bEnterKey = FALSE;
BOOL		m_bCheckKey = FALSE;

enum    tagtmMEDIATYPE
{
    tmMEDIA_MMI,
    tmMEDIA_SERVER,
    tmMEDIA_DISK
};

extern	BOOL	g_biNetStatus;	//2009-5-21
STROAGETYPE		TOStorageType(int type);
//  << Down  lines for sort only :yuc<080114>
#define 	MAX_MOLD_PER_INET_PAGE 		10		//fuxy, 2011-11-18, add for Mold32 
#define 	MAX_MOLD_PER_PAGE 	10//20		//此頁上下翻的最大個數
int 		m_SortType		= SORT_BY_DATE;
int 		m_SortType_old	= SORT_BY_DATE;			//默認排序類型 //Check if Sort type changed

DB_MOLDHDR SortModlData[MAX_MOLD_CNT*2+1];			//本地臨時緩存，用于排序
DB_MOLDHDR TempModlData[MAX_MOLD_CNT*2+1];			//本地臨時緩存，用于排序
// >>

// cjlee add ↓ 2019/2/23 下午 05:07:27
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

// cjlee add ↑ 2019/2/23 下午 05:07:31




/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
    short   i = 0, j = 0;
	char    msz[4];
	int		ntemp = 0;

    pwndEditSource         	= pwndSender->FindControlFromName("editSOURCEMEDIA");
    pwndEditSourceMax      	= pwndSender->FindControlFromName("editSOURCEMAX");
    pwndEditSourceRemain   	= pwndSender->FindControlFromName("editSOURCEREMAIN");
    pwndButtonPageDown	 	= pwndSender->FindControlFromName("buttonSOURCEDOWNTWENTY");
    pwndButtonPageUp	 	= pwndSender->FindControlFromName("buttonSOURCEUPTWENTY");
	pwndEditDeleteMoldNO	= pwndSender->FindControlFromName("editDELETEMOLDNO");
    pwndEditConfirm        	= pwndSender->FindControlFromName("editCONFIRM");
	pwndEditSortType       	= pwndSender->FindControlFromName("editMOlDSORTTYPE");//yuc
	pwndEditCurrentMold    	 = pwndSender->FindControlFromName("editCURRENTMOLD");

	pwndButton_Save	 = pwndSender->FindControlFromName("Button_Save");
	pwndButton_Read  = pwndSender->FindControlFromName("Button_Read");

	pwndBtn_SortType  = pwndSender->FindControlFromName("BtnSortType"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtn_SourceMedia  = pwndSender->FindControlFromName("BtnSourceMedia"); // cjlee add 2019/2/23 上午 08:43:25
	pwndBtnConfirm  = pwndSender->FindControlFromName("BtnConfirm"); // cjlee add 2019/2/23 上午 08:43:25
	


///	if(CtmConfig::GetInstance()->GetINETFlag() == 1)		//fuxy, 2011-11-21, for Mold 32 
///		nMoldCnt_Per_Page = MAX_MOLD_PER_INET_PAGE;
///	else
		nMoldCnt_Per_Page = MAX_MOLD_PER_PAGE; 

    for(i = 0; i < MAX_EDITTABLE_ROWS; i++)
    {
        for(j = 0; j < MAX_EDITTABLE_COLS; j++)
        {
            pwndEditSourceTable[i*MAX_EDITTABLE_ROWS+j] = NULL;
        }
    }

    for(i = 0; i < (MAX_EDITTABLE_ROWS - 1); i++)
    {
        sprintf(msz, "%02d", i);
        strcpy(m_acEditNameTemp, STR_EDITTABLE_SNAME);
        strcat(m_acEditNameTemp, msz);
        nSourceTableEditCount[i] = GetSpecialControlNum(pwndSender, m_acEditNameTemp, "CtmEdit", &pwndEditSourceTable[MAX_EDITTABLE_COLS*i]);
        if(nSourceTableEditCount[i] > 0)    nSourceTableRows++;
    }

//	if(pwndEditSource != NULL)
//   	{
//   		if(pwndEditSource->Is("CtmEditX1"))	//Sunny<20070530>
//   			pwndEditSource->GetPropValueT("value", &ntemp,sizeof(ntemp));
//   		else if(pwndEditSource->Is("CtmSelectEdit"))
//   			ntemp = ((CtmSelectEdit*)pwndEditSource)->GetIndex();
//   		m_nSourceMedia = TOStorageType(ntemp);
//   		m_nSourceMedia_old = TOStorageType(ntemp);
//   	}
	 // cjlee add 2019/2/23 下午 06:08:55
   if(pwndBtn_SourceMedia != NULL)
   	{
   		ntemp = n_SourceMedia;
   		m_nSourceMedia = TOStorageType(ntemp);
   		m_nSourceMedia_old = TOStorageType(ntemp);
   	}
   	
	CreateSet(pwndSender);
	
	if(pwndEditCurrentMold != NULL)		//fuxy, 2011-11-17, add for mold32 
	{
		pwndEditCurrentMold->SetPropValueT("text", CtmMoldSet::GetInstance()->GetCurrentMold());
		pwndEditCurrentMold->Update();
	}
	
	// cjlee add 2019/2/23 下午 06:07:15
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
	// cjlee add 2019/2/23 下午 06:07:22
	
	
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
}

WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
//	switch(wKey)
//	{
//		case 97:
//			::PutCommand("Hand.txt");
//			break;
//		case 98:
//			::PutCommand("MldS.txt");
//			break;
//		case 99:
//			::PutCommand("MldR.txt");
//			break;
//		case 100:
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
	if(wKey == _ENTER)
	{
		m_bEnterKey = TRUE;
	}
	else
	{
		m_bEnterKey = FALSE;
	}
	if(wKey == 46)
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
	int 	nSourceMedia = 0;
 	int 	iTemp =0, nTemp = 0;
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd ==pwndButton_Save)
	{
		::PutCommand("MldS.txt");
	}
	if(pwnd ==pwndButton_Read)
	{
		::PutCommand("MldR.txt");
	}
	
	// cjlee add 2019/2/23 下午 04:54:04	
	if(pwnd == pwndBtn_SourceMedia) // 來源
   	{
			b_SourceMedia = !b_SourceMedia;
			n_SourceMedia = b_SourceMedia;
			m_nSourceMedia = TOStorageType(n_SourceMedia);
			//Update string
			pwndBtn_SourceMedia->SetPropValueT("captionID",Str_SourceMedia[n_SourceMedia]);
			pwndBtn_SourceMedia->CreateA();
			pwndBtn_SourceMedia->Update();
   		
   		if(m_nSourceMedia != m_nSourceMedia_old)
   		{	
   			nStartValue = 1;
   			#ifdef	D_PXA270
   			if( m_nSourceMedia == STORAGE_TYPE_USB )
   			#else
   			if( m_nSourceMedia == STORAGE_TYPE_MMCARD )
   			#endif
   			{
   				Prompt(g_MultiLanguage["VM_DATA_READING"]);
   			}
   			
   			GetMoldCount();		//2009-5-21
   			UpdateSource(pwndSender);
  			m_nSourceMedia_old = m_nSourceMedia;
   		}
   	}
  else if(pwnd == pwndBtn_SortType) // 排序方式
	{
		b_SortType = !b_SortType;
		m_SortType = b_SortType;
		//Update string
		pwndBtn_SortType->SetPropValueT("captionID",Str_Sort[m_SortType]);
		pwndBtn_SortType->CreateA();
		pwndBtn_SortType->Update();
		
    if(m_SortType != m_SortType_old) //if changed.   yuc<080118>
    {
  		nStartValue = 1;
  		GetMoldCount();
  		m_SortType_old = m_SortType;	
			UpdateSource(pwndSender);
		}
	}
	else if(pwnd == pwndBtnConfirm) // 確認
	{
		{	   	
	   	//Sunny<20080214><<<<< 增加判斷Msgbox跳出時是否key下確認鍵，否則不做以下讀取操作
	   	MsgBox(g_MultiLanguage["PICKER_FILE_DELFILE"], tmFT_CODE_TECH);
	   	if(!g_bMsgBoxFlag)	return wIDControl;
	   	//Sunny<20080214>>>>>>
	   	
	    DeleteMold(nSourceMedia);
	    GetMoldCount();		//2009-5-21 
	    UpdateSource(pwndSender);
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
	// cjlee add 2019/2/23 下午 04:54:04
	
  return wIDControl;	
}

WORD	OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)		//ycfan 20080129
{
	int 	nSourceMedia = 0;
 	int 	iTemp =0, nTemp = 0;
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	if(pwnd == pwndEditSource)
   	{
   		if(pwnd->Is("CtmEditX1"))
   			pwnd->GetPropValueT("value", &iTemp,sizeof(iTemp));
   		else if(pwnd->Is("CtmSelectEdit"))
   			iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
		m_nSourceMedia = TOStorageType(iTemp);
  		
   		if(m_nSourceMedia != m_nSourceMedia_old)
   		{	
   			nStartValue = 1;
   			#ifdef	D_PXA270
   			if( m_nSourceMedia == STORAGE_TYPE_USB )
   			#else
   			if( m_nSourceMedia == STORAGE_TYPE_MMCARD )
   			#endif
   			{
   				Prompt(g_MultiLanguage["VM_DATA_READING"]);
   			}
   			
   			GetMoldCount();		//2009-5-21
   			UpdateSource(pwndSender);
  			m_nSourceMedia_old = m_nSourceMedia;
   		}
   	}
    else if(pwnd == pwndEditSortType)
	{
		if(pwnd->Is("CtmSelectEdit"))
    		m_SortType = ((CtmSelectEdit*)pwnd)->GetIndex();
    	if(m_SortType != m_SortType_old) //if changed.   yuc<080118>
    	{
    		nStartValue = 1;
    		GetMoldCount();
    		m_SortType_old = m_SortType;	
			UpdateSource(pwndSender);
		}
	}
	else if(pwnd == pwndEditDeleteMoldNO)
	{
		pwnd 	-> GetPropValueT("value", &nDeleteMoldNo,sizeof(nDeleteMoldNo));
		UpdateSourceBar(pwndSender);
	}
	else if(pwnd == pwndEditConfirm)
	{
		//if(m_bEnterKey)	//2013-6-21, mark for touch 
		{		
			if(pwndEditSource->Is("CtmEditX1"))							  
    			pwndEditSource->GetPropValueT("value",&nSourceMedia,sizeof(nSourceMedia));
    		else if(pwndEditSource->Is("CtmSelectEdit"))
    			nSourceMedia = ((CtmSelectEdit*)pwndEditSource)->GetIndex();
   					
    		if(pwnd->Is("CtmEditX1"))						//Sunny<20070530>
    			pwnd->GetPropValueT("value", &iTemp,sizeof(iTemp));
    		else if(pwnd->Is("CtmSelectEdit"))
    			iTemp = ((CtmSelectEdit*)pwnd)->GetIndex();
        	
        	if(iTemp)
        	{
        	    nTemp = 0;
        	    if(pwnd->Is("CtmEditX1"))					//Sunny<20070530>
    			 	pwnd->SetPropValueT("value",nTemp);
    			else if(pwnd->Is("CtmSelectEdit"))
    				((CtmSelectEdit*)pwnd)->SetIndex(nTemp);
        	    pwnd->Update();
        	   	
        	   	//Sunny<20080214><<<<< 增加判斷Msgbox跳出時是否key下確認鍵，否則不做以下讀取操作
        	   	MsgBox(g_MultiLanguage["PICKER_FILE_DELFILE"], tmFT_CODE_TECH);
        	   	if(!g_bMsgBoxFlag)	return wIDControl;
        	   	//Sunny<20080214>>>>>>
        	   	
        	    DeleteMold(nSourceMedia);
				GetMoldCount();		//2009-5-21 
        	    UpdateSource(pwndSender);
       		}
        }
    }
    
	return wIDControl;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;

	if(pwnd == pwndButtonPageDown)
	{
		if(m_bEnterKey)
		{
			PageDown(pwndSender);
		}
	}
	else if(pwnd == pwndButtonPageUp)
	{
		if(m_bEnterKey)
		{
			PageUp(pwndSender);
		}	
	}
	/*else  if(pwnd == pwndEditConfirm)//20150603修正觸屏下刪除外部存儲設備里模具刪除完成仍提示刪除中
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
	GetMoldCount();
    SetSource(pwndSender);
}

void	GetMoldCount()
{
    //獲取 nMaxCount,nSourceCount ,前者指可存儲最大數,後者指獲得的模組數
   	nMaxCount     = CtmMoldSet::GetInstance()->GetMaxMoldSetNum();
   	nSourceCount  = CtmMoldSet::GetInstance()->GetMoldSetNum(m_nSourceMedia);
   	nPageSourceCount = (nMoldCnt_Per_Page < nSourceCount - nStartValue +1) ? nMoldCnt_Per_Page : nSourceCount - nStartValue +1;
	SetDeletNoMax();
}

void  SetSource(CtmWnd* pwndSender)
{
    int     i =0, j =0;
    int     StartIndex = 0;
   	StartIndex = nStartValue;

    //設置最大存儲容量和剩余容量
    if(pwndEditSourceMax   != NULL) 
	{
		pwndEditSourceMax ->SetPropValueT("value", nMaxCount);
		pwndEditSourceMax ->Update();
	}
 
    if(pwndEditSourceRemain!= NULL) 
    {
    	pwndEditSourceRemain ->SetPropValueT("value", nMaxCount - nSourceCount);
    	pwndEditSourceRemain ->Update();
    }
   // <<	 yuc080114
    //加載MODL到本地緩存，用于排序,同時使用排序類型和排序媒介
    LoadData(SortModlData,m_nSourceMedia, nSourceCount, m_SortType);

	//已使用臨時緩沖區代替直接從設備讀入
    for(i = 0; i < nSourceCount; i++)
    {
    	// 此循環內的代碼需要在每一個模具相關的代碼里修改
		if(SortModlData[i+nStartValue-1].acID[0] =='\0' ) //是否有資料
				break;	
		sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+0], "%3d"           , i+StartIndex);
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+1], "%s"            ,SortModlData[i+nStartValue-1].acID);
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+2], "%02d/%02d/%02d",SortModlData[i+nStartValue-1].DateSave.month, SortModlData[i+nStartValue-1].DateSave.day, SortModlData[i+nStartValue-1].DateSave.year%100);
        SortModlData[i].szMaterial[5] = '\0';
        SortModlData[i].szColor[5] = '\0';
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+3], "%s"            ,SortModlData[i+nStartValue-1].szMaterial);
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+4], "%s"            ,SortModlData[i+nStartValue-1].szColor);
    }
    // >>
    for(i = nPageSourceCount; i < nSourceTableRows; i++)		//fuxy, 2010-5-20, for(i = i; i < nSourceTableRows; i++)	
    {
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+0], "%3d"           , i+StartIndex);
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+1], " ");
        sprintf(m_szSourceTable[i*MAX_EDITTABLE_COLS+2], " ");
        strcpy (m_szSourceTable[i*MAX_EDITTABLE_COLS+3], " ");
        strcpy (m_szSourceTable[i*MAX_EDITTABLE_COLS+4], " ");
    }
    for(i = 0; i < nSourceTableRows; i++)
    {
        for(j = 0; j < nSourceTableEditCount[i]; j++)
        {
            if(pwndEditSourceTable[i*MAX_EDITTABLE_COLS + j] != NULL)
            {
                pwndEditSourceTable[i*MAX_EDITTABLE_COLS + j] ->SetPropValueT("text", m_szSourceTable[i*MAX_EDITTABLE_COLS + j]);
            }
        }
    }
    strcpy(m_szSourceTable[MAX_EDITTABLE_COLS*(MAX_EDITTABLE_ROWS - 1)], g_MultiLanguage["VW_MOLD_NEWFILE"]);
    for(i = 1; i < nSourceTableEditCount[MAX_EDITTABLE_ROWS - 1]; i++)   
    	strcpy (m_szSourceTable[MAX_EDITTABLE_COLS*(MAX_EDITTABLE_ROWS - 1) + i], " ");
    
    for(i = 0; i < nSourceTableEditCount[MAX_EDITTABLE_ROWS - 1]; i++)
    {
        if(pwndEditSourceTable[(MAX_EDITTABLE_ROWS - 1)*MAX_EDITTABLE_COLS + i] != NULL)
        {
            pwndEditSourceTable[(MAX_EDITTABLE_ROWS - 1)*MAX_EDITTABLE_COLS + i]->SetPropValueT("text", m_szSourceTable[MAX_EDITTABLE_COLS*(MAX_EDITTABLE_ROWS - 1) + i]);
        }
    }
}

void  UpdateSource(CtmWnd* pwndSender)
{	
    short       i = 0, j = 0;

    SetSource(pwndSender);
    if(pwndEditSourceMax    != NULL)     	pwndEditSourceMax->Update();
    if(pwndEditSourceRemain != NULL)     	pwndEditSourceRemain->Update();
    
    for (i = 0; i < nSourceTableRows; i++)
    {
        for(j = 0; j < nSourceTableEditCount[i]; j++)
        {
            if(pwndEditSourceTable[i*MAX_EDITTABLE_COLS + j] != NULL)
            {
                pwndEditSourceTable[i*MAX_EDITTABLE_COLS + j]->Update();
            }
        }
    }
}

void  UpdateSourceBar(CtmWnd* pwndSender)
{
	int         i = 0;
	int         nIndex = 0;
	FGC         FgcTemp;
	BGC         BgcTemp;
	FGC		  	fgc ;
	BGC 	  	bgc ;	
    nIndex = nDeleteMoldNo -nStartValue;
    
    for(i = 0; i < MAX_EDITTABLE_COLS; i++)
    {
       if(pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndexOld + i] != NULL && nIndexOld >= 0)
       {
           	pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndexOld + i]->Update();
       }
       if(pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i] != NULL && nIndex >= 0)
       {
       		FgcTemp = LIGHTBLUE;
			BgcTemp = YELLOW;
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->GetPropValueT("fgc", &fgc,sizeof(fgc));
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->GetPropValueT("bgc", &bgc,sizeof(bgc));
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->SetPropValueT("fgc",FgcTemp);
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->SetPropValueT("bgc",BgcTemp);
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->Update();
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->SetPropValueT("fgc",fgc);
			pwndEditSourceTable[MAX_EDITTABLE_COLS*nIndex + i]->SetPropValueT("bgc",bgc);
       }
    }
    nIndexOld = nIndex;
}

void	DeleteMold(int nSourceMedia)
{
	DB_MOLDHDR tempMoldHead;
	memset(&tempMoldHead, 0, sizeof(tempMoldHead));		//fuxy, 2012-2-22, add 初始化資料 	
	m_nSourceMedia 	= TOStorageType(nSourceMedia);
	
	if(pwndEditDeleteMoldNO != NULL)
		pwndEditDeleteMoldNO -> GetPropValueT("value", &nDeleteMoldNo,sizeof(nDeleteMoldNo));
	nDeleteMoldNo -=1;		//yuc	
	// << :yuc
	strncpy(tempMoldHead.acID, SortModlData[nDeleteMoldNo].acID, sizeof(tempMoldHead.acID) -1);
	strncpy(tempMoldHead.szMaterial ,SortModlData[nDeleteMoldNo].szMaterial,sizeof(tempMoldHead.szMaterial) -1);
	strncpy(tempMoldHead.szColor, SortModlData[nDeleteMoldNo].szColor, sizeof(tempMoldHead.szColor) -1);
	// >>
	tmDATE		dtDate;
	GetDate(&dtDate);
	tempMoldHead.cUnitCount = GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
	tempMoldHead.DateSave	= tmDATE2DATE(dtDate);	

	memcpy(g_MoldHead.acID,tempMoldHead.acID, sizeof(tempMoldHead.acID));		//2009-5-21
    memcpy(g_MoldHead.szMaterial,tempMoldHead.szMaterial, sizeof(tempMoldHead.szMaterial));
    memcpy(g_MoldHead.szColor, tempMoldHead.szColor,sizeof(tempMoldHead.szColor));

    int nMoldIndex =CtmMoldSet::GetInstance()->Find(tempMoldHead, m_nSourceMedia);
    SendMsg(MSG_SH_INET_MOLD_DEL, nMoldIndex, (int)STORAGE_TYPE_FLASH, NULL);
    CtmMoldSet::GetInstance()->Delete(tempMoldHead,m_nSourceMedia);
}

STROAGETYPE		TOStorageType(int type)
{
	STROAGETYPE Result;
   	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   	int	nStatus = 0;
	switch(type)		//choose storage type
   	{
   		case 0:
   			#ifdef D_NOWRITE_FALSH
   			if(GetFileType(CHECKFILETYPE))
    			nStorageType = STORAGE_TYPE_FLASH;
   			Result = nStorageType;
   			#else
   			Result = STORAGE_TYPE_SRAM;
   			#endif
   			
   			break;
   		case 1:
   			#ifdef	D_ARM
   			#ifdef	D_PXA270		//James add for pxa270 sd 優先找usb，然后再找sd 2009/3/19 10:56上午
				if(!g_wUSBStatus)
				{
					nStatus = CtmUsbFile::GetInstance()->Mount(0);
				}
				else
				{
					nStatus = 1;	
				}
			#endif
				if(nStatus < 1)
				{
					Result = STORAGE_TYPE_MMCARD;
					if (!g_wMMCStatus)
						nStatus = CtmMMCardFile::GetInstance()->Mount();
				}
	   			else
	   			{
		   			Result = STORAGE_TYPE_USB;
		   		}
	   			if(g_wMMCStatus || g_wUSBStatus)
	   				Prompt(g_MultiLanguage["VM_DATA_DELETING"]);
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


void	SetDeletNoMax()
{
	if(pwndEditDeleteMoldNO != NULL)
	{
		pwndEditDeleteMoldNO -> SetPropValueT("value",nStartValue);
		pwndEditDeleteMoldNO -> SetPropValueT("min",nStartValue);
		pwndEditDeleteMoldNO -> SetPropValueT("max",nPageSourceCount +nStartValue -1);
		pwndEditDeleteMoldNO -> SetPropValueT("maxchar",3);  //20151015
		pwndEditDeleteMoldNO -> Update();
	}
}

//=====================================================
//	只在目標選擇Inet的時候調用一次，將Inet所有的模組請求過來
//=======================================================
void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)	//2009-4-15, add
{
	switch(message)
	{
		case MSG_MOLDSET_DELETE:
			MsgBox(g_MultiLanguage["PICKER_FILE_DELFILE"], tmFT_CODE_TECH);
			break;
		case MSG_MOLDSET_DEL_CUR:
			MsgBox(g_MultiLanguage["PICKER_FILE_CANTDEL"], tmFT_CODE_TECH);
			break;
		case MSG_SH_REV_INET_MOLD_EXIST:
			if(wParam == 1) 			//模具不存在或沒有審核；  
				MsgBox(g_MultiLanguage["DLG_MOLD_NOTEXIST"], tmFT_CODE_TECH);
			else  if(wParam == 0x10000)		//模具資料庫不匹配 
				MsgBox(g_MultiLanguage["DLG_MOLD_NOTMATCH"], tmFT_CODE_TECH);
			break;
		default:
			break;	
	}
}

void PageUp(CtmWnd* pwndSender)
{
	if( ((nStartValue -nMoldCnt_Per_Page) <= 0) && nStartValue > 1)
	{
		nStartValue = 1;	
	}
	else if(nStartValue == 1)
	{
		return ;
	}
	else
	{
		nStartValue = nStartValue -nMoldCnt_Per_Page;
	}
		
	GetMoldCount();		//2009-5-21
	UpdateSource(pwndSender);
	
    ((CtmFormView*)pwndSender)->Goto(pwndButtonPageUp->GetTabOrder());
}

void	PageDown(CtmWnd* pwndSender)
{
	if( (nStartValue +nMoldCnt_Per_Page) > CtmMoldSet::GetInstance()->GetMaxMoldSetNum ())
	{
		nStartValue = CtmMoldSet::GetInstance()->GetMaxMoldSetNum () -nMoldCnt_Per_Page;
	}
	else if((nStartValue +nMoldCnt_Per_Page) > nSourceCount)
	{
		return ;
	}
	else
	{
		nStartValue = nStartValue +nMoldCnt_Per_Page;
	}
	
	GetMoldCount();		//2009-5-21
	UpdateSource(pwndSender);
    
    ((CtmFormView*)pwndSender)->Goto(pwndButtonPageDown->GetTabOrder()); 
}
