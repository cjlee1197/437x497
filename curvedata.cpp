/* Copyright (C), 2004-2009, Techmation. Co., Ltd.*/ 

/*===========================================================================+
|  Class    : CtmCurveOilData and CtmCurveDataControl etc		             |
|  Task     : control the curve data				                         |
|----------------------------------------------------------------------------|
|  Compile  : G++(GCC)3.2                                                    |
|  Link     : G++(GCC)3.2                                                    |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : fans		                                                     |
|  Version  : V1.00                                                          |
|  Creation : 			                                                     |
|  Revision : 							                                     |
+===========================================================================*/
#include	"curvedata.h"
#include	"machinefactor.h"
#include	"utils.h"
#include	"tmdebug.h"
//#include	"memwatch.h"
/*===========================================================================+
|           Global variable                                                  |
+===========================================================================*/
//=======
//      Initialize static class member
//=======
CtmCurveDataControl*	CtmCurveDataControl::m_pInstance = NULL;
CtmCurveData*			CtmCurveDataControl::m_pCurveData = NULL;	
/*==========================================================================+
|           Class implementation - CtmCurveOilData                          |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmCurveOilData::CtmCurveOilData()
{

}


CtmCurveOilData::~CtmCurveOilData()
{
	
}

/*--------------------------------------------------------------------------+
|			Operations										 			 	|
+--------------------------------------------------------------------------*/
int		CtmCurveOilData::SetData(int nType, double* dX, double* dY, int nNum)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->SetData(nType, dX, dY, nNum);
	}
	return 0;
}

int		CtmCurveOilData::SetData(int	nType, DWORD* dX, DWORD* dY, int nNum)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->SetData(nType, dX, dY, nNum);
	}
	return 0;	
}

int		CtmCurveOilData::SetData(int	nType, void* pInputData, int nNum)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->SetData(nType, pInputData, nNum);
	}
	return 0;	
}

int		CtmCurveOilData::GetData(int nType, void* pOutData,int TempDataIndex)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->GetData(nType, pOutData, TempDataIndex);
	}
	return 0;	
}

void	CtmCurveOilData::SetComplete(int nType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->SetComplete(nType);
	}
}

int		CtmCurveOilData::SetState(int nType, int nState)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->SetState(nType, nState);
	}
	return 0;
}

int		CtmCurveOilData::GetState(int nType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->GetState(nType);
	}
	return 0;
}

void    CtmCurveOilData::ClearBuffer(int nType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->CurveDataControl() != NULL)
			return g_ptmControlServer->CurveDataControl()->ClearBuffer(nType);
	}
}

/*==========================================================================+
|           Class implementation - CtmCurveDataControl                       |
+==========================================================================*/
/*---------------------------------------------------------------------------+
|           Constructor and destructor                                       |
+---------------------------------------------------------------------------*/
CtmCurveDataControl::CtmCurveDataControl()
{
	//int	nMachineType = CtmConfig::GetInstance()->GetMachineType();
	//switch (nMachineType)
	//{
	//	case MACHINE_TYPE_SIM:			//	¥þ¹qSim
	//		break;
	//	case MACHINE_TYPE_OIL:			//	ªoÀ£J6
	//		m_pCurveData = new CtmCurveOilData();
	//		break;
	//}
//	printf("Create CurveData!\n");
	m_pCurveData = CtmMachineBuild::GetInstance()->BuildCCurveData();
}


CtmCurveDataControl::~CtmCurveDataControl()
{
	if (m_pCurveData != NULL)	
	{
		delete m_pCurveData;
		m_pCurveData = NULL;
	}
}

CtmCurveDataControl*	CtmCurveDataControl::GetInstance()
{
	
	if (m_pInstance == NULL)	
	{
		m_pInstance = new CtmCurveDataControl();
	}
	return m_pInstance;
}

void					CtmCurveDataControl::ReleaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

/*--------------------------------------------------------------------------+
|  Function :  SetData(int nType, double* dX, double* dY, int nNum)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmCurveDataControl::SetData(int nType, double* dX, double* dY, int nNum)
{	
    //printf("CtmCurveDataControl::SetData %d, dX=%f, dY=%f, Num=%d\n", nType, *dX, *dY, nNum);
	return m_pCurveData->SetData(nType, dX, dY, nNum);
	
}

/*--------------------------------------------------------------------------+
|  Function :  SetData(int nType, DWORD* dX, DWORD* dY, int nNum)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmCurveDataControl::SetData(int nType, DWORD* dX, DWORD* dY, int nNum)
{
    //printf("CtmCurveDataControl::SetData %d, dX=%d, dY=%d, Num=%d\n", nType, *dX, *dY, nNum);
	return m_pCurveData->SetData(nType, dX, dY, nNum);
}

/*--------------------------------------------------------------------------+
|  Function :  SetData(int nType, void* pInputData, int nNum)|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmCurveDataControl::SetData(int nType, void* pInputData, int nNum)
{
	return m_pCurveData->SetData(nType, pInputData, nNum);
}

/*--------------------------------------------------------------------------+
|  Function :  GetData(int nType, void* pOutData)		|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmCurveDataControl::GetData(int nType, void* pOutData,int TempDataIndex)
{
	return m_pCurveData->GetData(nType, pOutData,TempDataIndex);
}

/*--------------------------------------------------------------------------+
|  Function :  SetComplete(int nType)					|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void	CtmCurveDataControl::SetComplete(int nType)
{
	return m_pCurveData->SetComplete(nType);	
}

/*--------------------------------------------------------------------------+
|  Function :  SetState(int nType, int nState)			|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmCurveDataControl::SetState(int nType, int nState)
{
	return m_pCurveData->SetState(nType, nState);	
}

/*--------------------------------------------------------------------------+
|  Function :  GetState(int nType)						|	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
int		CtmCurveDataControl::GetState(int nType)
{
	return m_pCurveData->GetState(nType);
}
/*--------------------------------------------------------------------------+
|  Function :  GetState(int nType)						                    |	
|  Task     :                                                               |
+---------------------------------------------------------------------------+
|  Call     :                                                               |
|                                                                           |
|  Parameter:                           -                                   |
|                                                                           |
|  Return   :                           -                                   |
+--------------------------------------------------------------------------*/
void    CtmCurveDataControl::ClearBuffer(int nType)
{
    m_pCurveData->ClearBuffer(nType);
}