/*===========================================================================+
|  Function : tminit	                                                     |
|  Task     : tminit Utilities Cpp File	                                     |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC) V3.2                                                  |
|  Link     : G++(GCC) V3.2                                                  |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Sunny		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 06/05/2008                                                     |
|  Revision : 06/05/2008                                                     |
+===========================================================================*/

#include	"tmInit.h"
#include	"../../tmdebug.h"
#include	"../../tmcontrol.h"
#include	"../../font.h"	//	fans 2008/6/18 06:16下午
#include	"../../selvat_ancestor.h"
#include	"../../package.h"
#include	"../../default.h"
#include	"../../commonaction.h"
//	fans add 2008/8/29 01:38下午 for touchscreen
#include	"../../touchscreen.h"
#include	"../../cursor.h"
#include	"../../freedata.h"
#include	"../../global.h"	//	fans add 2008-11-13 11:15:05
#include	"../../iotable.h"
#include	"../../taskdataprocess.h"
#include    "../../taskammeter.h"
#include    "../picker_comm/tmTaskPickerComm.h"
#include	"../../exdatabase.h"
#include	"../../parsexml.h"
#include	"../../canMaster.h"
#include	"../../sram.h"
#include	"../../dbid2.h"
#include	"../../taskmodbus.h"

/*joyce20130817 bug:國光液晶問題:在屏保黑屏后再喚醒的時候,會出現橫條白色閃屏,然后慢慢的恢複到正常的畫面
  軟體進行改善:點亮LCD前加大延遲時間,由標準10ms改為200ms*/
extern	int			g_nRTCFd;
#ifdef	D_PXA270
#define     LCD_CMD_BACKONOFF       0x4015
#define     LCD_CMD_SIGNONOFF       0x4012
#define     LCD_CMD_DISPONOFF       0x4013
#define     LCD_CMD_POWRONOFF       0x4014
#else
#define     LCD_CMD_BACKONOFF       1
#define     LCD_CMD_SIGNONOFF       2
#define     LCD_CMD_DISPONOFF       3
#define     LCD_CMD_POWRONOFF       4
#endif
void	SPC_LCDON()
{
  ioctl(g_nRTCFd, LCD_CMD_POWRONOFF, 1);
  usleep(200*1000);
  ioctl(g_nRTCFd, LCD_CMD_SIGNONOFF, 1);
  usleep(200*1000);
  ioctl(g_nRTCFd, LCD_CMD_BACKONOFF, 1);
}

IMPLEMENT_DYNCREATE(CtmInit, CInit)
CtmInit*	CtmInit::m_pSelf = NULL;


/*==========================================================================+
|						Define												|
+==========================================================================*/
	
	//nDataID   wDataType  wPrecision  nOffset data
TMDB2	g_TMDB2[] = 
{
	{9999,				    TYPE_INT, 	0, 	-1},
	{9998,				    TYPE_INT, 	0, 	-1},
	{DBID2_DATA_1001,	TYPE_WORD,   0,	-1},   //記錄Msgbox跳出時是否key下確認鍵 0x1234
	{DBID2_DATA_1002,	TYPE_WORD, 	0,	-1},   //重置完后第1次進入程式
	{DBID2_DATA_1003,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1004,	TYPE_WORD, 	0,	-1},    //201601, Server NO
	{DBID2_DATA_1005,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1006,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1007,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1008,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1009,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1010,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1011,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1012,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1013,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1014,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1015,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1016,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1017,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1018,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1019,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1020,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1021,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1022,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1023,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1024,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1025,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1026,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1027,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1028,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1029,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1030,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1031,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1032,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1033,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1034,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1035,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1036,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1037,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1038,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1039,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1040,	TYPE_WORD, 	0,	-1},	
	{DBID2_DATA_1041,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1042,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1043,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1044,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1045,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1046,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1047,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1048,	TYPE_WORD, 	0,	-1},	
	{DBID2_DATA_1049,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1050,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1051,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1052,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1053,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1054,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1055,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1056,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1057,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1058,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1059,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1050,	TYPE_WORD, 	0,	-1},	
	{DBID2_DATA_1061,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1062,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1063,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1064,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1065,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1066,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1067,	TYPE_WORD,  0,	-1},
	{DBID2_DATA_1068,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1069,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1070,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1071,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1072,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1073,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1074,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1075,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1076,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1077,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1078,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1079,	TYPE_WORD, 	0,	-1},
	{DBID2_DATA_1080,	TYPE_WORD, 	0,	-1},			//lcd backlight count  James add 2015/4/22 14:47:58
	{DBID2_DATA_1081,	TYPE_FLOAT, 	1,	-1},	
	{DBID2_DATA_1082,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1083,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1084,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1085,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1086,	TYPE_FLOAT,   1,	-1},
	{DBID2_DATA_1087,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1088,	TYPE_FLOAT,   1,	-1},
	{DBID2_DATA_1089,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1090,	TYPE_FLOAT, 	1,	-1},
	{DBID2_DATA_1091,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1092,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1093,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1094,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1095,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1096,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1097,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1098,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1099,	TYPE_FLOAT, 	2,	-1},
	{DBID2_DATA_1100,	TYPE_FLOAT, 	2,	-1}
};
			
/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/
void	InitDB2Def();	
int		GetDB2Size();

/*---------------------------------------------------------------------------+
|  Function : CtmInit::CtmInit()				                             |
|  Task     :  CtmInit構造函數                                               |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/

CtmInit::CtmInit() : CInit()
{
	m_pSelf		= this;
	
}

CtmInit::~CtmInit()
{
		
}

int		CtmInit::CreateSelf()
{
	g_tmDebugInfo->PrintDebugInfo("CtmInit::CreateSelf\n");
	Init();
	return 0;
}


int		CtmInit::FreeSelf()
{
	Destroy();
	return 0;
}
/*==========================================================================+
|  	Function	: Init                                 						|
|	Return		: 0			 		                    					|
|				  其它	錯誤代碼											|
|	Remark		: 															|
+==========================================================================*/
int CtmInit::Init()
{
	//	fans add 2008-11-13 11:14:33; for the global
	g_pGlobalFunVar	= dlopen("./lib/hmi_group/tmGlobalG.so", RTLD_LAZY);		
	
	g_tmDebugInfo->PrintDebugInfo("3.CtmInit::Init\n");
	
	//Init_ViewID();/*畫面緩存處理*/ /*wangli2016-3-22 13:07:38 3354不需要 畫面緩存處理*/
	#ifndef	D_3354
	OpenBattery_2();//2011-1-19 OpenBattery();
	#endif
	OpenRTC();
	Init_Timer();
	Beep();
	usleep(100000);
	Beep();
	usleep(900000);
	if (CtmConfig::GetInstance()->GetSramInit() <= 0)	CtmSram::GetInstance()->Delete();

	if(g_pParseXMLRecord == NULL)
	{
		char*	pPath = CtmConfig::GetInstance()->GetLanguagepath();
		char*	pFileName = CtmConfig::GetInstance()->GetRecordXMLFileName();
		char	pszFilePath[256];
		memset(pszFilePath, 0, sizeof(pszFilePath));
		sprintf(pszFilePath, "%s%s", pPath, pFileName);
		g_pParseXMLRecord = new CtmParseXMLRecord();
		//printf("1:pszFilePath=%s\n",pszFilePath);
		g_pParseXMLRecord->LoadXMLData(pszFilePath);
	}
	if(g_pParseXMLViewStr == NULL)
	{
		char*	pPath = CtmConfig::GetInstance()->GetLanguagepath();
		char*	pFileName = CtmConfig::GetInstance()->GetViewXMLFileName();
		char	pszFilePath[256];
		memset(pszFilePath, 0, sizeof(pszFilePath));
		sprintf(pszFilePath, "%s%s", pPath, pFileName);
		//printf("2:pszFilePath=%s\n",pszFilePath);
		g_pParseXMLViewStr = new CtmParseXMLRecord();
		g_pParseXMLViewStr->LoadXMLData(pszFilePath);
	}
	//_Init_KB();
	//_Init_KB();
	g_pFreeData	= static_cast<CtmFreeData*>(g_pPackage->CreateClassInstance("HmiFreeData"));
	GUI_Init();
	_GuiInitEx();													// <FANS> 2006/11/23 01:19下午
	Init_Block();													//<<yang 2006/12/8 
	Init_Bmp();
	Init_System();

	InitDB2Def();
	SetDB2Info(g_TMDB2, sizeof(g_TMDB2)/ sizeof(TMDB2));
	
	#ifdef	D_3354
	int	nValue = GetDB2Value(DBID2_DATA_1080).lValue;
	//printf("init nValue=%d \n", nValue);
	LCDPowerOn_3354(nValue);
	#else
	SPC_LCDON();													//	fans add 2007/1/9 04:26下午
	#endif
	
	if(Init_DB() & DB_ERR_FATAL)
	{
		perror("Init Database Error! Please check the database file\n");
		exit(1);
	}
	
	if(!GetFileType(CHECKFILETYPE))
	{
		Init_FlashEX();//James add 2008/4/29 06:07下午 要放到Init_DB()后面.
	}
	
	g_pExTable	= static_cast<CtmExtentTable*>(g_pPackage->CreateClassInstance("HostTable"));
	g_pDefault	=	static_cast<CtmDefault*>(g_pPackage->CreateClassInstance("HostDefault"));
	DealOperationStartMachine();//JOYCE2008-11-10 add for install
	SetInstallType();/*JOYCE2015-7-15 新停機功能flag*/
	
	BOOL	bNotInit = FALSE;
	if (CtmConfig::GetInstance()->GetSramInit() <= 0 || 
		(!CtmSarmRecordFileControl::GetInstance()->CheckData(-1)))
	{
		bNotInit = TRUE;
		//bNotInit = CtmSram::GetInstance()->Clear_AllSRAM() > 0;
	}
	
	if( g_ptmControlServer != NULL )
		g_ptmControlServer->ErrorControl()->Init_ErrorLog();		//James modify 2008/5/11 04:01下午
	Init_AxialConfig(CtmConfig::GetInstance()->GetConfigDealRamp());//fans add 2007/1/13 03:45下午
	g_pPackage->CreateClassInstance("HmiMsg");		//JOYCE2010-8-12
	if (bNotInit) CtmConfig::GetInstance()->SetSramInit(1);			//	fans add 
	
	//	fans 2008/6/18 06:16下午
	g_pFont 	= static_cast<CtmFontControl*>(g_pPackage->CreateClassInstance("Gui_Font"));
	g_pFontHelp	= static_cast<CtmFontControl*>(g_pPackage->CreateClassInstance("Gui_Font_Help"));//JOYCE2012-5-9 
	g_pSelvat	= static_cast<CtmVat_Ancestor*>(g_pPackage->CreateClassInstance("HostSelvat"));
	g_pFreeData	= static_cast<CtmFreeData*>(g_pPackage->CreateClassInstance("HmiFreeData"));
	
	Init_Resource();
	
	//InitCurrentUser();
	if( g_ptmControlServer != NULL )
		g_ptmControlServer->UserControl()->InitCurrentUser();		//James modify 2008/5/11 04:01下午
	if( g_ptmControlServer != NULL )
		g_ptmControlServer->MoniControl()->Init_MoniLog();			//James modify 2008/5/11 04:01下午
	if( g_ptmControlServer != NULL )
		g_ptmControlServer->MoldControl()->InitControl();
	//Init_MoniLog();
	Init_Mold();
	g_pTaskDataProcess = static_cast<CTaskDataProcess*>(g_pPackage->CreateClassInstance("DataProcess"));
	//printf("g_pTaskDataProcess=%x \n", g_pTaskDataProcess);
	
	//James add by the Keylock
	if( CtmConfig::GetInstance()->GetKeyLockFlag() == 1 || CtmConfig::GetInstance()->GetKeyLockFlag() == 2)
	{
		if(g_pExTable->GetTableValue(0,"D_7HTKEYLOCK"))  //2013-3-18 
		{
			g_iPrivilege = 0;       /*開機不登錄*/
			g_bKeyLockFlag = FALSE; /*0權級畫面 可設定光標可以移動(special)*/
		}
		else
		g_bStart = FALSE;
	}
	
	/* 機械手一體化   added by Wang.z.w  2015/9/15 9:47:30*/
	if(CtmConfig::GetInstance()->GetDspCom2Flag() == 5)
	{
		g_ptaskPickerComm = static_cast<CtmTaskPickerComm*>(g_pPackage->CreateClassInstance("HostTMTaskPickerComm"));
		if(NULL == g_ptaskPickerComm)
			printf("g_ptaskPickerComm is NULL.\n");
	}
	
	#ifdef	D_3354	
	if(CtmConfig::GetInstance()->GetPickerFlag())
	{
		g_ptaskpicker	= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("HostPicker"));
	}
	#endif
	
	//printf("Hardware_KBHardware_KBHardware_KB\n");
	g_ptaskKB	= static_cast<CtmKeyBoard*>(g_pPackage->CreateClassInstance("Hardware_KB"));
	g_pgpio	= static_cast<CtmGpio*>(g_pPackage->CreateClassInstance("Hardware_Gpio"));
	/*wangli2016-6-24 14:53:58  程序初始化未完成會當機，所以等初始化完成后再啟動按鍵板*/
	
	if(g_pExTable->GetTableValue(0,"D_AUXI")) /*joyce2016-10-11 輔機*/
		g_pTaskModbus = static_cast<CTaskModbus*>(g_pPackage->CreateClassInstance("HostModbus"));
	
	InitHelpTimer();//JOYCE2012-5-9 online help
	
	return 0;
}

int	CtmInit::Init_Resource()    //netdigger 2007/4/17
{
	CURSOR	 cursor;
	char	pszCursorPath[128];

	sprintf(pszCursorPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), "123.cur");
	cursor.pszName	= pszCursorPath;
	printf("%s\n", cursor.pszName);
	
	g_pCursor	= new CCursor;
	if(g_pCursor == NULL)	return -1;
	
	g_pCursor->Open(&cursor);
	g_pTouch	= static_cast<CTouchScreen*>(g_pPackage->CreateClassInstance("Hardware_TS"));
	
    return 0;
}

int	CtmInit::Destroy_Resource()    //netdigger 2007/4/17
{
	if(g_pCursor != NULL)
	{
		delete g_pCursor;
		g_pCursor	= NULL;
	}
	g_pPackage->ReleaseClassInstance("Hardware_TS", g_pTouch);
	g_pTouch = NULL;
	
	return 0;
}

/*==========================================================================+
|  	Function	: Init_BMP                       							|
|	Return		: 0			 		                    					|
|				  															|
|	Remark		: Initialize BMP											|
+==========================================================================*/
int	CtmInit::Init_Bmp()
{
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_Bmp\n");
	int 	i;
	
	i = 0;
	char	pszPath[512];
	while (g_aIniBmp[i].cBMPPath[0] != '\0')
	{
		g_aIniBmp[i].pIniBmp = new CtmBitMap();
		sprintf(pszPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), g_aIniBmp[i].cBMPPath); 
		g_aIniBmp[i].pIniBmp->Create(pszPath);
		strcpy(g_aIniBmp[i].cBMPPath, pszPath);
		i++;
	}
	while (g_aIniGif[i].cGifPath[0] != '\0')/*開機先加載gif圖片*/
	{
		g_aIniGif[i].pIniGif = new CtmGif();
		sprintf(pszPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), g_aIniGif[i].cGifPath); 
		g_aIniGif[i].pIniGif->Create(pszPath);
		strcpy(g_aIniGif[i].cGifPath, pszPath);
		i++;
	}
	return 0;
}

void   CtmInit::Init_System()
{
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_System\n");
	MULSTRINFO* pInfo;
	pInfo = new MULSTRINFO;	
	pInfo->strDir = CtmConfig::GetInstance()->GetLanguagepath();
	pInfo->strFileName = CtmConfig::GetInstance()->GetLanguageFileName();
	pInfo->nValueCode  = CtmConfig::GetInstance()->GetFontCode();
//	clock_t start, finish;
//	double  duration;
//	start = clock();
	//	最新的方式
	g_MultiLanguage.ReadMulStrings(pInfo);
//	finish = clock();
//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
   	//printf( "read string %2.4f seconds\n", duration );
	pInfo->strDir = CtmConfig::GetInstance()->GetLanguagepathHelp();//JOYCE2012-5-9 
 	if(strcmp(pInfo->strFileName, "Stablchn.txt") != 0 && 
 		 strcmp(pInfo->strFileName, "Stableng.txt") != 0 &&
   	 strcmp(pInfo->strFileName, "Stablchs.txt") != 0)
   	{
   		strcpy(pInfo->strFileName, "Stableng.txt");	
   	}	
	g_MultiLanguageHelp.ReadMulStrings(pInfo);//JOYCE2012-5-9 
		
	delete pInfo ;
}

int	CtmInit::Init_DB()
{
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_DB\n");
	DBINFO	dbinfo;
	char	szTemp0[512];
	char	szTemp1[512];
	char	szTemp2[512];
	char	szTemp3[512];	
	char 	szTemp4[512];
	//Sunny<20070622><<<<<
    sprintf(szTemp0, "%s", CtmConfig::GetInstance()->GetDBID());
	dbinfo.pszID		= szTemp0;
//	printf("szTemp0:%s\n",szTemp0);
    sprintf(szTemp1, "%s", CtmConfig::GetInstance()->GetDBName());
	dbinfo.pszName		= szTemp1;
//	printf("szTemp1:%s\n",szTemp1);
    sprintf(szTemp2, "%s", CtmConfig::GetInstance()->GetDBAttr());
	dbinfo.pszAttr		= szTemp2;
// 	printf("szTemp2:%s\n",szTemp2);
    sprintf(szTemp3, "%s", CtmConfig::GetInstance()->GetDBDefault());
	dbinfo.pszDefault	= szTemp3;
//	printf("szTemp3:%s\n",szTemp3);
	//Sunny<20070622>>>>>>
	dbinfo.pszMaxDef	= NULL;		// 最大默認值
	dbinfo.pszMinDef	= NULL;		// 最小默認值

	//alien add for database3.0
	memset(szTemp4, 0, 512);
	memcpy(szTemp4, szTemp3, 512);
	
	//獲取資料庫版本，ver文件必須與default文件在同一目錄下
	CtmFile File;
	char ver = '0';
	char *p = strrchr(szTemp4, '/');
	if(p != NULL)
		strcpy(p+1, "ver");
	if(File.Open(szTemp4) == tmFILE_GOOD)
	{
		File.Seek(14, SEEK_SET);
		File.Read(&ver,1);
		File.Close();
	}
	if(ver == '3')
	{
		dbinfo.nVersion = DATABASE_VERSION_3;
	}
	else
	{
		dbinfo.nVersion = DATABASE_VERSION_2;
	}
	g_pDatabase 		= static_cast<CDatabase*>(g_pPackage->CreateClassInstance("Database"));
	if(g_pDatabase == NULL)			//James add 2008/6/11 02:12下午
		g_pDatabase	= new CDatabase;

	return g_pDatabase->Create(&dbinfo);
}

int	CtmInit::Destroy_DB()
{
	g_pPackage->ReleaseClassInstance("Database", g_pDatabase);
	if (g_pDatabase != NULL)	delete g_pDatabase;
	return 0;
}

/*==========================================================================+
|  	Function	: Destroy_Bmp                          						|
|	Return		: 0			 		                    					|
|				  															|
|	Remark		: Release bmp												|
+==========================================================================*/
int CtmInit::Destroy_Bmp()
{
	int 	i;
	
	i = 0;
	while (g_aIniBmp[i].cBMPPath[0] != '\0')
	{
		delete g_aIniBmp[i].pIniBmp;
		i++;
	}
	return 0;
}


/*==========================================================================+
|  	Function	: Destroy_System                          						|
|	Return		: 0			 		                    					|
|				  其它	錯誤代碼											|
|	Remark		: 釋放字串資源												|
+==========================================================================*/
int CtmInit::Destroy_System()
{
	
	
	return 0;
}

int CtmInit::Destroy()
{
	Destroy_Timer();
	g_pPackage->ReleaseClassInstance("Gui_Font", g_pFont);
	//_Restore_KB();
	g_pPackage->ReleaseClassInstance("Hardware_KB", g_ptaskKB);
	g_pPackage->ReleaseClassInstance("Hardware_Gpio", g_pgpio);
	Destroy_Resource();
	
	Destroy_Bmp();
	Destroy_System();
	Destroy_DB();
	g_ptmControlServer->ErrorControl()->Destroy_ErrorLog();		//James modify 2008/6/11 11:39上午
	g_ptmControlServer->MoniControl()->Destroy_MoniLog();		//James modify 2008/6/11 11:39上午
	Destroy_Block();		//<<yang 2006/12/8 
	CloseRTC();
	CloseBattery();		
		
	g_pPackage->ReleaseClassInstance("HostTable", g_pExTable);
	g_pExTable = NULL;	
	
	return 0;
}

int CtmInit::Init_Block()
{
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_Block\n");
	char	szTemp[512];	//Sunny<20070622>
    sprintf(szTemp, "%s", CtmConfig::GetInstance()->GetDBBlock());		
	
	g_pBlock = new CtmBlock();
	g_pBlock->Create(szTemp/*"res/db/block"*/);
	
	return 0;
}

int	CtmInit::Destroy_Block()
{
	if (g_pBlock != NULL)	delete	g_pBlock;
	return 0;
}

//	fans	add 2007/1/13 09:32下午
void	CtmInit::InitData(char*	pszRank, char***	pszArys, int wData)
{
	g_tmDebugInfo->PrintDebugInfo("6CtmInit::InitData\n");
	char**	pszTemp = NULL;
	
	int i = 0, j = 0;
	BOOL	bFind = FALSE;
	long temp = GetDBValue(pszRank, &bFind).lValue;
	if (!bFind) return;
	while(pszArys[i] != NULL)
	{
		pszTemp = pszArys[i];
		j = 0;
		while(pszTemp[j] != NULL )
		{
			if ( j > (temp - 1))
			{
				SetDBValue(pszTemp[j], wData);
			}
			else   						//yangpeng 2007/5/25 02:50下午
			{
				if((GetDBValue(pszTemp[j]).lValue == 0xFFFF)||(GetDBValue(pszTemp[j]).lValue == 0xFFFFFFFF))
				{
					WORD DefaultValue =GetDBDefaultValue(pszTemp[j]).lValue;
					SetViewDBValue(pszTemp[j],DefaultValue);
				}
			}
			j++;
		}		
		i++;
	}	
}

void	CtmInit::InitDataCoreSlow(char*	pszRank, char***	pszArys, int wData)
{
	g_tmDebugInfo->PrintDebugInfo("6CtmInit::InitData\n");
	char**	pszTemp = NULL;
	
	int i = 0, j = 0;
	BOOL	bFind = FALSE;
	long temp = GetDBValue(pszRank, &bFind).lValue;
	if (!bFind) return;
	while(pszArys[i] != NULL)
	{
		pszTemp = pszArys[i];
	
		j = 0;
		while(pszTemp[j] != NULL )
		{
			if ( j > (temp - 1))
			{
				SetDBValue(pszTemp[j], wData);
			}
			else   						//yangpeng 2007/5/25 02:50下午
			{
				if((GetDBValue(pszTemp[j]).lValue == 0xFFFF)||(GetDBValue(pszTemp[j]).lValue == 0xFFFFFFFF))
				{
					int   CoreSlowDefValue[] ={50,50,300,45,45,300};
					//printf("i:%d   j:%d  temp:%d\n   ",i,j,temp);
					SetViewDBValue(pszTemp[j],CoreSlowDefValue[i]);
				}
			}
			j++;
		}		
		i++;
	}	
}

//	close mold
void	CtmInit::Init_CloseMoldConfig(int nRampFlag)
{
	g_tmDebugInfo->PrintDebugInfo("5CtmInit::Init_CloseMoldConfig\n");
	// pressure for close mold
	char*	pszCloseMoldPres[] =
	{
		"MSET_MLD_CLOSE_STEP1_PRESSURE",
		"NULL							",
		"NULL							",
		"MSET_MLD_CLOSE_STEP2_PRESSURE",
		"MSET_MLD_CLOSE_STEP3_PRESSURE",		
		NULL
	};	
	//	speed for close mold
	char*	pszCloseMoldSpeed[] =
	{
		"MSET_MLD_CLOSE_STEP1_FLOW",
		"NULL							",
		"NULL							",
		"MSET_MLD_CLOSE_STEP2_FLOW",
		"MSET_MLD_CLOSE_STEP3_FLOW",
		NULL
	};	
	//	position for close mold
	char*	pszCloseMoldPos[] =
	{
		"MSET_MLD_CLOSE_STEP1_POSITION",
		"NULL							",
		"NULL							",
		"MSET_MLD_CLOSE_STEP2_POSITION",
		"MSET_MLD_CLOSE_STEP3_POSITION",
		NULL
	};	
	char*	pszCloseMoldPresRamp[] =
	{
		"MACH_MLD_CLOSE_RAMP_PRESTIME",
		"NULL",
		"NULL",
		"MACH_MLD_CLOSE_RAMP2_PRESTIME",
		"MACH_MLD_CLOSE_RAMP3_PRESTIME",
		NULL
	};
	char*	pszCloseMoldFlowRamp[] =
	{
		"MACH_MLD_CLOSE_RAMP_FLOWTIME",
		"NULL",
		"NULL",
		"MACH_MLD_CLOSE_RAMP2_FLOWTIME",
		"MACH_MLD_CLOSE_RAMP3_FLOWTIME",
		NULL
	};	
	char**	pszCloseMold[] =
	{
		pszCloseMoldPres, 
		pszCloseMoldSpeed, 
		pszCloseMoldPos,
		pszCloseMoldPresRamp,
		pszCloseMoldFlowRamp,
		NULL
	};
	
	if(nRampFlag == 0)
	{
		pszCloseMold[3] = NULL;
		pszCloseMold[4] = NULL;
	}
	
	InitData("MCONF_MOLD_CLOSE_NULL_RANK", pszCloseMold, 0xFFFF);
}

//	open mold 
void	CtmInit::Init_OpenMoldConfig(int nDealRamp)
{
	g_tmDebugInfo->PrintDebugInfo("5CtmInit::Init_OpenMoldConfig\n");
	//	pressure for open mold
	char*	pszOpenMoldPres[] =
	{
		"MSET_MLD_OPEN_STEP1_PRESSURE",
		"MSET_MLD_OPEN_STEP5_PRESSURE",
		"MSET_MLD_OPEN_STEP2_PRESSURE",
		"MSET_MLD_OPEN_STEP3_PRESSURE",
		"MSET_MLD_OPEN_STEP4_PRESSURE",
		
		NULL
	};	
	//	speed for open mold
	char*	pszOpenMoldSpeed[] =
	{
		"MSET_MLD_OPEN_STEP1_FLOW",
		"MSET_MLD_OPEN_STEP5_FLOW",
		"MSET_MLD_OPEN_STEP2_FLOW",
		"MSET_MLD_OPEN_STEP3_FLOW",
		"MSET_MLD_OPEN_STEP4_FLOW",
		
		NULL
	};	
	//	position for open mold
	char*	pszOpenMoldPos[] =
	{
		"MSET_MLD_OPEN_STEP1_POSITION",
		"MSET_MLD_OPEN_STEP5_POSITION",
		"MSET_MLD_OPEN_STEP2_POSITION",
		"MSET_MLD_OPEN_STEP3_POSITION",
		"MSET_MLD_OPEN_STEP4_POSITION",
		
		NULL
	};	
	char*	pszOpenMoldPresRamp[] =
	{
		"MACH_MLD_OPEN_RAMP_PRESTIME",
		"MACH_MLD_OPEN_RAMP5_PRESTIME",
		"MACH_MLD_OPEN_FASTRAMP_PRESTIME",
		"MACH_MLD_OPEN_RAMP3_PRESTIME",
		"MACH_MLD_OPEN_RAMP4_PRESTIME",
		NULL
	};
	char*	pszOpenMoldFlowRamp[] =
	{
		"MACH_MLD_OPEN_RAMP_FLOWTIME",
		"MACH_MLD_OPEN_FASTRAMP_FLOWTIME",
		"MACH_MLD_OPEN_RAMP3_FLOWTIME",
		"MACH_MLD_OPEN_RAMP4_FLOWTIME",
		"MACH_MLD_OPEN_RAMP52_FLOWTIME",
		NULL
	};	
	char**	pszOpenMold[] =
	{
		pszOpenMoldPres, 
		pszOpenMoldSpeed,
		pszOpenMoldPos,
		pszOpenMoldPresRamp,
		pszOpenMoldFlowRamp,
		NULL
	};
	
	if(nDealRamp == 0)
	{
		pszOpenMold[3] = NULL;
		pszOpenMold[4] = NULL;
	}
	InitData("MCONF_MOLD_OPEN_NULL_RANK", pszOpenMold, 0xFFFF);
}

//	inject
void	CtmInit::Init_InjectConfig(int nDealRamp)
{
	//	pressure for inject
	char*	pszInjectPress[] =
	{
		"MSET_SCRW_INJ_STEP1_PRESSURE",
		"MSET_SCRW_INJ_STEP2_PRESSURE",
		"MSET_SCRW_INJ_STEP3_PRESSURE",
		"MSET_SCRW_INJ_STEP4_PRESSURE",
		"MSET_SCRW_INJ_STEP5_PRESSURE",
		"MSET_SCRW_INJ_STEP6_PRESSURE",
		NULL
	};	
	//	speed for inject
	char*	pszInjectSpeed[] =
	{
		"MSET_SCRW_INJ_STEP1_FLOW",
		"MSET_SCRW_INJ_STEP2_FLOW",
		"MSET_SCRW_INJ_STEP3_FLOW",
		"MSET_SCRW_INJ_STEP4_FLOW",
		"MSET_SCRW_INJ_STEP5_FLOW",
		"MSET_SCRW_INJ_STEP6_FLOW",
		NULL
	};	
	//	Position for inject
	char*	pszInjectPos[] =
	{
		"MSET_SCRW_INJ_STEP1_POSITION",			//	
		"MSET_SCRW_INJ_STEP2_POSITION",
		"MSET_SCRW_INJ_STEP3_POSITION",
		"MSET_SCRW_INJ_STEP4_POSITION",
		"MSET_SCRW_INJ_STEP5_POSITION",
		"MSET_SCRW_INJ_STEP6_POSITION",
		NULL
	};
	char*	pszInjectPresRamp[] =
	{
		"MACH_SCRW_INJ_RAMP_PRESTIME",
		"MACH_SCRW_INJ_RAMP2_PRESTIME",
		"MACH_SCRW_INJ_RAMP3_PRESTIME",
		"MACH_SCRW_INJ_RAMP4_PRESTIME",
		"MACH_SCRW_INJ_RAMP5_PRESTIME",
		"MACH_SCRW_INJ_RAMP6_PRESTIME",
		NULL
	};
	char*	pszInjectFlowRamp[] =
	{
		"MACH_SCRW_INJ_RAMP_FLOWTIME",
		"MACH_SCRW_INJ_RAMP2_FLOWTIME",
		"MACH_SCRW_INJ_RAMP3_FLOWTIME",
		"MACH_SCRW_INJ_RAMP4_FLOWTIME",
		"MACH_SCRW_INJ_RAMP5_FLOWTIME",
		"MACH_SCRW_INJ_RAMP6_FLOWTIME",
		NULL
	};	
	char**	pszInject[] =
	{
		pszInjectPress, 
		pszInjectSpeed,
		pszInjectPos,
		pszInjectPresRamp,
		pszInjectFlowRamp,
		NULL
	};
	if(nDealRamp==0)
	{
		pszInject[3] = NULL;
		pszInject[4] = NULL;	
	}
	InitData("MCONF_SCREW_INJECT_NULL_RANK", pszInject, 0xFFFF);
}

void	CtmInit::Init_InjectBConfig(int nDealRamp)
{
	//	pressure for inject
	char*	pszInjectPress[] =
	{
		"MSETB_SCRW2_INJ_STEP1_PRESSURE",
		"MSETB_SCRW2_INJ_STEP2_PRESSURE",
		"MSETB_SCRW2_INJ_STEP3_PRESSURE",
		"MSETB_SCRW2_INJ_STEP4_PRESSURE",
		"MSETB_SCRW2_INJ_STEP5_PRESSURE",
		"MSETB_SCRW2_INJ_STEP6_PRESSURE",
		NULL
	};	
	//	speed for inject
	char*	pszInjectSpeed[] =
	{
		"MSETB_SCRW2_INJ_STEP1_FLOW",
		"MSETB_SCRW2_INJ_STEP2_FLOW",
		"MSETB_SCRW2_INJ_STEP3_FLOW",
		"MSETB_SCRW2_INJ_STEP4_FLOW",
		"MSETB_SCRW2_INJ_STEP5_FLOW",
		"MSETB_SCRW2_INJ_STEP6_FLOW",
		NULL
	};	
	//	Position for inject
	char*	pszInjectPos[] =
	{
		"MSETB_SCRW2_INJ_STEP1_POSITION",			//	
		"MSETB_SCRW2_INJ_STEP2_POSITION",
		"MSETB_SCRW2_INJ_STEP3_POSITION",
		"MSETB_SCRW2_INJ_STEP4_POSITION",
		"MSETB_SCRW2_INJ_STEP5_POSITION",
		"MSETB_SCRW2_INJ_STEP6_POSITION",
		NULL
	};
	char*	pszInjectPresRamp[] =
	{
		"MACHB_SCRW2_INJ_RAMP_PRESTIME",
		"MACHB_SCRW2_INJ_RAMP2_PRESTIME",
		"MACHB_SCRW2_INJ_RAMP3_PRESTIME",
		"MACHB_SCRW2_INJ_RAMP4_PRESTIME",
		"MACHB_SCRW2_INJ_RAMP5_PRESTIME",
		"MACHB_SCRW2_INJ_RAMP6_PRESTIME",
		NULL
	};
	char*	pszInjectFlowRamp[] =
	{
		"MACHB_SCRW2_INJ_RAMP_FLOWTIME",
		"MACHB_SCRW2_INJ_RAMP2_FLOWTIME",
		"MACHB_SCRW2_INJ_RAMP3_FLOWTIME",
		"MACHB_SCRW2_INJ_RAMP4_FLOWTIME",
		"MACHB_SCRW2_INJ_RAMP5_FLOWTIME",
		"MACHB_SCRW2_INJ_RAMP6_FLOWTIME",
		NULL
	};	
	char**	pszInject[] =
	{
		pszInjectPress, 
		pszInjectSpeed,
		pszInjectPos,
		pszInjectPresRamp,
		pszInjectFlowRamp,
		NULL
	};
	if(nDealRamp==0)
	{
		pszInject[3] = NULL;
		pszInject[4] = NULL;	
	}
	InitData("MCONF_SCREW2_INJECT_NULL_RANK", pszInject, 0xFFFF);
}
//	HoldPressure
void	CtmInit::Init_HoldConfig(int nDealRamp)
{
	//	pressure for hold
	char*	pszHoldPres[] =
	{
		"MSET_SCRW_HLDP_STEP1_PRESSURE",
		"MSET_SCRW_HLDP_STEP2_PRESSURE",
		"MSET_SCRW_HLDP_STEP3_PRESSURE",
		"MSET_SCRW_HLDP_STEP4_PRESSURE",
		"MSET_SCRW_HLDP_STEP5_PRESSURE",
		"MSET_SCRW_HLDP_STEP6_PRESSURE",
		NULL
	};
	//	speed for hold
	char*	pszHoldSpeed[] =
	{
		"MSET_SCRW_HLDP_STEP1_FLOW",
		"MSET_SCRW_HLDP_STEP2_FLOW",
		"MSET_SCRW_HLDP_STEP3_FLOW",
		"MSET_SCRW_HLDP_STEP4_FLOW",
		"MSET_SCRW_HLDP_STEP5_FLOW",
		"MSET_SCRW_HLDP_STEP6_FLOW",
		NULL
	};
	//	position for hold
	char*	pszHoldPos[] =
	{
		"MSET_SCRW_HLDP_STEP1_TIME",
		"MSET_SCRW_HLDP_STEP2_TIME",
		"MSET_SCRW_HLDP_STEP3_TIME",
		"MSET_SCRW_HLDP_STEP4_TIME",
		"MSET_SCRW_HLDP_STEP5_TIME",
		"MSET_SCRW_HLDP_STEP6_TIME",
		NULL
	};
	char*	pszHoldPresRamp[] =
	{
		"MACH_SCRW_HLDP_RAMP_PRESTIME",
		"MACH_SCRW_HLDP_RAMP2_PRESTIME",
		"MACH_SCRW_HLDP_RAMP3_PRESTIME",
		"MACH_SCRW_HLDP_RAMP4_PRESTIME",
		"MACH_SCRW_HLDP_RAMP5_PRESTIME",
		"MACH_SCRW_HLDP_RAMP6_PRESTIME",
		NULL
	};
	char*	pszHoldFlowRamp[] =
	{
		"MACH_SCRW_HLDP_RAMP_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP2_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP3_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP4_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP5_FLOWTIME",
		"MACH_SCRW_HLDP_RAMP6_FLOWTIME",
		NULL
	};	
	char**	pszHold[] =
	{
		pszHoldPres, 
		pszHoldSpeed,
		pszHoldPos,
		pszHoldPresRamp,
		pszHoldFlowRamp,
		NULL
	};
	
	if(nDealRamp==0)
	{
		pszHold[3] = NULL;
		pszHold[4] = NULL;	
	}
	if(CtmConfig::GetInstance()->GetKeyFlag()==0)
	{
		pszHoldPres[5] = NULL;
		pszHoldSpeed[5] = NULL;
		pszHoldPos[5] = NULL;
		pszHoldPresRamp[5] = NULL;
		pszHoldFlowRamp[5] = NULL;
	}
	InitData("MCONF_SCREW_HOLDPRESSURE_NULL_RANK", pszHold, 0xFFFFFFFF);
}

void	CtmInit::Init_HoldBConfig(int nDealRamp)
{
	//	pressure for hold
	char*	pszHoldPres[] =
	{
		"MSETB_SCRW2_HLDP_STEP1_PRESSURE",
		"MSETB_SCRW2_HLDP_STEP2_PRESSURE",
		"MSETB_SCRW2_HLDP_STEP3_PRESSURE",
		"MSETB_SCRW2_HLDP_STEP4_PRESSURE",
		"MSETB_SCRW2_HLDP_STEP5_PRESSURE",
		"MSETB_SCRW2_HLDP_STEP6_PRESSURE",
		NULL
	};
	//	speed for hold
	char*	pszHoldSpeed[] =
	{
		"MSETB_SCRW2_HLDP_STEP1_FLOW",
		"MSETB_SCRW2_HLDP_STEP2_FLOW",
		"MSETB_SCRW2_HLDP_STEP3_FLOW",
		"MSETB_SCRW2_HLDP_STEP4_FLOW",
		"MSETB_SCRW2_HLDP_STEP5_FLOW",
		"MSETB_SCRW2_HLDP_STEP6_FLOW",
		NULL
	};
	//	position for hold
	char*	pszHoldPos[] =
	{
		"MSETB_SCRW2_HLDP_STEP1_TIME",
		"MSETB_SCRW2_HLDP_STEP2_TIME",
		"MSETB_SCRW2_HLDP_STEP3_TIME",
		"MSETB_SCRW2_HLDP_STEP4_TIME",
		"MSETB_SCRW2_HLDP_STEP5_TIME",
		"MSETB_SCRW2_HLDP_STEP6_TIME",
		NULL
	};
	char*	pszHoldPresRamp[] =
	{
		"MACHB_SCRW2_HLDP_RAMP_PRESTIME",
		"MACHB_SCRW2_HLDP_RAMP2_PRESTIME",
		"MACHB_SCRW2_HLDP_RAMP3_PRESTIME",
		"MACHB_SCRW2_HLDP_RAMP4_PRESTIME",
		"MACHB_SCRW2_HLDP_RAMP5_PRESTIME",
		"MACHB_SCRW2_HLDP_RAMP6_PRESTIME",
		NULL
	};
	char*	pszHoldFlowRamp[] =
	{
		"MACHB_SCRW2_HLDP_RAMP_FLOWTIME",
		"MACHB_SCRW2_HLDP_RAMP2_FLOWTIME",
		"MACHB_SCRW2_HLDP_RAMP3_FLOWTIME",
		"MACHB_SCRW2_HLDP_RAMP4_FLOWTIME",
		"MACHB_SCRW2_HLDP_RAMP5_FLOWTIME",
		"MACHB_SCRW2_HLDP_RAMP6_FLOWTIME",
		NULL
	};	
	char**	pszHold[] =
	{
		pszHoldPres, 
		pszHoldSpeed,
		pszHoldPos,
		pszHoldPresRamp,
		pszHoldFlowRamp,
		NULL
	};
	
	if(nDealRamp==0)
	{
		pszHold[3] = NULL;
		pszHold[4] = NULL;	
	}
	if(CtmConfig::GetInstance()->GetKeyFlag()==0)
	{
		pszHoldPres[5] = NULL;
		pszHoldSpeed[5] = NULL;
		pszHoldPos[5] = NULL;
		pszHoldPresRamp[5] = NULL;
		pszHoldFlowRamp[5] = NULL;
	}
	InitData("MCONF_SCREW2_HOLDPRESSURE_NULL_RANK", pszHold, 0xFFFFFFFF);
}
//	charge
void	CtmInit::Init_ChargeConfig(int nDealRamp)
{
	//	pressure for charge
	char*	pszRcvPress[] =
	{
		"MSET_SCRW_RCV_STEP1_PRESSURE",
		"MSET_SCRW_RCV_STEP2_PRESSURE",
		"MSET_SCRW_RCV_STEP3_PRESSURE",
		"MSET_SCRW_RCV_STEP4_PRESSURE",
		"MSET_SCRW_RCV_STEP5_PRESSURE",
		NULL
	};
	//	speed for charge
	char*	pszRcvSpeed[] =
	{
		"MSET_SCRW_RCV_STEP1_FLOW",
		"MSET_SCRW_RCV_STEP2_FLOW",
		"MSET_SCRW_RCV_STEP3_FLOW",
		"MSET_SCRW_RCV_STEP4_FLOW",
		"MSET_SCRW_RCV_STEP5_FLOW",
		NULL
	};
	//	Position for charge
	char*	pszRcvPos[] =
	{
		"MSET_SCRW_RCV_STEP1_POSITION",
		"MSET_SCRW_RCV_STEP2_POSITION",
		"MSET_SCRW_RCV_STEP3_POSITION",
		"MSET_SCRW_RCV_STEP4_POSITION",
		"MSET_SCRW_RCV_STEP5_POSITION",
		NULL
	};
	//	back pressure for charge
	char*	pszRcvBackPress[] =
	{
		"MSET_SCRW_RCV_STEP1_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP2_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP3_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP4_BACKPRESSURE",
		"MSET_SCRW_RCV_STEP5_BACKPRESSURE",
		NULL
	};
	char*	pszRcvPresRamp[] =
	{
		"MACH_SCRW_RCV_RAMP_PRESTIME",
		"MACH_SCRW_RCV_RAMP2_PRESTIME",
		"MACH_SCRW_RCV_RAMP3_PRESTIME",
		"MACH_SCRW_RCV_RAMP4_PRESTIME",
		"MACH_SCRW_RCV_RAMP5_PRESTIME",
		NULL
	};
	char*	pszRcvFlowRamp[] =
	{
		"MACH_SCRW_RCV_RAMP_FLOWTIME",
		"MACH_SCRW_RCV_RAMP2_FLOWTIME",
		"MACH_SCRW_RCV_RAMP3_FLOWTIME",
		"MACH_SCRW_RCV_RAMP4_FLOWTIME",
		"MACH_SCRW_RCV_RAMP5_FLOWTIME",
		NULL
	};	
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		pszRcvBackPress,
		pszRcvPresRamp,
		pszRcvFlowRamp,
		NULL
	};
	
	if(nDealRamp ==0)
	{
		pszRcv[4] = NULL;
		pszRcv[5] = NULL;	
	}
	InitData("MCONF_SCREW_RECOVERY_NULL_RANK", pszRcv, 0xFFFF);
}

void	CtmInit::Init_ChargeBConfig(int nDealRamp)
{
	//	pressure for charge
	char*	pszRcvPress[] =
	{
		"MSETB_SCRW2_RCV_STEP1_PRESSURE",
		"MSETB_SCRW2_RCV_STEP2_PRESSURE",
		"MSETB_SCRW2_RCV_STEP3_PRESSURE",
		"MSETB_SCRW2_RCV_STEP4_PRESSURE",
		"MSETB_SCRW2_RCV_STEP5_PRESSURE",
		NULL
	};
	//	speed for charge
	char*	pszRcvSpeed[] =
	{
		"MSETB_SCRW2_RCV_STEP1_FLOW",
		"MSETB_SCRW2_RCV_STEP2_FLOW",
		"MSETB_SCRW2_RCV_STEP3_FLOW",
		"MSETB_SCRW2_RCV_STEP4_FLOW",
		"MSETB_SCRW2_RCV_STEP5_FLOW",
		NULL
	};
	//	Position for charge
	char*	pszRcvPos[] =
	{
		"MSETB_SCRW2_RCV_STEP1_POSITION",
		"MSETB_SCRW2_RCV_STEP2_POSITION",
		"MSETB_SCRW2_RCV_STEP3_POSITION",
		"MSETB_SCRW2_RCV_STEP4_POSITION",
		"MSETB_SCRW2_RCV_STEP5_POSITION",
		NULL
	};
	//	back pressure for charge
	char*	pszRcvBackPress[] =
	{
		"MSETB_SCRW2_RCV_STEP1_BACKPRESSURE",
		"MSETB_SCRW2_RCV_STEP2_BACKPRESSURE",
		"MSETB_SCRW2_RCV_STEP3_BACKPRESSURE",
		"MSETB_SCRW2_RCV_STEP4_BACKPRESSURE",
		"MSETB_SCRW2_RCV_STEP5_BACKPRESSURE",
		NULL
	};
	char*	pszRcvPresRamp[] =
	{
		"MACHB_SCRW2_RCV_RAMP_PRESTIME",
		"MACHB_SCRW2_RCV_RAMP2_PRESTIME",
		"MACHB_SCRW2_RCV_RAMP3_PRESTIME",
		"MACHB_SCRW2_RCV_RAMP4_PRESTIME",
		"MACHB_SCRW2_RCV_RAMP5_PRESTIME",
		NULL
	};
	char*	pszRcvFlowRamp[] =
	{
		"MACHB_SCRW2_RCV_RAMP_FLOWTIME",
		"MACHB_SCRW2_RCV_RAMP2_FLOWTIME",
		"MACHB_SCRW2_RCV_RAMP3_FLOWTIME",
		"MACHB_SCRW2_RCV_RAMP4_FLOWTIME",
		"MACHB_SCRW2_RCV_RAMP5_FLOWTIME",
		NULL
	};	
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		pszRcvBackPress,
		pszRcvPresRamp,
		pszRcvFlowRamp,
		NULL
	};
	
	if(nDealRamp ==0)
	{
		pszRcv[4] = NULL;
		pszRcv[5] = NULL;	
	}
	InitData("MCONF_SCREW2_RECOVERY_NULL_RANK", pszRcv, 0xFFFF);
}
//Sunny<20070614><<<<<
//托模進
void	CtmInit::Init_EjtADVConfig(int nDealRamp)
{
	//	pressure for EjtADV
	char*	pszRcvPress[] =
	{
		"MSET_EJT_FWD_STEP1_PRESSURE",
		"MSET_EJT_FWD_NULL_PRESSURE",
		NULL
	};
	//	speed for EjtADV
	char*	pszRcvSpeed[] =
	{
		"MSET_EJT_FWD_STEP1_FLOW",
		"MSET_EJT_FWD_NULL_FLOW",
		NULL
	};
	//	Position for EjtADV
	char*	pszRcvPos[] =
	{
		"MSET_EJT_FWD_STEP1_POSITION",
		"MSET_EJT_FWD_NULL_POSITION",
		NULL
	};
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		NULL
	};
	InitData("MCONF_EJECTOR_FORWARD_NULL_RANK", pszRcv, 0xFFFF);
}
//托模退
void	CtmInit::Init_EjtRETConfig(int nDealRamp)
{
	//	pressure for EjtADV
	char*	pszRcvPress[] =
	{
		"MSET_EJT_BWD_NULL_VIBRATEPRESSURE",
		"MSET_EJT_BWD_NULL_PRESSURE",
		NULL
	};
	//	speed for EjtADV
	char*	pszRcvSpeed[] =
	{
		"MSET_EJT_BWD_NULL_VIBRATEFLOW",
		"MSET_EJT_BWD_NULL_FLOW",
		NULL
	};
	//	Position for EjtADV
	char*	pszRcvPos[] =
	{
		"MSET_EJT_BWD_STEP1_POSITION",
		"MSET_EJT_BWD_NULL_POSITION",
		NULL
	};
	char**	pszRcv[] =
	{
		pszRcvPress, 
		pszRcvSpeed,
		pszRcvPos,
		NULL
	};
	InitData("MCONF_EJECTOR_BACKWORD_NULL_RANK", pszRcv, 0xFFFF);
}
//Sunny<20070614>>>>>>>

//	air blast
void	CtmInit::Init_AirConfig()
{
	//	Position for air
	char*	pszAirPos[] =
	{
		"MSET2_BLST1_ON_START_POSITION",
		"MSET2_MBLST1_ON_START_POSITION",
		"MSET2_BLST2_ON_START_POSITION",
		"MSET2_MBLST2_ON_START_POSITION",
		"MSET2_BLST3_ON_START_POSITION",
		"MSET2_MBLST3_ON_START_POSITION",
		NULL
	};
	//	delay time for air
	char*	pszAirDelayTime[] =
	{
		"MSET2_BLST1_ON_START_DELAYTIME",
		"MSET2_MBLST1_ON_START_DELAYTIME",
		"MSET2_BLST2_ON_START_DELAYTIME",
		"MSET2_MBLST2_ON_START_DELAYTIME",
		"MSET2_BLST3_ON_START_DELAYTIME",
		"MSET2_MBLST3_ON_START_DELAYTIME",
		NULL
	};
	//	total time for air
	char*	pszAirTotalTime[] =
	{
		"MSET2_BLST1_ON_NULL_TOTALTIME",
		"MSET2_MBLST1_ON_NULL_TOTALTIME",
		"MSET2_BLST2_ON_NULL_TOTALTIME",
		"MSET2_MBLST2_ON_NULL_TOTALTIME",
		"MSET2_BLST3_ON_NULL_TOTALTIME",
		"MSET2_MBLST3_ON_NULL_TOTALTIME",
		NULL
	};

	char**	pszAir[] =
	{
		pszAirPos, 
		pszAirDelayTime,
		pszAirTotalTime,
		NULL
	};
	InitData("MCONF_AIRBLAST_AIRBLAST_NULL_SET", pszAir, 0xFFFF);	


	if(!(CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC))/*joyce2012-12-11 吹氣擴展功能*/
	{
		char sz[128];
		int AirGroup =GetDBValue("MCONF_AIRBLAST_AIRBLAST_NULL_SET").lValue;
		int nTemp = GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED15").lValue;
		for(int i=2; i<6; i++)
		{
			if(i%2)
				sprintf(sz,"MSET2_MBLST%d_NULL_MODE",i/2+1);
			else
				sprintf(sz,"MSET2_BLST%d_NULL_MODE",i/2+1);
			//printf("init i:%d, sz:%s \n", i, sz);
			if((nTemp == 0) || ((nTemp == 1) && (i >= AirGroup)))
				SetDBValue(sz, 0xffff);
			else  if((nTemp == 1) && (i < AirGroup))
			{
				if(GetDBValue(sz).lValue == 0xffff)
					SetDBValue(sz, 0);
			}
		}
	}	
}

//	core
void	CtmInit::Init_CoreConfig()
{
	//	function for core
	char*	pszCoreFunction[] =
	{
		"MSET_CORA_CORA_NULL_CONTROLOPTION",
		"MSET_CORB_CORB_NULL_CONTROLOPTION",
		"MSET_CORC_CORC_NULL_CONTROLOPTION",
		"MSET_CORD_CORD_NULL_CONTROLOPTION",
		"MSET_CORE_CORE_NULL_CONTROLOPTION",
		"MSET_CORF_CORF_NULL_CONTROLOPTION",
		"MSET2_CORG_CORG_NULL_CONTROLOPTION",
		"MSET2_CORH_CORH_NULL_CONTROLOPTION",
		NULL
	};
	//	in mode for core
	char*	pszCoreInMode[] =
	{
		"MSET_CORA_IN_NULL_MODEOPTION",
		"MSET_CORB_IN_NULL_MODEOPTION",
		"MSET_CORC_IN_NULL_MODEOPTION",
		"MSET_CORD_IN_NULL_MODEOPTION",
		"MSET_CORE_IN_NULL_MODEOPTION",
		"MSET_CORF_IN_NULL_MODEOPTION",
		"MSET2_CORG_IN_NULL_MODEOPTION",
		"MSET2_CORH_IN_NULL_MODEOPTION",
		NULL
	};
	//	input pressure for core
	char*	pszCoreInPress[] =
	{
		"MSET_CORA_IN_NULL_PRESSURE",
		"MSET_CORB_IN_NULL_PRESSURE",
		"MSET_CORC_IN_NULL_PRESSURE",
		"MSET_CORD_IN_NULL_PRESSURE",
		"MSET_CORE_IN_NULL_PRESSURE",
		"MSET_CORF_IN_NULL_PRESSURE",
		"MSET2_CORG_IN_NULL_PRESSURE",
		"MSET2_CORH_IN_NULL_PRESSURE",
		NULL
	};
	//	input Speed for core
	char*	pszCoreInSpeed[] =
	{
		"MSET_CORA_IN_NULL_FLOW",
		"MSET_CORB_IN_NULL_FLOW",
		"MSET_CORC_IN_NULL_FLOW",
		"MSET_CORD_IN_NULL_FLOW",
		"MSET_CORE_IN_NULL_FLOW",
		"MSET_CORF_IN_NULL_FLOW",
		"MSET2_CORG_IN_NULL_FLOW",
		"MSET2_CORH_IN_NULL_FLOW",
		NULL
	};
	//	input Time for core
	char*	pszCoreInTime[] =
	{
		"MSET_CORA_IN_NULL_TIME",
		"MSET_CORB_IN_NULL_TIME",
		"MSET_CORC_IN_NULL_TIME",
		"MSET_CORD_IN_NULL_TIME",
		"MSET_CORE_IN_NULL_TIME",
		"MSET_CORF_IN_NULL_TIME",
		"MSET2_CORG_IN_NULL_TIME",
		"MSET2_CORH_IN_NULL_TIME",
		NULL
	};
	//	input Counter for core
	char*	pszCoreInCount[] =
	{
		"MSET_CORA_IN_NULL_COUNTER",
		"MSET_CORB_IN_NULL_COUNTER",
		"MSET_CORC_IN_NULL_COUNTER",
		"MSET_CORD_IN_NULL_COUNTER",
		"MSET_CORE_IN_NULL_COUNTER",
		"MSET_CORF_IN_NULL_COUNTER",
		"MSET2_CORG_IN_NULL_COUNTER",
		"MSET2_CORH_IN_NULL_COUNTER",
		NULL
	};
	//	input act position point for core
	char*	pszCoreInActPoint[] =
	{
		"MSET_CORA_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORB_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORC_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORD_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORE_IN_NULL_ACTPOSITIONOPTION",
		"MSET_CORF_IN_NULL_ACTPOSITIONOPTION",
		"MSET2_CORG_IN_NULL_ACTPOSITIONOPTION",
		"MSET2_CORH_IN_NULL_ACTPOSITIONOPTION",
		NULL
	};
	//	input set mold's position  for core
	char*	pszCoreInSetPos[] =
	{
		"MSET_CORA_IN_NULL_SETMOLDPOSITION",
		"MSET_CORB_IN_NULL_SETMOLDPOSITION",
		"MSET_CORC_IN_NULL_SETMOLDPOSITION",
		"MSET_CORD_IN_NULL_SETMOLDPOSITION",
		"MSET_CORE_IN_NULL_SETMOLDPOSITION",
		"MSET_CORF_IN_NULL_SETMOLDPOSITION",
		"MSET2_CORG_IN_NULL_SETMOLDPOSITION",
		"MSET2_CORH_IN_NULL_SETMOLDPOSITION",
		NULL
	};	
	//	out mode for core
	char*	pszCoreOutMode[] =
	{
		"MSET_CORA_OUT_NULL_MODEOPTION",
		"MSET_CORB_OUT_NULL_MODEOPTION",
		"MSET_CORC_OUT_NULL_MODEOPTION",
		"MSET_CORD_OUT_NULL_MODEOPTION",
		"MSET_CORE_OUT_NULL_MODEOPTION",
		"MSET_CORF_OUT_NULL_MODEOPTION",
		"MSET2_CORG_OUT_NULL_MODEOPTION",
		"MSET2_CORH_OUT_NULL_MODEOPTION",
		NULL
	};
	//	out pressure for core
	char*	pszCoreOutPress[] =
	{
		"MSET_CORA_OUT_NULL_PRESSURE",
		"MSET_CORB_OUT_NULL_PRESSURE",
		"MSET_CORC_OUT_NULL_PRESSURE",
		"MSET_CORD_OUT_NULL_PRESSURE",
		"MSET_CORE_OUT_NULL_PRESSURE",
		"MSET_CORF_OUT_NULL_PRESSURE",
		"MSET2_CORG_OUT_NULL_PRESSURE",
		"MSET2_CORH_OUT_NULL_PRESSURE",
		NULL
	};
	//	out speed for core
	char*	pszCoreOutSpeed[] =
	{
		"MSET_CORA_OUT_NULL_FLOW",
		"MSET_CORB_OUT_NULL_FLOW",
		"MSET_CORC_OUT_NULL_FLOW",
		"MSET_CORD_OUT_NULL_FLOW",
		"MSET_CORE_OUT_NULL_FLOW",
		"MSET_CORF_OUT_NULL_FLOW",
		"MSET2_CORG_OUT_NULL_FLOW",
		"MSET2_CORH_OUT_NULL_FLOW",
		NULL
	};
	//	out Time for core
	char*	pszCoreOutTime[] =
	{
		"MSET_CORA_OUT_NULL_TIME",
		"MSET_CORB_OUT_NULL_TIME",
		"MSET_CORC_OUT_NULL_TIME",
		"MSET_CORD_OUT_NULL_TIME",
		"MSET_CORE_OUT_NULL_TIME",
		"MSET_CORF_OUT_NULL_TIME",
		"MSET2_CORG_OUT_NULL_TIME",
		"MSET2_CORH_OUT_NULL_TIME",
		NULL
	};
	//	out count for core
	char*	pszCoreOutCount[] =
	{
		"MSET_CORA_OUT_NULL_COUNTER",
		"MSET_CORB_OUT_NULL_COUNTER",
		"MSET_CORC_OUT_NULL_COUNTER",
		"MSET_CORD_OUT_NULL_COUNTER",
		"MSET_CORE_OUT_NULL_COUNTER",
		"MSET_CORF_OUT_NULL_COUNTER",
		"MSET2_CORG_OUT_NULL_COUNTER",
		"MSET2_CORH_OUT_NULL_COUNTER",
		NULL
	};
	//	out act point for core
	char*	pszCoreOutActPoint[] =
	{
		"MSET_CORA_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORB_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORC_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORD_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORE_OUT_NULL_ACTPOSITIONOPTION",
		"MSET_CORF_OUT_NULL_ACTPOSITIONOPTION",
		"MSET2_CORG_OUT_NULL_ACTPOSITIONOPTION",
		"MSET2_CORH_OUT_NULL_ACTPOSITIONOPTION",
		NULL
	};
	//	out set mold's position for core
	char*	pszCoreOutSetPos[] =
	{
		"MSET_CORA_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORB_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORC_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORD_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORE_OUT_NULL_SETMOLDPOSITION",
		"MSET_CORF_OUT_NULL_SETMOLDPOSITION",
		"MSET2_CORG_OUT_NULL_SETMOLDPOSITION",
		"MSET2_CORH_OUT_NULL_SETMOLDPOSITION",
		NULL
	};
	
	char**	pszCore[] =
	{
		pszCoreFunction, 
		pszCoreInMode,
		pszCoreInPress,
		pszCoreInSpeed,
		pszCoreInTime,
		pszCoreInCount,
		pszCoreInActPoint,
		pszCoreInSetPos,
		pszCoreOutMode,
		pszCoreOutPress,
		pszCoreOutSpeed,
		pszCoreOutTime,
		pszCoreOutCount,
		pszCoreOutActPoint,
		pszCoreOutSetPos,
		NULL
	};
	InitData("MCONF_COREPULLER_COREPULLER_NULL_SET", pszCore, 0xFFFF);
	
	//wangli2014-3-6 10:06:31  中子進退兩段控製
	if(g_pExTable->GetTableValue(0,"D_CORE2SEC"))
	{
		//	in slow press for core
		char*	pszCoreInSlowPress[] =
		{
			"MSET_CORA_IN_SLOW_PRESSURE",
			"MSET_CORB_IN_SLOW_PRESSURE",
			"MSET_CORC_IN_SLOW_PRESSURE",
			"MSET_CORD_IN_SLOW_PRESSURE",
			"MSET_CORE_IN_SLOW_PRESSURE",
			"MSET_CORF_IN_SLOW_PRESSURE",
			NULL
		};
		//	in slow speed for core
		char*	pszCoreInSlowSpeed[] =
		{
			"MSET_CORA_IN_SLOW_FLOW",
			"MSET_CORB_IN_SLOW_FLOW",
			"MSET_CORC_IN_SLOW_FLOW",
			"MSET_CORD_IN_SLOW_FLOW",
			"MSET_CORE_IN_SLOW_FLOW",
			"MSET_CORF_IN_SLOW_FLOW",
			NULL
		};
		//	in slow time for core
		char*	pszCoreInSlowTime[] =
		{
			"MSET_CORA_IN_SLOW_TIME",
			"MSET_CORB_IN_SLOW_TIME",
			"MSET_CORC_IN_SLOW_TIME",
			"MSET_CORD_IN_SLOW_TIME",
			"MSET_CORE_IN_SLOW_TIME",
			"MSET_CORF_IN_SLOW_TIME",
			NULL
		};
		//	out slow press for core
		char*	pszCoreOutSlowPress[] =
		{
			"MSET_CORA_OUT_SLOW_PRESSURE",
			"MSET_CORB_OUT_SLOW_PRESSURE",
			"MSET_CORC_OUT_SLOW_PRESSURE",
			"MSET_CORD_OUT_SLOW_PRESSURE",
			"MSET_CORE_OUT_SLOW_PRESSURE",
			"MSET_CORF_OUT_SLOW_PRESSURE",
			NULL
		};
		//	out slow speed for core
		char*	pszCoreOutSlowSpeed[] =
		{
			"MSET_CORA_OUT_SLOW_FLOW",
			"MSET_CORB_OUT_SLOW_FLOW",
			"MSET_CORC_OUT_SLOW_FLOW",
			"MSET_CORD_OUT_SLOW_FLOW",
			"MSET_CORE_OUT_SLOW_FLOW",
			"MSET_CORF_OUT_SLOW_FLOW",
			NULL
		};
		//	out slow time for core
		char*	pszCoreOutSlowTime[] =
		{
			"MSET_CORA_OUT_SLOW_TIME",
			"MSET_CORB_OUT_SLOW_TIME",
			"MSET_CORC_OUT_SLOW_TIME",
			"MSET_CORD_OUT_SLOW_TIME",
			"MSET_CORE_OUT_SLOW_TIME",
			"MSET_CORF_OUT_SLOW_TIME",
			NULL
		};
		char**	pszCoreSlow[] =
		{
			pszCoreInSlowPress,
			pszCoreInSlowSpeed,
			pszCoreInSlowTime,
			pszCoreOutSlowPress,
			pszCoreOutSlowSpeed,
			pszCoreOutSlowTime,
			NULL
		};

		InitDataCoreSlow("MCONF_COREPULLER_COREPULLER_NULL_SET", pszCoreSlow, 0xFFFF);
	}

}

//	Close Inject
void	CtmInit::Init_CloseInject()
{
	
	char*	pszPress[] =
	{
		"MSET_SCRW_INJ_STEP1_PRESSURE",
		"MSET_SCRW_INJ_STEP2_PRESSURE",
		"MSET_SCRW_INJ_STEP3_PRESSURE",
		"MSET_SCRW_INJ_STEP4_PRESSURE",
		"MSET_SCRW_INJ_STEP5_PRESSURE",
		"MSET_SCRW_INJ_STEP6_PRESSURE",
		NULL
	};
	char*	pszSpeed[] =
	{
		"MSET_SCRW_HLDP_STEP1_FLOW",
		"MSET_SCRW_HLDP_STEP2_FLOW",
		"MSET_SCRW_HLDP_STEP3_FLOW",
		"MSET_SCRW_HLDP_STEP4_FLOW",
		"MSET_SCRW_HLDP_STEP5_FLOW",
		NULL
	};
	char**	pszCloseInject[] =
	{
		pszPress,
		//pszSpeed,
		NULL
	};
	char**	pszCloseInject1[] =
	{
		pszSpeed,
		NULL
	};
	
	long temp = GetDBValue("MCONF_SCREW_INJECT_NULL_CLOSELOOPOPTION").lValue;
	
	if (temp == 1)
	{
		InitData("MCONF_SCREW_INJECT_NULL_RANK", pszCloseInject, 
				(WORD)GetDBValue("MSET_SCRW_INJ_STEP1_PRESSURE").lValue);
		InitData("MCONF_SCREW_HOLDPRESSURE_NULL_RANK", pszCloseInject1, 
				(WORD)GetDBValue("MSET_SCRW_HLDP_STEP1_FLOW").lValue);		
	}
	
}


void	CtmInit::Init_AxialConfig(int nDealRamp)
{
	//=============
	//	close mold
	//=============
	//printf("init close mold\n");
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_AxialConfig\n");
	Init_CloseMoldConfig(nDealRamp);
	
	//=============
	//	open mold
	//=============	
	//printf("init open mold\n");
	Init_OpenMoldConfig(nDealRamp);
	
	//=============
	//	Inject
	//=============	
	//printf("init inject\n");
	Init_InjectConfig(nDealRamp);
	if(CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC)
	Init_InjectBConfig(nDealRamp);
	//=============
	//	Hold pressure
	//=============	
	//printf("init hold\n");
	Init_HoldConfig(nDealRamp);
	if(CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC)
	Init_HoldBConfig(nDealRamp);
	//=============
	//	charge
	//=============
	//printf("init charge\n");
	Init_ChargeConfig(nDealRamp);
	if(CtmConfig::GetInstance()->GetMachineType() & MACHINE_TYPE_OIL_DC)
	Init_ChargeBConfig(nDealRamp);	
	//Sunny<20070614> add
	//=============
	//	Eject
	//=============
	Init_EjtADVConfig(nDealRamp);
	Init_EjtRETConfig(nDealRamp);
	
	//=============
	//	air blast
	//=============
	//printf("init air\n");
	Init_AirConfig();
	
	//=============
	//	Core
	//=============
	//printf("init core\n");
	Init_CoreConfig();
	//=============
	//	KEB
	//=============
	//g_lKEBPulse		= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED61").lValue;
	//g_lKEBUM		= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED62").lValue;
	//g_lKEBRPM		= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED63").lValue;
	//g_lKEBMMPerSec	= GetDBValue("SYSX_OTHERS_OTHERS_NULL_RESERVED64").lValue;
}

void	CtmInit::Init_Mold()
{
	if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_MOLDSET_SRAMFILE))
		CtmSarmRecordFileControl::GetInstance()->DeleteRecord(ID_MOLDSET_SRAMFILE);
	
	tmStorageType	nStorageType = STORAGE_TYPE_FLASHEX;
   	if(GetFileType(CHECKFILETYPE))
    	nStorageType = STORAGE_TYPE_FLASH;
	#ifdef	D_NOWRITE_FALSH
		if (CtmMoldSet::GetInstance()->GetMoldSetNum(nStorageType) == 0)
	#else
		if (CtmMoldSet::GetInstance()->GetMoldSetNum() == 0)
	#endif
	{
		g_bFindMoldSetMsg = FALSE;
		CtmMoldSet::GetInstance()->SaveToCurrentMold();
		g_bFindMoldSetMsg = TRUE;
	}
}

void	CtmInit::Init_FlashEX()
{
	#ifdef	D_ARM
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_FlashEX\n");
	if (CtmConfig::GetInstance()->GetFlashEXInit() == 0)
	{
		CtmFlashEx::GetInstance()->Delete();
		CtmConfig::GetInstance()->SetFlashEXInit(1);
		
			CtmBlockControl::GetInstance()->SetBlockIndex(BLOCK_MACHINE_ID);
			CtmBlockControl::GetInstance()->SaveAs(STORAGE_TYPE_FLASHEX);
	}
	#endif
}

//	2007/11/19 03:56下午	fans add
void	CtmInit::Init_SramFromBack()
{
	g_tmDebugInfo->PrintDebugInfo("4CtmInit::Init_SramFromBack\n");
	int	 m_nBackHandle = -1;
	CtmSram*	pSram = CtmSram::GetInstance();	
	m_nBackHandle = open("/usr/Sram.bak", O_RDWR | O_CREAT);	
	if (m_nBackHandle >= 0)
	{
		BYTE*	pSramData = new BYTE[CtmConfig::GetInstance()->GetSramSize()];			
		memset(pSramData, 0, CtmConfig::GetInstance()->GetSramSize());				
		read(m_nBackHandle, pSramData, CtmConfig::GetInstance()->GetSramSize());		
		pSram->Write(0, CtmConfig::GetInstance()->GetSramSize(), pSramData);
		close(m_nBackHandle);
		delete []pSramData;
	}
}


void	CtmInit::SetInstallType() /*JOYCE2015-7-15 新停機功能flag*/
{	
	int	m_nInstallFlag = 0;
	int	nFd = open("/conf/installFlag", O_RDWR|O_CREAT);
	if (nFd >= 0)
	{
		read(nFd, &m_nInstallFlag, sizeof(m_nInstallFlag));
	}
 	close(nFd);
	printf("conrtolfunction   m_nInstallFlag=%d\n",m_nInstallFlag);

	if(m_nInstallFlag == 1)
	{	
		if(CtmConfig::GetInstance()->GetStopPWDType() != 1)
			CtmConfig::GetInstance()->SetStopPWDType(1);	
	}
}

void  CtmInit::OpenBattery_2() //JOYCE2011-1-19 
{
#ifdef  D_ARM
#ifdef D_PXA270
  g_nBatteryFd = open("/dev/rtc", O_RDWR | O_NOCTTY);
#else
  g_nBatteryFd = open(BATTERY_DEVICE, O_RDWR | O_NOCTTY);
#endif
#endif
}

void	InitDB2Def()
{
	g_TMDB2[0].data.lData = 0x123321;
	g_TMDB2[1].data.lData = 16;
	
	g_TMDB2[2].data.lData = 0;	
	g_TMDB2[3].data.lData = 0;
	g_TMDB2[4].data.lData = 0;
	g_TMDB2[5].data.lData = 0;
	g_TMDB2[6].data.lData = 0;
	g_TMDB2[7].data.lData = 0;
	g_TMDB2[8].data.lData = 0;
	g_TMDB2[9].data.lData = 0;
	g_TMDB2[10].data.lData = 0;
	g_TMDB2[11].data.lData = 0;
	g_TMDB2[12].data.lData = 0;
	g_TMDB2[13].data.lData = 0;
	g_TMDB2[14].data.lData = 0;
	g_TMDB2[15].data.lData = 0;
	g_TMDB2[16].data.lData = 0;
	g_TMDB2[17].data.lData = 0;
	g_TMDB2[18].data.lData = 0;
	g_TMDB2[19].data.lData = 0;
	g_TMDB2[20].data.lData = 0;
	g_TMDB2[21].data.lData = 0;
	g_TMDB2[22].data.lData = 0;	
	g_TMDB2[23].data.lData = 0;
	g_TMDB2[24].data.lData = 0;
	g_TMDB2[25].data.lData = 0;
	g_TMDB2[26].data.lData = 0;
	g_TMDB2[27].data.lData = 0;
	g_TMDB2[28].data.lData = 0;
	g_TMDB2[29].data.lData = 0;
	g_TMDB2[30].data.lData = 0;
	g_TMDB2[31].data.lData = 0;
	g_TMDB2[32].data.lData = 0;
	g_TMDB2[33].data.lData = 0;
	g_TMDB2[34].data.lData = 0;
	g_TMDB2[35].data.lData = 0;
	g_TMDB2[36].data.lData = 0;
	g_TMDB2[37].data.lData = 0;
	g_TMDB2[38].data.lData = 0;
	g_TMDB2[39].data.lData = 0;
	g_TMDB2[40].data.lData = 0;
	g_TMDB2[41].data.lData = 0;
	g_TMDB2[42].data.lData = 0;	
	g_TMDB2[43].data.lData = 0;
	g_TMDB2[44].data.lData = 0;
	g_TMDB2[45].data.lData = 0;
	g_TMDB2[46].data.lData = 0;
	g_TMDB2[47].data.lData = 0;
	g_TMDB2[48].data.lData = 0;
	g_TMDB2[49].data.lData = 0;
	g_TMDB2[50].data.lData = 0;
	g_TMDB2[51].data.lData = 0;
	g_TMDB2[52].data.lData = 0;
	g_TMDB2[53].data.lData = 0;
	g_TMDB2[54].data.lData = 0;
	g_TMDB2[55].data.lData = 0;
	g_TMDB2[56].data.lData = 0;
	g_TMDB2[57].data.lData = 0;
	g_TMDB2[58].data.lData = 0;
	g_TMDB2[59].data.lData = 0;
	g_TMDB2[60].data.lData = 0;
	g_TMDB2[61].data.lData = 0;
	g_TMDB2[62].data.lData = 0;
	g_TMDB2[63].data.lData = 0;
	g_TMDB2[64].data.lData = 0;
	g_TMDB2[65].data.lData = 0;
	g_TMDB2[66].data.lData = 0;
	g_TMDB2[67].data.lData = 0;
	g_TMDB2[68].data.lData = 0;
	g_TMDB2[69].data.lData = 0;
	g_TMDB2[70].data.lData = 0;
	g_TMDB2[71].data.lData = 0;
	g_TMDB2[72].data.lData = 0;
	g_TMDB2[73].data.lData = 0;
	g_TMDB2[74].data.lData = 0;
	g_TMDB2[75].data.lData = 0;
	g_TMDB2[76].data.lData = 0;
	g_TMDB2[77].data.lData = 0;
	g_TMDB2[78].data.lData = 0;
	g_TMDB2[79].data.lData = 0;
	g_TMDB2[80].data.lData = 0;
	g_TMDB2[81].data.lData = 90;			//lcd backlight count  James add 2015/4/22 14:47:58
	
	g_TMDB2[82].data.fData = 0.0;
	g_TMDB2[83].data.fData = 0.0;
	g_TMDB2[84].data.fData = 0.0;
	g_TMDB2[85].data.fData = 0.0;
	g_TMDB2[86].data.fData = 0.0;
	g_TMDB2[87].data.fData = 0.0;
	g_TMDB2[88].data.fData = 0.0;
	g_TMDB2[89].data.fData = 0.0;
	g_TMDB2[90].data.fData = 0.0;
	g_TMDB2[91].data.fData = 0.0;
	g_TMDB2[92].data.fData = 0.00;
	g_TMDB2[93].data.fData = 0.00;
	g_TMDB2[94].data.fData = 0.00;
	g_TMDB2[95].data.fData = 0.00;
	g_TMDB2[96].data.fData = 0.00;
	g_TMDB2[97].data.fData = 0.00;
	g_TMDB2[98].data.fData = 0.00;
	g_TMDB2[99].data.fData = 0.00;
	g_TMDB2[100].data.fData = 0.00;
	g_TMDB2[101].data.fData = 0.00;	
}

struct lcd_backlight
{
	unsigned int pre_scaler;
	unsigned int period;
	unsigned int count;
};

void		LCDPowerOn_3354(int nCount)
{
	
	struct lcd_backlight	param;
    param.pre_scaler 	= 0;
    param.period 		= 100;
    if(nCount <=50)
    	param.count 		= 50;
    else
    	param.count 		= nCount;
    	
	if (_fbData.nfb == -1)
		_fbData.nfb	= open("/dev/fb0", O_RDWR, 0);
		
    if (ioctl(_fbData.nfb, 0x40044632, &param) == -1) 
    {
		;//perror("Error Setting backlight param");
    }
}


#ifndef	D_BEOBJECT_CTMINIT
CObject*	CreateInstance(char* Name)
{
		g_tmDebugInfo->PrintDebugInfo("CtmInit::CreateInstance\n");
	if (g_pInit == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CInit*>(pResult))->CreateSelf();
		g_pInit = static_cast<CInit*>(pResult);
	}
	return g_pInit;
}

void	ReleaseInstance(CObject* pInstance)
{
	g_tmDebugInfo->PrintDebugInfo("CtmInit::ReleaseInstance\n");
	if (pInstance != NULL)
		(static_cast<CInit*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
