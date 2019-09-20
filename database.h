/*==========================================================================+
|  Class    : Database 			                                            |
|  Task     : Database header file                          				|
|---------------------------------------------------------------------------|
|  Compile  : G++ V3.2.2 -                                                  |
|  Link     : G++ V3.2.2 -                                                  |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2005/5/19                                                    	|
|  Revision : 			                                                    |
+==========================================================================*/
#ifndef		D_DDATABASE
#define		D_DDATABASE

#include	"common.h"
#include	"files.h"
#include	"strings.h"
#include	"dbid.h"
#include	"tmpackclass.h"
#include	"sram.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		DB_SUCCESS			0x0000			//�Ʈw�w���`
#define		DB_ERR_DBNAME		0x0001			//�ƾڮw�W�ٿ��~
#define		DB_ERR_PROFILE		0x0002			//�ƾڮw�t�m�����~
#define		DB_ERR_DB			0x0004			//�ƾڮw���~
#define		DB_ERR_DBDEFFILE	0x0008			//�ƾڮwdefault�����~
#define		DB_ERR_DATAATTR		0x0010			//Database data attribute error	
#define		DB_ERR_DBDEF		0x0040			//���u�w�Ldefault

#define		DB_ERR_FATAL		(DB_ERR_DATAATTR | DB_ERR_DB)
//=======
// ��ƮwŪ�g���~�H��
//=======
#define		DB_ERR_OUTRANGE		0x0001			// �W�XID���\�S��
#define		DB_ERR_NOUSED		0x0002			// ��e�����S���Q�ϥ�

#define		DB_ERR_DATA			0x0008			// ��Ƥ��e���~
#define		DB_ERR_TYPE			0x0010			// ����������~�Ϊ̤��ǰt
#define		DB_ERR_ID			0x0020			// ���ID�����~

#define     tmMAX_STROKE          480
#define     tmMAX_ERRORLOG        100
#define     tmMAX_MODIFYLOG       100
#define		tmMAX_TEMPERLOG		  200

#define		DATA_PATH_ELECTRIC		1
#define		DATA_PATH_HYDRAULIC		2
#define		DATA_PATH_I32			4
#define		DATA_PATH_KEB			8
#define		DATA_PATH_HMI			16
#define		DATA_PATH_DSP28			32
#define		DATA_PATH_CNC			256
#define		DATA_PATH_AUSTONE		512
#define		DATA_PATH_HYDRAULIC2	2048

//=======
//		Reset DB block ID
//=======
#define		RESET_DB_ALL			0
#define		RESET_DB_DA				1
#define		RESET_DB_MOLDSET		2
#define		RESET_DB_MACHINE		3
#define		RESET_DB_OPERATION		4
#define		RESET_DB_MACHINEB		5
//=======
//		Compute Limits Macro zholy061221
//=======
#define		ULMT_TYPE_PVL							0
#define		ULMT_TYPE_POSI							1
#define		ULMT_TYPE_TORQUE						2
#define		ULMT_TYPE_TEMP							3
#define		ULMT_TYPE_COUNT							4
                                    				
#define		ULMT_STEP_ULMTC							0
#define		ULMT_STEP_ULMTV							1
#define		ULMT_STEP_COUNT							2
                                    				
#define		CONF_CHL_STARTSTRID						"CONF_CODE_READ_CHL1_MAXPOSITION"
#define		CONF_PVL_STARTSTRID						"CONF_PVL1_READ_NULL_MAXDATA"
#define		CONF_TEM_STARTSTRID						"CONF_TMP1_HEATERON_NULL_MAXDEGREE"
#define		CONF_TORQUE_STARTSTRID					NULL

#define		COMPUTE_SUCCESS							0x0000
#define		COMPUTE_ERROR							0x0001
#define		COMPUTE_NO								0x0002

#define		CHECKFILETYPE							"/usr/Data"
 
#define		DATABASE2_ATTR_LEN						28
#define		DATABASE3_ATTR_LEN						38

#define		DATABASE_VERSION_2						0x32
#define		DATABASE_VERSION_3						0x33
 

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagDBINFO
{
	int				nVersion;				// �ƾڮw����
	unsigned int	nDBLen;					// �ƾڮw����
	unsigned int	nDBDefLen;				// �ƾڰt�m�H������	
	
	char			*pszID;					// ID���W
	char			*pszName;				// �ƾڮw���W��
	char			*pszAttr;				// Data attribute file name	
	char			*pszDefault;			// �q�{��
	char			*pszMaxDef;				// �̤j�q�{��
	char			*pszMinDef;				// �̤j�q�{��
}	DBINFO;

typedef	struct tagDATATYPE
{
	unsigned short	wType;					// �������
	unsigned short	wLength;				// ��ƪ��� �p�G�O�r��,���ץ[1
}	DATATYPE;

typedef struct tagDB_DATAATTRIBUTE		
{
   DWORD	dwOffset;
   DATATYPE	DataType;
   DWORD	dwDataPath;
   DWORD	dwMaxIndex;
   DWORD	dwMinIndex;
   WORD		wDataLabel;						//������� �p�G���O�B�y�q��
   WORD		wPrecision;
   DWORD	dwPrivilege;
   DWORD	dwPowerSupply;					//=======�ʤO�ӷ� 3.0���s�W
   DWORD	dwUnit;							//=====��� 3.0���s�W
   WORD		wDisplayPrecision;				//======��ܺ�� 3.0���s�W
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	DB_DATAATTRIBUTE;
#else 
#ifdef	D_X86
DB_DATAATTRIBUTE;
#endif
#endif

typedef union	tagDBDATA
{
	char 			acData[8];
	unsigned char 	ucData;
	unsigned short	wData;
	unsigned long	dwData;
	signed char 	cData;
	signed short	nData;
	signed long		lData;
	float 			fData;
	double 			dData;
	char			*pcData;
} DBDATA;

typedef	struct	tagDBVALUE
{
	DWORD			dwIndex;			// ��ƧǸ�	
	DATATYPE 		DataType;
	DWORD			dwDataPath;
	DWORD			dwPowerSupply;		//�ʤO�ӷ�
	DWORD			dwMaxIndex;
	DWORD			dwMinIndex;
	WORD			wDataLabel;			
	WORD			wPrecision;
	WORD			wDisplayPrecision;	//��ܺ��
	DWORD			dwPrivilege;
	DWORD			dwUnit;				//���
	long			lID;
	unsigned int 	dwState;			// Ū�g���A
	union
	{
		char 			acData[8];
		unsigned char 	ucData;
		unsigned short	wData;
		unsigned long	dwData;
		signed char 	cData;
		signed short	nData;
		signed long		lData;
		float 			fData;
		double 			dData;
		char			*pcData;
	};
}	DBVALUE;

typedef struct tagDBVIEWVALUE			
{
	long long	lValue;
	long long	lMaxValue;
	long long	lMinValue;
	WORD		wPrecision;
	WORD		wDisplayPrecision;
	DWORD		dwPrivilege;
}	DBVIEWVALUE;

typedef struct tagDBCURVE
{
	int			nIndex;	
	int			nCount;
	double		dDistance;
	double		dStartX;
	DATATYPE	DataType;
	char		Reserve[4];
	DBDATA		Value[500];
} DBCURVE;

typedef	struct tagINDEX2ID
{
	int		nIndex;
	long	lDataID;
}	INDEX2ID;

typedef struct tagDBRECORD
{
	DWORD	dwComponent;
	DWORD	dwMotion;
	DWORD	dwActPoint;
	DWORD	dwEffect;
}	DBRECORD;

typedef struct tagULMTDATAID_SCOPE
{
	DWORD	dwStart;
	DWORD	dwEnd;
} ULMTDATAID_SCOPE;//zholy061221

typedef	struct	tagTMDB2
{	
	int		nDataID;
	WORD	wDataType;
	WORD	wPrecision;
	int		nOffset;
	DBDATA	data;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	TMDB2;
#else 
#ifdef	D_X86
TMDB2;
#endif
#endif
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - Wnd                                          |
+===========================================================================*/
class CDatabase: public CtmPackClass
{	
	DECLARE_DYNCREATE(CDatabase);
public:
	CDatabase();
	~CDatabase();
	
private:
	WORD				m_wState;							// �ƾڮw���A�Ч� 
	int					m_nIDCount;							// ID�ƥ�
                    	
	char*				m_pcDB;								// Database
	char*				m_pcDefDB;							// Database default value
                		
	CStrings*			m_pIDList;
	DB_DATAATTRIBUTE*	m_pDataAttr;						// Data Attribute	
	
	DBINFO				m_dbInfo;							// ��Ʈw�򥻫H��	
	DWORD				m_dwLength;							// ��Ʈw
//	CtmFile				m_DBFile;							// ��Ʈw���

	pthread_mutex_t		mutex;								// �����i�{��
	
	int					m_nMaxIndex;
	INDEX2ID*			m_pIndexToID;
	DBRECORD*			m_pDBRecord;
	long				m_lMaxDataID;
public:	
	//	fans add for test
	DB_DATAATTRIBUTE*	GetAttr() {return m_pDataAttr;};
	int					LoadRecords();
	int					LoadAttr();
	int					GetDBVersion() {return m_dbInfo.nVersion;};
	int					Create(DBINFO *pInfo);
	int					GetIndex(char *psz);
	int					GetDataID(char *psz);
	char*				GetString(int nIndex);
	int					Save(int nLength, int nOffset, int nIndex=-1);
	
	BYTE				Readb(int nIndex);
	WORD				Readw(int nIndex);
	long				Readl(int nIndex);
	char*				Reads(int nIndex);
	
	DBVALUE				Read(int nIndex);
	DBVALUE				Read(char* pID);
	DBVALUE				ReadDefault(int nIndex);
	DBVALUE				ReadDefault(char* pID);
	int					ReadBatch(void *pData,	int	nOffset, int nLength);
	
	BYTE				Writeb(BYTE Values, int nIndex, BOOL bSave = TRUE);
	WORD				Writew(WORD Values, int nIndex, BOOL bSave = TRUE);
	DWORD				Writel(DWORD Values, int nIndex, BOOL bSave = TRUE);
	char*				Writes(char *psz, int nIndex, int nLength, BOOL bSave = TRUE);
	
	DBVALUE*			Write(DBVALUE *pData, BOOL bSave = TRUE);
	DBVALUE				Write(int nIndex, void *pData, BOOL bSave = TRUE);
	DBVALUE				Write(char *pID, void *pData, BOOL bSave = TRUE);
	DBVALUE				WriteDefault(int nIndex, void *pData, BOOL bSave = TRUE);
	DBVALUE				WriteDefault(char *pID, void *pData, BOOL bSave = TRUE);
	char*				WriteDefaults(int nIndex, char *psz, int nLength, BOOL bSave = TRUE);
	char*				WriteDefaults(char *pID, char *psz, int nLength, BOOL bSave = TRUE);
	int					WriteBatch(void *pData,	int	nOffset, int nLength, BOOL bSave = FALSE);		//for CNC	//Mario debug
	int					DataIDGetOffset(int nDataID);		//Mario debug
	DATATYPE			DataIDGetDataType(int	nDataID);		//Mario debug
	int					IndexGetOffset(int	nIndex);		//Mario debug
	DATATYPE			IndexGetDataType(int	nIndex);		//Mario debug
	int					DataIDToIndex(long lDataID);
	long				IndexToDataID(int nIndex);
	int					GetIDCount() { return m_nIDCount; };
	DWORD				GetDBSize()	{ return m_dwLength; };
	DBRECORD			GetDBRecord(int	nIndex);
	void				Reset();
	int					LoadDB();		//Mario debug
	int					m_nDBState;					//�P�ʆV�ƃ��]��0=���` 1=�V�Ƥ����Q���m
public:
	static		int		m_nDAChange;
	static		int		m_nMachineChange;
	static		int		m_nSystemxChange;
	static		int		m_nDAState;
	static		int		m_nMachineState;
	static		int		m_nSystemxState;
	char*			 	m_pszSystemxName;
	char*				m_pszUserName;
	char*			 	m_pszSystemxValue;
	char*			 	m_pszUserValue;
	char*				m_pszSystemDBID[256];
private:
	//int					LoadDB();		Mario debug
	void				CheckBackupDBInfo();
	void				BackupDBInfo();
	void				UpdateBackupDBInfo();
	class				CtmPrivateData;
	CtmPrivateData*		m_data;
public:
	virtual		double	GetLimitValue(int nIndex, int nType)			{return 0;}
	virtual		WORD	ComputeLimitValue(char* pcID)					{return 0;}
	virtual		WORD	ComputeLimitValue(WORD wRegion, char* pcID)		{return 0;}

};

extern	CDatabase*		g_pDatabase;

void					Reset_DB(WORD	wType);
DBVIEWVALUE				GetDBValue(WORD	wIndex);
DBVIEWVALUE				GetDBValue(char* pcID, BOOL* pbFind = NULL);				
DBVIEWVALUE				GetIDValue(DWORD dwDataID);						
DBVIEWVALUE				GetDBDefaultValue(WORD wIndex);
DBVIEWVALUE				GetDBDefaultValue(char* pcID);

WORD					GetDBString(char* pcID, char* pszStr, int nLength);
WORD					SetDBString(char* pcID, char* pszStr, int nLength);

WORD					SetDBValue(char* pcID, long lValue, BOOL bSend=TRUE);
WORD					SetDBValue(WORD	wIndex, long lValue, BOOL bSend=TRUE);
WORD					SetViewDBValue(char* pcID, long lValue);
WORD					SetIDValue(DWORD dwDataID, long lValue);		
double					GetValue(DBVALUE dbValue);	
//double					GetLimitValue(int nIndex, int nType);
//WORD					ComputeLimitValue(char* pcID);
//WORD					ComputeLimitValue(WORD wRegion, char* pcID);
int						CheckBlockData(CtmRecordFileEx* ptmRecordFile, int	nDataTye, int nBlockID, tmStorageType StorageType, BOOL bRead = FALSE);
int						CheckFreeBlockData(char* pszIDTableFile, char* pszValueFile, BOOL bRead = FALSE);
int						SearchIndexFromFreeData(char* pszIDTableFile, char* pszValueFile, int nIndex);
int						GetFileType(char* pszPath);

WORD					SetDBValueM(char* pcID, long lValue, BOOL bSend = TRUE);
WORD					SetDBValueM(WORD wIndex, long lValue, BOOL bSend = TRUE);
WORD					SetIDValueM(DWORD dwDataID, long lValue);
DBVIEWVALUE				GetDBValueM(WORD	wIndex);
DBVIEWVALUE				GetDBValueM(char* pcID, BOOL* pbFind = NULL);				
DBVIEWVALUE				GetIDValueM(DWORD dwDataID);	

//New DB2 Sram
int						SetDB2Info(TMDB2*	tmdb2, int nCount);
int						GetDB2Size(TMDB2*	tmdb2, int nCount);
DBVIEWVALUE				GetDB2Value(int nDataID, BOOL* pbFind = NULL);
int						SetDB2Value(int	nDataID, long lValue,BOOL bSave = TRUE);
int						GetDefData(TMDB2*	tmdb2, int nCount, char* pszDefDB);

#endif
