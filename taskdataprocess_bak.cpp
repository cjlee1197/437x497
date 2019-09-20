/*===========================================================================+
|  Class    : TaskDataProcess                                                |
|  Task     : Command Service Routine cpp file                            	 |
|----------------------------------------------------------------------------|
|  Compile  : gcc V3.2 -                	                                 |
|  Link     : gcc V3.2 -                                                     |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James	                                                         |
|  Version  : V1.00                                                          |
|  Creation : 10/02/2010                                                     |
|  Revision :                                                                |
+===========================================================================*/

#include    "taskdataprocess.h"
#include    "main.h"
/*==========================================================================+
|           Type definition                                                 |
+==========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

IMPLEMENT_DYNCREATE(CTaskDataProcess, CtmPackClass)
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
CTaskDataProcess*	g_pTaskDataProcess = NULL;

/*===========================================================================+
|           Class implementation - TaskCmd                                   |
+===========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CTaskDataProcess::CTaskDataProcess() : CTask(), CtmPackClass()
{
}

CTaskDataProcess::~CTaskDataProcess()
{
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
