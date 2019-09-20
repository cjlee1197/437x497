/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmConfig				                                         |
|  Task     : CtmConfig header file					                         |
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



#ifndef		D_TMCONFIG
#define		D_TMCONFIG

#include	"inifile.h"
/*==========================================================================+
|           Constants	                                                   	|
+==========================================================================*/
const	int	MAX_LANGUAGE_NUM 			= 128;
const	int	MACHINE_TYPE_SIM			= 0;		//	���qSim
const	int	MACHINE_TYPE_OIL_J5			= 1;		//	�o��J5
const	int	MACHINE_TYPE_OIL 			= 2;		//	�o��J6
const	int	MACHINE_TYPE_OIL_DC			= 4;		//  DC

/*==========================================================================+
|          Type definition                                                  |
+==========================================================================*/
//	�t�Ϊ��t�m	// <FANS> 2006/11/16 04:04�U��
typedef	struct	tagtmSystemConfig
{
	int		nState;
	char*	pszConfigPath;			//	config.ini path
	char*	pszFormPath;			//	���骺���|�W
	char*	pszHelpPath;			// �b�u���U���|�W
	char*	pszFormlibPath;			//	����B�z�޿誺���|�W
	char*	pszMainFormName;		//	�D���骺�W��
	char*	pszToolBarPath;			//	toolbar�����|
	char*	pszBmpPath;				//	bmp path
	int		nMachineType;			//	����������,0��ܥ��q,1��ܪo��
	int		nSramSize;				//	Sram size
	int		nSramInit;				//	�аOSram�O�_�ݭn��l��, 0��ܻݭn��l��, 1��ܤ��ݭn
	int		nFlashEXInit;			//	�аOFlashEX�O�_�ݭn��l��,0��ܻݭn��l��,1��ܤ��ݭn
	int		nLCDParaInit;			//	�аOLCD�O�_�ݭn��l�ơA0��ܱqconfig,LCD�]�w�ѼƤ���l�ơA1�зǥ�kernel���]�w���Ѽ�
	int		nMaxInjectSpeed;		//	�g�X�̤j�t��
	int		nMaxChargeSpeed;		//	�x�Ƴ̤j�t��
	int		nDsp28Flag;				//  �O�_�㦳dsp28�\��
	int		nAustoneFlag;			//	�O�_�}��Austone�q�T
	int		nN2CFlag;				//	�O�_�}��N2C�q�T	
	int		nM3Flag;				//	�O�_�}��M3�q�T	
	int		nDspCom2Flag;			//	�ҷž��O�_�ϥ�
	int		nCom2Type;				//	Com2 0=�ҷž� 1=���dsp James add 2011/4/8 �W�� 09:54:18
	int		nTempCurveFlag;			//  �O�_�㦳�ūצ��u�\��
	int		nConfigDealRamp;		//  �}���O�_�ھڬq���ܤƪ�l��ramp���
	int		nKeyFlag;				//  �ϥΤ��\�w�����
	int		nKeyLedFlag;			//	�ϥΤ��\����led���
	int		nLCDSize;				//	�G���ؤo
	int		nLCDHeight;				//	�G������
	int		nLCDWidth;				//	�G���e��
	int		nKeyLockFlag;			//  keyLock�O�_�ϥ�
	int		nDemoFlag;				//	Demo Flag
	int		nTimeFlag;
	//	2009-2-11 15:42:38 fans add
	int		nMainFrameType;			//	the mainframe's type
	int		nCommTypeSwitch;		//	communication type switch(0--rs232 1--ethernet)
	int     nAmmeterFlag;       	//  Ammeter Flag
	int		nTouchPanlType;			//  touchpanl		James add 2009/10/27 �U�� 04:27:06
	int		nBinaryType;			//	0= txt 1= binary
	int		nStopPwdType;			//  0= Old 1= new
	int		nOkKey;					//	1:use ok; 0:not use ok
	int 	nRemoteFlag;			//  1:use     0:not use
	int		niNetPrivFlag;			//  init is 5
	int 	nRemoteImageType;		// 	1:bmp; 2:png; 3:jpg
	int 	nErrorControl;			// 	0: Old; 1: New
	int		nHmiCommFlag;			//	0: no use hmicomm 1: use hmicomm
	int		nDefaultPrivilege;		//	user default privilege
	int		nEnergy2MaxNum;			//  save energy maxnum
	int		nOnOffStateMaxNum;		//	save onoffstate maxnum
	int		nSlaveFlag;			    //  0��24���¶ȿ��� 1:48���¶ȿ���
	int		nPrivSwitch;
	int		nCanvasFlag;
	int		nMaxCurveNum;
	int		nFocusColor;			//  set edit focus color
	int		nModbusType;

}tmSystemConfig;

typedef struct  tagtmLogoConfig
{
	int		nLogoFlag;
	char*	pszLogoForm;
	char*	pszLogoPath;
	char*   pszConfigPath;
	int 	nLogoTime;				//Caozz 2011/10/12 �U�� 04:09:47
	int		nGifType;				//Caozz	2014/11/21 14:46:56
}tmLogoConfig;

typedef struct  tagtmiNetConfig
{
	int		nInetFlag;
	int		nInetMacSwitch;
	char*   pszConfigPath;
	int		niNetPrivFlag;			//  init is 5
	int		niNetFontType;
	int		niNetHostType;
}tmiNetConfig; 

typedef struct 	tagtmRemoteFlag
{
	int 	nRemoteFlag;			//  1:use     0:not use
	int 	nRemoteImageType;		// 	1:bmp; 2:png; 3:jpg
	char*   pszConfigPath;
	int		nRemoteMacSwitch;
}tmRemoteConfig;

typedef	struct	tagtmFunctionFlag
{
	int		nDspCom2Flag;			//	�ҷž��O�_�ϥ�
	int		nCom2Type;				//	Com2 0=�ҷž� 1=���dsp James add 2011/4/8 �W�� 09:54:18
	int		nDsp28Flag;				//  �O�_�㦳dsp28�\��
	int		nAustoneFlag;			//	�O�_�}��Austone�q�T
	int		nN2CFlag;				//	�O�_�}��N2C�q�T	
	int		nM3Flag;				//	�O�_�}��N2C�q�T	
	int		nCommTypeSwitch;		//	communication type switch(0--rs232 1--ethernet)
	int     nAmmeterFlag;       	//  Ammeter Flag
	int		nTouchPanlType;			//  touchpanl		James add 2009/10/27 �U�� 04:27:06
	int		nStopPwdType;			//  0= Old 1= new
	int 	nErrorControl;			// 	0: Old; 1: New
	int		nHmiCommFlag;			//	0: no use hmicomm 1: use hmicomm
	int		nEnergy2MaxNum;			//  save energy maxnum
	int		nOnOffStateMaxNum;		//	save onoffstate maxnum
	int		nCanType;				//  0=nouse 1=keyboard 2=mw
	int		nMachineAxis[10];			//	MachinxAxis1 type
	int		nCANIndex;				// CAN Index
	int		nRecordType;			//	recordtype
	char*	pszRecordFilePath;		// 	record file path
	char*   pszConfigPath;	
	int		nPickerFlag	;
	int		nMaxCurveNum;
	int		nTimerTaskType;
}tmFunctionConfig;

typedef	struct	tagtmLCDParaConfig
{
	int	nsignal;
	int	pixclock;
	int	lccr0;
	int	lccr3;
	int	xres;
	int	yres;
	int	hsync_len;
	int	vsync_len;
	int	left_margin;
	int	upper_margin;
	int	right_margin;
	int	lower_margin;
	int	sync;
	int	reserved;
}tmLCDParaConfig;

//	�h��y��	// <FANS> 2006/11/16 04:07�U��
typedef	struct	tagtmLanguagesConfig
{
	int		nState;
	int		nLanguagePatch;
	int		nLanguageType;
	char*	pszCurrentLanguage;		//	�t�Υثe���y��
	char*	pszLanguages[MAX_LANGUAGE_NUM];		//	�t�Ψ�ƪ��y��
}tmLanguagesConfig;

typedef struct	tagtmCurveConfig
{
	int		nState;
	char*	pszDspCurveFile;
	char*	pszSimCurveFile;
}tmCurveConfig;

typedef struct	tagtmMoldSetConfig
{
	int		nState;
	char*	pszHeadFile;
	char*	pszContentFile;
	int		nMaxNum;
}tmMoldSetConfig;

typedef struct	tagtmRecordSetConfig
{
	int		nState;
	int		nMaxNum;
	char*	pszFile;
}tmRecordSetConfig;

typedef	struct	tagtmDataProcessSetConfig
{
	int		nDataProcessFlag;
	int		nStateMaxNum;
	int		nEnergyMaxNum;
	int		nPowerOnOffMaxNum;
	int		nOnOffStateMaxNum;
	char*	pszStateFile;
	char*	pszEnergyFile;
	char*	pszPowerOnOffFile;
	char*   pszOnOffStateFile;
	char*   pszConfigPath;
}tmDataProcessSetConfig;

typedef struct	tagtmLanguageConfig
{
	int		nState;
	int		nFontSize;				//	�r�骺�j�p
	int		nFontCode;				//	�r��s�X���}�l
	int		nFontConvert;			//	J.Wang add for arabic lan.
	int		nShowFontSize;			//	��ܦr��j�p James add 2011/8/2 �U�� 06:44:57
	char*	pszLanguagePath;		//	�h��y�������|
	char*	pszLanguagePathHelp;	// online helper
	char*	pszFontFileName;		//	�I����
	char*	pszLanguageFileName;	//	�y�����
	char*   pszConfigPath;
	char*	pszRecordXMLFileName;		//James add for record xml
	char*	pszViewXMLFileName;			//James add for view xml
	char*	pszFontFTFileName;		// James add for Freetype code 
}tmLanguageConfig;

typedef	tmRecordSetConfig		tmErrorConfig;
typedef	tmRecordSetConfig		tmMoniConfig;
typedef	tmDataProcessSetConfig	tmDataProcessConfig;

typedef struct	tagtmDBConfig		//Sunny<20070622>
{
	char*	pszDBIDPathName;
	char*	pszDBNamePathName;
	char*	pszDBAttrPathName;
	char*	pszDBDefaultPathName;
	char*	pszDBBlockPathName;
	char*	pszDBIndex2idPathName;
	char*	pszDBRecordsPathName;
	int		nDBSramSize;
}tmDBConfig;
/*===========================================================================+
|           Class declaration - CtmConfig                                    |
+===========================================================================*/
class	CtmConfig
{
	/*---------------------------------------------------------------------------+
	|           Constructor and destructor                                       |
	+---------------------------------------------------------------------------*/
	protected:
		CtmConfig();
		virtual ~CtmConfig();
	public:
		static CtmConfig*	GetInstance();
		static void		ReleaseInstance();
	/*-------------------------------------------------------------------------------+
	|        Operations                                                              |
	+-------------------------------------------------------------------------------*/		
	protected:
		virtual	void	ReadSystemConfig();
		virtual	void	ReadLCDParaConfig();
		virtual	int		ReadLanguagesConfig();
		virtual	void	ReadLanguageConfig();
		virtual	void	ReadCurveConfig();
		virtual	void	ReadMoldSetConfig();
		virtual	void	ReadRecordSetConfig();
		virtual	void	ReadErrorConfig();
		virtual	void	ReadMoniConfig();
		virtual	void	ReadDataProcessConfig();
		virtual	void	ReadDBConfig();	//Sunny<20070622>	
		virtual void    ReadLogoConfig();
		virtual void	ReadiNetConfig();
		virtual	void	ReadRemoteConfig();
		virtual	void	ReadFunctionConfig();
	public:
		
		bool		OpenConfigFile();
		void		CloseConfigFile();
		
		char*	GetConfigPath() const;								//  config.ini�����|
		char*	GetFormPath() const;								//	���骺���|�W 
		void	SetFormPath(char* pszPath);           
		char*	GetHelpPath() const;
		void	SetHelpPath(char* pszPath);
		
		char*	GetFormlibPath() const; 	              			//	����B�z�޿誺���|�W 
		void	SetFormLibPath(char* pszPath);   
		char*	GetMainFormName() const;	              			//	�D���骺�W��  
		void	SetMainFormName(char* pszPath);
		char*	GetToolBarPath() const; 							//	toolbar�����|
		void	SetToolBarPath(char* pszPath);
		char*	GetBmpPath() const;
		void	SetBmpPath(char* pszPath);
		
		//	fans add 2009/8/22 10:02�W��
		char*	GetLogoForm() const;
		void	SetLogoForm(char* pszPath);
		
		char*	GetLogoPath() const;
		void	SetLogoPath(char* pszPath);
		
		int		GetFocusColor() const;
		void	SetFocusColor(int nColor);
		
		int		GetLanguageType() const;
		int		GetLanguagePatch() const;
		void	SetLanguageType(int nType);
		char*	GetCurrentLanguage() const;							//	�t�Υثe���y��
		void	SetCurrentLanguage(char* pszStr);
		int		GetMachineType() const;								//	��������
		void	SetMachineType(int nType);
		
		int		GetSramSize() const;								//	sram size
		
		int		GetSramInit() const;		
		void	SetSramInit(int nInit);								//	sram ��l�ƪ��A
		
		int		GetFlashEXInit() const;								//  FlashEX��l�ƪ��A
		void	SetFlashEXInit(int nInit);							//	�]�m
		
		int		GetMaxInjectSpeed() const;							//	�g�X�̤j��
		void	SetMaxInjectSpeed(int nData);
		
		int		GetMaxChargeSpeed() const;							//	�x�Ƴ̤j��
		void	SetMaxChargeSpeed(int nData);						
		
		//	2008/12/10 09:15�W�� Fans add
		int		GetLogoFlag() const;								//	Logo flag
		void	SetLogoFlag(int nData);
		
		int		GetLogoTime() const;								
		void	SetLogoTime(int nData);
		
		int		GetModbusType() const;
		
		int		GetGifType() const;								
		void	SetGifType(int nData);
		
		int		GetDemoFlag() const;								//	Logo flag
		void	SetDemoFlag(int nData);
		
		int		GetPrintTimeFlag() const;
		void	SetPrivSwitch(int nData);
		int		GetPrivSwitch() const;
		int		GetCanvasFlag() const;
		
		//	2009-2-11 15:44:00 fans add
		int		GetMainFrameType() const;
		void	SetMainFrameType(int nData);
		
		int		GetCommType() const;
		void	SetCommType(int nData);
		
		int     GetAmmeterFlag() const;
		void	SetAmmeterFlag(int nData);
		
		int		GetTouchPanlType() const;							// touchpanl type 
		void	SetTouchPanlType(int nData);
		
		int		GetBinaryType() const;								// Binary type 
		void	SetBinaryType(int nData);
		
		int		GetStopPWDType() const;								// Stopmachine pwd type 
		void	SetStopPWDType(int nData);
		
		int		GetRemoteFlag() const;								//
		void	SetRemoteFlag(int nData);
		
		int		GetHmiCommFlag() const;								//
		void	SetHmiCommFlag(int nData);
		
		int		GetINETFontType() const;
		void	SetINETFontType(int nData);							//rw.ying 2011/6/10 11:05�W��
		
		int		GetINETHostType() const;
		void	SetINETHostType(int nData);							//Caozz 2013/3/21 �U�� 01:07:30
		
		int		GetRemoteImageType() const;							// RemoteImageType Caozz 2011/3/22 �U�� 01:29:03
		void	SetRemoteImageType(int nData);
				
		int		GetRemoteMacSwtich() const;							// RemoteImageType Caozz 2011/3/22 �U�� 01:29:03
		void	SetRemoteMacSwtich(int nData);
		
		int		GetErrorControl() const;							//Caozz 2011/4/7 �W�� 09:58:18
		void	SetErrorControl(int nData);
		
		int		GetiNetPrivFlag() const;			//rw.ying	2011/3/4 01:29�U��
		void	SetiNetPrivFlag(int nData);			//for iNet privilege
		
		int		GetOKKey() const;									//	SaveKey
		void	SetOKKey(int nData);
		
		int		GetDefaultPrivilege() const;
		void	SetDefaultPrivilege(int nDefaultPrivilege);
		
		tmSystemConfig*	GetSystemConfig() {return &m_SystemConfig;};
		
		char*	GetCurrentLanguageShowStr();						//	
		int		GetLanguageNum();
		int		GetCurrentIndex();			
		void	SetCurrentIndex(int nIndex);						
		char*	GetLanguageShowStr(int	index);
		char*	GetLanguagesBmpPath(int	index);
		char*	GetLanguageStr(int index);
		
		int		GetFontSize();
		int		GetFontConvert();
		int		GetShowFontSize();							//James add 2011/8/2 �U�� 08:14:42
		int		GetFontCode();
		char*	GetLanguagepath();              			//	�h��y�������|
		char*	GetLanguagepathHelp();
		char*	GetFontFileName();
		char*	GetLanguageFileName();
		char*	GetRecordXMLFileName();
		char*	GetViewXMLFileName();
		char*	GetFontFTFileName();
		
		
		char*	GetDspCurveFileName();
		char*	GetSimCurveFileName();
		
		char*	GetMoldSetHeadFileName();
		char*	GetMoldSetContentFileName();
		int		GetMoldSetMaxNum();
		
		char*	GetRecordSetFileName();
		int		GetRecordSetMaxNum();
		
		char*	GetErrorFileName();
		int		GetErrorMaxNum();
		
		char*	GetMoniFileName();
		int		GetMoniMaxNum();
		
		int		GetDataProcessFlag();
		
		char*	GetDataStateFileName();
		int		GetDataStateMaxNum();
		
		char*	GetDataEnergyFileName();
		int		GetDataEnergyMaxNum();
		
		char*	GetDataPowerOnOffFileName();
		int		GetDataPowerOnOffMaxNum();
		
		int		GetDataEnergy2MaxNum();	
		int		GetOnOffStateMaxNum();
		int		GetDataCanType();	
		int		GetMachineAxis(int nIndex);
		int		GetCANIndex();
		int		GetRecordType();
		char*	GetRecordFilePath();
		int		GetMaxCurveNum();	
		
		//Sunny<20070622>
		char*	GetDBID();
		char*	GetDBName();
		char*	GetDBAttr();
		char*	GetDBDefault();	
		char*	GetDBBlock();
		char*	GetDBIndex2id();
		char*	GetDBRecords();
		int		GetDBSramSize();
		
		//James add 2008/2/12 10:19�W��
		int		GetINETFlag();
		int		GetINETMacSwtich();	//Sunny<20080816>
		int		GetDSP28Flag();		
		int		GetPickerFlag();		
		int		GetAustoneFlag();	//James add for austone 2010/5/19
		int 	GetN2CFlag();		//Caozz add 2010/10/22 08:47�W��
		int 	GetM3Flag();		//Caozz add 2010/10/22 08:47�W��
		int		GetTempCurveFlag();
		int		GetConfigDealRamp();
		int		GetKeyFlag();
		int		GetKeyLedFlag();				//James add 
		int		GetLCDSizeFlag();
		int		GetLCDHeight();
		int		GetLCDWidth();
		int		GetKeyLockFlag();
		int		GetDspCom2Flag();
		int		GetCom2Type();
		int		GetLCDParaInit();
		int		GetSlaveFlag()	const;
		
		
		void	SetKeyFlag(int nFlag);
		void	SetKeyLedFlag(int nFlag);		//James add 
		void	SetINETFlag(int nFlag);
		void	SetINETMacSwtich(int nFlag);	//Sunny<20080816>
		void	SetDSP28Flag(int nFlag);		
		void	SetPickerFlag(int nFlag);
		void	SetAustoneFlag(int nFlag);		//James add for austone 2010/5/19
		void	SetN2CFlag(int nFlag);			//Caozz add 2010/10/22 08:46�W��
		void	SetM3Flag(int nFlag);			//Caozz add 2010/10/22 08:46�W��
		void	SetTempCurveFlag(int nFlag);
		void	SetConfigDealRamp(int nFlag);
		void	SetLCDSizeFlag(int nFlag);
		void	SetLCDHeight(int nHeight);
		void	SetLCDWidth(int	nWidth);
		void	SetKeyLockFlag(int nFlag);
		void	SetDspCom2Flag(int nFlag);
		void	SetCom2Type(int nFlag);
		void	SetLCDParaInit(int nFlag);
		void	SetSlaveFlag(int nFlag);
		
		//LCD
		void	SetLCDReserved(int nReserved);
		int		GetLCDReserved();
		void	SetLCDSync(int nSync);
		int		GetLCDSync();
		void	SetLCDLower_margin(int nLower_margin);
		int		GetLCDLower_margin();
		void	SetLCDRight_margin(int nRight_margin);
		int		GetLCDRight_margin();
		void	SetLCDUpper_margin(int nUpper_margin);
		int		GetLCDUpper_margin();
		void	SetLCDLeft_margin(int nLeft_margin);
		int		GetLCDLeft_margin();
		void	SetLCDVsync_len(int nVsync_len);
		int		GetLCDVsync_len();
		void	SetLCDHsync_len(int nHsync_len);
		int		GetLCDHsync_len();
		void	SetLCDYres(int nYres);
		int		GetLCDYres();
		void	SetLCDXres(int nXres);
		int		GetLCDXres();
		void	SetLCDLccr3(int nLccr3);
		int		GetLCDLccr3();
		void	SetLCDLccr0(int nLccr0);
		int		GetLCDLccr0();
		void	SetLCDPixclock(int nPixclock);
		int		GetLCDPixclock();
		void	SetLCDSignal(int nSignal);
		int		GetLCDSignal();
		
		void	SetTimerTaskType(const int nType);
		int		GetTimerTaskType()const;	
	/*------------------------------------------------------------------------------+
	|           Attributes                                                          |
	+------------------------------------------------------------------------------*/		
	private:
		CIniFile			m_IniFile;	
		CIniFile			m_SramIni;	
		int					m_nLanguageNum;
		char*				m_pszLanguages[MAX_LANGUAGE_NUM];
		char*				m_pszLanguagesBmpPath[MAX_LANGUAGE_NUM];
		
		tmSystemConfig		m_SystemConfig;
		tmLCDParaConfig		m_LCDParaConfig;
		tmLanguagesConfig	m_LanguagesConfig;
		tmLanguageConfig	m_LanguageConfig;
		tmCurveConfig		m_CurveConfig;
		tmMoldSetConfig		m_MoldSetConfig;
		tmRecordSetConfig	m_RecordSetConfig;
		tmErrorConfig		m_ErrorConfig;
		tmMoniConfig		m_MoniConfig;
		tmDataProcessConfig	m_DataProcessConfig;
		tmDBConfig			m_DBConfig;				//Sunny<20070622>
		tmLogoConfig		m_LogoConfig;
		tmiNetConfig		m_iNetConfig;
		tmRemoteConfig		m_RemoteConfig;
		tmFunctionConfig	m_FunctionConfig;
		static	CtmConfig*	m_pInstance;	
		
		
		
		
		
		
		
};

#endif
