/*===========================================================================+
|  Class    : ViewScreenSaver  library                                       |
|  Task     : ViewScreenSaver  library action source file                    |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny                                                          |
|  Version  : V1.00                                                          |
|  Creation : 04/03/2007                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include 	"ViewScreenSaver.h"
#include 	"../utils.h"
#include	"../commonaction.h"
#include    "../taskmoni.h"

/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
#define		NAME_NUMPART	  "PART"
#define		NAME_IMAGEBG	  "ImageBG"
const	int	MAX_NUMBER_BIT=6;	//預設一行是6個數字

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
long	u_lMoveIntervalTime = 0;		//	update time
bool	u_bScreenSaverLogoutOption;
int		u_iBit[MAX_NUMBER_BIT];

WORD	u_wOPStatus=0;    			//紀錄當前機器操作狀態
bool	u_bOPStatus=0;				//手動狀態or非手動狀態

bool 	u_bvisibleT =TRUE;			
bool 	u_bvisibleF =FALSE;

CtmWnd* u_pwndImageColon0	= NULL;	//畫面上冒號之點一       
CtmWnd* u_pwndImageColon1	= NULL;	//畫面上冒號之點二
CtmWnd* u_pwndImagePoint0	= NULL;	//畫面上的小數點
CtmWnd* u_pwndImageBG		= NULL;	//背景圖片
CtmWnd* u_pwndImageSpacePoint0= NULL;
CtmWnd* u_pwndImageSpacePoint1= NULL;

bool	u_bHorizontalFlag =TRUE; //水平移動Flag
bool	u_bBverticalFlag  =TRUE; //豎直移動Flag
short	ViewWidth;
short	ViewHeight;
	
long	u_lOpenCount;				//開模總數值
long	u_lCycleTime;				
BOOL 	u_bVisible =TRUE;

tmNUMBERBMP		m_NumberBmp[] 	={
	{	1,	0,	1,	1,	1,	1,	1},//0
	{	0,	0,	0,	0,	1,	0,	1},//1	
	{	1,	1,	1,	0,	1,	1,	0},//2
	{	1,	1,	1,	0,	1,	0,	1},//3
	{	0,	1,	0,	1,	1,	0,	1},//4
	{	1,	1,	1,	1,	0,	0,	1},//5
	{	1,	1,	1,	1,	0,	1,	1},//6
	{	1,	0,	0,	0,	1,	0,	1},//7
	{	1,	1,	1,	1,	1,	1,	1},//8
	{	1,	1,	1,	1,	1,	0,	1} //9
};

CtmWnd* pwndNUMPART[256];
/*===========================================================================+
|           Class implementation - ViewScreenSaver                           |
+===========================================================================*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{		
	u_pwndImageBG		=pwndSender->FindControlFromName(NAME_IMAGEBG);
	
	u_pwndImageColon0		=u_pwndImageBG->FindControlFromName("ImageBColonPoint0");
	u_pwndImageColon1		=u_pwndImageBG->FindControlFromName("ImageBColonPoint1");
	u_pwndImagePoint0		=u_pwndImageBG->FindControlFromName("ImageBPoint0");
	u_pwndImageSpacePoint0	=u_pwndImageBG->FindControlFromName("ImageBSpacePoint0");
	u_pwndImageSpacePoint1	=u_pwndImageBG->FindControlFromName("ImageBSpacePoint1");
		
	GetSpecialControlNum(u_pwndImageBG, NAME_NUMPART, "CtmImageBox", pwndNUMPART);
	
	
	u_lMoveIntervalTime = 500;		//	Move Interval Time
	AddTimer(&u_lMoveIntervalTime);

	u_bHorizontalFlag =TRUE; //水平移動Flag
	u_bBverticalFlag  =TRUE; //豎直移動Flag
	
	pwndSender->GetPropValueT("right", &ViewWidth,sizeof(ViewWidth));
	pwndSender->GetPropValueT("bottom", &ViewHeight,sizeof(ViewHeight));
	
	return TRUE;
}

void	OnShowA(CtmWnd* pwndSender)
{
    
	pwndSender->Clear();
	u_bVisible =FALSE;
	
	GetOPStatus();
	return;
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//printf("key is %d\n",wKey);
	Exit();
	return wKey;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
	if(g_ptaskMoni!=NULL && !(g_ptaskMoni->IsSleep())) 
	{
		Exit();
	}
	tmDATE  date;
	tmTIME  time;
	long	lYearMonthDay;
	long 	lTimeHourMinute;
	
	GetOPStatus();
	if(u_wOPStatus == 0)
	{
		if(u_bOPStatus) 
		{
			for (int i = 0; i < pwndSender->GetControlCount(); i++)
			{
				//if (pwndSender->Controls[i]->GetParent() ==u_pwndImageBG)
				{
					pwndSender->Controls[i] -> SetPropValueT("visible",(double)u_bvisibleF);
					pwndSender->Controls[i] -> Update();
				}
			}
		}
		
		u_bOPStatus =0;
		
		GetTime(&time); 
		GetDate(&date);
		
		u_pwndImageColon0 -> SetPropValueT("visible",(double)u_bvisibleT);
		u_pwndImageColon0 -> Update();
		u_pwndImageColon1 -> SetPropValueT("visible",(double)u_bvisibleT);
		u_pwndImageColon1 -> Update();
		
		u_pwndImageSpacePoint0 -> SetPropValueT("visible",(double)u_bvisibleT);
		u_pwndImageSpacePoint0 -> Update();
		u_pwndImageSpacePoint1 -> SetPropValueT("visible",(double)u_bvisibleT);
		u_pwndImageSpacePoint1 -> Update();
		
		lYearMonthDay = (date.year%100)*10000+(date.month)*100+date.day;
		lTimeHourMinute =time.hour*100+time.minute;
		
		SelectBit(lYearMonthDay,0,0);
		SelectBit(lTimeHourMinute,1,2);
	}
	else
	{
		u_bOPStatus =1;
			
		u_lOpenCount		= GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
		u_lCycleTime		= GetDBValue("MONI_OTHR_OTHR_NULL_CYCLEREALTIME").lValue;
		
		u_pwndImagePoint0 -> SetPropValueT("visible",(double)u_bvisibleT);
		u_pwndImagePoint0 -> Update();
		
		SelectBitDB(u_lOpenCount,0,0);
		SelectBitDB(u_lCycleTime,1,2);
	}
	AllControlMove(pwndSender);
	
	return;
}

void	OnDestroyA(CtmWnd* pwndSender)
{	
	
	DeleteTimer(&u_lMoveIntervalTime);
}

/*===========================================================================+
|                                   Helper                                   |
+===========================================================================*/
void	ShowNumber(int NumPart,int No)
{
	for(int i =0; i<7;i++)
	{
		if(m_NumberBmp[NumPart].Part[i]==0) pwndNUMPART[No*7+i] -> SetPropValueT("visible",(double)u_bvisibleF);
		else                           		pwndNUMPART[No*7+i] -> SetPropValueT("visible",(double)u_bvisibleT);
		pwndNUMPART[No*7+i] -> Update();	
	}

/*	if(m_NumberBmp[NumPart].UpHori0==0) pwndNUMPART[No*7+0] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                           		pwndNUMPART[No*7+0] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+0] -> Update();
    
	if(m_NumberBmp[NumPart].MidHori1==0) pwndNUMPART[No*7+1] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                             	 pwndNUMPART[No*7+1] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+1] -> Update();
	
	if(m_NumberBmp[NumPart].DownHori2==0) pwndNUMPART[No*7+2] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                              	  pwndNUMPART[No*7+2] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+2] -> Update();
	
	if(m_NumberBmp[NumPart].UpLBVert0==0) pwndNUMPART[No*7+3] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                                  pwndNUMPART[No*7+3] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+3] -> Update();
    
	if(m_NumberBmp[NumPart].UpRVert1==0) pwndNUMPART[No*7+4] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                                 pwndNUMPART[No*7+4] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+4] -> Update();
    
	if(m_NumberBmp[NumPart].DownLVert2==0) pwndNUMPART[No*7+5] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                                   pwndNUMPART[No*7+5] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+5] -> Update();
    
	if(m_NumberBmp[NumPart].DownRVert3==0) pwndNUMPART[No*7+6] -> SetPropValueT("visible",(double)u_bvisibleF);
	else                                   pwndNUMPART[No*7+6] -> SetPropValueT("visible",(double)u_bvisibleT);
	pwndNUMPART[No*7+6] -> Update();
*/
}

void	SelectBit(long Value,int Line,int VisibleNum)
{
	for(int i=0;i<(MAX_NUMBER_BIT-VisibleNum);i++)
	{
		long a =Power(10,(MAX_NUMBER_BIT-VisibleNum-i));
		long b =a/10;
		u_iBit[i] =	(Value % a)/b;
		
		ShowNumber(u_iBit[i],i+Line*MAX_NUMBER_BIT);
	}
}

void	SelectBitDB(long Value,int Line,int iPrecision)
{
	bool	bHighBitVisibleFlag=TRUE;
	for(int i=0;i<MAX_NUMBER_BIT;i++)
	{
		long a =Power(10,(MAX_NUMBER_BIT-i));
		long b =a/10;
		u_iBit[i] =	(Value % a)/b;

		if((i<(MAX_NUMBER_BIT-iPrecision-1))&&(u_iBit[i]==0)&&(bHighBitVisibleFlag))
		{
			for(int j =0; j<7;j++)
			{
				pwndNUMPART[(i+Line*MAX_NUMBER_BIT)*7+j] -> SetPropValueT("visible",(double)u_bvisibleF);
				pwndNUMPART[(i+Line*MAX_NUMBER_BIT)*7+j] -> Update();
			}
			continue;
		}
			
		ShowNumber(u_iBit[i],i+Line*MAX_NUMBER_BIT);
		bHighBitVisibleFlag =FALSE;
	}
}

void	AllControlMove(CtmWnd* pwndSender)
{
	short	TempLeft;
	short	TempRight;
	short	TempTop;
	short	TempBottom;

	u_pwndImageBG->GetPropValueT("left", &TempLeft,sizeof(TempLeft));
	u_pwndImageBG->GetPropValueT("right", &TempRight,sizeof(TempRight));
	u_pwndImageBG->GetPropValueT("top", &TempTop,sizeof(TempTop));
	u_pwndImageBG->GetPropValueT("bottom", &TempBottom,sizeof(TempBottom));
					
	if (u_lMoveIntervalTime <= 0)
	{
		u_lMoveIntervalTime = 500;
		if((TempRight>0)&&(TempRight<ViewWidth)&&(u_bHorizontalFlag))//往右移
		{
			TempLeft= TempLeft+1;
			u_pwndImageBG-> SetPropValueT("left",TempLeft); 
			TempRight= TempRight+1;                         
			u_pwndImageBG->SetPropValueT("right",TempRight);

			if(TempRight==(ViewWidth-1)) u_bHorizontalFlag =FALSE;
		}
		
		if((TempLeft>0)&&(TempLeft<ViewWidth)&&(!(u_bHorizontalFlag)))//往左移
		{
			TempLeft= TempLeft-1;
			u_pwndImageBG-> SetPropValueT("left",TempLeft); 
			TempRight= TempRight-1;                         
			u_pwndImageBG->SetPropValueT("right",TempRight);
			
			if(TempLeft==0) u_bHorizontalFlag =TRUE;
		}
		
		if((TempBottom>0)&&(TempBottom<ViewHeight)&&(u_bBverticalFlag))//往下移
		{
			TempTop= TempTop+1;
			u_pwndImageBG-> SetPropValueT("top",TempTop); 
			TempBottom= TempBottom+1;                         
			u_pwndImageBG ->SetPropValueT("bottom",TempBottom);
			
			if(TempBottom==(ViewHeight-1)) u_bBverticalFlag =FALSE;
		}

		if((TempTop>0)&&(TempTop<ViewHeight)&&(!(u_bBverticalFlag)))//往上移
		{
			TempTop= TempTop-1;
			u_pwndImageBG-> SetPropValueT("top",TempTop); 
			TempBottom= TempBottom-1;                         
			u_pwndImageBG->SetPropValueT("bottom",TempBottom);
			
			if(TempTop==0) u_bBverticalFlag =TRUE;
		}
		u_pwndImageBG->Update();
	}
}

void	GetOPStatus() 	//Sunny<20070428>判斷機器當前操作狀態
{
	u_wOPStatus = _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue) + 1;
	if(u_wOPStatus == 1)
	{
		for(int j = u_wOPStatus; j < 16; j++) 
		{
			if( _TestBit((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue,j))	
			{
				u_wOPStatus = j+1;
				break;
			}
		}
	}
	else if(u_wOPStatus == 0xFFFF)		u_wOPStatus =0;
}
