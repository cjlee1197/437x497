/*===========================================================================+
|  Class    : ViewTouchScreenAdjust  library                                 |
|  Task     : ViewTouchScreenAdjust  library header file                     |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) 3.2                                                   |
|  Link     : G++(GCC) 3.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James                                                          |
|  Version  : V1.00                                                          |
|  Creation : 01/25/2010                                                  	 |
|  Revision :                                                                |
+===========================================================================*/

#include 	"ViewTouchScreenAdjust.h"
#include 	"../utils.h"
#include	"../commonaction.h"
#include	"../touchscreen.h"


/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
#define		TOUCH_DOWN						0x01
#define		TOUCH_UP						0x02
#define     TOUCH_MOVE              		0x03 

#define		TOUCHDOWNTIME		1500
#define		TOUCHPOINTCOUNT		5
long		lTouchDownTime 		= 0;
long		lTouchIndex			= 0;
XY			u_TsPoints[TOUCHPOINTCOUNT];
XY			u_LcdPoints[TOUCHPOINTCOUNT];
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*		pwndPoint1 = NULL;
CtmWnd*		pwndPoint2 = NULL;
CtmWnd*		pwndPoint3 = NULL;
CtmWnd*		pwndPoint4 = NULL;
CtmWnd*		pwndPoint5 = NULL;

int			nAdjustState = 0;

void	MoveTouchCross5(long	lIndex);
void	MoveTouchCross3(long	lIndex);
/*===========================================================================+
|           Class implementation - ViewScreenSaver                           |
+===========================================================================*/

BOOL	OnCreateA(CtmWnd* pwndSender)
{	
	pwndPoint1 = pwndSender->FindControlFromName("ImagePonit1");
	AddTimer(&lTouchDownTime);
	
	nAdjustState = 1;
	if(g_pTouch != NULL)
		g_pTouch->SetAdjustState(nAdjustState);
	
	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = pwndSender->FindControlFromTab(wIDControl);
	return TRUE;
}

void	OnMessage(CtmWnd* pwnd, int message, WPARAM wParam, LPARAM lParam)
{
	short 	sX = 0, sY = 0;
	long	lWaitTime = lParam;
	static	BOOL	bStart = TRUE;
	int		nResult = 0;
	
	switch(message)
	{
		case MSG_MOUSELUP:
			if(lTouchIndex == TOUCHPOINTCOUNT)
			{
				if(g_pTouch != NULL)
				{
					nAdjustState = 0;
					nResult = g_pTouch->SetAdjustData(u_LcdPoints, u_TsPoints, TOUCHPOINTCOUNT);
					g_pTouch->SetAdjustState(nAdjustState);
				}
				if (nResult < 0)
					MsgBox(g_MultiLanguage["VM_MLD_OVERWRITE"], tmFT_CODE_TECH);
				Exit();
			}
		
			break;
		case MSG_MOUSELADJUST:
			printf("mouse adjust lTouchDownTime=%d lTouchIndex=%d \n", lTouchDownTime, lTouchIndex);
			if (lParam == TOUCH_DOWN)
				break;
			sX = wParam >> 16;
			sY = wParam & 0x0000FFFF;
			printf("click sX=%d sY=%d lWaitTime=%d lTouchDownTime=%d \n", sX, sY, lWaitTime, lTouchDownTime);
			bStart = TRUE;
			if(abs(lTouchDownTime) > TOUCHDOWNTIME)
			{
				u_TsPoints[lTouchIndex].x = sX;
				u_TsPoints[lTouchIndex].y = sY;
				lTouchIndex++;
				if (TOUCHPOINTCOUNT == 3)
					MoveTouchCross3(lTouchIndex);
				else
					MoveTouchCross5(lTouchIndex);
			}
			break;
	}
}

void	OnShowA(CtmWnd* pwndSender)
{
  	if (TOUCHPOINTCOUNT == 3)
		MoveTouchCross3(0);
	else
		MoveTouchCross5(0);
		
	return;
}

WORD 	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	int	nTab = ((CtmFormView*)pwndSender)->Get_TabStop();
	CtmWnd*	pwnd = pwndSender->FindControlFromTab(nTab);
	
	((CtmFormView*)pwndSender)->OnKey1(wKey);
	
	return wKey;
}
void	OnUpdateA(CtmWnd* pwndSender)
{

	return;
}

void	OnDestroyA(CtmWnd* pwndSender)
{	
	nAdjustState = 0;
	g_pTouch->SetAdjustState(nAdjustState);
	DeleteTimer(&lTouchDownTime);
}

/*===========================================================================+
|                                   Helper                                   |
+===========================================================================*/
void	MoveTouchCross5(long	lIndex)
{
	short	nLeft = 0, nTop = 0, right = 0, nBottom = 0;
	short   Picright = 20, PicnBottom = 20;
	if(pwndPoint1 != NULL)
	{
		switch(lIndex)
		{
			case 0:
				nLeft 	= Picright * 2;
				nTop 	= PicnBottom * 2;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->UpdateAll();
				
				break;
			case 1:
				nLeft 	= g_infoDrv.nX - Picright * 3;
				nTop 	= PicnBottom * 2;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->Update();
				
				break;
			case 2:
				nLeft 	= Picright * 2;
				nTop 	= g_infoDrv.nY- PicnBottom * 3;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->Update();
				
				break;
			case 3:
				nLeft 	= g_infoDrv.nX - Picright * 3;
				nTop 	= g_infoDrv.nY- PicnBottom * 3;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->Update();
				
				break;
			case 4:
				nLeft 	= (g_infoDrv.nX - Picright) / 2;
				nTop 	= (g_infoDrv.nY- PicnBottom) / 2;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->Update();
				
				break;
			default:
				return;
		}
		u_LcdPoints[lIndex].x = nLeft + Picright / 2;
		u_LcdPoints[lIndex].y = nTop + PicnBottom / 2;
		//printf("point%d:(%d, %d)\n", lIndex + 1, u_LcdPoints[lIndex].x, u_LcdPoints[lIndex].y);
	}
}

void	MoveTouchCross3(long	lIndex)
{
	short	nLeft = 0, nTop = 0, right = 0, nBottom = 0;
	short   Picright = 20, PicnBottom = 20;
	if(pwndPoint1 != NULL)
	{
		switch(lIndex)
		{
			case 0:
				nLeft 	= g_infoDrv.nX - Picright * 3;
				nTop 	= g_infoDrv.nY- PicnBottom * 3;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->UpdateAll();
				
				break;
			case 1:
				nLeft 	= (g_infoDrv.nX - Picright) / 2;
				nTop 	= (g_infoDrv.nY- PicnBottom) / 2;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->Update();
				
				break;
			case 2:
				nLeft 	= Picright * 2;
				nTop 	= PicnBottom * 2;
				right 	= nLeft + Picright;
				nBottom = nTop + PicnBottom;
				pwndPoint1->SetProperty("left", &nLeft);
				pwndPoint1->SetProperty("top", &nTop);
				pwndPoint1->SetProperty("right", &right);
				pwndPoint1->SetProperty("bottom", &nBottom);
				pwndPoint1->Update();
				
				break;
			default:
				return;
		}
		u_LcdPoints[lIndex].x = nLeft + Picright / 2;
		u_LcdPoints[lIndex].y = nTop + PicnBottom / 2;
		//printf("point%d:(%d, %d)\n", lIndex + 1, u_LcdPoints[lIndex].x, u_LcdPoints[lIndex].y);
	}
}