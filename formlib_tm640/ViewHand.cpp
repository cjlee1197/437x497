/*===========================================================================+
|  Class    : ViewHand library                                          	 |
|  Task     : ViewHand library action source file                       	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : 		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 03/17/2014                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewHand.h"
#include	"../database.h"
#include	"../init.h"
#include	"../selectedit.h"
#include	"../tmconfig.h"
//#include 	"../lib/hmi_control_data/tmControl_CurveExData.h"
#include	"../font.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		ON					1
#define		OFF					0

#define		SPEEDMAXNUM					10
#define		BUTTONMAXNUM				10
#define		AXISMAXNUM					5
#define		AXIS_X_SPEED				0
#define		AXIS_Y_SPEED				1
#define		AXIS_Z_SPEED				2
#define		AXIS_X2_SPEED				3
#define		AXIS_Y2_SPEED				4

#define			GPIO_SAFEKEY_BIT	0x14 //cjlee 
#define			GPIO_SAFEKEY_0 0x4
#define			GPIO_SAFEKEY_1 0x10
#define			GPIO_SAFEKEY_2 0x14
#define			MANUAL_CONTINUE_MODE	0
#define			MANUAL_JOG_MODE	1
#define			MANUAL_STOP_MODE	2

#define		COMMAND_X_FWD				0xFF01
#define		COMMAND_X_BACK			0xFF02
#define		COMMAND_Y_FWD				0xFF03
#define		COMMAND_Y_BACK			0xFF04
#define		COMMAND_Z_FWD				0xFF05
#define		COMMAND_Z_BACK			0xFF06
#define		COMMAND_X2_FWD			0xFF2E
#define		COMMAND_X2_BACK			0xFF2F	
#define		COMMAND_Y2_FWD			0xFF30
#define		COMMAND_Y2_BACK			0xFF31
#define		COMMAND_STOP				0xFFFF
#define		COMMAND_X_HOME			0xFF3B
#define		COMMAND_Y_HOME			0xFF3C
#define		COMMAND_Z_HOME			0xFF3D
#define		COMMAND_X2_HOME			0xFF3E
#define		COMMAND_Y2_HOME			0xFF3F

/*-------------------------------3354_IO定義----------------------------------*/
/*
#define		COMMAND_HORIZONTAL  0xFF07
#define		COMMAND_VERTICAL    0xFF08
#define		COMMAND_CLAMP1ON   	0xFF09
#define		COMMAND_CLAMP1OFF	  0xFF0A
#define		COMMAND_CLAMP2ON    0xFF0B  //linda20140902
#define		COMMAND_CLAMP2OFF   0xFF0C  //linda20140902

//#define		COMMAND_STATICON    0xFF0F  //andy20141101
//#define		COMMAND_STATICOFF   0xFF10  //andy20141101

#define		COMMAND_CLAMP3ON   	0xFF11
#define		COMMAND_CLAMP3OFF	  0xFF12
#define		COMMAND_CLAMP4ON    0xFF13  //linda20140902
#define		COMMAND_CLAMP4OFF   0xFF14  //linda20140902

#define		COMMAND_BLOW1ON   	0xFF15
#define		COMMAND_BLOW1OFF	  0xFF16
#define		COMMAND_BLOW2ON    	0xFF17  //linda20140902
#define		COMMAND_BLOW2OFF   	0xFF18  //linda20140902
#define		COMMAND_BLOW3ON   	0xFF19
#define		COMMAND_BLOW3OFF	  0xFF1A
#define		COMMAND_BLOW4ON    	0xFF1B  //linda20140902
#define		COMMAND_BLOW4OFF   	0xFF1C  //linda20140902

#define		COMMAND_FORWARD		  0xFF1D
#define		COMMAND_BACKWARD    0xFF1E

#define		COMMAND_VACUUMON    0xFF1F  //andy20141101
#define		COMMAND_VACUUMOFF   0xFF20  //andy20141101
#define		COMMAND_CONVEYERON  0xFF21  //andy20141101
#define		COMMAND_CONVEYEROFF 0xFF22  //andy20141101
#define		COMMAND_STATICON    0xFF23  //andy20141101
#define		COMMAND_STATICOFF   0xFF24  //andy20141101

#define		KEY_CLAMP1ON				0x0001
#define		KEY_CLAMP1OFF				0x0002
#define		KEY_CLAMP2ON		    0x0003  //linda20140902
#define		KEY_CLAMP2OFF			  0x0004  //linda20140902
#define		KEY_CLAMP3ON				0x0005
#define		KEY_CLAMP3OFF				0x0006
#define		KEY_CLAMP4ON		    0x0007  //linda20140902
#define		KEY_CLAMP4OFF			  0x0008  //linda20140902
#define		KEY_BLOW1ON					0x0009
#define		KEY_BLOW1OFF				0x000A
#define		KEY_BLOW2ON		    	0x000B  //linda20140902
#define		KEY_BLOW2OFF			  0x000C  //linda20140902

#define		KEY_BLOW3ON					0x000D
#define		KEY_BLOW3OFF				0x000E
#define		KEY_BLOW4ON		   		0x000F  //linda20140902
#define		KEY_BLOW4OFF			  0x0010  //linda20140902

#define		KEY_HORIZONTAL			0x0011
#define		KEY_VERICAL					0x0012
#define		KEY_FORWARD					0x0013
#define		KEY_BACKWARD				0x0014

#define		KEY_STATICON			  0x0015  //andy20141101
#define		KEY_STATICOFF			  0x0016  //andy20141101
#define		KEY_VACUUMON			  0x0017  //andy20141101
#define		KEY_VACUUMOFF			  0x0018  //andy20141101
#define		KEY_CONVEYERON			0x0019  //andy20141101
#define		KEY_CONVEYEROFF			0x001A  //andy20141101
*/
/*-------------------------------3354_IO定義----------------------------------*/

// cjlee add 2019/1/29 下午 01:43:30

/*-------------------------------哈鏌IO定義----------------------------------*/
/*
#define		COMMAND_HORIZONTAL  0xFF07 		//姿態
#define		COMMAND_VERTICAL    0xFF08

#define		COMMAND_CLAMP1ON     0xFF09
#define		COMMAND_CLAMP1OFF    0xFF0A
#define		COMMAND_CLAMP2ON     0xFF0B
#define		COMMAND_CLAMP2OFF    0xFF0C
#define		COMMAND_CLAMP3ON     0xFF11
#define		COMMAND_CLAMP3OFF    0xFF12
#define		COMMAND_CLAMP4ON     0xFF13
#define		COMMAND_CLAMP4OFF    0xFF14

#define		COMMAND_SUCK1ON      0xFF33
#define		COMMAND_SUCK1OFF     0xFF34
#define		COMMAND_SUCK2ON      0xFF35
#define		COMMAND_SUCK2OFF     0xFF36
#define		COMMAND_SUCK3ON      0xFF37
#define		COMMAND_SUCK3OFF     0xFF38
#define		COMMAND_SUCK4ON      0xFF39
#define		COMMAND_SUCK4OFF     0xFF3A

#define		COMMAND_VACUUMON     0xFF1F  //真空閥
#define		COMMAND_VACUUMOFF    0xFF20  
#define		COMMAND_CONVEYERON   0xFF21  //輸送帶
#define		COMMAND_CONVEYEROFF  0xFF22  
#define		COMMAND_STATICON     0xFF23  //打靜電
#define		COMMAND_STATICOFF    0xFF24  


#define		KEY_CLAMP1ON     0x0001
#define		KEY_CLAMP1OFF    0x0002
#define		KEY_CLAMP2ON     0x0003
#define		KEY_CLAMP2OFF    0x0004
#define		KEY_CLAMP3ON     0x0005
#define		KEY_CLAMP3OFF    0x0006
#define		KEY_CLAMP4ON     0x0007
#define		KEY_CLAMP4OFF    0x0008
#define		KEY_SUCK1ON      0x0009
#define		KEY_SUCK1OFF     0x000A
#define		KEY_SUCK2ON      0x000B
#define		KEY_SUCK2OFF     0x000C
#define		KEY_SUCK3ON      0x000D
#define		KEY_SUCK3OFF     0x000E
#define		KEY_SUCK4ON      0x000F
#define		KEY_SUCK4OFF     0x0010

#define		KEY_HORIZONTAL			0x0011
#define		KEY_VERICAL					0x0012

#define		KEY_STATICON			  0x0015
#define		KEY_STATICOFF			  0x0016
#define		KEY_VACUUMON			  0x0017
#define		KEY_VACUUMOFF			  0x0018
#define		KEY_CONVEYERON			0x0019
#define		KEY_CONVEYEROFF			0x001A
*/
/*-------------------------------哈鏌IO定義----------------------------------*/


/*---------------------------2019ChinaPlas_IO定義------------------------------*/
#define		COMMAND_HORIZONTAL  0xFF07 		//姿態
#define		COMMAND_VERTICAL    0xFF08

#define		COMMAND_CLAMP1ON     0xFF09
#define		COMMAND_CLAMP1OFF    0xFF0A
#define		COMMAND_CLAMP2ON     0xFF0B
#define		COMMAND_CLAMP2OFF    0xFF0C
#define		COMMAND_CLAMP3ON     0xFF11
#define		COMMAND_CLAMP3OFF    0xFF12
#define		COMMAND_CLAMP4ON     0xFF13
#define		COMMAND_CLAMP4OFF    0xFF14

#define		COMMAND_SUCK1ON      0xFF33
#define		COMMAND_SUCK1OFF     0xFF34
#define		COMMAND_SUCK2ON      0xFF35
#define		COMMAND_SUCK2OFF     0xFF36
#define		COMMAND_SUCK3ON      0xFF37
#define		COMMAND_SUCK3OFF     0xFF38
#define		COMMAND_SUCK4ON      0xFF39
#define		COMMAND_SUCK4OFF     0xFF3A

#define		COMMAND_VACUUMON     0xFF1F  //真空閥
#define		COMMAND_VACUUMOFF    0xFF20  
#define		COMMAND_CONVEYERON   0xFF21  //輸送帶
#define		COMMAND_CONVEYEROFF  0xFF22  
#define		COMMAND_STATICON     0xFF23  //打靜電
#define		COMMAND_STATICOFF    0xFF24  
#define		KEY_CLAMP1     0x0001
#define		KEY_CLAMP2     0x0003
#define		KEY_SUCK1      0x0009
#define		KEY_SUCK2      0x000B

#define		KEY_HORIZONTAL		0x0011 // 水平
#define		KEY_VERICAL				0x0012 // 垂直
#define		KEY_STATIC			  0x0015 // 靜電
#define		KEY_VACUUM			  0x0017 // 真空
#define		KEY_CONVEYER			0x0019 // 輸送帶
/*---------------------------2019ChinaPlas_IO定義------------------------------*/

#define		CONST_REQ_COMMAND   6

#define		STATE_HAND					3
#define     PB_MAXNUM						5
#define     PC_MAXNUM						5
#define		ID_MAX_SELECT		48
#define     STR_MAX_COUNT       256
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CtmWnd*		pwndButton_para       = NULL;
CtmWnd*		pwndButton_out     		= NULL; 

CtmWnd*		u_pwndHandSpeed[SPEEDMAXNUM] = {NULL};
CtmWnd*		u_pwndButtonHand[BUTTONMAXNUM]  = {NULL};
CtmWnd*		u_pwndButtonHorizontal = NULL;
CtmWnd*		u_pwndButtonVertical   = NULL;
CtmWnd*		u_pwndButtonClampOpen  = NULL;
CtmWnd*		u_pwndButtonClampClose = NULL;
//CtmWnd*		u_pwndButtonEnableOn   = NULL;
//CtmWnd*		u_pwndButtonEnableOff  = NULL;
CtmWnd*		u_pwndBtnServo 				 = NULL;
CtmWnd*   u_pwndValveState[16] ={NULL};
CtmWnd*   u_pwndINIO_State[16] ={NULL};
CtmWnd*   u_pwndEXIO_State[16] ={NULL};
CtmWnd*   u_pwndINIOBtn_State[16] ={NULL};
CtmWnd*   u_pwndEXIOBtn_State[16] ={NULL};


CtmWnd*		u_pwndBtnManualType0   = NULL;
CtmWnd*		u_pwndBtnManualType1   = NULL;

long lInOutValue_Old = -1;
long lExOutValue_Old = -1;

//CtmWnd*		u_pwndBmpEnableoff  = NULL;
//CtmWnd*		u_pwndBmpEnableon   = NULL;
CtmWnd*		u_pwndBmpServo  = NULL;

CtmWnd*			pwndEditPostionX = NULL;
CtmWnd*			pwndEditPostionY = NULL;
CtmWnd*			pwndEditPostionZ = NULL;
CtmWnd*			pwndEditPostionY2 = NULL;
CtmWnd*     u_pwndSelectBox[ID_MAX_SELECT];
long	u_lAxisSpeed[AXISMAXNUM];
WORD	u_wPickerOPSatus = 0;
int		u_nMANUAL_TYPE = 0; // MACHINE_CONFIGURATION_MANUAL_TYPE
char u_szPath[256] = "\0";
int 		u_nSelectBoxCount 	= 0;
char        u_acSelBoxString[ID_MAX_SELECT][STR_MAX_COUNT];

WORD wEnableStatus,wEnableStatus_Old;

DBVIEWVALUE u_dbInputValue;
WORD        u_wInputValue;

DBVIEWVALUE u_dbOutputValue;
WORD        u_wOutputValue;

/*int 	u_nCommand[] = { COMMAND_X_FWD, 		COMMAND_X_BACK, 		COMMAND_X_HOME,
                    	 COMMAND_Y_FWD, 		COMMAND_Y_BACK, 		COMMAND_Y_HOME,
	                     COMMAND_Z_FWD, 		COMMAND_Z_BACK, 		COMMAND_Z_HOME,
	                   };*/
int 	u_nCommand[] = { COMMAND_X_FWD,			COMMAND_X_BACK, 		
 											 COMMAND_Y_FWD,			COMMAND_Y_BACK,		 
 											 COMMAND_Z_FWD,			COMMAND_Z_BACK,		
 											 COMMAND_X2_FWD,		COMMAND_X2_BACK,		 		
 											 COMMAND_Y2_FWD, 		COMMAND_Y2_BACK,		   
	                   };

char*	u_pszAxisSpeedID[] = 
{
	"AXIS1_PROFILE_MAXSPEED",
	"AXIS2_PROFILE_MAXSPEED",
	"AXIS3_PROFILE_MAXSPEED",
	"AXIS4_PROFILE_MAXSPEED",
	"AXIS5_PROFILE_MAXSPEED",
	 NULL
};

char* u_pszRealSpeedID[] =
{
	"AXIS1_PROFILE_JOGFWD_FVELOCITY",
	"AXIS1_PROFILE_JOGBWD_FVELOCITY",
	"AXIS2_PROFILE_JOGFWD_FVELOCITY",
	"AXIS2_PROFILE_JOGBWD_FVELOCITY",
	"AXIS3_PROFILE_JOGFWD_FVELOCITY",
	"AXIS3_PROFILE_JOGBWD_FVELOCITY",
	"AXIS4_PROFILE_JOGFWD_FVELOCITY",
	"AXIS4_PROFILE_JOGBWD_FVELOCITY",
	"AXIS5_PROFILE_JOGFWD_FVELOCITY",
	"AXIS5_PROFILE_JOGBWD_FVELOCITY",
	 NULL
};

char*	u_pszControlName[] =
{				
	"EditSet_X_FWDSpeed",
	"EditSet_X_BackSpeed",
	"EditSet_Y_FWDSpeed",
	"EditSet_Y_BackSpeed",
	"EditSet_Z_FWDSpeed",
	"EditSet_Z_BackSpeed",
	"EditSet_X2_FWDSpeed",
	"EditSet_X2_BackSpeed",
	"EditSet_Y2_FWDSpeed",
	"EditSet_Y2_BackSpeed",
	 NULL
};

/*char*	u_pszButtonName[] =
{
	"Button_X_FWD",
	"Button_X_Back",
	"Button_X_Home",
	"Button_Y_FWD",
	"Button_Y_Back",
	"Button_Y_Home",
	"Button_Z_FWD",
	"Button_Z_Back",
	"Button_Z_Home",
	 NULL 
};*/
char*	u_pszButtonName[] =
{
	"Button_X_FWD",
	"Button_X_Back",
	"Button_Y_FWD",
	"Button_Y_Back",
	"Button_Z_FWD",
	"Button_Z_Back",
	"Button_X2_FWD",
	"Button_X2_Back",
	"Button_Y2_FWD",
	"Button_Y2_Back",
	 NULL 
};
// u_pszImageBoxString for HiMore cjlee add 2019/1/29 上午 10:52:15

char* u_pszImageBoxString_INIO[] =
{
	"BmpPileVertical",
	"BmpPileHorizontal",
	"BmpClamp1",
	"BmpClamp2",
	"BmpClamp3",
	"BmpClamp4",
	"BmpBlow1",	
	"BmpBlow2",
	"BmpBlow3",
	"BmpBlow4",
	"",
	"",
	"",
	"",
	"",
	"BmpVacuum",
};

char* u_pszImageBoxString_EXIO[] =
{
	"BmpStatic",
	"",
	"BmpConveyor",
};

char* u_pszBtnImgString_INIO[] =
{
	"",
	"",
	"Btn_Clamp1",
	"Btn_Clamp2",
	"Btn_Clamp3",
	"Btn_Clamp4",
	"Btn_Suck1",	
	"Btn_Suck2",
	"Btn_Suck3",
	"Btn_Suck4",
	"",
	"",
	"",
	"",
	"",
	"Btn_VACUUMON",
};
char* u_pszBtnImgString_EXIO[] =
{
	"Btn_STATICON",
	"",
	"Btn_CONVEYERON",
};
char* BtnImgOn_INIO[] = // 按鈕ON 圖片路徑
{
	"",
	"",
	"clamp1_on.bmp",
	"clamp2_on.bmp",
	"",
	"",
	"CupulaOpen1.bmp",	
	"CupulaOpen2.bmp",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"vacuum_on.bmp",
};
char* BtnImgOn_EXIO[] = // 按鈕OFF 圖片路徑
{
	"static_on.bmp",
	"",
	"vonveyor_on.bmp",
};
char* BtnImgOff_INIO[] =
{
	"",
	"",
	"clamp1_off.bmp",
	"clamp2_off.bmp",
	"",
	"",
	"CupulaClose1.bmp",	
	"CupulaClose2.bmp",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"vacuum_off.bmp",
};
char* BtnImgOff_EXIO[] =
{
	"static_off.bmp",
	"",
	"vonveyor_off.bmp",
};


/*
char* u_pszImageBoxString[] =
{
	"BmpClamp1",
	"BmpBlow1",
	"BmpClamp2",
	"BmpBlow2",
	"BmpClamp3",
	"BmpBlow3",
	"BmpClamp4",
	"BmpBlow4",
	"BmpStatic",
	"BmpConveyor",
	"BmpVacuum",
	"",
	"BmpPileHorizontal",
	"BmpPileVertical",
	"BmpPileForward",
	"BmpPileBackward",
};*/
long	Btn_bgc[] =
{
	0xD6BA,
	0xEF32,
};
long	Btn_fgc[] =
{
	//0x8CD1, //gray
	//0x4BAE,
	0x0000,
	0x0000,
};
				

/*---------------------------------------------------------------------------+
|           View Content - Hand                                              |
+---------------------------------------------------------------------------*/  
/*---------------------------------------------------------------------------+
|  Function : OnCreateA()                      	     	                       |
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
	pwndButton_para  = pwndSender->FindControlFromName("Button_para");  
	pwndButton_out   = pwndSender->FindControlFromName("Button_out"); 
	
	int	nTemp = -2;
  strncpy(u_szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
	
  pwndEditPostionX = pwndSender->FindControlFromName("EditXposition");
  pwndEditPostionY = pwndSender->FindControlFromName("EditYposition");
  pwndEditPostionZ = pwndSender->FindControlFromName("EditZposition");
  pwndEditPostionY2 = pwndSender->FindControlFromName("EditY2position");
	
	u_wPickerOPSatus = GetDBValue("MACHINE_INTERFACE_WOPERATINGSTATE").lValue;
	
	for(int i = 0; i < BUTTONMAXNUM; i++)
	{
 		u_pwndButtonHand[i] = pwndSender->FindControlFromName(u_pszButtonName[i]);
 		//printf("Set u_pwndButtonHand[%d]=%s\n",i,u_pszButtonName[i]);
	}
 	
	for(int i = 0; i < SPEEDMAXNUM; i++)
	{
		u_pwndHandSpeed[i] 	= pwndSender->FindControlFromName(u_pszControlName[i]);
	}
	
	u_pwndButtonHorizontal = pwndSender->FindControlFromName("Button_Horizontal");
	u_pwndButtonVertical   = pwndSender->FindControlFromName("Button_Vertical");
	u_pwndButtonClampOpen  = pwndSender->FindControlFromName("Button_ClampOpen");
	u_pwndButtonClampClose = pwndSender->FindControlFromName("Button_ClampClose");
	//u_pwndButtonEnableOn   = pwndSender->FindControlFromName("BtnEnableOn");
	//u_pwndButtonEnableOff  = pwndSender->FindControlFromName("BtnEnableOff");
	u_pwndBtnServo				 = pwndSender->FindControlFromName("BtnServo");
	
	u_nSelectBoxCount = GetSpecialControlNum(pwndSender, (char*)NULL, "CtmSelectBox", &u_pwndSelectBox[0]);//2017-8-3 16:11:41
	
	//u_pwndBmpEnableoff = pwndSender->FindControlFromName("BmpEnableoff");
	//u_pwndBmpEnableon  = pwndSender->FindControlFromName("BmpEnableon");
	u_pwndBmpServo		 = pwndSender->FindControlFromName("BmpServo");
	
	u_pwndBtnManualType0  = pwndSender->FindControlFromName("ManualType0");
	u_pwndBtnManualType1  = pwndSender->FindControlFromName("ManualType1");
	
//	for(int i = 0; i < sizeof(u_pszImageBoxString)/sizeof(u_pszImageBoxString[0]); i++ )
//	{
//		u_pwndValveState[i] = pwndSender->FindControlFromName(u_pszImageBoxString[i]);
//	}
	
	for(int i = 0; i < sizeof(u_pszImageBoxString_INIO)/sizeof(u_pszImageBoxString_INIO[0]); i++ )
	{
		u_pwndINIO_State[i] = pwndSender->FindControlFromName(u_pszImageBoxString_INIO[i]);
	}
	for(int i = 0; i < sizeof(u_pszImageBoxString_EXIO)/sizeof(u_pszImageBoxString_EXIO[0]); i++ )
	{
		u_pwndEXIO_State[i] = pwndSender->FindControlFromName(u_pszImageBoxString_EXIO[i]);
	}
	int GetIOBtn =0;
	for(int i = 0; i < sizeof(u_pszBtnImgString_INIO)/sizeof(u_pszBtnImgString_INIO[0]); i++ )
	{
		u_pwndINIOBtn_State[i] = pwndSender->FindControlFromName(u_pszBtnImgString_INIO[i]);
		if(u_pwndINIOBtn_State[i]!=NULL) GetIOBtn++;
	}
	printf("GetIOBtn=%d\n",GetIOBtn);
	for(int i = 0; i < sizeof(u_pszBtnImgString_EXIO)/sizeof(u_pszBtnImgString_EXIO[0]); i++ )
	{
		u_pwndEXIOBtn_State[i] = pwndSender->FindControlFromName(u_pszBtnImgString_EXIO[i]);
	}
	
	
	for(int i = 0; i < AXISMAXNUM; i++)
	{
		u_lAxisSpeed[i] = GetDBValue(u_pszAxisSpeedID[i]).lValue;
	}
	
  	if(u_wPickerOPSatus != STATE_HAND)
  	{
  		for(int i = 0; i < BUTTONMAXNUM; i++ )
  		{
  			if(u_pwndButtonHand[i] != NULL)
  			{
  				u_pwndButtonHand[i]->SetTabOrder(nTemp);
  				//u_pwndButtonHand[i]->OnLoseFocus();
  		  	}
  		}
  		if(u_pwndButtonHorizontal!=NULL)
  			u_pwndButtonHorizontal->SetTabOrder(nTemp);
  			
  		if(u_pwndButtonVertical!=NULL)
  			u_pwndButtonVertical->SetTabOrder(nTemp);
  			
  		if(u_pwndButtonClampOpen!=NULL)
  			u_pwndButtonClampOpen->SetTabOrder(nTemp);
  			
  		if(u_pwndButtonClampClose!=NULL)
  			u_pwndButtonClampClose->SetTabOrder(nTemp);
    }
    
	//CreateLEDSet();
	//CreateStringSet();
	
	// 首次刷新使能LED
	wEnableStatus= (WORD)GetDBValue("MACHINE_INTERFACE_MOTORENABLESTATE").lValue;
	//if((u_pwndBmpEnableon!=NULL)&&(u_pwndBmpEnableoff!=NULL))
	if((u_pwndBmpServo!=NULL)&&(u_pwndBtnServo!=NULL))
	{
		u_pwndBtnServo->CreateA();
		u_pwndBtnServo->Update();
		if(wEnableStatus)
		{
			//u_pwndBmpEnableon->SetPropValueT("imagepath", "res_tm640/pic/picker/selected.bmp");
			//u_pwndBmpEnableoff->SetPropValueT("imagepath", "res_tm640/pic/picker/unselected.bmp");
			u_pwndBmpServo->SetPropValueT("imagepath", "res_tm640/pic/picker/selected.bmp");
			((CtmButton*)u_pwndBtnServo)->Press(tmBUTTON_DOWN);
		}
		else
		{
			//u_pwndBmpEnableon->SetPropValueT("imagepath", "res_tm640/pic/picker/unselected.bmp");
			//u_pwndBmpEnableoff->SetPropValueT("imagepath", "res_tm640/pic/picker/selected.bmp");
			u_pwndBmpServo->SetPropValueT("imagepath", "res_tm640/pic/picker/unselected.bmp");
			((CtmButton*)u_pwndBtnServo)->Press(tmBUTTON_UP);
		}
		//u_pwndBmpEnableon->Update();
		//u_pwndBmpEnableoff->Update();
		u_pwndBmpServo->Update();
	}
	
	// 取得手動模式
	if(u_pwndBtnManualType0 != NULL && u_pwndBtnManualType1 != NULL)
		{
			SetManualMode();
		}

	SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 3);//進入手動畫面送3 離開送0 主機判斷
				
	u_nMANUAL_TYPE = GetDBValue("MACHINE_CONFIGURATION_MANUAL_TYPE").lValue;

	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnChangeA()                      	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pWnd = NULL;

	pWnd = pwndSender->FindControlFromTab(wIDControl);
	if(pWnd == NULL) return wIDControl;

	SetSpeedToDB(pWnd);	
	
	return wIDControl;
} 

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
  //printf("key = %d\n", wKey);
	switch(wKey)
	{
  	case 70: // Home
			::PutCommand("Over.txt"); // cjlee 2019/4/17 下午 06:56:41
			break;
		case 71: // Index
			::PutCommand("Index.txt");
			break;
		case 72: // Back
			::PutCommand("Index.txt");
			break;
	}
  
  
  // 3354_IO定義
  /*
	switch(wKey)
	{
		case KEY_HORIZONTAL:
			SendCommand(COMMAND_HORIZONTAL);
			break;
		case KEY_VERICAL:
			SendCommand(COMMAND_VERTICAL);
			break;
		case KEY_FORWARD:
			SendCommand(COMMAND_FORWARD);
			break;
		case KEY_BACKWARD:
			SendCommand(COMMAND_BACKWARD);
			break;
		case KEY_CLAMP1ON:
			SendCommand(COMMAND_CLAMP1ON);
			break;
		case KEY_CLAMP1OFF:
			SendCommand(COMMAND_CLAMP1OFF);
			break;
		case KEY_CLAMP2ON:
			SendCommand(COMMAND_CLAMP2ON);//linda20140902
			break;	
		case KEY_CLAMP2OFF:
			SendCommand(COMMAND_CLAMP2OFF);//linda20140902
			break;
		case KEY_CLAMP3ON:
			SendCommand(COMMAND_CLAMP3ON);
			break;
		case KEY_CLAMP3OFF:
			SendCommand(COMMAND_CLAMP3OFF);
			break;
		case KEY_CLAMP4ON:
			SendCommand(COMMAND_CLAMP4ON);//linda20140902
			break;	
		case KEY_CLAMP4OFF:
			SendCommand(COMMAND_CLAMP4OFF);//linda20140902
			break;
		case KEY_BLOW1ON:
			SendCommand(COMMAND_BLOW1ON);
			break;
		case KEY_BLOW1OFF:
			SendCommand(COMMAND_BLOW1OFF);
			break;
		case KEY_BLOW2ON:
			SendCommand(COMMAND_BLOW2ON);
			break;
		case KEY_BLOW2OFF:
			SendCommand(COMMAND_BLOW2OFF);
			break;
		case KEY_BLOW3ON:
			SendCommand(COMMAND_BLOW3ON);
			break;
		case KEY_BLOW3OFF:
			SendCommand(COMMAND_BLOW3OFF);
			break;
		case KEY_BLOW4ON:
			SendCommand(COMMAND_BLOW4ON);
			break;
		case KEY_BLOW4OFF:
			SendCommand(COMMAND_BLOW4OFF);
			break;							
		case KEY_STATICON:
			SendCommand(COMMAND_STATICON);//andy20141101
			break;	
		case KEY_STATICOFF:
			SendCommand(COMMAND_STATICOFF);//andy20141101
			break;	
		case KEY_VACUUMON:
			SendCommand(COMMAND_VACUUMON);//andy20141101
			break;	
		case KEY_VACUUMOFF:
			SendCommand(COMMAND_VACUUMOFF);//andy20141101
			break;
		case KEY_CONVEYERON:
			SendCommand(COMMAND_CONVEYERON);//andy20141101
			break;	
		case KEY_CONVEYEROFF:
			SendCommand(COMMAND_CONVEYEROFF);//andy20141101
			break;
//		case 97:
//			::PutCommand("Hand.txt");
//			break;
//		case 98:
//			::PutCommand("GuideSet.txt");
//			break;
//		case 99:
//			::PutCommand("MldS.txt");
//			break;
//		case 100:
//			Login("password_touch.txt");
//    		if(g_WndLoginFlag)
//				::PutCommand("Syst.txt");
//			break;
		default:
			break;
	}
	*/
	/*-------------------------------哈鏌IO定義----------------------------------*/
	/*
	switch(wKey)
	{
		case KEY_HORIZONTAL:
			SendCommand(COMMAND_HORIZONTAL);
			break;
		case KEY_VERICAL:
			SendCommand(COMMAND_VERTICAL);
			break;
		case KEY_CLAMP1ON:
			SendCommand(COMMAND_CLAMP1ON);
			break;
		case KEY_CLAMP1OFF:
			SendCommand(COMMAND_CLAMP1OFF);
			break;
		case KEY_CLAMP2ON:
			SendCommand(COMMAND_CLAMP2ON);
			break;	
		case KEY_CLAMP2OFF:
			SendCommand(COMMAND_CLAMP2OFF);
			break;
		case KEY_CLAMP3ON:
			SendCommand(COMMAND_CLAMP3ON);
			break;
		case KEY_CLAMP3OFF:
			SendCommand(COMMAND_CLAMP3OFF);
			break;
		case KEY_CLAMP4ON:
			SendCommand(COMMAND_CLAMP4ON);
			break;	
		case KEY_CLAMP4OFF:
			SendCommand(COMMAND_CLAMP4OFF);
			break;
		case KEY_SUCK1ON:
			SendCommand(COMMAND_SUCK1ON);
			break;
		case KEY_SUCK2ON:
			SendCommand(COMMAND_SUCK2ON);
			break;
		case KEY_SUCK3ON:
			SendCommand(COMMAND_SUCK3ON);
			break;
		case KEY_SUCK4ON:
			SendCommand(COMMAND_SUCK4ON);
			break;
		case KEY_SUCK1OFF:
			SendCommand(COMMAND_SUCK1OFF);
			break;
		case KEY_SUCK2OFF:
			SendCommand(COMMAND_SUCK2OFF);
			break;
		case KEY_SUCK3OFF:
			SendCommand(COMMAND_SUCK3OFF);
			break;
		case KEY_SUCK4OFF:
			SendCommand(COMMAND_SUCK4OFF);
			break;
		case KEY_STATICON:
			SendCommand(COMMAND_STATICON);
			break;	
		case KEY_STATICOFF:
			SendCommand(COMMAND_STATICOFF);
			break;	
		case KEY_VACUUMON:
			SendCommand(COMMAND_VACUUMON);
			break;	
		case KEY_VACUUMOFF:
			SendCommand(COMMAND_VACUUMOFF);
			break;
		case KEY_CONVEYERON:
			SendCommand(COMMAND_CONVEYERON);
			break;	
		case KEY_CONVEYEROFF:
			SendCommand(COMMAND_CONVEYEROFF);//andy20141101
			break;
		default:
			break;
	}
	*/
	/*-------------------------------哈鏌IO定義----------------------------------*/
	
	/*---------------------------2019ChinaPlas_IO定義------------------------------*/
	long lInOutValue = 0;
	long lExOutValue = 0;
	lInOutValue   = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE").lValue;	
	lExOutValue   = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE").lValue;	
	switch(wKey)
	{
		case KEY_HORIZONTAL: // 水平
			SendCommand(COMMAND_HORIZONTAL);
			break;
		case KEY_VERICAL: // 垂直
			SendCommand(COMMAND_VERTICAL);
			break;
		case KEY_CLAMP1: // 夾1
			if(_TestBit(lInOutValue, 3-1)) 
				{
					SendCommand(COMMAND_CLAMP1OFF);
				}
			else 
				{
					SendCommand(COMMAND_CLAMP1ON);
				}
			break;
		case KEY_CLAMP2: // 夾2
			if(_TestBit(lInOutValue, 4-1)) SendCommand(COMMAND_CLAMP2OFF);
			else SendCommand(COMMAND_CLAMP2ON);		
			break;
		case KEY_SUCK1: // 吸1
			if(_TestBit(lInOutValue, 7-1)) SendCommand(COMMAND_SUCK1OFF);
			else SendCommand(COMMAND_SUCK1ON);	
			break;
		case KEY_SUCK2: // 吸2
			if(_TestBit(lInOutValue, 8-1)) SendCommand(COMMAND_SUCK2OFF);
			else SendCommand(COMMAND_SUCK2ON);
			break;
		case KEY_STATIC:// 靜電
			if(_TestBit(lExOutValue, 1-1)) SendCommand(COMMAND_STATICOFF);
			else SendCommand(COMMAND_STATICON);
			break;		
		case KEY_VACUUM: // 真空
			if(_TestBit(lInOutValue, 16-1)) SendCommand(COMMAND_VACUUMOFF);
			else SendCommand(COMMAND_VACUUMON);
			break;	
		case KEY_CONVEYER: // 輸送帶
			if(_TestBit(lExOutValue, 3-1)) SendCommand(COMMAND_CONVEYEROFF);
			else SendCommand(COMMAND_CONVEYERON);
			break;	
		default:
			break;
	}
	/*---------------------------2019ChinaPlas_IO定義------------------------------*/
	
	
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}
/*---------------------------------------------------------------------------+
|  Function : SetSpeedToDB()                   	     	                       |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	SetSpeedToDB(CtmWnd* pWnd)
{
	WORD	wSpeedPercent = 0;
	long	lSpeedValue = 0;

	for(int i = 0; i < SPEEDMAXNUM; i++)
	{
		if(pWnd == u_pwndHandSpeed[i])
		{
			u_pwndHandSpeed[i]->GetPropValueT("value", &wSpeedPercent, sizeof(WORD));
			if(i < 2)
			{
			 	lSpeedValue = u_lAxisSpeed[AXIS_X_SPEED]*wSpeedPercent/100;
			 
			  printf("u_lAxisSpeed[X] = %d, wSpeedPercent = %d, speed = %d\n", u_lAxisSpeed[AXIS_X_SPEED],wSpeedPercent,lSpeedValue);
		  	}
		  else if(i >= 2 && i <= 3)
		  {
		  	lSpeedValue = u_lAxisSpeed[AXIS_Y_SPEED]*wSpeedPercent/100;
		  	
        printf("u_lAxisSpeed[Y] = %d, wSpeedPercent = %d, speed = %d\n", u_lAxisSpeed[AXIS_Y_SPEED],wSpeedPercent,lSpeedValue);
		  }
		  else if(i > 3 && i <= 5)
		  {
		  	lSpeedValue = u_lAxisSpeed[AXIS_Z_SPEED]*wSpeedPercent/100;
        printf("u_lAxisSpeed[Z] = %d, wSpeedPercent = %d, speed = %d\n", u_lAxisSpeed[AXIS_Z_SPEED],wSpeedPercent,lSpeedValue);
		  }
		  else if(i > 5 && i <= 7)
		  {
		  	lSpeedValue = u_lAxisSpeed[AXIS_X2_SPEED]*wSpeedPercent/100;
        printf("u_lAxisSpeed[X2] = %d, wSpeedPercent = %d, speed = %d\n", u_lAxisSpeed[AXIS_X2_SPEED],wSpeedPercent,lSpeedValue);
		  }
		  else if(i > 7 && i <= 9)
		  {
		  	lSpeedValue = u_lAxisSpeed[AXIS_Y2_SPEED]*wSpeedPercent/100;
        printf("u_lAxisSpeed[Y2] = %d, wSpeedPercent = %d, speed = %d\n", u_lAxisSpeed[AXIS_Y2_SPEED],wSpeedPercent,lSpeedValue);
		  }
		  SetDBValue(u_pszRealSpeedID[i],lSpeedValue,TRUE);
		 // printf(" Set lSpeedValue = %d!\n", lSpeedValue);
		}
	}
	return TRUE;
}

/*---------------------------------------------------------------------------+
|  Function : OnShowA()                       	     	                       |
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
|  Function : OnDestroyA()                       	     	                     |
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
	SetDBValue("MACHINE_FUNCTION_OPTIONS_RSV04", 0);//進入手動畫面送3 離開送0 主機判斷
}

/*---------------------------------------------------------------------------+
|  Function : OnUpdateA()                        	     	                     |
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
	//long lTempValue = 0;
	long lInOutValue = 0;
	long lExOutValue = 0;
	long lInInValue = 0;
	long lExInValue = 0;
	
	BOOL 	bSelected = FALSE;
	char  pszPath[256] = "\0";
  //CreateLEDSet();
  SetEditValue(pwndEditPostionX);
  SetEditValue(pwndEditPostionY);
  SetEditValue(pwndEditPostionZ);
  SetEditValue(pwndEditPostionY2);
  
	wEnableStatus= (WORD)GetDBValue("MACHINE_INTERFACE_MOTORENABLESTATE").lValue;
	//if((u_pwndBmpEnableon!=NULL)&&(u_pwndBmpEnableoff!=NULL)&&(wEnableStatus!=wEnableStatus_Old))
	if((u_pwndBmpServo!=NULL)&&(u_pwndBtnServo!=NULL)&&(wEnableStatus!=wEnableStatus_Old))			
	{
		printf("wEnableStatus=%d,wEnableStatus_Old=%d\n",wEnableStatus,wEnableStatus_Old);
		u_pwndBtnServo->CreateA();
		u_pwndBtnServo->Update();
		if(wEnableStatus)
		{
			//u_pwndBmpEnableon->SetPropValueT("imagepath", "res_tm640/pic/picker/selected.bmp");
			//u_pwndBmpEnableoff->SetPropValueT("imagepath", "res_tm640/pic/picker/unselected.bmp");
			u_pwndBmpServo->SetPropValueT("imagepath", "res_tm640/pic/picker/selected.bmp");
			((CtmButton*)u_pwndBtnServo)->Press(tmBUTTON_DOWN);
		}
		else
		{
			//u_pwndBmpEnableon->SetPropValueT("imagepath", "res_tm640/pic/picker/unselected.bmp");
			//u_pwndBmpEnableoff->SetPropValueT("imagepath", "res_tm640/pic/picker/selected.bmp");
			u_pwndBmpServo->SetPropValueT("imagepath", "res_tm640/pic/picker/unselected.bmp");
			((CtmButton*)u_pwndBtnServo)->Press(tmBUTTON_UP);
		}
		//u_pwndBmpEnableon->Update();
		//u_pwndBmpEnableoff->Update();
		u_pwndBmpServo->Update();
		wEnableStatus_Old = wEnableStatus;
	}
	
//	lTempValue   = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE").lValue;	
//	for(int i = 0; i < sizeof(u_pszImageBoxString)/sizeof(u_pszImageBoxString[0]); i++)
//	{
//		if(_TestBit(lTempValue, i)) sprintf(pszPath,"%spic/picker/selected.bmp", /*"res_PICKER/image/picker/selected.bmp"*/u_szPath);
//		else sprintf(pszPath, "%spic/picker/unselected.bmp", /*"res_PICKER/image/picker/unselected.bmp"*/u_szPath);
//		 if(u_pwndValveState[i] != NULL)
//		 {
//			 //u_pwndValveState[i]->SetPropValueT("selected", bSelected);
//			 u_pwndValveState[i]->SetPropValueT("imagepath", pszPath);
//			 u_pwndValveState[i]->Update();
//			 
//			// ChangeImage(u_pwndValveState[i], u_pszSelectImagePath[wMotorStatus]);
//		 }
//	}	
	
	lInOutValue   = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE").lValue;	
	lInInValue 	  = GetDBValue("MACHINE_INTERFACE_DWEXTERNALINPUTSTATE").lValue;	
	lExInValue 	  = GetDBValue("MACHINE_INTERFACE_DWINTERNALINPUTSTATE").lValue;
	
	if(lInOutValue != lInOutValue_Old)	
		{
			for(int i = 0; i < sizeof(u_pszImageBoxString_INIO)/sizeof(u_pszImageBoxString_INIO[0]); i++)
			{
				if(_TestBit(lInOutValue, i)) sprintf(pszPath,"%spic/picker/selected.bmp", /*"res_PICKER/image/picker/selected.bmp"*/u_szPath);
				else sprintf(pszPath, "%spic/picker/unselected.bmp", /*"res_PICKER/image/picker/unselected.bmp"*/u_szPath);
				 if(u_pwndINIO_State[i] != NULL)
				 {
					 u_pwndINIO_State[i]->SetPropValueT("imagepath", pszPath);
					 u_pwndINIO_State[i]->Update();
				 }
			}		
			for(int i = 0; i < sizeof(u_pszBtnImgString_INIO)/sizeof(u_pszBtnImgString_INIO[0]); i++)
			{
				if(_TestBit(lInOutValue, i)) sprintf(pszPath,"%spic/picker/%s", u_szPath, BtnImgOn_INIO[i]);
				else sprintf(pszPath, "%spic/picker/%s", u_szPath, BtnImgOff_INIO[i]);
				 if(u_pwndINIOBtn_State[i] != NULL)
				 {
				 	 printf("Set %s = %s\n",u_pszBtnImgString_INIO[i],pszPath);
					 u_pwndINIOBtn_State[i]->SetPropValueT("upbitmap", pszPath);
					 u_pwndINIOBtn_State[i]->CreateA();
					 u_pwndINIOBtn_State[i]->Update();
				 }
			}	
			printf("lInOutValue=%x\n",lInOutValue);
			printf("lInInValue=%x\n",lInInValue);
			printf("lExInValue=%x\n",lExInValue);
			
			lInOutValue_Old = lInOutValue;
		}	


	lExOutValue   = GetDBValue("MACHINE_INTERFACE_DWEXTERNALOUTPUTSTATE").lValue;	
	if(lExOutValue != lExOutValue_Old)	
		{
			for(int i = 0; i < sizeof(u_pszImageBoxString_EXIO)/sizeof(u_pszImageBoxString_EXIO[0]); i++)
			{
				if(_TestBit(lExOutValue, i)) sprintf(pszPath,"%spic/picker/selected.bmp", /*"res_PICKER/image/picker/selected.bmp"*/u_szPath);
				else sprintf(pszPath, "%spic/picker/unselected.bmp", /*"res_PICKER/image/picker/unselected.bmp"*/u_szPath);
				 if(u_pwndEXIO_State[i] != NULL)
				 {
					 u_pwndEXIO_State[i]->SetPropValueT("imagepath", pszPath);
					 u_pwndEXIO_State[i]->Update();
				 }
			}	
			for(int i = 0; i < sizeof(u_pszBtnImgString_EXIO)/sizeof(u_pszBtnImgString_EXIO[0]); i++)
			{
				if(_TestBit(lExOutValue, i)) sprintf(pszPath,"%spic/picker/%s", u_szPath, BtnImgOn_EXIO[i]);
				else sprintf(pszPath, "%spic/picker/%s", u_szPath, BtnImgOff_EXIO[i]);
				 if(u_pwndEXIOBtn_State[i] != NULL)
				 {
				 	printf("Set %s = %s\n",u_pszBtnImgString_EXIO[i],pszPath);
					 u_pwndEXIOBtn_State[i]->SetPropValueT("upbitmap", pszPath);
					 u_pwndEXIOBtn_State[i]->CreateA();
					 u_pwndEXIOBtn_State[i]->Update();
				 }
			}	
			printf("lInOutValue=%x\n",lInOutValue);
			printf("lInInValue=%x\n",lInInValue);
			printf("lExInValue=%x\n",lExInValue);
			lExOutValue_Old = lExOutValue;
		}


}

/*---------------------------------------------------------------------------+
|  Function : OnMouseDown()                      	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnMouseDown(CtmWnd* pwndSender, WORD wIDControl)
{
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	if(pwnd == NULL) 		return wIDControl;
 		
//	if(pwnd == u_pwndButtonEnableOff)
//	{
//		SendCommand(0xF600);
//	}
//	if(pwnd == u_pwndButtonEnableOn)
//	{
//		SendCommand(0xF200);
//	}
		if(pwnd == u_pwndBtnServo) // Servo On/Off
	{
		if(wEnableStatus)
			{
				SendCommand(0xF600); // Servo Off
			}
		else
			{
				SendCommand(0xF200); // Servo On
			}
	}
	
	for(int i = 0; i < BUTTONMAXNUM; i++) // 各軸命令
	{
		if(pwnd == u_pwndButtonHand[i])
		{
			printf("Send u_nCommand[%d] = %x\n", i,u_nCommand[i]);
			
			if(g_ptaskpicker != NULL) // 手動模式下
			{
				if(u_nMANUAL_TYPE != MANUAL_STOP_MODE) // 安全按鈕有按下
					SendCommand(u_nCommand[i]);
				else // 請按安全開關	
					 MsgBoxCall("msgboxConfirm.txt","PICKER_SAFEEYNOTPRESS");				
			}
			
			
			return wIDControl;
		}
	}
  return wIDControl;	
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

	if(pwnd == u_pwndBtnManualType0) // 連續動
	{
		u_nMANUAL_TYPE = 0;
		SetDBValue("MACHINE_CONFIGURATION_MANUAL_TYPE",u_nMANUAL_TYPE);
		SetManualMode();
	}
	else if(pwnd == u_pwndBtnManualType1) // 點動
	{
		u_nMANUAL_TYPE = 1;
		SetDBValue("MACHINE_CONFIGURATION_MANUAL_TYPE",u_nMANUAL_TYPE);
		SetManualMode();
	}

	if(pwnd ==pwndButton_para)
	{
		::PutCommand("Hand_1.txt");
	}
	if(pwnd ==pwndButton_out)
	{
		::PutCommand("Hand_2.txt");
	}
	
//	if(pwnd == u_pwndButtonEnableOff)
//	{
//		SendCommand(0xFF60);
//	}
//	if(pwnd == u_pwndButtonEnableOn)
//	{
//		SendCommand(0xFF60);
//	}
	if(pwnd == u_pwndBtnServo)
	{
		SendCommand(0xFF60);
	}
	
	for(int i = 0; i < BUTTONMAXNUM; i++)
	{
		if(pwnd == u_pwndButtonHand[i])
		{
			SendCommand(COMMAND_STOP);
			return wIDControl;
 	  	}
  	}
  return wIDControl;	
}
/*---------------------------------------------------------------------------+
|  Function : OnMessage()                       	     	                     |
|  Task     :   						     	                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
	{
		case	MSG_GPIO_READ:
		{
			printf("MSG_GPIO_READ\n");

			// 背面安全按鈕
			{
			char gpio_safekey;
			gpio_safekey = wParam&GPIO_SAFEKEY_BIT;
			printf("gpio_safekey = %x\n",gpio_safekey);
			if(gpio_safekey==GPIO_SAFEKEY_0) // 放開 停止
				{
					u_nMANUAL_TYPE = MANUAL_STOP_MODE;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_1) // 1段 點動
				{
					u_nMANUAL_TYPE = MANUAL_JOG_MODE;
				}
			else if(gpio_safekey==GPIO_SAFEKEY_2) // 2段 連續動
				{
					u_nMANUAL_TYPE = MANUAL_CONTINUE_MODE;
				}
			}
		}
		break;
		default:
			break;	
	}	
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
			g_ptaskpicker->ReqValues(CONST_REQ_COMMAND, 1, &CommandID, NULL);
 	  	printf("Send Command = %x\n", CommandID);
		}
}

void		SetEditValue(CtmWnd* pwnd)
{
	char	pDataID[256];
	long long lTemp;
	//printf("Name = %s\n",pwnd->Name);
	if(pwnd != NULL)
	{
		if(pwnd->Is("CtmEditX1"))
		{
			pwnd->GetPropValueT("dbid1", pDataID,sizeof(pDataID));
      lTemp = (long)GetDBValue(pDataID).lValue;
  	}
    else
   	{
   		pwnd->GetPropValueT("dbid0", pDataID,sizeof(pDataID));
      lTemp = GetDBValue(pDataID).lValue/100;
   		//printf("StrID = %s, Value = %d\n", pDataID, lTemp);
   	}
		//printf("lTemp = %ld\n",lTemp);
		if(pDataID != NULL && pDataID[0] != '\0')
		{
			pwnd->SetPropValueT("value",lTemp);
			pwnd->UpdateAll();
		}
	}	
}
void       CreateLEDSet()
{
	BOOL 	bSelected = FALSE;
		
	u_dbInputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALINPUTSTATE");	
	u_wInputValue = static_cast<WORD>(u_dbInputValue.lValue);

	u_dbOutputValue = GetDBValue("MACHINE_INTERFACE_DWINTERNALOUTPUTSTATE");	
	u_wOutputValue = static_cast<WORD>(u_dbOutputValue.lValue);	

	for(int i = 0; i < u_nSelectBoxCount; i++)
	{
		if(u_pwndSelectBox[i] != NULL)
		{
			if(i < PB_MAXNUM)     // PB
			{
	   		if(_TestBit(u_wInputValue, i))	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
		 		u_pwndSelectBox[i]->Update();
			}
			else                 //PC
			{
				if(_TestBit(u_wOutputValue, i - PB_MAXNUM))	bSelected = TRUE;
		 		else 							bSelected = FALSE;
		 		u_pwndSelectBox[i]->SetPropValueT("selected", bSelected);
		 		u_pwndSelectBox[i]->Update();
			}
		}		
	}
}
void	CreateStringSet()
{
	char	pszString[128];
	int 	j;
	for( int i = 0; i < u_nSelectBoxCount; i++)
	{
		memset(u_acSelBoxString[i], 0, sizeof(u_acSelBoxString[i]));
		if(g_pFont->GetDefaultFont()->GetFontCode() == tmFT_CODE_TECH)
		{
		  	WORD *pwSrc;
      		char  szTemp[32];
      		WORD *pwDst;
      
      		pwDst   = (WORD *)(u_acSelBoxString[i]);
      		if(i < PB_MAXNUM)   //PB
     		{
     	 		sprintf(szTemp, "", i+1);
   	  		}
   	  		else                //PC
   	 		{
   	  			sprintf(szTemp, "", i+1-PB_MAXNUM);
   	  		}
      
     		int	j = 0;
      		while(szTemp[j] != 0)
	    	{
	     		*pwDst ++   = szTemp[j ++];
	    	}
	    	*pwDst++  = ' ';	    
	    	if(i < PB_MAXNUM)   //PB
	    	{
	   			sprintf(pszString, "PCIKER_INNER_I_%02d", i+1);
	    	}
	    	else                //PC
	    	{
	    		sprintf(pszString, "PICKER_INNER_O_%02d", i+1-PB_MAXNUM);
	    	}
	    
	    	pwSrc = (WORD *)g_MultiLanguage[pszString];
      		if(pwSrc != NULL)	
      		{
    	  		j = 0;
    	  		while(*pwSrc != 0)
    	     	*pwDst ++   = *pwSrc ++;
    		}
		  	*pwDst  = 0;
		}
	}
	
		for(int i = 0; i < u_nSelectBoxCount; i++)	
		{
	    	if(u_pwndSelectBox[i] != NULL)
	   	 	{
	    		u_pwndSelectBox[i]->SetPropValueT("text", u_acSelBoxString[i]);
	    	}
	  	}
}


void	SetManualMode()
{
	u_nMANUAL_TYPE = (int)GetDBValue("MACHINE_CONFIGURATION_MANUAL_TYPE").lValue;
	printf("MANUAL_TYPE=%d\n",u_nMANUAL_TYPE);
	switch(u_nMANUAL_TYPE)
	{
		case 0:
				u_pwndBtnManualType0->SetPropValueT("bgc",Btn_bgc[ON]);
				u_pwndBtnManualType1->SetPropValueT("bgc",Btn_bgc[OFF]);
				u_pwndBtnManualType0->SetPropValueT("fgc",Btn_fgc[ON]);
				u_pwndBtnManualType1->SetPropValueT("fgc",Btn_fgc[OFF]);				
			break;
		case 1:
    		u_pwndBtnManualType0->SetPropValueT("bgc",Btn_bgc[OFF]);	
				u_pwndBtnManualType1->SetPropValueT("bgc",Btn_bgc[ON]);
				u_pwndBtnManualType0->SetPropValueT("fgc",Btn_fgc[OFF]);
				u_pwndBtnManualType1->SetPropValueT("fgc",Btn_fgc[ON]);
			break;
		default:
			break;
	}			
	u_pwndBtnManualType0->CreateA();
	u_pwndBtnManualType0->Update();
	u_pwndBtnManualType1->CreateA();
	u_pwndBtnManualType1->Update();
	if(u_nMANUAL_TYPE == 0)
		((CtmButton*)u_pwndBtnManualType0)->Press(tmBUTTON_DOWN);
	else if(u_nMANUAL_TYPE == 1)
		((CtmButton*)u_pwndBtnManualType1)->Press(tmBUTTON_DOWN);
}