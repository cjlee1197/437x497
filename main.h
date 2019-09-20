/*===========================================================================+
|  System   : Injection Machine Control System                               |
|  Task     : Injection Machine Control System header file                   |
|----------------------------------------------------------------------------|
|  Compile  :                                                                |
|  Link     :                                                                |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 03/23/1996                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_MAIN
#define     D_MAIN


#include    "common.h"
#include    "app.h"
#include    "mainfrm.h"
#include    "taskcmd.h"
#include	"taskdblog.h"
//#include	"taskdsp.h"
#include	"parsepule.h"
#include	"taskhost.h"
#include	"tmcontrol.h"
#include	"taskcnc.h"

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/

/*===========================================================================+
|           Type definition                                                  |
+===========================================================================*/

/*===========================================================================+
|           Function and Class prototype                                     |
+===========================================================================*/

/*===========================================================================+
|           External                                                         |
+===========================================================================*/

extern      CApp*               g_pApplication; // Application processor
extern      CtmMainFrame*		g_pMainFrame;   // Main window
extern      CTaskCmd*           g_ptaskCmd;     // Command service
extern      CTaskComm*          g_ptaskComm;    // Communication service
extern		CTaskDBLog*			g_ptaskDBLog;
extern      CTaskDateTime*      g_ptaskDateTime;// DateTime service
extern      CTaskCNC*			g_ptaskCNC;
//extern		CTaskSiem*			g_ptasksiem;	//yang 2006/6/15 
//extern		CTaskI32*			g_ptaski32;		//yang 2006/6/15 
//extern		CTaskRS232*			g_ptaskrs232;
//extern		CTaskKeb*			g_ptaskkeb;		//yang 2006/12/17
//extern		CTaskiNet*			g_ptaskinet;	//zholy070416


//extern		CTaskDsp28*			g_ptaskdsp28;					//	fans add 2008/3/9 01:09¤W¤È
extern		CParseObject*  		g_ParseMainFrame;				//	fans add

#endif

