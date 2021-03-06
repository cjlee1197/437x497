/*===========================================================================+
|  Class    : CtmControlServer_lib	                                         |
|  Task     : All the muster Control Service	    	                     |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : James		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 05/11/2008	                                                 |
|  Revision : 05/11/2008  				                                     |
+===========================================================================*/
#include	"tmControlServer_lib.h"

IMPLEMENT_DYNCREATE(CtmControlServer_lib, CtmControlServer)

/*===========================================================================+
|           Constants                                                        |
+===========================================================================*/

/*===========================================================================+
|           Type	                                                         |
+===========================================================================*/

/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/

char*	u_aryControlServicePackageName[] = 
{
	"ControlServer_Error",
	"ControlServer_Monitor",
	"ControlServer_Mold",
	"ControlServer_Quality",
	"ControlServer_User",
	"ControlServer_CurveData",
	"ControlServer_Function",
	"ControlServer_Oper_Log"	,
	"ControlServer_Other"	
};

/*===========================================================================+
|           Function implementation                                          |
+===========================================================================*/	
/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::CtmControlServer()				             |
|  Task     :  CtmControlServer�c�y���                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControlServer_lib::CtmControlServer_lib(): CtmControlServer()
{
	
}
/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::~CtmControlServer()				             |
|  Task     :  CtmControlServer�R�c���                                      |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
CtmControlServer_lib::~CtmControlServer_lib()
{
		
}

/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::CreateSelf()				             	 |
|  Task     :  Initialize all service resources                              |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControlServer_lib::CreateSelf()
{
	for(unsigned int i = 0; i < sizeof(u_aryControlServicePackageName) / sizeof(char*); i++)
	{
		m_pControl[i] = static_cast<CtmControl*>(g_pPackage->CreateClassInstance(u_aryControlServicePackageName[i]));	
		//printf("u_aryControlServicePackageName[i] =%s m_pControl=%x \n",u_aryControlServicePackageName[i],  m_pControl[i] );
	}
	
	CreateControl();		//call the base class function
	return 0;
}

/*---------------------------------------------------------------------------+
|  Function : CtmControlServer::FreeSelf()				             	     |
|  Task     :  Release all service resources                                 |
+----------------------------------------------------------------------------+
|  Call     :                                                                |
|                                                                            |
|  Parameter:                           -                                    |
|                                                                            |
|  Return   :                           -                                    |
+---------------------------------------------------------------------------*/
int		CtmControlServer_lib::FreeSelf()
{
	for(unsigned int i = 0; i < sizeof(u_aryControlServicePackageName) / sizeof(char*); i++ )
	{
		g_pPackage->ReleaseClassInstance(u_aryControlServicePackageName[i], static_cast<CObject*>(m_pControl[i]));
	}
	
	DestroyControl();		//call the base class function
	return 0;
}


#ifndef	D_BEOBJECT_CTMCONTROLSERVER_LIB
CObject*	CreateInstance(char* Name)
{
//	g_tmDebugInfo->PrintDebugInfo("CMotionOil5AxisG::CreateInstance!\n");
	if (g_ptmControlServer == NULL)
	{
		CObject*	pResult = Create(Name);
		if (pResult != NULL)
		{
			(static_cast<CtmControlServer_lib*>(pResult))->CreateSelf();
			g_ptmControlServer = static_cast<CtmControlServer_lib*>(pResult);
		}
	}
	return g_ptmControlServer;
}

void	ReleaseInstance(CObject* pInstance)
{
	if (pInstance != NULL)
		(static_cast<CtmControlServer_lib*>(pInstance))->FreeSelf();
	delete pInstance;
	pInstance = NULL;
}

#endif
