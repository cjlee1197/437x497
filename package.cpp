#include	"package.h"
#include	"tmdebug.h"



const 	char*	CNT_STR_SECTION_NAME	= "lib";
const 	char*	CNT_STR_PACKAGE_CREATE	= "CreateInstance";
const 	char*	CNT_STR_PACKAGE_RELEASE = "ReleaseInstance";


CtmHmiPackage* CtmHmiPackage::m_pInstance = NULL;
CtmHmiPackage*		g_pPackage = NULL;//CtmHmiPackage::CreateInstance();	
/*!	\fn CtmHmiPackage	CtmHmiPackage::CreateInstance()
 *	\brief	獲取本類的實例指針，本類無法通過其他的途徑去獲取實例.
 *		本類的實例指針僅有1個
 *	\return 返回的是本類的實例指針
 *	\note	保證本類的實例僅有一個
*/
CtmHmiPackage*	CtmHmiPackage::CreateInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CtmHmiPackage;
	return m_pInstance;
}


/*!	\fn void	CtmHmiPackage::ReleaseInstance()
 *	\brief	釋放本類的實例
 *	\note	在一般的情況下，不需要釋放本類的實例，僅在退出的時候才會調用.
*/
void	CtmHmiPackage::ReleaseInstance()
{
	delete m_pInstance;
}

/*!	\fn CtmHmiPackage::CtmHmiPackage()
 *	\brief	真正的構造函數.
 *	\note	僅僅被內部使用
*/
CtmHmiPackage::CtmHmiPackage()
{
	
	m_nPackageNum = 0;
	memset(m_pPackageKind, 0, sizeof(m_pPackageKind));
	bool bOpen = m_IniPackage.OpenFile("./lib/configlib.ini");
	if (bOpen)
		g_tmDebugInfo->PrintDebugInfo("Open Configlib.ini is OK!\n");
	
	LoadPackage();
}

/*!	\fn CtmHmiPackage::~CtmHmiPackage()
 *	\brief	真正的析造函數.
 *	\note	僅僅被內部使用
*/
CtmHmiPackage::~CtmHmiPackage()
{
	UnLoadPackage();
	m_IniPackage.CloseFile();
}


/*!	\fn int	CtmHmiPackage::LoadPackage()
 *	\brief	裝載包.
 *	首先是裝載包，接著將包中的兩個函數引用出來：CreateInstance和ReleaseInstance.
 *	\return 裝載合法的包數
 *	\note	需要注意的是裝載的包數不可能是無限制的，本類一共可以裝載1024個包
 */
int	CtmHmiPackage::LoadPackage()
{
	int	nResult = 0;
	
	//	read package's function name from the config
	m_nPackageNum 	= m_IniPackage.ReadSection(CNT_STR_SECTION_NAME, m_pPackageKind, 0);
	m_pPackages 	= new tmPackageInfo[m_nPackageNum];
	
	memset(m_pPackages, 0, sizeof(tmPackageInfo) * m_nPackageNum);
	for (int i = 0; i < m_nPackageNum; i++)
	{
		if (m_pPackageKind[i] != NULL)
		{
			strcpy(m_pPackages[i].szFunctionName, m_pPackageKind[i]);
			g_tmDebugInfo->PrintDebugInfo("%d=%s\n", i, m_pPackages[i].szFunctionName);
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////
	//	read package's name from the config, and open the library by the package's name
	////////////////////////////////////////////////////////////////////////////////////
	char*	pszError;
	m_IniPackage.ReadSectionValues(CNT_STR_SECTION_NAME, m_pPackageKind, 0);
	for (int i = 0; i < m_nPackageNum; i++)
	{
		if (m_pPackageKind[i] != NULL)
			strcpy(m_pPackages[i].szPackageName, m_pPackageKind[i]);
			
		if (m_pPackages[i].szPackageName[0] != '\0');
		{
			//	open library, and save the handle
			m_pPackages[i].pHandle = dlopen(m_pPackages[i].szPackageName, RTLD_LAZY);
			//printf("m_pPackages[i].szPackageName=%s m_pPackages[i].pHandle=%x \n", m_pPackages[i].szPackageName, m_pPackages[i].pHandle);
			pszError = dlerror();	//	需要檢測錯誤
			if (pszError == NULL)
			{
				//	get the CreateInstance function pointer in the opened library
				m_pPackages[i].pCreate = (CObject* (*) (char* pszName))dlsym(m_pPackages[i].pHandle, CNT_STR_PACKAGE_CREATE);
				pszError = dlerror();
				if (pszError != NULL)
					g_tmDebugInfo->PrintDebugInfo("Load package %s, Get create function error:%s\n", 
													m_pPackages[i].szPackageName, pszError);
				
				//	get the ReleaseInstance function pointer in the opened library
				m_pPackages[i].pRelease = (void (*) (CObject* pInstance))dlsym(m_pPackages[i].pHandle, CNT_STR_PACKAGE_RELEASE);
				pszError = dlerror();
				if (pszError != NULL)
					g_tmDebugInfo->PrintDebugInfo("Load package %s, Get release function error:%s\n", 
													m_pPackages[i].szPackageName, pszError);
				nResult++;
			}
			else
			{
				g_tmDebugInfo->PrintDebugInfo("Load Package %s Error %s\n", 
												m_pPackages[i].szPackageName, pszError);
			}		
			
		}
	}
	return nResult;
}


/*!	\fn int	CtmHmiPackage::UnLoadPackage()
 *	\brief	卸載包.
 *	其本質就是將所有的library關閉.
 *	\return 卸載包的書目
 */
int	CtmHmiPackage::UnLoadPackage()
{
	int	nResult = 0;
	
	for (int i = 0; i < m_nPackageNum; i++)
	{
		if (m_pPackages[i].pHandle != NULL)
		{
			dlclose(m_pPackages[i].pHandle);
			nResult++;
		}
		
	}
	
	delete []m_pPackages;
	return nResult;
}



/*!	\fn CObject* 	CtmHmiPackage::CreateClassInstance(char* pszName)
 *	\brief	根據給定的功能名稱創建類的實例（此類是實現該項功能的）.
 *	根據功能名稱創建的類，一般其實現都在so中.
 *	\param	[in]	pszName	功能的名稱，在config中就是=左側的值
 *	\param	[i]		pszName1 子功能選項
 *	\return 創建出來類實例的指針
 *	\note	需要注意所有可創建的類，其必須是CObject的派生類
 */
CObject*	CtmHmiPackage::CreateClassInstance(char* pszName, char* pszName1)
{
	CObject* pResult = NULL;
	char	 szClassName[256];
	memset(szClassName, 0, sizeof(szClassName));
	//printf("pszName = %s\n",pszName);
	for (int i = 0; i < m_nPackageNum; i++)
	{
		if (strcmp(m_pPackages[i].szFunctionName, pszName) == 0)
		{
			if (pszName1 == NULL) 
				ParseClassName(m_pPackages[i].szPackageName, szClassName);
			else 
				strncpy(szClassName, pszName1, sizeof(szClassName));
			//printf("szClassName = %s\n",szClassName);
			if (m_pPackages[i].pCreate != NULL)
				pResult = m_pPackages[i].pCreate(szClassName);	
			
			//printf("szClassName =%s CObject pResult=%x \n", szClassName, pResult);
			break;
		}
	}
	
	return pResult;
}

/*!	\fn void		CtmHmiPackage::ReleaseClassInstance(char* pszName, CObject* pInstance)
 *	\brief	根據給定的功能名稱釋放類的實例（此類是實現該項功能的）.
 *	與CreateClassInstance配對使用，根據功能名稱釋放類的實例，一般其實現都在so中.
 *	\param	[in]	pszName	功能的名稱，在config中就是=左側的值
 *	\param	[in]	pInstance 需要釋放的實例指針.
 *	\note	需要注意所有釋放的類，其必須是CObject的派生類
 */
void		CtmHmiPackage::ReleaseClassInstance(char* pszName, CObject* pInstance)
{
	for (int i = 0; i < m_nPackageNum; i++)
	{
		if (strcmp(m_pPackages[i].szFunctionName, pszName) == 0)
		{
			if (m_pPackages[i].pRelease != NULL)
				m_pPackages[i].pRelease(pInstance);
			break;
		}
	}
}

/*!	\fn void		CtmHmiPackage::ParseClassName(char* pszInput, char* pszOut)
 *	\brief	根據so的名稱解析出類名.
 *	\param	[in]	pszInput	so的名稱（library的名稱）
 *	\param	[out]	pszOut 解析出來的類的名稱
 *	\note	需要注意的是：其so的名稱與包中實現類的名稱應該符合以下標準：C+PackageName-.so=類名.
 */
void		CtmHmiPackage::ParseClassName(char* pszInput, char* pszOut)
{
	if (pszOut == NULL) return;	
	pszOut[0] = 'C';
	
	int 	j 		= 1;
	char*	pszDest = NULL;	
	pszDest = strrchr(pszInput, '/');
	
	if (pszDest == NULL) 
		pszDest = pszInput;
		
	int	nStrNum = strlen(pszDest);
	
	for (int i = 1; i < nStrNum; i++)
	{
		if (pszDest[i] != '.')
		{
			pszOut[j++] = pszDest[i];
		}
		else break;
	}
}


