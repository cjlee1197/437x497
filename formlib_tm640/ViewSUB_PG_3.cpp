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
#include	"ViewSUB_PG_3.h"
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
	//No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED40").lValue-5;	
	No1 = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED50").lValue;	
	SelectNo= GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED51").lValue;	
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
	/*-----------------------------------Åã¥Ü°Ê§@¦Cªí--------------------------------------*/

	iActionName =GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED52").lValue;


	switch(iActionName)
	{
		case 1:		// ¶b°Ê§@
		break;
		case 2:		// µ¥«Ý
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
		case 3:		// ¤¹³\
			printf("Permit\n");
			for(int i=0;i<8;i++)
			{
				if(pwndButtonNum[i]!=NULL)
				{
					sprintf(pDataID,"PICKER_INNER_O_%02d",i);
					pwndButtonNum[i]->SetPropValueT("captionID", pDataID);
					pwndButtonNum[i]->CreateA();
					//pwndButtonNum[i]->UpdateAll();
				}
			}
			for(int i=8;i<nChkBoxNum;i++)
			{
				SetVisible(pwndButtonNum[i], FALSE ,0x0030);
			}
			printf("Permit show done\n");
		break;
		case 4:		// »Öªù
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
		case 5:		// ¼ÐÅÒ
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
		case 6:		// ¸õÂà
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
		case 8:		// ÀË´ú
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
	
	if(wKey>=0 && wKey<=16)
		{
			SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED54", wKey);
			Exit();
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
