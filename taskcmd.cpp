/*===========================================================================+
|  Class    : TaskCmd                                                        |
|  Task     : Command Service Routine                                        |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 05/07/1996                                                     |
|  Revision : V2.00                                                          |
|             07/01/1997                                                     |
|             - Take out the "taskkey.cpp" and handle the keyboard directly  |
+===========================================================================*/
#include    "_kb.h"
#include    "utils.h"
#include    "taskcmd.h"
#include    "stable.h"
//#ifdef      D_DEMO

//#endif

#include    "main.h"
//#include	"cmddef.h"
//#include	"tasksiem.h"
//#include	"taskkeb.h"
//#include	"memwatch.h"
#include	"tmdebug.h"

//	fans add 2007/11/19 02:42下午 for backup sram
#include	"storage.h"
#include	"tmconfig.h"
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/


IMPLEMENT_DYNCREATE(CTaskCmd, CtmPackClass)
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//******
//      Initialize static class member
//******
WORD        CTaskCmd::m_wTasksCount 	= 0;
long        CTaskCmd::m_lCounter 		= CMD_COUNTER;
long		CTaskCmd::m_lBackCounter	= CNT_LONG_BACKUP_SRAM_TIME;//	fans add 2007/11/19 03:10下午
BOOL        CTaskCmd::m_bIdle 			= FALSE;
WORD   		CTaskCmd::m_wSP 			= 0;
WORD   		CTaskCmd::m_wSP2 			= 0;
CtmWnd*		CTaskCmd::m_pwndStack[MAX_WINDOWS_FOCUS] = {NULL};
CtmWnd*		CTaskCmd::m_pwndStack2[MAX_WINDOWS_FOCUS] = {NULL};
CtmWnd*		CTaskCmd::m_pwndFocus = NULL;
CtmWnd*		CTaskCmd::m_pwndFocus2 = NULL;
//#ifdef      D_DEMO
CTaskDemo*  g_ptaskdemo = NULL;         // Demonstration
int         g_nDemo = 0;
//#endif
/*===========================================================================+
|           Class implementation - TaskCmd                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskCmd::CTaskCmd() : CTask(), CtmPackClass()
{
	
}

CTaskCmd::~CTaskCmd()
{
	
    
	//UnRegisterAllMessage(m_idMe);
}


/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------+
|  Function :                                                                |
|  Task     :                                                                |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
//void        CTaskCmd::Run()
//{
//	static	int	nPrintKeyNum 	= 0;
//	static	int	nRunNum 		= 0;
//	
//	nRunNum++;
//	//====================
//	//	backup Sram 2007/11/19 03:38下午
//	//====================
//	//if (m_lBackCounter <= 0)
//	//{
//	//	m_lBackCounter	= CNT_LONG_BACKUP_SRAM_TIME;
//	//	pthread_t	m_BackUpThread;
//	//	pthread_create(&m_BackUpThread, NULL, (PRSRUN)BackUpSram, NULL);
//	//}
//	
///****************************************************************************+
//|           Message process                                                  |
//+****************************************************************************/
//	MSG         msg;
//	int			nValue;
//	printf("CTaskCmd::Run() \n");
//	if (g_pApplication->GetMessage(CMD_SYSTEM, &msg))
//    {
//    	
//    	switch (msg.wMessage)
//        {
//        case _SYS_COMMAND:
//           if(msg.message == _SYS_COMMAND)
//            {
//            	if (!m_pwndFocus->OnCommand((char *)msg.lParam))              // If the _SYS_COMMAND message is not processed
//               	{
//               		g_pApplication->SetMessage(CMD_SYSTEM, &msg);   // Put it in the tail of message queue
//               	}
//            }
//           	else if (!m_pwndFocus->OnCommand(msg.wCmd))              // If the _SYS_COMMAND message is not processed
//                g_pApplication->SetMessage(CMD_SYSTEM, &msg);   // Put it in the tail of message queue
//            break;
//        default:
//            g_pApplication->SetMessage(CMD_SYSTEM, &msg);
//            break;
//        }
//    }
//    
///****************************************************************************+
//|           Key process                                                      |
//+****************************************************************************/
//
//
//	KEYCODE key;
//	KEYFLAG keyflag;
//	CtmWnd*   pwndAlpha;
//
//	if(g_ptaskKB->_TestKeyFlag((KEYFLAG __far*)&keyflag) != _NULL_KEY)
//    {
//    	key.wCode = g_ptaskKB->_GetKey();
//		if (key.wCode == _KEY_HEXADECIMAL && !m_bSystemIdle && m_wTasksCount == 1)
//		{
//			nPrintKeyNum ++;
//			//g_tmDebugInfo->PrintDebugInfo("nPrintKeyNum=%d, nRunNum=%d\n", 
//			//	nPrintKeyNum, nRunNum);
//			if (nRunNum <= 6)
//			{
//				if (nPrintKeyNum >= 2)
//				{
//					SaveScreen();
//					nPrintKeyNum = 0;
//					nRunNum = 0;
//				}
//			}
//			else
//			{
//				nPrintKeyNum = 0;
//				nRunNum = 0;
//			}
//				
//		}
//		else
//		{
//			nPrintKeyNum 	= 0;
//			nRunNum 		= 0;
//		}
///*******
//|       Check if now sleeping
//*******/
//    if (m_bSystemIdle)                //Chuntzu 2004/5/27 for screen saver
//        {
//        if ((key.wCode == _KEY_PANEL_MOLDCLOSE)||(key.wCode == _KEY_PANEL_DOOROPEN)||
//            (key.wCode == _KEY_PANEL_DOORCLOSE)||(key.wCode == _KEY_PANEL_BREAK))
//            {
//            //James 暫時mark 2008/5/11 04:05下午
//            //g_ptaskrs232->SendPanel(LOBYTE(key.wCode));
//            return;
//            }
//        goto __KEY_PROCESS_EXIT;
//        }
//	//James 暫時mark 2008/5/11 04:05下午
//	//if ((((WORD)(GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue) & 0x0010) == 0x0000) && (key.wCode == _KEY_PANEL_INJECT) &&
//	//	g_ptaskkeb != NULL)
//	//	g_ptaskkeb->SendInjectStartIndex();//zholy061228
//	//
//    //if (key.wCode == _ALT_S) g_ptaskrs232->SendPanel(LOBYTE(_KEY_PANEL_SEMIAUTO & 0x00FF));
//    //if (key.wCode == _ALT_T) g_ptaskrs232->SendPanel(LOBYTE(_KEY_PANEL_TIMEAUTO & 0x00FF));
//    //if (key.wCode == _ALT_B) g_ptaskrs232->SendPanel(LOBYTE(_KEY_PANEL_MANUAL & 0x00FF));
//    //if (key.wCode == _ALT_A) g_ptaskrs232->SendPanel(LOBYTE(_KEY_PANEL_MOLDCLOSE & 0x00FF));
//    //if (key.wCode == _ALT_C) g_ptaskrs232->SendPanel(LOBYTE(_KEY_PANEL_MOLDOPEN & 0x00FF));
//    //if (key.wCode == _ALT_F) g_ptaskrs232->SendPanel(LOBYTE(_KEY_PANEL_CHARGE & 0x00FF));
//
//		//=======
//		//       Standard keyboard preprocess
//		//=======
//	    if((key.wCode == _SPACE) && keyflag.Ctrl) key.wCode = _KEY_ALPHA;
//	    if((key.cASCII >= ASCII_NORMAL_START) && 
//	       (key.cASCII <= ASCII_NORMAL_END) && 
//	       (key.cScan != _KEY_PANEL_) )
//	         key.cScan = 0;
//
//#ifdef      D_DEMO
///*******
//|       Demonstration process
//*******/
//	
//
//    if (key.wCode == _F1)
//        {
//        if (g_nDemo++ != 0)
//            {
//            if (g_ptaskdemo == NULL && GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue == ID_VIEW_OVERVIEW)
//                {
//                if (g_nDemo >= 2)
//                	{
//	                g_ptaskdemo = new CTaskDemo;
//	                g_nDemo = 0;
//                	}
//                }
//            else
//                {
//                if (g_ptaskdemo != NULL) delete g_ptaskdemo;
//                g_ptaskdemo = NULL;
//                g_nDemo = 0;
//                }
//            }
//        }
//    else
//        {
//        g_nDemo = 0;
//        }
////
//#endif
//
////=======
//// Route directly to the focus window if Bypass flag is set
////=======
//    	if(m_bBypass && (m_pwndFocus != NULL))
//        {
//	        m_pwndFocus->OnKey(key.wCode);
//	        goto __KEY_PROCESS_EXIT;
//        }
//
////=======
////       Special keyboard preprocess - Key Lock check
////=======
///*    if (_GetKeyLock() || (!dbkeyrdy))
//        if (((key.wCode >= '0') && (key.wCode <= '9')) || (key.wCode == '*') || (key.wCode == '.') || (key.wCode == _KEY_CLEAR))    //wy040699
//            goto __KEY_PROCESS_EXIT;
//        else if ((key.wCode == _DELETE ) || (key.wCode == _PAGEUP) || (key.wCode == _PAGEDN) || (key.wCode == _UARROW) || (key.wCode == _DARROW) || (key.wCode == _LARROW) || (key.wCode == _RARROW))   //<<<WY040699  
//                goto __KEY_PROCESS_EXIT;                    // >>>WY040699
//            else if ((key.wCode == _KEY_LEFTFIELD ) || (key.wCode == _KEY_RIGHTFIELD) || (key.wCode == _KEY_UPFIELD) || (key.wCode == _KEY_DOWNFIELD))   //<<<WY040699  
//                    goto __KEY_PROCESS_EXIT;*/
////========
////      Special keyboard preprocess - adjust LCD bright
////========
///*	if (dbsyst.wIDView == ID_VIEW_OVERVIEW)
//	    {
//	    if (key.wCode == _KEY_UPFIELD)
//	        {
//	        if (dblcd.cBrighter >= 100) dblcd.cBrighter = 50;
//	        else                        dblcd.cBrighter += 1; //5;
//	        SaveSystem();
//	        _VideoBrighter((dblcd.cBrighter)*2+50);                                             //wy102500
//	        }
//	    else if (key.wCode == _KEY_DOWNFIELD)
//	        {
//	        if (dblcd.cBrighter <= 0) dblcd.cBrighter = 50;
//	        else                      dblcd.cBrighter -= 1; //5;
//	        SaveSystem();
//	        _VideoBrighter((dblcd.cBrighter)*2+50);                                             //wy102500
//	        }
//	    }*/
///*******
//|       Special keyboard preprocess - Do NOT wake up the system if it is a panel key
//*******/
//    if (HIBYTE(key.wCode) == _KEY_PANEL_)
//        {
//        nValue = GetDBValue("SYSX_COREPULLERD_COREPULLER_NULL_CDKEYOPTION").lValue;
//        if (nValue)
//            { 
//            if (key.wCode == _KEY_PANEL_CORECIN)  
//                {
//                if (nValue == 1)      key.wCode = _KEY_PANEL_COREDIN;
//                else if (nValue == 2) key.wCode = _KEY_PANEL_COREEIN;
//                else if (nValue == 3) key.wCode = _KEY_PANEL_COREFIN;
//                }
//            if (key.wCode == _KEY_PANEL_CORECOUT) 
//                {
//                if (nValue == 1)      key.wCode = _KEY_PANEL_COREDOUT;
//                else if (nValue == 2) key.wCode = _KEY_PANEL_COREEOUT;
//                else if (nValue == 3) key.wCode = _KEY_PANEL_COREFOUT;
//                }
//            }            
//        if (g_cAirBlastCKey==1)
//            {
//            if (key.wCode == _KEY_PANEL_AIRBLASTMOV)   key.wCode = _KEY_PANEL_AIR3;
//            }
//        //James add 2008/4/17 08:27下午
//        else if (g_cAirBlastCKey==2)
//            {
//            if (key.wCode == _KEY_PANEL_AIRBLASTMOV)   key.wCode = _KEY_PANEL_AIR5;
//            }
//        if (g_cAirBlastDKey==1)
//            {
//            if (key.wCode == _KEY_PANEL_AIRBLASTSTAT)  key.wCode = _KEY_PANEL_AIR4;
//            }
//        //James add 2008/4/17 08:27下午
//        else if (g_cAirBlastDKey==2)
//            {
//            if (key.wCode == _KEY_PANEL_AIRBLASTSTAT)  key.wCode = _KEY_PANEL_AIR6;
//            }
//		//James 暫時mark 2008/5/11 04:05下午
//        //g_ptaskrs232->SendPanel(LOBYTE(key.wCode));
//        if (m_lCounter < 0) m_bIdle = TRUE;
//        return;
//        }  
////=======
////       Special keyboard preprocess - Hexadecimal support
////=======
//    	if (key.wCode == _KEY_HEXADECIMAL)
//        	m_bHexadecimal = TRUE;
//    	else
//        {
//	        if ((key.wCode >= '0') && (key.wCode <= '5') && m_bHexadecimal) 
//	        	key.wCode += 'A' - '0';     //wy000103
//	        m_bHexadecimal = FALSE;
//        }
////=======
////      Handle event
////=======
//	    if (m_pwndFocus != NULL)
//	    {
//            m_pwndFocus->OnKey(key.wCode);
//		}
//
////=======
//// Reset system idle flag
////=======
//__KEY_PROCESS_EXIT:
//	    m_lCounter = CMD_COUNTER;
//	    m_bIdle = FALSE;
//    }
//	else if (m_lCounter < 0)
//    {
//	    m_bIdle = TRUE;
//	    nPrintKeyNum 	= 0;
//		nRunNum 		= 0;
//    }
//}
BOOL	CTaskCmd::SetIdle(BOOL bIdle)
{
	m_bIdle 	= bIdle;
	m_lCounter 	= CMD_COUNTER;
	
	return m_bIdle;
}

void        CTaskCmd::OnChange(WORD wIDControl)
{
	m_pwndFocus->OnChange(wIDControl);
}

void        CTaskCmd::PushFocus()
{
	if (m_pwndFocus != NULL)
    {
	    m_pwndFocus->OnLoseFocus();
	    Push(m_pwndFocus);
    }
}

void        CTaskCmd::PushFocus2()
{
	if (m_pwndFocus2 != NULL)
    {
	    m_pwndFocus2->OnLoseFocus();
	    Push2(m_pwndFocus2);
    }
}

extern int g_FbIotcol;
void        CTaskCmd::PopFocus()
{
	//ioctl(g_FbIotcol, 0xFFFF11, 0);
	//usleep(20*1000);
	if (m_pwndFocus != NULL)
	{
		WORD	wtype = 0;
		m_pwndFocus->GetPropValueT("wtype", &wtype, sizeof(wtype));
		
		if ((wtype & tmWS_REDRAW) && (wtype & tmWS_FRAMEWND))	
		m_pwndFocus->ShowBg();
	}
	
	m_pwndFocus = Pop();
	if (m_pwndFocus != NULL)
	{
	    m_pwndFocus->OnGetFocus();
	}

	//ioctl(g_FbIotcol, 0xFFFF12, 0);
}

void        CTaskCmd::PopFocus2()
{
	//ioctl(g_FbIotcol, 0xFFFF11, 0);
	//usleep(20*1000);
	if (m_pwndFocus2 != NULL)
	{
		WORD	wtype = 0;
		m_pwndFocus2->GetPropValueT("wtype", &wtype, sizeof(wtype));
		
		
		if ((wtype & tmWS_REDRAW) && (wtype & tmWS_FRAMEWND))	
			m_pwndFocus2->ShowBg();
		
	}
	
	m_pwndFocus2 = Pop2();
	if (m_pwndFocus2 != NULL)
	{
	    m_pwndFocus2->OnGetFocus();
	}
	//ioctl(g_FbIotcol, 0xFFFF12, 0);
}

void        CTaskCmd::SetFocus(CtmWnd* pwnd)
{
	PushFocus();
	m_pwndFocus = pwnd;
	//printf("m_pwndFocus =  %s  %s\n",m_pwndFocus->TYPEName,m_pwndFocus->Name);
	//printf("=========SetFocus \n");
	if (m_pwndFocus != NULL)
	{
		//ioctl(g_FbIotcol, 0xFFFF11, 0);
		//usleep(20*1000);
		m_pwndFocus->Show();//	fans add
		m_pwndFocus->OnGetFocus();
		//ioctl(g_FbIotcol, 0xFFFF12, 0);
	}
}

void        CTaskCmd::SetFocus2(CtmWnd* pwnd)
{
	PushFocus2();
	m_pwndFocus2 = pwnd;
	
	printf("=========SetFocus2 \n");
	if (m_pwndFocus2 != NULL)
	{
		//ioctl(g_FbIotcol, 0xFFFF11, 0);
		//usleep(20*1000);
		m_pwndFocus2->Show();
		m_pwndFocus2->OnGetFocus();
		//ioctl(g_FbIotcol, 0xFFFF12, 0);
	}
}

void        CTaskCmd::KillFocus(CtmWnd* pwnd)
{
	m_pwndFocus->OnLoseFocus();
	PopFocus();
}

void        CTaskCmd::KillFocus2(CtmWnd* pwnd)
{
	if(m_pwndFocus2 != NULL)
		m_pwndFocus2->OnLoseFocus();
	PopFocus2();
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/
void		CTaskCmd::Push(CtmWnd* pwnd)
{
	if (m_wSP < MAX_WINDOWS_FOCUS)
	    m_pwndStack[m_wSP++] = pwnd;
}

void		CTaskCmd::Push2(CtmWnd* pwnd)
{
	if (m_wSP2 < MAX_WINDOWS_FOCUS)
	    m_pwndStack2[m_wSP2++] = pwnd;
}

CtmWnd*		CTaskCmd::Pop()
{
	if (m_wSP > 0)
	    return m_pwndStack[--m_wSP];
	else
	    return NULL;
}

CtmWnd*		CTaskCmd::Pop2()
{
	if (m_wSP2 > 0)
	    return m_pwndStack2[--m_wSP2];
	else
	    return NULL;
}

//	fans add 2007/11/19 02:42下午 for backup sram
BOOL	CTaskCmd::BackUpSram()
{
	//BOOL bResult = FALSE;
	//int	 m_nBackHandle = -1;
	//
	//CtmSram*	pSram = CtmSram::GetInstance();
	//	
	//m_nBackHandle = open("/usr/Sram.bak", O_RDWR | O_CREAT);
	//if (m_nBackHandle >= 0)
	//{
	//	//g_tmDebugInfo->PrintDebugInfo("Backup /usr/Sram.bak!\n");
	//	ftruncate(m_nBackHandle, CtmConfig::GetInstance()->GetSramSize());
	//	
	//	BYTE*	pSramData = new BYTE[CtmConfig::GetInstance()->GetSramSize()];
	//		
	//	pSram->Read(0, CtmConfig::GetInstance()->GetSramSize(), pSramData);
	//	
	//	lseek(m_nBackHandle, 0, SEEK_SET);
	//	write(m_nBackHandle, (char*)pSramData, CtmConfig::GetInstance()->GetSramSize());
	//	close(m_nBackHandle);
	//	delete []pSramData;
	//	
	//}
	
	return TRUE;
}

