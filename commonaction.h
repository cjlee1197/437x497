/*===========================================================================+
|  Class    : common library                                                 |
|  Task     : common library  header file                                    |
|----------------------------------------------------------------------------|
|  Compile  :G++(GCC)3.2                                                     |
|  Link     :G++(GCC)3.2                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision :           			                                         |
+===========================================================================*/
#ifndef     D_COMMONACTION
#define     D_COMMONACTION

#include	"formview.h"
#include	<stdlib.h>
#include	"curve.h"
#include	"database.h"
#include 	"moldControl.h"

#define     SORT_BY_MOLDORDER     2
#define 	SORT_BY_NAME 		1
#define 	SORT_BY_DATE 		0
#if defined(D_3354)||defined(D_ATOM)
#define 	MAX_MOLD_CNT		1000	//模具可用最大組(本地排序緩存)
#else
#define 	MAX_MOLD_CNT		200
#endif
extern  WORD	OnChangeCommon(CtmWnd* pwndSender, WORD wIDControl);
extern  BOOL	OnCreateCommon(CtmWnd* pwndSender);
extern  void	OnShowCommon(CtmWnd* pwndSender);
extern  WORD 	OnKeyCommon(CtmWnd* pwndSender, WORD wKey);
extern	void	OnUpdateCommon(CtmWnd* pwndSender);
extern	void	OnFastUpdateCommon(CtmWnd** pwnd, int iCount);
extern	void	ControlsUpdate(char* pszControlType, CtmWnd* pwndSender);

extern	int		SetRandom(int iMax);
extern	void	SetRandoms(int* iArray, int iCount, int iMax = 0);
extern	void	SetRandoms(long long* iArray, int iCount, int iMax = 0);

extern	void	SetCurveLimit(CtmWnd* pwndCurve, double dx0, double dx1, double dy, DWORD dwColor, 
							  int iLineIndex, CURVEPOINT* points);
extern	void	SetCurveLine(CtmWnd* pwndCurve, DWORD dwColor, int iLineIndex, CURVEPOINT* points, int iPointNum);
extern	void	SetCurveLine(CtmWnd* pwndCurve, int iLineIndex, CURVEPOINT* points, int iPointNum);

extern	int		GetSpecialControlNum(CtmWnd* pwndSender, char* pszName, char* pszType, CtmWnd** pwnd);
extern	int		GetSpecialControlNum(CtmWnd* pwndSender, int  iTab, char* pszType, CtmWnd** pwnd);
extern	int		GetSpecialControlNum(CtmWnd* pwndSender, char* pszPropertyName, CtmWnd** pwnd);
extern	int		GetSpecialControlDB(CtmWnd* pwndSender, char* pszDB, char* pszType, CtmWnd** pwnd);
extern	int		GetSpecialControlNum(CtmWnd* pwndSender, CtmWnd** pwnd, char* pszName);	//20110921 add

extern	void	DrawTable(CtmWnd* pwndSender, XY* pRow, int iRowNum, XY* pCol, int iColNum);
extern  void	DrawEqualRow(CtmWnd* pwndSender, XY StartPoint, XY EndPoint, int iRowNum);
extern  void	DrawEqualCol(CtmWnd* pwndSender, XY StartPoint, XY EndPoint, int iColNum);

void	ReadCurveFromFile(CtmWnd* pwndCurve, CtmRecordFile*	pCurveFile, CURVEPOINT* Points, 
				 		  int iCurveID, int iLineIndex, DWORD dwColor);	
void	ReadCurveFromData(CtmWnd* pwndCurve, CURVEPOINT* Points, int iLineIndex, DWORD dwColor,
						  char* pszXStartDBID, char* pszXDBID, char* pszYDBID, int iCount,
						  char* pszXEndDBID = NULL);

void	GetControlInfoInForm(CtmWnd* pwndSender, char** pszArayControlName, int iMaxNum);
void	SetSpecialDB(CtmWnd* pwndEdit, int nStartIndex);

void	ControlGif(CtmWnd* pwndGif, long long llValue, long long llMin, long long llMax, WORD wMinFrame, WORD wMaxFrame, WORD wDirection);
////////////////
//	自動清料
///////////////
void	SetInjectToBackUp();
void	SetInjectFromBackUp();
void	SetAutoPurge();
void	SetChargeToBackUp();
void	SetChargeFromBackUp();

////////////////
//	自動調模
///////////////
void	SetMoldToBackUp();
void	SetMoldFromBackUp();
void 	SetAdjust();
extern  char*	CommandChang(char* Gcode, char* f_Old_Command);		//2015/4/21 上午 09:20:33 Mario
extern  char* FindAllRelCoordValue(char* G_Code ,char* CoordStr ,char* ptr ,int CoordMode ,int POINT_NUM);
extern  char* FindAllAbsCoordValue(char* G_Code ,char* CoordStr ,char* ptr ,int CoordMode ,int POINT_NUM);
extern  char* FindAbsCoordValue(char* restr ,char* Command ,int CoordNum ,int CoordMode);
extern  char* DoubleToString(char* restr, double value, int num); 
extern  BOOL	ISValue(char* str);
extern  int	  GetDefinValue(char* restr, char* value, BOOL mode, int prec);
extern  char* PassSpaceFindChar(char* ptr , char ptr1);
extern  char* PassInt(char* ptr);
extern  char* PassSpace(char* ptr);
extern  char*	GetCoordValue(char* restr, char* f_ptr ,int f_ValuePrec);
extern  void	PassMark(char* ptr , char* ptr1);
extern  BOOL	PassMark2(char* ptr , char* ptr1);
extern  int		blk_skip(char* ptr);

//ProgFile
extern	int CNC_Ethernet_Send(int type ,int s_arr ,int len ,void* data);
extern	int CNC_Ethernet_Send(int len ,void* data);
extern  int		UpdateProgToNC(char* FileName);
extern  int		CheckFile(char* FileName);
extern  BOOL	file_exists (char * fileName);
extern  int		Etnernet_File (char * fileName);
extern  int 	Compute_file_md5(const char *file_path, char *md5_str);
extern  int		UpdateTeachPoint(int cd ,int TPNum ,LONG_AXIS Value ,int Pos);
extern  int		UpdateFileToNC(char *full_filename);
//----------------------------------------------------------------------------------
// 以下為排序需要用到的頭及函數
//----------------------------------------------------------------------------------
WORD 		LoadData(DB_MOLDHDR *p_SortModlData,STROAGETYPE m_nDestiMedia,int total_num,WORD SORT_TYPE);
//----------------------------------------------------------------------------------

long long		GetRealPosData();	//Sunny<20070612>
void    		CodeChange(char *pDst, char *pSrc);//Sunny<20070626>
extern		int			g_iPrivilege;			//	權限
extern		char*		g_pszUserID;			//	用戶名稱
extern		BOOL		g_bStart;
extern		BOOL		g_bKeyLockFlag;			//  鍵盤Lock Flag
extern		int			g_nTestPortAry[6];

extern		char		g_szLastFormName[256];  //紀錄上一次頁面 // cjlee 2019/4/30 下午 05:02:41
extern		char		g_szCurrentFormName[256];  //現在頁面

extern 		WORD 		Diag_RomAdd , Diag_RamAdd;
extern		WORD      	Diag_InAdd ,Diag_OutAdd;
// for dsp55 
extern		DWORD		dwDiag_RamAdd;

//	模溫機
extern 		WORD 		Diag_RomAdd2 , Diag_RamAdd2;
extern		WORD      	Diag_InAdd2 ,Diag_OutAdd2;

// for dsp55
extern		DWORD		dwDiag_RamAdd2;
extern		WORD		DataAddress;
extern		WORD		DataAddress2;
extern 		BOOL 		g_WarmStartFlag;
extern 		BOOL 		g_bOPSumPass;
extern		int			g_QualityCurveIndex;	//品質曲線index的標記.
extern	    WORD		g_WndLoginPrivildge; //Sunny<20070402>當前要訪問的畫面要求的權限
extern	    BOOL		g_WndLoginFlag;//當前畫面是否能夠訪問
extern	    int			g_iChannelNO ; //當前通道數

extern		BOOL		g_bAction;
extern		WORD		g_wMMCStatus ;	// For statusbar0.cpp to check if usb/mmc was mounted done.<yuc080118>
extern		WORD		g_wUSBStatus ;
extern		BOOL		g_bAmmeterStatus;
extern		BOOL		g_bMsgBoxFlag;  //Sunny<20080214> 記錄Msgbox跳出時是否key下確認鍵
extern		BOOL		g_bCancelInstall;//Sunny<20080828>
extern		char		g_szLoginRequest[32];//	fans add 2009/5/13 05:56下午
extern		DB_MOLDHDR 	g_MoldHead;
//extern		DB_MOLDHDR 	SortModlData[MAX_MOLD_CNT];

//	fans add 2009-8-10 9:36:55
extern		CIniFile	g_StateIni;

extern	  int							g_nblock_no;								//CNC加工程式總單節	//Mario debug
extern	  CNC_BLOCK_DATA	g_CNC_block_data[4]; // 0:last, 1:current, 2:next, 3:queue		//Mario debug
extern	  int							g_nMetricOrInch_System;		//Mario debug
extern		tmGPSData	g_GPSData;

extern  	char 						g_szProgDirectory[128];//Mario debug
extern		WORD	g_wLoginFlag ;		//wwj	2010/8/19 12:01下午
extern		char	g_szLoginName[32];		//wwj	2010/8/19 12:01下午
extern		char	g_szLogoutName[32];		//wwj	2010/8/19 12:01下午

extern		INETDBLIMIT* 	g_piNetLimit; 		//iNetLimit    Caozz add 2010/8/5 06:11下午
extern		BOOL			g_biNetLimit;
extern		WORD			g_wiNetLimitDBNum;


//New add by Roy↓
extern		int			g_sOperate_FileName;		//給予使用者操作的檔案名稱
//20140613↑
extern		int				g_nFileProcessType;					//判斷目前要呼叫哪個功能的小視窗
//20140623↑
extern			int			g_wTeachPointNo;						//使用者欲查看的教點編號    2014/9/5 M:改為使用者指定的教點編號，(用於刪除.編輯.查看等等)
//20140709↑

//New add by Mario↓
extern		int				g_nTeachPointType;						//判斷目前要呼叫哪個功能的教導點小視窗
//20140910↑
//extern		int				g_UnitType;										//判斷目前的單位型態(0=公制,1=英制)
extern		double		g_Metric_Imperial_Unit ;			//公英制的換算(1 inch英寸 = 25.4 mm毫米)
//20140915  ↑
extern		char 			g_Now_FileName[64];						//（over.txt）每次光標移動皆記下目前所指的檔案
//20140923↑
extern		BOOL			g_Pass;							//判斷是否正確密碼
//20140926
extern		CtmWnd*		Now_Push_Btn;				//紀錄現在按下的按鈕(toolbar)用以使之浮起
//20140930
extern		int 			g_NowRunStatus;						//停止:0		執行:	1			暫停:	2
extern		char 			g_Now_Edit_FileName[192];				//編輯中的檔案
//extern		int				g_bMsgBoxThreeSel;					//記下MAS所選的選項(3種選擇時使用)
//20141007
extern		int			g_MoveCommandMode;						//移動指令的模式
//20141104
extern		int		RobotStatus;				//此畫面的狀態
//2015/3/5 下午 04:54:31
extern		int		Now_HMIStatus;			//實際上的狀態
//2015/3/20 下午 04:19:58
extern		CtmWnd* g_statusbar ;
//2015/5/7 上午 11:28:11
extern		CtmWnd*	g_Last_Tool_Btn;				//紀錄自動模式的上一個執行狀態按鈕指針
//2015/5/13 上午 09:49:59

extern	DWORD	g_dwInetSendCMD;
extern	DWORD	g_dwInetRevCMD;
extern	WORD	g_wInetMsgDataIDCount;
extern	DWORD	g_dwInetMsgDataIDList[512];

int 	cmp_dword( const void *a , const void *b);

extern    char	  *RevertCommand;				//指令還原為對話是視窗時使用
//2015/7/28 上午 08:06:44
extern		int		Sec5Timer;			//Sec5Prompt計時器
//2015/12/30 下午 03:17:40
extern		char  NowCommandType;    //0:No 1:JOG  2:Auto
//2016/1/6 上午 08:26:19
extern		int  Ethernet_Break;
//2016/5/6 上午 10:14:52  
extern		NC_Ethernet_Prog FileEthernetData;
//2016/5/9 下午 04:44:17
extern		short  Toolbar_MenuIndex[3];
extern		Robot_Config  cfg;
extern		int 	Gpio_Input;
extern		int 	KeyMode;

// 精靈模式 快速教導
typedef struct QTEACH_ACTION_P{     //定義
     int Step;
     int Type;
     int Num;
     int P1;
     int P2;
     int P3;
     int P4;
     int P5;
}QTEACH_ACTION_P;
extern		QTEACH_ACTION_P g_QTeach_Action_P[100]; // 快速教導 動作程式暫時存 cjlee add 2019/6/28 上午 08:42:07
extern		BOOL g_Hint_Download; // 提醒下載 0-下載完成 1-需要下載
extern		int g_Pile_Select; // 是否有選擇堆疊 n=第n組堆疊 cjlee add 2019/7/8 下午 03:49:27
extern		BOOL g_bMute; // 按鍵靜音 FALSE-按鍵音 TRUE-靜音 cjlee add 2019/9/12 下午 03:14:41
extern		BOOL g_AutoMode; // 按鍵靜音 FALSE-按鍵音 TRUE-靜音 cjlee add 2019/9/12 下午 03:14:41
extern		int	g_DBVale_497; // Database 數值 示教器 cjlee 2020/4/1 上午 10:50:20
extern		int	g_DBVale_28; // Database 數值 控制器 cjlee 2020/4/1 上午 10:50:17
extern		int g_DBPrecision; // Database 小數點位數 cjlee 2020/4/1 下午 01:41:05
extern		char  gstr_DBValue_497[256]; // Database 數值 cjlee 2020/4/9 下午 03:02:21
extern		char  gstr_DBValue_28[256]; // Database 數值 cjlee 2020/4/9 下午 03:02:21
extern		int  gQTeach_PosLimt[6][2]; // 快速教導 位置點上下限[軸1-5][MIN-0;MAX-1] cjlee 2020/5/25 上午 10:48:53
#endif
