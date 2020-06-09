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
#define 	MAX_MOLD_CNT		1000	//�Ҩ�i�γ̤j��(���a�Ƨǽw�s)
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
//	�۰ʲM��
///////////////
void	SetInjectToBackUp();
void	SetInjectFromBackUp();
void	SetAutoPurge();
void	SetChargeToBackUp();
void	SetChargeFromBackUp();

////////////////
//	�۰ʽռ�
///////////////
void	SetMoldToBackUp();
void	SetMoldFromBackUp();
void 	SetAdjust();
extern  char*	CommandChang(char* Gcode, char* f_Old_Command);		//2015/4/21 �W�� 09:20:33 Mario
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
// �H�U���Ƨǻݭn�Ψ쪺�Y�Ψ��
//----------------------------------------------------------------------------------
WORD 		LoadData(DB_MOLDHDR *p_SortModlData,STROAGETYPE m_nDestiMedia,int total_num,WORD SORT_TYPE);
//----------------------------------------------------------------------------------

long long		GetRealPosData();	//Sunny<20070612>
void    		CodeChange(char *pDst, char *pSrc);//Sunny<20070626>
extern		int			g_iPrivilege;			//	�v��
extern		char*		g_pszUserID;			//	�Τ�W��
extern		BOOL		g_bStart;
extern		BOOL		g_bKeyLockFlag;			//  ��LLock Flag
extern		int			g_nTestPortAry[6];

extern		char		g_szLastFormName[256];  //�����W�@������ // cjlee 2019/4/30 �U�� 05:02:41
extern		char		g_szCurrentFormName[256];  //�{�b����

extern 		WORD 		Diag_RomAdd , Diag_RamAdd;
extern		WORD      	Diag_InAdd ,Diag_OutAdd;
// for dsp55 
extern		DWORD		dwDiag_RamAdd;

//	�ҷž�
extern 		WORD 		Diag_RomAdd2 , Diag_RamAdd2;
extern		WORD      	Diag_InAdd2 ,Diag_OutAdd2;

// for dsp55
extern		DWORD		dwDiag_RamAdd2;
extern		WORD		DataAddress;
extern		WORD		DataAddress2;
extern 		BOOL 		g_WarmStartFlag;
extern 		BOOL 		g_bOPSumPass;
extern		int			g_QualityCurveIndex;	//�~�覱�uindex���аO.
extern	    WORD		g_WndLoginPrivildge; //Sunny<20070402>��e�n�X�ݪ��e���n�D���v��
extern	    BOOL		g_WndLoginFlag;//��e�e���O�_����X��
extern	    int			g_iChannelNO ; //��e�q�D��

extern		BOOL		g_bAction;
extern		WORD		g_wMMCStatus ;	// For statusbar0.cpp to check if usb/mmc was mounted done.<yuc080118>
extern		WORD		g_wUSBStatus ;
extern		BOOL		g_bAmmeterStatus;
extern		BOOL		g_bMsgBoxFlag;  //Sunny<20080214> �O��Msgbox���X�ɬO�_key�U�T�{��
extern		BOOL		g_bCancelInstall;//Sunny<20080828>
extern		char		g_szLoginRequest[32];//	fans add 2009/5/13 05:56�U��
extern		DB_MOLDHDR 	g_MoldHead;
//extern		DB_MOLDHDR 	SortModlData[MAX_MOLD_CNT];

//	fans add 2009-8-10 9:36:55
extern		CIniFile	g_StateIni;

extern	  int							g_nblock_no;								//CNC�[�u�{���`��`	//Mario debug
extern	  CNC_BLOCK_DATA	g_CNC_block_data[4]; // 0:last, 1:current, 2:next, 3:queue		//Mario debug
extern	  int							g_nMetricOrInch_System;		//Mario debug
extern		tmGPSData	g_GPSData;

extern  	char 						g_szProgDirectory[128];//Mario debug
extern		WORD	g_wLoginFlag ;		//wwj	2010/8/19 12:01�U��
extern		char	g_szLoginName[32];		//wwj	2010/8/19 12:01�U��
extern		char	g_szLogoutName[32];		//wwj	2010/8/19 12:01�U��

extern		INETDBLIMIT* 	g_piNetLimit; 		//iNetLimit    Caozz add 2010/8/5 06:11�U��
extern		BOOL			g_biNetLimit;
extern		WORD			g_wiNetLimitDBNum;


//New add by Roy��
extern		int			g_sOperate_FileName;		//�����ϥΪ̾ާ@���ɮצW��
//20140613��
extern		int				g_nFileProcessType;					//�P�_�ثe�n�I�s���ӥ\�઺�p����
//20140623��
extern			int			g_wTeachPointNo;						//�ϥΪ̱��d�ݪ����I�s��    2014/9/5 M:�אּ�ϥΪ̫��w�����I�s���A(�Ω�R��.�s��.�d�ݵ���)
//20140709��

//New add by Mario��
extern		int				g_nTeachPointType;						//�P�_�ثe�n�I�s���ӥ\�઺�о��I�p����
//20140910��
//extern		int				g_UnitType;										//�P�_�ثe����쫬�A(0=����,1=�^��)
extern		double		g_Metric_Imperial_Unit ;			//���^�����(1 inch�^�o = 25.4 mm�@��)
//20140915  ��
extern		char 			g_Now_FileName[64];						//�]over.txt�^�C�����в��ʬҰO�U�ثe�ҫ����ɮ�
//20140923��
extern		BOOL			g_Pass;							//�P�_�O�_���T�K�X
//20140926
extern		CtmWnd*		Now_Push_Btn;				//�����{�b���U�����s(toolbar)�ΥH�Ϥ��B�_
//20140930
extern		int 			g_NowRunStatus;						//����:0		����:	1			�Ȱ�:	2
extern		char 			g_Now_Edit_FileName[192];				//�s�褤���ɮ�
//extern		int				g_bMsgBoxThreeSel;					//�O�UMAS�ҿ諸�ﶵ(3�ؿ�ܮɨϥ�)
//20141007
extern		int			g_MoveCommandMode;						//���ʫ��O���Ҧ�
//20141104
extern		int		RobotStatus;				//���e�������A
//2015/3/5 �U�� 04:54:31
extern		int		Now_HMIStatus;			//��ڤW�����A
//2015/3/20 �U�� 04:19:58
extern		CtmWnd* g_statusbar ;
//2015/5/7 �W�� 11:28:11
extern		CtmWnd*	g_Last_Tool_Btn;				//�����۰ʼҦ����W�@�Ӱ��檬�A���s���w
//2015/5/13 �W�� 09:49:59

extern	DWORD	g_dwInetSendCMD;
extern	DWORD	g_dwInetRevCMD;
extern	WORD	g_wInetMsgDataIDCount;
extern	DWORD	g_dwInetMsgDataIDList[512];

int 	cmp_dword( const void *a , const void *b);

extern    char	  *RevertCommand;				//���O�٭쬰��ܬO�����ɨϥ�
//2015/7/28 �W�� 08:06:44
extern		int		Sec5Timer;			//Sec5Prompt�p�ɾ�
//2015/12/30 �U�� 03:17:40
extern		char  NowCommandType;    //0:No 1:JOG  2:Auto
//2016/1/6 �W�� 08:26:19
extern		int  Ethernet_Break;
//2016/5/6 �W�� 10:14:52  
extern		NC_Ethernet_Prog FileEthernetData;
//2016/5/9 �U�� 04:44:17
extern		short  Toolbar_MenuIndex[3];
extern		Robot_Config  cfg;
extern		int 	Gpio_Input;
extern		int 	KeyMode;

// ���F�Ҧ� �ֳt�о�
typedef struct QTEACH_ACTION_P{     //�w�q
     int Step;
     int Type;
     int Num;
     int P1;
     int P2;
     int P3;
     int P4;
     int P5;
}QTEACH_ACTION_P;
extern		QTEACH_ACTION_P g_QTeach_Action_P[100]; // �ֳt�о� �ʧ@�{���Ȯɦs cjlee add 2019/6/28 �W�� 08:42:07
extern		BOOL g_Hint_Download; // �����U�� 0-�U������ 1-�ݭn�U��
extern		int g_Pile_Select; // �O�_����ܰ��| n=��n�հ��| cjlee add 2019/7/8 �U�� 03:49:27
extern		BOOL g_bMute; // �����R�� FALSE-���䭵 TRUE-�R�� cjlee add 2019/9/12 �U�� 03:14:41
extern		BOOL g_AutoMode; // �����R�� FALSE-���䭵 TRUE-�R�� cjlee add 2019/9/12 �U�� 03:14:41
extern		int	g_DBVale_497; // Database �ƭ� �ܱо� cjlee 2020/4/1 �W�� 10:50:20
extern		int	g_DBVale_28; // Database �ƭ� ��� cjlee 2020/4/1 �W�� 10:50:17
extern		int g_DBPrecision; // Database �p���I��� cjlee 2020/4/1 �U�� 01:41:05
extern		char  gstr_DBValue_497[256]; // Database �ƭ� cjlee 2020/4/9 �U�� 03:02:21
extern		char  gstr_DBValue_28[256]; // Database �ƭ� cjlee 2020/4/9 �U�� 03:02:21
extern		int  gQTeach_PosLimt[6][2]; // �ֳt�о� ��m�I�W�U��[�b1-5][MIN-0;MAX-1] cjlee 2020/5/25 �W�� 10:48:53
#endif
