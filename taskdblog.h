/*===========================================================================+
|  Class    : TaskDBLog                                                      |
|  Task     : TaskDBLog header file                                          |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Wain                                                           |
|  Version  :                                                                |
|  Creation : 04/15/2007                                                     |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     D_TASKDBLOG
#define     D_TASKDBLOG

#include	"task.h"
#include	"files.h" 
#include	"database.h"
#include    "sram.h"
#include	"simplefile.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define     tmMAX_CH_TEMP	                 64
#define		MAX_TEMPSETSAVED				 100					//alien add  Sram���s�x���ū׳]�w�ƾڳ̤j�ƶq 
#define     MAX_TEMPRECORDSAVED              100                      //���s���̦h�O�s�ū׼ƶq
                                    
#define     MINUTE                           60
#define     HOUR                             (60*MINUTE)
#define     DAY                              (24*HOUR)
#define     YEAR                             (365*DAY)

//#define		TEMP_TIME_SELECT				 20*MINUTE
#define		TEMP_TIME_COUNTER		         4                               // �s�x�ū׼ƾڶ��j�ɶ�
#define     ONEHOURLENGTH                    (sizeof(INT16)*ONEHOURPOINT)    // �C���ūרC�p�ɦs�x�ݭn���Ŷ�
#define     ONEHOURPOINT                     (HOUR/TEMP_TIME_COUNTER)

#define     MAX_TEMPSELECT    				 2*ONEHOURPOINT+100						//�̦h�@�����1800���I�A�h�[100����X


#define		MAXCURVESEGMENT					 50
#define		WARMCURVESEGMENT				 50
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

typedef struct tagTEMPUNITSET
{
    UTIME    	Time;
    INT16      	Temp;
} TEMPUNITSET;

typedef struct tagTEMPSAVESELECT
{
    int      		nNumber;
    TEMPUNITSET    	Select[MAX_TEMPSELECT];
} PACKED TEMPSAVESELECT;

typedef struct tagTEMPSAVEBLOCK
{
    UTIME    StartTime;
    int      nPointNumber;
    int		 nSpaceTime;
    int		 nOffset;				
} TEMPSAVEBLOCK;

typedef struct tagTEMPSAVEDATA
{   
    INT16         SaveTemp[ONEHOURPOINT/3/15][tmMAX_CH_TEMP];
} PACKED TEMPSAVEDATA;

typedef struct tagTEMPSET
{
    UTIME    	Time;
    INT16     	Temp[tmMAX_CH_TEMP];
} TEMPSET;


typedef struct tagTEMPSETSELECT
{
    int         nNumber;
    TEMPUNITSET Select[MAX_TEMPSELECT*2];
} TEMPSETSELECT;

/*===========================================================================+
|           Class declaration - CTaskCursor                                  |
+===========================================================================*/
class CTaskDBLog : public CTask
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskDBLog();
    ~CTaskDBLog();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:	
	WORD                    m_wSaveIndex;
    int                     m_nStartTime;
    long                    m_lTempNumber;           // �}���Z�O�s���ū��I��                              
	long					m_lCounter;			     // �ū׫O�s���j
	long					m_lColedAlter;		     // �зǮt�Ķ����j�p��
	
    TEMPSAVESELECT          m_RealTempCurveLog;
    BOOL					m_bCollsion;
    
    int		                m_nSetTempID[tmMAX_CH_TEMP];
	int		                m_nRealTempID[tmMAX_CH_TEMP];
	
    
    INT16                   m_TempSave[MAX_TEMPRECORDSAVED][tmMAX_CH_TEMP];    // Wain 2007/04/02 add 
    INT16                   m_TempSet[MAX_TEMPRECORDSAVED][tmMAX_CH_TEMP];
    
    int                     m_nOldTempSet;
    pthread_t               m_FlashThread;
    pthread_t               m_MMcThread;
    
    static        int       m_nMaxCurvePointNumber;
    static        int       m_nWarmCurvePointNumber;
    static        int       m_nCurveUse;            //�O�����X�����u
    static        UTIME     m_RecentCloseTime;   // �W���t�������ɶ�
    static        UTIME     m_Time;             // �ū׶}�l�O�s�_sram�����ɶ�
    static		  UTIME		m_NextStartTime;		//�U�@�q���u�w���}�l�ɶ�
    static        BOOL      m_bFlashThread;
    static        BOOL      m_bMMcThread;
    static        int       m_nCount;           // sram���O�s���I���ƶq
    static        CtmSram*	m_pSram;
    static        int		m_bNextWarmSaveFlagAddr;
    static        int       m_nTempStartAddr;                                 // Sram�Ω�s��ūת����a�}							 
    static        int       m_nTempDataMessage;                               // Sram�s�񥻦��ū׫H�����a�}
    static        int       m_nTempDataAddr;                                  // Sram�Ω�s��ū׼ƾڪ��a�}
    static        int       m_nTempSetMessageAddr;                            // Sram�Ω�s��ū׳]�w�ȫH�����a�}
    static        int       m_nTempSetDataAddr;                               // Sram�Ω�s��ū׳]�w�ȼƾڦa�}
    
    static		  BOOL		m_bNextWarmSaveFlag;
  	static		  BOOL		m_bWarmSaveFlag;
  	static		  BOOL		m_bWarmStart;
    static		  UTIME		m_WarmStartTime;
    static		  int		m_nWarmOffset;
    static		  int		m_nNowOffset;
    static		  int		m_nNowSpaceTime;
    static		  int		m_nBeginBlockIndex;
    static        int		m_nEndBlockIndex;
    static		  int		m_nWarmBeginIndex;
    static        int		m_nWarmEndIndex;
    static  TEMPSAVEBLOCK	m_SaveBlockBuff[MAXCURVESEGMENT];
    static  TEMPSAVEBLOCK	m_WarmBlockBuff[WARMCURVESEGMENT];
    //static  CtmRecordFile   *m_pCurveFile;
    static	CSimpleFile		*m_pCurveFile;
public:  
    
    long    				m_lColCount;		// �ū׼Э�t��������
    long					m_lColedCount;		// �ū׼зǮt�w�g��������
    long					m_lColAlter;		// �зǮt�]�w�ļ˶��j
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

    void    				Run();
    void          			GetCurveData(char *cName, UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog, BOOL bWarm);
	void  					GetTempCurveData(char *cName, UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog);
    void					GetWarmCurveData(char *cName, UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog);
    void          			GetRealData(UTIME Time, int nSelectTemp, int TimeSelect, TEMPSAVESELECT *pDataLog, BOOL bWarm);
    void					ReadCurrentData(WORD TempRank, int TimeSelect, TEMPSAVESELECT *pDataLog);
    void           			GetSetData(UTIME Time, int nSelectTemp, int TimeSelect, TEMPSETSELECT *pDataLog);
    void		    		ReadCurrentSetData(WORD TempRank, int TimeSelect, TEMPSETSELECT *pDataLog);
    UTIME                   GetCurrentStartTime(int TimeSelect);
    UTIME					GetWarmStartTime();
    UTIME					GetWarmEndTime();
    UTIME					GetCurveStartTime();				//��o�ūצ��u���ļƾڳ̦��ɶ�
    int						GetSpaceTime();
    void					SetSpaceTime(int nSpaceTime);
    
    void                    SaveToMMc();  
    void                    Reset();
    void					ResetWarm();							//���m�[�Ŧ��u
    static         UTIME    GetNowTime();
    static         UTIME    GetSelectTime(int nHour);
    static         int      GetTempSramSize();
    static         int      GetCurveInUse();
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	void					        FindData();
	void					        Set();
	void					        ColData();	
	void                            SetTemp(UTIME Time);                                
	void                            SaveTempToSram();
	void                            InitTempSavedSram();
	void                            ClearTempHistoryRecord();
	void                            ClearTempDataSram();
	void                            InitTempSet(UTIME Time);
	void							LoadCurveInfo();
		
	static		   void				AddOneBlock(TEMPSAVEBLOCK *BlockBuff, TEMPSAVEBLOCK *pTempBlock,
										int *pBeginBlockIndex, int *pEndBlockIndex);
	static		   int				ReadSavedData(int offset, void *buf, int length);
	static		   int				WriteSavedData(int offset, void *buf, int length);	
	static		   void				SaveToWarmCurve(char *cName);
	static		   void				UMountTread();
	static         void             SaveFileToMMc(); 
	static         UTIME            MakeTime(tmDATETIME *tm);
	static         void			    SaveSramTemp(char *cName);
	static         void             SaveTempToFlash(BOOL bSaveWarm);
	static         void             WriteSram(int nPosition, int Length, BYTE *pSource);
	static         void             ReadSram(int nPosition, int Length, BYTE *pDesti);	
	static         int              GetStartNumber(UTIME Time);
	static         void	            ExistDir(char* pszPath);
};
#endif
