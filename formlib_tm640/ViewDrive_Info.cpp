/*===========================================================================+
|  Class    : ViewCore library                                          	 |
|  Task     : ViewCore library action source file                       	 |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#include	"ViewDrive_Info.h"
//#include	"../control.h"
#include	"../tmdebug.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		REQ_READMOTOR    				2   
#define		MechType3				0	 // 三軸
#define		MechType5				1  // 五軸
#define		EncType_Abs			0x00000000  // 絕對
#define		EncType_Res		  0x00010000  // 增量
#define		EncWord 				0xFFFF0000 // High Word
#define		MechWord 				0x0000FFFF // Low Word
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
BOOL				RunOnlyOne				=	FALSE;	//利用update僅執行一次

int u_Axis_Num = 1;
CtmWnd*		pwndDriveInfoSTR[11] 	={NULL}; 	
int iDriveInfoSTR = 0;

CtmWnd*		pwndIMG_Axis  		= NULL;

CtmWnd*		pwndBtn_Axis[5] 	={NULL}; 
int iBtn_Axis = 0;


DWORD dw_InfoValue[11] = {0};

char* u_pszStrID_X1[] = 
{
	"MACHINE_INTERFACE_X1_DRIVE_ID",        // X1_驅動器ID
	"MACHINE_INTERFACE_X1_LAST_ALARMID",	  // X1_最近一次警報代碼
	"MACHINE_INTERFACE_X1_CURRENTLOOP_KP",  // X1_電流環KP
	"MACHINE_INTERFACE_X1_CURRENTLOOP_KI",  // X1_電流環KI
	"MACHINE_INTERFACE_X1_SPEEDLOOP_KP",    // X1_速度環KP
	"MACHINE_INTERFACE_X1_SPEEDLOOP_KI",    // X1_速度環KI
	"MACHINE_INTERFACE_X1_BUS_V",           // X1_母線電壓
	"MACHINE_INTERFACE_X1_DRIVE_TMP",       // X1_驅動器溫度
	"MACHINE_INTERFACE_X1_MOTOR_TMP",       // X1_馬達溫度
	"MACHINE_INTERFACE_X1_DRIVE_I",         // X1_驅動器電流
	"MACHINE_INTERFACE_X1_MOTOR_RPM",       // X1_馬達轉速
};
char* u_pszStrID_Y1[] = 
{
	"MACHINE_INTERFACE_Y1_DRIVE_ID",        // Y1_驅動器ID
	"MACHINE_INTERFACE_Y1_LAST_ALARMID",	  // Y1_最近一次警報代碼
	"MACHINE_INTERFACE_Y1_CURRENTLOOP_KP",  // Y1_電流環KP
	"MACHINE_INTERFACE_Y1_CURRENTLOOP_KI",  // Y1_電流環KI
	"MACHINE_INTERFACE_Y1_SPEEDLOOP_KP",    // Y1_速度環KP
	"MACHINE_INTERFACE_Y1_SPEEDLOOP_KI",    // Y1_速度環KI
	"MACHINE_INTERFACE_Y1_BUS_V",           // Y1_母線電壓
	"MACHINE_INTERFACE_Y1_DRIVE_TMP",       // Y1_驅動器溫度
	"MACHINE_INTERFACE_Y1_MOTOR_TMP",       // Y1_馬達溫度
	"MACHINE_INTERFACE_Y1_DRIVE_I",         // Y1_驅動器電流
	"MACHINE_INTERFACE_Y1_MOTOR_RPM",       // Y1_馬達轉速
};
char* u_pszStrID_Z[] = 
{
	"MACHINE_INTERFACE_Z_DRIVE_ID",        // Z_驅動器ID
	"MACHINE_INTERFACE_Z_LAST_ALARMID",	 	 // Z_最近一次警報代碼
	"MACHINE_INTERFACE_Z_CURRENTLOOP_KP",  // Z_電流環KP
	"MACHINE_INTERFACE_Z_CURRENTLOOP_KI",  // Z_電流環KI
	"MACHINE_INTERFACE_Z_SPEEDLOOP_KP",    // Z_速度環KP
	"MACHINE_INTERFACE_Z_SPEEDLOOP_KI",    // Z_速度環KI
	"MACHINE_INTERFACE_Z_BUS_V",           // Z_母線電壓
	"MACHINE_INTERFACE_Z_DRIVE_TMP",       // Z_驅動器溫度
	"MACHINE_INTERFACE_Z_MOTOR_TMP",       // Z_馬達溫度
	"MACHINE_INTERFACE_Z_DRIVE_I",         // Z_驅動器電流
	"MACHINE_INTERFACE_Z_MOTOR_RPM",       // Z_馬達轉速
};
char* u_pszStrID_X2[] = 
{
	"MACHINE_INTERFACE_X2_DRIVE_ID",        // X2_驅動器ID
	"MACHINE_INTERFACE_X2_LAST_ALARMID",	  // X2_最近一次警報代碼
	"MACHINE_INTERFACE_X2_CURRENTLOOP_KP",  // X2_電流環KP
	"MACHINE_INTERFACE_X2_CURRENTLOOP_KI",  // X2_電流環KI
	"MACHINE_INTERFACE_X2_SPEEDLOOP_KP",    // X2_速度環KP
	"MACHINE_INTERFACE_X2_SPEEDLOOP_KI",    // X2_速度環KI
	"MACHINE_INTERFACE_X2_BUS_V",           // X2_母線電壓
	"MACHINE_INTERFACE_X2_DRIVE_TMP",       // X2_驅動器溫度
	"MACHINE_INTERFACE_X2_MOTOR_TMP",       // X2_馬達溫度
	"MACHINE_INTERFACE_X2_DRIVE_I",         // X2_驅動器電流
	"MACHINE_INTERFACE_X2_MOTOR_RPM",       // X2_馬達轉速
};
char* u_pszStrID_Y2[] = 
{
	"MACHINE_INTERFACE_Y2_DRIVE_ID",        // Y2_驅動器ID
	"MACHINE_INTERFACE_Y2_LAST_ALARMID",	  // Y2_最近一次警報代碼
	"MACHINE_INTERFACE_Y2_CURRENTLOOP_KP",  // Y2_電流環KP
	"MACHINE_INTERFACE_Y2_CURRENTLOOP_KI",  // Y2_電流環KI
	"MACHINE_INTERFACE_Y2_SPEEDLOOP_KP",    // Y2_速度環KP
	"MACHINE_INTERFACE_Y2_SPEEDLOOP_KI",    // Y2_速度環KI
	"MACHINE_INTERFACE_Y2_BUS_V",           // Y2_母線電壓
	"MACHINE_INTERFACE_Y2_DRIVE_TMP",       // Y2_驅動器溫度
	"MACHINE_INTERFACE_Y2_MOTOR_TMP",       // Y2_馬達溫度
	"MACHINE_INTERFACE_Y2_DRIVE_I",         // Y2_驅動器電流
	"MACHINE_INTERFACE_Y2_MOTOR_RPM",       // Y2_馬達轉速
};

/*--------------------------------------------------+
| dw_MechType 機型選擇  														|
|	Low WORD 0-三軸 1-五軸 	High WORD 0-絕對 1-增量		|
| EX: 絕對/五軸 0x0000 0001;  0x0001 0000 增量/三軸 |
+--------------------------------------------------*/
DWORD dw_MechType = 0; //紀錄 pMechTypeDB 的數值
int u_PickerType 	= 0; // 機型選擇 0-三軸 1-五軸
int u_EncType 		= 0; // 編碼器選擇  0-絕對 1-增量
char* pMechTypeDB	 = "MACHINE_CONFIGURATION_MACHINETYPE"; // 機型選擇DB 三軸 五軸
char* u_pszImgDriveInfo[] = 
{
	"res_tm640/pic/DriveInfo_Axis3_Axis",   // 
	"res_tm640/pic/DriveInfo_Axis5_Axis",	  // 
};
/*---------------------------------------------------------------------------+
|           View Content - statusbar                                         |
+---------------------------------------------------------------------------*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
	printf("Drive_Info\n");
	SetDBValue("MACHINE_CONFIGURATION_IO_REFERENCE_POINT", 0x01); // 送28命令 開始跟驅動器要資料
	
	iDriveInfoSTR	 	= GetSpecialControlNum(pwndSender, "STR", "CtmStaticX2", pwndDriveInfoSTR);
	
	pwndIMG_Axis    = pwndSender->FindControlFromName("IMG_Axis");
	
	iBtn_Axis	 	= GetSpecialControlNum(pwndSender, "Btn_Axis_", "CtmToolButton", pwndBtn_Axis);
	printf("iBtn_Axis=%d\n",iBtn_Axis);
	
	// 讀取設定 機型選擇 三五軸
	dw_MechType 	 = (GetDBValue(pMechTypeDB).lValue);
	u_PickerType = dw_MechType & MechWord;
	u_EncType		 = dw_MechType & EncWord;
	
	// 各軸示意圖
	char szImgPath[256];
	sprintf(szImgPath, "%s%d.bmp",u_pszImgDriveInfo[u_PickerType],u_Axis_Num);
	pwndIMG_Axis->SetPropValueT("imagepath",szImgPath);
	pwndIMG_Axis->Show();
	
	return TRUE;
}

WORD	OnChangeA(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwnd = NULL;	
	pwnd = pwndSender->FindControlFromTab(wIDControl);
	if(pwnd == NULL)	return wIDControl;
	
	return wIDControl;
}

void	OnUpdateA(CtmWnd* pwndSender)
{
//	struct timeval tv;
//	static struct timeval tv_old;
//	static int msg_last;
//	long lagtime=0;
//	
//	gettimeofday(&tv,NULL);
//  lagtime = (tv.tv_usec - tv_old.tv_usec)/1000;
//  printf("Update time = %d\n",lagtime);
//  
//	gettimeofday(&tv_old,NULL);
	
	if(!RunOnlyOne)
	{
		RunOnlyOne=TRUE;
		
		int		nTime 	= 0;
		
	  if(g_ptaskpicker != NULL)
		{
			WORD		wNum = 0;
			switch(u_Axis_Num)
			{
				case 1: // X1
					wNum = sizeof(u_pszStrID_X1)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X1, &nTime); // Update Data from 28
					break;
				case 2: // Y1
					wNum = sizeof(u_pszStrID_Y1)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y1, &nTime); // Update Data from 28
					break;
				case 3: // Z
					wNum = sizeof(u_pszStrID_Z)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Z, &nTime); // Update Data from 28
					break;
				case 4: // X2
					wNum = sizeof(u_pszStrID_X2)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X2, &nTime); // Update Data from 28
					break;
				case 5: // Y2
					wNum = sizeof(u_pszStrID_Y2)/sizeof(char*);
					nTime = 50;	//	the unit is ms, 
					g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y2, &nTime); // Update Data from 28
					break;
				default:
					break;
			}		
		}	
	}
	


	for(int i=0;i<iDriveInfoSTR;i++) // Update String value
	{
		DWORD dw;
		float f_value;
		switch(u_Axis_Num) // Get Data from DB
		{
			case 1: // X1
				dw = GetDBValue(u_pszStrID_X1[i]).lValue;
				break;
			case 2: // Y1
				dw = GetDBValue(u_pszStrID_Y1[i]).lValue;
				break;
			case 3: // Z
				dw = GetDBValue(u_pszStrID_Z[i]).lValue;
				break;
			case 4: // X2
				dw = GetDBValue(u_pszStrID_X2[i]).lValue;
				break;
			case 5: // Y2
				dw = GetDBValue(u_pszStrID_Y2[i]).lValue;
				break;
			default:
				break;
		}	
		
		char  str_temp[256] = "\0";
		if(i>=2)
		{
			if(u_EncType==EncType_Abs) // 絕對型 ENC for AUSTONE
			{
				if(i!=10)
				{
					f_value = Conv2Float(dw);
					if(f_value >= 65535)
						sprintf(str_temp ,"NULL");
					else
						sprintf(str_temp ,"%.02f",f_value);
				}
				else
					sprintf(str_temp ,"%d",dw);	
			}
			else if(u_EncType==EncType_Res) // 增量式 for HDT
			{
				if(i==6) // 母線電壓
					dw = dw/1000;
				sprintf(str_temp ,"%d",dw);						
			}
		}
		else
		{
			sprintf(str_temp ,"%d",dw);
			if(u_EncType==EncType_Res && (i==1)) // 增量式 for HDT // 最近一次警報
				sprintf(str_temp ,"%x",dw);		
		}
		
		if(pwndDriveInfoSTR[i]!=NULL) // Update String
		{
			pwndDriveInfoSTR[i]->SetPropValueT("text",str_temp);
			pwndDriveInfoSTR[i]->Update();
		}
	}
}

WORD	OnKeyA(CtmWnd* pwndSender, WORD wKey)
{
	//printf("OnKey\n");
	
	for(int i=0;i<iBtn_Axis;i++) // 軸按鍵變灰
	{
		pwndBtn_Axis[i]->SetPropValueT("upbitmap","res_tm640/pic/ButtonUp1.bmp");
		pwndBtn_Axis[i]->CreateA();
		pwndBtn_Axis[i]->Update();
	}

	
	switch(wKey)
	{
		case 1:
    	u_Axis_Num=1;
			break;	
		case 2:
    	u_Axis_Num=2;
			break;	
		case 3:
			u_Axis_Num=3;
			break;
		case 4:
			u_Axis_Num=4;
			break;
		case 5:
			u_Axis_Num=5;
			break;
		default:
			break;
	}
	
	if(pwndBtn_Axis[u_Axis_Num-1]) // 選取的軸按鍵
	{	
		pwndBtn_Axis[u_Axis_Num-1]->SetPropValueT("upbitmap","res_tm640/pic/ButtonDown1.bmp");
		pwndBtn_Axis[u_Axis_Num-1]->CreateA();
		pwndBtn_Axis[u_Axis_Num-1]->Update();
	}
	
	char szImagePath[256]; // 各軸示意圖
	sprintf(szImagePath, "%s%d.bmp",u_pszImgDriveInfo[u_PickerType],u_Axis_Num);
	pwndIMG_Axis->SetPropValueT("imagepath",szImagePath);
	pwndIMG_Axis->Show();

	 if(g_ptaskpicker != NULL)
	{
		WORD		wNum = 0;
		int		nTime 	= 0;
		switch(u_Axis_Num)
		{
			case 1: // X1
				wNum = sizeof(u_pszStrID_X1)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X1, &nTime); // Update Data from 28
				break;
			case 2: // Y1
				wNum = sizeof(u_pszStrID_Y1)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y1, &nTime); // Update Data from 28
				break;
			case 3: // Z
				wNum = sizeof(u_pszStrID_Z)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Z, &nTime); // Update Data from 28
				break;
			case 4: // X2
				wNum = sizeof(u_pszStrID_X2)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_X2, &nTime); // Update Data from 28
				break;
			case 5: // Y2
				wNum = sizeof(u_pszStrID_Y2)/sizeof(char*);
				nTime = 50;	//	the unit is ms, 
				g_ptaskpicker->WriteValue(REQ_READMOTOR, wNum ,u_pszStrID_Y2, &nTime); // Update Data from 28
				break;
			default:
				break;
		}		
	}
	
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
 	CtmWnd*     pwnd = pwndSender->FindControlFromTab(wIDControl);
 	
//	DWORD dw = GetDBValue("MACHINE_INTERFACE_X1_CURRENTLOOP_KP").lValue;
//	printf("dw=%d\n",dw);
//	float f_CL_KP = Conv2Float(dw);
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	SetDBValue("MACHINE_CONFIGURATION_IO_REFERENCE_POINT", 0);// 離開送0 主機判斷
}

float	Conv2Float(DWORD dw)
{
	float fd = 0;
	::memcpy((void*)&fd, (void*)&dw, 4);
	//printf("dw=%d\n",dw);
	//printf("fd=%f\n",fd);
	return fd;
}
