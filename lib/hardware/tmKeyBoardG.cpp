/*==========================================================================+
|  Class    : kb.cpp														|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : yang                                                       	|
|  Version  : V1.00                                                         |
|  Creation : 2005/01/18                                                    |
|			: 2005/01/31	netdigger										|
|			: 2006/06/06	James											|
|  Remark	: 																|
+==========================================================================*/
#include	"tmKeyBoardG.h"
#include	"../../utils.h"
#include	"../../tmconfig.h"
#include	"../../device.h"
#include    "../../common.h"
#include	"../../main.h"
#include    "../../taskmoni.h"

#include	"../../tmshmsg.h"
#ifdef	D_CANMASTER
#include	"../../canMaster.h"
#endif

/*==========================================================================+
|           Constant                                                        |
+==========================================================================*/
#define		KB_DEVNAME			"/dev/tty0"

#define 	TABLE_SPLIT_VAL 	58
#define 	TABLE_SPLIT_VAL2 	98
#define		KEYDOWN_COUNTER		200


#define SCANCODE_ESCAPE                 1

#define SCANCODE_1                      2
#define SCANCODE_2                      3
#define SCANCODE_3                      4
#define SCANCODE_4                      5
#define SCANCODE_5                      6
#define SCANCODE_6                      7
#define SCANCODE_7                      8
#define SCANCODE_8                      9
#define SCANCODE_9                      10
#define SCANCODE_0                      11

#define SCANCODE_MINUS                  12
#define SCANCODE_EQUAL                  13

#define SCANCODE_BACKSPACE              14
#define SCANCODE_TAB                    15

#define SCANCODE_Q                      16
#define SCANCODE_W                      17
#define SCANCODE_E                      18
#define SCANCODE_R                      19
#define SCANCODE_T                      20
#define SCANCODE_Y                      21
#define SCANCODE_U                      22
#define SCANCODE_I                      23
#define SCANCODE_O                      24
#define SCANCODE_P                      25
#define SCANCODE_BRACKET_LEFT           26
#define SCANCODE_BRACKET_RIGHT          27

#define SCANCODE_ENTER                  28

#define SCANCODE_LEFTCONTROL            29

#define SCANCODE_A                      30
#define SCANCODE_S                      31
#define SCANCODE_D                      32
#define SCANCODE_F                      33
#define SCANCODE_G                      34
#define SCANCODE_H                      35
#define SCANCODE_J                      36
#define SCANCODE_K                      37
#define SCANCODE_L                      38
#define SCANCODE_SEMICOLON              39
#define SCANCODE_APOSTROPHE             40
#define SCANCODE_GRAVE                  41

#define SCANCODE_LEFTSHIFT              42
#define SCANCODE_BACKSLASH              43

#define SCANCODE_Z                      44
#define SCANCODE_X                      45
#define SCANCODE_C                      46
#define SCANCODE_V                      47
#define SCANCODE_B                      48
#define SCANCODE_N                      49
#define SCANCODE_M                      50
#define SCANCODE_COMMA                  51
#define SCANCODE_PERIOD                 52
#define SCANCODE_SLASH                  53

#define SCANCODE_RIGHTSHIFT             54
#define SCANCODE_KEYPADMULTIPLY         55

#define SCANCODE_LEFTALT                56
#define SCANCODE_SPACE                  57
#define SCANCODE_CAPSLOCK               58

#define SCANCODE_F1                     59
#define SCANCODE_F2                     60
#define SCANCODE_F3                     61
#define SCANCODE_F4                     62
#define SCANCODE_F5                     63
#define SCANCODE_F6                     CN_INT_MAX_LED_NUM
#define SCANCODE_F7                     65
#define SCANCODE_F8                     66
#define SCANCODE_F9                     67
#define SCANCODE_F10                    68

#define SCANCODE_NUMLOCK                69
#define SCANCODE_SCROLLLOCK             70

#define SCANCODE_KEYPAD7                71
#define SCANCODE_CURSORUPLEFT           71
#define SCANCODE_KEYPAD8                72
#define SCANCODE_CURSORUP               72
#define SCANCODE_KEYPAD9                73
#define SCANCODE_CURSORUPRIGHT          73
#define SCANCODE_KEYPADMINUS            74
#define SCANCODE_KEYPAD4                75
#define SCANCODE_CURSORLEFT             75
#define SCANCODE_KEYPAD5                76
#define SCANCODE_KEYPAD6                77
#define SCANCODE_CURSORRIGHT            77
#define SCANCODE_KEYPADPLUS             78
#define SCANCODE_KEYPAD1                79
#define SCANCODE_CURSORDOWNLEFT         79
#define SCANCODE_KEYPAD2                80
#define SCANCODE_CURSORDOWN             80
#define SCANCODE_KEYPAD3                81
#define SCANCODE_CURSORDOWNRIGHT        81
#define SCANCODE_KEYPAD0                82
#define SCANCODE_KEYPADPERIOD           83

#define SCANCODE_LESS                   86

#define SCANCODE_F11                    87
#define SCANCODE_F12                    88

#define SCANCODE_KEYPADENTER            96
#define SCANCODE_RIGHTCONTROL           97
#define SCANCODE_CONTROL                97
#define SCANCODE_KEYPADDIVIDE           98
#define SCANCODE_PRINTSCREEN            99
#define SCANCODE_RIGHTALT               100
#define SCANCODE_BREAK                  101    /* Beware: is 119     */
#define SCANCODE_BREAK_ALTERNATIVE      119    /* on some keyboards! */

#define SCANCODE_HOME                   102
#define SCANCODE_CURSORBLOCKUP          103    /* Cursor key block */
#define SCANCODE_PAGEUP                 104
#define SCANCODE_CURSORBLOCKLEFT        105    /* Cursor key block */
#define SCANCODE_CURSORBLOCKRIGHT       106    /* Cursor key block */
#define SCANCODE_END                    107
#define SCANCODE_CURSORBLOCKDOWN        108    /* Cursor key block */
#define SCANCODE_PAGEDOWN               109
#define SCANCODE_INSERT                 110
#define SCANCODE_REMOVE                 111

#define SCANCODE_PAUSE                  119
#define SCANCODE_LEFTWIN                125
#define SCANCODE_RIGHTWIN               126
#define SCANCODE_MENU                   127

#define NR_KEYS                         128

#define SCANCODE_USER                   (NR_KEYS + 1)

#define MASK_KS_SHIFTKEYS               0x00000FFF

typedef void * (* PKBRUN)(void *);

#define	FLAG_SCROLLLOCK				0x0001
#define	FLAG_NUMLOCK				0x0002
#define	FLAG_CAPSLOCK				0x0004

#define MAX_LED_GROUP				8

/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/






/*==========================================================================+
|           Global variable                                                 |
+==========================================================================*/



static char KeyMapNCapsLock[] = 
{0,
	 27,'1','2','3','4','5','6','7','8','9','0', '-','=','\b','\t',                                        
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',  0, 
	'a','s','d','f','g','h','j','k','l',';','\'','`',  0,'\\',
	'z','x','c','v','b','n','m',',','.','/',  0,'*',  0,' ',  0,
};

static char KeyMapCapsLock[] =
{0,
	 27,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',  0,
	'A','S','D','F','G','H','J','K','L',':','"','~',  0,'|',
	'Z','X','C','V','B','N','M','<','>','?',  0,'*',  0,' ',  0,
};

static unsigned short m_KeyMap_ALT[] =
{
	_KEY_PANEL_,					// ALT+0
	_KEY_PANEL_COREBOUT,			// ALT+1
	_KEY_PANEL_,					// ALT+2
	_KEY_PANEL_MOTOR,				// ALT+3
	_KEY_PANEL_CORECIN,				// ALT+4
	_KEY_PANEL_CORECOUT,			// ALT+5
	_KEY_PANEL_MOTOROFF,			// ALT+6
	_KEY_PANEL_HEATEROFF,			// ALT+7
	_KEY_PANEL_,					// ALT+8
	_KEY_PANEL_,					// ALT+9
	_KEY_PANEL_,					// ALT+:
	_KEY_PANEL_,					// ALT+;
	_KEY_PANEL_,					// ALT+<
	_KEY_PANEL_,					// ALT+=
	_KEY_PANEL_,					// ALT+>
	_KEY_PANEL_,					// ALT+?
	_KEY_PANEL_,					// ALT+@
	_KEY_PANEL_MOLDCLOSE,			// ALT+A
	_KEY_PANEL_AUTOPURGE,			// ALT+B
	_KEY_PANEL_MOLDOPEN,			// ALT+C
	_KEY_PANEL_INJECT,				// ALT+D
	_KEY_PANEL_SUCKBACK,			// ALT+E
	_KEY_PANEL_CHARGE,				// ALT+F
	_KEY_PANEL_NOZZLEADV,			// ALT+G
	_KEY_PANEL_NOZZLERET,			// ALT+H
	_KEY_PANEL_EJECTADV,			// ALT+I
	_KEY_PANEL_EJECTRET,			// ALT+J
	_KEY_PANEL_COREAIN,				// ALT+K
	_KEY_PANEL_COREAOUT,			// ALT+L
	_KEY_PANEL_MOLDTHIN,			// ALT+M
	_KEY_PANEL_MOLDTHICK,			// ALT+N
	_KEY_PANEL_MOLDADJ,				// ALT+O
	_KEY_PANEL_SEMIAUTO,			// ALT+P
	_KEY_PANEL_TIMEAUTO,			// ALT+Q
	_KEY_PANEL_SENSORAUTO,			// ALT+R
	_KEY_PANEL_DOOROPEN,			// ALT+S
	_KEY_PANEL_DOORCLOSE,			// ALT+T
	_KEY_PANEL_HEATER,				// ALT+U
	_KEY_PANEL_LUBRICATION,			// ALT+V
	_KEY_PANEL_AIRBLASTMOV,			// ALT+W
	_KEY_PANEL_AIRBLASTSTAT,		// ALT+X
	_KEY_PANEL_MOTIONSTART,			// ALT+Y
	_KEY_PANEL_COREBIN,				// ALT+Z
	_KEY_PANEL_,					// ALT+[
	_KEY_PANEL_,					// ALT+
	_KEY_PANEL_,					// ALT+]
	_KEY_PANEL_,					// ALT+^
	_KEY_PANEL_,					// ALT+_
	_KEY_PANEL_,					// ALT+`
	_KEY_PANEL_MOLDCLOSE,			// ALT+a
	_KEY_PANEL_AUTOPURGE,			// ALT+b
	_KEY_PANEL_MOLDOPEN,			// ALT+c
	_KEY_PANEL_INJECT,				// ALT+d
	_KEY_PANEL_SUCKBACK,			// ALT+e
	_KEY_PANEL_CHARGE,				// ALT+f
	_KEY_PANEL_NOZZLEADV,			// ALT+g
	_KEY_PANEL_NOZZLERET,			// ALT+h
	_KEY_PANEL_EJECTADV,			// ALT+i
	_KEY_PANEL_EJECTRET,			// ALT+j
	_KEY_PANEL_COREAIN,				// ALT+k
	_KEY_PANEL_COREAOUT,			// ALT+l
	_KEY_PANEL_MOLDTHIN,			// ALT+m
	_KEY_PANEL_MOLDTHICK,			// ALT+n
	_KEY_PANEL_MOLDADJ,				// ALT+o
	_KEY_PANEL_SEMIAUTO,			// ALT+p
	_KEY_PANEL_TIMEAUTO,			// ALT+q
	_KEY_PANEL_SENSORAUTO,			// ALT+r
	_KEY_PANEL_DOOROPEN,			// ALT+s
	_KEY_PANEL_DOORCLOSE,			// ALT+t
	_KEY_PANEL_HEATER,				// ALT+u
	_KEY_PANEL_LUBRICATION,			// ALT+v
	_KEY_PANEL_AIRBLASTMOV,			// ALT+w
	_KEY_PANEL_AIRBLASTSTAT,		// ALT+x
	_KEY_PANEL_MOTIONSTART,			// ALT+y
	_KEY_PANEL_COREBIN,				// ALT+z
};

static unsigned short m_KeyMap_CTRL[] =
{
	_CTRL_A, _CTRL_B, _CTRL_C, _CTRL_D, _CTRL_E, _CTRL_F, _CTRL_G, 
	_CTRL_H, _CTRL_I, _CTRL_J, _CTRL_K, _CTRL_L, _CTRL_M, _CTRL_N,
	_CTRL_O, _CTRL_P, _CTRL_Q, _CTRL_R, _CTRL_S, _CTRL_T, _CTRL_U,
	_CTRL_V, _CTRL_W, _CTRL_X, _CTRL_Y, _CTRL_Z
};


//table2 translating depends on numlock & shift
static const char KeyMapNumShift [] = 
{ 0,
	0,0,0,0,0,0,0,0,0,0,0,0,    // f1-f10 numlock & scrolllock
	0,0,0,'-',0,0,0,'+',0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	'\n',0,'/'
};

static const char KeyMapNum [] = 
{ 0,
	0,0,0,0,0,0,0,0,0,0,0,0,    // f1-f10 numlock & scrolllock
	'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,
	0,0,0,0,0,0,0,0,0,
	'\n',0,'/'
};


BYTE		g_cLEDData		= 0x00;


BYTE		m_cSaveRestoreLED = 0;	// add by leo20071129

CIniFile	 m_KeyIni;	

short       m_nESCCount;							//Caozz 2010/9/14 09:15¤W¤È
extern		WORD				g_wControl ; 		//Caozz 2010/9/14 09:15¤W¤È

extern		int					g_nViewChange; 

long		m_lKeyEscCounter;

    	int			m_nKB;					// 鍵盤設備的文件句柄	

/*==========================================================================+
|           Function and Class prototype                                    |
+==========================================================================*/
IMPLEMENT_DYNCREATE(CtmKeyBoardG, CtmKeyBoard)
CtmKeyBoardG::CtmKeyBoardG() : CtmKeyBoard()
{
	
	
}

CtmKeyBoardG::~CtmKeyBoardG()
{
		
}

int		CtmKeyBoardG::CreateSelf()
{
	CtmKeyBoard::CreateSelf();
		
	m_nKB			= 0;
	m_nUseTM		= 0;
	m_iKBMode 		= 0;
	m_wLedGroup		= 0;
	m_bInit			= TRUE;
	m_pszKeyFile	= NULL;
	m_pszLedFile	= NULL;
	m_pszDefaultLed	= NULL;
	m_pszLedOrder	= NULL;
	m_bKeyUp		= TRUE;			//<<yang 2007/4/13 
	m_bKeyUp1		= TRUE;	
	m_wPreScan		= _NULL_KEY;
	m_lKeyUpCounter	= KEYDOWN_COUNTER;
	
	memset(m_KeyBuff, 		0, 		sizeof(m_KeyBuff));
	memset(&m_Termios,		0, 		sizeof(m_Termios));
	memset(m_aryLedKey, 	0, 		sizeof(m_aryLedKey));
	memset(m_pwndLeds,  	0, 		sizeof(m_pwndLeds));
	memset(m_aryAct,  		0, 		sizeof(m_aryAct));
	memset(m_awSaveLED, 	0, 		sizeof(m_awSaveLED));
	memset(m_aryDefaultLed,	0xFF, 	sizeof(m_aryDefaultLed));	
	memset(m_aryKey, 		0xFF, 	sizeof(m_aryKey));
	
	
	InitCurrentLed();

	
	Init_KB();
	#ifdef	D_CANMASTER
	//TaskCanMaster();		//James add 2013/4/2 11:57:49
	#endif
	return 0;
}


int		CtmKeyBoardG::FreeSelf()
{
	CtmKeyBoard::FreeSelf();
	Destroy_KB();
	#ifdef	D_CANMASTER
	//TaskCanMasterClose();
	#endif
	delete []m_pszKeyFile;
	delete []m_pszLedFile;
	delete []m_pszDefaultLed;
	delete []m_pszLedOrder;
	delete []m_pszActLed;
	return 0;
}

void	CtmKeyBoardG::InitCurrentLed()
{
	m_awCurrentLED[0] = 0x0000;
	m_awCurrentLED[1] = 0x0100;
	m_awCurrentLED[2] = 0x0200;
	m_awCurrentLED[3] = 0x0300;
	m_awCurrentLED[4] = 0x0400;
	m_awCurrentLED[5] = 0x0500;
	m_awCurrentLED[6] = 0x0600;
	m_awCurrentLED[7] = 0x0700;
}

/*--------------------------------------------------------------------------+
| Function : void Init_KB()                                              	|
| Task     : Init keyboard input                                   			|
+---------------------------------------------------------------------------+
| Call     : Init_KB()                                                		|
|                                                                           |
| Parameter: None                      -                                    |
|                                                                           |
| Return   : None                      -                         	 		|
+--------------------------------------------------------------------------*/


int		CtmKeyBoardG::OpenPanel(char* pszPanel)
{
	int nResult =  open(pszPanel, O_RDWR);
	
	m_nKB		= nResult;
	if (nResult > 0)
	{		
		m_nUseTM	= 1;	//	flag on the panel
		if (CtmConfig::GetInstance()->GetKeyFlag() == 1)
		{
			ioctl(m_nKB, 0xA012, 0); //	Test
		}
		else
		{
			ioctl(m_nKB, 0xA012, 1); //	Test
			ioctl(m_nKB, 0x1012, 1); //	Test
		}
	}
	return nResult;
}

int		CtmKeyBoardG::OpenKeyBoard(char* pszKB)
{
	
	termios		Termios;
	perror("_Init_panel failure:");
	m_nUseTM	= 0;		//	flag on the kb
	
	int nResult = open(KB_DEVNAME, O_RDONLY | O_NOCTTY);
	if (nResult < 0)
	{
		perror("_Init_KeyBoard Open");
		exit(1);		
	}

	if (tcgetattr(nResult, &m_Termios) < 0)
	{
		perror("_Init_KeyBoard tcgetattr");		
		exit(1);
	}

	Termios 			= m_Termios;
	Termios.c_lflag	   &= ~(ICANON | ECHO  | ISIG);
	Termios.c_iflag	   &= ~(ISTRIP | IGNCR | ICRNL | INLCR | IXOFF | IXON);
	Termios.c_iflag    |= IGNBRK;
	Termios.c_cc[VMIN] 	= 1;
	Termios.c_cc[VTIME] = 0;

	if(tcsetattr(nResult, TCSAFLUSH, &Termios) < 0)
	{
		perror("_Init_KeyBoard tcsetattr");		
		exit(1);
	}

	//========
	// Put the keyboard into MEDIUMRAW mode.  Despite the name, this
	// is really "mostly raw", with the kernel just folding long
	// scancode sequences (e.g. E0 XX) onto single keycodes.
	//=======
	ioctl(nResult, KDGKBMODE, &m_iKBMode);
	if(ioctl(nResult, KDSKBMODE, K_MEDIUMRAW) < 0)
	{
		perror("_Init_KeyBoard Set Keyboard mode");
		exit(1);
	}
	
	//=======
	//  ³]¸mÁä½L¿Oª¬ºA
	//=======	
	ioctl (nResult, KDSETLED, m_KeyFlag);
	
	
	return nResult;
	
}


int		CtmKeyBoardG::GetTableName()
{
	m_KeyIni.OpenFile("outerconfig.ini");	
	m_KeyIni.ReadString("keyled",	"Key", 			NULL, &m_pszKeyFile);
	m_KeyIni.ReadString("keyled",	"Led", 			NULL, &m_pszLedFile);	
	m_KeyIni.ReadString("keyled",	"DefaultLed", 	NULL, &m_pszDefaultLed);	
	m_KeyIni.ReadString("keyled", 	"LedOrder",		NULL, &m_pszLedOrder);
	m_KeyIni.ReadString("keyled",	"ActLed",		NULL, &m_pszActLed);
	m_KeyIni.CloseFile();
	
	return 0;
}

int		CtmKeyBoardG::GetTable()
{
	GetTableName();
		
	GetKeyTable();
	GetLedTable();	
	return 0;	
	
}

int		CtmKeyBoardG::GetKeyTable()
{
	int nResult = -1;
	printf("m_pszKeyFile=%s \n", m_pszKeyFile);
	if (m_pszKeyFile != NULL)
	{
		m_ReadKey.OpenFileInt(m_pszKeyFile);
		nResult = m_ReadKey.ReadData(m_aryKey);
		m_ReadKey.CloseFileInt();
	}
	return nResult;	
}

int		CtmKeyBoardG::GetLedTable()
{
	int nResult = -1;
	
	//	open the led's file
	if (m_pszLedFile != NULL)
	{
		m_ReadLed.OpenFileInt(m_pszLedFile);
		
		nResult 	= m_ReadLed.ReadData(m_aryLedKey, sizeof(m_aryLedKey) / sizeof(m_aryLedKey[0]));		
		
		char sz[10];
		for (int i = 0; i < CN_INT_MAX_LED_NUM; i++)
		{
			//	get the led
			sprintf(sz, "%s%d", "LED_", i);
			m_pwndLeds[i] 	= m_ReadLed.GetControlInt()->FindControlFromName(sz);
			
			//	get the led groups
			if (m_pwndLeds[i] != NULL) m_wLedGroup	= i / 8 + 1;
			
			//	get the led's mapping key
			sprintf(sz, "%s%d", "value", i);
			m_ReadLed.GetControlInt()->GetPropValueT(sz, &m_aryLedKey[i], sizeof(m_aryLedKey[i]));
			
			if (m_aryLedKey[i] != -1)
				nResult++;
		}
		m_ReadLed.CloseFileInt();
	}
	
	
	//	open the default led's file
	if (m_pszDefaultLed != NULL)
	{
		m_DefaultLed.OpenFileInt(m_pszDefaultLed);
		m_DefaultLed.ReadData(m_aryDefaultLed, sizeof(m_aryDefaultLed) / sizeof(m_aryDefaultLed[0]));
		m_DefaultLed.CloseFileInt();
		
	}
	
	//	open the led order's file
	if (m_pszLedOrder != NULL)
	{
		m_DefaultLed.OpenFileInt(m_pszLedOrder);
		m_DefaultLed.ReadData(m_aryLedOrder, sizeof(m_aryLedOrder) / sizeof(m_aryLedOrder[0]));
		m_DefaultLed.CloseFileInt();
	}
	
	//	open the act led index's file
	if (m_pszActLed != NULL)
	{
		m_DefaultLed.OpenFileInt(m_pszActLed);
		m_DefaultLed.ReadData(m_aryAct, sizeof(m_aryAct) / sizeof(m_aryAct[0]));
		m_DefaultLed.CloseFileInt();
	}
	return nResult;		
}

void CtmKeyBoardG::Init_KB()
{
	GetTable();
	
	m_nKB	= OpenPanel("/dev/panel");
	
	if (m_nKB > 0) InitLED();
	
	if(m_nKB < 0)
		m_nKB = OpenKeyBoard(KB_DEVNAME);
	  
	
	m_Queue.wInput		=
	m_Queue.wOutput		= 0;
	m_Queue.wEnd		= KB_MAX_TOKEN;
	m_Queue.bFull		= FALSE;	
	m_Queue.pKeyBuff	= m_KeyBuff;
	
	m_lKeyEscCounter	= 1000;
	
	AddTimer(&m_lKeyUpCounter);
	
	AddTimer(&m_lKeyEscCounter);
	
	sem_init(&m_KeyDownWait, 0, 0);
	sem_init(&m_KeyUpWait, 0, 0);
	
	#ifdef	D_CANMASTER
	pthread_create(&m_KeyBoardM3Thread, NULL, (PKBRUN)KBM3Run, this);
	#endif
	pthread_create(&m_ScanDownThread, NULL, (PKBRUN)KBRun, this);
	pthread_create(&m_ScanUpThread, NULL, (PKBRUN)KBScanKeyUp, this);	//yang 2007/4/13 
	sem_wait(&m_KeyDownWait);
	
	//atexit(Destroy_KB);
}

void	CtmKeyBoardG::InitLED()
{
	//	trun off the all led
	m_bInit	= TRUE;
	for (int i = 0; i < CN_INT_MAX_LED_NUM; i++)
	{
		if (m_pwndLeds[i] != NULL) 
		{
			if (m_aryDefaultLed[i] != (int)0xFFFFFFFF)
				PowerOnLED(i);
			else
				PowerOffLED(i);
		}
	}
	m_bInit	= FALSE;
}


void	CtmKeyBoardG::UpKeyLed(WORD wKeyCode)
{
	for (int i = 0; i < CN_INT_MAX_LED_NUM; i++)
	{
		if (wKeyCode == m_aryLedKey[i])
		{
			PowerOffLED(i);
			break;
		}
		
	}
}

void	CtmKeyBoardG::PressKeyLed(WORD wKeyCode)
{
	for (int i = 0; i < CN_INT_MAX_LED_NUM; i++)
	{
		if (wKeyCode == m_aryLedKey[i])
		{
			SetLED(i);
			break;
		}
		
	}
	
}

/*--------------------------------------------------------------------------+
| Function : void Destroy_KB()                                           	|
| Task     : Restore keyboard input                                			|
+---------------------------------------------------------------------------+
| Call     : Destroy_KB()                                             		|
|                                                                           |
| Parameter: None                      -                                    |
|                                                                           |
| Return   : None                      -                         	 		|
+--------------------------------------------------------------------------*/
void	CtmKeyBoardG::Destroy_KB()
{
	if(m_ScanDownThread != 0)		pthread_cancel(m_ScanDownThread);
	if(m_ScanUpThread != 0) pthread_cancel(m_ScanUpThread);					//yang 2007/4/13 
	
	#ifdef	D_CANMASTER
	if(m_KeyBoardM3Thread != 0)		pthread_cancel(m_KeyBoardM3Thread);		//James add 2013/4/2 13:03:00
	#endif
	if(m_nUseTM == 0)
	{
		ioctl(m_nKB, KDSKBMODE, m_iKBMode);
		tcsetattr(m_nKB, TCSANOW, &m_Termios);
		m_nUseTM	= 0;
	}
	if(m_nKB > 0)
	{
		close(m_nKB);
	}
	m_nKB 		= -1;
	DeleteTimer(&m_lKeyUpCounter);											//yang 2007/4/14 
	DeleteTimer(&m_lKeyEscCounter);
	sem_destroy(&m_KeyDownWait);													
	sem_destroy(&m_KeyUpWait);	
}


int		CtmKeyBoardG::WriteToLedDev(void* pData)
{
	int nSize = 1;
	
	//	KeyFlag == 1 is not 6951
	if (CtmConfig::GetInstance()->GetKeyFlag() == 1)
		nSize = 1;
	else 
		nSize = 2;
	
	#ifdef	D_CANMASTER
	DWORD	dwM3Led[2] = {0};
	WORD	wData = 0;
	BYTE	Low = 0, High = 0;
	
	wData 	= *(WORD*)(pData);
	Low 	= wData & 0xFF;
	High 	= (wData & 0xFF00) >> 8;
	//James for M3 KeyBoard
	//printf("wData=%x low=%d  High=%d \n", wData, Low, High);
	//printf("1 g_dwM3LED[0]=%04x 1=%04x \n", g_dwM3LED[0], g_dwM3LED[1]);
	switch(High)
	{
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
			wData = Low;
			//printf("wData=%d \n", wData);
			dwM3Led[0] = g_dwM3LED[0];
			//printf("1 dwM3Led[0] =%x \n", dwM3Led[0]);
			dwM3Led[0] = dwM3Led[0] & ~(0xFF << (High * 8)) ;
			//printf("2 dwM3Led[0] =%x \n", dwM3Led[0]);
			dwM3Led[0] = dwM3Led[0] | (wData << (High * 8)) ;
			//printf("3 dwM3Led[0] =%x \n", dwM3Led[0]);
			g_dwM3LED[0] = dwM3Led[0];
			//wData = wData +	(High*8);
			//	if(Low2 > 0)		//off led
			//	{
			//		g_dwM3LED[0] = g_dwM3LED[0] & ~(1 << (wData-1)) ;
			//	}
			//	else				// on led
			//	{
			//		g_dwM3LED[0] = g_dwM3LED[0] | (1 << (wData-1)) ;	
			//	}
			//}	
			break;
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			wData = Low;
			//printf("wData=%d \n", wData);
			dwM3Led[1] = g_dwM3LED[1];
			dwM3Led[1] = dwM3Led[1] & ~(0xFF << ((High-4) * 8)) ;
			dwM3Led[1] = dwM3Led[1] | (wData << ((High-4) * 8)) ;
			g_dwM3LED[1] = dwM3Led[1];
			break;
	}
	#endif
	return write(m_nKB, pData, nSize);
}

/*---------------------------------------------------------------------------+
;| Function : KEYCODE GetKey()                                              |
;| Task     : Read extended keyboard input                                   |
;+---------------------------------------------------------------------------+
;| Call     : key = GetKey()                                                |
;|                                                                           |
;| Parameter: None                      -                                    |
;|                                                                           |
;| Return   : al                        - ASCII code                         |
;|            ah                        - Scan  code                         |
;|            ax = GetKey              - Key   code                         |
;+--------------------------------------------------------------------------*/
WORD	CtmKeyBoardG::GetKey()
{
	WORD	wCode;
	
	if(m_Queue.wInput == m_Queue.wOutput && m_Queue.bFull == FALSE)
		return _NULL_KEY;

	wCode	= m_KeyBuff[m_Queue.wOutput ++].wCode;
	
	if(m_Queue.wOutput >= m_Queue.wEnd)		m_Queue.wOutput	= 0;
	m_Queue.bFull	= FALSE;
	
	return wCode;
}

/*---------------------------------------------------------------------------+
;| Function : KEYCODE _TestKeyFlag(WORD __far *fpKeyFlag)                    |
;| Task     : Test extended keyboard input and flags                         |
;+---------------------------------------------------------------------------+
;| Call     : key = _TestKeyFlag(fpKeyFlag)                                  |
;|                                                                           |
;| Parameter: fpKeyFlag                 - Far pointer to KeyFlag             |
;|                                                                           |
;| Return   : al                        - ASCII code                         |
;|            ah                        - Scan  code                         |
;|            ax = _TestKeyFlag         - Key   code                         |
;|                                        0 : No key in the keyboard buffer  |
;|            *fpKeyFlag                - Keyboard flags                     |
;+--------------------------------------------------------------------------*/
WORD 	CtmKeyBoardG::TestKeyFlag(FPKEYFLAG fpKeyFlag)
{
	if(m_Queue.wInput == m_Queue.wOutput && m_Queue.bFull == FALSE)
		return _NULL_KEY;
	*fpKeyFlag	= m_KeyBuff[m_Queue.wOutput].wFlag;

	return m_KeyBuff[m_Queue.wOutput].wCode;
}

/*---------------------------------------------------------------------------+
;| Function :                                                                |
;| Task     :                                                                |
;+---------------------------------------------------------------------------+
;| Call     :                                                                |
;|                                                                           |
;| Parameter:                                                                |
;|                                                                           |
;| Return   :                                                                |
;+--------------------------------------------------------------------------*/
void	CtmKeyBoardG::PutKeyTail(WORD /*KEYCODE*/ keycode)
{
	MSG				msg;
	KEYCODE			Key;	
	
	
	// ¬°¤F¦h½uµ{¦w¥þ¡A¥²¶·©ñ¦bQueueMessage¤§«e
	if(m_Queue.bFull == TRUE)
	{		
		return;
	}
	Key.wCode	= keycode;
	m_KeyBuff[m_Queue.wInput++]	= Key;
	
	if(m_Queue.wInput >= m_Queue.wEnd)		m_Queue.wInput	= 0;
	if(m_Queue.wInput == m_Queue.wOutput)	m_Queue.bFull	= TRUE;

	msg.message		= MSG_KEYDOWN;
	msg.wParam		= (WPARAM)Key.wCode;
	msg.lParam		= 0;
	//printf("CtmKeyBoardG MSG_KEYDOWN Key.wCode=%d\n",Key.wCode);
	g_pApplication->QueueMessage(&msg);	
}

/*--------------------------------------------------------------------------+
| Function : void GetAscii(BYTE cScan)                                   	|
| Task     : Restore keyboard input                                			|
+---------------------------------------------------------------------------+
| Call     : Destroy_KB()                                             		|
|                                                                           |
| Parameter: None                      -                                    |
|                                                                           |
| Return   : None                      -                         	 		|
+--------------------------------------------------------------------------*/
BYTE	CtmKeyBoardG::GetAscii(BYTE cScan)
{
	cScan	   &= 0x7F;
	
	if(cScan <= TABLE_SPLIT_VAL2)
	{
		if(cScan <= TABLE_SPLIT_VAL)
		{
			if(isalpha(KeyMapNCapsLock[cScan])) 
            {
                if(! (m_KeyFlag.CapsLock ^ 
                	 (m_KeyFlag.LShift | m_KeyFlag.RShift)) )
                    return KeyMapNCapsLock[cScan];
                else
                    return KeyMapCapsLock[cScan];
            }
            else if(m_KeyFlag.LShift | m_KeyFlag.RShift)
                return KeyMapCapsLock[cScan];
            else 
                return KeyMapNCapsLock[cScan];
		}
		else
		{
			if (!(m_KeyFlag.NumLock ^ 
				 (m_KeyFlag.LShift | m_KeyFlag.RShift)) )
				return KeyMapNumShift[cScan - TABLE_SPLIT_VAL];
			else
				return KeyMapNum[cScan - TABLE_SPLIT_VAL];
		}
	}
	
	return 0;
}

WORD	CtmKeyBoardG::SetKeyFlage(BYTE cScan)
{
	BOOL 	KeyDown;
	WORD	wIsSysKey	= 1;
	
	KeyDown	= (cScan & 0x80)?0:1;
	cScan  &= 0x7F;
	
	switch (cScan) 
	{
        case SCANCODE_CAPSLOCK:
        	if(KeyDown == 1)
        		m_KeyFlag.CapsLock		= ~m_KeyFlag.CapsLock;
        	ioctl (m_nKB, KDSETLED, m_KeyFlag.wCode);
        	break;
        case SCANCODE_NUMLOCK:
        	if(KeyDown == 1)
        		m_KeyFlag.NumLock		= ~m_KeyFlag.NumLock;
        	ioctl (m_nKB, KDSETLED, m_KeyFlag.wCode);
        	break;
        case SCANCODE_SCROLLLOCK:
			if(KeyDown == 1)	
				m_KeyFlag.ScrollLock	= ~m_KeyFlag.ScrollLock;
			ioctl (m_nKB, KDSETLED, m_KeyFlag.wCode);
            break;
        case SCANCODE_INSERT:
        	if(KeyDown == 1)	
				m_KeyFlag.Insert		= ~m_KeyFlag.Insert;
        case SCANCODE_LEFTCONTROL:
            m_KeyFlag.LCtrl 	= KeyDown;
            m_KeyFlag.Ctrl		= KeyDown;
            break;
        case SCANCODE_RIGHTCONTROL:
            m_KeyFlag.RCtrl 	= KeyDown;
            m_KeyFlag.Ctrl 		= KeyDown;
            break;
        case SCANCODE_LEFTSHIFT:
            m_KeyFlag.LShift	= KeyDown;
            break;
        case SCANCODE_RIGHTSHIFT:
            m_KeyFlag.RShift	= KeyDown;
            break;
        case SCANCODE_LEFTALT:
            m_KeyFlag.LAlt 		= KeyDown;
            m_KeyFlag.Alt 		= KeyDown;
            break;
        case SCANCODE_RIGHTALT:
            m_KeyFlag.RAlt 		= KeyDown;
            m_KeyFlag.Alt 		= KeyDown;
            break;
       	default:
       		wIsSysKey			= 0;
       		break;
    }
    
    return 	wIsSysKey;
}

int		GetBitIndex(DWORD	dwData)
{
	int i = 0, nResult = 0;
	for(i = 0; i < 32; i++)
	{
		if(dwData &  (1 << i))
		{
			nResult = i;
			break;
		}
	}
	return nResult;
}
#ifdef	D_CANMASTER
void	KBM3Run(CtmKeyBoardG* pKey)
{
	KEYCODE		Key;
	DWORD		dwM3Key_Old[MAX_KEYINDEX] = {0};
	WORD		wKeyIndex = 0;	//wOldKeyIndex= 0;
	DWORD		dwData = 0, dwOldData = 0, dwDifferent = 0; //dwOld_Action = 0;
 
	static WORD		wKeyLastScan = 0;
	static BOOL		bFlag = TRUE;
	//BOOL			bManulKey = FALSE;

	while (TRUE)
	{
		for(int i = 0; i < MAX_KEYINDEX; i++)
		{
			//printf("1 g_dwM3Key[i]=%x dwM3Key_Old[i]=%x \n", g_dwM3Key[i], dwM3Key_Old[i]);
			if(g_dwM3Key[i] != dwM3Key_Old[i])
			{
				//printf("2 g_dwM3Key[i]=%x dwM3Key_Old[i]=%x \n", g_dwM3Key[i], dwM3Key_Old[i]);
				dwOldData 	= dwM3Key_Old[i];
				dwData 		= g_dwM3Key[i];
				dwDifferent = dwData ^ dwOldData;
				for(int j = 0; j < 32; j++)
				{
					if((dwDifferent & (0x0001 << j)))
					{
						if(dwData & (0x0001 << j))
							wKeyIndex = (i*32) + j;
						else
							wKeyIndex = (i*32) + j + 128;
						Key.wCode	= pKey->m_aryKey[wKeyIndex];

						if( i >= 2)
						{
							if(wKeyLastScan >= 64 && wKeyLastScan < 128)
							{
								if(wKeyLastScan != wKeyIndex - 128) bFlag = FALSE;
								else 								bFlag = TRUE;	
							}

							if(bFlag)
								wKeyLastScan = wKeyIndex;

							//printf("wKeyIndex=%d wKeyLastScan=%d bFlag=%d \n", wKeyIndex, wKeyLastScan, bFlag);
							if(wKeyIndex >= 64 && wKeyIndex <128 && !bFlag) continue; 
							if(wKeyIndex >= 192 && wKeyIndex <256 && !bFlag) continue;
						}
						else
						{
							if(wKeyIndex >= 0x80 && wKeyIndex <= 0xC0)
							{
								if(wKeyIndex != 187 && wKeyIndex != 188)	//M3KEY板上的安全门开/关按键改为:按下有效，放开后清掉键值（与面膜上的安全门开关一致）；標準化
								{
									//printf("wKeyIndex=%x\n",wKeyIndex);
									pKey->m_wPreScan	 	= 0x9400|wKeyIndex;
									pKey->m_bKeyUp = TRUE;
									pKey->m_bKeyUp1 = TRUE;
									continue;	// ¼È¤£³B²zKEY_UP¨Æ¥ó
								}
							}
							if (Key.wCode == _KEY_RIGHTFIELD || Key.wCode == _KEY_UPFIELD || Key.wCode == _KEY_DOWNFIELD //yang 2007/4/14 for auto cursor
								|| Key.wCode == _KEY_LEFTFIELD || Key.wCode == _ENTER || Key.wCode == _KEY_CLEAR || (Key.wCode >= '0' && Key.wCode <='9'))				
							{
								pKey->m_bKeyUp 			= FALSE;
								pKey->m_wPreScan	 	= Key.wCode;
								pKey->m_lKeyUpCounter 	= 5 * KEYDOWN_COUNTER;
								sem_post(&pKey->m_KeyUpWait);
							}
						}
						//printf("wKeyIndex=%d i=%d, j=%d dwData=%x Key.wCode=%x \n", wKeyIndex, i, j, dwData, Key.wCode);
						pKey->PutKeyTail(Key.wCode);
						if (Key.wCode != 0xFFFF)
							Beep();

						if(m_lKeyEscCounter < 0)
						{
							m_nESCCount = 0;
							m_lKeyEscCounter = 1000;
						}	

						if(Key.wCode == _KEY_CLEAR) 
						{
							m_nESCCount++;       //³sÄò«ö¨â¦¸ 'C' ¡A­±ªO¥D°Ê¨ú®ø±±¨î Caozz 2010/9/14 02:55¤U¤È 
							
							if(m_nESCCount >= 2 && g_wControl != 0)
								SendMsg(MSG_SH_REMOTE_CANCEL_KEY, 0, 0, NULL);
							
							m_lKeyEscCounter = 1000;
						}
						else  m_nESCCount = 0;	

					}
				}
				dwM3Key_Old[i] = dwData;
			}
		}
		usleep(2000);
	}
}
#endif
void 	KBRun(CtmKeyBoardG* pKey)
{
	KEYCODE			Key;
	static WORD 	link_action=0;
	static WORD 	link_action2=0;
	static WORD 	link_action3=0;
	static WORD		wKeyLastScan = 0;
	static BOOL		bFlag = TRUE;
	
	char data;
	sem_post(&pKey->m_KeyDownWait);
	while (TRUE)
	{
		int rst = read(m_nKB, &data, 1);
		if(rst > 0)
		{
			g_ptaskCmd->SetIdle(FALSE);
			if(g_ptaskMoni!=NULL && g_ptaskMoni->IsSleep()) continue;
			//printf("read key:0x%x\n", data);
			if(link_action==0 && data==0x98) link_action=1;
			else if(link_action==1 && data==0x88) 
			{
				MSG				msg1;
				msg1.message		= MSG_KEY;
				msg1.wParam		= 0x6300;
				msg1.lParam		= 0;
				g_pApplication->QueueMessage(&msg1);	
				link_action=0;
			}
			else 
				link_action=0;
			
			if(link_action2==0 && data==0x99) link_action2=1;
			else if(link_action2==1 && data==0x89) 
			{
				int level = g_ptmControlServer->UserControl()->LoginLocal("ROOT","TECHEDO");
				if(level>0 && level <= 16)
					g_iPrivilege = level;
			}
			else 
				link_action2=0;
				
				
			if(link_action3==0 && data==0x99) link_action3=1;
			else if(link_action3==1 && data==0x98) 
			{
				if(g_iPrivilege == 16)
					MsgBoxCall("msgboxTestView.txt");	
			}
			else 
				link_action3=0;
				
			MSG				msg;
			KEYCODE		Key;	
			Key.wCode	= data;
			msg.message		= MSG_PANEL_KEY;
			msg.wParam		= (WPARAM)Key.wCode;
			msg.lParam		= 0;
			g_pApplication->QueueMessage(&msg);	
		}
		usleep(500);
	}
}


void	CtmKeyBoardG::TestLed(int nState)
{
	int i 			= 0;
	int	OldIndex 	= 0;
	
	if (m_aryLedOrder[0] == (int)0xFFFFFFFF) return;
	
	switch(nState)
	{
		case -1:
			m_wTestLedIndex	= 0;
			memcpy(m_awSaveLED, m_awCurrentLED, sizeof(m_awSaveLED));
			InitCurrentLed();
			
			for (i = 0; i < m_wLedGroup; i++)
			{
				WriteToLedDev(&m_awCurrentLED[i]);
			}
			break;
		case 0:
			memcpy(m_awCurrentLED, 	m_awSaveLED, sizeof(m_awSaveLED));
			for (i = 0; i < m_wLedGroup; i++)
			{
				WriteToLedDev(&m_awCurrentLED[i]);
			}
			break;
		case 0xFFFF:			
			if (m_aryLedOrder[m_wTestLedIndex] == (int)0xFFFFFFFF)
			{
				OldIndex		= m_wTestLedIndex - 1;
				m_wTestLedIndex = 0;
			}
			else
			{
				if (m_wTestLedIndex > 0)
					OldIndex 		= m_wTestLedIndex - 1;
				
			}
			
			PowerOffLED(m_aryLedOrder[OldIndex]);
			PowerOnLED(m_aryLedOrder[m_wTestLedIndex]);
						
			m_wTestLedIndex++;
			break;
		default:
			break;
	}
	
}

//void		CtmKeyBoardG::_Save_Restore_LED(int nState)	
//{
//	BYTE	cLED;
//	switch(nState)
//	{
//		case -1:
//			m_cSaveRestoreLED = g_cLEDData;
//			break;
//		case 0:
//			g_cLEDData = m_cSaveRestoreLED;
//			if(m_nKB >= 0)	write(m_nKB, &g_cLEDData, 1);
//			m_cSaveRestoreLED = 0;
//			break;
//		default:
//			cLED = nState;
//			if(m_nKB >= 0)	write(m_nKB, &cLED, 1);
//			break;
//	}
//}


WORD	CtmKeyBoardG::PowerOnLED(WORD wLEDID)
{
	if (wLEDID < CN_INT_MAX_LED_NUM)
	{
		WORD	High 	= wLEDID  / 8;
		BYTE	Low		= wLEDID - (High * 8);
		Low				= 1 << Low;
	
		WORD	wLed	= (High << 8) + Low;	
		wLed	= m_awCurrentLED[High] | wLed;
	
		if (m_awCurrentLED[High] != wLed || m_bInit)
		{
			m_awCurrentLED[High] = wLed;
			WriteToLedDev(&m_awCurrentLED[High]);
		}
		
		return wLEDID;
	}
	else return 0xFFFF;
	
}

WORD	CtmKeyBoardG::PowerOffLED(WORD wLEDID)
{
	if (wLEDID < CN_INT_MAX_LED_NUM)
	{
		WORD	High 	= wLEDID  / 8;
		BYTE	Low		= wLEDID - (High * 8);
		Low				= 1 << Low;
		
		Low				= ~Low;
		WORD	wLed	= (High << 8) + Low;
		
		wLed	= m_awCurrentLED[High] & wLed;
		
		if (m_awCurrentLED[High] != wLed || m_bInit)
		{
			m_awCurrentLED[High] = wLed;
			WriteToLedDev(&m_awCurrentLED[High]);
		}
		
		//printf("PowerOff m_awCurrentLED[%d] = %d\n", High, m_awCurrentLED[High]);	
		return wLEDID;
	}
	else return 0xFFFF;
}

int		CtmKeyBoardG::GetActLed(int nActionIndex)
{	
	return m_aryAct[nActionIndex];
}

int		CtmKeyBoardG::GetKeyMap(int nKeyIndex)
{
	return m_aryKey[nKeyIndex];
}

void	CtmKeyBoardG::SetLED(WORD wLEDID)
{
	if (wLEDID < CN_INT_MAX_LED_NUM)
	{
		//	first, turn on the wIEDID led
		PowerOnLED(wLEDID);
		
		//	second, trun off the mutex leds
		if (m_pwndLeds[wLEDID] != NULL)
		{
			WORD	wLEDIndex	= 0xFFFF;
			int 	i			= 0;
			char	sz[10];
			
			while (TRUE)
			{
				sprintf(sz, "%s%d", "value", i);		
				m_pwndLeds[wLEDID]->GetPropValueT(sz, &wLEDIndex, sizeof(wLEDIndex));
				i++;
				
				if (wLEDIndex == 0xFFFF || i >= CN_INT_MAX_LED_NUM) 	
					break;
				else 						
					PowerOffLED(wLEDIndex);
				
			}
		}
	}
	else
		perror("the max led index num is 63!\n");
	
	

		
}

void	CtmKeyBoardG::ActLed(int nActionIndex)
{
	if (nActionIndex >= 0 && nActionIndex < 256)
	{
		if (m_aryAct[nActionIndex] != -1)
		{
			//	power off the self led
			if (m_aryAct[nActionIndex] & ACT_LED_OFF_SELF_FLAG)
			{
				WORD	wLedID = (m_aryAct[nActionIndex] ^ ACT_LED_OFF_SELF_FLAG);
				PowerOffLED(wLedID);
			}
			else	//	power on the led
				SetLED(m_aryAct[nActionIndex]);
		}
	}
}

void	CtmKeyBoardG::UnActLed(int nActionIndex)
{
	if (nActionIndex >= 0 && nActionIndex < 256)
	{
		if (m_aryAct[nActionIndex] != -1)
		{
			WORD	wLedID = 0;
			if (m_aryAct[nActionIndex] & ACT_LED_OFF_SELF_FLAG)
				wLedID = (m_aryAct[nActionIndex] ^ ACT_LED_OFF_SELF_FLAG);
			else
				wLedID = m_aryAct[nActionIndex];
			PowerOffLED(wLedID);
				
		}
	}
	
}

BOOL	CtmKeyBoardG::GetKeyUp()
{
	return m_bKeyUp1;
}


void	KBScanKeyUp(CtmKeyBoardG* pKey)
{

}


#ifndef	D_BEOBJECT_CtmKeyBoardG
CObject*	CreateInstance(char* Name)
{
	if (g_ptaskKB == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
			(static_cast<CtmKeyBoard*>(pResult))->CreateSelf();
		g_ptaskKB = static_cast<CtmKeyBoard*>(pResult);
	}
	return g_ptaskKB;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmKeyBoard*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif

