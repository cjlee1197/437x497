/*============================================================================+
|  Class    : ViewRangeSet library             	   	                  				|
|  Task     : ViewRangeSet library  header file  	  	                 				|
|-----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     	|
|  Link     :G++(GCC)3.2                                                     	|
|  Call     :                                                                	|
|-----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     		|
|  Version  : V1.00                                                          	|
|  Creation : 			                                                     			|
|  Revision :           			                                         				|
+============================================================================*/

//Mario rewrite
#include	"ViewRangeSet.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/

#define SIGN(A)  ( (A) >= 0 ? 1: -1 )

#define		JOINT_LIMIT 0
#define		COORD_LIMIT 1
#define		DH_PARAM_A 2
#define		DH_PARAM_D 3
#define		DH_PARAM_APHA 4
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
CtmWnd*			u_pwndEditJointLimit[12]	=	{NULL};		
CtmWnd*			u_pwndEditCoordLimit[6]	=	{NULL};		
CtmWnd*			u_pwndEditDH_A_Param[6]	=	{NULL};		
CtmWnd*			u_pwndEditDH_D_Param[6]	=	{NULL};	
CtmWnd*			u_pwndEditDH_Apha_Param[6]	=	{NULL};	
CtmWnd*			u_pwndEditXYZUnit[6]	=	{NULL};	
CtmWnd*			u_pwndImageBoxLock[3]	=	{NULL};	
CtmWnd*			u_pwndStaticTitle[3]	=	{NULL};	

int					u_nwndEditJointLimitCnt		=	0;
int					u_nwndEditCoordLimitCnt		=	0;
int					u_nwndEditDH_A_ParamCnt		=	0;
int					u_nwndEditDH_D_ParamCnt		=	0;
int					u_nwndEditDH_APHA_ParamCnt		=	0;
int					u_nwndEditXYZUnitCnt	=	0;
int					u_nImageBoxLockCnt		=	0;
int					u_nStaticTitleCnt		=	0;
int					UnitIndex							=-1;

WORD				RangeStartNO					=	0;
WORD				DH_Param_A_ID					=	0;
WORD				DH_Param_D_ID					=	0;
BOOL			u_bEnterKey						= FALSE;
long				a[6];
long				d[6];
long				sa[6];
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/

BOOL		OnCreateA(CtmWnd* pwndSender)
{
	
	
	//抓取各元件的指針
	u_nwndEditXYZUnitCnt 	= GetSpecialControlNum(pwndSender, "StaticUnitXYZ", "CtmStaticX2", u_pwndEditXYZUnit);
	
	u_nwndEditJointLimitCnt	= GetSpecialControlNum(pwndSender, "EditJoint", "CtmEditX1",u_pwndEditJointLimit);
	u_nwndEditCoordLimitCnt	= GetSpecialControlNum(pwndSender, "EditCoord", "CtmEditX1",u_pwndEditCoordLimit);
	u_nwndEditDH_A_ParamCnt	= GetSpecialControlNum(pwndSender, "EditDHAParam", "CtmEditX1",u_pwndEditDH_A_Param);
	u_nwndEditDH_D_ParamCnt	= GetSpecialControlNum(pwndSender, "EditDHDParam", "CtmEditX1",u_pwndEditDH_D_Param);
	u_nwndEditDH_APHA_ParamCnt = GetSpecialControlNum(pwndSender, "EditDHAphaParam", "CtmEditX1",u_pwndEditDH_Apha_Param);
	
	u_nImageBoxLockCnt 	= GetSpecialControlNum(pwndSender, "ImageBoxLock", "CtmImageBox", u_pwndImageBoxLock);
	u_nStaticTitleCnt 	= GetSpecialControlNum(pwndSender, "StaticTitle", "CtmStaticX2", u_pwndStaticTitle);
	
	//以再加上六位的dh參數供其使用
	UnitIndex						= GetDBValue("TUNE_PARAM37").lValue;  
	DH_Param_A_ID				= g_pDatabase->GetIndex("MAKER_PARAM40");
	DH_Param_D_ID				= g_pDatabase->GetIndex("MAKER_PARAM34");
	SetUnit(UnitIndex);	
	ShowRange();
	g_Pass = FALSE;
	RobotStatus = OP_EDIT_MODE;
	return	TRUE;
}

WORD		OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL;
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);

	for(int i=0; i<u_nwndEditJointLimitCnt ; i++)
	{
		if(u_pwndEditJointLimit[i]!=NULL && u_pwndEditJointLimit[i]==pwndTemp)
		{
			SetRangeValue(u_pwndEditJointLimit[i],JOINT_LIMIT,i,TRUE);
			//if(u_bEnterKey)
			//	SetRangeValue(u_pwndEditJointLimit[i],JOINT_LIMIT,i,g_Pass);
			//else
			//	SetRangeValue(u_pwndEditJointLimit[i],JOINT_LIMIT,i,FALSE);
			return	wIDControl;
		}
	}
	for(int i=0; i<u_nwndEditCoordLimitCnt ; i++)
	{
		if(u_pwndEditCoordLimit[i]!=NULL && u_pwndEditCoordLimit[i]==pwndTemp)
		{
			SetRangeValue(u_pwndEditCoordLimit[i],COORD_LIMIT,i,TRUE);
			//if(u_bEnterKey)
			//	SetRangeValue(u_pwndEditCoordLimit[i],COORD_LIMIT,i,g_Pass);
			//else
			//	SetRangeValue(u_pwndEditCoordLimit[i],COORD_LIMIT,i,FALSE);
				
			return	wIDControl;
		}
	}
	

	return	wIDControl;
}

/*
WORD		OnChangeBM(CtmWnd* pwndSender, WORD wIDControl)
{
	return	wIDControl;
}
*/
WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{

	return	wIDControl;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	if(wKey == _ENTER)
		u_bEnterKey = TRUE;
	else
		u_bEnterKey = FALSE;
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	return	wKey;
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	g_Pass = FALSE;
}

/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void		ShowRange()								//show關節與卡氏運動範圍(控制參數開始的index , 公or英制)
{
	//show Joint Limit
	for(int i=0 ; i<u_nwndEditJointLimitCnt ; i++)
		SetRangeValue(u_pwndEditJointLimit[i],JOINT_LIMIT,i,FALSE);
	//show Coord Limit
	for(int i=0 ; i<u_nwndEditCoordLimitCnt ; i++)
		SetRangeValue(u_pwndEditCoordLimit[i],COORD_LIMIT,i,FALSE);
	//show DH Param
	ShowDHValue();
}

void	SetUnit(int UnitType)																					//設定單位	以及小數點位數				(單位型態(0 = 公制 , 1 = 英制))
{
	int temp = 0;
	if(UnitType == 0)	//單位mm or in
	{
		while(temp < 6)
		{
			u_pwndEditXYZUnit[temp]->SetPropValueT("textID","ROBOT_UNIT_MM");
			temp+=1;
		}
	}
	else
	{
		while(temp < 6)
		{
			u_pwndEditXYZUnit[temp]->SetPropValueT("textID","ROBOT_UNIT_INCH");
			temp+=1;
		}
	}
	
	
	for(int i=0; i<u_nwndEditJointLimitCnt ; i++)
	{
		u_pwndEditJointLimit[i]->SetPropValueT("precision", 3);		//mm or in需要3  or  4 
		u_pwndEditJointLimit[i]->Update();
	}
	
	for(int i=0; i<u_nwndEditCoordLimitCnt ; i++)
	{
		u_pwndEditCoordLimit[i]->SetPropValueT("precision", 3+UnitType);		//mm or in需要3  or  4 
		u_pwndEditCoordLimit[i]->Update();
	}
}

long	GetBDValuewithIndex(int Index)
{
	char*	pDataID		= NULL;
	pDataID		= g_pDatabase->GetString(Index);
	long	lDBvalue	=0;
	lDBvalue	= GetDBValue(pDataID).lValue;

	return	lDBvalue;
}

void		SetRangeValue(CtmWnd* u_pwnd_tempEdit ,int DataMode ,int	editNO ,BOOL Type)		//Type = TRUE -> SAVE DB   Type = FALSE -> 還原DB
{
	if(DataMode==JOINT_LIMIT)
		RangeStartNO = g_pDatabase->GetIndex("TUNE_PARAM41");
	else if(DataMode==COORD_LIMIT)
		RangeStartNO = g_pDatabase->GetIndex("TUNE_PARAM53");
		
	if(Type)
	{
		int			GetRangeValue					=0;	
		u_pwnd_tempEdit->GetPropValueT("value", &GetRangeValue, sizeof(GetRangeValue));
		if(COORD_LIMIT)
			SetDBValue( RangeStartNO+editNO , GetRangeValue * (!UnitIndex) + UnitIndex*(GetRangeValue *2.54+ 0.5*SIGN(GetRangeValue)) );
		else
			SetDBValue( RangeStartNO+editNO , GetRangeValue);
	}
	else
	{
		if(COORD_LIMIT)
			u_pwnd_tempEdit->SetPropValueT("value",GetBDValuewithIndex(RangeStartNO + editNO) * (!UnitIndex) + UnitIndex*(GetBDValuewithIndex(RangeStartNO + editNO)/2.54 +0.5*SIGN(GetBDValuewithIndex(RangeStartNO + editNO))));
		else
			u_pwnd_tempEdit->SetPropValueT("value",GetBDValuewithIndex(RangeStartNO + editNO));
		u_pwnd_tempEdit->Update();
	} 
}

void		ShowDHValue()		//Type = TRUE -> SAVE DB   Type = FALSE -> 還原DB
{
	GetDH_Param();
	int editNO = cfg.jn*2+6;
	int i;
	for(i=0 ; i<u_nwndEditDH_A_ParamCnt ; i++)
	{
		u_pwndEditDH_A_Param[i]->SetPropValueT("value",GetBDValuewithIndex(DH_Param_A_ID+i));
		u_pwndEditDH_A_Param[i]->Update();
		u_pwndEditDH_D_Param[i]->SetPropValueT("value",GetBDValuewithIndex(DH_Param_D_ID+i));
		u_pwndEditDH_D_Param[i]->Update();
		u_pwndEditDH_Apha_Param[i]->SetPropValueT("value",sa[i]);
		u_pwndEditDH_Apha_Param[i]->Update();
	}
}

void	GetDH_Param()
{
	BOOL DH_File_Error=FALSE;
	char s[200];
	char *token;
	FILE *fin;
	fin = fopen( "Data/DHParam.dat","r" );
	if(fin == NULL)	
	{
		DH_File_Error=TRUE;
		return ;
	}
	
	int i;
	if(!DH_File_Error)
	{
		for(i=0 ; i<cfg.jn ; i++)
		{
			if(fgets(s,200,fin) != NULL)
			{
				token=mystrtok1(s,"=");
				sa[i] = atoi(token);
				//printf("sa[i]=%d\n",sa[i]);
			}
			else
			{
				DH_File_Error=TRUE;
				break;
			}
		}
	}
	if(DH_File_Error)//檔案有異常時所使用的預設值
	{
		sa[0] = 1;
		sa[1] = 0;
		sa[2] = 1;
		sa[3] = -1;
		sa[4] = -1;
		sa[5] = 0;
	}
	
}

char *mystrtok1(char *str, char *ctrl)
{
	char *p=str,*q=ctrl,*r;
	while(p&&q&&*p&&*q){
		for(r=p,q=ctrl;*r&&*q&&*r==*q;r++,q++);
		if(q&&*q) p++; // not match,from next character to compare
		else {*p='\0'; break;} // first time founded,return
	}
	return p+1;
}