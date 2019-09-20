/*==========================================================================+
|  Class    : main.cpp														|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : 童宇揚                                                        |
|  Version  : V1.00                                                         |
|  Creation : 2004/05/11                                                    |
|  Remark	: 																|
+==========================================================================*/
#include	"init.h"
#include	"main.h"
#include	"utils.h"
#include    "taskmoni.h"

#include	"mainfrm.h"
#include	"app.h"
#include	"splash.h"
#include	"stable.h"
#include	"parsepule.h"
#include	"tmconfig.h"
#include	"sram.h"

#include	"package.h"								//	fans add 2008-1-17 9:20
#include	"selvat_ancestor.h"
#include	"taskhost.h"

#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <execinfo.h>
#include <sys/time.h>   
#include <time.h> 
#include <sys/sysinfo.h>


CtmMainFrame*       g_pMainFrame = NULL;			// Main window

CParseObject*  		g_ParseMainFrame = NULL;	
                                                	
CApp                g_Application;      			// Application processor
CApp*               g_pApplication	= NULL;     	// Application processor
CTaskCmd*           g_ptaskCmd		= NULL;			// Command service
//CTaskMoni*          g_ptaskMoni		= NULL;        	// System monitor service
CTaskComm*         	g_ptaskComm		= NULL;        	// Communication service
CTaskDateTime*		g_ptaskDateTime	= NULL;
CTaskDBLog*			g_ptaskDBLog	= NULL;
CTaskCNC*						g_ptaskCNC				= NULL;
//CTaskSiem*			g_ptasksiem		= NULL;
//CTaskI32*			g_ptaski32		= NULL;
//CTaskRS232*			g_ptaskrs232	= NULL;
//CTaskKeb*			g_ptaskkeb		= NULL;
//CTaskiNet*			g_ptaskinet     = NULL;         //zholy070416
//CTaskDsp28*			g_ptaskdsp28	= NULL;			//Fans add	2008/3/9 01:09上午
pthread_mutex_t  	g_setFlagLock=PTHREAD_MUTEX_INITIALIZER;
pthread_t			g_threadMain;
const int DATETIM_INDEX = 0;


void dump(int signo)
{
       tmDATE    date;
       tmTIME    time;
       void *array[100];
       size_t size;
       char **strings;
       size_t i;
       char sz[1024];
       char ps[1];
       ps[0]='\n';
       GetDate(&date);
       GetTime(&time);
       char* sigstr=strsignal(signo);
       int fd=open("/usr/debug",O_RDWR|O_CREAT|O_APPEND,0x666);
       if(fd<0)
          exit(0);
       
       size = backtrace (array, 100);
       strings = backtrace_symbols (array, size);
       sprintf(sz, "%04u.%02u.%02u/%02u:%02u: ", date.year, date.month, date.day, time.hour, time.minute);
       sprintf (sz+19,"Obtained %zd stack frames.\n", size);
       write(fd,sz,strlen(sz));
       write(fd,ps,1);
       for (i = 0; i < size; i++)
        //printf ("%s\n", strings[i]);
       {
               write(fd,strings[i],strlen(strings[i]));
               write(fd,ps,1);
       }
       free (strings);
       write(fd,sigstr,strlen(sigstr));
       write(fd,ps,1);
       close(fd);
       exit(0);
}


void dump2(int signo)
{
	printf("dump2 \n");
}

int main()
{
	printf("main Run123\n");
	signal(SIGSEGV, &dump);
	signal(SIGALRM, SIG_IGN);
	signal(SIGIO, SIG_IGN);
	//signal(SIGABRT,&dump);
	g_bInitDone = FALSE;
	g_pPackage 	= CtmHmiPackage::CreateInstance();	
	//g_pSelvat 	= static_cast<CtmVat_Ancestor*>(g_pPackage->CreateClassInstance("Selvat"));
	g_pApplication	= &g_Application;
	g_threadMain	= pthread_self();				// 取主線程ID 
	g_ptmControlServer 	= static_cast<CtmControlServer*>(g_pPackage->CreateClassInstance("ControlServer"));
	g_pInit 			= static_cast<CInit*>(g_pPackage->CreateClassInstance("HostInit"));
//	#ifdef    TCP_HMI_NC
//	g_ptaskCncComm 	= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("HostCncComm"));
//	#endif
	//g_ptaskpicker	= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("HostPicker"));
	g_ptaskCmd 			= static_cast<CTaskCmd*>(g_pPackage->CreateClassInstance("HostCmd"));
	//Init();
	
	//g_ptaskCmd  	= new CTaskCmd;					// Command service
	//g_ptaskDateTime = new CTaskDateTime; 
	 
	//if( CtmConfig::GetInstance()->GetTempCurveFlag() )	//Temp Curve
	//	g_ptaskDBLog   	= new CTaskDBLog;
	//g_ptaskrs232	= new CTaskRS232;
	//if( CtmConfig::GetInstance()->GetDSP28Flag() )
	//	g_ptaskdsp28		= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host0"));
	//if(0 == CtmConfig::GetInstance()->GetCommType())
	//	g_ptaskdsp			= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host1"));
	//else
	//	g_ptaskdsp			= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host4"));
	//if( CtmConfig::GetInstance()->GetDspCom2Flag() )		//Dsp com2 comm
	//	g_ptaskCom2dsp			= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host2"));
	//if( CtmConfig::GetInstance()->GetINETFlag() )			//Ethnet comm
    //	g_ptaskinet	= static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host3")); 
    
	//if( CtmConfig::GetInstance()->GetAustoneFlag() )		//for austone James add 2010/5/19
	//	g_ptaskdsp28_Austone = static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host5"));
	
	//if( CtmConfig::GetInstance()->GetN2CFlag() )		//for n2c Caozz add 2010/10/22 08:52上午
	//	g_ptaskdsp28_N2C = static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host6"));
	//if( CtmConfig::GetInstance()->GetM3Flag())
	//	g_ptaskdsp28_M3 = static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host9"));
	
	//if( CtmConfig::GetInstance()->GetRemoteFlag() )		
	//	g_ptaskremote = static_cast<CTaskHost*>(g_pPackage->CreateClassInstance("Host7"));
	//g_bInitDone = TRUE;
	//printf("CTaskCNC\n");
	g_ptaskCNC = new CTaskCNC;
	//printf("CTaskCNC over\n");
	//printf("g_ptaskremote = %x\n", g_ptaskremote);
	//	fans add 2008/12/10 11:23上午
	if (CtmConfig::GetInstance()->GetLogoFlag() == 1)
	{
		CTaskSplash*    ptaskSplash = new CTaskSplash;
	
		AssignCommandTarget(ptaskSplash->m_wndSplash);
		SetBypass(TRUE);
		#ifdef      D_DEMO
			PutKeyTail(_F5);
		#endif
	    g_pApplication->Run();							// Stay there to wait for user's input and time due
		::ReleaseCommandTarget(ptaskSplash->m_wndSplash);
		delete ptaskSplash;
		SetBypass(FALSE);
	}
	
	OpenMainForm();				//	fans	2006/12/6 05:54下午
	
	//g_ptaskMoni     = new CTaskMoni;				// System monitor service
	g_ptaskMoni 		= static_cast<CTaskMoni*>(g_pPackage->CreateClassInstance("HostMoni"));
	
	#ifdef      D_DEMO
	PutKeyTail(_F1);
	PutKeyTail(_F1);
	#endif
	g_pApplication->Run();
	ReleaseCommandTarget(g_pMainFrame);
	
	CtmConfig::ReleaseInstance();
	
	//delete g_ptaskinet; // zholy070416
	//delete g_ptaskdsp;
	//delete g_ptaskdsp28;
	//delete g_ptaskDBLog;
	//delete g_ptaskMoni;
	extern		CParseObject*		g_ParseView[20];
	for(int i = 0; i < 20; i++)
	{
		if(g_ParseView[i] != NULL)
			delete g_ParseView[i];	
	}
	delete g_ptaskDateTime;
	delete g_ParseMainFrame;
	//delete g_ptaskCmd;
	g_pInit->Destroy();
	g_pPackage->ReleaseClassInstance("HostCmd", g_ptaskCmd);
	g_pPackage->ReleaseClassInstance("Host1", g_ptaskdsp);
	g_pPackage->ReleaseClassInstance("Host2", g_ptaskCom2dsp);
	g_pPackage->ReleaseClassInstance("Host3", g_ptaskinet);
	g_pPackage->ReleaseClassInstance("Selvat", g_pSelvat);
	if( CtmConfig::GetInstance()->GetAustoneFlag() )
		g_pPackage->ReleaseClassInstance("Host5", g_ptaskdsp28_Austone);
	g_pPackage->ReleaseInstance();
	return 0;

}
