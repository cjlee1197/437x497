/*===========================================================================+
|  Function : _KEYBOARD                                                      |
|  Task     : Extended Keyboard Access Utility header file                   |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Andrew                                                         |
|  Version  : V1.00                                                          |
|  Creation : 03/26/1996                                                     |
|			: 06/06/2008      James											 |
|  Revision :                                                                |
+===========================================================================*/
#ifndef     D__TMKEYBOARDG
#define     D__TMKEYBOARDG

#include    "../../common.h"
#include	"../../_kb.h"
#include	"../../cmddef.h"
#include	"../../_iodef.h"
#include	"../../tmconfig.h"
#include	"../../tmvalues.h"
#include 	"../../commonaction.h"
#include	"../../readdata.h"

const	int	CN_INT_MAX_LED_NUM	= 64;



class	CtmKeyBoardG: public CtmKeyBoard
{
	friend	void 	KBRun(CtmKeyBoardG* pKey);
	friend	void	KBScanKeyUp(CtmKeyBoardG* pKey);
	#ifdef	D_CANMASTER
	friend	void 	KBM3Run(CtmKeyBoardG* pKey);
	#endif
	
	DECLARE_DYNCREATE(CtmKeyBoardG)
	public:
		CtmKeyBoardG();
		~CtmKeyBoardG();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
		WORD	GetKey();
		void 	PutKeyTail(WORD keycode);
		void	SetLED(WORD wLEDID);
		WORD 	TestKeyFlag(FPKEYFLAG fpKeyFlag);
		void	TestLed(int nState = 0xFFFF);
		
		void	ActLed(int nActionIndex);
		void	UnActLed(int nActionIndex);
		BOOL	GetKeyUp();
	protected:
		WORD	PowerOnLED(WORD wLEDID);
		WORD	PowerOffLED(WORD wLEDID);
		int		GetActLed(int nActionIndex);
		int		GetKeyMap(int nKeyIndex);
		
		
		
		void 	Init_KB();					//	calling it in the CreateSelf
		void 	Destroy_KB();				//	calling it in the FreeSelf
		void	InitLED();
	
		int		OpenPanel(char* pszPanel);	//	open the panel in the 255 , 270 or P3 
		int		OpenKeyBoard(char* pszKB);	//	open the keyboard in the pc
		
		int		GetTableName();
		int		GetTable();
		int		GetKeyTable();
		int		GetLedTable();
		
		
		WORD	SetKeyFlage(BYTE cScan);
		BYTE	GetAscii(BYTE cScan);
		
		int		WriteToLedDev(void* pData);
		void	PressKeyLed(WORD wKeyCode);
		void	UpKeyLed(WORD wKeyCode);
		
	private:
		void	InitCurrentLed();
    
    protected:
    	int			m_nUseTM;				// 面板按鍵狀態
    	int			m_iKBMode;				// 原先的鍵盤模式
    	termios		m_Termios;				// 原先的鍵盤設備設置
    	KEYCODE		m_KeyBuff[KB_MAX_TOKEN];
    	pthread_t	m_ScanDownThread;
		pthread_t	m_ScanUpThread;
		#ifdef	D_CANMASTER
		pthread_t	m_KeyBoardM3Thread;			//James add for M3 2013/4/2 13:02:33
		#endif
		sem_t		m_KeyDownWait;
		sem_t		m_KeyUpWait;
		BOOL		m_bKeyUp;
		BOOL		m_bKeyUp1;				//Caozz add 2012/5/10 下午 02:40:31
		WORD		m_wPreScan;
		long		m_lKeyUpCounter;		
    	
    	KEYFLAG		m_KeyFlag;
    	_KEYQUEUE	m_Queue;
    	int			m_nKeyNum;
    	int			m_aryKey[256];
    	int			m_aryAct[256];
    	
    	int			m_aryLedKey[CN_INT_MAX_LED_NUM];
    	int			m_aryDefaultLed[CN_INT_MAX_LED_NUM];
    	int			m_aryLedOrder[CN_INT_MAX_LED_NUM];
    	
    	CtmReadData	m_ReadKey;			//	read the key file
    	CtmReadData	m_ReadLed;			//	read the led file   	
    	CtmReadData	m_DefaultLed;		//	read the default led file , order and act index file
    	char*		m_pszKeyFile;		//	the key's file
    	char*		m_pszLedFile;		//	the led's file
    	char*		m_pszDefaultLed;	//	the default led file 
    	char*		m_pszLedOrder;		//	the led order's file
    	char*		m_pszActLed;		//	the act led index file

    	CtmWnd*		m_pwndLeds[CN_INT_MAX_LED_NUM];		//	the 
    	WORD		m_awCurrentLED[8];
    	WORD		m_awSaveLED[8];
    	WORD		m_wLedGroup;		//	the led's groups(the max is 8)
    	WORD		m_wTestLedIndex;
    	
    	BOOL		m_bInit;
    
    
    	 		

};

extern	void 	KBRun(CtmKeyBoardG* pKey);
extern	void	KBScanKeyUp(CtmKeyBoardG* pKey);
#ifdef	D_CANMASTER
extern	void	KBM3Run(CtmKeyBoardG* pKey);
#endif
//extern	void		_KB_Run();
//extern	void		_KB_ScanKeyUp();
//extern	void		_Restore_KB();

#ifndef	D_BEOBJECT_CTMKEYBOARDG	//	defineName=D_BEOBJECT_+CASE(ClassName) 
extern	"C"	CObject*	CreateInstance(char* Name);
extern	"C"	void		ReleaseInstance(CObject* pInstance);
#endif



#endif

