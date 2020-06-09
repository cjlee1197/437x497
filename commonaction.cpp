/*===========================================================================+
|  Class    : common library                                                 |
|  Task     : common library  action		                                 |
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

#include	"commonaction.h"
#include	"main.h"
#include    "utils.h"
#include	"selvat_ancestor.h"
#include	"cgif.h"
#include "md5.h"
#include	"imagebox.h"
#define READ_DATA_SIZE  1024  
#define MD5_SIZE        16  
#define MD5_STR_LEN     (MD5_SIZE * 2)

#ifdef	D_DEMO

int		g_iPrivilege = 0;			//全局的權限,第一次登錄時用戶權限,登陸後,永遠不會被改變,除非重新登錄程式			//全局的權限,第一次登錄時用戶權限,登陸後,永遠不會被改變,除非重新登錄程式
#else
int		g_iPrivilege = 0;			//全局的權限,第一次登錄時用戶權限,登陸後,永遠不會被改變,除非重新登錄程式			//全局的權限,第一次登錄時用戶權限,登陸後,永遠不會被改變,除非重新登錄程式
#endif


char		g_szLastFormName[256];  //紀錄上一次頁面 // cjlee 2019/4/30 下午 05:02:41
char		g_szCurrentFormName[256];

#define		COORD			0			//關節座標
#define		JOINT			1			//卡氏座標
#define		MAX_COMMAND_LEN 256
char*	g_pszUserID = NULL;
WORD	g_WndLoginPrivildge=0;		//Sunny<20070402>當前要訪問的畫面要求的權限
BOOL	g_bStart = TRUE;			//	first login
BOOL	g_bKeyLockFlag = TRUE;		//  第一次key是鎖起來

int		g_nTestPortAry[6];
WORD	Diag_RomAdd = 0;
WORD	Diag_RamAdd = 0;
// for dsp55
DWORD	dwDiag_RamAdd =0;
WORD	Diag_InAdd  = 0;              
WORD	Diag_OutAdd = 0;

//	模溫機
WORD	Diag_RomAdd2 = 0;
WORD	Diag_RamAdd2 = 0;
//for dsp55
DWORD	dwDiag_RamAdd2 =0;
WORD	Diag_InAdd2  = 0;              
WORD	Diag_OutAdd2 = 0;

PRODUCTSTATUS*		g_pProductStatus ;
WORD				g_wProductStatus ;
WORD				g_wCurrentStatus ;

BADPRODUCTINFO*		g_pBadProductInfo ;
WORD				g_wBadProductInfoNum;

WORD	DataAddress = 0;
WORD	DataAddress2= 0;
int		g_QualityCurveIndex = 0;	//品質曲線index的標記.
BOOL	g_WndLoginFlag =TRUE;		//當前畫面是否能夠訪問
BOOL    g_bOPSumPass = FALSE;           //st031701
BOOL	g_WarmStartFlag;				//Gwei 2005/5/20　給診斷判斷用，重送顯示需求值

BOOL	g_bMsgBoxFlag = FALSE;  //Sunny<20080214> 記錄Msgbox跳出時是否key下確認鍵
BOOL	g_bAction = TRUE;		//	2007/8/6 02:08下午 fans 是否允許動作按鍵發生
WORD	g_wMMCStatus = 0;		// James add 2008/3/26 06:47下午
WORD	g_wUSBStatus = 0;		// James add 2009/3/18 10:28上午
BOOL	g_bCancelInstall =FALSE;//Sunny<20080828>
BOOL	g_bAmmeterStatus;

//	fans add 2009/5/13 05:55下午
char	g_szLoginRequest[32];
DB_MOLDHDR g_MoldHead;
BOOL	g_biNetStatus = FALSE;
WORD	g_wiNetStatus 			= 0;
DB_PRODMANAGE	g_ProManage;

USERPRODUCTS	g_UserProductInfo;

int							g_nblock_no;										//CNC加工程式總單節	//Mario debug
WORD	g_wLoginFlag = 0;		//wwj	2010/8/19 12:01下午
char	g_szLoginName[32];		//wwj	2010/8/19 12:01下午
CNC_BLOCK_DATA	g_CNC_block_data[4]; // 0:last, 1:current, 2:next, 3:queue	//Mario debug
char 						g_szProgDirectory[128]			="\0";//Mario debug
char	g_szLogoutName[32];		//wwj	2010/8/19 12:01下午
int							g_nMetricOrInch_System			= -1;		//Mario debug


INETDBLIMIT* 	g_piNetLimit = NULL; 		
BOOL			g_biNetLimit = 	FALSE;
WORD			g_wiNetLimitDBNum = 0;

WORD			g_bMsgFormKey = 0;

INET_PUMPINFO	g_iNetPumpInfo;

WORD				g_wControl = 0;			//控制Flag  0 = 正常， 1 = 監測，2 = 控制

BOOL	g_biNetControlPlan = FALSE;

CIniFile			g_StateIni;	//	fans add 2009-8-10 9:26:23
tmGPSData			g_GPSData;	//	fans add 2009-12-22 15:48:34

DWORD		g_dwInetSendCMD = 0;
DWORD		g_dwInetRevCMD = 0;

WORD	 			g_wInetMsgDataIDCount = 0;
DWORD				g_dwInetMsgDataIDList[512] = {0};

//New add by Roy↓
int			g_sOperate_FileName= 0;		//給予使用者操作的檔案名稱
//20140613↑
int				g_nFileProcessType			=0;				//判斷目前要呼叫哪個功能的小視窗
//20140623↑
int			g_wTeachPointNo					=0;				//使用者欲查看的教導點編號   2014/9/5 M:改為使用者指定的教點編號，(用於刪除.編輯.查看等等)，原功能亦在
//20140709↑

//New add by Mario↓
int				g_nTeachPointType				=0;				//判斷目前要呼叫哪個功能的教導點小視窗
//20140910↑
double		g_Metric_Imperial_Unit 	=25.4;				//公英制的換算(1 inch英寸 = 25.4 mm毫米)
//20140915  ↑
char 			g_Now_FileName[64]			= "\0";				//（over.txt）每次光標移動皆記下目前所指的檔案
//20140923↑
BOOL			g_Pass									=	FALSE;			//判斷是否輸入正確密碼
//20140926
CtmWnd*		Now_Push_Btn						=NULL;				//紀錄現在按下的按鈕(toolbar)用以使之浮起
//20140930
int 			g_NowRunStatus					=0;						//
char 			g_Now_Edit_FileName[192] = "\0";				//編輯中的檔案路徑名稱
//20141007
int			g_MoveCommandMode				=0;							//移動指令的模式
//20141104
int		RobotStatus=0;						//畫面預設的HMI狀態，可能會隨目前工作狀態做限制		
//2015/3/20 下午 04:20:05	
int		Now_HMIStatus=0;					//實際上的HMI狀態。
//2015/3/5 下午 04:54:31
CtmWnd* g_statusbar = NULL;			//用以呼叫statusbar來update(一致醮典  一致原點)
//2015/5/7 上午 11:28:17
CtmWnd*		g_Last_Tool_Btn = NULL;				//紀錄自動模式的上一個執行狀態按鈕指針
//2015/5/13 上午 09:51:34

char	  *RevertCommand = NULL;				//指令還原為對話是視窗時使用
//2015/7/28 上午 08:07:01
int		Sec5Timer=-1;			//Sec5Prompt計時器
//2015/12/30 下午 03:17:40
char  NowCommandType=0;    //0:No 1:JOG  2:Auto
//2016/1/6 上午 08:26:19
int  	Ethernet_Break=-1;
//2016/5/6 上午 10:14:52  
NC_Ethernet_Prog FileEthernetData;
short Toolbar_MenuIndex[3]={0,0,0};
Robot_Config  cfg;
int 			Gpio_Input=0xFFFF;
int 			KeyMode=-1;
QTEACH_ACTION_P g_QTeach_Action_P[100]; // 快速教導 動作程式暫時存 cjlee add 2019/6/28 上午 08:42:07
BOOL g_Hint_Download; // 提醒下載 0-下載完成 1-需要下載 cjlee add 2019/7/1 上午 10:50:38
int g_Pile_Select; // 是否有選擇堆疊 n=第n組堆疊 cjlee add 2019/7/8 下午 03:49:27
BOOL g_bMute=FALSE; // 按鍵靜音 FALSE-按鍵音 TRUE-靜音 cjlee add 2019/9/12 下午 03:14:41
BOOL g_AutoMode=FALSE; // 全自動 Flag cjlee add 2019/12/11 下午 04:05:41
int	g_DBVale_497=0; // Database 數值 示教器 cjlee 2020/4/1 上午 10:50:20
int	g_DBVale_28=0; // Database 數值 控制器 cjlee 2020/4/1 上午 10:50:17
int g_DBPrecision=0; // Database 小數點位數 cjlee 2020/4/1 下午 01:41:05
char  gstr_DBValue_497[256] = "\0"; // Database 數值 cjlee 2020/4/9 下午 03:02:21
char  gstr_DBValue_28[256] = "\0"; // Database 數值 cjlee 2020/4/9 下午 03:02:21
extern		int  gQTeach_PosLimt[6][2] = {0}; // 快速教導 位置點上下限[軸1-5][MIN-0;MAX-1] cjlee 2020/5/25 上午 10:48:53
/*---------------------------------------------------------------------------+
|  Function :  OnKeyCommon   	                                             |
|  Task     :  View 標準OnChange事件                                         |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD	OnChangeCommon(CtmWnd* pwndSender, WORD wIDControl)
{
	CtmWnd* pwndTemp = NULL;
	char pszdb[256];
	WORD	precision = 0;
	long long	value = 0;
			
	pwndTemp = pwndSender->FindControlFromTab(wIDControl);
	
	if (pwndTemp != NULL)
	{
		if (pwndTemp->Is("CtmEditX1"))
		{
			
			pwndTemp->GetPropValueT("dbid0", pszdb, sizeof(value));
			pwndTemp->GetPropValueT("value", &value, sizeof(value));
			if (pszdb != NULL && pszdb[0] != '\0') SetDBValue(pszdb, value);
		}
	}

	return wIDControl;
}

BOOL	OnCreateCommon(CtmWnd* pwndSender)
{
	return TRUE;
}

void	OnShowCommon(CtmWnd* pwndSender)
{
	////cout << "libcommon.so OnShowA" << endl;
	return;
}

/*---------------------------------------------------------------------------+
|  Function :  OnKeyCommon   	                                             |
|  Task     :  View 標準OnKey 事件                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
WORD OnKeyCommon(CtmWnd* pwndSender, WORD wKey)
{
	if(pwndSender->Is("CtmFormView")) return ((CtmFormView*)pwndSender)->OnKey1(wKey);
	else return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|  Function :  OnUpdateCommon	                                             |
|  Task     :  View 標準Update事件                                           |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnUpdateCommon(CtmWnd* pwndSender)
{
	char	szDBID[256];
	int		iTabOrder = -1;
	WORD	wiType;
	for (int i = 0; i < pwndSender->GetControlCount(); i++)
	{
		if (pwndSender->Controls[i]->Is("CtmEditX1"))
		{
			pwndSender->Controls[i]->GetPropValueT("dbid0", szDBID, sizeof(szDBID));
			iTabOrder = -1;
			pwndSender->Controls[i]->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
			pwndSender->Controls[i]->GetPropValueT("wtype", &wiType, sizeof(wiType));
			
			if ((szDBID[0] != '\0') && (iTabOrder >= 0) && (wiType & tmWS_SHOW))
				pwndSender->Controls[i]->Update();			
		}
	}
}

/*---------------------------------------------------------------------------+
|  Function :  OnFastUpdateCommon                                            |
|  Task     :  即時快速update某類元件                                        |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	OnFastUpdateCommon(CtmWnd** pwnd, int iCount)
{
	char	sz[256];
	memset(sz,0,sizeof(sz));
	int		nTab = -2;
	BOOL    bVisible=FALSE;
	
	for (int i = 0; i < iCount; i++)
		if (pwnd[i] != NULL)  
		{
			pwnd[i]->GetPropValueT("dbid0", sz, sizeof(sz));
			pwnd[i]->GetPropValueT("taborder", &nTab);
			pwnd[i]->GetPropValueT("visible", &bVisible);
			if (sz[0] != '\0' && nTab < 0&& bVisible)
				pwnd[i]->Update();
		}
}

/*---------------------------------------------------------------------------+
|  Function :  ControlsUpdate                                                |
|  Task     :  刷新指定窗体上的指定的元件                                    |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ControlsUpdate(char* pszControlType, CtmWnd* pwndSender)
{
	if (pszControlType != NULL)
	{
		for (int i = 0; i < pwndSender->GetControlCount(); i++)
		{
			if (pwndSender->Controls[i]->Is(pszControlType))
				pwndSender->Controls[i]->Update();
		}
	}
	else
	{
		for (int i = 0; i < pwndSender->GetControlCount(); i++)		
			pwndSender->Controls[i]->Update();
	}
}

/*---------------------------------------------------------------------------+
|  Function :  SetRandom                                                     |
|  Task     :  獲取隨機數(此類function主要供測試時使用)                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		SetRandom(int iMax)
{
	int result = 0;
	if (iMax == 0) return 0;
	/*itime = (unsigned int)(time(NULL)) + rand();
	
	srand(itime);
	*/
	result = rand() % iMax + 1;
	return result;
}

void	SetRandoms(int* iArray, int iCount, int iMax)
{
	const int MAX = 30000;
	for(int i = 0; i < iCount; i++)
	{
		if (iMax <= 0)	*(iArray + i) = SetRandom(MAX);
		else			*(iArray + i) = SetRandom(iMax);
	}	
}

void	SetRandoms(long long* iArray, int iCount, int iMax)
{
	const int MAX = 300000;
	for(int i = 0; i < iCount; i++)
	{
		if (iMax <= 0)	*(iArray + i) = SetRandom(MAX);
		else			*(iArray + i) = SetRandom(iMax);
	}	
}

/*---------------------------------------------------------------------------+
|  Function :  SetCurveLimit                                                 |
|  Task     :  設置曲線設置的極值                                            |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetCurveLimit(CtmWnd* pwndCurve, double dx0, double dx1, double dy, DWORD dwColor, int iLineIndex, CURVEPOINT* points)
{
	if (pwndCurve == NULL) return;
	
	if (points == NULL) return;
	
	points[0].dX = dx0;
	points[1].dX = dx1;
	points[0].dY =
	points[1].dY = dy;
	SetCurveLine(pwndCurve, dwColor, iLineIndex, points, 2);
}

/*---------------------------------------------------------------------------+
|  Function :  SetCurveLine                                                  |
|  Task     :  設置曲線元件中的每根線斷                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetCurveLine(CtmWnd* pwndCurve, DWORD dwColor, int iLineIndex, CURVEPOINT* points, int iPointNum)
{
	if (pwndCurve == NULL) return;
	DWORD	color;
	char	szLineID[50];
	//if (points == NULL) return;
	
	sprintf(szLineID, "%s%d", "linecolor",iLineIndex);
	color = dwColor;
	pwndCurve->SetProperty(szLineID, &color);	
	((CCurve*)pwndCurve)->SetPoint(iLineIndex, points, iPointNum);
}

void	SetCurveLine(CtmWnd* pwndCurve, int iLineIndex, CURVEPOINT* points, int iPointNum)
{
	if (pwndCurve == NULL) return;
	((CCurve*)pwndCurve)->SetPoint(iLineIndex, points, iPointNum);
}

/*---------------------------------------------------------------------------+
|  Function :  GetSpecialControlNum                                          |
|  Task     :  獲得在pwndSender上面的，指定類型的且名稱中有指定字串的元件，  |
|              並且將元件的指針存儲在pwnd中                  				 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		GetSpecialControlNum(CtmWnd* pwndSender, char* pszName, char* pszType, CtmWnd** pwnd)
{
	int result = 0;
	char* temp = NULL;
	for (int i = 0; i < pwndSender->GetControlCount(); i++)
	{
		if (pszType == NULL) break;
		if (pwndSender->Controls[i]->Is(pszType))
		{
			if (pszName != NULL)
			{
				temp = strstr(pwndSender->Controls[i]->Name, pszName);				
				if (temp != NULL) 
				{	
					pwnd[result] = pwndSender->Controls[i];
					result++;
				}
			}
			else 
			{
				pwnd[result] = pwndSender->Controls[i];
				result++;
			}
		}
	}
	return result;
}

int		GetSpecialControlNum(CtmWnd* pwndSender, int  iTab, char* pszType, CtmWnd** pwnd)
{
	int result = 0;
	int	iTabOrder = 0xFFFFFFFF;
	for (int i = 0; i < pwndSender->GetControlCount(); i++)
	{
		if (pszType == NULL) break;
		if (pwndSender->Controls[i]->Is(pszType))
		{
			pwndSender->Controls[i]->GetPropValueT("taborder", &iTabOrder, sizeof(iTabOrder));
			if (iTabOrder <= iTab)			
			{
				pwnd[result] = pwndSender->Controls[i];
				result++;
			}
		}
	}
	return result;
}

int		GetSpecialControlNum(CtmWnd* pwndSender, char* pszPropertyName, CtmWnd** pwnd)
{
	int result = 0;
	for (int i = 0; i < pwndSender->GetControlCount(); i++)
	{
		if (pszPropertyName == NULL) break;
	
		if (pwndSender->Controls[i]->GetProperty(pszPropertyName) != NULL)
		{
			pwnd[result] = pwndSender->Controls[i];
			result++;	
		}
	}
	return result;
}

int		GetSpecialControlNum(CtmWnd* pwndSender, CtmWnd** pwnd, char* pszName)	//20110921 add
{
	int result = 0;
	char* temp = NULL;
	for (int i = 0; i < pwndSender->GetControlCount(); i++)
	{
		if (pwndSender->Controls[i]== NULL) break;
		
		if (pszName != NULL)
		{
			temp = strstr(pwndSender->Controls[i]->Name, pszName);				
			if (temp != NULL) 
			{	
				pwnd[result] = pwndSender->Controls[i];
				result++;
			}
		}
		else 
		{
			pwnd[result] = pwndSender->Controls[i];
			result++;
		}
	}
	return result;
}

int	GetSpecialControlDB(CtmWnd* pwndSender, char* pszDB, char* pszType, CtmWnd** pwnd)
{
	int result = 0;
	char szTemp[256];
	for (int i = 0; i < pwndSender->GetControlCount(); i++)
	{
		if (pszType == NULL) break;
		if (pwndSender->Controls[i]->Is(pszType))
		{
			if (pszDB != NULL)
			{
			    pwndSender->Controls[i]->GetPropValueT("dbid0", szTemp, sizeof(szTemp));
				if (szTemp[0] != '\0' && strcmp(szTemp, pszDB)==0)
				{
					pwnd[result] = pwndSender->Controls[i];
					result++;
				}
			}
			else 
			{
				pwnd[result] = pwndSender->Controls[i];
				result++;
			}
		}
	}
	return result;
}

/*---------------------------------------------------------------------------+
|  Function :  DrawTable                                                     |
|  Task     :  畫出表格                 				                     |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	DrawTable(CtmWnd* pwndSender, XY* pRow, int iRowNum, XY* pCol, int iColNum)
{
	_SetViewPort(pwndSender->GetWindowRECT().left, pwndSender->GetWindowRECT().top, pwndSender->GetWindowRECT().right, pwndSender->GetWindowRECT().bottom);
	_SetColor(BRIGHTWHITE);
	if (iRowNum > 0)
	{
		for (int i = 0; i < iRowNum / 2; i++)
		{
			_MoveTo(pRow->x, pRow->y);
			pRow++;
			_LineTo(pRow->x, pRow->y);
			pRow++;		
		}
	}
		
	if (iColNum > 0)
	{
		for (int i = 0; i < iColNum / 2; i++)
		{
			_MoveTo(pCol->x, pCol->y);
			pCol++;
			_LineTo(pCol->x, pCol->y);
			pCol++;		
		}
	}
}

void	DrawEqualRow(CtmWnd* pwndSender, XY StartPoint, XY EndPoint, int iRowNum)
{
	if (iRowNum <= 0) return;
	_SetViewPort(pwndSender->GetWindowRECT().left, pwndSender->GetWindowRECT().top, pwndSender->GetWindowRECT().right, pwndSender->GetWindowRECT().bottom);
	_SetColor(BRIGHTWHITE);
	int EqualY = (EndPoint.y - StartPoint.y) / iRowNum;
	for (int i = 0; i <= iRowNum; i++)
	{
		_MoveTo(StartPoint.x, StartPoint.y + EqualY * i);
		_LineTo(EndPoint.x, StartPoint.y + EqualY * i);
	}	
}

void	DrawEqualCol(CtmWnd* pwndSender, XY StartPoint, XY EndPoint, int iColNum)
{
	if (iColNum <= 0) return;
	_SetViewPort(pwndSender->GetWindowRECT().left, pwndSender->GetWindowRECT().top, pwndSender->GetWindowRECT().right, pwndSender->GetWindowRECT().bottom);
	_SetColor(BRIGHTWHITE);
	int EqualX = (EndPoint.x - StartPoint.x) / iColNum;
	for (int i = 0; i <= iColNum; i++)
	{
		_MoveTo(StartPoint.x + EqualX * i, StartPoint.y);
		_LineTo(StartPoint.x + EqualX * i, EndPoint.y);
	}
}

/*---------------------------------------------------------------------------+
|  Function :  ReadCurveFromFile                                             |
|  Task     :  Read Curve From File	                						 |
|  之所以點需要在外面定義,是因為曲線元件中是不保存點的,而僅僅是保持了點的指針|
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter: 第1個參數是操作的對象曲線                                      |
|             第2個參數是曲線所保存的文件                                    |
|             第3個參數是設定的點                                            |
|             第4個參數是曲線的ID                                            |
|             第5個參數是設置的Lineindex                                     |
|             第6個參數是設置的線的顏色                                      |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	ReadCurveFromFile(CtmWnd* pwndCurve, CtmRecordFile*	pCurveFile, CURVEPOINT* Points, 
				  int iCurveID, int iLineIndex, DWORD dwColor)
{
	int nReadState;
	double		dX;
	DBCURVE		CurveData;
	
	nReadState	= pCurveFile->ReadRecord(iCurveID, &CurveData);
	if( nReadState == tmFILE_BAD ||	CurveData.nCount <= 0 || CurveData.nIndex != iCurveID)
	{
		SetCurveLine(pwndCurve, dwColor, iLineIndex, NULL, 0);
	}
	else
	{
		dX = CurveData.dStartX;
		for (int i = 0; i < CurveData.nCount; i++)
		{
			Points[i].dX = dX;
			Points[i].dY = CurveData.Value[i].dData;
			dX -= CurveData.dDistance;
		}
		SetCurveLine(pwndCurve, dwColor, iLineIndex, Points, CurveData.nCount);
	}	
}

void	ReadCurveFromData(CtmWnd* pwndCurve, CURVEPOINT* Points, int iLineIndex, DWORD dwColor,
						char* pszXStartDBID, char* pszXDBID, char* pszYDBID, int iCount, char* pszXEndDBID)
{
	char sz[256];
	int i = 0;
	CURVEPOINT* TEmpPoints = Points;
	
	TEmpPoints->dX = g_pDatabase->Read(pszXStartDBID).dData;
	TEmpPoints++;
	for (i = 1; i <= iCount; i++)
	{
		sprintf(sz, "%s%d%s", pszXDBID, i, "_X");
		TEmpPoints->dX = 
		(TEmpPoints + 1)->dX = g_pDatabase->Read(sz).dData;
		TEmpPoints += 2;	
	}
	if (pszXEndDBID == NULL)	TEmpPoints->dX = 0.0;
	else						TEmpPoints->dX = g_pDatabase->Read(pszXEndDBID).dData;
	
	TEmpPoints = Points;
	for (i = 1; i <= iCount; i++)
	{
		sprintf(sz, "%s%d%s", pszYDBID, i, "_Y");
		(TEmpPoints + 1)->dY = 
		TEmpPoints->dY = g_pDatabase->Read(sz).dData;
	}
	SetCurveLine(pwndCurve, dwColor, iLineIndex, Points, iCount * 2);
}

void	GetControlInfoInForm(CtmWnd* pwndSender, char** pszArayControlName, int iMaxNum)
{
	/*
	int		GetSpecialControlNum(CtmWnd* pwndSender, char* pszName, char* pszType, CtmWnd** pwnd)
	char szName[50];
	int j = 0;
	for (int i = 0; i < iMaxNUm; i++)
	{
		j = 0;
		while ((pszArayControlName[j] != NULL) || (j < FUNCTION_NUM))
		{
			
			for (
			iArrayControlNum[i][j] = GetSpecialControlNum(pwndSender, 
			j++
		}
	}
	*/
}

/*---------------------------------------------------------------------------+
|  Function :  SetSpecialDB		                                             |
|  Task     :  將指定的Edit中的值,存儲到對應的DBID							 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetSpecialDB(CtmWnd* pwndEdit, int nStartIndex)
{
	int	i = nStartIndex;
	char	szDBID[256];// = NULL;
	char	psz[10];
	long long llValue = 0;
	
	if (!pwndEdit->Is("CtmEditX1")) return;
	do 
	{
		psz[0] = '\0';
		sprintf(psz, "dbid%d", i);
		pwndEdit->GetPropValueT(psz, szDBID, sizeof(szDBID));
		pwndEdit->GetPropValueT("value", &llValue, sizeof(llValue));
		if (szDBID[0] != '\0') 
		{
			SetDBValue(szDBID, llValue);
		}
		i++;
	} while (i < 10);
}

/*---------------------------------------------------------------------------+
|  Function :  SetInjectToBackUp                                             |
|  Task     :  將現有的射出10段位置,10段速度,段數,及射出保壓連動選擇,進行備份|
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
void	SetInjectToBackUp()
{
	////	段數
	//SetDBValue("TECH_SYS_INJ_PROF_INJ_CNNOPROFILEPOINTS", GetDBValue("INJ_PROF_INJPRESSURELIMIT_CNNOPROFILEPOINTS").lValue);
	//
	////射出保壓連動選擇
	//SetDBValue("TECH_SYS_INJ_CONFIG_FLAGINJECTIONMODE", GetDBValue("INJ_CONFIG_FLAGINJECTIONMODE").lValue);
	//
	////	射出轉換條件選擇
	//SetDBValue("TECH_SYS_INJ_TRANSITION_FLAGTRANSITIONCONDITION1", GetDBValue("INJ_TRANSITION_FLAGTRANSITIONCONDITION1").lValue);
	//SetDBValue("TECH_SYS_INJ_TRANSITION_POSNTRANSITION", GetDBValue("INJ_TRANSITION_POSNTRANSITION").lValue);
	//
	//char	szBackUp[128];
	//char	sz[128];
	//for ( int i = 0; i < 10; i++)
	//{
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_INJ_PROF_INJVELOCITY_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "INJ_PROF_INJVELOCITY_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);		
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_INJ_PROF_INJVELOCITY_POINTS%d_VELY", i + 1);
	//	sprintf(sz,  "INJ_PROF_INJVELOCITY_POINTS%d_VELY", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//}
}

/*------------------------------------------------------------------------------+
|  Function :  SetInjectFromBackUp                                              |
|  Task     :  將現有的射出10段位置,10段速度,段數,及射出保壓連動選擇從備份中恢復|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/
void	SetInjectFromBackUp()
{
	////	段數
	//SetDBValue("INJ_PROF_INJPRESSURELIMIT_CNNOPROFILEPOINTS", GetDBValue("TECH_SYS_INJ_PROF_INJ_CNNOPROFILEPOINTS").lValue);
	//SetDBValue("INJ_PROF_INJVELOCITY_CNNOPROFILEPOINTS", GetDBValue("TECH_SYS_INJ_PROF_INJ_CNNOPROFILEPOINTS").lValue);
	//
	////	射出轉換條件選擇
	//SetDBValue("INJ_TRANSITION_FLAGTRANSITIONCONDITION1", GetDBValue("TECH_SYS_INJ_TRANSITION_FLAGTRANSITIONCONDITION1").lValue);
	//SetDBValue("INJ_TRANSITION_POSNTRANSITION", GetDBValue("TECH_SYS_INJ_TRANSITION_POSNTRANSITION").lValue);
	//
	//char	szBackUp[128];
	//char	sz[128];
	//for ( int i = 0; i < 10; i++)
	//{
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_INJ_PROF_INJVELOCITY_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "INJ_PROF_INJVELOCITY_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_INJ_PROF_INJVELOCITY_POINTS%d_VELY", i + 1);
	//	sprintf(sz,  "INJ_PROF_INJVELOCITY_POINTS%d_VELY", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//}
	////射出保壓連動選擇
	//SetDBValue("INJ_CONFIG_FLAGINJECTIONMODE", GetDBValue("TECH_SYS_INJ_CONFIG_FLAGINJECTIONMODE").lValue);
}


/*------------------------------------------------------------------------------+
|  Function :  SetAutoPurge			                                            |
|  Task     :  將自動清料送至主機												|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/
void	SetAutoPurge()
{
	////	自動清料的時候首先需要保存射出,儲料的相關資料
	//SetInjectToBackUp();
	//SetChargeToBackUp();
	//
	////	射出段數
	//SetDBValue("INJ_PROF_INJPRESSURELIMIT_CNNOPROFILEPOINTS", 1);
	//SetDBValue("INJ_PROF_INJVELOCITY_CNNOPROFILEPOINTS", 1);
	////	儲料段數
	//SetDBValue("RCV_PROF_SPEED_CNNOPROFILEPOINTS", 1);
	//SetDBValue("RCV_PROF_PRESSURE_CNNOPROFILEPOINTS", 1);
	//
	////射出保壓連動選擇(射出)
	//SetDBValue("INJ_CONFIG_FLAGINJECTIONMODE", 1);
	//
	////	射出轉換條件選擇
	//SetDBValue("INJ_TRANSITION_FLAGTRANSITIONCONDITION1", 2);//	選擇位置
	//SetDBValue("INJ_TRANSITION_POSNTRANSITION", 500);
	//
	////	儲料背壓選擇
	//SetDBValue("RCV_CONFIG_FLAGPRESSURECONTROLMODE", 3);
	//
	////	位置
	//SetDBValue("INJ_PROF_INJVELOCITY_POINTS1_POSNX", GetDBValue("TECH_SYS_INJ_PROF_INJVELOCITY_POSNX").lValue);
	//SetDBValue("RCV_PROF_SPEED_POINTS1_POSNX", GetDBValue("TECH_SYS_RCV_PROF_SPEED_POSNX").lValue);
	//
	////	速度
	//SetDBValue("INJ_PROF_INJVELOCITY_POINTS1_VELY", GetDBValue("TECH_SYS_INJ_PROF_INJVELOCITY_VELY").lValue);
	//SetDBValue("RCV_PROF_SPEED_POINTS1_RPMY", GetDBValue("TECH_SYS_RCV_PROF_SPEED_RPMY").lValue);
}

/*------------------------------------------------------------------------------+
|  Function :  SetChargeToBackUp                                                |
|  Task     :  將現有的儲料10段位置，10段速度，段數，進行備份                   |
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/
void	SetChargeToBackUp()
{
	////	段數
	//SetDBValue("TECH_SYS_RCV_PROF_SPEED_CNNOPROFILEPOINTS", GetDBValue("RCV_PROF_SPEED_CNNOPROFILEPOINTS").lValue);
	//
	////	儲料背壓選擇
	//SetDBValue("TECH_SYS_RCV_CONFIG_FLAGPRESSURECONTROLMODE", GetDBValue("RCV_CONFIG_FLAGPRESSURECONTROLMODE").lValue);
	//
	//char	szBackUp[128];
	//char	sz[128];
	//for ( int i = 0; i < 10; i++)
	//{
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_RCV_PROF_SPEED_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "RCV_PROF_SPEED_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_RCV_PROF_SPEED_POINTS%d_RPMY", i + 1);
	//	sprintf(sz,  "RCV_PROF_SPEED_POINTS%d_RPMY", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//}
}

/*------------------------------------------------------------------------------+
|  Function :  SetChargeFromBackUp                                              |
|  Task     :  將現有的儲料10段位置，10段速度，段數，從備份中恢復               |
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/
void	SetChargeFromBackUp()
{
	////	段數
	//SetDBValue("RCV_PROF_SPEED_CNNOPROFILEPOINTS", GetDBValue("TECH_SYS_RCV_PROF_SPEED_CNNOPROFILEPOINTS").lValue);
	//SetDBValue("RCV_PROF_PRESSURE_CNNOPROFILEPOINTS", GetDBValue("TECH_SYS_RCV_PROF_SPEED_CNNOPROFILEPOINTS").lValue);
	//
	////	儲料背壓選擇
	//SetDBValue("RCV_CONFIG_FLAGPRESSURECONTROLMODE", GetDBValue("TECH_SYS_RCV_CONFIG_FLAGPRESSURECONTROLMODE").lValue);
	//
	//char	szBackUp[128];
	//char	sz[128];
	//for ( int i = 0; i < 10; i++)
	//{
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_RCV_PROF_SPEED_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "RCV_PROF_SPEED_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_RCV_PROF_SPEED_POINTS%d_RPMY", i + 1);
	//	sprintf(sz,  "RCV_PROF_SPEED_POINTS%d_RPMY", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//}
}


/*------------------------------------------------------------------------------+
|  Function :  SetMoldToBackUp		                                            |
|  Task     :  將現有的開關模10段位置,10段速度,段數,從備份中恢復,以及模保壓力;	|
|			   還有StopPointFlag,進行備份										|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/
void	SetMoldToBackUp()
{
	////	保存開模的段數
	//SetDBValue("TECH_SYS_MLD_PROF_VELOCITYOPEN_CNNOPROFILEPOINTS", GetDBValue("MLD_PROF_VELOCITYOPEN_CNNOPROFILEPOINTS").lValue);
	////	保存關模的段數
	//SetDBValue("TECH_SYS_MLD_PROF_VELOCITYCLOSE_CNNOPROFILEPOINTS", GetDBValue("MLD_PROF_VELOCITYCLOSE_CNNOPROFILEPOINTS").lValue);
	////	保存模保壓力(就是低壓施力上限)
	//SetDBValue("TECH_SYS_MLD_PROF_PROTECTION_FORCE", GetDBValue("MLD_PROF_PROTECTION_FORCE").lValue);
	////	保存開模行程
	//SetDBValue("TECH_SYS_MLD_GENER_POSNMOLDOPENPOSITION", GetDBValue("MLD_GENER_POSNMOLDOPENPOSITION").lValue);
	//
	//char	szBackUp[128];
	//char	sz[128];
	//for ( int i = 0; i < 10; i++)
	//{
	//	//	開模
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_VELY", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_VELY", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	if (i + 1 >= 2)	SetDBValue(sz, 100);
	//	
	//	//	關模
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_VELY", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_VELY", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	if (i + 1 >= 2)	SetDBValue(sz, 100);
	//	
	//	//	中子進的StopPointFlag
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_FLAGSTOPPOINT", i + 1);		 
	//	SetDBValue(sz, 0);
	//	
	//	//	中子退的StopPointFlag
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_FLAGSTOPPOINT", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_FLAGSTOPPOINT", i + 1);		 
	//	SetDBValue(szBackUp, GetDBValue(sz).lValue);
	//	
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_FLAGSTOPPOINT", i + 1);		 
	//	SetDBValue(sz, 0);
	//}
}


/*------------------------------------------------------------------------------+
|  Function :  SetMoldFromBackUp	                                            |
|  Task     :  將現有的開關模10段位置,10段速度,段數,從備份中恢復,以及模保壓力;	|
|			   還有StopPointFlag,從備份中恢復									|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/
void	SetMoldFromBackUp()
{
	//long nClose = GetDBValue("TECH_SYS_MLD_PROF_VELOCITYCLOSE_CNNOPROFILEPOINTS").lValue;
	//long nOpen = GetDBValue("TECH_SYS_MLD_PROF_VELOCITYOPEN_CNNOPROFILEPOINTS").lValue;
	////	保存模保壓力(就是低壓施力上限)
	//SetDBValue("MLD_PROF_PROTECTION_FORCE", GetDBValue("TECH_SYS_MLD_PROF_PROTECTION_FORCE").lValue);
	//SetDBValue("MLD_GENER_POSNMOLDOPENPOSITION", GetDBValue("TECH_SYS_MLD_GENER_POSNMOLDOPENPOSITION").lValue);
	//g_ptaski32->WriteValue("MLD_GENER_POSNMOLDOPENPOSITION");
	//char	szBackUp[128];
	//char	sz[128];
	//for (int i = 0; i < 10; i++)
	//{
	//	//	開模10段速度
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_VELY", i + 1);		 
	//	SetDBValue(sz,100);
	//	
	//	//	關模10段速度
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_VELY", i + 1);
	//	SetDBValue(sz, 100);
	//}
    //
	//long 	lPosTemp 	= 0;
	//long 	lPosTemp1 	= 0;
	//bool	bCanExit	= true;
	/*
	for (int i = 0; i < 10; i++)
	{
		
		//	關模需要檢測以後的幾段位置是否合法,條件是越來越小，且不能相等
		sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);
		lPosTemp = GetDBValue(szBackUp).lValue;
		
		if (i > 0)
		{
			sprintf(sz, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i);
			lPosTemp1 = GetDBValue(sz).lValue;
			
			if ((lPosTemp == lPosTemp1) &&  (lPosTemp1 > 0))	//	如果是非法的
			{
				lPosTemp = lPosTemp1 - 1;
				SetDBValue(szBackUp, lPosTemp);
			}
		}
		
		//	開模同楊需要檢測位置,條件是越來越大，且不能相等
		sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_POSNX", i + 1);
		lPosTemp = GetDBValue(szBackUp).lValue;
		if (i > 0)
		{
			sprintf(sz, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_POSNX", i);
			lPosTemp1 = GetDBValue(sz).lValue;
			
			if (lPosTemp == lPosTemp1)
			{
				lPosTemp = lPosTemp1 + 1;
				SetDBValue(szBackUp, lPosTemp);
			}
		}
	}
	*/
	//for (int i = 0; i < 10; i++)
	//{
	//	//	中子進的StopPointFlag
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_FLAGSTOPPOINT", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_FLAGSTOPPOINT", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	
	//	//	中子退的StopPointFlag
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_FLAGSTOPPOINT", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_FLAGSTOPPOINT", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	//	開模
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYOPEN_POINTS%d_VELY", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYOPEN_POINTS%d_VELY", i + 1);	
	//	if (i < nOpen) SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	else SetDBValue(sz, 100);
	//	
	//	//	關模
	//	//	10段位置
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_POSNX", i + 1);		 
	//	SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	//	10段速度
	//	sprintf(szBackUp, "TECH_SYS_MLD_PROF_VELOCITYCLOSE_POINTS%d_VELY", i + 1);
	//	sprintf(sz,  "MLD_PROF_VELOCITYCLOSE_POINTS%d_VELY", i + 1);		 
	//	//SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	if (i < nClose) SetDBValue(sz, GetDBValue(szBackUp).lValue);
	//	else SetDBValue(sz, 100);		
	//}
	//
	////	保存開模的段數
	//SetDBValue("MLD_PROF_VELOCITYOPEN_CNNOPROFILEPOINTS", nOpen);
	////	保存關模的段數
	//SetDBValue("MLD_PROF_VELOCITYCLOSE_CNNOPROFILEPOINTS", nClose);
}

/*------------------------------------------------------------------------------+
|  Function :  SetAdjust			                                            |
|  Task     :  將自動調模送至主機												|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/	
void 	SetAdjust()
{
	//SetMoldToBackUp();
	////	保存開模的段數
	//SetDBValue("MLD_PROF_VELOCITYOPEN_CNNOPROFILEPOINTS", 1);
	////	保存關模的段數
	//SetDBValue("MLD_PROF_VELOCITYCLOSE_CNNOPROFILEPOINTS", 1);
	////	保存模保壓力(就是低壓施力上限)
	//SetDBValue("MLD_PROF_PROTECTION_FORCE", GetDBValue("TECH_SYS_MLD_PROF_PROTECTION_FORCE").lValue);
	////	開模位置和速度
	//SetDBValue("MLD_PROF_VELOCITYOPEN_POINTS1_POSNX", GetDBValue("TECH_SYS_MLD_PROF_VELOCITYOPEN_POSNX").lValue);
	//SetDBValue("MLD_PROF_VELOCITYOPEN_POINTS1_VELY", GetDBValue("TECH_SYS_MLD_PROF_VELOCITYOPEN_VELY").lValue);
	////SetDBValue("MLD_GENER_POSNMOLDOPENPOSITION", GetDBValue("TECH_SYS_MLD_PROF_VELOCITYOPEN_POSNX").lValue);
	////	關模位置和速度
	//SetDBValue("MLD_PROF_VELOCITYCLOSE_POINTS1_POSNX", GetDBValue("TECH_SYS_MLD_PROF_VELOCITYCLOSE_POSNX").lValue);
	//SetDBValue("MLD_PROF_VELOCITYCLOSE_POINTS1_VELY", GetDBValue("TECH_SYS_MLD_PROF_VELOCITYCLOSE_VELY").lValue);
}

/*------------------------------------------------------------------------------+
|  Function :  GetRealPosData		                                            |
|  Task     :  由推力座位置算出模板實際位置										|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/	
long long		GetRealPosData()	//Sunny<20070612>
{
	long long OppRealPos = 0;
	OppRealPos = (long long)GetDBValue("METER_AD1_READ_CHL2_RELATIVEPOSITION").lValue;
	OppRealPos = OilToPosi(OppRealPos);
	return OppRealPos;
}

/*------------------------------------------------------------------------------+
|  Function :  CodeChange  			                                            |
|  Task     :  單字節轉化為雙字節處理											|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:                           -                                       |
|                                                                               |
|  Return   :                           -                                       |
+------------------------------------------------------------------------------*/	
void    CodeChange(char *pDst, char *pSrc) //Sunny<20070626>
{
    WORD    *pwDst  = (WORD *)pDst;
    
    while(*pSrc != 0)     *pwDst ++ = *pSrc ++;
    
    *pwDst  = 0;
}

/*------------------------------------------------------------------------------+
|  Function :  LoadData(DB_MOLDHDR *,STROAGETYPE ,int,WORD );					|
|  Task     :  先加載，後排序，直接用傳過來的指針修改外部資料	
|				此函數只用于對模具資料頭部結構	（ DB_MOLDHDR ）				|
+-------------------------------------------------------------------------------+
|  Call     :                                                                   |
|                                                                               |
|  Parameter:      	DB_MOLDHDR *p_SortModlData	:模具頭的緩沖區
|					STROAGETYPE	m_nDestiMedia	:存儲介質
|					int total_num				:需要排序的個數
|					WORD SORT_TYPE				:排序方式
|					
|						排序方式具體定義:  	0----按日期排序（）
|	           								1----按名稱排序（）
|	           								2----按材料排序（）
|	           								3----按顏色排序（）
|                                                                               |
|  Return   :      0                    -    yucheng<20080114>                  |
+------------------------------------------------------------------------------*/	
//此函數需要用到的兩個內容(已在commonaction.h中)
//#define SORT_BY_DATE 0
//#define SORT_BY_NAME 1
//================================================================================//
//		以下為排序所需要的功能函數，根據返回值決定排序方式，從大到小，或小到大。
//================================================================================//
// Sort by date from Big->Small
//如果需要反序，只需將返回值調換即可
int 	cmp_str_time ( const void *a , const void *b ) 
{ 
	DB_MOLDHDR *c = (DB_MOLDHDR*)a; 
	DB_MOLDHDR *d = (DB_MOLDHDR*)b;
	//modify by J.Wong 2017/2/9 10:01:40
	if(c->DateSave.year!=d->DateSave.year)
	{
		return d->DateSave.year-c->DateSave.year;
	}else{
		if(c->DateSave.month!=d->DateSave.month)
		{
			return d->DateSave.month-c->DateSave.month;	
		}else{
			return d->DateSave.day-c->DateSave.day>=0 ? 1:-1;	
		}	
	}
}  

// Sort by name(acID) 0-9,A-Z,a-z
int 	cmp_str_name ( const void *a , const void *b ) 
{ 
	DB_MOLDHDR *c = (DB_MOLDHDR*)a; 
	DB_MOLDHDR *d = (DB_MOLDHDR*)b;
	return strcmp( c->acID, d->acID); 
} 

//Sort by mold order  : add by Gemini 2014/7/28 14:20:12
//notice: the atrribute DB_MOLDHDR::Media is used as storaging  mold num
int cmp_word_moldorder(const void* a, const void* b)
{
	DB_MOLDHDR *c = (DB_MOLDHDR*)a; 
	DB_MOLDHDR *d = (DB_MOLDHDR*)b;
	return c->Media<=d->Media? -1 : 1 ;	
}

int 	cmp_dword ( const void *a , const void *b ) 
{
	return (*(DWORD *)a - *(DWORD *)b);
} 

//================================================================================//
//排序調用c標準庫中的qsort函數(現已不用，轉移到LoadData內部，僅留做參考yuc<080116>)
//================================================================================//
WORD 	SortModl(DB_MOLDHDR *p_SortModlData,int SortCount,WORD Type)
{
	switch(Type){
		case SORT_BY_NAME:		//sort by name		
			qsort(p_SortModlData,SortCount,sizeof(DB_MOLDHDR),cmp_str_name); 
			break;
		case SORT_BY_DATE:		//sort by date
			qsort(p_SortModlData,SortCount,sizeof(DB_MOLDHDR),cmp_str_time);
			break;
		case SORT_BY_MOLDORDER:  //sort by Mold order
			qsort(p_SortModlData,SortCount,sizeof(DB_MOLDHDR),cmp_word_moldorder);
			break;
		default:
			break;
	}
	return 0;
}
//================================================================================//
//	先加載，後排序，帶入：模具頭的緩沖區指針，存儲介質，頭部個數，排序類型
//================================================================================//
//	從物理介質讀取所有內容到本地緩沖
WORD LoadData(DB_MOLDHDR *p_SortModlData,STROAGETYPE m_nDestiMedia,int total_num,WORD SORT_TYPE)
{
	int i = 0, j = 0;
	int StartIndexIN=1;	
	DB_MOLDHDR	*m_pheader_sort = NULL;				//模具資料頭指針，包涵全部
	//m_pheader_sort	= CtmMoldSet::GetInstance()->GetMoldSetHead(StartIndexIN,total_num,m_nDestiMedia); 
	if (m_nDestiMedia == STORAGE_TYPE_INET && g_ptaskinet != NULL) 
	{
		/*FNET_MOLDHDRLIST *pnetmoldlist	= g_ptaskinet->GetNetMoldList();
		int nNum = pnetmoldlist->nCount / 20 +1;
		
		printf("nNum=%d \n", nNum);
		for(j = 0; j < nNum; j++)
		{
	    	if(pnetmoldlist != NULL)
	    	{
	    		m_pheader_sort			= pnetmoldlist->moldhdrlist;
	    		g_ptaskinet->GetNethdrList(j*20+1, (j+1)*20);//(j+1)*20);
	    	}
	    	
	    	printf("m_pheader_sort[0].acID=%s \n", m_pheader_sort[0].acID);

	    	for(i=0; m_pheader_sort[i].acID[0] !='\0' ;i++)
			{
				printf("m_pheader[%d].acID=%s j=%d \n", i, m_pheader_sort[i].acID, j);
				memcpy(&p_SortModlData[i+j*20],&m_pheader_sort[i],sizeof(DB_MOLDHDR));	//轉存到緩存
			}
		}
    	printf("LoadDatanCount:%d \n",pnetmoldlist->nCount);*/
    	//for(int i =0 ; i<total_num ; i++)
    	//	printf("LoadData--i:%d acID:%s %d/%d/%d \n",i,&m_pheader_sort[i].acID,m_pheader_sort[i].DateSave.month, m_pheader_sort[i].DateSave.day, m_pheader_sort[i].DateSave.year);
    }
    else
    {	
    	bzero(p_SortModlData,sizeof(p_SortModlData));
		m_pheader_sort	= CtmMoldSet::GetInstance()->GetMoldSetHead(StartIndexIN,total_num,m_nDestiMedia);
		for(i=0; m_pheader_sort[i].acID[0] !='\0' ;i++)
		{
			memcpy(&p_SortModlData[i],&m_pheader_sort[i],sizeof(DB_MOLDHDR));	//轉存到緩存
		} 
	}
	
	//p_SortModlData[m_nSourceCount].acID[0]='\0';
	switch (SORT_TYPE){
		case SORT_BY_NAME:		//sort by name		
			qsort(p_SortModlData,total_num,sizeof(DB_MOLDHDR),cmp_str_name); 
			break;
		case SORT_BY_DATE:		//sort by date
			qsort(p_SortModlData,total_num,sizeof(DB_MOLDHDR),cmp_str_time);
			break;
		case SORT_BY_MOLDORDER:  //sort by moldorder
			qsort(p_SortModlData,total_num,sizeof(DB_MOLDHDR),cmp_word_moldorder);
			break;
		default:
			qsort(p_SortModlData,total_num,sizeof(DB_MOLDHDR),cmp_str_time);
			break;
	}
//	for(int i =0 ; i<total_num ; i++)
//    		printf("LoadData--i:%d acID:%s %d/%d/%d \n",i,&p_SortModlData[i].acID,p_SortModlData[i].DateSave.month, m_pheader_sort[i].DateSave.day, p_SortModlData[i].DateSave.year);
	return 0;
}

void	ControlGif(CtmWnd* pwndGif, long long llValue, long long llMin, long long llMax, WORD wMinFrame, WORD wMaxFrame, WORD wDirection)
{
	gif_animation* 	pGifInfo;
	
	if(pwndGif != NULL)
	{
		pGifInfo = ((CtmImageGif*)pwndGif)->GetGifInfo();
		if(pGifInfo == NULL) return;
			
		wMinFrame = ((wMinFrame == 0) ? 0: wMinFrame - 1 );
		wMaxFrame = ((wMaxFrame == 0) ? 0: wMaxFrame  - 1);
		
		if(wMaxFrame > pGifInfo->frame_count - 1)
			wMaxFrame = pGifInfo->frame_count - 1;
		
		pwndGif->SetPropValueT("value", llValue);
		pwndGif->SetPropValueT("min", llMin);
		pwndGif->SetPropValueT("max", llMax);
		pwndGif->SetPropValueT("minframe", wMinFrame);
		pwndGif->SetPropValueT("maxframe", wMaxFrame);
		pwndGif->SetPropValueT("direction", wDirection);
		pwndGif->Update();
	}
}

char*	Point1_Str[] =		
{		
	"X",
	"Y",
	"Z",
	"U",
	"V",
	"W",
	"T",
	NULL
}; 
char*	Point2_Str[] =		
{		
	"O",
	"P",
	"Q",
	"I",
	"J",
	"K",
	"H",
	NULL
}; 



char*	CommandChang(char* G_Code1,char* f_Old_Command)		//2015/4/21 上午 09:20:33 Mario
{
	char* Old_Command1;
	char* Old_Command;
	char* temp_Command;
	char* ptr;
	int 	blk_skip_flag = 0;
	char* G_Code;
	int 	GOTO_Sign = 0;  
	Old_Command1= (char*) malloc(MAX_COMMAND_LEN*sizeof(char));
	temp_Command= (char*) malloc(MAX_COMMAND_LEN*sizeof(char));
	G_Code= (char*) malloc(INT_BUFFER_SIZE*sizeof(char));
	char * point1=NULL;			//取出第一點字串
	point1= (char*) malloc(MAX_COMMAND_LEN*sizeof(char));
	memset(point1,0,MAX_COMMAND_LEN);
	char * point2=NULL;			//取出第二點字串
	point2= (char*) malloc(MAX_COMMAND_LEN*sizeof(char));
	memset(point2,0,MAX_COMMAND_LEN);
	PassMark(Old_Command1,f_Old_Command);//移除指令中的註解
	//printf("Old_Command1 = %s.\n",Old_Command1);
	Old_Command = Old_Command1;
	//取出指令前的NXXX，並在return前加入
	Old_Command = PassSpace(Old_Command);
	//printf("Old_Command = %s.\n",Old_Command);
	if(*Old_Command == 'N' && isdigit(*(Old_Command+1))!=0)
	{
		GOTO_Sign = atoi(Old_Command+1);
		if(GOTO_Sign != 0)
		{
			Old_Command +=1;
			while(isdigit(*Old_Command)!=0) Old_Command+=1;
		}
		Old_Command = PassSpace(Old_Command);
	}
	blk_skip_flag = blk_skip(Old_Command);	//取出指令首字的'/'並在return前加入
	if(blk_skip_flag ==1 )
	{
		Old_Command = PassSpaceFindChar(Old_Command,'/');
		//printf("123 %s\n",Old_Command);
		if(Old_Command == NULL) goto ERROR;
		Old_Command++;
		//printf("123 %s\n",Old_Command);
	}
	memset(temp_Command,0,MAX_COMMAND_LEN);
	memset(G_Code,0,INT_BUFFER_SIZE);
	memset(G_Code1,0,INT_BUFFER_SIZE);
	strcpy(temp_Command,Old_Command);
			
	char DefinValue[15];	//配合GetDefinValue函示使用
	char DoubleStr[15];		//配合DoubleToString函示使用
	char CoordStr[15];		//配合GetCoordValue函示使用
	
	if(strspn(Old_Command,"C") == 1 && *(Old_Command+1) != '0' && isdigit(*(Old_Command+1)))//設定座標
	{
		char* temp=NULL;
		ptr = strstr(Old_Command,"C");
		if(ptr == NULL) goto ERROR;
		ptr+=1;
		if(!ISValue(ptr)) goto ERROR;
		sprintf(G_Code,"%d",atoi(ptr));
		ptr=PassInt(ptr);
		ptr=PassSpaceFindChar(ptr ,'=');
		if(ptr == NULL) goto ERROR;
		ptr = PassSpace(ptr+1);
		if(*ptr == '(')		//定義自訂點C
		{
			sprintf(G_Code,"G10 S%d ",atoi(G_Code));
			temp = FindAllAbsCoordValue(G_Code,CoordStr,ptr,COORD,1);
			if(temp == NULL) goto ERROR;
			sprintf(G_Code,"%s;",G_Code);
		}
		else if(*ptr == 'C' || *ptr == 'P')	//自訂點C的+-*/
		{
			BOOL OnePoint = FALSE;	//預設自訂點
			BOOL TwoPoint = FALSE;	//預設自訂點
			int  OneNum = 0;
			int  TwoNum = 0;
			if(*ptr == 'C')
			{
				ptr+=1;
				OnePoint=FALSE;
				if(!ISValue(ptr)) goto ERROR;
				OneNum = atoi(ptr);
			}
			else if(*ptr == 'P')
			{
				ptr+=1;
				OnePoint=TRUE;
				if(!ISValue(ptr)) goto ERROR;
				OneNum = atoi(ptr);
			}
			else goto ERROR;
			ptr=PassInt(ptr);
			ptr = PassSpace(ptr);
			if(*ptr == '+')
				sprintf(G_Code,"G06 P%d ",atoi(G_Code));
			else	if(*ptr == '-')
				sprintf(G_Code,"G07 P%d ",atoi(G_Code));
			else	if(*ptr == '*')
				sprintf(G_Code,"G08 P%d ",atoi(G_Code));
			else	if(*ptr == '/')
				sprintf(G_Code,"G09 P%d ",atoi(G_Code));
			ptr = PassSpace(ptr+1);
			if(*ptr == 'C')
			{
				ptr+=1;
				TwoPoint=FALSE;
				if(!ISValue(ptr)) goto ERROR;
				TwoNum = atoi(ptr);
			}
			else if(*ptr == 'P')
			{
				ptr+=1;
				TwoPoint=TRUE;
				if(!ISValue(ptr)) goto ERROR;
				TwoNum = atoi(ptr);
			}
			else	goto ERROR;
			ptr=PassInt(ptr);
			if(PassSpaceFindChar(ptr,';') == NULL) goto ERROR;
			
			if(OnePoint && TwoPoint)
				sprintf(G_Code,"%sQ%d ",G_Code,1);
			else if(OnePoint && !TwoPoint)
				sprintf(G_Code,"%sQ%d ",G_Code,3);
			else if(!OnePoint && TwoPoint)
				sprintf(G_Code,"%sQ%d ",G_Code,4);
			else if(!OnePoint && !TwoPoint)
				sprintf(G_Code,"%sQ%d ",G_Code,2);
			
			sprintf(G_Code,"%sR%d S%d;",G_Code,OneNum,TwoNum);
		}
	}
	else if(strspn(Old_Command,"J") == 1 && *(Old_Command+1) != '0' && isdigit(*(Old_Command+1)))//設定座標
	{
		char* temp=NULL;
		ptr = strstr(Old_Command,"J")+1;
		if(!ISValue(ptr)) goto ERROR; 
		sprintf(G_Code,"G10 S%d ",atoi(ptr));
		
		temp = FindAllAbsCoordValue(G_Code,CoordStr,ptr,JOINT,1);
		if(temp == NULL) goto ERROR;
		sprintf(G_Code,"%s;",G_Code);
	}
	else if(strspn(Old_Command,"#") == 1)//設定變數
	{
		char * temp= NULL;

		if(!ISValue(Old_Command+1)) goto ERROR; 
		sprintf(G_Code,"#%d=",atoi(Old_Command+1));
		temp = strstr(Old_Command,"=");
		if(temp == NULL) goto ERROR;
		temp +=1;
		while(temp != NULL)
		{
			while(PassSpaceFindChar(temp , '(')!=NULL)
			{
				temp = PassSpaceFindChar(temp , '(');
				sprintf(G_Code,"%s%c",G_Code,*temp);
				temp+=1;
			}
			//printf("1temp is %s\n",temp);
			int u_StrLen = GetDefinValue(DefinValue,temp,TRUE,3);
			if(u_StrLen == -1)	goto ERROR;
			sprintf(G_Code,"%s%s",G_Code,DefinValue);
			temp += u_StrLen;
			//printf("2temp is %s\n",temp);
			while(PassSpaceFindChar(temp , ')')!=NULL)
			{
				temp = PassSpaceFindChar(temp , ')');
				sprintf(G_Code,"%s%c",G_Code,*temp);
				temp+=1;
			}
			//printf("3temp is %s\n",temp);
			temp = strpbrk(temp,"+/-*");
			if(temp!=NULL)
			{
				sprintf(G_Code,"%s%c",G_Code,*temp);
				temp+=1;
			}
		}
		sprintf(G_Code,"%s;",G_Code);
	}
	else if(strspn(Old_Command,"@") == 1)//設定變數
	{
		char * temp= NULL;

		if(!ISValue(Old_Command+1)) goto ERROR; 
		sprintf(G_Code,"@%d=",atoi(Old_Command+1));
		temp = strstr(Old_Command,"=");
		if(temp == NULL) goto ERROR;
		temp +=1;
		while(temp != NULL)
		{
			while(PassSpaceFindChar(temp , '(')!=NULL)
			{
				temp = PassSpaceFindChar(temp , '(');
				sprintf(G_Code,"%s%c",G_Code,*temp);
				temp+=1;
			}
			//printf("1temp is %s\n",temp);
			int u_StrLen = GetDefinValue(DefinValue,temp,TRUE,3);
			if(u_StrLen == -1)	goto ERROR;
			sprintf(G_Code,"%s%s",G_Code,DefinValue);
			temp += u_StrLen;
			//printf("2temp is %s\n",temp);
			while(PassSpaceFindChar(temp , ')')!=NULL)
			{
				temp = PassSpaceFindChar(temp , ')');
				sprintf(G_Code,"%s%c",G_Code,*temp);
				temp+=1;
			}
			//printf("3temp is %s\n",temp);
			temp = strpbrk(temp,"+/-*");
			if(temp!=NULL)
			{
				sprintf(G_Code,"%s%c",G_Code,*temp);
				temp+=1;
			}
		}
		sprintf(G_Code,"%s;",G_Code);
	}
	else if(strstr(Old_Command,"PALLET") == Old_Command)//設定變數
	{
		ptr = Old_Command+6;
		if(!ISValue(ptr)) goto ERROR;
		sprintf(G_Code,"%d",atoi(ptr));
		ptr = PassSpace(PassInt(ptr));
		if(*ptr == '=')
		{
			int 	StartPoint = 0;
			int 	RowPoint = 0;
			int 	ColPoint = 0;
			int 	PointType = 0;
			char  RowNum[15];
			char  ColNum[15];
			int 	Mode = 0;
			ptr = PassSpace(ptr+1);
			if(*ptr != '(') goto ERROR;
			ptr+=1;
			ptr = PassSpace(ptr);
			//StartPoint
			if(*ptr == 'C' )
			{
				PointType+=4;
				ptr+=1;
				if(!ISValue(ptr)) goto ERROR;
				StartPoint = atoi(ptr);
			}
			else if (*ptr == 'P')
			{
				ptr+=1;
				if(!ISValue(ptr)) goto ERROR;
				StartPoint = atoi(ptr);
			}
			else goto ERROR;
			ptr = PassSpace(PassInt(ptr));
			//RowPoint
			if(*ptr == 'C' )
			{
				PointType+=2;
				ptr+=1;
				if(!ISValue(ptr)) goto ERROR;
				RowPoint = atoi(ptr);
			}
			else if (*ptr == 'P')
			{
				ptr+=1;
				if(!ISValue(ptr)) goto ERROR;
				RowPoint = atoi(ptr);
			}
			else goto ERROR;
			ptr = PassSpace(PassInt(ptr));
			//ColPoint
			if(*ptr == 'C' )
			{
				PointType+=1;
				ptr+=1;
				if(!ISValue(ptr)) goto ERROR;
				ColPoint = atoi(ptr);
			}
			else if (*ptr == 'P')
			{
				ptr+=1;
				if(!ISValue(ptr)) goto ERROR;
				ColPoint = atoi(ptr);
			}
			else goto ERROR;
			ptr = PassSpace(PassInt(ptr));
			
			if(strstr(ptr,"R") == ptr)
			{
				ptr = PassSpace(ptr+1);
				if(*ptr != '=') goto ERROR;
				ptr = PassSpace(ptr+1);
				int u_StrLen = GetDefinValue(DefinValue,ptr,FALSE,0);
				if(u_StrLen == -1)	goto ERROR;
				memset(RowNum,0,15);
				strcpy(RowNum,DefinValue);
				//if(!ISValue(ptr)) goto ERROR;
				//RowNum = atoi(ptr);
				ptr = PassSpace(ptr+u_StrLen);
			}
			else goto ERROR;
			
			if(strstr(ptr,"C") == ptr)
			{
				ptr = PassSpace(ptr+1);
				if(*ptr != '=') goto ERROR;
				ptr = PassSpace(ptr+1);
				int u_StrLen = GetDefinValue(DefinValue,ptr,FALSE,0);
				if(u_StrLen == -1)	goto ERROR;
				memset(ColNum,0,15);
				strcpy(ColNum,DefinValue);
				//if(!ISValue(ptr)) goto ERROR;
				//ColNum = atoi(ptr);
				ptr = PassSpace(ptr+u_StrLen);
			}
			else goto ERROR;
			if(*ptr == 'Z' )
				Mode = 0;
			else if(*ptr == 'S')
				Mode = 1;
			else
				goto ERROR;
			ptr = PassSpaceFindChar(ptr+1,')');
			if(ptr == NULL)  goto ERROR;
			ptr = PassSpaceFindChar(ptr+1,';');
			if(ptr == NULL)  goto ERROR;
			sprintf(G_Code,"G10 P%d Q%d A%d B%d C%d D%s E%s F%d;",atoi(G_Code),PointType,StartPoint,RowPoint,ColPoint,RowNum,ColNum,Mode);
		}
		else goto ERROR;
	}
	else if(strstr(Old_Command,"NEXT_PALLET")==Old_Command)
	{
		ptr = Old_Command + strlen("NEXT_PALLET");
		ptr = PassSpace(ptr);
		if(!isdigit(*ptr)) goto ERROR;
		sprintf(G_Code,"G12 P%d A1;",atoi(ptr));
		ptr = PassInt(ptr);
		if(PassSpaceFindChar(ptr,';') == NULL) goto ERROR;
	}
	else if(strstr(Old_Command,"LAST_PALLET")==Old_Command)
	{
		ptr = Old_Command + strlen("LAST_PALLET");
		ptr = PassSpace(ptr);
		if(!isdigit(*ptr)) goto ERROR;
		sprintf(G_Code,"G12 P%d A-1;",atoi(ptr));
		ptr = PassInt(ptr);
		if(PassSpaceFindChar(ptr,';') == NULL) goto ERROR;
	}
	else if(strstr(Old_Command,"FRIST_PALLET")==Old_Command)
	{
		ptr = Old_Command + strlen("FRIST_PALLET");
		ptr = PassSpace(ptr);
		if(!isdigit(*ptr)) goto ERROR;
		sprintf(G_Code,"G12 P%d A0;",atoi(ptr));
		ptr = PassInt(ptr);
		if(PassSpaceFindChar(ptr,';') == NULL) goto ERROR;
	}
	else if(strstr(Old_Command,"SET_PT_PALLET") == Old_Command)
	{
		ptr= Old_Command + strlen("SET_PT_PALLET");
		//printf("ptr is %s\n",ptr);
		//ptr = PassSpace(ptr);
		//if(*ptr != 'P') goto ERROR;
		//ptr = PassSpace(ptr+1);
		if(!isdigit(*ptr)) goto ERROR;
		sprintf(G_Code,"G12 P%d ",atoi(ptr));
		ptr = PassSpace(PassInt(ptr));
		if(*ptr != '=') goto ERROR;
		ptr = PassSpace(ptr+1);
		if(*ptr == '(')
		{
			ptr = PassSpace(ptr+1);
			if(*ptr != 'R')goto ERROR;
			ptr = PassSpace(ptr+1);
			if(*ptr != '=')goto ERROR;
			ptr = PassSpace(ptr+1);
			int u_StrLen = GetDefinValue(DefinValue,ptr,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!isdigit(*ptr)) goto ERROR;
			sprintf(G_Code,"%sR%s ",G_Code,DefinValue);
			ptr = PassSpace(ptr+u_StrLen);
			if(*ptr != 'C')goto ERROR;
			ptr = PassSpace(ptr+1);
			if(*ptr != '=')goto ERROR;
			ptr = PassSpace(ptr+1);
			u_StrLen = GetDefinValue(DefinValue,ptr,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!isdigit(*ptr)) goto ERROR;
			sprintf(G_Code,"%sC%s;",G_Code,DefinValue);
			//if(!isdigit(*ptr)) goto ERROR;
			//sprintf(G_Code,"%sC%d;",G_Code,atoi(ptr));
			ptr = PassSpace(ptr+u_StrLen);
			if(*ptr != ')')goto ERROR;
			ptr = PassSpace(ptr+1);
			if(PassSpaceFindChar(ptr,';')== NULL) goto ERROR;
		}
		else if(GetDefinValue(DefinValue,ptr,FALSE,0) != -1)
		{
			sprintf(G_Code,"%sB%s;",G_Code,DefinValue);
		}
		else goto ERROR;
	}
	else // 指令後強制空格
	{
		ptr = StrToke_TE(" =;(\n",Old_Command);
		if(strcmp(ptr,"PTP_ABS")==0) //指令PTP_ABS解碼
		{
			int 	PointType = 1;
			char * temp=NULL;
			char * temp1=NULL;
			ptr= temp_Command + strlen(ptr);
			temp = ptr;
			sprintf(G_Code,"G90 G00 ");
			memset(point1,0,MAX_COMMAND_LEN);
			char * Moveparam=NULL;
			temp = PassSpace(temp);
			if(*temp == '(')
			{
				temp1 = strstr(temp,")"); 
				if(temp1 == NULL) goto ERROR;
			}
			else
			{
				char tmp[15];
				if(!ISValue(temp+1)) goto ERROR;
				sprintf(tmp,"%d",atoi(temp+1));
				temp1 = PassSpace(temp+1);
				temp1 += strlen(tmp)-1;
			}
			strncpy(point1,temp,temp1-temp+1);
			
			Moveparam = temp1+1;
			if(strstr(point1,"(")!=NULL)	//輸入值
			{
				if(strstr(point1,"X")!=NULL)
				{
					PointType = 1;
					
					temp = FindAllAbsCoordValue(G_Code,CoordStr,point1,COORD,1);
					if(temp == NULL) goto ERROR;
				}
				else if(strstr(point1,"J")!=NULL)
				{
					PointType = 0;
					temp = FindAllAbsCoordValue(G_Code,CoordStr,point1,JOINT,1);
					if(temp == NULL) goto ERROR;
				}	
				else 
				goto ERROR; 
			}
			else if(strstr(point1,"C")!=NULL) 
			{
				PointType = 1;
				temp = strstr(point1,"C");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR; 
				sprintf(G_Code,"%sS%d ",G_Code,atoi(temp)); 
			} 
			else if(strstr(point1,"J")!=NULL) 
			{
				PointType = 0;
				temp = strstr(point1,"J");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR; 
				sprintf(G_Code,"%sS%d ",G_Code,atoi(temp)); 
			} 
			else if(strstr(point1,"P")!=NULL) 
			{ 
				PointType = 1;
				temp = strstr(point1,"P");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR; 
				sprintf(G_Code,"%sR%d ",G_Code,atoi(temp)); 
			} 
			else 
				goto ERROR; 
			temp= strstr(Moveparam,"S");
			if(temp == NULL) goto ERROR;
			temp +=1;
			int u_StrLen = GetDefinValue(DefinValue,temp,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			temp = PassSpace(temp+u_StrLen);
			temp =strstr(Moveparam,"F"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sB%d C%d;",G_Code,atoi(temp),PointType); 
		} 
		else if(strcmp(ptr,"LINE_ABS")==0)//指令LINE_ABS解碼 
		{ 
			char * temp=NULL;
			char * temp1=NULL;
			ptr= temp_Command + strlen(ptr);
			temp = ptr;
			sprintf(G_Code,"G90 G01 ");
			memset(point1,0,MAX_COMMAND_LEN);
			char * Moveparam=NULL;
			
			temp = PassSpace(temp);
			if(*temp == '(')
			{
				temp1 = strstr(temp,")"); 
				if(temp1 == NULL) goto ERROR;
			}
			else
			{
				char tmp[15];
				if(!ISValue(temp+1)) goto ERROR;
				sprintf(tmp,"%d",atoi(temp+1));
				temp1 = PassSpace(temp+1);
				temp1 += strlen(tmp)-1;
			}
			strncpy(point1,temp,temp1-temp+1);
			
			Moveparam = temp1+1;
			if(strstr(point1,"(")!=NULL)	//輸入值
			{
				if(strstr(point1,"X")!=NULL)
				{
					temp = FindAllAbsCoordValue(G_Code,CoordStr,point1,COORD,1);
					if(temp == NULL) goto ERROR;
				}
				else 
				goto ERROR; 
			}
			else if(strstr(point1,"C")!=NULL) 
			{
				temp = strstr(point1,"C");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR; 
				sprintf(G_Code,"%sS%d ",G_Code,atoi(temp)); 
			}
			else if(strstr(point1,"P")!=NULL) 
			{ 
				temp = strstr(point1,"P");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR; 
				sprintf(G_Code,"%sR%d ",G_Code,atoi(temp)); 
			} 
			else 
				goto ERROR; 
			temp= strstr(Moveparam,"S");
			if(temp == NULL) goto ERROR;
			temp +=1;
			int u_StrLen = GetDefinValue(DefinValue,temp,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			temp = PassSpace(temp+u_StrLen);
			temp =strstr(Moveparam,"F"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sB%d;",G_Code,atoi(temp)); 
		}
		else if(strcmp(ptr,"CIRCLE_ABS")==0)//指令CIRCLE_ABS解碼 
		{ 
			char * temp=NULL;
			char * temp1=NULL;
			ptr= temp_Command + strlen(ptr);
			temp = ptr;
			BOOL coordtype = TRUE;	// 預設為卡式
			sprintf(G_Code,"G90 G02 ");
			

			memset(point1,0,MAX_COMMAND_LEN);

			memset(point2,0,MAX_COMMAND_LEN);
			char * Moveparam=NULL;	//Moveparam為後面參數的字串
			
			temp = PassSpace(temp);
			if(*temp == '(')
			{
				temp1 = strstr(temp,")"); 
				if(temp1 == NULL) goto ERROR;
			}
			else
			{
				char tmp[15];
				if(!ISValue(temp+1)) goto ERROR;
				sprintf(tmp,"%d",atoi(temp+1));
				temp1 = PassSpace(temp+1);
				temp1 += strlen(tmp)-1;
			}
			strncpy(point1,temp,temp1-temp+1);
			
			temp = PassSpace(temp1+1);
			if(*temp == '(')
			{
				temp1 = strstr(temp,")")+1; 
				if(temp == NULL) goto ERROR;
			}
			else
			{
				char tmp[15];
				if(!ISValue(temp+1)) goto ERROR;
				sprintf(tmp,"%d",atoi(temp+1));
				temp1=temp+1;
				if(*temp1 == ' ')
					temp1+=1;
				temp1 += strlen(tmp)-1;
			}
			strncpy(point2,temp,temp1-temp+1);
			
			Moveparam = temp1+1;
			if(strstr(point1,"(") !=NULL)
			{
				if(strstr(point1,"X")!=NULL)
				{
					temp = FindAllAbsCoordValue(G_Code,CoordStr,point1,COORD,2);
					if(temp == NULL) goto ERROR;
				}
				else 
				goto ERROR; 		
			}
			else if(strstr(point1,"C")!=NULL) 
			{
				temp = strstr(point1,"C");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR;
				sprintf(G_Code,"%sF%d ",G_Code,atoi(temp)); 
			} 
			else if(strstr(point1,"P")!=NULL) 
			{ 
				temp = strstr(point1,"P");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR;
				sprintf(G_Code,"%sE%d ",G_Code,atoi(temp)); 
			} 
			else 
				goto ERROR; 
			
			if(strstr(point2,"(") != NULL)
			{
				if(strstr(point2,"X")!=NULL)
				{
					temp = FindAllAbsCoordValue(G_Code,CoordStr,point2,COORD,1);
					if(temp == NULL) goto ERROR;
				}
				else 
				goto ERROR; 
			}
			else if(strstr(point2,"C")!=NULL) 
			{
				temp = strstr(point2,"C");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR;
				sprintf(G_Code,"%sS%d ",G_Code,atoi(temp)); 
			} 
			else if(strstr(point2,"P")!=NULL) 
			{ 
				temp = strstr(point2,"P");
				if(temp == NULL) goto ERROR;
				temp +=1;
				if(!ISValue(temp)) goto ERROR;
				sprintf(G_Code,"%sR%d ",G_Code,atoi(temp)); 
			} 
			else 
				goto ERROR; 
			
			temp= strstr(Moveparam,"S"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			int u_StrLen = GetDefinValue(DefinValue,temp,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			temp = PassSpace(temp+u_StrLen); 
			temp =strstr(Moveparam,"F"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			if(!ISValue(temp)) goto ERROR;
			sprintf(G_Code,"%sB%d;",G_Code,atoi(temp)); 
		}
		else if(strcmp(ptr,"PTP_REL")==0)//指令PTP_REL解碼 
		{
			char * temp=NULL;
			char * temp1=NULL;
			int 	PointType = 1;
			ptr= temp_Command + strlen(ptr);
			sprintf(G_Code,"G91 G00 ");
			temp = PassSpace(ptr);

			memset(point1,0,MAX_COMMAND_LEN);
			char * Moveparam=NULL;
			if(*temp == '(')
			{
				temp1 = strstr(temp,")"); 
				if(temp1 == NULL) goto ERROR;
			}
			else
				goto ERROR;
			strncpy(point1,temp,temp1-temp+1);
			
			Moveparam=PassSpace(temp1+1);
			
			if(strstr(point1,"(")!=NULL)	//輸入值
			{
				if((strstr(point1,"X")!=NULL)||(strstr(point1,"Y")!=NULL)||(strstr(point1,"Z")!=NULL)||(strstr(point1,"RX")!=NULL)||(strstr(point1,"RY")!=NULL)||(strstr(point1,"RZ")!=NULL)||(strstr(point1,"POS")!=NULL))
				{
					PointType=1;
					temp = FindAllRelCoordValue(G_Code ,CoordStr ,point1 ,COORD ,1);
					if(temp==NULL) goto ERROR;
				}
				else if((strstr(point1,"J1")!=NULL)||(strstr(point1,"J2")!=NULL)||(strstr(point1,"J3")!=NULL)||(strstr(point1,"J4")!=NULL)||(strstr(point1,"J5")!=NULL)||(strstr(point1,"J6")!=NULL))
				{
					PointType=0;
					temp = FindAllRelCoordValue(G_Code ,CoordStr ,point1 ,JOINT ,1);
					if(temp==NULL) goto ERROR;
				}	
				else 
				goto ERROR; 
			}
			else 
				goto ERROR;
			temp= strstr(Moveparam,"S"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			int u_StrLen = GetDefinValue(DefinValue,temp,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			temp = PassSpace(temp+u_StrLen);
			temp =strstr(Moveparam,"F"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			if(!ISValue(temp)) goto ERROR;
			sprintf(G_Code,"%sB%d C%d;",G_Code,atoi(temp),PointType); 
			
		}
		else if(strcmp(ptr,"LINE_REL")==0)//指令LINE_REL解碼 
		{ 
			char * temp=NULL;
			char * temp1=NULL;
			ptr= temp_Command + strlen(ptr);
			sprintf(G_Code,"G91 G01 ");
			temp = PassSpace(ptr);
			memset(point1,0,MAX_COMMAND_LEN);
			char * Moveparam=NULL;
			if(*temp == '(')
			{
				temp1 = strstr(temp,")"); 
				if(temp1 == NULL) goto ERROR;
			}
			else
				goto ERROR;
			strncpy(point1,temp,temp1-temp+1);
			
			Moveparam=PassSpace(temp1+1);
			
			if(strstr(point1,"(")!=NULL)	//輸入值
			{
				if((strstr(point1,"X")!=NULL)||(strstr(point1,"Y")!=NULL)||(strstr(point1,"Z")!=NULL)||(strstr(point1,"RX")!=NULL)||(strstr(point1,"RY")!=NULL)||(strstr(point1,"RZ")!=NULL)||(strstr(point1,"POS")!=NULL))
				{
					temp = FindAllRelCoordValue(G_Code ,CoordStr ,point1 ,COORD ,1);
					if(temp==NULL) goto ERROR;
				}
				else 
				goto ERROR; 
			}
			else 
				goto ERROR; 
			temp= strstr(Moveparam,"S"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			int u_StrLen = GetDefinValue(DefinValue,temp,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			temp = PassSpace(temp+u_StrLen);
			temp =strstr(Moveparam,"F"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			if(!ISValue(temp)) goto ERROR;
			sprintf(G_Code,"%sB%d;",G_Code,atoi(temp)); 
		}
		else if(strcmp(ptr,"CIRCLE_REL")==0)//指令CIRCLE_REL解碼 
		{ 
			char * temp=NULL;
			char * temp1=NULL;
			ptr= temp_Command + strlen(ptr);
			temp = ptr;
			BOOL coordtype = TRUE;	// 預設為卡式
			sprintf(G_Code,"G91 G02 ");
			

			memset(point1,0,MAX_COMMAND_LEN);

			memset(point2,0,MAX_COMMAND_LEN);
			char * Moveparam=NULL;	//Moveparam為後面參數的字串
			
			temp=PassSpace(temp);
			
			if(*temp == '(')
				temp1 = strstr(temp,")"); 
			else
				goto ERROR;
			if(temp1 == NULL) goto ERROR;
			strncpy(point1,temp,temp1-temp+1);
			
			temp=PassSpace(temp1+1);
			
			if(*temp == '(')
				temp1 = strstr(temp,")"); 
			else
				goto ERROR;
			if(temp1 == NULL) goto ERROR;
			strncpy(point2,temp,temp1-temp+1);
			
			Moveparam = temp1+1;
			
			
			
			if(strstr(point1,"(")!=NULL)	//輸入值
			{
				if((strstr(point1,"X")!=NULL)||(strstr(point1,"Y")!=NULL)||(strstr(point1,"Z")!=NULL)||(strstr(point1,"RX")!=NULL)||(strstr(point1,"RY")!=NULL)||(strstr(point1,"RZ")!=NULL)||(strstr(point1,"POS")!=NULL))
				{
					temp = FindAllRelCoordValue(G_Code ,CoordStr ,point1 ,COORD ,2);
					if(temp==NULL) goto ERROR;
				}
			}
			else 
				goto ERROR;
			if(strstr(point2,"(")!=NULL)	//輸入值
			{
				if((strstr(point2,"X")!=NULL)||(strstr(point2,"Y")!=NULL)||(strstr(point2,"Z")!=NULL)||(strstr(point2,"RX")!=NULL)||(strstr(point2,"RY")!=NULL)||(strstr(point2,"RZ")!=NULL)||(strstr(point2,"POS")!=NULL))
				{
					temp = FindAllRelCoordValue(G_Code ,CoordStr ,point2 ,COORD ,1);
					if(temp==NULL) goto ERROR;
				}
			}
			else 
				goto ERROR; 
				
			temp= strstr(Moveparam,"S"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			int u_StrLen = GetDefinValue(DefinValue,temp,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			//if(!ISValue(temp)) goto ERROR; 
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			temp = PassSpace(temp+u_StrLen);
			temp =strstr(ptr,"F"); 
			if(temp == NULL) goto ERROR;
			temp +=1;
			if(!ISValue(temp)) goto ERROR;
			sprintf(G_Code,"%sB%d;",G_Code,atoi(temp)); 
		}
		else if(strcmp(ptr,"PTP_PLT")==0)
		{
			char * temp=NULL;
			ptr= temp_Command + strlen(ptr);
			ptr = PassSpace(ptr);
			if(ptr != strstr(ptr,"PALLET")) goto ERROR;
			ptr = PassSpace(ptr+6);
			if(!ISValue(ptr)) goto ERROR;
			sprintf(G_Code,"G00 P%d ",atoi(ptr));
			ptr = PassSpace(PassInt(ptr));
			if(*ptr == 'Z')
			{
				ptr = PassSpace(ptr+1);
				temp = GetCoordValue(CoordStr,ptr,3);
				if(temp == NULL) goto ERROR;
				sprintf(G_Code,"%sZ%s ",G_Code,temp);
				ptr = strstr(ptr,"S");
				if(ptr == NULL) goto ERROR;
			}
			else if(*ptr != 'S') goto ERROR;

			//if(*ptr != 'S') goto ERROR;
			ptr = PassSpace(ptr+1);
			int u_StrLen = GetDefinValue(DefinValue,ptr,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			ptr = PassSpace(ptr+u_StrLen);
			if(*ptr != 'F') goto ERROR;
			ptr = PassSpace(ptr+1);
			if(!ISValue(ptr)) goto ERROR;
			sprintf(G_Code,"%sB%d;",G_Code,atoi(ptr));
			ptr=PassSpace(PassInt(ptr));
			if(PassSpaceFindChar(ptr,';')== NULL) goto ERROR;
		}
		else if(strcmp(ptr,"LINE_PLT")==0)
		{
			char * temp=NULL;
			ptr= temp_Command + strlen(ptr);
			ptr = PassSpace(ptr);
			if(ptr != strstr(ptr,"PALLET")) goto ERROR;
			ptr = PassSpace(ptr+6);
			if(!ISValue(ptr)) goto ERROR;
			sprintf(G_Code,"G01 P%d ",atoi(ptr));
			ptr = PassSpace(PassInt(ptr));
			if(*ptr == 'Z')
			{
				ptr = PassSpace(ptr+1);
				temp = GetCoordValue(CoordStr,ptr,3);
				if(temp == NULL) goto ERROR;
				sprintf(G_Code,"%sZ%s ",G_Code,temp);
				ptr = strstr(ptr,"S");
				if(ptr == NULL) goto ERROR;
			}
			else if(*ptr == 'S')
				sprintf(G_Code,"%sZ0 ",G_Code);
			else goto ERROR;
			if(*ptr != 'S') goto ERROR;
			ptr = PassSpace(ptr+1);
			int u_StrLen = GetDefinValue(DefinValue,ptr,FALSE,0);
			if(u_StrLen == -1)	goto ERROR;
			sprintf(G_Code,"%sA%s ",G_Code,DefinValue); 
			ptr = PassSpace(ptr+u_StrLen);
			if(*ptr != 'F') goto ERROR;
			ptr = PassSpace(ptr+1);
			if(!ISValue(ptr)) goto ERROR;
			sprintf(G_Code,"%sB%d;",G_Code,atoi(ptr));
			ptr=PassSpace(PassInt(ptr));
			if(PassSpaceFindChar(ptr,';')== NULL) goto ERROR;
		}
		else if(strcmp(ptr,"GOTO")==0)
		{
			ptr= temp_Command + strlen(ptr);
			if(!ISValue(ptr)) goto ERROR;
			sprintf(G_Code,"GOTO %d;",atoi(ptr));
		}
		else if(strcmp(ptr,"RETURN")==0)
			sprintf(G_Code,"M99;");
		else if(strcmp(ptr,"CALLSUB")==0)
		{
			char* temp;
			sprintf(G_Code,"M98 ");
			ptr= temp_Command + strlen(ptr);
			if(!ISValue(ptr)) goto ERROR;
			sprintf(G_Code,"%sP%04d ",G_Code,atoi(ptr));
			temp = strstr(ptr,"L");
			if(temp == NULL) goto ERROR;
			temp+=1;
			if(!ISValue(temp)) goto ERROR;
			sprintf(G_Code,"%sL%d;",G_Code,atoi(temp));
		}
		else if(strcmp(ptr,"PAUSE")==0)
			sprintf(G_Code,"M00;");
		else if(strcmp(ptr,"END")==0)
			sprintf(G_Code,"M30;");
		else if(strcmp(ptr,"OPT_STOP")==0)
			sprintf(G_Code,"M01;");
		else if(strcmp(ptr,"WORKCOORD")==0)
		{
			ptr= PassSpace(temp_Command + strlen(ptr));
			if(*ptr == 'W')
			{
				ptr+=1;
				if(isdigit(*ptr)) 
					sprintf(G_Code,"G54 W%d;",atoi(ptr));
				else if(*ptr == '#')
				{
					ptr=PassSpace(ptr+1);
					if(!isdigit(*ptr)) goto ERROR;
					sprintf(G_Code,"G54 W#%d;",atoi(ptr));
				}
				else goto ERROR;
			}
			else goto ERROR;
		}
		else if(strcmp(ptr,"TOOL")==0)
		{
			ptr= PassSpace(temp_Command + strlen(ptr));
			if(*ptr == 'T')
			{
				ptr+=1;
				if(isdigit(*ptr)) 
					sprintf(G_Code,"G54 Q%d;",atoi(ptr));
				else if(*ptr == '#')
				{
					ptr=PassSpace(ptr+1);
					if(!isdigit(*ptr)) goto ERROR;
					sprintf(G_Code,"G54 Q#%d;",atoi(ptr));
				}
				else goto ERROR;
			}
			else goto ERROR;
		}
		else if(strcmp(ptr,"ALL_SPEED")==0)
		{
			ptr= temp_Command + strlen(ptr);
			ptr=PassSpace(ptr);
			if(!ISValue(ptr)) goto ERROR;
			if(PassSpaceFindChar(PassInt(ptr),';') == NULL) goto ERROR;
			sprintf(G_Code,"G10 A%d;",atoi(ptr));
		}
		else if(strcmp(ptr,"WAIT")==0)
		{
			ptr= PassSpace(temp_Command + strlen(ptr));
			if(strstr(ptr,"MS")!= NULL)
			{
				if(isdigit(*ptr))
				  sprintf(G_Code,"G04 P%d;",atoi(ptr));
				else if(*ptr == '#' && isdigit(*(ptr+1)))
					sprintf(G_Code,"G04 P#%d;",atoi(ptr+1));
				else goto ERROR;
			}
			else if(strstr(ptr,"S")!= NULL)
			{
				if(isdigit(*ptr))
					sprintf(G_Code,"G04 X%s;",DoubleToString(DoubleStr,atof(ptr),3));
				else if(*ptr == '#' && isdigit(*(ptr+1)))
					sprintf(G_Code,"G04 X#%d;",atoi(ptr+1));
				else goto ERROR;
			}
			else
				goto ERROR;
		}
		else if(strcmp(ptr,"SET_OUT")==0)
		{
			ptr= PassSpace(temp_Command + strlen(ptr));
			if(isdigit(*ptr))
				sprintf(G_Code,"G05 A%d ",atoi(ptr));
			else if(*ptr == '#' && isdigit(*(ptr+1)))
				sprintf(G_Code,"G05 A#%d ",atoi(ptr+1));
			else goto ERROR;
			ptr=strstr(ptr,"=");
			if(ptr == NULL) goto ERROR;
			ptr=PassSpace(ptr+1);
			if(isdigit(*ptr))
				sprintf(G_Code,"%sB%d;",G_Code,atoi(ptr));
			else if(*ptr == '#' && isdigit(*(ptr+1)))
				sprintf(G_Code,"%sB#%d;",G_Code,atoi(ptr+1));
			else goto ERROR;
		}
		else if(strcmp(ptr,"PATH_BLEND")==0)
		{
			ptr= PassSpace(temp_Command + strlen(ptr));
			if(strstr(ptr,"ON")!=NULL)
				sprintf(G_Code,"G05 A52 B1;");
			else if(strstr(ptr,"OFF")!=NULL)
				sprintf(G_Code,"G05 A52 B0;");
			else goto ERROR;
		}
		else if(strcmp(ptr,"IF")==0)
		{
			ptr= temp_Command + strlen(ptr);
			sprintf(G_Code,"IF ");
			char* point1_bk = point1;
			memset(point1_bk,0,MAX_COMMAND_LEN);
			memset(point2,0,MAX_COMMAND_LEN);
			int Cnt = 1;
			char * tmp = NULL;
			tmp =PassSpace(ptr);
			if(*tmp != '(')
				goto ERROR;
			while(Cnt != 0)
			{
				tmp+=1;
				if(*tmp == '(')
					Cnt+=1;
				else if(*tmp == ')')
					Cnt-=1;
				else if(*tmp == '\0' || *tmp == NULL)
					goto ERROR;
			}
			tmp+=1;
			strncpy(point1_bk,ptr,tmp-ptr);
			
			tmp =PassSpace(tmp);
			strcpy(point2,tmp);
			
			while(*point1_bk != '\0' || *point1_bk != NULL)
			{
				point1_bk =PassSpace(point1_bk);
				if(*point1_bk == '#')
				{
					if(GetDefinValue(DefinValue,point1_bk,TRUE,3) == -1)	goto ERROR;
					sprintf(G_Code,"%s%s",G_Code,DefinValue);
					point1_bk+=1;
					while(isdigit(*point1_bk) || *point1_bk == ' ')
						point1_bk+=1;
				}
				else if((*point1_bk == '-') || isdigit(*point1_bk))
				{
					if(GetDefinValue(DefinValue,point1_bk,TRUE,3) == -1)	goto ERROR;
					sprintf(G_Code,"%s%s",G_Code,DefinValue);
					point1_bk+=1;
					while(isdigit(*point1_bk) || *point1_bk == ' ')
						point1_bk+=1;
				}
				else if(isupper(*point1_bk))
				{
					if(strstr(point1_bk,"GET_IN") != NULL)
					{
						if(GetDefinValue(DefinValue,point1_bk,TRUE,3) == -1)	goto ERROR;
						sprintf(G_Code,"%s%s",G_Code,DefinValue);
						point1_bk = strstr (point1_bk,")")+1;
					}
					else if(strstr(point1_bk,"GET_PT_PALLET") != NULL)
					{
						if(GetDefinValue(DefinValue,point1_bk,TRUE,3) == -1)	goto ERROR;
						sprintf(G_Code,"%s%s",G_Code,DefinValue);
						point1_bk += 13;
						point1_bk = PassInt(PassSpace(point1_bk));
					}
					else "error";
				}
				else
				{
					sprintf(G_Code,"%s%c",G_Code,*point1_bk);
					point1_bk+=1;
				}
			}
			//sprintf(G_Code,"%s) ",G_Code);
			char tmp2[MAX_COMMAND_LEN];
			CommandChang(tmp2,point2);
			if(strcmp(tmp2,"error")==0) goto ERROR;
			sprintf(G_Code,"%s %s",G_Code,tmp2);
		}
		else if(strcmp(ptr,"MARKPASS")==0)
		{	
			free(point1);
			free(point2);
			free(Old_Command1);
			free(G_Code);
			free(temp_Command);
			return " ";				//被註解
		}
		else if(strlen(ptr) == 1)
		{
			free(point1);
			free(point2);
			free(Old_Command1);
			free(G_Code);
			free(temp_Command);
			return " ";				//被註解
		}
		else if(*ptr == '\n' )
		{	
			free(point1);
			free(point2);
			free(Old_Command1);
			free(G_Code);
			free(temp_Command);
			return " ";				//被註解
		}
		else
		{
			ERROR:
			free(point1);
			free(point2);
			free(Old_Command1);
			free(G_Code);
			free(temp_Command);
			//printf("end strlen(ptr) = %d\n",strlen(ptr));
			return "error";
		}
	}
	
	if(GOTO_Sign != 0)
	{
		sprintf(G_Code1,"N%d ",GOTO_Sign);
	}
	
	if(blk_skip_flag == 1)
	{
		sprintf(G_Code1,"%s/",G_Code1);
	}
	
	sprintf(G_Code1,"%s%s",G_Code1,G_Code);
	
	//sprintf(G_Code1,"%s",G_Code);
	//printf("G_Code = %s\n",G_Code);
	free(point1);
	free(point2);
	free(Old_Command1);
	free(G_Code);
	free(temp_Command);
	
	return G_Code1;

}        
char* FindAllRelCoordValue(char* G_Code ,char* CoordStr ,char* ptr ,int CoordMode ,int POINT_NUM)
{
	int Coord_Num=0,Max_Coord_Num=cfg.cn;
	if( CoordMode == JOINT )
		Max_Coord_Num=cfg.jn;
	char * Coord_Name=NULL;
	char* G_Flag=NULL;
	char* temp;
		
	while(Coord_Num<Max_Coord_Num)
	{
		if(CoordMode==COORD)
			Coord_Name = cfg.c_s[Coord_Num];
		else if(CoordMode==JOINT)
			Coord_Name = cfg.j_s[Coord_Num];
		if(Coord_Name == NULL) return NULL;
		
		temp = strstr(ptr,Coord_Name);
		if(POINT_NUM==1)
			G_Flag=Point1_Str[Coord_Num];
		else if(POINT_NUM==2)
			G_Flag=Point2_Str[Coord_Num];
			
		if(temp != NULL && *(temp-1) != 'R')
		{
			temp = GetCoordValue(CoordStr,temp+strlen(Coord_Name),3);
			if(temp == NULL) return NULL;
			
			sprintf(G_Code,"%s%s%s ",G_Code,G_Flag,temp);
		}
		else
			sprintf(G_Code,"%s%s%s ",G_Code,G_Flag,"0.0");
		Coord_Num++;
	}
	
	if(POINT_NUM==1)
		G_Flag=Point1_Str[6];
	else if(POINT_NUM==2)
		G_Flag=Point2_Str[6];
		
	if(CoordMode == COORD)
	{
		temp = strstr(ptr,cfg.c_s[cfg.cn]);
		if(temp == NULL) return NULL;
		temp = PassSpaceFindChar(temp+strlen(cfg.c_s[cfg.cn]),'=');
		if(temp == NULL) return NULL;
		temp = GetCoordValue(CoordStr,temp+1,0);
		if(temp == NULL) return NULL;
		sprintf(G_Code,"%s%s%s ",G_Code,G_Flag,temp);
	}
	else
		sprintf(G_Code,"%s%s%d ",G_Code,G_Flag,-1);
	
}
char* FindAllAbsCoordValue(char* G_Code ,char* CoordStr ,char* ptr ,int CoordMode ,int POINT_NUM)
{
	int Coord_Num=0,Max_Coord_Num=cfg.cn;
	if( CoordMode == JOINT )
		Max_Coord_Num=cfg.jn;
	char* G_Flag;
	char* temp;
	while(Coord_Num < Max_Coord_Num)
	{
		temp =FindAbsCoordValue(CoordStr,ptr,Coord_Num,CoordMode);
		if(temp == NULL) return NULL;
		if(POINT_NUM==1)
			G_Flag=Point1_Str[Coord_Num];
		else if(POINT_NUM==2)
			G_Flag=Point2_Str[Coord_Num];
		sprintf(G_Code,"%s%s%s ",G_Code,G_Flag,temp);
		Coord_Num++;
	}	
	//POSTURE
	if(POINT_NUM==1)
		G_Flag=Point1_Str[6];
	else if(POINT_NUM==2)
		G_Flag=Point2_Str[6];
	if(CoordMode==COORD)
	{
		/*temp =FindAbsCoordValue(CoordStr,ptr,Max_Coord_Num,CoordMode);
		if(temp == NULL) return NULL;
		sprintf(G_Code,"%s%s%s ",G_Code,G_Flag,temp);*/
		sprintf(G_Code,"%s%s%d ",G_Code,G_Flag,0);
	}
	else
		sprintf(G_Code,"%s%s%d ",G_Code,G_Flag,-1);
	return G_Code;
}
char* FindAbsCoordValue(char* restr ,char* Command ,int CoordNum ,int CoordMode)
{
	char * m_restr=NULL;
	char * Coord_Name=NULL;
	int prec = 3;
	if(CoordMode==COORD)
	{
		Coord_Name = cfg.c_s[CoordNum];
		if(Coord_Name == NULL) return NULL;
	}
	else if(CoordMode==JOINT)
		Coord_Name = cfg.j_s[CoordNum];
	if(Coord_Name == NULL) return NULL;
	m_restr = strstr(Command,Coord_Name);
	if(m_restr == NULL) return NULL;
	m_restr = PassSpaceFindChar(m_restr+strlen(Coord_Name),'=');
	if(m_restr == NULL) return NULL;
	m_restr = GetCoordValue(restr,m_restr+1,prec);
	return m_restr;
}

int	GetDefinValue(char* restr, char* value,BOOL mode, int prec) //整理數值，含#,123,GET_IN   mode用來決定是否判斷GET_IN與GET_PT_PALLET
{
	memset(restr,0,15);
	char* start_ptr = value;
	value =PassSpace(value);

	if(*value =='#' && isdigit(*(value+1)))
	{
		value+=1;
		value =PassSpace(value);
		sprintf(restr,"#%d",atoi(value));
		value +=strlen(restr)-1;
	}
	else if(strstr(value,"GET_IN") == value && mode)
	{
		value=strpbrk(value,"(");
		if(value == NULL) return -1;
		value+=1;
		value = PassSpace(value);
		if(!isdigit(*value)) return -1;
		if(atoi(value) > 100 || atoi(value)<1) return -1;
		sprintf(restr,"$%d",200+atoi(value));
		value=strpbrk(value,")");
		if(value == NULL) return -1;
		value+=1;
	}
	else if(strstr(value,"GET_PT_PALLET") == value && mode)
	{
		value= value + 13;
		value = PassSpace(value);
		if(!isdigit(*value)) return -1;
		if(atoi(value) > 50 || atoi(value)<1) return -1;
		int temp = atoi(value);
		sprintf(restr,"$%d",48+temp);
		char temp2[3];
		sprintf(temp2,"d",temp);
		value+=strlen(temp2);
	}
	else if(ISValue(value))
	{
		char temp[15];
		sprintf(restr,"%s",DoubleToString(temp,atof(value),prec));
		while(*value=='.' || isdigit(*value))
			value+=1;
	}
	else 
		return -1;
	return value-start_ptr;
}


BOOL	ISValue(char* str)		//忽略空格，判斷是否為數字或正負號
{
	str = PassSpace(str);
	if((*str == '+') || (*str == '-') || isdigit(*str))
		return TRUE;
	else 
		return FALSE;
	
}

char* DoubleToString(char* restr, double value,int num)	//配合小數點後num位 浮點轉字串
{
	char* temp2=NULL;
	sprintf(restr,"%.*f",num,value);
	temp2 = restr + strlen(restr) -1;
	
	for(int i = 0 ; i < num-1; i++)
	{
		if(*temp2 == '0')
		{
			*temp2 = NULL;
			temp2 -=1;
		}
		else
			break;
	}
	return restr;
}

char*	GetCoordValue(char* restr, char* f_ptr ,int f_ValuePrec)	//整理出錯標值
{
	//char* temp=NULL;
	//temp = (char*) malloc(15*sizeof(char));
	memset(restr,0,15);
	
	if(f_ptr == NULL) return NULL;
	char* ptr = NULL;
	ptr = PassSpace(f_ptr);
	
	//printf("ptr is %s\n",ptr);
	if(*ptr == '#')
	{
		ptr = PassSpace(ptr+1);
		if(!ISValue(ptr)) return NULL;
		sprintf(restr,"#%d",atoi(ptr));
	}
	else if((*ptr == '+') || (*ptr == '-'))
	{
		if(!isdigit(*(ptr+1))) return NULL;
		if(f_ValuePrec == 0)
			sprintf(restr,"%d",atoi(ptr));
		else 
		{
			char temp[15];
			sprintf(restr,"%s",DoubleToString(temp,atof(ptr),f_ValuePrec));
		}
	}
	else if(isdigit(*ptr))
	{
		if(f_ValuePrec == 0)
			sprintf(restr,"%d",atoi(ptr));
		else 
		{
			char temp[15];
			sprintf(restr,"%s",DoubleToString(temp,atof(ptr),f_ValuePrec));
		}
	}

	else return NULL;
	return restr;
}

char* PassInt(char* ptr)	//不須判斷NULL	(只會跳過為首的正負號)
{
	if(*ptr == '+' || *ptr == '-')
		ptr+=1;
	while(isdigit(*ptr))
		ptr+=1;
	return ptr;
}

char* PassSpace(char* ptr)	//不須判斷NULL	跳過空格
{
	while(*ptr == ' ')
		ptr++;
	return ptr;
}

char* PassSpaceFindChar(char* ptr , char ptr1)  //尋找某的字元，會忽略空格。
{
	ptr = PassSpace(ptr);
	if(*ptr == ptr1)
		return ptr;
	else
		return NULL;
}

int		blk_skip(char* ptr)
{
	ptr = PassSpace(ptr);
	if(*ptr == '/')
		return 1;
	else
		return 0;
	
}

void	PassMark(char* ptr , char* ptr1)		//處理指令註解(跳過)
{
	memset(ptr,0,MAX_COMMAND_LEN);
	char	TempStr[MAX_COMMAND_LEN];
	memset(TempStr,0,MAX_COMMAND_LEN);
	char*	temp = NULL;
	char* temp2 = NULL;
	ptr1 = PassSpace(ptr1);
	temp = strstr(ptr1,"//");
	
	if(temp != NULL)
	{
		if(temp != ptr1)
		{
			strncpy(ptr,ptr1,temp-ptr1);
			sprintf(ptr,"%s\n",ptr);
		}
		else if(temp == ptr1)
			sprintf(ptr,"MARKPASS");
	}
	else
	{
		strcpy(ptr,ptr1);
	}
	temp = NULL;
	
	temp = strstr(ptr,"/*");
	while(temp != NULL)
	{
		temp2 = strstr(temp,"*/");
		//printf("ptr = %s\n",ptr);
		//printf("temp2 = %s\n",temp2);
		if(temp2 != NULL)
		{
			memset(TempStr,0,MAX_COMMAND_LEN);
			//printf("1temp-ptr = %d\n",temp-ptr);
			strncpy(TempStr,ptr,temp-ptr);
			//printf("2TempStr = %s\n",TempStr);
			sprintf(TempStr,"%s%s",TempStr,temp2+2);
			//printf("3TempStr = %s\n",TempStr);
			strcpy(ptr,TempStr);
		}
		else 
		{
			sprintf(ptr,"error");
			return  ;
		}
		temp = NULL;
		temp = strstr(ptr,"/*");
	}
}

BOOL	PassMark2(char* ptr , char* ptr1)		//處理指令註解(跳過)
{
	memset(ptr,0,MAX_COMMAND_LEN);
	char	TempStr[MAX_COMMAND_LEN];
	memset(TempStr,0,MAX_COMMAND_LEN);
	char*	temp = NULL;
	char* temp2 = NULL;
	ptr1 = PassSpace(ptr1);
	strcpy(ptr,ptr1);
	temp = strstr(ptr,"/*");
	while(temp != NULL)
	{
		temp2 = strstr(temp,"*/");
		if(temp2 != NULL)
		{
			strncpy(TempStr,ptr,temp-ptr);
			sprintf(TempStr,"%s%s",TempStr,temp2+2);
			strcpy(ptr,TempStr);
		}
		else 
		{
			//sprintf(ptr,"error");
			return  FALSE;
		}
		temp = NULL;
		temp = strstr(TempStr,"/*");
	}
	return TRUE;
} 

int		UpdateProgToNC(char* FileName)
{
	//printf("UpdateProgToNC\n");
	NC_FileCheck SendDate;
	char md5_str[MD5_STR_LEN + 1];
	char file_name_md5[50];
	char full_filename[50];
	if(FileName==NULL)	//更新Prog目錄，傳送MD5給NC，作為判斷檔案是否存在且相同的依據
	{
		DIR *d; 
		struct dirent *file;
		d = opendir("Prog");
		while((file = readdir(d)) != NULL)
		{
			if(CheckFile(file->d_name) != 1) continue;
			//printf("file->d_name = %s\n",file->d_name);
			sprintf(full_filename, "%s/%s", g_szProgDirectory, file->d_name);
			//printf("full_filename = %s\n",full_filename);
			if(Compute_file_md5(full_filename, md5_str) == 0)
			{
				WORD filename_len = strlen(full_filename)+1;
				int package_len=sizeof(WORD)+sizeof(WORD)+sizeof(WORD)+filename_len+DM5_LENGTH;
				char* ptr = (char*)malloc(package_len);
				char* send_ptr = ptr;
				*(WORD*)send_ptr = HMI2NC_CONFIRM_FILE;
				send_ptr += sizeof(WORD);
				*(WORD*)send_ptr = 1;//isProg=true;
				send_ptr += sizeof(WORD);
				*(WORD*)send_ptr = filename_len;
				send_ptr += sizeof(WORD);
				strncpy(send_ptr,full_filename,filename_len);
				send_ptr += filename_len;
				strncpy(send_ptr,md5_str,DM5_LENGTH);
				CNC_Ethernet_Send(package_len,ptr);
				/*memset(&file_name_md5[0],0,sizeof(file_name_md5));
				SendDate.isProg=1;
				strcpy(&SendDate.Full_File_Name[0],full_filename);
				strcpy(&SendDate.MD5_Code[0],md5_str);
				memset(&md5_str[0],0,sizeof(md5_str));
				CNC_Ethernet_Send(HMI2NC_CONFIRM_FILE,0,sizeof(SendDate),(void*)&SendDate);*/
				//usleep(500);
			}	
		}
	}
	else
	{
		if(file_exists(FileName)!=0) return 0;
		Etnernet_File(FileName);
	}
	return 1;
	
}

int		UpdateFileToNC(char *full_filename)
{
	//printf("UpdateFileToNC\n");
	if(g_ptaskCncComm==NULL) return -1;
	NC_FileCheck SendDate;
	memset(&SendDate,0,sizeof(SendDate));
	char md5_str[MD5_STR_LEN + 1];
	memset(&md5_str[0],0,sizeof(md5_str));
	if(Compute_file_md5(full_filename, md5_str) == 0)
	{
		WORD filename_len = strlen(full_filename)+1;
		int package_len=sizeof(WORD)+sizeof(WORD)+sizeof(WORD)+filename_len+DM5_LENGTH;
		char* ptr = (char*)malloc(package_len);
		char* send_ptr = ptr;
		*(WORD*)send_ptr = HMI2NC_CONFIRM_FILE;
		send_ptr += sizeof(WORD);
		*(WORD*)send_ptr = 0;	//isProg=false;
		send_ptr += sizeof(WORD);
		*(WORD*)send_ptr = filename_len;
		send_ptr += sizeof(WORD);
		strncpy(send_ptr,full_filename,filename_len);
		send_ptr += filename_len;
		strncpy(send_ptr,md5_str,DM5_LENGTH);
		CNC_Ethernet_Send(package_len,ptr);
	}	
}

int		CheckFile(char* FileName)
{
	char *f_FileName=FileName;
	if(strcmp(f_FileName,"..")==0) return 11;
	if(*f_FileName == '.') return 12;
	if(strlen(f_FileName) != 5) return -1;
	if(*f_FileName != 'O') return -1;
	f_FileName++;
	if(isdigit(*f_FileName)==0)	return -1;
	f_FileName++;
	if(isdigit(*f_FileName)==0)	return -1;
	f_FileName++;
	if(isdigit(*f_FileName)==0)	return -1;
	f_FileName++;
	if(isdigit(*f_FileName)==0)	return -1;
	return 1;
	
}

BOOL file_exists (char * fileName)
{
  struct stat buf;
	int i = stat ( fileName, &buf );
  if ( i == 0 )
    return TRUE;
  return FALSE;
}

int		Etnernet_File (char * fileName)
{
	//printf("Etnernet_File\n");
	WORD PACKAGE_LENGTH=1000;
	
	if(g_ptaskCncComm==NULL) return -1;
	long	len,num=1;
	FILE *file = fopen(fileName, "rb");
	if(file == NULL)
		return -1;
	
	WORD filename_len = strlen(fileName)+1;
	long filebuffer_len = PACKAGE_LENGTH-(sizeof(WORD)+sizeof(WORD)+filename_len+sizeof(WORD)+sizeof(long));
	char buffer[filebuffer_len];	
	memset(buffer,'\0',sizeof(buffer));
	
	char* ptr = malloc(PACKAGE_LENGTH);
	memset(ptr,0,PACKAGE_LENGTH);
	char* set_data = ptr; 
	char* ptr_filebuffer_len;
	*(WORD*) set_data = HMI2NC_FILE;
	set_data +=sizeof(WORD);
	*(WORD*) set_data = filename_len;
	set_data +=sizeof(WORD);
	strncpy(set_data,fileName,filename_len);
	ptr_filebuffer_len = set_data+filename_len;
	while((len = fread(buffer, 1, sizeof(buffer), file)) == sizeof(buffer))
	{
		set_data=ptr_filebuffer_len;
		*(WORD*) set_data = num;
		set_data +=sizeof(WORD);
		*(long*) set_data = len;
		set_data +=sizeof(long);
		memcpy(set_data,buffer,len);
  	CNC_Ethernet_Send(PACKAGE_LENGTH,ptr);
		memset(buffer,'\0',sizeof(buffer));
		num++;
	}
	if(len >0)
  {
		set_data=ptr_filebuffer_len;
		*(WORD*) set_data = num;
		set_data +=sizeof(WORD);
		*(long*) set_data = len;
		set_data +=sizeof(long);
		memcpy(set_data,buffer,len);
  	CNC_Ethernet_Send(sizeof(WORD)+sizeof(WORD)+filename_len+sizeof(WORD)+sizeof(long)+len,ptr);
		memset(buffer,'\0',sizeof(buffer));
  }
  free(ptr);
  fclose(file);
  return 0; 
}
int Compute_file_md5(const char *file_path, char *md5_str)  
{  
    int i;  
    int fd;  
    int ret;  
    unsigned char data[READ_DATA_SIZE];  
    unsigned char md5_value[MD5_SIZE];  
    MD5_CTX md5;  
  
    fd = open(file_path, O_RDONLY);  
    if (-1 == fd)  
    {  
        perror("open");  
        return -1;  
    }  
  
    // init md5  
    MD5Init(&md5);  
  
    while (1)  
    {  
        ret = read(fd, data, READ_DATA_SIZE);  
        if (-1 == ret)  
        {  
            perror("read");  
            return -1;  
        }  
  
        MD5Update(&md5, data, ret);  
  
        if (0 == ret || ret < READ_DATA_SIZE)  
        {  
            break;  
        }  
    }  
  
    close(fd);  
  
    MD5Final(&md5, md5_value);  
  
    for(i = 0; i < MD5_SIZE; i++)  
    {  
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);  
    }  
    md5_str[MD5_STR_LEN] = '\0'; // add end  
  
    return 0;  
}  

int		UpdateTeachPoint(int cd ,int TPNum ,LONG_AXIS Value ,int Pos)
{
	//printf("UpdateTeachPoint\n");
	NC_TeachPoint	SendData;
	memset(&SendData,0,sizeof(SendData));
	SendData.Coordinate = cd;
	SendData.TeachPointNum = TPNum;
	memcpy(&SendData.TeachPointValue,&Value,sizeof(LONG_AXIS));
	SendData.TeachPointPos = Pos;
	CNC_Ethernet_Send(HMI2NC_TEACHPOINT,0,sizeof(SendData),&SendData);
}

int CNC_Ethernet_Send(int type ,int s_arr ,int len ,void* data)
{
	//printf("CNC_Ethernet_Send1\n");
	if(g_ptaskCncComm==NULL) return -1;
	return g_ptaskCncComm->WriteValue(type,s_arr,len,data);
}

int CNC_Ethernet_Send(int len ,void* data)
{
	//printf("CNC_Ethernet_Send2\n");
	if(g_ptaskCncComm==NULL) return -1;
	return g_ptaskCncComm->WriteValue(len,data);
}