/*==========================================================================+
|  Class    : Simotion communication          								|
|  Task     : Simotion communication header file                     		|
|---------------------------------------------------------------------------|
|  Compile  :                                                               |
|  Link     :                                                               |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : zholy                                                    		|
|  Version  : V1.00                                                         |
|  Creation : 2006/12/17                                                    |
|  Revision : 1.0                                                           |
+==========================================================================*/
#ifndef 	D_KEBMOTOR
#define		D_KEBMOTOR


#include	"taskcomm.h"
#include 	"common.h"
#include	"ethnet.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		KEB_QUEUE_COUNT						50
#define		KEB_INVERTER_ADDR					0x00
#define		MAXRANK_INJECT						6
#define		SIZEOF_TKEBW_S01PARA				19
#define		SIZEOF_TKEBR_MASKDATA				10


//===========
//           KEB Data type length
//===========
#define		KEB_LEN_ADDR						2
#define		KEB_LEN_MASK						2
#define		KEB_LEN_CMD							4
#define		KEB_LEN_WORD						4
#define		KEB_LEN_DWORD						8

#define		KEBDATA_LENGTH						26
#define		KEBPACKET_LENGTH					30
#define		KEBBUFF_LENGTH						2048

//===========
//           KEB USE ASCII Code
//===========
#define		CTRLCHAR_STX						0x02
#define		CTRLCHAR_ETX						0x03
#define		CTRLCHAR_EOT						0x04
#define		CTRLCHAR_ENQ						0x05
#define		CTRLCHAR_ACK						0x06
#define		CTRLCHAR_NAK						0x15

//===========
//           KEB CMD ID
//===========
#define		KEB_RW_PARA							0x47		// 'G' or 0x48 'H'
#define		KEB_RW_PARALIMIT					0x49		// 'I'
#define		KEB_RW_DEFAULT						0x4a		// 'J'
#define		KEB_RW_CHARACTER					0x4b		// 'K'
#define		KEB_RW_PARANAME						0x4d		// 'M'
#define		KEB_RW_SCOPE						0x59		// 'Y'
#define		KEB_SET_DEFAULT						0x67		// 'g' or 0x68 'h'
#define		KEB_CHECK_DEFAULT					0x68		// 'h' or 0x67 'g'

//===========
//          Read Value IID
//===========
#define		IID_STAT							0x47
#define		IID_POS								0x46
#define		IID_SPD								0x45
#define		IID_PRES							0x44
#define		IID_SCOPE							0x43

//===========
//          KEB Adress List
//===========
#define		ADDR_INVENTERSTAT					0x0200	// ru00
#define		ADDR_CURPOSI						0x0236	// ru56
#define		ADDR_CURSPED						0x0209	// ru09
#define		ADDR_CURPRES						0x021E  // ru30
#define		ADDR_SCOPE1							0x0021	// sy33
#define		ADDR_SCOPE2							0x0023	// sy35
#define		ADDR_SCOPE3							0x0025	// sy37
#define		ADDR_SCOPE4							0x0027	// sy39

#define		ADDR_SELTINDEX						0x1317	// ps23
#define		ADDR_INDEXPOSI						0x1318	// ps24
#define		ADDR_INDEXSPED						0x1319 	// ps25
#define		ADDR_NEXTINDEX						0x131A	// ps26
#define		ADDR_INDEXMODE						0x131B	// ps27
#define		ADDR_STRTINDEX						0x131C	// ps28
#define		ADDR_SETPRESS						0x0703  // cn03
#define		ADDR_SETTORQUE						0x0F12	// cs18
#define		ADDR_SETLOWSPD						0x0305  // op05
#define		ADDR_SETERROR						0x0C07	// do07
#define		ADDR_RESETPOSI						0x0C05	// do05
                                                
#define		ADDR_CONTROLCMD						0x002B	// sy43
#define		ADDR_SET							0x0909	// fr09

//===========
//          KEB Command List
//===========
#define		KEBCMD_INJECTON						9269
#define		KEBCMD_ADJINJECTON					8261
#define		KEBCMD_SUCKBACKON					9333
#define		KEBCMD_ADJSUCKBACKON				8293
#define		KEBCMD_HOLDON						8213
#define		KEBCMD_BACKON						8229

#define		KEBCMD_STOP							8197

//===========
//          Operate switch Flag
//===========
#define		KEBCURVE_ON							0x0001	// on
#define		KEBCURVE_OPING						0x0002	// operating
#define		KEBCURVE_OFF						0x0004	// off

//===========
//       	DSP TO KEB OPS CODE
//===========
#define		KEBOP_INJECT						0x0008
#define		KEBOP_CHARGE						0x0020
#define		KEBOP_SUCKBACK						0x0010
#define		KEBOP_HOLD							(KEBOP_INJECT | 0x4000)
#define		KEBOP								(KEBOP_INJECT | KEBOP_CHARGE | KEBOP_HOLD)
#define		KEBOP_NULL							0x0000

//===========
//       	KEB TO DSP OPS CODE
//===========
#define		KEBCOMMDSP_INJECT					0x0100
#define		KEBCOMMDSP_HOLD						0x0200
#define		KEBCOMMDSP_SUCKBACK					0x0400
#define		KEBCOMMDSP							(KEBCOMMDSP_INJECT | KEBCOMMDSP_HOLD | KEBCOMMDSP_SUCKBACK)


/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef void (* pCURVEFunc)(WORD);

typedef struct tagTKEBR_PARA
{
	char	cIndex[KEB_LEN_CMD];
	BYTE	cENQ;
} TKEBR_PARA;

typedef struct tagTKEBW_S01PARA
{
	BYTE	cSTX;
	BYTE	cSID;
	BYTE	cIID;
	char	cIndex[KEB_LEN_CMD];
	char	cData[KEB_LEN_DWORD];
	char	cMask[KEB_LEN_MASK];
	BYTE	cETX;
	BYTE	cBCC;
} TKEBW_S01PARA;

typedef	struct tagTKEBR_MASKDATA
{
	BYTE	cSID;
	BYTE	cIID;
	char	cIndex[KEB_LEN_CMD];
	char	cMask[KEB_LEN_MASK];
	BYTE	cENQ;
	BYTE	cBCC;
} TKEBR_MASKDATA;

typedef struct tagTKEBR_PD
{
	BYTE	cSID;
	BYTE	cIID;
	BYTE	cENQ;
	BYTE	cBCC;
} TKEBR_PD;
//---------------------------------------------------
typedef union tagFKEB_ANSWINQUIRY
{
	BYTE	cAnsw;
	BYTE	cACK;
	BYTE	cNAK;
} FKEB_ANSWINQUIRY;									// (size = 1) cAnsw = ACK or NAK

typedef struct tagFKEB_ERROR
{
	char	cEC;
	BYTE	cEnd;									// (size = 2) cEnd = NAK or EOT
} FKEB_ERROR;

typedef struct tagFKEB_IIDACK
{
	BYTE	cIID;
	BYTE	cACK;
} FKEB_IIDACK;										// (size = 2)

typedef struct tagFKEB_SERVERROR
{
	BYTE	cIID;
	char	cEC;
	BYTE	cEnd;
} FKEB_SERVERROR;									// (sizeof = 3) cEnd = NAK or EOT

typedef struct tagFKEB_ONEPARA
{
	BYTE	cSTX;
	char	cIndex[KEB_LEN_CMD];
	char	cData[KEB_LEN_WORD];
	BYTE	cETX;
	BYTE	cBCC;
} FKEB_ONEPARA;										// (size = 11)

typedef struct tagFKEB_SONEDATA
{
	BYTE	cSTX;
	BYTE	cSID;
	BYTE	cIID;
	char	cData[KEB_LEN_DWORD];
	BYTE	cETX;
	BYTE	cBCC;
} FKEB_SONEDATA;									// (size = 13)

typedef struct tagFKEB_STWODATA
{
	BYTE	cSTX;
	BYTE	cSID;
	BYTE	cIID;
	char	cData1[KEB_LEN_DWORD];
	char	cData2[KEB_LEN_DWORD];
	BYTE	cETX;
	BYTE	cBCC;
} FKEB_STWODATA;									// (size = 21)

typedef struct tagFKEB_SCOPE
{
	BYTE	cSTX;
	BYTE	cSID;
	BYTE	cIID;
	char	cTimeStamp[4];
	char	cData1[KEB_LEN_DWORD];
	char	cData2[KEB_LEN_DWORD];
	char	cData3[KEB_LEN_WORD];
	char	cData4[KEB_LEN_WORD];
	BYTE	cETX;
	BYTE	cBCC;
} FKEB_SCOPE;										// (size = 33)
//===========
//          KEB QUEUE
//===========
typedef	struct tagKEBQUEUE
{
	int		nIn;
	int		nOut;
	int		nFull;
} KEBQUEUE;

typedef struct tagWRITEECHO
{
	BYTE		cIID;
	BOOL		bEcho;
} WRITEECHO;

typedef struct tagREADLIST
{
	WORD		wAddr;
	BYTE		cIID;
} READLIST;
//===========
//          KEB SET
//===========
typedef struct tagSETDB
{
	char			bSetASCII[KEB_LEN_MASK];
	short			wIndexStart;
	short			wIndexEnd;
} SETDB;

//enum KEBCONTROLSET
//{
//	SET0_INIT,
//	SET1_HOLDPRES,
//	SET2_BACKPRES,
//	SET3_INJECTSPD,
//	SET4_LOWSPDINJ,
//	SET5_RESERVED,
//	SET6_LOWSUCKBACK,
//	SET7_SUCKSPD,
//	SET_ALL
//};

enum KEBCONTROLSET//zholy061207
{
	SET0_INIT,
	SET5_RESERVED,
	SET1_HOLDPRES,
	SET2_BACKPRES,
	SET3_INJECTSPD,
	SET4_LOWSPDINJ,
	SET7_SUCKSPD,
	SET6_LOWSUCKBACK,
	SET_ALL
};

//===========
//          KEB RealTime Data
//===========
typedef struct tagREALTIMEDATA
{
	long		lTimerCnt;
	long		lPosi;
	long		lSpeed;
	long		lPress;
	long		lStat;
} REALTIMEDATA;//ÀÊ π”√¿©’π

/*===========================================================================+
|           Class declaration - TaskComm                                     |
+===========================================================================*/
class  CTaskKeb: public CTaskComm
{
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
public:
    CTaskKeb();
    virtual ~CTaskKeb();
    
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
public:
	
private:
	static	int				m_nReadOrder;
	static	int				m_nValueID;
	static	ETHERNET		m_ethernet;
	static	BOOL			m_bNeedEOT;
	static	BOOL			m_bOneOffSend;
	static	BYTE			m_cInvokID;
	static	KEBQUEUE		m_KebQueue;
	static	WRITEECHO		m_WriteEcho;
	static	WORD    		m_wLastKEBOP;
	static	pCURVEFunc		m_pCurveFunc;
	static	BOOL			m_bKebOnline;
	static	long			m_lStepRank;
			
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
public:
	void	Run();
	void	ResetPosition();
	void	ErrorReset();
	void	SendInjectStartIndex();
	void    SendPOS_Index(BYTE cIndex, DWORD dwData);
	void    SendSPD_Index(BYTE cIndex, DWORD dwData);
	void	SendAdjustInjectTorque(DWORD dwData);
	void	SendAdjustSuckBackTorque(DWORD dwData);
	void	SendAdjustInjectSPD(DWORD dwData);
	void	SendAdjustSuckBackSPD(DWORD dwData);
	void	SendMessageToKEB(WORD wOPCode);
	void	SendRank_Inject();
	
/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
private:
	static	BYTE    ExclusiveOR(BYTE *pData, int nLength);
	static	BOOL    CheckBCC(BYTE *pData, int nLength);
	static	BOOL    SendKEBDATA(BYTE *pData, int nDataLen);
	static	int		AddItem(TKEBW_S01PARA *pItem);
	static	int		DelItem(BYTE cIID);
	static	int		ReadItem();
	static	int		SendKEBQueue();
	static	BOOL    Read_KebValue(WORD wAddr);
	static	BOOL	Read_KebValue(WORD wAddr, BYTE cIID, WORD wSet);
	static	BOOL	Read_KebProcessData(BYTE cSID, BYTE cIID);
	static	BOOL    Write_KebValue(WORD wAddr, DWORD dwData, WORD wSet);
	static	void	SetCurve(long lCurCount);
	static	void	GetScopeData(char *pdata, int nlength);
	static	void	AnalysisKEBData(char *pdata, int  nlength);
	static	void	SetHoldCurve(WORD wSwitch);
	static	void	SetBackCurve(WORD wSwitch);
	static	void	SetInjectCurve(WORD wSwitch);
	static	void	HookSetCurve(pCURVEFunc pFunc);
	static	void    UnHookSetCurve();
	static	void	Send_OneOffSendData();
	static	void	SetKEBOnlineState(BOOL bOnline);
};

/*===========================================================================+
|           Variable and Function declaration                                |
+===========================================================================*/
extern	REALTIMEDATA	g_RealData;
extern	long	long	g_lKEBPulse;
extern	long	long	g_lKEBUM;
extern	long	long	g_lKEBRPM;
extern	long	long	g_lKEBMMPerSec;
#endif
