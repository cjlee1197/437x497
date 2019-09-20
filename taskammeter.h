/*===========================================================================+
|  Class    : TaskAmmeter                                                       |
|  Task     : TaskAmmeter Service Routine header file                     |
|----------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                   |
|  Link     : Gcc V3.2.2                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Gerry                                                           |
|  Version  : V1.00                                                          |
|  Creation : 2011/6/2 15:03:30                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_TASKAMMETER
#define     D_TASKAMMETER

#include    "task.h"
#include	"tmpackclass.h"
/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

#define AMMETER_NOTIFY_TOTALPOWER  0x01
/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/
typedef	struct	tagRS485Ammeter
{
	struct termios 	newtermios;
	int				nBaudRate;
	WORD			wChannel;
}
#ifdef	D_ARM
__attribute__((packed, aligned(2)))	tmRS485AMMETER;
#else 
#ifdef	D_X86
tmRS485AMMETER;
#endif
#endif

typedef  void (*NOTIFIERFUNC)(int , void *, int, int);
/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/
//typedef  void (*NOTIFYERFUNC)(int , void *, int);
/*===========================================================================+
|           Class declaration - TaskMoni                                     |
+===========================================================================*/
class CTaskAmmeter : public CTask, public CtmPackClass
{
	DECLARE_DYNCREATE(CTaskAmmeter);
	
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
	public:
	    CTaskAmmeter();
	    ~CTaskAmmeter();
	    
    	int			CreateSelf();
		int			FreeSelf();
/*---------------------------------------------------------------------------+
|           Attributes                                                       |
+---------------------------------------------------------------------------*/
	protected:
		int 			m_fdcom;
		struct termios 	m_termios;
		
		sem_t 			send_sem;
		sem_t 			read_sem;
		
		NOTIFIERFUNC 	m_notifier;
	private:
		pthread_t 		m_RxThread; 
		pthread_attr_t  m_ThreadAttr;
		int				m_nBaudRate;
		
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
	public:
		virtual	 int		Request(int nType, WORD wAmmeterNo = 0) { return 0;};
		virtual	 int		Request(BYTE bCode, WORD wID = 0, int nLen = 0, BYTE *pData = NULL, WORD wAmmeterNo = 0) { return 0;};
		virtual	 int		Write(int nType, void *pData, int nLen) { return 0;};
		virtual	 int		Request(BYTE bCode, WORD* pwID, WORD wElement, WORD wAmmeterNo = 0) {return 0;}
		virtual	 int 		ReqWrite(WORD wID, void *pBuf, int nLen, WORD wAmmeterNo = 0){return 0;}
		virtual  int 		ReadInt(int nType, WORD wAmmeterNo = 0) {return 0;};
		virtual	 double		ReadDouble(int nType, WORD wAmmeterNo = 0) {return 0;}
		virtual	 float		ReadFloat(int nType, WORD wAmmeterNo = 0){return 0;}
		virtual	 int		ParseData(void *pData, int nLen) { return 0;};
		//add 	 by			J.Wong	2015/9/11 10:33:21
		virtual	 int		RequestData(int nType, void *pData, int nLen){return 0;};
		virtual	 void*		ReadConf() { return NULL;};
		virtual	 int		WriteConf(void* pConf) { return 0;};
		virtual	 void		SetAMTNode(){return ;}		
		virtual	 void		AmtSetAutoCheckFlag(BOOL bFlage) {}
		virtual	 DWORD		GetAmtOnLineS()	{return 0;}
		virtual	 DWORD*		GetCheckImpedance()  {return NULL;}
    	virtual	 DWORD*		GetCurImpedance()	{return NULL;}
    	virtual	 DWORD*		GetOldImpedance()	{return NULL;}
    	virtual	 WORD		GetAmtCount()	{return 0;}
		//virtual int		Request(BYTE bCode, WORD wID, int nLen, BYTE *pData) { return 0;};
		//virtual int		ReadHoldRegister(BYTE slaveID, WORD wStartAddr, WORD wNum) {return 0;};
		//virtual int		WriteSingleRegister(BYTE slaveID, WORD wStartAddr, WORD wValue) {return 0;};
		//virtual int		WriteRegisters(BYTE slaveID, WORD wStartAddr, int nNum, WORD* wValue) {return 0;};
		//virtual int		GetTotalPower() { return 0;};
		//		int 	SetNotifyer( NOTIFYERFUNC notifyer);
		void				SetBaudRate(int nRate);
		int					GetBaudRate() { return m_nBaudRate;}
		void 				SetNotifier( NOTIFIERFUNC notifier) { m_notifier = notifier;}
	protected:
		virtual int 	Open(tmRS485AMMETER* pRS485Ammeter = NULL);
   		virtual int 	Close();
   		void	WaitSend();
   		void	StartSend();
   		void	WaitRecv();
   		void	StartRecv();
   		
    	virtual int 	SendData(void *pData, int nLen);
    	virtual int 	Parse(void *pData, int nLen) { return 0;};
    private:
    	static void * RecvFunc(void *);
};

extern		CTaskAmmeter*          g_ptaskAmmeter;
extern		CTaskAmmeter*          g_ptaskPickerComm;
extern		CTaskAmmeter*		   g_pTaskHostRS232;

extern		BOOL				   g_bPickerLineState;
extern		BOOL				   g_bTecnoLineState;

#endif
