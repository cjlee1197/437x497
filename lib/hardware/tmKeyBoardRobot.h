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
#ifndef     D__TMKEYBOARDROBOT
#define     D__TMKEYBOARDROBOT

#include    "../../common.h"
#include	"../../_kb.h"
#include	"../../cmddef.h"
#include	"../../_iodef.h"
#include	"../../tmconfig.h"
#include	"../../tmvalues.h"
#include 	"../../commonaction.h"
#include	"../../readdata.h"

const	int	CN_INT_MAX_LED_NUM	= 64;



class	CtmKeyBoardRobot: public CtmKeyBoard
{
	friend	void 	KBRun(CtmKeyBoardRobot* pKey);
	
	DECLARE_DYNCREATE(CtmKeyBoardRobot)
	public:
		CtmKeyBoardRobot();
		~CtmKeyBoardRobot();
	public:
		//====================
		//	override the function that are defined in the CtmPackClass
		//====================
		int		CreateSelf();
		int		FreeSelf();
		
		void 	PutKeyTail(WORD keycode);
	protected:
		void 	Init_KB();					//	calling it in the CreateSelf
		void 	Destroy_KB();				//	calling it in the FreeSelf
		
	private:
		void	InitCurrentLed();
    
    protected:
    	KEYCODE		m_KeyBuff[KB_MAX_TOKEN];
    	_KEYQUEUE	m_Queue;
    	pthread_t	m_ScanDownThread;
};

extern	void 	KBRun(CtmKeyBoardRobot* pKey);


#endif

