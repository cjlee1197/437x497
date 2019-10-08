#ifndef		D__TMTASKDSP54_V01
#define		D__TMTASKDSP54_V01
#include	"../../taskhost.h"
#include	"../libComm.h"

class  CtmTaskDsp54: public CTaskHost
{
	DECLARE_DYNCREATE(CtmTaskDsp54)
	public:
		CtmTaskDsp54();
		~CtmTaskDsp54();
	public:
		int		CreateSelf();
		int		FreeSelf();
		
		int		ReqValues(int nType);
        int		ReqValues(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
        int		ReqValues(int nType, void* pOutData, int nSize);

		int		WriteValue(int nType, void* pInputData, int nSize);
		int		WriteValue(int nType, int nHostAdreess1, int nHostAdreess2 = 0);
		int		WriteValue(int nDataID);
    void	WndProc(int message, WPARAM wParam, LPARAM lParam);
		
		int		GetOnLineStatus();

		void    Run();
			
	protected:
		int		GetData(void* pdata, int nLen);

	private:
	    void	LED();
		static 	void	_Open(void *pData);
		static	void	_Run();
		static	void	Run_Pthread();
		static 	void	ResValueFunc(char *pData, int nLength);
	private:
	    static	CtmTaskDsp54*		m_pSelf;
		ETHERNET		m_ethernet;		
		int 			m_nConnecting;
		pthread_t 		m_OpenThreadID;
		pthread_t 		m_RunThreadID;
		pthread_mutex_t	mutex_Msg;					
		pthread_attr_t  m_ThreadAttr;
		long	        m_lFirstTime;	
		long	        m_lCheckQuickTimer;
		long	        m_lUpdateTimer;
		long	        m_lOnLineTime;
		long	        m_lTestTime;							
		long	        m_lCounterReqConfig;
		long	        m_lCounterW0;
        long	        m_lCounterW1;
        long	        m_lCounterBW1;
				
		BYTE	        m_RxBuffer[4096];
		BYTE	        m_TxBuffer[4096];
    
        TLED	        m_tled;
};

#ifndef	D_BEOBJECT_CTMTASKDSP54
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif

#endif

