#ifndef		D__TMTASKAMMETERDLT
#define		D__TMTASKAMMETERDLT
#include	"../../taskammeter.h"

#define	AMMETER_FRAME_START_FLAG	   	0x68		/* frame start flag 0x68 */
#define	AMMETER_FRAME_END_FLAG	   	   	0x16		/* frame end flag 0x16 */
#define	AMMETER_FRAME_WAKEUP_FLAG   	0xFE		/* wake up flag 0xFE */
#define	AMMETER_FRAME_DATAADD	   		0x33  		/* data added 0x33 */
#define	AMMETER_FRAME_BROADCAST			0x99		/* broadcast addr */
#define	AMMETER_FRAME_ADDR_LENGTH   	6			/* address len 6 */
#define	AMMETER_SENDDATA_MAXLENGTH 		50			/* master send data len <= 50 */
#define	AMMETER_RECVDATA_MAXLENGTH 		200			/* master read data len <= 200 */


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

#define	BUFF_SIZE	(AMMETER_RECVDATA_MAXLENGTH + sizeof(FRAMEHEADER) + sizeof(FRAMEFOOTER))

#define	AMMETER_CONFIG_VERSION  		0x00000001
#define	AMMETER_CONFIG_PATH  			"/conf/AmmeterConf"
#define	AMMETER_MAX_NUM  				8			/* ammeter max num 8 */

typedef struct tagAMMETERINFO
{
	BYTE 	address[AMMETER_FRAME_ADDR_LENGTH];
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
	BYTE 	address[AMMETER_FRAME_ADDR_LENGTH];
	double	dTotalPower;
	double	dEnergy;
	int		nVoltageA;
	int		nVoltageB;
	int		nVoltageC;
	double	dCurrentA;
	double	dCurrentB;
	double	dCurrentC;
} AmmeterPara;

class  CtmTaskAmmeterDLT: public CTaskAmmeter
{
	DECLARE_DYNCREATE(CtmTaskAmmeterDLT)
	
	private:
		int				m_nErrCount;
		
		int 			m_nSendCount[AMMETER_MAX_NUM];

		BYTE			m_readbuff[BUFF_SIZE];
		BYTE			m_sendbuff[BUFF_SIZE];
		BYTE			m_address[AMMETER_FRAME_ADDR_LENGTH];
		BYTE			m_wakeup[2];
		long			m_lCheckTimer;
		AmmeterPara		m_para[AMMETER_MAX_NUM + 1];
		tmRS485AMMETER	m_RS485Ammeter;
		
		WORD			m_SendAmmeter;
		WORD			m_ParseAmmeter;
		
		BOOL			m_bReadConfFlag;
		BOOL			m_bAmmeterConfFlag;
		static AMMETERCONFIG	m_AmmeterConf;
		
	public:
		CtmTaskAmmeterDLT();
		~CtmTaskAmmeterDLT();
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
		int 	Request(BYTE bCode, WORD wID = 0, int nLen = 0, BYTE *pData = NULL, WORD wAmmeterNo = 0);
		int		ReadInt(int nType, WORD wAmmeterNo = 0);
		double	ReadDouble(int nType, WORD wAmmeterNo = 0);
		
		void* 	ReadConf();
    	int 	WriteConf(void* pConf);
    protected:
		//int 	Request(BYTE bCode, WORD wID = 0, int nLen = 0, BYTE *pData = NULL);
    	int 	ReqRead(WORD wID);
    	int 	ReqFollowData(WORD wID);
    	int 	ReqReRead();
    	int 	ReqWrite(WORD wID, void *pBuf, int nLen, WORD wAmmeterNo = 0);
    	int		ReqAdjustTime();
    	int 	ReqWriteAddr(WORD wID, void *pAddr);
    	int 	ReqChangeRate(WORD wID, BYTE bRate);
    	void 	ResRead(void *pData, int nLen, WORD wAmmeterNo = 0);
    	
    	int 	SendData(void *pData, int nLen);
    	int 	Parse(void *pData, int nLen);
    private:
    	void 	SetHeader(BYTE bControl, int nLen, FRAMEHEADER *pHeader);
    	void 	SetFooter(FRAMEHEADER *pHeader);
    	int 	CheckFrame(BYTE *pFrame);
    	UTIME	GetNowTime();
    	UTIME	MakeTime(tmDATETIME *tm);
    	void	SaveEnergy(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveTotalPower(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveVoltageA(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveVoltageB(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveVoltageC(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveCurrentA(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveCurrentB(void *pData, int nLen, WORD wAmmeterNo = 0);
    	void	SaveCurrentC(void *pData, int nLen, WORD wAmmeterNo = 0);
    	
    	int		BCD2Int(void *pData, int nLen);
    	double	BCD2Double(void *pData, int nLen, int nPrecision);
    
};


#ifndef	D_BEOBJECT_CTMTASKAMMETERDLT	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

