#include	"../../package.h"
#include	"../../timer.h"
#include    "../../_kb.h"
#include	"../../main.h"
#include	"../../utils.h"
#include	"../../taskhost.h"
#include    "../../taskdemo.h"
#include	"tmTaskDataProcess.h"
#include 	"../../tmshmsg.h"
#include	"../libDataSave.h"
#include	"../../commonaction.h"
#include 	"../../qualitycontrol.h"
#include	"../../filerecord.h"

#ifdef D_PXA255
	#define	DEST_DIR			"/mnt/mmc"
#else
	#ifdef D_PXA270
			#ifdef	D_3354
				#define	DEST_DIR		"/media/sda1"  //JOYCE2012-4-26
			#else
				#define	DEST_DIR		"/mnt/usb"  //JOYCE2012-4-26
			#endif	
	#else
		#define	DEST_DIR		"/mnt/usb"
	#endif
#endif

#define		FILENAME_MONI		"Moni"
#define		FILENAME_END		".cdb"


#define		YEAR_TO_MIN		360
#define		YEAR_TO_HOUR


#define		DATAPROCESS_HEAD_TYPE_POS		4
#define		DATAPROCESS_HEAD_MODE_POS		8
#define		DATAPROCESS_HEAD_COUNT_POS		12
#define		DATAPROCESS_HEAD_SIZE			16

		CIniFile	m_IniDataFile;
#define		DATAPROCESS_STATEMODE_MAXCOUNT	10000
static int month[12] = {
	0,
	DAY*(31), 
	DAY*(31+28),
	DAY*(31+28+31),
	DAY*(31+28+31+30),
	DAY*(31+28+31+30+31),
	DAY*(31+28+31+30+31+30),
	DAY*(31+28+31+30+31+30+31),
	DAY*(31+28+31+30+31+30+31+31),
	DAY*(31+28+31+30+31+30+31+31+30),
	DAY*(31+28+31+30+31+30+31+31+30+31),
	DAY*(31+28+31+30+31+30+31+31+30+31+30)
};
int		u_DataMode[] = 
{
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_1,
	DATAPROCESS_MODE_1,
	DATAPROCESS_MODE_1,
	DATAPROCESS_MODE_1,
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_2,
	DATAPROCESS_MODE_2	
};

#define		DATAPROCESS_STATEMODE_TIMEAUTO			0x01
#define		DATAPROCESS_STATEMODE_SEMIAUTO			0x02
#define		DATAPROCESS_STATEMODE_MANUAL			0x03
#define		DATAPROCESS_STATEMODE_HANDMOLDADJ		0x04
#define		DATAPROCESS_STATEMODE_AUTOMOLDADJ		0x05
#define		DATAPROCESS_STATEMODE_SENSORAUTO		0x06
#define		DATAPROCESS_STATEMODE_MOTORON			0x07
#define		DATAPROCESS_STATEMODE_MOTOROFF			0x08
#define		DATAPROCESS_STATEMODE_HEATERON			0x09
#define		DATAPROCESS_STATEMODE_HEATEROFF			0x0A

int		u_DataState[] = 
{
	DATAPROCESS_STATEMODE_MANUAL,			//1 手動
	-1,										//2 自動
	DATAPROCESS_STATEMODE_SEMIAUTO,			//3 半自動
	DATAPROCESS_STATEMODE_SENSORAUTO,		//4 電眼自動
	DATAPROCESS_STATEMODE_TIMEAUTO,			//5 時間自動
	DATAPROCESS_STATEMODE_HANDMOLDADJ,		//6 粗調模
	-1,		//7 
	-1,		//8
	-1,		//9 自動清料
	-1,		//10
	-1,		//11
	-1,		//12
	DATAPROCESS_STATEMODE_AUTOMOLDADJ,		//13 自動調模
	DATAPROCESS_STATEMODE_AUTOMOLDADJ,		//14 自動調模
	DATAPROCESS_STATEMODE_AUTOMOLDADJ,		//15 自動調模
	DATAPROCESS_STATEMODE_AUTOMOLDADJ,		//16 自動調模
	DATAPROCESS_STATEMODE_AUTOMOLDADJ		//17 自動調模
	
};

IMPLEMENT_DYNCREATE(CtmDataProcessSave, CDataProcessSave)
//IMPLEMENT_DYNCREATE(CtmDataProcessRead, CObject)
IMPLEMENT_DYNCREATE(CtmTaskDataProcess, CTaskDataProcess)
BOOL	CtmTaskDataProcess::m_bPowerOnFlag = TRUE;
/*---------------------------------------------------------------------------+
|  Function : CtmTaskDataProcess::CtmTaskDataProcess()              	     |
|  Task     : 構造函數  						     	             		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmTaskDataProcess::CtmTaskDataProcess(): CTaskDataProcess()
{
	//RegisterMessage(MSG_SH_TASKDATA, m_idMe);
	m_lEnergyTimer_Const = 60*60*1000;
	AddTimer(&m_lEnergyTimer_Const);
	//CTaskDataProcess::CTaskDataProcess();
}

/*---------------------------------------------------------------------------+
|  Function : CtmTaskDataProcess::~CtmTaskDataProcess()              	     |
|  Task     : 析構函數  						     	             		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmTaskDataProcess::~CtmTaskDataProcess()
{
	//UnRegisterAllMessage(m_idMe);
	DeleteTimer(&m_lEnergyTimer_Const);
}

/*---------------------------------------------------------------------------+
|  Function : int		CtmTaskDataProcess::CreateSelf()        	     	 |
|  Task     : 創建函數  						     	             		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::CreateSelf()
{
	CtmStorageFile*	tm_pUsb	= CtmUsbFile::GetInstance();/*JOYCE2012-4-26 開機時"USB監測資料"已經啟動,則掛載USB*/

	m_lCounter = 0;
	m_nlibConfigCount = 0;
	m_lSaveEnergyMode = 1;
	CTaskDataProcess::CreateSelf();
	AddCounter(&m_lCounter, m_idMe);
	
	strcpy(m_cszlibConfig_Path, "DataPath0");
	memset(m_pConfig, 0 , sizeof(m_pConfig));				//init  2013-6-5
	memset(m_pConfigValue, 0 , sizeof(m_pConfigValue));		//init 
	Init();
	Init_DataEnergy();
	Init_DataState();
	Init_DataPowerOnOff();
	Init_OnOffState();
	m_bCapacitySaveFlag = TRUE;
	
	RegisterMessage(MSG_SH_DATA_MODE_MANUAL, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_SEMIAUTO, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_SENSORAUTO, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_TIMEAUTO, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_HANDMOLDADJ, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_AUTOMOLDADJ, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_HEATERON, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_HEATEROFF, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_MOTORON, m_idMe);
	RegisterMessage(MSG_SH_DATA_MODE_MOTOROFF, m_idMe);
	RegisterMessage(MSG_DSP54_GET_SHOTCOUNT, m_idMe);
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function : int		CtmTaskDataProcess::FreeSelf()        	     	 	 |
|  Task     : 銷毀函數  						     	             		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::FreeSelf()
{
	DeleteCounter(&m_lCounter, m_idMe);
	
	UnRegisterMessage(MSG_SH_DATA_MODE_MANUAL, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_SEMIAUTO, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_SENSORAUTO, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_TIMEAUTO, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_HANDMOLDADJ, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_AUTOMOLDADJ, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_HEATERON, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_HEATEROFF, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_MOTORON, m_idMe);
	UnRegisterMessage(MSG_SH_DATA_MODE_MOTOROFF, m_idMe);
	UnRegisterMessage(MSG_DSP54_GET_SHOTCOUNT, m_idMe);
	
	return CTaskDataProcess::FreeSelf();
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTaskDataProcess::WndProc(int message, WPARAM wParam, LPARAM lParam) |
|  Task     : 消息處理函數 						     	             		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:   message               -          消息ID                      |
| 				wParam                -          參數1                       |
|  				lParam                -          參數2                       |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmTaskDataProcess::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	CTask::WndProc(message, wParam, lParam);
	//printf("message =%x \n", message);
	DataProcess_Mode(message, wParam, lParam);
	switch (message)
	{
		case MSG_DSP54_GET_SHOTCOUNT://JOYCE2012-4-26
			if(wParam == 0)
			{
				SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED116", GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED116").lValue+1);
			//	printf("nCount = %d\n",  GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED116").lValue);	
			}
			break;
		
		default:
			break;
	}
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTaskDataProcess::Run()       	     	 	 		 |
|  Task     : run函數  						     	             		     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmTaskDataProcess::Run()
{
	ProcessEnergy();
	ProcessPowerOnOff();
	ProcessState();
	ProcessMoniOnline();//JOYCE2012-4-26 
	ProcessOnOffState();// James add 2014/11/11 
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTaskDataProcess::Init()       	     	 	 		 |
|  Task     : run函數  						     	             		     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::Init()
{
	char	cszDataPath[256], cszDataConfigPath[256], cszDataFile_Config[256], cszFileName[256], cszTempName[256];
	
	memset(cszDataPath, 		0, sizeof(cszDataPath));
	memset(cszDataConfigPath, 	0, sizeof(cszDataConfigPath));
	memset(cszFileName, 		0, sizeof(cszFileName));
	memset(cszTempName, 		0, sizeof(cszTempName));
	
	//bool bOpen = 
	m_IniDataFile.OpenFile("./lib/configlib_data.ini");
//	if (bOpen)
//		printf("Open Configlib.ini is OK!\n");
	
	m_nlibConfigCount = m_IniDataFile.ReadSection("datalib", m_pConfig, 0);
	m_IniDataFile.ReadSectionValues("datalib", m_pConfigValue, 0);
	
	strcpy(cszFileName, GetlibConfig(m_cszlibConfig_Path));
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_1"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_1"));
	//printf("count=%d cszFileName=%s \n", m_nlibConfigCount, cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetDataFileName_Config(cszDataFile_Config);
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_1"));
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_ERROR-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_2"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_2"));
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_2"));
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_MONITOR-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_3"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_3"));
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_3"));
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_TEMPER-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_4"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_4"));
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_4"));
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_5"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_5"));
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_5"));
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_ENERGY-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_6"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_6"));
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_6"));
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_SPC-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_7"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_7"));
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_7"));
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_ALARM-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_8"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_8"));
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_8"));
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_HARDINFO-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_9"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_9"));
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_9"));
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_USERINFO-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_10"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_10"));
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_10"));
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_MOLDSET-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_11"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_11"));
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_11"));
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_RECORD-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	strcpy(cszDataConfigPath, GetlibConfig("DataConfigPath_12"));
	strcpy(cszDataFile_Config, GetlibConfig("DataFile_12"));
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetDataPath(cszFileName);
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetDataConfigPath(cszDataConfigPath);
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetDataFileName_Config(cszDataFile_Config);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, GetlibConfig("DataConfig_12"));
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetConfigName(cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszFileName, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetDataFileName(cszTempName);
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[DATAPROCESS_TYPE_PRODUCTPLAN-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);

	m_IniDataFile.CloseFile();
	return 0;
}

int		CtmTaskDataProcess::SetDataPath(int nType, int	nPathFlag)
{
	char	cszDataPath[256], cszDataConfigPath[256], cszDataFile_Config[256], cszFileName[256], cszTempName[256];
	
	memset(cszTempName,			0, sizeof(cszTempName));
	memset(cszDataPath, 		0, sizeof(cszDataPath));
	memset(cszDataConfigPath, 	0, sizeof(cszDataConfigPath));
	memset(cszFileName, 		0, sizeof(cszFileName));
	memset(cszTempName, 		0, sizeof(cszTempName));
	
//	bool bOpen = 
	m_IniDataFile.OpenFile("./lib/configlib_data.ini");
//	if (bOpen)
//		printf("Open Configlib.ini is OK!\n");
	
	m_nlibConfigCount = m_IniDataFile.ReadSection("datalib", m_pConfig, 0);
	m_IniDataFile.ReadSectionValues("datalib", m_pConfigValue, 0);
	
	sprintf(cszTempName, "%s%d", "DataPath", nPathFlag);
	strcpy(cszDataPath, GetlibConfig(cszTempName));
	printf("cszDataPath=%s \n", cszDataPath);
	
	memset(cszTempName,			0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%d", "DataConfigPath_", nType);
	strcpy(cszDataConfigPath, GetlibConfig(cszTempName));
	printf("cszDataConfigPath=%s \n", cszDataConfigPath);
	
	memset(cszTempName,			0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%d", "DataConfig_", nType);
	strcpy(cszDataFile_Config, GetlibConfig(cszTempName));
	printf("cszDataFile_Config=%s \n", cszDataFile_Config);
	
	memset(cszTempName,			0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%d", "DataFile_", nType);
	strcpy(cszFileName, GetlibConfig(cszTempName));
	printf("cszFileName=%s \n", cszFileName);
	
	//printf("count=%d cszFileName=%s \n", m_nlibConfigCount, cszFileName);
	m_DataProcess[nType-1].SetDataPath(cszDataPath);
	m_DataProcess[nType-1].SetDataConfigPath(cszDataConfigPath);
	
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszDataPath, cszDataConfigPath, cszDataFile_Config);
	m_DataProcess[nType-1].SetConfigName(cszTempName);
	printf("ConfigName=%s \n", cszTempName);
	memset(cszTempName, 0, sizeof(cszTempName));
	sprintf(cszTempName, "%s%s%s", cszDataPath, cszDataConfigPath, cszFileName);
	m_DataProcess[nType-1].SetDataFileName_Config(cszTempName);
	
	printf("FileName_config=%s \n", cszTempName);
	m_DataProcess[nType-1].SetFileMaxCount(DATAPROCESS_FILE_MAXCOUNT);
	m_DataProcess[nType-1].SetMaxCount(DATAPROCESS_FILEDATA_MAXCOUNT);
	
	
	m_IniDataFile.CloseFile();
	//Init();
	return 0;
}

char*	CtmTaskDataProcess::GetlibConfig(char*	pName)
{
	//printf("m_nlibConfigCount=%d pName=%s \n", m_nlibConfigCount, pName);
	for(unsigned int i = 0; i < m_nlibConfigCount; i++)
	{
		//printf("i=%d, m_pConfig=%s \n", i, m_pConfig[i]);
		if(m_pConfig[i] != NULL)
		{
			if(strcmp(pName, m_pConfig[i]) == 0)
			{
				//printf("m_pConfigValue[i]=%s \n", m_pConfigValue[i]);
				return 	m_pConfigValue[i];
			}
		}
	}
	return NULL;
}

/*---------------------------------------------------------------------------+
|  Function : int	GetData(int	nType, int nIndex, int nSize,void* pData)    |
|  Task     : 讀取指定類型資料  						     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
| 				    nIndex            -     資料序號                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::GetData(int		nType, 	int nIndex, 	int nSize, 	void* pData)
{
	int	nResult = 0;
	if(nType <= DATAPROCESS_TYPE_MAXCOUNT)
	{
		nResult = m_DataProcess[nType-1].Read(nType, nIndex, nSize, pData);
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : int	GetDataS(int nType,int	nStartIndex,int nIndex, int nSize, void* pData)|
|  Task     : 讀取指定類型和指定個數資料  						     	     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
|  					nStartIndex           - 起始Index                        |
| 				    nCount            -     資料筆數                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::GetDataS(int	nType,	int	nStartIndex,int nCount, int nSize, 	void* pData)
{
	int	nResult = 0;
	if(nType <= DATAPROCESS_TYPE_MAXCOUNT)
	{
		nResult = m_DataProcess[nType-1].ReadS(nType, nStartIndex, nCount, nSize, pData);
	}
	return nResult;
}


/*---------------------------------------------------------------------------+
|  Function : int	SetData(int nType,int nIndex, int nSize, void* pData)    |
|  Task     : 設定指定類型資料  						     	     		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
| 				    nIndex            -     資料序號                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::SetData(int		nType,	int	nIndex, 	int nSize, 	void* pData)
{
	int	nResult = 0;
	
	if(nType <= DATAPROCESS_TYPE_MAXCOUNT)
	{
		nResult = m_DataProcess[nType-1].Write(nType, nIndex, nSize, pData);
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : int	SetDataS(int nType,int	nStartIndex,int nIndex, int nSize, void* pData)   |
|  Task     : 設定指定類型和指定筆數資料  						     	     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
|  					nStartIndex           - 起始Index                        |
| 				    nCount            -     資料筆數                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::SetDataS(int	nType,	int	nStartIndex,int nCount, int nSize,	void* pData)
{
	int	nResult = 0;
	if(nType <= DATAPROCESS_TYPE_MAXCOUNT)
	{
		nResult = m_DataProcess[nType-1].WriteS(nType, nStartIndex, nCount, nSize, pData);
	}
	return nResult;	
}

/*---------------------------------------------------------------------------+
|  Function : void	CtmTaskDataProcess::DataProcess_Mode(int message, WPARAM wParam, LPARAM lParam) |
|  Task     : 處理狀態資料  						     	                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       message         -     資料類型                           |
|  					nStartIndex           - 起始Index                        |
| 				    nCount            -     資料筆數                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	CtmTaskDataProcess::DataProcess_Mode(int message, WPARAM wParam, LPARAM lParam)
{
	STATEMODE	StateMode;
	BOOL		bFlag = FALSE;
	switch(message)
	{
		case MSG_SH_DATA_MODE_MANUAL:			//手動
			StateMode.nMode = DATAPROCESS_STATEMODE_MANUAL;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_SEMIAUTO:			//半自動
			StateMode.nMode = DATAPROCESS_STATEMODE_SEMIAUTO;
			bFlag = TRUE;
			break;	
		case MSG_SH_DATA_MODE_SENSORAUTO:		//電眼自動
			StateMode.nMode = DATAPROCESS_STATEMODE_SENSORAUTO;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_TIMEAUTO:			//全自動
			StateMode.nMode = DATAPROCESS_STATEMODE_TIMEAUTO;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_HANDMOLDADJ:		//手動調模
			StateMode.nMode = DATAPROCESS_STATEMODE_HANDMOLDADJ;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_AUTOMOLDADJ:		//自動調模
			StateMode.nMode = DATAPROCESS_STATEMODE_AUTOMOLDADJ;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_HEATERON:			//電熱開
			StateMode.nMode = DATAPROCESS_STATEMODE_HEATERON;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_HEATEROFF:		//電熱關
			StateMode.nMode = DATAPROCESS_STATEMODE_HEATEROFF;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_MOTORON:			//馬達開
			StateMode.nMode = DATAPROCESS_STATEMODE_MOTORON;
			bFlag = TRUE;
			break;
		case MSG_SH_DATA_MODE_MOTOROFF:			//馬達關
			StateMode.nMode = DATAPROCESS_STATEMODE_MOTOROFF;
			bFlag = TRUE;
			break;
		default:
			break;
	}
	if(bFlag)
	{
		if(m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].GetAllCount() > DATAPROCESS_STATEMODE_MAXCOUNT)
		{
			printf("save statemode outof range \n");
			return;	
		}
		GetDate(&StateMode.date);
		GetTime(&StateMode.time);
		//printf("GetManual Message %d / %d /%d: %d / %d /%d size=%d \n", StateMode.date.month, StateMode.date.year, StateMode.date.day, StateMode.time.hour, StateMode.time.minute, StateMode.time.second, sizeof(STATEMODE));
		if(m_DataProcess[DATAPROCESS_TYPE_OPSTATE-1].Write(DATAPROCESS_TYPE_OPSTATE, 0, sizeof(STATEMODE), (void*)&StateMode) <= 0)	//寫入失敗
		{
			printf("Save Data Error \n");
			SendMsg(MSG_SH_DATA_DATAPROCESS_ERROR, 0, 0, NULL);
		}
	}
}


/*---------------------------------------------------------------------------+
//處理方式：只有存儲到本機才這樣處理，
//1. 重新讀取計劃單，則重新開始保存。
//2. 手動可設置開始或停止保存。
//3. 
+---------------------------------------------------------------------------*/
int		CtmTaskDataProcess::ProcessEnergy()
{
	static	DWORD	dwMoldCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	DWORD	dwRealMoldCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	tmDATETIME	datetime;
	
	GetTime(&datetime.time);
	static	BYTE	bHour = datetime.time.hour;
	BYTE			bRealHour = datetime.time.hour;
	//printf("dwMoldCount=%d realcount=%d \n", dwMoldCount, dwRealMoldCount);
	if(dwMoldCount != dwRealMoldCount && m_bCapacitySaveFlag)
	{
		if(m_tmCapacityIndex.nCount == 0)		//若總數是0，則表示重新存儲
		{
			//printf("Save \n");
			m_tmCapacityIndex.nIndex = 1;
			m_tmCapacityIndex.nCount = 1;
			SaveEnergy(1, m_tmCapacityIndex.nIndex);
		}
		else
		{
			switch(m_lSaveEnergyMode)
			{
				case 0:							//固定時間存儲
					if(m_lEnergyTimer_Const <= 0)				//若時間小於0，則保存當前記錄
					{
						if (m_tmCapacityIndex.nCount < CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nCount++;
						}
						if ((++m_tmCapacityIndex.nIndex) >= CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nIndex = 0;
						}
						//printf(" m_tmCapacityIndex.nIndex=%d \n",  m_tmCapacityIndex.nIndex);
						SaveEnergy(1, m_tmCapacityIndex.nIndex);
						m_lEnergyTimer_Const = 60*60*1000;
					}
					else
					{
						SaveEnergy(0, m_tmCapacityIndex.nIndex);
					}
					break;
				case 1:							//整點時間存儲
					//printf("bHour=%d bRealHour=%d\n", bHour, bRealHour);
					if(bHour != bRealHour)
					{
						if (m_tmCapacityIndex.nCount < CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nCount++;
						}
						if ((++m_tmCapacityIndex.nIndex) >= CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nIndex = 0;
						}
						//printf(" 2 m_tmCapacityIndex.nIndex=%d \n",  m_tmCapacityIndex.nIndex);
						SaveEnergy(1, m_tmCapacityIndex.nIndex);
						bHour = bRealHour;	
					}
					else
					{
						SaveEnergy(0, m_tmCapacityIndex.nIndex);
					}
					break;
				case 2:							//開模數歸零存儲
					if(dwRealMoldCount != 0)				//當開模數有變化，則寫入數據
					{
						SaveEnergy(0, m_tmCapacityIndex.nIndex);
					}
					else									//若為0，則開模數清0，需要將nCount+1
					{
						if (m_tmCapacityIndex.nCount < CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nCount++;
						}
						if ((++m_tmCapacityIndex.nIndex) >= CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nIndex = 0;
						}
						SaveEnergy(1, m_tmCapacityIndex.nIndex);
					}
					break;
				default:
					if(dwRealMoldCount != 0)				//當開模數有變化，則寫入數據
					{
						SaveEnergy(0, m_tmCapacityIndex.nIndex);
					}
					else									//若為0，則開模數清0，需要將nCount+1
					{
						if (m_tmCapacityIndex.nCount < CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nCount++;
						}
						if ((++m_tmCapacityIndex.nIndex) >= CtmConfig::GetInstance()->GetDataEnergyMaxNum()) 
						{
							m_tmCapacityIndex.nIndex = 0;
						}
						SaveEnergy(1, m_tmCapacityIndex.nIndex);
					}
					break;
			}
		}
		dwMoldCount = dwRealMoldCount;
	}
	
	return 0;
}

int		CtmTaskDataProcess::ProcessPowerOnOff()
{
	//printf("m_bPowerOnFlag=%d \n", m_bPowerOnFlag);
	if(m_bPowerOnFlag == TRUE)
	{
		TMPOWERONOFF	tmPowerOnOff;
		m_bPowerOnFlag = FALSE;
		
		if (m_tmPowerOnOffIndex.nIndex > 0) 
		{	
			if(CtmConfig::GetInstance()->GetRecordType() == 1)
				CManageFile::GetInstance()->ReadRecord(FILE_RECORD_POWERONOFF, m_tmPowerOnOffIndex.nIndex, &tmPowerOnOff, sizeof(tmPowerOnOff));
			else
				CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPOWERONOFF_SRAMFILE, m_tmPowerOnOffIndex.nIndex, &tmPowerOnOff);
			
			STATEMODE	tmStateMode;
	
			tmStateMode.nMode = DATAPROCESS_STATEMODE_POWEROFF;
			memcpy(&tmStateMode.date, &tmPowerOnOff.powerofftime.date, sizeof(tmDATE));
			memcpy(&tmStateMode.time, &tmPowerOnOff.powerofftime.time, sizeof(tmTIME));
			//GetDate(&tmStateMode.date);
			//GetTime(&tmStateMode.time);
			tmStateMode.dwOpenMoldCount =  GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue + 1;/*wangli2016-1-22 10:58:30 事件畫面的開模序號從1開始*/
			if(g_pDatabase != NULL)
				memcpy(tmStateMode.cszUserID, g_pDatabase->Read("SYS_HMI_NULL_NULL_USERCURRENTID").pcData, 7);

			if (m_StateModeIndex.nIndex >= CtmConfig::GetInstance()->GetDataStateMaxNum()) 
			{
				m_StateModeIndex.nIndex = 0;
			}
			//printf("m_StateModeIndex.nIndex=%d count=%d \n", m_StateModeIndex.nIndex, m_StateModeIndex.nCount);
			if(CtmConfig::GetInstance()->GetRecordType() == 1)
				CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, m_StateModeIndex.nIndex + 1, &tmStateMode, sizeof(tmStateMode));
			else
				CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, m_StateModeIndex.nIndex + 1, &tmStateMode);
	
			if (m_StateModeIndex.nCount < CtmConfig::GetInstance()->GetDataStateMaxNum()) 
			{
				m_StateModeIndex.nCount++;
			}
			if ((++m_StateModeIndex.nIndex) >= CtmConfig::GetInstance()->GetDataStateMaxNum()) 
			{
				m_StateModeIndex.nIndex = 0;
			}
			if(CtmConfig::GetInstance()->GetRecordType() == 1)
			{
				tmFileRecordHeadInfo	HeadInfo;
				memset(&HeadInfo, 0, sizeof(HeadInfo));
				HeadInfo.nCount = m_StateModeIndex.nCount;
				HeadInfo.nIndex = m_StateModeIndex.nIndex;
				CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));
			}
			else
				CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, 0, &m_StateModeIndex);
		}
		SaveState(DATAPROCESS_STATEMODE_POWERON);
		SavePowerOnOff(0);
	}
	else
	{
		SavePowerOnOff(1);
	}
}

int		CtmTaskDataProcess::ProcessState()
{
	static	WORD	wOPState = _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) + 1; /*JOYCE2012-6-19 過濾高4個bit-非動作狀態*/
	WORD	wRealOPState = _ScanBitR((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF) + 1;     /*JOYCE2012-6-19 過濾高4個bit-非動作狀態*/
	
	static	WORD	wMotor = g_flMotorStatus;
	WORD	wRealMotor = g_flMotorStatus;
	
	static	WORD	wHeater = g_flHeaterStatus;
	WORD	wRealHeater = g_flHeaterStatus;
	
	//Op Mode save
	if(wOPState != wRealOPState)
	{
		if(wRealOPState == 1)
		{
			for(int j = wRealOPState; j < 16; j++) 
			{
				if( _TestBit((WORD)GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue & 0x0FFF,j))	/*JOYCE2012-6-19 過濾高4個bit-非動作狀態*/
				{
					wRealOPState = j+1;
					break;
				}
			}
		}
		else if(wRealOPState == 0xFFFF) 	wRealOPState = 0;
			
		if (wRealOPState < sizeof(u_DataState) / sizeof(int))
		{
			//printf("taskstate wRealOPState=%d \n", wRealOPState);
			if(u_DataState[wRealOPState] != -1)
				SaveState(u_DataState[wRealOPState]);
		}
		wOPState = wRealOPState;
	}
	
	//Motor save
	if(wMotor != wRealMotor)
	{
		if(wRealMotor == 0xFFFF)
			SaveState(DATAPROCESS_STATEMODE_MOTORON);
		else	if(wMotor == 0xFFFF) //old is on
			SaveState(DATAPROCESS_STATEMODE_MOTOROFF);
			
		wMotor = wRealMotor;
	}
	//Heater save
	if(wHeater != wRealHeater)
	{
		if(wRealHeater == 0xFFFF)
			SaveState(DATAPROCESS_STATEMODE_HEATERON);
		else
			SaveState(DATAPROCESS_STATEMODE_HEATEROFF);
		wHeater = 	wRealHeater;
	}
	
}

int		CtmTaskDataProcess::ProcessMoniOnline()//JOYCE2012-4-26
{
	BOOL	bMoniOnlineFlag = GetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED34").lValue;	/*此處Flag為是否啟用此功能*/
	WORD 	wCount =  GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED116").lValue;			/*此處為記錄當前開模累加*/

	//printf("CtmTaskDataProcess::g_bAutoHotPlug=%d\n",g_bAutoHotPlug);
	if(bMoniOnlineFlag)
	{
		//printf(" online flag \n");
	  g_bAutoHotPlug = TRUE;//mmi use
		
		if(g_wUSBStatus)
			m_bMoniActFlag = TRUE;

		if(wCount >= CtmConfig::GetInstance()->GetMoniMaxNum() && g_wUSBStatus)
		{
			ProcessMoniSave();
		}
	}
	else
	{
		if((m_bMoniActFlag)&&(g_wUSBStatus))/*選擇停止*/
			{
				m_bMoniActFlag = FALSE;
				ProcessMoniSave();
				//printf("CtmTaskDataProcess::UMount\n");
				g_bAutoHotPlug = FALSE;	
				CtmUsbFile::GetInstance()->UMount();
			}
		
	}

//printf("bMoniOnlineFlag =%d\n",bMoniOnlineFlag);
//printf("m_bMoniActFlag =%d\n",m_bMoniActFlag);
//printf("g_wUSBStatus =%d\n",g_wUSBStatus);
	if(bMoniOnlineFlag && m_bMoniActFlag && !g_wUSBStatus)/*不選擇停止,直接拔除USB*/
	{
		//printf("CtmTaskDataProcess::no usb\n");
		SetDBValue("MCONF_OTHERS_OTHERS_NULL_RESERVED34", 0);
		m_bMoniActFlag = FALSE;
		g_bAutoHotPlug = FALSE;	
	}
}

int		CtmTaskDataProcess::ProcessOnOffState()
{
	//dsp55 onoff line
	static	BOOL		nDsp55State = 0, nDsp55OldState = 0;
	static	BOOL		nDsp28State = 0, nDsp28OldState = 0;
	static	BOOL		nRmtp12State = 0, nRmtp12OldState = 0;
	static	BOOL		ntmIoTState = 0, ntmIoTOldState = 0;
	WORD	wSubIndex = 0;
	
	//Dsp55 OnOff Line
	nDsp55State = g_wDspStatus;
	if(nDsp55OldState != nDsp55State)
	{
		if(nDsp55State == TRUE)
		{
			SaveOnOffState(DATAPROCESS_ONFFDEVICE_55, 0, DATAPROCESS_ONFFSTATE_ON);
		}
		else
		{
			SaveOnOffState(DATAPROCESS_ONFFDEVICE_55, 0, DATAPROCESS_ONFFSTATE_OFF);
		}
		nDsp55OldState = nDsp55State;
	}
	
	//dsp28 onoff line
	if(g_ptaskdsp28_M3 != NULL)
		nDsp28State = !g_ptaskdsp28_M3->GetOnLineStatus();

	if(nDsp28OldState != nDsp28State)
	{
		if(nDsp28State == TRUE)
		{
			SaveOnOffState(DATAPROCESS_ONFFDEVICE_28, 0, DATAPROCESS_ONFFSTATE_ON);
		}
		else
		{
			SaveOnOffState(DATAPROCESS_ONFFDEVICE_28, 0, DATAPROCESS_ONFFSTATE_OFF);
		}
		nDsp28OldState = nDsp28State;
	}
	
	//rmtp12 onoff line
	nRmtp12State = GetDBValue("M3_STATE_TEMPERCARD1_LINESTATE").lValue;
	
	for(int i=0; i<8; i++) /*最多8張溫度卡*/
	{
		if(((nRmtp12OldState >> i)&0x0001) != ((nRmtp12State >> i)&0x0001))
		{
			wSubIndex = i;
			if((nRmtp12State >> i)&0x0001)
			{
				SaveOnOffState(DATAPROCESS_ONFFDEVICE_RMTP12, wSubIndex, DATAPROCESS_ONFFSTATE_ON);
				nRmtp12OldState = _SetBit(nRmtp12OldState, wSubIndex);
			}
			else
			{
				SaveOnOffState(DATAPROCESS_ONFFDEVICE_RMTP12, wSubIndex, DATAPROCESS_ONFFSTATE_OFF);
				nRmtp12OldState = _ClearBit(nRmtp12OldState, wSubIndex);
			}
		}
	}

	//pwm onoff line
	
	//tmiot onoff line
	ntmIoTState = GetDBValue("STATE_OTHR_OTHR_REMOTEIO_LINESTATE").lValue;
	
	for(int i=0; i<5; i++) /*最多5張tmIoT*/
	{
		if(((ntmIoTOldState >> i)&0x0001) != ((ntmIoTState >> i)&0x0001))
		{
			wSubIndex = i;
			if((ntmIoTState >> i)&0x0001)
			{
				SaveOnOffState(DATAPROCESS_ONFFDEVICE_TMIOT, wSubIndex, DATAPROCESS_ONFFSTATE_ON);
				ntmIoTOldState = _SetBit(ntmIoTOldState, wSubIndex);
			}
			else
			{
				SaveOnOffState(DATAPROCESS_ONFFDEVICE_TMIOT, wSubIndex, DATAPROCESS_ONFFSTATE_OFF);
				ntmIoTOldState = _ClearBit(ntmIoTOldState, wSubIndex);
			}
		}
	}
}

int		CtmTaskDataProcess::CopyFile(char*	pszSrc,	char*	pszDst)
{
	char	pszCmd[512];
	
	memset(pszCmd, 0, sizeof(pszCmd));
	sprintf(pszCmd, "cp %s %s ", pszSrc, pszDst);
	usleep(10*1000);
	system(pszCmd);
	sync();
	usleep(10*1000);
	return 1;	
}
	
int		CtmTaskDataProcess::ProcessMoniSave()//JOYCE2012-4-26
{
	int 	nShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	char	szFileName[32];
	tmDATETIME	datetime;

	if(GetDiskFreeSize(DEST_DIR) < 2048)
	{
		SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED116", 0);
		SendMsg(MSG_USER_SH_MONIONLINE_ERROR, 0, 0, NULL);//U盤快滿，畫面中可接收此MSG並提示用戶
		return -1;	
	}
		
	GetDate(&datetime.date);
	GetTime(&datetime.time);
	sprintf(szFileName, "moni_%d_%04d%02d%02d%02d%02d%02d.cdb", nShotCount, datetime.date.year, datetime.date.month,datetime.date.day, 
														 datetime.time.hour, datetime.time.minute, datetime.time.second);
	//printf("szFileName = %s\n", szFileName);
	
	  #ifdef	D_3354   //JOYCE2016-6-1

		char	pszMoniPath[256];
		char	cszFileName[256];
		memset(pszMoniPath, 0, sizeof(pszMoniPath));
		memset(cszFileName, 0, sizeof(cszFileName));

		//sprintf(pszMoniPath, "%s%s", "/media/sda1/monitor_55/",szFileName);
	
		char szFile[128];
		struct stat buf;
	    int k = stat(pszMoniPath,&buf);
	    if(k!=0)
	    {
	    	mkdir(pszMoniPath,0777);
	    }
	 		
		//sprintf(cszFileName, "%s%s", "/media/sda1/monitor/",szFileName);
	   sprintf(cszFileName, "%s%s%s", CtmConfig::GetInstance()->GetRecordFilePath(), FILENAME_MONI, FILENAME_END);
		sprintf(pszMoniPath, "%s%s", "/media/sda1/monitor_55/",szFileName);
    		
 		//printf("cszFileName:  %s\n", cszFileName);
 		//printf("pszMoniPath:  %s\n", pszMoniPath);
 		CopyFile(cszFileName, pszMoniPath);
 		
 		
	  #else
	  
		CtmUsbFile::GetInstance()->ReName(ID_MONI_SRAMFILE, szFileName);
		CtmQualityControl::GetInstance()->SaveAs(STORAGE_TYPE_USB);
		CtmUsbFile::GetInstance()->ReName(ID_MONI_SRAMFILE, "moni.cdb");
			
	  #endif	
			
	SetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED116", 0);
printf("ProcessMoniSave\n");			
}

int  CtmTaskDataProcess::GetDiskFreeSize(char* pszMountPoint)//JOYCE2012-4-26
{                                       
 struct statfs statFS;
 long long usedBytes = 0;
 long long freeBytes = 0;
 long long totalBytes = 0;
 
 if (statfs(pszMountPoint, &statFS) == -1){
  printf("statfs failed for path->[%s]\n", pszMountPoint);
  return -1;
 }
 
 totalBytes = (long long)statFS.f_blocks * (long long)statFS.f_frsize;
 freeBytes = (long long)statFS.f_bfree * (long long)statFS.f_frsize;
 usedBytes = (long long)(totalBytes - freeBytes);
 
 //printf("totalBytes = %lld, freeBytes = %lld, usedBytes = %lld\n", totalBytes/1024, freeBytes/1024,usedBytes/1024);
 return freeBytes/1024;
}

void	CtmTaskDataProcess::Init_DataEnergy()
{
	memset(&m_StateModeIndex, 0, sizeof(m_StateModeIndex));
	if(CtmConfig::GetInstance()->GetRecordType() == 1)		//james add new record moni
	{
		tmFileRecordHeadInfo	HeadInfo;
		
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_OPSTATE))
		{
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));
		}
		else
		{
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));
			m_StateModeIndex.nCount = HeadInfo.nCount;
			m_StateModeIndex.nIndex = HeadInfo.nIndex;
		}
																			   
		if(	m_StateModeIndex.nCount > CtmConfig::GetInstance()->GetDataEnergyMaxNum() || m_StateModeIndex.nCount < 0 ||    
			m_StateModeIndex.nIndex > m_StateModeIndex.nCount || m_StateModeIndex.nIndex < 0) 
		{                     
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));            
		}                                                                      
		if(m_StateModeIndex.nCount > CtmConfig::GetInstance()->GetDataEnergyMaxNum() && m_StateModeIndex.nIndex != 0)      
		{              
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo)); 
		}
	}
	else
	{
		if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DATASTATE_SRAMFILE))
		{
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, 0, &m_StateModeIndex);
		}
		else
		{
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATASTATE_SRAMFILE, 0, &m_StateModeIndex);
		}
																			   
		if(	m_StateModeIndex.nCount > CtmConfig::GetInstance()->GetDataEnergyMaxNum() || m_StateModeIndex.nCount < 0 ||    
			m_StateModeIndex.nIndex > m_StateModeIndex.nCount || m_StateModeIndex.nIndex < 0) 
		{                     
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, 0, &m_StateModeIndex);            
		}                                                                      
		if(m_StateModeIndex.nCount > CtmConfig::GetInstance()->GetDataEnergyMaxNum() && m_StateModeIndex.nIndex != 0)      
		{              
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, 0, &m_StateModeIndex); 
		}
	}
}

void	CtmTaskDataProcess::Init_DataState()
{
	memset(&m_tmCapacityIndex, 0, sizeof(m_tmCapacityIndex));
	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)		//james add new record moni
	{
		tmFileRecordHeadInfo	HeadInfo;
		
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_ENEGERY))
		{
			//printf("check faild Write \n");
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY, 0, &HeadInfo, sizeof(HeadInfo));
		}
		else
		{
			//printf("Read ok \n");
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_ENEGERY, 0, &HeadInfo, sizeof(HeadInfo));
			m_tmCapacityIndex.nCount = HeadInfo.nCount;
			m_tmCapacityIndex.nIndex = HeadInfo.nIndex;
			//printf("Read ok count=%d, index=%d \n", m_tmCapacityIndex.nCount, m_tmCapacityIndex.nIndex);
		}
																		   
		if(	m_tmCapacityIndex.nCount > CtmConfig::GetInstance()->GetDataStateMaxNum() || m_tmCapacityIndex.nCount < 0 ||    
			m_tmCapacityIndex.nIndex > m_tmCapacityIndex.nCount || m_tmCapacityIndex.nIndex < 0) 
		{                     
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY, 0, &HeadInfo, sizeof(HeadInfo));            
		}                                                                      
		if(m_tmCapacityIndex.nCount > CtmConfig::GetInstance()->GetDataStateMaxNum() && m_tmCapacityIndex.nIndex != 0)      
		{              
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY, 0, &HeadInfo, sizeof(HeadInfo)); 
		}
	}
	else
	{
		if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DATAENERGY_SRAMFILE))
		{
			//printf("check faild Write \n");
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE, 0, &m_tmCapacityIndex);
		}
		else
		{
			//printf("Read ok \n");
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAENERGY_SRAMFILE, 0, &m_tmCapacityIndex);
			//printf("Read ok count=%d, index=%d \n", m_tmCapacityIndex.nCount, m_tmCapacityIndex.nIndex);
		}
																			   
		if(	m_tmCapacityIndex.nCount > CtmConfig::GetInstance()->GetDataStateMaxNum() || m_tmCapacityIndex.nCount < 0 ||    
			m_tmCapacityIndex.nIndex > m_tmCapacityIndex.nCount || m_tmCapacityIndex.nIndex < 0) 
		{                     
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE, 0, &m_tmCapacityIndex);            
		}                                                                      
		if(m_tmCapacityIndex.nCount > CtmConfig::GetInstance()->GetDataStateMaxNum() && m_tmCapacityIndex.nIndex != 0)      
		{              
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE, 0, &m_tmCapacityIndex); 
		}
	}
}

void	CtmTaskDataProcess::Init_DataPowerOnOff()
{
	memset(&m_tmPowerOnOffIndex, 0, sizeof(m_tmPowerOnOffIndex));
	
	if(CtmConfig::GetInstance()->GetRecordType() == 1)		//james add new record moni
	{
		tmFileRecordHeadInfo	HeadInfo;
		
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_POWERONOFF))
		{
			//printf("check faild Write \n");
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, 0, &HeadInfo, sizeof(HeadInfo));
		}
		else
		{
			//printf("Read ok \n");
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_POWERONOFF, 0, &HeadInfo, sizeof(HeadInfo));
			m_tmPowerOnOffIndex.nCount = HeadInfo.nCount;
			m_tmPowerOnOffIndex.nIndex = HeadInfo.nIndex;
			//printf("Read poweron off ok count=%d, index=%d \n", m_tmPowerOnOffIndex.nCount, m_tmPowerOnOffIndex.nIndex);
		}
																			   
		if(	m_tmPowerOnOffIndex.nCount > CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum() || m_tmCapacityIndex.nCount < 0 ||    
			m_tmPowerOnOffIndex.nIndex > m_tmPowerOnOffIndex.nCount || m_tmCapacityIndex.nIndex < 0) 
		{                     
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, 0, &HeadInfo, sizeof(HeadInfo));            
		}                                                                      
		if(m_tmPowerOnOffIndex.nCount > CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum() && m_tmPowerOnOffIndex.nIndex != 0)      
		{              
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, 0, &HeadInfo, sizeof(HeadInfo)); 
		}	
	}
	else
	{
		if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DATAPOWERONOFF_SRAMFILE))
		{
			//printf("check faild Write \n");
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, 0, &m_tmPowerOnOffIndex);
		}
		else
		{
			//printf("Read ok \n");
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPOWERONOFF_SRAMFILE, 0, &m_tmPowerOnOffIndex);
			//printf("Read poweron off ok count=%d, index=%d \n", m_tmPowerOnOffIndex.nCount, m_tmPowerOnOffIndex.nIndex);
		}
																			   
		if(	m_tmPowerOnOffIndex.nCount > CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum() || m_tmCapacityIndex.nCount < 0 ||    
			m_tmPowerOnOffIndex.nIndex > m_tmPowerOnOffIndex.nCount || m_tmCapacityIndex.nIndex < 0) 
		{                     
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, 0, &m_tmPowerOnOffIndex);            
		}                                                                      
		if(m_tmPowerOnOffIndex.nCount > CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum() && m_tmPowerOnOffIndex.nIndex != 0)      
		{              
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, 0, &m_tmPowerOnOffIndex); 
		}
	}
}

void	CtmTaskDataProcess::Init_OnOffState()
{
	memset(&m_OnOffStateIndex, 0, sizeof(m_OnOffStateIndex));
	if(CtmConfig::GetInstance()->GetRecordType() == 1)		//james add new record moni
	{
		tmFileRecordHeadInfo	HeadInfo;
		
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		if (!CManageFile::GetInstance()->CheckData(FILE_RECORD_LINESTATE))
		{
			//printf("check faild Write \n");
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE, 0, &HeadInfo, sizeof(HeadInfo));
		}
		else
		{
			//printf("Read ok \n");
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_LINESTATE, 0, &HeadInfo, sizeof(HeadInfo));
			m_OnOffStateIndex.nCount = HeadInfo.nCount;
			m_OnOffStateIndex.nIndex = HeadInfo.nIndex;
			//printf("Read poweron off ok count=%d, index=%d \n", m_OnOffStateIndex.nCount, m_OnOffStateIndex.nIndex);
		}
																			   
		if(	m_OnOffStateIndex.nCount > CtmConfig::GetInstance()->GetOnOffStateMaxNum() ||
			m_OnOffStateIndex.nIndex > m_OnOffStateIndex.nCount ) 
		{                     
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE, 0, &HeadInfo, sizeof(HeadInfo));            
		}                                                                      
		if(m_OnOffStateIndex.nCount > CtmConfig::GetInstance()->GetOnOffStateMaxNum() && m_OnOffStateIndex.nIndex != 0)      
		{              
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE, 0, &HeadInfo, sizeof(HeadInfo)); 
		}
	}
	else
	{
		if (!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_ONOFFSTATE_SRAMFILE))
		{
			//printf("check faild Write \n");
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE, 0, &m_OnOffStateIndex);
		}
		else
		{
			//printf("Read ok \n");
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_ONOFFSTATE_SRAMFILE, 0, &m_OnOffStateIndex);
			//printf("Read poweron off ok count=%d, index=%d \n", m_OnOffStateIndex.nCount, m_OnOffStateIndex.nIndex);
		}
																			   
		if(	m_OnOffStateIndex.nCount > CtmConfig::GetInstance()->GetOnOffStateMaxNum() ||
			m_OnOffStateIndex.nIndex > m_OnOffStateIndex.nCount ) 
		{                     
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE, 0, &m_OnOffStateIndex);            
		}                                                                      
		if(m_OnOffStateIndex.nCount > CtmConfig::GetInstance()->GetOnOffStateMaxNum() && m_OnOffStateIndex.nIndex != 0)      
		{              
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE, 0, &m_OnOffStateIndex); 
		}
	}
}

//nType = 0, 當前繼續存儲
//nType = 1,,重新開始存儲
//nType = 2, 暫停存儲
//nType = 3, 開始存儲
//nType = 4, 停止存儲
int		CtmTaskDataProcess::SaveEnergy(int nType, int nIndex)
{
	TMCAPACITY	tmCapacity;
	tmDATETIME	datetime;
	DWORD		dwMoldCount = 0, dwGoodProduct = 0, dwBadProduct = 0, dwProductUnit = 0, dwEnergy = 0;
	
	dwMoldCount 	= GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	dwProductUnit 	= GetDBValue("MHDR_MODULE_NULL_NULL_MODULEUNIT").lValue;
	dwEnergy		= GetDBValue("AMMETER_HMI_REAL_POWER").lValue;
	dwBadProduct 	= 0;
	
	//printf("nType=%d nIndex=%d nCount=%d, nIndex=%d dwProductUnit=%d \n", nType, nIndex, m_tmCapacityIndex.nCount, m_tmCapacityIndex.nIndex, dwProductUnit);
	memset(&tmCapacity, 0, sizeof(tmCapacity));
	if(dwProductUnit  == 0)								//若單模產品總數是0，則為1
		dwProductUnit = 1;
	GetDate(&datetime.date);
	GetTime(&datetime.time);
	if(nType == 0)										//在當前記錄中繼續存儲
	{
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->ReadRecord(FILE_RECORD_ENEGERY, nIndex, &tmCapacity, sizeof(tmCapacity));
		else
			CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAENERGY_SRAMFILE, nIndex, &tmCapacity);
		
		if(tmCapacity.starttime.date.year <= 1970)		//說明日期錯誤
		{
			memcpy(&tmCapacity.starttime, &datetime, sizeof(datetime));
		}
		memcpy(&tmCapacity.endtime, &datetime, sizeof(datetime));
		tmCapacity.dwMoldCountEnd 	= dwMoldCount;		//結束開模序號
		tmCapacity.dwGoodProduct	+= dwProductUnit;	//良品數 累加方式
		tmCapacity.dwBadProduct		+= dwBadProduct;	//不良品數
		tmCapacity.dwEnergyEnd		=  dwEnergy;		//結束能耗	
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY, nIndex, &tmCapacity, sizeof(tmCapacity));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE, nIndex, &tmCapacity);
	}
	else if(nType == 1)									//重新開始記錄
	{
		memcpy(&tmCapacity.starttime, &datetime, sizeof(datetime));
		memcpy(&tmCapacity.endtime, &datetime, sizeof(datetime));
		tmCapacity.dwMoldCountStart = dwMoldCount;		//起始開模序號
		tmCapacity.dwMoldCountEnd 	= dwMoldCount;		//結束開模序號
		tmCapacity.dwGoodProduct	= 0;				//良品數 累加方式
		tmCapacity.dwBadProduct		= 0;				//不良品數
		tmCapacity.dwEnergyStart	= dwEnergy;			//起始能耗
		tmCapacity.dwEnergyEnd		= dwEnergy;			//結束能耗	
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
		{
			tmFileRecordHeadInfo	HeadInfo;
			memset(&HeadInfo, 0, sizeof(HeadInfo));
			
			HeadInfo.nCount = m_tmCapacityIndex.nCount;
			HeadInfo.nIndex = m_tmCapacityIndex.nIndex;
			
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY, nIndex, &tmCapacity, sizeof(tmCapacity));
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY, 0, &HeadInfo, sizeof(HeadInfo));
		}
		else
		{
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE, nIndex, &tmCapacity);
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE, 0, &m_tmCapacityIndex);
		}
		
	}
	else if(nType == 2)									//暫停存儲
	{
		m_bCapacitySaveFlag = FALSE;	
	}
	else if(nType == 3)									//開始存儲
	{
		m_bCapacitySaveFlag = TRUE;	
	}
	return TRUE;
}

int	CtmTaskDataProcess::SaveState(int nMode, int nIndex)
{
	STATEMODE	tmStateMode;
	
	
	tmStateMode.nMode = nMode;
	GetDate(&tmStateMode.date);
	GetTime(&tmStateMode.time);
	tmStateMode.dwOpenMoldCount =  GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue + 1;/*wangli2016-1-22 10:58:30 事件畫面的開模序號從1開始*/
//	if(g_pDatabase != NULL)
//		memcpy(tmStateMode.cszUserID, g_pDatabase->Read("SYS_HMI_NULL_NULL_USERCURRENTID").pcData, 7);
	if ((g_pszUserID != NULL && g_pszUserID[0] != '\0'))
			strcpy(tmStateMode.cszUserID, g_pszUserID);
	else	strcpy(tmStateMode.cszUserID, "NOLOGIN");	
	
	if(nIndex >=0)
	{
		if (m_StateModeIndex.nIndex > CtmConfig::GetInstance()->GetDataStateMaxNum()) 
		{
			m_StateModeIndex.nIndex = 0;
		}
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, nIndex, &tmStateMode, sizeof(tmStateMode));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, nIndex, &tmStateMode);
			
	}
	else
	{
		if (m_StateModeIndex.nIndex >= CtmConfig::GetInstance()->GetDataStateMaxNum()) 
		{
			m_StateModeIndex.nIndex = 0;
		}
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, m_StateModeIndex.nIndex + 1, &tmStateMode, sizeof(tmStateMode));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, m_StateModeIndex.nIndex + 1, &tmStateMode);
	}
	if (m_StateModeIndex.nCount < CtmConfig::GetInstance()->GetDataStateMaxNum()) 
	{
		m_StateModeIndex.nCount++;
	}
	if ((++m_StateModeIndex.nIndex) >= CtmConfig::GetInstance()->GetDataStateMaxNum()) 
	{
		m_StateModeIndex.nIndex = 0;
	}
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		
		HeadInfo.nCount = m_StateModeIndex.nCount;
		HeadInfo.nIndex = m_StateModeIndex.nIndex;
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE, 0, &HeadInfo, sizeof(HeadInfo));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE, 0, &m_StateModeIndex);
	
	return TRUE;		
}

int	CtmTaskDataProcess::SavePowerOnOff(int nMode, int nIndex)
{
	TMPOWERONOFF	tmPowerOnOff;
	int		nTempIndex = 0;
	
	memset(&tmPowerOnOff, 0, sizeof(tmPowerOnOff));
	switch(nMode)
	{
		case 0:					//PowerOn
			GetDate(&tmPowerOnOff.powerontime.date);
			GetTime(&tmPowerOnOff.powerontime.time);
			GetDate(&tmPowerOnOff.powerofftime.date);
			GetTime(&tmPowerOnOff.powerofftime.time);
			if(nIndex >=0)
			{
				if (m_tmPowerOnOffIndex.nIndex > CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum()) 
				{
					m_tmPowerOnOffIndex.nIndex = 0;
				}
				if(CtmConfig::GetInstance()->GetRecordType() == 1)
					CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, nIndex, &tmPowerOnOff, sizeof(tmPowerOnOff));	
				else
					CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, nIndex, &tmPowerOnOff);	
			}
			else
			{
				if (m_tmPowerOnOffIndex.nIndex >= CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum()) 
				{
					m_tmPowerOnOffIndex.nIndex = 0;
				}
				//printf("SavePowerOnOff 0 nIndex=%d count=%d \n", m_tmPowerOnOffIndex.nIndex, m_tmPowerOnOffIndex.nCount);
				if(CtmConfig::GetInstance()->GetRecordType() == 1)
					CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, m_tmPowerOnOffIndex.nIndex + 1, &tmPowerOnOff, sizeof(tmPowerOnOff));
				else
					CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, m_tmPowerOnOffIndex.nIndex + 1, &tmPowerOnOff);
			}
			if (m_tmPowerOnOffIndex.nCount < CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum()) 
			{
				m_tmPowerOnOffIndex.nCount++;
			}
			if ((++m_tmPowerOnOffIndex.nIndex) >= CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum()) 
			{
				m_tmPowerOnOffIndex.nIndex = 0;
			}
			if(CtmConfig::GetInstance()->GetRecordType() == 1)
			{
				tmFileRecordHeadInfo	HeadInfo;
				memset(&HeadInfo, 0, sizeof(HeadInfo));
				
				HeadInfo.nCount = m_tmPowerOnOffIndex.nCount;
				HeadInfo.nIndex = m_tmPowerOnOffIndex.nIndex;
				CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, 0, &HeadInfo, sizeof(HeadInfo));
			}
			else
				CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, 0, &m_tmPowerOnOffIndex);
		
			break;
		case 1:					//PowerOff
			//printf("SavePowerOnOff 1 nIndex=%d count=%d \n", m_tmPowerOnOffIndex.nIndex, m_tmPowerOnOffIndex.nCount);
			if(nIndex >=0)
			{
				if(CtmConfig::GetInstance()->GetRecordType() == 1)
				{
					CManageFile::GetInstance()->ReadRecord(FILE_RECORD_POWERONOFF, nIndex, &tmPowerOnOff, sizeof(tmPowerOnOff));
					GetDate(&tmPowerOnOff.powerofftime.date);
					GetTime(&tmPowerOnOff.powerofftime.time);
					CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, nIndex, &tmPowerOnOff, sizeof(tmPowerOnOff));
				}
				else
				{
					CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPOWERONOFF_SRAMFILE, nIndex, &tmPowerOnOff);
					GetDate(&tmPowerOnOff.powerofftime.date);
					GetTime(&tmPowerOnOff.powerofftime.time);
					CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, nIndex, &tmPowerOnOff);
				}
			}
			else
			{
				if(m_tmPowerOnOffIndex.nIndex <=0)
					nTempIndex = CtmConfig::GetInstance()->GetDataPowerOnOffMaxNum();
				else
					nTempIndex = m_tmPowerOnOffIndex.nIndex;
				if(CtmConfig::GetInstance()->GetRecordType() == 1)
				{
					CManageFile::GetInstance()->ReadRecord(FILE_RECORD_POWERONOFF, nTempIndex, &tmPowerOnOff, sizeof(tmPowerOnOff));
					GetDate(&tmPowerOnOff.powerofftime.date);
					GetTime(&tmPowerOnOff.powerofftime.time);
					CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF, nTempIndex, &tmPowerOnOff, sizeof(tmPowerOnOff));
				}
				else
				{
					CtmSarmRecordFileControl::GetInstance()->ReadRecord(ID_DATAPOWERONOFF_SRAMFILE, nTempIndex, &tmPowerOnOff);
					GetDate(&tmPowerOnOff.powerofftime.date);
					GetTime(&tmPowerOnOff.powerofftime.time);
					CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE, nTempIndex, &tmPowerOnOff);
				}
			}
			
			break;
		default:
			break;	
	}
}

int	CtmTaskDataProcess::SaveOnOffState(int nDevice, WORD	wSubIndex, WORD wState, int nIndex)
{
	ONOFFSTATE	tmOnOffState;
	
	tmOnOffState.nDevice 	= nDevice;
	tmOnOffState.wSubIndex 	= wSubIndex;
	tmOnOffState.wState 	= wState;
	GetDate(&tmOnOffState.date);
	GetTime(&tmOnOffState.time);
	tmOnOffState.dwOpenMoldCount =  GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue +1;/*5528事件畫面的開模序號從1開始*/
	//printf("CtmTaskDataProcess::SaveOnOffState nDevice=%d,wSubIndex=%d nIndex=%d \n",nDevice,wSubIndex, nIndex);
	if(nIndex >=0)
	{
		if (m_OnOffStateIndex.nIndex > CtmConfig::GetInstance()->GetOnOffStateMaxNum()) 
		{
			m_OnOffStateIndex.nIndex = 0;
		}
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE, nIndex, &tmOnOffState, sizeof(tmOnOffState));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE, nIndex, &tmOnOffState);
	}
	else
	{
		if (m_OnOffStateIndex.nIndex >= CtmConfig::GetInstance()->GetOnOffStateMaxNum()) 
		{
			m_OnOffStateIndex.nIndex = 0;
		}
		if(CtmConfig::GetInstance()->GetRecordType() == 1)
			CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE, m_OnOffStateIndex.nIndex + 1, &tmOnOffState, sizeof(tmOnOffState));
		else
			CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE, m_OnOffStateIndex.nIndex + 1, &tmOnOffState);
	}
	if (m_OnOffStateIndex.nCount < CtmConfig::GetInstance()->GetOnOffStateMaxNum()) 
	{
		m_OnOffStateIndex.nCount++;
	}
	if ((++m_OnOffStateIndex.nIndex) >= CtmConfig::GetInstance()->GetOnOffStateMaxNum()) 
	{
		m_OnOffStateIndex.nIndex = 0;
	}
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		
		HeadInfo.nCount = m_OnOffStateIndex.nCount;
		HeadInfo.nIndex = m_OnOffStateIndex.nIndex;
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE, 0, &HeadInfo, sizeof(HeadInfo));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE, 0, &m_OnOffStateIndex);
	
	return TRUE;		
}

int	CtmTaskDataProcess::ClearStateLog()
{
	m_StateModeIndex.nCount = 0;
	m_StateModeIndex.nIndex = 0;
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_OPSTATE,0, &HeadInfo, sizeof(HeadInfo));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATASTATE_SRAMFILE,0, &m_StateModeIndex);
	return TRUE;
}

int	CtmTaskDataProcess::ClearEnergyLog()
{
	m_tmCapacityIndex.nCount = 0;
	m_tmCapacityIndex.nIndex = 0;
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_ENEGERY,0, &HeadInfo, sizeof(HeadInfo));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAENERGY_SRAMFILE,0, &m_tmCapacityIndex);
	return TRUE;
}

int	CtmTaskDataProcess::ClearPowerOnOff()
{
	m_tmPowerOnOffIndex.nCount = 0;
	m_tmPowerOnOffIndex.nIndex = 0;
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_POWERONOFF,0, &HeadInfo, sizeof(HeadInfo));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_DATAPOWERONOFF_SRAMFILE,0, &m_tmPowerOnOffIndex);
	return TRUE;
}

int	CtmTaskDataProcess::ClearOnOffState()
{
	m_OnOffStateIndex.nCount = 0;
	m_OnOffStateIndex.nIndex = 0;
	if(CtmConfig::GetInstance()->GetRecordType() == 1)
	{
		tmFileRecordHeadInfo	HeadInfo;
		memset(&HeadInfo, 0, sizeof(HeadInfo));
		CManageFile::GetInstance()->WriteRecord(FILE_RECORD_LINESTATE,0, &HeadInfo, sizeof(HeadInfo));
	}
	else
		CtmSarmRecordFileControl::GetInstance()->WriteRecord(ID_ONOFFSTATE_SRAMFILE,0, &m_OnOffStateIndex);
	return TRUE;
}

int	CtmTaskDataProcess::CalculateTime(int	nType, tmDATETIME datetime_start, tmDATETIME datetime_end)
{
	int		nTime = 0;
	nTime = MakeTime(&datetime_end, nType) - MakeTime(&datetime_start, nType);
/*	DWORD	dwData = 0, dwYear = 0, dwMonth = 0, dwDay = 0, dwHour = 0, dwMin = 0;
	DWORD	dwYear2 = 0, dwMonth2 = 0, dwDay2 = 0, dwHour2 = 0, dwMin2 = 0;
	int		nYear3 = 0, nMonth3 = 0, dwDay2 = 0, nHour3 = 0, nMin3 = 0;
	int		nCount_Min = 0;
	dwYear 		= datetime_start.date.year;
	dwMonth		= datetime_start.date.month;
	dwDay		= datetime_start.date.day;
	dwHour  	= datetime_start.time.hour;
	dwMin		= datetime_start.time.minute;
	
	dwYear2 	= datetime_end.date.year;
	dwMonth2	= datetime_end.date.month;
	dwDay2		= datetime_end.date.day;
	dwHour2  	= datetime_end.time.hour;
	dwMin2		= datetime_end.time.minute;
	
	nYear3 	= dwYear2 - dwYear;
	nMonth3 = dwMonth2 - dwMonth;
	dwDay2	= dwDay2 - dwDay;
	nHour3	= dwHour2 - dwHour;
	nMin3	= dwMin2 - dwMin;
	
	
	if(nYear3 >= 0)
	{
		nCount_Min += YEAR*nYear3;
	}
	else
	{
		
	}
	
	if(dwYear2 >= dwYear)
	{
		if(dwYear2 == dwYear)		//年份相同
		{
			if(dwMonth2 >= dwMonth)	//月份大
			{
				nMonth3 = dwMonth2-dwMonth;
			}
			else					//月份小
			{
				nMonth3 
			}
		}
		else						//夸年份
		{
			nYear3 = dwYear2-dwYear;
		}
	}
	else
	{
			
	}
	switch(nType)
	{
		case 0:				//Get 	min
			
			break;
		case 1:				//Get	hour
			
			break;
		case 2:				//Get	day
			
			break;
		default:
			break;
	}*/
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function : MakeTime(tmDATETIME *tm)                                       |
|  Task     : 將以年月日表示的時間轉化為按從1970年1月1日0時0分0秒至今的秒數  |
|             來表示                                                         |
+----------------------------------------------------------------------------+
|  Call     : MakeTime(tmDATETIME *tm)                                       |
|                                                                            |
|  Parameter: tm :以年月日方式表示的日期                                     |
|                                                                            |
|  Return   : 從1970年1月1日0時0分0秒至今的秒數                              |
+---------------------------------------------------------------------------*/
UTIME      CtmTaskDataProcess::MakeTime(tmDATETIME *tm, int nType)
{
	UTIME res;
	int   year;

	year = tm->date.year - 1970;
	res = YEAR*year + DAY*((year+1)/4);									//滿百年的非閏年問題已經無法考慮了
	res += month[tm->date.month - 1];
	
	if ((tm->date.month > 2 ) && (tm->date.year%4 == 0))
		res += DAY;
	
	switch(nType)
	{
		case 0:				//get second
			res += DAY*(tm->date.day-1);
			res += HOUR*tm->time.hour;
			res += MINUTE*tm->time.minute;
			res += tm->time.second;
			
			break;
		case 1:				//get minute
			res += DAY*(tm->date.day-1);
			res += HOUR*tm->time.hour;
			break;
		case 2:				//get hour
			res += DAY*(tm->date.day-1);
			break;
		default:
			res += DAY*(tm->date.day-1);
			res += HOUR*tm->time.hour;
			res += MINUTE*tm->time.minute;
			res += tm->time.second;
			break;
	}
	
	return res;
}


//==============================================================================
//DataProcessSave
//==============================================================================
/*---------------------------------------------------------------------------+
|  Function : CtmDataProcessSave::CtmDataProcessSave()						 |
|  Task     : 構造函數  						     	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       			         -     	                             |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmDataProcessSave::CtmDataProcessSave() : CDataProcessSave()
{
	for(unsigned int i = 0; i < DATAPROCESS_FILE_MAXCOUNT; i++)
	{
		m_pszArryFileName[i] = new char[DATAPROCESS_FILENAME_MAXLEN];
		memset(m_pszArryFileName[i], 0, DATAPROCESS_FILENAME_MAXLEN);
	}
	m_dwFileType 		= 0;
	m_dwFileCount 		= 0;
	m_dwFileMaxCount	= 0;
	m_dwFileNameLen 	= 0;
	m_dwFileIndex		= 1;
	
	m_dwType 			= 0;
	m_dwMode			= 0;
	m_dwCount			= 0;
	m_dwMaxCount		= 0;
	m_dwLen				= 0;
	m_pData				= NULL;
	memset(m_pszConfigName, 0, sizeof(m_pszConfigName));
	memset(m_pszFileName, 0, sizeof(m_pszFileName));
	memset(m_pszDataPath, 0, sizeof(m_pszDataPath));
	memset(m_pszDataConfigPath, 0, sizeof(m_pszDataConfigPath));
}

/*---------------------------------------------------------------------------+
|  Function : CtmDataProcessSave::~CtmDataProcessSave()						 |
|  Task     : 析構函數  						     	                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       			         -     	                             |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmDataProcessSave::~CtmDataProcessSave()
{
	for(unsigned int i = 0; i < DATAPROCESS_FILE_MAXCOUNT; i++)
	{
		if(m_pszArryFileName[i] != NULL)
		{
			delete [] 	m_pszArryFileName[i];
			m_pszArryFileName[i] = NULL;
		}
	}
}

/*---------------------------------------------------------------------------+
|  Function : int		CtmDataProcessSave::SetDataPath(char*	pszName)	 |
|  Task     : 設置存儲的文件路徑  						     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       pszName         -     文件名稱                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::SetDataPath(char*	pszName)
{
	int nResult = 0;
	if(pszName != NULL)
	{
		strcpy(m_pszDataPath, pszName);	
	}
	
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : int		CtmDataProcessSave::SetDataConfigPath(char*	pszName) |
|  Task     : 設置存儲的文件路徑  						     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       pszName         -     文件名稱                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::SetDataConfigPath(char*	pszName)
{
	int nResult = 0;
	if(pszName != NULL)
	{
		strcpy(m_pszDataConfigPath, pszName);	
	}
	
	return nResult;	
}


/*---------------------------------------------------------------------------+
|  Function : int		CtmDataProcessSave::SetFileName(char*	pszName)	 |
|  Task     : 設置存儲的文件名稱  						     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       pszName         -     文件名稱                           |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::SetConfigName(char*	pszName)
{
	int	nResult = 0;
	//printf("SetConfigName=%s \n", pszName);
	if(pszName != NULL)
	{
		strcpy(m_pszConfigName, pszName);	
	}
	GetInitInfo();
	
	return nResult;
}

int		CtmDataProcessSave::SetDataFileName(char*	pszName)
{
	int	nResult = 0;
	//printf("SetDataFileName=%s \n", pszName);
	if(pszName != NULL)
	{
		strcpy(m_pszDataFileName, pszName);	
	}
	
	return nResult;
}

int		CtmDataProcessSave::SetDataFileName_Config(char*	pszName)
{
	int	nResult = 0;
	//printf("SetDataFileName=%s \n", pszName);
	if(pszName != NULL)
	{
		strcpy(m_pszFileName_Config, pszName);	
	}
	
	return nResult;
}
/*---------------------------------------------------------------------------+
|  Function : int		CtmDataProcessSave::GetInitInfo()					 |
|  Task     : 讀取文件Head相關訊息  						     	         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       			         -     	                             |
|                                                                            |
|  Return   :   0                       -       成功                         |
|               -1                      -       失敗                         |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::GetInitInfo()
{
	int nResult = 0;
	
	//讀取config文件相關訊息
	if(m_File.Open(m_pszConfigName) == tmFILE_GOOD)
	{
		m_File.Read(&m_dwFileType, 	sizeof(m_dwFileType));
		
		//若為config文件，讀取config 中文件個數
		if(m_dwFileType == DATAPROCESS_FILETYPE_CONFIG)
		{
			m_File.Close();
			m_dwFileCount = ReadConfig((char**)m_pszArryFileName);
		}
		//若為Data文件, 讀取data文件相關head信息
		else if(m_dwFileType == DATAPROCESS_FILETYPE_DATA)
		{
			m_File.Read(&m_dwType, 	sizeof(m_dwType));
			m_File.Read(&m_dwMode, 	sizeof(m_dwMode));
			m_File.Read(&m_dwCount, sizeof(m_dwCount));
			m_File.Read(&m_dwLen, 	sizeof(m_dwLen));
		}
		m_File.Close();
	}
	else
	{
		nResult = -1;	
	}
	//m_File.Close();
	return nResult;
}

//返回讀到的config數據個數
int		CtmDataProcessSave::ReadConfig(char**	pFileName)
{
	int nResult = 0, i = 0;
	DWORD	dwLen, dwDataFileCount = 0;		
	char	cszFileName[256];
	CtmFile	tmFile;
	if(m_File.Open(m_pszConfigName) == tmFILE_GOOD)
	{
		m_File.Read(&m_dwFileType, 		sizeof(m_dwFileType));
		m_File.Read(&m_dwFileCount, 	sizeof(m_dwFileCount));
		if(m_dwFileCount > DATAPROCESS_FILE_MAXCOUNT)
			m_dwFileCount = DATAPROCESS_FILE_MAXCOUNT;
		//printf("m_dwFileCount=%d m_pszConfigName=%s \n", m_dwFileCount, m_pszConfigName);
		for(i = 0; i < m_dwFileCount; i++)
		{
			nResult = m_File.Read(&dwLen, sizeof(dwLen));
			//當讀到數據，則讀取，否則break，表示文件讀取完成
			if(nResult > 0)
			{
				if(dwLen <= DATAPROCESS_FILENAME_MAXLEN)
					m_File.Read(pFileName[i], 	dwLen);
				else 
					m_File.Seek(dwLen, SEEK_CUR);
			}
			else
			{
				printf("Read Config count Error i=%d \n", i);	
				break;
			}
			//Read All DataFile Count
			memset(cszFileName, 0, sizeof(cszFileName));
			sprintf(cszFileName, "%s%s%s", m_pszDataPath, m_pszDataConfigPath, pFileName[i]);
			//if(tmFile.Open(pFileName[i]) == tmFILE_GOOD)
			if(tmFile.Open(cszFileName) == tmFILE_GOOD)
			{
				dwDataFileCount = 0;
				tmFile.Seek(DATAPROCESS_HEAD_COUNT_POS, SEEK_SET);
				tmFile.Read(&dwDataFileCount, 		sizeof(dwDataFileCount));
				 m_dwAllCount += dwDataFileCount;
				tmFile.Close();
			}
		}
		nResult = m_dwFileIndex = i;
		memset(cszFileName, 0, sizeof(cszFileName));
		if(m_dwFileIndex >0)
			sprintf(cszFileName, "%s%s%s", m_pszDataPath, m_pszDataConfigPath, pFileName[m_dwFileIndex-1]);	
		else
			sprintf(cszFileName, "%s%s%s", m_pszDataPath, m_pszDataConfigPath, pFileName[0]);
		strcpy(m_pszFileName, cszFileName);
		//strcpy(m_pszFileName, pFileName[m_dwFileIndex]);
		m_File.Close();
		
		//printf("m_pszFileName=%s m_dwFileIndex=%d\n", m_pszFileName, m_dwFileIndex);
		//讀取當前Data文件數據個數
		if(m_File.Open(m_pszFileName) == tmFILE_GOOD)
		{
			m_File.Read(&m_dwFileType, 		sizeof(m_dwFileType));
			m_File.Read(&m_dwType, 			sizeof(m_dwType));
			m_File.Read(&m_dwMode, 			sizeof(m_dwMode));
			m_File.Read(&m_dwCount, 		sizeof(m_dwCount));
			//printf("ReadConfig m_dwAllCount=%d m_dwCount=%d \n", m_dwAllCount, m_dwCount);
			m_File.Close();
		}
	}
	else
	{
		nResult = -1;	
	}
	
	return nResult;
}

int		CtmDataProcessSave::WriteConfig(char*	pFileName)
{
	int nResult = 0, i=0;
	DWORD	dwLen;		
	if(m_File.Open(m_pszConfigName) == tmFILE_GOOD)
	{
		m_File.Read(&m_dwFileType, 		sizeof(m_dwFileType));
		m_File.Read(&m_dwFileCount, 	sizeof(m_dwFileCount));
		if(m_dwFileCount > DATAPROCESS_FILE_MAXCOUNT)
		{
			printf("Write Full \n");
			nResult = -1;
			return nResult;
		}
		printf("WriteConfig m_dwFileCount=%d \n", m_dwFileCount);
		for(i = 0; i < m_dwFileCount; i++)
		{
			nResult = m_File.Read(&dwLen, sizeof(dwLen));
			//當讀到數據，則讀取，否則break，表示文件讀取完成
			if(nResult > 0)
			{
				m_File.Seek(dwLen, SEEK_CUR);
			}
			else
			{
				printf("Read Config count Error \n");	
				break;
			}
		}
		nResult = /*m_dwFileIndex = */i;
		m_dwFileNameLen = strlen(pFileName);
		m_File.Write(&m_dwFileNameLen, sizeof(m_dwFileNameLen));
		printf("write config pFileName=%s m_dwFileIndex=%d \n", pFileName, m_dwFileIndex);
		m_File.Write(pFileName, m_dwFileNameLen);
		m_dwFileCount +=1;
		//sprintf(m_pszFileName, "%s%d", m_pszDataFileName, m_dwFileIndex);
		m_File.Seek(DATAPROCESS_HEAD_TYPE_POS, SEEK_SET);
		m_File.Write(&m_dwFileCount, 	sizeof(m_dwFileCount));
		m_File.Close();
	}
	else
	{
		nResult = -1;
	}
	
	return nResult;
}


int		CtmDataProcessSave::SetFileMaxCount(DWORD	dwFileMaxCount)
{
	int nResult = 0;
	m_dwFileMaxCount = dwFileMaxCount;
	return nResult;
}

int		CtmDataProcessSave::SetMaxCount(DWORD	dwMaxCount)
{
	int nResult = 0;
	m_dwMaxCount = dwMaxCount;
	return nResult;	
}

int		CtmDataProcessSave::CheckDir(char*	pszFileName)
{
	char	cszDir[DATAPROCESS_FILENAME_MAXLEN];
	DIR*		dir;
	int i = 0, j = 0, nResult = 0;
	memset(cszDir, 0, sizeof(cszDir));
	
	printf("============checkdir =%s \n", pszFileName);
	for(unsigned i = 0; i < strlen(pszFileName); i++)
	{
		if(pszFileName[i] == '/' && i != 0)
		{
			printf("check Dir cszDir=%s i=%d \n", cszDir, i);
			dir = opendir(cszDir);
			if (!dir) 
			{
				nResult = mkdir(cszDir, 777);
			}
			else
			{
				nResult = 0;	
			}
			closedir(dir);
			cszDir[i] = pszFileName[i];
		}
		else
		{
			cszDir[i] = pszFileName[i];
		}
	}
	
	return nResult;
}

int		CtmDataProcessSave::CheckFile(char*	pszFileName)
{
	int		nResult = -1;
	if(m_File.Open(pszFileName, O_RDWR) == tmFILE_GOOD)
	{
		//m_File.Seek(DATAPROCESS_HEAD_SIZE, SEEK_SET);
		m_File.Close();
		nResult = 0;
	}
	return nResult;	
}

int		CtmDataProcessSave::CheckConfigFile(char*	pszFileName, int	nType)
{
	int		nResult = -1;
	char	cszFileName[256];
	printf("CheckConfigFile=%s \n", pszFileName);
	if(m_File.Open(pszFileName, O_RDWR) == tmFILE_GOOD)
	{
		m_File.Close();
		nResult = 0;
	}
	else		
	{
		//創建目錄成功
		//printf("else check dir pszFileName=%s \n", pszFileName);
		nResult = CheckDir(pszFileName);
		if(0 == nResult)
		{
			//檢查有此文件
			nResult = CheckFile(pszFileName);
			if(0 == nResult)
			{
				;
			}
			else
			{
				//printf("pszFileName=%s \n", pszFileName);
				//若文件不存在，則重新創建，并寫入Head信息
				if(m_File.Open(pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
				{
					m_dwFileType 	= DATAPROCESS_FILETYPE_CONFIG;
					m_dwFileCount 	= 1;
					
					m_File.Write(&m_dwFileType, 	sizeof(m_dwFileType));
					m_File.Write(&m_dwFileCount, 	sizeof(m_dwFileCount));
					//組合當前使用的文件名稱
					memset(cszFileName, 0, sizeof(cszFileName));
					memset(m_pszFileName, 0, sizeof(m_pszFileName));
					sprintf(m_pszFileName, "%s%d", m_pszDataFileName, m_dwFileIndex);
					sprintf(cszFileName, "%s%d", m_pszFileName_Config,m_dwFileIndex);
					//printf("check config cszFileName=%s \n", cszFileName);
					m_dwFileNameLen = strlen(cszFileName);
					m_File.Write(&m_dwFileNameLen, 	sizeof(m_dwFileNameLen));
					m_File.Write(cszFileName, 	m_dwFileNameLen);
					m_File.Close();
					nResult = 0;
				}
				else		//創建失敗，返回
				{
					printf("Open Faild \n");
					nResult = -1;
					return nResult;
				}
			}
		}
	}
	return nResult;	
}

int		CtmDataProcessSave::CheckDataFile(char*	pszFileName, int	nType)
{
	int		nResult = -1;
	//printf("CheckDataFile=%s \n", pszFileName);
	//若文件名為NULL，則查找文件名
	if(m_pszFileName != NULL && m_pszFileName[0]  == '\0')
	{
		
	}
	if(m_File.Open(pszFileName, O_RDWR) == tmFILE_GOOD)
	{
		m_File.Close();
		nResult = 0;
	}
	else		
	{
		nResult = CheckDir(pszFileName);	//創建目錄成功
		//printf("dir nResult=%d \n", nResult);
		if(0 == nResult)
		{
			//檢查有此文件
			nResult = CheckFile(pszFileName);
			//printf("file nResult=%d \n", nResult);
			if(0 == nResult)
			{
				;
			}
			else
			{
				//若文件不存在，則重新創建，并寫入Head信息
				if(m_File.Open(pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
				{
					m_dwFileType 	= DATAPROCESS_FILETYPE_DATA;
					m_dwType 		= nType;
					m_dwMode		= GetMode(nType);
					m_dwCount		= 0;
					m_File.Write(&m_dwFileType, 	sizeof(m_dwFileType));
					m_File.Write(&m_dwType, 	sizeof(m_dwType));
					m_File.Write(&m_dwMode, 	sizeof(m_dwMode));
					m_File.Write(&m_dwCount, 	sizeof(m_dwCount));
					m_File.Close();
					nResult = 0;
				}
				else		//創建失敗，返回
				{
					printf("Open Faild \n");
					nResult = -1;
					return nResult;
				}
			}
		}
	}
	return nResult;	
}

/*---------------------------------------------------------------------------+
|  Function : int	Read(int	nType, int nIndex, int nSize,void* pData)    |
|  Task     : 讀取指定類型資料  						     	             |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
| 				    nIndex            -     資料序號                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :  0=未讀到任何資料 >0 讀到資料 <0 讀取失敗                      |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::Read(int	nType, int	nIndex, 	int nSize, void* pData)
{
	int	nResult = 0, nCurrentFileStart = 0;
	DWORD	dwLen = 0, dwPosition = 0;
	
	if(pData != NULL)
	{
		CalculateReadFileName(nIndex);
		nCurrentFileStart = nIndex%m_dwMaxCount;
		printf("read m_pszReadFileName=%s nIndex=%d nCurrentFileStart=%d m_dwMaxCount=%d\n", m_pszReadFileName, nIndex, nCurrentFileStart, m_dwMaxCount);
		if(m_File.Open(m_pszReadFileName, O_RDONLY) == tmFILE_GOOD)
		{
			m_File.Seek(DATAPROCESS_HEAD_SIZE, SEEK_SET);
			dwPosition += DATAPROCESS_HEAD_SIZE;
			for(int i = 0; i < nCurrentFileStart; i++)
			{
				nResult = m_File.Read(&dwLen, 	sizeof(dwLen));
				//printf("Read dwLen=%d m_pszReadFileName=%s \n", dwLen, m_pszReadFileName);
				if(nResult <=0 )
				{
					printf("Read Error \n");
					break;
				}
				m_File.Seek(dwLen, SEEK_CUR);
				dwPosition += dwLen;
			}
			if(nResult >= 0)
			{
				nResult = m_File.Read(&dwLen, 	sizeof(dwLen));
				nResult = m_File.Read(pData, 	dwLen);
			}
			m_File.Close();
		}
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : int	ReadS(int nType,int	nStartIndex,int nIndex, int nSize, void* pData)|
|  Task     : 讀取指定類型和指定個數資料  						     	     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
|  					nStartIndex           - 起始Index                        |
| 				    nCount            -     資料筆數                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :  0=未讀到任何資料 >0 讀到資料 <0 讀取失敗                      |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::ReadS(int	nType, int	nStartIndex,	int nCount, int nSize, void* pData)
{
	int	nResult = 0, nFileCount = 0, nCurrentCount = 0, nCurrentFileStart = 0, nCurrentFileCount = 0;
	DWORD	dwLen = 0, dwPosition = 0;
	if(pData != NULL)
	{
		nFileCount = nCount/m_dwMaxCount;
		
		nCurrentFileStart = nStartIndex%m_dwMaxCount;
		//printf("nFileCount=%d \n", nFileCount);
		for(int i = 0; i <= nFileCount; i++)
		{
			CalculateReadFileName(nStartIndex);
			if(nCurrentCount < nCount)
			{
				//printf("count 1=%d count2=%d \n", nCount - nCurrentCount, m_dwMaxCount-nCurrentFileStart);
				if((nCount - nCurrentCount) <= (m_dwMaxCount-nCurrentFileStart))
				{
					nCurrentFileCount = nCount - nCurrentCount;
					nCurrentCount += nCount;
				}
				else
				{
					nCurrentCount += m_dwMaxCount-nCurrentFileStart;
					nCurrentFileCount =m_dwMaxCount-nCurrentFileStart;
					nFileCount +=1;
					nStartIndex +=nCurrentCount;
				}
			}
			//printf("nCurrentCount=%d nFileCount=%d nCurrentFileStart=%d  \n", nCurrentCount, nFileCount, nCurrentFileStart);
			//printf("m_pszReadFileName=%s \n", m_pszReadFileName);
			if(m_File.Open(m_pszReadFileName, O_RDONLY) == tmFILE_GOOD)
			{
				m_File.Seek(DATAPROCESS_HEAD_SIZE, SEEK_SET);
				//dwPosition += DATAPROCESS_HEAD_SIZE;
				for(int i = 0; i < nCurrentFileStart; i++)
				{
					nResult = m_File.Read(&dwLen, 	sizeof(dwLen));
					if(nResult <=0 )
					{
						printf("Read Error \n");
						break;
					}
					m_File.Seek(dwLen, SEEK_CUR);
					//dwPosition += dwLen;
				}
				//printf("m_pszReadFileName =%s nResult=%d nCount=%d nCurrentFileCount=%d\n", m_pszReadFileName, nResult, nCount, nCurrentFileCount);
				if(nResult >= 0)
				{
					for(int j = 0; j < nCurrentFileCount ; j++)
					{
						nResult = m_File.Read(&dwLen, 	sizeof(dwLen));
						nResult = m_File.Read((BYTE*)pData + dwPosition, 	dwLen);
						//printf("dwLen=%d dwPosition=%d nResult=%d \n", dwLen, dwPosition, nResult);
						if(nResult <=0 )
						{
							printf("Read Error 2 \n");
							break;
						}
						dwPosition += dwLen;
					}
					
				}
				nCurrentFileStart = 0;
				m_File.Close();
			}
		}
	}
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : int	Write(int nType,int nIndex, int nSize, void* pData)      |
|  Task     : 設定指定類型資料  						     	     		 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
| 				    nIndex            -     資料序號                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :  0=未寫入任何資料 >0 寫入資料成功 <0 寫入失敗                  |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::Write(int	nType, int	nIndex, int nSize, void* pData)
{
	int	nResult = 0;
	DWORD	dwLen = 0, dwPosition = 0;
	char	cszFileName[256];
	printf("WriteFileName=%s pData=%x m_dwCount=%d m_dwMaxCount=%d m_dwFileIndex=%d nSize=%d \n", m_pszFileName, pData, m_dwCount, m_dwMaxCount, m_dwFileIndex, nSize);
	if(pData != NULL)
	{
		//若當前存儲個數>=當前文件最大個數，則重新產生新文件
		if(m_dwCount >= m_dwMaxCount && m_dwMaxCount != 0)
		{
			if((m_dwFileIndex >= DATAPROCESS_FILE_MAXCOUNT) || (m_dwFileCount >= m_dwMaxCount))
			{
				printf("Write File Count outof rang \n");
				nResult = -1;
				return nResult;
			}
			m_dwFileIndex +=1;
			memset(cszFileName, 0, sizeof(cszFileName));
			memset(m_pszFileName, 0, sizeof(m_pszFileName));
			sprintf(m_pszFileName, "%s%d", m_pszDataFileName, m_dwFileIndex);		//James modify 2010/11/11 下午 10:27:37
			sprintf(cszFileName, "%s%d", m_pszFileName_Config, m_dwFileIndex);
			printf("write cszFileName=%s m_dwFileIndex=%d m_dwFileCount=%d \n",cszFileName,m_dwFileIndex, m_dwFileCount );
			if(WriteConfig(cszFileName) < 0 )		//write full
			{
				nResult = -1;
				return nResult;	
			}
			m_dwCount = 0;
			//WriteConfig(m_pszFileName);
		}
		
		//判斷文件夾是否存在，若不存在則創建
		if(0 != CheckConfigFile(m_pszConfigName, nType))
		{
			printf("Write DirectFile error \n");
			nResult = -1;
			return nResult;
		}
		if(0 != CheckDataFile(m_pszFileName, nType))
		{
			printf("Write Datafile error \n");
			nResult = -1;
			return nResult;
		}
		
		if(m_File.Open(m_pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
		{
			//index=0 表示追加到最後寫入
			if(nIndex == 0)
			{
				dwLen = m_File.Seek(0, SEEK_END);
				m_File.Write(&nSize, sizeof(DWORD));
				nResult = m_File.Write(pData, nSize);
				m_dwCount += 1;
				m_File.Seek(DATAPROCESS_HEAD_COUNT_POS, SEEK_SET);
				m_File.Write(&m_dwCount, 	sizeof(m_dwCount));
			}
			else if(nIndex > 0)		//index > 0 表示從指定序號插入
			{
				//printf("m_dwCount=%d nIndex=%d nSize=%d dwPosition=%d \n", m_dwCount, nIndex, nSize, dwPosition);
				m_File.Seek(DATAPROCESS_HEAD_SIZE, SEEK_SET);
				for(int i = 0; i < nIndex; i++)
				{
					nResult = m_File.Read(&dwLen, 	sizeof(dwLen));
					if(nResult <=0 || dwLen > nSize * DATAPROCESS_FILEDATA_MAXCOUNT)
					{
						printf("Write Error \n");
						m_File.Close();
						return nResult;
					}
					//printf("dwLen=%d \n", dwLen);
					m_File.Seek(dwLen, SEEK_CUR);
					dwPosition += dwLen;
				}
				if(nResult > 0)
				{
					m_File.Write(&dwLen, sizeof(DWORD));
					nResult = m_File.Write(pData, 	nSize);
					m_dwCount += 1;
					m_File.Seek(DATAPROCESS_HEAD_COUNT_POS, SEEK_SET);
					m_File.Write(&m_dwCount, 	sizeof(m_dwCount));
				}	
			}
			else
			{
				;
			}
			m_File.Close();
		}
		
	}
	//printf("nResult=%d \n", nResult);
	return nResult;
}

/*---------------------------------------------------------------------------+
|  Function : int	WriteS(int nType,int	nStartIndex,int nIndex, int nSize, void* pData)   |
|  Task     : 設定指定類型和指定筆數資料  						     	     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:       nType             -     資料類型                         |
|  					nStartIndex           - 起始Index                        |
| 				    nCount            -     資料筆數                         |
|  					nSize                -  資料大小                         |
|  					pData                 - 資料存儲地址                     |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmDataProcessSave::WriteS(int	nType, int	nStartIndex,	int nCount, int nSize,	void* pData)
{
	int	nResult = 0;
	DWORD	dwLen = 0, dwPosition = 0;
	char	cszFileName[256];
	if(pData != NULL)
	{
		//若當前存儲格式>=當前文件最大個數，則重新產生新文件
		if(m_dwCount >= m_dwMaxCount && m_dwMaxCount != 0)
		{
			m_dwFileIndex +=1;
			memset(cszFileName, 0, sizeof(cszFileName));
			memset(m_pszFileName, 0, sizeof(m_pszFileName));
			sprintf(m_pszFileName, "%s%d", m_pszDataFileName, m_dwFileIndex);
			sprintf(cszFileName, "%s%d", m_pszFileName_Config, m_dwFileIndex);
			WriteConfig(cszFileName);
			m_dwCount= 0;
			//WriteConfig(m_pszFileName);
		}
		//判斷文件夾是否存在，若不存在則創建
		if(0 != CheckConfigFile(m_pszConfigName, nType))
		{
			printf("Write DirectFile error \n");
			nResult = -1;
			return nResult;
		}
		if(0 != CheckDataFile(m_pszFileName, nType))
		{
			printf("Write Datafile error \n");
			nResult = -1;
			return nResult;
		}
		//if(m_File.Open(m_pszFileName, O_RDWR) == tmFILE_GOOD)
		//{
		//	m_File.Seek(DATAPROCESS_HEAD_SIZE, SEEK_SET);
		//}
		//else
		//{
		//	if(m_File.Open(m_pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
		//	{
		//		m_dwType = nType;
		//		m_dwMode = GetMode(nType);
		//		m_dwCount= 0;
		//		m_File.Write(&m_dwType, 	sizeof(m_dwType));
		//		m_File.Write(&m_dwMode, 	sizeof(m_dwMode));
		//		m_File.Write(&m_dwCount, 	sizeof(m_dwCount));
		//	}
		//	else
		//	{
		//		return nResult;	
		//	}
		//}
		if(m_File.Open(m_pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
		{
			if(nStartIndex == 0)			//寫入到最後
			{
				dwLen = m_File.Seek(0, SEEK_END);
				for(unsigned int i = 0; i < nCount; i++)
				{
					//若當前存儲格式>=當前文件最大個數，則重新產生新文件
					if(m_dwCount >= m_dwMaxCount && m_dwMaxCount != 0)
					{
						m_File.Close();
						m_dwFileIndex +=1;
						memset(m_pszFileName, 0, sizeof(m_pszFileName));
						sprintf(m_pszFileName, "%s%d", m_pszDataFileName, m_dwFileIndex);
						//WriteConfig(m_pszFileName);
						memset(cszFileName, 0, sizeof(cszFileName));
						sprintf(cszFileName, "%s%d", m_pszFileName_Config, m_dwFileIndex);
						WriteConfig(cszFileName);
						
						
						if(0 != CheckConfigFile(m_pszConfigName, nType))
						{
							printf("Write DirectFile error \n");
							nResult = -1;
							return nResult;
						}
						if(0 != CheckDataFile(m_pszFileName, nType))
						{
							printf("Write Datafile error \n");
							nResult = -1;
							return nResult;
						}
						if(m_File.Open(m_pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
							dwLen = m_File.Seek(0, SEEK_END);
						else
						{
							nResult = -1;
							return nResult;
						}
					}
					
					m_File.Write(&nSize, sizeof(DWORD));
					nResult = m_File.Write((BYTE*)pData+nSize, nSize);
					if(nResult <= 0)
					{
						printf("Write Error 2 \n");
						break;	
					}
					else
						m_dwCount++;
					dwPosition += nSize;
				}
			}
			else if(nStartIndex > 0)
			{
				for(int i = 0; i < nStartIndex-1; i++)
				{
					nResult = m_File.Read(&dwLen, 	sizeof(dwLen));
					if(nResult <=0 )
					{
						printf("Write Error \n");
						break;
					}
					m_File.Seek(dwLen, SEEK_CUR);
				}
				if(nResult > 0)
				{
					for(int j = 0; j < nCount; j++)
					{
						//若當前存儲格式>=當前文件最大個數，則重新產生新文件
						if(m_dwCount >= m_dwMaxCount && m_dwMaxCount != 0)
						{
							m_File.Close();
							m_dwFileIndex +=1;
							memset(m_pszFileName, 0, sizeof(m_pszFileName));
							sprintf(m_pszFileName, "%s%d", m_pszDataFileName, m_dwFileIndex);
							//WriteConfig(m_pszFileName);
							memset(cszFileName, 0, sizeof(cszFileName));
							sprintf(cszFileName, "%s%d", m_pszFileName_Config, m_dwFileIndex);
							WriteConfig(cszFileName);
							
							if(0 != CheckConfigFile(m_pszConfigName, nType))
							{
								printf("Write DirectFile error \n");
								nResult = -1;
								return nResult;
							}
							if(0 != CheckDataFile(m_pszFileName, nType))
							{
								printf("Write Datafile error \n");
								nResult = -1;
								return nResult;
							}
							if(m_File.Open(m_pszFileName, O_RDWR | O_CREAT) == tmFILE_GOOD)
								dwLen = m_File.Seek(0, SEEK_END);
							else
							{
								nResult = -1;
								return nResult;
							}
						}
						
						m_File.Write(&nSize, 	sizeof(DWORD));
						nResult = m_File.Write((BYTE*)pData + dwPosition, 	nSize);
						if(nResult <= 0)
						{
							printf("Write Error 2 \n");
							break;	
						}
						else
							m_dwCount++;
						dwPosition += nSize;
					}
				}	
			}
			else
			{
					
			}
			m_File.Close();
		}
		
	}
	return nResult;
}

int		CtmDataProcessSave::GetMode(int nType)
{
	return u_DataMode[nType-1];
}

int		CtmDataProcessSave::CalculateWriteFileName(int	nIndex)
{
	int	nTempIndex = 0, nFileNub = 0;
	
	printf("CalculateWriteFileName m_pszDataFileName=%s \n", m_pszDataFileName);
	if(nIndex <= 0)
	{
		memset(m_pszFileName, 0, sizeof(m_pszFileName));
		sprintf(m_pszFileName, "%s%d", m_pszDataFileName, 0);	
	}
	else
	{
		if(m_dwMaxCount != 0 )
		{
			nFileNub 	= nIndex/m_dwMaxCount;
			nTempIndex 	= nIndex%m_dwMaxCount;
		}
		sprintf(m_pszFileName, "%s%d", m_pszDataFileName, nFileNub);
	}
	return nFileNub;
}

int		CtmDataProcessSave::CalculateReadFileName(int	nIndex)
{
	int	nTempIndex = 0, nFileNub = 0;
	
	if(nIndex <= 0)
	{
		sprintf(m_pszReadFileName, "%s%d", m_pszDataFileName, 0);	
	}
	else
	{
		if(m_dwMaxCount != 0 )
		{
			nFileNub 	= nIndex/m_dwMaxCount;
			nTempIndex 	= nIndex%m_dwMaxCount;
		}
		//printf("maxcount=%d, nFileNub=%d, nTempIndex=%d \n", m_dwMaxCount, nFileNub, nTempIndex);
		sprintf(m_pszReadFileName, "%s%d", m_pszDataFileName, nFileNub);
	}
	return nFileNub;
}

int		CtmDataProcessSave::GetAllCount()
{
	return m_dwAllCount;
}

//==============================================================================
//DataProcessRead
//==============================================================================
//CtmDataProcessRead::CtmDataProcessRead()
//{
//	
//}
//
//CtmDataProcessRead::~CtmDataProcessRead()
//{
//	
//}

//int		CtmDataProcessRead::Read_Config()
//{
//	int	nResult = 0;
//	
//	return nResult;	
//}
//
//int		CtmDataProcessRead::Read_Data()
//{
//	int	nResult = 0;
//	
//	return nResult;	
//}



#ifndef	D_BEOBJECT_CTMTASkDATAPROCESS
CObject*	CreateInstance(char* Name)
{
	CObject*	pResult = Create(Name);
	
	if (pResult != NULL)
	{
		(static_cast<CTaskDataProcess*>(pResult))->CreateSelf();
	}
	return pResult;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskDataProcess*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
