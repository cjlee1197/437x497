#ifndef     D__TMDEFAULT_LIB
#define     D__TMDEFAULT_LIB

#include	"../../freedata.h"
#include	"../../readdata.h"
#include	"../../inifile.h"
#include	"../../utils.h"
#include	"../../oil54struct.h"

const	int	CNT_INT_MAX_FREE_NUM = 256;
const	int	CNT_INT_FILE_LEN	 = 256;
const	int	CNT_INT_FIXDATA_LEN	 = 16;
const	int	CNT_INT_FIXDATA_TOTALLEN	 = 512;
const	int CNT_INT_FIXDATA_NUM	 = 32;
const	int CNT_INT_SPECDB_START = CNT_INT_FIXDATA_NUM;
const	int	CNT_INT_SPECDAT_LEN  = 1024;



typedef	enum tagtmSpecType 
{
	tmSpecStart = 0,
	//====================================================
	//	hmi spec
	//====================================================
	tmHmi0Spec = tmSpecStart,
	tmHmi1Spec,
	tmHmi2Spec,
	tmHmi3Spec,
	tmHmi4Spec,
	tmHmi5Spec,
	tmHmi6Spec,
	tmHmi7Spec,
	tmHmi8Spec,
	tmHmi9Spec,
	tmHmi10Spec,
	tmHmi11Spec,
	tmHmi12Spec,
	tmHmi13Spec,
	tmHmi14Spec,
	tmHmi15Spec,
	tmHmi16Spec,
	tmHmi17Spec,
	tmHmi18Spec,
	tmHmi19Spec,
	tmHmi20Spec,
	tmHmi21Spec,
	tmHmi22Spec,
	tmHmi23Spec,
	tmHmi24Spec,
	tmHmi25Spec,
	tmHmi26Spec,
	tmHmi27Spec,
	tmHmi28Spec,
	tmHmi29Spec,
	tmHmi30Spec,
	tmHmi31Spec,
	tmHmi32Spec,
	tmHmi33Spec,
	tmHmi34Spec,
	tmHmi35Spec,
	tmHmi36Spec,
	tmHmi37Spec,
	tmHmi38Spec,
	tmHmi39Spec,
	tmHmi40Spec,
	tmHmi41Spec,
	tmHmi42Spec,
	tmHmi43Spec,
	tmHmi44Spec,
	tmHmi45Spec,
	tmHmi46Spec,
	tmHmi47Spec,
	tmHmi48Spec,
	tmHmi49Spec,
	tmHmi50Spec,
	tmHmi51Spec,
	tmHmi52Spec,
	tmHmi53Spec,
	tmHmi54Spec,
	tmHmi55Spec,
	tmHmi56Spec,
	tmHmi57Spec,
	tmHmi58Spec,
	tmHmi59Spec,
	tmHmi60Spec,
	tmHmi61Spec,
	tmHmi62Spec,
	tmHmi63Spec,
	tmHmi64Spec,
	tmHmi65Spec,
	tmHmi66Spec,
	tmHmi67Spec,
	tmHmi68Spec,
	tmHmi69Spec,
	tmHmi70Spec,
	tmHmi71Spec,
	tmHmi72Spec,
	tmHmi73Spec,
	tmHmi74Spec,
	tmHmi75Spec,
	tmHmi76Spec,
	tmHmi77Spec,
	tmHmi78Spec,
	tmHmi79Spec,
	tmHmi80Spec,
	tmHmi81Spec,
	tmHmi82Spec,
	tmHmi83Spec,
	tmHmi84Spec,
	tmHmi85Spec,
	tmHmi86Spec,
	tmHmi87Spec,
	tmHmi88Spec,
	tmHmi89Spec,
	tmHmi90Spec,
	tmHmi91Spec,
	tmHmi92Spec,
	tmHmi93Spec,
	tmHmi94Spec,
	tmHmi95Spec,
	tmHmi96Spec,
	tmHmi97Spec,
	tmHmi98Spec,
	tmHmi99Spec,
	//====================================================
	//	lcd spec
	//====================================================
	
	tmLcd0Spec,//100
	tmLcd1Spec,
	tmLcd2Spec,
	tmLcd3Spec,
	tmLcd4Spec,
	tmLcd5Spec,
	tmLcd6Spec,
	tmLcd7Spec,
	tmLcd8Spec,
	tmLcd9Spec,
	tmLcd10Spec,
	tmLcd11Spec,
	tmLcd12Spec,
	tmLcd13Spec,
	tmLcd14Spec,
	tmLcd15Spec,
	tmLcd16Spec,
	tmLcd17Spec,
	tmLcd18Spec,
	tmLcd19Spec,
	tmLcd20Spec,
	tmLcd21Spec,
	tmLcd22Spec,
	tmLcd23Spec,
	tmLcd24Spec,
	tmLcd25Spec,
	tmLcd26Spec,
	tmLcd27Spec,
	tmLcd28Spec,
	tmLcd29Spec,
	tmLcd30Spec,
	tmLcd31Spec,
	tmLcd32Spec,
	tmLcd33Spec,
	tmLcd34Spec,
	tmLcd35Spec,
	tmLcd36Spec,
	tmLcd37Spec,
	tmLcd38Spec,
	tmLcd39Spec, 
	tmLcd40Spec,
	tmLcd41Spec,
	tmLcd42Spec,
	tmLcd43Spec,
	tmLcd44Spec,
	tmLcd45Spec,
	tmLcd46Spec,
	tmLcd47Spec,
	tmLcd48Spec,
	tmLcd49Spec,     
	tmLcd50Spec,
	tmLcd51Spec,
	tmLcd52Spec,
	tmLcd53Spec,
	tmLcd54Spec,
	tmLcd55Spec,
	tmLcd56Spec,
	tmLcd57Spec,
	tmLcd58Spec,
	tmLcd59Spec,
	tmLcd60Spec,
	tmLcd61Spec,
	tmLcd62Spec,
	tmLcd63Spec,
	tmLcd64Spec,
	tmLcd65Spec,
	tmLcd66Spec,
	tmLcd67Spec,
	tmLcd68Spec,
	tmLcd69Spec,
	tmLcd70Spec,
	tmLcd71Spec,
	tmLcd72Spec,
	tmLcd73Spec,
	tmLcd74Spec,
	tmLcd75Spec,
	tmLcd76Spec,
	tmLcd77Spec,
	tmLcd78Spec,
	tmLcd79Spec,
	tmLcd80Spec,
	tmLcd81Spec,
	tmLcd82Spec,
	tmLcd83Spec,
	tmLcd84Spec,
	tmLcd85Spec,
	tmLcd86Spec,
	tmLcd87Spec,
	tmLcd88Spec,
	tmLcd89Spec,      
	tmLcd90Spec,
	tmLcd91Spec,
	tmLcd92Spec,
	tmLcd93Spec,
	tmLcd94Spec,
	tmLcd95Spec,
	tmLcd96Spec,
	tmLcd97Spec,
	tmLcd98Spec,
	tmLcd99Spec,
	
	//====================================================
	//	dsp spec:dsp28 or dsp54 and so on
	//====================================================
	tmDsp0Spec, //200
	tmDsp1Spec, 
	tmDsp2Spec, 
	tmDsp3Spec, 
	tmDsp4Spec, 
	tmDsp5Spec, 
	tmDsp6Spec, 
	tmDsp7Spec,
	tmDsp8Spec,
	tmDsp9Spec,
	tmDsp10Spec, 
	tmDsp11Spec, 
	tmDsp12Spec, 
	tmDsp13Spec, 
	tmDsp14Spec, 
	tmDsp15Spec, 
	tmDsp16Spec, 
	tmDsp17Spec,
	tmDsp18Spec,
	tmDsp19Spec,
	tmDsp20Spec, 
	tmDsp21Spec, 
	tmDsp22Spec, 
	tmDsp23Spec, 
	tmDsp24Spec, 
	tmDsp25Spec, 
	tmDsp26Spec, 
	tmDsp27Spec,
	tmDsp28Spec,
	tmDsp29Spec,
	tmDsp30Spec, 
	tmDsp31Spec, 
	tmDsp32Spec, 
	tmDsp33Spec, 
	tmDsp34Spec, 
	tmDsp35Spec, 
	tmDsp36Spec, 
	tmDsp37Spec,
	tmDsp38Spec,
	tmDsp39Spec,  
	tmDsp40Spec, 
	tmDsp41Spec, 
	tmDsp42Spec, 
	tmDsp43Spec, 
	tmDsp44Spec, 
	tmDsp45Spec, 
	tmDsp46Spec, 
	tmDsp47Spec,
	tmDsp48Spec,
	tmDsp49Spec,
	tmDsp50Spec, 
	tmDsp51Spec, 
	tmDsp52Spec, 
	tmDsp53Spec, 
	tmDsp54Spec, 
	tmDsp55Spec, 
	tmDsp56Spec, 
	tmDsp57Spec,
	tmDsp58Spec,
	tmDsp59Spec,
	tmDsp60Spec, 
	tmDsp61Spec, 
	tmDsp62Spec, 
	tmDsp63Spec, 
	tmDsp64Spec, 
	tmDsp65Spec, 
	tmDsp66Spec, 
	tmDsp67Spec,
	tmDsp68Spec,
	tmDsp69Spec,
	tmDsp70Spec, 
	tmDsp71Spec, 
	tmDsp72Spec, 
	tmDsp73Spec, 
	tmDsp74Spec, 
	tmDsp75Spec, 
	tmDsp76Spec, 
	tmDsp77Spec,
	tmDsp78Spec,
	tmDsp79Spec,
	tmDsp80Spec, 
	tmDsp81Spec, 
	tmDsp82Spec, 
	tmDsp83Spec, 
	tmDsp84Spec, 
	tmDsp85Spec, 
	tmDsp86Spec, 
	tmDsp87Spec,
	tmDsp88Spec,
	tmDsp89Spec,
	tmDsp90Spec, 
	tmDsp91Spec, 
	tmDsp92Spec, 
	tmDsp93Spec, 
	tmDsp94Spec, 
	tmDsp95Spec, 
	tmDsp96Spec, 
	tmDsp97Spec,
	tmDsp98Spec,
	tmDsp99Spec,

	//====================================================
	//	Dirver spec
	//====================================================
	tmDriver0Spec,//299
	tmDriver1Spec,
	tmDriver2Spec,
	tmDriver3Spec,
	tmDriver4Spec,
	tmDriver5Spec,
	tmDriver6Spec,
	tmDriver7Spec,
	tmDriver8Spec,
	tmDriver9Spec,
	tmDriver10Spec,
	tmDriver11Spec,
	tmDriver12Spec,
	tmDriver13Spec,
	tmDriver14Spec,
	tmDriver15Spec,
	tmDriver16Spec,
	tmDriver17Spec,
	tmDriver18Spec,
	tmDriver19Spec,
	tmDriver20Spec,
	tmDriver21Spec,
	tmDriver22Spec,
	tmDriver23Spec,
	tmDriver24Spec,
	tmDriver25Spec,
	tmDriver26Spec,
	tmDriver27Spec,
	tmDriver28Spec,
	tmDriver29Spec,
	tmDriver30Spec,
	tmDriver31Spec,
	tmDriver32Spec,
	tmDriver33Spec,
	tmDriver34Spec,
	tmDriver35Spec,
	tmDriver36Spec,
	tmDriver37Spec,
	tmDriver38Spec,
	tmDriver39Spec,
	tmDriver40Spec,
	tmDriver41Spec,
	tmDriver42Spec,
	tmDriver43Spec,
	tmDriver44Spec,
	tmDriver45Spec,
	tmDriver46Spec,
	tmDriver47Spec,
	tmDriver48Spec,
	tmDriver49Spec,
	tmDriver50Spec,
	tmDriver51Spec,
	tmDriver52Spec,
	tmDriver53Spec,
	tmDriver54Spec,
	tmDriver55Spec,
	tmDriver56Spec,
	tmDriver57Spec,
	tmDriver58Spec,
	tmDriver59Spec,
	tmDriver60Spec,
	tmDriver61Spec,
	tmDriver62Spec,
	tmDriver63Spec,
	tmDriver64Spec,
	tmDriver65Spec,
	tmDriver66Spec,
	tmDriver67Spec,
	tmDriver68Spec,
	tmDriver69Spec,
	tmDriver70Spec,
	tmDriver71Spec,
	tmDriver72Spec,
	tmDriver73Spec,
	tmDriver74Spec,
	tmDriver75Spec,
	tmDriver76Spec,
	tmDriver77Spec,
	tmDriver78Spec,
	tmDriver79Spec,    
	tmDriver80Spec,
	tmDriver81Spec,
	tmDriver82Spec,
	tmDriver83Spec,
	tmDriver84Spec,
	tmDriver85Spec,
	tmDriver86Spec,
	tmDriver87Spec,
	tmDriver88Spec,
	tmDriver89Spec,
	tmDriver90Spec,
	tmDriver91Spec,
	tmDriver92Spec,
	tmDriver93Spec,
	tmDriver94Spec,
	tmDriver95Spec,
	tmDriver96Spec,
	tmDriver97Spec,
	tmDriver98Spec,
	tmDriver99Spec,
	
	//====================================================
	//	Motor spec
	//====================================================
	tmMotor0Spec,//400
	tmMotor1Spec,
	tmMotor2Spec,
	tmMotor3Spec,
	tmMotor4Spec,
	tmMotor5Spec,
	tmMotor6Spec,
	tmMotor7Spec,
	tmMotor8Spec,
	tmMotor9Spec,
	tmMotor10Spec,
	tmMotor11Spec,
	tmMotor12Spec,
	tmMotor13Spec,
	tmMotor14Spec,
	tmMotor15Spec,
	tmMotor16Spec,
	tmMotor17Spec,
	tmMotor18Spec,
	tmMotor19Spec,
	tmMotor20Spec,
	tmMotor21Spec,
	tmMotor22Spec,
	tmMotor23Spec,
	tmMotor24Spec,
	tmMotor25Spec,
	tmMotor26Spec,
	tmMotor27Spec,
	tmMotor28Spec,
	tmMotor29Spec,
	tmMotor30Spec,
	tmMotor31Spec,
	tmMotor32Spec,
	tmMotor33Spec,
	tmMotor34Spec,
	tmMotor35Spec,
	tmMotor36Spec,
	tmMotor37Spec,
	tmMotor38Spec,
	tmMotor39Spec,
	tmMotor40Spec,
	tmMotor41Spec,
	tmMotor42Spec,
	tmMotor43Spec,
	tmMotor44Spec,
	tmMotor45Spec,
	tmMotor46Spec,
	tmMotor47Spec,
	tmMotor48Spec,
	tmMotor49Spec,
	tmMotor50Spec,
	tmMotor51Spec,
	tmMotor52Spec,
	tmMotor53Spec,
	tmMotor54Spec,
	tmMotor55Spec,
	tmMotor56Spec,
	tmMotor57Spec,
	tmMotor58Spec,
	tmMotor59Spec,
	tmMotor60Spec,
	tmMotor61Spec,
	tmMotor62Spec,
	tmMotor63Spec,
	tmMotor64Spec,
	tmMotor65Spec,
	tmMotor66Spec,
	tmMotor67Spec,
	tmMotor68Spec,
	tmMotor69Spec,
	tmMotor70Spec,
	tmMotor71Spec,
	tmMotor72Spec,
	tmMotor73Spec,
	tmMotor74Spec,
	tmMotor75Spec,
	tmMotor76Spec,
	tmMotor77Spec,
	tmMotor78Spec,
	tmMotor79Spec,  
	tmMotor80Spec,
	tmMotor81Spec,
	tmMotor82Spec,
	tmMotor83Spec,
	tmMotor84Spec,
	tmMotor85Spec,
	tmMotor86Spec,
	tmMotor87Spec,
	tmMotor88Spec,
	tmMotor89Spec,
	tmMotor90Spec,
	tmMotor91Spec,
	tmMotor92Spec,
	tmMotor93Spec,
	tmMotor94Spec,
	tmMotor95Spec,
	tmMotor96Spec,
	tmMotor97Spec,
	tmMotor98Spec,
	tmMotor99Spec,


	//====================================================
	//	bomp spec
	//====================================================
	tmBomp0Spec,//501
	tmBomp1Spec,
	tmBomp2Spec,
	tmBomp3Spec,
	tmBomp4Spec,
	tmBomp5Spec,
	tmBomp6Spec,
	tmBomp7Spec,
	tmBomp8Spec,
	tmBomp9Spec,
	tmBomp10Spec,
	tmBomp11Spec,
	tmBomp12Spec,
	tmBomp13Spec,
	tmBomp14Spec,
	tmBomp15Spec,
	tmBomp16Spec,
	tmBomp17Spec,
	tmBomp18Spec,
	tmBomp19Spec,     
	tmBomp20Spec,
	tmBomp21Spec,
	tmBomp22Spec,
	tmBomp23Spec,
	tmBomp24Spec,
	tmBomp25Spec,
	tmBomp26Spec,
	tmBomp27Spec,
	tmBomp28Spec,
	tmBomp29Spec,
	tmBomp30Spec,
	tmBomp31Spec,
	tmBomp32Spec,
	tmBomp33Spec,
	tmBomp34Spec,
	tmBomp35Spec,
	tmBomp36Spec,
	tmBomp37Spec,
	tmBomp38Spec,
	tmBomp39Spec,   
	tmBomp40Spec,
	tmBomp41Spec,
	tmBomp42Spec,
	tmBomp43Spec,
	tmBomp44Spec,
	tmBomp45Spec,
	tmBomp46Spec,
	tmBomp47Spec,
	tmBomp48Spec,
	tmBomp49Spec,        
	tmBomp50Spec,
	tmBomp51Spec,
	tmBomp52Spec,
	tmBomp53Spec,
	tmBomp54Spec,
	tmBomp55Spec,
	tmBomp56Spec,
	tmBomp57Spec,
	tmBomp58Spec,
	tmBomp59Spec,
	tmBomp60Spec,
	tmBomp61Spec,
	tmBomp62Spec,
	tmBomp63Spec,
	tmBomp64Spec,
	tmBomp65Spec,
	tmBomp66Spec,
	tmBomp67Spec,
	tmBomp68Spec,
	tmBomp69Spec,
	tmBomp70Spec,
	tmBomp71Spec,
	tmBomp72Spec,
	tmBomp73Spec,
	tmBomp74Spec,
	tmBomp75Spec,
	tmBomp76Spec,
	tmBomp77Spec,
	tmBomp78Spec,
	tmBomp79Spec,
	tmBomp80Spec,
	tmBomp81Spec,
	tmBomp82Spec,
	tmBomp83Spec,
	tmBomp84Spec,
	tmBomp85Spec,
	tmBomp86Spec,
	tmBomp87Spec,
	tmBomp88Spec,
	tmBomp89Spec,
	tmBomp90Spec,
	tmBomp91Spec,
	tmBomp92Spec,
	tmBomp93Spec,
	tmBomp94Spec,
	tmBomp95Spec,
	tmBomp96Spec,
	tmBomp97Spec,
	tmBomp98Spec,
	tmBomp99Spec,
	
	
	tmEndSpecType
	
}tmSpecType;

typedef	enum tagtmHMIFixed
{
	//	cpu board
	tmHMIType = 0,
	tmHMIName, 
	tmHMIDate0,
	tmHMIDate1,
	
	//	lcd 
	tmLCDType, 
	tmLCDName,
	tmLCDDate0,
	tmLCDDate1,
	
	//	keyboard
	tmKeyType,
	tmKeyName,
	tmKeyDate0,
	tmKeyDate1,
	
	//	software
	tmHMIVersion0,
	tmHMIVersion1,
	tmHMIVersion2,
	tmHMIVersion3
	
} tmHMIFixed;

typedef	enum tagtmDspFixed
{
	//	cpu board
	tmDspType = 0,
	tmDspName, 
	tmDspDate0,
	tmDspDate1,
	
	//	IO 
	tmIOType
} tmDspFixed;

typedef	enum tagtmDriverFixed
{
	//	cpu board
	tmDriverType = 0,
	tmDriverName, 
	tmDriverDate0,
	tmDriverDate1
	
} tmDriverFixed;

typedef	enum tagtmMotorFixed
{
	//	cpu board
	tmMotorType = 0,
	tmMotorName, 
	tmMotorDate0,
	tmMotorDate1
} tmMotorFixed;

typedef	enum tagtmBompFixed
{
	//	cpu board
	tmBompType = 0,
	tmBompName, 
	tmBompDate0,
	tmBompDate1
} tmBompFixed;

typedef	struct tagtmSpecFInfo
{
	tmSpecType 	m_SpecType;
	int			m_nPos;			//	
	int			m_nFixedNo;		//	the max is CNT_INT_FIXDATA_NUM
	
} tmSpecInfo;



class	CtmFreeDataG: public CtmFreeData
{
	DECLARE_DYNCREATE(CtmFreeDataG)
	public:
		CtmFreeDataG();
		~CtmFreeDataG();
	public:
		virtual	int		CreateSelf();
		virtual	int		FreeSelf();
		
		virtual	int		LoadIDTable(char* pszIDTableFile);
		virtual	int		SaveDataToFile(char* pszFile);
		virtual	int		GetDataFromFile(char* pszFile);
		virtual	int		SearchIDFromTable(char* cszIDName);
		virtual	int		SearchIndexFromTable(int nIndex);
	protected:
		virtual	int		GetSize();
 	protected: 		
 		CtmReadData	m_ReadData;
 		char		m_szIDFile[128];
 		char		m_szValueFile[128]; 		
 		char*		m_szFreeDataID[CNT_INT_MAX_FREE_NUM];	
 		
 		int			m_nIDNum;
};


class	CtmSpecDataG: public CtmSpecData
{
	DECLARE_DYNCREATE(CtmSpecDataG)
	
	public:
		CtmSpecDataG();
		~CtmSpecDataG();
	public:
		virtual	int		CreateSelf();
		virtual	int		FreeSelf();
		
		
		virtual	int		SaveFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize);
		virtual	int		SaveDBData(int nSpecType);
		virtual	int		SaveDBDataPos(int WritePos,int nSpecType);
		virtual	int		GetFixedData(int nSpecType, int nFixedNum, char* pBuf, int nSize);
		virtual	int		GetDBData(int nSpecType);
		virtual	int		GetDBDataPos(int WritePos,int nSpecType);
		
		virtual	int		CleanDBData(int WritePos);
		
		virtual	int		SaveFixedDataTo();
		virtual	int		SaveFixedDataTo(int nSpecType);
		virtual	int		SaveDBDataTo();
		virtual	int		SaveDBDataTo(int nSpecType);
		virtual	int 	SaveDataTo();
		virtual	int		SaveAllDataTo();
		
		
		virtual	int		GetFixedDataFrom();
		virtual	int		GetFixedDataFrom(int nSpecType);
		virtual	int		GetDBDataFrom();
		virtual	int		GetDBDataFrom(int nSpecType);
		virtual	int		GetDataFrom();
		virtual	int		GetAllDataFrom();
	protected:
		int		WriteToFile(char* pszFile, char* pBuf, int nSize, int nPos);
		int		ReadFromFile(char* pszFile, char* pOut, int nSize, int nPos);
		int		GetPos(int nSpecType, int nFixedNum);// Total Length = 1024 Byte
		int		GetPos2(int nSpecType, int nFixedNum);// Total Length = 512 Byte
		
		int		ReadDBID(char* pszIDFile, char* pszID[]);
		int		GetDBFile(int nSpecType, char* pszOutFileName);
		int		GetBufferFromDB(char* pOut,int nSize, char* pszID[], int nNum); 	
		int		WriteToDB(char* pIn, int nSize, char* pszID[], int nNum);
		
		
		
		int		WriteSpecTo(char* pszSource, char* pszDest, int nSpecType, int nFixedNum, int nSize);
		int		CopyFile(char* pszSource, char* pszDest);
	protected:
		char	m_szSpecDataFile[CNT_INT_FILE_LEN];				//	the default is /usr/data/spec
		
		char	m_szSpecScriptPath[CNT_INT_FILE_LEN];			//	the default is ./lib/hmi_group/spec
																//	the file name is path + 
		
		
		char*	m_szDBDataID[CNT_INT_MAX_FREE_NUM];
		char	m_szCopyPath[CNT_INT_FILE_LEN];
		char	m_szCopyFile[CNT_INT_FILE_LEN];
		int		m_nIDNum;
		
	private:
		CtmReadData	m_ReadData;
	
};

#ifndef	D_BEOBJECT_CTMDEFAULTG
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

