/*============================================================================+
|  Class    : ViewManualMode library                	                  			|
|  Task     : ViewManualMode library  header file  	  	                 			|
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
#include	"ViewCoordinateSet.h"
#include "../utils.h"
#include "../commonaction.h"
#include "../language.h"
#include "../selectedit.h"
#include "../main.h"
#include "../tmcontrol.h"
#include	"../tmdebug.h"
#include	"../taskcnc.h"
#include	"../tmshmsg.h"

/*============================================================================+
|           Constant                                                     			|
+============================================================================*/
#define SIGN(A)  ( (A) >= 0 ? 1: -1 )
#define	MAXPAGE				 	3
#define	COORDNUMINPAGE	8
#define	MAMCOORDGROUP		24
#define	TOOLCOORDINATE	0
#define	WORKCOORDINATE	1
#define	COORDINATE_NUM	6
/*============================================================================+
|           Global variable                                                  	|
+============================================================================*/
double	u_pszEditBgcColor[3] =
{
	65500,		//單數原色
	65400,		//雙數原色
	17901		//指定色
};

int				u_Coordinatetype			=		0;				//座標型態[0 = 工具 ,1 =工件] 
WORD			u_Coordinate_StartNo	=		0;				//座標值控制參數開始的編號

CtmWnd*		u_pwndBtnPageUp							= NULL;			//上一頁
CtmWnd*		u_pwndBtnPageDown						= NULL;			//下一頁
CtmWnd*		u_pwndStaticAutoAlarm				= NULL;			//自動模式警告訊息
CtmWnd*		u_pwndStaticShowPage				= NULL;			//當前頁數
CtmWnd* 	u_pwndTitleUnitTable[3]			=	{NULL};		//XYZ的單位元件指標
CtmWnd* 	u_pwnEditBaseCoord[6]				=	{NULL};		//基準點輸入座標值元件
int 			u_nEditBaseCoordCnt					=		0;			//基準點輸入座標值元件總數
CtmWnd* 	u_pwnEditTableCoord[72]			=	{NULL};		//表格輸入座標值元件
int 			u_nEditTableCoordCnt				=		0;			//表格輸入座標值元件總數
CtmWnd* 	u_pwnBtnCoordSysNum[COORDNUMINPAGE]			=	{NULL};		//選擇坐標系按鈕元件
int 			u_nBtnCoordSysCnt						=		0;		//選擇坐標系按鈕元件總數
CtmWnd* 	u_pwnBtnSetCoordValue[COORDNUMINPAGE]			=	{NULL};		//設定坐標值按鈕元件總數
int 			u_nBtnSetCoordValueCnt						=		0;			//設定坐標值按鈕元件總數

int				Page												=	1;
int			Unitindex							=0;		//表示目前單位(0=公制,1=英制)，預設0	
/*============================================================================+
|              Function	                                                     	|
+============================================================================*/
BOOL	OnCreateA_Work(CtmWnd* pwndSender)
{
	u_Coordinate_StartNo	= g_pDatabase->GetIndex("CNC_WORK_COORD1_X");
	u_Coordinatetype = WORKCOORDINATE;					//指向表格
	u_nBtnSetCoordValueCnt	=	GetSpecialControlNum(pwndSender, "BtnSetCoordValue", "CtmToolButton", u_pwnBtnSetCoordValue);
	
	OnCreateA(pwndSender);
	return TRUE;
}

BOOL	OnCreateA_Tool(CtmWnd* pwndSender)
{
	u_Coordinate_StartNo	= g_pDatabase->GetIndex("CNC_TOOL_COORD1_X");
	u_Coordinatetype = TOOLCOORDINATE;
	
	OnCreateA(pwndSender);
	return TRUE;
}


BOOL	OnCreateA(CtmWnd* pwndSender)
{
	//printf("1 %d \n",&g_TxCNCData.shmCNCData);
	//printf("2 %d \n",&g_TxCNCData.shmCNCData.CNC_work_coord_no);
	u_nEditBaseCoordCnt			=	GetSpecialControlNum(pwndSender, "BasePoint", "CtmEditX1", u_pwnEditBaseCoord);							//指向基準點
	u_nEditTableCoordCnt		=	GetSpecialControlNum(pwndSender, "EditTABLE", "CtmEditX1", u_pwnEditTableCoord);						//指向表格
	u_nBtnCoordSysCnt		=	GetSpecialControlNum(pwndSender, "BtnGroupNum", "CtmToolButton", u_pwnBtnCoordSysNum);	//指向編號
	
	u_pwndTitleUnitTable[0] = pwndSender->FindControlFromName("StaticCoordinateX");
	u_pwndTitleUnitTable[1] = pwndSender->FindControlFromName("StaticCoordinateY");
	u_pwndTitleUnitTable[2] = pwndSender->FindControlFromName("StaticCoordinateZ");	
	
	Unitindex					= GetDBValue("TUNE_PARAM37").lValue;
	
	u_pwndStaticAutoAlarm		=	pwndSender->FindControlFromName("StaticAutoAlarm");	//自動警告訊息


	u_pwndBtnPageUp					=	pwndSender->FindControlFromName("BtnPageUp");		//上一頁
	u_pwndBtnPageDown				=	pwndSender->FindControlFromName("BtnPageDown");		//下一頁
	u_pwndStaticShowPage		=	pwndSender->FindControlFromName("StaticShowPage");	//當前頁數
	//SetUnit();
	SetBaseCoord_DB_ID();
	SetPage(pwndSender);
	RobotStatus = OP_EDIT_MODE;
	return TRUE;
}

WORD		OnKeyA(CtmWnd* pwndSender, WORD wKey)
{	
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(((CtmFormView*)pwndSender)->Get_TabStop());
	if(wKey == _ENTER)
	{
		for(int EditBaseNum = 0 ; EditBaseNum < u_nEditBaseCoordCnt ; EditBaseNum++)
		{
			if(u_pwnEditBaseCoord[EditBaseNum] == pwndTemp)
			{
				int temp=1;
				g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_update_wt_coord,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
				//return	wKey;
			}	
		}	
		for(int EditNum = 0 ; EditNum < u_nEditTableCoordCnt ; EditNum++)
		{
			if(u_pwnEditTableCoord[EditNum] == pwndTemp)
			{
				int temp=(Page-1)*COORDNUMINPAGE+EditNum/cfg.cn+2;
				if(u_Coordinatetype == TOOLCOORDINATE)
					temp+=100;
				g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_update_wt_coord,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
				//return	wKey;
			}	
		}	
	}
	return	((CtmFormView*)pwndSender)->OnKey1(wKey);
}

WORD	OnMouseUp(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd*		pwndTemp = NULL; 
	pwndTemp	= pwndSender->FindControlFromTab(wIDControl);
	if(pwndTemp == u_pwndBtnPageUp && Page > 1)
	{
		Page--;
		SetPage(pwndSender);
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
			((CtmFormView*)pwndSender)->OnLoseFocus();
	}
	else if(pwndTemp == u_pwndBtnPageDown && Page < MAXPAGE)
	{
		Page++;
		SetPage(pwndSender);
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
			((CtmFormView*)pwndSender)->OnLoseFocus();
	}
	else
	{
		for(int BtnNum = 0 ; BtnNum < u_nBtnCoordSysCnt ; BtnNum++)
		{
			if(u_pwnBtnCoordSysNum[BtnNum] == pwndTemp && g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
				SetSelectGroup(BtnNum+1+(Page-1)*COORDNUMINPAGE,pwndSender);
			else if(u_pwnBtnCoordSysNum[BtnNum] == pwndTemp && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		}
		for(int BtnNum = 0 ; BtnNum < u_nBtnSetCoordValueCnt ; BtnNum++)
		{
			if(u_pwnBtnSetCoordValue[BtnNum] == pwndTemp && g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE)
			{
				WorkCoordSeatCall(BtnNum+1+(Page-1)*COORDNUMINPAGE);
				SetTalbeCoord_DB_ID();
				((CtmFormView*)pwndSender)->OnGetFocus();
			}
			else if(u_pwnBtnSetCoordValue[BtnNum] == pwndTemp && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
				MsgBoxConfirm(g_MultiLanguage["ROBOT_STR_ALRAM_AUTOING"], tmFT_CODE_TECH);
		}
	}
	return	wIDControl;
}

void		OnUpdateA(CtmWnd*	pwndSender)
{
	//printf("g_TxCNCData.shmCNCData.CNC_work_coord[0].x = %d\n",g_TxCNCData.shmCNCData.CNC_work_coord[0].x);
	static BOOL RunOnlyOne =TRUE;
	static BOOL SetCoordinateLock = (g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE);
	if(SetCoordinateLock && (g_TxCNCData.shmCNCData.CNC_mode_lgt!=OP_MEMORY_MODE))
	{
		SetCoordinateLock =FALSE; 	
		SetEditEnabled(SetCoordinateLock);
		u_pwndStaticAutoAlarm->SetPropValueT("textID","ROBOT_STR_DUMMY");
		u_pwndStaticAutoAlarm->Update();
	}
	else if(!SetCoordinateLock && g_TxCNCData.shmCNCData.CNC_mode_lgt==OP_MEMORY_MODE)
	{
		((CtmFormView*)pwndSender)->OnLoseFocus(); 
		SetCoordinateLock =TRUE;
		SetEditEnabled(SetCoordinateLock);
		u_pwndStaticAutoAlarm->SetPropValueT("textID","ROBOT_STR_COORDSET_ALARM");
		u_pwndStaticAutoAlarm->Update();
	}
	if(RunOnlyOne)
	{
		RunOnlyOne =FALSE;
		if(SetCoordinateLock)
		{
			((CtmFormView*)pwndSender)->OnLoseFocus(); 	
			u_pwndStaticAutoAlarm->SetPropValueT("textID","ROBOT_STR_COORDSET_ALARM");
		}
		else
		{
			((CtmFormView*)pwndSender)->OnGetFocus();
			u_pwndStaticAutoAlarm->SetPropValueT("textID","ROBOT_STR_DUMMY");
		}
		SetEditEnabled(SetCoordinateLock);
		u_pwndStaticAutoAlarm->Update();
	}
	
}
/*============================================================================+
|              Helper		                                                   		|
+============================================================================*/
void	SetBaseCoord_DB_ID()
{
	char*	pDataID = NULL;
	WORD wMaxchar =9;
	WORD	u_wStartDBIndex=0;
	u_wStartDBIndex = g_pDatabase->GetIndex("CNC_WORK_COORD0_X");
	for	(int EditBaseNum = 0 ; EditBaseNum < u_nEditBaseCoordCnt ; EditBaseNum++)
	{
		pDataID = g_pDatabase->GetString(u_wStartDBIndex +EditBaseNum);
		double DataValue = GetDBValue(pDataID).lValue;	
    u_pwnEditBaseCoord[EditBaseNum]->SetPropValueT("dbid0",pDataID);
    if(Unitindex == 1 && EditBaseNum % cfg.cn < 3)
    	DataValue = CnangeMMtoIN(DataValue);
    //u_pwnEditBaseCoord[EditBaseNum]->SetPropValueT("value",DataValue);
    u_pwnEditBaseCoord[EditBaseNum]->SetPropValueT("maxchar",wMaxchar);
    u_pwnEditBaseCoord[EditBaseNum]->Update();
	}
}

void	SetTalbeCoord_DB_ID()
{
	char*	pDataID = NULL;
	WORD wMaxchar =9;
	WORD	u_wStartDBIndex=0;
	int  Group;
	int  Coord;
	static int CoordNumOffset = (COORDINATE_NUM-cfg.cn);
	u_wStartDBIndex = u_Coordinate_StartNo+(Page-1)*72;
	for	(int EditTableNum = 0 ; EditTableNum < u_nEditTableCoordCnt ; EditTableNum++)
	{
		Group = EditTableNum/cfg.cn;
		Coord = CoordNumOffset*Group+EditTableNum;
		pDataID = g_pDatabase->GetString(u_wStartDBIndex +Coord);
		//double DataValue = GetDBValue(pDataID).lValue;	
    u_pwnEditTableCoord[EditTableNum]->SetPropValueT("dbid0",pDataID);
    //if(Unitindex == 1 && EditTableNum % cfg.cn < 3)
    //	DataValue = CnangeMMtoIN(DataValue);
    //u_pwnEditTableCoord[EditTableNum]->SetPropValueT("value",DataValue);
    u_pwnEditTableCoord[EditTableNum]->SetPropValueT("maxchar",wMaxchar);
    u_pwnEditTableCoord[EditTableNum]->Update();
	}
}

void	SetTableStartNo()
{
	int StartCoordSysNum =(Page-1)*COORDNUMINPAGE;
	for (int BtnCoordSysNum = 0 ; BtnCoordSysNum < u_nBtnCoordSysCnt ; BtnCoordSysNum++ )
	{ 
		StartCoordSysNum++;
		int		temp = 0;
		char	PageShow[15];
		memset(PageShow,0,sizeof(PageShow));
		sprintf(PageShow,"NO.%d",StartCoordSysNum);
		u_pwnBtnCoordSysNum[BtnCoordSysNum]->SetPropValueT("captionID0",PageShow);
		
		u_pwnBtnCoordSysNum[BtnCoordSysNum]->CreateA();
		u_pwnBtnCoordSysNum[BtnCoordSysNum]->Update();
	}
}

void	SetSelectGroup(int	SelectNum ,CtmWnd* f_pwndSender)			
{ 
	int old_SelectNum = 0;
	if(u_Coordinatetype == WORKCOORDINATE)
		old_SelectNum = GetDBValue("CNC_WORK_COORD_NO").lValue;	
	else
		old_SelectNum = GetDBValue("CNC_TOOL_COORD_NO").lValue;	
	//printf("SelectNum = %d old_SelectNum=%d\n",SelectNum,old_SelectNum);
		
	if(old_SelectNum != SelectNum)
	{
		((CtmFormView*)f_pwndSender)->OnLoseFocus();
		if(old_SelectNum != 0)
			SetGroupBGC(old_SelectNum,FALSE);	
		SetGroupBGC(SelectNum,TRUE);	
		//printf("SelectNum = %d\n",SelectNum);
		int a;
		if(u_Coordinatetype == WORKCOORDINATE)
			a=SetDBValue("CNC_WORK_COORD_NO", SelectNum);	
		else
			a=SetDBValue("CNC_TOOL_COORD_NO", SelectNum);	
		//printf("a=%d g_TxCNCData.shmCNCData.CNC_work_coord_no = %d\n",a,g_TxCNCData.shmCNCData.CNC_work_coord_no);	
			
		((CtmFormView*)f_pwndSender)->OnGetFocus();	
	}
}

void	SetGroupBGC(int f_GroupNum , BOOL SetOn)
{
	if(f_GroupNum <= 0)
		return;
	if(SetOn)		//選取該組
	{
		if(f_GroupNum > COORDNUMINPAGE* (Page-1) && f_GroupNum <= Page*COORDNUMINPAGE)		
		{
			while(f_GroupNum > COORDNUMINPAGE)
				f_GroupNum -= COORDNUMINPAGE;
			for(int EditNum = (f_GroupNum-1)*cfg.cn ; EditNum < f_GroupNum*cfg.cn ; EditNum++)
			{
				u_pwnEditTableCoord[EditNum]->SetPropValueT("bgc",u_pszEditBgcColor[2]);
							
				u_pwnEditTableCoord[EditNum]->Update();
			}
		}
		else		//不同頁時取消選取
		{
			int EditColor = 0;
			if(f_GroupNum % 2 == 1)  //單數組
				EditColor = u_pszEditBgcColor[0];
			else		//雙數組
				EditColor = u_pszEditBgcColor[1];
				
			while(f_GroupNum > COORDNUMINPAGE)
				f_GroupNum -= COORDNUMINPAGE;
			for(int EditNum = (f_GroupNum-1)*cfg.cn ; EditNum < f_GroupNum*cfg.cn ; EditNum++)
			{
				u_pwnEditTableCoord[EditNum]->SetPropValueT("bgc",EditColor);
							
				u_pwnEditTableCoord[EditNum]->Update();
			}
		}
	}
	else				//取消選取該組
	{
		if(f_GroupNum > COORDNUMINPAGE* (Page-1) && f_GroupNum <= Page*COORDNUMINPAGE)
		{
			int EditColor = 0;
			if(f_GroupNum % 2 == 1)  //單數組
				EditColor = u_pszEditBgcColor[0];
			else		//雙數組
				EditColor = u_pszEditBgcColor[1];
			
			
			while(f_GroupNum > COORDNUMINPAGE)
				f_GroupNum -= COORDNUMINPAGE;
			for(int EditNum = (f_GroupNum-1)*cfg.cn ; EditNum < f_GroupNum*cfg.cn ; EditNum++)
			{
				u_pwnEditTableCoord[EditNum]->SetPropValueT("bgc",EditColor);
							
				u_pwnEditTableCoord[EditNum]->Update();
			}	
			
		}
	}
	
}

void	SetPage(CtmWnd* f_pwndSender)
{
	static int Old_Page = -1;
	if(Old_Page != Page)
	{
		((CtmFormView*)f_pwndSender)->OnLoseFocus();
		Old_Page = Page;
		char	PageShow[8];
		char 	PageShow1[16];
		sprintf(PageShow,"%d/%d",Page,MAXPAGE);
    CodeChange(PageShow1, PageShow);
    u_pwndStaticShowPage->SetPropValueT("text",PageShow1);
    u_pwndStaticShowPage->Update();
		SetTalbeCoord_DB_ID();
		SetTableStartNo();
		if(u_Coordinatetype == WORKCOORDINATE)
			SetGroupBGC(g_TxCNCData.shmCNCData.CNC_work_coord_no,TRUE);
		else
			SetGroupBGC(g_TxCNCData.shmCNCData.CNC_tool_coord_no,TRUE);
		((CtmFormView*)f_pwndSender)->OnGetFocus();
	}
}

void	SetUnit()
{
	char StaticX[64] = "\0";
	char StaticY[64] = "\0";
	char StaticZ[64] = "\0";
	int  EditXYZPrec = 0;
	if(Unitindex == 0) //公制
	{
		sprintf(StaticX,"ROBOT_STR_COORDINATE_X_MM");
		sprintf(StaticY,"ROBOT_STR_COORDINATE_Y_MM");
		sprintf(StaticZ,"ROBOT_STR_COORDINATE_Z_MM");
		EditXYZPrec = 3;
	}
	else if(Unitindex == 1)//英制
	{
		sprintf(StaticX,"ROBOT_STR_COORDINATE_X_IN");
		sprintf(StaticY,"ROBOT_STR_COORDINATE_Y_IN");
		sprintf(StaticZ,"ROBOT_STR_COORDINATE_Z_IN");
		EditXYZPrec = 4;
	}
	
	for(int i = 0 ; i < u_nEditTableCoordCnt ; i++)
	{
		if(i % cfg.cn < 3)
			u_pwnEditTableCoord[i]->SetPropValueT("precision",EditXYZPrec);
		else 
			u_pwnEditTableCoord[i]->SetPropValueT("precision",3);
		u_pwnEditTableCoord[i]->Update();
	}
	for(int i = 0 ; i < u_nEditBaseCoordCnt ; i++)
	{
		if(i % cfg.cn < 3)
		{
		u_pwnEditBaseCoord[i]->SetPropValueT("precision",EditXYZPrec);
		u_pwnEditBaseCoord[i]->Update();
		}
	}
	u_pwndTitleUnitTable[0]->SetPropValueT("textID",StaticX);
	u_pwndTitleUnitTable[1]->SetPropValueT("textID",StaticY);
	u_pwndTitleUnitTable[2]->SetPropValueT("textID",StaticZ);
	u_pwndTitleUnitTable[0]->Update();
	u_pwndTitleUnitTable[1]->Update();
	u_pwndTitleUnitTable[2]->Update();
}

double	CnangeMMtoIN(double f_value)		//公轉英 且精度+1
{
	f_value	=	f_value/g_Metric_Imperial_Unit*10;
	f_value += 0.5*SIGN(f_value);
	return f_value;
	
}


double	CnangeINtoMM(double f_value)		//英轉公 精度-1
{
	f_value	=	f_value*g_Metric_Imperial_Unit/10;	
	f_value += 0.5*SIGN(f_value);
	return f_value;
	
}

void	SetEditEnabled(BOOL f_enabled)
{
	for(int i = 0 ; i < u_nEditBaseCoordCnt ; i++ )
	{
		u_pwnEditBaseCoord[i]->SetPropValueT("enabled",!f_enabled);
		u_pwnEditBaseCoord[i]->Update();
	}
	for(int i = 0 ; i < u_nEditTableCoordCnt ; i++ )
	{
		u_pwnEditTableCoord[i]->SetPropValueT("enabled",!f_enabled);
		u_pwnEditTableCoord[i]->Update();
	}
}