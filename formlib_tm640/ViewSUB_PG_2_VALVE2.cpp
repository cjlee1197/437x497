/*===========================================================================+
|  Class    : ViewMagBox library                                             |
|  Task     : ViewMagBox library action source file                          |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewSUB_PG_2_VALVE2.h"
#include	"../utils.h"
#include	"../commonaction.h"
//#include	"../control.h"
#include	"../iotable.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
long		u_SubGroup =0; // °Æµ{¦¡¸¹½X
CtmWnd*		pwndBtnOk 		= NULL;			//	ok	button
CtmWnd*		pwndBtnCancle 	= NULL;			//	cancle button
CtmWnd*		pwndStaticACTION_NUM 	= NULL;
CtmWnd*		pwndMovMld 	= NULL;
BOOL   		b_Restart = FALSE;
BOOL  		b_PushKey = FALSE;
BOOL  		b_7FTMove = FALSE;
BOOL  		bReturn = FALSE;
BOOL 		bSubTech = FALSE;
int			iMovMldNum = 0;
int			iActionName = 0;
char 		pDataID[256];

char*		g_ActNum[] =
{
	"VW_ACTION_1	",
	"VW_ACTION_2	",
	"VW_ACTION_3	",
	"VW_ACTION_4	",
	"VW_ACTION_5	",
	"VW_ACTION_6	",
	"VW_ACTION_7	",
	"VW_ACTION_8	",
	"VW_ACTION_9 	",
	"VW_ACTION_10	",
	"VW_ACTION_11	",
	NULL
};

char*	pszMoveActtionNumID[] =
{
	"MACH_SCRW_HLDP_RAMP6_PRESTIME",
	"MACH_SCRW_HLDP_RAMP6_FLOWTIME",
	"MACH_SCRW_HLDP_RAMP7_PRESTIME",
	"MACH_SCRW_HLDP_RAMP7_FLOWTIME",
	"MACH_SCRW_HLDP_RAMP8_PRESTIME",
	"MACH_SCRW_HLDP_RAMP8_FLOWTIME",
	"MACH_SCRW_HLDP_RAMP9_PRESTIME",
	"MACH_SCRW_HLDP_RAMP9_FLOWTIME",
	"MACH_SCRW_HLDP_RAMP10_PRESTIME",
	"MACH_SCRW_HLDP_RAMP10_FLOWTIME"
};

char*	pszFixActtionNumID[] =
{
	"MACH_SCRW_HLDP_NULL_W0139",
	"MACH_SCRW_HLDP_NULL_W013A",
	"MACH_SCRW_HLDP_NULL_W013B",
	"MACH_SCRW_HLDP_NULL_W013C",
	"MACH_SCRW_HLDP_NULL_W013D",
	"MACH_SCRW_HLDP_NULL_W013E",
	"MACH_SCRW_HLDP_NULL_W013F",
	"MACH_MEJT_FWD_RAMP_PRESTIME",
	"MACH_MEJT_FWD_RAMP_FLOWTIME",
	"MACH_MEJT_FWD_RAMP2_PRESTIME"
};

char*	pszMoveFwdNumID[] =
{
	"MSET_OTHR_OTHR_NULL_W0831",
	"MSET_OTHR_OTHR_NULL_W0835",
	"MSET_OTHR_OTHR_NULL_W0839",
	"MSET_OTHR_OTHR_NULL_W083D",
	"MSET_OTHR_OTHR_NULL_W0841",
	"MSET_OTHR_OTHR_NULL_W0845",
	"MSET_OTHR_OTHR_NULL_W0849",
	"MSET_OTHR_OTHR_NULL_W084D",
	"MSET_OTHR_OTHR_NULL_W0851",
	"MSET_OTHR_OTHR_NULL_W0855",
	"MSET_OTHR_OTHR_NULL_W0869"
};
char*	pszMoveBwdNumID[] =
{
	"MSET_OTHR_OTHR_NULL_W0832",
	"MSET_OTHR_OTHR_NULL_W0836",
	"MSET_OTHR_OTHR_NULL_W083A",
	"MSET_OTHR_OTHR_NULL_W083E",
	"MSET_OTHR_OTHR_NULL_W0842",
	"MSET_OTHR_OTHR_NULL_W0846",
	"MSET_OTHR_OTHR_NULL_W084A",
	"MSET_OTHR_OTHR_NULL_W084E",
	"MSET_OTHR_OTHR_NULL_W0852",
	"MSET_OTHR_OTHR_NULL_W0856",
	"MSET_OTHR_OTHR_NULL_W086A"
};
char*	pszFixFwdNumID[] =
{
	"MSET_OTHR_OTHR_NULL_W0833",
	"MSET_OTHR_OTHR_NULL_W0837",
	"MSET_OTHR_OTHR_NULL_W083B",
	"MSET_OTHR_OTHR_NULL_W083F",
	"MSET_OTHR_OTHR_NULL_W0843",
	"MSET_OTHR_OTHR_NULL_W0847",
	"MSET_OTHR_OTHR_NULL_W084B",
	"MSET_OTHR_OTHR_NULL_W084F",
	"MSET_OTHR_OTHR_NULL_W0853",
	"MSET_OTHR_OTHR_NULL_W0857",
	"MSET_OTHR_OTHR_NULL_W086B"
};
char*	pszFixBwdNumID[] =
{
	"MSET_OTHR_OTHR_NULL_W0834",
	"MSET_OTHR_OTHR_NULL_W0838",
	"MSET_OTHR_OTHR_NULL_W083C",
	"MSET_OTHR_OTHR_NULL_W0840",
	"MSET_OTHR_OTHR_NULL_W0844",
	"MSET_OTHR_OTHR_NULL_W0848",
	"MSET_OTHR_OTHR_NULL_W084C",
	"MSET_OTHR_OTHR_NULL_W0850",
	"MSET_OTHR_OTHR_NULL_W0854",
	"MSET_OTHR_OTHR_NULL_W0858",
	"MSET_OTHR_OTHR_NULL_W086C"
};

char*	pszACTIONDataID[] =
{
	"MSET_OTHR_OTHR_NULL_W085B",
	"MSET_OTHR_OTHR_NULL_W085C",
	"MSET_OTHR_OTHR_NULL_W085D",
	"MSET_OTHR_OTHR_NULL_W085E",
	"MSET_OTHR_OTHR_NULL_W085F",
	"MSET_OTHR_OTHR_NULL_W0860",
	"MSET_OTHR_OTHR_NULL_W0861",
	"MSET_OTHR_OTHR_NULL_W0862",
	"MSET_OTHR_OTHR_NULL_W0863",
	"MSET_OTHR_OTHR_NULL_W0864",
	"MSET_OTHR_OTHR_NULL_W086D"
};

int	nChkBoxNum = 0,iActNum = 0;
CtmWnd*		pwndButtonNum[48]		={NULL};

int		SelectNo	=0;
int   iEditNo=0,iCheckBoxAct=0,iEditACTION=0,iSelectEditACTION=0,iStaticACTION=0,iStaticEditNo;
int 	iStaticAct=0;
int 	No1=0,No=0;
int		Cleanhelp =0;
int		MaxDBNum = 100;
bool	m_bEnterKey = FALSE;
char 	pNo[256]	={NULL}; 
char 	pStaticNo[256]	={NULL}; 

CtmWnd*		pwndEditNo[32] 	={NULL}; 		//
CtmWnd*		pwndStaticEditNo[32] 	={NULL}; 
CtmWnd*		pwndCheckBoxAct[32] ={NULL}; 	//
CtmWnd*		pwndStaticAct[32] ={NULL}; 	//°Ê§@¦C¤å¦r
CtmWnd*		pwndButtonPageDown	= NULL;
CtmWnd*		pwndButtonPageUp	= NULL;
CtmWnd*		pwndSelectEditACTIONTYPE	= NULL;
CtmWnd*		pwndSelectEditACTIONNUM	= NULL;
CtmWnd*		pwndEditACTIONNUM	= NULL;
CtmWnd*		pwndEditSelectNo	= NULL;
CtmWnd*		pwndEditACTION[32] ={NULL};
CtmWnd*		pwndSelectEditACTION[32] ={NULL};
CtmWnd*		pwndStaticACTION[32] ={NULL};
CtmWnd*		pwndButtonSAVE	= NULL;
CtmWnd*		pwndButtonCANEL	= NULL;
CtmWnd*		pwndButtonInsert	= NULL;
CtmWnd*		pwndButtonDelete	= NULL;
CtmWnd*		pwndButtonEditNUM	= NULL;
CtmWnd*		pwndStaticACTIONPARA1	= NULL;

CtmWnd*		pwndButtonAct[32] 	={NULL};
/*===========================================================================+
|                 Function                                                   |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndBtnOk 		= pwndSender->FindControlFromName("btnOK");
	pwndBtnCancle 	= pwndSender->FindControlFromName("btnCancle");
	
	pwndStaticACTION_NUM = pwndSender->FindControlFromName("StaticACTION_NUM");
	nChkBoxNum 			 = GetSpecialControlNum(pwndSender, "buttonACT_",  "CtmToolButton", pwndButtonNum);
	pwndMovMld	= pwndSender->FindControlFromName("editMOVMOLDNUM");
	/*-----------------------------------Åã¥Ü°Ê§@¦Cªí--------------------------------------*/
	iEditNo 	 			= GetSpecialControlNum(pwndSender, "EditNo", "CtmEdit", pwndEditNo);					//
	iStaticEditNo 	 			= GetSpecialControlNum(pwndSender, "StaticNo", "CtmEdit", pwndStaticEditNo);	//§ÇÈþ¦CÈþ¶q
	iCheckBoxAct				= GetSpecialControlNum(pwndSender, "ToolButtonBoxAct", "CtmToolButton", pwndCheckBoxAct);	//
	iStaticAct				= GetSpecialControlNum(pwndSender, "StaticAct", "CtmStaticX2", pwndStaticAct);	//¶¯×÷ÁÐÊýÁ¿ °Ê§@¦C¼Æ¶q
	iEditACTION				= GetSpecialControlNum(pwndSender, "EditACTION", "CtmEditX1", pwndEditACTION);
	iSelectEditACTION				= GetSpecialControlNum(pwndSender, "SelectEditACTION", "CtmSelectEdit", pwndSelectEditACTION);
	iStaticACTION				= GetSpecialControlNum(pwndSender, "StaticACTION", "CtmStaticX2", pwndStaticACTION);

	No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50").lValue;	
	SelectNo		=GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue; // Åª¨ú¨BÆJ
	u_SubGroup = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED47").lValue; // Åª¨ú°Æµ{¦¡½s¸¹
	
	for (int i = 0; i < iEditNo; i++)	//
	{
		No = i+1+No1;
		memset(pNo, 0 ,sizeof(pNo));
		sprintf(pNo,"%d",No);
		if(pwndEditNo[i] != NULL)
		{
			pwndEditNo[i]->SetPropValueT("text",pNo);
			pwndEditNo[i]->Update();
		}
		if(No<6)
		{
			if(pwndEditNo[i] != NULL)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",50712);
				pwndStaticAct[i]->Update();
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",50712);
				//pwndCheckBoxAct[i]->Update();
			}
			if(pwndStaticEditNo[i] != NULL)
			{
				pwndStaticEditNo[i]->SetPropValueT("text","WAIT");
				pwndStaticEditNo[i]->Update();
			}
		}
		else
		{
			memset(pStaticNo, 0 ,sizeof(pStaticNo));
			sprintf(pStaticNo,"%d",No-5);
			pwndStaticEditNo[i]->SetPropValueT("text",pStaticNo);
			pwndStaticEditNo[i]->Update();
		}
		if(pwndEditNo[i] != NULL)			//
		{
			if(No==SelectNo)
			{
				pwndStaticAct[i]->SetPropValueT("bgc",36256);
				//pwndCheckBoxAct[i]->SetPropValueT("bgc",36256);
			}
		}
	}
	UpdateText();
	/*-----------------------------------Åã¥Ü°Ê§@¦Cªí--------------------------------------*/

	{
		iActionName =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52").lValue;
	}

	switch(iActionName)
	{
		case 1:		//è½´åŠ¨ä½
		break;
		case 2:		//ç­‰åÈþ
			for(int i=0;i<8;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					sprintf(pDataID,"PCIKER_INNER_I_%02d",i);
					pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
			for(int i=8;i<nChkBoxNum;i++)
			{
				SetVisible(pwndButtonNum[i], FALSE ,0x0030);
			}
		break;
		case 3:		//Èþè®¸
			for(int i=0;i<7;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					sprintf(pDataID,"PICKER_INNER_O_%02d",i);
					pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
			for(int i=7;i<nChkBoxNum;i++)
			{
				SetVisible(pwndButtonNum[i], FALSE ,0x0030);
			}
		break;
		case 4:		//ÈþÈþ
			for(int i=0;i<nChkBoxNum;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					if(i==0)
						sprintf(pDataID,"VW_PICKER_NULL");
					else
						sprintf(pDataID,"PICKER_REMOTE_O_%02d",i);
					pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
		break;
		case 5:		//Èþ‡ç­¾
			for(int i=0;i<11;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					if(i == 1 && !bSubTech)
					{
						pwndButtonNum[i]->SetPropValueT("visible", double(FALSE));
						pwndButtonNum[i]->SetPropValueT("wtype", 0x30);
					}
					else
					{
						sprintf(pDataID,"PICKER_LABEL_%d",i);
						pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					}
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
			for(int i=11;i<nChkBoxNum;i++)
			{
				SetVisible(pwndButtonNum[i], FALSE ,0x0030);
			}
		break;
		case 6:		//è·³è½¬
			for(int i=0;i<11;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					if(i == 1 && !bSubTech)
					{
						pwndButtonNum[i]->SetPropValueT("visible", double(FALSE));
						pwndButtonNum[i]->SetPropValueT("wtype", 0x30);
					}
					else
					{
						sprintf(pDataID,"PICKER_LABEL_%d",i);
						pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					}
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
			for(int i=11;i<nChkBoxNum;i++)
			{
				SetVisible(pwndButtonNum[i], FALSE ,0x0030);
			}
		break;
		case 8:		//æ£æµ
			for(int i=0;i<9;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					sprintf(pDataID,"PICKER_REMOTE_I_0%d",i);
					pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
			for(int i=9;i<nChkBoxNum;i++)
			{
				SetVisible(pwndButtonNum[i], FALSE ,0x0030);
			}
		break;
		case 12:	//Èþ†åÈþ
		break;
		case 13:	//å­çÈþåº
		break;
		default:
		break;
	}
	if(b_Restart)
	{
		pwndBtnOk->SetPropValueT("wtype",(double)0x8010); 		
		pwndBtnOk->SetPropValueT("visible",(double)FALSE); 		

		pwndBtnCancle->SetPropValueT("wtype",(double)0x8010); 		
		pwndBtnCancle->SetPropValueT("visible",(double)FALSE); 		
	}

	return TRUE;
}
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	int i =0;
	CtmWnd*	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL )	return wIDControl;
	
	return wIDControl;
}
void OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("OnKey wKey=%d\n",wKey);
	CtmWnd*	pwnd = NULL;
  	WORD wIDControl = ((CtmFormView *)pwndSender)->Get_TabStop();
  	pwnd = pwndSender->FindControlFromTab(wIDControl);
	
	g_bMsgBoxFlag =FALSE;

 	if(!b_Restart)
 	{	
 		switch(wKey)
		{
			case 0:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 1:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 2:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 3:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 4:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 5:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 6:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 7:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 8:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 9:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 10:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 11:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 12:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 13:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 14:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 15:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			case 16:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				//Exit();
				::PutCommand("SUB_PG_2_VALVE.txt");
			break;
			default:
			break;
		}
		if(wKey == _ENTER)
		{
			if((g_pExTable->GetTableValue(0,"D_MLDZERO"))&&(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70").lValue)) /*ZHBA 2013-1-18*/
			{
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70",0);
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL44DATA",1);
			}
			else if(g_pExTable->GetTableValue(0,"D_BOARDRES") || g_pExTable->GetTableValue(0,"D_ZEROMOLD")) //tyl 2013.08.21 why20150817
			{
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL43DATA",0);
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL44DATA",1);
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL44DATA",0);
			}
					
			g_bMsgBoxFlag =TRUE;
			//Exit();
			::PutCommand("SUB_PG_2_VALVE.txt");
		}			
		else if (wKey == _KEY_CLEAR || wKey == _EXT_ALT_ENTER)
		{
			if((g_pExTable->GetTableValue(0,"D_MLDZERO"))&&(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70").lValue)) /*ZHBA 2013-1-18*/
			{	
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL44DATA",2);
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED70",0);
			}
			
			else if(g_pExTable->GetTableValue(0,"D_BOARDSPC") || g_pExTable->GetTableValue(0,"D_ZEROMOLD")) //tyl 2013.08.21 	
			{	
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL43DATA",0);
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL44DATA",2);
				SetDBValue("MSET_OTHR_OTHR_NULL_MOLDSPECIAL44DATA",0);
			}
			
    	
			g_bMsgBoxFlag =FALSE;
			//Exit();
			::PutCommand("SUB_PG_2_VALVE.txt");
		}
		else  ((CtmFormView*)pwndSender)->OnKey1(wKey);		
	}
}
void		SetVisible(CtmWnd* pwnd, BOOL  bVisible, WORD  wType)
{
	if(pwnd != NULL)
	{
		if(wType)
			pwnd->SetPropValueT("wtype",wType); 
		else
		{
			if(bVisible)
				pwnd->SetPropValueT("wtype",0x23);
		   	else
        		pwnd->SetPropValueT("wtype",0x30);
        }

			pwnd->SetPropValueT("visible",(double)bVisible);
	}		
}

void	UpdateText()						//§ó·sÅã¥Ü¦r¦ê
{
	//printf("UpdateText\n");
	char 	pDataID[256];
	char 	pDataID2[256];
	char 	pDataID3[256];
	char 	pDataID4[256];
	char 	pDataID5[256];
	char 	pDataID6[256];
	char 	pDataID7[256];
	char 	pDataID8[256];
	char 	pDataID9[256];
	char 	pDataID10[256];
	char 	pDataID11[256];
	char 	pDataID12[256];
	char    szActPara1[256];
	char    szActPara2[256];
	char    szActPara3[256];
	char    szActPara5[256];
	int	ACTIONTYPE =0,ACTIONNUM =0;
	DWORD   wActPara1=0,wActPara2=0,wActPara3=0,wActPara5=0;
	// ¡õ ¦r¦ê¦X¨ÖÅã¥Ü cjlee add 	
	int index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12;
	char	str[1024]; // Åã¥Ü¦r¦ê¥Î
	char	path[1024]; // Åã¥Ü¦r¦ê¥Î
	char* str_1;
	char* str_2;
	char* str_3;
	char* str_4;
	char* str_5;
	char* str_6;
	char* str_7;
	char* str_8;
	char* str_9;
	char* str_10;
	char* str_11;
	char* str_12;
	
	// ¡ô ¦r¦ê¦X¨ÖÅã¥Ü cjlee add 
	for(int i=0;i<iCheckBoxAct;i++)
	{
		memset(str,0,sizeof(str));
		memset(pDataID, 0 ,sizeof(pDataID));
		sprintf(pDataID,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_TYPE",u_SubGroup,i+1+No1);
		memset(pDataID2, 0 ,sizeof(pDataID2));
		sprintf(pDataID2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_NUM",u_SubGroup,i+1+No1);
		sprintf(szActPara1,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER1",u_SubGroup, i+1+No1); 
		sprintf(szActPara2,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER2",u_SubGroup, i+1+No1); 
		sprintf(szActPara3,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER3",u_SubGroup, i+1+No1); 
		sprintf(szActPara5,"MACHINE_PROFILE_SUB%d_NUM%d_ACTION_PARAMETER5",u_SubGroup, i+1+No1); 
		ACTIONTYPE =GetDBValue(pDataID).lValue;
		ACTIONNUM =GetDBValue(pDataID2).lValue;
		wActPara1  = GetDBValue(szActPara1).lValue;
		wActPara2  = GetDBValue(szActPara2).lValue;
		wActPara3  = GetDBValue(szActPara3).lValue;
		wActPara5  = GetDBValue(szActPara5).lValue;
		//printf("LINE%d, ACTIONTYPE:%d, ACTIONNUM:%d, Para1:%d, Para2:%d, Para3:%d, Para5:%d\n",i+1+No1,ACTIONTYPE,ACTIONNUM,wActPara1,wActPara2,wActPara3,wActPara5);
		memset(pDataID, 0 ,sizeof(pDataID));
		memset(pDataID2, 0 ,sizeof(pDataID2));
		memset(pDataID3, 0 ,sizeof(pDataID3));
		memset(pDataID4, 0 ,sizeof(pDataID4));
		memset(pDataID5, 0 ,sizeof(pDataID5));
		memset(pDataID6, 0 ,sizeof(pDataID6));
		memset(pDataID7, 0 ,sizeof(pDataID7));
		memset(pDataID8, 0 ,sizeof(pDataID8));
		memset(pDataID9, 0 ,sizeof(pDataID9));
		memset(pDataID10,0 ,sizeof(pDataID10));
		memset(pDataID11,0 ,sizeof(pDataID11));
		memset(pDataID12,0 ,sizeof(pDataID12));

		//printf("Line%d:ACTIONTYPE=%d,ACTIONNUM=%d\n",i,ACTIONTYPE,ACTIONNUM);
		switch(ACTIONTYPE)
		{
			case 0:		//
				sprintf(pDataID,"VW_PICKER_NULL");
				break;
			case 1:		//¶b°Ê§@
				if(wActPara5) // ©µ®É
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4"); // "©µ®É"
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8"); // "¬í"
				}
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID4,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID4,"VW_HAP5_ROBOT_XAXIS"); // "X¶b"
						break;
					case 2:
						sprintf(pDataID4,"VW_HAP5_ROBOT_YAXIS"); // "Y¶b"
						break;
					case 3:
						sprintf(pDataID4,"VW_HAP5_ROBOT_ZAXIS"); // "Z¶b"
						break;
					case 4: // cjlee 2019/4/6 ¤U¤È 05:41:56
						sprintf(pDataID4,"VW_HAP5_ROBOT_X2AXIS"); // "X2¶b"
						break;
					case 5: // cjlee 2019/4/6 ¤U¤È 05:41:56
						sprintf(pDataID4,"VW_HAP5_ROBOT_Y2AXIS"); // "Y2¶b"
						break;
					default:
						break;
				}
				if(ACTIONNUM <= 5) //(ACTIONNUM <= 3 || ACTIONNUM >= 5) // cjlee 2019/4/6 ¤U¤È 05:41:56
				{
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_1"); // "¥H"
					sprintf(pDataID6,"%3d",wActPara2);
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_2"); // "ªº³t«×²¾°Ê¨ì"
					sprintf(pDataID8,"%3d.%02d",wActPara1/100,wActPara1%100);
					sprintf(pDataID9,"PICKER_DESCRIBE_AXIS_3");
					if(wActPara3)
					{
						sprintf(pDataID10,"PICKER_DESCRIBE_AXIS_7");
						sprintf(pDataID11,"%3d.%02d",wActPara3/100,wActPara3%100);
						sprintf(pDataID12,"VW_CHARGE_MM");
					}
				}
				else if(ACTIONNUM == 6)//if(ACTIONNUM == 4)  // cjlee 2019/4/6 ¤U¤È 05:41:56
				{
					if(wActPara2)
						sprintf(pDataID4,"PICKER_REMOTE_O_14");
					else
						sprintf(pDataID4,"PICKER_REMOTE_O_13");
					if(wActPara3)
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
					else
						sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				}
				break;
			case 2:		//µ¥«Ý
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_WAIT");
				sprintf(pDataID5,"PCIKER_INNER_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 3:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_INNER_O_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID5,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 4:		//»Öªù
				if(wActPara5) // "©µ®É"
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_VALVE"); // "»Öªù"
				sprintf(pDataID5,"DLG_DATETIME_COLON"); // ":"
				sprintf(pDataID6,"PICKER_REMOTE_O_%02d",ACTIONNUM);
				if(wActPara1) // "¥´¶}/Ãö³¬"
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID7,"PICKER_DESCRIBE_AXIS_6");
				if(wActPara2) // "ÀË´ú"
					sprintf(pDataID8,"VW_PUMP_TEST");
//				if(wActPara2)
//				{
//					sprintf(pDataID8,"PICKER_ACTTIME");
//					sprintf(pDataID9,"%2d.%02d",wActPara2/100,wActPara2%100);
//					sprintf(pDataID10,"VW_INJPRO_SECOND");
//				}
				break;
			case 5:		//¼ÐÅÒ
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_START");
				}
				else
				{
					sprintf(pDataID4,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 6:		//¸õÂà
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM == 1)
				{
					sprintf(pDataID4,"ACTIONPOINT_END");
				}
				else
				{
					sprintf(pDataID4,"PICKER_JUMP");
					sprintf(pDataID5,"PICKER_LABEL_%d",ACTIONNUM);
				}
				break;
			case 8:		//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"VW_PUMP_TEST");
				sprintf(pDataID5,"PICKER_REMOTE_I_0%d",ACTIONNUM);
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 12:	//
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				if(ACTIONNUM)
					sprintf(pDataID4,"VW_PID_GROUP%d",ACTIONNUM);
				else
					sprintf(pDataID4,"VW_PICKER_NULL");
				sprintf(pDataID5,"VW_PICKER_PILE");
				if(wActPara1)
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_5");
				else
					sprintf(pDataID6,"PICKER_DESCRIBE_AXIS_6");
				break;
			case 13:	//°Æµ{¦¡
				if(wActPara5)
				{
					sprintf(pDataID,"PICKER_DESCRIBE_AXIS_4");
					sprintf(pDataID2,"%3d.%02d",wActPara5/1000,(wActPara5%1000)/10);
					sprintf(pDataID3,"PICKER_DESCRIBE_AXIS_8");
				}
				sprintf(pDataID4,"PICKER_SUBTECH_1");
				switch(ACTIONNUM)
				{
					case 0:
						sprintf(pDataID5,"VW_PICKER_NULL");
						break;
					case 1:
						sprintf(pDataID5,"PICKER_PROD_BAD");
						break;
					case 2:
						sprintf(pDataID5,"PICKER_PROD_SAMPLE");
						break;
					case 3:
						sprintf(pDataID5,"PICKER_PROD_TEST");
						break;
					case 4:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					case 5:
						sprintf(pDataID5,"VW_AD_RESERVED");
						break;
					default:
						break;
				}
				if(wActPara1)
					sprintf(pDataID6,"ACTIONPOINT_START");
				else
					sprintf(pDataID6,"ACTIONPOINT_END");
				sprintf(pDataID7,"PICKER_SUBTECH_2");
				sprintf(pDataID8,"%2d",wActPara2);
				break;
			case 14: // °Æµ{¦¡µ²§ô
				sprintf(pDataID4,"STR_SUB_END");
				break;
			default: 
				break;	
		}
		//printf("Line%d DataID: %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s\n",i+1+No1,pDataID,pDataID2,pDataID3,pDataID4,pDataID5,pDataID6,pDataID7,pDataID8,pDataID9,pDataID10,pDataID11,pDataID12);
		// ¡õ ¦r¦ê¦X¨ÖÅã¥Ü cjlee add 
		index_1 = g_MultiLanguage.GetStrKeyIndex(pDataID);
		index_2 = g_MultiLanguage.GetStrKeyIndex(pDataID2);	
		index_3 = g_MultiLanguage.GetStrKeyIndex(pDataID3);	
		index_4 = g_MultiLanguage.GetStrKeyIndex(pDataID4);
		index_5 = g_MultiLanguage.GetStrKeyIndex(pDataID5);	
		index_6 = g_MultiLanguage.GetStrKeyIndex(pDataID6);	
		index_7 = g_MultiLanguage.GetStrKeyIndex(pDataID7);
		index_8 = g_MultiLanguage.GetStrKeyIndex(pDataID8);	
		index_9 = g_MultiLanguage.GetStrKeyIndex(pDataID9);	
		index_10 = g_MultiLanguage.GetStrKeyIndex(pDataID10);
		index_11 = g_MultiLanguage.GetStrKeyIndex(pDataID11);	
		index_12 = g_MultiLanguage.GetStrKeyIndex(pDataID12);	
		
		if (index_1<0)
			index_1 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_2<0)
			str_2 = pDataID2;
		else
			str_2 = g_MultiLanguage[index_2];
		if (index_3<0)
			index_3 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_4<0)
			index_4 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_5<0)
			index_5 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_6<0)
			str_6 = pDataID6;
		else
			str_6 = g_MultiLanguage[index_6];
		if (index_7<0)
			index_7 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_8<0)
			str_8 = pDataID8;
		else
			str_8 = g_MultiLanguage[index_8];
		if (index_9<0)
			index_9 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_10<0)
			index_10 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		if (index_11<0)
			str_11= pDataID11;
		else
			str_11 = g_MultiLanguage[index_11];
		if (index_12<0)
			index_12 = g_MultiLanguage.GetStrKeyIndex("ROBOT_STR_DUMP");
		//printf("Line%d = %d  %d  %d %d  %d  %d %d  %d  %d %d  %d  %d\n",i+1+No1,index_1,index_2,index_3,index_4,index_5,index_6,index_7,index_8,index_9,index_10,index_11,index_12);
		
		str_1 = g_MultiLanguage[index_1];
		//str_2 = g_MultiLanguage[index_2];
		str_3 = g_MultiLanguage[index_3];
		str_4 = g_MultiLanguage[index_4];
		str_5 = g_MultiLanguage[index_5];
		//str_6 = g_MultiLanguage[index_6];
		str_7 = g_MultiLanguage[index_7];
		//str_8 = g_MultiLanguage[index_8];
		str_9 = g_MultiLanguage[index_9];
		str_10 = g_MultiLanguage[index_10];
		//str_11 = g_MultiLanguage[index_11];
		str_12 = g_MultiLanguage[index_12];
		
		memset(str,0,sizeof(str));
		//printf("sl_1=%d, sl_2=%d, sl_3=%d, sl_4=%d, sl_5=%d, sl_6=%d, sl_7=%d, sl_8=%d, sl_9=%d, sl_10=%d, sl_11=%d, sl_12=%d\n",strlen(str_1),strlen(str_2),strlen(str_3),strlen(str_4),strlen(str_5),strlen(str_6),strlen(str_7),strlen(str_8),strlen(str_9),strlen(str_10),strlen(str_11),strlen(str_12));
		strncat(str, str_1, strlen(str_1));
		strncat(str, str_2, strlen(str_2));
		strncat(str, str_3, strlen(str_3));
		strncat(str, str_4, strlen(str_4));
		strncat(str, str_5, strlen(str_5));
		strncat(str, str_6, strlen(str_6));
		strncat(str, str_7, strlen(str_7));
		strncat(str, str_8, strlen(str_8));
		strncat(str, str_9, strlen(str_9));
		strncat(str, str_10, strlen(str_10));
		strncat(str, str_11, strlen(str_11));
		strncat(str, str_12, strlen(str_12));

		if(pwndStaticAct[i] != NULL)
		{
			RegionUpdateStop_APP = TRUE; //°±¤î¤¸¥ó¿W¥ß¨ê·s¡A°O¿ý¥¼¨ê·s½d³ò
			
			pwndStaticAct[i]->SetPropValueT("text",str);
			pwndStaticAct[i]->CreateA();
			pwndStaticAct[i]->Show();
			pwndStaticAct[i]->Update();
			pwndStaticAct[i]->UpdateAll();
			
			ChangePos(1);  //±N¥¼¨ê·sªº³¡¤À¤@¦¸¨ê·s
			

		}
		//pwndCheckBoxAct[i]->Update();
		// ¡ô ¦r¦ê¦X¨ÖÅã¥Ü cjlee add 		
	}
}