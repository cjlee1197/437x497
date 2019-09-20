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
#include	"database.h"
#include	"profile.h"
#include	"utils.h"
#include	"main.h"
#include	"recordset.h"	//	fans	2006/12/15 12:43¤W¤È
#include	"commonaction.h"//	fans	2006/12/15 12:56¤W¤È
#include	"moldControl.h"	//	fans	2007/1/12 10:35¤W¤È
#include	"tmdebug.h"
#include	"machinefactor.h"
#include	"freedata.h"
#include	"readdata.h"
#include	"tmshmsg.h"
#include	"tmconfig.h"
#include	"./lib/inet/tmTaskiNet.h"
#include	<iostream>

using namespace std;


typedef struct tmtagBackupDBInfo
{
	DWORD 	  dwID;
	DWORD 	  dwOffset;
	DATATYPE  DataType;	
}BPINFO;




IMPLEMENT_DYNCREATE(CDatabase, CtmPackClass)
/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/
CDatabase*	g_pDatabase;
pthread_mutex_t			g_DataWriteMutex;								// ®ø®§¶iµ{Âê
pthread_mutex_t			g_DataStrWriteMutex;							// ®ø®§¶iµ{Âê
pthread_mutex_t			g_DataReadMutex;								// ®ø®§¶iµ{
int			g_nDB2Count = 0;
TMDB2*		g_ptmdb2	= NULL;
char * 		g_pcDB2 = NULL;


int	CDatabase::m_nDAChange 			= 0;
int	CDatabase::m_nMachineChange		= 0;
int	CDatabase::m_nSystemxChange	= 0;
int	CDatabase::m_nDAState 			= 0;
int	CDatabase::m_nMachineState 		= 0;
int	CDatabase::m_nSystemxState 	= 0;
/*==========================================================================+
|           Class implementation - CtmDatabase                              |
+==========================================================================*/
/*--------------------------------------------------------------------------+
|           Constructor and destructor                                      |
+--------------------------------------------------------------------------*/
class CDatabase::CtmPrivateData{
	public:
		CtmPrivateData():m_last_db(0),m_last_defdb(0)
		{
			;
		}
		~CtmPrivateData()
		{
			if(m_last_db)
			{
				delete m_last_db;
				m_last_db=0;	
			}
			if(m_last_defdb)
			{
				delete m_last_defdb;
				m_last_defdb=0;	
			}	
		}
	public:
		
		/*
		*  key:dwID  value: BackupDBInfo
		*/
		char*			  m_last_db;
		char*			  m_last_defdb;
};



CDatabase::CDatabase()
{
	m_nIDCount	= 0;
	m_pDataAttr	= NULL;
	m_pcDB		= NULL;
	m_pcDefDB	= NULL;
	m_pIDList	= NULL;
	
	m_pszSystemxName	= NULL;
	m_pszUserName		= NULL;
	m_pszSystemxValue	= NULL;
	m_pszUserValue		= NULL;
	//m_pszSystemDBID[0]	= NULL;
	memset(m_pszSystemDBID, 0, sizeof(m_pszSystemDBID));
	m_nDBState 	= 0;
	m_dbInfo.pszID 		= new char[256];
	m_dbInfo.pszName 	= new char[256];
	m_dbInfo.pszAttr 	= new char[256];
	m_dbInfo.pszDefault = new char[256];
	m_dbInfo.pszMaxDef 	= new char[256];
	m_dbInfo.pszMinDef 	= new char[256];
	m_data= new CtmPrivateData();
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&g_DataWriteMutex, NULL);
	pthread_mutex_init(&g_DataStrWriteMutex, NULL);
	pthread_mutex_init(&g_DataReadMutex, NULL);
	
	
	
	
	
}

CDatabase::~CDatabase()
{
//	m_DBFile.Seek(0, SEEK_SET);
//	m_DBFile.Write(m_pcDB, m_dwLength);
//	m_DBFile.Close();
	
	m_wState = DB_ERR_FATAL;
	if(m_data)		delete m_data;
	if (m_pcDB 		!= NULL) 	free(m_pcDB);
	if (m_pcDefDB	!= NULL)	free(m_pcDefDB);
	if (m_pDataAttr != NULL)	free(m_pDataAttr);	
	if (m_pDBRecord != NULL)	free(m_pDBRecord);	
	if (m_pIDList	!= NULL)			delete m_pIDList;
	if(m_dbInfo.pszID != NULL )  		delete [] m_dbInfo.pszID;
	if(m_dbInfo.pszName != NULL )  		delete [] m_dbInfo.pszName;
	if(m_dbInfo.pszAttr != NULL )  		delete [] m_dbInfo.pszAttr;
	if(m_dbInfo.pszDefault != NULL )  	delete [] m_dbInfo.pszDefault;
	if(m_dbInfo.pszMaxDef != NULL )  	delete [] m_dbInfo.pszMaxDef;
	if(m_dbInfo.pszMinDef != NULL )  	delete [] m_dbInfo.pszMinDef;
	if(m_pszSystemxName != NULL)		delete	m_pszSystemxName;
	if(m_pszUserName != NULL)			delete	m_pszUserName;
	if(m_pszSystemxValue != NULL)		delete	m_pszSystemxValue;
	if(m_pszUserValue != NULL)			delete	m_pszUserValue;
	
	for(int i = 0; i < 256; i++)
	{
		if(m_pszSystemDBID[i] != NULL)
			delete []m_pszSystemDBID[i];
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&g_DataWriteMutex);
	pthread_mutex_destroy(&g_DataStrWriteMutex);
	pthread_mutex_destroy(&g_DataReadMutex);
	
	m_dbInfo.pszID 		= NULL;
	m_dbInfo.pszName 	= NULL;
	m_dbInfo.pszAttr 	= NULL;
	m_dbInfo.pszDefault = NULL;
	m_dbInfo.pszMaxDef 	= NULL;
	m_dbInfo.pszMinDef	= NULL;
	
	m_pDataAttr	= NULL;									
	m_pcDB		= NULL;
	m_pcDefDB	= NULL;
	m_pIDList	= NULL;
	if(m_pIndexToID	!= NULL)	free(m_pIndexToID);
		
	if(g_pcDB2 != NULL)
		delete [] g_pcDB2;
}

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|	function	: Create(char *pszDBName)									|
|	remark		: ªì©l¤Æ¸ê®Æ®w												|
|---------------------------------------------------------------------------|
|	call		: Create(pszDBName)											|
|	para		: pszDBName		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	0			: ªì©l¤Æ¦¨¥\								|
|				:	¨ä¥¦		: ¿ù»~¥N½X									|
+--------------------------------------------------------------------------*/		
int		CDatabase::Create(DBINFO *pInfo)
{
	memcpy(m_dbInfo.pszID, 		pInfo->pszID, 256);
	memcpy(m_dbInfo.pszName, 	pInfo->pszName, 256);
	memcpy(m_dbInfo.pszAttr, 	pInfo->pszAttr, 256);
	memcpy(m_dbInfo.pszDefault, pInfo->pszDefault, 256);
	m_dbInfo.nVersion	= pInfo->nVersion;
	//m_dbInfo	= *pInfo;
	
	//=======
	// ¦pªG­ì¥ý¤w¸g¤À°t¹L¡A«hÄÀ©ñ
	//=======
	
	if(m_pDataAttr 	!= NULL)	free(m_pDataAttr);		
	if(m_pcDB 		!= NULL) 	free(m_pcDB);
	if(m_pcDefDB	!= NULL)	free(m_pcDefDB);
	if(m_pIDList	!= NULL)	delete m_pIDList;
	
	m_pDataAttr				= NULL;
	m_pcDB					= NULL;
	m_pIDList				= NULL;
	m_pcDefDB				= NULL;
	m_wState				= DB_SUCCESS;
	m_nIDCount				= 0;
	m_pIndexToID			= NULL;
	m_nMaxIndex				= 
	m_lMaxDataID			= 0;
	
	m_wState	= LoadDB();
	
	return m_wState;
}

int		CDatabase::Save(int nLength, int nOffset, int nIndex)
{
	//if(m_pcDB == NULL)	return DB_ERR_DB;
	//m_DBFile.Seek(0, SEEK_SET);
	//m_DBFile.Write(m_pcDB, m_dwLength);     
	//return 0;
	CtmSarmRecordFileControl*	pSram;
	DWORD dwIndex = 0, dwDataID = 0;
	BOOL	bFlag = TRUE;
	long	lBlockOffset = -1;
	if (m_pcDB == NULL)
		return DB_ERR_DB;
	
	//¦pªG¬O·sªº¦s‰É…CÌÛ¡Aƒh†¿ƒº¦s‰É
	//if(GetFileType(CHECKFILETYPE))
	//{
	//	//James add Éó¾¹†V®Æ©MDA†V®Æ³£¦s¨ìFlashEX¤¤¥h 2008/10/28 1:24PM
	//	dwDataID = g_pDatabase->IndexToDataID(nIndex);
	//	if(nIndex >=0 && (dwDataID < 0x400 || (dwDataID > 0x8FF && dwDataID < 0x1100) || dwDataID > 0x13FF) )
	//	{
	//		//‰ä„ò†V®Æ¬O§_Œp¤_MACHINEªºBlock 2008/10/28 1:25PM
	//		if(m_nDAChange <= 0 && bFlag)
	//		{
	//			if((lBlockOffset = g_pBlock->SearchIndexFromBlock(BLOCK_NEWPOWERMATCH_ID, nIndex)) >=0)
	//			{
	//				bFlag = FALSE;
	//				m_nDAChange++;
	//			}
	//			//printf("nIndex=%d dwIndex=%d m_nDAChange=%d lBlockOffset=%d\n",nIndex,dwIndex, m_nSystemxChange, lBlockOffset);
	//		}
	//		if(m_nMachineChange <= 0 && bFlag)
	//		{
	//			if((lBlockOffset = g_pBlock->SearchIndexFromBlock(BLOCK_MACHINE_ID, nIndex)) >=0)
	//			{
	//				//¶}¼ÒÁ`¼Æ¤£¦s
	//				if(0 != strcmp("MACH_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT", GetString(nIndex)))
	//				{
	//					m_nMachineChange++;
	//				}
	//				bFlag = FALSE;
	//			}
	//			else if((lBlockOffset = g_pBlock->SearchIndexFromBlock(BLOCK_MACHINEB_ID, nIndex)) >=0)
	//			{
	//				m_nMachineChange++;
	//				bFlag = FALSE;
	//			}
	//			//printf("nIndex=%d dwIndex=%d m_nMachineChange=%d lBlockOffset=%d\n",nIndex, dwIndex, m_nMachineChange, lBlockOffset);
	//		}
	//		if(m_nSystemxChange <= 0 && bFlag)
	//		{
	//			if((lBlockOffset = SearchIndexFromFreeData(m_pszSystemxName, m_pszSystemxValue, nIndex)) > 0)
	//			{
	//				bFlag = FALSE;
	//				m_nSystemxChange++;
	//			}
	//			//printf("nIndex=%d dwIndex=%d m_nSystemxChange=%d lBlockOffset=%d\n",nIndex, dwIndex, m_nSystemxChange, lBlockOffset);
	//		}
	//	}
	//}
	
	pSram->GetInstance()->WriteRecord(ID_DB_SRAMFILE, 1, m_pcDB + nOffset, nLength, nOffset);

	return 0;
}

int		CDatabase::LoadRecords()
{
	CtmFile File;
	char	szTemp1[512];	//Sunny<20070622>
    sprintf(szTemp1, "%s", CtmConfig::GetInstance()->GetDBRecords());		
	if(File.Open(szTemp1/*"res_tm800/db/records"*/) != tmFILE_GOOD)	
		return DB_ERR_DATAATTR;
	int nFileLength	= File.Seek(0, SEEK_END);
	if (m_dbInfo.nVersion == DATABASE_VERSION_2 && m_nIDCount != nFileLength/(int)sizeof(DBRECORD))
		return DB_ERR_DATAATTR;
	File.Seek(0, SEEK_SET);
	m_pDBRecord	= (DBRECORD *)malloc(nFileLength);
	if(m_pDBRecord == NULL)
	{
		File.Close();
		return DB_ERR_DB;
	}
	File.Read(m_pDBRecord, nFileLength);
	File.Close();
	if(m_dbInfo.nVersion == DATABASE_VERSION_2)
	{
		return DB_SUCCESS;
	}
	else if(m_dbInfo.nVersion == DATABASE_VERSION_3)
	{
		DBRECORD *temp = m_pDBRecord;
		m_pDBRecord = (DBRECORD *)malloc(m_nIDCount*sizeof(DBRECORD));
		if(m_pDBRecord == NULL)
			return DB_ERR_DB;
		char *p = (char *)temp;
		char *q = p;
		int i  = 0;
		while(i < m_nIDCount)
		{
			while(*p++);
			m_pDBRecord[i].dwComponent 		= g_MultiLanguage.GetStrKeyIndex(q);
			q = p;
			while(*p++);
			m_pDBRecord[i].dwMotion 		= g_MultiLanguage.GetStrKeyIndex(q);;
			q = p;
			while(*p++);
			m_pDBRecord[i].dwActPoint 		= g_MultiLanguage.GetStrKeyIndex(q);;
			q = p;
			while(*p++);
			m_pDBRecord[i].dwEffect 		= g_MultiLanguage.GetStrKeyIndex(q);;
			q = p;
			i++;
		}
		free(temp);
		return DB_SUCCESS;
	}
	else
	{
		return DB_ERR_DB;
	}
}

int		CDatabase::LoadAttr()
{
	CtmFile File;
	if (File.Open(m_dbInfo.pszAttr) != tmFILE_GOOD) 
		return DB_ERR_DATAATTR;
	int nFileLength = File.Seek(0, SEEK_END) - sizeof(m_dwLength);
	if ((m_dbInfo.nVersion == DATABASE_VERSION_2 && m_nIDCount != nFileLength/DATABASE2_ATTR_LEN) ||
		(m_dbInfo.nVersion == DATABASE_VERSION_3 && m_nIDCount != nFileLength/DATABASE3_ATTR_LEN))
		return DB_ERR_DATAATTR;
	m_pDataAttr = (DB_DATAATTRIBUTE *)malloc(nFileLength);
	if(m_pDataAttr == NULL)
		return DB_ERR_DB;
	File.Seek(0, SEEK_SET);
	File.Read(&m_dwLength, sizeof(m_dwLength));
	File.Read(m_pDataAttr, nFileLength);
	File.Close();
	if (m_dbInfo.nVersion == DATABASE_VERSION_3)
	{
		return DB_SUCCESS;
	}
	else if(m_dbInfo.nVersion == DATABASE_VERSION_2)
	{
		char *temp = (char *)m_pDataAttr;
		m_pDataAttr = (DB_DATAATTRIBUTE *)malloc(m_nIDCount*DATABASE3_ATTR_LEN);
		memset(m_pDataAttr, 0, m_nIDCount*DATABASE3_ATTR_LEN);
		if(m_pDataAttr == NULL)
			return DB_ERR_DB;
		for(int i=0; i<m_nIDCount; i++)
		{
			memcpy((char *)m_pDataAttr+i*DATABASE3_ATTR_LEN, temp+i*DATABASE2_ATTR_LEN, DATABASE2_ATTR_LEN);
		}
		free(temp);
		return DB_SUCCESS;
	}
	else
	{
		return DB_ERR_DB;
	}
}

/*--------------------------------------------------------------------------+
|	function	: LoadDB()													|
|	remark		: ®ÚÕu°t¸m¤å¥óªº«H®§¡A§â¸ê®Æ±q¤å¥óload¶i¤º¦s				|
|---------------------------------------------------------------------------|
|	call		: 															|
|	parameter	: 				: 											|
|	return 		:	0			: ¦¨¥\										|
|				:	¨ä¥¦		: ¿ù»~										|
+--------------------------------------------------------------------------*/
int		CDatabase::LoadDB()
{
	int		wState	= 0;
	int		i, j;
	int		nFileLength;
	char	*pc, *pID, *pszID;
	CtmFile	File;
	STRINGSINFO	Info;
	struct	stat Stat;
	
	//=======
	// §PÂ_¸ê®Æ°t¸m¬O§_¥¿ÚÌ
	//=======
	if(m_dbInfo.pszID == NULL)		return DB_ERR_ID;
	if(m_dbInfo.pszAttr == NULL)	return DB_ERR_DATAATTR;
		
	//=======
	// Åª¨úID¤å¥ó
	//=======
	if(File.Open(m_dbInfo.pszID) != tmFILE_GOOD)return DB_ERR_ID;
	nFileLength	= File.Seek(0, SEEK_END);
	pszID		= (char *)malloc(nFileLength);
	if(pszID == NULL)							return DB_ERR_ID;
	File.Seek(0, SEEK_SET);
	File.Read(pszID, nFileLength);	
	File.Close();
	//=======
	// ­pºâ¦³®Ä¦r¦ê¼Æ¥Ø
	//=======
	pc	= pszID;
	j	= 0;
	for(i = 0; i < nFileLength; i ++)
	{
		if(*pc != 0)	j ++;
		else
		{
			if(j != 0)
			{	
				m_nIDCount	++;
			}
			j	= 0;
		}	
		pc ++;
	}
	
	Info.nCount	= m_nIDCount;
	Info.nSize	= nFileLength;
	m_pIDList	= new CStrings;
	m_pIDList->Create(&Info);
	
	pID	=
	pc	= pszID;
	j	= 0;
	for(i = 0; i < nFileLength; i ++)
	{
		if(*pc != 0)	j ++;
		else
		{
			if(j != 0)
			{	
				m_pIDList->AddString(pID);
			}
			j	= 0;
			pID	= pc + 1;
		}	
		pc ++;
	}
	free(pszID);

	//=======
	//		load database data attribute file	
	//=======
	if((wState = LoadAttr()) != DB_SUCCESS)
		return wState;
		
	//printf("DB_DATAATTRIBUTE=%d DataCount=%d FileLength=%d\n", sizeof(DB_DATAATTRIBUTE), nFileLength/(int)sizeof(DB_DATAATTRIBUTE), nFileLength);
	//=======
	// Åª¨ú¸ê®Æ¤å¥ó
	//=======
	
	if(stat(m_dbInfo.pszDefault, &Stat) == tmFILE_GOOD)
	{
		m_pcDefDB	= (char *)calloc(m_dwLength, 1);
		File.Open(m_dbInfo.pszDefault);
		nFileLength	= File.Seek(0, SEEK_END);
		if((DWORD)nFileLength == m_dwLength)
		{
			File.Seek(0, SEEK_SET);
			File.Read(m_pcDefDB, m_dwLength);				
		}
		else
			return	DB_ERR_DBDEF;
		File.Close();
	}
	else
		return	DB_ERR_DBDEF;

	m_pcDB		= (char *)calloc(m_dwLength, 1);
	
	
	//	fans mark	2007/1/9 08:29¤U¤È
	/*
	if(stat(m_dbInfo.pszName, &Stat) != tmFILE_GOOD)
	{	// ¦pªG¸ê®Æ®w¤å¥ó¤£¦s¦b
		
		if(m_DBFile.Open(m_dbInfo.pszName, O_RDWR | O_CREAT) != tmFILE_GOOD)
			return DB_ERR_DB;
		memcpy(m_pcDB, m_pcDefDB, m_dwLength);
		m_DBFile.Seek(0, SEEK_SET);
		m_DBFile.Write(m_pcDB, m_dwLength);
		File.Close();
	}
	else
	{
		m_DBFile.Open(m_dbInfo.pszName);
		nFileLength	= m_DBFile.Seek(0, SEEK_END);
		if ((DWORD)nFileLength != m_dwLength)
			m_DBFile.Truncate(m_dwLength);
		m_DBFile.Seek(0, SEEK_SET);
		m_DBFile.Read(m_pcDB, m_dwLength);
	}
	*/
	
	//=======
	// ³Ð«Ø¦èªù¤l©Mµ{§ÇªºIDÂà´«ªí
	//=======	
	char	szTemp0[512];	//Sunny<20070622><<<<
    sprintf(szTemp0, "%s", CtmConfig::GetInstance()->GetDBIndex2id());	
	if(File.Open(szTemp0/*"res_tm800/db/index2id"*/) != tmFILE_GOOD);
	//Sunny<20070622>>>>>
	nFileLength	= File.Seek(0, SEEK_END);
	if (m_nIDCount != nFileLength/(int)sizeof(INDEX2ID))
		return DB_ERR_DATAATTR;
	File.Seek(0, SEEK_SET);
	m_pIndexToID	= (INDEX2ID *)malloc(nFileLength);
	File.Read(m_pIndexToID, nFileLength);
	File.Close();
	m_nMaxIndex		= m_pIndexToID[m_nIDCount - 1].nIndex;
	m_lMaxDataID	= m_pIndexToID[m_nIDCount - 1].lDataID;
	
	//=======
	//	Load record str file
	//=======	
	if((wState = LoadRecords()) != DB_SUCCESS)
		return wState;
		
	if(m_pcDB == NULL || m_pDataAttr == NULL)
		return DB_ERR_FATAL;
	
	//=======
	//†Á†V®Æƒ·‰ä„ò…n¦æ¬Û‹×¾Þ§@
	//=======	
	CtmSarmRecordFileControl*	pSram;
	CtmStorageFile* 	u_pSramFile = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_SRAM);
	CtmRecordFileEx*	pFlashEXFile = NULL;
	if(GetFileType(CHECKFILETYPE))
	{
		pFlashEXFile = CtmMachineBuild::GetInstance()->BuildCStorageRecordFile(STORAGE_TYPE_FLASH);
	}
	int	nDBCount = 0, nDBSize = 0;
	//ŒÝ¨ú°t¸m¤å¥óSystem©MUser¸ô’¦¦W†ï
	CIniFile	 m_FreeDataIni;
	m_FreeDataIni.OpenFile("outerconfig.ini");
	m_FreeDataIni.ReadString("FreeData", "Systemx", NULL, &m_pszSystemxName);
	m_FreeDataIni.ReadString("FreeData", "User", NULL, &m_pszUserName);
	m_FreeDataIni.ReadString("FreeData", "Systemx_Value", NULL, &m_pszSystemxValue);
	m_FreeDataIni.ReadString("FreeData", "User_Value", NULL, &m_pszUserValue);
	m_FreeDataIni.CloseFile();
	if (CtmConfig::GetInstance()->GetSramInit() <= 0 || 
		(!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DB_SRAMFILE)))
	{
		memcpy(m_pcDB, m_pcDefDB, m_dwLength);
		
		pSram->GetInstance()->WriteRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
		//‰ä¬dMachine,DA,StopMachine†V®Æ¬O§_A¥¢©Î‰ä„ò®ÕJ±K‡ù¤£†Á¡A¦p¥¿ÚÌ¡A’âFlash¤¤ªº†V®Æ«þƒP¨ìsram¡A§_ƒh„Gsram¦s¨ìFlash
		CheckBlockData(pFlashEXFile, ID_DATABLE_FILE, BLOCK_NEWPOWERMATCH_ID, STORAGE_TYPE_FLASH, TRUE);
		CheckBlockData(pFlashEXFile, ID_MACHINE_FILE, BLOCK_MACHINE_ID, STORAGE_TYPE_FLASH, TRUE);
		CheckBlockData(pFlashEXFile, ID_MACHINEB_FILE, BLOCK_MACHINEB_ID, STORAGE_TYPE_FLASH, TRUE);	//James add 2009/2/16 07:23¤U¤È
		if(pFlashEXFile != NULL)
		{
			CheckFreeBlockData(m_pszSystemxName,m_pszSystemxValue,TRUE);
			CheckFreeBlockData(m_pszUserName,m_pszUserValue, TRUE);
		}
		
		g_tmDebugInfo->PrintDebugInfo("m_nIDCount=%d m_dwLength=%d \n", m_nIDCount, m_dwLength);
		u_pSramFile->Write(SRAM_START_NUM, sizeof(m_nIDCount), (BYTE*)&m_nIDCount);
		u_pSramFile->Write(SRAM_START_NUM+sizeof(m_nIDCount), sizeof(m_dwLength), (BYTE*)&m_dwLength);
		m_nDBState = 1;		
		BackupDBInfo();
	}
	else 
	{
		u_pSramFile->Read(SRAM_START_NUM, sizeof(nDBCount), (BYTE*)&nDBCount);				//¥H«e†V®Æªº…@‡Û
		u_pSramFile->Read(SRAM_START_NUM+sizeof(nDBCount), sizeof(nDBSize), (BYTE*)&nDBSize);	//¥H«e†V®Æªº¤j¤p
		
		//printf("nDSize=%d,nDBCount=%d\n",nDBSize,nDBCount);
		nDBSize=(nDBSize>m_dwLength)?nDBSize:m_dwLength;
		m_data->m_last_db= new char[nDBSize];
		pSram->GetInstance()->ReadRecord(ID_DB_SRAMFILE, 1, (void*)m_data->m_last_db);
		memcpy(m_pcDB, m_pcDefDB, m_dwLength);
		CheckBackupDBInfo();
		pSram->GetInstance()->WriteRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
			
		CheckBlockData(pFlashEXFile, ID_DATABLE_FILE, BLOCK_NEWPOWERMATCH_ID, STORAGE_TYPE_FLASH, FALSE);
		CheckBlockData(pFlashEXFile, ID_MACHINE_FILE, BLOCK_MACHINE_ID, STORAGE_TYPE_FLASH, FALSE);
		CheckBlockData(pFlashEXFile, ID_MACHINEB_FILE, BLOCK_MACHINEB_ID, STORAGE_TYPE_FLASH, FALSE);
		if(pFlashEXFile != NULL)
		{
			CheckFreeBlockData(m_pszSystemxName,m_pszSystemxValue,FALSE);
			CheckFreeBlockData(m_pszUserName,m_pszUserValue, FALSE);
		}
		u_pSramFile->Write(SRAM_START_NUM, sizeof(m_nIDCount), (BYTE*)&m_nIDCount);
		u_pSramFile->Write(SRAM_START_NUM+sizeof(m_nIDCount), sizeof(m_dwLength), (BYTE*)&m_dwLength);
		UpdateBackupDBInfo();

		//¦pªGsram†V®Æ¤p¤_·sªº†V®Æ¡A’â·sªº†V®Æ³Ì¦Z¦h¤_ªº«þƒP¨ìsram¤¤
//		if(m_nIDCount > nDBCount && m_dwLength > nDBSize)	
//		{
//			g_tmDebugInfo->PrintDebugInfo("======Database new m_nIDCount=%d nDBCount=%d m_dwLength=%d nDBSize=%d\n", 
//			m_nIDCount, nDBCount, m_dwLength, nDBSize);
//			//printf("======Database new m_nIDCount=%d nDBCount=%d m_dwLength=%d nDBSize=%d\n", 
//			//m_nIDCount, nDBCount, m_dwLength, nDBSize);
//			pSram->GetInstance()->ReadRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
//			memcpy(m_pcDB+nDBSize, m_pcDefDB+nDBSize, m_dwLength-nDBSize);
//			pSram->GetInstance()->WriteRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
//			
//			CheckBlockData(pFlashEXFile, ID_DATABLE_FILE, BLOCK_NEWPOWERMATCH_ID, STORAGE_TYPE_FLASH, FALSE);
//			CheckBlockData(pFlashEXFile, ID_MACHINE_FILE, BLOCK_MACHINE_ID, STORAGE_TYPE_FLASH, FALSE);
//			CheckBlockData(pFlashEXFile, ID_MACHINEB_FILE, BLOCK_MACHINEB_ID, STORAGE_TYPE_FLASH, FALSE);
//			if(pFlashEXFile != NULL)
//			{
//				CheckFreeBlockData(m_pszSystemxName,m_pszSystemxValue,FALSE);
//				CheckFreeBlockData(m_pszUserName,m_pszUserValue, FALSE);
//			}
//			
//			u_pSramFile->Write(SRAM_START_NUM, sizeof(m_nIDCount), (BYTE*)&m_nIDCount);
//			u_pSramFile->Write(SRAM_START_NUM+sizeof(m_nIDCount), sizeof(m_dwLength), (BYTE*)&m_dwLength);
//		}
//		else if(m_nIDCount == nDBCount && m_dwLength == nDBSize)
//		{
//			//†V®Æƒ·¥¿ÚÌ
//			g_tmDebugInfo->PrintDebugInfo("======Database OK m_nIDCount=%d nDBCount=%d m_dwLength=%d nDBSize=%d\n", 
//			m_nIDCount, nDBCount, m_dwLength, nDBSize);
//			pSram->GetInstance()->ReadRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
//			CtmMoldSet::GetInstance()->GetCurrentMold();
//			CheckBlockData(pFlashEXFile, ID_DATABLE_FILE, BLOCK_NEWPOWERMATCH_ID, STORAGE_TYPE_FLASH, FALSE);
//			CheckBlockData(pFlashEXFile, ID_MACHINE_FILE, BLOCK_MACHINE_ID, STORAGE_TYPE_FLASH, FALSE);
//			CheckBlockData(pFlashEXFile, ID_MACHINEB_FILE, BLOCK_MACHINEB_ID, STORAGE_TYPE_FLASH, FALSE);
//			if(pFlashEXFile != NULL)
//			{
//				CheckFreeBlockData(m_pszSystemxName,m_pszSystemxValue,FALSE);
//				CheckFreeBlockData(m_pszUserName,m_pszUserValue, FALSE);
//			}
//		}
//		else//†V®Æƒ·¤£¤Ç°t¡A¦³‡R,¦ÛƒðŠx­ìƒoÀq‡P­È
//		{
//			g_tmDebugInfo->PrintDebugInfo("======Database Error m_nIDCount=%d nDBCount=%d m_dwLength=%d nDBSize=%d\n", 
//			m_nIDCount, nDBCount, m_dwLength, nDBSize);
//			//memcpy(m_pcDB, m_pcDefDB, m_dwLength);
//			
//			CheckBlockData(pFlashEXFile, ID_DATABLE_FILE, BLOCK_NEWPOWERMATCH_ID, STORAGE_TYPE_FLASH, TRUE);
//			CheckBlockData(pFlashEXFile, ID_MACHINE_FILE, BLOCK_MACHINE_ID, STORAGE_TYPE_FLASH, TRUE);
//			CheckBlockData(pFlashEXFile, ID_MACHINEB_FILE, BLOCK_MACHINEB_ID, STORAGE_TYPE_FLASH, TRUE);
//			if(pFlashEXFile != NULL)
//			{
//				CheckFreeBlockData(m_pszSystemxName,m_pszSystemxValue,TRUE);
//				CheckFreeBlockData(m_pszUserName,m_pszUserValue,TRUE);
//			}
//		
//			pSram->GetInstance()->WriteRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
//			u_pSramFile->Write(SRAM_START_NUM, sizeof(m_nIDCount), (BYTE*)&m_nIDCount);
//			u_pSramFile->Write(SRAM_START_NUM+sizeof(m_nIDCount), sizeof(m_dwLength), (BYTE*)&m_dwLength);
//			m_nDBState = 1;
//		}
		//pSram->GetInstance()->ReadRecord(ID_DB_SRAMFILE, 1, (void*)m_pcDB);
		//CtmMoldSet::GetInstance()->GetCurrentMold();
	}
	return wState;
}

/*--------------------------------------------------------------------------+
|	function	: Read(char *pID)											|
|	remark		: ±q¸ê®Æ®wÅª¨úpID«ü©wªº¸ê®Æ,								|
|---------------------------------------------------------------------------|
|	call		: Readb(pID)												|
|	para		: pID			- ¸ê®ÆªºID¦r¦ê«ü°w							|
|																			|
|	return 		:				- ¸ê®Æ¤º®e									|
+--------------------------------------------------------------------------*/
DBVALUE		CDatabase::Read(char *pID)
{
	DBVALUE			data;
	char 			cszStrID[256];
	int				nIndex	= 0;
	
	memset(cszStrID, 0, sizeof(cszStrID));
	if(pID != NULL )
	{
		Trim(cszStrID, pID, sizeof(cszStrID));
		nIndex	= m_pIDList->GetIndex(cszStrID);
		memset(&data, 0, sizeof(DBVALUE));
		if (nIndex < 0)		return data;
		
		data = Read(nIndex);
	}
	return data;
}

DBVALUE		CDatabase::Read(int nIndex)
{
	unsigned int	dwOffset;
	DBVALUE			data;

	memset(&data, 0, sizeof(DBVALUE));
	if(m_wState & DB_ERR_FATAL)		return data;
	
	data.dwIndex	= nIndex;
	data.dwState	= DB_SUCCESS;
	if(nIndex > m_nIDCount || nIndex < 0)
	{
		data.dwState	= DB_ERR_OUTRANGE;
		return data;
	}
	dwOffset	= m_pDataAttr[nIndex].dwOffset;

	if(dwOffset == 0xFFFFFFFF)
	{
		data.dwState	= DB_ERR_NOUSED;
		return data;
	}
	
	//data.DataType			= m_pDataAttr[nIndex].DataType;
	memcpy(&data.DataType, &m_pDataAttr[nIndex].DataType, sizeof(DATATYPE));
	data.dwDataPath			= m_pDataAttr[nIndex].dwDataPath;
	data.dwPowerSupply		= m_pDataAttr[nIndex].dwPowerSupply;
	data.dwMaxIndex			= m_pDataAttr[nIndex].dwMaxIndex;
	data.dwMinIndex			= m_pDataAttr[nIndex].dwMinIndex;
	data.wDataLabel			= m_pDataAttr[nIndex].wDataLabel;
	data.wPrecision			= m_pDataAttr[nIndex].wPrecision;
	data.wDisplayPrecision	= m_pDataAttr[nIndex].wDisplayPrecision;
	data.dwPrivilege		= m_pDataAttr[nIndex].dwPrivilege;
	data.dwUnit				= m_pDataAttr[nIndex].dwUnit;
	data.lID				= IndexToDataID(nIndex);	//2007/1/5 01:24¤U¤È	
	switch(data.DataType.wType)
	{
		case TYPE_BOOL:
			data.wData	= *(BOOL *)(m_pcDB + dwOffset);
			break;
		case TYPE_BYTE:
			data.ucData	= *(BYTE *)(m_pcDB + dwOffset);
			break;
		case TYPE_WORD:
			data.wData	= *(WORD *)(m_pcDB + dwOffset);
			break;
		case TYPE_DWORD:
			data.dwData	= *(DWORD *)(m_pcDB + dwOffset);
			break;
		case TYPE_CHAR:
			data.cData	= *(char *)(m_pcDB + dwOffset);
			break;
		case TYPE_SHORT:
			data.nData	= *(short *)(m_pcDB + dwOffset);
			break;
		case TYPE_INT:
			data.lData	= *(long *)(m_pcDB + dwOffset);
			break;
		case TYPE_FLOAT:
			data.fData	= *(float *)(m_pcDB + dwOffset);
			break;
		case TYPE_DOUBLE:
			data.dData	= *(double *)(m_pcDB + dwOffset);
			break;
		case TYPE_STRING:
			data.pcData	= (char *)(m_pcDB + dwOffset);
			break;
		case TYPE_UNKNOWN:
		default:
			data.dwState	= DB_ERR_TYPE;
	}
/*	if (nIndex == 0)
		{
		
		//printf("ReadIndex%d=%ld \n", nIndex, data.nData);
		}*/
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: ReadDefault(char* pID)									|
|	remark		: Read default value from database							|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pID			: Database string index						|
|																			|
|	return 		: dbvalue		: 											|
+--------------------------------------------------------------------------*/
DBVALUE				CDatabase::ReadDefault(char* pID)
{
	char 			cszStrID[256];
	int				nIndex	= 0;
	DBVALUE			data;
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pID, sizeof(cszStrID));
	nIndex = m_pIDList->GetIndex(cszStrID);
	memset(&data, 0, sizeof(DBVALUE));
	if (nIndex < 0)		return data;
	
	data = ReadDefault(nIndex);
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: ReadDefault(int nIndex)									|
|	remark		: Read default value from database  						|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: nIndex		:Database Index								|
|																			|
|	return 		: dbvalue		: 											|
+--------------------------------------------------------------------------*/
DBVALUE				CDatabase::ReadDefault(int nIndex)
{
	unsigned int	dwOffset;
	DBVALUE			data;

	memset(&data, 0, sizeof(DBVALUE));
	if(m_wState & DB_ERR_FATAL)		return data;
	
	data.dwIndex	= nIndex;
	data.dwState	= DB_SUCCESS;
	if(nIndex > m_nIDCount || nIndex < 0)
	{
		data.dwState	= DB_ERR_OUTRANGE;
		return data;
	}
	dwOffset	= m_pDataAttr[nIndex].dwOffset;

	if(dwOffset == 0xFFFFFFFF)
	{
		data.dwState	= DB_ERR_NOUSED;
		return data;
	}
	
	//data.DataType			= m_pDataAttr[nIndex].DataType;
	memcpy(&data.DataType, &m_pDataAttr[nIndex].DataType, sizeof(DATATYPE));
	data.dwDataPath			= m_pDataAttr[nIndex].dwDataPath;
	data.dwPowerSupply		= m_pDataAttr[nIndex].dwPowerSupply;
	data.dwMaxIndex			= m_pDataAttr[nIndex].dwMaxIndex;
	data.dwMinIndex			= m_pDataAttr[nIndex].dwMinIndex;
	data.wDataLabel			= m_pDataAttr[nIndex].wDataLabel;
	data.wPrecision			= m_pDataAttr[nIndex].wPrecision;
	data.wDisplayPrecision	= m_pDataAttr[nIndex].wDisplayPrecision;
	data.dwPrivilege		= m_pDataAttr[nIndex].dwPrivilege;
	data.dwUnit				= m_pDataAttr[nIndex].dwUnit;
	data.lID			= IndexToDataID(nIndex);	//	2007/1/5 01:24¤U¤È
	switch(data.DataType.wType)
	{
		case TYPE_BOOL:
			data.wData	= *(BOOL *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_BYTE:
			data.ucData	= *(BYTE *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_WORD:
			data.wData	= *(WORD *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_DWORD:
			data.dwData	= *(DWORD *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_CHAR:
			data.cData	= *(char *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_SHORT:
			data.nData	= *(short *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_INT:
			data.lData	= *(long *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_FLOAT:
			data.fData	= *(float *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_DOUBLE:
			data.dData	= *(double *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_STRING:
			data.pcData	= (char *)(m_pcDefDB + dwOffset);
			break;
		case TYPE_UNKNOWN:
		default:
			data.dwState	= DB_ERR_TYPE;
	}
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: Write(DBVALUE *pDate)										|
|	remark		: ¦V¸ê®Æ®w¼g¤J«ü©wªº¸ê®Æ,									|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºA							|
+--------------------------------------------------------------------------*/
DBVALUE	CDatabase::Write(int nIndex, void *pData, BOOL bSave)
{
	DBVALUE			data;
	DWORD			dwOffset;
	DBVALUE			dbValue;
	pthread_mutex_lock(&mutex);
	data.dwIndex	= nIndex;
	data.dwState	= DB_SUCCESS;
	
	if(pData == NULL)						data.dwState   |= DB_ERR_DATA;
	if(nIndex > m_nIDCount || nIndex < 0)	data.dwState   |= DB_ERR_OUTRANGE;
	if((m_wState & DB_ERR_FATAL) != 0)		data.dwState   |= DB_ERR_DB;
	
	if(data.dwState != DB_SUCCESS)
	{
		pthread_mutex_unlock(&mutex);
		return data;
	}
	
	dwOffset	= m_pDataAttr[nIndex].dwOffset;	
	if(dwOffset == 0xFFFFFFFF)
	{
		data.dwState   |= DB_ERR_NOUSED;
		pthread_mutex_unlock(&mutex);
		return data;
	}
	//data.DataType	= m_pDataAttr[nIndex].DataType;
	memcpy(&data.DataType, &m_pDataAttr[nIndex].DataType, sizeof(DATATYPE));
	dbValue	= Read(nIndex);
/*	if (nIndex <= 600)
		{
		//printf("WriteIndex%d=%ld \n", nIndex, data.nData);
		}*/
	
	tmMsgParaID 	MsgParaID;
	
	memset(&MsgParaID, 0, sizeof(MsgParaID));
	memcpy(MsgParaID.szOldValue, m_pcDB + dwOffset, 8);
	
	switch(data.DataType.wType)
	{
		case TYPE_BOOL:
			*(BOOL *)(m_pcDB + dwOffset)	= *(BOOL *)pData;
			data.wData		= *(BOOL *)(m_pcDB + dwOffset);
			break;
		case TYPE_BYTE:
			*(BYTE *)(m_pcDB + dwOffset)	= *(BYTE *)pData;
			data.ucData		= *(BYTE *)(m_pcDB + dwOffset);
			break;
		case TYPE_WORD:
			*(WORD *)(m_pcDB + dwOffset)	= *(WORD *)pData;
			data.wData		= *(WORD *)(m_pcDB + dwOffset);
			break;
		case TYPE_DWORD:
			*(DWORD *)(m_pcDB + dwOffset)	= *(DWORD *)pData;
			data.dwData		= *(DWORD *)(m_pcDB + dwOffset);
			break;
		case TYPE_CHAR:
			*(char *)(m_pcDB + dwOffset)	= *(char *)pData;
			data.cData		= *(char *)(m_pcDB + dwOffset);
			break;
		case TYPE_SHORT:
			*(short *)(m_pcDB + dwOffset)	= *(short *)pData;
			data.nData		= *(short *)(m_pcDB + dwOffset);
			break;
		case TYPE_INT:
			*(long *)(m_pcDB + dwOffset)	= *(long *)pData;
			data.lData		= *(long *)(m_pcDB + dwOffset);
			break;
		case TYPE_FLOAT:
			*(float *)(m_pcDB + dwOffset)	= *(float *)pData;
			data.fData		= *(float *)(m_pcDB + dwOffset);
			break;
		case TYPE_DOUBLE:
			*(double *)(m_pcDB + dwOffset)	= *(double *)pData;
			data.dData		= *(double *)(m_pcDB + dwOffset);
			break;
		case TYPE_STRING:
			strncpy(m_pcDB + dwOffset, (char *)pData, data.DataType.wLength);
			data.pcData		= (char *)(m_pcDB + dwOffset);
			break;
		case TYPE_UNKNOWN:
		default:
			data.dwState	= DB_ERR_TYPE;
			break;
	}
	if(g_ptaskinet != NULL)
	{
		DWORD dwDBID = IndexToDataID(data.dwIndex);
		DWORD* pID = 0;
	
		pID = (DWORD*)bsearch(&dwDBID, g_dwInetMsgDataIDList, g_wInetMsgDataIDCount, sizeof(DWORD), cmp_dword);
		if(pID != NULL)
		{
			MsgParaID.dwDBID = *pID;
			
			g_ptaskinet->ReqValues(CONST_MSG_PARA_ID, &MsgParaID, sizeof(MsgParaID));
			
			//SendMsg(MSG_SH_INET_PARADATAID, ID, (LPARAM)szOldValue, NULL);
		}
	}
	if (bSave)									//Yang 2007/04/21
		Save(data.DataType.wLength, dwOffset, nIndex);
	pthread_mutex_unlock(&mutex);
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: Write(char *pID, void *pData)								|
|	remark		: ¦V¸ê®Æ®w¼g¤J«ü©wªº¸ê®Æ,									|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pID			: ¸ê®ÆID									|
|				  pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºA							|
+--------------------------------------------------------------------------*/
DBVALUE	CDatabase::Write(char *pID, void *pData, BOOL bSave)
{
	DBVALUE			data;
	char 			cszStrID[256];
	int				nIndex = 0;
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pID, sizeof(cszStrID));
	nIndex	= m_pIDList->GetIndex(cszStrID);
	memset(&data, 0 , sizeof(DBVALUE));
	if (nIndex < 0)		return data;

	data = Write(nIndex, pData, bSave);
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: WriteDefault(int nIndex, void *pData)						|
|	remark		: ¦V¸ê®Æ®w¤å¥ó¤¤¼g¤J«ü©wªº¸ê®Æ,								|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: nIndex		: ¸ê®Æindex									|
|				  pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºA							|
+--------------------------------------------------------------------------*/
DBVALUE	CDatabase::WriteDefault(int nIndex, void *pData, BOOL bSave)
{
	DBVALUE			data;
	DWORD			dwOffset;
	struct	stat Stat;
	int		nFileLength;
	CtmFile	File;
	
	pthread_mutex_lock(&mutex);
	data.dwIndex	= nIndex;
	data.dwState	= DB_SUCCESS;
	
	if(pData == NULL)						data.dwState   |= DB_ERR_DATA;
	if(nIndex > m_nIDCount || nIndex < 0)	data.dwState   |= DB_ERR_OUTRANGE;
	if((m_wState & DB_ERR_FATAL) != 0)		data.dwState   |= DB_ERR_DB;
	
	if(data.dwState != DB_SUCCESS)
	{
		pthread_mutex_unlock(&mutex);
		return data;
	}
	
	dwOffset	= m_pDataAttr[nIndex].dwOffset;	
	if(dwOffset == 0xFFFFFFFF)
	{
		data.dwState   |= DB_ERR_NOUSED;
		pthread_mutex_unlock(&mutex);
		return data;
	}
	//data.DataType	= m_pDataAttr[nIndex].DataType;
	memcpy(&data.DataType, &m_pDataAttr[nIndex].DataType, sizeof(DATATYPE));

	switch(data.DataType.wType)
	{
		case TYPE_BOOL:
			*(BOOL *)(m_pcDB + dwOffset)	= *(BOOL *)pData;
			data.wData		= *(BOOL *)(m_pcDB + dwOffset);
			break;
		case TYPE_BYTE:
			*(BYTE *)(m_pcDB + dwOffset)	= *(BYTE *)pData;
			data.ucData		= *(BYTE *)(m_pcDB + dwOffset);
			break;
		case TYPE_WORD:
			*(WORD *)(m_pcDB + dwOffset)	= *(WORD *)pData;
			data.wData		= *(WORD *)(m_pcDB + dwOffset);
			break;
		case TYPE_DWORD:
			*(DWORD *)(m_pcDB + dwOffset)	= *(DWORD *)pData;
			data.dwData		= *(DWORD *)(m_pcDB + dwOffset);
			break;
		case TYPE_CHAR:
			*(char *)(m_pcDB + dwOffset)	= *(char *)pData;
			data.cData		= *(char *)(m_pcDB + dwOffset);
			break;
		case TYPE_SHORT:
			*(short *)(m_pcDB + dwOffset)	= *(short *)pData;
			data.nData		= *(short *)(m_pcDB + dwOffset);
			break;
		case TYPE_INT:
			*(long *)(m_pcDB + dwOffset)	= *(long *)pData;
			data.lData		= *(long *)(m_pcDB + dwOffset);
			break;
		case TYPE_FLOAT:
			*(float *)(m_pcDB + dwOffset)	= *(float *)pData;
			data.fData		= *(float *)(m_pcDB + dwOffset);
			break;
		case TYPE_DOUBLE:
			*(double *)(m_pcDB + dwOffset)	= *(double *)pData;
			data.dData		= *(double *)(m_pcDB + dwOffset);
			break;
		case TYPE_STRING:
			strncpy(m_pcDB + dwOffset, (char *)pData, data.DataType.wLength);
			data.pcData		= (char *)(m_pcDB + dwOffset);
			break;
		case TYPE_UNKNOWN:
		default:
			data.dwState	= DB_ERR_TYPE;
			break;
	}
	if (bSave)									//Yang 2007/04/21
		Save(data.DataType.wLength, dwOffset, nIndex);
	//James add ¥i¥H©¹¤å¥ó¤¤¼g¤JÀq»{­È
	//­pºâconfig¤å¥ó©ñ¸mªº¸ô®|
	char cszDefaultFile[256] = "\0", *pConfigFile=NULL, *pConfigPath = NULL;
	pConfigFile = CtmConfig::GetInstance()->GetConfigPath();
	pConfigPath = strstr(CtmConfig::GetInstance()->GetConfigPath(),"config.ini");
	if(pConfigFile == pConfigPath)
	{
		pConfigPath = NULL;
	}
	else if(pConfigPath != NULL)
	{
		strcpy(cszDefaultFile,CtmConfig::GetInstance()->GetConfigPath());
		if(pConfigPath > pConfigFile)
			cszDefaultFile[pConfigPath-pConfigFile] = '\0';
	}
	sprintf(cszDefaultFile, "%s%s", cszDefaultFile, CtmConfig::GetInstance()->GetDBDefault());
	if(stat(cszDefaultFile, &Stat) == tmFILE_GOOD)
	{
		File.Open(cszDefaultFile);
		nFileLength	= File.Seek(0, SEEK_END);
		if((DWORD)nFileLength == m_dwLength)
		{
			File.Seek(dwOffset, SEEK_SET);
			File.Write(pData, data.DataType.wLength);				//wType -> wLength	James modify 2011/6/30 ¤U¤È 12:39:40
		}
		File.Close();
	}
	
	pthread_mutex_unlock(&mutex);
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: WriteDefault(char *pID, void *pData)						|
|	remark		: ¦V¸ê®Æ®w¤å¥ó¤¤¼g¤J«ü©wªº¸ê®Æ,								|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pID			: ¸ê®ÆID									|
|				  pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºA							|
+--------------------------------------------------------------------------*/
DBVALUE	CDatabase::WriteDefault(char *pID, void *pData, BOOL bSave)
{
	DBVALUE			data;
	char 			cszStrID[256];
	int				nIndex = 0;
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pID, sizeof(cszStrID));
	nIndex	= m_pIDList->GetIndex(cszStrID);
	memset(&data, 0 , sizeof(DBVALUE));
	if (nIndex < 0)		return data;
	data = WriteDefault(nIndex, pData);
	return data;
}

/*--------------------------------------------------------------------------+
|	function	: WriteDefaults(int nIndex, char *psz, int nLength, BOOL bSave)|
|	remark		: ¦V¸ê®Æ®w¤å¥ó¤¤¼g¤J«ü©wªº¸ê®Æ,								|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pID			: ¸ê®ÆID									|
|				  pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºA							|
+--------------------------------------------------------------------------*/
char*	CDatabase::WriteDefaults(int nIndex, char *psz, int nLength, BOOL bSave)
{
	WORD	wStrLength;
	struct	stat Stat;
	int		nFileLength;
	CtmFile	File;
	
	if(m_pcDB == NULL  || nIndex < 0)
		return NULL;
	wStrLength = Read(nIndex).DataType.wLength;
	if (nLength > wStrLength) nLength = wStrLength;
		strncpy(m_pcDB + m_pDataAttr[nIndex].dwOffset, psz, nLength);
	if (bSave)
		Save(m_pDataAttr[nIndex].DataType.wLength, m_pDataAttr[nIndex].dwOffset, nIndex);

	//James add ¥i¥H©¹¤å¥ó¤¤¼g¤JÀq»{­È
	//­pºâconfig¤å¥ó©ñ¸mªº¸ô®|
	char cszDefaultFile[256] = "\0", *pConfigFile=NULL, *pConfigPath = NULL;
	pConfigFile = CtmConfig::GetInstance()->GetConfigPath();
	pConfigPath = strstr(CtmConfig::GetInstance()->GetConfigPath(),"config.ini");
	if(pConfigFile == pConfigPath)
	{
		pConfigPath = NULL;
	}
	else if(pConfigPath != NULL)
	{
		strcpy(cszDefaultFile,CtmConfig::GetInstance()->GetConfigPath());
		if(pConfigPath > pConfigFile)
			cszDefaultFile[pConfigPath-pConfigFile] = '\0';
	}
	sprintf(cszDefaultFile, "%s%s", cszDefaultFile, CtmConfig::GetInstance()->GetDBDefault());
	if(stat(cszDefaultFile, &Stat) == tmFILE_GOOD)
	{
		File.Open(cszDefaultFile);
		nFileLength	= File.Seek(0, SEEK_END);
		if((DWORD)nFileLength == m_dwLength)
		{
			File.Seek(m_pDataAttr[nIndex].dwOffset, SEEK_SET);
			File.Write(psz, nLength);			
		}
		File.Close();
	}
	return m_pcDB + m_pDataAttr[nIndex].dwOffset;
}


/*--------------------------------------------------------------------------+
|	function	: WriteDefaults(char *pID, char *psz, int nLength, BOOL bSave)|
|	remark		: ¦V¸ê®Æ®w¤å¥ó¤¤¼g¤J«ü©wªº¸ê®Æ,								|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pID			: ¸ê®ÆID									|
|				  pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºA							|
+--------------------------------------------------------------------------*/
char*	CDatabase::WriteDefaults(char *pID, char *psz, int nLength, BOOL bSave)
{
	char 		cszStrID[256];
	int			nIndex = 0;
	
	memset(cszStrID, 0, sizeof(cszStrID));
	if (pID != NULL && nLength >= 0)
	{
		Trim(cszStrID, pID, sizeof(cszStrID));
		nIndex	= m_pIDList->GetIndex(cszStrID);
		if (nIndex < 0) 
			return	NULL;
		if (g_pDatabase->Read(nIndex).DataType.wType == TYPE_STRING)
		{
			return WriteDefaults(nIndex, psz, nLength);
		}
		else
		{
			return NULL;
		}
	}
}

/*--------------------------------------------------------------------------+
|	function	: WriteBatch(void *pData,	int	nOffset, int nLength, 		|	
                  BOOL bSave)												|
|	remark		: §å¶q¼g¤J¼Æ¾Ú¨ìSDRAM¡BSRAM									|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: pData			: ¸ê®Æ¤º®e									|
|				  nOffset		: °_©l°¾²¾¶q								|
|				  nLength		: ¸ê®Æªø«×									|
|	return 		:				: ¼g¤Jªø«×									|
+--------------------------------------------------------------------------*/
int		CDatabase::WriteBatch(void *pData,	int	nOffset, int nLength, BOOL bSave)		//Mario debug
{
	int	nResult = 0;
	CtmSarmRecordFileControl*	pSram;
	//nLength = m_dwLength-nOffset;	//Sunny<20100414> add ¤£¬O¾ã­Ósttruct³£¦s¨ì¸ê®Æ®w
	//printf("WriteBatch:%ld %ld %ld \n",nOffset,nLength,m_dwLength);
	if(m_pcDB != NULL && nOffset >= 0 && (nOffset+nLength) <= m_dwLength && nLength >= 0)
	{
		if(bSave)
		{
			memcpy(m_pcDB + nOffset, pData, nLength);	//20110708 move to front
			pSram->GetInstance()->WriteRecord(ID_DB_SRAMFILE, 1, m_pcDB + nOffset, nLength, nOffset);
		}
		else
		{
			memcpy(m_pcDB + nOffset, pData, nLength);
		}
	}
	
	return nResult;	
}

/*--------------------------------------------------------------------------+
|	function	: ReadBatch(void *pData,	int	nOffset, int nLength) 		|	
|	remark		: §å¶q±qDRAM¤¤Åª¨ú¼Æ¾Ú 									    |
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: pData			: ¦sÀx¸ê®Æ¤º®e								|
|				  nOffset		: °_©l°¾²¾¶q								|
|				  nLength		: ¸ê®Æªø«×									|
|	return 		:				: Åª¨úªø«×									|
+--------------------------------------------------------------------------*/
int		CDatabase::ReadBatch(void *pData,	int	nOffset, int nLength)	//Mario debug
{
	int	nResult = 0;
	//printf("ReadBatch:%ld %ld %ld \n",nOffset,nLength,m_dwLength);
	if(m_pcDB != NULL && nOffset >= 0 && (nOffset+nLength) <= m_dwLength && nLength >= 0)
	{
		memcpy(pData, m_pcDB + nOffset, nLength);
	}
	
	return nResult;
}

/*--------------------------------------------------------------------------+
|	function	: DataIDGetOffset(int nDataID)         						|
|	remark		: Åª¨ú¸ê®Æªºoffset											|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: nDataID		: ¸ê®ÆDataID								|
|																			|
|	return 		:				: offset ¦a§}								|
+--------------------------------------------------------------------------*/
int		CDatabase::DataIDGetOffset(int nDataID)	//Mario debug
{
	int		nIndex = 0;
	DWORD	dwOffset = 0;
	nIndex	= DataIDToIndex(nDataID);
	
	IndexGetOffset(nIndex);
}

/*--------------------------------------------------------------------------+
|	function	: IndexGetOffset(int nIndex)         						|
|	remark		: Åª¨ú¸ê®Æªºoffset											|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: nDataID		: ¸ê®ÆDataID								|
|																			|
|	return 		:				: offset ¦a§}								|
+--------------------------------------------------------------------------*/
int		CDatabase::IndexGetOffset(int nIndex)		//Mario debug
{
	DWORD	dwOffset = 0;;
	
	if(nIndex > m_nIDCount || nIndex < 0)
	{
		return -1;
	}
	dwOffset	= m_pDataAttr[nIndex].dwOffset;
	if(dwOffset == 0xFFFFFFFF)
	{
		return -1;
	}
	return dwOffset;
}


/*--------------------------------------------------------------------------+
|	function	: DataIDGetDataType(int nDataID)         					|
|	remark		: Åª¨ú¸ê®Æªºdatatype										|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: nDataID		: ¸ê®ÆDataID								|
|																			|
|	return 		:				: offset ¦a§}								|
+--------------------------------------------------------------------------*/
DATATYPE		CDatabase::DataIDGetDataType(int	nDataID)		//Mario debug
{
	int	nIndex = 0;
	nIndex= DataIDToIndex(nDataID);
	return	IndexGetDataType(nIndex);
}


/*--------------------------------------------------------------------------+
|	function	: IndexGetDataType(int nIndex)         						|
|	remark		: Åª¨ú¸ê®Æªºdatatype										|
|---------------------------------------------------------------------------|
|	call		: 															|
|	para		: nIndex		: ¸ê®ÆnIndex								|
|																			|
|	return 		:				: datatype									|
+--------------------------------------------------------------------------*/
DATATYPE		CDatabase::IndexGetDataType(int	nIndex)		//Mario debug
{
	DATATYPE	datatype;
	if(nIndex > m_nIDCount || nIndex < 0)
	{
		memset(&datatype, 0 , sizeof(DATATYPE));
		return datatype;
	}
	return m_pDataAttr[nIndex].DataType;
}

/*--------------------------------------------------------------------------+
|	function	: GetIndex(char *psz)										|
|	remark		: ¨ú¹ïÀ³¸ê®ÆªºID											|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºAªº«ü°w						|
+--------------------------------------------------------------------------*/
int		CDatabase::GetIndex(char *psz)
{
	//
	if(m_pIDList == NULL || psz == NULL)	return -1;
	return m_pIDList->GetIndex(psz);
}

/*--------------------------------------------------------------------------+
|	function	: CStrings::GetString(int nIndex)							|
|	remark		: ¨ú¹ïÀ³¸ê®ÆªºID											|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºAªº«ü°w						|
+--------------------------------------------------------------------------*/
char*		CDatabase::GetString(int nIndex)
{
	if(m_pIDList == NULL || nIndex < 0)	return NULL;
	return m_pIDList->GetString(nIndex);
}
/*--------------------------------------------------------------------------+
|	function	: CStrings::GetString(int nIndex)							|
|	remark		: ¨ú¹ïÀ³¸ê®ÆªºID											|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºAªº«ü°w						|
+--------------------------------------------------------------------------*/
int		CDatabase::GetDataID(char *psz)
{
	int		nIndex;
	
	if ((nIndex = GetIndex(psz)) == -1) return -1;
	return IndexToDataID(nIndex);
	
}
/*--------------------------------------------------------------------------+
|	function	: Write(DBVALUE *pDate)										|
|	remark		: ¦V¸ê®Æ®w¼g¤J«ü©wªº¸ê®Æ,									|
|---------------------------------------------------------------------------|
|	call		: Write(pDate)												|
|	para		: pData			: ¸ê®Æ¤º®e									|
|																			|
|	return 		:				: ¸ê®Æ¤º®e¤Îª¬ºAªº«ü°w						|
+--------------------------------------------------------------------------*/
DBVALUE*	CDatabase::Write(DBVALUE *pData, BOOL bSave)
{
	unsigned int	dwOffset;
	DBVALUE			data;
//	//printf("dwIndex=%d,m_nIDCount=%d,", pData->dwIndex, m_nIDCount);
	pthread_mutex_lock(&mutex);
	//=======
	// ÀË¬d¸ê®Æ¬O§_¥i¥H¼g¤J
	//=======
	pData->dwState		= DB_SUCCESS;
	if(pData->dwIndex > (unsigned)m_nIDCount  || pData->dwIndex < 0)
	{
		pData->dwState	= DB_ERR_OUTRANGE;
		pthread_mutex_unlock(&mutex);
		return pData;
	}
	
	dwOffset	= m_pDataAttr[pData->dwIndex].dwOffset;
//	//printf("dwOffset=%d", dwOffset);
	if(dwOffset == 0xFFFFFFFF)
	{
		pData->dwState	= DB_ERR_NOUSED;
		pthread_mutex_unlock(&mutex);
		return pData;
	}
	
	//data.DataType	= m_pDataAttr[pData->dwIndex].DataType;
	memcpy(&data.DataType, &m_pDataAttr[pData->dwIndex].DataType, sizeof(DATATYPE));
		
	if(pData->DataType.wLength > data.DataType.wLength)	// ¬O§_¦X²z¡H
	{
//	//printf("wLength_com=%d, type=%d, wLength_db=%d, type=%d\n", pData->DataType.wLength, pData->DataType.wType, data.DataType.wLength, data.DataType.wType);
		pData->dwState	= DB_ERR_TYPE;
		pthread_mutex_unlock(&mutex);
		return pData;
	}
	switch(pData->DataType.wType)
	{
		case TYPE_BOOL:
			*(BOOL *)(m_pcDB + dwOffset)	= pData->wData;
			break;
		case TYPE_BYTE:
			*(BYTE *)(m_pcDB + dwOffset)	= pData->ucData;
			break;
		case TYPE_WORD:
			*(WORD *)(m_pcDB + dwOffset)	= pData->wData;
			break;
		case TYPE_DWORD:
			*(DWORD *)(m_pcDB + dwOffset)	= pData->dwData;
			break;
		case TYPE_CHAR:
			*(char *)(m_pcDB + dwOffset)	= pData->cData;
			break;
		case TYPE_SHORT:
			*(short *)(m_pcDB + dwOffset)	= pData->nData;
			break;
		case TYPE_INT:
			*(long *)(m_pcDB + dwOffset)	= pData->lData;
			break;
		case TYPE_FLOAT:
			*(float *)(m_pcDB + dwOffset)	= pData->fData;
			break;
		case TYPE_DOUBLE:
			*(double *)(m_pcDB + dwOffset)	= pData->dData;
//			//printf("double=%lf \n", *(double *)(m_pcDB + dwOffset));
			break;
		case TYPE_STRING:
			if(pData->pcData == NULL)
			{
				pData->dwState	= DB_ERR_DATA;
				pthread_mutex_unlock(&mutex);
				return pData;
			}
			strncpy(m_pcDB + dwOffset, pData->pcData, pData->DataType.wLength);
			break;
		case TYPE_UNKNOWN:
		default:
			data.dwState	= DB_ERR_TYPE;
	}
	if (bSave)
		Save(data.DataType.wLength, dwOffset, pData->dwIndex);
	pthread_mutex_unlock(&mutex);
	return pData;
}

/*--------------------------------------------------------------------------+
|	function	: Readb(int nIndex)											|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ,¸ê®ÆÃþ«¬¬°byte				|
|---------------------------------------------------------------------------|
|	call		: Readb(int nIndex)											|
|	para		: nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ¸ê®Æ¤º®e									|
|				:	0xFF		: Åª¨ú¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/	
BYTE	CDatabase::Readb(int nIndex)
{
	return	Read(nIndex).ucData;
}

/*--------------------------------------------------------------------------+
|	function	: Readw(int nIndex)											|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ								|
|---------------------------------------------------------------------------|
|	call		: Readw(int nIndex)											|
|	para		: nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ¸ê®Æ¤º®e									|
|				:	0xFFFF		: Åª¨ú¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
WORD	CDatabase::Readw(int nIndex)
{
	return	Read(nIndex).wData;
}

/*--------------------------------------------------------------------------+
|	function	: Readl(int nIndex)											|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ								|
|---------------------------------------------------------------------------|
|	call		: Readl(int nIndex)											|
|	para		: nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ¸ê®Æ¤º®e									|
|				:	0xFFFFFFFF	: Åª¨ú¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
long	CDatabase::Readl(int nIndex)
{
	return	Read(nIndex).lData;
}

/*--------------------------------------------------------------------------+
|	function	: Reads(int nIndex)											|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ								|
|---------------------------------------------------------------------------|
|	call		: Reads(int nIndex)											|
|	para		: nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ¦r¦ê«ü°w									|
|				:	NULL		: Åª¨ú¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
char* CDatabase::Reads(int nIndex)
{
	return	Read(nIndex).pcData;
}

/*--------------------------------------------------------------------------+
|	function	: Writeb(BYTE Values, int nIndex)							|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ								|
|---------------------------------------------------------------------------|
|	call		: Writeb(Values, nIndex)									|
|	para		: Values		: ·sªº­È									|
|				  nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ­ì©l¸ê®Æ¤º®e								|
|				:	0xFF		: ¼g¨ú¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
BYTE	CDatabase::Writeb(BYTE Values, int nIndex, BOOL bSave)
{
	BYTE	OldValues;
	
	if(m_pcDB == NULL  || nIndex < 0)
		return 0xFF;	
	
	OldValues	= m_pcDB[m_pDataAttr[nIndex].dwOffset];
	
	m_pcDB[m_pDataAttr[nIndex].dwOffset]	= Values;
	if (bSave)
		Save(m_pDataAttr[nIndex].DataType.wLength, m_pDataAttr[nIndex].dwOffset, nIndex);
	return OldValues;
}

/*--------------------------------------------------------------------------+
|	function	: Writew(WORD Values, int nIndex)							|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ								|
|---------------------------------------------------------------------------|
|	call		: Writew(Values, nIndex)									|
|	para		: Values		: ·sªº­È									|
|				  nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ­ì©l¸ê®Æ¤º®e								|
|				:	0xFFFF		: ¼g¨ú¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
WORD	CDatabase::Writew(WORD Values, int nIndex, BOOL bSave)
{
	WORD	OldValues;
	
	if(m_pcDB == NULL  || nIndex < 0)
		return 0xFFFF;
	
	OldValues	= *((WORD *)(m_pcDB + m_pDataAttr[nIndex].dwOffset));
	
	*((WORD *)(m_pcDB + m_pDataAttr[nIndex].dwOffset))	= Values;
	if (bSave)
		Save(m_pDataAttr[nIndex].DataType.wLength, m_pDataAttr[nIndex].dwOffset, nIndex);
	return OldValues;
}

/*--------------------------------------------------------------------------+
|	function	: Writel(DWORD Values, int nIndex)							|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ								|
|---------------------------------------------------------------------------|
|	call		: Writel(Values, nIndex)									|
|	para		: Values		: ·sªº­È									|
|				  nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|																			|
|	return 		:	¨ä¥¦		: ­ì©l¸ê®Æ¤º®e								|
|				:	0xFFFFFFFF	: ¼g¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
DWORD	CDatabase::Writel(DWORD Values, int nIndex, BOOL bSave)
{
	DWORD	OldValues;
	
	if(m_pcDB == NULL  || nIndex < 0)
		return 0xFFFF;
	
	OldValues	= *((DWORD *)(m_pcDB + m_pDataAttr[nIndex].dwOffset));
	
	*((DWORD *)(m_pcDB + m_pDataAttr[nIndex].dwOffset))	= Values;
	if (bSave)
		Save(m_pDataAttr[nIndex].DataType.wLength, m_pDataAttr[nIndex].dwOffset, nIndex);
	return OldValues;
}

/*--------------------------------------------------------------------------+
|	function	: Writes(DWORD Values, int nIndex)							|
|	remark		: ±q¸ê®Æ®wÅª¨únIndex«ü©wªº¸ê®Æ,ª`·N¸ê®Æªø«×¡A¥Ø«e¨S¦³¿ìªk	|
|				  §¹¥þ«Oµý¼g¤J¸ê®Æ¤£·|¨R±¼«á­±ªº¸ê®Æ						|
|---------------------------------------------------------------------------|
|	call		: Writel(Values, nIndex)									|
|	para		: psz			: ·sªº­È									|
|				  nIndex		: ¦bini¤å¥ó¤¤databaseªºsection¦WºÙ			|
|				  nLength		: ¤¹³\©ñ¤Jªº¦r¦ê³Ìªøªø«×					|
|																			|
|	return 		:	¨ä¥¦		: ­ì©l¸ê®Æ¤º®e								|
|				:	NULL		: ¼g¤£¦¨¥\©ÎªÌ¸ê®Æ¥¼©w¸q					|
+--------------------------------------------------------------------------*/
char*	CDatabase::Writes(char *psz, int nIndex, int nLength, BOOL bSave)
{
	WORD	wStrLength;
	
	if(m_pcDB == NULL  || nIndex < 0)
		return NULL;
	wStrLength = Read(nIndex).DataType.wLength;
	if (nLength > wStrLength) nLength = wStrLength;
	strncpy(m_pcDB + m_pDataAttr[nIndex].dwOffset, psz, nLength);
	if (bSave)
		Save(m_pDataAttr[nIndex].DataType.wLength, m_pDataAttr[nIndex].dwOffset, nIndex);
	return m_pcDB + m_pDataAttr[nIndex].dwOffset;
}

/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------+
|  Function :  DataIDToIndex(long lDataID)									|
|  Task     :  §â¦èªù¤lªºIDÂà¦¨¨t²ÎªºID                                     |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int	CDatabase::DataIDToIndex(long lDataID)
{
	int	nMinIndex, nMaxIndex, nIndex, nOldIndex = 0;
	if(lDataID > m_lMaxDataID || lDataID < 0)	return -1;
	
	nMinIndex	= 0;
	nMaxIndex	= m_nIDCount - 1;
	nIndex		= nMaxIndex / 2;
	
	if(m_pIndexToID[nMinIndex].lDataID == lDataID)	
		return m_pIndexToID[nMinIndex].nIndex;
	if(m_pIndexToID[nMaxIndex].lDataID == lDataID)	
		return m_pIndexToID[nMaxIndex].nIndex;
	
	do
	{
		if(m_pIndexToID[nIndex].lDataID == lDataID)	
			return m_pIndexToID[nIndex].nIndex;
		nOldIndex	= nIndex;
		if(m_pIndexToID[nIndex].lDataID > lDataID)
			nMaxIndex	= nIndex;
		else
			nMinIndex	= nIndex;
		nIndex		= (nMaxIndex + nMinIndex) / 2;
		
	}while(nOldIndex != nIndex);
//	printf("DataIDToIndex:DataID=%ld Not find Index!\n", lDataID);
	return -1;
};

/*--------------------------------------------------------------------------+
|  Function :  IndexToDataID(int nIndex)									|
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
long	CDatabase::IndexToDataID(int nIndex)
{
	if(nIndex > m_nMaxIndex || nIndex < 0)	return -1;
		
	return		m_pIndexToID[nIndex].lDataID;
}

DBRECORD	CDatabase::GetDBRecord(int	nIndex)
{
	DBRECORD	dbRecord;
	
	memset(&dbRecord, 0, sizeof(DBRECORD));
	if (nIndex < 0 || m_nIDCount < nIndex)  
		return dbRecord;
	
	return	m_pDBRecord[nIndex];
}

void		CDatabase::Reset()
{
	CtmFile	File;
	int		nFileLength;
	struct	stat Stat;
	
	if (stat(m_dbInfo.pszDefault, &Stat) == tmFILE_GOOD)
	{
		if (File.Open(m_dbInfo.pszDefault) == tmFILE_GOOD)
		{
			nFileLength	= File.Seek(0, SEEK_END);
			if((DWORD)nFileLength == m_dwLength)
			{
				File.Seek(0, SEEK_SET);
				File.Read(m_pcDB, m_dwLength);
			}
		}
		File.Close();
	}
}


void				CDatabase::CheckBackupDBInfo()
{
	DWORD   dwOffset=0;
	DWORD 	dwDefDBLen=0;
	BOOL	bExistFlag=FALSE;
	ifstream in;
	#ifdef D_ARM
	in.open("/usr/LastVerDBInfo.dat",ios::in|ios::binary);
	#else
	in.open("./LastVerDBInfo.dat",ios::in|ios::binary);
	#endif
	if(!in.is_open()) return ;
	in.read((char*)&dwOffset,sizeof(DWORD));
	if(!dwOffset)	return ;
	char*  pContent=new char[dwOffset];
	memset(pContent,0,dwOffset);
	in.read(pContent,dwOffset);
	in.read((char*)&dwDefDBLen,sizeof(DWORD));
	if(dwDefDBLen)
	{
		m_data->m_last_defdb=new char[dwDefDBLen];
		memset(m_data->m_last_defdb,0,dwDefDBLen);
		in.read(m_data->m_last_defdb,dwDefDBLen);
		bExistFlag=TRUE;	
	}
	
	in.close();
	DWORD  dwCalc=0;
	
	while(dwOffset!=dwCalc)
	{
		BPINFO	_value;
		//DWORD	_key;
		memcpy(&_value,pContent+dwCalc,sizeof(BPINFO));
		//_key=_value.dwID;
		//m_data->m_backup_dbinfo.insert(pair<DWORD,BPINFO>(_key,_value));
		int nIndex=DataIDToIndex(_value.dwID);
		if(nIndex>=0)
		{
			if(m_pDataAttr[nIndex].DataType.wType==_value.DataType.wType)
			{
				DWORD dwOffset=m_pDataAttr[nIndex].dwOffset;
				//if(dwOffset!=_value.dwOffset)
				switch(m_pDataAttr[nIndex].DataType.wType)
				{
					case TYPE_BOOL:
						*(BOOL *)(m_pcDB + dwOffset)= *(BOOL *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_BYTE:
						*(BYTE *)(m_pcDB + dwOffset)	= *(BYTE *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_WORD:
						if(bExistFlag)
						{
							if((*(WORD *)(m_data->m_last_defdb + _value.dwOffset)==0xFFFF)&&
								(*(WORD *)(m_data->m_last_db + _value.dwOffset)==0xFFFF)&&
								*(WORD *)(m_pcDB + dwOffset)!=0xFFFF)
							{
								;	
							}
							else
							{
								*(WORD *)(m_pcDB + dwOffset)	= *(WORD *)(m_data->m_last_db + _value.dwOffset);	
							}
						}
						else
							*(WORD *)(m_pcDB + dwOffset)	= *(WORD *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_DWORD:
						if(bExistFlag)
						{
							if((*(DWORD *)(m_data->m_last_defdb + _value.dwOffset)==0xFFFFFFFF)&&
								(*(DWORD *)(m_data->m_last_db + _value.dwOffset)==0xFFFFFFFF)&&
								*(DWORD *)(m_pcDB + dwOffset)!=0xFFFFFFFF)
							{
								;	
							}
							else
							{
								
								*(DWORD *)(m_pcDB + dwOffset)	= *(DWORD *)(m_data->m_last_db + _value.dwOffset);	
							}
						}
						else
							*(DWORD *)(m_pcDB + dwOffset)	= *(DWORD *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_CHAR:
						*(char *)(m_pcDB + dwOffset)	= *(char *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_SHORT:
						*(short *)(m_pcDB + dwOffset)	= *(short *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_INT:
						*(long *)(m_pcDB + dwOffset)	= *(long *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_FLOAT:
						*(float *)(m_pcDB + dwOffset)	= *(float *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_DOUBLE:
						*(double *)(m_pcDB + dwOffset)	= *(double *)(m_data->m_last_db + _value.dwOffset);
					break;
					case TYPE_STRING:
						{
							int nLength=min(m_pDataAttr[nIndex].DataType.wLength,_value.DataType.wLength);
							strncpy(m_pcDB + dwOffset, m_data->m_last_db + _value.dwOffset,nLength);
						}
					break;
					case TYPE_UNKNOWN:
					default:
						break;
				}
			}	
		}
		dwCalc+=sizeof(BPINFO);	
	}
	delete[] pContent;
	
}

void				CDatabase::BackupDBInfo()
{
	
	DWORD			dwOffset=0;
	if(m_nIDCount<=0)	return;
	char*			pContent=new char[m_nIDCount*sizeof(BPINFO)];
	for(int index=0; index<m_nIDCount; index++)
	{
		BPINFO	_value;
		_value.dwID=IndexToDataID(index);
		_value.dwOffset=m_pDataAttr[index].dwOffset;
		memcpy(&_value.DataType,&m_pDataAttr[index].DataType,sizeof(DATATYPE));
		memcpy(pContent+dwOffset,&_value,sizeof(BPINFO));
		dwOffset+=sizeof(BPINFO);
	}
	ofstream out;
	#ifdef	D_ARM
	out.open("/usr/LastVerDBInfo.dat",ios::out|ios::binary);
	#else
	out.open("./LastVerDBInfo.dat",ios::out|ios::binary);
	#endif
	DWORD dwPlaceholder=0;
	out.write((const char*)&dwPlaceholder,sizeof(DWORD));
	out.write(pContent,dwOffset);
	out.seekp(0,ios::beg);
	out.write((const char*)&dwOffset,sizeof(DWORD));
	out.seekp(dwOffset+sizeof(DWORD),ios::beg);
	out.write((const char*)&dwPlaceholder,sizeof(DWORD));
	out.write((const char*)m_pcDefDB,m_dwLength);
	out.seekp(dwOffset+sizeof(DWORD),ios::beg);
	out.write((const char*)&m_dwLength,sizeof(DWORD));
	out.close();
	delete[] pContent;
}
void	CDatabase::UpdateBackupDBInfo()
{
		BackupDBInfo();
}

void		Reset_DB(WORD	wType)
{
	int			nSize	= 0;
	WORD		wBlockID= 0;
	BYTE*		pcPB	= NULL;
	BYTE*		pcMach	= NULL;
	BYTE*		pcMachb	= NULL;
	BYTE*		pcOpt	= NULL;
	BYTE*		pcBuff	= NULL;
	
	switch(wType)
	{
		case RESET_DB_ALL:
			wBlockID = 0;
			break;
		case RESET_DB_DA:
			wBlockID = BLOCK_NEWPOWERMATCH_ID;
			break;
		case RESET_DB_MOLDSET:
			wBlockID = BLOCK_MOLDSET_ID;
			break;
		case RESET_DB_MACHINE:
			wBlockID = BLOCK_MACHINE_ID;
			break;
		case RESET_DB_OPERATION:
			wBlockID = BLOCK_OPERATION_ID;
			break;
		case RESET_DB_MACHINEB:
			wBlockID = BLOCK_MACHINEB_ID;
			break;
		default:
			wBlockID = 0;
			break;
	}
	if (wType != RESET_DB_ALL)
	{
		nSize = g_pBlock->GetBlockSize(wBlockID);
		pcBuff	   = (BYTE *)calloc(nSize, 1);
		if (pcBuff == NULL) return;
		g_pBlock->SetDefBlock(pcBuff, wBlockID);
		g_pBlock->GetBlock(pcBuff, wBlockID);
		if (pcBuff != NULL) 	free(pcBuff);
	}
	else
	{
		nSize = g_pBlock->GetBlockSize(BLOCK_NEWPOWERMATCH_ID);		//Get Current DA Data
		pcPB	   = (BYTE *)calloc(nSize, 1);
		g_pBlock->SetBlock(pcPB, BLOCK_NEWPOWERMATCH_ID);
		
		nSize = g_pBlock->GetBlockSize(BLOCK_MACHINE_ID);			//Get Current Machine Data
		pcMach	   = (BYTE *)calloc(nSize, 1);
		g_pBlock->SetBlock(pcMach, BLOCK_MACHINE_ID);
		
		nSize = g_pBlock->GetBlockSize(BLOCK_MACHINEB_ID);			//Get Current Machineb Data
		pcMachb	   = (BYTE *)calloc(nSize, 1);
		g_pBlock->SetBlock(pcMachb, BLOCK_MACHINEB_ID);
		
		nSize = g_pBlock->GetBlockSize(BLOCK_OPERATION_ID);			//Get Opertion Data
		pcOpt	   = (BYTE *)calloc(nSize, 1);
		g_pBlock->SetBlock(pcOpt, BLOCK_OPERATION_ID);			
		
		g_pDatabase->Reset();
		
		g_pBlock->GetBlock(pcPB, BLOCK_NEWPOWERMATCH_ID);			//Save Current DA Data
		g_pBlock->GetBlock(pcMach, BLOCK_MACHINE_ID);				//Save Current Machine Data
		g_pBlock->GetBlock(pcMachb, BLOCK_MACHINEB_ID);				//Save Current Machineb Data
		g_pBlock->GetBlock(pcOpt, BLOCK_OPERATION_ID);				//Save Opertion Data
		if (pcPB != NULL)		free(pcPB);
		if (pcMach != NULL)		free(pcMach);
		if (pcMachb != NULL)	free(pcMachb);
		if (pcOpt != NULL) 		free(pcOpt);
	}
//	printf("wBlockID=%d nSize=%d DBSize=%ld\n", wBlockID, nSize, g_pDatabase->GetDBSize());
	g_pDatabase->Save(g_pDatabase->GetDBSize(), 0);
	//James mark 2008/4/17 09:04¤W¤È by Andy
	//ClearErrorLog();
	//ClearMoniLog();
}

DBVIEWVALUE	GetDBValue(WORD	wIndex)
{
	DBVIEWVALUE	dbViewValue;
	DBVALUE		dbValue;
	int			nMultiple;
	
	memset(&dbValue, 0, sizeof(DBVALUE));
	memset(&dbViewValue, 0, sizeof(DBVIEWVALUE));
	
	dbValue = g_pDatabase->Read(wIndex);
	if (dbValue.dwState	!= DB_SUCCESS)	return dbViewValue;
	dbViewValue.wPrecision = dbValue.wPrecision;
	dbViewValue.dwPrivilege = dbValue.dwPrivilege;
	if (dbValue.dwDataPath == DATA_PATH_ELECTRIC)
		nMultiple = GetDecimalPowerVal(dbViewValue.wPrecision);
	else
		nMultiple = 1;
//	//printf("Index=%d Privilege=%ld Precision=%d\n", wIndex, dbValue.dwPrivilege, dbValue.wPrecision);
	if (dbValue.dwMaxIndex != 0xffffffff)	
		{
#ifdef	D_X86
		dbViewValue.lMaxValue = (long long)roundl(g_pDatabase->GetLimitValue(dbValue.dwMaxIndex, dbValue.DataType.wType)*nMultiple);
#else
#ifdef	D_ARM
		dbViewValue.lMaxValue = (long long)round(g_pDatabase->GetLimitValue(dbValue.dwMaxIndex, dbValue.DataType.wType)*nMultiple);
#endif
#endif
		}
	if (dbValue.dwMinIndex != 0xffffffff)	
		{
#ifdef	D_X86
		dbViewValue.lMinValue = (long long)roundl(g_pDatabase->GetLimitValue(dbValue.dwMinIndex, dbValue.DataType.wType)*nMultiple);
#else
#ifdef	D_ARM
		dbViewValue.lMinValue = (long long)round(g_pDatabase->GetLimitValue(dbValue.dwMinIndex, dbValue.DataType.wType)*nMultiple);
#endif
#endif
		}
	
	switch(dbValue.DataType.wType)
		{
		case TYPE_BOOL:
			dbViewValue.lValue = (long long)round(dbValue.wData*nMultiple);
			break;
		case TYPE_BYTE:
			dbViewValue.lValue = (long long)round(dbValue.ucData*nMultiple);
			break;
		case TYPE_WORD:
			dbViewValue.lValue = (long long)round(dbValue.wData*nMultiple);
			break;
		case TYPE_DWORD:
			dbViewValue.lValue = (long long)round(dbValue.dwData*nMultiple);
			break;
		case TYPE_CHAR:
			dbViewValue.lValue = (long long)round(dbValue.cData*nMultiple);
			break;
		case TYPE_SHORT:
			dbViewValue.lValue = (long long)round(dbValue.nData*nMultiple);
			break;
		case TYPE_INT:
			dbViewValue.lValue = (long long)round(dbValue.lData*nMultiple);
			break;
		case TYPE_FLOAT:
			nMultiple = GetDecimalPowerVal(dbViewValue.wPrecision);		//James add 2010/5/28
			dbViewValue.lValue = (long long)round(dbValue.fData*nMultiple);
			break;
		case TYPE_DOUBLE:
			nMultiple = GetDecimalPowerVal(dbViewValue.wPrecision);		//James add 2010/5/28
			dbViewValue.lValue = (long long)round(dbValue.dData*nMultiple);
			break;
		case TYPE_STRING:
			
			break;
		case TYPE_UNKNOWN:
		default:
			break;
		}
	
	return	dbViewValue;
}

DBVIEWVALUE		GetDBValue(char* pcID, BOOL* pbFind)					 
{
	
	pthread_mutex_lock(&g_DataReadMutex);
	int		nIndex;
	DBVIEWVALUE	dbViewValue;
	char 		cszStrID[256];
	if(pcID == NULL)
	{
		memset(&dbViewValue, 0, sizeof(dbViewValue));
		pthread_mutex_unlock(&g_DataReadMutex);
		return dbViewValue;
	}
	//printf("pcID=%x \n", pcID);
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pcID, sizeof(cszStrID));
	memset(&dbViewValue, 0, sizeof(dbViewValue));
	if (pcID == NULL || (nIndex = g_pDatabase->GetIndex(cszStrID)) == -1)
	{
		if (pbFind != NULL) *pbFind = FALSE;
		pthread_mutex_unlock(&g_DataReadMutex);
		return dbViewValue;
	}
	if (pbFind != NULL) *pbFind = TRUE;
	dbViewValue = GetDBValue(nIndex);
	pthread_mutex_unlock(&g_DataReadMutex);
	return	dbViewValue;
}

DBVIEWVALUE		GetDBDefaultValue(WORD wIndex)
{
	DBVIEWVALUE	dbViewValue;
	DBVALUE		dbValue;
	int			nMultiple;
	
	memset(&dbValue, 0, sizeof(DBVALUE));
	memset(&dbViewValue, 0, sizeof(DBVIEWVALUE));
	
	dbValue = g_pDatabase->ReadDefault(wIndex);
	if (dbValue.dwState	!= DB_SUCCESS)	return dbViewValue;
	dbViewValue.wPrecision = dbValue.wPrecision;
	dbViewValue.dwPrivilege = dbValue.dwPrivilege;
	if (dbValue.dwDataPath == DATA_PATH_ELECTRIC)
		nMultiple = GetDecimalPowerVal(dbViewValue.wPrecision);
	else
		nMultiple = 1;
//	//printf("Index=%d Privilege=%ld Precision=%d\n", wIndex, dbValue.dwPrivilege, dbValue.wPrecision);
	if (dbValue.dwMaxIndex != 0xffffffff)	
		{
#ifdef	D_X86
		dbViewValue.lMaxValue = (long long)roundl(g_pDatabase->GetLimitValue(dbValue.dwMaxIndex, dbValue.DataType.wType)*nMultiple);
#else
#ifdef	D_ARM
		dbViewValue.lMaxValue = (long long)round(g_pDatabase->GetLimitValue(dbValue.dwMaxIndex, dbValue.DataType.wType)*nMultiple);
#endif
#endif
		}
	if (dbValue.dwMinIndex != 0xffffffff)	
		{
#ifdef	D_X86
		dbViewValue.lMinValue = (long long)roundl(g_pDatabase->GetLimitValue(dbValue.dwMinIndex, dbValue.DataType.wType)*nMultiple);
#else
#ifdef	D_ARM
		dbViewValue.lMinValue = (long long)round(g_pDatabase->GetLimitValue(dbValue.dwMinIndex, dbValue.DataType.wType)*nMultiple);
#endif
#endif
		}
	
	switch(dbValue.DataType.wType)
		{
		case TYPE_BOOL:
			dbViewValue.lValue = (long long)round(dbValue.wData*nMultiple);
			break;
		case TYPE_BYTE:
			dbViewValue.lValue = (long long)round(dbValue.ucData*nMultiple);
			break;
		case TYPE_WORD:
			dbViewValue.lValue = (long long)round(dbValue.wData*nMultiple);
			break;
		case TYPE_DWORD:
			dbViewValue.lValue = (long long)round(dbValue.dwData*nMultiple);
			break;
		case TYPE_CHAR:
			dbViewValue.lValue = (long long)round(dbValue.cData*nMultiple);
			break;
		case TYPE_SHORT:
			dbViewValue.lValue = (long long)round(dbValue.nData*nMultiple);
			break;
		case TYPE_INT:
			dbViewValue.lValue = (long long)round(dbValue.lData*nMultiple);
			break;
		case TYPE_FLOAT:
			nMultiple = GetDecimalPowerVal(dbViewValue.wPrecision);
			dbViewValue.lValue = (long long)round(dbValue.fData*nMultiple);
			break;
		case TYPE_DOUBLE:
			nMultiple = GetDecimalPowerVal(dbViewValue.wPrecision);
			dbViewValue.lValue = (long long)round(dbValue.dData*nMultiple);
			break;
		case TYPE_STRING:
			
			break;
		case TYPE_UNKNOWN:
		default:
			break;
		}
	
	return	dbViewValue;
}

DBVIEWVALUE	GetDBDefaultValue(char* pcID)
{
	int		nIndex;
	DBVIEWVALUE	dbViewValue;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pcID, sizeof(cszStrID));
	memset(&dbViewValue, 0, sizeof(dbViewValue));
	if (pcID == NULL || (nIndex = g_pDatabase->GetIndex(cszStrID)) == -1)
		return dbViewValue;
	dbViewValue = GetDBDefaultValue(nIndex);
	
	return	dbViewValue;
}

WORD		GetDBString(char* pcID, char* pszStr, int nLength)
{
	WORD		wState = DB_ERR_DB;
	int			nIndex;
	DATATYPE	DataType;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pcID, sizeof(cszStrID));
	if (pcID != NULL && pszStr != NULL && nLength > 0)
		{
		if ((nIndex = g_pDatabase->GetIndex(cszStrID)) < 0) 
			return	wState;
		//DataType = g_pDatabase->Read(nIndex).DataType;
		memcpy(&DataType, &g_pDatabase->Read(nIndex).DataType, sizeof(DATATYPE));
		if (DataType.wType == TYPE_STRING)
			{
			if (DataType.wLength <= nLength)
				strcpy(pszStr, g_pDatabase->Reads(nIndex));
			else
				strncpy(pszStr, g_pDatabase->Reads(nIndex), nLength);
			wState = DB_SUCCESS;
			}
		}
	return	wState;
}

WORD		SetDBString(char* pcID, char* pszStr, int nLength)
{
	WORD		wState = DB_ERR_DB;
	int			nIndex;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pcID, sizeof(cszStrID));
	if (pcID != NULL && nLength >= 0)
		{
		if ((nIndex = g_pDatabase->GetIndex(cszStrID)) < 0) 
			return	wState;
		if (g_pDatabase->Read(nIndex).DataType.wType == TYPE_STRING)
			{
			g_pDatabase->Writes(pszStr, nIndex, nLength);
			wState = DB_SUCCESS;
			}
		}
	return	wState;
}

WORD		SetDBValue(char* pcID, long lValue, BOOL bValue)
{
	WORD		wIndex;
	WORD		wState = DB_ERR_DB;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	if (pcID == NULL) return wState;
		
	pthread_mutex_lock(&g_DataStrWriteMutex);
	Trim(cszStrID, pcID, sizeof(cszStrID));
	wIndex = g_pDatabase->GetIndex(cszStrID);
	wState = SetDBValue(wIndex, lValue,bValue);
	pthread_mutex_unlock(&g_DataStrWriteMutex);
	return	wState;
}

//	fans	2006/12/14 06:49¤U¤È
WORD		SetViewDBValue(char* pcID, long lValue)		
{
	WORD		wIndex;
	WORD		wState = DB_ERR_DB;
	char 			cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	if (pcID == NULL) return wState;
	
	Trim(cszStrID, pcID, sizeof(cszStrID));
	wIndex = g_pDatabase->GetIndex(cszStrID);
	//	save modify record!!
	tmModifyRecordSet_Oil	TempRecord;	
	TempRecord.OldValue = GetDBValue(wIndex).lValue;
	TempRecord.NewValue = lValue;
	TempRecord.DBIndex = wIndex;
	GetDate(&TempRecord.date);
	GetTime(&TempRecord.time);
	if (g_pszUserID != NULL && g_pszUserID[0] != '\0')
		strncpy(TempRecord.szID, g_pszUserID, sizeof(TempRecord.szID) - 1);
	else
		strcpy(TempRecord.szID, "NOLOGIN");
	if (TempRecord.OldValue != TempRecord.NewValue)
		CRecordSetControl::GetInstance()->SaveModifyRecord(&TempRecord);	
	
	wState = SetDBValue(wIndex, lValue);


	return	wState;
}

WORD		SetDBValue(WORD	wIndex, long lValue, BOOL bSend)
{
	pthread_mutex_lock(&g_DataWriteMutex);
	
	WORD		wState = DB_SUCCESS;
	BOOL		bChange = FALSE;
	char*		pszID;
	double		dValue;
	float		fValue;
	BYTE		bValue;
	char		cValue;
	WORD		wValue;
	DWORD		dwValue;
	int			iValue;
	short		nValue;
	DBVALUE		dbValue;
	int			nMultiple;
	long    oldValue;
	dbValue = g_pDatabase->Read(wIndex);
	if (dbValue.dwState	!= DB_SUCCESS)	
	{
		pthread_mutex_unlock(&g_DataWriteMutex);
		return DB_ERR_DATA;
	}
	
	if (dbValue.dwDataPath == DATA_PATH_ELECTRIC)
		nMultiple = GetDecimalPowerVal(dbValue.wPrecision);
	else
		nMultiple = 1;
	////printf("Index=%d Value=%ld wMachinePower=%d \n", wIndex, lValue, dbValue.dwDataPath);
	switch(dbValue.DataType.wType)
		{
		case TYPE_BOOL:
			wValue = (WORD)lValue / nMultiple;
			if (dbValue.wData != wValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.wData*nMultiple)+0.5;
				dbValue.wData = wValue;
				wState = (g_pDatabase->Write(wIndex, &wValue)).dwState;
				}
			break;
		case TYPE_BYTE:
			bValue = (WORD)lValue / nMultiple;
			if (dbValue.ucData != bValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.ucData*nMultiple)+0.5;
				dbValue.ucData = bValue;
				wState = (g_pDatabase->Write(wIndex, &bValue)).dwState;
				}
			break;
		case TYPE_WORD:
			wValue = (WORD)lValue / nMultiple;
			if (dbValue.wData != wValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.wData*nMultiple)+0.5;
				dbValue.wData = wValue;
				wState = (g_pDatabase->Write(wIndex, &wValue)).dwState;
				}
			break;
		case TYPE_DWORD:
			dwValue = (DWORD)lValue / nMultiple;
			if (dbValue.dwData != dwValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.dwData*nMultiple)+0.5;
				dbValue.dwData = dwValue;
				wState = (g_pDatabase->Write(wIndex, &dwValue)).dwState;
				}
			break;
		case TYPE_CHAR:
			cValue = (WORD)lValue / nMultiple;
			if (dbValue.cData != cValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.cData*nMultiple)+0.5;
				dbValue.cData = cValue;
				wState = (g_pDatabase->Write(wIndex, &cValue)).dwState;
				}
			break;
		case TYPE_SHORT:
			nValue = (short)lValue / nMultiple;
			if (dbValue.nData != nValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.nData*nMultiple)+0.5;
				dbValue.nData = nValue;
				wState = (g_pDatabase->Write(wIndex, &nValue)).dwState;
				}
			break;
		case TYPE_INT:
			iValue = lValue / nMultiple;
			if (dbValue.lData != iValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.lData*nMultiple)+0.5;
				dbValue.lData = iValue;
				wState = (g_pDatabase->Write(wIndex, &iValue)).dwState;
				}
			break;
		case TYPE_FLOAT:
			nMultiple = GetDecimalPowerVal(dbValue.wPrecision);		//James add 2010/5/28
			fValue = (float)lValue/nMultiple;
			//printf("fValue=%lf lValue=%d nMultiple=%d dbValue.wPrecision=%d \n",fValue,lValue,  nMultiple, dbValue.wPrecision);
			if (dbValue.fData != fValue) 
			{
				bChange = TRUE;
				oldValue = (dbValue.fData*nMultiple)+0.5;
				dbValue.fData = fValue;
				wState = (g_pDatabase->Write(wIndex, &fValue)).dwState;
			}
			break;
		case TYPE_DOUBLE:
			nMultiple = GetDecimalPowerVal(dbValue.wPrecision);		//James add 2010/5/28
			dValue = (double)lValue / nMultiple;
			if (dbValue.dData != dValue) 
				{
				bChange = TRUE;
				oldValue = (dbValue.dData*nMultiple)+0.5;
				dbValue.dData = dValue;
				wState = (g_pDatabase->Write(wIndex, &dValue)).dwState;
				}
			break;
		case TYPE_STRING:
			
			break;
		case TYPE_UNKNOWN:
		default:
			break;
		}
	if ((dbValue.dwDataPath & DATA_PATH_ELECTRIC) && bChange && bSend)
	{
		//if (g_ptasksiem != NULL)
		//	g_ptasksiem->WriteValue(1, &dbValue);
	}
	if ((dbValue.dwDataPath & DATA_PATH_I32) && bChange && bSend)
	{
		pszID	= g_pDatabase->GetString(wIndex);
		//if (g_ptaski32 != NULL)
		//	g_ptaski32->WriteValue(pszID);
	}
	if ((dbValue.dwDataPath & DATA_PATH_HYDRAULIC) && bChange && bSend)
	{
		if (g_ptaskdsp != NULL)
			g_ptaskdsp->WriteValue(COMM_SENDCONTROLINDEX, wIndex);
		if(CtmConfig::GetInstance()->GetCom2Type() == 0)
		{
			if (g_ptaskCom2dsp != NULL)
				g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX, wIndex);
		}
		if (g_ptaskdsp28 != NULL )
		{
			int	nIndex = dbValue.lID;
			g_ptaskdsp28->WriteValue(3, 1, &nIndex);
		}
	}
	if ((dbValue.dwDataPath & DATA_PATH_HYDRAULIC2) && bChange && bSend)
	{
		if(CtmConfig::GetInstance()->GetCom2Type() == 1)
		{
			if (g_ptaskCom2dsp != NULL)
				g_ptaskCom2dsp->WriteValue(COMM_SENDCONTROLINDEX, wIndex);
		}
	}
	if ((dbValue.dwDataPath & DATA_PATH_DSP28) && bChange && bSend)		//James add 2008/3/7 12:43¤U¤È
	{	
		if (g_ptaskdsp28 != NULL )
		{
			int	nIndex = dbValue.lID;
			g_ptaskdsp28->WriteValue(3, 1, &nIndex);
		}
		if (g_ptaskdsp28_N2C != NULL )
		{
			int	nIndex = dbValue.lID;
			g_ptaskdsp28_N2C->WriteValue(3, 1, &nIndex);
		}
		
		if (g_ptaskdsp28_M3 != NULL )
		{
			int	nIndex = dbValue.lID;
			g_ptaskdsp28_M3->WriteValue(3, 1, &nIndex);
		}
		//add by J.Wong 2016/4/14 19:45:54
		if(g_ptaskpicker !=NULL )
		{
			printf("g_ptaskpicker TRUE!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			g_ptmControlServer->Oper_Log()->AddOperLog(wIndex,oldValue,lValue);
			int nIndex=dbValue.lID;
			g_ptaskpicker->WriteValue(3, 1, &nIndex);
		}
	}
	if ((dbValue.dwDataPath & DATA_PATH_AUSTONE) && bChange && bSend)		//James add 2008/3/7 12:43¤U¤È
	{	
		if (g_ptaskdsp28_Austone != NULL )
		{
			int	nIndex = dbValue.lID;
			g_ptaskdsp28_Austone->WriteValue(3, 1, &nIndex);
		}
		if (g_ptaskdsp28_N2C != NULL )
		{
			int	nIndex = dbValue.lID;
			g_ptaskdsp28_N2C->WriteValue(3, 1, &nIndex);
		}
	}
	if ((dbValue.dwDataPath & DATA_PATH_CNC) && bChange && bSend)			//James add 2010/1/19	//Mario debug
	{
		if (g_ptaskCNC != NULL )
		{
			int	nDataID = g_pDatabase->DataIDToIndex(dbValue.lID);
			g_ptaskCNC->WriteValue(nDataID);
		}
	}
	//if ((dbValue.dwDataPath & DATA_PATH_DSP28) && bChange && bSend)		//James add 2008/3/7 12:43¤U¤È
	//	{
	//	if (g_ptaskdsp28 != NULL )
	//		g_ptaskdsp28->WriteValue(1, &dbValue);		
	//	}
	
	pthread_mutex_unlock(&g_DataWriteMutex);
	return wState;
}


double		GetValue(DBVALUE dbValue)
{
	double	dValue = 0;
	
	switch(dbValue.DataType.wType)
		{
		case TYPE_BOOL:
			dValue = dbValue.wData;
			break;
		case TYPE_BYTE:
			dValue = dbValue.ucData;
			break;
		case TYPE_WORD:
			dValue = dbValue.wData;
			break;
		case TYPE_DWORD:
			dValue = dbValue.dwData;
			break;
		case TYPE_CHAR:
			dValue = dbValue.cData;
			break;
		case TYPE_SHORT:
			dValue = dbValue.nData;
			break;
		case TYPE_INT:
			dValue = dbValue.lData;
			break;
		case TYPE_FLOAT:
			dValue = dbValue.fData;
			break;
		case TYPE_DOUBLE:
			dValue = dbValue.dData;
			break;
		case TYPE_STRING:
			
			break;
		case TYPE_UNKNOWN:
		default:
			break;
		}
	return dValue;
}

DBVIEWVALUE	GetIDValue(DWORD dwDataID)						
{
	int		nIndex;
	
	nIndex = g_pDatabase->DataIDToIndex(dwDataID);
	
	return	GetDBValue(nIndex);
}

WORD		SetIDValue(DWORD dwDataID, long lValue)			
{
	int		nIndex;
	
	nIndex = g_pDatabase->DataIDToIndex(dwDataID);
	return	SetDBValue(nIndex, lValue);
}


int			GetFileType(char* pszPath)
{
	static	int	u_nFileType = -1;
	if(u_nFileType == -1)
	{
		DIR*		dir;
		dir = opendir(pszPath);
		if (!dir) 
		{
			g_tmDebugInfo->PrintDebugInfo("=====================no dir \n");
			u_nFileType = 0;
			//mkdir(pszPath, 777);
		}
		else
		{
			g_tmDebugInfo->PrintDebugInfo("=====================yes dir \n");
			u_nFileType = 1;
		}
		closedir(dir);
	}
	return u_nFileType;
}

int		CheckBlockData(CtmRecordFileEx* ptmRecordFile, int	nDataTye, int nBlockID, tmStorageType StorageType, BOOL bRead)
{
	if(ptmRecordFile != NULL)
	{
		//¦pªG²Ä¤@¦¸…{Éó¡A©Î‰ä„ò®ÕJ±K‡ù¤£†Á¡A’â†V®Æ¦s¨ì«ü©wªº„¦„Â¤¤¥h
		if(!ptmRecordFile->CheckData(nDataTye))	
		{
			CtmBlockControl::GetInstance()->SetBlockIndex(nBlockID);
			CtmBlockControl::GetInstance()->SaveAs(StorageType);
			return 1;
		}
		else	//§_ƒh„G«ü©wªº„¦„Â¤¤ŒÝ¨ú†V®Æ
		{
			if(bRead)
			{
				CtmBlockControl::GetInstance()->SetBlockIndex(nBlockID);
				CtmBlockControl::GetInstance()->Read(StorageType);
			}
			return 0;
		}
	}
	return 0;
}

int		CheckFreeBlockData(char* pszIDTableFile, char* pszValueFile, BOOL bRead)
{
	int nResult = 0;
	if(g_pFreeData != NULL)
		nResult = g_pFreeData->LoadIDTable(pszIDTableFile);
	else 
		return 0;
	
	if(pszValueFile != NULL && nResult >= 0)
	{
		int nFd = open(pszValueFile, O_RDWR);
		if(nFd < 0)
		{
			nResult = g_pFreeData->SaveDataToFile(pszValueFile);
		}
		else
		{
			close(nFd);
			if(bRead)
			{
				nResult = g_pFreeData->GetDataFromFile(pszValueFile);
			}
		}
	}
	return nResult;
}

int		SearchIndexFromFreeData(char* pszIDTableFile, char* pszValueFile, int nIndex)
{
	int 	i = 0;
	BOOL	bFlag = FALSE;
	char 	cszDBID[64] = "\0";
	if(g_pDatabase->m_pszSystemDBID[0] == NULL)
	{
		CtmReadData	u_ReadData;
		for (i = 0; i < 256; i++)
		{
			g_pDatabase->m_pszSystemDBID[i] = new char[64];
			memset(g_pDatabase->m_pszSystemDBID[i], 0, 64);
		}
		u_ReadData.OpenFileStr(pszIDTableFile);
		u_ReadData.ReadData(g_pDatabase->m_pszSystemDBID);
		u_ReadData.CloseFileStr();
	}
	else
	{
		i = 0;
		strcpy(cszDBID, g_pDatabase->GetString(nIndex));
		while(g_pDatabase->m_pszSystemDBID[i][0] != '\0')
		{
			if(strcmp(g_pDatabase->m_pszSystemDBID[i++], cszDBID) == 0)
			{
				bFlag = TRUE;
				break;	
			}
		}
		if(!bFlag)
			i = 0;
	}
	
	return i;
}

WORD		SetDBValueM(char* pcID, long lValue, BOOL bValue)
{
	WORD		wIndex;
	WORD		wState = DB_ERR_DB;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	if (pcID == NULL) return wState;
		
	pthread_mutex_lock(&g_DataStrWriteMutex);
	Trim(cszStrID, pcID, sizeof(cszStrID));
	wIndex = g_pDatabase->GetIndex(cszStrID);
	wState = SetDBValueM(wIndex, lValue,bValue);
	pthread_mutex_unlock(&g_DataStrWriteMutex);
	return	wState;
}

WORD		SetDBValueM(WORD	wIndex, long lValue, BOOL bSend)
{
	WORD	wResult = SetDBValue(wIndex, lValue, bSend);
	SendMsg(MSG_SH_SET_DBVALUE, wIndex, bSend, NULL);	//	fans add 2009-12-23 16:29:27
	return 	wResult;
}

WORD		SetIDValueM(DWORD dwDataID, long lValue)			
{
	int		nIndex;
	
	nIndex = g_pDatabase->DataIDToIndex(dwDataID);
	return	SetDBValueM(nIndex, lValue);
}

DBVIEWVALUE				GetDBValueM(char* pcID, BOOL* pbFind)
{
	pthread_mutex_lock(&g_DataReadMutex);
	int		nIndex;
	DBVIEWVALUE	dbViewValue;
	char 		cszStrID[256];
	
	memset(cszStrID, 0, sizeof(cszStrID));
	Trim(cszStrID, pcID, sizeof(cszStrID));
	memset(&dbViewValue, 0, sizeof(dbViewValue));
	if (pcID == NULL || (nIndex = g_pDatabase->GetIndex(cszStrID)) == -1)
	{
		if (pbFind != NULL) *pbFind = FALSE;
		pthread_mutex_unlock(&g_DataReadMutex);
		return dbViewValue;
	}
	if (pbFind != NULL) *pbFind = TRUE;
	dbViewValue = GetDBValueM(nIndex);
	pthread_mutex_unlock(&g_DataReadMutex);
	return	dbViewValue;	
}

DBVIEWVALUE			GetDBValueM(WORD	wIndex)
{
	DBVIEWVALUE	dbViewValue = GetDBValue(wIndex);
	
	SendMsg(MSG_SH_SET_DBVALUE, wIndex, 0, NULL);
	return dbViewValue;
}

DBVIEWVALUE				GetIDValueM(DWORD dwDataID)
{
	int		nIndex;
	
	nIndex = g_pDatabase->DataIDToIndex(dwDataID);
	
	return	GetDBValueM(nIndex);
}

int		SetDB2Info(TMDB2*	tmdb2, int nCount)
{
	int	nSramSize = 0, nSaveSramSize = 0, nIDCount = 0;
	int	nFlag1 = 0, nFlag2 = 0;
	char * pcDefDB = NULL;
	if(tmdb2 != NULL)
	{
		nSramSize 	= 	GetDB2Size(tmdb2, nCount);
		if(pcDefDB == NULL)
			pcDefDB = new char[nSramSize];
		if(g_pcDB2 == NULL)
			g_pcDB2 = new char[nSramSize];
			
		GetDefData(tmdb2, nCount, pcDefDB);
		
		g_nDB2Size	= nSramSize;
		
		CtmSarmRecordFileControl*	pSram;
		CtmStorageFile* 	u_pSramFile = CtmMachineBuild::GetInstance()->BuildStorage(STORAGE_TYPE_SRAM);
			
		nFlag1 	= 	CtmConfig::GetInstance()->GetSramInit();
		nFlag2	=	CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DB2_SRAMFILE);
		
		if (CtmConfig::GetInstance()->GetSramInit() <= 0 || 
		(!CtmSarmRecordFileControl::GetInstance()->CheckData(ID_DB2_SRAMFILE)))
		{
			memcpy(pcDefDB, &nCount, sizeof(nCount));
			memcpy(pcDefDB+sizeof(nCount), &nSramSize, sizeof(nSramSize));
			memcpy(g_pcDB2, pcDefDB, nSramSize);
			pSram->GetInstance()->WriteRecord(ID_DB2_SRAMFILE, 1, (void*)pcDefDB);
			printf("init DB2 reset  nCount=%d  nSramSize=%d \n", nCount, nSramSize);
		}
		else
		{
			pSram->GetInstance()->ReadRecord(ID_DB2_SRAMFILE, 1, (void*)g_pcDB2);
			memcpy(&nIDCount, g_pcDB2, sizeof(nIDCount));
			memcpy(&nSaveSramSize, g_pcDB2+sizeof(nIDCount), sizeof(nSaveSramSize));
			
			printf("init DB2 nIDCount=%d  nCount=%d nSramSize=%d nSaveSramSize=%d \n", nIDCount, nCount,
			nSramSize, nSaveSramSize);
			if(nIDCount > nCount && nSramSize > nSaveSramSize)	
			{
				printf("init DB2 add \n");
				memcpy(g_pcDB2+nSaveSramSize, pcDefDB+nSaveSramSize, nSramSize-nSaveSramSize);
				
				memcpy(g_pcDB2, &nCount, sizeof(nCount));
				memcpy(g_pcDB2+sizeof(nCount), &nSramSize, sizeof(nSramSize));
				pSram->GetInstance()->WriteRecord(ID_DB2_SRAMFILE, 1, (void*)g_pcDB2);
				
			}
			else if(nIDCount == nCount && nSramSize == nSaveSramSize)
			{
				;//printf("init DB2 ok \n");
			}
			else
			{
				
				printf("init DB2 reset \n");
				memcpy(g_pcDB2, pcDefDB, nSramSize);
				memcpy(g_pcDB2, &nCount, sizeof(nCount));
				memcpy(g_pcDB2+sizeof(nCount), &nSramSize, sizeof(nSramSize));
				printf("nSramSize=%d \n", nSramSize);
				pSram->GetInstance()->WriteRecord(ID_DB2_SRAMFILE, 1, (void*)g_pcDB2);
			}
		}
		//printf("nSramSize=%d \n", nSramSize);
	}
	g_ptmdb2 	= tmdb2;
	g_nDB2Count	= nCount;
	if(pcDefDB != NULL)
		delete [] pcDefDB;
	//if(pcDB != NULL)
	//	delete [] pcDB;
}

int		GetDefData(TMDB2*	tmdb2, int nCount, char* pszDefDB)
{
	int	nOffset = 0;
	if(pszDefDB != NULL)
	{
		for(int i = 0; i < nCount; i++)
		{
			if(tmdb2[i].nDataID >= 0)
			{
				switch(tmdb2[i].wDataType)
				{
					case TYPE_BOOL:
					case TYPE_BYTE:
					case TYPE_WORD:
					case TYPE_CHAR:
					case TYPE_SHORT:
						memcpy(pszDefDB+nOffset, tmdb2[i].data.acData, 2);
						nOffset+=2;
						break;
					case TYPE_DWORD:
					case TYPE_INT:
					case TYPE_FLOAT:
						memcpy(pszDefDB+nOffset, tmdb2[i].data.acData, 4);
						nOffset+=4;
						break;
					case TYPE_DOUBLE:
						memcpy(pszDefDB+nOffset, tmdb2[i].data.acData, 8);
						nOffset+=8;
						break;
					case TYPE_UNKNOWN:
						break;
					default:
						break;
				}
			}
		}
	}
	return nOffset;
}

int		GetDB2Size(TMDB2*	tmdb2, int nCount)
{
	int	nResult = 0;
	for(int i = 0; i < nCount; i++)
	{
		if(tmdb2[i].nDataID >= 0)
		{
			tmdb2[i].nOffset = nResult;
			switch(tmdb2[i].wDataType)
			{
				case TYPE_BOOL:
				case TYPE_BYTE:
				case TYPE_WORD:
				case TYPE_CHAR:
				case TYPE_SHORT:
					nResult +=2;
					break;
				case TYPE_DWORD:
				case TYPE_INT:
				case TYPE_FLOAT:
					nResult +=4;
					break;
				case TYPE_DOUBLE:
					nResult +=8;
					break;
				case TYPE_UNKNOWN:
				default:
					break;
			}
		}
	}
	return nResult;
}

DBVIEWVALUE	GetDB2Value(int nDataID, BOOL* pbFind)
{
	DBVIEWVALUE dbViewValue;
	
	WORD	wPrecision = 0;
	int		nMultiple = 1;
	double		dValue;
	float		fValue;
	BYTE		bValue;
	char		cValue;
	WORD		wValue;
	DWORD		dwValue;
	int			iValue;
	short		nValue;
	
	int			nOffset = 0;
	
	memset(&dbViewValue, 0, sizeof(dbViewValue));
	if(g_ptmdb2 != NULL)
	{
		for(int i = 0; i < g_nDB2Count; i++)
		{
			if(g_ptmdb2[i].nDataID == nDataID)
			{
				//memcpy(&data, &g_ptmdb2[i].data, sizeof(data));
				//return g_ptmdb2[i].data;
				wPrecision 	= g_ptmdb2[i].wPrecision;
				nOffset		=g_ptmdb2[i].nOffset;
				switch(g_ptmdb2[i].wDataType)
				{
					case TYPE_BOOL:
						wValue	= *(BOOL *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(wValue*nMultiple);
						break;
					case TYPE_BYTE:
						bValue	= *(BYTE *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(bValue*nMultiple);
						break;
					case TYPE_WORD:
						wValue	= *(WORD *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(wValue*nMultiple);
						break;
					case TYPE_DWORD:
						dwValue	= *(DWORD *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(dwValue*nMultiple);
						break;
					case TYPE_CHAR:
						cValue	= *(char *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(cValue*nMultiple);
						break;
					case TYPE_SHORT:
						nValue	= *(short *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(nValue*nMultiple);
						break;
					case TYPE_INT:
						iValue	= *(int *)(g_pcDB2 + nOffset);
						dbViewValue.lValue = (long long)round(iValue*nMultiple);
						break;
					case TYPE_FLOAT:
						fValue	= *(float *)(g_pcDB2 + nOffset);
						nMultiple = GetDecimalPowerVal(wPrecision);
						dbViewValue.lValue = (long long)round(fValue*nMultiple);
						break;
					case TYPE_DOUBLE:
						dValue	= *(double *)(g_pcDB2 + nOffset);
						nMultiple = GetDecimalPowerVal(wPrecision);
						dbViewValue.lValue = (long long)round(dValue*nMultiple);
						break;
					case TYPE_STRING:
						
						break;
					case TYPE_UNKNOWN:
					default:
						break;
				}
				break;
				if(pbFind != NULL)
					*pbFind = TRUE;
				return dbViewValue;
			}
		}	
	}
	return dbViewValue;
}

int		SetDB2Value(int	nDataID, long lValue,BOOL bSave)
{
	WORD	wPrecision = 0;
	BOOL	bChange = FALSE;
	WORD	wLength	= 0 ;
	int		nMultiple = 0;
	int		nOffset = 0;
	
	double		dValue;
	float		fValue;
	BYTE		bValue;
	char		cValue;
	WORD		wValue;
	DWORD		dwValue;
	int			iValue;
	short		nValue;
	long		lReadValue = 0;
	
	CtmSarmRecordFileControl*	pSram;
	
	if(g_ptmdb2 != NULL)
	{
		for(int i = 0; i < g_nDB2Count; i++)
		{
			if(g_ptmdb2[i].nDataID == nDataID)
			{
				//memcpy(&data, &g_ptmdb2[i].data, sizeof(data));
				//return g_ptmdb2[i].data;
				wPrecision 	= g_ptmdb2[i].wPrecision;
				nOffset	= g_ptmdb2[i].nOffset;
				switch(g_ptmdb2[i].wDataType)
				{
					case TYPE_BOOL:
						wValue 	= (WORD)lValue;
						wLength	= 2;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.cData 			= wValue;
							*(BOOL *)(g_pcDB2 + nOffset)	= (BOOL)wValue;
						}
						break;
					case TYPE_BYTE:
						bValue = (WORD)lValue;
						wLength	= 2;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.ucData = bValue;
							*(BYTE *)(g_pcDB2 + nOffset)	= (BYTE)bValue;
						}
						break;
					case TYPE_WORD:
						wValue = (WORD)lValue;
						wLength	= 2;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.wData = wValue;
							*(WORD *)(g_pcDB2 + nOffset)	= (WORD)wValue;
						}
						break;
					case TYPE_DWORD:
						dwValue = (DWORD)lValue;
						wLength	= 4;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.dwData = dwValue;
							*(DWORD *)(g_pcDB2 + nOffset)	= (DWORD)dwValue;
						}
						break;
					case TYPE_CHAR:
						cValue = (WORD)lValue;
						wLength	= 2;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.cData = cValue;
							*(char *)(g_pcDB2 + nOffset)	= (char)cValue;
						}
						break;
					case TYPE_SHORT:
						nValue = (short)lValue;
						wLength	= 2;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.nData = nValue;
							*(short *)(g_pcDB2 + nOffset)	= (short)nValue;
						}
						break;
					case TYPE_INT:
						iValue = lValue;
						wLength	= 4;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.lData = iValue;
							*(short *)(g_pcDB2 + nOffset)	= (long)lValue;
						}
						break;
					case TYPE_FLOAT:
						nMultiple = GetDecimalPowerVal(g_ptmdb2[i].wPrecision);
						fValue = (float)lValue / nMultiple;
						wLength	= 4;
						//printf("fValue=%lf lValue=%d nMultiple=%d dbValue.wPrecision=%d \n",fValue,lValue,  nMultiple, dbValue.wPrecision);
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.fData = fValue;
							*(float *)(g_pcDB2 + nOffset)	= (float)fValue;
						}
						break;
					case TYPE_DOUBLE:
						nMultiple = GetDecimalPowerVal(g_ptmdb2[i].wPrecision);		//James add 2010/5/28
						dValue = (double)lValue / nMultiple;
						wLength	= 8;
						lReadValue = GetDB2Value(nDataID).lValue;
						if (lReadValue != lValue) 
						{
							bChange = TRUE;
							g_ptmdb2[i].data.dData = dValue;
							*(double *)(g_pcDB2 + nOffset)	= (double)dValue;
						}
						break;
					case TYPE_STRING:
						
						break;
					case TYPE_UNKNOWN:
					default:
						break;
				}
				
				if(bChange)
				{
					//printf("offset=%d wLength=%d \n", g_ptmdb2[i].nOffset, wLength);
					if(bSave)
						pSram->GetInstance()->WriteRecord(ID_DB2_SRAMFILE, 1, g_pcDB2+nOffset, wLength, nOffset);
				}
				break;
			}
		}	
	}
}



