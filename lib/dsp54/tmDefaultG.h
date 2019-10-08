#ifndef     D__TMDEFAULT_LIB
#define     D__TMDEFAULT_LIB

#include	"../../default.h"
#include	"../../readdata.h"
//#include	"../../tmconfig.h"
#include	"../../inifile.h"

const	int	CNT_INT_MAX_DEFAULT_NUM = 256;

class	CtmDefaultG: public CtmDefault
{
	DECLARE_DYNCREATE(CtmDefaultG)
	public:
		CtmDefaultG();
		~CtmDefaultG();
	public:
		int		SetDefault();	//	override the CtmDefault funtion
		
		int		CreateSelf();	//	override the CtmPackClass funtion
		int		FreeSelf();		//	override the CtmPackClass funtion
 	protected:
 		BOOL	GetFileName();
 		void	HEX2String(WORD wCode,char* psz);
 		void	Init();
 		int		GetVerInfo();
 		int		LoadAttr(char*	cszDBID, int nPrecision);
 		
 		void	SetDefaultBy8C();
 		void	SetRUBDefaultBy86();
 		void	SetDefaultByDC(); 	//Andy 20090724
 		void	SetDefaultJ5();
 		void	SetDefaultByF2();  //JOYCE2011-3-3 FOR LUB2
 		void	SetDefaultByF4();  //JOYCE2011-7-7
 		void 	SetDefaultInit();
 		void 	SetAttrInit();			//Deal precision
 		void	SetVersionInf();		//for String Version
 		void	SetNodeID();
 		void	SetDefaultByFunc();
 		void	SetDefaultUser();//JOYCE2012-2-1 for inet
		void	ReadEnergyDataFrom();//JOYCE2010-7-22 
		void  ReadShotCount();//JOYCE2010-10-12 累積運行模數
		void	ReadMoldWeight(); //JOYCE2011-6-7 
		void	ReadFlowRatio();
		void	ReadEjtData();
		void	ReadRoteTimeOpt();//wangli2014-9-10 15:45:53 
		void	ReadOneJarInjOpt();
		void	ReadOneJarInjOptDC();	//wangli2014-10-27 11:52:59 雙色機 單杠注射
		void	ReadLubricate1();//JOYCE2011-8-3 lubricate
		void	ReadLubricate2();//JOYCE2011-8-3 lubricate
		void	ReadLubricate3();/*wangli2015-11-13 9:40:57*/
		void	SetLubricate();//JOYCE2011-8-23 lubricate
		void	ReadSDOOR();
		void	ReadLubType();
		void	ReadOldLub();		
		BOOL	ReadBlock(char* pszFileName, WORD wBlockID);
		void	ReadParaPassword();
		void	SetAdjCnt();
		void	ReadLubOpt();
		void	ReadCOM2AmmeterOpt();
		void	ReadWarmTime();
		void	ReadRotateOpt();
		void	Read28RotateData();
		void	ReadTBARData();
		void  ReadInjhSynchro();
		void  ReadOpnCntData();
		void	ReadNotclmptime();/*wangli2015-12-10 16:56:21 馬達關閉時間*/
		void	ReadSetGrad();
		void	SetPowonLubHr();/*wangli2016-1-19 14:14:58重置后  設定值 與 累計值進行比較*/
		void  ReadWarningCnt();
		void  TestMode();
		void  ReadParallelCnt();
		void	MoldTempSet();
		void  	MoldTempUse(int nMTBegin, int nMTEnd, BOOL bMTFlag);
		void    TempPrecentClear(int nMTBegin, int nMTEnd, BOOL bMTFlag,int nGroup);
		void	ReadAutoRunTime();
		void	SetMrtTempNUse();
		void  SetDefaultBySVRT();
		WORD	GetMoldTempType();
		BOOL	m_b3LUB;
		BOOL	m_bALUB;
		BOOL	m_bOLDLUB;
		BOOL	m_bVrt;
		BOOL	m_bDC;
		BOOL	m_biNet;
		BOOL	m_bStepLub;
		BOOL	m_bAmmeter;
		BOOL	m_bPMTEST;
		BOOL	m_bSVRT;
		BOOL 	nDSP28flag;
		BOOL	m_b28Clmp;
		BOOL	m_bT11Port19;
		BOOL	m_bMold36;
		BOOL	m_bJUIII;
 	protected: 		
 		CtmReadData	m_ReadData;
 		//外掛文件名
 		char*		m_pszFileDefaultID;
 		char*		m_pszFileDefaultValue;
 		
 		char*		m_pszFileAttrID;
 		char*		m_pszFileAttrValue;
 		
 		char*		m_pszFileDefaultSID; //for String Type
 		char*		m_pszFileDefaultSValue; //for String Type
 		
 		char*		m_pszFileVersionSID; //for String Version
 		char*		m_pszFileVersionSValue; //for String Version
 		//buffer for int type 		
 		char*		m_szDefaultID[CNT_INT_MAX_DEFAULT_NUM];
		int			m_DefaultValue[CNT_INT_MAX_DEFAULT_NUM];
		//buffer for str type 	
		char*		m_szDefaultSID[16]; //for String Type
		char*		m_DefaultSValue[16]; //for String Type
		char*		m_szVersionSID[16]; //for String Version
		char*		m_VersionSValue[16]; //for String Version
				
		DWORD		m_dwVers;		
		//為了節省內存，統一由m_szDefaultID,m_DefaultValue替代掉!
		//char*		m_sz8CID[CNT_INT_MAX_DEFAULT_NUM];
		//char*		m_szRubID[CNT_INT_MAX_DEFAULT_NUM];
		////char*		m_szJ5ID[CNT_INT_MAX_DEFAULT_NUM];
		//char*		m_szDCID[CNT_INT_MAX_DEFAULT_NUM]; //Andy 20090724
		//
		//int			m_8CValue[CNT_INT_MAX_DEFAULT_NUM];
		//int			m_RUBValue[CNT_INT_MAX_DEFAULT_NUM];
		////int			m_J5Value[CNT_INT_MAX_DEFAULT_NUM];
		//int			m_DCValue[CNT_INT_MAX_DEFAULT_NUM]; //Andy 20090724
	
};


#ifndef	D_BEOBJECT_CTMDEFAULTG
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

