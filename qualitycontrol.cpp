#include	"qualitycontrol.h"
#include	"machinefactor.h"
//#include	"memwatch.h"
#include	"tmdebug.h"
int CtmQuality::GetData(int nDataType, int nFunctionType, double* pData)
{
	int nResult = GetMoniCount(nDataType);
	
	//printf("moniCount =%d\n", nResult);
	for (int i = 0; i < nResult; i++)
	{
		*(pData + i) = GetDataS(nDataType, nFunctionType, i + 1);
	}
	return	nResult;
}



double CtmQuality::GetDataS(int nDataType, int nFuctionType, int nIndex)
{
	double	dResult = 0;
	void*	pMoni = NULL;
	pMoni = GetMachineDataType(nDataType, nIndex);
	GetDataBasicFunction(nFuctionType, pMoni, &dResult);
	return dResult;	
}

CtmOilQuality::CtmOilQuality()
{

}

CtmOilQuality::~CtmOilQuality()
{
	
}

void	  CtmOilQuality::Clear()
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			g_ptmControlServer->QualityControl()->Clear();
		}	
	}
}

int			CtmOilQuality::GetMoniCount(int nDataType)											 
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			return g_ptmControlServer->QualityControl()->GetMoniCount(nDataType);
		}	
	}
	return 0;
	
}

double		CtmOilQuality::SetData(int nDataType, int nFunctionType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			return g_ptmControlServer->QualityControl()->SetData(nDataType, nFunctionType);
		}	
	}
	return 0;
}

void		CtmOilQuality::GetDataBasicFunction(int nFunctionType, void* pMachineDataType, 
									double* pSingleOutData)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			g_ptmControlServer->QualityControl()->GetDataBasicFunction(nFunctionType, pMachineDataType, pSingleOutData);
		}	
	}	
}

void*		CtmOilQuality::GetMachineDataType(int nDataType, int nIndex)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			g_ptmControlServer->QualityControl()->GetMachineDataType(nDataType, nIndex);
		}	
	}
}


CtmSimQuality::CtmSimQuality()
{
	
}

CtmSimQuality::~CtmSimQuality()
{
	
}


CtmQualityControl*	CtmQualityControl::m_pInstance = NULL;
	
CtmQuality*	CtmQualityControl::m_pQuality = NULL;
	
 /*! \fn CtmQualityControl::CtmQualityControl() 
 *  \brief 構造函數不能直接調用,請使用GetInstance()
 */
CtmQualityControl::CtmQualityControl()
{
	//int	nType = 0;
	//
	//nType = CtmConfig::GetInstance()->GetMachineType();
	//
	//switch (nType)
	//{
	//	case MACHINE_TYPE_SIM:
	//		m_pQuality = new CtmSimQuality();
	//		break;
	//	case MACHINE_TYPE_OIL:
	//		m_pQuality = new CtmOilQuality();
	//		break;
	//}
	m_pQuality = CtmMachineBuild::GetInstance()->BuildCQuality();
	
}

CtmQualityControl*	CtmQualityControl::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CtmQualityControl;
	return	m_pInstance;
}

CtmQualityControl::~CtmQualityControl()
{
	if (m_pQuality != NULL)
	{
		delete m_pQuality;
		m_pQuality = NULL;
	}
}

void		CtmQualityControl::ReleaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}  
  
int	CtmQualityControl::GetDataCurve(int nDataType, int nFunctionType, void* pData)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			return g_ptmControlServer->QualityControl()->GetDataCurve(nDataType, nFunctionType, pData);
		}	
	}
	return 0;
}

int			CtmQualityControl::GetMoniCount(int	nDataType)
{
	return	m_pQuality->GetMoniCount(nDataType);
}

int			CtmQualityControl::GetData(int nDataType, int nFunctionType, double* pData)
{
	return	m_pQuality->GetData(nDataType, nFunctionType, pData);
}

double 	CtmQualityControl::GetDataS(int nDataType, int nFuctionType, int nIndex)
{
	return	m_pQuality->GetDataS(nDataType, nFuctionType, nIndex);
}

double 	CtmQualityControl::SetData(int nDataType, int nFunctionType)
{
	return	m_pQuality->SetData(nDataType, nFunctionType);
}

char* CtmQualityControl::GetYName()
{
	return NULL;
}


char*	CtmQualityControl::GetXName()
{
	return NULL;
}	

void	 CtmQualityControl::Clear()
{
	m_pQuality->Clear();
}

BOOL	CtmQualityControl::SaveAs(tmStorageType StorageType)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			return g_ptmControlServer->QualityControl()->SaveAs(StorageType);
		}	
	}
	return FALSE;
}

//將監測數據從一個地方拷貝到另外一個地方
BOOL	CtmQualityControl::ReadAs(tmStorageType StorageTypeSrc, tmStorageType StorageTypeDst)
{
	if(g_ptmControlServer != NULL)
	{
		if(g_ptmControlServer->QualityControl() != NULL)
		{
			return g_ptmControlServer->QualityControl()->ReadAs(StorageTypeSrc, StorageTypeDst);
		}	
	}
	return FALSE;
}
