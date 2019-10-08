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
		char*		m_pszArryFileName[DATAPROCESS_FILE_MAXCOUNT];				//�����s�x
		char		m_pszFileName[256];					//��e�g���W��  	DataFileName + FileIndex
		char		m_pszReadFileName[256];				//��eŪ���W��	DataFileName + FileIndex
		char		m_pszDataFileName[256];				//�ƾڤ�󤽦@�W��
		char*		m_pszFileName_Sub1[DATAPROCESS_FILE_MAXCOUNT];			//�U�����s�x
		char		m_pszConfigName[256];				//��e�t�m���W��
		char		m_pszDataPath[256];					//�t�m�����|
		char		m_pszDataConfigPath[256];			//
		char		m_pszFileName_Config[256];			//file name
		
		
		DWORD		m_dwFileType;		//�������		01=Config��� 02=Data���
		DWORD		m_dwFileCount; 		//���Ӽ�
		DWORD		m_dwFileMaxCount;	//���̤j�Ӽ�
		DWORD		m_dwFileNameLen;	//���W����	
		
		DWORD		m_dwFileIndex;		//��e���Ǹ�
		DWORD		m_dwReadFileIndex;	//��eŪ���Ǹ�
		
		
		DWORD		m_dwType;		//�������
		DWORD		m_dwMode;		//��Ʀs�x�Ҧ�(0=�T�w�s�x 1=�s��s�x 2=�����s�x)
		DWORD		m_dwCount;		//��Ʒ�e����`�Ӽ�
		DWORD		m_dwAllCount;	//�Ҧ�������`��
		DWORD		m_dwMaxCount;	//����`�̤j�Ӽ�
		DWORD		m_dwLen;		//��Ӹ�ƪ���
		BYTE*		m_pData;		//��Ƥ��e
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
		
		int		SetDataPath(int nType,	int	nPathFlag);				//�]�m���s�x���|
		
		//BOOL	AddEnergy(int nIndex);
		char*	GetlibConfig(char*	pName);
		int		ProcessEnergy();					//�B�z���
		int		ProcessPowerOnOff();				//�B�z�}�����ɶ�
		int		ProcessState();						//�B�z�ʧ@���A����
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
		long			m_lSaveEnergyMode;			//0==���T�w�ɶ��s�x�A1=�����I�ɶ��s�x�A2=���Ͳ��}�Ҽ��k�s�s�x
		
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

