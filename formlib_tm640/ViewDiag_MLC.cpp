/*===========================================================================+
|  Class    : ViewDiag_MLC_I library                                         |
|  Task     : ViewDiag_MLC_I library action source file                      |
|--------------------------------------------------------------------------- |
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |  
+===========================================================================*/
#include 	"ViewDiag_MLC.h"
#include 	"../commonaction.h"
#include	"../font.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define	    SELECTBOX_TABLE			"SelectBox"           //  The Controls in Table
#define	    STATIC_TABLE			"Static"         
#define	    STATIC_INDEX_TABLE		"StaIndex"         
#define     PLC_TALBE_NUM_IO       	32//128     
#define     PLC_TALBE_NUM       	  64//128

#define     PLC_CNT_MAX			    		512

#define     FILE_PLC_I					"PLC_I.csv"
#define     FILE_PLC_O					"PLC_O.csv"
#define     FILE_PLC_C					"PLC_C.csv"
#define     FILE_PLC_S					"PLC_S.csv"
#define     FILE_PLC_A					"PLC_A.csv"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
WORD		u_nBtn_Cnt 		= 0;
WORD		u_nSelect_Cnt 		= 0;
WORD		u_nStatic_Cnt 		= 0;
WORD		u_nStaticIndex_Cnt	= 0;
WORD		u_wStartNo 			= 0;
WORD		u_wStartDBIndex 	= 0;
WORD		u_wTypeFlag 		= 0;
char 		szStringID_B[7]		="\0";
int			table_cnt					=	0;
char		FileName[16]			="\0";

CtmWnd*		pwndSelectBox[256] 	= {NULL};
CtmWnd*		pwndStatic[256] 	= {NULL};
CtmWnd*		pwndStaticIndex[256]= {NULL};
CtmWnd*		pwndEditGoTo				=	NULL;
CtmWnd*		pwndBtnOutput[256]				= {NULL};
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreate_MLC_I(CtmWnd* pwndSender)
{
	u_wTypeFlag = 1;
	table_cnt = PLC_TALBE_NUM_IO;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_I_BIT0");
	strcpy(szStringID_B,"_MLC_I");
	strcpy(FileName,FILE_PLC_I);
	
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_O(CtmWnd* pwndSender)
{
	u_wTypeFlag = 1;
	table_cnt = PLC_TALBE_NUM_IO;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_O_BIT0");
	strcpy(szStringID_B,"_MLC_O");
	strcpy(FileName,FILE_PLC_O);
	
	u_nBtn_Cnt		= GetSpecialControlNum(pwndSender, "BtnStatus" , "CtmToolButton", pwndBtnOutput);
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_C(CtmWnd* pwndSender)
{
	u_wTypeFlag = 1;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_C_BIT0");
	strcpy(szStringID_B,"_MLC_C");
	strcpy(FileName,FILE_PLC_C);
	
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_S(CtmWnd* pwndSender)
{
	u_wTypeFlag = 1;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_S_BIT0");
	strcpy(szStringID_B,"_MLC_S");
	strcpy(FileName,FILE_PLC_S);
	
	CreatePublic(pwndSender);
	return TRUE;
}
BOOL	OnCreate_MLC_A(CtmWnd* pwndSender)
{
	u_wTypeFlag = 1;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_A_BIT0");
	strcpy(szStringID_B,"_MLC_A");
	strcpy(FileName,FILE_PLC_A);
	
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_Timer(CtmWnd* pwndSender)
{
	u_wTypeFlag = 0;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_TMR1_0");
	strcpy(szStringID_B,"_MLC_TMR1");

	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_Cnt(CtmWnd* pwndSender)
{
	u_wTypeFlag = 0;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_CNT0");
	strcpy(szStringID_B,"_MLC_CNT");
	
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_Reg(CtmWnd* pwndSender)
{
	u_wTypeFlag = 0;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_REG0");
	strcpy(szStringID_B,"_MLC_REG");
	
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	OnCreate_MLC_Dreg(CtmWnd* pwndSender)
{
	u_wTypeFlag = 0;
	table_cnt = PLC_TALBE_NUM;
	u_wStartDBIndex = g_pDatabase->GetIndex("S_MLC_BREG0");
	strcpy(szStringID_B,"_MLC_DREG");
	
	CreatePublic(pwndSender);
	return TRUE;
}

BOOL	CreatePublic(CtmWnd* pwndSender)
{
	u_nSelect_Cnt		= GetSpecialControlNum(pwndSender, SELECTBOX_TABLE, "CtmSelectBox", pwndSelectBox);
	u_nStatic_Cnt		= GetSpecialControlNum(pwndSender, STATIC_TABLE, "CtmStaticX2", pwndStatic);
	u_nStaticIndex_Cnt	= GetSpecialControlNum(pwndSender, STATIC_INDEX_TABLE, "CtmStaticX2", pwndStaticIndex);
	pwndEditGoTo		= pwndSender->FindControlFromName("EditStartNo");	
	
	((CtmFormView*)pwndSender)->OnLoseFocus();
	if(pwndEditGoTo != NULL) 
	{
		pwndEditGoTo->SetPropValueT("max",PLC_CNT_MAX-table_cnt);
		pwndEditGoTo->Update();
	}
	((CtmFormView*)pwndSender)->OnGetFocus();
	if(u_wTypeFlag)		UpdateSelectTable(u_wStartNo);
	UpdateStaticTable(u_wStartNo);
	RobotStatus = OP_EDIT_MODE;
}


WORD OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//printf("wKey:%X \n",wKey);
	static int u_wStartNo_old = u_wStartNo;
	if((wKey == 0x6D00)||(wKey == 0x6800))//Page Change
	{
		if(wKey == 0x6D00)	//PageDown
		{
			u_wStartNo += table_cnt;
			if(u_wStartNo >= (512-table_cnt))	u_wStartNo = 512-table_cnt;
		}
		else if(wKey == 0x6800)	//PageUp
		{
			u_wStartNo -= table_cnt;
			if((u_wStartNo >= (512-table_cnt)) ||(u_wStartNo <0))	u_wStartNo =0;
		}
		if(u_wStartNo_old != u_wStartNo)
		{
			//if(u_wTypeFlag)		UpdateSelectTable(u_wStartNo);
			UpdateStaticTable(u_wStartNo);
			u_wStartNo_old = u_wStartNo;
			
			((CtmFormView*)pwndSender)->OnLoseFocus();	
			pwndEditGoTo->SetPropValueT("value",u_wStartNo_old);
			pwndEditGoTo->Update();
			((CtmFormView*)pwndSender)->OnGetFocus();	
		}
	}
	else if(wKey == _ENTER)
	{
		CtmWnd*		pwndTemp = NULL; 
		pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
		if(pwndTemp == pwndEditGoTo ) pwndEditGoTo->GetPropValueT("value", &u_wStartNo, sizeof(u_wStartNo));
		if(u_wStartNo_old != u_wStartNo)
		{
			//if(u_wTypeFlag)		UpdateSelectTable(u_wStartNo);
			UpdateStaticTable(u_wStartNo);
			u_wStartNo_old = u_wStartNo;
		}
	}
	else ((CtmFormView*)pwndSender)->OnKey1(wKey);
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		return	wIDControl;
	}
	for(int i=0 ; i<u_nBtn_Cnt ; i++)
	{
		if(pwndTemp == pwndBtnOutput[i] && g_TxCNCData.shmCNCData.CNC_jaws_control==0)
		{
			int tmp = (i+1)*10;
			if(g_pRxCNCData->s_mlc_s_bit[101+i]==0)
				tmp+=1;
			//printf("tmp = %d\n",tmp);
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		}
	}
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(u_wTypeFlag)		UpdateSelectTable(u_wStartNo);
	if(!u_wTypeFlag)	UpdateStaticTable(u_wStartNo);
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void    UpdateSelectTable(WORD	wStartNo)
{
	static	WORD		wStartNo_Old =0;
	static	long long	llDBValue	 =0;
	static	BOOL		bSelected	 =FALSE;
	
	//if((wStartNo_Old == wStartNo)&& wStartNo_Old != 0) return;
    
    WORD    *pwDst, *pwSrc;
	char temp[128];
	char szTemp[256];
	for (int i=0; i<u_nSelect_Cnt; i++)
    {
        if(pwndSelectBox[i] != NULL)
        {
        	llDBValue 	=GetDBValue(u_wStartDBIndex+i+wStartNo).lValue;
        	//if(i==2 || i ==3 )printf("%d Value=%lld \n",i,llDBValue);
        	if(llDBValue == 0)	bSelected = FALSE;
        	else				bSelected = TRUE;
        	pwndSelectBox[i] -> SetPropValueT("selected", bSelected);
        	pwndSelectBox[i] -> Update();
        }
    }
}

void    UpdateStaticTable(WORD	wStartNo)
{
	static	WORD		wStartNo_Old =0;
	char				szStringID[20]="\0";
	char 				szMessage[200]="\0";
	static	long long	llDBValue	 =0;
	//printf("wStartNo_Old:%d wStartNo:%d \n",wStartNo_Old,wStartNo);
	if(u_wTypeFlag)
	{
		if((wStartNo_Old == wStartNo)&& wStartNo_Old != 0) return;
  }
    
  WORD    *pwDst, *pwSrc;
  char			*pwDst1;
	char szPointNo[128];
	char szTemp[256];
	char File_PLC[128];
	sprintf(File_PLC,"Data/PLC/%s",FileName);
	ifstream fin;
	if(u_wTypeFlag)
	{
		fin.open(File_PLC);
		if(fin) 
		{
      char		Jump[60];
			for (int i = 0 ; i < wStartNo ; i++)
			{
    		fin.getline(Jump,60);
      	//printf("%d Jump is %s\n",i,Jump);
			}
		}
		for (int i=0; i<u_nStatic_Cnt; i++)
		{
			if(pwndStatic[i] != NULL)
			{
	      	WORD    *pwSrc = NULL, *pwDst = NULL;
	      	char	  *pwSrc_c = NULL;
					memset(szMessage, 0, 200);
					WORD wServPNo = i+wStartNo;	
	      	sprintf(szPointNo ,"%03d",wServPNo);
	      	
	      	char *pszPointNo = szPointNo;
	      	pwDst = (WORD *)szMessage;
					
					while (*pszPointNo != 0)
	      	{
	      		*pwDst = *pszPointNo;
	      		pwDst ++;
	      		pszPointNo++;
	      	}
					*pwDst ++ = ' ';
					*pwDst ++ = ' '; //20110715 add
					*pwDst ++ = ' '; //20120406 add
					
					//sprintf(szStringID,"_MLC_I%03d",wServPNo);
					//sprintf(szStringID,"%s%03d",szStringID_B,wServPNo);
					//pwSrc = (WORD *)g_MultiLanguage[szStringID];
	      		
	      	if(pwSrc == NULL)	//pwSrc = (WORD *)g_MultiLanguage["MSG_DUMMY"];
	      	{
						if(!fin) break;
	      		char		pwSrc1[60];
	      		fin.getline(pwSrc1,60);
	      		pwSrc_c = pwSrc1;
	      		//printf("%d pwSrc_c is %s\n",i,pwSrc_c);
	      		//抓取PLC說明文件
	      	}
	      	else
	      	{
						if(!fin) break;
	      		char		Jump[60];
	      		fin.getline(Jump,60);	
	      	}
	      	
	      	if(pwSrc != NULL)
	      	{
	      		while(*pwSrc != 0)
	      		{
	      		   *pwDst = *pwSrc;
	      		   pwDst ++;
	      		   pwSrc ++;
	      		}
	      	}
	      	else if(pwSrc_c != NULL)
	      	{
	      		while(*pwSrc_c != 0)
	      		{
	      		   *pwDst = *pwSrc_c;
	      		   pwDst ++;
	      		   pwSrc_c ++;
	      		}
	      	}
	     	
	      pwndStatic[i] -> SetPropValueT("text", szMessage);
	      pwndStatic[i] -> Update();
	 
			}
		}
		fin.close();
	}
	if(!u_wTypeFlag)
	{
		for (int i=0; i<u_nStaticIndex_Cnt; i++)
		{
		  if(pwndStaticIndex[i] != NULL)
		  {
				WORD    *pwSrc = NULL, *pwDst = NULL;
				memset(szMessage, 0, 200);
				WORD wServPNo = i+wStartNo;	
	    	
				sprintf(szPointNo ,"%03d",wServPNo);
		  	char *pszPointNo = szPointNo;
		  	pwDst = (WORD *)szMessage;
				
				while (*pszPointNo != 0)
		  	{
		  		*pwDst = *pszPointNo;
		  		pwDst ++;
		  		pszPointNo++;
				}
	    	
				pwndStaticIndex[i] -> SetPropValueT("text", szMessage);
				pwndStaticIndex[i] -> Update();  
			}
			if(pwndStatic[i] != NULL)
		  {
				WORD    *pwSrc = NULL, *pwDst = NULL;
				memset(szMessage, 0, 200);
				WORD wServPNo = i+wStartNo;	
		   	llDBValue 	=GetDBValue(u_wStartDBIndex+i+wStartNo).lValue;
		      
				sprintf(szPointNo ,"%d",llDBValue);
		    char *pszPointNo = szPointNo;
		    pwDst = (WORD *)szMessage;
					
				while (*pszPointNo != 0)
		    {
		    	*pwDst = *pszPointNo;
		    	pwDst ++;
		    	pszPointNo++;
		   	}
		   	
		   	pwndStatic[i] -> SetPropValueT("text", szMessage);
		    pwndStatic[i] -> Update();
		  }
		}
	}
}

void    CodeChange(char *pDst, char *pSrc)
{
    WORD    *pwDst  = (WORD *)pDst;
    
    while(*pSrc != 0)      *pwDst ++ = *pSrc ++;
    
    *pwDst  = 0;
    
}

