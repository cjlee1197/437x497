#ifndef		D__TMTASKAMMETERTM
#define		D__TMTASKAMMETERTM
#include	"../../taskammeter.h"
#include	"../../app.h"
#include	"../../main.h"
#include	"../../utils.h"
#include	"../libControl.h"
#include	"../../commonaction.h"
#include	"../../tmshmsg.h"


#define	AMMETER_FRAME_START_FLAG	   	0x68		/* frame start flag 0x68 */
#define	AMMETER_FRAME_END_FLAG	   	   	0x16		/* frame end flag 0x16 */
#define	AMMETER_FRAME_WAKEUP_FLAG   	0xFE		/* wake up flag 0xFE */
#define	AMMETER_FRAME_DATAADD	   		0x33  		/* data added 0x33 */
#define	AMMETER_FRAME_BROADCAST			0x99		/* broadcast addr */
#define	AMMETER_FRAME_ADDR_LENGTH   	6			/* address len 6 */
#define	AMMETER_SENDDATA_MAXLENGTH 		50			/* master send data len <= 50 */
#define	AMMETER_RECVDATA_MAXLENGTH 		200			/* master read data len <= 200 */
#define	AMMETER_SEGMENT					9
#define	AMMETER_COUNT					5


#define	AMMETER_CONTROL_RESPONSE		0x80		/* control code response - bit7 */
#define	AMMETER_CONTROL_ERROR			0x40		/* control code response error - bit6 */
#define	AMMETER_CONTROL_NEXTFRAME		0x20		/* control code follow-up frame - bit5 */

#define	AMMETER_CONTROL_READ			0x01		/* control code read data */
#define	AMMETER_CONTROL_FOLLOW			0x02		/* control code read follow-up data */
#define	AMMETER_CONTROL_REREAD			0x03		/* control code re-read pre-frame */
#define	AMMETER_CONTROL_WRITE			0x04		/* control code write data */
#define	AMMETER_CONTROL_TIME			0x08		/* control code broadcast adjusting time */
#define	AMMETER_CONTROL_ADDR			0x0A		/* control code write device's address */
#define	AMMETER_CONTROL_BITRATE			0x0C		/* control code change bit-rate */
#define	AMMETER_CONTROL_PASSWORD		0x0F		/* control code change password */
#define	AMMETER_CONTROL_ZERO			0x10		/* control code maximum zero clearing */
#define	AMMETER_CONTROL_MAX				16

#define	AMMETER_REQ_ENERGY				0x9010
#define	AMMETER_REQ_VOLTAGE_A			0xB611
#define	AMMETER_REQ_VOLTAGE_B			0xB612
#define AMMETER_REQ_VOLTAGE_C			0xB613
#define	AMMETER_REQ_VOLTAGE				0xB61F
#define	AMMETER_REQ_CURRENT_A			0xB621
#define	AMMETER_REQ_CURRENT_B			0xB622
#define	AMMETER_REQ_CURRENT_C			0xB623
#define	AMMETER_REQ_CURRENT				0xB62F
#define	AMMETER_REQ_TOTAL_POWER			0xB630

#define	AMMETER_REQ_ENERGY				0x9010
#define	AMMETER_REQ_VOLTAGE_A			0xB611
#define	AMMETER_REQ_VOLTAGE_B			0xB612
#define AMMETER_REQ_VOLTAGE_C			0xB613
#define	AMMETER_REQ_VOLTAGE				0xB61F
#define	AMMETER_REQ_CURRENT_A			0xB621
#define	AMMETER_REQ_CURRENT_B			0xB622
#define	AMMETER_REQ_CURRENT_C			0xB623
#define	AMMETER_REQ_CURRENT				0xB62F
#define	AMMETER_REQ_TOTAL_POWER			0xB630

#define	AMMETER_ERRORCNT				3

#define	BUFF_SIZE	(AMMETER_RECVDATA_MAXLENGTH + sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER))
#define	SINGLE_DATA_LEN		6


#define	AMMETER_CONFIG_VERSION  		0x00000001
#define	AMMETER_CONFIG_PATH  			"/conf/TMAmmeterConf"
#define	AMMETER_MAX_NUM  				8	/* ammeter max num 8 */

typedef struct tagAMMETERINFO
{
	WORD 	wNode;
	WORD 	wCTPara;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AMMETERINFO;
#else 
#ifdef	D_X86
AMMETERINFO;
#endif
#endif


typedef struct tagAMMETERCONFIG
{
	WORD 		wCount;
	AMMETERINFO AmmeterInfo[AMMETER_MAX_NUM];
	WORD		N2CVoltage[2][9];			//[2]0:MaxV 1:MinV; [9]Amt1:A,B,C(V);Amt2:A,B,C(V);Amt3:A,B,C(V)
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AMMETERCONFIG;
#else 
#ifdef	D_X86
AMMETERCONFIG;
#endif
#endif

typedef struct tagFRAMEHEADER
{
	BYTE 	starta;
	BYTE 	address[AMMETER_FRAME_ADDR_LENGTH];
	BYTE 	startb;
	BYTE 	control;
	BYTE 	len;
}
#ifdef	D_ARM
__attribute__((packed, aligned(1)))	FRAMEHEADER;
#else 
#ifdef	D_X86
FRAMEHEADER;
#endif
#endif

typedef struct tagFRAMEFOOTER
{
	BYTE 	cs;
	BYTE 	end;
}
#ifdef	D_ARM
__attribute__((packed, aligned(1)))	FRAMEFOOTER;
#else 
#ifdef	D_X86
FRAMEFOOTER;
#endif
#endif

typedef struct tagAmmeterPara
{
	float	fTotalPower;				// mw
	float	fEnergy;					// mw/h   
	float	fTotalVoltage;				// total voltage
	float	fVoltageA;					// v
	float	fVoltageB;					// v
	float	fVoltageC;					// v	
	float	fTotalCurrent;				// total current			
	float	fCurrentA;					// mA
	float	fCurrentB;					// mA
	float	fCurrentC;					// mA
	float	fVoltageAB;					// v A-B
	float	fVoltageBC;					// v B-C
	float	fVoltageAC;					// v A-C
	DWORD	dwDataVer;
	DWORD	dwPhaseDeviation;
	float	fPowerFactor;	
} AmmeterPara;

typedef struct tagAMTERROR			//rw.ying
{
	WORD 	wCount;
	WORD	wTotalNum;
	WORD	wFlag;
}
#ifdef	D_ARM
__attribute__((packed, aligned(1)))	AMTERROR;
#else 
#ifdef	D_X86
AMTERROR;
#endif
#endif

class  CtmTaskAmmeterTM: public CTaskAmmeter
{
	DECLARE_DYNCREATE(CtmTaskAmmeterTM)
	
	private:
		int				m_nErrCount;

		BYTE			m_readbuff[BUFF_SIZE];
		BYTE			m_sendbuff[BUFF_SIZE];
		BYTE			m_address[AMMETER_FRAME_ADDR_LENGTH];
		BYTE			m_wakeup[4];
		long			m_lCheckTimer;
		float			m_CTPara;
		
		AMTERROR		m_AMTError[AMMETER_SEGMENT];
		DWORD			m_dwAmtImpedance[AMMETER_SEGMENT][AMMETER_COUNT];
		DWORD			m_dwAmtCount;
		WORD			m_wNode[AMMETER_RS485ID_COUNT];
		BYTE			m_cCurIndex;
		BYTE			m_cOldIndex;
		BOOL			m_bAutoCheckAmt;
		WORD			m_wLineTime[AMMETER_RS485ID_COUNT];
		DWORD			m_dwConfigV[AMMETER_RS485ID_COUNT];
		AmmeterPara		m_para[AMMETER_RS485ID_COUNT];
		static AMMETERCONFIG	m_AmmeterConf;
		
		DWORD			m_dwCurImpedance[AMMETER_SEGMENT];
		DWORD			m_dwOldImpedance[AMMETER_SEGMENT];
		DWORD			m_dwCheckImpedance[AMMETER_SEGMENT];
		BOOL			m_bCheckFlag;
		WORD			m_wAmtRate;
		WORD			m_wVoltageV[2][AMMETER_SEGMENT];
		DWORD			m_dwSetAmtCount;
		WORD			m_wAmtCTRate[AMMETER_RS485ID_COUNT];
		
		tmRS485AMMETER	m_RS485Ammeter;
	public:
		CtmTaskAmmeterTM();
		~CtmTaskAmmeterTM();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
		
	public:
		int		Request(int nType, WORD wAmmeterNo = 0);
		int		ReadInt(int nType, WORD wAmmeterNo = 0);
		double	ReadDouble(int nType, WORD wAmmeterNo = 0);
		float	ReadFloat(int nType, WORD wAmmeterNo = 0);
		
		void* 	ReadConf();
    	int 	WriteConf(void* pConf);
    protected:
		int 	Request(BYTE bCode, WORD wID = 0, int nLen = 0, BYTE *pData = NULL);
		int		Request(BYTE bCode, WORD* pwID, WORD wElement, WORD wIndex);
    	int 	ReqRead(WORD wID);
    	int 	ReqRead(WORD* pwID, WORD wElement, WORD wIndex);
    	int 	ReqFollowData(WORD wID);
    	int 	ReqReRead();
    	int		Write(int nType, void *pData, int nLen);
    	int 	ReqWrite(WORD wID, void *pBuf, int nLen);
    	int		ReqAdjustTime();
    	int 	ReqWriteAddr(WORD wID, void *pAddr);
    	int 	ReqChangeRate(WORD wID, BYTE bRate);
    	void 	ResRead(void *pData, int nLen);
    	
    	int 	SendData(void *pData, int nLen);
    	int 	Parse(void *pData, int nLen);
    private:
    	void 	SetHeader(BYTE bControl, int nLen, FRAMEHEADER *pHeader, WORD wIndex =0);
    	void 	SetFooter(FRAMEHEADER *pHeader);
    	int 	CheckFrame(BYTE *pFrame);
    	BOOL	m_bReadConfFlag;
    	void	SaveEnergy(void *pData, int nLen);
    	void	SaveTotalPower(void *pData, int nLen);
    	void	SaveVoltageA(void *pData, int nLen);
    	void	SaveVoltageB(void *pData, int nLen);
    	void	SaveVoltageC(void *pData, int nLen);
    	void	SaveCurrentA(void *pData, int nLen);
    	void	SaveCurrentB(void *pData, int nLen);
    	void	SaveCurrentC(void *pData, int nLen);
    	void	SaveVoltageAB(void *pData, int nLen);
    	void	SaveVoltageBC(void *pData, int nLen);
    	void	SaveVoltageAC(void *pData, int nLen);
    	void	SaveDataVersion(void *pData, int nLen);
    	void	SavePhaseDeviation(void *pData, int nLen);
    	void	SavePowerFactor(void *pData, int nLen);
    	int		BCD2Int(void *pData, int nLen);
    	double	BCD2Double(void *pData, int nLen, int nPrecision);
    	float	DATA2FLOAT(void *pData, int nLen);
    	
    	void	SetAMTNode();
    	int		GetAmtCurNum(int nNode);
    	void	DealAmtErrlog(int nTmpIndex, int nIndx);
    	void	AmtSetAutoCheckFlag(BOOL bFlage) {m_bAutoCheckAmt = bFlage;m_cOldIndex = m_cCurIndex;}
    	void	SetAmtAutoCheck(DWORD	dwValue, int nSegment);
    	void	SaveAmtCheckV();
    	DWORD	GetAmtOnLineS();
    	DWORD*	GetCheckImpedance()  {return m_dwCheckImpedance;}
    	DWORD*	GetCurImpedance()	{return m_dwCurImpedance;}
    	DWORD*	GetOldImpedance()	{return m_dwOldImpedance;}
    	WORD	GetAmtCount()	{return m_dwSetAmtCount;}
    	void	DearlErrorVoltage(int	nDataType, int nAmtIndex, void* pData);
    	void	DearlErrorDeviation(int nAmtIndex, int nValue);
    
};


#ifndef	D_BEOBJECT_CTMTASKAMMETERTM	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

