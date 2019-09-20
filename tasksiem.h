/*==========================================================================+
|  Class    : Simotion communication          								|
|  Task     : Simotion communication header file                     		|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : netdigger                                                     |
|  Version  : V1.00                                                         |
|  Creation : 2005/12/23                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#ifndef 	D_DSIEMENS
#define		D_DSIEMENS

#include 	"common.h"
#include	"taskcomm.h"
#include	"ethnet.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define 	SIMD_RESBASE		100		// Response Message Base
#define 	SIMD_ACTUAL			1		// Circle request Actual Values
#define 	SIMD_REFRENCE		2		// Circle request Reference Values
#define 	SIMD_SINGLE			3		// Single request actual/reference
#define 	SIMD_WRITE			4		// Wirte Actual/Reference Valuses
#define 	SIMD_FOCMD			31		// FO command;

#define		DATA_READVALUE		1
#define		DATA_WRITEVALUE		2
#define		DATA_FO				200
#define		MAX_SIMD_REQCOUNT	100
#define		MAX_PARA_REQCOUNT	20
#define		CURVE_NUMBER		27				// 曲線最大數目
#define		CURVE_SAMPLE_NUMBER	20				// 范例曲線數目

//=======
// 西門子曲線
//=======
#define		CURVE_INJECT_SPEEDSET		1			//射出速度設定曲線ID
#define		CURVE_INJECT_PRESSSET		2			//射出壓力設定曲線ID
#define		CURVE_INJECT_SPEEDREAL		3			//射出速度實際曲線ID
#define		CURVE_INJECT_PRESSREAL		4			//射出壓力實際曲線ID
#define		CURVE_CONVER_SPEED			5			//轉換點速度曲線ID
#define		CURVE_CONVER_PRESS			6			//轉換點壓力曲線ID
#define		CURVE_CONVER_POSITOIN		7			//轉換點位置曲線ID
#define		CURVE_HOLDPRESS_SET			8			//保壓設定曲線ID
#define		CURVE_HOLDPRESS_REAL		9			//保壓實際曲線ID
#define		CURVE_CLOSEMOLD_SPEEDSET	10			//關模速度設定曲線ID
#define		CURVE_CLOSEMOLD_SPEEDREAL	11			//關模速度設定曲線ID
#define		CURVE_CLOSEMOLD_PRESSSET	12			//關模速度設定曲線ID
#define		CURVE_CLOSEMOLD_PRESSREAL	13			//關模速度設定曲線ID	
#define		CURVE_CHARGE_SPEEDSET		14			//儲料速度設定曲線ID
#define		CURVE_CHARGE_SPEEDREAL		15			//儲料速度實際曲線ID
#define		CURVE_CHARGE_PRESSSET		16			//儲料壓力設定曲線ID
#define		CURVE_CHARGE_PRESSREAL		17			//儲料壓力實際曲線ID
#define		CURVE_EJECT_SPEEDFREAL		18			//脫模向前速度實際曲線ID
#define		CURVE_EJECT_SPEEDFSET		19			//脫模向前速度設定曲線ID
#define		CURVE_EJECT_PRESSFREAL		20			//脫模向前壓力實際曲線ID
#define		CURVE_EJECT_SPEEDBREAL		21			//脫模向後速度實際曲線ID
#define		CURVE_EJECT_SPEEDBSET		22			//脫模向後速度設定曲線ID
#define		CURVE_EJECT_PRESSBREAL		23			//脫模向後壓力實際曲線ID
#define		CURVE_EJECT_SPEEDREAL		18			//脫模速度實際曲線ID
#define		CURVE_EJECT_SPEEDSET		19			//脫模速度設定曲線ID
#define		CURVE_EJECT_PRESSREAL		20			//脫模壓力實際曲線ID
#define		CURVE_EJECT_POSITIONREAL	21			//脫模位置實際曲線ID
#define		CURVE_EJECT_POSITIONSET		22			//脫模位置設定曲線ID
#define		CURVE_OPENMOLD_SPEEDSET		24			//開模速度設定曲線ID
#define		CURVE_OPENMOLD_SPEEDREAL	25			//開模速度實際曲線ID
#define		CURVE_OPENMOLD_PRESSREAL	26			//開模壓力實際曲線ID
#define		CURVE_PROTMOLD_PRESSREAL	27			//模保壓力實際曲線ID


//=======
//范例曲線
//=======
#define		CURVE_INJECT_SPEEDSAMPLE	(CURVE_NUMBER + 1)	//射出速度曲線范例ID
#define		CURVE_INJECT_PRESSSAMPLE	(CURVE_NUMBER + 2)	//射出壓力曲線范例ID
#define		CURVE_CONVER_SPEEDSAMPLE	(CURVE_NUMBER + 3)	//轉換點速度曲線范例ID
#define 	CURVE_CONVER_PRESSSAMPLE	(CURVE_NUMBER + 4)	//轉換點壓力曲練范例ID
#define		CURVE_CONVER_POSITIONSAMPLE	(CURVE_NUMBER + 5)	//轉換點位置曲線落伍ID
#define		CURVE_CLOSEMOLD_SPEEDSAMPLE	(CURVE_NUMBER + 6)	//關模速度曲線范例ID
#define		CURVE_CLOSEMOLD_PRESSSAMPLE	(CURVE_NUMBER + 7)	//關模壓力曲線范例ID
#define		CURVE_CHARGE_SPEEDSAMPLE	(CURVE_NUMBER + 8)	//儲料速度曲線范例ID
#define		CURVE_CHARGE_PRESSSAMPLE	(CURVE_NUMBER + 9)	//儲料壓力曲線范例ID
#define		CURVE_HOLDPRESS_SAMPLE		(CURVE_NUMBER + 10)	//保壓曲線范例ID
#define		CURVE_PROTMOLD_PRESSSAMPLE	(CURVE_NUMBER + 11)	//模保曲線范例ID
#define		CURVE_OPENMOLD_SPEEDSAMPLE	(CURVE_NUMBER + 12) //開模速度曲線范例ID
#define		CURVE_OPENMOLD_PRESSSAMPLE	(CURVE_NUMBER + 13) //開模壓力曲線范例ID
#define		CURVE_EJECT_SPEEDFSAMPLE	(CURVE_NUMBER + 14) //脫模前進速度曲線范例ID
#define		CURVE_EJECT_SPEEDBSAMPLE	(CURVE_NUMBER + 15) //脫模後退速度曲線范例ID
#define		CURVE_EJECT_PRESSFSAMPLE	(CURVE_NUMBER + 16) //脫模前進壓力曲線范例ID
#define		CURVE_EJECT_PRESSBSAMPLE	(CURVE_NUMBER + 17) //脫模後退壓力曲線范例ID
#define		CURVE_EJECT_SPEEDSAMPLE		(CURVE_NUMBER + 18) //脫模速度曲線范例ID
#define		CURVE_EJECT_PRESSSAMPLE		(CURVE_NUMBER + 19) //脫模壓力曲線范例ID
#define		CURVE_EJECT_POSITIONSAMPLE	(CURVE_NUMBER + 20) //脫模位置曲線范例ID
//=======
//		Communication ID
//=======
#define		COM_VALUE_ID				1
#define		COM_FO_ID					2
#define     COM_CURVE_ID				3
#define     COM_MSG_ID					4
#define     COM_PARA_ID					5
#define     COM_RECIPE_ID				6
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef struct tagSIMDENTRY_VALUE
{
	unsigned long int	dwID;
	union
	{
		unsigned char	ucState;
		unsigned char	ucMode;
	};
	unsigned char		ucDataType;
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
	};
} PACKED SIMD_VALUE;
/*
typedef union tagSIMD_MESSAGE
{
	struct
	{
		int		nCount;
		int		nIndex;
	};
	struct
	{
		BYTE		bMsgType;
		BYTE		bState;
		DWORD		dwTime;
		DWORD		dwDate;
		WORD		wObjNumber;
		union
		{
			char	cData[16];
			struct
			{
				long	lFO_Error;
				long	lFO_ErrorID1;
				long	lFO_ErrorID2;
				long	lFO_ErrorID3;
			};
			long	lAlarmNo;
			DWORD	dwInitEvent;
			DWORD	dwExecFaultType;
			struct
			{
				DWORD	dwPH_InitEvent;
				long	lPH_LogBaseAdrIn;
				long	lPH_LogBaseAdrOut;
				long	lPH_LogDiagAdr;
			};
			struct
			{
				long	lErrorCode;
				short	nSizeRet;
				short	nTypeRet;
				short	nCounterRet;
				short	nCycleRet;
				short	nNoElementsRet;
			};
			WORD		wErrorID;
		};
	};
	struct
        {
        WORD	wID;                                // 故障代碼
        DWORD	dwShotCount;                        // 開模序號
        tmDATE	dateStart;                          // 故障起始日期
        tmTIME	timeStart;                          // 故障起始時間
        tmDATE	dateFixed;                          // 故障排除日期
        tmTIME	timeFixed;                          // 故障排除時間
        };
} PACKED SIMD_MSG;
*/
typedef struct tagSIMD_FO
{
	union
	{
    DWORD	nCommand;
    DWORD 	nState;
	};
}	PACKED SIMD_FO;

typedef void *(* PRSRUN)(void *);

typedef struct tagSIMDHEADER
{
	unsigned char 		acHeader[2];
	unsigned short int	wSize;
	unsigned short int	wType;
	unsigned short int	wCounter;
	union
	{
		unsigned short int	wCounterRet;
		unsigned short int	wCycle;
		unsigned short int	wSequence;
		unsigned short int	wInternalState;
	};
	unsigned short int	wNoElements;
} PACKED SIMDHEADER;

typedef struct tagDATATIME
{
	DWORD	dwDate;
	DWORD	dwTime;
} DATATIME;

typedef struct tagSIMDFOOTER
{
	unsigned char acFooter[2];
} PACKED SIMDFOOTER;

typedef struct tagSIMD_ACTVALUE
{
	int	nCommand;		// 命令
	int	nCycle;			// 循環時間
	int	nCount;			// 數目
	void	*pData;		// 資料
} SIMD_ACTVALUE;

typedef struct tagSIMD_DAYTIME
{
	DWORD	dwTime;
	DWORD	dwDay;
} PACKED SIMD_DAYTIME;

typedef struct tagSIMD_CURVE
{
	DWORD	dwIndex;
	DWORD	dwDistance;
	BYTE	bState;
	BYTE	bDataType;
	double	dStartX;
} PACKED SIMD_CURVE;

typedef struct tagCONFIGDATA
{
	char*	pcID;
	WORD	wConfigValue;
}	CONFIGDATA;

typedef struct tagSIMD_PARADATA
{
	WORD	wParaNo;
	WORD	wSubIndex;
	union	
		{
		BYTE	bState;
		BYTE	bMode;
		};
	BYTE	bDataType;
	union
	{
		BYTE 			abData[8];
		unsigned char 	ucData;
		unsigned short	wData;
		unsigned long	dwData;
		signed char 	cData;
		signed short	nData;
		signed long		lData;
		float 			fData;
		double 			dData;
	};
} PACKED SIMD_PARADATA;

typedef struct tagSIMD_PARA
{
	WORD			wType;
	SIMD_PARADATA	ParaData[MAX_PARA_REQCOUNT];
}	SIMD_PARA;

typedef struct tagSIMD_RECIPEDATA
{
	DWORD		dwID;
	union	
		{
		BYTE	bState;
		BYTE	bMode;
		};
	BYTE	bDataType;
	union
	{
		BYTE 			abData[8];
		unsigned char 	ucData;
		unsigned short	wData;
		unsigned long	dwData;
		signed char 	cData;
		signed short	nData;
		signed long		lData;
		float 			fData;
		double 			dData;
	};
} SIMD_RECIPEDATA;
/*--------------------------------------------------------------------------+
|           Operations                                                      |
+--------------------------------------------------------------------------*/

//extern	CtmRecordFile		g_ErrorFile;
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class  CTaskSiem: public CTaskComm
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskSiem();
    virtual ~CTaskSiem();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/

private:
	static int		m_nValueID;
	static int		m_nFOID;
	static int		m_nMsgID;
	static int		m_nCurveID;
	static int		m_nParaID;
	static int		m_nRecipeID;
	static int 		m_nConnecting;					// 正在連接
	static pthread_t m_ThreadID;

	static DBCURVE	m_Curve[CURVE_NUMBER+10];
	static SIMD_VALUE m_asReqValue[MAX_SIMD_REQCOUNT];              /* storage for user input */

	int				m_nClosedError;					// 關閉錯誤線程標識
	static long		m_lTimerCount;					// 實際值定時
//	static SIMD_MSG	m_Msg;
	static SIMD_PARA m_Para;
	SIMD_FO			m_asFO[MAX_SIMD_REQCOUNT];
	long			m_lUpdateCounter;

	pthread_t		m_ErrorThreadID;	
	sem_t			m_ErrorSem;						// 錯誤線程退出信號

	static pthread_mutex_t	mutex_Msg;					// 消息進程鎖
	static	ETHERNET	m_ethernet;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	static int		m_nCurveState;					// 表格接收狀態
	
    void    	Run();
	static int	ReqValues(int nCount, int nType, int nCycle, UINT32 *pIDList);
	static int	ReqValues(int nCount, int nType, int nCycle, char **pszIDList);
	static int	ReqPara(int nCount, int nType, int nCycle, SIMD_PARA Para);
	static int	ReqRecipe(int nCount, SIMD_RECIPEDATA* pRecipeList);
	static int	WriteValue(int nCount, int nType, SIMD_VALUE *pData);
	static int	WriteValue(int nCount, DBVALUE *pData);
	int			ExeFO(int nCount, int nType, void *pData);
	int			GetStatus(int nIndex);
	DBCURVE*	ReqCurve(int nCount, BYTE nMode, int *pIndexList);
//	static int	SaveMsg(int nCount, SIMD_MSG *pMsg);
	int			GetCurveState()				{ return m_nCurveState; };
	void		SetCurveState(int nState)	{ m_nCurveState = nState; };
	SIMD_PARADATA	GetParaValue(WORD wParaNo, WORD wIndex);
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	void		StopActualData();
	int			Write(int nType, int nLength, void *pData);

	static int	QuickValue(SIMD_VALUE	*pValue, int nCount);
	static int	Reset(int *pnIndex);
	static int	SetTime();
	static void GetActualData();
	static int	SyncFrom();
	static void	SetConfigData();
	static void SetCurve();
	static void	_Open(void *pData);
	static void	ResValueFunc(char *pData, int nLength);
	static void	FOFunc(char *pData, int nLength);
	static void CurveFunc(char *pData, int nLength);
	static void	MsgFunc(char *pData, int nLength);
	static void ParaFunc(char *pData, int nLength);
	static void RecipeFunc(char *pData, int nLength);
	static BOOL FOReady();
	static void InitCore();
};

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
#endif
