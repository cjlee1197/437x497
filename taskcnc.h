/*==========================================================================+
|  Class    : CNC communication          									|
|  Task     : CNC communication file                     					|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : James		                                                    |
|  Version  : V1.00                                                         |
|  Creation : 2009/12/23                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#ifndef		D_DTASKCNC
#define		D_DTASKCNC

#include	"common.h"
#include	"taskcomm.h"
#include	"libCNCComm.h"
#include 	"strlist.h"
/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		PARAMTYPE_NUB			5
#define		PARAMTYPE_COUNT			2
#define		INITSEND_NUB			2
#define		INITSEND_COUNT			2
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/
typedef void *(* PRSRUN)(void *);

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class  CTaskCNC: public CTaskComm
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskCNC();
    virtual ~CTaskCNC();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
private:
	static 	int			m_nID;
			long		m_lUpdateCounter;
//	static 	pthread_t	m_CNCThreadID;  mario mark 2015/12/28 下午 03:14:56
	static	int			m_nParamID[PARAMTYPE_NUB][PARAMTYPE_COUNT];		
	static	int			m_nInitSendID[INITSEND_NUB][INITSEND_COUNT];		//for Init
	static	int			m_RxSize;
	static	int			m_TxSize;
	static	int			m_ReadStartOffset;
	static	int			m_ReadEndOffset;

			pthread_t	m_RxEndPointThread;	// 接收EndPoint線程ID

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	void    Run();
	int		GetStatus();
	int		Write(int nType, int nLength, void *pData);
	int		Read(int nType,	int nLength, void *pData);
	int		WriteValue(int nType, int nElements, char* pszIDs[], void* pEx = NULL);
	int		WriteValue(int nType, int nElements, int pIndexs[], void* pEx = NULL);
	int		WriteValue(int nType, void* pInputData, int nSize);	
	int		WriteValue(int nDataID);
	int		WriteValue(char *pszID);
	int		WriteValue(void* pInputData, int nOffset, int nLength);				//James add for test 2009/12/31
	void	WriteValue(long shmDataAddr, void* setDataAddr, int nLength);	//Mario add 2015/3/11 上午 11:10:51
	int		ReadValue(void* pInputData, int nOffset, int nLength);			//James add for test 2009/12/31

	BOOL/*void*/	ReadOperationFile(char *pszFileName, BOOL bFileContentModify = FALSE,BOOL bWholeDirectory = FALSE);	//Sunny<20100428>
public:
	int		FindParamType(char *pszID);
	int		FindParamType(int nDataID);
	int		Init();
	void	SaveParaToFile();//Sunny<20100630>
	void	Alarm_Control();//Sunny<20100714>
	void	Motion_DB_Modify();
	void	ResetQueue(void);
	void	BuildQueue(void);
	void	ReadQueue(void);
	void	ResetQueue_MDI(void);
	void	BuildQueue_MDI(void);
	void	ReadQueue_MDI(void);
	int		SaveCurveScreen(WORD wViewTop,WORD ViewBottom);
	
	int		AddTrackPoint(TRACKPOINT*	pTrackPoint, int nIndex=0);
	void	GetAndDrawTrackPoint(CtmWnd* pwndSender, WORD wXStart, WORD wYStart, WORD wLength, WORD wHeight, \
								 WORD wCoordinateMode=(TRACK_CURVE_XY|TRACK_CURVE_NORMAL));

	void	DrawTrackPoint(CtmWnd* pwndSender,double dPoint_X,double dPoint_Y,unsigned short Color,BOOL bFirstPointFlag=FALSE,WORD wCoordinateMode=0);//wCoordinateMode:0-XY 1-XZ 2-YZ
	int		FreeTrackPoint();
	void	QueryCurrentCommand(int offset);
	void	MetricOrInch_System_Control();
	
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	static	void	ResValueFunc(char *pData, int nLength);
	static	void	SaveTrackEndPoint();
};

extern 	CNC_TXDATA		g_TxCNCData;
extern 	CNC_RXDATA*		g_pRxCNCData;
extern 	TP_TABLE			g_TeachPoint;

extern  char 			u_build_queue;

extern  TRACKPOINT*		ptmTrackPoint_Head;
extern  TRACKPOINT*		ptmTrackPoint_Temp;
extern  LONG_AXIS		tmTrackPoint_Max,tmTrackPoint_Min;

extern  CtmStringList	g_OpFileStringList;

extern  WORD			g_wCurve_Horizontal_Axis, g_wCurve_Vertical_Axis;
extern  WORD			g_wCurve_Point_Save_Cnt;
extern  BOOL			g_bCurve_Point_Save_Flag;
extern  long			g_lCNC_Current_block_no_Old;

extern  TRACKPOINT		g_TrackPoint_EndPoint;

//教導點相關
void	LoadTeachPoint();
void	CreateTeachPoint();
void	UpdateTeachPoint();
int		GetTeachPointCnt(int	f_CooreSys = 0);
BOOL	GetTechPointValue(LONG_AXIS *f_GetTeachPointValue ,int *f_GetTeachPointPos ,int	f_TeachPointNum ,int	f_CooreSys = 0);
BOOL	EditTeachPoint(int	f_TeachPointNum ,LONG_AXIS f_SetTeachPointValue ,int f_GetTeachPointPos ,int	f_CooreSys = 0);
BOOL	AddTeachPoint(LONG_AXIS f_SetTeachPointValue ,int f_GetTeachPointPos ,int	f_CooreSys = 0);
BOOL	DeleteTeachPoint(int	f_TeachPointNum ,int	f_CooreSys = 0);
BOOL	DelAllTeachPoint(int	f_CooreSys = 0);
int		EtherNet_WriteValue(int type ,int s_arr ,int len ,void* data);

int	ReadRobotData();

#endif
