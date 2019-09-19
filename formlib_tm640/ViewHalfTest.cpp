/*===========================================================================+
|  Class    : ViewHalfTest library                                           |
|  Task     : ViewHalfTest library action source file                        |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 										                                           |
|  Version  : V1.00      			                                               |
|  Creation : 			                                            		         |
|  Revision :   																														 |
+===========================================================================*/
#include 	"ViewHalfTest.h"
#include	"../main.h"
#include	"../_kb.h"
#include 	"../common.h"
#include 	"../commonaction.h"
#include	"../timer.h"
#include	"../utils.h"
#include	"../manufacture.h"
#include	"../tmdebug.h"
#include	"../selvat_ancestor.h"
#include	"../tmconfig.h"
#include	"../lib/libiNetComm.h"
#include	"../tmshmsg.h"
#include	"../lib/libControl.h"
#include 	"../selectedit.h"
/*===========================================================================
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL		bLCD_Light_GOSTOP =FALSE;
BOOL		bLCD_Color_GOSTOP =FALSE;
int	uLCD_Light_TimeCount = 0;
int	uLCD_Color_TimeCount = 0;
int Bglight = 0;
int	Color = 0;

CtmWnd*			pwndStatic_LCD_Light = NULL;
CtmWnd*			pwndStatic_LCD_Color = NULL;
CtmWnd*			pwndBtn_LCD_Light = NULL;
CtmWnd*			pwndBtn_LCD_Color = NULL;
CtmWnd*			pwndResult_LCD_Light = NULL;
CtmWnd*			pwndResult_LCD_Color = NULL;
CtmWnd*			pwndImageBGLOGO = NULL;

char*	Str_GoStop[] =
{
	"GO",
	"STOP",
};

char*	BGpath[] =
{
	"res_tm640/pic/Background.bmp",
	"res_tm640/pic/BG_Red.bmp",
	"res_tm640/pic/BG_Green.bmp",
	"res_tm640/pic/BG_Blue.bmp",
	"res_tm640/pic/BG_White.bmp",
	"res_tm640/pic/BG_Black.bmp",
};

char*	Str_BGColor[] =
{
	"BG",
	" Red ",
	"Green",
	"Blue",
	"White",
	"Black",
};

/*---------------------------------------------------------------------------+
|           View Content - GuideSet                                          |
+---------------------------------------------------------------------------*/ 

/*---------------------------------------------------------------------------+
|  Function : OnCreateA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	pwndStatic_LCD_Light     = pwndSender->FindControlFromName("Static_LCD_Light");
	pwndStatic_LCD_Color     = pwndSender->FindControlFromName("Static_LCD_Color");
	pwndBtn_LCD_Light     = pwndSender->FindControlFromName("Btn_LCD_Light");
	pwndBtn_LCD_Color     = pwndSender->FindControlFromName("Btn_LCD_Color");
	pwndResult_LCD_Light	= pwndSender->FindControlFromName("Result_LCD_Light");
	pwndResult_LCD_Color	= pwndSender->FindControlFromName("Result_LCD_Color");
	pwndImageBGLOGO				= pwndSender->FindControlFromName("ImageBGLOGO");
	
	// Update String
	pwndBtn_LCD_Light->SetPropValueT("captionID",Str_GoStop[bLCD_Light_GOSTOP]);
	pwndBtn_LCD_Light->CreateA();
	pwndBtn_LCD_Light->Update();
	Show_LCD_Light_Result(7);
	pwndBtn_LCD_Color->SetPropValueT("captionID",Str_GoStop[bLCD_Color_GOSTOP]);
	pwndBtn_LCD_Color->CreateA();
	pwndBtn_LCD_Color->Update();
	Show_LCD_Color_Result(0);
	
	TestDRAM();
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
//WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
//{
//	CtmWnd* pWnd = NULL;
//	DWORD		dwTempValue = 0;
//
//	pWnd = pwndSender->FindControlFromTab(wIDControl);
//	if(pWnd == NULL) return wIDControl;
//		
//	return wIDControl;
//}

/*---------------------------------------------------------------------------+
|  Function : OnKeyA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	printf("key = %d\n", wKey);

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
	printf("OnMouseUp\n");	
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL)	return wIDControl;
	
	if(pwnd == pwndBtn_LCD_Light) // LCD Light Test
	{
		bLCD_Light_GOSTOP = !bLCD_Light_GOSTOP;
		printf("bLCD_Light_GOSTOP=%d\n",bLCD_Light_GOSTOP);
		pwndBtn_LCD_Light->SetPropValueT("captionID",Str_GoStop[bLCD_Light_GOSTOP]);
		pwndBtn_LCD_Light->CreateA();
		pwndBtn_LCD_Light->Update();
		if(!bLCD_Light_GOSTOP) // LCD Light Test STOP
			{
				uLCD_Light_TimeCount=0;
				LCD_Light_Test(7);
				Show_LCD_Light_Result(7);
			}
	}
	
	if(pwnd == pwndBtn_LCD_Color) // LCD Color Test
	{
		bLCD_Color_GOSTOP = !bLCD_Color_GOSTOP;
		printf("bLCD_Color_GOSTOP=%d\n",bLCD_Color_GOSTOP);
		pwndBtn_LCD_Color->SetPropValueT("captionID",Str_GoStop[bLCD_Color_GOSTOP]);
		pwndBtn_LCD_Color->CreateA();
		pwndBtn_LCD_Color->Update();
		if(!bLCD_Color_GOSTOP) // LCD Color Test STOP
			{
				uLCD_Color_TimeCount=0;
				LCD_Color_Test(0);
				Color=0;
				Show_LCD_Color_Result(0);
			}
	}

		
  return wIDControl;	
}




/*---------------------------------------------------------------------------+
|  Function : OnDestroyA()                     	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnDestroyA(CtmWnd* pwndSender)
{
	uLCD_Light_TimeCount=0;
	LCD_Light_Test(7);
	Show_LCD_Light_Result(7);
}

/*---------------------------------------------------------------------------+
|  Function : OnShowA()                        	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnShowA(CtmWnd* pwndSender)
{

}

/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateA(CtmWnd* pwndSender)
{
	if(bLCD_Light_GOSTOP) // LCD Light Test
	{
		//printf("count=%d\n",uLCD_Light_TimeCount);
		uLCD_Light_TimeCount++;
		if(uLCD_Light_TimeCount>30)
		{
			if(Bglight>10)
				Bglight=0;
			LCD_Light_Test(Bglight);
			// update Result string
			Show_LCD_Light_Result(Bglight);
			Bglight=Bglight+2;
			uLCD_Light_TimeCount=0;
		}
	}
	if(bLCD_Color_GOSTOP)
	{
		//printf("count=%d\n",uLCD_Color_TimeCount);
		uLCD_Color_TimeCount++;
		if(uLCD_Color_TimeCount>30)
		{
			Color++;
			if(Color>5)
				Color=1;
			LCD_Color_Test(Color);
			Show_LCD_Color_Result(Color);
			uLCD_Color_TimeCount=0;
		}
		
	}
	
	;
}

/*---------------------------------------------------------------------------+
|  Function : SendCommand()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SendCommand(int	CommandID)
{
	
	 	if(g_ptaskpicker != NULL)
 		{			
			g_ptaskpicker->ReqValues(6, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
		}
}


void	LCD_Light_Test(int Bglight)
{
	char cmd[128];
	sprintf(cmd,"./SetBL %d",Bglight);
	system(cmd);
}
void	Show_LCD_Light_Result(int Bglight)
{
	char strResult_LCD_light[256]	={NULL}; 
	sprintf(strResult_LCD_light,"Light %d %",Bglight*10);
	pwndResult_LCD_Light->SetPropValueT("text",strResult_LCD_light);
	pwndResult_LCD_Light->Show();
}

void	LCD_Color_Test(int Color)
{
	//printf("LCD Color Test %s\n",BGpath[Color]);
	pwndImageBGLOGO->SetPropValueT("imagepath",BGpath[Color]);
	//pwndImageBGLOGO->Update();
	pwndImageBGLOGO->Show();
	ShowItem();
}

void	ShowItem()
{
	pwndStatic_LCD_Light->CreateA();
	pwndStatic_LCD_Color->CreateA();
	pwndBtn_LCD_Light->CreateA();
	pwndBtn_LCD_Color->CreateA();
	pwndResult_LCD_Light->CreateA();
	pwndResult_LCD_Color->CreateA();
	
	pwndStatic_LCD_Light->Show();
	pwndStatic_LCD_Color->Show();
	pwndBtn_LCD_Light->Show();   
	pwndBtn_LCD_Color->Show();  
	pwndResult_LCD_Light->Show();
	pwndResult_LCD_Color->Show();
}

void	Show_LCD_Color_Result(int Color)
{
	printf("LCD Color Test %s\n",Str_BGColor[Color]);
	pwndResult_LCD_Color->SetPropValueT("text",Str_BGColor[Color]);
	pwndResult_LCD_Color->Show();
}

void TestDRAM()
	{
		printf("TestDRAM\n");
		FILE *fd;
		char buff[256];
		int total = 0;
		int free = 0;
		float usr =0;
		
		fd =fopen ("/proc/meminfo", "r");
		fgets (buff, sizeof(buff), fd);
		printf("buff=%s\n\n",buff);
		sscanf (buff, "%*s%d%*s", &total);

		fgets (buff, sizeof(buff), fd);
		printf("buff=%s\n\n",buff);
		sscanf (buff, "%*s%d%*s", &free);
		fclose(fd);
		printf("total=%d, free=%d\n",total,free);
	}