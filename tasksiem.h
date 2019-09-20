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
#define		CURVE_NUMBER		27				// ���u�̤j�ƥ�
#define		CURVE_SAMPLE_NUMBER	20				// �S�Ҧ��u�ƥ�

//=======
// ����l���u
//=======
#define		CURVE_INJECT_SPEEDSET		1			//�g�X�t�׳]�w���uID
#define		CURVE_INJECT_PRESSSET		2			//�g�X���O�]�w���uID
#define		CURVE_INJECT_SPEEDREAL		3			//�g�X�t�׹�ڦ��uID
#define		CURVE_INJECT_PRESSREAL		4			//�g�X���O��ڦ��uID
#define		CURVE_CONVER_SPEED			5			//�ഫ�I�t�צ��uID
#define		CURVE_CONVER_PRESS			6			//�ഫ�I���O���uID
#define		CURVE_CONVER_POSITOIN		7			//�ഫ�I��m���uID
#define		CURVE_HOLDPRESS_SET			8			//�O���]�w���uID
#define		CURVE_HOLDPRESS_REAL		9			//�O����ڦ��uID
#define		CURVE_CLOSEMOLD_SPEEDSET	10			//���ҳt�׳]�w���uID
#define		CURVE_CLOSEMOLD_SPEEDREAL	11			//���ҳt�׳]�w���uID
#define		CURVE_CLOSEMOLD_PRESSSET	12			//���ҳt�׳]�w���uID
#define		CURVE_CLOSEMOLD_PRESSREAL	13			//���ҳt�׳]�w���uID	
#define		CURVE_CHARGE_SPEEDSET		14			//�x�Ƴt�׳]�w���uID
#define		CURVE_CHARGE_SPEEDREAL		15			//�x�Ƴt�׹�ڦ��uID
#define		CURVE_CHARGE_PRESSSET		16			//�x�����O�]�w���uID
#define		CURVE_CHARGE_PRESSREAL		17			//�x�����O��ڦ��uID
#define		CURVE_EJECT_SPEEDFREAL		18			//��ҦV�e�t�׹�ڦ��uID
#define		CURVE_EJECT_SPEEDFSET		19			//��ҦV�e�t�׳]�w���uID
#define		CURVE_EJECT_PRESSFREAL		20			//��ҦV�e���O��ڦ��uID
#define		CURVE_EJECT_SPEEDBREAL		21			//��ҦV��t�׹�ڦ��uID
#define		CURVE_EJECT_SPEEDBSET		22			//��ҦV��t�׳]�w���uID
#define		CURVE_EJECT_PRESSBREAL		23			//��ҦV�����O��ڦ��uID
#define		CURVE_EJECT_SPEEDREAL		18			//��ҳt�׹�ڦ��uID
#define		CURVE_EJECT_SPEEDSET		19			//��ҳt�׳]�w���uID
#define		CURVE_EJECT_PRESSREAL		20			//������O��ڦ��uID
#define		CURVE_EJECT_POSITIONREAL	21			//��Ҧ�m��ڦ��uID
#define		CURVE_EJECT_POSITIONSET		22			//��Ҧ�m�]�w���uID
#define		CURVE_OPENMOLD_SPEEDSET		24			//�}�ҳt�׳]�w���uID
#define		CURVE_OPENMOLD_SPEEDREAL	25			//�}�ҳt�׹�ڦ��uID
#define		CURVE_OPENMOLD_PRESSREAL	26			//�}�����O��ڦ��uID
#define		CURVE_PROTMOLD_PRESSREAL	27			//�ҫO���O��ڦ��uID


//=======
//�S�Ҧ��u
//=======
#define		CURVE_INJECT_SPEEDSAMPLE	(CURVE_NUMBER + 1)	//�g�X�t�צ��u�S��ID
#define		CURVE_INJECT_PRESSSAMPLE	(CURVE_NUMBER + 2)	//�g�X���O���u�S��ID
#define		CURVE_CONVER_SPEEDSAMPLE	(CURVE_NUMBER + 3)	//�ഫ�I�t�צ��u�S��ID
#define 	CURVE_CONVER_PRESSSAMPLE	(CURVE_NUMBER + 4)	//�ഫ�I���O���m�S��ID
#define		CURVE_CONVER_POSITIONSAMPLE	(CURVE_NUMBER + 5)	//�ഫ�I��m���u����ID
#define		CURVE_CLOSEMOLD_SPEEDSAMPLE	(CURVE_NUMBER + 6)	//���ҳt�צ��u�S��ID
#define		CURVE_CLOSEMOLD_PRESSSAMPLE	(CURVE_NUMBER + 7)	//�������O���u�S��ID
#define		CURVE_CHARGE_SPEEDSAMPLE	(CURVE_NUMBER + 8)	//�x�Ƴt�צ��u�S��ID
#define		CURVE_CHARGE_PRESSSAMPLE	(CURVE_NUMBER + 9)	//�x�����O���u�S��ID
#define		CURVE_HOLDPRESS_SAMPLE		(CURVE_NUMBER + 10)	//�O�����u�S��ID
#define		CURVE_PROTMOLD_PRESSSAMPLE	(CURVE_NUMBER + 11)	//�ҫO���u�S��ID
#define		CURVE_OPENMOLD_SPEEDSAMPLE	(CURVE_NUMBER + 12) //�}�ҳt�צ��u�S��ID
#define		CURVE_OPENMOLD_PRESSSAMPLE	(CURVE_NUMBER + 13) //�}�����O���u�S��ID
#define		CURVE_EJECT_SPEEDFSAMPLE	(CURVE_NUMBER + 14) //��ҫe�i�t�צ��u�S��ID
#define		CURVE_EJECT_SPEEDBSAMPLE	(CURVE_NUMBER + 15) //��ҫ�h�t�צ��u�S��ID
#define		CURVE_EJECT_PRESSFSAMPLE	(CURVE_NUMBER + 16) //��ҫe�i���O���u�S��ID
#define		CURVE_EJECT_PRESSBSAMPLE	(CURVE_NUMBER + 17) //��ҫ�h���O���u�S��ID
#define		CURVE_EJECT_SPEEDSAMPLE		(CURVE_NUMBER + 18) //��ҳt�צ��u�S��ID
#define		CURVE_EJECT_PRESSSAMPLE		(CURVE_NUMBER + 19) //������O���u�S��ID
#define		CURVE_EJECT_POSITIONSAMPLE	(CURVE_NUMBER + 20) //��Ҧ�m���u�S��ID
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
        WORD	wID;                                // �G�٥N�X
        DWORD	dwShotCount;                        // �}�ҧǸ�
        tmDATE	dateStart;                          // �G�ٰ_�l���
        tmTIME	timeStart;                          // �G�ٰ_�l�ɶ�
        tmDATE	dateFixed;                          // �G�ٱư����
        tmTIME	timeFixed;                          // �G�ٱư��ɶ�
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
	int	nCommand;		// �R�O
	int	nCycle;			// �`���ɶ�
	int	nCount;			// �ƥ�
	void	*pData;		// ���
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
	static int 		m_nConnecting;					// ���b�s��
	static pthread_t m_ThreadID;

	static DBCURVE	m_Curve[CURVE_NUMBER+10];
	static SIMD_VALUE m_asReqValue[MAX_SIMD_REQCOUNT];              /* storage for user input */

	int				m_nClosedError;					// �������~�u�{����
	static long		m_lTimerCount;					// ��ڭȩw��
//	static SIMD_MSG	m_Msg;
	static SIMD_PARA m_Para;
	SIMD_FO			m_asFO[MAX_SIMD_REQCOUNT];
	long			m_lUpdateCounter;

	pthread_t		m_ErrorThreadID;	
	sem_t			m_ErrorSem;						// ���~�u�{�h�X�H��

	static pthread_mutex_t	mutex_Msg;					// �����i�{��
	static	ETHERNET	m_ethernet;
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	static int		m_nCurveState;					// ��汵�����A
	
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
