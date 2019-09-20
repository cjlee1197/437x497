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
#define		MAX_TEMPSETSAVED				 100					//alien add  Sramい纗放砞﹚计沮程计秖 
#define     MAX_TEMPRECORDSAVED              100                      //ずい程玂放计秖
                                    
#define     MINUTE                           60
#define     HOUR                             (60*MINUTE)
#define     DAY                              (24*HOUR)
#define     YEAR                             (365*DAY)

//#define		TEMP_TIME_SELECT				 20*MINUTE
#define		TEMP_TIME_COUNTER		         4                               // 纗放计沮丁筳丁
#define     ONEHOURLENGTH                    (sizeof(INT16)*ONEHOURPOINT)    // –兵放–纗惠璶丁
#define     ONEHOURPOINT                     (HOUR/TEMP_TIME_COUNTER)

#define     MAX_TEMPSELECT    				 2*ONEHOURPOINT+100						//程Ω莉1800翴100ňゎ犯


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
    long                    m_lTempNumber;           // 秨诀玂放翴计                              
	long					m_lCounter;			     // 放玂丁筳
	long					m_lColedAlter;		     // 夹非畉蹦栋丁筳璸计
	
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
    static        int       m_nCurveUse;            //癘魁Τ碭兵Ρ絬
    static        UTIME     m_RecentCloseTime;   // Ω╰参闽诀盯
    static        UTIME     m_Time;             // 放秨﹍玂sramい丁
    static		  UTIME		m_NextStartTime;		//琿Ρ絬箇戳秨﹍丁
    static        BOOL      m_bFlashThread;
    static        BOOL      m_bMMcThread;
    static        int       m_nCount;           // sramい玂翴计秖
    static        CtmSram*	m_pSram;
    static        int		m_bNextWarmSaveFlagAddr;
    static        int       m_nTempStartAddr;                                 // Sramノ放							 
    static        int       m_nTempDataMessage;                               // SramセΩ放獺
    static        int       m_nTempDataAddr;                                  // Sramノ放计沮
    static        int       m_nTempSetMessageAddr;                            // Sramノ放砞﹚獺
    static        int       m_nTempSetDataAddr;                               // Sramノ放砞﹚计沮
    
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
    
    long    				m_lColCount;		// 放夹畉栋Ω计
    long					m_lColedCount;		// 放夹非畉竒栋Ω计
    long					m_lColAlter;		// 夹非畉砞﹚蹦妓丁筳
    
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
    UTIME					GetCurveStartTime();				//莉眔放Ρ絬Τ计沮程Ν丁
    int						GetSpaceTime();
    void					SetSpaceTime(int nSpaceTime);
    
    void                    SaveToMMc();  
    void                    Reset();
    void					ResetWarm();							//竚放Ρ絬
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
