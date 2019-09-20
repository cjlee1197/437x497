/*===========================================================================+
|  Function : Splash                                                         |
|  Task     : Splash                                                         |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 08/22/1997                                                     |
|  Revision : V2.00                                                          |
|             10/21/1995 by C.C. Chen                                        |
|             - Change Splash from a function to a task                      |
+===========================================================================*/

#include    "stable.h"
#include    "utils.h"                                   
#include    "main.h"
#include    "splash.h"
#include	"tmconfig.h"
//#include	"update.h"	//LEO20070421
#include	"imagebox.h"
#include	"commonaction.h"
#include	<arpa/inet.h>
/*===========================================================================+
|           External                                                         |
+===========================================================================*/

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//******
//      Initialize static class member
//******
WORD        CTaskSplash::m_wTasksCount = 0;

long		m_lSplashTimer		= 3500;

long		m_lLogoTime			= 5000;
WORD		g_wExternLogoTime	=0;

long		g_lGifLogoTime = 0;
CtmWnd*		g_pGifLogo = NULL;
extern 		int 	g_FbIotcol;

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/

/*===========================================================================+
|           Class implementation - CtmWndSplash                                |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CtmWndSplash, CtmWnd)
CtmWndSplash::CtmWndSplash() : CtmWnd()
{
	strcpy(TYPEName, "CtmWndSplash");	
	m_pszLogo = NULL;
	m_pbmpLogo = NULL;
	m_pszVersion = NULL;
	m_pszVersionID = NULL;
	
	m_iTextLeft = 680;
	m_iTextTop = 570;
	m_bKeyIn = FALSE;
	
}

//	fans add
BOOL 	CtmWndSplash::CreateA()
{
	char    szTemp[40];
	char    *psz;
	WORD    *pw;
	
	CtmWnd::CreateA();
	    
	CtmWnd* pwnd = FindControlFromName("StaticCAPTION");
	
	if(pwnd != NULL)
	{
	    psz = *(char **)(pwnd->GetPropValue("text"));
	    if(psz != NULL)
	    {
    	    
    	    pw  = (WORD *)szTemp;
    	    while(*psz != 0)
    	    {
    	        *pw = *psz;
    	        pw ++;
    	        psz ++;
    	    }
    	    *pw  = 0;
    	    
    	    pwnd->SetPropValueT("text", szTemp);	  
    	}  
	}
	
	pwnd = FindControlFromName("StaticUpdataMMI");
	
	if(pwnd != NULL)
	{
	    psz = *(char **)(pwnd->GetPropValue("text"));
	    if(psz != NULL)
	    {
    	    pw  = (WORD *)szTemp;
    	    while(*psz != 0)
    	    {
    	        *pw = *psz;
    	        pw ++;
    	        psz ++;
    	    }
    	    *pw  = 0;
    	    
    	    pwnd->SetPropValueT("text", szTemp);	
    	}    
	}
	g_pGifLogo = FindControlFromName("gifLogo");
	pwnd = FindControlFromName("StaticIP");	//2016/5/23 ¤U¤È 04:11:58
	if(pwnd!=NULL)
	{
		int sock_get_ip;  
	  char ipaddr[50]; 
		char	szTemp[50]="\0";
		struct   sockaddr_in *sin;  
	  struct   ifreq ifr_ip;     
	  if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) != -1)  
	  {   
		  memset(&ifr_ip, 0, sizeof(ifr_ip));     
		  strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);
		  if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) >= 0 )
		  {
			  sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
			  strcpy(ipaddr,inet_ntoa(sin->sin_addr));
			  CodeChange(szTemp, ipaddr);
				pwnd->SetPropValueT("text", szTemp);
			}
		}
		close(sock_get_ip);
	}
}


void 	CtmWndSplash::SetPropertys()
{
	CtmWnd::SetPropertys();
	
	//SetPropInfo("textleft", 	tmINT,		&m_iTextLeft);
	//SetPropInfo("textTop", 		tmINT,		&m_iTextTop);
	SetPropInfo("logopath",		tmSTR,		&m_pszLogo);
	//SetPropInfo("text", 		tmSTR, 		&m_pszVersion);
	//SetPropInfo("textid", 		tmSTR, 		&m_pszVersionID);
	
}

void 	CtmWndSplash::SetPropertys_Nub()
{
	CtmWnd::SetPropertys_Nub();
	
	SetPropInfo(SPLASH_LOGOPATH,		tmSTR,		&m_pszLogo);
	
}

void	CtmWndSplash::WndProc(int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
    {
        case MSG_MOUSELUP:
            PutKeyTail(_F5);
        	break;
        default:
        	break;
    }
}

void    CtmWndSplash::Show()
{
	static BOOL	bShowFlag = FALSE;
	
	if(bShowFlag) return;
		
	char *pszLogo = NULL;
	//SetBGColor(BLACK);	
	RECT rect;
	MKRECT(rect, m_wnd.rect.left, m_wnd.rect.top, m_wnd.rect.right, m_wnd.rect.bottom);	
	if (m_pbmpLogo == NULL) m_pbmpLogo = new CtmBitMap();
		
	//	fans modify 2009/8/22 10:16¤W¤È
	char	szPath[256];
	pszLogo = CtmConfig::GetInstance()->GetLogoPath();
	if(pszLogo != NULL)
		sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), pszLogo);
	else if (m_pszLogo == NULL)
		sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), tmBMP_TMLOGO);
	else 	
		sprintf(szPath, "%s%s", CtmConfig::GetInstance()->GetBmpPath(), m_pszLogo);

	ioctl(g_FbIotcol, 0xFFFF11, NULL);
	usleep(10*1000);
	m_pbmpLogo->Create(szPath);
	m_pbmpLogo->SetKeyColor(63519);
	_SetViewPort(0,0,rect.right,rect.bottom);
	m_pbmpLogo->Show(rect, tmBMP_FILLRECT | tmBMP_KEYCOLOR);
	//if (m_pszVersionID != NULL) StrIDToStr(m_pszVersionID, &m_pszVersion);
	//
	//if (m_pszVersion != NULL) CtmWnd::Show(m_iTextLeft, m_iTextTop, m_pszVersion);
		
	CtmWnd::Show();
	
	ioctl(g_FbIotcol, 0xFFFF12, NULL);
	//if(g_pGifLogo != NULL)
    //{
    //	gif_animation* 	gifInfo = ((CtmImageGif*)g_pGifLogo)->GetGifInfo();
    //	if(gifInfo != NULL)
	//	{
	//		for(int i = 1; i < gifInfo->frame_count; i++)
	//		{
	//			ControlGif(g_pGifLogo, i, 1,  gifInfo->frame_count, 1,  gifInfo->frame_count, GIF_FWD);	
	//			usleep(gifInfo->frames[i].frame_delay*10*1000);
	//		}
	//	}
	//	//CtmWnd::Show();
    //}
	bShowFlag = TRUE;
}

CtmWndSplash::~CtmWndSplash()
{
	if (m_pbmpLogo != NULL) delete m_pbmpLogo;
}

/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
WORD        CtmWndSplash::OnKey(WORD wKey)
{
//if ((wKey !=_KEY_PANEL_RESET) && (wKey !=_KEY_PANEL_KEYLOCK))

if ((wKey !=_KEY_PANEL_RESET) && (wKey !=_KEY_PANEL_KEYLOCK) && (wKey !=_KEY_PANEL_MANUAL)&&(wKey!=0xFFFF)) //092401
	m_bKeyIn = TRUE;
return _NULL_KEY;
}

/*---------------------------------------------------------------------------+
|           Helpers                                                          |
+---------------------------------------------------------------------------*/

/*===========================================================================+
|           Class implementation - CTaskSplash                               |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskSplash::CTaskSplash() : CTask()
{
	RegisterMessage(MSG_KEY, m_idMe);
	char	szCmd[256];
	
	//	fans add 
	char*	pszName = NULL;
	pszName = CtmConfig::GetInstance()->GetLogoForm();
		
	if (pszName == NULL)
		sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), "splash.txt");
	else
		sprintf(szCmd, "%s%s", CtmConfig::GetInstance()->GetFormPath(), pszName);

	m_parseSplash.OpenFile(szCmd);
	m_parseSplash.CloseFile();
	RegionUpdateStop=TRUE;
	//m_parseSplash.OpenFile("res/form/splash.TXT");	// <FANS> 2006/11/21 10:49¤W¤È
	m_wndSplash = (CtmWndSplash*)m_parseSplash.GetMainControl();
	if (m_wndSplash != NULL)
	{
		m_wndSplash->CreateA();
		m_wndSplash->Show();
    	//printf("UpdateMMIFile\n");
	}
	g_nCommand=1;
	ChangePos();
	//printf("m_wndSplash = %x\n", m_wndSplash);
	//UpdateMMIFile(m_wndSplash);
	//if(m_wndSplash != NULL)	//LEO20070421
	//{
	//	//tmTIME          timestart;
    //    //tmTIME          timeend;
    //    //int			nMountStatus;
    //    //nMountStatus = mount("/dev/mmcda1", "/mnt/mmc", "vfat", 0, NULL);
    //    //if (nMountStatus == 0)
    //    //{
    //    //    GetTime(&timestart);
    //    //    printf("Start time minute = %d,second = %d\n", timestart.minute, timestart.second);
    //	//	printf("UpdateMMIFile\n");
    //	//	system("cp -fpR /mnt/mmc/i86 /usr");
    //	//	GetTime(&timeend);
    //	//    printf("End time minute = %d,second = %d\n", timeend.minute, timeend.second);
    //	//    printf("minute = %d,second = %d\n", timeend.minute - timestart.minute, timeend.second - timestart.second);
    //	//	umount("/mnt/mmc");
    //	//}
    //	printf("UpdateMMIFile\n");
	//	UpdateMMIFile(m_wndSplash);
	//}
	if( CtmConfig::GetInstance()->GetINETFlag() )
		AddCounter(&m_lSplashTimer, m_idMe);
	
	if(CtmConfig::GetInstance()->GetLogoTime() )
	{
		m_lLogoTime = CtmConfig::GetInstance()->GetLogoTime() * 1000;
		AddCounter(&m_lLogoTime, m_idMe);
	}
	else if(g_wExternLogoTime)
	{
		m_lLogoTime=g_wExternLogoTime*1000;
		AddCounter(&m_lLogoTime, m_idMe);
	}
	
	
	AddCounter(&g_lGifLogoTime, m_idMe);
}

CTaskSplash::~CTaskSplash()
{
	m_parseSplash.CloseFile();
	if( CtmConfig::GetInstance()->GetINETFlag() )
		DeleteCounter(&m_lSplashTimer, m_idMe);	
	
	if(CtmConfig::GetInstance()->GetLogoTime() )
		DeleteCounter(&m_lLogoTime, m_idMe);	
	
	DeleteCounter(&g_lGifLogoTime, m_idMe);	
	UnRegisterAllMessage(m_idMe);
}
    
/*---------------------------------------------------------------------------+
|           Operations                                                       |
+---------------------------------------------------------------------------*/
void        CTaskSplash::Run()
{
	MSG			msg;
	tmDATE		date;       //wy102999

	GetDate(&date);         //wy102999

	if(CtmConfig::GetInstance()->GetLogoTime())
	{
		if (m_lLogoTime <= 0)
		{
			ioctl(g_FbIotcol, 0xFFFF11, NULL);
			usleep(20*1000);
			m_wndSplash->Clear();
			//ioctl(g_FbIotcol, 0xFFFF12, NULL);
	    	msg.wMessage = _SYS_EXIT;
	    	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
		}		
	}
	if (m_wndSplash->IsKeyIn())
    {
    	if( CtmConfig::GetInstance()->GetINETFlag() )
    	{
    		if (m_lSplashTimer <= 0)
    		{
    			ioctl(g_FbIotcol, 0xFFFF11, NULL);
    			usleep(20*1000);
				m_wndSplash->Clear();
				//ioctl(g_FbIotcol, 0xFFFF12, NULL);
		    	msg.wMessage = _SYS_EXIT;
		    	g_pApplication->SetMessage(CMD_SYSTEM, &msg);
			}	
    	}
    	else
    	{

		    ioctl(g_FbIotcol, 0xFFFF11, NULL);
		    usleep(40*1000);
			m_wndSplash->Clear();
			//ioctl(g_FbIotcol, 0xFFFF12, NULL);
			//usleep(50*1000);
		    msg.wMessage = _SYS_EXIT;
		    g_pApplication->SetMessage(CMD_SYSTEM, &msg);
		}
    }
	else
    {                                   
    }                                   
    
   	if(g_pGifLogo != NULL && g_lGifLogoTime <= 0)
    {
    	static int i = 1;
    	gif_animation* 	gifInfo = ((CtmImageGif*)g_pGifLogo)->GetGifInfo();
    	if(gifInfo != NULL)
		{
			if(i <= gifInfo->frame_count)
			{
				ControlGif(g_pGifLogo, i, 1,  gifInfo->frame_count, 1,  gifInfo->frame_count, GIF_FWD);	
				g_lGifLogoTime = gifInfo->frames[i-1].frame_delay*10;
				i++;
			}
			else
			{
				if(CtmConfig::GetInstance()->GetGifType() == 1)
					i =1;
			}
		}
    }
    	
}

/*===========================================================================+
|           Class implementation - CtmWndStart                                 |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmWndStart::CtmWndStart() : CtmWnd()
{


}

CtmWndStart::~CtmWndStart()
{
}




