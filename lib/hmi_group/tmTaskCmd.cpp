#include	<fstream> 
#include	"../../package.h"
#include	"../../timer.h"
#include    "../../_kb.h"
#include	"../../main.h"
#include	"../../tmdebug.h"
#include	"../../task.h"
#include	"../../utils.h"
#include	"../../taskhost.h"
#include    "../../taskdemo.h"
#include	"../../iotable.h"
#include	"tmTaskCmd.h"
#include	"../libComm.h"
#include 	"../../tmshmsg.h"
#include 	"../../tmnbmsg.h"

fstream 	m_file;

extern 	INETDBLIMIT* 	g_piNetLimit ;
extern 	BOOL			g_biNetLimit ;
extern 	WORD			g_wiNetLimitDBNum;
BOOL  bKeyEffect = FALSE;//JOYCE2011-1-19 
IMPLEMENT_DYNCREATE(CtmTaskCmd, CTaskCmd)
int	g_nCount = 0;
#ifdef  D_DEMO
extern int g_nDemo;
#endif 

typedef void * (* PMANUALRUN)(void *);/*JOYCE2014-8-19*/
typedef void *(* PRSRUN)(void *);

CtmTaskCmd::CtmTaskCmd(): CTaskCmd()
{
	//RegisterMessage(MSG_KEY, m_idMe);
	//RegisterMessage(MSG_MMI, m_idMe);
	//remove by Wong 2015/3/11 13:59:26
	m_SaveScreenThreadID=-1;
	m_ScanManualThread=-1;
#if defined(D_PXA270)&&(!defined(D_CANMASTER))
	CTaskCmd::CTaskCmd();
	pthread_create(&m_ScanManualThread, NULL, (PMANUALRUN)ManualRun, this);
#endif
    
   	m_nSaveScreenFlag = 0;
	pthread_create(&m_SaveScreenThreadID, NULL, (PRSRUN)_SaveScreen, this);
}

CtmTaskCmd::~CtmTaskCmd()
{
	//UnRegisterAllMessage(m_idMe);
	if(m_SaveScreenThreadID>=0)
		pthread_cancel(m_SaveScreenThreadID);
	#if defined(D_PXA270)&&(!defined(D_CANMASTER))
		pthread_cancel(m_ScanManualThread);  /*JOYCE2015-4-14 發送manual key線程沒銷毀，導致重複發送從而通訊中斷以及當機問題*/
	#endif
}

//	execute it atfer the contructor
int		CtmTaskCmd::CreateSelf()
{
	CTaskCmd::CreateSelf();
	if (m_wTasksCount++ == 0)
    	AddCounter((long __far*)&m_lCounter, m_idMe);
	else
		AddCounter((long __far*)&m_lTestCounter, m_idMe);
		
	//AddCounter(&m_lBackCounter, m_idMe);	//	2007/11/19 03:10下午 fans ad
	m_lTestCounter = CMD_COUNTER;
//	m_wSP = 0;
//	m_pwndFocus = NULL;
	m_bBypass = FALSE;
//	m_bHexadecimal = FALSE;
	RegisterMessage(MSG_KEY, m_idMe);
	RegisterMessage(MSG_MMI, m_idMe);
	RegisterMessage(MSG_SH_INET_LIMIT_SAVE_REV, 		m_idMe);
	RegisterMessage(MSG_NB_HMI_KEYNOCE, 		m_idMe);
	return 0;
}

//	execute it before the destructor
int		CtmTaskCmd::FreeSelf()
{
	UnRegisterMessage(MSG_KEY, m_idMe);
	UnRegisterMessage(MSG_MMI, m_idMe);
	UnRegisterMessage(MSG_NB_HMI_KEYNOCE, 		m_idMe);
	UnRegisterMessage(MSG_SH_INET_LIMIT_SAVE_REV, 		m_idMe);
	//UnRegisterAllMessage(m_idMe);
	if (--m_wTasksCount == 0)
    {
//#ifdef      D_DEMO
    	if (g_ptaskdemo != NULL) delete g_ptaskdemo;
//#endif
    	DeleteCounter((long __far*)&m_lCounter, m_idMe);
    }
    else
    	DeleteCounter((long __far*)&m_lTestCounter, m_idMe);
    	
    //	2007/11/19 03:09下午 fans add
    //DeleteCounter(&m_lBackCounter, m_idMe);
	return CTaskCmd::FreeSelf();
}

void	CtmTaskCmd::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case MSG_SH_INET_LIMIT_SAVE_REV:		//接收iNet發來的消息，保存iNet上下限成功。
			if (access("/usr/i86/lib/inet/iNetLimit.dat", R_OK) == 0)
			{
				m_file.open("/usr/i86/lib/inet/iNetLimit.dat", ios_base::in|ios_base::out|ios_base::binary);
				m_file.read((char *)(&g_wiNetLimitDBNum),sizeof(g_wiNetLimitDBNum));
				if(g_piNetLimit !=NULL) 
				{
					delete []g_piNetLimit;
					g_piNetLimit = NULL; 
				}
				g_piNetLimit = new INETDBLIMIT[g_wiNetLimitDBNum] ;
				m_file.read((char *)(g_piNetLimit),sizeof(INETDBLIMIT)*g_wiNetLimitDBNum);
				m_file.close();
				g_biNetLimit = TRUE;
			}
			break;
		case MSG_NB_HMI_KEYNOCE:	//JOYCE2011-1-19 
			bKeyEffect =TRUE;
			break;
		default:
			break;
	}
	CTask::WndProc(message, wParam, lParam);
}

void	CtmTaskCmd::Run()
{
	static	int	nPrintKeyNum 	= 0;
	static	int	nRunNum 		= 0;
	
	
	KEYCODE key;
	KEYFLAG keyflag;
	CtmWnd*   pwndAlpha;
	
	nRunNum++;

	BYTE	u_bCommState = 0;	//2008-10-23
	WORD	u_bConfState = 0;	//2009-11-6
	//if(g_ptaskdsp != NULL)
	//{
	//	g_ptaskdsp->ReqValues(0x7000, &u_bCommState, sizeof(u_bCommState));		//2008-10-23	
	//	g_ptaskdsp->ReqValues(0x7001, &u_bConfState, sizeof(u_bConfState));		//2009-11-6
	//	if((u_bConfState == 1)&&(g_wDspStatus)) return;
	//}
	//====================
	//	backup Sram 2007/11/19 03:38下午
	//====================
	//if (m_lBackCounter <= 0)
	//{
	//	m_lBackCounter	= CNT_LONG_BACKUP_SRAM_TIME;
	//	pthread_t	m_BackUpThread;
	//	pthread_create(&m_BackUpThread, NULL, (PRSRUN)BackUpSram, NULL);
	//}
/****************************************************************************+
|           Message process                                                  |
+****************************************************************************/
	MSG         msg;
	//int			nValue;
	
    if (g_pApplication->GetMessage(CMD_SYSTEM, &msg))
    {
    	switch (msg.wMessage)
        {
        case _SYS_COMMAND:
           if(msg.message == _SYS_COMMAND)
            {
            	if (!m_pwndFocus->OnCommand((char *)msg.lParam))              // If the _SYS_COMMAND message is not processed
               	{
               		;//g_pApplication->SetMessage(CMD_SYSTEM, &msg);   // Put it in the tail of message queue
               	}
            }
           	else if (!m_pwndFocus->OnCommand(msg.wCmd));              // If the _SYS_COMMAND message is not processed
                //g_pApplication->SetMessage(CMD_SYSTEM, &msg);   // Put it in the tail of message queue
            break;
        default:
            g_pApplication->SetMessage(CMD_SYSTEM, &msg);
            break;
        }
    }
    
/****************************************************************************+
|           Key process                                                      |
+****************************************************************************/
	if(_TestKeyFlag((KEYFLAG __far*)&keyflag) != _NULL_KEY)
    {
    	key.wCode = _GetKey();
    	//printf("key.wCode :%4X\n",key.wCode);
    	//printf("key.wCode=%x m_bSystemIdle=%d m_wTasksCount=%d \n", key.wCode, m_bSystemIdle, m_wTasksCount);

		if((GetDBValue("SYSX_OTHERS_OTHERS_INT_RESERVED74").lValue == 0x1234) && ((key.wCode == _ENTER)||(key.wCode == _KEY_CLEAR)))	 return;  	/*JOYCE2014-9-30 把確定/取消按鍵鎖死,確保必須重啟*/ 

		if( bKeyEffect && ((key.wCode ==_ENTER)||(key.wCode ==_CTRL_C)))	//JOYCE2011-1-19  
			key.wCode = _NULL_KEY;

    	int IStarKeyValue=0; /*星鍵將用與輸入負號*/
   		if((key.wCode ==0x92A )&& (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue == ID_VIEW_AUSTONE))
   			{
   			IStarKeyValue = key.wCode =0x02D;
   			//printf("IStarKeyValue=%x\n",IStarKeyValue);
   			}
   		else
   			IStarKeyValue=0x92A;

		if (key.wCode == IStarKeyValue && !m_bSystemIdle && m_wTasksCount <= 2 /*== 1*/)
		{
			if (GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue !=ID_VIEW_PANEL
				 && GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue != ID_VIEW_MOLDSAVE		//fuxy, 2010-11-16, modify for input *  in  ID_VIEW_MOLDSAVE
     			&& GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue != ID_VIEW_AUSTONE
     			&& GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue != ID_VIEW_OVERVIEW) /*JOYCE2015-10-20  停機功能分期付款功能,主畫面組合鍵*+.*/
     			key.wCode = _NULL_KEY; //ZHBA 2010-3-25
			else if(GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue == ID_VIEW_MOLDSAVE)
				key.wCode = _MINUS; 

			nPrintKeyNum ++;
			//printf("nPrintKeyNum=%d, nRunNum=%d\n", 
			//	nPrintKeyNum, nRunNum);
			if(nPrintKeyNum == 1)
			{
				nRunNum=0;
			}
			if (nRunNum <= 10) //JOYCE2010-11-16  6
			{
				if (nPrintKeyNum >= 2)
				{
					//SaveScreen();
					m_nSaveScreenFlag = 1;
					nPrintKeyNum = 0;
					nRunNum = 0;
				}
			}
			else
			{
				nPrintKeyNum = 0;
				nRunNum = 0;
			}
				
		}
		else
		{
			nPrintKeyNum 	= 0;
			nRunNum 		= 0;
		}

		if((m_nSaveScreenFlag == 1) && (key.wCode != 0xFFFF)) return;   /*JOYCE2013-10-11 增加判斷條件,允許按鍵放開操作*/
/*******
|       Check if now sleeping
*******/
    if (m_bSystemIdle)                //Chuntzu 2004/5/27 for screen saver
        {
        if ((key.wCode == _KEY_PANEL_MOLDCLOSE)||(key.wCode == _KEY_PANEL_DOOROPEN)||
            (key.wCode == _KEY_PANEL_DOORCLOSE)||(key.wCode == _KEY_PANEL_BREAK))
            {
            if(u_bCommState == COMM_STATE_RUN)	//2008-10-23
            	g_ptaskdsp->WriteValue(COMM_SENDPANEL,key.wCode);
            //g_ptaskrs232->SendPanel(LOBYTE(key.wCode));
            return;
            }
        goto __KEY_PROCESS_EXIT;
        }
	//if ((((WORD)(GetDBValue("STATE_OTHR_OTHR_NULL_OPERMODE").lValue) & 0x0010) == 0x0000) && (key.wCode == _KEY_PANEL_INJECT) &&
	//	g_ptaskkeb != NULL)
	//	g_ptaskkeb->SendInjectStartIndex();//zholy061228
	
  if(u_bCommState == COMM_STATE_RUN)	//2008-10-23
  { 
    if (key.wCode == _ALT_S) g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_SEMIAUTO & 0x00FF);
    if (key.wCode == _ALT_T) g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_TIMEAUTO & 0x00FF);
    if (key.wCode == _ALT_B) g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_MANUAL & 0x00FF);
    if (key.wCode == _ALT_A) g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_MOLDCLOSE & 0x00FF);
    if (key.wCode == _ALT_C) g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_MOLDOPEN & 0x00FF);
    if (key.wCode == _ALT_F) g_ptaskdsp->WriteValue(COMM_SENDPANEL, _KEY_PANEL_CHARGE & 0x00FF);
   }
		//=======
		//       Standard keyboard preprocess
		//=======
	    if((key.wCode == _SPACE) && keyflag.Ctrl) key.wCode = _KEY_ALPHA;
	    if((key.cASCII >= ASCII_NORMAL_START) && 
	       (key.cASCII <= ASCII_NORMAL_END) && 
	       (key.cScan != _KEY_PANEL_) )
	         key.cScan = 0;

#ifdef      D_DEMO
/*******
|       Demonstration process
*******/
	

    if (key.wCode == _F1)
        {
        if (g_nDemo++ != 0)
            {
            if (g_ptaskdemo == NULL && GetDBValue("SYS_HMI_NULL_NULL_VIEWID").lValue == ID_VIEW_OVERVIEW)
                {
                if (g_nDemo >= 2)
                	{
	                g_ptaskdemo = new CTaskDemo;
	                g_nDemo = 0;
                	}
                }
            else
                {
                if (g_ptaskdemo != NULL) delete g_ptaskdemo;
                g_ptaskdemo = NULL;
                g_nDemo = 0;
                }
            }
        }
    else
        {
        g_nDemo = 0;
        }
//
#endif

//=======
// Route directly to the focus window if Bypass flag is set
//=======
    	if(m_bBypass && (m_pwndFocus != NULL))
        {
	        m_pwndFocus->OnKey(key.wCode);
	        goto __KEY_PROCESS_EXIT;
        }

//=======
//       Special keyboard preprocess - Key Lock check
//=======
/*    if (_GetKeyLock() || (!dbkeyrdy))
        if (((key.wCode >= '0') && (key.wCode <= '9')) || (key.wCode == '*') || (key.wCode == '.') || (key.wCode == _KEY_CLEAR))    //wy040699
            goto __KEY_PROCESS_EXIT;
        else if ((key.wCode == _DELETE ) || (key.wCode == _PAGEUP) || (key.wCode == _PAGEDN) || (key.wCode == _UARROW) || (key.wCode == _DARROW) || (key.wCode == _LARROW) || (key.wCode == _RARROW))   //<<<WY040699  
                goto __KEY_PROCESS_EXIT;                    // >>>WY040699
            else if ((key.wCode == _KEY_LEFTFIELD ) || (key.wCode == _KEY_RIGHTFIELD) || (key.wCode == _KEY_UPFIELD) || (key.wCode == _KEY_DOWNFIELD))   //<<<WY040699  
                    goto __KEY_PROCESS_EXIT;*/
//========
//      Special keyboard preprocess - adjust LCD bright
//========
/*	if (dbsyst.wIDView == ID_VIEW_OVERVIEW)
	    {
	    if (key.wCode == _KEY_UPFIELD)
	        {
	        if (dblcd.cBrighter >= 100) dblcd.cBrighter = 50;
	        else                        dblcd.cBrighter += 1; //5;
	        SaveSystem();
	        _VideoBrighter((dblcd.cBrighter)*2+50);                                             //wy102500
	        }
	    else if (key.wCode == _KEY_DOWNFIELD)
	        {
	        if (dblcd.cBrighter <= 0) dblcd.cBrighter = 50;
	        else                      dblcd.cBrighter -= 1; //5;
	        SaveSystem();
	        _VideoBrighter((dblcd.cBrighter)*2+50);                                             //wy102500
	        }
	    }*/
/*******
|       Special keyboard preprocess - Do NOT wake up the system if it is a panel key
*******/
    if (HIBYTE(key.wCode) == _KEY_PANEL_)
        {
        if(g_pExTable != NULL)
        	key.wCode = g_pExTable->GetTableValue(4, (int *)&key.wCode);

		//key.wCode = _KEY_PANEL_MOLDOPEN;
        if(u_bCommState == COMM_STATE_RUN)	//2008-10-23
        {
        	if(bKeyEffect)		//JOYCE2011-1-19 拔電池后,開機時鎖key 
	       		return;
        	else	
        	{
	        	g_ptaskdsp->WriteValue(COMM_SENDPANEL, key.wCode);
	        	/*JOYCE2015-11-6 55是net通訊了*/
	        	//if(g_ptaskCom2dsp != NULL)		//James add 當按鍵是按下或者松開的時候，同時也要傳給COM2
	        	//{
	        	//	if ((key.wCode == _KEY_PANEL_HEATER)||(key.wCode == _KEY_PANEL_HEATEROFF)||(key.wCode == _KEY_PANEL_BREAK))
	        	//	{
	        	//		g_ptaskCom2dsp->WriteValue(COMM_SENDPANEL, key.wCode);
	        	//	}
	        	//}
	        }
        }	
        if (m_lCounter < 0) m_bIdle = TRUE;
        return;
        }  
//=======
//       Special keyboard preprocess - Hexadecimal support
//=======
/*		//2010-11-16, mark for not need * +0-5 to input hex 
    	if (key.wCode == _KEY_HEXADECIMAL)
        	m_bHexadecimal = TRUE;
    	else
        {
	        if ((key.wCode >= '0') && (key.wCode <= '5') && m_bHexadecimal) 
	        	key.wCode += 'A' - '0';     //wy000103
	        m_bHexadecimal = FALSE;
        }
*/
//=======
//      Handle event
//=======
	    if (m_pwndFocus != NULL)
	    {
            m_pwndFocus->OnKey(key.wCode);
	    }

//=======
// Reset system idle flag
//=======
__KEY_PROCESS_EXIT:
	    m_lCounter = CMD_COUNTER;
	    m_bIdle = FALSE;
    }
	else if (m_lCounter < 0)
    {
	    m_bIdle = TRUE;
	    nPrintKeyNum 	= 0;
		nRunNum 		= 0;
    }
}

void 	ManualRun(WORD	wCode)/*JOYCE2014-8-19 發送手動按鍵,有效的前提是主機有加相應的手動功能,會接收手動鍵,否則無效*/
{
	int			nKeyManual=0;
	static	BOOL		bKey = FALSE;
	WORD		wKeyManualCode=0xFF90;
	pthread_detach(pthread_self());	                                         /*JOYCE20150423 解決內存泄露導致當機問題*/
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)        /*JOYCE20150423 解決內存泄露導致當機問題*/
	{
		;		
	}
	//printf("manual hread_id=%d\n",pthread_self());
	while (TRUE)
	{
		nKeyManual = ioctl(g_nRTCFd, 0x4010, 1);
		//printf("nKeyManual=%d g_ptaskdsp=%x bEnter=%d\n", nKeyManual, g_ptaskdsp, bKey);
		if(nKeyManual == 0 && bKey == FALSE)
		{
			bKey = TRUE;
			wKeyManualCode = 0xFF90;
			if(g_ptaskdsp != NULL)
			g_ptaskdsp->WriteValue(COMM_SENDPANEL,wKeyManualCode);
		}
		if(nKeyManual == 1 && bKey == TRUE )
		{
			bKey = FALSE;
			wKeyManualCode = 0xFFFF;
			if(g_ptaskdsp != NULL)
			g_ptaskdsp->WriteValue(COMM_SENDPANEL,wKeyManualCode);
		}
		usleep(5000);
	}
}

void	_SaveScreen(CtmTaskCmd *pTaskCmd)
{
	pthread_detach(pthread_self());	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0)
	{
		;		
	}
	while(1)
	{
		if(pTaskCmd->m_nSaveScreenFlag == 1)
		{
			SaveScreen();
			pTaskCmd->m_nSaveScreenFlag = 0;
		}
		usleep(50*1000);	
	}
}

#ifndef	D_BEOBJECT_CTMTASKCMD
CObject*	CreateInstance(char* Name)
{
	//if (g_ptaskCmd == NULL)
//	{
		CObject*	pResult = Create(Name);

		if (pResult != NULL)
		{
			(static_cast<CTaskCmd*>(pResult))->CreateSelf();
		}
		//g_ptaskCmd = static_cast<CTaskCmd*>(pResult);
	//}
	
	
	return pResult;
	
	
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CTaskCmd*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}
#endif
