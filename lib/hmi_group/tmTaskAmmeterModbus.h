#ifndef		D__TMTASKAMMETERMODBUS
#define		D__TMTASKAMMETERMODBUS

#include	"../../taskammeter.h"

#define	MODBUS_RTU_FRAME_MAXLEN				256

#define	MODBUS_CODE_READ_HOLDREGISTER		0x03
#define	MODBUS_CODE_READ_INPUTREGISTER		0x04
#define	MODBUS_CODE_WRITE_SINGLE_REGISTER	0x06
#define	MODBUS_CODE_WRITE_REGISTERS			0x10

#define	MODBUS_CODE_ERR_FLAG				0x80

#define	MODBUS_TYPE_NONE					0
#define	MODBUS_TYPE_ENERGY					1
#define	MODBUS_TYPE_VOLTAGE					2
#define	MODBUS_TYPE_CURRENT					3
#define	MODBUS_TYPE_POWER					4
#define	MODBUS_TYPE_CT_R					5
#define	MODBUS_TYPE_CT_W					6


#define	AMMETER_CONFIG_VERSION  		0x00000001
#define	AMMETER_CONFIG_PATH  			"/conf/ModAmmeterConf"
#define	AMMETER_MAX_NUM  				8		/* ammeter max num 8 */


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
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	AMMETERCONFIG;
#else 
#ifdef	D_X86
AMMETERCONFIG;
#endif
#endif

typedef struct tagAmmeterPara_Modbus
{
	DWORD	dwEnergy;
	int		nVoltage;
	int		nCurrent;
	int		nTotalPower;
	WORD	wCT;
} AmmeterPara_Modbus;


class  CtmTaskAmmeterModbus: public CTaskAmmeter
{
	DECLARE_DYNCREATE(CtmTaskAmmeterModbus)
	
	private:
		int				m_nErrCount;
		BYTE			m_readbuff[MODBUS_RTU_FRAME_MAXLEN];
		BYTE			m_sendbuff[MODBUS_RTU_FRAME_MAXLEN];
		long			m_lCheckTimer;
		int				m_nRegType;
		WORD 			wTemp;
		BOOL			m_bReadConfFlag;
		
		AmmeterPara_Modbus m_para;
		tmRS485AMMETER	m_RS485Ammeter;
		static AMMETERCONFIG	m_AmmeterConf;
	public:
		CtmTaskAmmeterModbus();
		~CtmTaskAmmeterModbus();
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
	
	public:	
		int		ReadInt(int nType, WORD wAmmeterNo = 0);
		double	ReadDouble(int nType, WORD wAmmeterNo = 0);
		float	ReadFloat(int nType, WORD wAmmeterNo = 0);
		
		void* 	ReadConf();
		int 	WriteConf(void* pConf);
    protected:
    	int 	Parse(void *pData, int nLen);
    	int 	SendData(void *pData, int nLen);

		int		Request(int nType, WORD wAmmeterNo = 0);
		int		Write(int nType, void *pData, int nLen);
    	int 	ReadHoldRegister(BYTE slaveID, WORD wStartAddr, WORD wNum);
    	int		WriteSingleRegister(BYTE slaveID, WORD wStartAddr, WORD wValue);
    	int 	WriteRegisters(BYTE slaveID, WORD wStartAddr, int nNum, WORD* wValue);
    private:
    	void	ResReadHoldRegister(void *pData, int nLen);
    	
    	void	SaveEnergy(void *pData, int nLen);
    	void	SaveVoltage(void *pData, int nLen);
    	void	SaveCurrent(void *pData, int nLen);
    	void	SaveTotalPower(void *pData, int nLen);
    	void	SaveCT(void *pData, int nLen, int index);
};


#ifndef	D_BEOBJECT_CTMTASKAMMETERDLT	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

