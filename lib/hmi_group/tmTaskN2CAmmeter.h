#ifndef		D__TMTASKN2CAMMETER
#define		D__TMTASKN2CAMMETER
#include	"../../taskammeter.h"

#define	AMMETER_CONTROL_READ			0x01		/* control code read data */
#define	AMMETER_CONTROL_WRITE			0x02		/* control code write data */
#define	AMMETER_CONTROL_ACTUALVALUE		0x03		/* control code ammeter ActualValue */

#define	N2C_AMMETER_MAXNUM				32

#define	N2C_AMMETER_DATA				0xB000

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
	DWORD	dwDataVer;					// Amt Ver
	DWORD	dwPhaseDeviation;			// Deviation
	float	fPowerFactor;				// Factor
	float	fPowerA;					// v A-B
	float	fPowerB;					// v B-C
	float	fPowerC;					// v A-C
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AmmeterPara;
#else 
#ifdef	D_X86
AmmeterPara;
#endif
#endif


typedef struct tagAmmeterHeader
{
	WORD	wCycleTime;
	WORD	wCmd;
	WORD	wElementCount;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AmmeterHeader;
#else 
#ifdef	D_X86
AmmeterHeader;
#endif
#endif


#define	AMMETER_CONFIG_VERSION  		0x00000001
#define	AMMETER_CONFIG_PATH  			"/conf/N2CAmmeterConf"
#define	AMMETER_MAX_NUM  				8			/* ammeter max num 10 */
#define	AMMETER_N2CNUM					3			//Rw.ying	

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


class  CtmTaskN2CAmmeter: public CTaskAmmeter
{
	DECLARE_DYNCREATE(CtmTaskN2CAmmeter)
	
	private:
		int				m_nErrCount;

		long			m_lCheckTimer;
		float			m_CTPara;
		
		BYTE			m_sendbuff[4096];
		BYTE			m_bMachineID;
		
		AmmeterHeader	m_Head;
		AmmeterPara		m_para[N2C_AMMETER_MAXNUM];
		
		static AMMETERCONFIG	m_AmmeterConf;
	public:
		CtmTaskN2CAmmeter();
		~CtmTaskN2CAmmeter();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		//void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		//====================
		//	override the function that are defined in the CTask
		//====================
		void    Run();
		int		m_nDataState;
		
	public:
		int		Request(int nType, WORD wAmmeterNo = 0);
		int		ReadInt(int nType, WORD wAmmeterNo = 0);
		double	ReadDouble(int nType, WORD wAmmeterNo = 0);
		float	ReadFloat(int nType, WORD wAmmeterNo = 0);
		void* 	ReadConf();
    	int 	WriteConf(void* pConf);
    protected:
		int 	Request(BYTE bCode, WORD wID = 0, int nLen = 0, BYTE *pData = NULL, WORD wAmmeterNo = 0);
		int		Request(BYTE bCode, WORD* pwID, WORD wElement, WORD wAmmeterNo = 0);
    	int 	ReqRead(WORD wID);
    	int 	ReqRead(BYTE bCode, WORD* pwID, WORD wElement);
    	int 	ReqWrite(WORD wID, void *pBuf, int nLen);
    	int		Write(int nType, void *pData, int nLen);
    	
    	int 	ParseData(void *pData, int nLen);
    private:
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
    	void	SavePowerA(void *pData, int nLen);
    	void	SavePowerB(void *pData, int nLen);
    	void	SavePowerC(void *pData, int nLen);
    	void	SavePhaseDeviation(void *pData, int nLen);
    	void	SavePowerFactor(void *pData, int nLen);
    	void	SaveDataVersion(void *pData, int nLen);
    	int		BCD2Int(void *pData, int nLen);
    	double	BCD2Double(void *pData, int nLen, int nPrecision);
    	float	DATA2FLOAT(void *pData, int nLen);
    	void	DearlErrorVoltage(int	nDataType, int nAmtIndex, void* pData);
    	void	DearlErrorDeviation(int  nAmtIndex, int nValue);
    	//void	DearlErrorPower(int	nDataType, int nAmtIndex, void* pData);
    	
};


#ifndef	D_BEOBJECT_CTMTASKN2CAMMETER	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

