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
#include	"ViewProgMsgBox.h"
#include	"../utils.h"
#include	"../commonaction.h"
//#include	"../control.h"
#include	"../iotable.h"

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

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
	
	if(GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED49").lValue)	//Â≠êÁ»˛Â∫èË»˛»˛
	{
		iActionName =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED45").lValue;
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED49", 0);
		bSubTech = TRUE;
	}
	else
	{
		iActionName =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED41").lValue;
	}

	switch(iActionName)
	{
		case 1:		//ËΩ¥Âä®‰Ω
		break;
		case 2:		//Á≠âÂ»˛
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
		case 3:		//»˛ÅËÆ∏
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
		case 4:		//»˛»˛
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
		case 5:		//»˛áÁ≠æ
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
		case 6:		//Ë∑≥ËΩ¨
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
		case 8:		//Ê£Êµ
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
		case 12:	//»˛ÜÂ»˛
		break;
		case 13:	//Â≠êÁ»˛Â∫
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
				Exit();
			break;
			case 1:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 2:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 3:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 4:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 5:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 6:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 7:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 8:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 9:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 10:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 11:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 12:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 13:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 14:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 15:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
			break;
			case 16:
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED46", wKey);
				Exit();
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
			Exit();
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
			Exit();
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
/*void	GetChecked(BOOL bFalg)
{
	WORD wTemp1 = 0,wTemp2 = 0;
	long lTempFwd = 0,lTempBwd = 0;
	long lMovFwd = 0,lMovBwd = 0,lFixFwd = 0,lFixBwd = 0;
	BOOL  bCheck = 0,bChkBefo = 0;
	iActNum = GetDBValue("SYSX_OTHERS_OTHERS_DWORD_RESERVED70").lValue;//»˛ñËûÁ»˛»˛1~Á¨ÜÓ»˛10
	int iNum  = 0, a  = 0,iOpt =0;
	if(iMovMldNum==5)//Á¨ÜÂÆ∂»˛ôMsgBoxÁ´≤„Çª
	{
		a = 0;
		iOpt = 21;
	}
	else if(iMovMldNum==6)//ÔπöÂÆ∂»˛ôMsgBoxÁ´≤„Çª
	{
		a = 3;
		iOpt = 14;
	}

	//printf("iActNum:%x\n",iActNum);
	if(bFalg)//Á§∂ÓÑ™»˛øÓÇï
	{
		if(pwndStaticACTION_NUM!=NULL)
		{
			pwndStaticACTION_NUM->SetPropValueT("textID", g_ActNum[iActNum]);
		}
		if(iMovMldNum==5)//Á¨ÜÂÆ∂»˛ôMsgBoxÁ´≤„Çª
		{
			//wTemp = GetDBValue(pszMoveActtionNumID[iActNum]).lValue;
			lTempFwd = GetDBValue(pszMoveFwdNumID[iActNum]).lValue;
    		lTempBwd = GetDBValue(pszMoveBwdNumID[iActNum]).lValue;
		}
		else if(iMovMldNum==6)//ÔπöÂÆ∂»˛ôMsgBoxÁ´≤„Çª
		{
			//wTemp = GetDBValue(pszFixActtionNumID[iActNum]).lValue;
			lTempFwd = GetDBValue(pszFixFwdNumID[iActNum]).lValue;
    		lTempBwd = GetDBValue(pszFixBwdNumID[iActNum]).lValue;
		}
		for(int i=0;i<10-a;i++)
    	{	//printf("lTempBwd:%x\n",lTempBwd);
    		if( _TestBit(lTempFwd,i))
    		{
    			if(pwndCheckBox[i]!=NULL)
    			{
    				pwndCheckBox[i]->SetPropValueT("checked",TRUE);
    			}
    		}
    		if( _TestBit(lTempBwd,i))
    		{//printf("a:%d i:%d\n",a,i);
    			if(pwndCheckBox[10-a+i]!=NULL)
    			{
    				pwndCheckBox[10-a+i]->SetPropValueT("checked",TRUE);
    			}
    		}
		}
		if( _TestBit(lTempFwd,10))
		{
			if(pwndCheckBox[20]!=NULL)
    		{
    			pwndCheckBox[20]->SetPropValueT("checked",TRUE);
    		}
		}
	}
	else//»˛≥¬®Á»˛Ôπö„
	{
		bReturn = FALSE;
		for(int i = 0; i < iOpt; i++)
		{
			if(pwndCheckBox[i]!=NULL)
			{
				pwndCheckBox[i]->GetPropValueT("checked", &bCheck,sizeof(bCheck));
				//printf("bCheck[%d]:%d %d\n",i,bCheck,a);
				if(bCheck)
				{
					if(i<10-a)
					{
						long lValue  = pow(2,i);
						wTemp1 = wTemp1 + lValue;
						//printf("wTemp1[%d]:%d\n",i,wTemp1);
						//printf("pszMoveActtionNumID[%d]:%x\n",i,pszMoveActtionNumID[iActNum]);
					}
					else if( (10-a<=i) && (i<20-a*2) )
					{
						long lValue  = pow(2,i-10+a);
						wTemp2 = wTemp2 + lValue;
						//printf("wTemp2[%d]:%d\n",i,wTemp2);
						pwndCheckBox[i-10+a]->GetPropValueT("checked", &bChkBefo,sizeof(bChkBefo));
						if(bChkBefo)
						{
							bReturn = TRUE;
							Prompt(g_MultiLanguage["ERR_MOVMLDHELP"], 0);
							break;
						}
					}
					else
					{
						//printf("wTemp3[%d]:%d\n",i,wTemp2);
						if((wTemp1!=0)||(wTemp2!=0))
						{
							bReturn = TRUE;
							Prompt(g_MultiLanguage["ERR_MOVMLDHELP"], 0);
							break;
						}
						else
						{
							long lValue  = pow(2,10);
							wTemp1 = wTemp1 + lValue;
						}
					}
				}
			}
		}
		if(!bReturn)//»˛É‰æ•»˛ãÓôÓÜ†»˛çÁô≥»˛
		{
			if(iMovMldNum==5)
			{
				//printf("11wTemp1[%d]:%d,%d\n",i,wTemp1,wTemp2);
				SetDBValue(pszMoveFwdNumID[iActNum],wTemp1);
				SetDBValue(pszMoveBwdNumID[iActNum],wTemp2);
				
				lMovFwd = GetDBValue(pszMoveFwdNumID[iActNum]).lValue;
    		    lMovBwd = GetDBValue(pszMoveBwdNumID[iActNum]).lValue;
    		    lFixFwd = GetDBValue(pszFixFwdNumID[iActNum]).lValue;
    		    lFixBwd = GetDBValue(pszFixBwdNumID[iActNum]).lValue;
				if((!lMovFwd)&&(!lMovBwd)&&(!lFixFwd)&&(!lFixBwd))
				{
					SetDBValue(pszACTIONDataID[iActNum],0);
				}
				else
				{
					SetDBValue(pszACTIONDataID[iActNum],1);
				}
			}
			else if(iMovMldNum==6)
			{   //printf("22wTemp1[%d]:%d,%d\n",i,wTemp1,wTemp2);
				SetDBValue(pszFixFwdNumID[iActNum],wTemp1);
				SetDBValue(pszFixBwdNumID[iActNum],wTemp2);


                lMovFwd = GetDBValue(pszMoveFwdNumID[iActNum]).lValue;
    		    lMovBwd = GetDBValue(pszMoveBwdNumID[iActNum]).lValue;
    		    lFixFwd = GetDBValue(pszFixFwdNumID[iActNum]).lValue;
    		    lFixBwd = GetDBValue(pszFixBwdNumID[iActNum]).lValue;
				if((!lMovFwd)&&(!lMovBwd)&&(!lFixFwd)&&(!lFixBwd))
				{
					SetDBValue(pszACTIONDataID[iActNum],0);
				}
				else
				{
					SetDBValue(pszACTIONDataID[iActNum],1);
				}
			}
		}
	}
}*/
