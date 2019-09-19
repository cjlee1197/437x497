/*============================================================================+
|  Class    : ViewMessageBoxConfirm library                          			    |
|  Task     : ViewMessageBoxConfirm library action source file								|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                    	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : fans		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewMsgBoxMovePallet.h"
#include	"../utils.h"
#include	"../commonaction.h"
#include	"../tmcontrol.h"
#include	"../selectedit.h"

/*========================================================+
|						Constants																			|
+========================================================*/

//�U����taborder��
#define		INDEX_BTN_OK									0

/*========================================================+
|						Global Variable																|
+========================================================*/
char*	ImgPath_1[] =					//Point_1�Ϥ����|
{
	"res_tm640/PTP.png",			
	"res_tm640/PTP2.png",	
	"res_tm640/LIN.png",			
	"res_tm640/LIN2.png",					
	NULL
}; 


CtmWnd*		u_pwndBtnOK							= NULL;
CtmWnd*		u_pwndBtnCancel					= NULL;

CtmWnd*		u_pwndSelectEditMoveMode	= NULL;
CtmWnd*		u_pwndEditPalletNo				= NULL;
CtmWnd*		u_pwndEditZRel						= NULL;
CtmWnd*		u_pwndStaticZRelUnit			= NULL;		//������
CtmWnd*		u_pwndShowMoveType				= NULL;
CtmWnd*		u_pwndBtnSpeedP						= NULL;		//�t��+
CtmWnd*		u_pwndBtnSpeedM						= NULL;		//�t��-
CtmWnd*		u_pwndShowSpeed						= NULL;		//�t��
//CtmWnd*			u_pwndBtnNearPlace				= NULL;		//������
CtmWnd*			u_pwndBtnSoftNessP				= NULL;
CtmWnd*			u_pwndBtnSoftNessM				= NULL;
CtmWnd*			u_pwndShowSoftNess				= NULL;	

char			CommandStr[256] 				="\0"; 		//�̫��X���O�r��
int 			speed 									= 10;			//�ثe�t��
int			nearplace									=3;		//FALSE=OFF TRUE=ON
int				UnitMode								=	0;				//��ܥثe���(0=����,1=�^��)�A�w�]0
int				MoveMode								=	0;			//�ثe���w�����ʼҦ�  0:PTP		1:LIN
int 			u_time 									= 0;
BOOL			FristOne								=	TRUE;
char 			ImagePath1[32] 					= "\0";		//�Ϥ����|
char 			ImagePath2[32] 					= "\0";		//�Ϥ����|	
BOOL			u_bEnterKey							= FALSE;
/*========================================================+
|						Function																			|
+========================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{	
	u_pwndSelectEditMoveMode	= pwndSender->FindControlFromName("SelectMoveType");
	u_pwndEditPalletNo				= pwndSender->FindControlFromName("EditPalletNO");
	u_pwndEditZRel						= pwndSender->FindControlFromName("EditZRel");
	u_pwndStaticZRelUnit			= pwndSender->FindControlFromName("StaticZUnit");
	u_pwndShowMoveType				= pwndSender->FindControlFromName("ShowMoveType");
	
	u_pwndBtnSpeedP						= pwndSender->FindControlFromName("btnSpeedP");
	u_pwndBtnSpeedM						= pwndSender->FindControlFromName("btnSpeedM");
	u_pwndShowSpeed						= pwndSender->FindControlFromName("ShowSpeed");
	//u_pwndBtnNearPlace= pwndSender->FindControlFromName("btnNearPlace");
	u_pwndBtnSoftNessP= pwndSender->FindControlFromName("btnSoftNessP");
	u_pwndBtnSoftNessM= pwndSender->FindControlFromName("btnSoftNessM");
	u_pwndShowSoftNess= pwndSender->FindControlFromName("ShowSoftNess");
	
	u_pwndBtnOK 		= pwndSender->FindControlFromName("btnOK");
	u_pwndBtnCancel	= pwndSender->FindControlFromName("btnCancel");
	
	UnitMode									= GetDBValue("TUNE_PARAM37").lValue;		//���o���^��
	return TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_pwndSelectEditMoveMode)
	{
		MoveMode = ((CtmSelectEdit*)u_pwndSelectEditMoveMode)->GetIndex();	//���o�ثe���w�����ʼҦ�
		strcpy(ImagePath1,ImgPath_1[MoveMode*2]);												//�]�w�Ϥ����|
		strcpy(ImagePath2,ImgPath_1[MoveMode*2+1]);
		u_time=9;
	}
	/*else if(pwndTemp == u_pwndEditPalletNo)
	{
		u_pwndEditPalletNo->SetPropValueT("fgc",(double)0);	
		u_pwndEditPalletNo->Update();
	}*/
	
		
	return wIDControl;
}

WORD		OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*	pwndTemp	= NULL;
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_pwndBtnCancel)
	{
		remove("./DialogHelp/buffer");
		Exit();
	}
	else if(pwndTemp == u_pwndBtnSpeedP)
	{
		SetSpeedAndNearPlace(TRUE,TRUE);
	}
	else if(pwndTemp == u_pwndBtnSpeedM)
	{
		SetSpeedAndNearPlace(TRUE,FALSE);
	}
	else if(pwndTemp == u_pwndBtnSoftNessP)
	{ 
		SetSpeedAndNearPlace(FALSE,TRUE);
	}
	else if(pwndTemp == u_pwndBtnSoftNessM)
	{ 
		SetSpeedAndNearPlace(FALSE,FALSE);
	}
	/*else if(pwndTemp == u_pwndBtnNearPlace)
	{ 
		nearplace=!nearplace;
		if(nearplace)
			u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_OPEN");
		else
			u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_CLOSE");
		u_pwndBtnNearPlace->CreateA();
		u_pwndBtnNearPlace->Update();
	}*/
	else if(pwndTemp == u_pwndBtnOK)
	{
		char temp[16] ="\0";
		double temp_in = 10000/g_Metric_Imperial_Unit;
		double temp_mm = 1000;
		int temp2 = 0;
		int temp3 = 0;
		u_pwndEditPalletNo->GetPropValueT("value", &temp2, sizeof(temp2));
		u_pwndEditZRel->GetPropValueT("value", &temp3, sizeof(temp3));
		
		if(temp2 > 0 && temp2 < 101)
		{
			if(UnitMode)																		//|
			 	sprintf(temp,"%.3f",(double)temp3/temp_in);		//|�w�w�����^���ഫ
			else																						//|
				sprintf(temp,"%.3f",(double)temp3/temp_mm);		//|
				

			if(MoveMode == 0)
			{
				sprintf(CommandStr,"PTP_PLT PALLET%d Z%s S%d F%d;",temp2,ClearCoordZeor(temp,strlen(temp)),speed,nearplace);
			}
			else if(MoveMode == 1)
			{
				sprintf(CommandStr,"LINE_PLT PALLET%d Z%s S%d F%d;",temp2,ClearCoordZeor(temp,strlen(temp)),speed,nearplace);
			}
			
			
			AddCommand(CommandStr);
			Exit();
			
		}
		else
		{
			MsgBoxConfirm(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("ROBOT_MSG_NO_INPUT_POINT")], tmFT_CODE_TECH);
		}
	}

	return wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	  
	((CtmFormView*)pwndSender)->OnKey1(wKey);		
	
	return wKey;
}

void		OnUpdateA(CtmWnd* pwndSender)
{
	if(FristOne)
	{
		FristOne = FALSE;
		/*u_pwndEditPalletNo->SetPropValueT("fgc",63488);	
		u_pwndEditPalletNo->Update();*/
		u_pwndEditZRel->SetPropValueT("precision",3+UnitMode);
		u_pwndEditZRel->Update();
		if(UnitMode)
			u_pwndStaticZRelUnit->SetPropValueT("textID","ROBOT_UNIT_INCH");
		else
			u_pwndStaticZRelUnit->SetPropValueT("textID","ROBOT_UNIT_MM");
		u_pwndStaticZRelUnit->Update();
		SetSpeedValue();
		SetMoveMode();
		
		if(RevertCommand != NULL)
		{
			if(!RevertMoveCommand(RevertCommand))
			{
				remove("./DialogHelp/buffer");
				Exit();
			}
		}
	}
	
	u_time = u_time+1;
	if(u_time == 10)
	{
		u_pwndShowMoveType->SetPropValueT("imagepath",ImagePath1);
		//u_pwndShowMoveType->CreateA();
		u_pwndShowMoveType->Update();
	}
	else if(u_time == 25)
	{
		u_pwndShowMoveType->SetPropValueT("imagepath",ImagePath2);
		//u_pwndShowMoveType->CreateA();
		u_pwndShowMoveType->Update();
		u_time = -5;
	}
}
/*========================================================+
|						Helper																				|
+========================================================*/
void		AddCommand(char* pszCommand)		//���J���O
{
	//�ǥ��B�~���ͪ��ɮרӼȦs�n���J�����O
	FILE*		fCommandBuffer;
	fCommandBuffer = fopen("./DialogHelp/buffer", "w");
	fprintf(fCommandBuffer, "%s", pszCommand);
	fclose(fCommandBuffer);
}

void		SetSpeedAndNearPlace(BOOL Mode ,BOOL Type)
{
	char	temp[15] 		= "\0";
	char	szTemp[15]		= "\0";
	if(Mode)	//�t��
	{
		if(Type)//+
		{
			if(speed<=90)
				speed+=10;
		}
		else//-
		{
			if(speed>=20)
				speed-=10;
		}
	sprintf(temp, "%d%s", speed, "%");
	CodeChange(szTemp, temp);
	u_pwndShowSpeed->SetPropValueT("text", szTemp);
	u_pwndShowSpeed->Update();
	}
	else	if(!Mode)	//SoftNess
	{
		if(Type)//+
		{
			if(nearplace<5)
				nearplace++;
		}
		else//-
		{
			if(nearplace>0)
				nearplace--;
		}
		sprintf(temp, "%d", nearplace);
		CodeChange(szTemp, temp);
		u_pwndShowSoftNess->SetPropValueT("text", szTemp);
		u_pwndShowSoftNess->Update();
	}
}

void		SetMoveMode()				//�]�w���ʼҦ�	
{
	if(g_MoveCommandMode <0 && g_MoveCommandMode >1)
		g_MoveCommandMode=0;
	((CtmSelectEdit*)u_pwndSelectEditMoveMode)->SetIndex(g_MoveCommandMode);
	g_MoveCommandMode=0;
	u_pwndSelectEditMoveMode->Update();
	
	MoveMode = ((CtmSelectEdit*)u_pwndSelectEditMoveMode)->GetIndex();	//���o�ثe���w�����ʼҦ�
	strcpy(ImagePath1,ImgPath_1[MoveMode*2]);												//�]�w�Ϥ����|
	strcpy(ImagePath2,ImgPath_1[MoveMode*2+1]);
	u_time=9;
}

void		SetSpeedValue()			//�]�w�t�ת���l��
{
	speed =10;
	char	temp[15] 		= "\0";
	char	szTemp[15]		= "\0";
	sprintf(temp, "%d%s", speed, "%");
	CodeChange(szTemp, temp);
	u_pwndShowSpeed->SetPropValueT("text", szTemp);
	u_pwndShowSpeed->Update();
	
	nearplace=3;
	memset(temp,0,sizeof(temp));
	memset(szTemp,0,sizeof(szTemp));
	sprintf(temp, "%d\0", nearplace);
	CodeChange(szTemp, temp);
	u_pwndShowSoftNess->SetPropValueT("text", szTemp);
	u_pwndShowSoftNess->Update();
}

char*		ClearCoordZeor(char *Coord,int Num)					//��z�y�� �h���p���I��h�l0
{
	int i = Num-1;
	for( i ; i > 0 ; i--)
	{
		if( *(Coord+i) == '0')
			*(Coord+i)=NULL;
		else if(*(Coord+i) == '.' )
		{
			*(Coord+i)=NULL;
			break;
		}
		else
			break;
	}
	
	if(*Coord != '-')
	{
		for( i ; i > -1 ; i-- )
		{
			*(Coord+i+1)=*(Coord+i);
		}
		*(Coord) = '+';
	}
	return Coord;
}

BOOL	RevertMoveCommand(char *Command)
{
	//==================�٭�һݰѼ�==================================================//
	/**/int	 R_PLT_No = 0; 		//���L�s��      																			/**/
	/**/long R_Z_Offset = 0; 	//��JZ�b�����q			       														/**/
	/**/int  R_Speed = 0; //�t��                																		/**/
	/**/BOOL R_NearPlace = FALSE; //������        														    /**/
	//================================================================================//	
	char *ptr = NULL;
	ptr =  (char*) malloc(COMMAND_STR_SIZE*sizeof(char));
	if(!PassMark2(ptr,Command)) return FALSE;
	
	ptr = PassSpace(ptr);
	while(*ptr != ' ') ptr++;//�e�i��Ѽƶ}�l���a��
	ptr = PassSpace(ptr);

	if(ptr != strstr(ptr,"PALLET")) return FALSE;
	ptr = PassSpace(ptr+6);
	if(!isdigit(*ptr)) return FALSE;
	R_PLT_No = atoi(ptr);
	ptr = PassSpace(PassInt(ptr));
	
	if(*ptr == 'Z') 
	{
		ptr = PassSpace(ptr+1);
		if(!ISValue(ptr)) return FALSE;
		if(*ptr == '+') ptr++;
		float CoordValue = atof(ptr);
		R_Z_Offset = (long)(CoordValue*1000);
		ptr = strstr(ptr,"S");
		if(ptr == NULL) return FALSE;
	}
	else if(*ptr != 'S') return FALSE;

	ptr = PassSpace(ptr+1);
	if(!ISValue(ptr)) return FALSE;
	R_Speed = atoi(ptr);
	ptr=PassSpace(PassInt(ptr));
	if(*ptr != 'F') return FALSE;
	ptr = PassSpace(ptr+1);
	if(!ISValue(ptr)) return FALSE;
	R_NearPlace =atoi(ptr);
	
	speed = R_Speed;
	nearplace = R_NearPlace;
	
	u_pwndEditPalletNo->SetPropValueT("value",R_PLT_No);
	u_pwndEditZRel		->SetPropValueT("value",R_Z_Offset);
	u_pwndEditPalletNo->Update();
	u_pwndEditZRel		->Update();


	UpdateSpeed();
	/*if(nearplace)
		u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_OPEN");
	else
		u_pwndBtnNearPlace->SetPropValueT("captionID","ROBOT_BTN_CYCLE_CLOSE");
	u_pwndBtnNearPlace->CreateA();
	u_pwndBtnNearPlace->Update();*/
	return TRUE;
}

void		UpdateSpeed()
{
	char	temp[15] 		= "\0";
	char	szTemp[15]		= "\0";
	sprintf(temp, "%d%s", speed, "%");
	CodeChange(szTemp, temp);
	u_pwndShowSpeed->SetPropValueT("text", szTemp);
	u_pwndShowSpeed->Update();
	
	memset(temp,0,sizeof(temp));
	memset(szTemp,0,sizeof(szTemp));
	sprintf(temp, "%d\0", nearplace);
	CodeChange(szTemp, temp);
	u_pwndShowSoftNess->SetPropValueT("text", szTemp);
	u_pwndShowSoftNess->Update();
}
