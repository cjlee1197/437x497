#ifndef     D__TMTABLE_LIB
#define     D__TMTABLE_LIB

#include	"../../iotable.h"
#include	"../../readdata.h"
//#include	"../../tmconfig.h"
#include	"../../inifile.h"
#include	"../../oil54struct.h"

const	int	CNT_INT_MAX_TABLE_NUM = 256;

class	CtmTableG: public CtmExtentTable
{
	DECLARE_DYNCREATE(CtmTableG)
	public:
		CtmTableG();
		~CtmTableG();
	public:
		WORD	GetTableValue(int nType,int*	pResult,void* pEx = NULL);//	override the CtmDefault funtion
		WORD	GetTableValue(int nType,char* pszID,	void* pEx = NULL);
		
		int		CreateSelf();	//	override the CtmPackClass funtion
		int		FreeSelf();		//	override the CtmPackClass funtion
 	protected:
 		BOOL	GetFileName();
 		void	Init();
 		
 		WORD	GetPBTableValue(WORD wAssign,BOOL* pEx = NULL);
 		WORD	GetPCTableValue(WORD wAssign,BOOL* pEx = NULL);
 		WORD	GetStateTableValue(WORD wAssign);
 		WORD	GetErrotTableValue(int wAssign,int* pResult,void* pEx = NULL);
 		WORD	GetExKey(WORD wIndex); //獲取按鍵交換值 Andy 20081105
 		WORD	GetViewTableName(int* pViewID,char* ViewName);
 		void	GetMoldTempType();
 		WORD	GetMoldTempErrorString(WORD wError2, int nMoldTempIndex);
 		WORD	GetMoldTempPBString(WORD wAssign, int nMoldTempIndex);
 	protected: 		
 		CtmReadData	m_ReadData;
 		char*		m_pszFilePAID;
 		char*		m_pszFilePBID;
 		char*		m_pszFilePCID;
 		char*		m_pszFileStateID;
 		char*		m_pszFileErrorID;	
 		
 		char*		m_pszFileViewPath;//20100401
 		char*		m_pszFileViewID; //Andy 20090903		
 		char*		m_pszFileViewNID; //Andy 20090903	
 		
 		char*		m_pszFileExKeyID;
 		char*		m_pszFileExKeyCode;//獲取按鍵交換值所在文件 Andy 20081105
 		 		
 		char*		m_szPAID[CNT_INT_MAX_TABLE_NUM];
		char*		m_szPBID[4][CNT_INT_MAX_TABLE_NUM];
		char*		m_szPCID[4][CNT_INT_MAX_TABLE_NUM];
		char*		m_szStateID[CNT_INT_MAX_TABLE_NUM];
		char*		m_szErrorID[16][CNT_INT_MAX_TABLE_NUM];
		char*		m_szExKeyID[CNT_INT_MAX_TABLE_NUM];//獲取按鍵交換值所關聯在ID Andy 20081105
		char*		m_szViewName[CNT_INT_MAX_TABLE_NUM];//Andy 20090903
		
		char*		m_pszFileStateValue;
		char*		m_pszFileErrorValue;

 		char*		m_pszFileActLedValue;	 /*ZHBA 2014-2-18 */
 		char*		m_pszFileOperConf; /*ZHBA 2014-2-18 */
 		char*		m_pszFileOperLed;  //20150513
		int			m_ActLedValue[CNT_INT_MAX_TABLE_NUM]; /*ZHBA 2014-2-18 */
		int			m_OperConf[CNT_INT_MAX_TABLE_NUM];/*ZHBA 2014-2-18 */
		int			m_OperLed[CNT_INT_MAX_TABLE_NUM];

		int			m_StateValue[CNT_INT_MAX_TABLE_NUM];
		int			m_ErrorValue[16][CNT_INT_MAX_TABLE_NUM];
		int			m_nExKeyValue[CNT_INT_MAX_TABLE_NUM][10];//獲取按鍵交換值所關聯在CODE Andy 20081105
		int			m_ViewID[CNT_INT_MAX_TABLE_NUM];//Andy 20090903
		
		char*		m_szFuncDefine[CNT_INT_MAX_TABLE_NUM];//Andy 20090908 記錄stateconfig.ini中的define;
		
		WORD	  m_wMldTmpIndex[20];
};


#ifndef	D_BEOBJECT_CTMTABLEG
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

