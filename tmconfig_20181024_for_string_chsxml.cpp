/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmConfig											             |
|  Task     : build the correct interfaces			                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision : 							                                     |
+===========================================================================*/
#include	"tmconfig.h"
#include	"commonaction.h"

/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
const	char*	PSZ_CONFIG_FILENAME				= "config.ini";
const	char*	PSZ_CONFIG_SRAM					= "consram.ini";
const	char*	INI_SECTION_CONFIG				= "config";
const	char*	INI_KEY_CONIFG_NAME				= "Path";
const	char*	INI_KEY_LANGUAGECONIFG_NAME		= "Languageconf";
const	char*	INI_KEY_LOGOCONIFG_NAME			= "Logoconf";
const	char*   INI_KEY_INETCONFIG_NAME			= "iNetconf";
const	char*	INI_KEY_REMOTE_NAME				= "Remote";
const	char*	INI_KEY_FUNCTION_NAME			= "Function";
const	char*	INI_KEY_DATAPROCESS_NAME		= "Dataprocess";


                             
const	char*	INI_SECTION_SYSTEM_NAME			= "system";
const	char*	INI_SECTION_LCD_NAME			= "LCD";
const	char*	INI_SECTION_LANGUAGE_NAME		= "language";
const	char*	INT_SECTION_LANGUAGEX_NAME_EX	= "language";         
const	char*	INI_SECTION_LANGUAGE_BMPPATH	= "LanguageBmpPath";
const	char*	INI_SECTION_CURVE_NAME			= "curve";    
const	char*	INI_SECTION_MOLDSET_NAME		= "moldset";     
const	char*	INI_SECTION_RECORDSET_NAME		= "recordset";     
const	char*	INI_SECTION_ERROR_NAME			= "error";      
const	char*	INI_SECTION_MONI_NAME			= "moni";
const	char*	INI_SECTION_DATAPROCESS_NAME	= "dataprocess";
const	char*	INI_SECTION_DATABASE_NAME		= "database";//Sunny<20070622>
const	char* 	INI_SECTION_MODBUS_NAME			= "modbus";
const	char*	INI_KEY_MODBUSTYPE_NAME			= "ModbusType";
//	system configs
const	char*	INI_KEY_MAINFORM_NAME			= "MainForm";				//	主窗體的名稱
const	char*	INI_KEY_FORMPATH_NAME			= "FormPath";				//	窗體的路徑名
const	char*	INI_KEY_HELPPATH_NAME			= "HelpPath";				//  在線幫助路徑名
const	char*	INI_KEY_FORMLIB_NAME			= "FormlibPath";			//	窗體處理邏輯的路徑名
const	char*	INI_KEY_TOOLBARPATH_NAME		= "ToolBarPath";			//	toolbar的路徑
const	char*	INI_KEY_CURRENTLANGUAGE_NAME	= "CurrentLanguage";		//	系統目前的語言
const	char*	INI_KEY_LOGOFORM_NAME			= "LogoForm";				//	Logoform的名稱
const	char*	INI_KEY_LOGOPATH_NAME			= "LogoPath";
const	char*	INI_KEY_GIFLOGOTYPE_NAME		= "GifType";
const	char*	INI_KEY_MACHINETYPE_NAME		= "MachineType";			//	machine's type; J6, Sim etc.
const	char*	INI_KEY_SRAMESIZE_NAME			= "SramSize";				//	sram's size
const	char*	INI_KEY_SRAM_INITSTATE_NAME		= "SramInitState";			//	sram's state
const	char*	INI_KEY_FLASHEX_INITSTATE_NAME	= "FlashEXInitState";		//	FlashEX狀態
const	char*	INI_KEY_LCD_INITSTATE_NAME		= "LCDParaInitState";		//	LCDPara狀態
const	char*	INI_KEY_BMPPATH_NAME			= "BmpPath";				//	bmp's path
const	char*	INI_KEY_INETFLAG_NAME			= "iNetFlag";				//  iNet Flag
const	char*	INI_KEY_INETMACSWITCH_NAME		= "iNetMac";				//  iNet Mac Switch
const	char*	INI_KEY_DSP28FLAG_NAME			= "Dsp28Flag";				//  Dsp28 Flag
const	char*	INI_KEY_PICKERFLAG_NAME			= "PickerFlag";				//  Picker Flag
const	char*	INI_KEY_AUSTONEFLAG_NAME		= "AustoneFlag";			//	Austone Flag
const	char*	INI_KEY_N2CFLAG_NAME			= "N2CFlag";				//	N2C Flag	Caozz add 2010/10/21 09:40上午
const	char*	INI_KEY_M3FLAG_NAME				= "M3Flag";					//	M3 Flag	 add 2010/10/21 09:40
const	char*	INI_KEY_TEMPCURVEFLAG_NAME		= "TempCurveFlag";			//	TempCurve Flag
const	char*	INI_KEY_KEYFLAG_NAME			= "KeyFlag";				//  Key Flag
const	char*	INI_KEY_KEYLEDFLAG_NAME			= "KeyLedFlag";				//  Key led Flag
const	char*	INI_KEY_CONFIGRAMPDEAL_NAME		= "ConfigDealRamp";			//  ConfigDealRamp
const	char*	INI_KEY_LCDSIZE_NAME			= "LCDSize";				//  LCD Size
const	char*	INI_KEY_LCDHEIGHT_NAME			= "LCDHeight";				//	LCD Height
const	char*	INI_KEY_LCDWIDTH_NAME			= "LCDWidth";				//	LCD Width
const	char*	INI_KEY_KEYLOACKFLAG_NAME		= "KeyLockFlag";			//  KeyLock Flag
const	char*	INI_KEY_DSPCOM2FLAG_NAME		= "DspCom2Flag";			//  KeyLock Flag
const	char*	INI_KEY_LOGOFLAG_NAME			= "LogoFlag";				//	logo flag
const	char*	INI_KEY_DEMOFLAG_NAME			= "DemoFlag";				//	Demo flag
const	char*	INI_KEY_SLAVEFLAG_NAME			= "SlaveFlag";				//	Slave flag
const	char*	INI_KEY_TIMEFLAG_NAME			= "PrintTimeFlag";			//  print form show time flag
const	char*	INI_KEY_PRIVSWITCH_NAME			= "PrivSwitch";	
const	char*	INI_KEY_CANVASFLAG_NAME			= "CanvasFlag";
const	char*	INI_KEY_MAINFRAMETYPE_NAME		= "MainFrameType";			//	mainframe type
const	char*	INI_KEY_COMMTYPESWITCH_NAME		= "CommTypeSwitch";			//	comm type switch
const   char*   INI_KEY_AMMETERFLAG_NAME        = "AmmeterFlag";            //  AmmeterFlag
const	char*	INI_KEY_TOUCHPANLTYPE_NAME		= "TouchPanlType";			//	touchpanl type
const	char*	INI_KEY_OKKEY_NAME				= "OkKey";					//	ok
const	char*	INI_KEY_BINARY_NAME				= "BinaryType";				//	Binary Use
const	char*	INI_KEY_STOPPWD_NAME			= "StopPWDType";			//	stop machine pwd
const	char*	INI_KEY_REMOTEFLAG_NAME			= "RemoteFlag";				//	remote flag
const	char*	INI_KEY_REMOTEIMAGETYPE_NAME	= "RemoteImageType";
const	char*	INI_KEY_REMOTEMACSWITCH_NAME	= "RemoteMac";				//Remote Mac Switch
const	char*	INI_KEY_INETPRIVILEGE			= "iNetPrivilege";			// get iNet privilege
const	char*	INI_KEY_INET_FONTTYPE			= "iNetFontType";			// Set font type give iNet rw.ying
const	char*	INI_KEY_ERRORCONTROL			= "ErrorControl";			// Caozz 2011/4/7 上午 09:59:20
const	char*	INI_KEY_COM2TYPE_NAME			= "Com2Type";				// 0=MWcom2 1= two dsp james add 2011/4/8 上午 09:36:29
const	char*	INI_KEY_HMICOMMTYPE_NAME		= "HmiCommFlag";			// 0 nouse hmicomm 1:use hmicomm
const	char*	INI_KEY_LOGOTIME_NAME			= "LogoTime";				// logo screen residence time
const	char*	INI_KEY_ENERGYMAXNUM_NAME		= "EnergyMaxNum";			// energy max num
const	char*	INI_KEY_CANTYPE_NAME			= "CanType";				// CanType
const	char*	INI_KEY_MACHINEAXIS_NAME		= "MachineAxis";
const	char*	INI_KEY_CANKEYBOARDNUMBER_NAME	= "CANIndex";				// CAN number
const	char*	INI_KEY_RECORDTYPE_NAME			= "RecordType";
const	char*	INI_KEY_RECORDFILEPATH_NAME		= "RecordFilePath";
const	char*	INI_KEY_INET_HOSTTYPE			= "iNetHostType";
const	char*	INI_KEY_TIMERTASKTYPE_NAME		= "TimerTaskType";
const	char* 	INI_KEY_MAXCURVENUM_NAME		= "MaxCurveNum";
const	char*	INI_KEY_FOCUSCOLOR_NAME			= "FocusColor";
//LCD Para
const	char*	INI_KEY_SIGNAL_NAME				= "signal";
const	char*	INI_KEY_PIXCLOCK_NAME			= "pixclock";
const	char*	INI_KEY_LCCR0_NAME				= "lccr0";
const	char*	INI_KEY_LCCR3_NAME				= "lccr3";
const	char*	INI_KEY_XRES_NAME				= "xres";
const	char*	INI_KEY_YRES_NAME				= "yres";
const	char*	INI_KEY_HSYNC_LEN_NAME			= "hsync_len";
const	char*	INI_KEY_VSYNC_LEN_NAME			= "vsync_len";
const	char*	INI_KEY_LEFT_MARGIN_NAME		= "left_margin";
const	char*	INI_KEY_UPPER_MARGIN_NAME		= "upper_margin";
const	char*	INI_KEY_RIGHT_MARGIN_NAME		= "right_margin";
const	char*	INI_KEY_LOWER_MARGIN_NAME		= "lower_margin";
const	char*	INI_KEY_SYNC_NAME				= "sync";
const	char*	INI_KEY_RESERVED_NAME			= "reserved";


//	language
const	char*	INI_KEY_LANGUAGEPATH_NAME		= "LanguagePath";			//	多國語言的路徑
const	char*	INI_KEY_LANGUAGEPATH_HELP_NAME	= "LanguagePathHelp";		// for online helper
const	char*	INI_KEY_FONTCODE_NAME			= "FontCode";				//	字體編碼的開始                              
const	char*	INI_KEY_FONTSIZE_NAEM			= "FontSize";				//	字體的大小     	
const	char*	INI_KEY_SHOWFONTSIZE_NAEM		= "ShowFontSize";			//  顯示字體大小	//James add 2011/8/2 
const	char*	INI_KEY_FONTFILE_NAME			= "Font";    				//	點整文件       
const	char*	INI_KEY_LANGUAGE_NAME			= "Language";				//	語言文件       
const	char*	INI_KEY_LANGUAGETYPE_NAME		= "LanguageType";			//	語言文件       
const	char*	INI_KEY_LANGUAGEPATCH_NAME		= "LanguagePatch";
const	char*	INI_KEY_FONT_CONVERT			= "Font_Convert";
const	char*	INI_KEY_RECORDXML_FILENAME		= "Language_record";
const	char*	INI_KEY_VIEWXML_FILENAME		= "Language_view";
const	char*	INI_KEY_FONTFT_FILENAME			= "Font_Freetype";


const	char*	INI_SECTION_ENGLISH_NAME		= "language0_font";			//	英文的配置信息
//	curve configs
const	char*	INI_KEY_DSPCURVE_NAME			= "DspCurve";
const	char*	INI_KEY_SIMCURVE_NAME			= "SimCurve";

//	mold set configs
const	char*	INI_KEY_HEAD_NAME				= "HeadFile";
const	char*	INI_KEY_CONTENT_NAME			= "ContentFile";
const	char*	INI_KEY_MOLD_MAX_NAME			= "MaxMold";

//	modified record set configs
const	char*	INI_KEY_RECORD_MAX_NAME			= "MaxRecord";
const	char*	INI_KEY_RECORD_FILE_NAME		= "FileName";

//	error configs
const	char*	INI_KEY_ERROR_MAX_NAME			= "MaxRecord";
const	char*	INI_KEY_ERROR_FILE_NAME			= "FileName";

//	moni configs
const	char*	INI_KEY_MONI_MAX_NAME			= "MaxRecord";
const	char*	INI_KEY_MONI_FILE_NAME			= "FileName";

const	char*	INI_KEY_DATAPROCESS_FLAG_NAME	= "DataProcessFlag";
//	datastate configs
const	char*	INI_KEY_DATASTATE_MAX_NAME		= "StateMaxRecord";
const	char*	INI_KEY_DATASTATE_FILE_NAME		= "StateFileName";

//	dataenergy configs
const	char*	INI_KEY_DATAENERGY_MAX_NAME		= "EnergyMaxRecord";
const	char*	INI_KEY_DATAENERGY_FILE_NAME	= "EnergyFileName";

//	datapoweroff configs
const	char*	INI_KEY_DATAPOWERONOFF_MAX_NAME	= "PowerOnOffMaxRecord";
const	char*	INI_KEY_DATAPOWERONOFF_FILE_NAME= "PowerOnOffFileName";


const	char*	INI_KEY_ONOFFSTATEMAXNUM_NAME	= "OnOffStateMaxNum";
const	char*	INI_KEY_ONOFFSTATE_FILE_NAME	= "OnOffSateFileName";	




//	DB Sunny<20070622>
const	char*	INI_KEY_DB_ID					= "DBID";
const	char*	INI_KEY_DB_NAME				= "DBName";                         
const	char*	INI_KEY_DB_ATTR				= "DBAttr";
const	char*	INI_KEY_DB_DEFAULT				= "DBDefault";
const	char*	INI_KEY_DB_BLOCK				= "DBBlock";
const	char*	INI_KEY_DB_INDEX2ID				= "DBIndex2id";
const	char*	INI_KEY_DB_RECORDS				= "DBRecords";
const	char*	INI_KEY_DB_DBSRAMSIZE			= "DBSramSize";

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//=======
//      Initialize static class member
//=======
CtmConfig*	CtmConfig::m_pInstance = NULL;

/*==========================================================================+
|           Class implementation - CtmConfig		                        |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmConfig::CtmConfig()
{
	
	
	for (int i = 0; i < MAX_LANGUAGE_NUM; i++)
	{
		m_LanguagesConfig.pszLanguages[i] = NULL;
		m_pszLanguages[i] = NULL;
		m_pszLanguagesBmpPath[i] = NULL;
	}
	m_LanguagesConfig.pszCurrentLanguage	= NULL;					//	系統目前的語言
	m_LanguagesConfig.nLanguageType			= 0;
	m_LanguagesConfig.nLanguagePatch		= 0;
	m_SystemConfig.nState					= 0;
	m_SystemConfig.pszConfigPath			= NULL;					//  config.ini的路徑
	m_SystemConfig.pszFormPath 				= NULL;					//	窗體的路徑名
	m_SystemConfig.pszHelpPath 				= NULL;					//	在線幫助路徑名
	m_SystemConfig.pszFormlibPath 			= NULL;					//	窗體處理邏輯的路徑名
	m_SystemConfig.pszMainFormName			= NULL;					//	主窗體的名稱	
	m_SystemConfig.pszToolBarPath			= NULL;
	m_SystemConfig.nAmmeterFlag             = 0;
	m_SystemConfig.nModbusType				= 0;
	m_SystemConfig.nPrivSwitch		=0;
	m_SystemConfig.nOkKey					= 0;					//	ok key fans add 2010-5-6 11:27:25
	m_SystemConfig.nEnergy2MaxNum			= 0;
	
	//m_SystemConfig.nOnOffStateMaxNum		= 0;
	m_LCDParaConfig.nsignal					= 0xFFFFFFFF;
	m_LCDParaConfig.pixclock				= 0xFFFFFFFF;
	m_LCDParaConfig.lccr0					= 0xFFFFFFFF;
	m_LCDParaConfig.lccr3					= 0xFFFFFFFF;
	m_LCDParaConfig.xres					= 0xFFFF;
	m_LCDParaConfig.yres					= 0xFFFF;
	m_LCDParaConfig.hsync_len				= 0xFFFF;
	m_LCDParaConfig.vsync_len				= 0xFFFF;
	m_LCDParaConfig.left_margin				= 0xFFFF;
	m_LCDParaConfig.upper_margin			= 0xFFFF;
	m_LCDParaConfig.right_margin			= 0xFFFF;
	m_LCDParaConfig.lower_margin			= 0xFFFF;
	m_LCDParaConfig.sync					= 0xFFFF;
	m_LCDParaConfig.reserved				= 0xFFFF;
	
	
	m_LanguageConfig.nState					= 0;
	m_LanguageConfig.pszLanguagePath		= NULL;					//	多國語言的路徑
	m_LanguageConfig.pszLanguagePathHelp	= NULL;
	m_LanguageConfig.nFontSize				= 0;
	m_LanguageConfig.nFontConvert			= 0;
	m_LanguagesConfig.nLanguageType			= 0;
	m_LanguagesConfig.nLanguagePatch		= 0;
	m_LanguageConfig.nFontCode				= 0;
	m_LanguageConfig.nShowFontSize			= 0;
	m_LanguageConfig.pszFontFileName		= NULL;					//	點整文件
	m_LanguageConfig.pszLanguageFileName	= NULL;					//	語言文件
	m_LanguageConfig.pszConfigPath			= NULL;
	m_LanguageConfig.pszRecordXMLFileName	= NULL;
	m_LanguageConfig.pszViewXMLFileName		= NULL;
	m_LanguageConfig.pszFontFTFileName		= NULL;

	m_CurveConfig.nState					= 0;
	m_CurveConfig.pszDspCurveFile			= NULL;						//	dsp curve file
	m_CurveConfig.pszSimCurveFile			= NULL;						//	sim curve file
	
	m_MoldSetConfig.nState					= 0;
	m_MoldSetConfig.pszHeadFile				= NULL;						//	moldset's head file
	m_MoldSetConfig.pszContentFile			= NULL;						//	moldset's content file
	
	m_RecordSetConfig.nState				= 0;
	m_RecordSetConfig.nMaxNum				= 0;						//	max num for modified record 
	m_RecordSetConfig.pszFile				= NULL;						//	file
	
	m_ErrorConfig.nState					= 0;
	m_ErrorConfig.nMaxNum					= 0;						//	max num for error
	m_ErrorConfig.pszFile					= NULL;						//	file
	
	m_MoniConfig.nState						= 0;
	m_MoniConfig.nMaxNum					= 0;						//	max num for moni
	m_MoniConfig.pszFile					= NULL;						//	file
	
	m_DataProcessConfig.nDataProcessFlag	= 0;
	m_DataProcessConfig.nStateMaxNum		= 0;						//	max num for moni
	m_DataProcessConfig.pszStateFile		= NULL;						//	file
	
	m_DataProcessConfig.nEnergyMaxNum		= 0;						//	max num for moni
	m_DataProcessConfig.pszEnergyFile		= NULL;						//	file
	
	m_DataProcessConfig.nPowerOnOffMaxNum	= 0;						//	max num for moni
	m_DataProcessConfig.pszPowerOnOffFile	= NULL;						//	file
	
	m_DataProcessConfig.nOnOffStateMaxNum	= 0;						//	max num for moni
	m_DataProcessConfig.pszOnOffStateFile	= NULL;						//	file
	
	m_DBConfig.pszDBIDPathName 				= NULL;		//Sunny<20070622>
	m_DBConfig.pszDBNamePathName 			= NULL;
	m_DBConfig.pszDBAttrPathName 			= NULL;
	m_DBConfig.pszDBDefaultPathName 		= NULL;
	m_DBConfig.pszDBBlockPathName 			= NULL;
	m_DBConfig.pszDBIndex2idPathName		= NULL;
	m_DBConfig.pszDBRecordsPathName			= NULL;
	m_DBConfig.nDBSramSize					= 0;

	m_LogoConfig.nLogoFlag					= 0;
	m_LogoConfig.pszLogoForm				= NULL;
	m_LogoConfig.pszLogoPath				= NULL;
	m_LogoConfig.pszConfigPath				= NULL;
	m_LogoConfig.nLogoTime					= 0;
	m_LogoConfig.nGifType					= 0;

	m_iNetConfig.nInetFlag					= 0;
	m_iNetConfig.nInetMacSwitch				= 0;
	m_iNetConfig.pszConfigPath				= NULL;
	m_iNetConfig.niNetFontType				= 0; 

	m_RemoteConfig.nRemoteFlag				= 0;
	m_RemoteConfig.nRemoteImageType			= 2;
	m_RemoteConfig.nRemoteMacSwitch			= 0;
	m_FunctionConfig.nDspCom2Flag			= 0;
	m_FunctionConfig.nCom2Type				= 0;
	m_FunctionConfig.nDsp28Flag				= 0;
	m_FunctionConfig.nPickerFlag			= 0;
	m_FunctionConfig.nAustoneFlag			= 0;
	m_FunctionConfig.nN2CFlag				= 0;
	m_FunctionConfig.nCommTypeSwitch		= 0;
	m_FunctionConfig.nAmmeterFlag			= 0;
	m_FunctionConfig.nTouchPanlType			= 0;
	m_FunctionConfig.nStopPwdType			= 0;
	m_FunctionConfig.nErrorControl			= 0;
	m_FunctionConfig.nHmiCommFlag			= 0;
	m_FunctionConfig.nEnergy2MaxNum			= 0;
	m_FunctionConfig.nCanType				= 0;
	m_FunctionConfig.nMachineAxis[0]		= 0;
	m_FunctionConfig.nMachineAxis[1]		= 0;
	m_FunctionConfig.nMachineAxis[2]		= 0;
	m_FunctionConfig.nMachineAxis[3]		= 0;
	m_FunctionConfig.nMachineAxis[4]		= 0;
	m_FunctionConfig.nMachineAxis[5]		= 0;
	m_FunctionConfig.nMachineAxis[6]		= 0;
	m_FunctionConfig.nMachineAxis[7]		= 0;
	m_FunctionConfig.nMachineAxis[8]		= 0;
	m_FunctionConfig.nMachineAxis[9]		= 0;
	m_FunctionConfig.nCANIndex 				= 0;
	m_FunctionConfig.nRecordType			= 0;
	m_FunctionConfig.nOnOffStateMaxNum		= 0;
	m_FunctionConfig.pszConfigPath			= NULL;
	
	
	OpenConfigFile();
	
	//	fans add 2009-8-10 9:27:33
	char*	pszTemp = NULL;
	m_SramIni.OpenFile(PSZ_CONFIG_SRAM);
	m_SramIni.ReadString(INI_SECTION_CONFIG, "StatePath", NULL, &pszTemp);
	m_SramIni.CloseFile();
	if (pszTemp != NULL)
	{
		g_StateIni.OpenFile(pszTemp);
		g_StateIni.CloseFile();
		delete []pszTemp;
	}
	
	
	//OpenConfigFile(PSZ_CONFIG_FILENAME);
}

CtmConfig::~CtmConfig()
{

	for (int i = 0; i < MAX_LANGUAGE_NUM; i++)
	{
		if (m_LanguagesConfig.pszLanguages[i] != NULL)
		{
			delete []m_LanguagesConfig.pszLanguages[i];
			m_LanguagesConfig.pszLanguages[i] = NULL;
		}
		
		if (m_pszLanguages[i] != NULL)
		{
			delete []m_pszLanguages[i];
			m_pszLanguages[i] = NULL;
		}
		
		if (m_pszLanguagesBmpPath[i] != NULL)
		{
			delete []m_pszLanguagesBmpPath[i];
			m_pszLanguagesBmpPath[i] = NULL;
		}
	}
	if (m_LanguagesConfig.pszCurrentLanguage!= NULL)
		delete []m_LanguagesConfig.pszCurrentLanguage;

	if (m_SystemConfig.pszBmpPath != NULL)
		delete[] m_SystemConfig.pszBmpPath;
	if (m_SystemConfig.pszFormPath!= NULL)
		delete []m_SystemConfig.pszFormPath;
	if (m_SystemConfig.pszHelpPath!= NULL)
		delete []m_SystemConfig.pszHelpPath;
	if (m_SystemConfig.pszFormlibPath!= NULL)
		delete []m_SystemConfig.pszFormlibPath;	
	if (m_SystemConfig.pszMainFormName!= NULL)
		delete []m_SystemConfig.pszMainFormName;	
	if (m_SystemConfig.pszToolBarPath != NULL)
		delete []m_SystemConfig.pszToolBarPath;

	if(m_LogoConfig.pszLogoForm != NULL)
		delete []m_LogoConfig.pszLogoForm;

	if(m_LogoConfig.pszLogoPath != NULL)
		delete []m_LogoConfig.pszLogoPath;

	if(m_LogoConfig.pszConfigPath != NULL)
		delete []m_LogoConfig.pszConfigPath;

	if(m_iNetConfig.pszConfigPath != NULL)
		delete []m_iNetConfig.pszConfigPath;
	
	if (m_SystemConfig.pszConfigPath != NULL) 
	{
		delete []m_SystemConfig.pszConfigPath;
		m_SystemConfig.pszConfigPath = NULL;
	}

	if(m_FunctionConfig.pszRecordFilePath != NULL)
		delete []m_FunctionConfig.pszRecordFilePath;
	
	if (m_LanguageConfig.pszLanguagePath!= NULL)
		delete []m_LanguageConfig.pszLanguagePath;	
	if (m_LanguageConfig.pszLanguagePathHelp!= NULL)
		delete []m_LanguageConfig.pszLanguagePathHelp;
	if (m_LanguageConfig.pszFontFileName != NULL)
		delete []m_LanguageConfig.pszFontFileName;
	if (m_LanguageConfig.pszLanguageFileName != NULL)
		delete []m_LanguageConfig.pszLanguageFileName;
	if (m_LanguageConfig.pszConfigPath != NULL)
		delete []m_LanguageConfig.pszConfigPath;
		
	if (m_LanguageConfig.pszRecordXMLFileName != NULL)			//James add for freetype 2015/5/5 14:28:58
		delete []m_LanguageConfig.pszRecordXMLFileName;
	if (m_LanguageConfig.pszViewXMLFileName != NULL)
		delete []m_LanguageConfig.pszViewXMLFileName;
	if (m_LanguageConfig.pszFontFTFileName != NULL)
		delete []m_LanguageConfig.pszFontFTFileName;

	if (m_CurveConfig.pszDspCurveFile != NULL) 
		delete []m_CurveConfig.pszDspCurveFile;
	if (m_CurveConfig.pszSimCurveFile != NULL)
		delete []m_CurveConfig.pszSimCurveFile;

	if (m_MoldSetConfig.pszHeadFile != NULL)
		delete []m_MoldSetConfig.pszHeadFile;
	if (m_MoldSetConfig.pszContentFile != NULL)
		delete []m_MoldSetConfig.pszContentFile;
	
	if (m_RecordSetConfig.pszFile != NULL)
		delete []m_RecordSetConfig.pszFile;
	
	if (m_ErrorConfig.pszFile != NULL)
		delete []m_ErrorConfig.pszFile;
	
	if (m_MoniConfig.pszFile != NULL)
		delete []m_MoniConfig.pszFile;
	
	if (m_DBConfig.pszDBIDPathName != NULL)		//Sunny<20070622>
		delete []m_DBConfig.pszDBIDPathName;
	if (m_DBConfig.pszDBNamePathName != NULL)
		delete []m_DBConfig.pszDBNamePathName;
	if (m_DBConfig.pszDBAttrPathName != NULL)
		delete []m_DBConfig.pszDBAttrPathName;
	if (m_DBConfig.pszDBDefaultPathName != NULL)
		delete []m_DBConfig.pszDBDefaultPathName;
	if (m_DBConfig.pszDBBlockPathName != NULL)
		delete []m_DBConfig.pszDBBlockPathName;
	if (m_DBConfig.pszDBIndex2idPathName != NULL)
		delete []m_DBConfig.pszDBIndex2idPathName;
	if (m_DBConfig.pszDBRecordsPathName != NULL)
		delete []m_DBConfig.pszDBRecordsPathName;
	
	
	CloseConfigFile();
	g_StateIni.CloseFile();	//	fans add 2009-8-10 9:35:15

}

CtmConfig*	CtmConfig::GetInstance()
{
	
	if	(m_pInstance == NULL)
	{
		
		m_pInstance = new CtmConfig();
		
	}
	
	return m_pInstance;
}

void	CtmConfig::ReleaseInstance()
{
	if	(m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  OpenConfigFile(const char* pszFileName)						|	
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
bool	CtmConfig::OpenConfigFile()
{
	
	m_SramIni.OpenFile(PSZ_CONFIG_SRAM);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_CONIFG_NAME, NULL, 		&m_SystemConfig.pszConfigPath);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_LANGUAGECONIFG_NAME, NULL, &m_LanguageConfig.pszConfigPath);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_LOGOCONIFG_NAME, NULL, 	&m_LogoConfig.pszConfigPath);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_INETCONFIG_NAME, NULL, 	&m_iNetConfig.pszConfigPath);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_REMOTE_NAME, NULL, 		&m_RemoteConfig.pszConfigPath);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_FUNCTION_NAME, NULL, 		&m_FunctionConfig.pszConfigPath);
	m_SramIni.ReadString(INI_SECTION_CONFIG, INI_KEY_DATAPROCESS_NAME, NULL, 	&m_DataProcessConfig.pszConfigPath);
	m_SramIni.CloseFile();			//james add 
	
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		
	//	get system config
	ReadSystemConfig();	
	ReadLCDParaConfig();	
	//	get curve file config to writing and reading
	ReadCurveConfig();	
	//	get moldset file config
	ReadMoldSetConfig();	
	//	get Record file config
	ReadRecordSetConfig();	
	//	get error file config
	ReadErrorConfig();	
	//	get moni file config
	ReadMoniConfig();	
	//	get dataprocess config
	//ReadDataProcessConfig();
	//	get DB file config
	ReadDBConfig();	//Sunny<20070622>	
	
	//language
	
	if(m_LanguageConfig.pszConfigPath != NULL)
	{	
		if ( access(m_LanguageConfig.pszConfigPath, R_OK) ) 
		{
			delete []m_LanguageConfig.pszConfigPath;
			m_LanguageConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
			strcpy(m_LanguageConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
		}
	}
	else
	{
		m_LanguageConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
		strcpy(m_LanguageConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
	}
	m_IniFile.CloseFile();
	m_IniFile.OpenFile(m_LanguageConfig.pszConfigPath);
	m_nLanguageNum = ReadLanguagesConfig();	
	ReadLanguageConfig();
	//logo
	if(m_LogoConfig.pszConfigPath != NULL)
	{
		if (access(m_LogoConfig.pszConfigPath, R_OK) ) 
		{
			delete []m_LogoConfig.pszConfigPath;
			m_LogoConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
			strcpy(m_LogoConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
		}
	}
	else
	{
		m_LogoConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
		strcpy(m_LogoConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
	}
	m_IniFile.CloseFile();
	m_IniFile.OpenFile(m_LogoConfig.pszConfigPath);
	ReadLogoConfig();
	//iNet	
	if(m_iNetConfig.pszConfigPath != NULL)
	{
		if ( access(m_iNetConfig.pszConfigPath, R_OK) ) 
		{
			delete []m_iNetConfig.pszConfigPath;
			m_iNetConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
			strcpy(m_iNetConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
		}
	}
	else
	{
		m_iNetConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
		strcpy(m_iNetConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
	}
	m_IniFile.CloseFile();
	m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	ReadiNetConfig();
	
	//DataProcess
	if(m_DataProcessConfig.pszConfigPath != NULL)
	{
		if ( access(m_DataProcessConfig.pszConfigPath, R_OK) ) 
		{
			delete []m_DataProcessConfig.pszConfigPath;
			m_DataProcessConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
			strcpy(m_DataProcessConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
		}
	}
	else
	{
		m_DataProcessConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
		strcpy(m_DataProcessConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
	}
	m_IniFile.CloseFile();
	m_IniFile.OpenFile(m_DataProcessConfig.pszConfigPath);
	ReadDataProcessConfig();
	
	//Remote	
	if(m_RemoteConfig.pszConfigPath != NULL)
	{
		if ( access(m_RemoteConfig.pszConfigPath, R_OK) ) 
		{
			delete []m_RemoteConfig.pszConfigPath;
			m_RemoteConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
			strcpy(m_RemoteConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
		}
	}
	else
	{
		m_RemoteConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
		strcpy(m_RemoteConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
	}
	m_IniFile.CloseFile();
	m_IniFile.OpenFile(m_RemoteConfig.pszConfigPath);
	ReadRemoteConfig();
	
	//Function	
	if(m_FunctionConfig.pszConfigPath != NULL)
	{
		if ( access(m_FunctionConfig.pszConfigPath, R_OK) ) 
		{
			delete []m_FunctionConfig.pszConfigPath;
			m_FunctionConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
			strcpy(m_FunctionConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
		}
	}
	else
	{
		m_FunctionConfig.pszConfigPath = new char[strlen(m_SystemConfig.pszConfigPath) + 1];
		strcpy(m_FunctionConfig.pszConfigPath, m_SystemConfig.pszConfigPath);
	}
	m_IniFile.CloseFile();
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	ReadFunctionConfig();
	
	m_IniFile.CloseFile();
	
	return true;
}

/*--------------------------------------------------------------------------+
|  Function :  CloseConfigFile()											|	
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::CloseConfigFile()
{
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetConfigPath()												|	
|  Task     :  													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetConfigPath() const
{
	return m_SystemConfig.pszConfigPath;	
}

//	system config info
/*--------------------------------------------------------------------------+
|  Function :  GetFormPath() const											|	
|  Task     :  Get form path									            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetFormPath() const
{
	//printf("Formpath =%s\n", m_SystemConfig.pszFormPath);
	return m_SystemConfig.pszFormPath;
}

/*--------------------------------------------------------------------------+
|  Function :  SetFormPath(char*	pszPath)								|	
|  Task     :  Set form path									            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetFormPath(char*	pszPath)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nState = 1;
	if (strcmp(pszPath, m_SystemConfig.pszFormPath) == 0) return;
	
	if (m_SystemConfig.pszFormPath != NULL)
	{
		delete []m_SystemConfig.pszFormPath;
		m_SystemConfig.pszFormPath = NULL;
	}
	m_SystemConfig.pszFormPath = new char[strlen(pszPath) + 1];
	strcpy(m_SystemConfig.pszFormPath, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_FORMPATH_NAME, m_SystemConfig.pszFormPath);
	m_IniFile.CloseFile();
}

char*	CtmConfig::GetHelpPath() const
{
	//printf("HelpPath =%s\n", m_SystemConfig.pszHelpPath);
	return m_SystemConfig.pszHelpPath;
}

void	CtmConfig::SetHelpPath(char*	pszPath)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nState = 1;
	if (strcmp(pszPath, m_SystemConfig.pszHelpPath) == 0) return;
	
	if (m_SystemConfig.pszHelpPath != NULL)
	{
		delete []m_SystemConfig.pszHelpPath;
		m_SystemConfig.pszHelpPath = NULL;
	}
	m_SystemConfig.pszHelpPath = new char[strlen(pszPath) + 1];
	strcpy(m_SystemConfig.pszHelpPath, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_HELPPATH_NAME, m_SystemConfig.pszHelpPath);
	m_IniFile.CloseFile();
}

char*	CtmConfig::GetBmpPath() const
{
	return m_SystemConfig.pszBmpPath;
}

void	CtmConfig::SetBmpPath(char* pszPath)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	if (strcmp(pszPath, m_SystemConfig.pszBmpPath) == 0) return;
	
	if (m_SystemConfig.pszBmpPath != NULL)
	{
		delete []m_SystemConfig.pszBmpPath;
		m_SystemConfig.pszBmpPath = NULL;
	}
	m_SystemConfig.pszBmpPath = new char[strlen(pszPath) + 1];
	strcpy(m_SystemConfig.pszBmpPath, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_BMPPATH_NAME, m_SystemConfig.pszBmpPath);
	m_IniFile.CloseFile();
}

char*	CtmConfig::GetLogoPath() const
{
	return m_LogoConfig.pszLogoPath;
}

void	CtmConfig::SetLogoPath(char* pszPath)
{
	m_IniFile.OpenFile(m_LogoConfig.pszConfigPath);
	if (strcmp(pszPath, m_LogoConfig.pszLogoPath) == 0) return;
	
	if (m_LogoConfig.pszLogoPath != NULL)
	{
		delete []m_LogoConfig.pszLogoPath;
		m_LogoConfig.pszLogoPath = NULL;
	}
	m_LogoConfig.pszLogoPath = new char[strlen(pszPath) + 1];
	strcpy(m_LogoConfig.pszLogoPath, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOPATH_NAME, m_LogoConfig.pszLogoPath);
	m_IniFile.CloseFile();
}

char*	CtmConfig::GetLogoForm() const
{
	return m_LogoConfig.pszLogoForm;
}

void	CtmConfig::SetLogoForm(char* pszPath)
{
	m_IniFile.OpenFile(m_LogoConfig.pszConfigPath);
	if (strcmp(pszPath, m_LogoConfig.pszLogoForm) == 0) return;
	
	if (m_LogoConfig.pszLogoForm != NULL)
	{
		delete []m_LogoConfig.pszLogoForm;
		m_LogoConfig.pszLogoForm = NULL;
	}
	m_LogoConfig.pszLogoForm = new char[strlen(pszPath) + 1];
	strcpy(m_LogoConfig.pszLogoForm, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOFORM_NAME, m_LogoConfig.pszLogoForm);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetFormlibPath() const										|	
|  Task     :  Get form lib's path								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetFormlibPath() const
{
	return m_SystemConfig.pszFormlibPath;
}

/*--------------------------------------------------------------------------+
|  Function :  SetFormLibPath(char* pszPath)								|	
|  Task     :  Set form lib's path								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetFormLibPath(char* pszPath)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	if (strcmp(pszPath, m_SystemConfig.pszFormlibPath) == 0) return;
	
	if (m_SystemConfig.pszFormlibPath != NULL)
	{
		delete []m_SystemConfig.pszFormlibPath;
		m_SystemConfig.pszFormlibPath = NULL;
	}
	m_SystemConfig.pszFormlibPath = new char[strlen(pszPath) + 1];
	strcpy(m_SystemConfig.pszFormlibPath, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_FORMLIB_NAME, m_SystemConfig.pszFormlibPath);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetMainFormName() const										|	
|  Task     :  Get mainform's name								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetMainFormName() const
{
	return m_SystemConfig.pszMainFormName;
}

/*--------------------------------------------------------------------------+
|  Function :  SetMainFormName(char* pszPath)								|	
|  Task     :  Set mainform's name								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetMainFormName(char* pszPath)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	if (strcmp(pszPath, m_SystemConfig.pszMainFormName) == 0) return;
	
	if (m_SystemConfig.pszMainFormName != NULL)
	{
		delete []m_SystemConfig.pszMainFormName;
		m_SystemConfig.pszMainFormName = NULL;
	}
	m_SystemConfig.pszMainFormName = new char[strlen(pszPath) + 1];
	strcpy(m_SystemConfig.pszMainFormName, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFORM_NAME, m_SystemConfig.pszMainFormName);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetToolBarPath() const										|	
|  Task     :  Get toolbar's path								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetToolBarPath() const
{
	return m_SystemConfig.pszToolBarPath;	
}

/*--------------------------------------------------------------------------+
|  Function :  SetToolBarPath(char* pszPath)								|	
|  Task     :  Set toolbar's path								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetToolBarPath(char* pszPath)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	if (strcmp(pszPath, m_SystemConfig.pszToolBarPath) == 0) return;
	
	if (m_SystemConfig.pszToolBarPath != NULL)
	{
		delete []m_SystemConfig.pszToolBarPath;
		m_SystemConfig.pszToolBarPath = NULL;
	}
	m_SystemConfig.pszToolBarPath = new char[strlen(pszPath) + 1];
	strcpy(m_SystemConfig.pszToolBarPath, pszPath);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_TOOLBARPATH_NAME, m_SystemConfig.pszToolBarPath);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetCurrentLanguage() const									|	
|  Task     :  Get current language's ident						            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetCurrentLanguage() const
{
	return m_LanguagesConfig.pszCurrentLanguage;
}

/*--------------------------------------------------------------------------+
|  Function :  SetCurrentLanguage(char* pszStr)								|	
|  Task     :  Set current language's ident						            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetCurrentLanguage(char* pszStr)
{
	m_IniFile.OpenFile(m_LanguageConfig.pszConfigPath);
	if (strcmp(pszStr, m_LanguagesConfig.pszCurrentLanguage) == 0) return;
	
	if (m_LanguagesConfig.pszCurrentLanguage != NULL)
	{
		delete []m_LanguagesConfig.pszCurrentLanguage;
		m_LanguagesConfig.pszCurrentLanguage = NULL;
	}
	m_LanguagesConfig.pszCurrentLanguage = new char[strlen(pszStr) + 1];
	strcpy(m_LanguagesConfig.pszCurrentLanguage, pszStr);
	m_IniFile.WriteString(INI_SECTION_SYSTEM_NAME, INI_KEY_CURRENTLANGUAGE_NAME, m_LanguagesConfig.pszCurrentLanguage);
	 
	ReadLanguageConfig();
	//	需要將英文改成當前的信息(這是由於每個點整,都自動包含了英文的點整文件)
//	m_IniFile.WriteString(INI_SECTION_ENGLISH_NAME, INI_KEY_FONTFILE_NAME, m_LanguageConfig.pszFontFileName);
//	m_IniFile.WriteString(INI_SECTION_ENGLISH_NAME, INI_KEY_LANGUAGEPATH_NAME, m_LanguageConfig.pszLanguagePath);
//	m_IniFile.WriteString(INI_SECTION_ENGLISH_NAME, INI_KEY_LANGUAGEPATH_HELP_NAME, m_LanguageConfig.pszLanguagePathHelp);
//	m_IniFile.WriteInteger(INI_SECTION_ENGLISH_NAME,INI_KEY_FONTCODE_NAME, m_LanguageConfig.nFontCode);
//	m_IniFile.WriteInteger(INI_SECTION_ENGLISH_NAME,INI_KEY_FONTSIZE_NAEM, m_LanguageConfig.nFontSize);
//	m_IniFile.WriteInteger(INI_SECTION_ENGLISH_NAME,INI_KEY_FONT_CONVERT, m_LanguageConfig.nFontConvert);
//	m_IniFile.WriteInteger(INI_SECTION_ENGLISH_NAME,INI_KEY_LANGUAGETYPE_NAME, m_LanguagesConfig.nLanguageType);
	
	m_IniFile.CloseFile();
}




/*--------------------------------------------------------------------------+
|  Function :  GetMachineType()	const										|	
|  Task     :  Get machine type									            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetMachineType()	const
{
	return	m_SystemConfig.nMachineType;
}

/*--------------------------------------------------------------------------+
|  Function :  SetMachineType(int nType)									|	
|  Task     :  Set machine type									            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetMachineType(int nType)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nMachineType = nType;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MACHINETYPE_NAME, nType);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetSramSize() const											|	
|  Task     :  Get sram size									            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetSramSize() const
{
	return m_SystemConfig.nSramSize;
}

/*--------------------------------------------------------------------------+
|  Function :  GetSramInit() const											|	
|  Task     :  Get sram init state								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetSramInit() const
{
	return m_SystemConfig.nSramInit;	
}

/*--------------------------------------------------------------------------+
|  Function :  SetSramInit(int nInit)										|	
|  Task     :  Set sram init state								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetSramInit(int nInit)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	//printf("PATH=%s\n",m_SystemConfig.pszConfigPath);
	m_SystemConfig.nSramInit = nInit;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_SRAM_INITSTATE_NAME, nInit);
	m_IniFile.CloseFile();
}


/*--------------------------------------------------------------------------+
|  Function :  GetFlashEXInit() const										|	
|  Task     :  Get sram init state								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetFlashEXInit() const
{
	return m_SystemConfig.nFlashEXInit;	
}


/*--------------------------------------------------------------------------+
|  Function :  SetFlashEXInit(int nInit)										|	
|  Task     :  Set sram init state								            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetFlashEXInit(int nInit)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nFlashEXInit = nInit;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_FLASHEX_INITSTATE_NAME, nInit);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetLogoFlag() const
{
	return m_LogoConfig.nLogoFlag;	
}
		
void	CtmConfig::SetLogoFlag(int nData)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LogoConfig.nLogoFlag = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOFLAG_NAME, nData);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetLogoTime() const
{
	return m_LogoConfig.nLogoTime;	
}
		
void	CtmConfig::SetLogoTime(int nData)
{
	m_IniFile.OpenFile(m_LogoConfig.pszConfigPath);
	m_LogoConfig.nLogoTime = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOTIME_NAME, nData);
	m_IniFile.CloseFile();
}

int 	CtmConfig::GetSlaveFlag() const
{
		return m_SystemConfig.nSlaveFlag;  //0: 24 temperature channels 1:48 temperature channels
}

void	CtmConfig::SetSlaveFlag(int nData) 
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nSlaveFlag = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_SLAVEFLAG_NAME, nData);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetGifType() const
{
	return m_LogoConfig.nGifType;	
}
		
void	CtmConfig::SetGifType(int nData)
{
	m_IniFile.OpenFile(m_LogoConfig.pszConfigPath);
	m_LogoConfig.nGifType = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_GIFLOGOTYPE_NAME, nData);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetDemoFlag() const								//	Logo flag
{
	return m_SystemConfig.nDemoFlag;	
}
int		CtmConfig::GetPrintTimeFlag() const
{
	return m_SystemConfig.nTimeFlag;	
}

void	CtmConfig::SetDemoFlag(int nData)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nDemoFlag = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DEMOFLAG_NAME, nData);
	m_IniFile.CloseFile();
}


int		CtmConfig::GetMainFrameType() const
{
	return m_SystemConfig.nMainFrameType;
}

void	CtmConfig::SetMainFrameType(int nData)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	int nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME);
	printf("nDataFlag=%d\n",nDataFlag);
	if(nDataFlag<=0){
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		printf("nData=%d\n",nData);	
	}
	m_SystemConfig.nMainFrameType = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME, nData);
	m_IniFile.CloseFile();
//	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
//	m_SystemConfig.nMainFrameType = nData;
//	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME, nData);
//	m_IniFile.CloseFile();
}

int		CtmConfig::GetFocusColor() const
{
	return m_SystemConfig.nFocusColor;
}

void	CtmConfig::SetFocusColor(int nColor)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nFocusColor = nColor;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_FOCUSCOLOR_NAME, nColor);
	m_IniFile.CloseFile();
} 

void	CtmConfig::SetPrivSwitch(int nData)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nPrivSwitch = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_PRIVSWITCH_NAME, nData);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetPrivSwitch() const
{
	return m_SystemConfig.nPrivSwitch;
}
int		CtmConfig::GetCanvasFlag() const
{
	return m_SystemConfig.nCanvasFlag;
}

int		CtmConfig::GetCommType() const
{
	return m_FunctionConfig.nCommTypeSwitch;
}

void	CtmConfig::SetCommType(int nData)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nCommTypeSwitch = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COMMTYPESWITCH_NAME, nData);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetAmmeterFlag() const
{
	return 	m_FunctionConfig.nAmmeterFlag;
}

void	CtmConfig::SetAmmeterFlag(int nData)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nAmmeterFlag = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AMMETERFLAG_NAME, nData);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetTouchPanlType() const
{
	return m_FunctionConfig.nTouchPanlType;
}

void	CtmConfig::SetTouchPanlType(int nData)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nTouchPanlType = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TOUCHPANLTYPE_NAME, nData);
	m_IniFile.CloseFile();
	
}

//	fans add 2010-5-6 11:27:33
int		CtmConfig::GetOKKey() const
{
	return m_SystemConfig.nOkKey;
}

void	CtmConfig::SetOKKey(int nData)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nOkKey = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_OKKEY_NAME, nData);
	m_IniFile.CloseFile();
	
}		


int		CtmConfig::GetMaxInjectSpeed() const						//	射出最大值
{
	return m_SystemConfig.nMaxInjectSpeed;
}
		
void	CtmConfig::SetMaxInjectSpeed(int nData)
{
	m_SystemConfig.nMaxInjectSpeed = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, "MaxInjectSpeed", nData);
}
		

int		CtmConfig::GetMaxChargeSpeed() const							//	儲料最大值
{
	return m_SystemConfig.nMaxChargeSpeed;
}
		
void	CtmConfig::SetMaxChargeSpeed(int nData)
{
	m_SystemConfig.nMaxChargeSpeed = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, "MaxChargeSpeed", nData);
}


int		CtmConfig::GetLanguageType() const
{
	return m_LanguagesConfig.nLanguageType;
}

int		CtmConfig::GetLanguagePatch() const
{
	return m_LanguagesConfig.nLanguagePatch;
}

//void	CtmConfig::SetLanguageType(int nType)
//{
//	if (nType == m_LanguagesConfig.nLanguageType) 
//		return;	
//	m_LanguagesConfig.nLanguageType = nType;
//	m_IniFile.OpenFile(m_LanguageConfig.pszConfigPath);
//	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME,INI_KEY_LANGUAGETYPE_NAME, m_LanguagesConfig.nLanguageType);
//	m_IniFile.CloseFile();
//}
/*--------------------------------------------------------------------------+
|  Function :  GetLanguageNum()												|	
|  Task     :  get the num of the language in the program		            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetLanguageNum()
{
	return m_nLanguageNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetFontCode()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetFontCode()
{
	//printf("fontcode =%d\n", m_LanguageConfig.nFontCode);
	return m_LanguageConfig.nFontCode;
}

/*--------------------------------------------------------------------------+
|  Function :  GetLanguagepath()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetLanguagepath()
{
	return m_LanguageConfig.pszLanguagePath;
}

char*	CtmConfig::GetLanguagepathHelp()
{
	return m_LanguageConfig.pszLanguagePathHelp;
}
/*--------------------------------------------------------------------------+
|  Function :  GetFontFileName()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetFontFileName()
{
	//printf("%s\n", m_LanguageConfig.pszFontFileName);
	//	需要將英文改成當前的信息(這是由於每個點整,都自動包含了英文的點整文件)
	m_IniFile.WriteString(INI_SECTION_ENGLISH_NAME, INI_KEY_FONTFILE_NAME, m_LanguageConfig.pszFontFileName);
	return m_LanguageConfig.pszFontFileName;
}   

/*--------------------------------------------------------------------------+
|  Function :  GetLanguageFileName()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetLanguageFileName()
{
	return m_LanguageConfig.pszLanguageFileName;	
}

/*--------------------------------------------------------------------------+
|  Function :  GetLanguageStr(int index)									|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetLanguageStr(int index)
{
	if (index < 0 || index >= m_nLanguageNum) return NULL;
	return	m_LanguagesConfig.pszLanguages[index];
}

char*	CtmConfig::GetRecordXMLFileName()
{
	return m_LanguageConfig.pszRecordXMLFileName;	
}

char*	CtmConfig::GetViewXMLFileName()
{
	return m_LanguageConfig.pszViewXMLFileName;	
}

char*	CtmConfig::GetFontFTFileName()
{
	return m_LanguageConfig.pszFontFTFileName;	
}

/*--------------------------------------------------------------------------+
|  Function :  GetMoldSetHeadFileName()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetMoldSetHeadFileName()
{
	return m_MoldSetConfig.pszHeadFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMoldSetMaxNum()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetMoldSetMaxNum()
{
	return m_MoldSetConfig.nMaxNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMoldSetContentFileName()									|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetMoldSetContentFileName()
{
	return m_MoldSetConfig.pszContentFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetRecordSetFileName()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetRecordSetFileName()
{
	return	m_RecordSetConfig.pszFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetRecordSetMaxNum()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetRecordSetMaxNum()
{
	return	m_RecordSetConfig.nMaxNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetErrorFileName()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetErrorFileName()
{
	return m_ErrorConfig.pszFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetErrorMaxNum()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetErrorMaxNum()
{
	return	m_ErrorConfig.nMaxNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMoniFileName()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetMoniFileName()
{
	return m_MoniConfig.pszFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetMoniMaxNum()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetMoniMaxNum()
{
	return m_MoniConfig.nMaxNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataProcessFlag()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetDataProcessFlag()
{
	return m_DataProcessConfig.nDataProcessFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataStateFileName()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetDataStateFileName()
{
	return m_DataProcessConfig.pszStateFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataStateMaxNum()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetDataStateMaxNum()
{
	return m_DataProcessConfig.nStateMaxNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataEnergyFileName()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetDataEnergyFileName()
{
	return m_DataProcessConfig.pszEnergyFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataEnergyMaxNum()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetDataEnergyMaxNum()
{
	return m_DataProcessConfig.nEnergyMaxNum;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataPowerOnOffFileName()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetDataPowerOnOffFileName()
{
	return m_DataProcessConfig.pszPowerOnOffFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDataPowerOnOffMaxNub()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetDataPowerOnOffMaxNum()
{
	return m_DataProcessConfig.nPowerOnOffMaxNum;
}

int		CtmConfig::GetDataEnergy2MaxNum()
{
	return m_FunctionConfig.nEnergy2MaxNum;
}
int		CtmConfig::GetMaxCurveNum()
{
	return m_FunctionConfig.nMaxCurveNum;
}

int		CtmConfig::GetOnOffStateMaxNum()
{
	return m_DataProcessConfig.nOnOffStateMaxNum;
}

int		CtmConfig::GetDataCanType()
{
	return m_FunctionConfig.nCanType;
}

int		CtmConfig::GetMachineAxis(int nIndex)
{
	if(nIndex >=0 && nIndex < 10)
		return m_FunctionConfig.nMachineAxis[nIndex];
	else
		return 0;
}

int		CtmConfig::GetCANIndex()
{
	return m_FunctionConfig.nCANIndex;
}

int		CtmConfig::GetRecordType()
{
	return m_FunctionConfig.nRecordType;
}

/*--------------------------------------------------------------------------+
|  Function :  GetCurrentIndex()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetCurrentIndex()
{
	int	nResult = -1;
	for (int i =0; i < m_nLanguageNum; i++)
	{
		if (strcmp(m_LanguagesConfig.pszLanguages[i], m_LanguagesConfig.pszCurrentLanguage) == 0)
			nResult = i;
	}
	return nResult;
	
}

/*--------------------------------------------------------------------------+
|  Function :  SetCurrentIndex(int nIndex)									|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetCurrentIndex(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_nLanguageNum) return;
	
	SetCurrentLanguage(m_LanguagesConfig.pszLanguages[nIndex]);
}

/*--------------------------------------------------------------------------+
|  Function :  GetLanguageShowStr(int	index)								|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetLanguageShowStr(int	index)
{
	if (index < 0 || index >= m_nLanguageNum) return NULL;
	return	m_pszLanguages[index];
}

/*--------------------------------------------------------------------------+
|  Function :  GetLanguagesBmpPath(int	index)								|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetLanguagesBmpPath(int	index)
{
	if (index < 0 || index >= m_nLanguageNum) return NULL;
	return	m_pszLanguagesBmpPath[index];
}

/*--------------------------------------------------------------------------+
|  Function :  GetCurrentLanguageShowStr()									|	
|  Task     : 	get current language's string on the form		            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetCurrentLanguageShowStr()
{
	/*
	char*	pszReturn = NULL;
	for (int i =0; i < m_nLanguageNum; i++)
	{
		if (strcmp(m_LanguagesConfig.pszLanguages[i], m_SystemConfig.pszCurrentLanguage) == 0)
			pszReturn = m_LanguagesConfig.pszLanguages[i];
	}
	return pszReturn;
	*/
	return GetLanguageShowStr(GetCurrentIndex());
}

/*--------------------------------------------------------------------------+
|  Function :  GetFontSize()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetFontSize()
{
	return	m_LanguageConfig.nFontSize;
}

/*--------------------------------------------------------------------------+
|  Function :  GetFontConvert()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/

int		CtmConfig::GetFontConvert()
{
	return	m_LanguageConfig.nFontConvert;
}


/*--------------------------------------------------------------------------+
|  Function :  GetShowFontSize()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetShowFontSize()
{
	return	m_LanguageConfig.nShowFontSize;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDspCurveFileName()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetDspCurveFileName()
{
	return m_CurveConfig.pszDspCurveFile;
	
}

/*--------------------------------------------------------------------------+
|  Function :  GetSimCurveFileName()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetSimCurveFileName()
{
	return m_CurveConfig.pszSimCurveFile;
}

/*--------------------------------------------------------------------------+
|  Function :  GetDBID(),GetDBName(),GetDBAttr(),GetDBDefault(),GetDBBlock()|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
char*	CtmConfig::GetDBID()	//Sunny<20070622>
{
	return m_DBConfig.pszDBIDPathName;
}

char*	CtmConfig::GetDBName()
{
	return m_DBConfig.pszDBNamePathName;
}

char*	CtmConfig::GetDBAttr()
{
	return m_DBConfig.pszDBAttrPathName;
}

char*	CtmConfig::GetDBDefault()
{
	return m_DBConfig.pszDBDefaultPathName;
}

char*	CtmConfig::GetDBBlock()
{
	return m_DBConfig.pszDBBlockPathName;
}

char*	CtmConfig::GetDBIndex2id()
{
	return m_DBConfig.pszDBIndex2idPathName;
}

char*	CtmConfig::GetDBRecords()
{
	return m_DBConfig.pszDBRecordsPathName;
}

int		CtmConfig::GetDBSramSize()
{
	return m_DBConfig.nDBSramSize;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadSystemConfig()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::ReadSystemConfig()
{
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_FORMPATH_NAME			, NULL, &m_SystemConfig.pszFormPath);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_HELPPATH_NAME			, NULL, &m_SystemConfig.pszHelpPath);
	//printf("help path:%s\n", m_SystemConfig.pszHelpPath);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_FORMLIB_NAME			, NULL, &m_SystemConfig.pszFormlibPath);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFORM_NAME			, NULL, &m_SystemConfig.pszMainFormName);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_TOOLBARPATH_NAME		, NULL, &m_SystemConfig.pszToolBarPath);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_BMPPATH_NAME			, NULL, &m_SystemConfig.pszBmpPath);
	
	m_SystemConfig.nMachineType 	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MACHINETYPE_NAME, 0);
	m_SystemConfig.nSramSize 		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_SRAMESIZE_NAME, 0);
	m_SystemConfig.nSramInit 		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_SRAM_INITSTATE_NAME, 1);
	m_SystemConfig.nFlashEXInit		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_FLASHEX_INITSTATE_NAME, 1);
	m_SystemConfig.nLCDParaInit		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCD_INITSTATE_NAME, 0);		//James add 2011/3/11 下午 01:12:36
	m_SystemConfig.nMaxInjectSpeed	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, "MaxInjectSpeed", 100);
	m_SystemConfig.nMaxChargeSpeed 	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, "MaxChargeSpeed", 100);
	m_SystemConfig.nKeyFlag			= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_KEYFLAG_NAME, 0);
	m_SystemConfig.nKeyLedFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_KEYLEDFLAG_NAME, 0);  
	//m_SystemConfig.nAustoneFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AUSTONEFLAG_NAME, 0);
	//m_SystemConfig.nN2CFlag			= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_N2CFLAG_NAME, 0);
	//m_SystemConfig.nDsp28Flag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSP28FLAG_NAME, 0);
	m_SystemConfig.nTempCurveFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TEMPCURVEFLAG_NAME, 0);
	m_SystemConfig.nConfigDealRamp	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_CONFIGRAMPDEAL_NAME, 0);
	m_SystemConfig.nLCDSize			= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCDSIZE_NAME, 0);
	m_SystemConfig.nKeyLockFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_KEYLOACKFLAG_NAME, 0);
	m_SystemConfig.nLCDHeight		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCDHEIGHT_NAME, 0);
	m_SystemConfig.nLCDWidth		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCDWIDTH_NAME, 0);
	//m_SystemConfig.nDspCom2Flag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSPCOM2FLAG_NAME, 0);
	m_SystemConfig.nDemoFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DEMOFLAG_NAME, 0);
	m_SystemConfig.nTimeFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TIMEFLAG_NAME, 0);	
	m_SystemConfig.nSlaveFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_SLAVEFLAG_NAME, 0);
	//m_SystemConfig.nMainFrameType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME, 0);
	m_SystemConfig.nPrivSwitch		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_PRIVSWITCH_NAME, 0);
	m_SystemConfig.nCanvasFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_CANVASFLAG_NAME, 0);
	m_SystemConfig.nFocusColor		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME,INI_KEY_FOCUSCOLOR_NAME,0);
	//m_SystemConfig.nCommTypeSwitch	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COMMTYPESWITCH_NAME, 0);
	//m_SystemConfig.nAmmeterFlag	    = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AMMETERFLAG_NAME, 0);
	//m_SystemConfig.nTouchPanlType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TOUCHPANLTYPE_NAME, 0);
	//	fans add 2010-5-6 11:27:52
	m_SystemConfig.nOkKey			= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_OKKEY_NAME, 0);
	m_SystemConfig.nBinaryType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_BINARY_NAME, 0);
	m_SystemConfig.nModbusType	= m_IniFile.ReadInteger(INI_SECTION_MODBUS_NAME, INI_KEY_MODBUSTYPE_NAME, 0);
	//Caozz add 2010/9/26 09:48上午
	//m_SystemConfig.nStopPwdType	    = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_STOPPWD_NAME, 0);
	
	//m_SystemConfig.nRemoteFlag	    = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEFLAG_NAME, 0);
	//m_SystemConfig.nRemoteImageType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEIMAGETYPE_NAME, 0);
	//m_SystemConfig.niNetPrivFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETPRIVILEGE, 0);
	//m_SystemConfig.nErrorControl	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ERRORCONTROL, 0);
	//m_SystemConfig.nCom2Type		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COM2TYPE_NAME, 0);
	//m_SystemConfig.nHmiCommFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_HMICOMMTYPE_NAME, 0);
	//m_SystemConfig.nEnergy2MaxNum	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ENERGYMAXNUM_NAME, 0);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadLanguagesConfig()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::ReadLanguagesConfig()
{
	int	nTemp = 0;
	int	nDataFlag = 0;
	
	nTemp = m_IniFile.ReadSection(INI_SECTION_LANGUAGE_NAME, m_LanguagesConfig.pszLanguages, 0);
	
	m_IniFile.ReadSectionValues(INI_SECTION_LANGUAGE_NAME, m_pszLanguages, 0);
	m_IniFile.ReadSectionValues(INI_SECTION_LANGUAGE_BMPPATH, m_pszLanguagesBmpPath, 0);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_CURRENTLANGUAGE_NAME	, NULL, &m_LanguagesConfig.pszCurrentLanguage);
	
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_HELPPATH_NAME);
	if(nDataFlag > 0)
	{
		m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_HELPPATH_NAME	, NULL, &m_SystemConfig.pszHelpPath);
	}
	//m_LanguagesConfig.nLanguageType = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LANGUAGETYPE_NAME, 0);
	
	return nTemp;
}

void	CtmConfig::ReadLogoConfig ( )
{
	m_LogoConfig.nLogoFlag = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOFLAG_NAME, 0);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOFORM_NAME, NULL, &m_LogoConfig.pszLogoForm);
	m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOPATH_NAME, NULL, &m_LogoConfig.pszLogoPath);
	m_LogoConfig.nLogoTime = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LOGOTIME_NAME, 0);
	m_LogoConfig.nGifType = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_GIFLOGOTYPE_NAME, 0);
	return ;
}		/* -----  end of method CtmConfig::ReadLogoConfig  ----- */


void	CtmConfig::ReadiNetConfig ( )
{
	int	nDataFlag = 0;
	m_iNetConfig.nInetFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETFLAG_NAME, 0); 
	m_iNetConfig.nInetMacSwitch	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETMACSWITCH_NAME, 0);
	m_iNetConfig.niNetFontType 	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INET_FONTTYPE, 0);
	
	
	//niNetPrivFlag 
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_INETPRIVILEGE);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_iNetConfig.niNetPrivFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETPRIVILEGE, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_iNetConfig.niNetPrivFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETPRIVILEGE, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	}
	//printf("niNetPrivFlag =%d nDataFlag=%d \n", m_iNetConfig.niNetPrivFlag, nDataFlag);
	//m_iNetConfig.niNetPrivFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETPRIVILEGE, 0);
	
	return ;
}		/* -----  end of method CtmConfig::ReadiNetConfig  ----- */

void	CtmConfig::ReadRemoteConfig ( )
{
	m_RemoteConfig.nRemoteFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEFLAG_NAME, 0);
	m_RemoteConfig.nRemoteImageType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEIMAGETYPE_NAME, 0);
	m_RemoteConfig.nRemoteMacSwitch	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEMACSWITCH_NAME, 0);
	
	return ;
}

void	CtmConfig::ReadFunctionConfig ( )
{
	int	nDataFlag = 0;
	
	//nDspCom2Flag 
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_DSPCOM2FLAG_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nDspCom2Flag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSPCOM2FLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nDspCom2Flag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSPCOM2FLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	//printf("GetDspCom2Flag =%d nDataFlag=%d \n", m_FunctionConfig.nDspCom2Flag, nDataFlag);
	//nCom2Type
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_COM2TYPE_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nCom2Type		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COM2TYPE_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nCom2Type		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COM2TYPE_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nCom2Type =%d nDataFlag=%d \n", m_FunctionConfig.nCom2Type, nDataFlag);
	
	//nDsp28Flag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_DSP28FLAG_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nDsp28Flag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSP28FLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nDsp28Flag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSP28FLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nDsp28Flag =%d nDataFlag=%d \n", m_FunctionConfig.nDsp28Flag, nDataFlag);
	
	//picker flag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_PICKERFLAG_NAME);
	if(nDataFlag > 0)
	{
		m_FunctionConfig.nPickerFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_PICKERFLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nPickerFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_PICKERFLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	
	//nAustoneFlag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_AUSTONEFLAG_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nAustoneFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AUSTONEFLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nAustoneFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AUSTONEFLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nAustoneFlag =%d nDataFlag=%d \n", m_FunctionConfig.nAustoneFlag, nDataFlag);
	
	//nN2CFlag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_N2CFLAG_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nN2CFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_N2CFLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nN2CFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_N2CFLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	//M3Flag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_M3FLAG_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nM3Flag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_M3FLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nM3Flag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_M3FLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nN2CFlag =%d nDataFlag=%d \n", m_FunctionConfig.nN2CFlag, nDataFlag);
	
	//nCommTypeSwitch
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_COMMTYPESWITCH_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nCommTypeSwitch		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COMMTYPESWITCH_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nCommTypeSwitch		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COMMTYPESWITCH_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nCommTypeSwitch =%d nDataFlag=%d \n", m_FunctionConfig.nCommTypeSwitch, nDataFlag);
	
	//nAmmeterFlag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_AMMETERFLAG_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nAmmeterFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AMMETERFLAG_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nAmmeterFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AMMETERFLAG_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nAmmeterFlag =%d nDataFlag=%d \n", m_FunctionConfig.nAmmeterFlag, nDataFlag);
	
	//nTouchPanlType
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_TOUCHPANLTYPE_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nTouchPanlType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TOUCHPANLTYPE_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nTouchPanlType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TOUCHPANLTYPE_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nTouchPanlType =%d nDataFlag=%d \n", m_FunctionConfig.nTouchPanlType, nDataFlag);
	
	//nStopPwdType
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_STOPPWD_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nStopPwdType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_STOPPWD_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nStopPwdType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_STOPPWD_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nStopPwdType =%d nDataFlag=%d \n", m_FunctionConfig.nStopPwdType, nDataFlag);
	
	//nErrorControl
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_ERRORCONTROL);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nErrorControl		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ERRORCONTROL, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nErrorControl		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ERRORCONTROL, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	//printf("nErrorControl =%d nDataFlag=%d \n", m_FunctionConfig.nErrorControl, nDataFlag);
	
	//nHmiCommFlag
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_HMICOMMTYPE_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nHmiCommFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_HMICOMMTYPE_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nHmiCommFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_HMICOMMTYPE_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	//nEnergy2MaxNum
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_ENERGYMAXNUM_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nEnergy2MaxNum		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ENERGYMAXNUM_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nEnergy2MaxNum		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ENERGYMAXNUM_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	//nCanType
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_CANTYPE_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nCanType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_CANTYPE_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nCanType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_CANTYPE_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_MAXCURVENUM_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nMaxCurveNum		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAXCURVENUM_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nMaxCurveNum		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAXCURVENUM_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	//MainFrameType
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME);
	if(nDataFlag>0){
		m_SystemConfig.nMainFrameType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME, 0);
	}
	else{
			m_IniFile.CloseFile();
			m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
			m_SystemConfig.nMainFrameType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_MAINFRAMETYPE_NAME, 0);
			m_IniFile.CloseFile();
			m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
		}
	
	//nMachineAxis
	char	pszMachineAxis[256];
	for(int i = 0; i < 10; i++)
	{
		memset(pszMachineAxis, 0, sizeof(pszMachineAxis));
		
		sprintf(pszMachineAxis, "%s%d", INI_KEY_MACHINEAXIS_NAME, i+1);
		nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, pszMachineAxis);
		if(nDataFlag > 0)				//if function.ini have the section
		{
			m_FunctionConfig.nMachineAxis[i] = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, pszMachineAxis, 0);
		}
		else							//if no have
		{
			m_IniFile.CloseFile();
			m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
			m_FunctionConfig.nMachineAxis[i] = m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, pszMachineAxis, 0);
			
			m_IniFile.CloseFile();
			m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
		}
	}
	
	
	nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_TIMERTASKTYPE_NAME);
	if(nDataFlag > 0)				//if function.ini have the section
	{
		m_FunctionConfig.nTimerTaskType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TIMERTASKTYPE_NAME, 0);
	}
	else							//if no have
	{
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
		m_FunctionConfig.nTimerTaskType		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TIMERTASKTYPE_NAME, 0);
		
		m_IniFile.CloseFile();
		m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	}
	
	
	
	//James add for CAN Index
	nDataFlag		= m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_CANKEYBOARDNUMBER_NAME);
	if(nDataFlag > 0)
	{
		m_FunctionConfig.nCANIndex	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_CANKEYBOARDNUMBER_NAME, 0);
	}
	
	nDataFlag		= m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_RECORDTYPE_NAME);
	if(nDataFlag > 0)
	{
		m_FunctionConfig.nRecordType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_RECORDTYPE_NAME, 0);
	}
	
	nDataFlag		= m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_RECORDFILEPATH_NAME);
	if(nDataFlag > 0)
	{
		m_IniFile.ReadString(INI_SECTION_SYSTEM_NAME, INI_KEY_RECORDFILEPATH_NAME	, NULL, &m_FunctionConfig.pszRecordFilePath);
	}
	
	
	//nOnOffStateMaxNum
	//nDataFlag = m_IniFile.CheckVaildSection(INI_SECTION_SYSTEM_NAME, INI_KEY_ONOFFSTATEMAXNUM_NAME);
	//if(nDataFlag > 0)				//if function.ini have the section
	//{
	//	m_FunctionConfig.nOnOffStateMaxNum		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ONOFFSTATEMAXNUM_NAME, 0);
	//}
	//else							//if no have
	//{
	//	m_IniFile.CloseFile();
	//	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	//	m_FunctionConfig.nOnOffStateMaxNum		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ONOFFSTATEMAXNUM_NAME, 0);
	//	
	//	m_IniFile.CloseFile();
	//	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	//}
	//printf("nHmiCommFlag =%d nDataFlag=%d \n", m_FunctionConfig.nHmiCommFlag, nDataFlag);
	//m_FunctionConfig.nCom2Type		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COM2TYPE_NAME, 0);
	//m_FunctionConfig.nDsp28Flag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSP28FLAG_NAME, 0);
	//m_FunctionConfig.nAustoneFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AUSTONEFLAG_NAME, 0);
	//m_FunctionConfig.nN2CFlag		= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_N2CFLAG_NAME, 0);
	//m_FunctionConfig.nCommTypeSwitch= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COMMTYPESWITCH_NAME, 0);
	//m_FunctionConfig.nAmmeterFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AMMETERFLAG_NAME, 0);
	//m_FunctionConfig.nTouchPanlType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TOUCHPANLTYPE_NAME, 0);
	//m_FunctionConfig.nStopPwdType	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_STOPPWD_NAME, 0);
	//m_FunctionConfig.nErrorControl	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ERRORCONTROL, 0);
	//m_FunctionConfig.nHmiCommFlag	= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_HMICOMMTYPE_NAME, 0);
	
	return ;
}

/*--------------------------------------------------------------------------+
|  Function :  ReadLanguageConfig()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void		CtmConfig::ReadLanguageConfig()
{
	char	sz[32];

	for (int i = strlen(m_LanguagesConfig.pszCurrentLanguage) - 1;i >= 0 ;i--)
	{
		if (m_LanguagesConfig.pszCurrentLanguage[i] == ' ' || 
			m_LanguagesConfig.pszCurrentLanguage[i] == '\t')
			m_LanguagesConfig.pszCurrentLanguage[i] = '\0';
		else
			break;
	}
	sprintf(sz, "%s%s", m_LanguagesConfig.pszCurrentLanguage, "_font");
	m_LanguageConfig.nFontCode = m_IniFile.ReadInteger(sz, INI_KEY_FONTCODE_NAME, 0);
	m_LanguageConfig.nFontSize = m_IniFile.ReadInteger(sz, INI_KEY_FONTSIZE_NAEM, 0);
	m_LanguageConfig.nFontConvert = m_IniFile.ReadInteger(sz, INI_KEY_FONT_CONVERT, 0);
	m_LanguagesConfig.nLanguageType= m_IniFile.ReadInteger(sz, INI_KEY_LANGUAGETYPE_NAME, 0);
	m_LanguagesConfig.nLanguagePatch= m_IniFile.ReadInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LANGUAGEPATCH_NAME, 0);	
	
	m_LanguageConfig.nShowFontSize = m_IniFile.ReadInteger(sz, INI_KEY_SHOWFONTSIZE_NAEM, 0);
	m_IniFile.ReadString(sz, INI_KEY_LANGUAGEPATH_NAME	, NULL, &m_LanguageConfig.pszLanguagePath);
	m_IniFile.ReadString(sz, INI_KEY_LANGUAGEPATH_HELP_NAME	, NULL, &m_LanguageConfig.pszLanguagePathHelp);
	m_IniFile.ReadString(sz, INI_KEY_FONTFILE_NAME		, NULL, &m_LanguageConfig.pszFontFileName);
	m_IniFile.ReadString(sz, INI_KEY_LANGUAGE_NAME		, NULL, &m_LanguageConfig.pszLanguageFileName);
	
	m_IniFile.ReadString(sz, INI_KEY_RECORDXML_FILENAME		, NULL, &m_LanguageConfig.pszRecordXMLFileName);
	m_IniFile.ReadString(sz, INI_KEY_VIEWXML_FILENAME		, NULL, &m_LanguageConfig.pszViewXMLFileName);
	m_IniFile.ReadString(sz, INI_KEY_FONTFT_FILENAME		, NULL, &m_LanguageConfig.pszFontFTFileName);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadCurveConfig()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void		CtmConfig::ReadCurveConfig()
{
	m_IniFile.ReadString(INI_SECTION_CURVE_NAME, INI_KEY_DSPCURVE_NAME, NULL, &m_CurveConfig.pszDspCurveFile);
	m_IniFile.ReadString(INI_SECTION_CURVE_NAME, INI_KEY_SIMCURVE_NAME, NULL, &m_CurveConfig.pszSimCurveFile);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadMoldSetConfig()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void		CtmConfig::ReadMoldSetConfig()
{
	m_IniFile.ReadString(INI_SECTION_MOLDSET_NAME, INI_KEY_HEAD_NAME, NULL, &m_MoldSetConfig.pszHeadFile);
	m_IniFile.ReadString(INI_SECTION_MOLDSET_NAME, INI_KEY_CONTENT_NAME, NULL, &m_MoldSetConfig.pszContentFile);
	m_MoldSetConfig.nMaxNum = m_IniFile.ReadInteger(INI_SECTION_MOLDSET_NAME, INI_KEY_MOLD_MAX_NAME, 0);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadRecordSetConfig()										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::ReadRecordSetConfig()
{
	m_IniFile.ReadString(INI_SECTION_RECORDSET_NAME, INI_KEY_RECORD_FILE_NAME, NULL, &m_RecordSetConfig.pszFile);
	m_RecordSetConfig.nMaxNum = m_IniFile.ReadInteger(INI_SECTION_RECORDSET_NAME, INI_KEY_RECORD_MAX_NAME, 0);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadErrorConfig()											|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::ReadErrorConfig()
{
	m_IniFile.ReadString(INI_SECTION_ERROR_NAME, INI_KEY_ERROR_FILE_NAME, NULL, &m_ErrorConfig.pszFile);
	m_ErrorConfig.nMaxNum = m_IniFile.ReadInteger(INI_SECTION_ERROR_NAME, INI_KEY_ERROR_MAX_NAME, 0);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadMoniConfig()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::ReadMoniConfig()
{
	m_IniFile.ReadString(INI_SECTION_MONI_NAME, INI_KEY_MONI_FILE_NAME, NULL, &m_MoniConfig.pszFile);
	m_MoniConfig.nMaxNum = m_IniFile.ReadInteger(INI_SECTION_MONI_NAME, INI_KEY_MONI_MAX_NAME, 0);
}

/*--------------------------------------------------------------------------+
|  Function :  ReadDataProcessConfig()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::ReadDataProcessConfig()
{
	m_DataProcessConfig.nDataProcessFlag 	 = m_IniFile.ReadInteger(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATAPROCESS_FLAG_NAME, 0);

	m_IniFile.ReadString(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATASTATE_FILE_NAME, NULL, &m_DataProcessConfig.pszStateFile);
	m_DataProcessConfig.nStateMaxNum = m_IniFile.ReadInteger(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATASTATE_MAX_NAME, 0);

	m_IniFile.ReadString(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATAENERGY_FILE_NAME, NULL, &m_DataProcessConfig.pszEnergyFile);
	m_DataProcessConfig.nEnergyMaxNum = m_IniFile.ReadInteger(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATAENERGY_MAX_NAME, 0);
	
	m_IniFile.ReadString(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATAPOWERONOFF_FILE_NAME, NULL, &m_DataProcessConfig.pszPowerOnOffFile);
	m_DataProcessConfig.nPowerOnOffMaxNum = m_IniFile.ReadInteger(INI_SECTION_DATAPROCESS_NAME, INI_KEY_DATAPOWERONOFF_MAX_NAME, 0);

	m_IniFile.ReadString(INI_SECTION_DATAPROCESS_NAME, INI_KEY_ONOFFSTATE_FILE_NAME, NULL, &m_DataProcessConfig.pszOnOffStateFile);
	m_DataProcessConfig.nOnOffStateMaxNum = m_IniFile.ReadInteger(INI_SECTION_DATAPROCESS_NAME, INI_KEY_ONOFFSTATEMAXNUM_NAME, 0);

}

/*--------------------------------------------------------------------------+
|  Function :  ReadDBConfig()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void		CtmConfig::ReadDBConfig()	//Sunny<20070622>
{
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_ID, 		NULL, &m_DBConfig.pszDBIDPathName);
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_NAME, 	NULL, &m_DBConfig.pszDBNamePathName);
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_ATTR,	NULL, &m_DBConfig.pszDBAttrPathName);
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_DEFAULT,	NULL, &m_DBConfig.pszDBDefaultPathName);
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_BLOCK,	NULL, &m_DBConfig.pszDBBlockPathName);
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_INDEX2ID,NULL, &m_DBConfig.pszDBIndex2idPathName);
	m_IniFile.ReadString(INI_SECTION_DATABASE_NAME, INI_KEY_DB_RECORDS,	NULL, &m_DBConfig.pszDBRecordsPathName);
	m_DBConfig.nDBSramSize = m_IniFile.ReadInteger(INI_SECTION_DATABASE_NAME, INI_KEY_DB_DBSRAMSIZE, 0);
	
}

/*--------------------------------------------------------------------------+
|  Function :  GetINETFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetINETFlag()
{
	return 	m_iNetConfig.nInetFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetINETFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetINETFlag(int nFlag)
{
	m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	m_iNetConfig.nInetFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}
//Sunny<20080826><<<<<
/*--------------------------------------------------------------------------+
|  Function :  GetINETMacSwtich()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetINETMacSwtich()
{
	return 	m_iNetConfig.nInetMacSwitch;
}

/*--------------------------------------------------------------------------+
|  Function :  SetINETFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetINETMacSwtich(int nFlag)
{
	m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	m_iNetConfig.nInetMacSwitch = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETMACSWITCH_NAME, nFlag);
	m_IniFile.CloseFile();
}
//Sunny<20080826>>>>>>
/*--------------------------------------------------------------------------+
|  Function :  GetDSP28Flag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetDSP28Flag()
{
	return 	m_FunctionConfig.nDsp28Flag;
}

/*--------------------------------------------------------------------------+
|  Function :  GetPickerFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetPickerFlag()
{
	return 	m_FunctionConfig.nPickerFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetDSP28Flag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetDSP28Flag(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nDsp28Flag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSP28FLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  SetPickerFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetPickerFlag(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nPickerFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_PICKERFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetAustoneFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetAustoneFlag()
{
	return 	m_FunctionConfig.nAustoneFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetAustoneFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetAustoneFlag(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nAustoneFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_AUSTONEFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetN2CFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetN2CFlag()
{
	return 	m_FunctionConfig.nN2CFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetN2CFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetN2CFlag(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nN2CFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_N2CFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetM3Flag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetM3Flag()
{
	return 	m_FunctionConfig.nM3Flag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetM3Flag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetM3Flag(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nM3Flag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_M3FLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetKeyFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetKeyFlag()
{
	return 	m_SystemConfig.nKeyFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  GetKeyLedFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetKeyLedFlag()
{
	return 	m_SystemConfig.nKeyLedFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetKeyFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetKeyFlag(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nKeyFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_KEYFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  SetKeyLedFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetKeyLedFlag(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nKeyLedFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_KEYLEDFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetTempCurveFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetTempCurveFlag()
{
	return 	m_SystemConfig.nTempCurveFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetTempCurveFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetTempCurveFlag(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nTempCurveFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_TEMPCURVEFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetConfigDealRamp()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetConfigDealRamp()
{
	return 	m_SystemConfig.nConfigDealRamp;
}

/*--------------------------------------------------------------------------+
|  Function :  SetConfigDealRamp(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetConfigDealRamp(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nConfigDealRamp = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_CONFIGRAMPDEAL_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetLCDSizeFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetLCDSizeFlag()
{
	return 	m_SystemConfig.nLCDSize;
}

/*--------------------------------------------------------------------------+
|  Function :  SetLCDSizeFlag(int nFlag)									|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetLCDSizeFlag(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nLCDSize = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCDSIZE_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetKeyLockFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetKeyLockFlag()
{
	return 	m_SystemConfig.nKeyLockFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetKeyLockFlag(int nFlag)									|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetKeyLockFlag(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nKeyLockFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_KEYLOACKFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetLCDHeight()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int			CtmConfig::GetLCDHeight()
{
	return 	m_SystemConfig.nLCDHeight;
}

/*--------------------------------------------------------------------------+
|  Function :  SetLCDHeight(int nHeight)									|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetLCDHeight(int nHeight)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nLCDHeight = nHeight;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCDHEIGHT_NAME, nHeight);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetLCDWidth()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetLCDWidth()
{
	return 	m_SystemConfig.nLCDWidth;
}

/*--------------------------------------------------------------------------+
|  Function :  SetLCDWidth(int nWidth)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetLCDWidth(int nWidth)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nLCDWidth = nWidth;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCDWIDTH_NAME, nWidth);
	m_IniFile.CloseFile();
}

void	CtmConfig::ReadLCDParaConfig()
{
	m_LCDParaConfig.nsignal 	= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_SIGNAL_NAME, -1);
	m_LCDParaConfig.pixclock	= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_PIXCLOCK_NAME, -1);
	m_LCDParaConfig.lccr0		= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_LCCR0_NAME, -1);
	m_LCDParaConfig.lccr3		= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_LCCR3_NAME, -1);
	m_LCDParaConfig.xres		= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_XRES_NAME, -1);
	m_LCDParaConfig.yres		= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_YRES_NAME, -1);
	m_LCDParaConfig.hsync_len	= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_HSYNC_LEN_NAME, -1);
	m_LCDParaConfig.vsync_len	= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_VSYNC_LEN_NAME, -1);
	m_LCDParaConfig.left_margin	= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_LEFT_MARGIN_NAME, -1);
	m_LCDParaConfig.upper_margin= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_UPPER_MARGIN_NAME, -1);
	m_LCDParaConfig.right_margin= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_RIGHT_MARGIN_NAME, -1);
	m_LCDParaConfig.lower_margin= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_LOWER_MARGIN_NAME, -1);
	m_LCDParaConfig.sync		= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_SYNC_NAME, -1);
	m_LCDParaConfig.reserved	= m_IniFile.ReadInteger(INI_SECTION_LCD_NAME, INI_KEY_RESERVED_NAME, -1);
}

//signal
int		CtmConfig::GetLCDSignal()
{
	return 	m_LCDParaConfig.nsignal;
}

void	CtmConfig::SetLCDSignal(int nSignal)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.nsignal = nSignal;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_SIGNAL_NAME, nSignal);
	m_IniFile.CloseFile();
}

//pixclock
int		CtmConfig::GetLCDPixclock()
{
	return 	m_LCDParaConfig.pixclock;
}

void	CtmConfig::SetLCDPixclock(int nPixclock)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.pixclock = nPixclock;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_PIXCLOCK_NAME, nPixclock);
	m_IniFile.CloseFile();
}

// lccr0 
int		CtmConfig::GetLCDLccr0()
{
	return 	m_LCDParaConfig.lccr0;
}

void	CtmConfig::SetLCDLccr0(int nLccr0)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.lccr0 = nLccr0;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_LCCR0_NAME, nLccr0);
	m_IniFile.CloseFile();
}

// lccr3 
int		CtmConfig::GetLCDLccr3()
{
	return 	m_LCDParaConfig.lccr3;
}

void	CtmConfig::SetLCDLccr3(int nLccr3)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.lccr3 = nLccr3;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_LCCR3_NAME, nLccr3);
	m_IniFile.CloseFile();
}

// xres 
int		CtmConfig::GetLCDXres()
{
	return 	m_LCDParaConfig.xres;
}

void	CtmConfig::SetLCDXres(int nXres)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.xres = nXres;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_XRES_NAME, nXres);
	m_IniFile.CloseFile();
}

// yres 
int		CtmConfig::GetLCDYres()
{
	return 	m_LCDParaConfig.yres;
}

void	CtmConfig::SetLCDYres(int nYres)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.yres = nYres;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_YRES_NAME, nYres);
	m_IniFile.CloseFile();
}

// hsync_len 
int		CtmConfig::GetLCDHsync_len()
{
	return 	m_LCDParaConfig.hsync_len;
}

void	CtmConfig::SetLCDHsync_len(int nHsync_len)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.hsync_len = nHsync_len;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_HSYNC_LEN_NAME, nHsync_len);
	m_IniFile.CloseFile();
}

// vsync_len 
int		CtmConfig::GetLCDVsync_len()
{
	return 	m_LCDParaConfig.vsync_len;
}

void	CtmConfig::SetLCDVsync_len(int nVsync_len)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.vsync_len = nVsync_len;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_VSYNC_LEN_NAME, nVsync_len);
	m_IniFile.CloseFile();
}

// left_margin 
int		CtmConfig::GetLCDLeft_margin()
{
	return 	m_LCDParaConfig.left_margin;
}

void	CtmConfig::SetLCDLeft_margin(int nLeft_margin)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.left_margin = nLeft_margin;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_LEFT_MARGIN_NAME, nLeft_margin);
	m_IniFile.CloseFile();
}

// upper_margin 
int		CtmConfig::GetLCDUpper_margin()
{
	return 	m_LCDParaConfig.upper_margin;
}

void	CtmConfig::SetLCDUpper_margin(int nUpper_margin)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.upper_margin = nUpper_margin;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_UPPER_MARGIN_NAME, nUpper_margin);
	m_IniFile.CloseFile();
}

// right_margin 
int		CtmConfig::GetLCDRight_margin()
{
	return 	m_LCDParaConfig.right_margin;
}

void	CtmConfig::SetLCDRight_margin(int nRight_margin)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.right_margin = nRight_margin;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_RIGHT_MARGIN_NAME, nRight_margin);
	m_IniFile.CloseFile();
}

// lower_margin 
int		CtmConfig::GetLCDLower_margin()
{
	return 	m_LCDParaConfig.lower_margin;
}

void	CtmConfig::SetLCDLower_margin(int nLower_margin)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.lower_margin = nLower_margin;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_LOWER_MARGIN_NAME, nLower_margin);
	m_IniFile.CloseFile();
}

// sync 
int		CtmConfig::GetLCDSync()
{
	return 	m_LCDParaConfig.sync;
}

void	CtmConfig::SetLCDSync(int nSync)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.sync = nSync;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_SYNC_NAME, nSync);
	m_IniFile.CloseFile();
}

// reserved 
int		CtmConfig::GetLCDReserved()
{
	return 	m_LCDParaConfig.reserved;
}

void	CtmConfig::SetLCDReserved(int nReserved)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_LCDParaConfig.reserved = nReserved;
	m_IniFile.WriteInteger(INI_SECTION_LCD_NAME, INI_KEY_RESERVED_NAME, nReserved);
	m_IniFile.CloseFile();
}

// Init LCD Para 
int		CtmConfig::GetLCDParaInit()
{
	return 	m_SystemConfig.nLCDParaInit;
}

void	CtmConfig::SetLCDParaInit(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nLCDParaInit = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_LCD_INITSTATE_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetDspCom2Flag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetDspCom2Flag()
{
	return 	m_FunctionConfig.nDspCom2Flag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetDsoCom2Flag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetDspCom2Flag(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nDspCom2Flag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_DSPCOM2FLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetBinaryType()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetBinaryType() const
{
	return 	m_SystemConfig.nBinaryType;
}

/*--------------------------------------------------------------------------+
|  Function :  SetBinaryType(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetBinaryType(int nFlag)
{
	m_IniFile.OpenFile(m_SystemConfig.pszConfigPath);
	m_SystemConfig.nBinaryType = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_BINARY_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetStopPWDType()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetStopPWDType() const
{
	return 	m_FunctionConfig.nStopPwdType;
}

/*--------------------------------------------------------------------------+
|  Function :  SetStopPWDType(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetStopPWDType(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nStopPwdType = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_STOPPWD_NAME, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetRemoteFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetRemoteFlag() const
{
	return 	m_RemoteConfig.nRemoteFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetRemoteFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetRemoteFlag(int nFlag)
{
	m_IniFile.OpenFile(m_RemoteConfig.pszConfigPath);
	m_RemoteConfig.nRemoteFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEFLAG_NAME, nFlag);
	m_IniFile.CloseFile();
}

int		CtmConfig::GetiNetPrivFlag() const
{
	return 	m_iNetConfig.niNetPrivFlag;
}

void	CtmConfig::SetiNetPrivFlag(int nFlag)
{
	m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	m_iNetConfig.niNetPrivFlag = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INETPRIVILEGE, nFlag);
	m_IniFile.CloseFile();
}

/*--------------------------------------------------------------------------+
|  Function :  GetRemoteImageType()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetRemoteImageType() const
{
	return 	m_RemoteConfig.nRemoteImageType;
}

/*--------------------------------------------------------------------------+
|  Function :  SetRemoteFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetRemoteImageType(int nFlag)
{
	m_IniFile.OpenFile(m_RemoteConfig.pszConfigPath);
	m_RemoteConfig.nRemoteImageType = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEIMAGETYPE_NAME, nFlag);
	m_IniFile.CloseFile(); 
}

/*--------------------------------------------------------------------------+
|  Function :  GetRemoteImageType()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetRemoteMacSwtich() const
{
	return 	m_RemoteConfig.nRemoteMacSwitch;
}

/*--------------------------------------------------------------------------+
|  Function :  SetRemoteFlag(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetRemoteMacSwtich(int nFlag)
{
	m_IniFile.OpenFile(m_RemoteConfig.pszConfigPath);
	m_RemoteConfig.nRemoteMacSwitch = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_REMOTEMACSWITCH_NAME, nFlag);
	m_IniFile.CloseFile(); 
}


/*--------------------------------------------------------------------------+
|  Function :  GetErrorControl()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetErrorControl() const
{
	return 	m_FunctionConfig.nErrorControl;
}

/*--------------------------------------------------------------------------+
|  Function :  SetErrorControl(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetErrorControl(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nErrorControl = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_ERRORCONTROL, nFlag);
	m_IniFile.CloseFile(); 
}

/*--------------------------------------------------------------------------+
|  Function :  GetCom2Type()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetCom2Type()
{
	return 	m_FunctionConfig.nCom2Type;
}

/*--------------------------------------------------------------------------+
|  Function :  SetCom2Type(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetCom2Type(int nFlag)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nCom2Type = nFlag;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_COM2TYPE_NAME, nFlag);
	m_IniFile.CloseFile(); 
}

/*--------------------------------------------------------------------------+
|  Function :  GetHmiCommFlag()												|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmConfig::GetHmiCommFlag() const
{
	return 	m_FunctionConfig.nHmiCommFlag;
}

/*--------------------------------------------------------------------------+
|  Function :  SetCom2Type(int nFlag)										|	
|  Task     : 													            |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmConfig::SetHmiCommFlag(int nData)
{
	m_IniFile.OpenFile(m_FunctionConfig.pszConfigPath);
	m_FunctionConfig.nHmiCommFlag = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_HMICOMMTYPE_NAME, nData);
	m_IniFile.CloseFile(); 
}

int		CtmConfig::GetINETFontType() const
{
	return 	m_iNetConfig.niNetFontType;
}

void	CtmConfig::SetINETFontType(int nData)
{
	m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	m_iNetConfig.niNetFontType = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INET_FONTTYPE, nData);
	m_IniFile.CloseFile(); 
}

//add by J.Wang 2015/7/24 9:49:11
int		CtmConfig::GetINETHostType() const
{
	return 	m_iNetConfig.niNetHostType;
}


void	CtmConfig::SetINETHostType(int nData)
{
	m_IniFile.OpenFile(m_iNetConfig.pszConfigPath);
	m_iNetConfig.niNetHostType = nData;
	m_IniFile.WriteInteger(INI_SECTION_SYSTEM_NAME, INI_KEY_INET_HOSTTYPE, nData);
	m_IniFile.CloseFile(); 
}

char*	CtmConfig::GetRecordFilePath()
{
	return	m_FunctionConfig.pszRecordFilePath;
}

int		CtmConfig::GetModbusType() const
{
		return m_SystemConfig.nModbusType;
}

void	CtmConfig::SetTimerTaskType(const int nType)
{
	m_FunctionConfig.nTimerTaskType=nType;
}

int		CtmConfig::GetTimerTaskType()const
{
	return m_FunctionConfig.nTimerTaskType;
}