/*===========================================================================+
|  Class    : TaskMoni                                                       |
|  Task     : System Monitor Service Routine                                 |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.50 -                                                   |
|  Link     : MSVC V1.50 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : Quen                                                           |
|  Version  : V1.00                                                          |
|  Creation : 07/01/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/
#include    "utils.h"
#include    "taskmoni.h"
#include    "main.h"
#include	"_kb.h"
#include	"commonaction.h"	//Sunny<20070405>
#include	"tmdebug.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//IMPLEMENT_DYNCREATE(CTaskMoni, CTask)
IMPLEMENT_DYNCREATE(CTaskMoni, CtmPackClass)

CTaskMoni*          g_ptaskMoni		= NULL;
//******
//      Initialize static class member
//******
#define		TIME_MONI_SAVEOPERATION			3600*1024	//LEO20070404
#define		PER_MINUTE	60 * 1000 //1000*2
static		tmDATE	u_Currentdate;

WORD		g_wTaskDSP54;
/*===========================================================================+
|           Class implementation - TaskMoni                                  |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskMoni::CTaskMoni() : CTask()
{
	//m_dwShotCountCurrent = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
	//m_bIdle = FALSE;
	//m_lCounter = GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERTIME").lValue * PER_MINUTE;
	//m_lHeatCounter = 1000;
	//m_bSaver = FALSE;
	//m_pwndScreen = NULL;
	////LEO20070404
	//m_lCounterRun 		= 
	//m_lCounterRunAuto 	= 
	//m_lCounterRunMotor	= (long)60*1024;
	//m_lCounterAlarm		= 0;
	//m_lCounterSaveOpt	= TIME_MONI_SAVEOPERATION;
	//GetDate(&u_Currentdate);
	///*#ifdef	D_X86
	//m_lCounterRun = (long)1000;
	//::AddCounter((long __far*)&m_lCounterRun, m_idMe);
	//#endif*/
	//
	////LEO20070404
	//::AddCounter((long __far*)&m_lCounterRun, m_idMe);
	//::AddCounter((long __far*)&m_lCounterRunAuto, m_idMe);
	//::AddCounter((long __far*)&m_lCounterRunMotor, m_idMe);
	//::AddCounter((long __far*)&m_lCounterSaveOpt, m_idMe);
	//
	//::AddCounter((long __far*)&m_lCounter, m_idMe);
	//::AddCounter((long __far*)&m_lHeatCounter, m_idMe);
}

CTaskMoni::~CTaskMoni()
{
	////LEO20070404
	//::DeleteCounter((long __far*)&m_lCounterSaveOpt, m_idMe);
	//::DeleteCounter((long __far*)&m_lCounterRunAuto, m_idMe);
	//::DeleteCounter((long __far*)&m_lCounterRunMotor, m_idMe);
	////#ifdef	D_X86
	//::DeleteCounter((long __far*)&m_lCounterRun, m_idMe);
	////#endif
	//::DeleteCounter((long __far*)&m_lCounter, m_idMe);
	//::DeleteCounter((long __far*)&m_lHeatCounter, m_idMe);
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
//void        CTaskMoni::Run()
//{
//	int nShotCount = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
//	static	int	nCount = nShotCount;//dbbuff.wSwitchToAutoShutCount ;	// 目標產品總數		James modify 2007/9/21 12:35上午
//	static  int nPackCount = nShotCount;								// 保存上一次的目標包裝總數
//	if (/*dbbuff.wSwitchToAutoShutCount*/ nShotCount == 0)				// James modify 2007/9/21 12:35上午
//	{
//		nCount = 0;
//	}
//	BOOL	bScreenSaverLogoutOption=GetDBValue("SYSX_HMI_NULL_NULL_SCREENSAVERLOGOUTOPTION").lValue;//屏保後自動登出
//	
//	m_bIdle = g_ptaskCmd->IsIdle();
//	if(m_bIdle == TRUE && m_bSaver == FALSE)
//	{
//		if (m_lCounter < 0 || 
//			(/*dbbuff.wSwitchToAutoShutCount*/nShotCount - nCount) >= GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERSHOTCOUNT").lValue)
//		{
//			m_lCounter = GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERTIME").lValue * PER_MINUTE;
//			//printf("will do ActivateScreenSaver,m_lCounter = %d\n",m_lCounter);
//			#ifndef      D_DEMO
//			ActivateScreenSaver(TRUE);
//			#endif
//			if(bScreenSaverLogoutOption && g_iPrivilege != 1)
//			{
//				g_iPrivilege = 1;
//				g_pszUserID = NULL;
//				Login();
//			}
//		}
//	}
//	if(m_bIdle == FALSE)
//	{
//		if (m_bSaver == TRUE)	
//		{
//			ActivateScreenSaver(FALSE);
//			nCount = nShotCount/*dbbuff.wSwitchToAutoShutCount*/;
//		}
//		m_lCounter = GetDBValue("SYS_HMI_NULL_NULL_SCREENSAVERTIME").lValue * PER_MINUTE;
//		
//		//printf("Idle FALSE Counter=%ld\n", m_lCounter);
//	}
//
//	DWORD	dwShotCountSetting = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_SETSHOTCOUNT").lValue;
//	DWORD	dwShotCountCurrent = GetDBValue("PROD_PRODUCT_PRODUCT_NULL_REALSHOTCOUNT").lValue;
//	
//	if ((dwShotCountSetting != 0) 							&&
//		(m_dwShotCountCurrent != dwShotCountCurrent)		&&
//		(dwShotCountCurrent >= dwShotCountSetting)			&&
//		!(dwShotCountCurrent & 0x80000000))
//	{
//		g_ptaskdsp->WriteData(COMM_SENDSTATUS, COMM_S_REACHSHOTCOUNT);
//		//g_ptaskrs232->SendStatus(COMM_S_REACHSHOTCOUNT);
//	}
//	
//	DWORD	dwPackCountSetting = GetDBValue("PROD_PRODUCT_PACK_NULL_SETSHOTCOUNT").lValue;
//	if ((dwPackCountSetting != 0) 						&&
//		(m_dwShotCountCurrent != dwShotCountCurrent)	&&
//		((dwShotCountCurrent - nPackCount) >= dwPackCountSetting ) )			//解決中途更改包裝數第一次會不對 James add 2007/9/22 11:11上午
//	{
//		nPackCount = dwShotCountCurrent;
//		if (dwShotCountCurrent != 0) g_ptaskdsp->WriteData(COMM_SENDSTATUS,COMM_S_REACHPACKCOUNT);
//		SetDBValue("PROD_PRODUCT_PACK_NULL_REALSHOTCOUNT", 0);
//	}
//    
//    if(m_dwShotCountCurrent != dwShotCountCurrent)  //Gwei 2005/5/16加入以解決同一目標產品總數,第二次會多一模才送COMM_S_REACHSHOTCOUNT的問題
//    m_dwShotCountCurrent = dwShotCountCurrent;
//	
//	/*******
//	|       Process save operation
//	*******///LEO20070404
//	ProcessSaveOperation();
//	/*******
//	|       Process machine timing
//	*******///LEO20070404
//	ProcessAutoRun();
//	/*******
//	|       Process motor working time
//	*******///LEO20070404
//	ProcessMotorRun();
//	/*******
//	|       process stop operation
//	*******///LEO20070404
//	ProcessStopMachine();
////#ifdef	D_X86
///*2007/4/11 03:59下午 yangpeng
//if(m_bSaver == TRUE && m_lCounterRun <= 0)
//	{
//	m_lCounterRun	= 50;
//	}
//
//*/
////if (m_lHeatCounter < 0)
////	{
////	CountZero();
////	m_lHeatCounter = 5000;
////	}
//
////#elif	D_ARM
// if (m_lHeatCounter < 0)
//	{
//	ClearZeroOil();
//	OnTimeHeater();
//	TestBatPower();			//yang 2007/7/31 for test battery power
//	m_lHeatCounter = 1000;
//	if (g_ptaskCmd->GetFocus() != NULL && !g_wTaskDSP54) 
//		g_ptaskCmd->GetFocus()->Update();
//
//	}    
//#endif

//}

/*===========================================================================+
|           Class implementation - Screen                                    |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/

CScreen::CScreen(tmWND* pwnd) : CtmWnd()
{
SetBorderWidth(0);
SetBGColor(BLACK);
SetFGColor(YELLOW);        //wy081399
Create(pwnd);
Clear();
srand(time(0));
for(int i = 0; i < 10; i ++)
	m_BmpInfo[i].pBMP	= NULL;
m_size = CtmWnd::GetStringExtent(g_MultiLanguage["MSG_BANNER"]);
m_nX = GetClientRight();
m_nY = rand() % (GetClientBottom() - m_size.h);
}

CScreen::~CScreen()
{
	for(int i= 0; i < 10; i ++)
		if(m_BmpInfo[i].pBMP != NULL)	delete m_BmpInfo[i].pBMP;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/

void        CScreen::Show()
{
	int i, j, nX, nY;
	_SetViewPort(m_rectClient.left, m_rectClient.top, 
				 m_rectClient.right, m_rectClient.bottom);
	for(i = 0; i < 10; i ++)
	{
		if(m_BmpInfo[i].pBMP == NULL)	return;
		
		nX	= rand() & 0x01;
		nY	= rand() & 0x01;
		
		if(nX == nY && nX == 0)
			nX	=
			nY	= 1;
		
		for(j = 0; j < i; j ++)
		{
			if(m_BmpInfo[i].rect.left > m_BmpInfo[j].rect.right)	continue;
			if(m_BmpInfo[i].rect.right < m_BmpInfo[j].rect.left)	continue;
			if(m_BmpInfo[i].rect.bottom < m_BmpInfo[j].rect.top)	continue;
			if(m_BmpInfo[i].rect.top > m_BmpInfo[j].rect.bottom)	continue;
					
			if(m_BmpInfo[i].nXState != m_BmpInfo[j].nXState)
			{
				if(m_BmpInfo[i].rect.left < m_BmpInfo[j].rect.left && m_BmpInfo[i].rect.right >= m_BmpInfo[j].rect.left)
				{
					if(m_BmpInfo[i].nXState == 1)	m_BmpInfo[i].nXState = -1;
					if(m_BmpInfo[j].nXState == -1)	m_BmpInfo[j].nXState = 1;
				}
				else if(m_BmpInfo[i].rect.right  > m_BmpInfo[j].rect.right && m_BmpInfo[i].rect.left <= m_BmpInfo[j].rect.right)
				{
					if(m_BmpInfo[i].nXState == -1)	m_BmpInfo[i].nXState = 1;
					if(m_BmpInfo[j].nXState == 1)	m_BmpInfo[j].nXState = -1;
				}
			}
			if(m_BmpInfo[i].nYState != m_BmpInfo[j].nYState)
			{		
				if(m_BmpInfo[i].rect.top < m_BmpInfo[j].rect.top && m_BmpInfo[i].rect.bottom >= m_BmpInfo[j].rect.top)
				{
					if(m_BmpInfo[i].nYState == 1)	m_BmpInfo[i].nYState = -1;
					if(m_BmpInfo[j].nYState == -1)	m_BmpInfo[j].nYState = 1;
				}
				else if(m_BmpInfo[i].rect.bottom > m_BmpInfo[j].rect.bottom && m_BmpInfo[i].rect.top <= m_BmpInfo[j].rect.bottom)
				{
					if(m_BmpInfo[i].nYState == -1)	m_BmpInfo[i].nYState = 1;
					if(m_BmpInfo[j].nYState == 1)	m_BmpInfo[j].nYState = -1;
				}
			}
		}
		
		if(nX == 1)
		{
			if(m_BmpInfo[i].rect.left <= 0)
				m_BmpInfo[i].nXState	= 1;
			else if(m_BmpInfo[i].rect.left >= m_BmpInfo[i].nXMax)
				m_BmpInfo[i].nXState	= -1;
			
							
		}
		if(nY == 1)
		{
			if(m_BmpInfo[i].rect.top <= 0)
				m_BmpInfo[i].nYState	= 1;
			else if(m_BmpInfo[i].rect.top >= m_BmpInfo[i].nYMax)
				m_BmpInfo[i].nYState	= -1;							
		}
		
		_SetColor(0);
		if(nX == 1)
		{
			if(m_BmpInfo[i].nXState == 1)
			{
				_MoveTo(m_BmpInfo[i].rect.left, m_BmpInfo[i].rect.top);
				_LineTo(m_BmpInfo[i].rect.left, m_BmpInfo[i].rect.bottom);
			}
			if(m_BmpInfo[i].nXState == -1)
			{
				_MoveTo(m_BmpInfo[i].rect.right, m_BmpInfo[i].rect.top);
				_LineTo(m_BmpInfo[i].rect.right, m_BmpInfo[i].rect.bottom);
			}
		}
		
		if(nY == 1)
		{
			if(m_BmpInfo[i].nYState == 1)
			{
				_MoveTo(m_BmpInfo[i].rect.left, m_BmpInfo[i].rect.top);
				_LineTo(m_BmpInfo[i].rect.right, m_BmpInfo[i].rect.top);
			}
			if(m_BmpInfo[i].nYState == -1)
			{
				_MoveTo(m_BmpInfo[i].rect.left, m_BmpInfo[i].rect.bottom);
				_LineTo(m_BmpInfo[i].rect.right, m_BmpInfo[i].rect.bottom);
			}
		}
	
//		Clear();		
		if(nX == 1)
		{	
			m_BmpInfo[i].rect.left 	+= m_BmpInfo[i].nXState;
			m_BmpInfo[i].rect.right	+= m_BmpInfo[i].nXState;
		}
		if(nY == 1)
		{
			m_BmpInfo[i].rect.top 	+= m_BmpInfo[i].nYState;
			m_BmpInfo[i].rect.bottom+= m_BmpInfo[i].nYState;
		}
		
		if(nX != 0 || nY != 0)
			m_BmpInfo[i].pBMP->Show(m_BmpInfo[i].rect, 0);
	}
	
}

int        CScreen::AddBMP(char *pszBMP)
{
	int	nIndex = 0;
	if(m_nBMPCount >= 10)	return -1;
	if(pszBMP == NULL)		return -1;
	while(m_BmpInfo[nIndex].pBMP != NULL) nIndex ++;
	if(nIndex >= 10)		return -1;
		
	m_BmpInfo[nIndex].pBMP	= new CtmBitMap;
	if(m_BmpInfo[nIndex].pBMP->Create(pszBMP) != 0)
	{
		delete m_BmpInfo[nIndex].pBMP;
		m_BmpInfo[nIndex].pBMP	= NULL;
		return -1;
	}
	
	m_BmpInfo[nIndex].nXMax	= GetClientWidth() - m_BmpInfo[nIndex].pBMP->m_lWidth;
	m_BmpInfo[nIndex].nYMax	= GetClientHeight() - m_BmpInfo[nIndex].pBMP->m_lHeight;
	
	m_BmpInfo[nIndex].rect.left		= rand() % m_BmpInfo[nIndex].nXMax;
	m_BmpInfo[nIndex].rect.top		= rand() % m_BmpInfo[nIndex].nYMax;
	m_BmpInfo[nIndex].rect.right	= m_BmpInfo[nIndex].rect.left + m_BmpInfo[nIndex].pBMP->m_lWidth;
	m_BmpInfo[nIndex].rect.bottom	= m_BmpInfo[nIndex].rect.top + m_BmpInfo[nIndex].pBMP->m_lHeight;
	if((rand() & 0x01) == 0) m_BmpInfo[nIndex].nXState	= -1;
	else					 m_BmpInfo[nIndex].nXState	= 1;
	if((rand() & 0x01) == 0) m_BmpInfo[nIndex].nYState	= -1;
	else					 m_BmpInfo[nIndex].nYState	= 1;
	
	return 0;
}

WORD        CScreen::OnKey(WORD wKey)
{
return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

