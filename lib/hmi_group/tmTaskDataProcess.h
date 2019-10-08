#ifndef		D__TMTASKDATAPROCESS_V01
#define		D__TMTASKDATAPROCESS_V01
#include	"../../taskdataprocess.h"
#include	"../../dataprocesssave.h"
#include	"../../main.h"
#include	"../libDataSave.h"

//============================================
// configfile
// type  |  count | len		|	Name	| len	| Name ......  
//   4B		4B		4B      	n * B      4B      n * B
//============================================
//============================================
// datafile
// type	 |	datatype | mode	   |  count | len  | data 
//   4B       4B         4B         4B     4B     n * B
//============================================

#define		DATAPROCESS_FILENAME_MAXLEN			256
#define		DATAPROCESS_FILE_MAXCOUNT			10
#define		DATAPROCESS_FILEDATA_MAXCOUNT		60

//class	CtmDataProcessRead: public CObject
//{
//	DECLARE_DYNCREATE(CtmDataProcessRead)
//	public:
//		CtmDataProcessRead();
//		~CtmDataProcessRead();
//	public:	
//		int		Read_Config();
//		int		Read_Data();
//};

class	CtmDataProcessSave:public CDataProcessSave
{
	DECLARE_DYNCREATE(CtmDataProcessSave)
	public:
		CtmDataProcessSave();
		~CtmDataProcessSave();
	public:
		int			SetDataPath(char*	pszName);
		int			SetDataConfigPath(char*	pszName);
		int			SetConfigName(char*	pszName);
		int			SetDataFileName(char*	pszName);
		int			SetDataFileName_Config(char*	pszName);
		int			GetInitInfo();
		int			GetAllCount();
		int			GetMode(int nType);
		int			Read(int	nType,int	nIndex, 		int nSize, 	void* pData);
		int			ReadS(int	nType,int	nStartIndex,	int nCount, int nSize, 	void* pData);
		int			Write(int	nType,int	nIndex, 		int nSize, 	void* pData);
		int			WriteS(int	nType,int	nStartIndex,	int nCount, int nSize,	void* pData);
		
		int			ReadConfig(char**	pFileName);
		int			WriteConfig(char*	pFileName);
		
		int			CheckConfigFile(char*	pszFileName, int	nType);
		int			CheckDataFile(char*	pszFileName, int	nType);
		int			CheckDir(char*	pszFileName);
		int			CheckFile(char*	pszFileName);
		//char*		GetCurrentFileName();
		int			SetFileMaxCount(DWORD	dwFileMaxCount);
		int			SetMaxCount(DWORD	dwMaxCount);
		
		int			CalculateWriteFileName(int	nIndex);
		int			CalculateReadFileName(int	nIndex);
	public:
		char*		m_pszArryFileName[DATAPROCESS_FILE_MAXCOUNT];				//分文件存儲
		char		m_pszFileName[256];					//當前寫文件名稱  	DataFileName + FileIndex
		char		m_pszReadFileName[256];				//當前讀文件名稱	DataFileName + FileIndex
		char		m_pszDataFileName[256];				//數據文件公共名稱
		char*		m_pszFileName_Sub1[DATAPROCESS_FILE_MAXCOUNT];			//下階文件存儲
		char		m_pszConfigName[256];				//當前配置文件名稱
		char		m_pszDataPath[256];					//配置文件路徑
		char		m_pszDataConfigPath[256];			//
		char		m_pszFileName_Config[256];			//file name
		
		
		DWORD		m_dwFileType;		//文件類型		01=Config文件 02=Data文件
		DWORD		m_dwFileCount; 		//文件個數
		DWORD		m_dwFileMaxCount;	//文件最大個數
		DWORD		m_dwFileNameLen;	//文件名長度	
		
		DWORD		m_dwFileIndex;		//當前文件序號
		DWORD		m_dwReadFileIndex;	//當前讀文件序號
		
		
		DWORD		m_dwType;		//資料類型
		DWORD		m_dwMode;		//資料存儲模式(0=固定存儲 1=連續存儲 2=分文件存儲)
		DWORD		m_dwCount;		//資料當前文件中總個數
		DWORD		m_dwAllCount;	//所有文件資料總數
		DWORD		m_dwMaxCount;	//資料總最大個數
		DWORD		m_dwLen;		//單個資料長度
		BYTE*		m_pData;		//資料內容
		CtmFile		m_File;
};

class  CtmTaskDataProcess: public CTaskDataProcess
{
	DECLARE_DYNCREATE(CtmTaskDataProcess)
	public:
		CtmTaskDataProcess();
		~CtmTaskDataProcess();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
	public:
		void	DataProcess_Mode(int message, WPARAM wParam, LPARAM lParam);
		int		Init();
		void	Init_DataEnergy();
		void	Init_DataState();
		void	Init_DataPowerOnOff();
		void	Init_OnOffState();
		
		int		SetDataPath(int nType,	int	nPathFlag);				//設置文件存儲路徑
		
		//BOOL	AddEnergy(int nIndex);
		char*	GetlibConfig(char*	pName);
		int		ProcessEnergy();					//處理能耗
		int		ProcessPowerOnOff();				//處理開關機時間
		int		ProcessState();						//處理動作狀態切換
		int		SaveEnergy(int nType, int nIndex = -1);
		int		SaveState(int nMode, int nIndex = -1);
		int		SavePowerOnOff(int nMode, int nIndex = -1);
		int		SaveOnOffState(int nMode, WORD	wSubIndex, WORD nState, int nIndex = -1);
		int		ProcessMoniOnline();//JOYCE2012-4-26 
		int		ProcessMoniSave();//JOYCE2012-4-26 
		int		CopyFile(char*	pszSrc,	char*	pszDst);
		int		ProcessOnOffState();
		int		GetDiskFreeSize(char* pszMountPoint);
		
		int		ClearEnergyLog();
		int		ClearStateLog();
		int		ClearPowerOnOff();
		int		ClearOnOffState();
		int		CalculateTime(int	nType, tmDATETIME datetime_start, tmDATETIME datetime_end);
		UTIME   MakeTime(tmDATETIME *tm, int nType = 0);
		/*virtual	*/int		GetData(int		nType, 	int nIndex, 	int nSize, 	void* pData);
		/*virtual	*/int		GetDataS(int	nType,	int	nStartIndex,int nCount, int nSize, 	void* pData);
		/*virtual	*/int		SetData(int		nType,	int	nIndex, 	int nSize, 	void* pData);
		/*virtual	*/int		SetDataS(int	nType,	int	nStartIndex,int nCount, int nSize,	void* pData);
	private:
		CtmDataProcessSave	m_DataProcess[DATAPROCESS_TYPE_MAXCOUNT];
		char*		m_pConfig[1024];
		char*		m_pConfigValue[1024];
		char		m_cszlibConfig_Path[256];
		int			m_nlibConfigCount;
		long		m_lCounter;
		BOOL		m_bCapacitySaveFlag;
		BOOL	  m_bMoniActFlag;
		static		BOOL	m_bPowerOnFlag;
	private:
		long			m_lEnergyTimer_Const;
		long			m_lEnergyTimer_Hour;
		long			m_lSaveEnergyMode;			//0==按固定時間存儲，1=按整點時間存儲，2=按生產開模數歸零存儲
		
		STATEMODE		m_CurrentStateMode;
		STATEMODE		m_StateModeIndex;
		ONOFFSTATE		m_CurrentOnOffState;
		ONOFFSTATE		m_OnOffStateIndex;
		TMCAPACITY		m_CurrenttmCapacity;
		TMCAPACITY		m_tmCapacityIndex;
		TMPOWERONOFF 	m_CurrenttmPowerOnOff;
		TMPOWERONOFF	m_tmPowerOnOffIndex;
		
};


#ifndef	D_BEOBJECT_CTMTASKDATAPROCESS	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

