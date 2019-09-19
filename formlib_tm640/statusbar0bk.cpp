/*===========================================================================+
|  Class    : statusbar0 library                                             |
|  Task     : statusbar0 library action source file                          |
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
#include	"statusbar0.h"
#include	"../commonaction.h"
#include	"../main.h"
#include	"../tmconfig.h"
#include    "../utils.h"
#include 	"../tmshmsg.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define 		CNT_INT_DEMO_TIMER 		2000;//1000; 50
#define 		GPIO_LED_DEFINE  		0x07;//1000; 50
#define 		GPIO_BIT_AUTO 0x01
#define 		GPIO_BIT_ALARM 0x02
#define 		GPIO_BIT_ENABLE 0x03

const			int	CNT_INT_UPDATE_DATE_TIMER = 1000;	//Delay time.

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//<< autodemo
extern	int     g_nDemo;									//	標記Demo，0為在自動Demo狀態，1為在非Demo狀態    
static  int     u_nKeyIndex		= 0;
BOOL			u_bPCLinkFlag	= FALSE;
BOOL			u_bRemote		= FALSE;
BOOL			u_biNet			= FALSE;
int				work_coord_no = -1;
int				tool_coord_no = -1;
WORD        u_wKeys[] = {           //  PanelMap  //Sunny<20060918> add
//_F2_Offset_Work
    _F2,
    _F3,
    _F4,
    _F5,
    _F1,
    _F1,
//_F3_Offset_Tool
    _F3,
    _F3,
    _F4,
    _F5,
    _F1,
//_F4_System
    _F4, 
    _F3,
  	//_F4_F3_Para
    _F3,
    _F4,
    _F5,
    _F6,
    _F1,
    //_F4_F4_MLC
    _F4,
    _F3,
    _F4,
    _F5,
    _F6,
    _F7,
    _F8,
    _F9,
    _F10,
    _F1,
    _F5,
    _F6,
    _F7,
    _F1,
	//_F5_ProgManage
    _F5,
    _F3,
    _F4,
    _F1,
//_F6_Alarm
    _F6,
    _F3,
    _F4,
    _F5,
    _F1,
//_F7_Tune
    _F7,
    _F3,
    _F1,
//_F8
	_F8,
    _F3,
    _F1,
//_F9
	_F9,
//_F10
	_F10,
//Change Language
//	_F4,
//	_F7,
//	_KEY_RIGHTFIELD,
//	46,//_DOT
//	_KEY_DOWNFIELD,
//	46,//_DOT
//	_ENTER,

    _F1,
	_NULL_KEY    
};


char*	u_pszRobotStatus[] =
{
	"ROBOT_STR_NOT_READY",	
	"ROBOT_STR_READY",			//edit
	"ROBOT_BTN_AUTOMATIC",	//auto
	"ROBOT_BTN_AUTOMATIC",	//mdi
	"ROBOT_BTN_MANUAL",			//JOG
	"ROBOT_BTN_MANUAL",			//吋動
	"ROBOT_BTN_MANUAL",			//MPG
	"ROBOT_BTN_BACKTO_HOME",//backhome
};

char*	u_pszAutoStatusImagePath[] =
{
	"res_tm640/StatusBarAutoStop.bmp",	
	"res_tm640/StatusBarAutoRun.bmp",
	"res_tm640/StatusBarAutoPause.bmp",
	"NULL"
};

char*	u_pszRunStatus[] =
{
	"ROBOT_STR_STATUS_STOPING",
	"ROBOT_STR_STATUS_RUNING",
	"ROBOT_STR_STATUS_PAUSEING"
};

char*	u_pszServoImagePath[] =
{
	"res_tm640/servo_off.png",	//servo_off
	"res_tm640/servo_on.png"	  //servo_on
};

char*	u_pszJawsImagePath[] =
{
	"res_tm640/ON.bmp",	//ON
	"res_tm640/OFF.bmp"	//OFF
};
enum KEYMODE {AUTO,MANUAL,REMOTE};
//CtmWnd*			pwndRobotStatusImage 	= NULL;
CtmWnd*			pwndAutoStatusImage 	= NULL;
CtmWnd*			pwndRobotStatus 	= NULL;
CtmWnd*			pwndCoordTitle 		= NULL;
CtmWnd*			pwndAutoStatus		= NULL;
//CtmWnd*			pwndErrorImage 		= NULL;
CtmWnd*			pwndServoImage 		= NULL;
CtmWnd*			pwndJaws1Image 		= NULL;
CtmWnd*			pwndJaws2Static 		= NULL;
CtmWnd*			pwndJaws2Image 		= NULL;
CtmWnd*			pwndJaws2Mark			= NULL;
char 			m_szRemoteStr[1024];
long 			u_lDateTimer = CNT_INT_UPDATE_DATE_TIMER;
int 			RobotStatus = 0;
char 			old_filename[INT_FN_LEN] = "\0";
int 			KeyMode;
/*===========================================================================+
|           Function		                                                 |
+===========================================================================*/
BOOL	OnCreateA(CtmWnd* pwndSender)
{
    AddTimer(&u_lDateTimer);
    #ifdef      D_DEMO
    g_bKeyLockFlag = FALSE;
    #endif

	memset(m_szRemoteStr, 0, sizeof(m_szRemoteStr));
	u_bRemote = CtmConfig::GetInstance()->GetRemoteFlag();
	
	//strncpy(szPath, CtmConfig::GetInstance()->GetBmpPath(), 127);
	//pwndRobotStatusImage	= pwndSender->FindControlFromName("HMIModeImage");
	pwndAutoStatusImage	= pwndSender->FindControlFromName("HMIAutoSatausImage");
	pwndRobotStatus		= pwndSender->FindControlFromName("HMIModeStatic");
	pwndCoordTitle    = pwndSender->FindControlFromName("CoordianteStatic");
	pwndAutoStatus		= pwndSender->FindControlFromName("AutoSataus");
	//pwndErrorImage		= pwndSender->FindControlFromName("AlarmImage");
	pwndServoImage		= pwndSender->FindControlFromName("servoimage");
	//pwndJaws1Image		= pwndSender->FindControlFromName("Jaws1image");
	//pwndJaws2Static		= pwndSender->FindControlFromName("Jaws2Static");
	pwndJaws2Image		= pwndSender->FindControlFromName("Jaws2image");
	pwndJaws2Mark			= pwndSender->FindControlFromName("Jaws2Mark");
	
	//if(CtmConfig::GetInstance()->GetCncCommFlag() ==1)	SendMsg(MSG_CNC_PCTOOL_ONLINE, 0xffff, (long)0xffffffff, NULL);
	
	Init_Common_User();
	if(g_statusbar == NULL)
		g_statusbar = pwndSender;
	return TRUE;
}


void	OnMessage(CtmWnd* pwndSender, int message, WPARAM wParam, LPARAM lParam)
{
	int 	nErrorStrIndex =-1;
	char	szErrorText[40];
 	szErrorText[0]='\0';
    long Color =63488;
    BOOL	ShowErrorNo=FALSE;
		int servo_temp = 0;
  switch(message)
	{
		case MSG_USER_SH_ERROR_ADD:

			//進警報下始能
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_servo_on,&servo_temp,sizeof(g_TxCNCData.shmCNCData.CNC_servo_on));
			
			SetGpioLed(TRUE,GPIO_BIT_ALARM);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> SetPropValueT("fgc",Color);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> Update();
			switch(g_pRxCNCData->CNC_alarm_group)	//OPER,INTER,MOTION,MLC
			{
				case ID_OPER://CNC_ALARM_OP://1
					sprintf(szErrorText, "ROBOT_ALARM_OPER%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_INTER://CNC_ALARM_INTER://2
					sprintf(szErrorText, "ROBOT_ALARM_INTER%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_MOTION://CNC_ALARM_MOTION://3
					sprintf(szErrorText, "ROBOT_ALARM_MOT%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_MLC://CNC_ALARM_MLC://4
					sprintf(szErrorText, "_ALARM_MLC_%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				case ID_DRIVERX://CNC_ALARM_DRIVERX://5
					sprintf(szErrorText, "ROBOT_ALARM_AXIS1_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERY://CNC_ALARM_DRIVERY://6
					sprintf(szErrorText, "ROBOT_ALARM_AXIS2_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERZ://CNC_ALARM_DRIVERZ://7
					sprintf(szErrorText, "ROBOT_ALARM_AXIS3_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERA://CNC_ALARM_DRIVERA://8
					sprintf(szErrorText, "ROBOT_ALARM_AXIS4_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERB://CNC_ALARM_DRIVERB://9
					sprintf(szErrorText, "ROBOT_ALARM_AXIS5_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DRIVERC://CNC_ALARM_DRIVERC://10
					sprintf(szErrorText, "ROBOT_ALARM_AXIS6_001");
					ShowErrorNo = TRUE;
					break;
				case ID_DSP://CNC_ALARM_DSP://11
					sprintf(szErrorText, "ROBOT_ALARM_DSP%03d", g_pRxCNCData->CNC_alarm_id);
					break;
				default:
					sprintf(szErrorText, "_VW_DRIVER_MANUAL");
					break;	
			}
			nErrorStrIndex = g_MultiLanguage.GetStrKeyIndex(szErrorText);
			if(ShowErrorNo)
			{
				char temp[10],szTemp[20],ErrorStr[256];
				memset(temp, 0, sizeof(temp));
				memset(szTemp, 0, sizeof(szTemp));
				memset(ErrorStr, 0, sizeof(ErrorStr));
				sprintf(temp,"%d",g_pRxCNCData->CNC_alarm_id);
				CodeChange(szTemp,temp);
				Double_byte_String_Merger(ErrorStr,g_MultiLanguage[nErrorStrIndex],szTemp);
				Prompt(ErrorStr,1);
			}
			else
				Prompt(g_MultiLanguage[nErrorStrIndex],1);
			break;
		case MSG_USER_SH_ERROR_FIX:
			SetGpioLed(FALSE,GPIO_BIT_ALARM);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> SetPropValueT("fgc",(double)0x00);
			g_pMainFrame->GetPromptBar()->GetControlA(1) -> Update();
			Prompt(g_MultiLanguage[g_MultiLanguage.GetStrKeyIndex("MSG_DUMMY")],1);
			break; 
		case  MSG_SH_REMOTE_REQ_CONTROL:
			sprintf(m_szRemoteStr, "Connection Request!!\nName:%s\nPassWord:%s", 
									(char*)wParam, (char*)lParam);
			 
			MsgBox(g_MultiLanguage["VW_REMOTE_VIEW_RECV"], tmFT_CODE_TECH);
			SendMsg(MSG_SH_REMOTE_REV_CONTROL, g_bMsgBoxFlag, 0, NULL);
			break;
		//2012/6/20 上午 11:10:18 add for remote>>>>>		
		case	MSG_GPIO_READ:
		/*
			enum KEYMODE {AUTO,MANUAL,REMOTE};
			int 			KeyMode;
		*/
		break;
		default:
			break;	
	}	
}


void	OnUpdateA(CtmWnd* pwndSender)
{
	//printf("status OnUpdateA\n");
	WORD	wActionSatus = g_pRxCNCData->CNC_status;//Sunny<20101125>
	static char old_NowRunStatus = 9;
	static char Servo_status = 0;
	static char Auto_status = -1;
	static char Jaws1_status = -1;
	static char Jaws2_status = -1;
	char    sz[256];
	tmDATE        date;
	tmTIME        time;	
	
	/*if(Jaws1_status!=g_pRxCNCData->s_mlc_s_bit[101])
	{
		Jaws1_status=g_pRxCNCData->s_mlc_s_bit[101];
		pwndJaws1Image->SetPropValueT("imagepath",u_pszJawsImagePath[Jaws1_status]);
		pwndJaws1Image->Update();
		
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE)
		{
			int tmp = g_TxCNCData.shmCNCData.CNC_jaws_control;
			if(tmp%2>=1 && g_pRxCNCData->s_mlc_s_bit[101]==0)
	      tmp-=1;
			else if(tmp%2<1 && g_pRxCNCData->s_mlc_s_bit[101]==1)
	      tmp+=1;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		}
	}*/
	/*
	static int Last_Jaws_Count = -1;
	if(Last_Jaws_Count<2 && g_TxCNCData.shmCNCData.PARAM.maker_param[33]>=2)
	{
		Last_Jaws_Count = g_TxCNCData.shmCNCData.PARAM.maker_param[33];
		pwndJaws2Image->Update();
		pwndJaws2Static->Update();
	}
	else if (Last_Jaws_Count>=2 && g_TxCNCData.shmCNCData.PARAM.maker_param[33]<2)
	{
		Last_Jaws_Count = g_TxCNCData.shmCNCData.PARAM.maker_param[33];
		pwndJaws2Mark->Update();
	}
	
	if(Jaws2_status!=g_pRxCNCData->s_mlc_s_bit[102])
	{
		Jaws2_status=g_pRxCNCData->s_mlc_s_bit[102];
		pwndJaws2Image->SetPropValueT("imagepath",u_pszJawsImagePath[Jaws2_status]);
		if(g_TxCNCData.shmCNCData.PARAM.maker_param[33]>=2)
			pwndJaws2Image->Update();
		
		if(g_TxCNCData.shmCNCData.CNC_mode_lgt == OP_MEMORY_MODE)
		{
			int tmp = g_TxCNCData.shmCNCData.CNC_jaws_control;
			int tmp1 = g_TxCNCData.shmCNCData.CNC_jaws_control%4;
			if(tmp%4>=2 && g_pRxCNCData->s_mlc_s_bit[102]==0)
	      tmp-=2;
			else if(tmp%4<2 && g_pRxCNCData->s_mlc_s_bit[102]==1)
	      tmp+=2;
	      
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jaws_control,&tmp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		}
	}
	*/
	
	if(g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP)
		SetGpioLed(TRUE,GPIO_BIT_AUTO);
	else
		SetGpioLed(FALSE,GPIO_BIT_AUTO);

		
		
	if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_STOP)
	{
		strcpy(old_filename,g_TxCNCData.shmCNCData.CNC_mem_part);
 		g_pMainFrame->SetTitle2(g_TxCNCData.shmCNCData.CNC_mem_part);
	}
	else if(g_pRxCNCData->CNC_curr_blk.modal.fn[0] != 0  && (strcmp(old_filename,g_pRxCNCData->CNC_curr_blk.modal.fn) != 0) && (g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_RUN || g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_PAUSE) )
	{
		strcpy(old_filename,g_pRxCNCData->CNC_curr_blk.modal.fn);
 		g_pMainFrame->SetTitle2(g_pRxCNCData->CNC_curr_blk.modal.fn);
	}
	
	if(Servo_status != g_pRxCNCData->CNC_servo_status)
	{
		Servo_status = g_pRxCNCData->CNC_servo_status;
		if(Servo_status==1)
			SetGpioLed(TRUE,GPIO_BIT_ENABLE);
		else
			SetGpioLed(FALSE,GPIO_BIT_ENABLE);
		/*pwndServoImage->SetPropValueT("imagepath",u_pszServoImagePath[(int)Servo_status]);
		pwndServoImage->Update();*/
	}
	
	if(old_NowRunStatus != g_TxCNCData.shmCNCData.CNC_auto_status)
	{
		old_NowRunStatus = g_TxCNCData.shmCNCData.CNC_auto_status;
		//pwndAutoStatus->SetPropValueT("textID",u_pszRunStatus[(int)g_TxCNCData.shmCNCData.CNC_auto_status]);
		//pwndAutoStatus->Update();
	}
	 
	if(Auto_status!=g_TxCNCData.shmCNCData.CNC_auto_status && pwndAutoStatusImage!=NULL)
	{
		Auto_status=g_TxCNCData.shmCNCData.CNC_auto_status;
		//pwndAutoStatusImage->SetPropValueT("imagepath",u_pszAutoStatusImagePath[Auto_status]);
		//pwndAutoStatusImage->Update();
	}
	//------------HMI模式狀態---------------//
	
	/*if(KeyMode!=AUTO && (g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP))
	{
		char temp = RUN_STATUS_STOP;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_auto_status,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_auto_status));
	}
	else if(KeyMode != MANUAL && KeyMode != REMOTE)
	{
		char temp_CHAT = 0;
		int temp_INT = 0;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_coor,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_coor));
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_axis,&temp_INT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_axis));
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_jog_status,&temp_CHAT,sizeof(g_TxCNCData.shmCNCData.CNC_jog_status));
	}*/
	//if(g_pRxCNCData->CNC_move_end==1)
	//{
		//if(KeyMode==AUTO)
	if((g_TxCNCData.shmCNCData.CNC_auto_status != RUN_STATUS_STOP) && g_TxCNCData.shmCNCData.CNC_mode_lgt != OP_MEMORY_MODE)  	//執行時強制auto狀態
		{
			int mode_lgt  = OP_MEMORY_MODE;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_mode_lgt,&mode_lgt,sizeof(g_TxCNCData.shmCNCData.CNC_mode_lgt));
		}
		else if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_STOP && g_pRxCNCData->CNC_status==M_READY && g_TxCNCData.shmCNCData.CNC_mode_lgt != RobotStatus)								//停止時顯示目前狀態
		{
			int mode_lgt = RobotStatus;
			//if(mode_lgt==OP_MEMORY_MODE) mode_lgt=OP_EDIT_MODE;
			g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_mode_lgt,&mode_lgt,sizeof(g_TxCNCData.shmCNCData.CNC_mode_lgt));
		}
		//	printf("g_TxCNCData.shmCNCData.CNC_mode_lgt = %d\n",g_TxCNCData.shmCNCData.CNC_mode_lgt);
	//}
		
	//----------現在時間---------------//
	if (u_lDateTimer <= 0)
	{	
			GetTime(&time);  		
   		GetDate(&date);

   		sprintf(sz, "%04u.%02u.%02u/%02u:%02u", date.year, date.month, date.day, time.hour, time.minute);
      g_pMainFrame->GetPromptBar()->GetControlA(0)->Clear();
      g_pMainFrame->GetPromptBar()->GetControlA(0)->ShowExtent(0, 1, sz, 0);
		
		u_lDateTimer= CNT_INT_UPDATE_DATE_TIMER;
	}
	
	//-------------工作/工具座標顯示---------------//
	
	if(g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_RUN || g_TxCNCData.shmCNCData.CNC_auto_status == RUN_STATUS_PAUSE)
	{
		if(work_coord_no != g_pRxCNCData->CNC_curr_blk.trans.cw_coord  || tool_coord_no != g_pRxCNCData->CNC_curr_blk.trans.ct_coord)
		{
			char	temp[15] 		= "\0";
			char	szTemp[15]		= "\0";
			memset(temp, 0, sizeof(temp));
			memset(szTemp, 0, sizeof(szTemp));
			work_coord_no = g_pRxCNCData->CNC_curr_blk.trans.cw_coord;
			tool_coord_no = g_pRxCNCData->CNC_curr_blk.trans.ct_coord;
			SetDBValue("CNC_WORK_COORD_NO", work_coord_no);	
			SetDBValue("CNC_TOOL_COORD_NO", tool_coord_no);
			if(work_coord_no == 0 && tool_coord_no == 0)
				sprintf(temp, "W:No T:No");
			else if(work_coord_no == 0)
				sprintf(temp, "W:No T:%d",tool_coord_no);
			else if(tool_coord_no == 0)
				sprintf(temp, "W:%d T:No", work_coord_no);
			else
				sprintf(temp, "W:%d T:%d", work_coord_no,tool_coord_no);
			
			CodeChange(szTemp, temp);
			pwndCoordTitle->SetPropValueT("text", szTemp);
			pwndCoordTitle->Update();
		}
	}
	else
	{
		//printf("status g_TxCNCData.shmCNCData.CNC_work_coord_no\n");
		if(g_TxCNCData.shmCNCData.CNC_work_coord_no==0)
			SetDBValue("CNC_WORK_COORD_NO", 1);	
		if(g_TxCNCData.shmCNCData.CNC_tool_coord_no==0)
			SetDBValue("CNC_TOOL_COORD_NO", 1);
		//printf("work_coord_no = %d , g_TxCNCData.shmCNCData.CNC_work_coord_no = %d\n",work_coord_no,g_TxCNCData.shmCNCData.CNC_work_coord_no);
		if(work_coord_no != g_TxCNCData.shmCNCData.CNC_work_coord_no  || tool_coord_no != g_TxCNCData.shmCNCData.CNC_tool_coord_no)
		{
			char	temp[15] 		= "\0";
			char	szTemp[15]		= "\0";
			memset(temp, 0, sizeof(temp));
			memset(szTemp, 0, sizeof(szTemp));
			work_coord_no = g_TxCNCData.shmCNCData.CNC_work_coord_no;
			tool_coord_no = g_TxCNCData.shmCNCData.CNC_tool_coord_no;
			if(work_coord_no == 0 && tool_coord_no == 0)
				sprintf(temp, "W:No T:No");
			else if(work_coord_no == 0)
				sprintf(temp, "W:No T:%d",tool_coord_no);
			else if(tool_coord_no == 0)
				sprintf(temp, "W:%d T:No", work_coord_no);
			else
				sprintf(temp, "W:%d T:%d", work_coord_no,tool_coord_no);
			
			CodeChange(szTemp, temp);
			pwndCoordTitle->SetPropValueT("text", szTemp);
			pwndCoordTitle->Update();
		}
	}
	
	if(g_TxCNCData.shmCNCData.CNC_update_wt_coord != 0 && g_pRxCNCData->CNC_update_wt_coord_OK != 0)
	{
		char temp=0;
		g_ptaskCNC->WriteValue((long)&g_TxCNCData.shmCNCData.CNC_update_wt_coord,&temp,sizeof(g_TxCNCData.shmCNCData.CNC_jog_mode));
	}
	
	if(Ethernet_Break == 1 || Ethernet_Break == 2)
	{
		MsgBoxCall("msgboxEthernetBreak.txt");
	}
	
}

void	OnDestroyA(CtmWnd* pwndSender)
{
	//DeleteTimer(&u_lDemoTime);			//	銷毀計時器
	//DeleteTimer(&u_lDateTimer);
	
}


void	Init_Common_User()
{
	umount("/mnt/usb");
}

void	Double_byte_String_Merger(char* out, char* str1 , char* str2) 
{
	if(str1 == NULL || str2 == NULL)  
	{
		//printf("NULL\n");
		return;
	}
	
	WORD *pszDst,*pszSrc;
	pszDst = (WORD*)out;
	
  pszSrc=(WORD*)str1;
	while(*pszSrc != 0) 
  {
  	*pszDst ++ = *pszSrc ++;
  }
  
  pszSrc=(WORD*)str2;
  while(*pszSrc != 0) 
  {
  	*pszDst ++ = *pszSrc ++;
  }
  out = (char*)pszDst;
}


void	SetGpioLed(BOOL status,int bit)
{
	static char ledstatus=0x07;	//gpio 
	if (bit < 1) return;
	char bit_0x = 0x01 << (bit-1);
	//printf("bit_0x = 0x%02x\n",bit_0x);
	if(status)
	{
		bit_0x = ~bit_0x;
		ledstatus &= bit_0x;
		_Gpio_OutputWrite(&ledstatus);
		//printf("true bit_0x = 0x%02x ledstatus = 0x%02x\n",bit_0x,ledstatus);
	}
	else
	{
		ledstatus |= bit_0x;
		_Gpio_OutputWrite(&ledstatus);
		//printf("false bit_0x = 0x%02x ledstatus = 0x%02x\n",bit_0x,ledstatus);
	}
}